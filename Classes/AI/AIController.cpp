#include "AI/AIController.h"
#include "AI/HeroHooks.h"
#include "AI/TeamBrain.h"
#include "AI/BuildBook.h"
#include "Characters.h"
#include "GameLayer.h"
#include <algorithm>

namespace ai
{

namespace
{

bool inMoveState(ActionManager* unit)
{
    auto state = unit->getActionState();
    return state == ACTION_STATE_IDLE || state == ACTION_STATE_WALK || state == ACTION_STATE_ATTACK;
}

bool isDown(ActionManager* unit)
{
    auto state = unit->getActionState();
    return state == ACTION_STATE_KOCKDOWN || state == ACTION_STATE_FLOAT || state == ACTION_STATE_AIRHURT;
}

bool hasMonster(ActionManager* unit, CharacterId id)
{
    if (!unit->getMonsterArray())
        return false;
    CCObject* object;
    CCARRAY_FOREACH(unit->getMonsterArray(), object)
    {
        if (((ActionManager*)object)->isCharacter(id))
            return true;
    }
    return false;
}

abType toAbType(Slot slot)
{
    switch (slot)
    {
    case Slot::Skill1: return SKILL1;
    case Slot::Skill2: return SKILL2;
    case Slot::Skill3: return SKILL3;
    case Slot::Ougis1: return OUGIS1;
    case Slot::Ougis2: return OUGIS2;
    default: return NAttack;
    }
}

// valid enemy hero for targeting (same filter as ActionManager::findEnemy2)
bool targetable(ActionManager* self, ActionManager* unit, GameLayer* game)
{
    if (!isLaneHero(unit) || !self->isEnemy(unit) || unit->getActionState() == ACTION_STATE_DEAD || !unit->_isVisable ||
        unit->_isWudi)
        return false;
    float tile = game->currentMap->getTileSize().width;
    float unitX = unit->getPositionX();
    if (unitX < tile * 3 || unitX > (game->currentMap->getMapSize().width - 3) * tile)
        return false;
    return fabsf(unitX - self->getPositionX()) < kGameWidth / 2;
}

}  // namespace

AIController* AIController::create(Hero* hero)
{
    if (hero->role() != Role::Com && hero->role() != Role::Player)
        return nullptr;  // clones and summons keep their small legacy routines
    const char* name = hero->getCharacter()->getCString();
    const Profile* profile = findProfile(name);
    if (!profile)
        profile = defaultProfile(name);
    return profile ? new AIController(hero, profile) : nullptr;
}

AIController::AIController(Hero* hero, const Profile* profile)
    : _hero(hero), _profile(profile), _game(hero->getDelegate())
{
    _params = &tuning();
    _nextThink = randFloat(0, _params->thinkMax);  // desync the squad
}

bool AIController::canAct() const
{
    return inMoveState(_hero);
}

CCPoint AIController::deltaTo(ActionManager* target) const
{
    if (target->_originY)
        return ccpSub(ccp(target->getPositionX(), target->_originY), _hero->getPosition());
    return ccpSub(target->getPosition(), _hero->getPosition());
}

bool AIController::walkTo(const CCPoint& delta, int rangeX, int rangeY, bool speedGear)
{
    if (fabsf(delta.x) <= rangeX && fabsf(delta.y) <= rangeY)
        return false;
    if (speedGear && _hero->_isCanGear00)
        _hero->useGear(gear00);
    _hero->walk(ccpNormalize(delta));
    return true;
}

void AIController::strike(ActionManager* target, int type)
{
    if (target)
        _hero->changeSide(deltaTo(target));
    _hero->attack((abType)type);
}

void AIController::tick(float dt)
{
    // Movement and attacks are re-evaluated every tick like the original AI:
    // walk() keeps going until the next command, so skipping ticks made bots
    // overshoot their target and never get in range. Reaction time only
    // applies to switching targets (here) and to casting skills (ruleReady).
    _time += dt;
    switch (_hero->getActionState())
    {
    case ACTION_STATE_IDLE: _activity[0]++; break;
    case ACTION_STATE_ATTACK: _activity[1]++; break;
    case ACTION_STATE_SATTACK:
    case ACTION_STATE_OATTACK:
    case ACTION_STATE_O2ATTACK: _activity[2]++; break;
    case ACTION_STATE_WALK:
        // flipped = facing left; Konoha's base is on the left
        _activity[(_hero->_isFlipped == (_hero->group() == Group::Konoha)) ? 4 : 3]++;
        break;
    default: _activity[5]++; break;
    }
    bool think = _time >= _nextThink;
    _thinking = think;
    if (think)
        _nextThink = _time + randFloat(_params->thinkMin, _params->thinkMax);

    _hero->_mainTarget = nullptr;
    if (_profile->idleWhenBuff && _hero->_skillChangeBuffValue)
        return;
    if (!_profile->hook.empty())
    {
        if (PreTick hook = findHook(_profile->hook))
            if (hook(*this))
                return;
    }

    _hero->_mainTarget = pickHeroTarget(think);
    if (survive())
        return;
    buyGear();
    if (dodge())
        return;
    if (defendBase())
        return;
    if (fightHero())
        return;
    if (farm())
        return;
    advance();
}

std::string AIController::activitySummary() const
{
    int total = 0;
    for (int count : _activity)
        total += count;
    if (!total)
        return "";
    const char* keys = "iasfbh";
    std::string out;
    for (int i = 0; i < 6; i++)
        out += CCString::createWithFormat("%c%d", keys[i], _activity[i] * 100 / total)->getCString();
    return out;
}

// ---------------------------------------------------------------------------
// targeting

ActionManager* AIController::pickHeroTarget(bool think)
{
    // also refreshes enemy/friend combat points for the retreat check
    _hero->findEnemy2("Hero");
    ActionManager* nearest = _hero->_mainTarget;
    if (!nearest)
    {
        _committed = nullptr;
        return nullptr;
    }

    bool committedOk = _committed && _game->_CharacterArray->containsObject(_committed) &&
                       targetable(_hero, _committed, _game);
    if (committedOk && (_time < _commitUntil || !think))
        return _committed;
    if (!think && !_committed)
        return nearest;  // first contact: react on the next decision

    ActionManager* choice = nearest;
    const TeamBrain& team = TeamBrain::get(_game, _hero->group());
    if (_params->focusFire && team.focus && targetable(_hero, team.focus, _game))
        choice = team.focus;

    if (role() == HeroRole::Assassin)
    {
        // assassins pick the weakest hero in reach
        float best = choice->getHpPercent();
        CCObject* object;
        CCARRAY_FOREACH(_game->_CharacterArray, object)
        {
            ActionManager* unit = (ActionManager*)object;
            if (targetable(_hero, unit, _game) && unit->getHpPercent() < best - 0.15f)
            {
                best = unit->getHpPercent();
                choice = unit;
            }
        }
    }
    else if (role() == HeroRole::Tank && nearest->isHurtingTower)
    {
        choice = nearest;  // tanks peel whoever is on the tower
    }

    _committed = choice;
    _commitUntil = _time + randFloat(1.2f, 2.0f);
    return choice;
}

// ---------------------------------------------------------------------------
// survival

bool AIController::survive()
{
    Hero* hero = _hero;
    float hp = hero->getHpPercent();

    // substitution jutsu out of a combo
    bool beingHit = isDown(hero) || hero->getActionState() == ACTION_STATE_HURT;
    if (_profile->panicGear06 && hero->_isCanGear06 && beingHit && hp < 0.5f && !hero->_isBati && !hero->_isWudi)
        hero->useGear(gear06);

    // Turtle Shell Seal (gear11): brace when hurt with an enemy hero close by
    if (hero->_isCanGear11 && hp < _params->sealHp && enemyHeroNear(_params->sealRange))
        hero->useGear(gear11);

    float potionAt = _params->potionHp + (role() == HeroRole::Support ? 0.05f : 0.0f);
    int keep = hp < _params->retreatHp ? 0 : gearReserve();
    if (hero->_isCanItem1 && hero->getCoin() >= 50 + keep && !hero->_isHealling &&
        (hp < potionAt || hero->getMaxHP() - hero->getHP() >= 3000 + hero->gearRecoverValue + 2000))
        hero->setItem(Item1);

    if (hero->_isControled)
        return false;

    float retreatAt = _params->retreatHp;
    if (role() == HeroRole::Tank)
        retreatAt -= 0.05f;
    else if (role() == HeroRole::Assassin || role() == HeroRole::Support || role() == HeroRole::Ranged)
        retreatAt += 0.05f;

    // back to the lane once a potion or the base fountain gave some HP back
    if (hp < retreatAt)
        _retreating = true;
    else if (hp >= retreatAt + 0.2f)
        _retreating = false;

    if (!_retreating)
        return false;
    if (hero->_isHealling)
    {
        if (canAct())
            hero->idle();
        return true;
    }
    if (!canAct())
        return true;
    bool moved = hero->_mainTarget ? hero->stepBack2() : hero->stepBack();
    if (!moved)
        hero->idle();
    return true;
}

bool AIController::enemyHeroNear(float distance) const
{
    Hero* hero = _hero;
    CCObject* object;
    CCARRAY_FOREACH(_game->_CharacterArray, object)
    {
        ActionManager* unit = (ActionManager*)object;
        if (isLaneHero(unit) && hero->isEnemy(unit) && unit->getActionState() != ACTION_STATE_DEAD &&
            fabsf(unit->getPositionX() - hero->getPositionX()) <= distance)
            return true;
    }
    return false;
}

bool AIController::dodge()
{
    if (_params->dodgeChance <= 0 || !canAct())
        return false;
    if (_time < _dodgeUntil)
    {
        _hero->stepBack2();
        return true;
    }
    CCObject* object;
    CCARRAY_FOREACH(_game->_CharacterArray, object)
    {
        ActionManager* unit = (ActionManager*)object;
        if (!isLaneHero(unit) || !_hero->isEnemy(unit))
            continue;
        auto state = unit->getActionState();
        bool casting = state == ACTION_STATE_OATTACK || state == ACTION_STATE_O2ATTACK;
        if (!casting)
        {
            _dodgeRolled.erase(unit);
            continue;
        }
        CCPoint delta = ccpSub(unit->getPosition(), _hero->getPosition());
        if (fabsf(delta.x) > 160 || fabsf(delta.y) > 64 || _dodgeRolled[unit])
            continue;
        _dodgeRolled[unit] = true;
        if (randChance(_params->dodgeChance))
        {
            _dodgeUntil = _time + randFloat(0.4f, 0.8f);
            _hero->stepBack2();
            return true;
        }
    }
    return false;
}

bool AIController::defendBase()
{
    Hero* hero = _hero;
    if (!_profile->baseCheck || hero->_isControled)
        return false;

    const TeamBrain& team = TeamBrain::get(_game, hero->group());
    if (team.towerThreat && targetable(hero, team.towerThreat, _game) && role() != HeroRole::Assassin)
    {
        hero->_mainTarget = team.towerThreat;
        return false;  // fightHero takes it from here
    }

    if (!hero->isBaseDanger)
        return false;
    ActionManager* keep = hero->_mainTarget;
    if (!hero->checkBase())
    {
        hero->_mainTarget = keep;
        return false;
    }
    // checkBase put the intruder in _mainTarget; fall back toward the base first
    float heroX = hero->getPositionX();
    bool away = hero->group() == Group::Akatsuki ? heroX < BASE_X_AKATSUKI : heroX > BASE_X_KONOHA;
    if (away && canAct())
        return hero->stepBack2();
    return false;
}

// ---------------------------------------------------------------------------
// rules

bool AIController::slotReady(Slot slot) const
{
    Hero* hero = _hero;
    switch (slot)
    {
    case Slot::NAttack: return true;
    case Slot::Skill1: return hero->_isCanSkill1;
    case Slot::Skill2: return hero->_isCanSkill2;
    case Slot::Skill3: return hero->_isCanSkill3;
    case Slot::Ougis1: return hero->_isCanOugis1 && !hero->_isControled;
    case Slot::Ougis2: return hero->_isCanOugis2 && !hero->_isControled && _game->_isOugis2Game;
    case Slot::Gear00: return hero->_isCanGear00;
    case Slot::Gear03: return hero->_isCanGear03;
    }
    return false;
}

bool AIController::ruleReady(const SkillRule& rule)
{
    int key = (int)rule.slot;
    if (!slotReady(rule.slot))
    {
        _readySince.erase(key);
        return false;
    }
    if (rule.slot == Slot::NAttack || rule.slot == Slot::Gear00 || rule.slot == Slot::Gear03)
        return true;
    auto found = _readySince.find(key);
    if (found == _readySince.end())
    {
        // a human notices the cooldown a moment later
        _readySince[key] = _time + randFloat(_params->castDelayMin, _params->castDelayMax);
        return _params->castDelayMax <= 0;
    }
    if (_time < found->second)
        return false;
    auto skip = _skipUntil.find(key);
    return skip == _skipUntil.end() || _time >= skip->second;
}

void AIController::markUsed(Slot slot)
{
    _readySince.erase((int)slot);
}

bool AIController::condsHold(const SkillRule& rule, ActionManager* target)
{
    Hero* hero = _hero;
    for (const Cond& cond : rule.conds)
    {
        const std::string& name = cond.name;
        bool ok = true;
        if (name == "gp") ok = !target || target->getGP() < (_params->strictGp ? 5000 : 8000);
        else if (name == "tNotBati") ok = !target || !target->_isBati;
        else if (name == "tNotDown") ok = !target || target->getActionState() != ACTION_STATE_KOCKDOWN;
        else if (name == "tNotSticking") ok = !target || !target->_isSticking;
        else if (name == "tIdle") ok = target && inMoveState(target);
        else if (name == "tHpBelow") ok = target && target->getHpPercent() < cond.value;
        else if (name == "tHpAbsBelow") ok = target && target->getHP() < cond.value;
        else if (name == "tNoGear06") ok = target && !target->_isCanGear06;
        else if (name == "combo") ok = _params->combos && target && (isDown(target) || target->getActionState() == ACTION_STATE_HURT);
        else if (name == "selfBati") ok = hero->_isBati;
        else if (name == "selfNotBati") ok = !hero->_isBati;
        else if (name == "ougis2Ready") ok = slotReady(Slot::Ougis2);
        else if (name == "buff") ok = hero->_skillChangeBuffValue != 0;
        else if (name == "notBuff") ok = hero->_skillChangeBuffValue == 0;
        else if (name == "skillUP") ok = hero->_skillUPBuffValue != 0;
        else if (name == "notPowerUP") ok = hero->_powerUPBuffValue == 0;
        else if (name == "hpBelow") ok = hero->getHpPercent() < cond.value;
        else if (name == "hpAbove") ok = hero->getHpPercent() > cond.value;
        else if (name == "lv") ok = (int)hero->getLV() >= (int)cond.value;
        else if (name == "bamenBelow") ok = (int)hero->bamen < (int)cond.value;
        else if (name == "bamenAtLeast") ok = (int)hero->bamen >= (int)cond.value;
        else if (name == "noMonster") ok = !hasMonster(hero, characterIdFromString(cond.arg.c_str()));
        else if (name == "hasMonster") ok = hasMonster(hero, characterIdFromString(cond.arg.c_str()));
        else if (name == "notHealing") ok = !hero->_isHealling;
        else if (name == "noBuffTime") ok = !hero->_buffStartTime;
        else if (name == "invisible") ok = !hero->_isVisable;
        else if (name == "notTaunt") ok = !hero->_isTaunt;
        else if (name == "baseDanger") ok = hero->isBaseDanger;
        else if (name == "losing") ok = hero->battleCondiction < 0;
        else if (name == "nattackBelow") ok = hero->getnAttackValue() < cond.value;
        else if (name == "summonable")
            ok = hero->hearts > 0 && (!hero->getMonsterArray() || hero->getMonsterArray()->count() < 3);
        else if (name == "allyNear")
        {
            ok = false;
            CCObject* object;
            CCARRAY_FOREACH(_game->_CharacterArray, object)
            {
                ActionManager* unit = (ActionManager*)object;
                if (unit != hero && isLaneHero(unit) && !hero->isEnemy(unit) && unit->getActionState() != ACTION_STATE_DEAD &&
                    fabsf(unit->getPositionX() - hero->getPositionX()) <= kGameWidth / 2)
                    ok = true;
            }
        }
        else
            CCLOG("AI: unknown condition '%s' in %s", name.c_str(), _profile->name.c_str());
        if (!ok)
            return false;
    }
    return true;
}

bool AIController::retreatFromFight(const CCPoint& toTarget)
{
    Hero* hero = _hero;
    if (hero->_isHealling || hero->_isControled)
        return false;
    if (_time < _kiteUntil)
    {
        hero->stepBack2();
        return true;
    }
    if (_time < _braveUntil || fabsf(toTarget.x) >= 160)
        return false;
    int gap = hero->enemyCombatPoint - hero->friendCombatPoint;
    if (gap <= _profile->retreatGap)
        return false;
    // outmatched: back off for a moment, then fight again for a while,
    // instead of running back and forth at the edge of the fight
    _kiteUntil = _time + randFloat(0.8f, 1.5f);
    _braveUntil = _kiteUntil + randFloat(3.0f, 5.0f);
    hero->stepBack2();
    return true;
}

bool AIController::fightHero()
{
    Hero* hero = _hero;
    ActionManager* target = hero->_mainTarget;
    if (!target || target->role() == Role::Flog || target->role() == Role::Tower)
        return false;
    if (!canAct())
        return true;  // mid-animation: keep the target, act next tick

    CCPoint toTarget = deltaTo(target);
    bool retreatChecked = false;
    for (const SkillRule& rule : _profile->hero)
    {
        if (rule.gate && fabsf(toTarget.x) >= rule.gate)
            continue;
        if (rule.gate && !retreatChecked)
        {
            retreatChecked = true;
            if (retreatFromFight(toTarget))
                return true;
        }
        if (!ruleReady(rule) || !condsHold(rule, target))
            continue;
        bool inRange = fabsf(toTarget.x) <= rule.rangeX && fabsf(toTarget.y) <= rule.rangeY;
        if (!inRange)
        {
            if (!rule.approach)
                continue;
            return walkTo(toTarget, rule.rangeX, rule.rangeY, rule.speedGear);
        }
        if (rule.minRange && fabsf(toTarget.x) < rule.minRange && hero->stepBack())
            return true;
        if (rule.slot != Slot::NAttack && !randChance(_params->castChance))
        {
            _skipUntil[(int)rule.slot] = _time + randFloat(0.4f, 1.0f);
            continue;
        }
        if (rule.slot == Slot::Gear00 || rule.slot == Slot::Gear03)
        {
            hero->useGear(rule.slot == Slot::Gear00 ? gear00 : gear03);
            return true;
        }
        strike(rule.selfCast ? nullptr : target, toAbType(rule.slot));
        markUsed(rule.slot);
        return true;
    }

    if (!retreatChecked && retreatFromFight(toTarget))
        return true;
    if (fabsf(toTarget.x) >= 128)
        return false;  // nothing worth closing in for: go farm

    if (walkTo(toTarget, _profile->nattackX, _profile->nattackY))
        return true;
    // waiting on cooldowns: sometimes side-step instead of face-tanking
    if (_time < _strafeUntil)
        return true;
    if (_thinking && (role() == HeroRole::Ranged || role() == HeroRole::Support) && randChance(0.08f))
    {
        _strafeUntil = _time + 0.4f;
        hero->stepBack2();
        return true;
    }
    strike(target, NAttack);
    return true;
}

// ---------------------------------------------------------------------------
// lane

bool AIController::farm()
{
    Hero* hero = _hero;
    hero->_mainTarget = nullptr;
    const TeamBrain& team = TeamBrain::get(_game, hero->group());
    bool towerFirst = role() == HeroRole::Pusher || team.pushWindow ||
                      std::find(_profile->subRoles.begin(), _profile->subRoles.end(), "pusher") !=
                          _profile->subRoles.end();
    if (towerFirst)
    {
        if (!hero->findEnemy2("Tower"))
            hero->findEnemy2("Flog");
    }
    else if (!hero->findEnemy2("Flog"))
        hero->findEnemy2("Tower");

    ActionManager* target = hero->_mainTarget;
    if (!target)
        return false;
    CCPoint toTarget = deltaTo(target);
    if (walkTo(toTarget, 32, 32))
        return true;
    if (!canAct())
        return true;

    for (const SkillRule& rule : _profile->farm)
    {
        bool kindOk = rule.on == TargetKind::AnyFarm || (rule.on == TargetKind::Flog && target->role() == Role::Flog) ||
                      (rule.on == TargetKind::Tower && target->role() == Role::Tower);
        if (!kindOk || !ruleReady(rule) || !condsHold(rule, nullptr))
            continue;
        if (rule.slot == Slot::Gear03)
        {
            hero->useGear(gear03);
            return true;
        }
        strike(rule.selfCast ? nullptr : target, toAbType(rule.slot));
        markUsed(rule.slot);
        return true;
    }
    strike(target, NAttack);
    return true;
}

void AIController::advance()
{
    Hero* hero = _hero;
    if (hero->_isHealling && hero->getHpPercent() < 1)
    {
        if (canAct())
            hero->idle();
        return;
    }
    hero->stepOn();
}

// ---------------------------------------------------------------------------
// gear (HardCore only)

// HardCore: coin saved for the next gear of the build, so potions do not eat
// it (like the original AI, which only drank freely once it owned gear)
int AIController::gearReserve()
{
    if (!_game->_isHardCoreGame)
        return 0;
    if (!_buildChosen)
    {
        _build = chooseBuild(*this);
        _buildChosen = true;
    }
    int owned = _hero->getGearArray()->count();
    if (owned < 3 && owned < (int)_build.size())
        return ActionManager::gearCost((gearType)_build[owned]);
    gearType up = nextUpgrade();
    return up == None ? 0 : ActionManager::gearUpgradeCost(up, _hero->gearLevel(up));
}

// Once the build is bought, coin goes to upgrades: lowest level first, in build order
gearType AIController::nextUpgrade()
{
    gearType best = None;
    for (int id : _build)
    {
        gearType g = (gearType)id;
        int level = _hero->gearLevel(g);
        if (level >= 1 && level < 3 && (best == None || level < _hero->gearLevel(best)))
            best = g;
    }
    return best;
}

void AIController::buyGear()
{
    Hero* hero = _hero;
    if (!_game->_isHardCoreGame || hero->_isControled || hero->getCoin() < 500)
        return;
    int owned = hero->getGearArray()->count();
    if (!_buildChosen)
    {
        _build = chooseBuild(*this);
        _buildChosen = true;
    }
    if (owned < 3 && owned < (int)_build.size())
        hero->setGear((gearType)_build[owned]);
    else
    {
        gearType up = nextUpgrade();
        if (up != None)
            hero->upgradeGear(up);
    }
}

}  // namespace ai
