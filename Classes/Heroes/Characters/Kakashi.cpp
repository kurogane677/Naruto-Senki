#include "Heroes/Characters/Kakashi.h"
#include "HudLayer.h"

namespace
{
constexpr float kDogWallDistance = 56;
constexpr int kDogWallAttack = 1060;
}  // namespace

bool Kakashi::onTransformSkillStart()
{
    setSkill1Action(createAnimation(skillSPC1Array, 10.0f, false, true));
    setSkill2Action(createAnimation(skillSPC2Array, 10.0f, false, true));
    setIdleAction(createAnimation(skillSPC3Array, 5.0f, true, false));
    settempAttackValue1(_sattackValue1);
    setsAttackValue1(_spcattackValue1);

    // the Sharingan sees through stealth within half a screen
    CCObject* object;
    CCARRAY_FOREACH(_delegate->_CharacterArray, object)
    {
        ActionManager* enemy = (ActionManager*)object;
        if (group() != enemy->group() && (enemy->role() == Role::Player || enemy->role() == Role::Com) &&
            enemy->getActionState() != ACTION_STATE_HURT && enemy->getActionState() != ACTION_STATE_DEAD &&
            abs(ccpSub(enemy->getPosition(), getPosition()).x) < kGameWidth / 2)
            enemy->breakStealth();
    }

    showSkillIcons("Kakashi_skill1_1.png", "Kakashi_skill2_1.png");
    return true;
}

bool Kakashi::onTransformSkillEnd()
{
    setIdleAction(createAnimation(idleArray, 5.0f, true, false));
    setSkill1Action(createAnimation(skill1Array, 10.0f, false, true));
    setSkill2Action(createAnimation(skill2Array, 10.0f, false, true));
    setsAttackValue1(_tempAttackValue1);
    showSkillIcons("Kakashi_skill1.png", "Kakashi_skill2.png");
    return true;
}

// the player's skill buttons show whichever version is active
void Kakashi::showSkillIcons(const char* skill1, const char* skill2)
{
    if (role() != Role::Player)
        return;
    HudLayer* hud = _delegate->getHudLayer();
    hud->skill1Button->setSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->getSpriteFrameByName(skill1));
    hud->skill2Button->setSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->getSpriteFrameByName(skill2));
}

bool Kakashi::setUpClone(Hero* clone)
{
    clone->setID(CCString::create("DogWall"), CCString::create("Summon"), _group);
    clone->setPosition(ccp(getPositionX() + (_isFlipped ? -kDogWallDistance : kDogWallDistance), getPositionY()));
    clone->setAnchorPoint(ccp(0.5f, 0.1f));
    clone->_isBati = true;
    return true;
}

void Kakashi::finishClone(Hero* clone)
{
    clone->setnAttackValue(kDogWallAttack);
}
