#pragma once

#include "Function.hpp"
#include "ang16.hpp"
#include "fix16.hpp"
#include "gtx_0x106C.hpp"
#include <windows.h>

class Gang;
class Sprite;
class Fix16_Rect;

struct gmp_col_info
{
    u8 mHeight; // 0x0
    u8 mOffset; // 0x1
    s16 mPad2; // 0x2
    s32 mBlockd[2]; // 0x4
};

// gmp_block_info::mArrows bits: the low nibble is the green arrows, the high nibble the red ones
namespace block_arrow
{
enum
{
    green_left_1 = 0x1,
    green_right_2 = 0x2,
    green_up_4 = 0x4,
    green_down_8 = 0x8,
    red_left_10 = 0x10,
    red_right_20 = 0x20,
    red_up_40 = 0x40,
    red_down_80 = 0x80,
    green_mask = 0xF,
    red_mask = 0xF0,
    left_mask = 0x11, // green or red left
    right_mask = 0x22,
    horizontal_mask = 0x33, // left or right
    vertical_mask = 0xCC, // up or down
};
} // namespace block_arrow

struct gmp_block_info
{
    // 9.6f 0x44C840
    inline void init()
    {
        mLeft = 0;
        mRight = 0;
        mTop = 0;
        mBottom = 0;
        mLid = 0;
        mArrows = 0;
        mSlopeType = 0;
    }

    u16 mLeft; // 0x0
    u16 mRight; // 0x2
    u16 mTop; // 0x4
    u16 mBottom; // 0x6
    u16 mLid; // 0x8
    u8 mArrows; // 0xA
    u8 mSlopeType; // 0xB
};

enum gmp_slope_type
{
    AIR = 0,
    ROAD = 1,
    PAVEMENT = 2,
    FIELD = 3,
};

enum gmp_zone_type
{
    General_Purpose_0 = 0,
    Navigation_1 = 1,
    Traffic_light_2 = 2,
    Unused_3 = 3,
    Unused_4 = 4,
    Arrow_Blocker_5 = 5,
    Railway_Station_Platform_6 = 6,
    Bus_Stop_7 = 7,
    General_Trigger_8 = 8,
    Unused_9 = 9,
    Information_10 = 10,
    Railway_Station_Entry_Point_11 = 11,
    Railway_Station_Exit_Point_12 = 12,
    Railway_Station_Stop_Point_13 = 13,
    Gang_14 = 14,
    Local_Navigation_15 = 15,
    Restart_16 = 16,
    Unused_17 = 17,
    Unused_18 = 18,
    Unused_19 = 19,
    Arrest_Restart_20 = 20,
};

enum MapSlopes
{
    DIAGONAL_WALL_UP_LEFT = 0xB4,
    DIAGONAL_WALL_UP_RIGHT = 0xB8,
    DIAGONAL_WALL_DOWN_LEFT = 0xBC,
    DIAGONAL_WALL_DOWN_RIGHT = 0xC0,
    TRIANGULAR_SIDES_DIAGONAL_UP_LEFT = 0xC4,
    TRIANGULAR_SIDES_DIAGONAL_UP_RIGHT = 0xC8,
    TRIANGULAR_SIDES_DIAGONAL_DOWN_LEFT = 0xCC,
    TRIANGULAR_SIDES_DIAGONAL_DOWN_RIGHT = 0xD0,
    PARTIAL_BLOCK_LEFT = 0xD4,
    PARTIAL_BLOCK_RIGHT = 0xD8,
    PARTIAL_BLOCK_TOP = 0xDC,
    PARTIAL_BLOCK_BOTTOM = 0xE0,
    PARTIAL_TOP_LEFT_CORNER = 0xE4,
    PARTIAL_TOP_RIGHT_CORNER = 0xE8,
    PARTIAL_BOTTOM_RIGHT_CORNER = 0xEC,
    PARTIAL_BOTTOM_LEFT_CORNER = 0xF0,
    PARTIAL_CENTRE_BLOCK = 0xF4,
};

struct gmp_header
{
    char_type mFileType[4]; // 0x0
    s16 mVersion; // 0x4
};

struct DmapInfoList;

struct gmp_compressed_map_32
{
    EXPORT void ApplyDmapInfos(DmapInfoList* pMapSub);

    // 9.6f 0x4630B0
    gmp_compressed_map_32()
    {
        mpColumn = NULL;
        mBlock = NULL;
        mNumBlocks = 0;
        mColumnWords = 0;
    }

    u32 mBase[256][256]; // 0x0
    s32 mColumnWords; // 0x40000
    s32 mNumBlocks; // 0x40004
    u16** mpColumn; // 0x40008
    gmp_block_info* mBlock; // 0x4000C

    gmp_block_info* get_block(u32 block)
    {
        return &mBlock[block];
    }

    // 9.6f 0x42A830
    u32* get_base(s32 y, s32 x)
    {
        return &mBase[y][x];
    }
};

struct gmp_dmap_info
{
    // 9.6f 0x463080
    inline void set_data(u8 x, u8 y, u32 column_idx)
    {
        mX = x;
        mY = y;
        mColumnIdx = column_idx;
    }

    u32 mColumnIdx; // 0x0
    u8 mX; // 0x4
    u8 mY; // 0x5
    u16 mUnused6; // 0x6
};

struct DmapInfoList
{
    // 9.6f 0x4630A0
    DmapInfoList()
    {
        mMaxIdx = 0;
    }

    EXPORT void AddOrUpdateDmapInfo(u8 x, u8 y, u32 column_idx);

    gmp_dmap_info mDmapInfos[100]; // 0x0
    s32 mMaxIdx; // 0x320
};

class gmp_map_zone
{
  public:
    u8 mZoneType; // 0x0
    u8 mX; // 0x1
    u8 mY; // 0x2
    u8 mW; // 0x3
    u8 mH; // 0x4
    u8 mNameLength; // 0x5
    char_type mName[2]; // 0x6

    EXPORT bool IsZoneVisibleToAnyPlayer() const;
    EXPORT wchar_t* get_zone_str();

    // 9.6f 0x463020
    inline bool ContainsPoint(u8 x, u8 y) const
    {
        return x >= mX && y >= mY && x < mX + mW && y < mY + mH;
    }
};

struct gmp_zone_info
{
    u16 mCarDensity; // 0x0
    s16 mGoodcarRatio; // 0x2
    s16 mBadcarRatio; // 0x4
    s16 mPolicecarRatio; // 0x6
    s16 mGangcarRatio; // 0x8
    s16 mPedDensity; // 0xA
    s16 mMuggerRatio; // 0xC
    s16 mCarthiefRatio; // 0xE
    s16 mElvisRatio; // 0x10
    s16 mGangcharRatio; // 0x12
    s16 mPolicepedRatio; // 0x14
};

struct gmp_map_object
{
    u16 mX; // 0x0
    u16 mY; // 0x2
    Ang8 mRotation; // 0x4
    u8 mObjectType; // 0x5
};

struct gmp_map_light
{
    s32 mArgb; // 0x0
    s16 mX; // 0x4
    s16 mY; // 0x6
    s16 mZ; // 0x8
    s16 mRadius; // 0xA
    u8 mIntensity; // 0xC
    u8 mShape; // 0xD
    u8 mOnTime; // 0xE
    u8 mOffTime; // 0xF
};

struct gmp_tile_animation
{
    u16 mBase; // 0x0: The base tile of the animation. This marks the tile as an animating tile.
    u8 mFrameRate; // 0x2: The number of game frames that each frame of the animation is displayed for.
    u8 mRepeat; // 0x3: The number of times the animation will be played. 0 means play forever.
    u8 mAnimLength; // 0x4: The number of tiles in the animation
    u8 mUnused5; // 0x5
    // NOTE: Size set to 1 to shutup a warning
    u16 mTiles[1]; // 0x6: Size given by mAnimLength
};

struct gmp_map_slope
{
    EXPORT gmp_map_slope(s32 gradient_direction, s32 gradient_size, s32 gradient_level, Fix16 zpos_lower, Fix16 zpos_higher);

    gmp_map_slope()
    {
    }

    u8 mGradientDirection; // 0x0: the direction the slope level increases. E.g. "direction north" means it increases the level from south to north
    u8 mGradientSize; // 0x1: 1 = 45 degree; 2 = 26 degree; 8 = 7 degree slope
    u8 mGradientLevel; // 0x2: 0 = highest slope of the gradient, it increases as going lower
    u8 mPadding3; // 0x3
    Fix16 mZposLower; // 0x4: relative z coordinate from the lowest point of the slope
    Fix16 mZposHigher; // 0x8: relative z coordinate from the highest point of the slope
};

enum gmp_gradient_slope_direction // direction: low to high
{
    NO_GRADIENT_SLOPE_0 = 0,
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

enum gmp_arrow_direction
{
    UP_1 = 1,
    DOWN_2 = 2,
    LEFT_3 = 3,
    RIGHT_4 = 4,
};

namespace path_direction
{
enum
{
    up_1 = 1,
    down_2 = 2,
    right_3 = 3, // notice that right and left are swapped in comparison with gmp_arrow_direction
    left_4 = 4, // notice that right and left are swapped in comparison with gmp_arrow_direction
};
} // namespace path_direction

namespace road_direction
{
enum
{
    up_1 = 1,
    down_2 = 2,
    right_3 = 3, // notice that right and left are swapped in comparison with gmp_arrow_direction
    left_4 = 4, // notice that right and left are swapped in comparison with gmp_arrow_direction
};
} // namespace path_direction

enum gmp_arrow_type
{
    green_1 = 1,
    red_2 = 2,
    green_or_red_3 = 3,
};

EXTERN_GLOBAL_ARRAY(gmp_map_slope, gGmpSlopes, 64);

EXTERN_GLOBAL(Fix16, kGradientScaleSize1);

EXTERN_GLOBAL(Fix16, kGradientScaleSize2);

EXTERN_GLOBAL(Fix16, kGradientScaleSize8);

EXTERN_GLOBAL(Fix16, kFpZero_6F610C);
extern Fix16 kFpOne_6F6110;
EXTERN_GLOBAL(Ang16, kAng180_6F603E);
EXPORT Ang16 __stdcall ReturnAngleFromRoadDirection(s32& road_direction);

// Slope byte tests. 9.6f (inlining off) has no functions for these, so the original wrote them inline.
// Bits 0-1 are the ground type (0 air, 1 road, 2 pavement, 3 field), bits 2-7 the slope index (0-63).
#define GET_SLOPE_BITS(slope_byte) (static_cast<u8>((slope_byte) & 0xFCu)) // slope_byte with its first 2 bits cleared
#define GET_SLOPE_IDX(slope_byte) (static_cast<u8>((slope_byte) >> 2))     // 0 to 63
#define IS_AIR_TYPE(slope_byte) (((slope_byte) & 3) == 0)
#define IS_GRADIENT_SLOPE(slope_byte) (GET_SLOPE_BITS(slope_byte) > 0 && GET_SLOPE_BITS(slope_byte) < 0xB4u) // slope idx 1 to 44
#define GET_BLOCK_TYPE(slope_byte) (static_cast<u8>((slope_byte) & 3))
#define IS_SLOPE_IN_BLOCK_ABOVE(slope_byte) (GET_SLOPE_BITS(slope_byte) == 0xFC) // slope idx 63: the slope is in the block above
#define IS_PARTIAL_BLOCK(slope) ((slope) >= 0xD4 && (slope) <= 0xF4)
#define IS_TRIDIAGONAL_BLOCK(slope) ((slope) >= 0xC4 && (slope) <= 0xD0)

struct gmp_zone_list
{
    u16 mNumZones; // 0x0
    u16 mPad2; // 0x2
    gmp_map_zone* mZoneArray[2]; // 0x4
};

class Map_0x370
{
  public:
    STATIC_MEMBER(s32, sWestTileZ);
    STATIC_MEMBER(s32, sSouthTileZ);
    STATIC_MEMBER(s32, sSearchZ);
    STATIC_MEMBER(s32, sSearchY);
    STATIC_MEMBER(s32, sSearchX);
    STATIC_MEMBER(s32, sNorthTileZ);
    STATIC_MEMBER(s32, sEastTileZ);
    STATIC_MEMBER(gmp_block_info, sBlockInfo2);
    STATIC_MEMBER(gmp_block_info, sBlockInfo1);
    STATIC_MEMBER(gmp_block_info*, sBlockInfo0);
    EXPORT gmp_block_info* GetBlockOrNull(u8 x_coord, u8 y_coord, u8 z_coord) const;

    EXPORT gmp_map_zone* FindZoneByName(const char_type* pZoneName);

    EXPORT s32 FindZoneIdxByName(const char_type* pZoneName, u8 zone_name_len);

    EXPORT gmp_map_zone* GetNextZoneOfTypeRoundRobin(u8 zone_type);

    EXPORT gmp_map_zone* GetFirstZoneOfType(u8 zone_type);

    EXPORT gmp_map_zone* GetNearestZoneOfType(u8 xpos, u8 ypos, u8 zone_type);
    EXPORT gmp_map_zone* GetZoneOfTypeNearestCentre(u8 xpos, u8 ypos, u8 zone_type);

    EXPORT gmp_map_zone* GetZoneAtPosOfType(u8 zone_x, u8 zone_y, u8 zone_type);
    EXPORT gmp_map_zone* GetNavZoneAtPos(u8 zone_x, u8 zone_y);

    EXPORT gmp_map_zone* GetFirstZoneAtPos(u8 zone_x, u8 zone_y);

    EXPORT gmp_map_zone* GetNextZone();

    EXPORT gmp_zone_info* GetCurrentZoneInfo();

    EXPORT gmp_zone_info* GetZoneInfoAt(s32 zone_x, s32 zone_y);

    EXPORT void SetZoneInfo(s32 zone_idx,
                                            s16 car_density,
                                            s16 goodcar_ratio,
                                            s16 badcar_ratio,
                                            s16 policecar_ratio,
                                            s16 gangcar_ratio,
                                            s16 ped_density,
                                            s16 mugger_ratio,
                                            s16 carthief_ratio,
                                            s16 elvis_ratio,
                                            s16 gangchar_ratio,
                                            s16 policeped_ratio);

    EXPORT gmp_zone_info* GetZoneInfo(u16 zone_idx) const;

    EXPORT void SetZoneCarDensity(u16 zone_idx, s16 value);

    EXPORT void SetZoneGoodCarRatio(u16 zone_idx, s16 value);

    EXPORT void SetZoneBadCarRatio(u16 zone_idx, s16 value);

    EXPORT void SetZonePoliceCarRatio(u16 zone_idx, s16 value);

    EXPORT void SetZoneGangCarRatio(u16 zone_idx, s16 value);

    EXPORT void SetZonePedDensity(u16 zone_idx, s16 value);

    EXPORT void SetZoneMuggerRatio(u16 zone_idx, s16 value);

    EXPORT void SetZoneCarThiefRatio(u16 zone_idx, s16 value);

    EXPORT void SetZoneElvisRatio(u16 zone_idx, s16 value);

    EXPORT void SetZoneGangCharRatio(u16 zone_idx, s16 value);

    EXPORT void SetZonePolicePedRatio(u16 zone_idx, s16 value);

    EXPORT gmp_map_zone* GetZone(u16 zone_idx);

    EXPORT Gang* GetGangAtCoords(Fix16 x, Fix16 y);

    EXPORT void SpawnMapObjects();
    EXPORT void AllocZones();
    EXPORT void UpdateLights();

    EXPORT gmp_block_info* GetBlock(s32 x_coord, s32 y_coord, s32 z_coord) const;

    EXPORT gmp_block_info* GetEffectiveBlock(s32 x, s32 y, s32 z);
    EXPORT gmp_block_info* GetBlockClamped(s32 x_coord, s32 y_coord, s32 z_coord);

    EXPORT DWORD GetLeftEdgeSpec(Fix16 x_coord, Fix16 y_coord, Fix16 z_coord);

    EXPORT s32 GetTopEdgeSpec(Fix16 x_pos, Fix16 y_pos, Fix16 z_pos);
    EXPORT s32 GetBlockSpec(Fix16 x, Fix16 y, Fix16 z);

    // 9.6f 0x49EBE0, inlined in 10.5; defined in CarPhysics.cpp (ProcessGroundCollisionAndSurfaceType)
    inline u8 GetBlockSurfaceType(s32 x, s32 y, s32 z, u8* pGradientSize, u8* pGradientLevel);
    EXPORT bool TakeBlockedByTerrain();
    EXPORT bool TakeLowerBlockHasArrows();
    EXPORT bool IsMoveOntoSlopeTileBlocked(s32 x, s32 y, s32 z, s32 path_direction, u8* pSlopeZDelta, bool bReportStepUp);
    EXPORT bool RectHitsDiagonalWall(Fix16_Rect* pRect);
    EXPORT bool SpriteHitsDiagonalWall(s32 z_pos);
    EXPORT bool HasWallInArea(s32 x_min, s32 x_max, s32 y_min, s32 y_max, s32 z);
    EXPORT bool SpriteHitsBlockFacesInArea(s32 x_min, s32 x_max, s32 y_min, s32 y_max, s32 z);
    EXPORT bool IsTileBlockedForSprite(s32 regionLeft, s32 regionRight, s32 regionTop, s32 regionBottom, s32 tileX, s32 tileY, s32 zLevel);
    EXPORT bool IsMovementRegionBlocked(s32 tile_x, s32 tile_y, s32 tile_z, Sprite* pSprite, s16 collision_mask);
    EXPORT char_type CheckSlopeTopInSpriteGrid(Fix16 z_pos);
    EXPORT bool SlopeBordersFlatInSpriteGrid(Fix16 z_pos);
    EXPORT bool AllBlocksInRectHaveSlopeBits(Fix16_Rect* pRect, u8 slope_bits);
    EXPORT void FindNearbyBlockOfType(u8* pX, u8* pY, u8* pZ, char_type block_type);
    EXPORT static bool __stdcall IsSearchBlockOfType(char_type block_type);
    EXPORT bool CheckGreenArrowDirection(s32 direction, gmp_block_info* pBlock);
    EXPORT gmp_block_info* FindPavementBlockForCoord(s32 x, s32 y, s32& z);
    EXPORT gmp_block_info* FindHighestBlockForCoord(s32 x, s32 y, s32* found_z);
    EXPORT gmp_block_info* FindNonAirBlockAtOrBelowZ(s32 x, s32 y, s32& z);
    EXPORT Fix16 FindGroundZBelowCoord(Fix16 x_pos, Fix16 y_pos, Fix16 z_pos);
    EXPORT Fix16 FindGroundZAboveCoord(Fix16 x_pos, Fix16 y_pos, Fix16 z_pos);
    EXPORT Fix16 GetGroundZBelowCoord(Fix16 x, Fix16 y, Fix16 z);
    EXPORT Fix16 GetGroundZBelowCoordEx(Fix16 x, Fix16 y, Fix16 z, bool& bInsideSlope);
    EXPORT bool IsZOnGround(Fix16 x, Fix16 y, Fix16 z);
    EXPORT char_type GetSurfaceTypeBelowCoord(Fix16 x, Fix16 y, Fix16 z);
    EXPORT bool CheckZCollisionAtCoord(Fix16 x, Fix16 y, Fix16 z, Fix16 second_z);
    EXPORT bool CanPlaceOilOrMine(Fix16 x, Fix16 y, Fix16 z, Fix16 z_high, Fix16* found_z);
    EXPORT bool IsProbePathClear(Fix16 width, Fix16 height, Fix16 depth, Fix16 from_x, Fix16 from_y, Fix16 from_z, Fix16 to_x, Fix16 to_y, Fix16 to_z);
    EXPORT Fix16 FindGroundZForCoord(Fix16 x_pos, Fix16 y_pos);
    EXPORT u8 UpdateZFromSlopeAtCoord(Fix16 x_pos, Fix16 y_pos, Fix16& z_pos);
    EXPORT bool HasGreenArrowForPathDirection(gmp_block_info* pBlock, s32 direction, bool bDefault);
    EXPORT s32 GetArrowDirectionFromBlock(gmp_block_info* pBlock, bool bDontFlip);
    EXPORT s16 ClassifyArrowOnSlope(Fix16 x, Fix16 y, Fix16 z, s32 arrow_dir, bool bReverse);
    EXPORT Fix16 FloorPlusOne(Fix16 value);
    EXPORT gmp_block_info* FindRailwayAtCoord(s32 x, s32 y, s32& found_z);
    EXPORT gmp_block_info* FindRailwayBelowZAtCoord(s32 x, s32 y, s32& z);
    EXPORT Fix16 GetRailwayZBelowCoord(Fix16 x_pos, Fix16 y_pos, Fix16 z_pos);
    EXPORT Fix16 GetRailwayZCoordAtXY(Fix16 x, Fix16 y);
    EXPORT void SnapZToRailway(Fix16 x, Fix16 y, Fix16* pZ, bool bDontFlip, bool bReverse);
    EXPORT void MovePosAlongDirection(Fix16* pX, Fix16* pY, Fix16 dist, s32 direction);
    EXPORT Fix16 GetLateralOffsetInBlock(Fix16 x, Fix16 y, Ang16 angle);
    EXPORT Fix16 GetForwardOffsetInBlock(Fix16 x, Fix16 y, Ang16 angle);

    // The block under (x, y, z), or the one at z when the one under has slope index 63 (0xFC: its slope is in the block above, GMP format).
    // A ternary, not an if (also in GetRoadBlockAtOrNull_4E7190): its else block counts toward the 100+ blocks of AdvanceAlongRailway
    inline gmp_block_info* GetRoadBlockAt_4E6660(Fix16& x, Fix16& y, Fix16& z)
    {
        gmp_block_info* pBlock = GetBlock(x.ToInt(), y.ToInt(), (z - kFpOne_6F6110).ToInt());
        return IS_SLOPE_IN_BLOCK_ABOVE(pBlock->mSlopeType) ? GetBlock(x.ToInt(), y.ToInt(), z.ToInt()) : pBlock;
    }

    // As GetRoadBlockAt_4E6660, writing straight into pBlock (the original keeps pBlock in a register
    // through both lookups, which a returned temporary doesn't give)
    inline void SetRoadBlockAt_4E6660(gmp_block_info*& pBlock, Fix16& x, Fix16& y, Fix16& z)
    {
        pBlock = GetBlock(x.ToInt(), y.ToInt(), (z - kFpOne_6F6110).ToInt());
        if (IS_SLOPE_IN_BLOCK_ABOVE(pBlock->mSlopeType))
        {
            pBlock = GetBlock(x.ToInt(), y.ToInt(), z.ToInt());
        }
    }

    // As SetRoadBlockAt_4E6660, but copes with no block under (x, y, z)
    inline void SetRoadBlockAtOrNull_4E7190(gmp_block_info*& pBlock, Fix16& x, Fix16& y, Fix16& z)
    {
        pBlock = GetBlock(x.ToInt(), y.ToInt(), (z - kFpOne_6F6110).ToInt());
        if (pBlock && IS_SLOPE_IN_BLOCK_ABOVE(pBlock->mSlopeType))
        {
            pBlock = GetBlock(x.ToInt(), y.ToInt(), z.ToInt());
        }
    }

    // As GetRoadBlockAt_4E6660, but copes with no block under (x, y, z)
    inline gmp_block_info* GetRoadBlockAtOrNull_4E7190(Fix16& x, Fix16& y, Fix16& z)
    {
        gmp_block_info* pBlock = GetBlock(x.ToInt(), y.ToInt(), (z - kFpOne_6F6110).ToInt());
        return (pBlock && IS_SLOPE_IN_BLOCK_ABOVE(pBlock->mSlopeType)) ? GetBlock(x.ToInt(), y.ToInt(), z.ToInt()) : pBlock;
    }

    // Move up to one block along direction, using up dist
    inline void StepOneBlock_4E6660(Fix16* pX, Fix16* pY, Fix16& dist, s32 direction)
    {
        if (dist >= kFpOne_6F6110)
        {
            MovePosAlongDirection(pX, pY, kFpOne_6F6110, direction);
            dist -= kFpOne_6F6110;
        }
        else
        {
            MovePosAlongDirection(pX, pY, dist, direction);
            dist = kFpZero_6F610C;
        }
    }

    EXPORT s32 AdvanceAlongRailway(Fix16* pX, Fix16* pY, Fix16* pZ, Fix16 dist);
    EXPORT s32 AdvanceAlongRailwayChecked(Fix16* pX, Fix16* pY, Fix16* pZ, Fix16 dist);
    EXPORT bool FindFirstPavementCoord(u8* pX, char_type* pY);
    EXPORT bool CheckColumnHasSolidAbove(Fix16 x, Fix16 y, Fix16 z);
    EXPORT s32 AddBlock(gmp_block_info* pBlockInfo);
    EXPORT s32 CloneOriginalBlock(u32 read_block_idx);
    EXPORT s32 CloneOriginalColumn(u32 column_idx);
    EXPORT s32 CloneColumnExtendedToZ(u32 column_idx, s32 z);
    EXPORT s32 CloneColumnWithoutBlock(u32 column_idx, s32 z, char_type bDropAbove);
    EXPORT void ChangeBlock(s32 x, s32 y, s32 z, s32 info_type_to_set, u16 info_value);
    EXPORT void AddNewBlock(s32 x, s32 y, s32 z, gmp_block_info* pBlockData);
    EXPORT void RemoveBlock(s32 x_pos, s32 y_pos, s32 offset, char_type do_drop);
    EXPORT void LowerColumn(s32 x_pos, s32 y_pos);
    EXPORT void LowerLevel(s32 x_min, s32 x_max, s32 y_min, s32 y_max);
    EXPORT void OnModifiedMapDataLoaded(u32 column_bytes, u32 block_bytes, u32 obj_bytes);
    EXPORT void GetModifiedMapData(u16*** outColumnPtr,
                           u32* outColumnBytes,
                           gmp_block_info** outBlockPtr,
                           u32* outBlockBytes,
                           DmapInfoList** outSubObj,
                           s32* outSubObjBytes);

    EXPORT void do_process_loaded_zone_data();

    EXPORT void build_zone_list(u32 chunk_size);

    EXPORT void AddTileAnimations(s32 size);

    EXPORT void load_mobj(size_t len);

    EXPORT void load_lght(size_t size);

    EXPORT void load_zone(size_t size);

    EXPORT void load_anim(size_t size);

    EXPORT void load_dmap(u32 len);

    EXPORT void load_rgen();

    EXPORT void load_chunk(const char_type* pChunkType, size_t chunkLen);

    EXPORT void process_loaded_zone_data();

    EXPORT void LoadMap(const char_type* pGmpFileName);

    EXPORT Map_0x370();

    EXPORT ~Map_0x370();

    // 9.6f 0x4B9F40
    inline bool IsWaterBlockAt(s32 x, s32 y, s32 z)
    {
        gmp_block_info* pBlock = GetBlock(x, y, z);
        return pBlock && gGtx_0x106C->IsRemappedWaterTile(pBlock->mLid & 0x3FF);
    }

    // 9.6f inline 0x4634E0
    inline Fix16 get_grad_scale_from_size(Fix16& scale, u8 gradient_size)
    {
        switch (gradient_size)
        {
            case SIZE_1:
                scale = kGradientScaleSize1; //  16384
                break;
            case SIZE_2:
                scale = kGradientScaleSize2; //  8168
                break;
            case SIZE_8:
                scale = kGradientScaleSize8; //  2084
                break;
            default:
                scale = kFpZero_6F610C; //  0
                break;
        }
        return scale;
    }

    // 9.6f 0x42A850
    gmp_block_info* get_block(u8 x, u8 y, u8 z)
    {
        u32 col_idx = *mpDmap->get_base(y, x);
        gmp_col_info* pCol =
            reinterpret_cast<gmp_col_info*>(&mpDmap->mpColumn[col_idx]);
        if (z >= pCol->mHeight || z < pCol->mOffset)
        {
            return NULL;
        }
        return &mpDmap->mBlock[pCol->mBlockd[z - pCol->mOffset]];
    }

    bool IsBlockPavementType_42A8C0(u8 x, u8 y, u8 z)
    {
        gmp_block_info* pBlock = GetBlockOrNull(x, y, z);
        if (pBlock && GET_BLOCK_TYPE(pBlock->mSlopeType) == PAVEMENT)
        {
            return true;
        }
        return false;
    }

    inline bool IsBlockPavementTypeInlined_433530(u8 x, u8 y, u8 z)
    {
        gmp_block_info* pBlock = get_block(x, y, z);
        if (pBlock)
        {
            return GET_BLOCK_TYPE(pBlock->mSlopeType) == PAVEMENT;
        }
        return false;
    }

    // Like IsBlockPavementTypeInlined_433530 but via GetBlockOrNull (TrafficLight::Init)
    inline bool IsBlockPavementTypeAt_452980(u8 x, u8 y, u8 z)
    {
        gmp_block_info* pBlock = GetBlockOrNull(x, y, z);
        if (pBlock)
        {
            return GET_BLOCK_TYPE(pBlock->mSlopeType) == PAVEMENT;
        }
        return false;
    }

    inline bool IsBlockRoadType_42A8C0(u8 x, u8 y, u8 z)
    {
        gmp_block_info* pBlock = GetBlockOrNull(x, y, z);
        if (pBlock)
        {
            return GET_BLOCK_TYPE(pBlock->mSlopeType) == ROAD;
        }
        return false;
    }

    // 9.6f out of line 0x433470 (North), 0x4334A0 (East), 0x4334D0 (South), 0x433500 (West).
    inline bool IsBlockRoadTypeInlined_433470(u8 x, u8 y, u8 z)
    {
        gmp_block_info* pBlock = get_block(x, y, z);
        if (pBlock)
        {
            return GET_BLOCK_TYPE(pBlock->mSlopeType) == ROAD;
        }
        return false;
    }

    // 9.6f inline 0x420420
    inline u8 GetBlockTypeAtCoord(s32 xpos, s32 ypos, s32 zpos)
    {
        gmp_block_info* pBlock = GetBlock(xpos, ypos, zpos);
        if (pBlock)
        {
            return GET_BLOCK_TYPE(pBlock->mSlopeType);
        }
        return 0;
    }

    // 9.6f 0x433430
    inline bool IsSlopeB4ToD0At(s32 xpos, s32 ypos, s32 zpos)
    {
        gmp_block_info* pBlock = GetBlock(xpos, ypos, zpos);
        if (pBlock)
        {
            u32 slope_type = GET_SLOPE_BITS(pBlock->mSlopeType);
            if (slope_type >= DIAGONAL_WALL_UP_LEFT && slope_type <= TRIANGULAR_SIDES_DIAGONAL_DOWN_RIGHT)
            {
                return true;
            }
        }
        return false;
    }

    // 9.6f 0x466CF0
    inline bool IsGradientSlopeAt(s32 xpos, s32 ypos, s32 zpos)
    {
        gmp_block_info* pBlockAtPos = GetBlock(xpos, ypos, zpos);
        if (pBlockAtPos && IS_GRADIENT_SLOPE(pBlockAtPos->mSlopeType)
            && !IS_AIR_TYPE(pBlockAtPos->mSlopeType))
        {
            return true;
        }
        return false;
    }

    // 9.6f 0x492140
    inline bool HasBlockAnyArrows(s32 xpos, s32 ypos, s32 zpos)
    {
        gmp_block_info* pBlock = GetBlock(xpos, ypos, zpos);
        return pBlock && pBlock->mArrows;
    }

    // 9.6f 0x492130
    inline void ClearBlockedByTerrain()
    {
        mbBlockedByTerrain = false;
    }

    bool IsNorthOrSouthGradSlope_4634B0(gmp_block_info* pBlock) 
    {
        gmp_map_slope* gradient_slope = &gGmpSlopes[pBlock->mSlopeType >> 2];
        if (gradient_slope->mGradientDirection == NORTH_1 
            || gradient_slope->mGradientDirection == SOUTH_2)
        {
            return true;
        }
        return false;
    }

    // 9.6f 0x463480 (the name's 0x4634B0 is the north/south one)
    bool IsWestOrEastGradSlope_4634B0(gmp_block_info* pBlock) 
    {
        gmp_map_slope* gradient_slope = &gGmpSlopes[pBlock->mSlopeType >> 2];
        if (gradient_slope->mGradientDirection == WEST_3 
            || gradient_slope->mGradientDirection == EAST_4)
        {
            return true;
        }
        return false;
    }

    bool IsNorthBlockRoadType_433470(u8 x, u8 y, u8 z)
    {
        gmp_block_info* pBlock = GetBlockOrNull(x, y - 1, z);
        if (pBlock)
        {
            return GET_BLOCK_TYPE(pBlock->mSlopeType) == ROAD;
        }
        return false;
    }

    // 9.6f 0x4334A0
    bool IsEastBlockRoadType(u8 x, u8 y, u8 z)
    {
        gmp_block_info* pBlock = GetBlockOrNull(x + 1, y, z);
        if (pBlock)
        {
            return GET_BLOCK_TYPE(pBlock->mSlopeType) == ROAD;
        }
        return false;
    }

    // 9.6f 0x4334D0
    bool IsSouthBlockRoadType(u8 x, u8 y, u8 z)
    {
        gmp_block_info* pBlock = get_block(x, y + 1, z);
        if (pBlock)
        {
            return GET_BLOCK_TYPE(pBlock->mSlopeType) == ROAD;
        }
        return false;
    }

    // 9.6f 0x433500
    bool IsWestBlockRoadType(u8 x, u8 y, u8 z)
    {
        gmp_block_info* pBlock = get_block(x - 1, y, z);
        if (pBlock)
        {
            return GET_BLOCK_TYPE(pBlock->mSlopeType) == ROAD;
        }
        return false;
    }

    // 9.6f 0x48A350
    inline bool IsBlockNonAirType(u8 x, u8 y, u8 z)
    {
        gmp_block_info* pBlock = GetBlockOrNull(x, y, z);
        if (pBlock)
        {
            return GET_BLOCK_TYPE(pBlock->mSlopeType) != AIR;
        }
        return false;
    }

  public:
    gmp_compressed_map_32* mpDmap; // 0x0
    DmapInfoList mObj; // 0x4
    gmp_map_zone* mpZoneData; // 0x328
    gmp_zone_list* mpZones; // 0x32C
    u8* mpZoneArray; // 0x330
    gmp_zone_info* mpZoneInfo; // 0x334
    gmp_map_object* mpMapObjects; // 0x338
    gmp_map_light* mpLightData; // 0x33C
    gmp_tile_animation* mpTileAnimData; // 0x340
    s32 mMapObjectCount; // 0x344
    s32 mNumLights; // 0x348
    s32 mNumBlocks34C; // 0x34C
    s32 mNumBlocksExtra; // 0x350
    u32 mNumBlocks354; // 0x354: u32: OnModifiedMapDataLoaded matches only with these two unsigned
    s32 mColumnWords358; // 0x358
    s32 mColumnWordExtra; // 0x35C
    u32 mColumnWords360; // 0x360
    u16 mCurZoneIdx; // 0x364
    s16 mUnused366; // 0x366
    u8 mZoneType; // 0x368
    u8 mUnused369; // 0x369
    u8 mZoneX; // 0x36A
    u8 mZoneY; // 0x36B
    bool mbMatchPos; // 0x36C
    char_type mUnused36D; // 0x36D
    bool mbBlockedByTerrain; // 0x36E
    bool mbLowerBlockHasArrows; // 0x36F
};

EXTERN_GLOBAL(Map_0x370*, gMap_0x370);

EXTERN_GLOBAL(s32, gSpriteGrid_left);

EXTERN_GLOBAL(s32, gSpriteGrid_right);


static inline gmp_map_slope* get_slope_struct(u8& slope_byte)
{
    return &gGmpSlopes[GET_SLOPE_IDX(slope_byte)];
}
