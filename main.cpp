#include "Vmi/Vmi.h"
#include <windows.h>

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {
    switch (reason) {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(hModule);
            MemVM::Vmi::Start("MemVM_BlueStacks");
            break;

        case DLL_PROCESS_DETACH:
            MemVM::Vmi::Stop();
            break;
    }
    return TRUE;
}