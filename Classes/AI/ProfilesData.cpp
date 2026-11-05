// Bot (AI) profile per hero. This file is the source: bot behaviour is compiled in,
// so it can't be changed by editing the APK (#61). See AIProfile.h for the types.
//
// A profile only sets what differs from the defaults in Profile/SkillRule:
//   role         fighter (default) | assassin | tank | support | pusher | ranged | summoner;
//                picks targets, retreat timing, lane position and fallback builds
//   subRoles     extra roles that shape behaviour (e.g. "pusher")
//   nattackX/Y   normal attack range (default 32, 32)
//   retreatGap   enemy minus friendly combat points that triggers a retreat (default 3000)
//   baseCheck    defend the base when it is in danger (default true)
//   panicGear06  use the substitution gear when in trouble (default true)
//   idleWhenBuff stand still while a buff is active (e.g. Ino in another body)
//   hook         HeroHooks entry for mechanics data can't express
//   builds       gear ids in purchase order (HardCore); "default" came from the original
//                AI, role builds "aggressive"/"sustain"/"push" fill the rest
//   hero         skill rules vs heroes, first match wins
//   farm         skill rules vs creeps (default) or towers: .on(TargetKind::Tower/AnyFarm)
//
// Rules: rule(slot) vs heroes, farm(slot) vs creeps, then any of
//   .range(x, y)  cast range (default 32, 32); 999 means "anywhere in sight"
//   .gate(x)      only while the target is closer than x (the old "engaged" block; the
//                 team retreat check runs right before the first gated rule)
//   .minRange(x)  step back first when closer than x
//   .noApproach() skip the rule when out of range instead of walking in
//   .self()       no target needed / do not turn
//   .speedGear()  pop gear00 before walking in
//   .when(...)    conditions, all must hold: gp (target is not too strong),
//                 tNotBati/tNotDown/tNotSticking/tIdle (target state), selfBati/selfNotBati,
//                 buff/notBuff, skillUP, notPowerUP, hpBelow:x, hpAbove:x, tHpBelow:x,
//                 tHpAbsBelow:n, tNoGear06, lv:n, bamenBelow:n, bamenAtLeast:n,
//                 noMonster:Name, hasMonster:Name, noBuffTime, notHealing, invisible,
//                 notTaunt, baseDanger, losing, allyNear, summonable, nattackBelow:n,
//                 combo (target is down)
#include "AI/AIProfile.h"

#include <cstdlib>

namespace ai
{

namespace
{

class RuleBuilder
{
public:
    RuleBuilder(Slot slot, TargetKind on)
    {
        _rule.slot = slot;
        _rule.on = on;
    }
    RuleBuilder& on(TargetKind kind)
    {
        _rule.on = kind;
        return *this;
    }
    RuleBuilder& range(int rangeX, int rangeY)
    {
        _rule.rangeX = rangeX;
        _rule.rangeY = rangeY;
        return *this;
    }
    RuleBuilder& gate(int distance)
    {
        _rule.gate = distance;
        return *this;
    }
    RuleBuilder& minRange(int distance)
    {
        _rule.minRange = distance;
        return *this;
    }
    RuleBuilder& noApproach()
    {
        _rule.approach = false;
        return *this;
    }
    RuleBuilder& self()
    {
        _rule.selfCast = true;
        return *this;
    }
    RuleBuilder& speedGear()
    {
        _rule.speedGear = true;
        return *this;
    }
    // "name" or "name:arg", e.g. "gp", "hpBelow:0.5", "noMonster:Karasu"
    template <typename... Names>
    RuleBuilder& when(Names... names)
    {
        for (const char* text : {names...})
        {
            std::string condition = text;
            Cond cond;
            auto separator = condition.find(':');
            cond.name = condition.substr(0, separator);
            if (separator != std::string::npos)
            {
                cond.arg = condition.substr(separator + 1);
                cond.value = (float)atof(cond.arg.c_str());
            }
            _rule.conds.push_back(cond);
        }
        return *this;
    }
    operator SkillRule() const { return _rule; }

private:
    SkillRule _rule;
};

RuleBuilder rule(Slot slot)
{
    return RuleBuilder(slot, TargetKind::Hero);
}

RuleBuilder farm(Slot slot)
{
    return RuleBuilder(slot, TargetKind::Flog);
}

}  // namespace

std::map<std::string, Profile> builtinProfiles()
{
    std::map<std::string, Profile> profiles;
    auto addProfile = [&](const char* name) -> Profile& {
        Profile& profile = profiles[name];
        profile.name = name;
        return profile;
    };

    {
        Profile& profile = addProfile("Naruto");
        profile.nattackX = 40;
        profile.nattackY = 32;
        profile.panicGear06 = false;
        profile.builds["default"] = {3, 7, 2};
        profile.hero = {
            rule(Slot::Ougis2).range(999, 999).noApproach().when("selfNotBati"),
            rule(Slot::Skill2).range(999, 999).noApproach().when("selfNotBati"),
            rule(Slot::Ougis1).range(96, 32).gate(128).minRange(32).when("gp"),
            rule(Slot::Gear03).range(96, 32).gate(128),
            rule(Slot::Skill3).range(96, 32).gate(128).minRange(32).when("gp"),
            rule(Slot::Skill1).range(96, 32).gate(128).minRange(32).when("gp"),
        };
        profile.farm = {
            farm(Slot::Skill2).on(TargetKind::AnyFarm),
        };
    }

    {
        Profile& profile = addProfile("SageNaruto");
        profile.nattackX = 40;
        profile.nattackY = 32;
        profile.panicGear06 = false;
        profile.builds["default"] = {3, 7, 2};
        profile.hero = {
            rule(Slot::Ougis2).range(999, 999).noApproach(),
            rule(Slot::Skill2).range(999, 999).noApproach(),
            rule(Slot::Skill3).range(999, 999).noApproach().when("gp"),
            rule(Slot::Ougis1).range(96, 32).gate(128),
            rule(Slot::Gear03).range(96, 32).gate(128),
            rule(Slot::Skill1).range(96, 32).gate(128).minRange(32),
        };
        profile.farm = {
            farm(Slot::Skill2).on(TargetKind::AnyFarm),
        };
    }

    {
        Profile& profile = addProfile("RikudoNaruto");
        profile.panicGear06 = false;
        profile.builds["default"] = {3, 7, 2};
        profile.hero = {
            rule(Slot::Ougis2).range(999, 999).noApproach(),
            rule(Slot::Skill2).range(999, 999).noApproach(),
            rule(Slot::Skill3).range(999, 999).noApproach(),
            rule(Slot::Skill1).range(999, 999).noApproach(),
            rule(Slot::Gear03).range(999, 999).noApproach(),
            rule(Slot::Ougis1).gate(128),
        };
        profile.farm = {
            farm(Slot::Skill1),
            farm(Slot::Skill2).on(TargetKind::AnyFarm),
        };
    }

    {
        Profile& profile = addProfile("Asuma");
        profile.builds["default"] = {6, 5, 2};
        profile.hero = {
            rule(Slot::Ougis1).range(999, 999).noApproach().when("gp"),
            rule(Slot::Skill3).range(96, 32).when("gp"),
            rule(Slot::Skill2).range(96, 32).when("gp"),
            rule(Slot::Ougis2).range(96, 32).gate(128).when("gp", "tNotBati"),
            rule(Slot::Skill1).range(96, 32).gate(128).when("gp"),
        };
        profile.farm = {
            farm(Slot::Skill2).when("baseDanger"),
            farm(Slot::Skill1),
        };
    }

    {
        Profile& profile = addProfile("Kakashi");
        profile.role = HeroRole::Assassin;
        profile.builds["default"] = {6, 5, 2};
        profile.hero = {
            rule(Slot::Skill3).range(999, 999).noApproach(),
            rule(Slot::Skill2).range(999, 999).noApproach().when("gp"),
            rule(Slot::Ougis2).range(64, 32).gate(128).when("gp", "tNotBati", "tNotDown", "tNotSticking"),
            rule(Slot::Ougis1).range(64, 32).gate(128),
            rule(Slot::Skill1).range(64, 32).gate(128),
        };
        profile.farm = {
            farm(Slot::Skill3),
            farm(Slot::Skill1),
        };
    }

    {
        Profile& profile = addProfile("Konan");
        profile.role = HeroRole::Ranged;
        profile.nattackX = 48;
        profile.nattackY = 32;
        profile.panicGear06 = false;
        profile.builds["default"] = {3, 5, 2};
        profile.hero = {
            rule(Slot::Skill1).range(999, 999).noApproach(),
            rule(Slot::Ougis2).range(48, 16).when("gp", "tNotBati", "tNotDown", "tNotSticking", "selfNotBati"),
            rule(Slot::Skill3).range(128, 16).minRange(96).when("invisible", "gp", "tNotDown"),
            rule(Slot::Skill2).range(128, 16).minRange(96).when("invisible", "gp", "tNotDown"),
            rule(Slot::NAttack).range(8, 8).gate(128).when("selfBati"),
            rule(Slot::Ougis1).range(48, 32).gate(128).minRange(32),
            rule(Slot::Skill2).range(48, 32).gate(128).minRange(32),
            rule(Slot::Gear03).range(48, 32).gate(128),
        };
        profile.farm = {
            farm(Slot::Gear03).on(TargetKind::AnyFarm),
            farm(Slot::Ougis1).when("baseDanger"),
            farm(Slot::Skill2).when("baseDanger"),
        };
    }

    {
        Profile& profile = addProfile("Kurenai");
        profile.nattackX = 40;
        profile.nattackY = 32;
        profile.panicGear06 = false;
        profile.builds["default"] = {3, 7, 2};
        profile.hero = {
            rule(Slot::Skill1).range(999, 999).noApproach(),
            rule(Slot::Skill2).range(128, 32).minRange(64).when("invisible", "gp", "tNotDown"),
            rule(Slot::Ougis2).range(96, 32).gate(128).when("gp", "tNotBati", "tNotDown", "tNotSticking", "selfNotBati"),
            rule(Slot::Ougis1).range(64, 32).gate(128).minRange(32).when("gp"),
            rule(Slot::Gear03).range(96, 32).gate(128),
            rule(Slot::Skill3).gate(128).when("gp"),
            rule(Slot::Skill2).range(64, 32).gate(128).minRange(32).when("gp"),
        };
        profile.farm = {
            farm(Slot::Gear03).on(TargetKind::AnyFarm),
            farm(Slot::Skill3).when("baseDanger"),
        };
    }

    {
        Profile& profile = addProfile("Deidara");
        profile.role = HeroRole::Pusher;
        profile.subRoles = {"ranged"};
        profile.baseCheck = false;
        profile.panicGear06 = false;
        profile.builds["default"] = {0, 1, 5};
        profile.hero = {
            rule(Slot::Ougis2).range(48, 16).speedGear().when("selfNotBati"),
            rule(Slot::Ougis1).range(999, 999).noApproach(),
            rule(Slot::Skill3).range(64, 16).speedGear(),
            rule(Slot::Skill2).range(999, 999).noApproach(),
            rule(Slot::Skill1).range(999, 999).gate(999).noApproach(),
            rule(Slot::NAttack).range(8, 8).gate(128).when("selfBati"),
        };
        profile.farm = {
            farm(Slot::Ougis2).on(TargetKind::Tower).when("baseDanger"),
            farm(Slot::Skill1).on(TargetKind::Tower),
            farm(Slot::Skill2).on(TargetKind::AnyFarm),
        };
    }

    {
        Profile& profile = addProfile("Ino");
        profile.role = HeroRole::Support;
        profile.idleWhenBuff = true;
        profile.builds["default"] = {6, 5, 1};
        profile.hero = {
            rule(Slot::Ougis2).range(96, 16),
            rule(Slot::Skill2).range(96, 16).when("gp"),
            rule(Slot::Skill3).range(96, 16).when("gp"),
            rule(Slot::Ougis1).gate(128).when("gp"),
            rule(Slot::Skill1).gate(128),
        };
        profile.farm = {
            farm(Slot::Skill1),
        };
    }

    {
        Profile& profile = addProfile("Sai");
        profile.role = HeroRole::Ranged;
        profile.builds["default"] = {6, 5, 1};
        profile.hero = {
            rule(Slot::Ougis2).range(48, 16).when("gp", "tNotBati", "tNotDown", "tNotSticking", "selfNotBati"),
            rule(Slot::Skill3).range(64, 16),
            rule(Slot::Skill2).range(999, 999).noApproach(),
            rule(Slot::Ougis1).range(999, 999).noApproach(),
            rule(Slot::Skill1).range(999, 999).gate(999).noApproach(),
            rule(Slot::NAttack).range(8, 8).gate(128).when("selfBati"),
        };
        profile.farm = {
            farm(Slot::Skill1),
            farm(Slot::Skill2).when("baseDanger", "gp"),
        };
    }

    {
        Profile& profile = addProfile("Sakura");
        profile.role = HeroRole::Support;
        profile.subRoles = {"healer", "pusher"};
        profile.baseCheck = false;
        profile.panicGear06 = false;
        profile.builds["default"] = {0, 4, 8};
        profile.builds["push"] = {4, 8, 0};
        profile.builds["sustain"] = {7, 8, 1};
        profile.hero = {
            rule(Slot::Ougis2).range(48, 16).speedGear().when("gp", "tNotBati", "tNotDown", "tNotSticking", "notBuff"),
            rule(Slot::Ougis1).range(48, 16).speedGear().when("notBuff"),
            rule(Slot::Skill1).range(999, 999).gate(999).noApproach().self().when("hpBelow:0.9"),
            rule(Slot::Skill2).range(64, 32).gate(128).when("notBuff", "gp"),
            rule(Slot::Skill3).gate(128).when("notBuff", "hpAbove:0.5"),
        };
        profile.farm = {
            farm(Slot::Skill1).on(TargetKind::AnyFarm).self().when("hpBelow:0.9"),
            farm(Slot::Skill3).on(TargetKind::Tower).when("notBuff"),
            farm(Slot::Skill2),
        };
    }

    {
        Profile& profile = addProfile("Tenten");
        profile.role = HeroRole::Ranged;
        profile.panicGear06 = false;
        profile.builds["default"] = {0, 5, 2};
        profile.hero = {
            rule(Slot::Skill1).range(999, 999).noApproach(),
            rule(Slot::Ougis2).range(48, 16).speedGear().when("notBuff", "gp"),
            rule(Slot::Ougis1).range(48, 16).speedGear().when("tNotBati", "notBuff"),
            rule(Slot::Skill2).range(96, 16).gate(128).when("gp"),
            rule(Slot::Skill3).range(96, 16).gate(128).when("gp"),
        };
        profile.farm = {
            farm(Slot::Skill1),
            farm(Slot::Skill3),
            farm(Slot::Skill2).when("baseDanger"),
        };
    }

    {
        Profile& profile = addProfile("Itachi");
        profile.role = HeroRole::Assassin;
        profile.nattackX = 56;
        profile.nattackY = 32;
        profile.builds["default"] = {6, 2, 5};
        profile.hero = {
            rule(Slot::Ougis2).range(48, 32).when("selfNotBati"),
            rule(Slot::Ougis1).range(999, 999).noApproach().when("gp", "tNotBati", "tNotDown", "tNotSticking", "selfNotBati"),
            rule(Slot::Skill3).range(56, 32).gate(128).when("selfNotBati", "gp"),
            rule(Slot::Skill2).range(56, 32).gate(128).when("selfNotBati", "gp"),
            rule(Slot::Skill1).range(56, 32).gate(128).when("selfNotBati", "gp"),
        };
        profile.farm = {
            farm(Slot::Skill3).when("baseDanger"),
            farm(Slot::Skill2).when("baseDanger"),
            farm(Slot::Skill1),
        };
    }

    {
        Profile& profile = addProfile("Jiraiya");
        profile.role = HeroRole::Ranged;
        profile.builds["default"] = {6, 1, 2};
        profile.hero = {
            rule(Slot::Ougis2).range(999, 999).noApproach(),
            rule(Slot::Skill2).range(999, 999).noApproach().when("gp"),
            rule(Slot::Ougis1).gate(128),
            rule(Slot::Skill3).gate(128),
            rule(Slot::Skill1).gate(128),
        };
        profile.farm = {
            farm(Slot::Skill3).when("baseDanger"),
            farm(Slot::Skill1),
        };
    }

    {
        Profile& profile = addProfile("SageJiraiya");
        profile.role = HeroRole::Ranged;
        profile.builds["default"] = {6, 1, 2};
        profile.hero = {
            rule(Slot::Skill2).range(999, 999).noApproach().when("gp"),
            rule(Slot::Ougis2).range(96, 32).gate(128).when("gp"),
            rule(Slot::Ougis1).gate(128),
            rule(Slot::Skill3).gate(128),
            rule(Slot::Skill1).gate(128),
        };
        profile.farm = {
            farm(Slot::Skill3).when("baseDanger"),
            farm(Slot::Skill1),
        };
    }

    {
        Profile& profile = addProfile("Suigetsu");
        profile.panicGear06 = false;
        profile.builds["default"] = {0, 5, 1};
        profile.hero = {
            rule(Slot::Ougis2).range(48, 16).speedGear().when("gp", "tNotBati", "tNotDown", "tNotSticking", "selfNotBati"),
            rule(Slot::Ougis1).range(48, 16).speedGear().when("selfNotBati"),
            rule(Slot::Skill3).range(999, 999).noApproach().when("gp", "selfNotBati", "notBuff"),
            rule(Slot::Skill2).range(96, 32).gate(128),
            rule(Slot::Skill1).range(96, 32).gate(128),
        };
        profile.farm = {
            farm(Slot::Skill1),
            farm(Slot::Skill2).when("baseDanger"),
        };
    }

    {
        Profile& profile = addProfile("Tsunade");
        profile.role = HeroRole::Tank;
        profile.subRoles = {"healer"};
        profile.panicGear06 = false;
        profile.builds["default"] = {3, 2, 4};
        profile.hero = {
            rule(Slot::Ougis2).range(64, 16).when("gp", "selfNotBati"),
            rule(Slot::Skill3).range(999, 999).noApproach().when("gp", "selfNotBati"),
            rule(Slot::Gear03).range(64, 32).gate(128),
            rule(Slot::Ougis1).range(64, 32).gate(128),
            rule(Slot::Skill2).range(64, 32).gate(128),
            rule(Slot::Skill1).range(64, 32).gate(128),
        };
        profile.farm = {
            farm(Slot::Gear03).on(TargetKind::AnyFarm),
            farm(Slot::Skill1).when("baseDanger"),
        };
    }

    {
        Profile& profile = addProfile("Sasuke");
        profile.role = HeroRole::Assassin;
        profile.builds["default"] = {6, 1, 2};
        profile.hero = {
            rule(Slot::Ougis2).range(999, 999).noApproach().when("selfNotBati"),
            rule(Slot::Skill3).range(999, 999).gate(999).noApproach().when("notTaunt", "selfNotBati"),
            rule(Slot::Ougis1).range(96, 32).gate(128),
            rule(Slot::Skill2).range(96, 32).gate(128),
            rule(Slot::Skill1).range(96, 32).gate(128),
        };
        profile.farm = {
            farm(Slot::Skill1),
            farm(Slot::Skill2).when("baseDanger"),
            farm(Slot::Skill3),
        };
    }

    {
        Profile& profile = addProfile("ImmortalSasuke");
        profile.role = HeroRole::Assassin;
        profile.builds["default"] = {6, 1, 2};
        profile.hero = {
            rule(Slot::Ougis1).range(999, 999).noApproach(),
            rule(Slot::Ougis2).range(48, 32),
            rule(Slot::Skill3).range(999, 999).gate(999).noApproach().when("notTaunt"),
            rule(Slot::Skill2).range(96, 32).gate(128),
            rule(Slot::Skill1).range(96, 32).gate(128),
        };
        profile.farm = {
            farm(Slot::Skill1),
            farm(Slot::Skill2).when("baseDanger"),
        };
    }

    {
        Profile& profile = addProfile("Kankuro");
        profile.role = HeroRole::Summoner;
        profile.retreatGap = 5000;
        profile.builds["default"] = {6, 10, 8};
        profile.hero = {
            rule(Slot::Skill3).range(999, 999).noApproach(),
            rule(Slot::Skill1).range(999, 999).noApproach().self().when("noMonster:Karasu"),
            rule(Slot::Ougis1).range(999, 999).noApproach().self().when("noMonster:Sanshouuo"),
            rule(Slot::Ougis2).range(999, 999).noApproach().self().when("noMonster:Saso"),
        };
        profile.farm = {
            farm(Slot::Ougis1).on(TargetKind::AnyFarm).self().when("noMonster:Sanshouuo"),
            farm(Slot::Ougis2).on(TargetKind::AnyFarm).self().when("noMonster:Saso"),
            farm(Slot::Skill1).on(TargetKind::AnyFarm).self().when("noMonster:Karasu"),
        };
    }

    {
        Profile& profile = addProfile("Chiyo");
        profile.role = HeroRole::Support;
        profile.subRoles = {"summoner"};
        profile.retreatGap = 5000;
        profile.builds["default"] = {6, 7, 10};
        profile.hero = {
            rule(Slot::Ougis1).range(999, 999).noApproach().self().when("noBuffTime", "allyNear"),
            rule(Slot::Ougis2).range(96, 32).minRange(48).when("gp", "tNotBati", "tNotDown", "tNotSticking"),
            rule(Slot::Skill1).range(999, 999).noApproach().self().when("noMonster:Parents"),
        };
        profile.farm = {
            farm(Slot::Skill1).on(TargetKind::AnyFarm).self().when("noMonster:Parents"),
        };
    }

    {
        Profile& profile = addProfile("Kakuzu");
        profile.role = HeroRole::Tank;
        profile.retreatGap = 5000;
        profile.panicGear06 = false;
        profile.hook = "kakuzu";
        profile.builds["default"] = {3, 8, 10};
        profile.hero = {
            rule(Slot::Skill3).range(999, 999).noApproach(),
            rule(Slot::Ougis1).range(999, 999).noApproach().self().when("summonable"),
            rule(Slot::Skill2).range(96, 32),
            rule(Slot::Gear03).range(96, 32),
        };
        profile.farm = {
            farm(Slot::Ougis1).on(TargetKind::AnyFarm).self().when("summonable"),
        };
    }

    {
        Profile& profile = addProfile("Shikamaru");
        profile.role = HeroRole::Support;
        profile.panicGear06 = false;
        profile.builds["default"] = {0, 7, 8};
        profile.hero = {
            rule(Slot::Ougis2).range(999, 999).noApproach().when("tNotBati", "tNotSticking"),
            rule(Slot::Skill2).range(999, 999).noApproach().when("gp"),
            rule(Slot::Ougis1).range(48, 32).speedGear(),
            rule(Slot::Skill1).range(999, 999).gate(999).noApproach().when("lv:2"),
            rule(Slot::Skill3).range(156, 32).gate(999).minRange(32).speedGear().when("tNotBati", "tNotSticking"),
        };
        profile.farm = {
            farm(Slot::Skill2).when("baseDanger"),
        };
    }

    {
        Profile& profile = addProfile("Gaara");
        profile.role = HeroRole::Tank;
        profile.subRoles = {"ranged"};
        profile.builds["default"] = {6, 2, 5};
        profile.hero = {
            rule(Slot::Ougis2).range(999, 999).noApproach().when("selfNotBati", "gp"),
            rule(Slot::Ougis1).range(999, 999).noApproach().when("selfNotBati", "gp"),
            rule(Slot::Skill3).range(999, 999).noApproach().when("selfNotBati", "gp"),
            rule(Slot::Skill2).range(999, 999).noApproach().when("selfNotBati", "gp"),
            rule(Slot::Skill1).gate(128),
        };
        profile.farm = {
            farm(Slot::Skill1),
        };
    }

    {
        Profile& profile = addProfile("Tobirama");
        profile.nattackX = 48;
        profile.nattackY = 32;
        profile.builds["default"] = {6, 1, 7};
        profile.hero = {
            rule(Slot::Ougis2).range(999, 999).noApproach().when("gp"),
            rule(Slot::Skill1).range(999, 999).noApproach().when("tNotBati", "tIdle"),
            rule(Slot::Skill2).range(64, 16),
            rule(Slot::Ougis1).range(48, 32).gate(128),
            rule(Slot::Skill3).range(48, 32).gate(128),
        };
        profile.farm = {
            farm(Slot::Skill3).when("baseDanger"),
            farm(Slot::Skill2).when("baseDanger"),
        };
    }

    {
        Profile& profile = addProfile("Choji");
        profile.role = HeroRole::Tank;
        profile.subRoles = {"pusher"};
        profile.builds["default"] = {6, 1, 4};
        profile.builds["sustain"] = {8, 11, 7};
        profile.hero = {
            rule(Slot::Skill1).range(999, 999).noApproach().when("selfNotBati"),
            rule(Slot::Skill3).range(96, 16),
            rule(Slot::Ougis1).gate(128),
            rule(Slot::Skill2).gate(128).when("skillUP"),
            rule(Slot::Ougis2).gate(128),
        };
        profile.farm = {
            farm(Slot::Skill2),
            farm(Slot::Ougis1).when("baseDanger"),
            farm(Slot::Ougis2).on(TargetKind::Tower),
        };
    }

    {
        Profile& profile = addProfile("Karin");
        profile.role = HeroRole::Support;
        profile.subRoles = {"healer"};
        profile.builds["default"] = {6, 1, 7};
        profile.hero = {
            rule(Slot::Ougis2).range(96, 16).when("gp", "tNotBati", "tNotDown", "tNotSticking"),
            rule(Slot::Ougis1).range(999, 999).noApproach().self().when("hpBelow:0.9", "noBuffTime", "notHealing"),
            rule(Slot::Skill3).range(128, 16).when("gp"),
            rule(Slot::Skill2).range(96, 32).gate(128),
            rule(Slot::Skill1).range(96, 32).gate(128),
        };
        profile.farm = {
            farm(Slot::Skill1),
            farm(Slot::Skill2).when("baseDanger"),
        };
    }

    {
        Profile& profile = addProfile("Lee");
        profile.role = HeroRole::Assassin;
        profile.nattackX = 48;
        profile.nattackY = 32;
        profile.builds["default"] = {6, 7, 2};
        profile.hero = {
            rule(Slot::Skill1).range(999, 999).gate(128).noApproach().when("bamenBelow:5"),
            rule(Slot::Ougis1).range(48, 32).gate(128).when("gp", "tNotBati", "tNotDown", "tNotSticking", "bamenAtLeast:1"),
            rule(Slot::Skill2).range(48, 32).gate(128),
        };
        profile.farm = {
            farm(Slot::Skill2),
        };
    }

    {
        Profile& profile = addProfile("RockLee");
        profile.role = HeroRole::Assassin;
        profile.nattackX = 46;
        profile.nattackY = 32;
        profile.builds["default"] = {6, 7, 2};
        profile.hero = {
            rule(Slot::Skill1).range(999, 999).noApproach().when("bamenBelow:5"),
            rule(Slot::Ougis2).range(999, 999).noApproach().when("gp", "tNotBati", "tNotDown", "tNotSticking", "bamenAtLeast:3"),
            rule(Slot::Ougis1).range(46, 32).gate(128).when("gp", "tNotBati", "tNotDown", "tNotSticking", "bamenAtLeast:1"),
            rule(Slot::Skill2).range(46, 32).gate(128),
            rule(Slot::Skill3).range(46, 32).gate(128).when("bamenAtLeast:3"),
        };
        profile.farm = {
            farm(Slot::Skill2),
            farm(Slot::Skill3).when("baseDanger", "bamenAtLeast:3"),
        };
    }

    {
        Profile& profile = addProfile("Jugo");
        profile.role = HeroRole::Tank;
        profile.nattackX = 48;
        profile.nattackY = 32;
        profile.builds["default"] = {6, 1, 2};
        profile.hero = {
            rule(Slot::Skill3).range(999, 999).noApproach().when("gp", "notBuff"),
            rule(Slot::Ougis2).gate(128).when("gp", "tNotBati", "tNotDown", "tNotSticking", "notBuff"),
            rule(Slot::Ougis1).gate(128).when("notBuff"),
            rule(Slot::Skill2).range(96, 32).gate(128).when("notBuff"),
            rule(Slot::Skill1).range(96, 32).gate(128).when("notBuff"),
        };
        profile.farm = {
            farm(Slot::Skill1).when("notBuff"),
        };
    }

    {
        Profile& profile = addProfile("Kisame");
        profile.role = HeroRole::Tank;
        profile.nattackX = 64;
        profile.nattackY = 32;
        profile.panicGear06 = false;
        profile.builds["default"] = {0, 1, 5};
        profile.builds["sustain"] = {8, 11, 7};
        profile.hero = {
            rule(Slot::Ougis2).range(48, 32).speedGear().when("gp", "tNotBati", "tNotDown", "tNotSticking", "notBuff"),
            rule(Slot::Ougis1).range(48, 32).speedGear().when("notBuff"),
            rule(Slot::Skill3).range(96, 32).gate(999).when("notBuff"),
            rule(Slot::Skill2).range(96, 32).gate(999).when("gp", "notBuff"),
            rule(Slot::Skill1).range(64, 32).gate(128),
        };
        profile.farm = {
            farm(Slot::Skill2).when("baseDanger"),
            farm(Slot::Skill1),
        };
    }

    {
        Profile& profile = addProfile("Hidan");
        profile.nattackX = 48;
        profile.nattackY = 32;
        profile.hook = "hidan";
        profile.builds["default"] = {6, 1, 7};
        profile.hero = {
            rule(Slot::Skill1).range(48, 32).when("selfNotBati", "noMonster:CircleMark", "ougis2Ready"),
            rule(Slot::Ougis2).range(48, 32).when("selfNotBati"),
            rule(Slot::Skill2).range(56, 32).when("gp", "selfNotBati"),
            rule(Slot::Ougis1).range(48, 32).gate(128).when("gp", "selfNotBati"),
            rule(Slot::Skill3).range(48, 32).gate(128).when("selfNotBati"),
        };
        profile.farm = {
            farm(Slot::Skill3).when("baseDanger"),
            farm(Slot::Skill2).when("baseDanger"),
        };
    }

    {
        Profile& profile = addProfile("Tobi");
        profile.nattackX = 64;
        profile.nattackY = 32;
        profile.panicGear06 = false;
        profile.builds["default"] = {3, 2, 5};
        profile.hero = {
            rule(Slot::Ougis2).range(48, 16).when("gp", "tNotBati", "tNotDown", "tNotSticking"),
            rule(Slot::Ougis1).range(999, 999).noApproach().when("gp"),
            rule(Slot::Skill3).range(999, 999).noApproach().when("notBuff"),
            rule(Slot::Skill2).range(64, 32).gate(128).minRange(32).when("gp", "tHpBelow:0.5"),
            rule(Slot::Skill2).range(64, 32).gate(128).when("gp", "notBuff"),
            rule(Slot::Skill1).range(64, 32).gate(128).when("gp"),
            rule(Slot::Gear03).gate(128),
        };
        profile.farm = {
            farm(Slot::Skill2).when("baseDanger"),
            farm(Slot::Skill1),
            farm(Slot::Gear03).on(TargetKind::AnyFarm),
        };
    }

    {
        Profile& profile = addProfile("Shino");
        profile.role = HeroRole::Ranged;
        profile.panicGear06 = false;
        profile.builds["default"] = {0, 1, 2};
        profile.hero = {
            rule(Slot::Ougis2).range(64, 16).speedGear().when("gp", "tNotBati", "tNotDown", "tNotSticking"),
            rule(Slot::Ougis1).range(64, 16).speedGear(),
            rule(Slot::Skill2).range(999, 999).noApproach().when("gp"),
            rule(Slot::Skill3).range(64, 16).speedGear().when("gp"),
            rule(Slot::Skill1).range(999, 999).gate(999).noApproach(),
        };
        profile.farm = {
            farm(Slot::Skill3).when("baseDanger"),
            farm(Slot::Skill2).when("baseDanger"),
            farm(Slot::Skill1),
        };
    }

    {
        Profile& profile = addProfile("Hiruzen");
        profile.role = HeroRole::Ranged;
        profile.panicGear06 = false;
        profile.builds["default"] = {0, 2, 8};
        profile.hero = {
            rule(Slot::Ougis2).range(96, 32).minRange(32).speedGear().when("selfNotBati", "tHpAbsBelow:10000", "tNoGear06"),
            rule(Slot::Ougis1).range(48, 16).speedGear().when("selfNotBati"),
            rule(Slot::Skill3).range(999, 999).noApproach().when("selfNotBati", "notBuff"),
            rule(Slot::Skill1).gate(128),
            rule(Slot::Skill2).range(96, 32).gate(128).minRange(32),
        };
        profile.farm = {
            farm(Slot::Skill1),
            farm(Slot::Skill2).when("baseDanger"),
        };
    }

    {
        Profile& profile = addProfile("Kiba");
        profile.role = HeroRole::Assassin;
        profile.baseCheck = false;
        profile.panicGear06 = false;
        profile.builds["default"] = {0, 4, 10};
        profile.hero = {
            rule(Slot::Ougis2).speedGear().when("gp", "tNotBati", "tNotDown", "tNotSticking", "notPowerUP"),
            rule(Slot::Ougis1).range(999, 999).noApproach().when("notPowerUP"),
            rule(Slot::Skill1).range(999, 999).noApproach().self().when("selfBati"),
            rule(Slot::Skill3).gate(128).when("notPowerUP", "selfNotBati", "hpAbove:0.5"),
        };
        profile.farm = {
            farm(Slot::Skill1).on(TargetKind::AnyFarm).self().when("selfBati"),
            farm(Slot::Skill3).on(TargetKind::Tower),
        };
    }

    {
        Profile& profile = addProfile("Minato");
        profile.role = HeroRole::Assassin;
        profile.hook = "minato";
        profile.builds["default"] = {6, 5, 2};
        profile.hero = {
            rule(Slot::Skill1).range(999, 999).noApproach().when("noMonster:HiraishinMark"),
            rule(Slot::Ougis2).range(64, 16).when("gp"),
            rule(Slot::Ougis1).range(64, 16),
            rule(Slot::Skill2).range(999, 16),
            rule(Slot::Skill3).range(999, 999).gate(999).noApproach(),
        };
        profile.farm = {
            farm(Slot::Skill3),
        };
    }

    {
        Profile& profile = addProfile("Hinata");
        profile.panicGear06 = false;
        profile.builds["default"] = {0, 1, 2};
        profile.hero = {
            rule(Slot::Ougis2).range(48, 32).speedGear().when("gp", "tNotBati", "tNotDown", "tNotSticking"),
            rule(Slot::Ougis1).range(48, 32).speedGear(),
            rule(Slot::Skill1).range(999, 999).noApproach(),
            rule(Slot::Skill2).range(48, 32).speedGear().when("skillUP", "gp"),
            rule(Slot::Skill3).range(48, 32).speedGear().when("skillUP", "gp"),
        };
        profile.farm = {
            farm(Slot::Skill2).when("baseDanger"),
            farm(Slot::Skill3).when("baseDanger"),
        };
    }

    {
        Profile& profile = addProfile("Neji");
        profile.panicGear06 = false;
        profile.builds["default"] = {0, 1, 7};
        profile.hero = {
            rule(Slot::Ougis2).range(48, 32).speedGear().when("gp", "tNotBati", "tNotDown", "tNotSticking"),
            rule(Slot::Ougis1).range(48, 32).speedGear(),
            rule(Slot::Skill1).range(999, 999).gate(999).noApproach(),
            rule(Slot::Skill3).range(48, 32).gate(999).speedGear().when("selfBati", "gp"),
            rule(Slot::Skill2).gate(128).when("gp"),
        };
        profile.farm = {
            farm(Slot::Skill2).when("baseDanger"),
            farm(Slot::Skill3).when("baseDanger"),
        };
    }

    {
        Profile& profile = addProfile("Orochimaru");
        profile.builds["default"] = {6, 5, 2};
        profile.hero = {
            rule(Slot::Ougis2).range(48, 32).when("gp", "tNotBati"),
            rule(Slot::Ougis1).range(48, 32),
            rule(Slot::Skill3).range(64, 32).when("gp"),
            rule(Slot::Skill2).range(64, 32).when("gp"),
            rule(Slot::Skill1).range(48, 32).gate(128).when("gp"),
        };
        profile.farm = {
            farm(Slot::Skill1),
        };
    }

    {
        Profile& profile = addProfile("Nagato");
        profile.role = HeroRole::Ranged;
        profile.builds["default"] = {6, 1, 5};
        profile.hero = {
            rule(Slot::Ougis2).range(64, 32).when("gp", "tNotBati"),
            rule(Slot::Ougis1).range(64, 32),
            rule(Slot::Skill3).range(96, 32).when("gp"),
            rule(Slot::Skill2).range(96, 32).when("gp"),
            rule(Slot::Skill1).range(64, 32).gate(128).when("gp"),
        };
        profile.farm = {
            farm(Slot::Skill1),
        };
    }

    {
        Profile& profile = addProfile("Pain");
        profile.builds["default"] = {6, 5, 10};
        profile.hero = {
            rule(Slot::Ougis2).range(64, 32).when("gp", "tNotBati"),
            rule(Slot::Ougis1).range(64, 32),
            rule(Slot::Skill3).range(96, 32).when("gp"),
            rule(Slot::Skill2).range(96, 32).when("gp"),
            rule(Slot::Skill1).range(64, 32).gate(128).when("gp"),
        };
        profile.farm = {
            farm(Slot::Skill1),
        };
    }

    return profiles;
}

// For a hero without its own profile (e.g. a modded hero)
Profile builtinDefaultProfile()
{
    Profile profile;
    profile.name = "default";
    profile.panicGear06 = false;
    profile.builds["default"] = {4, 7, 8};
    profile.hero = {
        rule(Slot::Ougis2).range(64, 32).gate(128).when("gp", "tNotDown"),
        rule(Slot::Ougis1).range(64, 32).gate(128).when("gp"),
        rule(Slot::Skill1).range(64, 32).gate(128).when("gp"),
        rule(Slot::Skill2).range(64, 32).gate(128).when("gp"),
        rule(Slot::Skill3).range(64, 32).gate(128).when("gp"),
    };
    profile.farm = {
        farm(Slot::Skill1).when("baseDanger"),
    };
    return profile;
}

// Heroes that keep their old built-in routine: the HardCore guardians
bool usesLegacyAI(const std::string& hero)
{
    static const char* const legacy[] = {"Han", "Roshi"};
    for (const char* name : legacy)
        if (hero == name)
            return true;
    return false;
}

}  // namespace ai
