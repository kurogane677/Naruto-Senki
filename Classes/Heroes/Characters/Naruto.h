#pragma once
#include "Characters.h"

// Naruto and his two transformations. Transforming keeps the same object and only
// changes the character name, so one class covers all three forms; his shadow clones
// are Naruto objects too; Rikudo's Kurama is a plain Hero.
class Naruto : public Hero
{
public:
    CREATE_FUNC(Naruto);

    int expGainPercent() const override;

protected:
    bool onTransformSkillStart() override;
    bool onTransformSkillEnd() override;
    CharacterId cloneCharacter() const override;
    bool setUpClone(Hero* clone) override;
    bool cloneStartsWithOwnerHP() const override;
    void finishClone(Hero* clone) override;
    float cloneLifetime(int cloneTime) const override;
    bool clonesExpire() const override;
    int cloneDamageSharePercent() const override;
    bool targetsHeroesOnly() const override;
    void cloneAI() override;

private:
    enum class Form
    {
        Base,
        Sage,
        Rikudo,
    };
    Form form() const;
    bool isKuramaSummon() const;

    // clone AI
    CCPoint vectorToTarget() const;
    bool canAct() const;
    void chaseAndAttack(const CCPoint& toTarget);
    void fightHero(const CCPoint& toTarget);
};
