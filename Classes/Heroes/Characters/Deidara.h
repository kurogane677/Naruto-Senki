#pragma once
#include "Heroes/Characters/RidingHero.h"

// Deidara rides a clay bird that drops clay spiders. His other branches still live in
// ActionManager/Element until his own turn in the hero order.
class Deidara : public RidingHero
{
public:
    CREATE_FUNC(Deidara);

protected:
    const char* rideMonster() const override { return "Spider"; }
};
