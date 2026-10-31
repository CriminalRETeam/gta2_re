#pragma once

#include "Function.hpp"
#include "Game.hpp"
#include "fix16.hpp"
#include <wchar.h>

class gmp_map_zone;

class Gang
{
  public:
    // 9.6f 0x45DD50
    inline bool HasKillRespectChange_45DD50() const
    {
        return mKillRespectChange > 0;
    }

    // 9.6f 0x433B30
    inline bool DoesKillAffectRespect_433B30() const
    {
        return mKillAffectsRespect;
    }

    // 9.6f 0x4758A0
    inline void SetRemap_4758A0(char_type v)
    {
        mRemap = v;
    }

    // 9.6f 0x4758B0
    inline void SetBasicWeapon_4758B0(u32 v)
    {
        mBasicWeapon = v;
    }

    // 9.6f 0x4758C0
    inline void SetAngryWeapon_4758C0(u32 v)
    {
        mAngryWeapon = v;
    }

    // 9.6f 0x4758D0
    inline void SetHateWeapon_4758D0(u32 v)
    {
        mHateWeapon = v;
    }

    // 9.6f 0x4758E0
    inline void SetGangCarModel_4758E0(u32 v)
    {
        mGangCarModel = v;
    }

    // 9.6f 0x4758F0
    inline void SetGangCarRemap_4758F0(char_type v)
    {
        mGangCarRemap = v;
    }

    // 9.6f 0x475900
    inline void SetKillAffectsRespect_475900(bool v)
    {
        mKillAffectsRespect = v;
    }

    // 9.6f 0x475910
    inline void SetInfoPhonePos_475910(Fix16 x, Fix16 y, Fix16 z)
    {
        mInfoPhoneX = x;
        mInfoPhoneY = y;
        mInfoPhoneZ = z;
    }

    // 9.6f 0x475940
    inline void SetArrowColour_475940(u8 v)
    {
        mArrowColour = v;
    }

    // 9.6f 0x475950
    inline void SetKillRespectChange_475950(u8 v)
    {
        mKillRespectChange = v;
    }

    EXPORT Gang(); //  0x4BE4E0
    EXPORT ~Gang(); // 0x4BE4F0

    EXPORT wchar_t* GetNameWide_4BED30() const;
    EXPORT void Init_4BED70();
    EXPORT bool IsHostileToGang_4BEDF0(u8 gang_idx) const;
    EXPORT void SetRespect_4BEE30(u8 player_idx, char_type respect);
    EXPORT void IncrementRespect_4BEE50(u8 player_idx, char_type respect);
    EXPORT void DecrementRespect_4BEEA0(u8 player_idx, char_type respect);
    EXPORT char_type GetRespectForPlayer_4BEEF0(u8 player_idx) const;
    EXPORT bool IsRespectNegativeForPlayer_4BEF10(u8 player_idx) const;
    EXPORT void SetGangKillReaction_4BEF50(u8 gang_idx, char_type kill_reaction_value);
    EXPORT void ApplyKillRespectChange_4BEF70(u8 player_idx, u8 respect);
    EXPORT void ChangeRespectAndUpdate_4BF000(u8 player_idx, char_type respect);
    EXPORT s32 GetCurrentWeapon_4BF0C0() const;
    EXPORT void SetName_4BF090(const char_type* pName, u8 nameLen);
    EXPORT wchar_t* GetArrowColourText_4BF340() const;

    bool mUsed; // 0x0
    u8 mGangIdx; // 0x1
    char_type mName[4]; // 0x2
    char_type mUnused6; // 0x6
    char_type mUnused7; // 0x7
    u32 mUnused8[62]; // 0x8
    char_type mUnused100; // 0x100
    char_type mRemap; // 0x101
    char_type mUnused102; // 0x102
    char_type mUnused103; // 0x103
    u32 mBasicWeapon; // 0x104
    u32 mAngryWeapon; // 0x108
    u32 mHateWeapon; // 0x10C
    bool mHasHighRespect; // 0x110
    bool mKillAffectsRespect; // 0x111
    bool mHostileToGang[10]; // 0x112
    s8 mRespect[MAX_PLAYERS]; // 0x11C
    char_type mGangKillReaction[10]; // 0x122
    Fix16 mInfoPhoneX; // 0x12C
    Fix16 mInfoPhoneY; // 0x130
    Fix16 mInfoPhoneZ; // 0x134
    u8 mArrowColour; // 0x138
    u8 mKillRespectChange; // 0x139
    char_type mUnused13A; // 0x13A
    char_type mUnused13B; // 0x13B
    u32 mGangCarModel; // 0x13C
    char_type mGangCarRemap; // 0x140
    bool mAlwaysBasicWeapon; // 0x141
    char_type mUnused142; // 0x142
    char_type mUnused143; // 0x143
};
GTA2_ASSERT_SIZEOF_ALWAYS(Gang, 0x144);

class GangPool
{
  public:
    STATIC_MEMBER(s32, sGangIdx);
    GangPool()
    {
    }

    EXPORT ~GangPool();
    EXPORT Gang* FirstGang_4BECA0();
    EXPORT Gang* NextGang_4BECE0();
    EXPORT Gang* FindGangByName_4BF100(const char_type* pGangName);
    EXPORT Gang* AllocGang_4BF170();
    EXPORT Gang* GangByIdx_4BF1C0(u8 gang_idx);
    EXPORT void AddGangForMapZone_4BF1E0(gmp_map_zone* pMapZone);
    EXPORT s8 GetGangIdxByName_4BF210(const char_type* pGangName);
    EXPORT void SwapGangSlots_4BF230(Gang* pGang, u8 gang_idx);
    EXPORT s8 FindGangByCarModel_4BF2F0(s32 car_model);

    Gang mGangList[10]; // 0x0
};
GTA2_ASSERT_SIZEOF_ALWAYS(GangPool, 0xCA8);

EXTERN_GLOBAL(GangPool*, gGangPool_CA8_67E274);
