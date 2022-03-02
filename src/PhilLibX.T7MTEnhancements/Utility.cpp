#include "stdafx.h"
#include "Utility.h"

std::unique_ptr<char[]> Utility::ReadAllBytes(const std::string filePath, uint64_t* size)
{
    std::ifstream stream(filePath, std::ios::in | std::ios::binary | std::ios::ate);
    stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    *size = (uint64_t)stream.tellg();

    auto result = std::make_unique<char[]>(*size);

    stream.seekg(0, std::ios::beg);
    stream.read(result.get(), *size);

    return result;
}

std::string Utility::GetEnvVar(const std::string& var)
{
    char varData[MAX_PATH + 1];
    GetEnvironmentVariableA(var.c_str(), varData, MAX_PATH);
    return varData;
}

std::filesystem::path Utility::GetExePath()
{
    char appPath[MAX_PATH + 1];
    GetModuleFileNameA(NULL, appPath, MAX_PATH);
    return std::filesystem::path(appPath);
}

std::vector<std::string> Utility::SplitString(const std::string& input, char delimiter)
{
    std::vector<std::string> result;
    std::stringstream stringStream(input);
    std::string line;

    while (std::getline(stringStream, line, delimiter))
    {
        result.push_back(line);
    }

    return result;
}

void Utility::RemoveInString(std::string& str, size_t value, size_t len)
{
    if (value != -1)
        str.replace(value, len, "");
}

uint32_t Utility::ExtractHashFromString(const char* input)
{
    // Create a copy of the string
    auto value = std::string(input);

    try
    {
        RemoveInString(value, value.find("var_"), 4);
        RemoveInString(value, value.find("hash_"), 5);
        RemoveInString(value, value.find("function_"), 9);
        RemoveInString(value, value.find("namespace_"), 10);
        RemoveInString(value, value.find("class_"), 6);

        return std::strtoul(value.c_str(), 0, 16);
    }
    catch (...)
    {
        return 0;
    }
}

uint32_t Utility::ComputeFNVHash(std::string data)
{
    uint32_t Result = FNVOffsetBasis;

    for (size_t i = 0; i < data.length() + 1; i++)
        Result = FNVPrime * (tolower(data[i]) ^ Result);

    return Result;
}

void Utility::PatchMemory(ULONG_PTR Address, PBYTE Data, SIZE_T Size)
{
    DWORD d = 0;
    VirtualProtect((LPVOID)Address, Size, PAGE_EXECUTE_READWRITE, &d);

    for (SIZE_T i = 0; i < Size; i++)
        * (volatile BYTE*)(Address + i) = *Data++;

    VirtualProtect((LPVOID)Address, Size, d, &d);

    FlushInstructionCache(GetCurrentProcess(), (LPVOID)Address, Size);
}

void Utility::PatchPointer(ULONG_PTR Address, uint64_t Data)
{
    DWORD d = 0;
    VirtualProtect((LPVOID)Address, sizeof(uint64_t), PAGE_EXECUTE_READWRITE, &d);
    
    *(uint64_t*)(Address) = Data;

    VirtualProtect((LPVOID)Address, sizeof(uint64_t), d, &d);

    FlushInstructionCache(GetCurrentProcess(), (LPVOID)Address, sizeof(uint64_t));
}

DWORD_PTR Utility::GetProcessBaseAddress(DWORD processID)
{
    DWORD_PTR   baseAddress = 0;
    HANDLE      processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
    HMODULE* moduleArray;
    LPBYTE      moduleArrayBytes;
    DWORD       bytesRequired;

    if (processHandle)
    {
        if (EnumProcessModules(processHandle, NULL, 0, &bytesRequired))
        {
            if (bytesRequired)
            {
                moduleArrayBytes = (LPBYTE)LocalAlloc(LPTR, bytesRequired);

                if (moduleArrayBytes)
                {
                    unsigned int moduleCount;

                    moduleCount = bytesRequired / sizeof(HMODULE);
                    moduleArray = (HMODULE*)moduleArrayBytes;

                    if (EnumProcessModules(processHandle, moduleArray, bytesRequired, &bytesRequired))
                    {
                        baseAddress = (DWORD_PTR)moduleArray[0];
                    }

                    LocalFree(moduleArrayBytes);
                }
            }
        }

        CloseHandle(processHandle);
    }

    return baseAddress;
}
