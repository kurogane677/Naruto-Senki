#include "Heroes/Characters/Sakura.h"

namespace
{
constexpr float kLeapHeight = 64;
constexpr int kPunchBonus = 400;
}  // namespace

// leaps up where she stands; reTransport brings her back down to _originY
bool Sakura::onTransportCommand()
{
    if (!_originY)
        _originY = getPositionY() == 0 ? 0.1f : getPositionY();  // 0 would mean "not in the air"
    setPosition(ccp(getPositionX(), getPositionY() + kLeapHeight));
    return true;
}

bool Sakura::onTransformSkillStart()
{
    setNAttackAction(createAnimation(skillSPC1Array, 10.0f, false, true));
    setnAttackValue(_nattackValue + kPunchBonus);
    return true;
}

bool Sakura::onTransformSkillEnd()
{
    setNAttackAction(createAnimation(nattackArray, 10.0f, false, true));
    setnAttackValue(_nattackValue - kPunchBonus);
    return true;
}
