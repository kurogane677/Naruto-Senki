#include "Heroes/HeroRegistry.h"
#include <algorithm>

namespace HeroRegistry
{

namespace
{
constexpr int kHardCoreSelectButtons = 3 * 21;
const char* const kEmptySelectButton = "None";

std::vector<std::string> namesWhere(bool (*keep)(const HeroInfo&))
{
    std::vector<std::string> names;
    for (const HeroInfo& hero : heroes())
        if (keep(hero))
            names.push_back(hero.name);
    return names;
}

// names placed by slot; slots nobody uses keep `empty`
std::vector<std::string> namesBySlot(int HeroInfo::*slot, size_t size, const char* empty)
{
    std::vector<std::string> names(size, empty);
    for (const HeroInfo& hero : heroes())
        if (hero.*slot != kNoSlot)
            names.at(hero.*slot) = hero.name;
    return names;
}

int slotCount(int HeroInfo::*slot)
{
    return (int)std::count_if(heroes().begin(), heroes().end(),
                              [slot](const HeroInfo& hero) { return hero.*slot != kNoSlot; });
}
}  // namespace

const std::vector<HeroInfo>& heroes()
{
    // clang-format off
    static const std::vector<HeroInfo> table = {
        // name        record training  training  hardCore
        //                    opponent  slot      slot (sorted)
        {"Naruto",     true,  true,     0,        0},
        {"Sakura",     true,  true,     1,        1},
        {"Sai",        true,  true,     2,        2},
        {"Kakashi",    true,  true,     kNoSlot,  3},
        {"Shikamaru",  true,  false,    kNoSlot,  7},
        {"Ino",        true,  true,     kNoSlot,  8},
        {"Choji",      true,  true,     kNoSlot,  9},
        {"Asuma",      true,  true,     kNoSlot,  10},
        {"Kiba",       true,  true,     kNoSlot,  11},
        {"Hinata",     true,  true,     kNoSlot,  12},
        {"Shino",      true,  true,     kNoSlot,  13},
        {"Neji",       true,  true,     kNoSlot,  14},
        {"Tenten",     true,  true,     kNoSlot,  15},
        {"Lee",        true,  true,     8,        16},
        {"Kurenai",    true,  true,     kNoSlot,  17},
        {"Tobirama",   true,  true,     kNoSlot,  22},
        {"Hiruzen",    true,  true,     kNoSlot,  23},
        {"Minato",     true,  true,     kNoSlot,  24},
        {"Jiraiya",    true,  true,     kNoSlot,  25},
        {"Tsunade",    true,  true,     kNoSlot,  26},
        {"Orochimaru", false, false,    kNoSlot,  27},
        {"Deidara",    true,  true,     5,        29},
        {"Kakuzu",     true,  true,     kNoSlot,  30},
        {"Hidan",      true,  true,     kNoSlot,  31},
        {"Tobi",       true,  true,     kNoSlot,  32},
        {"Konan",      true,  true,     4,        33},
        {"Pain",       false, false,    kNoSlot,  34},
        {"Itachi",     true,  true,     3,        35},
        {"Kisame",     true,  false,    kNoSlot,  36},
        {"Sasuke",     true,  true,     6,        42},
        {"Karin",      true,  true,     7,        43},
        {"Suigetsu",   true,  true,     kNoSlot,  44},
        {"Jugo",       true,  true,     kNoSlot,  45},
        {"Gaara",      true,  true,     kNoSlot,  49},
        {"Kankuro",    true,  true,     kNoSlot,  51},
        {"Chiyo",      true,  false,    kNoSlot,  52},
    };
    // clang-format on
    return table;
}

std::vector<std::string> recordHeroes()
{
    return namesWhere([](const HeroInfo& hero) { return hero.hasRecord; });
}

std::vector<std::string> trainingOpponents()
{
    return namesWhere([](const HeroInfo& hero) { return hero.trainingOpponent; });
}

std::vector<std::string> trainingSelectScreen()
{
    return namesBySlot(&HeroInfo::trainingSlot, slotCount(&HeroInfo::trainingSlot), kEmptySelectButton);
}

std::vector<std::string> hardCoreSelectScreen()
{
    return namesBySlot(&HeroInfo::hardCoreSlot, kHardCoreSelectButtons, kEmptySelectButton);
}

}  // namespace HeroRegistry
