#pragma once

#include "Fix16_Point.hpp"
#include "Function.hpp"
#include "fix16.hpp"

namespace DataType
{
enum
{
    byte = 0,
    word = 1,
    dword = 2,
    fix16_float = 3,
};
}; // namespace DataType

struct PixelsToFix16Table
{
    EXPORT PixelsToFix16Table();

    // 9.6f 0x41FE70
    Fix16 SignedPixelsToFix16(const s8& pixels) const
    {
        if (pixels < 0)
        {
            return -list[-pixels];
        }
        return list[pixels];
    }

    // Same as SignedPixelsToFix16, but only matches with the argument by value
    EXPORT Fix16 SignedPixelsToFix16_440860(s8 pixels) const;

    Fix16 list[256];
};

EXTERN_GLOBAL(PixelsToFix16Table, gPixelsToFix16);

class ModelPhysics
{
  public:
    u8 mModel; // 0x0
    s8 mTurbo; // 0x1
    u8 mValue; // 0x2
    s8 mPadding3; // 0x3
    Fix16 mMass; // 0x4
    Fix16 mFrontDriveBias; // 0x8
    Fix16 mFrontMassBias; // 0xC
    Fix16 mBrakeFriction; // 0x10
    Fix16 mTurnIn; // 0x14
    Fix16 mTurnRatio; // 0x18
    Fix16 mRearEndStability; // 0x1C
    Fix16 mHandbrakeSlideValue; // 0x20
    Fix16 mThrust; // 0x24
    Fix16 mMaxSpeed; // 0x28
    Fix16 mAntiStrngth; // 0x2C
    Fix16 mSkedThreshold; // 0x30
    Fix16 mGear1Multiplier; // 0x34
    Fix16 mGear2Multiplier; // 0x38
    Fix16 mGear3Multiplier; // 0x3C
    Fix16 mGear2Speed; // 0x40
    Fix16 mGear3Speed; // 0x44

    Fix16 get_anti_strngth() const
    {
        return mAntiStrngth;
    }

    EXPORT void ConvertMass();
};
GTA2_ASSERT_SIZEOF_ALWAYS(ModelPhysics, 0x48);

class CarInfo_2C
{
  public:
    EXPORT CarInfo_2C();
    EXPORT ~CarInfo_2C();
    EXPORT void CalculateCarInfo(s32 idx);

    Fix16 mMomentOfInertia; // 0x0
    Fix16 mFrontWheelOffset; // 0x4
    Fix16 mRearWheelOffset; // 0x8
    Fix16_Point mCenterOfMassOffset; // 0xC
    Fix16 mHalfThrust; // 0x14
    Fix16 mFithThrust; // 0x18
    Fix16 mMaxThrustWithTurbo; // 0x1C
    Fix16 mRearDriveBias; // 0x20
    Fix16 mSkidThreshhold1; // 0x24
    Fix16 mSkidThreshhold2; // 0x28
};
GTA2_ASSERT_SIZEOF_ALWAYS(CarInfo_2C, 0x2C);

class CarInfoTable
{
  public:
    STATIC_MEMBER(u32, sProcessedOutput);
    STATIC_MEMBER(u32, sProcessedGciData);
    STATIC_MEMBER(u8*, sModelPhyArrPtr);
    STATIC_MEMBER(s32, sModelPhyArrLen);
    STATIC_MEMBER(s32, sLineNumber);
    STATIC_MEMBER(u8*, sInputData);
    STATIC_MEMBER(u32, sGciDataLen);
    STATIC_MEMBER_ARRAY(char, sFileName, 64);
    EXPORT ModelPhysics* GetModelPhysicsFromIdx(u8 model_idx) const;
    EXPORT void LoadModelPhysics();
    EXPORT CarInfo_2C* GetInfoAtIdx(u8 idx) const;
    EXPORT void CalculateAllCarInfo();
    EXPORT void ConvertAllMass();
    EXPORT void LoadFromGciFile(const char_type* pGciFilePath);
    EXPORT void Reload();
    EXPORT void Clear();
    EXPORT void Free();
    EXPORT CarInfoTable();
    EXPORT ~CarInfoTable();

    EXPORT static char* __stdcall parse_gci_file(void* input,
                                                 size_t input_size,
                                                 ModelPhysics* output,
                                                 size_t output_size,
                                                 u32* next_position);
    EXPORT static char* __stdcall SetErr(s32 error_code);
    EXPORT static s32 __stdcall ReadToken(char* pOut);
    EXPORT static s32 __stdcall PushData(void* pSrc, u32 size);
    EXPORT static s32 __stdcall HexStr2Long(const char* pStr, s32* pOut);
    EXPORT static s32 __stdcall HexStr2Short(const char* pStr, s16* pOut);
    EXPORT static s32 __stdcall StrToS32(const char* pStr, s32* pOut);
    EXPORT static s32 __stdcall FloatStrToFix16(char* pStr, Fix16& out);
    EXPORT static s32 __stdcall StrToS16(const char* pStr, s16* pOut);

    CarInfo_2C* mPtrArray[256]; // 0x0
    CarInfo_2C* mRawData400; // 0x400
    ModelPhysics* mModelPhysicsArray[256]; // 0x404
    ModelPhysics* mRawData804; // 0x804
};
GTA2_ASSERT_SIZEOF_ALWAYS(CarInfoTable, 0x808);

EXTERN_GLOBAL(CarInfoTable*, gCarInfo_808);

EXPORT s32 __stdcall ParseTokenAndPush(char_type* pStr);
