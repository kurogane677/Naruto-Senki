#pragma once

class GameLayer;

// Test mode: set NS_AUTOPLAY=1 and the game skips the menus, starts a
// Training match with the player hero driven by the AI, and loops forever,
// appending one line per match to <writable>/autoplay.log.
//   NS_AUTOPLAY_HERO=Naruto   player hero (default: random)
//   NS_AUTOPLAY_SPEED=4       game speed multiplier (default 3)
//   NS_AUTOPLAY_DIFFICULTY=2  bot level 0-2 (default: the saved choice)
//   NS_AUTOPLAY_HARDCORE=1    HardCore rules (random map, gear shop, guardians)
namespace AutoPlay
{
bool enabled();
float speed();
void startMatch();
void report(GameLayer* game, bool konohaWon);
}
