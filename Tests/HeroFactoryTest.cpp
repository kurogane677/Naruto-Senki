#include "doctest.h"
#include "Heroes/HeroFactory.h"
#include "Heroes/HeroRegistry.h"
#include "Units/MonsterFactory.h"

#include <set>

// Creating the objects needs a renderer, so these check the tables the factories
// look up; the objects themselves are covered by the soak.
TEST_CASE("a hero's forms share its hero class")
{
    const auto& classes = HeroFactory::heroClasses().classes();
    REQUIRE(classes.count(CharacterId::Naruto));
    CHECK(classes.at(CharacterId::SageNaruto) == classes.at(CharacterId::Naruto));
    CHECK(classes.at(CharacterId::RikudoNaruto) == classes.at(CharacterId::Naruto));
    CHECK(classes.at(CharacterId::Sakura) != classes.at(CharacterId::Naruto));
    CHECK(classes.count(CharacterId::Kurama) == 0);
}

static std::set<CharacterId> heroesAndForms()
{
    std::set<CharacterId> ids;
    for (const HeroRegistry::HeroInfo& hero : HeroRegistry::heroes())
        for (const std::string& form : HeroRegistry::forms(hero.name))
            ids.insert(characterIdFromString(form.c_str()));
    return ids;
}

TEST_CASE("only heroes and their forms have a hero class")
{
    const std::set<CharacterId> heroes = heroesAndForms();
    for (const auto& entry : HeroFactory::heroClasses().classes())
    {
        CAPTURE(static_cast<int>(entry.first));
        CHECK(heroes.count(entry.first) == 1);
    }
}

TEST_CASE("monsters that behave the same share a monster class")
{
    const auto& classes = MonsterFactory::monsterClasses().classes();
    REQUIRE(classes.count(CharacterId::FudonSRK));
    CHECK(classes.at(CharacterId::FudonSRK2) == classes.at(CharacterId::FudonSRK));
    CHECK(classes.at(CharacterId::Bikyu) != classes.at(CharacterId::FudonSRK));
    CHECK(classes.count(CharacterId::Kage) == 0);
}

TEST_CASE("no hero has a monster class")
{
    const std::set<CharacterId> heroes = heroesAndForms();
    for (const auto& entry : MonsterFactory::monsterClasses().classes())
    {
        CAPTURE(static_cast<int>(entry.first));
        CHECK(heroes.count(entry.first) == 0);
    }
}
