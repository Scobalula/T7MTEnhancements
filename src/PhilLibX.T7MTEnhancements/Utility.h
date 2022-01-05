#pragma once
#include "stdafx.h"
#include <filesystem>
#include <vector>

namespace Utility
{
    // FNV Offset
    constexpr auto FNVOffsetBasis = 0x4B9ACE2F;
    // FNV Prime
    constexpr auto FNVPrime = 0x1000193;

    // Reads all bytes from the file
    std::unique_ptr<char[]> ReadAllBytes(const std::string filePath, uint64_t* size);
    // Gets Env Var
    std::string GetEnvVar(const std::string& var);
    // Gets the path of the Exe
    std::filesystem::path GetExePath();
    // Splits a string by the delimiter
    std::vector<std::string> SplitString(const std::string& input, char delimiter);
    // Removes the data of the length at the given offset from the the string
    void RemoveInString(std::string& str, size_t value, size_t len);
    // Extracts a hash value from the string
    uint32_t ExtractHashFromString(const char* input);
    // Computes a 32Bit FNV Hash
    uint32_t ComputeFNVHash(std::string data);
    // Patches Memory at the given address (from LinkerMod (https://github.com/Nukem9/LinkerMod/blob/development/components/shared/shared_utility.h#L15))
    void PatchMemory(ULONG_PTR Address, PBYTE Data, SIZE_T Size);
    // Patches Memory at the given address (from LinkerMod (https://github.com/Nukem9/LinkerMod/blob/development/components/shared/shared_utility.h#L15))
    void PatchPointer(ULONG_PTR Address, ULONG_PTR Data);
    // Gets the base address of the given process (https://stackoverflow.com/a/26573045)
    DWORD_PTR GetProcessBaseAddress(DWORD processID);
};

