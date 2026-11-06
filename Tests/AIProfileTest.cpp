#include "doctest.h"
#include "AI/AIProfile.h"

namespace
{
// the selectable roster (SelectLayer)
const char* const kHeroes[] = {"Naruto",   "Sakura", "Sai",     "Kakashi",  "Asuma",  "Shikamaru", "Choji",
                               "Ino",      "Hinata", "Neji",    "Kiba",     "Shino",  "Tenten",    "Lee",
                               "Tsunade",  "Jiraiya", "Minato", "Hiruzen",  "Tobirama", "Gaara",   "Kankuro",
                               "Chiyo",    "Itachi", "Kisame",  "Deidara",  "Konan",  "Tobi",      "Hidan",
                               "Kakuzu",   "Sasuke", "Suigetsu", "Karin",   "Jugo",   "Kurenai"};
}

TEST_CASE("every selectable hero has its own AI profile")
{
    for (const char* hero : kHeroes)
    {
        CAPTURE(hero);
        const ai::Profile* profile = ai::findProfile(hero);
        REQUIRE(profile != nullptr);
        CHECK(profile->name == hero);
        CHECK_FALSE(profile->hero.empty());
    }
}

TEST_CASE("profile gear builds only use existing gears")
{
    for (const char* hero : kHeroes)
    {
        const ai::Profile* profile = ai::findProfile(hero);
        REQUIRE(profile != nullptr);
        for (const auto& [buildName, gears] : profile->builds)
        {
            CAPTURE(hero);
            CAPTURE(buildName);
            CHECK(gears.size() == 3);
            for (int gear : gears)
                CHECK((gear >= 0 && gear <= 11));
        }
    }
}

TEST_CASE("unknown heroes fall back to the default profile, guardians to the legacy AI")
{
    CHECK(ai::findProfile("NotAHero") == nullptr);
    CHECK(ai::defaultProfile("NotAHero") != nullptr);
    CHECK(ai::defaultProfile("Han") == nullptr);
    CHECK(ai::defaultProfile("Roshi") == nullptr);
}
