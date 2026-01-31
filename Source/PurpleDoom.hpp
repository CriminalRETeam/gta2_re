#pragma once

#include "Function.hpp"

class Sprite;
class Object_3C;
class Collide_8;
class Fix16_Rect;
class struct_4;
class Ped;

struct PurpleDoom_C
{
    void PoolAllocate()
    {
    }

    void PoolDeallocate()
    {
    }

    u8 field_0_x_len;
    char pad[3];
    Collide_8* field_4_p8;
    PurpleDoom_C* mpNext;
};

class PurpleDoom
{
  public:
    EXPORT void Empty_478A10();
    EXPORT void DrawSpritesClipped_477A40();
    EXPORT void AddToSingleBucket_477AE0(Sprite* pSprite);
    EXPORT void Remove_477B00(Sprite* pSprite);
    EXPORT void AddToRegionBuckets_477B20(Sprite* pSprite);
    EXPORT void AddToSpriteRectBuckets_477B60(Sprite* pSprite);
    EXPORT void DebugLogAll_477BA0();
    EXPORT char_type CheckAndHandleCollisionInStrips_477BD0(Sprite* pSprite);
    EXPORT bool CheckAndHandleAllCollisionsForSprite_477C30(Sprite* pSprite, s32 spriteTypeFilter);
    EXPORT Sprite* FindNearestSprite_SpiralSearch_477C90(s32 primarySpriteType,
                                                         s32 secondarySpriteType,
                                                         Sprite* pExclude,
                                                         u8 maxXCheck,
                                                         s32 searchMode,
                                                         char_type useSpriteZ);
    EXPORT void SetSpriteToExclude_477E50(Sprite* pSprite);
    EXPORT Sprite* FindNearestSpriteOfType_477E60(Sprite* pSprite, s32 desiredSpriteType);
    EXPORT bool CollectRectCollisions_477F30(Fix16_Rect* pRect,
                                             char_type doDeepCheck,
                                             s32 excludeSpriteType,
                                             Sprite* pExclude,
                                             struct_4* pOutList);
    EXPORT bool CheckRectForCollisions_477F60(Fix16_Rect* pRect, char_type doDeepCheck, s32 excludeSpriteType, Sprite* pExclude);
    EXPORT PurpleDoom();
    EXPORT ~PurpleDoom();

    // height = how many rows to scan downward from gPurpleDoom_start_y_679098
    EXPORT u32 SearchTileColumnForClosestSprite_478160(u8 height);

    // pBucketList = linked list of Collide_8 at a single (x,y) tile
    EXPORT void CheckTileSpritesForClosestMatch_478060(Collide_8* pBucketList);

    // width = how many columns to scan horizontally from gPurpleDoom_start_x_679090
    EXPORT void SearchTileStripForClosestSprite_4781E0(u8 width);
    EXPORT void AddToDrawList_478240(s32 left, s32 right, s32 top, s32 bottom);
    EXPORT void DoRemove_4782C0(s32 xpos, s32 ypos, Sprite* pSprite);

private:
    EXPORT void AddToColumnBuckets_478370(s32 y_pos, Sprite* pSprite);
    EXPORT void AddToSingleBucket_478440(s32 xpos, s32 ypos, Sprite* pSprite);
    EXPORT void AddToRowBuckets_4784D0(s32 y_pos, Sprite* pSprite);
    EXPORT PurpleDoom_C* GetFirstXCellInRow_478590(s32 y_pos);
    EXPORT char_type CheckRowForRectCollisions_4785D0(u32 y_pos, Fix16_Rect* pRect);
    EXPORT char_type CheckAndHandleCollisionsInStrip_478750(u32 y_pos, Sprite* pSprite);
    EXPORT bool CheckAndHandleRowCollisionsForSprite_4787E0(u32 y_pos, Sprite* pSprite);
    EXPORT Sprite* FindNearestSpriteInRow_478880(u32 y_pos, Sprite* pSprite);
    EXPORT void DebugLog_478950(s32 xpos, s32 ypos);
    EXPORT PurpleDoom_C** Clear_4789F0();

    PurpleDoom_C* field_0[256]; // rows of Y; each is a sparse X-linked list of PurpleDoom_C
};

EXTERN_GLOBAL(PurpleDoom*, gPurpleDoom_1_679208);

EXTERN_GLOBAL(PurpleDoom*, gPurpleDoom_2_67920C);

EXTERN_GLOBAL(PurpleDoom*, gPurpleDoom_3_679210);

EXTERN_GLOBAL(s32, gPurple_top_6F6108);

EXTERN_GLOBAL(s32, gPurple_bottom_6F5F38);

EXTERN_GLOBAL(Sprite*, gPurpleDoom_exclusion_sprite_678F84);
EXTERN_GLOBAL(Ped*, gPurpleDoom_ped_678F64);