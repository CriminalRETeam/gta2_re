#include "Function.hpp"
#include "map_0x370.hpp"
#include "gtx_0x106C.hpp"
#include "file.hpp"
#include "error.hpp"
#include "chunk.hpp"

Map_0x370* gMap_0x370_6F6268;

gmp_block_info gBlockInfo1_6F5F40;
gmp_block_info gBlockInfo2_6F6028;

MATCH_FUNC(0x452980)
gmp_block_info* Map_0x370::get_block_452980(unsigned __int8 x_coord, unsigned __int8 y_coord, unsigned __int8 z_coord)
{
    gmp_col_info* pColData = reinterpret_cast<gmp_col_info*>(&field_0_pDmap->field_40008_pColumn[field_0_pDmap->field_0_base[y_coord][x_coord]]);
    if (z_coord >= pColData->field_0_height || z_coord < pColData->field_1_offset)
    {
        return 0;
    }
    else
    {
        return &field_0_pDmap->field_4000C_block[pColData->field_4_blockd[z_coord - pColData->field_1_offset]];
    }
}

MATCH_FUNC(0x4DEFD0)
gmp_map_zone* Map_0x370::zone_by_name_4DEFD0(const char *pZoneName)
{
    unsigned int name_len = strlen(pZoneName);
    if (field_328_pZoneData)
    {
        for (field_364_cur_zone_idx = 0; field_364_cur_zone_idx < *(WORD *)field_32C_pZones; field_364_cur_zone_idx++)
        {
            gmp_map_zone *pZone = get_zone_4DFB30(field_364_cur_zone_idx);
            if (pZone->field_5_name_length == name_len  &&
                memcmp(pZone->field_6_name, pZoneName, name_len ) == 0)
            {
                return pZone;
            }
        }
    }
    return 0;
}

MATCH_FUNC(0x4DF050)
int Map_0x370::zone_idx_by_name_4DF050(const char *pZoneName, BYTE zone_name_len)
{
    if (!field_328_pZoneData)
    {
        return 0;
    }

    for (field_364_cur_zone_idx = 0; field_364_cur_zone_idx < *(WORD *)field_32C_pZones; field_364_cur_zone_idx++)
    {
        gmp_map_zone * pZone = get_zone_4DFB30(field_364_cur_zone_idx);
        // note: esp/stack reorder due to int -> byte cast on zone_name_len wrong type
        if (pZone->field_5_name_length == zone_name_len &&
            _strnicmp(pZone->field_6_name, pZoneName, zone_name_len) == 0)
        {
            return field_364_cur_zone_idx;
        }
    }
    return -1;
}

MATCH_FUNC(0x4DF0F0)
gmp_map_zone* Map_0x370::zone_by_type_bounded_4DF0F0(unsigned char zone_type)
{
    static short sLastIdx_6F626C;

    __int16 local_array[40];

    if (field_328_pZoneData)
    {
        unsigned __int16 idx = 0;
        field_36C_bUnknown = 0;
        field_368_zone_type = zone_type;
        field_364_cur_zone_idx = 0;

        for (field_364_cur_zone_idx = 0; field_364_cur_zone_idx < *(WORD *)field_32C_pZones; field_364_cur_zone_idx++)
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

    return 0;
}

MATCH_FUNC(0x4DF1D0)
gmp_map_zone* Map_0x370::first_zone_by_type_4DF1D0(unsigned char zone_type)
{
    if (field_328_pZoneData)
    {
        field_36C_bUnknown = 0;
        field_368_zone_type = zone_type;
        field_364_cur_zone_idx = 0;

        for (field_364_cur_zone_idx = 0; field_364_cur_zone_idx < *(WORD *)field_32C_pZones; field_364_cur_zone_idx++)
        {
            gmp_map_zone *pZone = get_zone_4DFB30(field_364_cur_zone_idx);
            if (pZone->field_0_zone_type == field_368_zone_type)
            {
                return pZone;
            }
        }
    }
    return 0;
}

// nomatch
gmp_map_zone* Map_0x370::zone_by_pos_and_type_4DF4D0(char zone_x, char zone_y, unsigned char zone_type)
{
    if (field_328_pZoneData)
    {
        field_368_zone_type = zone_type;
        field_36A_zone_x = zone_x;
        field_36B_zone_y = zone_y;
        field_36C_bUnknown = 1;

        for (field_364_cur_zone_idx = 0; field_364_cur_zone_idx < *(WORD *)field_32C_pZones; field_364_cur_zone_idx++)
        {
            gmp_map_zone *pZone = get_zone_4DFB30(field_364_cur_zone_idx);
            if (field_368_zone_type == pZone->field_0_zone_type &&
                field_36A_zone_x >= pZone->field_1_x &&
                field_36B_zone_y >= pZone->field_2_y &&
                field_36A_zone_x < pZone->field_1_x + pZone->field_3_w &&
                field_36B_zone_y < pZone->field_2_y + pZone->field_4_h)
            {
                return pZone;
            }
        }
    }
    return 0;
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
gmp_zone_unknown* Map_0x370::get_nav_zone_unknown_4DF890(int zone_x, int zone_y)
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
void Map_0x370::set_nav_unknown_data_4DF8C0(int zone_idx, __int16 a3, __int16 a4, __int16 a5, __int16 a6, __int16 a7, __int16 a8, __int16 a9, __int16 a10, __int16 a11, __int16 a12, __int16 a13)
{
    get_zone_4DFB30(zone_idx);
    gmp_zone_unknown *pZoneUnknown = &field_334_pUnknownZoneData[field_330_pZoneArray[zone_idx]];
    if (a3 != -1)
    {
        pZoneUnknown->field_0 = a3;
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
gmp_zone_unknown* Map_0x370::get_zone_unknown_4DF9A0(unsigned __int16 zone_idx)
{
    return &field_334_pUnknownZoneData[field_330_pZoneArray[zone_idx]];
}

MATCH_FUNC(0x4DF9D0)
void Map_0x370::set_nav_unknown_f0_4DF9D0(unsigned __int16 a2, __int16 a3)
{
    get_zone_unknown_4DF9A0(a2)->field_0 = a3;
}

MATCH_FUNC(0x4DF9F0)
void Map_0x370::set_nav_unknown_f2_4DF9F0(unsigned __int16 a2, __int16 a3)
{
    get_zone_unknown_4DF9A0(a2)->field_2 = a3;
}

MATCH_FUNC(0x4DFA10)
void Map_0x370::set_nav_unknown_f4_4DFA10(unsigned __int16 a2, __int16 a3)
{
    get_zone_unknown_4DF9A0(a2)->field_4 = a3;
}

MATCH_FUNC(0x4DFA30)
void Map_0x370::set_nav_unknown_f6_4DFA30(unsigned __int16 a2, __int16 a3)
{
    get_zone_unknown_4DF9A0(a2)->field_6 = a3;
}

MATCH_FUNC(0x4DFA50)
void Map_0x370::set_nav_unknown_f8_4DFA50(unsigned __int16 a2, __int16 a3)
{
    get_zone_unknown_4DF9A0(a2)->field_8 = a3;
}

MATCH_FUNC(0x4DFA70)
void Map_0x370::set_nav_unknown_fA_4DFA70(unsigned __int16 a2, __int16 a3)
{
    get_zone_unknown_4DF9A0(a2)->field_A = a3;
}

MATCH_FUNC(0x4DFA90)
void Map_0x370::set_nav_unknown_fC_4DFA90(unsigned __int16 a2, __int16 a3)
{
    get_zone_unknown_4DF9A0(a2)->field_C = a3;
}

MATCH_FUNC(0x4DFAB0)
void Map_0x370::set_nav_unknown_fE_4DFAB0(unsigned __int16 a2, __int16 a3)
{
    get_zone_unknown_4DF9A0(a2)->field_E = a3;
}

MATCH_FUNC(0x4DFAD0)
void Map_0x370::set_nav_unknown_f10_4DFAD0(unsigned __int16 a2, __int16 a3)
{
    get_zone_unknown_4DF9A0(a2)->field_10 = a3;
}

MATCH_FUNC(0x4DFAF0)
void Map_0x370::set_nav_unknown_f12_4DFAF0(unsigned __int16 a2, __int16 a3)
{
    get_zone_unknown_4DF9A0(a2)->field_12 = a3;
}

MATCH_FUNC(0x4DFB10)
void Map_0x370::set_nav_unknown_f14_4DFB10(unsigned __int16 a2, __int16 a3)
{
    get_zone_unknown_4DF9A0(a2)->field_14 = a3;
}

MATCH_FUNC(0x4DFB30)
gmp_map_zone* Map_0x370::get_zone_4DFB30(unsigned __int16 zone_idx)
{
    return field_32C_pZones[zone_idx + 1];
}

MATCH_FUNC(0x4DFE10)
gmp_block_info* Map_0x370::get_block_4DFE10(int x_coord, int y_coord, int z_coord)
{
    gmp_col_info* pCol =  reinterpret_cast<gmp_col_info*>(&field_0_pDmap->field_40008_pColumn[field_0_pDmap->field_0_base[y_coord][x_coord]]);
    if (z_coord < pCol->field_0_height)
    {
        int offset = pCol->field_1_offset;
        if (z_coord >= offset)
        {
            return &field_0_pDmap->field_4000C_block[pCol->field_4_blockd[z_coord - offset]];
        }
    }
    return 0;
}

Fix16 dword_6F6110; // = 0x4000; // todo

MATCH_FUNC(0x4DFF60)
DWORD Map_0x370::sub_4DFF60(Fix16 x_coord, Fix16 y_coord, Fix16 z_coord)
{
    gmp_block_info *pBlock1 = get_block_4DFE10((x_coord - dword_6F6110).ToInt(), y_coord.ToInt(), z_coord.ToInt());
    if (pBlock1 && pBlock1->field_2_right != 0)
    {
        DWORD spec = gGtx_0x106C_703DD4->field_6C_spec[pBlock1->field_2_right & 0x3FF];
        if (spec == 3)
        {
            return 1;
        }
        return spec;
    }

    gmp_block_info *pBlock2 = get_block_4DFE10(x_coord.ToInt(), y_coord.ToInt(), z_coord.ToInt());
    if (pBlock2 && pBlock2->field_0_left != 0)
    {
        DWORD spec = gGtx_0x106C_703DD4->field_6C_spec[pBlock2->field_0_left & 0x3FF];
        if (spec == 3)
        {
            return 1;
        }
        return spec;
    }

    return 0;
}

// nomatch
gmp_map_zone* Map_0x370::nav_zone_by_pos_4DF5C0(char zone_x, char zone_y)
{
    gmp_map_zone **field_32C_pZones; // edx
    gmp_map_zone *pZone; // eax
    char field_0_zone_type; // cl

    if (!this->field_328_pZoneData)
    {
        return 0;
    }

    field_32C_pZones = this->field_32C_pZones;
    this->field_36A_zone_x = zone_x;
    this->field_36B_zone_y = zone_y;
    this->field_364_cur_zone_idx = 0;
    if (!*(WORD *)field_32C_pZones)
    {
        return 0;
    }
    while (1)
    {
        pZone = get_zone_4DFB30(this->field_364_cur_zone_idx);
        field_0_zone_type = pZone->field_0_zone_type;
        if ((pZone->field_0_zone_type == 10 || field_0_zone_type == 1 || field_0_zone_type == 15)
            && this->field_36A_zone_x >= pZone->field_1_x
            && this->field_36B_zone_y >= pZone->field_2_y
            && this->field_36A_zone_x < pZone->field_1_x + pZone->field_3_w
            && this->field_36B_zone_y < pZone->field_2_y + pZone->field_4_h)
        {
            break;
        }
        if (++this->field_364_cur_zone_idx >= *(WORD *)this->field_32C_pZones)
        {
            return 0;
        }
    }
    return pZone;
}

// stub
void Map_0x370::load_chunk_4E94B0(const char *pChunkType, int chunkLen)
{
    /*
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
    }*/
}

// stub
void Map_0x370::process_loaded_zone_data_4E95A0()
{
    //do_process_loaded_zone_data_4E8E30();
}

MATCH_FUNC(0x4E95B0)
void Map_0x370::LoadMap_4E95B0(const char *pGmpFileName)
{
    gmp_header header;
    File::Global_Open_4A7060(pGmpFileName);

    unsigned int len = sizeof(gmp_header);
    chunk_header chunkHeader;
    File::Global_Read_4A71C0(&header, &len);

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
        FatalError_4A38C0(132, "C:\\Splitting\\Gta2\\Source\\map.cpp", 6329);// error map not compressed
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
        free(field_33C_pLightData);
    }
    this->field_33C_pLightData = 0;

    if (field_328_pZoneData)
    {
        free(field_328_pZoneData);
    }
    field_328_pZoneData = 0;

    if (field_32C_pZones)
    {
        free(field_32C_pZones);
    }
    field_32C_pZones = 0;

    if (field_338_pMapObjects)
    {
        free(field_338_pMapObjects);
    }
    field_338_pMapObjects = 0;

    if (field_0_pDmap)
    {
        if (field_0_pDmap->field_40008_pColumn)
        {
            free(field_0_pDmap->field_40008_pColumn);
        }
        field_0_pDmap->field_40008_pColumn = 0;
        
        if (field_0_pDmap->field_4000C_block)
        {
            free(field_0_pDmap->field_4000C_block);
        }
        field_0_pDmap->field_4000C_block = 0;

        delete field_0_pDmap;
    }

    if (field_330_pZoneArray)
    {
        free(field_330_pZoneArray);
    }
    field_330_pZoneArray = 0;

    if (field_334_pUnknownZoneData)
    {
        free(field_334_pUnknownZoneData);
    }
    field_334_pUnknownZoneData = 0;

    if (field_340_pTileAnimData)
    {
        free(field_340_pTileAnimData);
    }
    field_340_pTileAnimData = 0;
}
