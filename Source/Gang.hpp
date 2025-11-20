#pragma once

#include "Function.hpp"
#include "fix16.hpp"
#include <wchar.h>

class gmp_map_zone;

class Gang_144
{
  public:
    EXPORT Gang_144(); //  0x4BE4E0
    EXPORT ~Gang_144(); // 0x4BE4F0

    EXPORT wchar_t* get_name_wide_4BED30();
    EXPORT void init_4BED70();
    EXPORT char_type sub_4BEDF0(u8 gang_idx);
    EXPORT void SetRespect_4BEE30(u8 player_idx, char_type respect);
    EXPORT void IncrementRespect_4BEE50(u8 player_idx, char_type respect);
    EXPORT void DecrementRespect_4BEEA0(u8 player_idx, char_type respect);
    EXPORT char_type GetRespectForPlayer_4BEEF0(u8 player_idx);
    EXPORT bool IsRespectNegativeForPlayer_4BEF10(u8 player_idx);
    EXPORT void SetGangKillReaction_4BEF50(u8 gang_idx, char_type kill_reaction_value);
    EXPORT char_type sub_4BEF70(u8 a2, u8 a3);
    EXPORT void ChangeRespectAndUpdate_4BF000(u8 player_idx, char_type respect);
    EXPORT s32 GetGangCurrWeapon_4BF0C0();
    EXPORT void set_name_4BF090(const char_type* pName, u8 nameLen);
    EXPORT wchar_t* sub_4BF340();

    u8 field_0_used;
    u8 field_1_gang_idx;
    char field_2_name[4];
    char field_6;
    char field_7;
    u32 field_8;
    u32 field_C;
    u32 field_10;
    u32 field_14;
    u32 field_18;
    u32 field_1C;
    u32 field_20;
    u32 field_24;
    u32 field_28;
    u32 field_2C;
    u32 field_30;
    u32 field_34;
    u32 field_38;
    u32 field_3C;
    u32 field_40;
    u32 field_44;
    u32 field_48;
    u32 field_4C;
    u32 field_50;
    u32 field_54;
    u32 field_58;
    u32 field_5C;
    u32 field_60;
    u32 field_64;
    u32 field_68;
    u32 field_6C;
    u32 field_70;
    u32 field_74;
    u32 field_78;
    u32 field_7C;
    u32 field_80;
    u32 field_84;
    u32 field_88;
    u32 field_8C;
    u32 field_90;
    u32 field_94;
    u32 field_98;
    u32 field_9C;
    u32 field_A0;
    u32 field_A4;
    u32 field_A8;
    u32 field_AC;
    u32 field_B0;
    u32 field_B4;
    u32 field_B8;
    u32 field_BC;
    u32 field_C0;
    u32 field_C4;
    u32 field_C8;
    u32 field_CC;
    u32 field_D0;
    u32 field_D4;
    u32 field_D8;
    u32 field_DC;
    u32 field_E0;
    u32 field_E4;
    u32 field_E8;
    u32 field_EC;
    u32 field_F0;
    u32 field_F4;
    u32 field_F8;
    u32 field_FC;
    char field_100;
    char field_101;
    char field_102;
    char field_103;
    u32 field_104_basic_weapon;
    u32 field_108_angry_weapon;
    u32 field_10C_hate_weapon;
    char field_110;
    char field_111;
    char field_112[10];
    s8 field_11C_respect[6];  // size: max num of players
    char field_122_gang_kill_reaction[10];
    Fix16 field_12C_info_phone_x;
    Fix16 field_130_info_phone_y;
    Fix16 field_134_info_phone_z;
    u8 field_138_arrow_colour;
    u8 field_139_kill_respect_change;
    char field_13A;
    char field_13B;
    u32 field_13C_gang_car_model;
    char field_140_gang_car_remap;
    char field_141;
    char field_142;
    char field_143;
};

class GangPool_CA8
{
  public:
    GangPool_CA8()
    {
    }

    EXPORT ~GangPool_CA8();
    EXPORT Gang_144* sub_4BECA0();
    EXPORT Gang_144* sub_4BECE0();
    EXPORT Gang_144* gang_by_name_4BF100(const char* pGangName);
    EXPORT Gang_144* next_free_gang_slot_4BF170();
    EXPORT Gang_144* GangByIdx_4BF1C0(u8 zone_idx);
    EXPORT void alloc_gang_for_map_zone_4BF1E0(gmp_map_zone* pMapZone);
    EXPORT u8 get_gang_idx_by_name_4BF210(const char* gangName);
    EXPORT void sub_4BF230(Gang_144* pGang, u8 gang_idx);
    EXPORT s8 FindGangByCarModel_4BF2F0(s32 car_model);

    Gang_144 field_0_gang_list[10];
};

EXTERN_GLOBAL(GangPool_CA8*, gGangPool_CA8_67E274);
