#include "Heroes/Characters/Naruto.h"

bool Naruto::isForm(const std::string& character)
{
    return character == "Naruto" || character == "SageNaruto" || character == "RikudoNaruto";
}

Naruto::Form Naruto::form() const
{
    if (isCharacter("Naruto"))
        return Form::Base;
    if (isCharacter("SageNaruto"))
        return Form::Sage;
    if (isCharacter("RikudoNaruto"))
        return Form::Rikudo;
    return Form::None;
}

// Base and Sage Naruto get a quarter more EXP from every kill
int Naruto::expGainPercent() const
{
    return form() == Form::Base || form() == Form::Sage ? 125 : Hero::expGainPercent();
}

bool Naruto::onTransformSkillStart()
{
    switch (form())
    {
    case Form::Base:
        setIdleAction(createAnimation(skillSPC1Array, 5.0f, true, false));
        setWalkAction(createAnimation(skillSPC2Array, 10.0f, true, false));
        setNAttackAction(createAnimation(skillSPC3Array, 10.0f, false, true));
        setKnockDownAction(createAnimation(skillSPC4Array, 10.0f, false, true));
        setWalkSpeed(320);
        _originSpeed = 320;
        _isOnlySkillLocked = true;
        settempAttackValue1(_nattackValue);
        setnAttackValue(960);
        _nattackRangeX = _spcattackRangeX3;
        _nattackRangeY = _spcattackRangeY3;
        _tempAttackType = _nattackType;
        _nattackType = _spcattackType3;
        _gardValue += 5000;
        _isBati = true;
        lockOugis();
        return true;
    case Form::Sage:
    case Form::Rikudo:
        _gardValue += 5000;
        _isBati = true;
        isPofang = true;
        _isOnlySkillLocked = true;
        setIdleAction(createAnimation(skillSPC4Array, 5.0f, true, false));
        setWalkAction(createAnimation(skillSPC2Array, 10.0f, true, false));
        setNAttackAction(createAnimation(skillSPC3Array, 10.0f, false, true));
        if (form() == Form::Rikudo)
        {
            settempAttackValue1(_nattackValue);
            setnAttackValue(560);
        }
        setWalkSpeed(320);
        _originSpeed = 320;
        lockOugis();
        return true;
    default:
        return Hero::onTransformSkillStart();
    }
}

bool Naruto::onTransformSkillEnd()
{
    switch (form())
    {
    case Form::Base:
        setIdleAction(createAnimation(idleArray, 5.0f, true, false));
        setWalkAction(createAnimation(walkArray, 10.0f, true, false));
        setNAttackAction(createAnimation(nattackArray, 10.0f, false, true));
        _isOnlySkillLocked = false;
        unlockOugis();
        setWalkSpeed(224);
        _originSpeed = 224;
        if (_tempAttackValue1)
        {
            setnAttackValue(_tempAttackValue1);
            _tempAttackValue1 = 0;
        }
        isPofang = false;
        _nattackRangeX = 16;
        _nattackRangeY = 48;
        _nattackType = _tempAttackType;
        _gardValue -= 5000;
        _isBati = false;
        if (_actionState != ACTION_STATE_DEAD)
        {
            _actionState = ACTION_STATE_WALK;
            knockDown();
        }
        setKnockDownAction(createAnimation(knockDownArray, 10.0f, false, true));
        return true;
    case Form::Sage:
    case Form::Rikudo:
        setIdleAction(createAnimation(idleArray, 5.0f, true, false));
        setWalkAction(createAnimation(walkArray, 10.0f, true, false));
        setNAttackAction(createAnimation(nattackArray, 10.0f, false, true));
        _isOnlySkillLocked = false;
        unlockOugis();
        setWalkSpeed(224);
        _originSpeed = 224;
        _gardValue -= 5000;
        _isBati = false;
        isPofang = false;
        if (_actionState != ACTION_STATE_DEAD)
        {
            _actionState = ACTION_STATE_WALK;
            // at LV6 Sage Naruto falls into his awakening pose
            if (form() == Form::Sage && getLV() == 6)
            {
                setKnockDownAction(createAnimation(skillSPC5Array, 10.0f, false, true));
                knockDown();
                setKnockDownAction(createAnimation(knockDownArray, 10.0f, false, true));
                return true;
            }
            idle();
        }
        if (_tempAttackValue1)
        {
            setnAttackValue(_tempAttackValue1);
            _tempAttackValue1 = 0;
        }
        return true;
    default:
        return Hero::onTransformSkillEnd();
    }
}

// Shadow clones keep Naruto's form; Rikudo's 9th summon is Kurama. Clones are made
// from their owner's name (HeroFactory), so a Naruto's clone is a Naruto object.
bool Naruto::setUpClone(Hero* clone, int cloneTime)
{
    auto* narutoClone = dynamic_cast<Naruto*>(clone);
    if (!narutoClone)
        return Hero::setUpClone(clone, cloneTime);
    switch (form())
    {
    case Form::Base:
        narutoClone->setID(getCharacter(), CCString::create("Clone"), _group);
        return true;
    case Form::Rikudo:
        if (cloneTime == 9)
        {
            narutoClone->setID(CCString::create("Kurama"), CCString::create("Clone"), _group);
            narutoClone->_gardValue += 5000;
            narutoClone->_isBati = true;
            narutoClone->setWalkSpeed(320);
            narutoClone->_originSpeed = 320;
            narutoClone->isPofang = true;
            return true;
        }
        if (cloneTime != 10)
            return Hero::setUpClone(clone, cloneTime);
        [[fallthrough]];
    case Form::Sage:
        narutoClone->setID(getCharacter(), CCString::create("Clone"), _group);
        narutoClone->setSkill1Action(narutoClone->createAnimation(narutoClone->skillSPC1Array, 10.0f, false, true));
        narutoClone->_sattackValue1 = _spcattackValue1;
        narutoClone->_sattackType1 = _spcattackType1;
        narutoClone->_sattackcoldDown1 = _spcattackcoldDown1;
        return true;
    default:
        return Hero::setUpClone(clone, cloneTime);
    }
}

bool Naruto::cloneStartsWithOwnerHP(int cloneTime) const
{
    switch (form())
    {
    case Form::Base:
    case Form::Sage:
        return true;
    case Form::Rikudo:
        return cloneTime == 10;
    default:
        return Hero::cloneStartsWithOwnerHP(cloneTime);
    }
}

void Naruto::finishClone(Hero* clone, int cloneTime)
{
    auto* kurama = dynamic_cast<Naruto*>(clone);
    if (kurama && form() == Form::Rikudo && cloneTime == 9)
    {
        if (kurama->_hpBar)
            kurama->_hpBar->setPositionY(120);
        kurama->setnAttackValue(1060);
        return;
    }
    Hero::finishClone(clone, cloneTime);
}

bool Naruto::clonesExpire() const
{
    return form() != Form::None || Hero::clonesExpire();
}

// A shadow clone hit by an enemy hurts the attacker back
int Naruto::cloneDamageSharePercent() const
{
    switch (form())
    {
    case Form::Base:
        return 15;
    case Form::Sage:
        return 25;
    case Form::Rikudo:
        return 35;
    default:
        return Hero::cloneDamageSharePercent();
    }
}

bool Naruto::targetsHeroesOnly() const
{
    return form() == Form::Sage || form() == Form::Rikudo || Hero::targetsHeroesOnly();
}

CCPoint Naruto::vectorToTarget() const
{
    if (_mainTarget->_originY)
        return ccpSub(ccp(_mainTarget->getPositionX(), _mainTarget->_originY), getPosition());
    return ccpSub(_mainTarget->getPosition(), getPosition());
}

bool Naruto::canAct() const
{
    return getActionState() == ACTION_STATE_IDLE || getActionState() == ACTION_STATE_WALK ||
           getActionState() == ACTION_STATE_ATTACK;
}

void Naruto::chaseAndAttack(const CCPoint& toTarget)
{
    if (abs(toTarget.x) > 32 || abs(toTarget.y) > 32)
    {
        walk(ccpNormalize(toTarget));
    }
    else if (canAct())
    {
        changeSide(toTarget);
        attack(NAttack);
    }
}

// Sage and Rikudo clones: close in, then Rasengan (skill 1) when it is ready
void Naruto::fightHero(const CCPoint& toTarget)
{
    if (abs(toTarget.x) > 128 || abs(toTarget.y) > 16)
    {
        walk(ccpNormalize(toTarget));
        return;
    }
    if ((abs(toTarget.x) > 48 || abs(toTarget.y) > 8) && !_isCanSkill1)
    {
        walk(ccpNormalize(toTarget));
        return;
    }
    if (!canAct())
        return;
    // Sage clones step back first when the target is too close for the Rasengan
    if (form() == Form::Sage && _isCanSkill1 && abs(toTarget.x) < 32)
    {
        stepBack();
        return;
    }
    changeSide(toTarget);
    attack(_isCanSkill1 && _mainTarget->getGP() < 5000 ? SKILL1 : NAttack);
}

void Naruto::cloneAI()
{
    if (form() == Form::None)
    {
        Hero::cloneAI();
        return;
    }
    // base clones go for creeps first, Sage and Rikudo clones for heroes
    const char* searchOrder[3] = {"Hero", "Flog", "Tower"};
    if (form() == Form::Base)
        std::swap(searchOrder[0], searchOrder[1]);
    if (!findEnemy(searchOrder[0], 0) && !findEnemy(searchOrder[1], 0) && !findEnemy(searchOrder[2], 0))
        _mainTarget = NULL;

    if (!_mainTarget)
    {
        stepOn();
        return;
    }
    CCPoint toTarget = vectorToTarget();
    bool targetIsBuilding = _mainTarget->role() == Role::Tower || _mainTarget->role() == Role::Flog;
    if (form() == Form::Base || targetIsBuilding)
        chaseAndAttack(toTarget);
    else
        fightHero(toTarget);
}
