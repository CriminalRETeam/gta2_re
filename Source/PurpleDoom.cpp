#include "PurpleDoom.hpp"
#include "Camera.hpp"
#include "Car_BC.hpp"
#include "Globals.hpp"
#include "Montana.hpp"
#include "Object_5C.hpp"
#include "collide.hpp"
#include "error.hpp"
#include "map_0x370.hpp"
#include "sprite.hpp"

// Multi/regional bucket stuff, Cars/Peds/Cranes/Objects
DEFINE_GLOBAL(PurpleDoom*, gPurpleDoom_1_679208, 0x679208);

// Used by buses, car-trailer combo, Char_B4 in certain states and objects with certain collision category
DEFINE_GLOBAL(PurpleDoom*, gPurpleDoom_2_67920C, 0x67920C);

// Single bucket stuff, mostly particles
DEFINE_GLOBAL(PurpleDoom*, gPurpleDoom_3_679210, 0x679210);

DEFINE_GLOBAL(s32, gPurple_bottom_6F5F38, 0x6F5F38);
DEFINE_GLOBAL(s32, gPurple_top_6F6108, 0x6F6108);
DEFINE_GLOBAL(s32, dword_678FA8, 0x678FA8);
DEFINE_GLOBAL(s32, gPurpleDoom_exclude_type_678F60, 0x678F60);
DEFINE_GLOBAL(Sprite*, gPurpleDoom_smallestDistSprite_678E40, 0x678E40);
DEFINE_GLOBAL(u8, bDoCollisionCheck_679006, 0x679006);
DEFINE_GLOBAL(s32, gPurpleDoom_exclude_types_678F88, 0x678F88);
DEFINE_GLOBAL(s32, gPurpleDoom_start_x_679090, 0x679090);
DEFINE_GLOBAL(s32, gPurpleDoom_start_y_679098, 0x679098);
DEFINE_GLOBAL(struct_4*, gPurpleDoom_list_679214, 0x679214);
EXTERN_GLOBAL(Collide_C*, gCollide_C_6791FC);
EXTERN_GLOBAL(T_PurpleDoom_C_Pool*, gPurpleDoom_C_Pool_679204);
EXTERN_GLOBAL(T_Collide_8_Pool*, gCollide_8_Pool_679200);
DEFINE_GLOBAL(Sprite*, gPurpleDoom_exclusion_sprite_678F84, 0x678F84);
DEFINE_GLOBAL_INIT(Fix16, k_dword_678F74, Fix16(0x2000, 0), 0x678F74);
DEFINE_GLOBAL(s32, gPurpleDoom_sprite_type1_678FE8, 0x678FE8);
DEFINE_GLOBAL(s32, gPurpleDoom_sprite_type2_678FEC, 0x678FEC);
DEFINE_GLOBAL(Sprite*, gPurpleDoom_exclude_sprite_678F40, 0x678F40);
DEFINE_GLOBAL(Ped*, gPurpleDoom_ped_678F64, 0x678F64);
DEFINE_GLOBAL(Fix16, gPurpleDoom_smallestDistance_678E5C, 0x678E5C);
DEFINE_GLOBAL(Fix16, gPurpleDoom_zpos_max_678F38, 0x678F38);
DEFINE_GLOBAL(Fix16, gPurpleDoom_zpos_min_678F3C, 0x678F3C);
DEFINE_GLOBAL(s32, gPurpleDoom_search_mode_678FD0, 0x678FD0);

DEFINE_GLOBAL_INIT(Fix16, dword_678F80, Fix16(0x6000, 0), 0x678F80);
DEFINE_GLOBAL_INIT(Fix16, dword_679084, Fix16(1), 0x679084);

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
void PurpleDoom::AddToSingleBucket_477AE0(Sprite* a1)
{
    AddToSingleBucket_478440(a1->field_14_xy.x.ToInt(), a1->field_14_xy.y.ToInt(), a1);
}

MATCH_FUNC(0x477b00)
void PurpleDoom::Remove_477B00(Sprite* a1)
{
    // Note: Single bucket remove only - multi bucket remove doesn't exist
    DoRemove_4782C0(a1->field_14_xy.x.ToInt(), a1->field_14_xy.y.ToInt(), a1);
}

MATCH_FUNC(0x477b20)
void PurpleDoom::AddToRegionBuckets_477B20(Sprite* pSprite)
{
    pSprite->UpdateCollisionBoundsIfNeeded_59E9C0();
    pSprite->field_C_sprite_4c_ptr->SetCurrentRect_5A4D90();
    for (s32 y_pos = gPurple_top_6F6108; y_pos <= gPurple_bottom_6F5F38; ++y_pos)
    {
        AddToRowBuckets_4784D0(y_pos, pSprite);
    }
}

MATCH_FUNC(0x477b60)
void PurpleDoom::AddToSpriteRectBuckets_477B60(Sprite* pSprite)
{
    pSprite->field_C_sprite_4c_ptr->SetCurrentRect_5A4D90();
    for (s32 y_pos = gPurple_top_6F6108; y_pos <= gPurple_bottom_6F5F38; ++y_pos)
    {
        AddToColumnBuckets_478370(y_pos, pSprite);
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
char_type PurpleDoom::CheckAndHandleCollisionInStrips_477BD0(Sprite* pSprite)
{
    char_type bUnknown = 0;

    gCollide_C_6791FC->field_4_count++; // TODO: Prob an inline

    pSprite->UpdateCollisionBoundsIfNeeded_59E9C0();
    pSprite->field_C_sprite_4c_ptr->SetCurrentRect_5A4D90();

    for (s32 i = gPurple_top_6F6108; i <= gPurple_bottom_6F5F38; ++i)
    {
        bUnknown |= CheckAndHandleCollisionsInStrip_478750(i, pSprite);
    }
    return bUnknown;
}

MATCH_FUNC(0x477c30)
bool PurpleDoom::CheckAndHandleAllCollisionsForSprite_477C30(Sprite* pSprt, s32 a3)
{
    dword_678FA8 = a3;
    bool v3 = 0;
    ++gCollide_C_6791FC->field_4_count;
    pSprt->UpdateCollisionBoundsIfNeeded_59E9C0();
    pSprt->field_C_sprite_4c_ptr->SetCurrentRect_5A4D90();
    for (s32 i = gPurple_top_6F6108; i <= gPurple_bottom_6F5F38; ++i)
    {
        v3 |= PurpleDoom::CheckAndHandleRowCollisionsForSprite_4787E0(i, pSprt);
    }
    return v3;
}

MATCH_FUNC(0x477c90)
Sprite* PurpleDoom::FindNearestSprite_SpiralSearch_477C90(s32 sprite_type1,
                                                          s32 sprite_type2,
                                                          Sprite* pExclude,
                                                          u8 max_x_check,
                                                          s32 searchMode,
                                                          char_type bUseSpriteZ)
{
    gPurpleDoom_start_x_679090 = pExclude->field_14_xy.x.ToInt();
    gPurpleDoom_start_y_679098 = pExclude->field_14_xy.y.ToInt();
    gPurpleDoom_sprite_type1_678FE8 = sprite_type1;
    gPurpleDoom_sprite_type2_678FEC = sprite_type2;
    gPurpleDoom_exclude_sprite_678F40 = pExclude;
    gPurpleDoom_ped_678F64 = pExclude->GetPed_59E1B0();
    gPurpleDoom_smallestDistance_678E5C = Fix16(256);
    gPurpleDoom_smallestDistSprite_678E40 = 0;
    gPurpleDoom_search_mode_678FD0 = searchMode;

    if (bUseSpriteZ == 1)
    {
        gPurpleDoom_zpos_max_678F38 = pExclude->field_1C_zpos - k_dword_678F74;
        gPurpleDoom_zpos_min_678F3C = pExclude->field_1C_zpos + k_dword_678F74;
    }
    else
    {
        gPurpleDoom_zpos_max_678F38 = 0;
        gPurpleDoom_zpos_min_678F3C = Fix16(7);
    }

    SearchTileStripForClosestSprite_4781E0(1u);

    Sprite* pCollisionSprite = gPurpleDoom_smallestDistSprite_678E40; // from previous call
    if (searchMode > 1 || !gPurpleDoom_smallestDistSprite_678E40)
    {
        for (u32 x_inc = 2; x_inc <= 2 * max_x_check; x_inc += 2)
        {
            // LEFT COLUMN
            --gPurpleDoom_start_x_679090;
            gPurpleDoom_start_y_679098 += 2 - x_inc;
            SearchTileColumnForClosestSprite_478160(x_inc - 1);

            if (!searchMode)
            {
                pCollisionSprite = gPurpleDoom_smallestDistSprite_678E40;
                if (gPurpleDoom_smallestDistSprite_678E40)
                {
                    break;
                }
            }

            // TOP ROW
            --gPurpleDoom_start_y_679098;
            SearchTileStripForClosestSprite_4781E0(x_inc + 1);

            if (!searchMode)
            {
                pCollisionSprite = gPurpleDoom_smallestDistSprite_678E40;
                if (gPurpleDoom_smallestDistSprite_678E40)
                {
                    break;
                }
            }

            // RIGHT COLUMN
            gPurpleDoom_start_x_679090 += x_inc;
            ++gPurpleDoom_start_y_679098;
            SearchTileColumnForClosestSprite_478160(x_inc - 1);

            if (!searchMode)
            {
                pCollisionSprite = gPurpleDoom_smallestDistSprite_678E40;
                if (gPurpleDoom_smallestDistSprite_678E40)
                {
                    break;
                }
            }

            // BOTTOM ROW
            gPurpleDoom_start_x_679090 -= x_inc;
            gPurpleDoom_start_y_679098 = gPurpleDoom_start_y_679098 + x_inc - 1;
            SearchTileStripForClosestSprite_4781E0(x_inc + 1);

            pCollisionSprite = gPurpleDoom_smallestDistSprite_678E40;

            if (searchMode <= 1)
            {
                if (gPurpleDoom_smallestDistSprite_678E40)
                {
                    break;
                }
            }
        }
    }

    return pCollisionSprite;
}

MATCH_FUNC(0x477E50)
void PurpleDoom::SetSpriteToExclude_477E50(Sprite* pSprite)
{
    gPurpleDoom_exclusion_sprite_678F84 = pSprite;
}

MATCH_FUNC(0x477e60)
Sprite* PurpleDoom::FindNearestSpriteOfType_477E60(Sprite* pSprite, s32 sprite_type_enum)
{
    gPurpleDoom_exclude_type_678F60 = sprite_type_enum;
    gPurpleDoom_smallestDistSprite_678E40 = 0;

    gCollide_C_6791FC->field_4_count++;

    pSprite->UpdateCollisionBoundsIfNeeded_59E9C0();
    pSprite->field_C_sprite_4c_ptr->SetCurrentRect_5A4D90();

    for (s32 top = gPurple_top_6F6108; top <= gPurple_bottom_6F5F38; top++)
    {
        Sprite* pObj = FindNearestSpriteInRow_478880(top, pSprite);
        if (pObj)
        {
            return pObj;
        }
    }

    return gPurpleDoom_smallestDistSprite_678E40;
}

MATCH_FUNC(0x477f30)
bool PurpleDoom::CollectRectCollisions_477F30(Fix16_Rect* union_type, char_type a3, s32 a4, Sprite* a5, struct_4* a6)
{
    gPurpleDoom_list_679214 = a6;
    bool bRet = PurpleDoom::CheckRectForCollisions_477F60(union_type, a3, a4, a5);
    gPurpleDoom_list_679214 = 0;
    return bRet;
}

MATCH_FUNC(0x477f60)
bool PurpleDoom::CheckRectForCollisions_477F60(Fix16_Rect* pRect, char_type a3, s32 a4, Sprite* pSprite)
{
    bool bRet = false;
    ++gCollide_C_6791FC->field_4_count;
    pRect->DoSetCurrentRect_59DD60();
    bDoCollisionCheck_679006 = a3;
    gPurpleDoom_exclude_types_678F88 = a4;
    SetSpriteToExclude_477E50(pSprite);

    for (s32 y_pos = gPurple_top_6F6108; y_pos <= gPurple_bottom_6F5F38; y_pos++)
    {
        if (PurpleDoom::CheckRowForRectCollisions_4785D0(y_pos, pRect))
        {
            if (gPurpleDoom_list_679214)
            {
                bRet = true;
            }
            else
            {
                SetSpriteToExclude_477E50(0);
                return true;
            }
        }
    }
    SetSpriteToExclude_477E50(0);
    return bRet;
}

MATCH_FUNC(0x478040)
PurpleDoom::PurpleDoom()
{
    Clear_4789F0();
}

MATCH_FUNC(0x478A10)
void PurpleDoom::Empty_478A10()
{
}

MATCH_FUNC(0x478050)
PurpleDoom::~PurpleDoom()
{
    Empty_478A10();
}

STUB_FUNC(0x478160)
u32 PurpleDoom::SearchTileColumnForClosestSprite_478160(u8 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x478060)
void PurpleDoom::CheckTileSpritesForClosestMatch_478060(Collide_8* pStart)
{
    Fix16 dist;
    for (Collide_8* pColIter = pStart; pColIter; pColIter = pColIter->mpNext)
    {
        Sprite* pSprt = pColIter->field_0_sprt;
        if ((pColIter->field_0_sprt->field_30_sprite_type_enum == gPurpleDoom_sprite_type1_678FE8 ||
             pColIter->field_0_sprt->field_30_sprite_type_enum == gPurpleDoom_sprite_type2_678FEC) &&
            pSprt != gPurpleDoom_exclude_sprite_678F40)
        {

            if (pSprt->field_1C_zpos <= gPurpleDoom_zpos_min_678F3C && pSprt->field_1C_zpos >= gPurpleDoom_zpos_max_678F38)
            {
                if (gPurpleDoom_search_mode_678FD0 == 3)
                {
                    dist = gPurpleDoom_exclude_sprite_678F40->MinDistanceToAnySpriteBBoxCorner_5A22B0(pSprt);
                    if (dist < gPurpleDoom_smallestDistance_678E5C)
                    {
                        if (pColIter->field_0_sprt->field_30_sprite_type_enum == sprite_types_enum::car)
                        {
                            if (pColIter->field_0_sprt->field_8_car_bc_ptr->sub_445360())
                            {
                                gPurpleDoom_smallestDistance_678E5C = dist;
                                gPurpleDoom_smallestDistSprite_678E40 = pColIter->field_0_sprt;
                            }
                        }
                        else
                        {
                            gPurpleDoom_smallestDistance_678E5C = dist;
                            gPurpleDoom_smallestDistSprite_678E40 = pColIter->field_0_sprt;
                        }
                    }
                }
                else
                {

                    dist = Fix16::Abs(pSprt->field_14_xy.x - gPurpleDoom_exclude_sprite_678F40->field_14_xy.x) +
                        Fix16::Abs(pSprt->field_14_xy.y - gPurpleDoom_exclude_sprite_678F40->field_14_xy.y);

                    if (dist < gPurpleDoom_smallestDistance_678E5C)
                    {
                        if (pSprt->IsThreatToSearchingPed_59E830())
                        {
                            gPurpleDoom_smallestDistance_678E5C = dist;
                            gPurpleDoom_smallestDistSprite_678E40 = pColIter->field_0_sprt;
                        }
                    }
                }
            }
        }
    }
}

MATCH_FUNC(0x4781E0)
void PurpleDoom::SearchTileStripForClosestSprite_4781E0(u8 width)
{
    gPurple_left_6F5FD4 = gPurpleDoom_start_x_679090;
    gPurple_right_6F5B80 = gPurpleDoom_start_x_679090 + width - 1;

    for (PurpleDoom_C* pXItemIter = GetFirstXCellInRow_478590(gPurpleDoom_start_y_679098); pXItemIter; pXItemIter = pXItemIter->mpNext)
    {
        if (pXItemIter->field_0_x_len > gPurple_right_6F5B80)
        {
            break;
        }
        CheckTileSpritesForClosestMatch_478060(pXItemIter->field_4_p8);
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
            for (PurpleDoom_C* pXItem = *pYItem; pXItem; pXItem = pXItem->mpNext)
            {
                const s32 x_cell = pXItem->field_0_x_len;
                if (x_cell > right)
                {
                    break;
                }
                if (x_cell >= left)
                {
                    for (Collide_8* p8Iter = pXItem->field_4_p8; p8Iter; p8Iter = p8Iter->mpNext)
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

    for (PurpleDoom_C* pXIter = this->field_0[y_pos]; pXIter; pXIter = pXIter->mpNext)
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
                        pXIter->field_4_p8 = pCollideForX->mpNext;
                    }
                    else
                    {
                        pFoundCollideForX->mpNext = pCollideForX->mpNext;
                    }

                    gCollide_8_Pool_679200->DeAllocate(pCollideForX);

                    if (!pXIter->field_4_p8)
                    {
                        if (!pFound)
                        {
                            this->field_0[y_pos] = pXIter->mpNext;
                        }
                        else
                        {
                            pFound->mpNext = pXIter->mpNext;
                        }
                        gPurpleDoom_C_Pool_679204->DeAllocate(pXIter);
                    }
                    return;
                }

                pFoundCollideForX = pCollideForX;
                pCollideForX = pCollideForX->mpNext;
            }
        }
        pFound = pXIter;
    }
}

MATCH_FUNC(0x478370)
void PurpleDoom::AddToColumnBuckets_478370(s32 y_pos, Sprite* pSprite)
{
    s32 x_pos = gPurple_left_6F5FD4;
    PurpleDoom_C* pLastXIter = 0;
    for (PurpleDoom_C* pXItemIter = this->field_0[y_pos]; pXItemIter; pXItemIter = pXItemIter->mpNext)
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
                        pXItemIter->field_4_p8 = pObj->mpNext;
                    }
                    else
                    {
                        pLast->mpNext = pObj->mpNext;
                    }

                    gCollide_8_Pool_679200->DeAllocate(pObj);

                    if (!pXItemIter->field_4_p8)
                    {
                        PurpleDoom_C* pNext = pXItemIter->mpNext;
                        if (!pLastXIter)
                        {
                            this->field_0[y_pos] = pNext;
                        }
                        else
                        {
                            pLastXIter->mpNext = pNext;
                        }
                        pXItemIter = gPurpleDoom_C_Pool_679204->UnlinkAndReturnNext(pXItemIter);
                    }
                    else
                    {
                        pLastXIter = pXItemIter;
                        pXItemIter = pXItemIter->mpNext;
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
                    pObj = pObj->mpNext;
                }
            }
        }
        pLastXIter = pXItemIter;
    }
}

MATCH_FUNC(0x478440)
void PurpleDoom::AddToSingleBucket_478440(s32 xpos, s32 ypos, Sprite* pSprite)
{
    Collide_8* pNewCollide = gCollide_8_Pool_679200->Allocate();

    pNewCollide->field_0_sprt = pSprite;

    PurpleDoom_C* pAddedTo = 0;
    PurpleDoom_C* pIter;
    for (pIter = this->field_0[ypos]; pIter; pIter = pIter->mpNext)
    {
        const s32 x_len = pIter->field_0_x_len;
        if (x_len > xpos)
        {
            break;
        }
        if (x_len == xpos)
        {
            pNewCollide->mpNext = pIter->field_4_p8;
            pIter->field_4_p8 = pNewCollide;
            return;
        }
        pAddedTo = pIter;
    }

    PurpleDoom_C* pNewItem = gPurpleDoom_C_Pool_679204->Allocate();

    if (!pAddedTo)
    {
        this->field_0[ypos] = pNewItem;
    }
    else
    {
        pAddedTo->mpNext = pNewItem;
    }

    pNewItem->mpNext = pIter;
    pNewItem->field_4_p8 = pNewCollide;
    pNewItem->field_0_x_len = xpos;
    pNewCollide->mpNext = 0;
}

MATCH_FUNC(0x4784d0)
void PurpleDoom::AddToRowBuckets_4784D0(s32 y_pos, Sprite* pSprite)
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
                Collide_8* v7 = gCollide_8_Pool_679200->Allocate();
                v7->field_0_sprt = pSprite;
                v7->mpNext = pNewNext->field_4_p8;
                pNewNext->field_4_p8 = v7;
                if (purple_left > gPurple_right_6F5B80)
                {
                    return;
                }
            }
            purple_x = pNewNext;
            pNewNext = pNewNext->mpNext;
        }

        Collide_8* v8 = gCollide_8_Pool_679200->Allocate();
        v8->field_0_sprt = pSprite;
        PurpleDoom_C* pCIter = gPurpleDoom_C_Pool_679204->Allocate();

        if (!purple_x)
        {
            this->field_0[y_pos] = pCIter;
        }
        else
        {
            purple_x->mpNext = pCIter;
        }
        pCIter->mpNext = pNewNext;
        pCIter->field_4_p8 = v8;
        pCIter->field_0_x_len = purple_left;
        v8->mpNext = 0;
        pNewNext = pCIter->mpNext;
        ++purple_left;
        purple_x = pCIter;
    }
}

// Get first XItem at y_pos
MATCH_FUNC(0x478590)
PurpleDoom_C* PurpleDoom::GetFirstXCellInRow_478590(s32 start_idx)
{
    PurpleDoom_C* pIter;
    s32 f0;

    if (start_idx < 0 || start_idx > 255)
    {
        return 0;
    }
    for (pIter = this->field_0[start_idx]; pIter; pIter = pIter->mpNext)
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
MATCH_FUNC(0x4785d0)
char_type PurpleDoom::CheckRowForRectCollisions_4785D0(u32 y_pos, Fix16_Rect* pRect)
{
    PurpleDoom_C* pXItemIter = GetFirstXCellInRow_478590(y_pos);
    char bRet = 0;
    while (pXItemIter)
    {
        if (pXItemIter->field_0_x_len > gPurple_right_6F5B80)
        {
            return bRet;
        }

        for (Collide_8* pObj = pXItemIter->field_4_p8; pObj; pObj = pObj->mpNext)
        {
            Sprite* field_0_sprt = pObj->field_0_sprt;
            if (!pObj->field_0_sprt->TypeIs_446940(gPurpleDoom_exclude_types_678F88) &&
                field_0_sprt != gPurpleDoom_exclusion_sprite_678F84 &&
                !pObj->field_0_sprt->field_C_sprite_4c_ptr->CollisionIdIs_446930(gCollide_C_6791FC->field_4_count) &&
                field_0_sprt->sub_59E850(0))
            {
                //  83:    inc     %edx
                gCollide_C_6791FC->field_0_count++;

                Fix16_Rect* pBBox = &pObj->field_0_sprt->field_C_sprite_4c_ptr->field_30_boundingBox;
                if (pRect->AABB_Intersects_41E2F0(pBBox) &&
                    (!bDoCollisionCheck_679006 || pObj->field_0_sprt->field_C_sprite_4c_ptr->IsZeroWidth_41E390() ||
                     pObj->field_0_sprt->field_0.jIsAxisAligned_41E3C0() || pObj->field_0_sprt->sub_59FB10(pRect) ||
                     pRect->IntersectsSpriteRenderingRect_59DDF0(pObj->field_0_sprt)))
                {
                    if (gPurpleDoom_list_679214)
                    {
                        bRet = 1;
                        // Add to list and keep going to add more
                        gPurpleDoom_list_679214->AddSprite_5A6CD0(pObj->field_0_sprt);
                    }
                    else
                    {
                        // No list so stop here
                        return 1;
                    }
                }

                pObj->field_0_sprt->field_C_sprite_4c_ptr->SetCollisionId_446920(gCollide_C_6791FC->field_4_count);
            }

        } // end for
        pXItemIter = pXItemIter->mpNext;
    } // end while
    return bRet;
}

MATCH_FUNC(0x478750)
char_type PurpleDoom::CheckAndHandleCollisionsInStrip_478750(u32 y_pos, Sprite* pSprite)
{
    char_type bRet = 0;
    PurpleDoom_C* pIter = GetFirstXCellInRow_478590(y_pos);
    while (pIter)
    {
        if (pIter->field_0_x_len > gPurple_right_6F5B80)
        {
            break;
        }

        for (Collide_8* pC8Iter = pIter->field_4_p8; pC8Iter; pC8Iter = pC8Iter->mpNext)
        {
            if (!pC8Iter->field_0_sprt->field_C_sprite_4c_ptr->CollisionIdIs_446930(gCollide_C_6791FC->field_4_count))
            {
                gCollide_C_6791FC->field_0_count++;
                if (pSprite->CollisionCheck_59E590(pC8Iter->field_0_sprt))
                {
                    bRet = 1;
                    pC8Iter->field_0_sprt->HandleObjectCollision_59E8C0(pSprite);
                }
                pC8Iter->field_0_sprt->field_C_sprite_4c_ptr->SetCollisionId_446920(gCollide_C_6791FC->field_4_count);
            }
        }
        pIter = pIter->mpNext;
    }
    return bRet;
}

MATCH_FUNC(0x4787e0)
bool PurpleDoom::CheckAndHandleRowCollisionsForSprite_4787E0(u32 y_pos, Sprite* pSprite)
{
    bool bRet = false;
    PurpleDoom_C* pXItemIter = GetFirstXCellInRow_478590(y_pos);
    while (pXItemIter)
    {
        if (pXItemIter->field_0_x_len > gPurple_right_6F5B80)
        {
            break;
        }

        for (Collide_8* p8Iter = pXItemIter->field_4_p8; p8Iter; p8Iter = p8Iter->mpNext)
        {
            if (p8Iter->field_0_sprt->TypeIs_446940(dword_678FA8) &&
                !p8Iter->field_0_sprt->field_C_sprite_4c_ptr->CollisionIdIs_446930(gCollide_C_6791FC->field_4_count))
            {
                gCollide_C_6791FC->field_0_count++;

                if (pSprite->CollisionCheck_59E590(p8Iter->field_0_sprt))
                {
                    bRet = true;
                    p8Iter->field_0_sprt->ProcessCarToCarImpactIfCar_59E910(pSprite);
                }

                p8Iter->field_0_sprt->field_C_sprite_4c_ptr->SetCollisionId_446920(gCollide_C_6791FC->field_4_count);
            }
        }
        pXItemIter = pXItemIter->mpNext;
    }

    return bRet;
}

MATCH_FUNC(0x478880)
Sprite* PurpleDoom::FindNearestSpriteInRow_478880(u32 y_pos, Sprite* pSprite)
{
    PurpleDoom_C* pXItem = GetFirstXCellInRow_478590(y_pos);
    while (pXItem)
    {
        if (pXItem->field_0_x_len > gPurple_right_6F5B80)
        {
            return 0;
        }

        for (Collide_8* pObj = pXItem->field_4_p8; pObj; pObj = pObj->mpNext)
        {
            Sprite* pSprt = pObj->field_0_sprt;
            if (pObj->field_0_sprt == gPurpleDoom_exclusion_sprite_678F84 ||
                gPurpleDoom_smallestDistSprite_678E40 && !pSprt->TypeIs_446940(gPurpleDoom_exclude_type_678F60) ||
                pSprt->field_C_sprite_4c_ptr->CollisionIdIs_446930(gCollide_C_6791FC->field_4_count) || !pSprt->sub_59E850(pSprite))
            {
                continue;
            }

            gCollide_C_6791FC->field_0_count++;

            if (pSprite->CollisionCheck_59E590(pObj->field_0_sprt))
            {
                if (!pObj->field_0_sprt->TypeIs_446940(gPurpleDoom_exclude_type_678F60))
                {
                    gPurpleDoom_smallestDistSprite_678E40 = pObj->field_0_sprt;
                }
                else
                {
                    return pObj->field_0_sprt;
                }
            }
            pObj->field_0_sprt->field_C_sprite_4c_ptr->SetCollisionId_446920(gCollide_C_6791FC->field_4_count);
        }
        pXItem = pXItem->mpNext;
    }
    return 0;
}

MATCH_FUNC(0x478950)
void PurpleDoom::DebugLog_478950(s32 xpos, s32 ypos)
{
    for (PurpleDoom_C* i = field_0[ypos]; i; i = i->mpNext)
    {
        const s32 x_len = i->field_0_x_len;
        if (x_len > xpos)
        {
            break;
        }

        if (x_len == xpos)
        {
            sprintf(gTmpBuffer_67C598, "(%d,%d):", xpos, ypos);
            gFile_67C530.Write_Log_4D9650(gTmpBuffer_67C598);
            for (Collide_8* j = i->field_4_p8; j; j = j->mpNext)
            {
                if (j->field_0_sprt)
                {
                    sprintf(gTmpBuffer_67C598, " %d", (u16)j->field_0_sprt->field_20_id);
                    gFile_67C530.Write_Log_4D9650(gTmpBuffer_67C598);
                }
            }
            gFile_67C530.Write_Log_4D9650("\n");
        }
    }
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