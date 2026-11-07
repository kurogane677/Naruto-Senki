#pragma once
#include "Characters.h"

// Kakashi: his Sharingan buff swaps skill 1 and 2 for their stronger versions (and
// reveals hidden enemies nearby); skill04 summons the DogWall.
class Kakashi : public Hero
{
public:
    CREATE_FUNC(Kakashi);

protected:
    bool onTransformSkillStart() override;
    bool onTransformSkillEnd() override;
    bool swapsSkillAnimations() const override { return true; }
    CharacterId cloneCharacter() const override { return CharacterId::DogWall; }
    bool setUpClone(Hero* clone) override;
    void finishClone(Hero* clone) override;
    bool clonesExpire() const override { return true; }

private:
    void showSkillIcons(const char* skill1, const char* skill2);
};
