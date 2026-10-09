#include "SpriteGrid_400.hpp"
#include "Camera.hpp"
#include "Car_BC.hpp"
#include "Globals.hpp"
#include "SpriteRenderer_1C.hpp"
#include "Object_5C.hpp"
#include "collide.hpp"
#include "error.hpp"
#include "map_0x370.hpp"
#include "sprite.hpp"

// Multi/regional bucket stuff, Cars/Peds/Cranes/Objects
DEFINE_GLOBAL(SpriteGrid_400*, gSpriteGrid_1_679208, 0x679208);

// Used by buses, car-trailer combo, Char_B4 in certain states and objects with certain collision category
DEFINE_GLOBAL(SpriteGrid_400*, gSpriteGrid_2_67920C, 0x67920C);

// Single bucket stuff, mostly particles
DEFINE_GLOBAL(SpriteGrid_400*, gSpriteGrid_3_679210, 0x679210);

DEFINE_GLOBAL(s32, gSpriteGrid_bottom_6F5F38, 0x6F5F38);
DEFINE_GLOBAL(s32, gSpriteGrid_top_6F6108, 0x6F6108);
DEFINE_GLOBAL(s32, gSpriteGrid_sprite_type_filter_678FA8, 0x678FA8);
DEFINE_GLOBAL(s32, gSpriteGrid_exclude_type_678F60, 0x678F60);
DEFINE_GLOBAL(Sprite*, gSpriteGrid_smallestDistSprite_678E40, 0x678E40);
DEFINE_GLOBAL(u8, bDoCollisionCheck_679006, 0x679006);
DEFINE_GLOBAL(s32, gSpriteGrid_exclude_types_678F88, 0x678F88);
DEFINE_GLOBAL(s32, gSpriteGrid_start_x_679090, 0x679090);
DEFINE_GLOBAL(s32, gSpriteGrid_start_y_679098, 0x679098);
DEFINE_GLOBAL(struct_4*, gSpriteGrid_list_679214, 0x679214);
EXTERN_GLOBAL(CollisionCounters_C*, gCollisionCounters_6791FC);
EXTERN_GLOBAL(GridCell_Pool*, gGridCell_Pool_679204);
EXTERN_GLOBAL(GridSpriteLink_Pool*, gGridSpriteLink_Pool_679200);
DEFINE_GLOBAL(Sprite*, gSpriteGrid_exclusion_sprite_678F84, 0x678F84);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_678F74, Fix16(0x2000, 0), 0x678F74);
DEFINE_GLOBAL(s32, gSpriteGrid_sprite_type1_678FE8, 0x678FE8);
DEFINE_GLOBAL(s32, gSpriteGrid_sprite_type2_678FEC, 0x678FEC);
DEFINE_GLOBAL(Sprite*, gSpriteGrid_exclude_sprite_678F40, 0x678F40);
DEFINE_GLOBAL(Ped*, gSpriteGrid_ped_678F64, 0x678F64);
DEFINE_GLOBAL(Fix16, gSpriteGrid_smallestDistance_678E5C, 0x678E5C);
DEFINE_GLOBAL(Fix16, gSpriteGrid_zpos_max_678F38, 0x678F38);
DEFINE_GLOBAL(Fix16, gSpriteGrid_zpos_min_678F3C, 0x678F3C);
DEFINE_GLOBAL(s32, gSpriteGrid_search_mode_678FD0, 0x678FD0);

DEFINE_GLOBAL_INIT(Fix16, kFpOneAndHalf_678F80, Fix16(0x6000, 0), 0x678F80);
DEFINE_GLOBAL_INIT(Fix16, kFpOne_679084, Fix16(1), 0x679084);

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
void SpriteGrid_400::DrawSpritesClipped_477A40()
{
    const s32 left = Clamp((gViewCamera_676978->field_78_boundaries_non_neg.field_0_left - kFpOne_679084).ToInt(), 0, 255);
    const s32 right_val = Clamp((kFpOneAndHalf_678F80 + gViewCamera_676978->field_78_boundaries_non_neg.field_4_right).ToInt(), 0, 255);
    const s32 top_val = Clamp((gViewCamera_676978->field_78_boundaries_non_neg.field_8_top - kFpOne_679084).ToInt(), 0, 255);
    const s32 bottom_val = Clamp((kFpOneAndHalf_678F80 + gViewCamera_676978->field_78_boundaries_non_neg.field_C_bottom).ToInt(), 0, 255);

    AddToDrawList_478240(left, right_val, top_val, bottom_val);
}

MATCH_FUNC(0x477ae0)
void SpriteGrid_400::AddToSingleBucket_477AE0(Sprite* pSprite)
{
    AddToSingleBucket_478440(pSprite->field_14_xy.x.ToInt(), pSprite->field_14_xy.y.ToInt(), pSprite);
}

MATCH_FUNC(0x477b00)
void SpriteGrid_400::Remove_477B00(Sprite* pSprite)
{
    // Note: Single bucket remove only - multi bucket remove doesn't exist
    DoRemove_4782C0(pSprite->field_14_xy.x.ToInt(), pSprite->field_14_xy.y.ToInt(), pSprite);
}

MATCH_FUNC(0x477b20)
void SpriteGrid_400::AddToRegionBuckets_477B20(Sprite* pSprite)
{
    pSprite->UpdateCollisionBoundsIfNeeded_59E9C0();
    pSprite->field_C_sprite_4c_ptr->SetCurrentRect_5A4D90();
    for (s32 y_pos = gSpriteGrid_top_6F6108; y_pos <= gSpriteGrid_bottom_6F5F38; ++y_pos)
    {
        AddToRowBuckets_4784D0(y_pos, pSprite);
    }
}

MATCH_FUNC(0x477b60)
void SpriteGrid_400::AddToSpriteRectBuckets_477B60(Sprite* pSprite)
{
    pSprite->field_C_sprite_4c_ptr->SetCurrentRect_5A4D90();
    for (s32 y_pos = gSpriteGrid_top_6F6108; y_pos <= gSpriteGrid_bottom_6F5F38; ++y_pos)
    {
        AddToColumnBuckets_478370(y_pos, pSprite);
    }
}

MATCH_FUNC(0x477ba0)
void SpriteGrid_400::DebugLogAll_477BA0()
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
char_type SpriteGrid_400::CheckAndHandleCollisionInStrips_477BD0(Sprite* pSprite)
{
    char_type bHit = 0;

    gCollisionCounters_6791FC->field_4_query_id++; // TODO: Prob an inline

    pSprite->UpdateCollisionBoundsIfNeeded_59E9C0();
    pSprite->field_C_sprite_4c_ptr->SetCurrentRect_5A4D90();

    for (s32 i = gSpriteGrid_top_6F6108; i <= gSpriteGrid_bottom_6F5F38; ++i)
    {
        bHit |= CheckAndHandleCollisionsInStrip_478750(i, pSprite);
    }
    return bHit;
}

MATCH_FUNC(0x477c30)
bool SpriteGrid_400::CheckAndHandleAllCollisionsForSprite_477C30(Sprite* pSprite, s32 sprite_type_filter)
{
    gSpriteGrid_sprite_type_filter_678FA8 = sprite_type_filter;
    bool bHit = 0;
    ++gCollisionCounters_6791FC->field_4_query_id;
    pSprite->UpdateCollisionBoundsIfNeeded_59E9C0();
    pSprite->field_C_sprite_4c_ptr->SetCurrentRect_5A4D90();
    for (s32 i = gSpriteGrid_top_6F6108; i <= gSpriteGrid_bottom_6F5F38; ++i)
    {
        bHit |= SpriteGrid_400::CheckAndHandleRowCollisionsForSprite_4787E0(i, pSprite);
    }
    return bHit;
}

MATCH_FUNC(0x477c90)
Sprite* SpriteGrid_400::FindNearestSprite_SpiralSearch_477C90(s32 primary_sprite_type,
                                                          s32 secondary_sprite_type,
                                                          Sprite* pExclude,
                                                          u8 max_rings,
                                                          s32 search_mode,
                                                          char_type bUseSpriteZ)
{
    gSpriteGrid_start_x_679090 = pExclude->field_14_xy.x.ToInt();
    gSpriteGrid_start_y_679098 = pExclude->field_14_xy.y.ToInt();
    gSpriteGrid_sprite_type1_678FE8 = primary_sprite_type;
    gSpriteGrid_sprite_type2_678FEC = secondary_sprite_type;
    gSpriteGrid_exclude_sprite_678F40 = pExclude;
    gSpriteGrid_ped_678F64 = pExclude->GetPed_59E1B0();
    gSpriteGrid_smallestDistance_678E5C = Fix16(256);
    gSpriteGrid_smallestDistSprite_678E40 = 0;
    gSpriteGrid_search_mode_678FD0 = search_mode;

    if (bUseSpriteZ == 1)
    {
        gSpriteGrid_zpos_max_678F38 = pExclude->field_1C_zpos - kFpHalf_678F74;
        gSpriteGrid_zpos_min_678F3C = pExclude->field_1C_zpos + kFpHalf_678F74;
    }
    else
    {
        gSpriteGrid_zpos_max_678F38 = 0;
        gSpriteGrid_zpos_min_678F3C = Fix16(7);
    }

    SearchTileStripForClosestSprite_4781E0(1u);

    Sprite* pCollisionSprite = gSpriteGrid_smallestDistSprite_678E40; // from previous call
    if (search_mode > 1 || !gSpriteGrid_smallestDistSprite_678E40)
    {
        for (u32 ring_size = 2; ring_size <= 2 * max_rings; ring_size += 2)
        {
            // LEFT COLUMN
            --gSpriteGrid_start_x_679090;
            gSpriteGrid_start_y_679098 += 2 - ring_size;
            SearchTileColumnForClosestSprite_478160(ring_size - 1);

            if (!search_mode)
            {
                pCollisionSprite = gSpriteGrid_smallestDistSprite_678E40;
                if (gSpriteGrid_smallestDistSprite_678E40)
                {
                    break;
                }
            }

            // TOP ROW
            --gSpriteGrid_start_y_679098;
            SearchTileStripForClosestSprite_4781E0(ring_size + 1);

            if (!search_mode)
            {
                pCollisionSprite = gSpriteGrid_smallestDistSprite_678E40;
                if (gSpriteGrid_smallestDistSprite_678E40)
                {
                    break;
                }
            }

            // RIGHT COLUMN
            gSpriteGrid_start_x_679090 += ring_size;
            ++gSpriteGrid_start_y_679098;
            SearchTileColumnForClosestSprite_478160(ring_size - 1);

            if (!search_mode)
            {
                pCollisionSprite = gSpriteGrid_smallestDistSprite_678E40;
                if (gSpriteGrid_smallestDistSprite_678E40)
                {
                    break;
                }
            }

            // BOTTOM ROW
            gSpriteGrid_start_x_679090 -= ring_size;
            gSpriteGrid_start_y_679098 = gSpriteGrid_start_y_679098 + ring_size - 1;
            SearchTileStripForClosestSprite_4781E0(ring_size + 1);

            pCollisionSprite = gSpriteGrid_smallestDistSprite_678E40;

            if (search_mode <= 1)
            {
                if (gSpriteGrid_smallestDistSprite_678E40)
                {
                    break;
                }
            }
        }
    }

    return pCollisionSprite;
}

MATCH_FUNC(0x477E50)
void SpriteGrid_400::SetSpriteToExclude_477E50(Sprite* pSprite)
{
    gSpriteGrid_exclusion_sprite_678F84 = pSprite;
}

MATCH_FUNC(0x477e60)
Sprite* SpriteGrid_400::FindNearestSpriteOfType_477E60(Sprite* pSprite, s32 desired_sprite_type)
{
    gSpriteGrid_exclude_type_678F60 = desired_sprite_type;
    gSpriteGrid_smallestDistSprite_678E40 = 0;

    gCollisionCounters_6791FC->field_4_query_id++;

    pSprite->UpdateCollisionBoundsIfNeeded_59E9C0();
    pSprite->field_C_sprite_4c_ptr->SetCurrentRect_5A4D90();

    for (s32 top = gSpriteGrid_top_6F6108; top <= gSpriteGrid_bottom_6F5F38; top++)
    {
        Sprite* pObj = FindNearestSpriteInRow_478880(top, pSprite);
        if (pObj)
        {
            return pObj;
        }
    }

    return gSpriteGrid_smallestDistSprite_678E40;
}

MATCH_FUNC(0x477f30)
bool SpriteGrid_400::CollectRectCollisions_477F30(Fix16_Rect* pRect, char_type bDeepCheck, s32 exclude_sprite_type, Sprite* pExclude, struct_4* pOutList)
{
    gSpriteGrid_list_679214 = pOutList;
    bool bRet = SpriteGrid_400::CheckRectForCollisions_477F60(pRect, bDeepCheck, exclude_sprite_type, pExclude);
    gSpriteGrid_list_679214 = 0;
    return bRet;
}

MATCH_FUNC(0x477f60)
bool SpriteGrid_400::CheckRectForCollisions_477F60(Fix16_Rect* pRect, char_type bDeepCheck, s32 exclude_sprite_type, Sprite* pExclude)
{
    bool bRet = false;
    ++gCollisionCounters_6791FC->field_4_query_id;
    pRect->DoSetCurrentRect_59DD60();
    bDoCollisionCheck_679006 = bDeepCheck;
    gSpriteGrid_exclude_types_678F88 = exclude_sprite_type;
    SetSpriteToExclude_477E50(pExclude);

    for (s32 y_pos = gSpriteGrid_top_6F6108; y_pos <= gSpriteGrid_bottom_6F5F38; y_pos++)
    {
        if (SpriteGrid_400::CheckRowForRectCollisions_4785D0(y_pos, pRect))
        {
            if (gSpriteGrid_list_679214)
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
SpriteGrid_400::SpriteGrid_400()
{
    Clear_4789F0();
}

MATCH_FUNC(0x478050)
SpriteGrid_400::~SpriteGrid_400()
{
    Empty_478A10();
}

MATCH_FUNC(0x478060)
void SpriteGrid_400::CheckTileSpritesForClosestMatch_478060(GridSpriteLink_8* pSpriteLinks)
{
    Fix16 dist;
    for (GridSpriteLink_8* pColIter = pSpriteLinks; pColIter; pColIter = pColIter->mpNext)
    {
        Sprite* pSprt = pColIter->field_0_pSprite;
        if ((pColIter->field_0_pSprite->TypeIs_446940(gSpriteGrid_sprite_type1_678FE8) ||
             pColIter->field_0_pSprite->TypeIs_446940(gSpriteGrid_sprite_type2_678FEC)) &&
            pSprt != gSpriteGrid_exclude_sprite_678F40)
        {

            if (pSprt->field_1C_zpos <= gSpriteGrid_zpos_min_678F3C && pSprt->field_1C_zpos >= gSpriteGrid_zpos_max_678F38)
            {
                if (gSpriteGrid_search_mode_678FD0 == 3)
                {
                    dist = gSpriteGrid_exclude_sprite_678F40->MinDistanceToAnySpriteBBoxCorner_5A22B0(pSprt);
                    if (dist < gSpriteGrid_smallestDistance_678E5C)
                    {
                        if (pColIter->field_0_pSprite->get_type_416B40() == sprite_types_enum::car_2)
                        {
                            if (pColIter->field_0_pSprite->field_8_car_bc_ptr->IsEnterable_445360())
                            {
                                gSpriteGrid_smallestDistance_678E5C = dist;
                                gSpriteGrid_smallestDistSprite_678E40 = pColIter->field_0_pSprite;
                            }
                        }
                        else
                        {
                            gSpriteGrid_smallestDistance_678E5C = dist;
                            gSpriteGrid_smallestDistSprite_678E40 = pColIter->field_0_pSprite;
                        }
                    }
                }
                else
                {

                    dist = gSpriteGrid_exclude_sprite_678F40->ManhattanDistance_446960(pSprt);

                    if (dist < gSpriteGrid_smallestDistance_678E5C)
                    {
                        if (pSprt->IsThreatToSearchingPed_59E830())
                        {
                            gSpriteGrid_smallestDistance_678E5C = dist;
                            gSpriteGrid_smallestDistSprite_678E40 = pColIter->field_0_pSprite;
                        }
                    }
                }
            }
        }
    }
}

MATCH_FUNC(0x478160)
void SpriteGrid_400::SearchTileColumnForClosestSprite_478160(u8 height)
{
    s32 y_pos = gSpriteGrid_start_y_679098;
    if (gSpriteGrid_start_y_679098 < gSpriteGrid_start_y_679098 + (u32)height)
    {
        // Won't match without this redundant iter
        GridCell_C** pXItemIter = &this->field_0_rows[gSpriteGrid_start_y_679098];
        while (y_pos < gSpriteGrid_start_y_679098 + (u32)height)
        {
            CheckTileSpritesForClosestMatch_478060(GetCollideListAt_446820(gSpriteGrid_start_x_679090, y_pos));
            ++y_pos;
            ++pXItemIter;
        }
    }
}

MATCH_FUNC(0x4781E0)
void SpriteGrid_400::SearchTileStripForClosestSprite_4781E0(u8 width)
{
    gSpriteGrid_left_6F5FD4 = gSpriteGrid_start_x_679090;
    gSpriteGrid_right_6F5B80 = gSpriteGrid_start_x_679090 + width - 1;

    for (GridCell_C* pXItemIter = GetFirstXCellInRow_478590(gSpriteGrid_start_y_679098); pXItemIter; pXItemIter = pXItemIter->mpNext)
    {
        if (pXItemIter->field_0_x > gSpriteGrid_right_6F5B80)
        {
            break;
        }
        CheckTileSpritesForClosestMatch_478060(pXItemIter->field_4_pSpriteLinks);
    }
}

MATCH_FUNC(0x478240)
void SpriteGrid_400::AddToDrawList_478240(s32 left, s32 right, s32 top, s32 bottom)
{
    GridCell_C** pYItem = &this->field_0_rows[top]; // y_start?
    if (top <= bottom)
    {
        s32 y_total = bottom - top + 1;
        do
        {
            for (GridCell_C* pXItem = *pYItem; pXItem; pXItem = pXItem->mpNext)
            {
                const s32 x_cell = pXItem->field_0_x;
                if (x_cell > right)
                {
                    break;
                }
                if (x_cell >= left)
                {
                    for (GridSpriteLink_8* p8Iter = pXItem->field_4_pSpriteLinks; p8Iter; p8Iter = p8Iter->mpNext)
                    {
                        if (p8Iter->field_0_pSprite->IsTypeAbove1_446950())
                        {
                            gSpriteRenderer_67B580->DisplayAdd_495510(p8Iter->field_0_pSprite);
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
void SpriteGrid_400::DoRemove_4782C0(s32 x_pos, s32 y_pos, Sprite* pToFind)
{
    GridCell_C* pFound = 0;
    GridSpriteLink_8* pFoundCollideForX = 0;

    for (GridCell_C* pXIter = this->field_0_rows[y_pos]; pXIter; pXIter = pXIter->mpNext)
    {
        if (pXIter->field_0_x == x_pos)
        {
            GridSpriteLink_8* pCollideForX = pXIter->field_4_pSpriteLinks;
            while (pCollideForX)
            {
                if (pCollideForX->field_0_pSprite == pToFind)
                {
                    if (!pFoundCollideForX)
                    {
                        pXIter->field_4_pSpriteLinks = pCollideForX->mpNext;
                    }
                    else
                    {
                        pFoundCollideForX->mpNext = pCollideForX->mpNext;
                    }

                    gGridSpriteLink_Pool_679200->DeAllocate(pCollideForX);

                    if (!pXIter->field_4_pSpriteLinks)
                    {
                        if (!pFound)
                        {
                            this->field_0_rows[y_pos] = pXIter->mpNext;
                        }
                        else
                        {
                            pFound->mpNext = pXIter->mpNext;
                        }
                        gGridCell_Pool_679204->DeAllocate(pXIter);
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
void SpriteGrid_400::AddToColumnBuckets_478370(s32 y_pos, Sprite* pSprite)
{
    s32 x_pos = gSpriteGrid_left_6F5FD4;
    GridCell_C* pLastXIter = 0;
    for (GridCell_C* pXItemIter = this->field_0_rows[y_pos]; pXItemIter; pXItemIter = pXItemIter->mpNext)
    {
        if (pXItemIter->field_0_x == x_pos)
        {
            GridSpriteLink_8* pObj = pXItemIter->field_4_pSpriteLinks;
            GridSpriteLink_8* pLast = 0;
            while (pObj)
            {
                if (pObj->field_0_pSprite == pSprite)
                {
                    if (!pLast)
                    {
                        pXItemIter->field_4_pSpriteLinks = pObj->mpNext;
                    }
                    else
                    {
                        pLast->mpNext = pObj->mpNext;
                    }

                    gGridSpriteLink_Pool_679200->DeAllocate(pObj);

                    if (!pXItemIter->field_4_pSpriteLinks)
                    {
                        GridCell_C* pNext = pXItemIter->mpNext;
                        if (!pLastXIter)
                        {
                            this->field_0_rows[y_pos] = pNext;
                        }
                        else
                        {
                            pLastXIter->mpNext = pNext;
                        }
                        pXItemIter = gGridCell_Pool_679204->UnlinkAndReturnNext(pXItemIter);
                    }
                    else
                    {
                        pLastXIter = pXItemIter;
                        pXItemIter = pXItemIter->mpNext;
                    }
                    ++x_pos;

                    if (x_pos > gSpriteGrid_right_6F5B80)
                    {
                        return;
                    }
                    pObj = pXItemIter->field_4_pSpriteLinks;
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
void SpriteGrid_400::AddToSingleBucket_478440(s32 x_pos, s32 y_pos, Sprite* pSprite)
{
    GridSpriteLink_8* pNewCollide = gGridSpriteLink_Pool_679200->Allocate();

    pNewCollide->field_0_pSprite = pSprite;

    GridCell_C* pPrevCell = 0;
    GridCell_C* pCell;
    for (pCell = this->field_0_rows[y_pos]; pCell; pCell = pCell->mpNext)
    {
        const s32 x_len = pCell->field_0_x;
        if (x_len > x_pos)
        {
            break;
        }
        if (x_len == x_pos)
        {
            pNewCollide->mpNext = pCell->field_4_pSpriteLinks;
            pCell->field_4_pSpriteLinks = pNewCollide;
            return;
        }
        pPrevCell = pCell;
    }

    GridCell_C* pNewItem = gGridCell_Pool_679204->Allocate();

    if (!pPrevCell)
    {
        this->field_0_rows[y_pos] = pNewItem;
    }
    else
    {
        pPrevCell->mpNext = pNewItem;
    }

    pNewItem->mpNext = pCell;
    pNewItem->field_4_pSpriteLinks = pNewCollide;
    pNewItem->field_0_x = x_pos;
    pNewCollide->mpNext = 0;
}

MATCH_FUNC(0x4784d0)
void SpriteGrid_400::AddToRowBuckets_4784D0(s32 y_pos, Sprite* pSprite)
{
    s32 purple_left = gSpriteGrid_left_6F5FD4;
    GridCell_C* pNewNext = this->field_0_rows[y_pos];
    GridCell_C* purple_x = 0;
    while (purple_left <= gSpriteGrid_right_6F5B80)
    {
        while (pNewNext)
        {
            if (pNewNext->field_0_x > purple_left)
            {
                break;
            }
            if (pNewNext->field_0_x == purple_left)
            {
                ++purple_left;
                GridSpriteLink_8* v7 = gGridSpriteLink_Pool_679200->Allocate();
                v7->field_0_pSprite = pSprite;
                v7->mpNext = pNewNext->field_4_pSpriteLinks;
                pNewNext->field_4_pSpriteLinks = v7;
                if (purple_left > gSpriteGrid_right_6F5B80)
                {
                    return;
                }
            }
            purple_x = pNewNext;
            pNewNext = pNewNext->mpNext;
        }

        GridSpriteLink_8* v8 = gGridSpriteLink_Pool_679200->Allocate();
        v8->field_0_pSprite = pSprite;
        GridCell_C* pCIter = gGridCell_Pool_679204->Allocate();

        if (!purple_x)
        {
            this->field_0_rows[y_pos] = pCIter;
        }
        else
        {
            purple_x->mpNext = pCIter;
        }
        pCIter->mpNext = pNewNext;
        pCIter->field_4_pSpriteLinks = v8;
        pCIter->field_0_x = purple_left;
        v8->mpNext = 0;
        pNewNext = pCIter->mpNext;
        ++purple_left;
        purple_x = pCIter;
    }
}

// Get first XItem at y_pos
MATCH_FUNC(0x478590)
GridCell_C* SpriteGrid_400::GetFirstXCellInRow_478590(s32 y_pos)
{
    GridCell_C* pIter;
    s32 f0;

    if (y_pos < 0 || y_pos > 255)
    {
        return 0;
    }
    for (pIter = this->field_0_rows[y_pos]; pIter; pIter = pIter->mpNext)
    {
        f0 = (u8)pIter->field_0_x;
        if (f0 >= gSpriteGrid_left_6F5FD4)
        {
            break;
        }
        if (f0 > gSpriteGrid_right_6F5B80)
        {
            return 0;
        }
    }
    return pIter;
}

// https://decomp.me/scratch/me1ge
MATCH_FUNC(0x4785d0)
char_type SpriteGrid_400::CheckRowForRectCollisions_4785D0(u32 y_pos, Fix16_Rect* pRect)
{
    GridCell_C* pXItemIter = GetFirstXCellInRow_478590(y_pos);
    char bRet = 0;
    while (pXItemIter)
    {
        if (pXItemIter->field_0_x > gSpriteGrid_right_6F5B80)
        {
            return bRet;
        }

        for (GridSpriteLink_8* pObj = pXItemIter->field_4_pSpriteLinks; pObj; pObj = pObj->mpNext)
        {
            Sprite* field_0_pSprite = pObj->field_0_pSprite;
            if (!pObj->field_0_pSprite->TypeIs_446940(gSpriteGrid_exclude_types_678F88) &&
                field_0_pSprite != gSpriteGrid_exclusion_sprite_678F84 &&
                !pObj->field_0_pSprite->field_C_sprite_4c_ptr->CollisionIdIs_446930(gCollisionCounters_6791FC->field_4_query_id) &&
                field_0_pSprite->ShouldCollideWithSprite_59E850(0))
            {
                //  83:    inc     %edx
                gCollisionCounters_6791FC->field_0_test_count++;

                Fix16_Rect* pBBox = &pObj->field_0_pSprite->field_C_sprite_4c_ptr->field_30_boundingBox;
                if (pRect->AABB_Intersects_41E2F0(pBBox) &&
                    (!bDoCollisionCheck_679006 || pObj->field_0_pSprite->field_C_sprite_4c_ptr->IsZeroWidth_41E390() ||
                     pObj->field_0_pSprite->field_0.jIsAxisAligned_41E3C0() || pObj->field_0_pSprite->IntersectsRectSAT_59FB10(pRect) ||
                     pRect->IntersectsSpriteRenderingRect_59DDF0(pObj->field_0_pSprite)))
                {
                    if (gSpriteGrid_list_679214)
                    {
                        bRet = 1;
                        // Add to list and keep going to add more
                        gSpriteGrid_list_679214->AddSprite_5A6CD0(pObj->field_0_pSprite);
                    }
                    else
                    {
                        // No list so stop here
                        return 1;
                    }
                }

                pObj->field_0_pSprite->field_C_sprite_4c_ptr->SetCollisionId_446920(gCollisionCounters_6791FC->field_4_query_id);
            }

        } // end for
        pXItemIter = pXItemIter->mpNext;
    } // end while
    return bRet;
}

MATCH_FUNC(0x478750)
char_type SpriteGrid_400::CheckAndHandleCollisionsInStrip_478750(u32 y_pos, Sprite* pSprite)
{
    char_type bRet = 0;
    GridCell_C* pIter = GetFirstXCellInRow_478590(y_pos);
    while (pIter)
    {
        if (pIter->field_0_x > gSpriteGrid_right_6F5B80)
        {
            break;
        }

        for (GridSpriteLink_8* pC8Iter = pIter->field_4_pSpriteLinks; pC8Iter; pC8Iter = pC8Iter->mpNext)
        {
            if (!pC8Iter->field_0_pSprite->field_C_sprite_4c_ptr->CollisionIdIs_446930(gCollisionCounters_6791FC->field_4_query_id))
            {
                gCollisionCounters_6791FC->field_0_test_count++;
                if (pSprite->CollisionCheck_59E590(pC8Iter->field_0_pSprite))
                {
                    bRet = 1;
                    pC8Iter->field_0_pSprite->HandleObjectCollision_59E8C0(pSprite);
                }
                pC8Iter->field_0_pSprite->field_C_sprite_4c_ptr->SetCollisionId_446920(gCollisionCounters_6791FC->field_4_query_id);
            }
        }
        pIter = pIter->mpNext;
    }
    return bRet;
}

MATCH_FUNC(0x4787e0)
bool SpriteGrid_400::CheckAndHandleRowCollisionsForSprite_4787E0(u32 y_pos, Sprite* pSprite)
{
    bool bRet = false;
    GridCell_C* pXItemIter = GetFirstXCellInRow_478590(y_pos);
    while (pXItemIter)
    {
        if (pXItemIter->field_0_x > gSpriteGrid_right_6F5B80)
        {
            break;
        }

        for (GridSpriteLink_8* p8Iter = pXItemIter->field_4_pSpriteLinks; p8Iter; p8Iter = p8Iter->mpNext)
        {
            if (p8Iter->field_0_pSprite->TypeIs_446940(gSpriteGrid_sprite_type_filter_678FA8) &&
                !p8Iter->field_0_pSprite->field_C_sprite_4c_ptr->CollisionIdIs_446930(gCollisionCounters_6791FC->field_4_query_id))
            {
                gCollisionCounters_6791FC->field_0_test_count++;

                if (pSprite->CollisionCheck_59E590(p8Iter->field_0_pSprite))
                {
                    bRet = true;
                    p8Iter->field_0_pSprite->ProcessCarToCarImpactIfCar_59E910(pSprite);
                }

                p8Iter->field_0_pSprite->field_C_sprite_4c_ptr->SetCollisionId_446920(gCollisionCounters_6791FC->field_4_query_id);
            }
        }
        pXItemIter = pXItemIter->mpNext;
    }

    return bRet;
}

MATCH_FUNC(0x478880)
Sprite* SpriteGrid_400::FindNearestSpriteInRow_478880(u32 y_pos, Sprite* pSprite)
{
    GridCell_C* pXItem = GetFirstXCellInRow_478590(y_pos);
    while (pXItem)
    {
        if (pXItem->field_0_x > gSpriteGrid_right_6F5B80)
        {
            return 0;
        }

        for (GridSpriteLink_8* pObj = pXItem->field_4_pSpriteLinks; pObj; pObj = pObj->mpNext)
        {
            Sprite* pSprt = pObj->field_0_pSprite;
            if (pObj->field_0_pSprite == gSpriteGrid_exclusion_sprite_678F84 ||
                gSpriteGrid_smallestDistSprite_678E40 && !pSprt->TypeIs_446940(gSpriteGrid_exclude_type_678F60) ||
                pSprt->field_C_sprite_4c_ptr->CollisionIdIs_446930(gCollisionCounters_6791FC->field_4_query_id) || !pSprt->ShouldCollideWithSprite_59E850(pSprite))
            {
                continue;
            }

            gCollisionCounters_6791FC->field_0_test_count++;

            if (pSprite->CollisionCheck_59E590(pObj->field_0_pSprite))
            {
                if (!pObj->field_0_pSprite->TypeIs_446940(gSpriteGrid_exclude_type_678F60))
                {
                    gSpriteGrid_smallestDistSprite_678E40 = pObj->field_0_pSprite;
                }
                else
                {
                    return pObj->field_0_pSprite;
                }
            }
            pObj->field_0_pSprite->field_C_sprite_4c_ptr->SetCollisionId_446920(gCollisionCounters_6791FC->field_4_query_id);
        }
        pXItem = pXItem->mpNext;
    }
    return 0;
}

MATCH_FUNC(0x478950)
void SpriteGrid_400::DebugLog_478950(s32 x_pos, s32 y_pos)
{
    for (GridCell_C* i = field_0_rows[y_pos]; i; i = i->mpNext)
    {
        const s32 cell_x = i->field_0_x;
        if (cell_x > x_pos)
        {
            break;
        }

        if (cell_x == x_pos)
        {
            sprintf(gTmpBuffer_67C598, "(%d,%d):", x_pos, y_pos);
            gFile_67C530.Write_Log_4D9650(gTmpBuffer_67C598);
            for (GridSpriteLink_8* j = i->field_4_pSpriteLinks; j; j = j->mpNext)
            {
                if (j->field_0_pSprite)
                {
                    sprintf(gTmpBuffer_67C598, " %d", (u16)j->field_0_pSprite->field_20_id);
                    gFile_67C530.Write_Log_4D9650(gTmpBuffer_67C598);
                }
            }
            gFile_67C530.Write_Log_4D9650("\n");
        }
    }
}

MATCH_FUNC(0x4789f0)
GridCell_C** SpriteGrid_400::Clear_4789F0()
{
    for (u32 i = 0; i < GTA2_COUNTOF(field_0_rows); i++)
    {
        field_0_rows[i] = 0;
    }
    return field_0_rows;
}

MATCH_FUNC(0x478A10)
void SpriteGrid_400::Empty_478A10()
{
}