#include "Heroes/HeroFactory.h"
#include "Heroes/Characters/Naruto.h"
#include "Heroes/Characters/Sakura.h"

namespace HeroFactory
{

namespace
{
// CREATE_FUNC returns the class's own pointer type; the table holds Hero*
template <class HeroClass>
Hero* make()
{
    return HeroClass::create();
}
}  // namespace

const std::map<CharacterId, Creator>& heroClasses()
{
    static const std::map<CharacterId, Creator> table = {
        {CharacterId::Naruto, &make<Naruto>},
        {CharacterId::SageNaruto, &make<Naruto>},
        {CharacterId::RikudoNaruto, &make<Naruto>},
        {CharacterId::Sakura, &make<Sakura>},
    };
    return table;
}

Hero* create(CharacterId character)
{
    auto found = heroClasses().find(character);
    return found != heroClasses().end() ? found->second() : Hero::create();
}

}  // namespace HeroFactory
