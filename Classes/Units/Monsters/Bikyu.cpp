#include "Units/Monsters/Bikyu.h"

namespace
{
constexpr float kGap = 16;      // from the owner's front edge
constexpr float kDropY = 32;
}  // namespace

bool Bikyu::onSpawn(ActionManager* owner)
{
    setFlippedX(owner->_isFlipped);
    isPofang = true;
    float offsetX = kGap + owner->getContentSize().width;
    setPosition(ccp(owner->getPositionX() + (owner->_isFlipped ? -offsetX : offsetX), owner->getPositionY() - kDropY));
    owner->getMonsterArray()->addObject(this);
    attack(NAttack);
    return true;
}
