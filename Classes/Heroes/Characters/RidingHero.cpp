#include "Heroes/Characters/RidingHero.h"
#include "Units/MonsterFactory.h"

namespace
{
constexpr int kAttackBuff = 17;
constexpr int kRideBuff = 18;
constexpr float kRideSpeed = 320;
constexpr float kWalkSpeed = 224;
constexpr float kRideHPBarY = 120;
constexpr float kSpawnInterval = 0.5f;
// ride drops are capped and short-lived (#69)
constexpr int kMaxRideMonsters = 5;
constexpr float kRideMonsterLifetime = 4.0f;
}  // namespace

bool RidingHero::onTransformSkillStart()
{
    if (_skillChangeBuffValue == kAttackBuff)
        setNAttackAction(createAnimation(skillSPC1Array, 10.0f, false, true));
    else if (_skillChangeBuffValue == kRideBuff)
        startRide();
    return true;
}

bool RidingHero::onTransformSkillEnd()
{
    if (_skillChangeBuffValue == kAttackBuff)
        setNAttackAction(createAnimation(nattackArray, 10.0f, false, true));
    else if (_skillChangeBuffValue == kRideBuff)
        endRide(true);
    return true;
}

// the buff ends early (setActionResume)
bool RidingHero::onTransformSkillCancel()
{
    if (!_skillChangeBuffValue)
        return true;
    unschedule(schedule_selector(ActionManager::resumeAction));
    if (_skillChangeBuffValue == kAttackBuff)
        setNAttackAction(createAnimation(nattackArray, 10.0f, false, true));
    else if (_skillChangeBuffValue == kRideBuff)
        endRide(false);
    _skillChangeBuffValue = 0;
    return true;
}

void RidingHero::startRide()
{
    _isAllAttackLocked = true;
    _isBati = true;
    setWalkSpeed(kRideSpeed);
    _originSpeed = kRideSpeed;
    lockOugis();
    setIdleAction(createAnimation(skillSPC2Array, 5.0f, true, false));
    setWalkAction(createAnimation(skillSPC3Array, 5.0f, true, false));
    if (_hpBar)
        _hpBar->setPositionY(kRideHPBarY);
    schedule(schedule_selector(RidingHero::spawnRideMonster), kSpawnInterval);
}

void RidingHero::endRide(bool returnToIdle)
{
    _isBati = false;
    _isAllAttackLocked = false;
    _originSpeed = kWalkSpeed;
    setWalkSpeed(kWalkSpeed);
    unlockOugis();
    setIdleAction(createAnimation(idleArray, 5.0f, true, false));
    setWalkAction(createAnimation(walkArray, 10.0f, true, false));
    if (returnToIdle && _actionState != ACTION_STATE_DEAD)
    {
        _actionState = ACTION_STATE_WALK;
        idle();
    }
    if (_hpBar)
        _hpBar->setPositionY(getHeight());
    unschedule(schedule_selector(RidingHero::spawnRideMonster));
}

void RidingHero::spawnRideMonster(float dt)
{
    if (!_monsterArray)
    {
        _monsterArray = CCArray::create();
        _monsterArray->retain();
    }

    const CharacterId rideId = characterIdFromString(rideMonster());
    int alive = 0;
    CCObject* object;
    CCARRAY_FOREACH(_monsterArray, object)
    {
        ActionManager* monster = (ActionManager*)object;
        if (monster->isCharacter(rideId) && monster->getActionState() != ACTION_STATE_DEAD)
            alive++;
    }
    if (alive >= kMaxRideMonsters)
        return;

    Monster* monster = MonsterFactory::create(rideId);
    monster->setDelegate(_delegate);
    monster->setID(CCString::create(rideMonster()), CCString::create("Mon"), _group);
    onRideMonsterSpawned();

    monster->setMaster(this);
    monster->idle();
    monster->setPosition(ccp(getPositionX() + (_isFlipped ? -32 : 32), _originY ? _originY : getPositionY()));
    monster->setFlippedX(_isFlipped);
    monster->_isFlipped = _isFlipped;

    _monsterArray->addObject(monster);
    monster->doAI();
    monster->scheduleOnce(
        [monster](float) {
            if (monster->getActionState() != ACTION_STATE_DEAD && monster->getActionState() != ACTION_STATE_ATTACK)
            {
                // virtual: runs Monster::dealloc, which leaves the master's monster list
                static_cast<ActionManager*>(monster)->dealloc();
            }
        },
        kRideMonsterLifetime, "rideMonsterLifetime");

    _delegate->addChild(monster, -monster->getPositionY());
}
