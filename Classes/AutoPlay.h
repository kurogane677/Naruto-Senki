#pragma once

class GameLayer;

// Test mode: set NS_AUTOPLAY=1 and the game skips the menus, starts a
// Training match with the player hero driven by the AI, and loops forever,
// appending one line per match to <writable>/autoplay.log.
//   NS_AUTOPLAY_HERO=Naruto   player hero (default: random)
//   NS_AUTOPLAY_SPEED=4       game speed multiplier (default 3)
//   NS_AUTOPLAY_HARDCORE=1    HardCore rules (random map, gear shop, guardians)
//   NS_AUTOPLAY_SEED=42       soak mode for refactor checks: seeded randomness, fixed 1/60 s
//                             steps and a game clock, as fast as the machine renders. The
//                             seed fixes the heroes and maps, but matches still differ run to
//                             run: the engine runs node timers in memory-address order.
//                             NS_AUTOPLAY_SPEED is ignored.
//   NS_AUTOPLAY_MATCHES=3     quit after this many matches
//   NS_DATA_DIR=path          writable folder (sql.db, settings) instead of the user's own
namespace AutoPlay
{
bool enabled();
bool fixedStep();
float speed();
// call first in applicationDidFinishLaunching / last, after runWithScene
void beforeLaunch();
void afterLaunch();
void startMatch();
void report(GameLayer* game, bool konohaWon);
}
