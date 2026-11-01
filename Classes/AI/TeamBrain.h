#pragma once
#include "Types.h"

class ActionManager;
class GameLayer;

namespace ai
{

// What one side knows as a team. Recomputed at most every few frames and
// shared by every AI hero of that side.
struct TeamBrain
{
    ActionManager* focus = nullptr;        // enemy hero worth collapsing on
    ActionManager* towerThreat = nullptr;  // enemy hero hitting our tower
    bool pushWindow = false;               // two or more enemy heroes are dead
    bool outnumbered = false;              // more enemy heroes alive than ours
    bool hasFront = false;
    float frontX = 0;                      // x of our most advanced live hero

    static const TeamBrain& get(GameLayer* game, Group group);
};

// true for Player/Com heroes taking part in the lane fight (guardians excluded)
bool isLaneHero(ActionManager* unit);

}  // namespace ai
