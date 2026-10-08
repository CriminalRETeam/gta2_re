#pragma once

#include "Function.hpp"
#include "Ped_List_4.hpp"

class Ped;

// Entry of the search grid and of the open node list (the fields mean different things in each, see the cpp)
#pragma pack(push)
#pragma pack(1)
class PathNode_8
{
  public:
    char_type field_0_idx1;
    char_type field_1_idx2;
    u8 field_2_xpos;
    u8 field_3_ypos;
    u8 field_4_zpos;
    char_type field_5;
    s16 field_6_cost;
};
#pragma pack(pop)

// Local path search for peds. Searches a 34x33 window of map tiles around the start (field_40_grid) with a best-first
// search over field_2350_nodes, and is spread over several frames (field_2E_iteration_budget).
class PathFinder_2FD4
{
  public:
    PathFinder_2FD4()
    {
        Init_5545E0();
    }
    EXPORT bool CanMoveInDirection_554080(s32 path_direction);
    EXPORT char_type TestDiagonalMove_5540E0(u8 curr_xpos, u8 curr_ypos, u8 curr_zpos, u8 desired_xpos, u8 desired_ypos);
    EXPORT char_type CanMoveDiagonally_554110(u8 desired_xpos, u8 desired_ypos);
    EXPORT void ClearGrid_5545C0();
    EXPORT void Init_5545E0();

    ~PathFinder_2FD4()
    {
        // TODO: Should this be empty?
    }
    EXPORT void RemovePed_554620(s32 ped_id);
    EXPORT char_type EvaluateGridCell_554640();
    EXPORT void AddGridCell_554710();
    EXPORT bool ProcessGridCell_5548C0();
    EXPORT void RestoreSavedPosition_554920();
    EXPORT s32 IsFirstPassenger_554A90(Ped* pPed);
    EXPORT char_type ComputePath_554AB0(s32 ped_id, Ped* pPed, u8 x_start, u8 y_start, u8 z_start, u8 x_end, u8 y_end, u8 z_end, s32 angle_face, u8* pOutPathFailCount);
    EXPORT bool FindNearbyTileMatchingSlopeType_5552B0(u8 block_type, u8* xpos, u8* ypos, u8* zpos, char_type maybe_timer);

    s32 field_0_ped_id;
    char_type field_4_bFindTileMode; // 1: FindNearbyTileMatchingSlopeType, 0: ComputePath
    PathNode_8 * field_8_pNode;
    u16 field_C_node_count;
    u8 field_E_xStart;
    u8 field_F_yStart;
    u8 field_10_zStart;
    u8 field_11_xEnd;
    u8 field_12_yEnd;
    u8 field_13_zEnd;
    u8 field_14;
    u16 field_16;
    char_type field_18;
    char_type field_19;
    char_type field_1A;
    u8 field_1B_direction;
    u16 field_1C_grid_idx;
    s16 field_1E_current_cost;
    u8 field_20_xpos;
    u8 field_21_ypos;
    u8 field_22_zpos;
    u8 field_23_grid_x;
    u8 field_24_grid_y;
    u8 field_25_xpos;
    u8 field_26_ypos;
    u8 field_27_zpos;
    char_type field_28;
    char_type field_29;
    char_type field_2A;
    char_type field_2B;
    s16 field_2C;
    u16 field_2E_iteration_budget;
    s32 field_30;
    s16 field_34;
    s16 field_36;
    char_type field_38_bComputePathInProgress;
    s16 field_3A;
    Ped_List_4 field_3C_ped_list;
    PathNode_8 field_40_grid[1122];
    PathNode_8 field_2350_nodes[398];
    s32 field_2FC0;
    s32 field_2FC4;
    s32 field_2FC8;
    s32 field_2FCC;
    char_type field_2FD0_bTimedOut;
    u8 field_2FD1_time_out_counter;
};

EXTERN_GLOBAL(PathFinder_2FD4*, gPathFinder_6FDEF0);
