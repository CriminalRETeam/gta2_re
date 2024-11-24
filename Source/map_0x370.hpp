#pragma once

#include "Function.hpp"
#include "fix16.hpp"
#include <windows.h>

class Zone_144;

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
    char_type field_A_arrows;
    u8 field_B_slope_type;
};

struct gmp_header
{
    char_type field_0_file_type[4];
    s16 field_4_version;
};

struct gmp_compressed_map_32
{
    u32 field_0_base[256][256];
    s32 field_40000_column_words;
    s32 field_40004_num_blocks;
    u16** field_40008_pColumn;
    gmp_block_info* field_4000C_block;
};

struct Map_sub
{
    s32 field_0;
    s32 field_4;
    s32 field_8;
    s32 field_C;
    s32 field_10;
    s32 field_14;
    s32 field_18;
    s32 field_1C;
    s32 field_20;
    s32 field_24;
    s32 field_28;
    s32 field_2C;
    s32 field_30;
    s32 field_34;
    s32 field_38;
    s32 field_3C;
    s32 field_40;
    s32 field_44;
    s32 field_48;
    s32 field_4C;
    s32 field_50;
    s32 field_54;
    s32 field_58;
    s32 field_5C;
    s32 field_60;
    s32 field_64;
    s32 field_68;
    s32 field_6C;
    s32 field_70;
    s32 field_74;
    s32 field_78;
    s32 field_7C;
    s32 field_80;
    s32 field_84;
    s32 field_88;
    s32 field_8C;
    s32 field_90;
    s32 field_94;
    s32 field_98;
    s32 field_9C;
    s32 field_A0;
    s32 field_A4;
    s32 field_A8;
    s32 field_AC;
    s32 field_B0;
    s32 field_B4;
    s32 field_B8;
    s32 field_BC;
    s32 field_C0;
    s32 field_C4;
    s32 field_C8;
    s32 field_CC;
    s32 field_D0;
    s32 field_D4;
    s32 field_D8;
    s32 field_DC;
    s32 field_E0;
    s32 field_E4;
    s32 field_E8;
    s32 field_EC;
    s32 field_F0;
    s32 field_F4;
    s32 field_F8;
    s32 field_FC;
    s32 field_100;
    s32 field_104;
    s32 field_108;
    s32 field_10C;
    s32 field_110;
    s32 field_114;
    s32 field_118;
    s32 field_11C;
    s32 field_120;
    s32 field_124;
    s32 field_128;
    s32 field_12C;
    s32 field_130;
    s32 field_134;
    s32 field_138;
    s32 field_13C;
    s32 field_140;
    s32 field_144;
    s32 field_148;
    s32 field_14C;
    s32 field_150;
    s32 field_154;
    s32 field_158;
    s32 field_15C;
    s32 field_160;
    s32 field_164;
    s32 field_168;
    s32 field_16C;
    s32 field_170;
    s32 field_174;
    s32 field_178;
    s32 field_17C;
    s32 field_180;
    s32 field_184;
    s32 field_188;
    s32 field_18C;
    s32 field_190;
    s32 field_194;
    s32 field_198;
    s32 field_19C;
    s32 field_1A0;
    s32 field_1A4;
    s32 field_1A8;
    s32 field_1AC;
    s32 field_1B0;
    s32 field_1B4;
    s32 field_1B8;
    s32 field_1BC;
    s32 field_1C0;
    s32 field_1C4;
    s32 field_1C8;
    s32 field_1CC;
    s32 field_1D0;
    s32 field_1D4;
    s32 field_1D8;
    s32 field_1DC;
    s32 field_1E0;
    s32 field_1E4;
    s32 field_1E8;
    s32 field_1EC;
    s32 field_1F0;
    s32 field_1F4;
    s32 field_1F8;
    s32 field_1FC;
    s32 field_200;
    s32 field_204;
    s32 field_208;
    s32 field_20C;
    s32 field_210;
    s32 field_214;
    s32 field_218;
    s32 field_21C;
    s32 field_220;
    s32 field_224;
    s32 field_228;
    s32 field_22C;
    s32 field_230;
    s32 field_234;
    s32 field_238;
    s32 field_23C;
    s32 field_240;
    s32 field_244;
    s32 field_248;
    s32 field_24C;
    s32 field_250;
    s32 field_254;
    s32 field_258;
    s32 field_25C;
    s32 field_260;
    s32 field_264;
    s32 field_268;
    s32 field_26C;
    s32 field_270;
    s32 field_274;
    s32 field_278;
    s32 field_27C;
    s32 field_280;
    s32 field_284;
    s32 field_288;
    s32 field_28C;
    s32 field_290;
    s32 field_294;
    s32 field_298;
    s32 field_29C;
    s32 field_2A0;
    s32 field_2A4;
    s32 field_2A8;
    s32 field_2AC;
    s32 field_2B0;
    s32 field_2B4;
    s32 field_2B8;
    s32 field_2BC;
    s32 field_2C0;
    s32 field_2C4;
    s32 field_2C8;
    s32 field_2CC;
    s32 field_2D0;
    s32 field_2D4;
    s32 field_2D8;
    s32 field_2DC;
    s32 field_2E0;
    s32 field_2E4;
    s32 field_2E8;
    s32 field_2EC;
    s32 field_2F0;
    s32 field_2F4;
    s32 field_2F8;
    s32 field_2FC;
    s32 field_300;
    s32 field_304;
    s32 field_308;
    s32 field_30C;
    s32 field_310;
    s32 field_314;
    s32 field_318;
    s32 field_31C;
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
};

struct gmp_zone_unknown
{
    s16 field_0_density;
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
    s16 field_0_x;
    s16 field_2_y;
    char_type field_4_rotation;
    char_type field_5_object_type;
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

    EXPORT Zone_144* sub_4DFB50(Fix16 x, Fix16 y);

    EXPORT void sub_4DFB90();
    EXPORT void alloc_zones_4DFCA0();
    EXPORT void update_lights_4DFCD0();

    EXPORT gmp_block_info* get_block_4DFE10(s32 x_coord, s32 y_coord, s32 z_coord);

    EXPORT gmp_block_info* sub_4DFE60(s32 a2, s32 a3, s32 a4);
    EXPORT gmp_block_info* sub_4DFEE0(s32 x_coord, s32 y_coord, s32 z_coord);

    EXPORT DWORD sub_4DFF60(Fix16 x_coord, Fix16 y_coord, Fix16 z_coord);

    EXPORT s32 sub_4E0000(s32 a2, s32 a3, s32 a4);
    EXPORT s32 sub_4E00A0(s32 x, s32 y, s32 z);
    EXPORT char_type sub_4E0110();
    EXPORT char_type sub_4E0120();
    EXPORT char_type sub_4E0130(s32 a2, s32 a3, s32 a4, s32 a5, u8* a6, char_type a7);
    EXPORT char_type sub_4E11E0(s32* a2);
    EXPORT char_type sub_4E1520(s32 a2);
    EXPORT char_type sub_4E18A0(s32 a2, s32 a3, s32 a4, s32 a5, s32 a6);
    EXPORT char_type sub_4E1A30(s32 a2, s32 a3, s32 a4, s32 a5, s32 a6);
    EXPORT char_type sub_4E1E00(s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8);
    EXPORT char_type sub_4E4460(s32 a2, s32 a3, s32 a4, s32 a5, s16 a6);
    EXPORT char_type sub_4E4630(s32 a2);
    EXPORT char_type sub_4E4770(s32 a2);
    EXPORT char_type sub_4E4820(u32* a2, char_type a3);
    EXPORT char_type sub_4E4930(u8* a1, u8* a2, u8* a3, char_type a4);
    EXPORT char_type sub_4E4AC0(char_type a1);
    EXPORT char_type sub_4E4B40(s32 a1, gmp_block_info* a2);
    EXPORT gmp_block_info* sub_4E4BB0(s32 a2, s32 a3, u32* a4);
    EXPORT gmp_block_info* sub_4E4C30(s32 a2, s32 a3, u32* a4);
    EXPORT gmp_block_info* sub_4E4CB0(s32 a2, s32 a3, s32* a4);
    EXPORT s32* sub_4E4D40(s32* a2, s32 a3, s32 a4, s32 a5);
    EXPORT s32* sub_4E4E50(s32* a2, s32 a3, s32 a4, s32 a5);
    EXPORT s32* sub_4E4F40(s32* a2, s32 a3, s32 a4, s32 a5);
    EXPORT s32* sub_4E5050(s32* a2, s32 a3, s32 a4, s32 a5, u8* a6);
    EXPORT char_type sub_4E5170(s32 a2, s32 a3, s32 a4);
    EXPORT char_type sub_4E52A0(s32 a2, s32 a3, s32 a4);
    EXPORT char_type sub_4E5300(s32 a2, s32 a3, s32 a4, s32 a5);
    EXPORT char_type sub_4E5480(s32 a2, s32 a3, s32 a4, s32 a5, s32* a6);
    EXPORT char_type sub_4E5640(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8, s32 a9);
    EXPORT s32* sub_4E5B60(s32* a2, s32 a3, s32 a4);
    EXPORT char_type sub_4E5BF0(s32 a2, s32 a3, s32* a4);
    EXPORT s16 sub_4E6190(s32 x, s32 y, s32 z, s32 a5, char_type a6);
    EXPORT gmp_block_info* sub_4E62D0(s32 a2, s32 a3, u32* a4);
    EXPORT gmp_block_info* sub_4E6360(s32 a2, s32 a3, s32* a4);
    EXPORT s32* sub_4E6400(s32* a2, s32 a3, s32 a4, s32 a5);
    EXPORT s32* sub_4E6510(s32* a2, s32 a3, s32 a4);
    EXPORT s16 sub_4E65A0(s32 a2, s32 a3, s32* a4, char_type a5, char_type a6);
    EXPORT s32 sub_4E6660(s32* a2, s32* a3, s32* a4, s32 a5);
    EXPORT s32 sub_4E7190(s32* a2, s32* a3, s32* a4, s32 a5);
    EXPORT char_type sub_4E7E90(u8* a2, char_type* a3);
    EXPORT char_type sub_4E7FC0(s32 a2, s32 a3, s32 a4);
    EXPORT s32 sub_4E8140(u32* a2);
    EXPORT s32 sub_4E8180(u32 a2);
    EXPORT s32 sub_4E81D0(u32 a2);
    EXPORT s32 sub_4E8220(u32 a2, s32 a3);
    EXPORT u32 sub_4E8370(u32 a2, s32 a3, char_type a4);
    EXPORT void sub_4E8620(s32 a2, s32 a3, s32 a4, s32 info_type_to_set, s16 info_value);
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

  public:
    gmp_compressed_map_32* field_0_pDmap;
    Map_sub field_4_obj;
    gmp_map_zone* field_328_pZoneData;
    gmp_map_zone** field_32C_pZones;
    BYTE* field_330_pZoneArray;
    gmp_zone_unknown* field_334_pUnknownZoneData;
    gmp_map_object* field_338_pMapObjects;
    gmp_map_light* field_33C_pLightData;
    void* field_340_pTileAnimData;
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

extern Map_0x370* gMap_0x370_6F6268;
