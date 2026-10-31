#include "RouteFinder.hpp"
#include "Globals.hpp"
#include "debug.hpp"
#include "error.hpp"
#include "file.hpp"
#include "map_0x370.hpp"
#include "Game.hpp"
#include "Player.hpp"
#include "Camera.hpp"
#include "Hud.hpp"
#include "Frontend.hpp"
#include <cstdio>

// Forward declarations: the functions below are in address order
static inline void WorldToScreen_RouteFinder_40CFC0(Camera* pCam, Fix16 x, Fix16 y, Fix16 z, Fix16* pOutX, Fix16* pOutY);

DEFINE_GLOBAL(RouteFinder*, gRouteFinder_6FFDC8, 0x6FFDC8);
DEFINE_STATIC_MEMBER(u16, RouteFinder, sLastRouteLength, 0x6ffdcc);
DEFINE_GLOBAL_INIT(Fix16, kFpOne_6FFC7C, Fix16(1), 0x6FFC7C);
DEFINE_GLOBAL_INIT(Fix16, kFpEight_6FFC9C, Fix16(8), 0x6FFC9C);
EXTERN_GLOBAL(s16, gStyleTextFont_703BAA);

MATCH_FUNC(0x588580)
bool Junction::IsUsableWithArrowType_588580(s32 arrow_type) const
{
    if (arrow_type == red_2)
    {
        if (this->mType != 1)
        {
            return true;
        }
    }
    else if (arrow_type == green_1)
    {
        if (this->mType != 2)
        {
            return true;
        }
    }
    else if (arrow_type == green_or_red_3)
    {
        return true;
    }
    return false;
}

MATCH_FUNC(0x5885c0)
u16 Junction::GetDirectionToJunction_5885C0(u16 junction_idx) const
{
    if (junction_idx != 0)
    {
        if (mN.GetIndex_0040CE90() == junction_idx)
        {
            return 1;
        }
        if (mS.GetIndex_0040CE90() == junction_idx)
        {
            return 2;
        }
        if (mE.GetIndex_0040CE90() != junction_idx)
        {
            return 3;
        }
        else
        {
            return 4;
        }
    }
    else
    {
        return gRouteFinder_6FFDC8->mNoJunctionDirection;
    }
}

MATCH_FUNC(0x588620)
void RouteFinder::ShowJunctionIds_588620()
{
    Junction* pJunction = &mJunctions[1];
    for (u16 i = 1; i < GTA2_COUNTOF(mJunctions); i++, pJunction++)
    {
        if (!pJunction->mMinX)
        {
            break;
        }

        if (gGame_0x40_67E008->mLocalPlayer->mViewCamera.IsPointInBoundaries_58CF10(pJunction->mMinX,
                                                                                              static_cast<s32>(pJunction->mMinY)))
        {
            Fix16 screen_x;
            Fix16 screen_y;
            WorldToScreen_RouteFinder_40CFC0(&gGame_0x40_67E008->mLocalPlayer->mViewCamera,
                                             Fix16(pJunction->mMinX),
                                             Fix16(pJunction->mMinY),
                                             gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(Fix16(pJunction->mMinX),
                                                                                           Fix16(pJunction->mMinY)),
                                             &screen_x,
                                             &screen_y);

            swprintf(tmpBuff_67BD9C, L"%d", i);
            gHud_2B00_706620->mTexts.DisplayText_5D1F50(tmpBuff_67BD9C, screen_x.ToInt(), screen_y.ToInt(), gStyleTextFont_703BAA, 1);
        }
    }
}

// 9.6f 0x40CFC0: Camera::WorldToScreen_40CFC0 (x/y/z by value, writes through two out pointers), but
// the original reads this file's copies of the constants (0x6FFC7C, 0x6FFC9C), so the Camera.hpp inline can't
// be used here
static inline void WorldToScreen_RouteFinder_40CFC0(Camera* pCam, Fix16 x, Fix16 y, Fix16 z, Fix16* pOutX, Fix16* pOutY)
{
    Fix16 u = pCam->mCurPos.mZ - z;
    Fix16 t(kFpOne_6FFC7C / Fix16(u.mValue + kFpEight_6FFC9C.mValue, 0));
    *pOutX = (((x - pCam->mCurPos.mX) * pCam->mScreenScale.y) * t) + Fix16(320);
    *pOutY = (((y - pCam->mCurPos.mY) * pCam->mScreenScale.y) * t) + Fix16(240);
}

MATCH_FUNC(0x588810)
void RouteFinder::RoadOff_588810(u8 x, u8 y, u8 z)
{
    const u16 r1 = RouteFinder::FindHorzSegmentJunction_588E60(x, y, z, false, green_or_red_3);
    const u16 r2 = RouteFinder::FindHorzSegmentJunction_588E60(x, y, z, true, green_or_red_3);
    if (r1 && r2)
    {
        Junction* j1 = &mJunctions[r1];
        Junction* j2 = &mJunctions[r2];
        if ((j1->mE.GetIndex_0040CE90()) == r2)
        {
            j1->mE.Disable_40CEC0();
            j2->mW.Disable_40CEC0();
        }
        else if ((j1->mW.GetIndex_0040CE90()) == r2)
        {
            j1->mW.Disable_40CEC0();
            j2->mE.Disable_40CEC0();
        }
    }
    else
    {
        const u16 r3 = RouteFinder::FindVertSegmentJunction_588F30(x, y, z, false, green_or_red_3);
        const u16 r4 = RouteFinder::FindVertSegmentJunction_588F30(x, y, z, true, green_or_red_3);
        if (r3 && r4)
        {
            Junction* j1 = &mJunctions[r3];
            Junction* j2 = &mJunctions[r4];

            if ((j1->mS.GetIndex_0040CE90()) == r4)
            {
                j1->mS.Disable_40CEC0();
                j2->mN.Disable_40CEC0();
            }
            else if ((j1->mN.GetIndex_0040CE90()) == r4)
            {
                j1->mN.Disable_40CEC0();
                j2->mS.Disable_40CEC0();
            }
        }
    }
}

MATCH_FUNC(0x588950)
void RouteFinder::RoadOn_588950(u8 x, u8 y, u8 z)
{
    // Strangely not the exact inverse logic of RoadOff
    const u16 r1 = RouteFinder::FindHorzSegmentJunction_588E60(x, y, z, false, green_or_red_3);
    const u16 r2 = RouteFinder::FindHorzSegmentJunction_588E60(x, y, z, true, green_or_red_3);
    if (r1 && r2)
    {
        Junction* j1 = &mJunctions[r1];
        Junction* j2 = &mJunctions[r2];
        if ((j1->mE.GetIndex_0040CE90()) == r2)
        {
            j1->mE.Enable_40CEB0();
            j2->mW.Enable_40CEB0();
        }
        else if ((j1->mW.GetIndex_0040CE90()) == r2)
        {
            j1->mW.Enable_40CEB0();
            j2->mE.Enable_40CEB0();
        }
    }

    const u16 r3 = RouteFinder::FindVertSegmentJunction_588F30(x, y, z, false, green_or_red_3);
    const u16 r4 = RouteFinder::FindVertSegmentJunction_588F30(x, y, z, true, green_or_red_3);
    if (r3 && r4)
    {
        Junction* j1 = &mJunctions[r3];
        Junction* j2 = &mJunctions[r4];

        if ((j1->mS.GetIndex_0040CE90()) == r4)
        {
            j1->mS.Enable_40CEB0();
        }
        else if ((j1->mN.GetIndex_0040CE90()) == r4)
        {
            j1->mN.Enable_40CEB0();
        }

        if ((j2->mS.GetIndex_0040CE90()) == r3)
        {
            j2->mS.Enable_40CEB0();
        }
        else if ((j2->mN.GetIndex_0040CE90()) == r3)
        {
            j2->mN.Enable_40CEB0();
        }
    }
}

MATCH_FUNC(0x588aa0)
u16 RouteFinder::IsPointInJunctionBounds_588AA0(u8 x, u8 y, u16 junc_idx1, u16 junc_idx2)
{
    u8 x1;
    u8 y1;
    u8 y2;
    u8 x2;

    Junction* pJ1 = &this->mJunctions[junc_idx1];
    Junction* pJ2 = &this->mJunctions[junc_idx2];

    if (pJ1->mMinX <= pJ2->mMinX)
    {
        x1 = pJ1->mMinX;
    }
    else
    {
        x1 = pJ2->mMinX;
    }

    if (pJ1->mMaxX > pJ2->mMaxX)
    {
        x2 = pJ1->mMaxX;
    }
    else
    {
        x2 = pJ2->mMaxX;
    }

    if (pJ1->mMinY <= pJ2->mMinY)
    {
        y1 = pJ1->mMinY;
    }
    else
    {
        y1 = pJ2->mMinY;
    }

    if (pJ1->mMaxY > pJ2->mMaxY)
    {
        y2 = pJ1->mMaxY;
    }
    else
    {
        y2 = pJ2->mMaxY;
    }

    if (x >= x1 && x <= x2 && y >= y1 && y <= y2)
    {
        return true;
    }
    return false;
}

MATCH_FUNC(0x588b30)
void RouteFinder::Load_RGEN_588B30()
{
    File::Global_Read_4A71C0(mJunctions, 0x2210);
    File::Global_Read_4A71C0(this->mHorzSegments, 0x1108);
    File::Global_Read_4A71C0(this->mVertSegments, 0x1108);
    File::Global_Read_4A71C0(&this->mUnused4, 2);
    File::Global_Read_4A71C0(&this->mHorzSegmentCount, 2);
    File::Global_Read_4A71C0(&this->mVertSegmentCount, 2);
    this->mRouteCount = 0;

    if (bLog_routefinder_67D6D1)
    {
        s32 junction_idx = 0;
        do
        {
            sprintf(gTmpBuffer_67C598,
                    "Junc: %d (%d, %d) n %d s %d w %d e %d",
                    junction_idx,
                    mJunctions[junction_idx].mMinX,
                    mJunctions[junction_idx].mMinY,
                    mJunctions[junction_idx].mN.GetIndex_0040CE90(),
                    mJunctions[junction_idx].mS.GetIndex_0040CE90(),
                    mJunctions[junction_idx].mW.GetIndex_0040CE90(),
                    mJunctions[junction_idx].mE.GetIndex_0040CE90());
            gErrorLog_67C530.Write_4D9620(gTmpBuffer_67C598);

            if (junction_idx > 0 && mJunctions[junction_idx].mMinX == 0 && mJunctions[junction_idx].mMinY == 0)
            {
                break;
            }
            junction_idx++;
        } while (junction_idx < 0x221);

        gErrorLog_67C530.Write_4D9620("     ");
    }
}

MATCH_FUNC(0x588c60)
void RouteFinder::Reset_588C60()
{
    memset(this->mVisited, 0, sizeof(this->mVisited));
    memset(this->mNodes, 0, sizeof(this->mNodes));
    memset(this->mRoutes, 0, sizeof(this->mRoutes));
}

MATCH_FUNC(0x588ca0)
bool RouteFinder::HasBlockDesiredArrow_588CA0(const gmp_block_info* block, s32 arrow_type, u8 direction) const
{
    switch (direction)
    {
        case UP_1:

            switch (arrow_type)
            {
                case green_1:
                    if ((block->mArrows & block_arrow::green_up_4) != 0)
                    {
                        return true;
                    }
                    break;
                case red_2:
                    if ((block->mArrows & block_arrow::red_up_40) != 0)
                    {
                        return true;
                    }
                    break;
                case green_or_red_3:
                    if ((block->mArrows & (block_arrow::green_up_4 | block_arrow::red_up_40)) != 0)
                    {
                        return true;
                    }
                    break;
            }
            break;

        case DOWN_2:
            switch (arrow_type)
            {
                case green_1:
                    if ((block->mArrows & block_arrow::green_down_8) != 0)
                    {
                        return true;
                    }
                    break;
                case red_2:
                    if ((block->mArrows & block_arrow::red_down_80) != 0)
                    {
                        return true;
                    }
                    break;
                case green_or_red_3:
                    if ((block->mArrows & (block_arrow::green_down_8 | block_arrow::red_down_80)) != 0)
                    {
                        return true;
                    }
                    break;
            }
            break;

        case LEFT_3:
            switch (arrow_type)
            {
                case green_1:
                    if ((block->mArrows & block_arrow::green_left_1) != 0)
                    {
                        return true;
                    }
                    break;
                case red_2:
                    if ((block->mArrows & block_arrow::red_left_10) != 0)
                    {
                        return true;
                    }
                    break;
                case green_or_red_3:
                    if ((block->mArrows & block_arrow::left_mask) != 0)
                    {
                        return true;
                    }
                    break;
            }
            break;

        case RIGHT_4:
            switch (arrow_type)
            {
                case green_1:
                    if ((block->mArrows & block_arrow::green_right_2) != 0)
                    {
                        return true;
                    }
                    break;
                case red_2:
                    if ((block->mArrows & block_arrow::red_right_20) != 0)
                    {
                        return true;
                    }
                    break;
                case green_or_red_3:
                    if ((block->mArrows & block_arrow::right_mask) != 0)
                    {
                        return true;
                    }
                    break;
            }
            break;
    }
    return false;
}

MATCH_FUNC(0x588de0)
bool RouteFinder::HasBlockArrowInRoadDirection_588DE0(const gmp_block_info* pBlock, s32 arrow_type, s32 road_direction) const
{

    bool result = false;
    switch (road_direction)
    {
        case road_direction::up_1:
            result = HasBlockDesiredArrow_588CA0(pBlock, arrow_type, UP_1);
            break;
        case road_direction::down_2:
            result = HasBlockDesiredArrow_588CA0(pBlock, arrow_type, DOWN_2);
            break;
        case road_direction::left_4:
            result = HasBlockDesiredArrow_588CA0(pBlock, arrow_type, LEFT_3);
            break;
        case road_direction::right_3:
            result = HasBlockDesiredArrow_588CA0(pBlock, arrow_type, RIGHT_4);
            break;
    }
    return result;
}

MATCH_FUNC(0x588e60)
u16 RouteFinder::FindHorzSegmentJunction_588E60(u8 x, u8 y, u8 z, bool bIsDest, s32 arrow_type)
{
    JunctionSegment* pSegment = mHorzSegments;
    gmp_block_info* pBlockAtPos = gMap_0x370_6F6268->GetBlock_4DFE10(x, y, z);

    if (pBlockAtPos != NULL)
    {
        bool bHasArrow = RouteFinder::HasBlockDesiredArrow_588CA0(pBlockAtPos, arrow_type, LEFT_3);
        for (s16 junc_idx = 0; junc_idx < mHorzSegmentCount; pSegment++, junc_idx++)
        {
            if (pSegment->ContainsPoint_40CF20(x, y))
            {
                if (bIsDest == 1)
                {
                    if (bHasArrow == 1)
                    {
                        return pSegment->mJunctionNum1;
                    }
                    else
                    {
                        return pSegment->mJunctionNum2;
                    }
                }
                else
                {
                    if (bHasArrow == 1)
                    {
                        return pSegment->mJunctionNum2;
                    }
                    else
                    {
                        return pSegment->mJunctionNum1;
                    }
                }
            }
        }
    }
    return 0;
}

MATCH_FUNC(0x588f30)
u16 RouteFinder::FindVertSegmentJunction_588F30(u8 x, u8 y, u8 z, bool bIsDest, s32 arrow_type)
{
    JunctionSegment* pSegment = mVertSegments;
    gmp_block_info* pBlockAtPos = gMap_0x370_6F6268->GetBlock_4DFE10(x, y, z);

    if (pBlockAtPos != NULL)
    {
        bool bHasArrow = RouteFinder::HasBlockDesiredArrow_588CA0(pBlockAtPos, arrow_type, UP_1);
        for (s16 junc_idx = 0; junc_idx < mVertSegmentCount; pSegment++, junc_idx++)
        {
            if (pSegment->ContainsPoint_40CF20(x, y))
            {
                if (bIsDest == 1)
                {
                    if (bHasArrow == 1)
                    {
                        return pSegment->mJunctionNum1;
                    }
                    else
                    {
                        return pSegment->mJunctionNum2;
                    }
                }
                else
                {
                    if (bHasArrow == 1)
                    {
                        return pSegment->mJunctionNum2;
                    }
                    else
                    {
                        return pSegment->mJunctionNum1;
                    }
                }
            }
        }
    }
    return 0;
}

MATCH_FUNC(0x589000)
u16 RouteFinder::FindSegmentJunction_589000(u8 x_coord, u8 y_coord, u8 z_coord, bool bIsDest, s32 arrow_type)
{
    gmp_block_info* pBlock = gMap_0x370_6F6268->GetBlock_4DFE10(x_coord, y_coord, z_coord);

    if (pBlock)
    {
        if (RouteFinder::HasBlockDesiredArrow_588CA0(pBlock, arrow_type, UP_1) || RouteFinder::HasBlockDesiredArrow_588CA0(pBlock, arrow_type, DOWN_2))
        {
            return RouteFinder::FindVertSegmentJunction_588F30(x_coord, y_coord, z_coord, bIsDest, arrow_type);
        }
        if (RouteFinder::HasBlockDesiredArrow_588CA0(pBlock, arrow_type, LEFT_3) || RouteFinder::HasBlockDesiredArrow_588CA0(pBlock, arrow_type, RIGHT_4))
        {
            return RouteFinder::FindHorzSegmentJunction_588E60(x_coord, y_coord, z_coord, bIsDest, arrow_type);
        }
    }
    return 0;
}

MATCH_FUNC(0x5890d0)
void RouteFinder::FindArrowBlockInJunction_5890D0(u16 junction_idx, s32 direction, u8* xpos, u8* ypos)
{
    for (u8 y = mJunctions[junction_idx].mMinY; y <= mJunctions[junction_idx].mMaxY; y++)
    {
        for (u8 x = mJunctions[junction_idx].mMinX; x <= mJunctions[junction_idx].mMaxX; x++)
        {
            s32 z;
            gmp_block_info* block = gMap_0x370_6F6268->FindHighestBlockForCoord_4E4C30(x, y, &z);
            if (gMap_0x370_6F6268->CheckGreenArrowDirection_4E4B40(direction, block))
            {
                *xpos = x;
                *ypos = y;
                return;
            }
        }
    }

    for (u8 y_pos = mJunctions[junction_idx].mMinY; y_pos <= mJunctions[junction_idx].mMaxY; y_pos++)
    {
        for (u8 x_pos = mJunctions[junction_idx].mMinX; x_pos <= mJunctions[junction_idx].mMaxX; x_pos++)
        {
            s32 z;
            gmp_block_info* block = gMap_0x370_6F6268->FindHighestBlockForCoord_4E4C30(x_pos, y_pos, &z);
            if (gMap_0x370_6F6268->GetArrowDirectionFromBlock_4E5FC0(block, false))
            {
                *xpos = x_pos;
                *ypos = y_pos;
                return;
            }
        }
    }
}

MATCH_FUNC(0x589210)
s32 RouteFinder::NoRefs_589210(u8 x, u8 y, s32, u8 direction, s32, u16 junction_idx)
{
    Junction* pJunction = &mJunctions[junction_idx];
    s32 dy;
    s16 dx;
    switch (direction)
    {
        case 1:
            dx = 0;
            dy = -1;
            break;
        case 2:
            dx = 0;
            dy = 1;
            break;
        case 8:
            dx = 1;
            dy = 0;
            break;
        case 4:
            dx = -1;
            dy = 0;
            break;
    }

    s32 result = 0;
    if (pJunction->ContainsPoint(static_cast<u8>(x + static_cast<s16>(dx)), static_cast<u8>(y + dy)))
    {
        result = 1;
    }
    return result;
}

// dx/dy are uninitialised for a direction that isn't 1, 2, 4 or 8, as in the original
#pragma warning(push)
#pragma warning(disable : 4701)
MATCH_FUNC(0x5892d0)
RouteSearchNode::RouteSearchNode()
{
    mJunctionIdx = 0;
    mCost = -1;
    mExpanded = 0;
    mpParent = NULL;
    mpNext = NULL;
}
#pragma warning(pop)

MATCH_FUNC(0x5892f0)
RouteSearchNode* RouteFinder::NewChildNode_5892F0(RouteSearchNode* pParent, u16 idx, s16 step_cost)
{
    RouteSearchNode* pNode = &this->mNodes[this->m545Count++];

    s32 dy = abs(static_cast<u8>(mJunctions[idx].mMinY) - static_cast<u8>(mJunctions[mDestIdx].mMinY));
    s32 dx = abs(static_cast<u8>(mJunctions[idx].mMinX) - static_cast<u8>(mJunctions[mDestIdx].mMinX));
    s16 dist = dy + dx;
    s16 cost = pParent->mCost + step_cost;

    pNode->mJunctionIdx = idx;
    pNode->mCost = cost + dist;
    pNode->mpParent = pParent;
    pNode->mpNext = NULL;

    return pNode;
}

MATCH_FUNC(0x589390)
RouteSearchNode* RouteFinder::NewStartNode_589390(u16 junction_idx)
{
    RouteSearchNode* pNode = &mNodes[m545Count++];
    s32 distance = abs(mJunctions[junction_idx].mMinX - mJunctions[mDestIdx].mMinX) +
        abs(mJunctions[junction_idx].mMinY - mJunctions[mDestIdx].mMinY);

    pNode->mCost = distance;
    pNode->mJunctionIdx = junction_idx;
    pNode->mExpanded = 0;
    // field_6 is preserved by the original function.
    pNode->mpParent = NULL;
    pNode->mpNext = NULL;
    return pNode;
}

MATCH_FUNC(0x589420)
void RouteFinder::InsertIntoOpenList_589420(RouteSearchNode* pNode)
{
    mVisited[pNode->mJunctionIdx] = true;

    if (pNode->mCost < mOpenList->mCost)
    {
        pNode->mpNext = mOpenList;
        mOpenList = pNode;
    }
    else
    {
        RouteSearchNode* pInsertAfter;
        for (pInsertAfter = mOpenList; pInsertAfter->mpNext != NULL && pInsertAfter->mpNext->mCost < pNode->mCost; pInsertAfter = pInsertAfter->mpNext)
        {
        }
        pNode->mpNext = pInsertAfter->mpNext;
        pInsertAfter->mpNext = pNode;
    }
}

MATCH_FUNC(0x589480)
bool RouteFinder::InitSearch_589480(u8 start_x, u8 start_y, u8 start_z, u8 dest_x, u8 dest_y, u8 dest_z, s32 arrow_type)
{
    m545Count = 0;
    memset(mVisited, 0, sizeof(mVisited));
    memset(mNodes, 0, sizeof(mNodes));

    mStartIdx = FindSegmentJunction_589000(start_x, start_y, start_z, false, arrow_type);
    if (mStartIdx == 0)
    {
        mStartIdx = FindSegmentJunction_589000(start_x, start_y, start_z, true, arrow_type);
    }

    u16 initialIdx = mStartIdx;
    Junction* pJunction = &mJunctions[initialIdx];
    if (start_x < pJunction->mMinX || start_x > pJunction->mMaxX || start_y < pJunction->mMinY || start_y > pJunction->mMaxY)
    {
        mStartIdx = FindSegmentJunction_589000(start_x, start_y, start_z, true, arrow_type);
    }

    mDestIdx = FindSegmentJunction_589000(dest_x, dest_y, dest_z, true, arrow_type);
    if (mDestIdx == mStartIdx)
    {
        mStartIdx = initialIdx;
    }

    if (mStartIdx != 0 && mDestIdx != 0)
    {
        NewStartNode_589390(mStartIdx);
        mVisited[0] = true;
        mOpenList = mNodes;
        mVisited[mStartIdx] = true;
        return true;
    }
    return false;
}

MATCH_FUNC(0x5895c0)
bool RouteFinder::InitSearchWithDirection_5895C0(u8 x, u8 y, u8 z, s32 arrow_type, s32 direction)
{
    m545Count = 0;
    memset(mVisited, 0, sizeof(mVisited));
    RouteSearchNode* pStart = mNodes;
    memset(pStart, 0, sizeof(mNodes));

    mStartIdx = FindSegmentJunction_589000(x, y, z, false, arrow_type);
    if (mStartIdx == 0)
    {
        mStartIdx = FindSegmentJunction_589000(x, y, z, true, arrow_type);
    }

    if (mStartIdx != 0)
    {
        Junction* pJunction = &mJunctions[mStartIdx];
        if (x < pJunction->mMinX || x > pJunction->mMaxX || y < pJunction->mMinY ||
            y > pJunction->mMaxY)
        {
            mStartIdx = FindSegmentJunction_589000(x, y, z, true, arrow_type);
        }

        if (mStartIdx != 0)
        {
            NewStartNode_589390(mStartIdx);
            mOpenList = pStart;
            mVisited[0] = true;
            mVisited[mStartIdx] = true;

            pJunction = &mJunctions[mStartIdx];
            u16 north = pJunction->mN.GetIndex_0040CE90();
            u16 south = pJunction->mS.GetIndex_0040CE90();
            u16 east = pJunction->mE.GetIndex_0040CE90();
            u16 west = pJunction->mW.GetIndex_0040CE90();
            pStart->mExpanded = 1;

            switch (direction)
            {
                case 1:
                    if (!pJunction->mS.GetIndex_0040CE90())
                    {
                        if (east)
                        {
                            AddChildNode_589990(pStart, east, pJunction->mE.GetLength());
                        }
                        else if (west)
                        {
                            AddChildNode_589990(pStart, west, pJunction->mW.GetLength());
                        }
                        else
                        {
                            AddChildNode_589990(pStart, north, pJunction->mN.GetLength());
                        }
                    }
                    else
                    {
                        AddChildNode_589990(pStart, pJunction->mS.GetIndex_0040CE90(), pJunction->mS.GetLength());
                    }
                    break;
                case 2:
                    if (!pJunction->mN.GetIndex_0040CE90())
                    {
                        if (east)
                        {
                            AddChildNode_589990(pStart, east, pJunction->mE.GetLength());
                        }
                        else if (west)
                        {
                            AddChildNode_589990(pStart, west, pJunction->mW.GetLength());
                        }
                        else
                        {
                            AddChildNode_589990(pStart, south, pJunction->mS.GetLength());
                        }
                    }
                    else
                    {
                        AddChildNode_589990(pStart, pJunction->mN.GetIndex_0040CE90(), pJunction->mN.GetLength());
                    }
                    break;
                case 3:
                    if (!pJunction->mW.GetIndex_0040CE90())
                    {
                        if (south)
                        {
                            AddChildNode_589990(pStart, south, pJunction->mS.GetLength());
                        }
                        else if (north)
                        {
                            AddChildNode_589990(pStart, north, pJunction->mN.GetLength());
                        }
                        else
                        {
                            AddChildNode_589990(pStart, east, pJunction->mE.GetLength());
                        }
                    }
                    else
                    {
                        AddChildNode_589990(pStart, pJunction->mW.GetIndex_0040CE90(), pJunction->mW.GetLength());
                    }
                    break;
                case 4:
                    if (!pJunction->mE.GetIndex_0040CE90())
                    {
                        if (south)
                        {
                            AddChildNode_589990(pStart, south, pJunction->mS.GetLength());
                        }
                        else if (north)
                        {
                            AddChildNode_589990(pStart, north, pJunction->mN.GetLength());
                        }
                        else
                        {
                            AddChildNode_589990(pStart, west, pJunction->mW.GetLength());
                        }
                    }
                    else
                    {
                        AddChildNode_589990(pStart, pJunction->mE.GetIndex_0040CE90(), pJunction->mE.GetLength());
                    }
                    break;
            }
            return true;
        }
    }
    return false;
}

MATCH_FUNC(0x589930)
void RouteFinder::CancelRoute_589930(s16 route_idx)
{
    this->mRoutes[route_idx].mJunctions[0] = 0;
    if (this->mRouteCount > 0)
    {
        --this->mRouteCount;
    }
}

MATCH_FUNC(0x589960)
s16 RouteFinder::GetFreeRouteIdx_589960()
{
    s16 route_idx = 1;
    if (this->mRouteCount < 50)
    {
        while (route_idx < 50)
        {
            if (mRoutes[route_idx++].mJunctions[0] == 0)
            {
                return route_idx - 1;
            }
        }
    }
    return -1;
}

MATCH_FUNC(0x589990)
u16 RouteFinder::AddChildNode_589990(RouteSearchNode* pParent, u16 junction_idx, s16 step_cost)
{
    RouteSearchNode* pChild = NewChildNode_5892F0(pParent, junction_idx, step_cost);
    InsertIntoOpenList_589420(pChild);
    return pChild->mJunctionIdx;
}

// Returns true if a neighbour of the node's junction that hasn't been visited yet completes the route
MATCH_FUNC(0x5899c0)
bool RouteFinder::ExpandNode_5899C0(RouteSearchNode* pNode, s32 arrow_type)
{
    Junction* pJunction = &mJunctions[pNode->mJunctionIdx];
    u16 north = pJunction->mN.GetIndex_0040CE90();
    u16 west = pJunction->mW.GetIndex_0040CE90();
    u16 south = pJunction->mS.GetIndex_0040CE90();
    u16 east = pJunction->mE.GetIndex_0040CE90();
    pNode->mExpanded = 1;

    if (!mVisited[north] && pJunction->mN.IsEnabled() && mJunctions[north].IsUsableWithArrowType_588580(arrow_type) &&
        AddChildNode_589990(pNode, north, pJunction->mN.GetLength()) == mDestIdx)
    {
        return true;
    }
    if (!mVisited[south] && pJunction->mS.IsEnabled() && mJunctions[south].IsUsableWithArrowType_588580(arrow_type) &&
        AddChildNode_589990(pNode, south, pJunction->mS.GetLength()) == mDestIdx)
    {
        return true;
    }
    if (!mVisited[west] && pJunction->mW.IsEnabled() && mJunctions[west].IsUsableWithArrowType_588580(arrow_type) &&
        AddChildNode_589990(pNode, west, pJunction->mW.GetLength()) == mDestIdx)
    {
        return true;
    }
    if (!mVisited[east] && pJunction->mE.IsEnabled() && mJunctions[east].IsUsableWithArrowType_588580(arrow_type) &&
        AddChildNode_589990(pNode, east, pJunction->mE.GetLength()) == mDestIdx)
    {
        return true;
    }
    return false;
}

MATCH_FUNC(0x589bb0)
bool RouteFinder::ExpandNodeUnlessOffScreen_589BB0(RouteSearchNode* pNode, s32 arrow_type)
{
    Junction* pJunction = &mJunctions[pNode->mJunctionIdx];
    if (!gGame_0x40_67E008->IsPointOnScreen_4B9A80(pJunction->mMinX, pJunction->mMinY) &&
        !gGame_0x40_67E008->IsPointOnScreen_4B9A80(pJunction->mMaxX, pJunction->mMinY) &&
        !gGame_0x40_67E008->IsPointOnScreen_4B9A80(pJunction->mMinX, pJunction->mMaxY) &&
        !gGame_0x40_67E008->IsPointOnScreen_4B9A80(pJunction->mMaxX, pJunction->mMaxY))
    {
        return true;
    }

    u16 north = pJunction->mN.GetIndex_0040CE90();
    u16 south = pJunction->mS.GetIndex_0040CE90();
    u16 west = pJunction->mW.GetIndex_0040CE90();
    u16 east = pJunction->mE.GetIndex_0040CE90();
    pNode->mExpanded = 1;

    if (!mVisited[north] && pJunction->mN.IsEnabled() && mJunctions[north].IsUsableWithArrowType_588580(arrow_type))
    {
        AddChildNode_589990(pNode, north, pJunction->mN.GetLength());
    }
    if (!mVisited[south] && pJunction->mS.IsEnabled() && mJunctions[south].IsUsableWithArrowType_588580(arrow_type))
    {
        AddChildNode_589990(pNode, south, pJunction->mS.GetLength());
    }
    if (!mVisited[west] && pJunction->mW.IsEnabled() && mJunctions[west].IsUsableWithArrowType_588580(arrow_type))
    {
        AddChildNode_589990(pNode, west, pJunction->mW.GetLength());
    }
    if (!mVisited[east] && pJunction->mE.IsEnabled() && mJunctions[east].IsUsableWithArrowType_588580(arrow_type))
    {
        AddChildNode_589990(pNode, east, pJunction->mE.GetLength());
    }
    return false;
}

MATCH_FUNC(0x589e00)
RouteSearchNode* RouteFinder::GetFirstUnexpandedNode_589E00()
{
    RouteSearchNode* pNode;

    for (pNode = mOpenList; pNode != NULL && pNode->mExpanded != 0; pNode = pNode->mpNext)
        ;
    return pNode;
}

MATCH_FUNC(0x589e20)
bool RouteFinder::SearchToDest_589E20(s32 arrow_type)
{
    bool bRet = false;
    RouteSearchNode* pOpenList = this->mOpenList;
    if (pOpenList->mJunctionIdx == this->mDestIdx)
    {
        return true;
    }

    if (pOpenList)
    {
        // while (true) + break keeps the original's dead test of bRet on loop entry, while (!bRet) drops it
        while (true)
        {
            if (bRet)
            {
                break;
            }

            RouteSearchNode* pNode = GetFirstUnexpandedNode_589E00();
            if (!pNode)
            {
                break;
            }
            bRet = ExpandNode_5899C0(pNode, arrow_type);
        }
    }
    return bRet;
}

MATCH_FUNC(0x589e70)
bool RouteFinder::SearchUntilOffScreen_589E70(s32 arrow_type)
{
    bool result = false;
    if (this->mOpenList != NULL)
    {
        while (true)
        {
            if (result)
            {
                break;
            }

            RouteSearchNode* pNode = GetFirstUnexpandedNode_589E00();
            if (pNode == NULL)
            {
                break;
            }
            result = ExpandNodeUnlessOffScreen_589BB0(pNode, arrow_type);
        }
    }
    return result;
}

MATCH_FUNC(0x589eb0)
s16 RouteFinder::StoreRoute_589EB0()
{
    u16 count = 0;
    if (m545Count == 0)
    {
        return -1;
    }

    RouteSearchNode* pNode = &mNodes[m545Count - 1];
    s16 route_idx = GetFreeRouteIdx_589960();
    if (route_idx == -1)
    {
        return -1;
    }

    while (pNode != NULL)
    {
        pNode = pNode->mpParent;
        count++;
    }
    sLastRouteLength = count;

    pNode = &mNodes[m545Count - 1];

    if (pNode != NULL)
    {
        do
        {
            mRoutes[route_idx].mJunctions[--count] = pNode->mJunctionIdx;
            pNode = pNode->mpParent;
        } while (pNode != NULL);
    }
    mRoutes[route_idx].mJunctions[sLastRouteLength] = 0;
    mRouteCount++;
    return route_idx;
}

MATCH_FUNC(0x589f70)
s16 RouteFinder::StoreRouteReversed_589F70()
{
    s16 length = 0;
    u8 out_idx = 0;
    if (m545Count == 0)
    {
        return -1;
    }

    RouteSearchNode* pNode = &mNodes[m545Count - 1];
    s16 route_idx = GetFreeRouteIdx_589960();
    if (route_idx == -1)
    {
        return -1;
    }

    while (pNode != NULL)
    {
        pNode = pNode->mpParent;
        length++;
    }
    sLastRouteLength = length;

    pNode = &mNodes[m545Count - 1];

    if (pNode != NULL)
    {
        do
        {
            mRoutes[route_idx].mJunctions[out_idx] = pNode->mJunctionIdx;
            pNode = pNode->mpParent;
            out_idx++;

        } while (pNode != NULL);
    }
    mRouteCount++;
    return route_idx;
}

MATCH_FUNC(0x58a020)
void RouteFinder::DebugPrintRoute_58A020(char_type route_idx)
{
    s32 route_pos = 0;
    u16 i = this->mRoutes[route_idx].mJunctions[static_cast<u16>(route_pos)];
    while (i)
    {
        Junction* pJunc = &this->mJunctions[i];
        sprintf(gTmpBuffer_67C598,
                "Junc: %d : (%d, %d)(%d, %d)",
                i,
                pJunc->mMinX,
                pJunc->mMinY,
                pJunc->mMaxX,
                pJunc->mMaxY);
        gErrorLog_67C530.Write_4D9620(gTmpBuffer_67C598);
        ++route_pos;
        i = this->mRoutes[route_idx].mJunctions[static_cast<u16>(route_pos)];
    }
}

MATCH_FUNC(0x58a0b0)
Junction* RouteFinder::GetJunction_58A0B0(u16 junction_idx)
{
    return &mJunctions[junction_idx];
}

MATCH_FUNC(0x58a0d0)
s16 RouteFinder::DoStartRoute_58A0D0(u8 x1, u8 y1, u8 z1, u8 x2, u8 y2, u8 z2, s32 arrow_type)
{
    if (InitSearch_589480(x1, y1, z1, x2, y2, z2, arrow_type) && SearchToDest_589E20(arrow_type))
    {
        return StoreRoute_589EB0();
    }
    else
    {
        return -1;
    }
}

MATCH_FUNC(0x58a130)
s16 RouteFinder::FindOffScreenRoute_58A130(u8 x, s16 y, u8 z, u8* pRouteLength, s32 arrow_type, s32 direction)
{
    if (InitSearchWithDirection_5895C0(x, static_cast<u8>(y), z, arrow_type, direction))
    {
        if (SearchUntilOffScreen_589E70(arrow_type))
        {
            s16 ret = StoreRouteReversed_589F70();
            *pRouteLength = static_cast<u8>(sLastRouteLength);
            return ret;
        }
    }
    return -1;
}

MATCH_FUNC(0x58a190)
u16 RouteFinder::StartRoute_58A190(u8 x1, u8 y1, u8 z1, u8 x2, u8 y2, u8 z2, s32 arrow_type)
{
    return DoStartRoute_58A0D0(x1, y1, z1, x2, y2, z2, arrow_type);
}

MATCH_FUNC(0x58a1c0)
RouteFinder::RouteFinder()
{
    mRouteCount = 0;
    mNoJunctionDirection = 0;
    mUnused4 = 0;
    memset(mJunctions, 0, sizeof(mJunctions));
    memset(mRoutes, 0, sizeof(mRoutes));
    mStartIdx = 0;
    mDestIdx = 0;
    memset(mNodes, 0, sizeof(mNodes));
    mOpenList = NULL;
    memset(mHorzSegments, 0, sizeof(mHorzSegments));
    memset(mVertSegments, 0, sizeof(mVertSegments));
    memset(mVisited, 0, sizeof(mVisited));
    mHorzSegmentCount = 0;
    mVertSegmentCount = 0;
    m545Count = 0;
}