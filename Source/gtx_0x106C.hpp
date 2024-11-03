#pragma once

#include <windows.h>
#include "types.hpp"


struct palette_base
{
    u16 field_0_tile;
    u16 field_2_sprite;
    u16 field_4_car_remap;
    u16 field_6_red_remap;
    u16 field_8_code_obj_remap;
    u16 field_A_map_obj_remap;
    u16 field_C_user_remap;
    u16 field_E_font_remap;
};

struct sprite_base
{
    u16 field_0_car;
    u16 field_2_ped;
    u16 field_4_code_obj;
    u16 field_6_map_obj;
    u16 field_8_user;
    u16 field_A_font;
};

struct font_base
{
    s16 field_0_font_count;
    u16 field_2_base[1]; // variable � see font_count
};

struct sprite_index
{
    BYTE* field_0_pData;
    u8 field_4_width;
    u8 field_5_height;
    s16 field_6_pad;
};

struct object_info
{
    u8 field_0_model;
    u8 field_1_sprites;
};

struct palette_index
{
    u16 field_0_phys_palette[16384];
};

struct tile_array
{
    u16 field_0[1024];
};

struct delta_entry
{
    s16 field_0_which_sprite;
    char_type field_2_delta_count;
    char_type field_3_pad;
    s16 field_4_delta[2];
};

struct door_info
{
    char_type rx, ry;
};

struct car_info
{
    BYTE model;
    BYTE sprite;
    BYTE w;
    BYTE h;
    BYTE num_remaps;
    BYTE passengers;
    BYTE wreck;
    BYTE rating;
    char_type front_wheel_offset;
    char_type rear_wheel_offset;
    char_type front_window_offset;
    char_type rear_window_offset;
    BYTE info_flags;
    BYTE info_flags_2;
    BYTE remap[1]; // [variable � see num_remaps];
    BYTE num_doors;
    door_info doors[1]; // [variable � see num_doors];
};

struct car_info_container
{
    car_info_container()
    {
        field_400_count = 0;
        memset(field_0, 0, sizeof(field_0));
    }
    car_info *field_0[256];
    char_type field_400_count;
    //char_type field_401;// pad
    //char_type field_402;// pad
    //char_type field_403;// pad
};

struct delta_store_entry
{
    s16 field_0_offset;
    char_type field_2_len;
    char_type field_3_data[1]; // variable � see field_2_len
};


class gtx_0x106C
{
public:
    EXPORT car_info* get_car_info_5AA3B0(u8 idx);

    EXPORT BYTE* get_car_remap_5AA3D0(u8 idx);

    EXPORT sprite_index* get_sprite_index_5AA440(u16 idx);

    EXPORT u16 convert_sprite_pal_5AA460(s32 type, s16 sprite_pal);

    EXPORT s16 sub_5AA4F0(s32 a2);

    EXPORT s16 sub_5AA560(s32 a2);

    EXPORT s16 convert_pal_type_5AA5F0(s32 type, s16 pal);

    EXPORT BYTE* GetPalData_5AA6A0(s16 a2);

    EXPORT u16 get_phys_pal_5AA6F0(u16 palId);

    EXPORT u16 sub_5AA710(u16 a2, s16 a3);

    EXPORT u16 sub_5AA760(u16 *a2, wchar_t *a3);

    EXPORT u16 space_width_5AA7B0(u16 *a2);

    EXPORT s16 sub_5AA800(u16 *a2);

    EXPORT bool sub_5AA850(u16 tile_idx);

    EXPORT s16 sub_5AA870(u16 tile_idx);

    EXPORT s16 sub_5AA890();

    EXPORT s16 get_physical_palettes_len_5AA900();

    EXPORT object_info* get_map_object_info_5AA910(u16 idx);

    EXPORT s32 sub_5AA930(u16 tile_idx, s16 tile_val);

    EXPORT void create_tile_num_array_5AA950();

    EXPORT void sub_5AA9A0(s32 a2);

    EXPORT void sub_5AAB30(u32 delx_chunk_size);

    EXPORT void sub_5AABF0();

    EXPORT void SetSpriteIndexDataPtrs_5AAC40();

    EXPORT void sub_5AAC70();

    EXPORT void load_car_info_5AAD50(u32 cari_chunk_size);

    EXPORT void load_delta_index_5AAD80(u32 delx_chunk_size);

    EXPORT void load_delta_store_5AADD0(u32 dels_chunk_size);

    EXPORT void load_tiles_5AADF0(u32 tile_chunk_len);

    EXPORT void skip_ovly_5AAE20(u32 a1);

    EXPORT void skip_psxt_5AAE30(u32 a1);

    EXPORT void load_sprite_graphics_5AAE40(u32 sprg_chunk_len);

    EXPORT void load_physical_palettes_5AAE70(u32 ppal_chunk_size);

    EXPORT void load_palette_index_5AAEA0(u32 palx_chunk_len);

    EXPORT void load_map_object_info_5AAF00(u32 obji_chunk_len);

    EXPORT void load_sprite_index_5AAF80(u32 sprx_chunk_size);

    EXPORT void sub_5AAFE0(u16 a1);

    EXPORT void load_font_base_5AB0F0(u32 fonb_chunk_size);

    EXPORT static u16 __stdcall ConvertToVirtualOffsets_5AB1A0(u16 *pOffsets, u32 offsetsCount);

    EXPORT static void __stdcall ConvertToVirtualOffsets_5AB1C0(u16 *pBuffer, u32 len);

    EXPORT void load_sprite_base_5AB210(u32 sprite_base_chunk_size);

    EXPORT void load_palete_base_5AB2C0(u32 palette_base_chunk_len);

    EXPORT bool sub_5AB380(u8 car_id);

    EXPORT void load_car_recycling_info_5AB3C0(u32 recy_chunk_size);

    EXPORT void read_spec_5AB3F0(u32 read_size2);

    EXPORT void load_spec_5AB450();

    EXPORT void LoadChunk_5AB4B0(const char_type *Str1, u32 chunk_len);

    EXPORT void sub_5AB720();

    EXPORT void LoadSty_5AB750(const char_type *pStyFileName);

    // 0x5AB820
    EXPORT gtx_0x106C();

    // 0x5AB8A0
    EXPORT ~gtx_0x106C();

    s16 field_0_totalPalBase;
    s16 field_2_font_base_total;
    s16 field_4_sprite_index_count;
    u16 field_6_map_object_info_len;
    s16 field_8_physical_palettes_len;
    s16 field_A;
    palette_base *field_C_palette_base2;
    palette_base *field_10_palette_base1;
    sprite_base *field_14_sprite_base2;
    sprite_base *field_18_sprite_base1;
    font_base *field_1C_font_base;
    sprite_index *field_20_sprite_index;
    object_info *field_24_map_object_info;
    palette_index *field_28_palette_index;
    void* field_2C_physical_palettes;
    void* field_30_physical_palettes_size;
    BYTE *field_34_sprite_graphics;
    void* field_38;
    void *field_3C_tiles;
    tile_array *field_40_tile;
    void* field_44_aligned_tiles_size;
    delta_store_entry* field_48_delta_store;
    delta_entry *field_4C_delta_index;
    void *field_50_delta_buffer;
    s32 field_54_del;
    car_info** field_58_car_info;
    car_info_container* field_5C_cari;
    s32 field_60_delta_len;
    u8 *field_64_car_recycling_info;
    s16 field_68_recy_chunk_size;
    char_type field_6A;
    char_type field_6B;
    s32 field_6C_spec[1024];
};

extern gtx_0x106C *gGtx_0x106C_703DD4;
