#include "TextureCache.hpp"
#include "Function.hpp"
#include "Globals.hpp"
#include "crt_stubs.hpp"
#include "gbh_graphics.hpp"
#include "gtx_0x106C.hpp"
#include "sprite.hpp"
#include "memory.hpp"

DEFINE_GLOBAL(TextureCache*, gTextureCache_15D8, 0x705064);

MATCH_FUNC(0x5B8E90)
void RemapTextureSet::Alloc(s16 sprite_count, s16 remaps_per_sprite, s32 sprite_type, s32 pal_type)
{
    if (!mpTextures)
    {
        if (sprite_count)
        {
            mTextureCount = remaps_per_sprite * sprite_count;
            mRemapsPerSprite = remaps_per_sprite;
            mbDoFree = true;
            mSpriteType = sprite_type;
            mPalType = pal_type;
            mpTextures = static_cast<STexture**>(Memory::Checked_malloc(sizeof(STexture*) * mTextureCount));

            for (u32 i = 0; i < mTextureCount; i++)
            {
                mpTextures[i] = NULL;
            }
        }
    }
}

MATCH_FUNC(0x5B8F00)
void RemapTextureSet::LoadTextures()
{
    if (mbDoFree)
    {
        u32 i = 0;
        while (i < mTextureCount)
        {
            sprite_index* pSpriteIndex = gGtx_0x106C->get_sprite_index(i);
            u16 true_pal = gGtx_0x106C->GetTruePalette(palette_types_enum::sprites_2, i);
            u16 phys_pal = gGtx_0x106C->get_phys_pal(true_pal);

            mpTextures[i++] = pgbh_RegisterTexture(pSpriteIndex->mWidth,
                pSpriteIndex->mHeight,
                pSpriteIndex->mpData,
                phys_pal,
                0);
        }
    }
}

MATCH_FUNC(0x5B8F70)
void RemapTextureSet::LoadRemappedTextures()
{
    LOCAL_STATIC(u32, sRemappedSpriteCounter, 0x704ED0);
    LOCAL_STATIC(u32, sRemappedTextureCounter, 0x704F28);
    if (mbDoFree)
    {
        const u32 palTotal = mTextureCount / mRemapsPerSprite;
        for (u32 pal_idx = 0; pal_idx < palTotal; pal_idx++)
        {
            const u16 sprite_idx = gGtx_0x106C->GetSpriteTrueIndex(mSpriteType, pal_idx);
            sprite_index* pSpriteIndex = gGtx_0x106C->get_sprite_index(sprite_idx);

            sRemappedSpriteCounter++;

            for (u32 texture_idx = 0; texture_idx < mRemapsPerSprite; texture_idx++)
            {
                const s16 converted_pal_idx = gGtx_0x106C->GetTruePalette(mPalType, texture_idx);
                const u16 physPal = gGtx_0x106C->get_phys_pal(converted_pal_idx);
                mpTextures[texture_idx + (pal_idx * mRemapsPerSprite)] =
                    pgbh_RegisterTexture(pSpriteIndex->mWidth, pSpriteIndex->mHeight, pSpriteIndex->mpData, physPal, 1);

                sRemappedTextureCounter++;
            }
        }
    }
}

MATCH_FUNC(0x5B9050)
RemapTextureSet::~RemapTextureSet()
{
    if (mbDoFree && mpTextures)
    {
        for (u16 i = 0; i < mTextureCount; ++i)
        {
            pgbh_FreeTexture(mpTextures[i]);
        }

        crt::free(mpTextures);
        mpTextures = NULL;
    }
}

MATCH_FUNC(0x5B90A0)
STexture* RemapTextureSet::get_texture(s32 sprite_type, s16 sprite_idx) const
{
    return mpTextures[gGtx_0x106C->GetSpriteTrueIndex(sprite_type, sprite_idx)];
}

MATCH_FUNC(0x5B90D0)
STexture* RemapTextureSet::GetRemappedTexture(s16 sprite_idx, s16 remap) const
{
    return mpTextures[static_cast<u16>(remap + (sprite_idx * mRemapsPerSprite))];
}

MATCH_FUNC(0x5B90F0)
void TextureCache::LoadPals()
{
    mPalCount = gGtx_0x106C->get_physical_palettes_len();

    for (u16 palId = 0; palId < mPalCount; palId++)
    {
        pgbh_RegisterPalette(palId, reinterpret_cast<DWORD*>(gGtx_0x106C->GetPalData(palId)));
    }
}

MATCH_FUNC(0x5B9140)
void TextureCache::FreePals()
{
    for (u16 i = 0; i < mPalCount; ++i)
    {
        pgbh_FreePalette(i);
    }
    mPalCount = 0;
}

MATCH_FUNC(0x5B9180)
void TextureCache::LoadCarDamageTextures()
{
    u16 width_height = 64;
    if (gGtx_0x106C->GetSpriteBaseOfType(sprite_types_enum::car_2))
    {
        mbFreeCarDamageTextures = true;
        u16 sprite_idx = gGtx_0x106C->GetSpriteTrueIndex(sprite_types_enum::car_2, 0);
        u16 true_pal_idx = gGtx_0x106C->GetTruePalette(palette_types_enum::sprites_2, sprite_idx);
        u16 pal_idx = static_cast<u16>(gGtx_0x106C->get_phys_pal(true_pal_idx));

        for (u32 idx = 0; idx < 48; idx++)
        {
            if (idx == 32)
            {
                width_height = 128;
            }

            mCarDamageTextures[idx] = pgbh_RegisterTexture(width_height, 
                width_height, 
                gSpriteMaskCache->GetMaskPixels(idx), 
                pal_idx, 
                0);
        }
    }
}

MATCH_FUNC(0x5B9220)
s16 TextureCache::RegisterDigits(u16 num_of_digits, u16 palette)
{
    const u16 first_idx = mNextDigitIdx;
    const s16 sprite_idx = gGtx_0x106C->GetSpriteTrueIndex(sprite_types_enum::user_6, palette);
    sprite_index* pSpriteIndex = gGtx_0x106C->get_sprite_index(sprite_idx);
    u8* pPixelData = pSpriteIndex->mpData;
    const u16 phys_pal = gGtx_0x106C->get_phys_pal(gGtx_0x106C->GetTruePalette(palette_types_enum::sprites_2, sprite_idx));

    mNextDigitIdx += num_of_digits;

    for (s32 i = 0; i < num_of_digits; i++)
    {
        mDigitTextures[first_idx + i].mpTexture =
            pgbh_RegisterTexture(pSpriteIndex->mWidth, pSpriteIndex->mHeight, pPixelData, phys_pal, 0);
        mDigitTextures[first_idx + i].mpPixelData = pPixelData;
    }
    return first_idx;
}

MATCH_FUNC(0x5B92E0)
void TextureCache::ReadTextures()
{
    if (gGtx_0x106C->has_tiles())
    {
        mbFreeTileTextures = true;
        STexture** p = mTileTextures;
        for (u16 i = 0; i < GTA2_COUNTOF(mTileTextures); i++, p++)
        {
            // 64 256x256 pages of 64x64 8 bit tiles
            *p = pgbh_RegisterTexture(64, 64, gGtx_0x106C->get_tile(i), gGtx_0x106C->get_phys_pal(i), 0);
        }
    }
}

// 9.6f 0x4C2F30
inline void RemapTextureSet::AllocOfType(s32 sprite_type, s32 pal_type)
{
    Alloc(gGtx_0x106C->GetSpriteBaseOfType(sprite_type), gGtx_0x106C->GetPaletteBaseOfType(pal_type), sprite_type, pal_type);
}

MATCH_FUNC(0x5B9350)
void TextureCache::LoadStyleTextures()
{
    LoadPals();
    ReadTextures();

    mSpriteTextures.Alloc(gGtx_0x106C->GetPaletteBaseOfType(palette_types_enum::sprites_2), 1, sprite_types_enum::none_0, palette_types_enum::sprites_2);
    mSpriteTextures.LoadTextures();

    mCarRemapTextures.AllocOfType(sprite_types_enum::car_2, palette_types_enum::car_remaps_3);
    mCarRemapTextures.LoadRemappedTextures();

    mPedRemapTextures.AllocOfType(sprite_types_enum::ped_3, palette_types_enum::ped_remaps_4);
    mPedRemapTextures.LoadRemappedTextures();

    mCodeObjRemapTextures.AllocOfType(sprite_types_enum::code_obj1_4, palette_types_enum::code_obj_remaps_5);
    mCodeObjRemapTextures.LoadRemappedTextures();

    mMapObjRemapTextures.AllocOfType(sprite_types_enum::map_obj_5, palette_types_enum::map_obj_remaps_6);
    mMapObjRemapTextures.LoadRemappedTextures();

    mFontRemapTextures.AllocOfType(sprite_types_enum::font_7, palette_types_enum::font_remaps_8);
    mFontRemapTextures.LoadRemappedTextures();

    mUserRemapTextures.AllocOfType(sprite_types_enum::user_6, palette_types_enum::user_remaps_7);
    mUserRemapTextures.LoadRemappedTextures();

    LoadCarDamageTextures();

    mpSharedTexture = pgbh_RegisterTexture(128, 128, NULL, 0, 0);
}

MATCH_FUNC(0x5B94F0)
STexture* TextureCache::GetSpriteTexture(s32 sprite_type, u16 sprite_id, s32 palette_type, s16 remap) const
{
    STexture* result;

    switch (palette_type)
    {
        case palette_types_enum::sprites_2:
            result = mSpriteTextures.get_texture(sprite_type, sprite_id);
            break;
        case palette_types_enum::car_remaps_3:
            result = mCarRemapTextures.GetRemappedTexture(sprite_id, remap);
            break;
        case palette_types_enum::ped_remaps_4:
            result = mPedRemapTextures.GetRemappedTexture(sprite_id, remap);
            break;
        case palette_types_enum::code_obj_remaps_5:
            result = mCodeObjRemapTextures.GetRemappedTexture(sprite_id, remap);
            break;
        case palette_types_enum::map_obj_remaps_6:
            result = mMapObjRemapTextures.GetRemappedTexture(sprite_id, remap);
            break;
        case palette_types_enum::font_remaps_8:
            result = mFontRemapTextures.GetRemappedTexture(sprite_id, remap);
            break;
        case palette_types_enum::user_remaps_7:
            result = mUserRemapTextures.GetRemappedTexture(sprite_id, remap);
            break;
        default:
            result = NULL;
            break;
    }
    return result;
}

MATCH_FUNC(0x5B95D0)
STexture* TextureCache::GetCarDamageTexture(u16 texture_idx) const
{
    return mCarDamageTextures[texture_idx];
}

MATCH_FUNC(0x5B95F0)
STexture* TextureCache::GetDigitTexture(u16 idx, u16 row_offset, u16 height)
{
    DigitTexture* pDigit = &mDigitTextures[idx];
    STexture* pTexture = pDigit->mpTexture;

    if (height != pDigit->mHeight || row_offset != pDigit->mRowOffset)
    {
        STexture* pTextureInternal = pDigit->mpTexture;
        pDigit->mHeight = height;
        pDigit->mRowOffset = row_offset;
        pgbh_LockTexture(pTextureInternal);
        pTexture->field_14_original_pixel_data_ptr = &pDigit->mpPixelData[256 * row_offset];
        pTexture->field_10_height = height;
        pgbh_UnlockTexture(pTexture);
    }
    return pTexture;
}

MATCH_FUNC(0x5B9660)
void TextureCache::SetPal(u16 digit_idx, u16 pal_idx)
{
    STexture* pTexture = mDigitTextures[digit_idx].mpTexture;
    u16 pal = gGtx_0x106C->get_phys_pal(pal_idx);
    pgbh_LockTexture(pTexture);
    pgbh_AssignPalette(pTexture, pal);
    pgbh_UnlockTexture(pTexture);
}

MATCH_FUNC(0x5B96B0)
void TextureCache::SetCarDamageTextureSizeAndPalette(u16 texture_idx, u16 new_width, u16 new_height, u16 pal)
{
    u16 pal_idx = gGtx_0x106C->get_phys_pal(pal);
    STexture* pTexture = mCarDamageTextures[texture_idx];
    pgbh_LockTexture(pTexture);
    pTexture->field_E_width = new_width;
    pTexture->field_10_height = new_height;
    pgbh_AssignPalette(pTexture, pal_idx);
    pgbh_UnlockTexture(pTexture);
}

MATCH_FUNC(0x5B9710)
STexture* TextureCache::SetSharedTextureData(s16 width, s16 height, u8* pPixelData, u16 pal)
{
    u16 phys_pal = gGtx_0x106C->get_phys_pal(pal);
    pgbh_LockTexture(mpSharedTexture);
    mpSharedTexture->field_14_original_pixel_data_ptr = pPixelData;
    mpSharedTexture->field_E_width = width;
    mpSharedTexture->field_10_height = height;
    pgbh_AssignPalette(mpSharedTexture, phys_pal);
    pgbh_UnlockTexture(mpSharedTexture);
    return mpSharedTexture;
}

MATCH_FUNC(0x5B9790)
TextureCache::TextureCache()
{
    mbFreeCarDamageTextures = false;
    mbFreeTileTextures = false;
    mNextDigitIdx = 0;
    mPalCount = 0;
    s32 i;

    for (i = 0; i < GTA2_COUNTOF(mTileTextures); i++)
    {
        mTileTextures[i] = NULL;
    }

    for (i = 0; i < GTA2_COUNTOF(mCarDamageTextures); i++)
    {
        mCarDamageTextures[i] = NULL;
    }
    mpSharedTexture = NULL;
}

MATCH_FUNC(0x5B9900)
TextureCache::~TextureCache()
{
    if (mbFreeTileTextures)
    {
        for (s32 i = 0; i < GTA2_COUNTOF(mTileTextures); i++)
        {
            if (mTileTextures[i])
            {
                pgbh_FreeTexture(mTileTextures[i]);
                mTileTextures[i] = NULL;
            }
        }
    }

    if (mbFreeCarDamageTextures)
    {
        for (s32 i = 0; i < GTA2_COUNTOF(mCarDamageTextures); i++)
        {
            if (mCarDamageTextures[i])
            {
                pgbh_FreeTexture(mCarDamageTextures[i]);
                mCarDamageTextures[i] = NULL;
            }
        }
    }

    for (s32 i = 0; i < GTA2_COUNTOF(mDigitTextures); i++)
    {
        if (mDigitTextures[i].mpTexture)
        {
            pgbh_FreeTexture(mDigitTextures[i].mpTexture);
            mDigitTextures[i].mpTexture = NULL;
        }
    }

    if (mpSharedTexture)
    {
        pgbh_FreeTexture(mpSharedTexture);
        mpSharedTexture = NULL;
    }

    FreePals();
}