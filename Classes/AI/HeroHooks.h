#pragma once
#include <string>

namespace ai
{

class AIController;

// Hero mechanics that the rule data cannot express (marks, teleports, corpse
// harvesting). A hook runs first each decision; returning true ends the tick.
using PreTick = bool (*)(AIController& ai);

PreTick findHook(const std::string& name);

}  // namespace ai
