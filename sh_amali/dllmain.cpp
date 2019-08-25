// dllmain.cpp : Définit le point d'entrée de l'application DLL.
#include "pch.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

// gay microsoft stuffs alert
#define stdin (__acrt_iob_func(0))
#define stdout (__acrt_iob_func(1))
#define stderr (__acrt_iob_func(2))

FILE _iob[] = {*stdin, *stdout, *stderr};

extern "C" FILE* __cdecl __iob_func(void)
{
    return _iob;
}

using namespace memory_helper;
bool g_bWantExit = false;

BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD ul_reason_for_call,
                      LPVOID lpReserved)
{
    DisableThreadLibraryCalls(hModule);

    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        AllocConsole();;
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)mainThread, hModule, 0, 0);
    }
	else if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		FreeConsole();
	}

    return true;
}
