#pragma once
#include "Vmi_Types.h"
#include "Vmi_Hook.h"
#include "Mmf_Types.h"

namespace MemVM::Vmi {

    inline bool ProcessRead(const IO::Request& req, uint8_t* output, size_t maxSize) noexcept {
        if (req.size > maxSize) return false;
        
        void* vm = GetVMInstance();
        if (!vm) return false;

        return Original.read(vm, req.address, output, req.size) == 0;
    }

    inline bool ProcessWrite(const IO::Request& req, const uint8_t* data) noexcept {
        void* vm = GetVMInstance();
        if (!vm) return false;

        return Original.write(vm, req.address, (void*)data, req.size) == 0;
    }

    inline bool ProcessConvert(uintptr_t virtAddr, uintptr_t* physAddr) noexcept {
        void* vm = GetVMInstance();
        if (!vm) return false;

        return Original.convert(vm, virtAddr, physAddr) == 0;
    }
}