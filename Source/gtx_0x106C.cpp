#include "gtx_0x106C.hpp"
#include "Frontend.hpp" // for the gFrontend*Font globals
#include "Function.hpp"
#include "Globals.hpp"
#include "chunk.hpp"
#include "crt_stubs.hpp"
#include "error.hpp"
#include "file.hpp"
#include "memory.hpp"
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "enums.hpp"

// Forward declarations: the functions below are in address order
void __stdcall ClearPixelsOfColourInRow(u8* pData, s32 width, u8 clear_target);

DEFINE_GLOBAL(gtx_0x106C*, gGtx_0x106C, 0x703DD4);
DEFINE_GLOBAL(s16, gStyleMessageFont, 0x703D9C);
DEFINE_GLOBAL(s16, gKanjiLargeFont, 0x703D9A);
DEFINE_GLOBAL(s16, gKanjiSmallFont, 0x703C3E);
DEFINE_GLOBAL(s16, gStylePauseFont, 0x703DA4);
DEFINE_GLOBAL(s16, gStyleTextFont, 0x703BAA);
DEFINE_GLOBAL(s16, gStyleCarNameFont, 0x703D98);
DEFINE_GLOBAL(s16, gStyleZoneNameFont, 0x703C9C);

MATCH_FUNC(0x5AA3B0)
car_info* gtx_0x106C::get_car_info_5AA3B0(u8 idx) const
{
    return mCari->mCarInfo[idx];
}

MATCH_FUNC(0x5AA3D0)
u8* gtx_0x106C::get_car_door_data(u8 idx) const
{
    car_info* pCarInfo = mCari->mCarInfo[idx];
    return &pCarInfo->remap[pCarInfo->num_remaps];
}

MATCH_FUNC(0x5AA3F0)
sprite_delta* gtx_0x106C::get_delta(u16 sprite_idx, u8 delta_idx) const
{
    delta_container* pDel = mDel;

    if (sprite_idx >= pDel->mCount)
    {
        return NULL;
    }

    sprite_deltas* pEntry = pDel->mEntries[sprite_idx];

    if (!pEntry)
    {
        return NULL;
    }

    if (delta_idx >= pEntry->mCount)
    {
        return NULL;
    }

    sprite_delta* pDelta = &pEntry->mDeltas[delta_idx];

    return pDelta->mLen != 0 ? pDelta : NULL;
}

// note: param type matters
MATCH_FUNC(0x5AA440)
sprite_index* gtx_0x106C::get_sprite_index(u16 idx) const
{
    return &mSpriteIndex[idx];
}

MATCH_FUNC(0x5AA460)
u16 gtx_0x106C::GetSpriteTrueIndex(s32 sprite_type, s16 sprite_idx) const
{
    u16 result;
    switch (sprite_type)
    {
        case sprite_types_enum::car_2:
            result = sprite_idx + mSpriteBase2->mCar;
            break;
        case sprite_types_enum::ped_3:
            result = mSpriteBase2->mPed + sprite_idx;
            break;
        case sprite_types_enum::code_obj1_4:
        case sprite_types_enum::code_obj2_8:
            result = sprite_idx + mSpriteBase2->mCodeObj;
            break;
        case sprite_types_enum::map_obj_5:
            result = sprite_idx + mSpriteBase2->mMapObj;
            break;
        case sprite_types_enum::user_6:
            result = sprite_idx + static_cast<s16>(mSpriteBase2->mUser);
            break;
        case sprite_types_enum::font_7:
            result = sprite_idx + mSpriteBase2->mFont;
            break;
        default:
            // No assignment: VC6 gives result sprite_idx's stack slot, so this returns sprite_idx
            // with the original's 32-bit load. 'result = sprite_idx;' loads 16 bits.
            break;
    }
    return result;
}

MATCH_FUNC(0x5AA4F0)
s16 gtx_0x106C::GetSpriteBaseOfType(s32 sprite_type) const
{
    switch (sprite_type)
    {
        case sprite_types_enum::car_2:
            return mSpriteBase1->mCar;
        case sprite_types_enum::ped_3:
            return mSpriteBase1->mPed;
        case sprite_types_enum::code_obj1_4:
        case sprite_types_enum::code_obj2_8:
            return mSpriteBase1->mCodeObj;
        case sprite_types_enum::map_obj_5:
            return mSpriteBase1->mMapObj;
        case sprite_types_enum::font_7:
            return mSpriteBase1->mFont;
        case sprite_types_enum::user_6:
            return mSpriteBase1->mUser;
        default:
            return 0;
    }
}

MATCH_FUNC(0x5AA560)
s16 gtx_0x106C::GetPaletteBaseOfType(s32 palette_type) const
{
    switch (palette_type)
    {
        case palette_types_enum::tiles_1:
            return mPaletteBase1->mTile;
        case palette_types_enum::sprites_2:
            return mPaletteBase1->mSprite;
        case palette_types_enum::car_remaps_3:
            return mPaletteBase1->mCarRemap;
        case palette_types_enum::ped_remaps_4:
            return mPaletteBase1->mPedRemap;
        case palette_types_enum::code_obj_remaps_5:
            return mPaletteBase1->mCodeObjRemap;
        case palette_types_enum::map_obj_remaps_6:
            return mPaletteBase1->mMapObjRemap;
        case palette_types_enum::user_remaps_7:
            return mPaletteBase1->mUserRemap;
        case palette_types_enum::font_remaps_8:
            return mPaletteBase1->mFontRemap;
        default:
            return 0;
    }
}

MATCH_FUNC(0x5AA5F0)
s16 gtx_0x106C::GetTruePalette(s32 palette_type, s16 pal) const
{
    s16 result;

    switch (palette_type)
    {
        case palette_types_enum::tiles_1:
            result = pal + mPaletteBase2->mTile;
            break;
        case palette_types_enum::sprites_2:
            result = pal + mPaletteBase2->mSprite;
            break;
        case palette_types_enum::car_remaps_3:
            result = pal + mPaletteBase2->mCarRemap;
            break;
        case palette_types_enum::ped_remaps_4:
            result = pal + mPaletteBase2->mPedRemap;
            break;
        case palette_types_enum::code_obj_remaps_5:
            result = pal + mPaletteBase2->mCodeObjRemap;
            break;
        case palette_types_enum::map_obj_remaps_6:
            result = pal + mPaletteBase2->mMapObjRemap;
            break;
        case palette_types_enum::user_remaps_7:
            result = pal + mPaletteBase2->mUserRemap;
            break;
        case palette_types_enum::font_remaps_8:
            result = pal + mPaletteBase2->mFontRemap;
            break;
        default:
            result = pal;
            break;
    }
    return result;
}

MATCH_FUNC(0x5AA6A0)
u8* gtx_0x106C::GetPalData(u16 palette_idx) const
{
    return static_cast<u8*>(mPhysicalPalettes) + 4 * ((palette_idx & 63) + ((palette_idx & 0xFFFFC0) << 8));
}

MATCH_FUNC(0x5AA6F0)
u16 gtx_0x106C::get_phys_pal(u16 palId) const
{
    return mPaletteIndex->mPhysPalette[palId];
}

MATCH_FUNC(0x5AA710)
u16 gtx_0x106C::GetSpriteIdxFromFont(u16 font_type, s16 offset) const
{
    u16 result = mFontBase->mBase[font_type] + offset;
    if (font_type == mFontBase->mFontCount - 1)
    {
        if (result >= mFontBaseTotal)
        {
            return mFontBase->mBase[font_type];
        }
        else
        {
            return result;
        }
    }
    else if (result >= static_cast<u32>(mFontBase->mBase[font_type + 1]))
    {
        return mFontBase->mBase[font_type];
    }
    return result;
}

MATCH_FUNC(0x5AA760)
u16 gtx_0x106C::GetFontWidth(const u16* font_type, const wchar_t* pStr) const
{
    if (*font_type >= 101)
    {
        // Standardized width for all letters
        return (*font_type < 201) ? 16 : 32;
    }
    else
    {
        // In this case letters have different widths
        return this->mSpriteIndex[this->mSpriteBase2->mFont + gtx_0x106C::GetSpriteIdxFromFont(*font_type, *pStr - 33)]
            .mWidth;
    }
}

MATCH_FUNC(0x5AA7B0)
u16 gtx_0x106C::GetSpaceCharWidth(const u16* font_type) const
{
    if (*font_type >= 101)
    {
        // Standardized width for all space characters
        return (*font_type < 201) ? 16 : 32;
    }

    // In this case space characters have different widths for different font types below 101
    return mSpriteIndex[mSpriteBase2->mFont + 77 + mFontBase->mBase[*font_type]].mWidth;
}

MATCH_FUNC(0x5AA800)
s16 gtx_0x106C::GetLineSpacing(const u16* font_type) const
{
    if (*font_type >= 0x65u)
    {
        return (*font_type < 0xC9u) ? 0x11 : 0x22;
    }
    else
    {
        return this->mSpriteIndex[this->mSpriteBase2->mFont + 32 + this->mFontBase->mBase[*font_type]]
            .mHeight;
    }
}

MATCH_FUNC(0x5AA850)
bool gtx_0x106C::IsTileRemapped(u16 tile_idx) const
{
    return mTile->mTileMapping[tile_idx] != tile_idx;
}

MATCH_FUNC(0x5AA870)
s16 gtx_0x106C::GetTile(u16 tile_idx) const
{
    return mTile->mTileMapping[tile_idx];
}

MATCH_FUNC(0x5AA890)
s16 gtx_0x106C::GetFirstFreeReservedTileIdx() const
{
    // Decreasing order: starts from 1023 down to 992
    const u16* i = &mTile->mTileMapping[1023];
    for (u16 j = 1023; j >= 992; j--, i--)
    {
        if (*i == 0)
        {
            return j;
        }
    }

    return 0;
}

MATCH_FUNC(0x5AA8C0)
void* gtx_0x106C::GetTiles() const
{
    return mTiles;
}

MATCH_FUNC(0x5AA900)
u16 gtx_0x106C::get_physical_palettes_len() const
{
    return mPhysicalPalettesLen;
}

MATCH_FUNC(0x5AA910)
object_info* gtx_0x106C::get_map_object_info(u16 idx) const
{
    if (idx >= mMapObjectInfoLen)
    {
        return NULL;
    }
    return &mMapObjectInfo[idx];
}

MATCH_FUNC(0x5AA930)
void gtx_0x106C::SetTileRemap(u16 tile_idx, s16 tile_val)
{
    mTile->mTileMapping[tile_idx] = tile_val;
}

MATCH_FUNC(0x5AA950)
void gtx_0x106C::InitTileMapping()
{
    mTile = new tile_array();

    for (u16 tile_num = 0; tile_num < 992; tile_num++)
    {
        mTile->mTileMapping[tile_num] = tile_num;
    }

    u32 tile_num_2 = 992;
    for (u32 i = 32; i > 0; i--)
    {
        mTile->mTileMapping[tile_num_2++] = 0;
    }
}

MATCH_FUNC(0x5AA9A0)
void gtx_0x106C::BuildCarInfoContainer(s32 chunk_size)
{
    car_info* pCarInfoIter = reinterpret_cast<car_info*>(mCarInfo);
    u32 total_len = 0;
    u32 idx = 0;
    u8 total_sprite = 0;
    u8 last_car_sprite = 0;

    mCari = new car_info_container();

    if (mCari == NULL)
    {
        FatalError(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\style.cpp", 821);
    }

    while (total_len < static_cast<u32>(chunk_size))
    {
        if (idx >= 256)
        {
            FatalError(Gta2Error::FileOverflow, "C:\\Splitting\\Gta2\\Source\\style.cpp", 825);
        }

        if (pCarInfoIter->w > 0x80u || pCarInfoIter->h > 0x80u || pCarInfoIter->num_remaps > 0x40u)
        {
            FatalError(Gta2Error::InvalidCarModelStyleData, "C:\\Splitting\\Gta2\\Source\\style.cpp", 826, pCarInfoIter->model);
        }

        if (pCarInfoIter->sprite != 0 && pCarInfoIter->sprite != 1)
        {
            FatalError(Gta2Error::InvalidCarModelStyleData, "C:\\Splitting\\Gta2\\Source\\style.cpp", 827, pCarInfoIter->model);
        }

        mCari->mCarInfo[pCarInfoIter->model] = pCarInfoIter;

        if (pCarInfoIter->sprite)
        {
            total_sprite += last_car_sprite;
            last_car_sprite = pCarInfoIter->sprite;
        }

        pCarInfoIter->sprite = total_sprite;

        // Offset of num_doors. The door part (2 * num_doors + 1) is its own local, added to the offset at
        // both uses: one expression folds it into a single lea, the original keeps lea / add / add.
        u32 off = pCarInfoIter->num_remaps + 0xE;
        if (reinterpret_cast<u8*>(pCarInfoIter)[off] > 5u)
        {
            FatalError(Gta2Error::InvalidCarModelStyleData, "C:\\Splitting\\Gta2\\Source\\style.cpp", 842, pCarInfoIter->model);
        }
        u32 door_len = reinterpret_cast<u8*>(pCarInfoIter)[off] * sizeof(door_info) + 1;
        total_len += off + door_len;
        pCarInfoIter = reinterpret_cast<car_info*>(reinterpret_cast<u8*>(pCarInfoIter) + off + door_len);
        idx++;
    }
    mCari->mCount = idx;
}

MATCH_FUNC(0x5AAB30)
void gtx_0x106C::load_delx(u32 delx_chunk_size)
{
    delta_entry* pIter = mDeltaIndex;
    u32 off = 0;
    u32 total = 0;

    for (; off < delx_chunk_size;)
    {
        u32 size = 2 * pIter->mDeltaCount + 4;
        mDeltaLen += 8 * pIter->mDeltaCount + 4;
        pIter = reinterpret_cast<delta_entry*>(reinterpret_cast<u8*>(pIter) + size);
        off += size;
    }

    mDeltaBuffer = static_cast<sprite_deltas*>(Memory::Checked_malloc(mDeltaLen));
    delta_entry* pSrc = mDeltaIndex;
    u32 off2 = 0;
    sprite_deltas* pDst = static_cast<sprite_deltas*>(mDeltaBuffer);

    for (; off2 < delx_chunk_size;)
    {
        pDst->mWhichSprite = pSrc->mWhichSprite;
        pDst->mCount = pSrc->mDeltaCount;
        pDst->mPad3 = 0;

        for (u32 i = 0; i < pSrc->mDeltaCount; i++)
        {
            pDst->mDeltas[i].mLen = pSrc->mDelta[i];
            pDst->mDeltas[i].mpData = reinterpret_cast<delta_store_entry*>(total);
            total += pDst->mDeltas[i].mLen;
        }

        u32 size = 2 * pSrc->mDeltaCount + 4;
        pDst = reinterpret_cast<sprite_deltas*>(reinterpret_cast<u8*>(pDst) + 8 * pDst->mCount + 4);
        off2 += size;
        pSrc = reinterpret_cast<delta_entry*>(reinterpret_cast<u8*>(pSrc) + size);
    }
}

MATCH_FUNC(0x5AABF0)
void gtx_0x106C::SetDeltaDataPtrs()
{
    s32 size;
    sprite_deltas* pEntry = static_cast<sprite_deltas*>(mDeltaBuffer);

    for (u32 offset = 0; offset < static_cast<u32>(mDeltaLen); offset += size)
    {
        for (u32 delta_idx = 0; delta_idx < pEntry->mCount; delta_idx++)
        {
            // load_delx left the delta's offset into the store in mpData
            pEntry->mDeltas[delta_idx].mpData = reinterpret_cast<delta_store_entry*>(
                reinterpret_cast<u32>(pEntry->mDeltas[delta_idx].mpData) + reinterpret_cast<u32>(mDeltaStore));
        }
        size = 8 * pEntry->mCount + 4;
        pEntry = reinterpret_cast<sprite_deltas*>(reinterpret_cast<u8*>(pEntry) + size);
    }
}

MATCH_FUNC(0x5AAC40)
void gtx_0x106C::SetSpriteIndexDataPtrs()
{
    for (u32 i = 0; i < static_cast<u16>(this->mSpriteIndexCount); i++)
    {
        u8* pGraphicsOffset = this->mSpriteIndex[i].mpData;
        sprite_index* pSpriteIndex = &this->mSpriteIndex[i];
        pSpriteIndex->mpData = &this->mSpriteGraphics[reinterpret_cast<u32>(pGraphicsOffset)];
    }
}

MATCH_FUNC(0x5AAC70)
void gtx_0x106C::build_delta_container()
{
    u32 off = 0;
    sprite_deltas* pIter = static_cast<sprite_deltas*>(mDeltaBuffer);
    u16 maxSprite = 0;

    for (; off < static_cast<u32>(mDeltaLen);)
    {
        if (pIter->mWhichSprite >= maxSprite)
        {
            maxSprite = pIter->mWhichSprite;
        }

        u32 size = 8 * pIter->mCount + 4;
        off += size;
        pIter = reinterpret_cast<sprite_deltas*>(reinterpret_cast<u8*>(pIter) + size);
    }

    mDel = static_cast<delta_container*>(Memory::Checked_malloc(4 * maxSprite + 8));
    mDel->mCount = maxSprite + 1;

    for (u32 i = 0; i < mDel->mCount; i++)
    {
        mDel->mEntries[i] = NULL;
    }

    pIter = static_cast<sprite_deltas*>(mDeltaBuffer);

    for (u32 off2 = 0; off2 < static_cast<u32>(mDeltaLen);)
    {
        if (mDel->mEntries[pIter->mWhichSprite])
        {
            FatalError(Gta2Error::MultipleSpriteDeltas, "C:\\Splitting\\Gta2\\Source\\style.cpp", 996, pIter->mWhichSprite);
        }

        mDel->mEntries[pIter->mWhichSprite] = pIter;

        if (pIter->mWhichSprite >= maxSprite)
        {
            maxSprite = pIter->mWhichSprite;
        }

        u32 size = 8 * pIter->mCount + 4;
        pIter = reinterpret_cast<sprite_deltas*>(reinterpret_cast<u8*>(pIter) + size);
        off2 += size;
    }
}

MATCH_FUNC(0x5AAD50)
void gtx_0x106C::load_car_info(u32 cari_chunk_size)
{
    mCarInfo = static_cast<car_info**>(Memory::Checked_malloc(cari_chunk_size));
    File::Global_Read(mCarInfo, cari_chunk_size);

    BuildCarInfoContainer(cari_chunk_size);
}

MATCH_FUNC(0x5AAD80)
void gtx_0x106C::load_delta_index(u32 delx_chunk_size)
{
    this->mDeltaIndex = static_cast<delta_entry*>(Memory::Checked_malloc(delx_chunk_size));
    File::Global_Read(mDeltaIndex, delx_chunk_size);

    load_delx(delx_chunk_size);
    build_delta_container();

    crt::free(this->mDeltaIndex);
    this->mDeltaIndex = NULL;
}

MATCH_FUNC(0x5AADD0)
void gtx_0x106C::load_delta_store(u32 dels_chunk_size)
{
    mDeltaStore = static_cast<delta_store_entry*>(Memory::Checked_malloc(dels_chunk_size));
    File::Global_Read(mDeltaStore, dels_chunk_size);
}

MATCH_FUNC(0x5AADF0)
void gtx_0x106C::load_tiles(u32 tile_chunk_len)
{
    mTiles = static_cast<u8*>(Memory::Aligned_malloc(tile_chunk_len, &mTilesUnaligned));
    File::Global_Read(mTiles, tile_chunk_len);
    InitTileMapping();
}

MATCH_FUNC(0x5AAE20)
void gtx_0x106C::skip_ovly(u32 size)
{
    File::Global_Seek(&size);
}

MATCH_FUNC(0x5AAE30)
void gtx_0x106C::skip_psxt(u32 size)
{
    File::Global_Seek(&size);
}

MATCH_FUNC(0x5AAE40)
void gtx_0x106C::load_sprite_graphics(u32 sprg_chunk_len)
{
    mSpriteGraphics = reinterpret_cast<u8*>(Memory::Aligned_malloc(sprg_chunk_len, &mSpriteGraphicsUnaligned));
    File::Global_Read(mSpriteGraphics, sprg_chunk_len);
}

MATCH_FUNC(0x5AAE70)
void gtx_0x106C::load_physical_palettes(u32 ppal_chunk_size)
{
    mPhysicalPalettes = Memory::Aligned_malloc(ppal_chunk_size, &mPhysicalPalettesUnaligned);
    File::Global_Read(mPhysicalPalettes, ppal_chunk_size);
    mPhysicalPalettesLen = ppal_chunk_size >> 10;
}

MATCH_FUNC(0x5AAEA0)
void gtx_0x106C::load_palette_index(u32 palx_chunk_len)
{
    if (palx_chunk_len != sizeof(palette_index))
    {
        FatalError(Gta2Error::InvalidChunkSize, "C:\\Splitting\\Gta2\\Source\\style.cpp", 1157, palx_chunk_len);
    }

    mPaletteIndex = new palette_index();
    if (!mPaletteIndex)
    {
        FatalError(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\style.cpp", 1159);
    }

    File::Global_Read(mPaletteIndex, palx_chunk_len);
}

MATCH_FUNC(0x5AAF00)
void gtx_0x106C::load_map_object_info(u32 obji_chunk_len)
{
    mMapObjectInfo = static_cast<object_info*>(Memory::Checked_malloc(obji_chunk_len));
    File::Global_Read(mMapObjectInfo, obji_chunk_len);
    if (obji_chunk_len / 2 >= 0x10000)
    {
        FatalError(Gta2Error::TooManyLoadedObjectInfos, "C:\\Splitting\\Gta2\\Source\\style.cpp", 1177, obji_chunk_len / 2);
    }

    mMapObjectInfoLen = obji_chunk_len / 2;
    if (((obji_chunk_len / 2) & 0xffff) * 2 != obji_chunk_len)
    {
        FatalError(Gta2Error::InvalidLoadedObjectInfoData, "C:\\Splitting\\Gta2\\Source\\style.cpp", 1179);
    }
}

MATCH_FUNC(0x5AAF80)
void gtx_0x106C::load_sprite_index(u32 sprx_chunk_size)
{
    mSpriteIndex = static_cast<sprite_index*>(Memory::Checked_malloc(sprx_chunk_size + 8));
    File::Global_Read(mSpriteIndex, sprx_chunk_size);
    if ((sprx_chunk_size & ~7u) > 524280)
    {
        FatalError(Gta2Error::TooManySprites, "C:\\Splitting\\Gta2\\Source\\style.cpp", 1198, sprx_chunk_size >> 3);
    }
    mSpriteIndexCount = (sprx_chunk_size >> 3) + 1;
}

MATCH_FUNC(0x5AAFE0)
void gtx_0x106C::InitFontTypes(u16 fontCount)
{
    if (fontCount == 14)
    {
        gStyleMessageFont = 0;
        gKanjiSmallFont = 1;
        gKanjiLargeFont = 2;
        gStylePauseFont = 3;
        gFrontendNormalFont = 4;
        gFrontendSelectedFont = 5;
        gFrontendFont6 = 6;
        gFrontendFont7 = 7;
        gFrontendHeadingFont = 8;
        gFrontendPlayMenuScoreFont = 9;
        gFrontendScoreFont = 10;
        gFrontendAreaCompleteFont = 11;
        gFrontendFont12 = 12;
        gFrontendTitleFont = 13;
        gStyleTextFont = 99;
        gStyleCarNameFont = 99;
        gStyleZoneNameFont = 99;
    }
    else if (fontCount == 7)
    {
        gStyleMessageFont = 0;
        gStyleTextFont = 1;
        gStyleCarNameFont = 2;
        gKanjiSmallFont = 3;
        gKanjiLargeFont = 4;
        gStyleZoneNameFont = 5;
        gStylePauseFont = 6;
    }
    else
    {
        FatalError(Gta2Error::StyleFileContainsUnexpectedNumberOfFonts, "C:\\Splitting\\Gta2\\Source\\style.cpp", 1251, fontCount);
    }
}

MATCH_FUNC(0x5AB0F0)
void gtx_0x106C::load_font_base(u32 fonb_chunk_size)
{
    if (fonb_chunk_size < 2)
    {
        FatalError(Gta2Error::InvalidChunkSize, "C:\\Splitting\\Gta2\\Source\\style.cpp", 1268, fonb_chunk_size);
    }

    mFontBase = static_cast<font_base*>(Memory::Checked_malloc(fonb_chunk_size));
    File::Global_Read(mFontBase, fonb_chunk_size);

    u32 font_count = mFontBase->mFontCount;
    if (fonb_chunk_size != (font_count * 2) + 2)
    {
        FatalError(Gta2Error::InvalidChunkSize, "C:\\Splitting\\Gta2\\Source\\style.cpp", 1271, fonb_chunk_size);
    }

    mFontBaseTotal = SumCounts(mFontBase->mBase, mFontBase->mFontCount);

    ConvertToVirtualOffsets(mFontBase->mBase, mFontBase->mFontCount);

    InitFontTypes(mFontBase->mFontCount);
}

MATCH_FUNC(0x5AB1A0)
u16 __stdcall gtx_0x106C::SumCounts(const u16* pOffsets, u32 offsetsCount)
{
    u16 total = 0;
    while (offsetsCount > 0)
    {
        total += *pOffsets;
        pOffsets++;
        offsetsCount--;
    }
    return total;
}

MATCH_FUNC(0x5AB1C0)
void __stdcall gtx_0x106C::ConvertToVirtualOffsets(u16* pBuffer, u32 len)
{
    for (s32 i = len - 1; i > 0; i--)
    {
        pBuffer[i] = pBuffer[i - 1];
    }
    if (len != 0)
    {
        pBuffer[0] = 0;
    }
    s32 j = 1;
    for (s32 k = len; k > 1; k--, j++)
    {
        pBuffer[j] += pBuffer[j - 1];
    }
}

MATCH_FUNC(0x5AB210)
void gtx_0x106C::load_sprite_base(u32 sprite_base_chunk_size)
{
    if (sprite_base_chunk_size != sizeof(sprite_base))
    {
        FatalError(Gta2Error::InvalidChunkSize, "C:\\Splitting\\Gta2\\Source\\style.cpp", 1289, sprite_base_chunk_size);
    }

    mSpriteBase1 = new sprite_base();
    if (!mSpriteBase1)
    {
        FatalError(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\style.cpp", 1291);
    }

    mSpriteBase2 = new sprite_base();
    if (!mSpriteBase2)
    {
        FatalError(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\style.cpp", 1293);
    }

    File::Global_Read(mSpriteBase1, sprite_base_chunk_size);

    *mSpriteBase2 = *mSpriteBase1;

    ConvertToVirtualOffsets(&mSpriteBase2->mCar, 6);
}

MATCH_FUNC(0x5AB2C0)
void gtx_0x106C::load_palete_base(u32 palette_base_chunk_len)
{
    if (palette_base_chunk_len != sizeof(palette_base))
    {
        FatalError(Gta2Error::InvalidChunkSize, "C:\\Splitting\\Gta2\\Source\\style.cpp", 1311, palette_base_chunk_len);
    }

    mPaletteBase1 = new palette_base();
    if (!mPaletteBase1)
    {
        FatalError(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\style.cpp", 1313);
    }

    mPaletteBase2 = new palette_base();
    if (!mPaletteBase2)
    {
        FatalError(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\style.cpp", 1315);
    }

    File::Global_Read(mPaletteBase1, palette_base_chunk_len);

    *mPaletteBase2 = *mPaletteBase1;

    mTotalPalBase = SumCounts(&mPaletteBase2->mTile, 8);
    ConvertToVirtualOffsets(&mPaletteBase2->mTile, 8);
}

MATCH_FUNC(0x5AB380)
bool gtx_0x106C::IsCarModelInRecycleList(u8 car_id) const
{
    // Certain car models are in the recycle list only for certain stages/districts
    s32 i = 0;
    if (!mCarRecyclingInfo)
    {
        return true;
    }

    // This while only ends if it finds 'car_id' or if it reaches the 
    // sentinel value 255, which is surely greater than 'car_id'
    while (mCarRecyclingInfo[i] < car_id)
    {
        i++;
    }
    return mCarRecyclingInfo[i] == car_id;
}

MATCH_FUNC(0x5AB3C0)
void gtx_0x106C::load_car_recycling_info(u32 recy_chunk_size)
{
    mCarRecyclingInfo = static_cast<u8*>(Memory::Checked_malloc(recy_chunk_size));
    mRecyChunkSize = recy_chunk_size;
    File::Global_Read(mCarRecyclingInfo, recy_chunk_size);
}

MATCH_FUNC(0x5AB3F0)
void gtx_0x106C::read_spec(u32 spec)
{
    u16 index_to_set;
    u32 len = 2;
    File::Global_Read(&index_to_set, len);
    while (index_to_set)
    {
        this->mSpec[index_to_set] = spec;
        s32 len2 = 2;
        File::Global_Read(&index_to_set, len2);
    }
}

MATCH_FUNC(0x5AB450)
void gtx_0x106C::load_spec()
{
    read_spec(tile_spec::grass_dirt_floor);
    read_spec(tile_spec::road_junction_special);
    read_spec(tile_spec::water);
    read_spec(tile_spec::electrified_floor);
    read_spec(tile_spec::electrified_platform_floor);
    read_spec(tile_spec::wood_floor);
    read_spec(tile_spec::metal_floor);
    read_spec(tile_spec::metal_wall);
    read_spec(tile_spec::grass_dirt_wall);
}

MATCH_FUNC(0x5AB4B0)
void gtx_0x106C::LoadChunk(const char_type* pChunkType, u32 chunk_len)
{
    if (!strncmp(pChunkType, "PALB", 4u))
    {
        load_palete_base(chunk_len);
    }
    else if (!strncmp(pChunkType, "SPRB", 4u))
    {
        load_sprite_base(chunk_len);
    }
    else if (!strncmp(pChunkType, "FONB", 4u))
    {
        load_font_base(chunk_len);
    }
    else if (!strncmp(pChunkType, "SPRX", 4u))
    {
        load_sprite_index(chunk_len);
    }
    else if (!strncmp(pChunkType, "OBJI", 4u))
    {
        load_map_object_info(chunk_len);
    }
    else if (!strncmp(pChunkType, "PALX", 4u))
    {
        load_palette_index(chunk_len);
    }
    else if (!strncmp(pChunkType, "PPAL", 4u))
    {
        load_physical_palettes(chunk_len);
    }
    else if (!strncmp(pChunkType, "SPRG", 4u))
    {
        load_sprite_graphics(chunk_len);
    }
    else if (!strncmp(pChunkType, "TILE", 4u))
    {
        load_tiles(chunk_len);
    }
    else if (!strncmp(pChunkType, "DELS", 4u))
    {
        load_delta_store(chunk_len);
    }
    else if (!strncmp(pChunkType, "DELX", 4u))
    {
        load_delta_index(chunk_len);
    }
    else if (!strncmp(pChunkType, "CARI", 4u))
    {
        load_car_info(chunk_len);
    }
    else if (!strncmp(pChunkType, "PSXT", 4u))
    {
        skip_psxt(chunk_len);
    }
    else if (!strncmp(pChunkType, "OVLY", 4u))
    {
        skip_ovly(chunk_len);
    }
    else if (!strncmp(pChunkType, "SPEC", 4u))
    {
        load_spec();
    }
    else if (!strncmp(pChunkType, "RECY", 4u))
    {
        load_car_recycling_info(chunk_len);
    }
    else
    {
        File::Global_Seek(&chunk_len);
    }
}

MATCH_FUNC(0x5AB720)
void gtx_0x106C::SetDataPtrs()
{
    if (mDeltaBuffer && mDeltaStore)
    {
        SetDeltaDataPtrs();
    }

    if (mSpriteIndex && mSpriteGraphics)
    {
        SetSpriteIndexDataPtrs();
    }
}

MATCH_FUNC(0x5AB750)
void gtx_0x106C::LoadSty(const char_type* pStyFileName)
{
    File::Global_Open(pStyFileName);

    file_header styHeader;
    u32 len = sizeof(file_header);
    File::Global_Read(styHeader.mType, len);

    styHeader.verify_type("GBST");
    styHeader.verify_version(700);

    chunk_header chunkHeader;
    for (len = sizeof(chunk_header); File::Global_TryRead(&chunkHeader, &len); len = sizeof(chunk_header))
    {
        if (chunkHeader.mSize != 0)
        {
            LoadChunk(chunkHeader.mType, chunkHeader.mSize);
        }
    }

    File::Global_Close();

    SetDataPtrs();
}

MATCH_FUNC(0x5AB820)
gtx_0x106C::gtx_0x106C()
{
    ResetSpecs();

    mCarRecyclingInfo = NULL;
    mRecyChunkSize = 0;
    mSpriteBase2 = NULL;
    mPaletteBase2 = NULL;
    mFontBase = NULL;
    mSpriteIndex = NULL;
    mMapObjectInfo = NULL;
    mMapObjectInfoLen = 0;
    mPaletteIndex = NULL;
    mPhysicalPalettes = NULL;
    mPhysicalPalettesUnaligned = NULL;
    mTiles = NULL;
    mTilesUnaligned = NULL;
    mSpriteGraphics = NULL;
    mSpriteGraphicsUnaligned = NULL;
    mDeltaStore = NULL;
    mDeltaBuffer = NULL;
    mCarInfo = NULL;
    mCari = NULL;
    mDeltaIndex = NULL;
    mDeltaLen = 0;
    mDel = NULL;
    mSpriteIndexCount = 0;
    mTotalPalBase = 0;
    mFontBaseTotal = 0;
    mTile = NULL;
    mPhysicalPalettesLen = 0;
    mPalettesConverted = false;
    mPaletteBase1 = NULL;
    mSpriteBase1 = NULL;
}

MATCH_FUNC(0x5AB8A0)
gtx_0x106C::~gtx_0x106C()
{
    sprite_base* pSpriteBase2;
    sprite_base* pSpriteBase1;
    palette_base* pPaletteBase2;
    palette_base* pPaletteBase1;
    font_base* pFontBase;
    sprite_index* pSpriteIndex;
    object_info* pMapObjectInfo;
    palette_index* pPaletteIndex;
    void* pPhysPalettesUnaligned;
    void* pTilesUnaligned;
    void* pSpriteGraphicsUnaligned;
    delta_store_entry* pDeltaStore;
    void* pDeltaBuffer;
    void* pCarInfo;
    car_info_container* pCarInfoContainer;
    delta_entry* pDeltaIndex;
    void* pDeltaContainer;
    tile_array* pTileArray;

    if (mCarRecyclingInfo)
    {
        crt::free(mCarRecyclingInfo);
    }

    pSpriteBase2 = mSpriteBase2;
    mCarRecyclingInfo = NULL;
    delete pSpriteBase2;

    pSpriteBase1 = mSpriteBase1;
    mSpriteBase2 = NULL;
    delete (pSpriteBase1);

    pPaletteBase2 = mPaletteBase2;
    mSpriteBase1 = NULL;
    delete (pPaletteBase2);
    pPaletteBase1 = mPaletteBase1;
    mPaletteBase2 = NULL;
    delete (pPaletteBase1);
    pFontBase = mFontBase;
    mPaletteBase1 = NULL;
    if (pFontBase)
    {
        crt::free(pFontBase);
    }

    pSpriteIndex = mSpriteIndex;
    mFontBase = NULL;
    if (pSpriteIndex)
    {
        crt::free(pSpriteIndex);
    }
    pMapObjectInfo = mMapObjectInfo;
    mSpriteIndex = NULL;
    if (pMapObjectInfo)
    {
        crt::free(pMapObjectInfo);
    }
    pPaletteIndex = mPaletteIndex;
    mMapObjectInfo = NULL;
    delete (pPaletteIndex);
    pPhysPalettesUnaligned = mPhysicalPalettesUnaligned;
    mPaletteIndex = NULL;
    if (pPhysPalettesUnaligned)
    {
        crt::free(pPhysPalettesUnaligned);
    }
    pTilesUnaligned = mTilesUnaligned;
    mPhysicalPalettes = NULL;
    mPhysicalPalettesUnaligned = NULL;
    if (pTilesUnaligned)
    {
        crt::free(pTilesUnaligned);
    }
    pSpriteGraphicsUnaligned = mSpriteGraphicsUnaligned;
    mTiles = NULL;
    mTilesUnaligned = NULL;
    if (pSpriteGraphicsUnaligned)
    {
        crt::free(pSpriteGraphicsUnaligned);
    }
    pDeltaStore = mDeltaStore;
    mSpriteGraphics = NULL;
    mSpriteGraphicsUnaligned = NULL;
    if (pDeltaStore)
    {
        crt::free(pDeltaStore);
    }
    pDeltaBuffer = mDeltaBuffer;
    mDeltaStore = NULL;
    if (pDeltaBuffer)
    {
        crt::free(pDeltaBuffer);
    }
    pCarInfo = mCarInfo;
    mDeltaBuffer = NULL;
    if (pCarInfo)
    {
        crt::free(pCarInfo);
    }
    pCarInfoContainer = mCari;
    mCarInfo = NULL;
    delete (pCarInfoContainer);
    pDeltaIndex = mDeltaIndex;
    mCari = NULL;
    if (pDeltaIndex)
    {
        crt::free(pDeltaIndex);
    }
    pDeltaContainer = static_cast<void*>(mDel);
    mDeltaIndex = NULL;
    if (pDeltaContainer)
    {
        crt::free(pDeltaContainer);
    }
    pTileArray = mTile;
    mDel = NULL;
    delete (pTileArray);
    mTile = NULL;
}

// https://decomp.me/scratch/CjuP1
MATCH_FUNC(0x5ABA00)
void sprite_delta::ApplyToPixels(u8* pArray)
{
    // Inline asm in the original (lodsw, rep movsb). Each entry is a u16 offset from the end
    // of the previous run, a u8 length and that many bytes.
    sprite_delta* pThis = this;
    __asm
    {
        push edi
        push esi
        mov eax, 0
        mov ecx, 0
        mov edi, pArray
        mov ebx, pThis
        mov esi, [ebx]
        movzx ebx, word ptr [ebx+4]
        add ebx, esi
    next_entry:
        lodsw
        mov cl, [esi]
        add edi, eax
        inc esi
        rep movsb
        cmp esi, ebx
        jne next_entry
        pop esi
        pop edi
    }
}

// https://decomp.me/scratch/Cc0Dx Not fully working
MATCH_FUNC(0x5ABA40)
void sprite_delta::ApplyMirroredToPixels(u8* pArray, u32 width)
{
    // Inline asm in the original (lodsw / loop)
    // Each entry: u16 offset, u8 len, then len bytes written backwards from the end of the row
    __asm
    {
        push edi
        push esi
        mov eax, 0
        mov ecx, 0
        mov edi, pArray
        mov ebx, this
        mov esi, [ebx]
        movzx ebx, word ptr [ebx + 4]
        add ebx, esi
        dec edi
        add edi, width
    next_entry:
        lodsw
        mov cl, [esi]
        add edi, eax
        and eax, 0xFF
        shl eax, 1
        sub edi, eax
        cmp eax, 0x100
        jl skip_wrap
        add edi, 0x200
    skip_wrap:
        inc esi
    copy_bytes:
        mov al, [esi]
        mov [edi], al
        inc esi
        dec edi
        loop copy_bytes
        cmp esi, ebx
        jne next_entry
        pop esi
        pop edi
    }
}

MATCH_FUNC(0x5abaa0)
void sprite_index::ClearPixelsOfColour(u8 clear_target)
{
    for (s32 i = 0; i < mHeight; i++)
    {
        ClearPixelsOfColourInRow(&this->mpData[i * 256], this->mWidth, clear_target);
    }
}

MATCH_FUNC(0x5ABAE0)
void __stdcall ClearPixelsOfColourInRow(u8* pData, s32 width, u8 clear_target)
{
    u8* pDataIter = pData;
    for (s32 i = 0; i < width; i++)
    {
        if (*pDataIter == clear_target)
        {
            *pDataIter = 0;
        }
        pDataIter++;
    }
}

MATCH_FUNC(0x5abb00)
void sprite_index::CopyPixels(u8* pDst)
{
    for (s32 ypos = 0; ypos < mHeight; ypos++)
    {
        s32 idx = ypos << 8;
        memcpy(&pDst[idx], &mpData[idx], mWidth);
    }
}