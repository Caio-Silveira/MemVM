#pragma once
#include <cstdint>

#ifdef ERROR
#undef ERROR
#endif

namespace MemVM::IO {

    constexpr size_t MAX_CHANNELS = 16;
    constexpr size_t BUFFER_SIZE = 4096;
    constexpr size_t HEADER_SIZE = 64;
    constexpr size_t MAX_PAYLOAD = BUFFER_SIZE - HEADER_SIZE;

    enum class Command : uint8_t {
        NONE = 0,
        READ_PHYS = 1,
        WRITE_PHYS = 2,
        CLEAR_CACHE = 3,
        EXIT = 4
    };

    enum class Status : uint8_t {
        IDLE = 0,
        BUSY = 1,
        SUCCESS = 2,
        FAILED = 3
    };

    enum class Error : uint8_t {
        NONE = 0,
        INVALID_ADDRESS = 1,
        ACCESS_DENIED = 2,
        INVALID_SIZE = 3,
        TIMEOUT = 4,
        DRIVER_ERROR = 5
    };

    struct Request {
        uint64_t address;
        uint32_t size;
    };

    struct alignas(64) Channel {
        volatile Status status;
        Command command;
        Error error;
        uint8_t channelId;
        uint16_t payloadSize;
        uint8_t payload[MAX_PAYLOAD];
    };

    struct alignas(4096) SharedMemoryBlock {
        volatile bool serverAlive;
        volatile bool shutdown;
        
        Channel channels[MAX_CHANNELS];
    };
}