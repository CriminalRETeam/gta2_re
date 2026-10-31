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

    Fix16 SignedPixelsToFix16_41FE70(const s8& pixels) const
    {
        if (pixels < 0)
        {
            return -list[-pixels];
        }
        return list[pixels];
    }

    // Same as SignedPixelsToFix16_41FE70, but only matches with the argument by value
    EXPORT Fix16 SignedPixelsToFix16_440860(s8 pixels) const;

    Fix16 list[256];
};

EXTERN_GLOBAL(PixelsToFix16Table, gPixelsToFix16_6F6850);

class ModelPhysics
{
  public:
    u8 field_0_model; // 0x0
    s8 field_1_turbo; // 0x1
    u8 field_2_value; // 0x2
    s8 field_3_padding; // 0x3
    Fix16 field_4_mass; // 0x4
    Fix16 field_8_front_drive_bias; // 0x8
    Fix16 field_C_front_mass_bias; // 0xC
    Fix16 field_10_brake_friction; // 0x10
    Fix16 field_14_turn_in; // 0x14
    Fix16 field_18_turn_ratio; // 0x18
    Fix16 field_1C_rear_end_stability; // 0x1C
    Fix16 field_20_handbrake_slide_value; // 0x20
    Fix16 field_24_thrust; // 0x24
    Fix16 field_28_max_speed; // 0x28
    Fix16 field_2C_anti_strngth; // 0x2C
    Fix16 field_30_sked_threshold; // 0x30
    Fix16 field_34_gear1_multiplier; // 0x34
    Fix16 field_38_gear2_multiplier; // 0x38
    Fix16 field_3C_gear3_multiplier; // 0x3C
    Fix16 field_40_gear2_speed; // 0x40
    Fix16 field_44_gear3_speed; // 0x44

    Fix16 get_anti_strngth() const
    {
        return field_2C_anti_strngth;
    }

    EXPORT void ConvertMass_454680();
};
GTA2_ASSERT_SIZEOF_ALWAYS(ModelPhysics, 0x48);

class CarInfo_2C
{
  public:
    EXPORT CarInfo_2C();
    EXPORT ~CarInfo_2C();
    EXPORT void CalculateCarInfo_4542A0(s32 idx);

    Fix16 field_0_moment_of_inertia; // 0x0
    Fix16 field_4_front_wheel_offset; // 0x4
    Fix16 field_8_rear_wheel_offset; // 0x8
    Fix16_Point field_C_center_of_mass_offset; // 0xC
    Fix16 field_14_half_thrust; // 0x14
    Fix16 field_18_fith_thrust; // 0x18
    Fix16 field_1C_max_thrust_with_turbo; // 0x1C
    Fix16 field_20_rear_drive_bias; // 0x20
    Fix16 field_24_skid_threshhold_1; // 0x24
    Fix16 field_28_skid_threshhold_2; // 0x28
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
    EXPORT ModelPhysics* GetModelPhysicsFromIdx_4546B0(u8 model_idx) const;
    EXPORT void LoadModelPhysics_4546D0();
    EXPORT CarInfo_2C* GetInfoAtIdx_454840(u8 idx) const;
    EXPORT void CalculateAllCarInfo_454850();
    EXPORT void ConvertAllMass_4549C0();
    EXPORT void LoadFromGciFile_454A00(const char_type* pGciFilePath);
    EXPORT void Reload_454A50();
    EXPORT void Clear_454A80();
    EXPORT void Free_454AA0();
    EXPORT CarInfoTable();
    EXPORT ~CarInfoTable();

    EXPORT static char* __stdcall parse_gci_file_430A30(void* input,
                                                        size_t input_size,
                                                        ModelPhysics* output,
                                                        size_t output_size,
                                                        u32* next_position);
    EXPORT static char* __stdcall SetErr_430AC0(s32 error_code);
    EXPORT static s32 __stdcall ReadToken_430B10(char* pOut);
    EXPORT static s32 __stdcall PushData_430E60(void* pSrc, u32 size);
    EXPORT static s32 __stdcall HexStr2Int_430EC0(const char* pStr, s32* pOut);
    EXPORT static s32 __stdcall HexStr2Int_430F30(const char* pStr, s16* pOut);
    EXPORT static s32 __stdcall StrToInt_430FA0(const char* pStr, s32* pOut);
    EXPORT static s32 __stdcall FloatStrToFix16_431000(char* pStr, Fix16& out);
    EXPORT static s32 __stdcall StrToInt_431080(const char* pStr, s16* pOut);

    CarInfo_2C* field_0_ptr_array[256]; // 0x0
    CarInfo_2C* field_400_raw_data; // 0x400
    ModelPhysics* field_404_model_physics_array[256]; // 0x404
    ModelPhysics* field_804_raw_data; // 0x804
};
GTA2_ASSERT_SIZEOF_ALWAYS(CarInfoTable, 0x808);

EXTERN_GLOBAL(CarInfoTable*, gCarInfo_808_678098);

EXPORT s32 __stdcall ParseTokenAndPush_430C70(char_type* pStr);
