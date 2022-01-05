#include "stdafx.h"
#include "Linker.h"
#include "Utility.h"
// #include "FileReader.h"
#include "..\detours-master\detours\Detours.h"

// Sets the function address + base address
#define SetFunctionAddress(function, address) function = reinterpret_cast<decltype(function)>(BaseAddress + address);

bool IsDeltaAnimOverride(int xanim)
{
    auto str = Linker::GetString(xanim);

    if (Linker::Verbose) std::cout << "PhilLibX.T7MTEnhancements: Validating Delta XAnim: " << str << "\n"; std::cout.flush();

    // If it exists, that's enough for us rn
    if (str && std::filesystem::exists((std::filesystem::path(Utility::GetEnvVar("TA_TOOLS_PATH")) / "share\\raw\\xanim_raw" / str).string() + ".xanim_raw"))
        return false;
    // If it exists, that's enough for us rn
    if (str && std::filesystem::exists((std::filesystem::path(Utility::GetEnvVar("TA_TOOLS_PATH")) / "share\\raw\\xanim" / str).string() + ".direct_xanim"))
        return false;

    return Linker::IsDeltaAnim(xanim) == 0;
}

bool IsAdditiveAnimOverride(int xanim)
{
    auto str = Linker::GetString(xanim);

    if (Linker::Verbose) std::cout << "PhilLibX.T7MTEnhancements: Validating Is Additive XAnim: " << str << "\n"; std::cout.flush();

    // If it exists, that's enough for us rn
    if (str && std::filesystem::exists((std::filesystem::path(Utility::GetEnvVar("TA_TOOLS_PATH")) / "share\\raw\\xanim_raw" / str).string() + ".xanim_raw"))
        return false;
    // If it exists, that's enough for us rn
    if (str && std::filesystem::exists((std::filesystem::path(Utility::GetEnvVar("TA_TOOLS_PATH")) / "share\\raw\\xanim" / str).string() + ".direct_xanim"))
        return false;

    return Linker::IsAdditiveAnim(xanim) == 0;
}

bool IsLoopSyncAnimOverride(int xanim)
{
    auto str = Linker::GetString(xanim);

    if (Linker::Verbose) std::cout << "PhilLibX.T7MTEnhancements: Validating Is Loop Sync XAnim: " << str << "\n"; std::cout.flush();

    // If it exists, that's enough for us rn
    if (str && std::filesystem::exists((std::filesystem::path(Utility::GetEnvVar("TA_TOOLS_PATH")) / "share\\raw\\xanim_raw" / str).string() + ".xanim_raw"))
        return false;
    // If it exists, that's enough for us rn
    if (str && std::filesystem::exists((std::filesystem::path(Utility::GetEnvVar("TA_TOOLS_PATH")) / "share\\raw\\xanim" / str).string() + ".direct_xanim"))
        return false;

    return Linker::IsLoopSyncAnim(xanim) == 0;
}

bool IsLoopingAnimOverride(int xanim)
{
    auto str = Linker::GetString(xanim);

    if (Linker::Verbose) std::cout << "PhilLibX.T7MTEnhancements: Validating Is Looping XAnim: " << str << "\n"; std::cout.flush();

    // If it exists, that's enough for us rn
    if (str && std::filesystem::exists((std::filesystem::path(Utility::GetEnvVar("TA_TOOLS_PATH")) / "share\\raw\\xanim_raw" / str).string() + ".xanim_raw"))
        return false;
    // If it exists, that's enough for us rn
    if (str && std::filesystem::exists((std::filesystem::path(Utility::GetEnvVar("TA_TOOLS_PATH")) / "share\\raw\\xanim" / str).string() + ".direct_xanim"))
        return false;

    return Linker::IsLoopingAnim(xanim) == 0;
}

bool ValidateAimAnimation(uint64_t a1, uint64_t a2, uint64_t a3, uint64_t a4, uint32_t a5, uint32_t xanim, int a7, float a8)
{
    auto str = Linker::GetString(xanim);

    if (Linker::Verbose) std::cout << "PhilLibX.T7MTEnhancements: Validating Aim XAnim Angles: " << str << "\n"; std::cout.flush();

    // If it exists, that's enough for us rn
    if (str && std::filesystem::exists((std::filesystem::path(Utility::GetEnvVar("TA_TOOLS_PATH")) / "share\\raw\\xanim_raw" / str).string() + ".xanim_raw"))
        return true;
    // If it exists, that's enough for us rn
    if (str && std::filesystem::exists((std::filesystem::path(Utility::GetEnvVar("TA_TOOLS_PATH")) / "share\\raw\\xanim" / str).string() + ".direct_xanim"))
        return true;


    return Linker::ValidateAdditive(a1, a2, a3, a4, a5, xanim, a7, a8);
}

void Linker::SetFunctionAddresses()
{
    // Asset load functions
    SetFunctionAddress(Load_FxAsset,            0x31AB50);
    SetFunctionAddress(Load_ImpactFxAsset,      0x31A9B0);
    SetFunctionAddress(Load_RumbleAsset,        0x32DBD0);
    SetFunctionAddress(Load_LightDescription,   0x31C680);
    SetFunctionAddress(Load_XAnim,              0x3057D0);

    // Script related functions
    SetFunctionAddress(AddString, 0x53E30);
    SetFunctionAddress(GetString, 0x53610);

    // AI Validation
    SetFunctionAddress(IsDeltaAnim,         0x8ED30);
    SetFunctionAddress(IsAdditiveAnim,      0x8EB60);
    SetFunctionAddress(ValidateAdditive,    0x8FE90);
    SetFunctionAddress(IsLoopSyncAnim,      0x8EF40);
    SetFunctionAddress(IsLoopingAnim,       0x8F110);

    // General
    SetFunctionAddress(G_Error, 0x318780);
}

XAnimAsset* LoadXAnimOverride(char* xanimName)
{
    /*
    try
    {
        if (Linker::Verbose) std::cout << "PhilLibX.T7MTEnhancements: Loading XAnim Raw: " << xanimName << "\n"; std::cout.flush();

        // Look for raw version, then continue
        auto path = (std::filesystem::path(Utility::GetEnvVar("TA_TOOLS_PATH")) / "share\\raw\\xanim_raw" / xanimName).string() + ".xanim_raw";

        // Attempt to locate the xanim
        if (std::filesystem::exists(path))
        {

            auto reader = SL::FileReader(path);
            auto xanim = reader.ReadStruct<XAnimAsset>();
            auto result = std::make_unique<XAnimAsset>();
            std::memcpy(result.get(), &xanim, sizeof(xanim));

            if (xanim.Name != nullptr)
                result->Name = reader.ReadNullTerminatedStringRaw().release();

            if (xanim.Names != nullptr)
            {
                auto names = std::make_unique<ScriptString[]>(xanim.BoneCount);

                for (uint16_t i = 0; i < xanim.BoneCount; i++)
                    names[i] = Linker::AddString((char*)reader.ReadNullTerminatedString().c_str(), 0, 0xC);

                result->Names = names.release();
            }


            size_t bytesRead = 0;

            if (xanim.DataByte          != nullptr)
                result->DataByte         = reader.ReadBytes((size_t)xanim.DataByteCount, bytesRead).release();
            if (xanim.DataShort         != nullptr)
                result->DataShort        = reader.ReadBytes((size_t)xanim.DataShortCount * 2, bytesRead).release();
            if (xanim.DataInt           != nullptr)
                result->DataInt          = reader.ReadBytes((size_t)xanim.DataIntCount * 4, bytesRead).release();
            if (xanim.RandomDataByte    != nullptr)
                result->RandomDataByte   = reader.ReadBytes((size_t)xanim.RandomDataByteCount, bytesRead).release();
            if (xanim.RandomDataShort   != nullptr)
                result->RandomDataShort  = reader.ReadBytes((size_t)xanim.RandomDataShortCount * 2, bytesRead).release();
            if (xanim.RandomDataInt     != nullptr)
                result->RandomDataInt    = reader.ReadBytes((size_t)xanim.RandomDataIntCount * 4, bytesRead).release();
            if (xanim.ExtraChannelData  != nullptr)
                result->ExtraChannelData = reader.ReadBytes((size_t)xanim.ExtraChannelDataCount * 4, bytesRead).release();
            if (xanim.IKPitchLayers     != nullptr)
                result->IKPitchLayers    = reader.ReadBytes((size_t)xanim.IKPitchLayerCount * 8, bytesRead).release();
            if (xanim.IKPitchBones      != nullptr)
                result->IKPitchBones     = reader.ReadBytes((size_t)xanim.IKPitchBoneCount * 28, bytesRead).release();

            for (size_t i = 0; i < 3; i++)
            {
                if (xanim.Notifies[i].Infos != nullptr)
                {
                    auto infos = std::make_unique<XAnimNotifyInfo[]>(xanim.Notifies[i].Count);

                    for (uint8_t n = 0; n < xanim.Notifies[i].Count; n++)
                    {
                        infos[n].Time = reader.ReadFloat();
                        infos[n].Type = Linker::AddString((char*)reader.ReadNullTerminatedString().c_str(), 0, 0xC);
                        infos[n].Param1 = Linker::AddString((char*)reader.ReadNullTerminatedString().c_str(), 0, 0xC);
                        infos[n].Param2 = Linker::AddString((char*)reader.ReadNullTerminatedString().c_str(), 0, 0xC);
                    }

                    result->Notifies[i].Infos = infos.release();
                }
            }

            if (xanim.DeltaParts != nullptr)
            {
                auto xanimDeltaParts = reader.ReadStruct<XAnimDeltaPart>();
                auto partsResult = std::make_unique<XAnimDeltaPart>();

                if (xanimDeltaParts.Translations != nullptr)
                {
                    auto translationCount = reader.ReadUInt32();
                    auto byteTranslations = reader.ReadUInt16();
                    auto indexBufferSize = xanim.FrameCount >= 0x100 ? translationCount * 2 : translationCount;
                    auto frameBufferSize = byteTranslations == 0 ? translationCount * 6 : translationCount * 3;

                    if (translationCount == 1)
                    {
                        // Buffer
                        auto translationBuffer = std::make_unique<char[]>(20);
                        auto frame0 = reader.ReadBytes(12, bytesRead);

                        *(uint16_t*)(translationBuffer.get()) = (uint16_t)(translationCount - 1);
                        std::memcpy(translationBuffer.get() + 2, &byteTranslations, sizeof(byteTranslations));
                        std::memcpy(translationBuffer.get() + 8, frame0.get(), 12);

                        partsResult->Translations = translationBuffer.release();
                    }
                    else
                    {
                        auto translationBuffer = std::make_unique<char[]>(40 + (size_t)indexBufferSize);
                        auto min = reader.ReadBytes(12, bytesRead);
                        auto max = reader.ReadBytes(12, bytesRead);
                        auto frameBuffer = reader.ReadBytes(frameBufferSize, bytesRead);
                        auto indexBuffer = reader.ReadBytes(indexBufferSize, bytesRead);



                        *(uint16_t*)(translationBuffer.get()) = (uint16_t)(translationCount - 1);
                        std::memcpy(translationBuffer.get() + 2, &byteTranslations, sizeof(byteTranslations));
                        std::memcpy(translationBuffer.get() + 8, min.get(), 12);
                        std::memcpy(translationBuffer.get() + 20, max.get(), 12);
                        *(uint64_t*)(translationBuffer.get() + 32) = (uint64_t)frameBuffer.release();
                        std::memcpy(translationBuffer.get() + 40, indexBuffer.get(), indexBufferSize);

                        partsResult->Translations = translationBuffer.release();
                    }
                }


                if (xanimDeltaParts.Quaternions2D != nullptr)
                {
                    auto rotationCount = reader.ReadUInt32();
                    auto frameBufferSize = rotationCount * 4;
                    auto indexBufferSize = xanim.FrameCount >= 0x100 ? rotationCount * 2 : rotationCount;

                    if (rotationCount == 1)
                    {
                        // Buffer
                        auto rotationBuffer = std::make_unique<char[]>(12);

                        auto frame0 = reader.ReadBytes(4, bytesRead);

                        *(uint16_t*)(rotationBuffer.get()) = (uint16_t)(rotationCount - 1);
                        std::memcpy(rotationBuffer.get() + 8, frame0.get(), 4);

                        partsResult->Quaternions2D = rotationBuffer.release();
                    }
                    else
                    {
                        auto rotationBuffer = std::make_unique<char[]>(16 + (size_t)indexBufferSize);
                        auto frameBuffer = reader.ReadBytes(frameBufferSize, bytesRead);
                        auto indexBuffer = reader.ReadBytes(indexBufferSize, bytesRead);

                        *(uint16_t*)(rotationBuffer.get()) = (uint16_t)(rotationCount - 1);
                        *(uint64_t*)(rotationBuffer.get() + 8) = (uint64_t)frameBuffer.release();
                        std::memcpy(rotationBuffer.get() + 16, indexBuffer.get(), indexBufferSize);

                        partsResult->Quaternions2D = rotationBuffer.release();
                    }
                }

                if (xanimDeltaParts.Quaternions != nullptr)
                {
                    auto rotationCount = reader.ReadUInt32();
                    auto frameBufferSize = rotationCount * 4;
                    auto indexBufferSize = xanim.FrameCount >= 0x100 ? rotationCount * 2 : rotationCount;

                    if (rotationCount == 1)
                    {
                        // Buffer
                        auto rotationBuffer = std::make_unique<char[]>(16);
                        auto frame0 = reader.ReadBytes(4, bytesRead);

                        std::memcpy(rotationBuffer.get(), &rotationCount, sizeof(rotationCount));
                        std::memcpy(rotationBuffer.get() + 8, frame0.get(), 12);

                        partsResult->Quaternions2D = rotationBuffer.release();
                    }
                    else
                    {
                        auto rotationBuffer = std::make_unique<char[]>(16 + (size_t)indexBufferSize);
                        auto frameBuffer = reader.ReadBytes(frameBufferSize, bytesRead);
                        auto indexBuffer = reader.ReadBytes(indexBufferSize, bytesRead);

                        *(uint16_t*)(rotationBuffer.get()) = (uint16_t)(rotationCount - 1);
                        *(uint64_t*)(rotationBuffer.get() + 8) = (uint64_t)frameBuffer.release();
                        std::memcpy(rotationBuffer.get() + 16, indexBuffer.get(), indexBufferSize);

                        partsResult->Quaternions2D = rotationBuffer.release();
                    }
                }

                result->DeltaParts = partsResult.release();
            }

            return result.release();
        }
        else
        {
            return (XAnimAsset*)Linker::Load_XAnim(xanimName);
        }
    }
    catch (const std::exception& what)
    {
        Linker::G_Error(4, "Fatal error loading XAnim Raw: %s", what.what());
    }

    return nullptr;
    */
    return (XAnimAsset*)Linker::Load_XAnim(xanimName);
}

// Values we can substitute
std::map<uint32_t, uint32_t> Substitutes;

void Linker::LoadSubstitutes(std::filesystem::path dir)
{
    if (std::filesystem::exists(dir))
    {
        for (const auto& file : std::filesystem::directory_iterator(dir))
        {
            if (file.path().extension() == ".csv")
            {
                std::ifstream inputStream(file.path());

                if (inputStream)
                {
                    if (Linker::Verbose) std::cout << "PhilLibX.T7MTEnhancements: Loading Substitute Table: " << file.path().filename() << "..\n"; std::cout.flush();

                    std::string buffer;

                    while (inputStream >> buffer)
                    {
                        // Check for comments
                        if (buffer.find('#', 0) == std::string::npos)
                        {
                            auto lineSplit = Utility::SplitString(buffer, ',');

                            // Validate data, we're expecting 2 entries (String + Hash)
                            if (lineSplit.size() == 2)
                            {
                                uint32_t hash = std::stoul(lineSplit[1], nullptr, 16);

                                if (hash)
                                {
                                    Substitutes.emplace(Utility::ComputeFNVHash(lineSplit[0]), hash);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

int _fastcall Linker::ComputeFNVHashOverride(char* input)
{
    if (input == nullptr)
        return 0;

    auto value = std::string(input);

    // See if we have a hash value from Cerberus
    if (value.find("function_") == 0 || value.find("namespace_") == 0 || value.find("var_") == 0 || value.find("hash_") == 0)
    {
        auto hashValue = Utility::ExtractHashFromString(value.data());

        // Ensure success, if not resort back to the algorithm
        if (hashValue > 0)
        {
            if (Linker::Verbose) std::cout << "PhilLibX.T7MTEnhancements: Using Literal Hash Value: " << std::hex << hashValue << std::dec << " for: " << input << "\n"; std::cout.flush();
            return hashValue;
        }
    }

    // Perform standard hashing
    auto hashResult = Utility::ComputeFNVHash(value);

    // See if we can find an override
    auto sub = Substitutes.find(hashResult);

    if (sub != Substitutes.end())
    {
        if (Linker::Verbose) std::cout << "PhilLibX.T7MTEnhancements: Using Substitute Hash Value: " << std::hex << sub->second << std::dec << " for: " << input << "\n"; std::cout.flush();
        return sub->second;
    }

    // Perform standard hashing
    return hashResult;
}

uint64_t __fastcall Linker::HashNotificationOverride(char* value)
{
    return ComputeFNVHashOverride(value);
}

uint64_t _fastcall Linker::ComputeCRC32Override(uint64_t a1, uint64_t a2, uint32_t a3, uint32_t a4)
{
    return 0x424F4353;
}

void Linker::PatchMemory()
{
    //Detours::X64::DetourFunction((PBYTE)(BaseAddress + 0x8F340),    (PBYTE)&IsDeltaAnimOverride);
    //Detours::X64::DetourFunction((PBYTE)(BaseAddress + 0x8F320),    (PBYTE)&IsAdditiveAnimOverride);
    //Detours::X64::DetourFunction((PBYTE)(BaseAddress + 0x8F2E0),    (PBYTE)&IsLoopSyncAnimOverride);
    //Detours::X64::DetourFunction((PBYTE)(BaseAddress + 0x8F300),    (PBYTE)&IsLoopingAnimOverride);
    //Detours::X64::DetourFunction((PBYTE)(BaseAddress + 0x90E08),    (PBYTE)&ValidateAimAnimation);
    //Utility::PatchMemory(BaseAddress + 0x90E08, (PBYTE)"\xE8", 1);

    // Overrides 
    Detours::X64::DetourFunction((PBYTE)(BaseAddress + 0x7C6D60), (PBYTE)&ComputeCRC32Override);
    Detours::X64::DetourFunction((PBYTE)(BaseAddress + 0x2164E0), (PBYTE)&ComputeFNVHashOverride);
    Detours::X64::DetourFunction((PBYTE)(BaseAddress + 0x7C71C0), (PBYTE)&HashNotificationOverride);

    // We need to patch the in-lined loops on a case by case basis
    // All we need to worry about is the call, then we can fill it in nops
    // mov      rcx, <register with a pointer to the string>
    // call     ComputeFNVHashOverride
    // mov      <register that is used for hash>, rax
    // Patch Hash Loops and Divert to our function
    if (Linker::OverrideHash)
    {
        Utility::PatchMemory(BaseAddress + 0x7D258A, (PBYTE)"\x49\x8B\xC8\x90\xE8\x4D\x3F\xA4\xFF\x90\x8B\xD0\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90", 0x48);
        Utility::PatchMemory(BaseAddress + 0x7D25EF, (PBYTE)"\x49\x8B\xC8\x90\xE8\xE8\x3E\xA4\xFF\x90\x8B\xD0\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90", 67);
        Utility::PatchMemory(BaseAddress + 0x7DEBDB, (PBYTE)"\x48\x8B\xCB\xE8\xFD\x78\xA3\xFF\x90\x8B\xD0\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90", 0x46);
        Utility::PatchMemory(BaseAddress + 0x7D410E, (PBYTE)"\x4C\x89\xC1\x90\xE8\xC9\x23\xA4\xFF\x90\x89\xC2\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90", 0x44);
        Utility::PatchMemory(BaseAddress + 0x7D416A, (PBYTE)"\x4C\x89\xC1\x90\xE8\x6D\x23\xA4\xFF\x90\x89\xC2\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90", 0x48);
    }
    // Disable Unresolved External Check
    if(Linker::DisableUnresolvedExternalCheck)
        Utility::PatchMemory(BaseAddress + 0x7D40F5, (PBYTE)"\x90\x90\x90\x90\x90\x90\x90\x90\x90", 0x9);

    //// Asset Load Function Overrides
    //Utility::PatchPointer(BaseAddress + 0xC862D8, (uint64_t)&LoadXAnimOverride);
}

void Linker::Attach()
{
    Linker::Verbose = std::filesystem::exists(Utility::GetExePath().parent_path() / "debug.txt");

    BaseAddress = Utility::GetProcessBaseAddress(GetCurrentProcessId());

    // Compare the string, at this stage, linker should have decrypted the code segment
    while (strcmp((const char*)(BaseAddress + 0xCA13DA), "AVGeneratorData@hkaiOverlapManagerSection@@"));

    // Execute logic
    SetFunctionAddresses();
    PatchMemory();
}

enum SupportedTools : uint32_t
{
    LINKER = 0x85064a44,
    APE = 0x6F7CE918,
};

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        switch (Utility::ComputeFNVHash(Utility::GetExePath().filename().string()))
        {
        case SupportedTools::LINKER:
            std::cout << "PhilLibX.T7MTEnhancements: Initializing Version 2.0.0.0\n";
            std::flush(std::cout);
            Linker::LoadSubstitutes(Utility::GetExePath().parent_path() / "HashTables");
            CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&Linker::Attach, 0, 0, 0);
            break;
        }
    }

    return TRUE;
}