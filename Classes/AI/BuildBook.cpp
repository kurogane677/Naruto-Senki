#include "AI/BuildBook.h"
#include "AI/AIController.h"
#include "Characters.h"
#include "MyUtils/sqlite3.h"

namespace ai
{

namespace
{

// gear ids: 0 speed/stealth, 1 +CKR, 2 CKR on attack, 3 trap, 4 attack+pierce,
// 5 cooldown+crit, 6 substitution, 7 potion+, 8 HP+armor
const std::map<std::string, std::vector<int>>& roleBuilds(HeroRole role)
{
    static const std::map<HeroRole, std::map<std::string, std::vector<int>>> table = {
        {HeroRole::Fighter, {{"aggressive", {4, 5, 2}}, {"sustain", {7, 8, 1}}}},
        {HeroRole::Assassin, {{"aggressive", {0, 5, 4}}, {"sustain", {6, 8, 7}}}},
        {HeroRole::Tank, {{"aggressive", {8, 4, 2}}, {"sustain", {8, 6, 7}}}},
        {HeroRole::Support, {{"aggressive", {5, 1, 2}}, {"sustain", {7, 1, 8}}}},
        {HeroRole::Pusher, {{"aggressive", {4, 0, 8}}, {"push", {4, 0, 8}}, {"sustain", {7, 8, 0}}}},
        {HeroRole::Ranged, {{"aggressive", {5, 1, 2}}, {"sustain", {6, 7, 8}}}},
        {HeroRole::Summoner, {{"aggressive", {5, 7, 4}}, {"sustain", {7, 8, 6}}}},
    };
    return table.at(role);
}

const std::vector<int>* lookup(const Profile& p, const std::string& name)
{
    auto it = p.builds.find(name);
    if (it != p.builds.end())
        return &it->second;
    auto& rb = roleBuilds(p.role);
    auto r = rb.find(name);
    return r != rb.end() ? &r->second : nullptr;
}

sqlite3* openDb()
{
    sqlite3* db = nullptr;
    std::string path = CCFileUtils::sharedFileUtils()->getWritablePath() + "sql.db";
    if (sqlite3_open(path.c_str(), &db) != SQLITE_OK)
    {
        sqlite3_close(db);
        return nullptr;
    }
    sqlite3_exec(db,
                 "create table if not exists PlayerBuild (hero text, gear1 int, gear2 int, gear3 int,"
                 " games int default 0, wins int default 0, primary key(hero, gear1, gear2, gear3))",
                 nullptr, nullptr, nullptr);
    return db;
}

}  // namespace

std::vector<int> chooseBuild(AIController& ai)
{
    const Profile& p = *ai.profile();
    Hero* h = ai.hero();

    float copyChance = params((Difficulty)h->getDelegate()->aiDifficulty).playerBuildChance;
    if (copyChance > 0 && randChance(copyChance))
    {
        std::vector<int> mine = bestPlayerBuild(p.name);
        if (!mine.empty())
        {
            CCLOG("AI %s copies the player's build", p.name.c_str());
            return mine;
        }
    }

    std::string want = "default";
    if (h->battleCondiction < 0)
        want = "sustain";
    else if (h->battleCondiction > 0)
        want = p.role == HeroRole::Pusher ? "push" : "aggressive";
    else if (randChance(0.35f))
        want = randChance(0.5f) ? "aggressive" : "sustain";

    if (const std::vector<int>* b = lookup(p, want))
        return *b;
    if (const std::vector<int>* b = lookup(p, "default"))
        return *b;
    return roleBuilds(p.role).begin()->second;
}

void recordPlayerBuild(const std::string& hero, const std::vector<int>& gears, bool won)
{
    if (gears.size() != 3)
        return;  // only complete builds are worth copying
    sqlite3* db = openDb();
    if (!db)
        return;
    // the vendored SQLite (3.8) predates UPSERT: insert the row, then count
    const char* sqls[] = {
        "insert or ignore into PlayerBuild (hero, gear1, gear2, gear3, games, wins) values (?1, ?2, ?3, ?4, 0, 0)",
        "update PlayerBuild set games = games + 1, wins = wins + ?5"
        " where hero = ?1 and gear1 = ?2 and gear2 = ?3 and gear3 = ?4",
    };
    for (const char* sql : sqls)
    {
        sqlite3_stmt* st = nullptr;
        if (sqlite3_prepare_v2(db, sql, -1, &st, nullptr) == SQLITE_OK)
        {
            sqlite3_bind_text(st, 1, hero.c_str(), -1, SQLITE_TRANSIENT);
            for (int i = 0; i < 3; i++)
                sqlite3_bind_int(st, 2 + i, gears[i]);
            if (sqlite3_bind_parameter_count(st) >= 5)
                sqlite3_bind_int(st, 5, won ? 1 : 0);
            sqlite3_step(st);
        }
        sqlite3_finalize(st);
    }
    sqlite3_close(db);
}

std::vector<int> bestPlayerBuild(const std::string& hero)
{
    std::vector<int> best;
    sqlite3* db = openDb();
    if (!db)
        return best;
    sqlite3_stmt* st = nullptr;
    const char* sql =
        "select gear1, gear2, gear3 from PlayerBuild where hero = ? and games >= 3"
        " order by cast(wins as real) / games desc, games desc limit 1";
    if (sqlite3_prepare_v2(db, sql, -1, &st, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_text(st, 1, hero.c_str(), -1, SQLITE_TRANSIENT);
        if (sqlite3_step(st) == SQLITE_ROW)
            for (int i = 0; i < 3; i++)
                best.push_back(sqlite3_column_int(st, i));
    }
    sqlite3_finalize(st);
    sqlite3_close(db);
    return best;
}

}  // namespace ai
