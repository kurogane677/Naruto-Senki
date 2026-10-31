#include "AI/AIProfile.h"

namespace ai
{

namespace
{

HeroRole parseRole(const std::string& s)
{
    if (s == "assassin") return HeroRole::Assassin;
    if (s == "tank") return HeroRole::Tank;
    if (s == "support") return HeroRole::Support;
    if (s == "pusher") return HeroRole::Pusher;
    if (s == "ranged") return HeroRole::Ranged;
    if (s == "summoner") return HeroRole::Summoner;
    return HeroRole::Fighter;
}

Slot parseSlot(const std::string& s)
{
    if (s == "SKILL1") return Slot::Skill1;
    if (s == "SKILL2") return Slot::Skill2;
    if (s == "SKILL3") return Slot::Skill3;
    if (s == "OUGIS1") return Slot::Ougis1;
    if (s == "OUGIS2") return Slot::Ougis2;
    if (s == "GEAR00") return Slot::Gear00;
    if (s == "GEAR03") return Slot::Gear03;
    return Slot::NAttack;
}

TargetKind parseKind(const std::string& s)
{
    if (s == "flog") return TargetKind::Flog;
    if (s == "tower") return TargetKind::Tower;
    if (s == "farm") return TargetKind::AnyFarm;
    return TargetKind::Hero;
}

std::string str(const rapidjson::Value& v, const char* key, const char* def = "")
{
    auto it = v.FindMember(key);
    return it != v.MemberEnd() && it->value.IsString() ? it->value.GetString() : def;
}

int num(const rapidjson::Value& v, const char* key, int def)
{
    auto it = v.FindMember(key);
    return it != v.MemberEnd() && it->value.IsNumber() ? it->value.GetInt() : def;
}

bool flag(const rapidjson::Value& v, const char* key, bool def)
{
    auto it = v.FindMember(key);
    return it != v.MemberEnd() && it->value.IsBool() ? it->value.GetBool() : def;
}

std::vector<SkillRule> parseRules(const rapidjson::Value& arr, TargetKind defaultOn)
{
    std::vector<SkillRule> rules;
    if (!arr.IsArray())
        return rules;
    for (auto& r : arr.GetArray())
    {
        SkillRule rule;
        rule.slot = parseSlot(str(r, "slot"));
        std::string on = str(r, "on");
        rule.on = on.empty() ? defaultOn : parseKind(on);
        auto range = r.FindMember("range");
        if (range != r.MemberEnd() && range->value.IsArray() && range->value.Size() == 2)
        {
            rule.rangeX = range->value[0].GetInt();
            rule.rangeY = range->value[1].GetInt();
        }
        rule.gate = num(r, "gate", 0);
        rule.minRange = num(r, "min", 0);
        rule.approach = flag(r, "approach", true);
        rule.selfCast = flag(r, "self", false);
        rule.speedGear = flag(r, "gear00", false);
        auto conds = r.FindMember("if");
        if (conds != r.MemberEnd() && conds->value.IsArray())
        {
            for (auto& c : conds->value.GetArray())
            {
                std::string s = c.GetString();
                Cond cond;
                auto colon = s.find(':');
                cond.name = s.substr(0, colon);
                if (colon != std::string::npos)
                {
                    cond.arg = s.substr(colon + 1);
                    cond.value = (float)atof(cond.arg.c_str());
                }
                rule.conds.push_back(cond);
            }
        }
        rules.push_back(rule);
    }
    return rules;
}

std::map<std::string, Profile> load()
{
    std::map<std::string, Profile> profiles;
    const char* path = "AI/profiles.json";
    if (!CCFileUtils::sharedFileUtils()->isFileExist(path))
    {
        CCLOG("AI: %s missing, every hero uses the legacy AI", path);
        return profiles;
    }
    std::string json = CCFileUtils::sharedFileUtils()->getStringFromFile(path);
    rapidjson::Document doc;
    doc.Parse<rapidjson::kParseCommentsFlag | rapidjson::kParseTrailingCommasFlag>(json.c_str());
    if (doc.HasParseError() || !doc.IsObject())
    {
        CCLOG("AI: %s is not valid JSON (offset %u)", path, (unsigned)doc.GetErrorOffset());
        return profiles;
    }
    auto heroes = doc.FindMember("heroes");
    if (heroes == doc.MemberEnd() || !heroes->value.IsObject())
        return profiles;
    for (auto h = heroes->value.MemberBegin(); h != heroes->value.MemberEnd(); ++h)
    {
        const rapidjson::Value& v = h->value;
        Profile p;
        p.name = h->name.GetString();
        p.role = parseRole(str(v, "role", "fighter"));
        auto sub = v.FindMember("subRoles");
        if (sub != v.MemberEnd() && sub->value.IsArray())
            for (auto& s : sub->value.GetArray())
                p.subRoles.push_back(s.GetString());
        auto na = v.FindMember("nattack");
        if (na != v.MemberEnd() && na->value.IsArray() && na->value.Size() == 2)
        {
            p.nattackX = na->value[0].GetInt();
            p.nattackY = na->value[1].GetInt();
        }
        p.retreatGap = num(v, "retreatGap", 3000);
        p.baseCheck = flag(v, "baseCheck", true);
        p.panicGear06 = flag(v, "panicGear06", true);
        p.idleWhenBuff = flag(v, "idleWhenBuff", false);
        p.hook = str(v, "hook");
        auto builds = v.FindMember("builds");
        if (builds != v.MemberEnd() && builds->value.IsObject())
        {
            for (auto b = builds->value.MemberBegin(); b != builds->value.MemberEnd(); ++b)
            {
                std::vector<int> gears;
                for (auto& g : b->value.GetArray())
                    gears.push_back(g.GetInt());
                p.builds[b->name.GetString()] = gears;
            }
        }
        auto hero = v.FindMember("hero");
        if (hero != v.MemberEnd())
            p.hero = parseRules(hero->value, TargetKind::Hero);
        auto farm = v.FindMember("farm");
        if (farm != v.MemberEnd())
            p.farm = parseRules(farm->value, TargetKind::Flog);
        profiles[p.name] = p;
    }
    CCLOG("AI: loaded %d hero profiles", (int)profiles.size());
    return profiles;
}

}  // namespace

const Profile* findProfile(const std::string& hero)
{
    static const std::map<std::string, Profile> profiles = load();
    auto it = profiles.find(hero);
    return it == profiles.end() ? nullptr : &it->second;
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
