#pragma once
#include "Heroes/CharacterId.h"
#include <map>

class Hero;

// Creates the unit object for a character: its own hero class when it has one,
// otherwise a plain Hero. The caller still sets it up with setID.
namespace HeroFactory
{
using Creator = Hero* (*)();

// characters with their own hero class; a hero's forms share their hero's class
const std::map<CharacterId, Creator>& heroClasses();

Hero* create(CharacterId character);
}  // namespace HeroFactory
