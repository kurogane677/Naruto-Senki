#include "doctest.h"
#include "AI/AIProfile.h"
#include "Heroes/HeroRegistry.h"

TEST_CASE("every hero with a record has its own AI profile")
{
    for (const std::string& hero : HeroRegistry::recordHeroes())
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
    for (const std::string& hero : HeroRegistry::recordHeroes())
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
