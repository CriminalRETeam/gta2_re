#include "Gang.hpp"
#include "Function.hpp"
#include "Globals.hpp"
#include "error.hpp"
#include "map_0x370.hpp"
#include "TextTable.hpp"
#include <string.h>

DEFINE_GLOBAL(GangPool*, gGangPool_CA8_67E274, 0x67E274);
DEFINE_GLOBAL_INIT(s32, gGangIdx_6206B8, -1, 0x6206B8);

DEFINE_GLOBAL_ARRAY(wchar_t, gGangNameWide_67E030, 4, 0x67E030);

EXTERN_GLOBAL(s32, bStartNetworkGame_7081F0);

MATCH_FUNC(0x4BE4E0);
Gang::Gang()
{
    Init_4BED70();
}

MATCH_FUNC(0x4BE4F0);
Gang::~Gang()
{
}

MATCH_FUNC(0x4be500)
GangPool::~GangPool()
{
}

MATCH_FUNC(0x4beca0)
Gang* GangPool::FirstGang_4BECA0()
{
    for (gGangIdx_6206B8 = 0; gGangIdx_6206B8 < GTA2_COUNTOF_S(field_0_gang_list); gGangIdx_6206B8++)
    {
        if (field_0_gang_list[gGangIdx_6206B8].field_0_used && field_0_gang_list[gGangIdx_6206B8].HasKillRespectChange_45DD50())
        {
            return &field_0_gang_list[gGangIdx_6206B8];
        }
    }
    return NULL;
}

MATCH_FUNC(0x4bece0)
Gang* GangPool::NextGang_4BECE0()
{
    while (++gGangIdx_6206B8 < GTA2_COUNTOF_S(field_0_gang_list))
    {
        if (field_0_gang_list[gGangIdx_6206B8].field_0_used && field_0_gang_list[gGangIdx_6206B8].HasKillRespectChange_45DD50())
        {
            return &field_0_gang_list[gGangIdx_6206B8];
        }
    }
    return NULL;
}

MATCH_FUNC(0x4BED30)
wchar_t* Gang::GetNameWide_4BED30() const
{
    gGangNameWide_67E030[0] = field_2_name[0];
    gGangNameWide_67E030[1] = field_2_name[1];
    gGangNameWide_67E030[2] = field_2_name[2];
    gGangNameWide_67E030[3] = 0;
    return gGangNameWide_67E030;
}

MATCH_FUNC(0x4BED70);
void Gang::Init_4BED70()
{
    field_1_gang_idx = 0;
    field_0_used = false;
    field_101_remap = 1;
    field_110_has_high_respect = false;
    field_111_kill_affects_respect = false;
    field_104_basic_weapon = 0;
    field_108_angry_weapon = 0;
    field_10C_hate_weapon = 0;
    field_13C_gang_car_model = 1;
    field_140_gang_car_remap = -1;
    field_139_kill_respect_change = 0;

    for (u8 i = 0; i < 10; i++)
    {
        field_112_hostile_to_gang[i] = true;
        field_122_gang_kill_reaction[i] = 0;
    }

    for (u8 j = 0; j < MAX_PLAYERS; j++)
    {
        field_11C_respect[j] = 0;
    }
    field_141_always_basic_weapon = false;
}

MATCH_FUNC(0x4BEDF0)
bool Gang::IsHostileToGang_4BEDF0(u8 gang_idx) const
{
    return field_112_hostile_to_gang[gang_idx];
}

MATCH_FUNC(0x4BEE30)
void Gang::SetRespect_4BEE30(u8 player_idx, char_type respect)
{
    field_11C_respect[player_idx] = respect;
}

MATCH_FUNC(0x4BEE50)
void Gang::IncrementRespect_4BEE50(u8 player_idx, char_type respect)
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
        field_110_has_high_respect = true;
    }
    else
    {
        field_110_has_high_respect = false;
    }
}

MATCH_FUNC(0x4BEEA0)
void Gang::DecrementRespect_4BEEA0(u8 player_idx, char_type respect)
{
    s8 diff = field_11C_respect[player_idx] - respect;
    if (diff > field_11C_respect[player_idx] || (field_11C_respect[player_idx] = diff) < -100)
    {
        field_11C_respect[player_idx] = -100;
    }

    if (field_11C_respect[player_idx] >= 80)
    {
        field_110_has_high_respect = true;
    }
    else
    {
        field_110_has_high_respect = false;
    }
}

MATCH_FUNC(0x4BEEF0)
char_type Gang::GetRespectForPlayer_4BEEF0(u8 player_idx) const
{
    return field_11C_respect[player_idx];
}

MATCH_FUNC(0x4BEF10)
bool Gang::IsRespectNegativeForPlayer_4BEF10(u8 player_idx) const
{
    if (field_11C_respect[player_idx] < -19)
    {
        return true;
    }
    return false;
}

MATCH_FUNC(0x4BEF50)
void Gang::SetGangKillReaction_4BEF50(u8 gang_idx, char_type kill_reaction_value)
{
    field_122_gang_kill_reaction[gang_idx] = kill_reaction_value;
}

// https://decomp.me/scratch/lPPny
MATCH_FUNC(0x4BEF70)
void Gang::ApplyKillRespectChange_4BEF70(u8 player_idx, u8 respect)
{
    Gang::DecrementRespect_4BEEA0(player_idx, respect * field_139_kill_respect_change);

    for (u8 i = 0; i < 10; ++i)
    {
        Gang* pGang = gGangPool_CA8_67E274->GangByIdx_4BF1C0(i);
        if (field_1_gang_idx != pGang->field_1_gang_idx)
        {
            // The sum is written out twice in the checks and stored with +=, not kept in a local
            s8 change = respect * field_122_gang_kill_reaction[i];
            if (static_cast<s8>(pGang->field_11C_respect[player_idx] + change) < pGang->field_11C_respect[player_idx] ||
                static_cast<s8>(pGang->field_11C_respect[player_idx] + change) > 100)
            {
                pGang->field_11C_respect[player_idx] = 100;
            }
            else
            {
                pGang->field_11C_respect[player_idx] += change;
            }
        }
    }
}

MATCH_FUNC(0x4BF000)
void Gang::ChangeRespectAndUpdate_4BF000(u8 player_idx, char_type respect)
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
        Gang* pGang = gGangPool_CA8_67E274->GangByIdx_4BF1C0(i);
        if (field_1_gang_idx != pGang->field_1_gang_idx)
        {
            if (pGang->field_122_gang_kill_reaction[field_1_gang_idx])
            {
                if (respect > 0)
                {
                    pGang->DecrementRespect_4BEEA0(player_idx, respect);
                }
                else
                {
                    pGang->IncrementRespect_4BEE50(player_idx, abs(respect));
                }
            }
        }
    }
}

MATCH_FUNC(0x4BF090);
void Gang::SetName_4BF090(const char_type* pName, u8 nameLen)
{
    strncpy(field_2_name, pName, nameLen);
    field_2_name[nameLen] = 0; // NULL terminate the string
}

MATCH_FUNC(0x4BF0C0);
s32 Gang::GetCurrentWeapon_4BF0C0() const
{
    if (bStartNetworkGame_7081F0 || field_141_always_basic_weapon)
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

MATCH_FUNC(0x4BF100);
Gang* GangPool::FindGangByName_4BF100(const char_type* pGangName)
{
    for (u8 i = 0; i < 10; i++)
    {
        if (_strnicmp(pGangName, field_0_gang_list[i].field_2_name, 4u) == 0)
        {
            return &field_0_gang_list[i];
        }
    }
    return NULL;
}

MATCH_FUNC(0x4BF170);
Gang* GangPool::AllocGang_4BF170()
{
    for (u8 i = 0; i < GTA2_COUNTOF(field_0_gang_list); i++)
    {
        if (!field_0_gang_list[i].field_0_used)
        {
            Gang* result = &field_0_gang_list[i];
            result->field_1_gang_idx = i;
            result->field_0_used = true;
            return result;
        }
    }
    return NULL;
}

MATCH_FUNC(0x4BF1C0);
Gang* GangPool::GangByIdx_4BF1C0(u8 gang_idx)
{
    return &field_0_gang_list[gang_idx];
}

MATCH_FUNC(0x4BF1E0);
void GangPool::AddGangForMapZone_4BF1E0(gmp_map_zone* pMapZone)
{
    if (!FindGangByName_4BF100(pMapZone->field_6_name))
    {
        Gang* pNewGang = AllocGang_4BF170();
        pNewGang->SetName_4BF090(pMapZone->field_6_name, pMapZone->field_5_name_length);
    }
}

MATCH_FUNC(0x4BF210);
s8 GangPool::GetGangIdxByName_4BF210(const char_type* pGangName)
{
    Gang* pGang = FindGangByName_4BF100(pGangName);
    if (pGang)
    {
        return pGang->field_1_gang_idx;
    }
    else
    {
        return -1;
    }
}

MATCH_FUNC(0x4BF230);
void GangPool::SwapGangSlots_4BF230(Gang* pGang, u8 gang_idx)
{
    // The original only calls Init_4BED70 on this local, no Gang ctor/dtor
    u8 gang_buf[sizeof(Gang)];
    Gang& tmp_gang = *reinterpret_cast<Gang*>(gang_buf);
    tmp_gang.Init_4BED70();
    if (pGang->field_1_gang_idx != gang_idx)
    {
        Gang* pOtherGang = GangByIdx_4BF1C0(gang_idx);
        if (pOtherGang->field_0_used)
        {
            if (pOtherGang->field_138_arrow_colour != pGang->field_138_arrow_colour)
            {
                memcpy(&tmp_gang, pOtherGang, sizeof(tmp_gang));
                memcpy(&gGangPool_CA8_67E274->field_0_gang_list[pOtherGang->field_1_gang_idx],
                       pGang,
                       sizeof(gGangPool_CA8_67E274->field_0_gang_list[pOtherGang->field_1_gang_idx]));
                memcpy(&gGangPool_CA8_67E274->field_0_gang_list[pGang->field_1_gang_idx],
                       &tmp_gang,
                       sizeof(gGangPool_CA8_67E274->field_0_gang_list[pGang->field_1_gang_idx]));
                pGang->field_1_gang_idx = pOtherGang->field_1_gang_idx;
                gGangPool_CA8_67E274->field_0_gang_list[gang_idx].field_1_gang_idx = gang_idx;
            }
        }
    }
}

MATCH_FUNC(0x4BF2F0);
s8 GangPool::FindGangByCarModel_4BF2F0(s32 car_model)
{
    for (u8 i = 0; i < 10; i++)
    {
        if (GangByIdx_4BF1C0(i)->field_13C_gang_car_model == static_cast<u32>(car_model))
        {
            return i;
        }
    }
    return -1;
}

MATCH_FUNC(0x4BF340)
wchar_t* Gang::GetArrowColourText_4BF340() const
{
    sprintf(gTmpBuffer_67C598, "ganga%d", field_138_arrow_colour);
    return gText_0x14_704DFC->Find_5B5F90(gTmpBuffer_67C598);
}