#include "doctest.h"
#include "Types.h"

TEST_CASE("roleFromString maps every XML role name")
{
    CHECK(roleFromString("Player") == Role::Player);
    CHECK(roleFromString("Com") == Role::Com);
    CHECK(roleFromString("Clone") == Role::Clone);
    CHECK(roleFromString("Summon") == Role::Summon);
    CHECK(roleFromString("Flog") == Role::Flog);
    CHECK(roleFromString("Tower") == Role::Tower);
    CHECK(roleFromString("Mon") == Role::Mon);
    CHECK(roleFromString("Kuilei") == Role::Kuilei);
    CHECK(roleFromString("Bullet") == Role::Bullet);
}

TEST_CASE("roleFromString rejects unknown, null and wrongly cased names")
{
    CHECK(roleFromString(nullptr) == Role::None);
    CHECK(roleFromString("") == Role::None);
    CHECK(roleFromString("player") == Role::None);
    CHECK(roleFromString("Hero") == Role::None);
}

TEST_CASE("groupFromString")
{
    CHECK(groupFromString("Konoha") == Group::Konoha);
    CHECK(groupFromString("Akatsuki") == Group::Akatsuki);
    CHECK(groupFromString("konoha") == Group::None);
    CHECK(groupFromString(nullptr) == Group::None);
}
