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
    if (field_25_xpos > 1u && field_25_xpos < 254u && field_26_ypos > 1u && field_26_ypos < 254u)
    {
        bool result =
            gMap_0x370_6F6268->IsMoveOntoSlopeTileBlocked_4E0130(field_25_xpos, field_26_ypos, field_27_zpos, path_direction, &sPathFinderSlopeZDelta, true) ==
            0;
        return result;
    }
    return false;
}

MATCH_FUNC(0x5540e0)
bool PathFinder::TestMoveToAdjacentTile_5540E0(u8 curr_xpos, u8 curr_ypos, u8 curr_zpos, u8 desired_xpos, u8 desired_ypos)
{
    field_25_xpos = curr_xpos;
    field_26_ypos = curr_ypos;
    field_27_zpos = curr_zpos;
    return CanMoveToAdjacentTile_554110(desired_xpos, desired_ypos);
}

MATCH_FUNC(0x554110)
bool PathFinder::CanMoveToAdjacentTile_554110(u8 desired_xpos, u8 desired_ypos)
{

    sPathFinderSlopeZDelta = 0;

    const char_type xd = desired_xpos - field_25_xpos;
    const char_type yd = desired_ypos - field_26_ypos;

    if (xd == 0 && yd == 0)
    {
        return true; // there is no moving: of course, it's allowed
    }

    if (xd != 0 && yd != 0)
    {
        // No diagonal move from a slope tile
        if (gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(field_25_xpos, field_26_ypos, field_27_zpos))
        {
            return false;
        }

        if (xd == 1)
        {
            if (yd == 1)
            {
                // moving southeast
                if (gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(this->field_25_xpos, this->field_26_ypos + 1, this->field_27_zpos))
                {
                    return false;
                }

                if (gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(this->field_25_xpos + 1, this->field_26_ypos, this->field_27_zpos))
                {
                    return false;
                }

                if (CanMoveInDirection_554080(path_direction::down_2) && CanMoveInDirection_554080(path_direction::right_3))
                {
                    if (gMap_0x370_6F6268->IsMoveOntoSlopeTileBlocked_4E0130(this->field_25_xpos + 1,
                                                                       this->field_26_ypos,
                                                                       this->field_27_zpos,
                                                                       path_direction::down_2,
                                                                       &sPathFinderSlopeZDelta,
                                                                       1))
                    {
                        return false;
                    }
                    return gMap_0x370_6F6268->IsMoveOntoSlopeTileBlocked_4E0130(this->field_25_xpos,
                                                                              this->field_26_ypos + 1,
                                                                              this->field_27_zpos,
                                                                              path_direction::right_3,
                                                                              &sPathFinderSlopeZDelta,
                                                                              1) ? false : true;
                }
                return false;
            }

            if (gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(this->field_25_xpos, this->field_26_ypos - 1, this->field_27_zpos))
            {
                return false;
            }

            if (gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(this->field_25_xpos + 1, this->field_26_ypos, this->field_27_zpos))
            {
                return false;
            }

            if (!CanMoveInDirection_554080(path_direction::up_1) || !CanMoveInDirection_554080(path_direction::right_3))
            {
                return false;
            }

            if (gMap_0x370_6F6268
                    ->IsMoveOntoSlopeTileBlocked_4E0130(this->field_25_xpos + 1, this->field_26_ypos, this->field_27_zpos, path_direction::up_1, &sPathFinderSlopeZDelta, 1))
            {
                return false;
            }
            return gMap_0x370_6F6268->IsMoveOntoSlopeTileBlocked_4E0130(this->field_25_xpos,
                                                                      this->field_26_ypos - 1,
                                                                      this->field_27_zpos,
                                                                      path_direction::right_3,
                                                                      &sPathFinderSlopeZDelta,
                                                                      1) ? false : true;
        }

        if (yd == 1)
        {
            if (gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(this->field_25_xpos, this->field_26_ypos + 1, this->field_27_zpos))
            {
                return false;
            }

            if (gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(this->field_25_xpos - 1, this->field_26_ypos, this->field_27_zpos))
            {
                return false;
            }

            if (!CanMoveInDirection_554080(path_direction::down_2) || !CanMoveInDirection_554080(path_direction::left_4))
            {
                return false;
            }

            if (gMap_0x370_6F6268
                    ->IsMoveOntoSlopeTileBlocked_4E0130(this->field_25_xpos - 1, this->field_26_ypos, this->field_27_zpos, path_direction::down_2, &sPathFinderSlopeZDelta, 1))
            {
                return false;
            }
            return gMap_0x370_6F6268
                       ->IsMoveOntoSlopeTileBlocked_4E0130(this->field_25_xpos, this->field_26_ypos + 1, this->field_27_zpos, path_direction::left_4, &sPathFinderSlopeZDelta, 1) ? false : true;
        }

        if (gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(this->field_25_xpos, this->field_26_ypos - 1, this->field_27_zpos))
        {
            return false;
        }

        if (gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(this->field_25_xpos - 1, this->field_26_ypos, this->field_27_zpos))
        {
            return false;
        }

        if (!CanMoveInDirection_554080(path_direction::up_1) || !CanMoveInDirection_554080(path_direction::left_4))
        {
            return false;
        }

        if (gMap_0x370_6F6268
                ->IsMoveOntoSlopeTileBlocked_4E0130(this->field_25_xpos - 1, this->field_26_ypos, this->field_27_zpos, path_direction::up_1, &sPathFinderSlopeZDelta, 1))
        {
            return false;
        }
        return gMap_0x370_6F6268->IsMoveOntoSlopeTileBlocked_4E0130(this->field_25_xpos,
                                                                  this->field_26_ypos - 1,
                                                                  this->field_27_zpos,
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
    memset(this->field_40_grid, 0, sizeof(this->field_40_grid));
}

MATCH_FUNC(0x5545e0)
void PathFinder::Init_5545E0()
{
    memset(this->field_40_grid, 0, sizeof(this->field_40_grid));
    field_38_bComputePathInProgress = false;
    field_34_unused = 1;
    field_36_unused = 0;
    field_3A_unused = 0;
    field_2FD0_bTimedOut = true;
    field_2FD1_time_out_counter = 0;
}

MATCH_FUNC(0x554620)
void PathFinder::RemovePed_554620(s32 ped_id)
{
    if (ped_id == field_0_ped_id)
    {
        field_2FD0_bTimedOut = true;
        field_2FD1_time_out_counter = 0;
    }
}

MATCH_FUNC(0x554640)
char_type PathFinder::EvaluateGridCell_554640()
{
    if (field_23_grid_x <= 32)
    {
        if (field_24_grid_y <= 32)
        {
            if (field_22_zpos <= 8)
            {
                field_1C_grid_idx = field_23_grid_x + 34 * field_24_grid_y;
                PathNode* pNode = &field_40_grid[field_1C_grid_idx];
                if (pNode->field_1_idx2 == 0)
                {
                    return k_cell_open;
                }
                if (pNode->field_0_idx1 != 1 && pNode->field_2_xpos != field_22_zpos)
                {
                    if (abs(pNode->field_2_xpos - field_22_zpos) >= 1)
                    {
                        pNode->field_0_idx1 = 1;
                        return k_cell_open;
                    }
                }
                return k_cell_skip;
            }
        }
    }
    if (field_10_zStart != field_13_zEnd)
    {
        return k_cell_skip;
    }
    switch (sPathFinderAngleFace)
    {
        case path_direction::up_1:
            if (field_24_grid_y < 1)
            {
                return k_cell_edge_reached;
            }
            break;
        case path_direction::down_2:
            if (field_24_grid_y > 32)
            {
                return k_cell_edge_reached;
            }
            break;
        case path_direction::right_3:
            if (field_23_grid_x > 32)
            {
                return k_cell_edge_reached;
            }
            break;
        case path_direction::left_4:
            if (field_23_grid_x < 1)
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
    // zpos assigned in both arms (not `zpos = field_22_zpos; if (..) zpos += ..;`): the extra
    // definition raises its priority above dx/dy so it is coloured first and keeps edx.
    u8 zpos;
    if (sPathFinderSlopeZDelta)
    {
        zpos = field_22_zpos + sPathFinderSlopeZDelta;
    }
    else
    {
        zpos = field_22_zpos;
    }

    if (zpos != field_13_zEnd)
    {
        cost_scale = 2;
    }
    else
    {
        cost_scale = 1;
    }

    field_1C_grid_idx = field_23_grid_x + 34 * field_24_grid_y;
    PathNode* pNode = &field_40_grid[field_1C_grid_idx];
    if (field_40_grid[field_1C_grid_idx].field_0_idx1 == 1 && zpos == pNode->field_2_xpos)
    {
        pNode->field_0_idx1 = 0;
    }
    else
    {
        u16 heuristic;
        if (!field_4_bFindTileMode)
        {
            // The sum assigned first and `*= cost_scale` after (one expression keeps cost_scale's multiply in the sum's
            // register and pushes ebp at the top instead of inside this branch). The dx/dy locals give
            // the original's evaluation order (x difference first).
            u16 dx = field_20_xpos - field_11_xEnd;
            u16 dy = field_21_ypos - field_12_yEnd;
            heuristic = dy * dy + dx * dx;
            heuristic *= cost_scale;
        }
        else
        {
            heuristic = field_16_start_cost;
        }
        field_8_pNode->field_0_idx1 = field_23_grid_x;
        field_8_pNode->field_1_idx2 = field_24_grid_y;
        field_8_pNode->field_2_xpos = field_20_xpos;
        field_8_pNode->field_3_ypos = field_21_ypos;
        field_8_pNode->field_4_zpos = zpos;
        field_8_pNode->field_6_cost = heuristic;
        ++field_8_pNode;
        ++field_C_node_count;

        if (pNode->field_0_idx1 == 1)
        {
            pNode->field_3_ypos = field_1B_direction;
            pNode->field_4_zpos = zpos;
        }
        else
        {
            pNode->field_1_idx2 = field_1B_direction;
            pNode->field_2_xpos = zpos;
        }
        pNode->field_6_cost = field_1E_current_cost + 1;

        if (field_24_grid_y)
        {
            if (field_24_grid_y == 31)
            {
                sPathFinderEdge2GridX = field_23_grid_x;
                sPathFinderEdge2GridY = field_24_grid_y;
                sPathFinderEdge2X = field_20_xpos;
                sPathFinderEdge2Y = field_21_ypos;
                sPathFinderEdge2Z = field_22_zpos;
            }
        }
        else
        {
            sPathFinderEdge1GridX = field_23_grid_x;
            sPathFinderEdge1GridY = field_24_grid_y;
            sPathFinderEdge1X = field_20_xpos;
            sPathFinderEdge1Y = field_21_ypos;
            sPathFinderEdge1Z = field_22_zpos;
        }

        if (field_23_grid_x)
        {
            if (field_23_grid_x == 31)
            {
                sPathFinderEdge3GridX = 31;
                sPathFinderEdge3GridY = field_24_grid_y;
                sPathFinderEdge3X = field_20_xpos;
                sPathFinderEdge3Y = field_21_ypos;
                sPathFinderEdge3Z = field_22_zpos;
            }
        }
        else
        {
            sPathFinderEdge4GridX = 0;
            sPathFinderEdge4GridY = field_24_grid_y;
            sPathFinderEdge4X = field_20_xpos;
            sPathFinderEdge4Y = field_21_ypos;
            sPathFinderEdge4Z = field_22_zpos;
        }
    }
}

MATCH_FUNC(0x5548c0)
bool PathFinder::ProcessGridCell_5548C0()
{
    char_type cell_result = PathFinder::EvaluateGridCell_554640();
    if (cell_result == k_cell_open)
    {
        if (PathFinder::CanMoveToAdjacentTile_554110(field_20_xpos, field_21_ypos))
        {
            PathFinder::AddGridCell_554710();
        }
        else
        {
            PathNode* pCell = &field_40_grid[field_1C_grid_idx];
            if (pCell->field_0_idx1 == 1 && pCell->field_3_ypos == 0)
            {
                pCell->field_0_idx1 = 0;
            }
        }
    }
    else if (cell_result == k_cell_edge_reached)
    {
        field_18_bSearching = false;
        field_19_unused = 1;
        field_1A_unused = 1;
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
            this->field_25_xpos = sPathFinderEdge1X;
            sPathFinderBestX = sPathFinderEdge1X;
            this->field_26_ypos = sPathFinderEdge1Y;
            sPathFinderBestY = sPathFinderEdge1Y;
            this->field_27_zpos = sPathFinderEdge1Z;
            sPathFinderBestZ = sPathFinderEdge1Z;
            sPathFinderBestGridX = sPathFinderEdge1GridX;
            sPathFinderBestGridY = sPathFinderEdge1GridY;
            break;
        case path_direction::down_2:
            this->field_25_xpos = sPathFinderEdge2X;
            sPathFinderBestX = sPathFinderEdge2X;
            this->field_26_ypos = sPathFinderEdge2Y;
            sPathFinderBestY = sPathFinderEdge2Y;
            this->field_27_zpos = sPathFinderEdge2Z;
            sPathFinderBestZ = sPathFinderEdge2Z;
            sPathFinderBestGridX = sPathFinderEdge3GridX;
            sPathFinderBestGridY = sPathFinderEdge3GridY;
            break;
        case path_direction::right_3:
            this->field_25_xpos = sPathFinderEdge3X;
            sPathFinderBestX = sPathFinderEdge3X;
            this->field_26_ypos = sPathFinderEdge3Y;
            sPathFinderBestY = sPathFinderEdge3Y;
            this->field_27_zpos = sPathFinderEdge3Z;
            sPathFinderBestZ = sPathFinderEdge3Z;
            sPathFinderBestGridX = sPathFinderEdge3GridX;
            sPathFinderBestGridY = sPathFinderEdge3GridY;
            break;
        case path_direction::left_4:
            this->field_25_xpos = sPathFinderEdge4X;
            sPathFinderBestX = sPathFinderEdge4X;
            this->field_26_ypos = sPathFinderEdge4Y;
            sPathFinderBestY = sPathFinderEdge4Y;
            this->field_27_zpos = sPathFinderEdge4Z;
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
    return field_3C_ped_list.field_0_pFirstPed->field_0_char_ped == pPed;
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

    field_2E_iteration_budget = 100;
    field_0_ped_id = ped_id;
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
    if (field_2FD0_bTimedOut)
    {
        field_E_xStart = x_start;
        field_F_yStart = y_start;
        field_10_zStart = z_start;

        field_11_xEnd = x_end;
        field_12_yEnd = y_end;
        field_13_zEnd = z_end;

        field_2FD1_time_out_counter = 0;
        sPathFinderAngleFace = angle_face;
        field_4_bFindTileMode = false;
        PathFinder::ClearGrid_5545C0();

        if (!gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(x_start, y_start, z_start))
        {
            field_10_zStart = gMap_0x370_6F6268->FindGroundZBelowCoord_4E4D40(Fix16(x_start), Fix16(y_start), Fix16(z_start)).ToUInt8();
        }
        sPathFinderSlopeZDelta = 0;
        field_20_xpos = x_start;
        field_C_node_count = 0;
        field_8_pNode = field_2350_nodes;
        field_21_ypos = y_start;
        field_22_zpos = z_start;
        field_23_grid_x = 16;
        field_24_grid_y = 16;
        field_1B_direction = 66;
        PathFinder::AddGridCell_554710();
        field_8_pNode = field_2350_nodes;
        field_1C_grid_idx = 560;
        field_40_grid[field_1C_grid_idx].field_6_cost = 0;
        field_2FD0_bTimedOut = false;
        field_1E_current_cost = 0;
        field_18_bSearching = true;
    }
    ++field_2FD1_time_out_counter;
    if (field_2FD1_time_out_counter > 200)
    {
        field_2FD0_bTimedOut = true;
        field_38_bComputePathInProgress = false;
        return true;
    }
    if (field_C_node_count)
    {
        while (1)
        {
            field_38_bComputePathInProgress = true;
            field_8_pNode = field_2350_nodes;
            pBestNode = field_2350_nodes;

            for (j = 0; j < field_C_node_count - 1; j++)
            {
                ++field_8_pNode;
                if (field_8_pNode->field_6_cost < pBestNode->field_6_cost)
                {
                    pBestNode = field_8_pNode;
                }
            }

            sPathFinderBestGridX = pBestNode->field_0_idx1;
            sPathFinderBestGridY = pBestNode->field_1_idx2;
            sPathFinderBestX = pBestNode->field_2_xpos;
            sPathFinderBestY = pBestNode->field_3_ypos;
            sPathFinderBestZ = pBestNode->field_4_zpos;
            sPathFinderBestCost = pBestNode->field_6_cost;
            field_1C_grid_idx = sPathFinderBestGridX + 34 * sPathFinderBestGridY;
            field_1E_current_cost = field_40_grid[field_1C_grid_idx].field_6_cost;
            pBestNode->field_0_idx1 = field_8_pNode->field_0_idx1;
            pBestNode->field_1_idx2 = field_8_pNode->field_1_idx2;
            pBestNode->field_2_xpos = field_8_pNode->field_2_xpos;
            pBestNode->field_3_ypos = field_8_pNode->field_3_ypos;
            pBestNode->field_4_zpos = field_8_pNode->field_4_zpos;
            pBestNode->field_6_cost = field_8_pNode->field_6_cost;
            --field_C_node_count;
            field_25_xpos = sPathFinderBestX;
            field_26_ypos = sPathFinderBestY;
            field_27_zpos = sPathFinderBestZ;
            if (field_25_xpos == field_11_xEnd && field_26_ypos == field_12_yEnd && field_27_zpos == field_13_zEnd)
            {
                field_18_bSearching = false;
                field_19_unused = 1;
                field_1A_unused = 1;
                break;
            }
            field_20_xpos = sPathFinderBestX;
            field_21_ypos = sPathFinderBestY - 1;
            field_22_zpos = sPathFinderBestZ;
            field_23_grid_x = sPathFinderBestGridX;
            field_24_grid_y = sPathFinderBestGridY - 1;
            field_1B_direction = 1;

            if (PathFinder::ProcessGridCell_5548C0())
            {
                break;
            }
            field_20_xpos = sPathFinderBestX + 1;
            field_21_ypos = sPathFinderBestY - 1;
            field_22_zpos = sPathFinderBestZ;
            field_23_grid_x = sPathFinderBestGridX + 1;
            field_24_grid_y = sPathFinderBestGridY - 1;
            field_1B_direction = 5;

            if (PathFinder::ProcessGridCell_5548C0())
            {
                break;
            }
            field_20_xpos = sPathFinderBestX + 1;
            field_21_ypos = sPathFinderBestY;
            field_22_zpos = sPathFinderBestZ;
            field_23_grid_x = sPathFinderBestGridX + 1;
            field_24_grid_y = sPathFinderBestGridY;
            field_1B_direction = 2;

            if (PathFinder::ProcessGridCell_5548C0())
            {
                break;
            }
            field_20_xpos = sPathFinderBestX + 1;
            field_21_ypos = sPathFinderBestY + 1;
            field_22_zpos = sPathFinderBestZ;
            field_23_grid_x = sPathFinderBestGridX + 1;
            field_24_grid_y = sPathFinderBestGridY + 1;
            field_1B_direction = 6;

            if (PathFinder::ProcessGridCell_5548C0())
            {
                break;
            }
            field_20_xpos = sPathFinderBestX;
            field_21_ypos = sPathFinderBestY + 1;
            field_22_zpos = sPathFinderBestZ;
            field_23_grid_x = sPathFinderBestGridX;
            field_24_grid_y = sPathFinderBestGridY + 1;
            field_1B_direction = 3;
            if (PathFinder::ProcessGridCell_5548C0())
            {
                break;
            }
            field_20_xpos = sPathFinderBestX - 1;
            field_21_ypos = sPathFinderBestY + 1;
            field_22_zpos = sPathFinderBestZ;
            field_23_grid_x = sPathFinderBestGridX - 1;
            field_24_grid_y = sPathFinderBestGridY + 1;
            field_1B_direction = 7;
            if (PathFinder::ProcessGridCell_5548C0())
            {
                break;
            }
            field_20_xpos = sPathFinderBestX - 1;
            field_21_ypos = sPathFinderBestY;
            field_22_zpos = sPathFinderBestZ;
            field_23_grid_x = sPathFinderBestGridX - 1;
            field_24_grid_y = sPathFinderBestGridY;
            field_1B_direction = 4;
            if (PathFinder::ProcessGridCell_5548C0())
            {
                break;
            }
            field_20_xpos = sPathFinderBestX - 1;
            field_21_ypos = sPathFinderBestY - 1;
            field_22_zpos = sPathFinderBestZ;
            field_23_grid_x = sPathFinderBestGridX - 1;
            field_24_grid_y = sPathFinderBestGridY - 1;
            field_1B_direction = 8;

            if (PathFinder::ProcessGridCell_5548C0())
            {
                break;
            }
            if (!field_C_node_count)
            {
                field_18_bSearching = false;
                field_19_unused = 1;
                PathFinder::RestoreSavedPosition_554920();
                if (!sPathFinderBestX)
                {
                    goto fail;
                }
            }
            else
            {
                field_14_bExpanding = true;
                if (field_2E_iteration_budget == 0)
                {
                    return false;
                }
            }
            if (field_2E_iteration_budget > 0)
            {
                --field_2E_iteration_budget;
            }
            if (!field_18_bSearching)
            {
                break;
            }
        }
    }

    field_14_bExpanding = false;
    cur_x = field_25_xpos;
    cur_y = field_26_ypos;
    cur_z = field_27_zpos;
    idx1 = sPathFinderBestGridX;
    idx2 = sPathFinderBestGridY;
    field_8_pNode = field_2350_nodes;
    field_8_pNode->field_2_xpos = cur_x;
    field_8_pNode->field_3_ypos = cur_y;
    field_8_pNode->field_4_zpos = cur_z;
    field_C_node_count = 1;
    pCell = &field_40_grid[static_cast<s16>(idx1 + 34 * idx2)];
    while (2)
    {
        if (pCell->field_0_idx1 == 1)
        {
            if (abs(cur_z - pCell->field_4_zpos) < 1)
            {
                field_1B_direction = pCell->field_3_ypos;
                new_z = pCell->field_4_zpos;
                pCell->field_0_idx1 = 0;
            }
            else
            {
                field_1B_direction = pCell->field_1_idx2;
                u8 second_direction = pCell->field_3_ypos;
                new_z = pCell->field_2_xpos;
                pCell->field_1_idx2 = second_direction;
                pCell->field_0_idx1 = 0;
                pCell->field_2_xpos = pCell->field_4_zpos;
            }
        }
        else
        {
            field_1B_direction = pCell->field_1_idx2;
            new_z = pCell->field_2_xpos;
            pCell->field_1_idx2 = 0;
        }
        switch (field_1B_direction)
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
                if (field_C_node_count > 1)
                {
                    --field_8_pNode;
                }
                if (field_C_node_count > 100)
                {
                    field_C_node_count = 100;
                }

                for (i = 0; i < field_C_node_count; i++)
                {
                    PatrolPoint* pPatrolPoint = &pPed->field_0_patrol_points[i];
                    pPatrolPoint->field_0_x = field_8_pNode->field_2_xpos;
                    pPatrolPoint->field_1_y = field_8_pNode->field_3_ypos;
                    pPatrolPoint->field_2_z = field_8_pNode->field_4_zpos;
                    --field_8_pNode;
                }
                pEndPoint = &pPed->field_0_patrol_points[i];
                pEndPoint->field_0_x = 0;
                pEndPoint->field_1_y = 0;
                pEndPoint->field_2_z = 0;
                *pOutPathPointCount = static_cast<u8>(field_C_node_count);
                field_2FD0_bTimedOut = true;
                field_38_bComputePathInProgress = false;
                field_2FD1_time_out_counter = 0;
                return true;
            default:
                // Shared failure block with the "no more nodes" path: goto-free forms change the code
                goto fail;
        }
        field_8_pNode++;
        cur_z = new_z;
        field_8_pNode->field_2_xpos = cur_x;
        field_8_pNode->field_3_ypos = cur_y;
        field_8_pNode->field_4_zpos = cur_z;
        ++field_C_node_count;
    }
fail:
    PathFinder::RemovePed_554620(field_0_ped_id);
    return true;
}

// https://decomp.me/scratch/19LIh
MATCH_FUNC(0x5552b0)
bool PathFinder::FindNearbyTileMatchingSlopeType_5552B0(u8 block_type, u8* xpos, u8* ypos, u8* zpos, bool bAllowDuringPathCompute)
{
    u8 expanded_count = 0;
    if (field_38_bComputePathInProgress && !bAllowDuringPathCompute)
    {
        return false;
    }
    if (*zpos > 6u || *zpos == 0)
    {
        return false;
    }
    u8 default_pos = kFpZero_6FDD98.ToUInt8();
    field_27_zpos = default_pos;
    field_26_ypos = default_pos;
    field_25_xpos = default_pos;
    field_28_grid_origin_x = *xpos - 16;
    field_29_grid_origin_y = *ypos - 16;
    PathFinder::ClearGrid_5545C0();
    field_E_xStart = *xpos;
    field_F_yStart = *ypos;
    field_10_zStart = *zpos;
    field_4_bFindTileMode = true;

    if (!gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(field_E_xStart, field_F_yStart, field_10_zStart))
    {
        field_10_zStart = gMap_0x370_6F6268->FindGroundZBelowCoord_4E4D40(Fix16(field_E_xStart), Fix16(field_F_yStart), Fix16(field_10_zStart)).ToUInt8();
    }
    field_16_start_cost = 0;
    field_11_xEnd = 0;
    field_12_yEnd = 0;
    field_13_zEnd = 0;
    sPathFinderSlopeZDelta = 0;
    field_C_node_count = 0;
    field_8_pNode = field_2350_nodes;
    field_22_zpos = field_10_zStart;
    field_24_grid_y = 16;
    field_23_grid_x = 16;
    field_20_xpos = field_E_xStart;
    field_21_ypos = field_F_yStart;
    field_1B_direction = 66;
    PathFinder::AddGridCell_554710();
    field_8_pNode = field_2350_nodes;
    field_1C_grid_idx = 560;
    field_40_grid[field_1C_grid_idx].field_6_cost = field_16_start_cost;
    field_1E_current_cost = 0;
    field_18_bSearching = true;
    bool bListWasEmpty = true;
    while (field_C_node_count)
    {
        bListWasEmpty = false;
        PathNode* pIter = field_2350_nodes;
        field_8_pNode = field_2350_nodes;

        for (u8 i = 0; i < field_C_node_count - 1; i++)
        {
            ++field_8_pNode;
            if (field_8_pNode->field_6_cost < pIter->field_6_cost)
            {
                pIter = field_8_pNode;
            }
        }
        sPathFinderBestGridX = pIter->field_0_idx1;
        sPathFinderBestGridY = pIter->field_1_idx2;
        sPathFinderBestX = pIter->field_2_xpos;
        sPathFinderBestY = pIter->field_3_ypos;
        sPathFinderBestZ = pIter->field_4_zpos;
        sPathFinderBestCost = pIter->field_6_cost;
        field_1C_grid_idx = sPathFinderBestGridX + 34 * sPathFinderBestGridY;
        field_1E_current_cost = field_40_grid[field_1C_grid_idx].field_6_cost;
        pIter->field_0_idx1 = field_8_pNode->field_0_idx1;
        pIter->field_1_idx2 = field_8_pNode->field_1_idx2;
        pIter->field_2_xpos = field_8_pNode->field_2_xpos;
        pIter->field_3_ypos = field_8_pNode->field_3_ypos;
        pIter->field_4_zpos = field_8_pNode->field_4_zpos;
        pIter->field_6_cost = field_8_pNode->field_6_cost;
        --field_C_node_count;
        field_25_xpos = sPathFinderBestX;
        field_26_ypos = sPathFinderBestY;
        field_27_zpos = sPathFinderBestZ;

        u8 block_below_type = gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(field_25_xpos, field_26_ypos, field_27_zpos - 1);

        if (block_type == 5)
        {
            if (!gMap_0x370_6F6268->IsProbePathClear_4E5640(kFpOne64th_6FDD50,
                                               kFpQuarter_6FDC00,
                                               kFpOne64th_6FDD50,
                                               Fix16(*xpos) + kFpHalf_6FDCA8,
                                               Fix16(*ypos) + kFpHalf_6FDCA8,
                                               Fix16(*zpos),
                                               Fix16(field_25_xpos) + kFpHalf_6FDCA8,
                                               Fix16(field_26_ypos) + kFpHalf_6FDCA8,
                                               Fix16(field_27_zpos)))
            {
                field_18_bSearching = false;
                *xpos = field_25_xpos;
                *ypos = field_26_ypos;
                *zpos = field_27_zpos;
            }
        }
        else if (block_below_type == block_type)
        {
            if (block_type == ROAD)
            {
                if ((gMap_0x370_6F6268->GetBlock_4DFE10(field_25_xpos, field_26_ypos, field_27_zpos - 1)->field_A_arrows & block_arrow::green_mask) != 0)
                {
                    field_18_bSearching = false;
                    *xpos = field_25_xpos;
                    *ypos = field_26_ypos;
                    *zpos = field_27_zpos;
                    return true;
                }
            }
            else
            {
                field_18_bSearching = false;
                *xpos = field_25_xpos;
                *ypos = field_26_ypos;
                *zpos = field_27_zpos;
            }
        }
        field_20_xpos = sPathFinderBestX;
        field_21_ypos = sPathFinderBestY - 1;
        field_22_zpos = sPathFinderBestZ;
        field_23_grid_x = sPathFinderBestGridX;
        field_24_grid_y = sPathFinderBestGridY - 1;
        field_1B_direction = 1;
        if (!PathFinder::ProcessGridCell_5548C0())
        {
            field_20_xpos = sPathFinderBestX + 1;
            field_21_ypos = sPathFinderBestY;
            field_22_zpos = sPathFinderBestZ;
            field_23_grid_x = sPathFinderBestGridX + 1;
            field_24_grid_y = sPathFinderBestGridY;
            field_1B_direction = 2;
            if (!PathFinder::ProcessGridCell_5548C0())
            {
                field_20_xpos = sPathFinderBestX;
                field_21_ypos = sPathFinderBestY + 1;
                field_22_zpos = sPathFinderBestZ;
                field_23_grid_x = sPathFinderBestGridX;
                field_24_grid_y = sPathFinderBestGridY + 1;
                field_1B_direction = 3;
                if (!PathFinder::ProcessGridCell_5548C0())
                {
                    field_20_xpos = sPathFinderBestX - 1;
                    field_21_ypos = sPathFinderBestY;
                    field_22_zpos = sPathFinderBestZ;
                    field_23_grid_x = sPathFinderBestGridX - 1;
                    field_24_grid_y = sPathFinderBestGridY;
                    field_1B_direction = 4;
                    if (!PathFinder::ProcessGridCell_5548C0())
                    {
                        ++field_16_start_cost;
                        if (++expanded_count <= 6 || bAllowDuringPathCompute)
                        {
                            if (!field_18_bSearching)
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
