#pragma once
#include "Mmf_Types.h"

namespace MemVM::IO {

    inline Request Read(const Channel& channel) noexcept {
        Request req;
        
        for (size_t i = 0; i < sizeof(Request); ++i) {
            ((uint8_t*)&req)[i] = channel.payload[i];
        }
        
        return req;
    }
}