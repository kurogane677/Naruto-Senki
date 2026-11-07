#include "Heroes/HeroFactory.h"
#include "Heroes/Characters/Naruto.h"
#include "Heroes/Characters/Sakura.h"

namespace HeroFactory
{

Hero* create(const std::string& character)
{
    if (Naruto::isForm(character))
        return Naruto::create();
    if (characterIdFromString(character.c_str()) == CharacterId::Sakura)
        return Sakura::create();
    return Hero::create();
}

}  // namespace HeroFactory
