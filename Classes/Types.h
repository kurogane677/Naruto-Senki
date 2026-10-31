#pragma once
#include <cstring>

// Unit roles; the XML/scene code still passes them around as strings
// ("Player", "Com", ...), ActionManager::setRole maps them to this enum.
enum class Role
{
    None,
    Player,
    Com,
    Clone,
    Summon,
    Flog,
    Tower,
    Mon,
    Kuilei,
    Bullet,
};

enum class Group
{
    None,
    Konoha,
    Akatsuki,
};

inline Role roleFromString(const char* s)
{
    static const struct { const char* name; Role role; } table[] = {
        {"Player", Role::Player}, {"Com", Role::Com},       {"Clone", Role::Clone},
        {"Summon", Role::Summon}, {"Flog", Role::Flog},     {"Tower", Role::Tower},
        {"Mon", Role::Mon},       {"Kuilei", Role::Kuilei}, {"Bullet", Role::Bullet},
    };
    if (s)
        for (const auto& e : table)
            if (strcmp(s, e.name) == 0)
                return e.role;
    return Role::None;
}

inline Group groupFromString(const char* s)
{
    if (s && strcmp(s, "Konoha") == 0)
        return Group::Konoha;
    if (s && strcmp(s, "Akatsuki") == 0)
        return Group::Akatsuki;
    return Group::None;
}
