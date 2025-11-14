#pragma once
#include <cstdint>
#include <cstddef>

namespace MemVM::Vmi {

    using PGMPhysReadType = int(__cdecl*)(void*, uintptr_t, void*, size_t);
    using PGMPhysWriteType = int(__cdecl*)(void*, uintptr_t, void*, size_t);
    using PGMPhysGCPtr2GCPhysType = int(__cdecl*)(void*, uintptr_t, uintptr_t*);
    using VMMGetCpuByIdType = void*(__cdecl*)(void*, int);

    struct FunctionPointers {
        PGMPhysReadType read;
        PGMPhysWriteType write;
        PGMPhysGCPtr2GCPhysType convert;
        VMMGetCpuByIdType getCpu;
    };

    inline FunctionPointers Target;

    inline FunctionPointers Original;

}