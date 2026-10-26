#pragma once

#include "Function.hpp"

struct gmp_block_info;

class Link
{
  public:
    // 9.6f 0x40CE90
    inline u16 GetIndex_0040CE90() const
    {
        return field_0 & 0x1ff;
    }

    // 9.6f 0x40CED0
    inline u8 IsEnabled() const
    {
        return field_0 >> 15;
    }

    // 9.6f 0x40CEA0
    inline u16 GetLength() const
    {
        return (field_0 >> 9) & 0x3F;
    }

    inline void Disable_40CEC0()
    {
        field_0 &= ~0x8000u;
    }

    inline void Enable_40CEB0()
    {
        field_0 |= 0x8000u;
    }

    u16 field_0;
};
GTA2_ASSERT_SIZEOF_ALWAYS(Link, 0x2);

class Junction
{
  public:
    EXPORT bool IsUsableWithArrowType_588580(s32 arrow_type) const;
    EXPORT u16 GetDirectionToJunction_5885C0(u16 junction_idx) const;

    // 9.6f 0x40CEE0
    inline bool ContainsPoint(s16 x, s16 y) const
    {
        return x >= field_C_min_x && x <= field_E_max_x && y >= field_D_min_y && y <= field_F_max_y;
    }

    Link field_0_n;
    Link field_2_s;
    Link field_4_e;
    Link field_6_w;
    s32 field_8_type;
    u8 field_C_min_x;
    u8 field_D_min_y;
    u8 field_E_max_x;
    u8 field_F_max_y;
};
GTA2_ASSERT_SIZEOF_ALWAYS(Junction, 0x10);

struct JunctionSegment
{
    inline bool ContainsPoint_40CF20(s16 x, s16 y) const
    {
        return x >= field_4_min_x && x <= field_6_max_x && y >= field_5_min_y && y <= field_7_max_y;
    }
    u16 field_0_junction_num1;
    u16 field_2_junction_num2;
    u8 field_4_min_x;
    u8 field_5_min_y;
    u8 field_6_max_x;
    u8 field_7_max_y;
};
GTA2_ASSERT_SIZEOF_ALWAYS(JunctionSegment, 0x8);

class RouteSearchNode
{
  public:
    EXPORT RouteSearchNode();
    u16 field_0_junction_idx;
    u16 field_2_cost;
    s16 field_4_expanded;
    s16 field_6_unused;
    RouteSearchNode* field_8_pParent;
    RouteSearchNode* field_C_pNext;
};
GTA2_ASSERT_SIZEOF_ALWAYS(RouteSearchNode, 0x10);

class JunctionRoute
{
  public:
    u16 field_0_junctions[256];
};
GTA2_ASSERT_SIZEOF_ALWAYS(JunctionRoute, 0x200);

class RouteFinder
{
  public:
    EXPORT void ShowJunctionIds_588620();
    EXPORT void RoadOff_588810(u8 x, u8 y, u8 z);
    EXPORT void RoadOn_588950(u8 x, u8 y, u8 z);
    EXPORT u16 IsPointInJunctionBounds_588AA0(u8 x, u8 y, u16 junc_idx1, u16 junc_idx2); // 0 or 1, but u16: a bool return changes the code
    EXPORT void Load_RGEN_588B30();
    EXPORT void Reset_588C60();
    EXPORT bool HasBlockDesiredArrow_588CA0(const gmp_block_info* block, s32 arrow_type, u8 direction) const;
    EXPORT bool HasBlockArrowInRoadDirection_588DE0(const gmp_block_info* pBlock, s32 arrow_type, s32 road_direction) const;

  private:
    EXPORT u16 FindHorzSegmentJunction_588E60(u8 x, u8 y, u8 z, char_type bIsDest, s32 arrow_type);
    EXPORT u16 FindVertSegmentJunction_588F30(u8 x, u8 y, u8 z, char_type bIsDest, s32 arrow_type);
    EXPORT u16 FindSegmentJunction_589000(u8 x_coord, u8 y_coord, u8 z_coord, char_type bIsDest, s32 arrow_type);

  public:
    EXPORT void FindArrowBlockInJunction_5890D0(u16 junction_idx, s32 direction, u8* xpos, u8* ypos);

  private:
    EXPORT s32 NoRefs_589210(u8 x, u8 y, s32 a4, u8 direction, s32 a6, u16 junction_idx);
    EXPORT RouteSearchNode* NewChildNode_5892F0(RouteSearchNode* pParent, u16 idx, s16 step_cost);
    EXPORT RouteSearchNode* NewStartNode_589390(u16 junction_idx);
    EXPORT void InsertIntoOpenList_589420(RouteSearchNode* pNode);
    EXPORT bool InitSearch_589480(u8 start_x, u8 start_y, u8 start_z, u8 dest_x, u8 dest_y, u8 dest_z, s32 arrow_type);
    EXPORT bool InitSearchWithDirection_5895C0(u8 x, u8 y, u8 z, s32 arrow_type, s32 direction);

  public:
    EXPORT void CancelRoute_589930(s16 route_idx);

  private:
    EXPORT s16 GetFreeRouteIdx_589960();
    EXPORT u16 AddChildNode_589990(RouteSearchNode* pParent, u16 junction_idx, s16 step_cost);
    EXPORT bool ExpandNode_5899C0(RouteSearchNode* pNode, s32 arrow_type);
    EXPORT bool ExpandNodeUnlessOffScreen_589BB0(RouteSearchNode* pNode, s32 arrow_type);
    EXPORT RouteSearchNode* GetFirstUnexpandedNode_589E00();
    EXPORT bool SearchToDest_589E20(s32 arrow_type);
    EXPORT bool SearchUntilOffScreen_589E70(s32 arrow_type);
    EXPORT s16 StoreRoute_589EB0();
    EXPORT s16 StoreRouteReversed_589F70();

  public:
    EXPORT void DebugPrintRoute_58A020(char_type route_idx);
    EXPORT Junction* GetJunction_58A0B0(u16 junction_idx);
    EXPORT s16 DoStartRoute_58A0D0(u8 x1, u8 y1, u8 z1, u8 x2, u8 y2, u8 z2, s32 arrow_type);
    EXPORT s16 FindOffScreenRoute_58A130(u8 x, s16 y, u8 z, u8* pRouteLength, s32 arrow_type, s32 direction);
    EXPORT u16 StartRoute_58A190(u8 x1, u8 y1, u8 z1, u8 x2, u8 y2, u8 z2, s32 arrow_type);
    EXPORT RouteFinder();

    u16 field_0_route_count;
    u8 field_2;
    char_type field_3_unused;
    s16 field_4_unused;
    s16 field_6_unused;
    Junction field_8_junctions[545];
    JunctionRoute field_2218_routes[50];
    u16 field_8618_start_idx;
    u16 field_861A_dest_idx;
    RouteSearchNode field_861C_nodes[545];
    RouteSearchNode* field_A82C_open_list;
    JunctionSegment field_A830_horz_segments[545];
    JunctionSegment field_B938_vert_segments[545];
    bool field_CA40_visited[545];
    u16 field_CC62_horz_segment_count;
    u16 field_CC64_vert_segment_count;
    u16 field_CC66_545_count;
};

EXTERN_GLOBAL(RouteFinder*, gRouteFinder_6FFDC8);
