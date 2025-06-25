#include "PurpleDoom.hpp"
#include "Car_BC.hpp"
#include "DrawUnk_0xBC.hpp"
#include "Globals.hpp"
#include "Montana.hpp"
#include "Object_5C.hpp"
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

EXPORT_VAR s32 dword_678FA8;
GLOBAL(dword_678FA8, 0x678FA8);

EXPORT_VAR s32 dword_678F60;
GLOBAL(dword_678F60, 0x678F60);

EXPORT_VAR Sprite* dword_678E40;
GLOBAL(dword_678E40, 0x678E40);

EXPORT_VAR s32 gPurpleDoom_679090;
GLOBAL(gPurpleDoom_679090, 0x679090);

EXPORT_VAR s32 gPurpleDoom_start_y_679098;
GLOBAL(gPurpleDoom_start_y_679098, 0x679098);

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
    const s32 left = Clamp((gViewCamera_676978->field_78_boundaries_non_neg.field_0_left - dword_679084).ToInt(), 0, 255);
    const s32 right_val = Clamp((dword_678F80 + gViewCamera_676978->field_78_boundaries_non_neg.field_4_right).ToInt(), 0, 255);
    const s32 top_val = Clamp((gViewCamera_676978->field_78_boundaries_non_neg.field_8_top - dword_679084).ToInt(), 0, 255);
    const s32 bottom_val = Clamp((dword_678F80 + gViewCamera_676978->field_78_boundaries_non_neg.field_C_bottom).ToInt(), 0, 255);

    AddToDrawList_478240(left, right_val, top_val, bottom_val);
}

MATCH_FUNC(0x477ae0)
void PurpleDoom::Add_477AE0(Sprite* a1)
{
    DoAdd_478440(a1->field_14_xpos.ToInt(), a1->field_18_ypos.ToInt(), a1);
}

MATCH_FUNC(0x477b00)
void PurpleDoom::Remove_477B00(Sprite* a1)
{
    DoRemove_4782C0(a1->field_14_xpos.ToInt(), a1->field_18_ypos.ToInt(), a1);
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

    gCollide_C_6791FC->field_4_count.mValue++; // TODO: Prob an inline

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
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x477c90)
s32 PurpleDoom::sub_477C90(s32 a1, s32 a2, s32* a3, u8 a4, s32 a5, char_type a6)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x477e60)
Sprite* PurpleDoom::sub_477E60(Sprite* pSprite, s32 sprite_type_enum)
{
    dword_678F60 = sprite_type_enum;
    dword_678E40 = 0;

    gCollide_C_6791FC->field_4_count.mValue++;

    pSprite->sub_59E9C0();
    pSprite->field_C_sprite_4c_ptr->SetCurrentRect_5A4D90();

    for (s32 top = gPurple_top_6F6108; top <= gPurple_bottom_6F5F38; top++)
    {
        Sprite* pObj = sub_478880(top, pSprite);
        if (pObj)
        {
            return pObj;
        }
    }

    return dword_678E40;
}

STUB_FUNC(0x477f30)
char_type PurpleDoom::sub_477F30(s32* a2, char_type a3, s32 a4, Sprite* a5, Object_3C* a6)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x477f60)
char_type PurpleDoom::sub_477F60(s32* a2, char_type a3, s32 a4, Sprite* a5)
{
    NOT_IMPLEMENTED;
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
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x478160)
u32 PurpleDoom::sub_478160(u8 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x478060)
void __stdcall PurpleDoom::sub_478060(Collide_8 *a1)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x4781E0)
void PurpleDoom::sub_4781E0(u8 width)
{
    NOT_IMPLEMENTED;

    gPurple_left_6F5FD4 = gPurpleDoom_679090;
    gPurple_right_6F5B80 = width + gPurpleDoom_679090 - 1;

    for (PurpleDoom_C* pXItemIter = sub_478590(gPurpleDoom_start_y_679098); pXItemIter; pXItemIter = pXItemIter->field_8_pNext)
    {
        if (pXItemIter->field_0_x_len > gPurple_right_6F5B80)
        {
            break;
        }
        sub_478060(pXItemIter->field_4_p8);
    }
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

MATCH_FUNC(0x4782c0)
void PurpleDoom::DoRemove_4782C0(s32 x_pos, s32 y_pos, Sprite* pToFind)
{
    PurpleDoom_C* pFound = 0;
    Collide_8* pFoundCollideForX = 0;

    for (PurpleDoom_C* pXIter = this->field_0[y_pos]; pXIter; pXIter = pXIter->field_8_pNext)
    {
        if (pXIter->field_0_x_len == x_pos)
        {
            Collide_8* pCollideForX = pXIter->field_4_p8;
            while (pCollideForX)
            {
                if (pCollideForX->field_0_sprt == pToFind)
                {
                    if (!pFoundCollideForX)
                    {
                        pXIter->field_4_p8 = pCollideForX->field_4_pNext;
                    }
                    else
                    {
                        pFoundCollideForX->field_4_pNext = pCollideForX->field_4_pNext;
                    }

                    gCollide_8004_679200->Remove(pCollideForX);

                    if (!pXIter->field_4_p8)
                    {
                        if (!pFound)
                        {
                            this->field_0[y_pos] = pXIter->field_8_pNext;
                        }
                        else
                        {
                            pFound->field_8_pNext = pXIter->field_8_pNext;
                        }
                        gCollide_11944_679204->Remove(pXIter);
                    }
                    return;
                }

                pFoundCollideForX = pCollideForX;
                pCollideForX = pCollideForX->field_4_pNext;
            }
        }
        pFound = pXIter;
    }
}

MATCH_FUNC(0x478370)
void PurpleDoom::sub_478370(s32 y_pos, Sprite* pSprite)
{
    s32 x_pos = gPurple_left_6F5FD4;
    PurpleDoom_C* pLastXIter = 0;
    for (PurpleDoom_C* pXItemIter = this->field_0[y_pos]; pXItemIter; pXItemIter = pXItemIter->field_8_pNext)
    {
        if (pXItemIter->field_0_x_len == x_pos)
        {
            Collide_8* pObj = pXItemIter->field_4_p8;
            Collide_8* pLast = 0;
            while (pObj)
            {
                if (pObj->field_0_sprt == pSprite)
                {
                    if (!pLast)
                    {
                        pXItemIter->field_4_p8 = pObj->field_4_pNext;
                    }
                    else
                    {
                        pLast->field_4_pNext = pObj->field_4_pNext;
                    }

                    gCollide_8004_679200->Remove(pObj);

                    if (!pXItemIter->field_4_p8)
                    {
                        PurpleDoom_C* pNext = pXItemIter->field_8_pNext;
                        if (!pLastXIter)
                        {
                            this->field_0[y_pos] = pNext;
                        }
                        else
                        {
                            pLastXIter->field_8_pNext = pNext;
                        }
                        pXItemIter = gCollide_11944_679204->Allocate(pXItemIter);
                    }
                    else
                    {
                        pLastXIter = pXItemIter;
                        pXItemIter = pXItemIter->field_8_pNext;
                    }
                    ++x_pos;

                    if (x_pos > gPurple_right_6F5B80)
                    {
                        return;
                    }
                    pObj = pXItemIter->field_4_p8;
                    pLast = 0;
                }
                else
                {
                    pLast = pObj;
                    pObj = pObj->field_4_pNext;
                }
            }
        }
        pLastXIter = pXItemIter;
    }
}

MATCH_FUNC(0x478440)
void PurpleDoom::DoAdd_478440(s32 xpos, s32 ypos, Sprite* pSprite)
{
    Collide_8* pNewCollide = gCollide_8004_679200->Allocate();
    pNewCollide->field_0_sprt = pSprite;

    PurpleDoom_C* pAddedTo = 0;
    PurpleDoom_C* pIter;
    for (pIter = this->field_0[ypos]; pIter; pIter = pIter->field_8_pNext)
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

    PurpleDoom_C* pNewItem = gCollide_11944_679204->Allocate2();

    if (!pAddedTo)
    {
        this->field_0[ypos] = pNewItem;
    }
    else
    {
        pAddedTo->field_8_pNext = pNewItem;
    }

    pNewItem->field_8_pNext = pIter;
    pNewItem->field_4_p8 = pNewCollide;
    pNewItem->field_0_x_len = xpos;
    pNewCollide->field_4_pNext = 0;
}

MATCH_FUNC(0x4784d0)
void PurpleDoom::sub_4784D0(s32 y_pos, Sprite* pSprite)
{
    s32 purple_left = gPurple_left_6F5FD4;
    PurpleDoom_C* pNewNext = this->field_0[y_pos];
    PurpleDoom_C* purple_x = 0;
    while (purple_left <= gPurple_right_6F5B80)
    {
        while (pNewNext)
        {
            if (pNewNext->field_0_x_len > purple_left)
            {
                break;
            }
            if (pNewNext->field_0_x_len == purple_left)
            {
                ++purple_left;
                Collide_8* v7 = gCollide_8004_679200->Allocate2();
                v7->field_0_sprt = pSprite;
                v7->field_4_pNext = pNewNext->field_4_p8;
                pNewNext->field_4_p8 = v7;
                if (purple_left > gPurple_right_6F5B80)
                {
                    return;
                }
            }
            purple_x = pNewNext;
            pNewNext = pNewNext->field_8_pNext;
        }

        Collide_8* v8 = gCollide_8004_679200->Allocate2();
        v8->field_0_sprt = pSprite;
        PurpleDoom_C* pCIter = gCollide_11944_679204->Allocate2();

        if (!purple_x)
        {
            this->field_0[y_pos] = pCIter;
        }
        else
        {
            purple_x->field_8_pNext = pCIter;
        }
        pCIter->field_8_pNext = pNewNext;
        pCIter->field_4_p8 = v8;
        pCIter->field_0_x_len = purple_left;
        v8->field_4_pNext = 0;
        pNewNext = pCIter->field_8_pNext;
        ++purple_left;
        purple_x = pCIter;
    }
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

// https://decomp.me/scratch/me1ge
STUB_FUNC(0x4785d0)
char_type PurpleDoom::sub_4785D0(u32 y_pos, Fix16_Rect* pRect)
{
    NOT_IMPLEMENTED;

    /*
    PurpleDoom_C* v3; // ebp
    char bRet; // bl
    Collide_8* pObj; // esi
    Sprite* field_0_sprt; // ecx
    Sprite* v10; // ecx
    Sprite_4C* sprite_4c_ptr; // eax
    __int16 ang_v; // ax

    v3 = sub_478590(y_pos);
    bRet = 0;
    while (v3)
    {
        if (v3->field_0_x_len > gPurple_right_6F5B80)
        {
            return bRet;
        }

        pObj = v3->field_4_p8;
        while (pObj)
        {

            field_0_sprt = pObj->field_0_sprt;
            if (pObj->field_0_sprt->field_30_sprite_type_enum != dword_678F88 && field_0_sprt != gPurpleDoom_sprite_678F84 &&
                field_0_sprt->field_C_o5c->field_1C.field_10.mValue != gCollide_C_6791FC->field_4_count && field_0_sprt->sub_59E850(0))
            {
                gCollide_C_6791FC->field_0_count++;

                if (pRect->field_0_left < pObj->field_0_sprt->field_C_o5c->field_1C.field_14)
                {
                    if (pRect->field_4_right < pObj->field_0_sprt->field_C_o5c->field_1C.field_14)
                    {
                        goto LABEL_28;
                    }
                }
                else
                {
                    if (pRect->field_0_left > pObj->field_0_sprt->field_C_o5c->field_1C.field_18)
                    {
                        goto LABEL_28;
                    }
                }

                if (pRect->field_8_top < pObj->field_0_sprt->field_C_o5c->field_1C.field_1C.mValue)
                {
                    if (pRect->field_8_top > pObj->field_0_sprt->field_C_o5c->field_1C.field_20)
                    {
                        goto LABEL_28;
                    }
                }
                else if (pRect->field_C_bottom >= pObj->field_0_sprt->field_C_o5c->field_1C.field_1C.mValue)
                {
                    goto LABEL_28;
                }

                // LABEL_16:
                if (!Fix16::sub_438FB0(&pRect->field_10,
                                       &pRect->field_14,
                                       &pObj->field_0_sprt->field_C_o5c->field_1C.field_24,
                                       &pObj->field_0_sprt->field_C_o5c->field_1C.field_28))
                {
                    goto LABEL_28;
                }

                if (byte_679006)
                {
                    v10 = pObj->field_0_sprt;
                    sprite_4c_ptr = pObj->field_0_sprt->field_C_sprite_4c_ptr;
                    if (sprite_4c_ptr->field_0_w != sprite_4c_ptr->field_4_h || sprite_4c_ptr->field_0_w > dword_678EE4)
                    {
                        ang_v = v10->field_0.rValue;
                        if (v10->field_0.rValue)
                        {
                            if (ang_v != 360 && ang_v != 720 && ang_v != 1080 && !v10->sub_59FB10(&pRect->field_0_left) &&
                                !pRect->sub_59DDF0(pObj->field_0_sprt))
                            {
                                goto LABEL_28;
                            }
                        }
                    }
                }

                if (dword_679214)
                {
                    bRet = 1;
                    dword_679214->sub_5A6CD0(pObj->field_0_sprt);
                    goto LABEL_28;
                }
                else
                {
                    return 1;
                }

            LABEL_28:
                pObj->field_0_sprt->field_C_o5c->field_1C.field_10.mValue = gCollide_C_6791FC->field_4_count;
            }

            pObj = pObj->field_4_pNext;

        } // end while

        //    LABEL_30:
        v3 = v3->field_8_pNext;
    } // end while
    return bRet;
*/
    return 0;
}

MATCH_FUNC(0x478750)
char_type PurpleDoom::sub_478750(u32 y_pos, Sprite* pSprite)
{
    char_type bRet = 0;
    PurpleDoom_C* pIter = sub_478590(y_pos);
    while (pIter)
    {
        if (pIter->field_0_x_len > gPurple_right_6F5B80)
        {
            break;
        }

        Collide_8* pC8Iter = pIter->field_4_p8;
        while (pC8Iter)
        {
            if (pC8Iter->field_0_sprt->field_C_o5c->field_2C != gCollide_C_6791FC->field_4_count.mValue)
            {
                gCollide_C_6791FC->field_0_count.mValue++;
                if (pSprite->sub_59E590(pC8Iter->field_0_sprt))
                {
                    bRet = 1;
                    pC8Iter->field_0_sprt->sub_59E8C0(pSprite);
                }
                pC8Iter->field_0_sprt->field_C_o5c->field_2C = gCollide_C_6791FC->field_4_count.mValue;
            }
            pC8Iter = pC8Iter->field_4_pNext;
        }
        pIter = pIter->field_8_pNext;
    }
    return bRet;
}

MATCH_FUNC(0x4787e0)
bool PurpleDoom::sub_4787E0(u32 y_pos, Sprite* pSprite)
{
    bool bRet;
    PurpleDoom_C* pXItemIter;
    Collide_8* p8Iter;

    bRet = false;
    pXItemIter = sub_478590(y_pos);
    while (pXItemIter)
    {
        if (pXItemIter->field_0_x_len > gPurple_right_6F5B80)
        {
            break;
        }

        for (p8Iter = pXItemIter->field_4_p8; p8Iter; p8Iter = p8Iter->field_4_pNext)
        {
            if (p8Iter->field_0_sprt->field_30_sprite_type_enum == dword_678FA8 &&
                p8Iter->field_0_sprt->field_C_o5c->field_2C != gCollide_C_6791FC->field_4_count.mValue)
            {
                gCollide_C_6791FC->field_0_count.mValue++;

                if (pSprite->sub_59E590(p8Iter->field_0_sprt))
                {
                    bRet = true;
                    p8Iter->field_0_sprt->sub_59E910(pSprite);
                }

                p8Iter->field_0_sprt->field_C_o5c->field_2C = gCollide_C_6791FC->field_4_count.mValue;
            }
        }
        pXItemIter = pXItemIter->field_8_pNext;
    }

    return bRet;
}

STUB_FUNC(0x478880)
Sprite* PurpleDoom::sub_478880(u32 a2, Sprite* a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x478950)
void PurpleDoom::DebugLog_478950(s32 xpos, s32 ypos)
{
    NOT_IMPLEMENTED;
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
