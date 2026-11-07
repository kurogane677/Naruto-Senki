#pragma once
#include "Units/UnitFactory.h"

class Hero;

// Creates the unit object for a hero or a summon: its own class when it has one,
// otherwise a plain Hero. The caller still sets it up with setID.
namespace HeroFactory
{
// a hero's forms share their hero's class
const UnitClasses<Hero>& heroClasses();

Hero* create(CharacterId character);
}  // namespace HeroFactory
