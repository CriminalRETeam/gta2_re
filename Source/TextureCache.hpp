#pragma once

#include "Function.hpp"
#include <windows.h>

struct STexture;

// One scrolling digit strip texture, see RollingDigitCounter
class DigitTexture
{
  public:
    // inlined 0x4C2F70
    DigitTexture()
    {
        mpPixelData = NULL;
        mpTexture = NULL;
        mHeight = 0;
        mRowOffset = 0;
    }

    u8* mpPixelData; // 0x0
    STexture* mpTexture; // 0x4
    u16 mHeight; // 0x8
    u16 mRowOffset; // 0xA: first pixel row of the digit strip that the texture shows
};
GTA2_ASSERT_SIZEOF_ALWAYS(DigitTexture, 0xC);

// A set of textures registered for one sprite type, one per sprite and remap palette
class RemapTextureSet
{
  public:
    STexture** mpTextures; // 0x0
    u16 mTextureCount; // 0x4
    u16 mRemapsPerSprite; // 0x6
    s32 mSpriteType; // 0x8
    s32 mPalType; // 0xC
    bool mbDoFree; // 0x10

    // inlined 0x4C2F10
    RemapTextureSet()
    {
        mpTextures = NULL;
        mTextureCount = 0;
        mRemapsPerSprite = 0;
        mSpriteType = 0;
        mPalType = 0;
        mbDoFree = false;
    }

    EXPORT ~RemapTextureSet();

    EXPORT void Alloc(s16 sprite_count, s16 remaps_per_sprite, s32 sprite_type, s32 pal_type);

    // 9.6f 0x4C2F30, defined in TextureCache.cpp
    inline void AllocOfType_4C2F30(s32 sprite_type, s32 pal_type);

    EXPORT void LoadRemappedTextures();

    EXPORT void LoadTextures();

    EXPORT STexture* get_texture(s32 sprite_type, s16 sprite_idx) const;

    EXPORT STexture* GetRemappedTexture(s16 sprite_idx, s16 remap) const;
};
GTA2_ASSERT_SIZEOF_ALWAYS(RemapTextureSet, 0x14);

struct TextureCache
{
    STexture* mTileTextures[1024]; // 0x0
    bool mbFreeCarDamageTextures; // 0x1000
    bool mbFreeTileTextures; // 0x1001
    STexture* mCarDamageTextures[48]; // 0x1004
    DigitTexture mDigitTextures[96]; // 0x10C4
    STexture* mpSharedTexture; // 0x1544: 128x128, re-pointed at different pixel data on demand
    RemapTextureSet mSpriteTextures; // 0x1548
    RemapTextureSet mCarRemapTextures; // 0x155C
    RemapTextureSet mPedRemapTextures; // 0x1570
    RemapTextureSet mCodeObjRemapTextures; // 0x1584
    RemapTextureSet mMapObjRemapTextures; // 0x1598
    RemapTextureSet mFontRemapTextures; // 0x15AC
    RemapTextureSet mUserRemapTextures; // 0x15C0
    s16 mNextDigitIdx; // 0x15D4
    u16 mPalCount; // 0x15D6

    inline STexture* GetTexture_46BB50(const u16& tile_idx) const
    {
        return mTileTextures[tile_idx];
    }

    EXPORT void LoadPals();
    EXPORT void FreePals();
    EXPORT void LoadCarDamageTextures();
    EXPORT s16 RegisterDigits(u16 num_of_digits, u16 palette);
    EXPORT void ReadTextures();
    EXPORT void LoadStyleTextures();
    EXPORT STexture* GetSpriteTexture(s32 sprite_type, u16 sprite_id, s32 palette_type, s16 remap) const;
    EXPORT STexture* GetCarDamageTexture(u16 texture_idx) const;
    EXPORT STexture* GetDigitTexture(u16 idx, u16 row_offset, u16 height);
    EXPORT void SetPal(u16 digit_idx, u16 pal_idx);
    EXPORT void SetCarDamageTextureSizeAndPalette(u16 texture_idx, u16 new_width, u16 new_height, u16 pal);
    EXPORT STexture* SetSharedTextureData(s16 width, s16 height, u8* pPixelData, u16 pal);
    EXPORT TextureCache();
    EXPORT ~TextureCache();
};
GTA2_ASSERT_SIZEOF_ALWAYS(TextureCache, 0x15D8);

EXTERN_GLOBAL(TextureCache*, gTextureCache_15D8_705064);
