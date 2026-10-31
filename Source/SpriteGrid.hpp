#pragma once

#include "Function.hpp"

class Sprite;
class Object_3C;
class GridSpriteLink;
class Fix16_Rect;
class SpriteList;
class Ped;

// One occupied x cell of a grid row: the sprites in cell (mX, row). Cells of a row are sorted by x.
struct GridCell
{
    void PoolAllocate()
    {
    }

    void PoolDeallocate()
    {
    }

    u8 mX; // 0x0
    GridSpriteLink* mpSpriteLinks; // 0x4
    GridCell* mpNext;
};
GTA2_ASSERT_SIZEOF_ALWAYS(GridCell, 0xC);

// Sparse 256x256 spatial hash of sprites: 256 rows, each a sorted linked list of occupied cells. Collision and
// nearest-sprite queries walk the cells that overlap the query rect (gSpriteGrid_left/right/top/bottom).
class SpriteGrid
{
  public:
    STATIC_MEMBER(s32, sSpriteGridSpriteTypeFilter);
    STATIC_MEMBER(s32, sSpriteGridSpriteType2);
    STATIC_MEMBER(s32, sSpriteGridSpriteType1);
    STATIC_MEMBER(Sprite*, sSpriteGridSmallestDistSprite);
    STATIC_MEMBER(s32, sSpriteGridSearchMode);
    STATIC_MEMBER(s32, sSpriteGridPreferredType);
    STATIC_MEMBER(SpriteList*, sSpriteGridList);
    STATIC_MEMBER(s32, sSpriteGridExcludeTypes);
    STATIC_MEMBER(Sprite*, sSpriteGridExcludeSprite);
    STATIC_MEMBER(u8, sbDoCollisionCheck);
    // 9.6f 0x446820
    inline GridSpriteLink* GetCollideListAt_446820(s32 x_find, s32 y_pos) const
    {
        if (y_pos <= 255 && y_pos >= 0)
        {
            for (GridCell* i = mRows[y_pos]; i; i = i->mpNext)
            {
                s32 cell_x = i->mX;

                if (cell_x > x_find)
                {
                    return NULL;
                }

                if (cell_x == x_find)
                {
                    return i->mpSpriteLinks;
                }
            }
        }

        return NULL;
    }

    EXPORT void Empty();
    EXPORT void DrawSpritesClipped();
    EXPORT void AddToSingleBucket_477AE0(Sprite* pSprite);
    EXPORT void Remove(Sprite* pSprite);
    EXPORT void AddToRegionBuckets(Sprite* pSprite);
    EXPORT void RemoveFromRegionBuckets(Sprite* pSprite);
    EXPORT void DebugLogAll();
    EXPORT bool CheckAndHandleCollisionInStrips(Sprite* pSprite);
    EXPORT bool CheckAndHandleAllCollisionsForSprite(Sprite* pSprite, s32 sprite_type_filter);
    EXPORT Sprite* FindNearestSprite_SpiralSearch(s32 primary_sprite_type,
                                                         s32 secondary_sprite_type,
                                                         Sprite* pExclude,
                                                         u8 max_rings,
                                                         s32 search_mode,
                                                         char_type bUseSpriteZ);
    EXPORT void SetSpriteToExclude(Sprite* pSprite);
    EXPORT Sprite* FindCollidingSpritePreferType(Sprite* pSprite, s32 desired_sprite_type);
    EXPORT bool CollectRectCollisions(Fix16_Rect* pRect,
                                             bool bDeepCheck,
                                             s32 exclude_sprite_type,
                                             Sprite* pExclude,
                                             SpriteList* pOutList);
    EXPORT bool CheckRectForCollisions(Fix16_Rect* pRect, bool bDeepCheck, s32 exclude_sprite_type, Sprite* pExclude);
    EXPORT SpriteGrid();
    EXPORT ~SpriteGrid();

    // height = how many rows to scan downward from gSpriteGrid_start_y
    EXPORT void SearchTileColumnForClosestSprite(u8 height);

    // pSpriteLinks = linked list of GridSpriteLink at a single (x,y) tile
    EXPORT void CheckTileSpritesForClosestMatch(GridSpriteLink* pSpriteLinks);

    // width = how many columns to scan horizontally from gSpriteGrid_start_x
    EXPORT void SearchTileStripForClosestSprite(u8 width);
    EXPORT void AddToDrawList(s32 left, s32 right, s32 top, s32 bottom);
    EXPORT void DoRemove(s32 x_pos, s32 y_pos, Sprite* pSprite);

  private:
    EXPORT void RemoveFromRowBuckets(s32 y_pos, Sprite* pSprite);
    EXPORT void AddToSingleBucket_478440(s32 x_pos, s32 y_pos, Sprite* pSprite);
    EXPORT void AddToRowBuckets(s32 y_pos, Sprite* pSprite);
    EXPORT GridCell* GetFirstXCellInRow(s32 y_pos) const;
    EXPORT bool CheckRowForRectCollisions(u32 y_pos, Fix16_Rect* pRect);
    EXPORT bool CheckAndHandleCollisionsInStrip(u32 y_pos, Sprite* pSprite);
    EXPORT bool CheckAndHandleRowCollisionsForSprite(u32 y_pos, Sprite* pSprite);
    EXPORT Sprite* FindCollidingSpriteInRow(u32 y_pos, Sprite* pSprite);
    EXPORT void DebugLog(s32 x_pos, s32 y_pos) const;
    EXPORT GridCell** Clear();

    GridCell* mRows[256]; // 0x0: rows of Y; each is a sparse X-linked list of GridCell
};
GTA2_ASSERT_SIZEOF_ALWAYS(SpriteGrid, 0x400);

EXTERN_GLOBAL(SpriteGrid*, gSpriteGrid_1);

EXTERN_GLOBAL(SpriteGrid*, gSpriteGrid_2);

EXTERN_GLOBAL(SpriteGrid*, gSpriteGrid_3);

EXTERN_GLOBAL(s32, gSpriteGrid_top);

EXTERN_GLOBAL(s32, gSpriteGrid_bottom);

EXTERN_GLOBAL(Sprite*, gSpriteGrid_exclusion_sprite);
EXTERN_GLOBAL(Ped*, gSpriteGrid_ped);