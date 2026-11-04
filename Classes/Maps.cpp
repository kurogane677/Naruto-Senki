#include "Maps.h"
#include "Compat/Compat2x.h"
#include <algorithm>

namespace maps
{

namespace
{

bool exists(const std::string& path)
{
    return ax::FileUtils::getInstance()->isFileExist(path);
}

std::string withFallback(const std::string& path, const std::string& fallback)
{
    return exists(path) ? path : fallback;
}

}  // namespace

int count()
{
    static int n = 0;
    if (!n)
    {
        while (n < 99 && exists(tmx(n + 1)))
            n++;
        n = std::max(n, 1);
    }
    return n;
}

std::string tmx(int n)
{
    return "Tiles/" + std::to_string(n) + ".tmx";
}

std::string background(int n)
{
    return withFallback("Tiles/map_bg" + std::to_string(n) + ".png", "Tiles/map_bg1.png");
}

std::string towerSheet(int n)
{
    if (n == 1)
        return "Element/Tower/Tower.plist";
    return withFallback("Element/Tower/Tower" + std::to_string(n) + ".plist", "Element/Tower/Tower.plist");
}

std::string music(int n, int part)
{
    auto track = [](int k) { return "Audio/Music/Battle" + std::to_string(k) + ".mp3"; };
    return withFallback(track(part + 3 * (n - 1)), withFallback(track(part), track(1)));
}

}  // namespace maps
