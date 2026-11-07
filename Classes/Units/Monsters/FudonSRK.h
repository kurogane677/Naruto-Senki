#pragma once
#include "Element.h"

// Sage and Rikudo Naruto's Rasenshuriken (FudonSRK, FudonSRK2): it chases the
// target its owner had, or flies straight ahead, and bursts on the first hero it reaches.
class FudonSRK : public Monster
{
public:
    CREATE_FUNC(FudonSRK);
    bool onSpawn(ActionManager* owner) override;

protected:
    bool attackEndsInIdle() const override { return false; }
    void setAI(float dt) override;
    void dealloc() override;
};
