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
    RuleBuilder& range(int x, int y)
    {
        _rule.rangeX = x;
        _rule.rangeY = y;
        return *this;
    }
    RuleBuilder& gate(int x)
    {
        _rule.gate = x;
        return *this;
    }
    RuleBuilder& minRange(int x)
    {
        _rule.minRange = x;
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
            std::string s = text;
            Cond c;
            auto colon = s.find(':');
            c.name = s.substr(0, colon);
            if (colon != std::string::npos)
            {
                c.arg = s.substr(colon + 1);
                c.value = (float)atof(c.arg.c_str());
            }
            _rule.conds.push_back(c);
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
    auto add = [&](const char* name) -> Profile& {
        Profile& p = profiles[name];
        p.name = name;
        return p;
    };

    {
        Profile& p = add("Naruto");
        p.nattackX = 40;
        p.nattackY = 32;
        p.panicGear06 = false;
        p.builds["default"] = {3, 7, 2};
        p.hero = {
            rule(Slot::Ougis2).range(999, 999).noApproach().when("selfNotBati"),
            rule(Slot::Skill2).range(999, 999).noApproach().when("selfNotBati"),
            rule(Slot::Ougis1).range(96, 32).gate(128).minRange(32).when("gp"),
            rule(Slot::Gear03).range(96, 32).gate(128),
            rule(Slot::Skill3).range(96, 32).gate(128).minRange(32).when("gp"),
            rule(Slot::Skill1).range(96, 32).gate(128).minRange(32).when("gp"),
        };
        p.farm = {
            farm(Slot::Skill2).on(TargetKind::AnyFarm),
        };
    }

    {
        Profile& p = add("SageNaruto");
        p.nattackX = 40;
        p.nattackY = 32;
        p.panicGear06 = false;
        p.builds["default"] = {3, 7, 2};
        p.hero = {
            rule(Slot::Ougis2).range(999, 999).noApproach(),
            rule(Slot::Skill2).range(999, 999).noApproach(),
            rule(Slot::Skill3).range(999, 999).noApproach().when("gp"),
            rule(Slot::Ougis1).range(96, 32).gate(128),
            rule(Slot::Gear03).range(96, 32).gate(128),
            rule(Slot::Skill1).range(96, 32).gate(128).minRange(32),
        };
        p.farm = {
            farm(Slot::Skill2).on(TargetKind::AnyFarm),
        };
    }

    {
        Profile& p = add("RikudoNaruto");
        p.panicGear06 = false;
        p.builds["default"] = {3, 7, 2};
        p.hero = {
            rule(Slot::Ougis2).range(999, 999).noApproach(),
            rule(Slot::Skill2).range(999, 999).noApproach(),
            rule(Slot::Skill3).range(999, 999).noApproach(),
            rule(Slot::Skill1).range(999, 999).noApproach(),
            rule(Slot::Gear03).range(999, 999).noApproach(),
            rule(Slot::Ougis1).gate(128),
        };
        p.farm = {
            farm(Slot::Skill1),
            farm(Slot::Skill2).on(TargetKind::AnyFarm),
        };
    }

    {
        Profile& p = add("Asuma");
        p.builds["default"] = {6, 5, 2};
        p.hero = {
            rule(Slot::Ougis1).range(999, 999).noApproach().when("gp"),
            rule(Slot::Skill3).range(96, 32).when("gp"),
            rule(Slot::Skill2).range(96, 32).when("gp"),
            rule(Slot::Ougis2).range(96, 32).gate(128).when("gp", "tNotBati"),
            rule(Slot::Skill1).range(96, 32).gate(128).when("gp"),
        };
        p.farm = {
            farm(Slot::Skill2).when("baseDanger"),
            farm(Slot::Skill1),
        };
    }

    {
        Profile& p = add("Kakashi");
        p.role = HeroRole::Assassin;
        p.builds["default"] = {6, 5, 2};
        p.hero = {
            rule(Slot::Skill3).range(999, 999).noApproach(),
            rule(Slot::Skill2).range(999, 999).noApproach().when("gp"),
            rule(Slot::Ougis2).range(64, 32).gate(128).when("gp", "tNotBati", "tNotDown", "tNotSticking"),
            rule(Slot::Ougis1).range(64, 32).gate(128),
            rule(Slot::Skill1).range(64, 32).gate(128),
        };
        p.farm = {
            farm(Slot::Skill3),
            farm(Slot::Skill1),
        };
    }

    {
        Profile& p = add("Konan");
        p.role = HeroRole::Ranged;
        p.nattackX = 48;
        p.nattackY = 32;
        p.panicGear06 = false;
        p.builds["default"] = {3, 5, 2};
        p.hero = {
            rule(Slot::Skill1).range(999, 999).noApproach(),
            rule(Slot::Ougis2).range(48, 16).when("gp", "tNotBati", "tNotDown", "tNotSticking", "selfNotBati"),
            rule(Slot::Skill3).range(128, 16).minRange(96).when("invisible", "gp", "tNotDown"),
            rule(Slot::Skill2).range(128, 16).minRange(96).when("invisible", "gp", "tNotDown"),
            rule(Slot::NAttack).range(8, 8).gate(128).when("selfBati"),
            rule(Slot::Ougis1).range(48, 32).gate(128).minRange(32),
            rule(Slot::Skill2).range(48, 32).gate(128).minRange(32),
            rule(Slot::Gear03).range(48, 32).gate(128),
        };
        p.farm = {
            farm(Slot::Gear03).on(TargetKind::AnyFarm),
            farm(Slot::Ougis1).when("baseDanger"),
            farm(Slot::Skill2).when("baseDanger"),
        };
    }

    {
        Profile& p = add("Kurenai");
        p.nattackX = 40;
        p.nattackY = 32;
        p.panicGear06 = false;
        p.builds["default"] = {3, 7, 2};
        p.hero = {
            rule(Slot::Skill1).range(999, 999).noApproach(),
            rule(Slot::Skill2).range(128, 32).minRange(64).when("invisible", "gp", "tNotDown"),
            rule(Slot::Ougis2).range(96, 32).gate(128).when("gp", "tNotBati", "tNotDown", "tNotSticking", "selfNotBati"),
            rule(Slot::Ougis1).range(64, 32).gate(128).minRange(32).when("gp"),
            rule(Slot::Gear03).range(96, 32).gate(128),
            rule(Slot::Skill3).gate(128).when("gp"),
            rule(Slot::Skill2).range(64, 32).gate(128).minRange(32).when("gp"),
        };
        p.farm = {
            farm(Slot::Gear03).on(TargetKind::AnyFarm),
            farm(Slot::Skill3).when("baseDanger"),
        };
    }

    {
        Profile& p = add("Deidara");
        p.role = HeroRole::Pusher;
        p.subRoles = {"ranged"};
        p.baseCheck = false;
        p.panicGear06 = false;
        p.builds["default"] = {0, 1, 5};
        p.hero = {
            rule(Slot::Ougis2).range(48, 16).speedGear().when("selfNotBati"),
            rule(Slot::Ougis1).range(999, 999).noApproach(),
            rule(Slot::Skill3).range(64, 16).speedGear(),
            rule(Slot::Skill2).range(999, 999).noApproach(),
            rule(Slot::Skill1).range(999, 999).gate(999).noApproach(),
            rule(Slot::NAttack).range(8, 8).gate(128).when("selfBati"),
        };
        p.farm = {
            farm(Slot::Ougis2).on(TargetKind::Tower).when("baseDanger"),
            farm(Slot::Skill1).on(TargetKind::Tower),
            farm(Slot::Skill2).on(TargetKind::AnyFarm),
        };
    }

    {
        Profile& p = add("Ino");
        p.role = HeroRole::Support;
        p.idleWhenBuff = true;
        p.builds["default"] = {6, 5, 1};
        p.hero = {
            rule(Slot::Ougis2).range(96, 16),
            rule(Slot::Skill2).range(96, 16).when("gp"),
            rule(Slot::Skill3).range(96, 16).when("gp"),
            rule(Slot::Ougis1).gate(128).when("gp"),
            rule(Slot::Skill1).gate(128),
        };
        p.farm = {
            farm(Slot::Skill1),
        };
    }

    {
        Profile& p = add("Sai");
        p.role = HeroRole::Ranged;
        p.builds["default"] = {6, 5, 1};
        p.hero = {
            rule(Slot::Ougis2).range(48, 16).when("gp", "tNotBati", "tNotDown", "tNotSticking", "selfNotBati"),
            rule(Slot::Skill3).range(64, 16),
            rule(Slot::Skill2).range(999, 999).noApproach(),
            rule(Slot::Ougis1).range(999, 999).noApproach(),
            rule(Slot::Skill1).range(999, 999).gate(999).noApproach(),
            rule(Slot::NAttack).range(8, 8).gate(128).when("selfBati"),
        };
        p.farm = {
            farm(Slot::Skill1),
            farm(Slot::Skill2).when("baseDanger", "gp"),
        };
    }

    {
        Profile& p = add("Sakura");
        p.role = HeroRole::Support;
        p.subRoles = {"healer", "pusher"};
        p.baseCheck = false;
        p.panicGear06 = false;
        p.builds["default"] = {0, 4, 8};
        p.builds["push"] = {4, 8, 0};
        p.builds["sustain"] = {7, 8, 1};
        p.hero = {
            rule(Slot::Ougis2).range(48, 16).speedGear().when("gp", "tNotBati", "tNotDown", "tNotSticking", "notBuff"),
            rule(Slot::Ougis1).range(48, 16).speedGear().when("notBuff"),
            rule(Slot::Skill1).range(999, 999).gate(999).noApproach().self().when("hpBelow:0.9"),
            rule(Slot::Skill2).range(64, 32).gate(128).when("notBuff", "gp"),
            rule(Slot::Skill3).gate(128).when("notBuff", "hpAbove:0.5"),
        };
        p.farm = {
            farm(Slot::Skill1).on(TargetKind::AnyFarm).self().when("hpBelow:0.9"),
            farm(Slot::Skill3).on(TargetKind::Tower).when("notBuff"),
            farm(Slot::Skill2),
        };
    }

    {
        Profile& p = add("Tenten");
        p.role = HeroRole::Ranged;
        p.panicGear06 = false;
        p.builds["default"] = {0, 5, 2};
        p.hero = {
            rule(Slot::Skill1).range(999, 999).noApproach(),
            rule(Slot::Ougis2).range(48, 16).speedGear().when("notBuff", "gp"),
            rule(Slot::Ougis1).range(48, 16).speedGear().when("tNotBati", "notBuff"),
            rule(Slot::Skill2).range(96, 16).gate(128).when("gp"),
            rule(Slot::Skill3).range(96, 16).gate(128).when("gp"),
        };
        p.farm = {
            farm(Slot::Skill1),
            farm(Slot::Skill3),
            farm(Slot::Skill2).when("baseDanger"),
        };
    }

    {
        Profile& p = add("Itachi");
        p.role = HeroRole::Assassin;
        p.nattackX = 56;
        p.nattackY = 32;
        p.builds["default"] = {6, 2, 5};
        p.hero = {
            rule(Slot::Ougis2).range(48, 32).when("selfNotBati"),
            rule(Slot::Ougis1).range(999, 999).noApproach().when("gp", "tNotBati", "tNotDown", "tNotSticking", "selfNotBati"),
            rule(Slot::Skill3).range(56, 32).gate(128).when("selfNotBati", "gp"),
            rule(Slot::Skill2).range(56, 32).gate(128).when("selfNotBati", "gp"),
            rule(Slot::Skill1).range(56, 32).gate(128).when("selfNotBati", "gp"),
        };
        p.farm = {
            farm(Slot::Skill3).when("baseDanger"),
            farm(Slot::Skill2).when("baseDanger"),
            farm(Slot::Skill1),
        };
    }

    {
        Profile& p = add("Jiraiya");
        p.role = HeroRole::Ranged;
        p.builds["default"] = {6, 1, 2};
        p.hero = {
            rule(Slot::Ougis2).range(999, 999).noApproach(),
            rule(Slot::Skill2).range(999, 999).noApproach().when("gp"),
            rule(Slot::Ougis1).gate(128),
            rule(Slot::Skill3).gate(128),
            rule(Slot::Skill1).gate(128),
        };
        p.farm = {
            farm(Slot::Skill3).when("baseDanger"),
            farm(Slot::Skill1),
        };
    }

    {
        Profile& p = add("SageJiraiya");
        p.role = HeroRole::Ranged;
        p.builds["default"] = {6, 1, 2};
        p.hero = {
            rule(Slot::Skill2).range(999, 999).noApproach().when("gp"),
            rule(Slot::Ougis2).range(96, 32).gate(128).when("gp"),
            rule(Slot::Ougis1).gate(128),
            rule(Slot::Skill3).gate(128),
            rule(Slot::Skill1).gate(128),
        };
        p.farm = {
            farm(Slot::Skill3).when("baseDanger"),
            farm(Slot::Skill1),
        };
    }

    {
        Profile& p = add("Suigetsu");
        p.panicGear06 = false;
        p.builds["default"] = {0, 5, 1};
        p.hero = {
            rule(Slot::Ougis2).range(48, 16).speedGear().when("gp", "tNotBati", "tNotDown", "tNotSticking", "selfNotBati"),
            rule(Slot::Ougis1).range(48, 16).speedGear().when("selfNotBati"),
            rule(Slot::Skill3).range(999, 999).noApproach().when("gp", "selfNotBati", "notBuff"),
            rule(Slot::Skill2).range(96, 32).gate(128),
            rule(Slot::Skill1).range(96, 32).gate(128),
        };
        p.farm = {
            farm(Slot::Skill1),
            farm(Slot::Skill2).when("baseDanger"),
        };
    }

    {
        Profile& p = add("Tsunade");
        p.role = HeroRole::Tank;
        p.subRoles = {"healer"};
        p.panicGear06 = false;
        p.builds["default"] = {3, 2, 4};
        p.hero = {
            rule(Slot::Ougis2).range(64, 16).when("gp", "selfNotBati"),
            rule(Slot::Skill3).range(999, 999).noApproach().when("gp", "selfNotBati"),
            rule(Slot::Gear03).range(64, 32).gate(128),
            rule(Slot::Ougis1).range(64, 32).gate(128),
            rule(Slot::Skill2).range(64, 32).gate(128),
            rule(Slot::Skill1).range(64, 32).gate(128),
        };
        p.farm = {
            farm(Slot::Gear03).on(TargetKind::AnyFarm),
            farm(Slot::Skill1).when("baseDanger"),
        };
    }

    {
        Profile& p = add("Sasuke");
        p.role = HeroRole::Assassin;
        p.builds["default"] = {6, 1, 2};
        p.hero = {
            rule(Slot::Ougis2).range(999, 999).noApproach().when("selfNotBati"),
            rule(Slot::Skill3).range(999, 999).gate(999).noApproach().when("notTaunt", "selfNotBati"),
            rule(Slot::Ougis1).range(96, 32).gate(128),
            rule(Slot::Skill2).range(96, 32).gate(128),
            rule(Slot::Skill1).range(96, 32).gate(128),
        };
        p.farm = {
            farm(Slot::Skill1),
            farm(Slot::Skill2).when("baseDanger"),
            farm(Slot::Skill3),
        };
    }

    {
        Profile& p = add("ImmortalSasuke");
        p.role = HeroRole::Assassin;
        p.builds["default"] = {6, 1, 2};
        p.hero = {
            rule(Slot::Ougis1).range(999, 999).noApproach(),
            rule(Slot::Ougis2).range(48, 32),
            rule(Slot::Skill3).range(999, 999).gate(999).noApproach().when("notTaunt"),
            rule(Slot::Skill2).range(96, 32).gate(128),
            rule(Slot::Skill1).range(96, 32).gate(128),
        };
        p.farm = {
            farm(Slot::Skill1),
            farm(Slot::Skill2).when("baseDanger"),
        };
    }

    {
        Profile& p = add("Kankuro");
        p.role = HeroRole::Summoner;
        p.retreatGap = 5000;
        p.builds["default"] = {6, 4, 8};
        p.hero = {
            rule(Slot::Skill3).range(999, 999).noApproach(),
            rule(Slot::Skill1).range(999, 999).noApproach().self().when("noMonster:Karasu"),
            rule(Slot::Ougis1).range(999, 999).noApproach().self().when("noMonster:Sanshouuo"),
            rule(Slot::Ougis2).range(999, 999).noApproach().self().when("noMonster:Saso"),
        };
        p.farm = {
            farm(Slot::Ougis1).on(TargetKind::AnyFarm).self().when("noMonster:Sanshouuo"),
            farm(Slot::Ougis2).on(TargetKind::AnyFarm).self().when("noMonster:Saso"),
            farm(Slot::Skill1).on(TargetKind::AnyFarm).self().when("noMonster:Karasu"),
        };
    }

    {
        Profile& p = add("Chiyo");
        p.role = HeroRole::Support;
        p.subRoles = {"summoner"};
        p.retreatGap = 5000;
        p.builds["default"] = {6, 7, 4};
        p.hero = {
            rule(Slot::Ougis1).range(999, 999).noApproach().self().when("noBuffTime", "allyNear"),
            rule(Slot::Ougis2).range(96, 32).minRange(48).when("gp", "tNotBati", "tNotDown", "tNotSticking"),
            rule(Slot::Skill1).range(999, 999).noApproach().self().when("noMonster:Parents"),
        };
        p.farm = {
            farm(Slot::Skill1).on(TargetKind::AnyFarm).self().when("noMonster:Parents"),
        };
    }

    {
        Profile& p = add("Kakuzu");
        p.role = HeroRole::Tank;
        p.retreatGap = 5000;
        p.panicGear06 = false;
        p.hook = "kakuzu";
        p.builds["default"] = {3, 8, 4};
        p.hero = {
            rule(Slot::Skill3).range(999, 999).noApproach(),
            rule(Slot::Ougis1).range(999, 999).noApproach().self().when("summonable"),
            rule(Slot::Skill2).range(96, 32),
            rule(Slot::Gear03).range(96, 32),
        };
        p.farm = {
            farm(Slot::Ougis1).on(TargetKind::AnyFarm).self().when("summonable"),
        };
    }

    {
        Profile& p = add("Shikamaru");
        p.role = HeroRole::Support;
        p.panicGear06 = false;
        p.builds["default"] = {0, 7, 8};
        p.hero = {
            rule(Slot::Ougis2).range(999, 999).noApproach().when("tNotBati", "tNotSticking"),
            rule(Slot::Skill2).range(999, 999).noApproach().when("gp"),
            rule(Slot::Ougis1).range(48, 32).speedGear(),
            rule(Slot::Skill1).range(999, 999).gate(999).noApproach().when("lv:2"),
            rule(Slot::Skill3).range(156, 32).gate(999).minRange(32).speedGear().when("tNotBati", "tNotSticking"),
        };
        p.farm = {
            farm(Slot::Skill2).when("baseDanger"),
        };
    }

    {
        Profile& p = add("Gaara");
        p.role = HeroRole::Tank;
        p.subRoles = {"ranged"};
        p.builds["default"] = {6, 2, 5};
        p.hero = {
            rule(Slot::Ougis2).range(999, 999).noApproach().when("selfNotBati", "gp"),
            rule(Slot::Ougis1).range(999, 999).noApproach().when("selfNotBati", "gp"),
            rule(Slot::Skill3).range(999, 999).noApproach().when("selfNotBati", "gp"),
            rule(Slot::Skill2).range(999, 999).noApproach().when("selfNotBati", "gp"),
            rule(Slot::Skill1).gate(128),
        };
        p.farm = {
            farm(Slot::Skill1),
        };
    }

    {
        Profile& p = add("Tobirama");
        p.nattackX = 48;
        p.nattackY = 32;
        p.builds["default"] = {6, 1, 7};
        p.hero = {
            rule(Slot::Ougis2).range(999, 999).noApproach().when("gp"),
            rule(Slot::Skill1).range(999, 999).noApproach().when("tNotBati", "tIdle"),
            rule(Slot::Skill2).range(64, 16),
            rule(Slot::Ougis1).range(48, 32).gate(128),
            rule(Slot::Skill3).range(48, 32).gate(128),
        };
        p.farm = {
            farm(Slot::Skill3).when("baseDanger"),
            farm(Slot::Skill2).when("baseDanger"),
        };
    }

    {
        Profile& p = add("Choji");
        p.role = HeroRole::Tank;
        p.subRoles = {"pusher"};
        p.builds["default"] = {6, 1, 4};
        p.builds["sustain"] = {8, 6, 7};
        p.hero = {
            rule(Slot::Skill1).range(999, 999).noApproach().when("selfNotBati"),
            rule(Slot::Skill3).range(96, 16),
            rule(Slot::Ougis1).gate(128),
            rule(Slot::Skill2).gate(128).when("skillUP"),
            rule(Slot::Ougis2).gate(128),
        };
        p.farm = {
            farm(Slot::Skill2),
            farm(Slot::Ougis1).when("baseDanger"),
            farm(Slot::Ougis2).on(TargetKind::Tower),
        };
    }

    {
        Profile& p = add("Karin");
        p.role = HeroRole::Support;
        p.subRoles = {"healer"};
        p.builds["default"] = {6, 1, 7};
        p.hero = {
            rule(Slot::Ougis2).range(96, 16).when("gp", "tNotBati", "tNotDown", "tNotSticking"),
            rule(Slot::Ougis1).range(999, 999).noApproach().self().when("hpBelow:0.9", "noBuffTime", "notHealing"),
            rule(Slot::Skill3).range(128, 16).when("gp"),
            rule(Slot::Skill2).range(96, 32).gate(128),
            rule(Slot::Skill1).range(96, 32).gate(128),
        };
        p.farm = {
            farm(Slot::Skill1),
            farm(Slot::Skill2).when("baseDanger"),
        };
    }

    {
        Profile& p = add("Lee");
        p.role = HeroRole::Assassin;
        p.nattackX = 48;
        p.nattackY = 32;
        p.builds["default"] = {6, 7, 2};
        p.hero = {
            rule(Slot::Skill1).range(999, 999).gate(128).noApproach().when("bamenBelow:5"),
            rule(Slot::Ougis1).range(48, 32).gate(128).when("gp", "tNotBati", "tNotDown", "tNotSticking", "bamenAtLeast:1"),
            rule(Slot::Skill2).range(48, 32).gate(128),
        };
        p.farm = {
            farm(Slot::Skill2),
        };
    }

    {
        Profile& p = add("RockLee");
        p.role = HeroRole::Assassin;
        p.nattackX = 46;
        p.nattackY = 32;
        p.builds["default"] = {6, 7, 2};
        p.hero = {
            rule(Slot::Skill1).range(999, 999).noApproach().when("bamenBelow:5"),
            rule(Slot::Ougis2).range(999, 999).noApproach().when("gp", "tNotBati", "tNotDown", "tNotSticking", "bamenAtLeast:3"),
            rule(Slot::Ougis1).range(46, 32).gate(128).when("gp", "tNotBati", "tNotDown", "tNotSticking", "bamenAtLeast:1"),
            rule(Slot::Skill2).range(46, 32).gate(128),
            rule(Slot::Skill3).range(46, 32).gate(128).when("bamenAtLeast:3"),
        };
        p.farm = {
            farm(Slot::Skill2),
            farm(Slot::Skill3).when("baseDanger", "bamenAtLeast:3"),
        };
    }

    {
        Profile& p = add("Jugo");
        p.role = HeroRole::Tank;
        p.nattackX = 48;
        p.nattackY = 32;
        p.builds["default"] = {6, 1, 2};
        p.hero = {
            rule(Slot::Skill3).range(999, 999).noApproach().when("gp", "notBuff"),
            rule(Slot::Ougis2).gate(128).when("gp", "tNotBati", "tNotDown", "tNotSticking", "notBuff"),
            rule(Slot::Ougis1).gate(128).when("notBuff"),
            rule(Slot::Skill2).range(96, 32).gate(128).when("notBuff"),
            rule(Slot::Skill1).range(96, 32).gate(128).when("notBuff"),
        };
        p.farm = {
            farm(Slot::Skill1).when("notBuff"),
        };
    }

    {
        Profile& p = add("Kisame");
        p.role = HeroRole::Tank;
        p.nattackX = 64;
        p.nattackY = 32;
        p.panicGear06 = false;
        p.builds["default"] = {0, 1, 5};
        p.builds["sustain"] = {8, 1, 7};
        p.hero = {
            rule(Slot::Ougis2).range(48, 32).speedGear().when("gp", "tNotBati", "tNotDown", "tNotSticking", "notBuff"),
            rule(Slot::Ougis1).range(48, 32).speedGear().when("notBuff"),
            rule(Slot::Skill3).range(96, 32).gate(999).when("notBuff"),
            rule(Slot::Skill2).range(96, 32).gate(999).when("gp", "notBuff"),
            rule(Slot::Skill1).range(64, 32).gate(128),
        };
        p.farm = {
            farm(Slot::Skill2).when("baseDanger"),
            farm(Slot::Skill1),
        };
    }

    {
        Profile& p = add("Hidan");
        p.nattackX = 48;
        p.nattackY = 32;
        p.hook = "hidan";
        p.builds["default"] = {6, 1, 7};
        p.hero = {
            rule(Slot::Skill1).range(48, 32).when("selfNotBati", "noMonster:CircleMark", "ougis2Ready"),
            rule(Slot::Ougis2).range(48, 32).when("selfNotBati"),
            rule(Slot::Skill2).range(56, 32).when("gp", "selfNotBati"),
            rule(Slot::Ougis1).range(48, 32).gate(128).when("gp", "selfNotBati"),
            rule(Slot::Skill3).range(48, 32).gate(128).when("selfNotBati"),
        };
        p.farm = {
            farm(Slot::Skill3).when("baseDanger"),
            farm(Slot::Skill2).when("baseDanger"),
        };
    }

    {
        Profile& p = add("Tobi");
        p.nattackX = 64;
        p.nattackY = 32;
        p.panicGear06 = false;
        p.builds["default"] = {3, 2, 5};
        p.hero = {
            rule(Slot::Ougis2).range(48, 16).when("gp", "tNotBati", "tNotDown", "tNotSticking"),
            rule(Slot::Ougis1).range(999, 999).noApproach().when("gp"),
            rule(Slot::Skill3).range(999, 999).noApproach().when("notBuff"),
            rule(Slot::Skill2).range(64, 32).gate(128).minRange(32).when("gp", "tHpBelow:0.5"),
            rule(Slot::Skill2).range(64, 32).gate(128).when("gp", "notBuff"),
            rule(Slot::Skill1).range(64, 32).gate(128).when("gp"),
            rule(Slot::Gear03).gate(128),
        };
        p.farm = {
            farm(Slot::Skill2).when("baseDanger"),
            farm(Slot::Skill1),
            farm(Slot::Gear03).on(TargetKind::AnyFarm),
        };
    }

    {
        Profile& p = add("Shino");
        p.role = HeroRole::Ranged;
        p.panicGear06 = false;
        p.builds["default"] = {0, 1, 2};
        p.hero = {
            rule(Slot::Ougis2).range(64, 16).speedGear().when("gp", "tNotBati", "tNotDown", "tNotSticking"),
            rule(Slot::Ougis1).range(64, 16).speedGear(),
            rule(Slot::Skill2).range(999, 999).noApproach().when("gp"),
            rule(Slot::Skill3).range(64, 16).speedGear().when("gp"),
            rule(Slot::Skill1).range(999, 999).gate(999).noApproach(),
        };
        p.farm = {
            farm(Slot::Skill3).when("baseDanger"),
            farm(Slot::Skill2).when("baseDanger"),
            farm(Slot::Skill1),
        };
    }

    {
        Profile& p = add("Hiruzen");
        p.role = HeroRole::Ranged;
        p.panicGear06 = false;
        p.builds["default"] = {0, 2, 8};
        p.hero = {
            rule(Slot::Ougis2).range(96, 32).minRange(32).speedGear().when("selfNotBati", "tHpAbsBelow:10000", "tNoGear06"),
            rule(Slot::Ougis1).range(48, 16).speedGear().when("selfNotBati"),
            rule(Slot::Skill3).range(999, 999).noApproach().when("selfNotBati", "notBuff"),
            rule(Slot::Skill1).gate(128),
            rule(Slot::Skill2).range(96, 32).gate(128).minRange(32),
        };
        p.farm = {
            farm(Slot::Skill1),
            farm(Slot::Skill2).when("baseDanger"),
        };
    }

    {
        Profile& p = add("Kiba");
        p.role = HeroRole::Assassin;
        p.baseCheck = false;
        p.panicGear06 = false;
        p.builds["default"] = {0, 4, 8};
        p.hero = {
            rule(Slot::Ougis2).speedGear().when("gp", "tNotBati", "tNotDown", "tNotSticking", "notPowerUP"),
            rule(Slot::Ougis1).range(999, 999).noApproach().when("notPowerUP"),
            rule(Slot::Skill1).range(999, 999).noApproach().self().when("selfBati"),
            rule(Slot::Skill3).gate(128).when("notPowerUP", "selfNotBati", "hpAbove:0.5"),
        };
        p.farm = {
            farm(Slot::Skill1).on(TargetKind::AnyFarm).self().when("selfBati"),
            farm(Slot::Skill3).on(TargetKind::Tower),
        };
    }

    {
        Profile& p = add("Minato");
        p.role = HeroRole::Assassin;
        p.hook = "minato";
        p.builds["default"] = {6, 5, 2};
        p.hero = {
            rule(Slot::Skill1).range(999, 999).noApproach().when("noMonster:HiraishinMark"),
            rule(Slot::Ougis2).range(64, 16).when("gp"),
            rule(Slot::Ougis1).range(64, 16),
            rule(Slot::Skill2).range(999, 16),
            rule(Slot::Skill3).range(999, 999).gate(999).noApproach(),
        };
        p.farm = {
            farm(Slot::Skill3),
        };
    }

    {
        Profile& p = add("Hinata");
        p.panicGear06 = false;
        p.builds["default"] = {0, 1, 2};
        p.hero = {
            rule(Slot::Ougis2).range(48, 32).speedGear().when("gp", "tNotBati", "tNotDown", "tNotSticking"),
            rule(Slot::Ougis1).range(48, 32).speedGear(),
            rule(Slot::Skill1).range(999, 999).noApproach(),
            rule(Slot::Skill2).range(48, 32).speedGear().when("skillUP", "gp"),
            rule(Slot::Skill3).range(48, 32).speedGear().when("skillUP", "gp"),
        };
        p.farm = {
            farm(Slot::Skill2).when("baseDanger"),
            farm(Slot::Skill3).when("baseDanger"),
        };
    }

    {
        Profile& p = add("Neji");
        p.panicGear06 = false;
        p.builds["default"] = {0, 1, 7};
        p.hero = {
            rule(Slot::Ougis2).range(48, 32).speedGear().when("gp", "tNotBati", "tNotDown", "tNotSticking"),
            rule(Slot::Ougis1).range(48, 32).speedGear(),
            rule(Slot::Skill1).range(999, 999).gate(999).noApproach(),
            rule(Slot::Skill3).range(48, 32).gate(999).speedGear().when("selfBati", "gp"),
            rule(Slot::Skill2).gate(128).when("gp"),
        };
        p.farm = {
            farm(Slot::Skill2).when("baseDanger"),
            farm(Slot::Skill3).when("baseDanger"),
        };
    }

    {
        Profile& p = add("Orochimaru");
        p.builds["default"] = {6, 5, 2};
        p.hero = {
            rule(Slot::Ougis2).range(48, 32).when("gp", "tNotBati"),
            rule(Slot::Ougis1).range(48, 32),
            rule(Slot::Skill3).range(64, 32).when("gp"),
            rule(Slot::Skill2).range(64, 32).when("gp"),
            rule(Slot::Skill1).range(48, 32).gate(128).when("gp"),
        };
        p.farm = {
            farm(Slot::Skill1),
        };
    }

    {
        Profile& p = add("Nagato");
        p.role = HeroRole::Ranged;
        p.builds["default"] = {6, 1, 5};
        p.hero = {
            rule(Slot::Ougis2).range(64, 32).when("gp", "tNotBati"),
            rule(Slot::Ougis1).range(64, 32),
            rule(Slot::Skill3).range(96, 32).when("gp"),
            rule(Slot::Skill2).range(96, 32).when("gp"),
            rule(Slot::Skill1).range(64, 32).gate(128).when("gp"),
        };
        p.farm = {
            farm(Slot::Skill1),
        };
    }

    {
        Profile& p = add("Pain");
        p.builds["default"] = {6, 5, 2};
        p.hero = {
            rule(Slot::Ougis2).range(64, 32).when("gp", "tNotBati"),
            rule(Slot::Ougis1).range(64, 32),
            rule(Slot::Skill3).range(96, 32).when("gp"),
            rule(Slot::Skill2).range(96, 32).when("gp"),
            rule(Slot::Skill1).range(64, 32).gate(128).when("gp"),
        };
        p.farm = {
            farm(Slot::Skill1),
        };
    }

    return profiles;
}

// For a hero without its own profile (e.g. a modded hero)
Profile builtinDefaultProfile()
{
    Profile p;
    p.name = "default";
    p.panicGear06 = false;
    p.builds["default"] = {4, 7, 8};
    p.hero = {
        rule(Slot::Ougis2).range(64, 32).gate(128).when("gp", "tNotDown"),
        rule(Slot::Ougis1).range(64, 32).gate(128).when("gp"),
        rule(Slot::Skill1).range(64, 32).gate(128).when("gp"),
        rule(Slot::Skill2).range(64, 32).gate(128).when("gp"),
        rule(Slot::Skill3).range(64, 32).gate(128).when("gp"),
    };
    p.farm = {
        farm(Slot::Skill1).when("baseDanger"),
    };
    return p;
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
