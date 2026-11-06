#pragma once
#include <string>

class Hero;

// Creates the unit object for a character: its own hero class when it has one,
// otherwise a plain Hero. The caller still sets it up with setID.
namespace HeroFactory
{
Hero* create(const std::string& character);
}
