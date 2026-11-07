#include "doctest.h"
#include "Heroes/HeroAssets.h"
#include "Heroes/HeroRegistry.h"

#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;

static bool contains(const std::vector<std::string>& list, const std::string& item)
{
    return std::find(list.begin(), list.end(), item) != list.end();
}

TEST_CASE("a hero brings its skills, every form and its summons")
{
    const std::vector<std::string> naruto = HeroAssets::sheets("Naruto");
    CHECK(contains(naruto, "Element/Skills/Naruto_Skill.plist"));
    CHECK(contains(naruto, "Element/Naruto/Naruto.plist"));
    CHECK(contains(naruto, "Element/SageNaruto/SageNaruto.plist"));
    CHECK(contains(naruto, "Element/RikudoNaruto/RikudoNaruto.plist"));
    CHECK(contains(naruto, "Element/Kurama/Kurama.plist"));

    CHECK(contains(HeroAssets::sheets("Kankuro"), "Element/Saso/Saso.plist"));
    CHECK(contains(HeroAssets::sheets("Pain"), "Effects/FontEffect.plist"));
    CHECK(contains(HeroAssets::sheets("Lee"), "Element/RockLee/RockLee.plist"));
}

TEST_CASE("every sheet a hero brings is shipped; only some heroes lack a skill sheet")
{
    const fs::path resources = NS_RESOURCES_DIR;
    std::vector<std::string> heroes = {"Roshi", "Han"};
    for (const HeroRegistry::HeroInfo& hero : HeroRegistry::heroes())
        heroes.push_back(hero.name);

    std::vector<std::string> withoutSkillSheet;
    for (const std::string& hero : heroes)
    {
        for (const std::string& sheet : HeroAssets::sheets(hero))
        {
            if (fs::exists(resources / sheet))
                continue;
            CAPTURE(sheet);
            CHECK(sheet == "Element/Skills/" + hero + "_Skill.plist");
            withoutSkillSheet.push_back(hero);
        }
    }
    // LoadLayer used to skip Kiba's and Kakuzu's by name; the guardians never had one
    std::sort(withoutSkillSheet.begin(), withoutSkillSheet.end());
    CHECK(withoutSkillSheet == std::vector<std::string>{"Han", "Kakuzu", "Kiba", "Roshi"});
}
