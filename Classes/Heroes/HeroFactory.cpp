#include "Heroes/HeroFactory.h"
#include "Heroes/Characters/Naruto.h"

namespace HeroFactory
{

Hero* create(const std::string& character)
{
    if (Naruto::isForm(character))
        return Naruto::create();
    return Hero::create();
}

}  // namespace HeroFactory
