#include "IO/Mmf.h"
#include "IO/Mmf_Manage.h"
#include <windows.h>
#include <cstdlib>
namespace MemVM::IO {

    Mmf::Mmf(const char* name)
        : m_Block(nullptr), m_Handle(nullptr), m_Valid(false) {
        
        constexpr size_t SIZE = sizeof(SharedMemoryBlock);
        
        m_Handle = CreateFileMappingA(
            INVALID_HANDLE_VALUE,
            nullptr,
            PAGE_READWRITE,
            0,
            SIZE,
            name
        );
        
        if (!m_Handle) return;
        
        m_Block = (SharedMemoryBlock*)MapViewOfFile(
            m_Handle,
            FILE_MAP_ALL_ACCESS,
            0, 0, SIZE
        );
        
        if (!m_Block) {
            CloseHandle(m_Handle);
            m_Handle = nullptr;
            return;
        }
        
        InitializeBlock(m_Block);
        
        m_Valid = (m_Block != nullptr);
    }

    Mmf::~Mmf() {
        if (!m_Block) return;
        
        UnmapViewOfFile(m_Block);
        if (m_Handle) CloseHandle(m_Handle);
        
        m_Block = nullptr;
        m_Handle = nullptr;
        m_Valid = false;
    }
}