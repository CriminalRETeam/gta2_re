#pragma once

#include "Function.hpp"

class Ped;

class Car_Door_10
{
  public:
    EXPORT void AnimateOpening_439CD0(u32* a2);
    EXPORT void AnimateClosing_439D40(u32* a3);
    EXPORT void Service_439DA0(u32* a3);
    EXPORT void Init_439E40(u8 a2);
    EXPORT void Open_439E60();
    EXPORT void Close_439EA0();
    EXPORT Car_Door_10(); // 447330
    EXPORT ~Car_Door_10(); // 447350

    // 9.6f inline 0x421380
    inline void set_ped_421380(Ped* pPed)
    {
        field_8_pObj = pPed;
    }

    s8 field_0_animation_frame;
    s8 field_1_frame_delay;
    s8 field_2_first_delta_idx;
    s8 field_3;
    s32 field_4_state;
    Ped* field_8_pObj;
    u8 field_C_stay_open_timer;
    s8 field_D;
    s8 field_E;
    s8 field_F;
};
GTA2_ASSERT_SIZEOF_ALWAYS(Car_Door_10, 0x10)