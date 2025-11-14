#pragma once
#include "Vmi_Types.h"
#include "Vmi_Hook.h"
#include "../io/Mmf_Types.h"

namespace MemVM::Vmi {

    inline bool PtrToPhys(uintptr_t virtualAddress, uintptr_t* physicalAddress) noexcept {
        void* vm = GetVMInstance();
        if (!vm) return false;

        for (uint32_t cpuId = 0; cpuId < 4U; cpuId++) {
            void* cpu = Original.getCpu(vm, cpuId);
            if (!cpu) continue;

            uintptr_t tempPhys = 0;
            int status = Original.convert(cpu, virtualAddress, &tempPhys);

            if (status != 0) continue;

            *physicalAddress = tempPhys;
            return true;
        }

        return false;
    }

    inline bool ProcessRead(const IO::Request& req, uint8_t* output, size_t maxSize) noexcept {
        if (req.size > maxSize) return false;
        
        void* vm = GetVMInstance();
        if (!vm) return false;

        uintptr_t physicalAddress = 0;
        if (!PtrToPhys(req.address, &physicalAddress)) {
            return false;
        }

        return Original.read(vm, physicalAddress, output, req.size) == 0;
    }

    inline bool ProcessWrite(const IO::Request& req, const uint8_t* data) noexcept {
        void* vm = GetVMInstance();
        if (!vm) return false;

        uintptr_t physicalAddress = 0;
        if (!PtrToPhys(req.address, &physicalAddress)) {
            return false;
        }

        return Original.write(vm, physicalAddress, (void*)data, req.size) == 0;
    }

}