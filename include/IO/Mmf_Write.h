#pragma once
#include "Mmf_Types.h"
#include "Mmf_Utils.h"

namespace MemVM::IO {

    inline void WriteSuccess(Channel& channel) noexcept {
        channel.command = Command::NONE;
        channel.error = Error::NONE;
        SetStatus(channel, Status::SUCCESS);
    }

    inline void WriteError(Channel& channel, Error err) noexcept {
        channel.command = Command::NONE;
        channel.error = err;
        channel.payloadSize = 0;
        SetStatus(channel, Status::FAILED);
    }

    inline bool Write(Channel& channel, const uint8_t* data, size_t size) noexcept {
        if (size > MAX_PAYLOAD) {
            WriteError(channel, Error::INVALID_SIZE);
            return false;
        }
        
        for (size_t i = 0; i < size; ++i) {
            channel.payload[i] = data[i];
        }
        
        channel.payloadSize = static_cast<uint16_t>(size);
        
        WriteSuccess(channel);
        
        return true;
    }
}