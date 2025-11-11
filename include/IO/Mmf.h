#pragma once
#include "Mmf_Types.h"

namespace MemVM::IO {

class Mmf {
public:
    explicit Mmf(const char* name);
    ~Mmf();
    
    Mmf(const Mmf&) = delete;
    Mmf& operator=(const Mmf&) = delete;
    
    bool IsValid() const noexcept { return m_Valid; }
    SharedMemoryBlock* GetBlock() noexcept { return m_Block; }
    const SharedMemoryBlock* GetBlock() const noexcept { return m_Block; }

    private:
        SharedMemoryBlock* m_Block;
        void* m_Handle;
        bool m_Valid;
    };

}