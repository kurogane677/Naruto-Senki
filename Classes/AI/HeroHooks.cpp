#include "AI/HeroHooks.h"
#include "AI/AIController.h"
#include "AI/TeamBrain.h"
#include "Characters.h"
#include "GameLayer.h"

namespace ai
{

namespace
{

ActionManager* findMonster(ActionManager* owner, CharacterId id)
{
    if (!owner->getMonsterArray())
        return nullptr;
    CCObject* obj;
    CCARRAY_FOREACH(owner->getMonsterArray(), obj)
    {
        ActionManager* monster = (ActionManager*)obj;
        if (monster->isCharacter(id))
            return monster;
    }
    return nullptr;
}

// Hidan: Skill1 drops his ritual circle only to set up Ougis2 (the profile
// casts it while Ougis2 is ready). He walks onto it and, once in the ritual
// (bati), stands there; he only swings when an enemy hero is low.
bool hidan(AIController& ai)
{
    Hero* h = ai.hero();
    ActionManager* mark = findMonster(h, CharacterId::CircleMark);
    if (!mark || (!h->_isBati && !h->_isCanOugis2))
        return false;  // a circle left without Ougis2 does not tie him down
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
    Hero* hero = ai.hero();
    if (!hero->_isCanSkill1 || !ai.canAct() || !hero->getMonsterArray())
        return false;
    bool lowHp = hero->getHpPercent() < 0.3f;
    float laneDirection = hero->group() == Group::Konoha ? 1.0f : -1.0f;
    CCObject* object;
    CCARRAY_FOREACH(hero->getMonsterArray(), object)
    {
        ActionManager* mark = (ActionManager*)object;
        if (!mark->isCharacter(CharacterId::HiraishinMark))
            continue;
        float ahead = (mark->getPositionX() - hero->getPositionX()) * laneDirection;
        if (lowHp && ahead < 0)
        {
            hero->attack(SKILL1);
            return true;
        }
        if (!lowHp && ahead > kGameWidth / 2 && !hero->_isHealling)
        {
            hero->attack(SKILL1);
            return true;
        }
    }
    return false;
}

// Kakuzu: Skill1 on a fallen hero takes a heart (extra life for his masks).
bool kakuzu(AIController& ai)
{
    Hero* hero = ai.hero();
    if (!ai.canAct() || hero->getHpPercent() <= 0.3f || hero->_isControled || !hero->_isCanSkill1)
        return false;
    ActionManager* best = nullptr;
    float bestDistance = 0;
    CCObject* object;
    CCARRAY_FOREACH(hero->getDelegate()->_CharacterArray, object)
    {
        ActionManager* unit = (ActionManager*)object;
        if (!isLaneHero(unit) || unit->getActionState() != ACTION_STATE_DEAD)
            continue;
        CCPoint toUnit = ccpSub(unit->getPosition(), hero->getPosition());
        if (fabsf(toUnit.x) >= kGameWidth / 2)
            continue;
        float distance = ccpLength(toUnit);
        if (!best || distance < bestDistance)
        {
            best = unit;
            bestDistance = distance;
        }
    }
    if (!best)
        return false;
    hero->_mainTarget = best;
    CCPoint toUnit = ai.deltaTo(best);
    if (ai.walkTo(toUnit, 32, 32))
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
