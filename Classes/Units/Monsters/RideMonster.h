#pragma once
#include "Element.h"

// What a ride drops (Sai's Mouse, Deidara's Spider): it goes after heroes only;
// towers and creeps are left to the rest of the team (#69).
class RideMonster : public Monster
{
public:
    CREATE_FUNC(RideMonster);

protected:
    bool targetsHeroesOnly() const override { return true; }
};
