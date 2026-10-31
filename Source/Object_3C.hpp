#pragma once

#include "Fix16_Point.hpp"
#include "Function.hpp"
#include "ang16.hpp"
#include "fix16.hpp"

EXTERN_GLOBAL(u32, gObj3C_live_count_6F8E54);
EXTERN_GLOBAL(u32, gObj3C_dealloc_count_6F8F0C);
EXTERN_GLOBAL(Ang16, kZeroAng_6F8F68);

class Sprite;
class AttachedSprite;
class Object;

class SpriteList
{
  public:
    SpriteList() // 424620 inline
    {
        ResetHead_4207E0();
    }

    void ResetHead_4207E0() // inline
    {
        mP18 = NULL;
    }

    EXPORT AttachedSprite* GetSpriteForModel_5A6A50(s32 obj_type) const;
    EXPORT Object* FindObject2CByModel_5A6A90(s32 obj_type) const;
    EXPORT AttachedSprite* FindFirstFireExplosion_5A6AD0() const;
    EXPORT void RemoveSprite_5A6B10(Sprite* toFind);
    EXPORT void RemoveSpriteSafe_5A6B60(Sprite* toFind);
    EXPORT void DestroyAllAttached_5A6BB0();
    EXPORT void SinkAllAttachedCars_5A6BD0();
    EXPORT void DispatchCarImpactEvents_5A6BF0(Sprite* pSprite);
    EXPORT bool TagSpriteWithRng_5A6C10(Sprite* toFind);
    EXPORT void RemoveAllExceptRngValue_5A6C40(s32 toFind);
    EXPORT Sprite* FirstSpriteOfType_5A6CA0(s32 sprite_type) const;
    EXPORT void AddSprite_5A6CD0(Sprite* pSprite);
    EXPORT void PushAttachedSprite_5A6D00(Sprite* pSprite, Fix16 x, Fix16 y, Ang16 angle);
    EXPORT void PushSprite_5A6D40(Sprite* pSprite);
    EXPORT bool SpriteExists_5A6D80(Sprite* pToFind) const;
    EXPORT Sprite* PopFrontSprite_5A6DA0();
    EXPORT Sprite* PopBackSprite_5A6DC0();
    EXPORT AttachedSprite* ClearList_5A6E10();
    EXPORT Sprite* FindClosestSprite_5A6E40(Fix16 xpos, Fix16 ypos) const;
    EXPORT Sprite* TakeClosestSprite_5A6EA0(Fix16 xpos, Fix16 ypos);

    EXPORT void PoolUpdate_5A6F70(Sprite* pSprite);
    EXPORT void DestroyAllSprites_5A7010();
    EXPORT void CleanupSpriteList_5A7080();
    EXPORT void ClearGangIconSprite_5A7110();
    EXPORT s32 GetGangIdxFromSpriteIfAny_5A71A0() const;
    EXPORT void ExpireAttachedFlames_5A71F0();
    EXPORT void PruneNonCollidingSprites_5A7240(Sprite* pSprite);
    EXPORT void PropagateMaxZLayer_5A72B0(Sprite* pSprite, bool bAlsoSetSprite);
    EXPORT bool AnySpriteHasSlopeTopInFootprint_5A7310() const;

  public:
    AttachedSprite* mP18; // 0x0
};
GTA2_ASSERT_SIZEOF_ALWAYS(SpriteList, 0x4);

class Object_3C
{
  public:
    EXPORT Object_3C(); // 0x52ad80
    EXPORT ~Object_3C(); // 0x52ade0

    // 9.6f 0x482B20
    void PoolAllocate()
    {
        ++gObj3C_live_count_6F8E54;
        mSpeed = 0;
        mAngle = kZeroAng_6F8F68;
        mFriction = Fix16(0);
        mNextDefinitionTimer = 0;
        mConveyorSpeed = 0;
        mSpriteCollideMode = 2;
        mUnused24 = 0;
        mbOnSlope = false;
        mbSkipAnim = false;
    }

    // 9.6f 0x482B80
    void PoolDeallocate()
    {
        --gObj3C_live_count_6F8E54;
        ++gObj3C_dealloc_count_6F8F0C;
    }

    void ClearConveyorSpeed_482BD0()
    {
        mConveyorSpeed = 0;
    }

    EXPORT Fix16_Point GetSpeedVector_52ADF0() const;

    // 9.6f 0x482BA0; GetSpeedVector_52ADF0 is its out-of-line copy. It returns a Fix16_Point (which has a
    // destructor), so VC6 never expands it: it is charged to the caller's inline budget and called out of
    // line (GetMovementSpeedAndAngle_521FD0)
    inline Fix16_Point GetSpeedVector_482BA0() const
    {
        Fix16_Point p;
        p.FromPolar_41E210(mSpeed, mAngle);
        return p;
    }

    EXPORT void GetMovementSpeedAndAngle_521FD0(Fix16& Speed, Ang16& Angle);

    SpriteList mAttachedSprites; // 0x0
    Ang16 mAngle; // 0x4
    s16 mUnused6; // 0x6
    Object_3C* mpNext;
    Fix16 mSpeed; // 0xC
    Fix16 mZSpeed; // 0x10
    Fix16 mSavedSpeed; // 0x14
    Fix16 mFriction; // 0x18
    Fix16 mZAccel; // 0x1C
    s32 mObj2cId; // 0x20
    s32 mUnused24; // 0x24
    s16 mNextDefinitionTimer; // 0x28
    s16 mbAirborne; // 0x2A
    s16 mUnused2C; // 0x2C
    char_type mUnused2E; // 0x2E
    char_type mbOnSlope; // 0x2F
    bool mbSkipAnim; // 0x30
    u8 mUnused31; // 0x31
    u16 mUnused32; // 0x32
    s32 mSpriteCollideMode; // 0x34: 2 collides with sprites, 1 passes over them (draw layer 29), 0 under (layer 6)
    char_type mConveyorSpeed; // 0x38
    char_type mUnused39; // 0x39
    char_type mUnused3A; // 0x3A
    char_type mUnused3B; // 0x3B
};

EXTERN_GLOBAL(SpriteList, gTrailerHitchCandidates_67727C);
