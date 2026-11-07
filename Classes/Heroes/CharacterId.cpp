#include "Heroes/CharacterId.h"

#include <string_view>
#include <unordered_map>

CharacterId characterIdFromString(const char* name)
{
    static const std::unordered_map<std::string_view, CharacterId> ids = {
#define X(id) {#id, CharacterId::id},
        CHARACTER_IDS(X)
#undef X
    };
    if (!name)
        return CharacterId::None;
    auto found = ids.find(name);
    return found != ids.end() ? found->second : CharacterId::None;
}
