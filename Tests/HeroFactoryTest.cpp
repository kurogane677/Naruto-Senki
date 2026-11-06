#include "doctest.h"
#include "Heroes/Characters/Naruto.h"

// HeroFactory picks Naruto's class by Naruto::isForm. Creating the objects needs a
// renderer, so the factory itself is covered by the soak, not here.
TEST_CASE("Naruto's three forms are one hero class")
{
    CHECK(Naruto::isForm("Naruto"));
    CHECK(Naruto::isForm("SageNaruto"));
    CHECK(Naruto::isForm("RikudoNaruto"));
    CHECK_FALSE(Naruto::isForm("Kurama"));
    CHECK_FALSE(Naruto::isForm("naruto"));
}
