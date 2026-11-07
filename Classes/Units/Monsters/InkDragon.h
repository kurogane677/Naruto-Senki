#pragma once
#include "Element.h"

// Sai's ink dragon: appears ahead of him and flies forward
class InkDragon : public Monster
{
public:
    CREATE_FUNC(InkDragon);
    bool onSpawn(ActionManager* owner) override;
};
