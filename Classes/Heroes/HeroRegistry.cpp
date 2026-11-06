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
        // name        record training  training  hardCore  ranking
        //                    opponent  slot      slot      slot
        {"Konan",      true,  true,     4,        33,       31},
        {"Sakura",     true,  true,     1,        1,        1},
        {"Naruto",     true,  true,     0,        0,        0},
        {"Sai",        true,  true,     2,        2,        2},
        {"Deidara",    true,  true,     5,        29,       27},
        {"Kakashi",    true,  true,     kNoSlot,  3,        3},
        {"Itachi",     true,  true,     3,        35,       32},
        {"Tenten",     true,  true,     kNoSlot,  15,       5},
        {"Jiraiya",    true,  true,     kNoSlot,  25,       18},
        {"Suigetsu",   true,  true,     kNoSlot,  44,       25},
        {"Tsunade",    true,  true,     kNoSlot,  26,       19},
        {"Tobirama",   true,  true,     kNoSlot,  22,       15},
        {"Neji",       true,  true,     kNoSlot,  14,       6},
        {"Ino",        true,  true,     kNoSlot,  8,        8},
        {"Asuma",      true,  true,     kNoSlot,  10,       10},
        {"Gaara",      true,  true,     kNoSlot,  49,       20},
        {"Karin",      true,  true,     7,        43,       24},
        {"Sasuke",     true,  true,     6,        42,       23},
        {"Hidan",      true,  true,     kNoSlot,  31,       29},
        {"Choji",      true,  true,     kNoSlot,  9,        9},
        {"Kankuro",    true,  true,     kNoSlot,  51,       21},
        {"Shino",      true,  true,     kNoSlot,  13,       11},
        {"Minato",     true,  true,     kNoSlot,  24,       17},
        {"Tobi",       true,  true,     kNoSlot,  32,       28},
        {"Kakuzu",     true,  true,     kNoSlot,  30,       30},
        {"Hinata",     true,  true,     kNoSlot,  12,       12},
        {"Shikamaru",  true,  false,    kNoSlot,  7,        7},
        {"Chiyo",      true,  false,    kNoSlot,  52,       22},
        {"Kisame",     true,  false,    kNoSlot,  36,       33},
        {"Hiruzen",    true,  true,     kNoSlot,  23,       16},
        {"Kiba",       true,  true,     kNoSlot,  11,       13},
        {"Jugo",       true,  true,     8,        45,       26},
        {"Lee",        true,  true,     kNoSlot,  16,       4},
        {"Kurenai",    true,  true,     kNoSlot,  17,       14},
        {"Orochimaru", false, false,    kNoSlot,  27,       kNoSlot},
        {"Pain",       false, false,    kNoSlot,  34,       kNoSlot},
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

std::vector<std::string> rankingList()
{
    return namesBySlot(&HeroInfo::rankingSlot, slotCount(&HeroInfo::rankingSlot), kEmptySelectButton);
}

}  // namespace HeroRegistry
