#include "doctest.h"
#include "AI/BuildBook.h"
#include "Compat/Compat2x.h"
#include <filesystem>

namespace
{
// a throwaway writable folder, so the tests never touch the player's sql.db
struct TempDataDir
{
    std::filesystem::path path;
    std::string previous;
    TempDataDir()
    {
        path = std::filesystem::temp_directory_path() / "NarutoSenki-test-buildbook";
        std::filesystem::remove_all(path);
        std::filesystem::create_directories(path);
        previous = CCFileUtils::sharedFileUtils()->getWritablePath();
        CCFileUtils::sharedFileUtils()->setWritablePath(path.generic_string() + "/");
    }
    ~TempDataDir()
    {
        CCFileUtils::sharedFileUtils()->setWritablePath(previous);
        std::error_code ignored;
        std::filesystem::remove_all(path, ignored);
    }
};
}  // namespace

TEST_CASE("a player build is only copied after 3 games")
{
    TempDataDir dataDir;
    ai::recordPlayerBuild("Naruto", {4, 5, 2}, true);
    ai::recordPlayerBuild("Naruto", {4, 5, 2}, false);
    CHECK(ai::bestPlayerBuild("Naruto").empty());
    ai::recordPlayerBuild("Naruto", {4, 5, 2}, true);
    CHECK(ai::bestPlayerBuild("Naruto") == std::vector<int>{4, 5, 2});
}

TEST_CASE("the build with the best win rate wins")
{
    TempDataDir dataDir;
    for (int game = 0; game < 4; game++)
        ai::recordPlayerBuild("Sasuke", {0, 5, 4}, game == 0);  // 1 of 4
    for (int game = 0; game < 3; game++)
        ai::recordPlayerBuild("Sasuke", {7, 8, 11}, game < 2);  // 2 of 3
    CHECK(ai::bestPlayerBuild("Sasuke") == std::vector<int>{7, 8, 11});
    CHECK(ai::bestPlayerBuild("Naruto").empty());
}

TEST_CASE("incomplete builds are not recorded")
{
    TempDataDir dataDir;
    for (int game = 0; game < 3; game++)
        ai::recordPlayerBuild("Lee", {4, 5}, true);
    CHECK(ai::bestPlayerBuild("Lee").empty());
}
