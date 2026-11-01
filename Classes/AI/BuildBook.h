#pragma once
#include <string>
#include <vector>

namespace ai
{

class AIController;

// Gear builds for AI heroes (HardCore only, where the shop exists).
//
// Each hero has the builds from its profile plus defaults for its role. At
// the first purchase the bot picks one by match state: losing -> "sustain",
// winning -> "aggressive"/"push", otherwise its profile's "default". On top of
// that the bot may copy the human player's best-performing build for the same
// hero, recorded in the PlayerBuild table of sql.db.
std::vector<int> chooseBuild(AIController& ai);

// Records the player's gear purchase order for `hero` after a HardCore match.
void recordPlayerBuild(const std::string& hero, const std::vector<int>& gears, bool won);

// Best recorded build (>= 3 games, highest win rate) or empty.
std::vector<int> bestPlayerBuild(const std::string& hero);

}  // namespace ai
