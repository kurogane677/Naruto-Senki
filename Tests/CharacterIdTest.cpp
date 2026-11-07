#include "doctest.h"
#include "Heroes/CharacterId.h"
#include "Heroes/HeroRegistry.h"

#include <filesystem>
#include <fstream>
#include <regex>
#include <sstream>

namespace fs = std::filesystem;

static std::string readFile(const fs::path& path)
{
    std::ifstream in(path, std::ios::binary);
    std::stringstream text;
    text << in.rdbuf();
    return text.str();
}

// every name in `text` captured by `pattern`'s first group that has no CharacterId
static std::vector<std::string> unknownNames(const std::string& text, const std::regex& pattern)
{
    std::vector<std::string> unknown;
    for (std::sregex_iterator it(text.begin(), text.end(), pattern), end; it != end; ++it)
        if (characterIdFromString((*it)[1].str().c_str()) == CharacterId::None)
            unknown.push_back((*it)[1].str());
    return unknown;
}

TEST_CASE("characterIdFromString")
{
    CHECK(characterIdFromString("Naruto") == CharacterId::Naruto);
    CHECK(characterIdFromString("KonohaCenter") == CharacterId::KonohaCenter);
    CHECK(characterIdFromString("naruto") == CharacterId::None);
    CHECK(characterIdFromString("") == CharacterId::None);
    CHECK(characterIdFromString(nullptr) == CharacterId::None);
}

TEST_CASE("every hero and form has a CharacterId")
{
    for (const HeroRegistry::HeroInfo& hero : HeroRegistry::heroes())
    {
        CAPTURE(hero.name);
        CHECK(characterIdFromString(hero.name) != CharacterId::None);
        for (const char* form = HeroRegistry::nextForm(hero.name); form && std::string(form) != hero.name;
             form = HeroRegistry::nextForm(form))
        {
            CAPTURE(form);
            CHECK(characterIdFromString(form) != CharacterId::None);
        }
    }
}

TEST_CASE("every unit the data spawns has a CharacterId")
{
    const fs::path resources = NS_RESOURCES_DIR;
    const std::regex spawn(R"re(type="(?:setMon|setBullet|setTrap)">(\w+)<)re");
    for (const auto& entry : fs::recursive_directory_iterator(resources / "Element"))
    {
        if (entry.path().extension() != ".xml")
            continue;
        CAPTURE(entry.path().string());
        CHECK(unknownNames(readFile(entry.path()), spawn).empty());
    }

    const std::regex tower(R"re(<object[^>]*name="((?:Konoha|Akatsuki)\w+)")re");
    for (const auto& entry : fs::directory_iterator(resources / "Tiles"))
    {
        if (entry.path().extension() != ".tmx")
            continue;
        CAPTURE(entry.path().string());
        CHECK(unknownNames(readFile(entry.path()), tower).empty());
    }
}
