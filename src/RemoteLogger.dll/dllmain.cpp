#include "pch.h"

// RemoteLogger is a DLL Treyarch seem to use that is loaded by APE, Linker, and Radiant before anything happens
// and is guaranteed to be loaded after init

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        LoadLibraryA("PhilLibX.T7MTEnhancements.dll");
    }

    return TRUE;
}

