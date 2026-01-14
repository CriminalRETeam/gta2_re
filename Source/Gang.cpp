#include "Gang.hpp"
#include "Function.hpp"
#include "Globals.hpp"
#include "error.hpp"
#include "map_0x370.hpp"
#include "text_0x14.hpp"
#include <string.h>

DEFINE_GLOBAL(GangPool_CA8*, gGangPool_CA8_67E274, 0x67E274);
DEFINE_GLOBAL_INIT(s32, gGangIdx_6206B8, -1, 0x6206B8);

DEFINE_GLOBAL_ARRAY(wchar_t, gGangNameWide_67E030, 4, 0x67E030);

EXTERN_GLOBAL(s32, bStartNetworkGame_7081F0);

MATCH_FUNC(0x4BE4E0);
Gang_144::Gang_144()
{
    init_4BED70();
}

MATCH_FUNC(0x4BE4F0);
Gang_144::~Gang_144()
{
}

MATCH_FUNC(0x4BED30)
wchar_t* Gang_144::get_name_wide_4BED30()
{
    gGangNameWide_67E030[0] = field_2_name[0];
    gGangNameWide_67E030[1] = field_2_name[1];
    gGangNameWide_67E030[2] = field_2_name[2];
    gGangNameWide_67E030[3] = 0;
    return gGangNameWide_67E030;
}

MATCH_FUNC(0x4BED70);
void Gang_144::init_4BED70()
{
    field_1_gang_idx = 0;
    field_0_used = 0;
    field_101 = 1;
    field_110 = 0;
    field_111 = 0;
    field_104_basic_weapon = 0;
    field_108_angry_weapon = 0;
    field_10C_hate_weapon = 0;
    field_13C_gang_car_model = 1;
    field_140_gang_car_remap = -1;
    field_139_kill_respect_change = 0;

    for (u8 i = 0; i < 10; i++)
    {
        field_112[i] = 1;
        field_122_gang_kill_reaction[i] = 0;
    }

    for (u8 j = 0; j < 6; j++)
    {
        field_11C_respect[j] = 0;
    }
    field_141 = 0;
}

MATCH_FUNC(0x4BEDF0)
char_type Gang_144::sub_4BEDF0(u8 gang_idx)
{
    return field_112[gang_idx];
}

MATCH_FUNC(0x4BEE30)
void Gang_144::SetRespect_4BEE30(u8 player_idx, char_type respect)
{
    field_11C_respect[player_idx] = respect;
}

MATCH_FUNC(0x4BEE50)
void Gang_144::IncrementRespect_4BEE50(u8 player_idx, char_type respect)
{
    const s8 new_attr = field_11C_respect[player_idx] + respect;
    if (new_attr < field_11C_respect[player_idx])
    {
        field_11C_respect[player_idx] = 100;
    }
    else
    {
        field_11C_respect[player_idx] = new_attr;
        if (new_attr > 100)
        {
            field_11C_respect[player_idx] = 100;
        }
    }

    if (field_11C_respect[player_idx] >= 80)
    {
        field_110 = true;
    }
    else
    {
        field_110 = false;
    }
}

MATCH_FUNC(0x4BEEA0)
void Gang_144::DecrementRespect_4BEEA0(u8 player_idx, char_type respect)
{
    s8 diff = field_11C_respect[player_idx] - respect;
    if (diff > field_11C_respect[player_idx] || (field_11C_respect[player_idx] = diff) < -100)
    {
        field_11C_respect[player_idx] = -100;
    }

    if (field_11C_respect[player_idx] >= 80)
    {
        field_110 = true;
    }
    else
    {
        field_110 = false;
    }
}

MATCH_FUNC(0x4BEEF0)
char_type Gang_144::GetRespectForPlayer_4BEEF0(u8 player_idx)
{
    return field_11C_respect[player_idx];
}

MATCH_FUNC(0x4BEF10)
bool Gang_144::IsRespectNegativeForPlayer_4BEF10(u8 player_idx)
{
    if (field_11C_respect[player_idx] < -19)
    {
        return true;
    }
    return false;
}

MATCH_FUNC(0x4BEF50)
void Gang_144::SetGangKillReaction_4BEF50(u8 gang_idx, char_type kill_reaction_value)
{
    field_122_gang_kill_reaction[gang_idx] = kill_reaction_value;
}

STUB_FUNC(0x4BEF70)
char_type Gang_144::sub_4BEF70(u8 a2, u8 a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x4BF000)
void Gang_144::ChangeRespectAndUpdate_4BF000(u8 player_idx, char_type respect)
{
    if (respect > 0)
    {
        IncrementRespect_4BEE50(player_idx, respect);
    }
    else
    {
        DecrementRespect_4BEEA0(player_idx, abs(respect));
    }

    for (u8 i = 0; i < 10; ++i)
    {
        Gang_144* pGangFromIdx = gGangPool_CA8_67E274->GangByIdx_4BF1C0(i);
        if (field_1_gang_idx != pGangFromIdx->field_1_gang_idx)
        {
            if (pGangFromIdx->field_122_gang_kill_reaction[field_1_gang_idx])
            {
                if (respect > 0)
                {
                    pGangFromIdx->DecrementRespect_4BEEA0(player_idx, respect);
                }
                else
                {
                    pGangFromIdx->IncrementRespect_4BEE50(player_idx, abs(respect));
                }
            }
        }
    }
}

MATCH_FUNC(0x4BF0C0);
s32 Gang_144::GetGangCurrWeapon_4BF0C0()
{
    if (bStartNetworkGame_7081F0 || field_141)
    {
        return field_104_basic_weapon;
    }

    if (field_11C_respect[0] == -100)
    {
        return field_10C_hate_weapon;
    }

    if (field_11C_respect[0] > -60)
    {
        return field_104_basic_weapon;
    }
    else
    {
        return field_108_angry_weapon;
    }
}

MATCH_FUNC(0x4BF090);
void Gang_144::set_name_4BF090(const char_type* pName, u8 nameLen)
{
    strncpy(field_2_name, pName, nameLen);
    field_2_name[nameLen] = 0; // NULL terminate the string
}

MATCH_FUNC(0x4BF340)
wchar_t* Gang_144::sub_4BF340()
{
    sprintf(gTmpBuffer_67C598, "ganga%d", field_138_arrow_colour);
    return gText_0x14_704DFC->Find_5B5F90(gTmpBuffer_67C598);
}

// =====================

MATCH_FUNC(0x4be500)
GangPool_CA8::~GangPool_CA8()
{
}

MATCH_FUNC(0x4beca0)
Gang_144* GangPool_CA8::sub_4BECA0()
{
    for (gGangIdx_6206B8 = 0; gGangIdx_6206B8 < GTA2_COUNTOF_S(field_0_gang_list); gGangIdx_6206B8++)
    {
        if (field_0_gang_list[gGangIdx_6206B8].field_0_used && field_0_gang_list[gGangIdx_6206B8].field_139_kill_respect_change > 0)
        {
            return &field_0_gang_list[gGangIdx_6206B8];
        }
    }
    return 0;
}

MATCH_FUNC(0x4bece0)
Gang_144* GangPool_CA8::sub_4BECE0()
{
    while (++gGangIdx_6206B8 < GTA2_COUNTOF_S(field_0_gang_list))
    {
        if (field_0_gang_list[gGangIdx_6206B8].field_0_used && field_0_gang_list[gGangIdx_6206B8].field_139_kill_respect_change > 0)
        {
            return &field_0_gang_list[gGangIdx_6206B8];
        }
    }
    return 0;
}

MATCH_FUNC(0x4BF100);
Gang_144* GangPool_CA8::gang_by_name_4BF100(const char* pGangName)
{
    for (u8 i = 0; i < 10; i++)
    {
        if (_strnicmp(pGangName, field_0_gang_list[i].field_2_name, 4u) == 0)
        {
            return &field_0_gang_list[i];
        }
    }
    return 0;
}

MATCH_FUNC(0x4BF170);
Gang_144* GangPool_CA8::next_free_gang_slot_4BF170()
{
    for (u8 i = 0; i < GTA2_COUNTOF(field_0_gang_list); i++)
    {
        if (!field_0_gang_list[i].field_0_used)
        {
            Gang_144* result = &field_0_gang_list[i];
            result->field_1_gang_idx = i;
            result->field_0_used = 1;
            return result;
        }
    }
    return NULL;
}

MATCH_FUNC(0x4BF1C0);
Gang_144* GangPool_CA8::GangByIdx_4BF1C0(u8 zone_idx)
{
    return &field_0_gang_list[zone_idx];
}

MATCH_FUNC(0x4BF1E0);
void GangPool_CA8::alloc_gang_for_map_zone_4BF1E0(gmp_map_zone* pMapZone)
{
    if (!gang_by_name_4BF100(pMapZone->field_6_name))
    {
        Gang_144* pFreeZone = next_free_gang_slot_4BF170();
        pFreeZone->set_name_4BF090(pMapZone->field_6_name, pMapZone->field_5_name_length);
    }
}

MATCH_FUNC(0x4BF210);
u8 GangPool_CA8::get_gang_idx_by_name_4BF210(const char* gangName)
{
    Gang_144* pZone = gang_by_name_4BF100(gangName);
    if (pZone)
    {
        return pZone->field_1_gang_idx;
    }
    else
    {
        return -1;
    }
}

WIP_FUNC(0x4BF230);
void GangPool_CA8::sub_4BF230(Gang_144* pGang, u8 gang_idx)
{
    WIP_IMPLEMENTED;
    Gang_144 zone;
    zone.init_4BED70();
    if (pGang->field_1_gang_idx != gang_idx)
    {
        Gang_144* pZoneByIdx = GangByIdx_4BF1C0(gang_idx);
        if (pZoneByIdx->field_0_used)
        {
            if (pZoneByIdx->field_138_arrow_colour != pGang->field_138_arrow_colour)
            {
                memcpy(&zone, pZoneByIdx, sizeof(zone));
                memcpy(&gGangPool_CA8_67E274->field_0_gang_list[pZoneByIdx->field_1_gang_idx],
                       pGang,
                       sizeof(gGangPool_CA8_67E274->field_0_gang_list[pZoneByIdx->field_1_gang_idx]));
                memcpy(&gGangPool_CA8_67E274->field_0_gang_list[pGang->field_1_gang_idx],
                       &zone,
                       sizeof(gGangPool_CA8_67E274->field_0_gang_list[pGang->field_1_gang_idx]));
                pGang->field_1_gang_idx = pZoneByIdx->field_1_gang_idx;
                gGangPool_CA8_67E274->field_0_gang_list[gang_idx].field_1_gang_idx = gang_idx;
            }
        }
    }
}

MATCH_FUNC(0x4BF2F0);
s8 GangPool_CA8::FindGangByCarModel_4BF2F0(s32 car_model)
{
    for (u8 i = 0; i < 10; i++)
    {
        if (GangByIdx_4BF1C0(i)->field_13C_gang_car_model == car_model)
        {
            return i;
        }
    }
    return -1;
}