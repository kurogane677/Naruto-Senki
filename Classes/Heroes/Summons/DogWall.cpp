#include "Heroes/Summons/DogWall.h"
#include "GameLayer.h"

namespace
{
// Creeps drop any target that is not a creep or a tower once it is more than 64 px
// away (Flog::setAI), so the pull reaches no further than that.
constexpr float kAggroRange = 64;
}  // namespace

void DogWall::ownAI()
{
    pullCreeps();
    attack(NAttack);
}

void DogWall::dealloc()
{
    releaseCreeps();
    Hero::dealloc();
}

CCArray* DogWall::enemyCreeps() const
{
    return group() == Group::Konoha ? _delegate->_AkatsukiFlogArray : _delegate->_KonohaFlogArray;
}

void DogWall::pullCreeps()
{
    CCObject* object;
    CCARRAY_FOREACH(enemyCreeps(), object)
    {
        ActionManager* creep = (ActionManager*)object;
        if (creep->getActionState() != ACTION_STATE_DEAD &&
            abs(creep->getPositionX() - getPositionX()) <= kAggroRange)
            creep->_mainTarget = this;
    }
}

// creeps still on the wall look for their own targets again (and keep no pointer to it)
void DogWall::releaseCreeps()
{
    CCObject* object;
    CCARRAY_FOREACH(enemyCreeps(), object)
    {
        ActionManager* creep = (ActionManager*)object;
        if (creep->_mainTarget == this)
            creep->_mainTarget = NULL;
    }
}
