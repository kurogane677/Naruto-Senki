#include "Heroes/Characters/Naruto.h"

namespace
{
constexpr float kKuramaLifetime = 9;  // seconds, whatever the setClone value says
}  // namespace

// HeroFactory gives this class to Naruto's three forms only
Naruto::Form Naruto::form() const
{
    if (isCharacter(CharacterId::Naruto))
        return Form::Base;
    if (isCharacter(CharacterId::SageNaruto))
        return Form::Sage;
    return Form::Rikudo;
}

// Rikudo's skill05 (ougi 2) summons Kurama
bool Naruto::isKuramaSummon() const
{
    return form() == Form::Rikudo && _actionState == ACTION_STATE_O2ATTACK;
}

// Base and Sage Naruto get a quarter more EXP from every kill
int Naruto::expGainPercent() const
{
    return form() == Form::Base || form() == Form::Sage ? 125 : Hero::expGainPercent();
}

bool Naruto::onTransformSkillStart()
{
    if (form() == Form::Base)
    {
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
    }

    // Sage and Rikudo
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
}

bool Naruto::onTransformSkillEnd()
{
    if (form() == Form::Base)
    {
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
    }

    // Sage and Rikudo
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
}

// Kurama is a plain Hero; every other clone is a shadow clone, a Naruto object of the same form
CharacterId Naruto::cloneCharacter() const
{
    return isKuramaSummon() ? CharacterId::Kurama : characterId();
}

bool Naruto::setUpClone(Hero* clone)
{
    if (isKuramaSummon())
    {
        clone->setID(CCString::create("Kurama"), CCString::create("Clone"), _group);
        clone->_isBati = true;
        clone->setWalkSpeed(320);
        clone->_originSpeed = 320;
        clone->isPofang = true;
        return true;
    }
    auto* shadowClone = dynamic_cast<Naruto*>(clone);
    if (!shadowClone)
        return Hero::setUpClone(clone);

    shadowClone->setID(getCharacter(), CCString::create("Clone"), _group);
    if (form() != Form::Base)
    {
        shadowClone->setSkill1Action(shadowClone->createAnimation(shadowClone->skillSPC1Array, 10.0f, false, true));
        shadowClone->_sattackValue1 = _spcattackValue1;
        shadowClone->_sattackType1 = _spcattackType1;
        shadowClone->_sattackcoldDown1 = _spcattackcoldDown1;
    }
    return true;
}

bool Naruto::cloneStartsWithOwnerHP() const
{
    return !isKuramaSummon();
}

void Naruto::finishClone(Hero* clone)
{
    if (isKuramaSummon())
    {
        if (clone->_hpBar)
            clone->_hpBar->setPositionY(120);
        clone->setnAttackValue(1060);
        return;
    }
    Hero::finishClone(clone);
}

float Naruto::cloneLifetime(int cloneTime) const
{
    return isKuramaSummon() ? kKuramaLifetime : Hero::cloneLifetime(cloneTime);
}

bool Naruto::clonesExpire() const
{
    return true;
}

// A shadow clone hit by an enemy hurts the attacker back
int Naruto::cloneDamageSharePercent() const
{
    if (form() == Form::Base)
        return 15;
    if (form() == Form::Sage)
        return 25;
    return 35;
}

bool Naruto::targetsHeroesOnly() const
{
    return form() != Form::Base;
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
