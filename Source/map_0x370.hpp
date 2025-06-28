#pragma once

#include "Function.hpp"
#include "fix16.hpp"
#include "ang16.hpp"
#include "Car_BC.hpp"
#include <windows.h>

class Gang_144;

struct gmp_col_info
{
    u8 field_0_height;
    u8 field_1_offset;
    s16 field_2_pad;
    s32 field_4_blockd[2];
};

struct gmp_block_info
{
    s16 field_0_left;
    s16 field_2_right;
    s16 field_4_top;
    s16 field_6_bottom;
    s16 field_8_lid;
    u8 field_A_arrows;
    u8 field_B_slope_type;
};

enum gmp_slope_type
{
    AIR = 0,
    ROAD = 1,
    PAVEMENT = 2,
    FIELD = 3,
};

struct gmp_header
{
    char_type field_0_file_type[4];
    s16 field_4_version;
};

struct Map_sub;

struct gmp_compressed_map_32
{
    EXPORT void sub_4E80A0(Map_sub* a2);

    u32 field_0_base[256][256];
    s32 field_40000_column_words;
    s32 field_40004_num_blocks;
    u16** field_40008_pColumn;
    gmp_block_info* field_4000C_block;

    gmp_block_info* get_block(u32 block)
    {
        return &field_4000C_block[block];
    }
};

struct gmp_dmap_info
{
    inline void set_data_463080(u8 x, u8 y, u32 column_idx)
    {
        field_4_x = x;
        field_5_y = y;
        field_0_column_idx = column_idx;
    }
    
    u32 field_0_column_idx;
    u8 field_4_x;
    u8 field_5_y;
    u16 field_6_unk;
};

struct Map_sub
{
    EXPORT void sub_4E80E0(u8 x, u8 y, u32 column_idx);

    gmp_dmap_info field_0[100];
    s32 field_320_max_idx;
};

class gmp_map_zone
{
  public:
    u8 field_0_zone_type;
    u8 field_1_x;
    u8 field_2_y;
    u8 field_3_w;
    u8 field_4_h;
    u8 field_5_name_length;
    char_type field_6_name[2];

    EXPORT s8 sub_4DEF40();
};

struct gmp_zone_unknown
{
    u16 field_0_density;
    s16 field_2;
    s16 field_4;
    s16 field_6;
    s16 field_8;
    s16 field_A;
    s16 field_C;
    s16 field_E;
    s16 field_10;
    s16 field_12;
    s16 field_14;
};

struct gmp_map_object
{
    u16 field_0_x;
    u16 field_2_y;
    Ang8 field_4_rotation;
    u8 field_5_object_type;
};

struct gmp_map_light
{
    s32 field_0_argb;
    s16 field_4_x;
    s16 field_6_y;
    s16 field_8_z;
    s16 field_A_radius;
    char_type field_C_intensity;
    char_type field_D_shape;
    char_type field_E_on_time;
    char_type field_F_off_time;
};

struct gmp_tile_animation
{
    u16 field_0_base; // The base tile of the animation. This marks the tile as an animating tile.
    u8 field_2_frame_rate; // The number of game frames that each frame of the animation is displayed for.
    u8 field_3_repeat; // The number of times the animation will be played. 0 means play forever.
    u8 field_4_anim_length; // The number of tiles in the animation
    u8 field_5_unused;
    // NOTE: Size set to 1 to shutup a warning
    u16 field_6_tiles[1]; // Size given by field_4_anim_length
};

struct gmp_map_slope
{
    u8 field_0_gradient_direction; // the direction the slope level increases. E.g. "direction north" means it increases the level from south to north
    u8 field_1_gradient_size; // 1 = 45 degree; 2 = 26 degree; 8 = 7 degree slope
    u8 field_2_gradient_level; // 0 = highest slope of the gradient, it increases as going lower
    u8 field_3_padding;
    Fix16 field_4_zpos_lower; // relative z coordinate from the lowest point of the slope
    Fix16 field_8_zpos_higher; // relative z coordinate from the highest point of the slope
};

enum gmp_gradient_slope_direction   // direction: low to high
{
    NORTH_1 = 1,
    SOUTH_2 = 2,
    WEST_3 = 3,
    EAST_4 = 4,
};

enum gmp_gradient_slope_size
{
    SIZE_1 = 1,
    SIZE_2 = 2,
    SIZE_8 = 8,
};

EXPORT_VAR extern gmp_map_slope byte_6F5BA8[64];
EXPORT_VAR extern Fix16 dword_6F5ED8;
EXPORT_VAR extern Fix16 dword_6F5FA8;
EXPORT_VAR extern Fix16 dword_6F6214;
EXPORT_VAR extern Fix16 dword_6F610C;

class Map_0x370
{
  public:
    EXPORT gmp_block_info* get_block_452980(u8 x_coord, u8 y_coord, u8 z_coord);

    EXPORT gmp_map_zone* zone_by_name_4DEFD0(const char_type* pZoneName);

    EXPORT s32 zone_idx_by_name_4DF050(const char_type* pZoneName, BYTE zone_name_len);

    EXPORT gmp_map_zone* zone_by_type_bounded_4DF0F0(u8 zone_type);

    EXPORT gmp_map_zone* first_zone_by_type_4DF1D0(u8 zone_type);

    EXPORT gmp_map_zone* sub_4DF240(u8 a2, u8 a3, char_type a4);
    EXPORT u8* sub_4DF3E0(u8 a2, u8 a3, char_type a4);

    EXPORT gmp_map_zone* zone_by_pos_and_type_4DF4D0(u8 zone_x, u8 zone_y, u8 zone_type);
    EXPORT gmp_map_zone* nav_zone_by_pos_4DF5C0(u8 zone_x, u8 zone_y);

    EXPORT gmp_map_zone* sub_4DF6A0(u8 a2, u8 a3);

    EXPORT gmp_map_zone* next_zone_4DF770();

    EXPORT gmp_zone_unknown* cur_zone_4DF840();

    EXPORT gmp_zone_unknown* get_nav_zone_unknown_4DF890(s32 zone_x, s32 zone_y);

    EXPORT void set_nav_unknown_data_4DF8C0(s32 zone_idx,
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
                                            s16 a13);

    EXPORT gmp_zone_unknown* get_zone_unknown_4DF9A0(u16 zone_idx);

    EXPORT void set_nav_unknown_f0_4DF9D0(u16 a2, s16 a3);

    EXPORT void set_nav_unknown_f2_4DF9F0(u16 a2, s16 a3);

    EXPORT void set_nav_unknown_f4_4DFA10(u16 a2, s16 a3);

    EXPORT void set_nav_unknown_f6_4DFA30(u16 a2, s16 a3);

    EXPORT void set_nav_unknown_f8_4DFA50(u16 a2, s16 a3);

    EXPORT void set_nav_unknown_fA_4DFA70(u16 a2, s16 a3);

    EXPORT void set_nav_unknown_fC_4DFA90(u16 a2, s16 a3);

    EXPORT void set_nav_unknown_fE_4DFAB0(u16 a2, s16 a3);

    EXPORT void set_nav_unknown_f10_4DFAD0(u16 a2, s16 a3);

    EXPORT void set_nav_unknown_f12_4DFAF0(u16 a2, s16 a3);

    EXPORT void set_nav_unknown_f14_4DFB10(u16 a2, s16 a3);

    EXPORT gmp_map_zone* get_zone_4DFB30(u16 zone_idx);

    EXPORT Gang_144* sub_4DFB50(Fix16 x, Fix16 y);

    EXPORT void sub_4DFB90();
    EXPORT void alloc_zones_4DFCA0();
    EXPORT void update_lights_4DFCD0();

    EXPORT gmp_block_info* get_block_4DFE10(s32 x_coord, s32 y_coord, s32 z_coord);

    EXPORT gmp_block_info* sub_4DFE60(s32 a2, s32 a3, s32 a4);
    EXPORT gmp_block_info* sub_4DFEE0(s32 x_coord, s32 y_coord, s32 z_coord);

    EXPORT DWORD sub_4DFF60(Fix16 x_coord, Fix16 y_coord, Fix16 z_coord);

    EXPORT s32 sub_4E0000(s32 a2, s32 a3, s32 a4);
    EXPORT s32 sub_4E00A0(Fix16 x, Fix16 y, Fix16 z);
    EXPORT char_type sub_4E0110();
    EXPORT char_type sub_4E0120();
    EXPORT char_type sub_4E0130(s32 a2, s32 a3, s32 a4, s32 a5, u8* a6, char_type a7);
    EXPORT char_type sub_4E11E0(s32* a2);
    EXPORT char_type sub_4E1520(s32 a2);
    EXPORT char_type sub_4E18A0(s32 a2, s32 a3, s32 a4, s32 a5, s32 a6);
    EXPORT char_type sub_4E1A30(s32 a2, s32 a3, s32 a4, s32 a5, s32 a6);
    EXPORT char_type sub_4E1E00(s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8);
    EXPORT char_type sub_4E4460(s32 a2, s32 a3, s32 a4, Sprite* a5, s16 a6);
    EXPORT char_type sub_4E4630(Fix16 a2);
    EXPORT char_type sub_4E4770(s32 a2);
    EXPORT char_type sub_4E4820(u32* a2, char_type a3);
    EXPORT char_type sub_4E4930(u8* a1, u8* a2, u8* a3, char_type a4);
    EXPORT char_type sub_4E4AC0(char_type a1);
    EXPORT char_type sub_4E4B40(s32 a1, gmp_block_info* a2);
    EXPORT gmp_block_info* sub_4E4BB0(s32 a2, s32 a3, u32& a4);
    EXPORT gmp_block_info* FindHighestBlockForCoord_4E4C30(s32 a2, s32 a3, u32* a4);
    EXPORT gmp_block_info* sub_4E4CB0(s32 a2, s32 a3, s32& a4);
    EXPORT Fix16* sub_4E4D40(Fix16* a2, Fix16 a3, Fix16 a4, Fix16 a5);
    EXPORT Fix16* sub_4E4E50(Fix16* a2, Fix16 a3, Fix16 a4, Fix16 a5);
    EXPORT s32* sub_4E4F40(s32* a2, s32 a3, s32 a4, s32 a5);
    EXPORT s32* sub_4E5050(s32* a2, s32 a3, s32 a4, s32 a5, u8* a6);
    EXPORT char_type sub_4E5170(s32 a2, s32 a3, s32 a4);
    EXPORT char_type sub_4E52A0(s32 a2, s32 a3, s32 a4);
    EXPORT char_type sub_4E5300(s32 a2, s32 a3, s32 a4, s32 a5);
    EXPORT char_type sub_4E5480(s32 a2, s32 a3, s32 a4, s32 a5, s32* a6);
    EXPORT char_type sub_4E5640(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8, s32 a9);
    EXPORT Fix16* FindGroundZForCoord_4E5B60(Fix16* a2, Fix16 a3, Fix16 a4);
    EXPORT u8 UpdateZFromSlopeAtCoord_4E5BF0(Fix16 a2, Fix16 a3, Fix16& a4);
    EXPORT s16 sub_4E6190(s32 x, s32 y, s32 z, s32 a5, char_type a6);
    EXPORT gmp_block_info* sub_4E62D0(s32 a2, s32 a3, u32* a4);
    EXPORT gmp_block_info* sub_4E6360(s32 a2, s32 a3, s32* a4);
    EXPORT Fix16* sub_4E6400(Fix16* a2, Fix16 a3, Fix16 a4, Fix16 a5);
    EXPORT Fix16* sub_4E6510(Fix16* a2, Fix16 a3, Fix16 a4);
    EXPORT s16 sub_4E65A0(s32 a2, s32 a3, s32* a4, char_type a5, char_type a6);
    EXPORT s32 sub_4E6660(s32* a2, s32* a3, s32* a4, s32 a5);
    EXPORT s32 sub_4E7190(s32* a2, s32* a3, s32* a4, s32 a5);
    EXPORT char_type sub_4E7E90(u8* a2, char_type* a3);
    EXPORT char_type sub_4E7FC0(s32 a2, s32 a3, s32 a4);
    EXPORT s32 sub_4E8140(gmp_block_info *pBlockInfo);
    EXPORT s32 sub_4E8180(u32 a2);
    EXPORT s32 sub_4E81D0(u32 a2);
    EXPORT s32 sub_4E8220(u32 a2, s32 a3);
    EXPORT u32 sub_4E8370(u32 a2, s32 a3, char_type a4);
    EXPORT void sub_4E8620(s32 a2, s32 a3, s32 a4, s32 info_type_to_set, u16 info_value);
    EXPORT void sub_4E87C0(s32 a2, s32 a3, s32 a4, u32* pBlockData);
    EXPORT void sub_4E8940(s32 a2, s32 a3, s32 a4, char_type a5);
    EXPORT void sub_4E8A10(s32 a2, s32 a3);
    EXPORT void sub_4E8B70(s32 a2, s32 a3, s32 a4, s32 a5);
    EXPORT void sub_4E8C00(u32 a2, u32 a3, u32 a4);
    EXPORT s32 sub_4E8CF0(u32* a2, u32* a3, u32* a4, u32* a5, Map_sub** a6, s32* a7);

    EXPORT void do_process_loaded_zone_data_4E8E30();

    EXPORT void sub_4E90E0(u32 a2);

    EXPORT void sub_4E9160(s32 size);

    EXPORT void load_mobj_4E91A0(size_t len);

    EXPORT void load_lght_4E9200(size_t a2);

    EXPORT void load_zone_4E9250(size_t a2);

    EXPORT void load_anim_4E9280(size_t size);

    EXPORT void load_dmap_4E92B0(s32 len);

    EXPORT void load_rgen_4E94A0();

    EXPORT void load_chunk_4E94B0(const char_type* pChunkType, size_t chunkLen);

    EXPORT void process_loaded_zone_data_4E95A0();

    EXPORT void LoadMap_4E95B0(const char_type* pGmpFileName);

    EXPORT Map_0x370();

    EXPORT ~Map_0x370();

    // 9.6f inline 0x4634E0
    inline Fix16 get_grad_scale_from_size_4634E0(Fix16& scale, u8 gradient_size)
    {
        switch (gradient_size)
        {
            case SIZE_1:
                scale = dword_6F5ED8;    //  16384
                break;
            case SIZE_2:
                scale = dword_6F5FA8;    //  8168
                break;
            case SIZE_8:
                scale = dword_6F6214;    //  2084
                break;
            default:
                scale = dword_6F610C;    //  0
                break;
        }
        return scale;
    }

  public:
    gmp_compressed_map_32* field_0_pDmap;
    Map_sub field_4_obj;
    gmp_map_zone* field_328_pZoneData;
    gmp_map_zone** field_32C_pZones;
    BYTE* field_330_pZoneArray;
    gmp_zone_unknown* field_334_pUnknownZoneData;
    gmp_map_object* field_338_pMapObjects;
    gmp_map_light* field_33C_pLightData;
    gmp_tile_animation* field_340_pTileAnimData;
    s32 field_344_map_object_count;
    s32 field_348_num_lights;
    s32 field_34C_num_blocks;
    s32 field_350_num_blocks_extra;
    s32 field_354_num_blocks;
    s32 field_358_column_words;
    s32 field_35C_column_word_extra;
    s32 field_360_column_words;
    u16 field_364_cur_zone_idx;
    s16 field_366;
    u8 field_368_zone_type;
    u8 field_369;
    u8 field_36A_zone_x;
    u8 field_36B_zone_y;
    char_type field_36C_bUnknown;
    char_type field_36D;
    char_type field_36E;
    char_type field_36F;
};

EXPORT_VAR extern Map_0x370* gMap_0x370_6F6268;

EXPORT_VAR extern s32 gPurple_left_6F5FD4;
EXPORT_VAR extern s32 gPurple_right_6F5B80;
