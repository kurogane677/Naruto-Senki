#pragma once
#include "AI/AIDifficulty.h"
#include "AI/AIProfile.h"
#include <map>

class ActionManager;
class Hero;
class GameLayer;

namespace ai
{

class TeamBrain;

// The decision loop shared by every AI hero; replaces the old per-hero
// Hero::AI_<Name>() copies. One instance per AI-controlled hero.
class AIController
{
public:
    // nullptr when the hero has no profile (then the legacy AI keeps running)
    static AIController* create(Hero* hero);

    explicit AIController(Hero* hero, const Profile* profile);

    void tick(float dt);

    Hero* hero() const { return _hero; }
    const Profile* profile() const { return _profile; }
    HeroRole role() const { return _profile->role; }
    float now() const { return _time; }

    // helpers shared with HeroHooks
    bool walkTo(const CCPoint& delta, int rangeX, int rangeY, bool speedGear = false);
    void strike(ActionManager* target, int slotType);
    CCPoint deltaTo(ActionManager* target) const;
    bool canAct() const;

private:
    // decision steps; each returns true when it issued a command this tick
    bool survive();
    void buyGear();
    bool defendBase();
    bool dodge();
    bool fightHero();
    bool farm();
    void advance();

    ActionManager* pickHeroTarget();
    bool ruleReady(const SkillRule& rule);
    bool condsHold(const SkillRule& rule, ActionManager* target);
    bool slotReady(Slot slot) const;
    bool retreatFromFight(const CCPoint& sp);
    void markUsed(Slot slot);

    Hero* _hero;
    const Profile* _profile;
    GameLayer* _game;
    Difficulty _difficulty;
    const DifficultyParams* _params;

    float _time = 0;
    float _nextThink = 0;
    ActionManager* _committed = nullptr;  // hero target kept for a while
    float _commitUntil = 0;
    float _dodgeUntil = 0;
    float _strafeUntil = 0;
    bool _retreating = false;
    std::map<int, float> _readySince;     // slot -> time it came off cooldown
    std::map<int, float> _skipUntil;      // slot -> declined opportunity
    std::map<ActionManager*, bool> _dodgeRolled;
    std::vector<int> _build;
    bool _buildChosen = false;
};

}  // namespace ai
