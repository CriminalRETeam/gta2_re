#include "map_0x370.hpp"
#include "Car_BC.hpp"
#include "Fix16_Rect.hpp"
#include "Function.hpp"
#include "Game_0x40.hpp"
#include "Globals.hpp"
#include "Object_5C.hpp"
#include "PurpleDoom.hpp"
#include "RouteFinder.hpp"
#include "Zones_CA8.hpp"
#include "chunk.hpp"
#include "crt_stubs.hpp"
#include "error.hpp"
#include "file.hpp"
#include "gtx_0x106C.hpp"
#include "memory.hpp"

EXPORT_VAR Map_0x370* gMap_0x370_6F6268;
GLOBAL(gMap_0x370_6F6268, 0x6F6268);

EXPORT_VAR gmp_block_info* gBlockInfo0_6F5EB0;
GLOBAL(gBlockInfo0_6F5EB0, 0x6F5EB0);

EXPORT_VAR gmp_block_info gBlockInfo1_6F5F40;
GLOBAL(gBlockInfo1_6F5F40, 0x6F5F40);

EXPORT_VAR gmp_block_info gBlockInfo2_6F6028;
GLOBAL(gBlockInfo2_6F6028, 0x6F6028);

EXPORT_VAR gmp_map_slope byte_6F5BA8[64];   // maybe size 64
GLOBAL(byte_6F5BA8, 0x6F5BA8);

EXPORT_VAR gmp_map_slope* dword_6F5EC8;
GLOBAL(dword_6F5EC8, 0x6F5EC8);

EXPORT_VAR s16 word_6F6002;
GLOBAL(word_6F6002, 0x6F6002);

EXPORT_VAR s32 gPurple_right_6F5B80;
GLOBAL(gPurple_right_6F5B80, 0x6F5B80);

EXPORT_VAR s32 gPurple_left_6F5FD4;
GLOBAL(gPurple_left_6F5FD4, 0x6F5FD4);

EXPORT_VAR s32 dword_6F620C;
GLOBAL(dword_6F620C, 0x6F620C);

EXPORT_VAR s32 dword_6F5BA0;
GLOBAL(dword_6F5BA0, 0x6F5BA0);

EXPORT_VAR s32 dword_6F6248;
GLOBAL(dword_6F6248, 0x6F6248);

EXPORT_VAR s32 dword_6F5FAC;
GLOBAL(dword_6F5FAC, 0x6F5FAC);

EXPORT_VAR Fix16 dword_6F610C;
GLOBAL(dword_6F610C, 0x6F610C);

EXPORT_VAR Fix16 dword_6F6130;
GLOBAL(dword_6F6130, 0x6F6130);

EXPORT_VAR Fix16 dword_6F60C0;
GLOBAL(dword_6F60C0, 0x6F60C0);

EXPORT_VAR Fix16 dword_6F5ED8;
GLOBAL(dword_6F5ED8, 0x6F5ED8);

EXPORT_VAR Fix16 dword_6F5FA8;
GLOBAL(dword_6F5FA8, 0x6F5FA8);

EXPORT_VAR Fix16 dword_6F6214;
GLOBAL(dword_6F6214, 0x6F6214);

EXPORT_VAR s32 dword_6F6164;
GLOBAL(dword_6F6164, 0x6F6164);

EXPORT_VAR s32 dword_6F6148;
GLOBAL(dword_6F6148, 0x6F6148);

EXPORT_VAR s32 dword_6F613C;
GLOBAL(dword_6F613C, 0x6F613C);

static inline bool Overlaps(gmp_map_zone* pZone, u8 x, u8 y)
{
    return x >= pZone->field_1_x && y >= pZone->field_2_y && x < pZone->field_1_x + pZone->field_3_w &&
        y < pZone->field_2_y + pZone->field_4_h;
}

static inline s32 clamp_z_from_column(s32 height, s32 offset, s32 z_pos)
{
    if (z_pos >= height)
    {
        return height - offset - 1; // highest block possible on the column
    }
    else
    {
        return z_pos - offset;
    }
}

static inline u8 get_slope_bits(u8& slope_byte) // slope_byte but with its first 2 bits cleared
{
    return slope_byte & 0xFCu;
}

static inline u8 get_slope_idx(u8& slope_byte) // 0 to 63
{
    return slope_byte >> 2;
}

static inline bool is_air_type(u8& slope_byte)
{
    return (slope_byte & 3) == 0;
}

static inline bool is_gradient_slope(u8& slope_byte)
{
    u8 slope = get_slope_bits(slope_byte);
    return slope > 0 && slope < 0xB4u;  // slope idx in range 1 to 45
}

static inline u8 get_block_type(u8& slope_byte)
{
    return slope_byte & 3;
}

static inline bool is_partial_block(s32& slope)
{
    return slope >= 0xD4 && slope <= 0xF4;
}

static inline bool is_diagonal_block(s32& slope)
{
    return slope >= 0xC4 && slope <= 0xD0;
}

static inline u16 get_tile_idx(u16& side_word)
{
    return side_word & 1023;
}

static inline bool has_any_green_arrows(u8& arrow_data)
{
    return (arrow_data & 0xF) != 0;
}

static inline bool check_green_up(u8& arrow_data)
{
    return (arrow_data >> 2) & 1;
}

static inline bool check_green_down(u8& arrow_data)
{
    return (arrow_data >> 3) & 1;
}

static inline bool check_green_right(u8& arrow_data)
{
    return (arrow_data >> 1) & 1;
}

static inline bool check_green_left(u8& arrow_data)
{
    return arrow_data & 1;
}

static inline bool has_bottom(gmp_block_info* pBlock)
{
    return pBlock->field_6_bottom != 0;
}

static inline bool has_top(gmp_block_info* pBlock)
{
    return pBlock->field_4_top != 0;
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
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4DF3E0)
u8* Map_0x370::sub_4DF3E0(u8 a2, u8 a3, char_type a4)
{
    NOT_IMPLEMENTED;
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

MATCH_FUNC(0x4DFB90)
void Map_0x370::sub_4DFB90()
{
    gmp_map_object* pMapObjects = field_338_pMapObjects;
    for (u32 i = 0; i < this->field_344_map_object_count; i++, ++pMapObjects)
    {
        Fix16 x_pos = Fix16::ctor_462ED0(pMapObjects->field_0_x);
        Fix16 y_pos = Fix16::ctor_462ED0(pMapObjects->field_2_y);

        Ang16 rotation = pMapObjects->field_4_rotation.convert_to_ang16();
        rotation.Normalize();

        Fix16 temp_z;

        gObject_5C_6F8F84->sub_5299B0(pMapObjects->field_5_object_type,
                                      x_pos,
                                      y_pos,
                                      *Map_0x370::FindGroundZForCoord_4E5B60(&temp_z, x_pos, y_pos),
                                      rotation);
    }
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
    NOT_IMPLEMENTED;
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
gmp_block_info* Map_0x370::sub_4DFE60(s32 x, s32 y, s32 z)
{
    gmp_col_info* v5 = (gmp_col_info*)&field_0_pDmap->field_40008_pColumn[field_0_pDmap->field_0_base[y][x]];
    if (z < v5->field_0_height)
    {
        if (z >= v5->field_1_offset)
        {
            gmp_block_info* v7_block = &field_0_pDmap->field_4000C_block[v5->field_4_blockd[z - v5->field_1_offset]];
            s32 slope_type = get_slope_bits(v7_block->field_B_slope_type);

            if (!is_partial_block(slope_type))
            {
                if (is_diagonal_block(slope_type))
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

Fix16 dword_6F6110 = Fix16(1); // = 0x4000; // todo

MATCH_FUNC(0x4DFF60)
DWORD Map_0x370::sub_4DFF60(Fix16 x_coord, Fix16 y_coord, Fix16 z_coord)
{
    gmp_block_info* pBlock1 = get_block_4DFE10((x_coord - dword_6F6110).ToInt(), y_coord.ToInt(), z_coord.ToInt());
    if (pBlock1 && pBlock1->field_2_right != 0)
    {
        u32 spec = gGtx_0x106C_703DD4->field_6C_spec[get_tile_idx(pBlock1->field_2_right)];
        if (spec == tile_spec::road_junction_special)
        {
            return 1;
        }
        return spec;
    }

    gmp_block_info* pBlock2 = get_block_4DFE10(x_coord.ToInt(), y_coord.ToInt(), z_coord.ToInt());
    if (pBlock2 && pBlock2->field_0_left != 0)
    {
        u32 spec = gGtx_0x106C_703DD4->field_6C_spec[get_tile_idx(pBlock2->field_0_left)];
        if (spec == tile_spec::road_junction_special)
        {
            return 1;
        }
        return spec;
    }

    return 0;
}

MATCH_FUNC(0x4E0000)
s32 Map_0x370::sub_4E0000(Fix16 x_pos, Fix16 y_pos, Fix16 z_pos)
{
    s32 spec;
    gmp_block_info* block_4DFE10 = Map_0x370::get_block_4DFE10(x_pos.ToInt(), (y_pos - dword_6F6110).ToInt(), z_pos.ToInt());
    if (block_4DFE10 && has_bottom(block_4DFE10))
    {
        spec = gGtx_0x106C_703DD4->field_6C_spec[get_tile_idx(block_4DFE10->field_6_bottom)];
        if (spec == tile_spec::road_junction_special)
        {
            return 1;
        }
    }
    else
    {
        gmp_block_info* pBlock = Map_0x370::get_block_4DFE10(x_pos.ToInt(), y_pos.ToInt(), z_pos.ToInt());
        if (pBlock && has_top(pBlock))
        {
            spec = gGtx_0x106C_703DD4->field_6C_spec[get_tile_idx(pBlock->field_4_top)];
            if (spec == tile_spec::road_junction_special)
            {
                return 1;
            }
        }
        else
        {
            return 0;
        }
    }
    return spec;
}

MATCH_FUNC(0x4E00A0)
s32 Map_0x370::sub_4E00A0(Fix16 x, Fix16 y, Fix16 z)
{
    if (z >= dword_6F610C)
    {
        if (z < dword_6F6130)
        {
            gmp_block_info* pBlock = gMap_0x370_6F6268->get_block_4DFE10(x.ToInt(), y.ToInt(), z.ToInt());
            if (pBlock)
            {
                u16 lid = pBlock->field_8_lid;
                if (lid)
                {
                    s32 spec = gGtx_0x106C_703DD4->field_6C_spec[get_tile_idx(lid)];
                    if (spec == tile_spec::road_junction_special)
                    {
                        return 1;
                    }
                    return spec;
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
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4E11E0)
char_type Map_0x370::sub_4E11E0(s32* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4E1520)
char_type Map_0x370::sub_4E1520(s32 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x4E18A0)
bool Map_0x370::sub_4E18A0(s32 x_min, s32 x_max, s32 y_min, s32 y_max, s32 z)
{
    gmp_block_info* pBlock;

    for (s32 y_pos = y_min; y_pos <= y_max; y_pos++)
    {
        for (s32 x_pos = x_min; x_pos <= x_max; x_pos++)
        {
            if (x_pos < x_max)
            {
                pBlock = Map_0x370::sub_4DFE60(x_pos, y_pos, z);
                if (pBlock)
                {
                    if ((pBlock->field_2_right & 0x400) != 0) // is wall tile
                    {
                        return true;
                    }
                }
                pBlock = Map_0x370::sub_4DFE60(x_pos + 1, y_pos, z);
                if (pBlock)
                {
                    if ((pBlock->field_0_left & 0x400) != 0) // is wall tile
                    {
                        return true;
                    }
                }
            }

            if (y_pos < y_max)
            {
                pBlock = Map_0x370::sub_4DFE60(x_pos, y_pos, z);
                if (pBlock)
                {
                    if ((pBlock->field_6_bottom & 0x400) != 0) // is wall tile
                    {
                        return true;
                    }
                }
                pBlock = Map_0x370::sub_4DFE60(x_pos, y_pos + 1, z);
                if (pBlock)
                {
                    if ((pBlock->field_4_top & 0x400) != 0) // is wall tile
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

STUB_FUNC(0x4E1A30)
char_type Map_0x370::sub_4E1A30(s32 a2, s32 a3, s32 a4, s32 a5, s32 a6)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4E1E00)
char_type Map_0x370::sub_4E1E00(s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x4E4460)
char_type Map_0x370::sub_4E4460(s32 a2, s32 a3, s32 a4, Sprite* a5, s16 a6)
{
    gSprite_6F61E8 = a5;
    word_6F6002 = a6;
    if (gPurple_right_6F5B80 - gPurple_left_6F5FD4 >= 3 || a2 > gPurple_left_6F5FD4 + 1 || a2 < gPurple_right_6F5B80 - 1)
    {
        if (sub_4E1E00(a2 - 1, a2 + 1, gPurple_top_6F6108, gPurple_bottom_6F5F38, a2, a3, a4))
        {
            return 1;
        }
        else if (gPurple_right_6F5B80 == a2 + 2)
        {
            return sub_4E1E00(gPurple_right_6F5B80 - 1,
                              gPurple_right_6F5B80,
                              gPurple_top_6F6108,
                              gPurple_bottom_6F5F38,
                              gPurple_right_6F5B80 - 1,
                              a3,
                              dword_6F620C);
        }
        else
        {
            return sub_4E1E00(gPurple_left_6F5FD4,
                              gPurple_left_6F5FD4 + 1,
                              gPurple_top_6F6108,
                              gPurple_bottom_6F5F38,
                              gPurple_left_6F5FD4 + 1,
                              a3,
                              dword_6F5BA0);
        }
    }
    else if (gPurple_bottom_6F5F38 - gPurple_top_6F6108 >= 3 || a3 > gPurple_top_6F6108 + 1 || a3 < gPurple_bottom_6F5F38 - 1)
    {
        if (sub_4E1E00(gPurple_left_6F5FD4, gPurple_right_6F5B80, a3 - 1, a3 + 1, a2, a3, a4))
        {
            return 1;
        }
        else if (gPurple_bottom_6F5F38 == a3 + 2)
        {
            return sub_4E1E00(gPurple_left_6F5FD4,
                              gPurple_right_6F5B80,
                              gPurple_bottom_6F5F38 - 1,
                              gPurple_bottom_6F5F38,
                              a2,
                              gPurple_bottom_6F5F38 - 1,
                              dword_6F6248);
        }
        else
        {
            return sub_4E1E00(gPurple_left_6F5FD4,
                              gPurple_right_6F5B80,
                              gPurple_top_6F6108,
                              gPurple_top_6F6108 + 1,
                              a2,
                              gPurple_top_6F6108 + 1,
                              dword_6F5FAC);
        }
    }
    else
    {
        return sub_4E1E00(gPurple_left_6F5FD4, gPurple_right_6F5B80, gPurple_top_6F6108, gPurple_bottom_6F5F38, a2, a3, a4);
    }
}

STUB_FUNC(0x4E4630)
char_type Map_0x370::sub_4E4630(Fix16 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x4E4770)
bool Map_0x370::sub_4E4770(Fix16 a2)
{
    s32 z = a2.ToInt();
    bool flag2 = false;
    bool flag1 = false;

    for (s32 y_pos = gPurple_top_6F6108; y_pos <= gPurple_bottom_6F5F38; y_pos++)
    {
        for (s32 x_pos = gPurple_left_6F5FD4; x_pos <= gPurple_right_6F5B80; x_pos++)
        {
            gBlockInfo0_6F5EB0 = Map_0x370::get_block_4DFE10(x_pos, y_pos, z);

            if (gBlockInfo0_6F5EB0 != NULL)
            {
                if (is_gradient_slope(gBlockInfo0_6F5EB0->field_B_slope_type) 
                    && !is_air_type(gBlockInfo0_6F5EB0->field_B_slope_type))
                {
                    if (flag1)
                    {
                        return true;
                    }
                    flag2 = true;
                    continue;
                }
            }

            if (flag2)
            {
                return true;
            }
            flag1 = true;
        }
    }
    return false;
}

STUB_FUNC(0x4E4820)
char_type Map_0x370::sub_4E4820(u32* a2, char_type a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4E4930)
char_type Map_0x370::sub_4E4930(u8* a1, u8* a2, u8* a3, char_type a4)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x4E4AC0)
bool Map_0x370::sub_4E4AC0(char_type block_type) // __stdcall ?
{
    if (dword_6F6164 >= 0 && dword_6F6148 >= 0 && dword_6F6164 <= 255 && dword_6F6148 <= 255)
    {
        if (block_type == PAVEMENT)
        {
            if (gMap_0x370_6F6268->sub_4E4BB0(dword_6F6164, dword_6F6148, dword_6F613C))
            {
                return true;
            }
        }
        else if (gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(dword_6F6164, dword_6F6148, dword_6F613C) == ROAD)
        {
            return true;
        }
    }
    return false;
}

MATCH_FUNC(0x4E4B40)
bool Map_0x370::sub_4E4B40(s32 a1, gmp_block_info* a2)
{
    switch (a1)
    {
        case 1:
            return check_green_up(a2->field_A_arrows);
            break;
        case 2:
            return check_green_down(a2->field_A_arrows);
            break;
        case 3:
            return check_green_right(a2->field_A_arrows);
            break;
        case 4:
            return check_green_left(a2->field_A_arrows);
            break;
        default:
            return true;
            break;
    }
}

MATCH_FUNC(0x4E4BB0)
gmp_block_info* Map_0x370::sub_4E4BB0(s32 x, s32 y, s32& z) //  TODO: rename it to FindPavementBlockForCoord_4E4BB0
{
    gmp_col_info* pColumn = (gmp_col_info*)&this->field_0_pDmap->field_40008_pColumn[this->field_0_pDmap->field_0_base[y][x]];
    for (s32 curr_z_pos = pColumn->field_0_height - pColumn->field_1_offset - 1; curr_z_pos >= 0; curr_z_pos--)
    {
        //  Begin with the highest non-empty block in the column
        gBlockInfo0_6F5EB0 = this->field_0_pDmap->get_block(pColumn->field_4_blockd[curr_z_pos]); //  get the block
        switch (get_block_type(gBlockInfo0_6F5EB0->field_B_slope_type))
        {
            case AIR:
                continue;
            case PAVEMENT:
                z = curr_z_pos + pColumn->field_1_offset;
                return gBlockInfo0_6F5EB0;
            default:
                return NULL;
        }
    }
    return NULL;
}

MATCH_FUNC(0x4E4C30)
gmp_block_info* Map_0x370::FindHighestBlockForCoord_4E4C30(s32 x, s32 y, s32* z_pos)
{
    gmp_col_info* v4 = (gmp_col_info*)&this->field_0_pDmap->field_40008_pColumn[field_0_pDmap->field_0_base[y][x]];

    for (s32 curr_z_pos = v4->field_0_height - v4->field_1_offset - 1; curr_z_pos >= 0; curr_z_pos--)
    {
        gBlockInfo0_6F5EB0 = field_0_pDmap->get_block(v4->field_4_blockd[curr_z_pos]);
        u8 slope_byte = gBlockInfo0_6F5EB0->field_B_slope_type;
        if (!is_air_type(slope_byte))
        {
            *z_pos = curr_z_pos + v4->field_1_offset;
            return gBlockInfo0_6F5EB0;
        }
    }
    return 0;
}

MATCH_FUNC(0x4E4CB0)
gmp_block_info* Map_0x370::sub_4E4CB0(s32 x, s32 y, s32& z)
{
    gmp_col_info* pColumn = (gmp_col_info*)&this->field_0_pDmap->field_40008_pColumn[this->field_0_pDmap->field_0_base[y][x]];

    if (z < pColumn->field_1_offset)
    {
        return 0;
    }

    s32 curr_z_pos = (z >= pColumn->field_0_height ? pColumn->field_0_height - pColumn->field_1_offset - 1 : z - pColumn->field_1_offset);

    for (; curr_z_pos >= 0; curr_z_pos--)
    {
        gBlockInfo0_6F5EB0 = field_0_pDmap->get_block(pColumn->field_4_blockd[curr_z_pos]);
        if (get_block_type(gBlockInfo0_6F5EB0->field_B_slope_type) != AIR)
        {
            z = curr_z_pos + pColumn->field_1_offset;
            return gBlockInfo0_6F5EB0;
        }
    }
    return NULL;
}

MATCH_FUNC(0x4E4D40)
Fix16* Map_0x370::sub_4E4D40(Fix16* a2, Fix16 x_pos, Fix16 y_pos, Fix16 z_pos)
{
    gmp_block_info* block_4DFE10;
    u8 slope_byte;
    Fix16 new_z;

    if (z_pos.GetFracValue() == dword_6F610C 
        || (block_4DFE10 = Map_0x370::get_block_4DFE10(x_pos.ToInt(), y_pos.ToInt(), z_pos.ToInt())) == NULL 
        || (slope_byte = block_4DFE10->field_B_slope_type, 
            !is_gradient_slope(slope_byte))
        || is_air_type(slope_byte) 
        || (new_z = z_pos.GetRoundValue(), 
            Map_0x370::UpdateZFromSlopeAtCoord_4E5BF0(x_pos, y_pos, new_z), 
            new_z > z_pos))
    {
        s32 v14 = z_pos.ToInt() - 1;
        gmp_block_info* v11 = Map_0x370::sub_4E4CB0(x_pos.ToInt(), y_pos.ToInt(), v14);
        gBlockInfo0_6F5EB0 = v11;
        if (v11 == NULL)
        {
            *a2 = dword_6F6110;
            return a2;
        }
        slope_byte = v11->field_B_slope_type;

        if (is_gradient_slope(slope_byte) && !is_air_type(slope_byte))
        {
            new_z = Fix16(v14);
            Map_0x370::UpdateZFromSlopeAtCoord_4E5BF0(x_pos, y_pos, new_z);
        }
        else
        {
            new_z = Fix16(v14) + 1;
        }
    }
    *a2 = new_z;
    return a2;
}

MATCH_FUNC(0x4E4E50)
Fix16* Map_0x370::sub_4E4E50(Fix16* a2, Fix16 x_pos, Fix16 y_pos, Fix16 z_pos)
{
    for (Fix16 curr_z = z_pos; curr_z < z_pos + dword_6F6110; curr_z = dword_6F6110 + curr_z.GetRoundValue())
    {
        gmp_block_info* block_4DFE10 = get_block_4DFE10(x_pos.ToInt(), y_pos.ToInt(), curr_z.ToInt());
        gBlockInfo0_6F5EB0 = block_4DFE10;
        u8 slope_byte;
        if (block_4DFE10 == NULL || (slope_byte = block_4DFE10->field_B_slope_type, is_air_type(slope_byte)))
        {
            gmp_block_info* v12 = get_block_4DFE10(x_pos.ToInt(), y_pos.ToInt(), (curr_z - dword_6F6110).ToInt());
            gBlockInfo0_6F5EB0 = v12;
            if (v12)
            {
                u8 slope_byte = v12->field_B_slope_type;

                if (!is_air_type(slope_byte))
                {
                    u8 slope = get_slope_bits(slope_byte);
                    if (slope <= 0 || slope >= 0xB4u) // !is_gradient_slope(slope_byte)
                    {
                        *a2 = curr_z;
                        return a2;
                    }
                }
            }
        }
        else
        {
            if (is_gradient_slope(slope_byte))
            {
                Fix16 unk_z_coord = curr_z.GetRoundValue();
                Map_0x370::UpdateZFromSlopeAtCoord_4E5BF0(x_pos, y_pos, unk_z_coord);
                if (unk_z_coord >= curr_z)
                {
                    *a2 = unk_z_coord;
                    return a2;
                }
            }
        }
    }
    *a2 = z_pos;
    return a2;
}

STUB_FUNC(0x4E4F40)
s32* Map_0x370::sub_4E4F40(s32* a2, s32 a3, s32 a4, s32 a5)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4E5050)
s32* Map_0x370::sub_4E5050(s32* a2, s32 a3, s32 a4, s32 a5, u8* a6)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x4E5170)
bool Map_0x370::sub_4E5170(Fix16 x, Fix16 y, Fix16 z)
{
    if (z.GetFracValue() != dword_6F610C)
    {
        gBlockInfo0_6F5EB0 = Map_0x370::get_block_4DFE10(x.ToInt(), y.ToInt(), z.ToInt());
        if (gBlockInfo0_6F5EB0)
        {
            if (is_gradient_slope(gBlockInfo0_6F5EB0->field_B_slope_type) 
                && !is_air_type(gBlockInfo0_6F5EB0->field_B_slope_type))
            {
                Fix16 new_z = z.GetRoundValue();
                Map_0x370::UpdateZFromSlopeAtCoord_4E5BF0(x, y, new_z);
                if (new_z == z)
                {
                    return true;
                }
            }
        }
    }
    else
    {
        if (z == dword_6F6110)
        {
            return true;
        }
        gBlockInfo0_6F5EB0 = Map_0x370::get_block_4DFE10(x.ToInt(), y.ToInt(), (z - dword_6F6110).ToInt());
        if (gBlockInfo0_6F5EB0 && !is_air_type(gBlockInfo0_6F5EB0->field_B_slope_type))
        {
            return true;
        }
    }
    return false;
}

MATCH_FUNC(0x4E52A0)
char_type Map_0x370::sub_4E52A0(Fix16 a2, Fix16 a3, Fix16 a4)
{
    gBlockInfo0_6F5EB0 = Map_0x370::get_block_4DFE10(a2.ToInt(), a3.ToInt(), (a4 - dword_6F6110).ToInt());

    if (gBlockInfo0_6F5EB0)
    {
        if (gGtx_0x106C_703DD4->field_6C_spec[get_tile_idx(gBlockInfo0_6F5EB0->field_8_lid)] == tile_spec::water)
        {
            return 7;
        }
        if (!is_air_type(gBlockInfo0_6F5EB0->field_B_slope_type))
        {
            return 0;
        }
    }
    return 5;
}

STUB_FUNC(0x4E5300)
char_type Map_0x370::sub_4E5300(s32 a2, s32 a3, s32 a4, s32 a5)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x4E5480)
bool Map_0x370::sub_4E5480(Fix16 x, Fix16 y, Fix16 z, Fix16 unk_z_coord, Fix16* found_z)
{
    Fix16 z_floor = z.GetRoundValue();
    Fix16 unk_z_floor = unk_z_coord.GetRoundValue();

    if (z_floor == unk_z_floor)
    {
        gBlockInfo0_6F5EB0 = Map_0x370::get_block_4DFE10(x.ToInt(), y.ToInt(), z_floor.ToInt());

        if (gBlockInfo0_6F5EB0)
        {
            if (!is_air_type(gBlockInfo0_6F5EB0->field_B_slope_type) 
                && is_gradient_slope(gBlockInfo0_6F5EB0->field_B_slope_type))
            {
                *found_z = z_floor;
                Map_0x370::UpdateZFromSlopeAtCoord_4E5BF0(x, y, *found_z);
                if (unk_z_coord >= *found_z && z <= *found_z)
                {
                    return true;
                }
            }
        }
    }
    else
    {
        gBlockInfo0_6F5EB0 = Map_0x370::get_block_4DFE10(x.ToInt(), y.ToInt(), unk_z_floor.ToInt());

        if (gBlockInfo0_6F5EB0 && !is_air_type(gBlockInfo0_6F5EB0->field_B_slope_type) 
            && is_gradient_slope(gBlockInfo0_6F5EB0->field_B_slope_type))
        {
            *found_z = unk_z_floor;
            Map_0x370::UpdateZFromSlopeAtCoord_4E5BF0(x, y, *found_z);
            if (unk_z_coord >= *found_z)
            {
                return true;
            }
        }

        gBlockInfo0_6F5EB0 = Map_0x370::get_block_4DFE10(x.ToInt(), y.ToInt(), z_floor.ToInt());

        if (gBlockInfo0_6F5EB0)
        {
            if (!is_air_type(gBlockInfo0_6F5EB0->field_B_slope_type))
            {
                if (is_gradient_slope(gBlockInfo0_6F5EB0->field_B_slope_type))
                {
                    *found_z = z_floor;
                    Map_0x370::UpdateZFromSlopeAtCoord_4E5BF0(x, y, *found_z);
                    if (z <= *found_z)
                    {
                        return true;
                    }
                }
                else
                {
                    *found_z = z_floor + dword_6F6110;
                    return true;
                }
            }
        }
    }
    return false;
}

STUB_FUNC(0x4E5640)
char_type Map_0x370::sub_4E5640(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8, s32 a9)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x4E5B60)
Fix16* Map_0x370::FindGroundZForCoord_4E5B60(Fix16* found_z, Fix16 x_pos, Fix16 y_pos)
{
    s32 z_pos_int;
    gmp_block_info* pHighestBlock = Map_0x370::FindHighestBlockForCoord_4E4C30(x_pos.ToInt(), y_pos.ToInt(), &z_pos_int);
    gBlockInfo0_6F5EB0 = pHighestBlock;

    if (pHighestBlock == NULL)
    {
        *found_z = 0;
        return found_z;
    }
    else
    {
        Fix16 z_pos;
        u8 slope_byte = pHighestBlock->field_B_slope_type;
        if (is_gradient_slope(slope_byte) 
            && !is_air_type(slope_byte))
        {
            z_pos = Fix16(z_pos_int);
            Map_0x370::UpdateZFromSlopeAtCoord_4E5BF0(x_pos, y_pos, z_pos); //  get the Z position based on the slope angle
        }
        else
        {
            z_pos = Fix16(z_pos_int) + 1;
        }
        *found_z = z_pos;
        return found_z;
    }
}

MATCH_FUNC(0x4E5BF0)
u8 Map_0x370::UpdateZFromSlopeAtCoord_4E5BF0(Fix16 x_pos, Fix16 y_pos, Fix16& z_pos)
{
    gBlockInfo0_6F5EB0 = Map_0x370::get_block_4DFE10(x_pos.ToInt(), y_pos.ToInt(), z_pos.ToInt());

    if (gBlockInfo0_6F5EB0)
    {
        Fix16 grad_pos;
        Fix16 grad_scale;

        if (!is_air_type(gBlockInfo0_6F5EB0->field_B_slope_type))
        {
            dword_6F5EC8 = &byte_6F5BA8[get_slope_idx(gBlockInfo0_6F5EB0->field_B_slope_type)];

            switch (dword_6F5EC8->field_0_gradient_direction)
            {
                case 0:
                    return 0;
                case NORTH_1:
                    grad_pos = dword_6F6110 - y_pos.GetFracValue();
                    break;
                case SOUTH_2:
                    grad_pos = y_pos.GetFracValue();
                    break;
                case WEST_3:
                    grad_pos = dword_6F6110 - x_pos.GetFracValue();
                    break;
                case EAST_4:
                    grad_pos = x_pos.GetFracValue();
                    break;
                default:
                    break;
            }

            get_grad_scale_from_size_4634E0(grad_scale, dword_6F5EC8->field_1_gradient_size);

            Fix16 relative_z_pos = dword_6F5EC8->field_4_zpos_lower + (grad_pos * grad_scale);

            if (relative_z_pos == dword_6F610C)
            {
                relative_z_pos = dword_6F60C0;
            }
            z_pos = relative_z_pos + z_pos.GetRoundValue();
            return dword_6F5EC8->field_0_gradient_direction;
        }
    }
    return 0;
}

MATCH_FUNC(0x4E5FC0)
s32 Map_0x370::sub_4E5FC0(gmp_block_info* pBlock, char_type a2)
{
    s32 result = 0;
    if (gRouteFinder_6FFDC8->sub_588CA0(pBlock, 3, 4))
    {
        if (a2 != 0)
        {
            result = 3;
        }
        else
        {
            result = 4;
        }
    }
    else if (gRouteFinder_6FFDC8->sub_588CA0(pBlock, 3, 2))
    {
        if (a2 != 0)
        {
            result = 2;
        }
        else
        {
            result = 1;
        }
    }
    else if (gRouteFinder_6FFDC8->sub_588CA0(pBlock, 3, 3))
    {
        if (a2 != 0)
        {
            result = 4;
        }
        else
        {
            result = 3;
        }
    }
    else if (gRouteFinder_6FFDC8->sub_588CA0(pBlock, 3, 1))
    {
        if (a2 != 0)
        {
            result = 1;
        }
        else
        {
            result = 2;
        }
    }
    return result;
}

STUB_FUNC(0x4E6190)
s16 Map_0x370::sub_4E6190(Fix16 x, Fix16 y, Fix16 z, s32 a5, char_type a6)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x4E62B0)
Fix16* Map_0x370::sub_4E62B0(Fix16* a1, Fix16 a2)
{
    *a1 = dword_6F6110 + a2.GetRoundValue();
    return a1;
}

MATCH_FUNC(0x4E62D0)
gmp_block_info* Map_0x370::sub_4E62D0(s32 x, s32 y, s32& found_z) // TODO: rename it to FindRailwayAtCoord_4E62D0
{
    gmp_col_info* pColumn = (gmp_col_info*)&field_0_pDmap->field_40008_pColumn[field_0_pDmap->field_0_base[y][x]];

    for (s32 curr_z = pColumn->field_0_height - pColumn->field_1_offset - 1; curr_z >= 0; curr_z--)
    {
        gBlockInfo0_6F5EB0 = field_0_pDmap->get_block(pColumn->field_4_blockd[curr_z]);
        if (get_block_type(gBlockInfo0_6F5EB0->field_B_slope_type) == FIELD 
            && has_any_green_arrows(gBlockInfo0_6F5EB0->field_A_arrows))
        {
            found_z = curr_z + pColumn->field_1_offset;
            return gBlockInfo0_6F5EB0;
        }
    }
    return 0;
}

MATCH_FUNC(0x4E6360)
gmp_block_info* Map_0x370::sub_4E6360(s32 x, s32 y, s32& z) // TODO: rename it to FindRailwayBelowZAtCoord_4E6360
{
    gmp_col_info* pColumn = (gmp_col_info*)&field_0_pDmap->field_40008_pColumn[field_0_pDmap->field_0_base[y][x]];

    if (z >= pColumn->field_1_offset)
    {
        s32 curr_z = clamp_z_from_column(pColumn->field_0_height, pColumn->field_1_offset, z);

        for (; curr_z >= 0; curr_z--)
        {
            gmp_block_info* pBlock = field_0_pDmap->get_block(pColumn->field_4_blockd[curr_z]);
            gBlockInfo0_6F5EB0 = pBlock;

            if (get_block_type(pBlock->field_B_slope_type) == FIELD 
                && has_any_green_arrows(pBlock->field_A_arrows))
            {
                z = curr_z + pColumn->field_1_offset;
                return gBlockInfo0_6F5EB0;
            }
        }
    }
    return NULL;
}

MATCH_FUNC(0x4E6400)
Fix16* Map_0x370::sub_4E6400(Fix16* a2, Fix16 x_pos, Fix16 y_pos, Fix16 z_pos)
{
    gmp_block_info* block_4DFE10;
    u8 slope_byte;
    Fix16 new_z;

    if (z_pos.GetFracValue() == dword_6F610C 
        || (block_4DFE10 = get_block_4DFE10(x_pos.ToInt(), y_pos.ToInt(), z_pos.ToInt())) == NULL 
        || (slope_byte = block_4DFE10->field_B_slope_type, 
            !is_gradient_slope(slope_byte)) 
        || is_air_type(slope_byte) 
        || (new_z = z_pos.GetRoundValue(), 
            Map_0x370::UpdateZFromSlopeAtCoord_4E5BF0(x_pos, y_pos, new_z), new_z > z_pos))
    {
        s32 v14 = z_pos.ToInt() - 1;
        gmp_block_info* pBlock = Map_0x370::sub_4E6360(x_pos.ToInt(), y_pos.ToInt(), v14);
        gBlockInfo0_6F5EB0 = pBlock;
        if (pBlock == NULL)
        {
            *a2 = Fix16(1);
            return a2;
        }
        slope_byte = pBlock->field_B_slope_type;
        if (is_gradient_slope(slope_byte) && !is_air_type(slope_byte))
        {
            new_z = Fix16(v14);
            Map_0x370::UpdateZFromSlopeAtCoord_4E5BF0(x_pos, y_pos, new_z);
        }
        else
        {
            new_z = Fix16(v14) + 1;
        }
    }
    *a2 = new_z;
    return a2;
}

MATCH_FUNC(0x4E6510)
Fix16* Map_0x370::sub_4E6510(Fix16* z, Fix16 x, Fix16 y)
{
    s32 found_z;
    gBlockInfo0_6F5EB0 = Map_0x370::sub_4E62D0(x.ToInt(), y.ToInt(), found_z);
    
    if (!gBlockInfo0_6F5EB0)
    {
        *z = 0;
        return z;
    }
    else
    {
        Fix16 new_z;
        if (is_gradient_slope(gBlockInfo0_6F5EB0->field_B_slope_type)
           && !is_air_type(gBlockInfo0_6F5EB0->field_B_slope_type))
        {
            new_z = Fix16(found_z);
            Map_0x370::UpdateZFromSlopeAtCoord_4E5BF0(x, y, new_z);
        }
        else
        {
            new_z = Fix16(found_z + 1);
        }
        *z = new_z;
        return z;
    }
}

MATCH_FUNC(0x4E65A0)
void Map_0x370::sub_4E65A0(Fix16 x, Fix16 y, Fix16* z_pos, char a5, char a6)
{
    gmp_block_info* block_4DFE10 = Map_0x370::get_block_4DFE10(x.ToInt(), y.ToInt(), (*z_pos - dword_6F6110).ToInt());
    if (get_slope_bits(block_4DFE10->field_B_slope_type) == 0xFC)
    {
        Fix16 temp;
        *z_pos = *sub_4E62B0(&temp, *z_pos);
        block_4DFE10 = Map_0x370::get_block_4DFE10(x.ToInt(), y.ToInt(), (*z_pos - dword_6F6110).ToInt());
    }
    s32 v11 = sub_4E5FC0(block_4DFE10, a5);

    if (Map_0x370::sub_4E6190(x, y, *z_pos - dword_6F6110, v11, a6) != 3)
    {
        Fix16 z_temp;
        *z_pos = *Map_0x370::sub_4E6400(&z_temp, x, y, *z_pos);
    }
}

STUB_FUNC(0x4E6660)
s32 Map_0x370::sub_4E6660(s32* a2, s32* a3, s32* a4, s32 a5)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4E7190)
s32 Map_0x370::sub_4E7190(s32* a2, s32* a3, s32* a4, s32 a5)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4E7E90)
char_type Map_0x370::sub_4E7E90(u8* a2, char_type* a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4E7FC0)
char_type Map_0x370::sub_4E7FC0(s32 a2, s32 a3, s32 a4)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x4E80A0)
void gmp_compressed_map_32::sub_4E80A0(Map_sub* a2)
{
    u32 j = 0;
    for (u32 i = 0; i < a2->field_320_max_idx; i++, j++)
    {
        u32 x = a2->field_0[j].field_4_x;
        u32 y = a2->field_0[j].field_5_y;
        field_0_base[y][x] = a2->field_0[j].field_0_column_idx;
    }
}

MATCH_FUNC(0x4E80E0)
void Map_sub::sub_4E80E0(u8 x, u8 y, u32 column_idx)
{
    u32 local_max_idx = field_320_max_idx;
    gmp_dmap_info* v6 = &field_0[0];

    for (u32 i = 0; i < local_max_idx; i++, v6++)
    {
        if (v6->field_4_x == x && v6->field_5_y == y)
        {
            v6->field_0_column_idx = column_idx;
            return;
        }
    }

    field_0[local_max_idx].set_data_463080(x, y, column_idx);
    ++field_320_max_idx;
}

MATCH_FUNC(0x4E8140)
s32 Map_0x370::sub_4E8140(gmp_block_info* pBlockInfo)
{
    s32 num_blocks = this->field_354_num_blocks;
    this->field_354_num_blocks = num_blocks + 1;
    this->field_0_pDmap->field_4000C_block[num_blocks] = *pBlockInfo;
    return num_blocks;
}

MATCH_FUNC(0x4E8180)
s32 Map_0x370::sub_4E8180(u32 read_block_idx)
{
    if (read_block_idx < this->field_34C_num_blocks)
    {
        s32 old_num_blocks = this->field_354_num_blocks;
        this->field_354_num_blocks = old_num_blocks + 1;
        field_0_pDmap->field_4000C_block[old_num_blocks] = field_0_pDmap->field_4000C_block[read_block_idx];
        return old_num_blocks;
    }
    return read_block_idx;
}

MATCH_FUNC(0x4E81D0)
s32 Map_0x370::sub_4E81D0(u32 column_idx)
{
    s32 idx = column_idx;

    if (column_idx < field_358_column_words)
    {
        gmp_col_info* pColumn = (gmp_col_info*)&field_0_pDmap->field_40008_pColumn[column_idx];
        s32 len = pColumn->field_0_height - pColumn->field_1_offset + 1;
        idx = field_360_column_words;
        field_360_column_words += len;
        memcpy(&field_0_pDmap->field_40008_pColumn[idx], pColumn, 4 * len);
    }
    return idx;
}

MATCH_FUNC(0x4E8220)
s32 Map_0x370::sub_4E8220(u32 column_idx, s32 z)
{
    gmp_col_info* v5 = (gmp_col_info*)&field_0_pDmap->field_40008_pColumn[column_idx];

    if (z < v5->field_0_height && z >= v5->field_1_offset)
    {
        return Map_0x370::sub_4E81D0(column_idx);
    }

    gmp_col_info* v8 = (gmp_col_info*)&field_0_pDmap->field_40008_pColumn[field_360_column_words];
    s32 local_column_words = field_360_column_words;

    if (z >= v5->field_0_height)
    {
        v8->field_0_height = z + 1;
        v8->field_1_offset = v5->field_1_offset;

        for (s32 i = 0; i < v5->field_0_height - v5->field_1_offset; i++)
        {
            v8->field_4_blockd[i] = v5->field_4_blockd[i];
        }

        for (s32 j = v5->field_0_height - v5->field_1_offset; j < v8->field_0_height - v8->field_1_offset; j++)
        {
            v8->field_4_blockd[j] = 0;
        }
    }
    else
    {
        v8->field_0_height = v5->field_0_height;
        v8->field_1_offset = z;

        s32 v13 = v5->field_1_offset - (u8)z;

        for (s32 j = 0; j < v13; j++)
        {
            v8->field_4_blockd[j] = 0;
        }

        for (s32 k = 0; k < v5->field_0_height - v5->field_1_offset; k++)
        {
            v8->field_4_blockd[k + v13] = v5->field_4_blockd[k];
        }
    }
    field_360_column_words += v8->field_0_height - v8->field_1_offset + 1;
    return local_column_words;
}

STUB_FUNC(0x4E8370)
u32 Map_0x370::sub_4E8370(u32 a2, s32 a3, char_type a4)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x4E8620)
void Map_0x370::sub_4E8620(s32 x, s32 y, s32 z, s32 info_type_to_set, u16 info_value)
{
    s32 column_idx = Map_0x370::sub_4E81D0(field_0_pDmap->field_0_base[y][x]);
    field_0_pDmap->field_0_base[y][x] = column_idx;
    field_4_obj.sub_4E80E0(x, y, column_idx);

    gmp_col_info* pColumn = (gmp_col_info*)&this->field_0_pDmap->field_40008_pColumn[column_idx];

    s32 block_idx = Map_0x370::sub_4E8180(pColumn->field_4_blockd[z - pColumn->field_1_offset]);

    pColumn->field_4_blockd[z - pColumn->field_1_offset] = block_idx;
    gBlockInfo0_6F5EB0 = &this->field_0_pDmap->field_4000C_block[block_idx];

    switch (info_type_to_set)
    {
        case 1:
            gBlockInfo0_6F5EB0->field_0_left = info_value;
            break;
        case 2:
            gBlockInfo0_6F5EB0->field_2_right = info_value;
            break;
        case 3:
            gBlockInfo0_6F5EB0->field_4_top = info_value;
            break;
        case 4:
            gBlockInfo0_6F5EB0->field_6_bottom = info_value;
            break;
        case 5:
            gBlockInfo0_6F5EB0->field_8_lid = info_value;
            break;
        case 6:
            gBlockInfo0_6F5EB0->field_B_slope_type = info_value;
            break;
        case 7:
            gBlockInfo0_6F5EB0->field_A_arrows = info_value;
            break;
        default:
            return;
    }
}

MATCH_FUNC(0x4E87C0)
void Map_0x370::sub_4E87C0(s32 x, s32 y, s32 z, gmp_block_info* pBlockData)
{
    s32 column_idx = Map_0x370::sub_4E8220(field_0_pDmap->field_0_base[y][x], z);
    field_0_pDmap->field_0_base[y][x] = column_idx;
    field_4_obj.sub_4E80E0(x, y, column_idx);
    gmp_col_info* pColumn = (gmp_col_info*)&field_0_pDmap->field_40008_pColumn[column_idx];
    u32 block_id = pColumn->field_4_blockd[z - pColumn->field_1_offset];

    if (block_id < field_34C_num_blocks)
    {
        pColumn->field_4_blockd[z - pColumn->field_1_offset] = Map_0x370::sub_4E8140(pBlockData);
    }
    else
    {
        field_0_pDmap->field_4000C_block[block_id] = *pBlockData;
    }
}

MATCH_FUNC(0x4E8940)
void Map_0x370::sub_4E8940(s32 x_pos, s32 y_pos, s32 offset, char_type do_drop)
{
    const s32 column_idx = sub_4E8370(field_0_pDmap->field_0_base[y_pos][x_pos], offset, do_drop);
    if (column_idx != -1)
    {
        field_0_pDmap->field_0_base[y_pos][x_pos] = column_idx;
        field_4_obj.sub_4E80E0(x_pos, y_pos, column_idx);
    }
}

STUB_FUNC(0x4E8A10)
void Map_0x370::sub_4E8A10(s32 a2, s32 a3)
{
    NOT_IMPLEMENTED;
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
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x4E8CF0)
s32 Map_0x370::sub_4E8CF0(u32* a2, u32* a3, u32* a4, u32* a5, Map_sub** a6, s32* a7)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4E8E30)
void Map_0x370::do_process_loaded_zone_data_4E8E30()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x4E90E0)
void Map_0x370::sub_4E90E0(u32 chunk_size)
{
    u32 in_use_size = 0;
    s32 num_zones = 0;
    s32 zone_data_size;

    for (gmp_map_zone* pZone = this->field_328_pZoneData; in_use_size < chunk_size; ++num_zones)
    {
        zone_data_size = pZone->field_5_name_length + 6;
        in_use_size += zone_data_size;
        pZone = (gmp_map_zone*)((u8*)pZone + zone_data_size);
    }
    gmp_map_zone** v7 = (gmp_map_zone**)Memory::malloc_4FE4D0(4 * num_zones + 4);
    this->field_32C_pZones = v7;

    in_use_size = 0;
    *(u16*)v7 = num_zones; // TODO: fix this

    gmp_map_zone* local_pZoneData = this->field_328_pZoneData;
    if (chunk_size > 0)
    {
        u32 zone_idx = 1;
        do
        {
            this->field_32C_pZones[zone_idx++] = local_pZoneData;
            zone_data_size = local_pZoneData->field_5_name_length + 6;
            in_use_size += zone_data_size;
            local_pZoneData = (gmp_map_zone*)((char*)local_pZoneData + zone_data_size);
        } while (in_use_size < chunk_size);
    }
}

STUB_FUNC(0x4E9160)
void Map_0x370::sub_4E9160(s32 size)
{
    NOT_IMPLEMENTED;
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

MATCH_FUNC(0x4E92B0)
void Map_0x370::load_dmap_4E92B0(u32 len)
{
    field_0_pDmap = new gmp_compressed_map_32();
    if (field_0_pDmap == NULL)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\map.cpp", 6147);
    }

    File::Global_Read_4A71C0(&field_0_pDmap->field_0_base[0][0], 0x40000);
    File::Global_Read_4A71C0(&field_0_pDmap->field_40000_column_words, sizeof(DWORD));

    if ((field_0_pDmap->field_40000_column_words + 1024) > 0x20000u)
    {
        FatalError_4A38C0(1127, "C:\\Splitting\\Gta2\\Source\\map.cpp", 6150, field_0_pDmap->field_40000_column_words - 0x1FC00);
    }

    field_35C_column_word_extra = field_0_pDmap->field_40000_column_words + 1024;

    u32 m_size = field_35C_column_word_extra * sizeof(DWORD);
    u32 column_data_size = field_0_pDmap->field_40000_column_words * sizeof(DWORD);

    field_0_pDmap->field_40008_pColumn = (u16**)Memory::malloc_4FE4D0(m_size);

    File::Global_Read_4A71C0(field_0_pDmap->field_40008_pColumn, column_data_size);

    field_358_column_words = field_0_pDmap->field_40000_column_words;
    field_360_column_words = field_358_column_words;

    File::Global_Read_4A71C0(&field_0_pDmap->field_40004_num_blocks, 4);

    if ((field_0_pDmap->field_40004_num_blocks + 200) > 0x20000u)
    {
        FatalError_4A38C0(1129, "C:\\Splitting\\Gta2\\Source\\map.cpp", 6161, field_0_pDmap->field_40004_num_blocks - 130872);
    }
    field_350_num_blocks_extra = field_0_pDmap->field_40004_num_blocks + 200;

    u32 m_size2 = field_350_num_blocks_extra * sizeof(gmp_block_info);
    u32 block_info_size = field_0_pDmap->field_40004_num_blocks * sizeof(gmp_block_info);

    field_0_pDmap->field_4000C_block = (gmp_block_info*)Memory::malloc_4FE4D0(m_size2);

    File::Global_Read_4A71C0(field_0_pDmap->field_4000C_block, block_info_size);

    field_34C_num_blocks = field_0_pDmap->field_40004_num_blocks;
    field_354_num_blocks = field_34C_num_blocks;

    if (len != block_info_size + column_data_size + 0x40008)
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
