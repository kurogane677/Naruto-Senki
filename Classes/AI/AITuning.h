#pragma once

namespace ai
{

// How every bot plays (allies and enemies alike). There is no difficulty
// choice: bots always use what used to be the "Hard" level.
struct Tuning
{
    float thinkMin, thinkMax;          // seconds before a bot re-picks its target
    float castDelayMin, castDelayMax;  // hesitation after a skill comes off cooldown
    float castChance;                  // chance to take a skill opportunity
    float potionHp;                    // drink a potion below this HP fraction
    float retreatHp;                   // walk home below this HP fraction
    float dodgeChance;                 // chance to step out of an enemy ougis
    bool combos;                       // follow up on a downed/stunned target
    bool focusFire;                    // follow the team's focus target
    bool strictGp;                     // keep the original "target GP < 5000" skill gate
    float playerBuildChance;           // HardCore: chance to copy the player's best build
};

inline const Tuning& tuning()
{
    static const Tuning t = {0.10f, 0.15f, 0.00f, 0.20f, 0.95f, 0.50f, 0.20f, 0.80f, true, true, false, 0.6f};
    return t;
}

}  // namespace ai
