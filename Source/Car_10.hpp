#pragma once

#include "Function.hpp"

class Ped;

class Car_Door_10
{
  public:
    // 9.6f 0x4341B0
    inline Ped* get_pObj_4341B0()
    {
        return field_8_pObj;
    }

    EXPORT void sub_439CD0(u32* a2);
    EXPORT void sub_439D40(u32* a3);
    EXPORT void sub_439DA0(u32* a3);
    EXPORT void sub_439E40(u8 a2);
    EXPORT void sub_439E60();
    EXPORT void sub_439EA0();
    EXPORT Car_Door_10(); // 447330
    EXPORT ~Car_Door_10(); // 447350

    // 9.6f 0x421360
    inline bool IsStateActive_421360()
    {
        return field_4_state != 0 && field_4_state != 6;
    }

    // 9.6f 0x421340
    inline void ResetState_421340()
    {
        field_4_state = 0;
    }

    // 9.6f inline 0x421380
    inline void set_ped_421380(Ped* pPed)
    {
        field_8_pObj = pPed;
    }

    s8 field_0_animation_frame;
    s8 field_1;
    s8 field_2;
    s8 field_3;
    s32 field_4_state;
    Ped* field_8_pObj;
    u8 field_C;
    s8 field_D;
    s8 field_E;
    s8 field_F;
};
GTA2_ASSERT_SIZEOF_ALWAYS(Car_Door_10, 0x10)