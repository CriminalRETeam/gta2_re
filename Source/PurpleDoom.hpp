#pragma once

#include "Function.hpp"

class Sprite;
class Object_3C;
class Collide_8;
class Fix16_Rect;
class struct_4;

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
    EXPORT void DrawSpritesClipped_477A40();
    EXPORT void Add_477AE0(Sprite* a1);
    EXPORT void Remove_477B00(Sprite* a1);
    EXPORT void sub_477B20(Sprite* a1a);
    EXPORT void sub_477B60(Sprite* a1);
    EXPORT void DebugLogAll_477BA0();
    EXPORT char_type CheckAndHandleCollisionInStrips_477BD0(Sprite* a2);
    EXPORT bool sub_477C30(Sprite* pSprt, s32 a3);
    EXPORT Sprite* FindNearestSprite_SpiralSearch_477C90(s32 sprite_type1, s32 sprite_type2, Sprite* pExclude, u8 max_x_check, s32 searchMode, char_type bUseSpriteZ);
    EXPORT void SetSpriteToExclude_477E50(Sprite* pSprite);
    EXPORT Sprite* sub_477E60(Sprite* a2, s32 a3);
    EXPORT bool sub_477F30(Fix16_Rect* a2, char_type a3, s32 a4, Sprite* a5, struct_4* a6);
    EXPORT bool sub_477F60(Fix16_Rect* a2, char_type a3, s32 a4, Sprite* a5);
    EXPORT PurpleDoom();
    EXPORT ~PurpleDoom();
    EXPORT u32 sub_478160(u8 a2);
    EXPORT void CheckTileSpritesForClosestMatch_478060(Collide_8* a1);
    EXPORT void sub_4781E0(u8 width);
    EXPORT void AddToDrawList_478240(s32 left, s32 right, s32 top, s32 bottom);
    EXPORT void DoRemove_4782C0(s32 a2, s32 idx, Sprite* a4);
    EXPORT void sub_478370(s32 y_pos, Sprite* pSprite);
    EXPORT void DoAdd_478440(s32 xpos, s32 ypos, Sprite* pSprite);
    EXPORT void sub_4784D0(s32 idx, Sprite* a3);
    EXPORT PurpleDoom_C* sub_478590(s32 start_idx);
    EXPORT char_type sub_4785D0(u32 y_pos, Fix16_Rect* pRect);
    EXPORT char_type CheckAndHandleCollisionsInStrip_478750(u32 a2, Sprite* a3);
    EXPORT bool sub_4787E0(u32 a2, Sprite* a3);
    EXPORT Sprite* sub_478880(u32 a2, Sprite* a3);
    EXPORT void DebugLog_478950(s32 xpos, s32 ypos);
    EXPORT PurpleDoom_C** Clear_4789F0();

    PurpleDoom_C* field_0[256]; // y cols with links to x rows, x is sparse to save mem? (whole map is 256x265x8)
};

EXTERN_GLOBAL(PurpleDoom*, gPurpleDoom_1_679208);

EXTERN_GLOBAL(PurpleDoom*, gPurpleDoom_2_67920C);

EXTERN_GLOBAL(PurpleDoom*, gPurpleDoom_3_679210);

EXTERN_GLOBAL(s32, gPurple_top_6F6108);

EXTERN_GLOBAL(s32, gPurple_bottom_6F5F38);

EXTERN_GLOBAL(Sprite*, gPurpleDoom_exclusion_sprite_678F84);
