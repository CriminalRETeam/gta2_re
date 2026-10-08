#pragma once

#include "Function.hpp"

class Sprite;
class Object_3C;
class GridSpriteLink_8;
class Fix16_Rect;
class struct_4;
class Ped;

// One occupied x cell of a grid row: the sprites in cell (field_0_x, row). Cells of a row are sorted by x.
struct GridCell_C
{
    void PoolAllocate()
    {
    }

    void PoolDeallocate()
    {
    }

    u8 field_0_x;
    GridSpriteLink_8* field_4_pSpriteLinks;
    GridCell_C* mpNext;
};

// Sparse 256x256 spatial hash of sprites: 256 rows, each a sorted linked list of occupied cells. Collision and
// nearest-sprite queries walk the cells that overlap the query rect (gSpriteGrid_left/right/top/bottom).
class SpriteGrid_400
{
  public:
    inline GridSpriteLink_8* GetCollideListAt_446820(s32 x_find, s32 y_pos)
    {
        if (y_pos <= 255 && y_pos >= 0)
        {
            for (GridCell_C* i = field_0_rows[y_pos]; i; i = i->mpNext)
            {
                s32 x_len = i->field_0_x;

                if (x_len > x_find)
                {
                    return NULL;
                }

                if (x_len == x_find)
                {
                    return i->field_4_pSpriteLinks;
                }
            }
        }

        return NULL;
    }

    EXPORT void Empty_478A10();
    EXPORT void DrawSpritesClipped_477A40();
    EXPORT void AddToSingleBucket_477AE0(Sprite* pSprite);
    EXPORT void Remove_477B00(Sprite* pSprite);
    EXPORT void AddToRegionBuckets_477B20(Sprite* pSprite);
    EXPORT void AddToSpriteRectBuckets_477B60(Sprite* pSprite);
    EXPORT void DebugLogAll_477BA0();
    EXPORT char_type CheckAndHandleCollisionInStrips_477BD0(Sprite* pSprite);
    EXPORT bool CheckAndHandleAllCollisionsForSprite_477C30(Sprite* pSprite, s32 sprite_type_filter);
    EXPORT Sprite* FindNearestSprite_SpiralSearch_477C90(s32 primary_sprite_type,
                                                         s32 secondary_sprite_type,
                                                         Sprite* pExclude,
                                                         u8 max_rings,
                                                         s32 search_mode,
                                                         char_type bUseSpriteZ);
    EXPORT void SetSpriteToExclude_477E50(Sprite* pSprite);
    EXPORT Sprite* FindNearestSpriteOfType_477E60(Sprite* pSprite, s32 desired_sprite_type);
    EXPORT bool CollectRectCollisions_477F30(Fix16_Rect* pRect,
                                             char_type bDeepCheck,
                                             s32 exclude_sprite_type,
                                             Sprite* pExclude,
                                             struct_4* pOutList);
    EXPORT bool CheckRectForCollisions_477F60(Fix16_Rect* pRect, char_type bDeepCheck, s32 exclude_sprite_type, Sprite* pExclude);
    EXPORT SpriteGrid_400();
    EXPORT ~SpriteGrid_400();

    // height = how many rows to scan downward from gSpriteGrid_start_y_679098
    EXPORT void SearchTileColumnForClosestSprite_478160(u8 height);

    // pSpriteLinks = linked list of GridSpriteLink_8 at a single (x,y) tile
    EXPORT void CheckTileSpritesForClosestMatch_478060(GridSpriteLink_8* pSpriteLinks);

    // width = how many columns to scan horizontally from gSpriteGrid_start_x_679090
    EXPORT void SearchTileStripForClosestSprite_4781E0(u8 width);
    EXPORT void AddToDrawList_478240(s32 left, s32 right, s32 top, s32 bottom);
    EXPORT void DoRemove_4782C0(s32 x_pos, s32 y_pos, Sprite* pSprite);

  private:
    EXPORT void AddToColumnBuckets_478370(s32 y_pos, Sprite* pSprite);
    EXPORT void AddToSingleBucket_478440(s32 x_pos, s32 y_pos, Sprite* pSprite);
    EXPORT void AddToRowBuckets_4784D0(s32 y_pos, Sprite* pSprite);
    EXPORT GridCell_C* GetFirstXCellInRow_478590(s32 y_pos);
    EXPORT char_type CheckRowForRectCollisions_4785D0(u32 y_pos, Fix16_Rect* pRect);
    EXPORT char_type CheckAndHandleCollisionsInStrip_478750(u32 y_pos, Sprite* pSprite);
    EXPORT bool CheckAndHandleRowCollisionsForSprite_4787E0(u32 y_pos, Sprite* pSprite);
    EXPORT Sprite* FindNearestSpriteInRow_478880(u32 y_pos, Sprite* pSprite);
    EXPORT void DebugLog_478950(s32 x_pos, s32 y_pos);
    EXPORT GridCell_C** Clear_4789F0();

    GridCell_C* field_0_rows[256]; // rows of Y; each is a sparse X-linked list of GridCell_C
};

EXTERN_GLOBAL(SpriteGrid_400*, gSpriteGrid_1_679208);

EXTERN_GLOBAL(SpriteGrid_400*, gSpriteGrid_2_67920C);

EXTERN_GLOBAL(SpriteGrid_400*, gSpriteGrid_3_679210);

EXTERN_GLOBAL(s32, gSpriteGrid_top_6F6108);

EXTERN_GLOBAL(s32, gSpriteGrid_bottom_6F5F38);

EXTERN_GLOBAL(Sprite*, gSpriteGrid_exclusion_sprite_678F84);
EXTERN_GLOBAL(Ped*, gSpriteGrid_ped_678F64);