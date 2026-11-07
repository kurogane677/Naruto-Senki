#include "Units/MonsterFactory.h"
#include "Units/Monsters/Bikyu.h"
#include "Units/Monsters/FudonSRK.h"
#include "Units/Monsters/InkDragon.h"
#include "Units/Monsters/RideMonster.h"

namespace MonsterFactory
{

const UnitClasses<Monster>& monsterClasses()
{
    static const UnitClasses<Monster> table({
        {CharacterId::Bikyu, &makeUnit<Monster, Bikyu>},
        {CharacterId::FudonSRK, &makeUnit<Monster, FudonSRK>},
        {CharacterId::FudonSRK2, &makeUnit<Monster, FudonSRK>},
        {CharacterId::InkDragon, &makeUnit<Monster, InkDragon>},
        {CharacterId::Mouse, &makeUnit<Monster, RideMonster>},
        {CharacterId::Spider, &makeUnit<Monster, RideMonster>},
    });
    return table;
}

Monster* create(CharacterId character)
{
    return monsterClasses().create(character);
}

}  // namespace MonsterFactory
