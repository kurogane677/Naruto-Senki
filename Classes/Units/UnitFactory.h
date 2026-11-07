#pragma once
#include "Heroes/CharacterId.h"
#include <map>

// A table of the classes a kind of unit (Hero, Monster, ...) can be created as:
// characters with their own class map to it, every other character is a plain Base.
template <class Base>
class UnitClasses
{
public:
    using Creator = Base* (*)();

    UnitClasses(std::map<CharacterId, Creator> classes) : _classes(std::move(classes)) {}

    Base* create(CharacterId character) const
    {
        auto found = _classes.find(character);
        return found != _classes.end() ? found->second() : Base::create();
    }

    const std::map<CharacterId, Creator>& classes() const { return _classes; }

private:
    std::map<CharacterId, Creator> _classes;
};

// a table entry: CREATE_FUNC returns the class's own pointer type, the table holds Base*
template <class Base, class Unit>
Base* makeUnit()
{
    return Unit::create();
}
