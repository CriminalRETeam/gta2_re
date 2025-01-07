#pragma once

#include "Function.hpp"
#include "fix16.hpp"

class Monster_48
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
};
GTA2_ASSERT_SIZEOF_ALWAYS(Monster_48, 0x48);

class Monster_2C
{
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
GTA2_ASSERT_SIZEOF_ALWAYS(Monster_2C, 0x2C);

class Monster_808
{
  public:
    EXPORT Monster_48* sub_4546B0(u8 a2);
    EXPORT void sub_4546D0();
    EXPORT Monster_2C* sub_454840(u8 idx);
    EXPORT s32 sub_454850();
    EXPORT void sub_4549C0();
    EXPORT s32 sub_454A00(const char_type* pGciFilePath);
    EXPORT s32 sub_454A50();
    EXPORT s32 Monster_808::sub_454A80(Monster_808* a1);
    EXPORT void sub_454AA0();
    EXPORT Monster_808();
    EXPORT ~Monster_808();

    Monster_2C* field_0_ptr_array[256];
    Monster_2C* field_400_raw_data;
    Monster_48* field_404_ptr_array[256];
    Monster_48* field_804_raw_data;
};
GTA2_ASSERT_SIZEOF_ALWAYS(Monster_808, 0x808);
