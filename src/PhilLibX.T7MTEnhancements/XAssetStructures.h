#pragma once
#include "stdafx.h"

typedef uint32_t ScriptString;

struct XAnimNotifyInfo
{
    ScriptString Type;
    float Time;
    ScriptString Param1;
    ScriptString Param2;
};

#pragma pack(push, 8)
struct XAnimNotify
{
    XAnimNotifyInfo* Infos;
    uint8_t Count;
};
#pragma pack(pop)

struct XAnimDeltaPart
{
    char* Translations;
    char* Quaternions2D;
    char* Quaternions;
};

struct XAnimAsset
{
    char* Name;
    uint32_t RandomDataByteCount;
    uint32_t DataShortCount;
    uint32_t ExtraChannelDataCount;
    uint32_t DataByteCount;
    uint32_t DataIntCount;
    uint32_t RandomDataIntCount;
    uint16_t FrameCount;
    uint16_t BoneCount;
    uint8_t Flags[8];
    uint32_t UnkCount;
    uint16_t BoneCounts[10]; // Above is total, this is across different part types
    uint8_t AssetInfo[4];
    uint32_t RandomDataShortCount;
    uint32_t IndexCount;
    float FrameRate;
    float Frequency;
    float PrimedLength;
    float LoopEntryTime;
    uint32_t IKPitchLayerCount;
    uint32_t IKPitchBoneCount;
    ScriptString* Names;
    char* DataByte;
    char* DataShort;
    char* DataInt;
    char* RandomDataShort;
    char* RandomDataByte;
    char* RandomDataInt;
    char* ExtraChannelData;
    char* Indices;
    char* IKPitchLayers;
    char* IKPitchBones;
    XAnimNotify Notifies[3]; // Startup, Notify, Shutdown 
    XAnimDeltaPart* DeltaParts;
};
