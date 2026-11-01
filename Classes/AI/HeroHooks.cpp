#include "AI/HeroHooks.h"
#include "AI/AIController.h"
#include "AI/TeamBrain.h"
#include "Characters.h"
#include "GameLayer.h"

namespace ai
{

namespace
{

ActionManager* findMonster(ActionManager* owner, const char* name)
{
    if (!owner->getMonsterArray())
        return nullptr;
    CCObject* obj;
    CCARRAY_FOREACH(owner->getMonsterArray(), obj)
    {
        ActionManager* m = (ActionManager*)obj;
        if (m->isCharacter(name))
            return m;
    }
    return nullptr;
}

// Hidan: after Skill1 drops his ritual circle he walks onto it and, once in
// the ritual (bati), stands there; he only swings when an enemy hero is low.
bool hidan(AIController& ai)
{
    Hero* h = ai.hero();
    ActionManager* mark = findMonster(h, "CircleMark");
    if (!mark)
        return false;
    h->_mainTarget = mark;
    if (ai.walkTo(ai.deltaTo(mark), 8, 8))
        return true;
    if (!h->_isBati)
        return false;
    CCObject* obj;
    CCARRAY_FOREACH(h->getDelegate()->_CharacterArray, obj)
    {
        ActionManager* u = (ActionManager*)obj;
        if (isLaneHero(u) && h->isEnemy(u) && u->getHP() < 2000 && u->getActionState() != ACTION_STATE_DEAD)
        {
            h->attack(NAttack);
            return true;
        }
    }
    if (ai.canAct())
        h->idle();
    return true;
}

// Minato: Skill1 teleports to a Hiraishin mark. Low on HP he jumps to a mark
// behind him; otherwise, with nothing to fight, to a mark further up the lane.
bool minato(AIController& ai)
{
    Hero* h = ai.hero();
    if (!h->_isCanSkill1 || !ai.canAct() || !h->getMonsterArray())
        return false;
    bool lowHp = h->getHpPercent() < 0.3f;
    float dir = h->group() == Group::Konoha ? 1.0f : -1.0f;
    CCObject* obj;
    CCARRAY_FOREACH(h->getMonsterArray(), obj)
    {
        ActionManager* m = (ActionManager*)obj;
        if (!m->isCharacter("HiraishinMark"))
            continue;
        float ahead = (m->getPositionX() - h->getPositionX()) * dir;
        if (lowHp && ahead < 0)
        {
            h->attack(SKILL1);
            return true;
        }
        if (!lowHp && ahead > winSize.width / 2 && !h->_isHealling)
        {
            h->attack(SKILL1);
            return true;
        }
    }
    return false;
}

// Kakuzu: Skill1 on a fallen hero takes a heart (extra life for his masks).
bool kakuzu(AIController& ai)
{
    Hero* h = ai.hero();
    if (!ai.canAct() || h->getHpPercent() <= 0.3f || h->_isControled || !h->_isCanSkill1)
        return false;
    ActionManager* best = nullptr;
    float bestDist = 0;
    CCObject* obj;
    CCARRAY_FOREACH(h->getDelegate()->_CharacterArray, obj)
    {
        ActionManager* u = (ActionManager*)obj;
        if (!isLaneHero(u) || u->getActionState() != ACTION_STATE_DEAD)
            continue;
        CCPoint d = ccpSub(u->getPosition(), h->getPosition());
        if (fabsf(d.x) >= winSize.width / 2)
            continue;
        float dist = ccpLength(d);
        if (!best || dist < bestDist)
        {
            best = u;
            bestDist = dist;
        }
    }
    if (!best)
        return false;
    h->_mainTarget = best;
    CCPoint d = ai.deltaTo(best);
    if (ai.walkTo(d, 32, 32))
        return true;
    ai.strike(best, SKILL1);
    return true;
}

}  // namespace

PreTick findHook(const std::string& name)
{
    if (name == "hidan") return hidan;
    if (name == "minato") return minato;
    if (name == "kakuzu") return kakuzu;
    CCLOG("AI: unknown hook '%s'", name.c_str());
    return nullptr;
}

}  // namespace ai
