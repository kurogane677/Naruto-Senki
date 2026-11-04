#include "AI/AIProfile.h"

namespace ai
{

const Profile* findProfile(const std::string& hero)
{
    static const std::map<std::string, Profile> profiles = builtinProfiles();
    auto it = profiles.find(hero);
    return it == profiles.end() ? nullptr : &it->second;
}

const Profile* defaultProfile(const std::string& hero)
{
    static const Profile fallback = builtinDefaultProfile();
    return usesLegacyAI(hero) ? nullptr : &fallback;
}

const char* roleName(HeroRole role)
{
    switch (role)
    {
    case HeroRole::Assassin: return "assassin";
    case HeroRole::Tank: return "tank";
    case HeroRole::Support: return "support";
    case HeroRole::Pusher: return "pusher";
    case HeroRole::Ranged: return "ranged";
    case HeroRole::Summoner: return "summoner";
    default: return "fighter";
    }
}

}  // namespace ai
