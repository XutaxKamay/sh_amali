#include "pch.h"
#include <TlHelp32.h>
#include <condition_variable>
#include <hooks.h>

void mainThread(HMODULE hModule)
{
    auto gameBase = reinterpret_cast<uintptr_t>(GetModuleHandleA("game.exe"));

    auto gameDosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(gameBase);

    console::Msg("Found game module: %p\n", gameBase);

    auto gameNtHeader = reinterpret_cast<PIMAGE_NT_HEADERS>(
        gameBase + gameDosHeader->e_lfanew);

    console::Msg("Found game ntheader: %p\n", gameNtHeader);

    auto gameImageSize = gameNtHeader->OptionalHeader.SizeOfImage;

    console::Msg("Found game module size: %p\n", gameImageSize);

    // 6A 18 B8 ? ? ? ? E8 ? ? ? ? 8B 7D 08 8B DF 8D 47 04 F7 DB 1B DB 23 D8 89
    // 5D E0 33 F6 89 75 E4 39 73 18 75 73 EndScene
    auto d3dDevInst = memory_info::search("83 EC 78 A1 ?? ?? ?? ?? 53",
                                            gameBase,
                                            gameBase + gameImageSize);

    uintptr_t* d3dDeviceVTable = nullptr;

    if (d3dDevInst.size() != 0)
    {
        // 0x404
        auto ptrDevice = *reinterpret_cast<uintptr_t*>(d3dDevInst[0] + 4);
        d3dDeviceVTable = **reinterpret_cast<uintptr_t***>(ptrDevice);

        console::Msg("Found d3d device vtable: %p (EndScene %p)\n",
                     d3dDeviceVTable,
                     (d3dDeviceVTable[42] - reinterpret_cast<uintptr_t>(
                                                GetModuleHandleA("d3d9.dll"))));

        DWORD dwOld;
        VirtualProtect(reinterpret_cast<ptr_t>(&d3dDeviceVTable[42]),
                       sizeof(ptr_t),
                       PAGE_EXECUTE_READWRITE,
                       &dwOld);

        OriginalEndScene = reinterpret_cast<EndScene_t>(d3dDeviceVTable[42]);
        d3dDeviceVTable[42] = reinterpret_cast<uintptr_t>(new_EndScene);

        VirtualProtect(reinterpret_cast<ptr_t>(&d3dDeviceVTable[42]),
                       sizeof(ptr_t),
                       PAGE_EXECUTE_READWRITE,
                       &dwOld);

        console::Msg("Hooked endscene\n");
    }

    auto window = FindWindowA(nullptr, "Shaiya");
    oWndProc = (WNDPROC)SetWindowLongPtr(window, GWL_WNDPROC, (LONG_PTR)WndProc);

    while (true)
    {
        if (GetAsyncKeyState(VK_F8))
        {
            FreeConsole();

            SetWindowLongPtr(window, GWL_WNDPROC, (LONG_PTR)oWndProc);

            auto suspendThreads = []() {
                HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
                if (h != INVALID_HANDLE_VALUE)
                {
                    THREADENTRY32 te;
                    te.dwSize = sizeof(te);
                    if (Thread32First(h, &te))
                    {
                        do
                        {
                            if (te.dwSize >= FIELD_OFFSET(THREADENTRY32,
                                                          th32OwnerProcessID) +
                                                 sizeof(te.th32OwnerProcessID))
                            {
                                // Suspend all threads EXCEPT the one we
                                // want to keep running
                                if (te.th32ThreadID != GetCurrentThreadId() &&
                                    te.th32OwnerProcessID ==
                                        GetCurrentProcessId())
                                {
                                    HANDLE thread = ::OpenThread(
                                        THREAD_ALL_ACCESS,
                                        FALSE,
                                        te.th32ThreadID);
                                    if (thread != NULL)
                                    {
                                        SuspendThread(thread);
                                        CloseHandle(thread);
                                    }
                                }
                            }
                            te.dwSize = sizeof(te);
                        } while (Thread32Next(h, &te));
                    }
                    CloseHandle(h);
                }
            };

            DWORD dwOld;
            VirtualProtect(reinterpret_cast<ptr_t>(&d3dDeviceVTable[42]),
                           sizeof(ptr_t),
                           PAGE_EXECUTE_READWRITE,
                           &dwOld);

            d3dDeviceVTable[42] = reinterpret_cast<uintptr_t>(OriginalEndScene);

            VirtualProtect(reinterpret_cast<ptr_t>(&d3dDeviceVTable[42]),
                           sizeof(ptr_t),
                           PAGE_EXECUTE_READWRITE,
                           &dwOld);

            g_bWantExit = true;

            if (g_bRendering)
            {
                std::mutex m;
                std::unique_lock<std::mutex> l(m);
                g_bRenderingEvent.wait(l);
            }

            FreeLibraryAndExitThread(hModule, 0);
            break;
        }

        Sleep(100);
    }
}
