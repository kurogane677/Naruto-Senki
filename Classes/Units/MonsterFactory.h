#pragma once
#include "Units/UnitFactory.h"

class Monster;

// Creates the object for a monster a skill spawns (setMon): its own class when it
// has one, otherwise a plain Monster. The caller still sets it up with setID.
namespace MonsterFactory
{
// monsters that behave the same share a class
const UnitClasses<Monster>& monsterClasses();

Monster* create(CharacterId character);
}  // namespace MonsterFactory
