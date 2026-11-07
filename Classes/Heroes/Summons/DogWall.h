#pragma once
#include "Characters.h"

// Kakashi's summoned wall of dogs: it stays put and keeps attacking until it expires
class DogWall : public Hero
{
public:
    CREATE_FUNC(DogWall);

protected:
    void ownAI() override { attack(NAttack); }
};
