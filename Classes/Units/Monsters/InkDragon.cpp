#include "Units/Monsters/InkDragon.h"

namespace
{
constexpr float kSpawnDistance = 128;
constexpr unsigned int kFlyDistance = 156;
constexpr float kFlyTime = 2.0f;
}  // namespace

bool InkDragon::onSpawn(ActionManager* owner)
{
    setPosition(ccp(owner->getPositionX() + (owner->_isFlipped ? -kSpawnDistance : kSpawnDistance), owner->getPositionY()));
    attack(NAttack);
    setDirectMove(kFlyDistance, kFlyTime, false);
    return true;
}
