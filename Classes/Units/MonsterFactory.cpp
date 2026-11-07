#include "Units/MonsterFactory.h"
#include "Units/Monsters/Bikyu.h"
#include "Units/Monsters/FudonSRK.h"

namespace MonsterFactory
{

const UnitClasses<Monster>& monsterClasses()
{
    static const UnitClasses<Monster> table({
        {CharacterId::Bikyu, &makeUnit<Monster, Bikyu>},
        {CharacterId::FudonSRK, &makeUnit<Monster, FudonSRK>},
        {CharacterId::FudonSRK2, &makeUnit<Monster, FudonSRK>},
    });
    return table;
}

Monster* create(CharacterId character)
{
    return monsterClasses().create(character);
}

}  // namespace MonsterFactory
