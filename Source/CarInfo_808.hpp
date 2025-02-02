#pragma once

#include "Function.hpp"
#include "fix16.hpp"

class CarInfo_48
{
  public:
    u8 field_0_model;
    s8 field_1_turbo;
    s8 field_2_value;
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

    EXPORT void sub_454680();
};
GTA2_ASSERT_SIZEOF_ALWAYS(CarInfo_48, 0x48);

class CarInfo_2C
{
  public:
    EXPORT CarInfo_2C();
    EXPORT ~CarInfo_2C();
    EXPORT void sub_4542A0(s32 idx);

    s32 field_0;
    s32 field_4_front_wheel_offset;
    s32 field_8_rear_wheel_offset;
    s32 field_C;
    s32 field_10;
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
    EXPORT CarInfo_48* sub_4546B0(u8 a2);
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

    static char* __stdcall parse_gci_file_430A30(void* input,
                                                 size_t input_size,
                                                 CarInfo_48* output,
                                                 size_t output_size,
                                                 u32* next_position);
    static s32 __stdcall sub_430b10(char* param_1);
    static s32 __stdcall sub_430E60(void* param_1, u32 param_2);
    static s32 __stdcall HexStr2Int_430EC0(const char* param_1, s32* param_2);
    static s32 __stdcall HexStr2Int_430F30(const char* param_1, s16* param_2);
    static s32 __stdcall StrToInt_430FA0(const char* param_1, s32* param_2);
    static s32 __stdcall FloatStrToFix16_431000(char* param_1, Fix16& param_2);
    static s32 __stdcall StrToInt_431080(const char* param_1, s16* param_2);

    CarInfo_2C* field_0_ptr_array[256];
    CarInfo_2C* field_400_raw_data;
    CarInfo_48* field_404_ptr_array[256];
    CarInfo_48* field_804_raw_data;
};
GTA2_ASSERT_SIZEOF_ALWAYS(CarInfo_808, 0x808);

extern EXPORT_VAR CarInfo_808* gMonster_808_678098;
