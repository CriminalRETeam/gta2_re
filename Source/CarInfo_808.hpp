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

struct UnknownList
{
    Fix16 sub_41FE70(s8& var)
    {
        if (var < 0)
        {
            return -list[-var];
        }
        return list[var];
    }

    EXPORT Fix16 sub_440860(s8& var);

    Fix16 list[256];
};

MATCH_FUNC(0x440860)
Fix16 UnknownList::sub_440860(s8& var)
{
    if (var < 0)
    {
        return -list[-var];
    }
    return list[var];
}

EXTERN_GLOBAL(UnknownList, dword_6F6850);

class ModelPhysics_48
{
  public:
    u8 field_0_model;
    s8 field_1_turbo;
    u8 field_2_value;
    s8 field_3_padding;
    Fix16 field_4_mass;
    Fix16 field_8_front_drive_bias;
    Fix16 field_C_front_mass_bias;
    Fix16 field_10_brake_friction;
    Fix16 field_14_turn_in;
    Fix16 field_18_turn_ratio;
    Fix16 field_1C_rear_end_stability;
    Fix16 field_20_handbrake_slide_value;
    Fix16 field_24_thrust;
    Fix16 field_28_max_speed;
    Fix16 field_2C_anti_strngth;
    Fix16 field_30_sked_threshold;
    Fix16 field_34_gear1_multiplier;
    Fix16 field_38_gear2_multiplier;
    Fix16 field_3C_gear3_multiplier;
    Fix16 field_40_gear2_speed;
    Fix16 field_44_gear3_speed;

    Fix16 get_anti_strngth()
    {
        return field_2C_anti_strngth;
    }

    EXPORT void sub_454680();
};
GTA2_ASSERT_SIZEOF_ALWAYS(ModelPhysics_48, 0x48);

class CarInfo_2C
{
  public:
    EXPORT CarInfo_2C();
    EXPORT ~CarInfo_2C();
    EXPORT void sub_4542A0(s32 idx);

    Fix16 field_0;
    s32 field_4_front_wheel_offset;
    Fix16 field_8_rear_wheel_offset;
    Fix16_Point field_C;
    Fix16 field_14;
    Fix16 field_18;
    s32 field_1C;
    Fix16 field_20_front_drive_bias;
    Fix16 field_24_skid_threshhold_1;
    Fix16 field_28_skid_threshhold_2;
};
GTA2_ASSERT_SIZEOF_ALWAYS(CarInfo_2C, 0x2C);

class CarInfo_808
{
  public:
    EXPORT ModelPhysics_48* GetModelPhysicsFromIdx_4546B0(u8 model_idx);
    EXPORT void sub_4546D0();
    EXPORT CarInfo_2C* sub_454840(u8 idx);
    EXPORT void sub_454850();
    EXPORT void sub_4549C0();
    EXPORT void sub_454A00(const char_type* pGciFilePath);
    EXPORT void sub_454A50();
    EXPORT void sub_454A80();
    EXPORT void sub_454AA0();
    EXPORT CarInfo_808();
    EXPORT ~CarInfo_808();

    EXPORT static char* __stdcall parse_gci_file_430A30(void* input,
                                                        size_t input_size,
                                                        ModelPhysics_48* output,
                                                        size_t output_size,
                                                        u32* next_position);
    EXPORT static char* __stdcall SetErr_430AC0(s32 a1);
    EXPORT static s32 __stdcall sub_430b10(char* param_1);
    EXPORT static s32 __stdcall sub_430E60(void* param_1, u32 param_2);
    EXPORT static s32 __stdcall HexStr2Int_430EC0(const char* param_1, s32* param_2);
    EXPORT static s32 __stdcall HexStr2Int_430F30(const char* param_1, s16* param_2);
    EXPORT static s32 __stdcall StrToInt_430FA0(const char* param_1, s32* param_2);
    EXPORT static s32 __stdcall FloatStrToFix16_431000(char* param_1, Fix16& param_2);
    EXPORT static s32 __stdcall StrToInt_431080(const char* param_1, s16* param_2);

    CarInfo_2C* field_0_ptr_array[256];
    CarInfo_2C* field_400_raw_data;
    ModelPhysics_48* field_404_model_physics_array[256];
    ModelPhysics_48* field_804_raw_data;
};
GTA2_ASSERT_SIZEOF_ALWAYS(CarInfo_808, 0x808);

EXTERN_GLOBAL(CarInfo_808*, gCarInfo_808_678098);

EXPORT s32 __stdcall sub_430C70(char_type* a1);
