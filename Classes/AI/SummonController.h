#pragma once
#include "AI/AIProfile.h"
#include "Compat/Compat2x.h"

class ActionManager;
class Hero;
class GameLayer;

namespace ai
{

struct SummonSpec;

// Drives summons and puppets (Akamaru, Kurama, Karasu, the Paths, ...).
// Who they go after follows the summoner's AI profile role; how they
// fight comes from a per-summon spec. Summons cannot heal, so unlike
// AIController they never look at their own HP: no retreat, potions or gear.
class SummonController
{
public:
    // nullptr for units that are not driven summons (heroes, Naruto clones, DogWall)
    static SummonController* create(Hero* summon);

    void tick(float dt);

private:
    SummonController(Hero* summon, const SummonSpec* spec);

    Hero* liveMaster() const;
    HeroRole summonerRole(Hero* master) const;
    int leash(HeroRole role, Hero* master) const;
    ActionManager* pickTarget(HeroRole role, Hero* master, int leash);
    ActionManager* weakestHero(ActionManager* center, float range) const;
    bool validTarget(ActionManager* u) const;
    bool canWalk(Hero* master) const;

    Hero* _summon;
    const SummonSpec* _spec;
    GameLayer* _game;
};

}  // namespace ai
