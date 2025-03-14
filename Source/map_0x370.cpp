#include "map_0x370.hpp"
#include "Car_BC.hpp"
#include "Function.hpp"
#include "Globals.hpp"
#include "PurpleDoom.hpp"
#include "RouteFinder.hpp"
#include "Zones_CA8.hpp"
#include "chunk.hpp"
#include "crt_stubs.hpp"
#include "error.hpp"
#include "file.hpp"
#include "gtx_0x106C.hpp"
#include "memory.hpp"
#include "Game_0x40.hpp"

EXPORT_VAR Map_0x370* gMap_0x370_6F6268;
GLOBAL(gMap_0x370_6F6268, 0x6F6268);

EXPORT_VAR gmp_block_info* gBlockInfo0_6F5EB0;
GLOBAL(gBlockInfo0_6F5EB0, 0x6F5EB0);

EXPORT_VAR gmp_block_info gBlockInfo1_6F5F40;
GLOBAL(gBlockInfo1_6F5F40, 0x6F5F40);

EXPORT_VAR gmp_block_info gBlockInfo2_6F6028;
GLOBAL(gBlockInfo2_6F6028, 0x6F6028);

EXPORT_VAR s16 word_6F6002;
GLOBAL(word_6F6002, 0x6F6002);

EXPORT_VAR s32 dword_6F5B80;
GLOBAL(dword_6F5B80, 0x6F5B80);

EXPORT_VAR s32 dword_6F5FD4;
GLOBAL(dword_6F5FD4, 0x6F5FD4);

EXPORT_VAR s32 dword_6F620C;
GLOBAL(dword_6F620C, 0x6F620C);

EXPORT_VAR s32 dword_6F5BA0;
GLOBAL(dword_6F5BA0, 0x6F5BA0);

EXPORT_VAR s32 dword_6F6248;
GLOBAL(dword_6F6248, 0x6F6248);

EXPORT_VAR s32 dword_6F5FAC;
GLOBAL(dword_6F5FAC, 0x6F5FAC);

EXPORT_VAR s32 dword_6F610C;
GLOBAL(dword_6F610C, 0x6F610C);

EXPORT_VAR s32 dword_6F6130;
GLOBAL(dword_6F6130, 0x6F6130);

static inline bool Overlaps(gmp_map_zone* pZone, u8 x, u8 y)
{
    return x >= pZone->field_1_x && y >= pZone->field_2_y && x < pZone->field_1_x + pZone->field_3_w &&
        y < pZone->field_2_y + pZone->field_4_h;
}

MATCH_FUNC(0x452980)
gmp_block_info* Map_0x370::get_block_452980(u8 x_coord, u8 y_coord, u8 z_coord)
{
    gmp_col_info* pColData =
        reinterpret_cast<gmp_col_info*>(&field_0_pDmap->field_40008_pColumn[field_0_pDmap->field_0_base[y_coord][x_coord]]);
    if (z_coord >= pColData->field_0_height || z_coord < pColData->field_1_offset)
    {
        return 0;
    }
    else
    {
        return &field_0_pDmap->field_4000C_block[pColData->field_4_blockd[z_coord - pColData->field_1_offset]];
    }
}

MATCH_FUNC(0x4DEF40)
s8 gmp_map_zone::sub_4DEF40()
{
    Fix16_Rect zoneBounds(field_1_x, field_2_y, field_3_w, field_4_h);
    return gGame_0x40_67E008->sub_4B9B10(&zoneBounds);
}

MATCH_FUNC(0x4DEFD0)
gmp_map_zone* Map_0x370::zone_by_name_4DEFD0(const char_type* pZoneName)
{
    u32 name_len = strlen(pZoneName);
    if (field_328_pZoneData)
    {
        for (field_364_cur_zone_idx = 0; field_364_cur_zone_idx < *(u16*)field_32C_pZones; field_364_cur_zone_idx++)
        {
            gmp_map_zone* pZone = get_zone_4DFB30(field_364_cur_zone_idx);
            if (pZone->field_5_name_length == name_len && memcmp(pZone->field_6_name, pZoneName, name_len) == 0)
            {
                return pZone;
            }
        }
    }
    return NULL;
}

MATCH_FUNC(0x4DF050)
s32 Map_0x370::zone_idx_by_name_4DF050(const char_type* pZoneName, BYTE zone_name_len)
{
    if (!field_328_pZoneData)
    {
        return 0;
    }

    for (field_364_cur_zone_idx = 0; field_364_cur_zone_idx < *(u16*)field_32C_pZones; field_364_cur_zone_idx++)
    {
        gmp_map_zone* pZone = get_zone_4DFB30(field_364_cur_zone_idx);
        // note: esp/stack reorder due to s32 -> byte cast on zone_name_len wrong type
        if (pZone->field_5_name_length == zone_name_len && _strnicmp(pZone->field_6_name, pZoneName, zone_name_len) == 0)
        {
            return field_364_cur_zone_idx;
        }
    }
    return -1;
}

MATCH_FUNC(0x4DF0F0)
gmp_map_zone* Map_0x370::zone_by_type_bounded_4DF0F0(u8 zone_type)
{
    static short sLastIdx_6F626C;

    s16 local_array[40];

    if (field_328_pZoneData)
    {
        u16 idx = 0;
        field_36C_bUnknown = 0;
        field_368_zone_type = zone_type;
        field_364_cur_zone_idx = 0;

        for (field_364_cur_zone_idx = 0; field_364_cur_zone_idx < *(u16*)field_32C_pZones; field_364_cur_zone_idx++)
        {
            if (get_zone_4DFB30(field_364_cur_zone_idx)->field_0_zone_type == field_368_zone_type)
            {
                local_array[idx++] = field_364_cur_zone_idx;
                if (idx >= 40u)
                {
                    break;
                }
            }
        }

        if (idx > 0)
        {
            sLastIdx_6F626C++;
            if (sLastIdx_6F626C >= idx)
            {
                sLastIdx_6F626C = 0;
            }
            return get_zone_4DFB30(local_array[sLastIdx_6F626C]);
        }
    }

    return NULL;
}

MATCH_FUNC(0x4DF1D0)
gmp_map_zone* Map_0x370::first_zone_by_type_4DF1D0(u8 zone_type)
{
    if (field_328_pZoneData)
    {
        field_36C_bUnknown = 0;
        field_368_zone_type = zone_type;
        field_364_cur_zone_idx = 0;

        for (field_364_cur_zone_idx = 0; field_364_cur_zone_idx < *(u16*)field_32C_pZones; field_364_cur_zone_idx++)
        {
            gmp_map_zone* pZone = get_zone_4DFB30(field_364_cur_zone_idx);
            if (pZone->field_0_zone_type == field_368_zone_type)
            {
                return pZone;
            }
        }
    }
    return NULL;
}

STUB_FUNC(0x4DF240)
gmp_map_zone* Map_0x370::sub_4DF240(u8 a2, u8 a3, char_type a4)
{
    return 0;
}

STUB_FUNC(0x4DF3E0)
u8* Map_0x370::sub_4DF3E0(u8 a2, u8 a3, char_type a4)
{
    return 0;
}

MATCH_FUNC(0x4DF4D0)
gmp_map_zone* Map_0x370::zone_by_pos_and_type_4DF4D0(u8 zone_x, u8 zone_y, u8 zone_type)
{
    if (field_328_pZoneData)
    {
        field_368_zone_type = zone_type;
        field_36A_zone_x = zone_x;
        field_36B_zone_y = zone_y;
        field_36C_bUnknown = 1;

        for (field_364_cur_zone_idx = 0; field_364_cur_zone_idx < *(u16*)field_32C_pZones; field_364_cur_zone_idx++)
        {
            gmp_map_zone* pZone = get_zone_4DFB30(field_364_cur_zone_idx);
            if (pZone->field_0_zone_type == field_368_zone_type && Overlaps(pZone, field_36A_zone_x, field_36B_zone_y))
            {
                return pZone;
            }
        }
    }
    return NULL;
}

MATCH_FUNC(0x4DF5C0)
gmp_map_zone* Map_0x370::nav_zone_by_pos_4DF5C0(u8 zone_x, u8 zone_y)
{
    if (field_328_pZoneData)
    {
        field_36A_zone_x = zone_x;
        field_36B_zone_y = zone_y;

        for (field_364_cur_zone_idx = 0; field_364_cur_zone_idx < *(u16*)field_32C_pZones; field_364_cur_zone_idx++)
        {
            gmp_map_zone* pZone = get_zone_4DFB30(field_364_cur_zone_idx);
            if ((pZone->field_0_zone_type == 10 || pZone->field_0_zone_type == 1 || pZone->field_0_zone_type == 15) &&
                Overlaps(pZone, field_36A_zone_x, field_36B_zone_y))
            {
                return pZone;
            }
        }
    }
    return NULL;
}

MATCH_FUNC(0x4DF6A0)
gmp_map_zone* Map_0x370::sub_4DF6A0(u8 zone_x, u8 zone_y)
{
    gmp_map_zone* pZone;

    if (this->field_328_pZoneData)
    {
        this->field_36A_zone_x = zone_x;
        this->field_36B_zone_y = zone_y;
        this->field_36C_bUnknown = 1;

        for (field_364_cur_zone_idx = 0; field_364_cur_zone_idx < *(u16*)this->field_32C_pZones; field_364_cur_zone_idx++)
        {
            pZone = Map_0x370::get_zone_4DFB30(this->field_364_cur_zone_idx);
            if (Overlaps(pZone, this->field_36A_zone_x, this->field_36B_zone_y))
            {
                return pZone;
            }
        }
    }
    return NULL;
}

MATCH_FUNC(0x4DF770)
gmp_map_zone* Map_0x370::next_zone_4DF770()
{
    gmp_map_zone* pZoneIter;

    if (this->field_328_pZoneData)
    {
        for (++this->field_364_cur_zone_idx; field_364_cur_zone_idx < *(u16*)this->field_32C_pZones; ++this->field_364_cur_zone_idx)
        {
            if (field_364_cur_zone_idx >= *(u16*)this->field_32C_pZones)
            {
                return 0;
            }
            pZoneIter = Map_0x370::get_zone_4DFB30(field_364_cur_zone_idx);
            if (pZoneIter->field_0_zone_type == this->field_368_zone_type &&
                (!this->field_36C_bUnknown || Overlaps(pZoneIter, field_36A_zone_x, field_36B_zone_y)))
            {
                return pZoneIter;
            }
        }
    }
    return NULL;
}

MATCH_FUNC(0x4DF840)
gmp_zone_unknown* Map_0x370::cur_zone_4DF840()
{
    if (field_328_pZoneData)
    {
        BYTE zone_idx = field_330_pZoneArray[field_364_cur_zone_idx];
        if (zone_idx != 0)
        {
            return &field_334_pUnknownZoneData[zone_idx];
        }
    }
    return 0;
}

MATCH_FUNC(0x4DF890)
gmp_zone_unknown* Map_0x370::get_nav_zone_unknown_4DF890(s32 zone_x, s32 zone_y)
{
    if (nav_zone_by_pos_4DF5C0(zone_x, zone_y))
    {
        return cur_zone_4DF840();
    }
    else
    {
        return field_334_pUnknownZoneData;
    }
}

MATCH_FUNC(0x4DF8C0)
void Map_0x370::set_nav_unknown_data_4DF8C0(s32 zone_idx,
                                            s16 a3,
                                            s16 a4,
                                            s16 a5,
                                            s16 a6,
                                            s16 a7,
                                            s16 a8,
                                            s16 a9,
                                            s16 a10,
                                            s16 a11,
                                            s16 a12,
                                            s16 a13)
{
    get_zone_4DFB30(zone_idx);
    gmp_zone_unknown* pZoneUnknown = &field_334_pUnknownZoneData[field_330_pZoneArray[zone_idx]];
    if (a3 != -1)
    {
        pZoneUnknown->field_0_density = a3;
    }

    if (a4 != -1)
    {
        pZoneUnknown->field_2 = a4;
    }

    if (a5 != -1)
    {
        pZoneUnknown->field_4 = a5;
    }

    if (a6 != -1)
    {
        pZoneUnknown->field_6 = a6;
    }

    if (a7 != -1)
    {
        pZoneUnknown->field_8 = a7;
    }

    if (a8 != -1)
    {
        pZoneUnknown->field_A = a8;
    }

    if (a9 != -1)
    {
        pZoneUnknown->field_C = a9;
    }

    if (a10 != -1)
    {
        pZoneUnknown->field_E = a10;
    }

    if (a11 != -1)
    {
        pZoneUnknown->field_10 = a11;
    }

    if (a12 != -1)
    {
        pZoneUnknown->field_12 = a12;
    }

    if (a13 != -1)
    {
        pZoneUnknown->field_14 = a13;
    }
}

MATCH_FUNC(0x4DF9A0)
gmp_zone_unknown* Map_0x370::get_zone_unknown_4DF9A0(u16 zone_idx)
{
    return &field_334_pUnknownZoneData[field_330_pZoneArray[zone_idx]];
}

MATCH_FUNC(0x4DF9D0)
void Map_0x370::set_nav_unknown_f0_4DF9D0(u16 a2, s16 a3)
{
    get_zone_unknown_4DF9A0(a2)->field_0_density = a3;
}

MATCH_FUNC(0x4DF9F0)
void Map_0x370::set_nav_unknown_f2_4DF9F0(u16 a2, s16 a3)
{
    get_zone_unknown_4DF9A0(a2)->field_2 = a3;
}

MATCH_FUNC(0x4DFA10)
void Map_0x370::set_nav_unknown_f4_4DFA10(u16 a2, s16 a3)
{
    get_zone_unknown_4DF9A0(a2)->field_4 = a3;
}

MATCH_FUNC(0x4DFA30)
void Map_0x370::set_nav_unknown_f6_4DFA30(u16 a2, s16 a3)
{
    get_zone_unknown_4DF9A0(a2)->field_6 = a3;
}

MATCH_FUNC(0x4DFA50)
void Map_0x370::set_nav_unknown_f8_4DFA50(u16 a2, s16 a3)
{
    get_zone_unknown_4DF9A0(a2)->field_8 = a3;
}

MATCH_FUNC(0x4DFA70)
void Map_0x370::set_nav_unknown_fA_4DFA70(u16 a2, s16 a3)
{
    get_zone_unknown_4DF9A0(a2)->field_A = a3;
}

MATCH_FUNC(0x4DFA90)
void Map_0x370::set_nav_unknown_fC_4DFA90(u16 a2, s16 a3)
{
    get_zone_unknown_4DF9A0(a2)->field_C = a3;
}

MATCH_FUNC(0x4DFAB0)
void Map_0x370::set_nav_unknown_fE_4DFAB0(u16 a2, s16 a3)
{
    get_zone_unknown_4DF9A0(a2)->field_E = a3;
}

MATCH_FUNC(0x4DFAD0)
void Map_0x370::set_nav_unknown_f10_4DFAD0(u16 a2, s16 a3)
{
    get_zone_unknown_4DF9A0(a2)->field_10 = a3;
}

MATCH_FUNC(0x4DFAF0)
void Map_0x370::set_nav_unknown_f12_4DFAF0(u16 a2, s16 a3)
{
    get_zone_unknown_4DF9A0(a2)->field_12 = a3;
}

MATCH_FUNC(0x4DFB10)
void Map_0x370::set_nav_unknown_f14_4DFB10(u16 a2, s16 a3)
{
    get_zone_unknown_4DF9A0(a2)->field_14 = a3;
}

MATCH_FUNC(0x4DFB30)
gmp_map_zone* Map_0x370::get_zone_4DFB30(u16 zone_idx)
{
    return field_32C_pZones[zone_idx + 1];
}

MATCH_FUNC(0x4DFB50)
Gang_144* Map_0x370::sub_4DFB50(Fix16 x, Fix16 y)
{
    gmp_map_zone* pZone = zone_by_pos_and_type_4DF4D0(x.ToInt(), y.ToInt(), 0xEu);
    if (pZone)
    {
        return gZones_CA8_67E274->zone_by_name_4BF100(pZone->field_6_name);
    }
    else
    {
        return 0;
    }
}

STUB_FUNC(0x4DFB90)
void Map_0x370::sub_4DFB90()
{
}

MATCH_FUNC(0x4DFCA0)
void Map_0x370::alloc_zones_4DFCA0()
{
    gmp_map_zone* pZoneIter = first_zone_by_type_4DF1D0(14);
    while (pZoneIter)
    {
        gZones_CA8_67E274->alloc_map_zone_4BF1E0(pZoneIter);
        pZoneIter = next_zone_4DF770();
    }
}

STUB_FUNC(0x4DFCD0)
void Map_0x370::update_lights_4DFCD0()
{
}

MATCH_FUNC(0x4DFE10)
gmp_block_info* Map_0x370::get_block_4DFE10(s32 x_coord, s32 y_coord, s32 z_coord)
{
    gmp_col_info* pCol =
        reinterpret_cast<gmp_col_info*>(&field_0_pDmap->field_40008_pColumn[field_0_pDmap->field_0_base[y_coord][x_coord]]);
    if (z_coord < pCol->field_0_height)
    {
        s32 offset = pCol->field_1_offset;
        if (z_coord >= offset)
        {
            return &field_0_pDmap->field_4000C_block[pCol->field_4_blockd[z_coord - offset]];
        }
    }
    return 0;
}

MATCH_FUNC(0x4DFE60)
gmp_block_info* Map_0x370::sub_4DFE60(s32 x, s32 y, s32 a4)
{
    gmp_col_info* v5;

    v5 = (gmp_col_info*)&field_0_pDmap->field_40008_pColumn[field_0_pDmap->field_0_base[y][x]];
    if (a4 < v5->field_0_height)
    {
        s32 v6 = v5->field_1_offset;
        if (a4 >= v6)
        {
            gmp_block_info* v7_block = &field_0_pDmap->field_4000C_block[v5->field_4_blockd[a4 - v6]];
            s32 slope_type = v7_block->field_B_slope_type & 0xFCu;
            if (slope_type < 0xD4 || slope_type > 0xF4)
            {
                if (slope_type >= 0xC4 && slope_type <= 0xD0)
                {
                    return &gBlockInfo2_6F6028;
                }
                return v7_block;
            }
        }
    }
    return NULL;
}

MATCH_FUNC(0x4DFEE0)
gmp_block_info* Map_0x370::sub_4DFEE0(s32 x_coord, s32 y_coord, s32 z_coord)
{
    gmp_compressed_map_32* field_0_pDmap;
    gmp_col_info* pColInfo;
    s32 offset;

    if (x_coord < 0)
    {
        x_coord = 0;
    }
    else
    {
        if (x_coord > 255)
        {
            x_coord = 255;
        }
    }

    if (y_coord < 0)
    {
        y_coord = 0;
    }
    else
    {
        if (y_coord > 255)
        {
            y_coord = 255;
        }
    }

    field_0_pDmap = this->field_0_pDmap;
    pColInfo = (gmp_col_info*)&field_0_pDmap->field_40008_pColumn[field_0_pDmap->field_0_base[y_coord][x_coord]];

    if (z_coord < (u8)pColInfo->field_0_height)
    {
        offset = (u8)pColInfo->field_1_offset;
        if (z_coord >= offset)
        {
            return &field_0_pDmap->field_4000C_block[pColInfo->field_4_blockd[z_coord - offset]];
        }
    }
    return NULL;
}

Fix16 dword_6F6110; // = 0x4000; // todo

MATCH_FUNC(0x4DFF60)
DWORD Map_0x370::sub_4DFF60(Fix16 x_coord, Fix16 y_coord, Fix16 z_coord)
{
    gmp_block_info* pBlock1 = get_block_4DFE10((x_coord - dword_6F6110).ToInt(), y_coord.ToInt(), z_coord.ToInt());
    if (pBlock1 && pBlock1->field_2_right != 0)
    {
        u32 spec = gGtx_0x106C_703DD4->field_6C_spec[pBlock1->field_2_right & 0x3FF];
        if (spec == 3)
        {
            return 1;
        }
        return spec;
    }

    gmp_block_info* pBlock2 = get_block_4DFE10(x_coord.ToInt(), y_coord.ToInt(), z_coord.ToInt());
    if (pBlock2 && pBlock2->field_0_left != 0)
    {
        u32 spec = gGtx_0x106C_703DD4->field_6C_spec[pBlock2->field_0_left & 0x3FF];
        if (spec == 3)
        {
            return 1;
        }
        return spec;
    }

    return 0;
}

MATCH_FUNC(0x4E0000)
s32 Map_0x370::sub_4E0000(s32 a2, s32 a3, s32 a4)
{
    gmp_block_info* block_4DFE10;
    s16 field_6_bottom;
    s32 result;
    gmp_block_info* v8;
    s16 field_4_top;

    block_4DFE10 = Map_0x370::get_block_4DFE10(a2 >> 14, (a3 - dword_6F6110.mValue) >> 14, a4 >> 14);
    if (block_4DFE10 && (field_6_bottom = block_4DFE10->field_6_bottom) != 0)
    {
        result = gGtx_0x106C_703DD4->field_6C_spec[field_6_bottom & 0x3FF];
        if (result == 3)
        {
            return 1;
        }
    }
    else
    {
        v8 = Map_0x370::get_block_4DFE10(a2 >> 14, a3 >> 14, a4 >> 14);
        if (v8 && (field_4_top = v8->field_4_top) != 0)
        {
            result = gGtx_0x106C_703DD4->field_6C_spec[field_4_top & 0x3FF];
            if (result == 3)
            {
                return 1;
            }
        }
        else
        {
            return 0;
        }
    }
    return result;
}

MATCH_FUNC(0x4E00A0)
s32 Map_0x370::sub_4E00A0(s32 x, s32 y, s32 z)
{
    gmp_block_info* pBlock;
    s16 lid;
    s32 result;

    if (z >= dword_6F610C)
    {
        if (z < dword_6F6130)
        {
            pBlock = gMap_0x370_6F6268->get_block_4DFE10(x >> 14, y >> 14, z >> 14);
            if (pBlock)
            {
                lid = pBlock->field_8_lid;
                if (lid)
                {
                    result = gGtx_0x106C_703DD4->field_6C_spec[lid & 0x3FF];
                    if (result == 3)
                    {
                        return 1;
                    }
                    return result;
                }
            }
        }
    }
    return 0;
}

MATCH_FUNC(0x4E0110)
char_type Map_0x370::sub_4E0110()
{
    char result;
    result = field_36E;
    field_36E = 0;
    return result;
}

MATCH_FUNC(0x4E0120)
char_type Map_0x370::sub_4E0120()
{
    char result;
    result = this->field_36F;
    this->field_36F = 0;
    return result;
}

STUB_FUNC(0x4E0130)
char_type Map_0x370::sub_4E0130(s32 a2, s32 a3, s32 a4, s32 a5, u8* a6, char_type a7)
{
    return 0;
}

STUB_FUNC(0x4E11E0)
char_type Map_0x370::sub_4E11E0(s32* a2)
{
    return 0;
}

STUB_FUNC(0x4E1520)
char_type Map_0x370::sub_4E1520(s32 a2)
{
    return 0;
}

STUB_FUNC(0x4E18A0)
char_type Map_0x370::sub_4E18A0(s32 a2, s32 a3, s32 a4, s32 a5, s32 a6)
{
    return 0;
}

STUB_FUNC(0x4E1A30)
char_type Map_0x370::sub_4E1A30(s32 a2, s32 a3, s32 a4, s32 a5, s32 a6)
{
    return 0;
}

STUB_FUNC(0x4E1E00)
char_type Map_0x370::sub_4E1E00(s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8)
{
    return 0;
}

MATCH_FUNC(0x4E4460)
char_type Map_0x370::sub_4E4460(s32 a2, s32 a3, s32 a4, Sprite* a5, s16 a6)
{
    gSprite_6F61E8 = a5;
    word_6F6002 = a6;
    if (dword_6F5B80 - dword_6F5FD4 >= 3 || a2 > dword_6F5FD4 + 1 || a2 < dword_6F5B80 - 1)
    {
        if (sub_4E1E00(a2 - 1, a2 + 1, dword_6F6108, dword_6F5F38, a2, a3, a4))
        {
            return 1;
        }
        else if (dword_6F5B80 == a2 + 2)
        {
            return sub_4E1E00(dword_6F5B80 - 1, dword_6F5B80, dword_6F6108, dword_6F5F38, dword_6F5B80 - 1, a3, dword_6F620C);
        }
        else
        {
            return sub_4E1E00(dword_6F5FD4, dword_6F5FD4 + 1, dword_6F6108, dword_6F5F38, dword_6F5FD4 + 1, a3, dword_6F5BA0);
        }
    }
    else if (dword_6F5F38 - dword_6F6108 >= 3 || a3 > dword_6F6108 + 1 || a3 < dword_6F5F38 - 1)
    {
        if (sub_4E1E00(dword_6F5FD4, dword_6F5B80, a3 - 1, a3 + 1, a2, a3, a4))
        {
            return 1;
        }
        else if (dword_6F5F38 == a3 + 2)
        {
            return sub_4E1E00(dword_6F5FD4, dword_6F5B80, dword_6F5F38 - 1, dword_6F5F38, a2, dword_6F5F38 - 1, dword_6F6248);
        }
        else
        {
            return sub_4E1E00(dword_6F5FD4, dword_6F5B80, dword_6F6108, dword_6F6108 + 1, a2, dword_6F6108 + 1, dword_6F5FAC);
        }
    }
    else
    {
        return sub_4E1E00(dword_6F5FD4, dword_6F5B80, dword_6F6108, dword_6F5F38, a2, a3, a4);
    }
}

STUB_FUNC(0x4E4630)
char_type Map_0x370::sub_4E4630(s32 a2)
{
    return 0;
}

STUB_FUNC(0x4E4770)
char_type Map_0x370::sub_4E4770(s32 a2)
{
    return 0;
}

STUB_FUNC(0x4E4820)
char_type Map_0x370::sub_4E4820(u32* a2, char_type a3)
{
    return 0;
}

STUB_FUNC(0x4E4930)
char_type Map_0x370::sub_4E4930(u8* a1, u8* a2, u8* a3, char_type a4)
{
    return 0;
}

STUB_FUNC(0x4E4AC0)
char_type Map_0x370::sub_4E4AC0(char_type a1)
{
    return 0;
}

STUB_FUNC(0x4E4B40)
char_type Map_0x370::sub_4E4B40(s32 a1, gmp_block_info* a2)
{
    return 0;
}

MATCH_FUNC(0x4E4BB0)
gmp_block_info* Map_0x370::sub_4E4BB0(s32 a2, s32 a3, u32* a4)
{
    gmp_col_info* v4 = (gmp_col_info*)&this->field_0_pDmap->field_40008_pColumn[this->field_0_pDmap->field_0_base[a3][a2]];
    for (s32 v5 = v4->field_0_height - v4->field_1_offset - 1; v5 >= 0; v5--)
    {
        //  Begin with the highest non-empty block in the column
        gBlockInfo0_6F5EB0 = this->field_0_pDmap->get_block(v4->field_4_blockd[v5]); //  get the block
        switch (gBlockInfo0_6F5EB0->field_B_slope_type & 3)
        {
            case 0:
                break;
            case 2:
                *a4 = v5 + v4->field_1_offset;
                return gBlockInfo0_6F5EB0;
            default:
                return 0;
        }
    }
    return 0;
}

MATCH_FUNC(0x4E4C30)
gmp_block_info* Map_0x370::FindHighestBlockForCoord_4E4C30(s32 a2, s32 a3, u32* a4)
{

    gmp_col_info* v4;
    s32 v5;
    s32* j;

    //  get the column at ( x = a2 , y = a3 )
    v4 = (gmp_col_info*)&field_0_pDmap->field_40008_pColumn[field_0_pDmap->field_0_base[a3][a2]];

    //  Subtract the empty bottom blocks from the column
    //  and subtract "1"
    v5 = v4->field_0_height - v4->field_1_offset - 1;

    //  air blocks can be empty blocks or blocks with tiles but without ground

    //  if field_0_height != field_1_offset, then there are non-empty blocks on the column v4
    if (v5 >= 0)
    {

        j = &(v4->field_4_blockd[v5]); //  block number reference;

        //  Begin with the highest non-empty block in the column

        while (1)
        {
            gBlockInfo0_6F5EB0 = &field_0_pDmap->field_4000C_block[*j]; //  get the block

            if ((gBlockInfo0_6F5EB0->field_B_slope_type & 3) != 0) //  if it isn't an air block
            {
                break;
            }

            v5--;
            j -= 1;

            if (v5 < 0)
            {
                //  despite having non-empty blocks, all blocks of this column are air blocks
                return 0;
            }
        }
    }
    else
    {
        //  if field_0_height = field_1_offset, then there isn't non-empty blocks on the column v4
        return 0;
    }

    //  the highest non-air block has Z_coord = (v5'th block) + (number of bottom empty blocks)
    *a4 = v5 + v4->field_1_offset;
    return gBlockInfo0_6F5EB0;
}

MATCH_FUNC(0x4E4CB0)
gmp_block_info* Map_0x370::sub_4E4CB0(s32 a2, s32 a3, s32* a4)
{
    gmp_col_info* v4;
    s32 v5;
    s32 v6;
    s32 v7;
    s32 v8;
    u32* j;

    v4 = (gmp_col_info*)&this->field_0_pDmap->field_40008_pColumn[this->field_0_pDmap->field_0_base[a3][a2]];

    v6 = v4->field_1_offset;
    v5 = *a4;

    if (v5 < v6)
    {
        return 0;
    }
    v7 = v4->field_0_height;
    v8 = (v5 >= v7 ? v7 - v6 - 1 : v5 - v6);

    if (v8 >= 0)
    {

        j = &(((u32*)v4)[v8 + 1]); //  block number reference; add 1 to correct the index (or bypass the first dword)

        //  Begin with the highest non-empty block in the column

        while (1)
        {
            gBlockInfo0_6F5EB0 = &this->field_0_pDmap->field_4000C_block[*j]; //  get the block

            if ((gBlockInfo0_6F5EB0->field_B_slope_type & 3) != 0) //  if it isn't an air block
            {
                break;
            }

            v8--;
            j -= 1;

            if (v8 < 0)
            {
                //  despite having non-empty blocks, all blocks of this column are air blocks
                return 0;
            }
        }
    }
    else
    {
        //  if field_0_height = field_1_offset, then there isn't non-empty blocks on the column v4
        return 0;
    }

    *a4 = v8 + v4->field_1_offset;
    return gBlockInfo0_6F5EB0;
}

MATCH_FUNC(0x4E4D40)
Fix16* Map_0x370::sub_4E4D40(Fix16* a2, Fix16 x_pos, Fix16 y_pos, Fix16 z_pos)
{
    Fix16 v4;
    Fix16 v5;
    Fix16 v6;
    gmp_block_info* block_4DFE10;
    char_type v9;
    Fix16 v10;
    gmp_block_info* v11;
    char_type field_B_slope_type;

    u8 v13;
    s32 v14;

    v5 = z_pos;
    v6 = y_pos;
    v4 = x_pos;
    if ((z_pos.mValue & 0x3FFF) == dword_6F610C ||
        (block_4DFE10 = Map_0x370::get_block_4DFE10(x_pos.ToInt(), y_pos.ToInt(), z_pos.ToInt())) == 0 ||
        (v9 = block_4DFE10->field_B_slope_type, (v13 = v9 & 0xFCu) <= 0) //  or it's a flat block
        || v13 >= 0xB4 //  or it's not a ramp
        || (v9 & 3) == 0 //  or it's a air block
        || (v10.mValue = v5.mValue & 0xFFFFC000, Map_0x370::sub_4E5BF0(x_pos, v6, &v10), v10 > v5))
    {
        v14 = v5.ToInt() - 1;
        v11 = Map_0x370::sub_4E4CB0(v4.ToInt(), v6.ToInt(), &v14);
        gBlockInfo0_6F5EB0 = v11;
        if (!v11)
        {
            a2->mValue = dword_6F6110.mValue;
            return a2;
        }
        field_B_slope_type = v11->field_B_slope_type;

        if ((v13 = field_B_slope_type & 0xFC) > 0 //  it's not a flat block
            && v13 < 0xB4 && (field_B_slope_type & 3) != 0)
        {
            v10.FromInt(v14);
            Map_0x370::sub_4E5BF0(x_pos, v6, &v10);
            a2->mValue = v10.mValue;
            return a2;
        }
        v10.FromInt(++v14);
    }
    a2->mValue = v10.mValue;
    return a2;
}

STUB_FUNC(0x4E4E50)     //  DAMN reg swap    https://decomp.me/scratch/aQODE
Fix16* Map_0x370::sub_4E4E50(Fix16* a2, Fix16 a3, Fix16 a4, Fix16 a5)
{
    Fix16 v5;
    gmp_block_info* block_4DFE10;
    s8 v10;
    u8 v11;
    gmp_block_info* v12;
    s8 field_B_slope_type;
    u8 v14;
    Fix16 v16;

    for (v5.mValue = a5.mValue; v5.mValue < a5.mValue + dword_6F6110.mValue; v5.mValue = dword_6F6110.mValue + (v5.mValue & 0xFFFFC000))
    {
        block_4DFE10 = Map_0x370::get_block_4DFE10(a3.ToInt(), a4.ToInt(), v5.ToInt());
        gBlockInfo0_6F5EB0 = block_4DFE10;
        if (!block_4DFE10 || (v10 = block_4DFE10->field_B_slope_type, (v10 & 3) == 0))
        {
            v12 = Map_0x370::get_block_4DFE10(a3.ToInt(), a4.ToInt(), (v5 - dword_6F6110).ToInt());
            gBlockInfo0_6F5EB0 = v12;
            if (v12)
            {
                field_B_slope_type = v12->field_B_slope_type;
                if ((field_B_slope_type & 3) != 0)
                {
                    v14 = field_B_slope_type & 0xFC;
                    if (v14 <= 0 || v14 >= 0xB4u)
                    {
                        a2->mValue = v5.mValue;
                        return a2;
                    }
                }
            }
        }
        else
        {
            v11 = v10 & 0xFC;
            if (v11 > 0)
            {
                if (v11 < 0xB4u)
                {
                    v16.mValue = v5.mValue & 0xFFFFC000;
                    Map_0x370::sub_4E5BF0(a3, a4, &v16);
                    if (v16 >= v5)
                    {
                        a2->mValue = v16.mValue;
                        return a2;
                    }
                }
            }
        }
    }
    v5 = a5;
    a2->mValue = v5.mValue;
    return a2;
}

STUB_FUNC(0x4E4F40)
s32* Map_0x370::sub_4E4F40(s32* a2, s32 a3, s32 a4, s32 a5)
{
    return 0;
}

STUB_FUNC(0x4E5050)
s32* Map_0x370::sub_4E5050(s32* a2, s32 a3, s32 a4, s32 a5, u8* a6)
{
    return 0;
}

STUB_FUNC(0x4E5170)
char_type Map_0x370::sub_4E5170(s32 a2, s32 a3, s32 a4)
{
    return 0;
}

STUB_FUNC(0x4E52A0)
char_type Map_0x370::sub_4E52A0(s32 a2, s32 a3, s32 a4)
{
    return 0;
}

STUB_FUNC(0x4E5300)
char_type Map_0x370::sub_4E5300(s32 a2, s32 a3, s32 a4, s32 a5)
{
    return 0;
}

STUB_FUNC(0x4E5480)
char_type Map_0x370::sub_4E5480(s32 a2, s32 a3, s32 a4, s32 a5, s32* a6)
{
    return 0;
}

STUB_FUNC(0x4E5640)
char_type Map_0x370::sub_4E5640(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8, s32 a9)
{
    return 0;
}

MATCH_FUNC(0x4E5B60)
Fix16* Map_0x370::FindGroundZForCoord_4E5B60(Fix16* a2, Fix16 x_pos, Fix16 y_pos)
{
    Fix16 v4;
    Fix16 v5;
    gmp_block_info* v7;
    char_type field_B_slope_type;

    //  This function returns the Z coord based on the highest block of (X = a3, Y = a4);

    s32 v6;

    Fix16 v9;

    v4 = x_pos;
    v5 = y_pos;

    //  get the highest non-air block at (x_pos,y_pos)
    v7 = Map_0x370::FindHighestBlockForCoord_4E4C30(x_pos.ToInt(), y_pos.ToInt(), (u32*)&v6);
    gBlockInfo0_6F5EB0 = v7;

    if (!v7)
    {
        a2->mValue = 0;
        return a2;
    }
    else
    {
        field_B_slope_type = v7->field_B_slope_type;

        //  0xFCu = 1111 1100
        //  bits 0-1 = surface type
        //  bits 2-7 = slope angle type
        if (((u8)field_B_slope_type & 0xFCu) > 0 //  it's not a flat block
            && ((u8)field_B_slope_type & 0xFCu) < 0xB4u //  < 10110100 = it's a ramp
            && ((u8)field_B_slope_type & 3) != 0) //  (bits 0-1) != 0 means it's not an air block
        {
            v9.FromInt(v6);
            Map_0x370::sub_4E5BF0(v4, v5, &v9); //  get the Z position based on the slope angle?
        }
        else
        {
            v9.FromInt(v6 + 1);
        }
        a2->mValue = v9.mValue;
        return a2;
    }
}

STUB_FUNC(0x4E5BF0)
char_type Map_0x370::sub_4E5BF0(Fix16 a2, Fix16 a3, Fix16* a4)
{
    return 0;
}

STUB_FUNC(0x4E6190)
s16 Map_0x370::sub_4E6190(s32 x, s32 y, s32 z, s32 a5, char_type a6)
{
    return 0;
}

MATCH_FUNC(0x4E62D0)
gmp_block_info* Map_0x370::sub_4E62D0(s32 a2, s32 a3, u32* a4)
{
    gmp_col_info* v4;
    s32 v5;

    v4 = (gmp_col_info*)&this->field_0_pDmap->field_40008_pColumn[this->field_0_pDmap->field_0_base[a3][a2]];

    for (v5 = v4->field_0_height - v4->field_1_offset - 1; v5 >= 0; v5--)
    {
        gBlockInfo0_6F5EB0 = this->field_0_pDmap->get_block(v4->field_4_blockd[v5]);
        if ((gBlockInfo0_6F5EB0->field_B_slope_type & 3) == 3 && (gBlockInfo0_6F5EB0->field_A_arrows & 0xF) != 0)
        {
            *a4 = v5 + v4->field_1_offset;
            return gBlockInfo0_6F5EB0;
        }
    }
    return 0;
}

MATCH_FUNC(0x4E6360)
gmp_block_info* Map_0x370::sub_4E6360(s32 a2, s32 a3, s32* a4)
{
    gmp_col_info* v4;
    s32 v7;
    s32 v9;
    s32 v10;
    s32 v11;
    gmp_block_info* v13;

    v4 = (gmp_col_info*)&this->field_0_pDmap->field_40008_pColumn[this->field_0_pDmap->field_0_base[a3][a2]];

    v7 = v4->field_1_offset;

    v9 = *a4;
    if (v9 < v7)
    {
        return 0;
    }

    v10 = v4->field_0_height;
    v11 = v9 >= v10 ? v10 - v7 - 1 : v9 - v7;

    for (; v11 >= 0; v11--)
    {
        v13 = this->field_0_pDmap->get_block(v4->field_4_blockd[v11]);
        gBlockInfo0_6F5EB0 = v13;
        if ((v13->field_B_slope_type & 3) == 3 && (v13->field_A_arrows & 0xF) != 0)
        {
            *a4 = v11 + v4->field_1_offset;
            return gBlockInfo0_6F5EB0;
        }
    }
    return 0;
}

MATCH_FUNC(0x4E6400)
Fix16* Map_0x370::sub_4E6400(Fix16* a2, Fix16 a3, Fix16 a4, Fix16 a5)
{
    Fix16 v5;
    Fix16 v6;
    Fix16 v7;
    gmp_block_info* block_4DFE10;
    u8 v9;
    Fix16 v10;
    u8 v13;

    v5 = a5;
    v6 = a4;
    v7 = a3;
    if ((a5.mValue & 0x3FFF) == dword_6F610C 
        || (block_4DFE10 = Map_0x370::get_block_4DFE10(a3.ToInt(), a4.ToInt(), a5.ToInt())) == 0
        || (v9 = block_4DFE10->field_B_slope_type, (v13 = v9 & 0xFC) <= 0)
        || v13 >= 0xB4
        || (v9 & 3) == 0
        || (v10.mValue = v5.mValue & 0xFFFFC000, Map_0x370::sub_4E5BF0(v7, v6, &v10),
         v10 > v5))
    {
        s32 v14 = v5.ToInt() - 1;
        gmp_block_info* v11 = Map_0x370::sub_4E6360(v7.ToInt(), v6.ToInt(), &v14);
        gBlockInfo0_6F5EB0 = v11;
        if (!v11)
        {
            a2->mValue = 0x4000;
            return a2;
        }
        s8 field_B_slope_type = v11->field_B_slope_type;
        if ( ( v13 = field_B_slope_type & 0xFCu ) > 0 
            && v13 < 0xB4u
            && (field_B_slope_type & 3) != 0 )
        {
            v10.FromInt(v14);
            Map_0x370::sub_4E5BF0(v7, v6, &v10);
        }
        else
        {
            v10.FromInt(v14 + 1);
        }
    }
    a2->mValue = v10.mValue;
    return a2;
}

MATCH_FUNC(0x4E6510)
Fix16* Map_0x370::sub_4E6510(Fix16* a2, Fix16 a3, Fix16 a4)
{
    Fix16 v4;
    Fix16 v5;
    gmp_block_info* v7;
    Fix16 v10;
    u32 v6;
    
    v4 = a3;
    v5 = a4;
    v7 = Map_0x370::sub_4E62D0(a3.ToInt(), a4.ToInt(), &v6);
    gBlockInfo0_6F5EB0 = v7;
    if (!v7)
    {
        a2->mValue = 0;
        return a2;
    }
    else
    {
        u8 field_B_slope_type = v7->field_B_slope_type;
        u8 v13 = field_B_slope_type & 0xFC;
        if ( v13 > 0 
            && v13 < 0xB4 
            && (field_B_slope_type & 3) != 0)
        {
            v10.FromInt(v6);
            Map_0x370::sub_4E5BF0(v4, v5, &v10);
        }
        else
        {
            v10.FromInt(v6 + 1);
        }
    }
    a2->mValue = v10.mValue;
    return a2;
}

STUB_FUNC(0x4E65A0)
s16 Map_0x370::sub_4E65A0(s32 a2, s32 a3, s32* a4, char_type a5, char_type a6)
{
    return 0;
}

STUB_FUNC(0x4E6660)
s32 Map_0x370::sub_4E6660(s32* a2, s32* a3, s32* a4, s32 a5)
{
    return 0;
}

STUB_FUNC(0x4E7190)
s32 Map_0x370::sub_4E7190(s32* a2, s32* a3, s32* a4, s32 a5)
{
    return 0;
}

STUB_FUNC(0x4E7E90)
char_type Map_0x370::sub_4E7E90(u8* a2, char_type* a3)
{
    return 0;
}

STUB_FUNC(0x4E7FC0)
char_type Map_0x370::sub_4E7FC0(s32 a2, s32 a3, s32 a4)
{
    return 0;
}

STUB_FUNC(0x4E8140)
s32 Map_0x370::sub_4E8140(u32* a2)
{
    return 0;
}

STUB_FUNC(0x4E8180)
s32 Map_0x370::sub_4E8180(u32 a2)
{
    return 0;
}

STUB_FUNC(0x4E81D0)
s32 Map_0x370::sub_4E81D0(u32 a2)
{
    return 0;
}

STUB_FUNC(0x4E8220)
s32 Map_0x370::sub_4E8220(u32 a2, s32 a3)
{
    return 0;
}

STUB_FUNC(0x4E8370)
u32 Map_0x370::sub_4E8370(u32 a2, s32 a3, char_type a4)
{
    return 0;
}

STUB_FUNC(0x4E8620)
void Map_0x370::sub_4E8620(s32 a2, s32 a3, s32 a4, s32 info_type_to_set, s16 info_value)
{
}

STUB_FUNC(0x4E87C0)
void Map_0x370::sub_4E87C0(s32 a2, s32 a3, s32 a4, u32* pBlockData)
{
}

STUB_FUNC(0x4E8940)
void Map_0x370::sub_4E8940(s32 a2, s32 a3, s32 a4, char_type a5)
{
}

STUB_FUNC(0x4E8A10)
void Map_0x370::sub_4E8A10(s32 a2, s32 a3)
{
}

MATCH_FUNC(0x4E8B70)
void Map_0x370::sub_4E8B70(s32 x_min, s32 x_max, s32 y_min, s32 y_max)
{
    for (s32 column_y = y_min; column_y <= y_max; ++column_y)
    {
        for (s32 column_x = x_min; column_x <= x_max; ++column_x)
        {
            Map_0x370::sub_4E8A10(column_x, column_y);
        }
    }
}

STUB_FUNC(0x4E8C00)
void Map_0x370::sub_4E8C00(u32 a2, u32 a3, u32 a4)
{
}

STUB_FUNC(0x4E8CF0)
s32 Map_0x370::sub_4E8CF0(u32* a2, u32* a3, u32* a4, u32* a5, Map_sub** a6, s32* a7)
{
    return 0;
}

STUB_FUNC(0x4E8E30)
void Map_0x370::do_process_loaded_zone_data_4E8E30()
{
}

STUB_FUNC(0x4E90E0)
void Map_0x370::sub_4E90E0(u32 a2)
{
}

STUB_FUNC(0x4E9160)
void Map_0x370::sub_4E9160(s32 size)
{
}

MATCH_FUNC(0x4E91A0)
void Map_0x370::load_mobj_4E91A0(size_t len)
{
    gmp_map_object* v3 = (gmp_map_object*)Memory::malloc_4FE4D0(len);
    field_338_pMapObjects = v3;
    File::Global_Read_4A71C0(v3, len);
    field_344_map_object_count = len / 6;
    if (6 * field_344_map_object_count != len)
    {
        FatalError_4A38C0(40, "C:\\Splitting\\Gta2\\Source\\map.cpp", 6024);
    }
}

MATCH_FUNC(0x4E9200)
void Map_0x370::load_lght_4E9200(size_t a2)
{
    gmp_map_light* v3 = (gmp_map_light*)Memory::malloc_4FE4D0(a2);
    field_33C_pLightData = v3;
    File::Global_Read_4A71C0(v3, a2);
    field_348_num_lights = a2 >> 4;
    if (16 * field_348_num_lights != a2)
    {
        FatalError_4A38C0(51, "C:\\Splitting\\Gta2\\Source\\map.cpp", 6046);
    }
}

MATCH_FUNC(0x4E9250)
void Map_0x370::load_zone_4E9250(size_t a2)
{
    gmp_map_zone* v3 = (gmp_map_zone*)Memory::malloc_4FE4D0(a2);
    field_328_pZoneData = v3;
    File::Global_Read_4A71C0(v3, a2);
    sub_4E90E0(a2);
}

MATCH_FUNC(0x4E9280)
void Map_0x370::load_anim_4E9280(size_t size)
{
    gmp_tile_animation* pAlloc = static_cast<gmp_tile_animation*>(Memory::malloc_4FE4D0(size));
    field_340_pTileAnimData = pAlloc;
    File::Global_Read_4A71C0(pAlloc, size);
    sub_4E9160(size);
}

STUB_FUNC(0x4E92B0)
void Map_0x370::load_dmap_4E92B0(s32 len)
{
    size_t a2;

    gmp_compressed_map_32* pCompressedMap = new gmp_compressed_map_32();
    if (pCompressedMap)
    {
        pCompressedMap->field_40008_pColumn = 0;
        pCompressedMap->field_4000C_block = 0;
        pCompressedMap->field_40004_num_blocks = 0;
        pCompressedMap->field_40000_column_words = 0;
    }
    else
    {
        pCompressedMap = NULL;
    }

    field_0_pDmap = pCompressedMap;
    if (field_0_pDmap == NULL)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\map.cpp", 6147);
    }

    size_t len_1 = 0x40000;
    File::Global_Read_4A71C0(field_0_pDmap, len_1);

    len_1 = 4;
    File::Global_Read_4A71C0(&field_0_pDmap->field_40000_column_words, len_1);
    if ((field_0_pDmap->field_40000_column_words + 1024) > 0x20000)
    {
        FatalError_4A38C0(1127, "C:\\Splitting\\Gta2\\Source\\map.cpp", 6150, field_0_pDmap->field_40000_column_words - 130048);
    }

    field_35C_column_word_extra = field_0_pDmap->field_40000_column_words + 1024;
    a2 = 4 * field_0_pDmap->field_40000_column_words;
    field_0_pDmap->field_40008_pColumn = (u16**)Memory::malloc_4FE4D0(4 * field_35C_column_word_extra);
    File::Global_Read_4A71C0(field_0_pDmap->field_40008_pColumn, a2);
    len_1 = 4;
    field_358_column_words = field_0_pDmap->field_40000_column_words;
    field_360_column_words = field_0_pDmap->field_40000_column_words;
    File::Global_Read_4A71C0(&field_0_pDmap->field_40004_num_blocks, len_1);
    if ((field_0_pDmap->field_40004_num_blocks + 200) > 0x20000)
    {
        FatalError_4A38C0(1129, "C:\\Splitting\\Gta2\\Source\\map.cpp", 6161, field_0_pDmap->field_40004_num_blocks - 130872);
    }
    field_350_num_blocks_extra = field_0_pDmap->field_40004_num_blocks + 200;
    size_t v17 = 12 * field_0_pDmap->field_40004_num_blocks;
    field_0_pDmap->field_4000C_block = (gmp_block_info*)Memory::malloc_4FE4D0(12 * field_350_num_blocks_extra);
    File::Global_Read_4A71C0(field_0_pDmap->field_4000C_block, v17);
    field_34C_num_blocks = field_0_pDmap->field_40004_num_blocks;
    field_354_num_blocks = field_0_pDmap->field_40004_num_blocks;
    if (len != v17 + a2 + 0x40008)
    {
        FatalError_4A38C0(1033, "C:\\Splitting\\Gta2\\Source\\map.cpp", 6170, len);
    }
}

MATCH_FUNC(0x4E94A0)
void Map_0x370::load_rgen_4E94A0()
{
    gRouteFinder_6FFDC8->Load_RGEN_588B30();
}

MATCH_FUNC(0x4E94B0)
void Map_0x370::load_chunk_4E94B0(const char_type* pChunkType, size_t chunkLen)
{
    if (!strncmp(pChunkType, "DMAP", 4u))
    {
        load_dmap_4E92B0(chunkLen);
    }
    else if (!strncmp(pChunkType, "ZONE", 4u))
    {
        load_zone_4E9250(chunkLen);
    }
    else if (!strncmp(pChunkType, "MOBJ", 4u))
    {
        load_mobj_4E91A0(chunkLen);
    }
    else if (!strncmp(pChunkType, "ANIM", 4u))
    {
        load_anim_4E9280(chunkLen);
    }
    else if (!strncmp(pChunkType, "LGHT", 4u))
    {
        load_lght_4E9200(chunkLen);
    }
    else if (!strncmp(pChunkType, "RGEN", 4u))
    {
        load_rgen_4E94A0();
    }
    else
    {
        File::Global_Seek_4A7140(&chunkLen);
    }
}

MATCH_FUNC(0x4E95A0)
void Map_0x370::process_loaded_zone_data_4E95A0()
{
    do_process_loaded_zone_data_4E8E30();
}

MATCH_FUNC(0x4E95B0)
void Map_0x370::LoadMap_4E95B0(const char_type* pGmpFileName)
{
    gmp_header header;
    File::Global_Open_4A7060(pGmpFileName);

    u32 len = sizeof(gmp_header);
    chunk_header chunkHeader;
    File::Global_Read_4A71C0(&header, len);

    len = sizeof(chunk_header);
    while (File::Global_Read_4A7210(&chunkHeader, &len))
    {
        if (chunkHeader.field_4_size != 0)
        {
            load_chunk_4E94B0(chunkHeader.field_0_type, chunkHeader.field_4_size);
        }
        len = sizeof(chunk_header);
    }
    File::Global_Close_4A70C0();

    process_loaded_zone_data_4E95A0();

    if (!field_0_pDmap)
    {
        FatalError_4A38C0(132, "C:\\Splitting\\Gta2\\Source\\map.cpp", 6329); // error map not compressed
    }
}

MATCH_FUNC(0x4E9660)
Map_0x370::Map_0x370()
{
    field_4_obj.field_320_max_idx = 0;
    field_328_pZoneData = 0;
    field_32C_pZones = 0;
    field_338_pMapObjects = 0;
    field_344_map_object_count = 0;
    field_0_pDmap = 0;
    field_330_pZoneArray = 0;
    field_334_pUnknownZoneData = 0;
    field_340_pTileAnimData = 0;
    field_33C_pLightData = 0;
    field_348_num_lights = 0;
    field_350_num_blocks_extra = 0;
    field_34C_num_blocks = 0;
    field_354_num_blocks = 0;
    field_358_column_words = 0;
    field_35C_column_word_extra = 0;
    field_360_column_words = 0;
    field_364_cur_zone_idx = 0xFFFF; // 0xFFFF literal
    field_368_zone_type = 0xff; // dl
    field_36A_zone_x = 0;
    field_36B_zone_y = 0;
    field_36C_bUnknown = 0;
    field_366 = 0;
    field_369 = 0xff; // dl
    field_36D = 0;
    field_36E = 0;
    field_36F = 0;

    gBlockInfo1_6F5F40.field_0_left = 0;
    gBlockInfo1_6F5F40.field_2_right = 0;
    gBlockInfo1_6F5F40.field_4_top = 0;
    gBlockInfo1_6F5F40.field_6_bottom = 0;
    gBlockInfo1_6F5F40.field_8_lid = 0;
    gBlockInfo1_6F5F40.field_A_arrows = 0;
    gBlockInfo1_6F5F40.field_B_slope_type = 0;

    gBlockInfo2_6F6028.field_A_arrows = 0;
    gBlockInfo2_6F6028.field_0_left = 3073;
    gBlockInfo2_6F6028.field_2_right = 3073;
    gBlockInfo2_6F6028.field_4_top = 3073;
    gBlockInfo2_6F6028.field_6_bottom = 3073;
    gBlockInfo2_6F6028.field_8_lid = 1;
    gBlockInfo2_6F6028.field_B_slope_type = 3;
}

MATCH_FUNC(0x4E9770)
Map_0x370::~Map_0x370()
{
    if (field_33C_pLightData)
    {
        crt::free(field_33C_pLightData);
    }
    field_33C_pLightData = 0;

    if (field_328_pZoneData)
    {
        crt::free(field_328_pZoneData);
    }
    field_328_pZoneData = 0;

    if (field_32C_pZones)
    {
        crt::free(field_32C_pZones);
    }
    field_32C_pZones = 0;

    if (field_338_pMapObjects)
    {
        crt::free(field_338_pMapObjects);
    }
    field_338_pMapObjects = 0;

    if (field_0_pDmap)
    {
        if (field_0_pDmap->field_40008_pColumn)
        {
            crt::free(field_0_pDmap->field_40008_pColumn);
        }
        field_0_pDmap->field_40008_pColumn = 0;

        if (field_0_pDmap->field_4000C_block)
        {
            crt::free(field_0_pDmap->field_4000C_block);
        }
        field_0_pDmap->field_4000C_block = 0;

        delete field_0_pDmap;
    }

    if (field_330_pZoneArray)
    {
        crt::free(field_330_pZoneArray);
    }
    field_330_pZoneArray = 0;

    if (field_334_pUnknownZoneData)
    {
        crt::free(field_334_pUnknownZoneData);
    }
    field_334_pUnknownZoneData = 0;

    if (field_340_pTileAnimData)
    {
        crt::free(field_340_pTileAnimData);
    }
    field_340_pTileAnimData = 0;
}
