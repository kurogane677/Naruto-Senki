#pragma once
#include <string>
#include <vector>

// The one list of playable heroes. A new hero is added here, with where it appears
// on each screen; every hero list in the game is built from this table.
namespace HeroRegistry
{

constexpr int kNoSlot = -1;

struct HeroInfo
{
    const char* name;
    bool hasRecord;         // wins/games row in CharRecord, COM opponent in HardCore, Ranking entry
    bool trainingOpponent;  // can be drawn as a COM in Training
    int trainingSlot;       // place on the Training select screen (3x3), or kNoSlot
    int hardCoreSlot;       // place on the HardCore select pages (3 pages of 3x7), or kNoSlot
    int rankingSlot;        // place in the Ranking hero grid, or kNoSlot
};

// in CharRecord order, the order COM opponents are drawn from
const std::vector<HeroInfo>& heroes();

std::vector<std::string> recordHeroes();
std::vector<std::string> trainingOpponents();
std::vector<std::string> trainingSelectScreen();
// one entry per HardCore select button, "None" for an empty button
std::vector<std::string> hardCoreSelectScreen();
std::vector<std::string> rankingList();

}  // namespace HeroRegistry
