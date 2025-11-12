# MemVM

High-performance memory access layer for VirtualBox-based VMs through shared memory communication.

## Overview

MemVM provides direct physical memory access to virtual machines by hooking into VirtualBox's internal memory functions. Communication happens through lock-free shared memory channels, enabling external processes to read and write VM memory with minimal overhead.

**Key characteristics:**
- Zero-dependency design compatible with reflective injection
- 16 concurrent communication channels
- ~1-10 μs operation latency
- Thread-safe lock-free protocol

## Architecture

External processes communicate with an injected DLL running inside the VM process. Requests and responses flow through a single bidirectional shared memory block.

## Quick Start

**Build:**
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

**Inject:**
```bash
injector.exe --pid <vm_process_id> --dll bst.dll
```

**Usage from external process:**
```cpp
IO::Mmf mmf("MemVM_BlueStacks");
```

## Project Structure

```
include/IO/        Shared memory protocol implementation
include/Vmi/       VM interception and hooking logic
src/               Implementation files
libs/minhook/      Function hooking library
```

[Explore codebase structure →](#codebase-overview)

## Protocol

Communication follows a simple request-response pattern:

1. Client writes request to free channel and sets status to BUSY
2. Server processes request and writes response to same channel
3. Server sets status to SUCCESS/ERROR
4. Client reads response and resets channel to IDLE

## Codebase Overview

### Shared Memory Layer (`include/IO/`)

- [`Mmf_Types.h`](bst/include/IO/Mmf_Types.h) - Core data structures and protocol definitions
- [`Mmf_Manage.h`](bst/include/IO/Mmf_Manage.h) - Channel allocation and lifecycle
- [`Mmf_Read.h`](bst/include/IO/Mmf_Read.h) - Request deserialization
- [`Mmf_Write.h`](bst/include/IO/Mmf_Write.h) - Response serialization
- [`Mmf.h`](bst/include/IO/Mmf.h) - Memory-mapped file wrapper

### VM Interception Layer (`include/Vmi/`)

- [`Vmi_Types.h`](bst/include/Vmi/Vmi_Types.h) - VirtualBox function signatures
- [`Vmi_Hook.h`](bst/include/Vmi/Vmi_Hook.h) - Runtime VM instance capture
- [`Vmi_Manage.h`](bst/include/Vmi/Vmi_Manage.h) - Hook installation and lifecycle
- [`Vmi_Process.h`](bst/include/Vmi/Vmi_Process.h) - Memory operation wrappers
- [`Vmi.h`](bst/include/Vmi/Vmi.h) - Public API surface

## Use Cases

- Game memory scanning and modification
- VM introspection tools
- Anti-cheat research
- Automated testing frameworks
- Memory forensics

## Requirements

- Windows 10/11 x64
- CMake 3.20+
- C++23 compiler (GCC 13+ / MSVC 2022+)
- Target: VirtualBox-based VM processes

## Technical Constraints

Built without C runtime dependencies to support reflective DLL injection. All synchronization uses Windows API primitives. Memory layout is cache-aligned for optimal performance.

## License

MIT

## Contributing

Pull requests welcome. Please maintain the zero-dependency constraint and follow existing code organization patterns.