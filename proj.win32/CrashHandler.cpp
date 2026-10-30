#include "CrashHandler.h"

#include <windows.h>
#include <dbghelp.h>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <string>

#pragma comment(lib, "dbghelp.lib")

static std::wstring exeDir()
{
    wchar_t path[MAX_PATH];
    GetModuleFileNameW(nullptr, path, MAX_PATH);
    std::wstring dir(path);
    return dir.substr(0, dir.find_last_of(L"\\/") + 1);
}

static void writeMiniDump(EXCEPTION_POINTERS* info)
{
    std::wstring path = exeDir() + L"crash.dmp";
    HANDLE file = CreateFileW(path.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (file == INVALID_HANDLE_VALUE)
        return;
    MINIDUMP_EXCEPTION_INFORMATION mei{GetCurrentThreadId(), info, FALSE};
    MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), file, MiniDumpNormal, &mei, nullptr, nullptr);
    CloseHandle(file);
}

static void writeStackTrace(FILE* out, CONTEXT* ctx)
{
    HANDLE process = GetCurrentProcess();
    HANDLE thread  = GetCurrentThread();
    SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS);
    SymInitialize(process, nullptr, TRUE);

    CONTEXT context = *ctx;
    STACKFRAME64 frame{};
    frame.AddrPC.Offset    = context.Rip;
    frame.AddrPC.Mode      = AddrModeFlat;
    frame.AddrFrame.Offset = context.Rbp;
    frame.AddrFrame.Mode   = AddrModeFlat;
    frame.AddrStack.Offset = context.Rsp;
    frame.AddrStack.Mode   = AddrModeFlat;

    alignas(SYMBOL_INFO) char symbolBuffer[sizeof(SYMBOL_INFO) + 512];
    auto symbol          = reinterpret_cast<SYMBOL_INFO*>(symbolBuffer);
    symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
    symbol->MaxNameLen   = 511;

    for (int i = 0; i < 64; ++i)
    {
        if (!StackWalk64(IMAGE_FILE_MACHINE_AMD64, process, thread, &frame, &context, nullptr,
                         SymFunctionTableAccess64, SymGetModuleBase64, nullptr) ||
            frame.AddrPC.Offset == 0)
            break;

        DWORD64 addr = frame.AddrPC.Offset;
        char module[MAX_PATH] = "?";
        if (DWORD64 base = SymGetModuleBase64(process, addr))
            GetModuleFileNameA((HMODULE)base, module, MAX_PATH);
        const char* moduleName = strrchr(module, '\\') ? strrchr(module, '\\') + 1 : module;

        DWORD64 displacement = 0;
        const char* name     = SymFromAddr(process, addr, &displacement, symbol) ? symbol->Name : "?";

        IMAGEHLP_LINE64 line{};
        line.SizeOfStruct = sizeof(line);
        DWORD lineDisp    = 0;
        if (SymGetLineFromAddr64(process, addr, &lineDisp, &line))
            fprintf(out, "#%02d %s!%s  %s(%lu)\n", i, moduleName, name, line.FileName, line.LineNumber);
        else
            fprintf(out, "#%02d %s!%s +0x%llx\n", i, moduleName, name, (unsigned long long)displacement);
    }
    SymCleanup(process);
}

static LONG WINAPI onUnhandledException(EXCEPTION_POINTERS* info)
{
    std::wstring path = exeDir() + L"crash.log";
    FILE* out         = _wfopen(path.c_str(), L"a");
    if (out)
    {
        time_t now = time(nullptr);
        char stamp[64];
        strftime(stamp, sizeof(stamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
        auto rec = info->ExceptionRecord;
        fprintf(out, "==== crash %s  code=0x%08lX  address=%p\n", stamp, rec->ExceptionCode, rec->ExceptionAddress);
        if (rec->ExceptionCode == EXCEPTION_ACCESS_VIOLATION && rec->NumberParameters >= 2)
            fprintf(out, "access violation %s address 0x%llx\n", rec->ExceptionInformation[0] ? "writing" : "reading",
                    (unsigned long long)rec->ExceptionInformation[1]);
        writeStackTrace(out, info->ContextRecord);
        fprintf(out, "\n");
        fclose(out);
    }
    writeMiniDump(info);
    return EXCEPTION_CONTINUE_SEARCH;
}

// abort()/assert/std::terminate do not raise an SEH exception; turn them into one
// so they are logged by the same filter.
static void onAbortSignal(int)
{
    RaiseException(0xE0000ABE, EXCEPTION_NONCONTINUABLE, 0, nullptr);
}

void installCrashHandler()
{
    SetUnhandledExceptionFilter(onUnhandledException);
    signal(SIGABRT, onAbortSignal);
    _set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);
}
