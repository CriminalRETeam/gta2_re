#include "PurpleDoom.hpp"
#include "Car_BC.hpp"
#include "DrawUnk_0xBC.hpp"
#include "Globals.hpp"
#include "Montana.hpp"
#include "collide.hpp"
#include "map_0x370.hpp"
#include "sprite.hpp"

EXPORT_VAR PurpleDoom* gPurpleDoom_1_679208;
GLOBAL(gPurpleDoom_1_679208, 0x679208);

EXPORT_VAR PurpleDoom* gPurpleDoom_2_67920C;
GLOBAL(gPurpleDoom_2_67920C, 0x67920C);

EXPORT_VAR PurpleDoom* gPurpleDoom_3_679210;
GLOBAL(gPurpleDoom_3_679210, 0x679210);

EXPORT_VAR s32 gPurple_bottom_6F5F38;
GLOBAL(gPurple_bottom_6F5F38, 0x6F5F38);

EXPORT_VAR s32 gPurple_top_6F6108;
GLOBAL(gPurple_top_6F6108, 0x6F6108);

extern EXPORT_VAR Collide_C* gCollide_C_6791FC;
extern EXPORT_VAR Collide_11944* gCollide_11944_679204;
extern EXPORT_VAR Collide_8004* gCollide_8004_679200;

Fix16 dword_678F80(0x6000); // 1.5
Fix16 dword_679084(0x4000);

// TODO: might be used elsewhere too or have been a macro
static inline s32 Clamp(s32 value, s32 min, s32 max)
{
    if (value < min)
    {
        value = min;
    }
    else if (value > max)
    {
        value = max;
    }
    return value;
}

MATCH_FUNC(0x477a40)
void PurpleDoom::DrawSpritesClipped_477A40()
{
    const s32 left = Clamp((gViewCamera_676978->field_78_win_left - dword_679084).ToInt(), 0, 255);
    const s32 right_val = Clamp((dword_678F80 + gViewCamera_676978->field_7C_win_right).ToInt(), 0, 255);
    const s32 top_val = Clamp((gViewCamera_676978->field_80_win_top - dword_679084).ToInt(), 0, 255);
    const s32 bottom_val = Clamp((dword_678F80 + gViewCamera_676978->field_84_win_bottom).ToInt(), 0, 255);

    AddToDrawList_478240(left, right_val, top_val, bottom_val);
}

MATCH_FUNC(0x477ae0)
void PurpleDoom::sub_477AE0(Sprite* a1)
{
    DoAdd_478440(a1->field_14_xpos.ToInt(), a1->field_18_ypos.ToInt(), a1);
}

MATCH_FUNC(0x477b00)
void PurpleDoom::Remove_477B00(Sprite* a1)
{
    sub_4782C0(a1->field_14_xpos.ToInt(), a1->field_18_ypos.ToInt(), a1);
}

MATCH_FUNC(0x477b20)
void PurpleDoom::sub_477B20(Sprite* pSprite)
{
    pSprite->sub_59E9C0();
    pSprite->field_C_sprite_4c_ptr->SetCurrentRect_5A4D90();
    for (s32 y_pos = gPurple_top_6F6108; y_pos <= gPurple_bottom_6F5F38; ++y_pos)
    {
        sub_4784D0(y_pos, pSprite);
    }
}

MATCH_FUNC(0x477b60)
void PurpleDoom::sub_477B60(Sprite* pSprite)
{
    pSprite->field_C_sprite_4c_ptr->SetCurrentRect_5A4D90();
    for (s32 y_pos = gPurple_top_6F6108; y_pos <= gPurple_bottom_6F5F38; ++y_pos)
    {
        sub_478370(y_pos, pSprite);
    }
}

MATCH_FUNC(0x477ba0)
void PurpleDoom::DebugLogAll_477BA0()
{
    for (s32 i = 0; i < 256; ++i)
    {
        for (s32 j = 0; j < 256; ++j)
        {
            DebugLog_478950(j, i);
        }
    }
}

MATCH_FUNC(0x477bd0)
char_type PurpleDoom::sub_477BD0(Sprite* pSprite)
{
    char_type bUnknown = 0;

    gCollide_C_6791FC->field_4_count++; // TODO: Prob an inline

    pSprite->sub_59E9C0();
    pSprite->field_C_sprite_4c_ptr->SetCurrentRect_5A4D90();

    for (s32 i = gPurple_top_6F6108; i <= gPurple_bottom_6F5F38; ++i)
    {
        bUnknown |= sub_478750(i, pSprite);
    }
    return bUnknown;
}

STUB_FUNC(0x477c30)
char_type PurpleDoom::sub_477C30(Sprite* a2)
{
    return 0;
}

STUB_FUNC(0x477c90)
s32 PurpleDoom::sub_477C90(s32 a1, s32 a2, s32* a3, u8 a4, s32 a5, char_type a6)
{
    return 0;
}

STUB_FUNC(0x477e60)
Sprite* PurpleDoom::sub_477E60(Sprite* a2, s32 a3)
{
    return 0;
}

STUB_FUNC(0x477f30)
char_type PurpleDoom::sub_477F30(s32* a2, char_type a3, s32 a4, Sprite* a5, Object_3C* a6)
{
    return 0;
}

STUB_FUNC(0x477f60)
char_type PurpleDoom::sub_477F60(s32* a2, char_type a3, s32 a4, Sprite* a5)
{
    return 0;
}

MATCH_FUNC(0x478040)
PurpleDoom::PurpleDoom()
{
    Clear_4789F0();
}

STUB_FUNC(0x478050)
PurpleDoom::~PurpleDoom()
{
}

STUB_FUNC(0x478160)
u32 PurpleDoom::sub_478160(u8 a2)
{
    return 0;
}

MATCH_FUNC(0x478240)
void PurpleDoom::AddToDrawList_478240(s32 left, s32 right, s32 top, s32 bottom)
{
    PurpleDoom_C** pYItem = &this->field_0[top]; // y_start?
    if (top <= bottom)
    {
        s32 y_total = bottom - top + 1;
        do
        {
            for (PurpleDoom_C* pXItem = *pYItem; pXItem; pXItem = pXItem->field_8_pNext)
            {
                const s32 x_cell = pXItem->field_0_x_len;
                if (x_cell > right)
                {
                    break;
                }
                if (x_cell >= left)
                {
                    for (Collide_8* p8Iter = pXItem->field_4_p8; p8Iter; p8Iter = p8Iter->field_4_pNext)
                    {
                        if (p8Iter->field_0_sprt->field_30_sprite_type_enum > sprite_types_enum::unknown_1)
                        {
                            gMontana_67B580->DisplayAdd_495510(p8Iter->field_0_sprt);
                        }
                    }
                }
            }
            ++pYItem;
            --y_total;
        } while (y_total);
    }
}

STUB_FUNC(0x4782c0)
void PurpleDoom::sub_4782C0(s32 a2, s32 idx, Sprite* a4)
{
}

STUB_FUNC(0x478370)
u8* PurpleDoom::sub_478370(s32 a2, Sprite* a3)
{
    return 0;
}

STUB_FUNC(0x478440)
void PurpleDoom::DoAdd_478440(s32 xpos, s32 ypos, Sprite* pSprite)
{
    Collide_8* pNewCollide = gCollide_8004_679200->field_0;
    PurpleDoom_C* pAddedTo = 0;
    gCollide_8004_679200->field_0 = gCollide_8004_679200->field_0->field_4_pNext;
    pNewCollide->field_0_sprt = pSprite;
    for (PurpleDoom_C* pIter = this->field_0[ypos]; pIter; pIter = pIter->field_8_pNext)
    {
        const s32 x_len = pIter->field_0_x_len;
        if (x_len > xpos)
        {
            break;
        }
        if (x_len == xpos)
        {
            pNewCollide->field_4_pNext = pIter->field_4_p8;
            pIter->field_4_p8 = pNewCollide;
            return;
        }
        pAddedTo = pIter;
    }
    
    PurpleDoom_C* pNewItem = gCollide_11944_679204->field_0;
    gCollide_11944_679204->field_0 = gCollide_11944_679204->field_0->field_8_pNext;
    
    if (pAddedTo)
    {
        pAddedTo->field_8_pNext = pNewItem;
    }
    else
    {
        this->field_0[ypos] = pNewItem;
    }

    pNewItem->field_8_pNext = pIter;
    pNewItem->field_4_p8 = pNewCollide;
    pNewItem->field_0_x_len = xpos;
    pNewCollide->field_4_pNext = 0;
}

STUB_FUNC(0x4784d0)
void PurpleDoom::sub_4784D0(s32 idx, Sprite* a3)
{
}

// Get first XItem at y_pos
MATCH_FUNC(0x478590)
PurpleDoom_C* PurpleDoom::sub_478590(s32 start_idx)
{
    PurpleDoom_C* pIter;
    s32 f0;

    if (start_idx < 0 || start_idx > 255)
    {
        return 0;
    }
    for (pIter = this->field_0[start_idx]; pIter; pIter = pIter->field_8_pNext)
    {
        f0 = (u8)pIter->field_0_x_len;
        if (f0 >= gPurple_left_6F5FD4)
        {
            break;
        }
        if (f0 > gPurple_right_6F5B80)
        {
            return 0;
        }
    }
    return pIter;
}

STUB_FUNC(0x4785d0)
char_type PurpleDoom::sub_4785D0(u32 a2, s32* a3)
{
    return 0;
}

STUB_FUNC(0x478750)
char_type PurpleDoom::sub_478750(u32 a2, Sprite* a3)
{
    return 0;
}

STUB_FUNC(0x4787e0)
char_type PurpleDoom::sub_4787E0(u32 a2, u32* a3)
{
    return 0;
}

STUB_FUNC(0x478880)
s32 PurpleDoom::sub_478880(u32 a2, u32* a3)
{
    return 0;
}

STUB_FUNC(0x478950)
void PurpleDoom::DebugLog_478950(s32 xpos, s32 ypos)
{
}

MATCH_FUNC(0x4789f0)
PurpleDoom_C** PurpleDoom::Clear_4789F0()
{
    for (u32 i = 0; i < GTA2_COUNTOF(field_0); i++)
    {
        field_0[i] = 0;
    }
    return field_0;
}
