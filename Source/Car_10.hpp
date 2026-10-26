#pragma once

#include "Function.hpp"

class Ped;

class Car_Door
{
  public:
    // 9.6f 0x4341B0
    inline Ped* get_pObj_4341B0() const
    {
        return field_8_pObj;
    }

    EXPORT void AnimateOpening_439CD0(u32* pDeltaMask);
    EXPORT void AnimateClosing_439D40(u32* pDeltaMask);
    EXPORT void Service_439DA0(u32* pDeltaMask);
    EXPORT void Init_439E40(u8 door_idx);
    EXPORT void Open_439E60();
    EXPORT void Close_439EA0();
    EXPORT Car_Door(); // 447330
    EXPORT ~Car_Door(); // 447350

    // 9.6f 0x421360
    inline bool IsStateActive_421360() const
    {
        return field_4_state != 0 && field_4_state != 6;
    }

    // 9.6f 0x421340
    inline void ResetState_421340()
    {
        field_4_state = 0;
    }

    // Inlined in sound_obj::HandleCarDoorSounds_4182E0
    inline bool IsStartingToOpen() const
    {
        return field_4_state == 2 && field_0_animation_frame == 0 && field_1_frame_delay == 0;
    }

    // Inlined in sound_obj::HandleCarDoorSounds_4182E0
    inline bool IsStartingToClose() const
    {
        return field_4_state == 3 && field_0_animation_frame == 1;
    }

    // 9.6f inline 0x421380
    inline void set_ped_421380(Ped* pPed)
    {
        field_8_pObj = pPed;
    }

    s8 field_0_animation_frame;
    s8 field_1_frame_delay;
    s8 field_2_first_delta_idx;
    s8 field_3_unused;
    s32 field_4_state;
    Ped* field_8_pObj;
    u8 field_C_stay_open_timer;
    s8 field_D_unused;
    s8 field_E_unused;
    s8 field_F_unused;
};
GTA2_ASSERT_SIZEOF_ALWAYS(Car_Door, 0x10)