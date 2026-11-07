#include "Heroes/HeroAssets.h"
#include "Heroes/HeroRegistry.h"
#include "Compat/Compat2x.h"

#include <map>

namespace HeroAssets
{

namespace
{

std::string elementSheet(const std::string& character)
{
    return "Element/" + character + "/" + character + ".plist";
}

// sheets of the units a hero's skills spawn with their own sprites
const std::vector<std::string>& extraSheets(const std::string& hero)
{
    static const std::map<std::string, std::vector<std::string>> sheetsByHero = {
        {"Chiyo", {elementSheet("Parents")}},
        {"Deidara", {elementSheet("Centipede")}},
        {"Kakashi", {elementSheet("DogWall")}},
        {"Kakuzu", {elementSheet("MaskRaidon"), elementSheet("MaskFudon"), elementSheet("MaskKadon")}},
        {"Kankuro", {elementSheet("Karasu"), elementSheet("Sanshouuo"), elementSheet("Saso")}},
        {"Kiba", {elementSheet("Akamaru")}},
        {"Naruto", {elementSheet("Kurama")}},
        {"Pain", {"Effects/FontEffect.plist"}},
        {"Tsunade", {elementSheet("Slug")}},
    };
    static const std::vector<std::string> none;
    auto found = sheetsByHero.find(hero);
    return found != sheetsByHero.end() ? found->second : none;
}

}  // namespace

std::vector<std::string> sheets(const std::string& hero)
{
    std::vector<std::string> paths = {"Element/Skills/" + hero + "_Skill.plist"};
    for (const std::string& form : HeroRegistry::forms(hero))
        paths.push_back(elementSheet(form));
    const std::vector<std::string>& extras = extraSheets(hero);
    paths.insert(paths.end(), extras.begin(), extras.end());
    return paths;
}

void load(const std::string& hero)
{
    for (const std::string& path : sheets(hero))
        if (CCFileUtils::sharedFileUtils()->isFileExist(path))
            CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(path.c_str());
}

void unload(const std::string& hero)
{
    for (const std::string& path : sheets(hero))
        CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(path.c_str());
}

}  // namespace HeroAssets
