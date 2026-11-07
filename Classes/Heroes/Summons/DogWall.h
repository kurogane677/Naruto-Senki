#pragma once
#include "Characters.h"

// Kakashi's summoned wall of dogs: it stays put and keeps attacking until it expires.
// While it stands, enemy creeps close by turn on it (aggro); once it is gone they go
// back to their own targets.
class DogWall : public Hero
{
public:
    CREATE_FUNC(DogWall);
    void dealloc() override;

protected:
    void ownAI() override;

private:
    CCArray* enemyCreeps() const;
    void pullCreeps();
    void releaseCreeps();
};
