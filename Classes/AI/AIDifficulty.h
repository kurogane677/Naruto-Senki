#pragma once

namespace ai
{

enum class Difficulty
{
    Easy = 0,
    Normal = 1,
    Hard = 2,
};

// Tunables per difficulty level. Applies to every bot, allies included.
struct DifficultyParams
{
    float thinkMin, thinkMax;        // seconds before a bot re-picks its target
    float castDelayMin, castDelayMax;  // hesitation after a skill comes off cooldown
    float castChance;                // chance to take a skill opportunity
    float potionHp;                  // drink a potion below this HP fraction
    float retreatHp;                 // walk home below this HP fraction
    float dodgeChance;               // chance to step out of an enemy ougis
    bool combos;                     // save ougis for a downed/stunned target
    bool focusFire;                  // follow the team's focus target
    bool strictGp;                   // keep the original "target GP < 5000" skill gate
    float playerBuildChance;         // HardCore: chance to copy the player's best build
};

inline const DifficultyParams& params(Difficulty d)
{
    static const DifficultyParams table[] = {
        // think        castDelay    chance potion retreat dodge  combo  focus  strictGp build
        {0.35f, 0.60f, 0.80f, 1.50f, 0.50f, 0.15f, 0.10f, 0.00f, false, false, true,  0.0f},  // Easy
        {0.20f, 0.35f, 0.30f, 0.80f, 0.75f, 0.35f, 0.15f, 0.40f, true,  true,  true,  0.3f},  // Normal
        {0.10f, 0.15f, 0.00f, 0.20f, 0.95f, 0.50f, 0.20f, 0.80f, true,  true,  false, 0.6f},  // Hard
    };
    return table[(int)d];
}

}  // namespace ai
