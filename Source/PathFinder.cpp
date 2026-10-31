#include "PathFinder.hpp"
#include "Char_Pool.hpp"
#include "Globals.hpp"
#include "map_0x370.hpp"
#include <string.h>

DEFINE_STATIC_MEMBER(u8, PathFinder, sPathFinderSlopeZDelta, 0x6FDEEC);
DEFINE_STATIC_MEMBER(s16, PathFinder, sPathFinderBestCost, 0x6FDECE);

DEFINE_GLOBAL(PathFinder*, gPathFinder_6FDEF0, 0x6FDEF0);

DEFINE_GLOBAL_INIT(Fix16, kFpZero_6FDD98, Fix16(0), 0x6FDD98);
DEFINE_GLOBAL_INIT(Fix16, kFpOne64th_6FDD50, Fix16(0x100, 0), 0x6FDD50);
DEFINE_GLOBAL_INIT(Fix16, kFpQuarter_6FDC00, Fix16(0.25), 0x6FDC00);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_6FDCA8, Fix16(0.5), 0x6FDCA8);

DEFINE_STATIC_MEMBER(u8, PathFinder, sPathFinderEdge2GridX, 0x6FDBF8);
DEFINE_STATIC_MEMBER(u8, PathFinder, sPathFinderEdge2GridY, 0x6FDBF9);

DEFINE_STATIC_MEMBER(s32, PathFinder, sPathFinderAngleFace, 0x6FDD38);
DEFINE_STATIC_MEMBER(u8, PathFinder, sPathFinderEdge1X, 0x6FDBE2);
DEFINE_STATIC_MEMBER(u8, PathFinder, sPathFinderEdge1Y, 0x6FDBE3);
DEFINE_STATIC_MEMBER(u8, PathFinder, sPathFinderEdge1Z, 0x6FDBE4);
DEFINE_STATIC_MEMBER(u8, PathFinder, sPathFinderEdge1GridX, 0x6FDBE0);
DEFINE_STATIC_MEMBER(u8, PathFinder, sPathFinderEdge1GridY, 0x6FDBE1);

DEFINE_STATIC_MEMBER(u8, PathFinder, sPathFinderBestX, 0x6FDECA);
DEFINE_STATIC_MEMBER(u8, PathFinder, sPathFinderBestY, 0x6FDECB);
DEFINE_STATIC_MEMBER(u8, PathFinder, sPathFinderBestZ, 0x6FDECC);
DEFINE_STATIC_MEMBER(u8, PathFinder, sPathFinderBestGridX, 0x6FDEC8);
DEFINE_STATIC_MEMBER(u8, PathFinder, sPathFinderBestGridY, 0x6FDEC9);

DEFINE_STATIC_MEMBER(u8, PathFinder, sPathFinderEdge2X, 0x6FDBFA);
DEFINE_STATIC_MEMBER(u8, PathFinder, sPathFinderEdge2Y, 0x6FDBFB);
DEFINE_STATIC_MEMBER(u8, PathFinder, sPathFinderEdge2Z, 0x6FDBFC);
DEFINE_STATIC_MEMBER(u8, PathFinder, sPathFinderEdge3GridX, 0x6FDB68);
DEFINE_STATIC_MEMBER(u8, PathFinder, sPathFinderEdge3GridY, 0x6FDB69);

DEFINE_STATIC_MEMBER(u8, PathFinder, sPathFinderEdge3X, 0x6FDB6A);
DEFINE_STATIC_MEMBER(u8, PathFinder, sPathFinderEdge3Y, 0x6FDB6B);
DEFINE_STATIC_MEMBER(u8, PathFinder, sPathFinderEdge3Z, 0x6FDB6C);

DEFINE_STATIC_MEMBER(u8, PathFinder, sPathFinderEdge4X, 0x6FDBF2);
DEFINE_STATIC_MEMBER(u8, PathFinder, sPathFinderEdge4Y, 0x6FDBF3);
DEFINE_STATIC_MEMBER(u8, PathFinder, sPathFinderEdge4Z, 0x6FDBF4);
DEFINE_STATIC_MEMBER(u8, PathFinder, sPathFinderEdge4GridX, 0x6FDBF0);
DEFINE_STATIC_MEMBER(u8, PathFinder, sPathFinderEdge4GridY, 0x6FDBF1);

// EvaluateGridCell_554640 results: skip the cell, add it, or the grid edge in the facing direction is reached
enum
{
    k_cell_skip = 0,
    k_cell_open = 1,
    k_cell_edge_reached = 2,
};

MATCH_FUNC(0x554080)
bool PathFinder::CanMoveInDirection_554080(s32 path_direction) const
{
    if (mXpos25 > 1u && mXpos25 < 254u && mYpos26 > 1u && mYpos26 < 254u)
    {
        bool result =
            gMap_0x370_6F6268->IsMoveOntoSlopeTileBlocked_4E0130(mXpos25, mYpos26, mZpos27, path_direction, &sPathFinderSlopeZDelta, true) ==
            0;
        return result;
    }
    return false;
}

MATCH_FUNC(0x5540e0)
bool PathFinder::TestMoveToAdjacentTile_5540E0(u8 curr_xpos, u8 curr_ypos, u8 curr_zpos, u8 desired_xpos, u8 desired_ypos)
{
    mXpos25 = curr_xpos;
    mYpos26 = curr_ypos;
    mZpos27 = curr_zpos;
    return CanMoveToAdjacentTile_554110(desired_xpos, desired_ypos);
}

MATCH_FUNC(0x554110)
bool PathFinder::CanMoveToAdjacentTile_554110(u8 desired_xpos, u8 desired_ypos)
{

    sPathFinderSlopeZDelta = 0;

    const char_type xd = desired_xpos - mXpos25;
    const char_type yd = desired_ypos - mYpos26;

    if (xd == 0 && yd == 0)
    {
        return true; // there is no moving: of course, it's allowed
    }

    if (xd != 0 && yd != 0)
    {
        // No diagonal move from a slope tile
        if (gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(mXpos25, mYpos26, mZpos27))
        {
            return false;
        }

        if (xd == 1)
        {
            if (yd == 1)
            {
                // moving southeast
                if (gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(this->mXpos25, this->mYpos26 + 1, this->mZpos27))
                {
                    return false;
                }

                if (gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(this->mXpos25 + 1, this->mYpos26, this->mZpos27))
                {
                    return false;
                }

                if (CanMoveInDirection_554080(path_direction::down_2) && CanMoveInDirection_554080(path_direction::right_3))
                {
                    if (gMap_0x370_6F6268->IsMoveOntoSlopeTileBlocked_4E0130(this->mXpos25 + 1,
                                                                       this->mYpos26,
                                                                       this->mZpos27,
                                                                       path_direction::down_2,
                                                                       &sPathFinderSlopeZDelta,
                                                                       1))
                    {
                        return false;
                    }
                    return gMap_0x370_6F6268->IsMoveOntoSlopeTileBlocked_4E0130(this->mXpos25,
                                                                              this->mYpos26 + 1,
                                                                              this->mZpos27,
                                                                              path_direction::right_3,
                                                                              &sPathFinderSlopeZDelta,
                                                                              1) ? false : true;
                }
                return false;
            }

            if (gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(this->mXpos25, this->mYpos26 - 1, this->mZpos27))
            {
                return false;
            }

            if (gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(this->mXpos25 + 1, this->mYpos26, this->mZpos27))
            {
                return false;
            }

            if (!CanMoveInDirection_554080(path_direction::up_1) || !CanMoveInDirection_554080(path_direction::right_3))
            {
                return false;
            }

            if (gMap_0x370_6F6268
                    ->IsMoveOntoSlopeTileBlocked_4E0130(this->mXpos25 + 1, this->mYpos26, this->mZpos27, path_direction::up_1, &sPathFinderSlopeZDelta, 1))
            {
                return false;
            }
            return gMap_0x370_6F6268->IsMoveOntoSlopeTileBlocked_4E0130(this->mXpos25,
                                                                      this->mYpos26 - 1,
                                                                      this->mZpos27,
                                                                      path_direction::right_3,
                                                                      &sPathFinderSlopeZDelta,
                                                                      1) ? false : true;
        }

        if (yd == 1)
        {
            if (gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(this->mXpos25, this->mYpos26 + 1, this->mZpos27))
            {
                return false;
            }

            if (gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(this->mXpos25 - 1, this->mYpos26, this->mZpos27))
            {
                return false;
            }

            if (!CanMoveInDirection_554080(path_direction::down_2) || !CanMoveInDirection_554080(path_direction::left_4))
            {
                return false;
            }

            if (gMap_0x370_6F6268
                    ->IsMoveOntoSlopeTileBlocked_4E0130(this->mXpos25 - 1, this->mYpos26, this->mZpos27, path_direction::down_2, &sPathFinderSlopeZDelta, 1))
            {
                return false;
            }
            return gMap_0x370_6F6268
                       ->IsMoveOntoSlopeTileBlocked_4E0130(this->mXpos25, this->mYpos26 + 1, this->mZpos27, path_direction::left_4, &sPathFinderSlopeZDelta, 1) ? false : true;
        }

        if (gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(this->mXpos25, this->mYpos26 - 1, this->mZpos27))
        {
            return false;
        }

        if (gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(this->mXpos25 - 1, this->mYpos26, this->mZpos27))
        {
            return false;
        }

        if (!CanMoveInDirection_554080(path_direction::up_1) || !CanMoveInDirection_554080(path_direction::left_4))
        {
            return false;
        }

        if (gMap_0x370_6F6268
                ->IsMoveOntoSlopeTileBlocked_4E0130(this->mXpos25 - 1, this->mYpos26, this->mZpos27, path_direction::up_1, &sPathFinderSlopeZDelta, 1))
        {
            return false;
        }
        return gMap_0x370_6F6268->IsMoveOntoSlopeTileBlocked_4E0130(this->mXpos25,
                                                                  this->mYpos26 - 1,
                                                                  this->mZpos27,
                                                                  path_direction::left_4,
                                                                  &sPathFinderSlopeZDelta,
                                                                  1) ? false : true;
    }

    if (xd != 0)
    {
        if (xd == -1)
        {
            return CanMoveInDirection_554080(path_direction::left_4);
        }
        return CanMoveInDirection_554080(path_direction::right_3);
    }

    if (yd == -1)
    {
        return CanMoveInDirection_554080(path_direction::up_1);
    }
    return CanMoveInDirection_554080(path_direction::down_2);
}

MATCH_FUNC(0x5545c0)
void PathFinder::ClearGrid_5545C0()
{
    memset(this->mGrid, 0, sizeof(this->mGrid));
}

MATCH_FUNC(0x5545e0)
void PathFinder::Init_5545E0()
{
    memset(this->mGrid, 0, sizeof(this->mGrid));
    mbComputePathInProgress = false;
    mUnused34 = 1;
    mUnused36 = 0;
    mUnused3A = 0;
    mbTimedOut = true;
    mTimeOutCounter = 0;
}

MATCH_FUNC(0x554620)
void PathFinder::RemovePed_554620(s32 ped_id)
{
    if (ped_id == mPedId)
    {
        mbTimedOut = true;
        mTimeOutCounter = 0;
    }
}

MATCH_FUNC(0x554640)
char_type PathFinder::EvaluateGridCell_554640()
{
    if (mGridX <= 32)
    {
        if (mGridY <= 32)
        {
            if (mZpos22 <= 8)
            {
                mGridIdx = mGridX + 34 * mGridY;
                PathNode* pNode = &mGrid[mGridIdx];
                if (pNode->mIdx2 == 0)
                {
                    return k_cell_open;
                }
                if (pNode->mIdx1 != 1 && pNode->mXpos != mZpos22)
                {
                    if (abs(pNode->mXpos - mZpos22) >= 1)
                    {
                        pNode->mIdx1 = 1;
                        return k_cell_open;
                    }
                }
                return k_cell_skip;
            }
        }
    }
    if (mZStart != mZEnd)
    {
        return k_cell_skip;
    }
    switch (sPathFinderAngleFace)
    {
        case path_direction::up_1:
            if (mGridY < 1)
            {
                return k_cell_edge_reached;
            }
            break;
        case path_direction::down_2:
            if (mGridY > 32)
            {
                return k_cell_edge_reached;
            }
            break;
        case path_direction::right_3:
            if (mGridX > 32)
            {
                return k_cell_edge_reached;
            }
            break;
        case path_direction::left_4:
            if (mGridX < 1)
            {
                return k_cell_edge_reached;
            }
            break;
    }
    return k_cell_skip;
}

// https://decomp.me/scratch/f8WDL
MATCH_FUNC(0x554710)
void PathFinder::AddGridCell_554710()
{
    u16 cost_scale;
    // zpos assigned in both arms (not `zpos = mZpos22; if (..) zpos += ..;`): the extra
    // definition raises its priority above dx/dy so it is coloured first and keeps edx.
    u8 zpos;
    if (sPathFinderSlopeZDelta)
    {
        zpos = mZpos22 + sPathFinderSlopeZDelta;
    }
    else
    {
        zpos = mZpos22;
    }

    if (zpos != mZEnd)
    {
        cost_scale = 2;
    }
    else
    {
        cost_scale = 1;
    }

    mGridIdx = mGridX + 34 * mGridY;
    PathNode* pNode = &mGrid[mGridIdx];
    if (mGrid[mGridIdx].mIdx1 == 1 && zpos == pNode->mXpos)
    {
        pNode->mIdx1 = 0;
    }
    else
    {
        u16 heuristic;
        if (!mbFindTileMode)
        {
            // The sum assigned first and `*= cost_scale` after (one expression keeps cost_scale's multiply in the sum's
            // register and pushes ebp at the top instead of inside this branch). The dx/dy locals give
            // the original's evaluation order (x difference first).
            u16 dx = mXpos20 - mXEnd;
            u16 dy = mYpos21 - mYEnd;
            heuristic = dy * dy + dx * dx;
            heuristic *= cost_scale;
        }
        else
        {
            heuristic = mStartCost;
        }
        mpNode->mIdx1 = mGridX;
        mpNode->mIdx2 = mGridY;
        mpNode->mXpos = mXpos20;
        mpNode->mYpos = mYpos21;
        mpNode->mZpos = zpos;
        mpNode->mCost = heuristic;
        ++mpNode;
        ++mNodeCount;

        if (pNode->mIdx1 == 1)
        {
            pNode->mYpos = mDirection;
            pNode->mZpos = zpos;
        }
        else
        {
            pNode->mIdx2 = mDirection;
            pNode->mXpos = zpos;
        }
        pNode->mCost = mCurrentCost + 1;

        if (mGridY)
        {
            if (mGridY == 31)
            {
                sPathFinderEdge2GridX = mGridX;
                sPathFinderEdge2GridY = mGridY;
                sPathFinderEdge2X = mXpos20;
                sPathFinderEdge2Y = mYpos21;
                sPathFinderEdge2Z = mZpos22;
            }
        }
        else
        {
            sPathFinderEdge1GridX = mGridX;
            sPathFinderEdge1GridY = mGridY;
            sPathFinderEdge1X = mXpos20;
            sPathFinderEdge1Y = mYpos21;
            sPathFinderEdge1Z = mZpos22;
        }

        if (mGridX)
        {
            if (mGridX == 31)
            {
                sPathFinderEdge3GridX = 31;
                sPathFinderEdge3GridY = mGridY;
                sPathFinderEdge3X = mXpos20;
                sPathFinderEdge3Y = mYpos21;
                sPathFinderEdge3Z = mZpos22;
            }
        }
        else
        {
            sPathFinderEdge4GridX = 0;
            sPathFinderEdge4GridY = mGridY;
            sPathFinderEdge4X = mXpos20;
            sPathFinderEdge4Y = mYpos21;
            sPathFinderEdge4Z = mZpos22;
        }
    }
}

MATCH_FUNC(0x5548c0)
bool PathFinder::ProcessGridCell_5548C0()
{
    char_type cell_result = PathFinder::EvaluateGridCell_554640();
    if (cell_result == k_cell_open)
    {
        if (PathFinder::CanMoveToAdjacentTile_554110(mXpos20, mYpos21))
        {
            PathFinder::AddGridCell_554710();
        }
        else
        {
            PathNode* pCell = &mGrid[mGridIdx];
            if (pCell->mIdx1 == 1 && pCell->mYpos == 0)
            {
                pCell->mIdx1 = 0;
            }
        }
    }
    else if (cell_result == k_cell_edge_reached)
    {
        mbSearching = false;
        mUnused19 = 1;
        mUnused1A = 1;
        return true;
    }
    return false;
}

MATCH_FUNC(0x554920)
void PathFinder::RestoreSavedPosition_554920()
{
    switch (sPathFinderAngleFace)
    {
        case path_direction::up_1:
            this->mXpos25 = sPathFinderEdge1X;
            sPathFinderBestX = sPathFinderEdge1X;
            this->mYpos26 = sPathFinderEdge1Y;
            sPathFinderBestY = sPathFinderEdge1Y;
            this->mZpos27 = sPathFinderEdge1Z;
            sPathFinderBestZ = sPathFinderEdge1Z;
            sPathFinderBestGridX = sPathFinderEdge1GridX;
            sPathFinderBestGridY = sPathFinderEdge1GridY;
            break;
        case path_direction::down_2:
            this->mXpos25 = sPathFinderEdge2X;
            sPathFinderBestX = sPathFinderEdge2X;
            this->mYpos26 = sPathFinderEdge2Y;
            sPathFinderBestY = sPathFinderEdge2Y;
            this->mZpos27 = sPathFinderEdge2Z;
            sPathFinderBestZ = sPathFinderEdge2Z;
            sPathFinderBestGridX = sPathFinderEdge3GridX;
            sPathFinderBestGridY = sPathFinderEdge3GridY;
            break;
        case path_direction::right_3:
            this->mXpos25 = sPathFinderEdge3X;
            sPathFinderBestX = sPathFinderEdge3X;
            this->mYpos26 = sPathFinderEdge3Y;
            sPathFinderBestY = sPathFinderEdge3Y;
            this->mZpos27 = sPathFinderEdge3Z;
            sPathFinderBestZ = sPathFinderEdge3Z;
            sPathFinderBestGridX = sPathFinderEdge3GridX;
            sPathFinderBestGridY = sPathFinderEdge3GridY;
            break;
        case path_direction::left_4:
            this->mXpos25 = sPathFinderEdge4X;
            sPathFinderBestX = sPathFinderEdge4X;
            this->mYpos26 = sPathFinderEdge4Y;
            sPathFinderBestY = sPathFinderEdge4Y;
            this->mZpos27 = sPathFinderEdge4Z;
            sPathFinderBestZ = sPathFinderEdge4Z;
            sPathFinderBestGridX = sPathFinderEdge4GridX;
            sPathFinderBestGridY = sPathFinderEdge4GridY;
            break;
        default:
            return;
    }
}

MATCH_FUNC(0x554a90)
s32 PathFinder::IsFirstInPathQueue_554A90(Ped* pPed) const
{
    return mPedList.mpFirstPed->mCharPed == pPed;
}

// https://decomp.me/scratch/Fr0bT
MATCH_FUNC(0x554ab0)
bool PathFinder::ComputePath_554AB0(s32 ped_id,
                                        Ped* pPed,
                                        u8 x_start,
                                        u8 y_start,
                                        u8 z_start,
                                        u8 x_end,
                                        u8 y_end,
                                        u8 z_end,
                                        s32 angle_face,
                                        u8* pOutPathPointCount)
{
    PathNode* pBestNode;
    PathNode* pCell;
    u8 cur_y;
    u8 idx1;
    u8 idx2;
    u8 cur_x;
    u8 cur_z;
    u8 new_z; // only stored to cur_z on the paths that add a node
    u8 i;

    PatrolPoint* pEndPoint;
    u16 j;

    mIterationBudget = 100;
    mPedId = ped_id;
    sPathFinderEdge1GridX = 0;
    sPathFinderEdge1GridY = 0;
    sPathFinderEdge1X = 0;
    sPathFinderEdge1Y = 0;
    sPathFinderEdge1Z = 0;
    sPathFinderEdge2GridX = 0;
    sPathFinderEdge2GridY = 0;
    sPathFinderEdge2X = 0;
    sPathFinderEdge2Y = 0;
    sPathFinderEdge2Z = 0;
    sPathFinderEdge3GridX = 0;
    sPathFinderEdge3GridY = 0;
    sPathFinderEdge3X = 0;
    sPathFinderEdge3Y = 0;
    sPathFinderEdge3Z = 0;
    sPathFinderEdge4GridX = 0;
    sPathFinderEdge4GridY = 0;
    sPathFinderEdge4X = 0;
    sPathFinderEdge4Y = 0;
    sPathFinderEdge4Z = 0;
    if (mbTimedOut)
    {
        mXStart = x_start;
        mYStart = y_start;
        mZStart = z_start;

        mXEnd = x_end;
        mYEnd = y_end;
        mZEnd = z_end;

        mTimeOutCounter = 0;
        sPathFinderAngleFace = angle_face;
        mbFindTileMode = false;
        PathFinder::ClearGrid_5545C0();

        if (!gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(x_start, y_start, z_start))
        {
            mZStart = gMap_0x370_6F6268->FindGroundZBelowCoord_4E4D40(Fix16(x_start), Fix16(y_start), Fix16(z_start)).ToUInt8();
        }
        sPathFinderSlopeZDelta = 0;
        mXpos20 = x_start;
        mNodeCount = 0;
        mpNode = mNodes;
        mYpos21 = y_start;
        mZpos22 = z_start;
        mGridX = 16;
        mGridY = 16;
        mDirection = 66;
        PathFinder::AddGridCell_554710();
        mpNode = mNodes;
        mGridIdx = 560;
        mGrid[mGridIdx].mCost = 0;
        mbTimedOut = false;
        mCurrentCost = 0;
        mbSearching = true;
    }
    ++mTimeOutCounter;
    if (mTimeOutCounter > 200)
    {
        mbTimedOut = true;
        mbComputePathInProgress = false;
        return true;
    }
    if (mNodeCount)
    {
        while (1)
        {
            mbComputePathInProgress = true;
            mpNode = mNodes;
            pBestNode = mNodes;

            for (j = 0; j < mNodeCount - 1; j++)
            {
                ++mpNode;
                if (mpNode->mCost < pBestNode->mCost)
                {
                    pBestNode = mpNode;
                }
            }

            sPathFinderBestGridX = pBestNode->mIdx1;
            sPathFinderBestGridY = pBestNode->mIdx2;
            sPathFinderBestX = pBestNode->mXpos;
            sPathFinderBestY = pBestNode->mYpos;
            sPathFinderBestZ = pBestNode->mZpos;
            sPathFinderBestCost = pBestNode->mCost;
            mGridIdx = sPathFinderBestGridX + 34 * sPathFinderBestGridY;
            mCurrentCost = mGrid[mGridIdx].mCost;
            pBestNode->mIdx1 = mpNode->mIdx1;
            pBestNode->mIdx2 = mpNode->mIdx2;
            pBestNode->mXpos = mpNode->mXpos;
            pBestNode->mYpos = mpNode->mYpos;
            pBestNode->mZpos = mpNode->mZpos;
            pBestNode->mCost = mpNode->mCost;
            --mNodeCount;
            mXpos25 = sPathFinderBestX;
            mYpos26 = sPathFinderBestY;
            mZpos27 = sPathFinderBestZ;
            if (mXpos25 == mXEnd && mYpos26 == mYEnd && mZpos27 == mZEnd)
            {
                mbSearching = false;
                mUnused19 = 1;
                mUnused1A = 1;
                break;
            }
            mXpos20 = sPathFinderBestX;
            mYpos21 = sPathFinderBestY - 1;
            mZpos22 = sPathFinderBestZ;
            mGridX = sPathFinderBestGridX;
            mGridY = sPathFinderBestGridY - 1;
            mDirection = 1;

            if (PathFinder::ProcessGridCell_5548C0())
            {
                break;
            }
            mXpos20 = sPathFinderBestX + 1;
            mYpos21 = sPathFinderBestY - 1;
            mZpos22 = sPathFinderBestZ;
            mGridX = sPathFinderBestGridX + 1;
            mGridY = sPathFinderBestGridY - 1;
            mDirection = 5;

            if (PathFinder::ProcessGridCell_5548C0())
            {
                break;
            }
            mXpos20 = sPathFinderBestX + 1;
            mYpos21 = sPathFinderBestY;
            mZpos22 = sPathFinderBestZ;
            mGridX = sPathFinderBestGridX + 1;
            mGridY = sPathFinderBestGridY;
            mDirection = 2;

            if (PathFinder::ProcessGridCell_5548C0())
            {
                break;
            }
            mXpos20 = sPathFinderBestX + 1;
            mYpos21 = sPathFinderBestY + 1;
            mZpos22 = sPathFinderBestZ;
            mGridX = sPathFinderBestGridX + 1;
            mGridY = sPathFinderBestGridY + 1;
            mDirection = 6;

            if (PathFinder::ProcessGridCell_5548C0())
            {
                break;
            }
            mXpos20 = sPathFinderBestX;
            mYpos21 = sPathFinderBestY + 1;
            mZpos22 = sPathFinderBestZ;
            mGridX = sPathFinderBestGridX;
            mGridY = sPathFinderBestGridY + 1;
            mDirection = 3;
            if (PathFinder::ProcessGridCell_5548C0())
            {
                break;
            }
            mXpos20 = sPathFinderBestX - 1;
            mYpos21 = sPathFinderBestY + 1;
            mZpos22 = sPathFinderBestZ;
            mGridX = sPathFinderBestGridX - 1;
            mGridY = sPathFinderBestGridY + 1;
            mDirection = 7;
            if (PathFinder::ProcessGridCell_5548C0())
            {
                break;
            }
            mXpos20 = sPathFinderBestX - 1;
            mYpos21 = sPathFinderBestY;
            mZpos22 = sPathFinderBestZ;
            mGridX = sPathFinderBestGridX - 1;
            mGridY = sPathFinderBestGridY;
            mDirection = 4;
            if (PathFinder::ProcessGridCell_5548C0())
            {
                break;
            }
            mXpos20 = sPathFinderBestX - 1;
            mYpos21 = sPathFinderBestY - 1;
            mZpos22 = sPathFinderBestZ;
            mGridX = sPathFinderBestGridX - 1;
            mGridY = sPathFinderBestGridY - 1;
            mDirection = 8;

            if (PathFinder::ProcessGridCell_5548C0())
            {
                break;
            }
            if (!mNodeCount)
            {
                mbSearching = false;
                mUnused19 = 1;
                PathFinder::RestoreSavedPosition_554920();
                if (!sPathFinderBestX)
                {
                    goto fail;
                }
            }
            else
            {
                mbExpanding = true;
                if (mIterationBudget == 0)
                {
                    return false;
                }
            }
            if (mIterationBudget > 0)
            {
                --mIterationBudget;
            }
            if (!mbSearching)
            {
                break;
            }
        }
    }

    mbExpanding = false;
    cur_x = mXpos25;
    cur_y = mYpos26;
    cur_z = mZpos27;
    idx1 = sPathFinderBestGridX;
    idx2 = sPathFinderBestGridY;
    mpNode = mNodes;
    mpNode->mXpos = cur_x;
    mpNode->mYpos = cur_y;
    mpNode->mZpos = cur_z;
    mNodeCount = 1;
    pCell = &mGrid[static_cast<s16>(idx1 + 34 * idx2)];
    while (2)
    {
        if (pCell->mIdx1 == 1)
        {
            if (abs(cur_z - pCell->mZpos) < 1)
            {
                mDirection = pCell->mYpos;
                new_z = pCell->mZpos;
                pCell->mIdx1 = 0;
            }
            else
            {
                mDirection = pCell->mIdx2;
                u8 second_direction = pCell->mYpos;
                new_z = pCell->mXpos;
                pCell->mIdx2 = second_direction;
                pCell->mIdx1 = 0;
                pCell->mXpos = pCell->mZpos;
            }
        }
        else
        {
            mDirection = pCell->mIdx2;
            new_z = pCell->mXpos;
            pCell->mIdx2 = 0;
        }
        switch (mDirection)
        {
            case 1:
                pCell += 34;
                ++cur_y;
                break;
            case 3:
                pCell -= 34;
                --cur_y;
                break;
            case 2:
                --pCell;
                cur_x--;
                break;
            case 4:
                // ++cur_x written out in cases 4, 7 and 8 (VC6 tail-merges them after register allocation);
                // a shared ++cur_x label gives new_z 2 more regalloc priority than the switch temp, so new_z
                // took eax (al) instead of edx (dl)
                ++pCell;
                ++cur_x;
                break;
            case 5:
                ++cur_y;
                pCell += 33;
                cur_x--;
                break;
            case 6:
                --cur_y;
                pCell -= 35;
                cur_x--;
                break;
            case 7:
                --cur_y;
                pCell -= 33;
                ++cur_x;
                break;
            case 8:
                ++cur_y;
                pCell += 35;
                ++cur_x;
                break;
            case 66:
                if (mNodeCount > 1)
                {
                    --mpNode;
                }
                if (mNodeCount > 100)
                {
                    mNodeCount = 100;
                }

                for (i = 0; i < mNodeCount; i++)
                {
                    PatrolPoint* pPatrolPoint = &pPed->mPatrolPoints[i];
                    pPatrolPoint->mX = mpNode->mXpos;
                    pPatrolPoint->mY = mpNode->mYpos;
                    pPatrolPoint->mZ = mpNode->mZpos;
                    --mpNode;
                }
                pEndPoint = &pPed->mPatrolPoints[i];
                pEndPoint->mX = 0;
                pEndPoint->mY = 0;
                pEndPoint->mZ = 0;
                *pOutPathPointCount = static_cast<u8>(mNodeCount);
                mbTimedOut = true;
                mbComputePathInProgress = false;
                mTimeOutCounter = 0;
                return true;
            default:
                // Shared failure block with the "no more nodes" path: goto-free forms change the code
                goto fail;
        }
        mpNode++;
        cur_z = new_z;
        mpNode->mXpos = cur_x;
        mpNode->mYpos = cur_y;
        mpNode->mZpos = cur_z;
        ++mNodeCount;
    }
fail:
    PathFinder::RemovePed_554620(mPedId);
    return true;
}

// https://decomp.me/scratch/19LIh
MATCH_FUNC(0x5552b0)
bool PathFinder::FindNearbyTileMatchingSlopeType_5552B0(u8 block_type, u8* xpos, u8* ypos, u8* zpos, bool bAllowDuringPathCompute)
{
    u8 expanded_count = 0;
    if (mbComputePathInProgress && !bAllowDuringPathCompute)
    {
        return false;
    }
    if (*zpos > 6u || *zpos == 0)
    {
        return false;
    }
    u8 default_pos = kFpZero_6FDD98.ToUInt8();
    mZpos27 = default_pos;
    mYpos26 = default_pos;
    mXpos25 = default_pos;
    mGridOriginX = *xpos - 16;
    mGridOriginY = *ypos - 16;
    PathFinder::ClearGrid_5545C0();
    mXStart = *xpos;
    mYStart = *ypos;
    mZStart = *zpos;
    mbFindTileMode = true;

    if (!gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(mXStart, mYStart, mZStart))
    {
        mZStart = gMap_0x370_6F6268->FindGroundZBelowCoord_4E4D40(Fix16(mXStart), Fix16(mYStart), Fix16(mZStart)).ToUInt8();
    }
    mStartCost = 0;
    mXEnd = 0;
    mYEnd = 0;
    mZEnd = 0;
    sPathFinderSlopeZDelta = 0;
    mNodeCount = 0;
    mpNode = mNodes;
    mZpos22 = mZStart;
    mGridY = 16;
    mGridX = 16;
    mXpos20 = mXStart;
    mYpos21 = mYStart;
    mDirection = 66;
    PathFinder::AddGridCell_554710();
    mpNode = mNodes;
    mGridIdx = 560;
    mGrid[mGridIdx].mCost = mStartCost;
    mCurrentCost = 0;
    mbSearching = true;
    bool bListWasEmpty = true;
    while (mNodeCount)
    {
        bListWasEmpty = false;
        PathNode* pIter = mNodes;
        mpNode = mNodes;

        for (u8 i = 0; i < mNodeCount - 1; i++)
        {
            ++mpNode;
            if (mpNode->mCost < pIter->mCost)
            {
                pIter = mpNode;
            }
        }
        sPathFinderBestGridX = pIter->mIdx1;
        sPathFinderBestGridY = pIter->mIdx2;
        sPathFinderBestX = pIter->mXpos;
        sPathFinderBestY = pIter->mYpos;
        sPathFinderBestZ = pIter->mZpos;
        sPathFinderBestCost = pIter->mCost;
        mGridIdx = sPathFinderBestGridX + 34 * sPathFinderBestGridY;
        mCurrentCost = mGrid[mGridIdx].mCost;
        pIter->mIdx1 = mpNode->mIdx1;
        pIter->mIdx2 = mpNode->mIdx2;
        pIter->mXpos = mpNode->mXpos;
        pIter->mYpos = mpNode->mYpos;
        pIter->mZpos = mpNode->mZpos;
        pIter->mCost = mpNode->mCost;
        --mNodeCount;
        mXpos25 = sPathFinderBestX;
        mYpos26 = sPathFinderBestY;
        mZpos27 = sPathFinderBestZ;

        u8 block_below_type = gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(mXpos25, mYpos26, mZpos27 - 1);

        if (block_type == 5)
        {
            if (!gMap_0x370_6F6268->IsProbePathClear_4E5640(kFpOne64th_6FDD50,
                                               kFpQuarter_6FDC00,
                                               kFpOne64th_6FDD50,
                                               Fix16(*xpos) + kFpHalf_6FDCA8,
                                               Fix16(*ypos) + kFpHalf_6FDCA8,
                                               Fix16(*zpos),
                                               Fix16(mXpos25) + kFpHalf_6FDCA8,
                                               Fix16(mYpos26) + kFpHalf_6FDCA8,
                                               Fix16(mZpos27)))
            {
                mbSearching = false;
                *xpos = mXpos25;
                *ypos = mYpos26;
                *zpos = mZpos27;
            }
        }
        else if (block_below_type == block_type)
        {
            if (block_type == ROAD)
            {
                if ((gMap_0x370_6F6268->GetBlock_4DFE10(mXpos25, mYpos26, mZpos27 - 1)->mArrows & block_arrow::green_mask) != 0)
                {
                    mbSearching = false;
                    *xpos = mXpos25;
                    *ypos = mYpos26;
                    *zpos = mZpos27;
                    return true;
                }
            }
            else
            {
                mbSearching = false;
                *xpos = mXpos25;
                *ypos = mYpos26;
                *zpos = mZpos27;
            }
        }
        mXpos20 = sPathFinderBestX;
        mYpos21 = sPathFinderBestY - 1;
        mZpos22 = sPathFinderBestZ;
        mGridX = sPathFinderBestGridX;
        mGridY = sPathFinderBestGridY - 1;
        mDirection = 1;
        if (!PathFinder::ProcessGridCell_5548C0())
        {
            mXpos20 = sPathFinderBestX + 1;
            mYpos21 = sPathFinderBestY;
            mZpos22 = sPathFinderBestZ;
            mGridX = sPathFinderBestGridX + 1;
            mGridY = sPathFinderBestGridY;
            mDirection = 2;
            if (!PathFinder::ProcessGridCell_5548C0())
            {
                mXpos20 = sPathFinderBestX;
                mYpos21 = sPathFinderBestY + 1;
                mZpos22 = sPathFinderBestZ;
                mGridX = sPathFinderBestGridX;
                mGridY = sPathFinderBestGridY + 1;
                mDirection = 3;
                if (!PathFinder::ProcessGridCell_5548C0())
                {
                    mXpos20 = sPathFinderBestX - 1;
                    mYpos21 = sPathFinderBestY;
                    mZpos22 = sPathFinderBestZ;
                    mGridX = sPathFinderBestGridX - 1;
                    mGridY = sPathFinderBestGridY;
                    mDirection = 4;
                    if (!PathFinder::ProcessGridCell_5548C0())
                    {
                        ++mStartCost;
                        if (++expanded_count <= 6 || bAllowDuringPathCompute)
                        {
                            if (!mbSearching)
                            {
                                return true;
                            }
                            continue;
                        }
                    }
                }
            }
        }
        return false;
    }
    if (bListWasEmpty)
    {
        return true;
    }
    return false;
}
