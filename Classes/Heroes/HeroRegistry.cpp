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

struct FormChain
{
    std::vector<const char*> forms;  // base form first
    bool stance;                     // switches back from the last form to the first
};

const std::vector<FormChain>& formChains()
{
    static const std::vector<FormChain> chains = {
        {{"Naruto", "SageNaruto", "RikudoNaruto"}, false},
        {{"Jiraiya", "SageJiraiya"}, false},
        {{"Sasuke", "ImmortalSasuke"}, false},
        {{"Lee", "RockLee"}, true},
    };
    return chains;
}

// the chain holding `name` and its place in it; nullptr when it has no other form
const FormChain* findChain(const std::string& name, size_t& index)
{
    for (const FormChain& chain : formChains())
        for (index = 0; index < chain.forms.size(); index++)
            if (name == chain.forms[index])
                return &chain;
    return nullptr;
}
}  // namespace

const std::vector<HeroInfo>& heroes()
{
    // clang-format off
    static const std::vector<HeroInfo> table = {
        // name        record training  training  hardCore  stealth
        //                    opponent  slot      slot      skill
        //                                        (sorted)
        {"Naruto",     true,  true,     0,        0,        false},
        {"Sakura",     true,  true,     1,        1,        false},
        {"Sai",        true,  true,     2,        2,        false},
        {"Kakashi",    true,  true,     kNoSlot,  3,        false},
        {"Shikamaru",  true,  false,    kNoSlot,  7,        false},
        {"Ino",        true,  true,     kNoSlot,  8,        false},
        {"Choji",      true,  true,     kNoSlot,  9,        false},
        {"Asuma",      true,  true,     kNoSlot,  10,       false},
        {"Kiba",       true,  true,     kNoSlot,  11,       false},
        {"Hinata",     true,  true,     kNoSlot,  12,       false},
        {"Shino",      true,  true,     kNoSlot,  13,       false},
        {"Neji",       true,  true,     kNoSlot,  14,       false},
        {"Tenten",     true,  true,     kNoSlot,  15,       false},
        {"Lee",        true,  true,     kNoSlot,  16,       false},
        {"Kurenai",    true,  true,     kNoSlot,  17,       true},
        {"Tobirama",   true,  true,     kNoSlot,  22,       false},
        {"Hiruzen",    true,  true,     kNoSlot,  23,       false},
        {"Minato",     true,  true,     kNoSlot,  24,       false},
        {"Jiraiya",    true,  true,     kNoSlot,  25,       false},
        {"Tsunade",    true,  true,     kNoSlot,  26,       false},
        {"Orochimaru", false, false,    kNoSlot,  27,       false},
        {"Deidara",    true,  true,     5,        29,       true},
        {"Kakuzu",     true,  true,     kNoSlot,  30,       false},
        {"Hidan",      true,  true,     kNoSlot,  31,       false},
        {"Tobi",       true,  true,     kNoSlot,  32,       false},
        {"Konan",      true,  true,     4,        33,       true},
        {"Pain",       false, false,    kNoSlot,  34,       false},
        {"Itachi",     true,  true,     3,        35,       false},
        {"Kisame",     true,  false,    kNoSlot,  36,       false},
        {"Sasuke",     true,  true,     6,        42,       false},
        {"Karin",      true,  true,     7,        43,       false},
        {"Suigetsu",   true,  true,     kNoSlot,  44,       false},
        {"Jugo",       true,  true,     8,        45,       false},
        {"Gaara",      true,  true,     kNoSlot,  49,       false},
        {"Kankuro",    true,  true,     kNoSlot,  51,       false},
        {"Chiyo",      true,  false,    kNoSlot,  52,       false},
    };
    // clang-format on
    return table;
}

bool hasStealthSkill(const std::string& name)
{
    for (const HeroInfo& hero : heroes())
        if (name == hero.name)
            return hero.stealthSkill;
    return false;
}

const char* nextForm(const std::string& name)
{
    size_t index;
    const FormChain* chain = findChain(name, index);
    if (!chain)
        return nullptr;
    if (index + 1 < chain->forms.size())
        return chain->forms[index + 1];
    return chain->stance ? chain->forms.front() : nullptr;
}

const char* nextAwakening(const std::string& name)
{
    size_t index;
    const FormChain* chain = findChain(name, index);
    return chain && !chain->stance ? nextForm(name) : nullptr;
}

std::string baseForm(const std::string& name)
{
    size_t index;
    const FormChain* chain = findChain(name, index);
    return chain ? chain->forms.front() : name;
}

std::vector<std::string> forms(const std::string& name)
{
    size_t index;
    const FormChain* chain = findChain(name, index);
    return chain ? std::vector<std::string>(chain->forms.begin(), chain->forms.end()) : std::vector<std::string>{name};
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
