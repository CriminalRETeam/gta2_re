#pragma once

#include "Function.hpp"
#include "enums.hpp"
#include <cstdio>
#include <windows.h>

struct palette_base
{
    u16 mTile; // 0x0
    u16 mSprite; // 0x2
    u16 mCarRemap; // 0x4
    u16 mPedRemap; // 0x6
    u16 mCodeObjRemap; // 0x8
    u16 mMapObjRemap; // 0xA
    u16 mUserRemap; // 0xC
    u16 mFontRemap; // 0xE
};

struct sprite_base
{
    u16 mCar; // 0x0
    u16 mPed; // 0x2
    u16 mCodeObj; // 0x4
    u16 mMapObj; // 0x6
    u16 mUser; // 0x8
    u16 mFont; // 0xA
};

struct font_base
{
    u16 mFontCount; // 0x0
    u16 mBase[1]; // 0x2: variable, see font_count
};

struct sprite_index
{
    EXPORT void ClearPixelsOfColour(u8 clear_target);
    EXPORT void CopyPixels(u8* pDst);

    u8* mpData; // 0x0
    u8 mWidth; // 0x4
    u8 mHeight; // 0x5
    s16 mPad6; // 0x6
};

struct object_info
{
    u8 mModel; // 0x0
    u8 mSprites; // 0x1
};

struct palette_index
{
    u16 mPhysPalette[16384]; // 0x0
};

struct tile_array
{
    u16 mTileMapping[1024]; // 0x0
};

struct delta_entry
{
    s16 mWhichSprite; // 0x0
    u8 mDeltaCount; // 0x2
    u8 mPad3; // 0x3
    s16 mDelta[2]; // 0x4
};

struct door_info
{
    char_type rx, ry;
};

// car_info::info_flags bits (from how Car uses them)
namespace car_info_flag
{
enum
{
    emergency = 0x2, // police/ambulance/FBI style lights, excluded from respray
    roof_light = 0x4,
    truck_cab = 0x8, // can pull a trailer
    trailer = 0x10, // is a trailer (not enterable, has a hitch point)
    taxi_light = 0x20,
    bottom_roof_light = 0x40,
    top_roof_lights = 0x80,
};
} // namespace car_info_flag

// car_info::info_flags_2 bits
namespace car_info_flag2
{
enum
{
    heavy = 0x1, // crushes cars, different crash sound and sprite number
    popup_headlights = 0x2,
};
} // namespace car_info_flag2

class car_info
{
  public:
    BYTE model;
    BYTE sprite;
    BYTE w;
    BYTE h;
    BYTE num_remaps;
    BYTE passengers;
    BYTE wreck;
    BYTE rating;
    s8 front_wheel_offset;
    s8 rear_wheel_offset;
    s8 front_window_offset;
    s8 rear_window_offset;
    BYTE info_flags;
    BYTE info_flags_2;
    BYTE remap[1]; // [variable, see num_remaps];
    BYTE num_doors;
    door_info doors[1]; // [variable, see num_doors];

    bool has_remaps() const
    {
        return num_remaps > 1;
    }

    // 9.6f 0x41FEA0
    bool IsTruckCab() const
    {
        return (this->info_flags & car_info_flag::truck_cab) == car_info_flag::truck_cab;
    }

    // Like IsTruckCab; going through a car_info method keeps the check a plain branch in
    // Car::CanBeLiftedByCrane (a direct `(flags & 0x10) == 0x10` there gives a sete)
    bool IsTrailer() const
    {
        return (this->info_flags & car_info_flag::trailer) == car_info_flag::trailer;
    }

    // 9.6f 0x41FF00
    bool IsHeavy() const
    {
        return (this->info_flags_2 & car_info_flag2::heavy) == car_info_flag2::heavy;
    }
};

struct car_info_container
{
    car_info_container()
    {
        mCount = 0;
        memset(mCarInfo, 0, sizeof(mCarInfo));
    }
    car_info* mCarInfo[256]; // 0x0
    u8 mCount; // 0x400
    //char_type field_401;// pad
    //char_type field_402;// pad
    //char_type field_403;// pad
};

struct delta_store_entry
{
    u16 mOffset; // 0x0
    u8 mLen; // 0x2
    u8 mData[1]; // 0x3: variable, see mLen
};

struct sprite_delta
{
    EXPORT void Delta_5ABA00(u8* pArray);
    EXPORT void Delta_5ABA40(u8* pArray, u32 width);

    delta_store_entry* mpData; // 0x0
    u16 mLen; // 0x4
    u16 mPad6; // 0x6
};

struct sprite_deltas
{
    u16 mWhichSprite; // 0x0
    u8 mCount; // 0x2
    u8 mPad3; // 0x3
    sprite_delta mDeltas[1]; // 0x4
};

struct delta_container
{
    u16 mCount; // 0x0
    u16 mPad2; // 0x2
    sprite_deltas* mEntries[1]; // 0x4
};

namespace tile_spec
{
enum
{
    none = 1,
    grass_dirt_floor = 2,
    road_junction_special = 3,
    water = 4,
    electrified_floor = 5,
    electrified_platform_floor = 6,
    wood_floor = 7,
    metal_floor = 8,
    metal_wall = 9,
    grass_dirt_wall = 10,
};
} // namespace tile_spec

class gtx_0x106C
{
  public:
    // 9.6f 0x49E540
    inline bool IsRemappedWaterTile(u16 spec_idx) const
    {
        if (mSpec[spec_idx] == tile_spec::water && IsTileRemapped(spec_idx))
        {
            return true;
        }
        return false;
    }

    // 9.6f 0x4C03D0 (a ctor of the spec array there)
    inline void ResetSpecs()
    {
        for (s32 i = 0; i < GTA2_COUNTOF(mSpec); i++)
        {
            mSpec[i] = tile_spec::none;
        }
    }

    // 9.6f 0x462FB0
    inline bool IsWaterTile(u16 spec_idx) const
    {
        return mSpec[spec_idx] == tile_spec::water;
    }

    // 9.6f 0x42A810
    inline bool IsRoadTile(u16 spec_idx) const
    {
        return mSpec[spec_idx] == tile_spec::road_junction_special;
    }

    // 9.6f 0x462FD0
    inline s32 GetSpecIgnoringRoadJunction(u16 spec_idx) const
    {
        s32 result = mSpec[spec_idx];
        if (result == tile_spec::road_junction_special)
        {
            result = tile_spec::none;
        }
        return result;
    }

    // 9.6f 0x49E570
    inline bool IsUnremappedWaterTile(u16 spec_idx) const
    {
        if (mSpec[spec_idx] == tile_spec::water && !IsTileRemapped(spec_idx))
        {
            return true;
        }
        return false;
    }

    // inline
    // 9.6f 0x4C2EE0
    bool has_tiles() const
    {
        return mTiles != NULL;
    }

    // inline
    // 9.6f 0x4C2EB0
    u8* get_tile(u16 tile_num) const
    {
        return &mTiles[(((tile_num & ~3) * 64) + (tile_num & 3)) * 64];
    }

    // 9.6f 0x420220
    inline u8 get_sprite_width(u16 sprite_idx) const
    {
        return get_sprite_index(sprite_idx)->mWidth;
    }

    // 9.6f 0x4C6C90
    inline u8 get_sprite_height(u16 sprite_idx) const
    {
        return get_sprite_index(sprite_idx)->mHeight;
    }

    EXPORT car_info* get_car_info_5AA3B0(u8 idx) const;

    EXPORT u8* get_car_door_data(u8 idx) const;

    EXPORT sprite_delta* get_delta(u16 sprite_idx, u8 delta_idx) const;

    EXPORT sprite_index* get_sprite_index(u16 idx) const;

    EXPORT u16 GetSpriteTrueIndex(s32 sprite_type, s16 sprite_idx) const;

    EXPORT s16 GetSpriteBaseOfType(s32 sprite_type) const;

    EXPORT s16 GetPaletteBaseOfType(s32 palette_type) const;

    EXPORT s16 GetTruePalette(s32 palette_type, s16 pal) const; // maybe "get virtual palette"?

    EXPORT u8* GetPalData(u16 palette_idx) const;

    EXPORT u16 get_phys_pal(u16 palId) const;

    EXPORT u16 GetSpriteIdxFromFont(u16 font_type, s16 offset) const;

    EXPORT u16 GetFontWidth(const u16* font_type, const wchar_t* pStr) const;

    EXPORT u16 GetSpaceCharWidth(const u16* font_type) const;

    EXPORT s16 GetLineSpacing(const u16* font_type) const;

    EXPORT bool IsTileRemapped(u16 tile_idx) const;

    EXPORT s16 GetTile(u16 tile_idx) const;

    EXPORT s16 GetFirstFreeReservedTileIdx() const;

    EXPORT void* GetTiles() const;

    EXPORT u16 get_physical_palettes_len() const;

    EXPORT object_info* get_map_object_info(u16 idx) const;

    EXPORT void SetTileRemap(u16 tile_idx, s16 tile_val);

    EXPORT void InitTileMapping();

    EXPORT void BuildCarInfoContainer(s32 chunk_size);

    EXPORT void load_delx(u32 delx_chunk_size);

    EXPORT void SetDeltaDataPtrs();

    EXPORT void SetSpriteIndexDataPtrs();

    EXPORT void build_delta_container();

    EXPORT void load_car_info(u32 cari_chunk_size);

    EXPORT void load_delta_index(u32 delx_chunk_size);

    EXPORT void load_delta_store(u32 dels_chunk_size);

    EXPORT void load_tiles(u32 tile_chunk_len);

    EXPORT void skip_ovly(u32 size);

    EXPORT void skip_psxt(u32 size);

    EXPORT void load_sprite_graphics(u32 sprg_chunk_len);

    EXPORT void load_physical_palettes(u32 ppal_chunk_size);

    EXPORT void load_palette_index(u32 palx_chunk_len);

    EXPORT void load_map_object_info(u32 obji_chunk_len);

    EXPORT void load_sprite_index(u32 sprx_chunk_size);

    EXPORT void InitFontTypes(u16 fontCount);

    EXPORT void load_font_base(u32 fonb_chunk_size);

    EXPORT static u16 __stdcall SumCounts(const u16* pOffsets, u32 offsetsCount);

    EXPORT static void __stdcall ConvertToVirtualOffsets(u16* pBuffer, u32 len);

    EXPORT void load_sprite_base(u32 sprite_base_chunk_size);

    EXPORT void load_palete_base(u32 palette_base_chunk_len);

    EXPORT bool IsCarModelInRecycleList(u8 car_id) const;

    EXPORT void load_car_recycling_info(u32 recy_chunk_size);

    EXPORT void read_spec(u32 spec);

    EXPORT void load_spec();

    EXPORT void LoadChunk(const char_type* pChunkType, u32 chunk_len);

    EXPORT void SetDataPtrs();

    EXPORT void LoadSty(const char_type* pStyFileName);

    // 0x5AB820
    EXPORT gtx_0x106C();

    // 0x5AB8A0
    EXPORT ~gtx_0x106C();

    // inlined v9.6f, 0x432850
    u8 get_number_of_cars() const
    {
        return mCari->mCount;
    }

    // inlined v9.6f, 0x420200
    bool does_car_exist(u8 model_idx) const
    {
        return mCari->mCarInfo[model_idx] != NULL;
    }

    // 9.6f 0x491F80
    inline bool IsElectrifiedFloorType(u16 tile_idx) const
    {
        return mSpec[tile_idx] == tile_spec::electrified_floor;
    }

    inline car_info* get_car_info_4BF1F0(u8 model_idx) const
    {
        return mCari->mCarInfo[model_idx];
    }

    s16 mTotalPalBase; // 0x0
    u16 mFontBaseTotal; // 0x2
    s16 mSpriteIndexCount; // 0x4
    u16 mMapObjectInfoLen; // 0x6
    s16 mPhysicalPalettesLen; // 0x8
    s16 mUnusedA; // 0xA
    palette_base* mPaletteBase2; // 0xC
    palette_base* mPaletteBase1; // 0x10
    sprite_base* mSpriteBase2; // 0x14
    sprite_base* mSpriteBase1; // 0x18
    font_base* mFontBase; // 0x1C
    sprite_index* mSpriteIndex; // 0x20
    object_info* mMapObjectInfo; // 0x24
    palette_index* mPaletteIndex; // 0x28
    void* mPhysicalPalettes; // 0x2C
    void* mPhysicalPalettesUnaligned; // 0x30
    BYTE* mSpriteGraphics; // 0x34
    void* mSpriteGraphicsUnaligned; // 0x38
    u8* mTiles; // 0x3C
    tile_array* mTile; // 0x40
    void* mTilesUnaligned; // 0x44
    delta_store_entry* mDeltaStore; // 0x48
    delta_entry* mDeltaIndex; // 0x4C
    void* mDeltaBuffer; // 0x50
    delta_container* mDel; // 0x54
    car_info** mCarInfo; // 0x58
    car_info_container* mCari; // 0x5C
    s32 mDeltaLen; // 0x60
    u8* mCarRecyclingInfo; // 0x64
    s16 mRecyChunkSize; // 0x68
    bool mPalettesConverted; // 0x6A
    char_type mUnused6B; // 0x6B
    s32 mSpec[1024]; // 0x6C
};

EXTERN_GLOBAL(gtx_0x106C*, gGtx_0x106C);

// 9.6f 0x4C23D0
inline s32 __stdcall GetSpaceWidth(u16 font_type)
{
    return static_cast<u16>(gGtx_0x106C->GetSpaceCharWidth(&font_type));
}

EXTERN_GLOBAL(s16, gStyleCarNameFont);
EXTERN_GLOBAL(s16, gStyleTextFont);
EXTERN_GLOBAL(s16, gStyleZoneNameFont);
EXTERN_GLOBAL(s16, gStyleMessageFont);
EXTERN_GLOBAL(s16, gStylePauseFont);
EXTERN_GLOBAL(s16, gKanjiSmallFont);
EXTERN_GLOBAL(s16, gKanjiLargeFont);
