#include "Heroes/HeroFactory.h"
#include "Heroes/Characters/Deidara.h"
#include "Heroes/Characters/Naruto.h"
#include "Heroes/Characters/Sai.h"
#include "Heroes/Characters/Sakura.h"

namespace HeroFactory
{

const UnitClasses<Hero>& heroClasses()
{
    static const UnitClasses<Hero> table({
        {CharacterId::Naruto, &makeUnit<Hero, Naruto>},
        {CharacterId::SageNaruto, &makeUnit<Hero, Naruto>},
        {CharacterId::RikudoNaruto, &makeUnit<Hero, Naruto>},
        {CharacterId::Sakura, &makeUnit<Hero, Sakura>},
        {CharacterId::Sai, &makeUnit<Hero, Sai>},
        {CharacterId::Deidara, &makeUnit<Hero, Deidara>},
    });
    return table;
}

Hero* create(CharacterId character)
{
    return heroClasses().create(character);
}

}  // namespace HeroFactory
