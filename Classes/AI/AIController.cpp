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

bool inMoveState(ActionManager* u)
{
    auto s = u->getActionState();
    return s == ACTION_STATE_IDLE || s == ACTION_STATE_WALK || s == ACTION_STATE_ATTACK;
}

bool isDown(ActionManager* u)
{
    auto s = u->getActionState();
    return s == ACTION_STATE_KOCKDOWN || s == ACTION_STATE_FLOAT || s == ACTION_STATE_AIRHURT;
}

bool hasMonster(ActionManager* u, const std::string& name)
{
    if (!u->getMonsterArray())
        return false;
    CCObject* obj;
    CCARRAY_FOREACH(u->getMonsterArray(), obj)
    {
        if (((ActionManager*)obj)->isCharacter(name.c_str()))
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
bool targetable(ActionManager* self, ActionManager* u, GameLayer* game)
{
    if (!isLaneHero(u) || !self->isEnemy(u) || u->getActionState() == ACTION_STATE_DEAD || !u->_isVisable ||
        u->_isWudi)
        return false;
    float tile = game->currentMap->getTileSize().width;
    float x = u->getPositionX();
    if (x < tile * 3 || x > (game->currentMap->getMapSize().width - 3) * tile)
        return false;
    return fabsf(x - self->getPositionX()) < winSize.width / 2;
}

}  // namespace

AIController* AIController::create(Hero* hero)
{
    if (hero->role() != Role::Com && hero->role() != Role::Player)
        return nullptr;  // clones and summons keep their small legacy routines
    const Profile* p = findProfile(hero->getCharacter()->getCString());
    return p ? new AIController(hero, p) : nullptr;
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
    for (int n : _activity)
        total += n;
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
        CCObject* obj;
        CCARRAY_FOREACH(_game->_CharacterArray, obj)
        {
            ActionManager* u = (ActionManager*)obj;
            if (targetable(_hero, u, _game) && u->getHpPercent() < best - 0.15f)
            {
                best = u->getHpPercent();
                choice = u;
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
    Hero* h = _hero;
    float hp = h->getHpPercent();

    // substitution jutsu out of a combo
    bool beingHit = isDown(h) || h->getActionState() == ACTION_STATE_HURT;
    if (_profile->panicGear06 && h->_isCanGear06 && beingHit && hp < 0.5f && !h->_isBati && !h->_isWudi)
        h->useGear(gear06);

    float potionAt = _params->potionHp + (role() == HeroRole::Support ? 0.05f : 0.0f);
    if (h->_isCanItem1 && h->getCoin() >= 50 && !h->_isHealling &&
        (hp < potionAt || h->getMaxHP() - h->getHP() >= 3000 + h->gearRecoverValue + 2000))
        h->setItem(Item1);

    if (h->_isControled)
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
    if (h->_isHealling)
    {
        if (canAct())
            h->idle();
        return true;
    }
    if (!canAct())
        return true;
    bool moved = h->_mainTarget ? h->stepBack2() : h->stepBack();
    if (!moved)
        h->idle();
    return true;
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
    CCObject* obj;
    CCARRAY_FOREACH(_game->_CharacterArray, obj)
    {
        ActionManager* u = (ActionManager*)obj;
        if (!isLaneHero(u) || !_hero->isEnemy(u))
            continue;
        auto s = u->getActionState();
        bool casting = s == ACTION_STATE_OATTACK || s == ACTION_STATE_O2ATTACK;
        if (!casting)
        {
            _dodgeRolled.erase(u);
            continue;
        }
        CCPoint d = ccpSub(u->getPosition(), _hero->getPosition());
        if (fabsf(d.x) > 160 || fabsf(d.y) > 64 || _dodgeRolled[u])
            continue;
        _dodgeRolled[u] = true;
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
    Hero* h = _hero;
    if (!_profile->baseCheck || h->_isControled)
        return false;

    const TeamBrain& team = TeamBrain::get(_game, h->group());
    if (team.towerThreat && targetable(h, team.towerThreat, _game) && role() != HeroRole::Assassin)
    {
        h->_mainTarget = team.towerThreat;
        return false;  // fightHero takes it from here
    }

    if (!h->isBaseDanger)
        return false;
    ActionManager* keep = h->_mainTarget;
    if (!h->checkBase())
    {
        h->_mainTarget = keep;
        return false;
    }
    // checkBase put the intruder in _mainTarget; fall back toward the base first
    float x = h->getPositionX();
    bool away = h->group() == Group::Akatsuki ? x < BASE_X_AKATSUKI : x > BASE_X_KONOHA;
    if (away && canAct())
        return h->stepBack2();
    return false;
}

// ---------------------------------------------------------------------------
// rules

bool AIController::slotReady(Slot slot) const
{
    Hero* h = _hero;
    switch (slot)
    {
    case Slot::NAttack: return true;
    case Slot::Skill1: return h->_isCanSkill1;
    case Slot::Skill2: return h->_isCanSkill2;
    case Slot::Skill3: return h->_isCanSkill3;
    case Slot::Ougis1: return h->_isCanOugis1 && !h->_isControled;
    case Slot::Ougis2: return h->_isCanOugis2 && !h->_isControled && _game->_isOugis2Game;
    case Slot::Gear00: return h->_isCanGear00;
    case Slot::Gear03: return h->_isCanGear03;
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
    auto it = _readySince.find(key);
    if (it == _readySince.end())
    {
        // a human notices the cooldown a moment later
        _readySince[key] = _time + randFloat(_params->castDelayMin, _params->castDelayMax);
        return _params->castDelayMax <= 0;
    }
    if (_time < it->second)
        return false;
    auto skip = _skipUntil.find(key);
    return skip == _skipUntil.end() || _time >= skip->second;
}

void AIController::markUsed(Slot slot)
{
    _readySince.erase((int)slot);
}

bool AIController::condsHold(const SkillRule& rule, ActionManager* t)
{
    Hero* h = _hero;
    for (const Cond& c : rule.conds)
    {
        const std::string& n = c.name;
        bool ok = true;
        if (n == "gp") ok = !t || t->getGP() < (_params->strictGp ? 5000 : 8000);
        else if (n == "tNotBati") ok = !t || !t->_isBati;
        else if (n == "tNotDown") ok = !t || t->getActionState() != ACTION_STATE_KOCKDOWN;
        else if (n == "tNotSticking") ok = !t || !t->_isSticking;
        else if (n == "tIdle") ok = t && inMoveState(t);
        else if (n == "tHpBelow") ok = t && t->getHpPercent() < c.value;
        else if (n == "tHpAbsBelow") ok = t && t->getHP() < c.value;
        else if (n == "tNoGear06") ok = t && !t->_isCanGear06;
        else if (n == "combo") ok = _params->combos && t && (isDown(t) || t->getActionState() == ACTION_STATE_HURT);
        else if (n == "selfBati") ok = h->_isBati;
        else if (n == "selfNotBati") ok = !h->_isBati;
        else if (n == "buff") ok = h->_skillChangeBuffValue != 0;
        else if (n == "notBuff") ok = h->_skillChangeBuffValue == 0;
        else if (n == "skillUP") ok = h->_skillUPBuffValue != 0;
        else if (n == "notPowerUP") ok = h->_powerUPBuffValue == 0;
        else if (n == "hpBelow") ok = h->getHpPercent() < c.value;
        else if (n == "hpAbove") ok = h->getHpPercent() > c.value;
        else if (n == "lv") ok = (int)h->getLV() >= (int)c.value;
        else if (n == "bamenBelow") ok = (int)h->bamen < (int)c.value;
        else if (n == "bamenAtLeast") ok = (int)h->bamen >= (int)c.value;
        else if (n == "noMonster") ok = !hasMonster(h, c.arg);
        else if (n == "hasMonster") ok = hasMonster(h, c.arg);
        else if (n == "notHealing") ok = !h->_isHealling;
        else if (n == "noBuffTime") ok = !h->_buffStartTime;
        else if (n == "invisible") ok = !h->_isVisable;
        else if (n == "notTaunt") ok = !h->_isTaunt;
        else if (n == "baseDanger") ok = h->isBaseDanger;
        else if (n == "losing") ok = h->battleCondiction < 0;
        else if (n == "nattackBelow") ok = h->getnAttackValue() < c.value;
        else if (n == "summonable")
            ok = h->hearts > 0 && (!h->getMonsterArray() || h->getMonsterArray()->count() < 3);
        else if (n == "allyNear")
        {
            ok = false;
            CCObject* obj;
            CCARRAY_FOREACH(_game->_CharacterArray, obj)
            {
                ActionManager* u = (ActionManager*)obj;
                if (u != h && isLaneHero(u) && !h->isEnemy(u) && u->getActionState() != ACTION_STATE_DEAD &&
                    fabsf(u->getPositionX() - h->getPositionX()) <= winSize.width / 2)
                    ok = true;
            }
        }
        else
            CCLOG("AI: unknown condition '%s' in %s", n.c_str(), _profile->name.c_str());
        if (!ok)
            return false;
    }
    return true;
}

bool AIController::retreatFromFight(const CCPoint& sp)
{
    Hero* h = _hero;
    if (h->_isHealling || h->_isControled)
        return false;
    if (_time < _kiteUntil)
    {
        h->stepBack2();
        return true;
    }
    if (_time < _braveUntil || fabsf(sp.x) >= 160)
        return false;
    int gap = h->enemyCombatPoint - h->friendCombatPoint;
    if (gap <= _profile->retreatGap)
        return false;
    // outmatched: back off for a moment, then fight again for a while,
    // instead of running back and forth at the edge of the fight
    _kiteUntil = _time + randFloat(0.8f, 1.5f);
    _braveUntil = _kiteUntil + randFloat(3.0f, 5.0f);
    h->stepBack2();
    return true;
}

bool AIController::fightHero()
{
    Hero* h = _hero;
    ActionManager* t = h->_mainTarget;
    if (!t || t->role() == Role::Flog || t->role() == Role::Tower)
        return false;
    if (!canAct())
        return true;  // mid-animation: keep the target, act next tick

    CCPoint sp = deltaTo(t);
    bool retreatChecked = false;
    for (const SkillRule& rule : _profile->hero)
    {
        if (rule.gate && fabsf(sp.x) >= rule.gate)
            continue;
        if (rule.gate && !retreatChecked)
        {
            retreatChecked = true;
            if (retreatFromFight(sp))
                return true;
        }
        if (!ruleReady(rule) || !condsHold(rule, t))
            continue;
        bool inRange = fabsf(sp.x) <= rule.rangeX && fabsf(sp.y) <= rule.rangeY;
        if (!inRange)
        {
            if (!rule.approach)
                continue;
            return walkTo(sp, rule.rangeX, rule.rangeY, rule.speedGear);
        }
        if (rule.minRange && fabsf(sp.x) < rule.minRange && h->stepBack())
            return true;
        if (rule.slot != Slot::NAttack && !randChance(_params->castChance))
        {
            _skipUntil[(int)rule.slot] = _time + randFloat(0.4f, 1.0f);
            continue;
        }
        if (rule.slot == Slot::Gear00 || rule.slot == Slot::Gear03)
        {
            h->useGear(rule.slot == Slot::Gear00 ? gear00 : gear03);
            return true;
        }
        strike(rule.selfCast ? nullptr : t, toAbType(rule.slot));
        markUsed(rule.slot);
        return true;
    }

    if (!retreatChecked && retreatFromFight(sp))
        return true;
    if (fabsf(sp.x) >= 128)
        return false;  // nothing worth closing in for: go farm

    if (walkTo(sp, _profile->nattackX, _profile->nattackY))
        return true;
    // waiting on cooldowns: sometimes side-step instead of face-tanking
    if (_time < _strafeUntil)
        return true;
    if (_thinking && (role() == HeroRole::Ranged || role() == HeroRole::Support) && randChance(0.08f))
    {
        _strafeUntil = _time + 0.4f;
        h->stepBack2();
        return true;
    }
    strike(t, NAttack);
    return true;
}

// ---------------------------------------------------------------------------
// lane

bool AIController::farm()
{
    Hero* h = _hero;
    h->_mainTarget = nullptr;
    const TeamBrain& team = TeamBrain::get(_game, h->group());
    bool towerFirst = role() == HeroRole::Pusher || team.pushWindow ||
                      std::find(_profile->subRoles.begin(), _profile->subRoles.end(), "pusher") !=
                          _profile->subRoles.end();
    if (towerFirst)
    {
        if (!h->findEnemy2("Tower"))
            h->findEnemy2("Flog");
    }
    else if (!h->findEnemy2("Flog"))
        h->findEnemy2("Tower");

    ActionManager* t = h->_mainTarget;
    if (!t)
        return false;
    CCPoint sp = deltaTo(t);
    if (walkTo(sp, 32, 32))
        return true;
    if (!canAct())
        return true;

    for (const SkillRule& rule : _profile->farm)
    {
        bool kindOk = rule.on == TargetKind::AnyFarm || (rule.on == TargetKind::Flog && t->role() == Role::Flog) ||
                      (rule.on == TargetKind::Tower && t->role() == Role::Tower);
        if (!kindOk || !ruleReady(rule) || !condsHold(rule, nullptr))
            continue;
        if (rule.slot == Slot::Gear03)
        {
            h->useGear(gear03);
            return true;
        }
        strike(rule.selfCast ? nullptr : t, toAbType(rule.slot));
        markUsed(rule.slot);
        return true;
    }
    strike(t, NAttack);
    return true;
}

void AIController::advance()
{
    Hero* h = _hero;
    if (h->_isHealling && h->getHpPercent() < 1)
    {
        if (canAct())
            h->idle();
        return;
    }
    h->stepOn();
}

// ---------------------------------------------------------------------------
// gear (HardCore only)

void AIController::buyGear()
{
    Hero* h = _hero;
    if (!_game->_isHardCoreGame || h->_isControled || h->getCoin() < 500)
        return;
    int owned = h->getGearArray()->count();
    if (owned >= 3)
        return;
    if (!_buildChosen)
    {
        _build = chooseBuild(*this);
        _buildChosen = true;
    }
    if (owned < (int)_build.size())
        h->setGear((gearType)_build[owned]);
}

}  // namespace ai
