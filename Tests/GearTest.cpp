#include "doctest.h"
#include "ActionManager.h"

// Snapshot of the gear table (ActionManager.cpp kGearSpecs) as of v0.0.7-beta: a refactor
// that moves the table must keep every number.

TEST_CASE("gear prices")
{
    const int expectedCost[] = {500, 1000, 1000, 500, 1000, 1000, 500, 1000, 1000, 1000, 1000, 1000};
    for (int gear = gear00; gear < None; gear++)
    {
        CAPTURE(gear);
        CHECK(ActionManager::gearCost(gearType(gear)) == expectedCost[gear]);
    }
    CHECK(ActionManager::gearCost(None) == 0);
}

TEST_CASE("upgrade cost is the price times the current level")
{
    CHECK(ActionManager::gearUpgradeCost(gear01, 1) == 1000);
    CHECK(ActionManager::gearUpgradeCost(gear01, 2) == 2000);
    CHECK(ActionManager::gearUpgradeCost(gear00, 2) == 1000);
}

TEST_CASE("selling refunds half of everything paid")
{
    CHECK(ActionManager::gearSellValue(gear01, 1) == 500);
    CHECK(ActionManager::gearSellValue(gear01, 2) == 1000);
    CHECK(ActionManager::gearSellValue(gear01, 3) == 2000);
    CHECK(ActionManager::gearSellValue(gear00, 3) == 1000);
}

TEST_CASE("gear values per level")
{
    const int expectedValue[][3] = {
        {15, 12, 9},       {25, 40, 60},       {15, 20, 25}, {15, 12, 9},
        {160, 240, 320},   {5, 6, 7},          {15, 12, 9},  {6000, 7500, 9000},
        {3000, 4000, 6000}, {10, 15, 20},      {30, 45, 60}, {70, 65, 50},
    };
    for (int gear = gear00; gear < None; gear++)
        for (int level = 1; level <= 3; level++)
        {
            CAPTURE(gear);
            CAPTURE(level);
            CHECK(ActionManager::gearValue(gearType(gear), level) == expectedValue[gear][level - 1]);
        }
}

TEST_CASE("second gear values")
{
    CHECK(ActionManager::gearValue(gear08, 1, 1) == 25);
    CHECK(ActionManager::gearValue(gear08, 3, 1) == 35);
    CHECK(ActionManager::gearValue(gear11, 1, 1) == 0);
    CHECK(ActionManager::gearValue(gear11, 3, 1) == 15);
    CHECK(ActionManager::gearValue(gear01, 2, 1) == 0);
}

TEST_CASE("gear values outside the table are 0")
{
    CHECK(ActionManager::gearValue(None, 1) == 0);
    CHECK(ActionManager::gearValue(gear01, 0) == 0);
}
