#include "doctest.h"
#include "Heroes/HeroFactory.h"
#include "Heroes/HeroRegistry.h"

#include <set>

// Creating the objects needs a renderer, so these check the table HeroFactory::create
// looks up; the objects themselves are covered by the soak.
TEST_CASE("a hero's forms share its hero class")
{
    const auto& classes = HeroFactory::heroClasses();
    REQUIRE(classes.count(CharacterId::Naruto));
    CHECK(classes.at(CharacterId::SageNaruto) == classes.at(CharacterId::Naruto));
    CHECK(classes.at(CharacterId::RikudoNaruto) == classes.at(CharacterId::Naruto));
    CHECK(classes.at(CharacterId::Sakura) != classes.at(CharacterId::Naruto));
    CHECK(classes.count(CharacterId::Kurama) == 0);
}

TEST_CASE("only heroes and their forms have a hero class")
{
    std::set<CharacterId> heroesAndForms;
    for (const HeroRegistry::HeroInfo& hero : HeroRegistry::heroes())
        for (const std::string& form : HeroRegistry::forms(hero.name))
            heroesAndForms.insert(characterIdFromString(form.c_str()));

    for (const auto& entry : HeroFactory::heroClasses())
    {
        CAPTURE(static_cast<int>(entry.first));
        CHECK(heroesAndForms.count(entry.first) == 1);
    }
}
