#pragma once
#include "Characters.h"

// Naruto and his two transformations. Transforming keeps the same object and only
// changes the character name, so one class covers all three forms; his shadow clones
// are Naruto objects too.
class Naruto : public Hero
{
public:
    CREATE_FUNC(Naruto);

    int expGainPercent() const override;

protected:
    bool onTransformSkillStart() override;
    bool onTransformSkillEnd() override;
    bool setUpClone(Hero* clone, int cloneTime) override;
    bool cloneStartsWithOwnerHP(int cloneTime) const override;
    void finishClone(Hero* clone, int cloneTime) override;
    bool clonesExpire() const override;
    int cloneDamageSharePercent() const override;
    bool targetsHeroesOnly() const override;
    void cloneAI() override;

private:
    // None: an object of this class under another name (Rikudo's Kurama clone)
    enum class Form
    {
        None,
        Base,
        Sage,
        Rikudo,
    };
    Form form() const;

    // clone AI
    CCPoint vectorToTarget() const;
    bool canAct() const;
    void chaseAndAttack(const CCPoint& toTarget);
    void fightHero(const CCPoint& toTarget);
};
