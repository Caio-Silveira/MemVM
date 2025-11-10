#include <windows.h>

void Log(const char* msg)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (!hConsole)
    {
        AllocConsole();
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    }
    DWORD written;
    WriteConsoleA(hConsole, msg, (DWORD)strlen(msg), &written, nullptr);
    WriteConsoleA(hConsole, "\n", 1, &written, nullptr);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        Log("DLL injected successfully!");
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
