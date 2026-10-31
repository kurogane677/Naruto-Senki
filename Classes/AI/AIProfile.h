#pragma once
#include "Defines.h"
#include <map>
#include <string>
#include <vector>

// Per-hero AI data, loaded from Resources/AI/profiles.json.
//
// A hero's combat is an ordered list of skill rules. Each tick the first rule
// whose slot is ready and whose conditions hold wins: the bot casts it when the
// target is inside [rangeX, rangeY], otherwise walks closer (if `approach`).
// Rules with `gate` only apply while the target is within that x distance;
// they form the old "engaged" block, and the team-strength retreat check runs
// right before the first gated rule.

namespace ai
{

enum class HeroRole
{
    Fighter,
    Assassin,
    Tank,
    Support,
    Pusher,
    Ranged,
    Summoner,
};

enum class Slot
{
    NAttack,
    Skill1,
    Skill2,
    Skill3,
    Ougis1,
    Ougis2,
    Gear00,  // use the active speed/stealth gear
    Gear03,  // drop the trap gear
};

enum class TargetKind
{
    Hero,
    Flog,
    Tower,
    AnyFarm,  // Flog or Tower
};

// A condition is a name plus an optional number, e.g. "hpBelow:0.9".
struct Cond
{
    std::string name;
    std::string arg;  // text after ':' ("noMonster:Karasu")
    float value = 0;  // arg as a number ("hpBelow:0.9")
};

struct SkillRule
{
    Slot slot = Slot::NAttack;
    TargetKind on = TargetKind::Hero;
    int rangeX = 32;
    int rangeY = 32;
    int gate = 0;          // 0: always considered; >0: only while |dx| < gate
    int minRange = 0;      // step back first when closer than this
    bool approach = true;  // walk into range; false: skip the rule when out of range
    bool selfCast = false; // no target needed / do not turn
    bool speedGear = false;  // pop gear00 before walking in
    std::vector<Cond> conds;
};

struct Profile
{
    std::string name;
    HeroRole role = HeroRole::Fighter;
    std::vector<std::string> subRoles;
    int nattackX = 32;
    int nattackY = 32;
    int retreatGap = 3000;  // enemy minus friendly combat points that triggers a retreat
    bool baseCheck = true;
    bool panicGear06 = true;
    bool idleWhenBuff = false;  // e.g. Ino while her mind is in another body
    std::string hook;  // HeroHooks entry for mechanics data cannot express
    std::map<std::string, std::vector<int>> builds;
    std::vector<SkillRule> hero;  // vs heroes
    std::vector<SkillRule> farm;  // vs creeps/towers
};

const Profile* findProfile(const std::string& hero);
const char* roleName(HeroRole role);

}  // namespace ai
