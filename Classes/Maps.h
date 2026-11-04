#pragma once
#include <string>

// Battle maps are found by file name, so a mod adds one by dropping files in:
//   Tiles/<n>.tmx                 required; maps are numbered 1, 2, 3, ... without gaps
//   Tiles/map_bg<n>.png           background strip
//   Element/Tower/Tower<n>.plist  tower sprites (map 1 uses Tower.plist)
//   Audio/Music/Battle<k>.mp3     HardCore music, k = part + 3 * (n - 1), part 1..3
// Missing extras fall back to map 1's, and missing music to Battle1.mp3.
namespace maps
{

int count();  // maps 1..count() exist
std::string tmx(int n);
std::string background(int n);
std::string towerSheet(int n);
std::string music(int n, int part);

}  // namespace maps
