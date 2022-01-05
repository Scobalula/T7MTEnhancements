#pragma once
#include "stdafx.h"
#include "XAssetStructures.h"

namespace Linker
{
    // Default Asset for load functions
    struct XAsset
    {
        char* Name;
    };

    // Settings
    bool Verbose = true;
    bool OverrideHash = true;
    bool DisableUnresolvedExternalCheck = true;

    // Linkers Base Address
    DWORD_PTR BaseAddress = 0;

    // Asset load functions
    XAsset* (__cdecl* Load_XAnim)(const char* name);
    XAsset* (__cdecl* Load_FxAsset)(const char* name);
    XAsset* (__cdecl* Load_ImpactFxAsset)(const char* name);
    XAsset* (__cdecl* Load_TagFxAsset)(const char* name);
    XAsset* (__cdecl* Load_RumbleAsset)(const char* name, char* a1, char* a2, char* a3);
    XAsset* (__cdecl* Load_LightDescription)(const char* name);

    // Script related functions
    ScriptString (__cdecl* AddString)(char* name, uint32_t user, uint32_t type);
    char* (__cdecl* GetString)(ScriptString index);

    // AI compiler functions
    bool(__cdecl* IsDeltaAnim)(uint32_t scriptStrIndex);
    bool(__cdecl* IsAdditiveAnim)(uint32_t scriptStrIndex);
    bool(__cdecl* IsLoopSyncAnim)(uint32_t scriptStrIndex);
    bool(__cdecl* IsLoopingAnim)(uint32_t scriptStrIndex);
    bool(__cdecl* ValidateAdditive)(uint64_t a1, uint64_t a2, uint64_t a3, uint64_t a4, uint32_t scriptStrIndex, uint32_t a5, uint32_t a6, float a7);

    // General
    char (__cdecl* G_Error)(uint32_t error, const char* msg, ...);

    // Loads Hash Substitutes from a CSV file
    void LoadSubstitutes(std::filesystem::path dir);
    // Overrides a function that we detour to so we can intercept string hashing
    int _fastcall ComputeFNVHashOverride(char* input);
    // Overrides notification hash function
    uint64_t __fastcall HashNotificationOverride(char* value);
    // Overrides the CRC32 Function
    uint64_t _fastcall ComputeCRC32Override(uint64_t a1, uint64_t a2, uint32_t a3, uint32_t a4);
    // Sets function addresses from Linker post-ASLR
    void SetFunctionAddresses();
    // Patches memory, detours function, etc.
    void PatchMemory();
    // Attaches to Linker to perform the necessary edits
    void Attach();
};

