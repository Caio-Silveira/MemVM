#pragma once
#include "Mmf_Types.h"

namespace MemVM::IO {
        inline Status GetStatus(const Channel& channel) noexcept {
            return channel.status;
        }

        inline void SetStatus(Channel& channel, Status s) noexcept {
            channel.status = s;
        }

        inline void Reset(Channel& channel) noexcept {
            channel.status = Status::IDLE;
            channel.command = Command::NONE;
            channel.error = Error::NONE;
            channel.payloadSize = 0;
        }
}