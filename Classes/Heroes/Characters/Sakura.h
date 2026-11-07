#pragma once
#include "Characters.h"

// Sakura: her setTransport skill leaps up instead of teleporting, and her
// transform skill swaps her basic attack for a stronger punch while it lasts.
class Sakura : public Hero
{
public:
    CREATE_FUNC(Sakura);

protected:
    bool onTransportCommand() override;
    bool onTransformSkillStart() override;
    bool onTransformSkillEnd() override;
};
