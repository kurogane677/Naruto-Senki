#include "Heroes/CharacterId.h"

#include <string_view>
#include <unordered_map>

CharacterId characterIdFromString(const char* name)
{
    static const std::unordered_map<std::string_view, CharacterId> idsByName = {
#define X(character) {#character, CharacterId::character},
        CHARACTER_IDS(X)
#undef X
    };
    if (!name)
        return CharacterId::None;
    auto found = idsByName.find(name);
    return found != idsByName.end() ? found->second : CharacterId::None;
}
