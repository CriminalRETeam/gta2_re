#pragma once

#include "Function.hpp"
#include "Ped_List.hpp"

class Ped;

// Entry of the search grid and of the open node list (the fields mean different things in each, see the cpp)
#pragma pack(push)
#pragma pack(1)
class PathNode
{
  public:
    char_type field_0_idx1; // 0x0
    char_type field_1_idx2; // 0x1
    u8 field_2_xpos; // 0x2
    u8 field_3_ypos; // 0x3
    u8 field_4_zpos; // 0x4
    char_type field_5_unused; // 0x5
    s16 field_6_cost; // 0x6
};
GTA2_ASSERT_SIZEOF_ALWAYS(PathNode, 0x8);
#pragma pack(pop)

// Local path search for peds. Searches a 34x33 window of map tiles around the start (field_40_grid) with a best-first
// search over field_2350_nodes, and is spread over several frames (field_2E_iteration_budget).
class PathFinder
{
  public:
    STATIC_MEMBER(u8, sPathFinderEdge4Z);
    STATIC_MEMBER(u8, sPathFinderEdge4Y);
    STATIC_MEMBER(u8, sPathFinderEdge4X);
    STATIC_MEMBER(u8, sPathFinderEdge4GridY);
    STATIC_MEMBER(u8, sPathFinderEdge4GridX);
    STATIC_MEMBER(u8, sPathFinderEdge3Z);
    STATIC_MEMBER(u8, sPathFinderEdge3Y);
    STATIC_MEMBER(u8, sPathFinderEdge3X);
    STATIC_MEMBER(u8, sPathFinderEdge3GridY);
    STATIC_MEMBER(u8, sPathFinderEdge3GridX);
    STATIC_MEMBER(u8, sPathFinderEdge2Z);
    STATIC_MEMBER(u8, sPathFinderEdge2Y);
    STATIC_MEMBER(u8, sPathFinderEdge2X);
    STATIC_MEMBER(u8, sPathFinderEdge2GridY);
    STATIC_MEMBER(u8, sPathFinderEdge2GridX);
    STATIC_MEMBER(u8, sPathFinderEdge1Z);
    STATIC_MEMBER(u8, sPathFinderEdge1Y);
    STATIC_MEMBER(u8, sPathFinderEdge1X);
    STATIC_MEMBER(u8, sPathFinderEdge1GridY);
    STATIC_MEMBER(u8, sPathFinderEdge1GridX);
    STATIC_MEMBER(u8, sPathFinderBestZ);
    STATIC_MEMBER(u8, sPathFinderBestY);
    STATIC_MEMBER(u8, sPathFinderBestX);
    STATIC_MEMBER(u8, sPathFinderBestGridY);
    STATIC_MEMBER(u8, sPathFinderBestGridX);
    STATIC_MEMBER(s16, sPathFinderBestCost);
    STATIC_MEMBER(s32, sPathFinderAngleFace);
    STATIC_MEMBER(u8, sPathFinderSlopeZDelta);
    PathFinder()
    {
        Init_5545E0();
    }
    EXPORT bool CanMoveInDirection_554080(s32 path_direction) const;
    EXPORT bool TestMoveToAdjacentTile_5540E0(u8 curr_xpos, u8 curr_ypos, u8 curr_zpos, u8 desired_xpos, u8 desired_ypos);

  private:
    EXPORT bool CanMoveToAdjacentTile_554110(u8 desired_xpos, u8 desired_ypos);
    EXPORT void ClearGrid_5545C0();
    EXPORT void Init_5545E0();

  public:
    ~PathFinder()
    {
    }
    EXPORT void RemovePed_554620(s32 ped_id);

  private:
    EXPORT char_type EvaluateGridCell_554640();
    EXPORT void AddGridCell_554710();
    EXPORT bool ProcessGridCell_5548C0();
    EXPORT void RestoreSavedPosition_554920();

  public:
    EXPORT s32 IsFirstInPathQueue_554A90(Ped* pPed) const;
    EXPORT bool ComputePath_554AB0(s32 ped_id,
                                        Ped* pPed,
                                        u8 x_start,
                                        u8 y_start,
                                        u8 z_start,
                                        u8 x_end,
                                        u8 y_end,
                                        u8 z_end,
                                        s32 angle_face,
                                        u8* pOutPathPointCount);
    EXPORT bool FindNearbyTileMatchingSlopeType_5552B0(u8 block_type, u8* xpos, u8* ypos, u8* zpos, bool bAllowDuringPathCompute);

    s32 field_0_ped_id; // 0x0
    bool field_4_bFindTileMode; // 0x4: FindNearbyTileMatchingSlopeType_5552B0, not ComputePath_554AB0
    PathNode * field_8_pNode; // 0x8
    u16 field_C_node_count; // 0xC
    u8 field_E_xStart; // 0xE
    u8 field_F_yStart; // 0xF
    u8 field_10_zStart; // 0x10
    u8 field_11_xEnd; // 0x11
    u8 field_12_yEnd; // 0x12
    u8 field_13_zEnd; // 0x13
    bool field_14_bExpanding; // 0x14
    u16 field_16_start_cost; // 0x16
    bool field_18_bSearching; // 0x18
    char_type field_19_unused; // 0x19
    char_type field_1A_unused; // 0x1A
    u8 field_1B_direction; // 0x1B
    u16 field_1C_grid_idx; // 0x1C
    s16 field_1E_current_cost; // 0x1E
    u8 field_20_xpos; // 0x20
    u8 field_21_ypos; // 0x21
    u8 field_22_zpos; // 0x22
    u8 field_23_grid_x; // 0x23
    u8 field_24_grid_y; // 0x24
    u8 field_25_xpos; // 0x25
    u8 field_26_ypos; // 0x26
    u8 field_27_zpos; // 0x27
    char_type field_28_grid_origin_x; // 0x28
    char_type field_29_grid_origin_y; // 0x29
    char_type field_2A_unused; // 0x2A
    char_type field_2B_unused; // 0x2B
    s16 field_2C_unused; // 0x2C
    u16 field_2E_iteration_budget; // 0x2E
    s32 field_30_unused; // 0x30
    s16 field_34_unused; // 0x34
    s16 field_36_unused; // 0x36
    bool field_38_bComputePathInProgress; // 0x38
    s16 field_3A_unused; // 0x3A
    Ped_List field_3C_ped_list; // 0x3C
    PathNode field_40_grid[1122]; // 0x40
    PathNode field_2350_nodes[398]; // 0x2350
    s32 field_2FC0_unused; // 0x2FC0
    s32 field_2FC4_unused; // 0x2FC4
    s32 field_2FC8_unused; // 0x2FC8
    s32 field_2FCC_unused; // 0x2FCC
    bool field_2FD0_bTimedOut; // 0x2FD0
    u8 field_2FD1_time_out_counter; // 0x2FD1
};
GTA2_ASSERT_SIZEOF_ALWAYS(PathFinder, 0x2FD4);

EXTERN_GLOBAL(PathFinder*, gPathFinder_6FDEF0);
