#include "SpriteGrid.hpp"
#include "Camera.hpp"
#include "Car.hpp"
#include "Globals.hpp"
#include "SpriteRenderer.hpp"
#include "Object_5C.hpp"
#include "collide.hpp"
#include "error.hpp"
#include "map_0x370.hpp"
#include "sprite.hpp"

// Multi/regional bucket stuff, Cars/Peds/Cranes/Objects
DEFINE_GLOBAL(SpriteGrid*, gSpriteGrid_1_679208, 0x679208);

// Used by buses, car-trailer combo, Char in certain states and objects with certain collision category
DEFINE_GLOBAL(SpriteGrid*, gSpriteGrid_2_67920C, 0x67920C);

// Single bucket stuff, mostly particles
DEFINE_GLOBAL(SpriteGrid*, gSpriteGrid_3_679210, 0x679210);

DEFINE_GLOBAL(s32, gSpriteGrid_bottom_6F5F38, 0x6F5F38);
DEFINE_GLOBAL(s32, gSpriteGrid_top_6F6108, 0x6F6108);
DEFINE_STATIC_MEMBER(s32, SpriteGrid, sSpriteGridSpriteTypeFilter, 0x678FA8);
DEFINE_STATIC_MEMBER(s32, SpriteGrid, sSpriteGridPreferredType, 0x678F60);
DEFINE_STATIC_MEMBER(Sprite*, SpriteGrid, sSpriteGridSmallestDistSprite, 0x678E40);
DEFINE_STATIC_MEMBER(u8, SpriteGrid, sbDoCollisionCheck, 0x679006);
DEFINE_STATIC_MEMBER(s32, SpriteGrid, sSpriteGridExcludeTypes, 0x678F88);
DEFINE_GLOBAL(s32, gSpriteGrid_start_x_679090, 0x679090);
DEFINE_GLOBAL(s32, gSpriteGrid_start_y_679098, 0x679098);
DEFINE_STATIC_MEMBER(SpriteList*, SpriteGrid, sSpriteGridList, 0x679214);
EXTERN_GLOBAL(CollisionCounters*, gCollisionCounters_6791FC);
EXTERN_GLOBAL(GridCell_Pool*, gGridCell_Pool_679204);
EXTERN_GLOBAL(GridSpriteLink_Pool*, gGridSpriteLink_Pool_679200);
DEFINE_GLOBAL(Sprite*, gSpriteGrid_exclusion_sprite_678F84, 0x678F84);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_678F74, Fix16(0x2000, 0), 0x678F74);
DEFINE_STATIC_MEMBER(s32, SpriteGrid, sSpriteGridSpriteType1, 0x678FE8);
DEFINE_STATIC_MEMBER(s32, SpriteGrid, sSpriteGridSpriteType2, 0x678FEC);
DEFINE_STATIC_MEMBER(Sprite*, SpriteGrid, sSpriteGridExcludeSprite, 0x678F40);
DEFINE_GLOBAL(Ped*, gSpriteGrid_ped_678F64, 0x678F64);
DEFINE_GLOBAL(Fix16, gSpriteGrid_smallestDistance_678E5C, 0x678E5C);
DEFINE_GLOBAL(Fix16, gSpriteGrid_zpos_low_678F38, 0x678F38);
DEFINE_GLOBAL(Fix16, gSpriteGrid_zpos_high_678F3C, 0x678F3C);
DEFINE_STATIC_MEMBER(s32, SpriteGrid, sSpriteGridSearchMode, 0x678FD0);

DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpOneAndHalf, Fix16(0x6000, 0), 0x678F80);
DEFINE_GLOBAL_INIT(Fix16, kFpOne_679084, Fix16(1), 0x679084);

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
void SpriteGrid::DrawSpritesClipped_477A40()
{
    const s32 left = Clamp((gViewCamera_676978->mViewBounds.mLeft - kFpOne_679084).ToInt(), 0, 255);
    const s32 right = Clamp((kFpOneAndHalf + gViewCamera_676978->mViewBounds.mRight).ToInt(), 0, 255);
    const s32 top = Clamp((gViewCamera_676978->mViewBounds.mTop - kFpOne_679084).ToInt(), 0, 255);
    const s32 bottom = Clamp((kFpOneAndHalf + gViewCamera_676978->mViewBounds.mBottom).ToInt(), 0, 255);

    AddToDrawList_478240(left, right, top, bottom);
}

MATCH_FUNC(0x477ae0)
void SpriteGrid::AddToSingleBucket_477AE0(Sprite* pSprite)
{
    AddToSingleBucket_478440(pSprite->mXy.x.ToInt(), pSprite->mXy.y.ToInt(), pSprite);
}

MATCH_FUNC(0x477b00)
void SpriteGrid::Remove_477B00(Sprite* pSprite)
{
    DoRemove_4782C0(pSprite->mXy.x.ToInt(), pSprite->mXy.y.ToInt(), pSprite);
}

MATCH_FUNC(0x477b20)
void SpriteGrid::AddToRegionBuckets_477B20(Sprite* pSprite)
{
    pSprite->UpdateCollisionBoundsIfNeeded_59E9C0();
    pSprite->mpCollisionShape->SetCurrentRect_5A4D90();
    for (s32 y_pos = gSpriteGrid_top_6F6108; y_pos <= gSpriteGrid_bottom_6F5F38; ++y_pos)
    {
        AddToRowBuckets_4784D0(y_pos, pSprite);
    }
}

MATCH_FUNC(0x477b60)
void SpriteGrid::RemoveFromRegionBuckets_477B60(Sprite* pSprite)
{
    pSprite->mpCollisionShape->SetCurrentRect_5A4D90();
    for (s32 y_pos = gSpriteGrid_top_6F6108; y_pos <= gSpriteGrid_bottom_6F5F38; ++y_pos)
    {
        RemoveFromRowBuckets_478370(y_pos, pSprite);
    }
}

MATCH_FUNC(0x477ba0)
void SpriteGrid::DebugLogAll_477BA0()
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
bool SpriteGrid::CheckAndHandleCollisionInStrips_477BD0(Sprite* pSprite)
{
    bool bHit = false;

    gCollisionCounters_6791FC->mQueryId++;

    pSprite->UpdateCollisionBoundsIfNeeded_59E9C0();
    pSprite->mpCollisionShape->SetCurrentRect_5A4D90();

    for (s32 i = gSpriteGrid_top_6F6108; i <= gSpriteGrid_bottom_6F5F38; ++i)
    {
        bHit |= CheckAndHandleCollisionsInStrip_478750(i, pSprite);
    }
    return bHit;
}

MATCH_FUNC(0x477c30)
bool SpriteGrid::CheckAndHandleAllCollisionsForSprite_477C30(Sprite* pSprite, s32 sprite_type_filter)
{
    sSpriteGridSpriteTypeFilter = sprite_type_filter;
    bool bHit = false;
    ++gCollisionCounters_6791FC->mQueryId;
    pSprite->UpdateCollisionBoundsIfNeeded_59E9C0();
    pSprite->mpCollisionShape->SetCurrentRect_5A4D90();
    for (s32 i = gSpriteGrid_top_6F6108; i <= gSpriteGrid_bottom_6F5F38; ++i)
    {
        bHit |= SpriteGrid::CheckAndHandleRowCollisionsForSprite_4787E0(i, pSprite);
    }
    return bHit;
}

MATCH_FUNC(0x477c90)
Sprite* SpriteGrid::FindNearestSprite_SpiralSearch_477C90(s32 primary_sprite_type,
                                                          s32 secondary_sprite_type,
                                                          Sprite* pExclude,
                                                          u8 max_rings,
                                                          s32 search_mode,
                                                          char_type bUseSpriteZ)
{
    gSpriteGrid_start_x_679090 = pExclude->mXy.x.ToInt();
    gSpriteGrid_start_y_679098 = pExclude->mXy.y.ToInt();
    sSpriteGridSpriteType1 = primary_sprite_type;
    sSpriteGridSpriteType2 = secondary_sprite_type;
    sSpriteGridExcludeSprite = pExclude;
    gSpriteGrid_ped_678F64 = pExclude->GetPed_59E1B0();
    gSpriteGrid_smallestDistance_678E5C = Fix16(256);
    sSpriteGridSmallestDistSprite = NULL;
    sSpriteGridSearchMode = search_mode;

    if (bUseSpriteZ == 1)
    {
        gSpriteGrid_zpos_low_678F38 = pExclude->mZpos - kFpHalf_678F74;
        gSpriteGrid_zpos_high_678F3C = pExclude->mZpos + kFpHalf_678F74;
    }
    else
    {
        gSpriteGrid_zpos_low_678F38 = 0;
        gSpriteGrid_zpos_high_678F3C = Fix16(7);
    }

    SearchTileStripForClosestSprite_4781E0(1u);

    Sprite* pCollisionSprite = sSpriteGridSmallestDistSprite; // from previous call
    if (search_mode > 1 || !sSpriteGridSmallestDistSprite)
    {
        for (u32 ring_size = 2; ring_size <= static_cast<u32>(2 * max_rings); ring_size += 2)
        {
            // LEFT COLUMN
            --gSpriteGrid_start_x_679090;
            gSpriteGrid_start_y_679098 += 2 - ring_size;
            SearchTileColumnForClosestSprite_478160(ring_size - 1);

            if (!search_mode)
            {
                pCollisionSprite = sSpriteGridSmallestDistSprite;
                if (sSpriteGridSmallestDistSprite)
                {
                    break;
                }
            }

            // TOP ROW
            --gSpriteGrid_start_y_679098;
            SearchTileStripForClosestSprite_4781E0(ring_size + 1);

            if (!search_mode)
            {
                pCollisionSprite = sSpriteGridSmallestDistSprite;
                if (sSpriteGridSmallestDistSprite)
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
                pCollisionSprite = sSpriteGridSmallestDistSprite;
                if (sSpriteGridSmallestDistSprite)
                {
                    break;
                }
            }

            // BOTTOM ROW
            gSpriteGrid_start_x_679090 -= ring_size;
            gSpriteGrid_start_y_679098 = gSpriteGrid_start_y_679098 + ring_size - 1;
            SearchTileStripForClosestSprite_4781E0(ring_size + 1);

            pCollisionSprite = sSpriteGridSmallestDistSprite;

            if (search_mode <= 1)
            {
                if (sSpriteGridSmallestDistSprite)
                {
                    break;
                }
            }
        }
    }

    return pCollisionSprite;
}

MATCH_FUNC(0x477E50)
void SpriteGrid::SetSpriteToExclude_477E50(Sprite* pSprite)
{
    gSpriteGrid_exclusion_sprite_678F84 = pSprite;
}

MATCH_FUNC(0x477e60)
Sprite* SpriteGrid::FindCollidingSpritePreferType_477E60(Sprite* pSprite, s32 desired_sprite_type)
{
    sSpriteGridPreferredType = desired_sprite_type;
    sSpriteGridSmallestDistSprite = NULL;

    gCollisionCounters_6791FC->mQueryId++;

    pSprite->UpdateCollisionBoundsIfNeeded_59E9C0();
    pSprite->mpCollisionShape->SetCurrentRect_5A4D90();

    for (s32 top = gSpriteGrid_top_6F6108; top <= gSpriteGrid_bottom_6F5F38; top++)
    {
        Sprite* pObj = FindCollidingSpriteInRow_478880(top, pSprite);
        if (pObj)
        {
            return pObj;
        }
    }

    return sSpriteGridSmallestDistSprite;
}

MATCH_FUNC(0x477f30)
bool SpriteGrid::CollectRectCollisions_477F30(Fix16_Rect* pRect, bool bDeepCheck, s32 exclude_sprite_type, Sprite* pExclude, SpriteList* pOutList)
{
    sSpriteGridList = pOutList;
    bool bRet = SpriteGrid::CheckRectForCollisions_477F60(pRect, bDeepCheck, exclude_sprite_type, pExclude);
    sSpriteGridList = NULL;
    return bRet;
}

MATCH_FUNC(0x477f60)
bool SpriteGrid::CheckRectForCollisions_477F60(Fix16_Rect* pRect, bool bDeepCheck, s32 exclude_sprite_type, Sprite* pExclude)
{
    bool bRet = false;
    ++gCollisionCounters_6791FC->mQueryId;
    pRect->DoSetCurrentRect_59DD60();
    sbDoCollisionCheck = bDeepCheck;
    sSpriteGridExcludeTypes = exclude_sprite_type;
    SetSpriteToExclude_477E50(pExclude);

    for (s32 y_pos = gSpriteGrid_top_6F6108; y_pos <= gSpriteGrid_bottom_6F5F38; y_pos++)
    {
        if (SpriteGrid::CheckRowForRectCollisions_4785D0(y_pos, pRect))
        {
            if (sSpriteGridList)
            {
                bRet = true;
            }
            else
            {
                SetSpriteToExclude_477E50(NULL);
                return true;
            }
        }
    }
    SetSpriteToExclude_477E50(NULL);
    return bRet;
}

MATCH_FUNC(0x478040)
SpriteGrid::SpriteGrid()
{
    Clear_4789F0();
}

MATCH_FUNC(0x478050)
SpriteGrid::~SpriteGrid()
{
    Empty_478A10();
}

MATCH_FUNC(0x478060)
void SpriteGrid::CheckTileSpritesForClosestMatch_478060(GridSpriteLink* pSpriteLinks)
{
    Fix16 dist;
    for (GridSpriteLink* pColIter = pSpriteLinks; pColIter; pColIter = pColIter->mpNext)
    {
        Sprite* pLinkSprite = pColIter->mpSprite;
        if ((pColIter->mpSprite->TypeIs_446940(sSpriteGridSpriteType1) ||
             pColIter->mpSprite->TypeIs_446940(sSpriteGridSpriteType2)) &&
            pLinkSprite != sSpriteGridExcludeSprite)
        {

            if (pLinkSprite->mZpos <= gSpriteGrid_zpos_high_678F3C && pLinkSprite->mZpos >= gSpriteGrid_zpos_low_678F38)
            {
                if (sSpriteGridSearchMode == 3)
                {
                    dist = sSpriteGridExcludeSprite->MinDistanceToAnySpriteBBoxCorner_5A22B0(pLinkSprite);
                    if (dist < gSpriteGrid_smallestDistance_678E5C)
                    {
                        if (pColIter->mpSprite->GetType_416B40() == sprite_types_enum::car_2)
                        {
                            if (pColIter->mpSprite->mCarBcPtr->IsEnterable_445360())
                            {
                                gSpriteGrid_smallestDistance_678E5C = dist;
                                sSpriteGridSmallestDistSprite = pColIter->mpSprite;
                            }
                        }
                        else
                        {
                            gSpriteGrid_smallestDistance_678E5C = dist;
                            sSpriteGridSmallestDistSprite = pColIter->mpSprite;
                        }
                    }
                }
                else
                {

                    dist = sSpriteGridExcludeSprite->ManhattanDistance_446960(pLinkSprite);

                    if (dist < gSpriteGrid_smallestDistance_678E5C)
                    {
                        if (pLinkSprite->IsThreatToSearchingPed_59E830())
                        {
                            gSpriteGrid_smallestDistance_678E5C = dist;
                            sSpriteGridSmallestDistSprite = pColIter->mpSprite;
                        }
                    }
                }
            }
        }
    }
}

MATCH_FUNC(0x478160)
void SpriteGrid::SearchTileColumnForClosestSprite_478160(u8 height)
{
    u32 y_pos = gSpriteGrid_start_y_679098;
    if (static_cast<u32>(gSpriteGrid_start_y_679098) < gSpriteGrid_start_y_679098 + static_cast<u32>(height))
    {
        // Won't match without this redundant iter
        GridCell** pXItemIter = &this->mRows[gSpriteGrid_start_y_679098];
        while (y_pos < gSpriteGrid_start_y_679098 + static_cast<u32>(height))
        {
            CheckTileSpritesForClosestMatch_478060(GetCollideListAt_446820(gSpriteGrid_start_x_679090, y_pos));
            ++y_pos;
            ++pXItemIter;
        }
    }
}

MATCH_FUNC(0x4781E0)
void SpriteGrid::SearchTileStripForClosestSprite_4781E0(u8 width)
{
    gSpriteGrid_left_6F5FD4 = gSpriteGrid_start_x_679090;
    gSpriteGrid_right_6F5B80 = gSpriteGrid_start_x_679090 + width - 1;

    for (GridCell* pXItemIter = GetFirstXCellInRow_478590(gSpriteGrid_start_y_679098); pXItemIter; pXItemIter = pXItemIter->mpNext)
    {
        if (pXItemIter->mX > gSpriteGrid_right_6F5B80)
        {
            break;
        }
        CheckTileSpritesForClosestMatch_478060(pXItemIter->mpSpriteLinks);
    }
}

MATCH_FUNC(0x478240)
void SpriteGrid::AddToDrawList_478240(s32 left, s32 right, s32 top, s32 bottom)
{
    GridCell** pYItem = &this->mRows[top];
    if (top <= bottom)
    {
        s32 y_total = bottom - top + 1;
        do
        {
            for (GridCell* pXItem = *pYItem; pXItem; pXItem = pXItem->mpNext)
            {
                const s32 x_cell = pXItem->mX;
                if (x_cell > right)
                {
                    break;
                }
                if (x_cell >= left)
                {
                    for (GridSpriteLink* pLinkIter = pXItem->mpSpriteLinks; pLinkIter; pLinkIter = pLinkIter->mpNext)
                    {
                        if (pLinkIter->mpSprite->IsTypeAbove1_446950())
                        {
                            gSpriteRenderer_67B580->DisplayAdd_495510(pLinkIter->mpSprite);
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
void SpriteGrid::DoRemove_4782C0(s32 x_pos, s32 y_pos, Sprite* pSprite)
{
    GridCell* pPrevCell = NULL;
    GridSpriteLink* pPrevLink = NULL;

    for (GridCell* pCell = this->mRows[y_pos]; pCell; pCell = pCell->mpNext)
    {
        if (pCell->mX == x_pos)
        {
            GridSpriteLink* pLink = pCell->mpSpriteLinks;
            while (pLink)
            {
                if (pLink->mpSprite == pSprite)
                {
                    if (!pPrevLink)
                    {
                        pCell->mpSpriteLinks = pLink->mpNext;
                    }
                    else
                    {
                        pPrevLink->mpNext = pLink->mpNext;
                    }

                    gGridSpriteLink_Pool_679200->DeAllocate(pLink);

                    if (!pCell->mpSpriteLinks)
                    {
                        if (!pPrevCell)
                        {
                            this->mRows[y_pos] = pCell->mpNext;
                        }
                        else
                        {
                            pPrevCell->mpNext = pCell->mpNext;
                        }
                        gGridCell_Pool_679204->DeAllocate(pCell);
                    }
                    return;
                }

                pPrevLink = pLink;
                pLink = pLink->mpNext;
            }
        }
        pPrevCell = pCell;
    }
}

MATCH_FUNC(0x478370)
void SpriteGrid::RemoveFromRowBuckets_478370(s32 y_pos, Sprite* pSprite)
{
    s32 x_pos = gSpriteGrid_left_6F5FD4;
    GridCell* pPrevCell = NULL;
    for (GridCell* pCell = this->mRows[y_pos]; pCell; pCell = pCell->mpNext)
    {
        if (pCell->mX == x_pos)
        {
            GridSpriteLink* pLink = pCell->mpSpriteLinks;
            GridSpriteLink* pPrevLink = NULL;
            while (pLink)
            {
                if (pLink->mpSprite == pSprite)
                {
                    if (!pPrevLink)
                    {
                        pCell->mpSpriteLinks = pLink->mpNext;
                    }
                    else
                    {
                        pPrevLink->mpNext = pLink->mpNext;
                    }

                    gGridSpriteLink_Pool_679200->DeAllocate(pLink);

                    if (!pCell->mpSpriteLinks)
                    {
                        GridCell* pNext = pCell->mpNext;
                        if (!pPrevCell)
                        {
                            this->mRows[y_pos] = pNext;
                        }
                        else
                        {
                            pPrevCell->mpNext = pNext;
                        }
                        pCell = gGridCell_Pool_679204->UnlinkAndReturnNext(pCell);
                    }
                    else
                    {
                        pPrevCell = pCell;
                        pCell = pCell->mpNext;
                    }
                    ++x_pos;

                    if (x_pos > gSpriteGrid_right_6F5B80)
                    {
                        return;
                    }
                    pLink = pCell->mpSpriteLinks;
                    pPrevLink = NULL;
                }
                else
                {
                    pPrevLink = pLink;
                    pLink = pLink->mpNext;
                }
            }
        }
        pPrevCell = pCell;
    }
}

MATCH_FUNC(0x478440)
void SpriteGrid::AddToSingleBucket_478440(s32 x_pos, s32 y_pos, Sprite* pSprite)
{
    GridSpriteLink* pNewCollide = gGridSpriteLink_Pool_679200->Allocate();

    pNewCollide->mpSprite = pSprite;

    GridCell* pPrevCell = NULL;
    GridCell* pCell;
    for (pCell = this->mRows[y_pos]; pCell; pCell = pCell->mpNext)
    {
        const s32 cell_x = pCell->mX;
        if (cell_x > x_pos)
        {
            break;
        }
        if (cell_x == x_pos)
        {
            pNewCollide->mpNext = pCell->mpSpriteLinks;
            pCell->mpSpriteLinks = pNewCollide;
            return;
        }
        pPrevCell = pCell;
    }

    GridCell* pNewItem = gGridCell_Pool_679204->Allocate();

    if (!pPrevCell)
    {
        this->mRows[y_pos] = pNewItem;
    }
    else
    {
        pPrevCell->mpNext = pNewItem;
    }

    pNewItem->mpNext = pCell;
    pNewItem->mpSpriteLinks = pNewCollide;
    pNewItem->mX = x_pos;
    pNewCollide->mpNext = NULL;
}

MATCH_FUNC(0x4784d0)
void SpriteGrid::AddToRowBuckets_4784D0(s32 y_pos, Sprite* pSprite)
{
    s32 x_pos = gSpriteGrid_left_6F5FD4;
    GridCell* pNextCell = this->mRows[y_pos];
    GridCell* pPrevCell = NULL;
    while (x_pos <= gSpriteGrid_right_6F5B80)
    {
        while (pNextCell)
        {
            if (pNextCell->mX > x_pos)
            {
                break;
            }
            if (pNextCell->mX == x_pos)
            {
                ++x_pos;
                GridSpriteLink* pLink = gGridSpriteLink_Pool_679200->Allocate();
                pLink->mpSprite = pSprite;
                pLink->mpNext = pNextCell->mpSpriteLinks;
                pNextCell->mpSpriteLinks = pLink;
                if (x_pos > gSpriteGrid_right_6F5B80)
                {
                    return;
                }
            }
            pPrevCell = pNextCell;
            pNextCell = pNextCell->mpNext;
        }

        GridSpriteLink* pLink = gGridSpriteLink_Pool_679200->Allocate();
        pLink->mpSprite = pSprite;
        GridCell* pCell = gGridCell_Pool_679204->Allocate();

        if (!pPrevCell)
        {
            this->mRows[y_pos] = pCell;
        }
        else
        {
            pPrevCell->mpNext = pCell;
        }
        pCell->mpNext = pNextCell;
        pCell->mpSpriteLinks = pLink;
        pCell->mX = x_pos;
        pLink->mpNext = NULL;
        pNextCell = pCell->mpNext;
        ++x_pos;
        pPrevCell = pCell;
    }
}

MATCH_FUNC(0x478590)
GridCell* SpriteGrid::GetFirstXCellInRow_478590(s32 y_pos) const
{
    GridCell* pIter;
    s32 cell_x;

    if (y_pos < 0 || y_pos > 255)
    {
        return NULL;
    }
    for (pIter = this->mRows[y_pos]; pIter; pIter = pIter->mpNext)
    {
        cell_x = static_cast<u8>(pIter->mX);
        if (cell_x >= gSpriteGrid_left_6F5FD4)
        {
            break;
        }
        if (cell_x > gSpriteGrid_right_6F5B80)
        {
            return NULL;
        }
    }
    return pIter;
}

// https://decomp.me/scratch/me1ge
MATCH_FUNC(0x4785d0)
bool SpriteGrid::CheckRowForRectCollisions_4785D0(u32 y_pos, Fix16_Rect* pRect)
{
    GridCell* pXItemIter = GetFirstXCellInRow_478590(y_pos);
    bool bRet = false;
    while (pXItemIter)
    {
        if (pXItemIter->mX > gSpriteGrid_right_6F5B80)
        {
            return bRet;
        }

        for (GridSpriteLink* pObj = pXItemIter->mpSpriteLinks; pObj; pObj = pObj->mpNext)
        {
            Sprite* pLinkSprite = pObj->mpSprite;
            if (!pObj->mpSprite->TypeIs_446940(sSpriteGridExcludeTypes) &&
                pLinkSprite != gSpriteGrid_exclusion_sprite_678F84 &&
                !pObj->mpSprite->mpCollisionShape->CollisionIdIs_446930(gCollisionCounters_6791FC->mQueryId) &&
                pLinkSprite->ShouldCollideWithSprite_59E850(NULL))
            {
                gCollisionCounters_6791FC->mTestCount++;

                Fix16_Rect* pBBox = &pObj->mpSprite->mpCollisionShape->mBoundingBox;
                if (pRect->AABB_Intersects_41E2F0(pBBox) &&
                    (!sbDoCollisionCheck || pObj->mpSprite->mpCollisionShape->IsZeroWidth_41E390() ||
                     pObj->mpSprite->mRotation.jIsAxisAligned_41E3C0() || pObj->mpSprite->IntersectsRectSAT_59FB10(pRect) ||
                     pRect->IntersectsSpriteRenderingRect_59DDF0(pObj->mpSprite)))
                {
                    if (sSpriteGridList)
                    {
                        bRet = true;
                        // Add to list and keep going to add more
                        sSpriteGridList->AddSprite_5A6CD0(pObj->mpSprite);
                    }
                    else
                    {
                        // No list so stop here
                        return true;
                    }
                }

                pObj->mpSprite->mpCollisionShape->SetCollisionId_446920(gCollisionCounters_6791FC->mQueryId);
            }

        }
        pXItemIter = pXItemIter->mpNext;
    }
    return bRet;
}

MATCH_FUNC(0x478750)
bool SpriteGrid::CheckAndHandleCollisionsInStrip_478750(u32 y_pos, Sprite* pSprite)
{
    bool bRet = false;
    GridCell* pIter = GetFirstXCellInRow_478590(y_pos);
    while (pIter)
    {
        if (pIter->mX > gSpriteGrid_right_6F5B80)
        {
            break;
        }

        for (GridSpriteLink* pLinkIter = pIter->mpSpriteLinks; pLinkIter; pLinkIter = pLinkIter->mpNext)
        {
            if (!pLinkIter->mpSprite->mpCollisionShape->CollisionIdIs_446930(gCollisionCounters_6791FC->mQueryId))
            {
                gCollisionCounters_6791FC->mTestCount++;
                if (pSprite->CollisionCheck_59E590(pLinkIter->mpSprite))
                {
                    bRet = true;
                    pLinkIter->mpSprite->HandleObjectCollision_59E8C0(pSprite);
                }
                pLinkIter->mpSprite->mpCollisionShape->SetCollisionId_446920(gCollisionCounters_6791FC->mQueryId);
            }
        }
        pIter = pIter->mpNext;
    }
    return bRet;
}

MATCH_FUNC(0x4787e0)
bool SpriteGrid::CheckAndHandleRowCollisionsForSprite_4787E0(u32 y_pos, Sprite* pSprite)
{
    bool bRet = false;
    GridCell* pXItemIter = GetFirstXCellInRow_478590(y_pos);
    while (pXItemIter)
    {
        if (pXItemIter->mX > gSpriteGrid_right_6F5B80)
        {
            break;
        }

        for (GridSpriteLink* pLinkIter = pXItemIter->mpSpriteLinks; pLinkIter; pLinkIter = pLinkIter->mpNext)
        {
            if (pLinkIter->mpSprite->TypeIs_446940(sSpriteGridSpriteTypeFilter) &&
                !pLinkIter->mpSprite->mpCollisionShape->CollisionIdIs_446930(gCollisionCounters_6791FC->mQueryId))
            {
                gCollisionCounters_6791FC->mTestCount++;

                if (pSprite->CollisionCheck_59E590(pLinkIter->mpSprite))
                {
                    bRet = true;
                    pLinkIter->mpSprite->ProcessCarToCarImpactIfCar_59E910(pSprite);
                }

                pLinkIter->mpSprite->mpCollisionShape->SetCollisionId_446920(gCollisionCounters_6791FC->mQueryId);
            }
        }
        pXItemIter = pXItemIter->mpNext;
    }

    return bRet;
}

MATCH_FUNC(0x478880)
Sprite* SpriteGrid::FindCollidingSpriteInRow_478880(u32 y_pos, Sprite* pSprite)
{
    GridCell* pXItem = GetFirstXCellInRow_478590(y_pos);
    while (pXItem)
    {
        if (pXItem->mX > gSpriteGrid_right_6F5B80)
        {
            return NULL;
        }

        for (GridSpriteLink* pObj = pXItem->mpSpriteLinks; pObj; pObj = pObj->mpNext)
        {
            Sprite* pLinkSprite = pObj->mpSprite;
            if (pObj->mpSprite == gSpriteGrid_exclusion_sprite_678F84 ||
                sSpriteGridSmallestDistSprite && !pLinkSprite->TypeIs_446940(sSpriteGridPreferredType) ||
                pLinkSprite->mpCollisionShape->CollisionIdIs_446930(gCollisionCounters_6791FC->mQueryId) || !pLinkSprite->ShouldCollideWithSprite_59E850(pSprite))
            {
                continue;
            }

            gCollisionCounters_6791FC->mTestCount++;

            if (pSprite->CollisionCheck_59E590(pObj->mpSprite))
            {
                if (!pObj->mpSprite->TypeIs_446940(sSpriteGridPreferredType))
                {
                    sSpriteGridSmallestDistSprite = pObj->mpSprite;
                }
                else
                {
                    return pObj->mpSprite;
                }
            }
            pObj->mpSprite->mpCollisionShape->SetCollisionId_446920(gCollisionCounters_6791FC->mQueryId);
        }
        pXItem = pXItem->mpNext;
    }
    return NULL;
}

MATCH_FUNC(0x478950)
void SpriteGrid::DebugLog_478950(s32 x_pos, s32 y_pos) const
{
    for (GridCell* i = mRows[y_pos]; i; i = i->mpNext)
    {
        const s32 cell_x = i->mX;
        if (cell_x > x_pos)
        {
            break;
        }

        if (cell_x == x_pos)
        {
            sprintf(gTmpBuffer_67C598, "(%d,%d):", x_pos, y_pos);
            gErrorLog_67C530.Write_Log_4D9650(gTmpBuffer_67C598);
            for (GridSpriteLink* j = i->mpSpriteLinks; j; j = j->mpNext)
            {
                if (j->mpSprite)
                {
                    sprintf(gTmpBuffer_67C598, " %d", static_cast<u16>(j->mpSprite->mInstanceId));
                    gErrorLog_67C530.Write_Log_4D9650(gTmpBuffer_67C598);
                }
            }
            gErrorLog_67C530.Write_Log_4D9650("\n");
        }
    }
}

MATCH_FUNC(0x4789f0)
GridCell** SpriteGrid::Clear_4789F0()
{
    for (u32 i = 0; i < GTA2_COUNTOF(mRows); i++)
    {
        mRows[i] = NULL;
    }
    return mRows;
}

MATCH_FUNC(0x478A10)
void SpriteGrid::Empty_478A10()
{
}