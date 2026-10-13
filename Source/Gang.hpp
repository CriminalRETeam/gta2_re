#pragma once

#include "Function.hpp"
#include "fix16.hpp"
#include <wchar.h>

class gmp_map_zone;

class Gang_144
{
  public:
    // 9.6f 0x45DD50
    inline bool HasKillRespectChange_45DD50()
    {
        return field_139_kill_respect_change > 0;
    }

    // 9.6f 0x433B30
    inline bool DoesKillAffectRespect_433B30()
    {
        return field_111_kill_affects_respect;
    }

    // 9.6f 0x4758A0
    inline void SetRemap_4758A0(char v)
    {
        field_101_remap = v;
    }

    // 9.6f 0x4758B0
    inline void SetBasicWeapon_4758B0(u32 v)
    {
        field_104_basic_weapon = v;
    }

    // 9.6f 0x4758C0
    inline void SetAngryWeapon_4758C0(u32 v)
    {
        field_108_angry_weapon = v;
    }

    // 9.6f 0x4758D0
    inline void SetHateWeapon_4758D0(u32 v)
    {
        field_10C_hate_weapon = v;
    }

    // 9.6f 0x4758E0
    inline void SetGangCarModel_4758E0(u32 v)
    {
        field_13C_gang_car_model = v;
    }

    // 9.6f 0x4758F0
    inline void SetGangCarRemap_4758F0(char v)
    {
        field_140_gang_car_remap = v;
    }

    // 9.6f 0x475900
    inline void SetKillAffectsRespect_475900(char v)
    {
        field_111_kill_affects_respect = v;
    }

    // 9.6f 0x475910
    inline void SetInfoPhonePos_475910(Fix16 x, Fix16 y, Fix16 z)
    {
        field_12C_info_phone_x = x;
        field_130_info_phone_y = y;
        field_134_info_phone_z = z;
    }

    // 9.6f 0x475940
    inline void SetArrowColour_475940(u8 v)
    {
        field_138_arrow_colour = v;
    }

    // 9.6f 0x475950
    inline void SetKillRespectChange_475950(u8 v)
    {
        field_139_kill_respect_change = v;
    }

    EXPORT Gang_144(); //  0x4BE4E0
    EXPORT ~Gang_144(); // 0x4BE4F0

    EXPORT wchar_t* GetNameWide_4BED30();
    EXPORT void Init_4BED70();
    EXPORT char_type IsHostileToGang_4BEDF0(u8 gang_idx);
    EXPORT void SetRespect_4BEE30(u8 player_idx, char_type respect);
    EXPORT void IncrementRespect_4BEE50(u8 player_idx, char_type respect);
    EXPORT void DecrementRespect_4BEEA0(u8 player_idx, char_type respect);
    EXPORT char_type GetRespectForPlayer_4BEEF0(u8 player_idx);
    EXPORT bool IsRespectNegativeForPlayer_4BEF10(u8 player_idx);
    EXPORT void SetGangKillReaction_4BEF50(u8 gang_idx, char_type kill_reaction_value);
    EXPORT void ApplyKillRespectChange_4BEF70(u8 player_idx, u8 respect);
    EXPORT void ChangeRespectAndUpdate_4BF000(u8 player_idx, char_type respect);
    EXPORT s32 GetCurrentWeapon_4BF0C0();
    EXPORT void SetName_4BF090(const char_type* pName, u8 nameLen);
    EXPORT wchar_t* GetArrowColourText_4BF340();

    u8 field_0_used;
    u8 field_1_gang_idx;
    char field_2_name[4];
    char field_6_unused;
    char field_7_unused;
    u32 field_8_unused[62];
    char field_100_unused;
    char field_101_remap;
    char field_102_unused;
    char field_103_unused;
    u32 field_104_basic_weapon;
    u32 field_108_angry_weapon;
    u32 field_10C_hate_weapon;
    char field_110_has_high_respect;
    char field_111_kill_affects_respect;
    char field_112_hostile_to_gang[10];
    s8 field_11C_respect[MAX_PLAYERS];
    char field_122_gang_kill_reaction[10];
    Fix16 field_12C_info_phone_x;
    Fix16 field_130_info_phone_y;
    Fix16 field_134_info_phone_z;
    u8 field_138_arrow_colour;
    u8 field_139_kill_respect_change;
    char field_13A_unused;
    char field_13B_unused;
    u32 field_13C_gang_car_model;
    char field_140_gang_car_remap;
    char field_141_always_basic_weapon;
    char field_142_unused;
    char field_143_unused;
};

class GangPool_CA8
{
  public:
    GangPool_CA8()
    {
    }

    EXPORT ~GangPool_CA8();
    EXPORT Gang_144* FirstGang_4BECA0();
    EXPORT Gang_144* NextGang_4BECE0();
    EXPORT Gang_144* FindGangByName_4BF100(const char* pGangName);
    EXPORT Gang_144* AllocGang_4BF170();
    EXPORT Gang_144* GangByIdx_4BF1C0(u8 gang_idx);
    EXPORT void AddGangForMapZone_4BF1E0(gmp_map_zone* pMapZone);
    EXPORT s8 GetGangIdxByName_4BF210(const char* gangName);
    EXPORT void SwapGangSlots_4BF230(Gang_144* pGang, u8 gang_idx);
    EXPORT s8 FindGangByCarModel_4BF2F0(s32 car_model);

    Gang_144 field_0_gang_list[10];
};

EXTERN_GLOBAL(GangPool_CA8*, gGangPool_CA8_67E274);
