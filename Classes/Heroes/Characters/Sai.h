#pragma once
#include "Heroes/Characters/RidingHero.h"

// Sai rides an ink bird that drops ink mice
class Sai : public RidingHero
{
public:
    CREATE_FUNC(Sai);

protected:
    const char* rideMonster() const override { return "Mouse"; }
    void onRideMonsterSpawned() override;
};
