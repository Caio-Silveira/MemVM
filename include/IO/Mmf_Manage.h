#pragma once
#include "Mmf_Types.h"
#include "Mmf_Utils.h"

namespace MemVM::IO{

    inline void InitializeBlock(SharedMemoryBlock* block) noexcept {
        block->serverAlive = false;
        block->shutdown = false;
        
        for (uint8_t i = 0; i < MAX_CHANNELS; ++i) {
            Reset(block->channels[i]);
            block->channels[i].channelId = i;
        }
    }

    inline uint8_t FindFreeChannel(const SharedMemoryBlock* block) noexcept {
        for (uint8_t i = 0; i < MAX_CHANNELS; ++i)
            if (GetStatus(block->channels[i]) == Status::IDLE)
                return i;
                
        return 255;
    }
}