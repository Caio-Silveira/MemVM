#pragma once
#include "Vmi_Types.h"
#include <windows.h>

namespace MemVM::Vmi {

    struct HookState {
        volatile void* vmInstance;
        volatile bool captured;
        volatile long lock;
    };

    inline HookState g_Hook;

    inline bool TryCapture(void* vmInstance) noexcept {
        #ifdef _MSC_VER
            while (InterlockedExchange(&g_Hook.lock, 1) != 0) {}
        #else
            while (__sync_lock_test_and_set(&g_Hook.lock, 1)) {}
        #endif

        bool result = false;
        if (!g_Hook.captured) {
            g_Hook.vmInstance = vmInstance;
            g_Hook.captured = true;
            result = true;
        }

        #ifdef _MSC_VER
            InterlockedExchange(&g_Hook.lock, 0);
        #else
            __sync_lock_release(&g_Hook.lock);
        #endif

        return result;
    }

    inline int __cdecl HookedRead(void* vmInstance, uintptr_t physAddress, 
                                  void* buffer, size_t bufferSize,
                                  PGMPhysReadType original) noexcept {
        
        if (!g_Hook.captured) {
            TryCapture(vmInstance);
        }
        
        return original(vmInstance, physAddress, buffer, bufferSize);
    }

    inline int __cdecl HookedWrite(void* vmInstance, uintptr_t physAddress, 
                                   void* buffer, size_t bufferSize,
                                   PGMPhysWriteType original) noexcept {
        
        if (!g_Hook.captured) {
            TryCapture(vmInstance);
        }
        
        return original(vmInstance, physAddress, buffer, bufferSize);
    }

    inline bool WaitForCapture(uint32_t timeoutMs) noexcept {
        const uint32_t sleepMs = 10;
        uint32_t elapsed = 0;

        while (!g_Hook.captured && elapsed < timeoutMs) {
            Sleep(sleepMs);
            elapsed += sleepMs;
        }

        return g_Hook.captured;
    }

    inline void* GetVMInstance() noexcept {
        return const_cast<void*>(g_Hook.vmInstance);
    }
}