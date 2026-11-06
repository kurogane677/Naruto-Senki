#include "AI/SummonController.h"
#include "AI/TeamBrain.h"
#include "Characters.h"
#include "GameLayer.h"
#include <algorithm>
#include <cmath>

namespace ai
{

namespace
{

constexpr int kScreen = -1;  // leash of half a screen, minus a margin

// one engagement, handed to a summon's own technique
struct Contact
{
    Hero* master;  // nullptr once the summoner is dead or gone
    ActionManager* target;
    CCPoint sp;    // summon -> target
    bool inReach;  // basic attack lands
    bool inSkill;  // the summon's own skill lands
};

// the summon's own technique; true when it issued a command this tick
using Act = bool (*)(Hero* self, const Contact& c);

}  // namespace

struct SummonSpec
{
    const char* name;
    int reachX, reachY;   // basic attack reach
    int skillX;           // reach of the summon's own skill, 0: none
    int leash;            // max |dx| from the summoner, 0: free, kScreen: half a screen
    bool heroes;          // goes after heroes at all (Slug and Centipede only push)
    bool puppet;          // only moves while its puppeteer is not hurt or down
    bool freeWhenBuffed;  // Parents: the leash drops while the skill buff is on
    Act act;
};

namespace
{

bool inMoveState(ActionManager* u)
{
    auto s = u->getActionState();
    return s == ACTION_STATE_IDLE || s == ACTION_STATE_WALK || s == ACTION_STATE_ATTACK;
}

// the puppeteer is acting, not hurt, down or dead
bool pulling(ActionManager* m)
{
    auto s = m->getActionState();
    return inMoveState(m) || s == ACTION_STATE_SATTACK || s == ACTION_STATE_OATTACK || s == ACTION_STATE_O2ATTACK;
}

bool isFarm(ActionManager* u)
{
    return u->role() == Role::Flog || u->role() == Role::Tower;
}

bool hasMonster(ActionManager* u, const char* name)
{
    if (!u->getMonsterArray())
        return false;
    CCObject* obj;
    CCARRAY_FOREACH(u->getMonsterArray(), obj)
    {
        if (((ActionManager*)obj)->isCharacter(name))
            return true;
    }
    return false;
}

void cast(Hero* self, const Contact& c, abType slot, float rearm)
{
    self->changeSide(c.sp);
    self->attack(slot);
    if (rearm <= 0)
        return;
    if (slot == SKILL1)
        self->scheduleOnce(schedule_selector(ActionManager::enableSkill1), rearm);
    else
        self->scheduleOnce(schedule_selector(ActionManager::enableSkill2), rearm);
}

// the summoner casts its own skill through the summon (Kiba's Gatsuga with
// Akamaru, Kankuro's and Chiyo's puppet techniques); bot summoners only
bool masterCast(Hero* self, const Contact& c, bool ready, abType slot)
{
    Hero* m = c.master;
    if (!m || !ready || isFarm(c.target) || c.target->getGP() >= 5000 || !(m->_isControled || m->_isAI) ||
        !inMoveState(m))
        return false;
    self->changeSide(c.sp);
    m->attack(slot);
    return true;
}

bool masterSkill2(Hero* self, const Contact& c)
{
    return c.inReach && c.master && masterCast(self, c, c.master->_isCanSkill2, SKILL2);
}

bool parents(Hero* self, const Contact& c)
{
    if (!c.master || self->_skillChangeBuffValue)
        return false;
    if (masterCast(self, c, c.master->_isCanSkill3, SKILL3))
        return true;
    return c.inReach && masterCast(self, c, c.master->_isCanSkill2, SKILL2);
}

bool ownSkill1(Hero* self, const Contact& c, float rearm)
{
    if (!c.inSkill || isFarm(c.target) || !self->_isCanSkill1 || c.target->getGP() >= 5000)
        return false;
    cast(self, c, SKILL1, rearm);
    return true;
}

bool sasoSkill(Hero* self, const Contact& c)
{
    return ownSkill1(self, c, self->_sattackcoldDown1);
}

bool maskSkill(Hero* self, const Contact& c)
{
    return ownSkill1(self, c, 0);
}

// DevaPath brings out the other Paths while they are missing
bool devaPath(Hero* self, const Contact& c)
{
    if (!c.inReach || isFarm(c.target))
        return false;
    if (self->_isCanSkill1 && !hasMonster(self, "AnimalPath"))
    {
        cast(self, c, SKILL1, self->_sattackcoldDown1);
        return true;
    }
    if (self->_isCanSkill2 && !hasMonster(self, "AsuraPath"))
    {
        cast(self, c, SKILL2, self->_sattackcoldDown2);
        return true;
    }
    return false;
}

// AsuraPath fires its missiles from a distance: back off first when close
bool asuraPath(Hero* self, const Contact& c)
{
    if (!c.inSkill || isFarm(c.target) || !self->_isCanSkill1)
        return false;
    float x = self->getPositionX();
    if (fabsf(c.sp.x) < 64 && x > 64 && x < 95 * 32 - 64 && self->stepBack())
        return true;
    cast(self, c, SKILL1, self->_sattackcoldDown1);
    return true;
}

bool animalPath(Hero* self, const Contact& c)
{
    if (!c.inReach || c.target->role() == Role::Tower || !self->_isCanSkill1)
        return false;
    cast(self, c, SKILL1, 10.0f);
    return true;
}

bool slug(Hero* self, const Contact& c)
{
    if (!c.inReach || !isFarm(c.target))
        return false;
    if (self->_isCanSkill1)
        cast(self, c, SKILL1, self->_sattackcoldDown1);
    else if (self->_isCanSkill2)
        cast(self, c, SKILL2, self->_sattackcoldDown2);
    else
        return false;
    return true;
}

const SummonSpec kSpecs[] = {
    // name        reach    skill leash    heroes puppet buffed act
    {"Akamaru",    48, 32,  0,    kScreen, true,  false, false, masterSkill2},
    {"Kurama",     156, 48, 0,    0,       true,  false, false, nullptr},
    {"Karasu",     32, 16,  0,    kScreen, true,  true,  false, masterSkill2},
    {"Saso",       48, 16,  48,   64,      true,  true,  false, sasoSkill},
    {"Sanshouuo",  32, 32,  96,   kScreen, true,  true,  false, sasoSkill},
    {"Parents",    48, 32,  0,    9,       true,  false, true,  parents},
    {"MaskRaidon", 48, 16,  96,   0,       true,  false, false, maskSkill},
    {"MaskFudon",  48, 16,  96,   0,       true,  false, false, maskSkill},
    {"MaskKadon",  48, 16,  96,   0,       true,  false, false, maskSkill},
    {"DevaPath",   32, 32,  0,    0,       true,  false, false, devaPath},
    {"AsuraPath",  32, 32,  256,  0,       true,  false, false, asuraPath},
    {"AnimalPath", 32, 32,  0,    0,       true,  false, false, animalPath},
    {"Slug",       32, 32,  0,    0,       false, false, false, slug},
    {"Centipede",  32, 32,  0,    0,       false, false, false, nullptr},
};

}  // namespace

SummonController* SummonController::create(Hero* summon)
{
    Role r = summon->role();
    if (r != Role::Clone && r != Role::Summon && r != Role::Kuilei)
        return nullptr;
    for (const SummonSpec& spec : kSpecs)
    {
        if (summon->isCharacter(spec.name))
            return new SummonController(summon, &spec);
    }
    return nullptr;
}

SummonController::SummonController(Hero* summon, const SummonSpec* spec)
    : _summon(summon), _spec(spec), _game(summon->getDelegate())
{
}

Hero* SummonController::liveMaster() const
{
    Hero* m = _summon->getMaster();
    if (!m || !_game->_CharacterArray->containsObject(m) || m->getActionState() == ACTION_STATE_DEAD)
        return nullptr;
    return m;
}

// the summoner's role; a summon of a summon (AnimalPath) asks the hero behind it
HeroRole SummonController::summonerRole(Hero* master) const
{
    if (!master)
        return HeroRole::Pusher;
    for (ActionManager* m : {(ActionManager*)master, master->getSecMaster()})
    {
        if (!m)
            continue;
        if (const Profile* p = findProfile(m->getCharacter()->getCString()))
            return p->role;
    }
    return HeroRole::Pusher;
}

int SummonController::leash(HeroRole role, Hero* master) const
{
    if (!master)
        return 0;
    int leashRange = (_spec->freeWhenBuffed && _summon->_skillChangeBuffValue) ? 0 : _spec->leash;
    if (!leashRange && (role == HeroRole::Support || role == HeroRole::Ranged))
        leashRange = kScreen;  // guards and backline summons stay by their summoner
    return leashRange == kScreen ? int(kGameWidth / 2 - 48) : leashRange;
}

bool SummonController::canWalk(Hero* master) const
{
    return !_spec->puppet || !master || pulling(master);
}

bool SummonController::validTarget(ActionManager* u) const
{
    if (!u || u->group() == _summon->group() || u->role() == Role::Kuilei)
        return false;
    CCArray* list = _game->_CharacterArray;
    if (u->role() == Role::Tower)
        list = _game->_TowerArray;
    else if (u->role() == Role::Flog)
        list = u->group() == Group::Konoha ? _game->_KonohaFlogArray : _game->_AkatsukiFlogArray;
    return list->containsObject(u) && u->getActionState() != ACTION_STATE_DEAD && u->_isVisable && !u->_isWudi;
}

ActionManager* SummonController::weakestHero(ActionManager* center, float range) const
{
    ActionManager* best = nullptr;
    CCObject* obj;
    CCARRAY_FOREACH(_game->_CharacterArray, obj)
    {
        ActionManager* u = (ActionManager*)obj;
        if (!isLaneHero(u) || !validTarget(u) || fabsf(u->getPositionX() - center->getPositionX()) >= range)
            continue;
        if (u->_isTaunt)
            return u;
        if (!best || u->getHpPercent() < best->getHpPercent())
            best = u;
    }
    return best;
}

ActionManager* SummonController::pickTarget(HeroRole role, Hero* master, int leash)
{
    Hero* summon = _summon;
    bool fromMaster = master && leash;
    int range = int(kGameWidth / 2 - 32);
    auto find = [&](const char* type, int searchRange) -> ActionManager* {
        return summon->findEnemy(type, searchRange, fromMaster) ? summon->_mainTarget : nullptr;
    };

    ActionManager* target = nullptr;
    if (!_spec->heroes || role == HeroRole::Pusher)
    {
        // pushers clear the lane first
        if (!(target = find("Flog", range)))
            target = find("Tower", range);
        if (!target && _spec->heroes)
            target = find("Hero", range);
        return target;
    }

    switch (role)
    {
    case HeroRole::Assassin:
        target = weakestHero(fromMaster ? (ActionManager*)master : summon, range);
        break;
    case HeroRole::Ranged:
        if (master && validTarget(master->_mainTarget))
            target = master->_mainTarget;  // focus what the summoner shoots at
        break;
    case HeroRole::Support:
        if (master)
            target = summon->findEnemy("Hero", 192, true) ? summon->_mainTarget : nullptr;  // whoever reaches the summoner
        break;
    default:
        break;
    }
    if (!target)
        target = find("Hero", range);
    if (!target)
        target = find("Flog", range);
    if (!target)
        target = find("Tower", range);
    return target;
}

void SummonController::tick(float)
{
    Hero* s = _summon;
    Hero* master = liveMaster();
    HeroRole role = summonerRole(master);
    int reach = leash(role, master);
    bool strings = canWalk(master);
    ActionManager* t = s->_mainTarget = pickTarget(role, master, reach);

    auto toward = [s](const CCPoint& p) { s->walk(ccpNormalize(ccpSub(p, s->getPosition()))); };
    auto hold = [&]() {
        if (master && reach)
        {
            // stay at the summoner's side
            if (fabsf(master->getPositionX() - s->getPositionX()) > std::min(reach, 64))
            {
                if (strings)
                    toward(master->getPosition());
            }
            else if (s->getActionState() == ACTION_STATE_WALK)
            {
                s->idle();
            }
        }
        else if (strings)
        {
            s->stepOn();
        }
    };

    if (master && reach && fabsf(master->getPositionX() - s->getPositionX()) > reach)
    {
        if (inMoveState(s))
            toward(master->getPosition());
        return;
    }
    if (!t)
    {
        hold();
        return;
    }

    Contact c;
    c.master = master;
    c.target = t;
    c.sp = ccpSub(ccp(t->getPositionX(), t->_originY ? t->_originY : t->getPositionY()),
                  ccp(s->getPositionX(), s->_originY ? s->_originY : s->getPositionY()));
    c.inReach = fabsf(c.sp.x) <= _spec->reachX && fabsf(c.sp.y) <= _spec->reachY;
    c.inSkill = _spec->skillX && fabsf(c.sp.x) <= _spec->skillX && fabsf(c.sp.y) <= _spec->reachY;

    if (!inMoveState(s))
        return;
    if (_spec->act && _spec->act(s, c))
        return;
    if (!c.inReach)
    {
        // a leashed summon does not chase past its leash
        bool chase = !master || !reach || fabsf(t->getPositionX() - master->getPositionX()) <= reach + _spec->reachX;
        if (chase && strings)
            s->walk(ccpNormalize(c.sp));
        else
            hold();
        return;
    }
    s->changeSide(c.sp);
    s->attack(NAttack);
}

}  // namespace ai
