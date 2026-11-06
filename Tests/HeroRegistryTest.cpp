#include "doctest.h"
#include "Heroes/HeroRegistry.h"
#include <set>

using Names = std::vector<std::string>;

// The lists below are the arrays the registry replaced, copied as they were in
// v0.0.7-beta. Select screens must keep the same heroes in the same places; the
// COM pools must keep the same heroes (each is drawn with the same chance in any order).

static std::set<std::string> asSet(const Names& names)
{
    return {names.begin(), names.end()};
}

TEST_CASE("CharRecord heroes and HardCore COM opponents (KTools, NetworkLayer)")
{
    const Names expected = {"Konan",  "Sakura",  "Naruto",  "Sai",     "Deidara", "Kakashi", "Itachi",
                            "Tenten", "Jiraiya", "Suigetsu", "Tsunade", "Tobirama", "Neji",    "Ino",
                            "Asuma",  "Gaara",   "Karin",   "Sasuke",  "Hidan",   "Choji",   "Kankuro",
                            "Shino",  "Minato",  "Tobi",    "Kakuzu",  "Hinata",  "Shikamaru", "Chiyo",
                            "Kisame", "Hiruzen", "Kiba",    "Jugo",    "Lee",     "Kurenai"};
    CHECK(HeroRegistry::recordHeroes().size() == expected.size());
    CHECK(asSet(HeroRegistry::recordHeroes()) == asSet(expected));
}

TEST_CASE("Training COM opponents (SelectLayer)")
{
    const Names expected = {"Konan",   "Sakura", "Naruto",  "Sai",    "Deidara", "Kakashi",  "Itachi", "Tenten",
                            "Jiraiya", "Suigetsu", "Tsunade", "Tobirama", "Neji", "Ino",     "Asuma",  "Gaara",
                            "Karin",   "Sasuke", "Hidan",   "Choji",  "Kankuro", "Shino",    "Minato", "Tobi",
                            "Kakuzu",  "Hinata", "Hiruzen", "Kiba",   "Jugo",    "Lee",      "Kurenai"};
    CHECK(HeroRegistry::trainingOpponents().size() == expected.size());
    CHECK(asSet(HeroRegistry::trainingOpponents()) == asSet(expected));
}

TEST_CASE("Training select screen (SelectLayer)")
{
    const Names expected = {"Naruto", "Sakura", "Sai", "Itachi", "Konan", "Deidara", "Sasuke", "Karin", "Jugo"};
    CHECK(HeroRegistry::trainingSelectScreen() == expected);
}

TEST_CASE("HardCore select pages (NetworkLayer)")
{
    const Names expected = {
        "Naruto", "Sakura",   "Sai",      "Kakashi",  "None",       "None",    "None",
        "Shikamaru", "Ino",   "Choji",    "Asuma",    "Kiba",       "Hinata",  "Shino",
        "Neji",   "Tenten",   "Lee",      "Kurenai",  "None",       "None",    "None",

        "None",   "Tobirama", "Hiruzen",  "Minato",   "Jiraiya",    "Tsunade", "Orochimaru",
        "None",   "Deidara",  "Kakuzu",   "Hidan",    "Tobi",       "Konan",   "Pain",
        "Itachi", "Kisame",   "None",     "None",     "None",       "None",    "None",

        "Sasuke", "Karin",    "Suigetsu", "Jugo",     "None",       "None",    "None",
        "Gaara",  "None",     "Kankuro",  "Chiyo",    "None",       "None",    "None",
        "None",   "None",     "None",     "None",     "None",       "None",    "None",
    };
    CHECK(HeroRegistry::hardCoreSelectScreen() == expected);
}

TEST_CASE("the heroes with a stealth skill (stBuff in their XML)")
{
    std::set<std::string> stealthHeroes;
    for (const HeroRegistry::HeroInfo& hero : HeroRegistry::heroes())
        if (hero.stealthSkill)
            stealthHeroes.insert(hero.name);
    CHECK(stealthHeroes == std::set<std::string>{"Konan", "Deidara", "Kurenai"});
    CHECK(HeroRegistry::hasStealthSkill("Kurenai"));
    CHECK_FALSE(HeroRegistry::hasStealthSkill("Naruto"));
    CHECK_FALSE(HeroRegistry::hasStealthSkill("Akamaru"));
}

TEST_CASE("hero names and slots are unique")
{
    std::set<std::string> names;
    std::set<int> trainingSlots, hardCoreSlots;
    for (const HeroRegistry::HeroInfo& hero : HeroRegistry::heroes())
    {
        CAPTURE(hero.name);
        CHECK(names.insert(hero.name).second);
        if (hero.trainingSlot != HeroRegistry::kNoSlot)
            CHECK(trainingSlots.insert(hero.trainingSlot).second);
        if (hero.hardCoreSlot != HeroRegistry::kNoSlot)
            CHECK(hardCoreSlots.insert(hero.hardCoreSlot).second);
        // a COM opponent needs a record row
        if (hero.trainingOpponent)
            CHECK(hero.hasRecord);
    }
}
