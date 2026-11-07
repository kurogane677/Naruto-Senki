#pragma once
#include "Element.h"

// Sage Naruto's Bikyu: appears just in front of him, breaks through guard (isPofang),
// and stays tied to him in his monster list.
class Bikyu : public Monster
{
public:
    CREATE_FUNC(Bikyu);
    bool onSpawn(ActionManager* owner) override;
};
