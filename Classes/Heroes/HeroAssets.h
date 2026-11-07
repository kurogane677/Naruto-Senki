#pragma once
#include <string>
#include <vector>

// What a hero brings into a match besides the shared UI: loaded on the loading screen,
// unloaded when the match is left. Both sides use the same lists.
namespace HeroAssets
{

// sprite sheets: its skills, every form, and the summons, puppets and effects its skills use
std::vector<std::string> sheets(const std::string& hero);
// sound sets in Audio/list.xml: one per form
std::vector<std::string> soundSets(const std::string& hero);

// sheets that are not shipped (heroes without a skill sheet) are skipped
void load(const std::string& hero);
void unload(const std::string& hero);

}  // namespace HeroAssets
