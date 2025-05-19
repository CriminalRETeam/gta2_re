#pragma once

#include "Function.hpp"

class Sprite;
class Object_3C;
class Collide_8;

struct PurpleDoom_C
{
    u8 field_0_x_len;
    char pad[3];
    Collide_8* field_4_p8;
    PurpleDoom_C *field_8_pNext;
};

class PurpleDoom
{
  public:
    EXPORT void DrawSpritesClipped_477A40();
    EXPORT void sub_477AE0(Sprite* a1);
    EXPORT void Remove_477B00(Sprite* a1);
    EXPORT void sub_477B20(Sprite* a1a);
    EXPORT void sub_477B60(Sprite* a1);
    EXPORT void DebugLogAll_477BA0();
    EXPORT char_type sub_477BD0(Sprite* a2);
    EXPORT char_type sub_477C30(Sprite* a2);
    EXPORT s32 sub_477C90(s32 a1, s32 a2, s32* a3, u8 a4, s32 a5, char_type a6);
    EXPORT Sprite* sub_477E60(Sprite* a2, s32 a3);
    EXPORT char_type sub_477F30(s32* a2, char_type a3, s32 a4, Sprite* a5, Object_3C* a6);
    EXPORT char_type sub_477F60(s32* a2, char_type a3, s32 a4, Sprite* a5);
    EXPORT PurpleDoom();
    EXPORT ~PurpleDoom();
    EXPORT u32 sub_478160(u8 a2);
    EXPORT void AddToDrawList_478240(s32 left, s32 right, s32 top, s32 bottom);
    EXPORT void DoRemove_4782C0(s32 a2, s32 idx, Sprite* a4);
    EXPORT void sub_478370(s32 y_pos, Sprite* pSprite);
    EXPORT void DoAdd_478440(s32 xpos, s32 ypos, Sprite* pSprite);
    EXPORT void sub_4784D0(s32 idx, Sprite* a3);
    EXPORT PurpleDoom_C* sub_478590(s32 start_idx);
    EXPORT char_type sub_4785D0(u32 a2, s32* a3);
    EXPORT char_type sub_478750(u32 a2, Sprite* a3);
    EXPORT bool sub_4787E0(u32 a2, Sprite* a3);
    EXPORT s32 sub_478880(u32 a2, u32* a3);
    EXPORT void DebugLog_478950(s32 xpos, s32 ypos);
    EXPORT PurpleDoom_C** Clear_4789F0();

    PurpleDoom_C* field_0[256]; // y cols with links to x rows, x is sparse to save mem? (whole map is 256x265)
};

EXPORT_VAR extern PurpleDoom* gPurpleDoom_1_679208;
EXPORT_VAR extern PurpleDoom* gPurpleDoom_2_67920C;
EXPORT_VAR extern PurpleDoom* gPurpleDoom_3_679210;
EXPORT_VAR extern s32 gPurple_top_6F6108;
EXPORT_VAR extern s32 gPurple_bottom_6F5F38;