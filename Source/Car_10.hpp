#pragma once

#include "Function.hpp"

class Ped;

class Car_Door
{
  public:
    // 9.6f 0x4341B0
    inline Ped* get_pObj_4341B0() const
    {
        return mpObj;
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
        return mState != 0 && mState != 6;
    }

    // 9.6f 0x421340
    inline void ResetState_421340()
    {
        mState = 0;
    }

    // Inlined in sound_obj::HandleCarDoorSounds_4182E0
    inline bool IsStartingToOpen() const
    {
        return mState == 2 && mAnimationFrame == 0 && mFrameDelay == 0;
    }

    // Inlined in sound_obj::HandleCarDoorSounds_4182E0
    inline bool IsStartingToClose() const
    {
        return mState == 3 && mAnimationFrame == 1;
    }

    // 9.6f inline 0x421380
    inline void set_ped_421380(Ped* pPed)
    {
        mpObj = pPed;
    }

    s8 mAnimationFrame; // 0x0
    s8 mFrameDelay; // 0x1
    s8 mFirstDeltaIdx; // 0x2
    s8 mUnused3; // 0x3
    s32 mState; // 0x4
    Ped* mpObj; // 0x8
    u8 mStayOpenTimer; // 0xC
    s8 mUnusedD; // 0xD
    s8 mUnusedE; // 0xE
    s8 mUnusedF; // 0xF
};
GTA2_ASSERT_SIZEOF_ALWAYS(Car_Door, 0x10)