#pragma once
#include "Vmi_Types.h"
#include "Vmi_Hook.h"
#include <windows.h>
#include <MinHook.h>

namespace MemVM::Vmi {

    struct ManageState {
        bool initialized;
        void* hookTargets[2];
    };

    inline ManageState g_Manage;

    static int __cdecl ReadWrapper(void* vm, uintptr_t addr, void* buf, size_t size) {
        return HookedRead(vm, addr, buf, size, Original.read);
    }

    static int __cdecl WriteWrapper(void* vm, uintptr_t addr, void* buf, size_t size) {
        return HookedWrite(vm, addr, buf, size, Original.write);
    }

    inline bool LoadTargets() noexcept {
        HMODULE bstkVMM = GetModuleHandleA("BstkVMM.dll");
        if (!bstkVMM) return false;

        Target.read = (PGMPhysReadType)GetProcAddress(bstkVMM, "PGMPhysRead");
        Target.write = (PGMPhysWriteType)GetProcAddress(bstkVMM, "PGMPhysSimpleWriteGCPhys");
        Target.convert = (PGMPhysGCPtr2GCPhysType)GetProcAddress(bstkVMM, "PGMPhysGCPtr2GCPhys");
        Target.getCpu = (VMMGetCpuByIdType)GetProcAddress(bstkVMM, "VMMGetCpuById");

        return Target.read && Target.write && Target.convert && Target.getCpu;
    }

    inline bool CreateHooks() noexcept {
        if (MH_Initialize() != MH_OK) return false;

        if (MH_CreateHook((void*)Target.read, (void*)ReadWrapper, (void**)&Original.read) != MH_OK)
            return false;

        if (MH_EnableHook((void*)Target.read) != MH_OK)
            return false;

        g_Manage.hookTargets[0] = (void*)Target.read;

        if (MH_CreateHook((void*)Target.write, (void*)WriteWrapper, (void**)&Original.write) != MH_OK)
            return false;

        if (MH_EnableHook((void*)Target.write) != MH_OK)
            return false;

        g_Manage.hookTargets[1] = (void*)Target.write;

        return true;
    }

    inline bool Initialize() noexcept {
        if (g_Manage.initialized) return true;

        if (!LoadTargets()) return false;
        if (!CreateHooks()) return false;
        if (!WaitForCapture(3000)) return false;

        MH_DisableHook(g_Manage.hookTargets[0]);
        MH_DisableHook(g_Manage.hookTargets[1]);

        g_Manage.initialized = true;
        return true;
    }

    inline void Cleanup() noexcept {
        if (!g_Manage.initialized) return;

        MH_Uninitialize();

        Target.read = nullptr;
        Target.write = nullptr;
        Target.convert = nullptr;
        Target.getCpu = nullptr;

        Original.read = nullptr;
        Original.write = nullptr;
        Original.convert = nullptr;
        Original.getCpu = nullptr;

        g_Manage.initialized = false;
    }
}