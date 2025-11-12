#pragma once
#include "Vmi_Manage.h"
#include "Vmi_Process.h"

namespace MemVM::Vmi {

    inline bool IsReady() noexcept {
        return g_Hook.captured;
    }

    bool Start(const char* mmfName) noexcept;
    void Stop() noexcept;
}