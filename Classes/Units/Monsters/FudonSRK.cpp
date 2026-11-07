#include "Units/Monsters/FudonSRK.h"

namespace
{
constexpr float kSpawnDistance = 48;
// close enough to burst
constexpr float kReachX = 48;
constexpr float kReachY = 32;
}  // namespace

bool FudonSRK::onSpawn(ActionManager* owner)
{
    setPosition(ccp(owner->getPositionX() + (owner->_isFlipped ? -kSpawnDistance : kSpawnDistance), owner->getPositionY()));
    attack(NAttack);

    // chase the owner's target when the owner faces it, otherwise fly straight ahead
    ActionManager* target = owner->_mainTarget;
    bool facesTarget = target && (owner->_isFlipped ? target->getPositionX() <= owner->getPositionX()
                                                    : target->getPositionX() > owner->getPositionX());
    if (facesTarget)
    {
        _mainTarget = target;
        doAI();
    }
    else
    {
        setActionState(ACTION_STATE_WALK);
        walk(ccpNormalize(ccp(owner->_isFlipped ? -1 : 1, 0)));
    }
    return true;
}

// it keeps the target it was given and does not look for another
void FudonSRK::setAI(float dt)
{
    if (!_mainTarget)
    {
        idle();
        return;
    }

    CCPoint toTarget = ccpSub(
        ccp(_mainTarget->getPositionX(), _mainTarget->_originY ? _mainTarget->_originY : _mainTarget->getPositionY()),
        ccp(getPositionX(), _originY ? _originY : getPositionY()));
    if (abs(toTarget.x) > kReachX || abs(toTarget.y) > kReachY)
    {
        setActionState(ACTION_STATE_WALK);
        walk(ccpNormalize(toTarget));
        return;
    }

    if (_mainTarget->role() == Role::Player || _mainTarget->role() == Role::Com)
    {
        stopAllActions();
        dealloc();
    }
}

// plays its burst before it is removed
void FudonSRK::dealloc()
{
    unschedule(schedule_selector(ActionManager::setAI));
    setActionState(ACTION_STATE_DEAD);
    stopAllActions();

    CCArray* burstThenRemove = CCArray::create();
    burstThenRemove->addObject(getDeadAction());
    burstThenRemove->addObject(CCCallFunc::create(this, callfunc_selector(FudonSRK::dealloc2)));
    runAction(CCSequence::create(burstThenRemove));
}
