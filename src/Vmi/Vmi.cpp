#include "Vmi/Vmi.h"
#include "IO/Mmf.h"
#include "IO/Mmf_Manage.h"
#include "IO/Mmf_Read.h"
#include "IO/Mmf_Write.h"
#include <windows.h>

namespace MemVM::Vmi {

    struct ServerState {
        HANDLE thread;
        volatile bool running;
    };

    static ServerState g_Server;

    static DWORD WINAPI ServerLoop(LPVOID lpParam) {
        const char* mmfName = (const char*)lpParam;

        if (!Initialize()) return 1;

        IO::Mmf mmf(mmfName);
        if (!mmf.IsValid()) return 1;

        auto* block = mmf.GetBlock();
        block->serverAlive = true;

        while (g_Server.running && !block->shutdown) {
            for (uint8_t i = 0; i < IO::MAX_CHANNELS; ++i) {
                auto& ch = block->channels[i];
                
                if (IO::GetStatus(ch) != IO::Status::BUSY) continue;

                IO::Request req = IO::Read(ch);

                switch (ch.command) {
                    case IO::Command::READ_PHYS: {
                        uint8_t buffer[IO::MAX_PAYLOAD];
                        
                        if (ProcessRead(req, buffer, sizeof(buffer))) {
                            IO::Write(ch, buffer, req.size);
                        } else {
                            IO::WriteError(ch, IO::Error::ACCESS_DENIED);
                        }
                        break;
                    }
                    
                    case IO::Command::WRITE_PHYS: {
                        const uint8_t* data = ch.payload + sizeof(IO::Request);
                        
                        if (ProcessWrite(req, data)) {
                            IO::WriteSuccess(ch);
                        } else {
                            IO::WriteError(ch, IO::Error::ACCESS_DENIED);
                        }
                        break;
                    }

                    case IO::Command::EXIT:
                        block->shutdown = true;
                        IO::WriteSuccess(ch);
                        break;

                    default:
                        IO::WriteError(ch, IO::Error::DRIVER_ERROR);
                        break;
                }
            }
            
            Sleep(1);
        }

        block->serverAlive = false;
        Cleanup();
        
        return 0;
    }

    bool Start(const char* mmfName) noexcept {
        if (g_Server.running) return false;

        g_Server.running = true;
        g_Server.thread = CreateThread(nullptr, 0, ServerLoop, (void*)mmfName, 0, nullptr);

        return g_Server.thread != nullptr;
    }

    void Stop() noexcept {
        if (!g_Server.running) return;

        g_Server.running = false;

        if (g_Server.thread) {
            WaitForSingleObject(g_Server.thread, 5000);
            CloseHandle(g_Server.thread);
            g_Server.thread = nullptr;
        }
    }
}