#pragma once

#include "Function.hpp"
#include <windows.h>

struct STexture;

// One scrolling digit strip texture, see RollingDigitCounter_38
class DigitTexture_C
{
  public:
    // inlined 0x4C2F70
    DigitTexture_C()
    {
        field_0_pPixelData = 0;
        field_4_pTexture = 0;
        field_8_height = 0;
        field_A_row_offset = 0;
    }

    u8* field_0_pPixelData;
    STexture* field_4_pTexture;
    u16 field_8_height;
    u16 field_A_row_offset; // first pixel row of the digit strip that the texture shows
};

// A set of textures registered for one sprite type, one per sprite and remap palette
class RemapTextureSet_14
{
  public:
    STexture** field_0_pTextures;
    u16 field_4_texture_count;
    u16 field_6_remaps_per_sprite;
    s32 field_8_sprite_type;
    s32 field_C_pal_type;
    char_type field_10_bDoFree;

    // inlined 0x4C2F10
    RemapTextureSet_14()
    {
        field_0_pTextures = 0;
        field_4_texture_count = 0;
        field_6_remaps_per_sprite = 0;
        field_8_sprite_type = 0;
        field_C_pal_type = 0;
        field_10_bDoFree = 0;
    }

    EXPORT ~RemapTextureSet_14();

    EXPORT void Alloc_5B8E90(s16 sprite_count, s16 remaps_per_sprite, s32 sprite_type, s32 pal_type);

    EXPORT void LoadRemappedTextures_5B8F70();

    EXPORT void LoadTextures_5B8F00();

    EXPORT STexture* get_texture_5B90A0(s32 sprite_type, s16 sprite_idx);

    EXPORT STexture* GetRemappedTexture_5B90D0(s16 sprite_idx, s16 remap);
};

struct TextureCache_15D8
{
    STexture* field_0_tile_textures[1024];
    char_type field_1000_bFreeCarDamageTextures;
    char_type field_1001_bFreeTileTextures;
    STexture* field_1004_car_damage_textures[48];
    DigitTexture_C field_10C4_digit_textures[96];
    STexture* field_1544_pShared_texture; // 128x128, re-pointed at different pixel data on demand
    RemapTextureSet_14 field_1548_sprite_textures;
    RemapTextureSet_14 field_155C_car_remap_textures;
    RemapTextureSet_14 field_1570_ped_remap_textures;
    RemapTextureSet_14 field_1584_code_obj_remap_textures;
    RemapTextureSet_14 field_1598_map_obj_remap_textures;
    RemapTextureSet_14 field_15AC_font_remap_textures;
    RemapTextureSet_14 field_15C0_user_remap_textures;
    s16 field_15D4_next_digit_idx;
    u16 field_15D6_pal_count;

    inline STexture* GetTexture_46BB50(u16& tile_idx)
    {
        return field_0_tile_textures[tile_idx];
    }

    EXPORT void LoadPals_5B90F0();
    EXPORT void FreePals_5B9140();
    EXPORT void LoadCarDamageTextures_5B9180();
    EXPORT s16 RegisterDigits_5B9220(u16 num_of_digits, u16 palette);
    EXPORT void ReadTextures_5B92E0();
    EXPORT void LoadStyleTextures_5B9350();
    EXPORT STexture* GetSpriteTexture_5B94F0(s32 sprite_type, u16 sprite_id, s32 palette_type, s16 remap);
    EXPORT STexture* GetCarDamageTexture_5B95D0(u16 texture_idx);
    EXPORT STexture* GetDigitTexture_5B95F0(u16 idx, u16 row_offset, u16 height);
    EXPORT void SetPal_5B9660(u16 digit_idx, u16 pal_idx);
    EXPORT void SetCarDamageTextureSizeAndPalette_5B96B0(u16 texture_idx, u16 new_width, u16 new_height, u16 pal);
    EXPORT STexture* SetSharedTextureData_5B9710(s16 width, s16 height, u8* pPixelData, u16 pal);
    EXPORT TextureCache_15D8();
    EXPORT ~TextureCache_15D8();
};

EXTERN_GLOBAL(TextureCache_15D8*, gTextureCache_15D8_705064);
