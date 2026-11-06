#include "doctest.h"
#include "Heroes/HeroRegistry.h"
#include <set>

using Names = std::vector<std::string>;

// The lists below are the arrays the registry replaced, copied as they were in
// v0.0.7-beta; every screen must keep the same heroes in the same places.

TEST_CASE("CharRecord heroes and HardCore COM opponents (KTools, NetworkLayer)")
{
    const Names expected = {"Konan",  "Sakura",  "Naruto",  "Sai",     "Deidara", "Kakashi", "Itachi",
                            "Tenten", "Jiraiya", "Suigetsu", "Tsunade", "Tobirama", "Neji",    "Ino",
                            "Asuma",  "Gaara",   "Karin",   "Sasuke",  "Hidan",   "Choji",   "Kankuro",
                            "Shino",  "Minato",  "Tobi",    "Kakuzu",  "Hinata",  "Shikamaru", "Chiyo",
                            "Kisame", "Hiruzen", "Kiba",    "Jugo",    "Lee",     "Kurenai"};
    CHECK(HeroRegistry::recordHeroes() == expected);
}

TEST_CASE("Training COM opponents (SelectLayer)")
{
    const Names expected = {"Konan",   "Sakura", "Naruto",  "Sai",    "Deidara", "Kakashi",  "Itachi", "Tenten",
                            "Jiraiya", "Suigetsu", "Tsunade", "Tobirama", "Neji", "Ino",     "Asuma",  "Gaara",
                            "Karin",   "Sasuke", "Hidan",   "Choji",  "Kankuro", "Shino",    "Minato", "Tobi",
                            "Kakuzu",  "Hinata", "Hiruzen", "Kiba",   "Jugo",    "Lee",      "Kurenai"};
    CHECK(HeroRegistry::trainingOpponents() == expected);
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

TEST_CASE("Ranking hero grid (RankingLayer)")
{
    const Names expected = {"Naruto",   "Sakura",  "Sai",     "Kakashi", "Lee",      "Tenten", "Neji",
                            "Shikamaru", "Ino",    "Choji",   "Asuma",   "Shino",    "Hinata", "Kiba",
                            "Kurenai",  "Tobirama", "Hiruzen", "Minato", "Jiraiya",  "Tsunade", "Gaara",
                            "Kankuro",  "Chiyo",   "Sasuke",  "Karin",   "Suigetsu", "Jugo",   "Deidara",
                            "Tobi",     "Hidan",   "Kakuzu",  "Konan",   "Itachi",   "Kisame"};
    CHECK(HeroRegistry::rankingList() == expected);
}

TEST_CASE("hero names and slots are unique")
{
    std::set<std::string> names;
    std::set<int> trainingSlots, hardCoreSlots, rankingSlots;
    for (const HeroRegistry::HeroInfo& hero : HeroRegistry::heroes())
    {
        CAPTURE(hero.name);
        CHECK(names.insert(hero.name).second);
        if (hero.trainingSlot != HeroRegistry::kNoSlot)
            CHECK(trainingSlots.insert(hero.trainingSlot).second);
        if (hero.hardCoreSlot != HeroRegistry::kNoSlot)
            CHECK(hardCoreSlots.insert(hero.hardCoreSlot).second);
        if (hero.rankingSlot != HeroRegistry::kNoSlot)
            CHECK(rankingSlots.insert(hero.rankingSlot).second);
        // a COM opponent needs a record row; only record heroes are ranked
        if (hero.trainingOpponent)
            CHECK(hero.hasRecord);
        CHECK(hero.hasRecord == (hero.rankingSlot != HeroRegistry::kNoSlot));
    }
}
