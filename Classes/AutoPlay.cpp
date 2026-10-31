#include "AutoPlay.h"
#include "GameLayer.h"
#include "Characters.h"
#include "AI/AIController.h"
#include "SelectLayer.h"
#include "MyUtils/KTools.h"
#include <cstdio>
#include <cstdlib>

#if defined(_WIN32)
#include <windows.h>
#include <dbghelp.h>
#pragma comment(lib, "dbghelp.lib")

// Unattended runs have no debugger attached: write the crashing stack to
// <writable>/autoplay_crash.log.
static LONG WINAPI crashHandler(EXCEPTION_POINTERS* info)
{
    std::string path = CCFileUtils::sharedFileUtils()->getWritablePath() + "autoplay_crash.log";
    FILE* f = fopen(path.c_str(), "a");
    if (!f)
        return EXCEPTION_CONTINUE_SEARCH;
    fprintf(f, "exception 0x%08lx at %p\n", info->ExceptionRecord->ExceptionCode,
            info->ExceptionRecord->ExceptionAddress);
    HANDLE process = GetCurrentProcess();
    SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_UNDNAME);
    SymInitialize(process, nullptr, TRUE);
    CONTEXT ctx = *info->ContextRecord;
    STACKFRAME64 frame = {};
    frame.AddrPC.Offset = ctx.Rip;
    frame.AddrFrame.Offset = ctx.Rbp;
    frame.AddrStack.Offset = ctx.Rsp;
    frame.AddrPC.Mode = frame.AddrFrame.Mode = frame.AddrStack.Mode = AddrModeFlat;
    for (int i = 0; i < 40 && StackWalk64(IMAGE_FILE_MACHINE_AMD64, process, GetCurrentThread(), &frame, &ctx,
                                          nullptr, SymFunctionTableAccess64, SymGetModuleBase64, nullptr);
         i++)
    {
        char buf[sizeof(SYMBOL_INFO) + 256] = {};
        SYMBOL_INFO* sym = (SYMBOL_INFO*)buf;
        sym->SizeOfStruct = sizeof(SYMBOL_INFO);
        sym->MaxNameLen = 255;
        DWORD64 disp = 0;
        IMAGEHLP_LINE64 line = {sizeof(line)};
        DWORD lineDisp = 0;
        const char* name = SymFromAddr(process, frame.AddrPC.Offset, &disp, sym) ? sym->Name : "?";
        if (SymGetLineFromAddr64(process, frame.AddrPC.Offset, &lineDisp, &line))
            fprintf(f, "  %s  %s:%lu\n", name, line.FileName, line.LineNumber);
        else
            fprintf(f, "  %s\n", name);
    }
    fclose(f);
    return EXCEPTION_CONTINUE_SEARCH;
}
#endif

namespace AutoPlay
{

bool enabled()
{
    static const bool on = [] {
        const char* v = getenv("NS_AUTOPLAY");
        bool active = v && *v && strcmp(v, "0") != 0;
#if defined(_WIN32)
        if (active)
            SetUnhandledExceptionFilter(crashHandler);
#endif
        return active;
    }();
    return on;
}

float speed()
{
    const char* v = getenv("NS_AUTOPLAY_SPEED");
    float s = v ? (float)atof(v) : 3.0f;
    return s > 0 ? s : 3.0f;
}

void startMatch()
{
    static bool dbReady = false;
    if (!dbReady)
    {
        KTools* tool = KTools::create();
        tool->initTableInDB();
        tool->initColumeInDB();
        dbReady = true;
    }

    auto* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    for (const char* plist : {"Menu.plist", "Result.plist", "NamePlate.plist", "Select.plist", "UI.plist",
                              "Report.plist", "Ougis.plist", "Ougis2.plist", "Map.plist", "Gears.plist"})
        cache->addSpriteFramesWithFile(plist);

    // the Training roster (SelectLayer's selectable heroes)
    static const char* heroes[] = {"Naruto", "Sakura", "Sai", "Kakashi", "Asuma", "Shikamaru", "Choji", "Ino",
                                   "Hinata", "Neji", "Kiba", "Shino", "Tenten", "Lee", "Tsunade", "Jiraiya",
                                   "Minato", "Hiruzen", "Tobirama", "Gaara", "Kankuro", "Chiyo", "Itachi",
                                   "Kisame", "Deidara", "Konan", "Tobi", "Hidan", "Kakuzu", "Sasuke",
                                   "Suigetsu", "Karin", "Jugo"};
    static std::string chosen;
    const char* pick = getenv("NS_AUTOPLAY_HERO");
    chosen = pick && *pick ? pick : heroes[randInt(sizeof(heroes) / sizeof(heroes[0]))];

    SelectLayer* select = SelectLayer::create();
    select->_selectHero = chosen.c_str();
    const char* hc = getenv("NS_AUTOPLAY_HARDCORE");
    select->_isHardCoreMode = hc && *hc && strcmp(hc, "0") != 0;
    select->onGameStart(nullptr);
}

void report(GameLayer* game, bool playerWon)
{
    std::string line = CCString::createWithFormat("%02u:%02u winner=%s player=%s |", game->_minute, game->_second,
                                                   playerWon ? "player" : "enemy",
                                                   game->currentPlayer->getCharacter()->getCString())
                           ->getCString();
    CCObject* obj;
    CCARRAY_FOREACH(game->_CharacterArray, obj)
    {
        ActionManager* unit = (ActionManager*)obj;
        if (unit->role() != Role::Player && unit->role() != Role::Com)
            continue;
        line += CCString::createWithFormat(" %s%s:%s LV%u K%d D%u",
                                           unit->group() == Group::Konoha ? "K" : "A",
                                           unit->role() == Role::Player ? "*" : "",
                                           unit->getCharacter()->getCString(), unit->getLV(), unit->getKillNum(),
                                           unit->_deadNum)
                    ->getCString();
        if (unit->role() == Role::Com || unit->role() == Role::Player)
        {
            Hero* hero = (Hero*)unit;
            if (hero->brain())
                line += " " + hero->brain()->activitySummary();
        }
        if (game->_isHardCoreGame)
        {
            line += " gear[";
            CCObject* g;
            CCARRAY_FOREACH(unit->getGearArray(), g)
            {
                line += ((CCString*)g)->getCString();
            }
            line += "]";
        }
    }
    CCLOG("[autoplay] %s", line.c_str());
    std::string path = CCFileUtils::sharedFileUtils()->getWritablePath() + "autoplay.log";
    if (FILE* f = fopen(path.c_str(), "a"))
    {
        fprintf(f, "%s\n", line.c_str());
        fclose(f);
    }
}

}  // namespace AutoPlay
