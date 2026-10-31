// This TU's copy of the Fix16_Point length zero (see Fix16_Point.hpp)
#define FIX16_POINT_ZERO kFpZero_6F610C

#include "map_0x370.hpp"
#include "Fix16_Rect.hpp"
#include "Function.hpp"
#include "Car.hpp"
#include "Game.hpp"
#include "Gang.hpp"
#include "Globals.hpp"
#include "LightPool.hpp"
#include "Object_5C.hpp"
#include "SpriteGrid.hpp"
#include "RouteFinder.hpp"
#include "TileAnimManager.hpp"
#include "chunk.hpp"
#include "crt_stubs.hpp"
#include "enums.hpp"
#include "error.hpp"
#include "file.hpp"
#include "gtx_0x106C.hpp"
#include "memory.hpp"
#include "sprite.hpp"
#include "CollisionSoundQueue.hpp"
#include "block_face.hpp"
#include "car_surface_type_more.hpp"

DEFINE_GLOBAL(Map_0x370*, gMap_0x370, 0x6F6268);
DEFINE_STATIC_MEMBER(gmp_block_info*, Map_0x370, sBlockInfo0, 0x6F5EB0);
DEFINE_STATIC_MEMBER(gmp_block_info, Map_0x370, sBlockInfo1, 0x6F5F40);
DEFINE_STATIC_MEMBER(gmp_block_info, Map_0x370, sBlockInfo2, 0x6F6028);
// Used by MapRenderer.cpp. Defined here, not there: with the definition in MapRenderer.cpp VC6 loads
// them 32-bit where the original has a 16-bit mov (MapRenderer::DrawPartialBlockTop etc.).
DEFINE_GLOBAL(u16, gBlockLeft, 0x6F62F6);
DEFINE_GLOBAL(u16, gBlockRight, 0x6F63C6);
DEFINE_GLOBAL(gmp_map_slope*, gCurrentBlockSlope, 0x6F5EC8);
// Defined in sprite.cpp: with the definition in this TU VC6 reads the s16 mask with a 32-bit mov
// (it knows the padding is safe to read), while the original's `mov 0x6F6002,%cx; test %cx,face`
// needs an extern declaration (Map_0x370::SpriteHitsBlockFacesInArea, IsTileBlockedForSprite).
EXTERN_GLOBAL(s16, gFaceCollisionMask);
DEFINE_GLOBAL(s32, gSpriteGrid_right, 0x6F5B80);
DEFINE_GLOBAL(s32, gSpriteGrid_left, 0x6F5FD4);
DEFINE_STATIC_MEMBER(s32, Map_0x370, sEastTileZ, 0x6F620C);
DEFINE_STATIC_MEMBER(s32, Map_0x370, sWestTileZ, 0x6F5BA0);
DEFINE_STATIC_MEMBER(s32, Map_0x370, sSouthTileZ, 0x6F6248);
DEFINE_STATIC_MEMBER(s32, Map_0x370, sNorthTileZ, 0x6F5FAC);
DEFINE_GLOBAL_INIT(Fix16, kFpZero_6F610C, Fix16(0), 0x6F610C);
DEFINE_GLOBAL_INIT(Fix16, kFpEight_6F6130, Fix16(8), 0x6F6130);
DEFINE_GLOBAL_INIT(Fix16, kFpOneThirtySecond_6F601C, Fix16(0x200, 0), 0x6F601C);
DEFINE_GLOBAL_INIT(Fix16, kFpOneSixtyFourth_6F60C0, Fix16(0x100, 0), 0x6F60C0);
DEFINE_GLOBAL_INIT(Fix16, kGradientScaleSize1, Fix16(1), 0x6F5ED8);
DEFINE_GLOBAL_INIT(Fix16, kGradientScaleSize2, Fix16(0x1FE8, 0), 0x6F5FA8);
DEFINE_GLOBAL_INIT(Fix16, kGradientScaleSize8, Fix16(0x824, 0), 0x6F6214);
DEFINE_STATIC_MEMBER(s32, Map_0x370, sSearchX, 0x6F6164);
DEFINE_STATIC_MEMBER(s32, Map_0x370, sSearchY, 0x6F6148);
DEFINE_STATIC_MEMBER(s32, Map_0x370, sSearchZ, 0x6F613C);
DEFINE_GLOBAL_INIT(Fix16, kFpOneEighth_6F6050, Fix16(0x800, 0), 0x6F6050);
DEFINE_GLOBAL_INIT(Fix16, kFpQuarter_6F6180, Fix16(0x1000, 0), 0x6F6180);
DEFINE_GLOBAL_INIT(Fix16, kFpThreeEighths_6F60A0, Fix16(0x1800, 0), 0x6F60A0);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_6F5F18, Fix16(0x2000, 0), 0x6F5F18);
DEFINE_GLOBAL_INIT(Fix16, kFpFiveEighths_6F60B8, Fix16(0x2800, 0), 0x6F60B8);
DEFINE_GLOBAL_INIT(Fix16, kFpThreeQuarters_6F61D8, Fix16(0x3000, 0), 0x6F61D8);
DEFINE_GLOBAL_INIT(Fix16, kFpSevenEighths, Fix16(0x3800, 0), 0x6F6218);
DEFINE_GLOBAL_INIT(Fix16, kFp255_6F5B8C, Fix16(255), 0x6F5B8C);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpTen, Fix16(10), 0x6F6184);
DEFINE_GLOBAL_INIT(Fix16, kFpSeven_6F6128, Fix16(7), 0x6F6128);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_6F5FE0, Fix16(0.5f), 0x6F5FE0);

DEFINE_GLOBAL_INIT(Ang16, kAng180_6F603E, Ang16(720), 0x6F603E);


Fix16 kFpOne_6F6110 = Fix16(1);

DEFINE_GLOBAL_ARRAY_INIT(gmp_map_slope,
                         gGmpSlopes,
                         64,
                         0x6F5BA8,
                         gmp_map_slope(0, 0, 0, kFpZero_6F610C, kFpZero_6F610C) COMMA
                         gmp_map_slope(1, 2, 1, kFpZero_6F610C, kFpHalf_6F5F18) COMMA
                         gmp_map_slope(1, 2, 0, kFpHalf_6F5F18, kFpOne_6F6110) COMMA
                         gmp_map_slope(2, 2, 1, kFpZero_6F610C, kFpHalf_6F5F18) COMMA
                         gmp_map_slope(2, 2, 0, kFpHalf_6F5F18, kFpOne_6F6110) COMMA
                         gmp_map_slope(3, 2, 1, kFpZero_6F610C, kFpHalf_6F5F18) COMMA
                         gmp_map_slope(3, 2, 0, kFpHalf_6F5F18, kFpOne_6F6110) COMMA
                         gmp_map_slope(4, 2, 1, kFpZero_6F610C, kFpHalf_6F5F18) COMMA
                         gmp_map_slope(4, 2, 0, kFpHalf_6F5F18, kFpOne_6F6110) COMMA
                         gmp_map_slope(1, 8, 7, kFpZero_6F610C, kFpOneEighth_6F6050) COMMA
                         gmp_map_slope(1, 8, 6, kFpOneEighth_6F6050, kFpQuarter_6F6180) COMMA
                         gmp_map_slope(1, 8, 5, kFpQuarter_6F6180, kFpThreeEighths_6F60A0) COMMA
                         gmp_map_slope(1, 8, 4, kFpThreeEighths_6F60A0, kFpHalf_6F5F18) COMMA
                         gmp_map_slope(1, 8, 3, kFpHalf_6F5F18, kFpFiveEighths_6F60B8) COMMA
                         gmp_map_slope(1, 8, 2, kFpFiveEighths_6F60B8, kFpThreeQuarters_6F61D8) COMMA
                         gmp_map_slope(1, 8, 1, kFpThreeQuarters_6F61D8, kFpSevenEighths) COMMA
                         gmp_map_slope(1, 8, 0, kFpSevenEighths, kFpOne_6F6110) COMMA
                         gmp_map_slope(2, 8, 7, kFpZero_6F610C, kFpOneEighth_6F6050) COMMA
                         gmp_map_slope(2, 8, 6, kFpOneEighth_6F6050, kFpQuarter_6F6180) COMMA
                         gmp_map_slope(2, 8, 5, kFpQuarter_6F6180, kFpThreeEighths_6F60A0) COMMA
                         gmp_map_slope(2, 8, 4, kFpThreeEighths_6F60A0, kFpHalf_6F5F18) COMMA
                         gmp_map_slope(2, 8, 3, kFpHalf_6F5F18, kFpFiveEighths_6F60B8) COMMA
                         gmp_map_slope(2, 8, 2, kFpFiveEighths_6F60B8, kFpThreeQuarters_6F61D8) COMMA
                         gmp_map_slope(2, 8, 1, kFpThreeQuarters_6F61D8, kFpSevenEighths) COMMA
                         gmp_map_slope(2, 8, 0, kFpSevenEighths, kFpOne_6F6110) COMMA
                         gmp_map_slope(3, 8, 7, kFpZero_6F610C, kFpOneEighth_6F6050) COMMA
                         gmp_map_slope(3, 8, 6, kFpOneEighth_6F6050, kFpQuarter_6F6180) COMMA
                         gmp_map_slope(3, 8, 5, kFpQuarter_6F6180, kFpThreeEighths_6F60A0) COMMA
                         gmp_map_slope(3, 8, 4, kFpThreeEighths_6F60A0, kFpHalf_6F5F18) COMMA
                         gmp_map_slope(3, 8, 3, kFpHalf_6F5F18, kFpFiveEighths_6F60B8) COMMA
                         gmp_map_slope(3, 8, 2, kFpFiveEighths_6F60B8, kFpThreeQuarters_6F61D8) COMMA
                         gmp_map_slope(3, 8, 1, kFpThreeQuarters_6F61D8, kFpSevenEighths) COMMA
                         gmp_map_slope(3, 8, 0, kFpSevenEighths, kFpOne_6F6110) COMMA
                         gmp_map_slope(4, 8, 7, kFpZero_6F610C, kFpOneEighth_6F6050) COMMA
                         gmp_map_slope(4, 8, 6, kFpOneEighth_6F6050, kFpQuarter_6F6180) COMMA
                         gmp_map_slope(4, 8, 5, kFpQuarter_6F6180, kFpThreeEighths_6F60A0) COMMA
                         gmp_map_slope(4, 8, 4, kFpThreeEighths_6F60A0, kFpHalf_6F5F18) COMMA
                         gmp_map_slope(4, 8, 3, kFpHalf_6F5F18, kFpFiveEighths_6F60B8) COMMA
                         gmp_map_slope(4, 8, 2, kFpFiveEighths_6F60B8, kFpThreeQuarters_6F61D8) COMMA
                         gmp_map_slope(4, 8, 1, kFpThreeQuarters_6F61D8, kFpSevenEighths) COMMA
                         gmp_map_slope(4, 8, 0, kFpSevenEighths, kFpOne_6F6110) COMMA
                         gmp_map_slope(1, 1, 0, kFpZero_6F610C, kFpOne_6F6110) COMMA
                         gmp_map_slope(2, 1, 0, kFpZero_6F610C, kFpOne_6F6110) COMMA
                         gmp_map_slope(3, 1, 0, kFpZero_6F610C, kFpOne_6F6110) COMMA
                         gmp_map_slope(4, 1, 0, kFpZero_6F610C, kFpOne_6F6110) COMMA
                         gmp_map_slope(0, 0, 0, kFpZero_6F610C, kFpZero_6F610C) COMMA
                         gmp_map_slope(0, 0, 0, kFpZero_6F610C, kFpZero_6F610C) COMMA
                         gmp_map_slope(0, 0, 0, kFpZero_6F610C, kFpZero_6F610C) COMMA
                         gmp_map_slope(0, 0, 0, kFpZero_6F610C, kFpZero_6F610C) COMMA
                         gmp_map_slope(0, 0, 0, kFpZero_6F610C, kFpZero_6F610C) COMMA
                         gmp_map_slope(0, 0, 0, kFpZero_6F610C, kFpZero_6F610C) COMMA
                         gmp_map_slope(0, 0, 0, kFpZero_6F610C, kFpZero_6F610C) COMMA
                         gmp_map_slope(0, 0, 0, kFpZero_6F610C, kFpZero_6F610C) COMMA
                         gmp_map_slope(0, 0, 0, kFpZero_6F610C, kFpZero_6F610C) COMMA
                         gmp_map_slope(0, 0, 0, kFpZero_6F610C, kFpZero_6F610C) COMMA
                         gmp_map_slope(0, 0, 0, kFpZero_6F610C, kFpZero_6F610C) COMMA
                         gmp_map_slope(0, 0, 0, kFpZero_6F610C, kFpZero_6F610C) COMMA
                         gmp_map_slope(0, 0, 0, kFpZero_6F610C, kFpZero_6F610C) COMMA
                         gmp_map_slope(0, 0, 0, kFpZero_6F610C, kFpZero_6F610C) COMMA
                         gmp_map_slope(0, 0, 0, kFpZero_6F610C, kFpZero_6F610C) COMMA
                         gmp_map_slope(0, 0, 0, kFpZero_6F610C, kFpZero_6F610C) COMMA
                         gmp_map_slope(0, 0, 0, kFpZero_6F610C, kFpZero_6F610C) COMMA
                         gmp_map_slope(0, 0, 0, kFpZero_6F610C, kFpZero_6F610C) COMMA
                         gmp_map_slope(0, 0, 0, kFpZero_6F610C, kFpZero_6F610C));

static inline s32 clamp_z_from_column(s32 height, s32 offset, s32 z_pos)
{
    if (z_pos >= height)
    {
        return height - offset - 1; // highest block possible on the column
    }
    else
    {
        return z_pos - offset;
    }
}

static inline u16 get_tile_idx(u16& side_word)
{
    return side_word & block_face::tile_idx_mask;
}

static inline bool has_any_green_arrows(u8& arrow_data)
{
    return (arrow_data & block_arrow::green_mask) != 0;
}

static inline bool check_green_up(u8& arrow_data)
{
    return (arrow_data >> 2) & 1;
}

static inline bool check_green_down(u8& arrow_data)
{
    return (arrow_data >> 3) & 1;
}

static inline bool check_green_right(u8& arrow_data)
{
    return (arrow_data >> 1) & 1;
}

static inline bool check_green_left(u8& arrow_data)
{
    return arrow_data & 1;
}

static inline bool has_bottom(gmp_block_info* pBlock)
{
    return pBlock->mBottom != 0;
}

static inline bool has_top(gmp_block_info* pBlock)
{
    return pBlock->mTop != 0;
}

MATCH_FUNC(0x452980)
gmp_block_info* Map_0x370::GetBlockOrNull(u8 x_coord, u8 y_coord, u8 z_coord) const
{
    gmp_col_info* pColData =
        reinterpret_cast<gmp_col_info*>(&mpDmap->mpColumn[*mpDmap->get_base(y_coord, x_coord)]);
    if (z_coord >= pColData->mHeight || z_coord < pColData->mOffset)
    {
        return NULL;
    }
    else
    {
        return &mpDmap->mBlock[pColData->mBlockd[z_coord - pColData->mOffset]];
    }
}

MATCH_FUNC(0x4DEF40)
bool gmp_map_zone::IsZoneVisibleToAnyPlayer() const
{
    Fix16_Rect zoneBounds;
    zoneBounds.SetFromPosSize(mX, mY, mW, mH);
    return gGame_0x40->IsRectVisibleToAnyPlayer(&zoneBounds);
}

MATCH_FUNC(0x4DEFD0)
gmp_map_zone* Map_0x370::FindZoneByName(const char_type* pZoneName)
{
    u32 name_len = strlen(pZoneName);
    if (mpZoneData)
    {
        for (mCurZoneIdx = 0; mCurZoneIdx < mpZones->mNumZones; mCurZoneIdx++)
        {
            gmp_map_zone* pZone = GetZone(mCurZoneIdx);
            if (pZone->mNameLength == name_len && memcmp(pZone->mName, pZoneName, name_len) == 0)
            {
                return pZone;
            }
        }
    }
    return NULL;
}

MATCH_FUNC(0x4DF050)
s32 Map_0x370::FindZoneIdxByName(const char_type* pZoneName, u8 zone_name_len)
{
    if (!mpZoneData)
    {
        return 0;
    }

    for (mCurZoneIdx = 0; mCurZoneIdx < mpZones->mNumZones; mCurZoneIdx++)
    {
        gmp_map_zone* pZone = GetZone(mCurZoneIdx);
        if (pZone->mNameLength == zone_name_len && _strnicmp(pZone->mName, pZoneName, zone_name_len) == 0)
        {
            return mCurZoneIdx;
        }
    }
    return -1;
}

MATCH_FUNC(0x4DF0F0)
gmp_map_zone* Map_0x370::GetNextZoneOfTypeRoundRobin(u8 zone_type)
{
    static s16 sLastIdx_6F626C; // the turn, one for every zone type: a call for one type moves it on for the others too

    s16 zone_stack[40];

    if (mpZoneData)
    {
        u16 idx = 0;
        mbMatchPos = false;
        mZoneType = zone_type;
        mCurZoneIdx = 0;

        for (mCurZoneIdx = 0; mCurZoneIdx < mpZones->mNumZones; mCurZoneIdx++)
        {
            if (GetZone(mCurZoneIdx)->mZoneType == mZoneType)
            {
                zone_stack[idx++] = mCurZoneIdx;
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
            return GetZone(zone_stack[sLastIdx_6F626C]);
        }
    }

    return NULL;
}

MATCH_FUNC(0x4DF1D0)
gmp_map_zone* Map_0x370::GetFirstZoneOfType(u8 zone_type)
{
    if (mpZoneData)
    {
        mbMatchPos = false;
        mZoneType = zone_type;
        mCurZoneIdx = 0;

        for (mCurZoneIdx = 0; mCurZoneIdx < mpZones->mNumZones; mCurZoneIdx++)
        {
            gmp_map_zone* pZone = GetZone(mCurZoneIdx);
            if (pZone->mZoneType == mZoneType)
            {
                return pZone;
            }
        }
    }
    return NULL;
}

// https://decomp.me/scratch/MqQPJ
MATCH_FUNC(0x4DF240)
gmp_map_zone* Map_0x370::GetNearestZoneOfType(u8 xpos, u8 ypos, u8 zone_type)
{
    Fix16 nearest_dist = kFp255_6F5B8C;
    Fix16 second_dist = kFp255_6F5B8C;

    gmp_map_zone* pChosenZone = NULL;
    gmp_map_zone* pOtherZone = NULL;

    if (mpZoneData == NULL)
    {
        return NULL;
    }

    s32 zone_idx;

    for (zone_idx = 0; zone_idx < mpZones->mNumZones; ++zone_idx)
    {
        gmp_map_zone* pZone = Map_0x370::GetZone(zone_idx);

        if (pZone->mZoneType == zone_type
            && !pZone->IsZoneVisibleToAnyPlayer())
        {
            Fix16 dist;
            dist = Fix16::MaxAbsDistance(Fix16(pZone->mX), Fix16(pZone->mY), Fix16(xpos), Fix16(ypos));

            if (dist < nearest_dist)
            {
                if (dist > kFpTen)
                {
                    if (pOtherZone)
                    {
                        pChosenZone = pOtherZone;
                        second_dist = nearest_dist;
                    }
                    else
                    {
                        second_dist = dist;
                        pChosenZone = pZone;
                    }
                    nearest_dist = dist;
                    pOtherZone = pZone;
                }
            }
            else if (dist < second_dist)
            {
                second_dist = dist;
                pChosenZone = pZone;
            }
        }
    }
    if (pChosenZone)
    {
        return pChosenZone;
    }

    if (!pOtherZone)
    {
        for (zone_idx = 0; zone_idx < mpZones->mNumZones; zone_idx++)
        {
            gmp_map_zone* pZone2 = Map_0x370::GetZone(zone_idx);
            if (pZone2->mZoneType == zone_type)
            {
                pOtherZone = pZone2;
            }
        }
    }
    return pOtherZone;
}

MATCH_FUNC(0x4DF3E0)
gmp_map_zone* Map_0x370::GetZoneOfTypeNearestCentre(u8 xpos, u8 ypos, u8 zone_type)
{
    // pBestZone declared before best_dist (prologue order) and ternary abs values (the compare uses the
    // dist copy, not diff_y)
    gmp_map_zone* pBestZone = NULL;
    Fix16 best_dist = kFp255_6F5B8C;

    if (mpZoneData == NULL)
    {
        return NULL;
    }

    for (s32 zone_idx = 0; zone_idx < mpZones->mNumZones; zone_idx++)
    {
        gmp_map_zone* pZone = Map_0x370::GetZone(zone_idx);
        if (pZone->mZoneType == zone_type)
        {
            s32 diff_x = (xpos << 14) - ((pZone->mX << 14) + ((pZone->mW >> 1) << 14));
            s32 diff_y = (ypos << 14) - ((pZone->mY << 14) + ((pZone->mH >> 1) << 14));
            s32 abs_y = diff_y > 0 ? diff_y : -diff_y;
            s32 abs_x = diff_x > 0 ? diff_x : -diff_x;
            Fix16 dist(abs_y, 0);
            if (abs_x > dist.mValue)
            {
                dist.mValue = abs_x;
            }
            if (dist < best_dist)
            {
                best_dist = dist;
                pBestZone = pZone;
            }
        }
    }
    return pBestZone;
}

MATCH_FUNC(0x4DF4D0)
gmp_map_zone* Map_0x370::GetZoneAtPosOfType(u8 zone_x, u8 zone_y, u8 zone_type)
{
    if (mpZoneData)
    {
        mZoneType = zone_type;
        mZoneX = zone_x;
        mZoneY = zone_y;
        mbMatchPos = true;

        for (mCurZoneIdx = 0; mCurZoneIdx < mpZones->mNumZones; mCurZoneIdx++)
        {
            gmp_map_zone* pZone = GetZone(mCurZoneIdx);
            if (pZone->mZoneType == mZoneType && pZone->ContainsPoint(mZoneX, mZoneY))
            {
                return pZone;
            }
        }
    }
    return NULL;
}

MATCH_FUNC(0x4DF5C0)
gmp_map_zone* Map_0x370::GetNavZoneAtPos(u8 zone_x, u8 zone_y)
{
    if (mpZoneData)
    {
        mZoneX = zone_x;
        mZoneY = zone_y;

        for (mCurZoneIdx = 0; mCurZoneIdx < mpZones->mNumZones; mCurZoneIdx++)
        {
            gmp_map_zone* pZone = GetZone(mCurZoneIdx);
            if ((pZone->mZoneType == Information_10 
                || pZone->mZoneType == Navigation_1 
                || pZone->mZoneType == Local_Navigation_15) &&
                pZone->ContainsPoint(mZoneX, mZoneY))
            {
                return pZone;
            }
        }
    }
    return NULL;
}

MATCH_FUNC(0x4DF6A0)
gmp_map_zone* Map_0x370::GetFirstZoneAtPos(u8 zone_x, u8 zone_y)
{
    gmp_map_zone* pZone;

    if (this->mpZoneData)
    {
        this->mZoneX = zone_x;
        this->mZoneY = zone_y;
        this->mbMatchPos = true;

        for (mCurZoneIdx = 0; mCurZoneIdx < mpZones->mNumZones; mCurZoneIdx++)
        {
            pZone = Map_0x370::GetZone(this->mCurZoneIdx);
            if (pZone->ContainsPoint(this->mZoneX, this->mZoneY))
            {
                return pZone;
            }
        }
    }
    return NULL;
}

MATCH_FUNC(0x4DF770)
gmp_map_zone* Map_0x370::GetNextZone()
{
    gmp_map_zone* pZoneIter;

    if (this->mpZoneData)
    {
        for (++this->mCurZoneIdx; mCurZoneIdx < mpZones->mNumZones; ++this->mCurZoneIdx)
        {
            if (mCurZoneIdx >= mpZones->mNumZones)
            {
                return NULL;
            }
            pZoneIter = Map_0x370::GetZone(mCurZoneIdx);
            if (pZoneIter->mZoneType == this->mZoneType &&
                (!this->mbMatchPos || pZoneIter->ContainsPoint(mZoneX, mZoneY)))
            {
                return pZoneIter;
            }
        }
    }
    return NULL;
}

MATCH_FUNC(0x4DF840)
gmp_zone_info* Map_0x370::GetCurrentZoneInfo()
{
    if (mpZoneData)
    {
        u8 zone_idx = mpZoneArray[mCurZoneIdx];
        if (zone_idx != 0)
        {
            return &mpZoneInfo[zone_idx];
        }
    }
    return NULL;
}

MATCH_FUNC(0x4DF890)
gmp_zone_info* Map_0x370::GetZoneInfoAt(s32 zone_x, s32 zone_y)
{
    if (GetNavZoneAtPos(zone_x, zone_y))
    {
        return GetCurrentZoneInfo();
    }
    else
    {
        return mpZoneInfo;
    }
}

MATCH_FUNC(0x4DF8C0)
void Map_0x370::SetZoneInfo(s32 zone_idx,
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
                                            s16 policeped_ratio)
{
    GetZone(zone_idx);
    gmp_zone_info* pZoneInfo = &mpZoneInfo[mpZoneArray[zone_idx]];
    if (car_density != -1)
    {
        pZoneInfo->mCarDensity = car_density;
    }

    if (goodcar_ratio != -1)
    {
        pZoneInfo->mGoodcarRatio = goodcar_ratio;
    }

    if (badcar_ratio != -1)
    {
        pZoneInfo->mBadcarRatio = badcar_ratio;
    }

    if (policecar_ratio != -1)
    {
        pZoneInfo->mPolicecarRatio = policecar_ratio;
    }

    if (gangcar_ratio != -1)
    {
        pZoneInfo->mGangcarRatio = gangcar_ratio;
    }

    if (ped_density != -1)
    {
        pZoneInfo->mPedDensity = ped_density;
    }

    if (mugger_ratio != -1)
    {
        pZoneInfo->mMuggerRatio = mugger_ratio;
    }

    if (carthief_ratio != -1)
    {
        pZoneInfo->mCarthiefRatio = carthief_ratio;
    }

    if (elvis_ratio != -1)
    {
        pZoneInfo->mElvisRatio = elvis_ratio;
    }

    if (gangchar_ratio != -1)
    {
        pZoneInfo->mGangcharRatio = gangchar_ratio;
    }

    if (policeped_ratio != -1)
    {
        pZoneInfo->mPolicepedRatio = policeped_ratio;
    }
}

MATCH_FUNC(0x4DF9A0)
gmp_zone_info* Map_0x370::GetZoneInfo(u16 zone_idx) const
{
    return &mpZoneInfo[mpZoneArray[zone_idx]];
}

MATCH_FUNC(0x4DF9D0)
void Map_0x370::SetZoneCarDensity(u16 zone_idx, s16 value)
{
    GetZoneInfo(zone_idx)->mCarDensity = value;
}

MATCH_FUNC(0x4DF9F0)
void Map_0x370::SetZoneGoodCarRatio(u16 zone_idx, s16 value)
{
    GetZoneInfo(zone_idx)->mGoodcarRatio = value;
}

MATCH_FUNC(0x4DFA10)
void Map_0x370::SetZoneBadCarRatio(u16 zone_idx, s16 value)
{
    GetZoneInfo(zone_idx)->mBadcarRatio = value;
}

MATCH_FUNC(0x4DFA30)
void Map_0x370::SetZonePoliceCarRatio(u16 zone_idx, s16 value)
{
    GetZoneInfo(zone_idx)->mPolicecarRatio = value;
}

MATCH_FUNC(0x4DFA50)
void Map_0x370::SetZoneGangCarRatio(u16 zone_idx, s16 value)
{
    GetZoneInfo(zone_idx)->mGangcarRatio = value;
}

MATCH_FUNC(0x4DFA70)
void Map_0x370::SetZonePedDensity(u16 zone_idx, s16 value)
{
    GetZoneInfo(zone_idx)->mPedDensity = value;
}

MATCH_FUNC(0x4DFA90)
void Map_0x370::SetZoneMuggerRatio(u16 zone_idx, s16 value)
{
    GetZoneInfo(zone_idx)->mMuggerRatio = value;
}

MATCH_FUNC(0x4DFAB0)
void Map_0x370::SetZoneCarThiefRatio(u16 zone_idx, s16 value)
{
    GetZoneInfo(zone_idx)->mCarthiefRatio = value;
}

MATCH_FUNC(0x4DFAD0)
void Map_0x370::SetZoneElvisRatio(u16 zone_idx, s16 value)
{
    GetZoneInfo(zone_idx)->mElvisRatio = value;
}

MATCH_FUNC(0x4DFAF0)
void Map_0x370::SetZoneGangCharRatio(u16 zone_idx, s16 value)
{
    GetZoneInfo(zone_idx)->mGangcharRatio = value;
}

MATCH_FUNC(0x4DFB10)
void Map_0x370::SetZonePolicePedRatio(u16 zone_idx, s16 value)
{
    GetZoneInfo(zone_idx)->mPolicepedRatio = value;
}

MATCH_FUNC(0x4DFB30)
gmp_map_zone* Map_0x370::GetZone(u16 zone_idx)
{
    return mpZones->mZoneArray[zone_idx];
}

MATCH_FUNC(0x4DFB50)
Gang* Map_0x370::GetGangAtCoords(Fix16 x, Fix16 y)
{
    gmp_map_zone* pZone = GetZoneAtPosOfType(x.ToInt(), y.ToInt(), Gang_14);
    if (pZone)
    {
        return gGangPool_CA8->FindGangByName(pZone->mName);
    }
    else
    {
        return NULL;
    }
}

MATCH_FUNC(0x4DFB90)
void Map_0x370::SpawnMapObjects()
{
    gmp_map_object* pMapObjects = mpMapObjects;
    for (u32 i = 0; i < static_cast<u32>(this->mMapObjectCount); i++, ++pMapObjects)
    {
        Fix16 x_pos = Fix16::ctor(pMapObjects->mX);
        Fix16 y_pos = Fix16::ctor(pMapObjects->mY);

        Ang16 rotation = pMapObjects->mRotation.convert_to_ang16();
        rotation.Normalize();

        gObject_5C->NewPhysicsObj(pMapObjects->mObjectType,
                                                x_pos,
                                                y_pos,
                                                Map_0x370::FindGroundZForCoord(x_pos, y_pos),
                                                rotation);
    }
}

MATCH_FUNC(0x4DFCA0)
void Map_0x370::AllocZones()
{
    gmp_map_zone* pZoneIter = GetFirstZoneOfType(Gang_14);
    while (pZoneIter)
    {
        gGangPool_CA8->AddGangForMapZone(pZoneIter);
        pZoneIter = GetNextZone();
    }
}

MATCH_FUNC(0x4DFCD0)
void Map_0x370::UpdateLights()
{
    if (mNumLights == 0)
    {
        return;
    }

    gmp_map_light* pMapLight = &mpLightData[0];

    for (u32 light_idx = 0; light_idx < static_cast<u32>(mNumLights); light_idx++, ++pMapLight)
    {
        Fix16 xpos = Fix16::ctor(pMapLight->mX);
        Fix16 ypos = Fix16::ctor(pMapLight->mY);
        Fix16 zpos = Fix16::ctor(pMapLight->mZ);
        Fix16 radius = Fix16::ctor(pMapLight->mRadius);

        if (radius == kFpEight_6F6130)
        {
            radius -= kFpOneThirtySecond_6F601C;
        }

        Light* pLight =
            gLight_1D4CC->Init_469010(xpos, ypos, zpos, pMapLight->mArgb, radius, pMapLight->mIntensity);

        if (pMapLight->mOnTime)
        {
            gLight_1D4CC->SetFlashing(pLight, pMapLight->mOnTime, pMapLight->mOffTime, pMapLight->mShape);
        }
    }
}

MATCH_FUNC(0x4DFE10)
gmp_block_info* Map_0x370::GetBlock(s32 x_coord, s32 y_coord, s32 z_coord) const
{
    gmp_col_info* pCol =
        reinterpret_cast<gmp_col_info*>(&mpDmap->mpColumn[*mpDmap->get_base(y_coord, x_coord)]);
    if (z_coord < pCol->mHeight)
    {
        s32 offset = pCol->mOffset;
        if (z_coord >= offset)
        {
            return &mpDmap->mBlock[pCol->mBlockd[z_coord - offset]];
        }
    }
    return NULL;
}

MATCH_FUNC(0x4DFE60)
gmp_block_info* Map_0x370::GetEffectiveBlock(s32 x, s32 y, s32 z)
{
    // If it's a partial block, get a null/empty block. 
    // If it's a tridiagonal block (four-sided or tri-sided), replace it by a flat solid block
    // Otherwise, return the original block. It's ideal for checking collisions
    gmp_col_info* pColumn = reinterpret_cast<gmp_col_info*>(&mpDmap->mpColumn[*mpDmap->get_base(y, x)]);
    if (z < pColumn->mHeight)
    {
        if (z >= pColumn->mOffset)
        {
            gmp_block_info* v7_block = &mpDmap->mBlock[pColumn->mBlockd[z - pColumn->mOffset]];
            s32 slope_type = GET_SLOPE_BITS(v7_block->mSlopeType);

            if (!IS_PARTIAL_BLOCK(slope_type))
            {
                if (IS_TRIDIAGONAL_BLOCK(slope_type))
                {
                    return &sBlockInfo2; // solid block
                }
                return v7_block;
            }
        }
    }
    return NULL;
}

MATCH_FUNC(0x4DFEE0)
gmp_block_info* Map_0x370::GetBlockClamped(s32 x_coord, s32 y_coord, s32 z_coord)
{
    if (x_coord < 0)
    {
        x_coord = 0;
    }
    else
    {
        if (x_coord > 255)
        {
            x_coord = 255;
        }
    }

    if (y_coord < 0)
    {
        y_coord = 0;
    }
    else
    {
        if (y_coord > 255)
        {
            y_coord = 255;
        }
    }

    gmp_compressed_map_32* pDmap = mpDmap;
    gmp_col_info* pColInfo = reinterpret_cast<gmp_col_info*>(&pDmap->mpColumn[*pDmap->get_base(y_coord, x_coord)]);

    if (z_coord < pColInfo->mHeight)
    {
        s32 offset = pColInfo->mOffset;
        if (z_coord >= offset)
        {
            return &pDmap->mBlock[pColInfo->mBlockd[z_coord - offset]];
        }
    }
    return NULL;
}

MATCH_FUNC(0x4DFF60)
DWORD Map_0x370::GetLeftEdgeSpec(Fix16 x_coord, Fix16 y_coord, Fix16 z_coord)
{
    gmp_block_info* pBlock1 = GetBlock((x_coord - kFpOne_6F6110).ToInt(), y_coord.ToInt(), z_coord.ToInt());
    if (pBlock1 && pBlock1->mRight != 0)
    {
        return gGtx_0x106C->GetSpecIgnoringRoadJunction(get_tile_idx(pBlock1->mRight));
    }

    gmp_block_info* pBlock2 = GetBlock(x_coord.ToInt(), y_coord.ToInt(), z_coord.ToInt());
    if (pBlock2 && pBlock2->mLeft != 0)
    {
        return gGtx_0x106C->GetSpecIgnoringRoadJunction(get_tile_idx(pBlock2->mLeft));
    }

    return 0;
}

MATCH_FUNC(0x4E0000)
s32 Map_0x370::GetTopEdgeSpec(Fix16 x_pos, Fix16 y_pos, Fix16 z_pos)
{
    gmp_block_info* pBlockAtPos = Map_0x370::GetBlock(x_pos.ToInt(), (y_pos - kFpOne_6F6110).ToInt(), z_pos.ToInt());
    if (pBlockAtPos && has_bottom(pBlockAtPos))
    {
        return gGtx_0x106C->GetSpecIgnoringRoadJunction(get_tile_idx(pBlockAtPos->mBottom));
    }
    else
    {
        gmp_block_info* pBlock = Map_0x370::GetBlock(x_pos.ToInt(), y_pos.ToInt(), z_pos.ToInt());
        if (pBlock && has_top(pBlock))
        {
            return gGtx_0x106C->GetSpecIgnoringRoadJunction(get_tile_idx(pBlock->mTop));
        }
        else
        {
            return 0;
        }
    }
}

MATCH_FUNC(0x4E00A0)
s32 Map_0x370::GetBlockSpec(Fix16 x, Fix16 y, Fix16 z)
{
    if (z >= kFpZero_6F610C)
    {
        if (z < kFpEight_6F6130)
        {
            gmp_block_info* pBlock = gMap_0x370->GetBlock(x.ToInt(), y.ToInt(), z.ToInt());
            if (pBlock)
            {
                u16 lid = pBlock->mLid;
                if (lid)
                {
                    return gGtx_0x106C->GetSpecIgnoringRoadJunction(get_tile_idx(lid));
                }
            }
        }
    }
    return 0;
}

MATCH_FUNC(0x4E0110)
bool Map_0x370::TakeBlockedByTerrain()
{
    bool bBlocked = mbBlockedByTerrain;
    mbBlockedByTerrain = false;
    return bBlocked;
}

MATCH_FUNC(0x4E0120)
bool Map_0x370::TakeLowerBlockHasArrows()
{
    bool bHasArrows = mbLowerBlockHasArrows;
    mbLowerBlockHasArrows = false;
    return bHasArrows;
}

// https://decomp.me/scratch/RMgzo
// Returns true when moving from block (x, y, z) one step in path_direction is blocked, false when the move is allowed.
// 0x400 in a block face word is the wall bit.
// mbBlockedByTerrain is set when the move fails because there is no usable ground ahead (a missing block,
// a slope that doesn't join up, a drop), rather than because of a wall. If the block below the target is an air
// block with arrows, the drop is allowed and mbLowerBlockHasArrows is set.
// *pSlopeZDelta gets 1 when the move steps up onto the next z level (only written if bReportStepUp) and
// 0xFF (-1) when it steps down onto a slope one level below.
// Inside the switch a `break` means "blocked": it leaves the switch for the final `return true`.
// (Matching needs these as breaks: a then-branch `return true` gets its own inline epilogue.)
// Exception: the "blocked by terrain" exit after the below-block pBaseSlope check is a `return true` in the
// up, down and right cases and a `break` in the left case. That decides which identical tails VC6 cross-jumps
// (the up case jumps into the down case's below-block code, the right case keeps its own third lookup).
// The up and down cases step y in place (`y--` / `y++`) before the shared lookups: that keeps y referenced in the
// below-block lookups (priority 98 vs z 76, x 68), which puts y in edi and z in ebp like the original. The right
// and left cases keep `x + 1` / `x - 1` expressions (an in-place `x++` lifts x above z: 484).
// The base slope is read through pNext, not pBlock: a separate variable keeps the first lookup's result in eax.
// The below-block gradient tests are `== false`: a plain `!` changes which tails VC6 cross-jumps.
MATCH_FUNC(0x4E0130)
bool Map_0x370::IsMoveOntoSlopeTileBlocked(s32 x, s32 y, s32 z, s32 path_direction, u8* pSlopeZDelta, bool bReportStepUp)
{
    gmp_block_info* pBlock;
    gmp_map_slope* pSlope = NULL;
    gmp_map_slope* pBaseSlope = NULL;
    gmp_block_info* pNext;
    gmp_block_info* pBelow;
    gmp_map_slope* pNextSlope;

    mbBlockedByTerrain = false;

    pNext = GetEffectiveBlock(x, y, z);
    pBlock = pNext;
    if (pNext)
    {
        pBaseSlope = get_slope_struct(pNext->mSlopeType);
    }
    else if (z > 0)
    {
        pNext = GetEffectiveBlock(x, y, z - 1);
        if (pNext)
        {
            pBaseSlope = get_slope_struct(pNext->mSlopeType);
        }
    }

    switch (path_direction)
    {
        case path_direction::up_1:
            if (pBlock)
            {
                if (y == 0)
                {
                    break;
                }

                pSlope = get_slope_struct(pBlock->mSlopeType);
                if (pSlope)
                {
                    if (pSlope->mGradientDirection != NORTH_1)
                    {
                        if (pSlope->mGradientDirection > SOUTH_2 && pSlope->mGradientDirection <= EAST_4)
                        {
                            if (y <= 0)
                            {
                                mbBlockedByTerrain = true;
                                break;
                            }
                            pNext = GetEffectiveBlock(x, y - 1, z);
                            if (!pNext)
                            {
                                mbBlockedByTerrain = true;
                                break;
                            }
                            if (IS_GRADIENT_SLOPE(pNext->mSlopeType) && !IS_AIR_TYPE(pNext->mSlopeType))
                            {
                                pNextSlope = get_slope_struct(pNext->mSlopeType);
                                if (pNextSlope->mGradientLevel != pSlope->mGradientLevel &&
                                    pSlope->mGradientDirection != pNextSlope->mGradientDirection)
                                {
                                    mbBlockedByTerrain = true;
                                    return true;
                                }
                            }
                        }
                        else if (pBlock->mTop & block_face::wall)
                        {
                            return true;
                        }
                    }
                    else if (pSlope->mGradientLevel == 0)
                    {
                        pNext = GetEffectiveBlock(x, y - 1, z + 1);
                        if (pNext && (pNext->mBottom & block_face::wall))
                        {
                            break;
                        }
                        if (GetEffectiveBlock(x, y - 1, z + 1))
                        {
                            if (bReportStepUp)
                            {
                                *pSlopeZDelta = 1;
                            }
                            return false;
                        }
                        pNext = GetEffectiveBlock(x, y - 1, z);
                        if (!pNext)
                        {
                            mbBlockedByTerrain = true;
                            break;
                        }
                        if (IS_AIR_TYPE(pNext->mSlopeType))
                        {
                            mbBlockedByTerrain = true;
                            return true;
                        }
                        if (bReportStepUp)
                        {
                            *pSlopeZDelta = 1;
                        }
                        return false;
                    }

                    if (pBlock->mTop & block_face::wall)
                    {
                        break;
                    }
                }
            }

            y--;
            pNext = GetEffectiveBlock(x, y, z);
            if (pNext)
            {
                if (pNext->mBottom & block_face::wall)
                {
                    break;
                }
                if (IS_GRADIENT_SLOPE(pNext->mSlopeType) && !IS_AIR_TYPE(pNext->mSlopeType))
                {
                    pNextSlope = get_slope_struct(pNext->mSlopeType);
                    switch (pNextSlope->mGradientDirection)
                    {
                        case NORTH_1:
                        case SOUTH_2:
                            return false;
                        case WEST_3:
                        case EAST_4:
                            if (pSlope)
                            {
                                if (pNextSlope->mGradientLevel == pSlope->mGradientLevel ||
                                    pSlope->mGradientDirection == pNextSlope->mGradientDirection)
                                {
                                    return false;
                                }
                                return true;
                            }
                            else
                            {
                                return true;
                            }
                        default:
                            return true;
                    }
                }
            }

            if (z > 0)
            {
                pBelow = GetEffectiveBlock(x, y, z - 1);
                if (pBelow && !IS_AIR_TYPE(pBelow->mSlopeType))
                {
                    if (IS_GRADIENT_SLOPE(pBelow->mSlopeType) == false)
                    {
                        return false;
                    }
                    pNextSlope = get_slope_struct(pBelow->mSlopeType);
                    if (pNextSlope->mGradientDirection > NO_GRADIENT_SLOPE_0 && pNextSlope->mGradientDirection <= SOUTH_2)
                    {
                        *pSlopeZDelta = 0xFF;
                        return false;
                    }
                    if (pBaseSlope && pBaseSlope->mGradientDirection == pNextSlope->mGradientDirection)
                    {
                        return false;
                    }
                    mbBlockedByTerrain = true;
                    return true;
                }
            }
            else
            {
                pBelow = NULL;
            }
            mbBlockedByTerrain = true;
            if (pBelow && pBelow->mArrows)
            {
                mbLowerBlockHasArrows = true;
                return false;
            }
            return true;

        case path_direction::down_2:
            if (pBlock)
            {
                if (y == 0xFF)
                {
                    break;
                }

                pSlope = get_slope_struct(pBlock->mSlopeType);
                if (pSlope)
                {
                    if (pSlope->mGradientDirection != SOUTH_2)
                    {
                        if (pSlope->mGradientDirection > SOUTH_2 && pSlope->mGradientDirection <= EAST_4)
                        {
                            pNext = GetEffectiveBlock(x, y + 1, z);
                            if (!pNext)
                            {
                                mbBlockedByTerrain = true;
                                break;
                            }
                            if (IS_GRADIENT_SLOPE(pNext->mSlopeType) && !IS_AIR_TYPE(pNext->mSlopeType))
                            {
                                pNextSlope = get_slope_struct(pNext->mSlopeType);
                                if (pNextSlope->mGradientLevel != pSlope->mGradientLevel &&
                                    pSlope->mGradientDirection != pNextSlope->mGradientDirection)
                                {
                                    mbBlockedByTerrain = true;
                                    return true;
                                }
                            }
                        }
                        else if (pBlock->mBottom & block_face::wall)
                        {
                            return true;
                        }
                    }
                    else if (pSlope->mGradientLevel == 0)
                    {
                        pNext = GetEffectiveBlock(x, y + 1, z + 1);
                        if (pNext && (pNext->mTop & block_face::wall))
                        {
                            break;
                        }
                        if (GetEffectiveBlock(x, y + 1, z + 1))
                        {
                            if (bReportStepUp)
                            {
                                *pSlopeZDelta = 1;
                            }
                            return false;
                        }
                        pNext = GetEffectiveBlock(x, y + 1, z);
                        if (!pNext)
                        {
                            mbBlockedByTerrain = true;
                            break;
                        }
                        if (IS_AIR_TYPE(pNext->mSlopeType))
                        {
                            mbBlockedByTerrain = true;
                            break;
                        }
                        if (bReportStepUp)
                        {
                            *pSlopeZDelta = 1;
                        }
                        return false;
                    }
                }

                if (pBlock->mBottom & block_face::wall)
                {
                    break;
                }
            }

            y++;
            pNext = GetEffectiveBlock(x, y, z);
            if (pNext)
            {
                if (pNext->mTop & block_face::wall)
                {
                    break;
                }
                if (IS_GRADIENT_SLOPE(pNext->mSlopeType) && !IS_AIR_TYPE(pNext->mSlopeType))
                {
                    pNextSlope = get_slope_struct(pNext->mSlopeType);
                    switch (pNextSlope->mGradientDirection)
                    {
                        case NORTH_1:
                        case SOUTH_2:
                            return false;
                        case WEST_3:
                        case EAST_4:
                            if (pSlope)
                            {
                                if (pNextSlope->mGradientLevel == pSlope->mGradientLevel ||
                                    pSlope->mGradientDirection == pNextSlope->mGradientDirection)
                                {
                                    return false;
                                }
                                return true;
                            }
                            else
                            {
                                return true;
                            }
                        default:
                            return true;
                    }
                }
            }

            if (z > 0)
            {
                pBelow = GetEffectiveBlock(x, y, z - 1);
                if (pBelow && !IS_AIR_TYPE(pBelow->mSlopeType))
                {
                    if (IS_GRADIENT_SLOPE(pBelow->mSlopeType) == false)
                    {
                        return false;
                    }
                    pNextSlope = get_slope_struct(pBelow->mSlopeType);
                    if (pNextSlope->mGradientDirection > NO_GRADIENT_SLOPE_0 && pNextSlope->mGradientDirection <= SOUTH_2)
                    {
                        *pSlopeZDelta = 0xFF;
                        return false;
                    }
                    if (pBaseSlope && pBaseSlope->mGradientDirection == pNextSlope->mGradientDirection)
                    {
                        return false;
                    }
                    mbBlockedByTerrain = true;
                    return true;
                }
            }
            else
            {
                pBelow = NULL;
            }
            mbBlockedByTerrain = true;
            if (pBelow && pBelow->mArrows)
            {
                mbLowerBlockHasArrows = true;
                return false;
            }
            return true;

        case path_direction::right_3:
            if (pBlock)
            {
                if (x == 0xFF)
                {
                    break;
                }

                pSlope = get_slope_struct(pBlock->mSlopeType);
                if (pSlope)
                {
                    switch (pSlope->mGradientDirection)
                    {
                        case EAST_4:
                            if (pSlope->mGradientLevel == 0)
                            {
                                pNext = GetEffectiveBlock(x + 1, y, z + 1);
                                if (pNext && (pNext->mLeft & block_face::wall))
                                {
                                    return true;
                                }
                                if (GetEffectiveBlock(x + 1, y, z + 1))
                                {
                                    if (bReportStepUp)
                                    {
                                        *pSlopeZDelta = 1;
                                    }
                                    return false;
                                }
                                pNext = GetEffectiveBlock(x + 1, y, z);
                                if (pNext)
                                {
                                    if (IS_AIR_TYPE(pNext->mSlopeType))
                                    {
                                        mbBlockedByTerrain = true;
                                        return true;
                                    }
                                    if (bReportStepUp)
                                    {
                                        *pSlopeZDelta = 1;
                                    }
                                    return false;
                                }
                                else
                                {
                                    mbBlockedByTerrain = true;
                                    return true;
                                }
                            }
                            break;
                        case NORTH_1:
                        case SOUTH_2:
                            pNext = GetEffectiveBlock(x + 1, y, z);
                            if (pNext)
                            {
                                if (IS_GRADIENT_SLOPE(pNext->mSlopeType) && !IS_AIR_TYPE(pNext->mSlopeType))
                                {
                                    pNextSlope = get_slope_struct(pNext->mSlopeType);
                                    if (pNextSlope->mGradientLevel != pSlope->mGradientLevel &&
                                        pSlope->mGradientDirection != pNextSlope->mGradientDirection)
                                    {
                                        mbBlockedByTerrain = true;
                                        return true;
                                    }
                                }
                            }
                            else
                            {
                                mbBlockedByTerrain = true;
                                return true;
                            }
                            break;
                        default:
                            if (!(pBlock->mRight & block_face::wall))
                            {
                                break;
                            }
                            else
                            {
                                return true;
                            }
                    }

                    if (pBlock->mRight & block_face::wall)
                    {
                        break;
                    }
                }
            }

            pNext = GetEffectiveBlock(x + 1, y, z);
            if (pNext)
            {
                if (pNext->mLeft & block_face::wall)
                {
                    break;
                }
                if (IS_GRADIENT_SLOPE(pNext->mSlopeType) && !IS_AIR_TYPE(pNext->mSlopeType))
                {
                    pNextSlope = get_slope_struct(pNext->mSlopeType);
                    switch (pNextSlope->mGradientDirection)
                    {
                        case NORTH_1:
                        case SOUTH_2:
                            if (pSlope)
                            {
                                if (pNextSlope->mGradientLevel == pSlope->mGradientLevel ||
                                    pSlope->mGradientDirection == pNextSlope->mGradientDirection)
                                {
                                    return false;
                                }
                                return true;
                            }
                            else
                            {
                                return true;
                            }
                        case WEST_3:
                        case EAST_4:
                            return false;
                        default:
                            return true;
                    }
                }
            }

            if (z > 0)
            {
                pBelow = GetEffectiveBlock(x + 1, y, z - 1);
                if (pBelow && !IS_AIR_TYPE(pBelow->mSlopeType))
                {
                    if (IS_GRADIENT_SLOPE(pBelow->mSlopeType) == false)
                    {
                        return false;
                    }
                    pNextSlope = get_slope_struct(pBelow->mSlopeType);
                    if (pNextSlope->mGradientDirection >= WEST_3 && pNextSlope->mGradientDirection <= EAST_4)
                    {
                        *pSlopeZDelta = 0xFF;
                        return false;
                    }
                    if (pBaseSlope && pBaseSlope->mGradientDirection == pNextSlope->mGradientDirection)
                    {
                        return false;
                    }
                    mbBlockedByTerrain = true;
                    return true;
                }
            }
            else
            {
                pBelow = NULL;
            }
            mbBlockedByTerrain = true;
            if (pBelow && pBelow->mArrows)
            {
                mbLowerBlockHasArrows = true;
                return false;
            }
            return true;

        case path_direction::left_4:
            if (pBlock)
            {
                if (x == 0)
                {
                    return true;
                }

                pSlope = get_slope_struct(pBlock->mSlopeType);
                if (pSlope)
                {
                    switch (pSlope->mGradientDirection)
                    {
                        case WEST_3:
                            if (pSlope->mGradientLevel == 0)
                            {
                                pNext = GetEffectiveBlock(x - 1, y, z + 1);
                                if (pNext && (pNext->mRight & block_face::wall))
                                {
                                    return true;
                                }
                                if (GetEffectiveBlock(x - 1, y, z + 1))
                                {
                                    if (bReportStepUp)
                                    {
                                        *pSlopeZDelta = 1;
                                    }
                                    return false;
                                }
                                pNext = GetEffectiveBlock(x - 1, y, z);
                                if (pNext)
                                {
                                    if (IS_AIR_TYPE(pNext->mSlopeType))
                                    {
                                        mbBlockedByTerrain = true;
                                        return true;
                                    }
                                    if (bReportStepUp)
                                    {
                                        *pSlopeZDelta = 1;
                                    }
                                    return false;
                                }
                                else
                                {
                                    mbBlockedByTerrain = true;
                                    return true;
                                }
                            }
                            break;
                        case NORTH_1:
                        case SOUTH_2:
                            pNext = GetEffectiveBlock(x - 1, y, z);
                            if (pNext)
                            {
                                if (IS_GRADIENT_SLOPE(pNext->mSlopeType) && !IS_AIR_TYPE(pNext->mSlopeType))
                                {
                                    pNextSlope = get_slope_struct(pNext->mSlopeType);
                                    if (pNextSlope->mGradientLevel != pSlope->mGradientLevel &&
                                        pSlope->mGradientDirection != pNextSlope->mGradientDirection)
                                    {
                                        mbBlockedByTerrain = true;
                                        return true;
                                    }
                                }
                            }
                            else
                            {
                                mbBlockedByTerrain = true;
                                return true;
                            }
                            break;
                        default:
                            if (pBlock->mLeft & block_face::wall)
                            {
                                return true;
                            }
                            break;
                    }

                    if (pBlock->mLeft & block_face::wall)
                    {
                        return true;
                    }
                }
            }

            pNext = GetEffectiveBlock(x - 1, y, z);
            if (pNext)
            {
                if (pNext->mRight & block_face::wall)
                {
                    return true;
                }
                if (IS_GRADIENT_SLOPE(pNext->mSlopeType) && !IS_AIR_TYPE(pNext->mSlopeType))
                {
                    pNextSlope = get_slope_struct(pNext->mSlopeType);
                    switch (pNextSlope->mGradientDirection)
                    {
                        case NORTH_1:
                        case SOUTH_2:
                            if (pSlope)
                            {
                                if (pNextSlope->mGradientLevel == pSlope->mGradientLevel ||
                                    pSlope->mGradientDirection == pNextSlope->mGradientDirection)
                                {
                                    return false;
                                }
                                return true;
                            }
                            else
                            {
                                return true;
                            }
                        case WEST_3:
                        case EAST_4:
                            return false;
                        default:
                            return true;
                    }
                }
            }

            if (z > 0)
            {
                pBelow = GetEffectiveBlock(x - 1, y, z - 1);
                if (pBelow && !IS_AIR_TYPE(pBelow->mSlopeType))
                {
                    if (IS_GRADIENT_SLOPE(pBelow->mSlopeType) == false)
                    {
                        return false;
                    }
                    pNextSlope = get_slope_struct(pBelow->mSlopeType);
                    if (pNextSlope->mGradientDirection >= WEST_3 && pNextSlope->mGradientDirection <= EAST_4)
                    {
                        mbBlockedByTerrain = true;
                        *pSlopeZDelta = 0xFF;
                        return false;
                    }
                    if (pBaseSlope && pBaseSlope->mGradientDirection == pNextSlope->mGradientDirection)
                    {
                        return false;
                    }
                    mbBlockedByTerrain = true;
                    break;
                }
            }
            else
            {
                pBelow = NULL;
            }
            mbBlockedByTerrain = true;
            if (pBelow && pBelow->mArrows)
            {
                mbLowerBlockHasArrows = true;
                return false;
            }
            return true;

        default:
            return false;
    }

    return true;
}

// 9.6f 0x463690: does the segment p1-p2 cross any edge of the rect
inline bool Fix16_Rect::EdgesCrossSegment(Fix16_Point& p1, Fix16_Point& p2)
{
    return ComputeScanlineIntersectionY(mLeft, mRight, mTop, p1, p2) ||
        ComputeScanlineIntersectionY(mLeft, mRight, mBottom, p1, p2) ||
        ComputeScanlineIntersectionX(mTop, mBottom, mLeft, p1, p2) ||
        ComputeScanlineIntersectionX(mTop, mBottom, mRight, p1, p2);
}

MATCH_FUNC(0x4E11E0)
bool Map_0x370::RectHitsDiagonalWall(Fix16_Rect* pRect)
{
    Fix16_Point p1;
    Fix16_Point p2;

    pRect->DoSetCurrentRect();

    s32 z_calc = pRect->GetMidZ();

    for (s32 y_count = gSpriteGrid_top; y_count <= gSpriteGrid_bottom; y_count++)
    {
        for (s32 x_count = gSpriteGrid_left; x_count <= gSpriteGrid_right; x_count++)
        {
            gmp_block_info* pBlock = GetBlock(x_count, y_count, z_calc);
            if (pBlock)
            {
                s32 slope_mask = GET_SLOPE_BITS(pBlock->mSlopeType);
                if (slope_mask >= DIAGONAL_WALL_UP_LEFT && slope_mask <= TRIANGULAR_SIDES_DIAGONAL_DOWN_RIGHT)
                {
                    // 9.6f: Fix16_Point::SetXY for p1/p2 (inlined, using it changes the code)
                    if (slope_mask == DIAGONAL_WALL_UP_LEFT || slope_mask == DIAGONAL_WALL_DOWN_RIGHT || slope_mask == TRIANGULAR_SIDES_DIAGONAL_UP_LEFT ||
                        slope_mask == TRIANGULAR_SIDES_DIAGONAL_DOWN_RIGHT)
                    {
                        p1.x = Fix16(x_count);
                        p1.y = Fix16(y_count + 1);
                        p2.x = Fix16(x_count + 1);
                        p2.y = Fix16(y_count);
                    }
                    else
                    {
                        p1.x = Fix16(x_count);
                        p1.y = Fix16(y_count);
                        p2.x = Fix16(x_count + 1);
                        p2.y = Fix16(y_count + 1);
                    }

                    if (pRect->EdgesCrossSegment(p1, p2))
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

// https://decomp.me/scratch/jaBFe
MATCH_FUNC(0x4E1520)
bool Map_0x370::SpriteHitsDiagonalWall(s32 z_pos)
{
    Fix16_Point wall_start;
    Fix16_Point wall_end;
    for (s32 y_pos = gSpriteGrid_top; y_pos <= gSpriteGrid_bottom; y_pos++)
    {
        for (s32 x_pos = gSpriteGrid_left; x_pos <= gSpriteGrid_right; x_pos++)
        {
            gmp_block_info* pBlock = Map_0x370::GetBlock(x_pos, y_pos, z_pos);

            if (pBlock != NULL)
            {
                u32 slope_type = GET_SLOPE_BITS(pBlock->mSlopeType);
                if (slope_type >= DIAGONAL_WALL_UP_LEFT && slope_type <= TRIANGULAR_SIDES_DIAGONAL_DOWN_RIGHT)
                {
                    // 9.6f: Fix16_Point::SetXY for both points (inlined, using it changes the code)
                    if (slope_type == DIAGONAL_WALL_UP_LEFT || slope_type == DIAGONAL_WALL_DOWN_RIGHT ||
                        slope_type == TRIANGULAR_SIDES_DIAGONAL_UP_LEFT || slope_type == TRIANGULAR_SIDES_DIAGONAL_DOWN_RIGHT)
                    {
                        wall_start.x = Fix16(x_pos);
                        wall_start.y = Fix16(y_pos + 1);
                        wall_end.x = Fix16(x_pos + 1);
                        wall_end.y = Fix16(y_pos);
                    }
                    else
                    {
                        wall_start.x = Fix16(x_pos);
                        wall_start.y = Fix16(y_pos);
                        wall_end.x = Fix16(x_pos + 1);
                        wall_end.y = Fix16(y_pos + 1);
                    }

                    if (gMapCollisionSprite->PointInsideRotatedBounds(wall_start, wall_end))
                    {
                        Sprite* pSprt = gObject_5C->GetDirectionalObject(slope_type)->mSprite;
                        // (u32): the block centre goes through the Fix16(u32) constructor (out-of-line copy 0x4926F0);
                        // z is converted inline in its argument slot
                        pSprt->set_xyz_lazy_451950(Fix16(static_cast<u32>(x_pos)) + kFpHalf_6F5FE0,
                                                   Fix16(static_cast<u32>(y_pos)) + kFpHalf_6F5FE0,
                                                   z_pos);
                        pSprt->UpdateCollisionBoundsIfNeeded();
                        gCollisionTarget.SetSprite(pSprt);
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

MATCH_FUNC(0x4E18A0)
bool Map_0x370::HasWallInArea(s32 x_min, s32 x_max, s32 y_min, s32 y_max, s32 z)
{
    gmp_block_info* pBlock;

    for (s32 y_pos = y_min; y_pos <= y_max; y_pos++)
    {
        for (s32 x_pos = x_min; x_pos <= x_max; x_pos++)
        {
            if (x_pos < x_max)
            {
                pBlock = Map_0x370::GetEffectiveBlock(x_pos, y_pos, z);
                if (pBlock)
                {
                    if ((pBlock->mRight & block_face::wall) != 0)
                    {
                        return true;
                    }
                }
                pBlock = Map_0x370::GetEffectiveBlock(x_pos + 1, y_pos, z);
                if (pBlock)
                {
                    if ((pBlock->mLeft & block_face::wall) != 0)
                    {
                        return true;
                    }
                }
            }

            if (y_pos < y_max)
            {
                pBlock = Map_0x370::GetEffectiveBlock(x_pos, y_pos, z);
                if (pBlock)
                {
                    if ((pBlock->mBottom & block_face::wall) != 0)
                    {
                        return true;
                    }
                }
                pBlock = Map_0x370::GetEffectiveBlock(x_pos, y_pos + 1, z);
                if (pBlock)
                {
                    if ((pBlock->mTop & block_face::wall) != 0)
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

MATCH_FUNC(0x4E1A30)
bool Map_0x370::SpriteHitsBlockFacesInArea(s32 x_min, s32 x_max, s32 y_min, s32 y_max, s32 z)
{
    // The hit tests take (u32)y -> Fix16(u32) 0x4926F0, y + 1 -> Fix16(s32) 0x4369F0 and the
    // face coordinate x + 1 / y + 1 (inlined, strength reduced for x).

    for (s32 y = y_min; y <= y_max; y++)
    {
        for (s32 x = x_min; x <= x_max; x++)
        {
            if (x < x_max)
            {
                gmp_block_info* pBlock1 = Map_0x370::GetEffectiveBlock(x, y, z);
                if (pBlock1)
                {
                    if ((static_cast<u16>(gFaceCollisionMask) & pBlock1->mRight) != 0)
                    {
                        if (!IsNorthOrSouthGradSlope(pBlock1))
                        {
                            if (gMapCollisionSprite->HitTestVerticalLine(static_cast<u32>(y), y + 1, x + 1))
                            {
                                return true;
                            }
                        }
                    }
                }
                gmp_block_info* pBlock2 = Map_0x370::GetEffectiveBlock(x + 1, y, z);
                if (pBlock2)
                {
                    if ((static_cast<u16>(gFaceCollisionMask) & pBlock2->mLeft) != 0)
                    {
                        if (!IsNorthOrSouthGradSlope(pBlock2))
                        {
                            if (gMapCollisionSprite->HitTestVerticalLine(static_cast<u32>(y), y + 1, x + 1))
                            {
                                return true;
                            }
                        }
                    }
                }
            }

            if (y < y_max)
            {
                gmp_block_info* pBlock3 = Map_0x370::GetEffectiveBlock(x, y, z);
                if (pBlock3)
                {
                    if ((static_cast<u16>(gFaceCollisionMask) & pBlock3->mBottom) != 0)
                    {
                        if (!IsWestOrEastGradSlope(pBlock3))
                        {
                            if (gMapCollisionSprite->CheckBBoxScanlineIntersection(static_cast<u32>(x), x + 1, y + 1))
                            {
                                return true;
                            }
                        }
                    }
                }

                gmp_block_info* pBlock4 = Map_0x370::GetEffectiveBlock(x, y + 1, z);
                if (pBlock4)
                {
                    if ((static_cast<u16>(gFaceCollisionMask) & pBlock4->mTop) != 0)
                    {
                        if (!IsWestOrEastGradSlope(pBlock4))
                        {
                            if (gMapCollisionSprite->CheckBBoxScanlineIntersection(static_cast<u32>(x), x + 1, y + 1))
                            {
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

// Each face test passes a plain tile coordinate as u32 (out-of-line Fix16(u32) copy 0x4926F0) and a
// computed one as s32 (Fix16(s32) copy 0x4369F0); the last argument is converted inline.
MATCH_FUNC(0x4E1E00)
bool Map_0x370::IsTileBlockedForSprite(s32 regionLeft,
                                          s32 regionRight,
                                          s32 regionTop,
                                          s32 regionBottom,
                                          s32 tileX,
                                          s32 tileY,
                                          s32 zLevel)
{
    LOCAL_STATIC(gmp_block_info*, sBlockAbove, 0x6F6054);
    LOCAL_STATIC(gmp_block_info*, sEastBlock, 0x6F6060);
    LOCAL_STATIC(gmp_map_slope*, sEastSlope, 0x6F6080);
    LOCAL_STATIC(gmp_block_info*, sNorthBlock, 0x6F606C);
    LOCAL_STATIC(gmp_block_info*, sNorthEastBlock, 0x6F5F54);
    LOCAL_STATIC(gmp_map_slope*, sNorthSlope, 0x6F6084);
    LOCAL_STATIC(gmp_block_info*, sNorthWestBlock, 0x6F5F90);
    LOCAL_STATIC(gmp_block_info*, sSouthBlock, 0x6F6070);
    LOCAL_STATIC(gmp_block_info*, sSouthEastBlock, 0x6F5F98);
    LOCAL_STATIC(gmp_map_slope*, sSouthSlope, 0x6F6088);
    LOCAL_STATIC(gmp_block_info*, sSouthWestBlock, 0x6F5FB0);
    LOCAL_STATIC(gmp_block_info*, sWestBlock, 0x6F6078);
    LOCAL_STATIC(gmp_map_slope*, sWestSlope, 0x6F608C);
    sNorthTileZ = zLevel;
    sSouthTileZ = zLevel;
    sEastTileZ = zLevel;
    sWestTileZ = zLevel;

    if (gMapCollisionSprite)
    {
        if (!IsZOnGround(gMapCollisionSprite->mXy.x, gMapCollisionSprite->mXy.y, gMapCollisionSprite->mZpos))
        {
            if (!IsGradientSlopeAt(gMapCollisionSprite->mXy.x.ToInt(),
                                          gMapCollisionSprite->mXy.y.ToInt(),
                                          gMapCollisionSprite->mZpos.ToInt()))
            {
                return SpriteHitsBlockFacesInArea(regionLeft, regionRight, regionTop, regionBottom, zLevel);
            }
        }
    }

    gmp_block_info* pBlock = GetEffectiveBlock(tileX, tileY, zLevel);
    sBlockInfo0 = pBlock;
    if (!pBlock)
    {
        pBlock = &sBlockInfo1;
        sBlockInfo0 = &sBlockInfo1;
    }

    u8 gradient_direction;
    gCurrentBlockSlope = &gGmpSlopes[pBlock->mSlopeType >> 2];
    if (!gCurrentBlockSlope->mGradientDirection || gCurrentBlockSlope->mGradientLevel)
    {
        gradient_direction = NO_GRADIENT_SLOPE_0;
    }
    else
    {
        gradient_direction = gCurrentBlockSlope->mGradientDirection;
        switch (gradient_direction)
        {
            case 1u:
                ++sNorthTileZ;
                break;
            case 2u:
                ++sSouthTileZ;
                break;
            case 3u:
                ++sWestTileZ;
                break;
            case 4u:
                ++sEastTileZ;
                break;
            default:
                break;
        }
        sBlockAbove = GetBlock(tileX, tileY, zLevel + 1);
    }

    // North (y - 1)
    if (regionTop < tileY)
    {
        if (gradient_direction == NORTH_1)
        {
            if (sBlockAbove && (static_cast<u16>(gFaceCollisionMask) & sBlockAbove->mTop) != 0)
            {
                if (!gMapCollisionSprite || gMapCollisionSprite->CheckBBoxScanlineIntersection(static_cast<u32>(tileX), tileX + 1, tileY))
                {
                    return true;
                }
            }
        }
        else if ((static_cast<u16>(gFaceCollisionMask) & sBlockInfo0->mTop) != 0 &&
                 gCurrentBlockSlope->mGradientDirection != WEST_3 &&
                 gCurrentBlockSlope->mGradientDirection != EAST_4)
        {
            if (!gMapCollisionSprite || gMapCollisionSprite->CheckBBoxScanlineIntersection(static_cast<u32>(tileX), tileX + 1, tileY))
            {
                return true;
            }
        }

        sNorthBlock = GetEffectiveBlock(tileX, tileY - 1, sNorthTileZ);
        if (sNorthBlock)
        {
            if ((static_cast<u16>(gFaceCollisionMask) & sNorthBlock->mBottom) != 0)
            {
                if (!gMapCollisionSprite || gMapCollisionSprite->CheckBBoxScanlineIntersection(static_cast<u32>(tileX), tileX + 1, tileY))
                {
                    return true;
                }
            }
            sNorthSlope = &gGmpSlopes[sNorthBlock->mSlopeType >> 2];
        }
    }

    // South (y + 1)
    if (regionBottom > tileY)
    {
        if (gradient_direction == SOUTH_2)
        {
            if (sBlockAbove && (static_cast<u16>(gFaceCollisionMask) & sBlockAbove->mBottom) != 0)
            {
                if (!gMapCollisionSprite || gMapCollisionSprite->CheckBBoxScanlineIntersection(static_cast<u32>(tileX), tileX + 1, tileY + 1))
                {
                    return true;
                }
            }
        }
        else if ((static_cast<u16>(gFaceCollisionMask) & sBlockInfo0->mBottom) != 0 &&
                 gCurrentBlockSlope->mGradientDirection != WEST_3 &&
                 gCurrentBlockSlope->mGradientDirection != EAST_4)
        {
            if (!gMapCollisionSprite || gMapCollisionSprite->CheckBBoxScanlineIntersection(static_cast<u32>(tileX), tileX + 1, tileY + 1))
            {
                return true;
            }
        }

        sSouthBlock = GetEffectiveBlock(tileX, tileY + 1, sSouthTileZ);
        if (sSouthBlock)
        {
            if ((static_cast<u16>(gFaceCollisionMask) & sSouthBlock->mTop) != 0)
            {
                if (!gMapCollisionSprite || gMapCollisionSprite->CheckBBoxScanlineIntersection(static_cast<u32>(tileX), tileX + 1, tileY + 1))
                {
                    return true;
                }
            }
            sSouthSlope = &gGmpSlopes[sSouthBlock->mSlopeType >> 2];
        }
    }

    // West (x - 1) and the north west / south west corners
    if (regionLeft < tileX)
    {
        if (gradient_direction == WEST_3)
        {
            if (sBlockAbove && (static_cast<u16>(gFaceCollisionMask) & sBlockAbove->mLeft) != 0)
            {
                if (!gMapCollisionSprite || gMapCollisionSprite->HitTestVerticalLine(static_cast<u32>(tileY), tileY + 1, tileX))
                {
                    return true;
                }
            }
        }
        else if ((static_cast<u16>(gFaceCollisionMask) & sBlockInfo0->mLeft) != 0 &&
                 gCurrentBlockSlope->mGradientDirection != NORTH_1 &&
                 gCurrentBlockSlope->mGradientDirection != SOUTH_2)
        {
            if (!gMapCollisionSprite || gMapCollisionSprite->HitTestVerticalLine(static_cast<u32>(tileY), tileY + 1, tileX))
            {
                return true;
            }
        }

        sWestBlock = GetEffectiveBlock(tileX - 1, tileY, sWestTileZ);
        if (sWestBlock)
        {
            if ((static_cast<u16>(gFaceCollisionMask) & sWestBlock->mRight) != 0)
            {
                if (!gMapCollisionSprite || gMapCollisionSprite->HitTestVerticalLine(static_cast<u32>(tileY), tileY + 1, tileX))
                {
                    return true;
                }
            }
            sWestSlope = &gGmpSlopes[sWestBlock->mSlopeType >> 2];
        }

        if (regionTop < tileY)
        {
            if (sNorthBlock && sWestTileZ <= sNorthTileZ &&
                (static_cast<u16>(gFaceCollisionMask) & sNorthBlock->mLeft) != 0 &&
                sNorthSlope->mGradientDirection != NORTH_1 && sNorthSlope->mGradientDirection != SOUTH_2)
            {
                if (!gMapCollisionSprite || gMapCollisionSprite->HitTestVerticalLine(tileY - 1, static_cast<u32>(tileY), tileX))
                {
                    return true;
                }
            }

            if (sWestBlock && sNorthTileZ <= sWestTileZ &&
                (static_cast<u16>(gFaceCollisionMask) & sWestBlock->mTop) != 0 &&
                sWestSlope->mGradientDirection != WEST_3 && sWestSlope->mGradientDirection != EAST_4)
            {
                if (!gMapCollisionSprite || gMapCollisionSprite->CheckBBoxScanlineIntersection(tileX - 1, static_cast<u32>(tileX), tileY))
                {
                    return true;
                }
            }

            sNorthWestBlock = GetEffectiveBlock(tileX - 1, tileY - 1, sNorthTileZ > sWestTileZ ? sNorthTileZ : sWestTileZ);
            if (sNorthWestBlock)
            {
                if ((static_cast<u16>(gFaceCollisionMask) & sNorthWestBlock->mRight) != 0)
                {
                    if (!gMapCollisionSprite || gMapCollisionSprite->HitTestVerticalLine(tileY - 1, static_cast<u32>(tileY), tileX))
                    {
                        return true;
                    }
                }
                if (sNorthWestBlock && (static_cast<u16>(gFaceCollisionMask) & sNorthWestBlock->mBottom) != 0)
                {
                    if (!gMapCollisionSprite || gMapCollisionSprite->CheckBBoxScanlineIntersection(tileX - 1, static_cast<u32>(tileX), tileY))
                    {
                        return true;
                    }
                }
            }
        }

        if (regionBottom > tileY)
        {
            if (sSouthBlock && sWestTileZ <= sSouthTileZ &&
                (static_cast<u16>(gFaceCollisionMask) & sSouthBlock->mLeft) != 0 &&
                sSouthSlope->mGradientDirection != NORTH_1 && sSouthSlope->mGradientDirection != SOUTH_2)
            {
                if (!gMapCollisionSprite || gMapCollisionSprite->HitTestVerticalLine(tileY + 1, tileY + 2, tileX))
                {
                    return true;
                }
            }

            if (sWestBlock && sSouthTileZ <= sWestTileZ &&
                (static_cast<u16>(gFaceCollisionMask) & sWestBlock->mBottom) != 0 &&
                sWestSlope->mGradientDirection != WEST_3 && sWestSlope->mGradientDirection != EAST_4)
            {
                if (!gMapCollisionSprite || gMapCollisionSprite->CheckBBoxScanlineIntersection(tileX - 1, static_cast<u32>(tileX), tileY + 1))
                {
                    return true;
                }
            }

            sSouthWestBlock = GetEffectiveBlock(tileX - 1, tileY + 1, sSouthTileZ > sWestTileZ ? sSouthTileZ : sWestTileZ);
            if (sSouthWestBlock)
            {
                if ((static_cast<u16>(gFaceCollisionMask) & sSouthWestBlock->mRight) != 0)
                {
                    if (!gMapCollisionSprite || gMapCollisionSprite->HitTestVerticalLine(tileY + 1, tileY + 2, tileX))
                    {
                        return true;
                    }
                }
                if (sSouthWestBlock && (static_cast<u16>(gFaceCollisionMask) & sSouthWestBlock->mTop) != 0)
                {
                    if (!gMapCollisionSprite || gMapCollisionSprite->CheckBBoxScanlineIntersection(tileX - 1, static_cast<u32>(tileX), tileY + 1))
                    {
                        return true;
                    }
                }
            }
        }
    }

    // East (x + 1) and the north east / south east corners
    if (regionRight > tileX)
    {
        if (gradient_direction == EAST_4)
        {
            if (sBlockAbove && (static_cast<u16>(gFaceCollisionMask) & sBlockAbove->mRight) != 0)
            {
                if (!gMapCollisionSprite || gMapCollisionSprite->HitTestVerticalLine(static_cast<u32>(tileY), tileY + 1, tileX + 1))
                {
                    return true;
                }
            }
        }
        else if ((static_cast<u16>(gFaceCollisionMask) & sBlockInfo0->mRight) != 0 &&
                 gCurrentBlockSlope->mGradientDirection != NORTH_1 &&
                 gCurrentBlockSlope->mGradientDirection != SOUTH_2)
        {
            if (!gMapCollisionSprite || gMapCollisionSprite->HitTestVerticalLine(static_cast<u32>(tileY), tileY + 1, tileX + 1))
            {
                return true;
            }
        }

        sEastBlock = GetEffectiveBlock(tileX + 1, tileY, sEastTileZ);
        if (sEastBlock)
        {
            if ((static_cast<u16>(gFaceCollisionMask) & sEastBlock->mLeft) != 0)
            {
                if (!gMapCollisionSprite || gMapCollisionSprite->HitTestVerticalLine(static_cast<u32>(tileY), tileY + 1, tileX + 1))
                {
                    return true;
                }
            }
            sEastSlope = &gGmpSlopes[sEastBlock->mSlopeType >> 2];
        }

        if (regionTop < tileY)
        {
            if (sNorthBlock && sEastTileZ <= sNorthTileZ &&
                (static_cast<u16>(gFaceCollisionMask) & sNorthBlock->mRight) != 0 &&
                sNorthSlope->mGradientDirection != NORTH_1 && sNorthSlope->mGradientDirection != SOUTH_2)
            {
                if (!gMapCollisionSprite || gMapCollisionSprite->HitTestVerticalLine(tileY - 1, static_cast<u32>(tileY), tileX + 1))
                {
                    return true;
                }
            }

            if (sEastBlock && sNorthTileZ <= sEastTileZ &&
                (static_cast<u16>(gFaceCollisionMask) & sEastBlock->mTop) != 0 &&
                sEastSlope->mGradientDirection != WEST_3 && sEastSlope->mGradientDirection != EAST_4)
            {
                if (!gMapCollisionSprite || gMapCollisionSprite->CheckBBoxScanlineIntersection(tileX + 1, tileX + 2, tileY))
                {
                    return true;
                }
            }

            sNorthEastBlock = GetEffectiveBlock(tileX + 1, tileY - 1, sNorthTileZ > sEastTileZ ? sNorthTileZ : sEastTileZ);
            if (sNorthEastBlock)
            {
                if ((static_cast<u16>(gFaceCollisionMask) & sNorthEastBlock->mLeft) != 0)
                {
                    if (!gMapCollisionSprite || gMapCollisionSprite->HitTestVerticalLine(tileY - 1, static_cast<u32>(tileY), tileX + 1))
                    {
                        return true;
                    }
                }
                if (sNorthEastBlock && (static_cast<u16>(gFaceCollisionMask) & sNorthEastBlock->mBottom) != 0)
                {
                    if (!gMapCollisionSprite || gMapCollisionSprite->CheckBBoxScanlineIntersection(tileX + 1, tileX + 2, tileY))
                    {
                        return true;
                    }
                }
            }
        }

        if (regionBottom > tileY)
        {
            if (sSouthBlock && sEastTileZ <= sSouthTileZ &&
                (static_cast<u16>(gFaceCollisionMask) & sSouthBlock->mRight) != 0 &&
                sSouthSlope->mGradientDirection != NORTH_1 && sSouthSlope->mGradientDirection != SOUTH_2)
            {
                if (!gMapCollisionSprite || gMapCollisionSprite->HitTestVerticalLine(tileY + 1, tileY + 2, tileX + 1))
                {
                    return true;
                }
            }

            if (sEastBlock && sSouthTileZ <= sEastTileZ &&
                (static_cast<u16>(gFaceCollisionMask) & sEastBlock->mBottom) != 0 &&
                sEastSlope->mGradientDirection != WEST_3 && sEastSlope->mGradientDirection != EAST_4)
            {
                if (!gMapCollisionSprite || gMapCollisionSprite->CheckBBoxScanlineIntersection(tileX + 1, tileX + 2, tileY + 1))
                {
                    return true;
                }
            }

            sSouthEastBlock = GetEffectiveBlock(tileX + 1, tileY + 1, sSouthTileZ > sEastTileZ ? sSouthTileZ : sEastTileZ);
            if (sSouthEastBlock)
            {
                if ((static_cast<u16>(gFaceCollisionMask) & sSouthEastBlock->mLeft) != 0)
                {
                    if (!gMapCollisionSprite || gMapCollisionSprite->HitTestVerticalLine(tileY + 1, tileY + 2, tileX + 1))
                    {
                        return true;
                    }
                }
                if (sSouthEastBlock && (static_cast<u16>(gFaceCollisionMask) & sSouthEastBlock->mTop) != 0)
                {
                    if (!gMapCollisionSprite || gMapCollisionSprite->CheckBBoxScanlineIntersection(tileX + 1, tileX + 2, tileY + 1))
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

MATCH_FUNC(0x4E4460)
bool Map_0x370::IsMovementRegionBlocked(s32 tile_x, s32 tile_y, s32 tile_z, Sprite* pSprite, s16 collision_mask)
{
    gMapCollisionSprite = pSprite;
    gFaceCollisionMask = collision_mask;
    if (gSpriteGrid_right - gSpriteGrid_left >= 3 || tile_x > gSpriteGrid_left + 1 || tile_x < gSpriteGrid_right - 1)
    {
        if (IsTileBlockedForSprite(tile_x - 1, tile_x + 1, gSpriteGrid_top, gSpriteGrid_bottom, tile_x, tile_y, tile_z))
        {
            return true;
        }
        else if (gSpriteGrid_right == tile_x + 2)
        {
            return IsTileBlockedForSprite(gSpriteGrid_right - 1,
                                             gSpriteGrid_right,
                                             gSpriteGrid_top,
                                             gSpriteGrid_bottom,
                                             gSpriteGrid_right - 1,
                                             tile_y,
                                             sEastTileZ);
        }
        else
        {
            return IsTileBlockedForSprite(gSpriteGrid_left,
                                             gSpriteGrid_left + 1,
                                             gSpriteGrid_top,
                                             gSpriteGrid_bottom,
                                             gSpriteGrid_left + 1,
                                             tile_y,
                                             sWestTileZ);
        }
    }
    else if (gSpriteGrid_bottom - gSpriteGrid_top >= 3 || tile_y > gSpriteGrid_top + 1 || tile_y < gSpriteGrid_bottom - 1)
    {
        if (IsTileBlockedForSprite(gSpriteGrid_left, gSpriteGrid_right, tile_y - 1, tile_y + 1, tile_x, tile_y, tile_z))
        {
            return true;
        }
        else if (gSpriteGrid_bottom == tile_y + 2)
        {
            return IsTileBlockedForSprite(gSpriteGrid_left,
                                             gSpriteGrid_right,
                                             gSpriteGrid_bottom - 1,
                                             gSpriteGrid_bottom,
                                             tile_x,
                                             gSpriteGrid_bottom - 1,
                                             sSouthTileZ);
        }
        else
        {
            return IsTileBlockedForSprite(gSpriteGrid_left,
                                             gSpriteGrid_right,
                                             gSpriteGrid_top,
                                             gSpriteGrid_top + 1,
                                             tile_x,
                                             gSpriteGrid_top + 1,
                                             sNorthTileZ);
        }
    }
    else
    {
        return IsTileBlockedForSprite(gSpriteGrid_left, gSpriteGrid_right, gSpriteGrid_top, gSpriteGrid_bottom, tile_x, tile_y, tile_z);
    }
}

MATCH_FUNC(0x4E4630)
char_type Map_0x370::CheckSlopeTopInSpriteGrid(Fix16 z_pos)
{
    s32 z_int = z_pos.ToInt();
    bool bFractionalZ = !!(z_pos.GetFracValue() != kFpZero_6F610C);
    bool bFound = false;

    for (s32 ypos = gSpriteGrid_top; ypos <= gSpriteGrid_bottom; ypos++)
    {
        for (s32 xpos = gSpriteGrid_left; xpos <= gSpriteGrid_right; xpos++)
        {
            gmp_block_info* pBlock = Map_0x370::GetBlock(xpos, ypos, z_int);
            sBlockInfo0 = pBlock;
            if (pBlock)
            {
                u8 slope_byte = pBlock->mSlopeType;
                if (IS_GRADIENT_SLOPE(slope_byte) && !IS_AIR_TYPE(slope_byte))
                {
                    if (bFractionalZ)
                    {
                        bFound = true;
                    }
                    else
                    {
                        gCurrentBlockSlope = &gGmpSlopes[GET_SLOPE_IDX(sBlockInfo0->mSlopeType)];
                        if (gCurrentBlockSlope->mGradientLevel == gCurrentBlockSlope->mGradientSize - 1)
                        {
                            bFound = true;
                        }
                    }
                }
                else
                {
                    if (bFractionalZ && IS_SLOPE_IN_BLOCK_ABOVE(slope_byte))
                    {
                        return 2;
                    }
                }
            }
        }
    }
    return bFound;
}

MATCH_FUNC(0x4E4770)
bool Map_0x370::SlopeBordersFlatInSpriteGrid(Fix16 z_pos)
{
    s32 z = z_pos.ToInt();
    bool bSeenSlope = false;
    bool bSeenFlat = false;

    for (s32 y_pos = gSpriteGrid_top; y_pos <= gSpriteGrid_bottom; y_pos++)
    {
        for (s32 x_pos = gSpriteGrid_left; x_pos <= gSpriteGrid_right; x_pos++)
        {
            sBlockInfo0 = Map_0x370::GetBlock(x_pos, y_pos, z);

            if (sBlockInfo0 != NULL)
            {
                if (IS_GRADIENT_SLOPE(sBlockInfo0->mSlopeType) && !IS_AIR_TYPE(sBlockInfo0->mSlopeType))
                {
                    if (bSeenFlat)
                    {
                        return true;
                    }
                    bSeenSlope = true;
                    continue;
                }
            }

            if (bSeenSlope)
            {
                return true;
            }
            bSeenFlat = true;
        }
    }
    return false;
}

MATCH_FUNC(0x4E4820)
bool Map_0x370::AllBlocksInRectHaveSlopeBits(Fix16_Rect* pRect, u8 slope_bits)
{
    s32 left = Fix16::Round_To_Int(pRect->get_left());
    s32 right = Fix16::Round_To_Int(pRect->get_right());
    s32 top = Fix16::Round_To_Int(pRect->get_top());
    s32 bottom = Fix16::Round_To_Int(pRect->get_bottom());

    for (s32 y = top; y <= bottom; y++)
    {
        for (s32 x = left; x <= right; x++)
        {
            sBlockInfo0 = GetBlock(x, y, pRect->get_low_z().ToInt());
            switch (slope_bits)
            {
                case 0:
                    if (sBlockInfo0 && !IS_AIR_TYPE(sBlockInfo0->mSlopeType))
                    {
                        return false;
                    }
                    break;
                case 1:
                    if (sBlockInfo0 && GET_BLOCK_TYPE(sBlockInfo0->mSlopeType) != 1)
                    {
                        return false;
                    }
                    break;
                case 2:
                    if (sBlockInfo0 && GET_BLOCK_TYPE(sBlockInfo0->mSlopeType) != 2)
                    {
                        return false;
                    }
                    break;
                case 3:
                    if (sBlockInfo0 && GET_BLOCK_TYPE(sBlockInfo0->mSlopeType) != 3)
                    {
                        return false;
                    }
                    break;
            }
        }
    }
    return true;
}

// Spirals out from (x, y) until it finds a block of the given type and writes its position back
MATCH_FUNC(0x4E4930)
void Map_0x370::FindNearbyBlockOfType(u8* pX, u8* pY, u8* pZ, char_type block_type)
{
    u16 step = 1;
    s32 direction = 3;
    sSearchX = *pX;
    sSearchY = *pY;
    sSearchZ = *pZ;
    u16 i;
    while (1)
    {
        switch (direction)
        {
            case 3:
                for (i = 0; i < step; i++)
                {
                    sSearchX++;
                    if (IsSearchBlockOfType(block_type))
                    {
                        *pX = sSearchX;
                        *pY = sSearchY;
                        *pZ = sSearchZ;
                        return;
                    }
                }
                direction = 2;

                break;
            case 2:
                for (i = 0; i < step; i++)
                {
                    sSearchY++;
                    if (IsSearchBlockOfType(block_type))
                    {
                        *pX = sSearchX;
                        *pY = sSearchY;
                        *pZ = sSearchZ;
                        return;
                    }
                }
                direction = 4;
                step++;
                break;
            case 4:
                for (i = 0; i < step; i++)
                {
                    sSearchX--;
                    if (IsSearchBlockOfType(block_type))
                    {
                        *pX = sSearchX;
                        *pY = sSearchY;
                        *pZ = sSearchZ;
                        return;
                    }
                }
                direction = 1;

                break;
            case 1:
                for (i = 0; i < step; i++)
                {
                    sSearchY--;
                    if (IsSearchBlockOfType(block_type))
                    {
                        *pX = sSearchX;
                        *pY = sSearchY;
                        *pZ = sSearchZ;
                        return;
                    }
                }
                direction = 3;
                step++;
                break;
        }
    }
}

MATCH_FUNC(0x4E4AC0)
bool __stdcall Map_0x370::IsSearchBlockOfType(char_type block_type)
{
    if (sSearchX >= 0 && sSearchY >= 0 && sSearchX <= 255 && sSearchY <= 255)
    {
        if (block_type == PAVEMENT)
        {
            if (gMap_0x370->FindPavementBlockForCoord(sSearchX, sSearchY, sSearchZ))
            {
                return true;
            }
        }
        else if (gMap_0x370->GetBlockTypeAtCoord(sSearchX, sSearchY, sSearchZ) == ROAD)
        {
            return true;
        }
    }
    return false;
}

MATCH_FUNC(0x4E4B40)
bool Map_0x370::CheckGreenArrowDirection(s32 direction, gmp_block_info* pBlock)
{
    switch (direction)
    {
        case 1:
            return check_green_up(pBlock->mArrows);
            break;
        case 2:
            return check_green_down(pBlock->mArrows);
            break;
        case 3:
            return check_green_right(pBlock->mArrows);
            break;
        case 4:
            return check_green_left(pBlock->mArrows);
            break;
        default:
            return true;
            break;
    }
}

MATCH_FUNC(0x4E4BB0)
gmp_block_info* Map_0x370::FindPavementBlockForCoord(s32 x, s32 y, s32& z)
{
    gmp_col_info* pColumn = reinterpret_cast<gmp_col_info*>(&mpDmap->mpColumn[*mpDmap->get_base(y, x)]);
    for (s32 curr_z_pos = pColumn->mHeight - pColumn->mOffset - 1; curr_z_pos >= 0; curr_z_pos--)
    {
        //  Begin with the highest non-empty block in the column
        sBlockInfo0 = mpDmap->get_block(pColumn->mBlockd[curr_z_pos]); //  get the block
        switch (GET_BLOCK_TYPE(sBlockInfo0->mSlopeType))
        {
            case AIR:
                continue;
            case PAVEMENT:
                z = curr_z_pos + pColumn->mOffset;
                return sBlockInfo0;
            default:
                return NULL;
        }
    }
    return NULL;
}

MATCH_FUNC(0x4E4C30)
gmp_block_info* Map_0x370::FindHighestBlockForCoord(s32 x, s32 y, s32* found_z)
{
    gmp_col_info* pColumn = reinterpret_cast<gmp_col_info*>(&this->mpDmap->mpColumn[*mpDmap->get_base(y, x)]);

    for (s32 curr_z_pos = pColumn->mHeight - pColumn->mOffset - 1; curr_z_pos >= 0; curr_z_pos--)
    {
        sBlockInfo0 = mpDmap->get_block(pColumn->mBlockd[curr_z_pos]);
        u8 slope_byte = sBlockInfo0->mSlopeType;
        if (!IS_AIR_TYPE(slope_byte))
        {
            *found_z = curr_z_pos + pColumn->mOffset;
            return sBlockInfo0;
        }
    }
    return NULL;
}

MATCH_FUNC(0x4E4CB0)
gmp_block_info* Map_0x370::FindNonAirBlockAtOrBelowZ(s32 x, s32 y, s32& z)
{
    gmp_col_info* pColumn = reinterpret_cast<gmp_col_info*>(&this->mpDmap->mpColumn[*this->mpDmap->get_base(y, x)]);

    if (z < pColumn->mOffset)
    {
        return NULL;
    }

    s32 curr_z_pos = clamp_z_from_column(pColumn->mHeight, pColumn->mOffset, z);

    for (; curr_z_pos >= 0; curr_z_pos--)
    {
        sBlockInfo0 = mpDmap->get_block(pColumn->mBlockd[curr_z_pos]);
        if (GET_BLOCK_TYPE(sBlockInfo0->mSlopeType) != AIR)
        {
            z = curr_z_pos + pColumn->mOffset;
            return sBlockInfo0;
        }
    }
    return NULL;
}

MATCH_FUNC(0x4E4D40)
Fix16 Map_0x370::FindGroundZBelowCoord(Fix16 x_pos, Fix16 y_pos, Fix16 z_pos)
{
    u8 slope_byte;
    Fix16 new_z;

    if (z_pos.GetFracValue() == kFpZero_6F610C || !IsGradientSlopeAt(x_pos.ToInt(), y_pos.ToInt(), z_pos.ToInt()) ||
        (new_z = z_pos.GetFloorValue(), Map_0x370::UpdateZFromSlopeAtCoord(x_pos, y_pos, new_z), new_z > z_pos))
    {
        s32 below_z = z_pos.ToInt() - 1;
        gmp_block_info* pBlock = Map_0x370::FindNonAirBlockAtOrBelowZ(x_pos.ToInt(), y_pos.ToInt(), below_z);
        sBlockInfo0 = pBlock;
        if (pBlock == NULL)
        {
            return kFpOne_6F6110;
        }
        slope_byte = pBlock->mSlopeType;

        if (IS_GRADIENT_SLOPE(slope_byte) && !IS_AIR_TYPE(slope_byte))
        {
            new_z = Fix16(below_z);
            Map_0x370::UpdateZFromSlopeAtCoord(x_pos, y_pos, new_z);
        }
        else
        {
            new_z = Fix16(below_z) + 1;
        }
    }
    return new_z;
}

MATCH_FUNC(0x4E4E50)
Fix16 Map_0x370::FindGroundZAboveCoord(Fix16 x_pos, Fix16 y_pos, Fix16 z_pos)
{
    for (Fix16 curr_z = z_pos; curr_z < z_pos + kFpOne_6F6110; curr_z = kFpOne_6F6110 + curr_z.GetFloorValue())
    {
        gmp_block_info* pBlockAtPos = GetBlock(x_pos.ToInt(), y_pos.ToInt(), curr_z.ToInt());
        sBlockInfo0 = pBlockAtPos;
        u8 slope_byte;
        if (pBlockAtPos == NULL || (slope_byte = pBlockAtPos->mSlopeType, IS_AIR_TYPE(slope_byte)))
        {
            gmp_block_info* pBlock = GetBlock(x_pos.ToInt(), y_pos.ToInt(), (curr_z - kFpOne_6F6110).ToInt());
            sBlockInfo0 = pBlock;
            if (pBlock)
            {
                u8 slope_byte = pBlock->mSlopeType;

                if (!IS_AIR_TYPE(slope_byte))
                {
                    u8 slope = GET_SLOPE_BITS(slope_byte);
                    if (slope <= 0 || slope >= 0xB4u) // !IS_GRADIENT_SLOPE(slope_byte)
                    {
                        return curr_z;
                    }
                }
            }
        }
        else
        {
            if (IS_GRADIENT_SLOPE(slope_byte))
            {
                Fix16 slope_z = curr_z.GetFloorValue();
                Map_0x370::UpdateZFromSlopeAtCoord(x_pos, y_pos, slope_z);
                if (slope_z >= curr_z)
                {
                    return slope_z;
                }
            }
        }
    }
    return z_pos;
}

MATCH_FUNC(0x4E4F40)
Fix16 Map_0x370::GetGroundZBelowCoord(Fix16 x, Fix16 y, Fix16 z)
{
    Fix16 new_z;

    if (z.GetFracValue() == kFpZero_6F610C || !IsGradientSlopeAt(x.ToInt(), y.ToInt(), z.ToInt()) ||
        (new_z = z.GetFloorValue(), Map_0x370::UpdateZFromSlopeAtCoord(x, y, new_z), new_z > z))
    {
        s32 int_z = z.ToInt() - 1;
        sBlockInfo0 = Map_0x370::FindNonAirBlockAtOrBelowZ(x.ToInt(), y.ToInt(), int_z);

        if (!sBlockInfo0)
        {
            return kFpZero_6F610C;
        }
        if (IS_GRADIENT_SLOPE(sBlockInfo0->mSlopeType) && !IS_AIR_TYPE(sBlockInfo0->mSlopeType))
        {
            new_z = Fix16(int_z);
            Map_0x370::UpdateZFromSlopeAtCoord(x, y, new_z);
        }
        else
        {
            new_z = Fix16(int_z + 1);
        }
    }
    return new_z;
}

MATCH_FUNC(0x4E5050)
Fix16 Map_0x370::GetGroundZBelowCoordEx(Fix16 x, Fix16 y, Fix16 z, bool& bInsideSlope)
{
    Fix16 new_z;
    bInsideSlope = false;
    if (z.GetFracValue() != kFpZero_6F610C)
    {
        gmp_block_info* pBlockAtPos = Map_0x370::GetBlock(x.ToInt(), y.ToInt(), z.ToInt());
        if (pBlockAtPos)
        {
            if (IS_GRADIENT_SLOPE(pBlockAtPos->mSlopeType) && !IS_AIR_TYPE(pBlockAtPos->mSlopeType))
            {
                new_z = z.GetFloorValue();
                Map_0x370::UpdateZFromSlopeAtCoord(x, y, new_z);
                if (new_z <= z)
                {
                    return new_z;
                }
                bInsideSlope = true;
            }
        }
    }
    s32 below_z = z.ToInt() - 1;
    sBlockInfo0 = Map_0x370::FindNonAirBlockAtOrBelowZ(x.ToInt(), y.ToInt(), below_z);
    if (!sBlockInfo0)
    {
        return kFpOne_6F6110;
    }
    if (IS_GRADIENT_SLOPE(sBlockInfo0->mSlopeType) && !IS_AIR_TYPE(sBlockInfo0->mSlopeType))
    {
        new_z = Fix16(below_z);
        Map_0x370::UpdateZFromSlopeAtCoord(x, y, new_z);
    }
    else
    {
        new_z = Fix16(below_z + 1);
    }
    return new_z;
}

MATCH_FUNC(0x4E5170)
bool Map_0x370::IsZOnGround(Fix16 x, Fix16 y, Fix16 z)
{
    if (z.GetFracValue() != kFpZero_6F610C)
    {
        sBlockInfo0 = Map_0x370::GetBlock(x.ToInt(), y.ToInt(), z.ToInt());
        if (sBlockInfo0)
        {
            if (IS_GRADIENT_SLOPE(sBlockInfo0->mSlopeType) && !IS_AIR_TYPE(sBlockInfo0->mSlopeType))
            {
                Fix16 new_z = z.GetFloorValue();
                Map_0x370::UpdateZFromSlopeAtCoord(x, y, new_z);
                if (new_z == z)
                {
                    return true;
                }
            }
        }
    }
    else
    {
        if (z == kFpOne_6F6110)
        {
            return true;
        }
        sBlockInfo0 = Map_0x370::GetBlock(x.ToInt(), y.ToInt(), (z - kFpOne_6F6110).ToInt());
        if (sBlockInfo0 && !IS_AIR_TYPE(sBlockInfo0->mSlopeType))
        {
            return true;
        }
    }
    return false;
}

MATCH_FUNC(0x4E52A0)
char_type Map_0x370::GetSurfaceTypeBelowCoord(Fix16 x, Fix16 y, Fix16 z)
{
    sBlockInfo0 = Map_0x370::GetBlock(x.ToInt(), y.ToInt(), (z - kFpOne_6F6110).ToInt());

    if (sBlockInfo0)
    {
        if (gGtx_0x106C->IsWaterTile(get_tile_idx(sBlockInfo0->mLid)))
        {
            return 7;
        }
        if (!IS_AIR_TYPE(sBlockInfo0->mSlopeType))
        {
            return 0;
        }
    }
    return car_surface_type::no_block_5;
}

MATCH_FUNC(0x4E5300)
bool Map_0x370::CheckZCollisionAtCoord(Fix16 x, Fix16 y, Fix16 z, Fix16 second_z)
{
    Fix16 new_z;
    Fix16 floor_z = z.GetFloorValue();
    Fix16 floor_second_z = second_z.GetFloorValue();

    if (floor_z == floor_second_z)
    {
        sBlockInfo0 = Map_0x370::GetBlock(x.ToInt(), y.ToInt(), floor_z.ToInt());
        if (sBlockInfo0)
        {
            if (!IS_AIR_TYPE(sBlockInfo0->mSlopeType) && IS_GRADIENT_SLOPE(sBlockInfo0->mSlopeType))
            {
                new_z = floor_z;
                Map_0x370::UpdateZFromSlopeAtCoord(x, y, new_z);
                if (second_z >= new_z && z <= new_z)
                {
                    return true;
                }
            }
        }
    }
    else
    {
        sBlockInfo0 = Map_0x370::GetBlock(x.ToInt(), y.ToInt(), floor_second_z.ToInt());
        if (sBlockInfo0)
        {
            if (!IS_AIR_TYPE(sBlockInfo0->mSlopeType) && IS_GRADIENT_SLOPE(sBlockInfo0->mSlopeType))
            {
                new_z = floor_second_z;
                Map_0x370::UpdateZFromSlopeAtCoord(x, y, new_z);
                if (second_z >= new_z)
                {
                    return true;
                }
            }
        }
        sBlockInfo0 = Map_0x370::GetBlock(x.ToInt(), y.ToInt(), floor_z.ToInt());
        if (sBlockInfo0)
        {
            if (!IS_AIR_TYPE(sBlockInfo0->mSlopeType))
            {
                if (IS_GRADIENT_SLOPE(sBlockInfo0->mSlopeType))
                {
                    new_z = floor_z;
                    Map_0x370::UpdateZFromSlopeAtCoord(x, y, new_z);
                    if (z <= new_z)
                    {
                        return true;
                    }
                }
                else
                {
                    return true;
                }
            }
        }
    }
    return false;
}

MATCH_FUNC(0x4E5480)
bool Map_0x370::CanPlaceOilOrMine(Fix16 x, Fix16 y, Fix16 z, Fix16 z_high, Fix16* found_z)
{
    Fix16 z_floor = z.GetFloorValue();
    Fix16 z_high_floor = z_high.GetFloorValue();

    if (z_floor == z_high_floor)
    {
        sBlockInfo0 = Map_0x370::GetBlock(x.ToInt(), y.ToInt(), z_floor.ToInt());

        if (sBlockInfo0)
        {
            if (!IS_AIR_TYPE(sBlockInfo0->mSlopeType) && IS_GRADIENT_SLOPE(sBlockInfo0->mSlopeType))
            {
                *found_z = z_floor;
                Map_0x370::UpdateZFromSlopeAtCoord(x, y, *found_z);
                if (z_high >= *found_z && z <= *found_z)
                {
                    return true;
                }
            }
        }
    }
    else
    {
        sBlockInfo0 = Map_0x370::GetBlock(x.ToInt(), y.ToInt(), z_high_floor.ToInt());

        if (sBlockInfo0 && !IS_AIR_TYPE(sBlockInfo0->mSlopeType) &&
            IS_GRADIENT_SLOPE(sBlockInfo0->mSlopeType))
        {
            *found_z = z_high_floor;
            Map_0x370::UpdateZFromSlopeAtCoord(x, y, *found_z);
            if (z_high >= *found_z)
            {
                return true;
            }
        }

        sBlockInfo0 = Map_0x370::GetBlock(x.ToInt(), y.ToInt(), z_floor.ToInt());

        if (sBlockInfo0)
        {
            if (!IS_AIR_TYPE(sBlockInfo0->mSlopeType))
            {
                if (IS_GRADIENT_SLOPE(sBlockInfo0->mSlopeType))
                {
                    *found_z = z_floor;
                    Map_0x370::UpdateZFromSlopeAtCoord(x, y, *found_z);
                    if (z <= *found_z)
                    {
                        return true;
                    }
                }
                else
                {
                    *found_z = z_floor + kFpOne_6F6110;
                    return true;
                }
            }
        }
    }
    return false;
}

// Ang16::PolarToCartesian_41FC20 with the radius taken by value: the original copies value_2 into a
// temporary before the two multiplications (the by-reference version passes value_2's own slot).
static inline void __stdcall PolarToCartesianByVal(Ang16& angle, Fix16 radius, Fix16& ret1, Fix16& ret2)
{
    ret1 = Ang16::sine(angle) * radius;
    ret2 = Ang16::cosine(angle) * radius;
}

// Walks the collision probe sprite from (x_1, y_1, z_1) towards (x_2, y_2, z_2) in steps of about `height`,
// returning 0 as soon as it hits something (line of sight / clear path test).
MATCH_FUNC(0x4E5640)
bool Map_0x370::IsProbePathClear(Fix16 width, Fix16 height, Fix16 depth, Fix16 from_x, Fix16 from_y, Fix16 from_z, Fix16 to_x, Fix16 to_y, Fix16 to_z)
{
    Sprite* pProbe = gObject_5C->mCollisionProbeSprite;
    Fix16 total_dz = to_z - from_z;
    Ang16 angle;
    Fix16_Point pos_diff(to_x - from_x, to_y - from_y);

    gCollisionTarget.Reset();

    angle = Fix16::atan2_fixed(to_y - from_y, to_x - from_x);

    Fix16 total_dist;
    total_dist = pos_diff.GetLength_41E260();
    pProbe->set_xyz_lazy(from_x, from_y, from_z);
    pProbe->set_ang_lazy(angle);
    pProbe->AllocCollisionShape(width, height, depth);

    Fix16 num_steps;
    Fix16 step_len;
    Fix16 step_dz;

    if (total_dist != kFpZero_6F610C)
    {
        num_steps = total_dist / height;
        step_len = total_dist / num_steps;
        step_dz = total_dz / num_steps;
    }
    else
    {
        num_steps = kFpZero_6F610C;
        step_len = kFpZero_6F610C;
    }

    if (num_steps < kFpOne_6F6110)
    {
        num_steps = kFpOne_6F6110;
        step_len = total_dist;
        step_dz = total_dz;
    }

    Fix16 step_x;
    Fix16 step_y;

    PolarToCartesianByVal(angle, step_len, step_x, step_y);

    for (u8 i = 1; i <= num_steps.ToInt(); i++)
    {
        Fix16 ground_z;
        ground_z = gMap_0x370->FindGroundZBelowCoord(pProbe->mXy.x, pProbe->mXy.y, pProbe->mZpos);
        if (step_dz == kFpZero_6F610C)
        {
            pProbe->set_xyz_lazy(pProbe->mXy.x + step_x,
                                          pProbe->mXy.y + step_y,
                                          pProbe->mZpos + step_dz);

            if (pProbe->CheckSpriteMovementRegion())
            {
                return false;
            }
        }
        else
        {
            if (step_dz > kFpZero_6F610C)
            {
                // Going up
                pProbe->set_xyz_lazy(pProbe->mXy.x + step_x,
                                              pProbe->mXy.y + step_y,
                                              pProbe->mZpos + step_dz);
                Fix16 new_z = pProbe->mZpos;
                if (new_z > kFpSeven_6F6128)
                {
                    return false;
                }

                if (!(pProbe->mZpos < kFpSeven_6F6128) ||
                    gMap_0x370->GetBlockTypeAtCoord(pProbe->mXy.x.ToInt(),
                                                                  pProbe->mXy.y.ToInt(),
                                                                  (pProbe->mZpos + kFpHalf_6F5FE0).ToInt()))
                {
                    if (!gMap_0x370->IsGradientSlopeAt(pProbe->mXy.x.ToInt(),
                                                                     pProbe->mXy.y.ToInt(),
                                                                     pProbe->mZpos.ToInt()))
                    {
                        return false;
                    }
                }

                pProbe->set_xyz_lazy(pProbe->mXy.x, pProbe->mXy.y, new_z);

                if (pProbe->CheckSpriteMovementRegion())
                {
                    return false;
                }
            }
            else
            {
                // Going down
                pProbe->set_xyz_lazy(pProbe->mXy.x + step_x,
                                              pProbe->mXy.y + step_y,
                                              pProbe->mZpos + step_dz);

                if (gMap_0x370->IsGradientSlopeAt(pProbe->mXy.x.ToInt(),
                                                                pProbe->mXy.y.ToInt(),
                                                                pProbe->mZpos.ToInt()))
                {
                    if (gMap_0x370->GetBlockTypeAtCoord(pProbe->mXy.x.ToInt(),
                                                                      pProbe->mXy.y.ToInt(),
                                                                      pProbe->mZpos.ToInt()))
                    {
                        ground_z = gMap_0x370->FindGroundZBelowCoord(pProbe->mXy.x,
                                                                            pProbe->mXy.y,
                                                                            pProbe->mZpos);
                        if (ground_z > pProbe->mZpos)
                        {
                            return false;
                        }
                    }
                }
                else if (ground_z > pProbe->mZpos)
                {
                    pProbe->set_xyz_lazy(pProbe->mXy.x, pProbe->mXy.y, ground_z);
                    return false;
                }

                if (pProbe->CheckSpriteMovementRegion())
                {
                    return false;
                }
            }
        }
    }
    return true;
}

MATCH_FUNC(0x4E5B30)
inline gmp_map_slope::gmp_map_slope(s32 gradient_direction, s32 gradient_size, s32 gradient_level, Fix16 zpos_lower, Fix16 zpos_higher)
{
    mGradientDirection = gradient_direction;
    mGradientSize = gradient_size;
    mGradientLevel = gradient_level;
    mZposLower = zpos_lower;
    mZposHigher = zpos_higher;
}

MATCH_FUNC(0x4E5B60)
Fix16 Map_0x370::FindGroundZForCoord(Fix16 x_pos, Fix16 y_pos)
{
    s32 z_pos_int;
    gmp_block_info* pHighestBlock = Map_0x370::FindHighestBlockForCoord(x_pos.ToInt(), y_pos.ToInt(), &z_pos_int);
    sBlockInfo0 = pHighestBlock;

    if (pHighestBlock == NULL)
    {
        return Fix16(0);
    }
    else
    {
        Fix16 z_pos;
        u8 slope_byte = pHighestBlock->mSlopeType;
        if (IS_GRADIENT_SLOPE(slope_byte) && !IS_AIR_TYPE(slope_byte))
        {
            z_pos = Fix16(z_pos_int);
            Map_0x370::UpdateZFromSlopeAtCoord(x_pos, y_pos, z_pos); //  get the Z position based on the slope angle
        }
        else
        {
            z_pos = Fix16(z_pos_int) + 1;
        }
        return z_pos;
    }
}

MATCH_FUNC(0x4E5BF0)
u8 Map_0x370::UpdateZFromSlopeAtCoord(Fix16 x_pos, Fix16 y_pos, Fix16& z_pos)
{
    sBlockInfo0 = Map_0x370::GetBlock(x_pos.ToInt(), y_pos.ToInt(), z_pos.ToInt());

    if (sBlockInfo0)
    {
        Fix16 grad_pos;
        Fix16 grad_scale;

        if (!IS_AIR_TYPE(sBlockInfo0->mSlopeType))
        {
            gCurrentBlockSlope = &gGmpSlopes[GET_SLOPE_IDX(sBlockInfo0->mSlopeType)];

            switch (gCurrentBlockSlope->mGradientDirection)
            {
                case NO_GRADIENT_SLOPE_0:
                    return 0;
                case NORTH_1:
                    grad_pos = kFpOne_6F6110 - y_pos.GetFracValue();
                    break;
                case SOUTH_2:
                    grad_pos = y_pos.GetFracValue();
                    break;
                case WEST_3:
                    grad_pos = kFpOne_6F6110 - x_pos.GetFracValue();
                    break;
                case EAST_4:
                    grad_pos = x_pos.GetFracValue();
                    break;
                default:
                    break;
            }

            get_grad_scale_from_size(grad_scale, gCurrentBlockSlope->mGradientSize);

            Fix16 relative_z_pos = gCurrentBlockSlope->mZposLower + (grad_pos * grad_scale);

            if (relative_z_pos == kFpZero_6F610C)
            {
                relative_z_pos = kFpOneSixtyFourth_6F60C0;
            }
            z_pos = relative_z_pos + z_pos.GetFloorValue();
            return gCurrentBlockSlope->mGradientDirection;
        }
    }
    return 0;
}

MATCH_FUNC(0x4E5D10)
void Map_0x370::MovePosAlongDirection(Fix16* pX, Fix16* pY, Fix16 dist, s32 direction)
{
    switch (direction)
    {
        case road_direction::up_1:
            *pY -= dist;
            break;
        case road_direction::down_2:
            *pY += dist;
            break;
        case road_direction::left_4:
            *pX -= dist;
            break;
        case road_direction::right_3:
            *pX += dist;
            break;
    }
}

MATCH_FUNC(0x4E5D70)
Fix16 Map_0x370::GetLateralOffsetInBlock(Fix16 x, Fix16 y, Ang16 angle)
{
    Fix16 result = kFpZero_6F610C;
    switch (Ang16::GetAngleFace(angle))
    {
        case road_direction::up_1:
            result = x.GetFracValue();
            break;
        case road_direction::down_2:
            result = kFpOne_6F6110 - x.GetFracValue();
            break;
        case road_direction::right_3:
            result = y.GetFracValue();
            break;
        case road_direction::left_4:
            result = kFpOne_6F6110 - y.GetFracValue();
            break;
    }
    return result;
}

MATCH_FUNC(0x4E5E00)
Fix16 Map_0x370::GetForwardOffsetInBlock(Fix16 x, Fix16 y, Ang16 angle)
{
    Fix16 result = kFpZero_6F610C;
    switch (Ang16::GetAngleFace(angle))
    {
        case road_direction::up_1:
            result = y.GetFracValue();
            break;
        case road_direction::down_2:
            result = kFpOne_6F6110 - y.GetFracValue();
            break;
        case road_direction::right_3:
            result = kFpOne_6F6110 - x.GetFracValue();
            break;
        case road_direction::left_4:
            result = x.GetFracValue();
            break;
    }
    return result;
}

// https://decomp.me/scratch/9rRLR
MATCH_FUNC(0x4E5E90)
bool Map_0x370::HasGreenArrowForPathDirection(gmp_block_info* pBlock, s32 direction, bool bDefault)
{
    // No default label (return bDefault after the switch). Cases 2 and 4 are ternaries: that lets VC6
    // cross-jump their calls into the neighbouring cases' calls like the original.
    switch (direction)
    {
        case 1:
            if (bDefault)
            {
                return gRouteFinder->HasBlockDesiredArrow(pBlock, green_1, UP_1);
            }
            else
            {
                return gRouteFinder->HasBlockDesiredArrow(pBlock, green_1, DOWN_2);
            }
        case 2:
            return bDefault ? gRouteFinder->HasBlockDesiredArrow(pBlock, green_1, DOWN_2) : gRouteFinder->HasBlockDesiredArrow(pBlock, green_1, UP_1);
        case 3:
            if (bDefault)
            {
                return gRouteFinder->HasBlockDesiredArrow(pBlock, green_1, RIGHT_4);
            }
            else
            {
                return gRouteFinder->HasBlockDesiredArrow(pBlock, green_1, LEFT_3);
            }
        case 4:
            return bDefault ? gRouteFinder->HasBlockDesiredArrow(pBlock, green_1, LEFT_3) : gRouteFinder->HasBlockDesiredArrow(pBlock, green_1, RIGHT_4);
    }
    return bDefault;
}

MATCH_FUNC(0x4E5FC0)
s32 Map_0x370::GetArrowDirectionFromBlock(gmp_block_info* pBlock, bool bDontFlip)
{
    s32 result = 0;
    if (gRouteFinder->HasBlockDesiredArrow(pBlock, green_or_red_3, RIGHT_4))
    {
        if (bDontFlip)
        {
            result = road_direction::right_3;
        }
        else
        {
            result = road_direction::left_4;
        }
    }
    else if (gRouteFinder->HasBlockDesiredArrow(pBlock, green_or_red_3, DOWN_2))
    {
        if (bDontFlip)
        {
            result = road_direction::down_2;
        }
        else
        {
            result = road_direction::up_1;
        }
    }
    else if (gRouteFinder->HasBlockDesiredArrow(pBlock, green_or_red_3, LEFT_3))
    {
        if (bDontFlip)
        {
            result = road_direction::left_4;
        }
        else
        {
            result = road_direction::right_3;
        }
    }
    else if (gRouteFinder->HasBlockDesiredArrow(pBlock, green_or_red_3, UP_1))
    {
        if (bDontFlip)
        {
            result = road_direction::up_1;
        }
        else
        {
            result = road_direction::down_2;
        }
    }
    return result;
}

// https://decomp.me/scratch/RZ1Gp
MATCH_FUNC(0x4E6190)
s16 Map_0x370::ClassifyArrowOnSlope(Fix16 x, Fix16 y, Fix16 z, s32 arrow_dir, bool bReverse)
{
    if (!IsGradientSlopeAt(x.ToInt(), y.ToInt(), z.ToInt()))
    {
        return 1;
    }

    sBlockInfo0 = Map_0x370::GetBlock(x.ToInt(), y.ToInt(), z.ToInt());

    gCurrentBlockSlope = &gGmpSlopes[sBlockInfo0->mSlopeType >> 2];
    s16 result;
    bool bMatched = false;
    switch (gCurrentBlockSlope->mGradientDirection)
    {
        case NORTH_1:
            switch (arrow_dir)
            {
                case 1:
                    result = bReverse ? 2 : 3;
                    bMatched = true;
                    break;
                case 2:
                    result = bReverse ? 3 : 2;
                    bMatched = true;
                    break;
            }
            break;
        case SOUTH_2:
            switch (arrow_dir)
            {
                case 1:
                    result = bReverse ? 3 : 2;
                    bMatched = true;
                    break;
                case 2:
                    result = bReverse ? 2 : 3;
                    bMatched = true;
                    break;
            }
            break;
        case WEST_3:
            switch (arrow_dir)
            {
                case 3:
                    return bReverse ? 3 : 2;
                case 4:
                    return bReverse ? 2 : 3;
            }
            break;
        case EAST_4:
            switch (arrow_dir)
            {
                case 3:
                    return bReverse ? 2 : 3;
                case 4:
                    return bReverse ? 3 : 2;
            }
            break;
        default:
            break;
    }
    if (bMatched)
    {
        return result;
    }
    return 0;
}

MATCH_FUNC(0x4E62B0)
Fix16 Map_0x370::FloorPlusOne(Fix16 value)
{
    return kFpOne_6F6110 + value.GetFloorValue();
}

MATCH_FUNC(0x4E62D0)
gmp_block_info* Map_0x370::FindRailwayAtCoord(s32 x, s32 y, s32& found_z)
{
    gmp_col_info* pColumn = reinterpret_cast<gmp_col_info*>(&mpDmap->mpColumn[*mpDmap->get_base(y, x)]);

    for (s32 curr_z = pColumn->mHeight - pColumn->mOffset - 1; curr_z >= 0; curr_z--)
    {
        sBlockInfo0 = mpDmap->get_block(pColumn->mBlockd[curr_z]);
        if (GET_BLOCK_TYPE(sBlockInfo0->mSlopeType) == FIELD && has_any_green_arrows(sBlockInfo0->mArrows))
        {
            found_z = curr_z + pColumn->mOffset;
            return sBlockInfo0;
        }
    }
    return NULL;
}

MATCH_FUNC(0x4E6360)
gmp_block_info* Map_0x370::FindRailwayBelowZAtCoord(s32 x, s32 y, s32& z)
{
    gmp_col_info* pColumn = reinterpret_cast<gmp_col_info*>(&mpDmap->mpColumn[*mpDmap->get_base(y, x)]);

    if (z >= pColumn->mOffset)
    {
        s32 curr_z = clamp_z_from_column(pColumn->mHeight, pColumn->mOffset, z);

        for (; curr_z >= 0; curr_z--)
        {
            gmp_block_info* pBlock = mpDmap->get_block(pColumn->mBlockd[curr_z]);
            sBlockInfo0 = pBlock;

            if (GET_BLOCK_TYPE(pBlock->mSlopeType) == FIELD && has_any_green_arrows(pBlock->mArrows))
            {
                z = curr_z + pColumn->mOffset;
                return sBlockInfo0;
            }
        }
    }
    return NULL;
}

MATCH_FUNC(0x4E6400)
Fix16 Map_0x370::GetRailwayZBelowCoord(Fix16 x_pos, Fix16 y_pos, Fix16 z_pos)
{
    u8 slope_byte;
    Fix16 new_z;

    if (z_pos.GetFracValue() == kFpZero_6F610C || !IsGradientSlopeAt(x_pos.ToInt(), y_pos.ToInt(), z_pos.ToInt()) ||
        (new_z = z_pos.GetFloorValue(), Map_0x370::UpdateZFromSlopeAtCoord(x_pos, y_pos, new_z), new_z > z_pos))
    {
        s32 below_z = z_pos.ToInt() - 1;
        gmp_block_info* pBlock = Map_0x370::FindRailwayBelowZAtCoord(x_pos.ToInt(), y_pos.ToInt(), below_z);
        sBlockInfo0 = pBlock;
        if (pBlock == NULL)
        {
            return Fix16(1);
        }
        slope_byte = pBlock->mSlopeType;
        if (IS_GRADIENT_SLOPE(slope_byte) && !IS_AIR_TYPE(slope_byte))
        {
            new_z = Fix16(below_z);
            Map_0x370::UpdateZFromSlopeAtCoord(x_pos, y_pos, new_z);
        }
        else
        {
            new_z = Fix16(below_z) + 1;
        }
    }
    return new_z;
}

MATCH_FUNC(0x4E6510)
Fix16 Map_0x370::GetRailwayZCoordAtXY(Fix16 x, Fix16 y)
{
    s32 rail_z_coord;
    sBlockInfo0 = Map_0x370::FindRailwayAtCoord(x.ToInt(), y.ToInt(), rail_z_coord);

    if (!sBlockInfo0)
    {
        return Fix16(0);
    }

    Fix16 new_z;
    if (IS_GRADIENT_SLOPE(sBlockInfo0->mSlopeType) && !IS_AIR_TYPE(sBlockInfo0->mSlopeType))
    {
        new_z = Fix16(rail_z_coord);
        Map_0x370::UpdateZFromSlopeAtCoord(x, y, new_z);
    }
    else
    {
        new_z = Fix16(rail_z_coord + 1);
    }
    return new_z;
}

MATCH_FUNC(0x4E65A0)
void Map_0x370::SnapZToRailway(Fix16 x, Fix16 y, Fix16* pZ, bool bDontFlip, bool bReverse)
{
    gmp_block_info* pBlockAtPos = Map_0x370::GetBlock(x.ToInt(), y.ToInt(), (*pZ - kFpOne_6F6110).ToInt());
    if (IS_SLOPE_IN_BLOCK_ABOVE(pBlockAtPos->mSlopeType))
    {
        *pZ = FloorPlusOne(*pZ);
        pBlockAtPos = Map_0x370::GetBlock(x.ToInt(), y.ToInt(), (*pZ - kFpOne_6F6110).ToInt());
    }
    s32 arrow_dir = GetArrowDirectionFromBlock(pBlockAtPos, bDontFlip);

    if (Map_0x370::ClassifyArrowOnSlope(x, y, *pZ - kFpOne_6F6110, arrow_dir, bReverse) != 3)
    {
        *pZ = Map_0x370::GetRailwayZBelowCoord(x, y, *pZ);
    }
}

// SetRoadBlockAt_4E6660 and StepOneBlock as do/while(0) macros: their extra blocks keep
// AdvanceAlongRailway at the 100+ blocks VC6 needs to allocate its registers as the original.
#define SET_ROAD_BLOCK_AT(pBlock, x, y, z) \
    do \
    { \
        pBlock = GetBlock((x).ToInt(), (y).ToInt(), ((z) - kFpOne_6F6110).ToInt()); \
        if (IS_SLOPE_IN_BLOCK_ABOVE(pBlock->mSlopeType)) \
        { \
            pBlock = GetBlock((x).ToInt(), (y).ToInt(), (z).ToInt()); \
        } \
    } while (0)

#define STEP_ONE_BLOCK(pX, pY, dist, direction) \
    do \
    { \
        if ((dist) >= kFpOne_6F6110) \
        { \
            MovePosAlongDirection(pX, pY, kFpOne_6F6110, direction); \
            (dist) -= kFpOne_6F6110; \
        } \
        else \
        { \
            MovePosAlongDirection(pX, pY, dist, direction); \
            (dist) = kFpZero_6F610C; \
        } \
    } while (0)

MATCH_FUNC(0x4E6660)
s32 Map_0x370::AdvanceAlongRailway(Fix16* pX, Fix16* pY, Fix16* pZ, Fix16 dist)
{
    bool bTurned = false;
    Fix16 x = *pX;
    Fix16 y = *pY;
    Fix16 to_centre;

    Fix16 z = *pZ;
    SnapZToRailway(x, y, &z, true, false);
    gmp_block_info* pBlock;
    SET_ROAD_BLOCK_AT(pBlock, x, y, z);
    s32 direction = GetArrowDirectionFromBlock(pBlock, true);

    Fix16 side;
    side = GetLateralOffsetInBlock(x, y, ReturnAngleFromRoadDirection(direction));
    if (side > kFpHalf_6F5F18)
    {
        switch (direction)
        {
            case road_direction::up_1:
                direction = road_direction::left_4;
                break;
            case road_direction::down_2:
                direction = road_direction::right_3;
                break;
            case road_direction::left_4:
                direction = road_direction::down_2;
                break;
            case road_direction::right_3:
                direction = road_direction::up_1;
                break;
        }
    }
    else if (side < kFpHalf_6F5F18)
    {
        switch (direction)
        {
            case road_direction::up_1:
                direction = road_direction::right_3;
                break;
            case road_direction::down_2:
                direction = road_direction::left_4;
                break;
            case road_direction::left_4:
                direction = road_direction::up_1;
                break;
            case road_direction::right_3:
                direction = road_direction::down_2;
                break;
        }
    }

    gmp_block_info* pPrev;
    if (side != kFpHalf_6F5F18)
    {
        to_centre = GetForwardOffsetInBlock(x, y, ReturnAngleFromRoadDirection(direction)) - kFpHalf_6F5F18;
        if (to_centre > kFpZero_6F610C)
        {
            if (dist >= to_centre)
            {
                MovePosAlongDirection(&x, &y, to_centre, direction);
                dist -= to_centre;
                direction = GetArrowDirectionFromBlock(pBlock, true);
                if (dist >= kFpHalf_6F5F18)
                {
                    pPrev = pBlock;
                    MovePosAlongDirection(&x, &y, kFpHalf_6F5F18, direction);
                    dist -= kFpHalf_6F5F18;
                    gmp_block_info* pNext = GetRoadBlockAt(x, y, z);
                    if (!pNext || !HasGreenArrowForPathDirection(pNext, direction, true))
                    {
                        STEP_ONE_BLOCK(&x, &y, dist, direction);
                    }
                    SET_ROAD_BLOCK_AT(pBlock, x, y, z);
                    if (pBlock != pPrev)
                    {
                        SnapZToRailway(x, y, &z, true, false);
                        SET_ROAD_BLOCK_AT(pBlock, x, y, z);
                    }
                }
                else
                {
                    MovePosAlongDirection(&x, &y, dist, direction);
                    // The original skips the loop and jumps straight to the shared exit
                    goto done;
                }
            }
            else
            {
                MovePosAlongDirection(&x, &y, dist, direction);
                goto done;
            }
        }
    }
    else
    {
        Fix16 to_edge;
        to_edge = GetForwardOffsetInBlock(x, y, ReturnAngleFromRoadDirection(direction));
        if (to_edge > kFpZero_6F610C)
        {
            SnapZToRailway(x, y, &z, true, true);
            pPrev = pBlock;
            MovePosAlongDirection(&x,
                       &y,
                       kFpOne_6F6110 - to_edge,
                       Ang16::GetAngleFace(
                           Ang16(ReturnAngleFromRoadDirection(direction).rValue + kAng180_6F603E.rValue).Normalized()));
            dist += kFpOne_6F6110 - to_edge;
            gmp_block_info* pNext = GetRoadBlockAtOrNull(x, y, z);
            if (!pNext || !HasGreenArrowForPathDirection(pNext, direction, true))
            {
                STEP_ONE_BLOCK(&x, &y, dist, direction);
            }
            SET_ROAD_BLOCK_AT(pBlock, x, y, z);
            if (pBlock != pPrev)
            {
                SnapZToRailway(x, y, &z, true, false);
                SET_ROAD_BLOCK_AT(pBlock, x, y, z);
            }
            else
            {
                SnapZToRailway(x, y, &z, true, false);
            }
        }
    }

    while (dist != kFpZero_6F610C)
    {
        if (HasGreenArrowForPathDirection(pBlock, direction, true) && !bTurned)
        {
            pPrev = pBlock;
            STEP_ONE_BLOCK(&x, &y, dist, direction);
            SET_ROAD_BLOCK_AT(pBlock, x, y, z);
            if (pBlock != pPrev)
            {
                SnapZToRailway(x, y, &z, true, false);
                SET_ROAD_BLOCK_AT(pBlock, x, y, z);
            }
            if (!HasGreenArrowForPathDirection(pBlock, direction, true))
            {
                pPrev = pBlock;
                Fix16 to_edge;
                to_edge = GetForwardOffsetInBlock(x, y, ReturnAngleFromRoadDirection(direction));
                MovePosAlongDirection(&x,
                           &y,
                           kFpOne_6F6110 - to_edge,
                           Ang16::GetAngleFace(
                               Ang16(ReturnAngleFromRoadDirection(direction).rValue + kAng180_6F603E.rValue).Normalized()));
                dist += kFpOne_6F6110 - to_edge;
                SET_ROAD_BLOCK_AT(pBlock, x, y, z);
                if (pBlock != pPrev)
                {
                    SnapZToRailway(x, y, &z, true, false);
                    SET_ROAD_BLOCK_AT(pBlock, x, y, z);
                }
                bTurned = true;
            }
        }
        else
        {
            bTurned = false;
            pPrev = pBlock;
            if (dist >= kFpOne_6F6110)
            {
                MovePosAlongDirection(&x, &y, kFpHalf_6F5F18, direction);
                direction = GetArrowDirectionFromBlock(GetRoadBlockAt(x, y, z), true);
                MovePosAlongDirection(&x, &y, kFpHalf_6F5F18, direction);
                dist -= kFpOne_6F6110;
                SET_ROAD_BLOCK_AT(pBlock, x, y, z);
                if (pBlock != pPrev)
                {
                    SnapZToRailway(x, y, &z, true, false);
                    SET_ROAD_BLOCK_AT(pBlock, x, y, z);
                }
            }
            else
            {
                if (dist > kFpHalf_6F5F18)
                {
                    MovePosAlongDirection(&x, &y, kFpHalf_6F5F18, direction);
                    SET_ROAD_BLOCK_AT(pBlock, x, y, z);
                    direction = GetArrowDirectionFromBlock(pBlock, true);
                    dist -= kFpHalf_6F5F18;
                }
                MovePosAlongDirection(&x, &y, dist, direction);
                dist = 0;
                if (pBlock != pPrev)
                {
                    SnapZToRailway(x, y, &z, true, false);
                    SET_ROAD_BLOCK_AT(pBlock, x, y, z);
                }
            }
        }
    }

done:
    *pX = x;
    *pY = y;
    *pZ = GetRailwayZBelowCoord(x, y, z);
    return direction;
}

MATCH_FUNC(0x4E7190)
s32 Map_0x370::AdvanceAlongRailwayChecked(Fix16* pX, Fix16* pY, Fix16* pZ, Fix16 dist)
{
    bool bTurned = false;
    Fix16 x = *pX;
    Fix16 y = *pY;
    Fix16 z = *pZ;
    Fix16 last_x = x;
    Fix16 last_y = y;
    Fix16 last_z = z;

    SnapZToRailway(x, y, &z, false, false);
    gmp_block_info* pBlock;
    SetRoadBlockAt_4E6660(pBlock, x, y, z);
    s32 direction = GetArrowDirectionFromBlock(pBlock, false);

    Fix16 side;
    side = GetLateralOffsetInBlock(x, y, ReturnAngleFromRoadDirection(direction));
    // The case order of the two switches picks which constants VC6 keeps in registers (`mov $1,%ebp; mov $3,%ebx`)
    if (side > kFpHalf_6F5F18)
    {
        switch (direction)
        {
            case road_direction::up_1:
                direction = road_direction::right_3;
                break;
            case road_direction::down_2:
                direction = road_direction::left_4;
                break;
            case road_direction::left_4:
                direction = road_direction::up_1;
                break;
            case road_direction::right_3:
                direction = road_direction::down_2;
                break;
        }
    }
    else if (side < kFpHalf_6F5F18)
    {
        switch (direction)
        {
            case road_direction::up_1:
                direction = road_direction::left_4;
                break;
            case road_direction::down_2:
                direction = road_direction::right_3;
                break;
            case road_direction::left_4:
                direction = road_direction::down_2;
                break;
            case road_direction::right_3:
                direction = road_direction::up_1;
                break;
        }
    }

    gmp_block_info* pPrev;
    if (side != kFpHalf_6F5F18)
    {
        pPrev = pBlock;
        Fix16 to_centre;
        to_centre = GetForwardOffsetInBlock(x, y, ReturnAngleFromRoadDirection(direction));
        if (to_centre > kFpZero_6F610C)
        {
            if (dist >= to_centre)
            {
                MovePosAlongDirection(&x, &y, to_centre, direction);
                dist -= to_centre;
            }
            else
            {
                MovePosAlongDirection(&x, &y, dist, direction);
                dist = kFpZero_6F610C;
            }
        }
        if (!HasGreenArrowForPathDirection(GetRoadBlockAt(x, y, z), direction, false))
        {
            StepOneBlock(&x, &y, dist, direction);
        }
        SetRoadBlockAt_4E6660(pBlock, x, y, z);
        if (pBlock != pPrev)
        {
            SnapZToRailway(x, y, &z, false, false);
            SetRoadBlockAt_4E6660(pBlock, x, y, z);
        }
    }
    else
    {
        Fix16 to_edge;
        to_edge = GetForwardOffsetInBlock(x, y, ReturnAngleFromRoadDirection(direction));
        if (to_edge > kFpZero_6F610C)
        {
            SnapZToRailway(x, y, &z, false, true);
            pPrev = pBlock;
            MovePosAlongDirection(&x,
                       &y,
                       kFpOne_6F6110 - to_edge,
                       Ang16::GetAngleFace(ReturnAngleFromRoadDirection(direction) + kAng180_6F603E));
            dist += kFpOne_6F6110 - to_edge;
            gmp_block_info* pNext = GetRoadBlockAtOrNull(x, y, z);
            if (!pNext || !HasGreenArrowForPathDirection(pNext, direction, false))
            {
                StepOneBlock(&x, &y, dist, direction);
            }
            SetRoadBlockAt_4E6660(pBlock, x, y, z);
            if (pBlock != pPrev)
            {
                SnapZToRailway(x, y, &z, false, false);
                SetRoadBlockAt_4E6660(pBlock, x, y, z);
            }
            else
            {
                SnapZToRailway(x, y, &z, false, false);
            }
        }
    }

    while (dist != kFpZero_6F610C)
    {
        if (HasGreenArrowForPathDirection(pBlock, direction, false) && !bTurned)
        {
            // pPrev first: the last_z store is scheduled after the pPrev copy
            pPrev = pBlock;
            last_x = x;
            last_y = y;
            last_z = z;
            StepOneBlock(&x, &y, dist, direction);
            // Not SetRoadBlockAtOrNull_4E7190: when the first lookup finds no block the original goes straight
            // to the not-green branch (`test %esi; je`), skipping the pBlock != pPrev step
            pBlock = GetBlock(x.ToInt(), y.ToInt(), (z - kFpOne_6F6110).ToInt());
            if (pBlock)
            {
                if (IS_SLOPE_IN_BLOCK_ABOVE(pBlock->mSlopeType))
                {
                    pBlock = GetBlock(x.ToInt(), y.ToInt(), z.ToInt());
                }
                if (pBlock != pPrev)
                {
                    SnapZToRailway(x, y, &z, false, false);
                    SetRoadBlockAtOrNull_4E7190(pBlock, x, y, z);
                }
            }
            if (!pBlock || !HasGreenArrowForPathDirection(pBlock, direction, false))
            {
                pPrev = pBlock;
                Fix16 to_edge;
                to_edge = GetForwardOffsetInBlock(x, y, ReturnAngleFromRoadDirection(direction));
                MovePosAlongDirection(&x,
                           &y,
                           kFpOne_6F6110,
                           Ang16::GetAngleFace(ReturnAngleFromRoadDirection(direction) + kAng180_6F603E));
                MovePosAlongDirection(&x,
                           &y,
                           kFpOne_6F6110 - to_edge,
                           Ang16::GetAngleFace(ReturnAngleFromRoadDirection(direction) + kAng180_6F603E));
                dist += kFpOne_6F6110 + kFpOne_6F6110 - to_edge;
                z = last_z;
                SetRoadBlockAt_4E6660(pBlock, x, y, z);
                if (pBlock != pPrev)
                {
                    SnapZToRailway(x, y, &z, false, false);
                    SetRoadBlockAt_4E6660(pBlock, x, y, z);
                }
                bTurned = true;
            }
        }
        else
        {
            s32 new_direction = 0;
            bTurned = false;
            switch (direction)
            {
                case road_direction::up_1:
                case road_direction::down_2:
                    pBlock = gMap_0x370->GetBlock((last_x + kFpOne_6F6110).ToInt(),
                                                                 last_y.ToInt(),
                                                                 (last_z - kFpOne_6F6110).ToInt());
                    if (pBlock)
                    {
                        new_direction = GetArrowDirectionFromBlock(pBlock, false);
                        if (new_direction != road_direction::right_3)
                        {
                            new_direction = 0;
                        }
                    }
                    if (!pBlock || !new_direction)
                    {
                        pBlock = gMap_0x370->GetBlock((last_x - kFpOne_6F6110).ToInt(),
                                                                     last_y.ToInt(),
                                                                     (last_z - kFpOne_6F6110).ToInt());
                        new_direction = GetArrowDirectionFromBlock(pBlock, false);
                    }
                    break;
                case road_direction::right_3:
                case road_direction::left_4:
                    pBlock = gMap_0x370->GetBlock(last_x.ToInt(),
                                                                 (last_y + kFpOne_6F6110).ToInt(),
                                                                 (last_z - kFpOne_6F6110).ToInt());
                    if (pBlock)
                    {
                        new_direction = GetArrowDirectionFromBlock(pBlock, false);
                        if (new_direction != road_direction::down_2)
                        {
                            new_direction = 0;
                        }
                    }
                    if (!pBlock || !new_direction)
                    {
                        pBlock = gMap_0x370->GetBlock(last_x.ToInt(),
                                                                     (last_y - kFpOne_6F6110).ToInt(),
                                                                     (last_z - kFpOne_6F6110).ToInt());
                        new_direction = GetArrowDirectionFromBlock(pBlock, false);
                    }
                    break;
            }

            // `pPrev = pBlock` in both arms: FlowOpts hoists it back in front of the `jl`, but the slot list
            // (built before that) counts both stores, which puts pPrev's slot ahead of last_x/last_y
            if (dist >= kFpOne_6F6110)
            {
                pPrev = pBlock;
                MovePosAlongDirection(&x, &y, kFpHalf_6F5F18, direction);
                GetRoadBlockAt(x, y, z);
                direction = new_direction;
                MovePosAlongDirection(&x, &y, kFpHalf_6F5F18, direction);
                dist -= kFpOne_6F6110;
                StepOneBlock(&x, &y, dist, direction);
                SetRoadBlockAt_4E6660(pBlock, x, y, z);
                if (pBlock != pPrev)
                {
                    SnapZToRailway(x, y, &z, false, false);
                    SetRoadBlockAt_4E6660(pBlock, x, y, z);
                }
            }
            else
            {
                pPrev = pBlock;
                if (dist > kFpHalf_6F5F18)
                {
                    MovePosAlongDirection(&x, &y, kFpHalf_6F5F18, direction);
                    SetRoadBlockAt_4E6660(pBlock, x, y, z);
                    direction = new_direction;
                    dist -= kFpHalf_6F5F18;
                }
                MovePosAlongDirection(&x, &y, dist, direction);
                dist = 0;
                if (pBlock != pPrev)
                {
                    SnapZToRailway(x, y, &z, false, false);
                    SetRoadBlockAt_4E6660(pBlock, x, y, z);
                }
            }
        }
    }

    *pX = x;
    *pY = y;
    *pZ = GetRailwayZBelowCoord(x, y, z);

    // 9.6f: GetOppositeDirection reverses the direction (inlined, using it as a helper changes the code: 103 -> 301)
    switch (direction)
    {
        case road_direction::up_1:
            return road_direction::down_2;
        case road_direction::down_2:
            return road_direction::up_1;
        case road_direction::left_4:
            return road_direction::right_3;
        case road_direction::right_3:
            return road_direction::left_4;
    }
    return 0;
}

MATCH_FUNC(0x4E7E90)
bool Map_0x370::FindFirstPavementCoord(u8* pX, char_type* pY)
{
    for (u8 x = 1; x < 255; x++)
    {
        for (u8 y = 1; y < 255; y++)
        {
            s32 z;
            if (FindPavementBlockForCoord(x, y, z))
            {
                *pX = x;
                *pY = y;
                return true;
            }
        }
    }
    return false;
}

MATCH_FUNC(0x4E7FC0)
bool Map_0x370::CheckColumnHasSolidAbove(Fix16 x, Fix16 y, Fix16 z)
{
    gmp_compressed_map_32* pDMap = this->mpDmap;

    s32 z_int = z.ToInt();
    gmp_col_info* pColInfo = reinterpret_cast<gmp_col_info*>(&pDMap->mpColumn[*pDMap->get_base(y.ToInt(), x.ToInt())]);
    if (z_int < pColInfo->mHeight)
    {
        if (z_int < pColInfo->mOffset)
        {
            z_int = pColInfo->mOffset;
        }

        s32 block_d_idx = z_int - pColInfo->mOffset;
        s32 block_idx = pColInfo->mBlockd[block_d_idx];

        if (pDMap->mBlock[block_idx].mLid)
        {
            if ((pDMap->mBlock[block_idx].mLid & block_face::flat) == 0)
            {
                u8 slope_type = pDMap->mBlock[block_idx].mSlopeType;
                u8 slope_type_masked = GET_SLOPE_BITS(slope_type);
                if ((GET_SLOPE_BITS(slope_type) < DIAGONAL_WALL_UP_LEFT || slope_type_masked > PARTIAL_CENTRE_BLOCK) &&
                    (!IS_GRADIENT_SLOPE(slope_type) || IS_AIR_TYPE(slope_type)))
                {
                    return true;
                }
            }
        }

        block_d_idx++;

        s32 remainder = pColInfo->mHeight - pColInfo->mOffset;
        while (block_d_idx < remainder)
        {
            s32 idx = pColInfo->mBlockd[block_d_idx];
            if (pDMap->mBlock[idx].mLid)
            {
                if ((pDMap->mBlock[idx].mLid & block_face::flat) == 0)
                {
                    u8 slope_type_masked_ = GET_SLOPE_BITS(pDMap->mBlock[idx].mSlopeType);
                    if (slope_type_masked_ < DIAGONAL_WALL_UP_LEFT || slope_type_masked_ > PARTIAL_CENTRE_BLOCK)
                    {
                        return true;
                    }
                }
            }
            ++block_d_idx;
        }
    }
    return false;
}


MATCH_FUNC(0x4E80A0)
void gmp_compressed_map_32::ApplyDmapInfos(DmapInfoList* pMapSub)
{
    u32 j = 0;
    for (u32 i = 0; i < static_cast<u32>(pMapSub->mMaxIdx); i++, j++)
    {
        u32 x = pMapSub->mDmapInfos[j].mX;
        u32 y = pMapSub->mDmapInfos[j].mY;
        *get_base(y, x) = pMapSub->mDmapInfos[j].mColumnIdx;
    }
}

MATCH_FUNC(0x4E80E0)
void DmapInfoList::AddOrUpdateDmapInfo(u8 x, u8 y, u32 column_idx)
{
    u32 local_max_idx = mMaxIdx;
    gmp_dmap_info* pInfo = &mDmapInfos[0];

    for (u32 i = 0; i < local_max_idx; i++, pInfo++)
    {
        if (pInfo->mX == x && pInfo->mY == y)
        {
            pInfo->mColumnIdx = column_idx;
            return;
        }
    }

    mDmapInfos[local_max_idx].set_data(x, y, column_idx);
    ++mMaxIdx;
}

MATCH_FUNC(0x4E8140)
s32 Map_0x370::AddBlock(gmp_block_info* pBlockInfo)
{
    s32 num_blocks = this->mNumBlocks354;
    this->mNumBlocks354 = num_blocks + 1;
    this->mpDmap->mBlock[num_blocks] = *pBlockInfo;
    return num_blocks;
}

MATCH_FUNC(0x4E8180)
s32 Map_0x370::CloneOriginalBlock(u32 read_block_idx)
{
    if (read_block_idx < static_cast<u32>(this->mNumBlocks34C))
    {
        s32 old_num_blocks = this->mNumBlocks354;
        this->mNumBlocks354 = old_num_blocks + 1;
        mpDmap->mBlock[old_num_blocks] = mpDmap->mBlock[read_block_idx];
        return old_num_blocks;
    }
    return read_block_idx;
}

MATCH_FUNC(0x4E81D0)
s32 Map_0x370::CloneOriginalColumn(u32 column_idx)
{
    s32 idx = column_idx;

    if (column_idx < static_cast<u32>(mColumnWords358))
    {
        gmp_col_info* pColumn = reinterpret_cast<gmp_col_info*>(&mpDmap->mpColumn[column_idx]);
        s32 len = pColumn->mHeight - pColumn->mOffset + 1;
        idx = mColumnWords360;
        mColumnWords360 += len;
        memcpy(&mpDmap->mpColumn[idx], pColumn, 4 * len);
    }
    return idx;
}

MATCH_FUNC(0x4E8220)
s32 Map_0x370::CloneColumnExtendedToZ(u32 column_idx, s32 z)
{
    gmp_col_info* pColumn = reinterpret_cast<gmp_col_info*>(&mpDmap->mpColumn[column_idx]);

    if (z < pColumn->mHeight && z >= pColumn->mOffset)
    {
        return Map_0x370::CloneOriginalColumn(column_idx);
    }

    gmp_col_info* pNewColumn = reinterpret_cast<gmp_col_info*>(&mpDmap->mpColumn[mColumnWords360]);
    s32 local_column_words = mColumnWords360;

    if (z >= pColumn->mHeight)
    {
        pNewColumn->mHeight = z + 1;
        pNewColumn->mOffset = pColumn->mOffset;

        for (s32 i = 0; i < pColumn->mHeight - pColumn->mOffset; i++)
        {
            pNewColumn->mBlockd[i] = pColumn->mBlockd[i];
        }

        for (s32 j = pColumn->mHeight - pColumn->mOffset; j < pNewColumn->mHeight - pNewColumn->mOffset; j++)
        {
            pNewColumn->mBlockd[j] = 0;
        }
    }
    else
    {
        pNewColumn->mHeight = pColumn->mHeight;
        pNewColumn->mOffset = z;

        s32 height_diff = pColumn->mOffset - static_cast<u8>(z);

        for (s32 j = 0; j < height_diff; j++)
        {
            pNewColumn->mBlockd[j] = 0;
        }

        for (s32 k = 0; k < pColumn->mHeight - pColumn->mOffset; k++)
        {
            pNewColumn->mBlockd[k + height_diff] = pColumn->mBlockd[k];
        }
    }
    mColumnWords360 += pNewColumn->mHeight - pNewColumn->mOffset + 1;
    return local_column_words;
}

MATCH_FUNC(0x4E8370)
s32 Map_0x370::CloneColumnWithoutBlock(u32 column_idx, s32 z, char_type bDropAbove)
{
    s32 offset;
    gmp_col_info* pColumn = reinterpret_cast<gmp_col_info*>(&mpDmap->mpColumn[column_idx]);
    s32 height = pColumn->mHeight;
    if (z >= height || z < (offset = pColumn->mOffset))
    {
        return -1;
    }

    if (column_idx < static_cast<u32>(mColumnWords358))
    {
        // new_idx/pNew are computed once before the bDropAbove test (that gives the original's load order ahead of
        // `mov bDropAbove,%al; test`); the bDropAbove arms each update field_360 and join at one return (see below).
        s32 new_idx;
        // The copied height reads through a reference: with `pColumn->mHeight` directly, both VC6
        // and VC7 load the two operands of every `height - offset` loop bound below in the other order.
        const u8& new_height = pColumn->mHeight;
        gmp_col_info* pNew;
        new_idx = mColumnWords360;
        pNew = reinterpret_cast<gmp_col_info*>(&mpDmap->mpColumn[new_idx]);
        if (bDropAbove)
        {
            if (z == height - 1)
            {
                pNew->mHeight = new_height - 1;
                pNew->mOffset = pColumn->mOffset;
                for (s32 i = 0; i < pNew->mHeight - pNew->mOffset; i++)
                {
                    pNew->mBlockd[i] = pColumn->mBlockd[i];
                }
            }
            else
            {
                s32 i;
                pNew->mHeight = new_height - 1;
                pNew->mOffset = pColumn->mOffset;
                for (i = 0; i < z - pNew->mOffset; i++)
                {
                    pNew->mBlockd[i] = pColumn->mBlockd[i];
                }
                for (i = z - pNew->mOffset; i < pNew->mHeight - pNew->mOffset; i++)
                {
                    pNew->mBlockd[i] = pColumn->mBlockd[i + 1];
                }
            }
            mColumnWords360 += pNew->mHeight - pNew->mOffset + 1;
        }
        else
        {
            if (z == height - 1)
            {
                pNew->mHeight = new_height - 1;
                pNew->mOffset = pColumn->mOffset;
                for (s32 i = 0; i < pNew->mHeight - pNew->mOffset; i++)
                {
                    pNew->mBlockd[i] = pColumn->mBlockd[i];
                }
            }
            else if (z == offset)
            {
                pNew->mHeight = new_height - 1;
                pNew->mOffset = pColumn->mOffset + 1;
                for (s32 i = 0; i < pNew->mHeight - pNew->mOffset; i++)
                {
                    pNew->mBlockd[i] = pColumn->mBlockd[i + 1];
                }
            }
            else
            {
                pNew->mHeight = new_height;
                pNew->mOffset = pColumn->mOffset;
                for (s32 i = 0; i < pNew->mHeight - pNew->mOffset; i++)
                {
                    pNew->mBlockd[i] = pColumn->mBlockd[i];
                }
                pNew->mBlockd[z - pColumn->mOffset] = 0;
            }
            mColumnWords360 += pNew->mHeight - pNew->mOffset + 1;
        }
        // Two returns of new_idx (folded into one): their two eax copy hints give the original's register
        // allocation. One plain return colours eax/ecx the other way round; a return at the end of each arm
        // gives the right allocation but its two tails don't cross-jump (FlowOpts cost check: exactly 20).
        if (bDropAbove)
        {
            return new_idx;
        }
        return new_idx;
    }

    if (bDropAbove)
    {
        for (s32 i = z - offset; i < pColumn->mHeight - pColumn->mOffset - 1; i++)
        {
            pColumn->mBlockd[i] = pColumn->mBlockd[i + 1];
        }
        pColumn->mHeight--;
    }
    else if (z == height - 1)
    {
        pColumn->mHeight--;
    }
    else
    {
        pColumn->mBlockd[z - offset] = 0;
    }
    return column_idx;
}

MATCH_FUNC(0x4E8620)
void Map_0x370::ChangeBlock(s32 x, s32 y, s32 z, s32 info_type_to_set, u16 info_value)
{
    s32 column_idx = Map_0x370::CloneOriginalColumn(*mpDmap->get_base(y, x));
    *mpDmap->get_base(y, x) = column_idx;
    mObj.AddOrUpdateDmapInfo(x, y, column_idx);

    gmp_col_info* pColumn = reinterpret_cast<gmp_col_info*>(&this->mpDmap->mpColumn[column_idx]);

    s32 block_idx = Map_0x370::CloneOriginalBlock(pColumn->mBlockd[z - pColumn->mOffset]);

    pColumn->mBlockd[z - pColumn->mOffset] = block_idx;
    sBlockInfo0 = &this->mpDmap->mBlock[block_idx];

    switch (info_type_to_set)
    {
        case 1:
            sBlockInfo0->mLeft = info_value;
            break;
        case 2:
            sBlockInfo0->mRight = info_value;
            break;
        case 3:
            sBlockInfo0->mTop = info_value;
            break;
        case 4:
            sBlockInfo0->mBottom = info_value;
            break;
        case 5:
            sBlockInfo0->mLid = info_value;
            break;
        case 6:
            sBlockInfo0->mSlopeType = static_cast<u8>(info_value);
            break;
        case 7:
            sBlockInfo0->mArrows = static_cast<u8>(info_value);
            break;
        default:
            return;
    }
}

MATCH_FUNC(0x4E87C0)
void Map_0x370::AddNewBlock(s32 x, s32 y, s32 z, gmp_block_info* pBlockData)
{
    s32 column_idx = Map_0x370::CloneColumnExtendedToZ(*mpDmap->get_base(y, x), z);
    *mpDmap->get_base(y, x) = column_idx;
    mObj.AddOrUpdateDmapInfo(x, y, column_idx);
    gmp_col_info* pColumn = reinterpret_cast<gmp_col_info*>(&mpDmap->mpColumn[column_idx]);
    u32 block_id = pColumn->mBlockd[z - pColumn->mOffset];

    if (block_id < static_cast<u32>(mNumBlocks34C))
    {
        pColumn->mBlockd[z - pColumn->mOffset] = Map_0x370::AddBlock(pBlockData);
    }
    else
    {
        mpDmap->mBlock[block_id] = *pBlockData;
    }
}

MATCH_FUNC(0x4E8940)
void Map_0x370::RemoveBlock(s32 x_pos, s32 y_pos, s32 offset, char_type do_drop)
{
    const s32 column_idx = CloneColumnWithoutBlock(*mpDmap->get_base(y_pos, x_pos), offset, do_drop);
    if (column_idx != -1)
    {
        *mpDmap->get_base(y_pos, x_pos) = column_idx;
        mObj.AddOrUpdateDmapInfo(x_pos, y_pos, column_idx);
    }
}

MATCH_FUNC(0x4E8A10)
void Map_0x370::LowerColumn(s32 x_pos, s32 y_pos)
{
    u32 column_idx = *mpDmap->get_base(y_pos, x_pos);
    gmp_col_info* pColumn = reinterpret_cast<gmp_col_info*>(&mpDmap->mpColumn[column_idx]);
    if (pColumn->mOffset == pColumn->mHeight - 1 && pColumn->mOffset)
    {
        if (column_idx < static_cast<u32>(mColumnWords358))
        {
            // Note: decrements the new column's height, not a copy of pColumn's
            gmp_col_info* pNew = reinterpret_cast<gmp_col_info*>(&mpDmap->mpColumn[mColumnWords360]);
            pNew->mOffset = pColumn->mOffset - 1;
            pNew->mHeight--;
            pNew->mBlockd[0] = pColumn->mBlockd[0];
            mColumnWords360 += 2;
        }
        else
        {
            pColumn->mOffset--;
            pColumn->mHeight--;
        }
    }
    else
    {
        RemoveBlock(x_pos, y_pos, pColumn->mOffset, 1);
    }
}

MATCH_FUNC(0x4E8B70)
void Map_0x370::LowerLevel(s32 x_min, s32 x_max, s32 y_min, s32 y_max)
{
    for (s32 column_y = y_min; column_y <= y_max; ++column_y)
    {
        for (s32 column_x = x_min; column_x <= x_max; ++column_x)
        {
            Map_0x370::LowerColumn(column_x, column_y);
        }
    }
}

MATCH_FUNC(0x4E8C00)
void Map_0x370::OnModifiedMapDataLoaded(u32 column_bytes, u32 block_bytes, u32 obj_bytes)
{
    this->mColumnWords360 = (column_bytes >> 2) + this->mpDmap->mColumnWords;
    this->mNumBlocks354 = block_bytes / 0xC + mpDmap->mNumBlocks;
    this->mObj.mMaxIdx = obj_bytes >> 3;
    mpDmap->ApplyDmapInfos(&mObj);
}

MATCH_FUNC(0x4E8CF0)
void Map_0x370::GetModifiedMapData(u16*** outColumnPtr,
                           u32* outColumnBytes,
                           gmp_block_info** outBlockPtr,
                           u32* outBlockBytes,
                           DmapInfoList** outSubObj,
                           s32* outSubObjBytes)
{
    *outColumnPtr = &this->mpDmap->mpColumn[this->mpDmap->mColumnWords];

    const s32 totalColumnWords = this->mColumnWords360;
    const s32 consumedColumnWords = this->mpDmap->mColumnWords;
    *outColumnBytes = 4 * (totalColumnWords - consumedColumnWords);

    *outBlockPtr = &this->mpDmap->mBlock[this->mpDmap->mNumBlocks];

    const s32 totalBlocks = this->mNumBlocks354;
    const s32 consumedBlocks = this->mpDmap->mNumBlocks;
    *outBlockBytes = 12 * (totalBlocks - consumedBlocks);

    *outSubObj = &this->mObj;

    *outSubObjBytes = 8 * this->mObj.mMaxIdx;
}

// https://decomp.me/scratch/eGx1i
MATCH_FUNC(0x4E8E30)
void Map_0x370::do_process_loaded_zone_data()
{
    u16 zone_count = 0;
    // The original reads the zone count before the null test.
    u16 num_zones = mpZones->mNumZones;
    u16 zonesSize = mpZoneData ? num_zones : 0;
    if (zonesSize)
    {
        mpZoneArray = static_cast<u8*>(Memory::Checked_malloc(zonesSize));
    }

    for (u16 zoneIdx = 0; zoneIdx < zonesSize; zoneIdx++)
    {
        gmp_map_zone* zone1 = GetZone(zoneIdx);
        if (zone1->mZoneType != Information_10 && zone1->mZoneType != Navigation_1 &&
            zone1->mZoneType != Local_Navigation_15)
        {
            mpZoneArray[zoneIdx] = 0;
            continue;
        }

        bool done = false;
        for (u16 src_idx = 0; src_idx < zoneIdx; src_idx++)
        {
            const char zone_char = mpZoneArray[src_idx];
            if (zone_char)
            {
                gmp_map_zone* zone2 = GetZone(src_idx);
                if (zone1->mNameLength == zone2->mNameLength &&
                    !strncmp(zone1->mName, zone2->mName, zone1->mNameLength))
                {
                    mpZoneArray[zoneIdx] = zone_char;
                    done = true;
                    break;
                }
            }
        }

        if (!done)
        {
            mpZoneArray[zoneIdx] = static_cast<u8>(++zone_count);
        }
    }

    s32 zoneCount = zone_count + 1;
    gmp_zone_info* pZoneInfo = static_cast<gmp_zone_info*>(Memory::Checked_malloc(sizeof(gmp_zone_info) * zoneCount));
    mpZoneInfo = pZoneInfo;
    s32 t = 0;
    do
    {

        pZoneInfo = &mpZoneInfo[t];
        pZoneInfo->mCarDensity = 500;
        pZoneInfo->mGoodcarRatio = 300;
        pZoneInfo->mBadcarRatio = 300;
        pZoneInfo->mPolicecarRatio = 100;
        pZoneInfo->mGangcarRatio = 0;
        pZoneInfo->mPedDensity = 500;
        pZoneInfo->mMuggerRatio = 50;
        pZoneInfo->mCarthiefRatio = 40;
        pZoneInfo->mElvisRatio = 10;
        pZoneInfo->mGangcharRatio = 200;
        pZoneInfo->mPolicepedRatio = 40;
        t++;
        zoneCount--;
    } while (zoneCount != 0);
}

MATCH_FUNC(0x4E90E0)
void Map_0x370::build_zone_list(u32 chunk_size)
{
    u32 in_use_size = 0;
    s32 num_zones = 0;
    s32 zone_data_size;

    for (gmp_map_zone* pZone = this->mpZoneData; in_use_size < chunk_size; ++num_zones)
    {
        zone_data_size = pZone->mNameLength + 6;
        in_use_size += zone_data_size;
        pZone = reinterpret_cast<gmp_map_zone*>(reinterpret_cast<u8*>(pZone) + zone_data_size);
    }
    mpZones = static_cast<gmp_zone_list*>(Memory::Checked_malloc(4 * num_zones + 4));

    in_use_size = 0;
    mpZones->mNumZones = num_zones;

    gmp_map_zone* pZoneIter = this->mpZoneData;
    for (u32 zone_idx = 0; in_use_size < chunk_size; zone_idx++)
    {
        mpZones->mZoneArray[zone_idx] = pZoneIter;
        zone_data_size = pZoneIter->mNameLength + 6;
        in_use_size += zone_data_size;
        pZoneIter = reinterpret_cast<gmp_map_zone*>(reinterpret_cast<u8*>(pZoneIter) + zone_data_size);
    }
}

MATCH_FUNC(0x4E9160)
void Map_0x370::AddTileAnimations(s32 size)
{
    u8* pTileAnimDataIter = reinterpret_cast<u8*>(this->mpTileAnimData);
    while (pTileAnimDataIter != reinterpret_cast<u8*>(this->mpTileAnimData) + size)
    {
        const gmp_tile_animation* pAnim = reinterpret_cast<const gmp_tile_animation*>(pTileAnimDataIter);
        gTileAnim_2->AddGmpAnim(pAnim);
        pTileAnimDataIter += (sizeof(u16) * pAnim->mAnimLength) +
            (sizeof(gmp_tile_animation) -
             sizeof(u16)); // field_4 is animation_length, 6 is the length of a record, each array entry is a u16
    }
}

MATCH_FUNC(0x4E91A0)
void Map_0x370::load_mobj(size_t len)
{
    gmp_map_object* pData = static_cast<gmp_map_object*>(Memory::Checked_malloc(len));
    mpMapObjects = pData;
    File::Global_Read(pData, len);
    mMapObjectCount = len / 6;
    if (static_cast<u32>(6 * mMapObjectCount) != len)
    {
        FatalError(Gta2Error::InvalidMapObjectData, "C:\\Splitting\\Gta2\\Source\\map.cpp", 6024);
    }
}

MATCH_FUNC(0x4E9200)
void Map_0x370::load_lght(size_t size)
{
    gmp_map_light* pData = static_cast<gmp_map_light*>(Memory::Checked_malloc(size));
    mpLightData = pData;
    File::Global_Read(pData, size);
    mNumLights = size >> 4;
    if (static_cast<u32>(16 * mNumLights) != size)
    {
        FatalError(Gta2Error::InvalidMapLightData, "C:\\Splitting\\Gta2\\Source\\map.cpp", 6046);
    }
}

MATCH_FUNC(0x4E9250)
void Map_0x370::load_zone(size_t size)
{
    gmp_map_zone* pData = static_cast<gmp_map_zone*>(Memory::Checked_malloc(size));
    mpZoneData = pData;
    File::Global_Read(pData, size);
    build_zone_list(size);
}

MATCH_FUNC(0x4E9280)
void Map_0x370::load_anim(size_t size)
{
    gmp_tile_animation* pAlloc = static_cast<gmp_tile_animation*>(Memory::Checked_malloc(size));
    mpTileAnimData = pAlloc;
    File::Global_Read(pAlloc, size);
    AddTileAnimations(size);
}

MATCH_FUNC(0x4E92B0)
void Map_0x370::load_dmap(u32 len)
{
    mpDmap = new gmp_compressed_map_32();
    if (mpDmap == NULL)
    {
        FatalError(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\map.cpp", 6147);
    }

    File::Global_Read(&mpDmap->mBase[0][0], 0x40000);
    File::Global_Read(&mpDmap->mColumnWords, sizeof(u32));

    if ((mpDmap->mColumnWords + 1024) > 0x20000u)
    {
        FatalError(Gta2Error::MapHasTooManyColumns,
                          "C:\\Splitting\\Gta2\\Source\\map.cpp",
                          6150,
                          mpDmap->mColumnWords - 0x1FC00);
    }

    mColumnWordExtra = mpDmap->mColumnWords + 1024;

    u32 m_size = mColumnWordExtra * sizeof(u32);
    u32 column_data_size = mpDmap->mColumnWords * sizeof(u32);

    mpDmap->mpColumn = static_cast<u16**>(Memory::Checked_malloc(m_size));

    File::Global_Read(mpDmap->mpColumn, column_data_size);

    mColumnWords358 = mpDmap->mColumnWords;
    mColumnWords360 = mColumnWords358;

    File::Global_Read(&mpDmap->mNumBlocks, 4);

    if ((mpDmap->mNumBlocks + 200) > 0x20000u)
    {
        FatalError(Gta2Error::MapHasTooManyBlocks,
                          "C:\\Splitting\\Gta2\\Source\\map.cpp",
                          6161,
                          mpDmap->mNumBlocks - 130872);
    }
    mNumBlocksExtra = mpDmap->mNumBlocks + 200;

    u32 m_size2 = mNumBlocksExtra * sizeof(gmp_block_info);
    u32 block_info_size = mpDmap->mNumBlocks * sizeof(gmp_block_info);

    mpDmap->mBlock = static_cast<gmp_block_info*>(Memory::Checked_malloc(m_size2));

    File::Global_Read(mpDmap->mBlock, block_info_size);

    mNumBlocks34C = mpDmap->mNumBlocks;
    mNumBlocks354 = mNumBlocks34C;

    if (len != block_info_size + column_data_size + 0x40008)
    {
        FatalError(Gta2Error::InvalidChunkSize, "C:\\Splitting\\Gta2\\Source\\map.cpp", 6170, len);
    }
}

MATCH_FUNC(0x4E94A0)
void Map_0x370::load_rgen()
{
    gRouteFinder->Load_RGEN();
}

MATCH_FUNC(0x4E94B0)
void Map_0x370::load_chunk(const char_type* pChunkType, size_t chunkLen)
{
    if (!strncmp(pChunkType, "DMAP", 4u))
    {
        load_dmap(chunkLen);
    }
    else if (!strncmp(pChunkType, "ZONE", 4u))
    {
        load_zone(chunkLen);
    }
    else if (!strncmp(pChunkType, "MOBJ", 4u))
    {
        load_mobj(chunkLen);
    }
    else if (!strncmp(pChunkType, "ANIM", 4u))
    {
        load_anim(chunkLen);
    }
    else if (!strncmp(pChunkType, "LGHT", 4u))
    {
        load_lght(chunkLen);
    }
    else if (!strncmp(pChunkType, "RGEN", 4u))
    {
        load_rgen();
    }
    else
    {
        File::Global_Seek(&chunkLen);
    }
}

MATCH_FUNC(0x4E95A0)
void Map_0x370::process_loaded_zone_data()
{
    do_process_loaded_zone_data();
}

MATCH_FUNC(0x4E95B0)
void Map_0x370::LoadMap(const char_type* pGmpFileName)
{
    gmp_header header;
    File::Global_Open(pGmpFileName);

    u32 len = sizeof(gmp_header);
    chunk_header chunkHeader;
    File::Global_Read(&header, len);

    len = sizeof(chunk_header);
    while (File::Global_TryRead(&chunkHeader, &len))
    {
        if (chunkHeader.mSize != 0)
        {
            load_chunk(chunkHeader.mType, chunkHeader.mSize);
        }
        len = sizeof(chunk_header);
    }
    File::Global_Close();

    process_loaded_zone_data();

    if (!mpDmap)
    {
        FatalError(Gta2Error::MapNotCompressed, "C:\\Splitting\\Gta2\\Source\\map.cpp", 6329);
    }
}

MATCH_FUNC(0x4E9660)
Map_0x370::Map_0x370()
{
    mpZoneData = NULL;
    mpZones = NULL;
    mpMapObjects = NULL;
    mMapObjectCount = 0;
    mpDmap = NULL;
    mpZoneArray = NULL;
    mpZoneInfo = NULL;
    mpTileAnimData = NULL;
    mpLightData = NULL;
    mNumLights = 0;
    mNumBlocksExtra = 0;
    mNumBlocks34C = 0;
    mNumBlocks354 = 0;
    mColumnWords358 = 0;
    mColumnWordExtra = 0;
    mColumnWords360 = 0;
    mCurZoneIdx = 0xFFFF;
    mZoneType = 0xff;
    mZoneX = 0;
    mZoneY = 0;
    mbMatchPos = false;
    mUnused366 = 0;
    mUnused369 = 0xff;
    mUnused36D = 0;
    mbBlockedByTerrain = false;
    mbLowerBlockHasArrows = false;

    sBlockInfo1.init();

    sBlockInfo2.mArrows = 0;
    sBlockInfo2.mLeft = 3073;
    sBlockInfo2.mRight = 3073;
    sBlockInfo2.mTop = 3073;
    sBlockInfo2.mBottom = 3073;
    sBlockInfo2.mLid = 1;
    sBlockInfo2.mSlopeType = 3;
}

MATCH_FUNC(0x4E9770)
Map_0x370::~Map_0x370()
{
    if (mpLightData)
    {
        crt::free(mpLightData);
    }
    mpLightData = NULL;

    if (mpZoneData)
    {
        crt::free(mpZoneData);
    }
    mpZoneData = NULL;

    if (mpZones)
    {
        crt::free(mpZones);
    }
    mpZones = NULL;

    if (mpMapObjects)
    {
        crt::free(mpMapObjects);
    }
    mpMapObjects = NULL;

    if (mpDmap)
    {
        if (mpDmap->mpColumn)
        {
            crt::free(mpDmap->mpColumn);
        }
        mpDmap->mpColumn = NULL;

        if (mpDmap->mBlock)
        {
            crt::free(mpDmap->mBlock);
        }
        mpDmap->mBlock = NULL;

        delete mpDmap;
    }

    if (mpZoneArray)
    {
        crt::free(mpZoneArray);
    }
    mpZoneArray = NULL;

    if (mpZoneInfo)
    {
        crt::free(mpZoneInfo);
    }
    mpZoneInfo = NULL;

    if (mpTileAnimData)
    {
        crt::free(mpTileAnimData);
    }
    mpTileAnimData = NULL;
}