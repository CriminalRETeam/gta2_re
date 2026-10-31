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
DEFINE_GLOBAL(SpriteGrid*, gSpriteGrid_1, 0x679208);

// Used by buses, car-trailer combo, Char in certain states and objects with certain collision category
DEFINE_GLOBAL(SpriteGrid*, gSpriteGrid_2, 0x67920C);

// Single bucket stuff, mostly particles
DEFINE_GLOBAL(SpriteGrid*, gSpriteGrid_3, 0x679210);

DEFINE_GLOBAL(s32, gSpriteGrid_bottom, 0x6F5F38);
DEFINE_GLOBAL(s32, gSpriteGrid_top, 0x6F6108);
DEFINE_STATIC_MEMBER(s32, SpriteGrid, sSpriteGridSpriteTypeFilter, 0x678FA8);
DEFINE_STATIC_MEMBER(s32, SpriteGrid, sSpriteGridPreferredType, 0x678F60);
DEFINE_STATIC_MEMBER(Sprite*, SpriteGrid, sSpriteGridSmallestDistSprite, 0x678E40);
DEFINE_STATIC_MEMBER(u8, SpriteGrid, sbDoCollisionCheck, 0x679006);
DEFINE_STATIC_MEMBER(s32, SpriteGrid, sSpriteGridExcludeTypes, 0x678F88);
DEFINE_GLOBAL(s32, gSpriteGrid_start_x, 0x679090);
DEFINE_GLOBAL(s32, gSpriteGrid_start_y, 0x679098);
DEFINE_STATIC_MEMBER(SpriteList*, SpriteGrid, sSpriteGridList, 0x679214);
EXTERN_GLOBAL(CollisionCounters*, gCollisionCounters);
EXTERN_GLOBAL(GridCell_Pool*, gGridCell_Pool);
EXTERN_GLOBAL(GridSpriteLink_Pool*, gGridSpriteLink_Pool);
DEFINE_GLOBAL(Sprite*, gSpriteGrid_exclusion_sprite, 0x678F84);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_678F74, Fix16(0x2000, 0), 0x678F74);
DEFINE_STATIC_MEMBER(s32, SpriteGrid, sSpriteGridSpriteType1, 0x678FE8);
DEFINE_STATIC_MEMBER(s32, SpriteGrid, sSpriteGridSpriteType2, 0x678FEC);
DEFINE_STATIC_MEMBER(Sprite*, SpriteGrid, sSpriteGridExcludeSprite, 0x678F40);
DEFINE_GLOBAL(Ped*, gSpriteGrid_ped, 0x678F64);
DEFINE_GLOBAL(Fix16, gSpriteGrid_smallestDistance, 0x678E5C);
DEFINE_GLOBAL(Fix16, gSpriteGrid_zpos_low, 0x678F38);
DEFINE_GLOBAL(Fix16, gSpriteGrid_zpos_high, 0x678F3C);
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
void SpriteGrid::DrawSpritesClipped()
{
    const s32 left = Clamp((gViewCamera->mViewBounds.mLeft - kFpOne_679084).ToInt(), 0, 255);
    const s32 right = Clamp((kFpOneAndHalf + gViewCamera->mViewBounds.mRight).ToInt(), 0, 255);
    const s32 top = Clamp((gViewCamera->mViewBounds.mTop - kFpOne_679084).ToInt(), 0, 255);
    const s32 bottom = Clamp((kFpOneAndHalf + gViewCamera->mViewBounds.mBottom).ToInt(), 0, 255);

    AddToDrawList(left, right, top, bottom);
}

MATCH_FUNC(0x477ae0)
void SpriteGrid::AddToSingleBucket(Sprite* pSprite)
{
    AddToSingleBucketAt(pSprite->mXy.x.ToInt(), pSprite->mXy.y.ToInt(), pSprite);
}

MATCH_FUNC(0x477b00)
void SpriteGrid::Remove(Sprite* pSprite)
{
    DoRemove(pSprite->mXy.x.ToInt(), pSprite->mXy.y.ToInt(), pSprite);
}

MATCH_FUNC(0x477b20)
void SpriteGrid::AddToRegionBuckets(Sprite* pSprite)
{
    pSprite->UpdateCollisionBoundsIfNeeded();
    pSprite->mpCollisionShape->SetCurrentRect();
    for (s32 y_pos = gSpriteGrid_top; y_pos <= gSpriteGrid_bottom; ++y_pos)
    {
        AddToRowBuckets(y_pos, pSprite);
    }
}

MATCH_FUNC(0x477b60)
void SpriteGrid::RemoveFromRegionBuckets(Sprite* pSprite)
{
    pSprite->mpCollisionShape->SetCurrentRect();
    for (s32 y_pos = gSpriteGrid_top; y_pos <= gSpriteGrid_bottom; ++y_pos)
    {
        RemoveFromRowBuckets(y_pos, pSprite);
    }
}

MATCH_FUNC(0x477ba0)
void SpriteGrid::DebugLogAll()
{
    for (s32 i = 0; i < 256; ++i)
    {
        for (s32 j = 0; j < 256; ++j)
        {
            DebugLog(j, i);
        }
    }
}

MATCH_FUNC(0x477bd0)
bool SpriteGrid::CheckAndHandleCollisionInStrips(Sprite* pSprite)
{
    bool bHit = false;

    gCollisionCounters->mQueryId++;

    pSprite->UpdateCollisionBoundsIfNeeded();
    pSprite->mpCollisionShape->SetCurrentRect();

    for (s32 i = gSpriteGrid_top; i <= gSpriteGrid_bottom; ++i)
    {
        bHit |= CheckAndHandleCollisionsInStrip(i, pSprite);
    }
    return bHit;
}

MATCH_FUNC(0x477c30)
bool SpriteGrid::CheckAndHandleAllCollisionsForSprite(Sprite* pSprite, s32 sprite_type_filter)
{
    sSpriteGridSpriteTypeFilter = sprite_type_filter;
    bool bHit = false;
    ++gCollisionCounters->mQueryId;
    pSprite->UpdateCollisionBoundsIfNeeded();
    pSprite->mpCollisionShape->SetCurrentRect();
    for (s32 i = gSpriteGrid_top; i <= gSpriteGrid_bottom; ++i)
    {
        bHit |= SpriteGrid::CheckAndHandleRowCollisionsForSprite(i, pSprite);
    }
    return bHit;
}

MATCH_FUNC(0x477c90)
Sprite* SpriteGrid::FindNearestSprite_SpiralSearch(s32 primary_sprite_type,
                                                          s32 secondary_sprite_type,
                                                          Sprite* pExclude,
                                                          u8 max_rings,
                                                          s32 search_mode,
                                                          char_type bUseSpriteZ)
{
    gSpriteGrid_start_x = pExclude->mXy.x.ToInt();
    gSpriteGrid_start_y = pExclude->mXy.y.ToInt();
    sSpriteGridSpriteType1 = primary_sprite_type;
    sSpriteGridSpriteType2 = secondary_sprite_type;
    sSpriteGridExcludeSprite = pExclude;
    gSpriteGrid_ped = pExclude->GetPed();
    gSpriteGrid_smallestDistance = Fix16(256);
    sSpriteGridSmallestDistSprite = NULL;
    sSpriteGridSearchMode = search_mode;

    if (bUseSpriteZ == 1)
    {
        gSpriteGrid_zpos_low = pExclude->mZpos - kFpHalf_678F74;
        gSpriteGrid_zpos_high = pExclude->mZpos + kFpHalf_678F74;
    }
    else
    {
        gSpriteGrid_zpos_low = 0;
        gSpriteGrid_zpos_high = Fix16(7);
    }

    SearchTileStripForClosestSprite(1u);

    Sprite* pCollisionSprite = sSpriteGridSmallestDistSprite; // from previous call
    if (search_mode > 1 || !sSpriteGridSmallestDistSprite)
    {
        for (u32 ring_size = 2; ring_size <= static_cast<u32>(2 * max_rings); ring_size += 2)
        {
            // LEFT COLUMN
            --gSpriteGrid_start_x;
            gSpriteGrid_start_y += 2 - ring_size;
            SearchTileColumnForClosestSprite(ring_size - 1);

            if (!search_mode)
            {
                pCollisionSprite = sSpriteGridSmallestDistSprite;
                if (sSpriteGridSmallestDistSprite)
                {
                    break;
                }
            }

            // TOP ROW
            --gSpriteGrid_start_y;
            SearchTileStripForClosestSprite(ring_size + 1);

            if (!search_mode)
            {
                pCollisionSprite = sSpriteGridSmallestDistSprite;
                if (sSpriteGridSmallestDistSprite)
                {
                    break;
                }
            }

            // RIGHT COLUMN
            gSpriteGrid_start_x += ring_size;
            ++gSpriteGrid_start_y;
            SearchTileColumnForClosestSprite(ring_size - 1);

            if (!search_mode)
            {
                pCollisionSprite = sSpriteGridSmallestDistSprite;
                if (sSpriteGridSmallestDistSprite)
                {
                    break;
                }
            }

            // BOTTOM ROW
            gSpriteGrid_start_x -= ring_size;
            gSpriteGrid_start_y = gSpriteGrid_start_y + ring_size - 1;
            SearchTileStripForClosestSprite(ring_size + 1);

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
void SpriteGrid::SetSpriteToExclude(Sprite* pSprite)
{
    gSpriteGrid_exclusion_sprite = pSprite;
}

MATCH_FUNC(0x477e60)
Sprite* SpriteGrid::FindCollidingSpritePreferType(Sprite* pSprite, s32 desired_sprite_type)
{
    sSpriteGridPreferredType = desired_sprite_type;
    sSpriteGridSmallestDistSprite = NULL;

    gCollisionCounters->mQueryId++;

    pSprite->UpdateCollisionBoundsIfNeeded();
    pSprite->mpCollisionShape->SetCurrentRect();

    for (s32 top = gSpriteGrid_top; top <= gSpriteGrid_bottom; top++)
    {
        Sprite* pObj = FindCollidingSpriteInRow(top, pSprite);
        if (pObj)
        {
            return pObj;
        }
    }

    return sSpriteGridSmallestDistSprite;
}

MATCH_FUNC(0x477f30)
bool SpriteGrid::CollectRectCollisions(Fix16_Rect* pRect, bool bDeepCheck, s32 exclude_sprite_type, Sprite* pExclude, SpriteList* pOutList)
{
    sSpriteGridList = pOutList;
    bool bRet = SpriteGrid::CheckRectForCollisions(pRect, bDeepCheck, exclude_sprite_type, pExclude);
    sSpriteGridList = NULL;
    return bRet;
}

MATCH_FUNC(0x477f60)
bool SpriteGrid::CheckRectForCollisions(Fix16_Rect* pRect, bool bDeepCheck, s32 exclude_sprite_type, Sprite* pExclude)
{
    bool bRet = false;
    ++gCollisionCounters->mQueryId;
    pRect->DoSetCurrentRect();
    sbDoCollisionCheck = bDeepCheck;
    sSpriteGridExcludeTypes = exclude_sprite_type;
    SetSpriteToExclude(pExclude);

    for (s32 y_pos = gSpriteGrid_top; y_pos <= gSpriteGrid_bottom; y_pos++)
    {
        if (SpriteGrid::CheckRowForRectCollisions(y_pos, pRect))
        {
            if (sSpriteGridList)
            {
                bRet = true;
            }
            else
            {
                SetSpriteToExclude(NULL);
                return true;
            }
        }
    }
    SetSpriteToExclude(NULL);
    return bRet;
}

MATCH_FUNC(0x478040)
SpriteGrid::SpriteGrid()
{
    Clear();
}

MATCH_FUNC(0x478050)
SpriteGrid::~SpriteGrid()
{
    Empty();
}

MATCH_FUNC(0x478060)
void SpriteGrid::CheckTileSpritesForClosestMatch(GridSpriteLink* pSpriteLinks)
{
    Fix16 dist;
    for (GridSpriteLink* pColIter = pSpriteLinks; pColIter; pColIter = pColIter->mpNext)
    {
        Sprite* pLinkSprite = pColIter->mpSprite;
        if ((pColIter->mpSprite->TypeIs(sSpriteGridSpriteType1) ||
             pColIter->mpSprite->TypeIs(sSpriteGridSpriteType2)) &&
            pLinkSprite != sSpriteGridExcludeSprite)
        {

            if (pLinkSprite->mZpos <= gSpriteGrid_zpos_high && pLinkSprite->mZpos >= gSpriteGrid_zpos_low)
            {
                if (sSpriteGridSearchMode == 3)
                {
                    dist = sSpriteGridExcludeSprite->MinDistanceToAnySpriteBBoxCorner(pLinkSprite);
                    if (dist < gSpriteGrid_smallestDistance)
                    {
                        if (pColIter->mpSprite->GetType() == sprite_types_enum::car_2)
                        {
                            if (pColIter->mpSprite->mCarBcPtr->IsEnterable())
                            {
                                gSpriteGrid_smallestDistance = dist;
                                sSpriteGridSmallestDistSprite = pColIter->mpSprite;
                            }
                        }
                        else
                        {
                            gSpriteGrid_smallestDistance = dist;
                            sSpriteGridSmallestDistSprite = pColIter->mpSprite;
                        }
                    }
                }
                else
                {

                    dist = sSpriteGridExcludeSprite->ManhattanDistance(pLinkSprite);

                    if (dist < gSpriteGrid_smallestDistance)
                    {
                        if (pLinkSprite->IsThreatToSearchingPed())
                        {
                            gSpriteGrid_smallestDistance = dist;
                            sSpriteGridSmallestDistSprite = pColIter->mpSprite;
                        }
                    }
                }
            }
        }
    }
}

MATCH_FUNC(0x478160)
void SpriteGrid::SearchTileColumnForClosestSprite(u8 height)
{
    u32 y_pos = gSpriteGrid_start_y;
    if (static_cast<u32>(gSpriteGrid_start_y) < gSpriteGrid_start_y + static_cast<u32>(height))
    {
        // Won't match without this redundant iter
        GridCell** pXItemIter = &this->mRows[gSpriteGrid_start_y];
        while (y_pos < gSpriteGrid_start_y + static_cast<u32>(height))
        {
            CheckTileSpritesForClosestMatch(GetCollideListAt(gSpriteGrid_start_x, y_pos));
            ++y_pos;
            ++pXItemIter;
        }
    }
}

MATCH_FUNC(0x4781E0)
void SpriteGrid::SearchTileStripForClosestSprite(u8 width)
{
    gSpriteGrid_left = gSpriteGrid_start_x;
    gSpriteGrid_right = gSpriteGrid_start_x + width - 1;

    for (GridCell* pXItemIter = GetFirstXCellInRow(gSpriteGrid_start_y); pXItemIter; pXItemIter = pXItemIter->mpNext)
    {
        if (pXItemIter->mX > gSpriteGrid_right)
        {
            break;
        }
        CheckTileSpritesForClosestMatch(pXItemIter->mpSpriteLinks);
    }
}

MATCH_FUNC(0x478240)
void SpriteGrid::AddToDrawList(s32 left, s32 right, s32 top, s32 bottom)
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
                        if (pLinkIter->mpSprite->IsTypeAbove1())
                        {
                            gSpriteRenderer->DisplayAdd(pLinkIter->mpSprite);
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
void SpriteGrid::DoRemove(s32 x_pos, s32 y_pos, Sprite* pSprite)
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

                    gGridSpriteLink_Pool->DeAllocate(pLink);

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
                        gGridCell_Pool->DeAllocate(pCell);
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
void SpriteGrid::RemoveFromRowBuckets(s32 y_pos, Sprite* pSprite)
{
    s32 x_pos = gSpriteGrid_left;
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

                    gGridSpriteLink_Pool->DeAllocate(pLink);

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
                        pCell = gGridCell_Pool->UnlinkAndReturnNext(pCell);
                    }
                    else
                    {
                        pPrevCell = pCell;
                        pCell = pCell->mpNext;
                    }
                    ++x_pos;

                    if (x_pos > gSpriteGrid_right)
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
void SpriteGrid::AddToSingleBucketAt(s32 x_pos, s32 y_pos, Sprite* pSprite)
{
    GridSpriteLink* pNewCollide = gGridSpriteLink_Pool->Allocate();

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

    GridCell* pNewItem = gGridCell_Pool->Allocate();

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
void SpriteGrid::AddToRowBuckets(s32 y_pos, Sprite* pSprite)
{
    s32 x_pos = gSpriteGrid_left;
    GridCell* pNextCell = this->mRows[y_pos];
    GridCell* pPrevCell = NULL;
    while (x_pos <= gSpriteGrid_right)
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
                GridSpriteLink* pLink = gGridSpriteLink_Pool->Allocate();
                pLink->mpSprite = pSprite;
                pLink->mpNext = pNextCell->mpSpriteLinks;
                pNextCell->mpSpriteLinks = pLink;
                if (x_pos > gSpriteGrid_right)
                {
                    return;
                }
            }
            pPrevCell = pNextCell;
            pNextCell = pNextCell->mpNext;
        }

        GridSpriteLink* pLink = gGridSpriteLink_Pool->Allocate();
        pLink->mpSprite = pSprite;
        GridCell* pCell = gGridCell_Pool->Allocate();

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
GridCell* SpriteGrid::GetFirstXCellInRow(s32 y_pos) const
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
        if (cell_x >= gSpriteGrid_left)
        {
            break;
        }
        if (cell_x > gSpriteGrid_right)
        {
            return NULL;
        }
    }
    return pIter;
}

// https://decomp.me/scratch/me1ge
MATCH_FUNC(0x4785d0)
bool SpriteGrid::CheckRowForRectCollisions(u32 y_pos, Fix16_Rect* pRect)
{
    GridCell* pXItemIter = GetFirstXCellInRow(y_pos);
    bool bRet = false;
    while (pXItemIter)
    {
        if (pXItemIter->mX > gSpriteGrid_right)
        {
            return bRet;
        }

        for (GridSpriteLink* pObj = pXItemIter->mpSpriteLinks; pObj; pObj = pObj->mpNext)
        {
            Sprite* pLinkSprite = pObj->mpSprite;
            if (!pObj->mpSprite->TypeIs(sSpriteGridExcludeTypes) &&
                pLinkSprite != gSpriteGrid_exclusion_sprite &&
                !pObj->mpSprite->mpCollisionShape->CollisionIdIs_446930(gCollisionCounters->mQueryId) &&
                pLinkSprite->ShouldCollideWithSprite(NULL))
            {
                gCollisionCounters->mTestCount++;

                Fix16_Rect* pBBox = &pObj->mpSprite->mpCollisionShape->mBoundingBox;
                if (pRect->AABB_Intersects(pBBox) &&
                    (!sbDoCollisionCheck || pObj->mpSprite->mpCollisionShape->IsZeroWidth() ||
                     pObj->mpSprite->mRotation.jIsAxisAligned() || pObj->mpSprite->IntersectsRectSAT(pRect) ||
                     pRect->IntersectsSpriteRenderingRect(pObj->mpSprite)))
                {
                    if (sSpriteGridList)
                    {
                        bRet = true;
                        // Add to list and keep going to add more
                        sSpriteGridList->AddSprite(pObj->mpSprite);
                    }
                    else
                    {
                        // No list so stop here
                        return true;
                    }
                }

                pObj->mpSprite->mpCollisionShape->SetCollisionId(gCollisionCounters->mQueryId);
            }

        }
        pXItemIter = pXItemIter->mpNext;
    }
    return bRet;
}

MATCH_FUNC(0x478750)
bool SpriteGrid::CheckAndHandleCollisionsInStrip(u32 y_pos, Sprite* pSprite)
{
    bool bRet = false;
    GridCell* pIter = GetFirstXCellInRow(y_pos);
    while (pIter)
    {
        if (pIter->mX > gSpriteGrid_right)
        {
            break;
        }

        for (GridSpriteLink* pLinkIter = pIter->mpSpriteLinks; pLinkIter; pLinkIter = pLinkIter->mpNext)
        {
            if (!pLinkIter->mpSprite->mpCollisionShape->CollisionIdIs_446930(gCollisionCounters->mQueryId))
            {
                gCollisionCounters->mTestCount++;
                if (pSprite->CollidesWith(pLinkIter->mpSprite))
                {
                    bRet = true;
                    pLinkIter->mpSprite->HandleObjectCollision(pSprite);
                }
                pLinkIter->mpSprite->mpCollisionShape->SetCollisionId(gCollisionCounters->mQueryId);
            }
        }
        pIter = pIter->mpNext;
    }
    return bRet;
}

MATCH_FUNC(0x4787e0)
bool SpriteGrid::CheckAndHandleRowCollisionsForSprite(u32 y_pos, Sprite* pSprite)
{
    bool bRet = false;
    GridCell* pXItemIter = GetFirstXCellInRow(y_pos);
    while (pXItemIter)
    {
        if (pXItemIter->mX > gSpriteGrid_right)
        {
            break;
        }

        for (GridSpriteLink* pLinkIter = pXItemIter->mpSpriteLinks; pLinkIter; pLinkIter = pLinkIter->mpNext)
        {
            if (pLinkIter->mpSprite->TypeIs(sSpriteGridSpriteTypeFilter) &&
                !pLinkIter->mpSprite->mpCollisionShape->CollisionIdIs_446930(gCollisionCounters->mQueryId))
            {
                gCollisionCounters->mTestCount++;

                if (pSprite->CollidesWith(pLinkIter->mpSprite))
                {
                    bRet = true;
                    pLinkIter->mpSprite->ProcessCarToCarImpactIfCar(pSprite);
                }

                pLinkIter->mpSprite->mpCollisionShape->SetCollisionId(gCollisionCounters->mQueryId);
            }
        }
        pXItemIter = pXItemIter->mpNext;
    }

    return bRet;
}

MATCH_FUNC(0x478880)
Sprite* SpriteGrid::FindCollidingSpriteInRow(u32 y_pos, Sprite* pSprite)
{
    GridCell* pXItem = GetFirstXCellInRow(y_pos);
    while (pXItem)
    {
        if (pXItem->mX > gSpriteGrid_right)
        {
            return NULL;
        }

        for (GridSpriteLink* pObj = pXItem->mpSpriteLinks; pObj; pObj = pObj->mpNext)
        {
            Sprite* pLinkSprite = pObj->mpSprite;
            if (pObj->mpSprite == gSpriteGrid_exclusion_sprite ||
                sSpriteGridSmallestDistSprite && !pLinkSprite->TypeIs(sSpriteGridPreferredType) ||
                pLinkSprite->mpCollisionShape->CollisionIdIs_446930(gCollisionCounters->mQueryId) || !pLinkSprite->ShouldCollideWithSprite(pSprite))
            {
                continue;
            }

            gCollisionCounters->mTestCount++;

            if (pSprite->CollidesWith(pObj->mpSprite))
            {
                if (!pObj->mpSprite->TypeIs(sSpriteGridPreferredType))
                {
                    sSpriteGridSmallestDistSprite = pObj->mpSprite;
                }
                else
                {
                    return pObj->mpSprite;
                }
            }
            pObj->mpSprite->mpCollisionShape->SetCollisionId(gCollisionCounters->mQueryId);
        }
        pXItem = pXItem->mpNext;
    }
    return NULL;
}

MATCH_FUNC(0x478950)
void SpriteGrid::DebugLog(s32 x_pos, s32 y_pos) const
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
            sprintf(gTmpBuffer, "(%d,%d):", x_pos, y_pos);
            gErrorLog_67C530.Write_Log(gTmpBuffer);
            for (GridSpriteLink* j = i->mpSpriteLinks; j; j = j->mpNext)
            {
                if (j->mpSprite)
                {
                    sprintf(gTmpBuffer, " %d", static_cast<u16>(j->mpSprite->mInstanceId));
                    gErrorLog_67C530.Write_Log(gTmpBuffer);
                }
            }
            gErrorLog_67C530.Write_Log("\n");
        }
    }
}

MATCH_FUNC(0x4789f0)
GridCell** SpriteGrid::Clear()
{
    for (u32 i = 0; i < GTA2_COUNTOF(mRows); i++)
    {
        mRows[i] = NULL;
    }
    return mRows;
}

MATCH_FUNC(0x478A10)
void SpriteGrid::Empty()
{
}