#pragma once

#include "Function.hpp"

class Ped;

class Car_Door
{
  public:
    // 9.6f 0x4341B0
    inline Ped* get_pObj() const
    {
        return mpObj;
    }

    EXPORT void AnimateOpening(u32* pDeltaMask);
    EXPORT void AnimateClosing(u32* pDeltaMask);
    EXPORT void Service(u32* pDeltaMask);
    EXPORT void Init(u8 door_idx);
    EXPORT void Open();
    EXPORT void Close();
    EXPORT Car_Door(); // 447330
    EXPORT ~Car_Door(); // 447350

    // 9.6f 0x421360
    inline bool IsStateActive() const
    {
        return mState != 0 && mState != 6;
    }

    // 9.6f 0x421340
    inline void ResetState()
    {
        mState = 0;
    }

    // Inlined in sound_obj::HandleCarDoorSounds
    inline bool IsStartingToOpen() const
    {
        return mState == 2 && mAnimationFrame == 0 && mFrameDelay == 0;
    }

    // Inlined in sound_obj::HandleCarDoorSounds
    inline bool IsStartingToClose() const
    {
        return mState == 3 && mAnimationFrame == 1;
    }

    // 9.6f inline 0x421380
    inline void set_ped(Ped* pPed)
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