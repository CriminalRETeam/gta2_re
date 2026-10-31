#pragma once

#include "Function.hpp"

struct gmp_block_info;

class Link
{
  public:
    // 9.6f 0x40CE90
    inline u16 GetIndex_0040CE90() const
    {
        return mPacked & 0x1ff;
    }

    // 9.6f 0x40CED0
    inline u8 IsEnabled() const
    {
        return mPacked >> 15;
    }

    // 9.6f 0x40CEA0
    inline u16 GetLength() const
    {
        return (mPacked >> 9) & 0x3F;
    }

    // 9.6f 0x40CEC0
    inline void Disable_40CEC0()
    {
        mPacked &= ~0x8000u;
    }

    // 9.6f 0x40CEB0
    inline void Enable_40CEB0()
    {
        mPacked |= 0x8000u;
    }

    u16 mPacked; // 0x0
};
GTA2_ASSERT_SIZEOF_ALWAYS(Link, 0x2);

class Junction
{
  public:
    EXPORT bool IsUsableWithArrowType(s32 arrow_type) const;
    EXPORT u16 GetDirectionToJunction(u16 junction_idx) const;

    // 9.6f 0x40CEE0
    inline bool ContainsPoint(s16 x, s16 y) const
    {
        return x >= mMinX && x <= mMaxX && y >= mMinY && y <= mMaxY;
    }

    Link mN; // 0x0
    Link mS; // 0x2
    Link mE; // 0x4
    Link mW; // 0x6
    s32 mType; // 0x8
    u8 mMinX; // 0xC
    u8 mMinY; // 0xD
    u8 mMaxX; // 0xE
    u8 mMaxY; // 0xF
};
GTA2_ASSERT_SIZEOF_ALWAYS(Junction, 0x10);

struct JunctionSegment
{
    // 9.6f 0x40CF20
    inline bool ContainsPoint_40CF20(s16 x, s16 y) const
    {
        return x >= mMinX && x <= mMaxX && y >= mMinY && y <= mMaxY;
    }
    u16 mJunctionNum1; // 0x0
    u16 mJunctionNum2; // 0x2
    u8 mMinX; // 0x4
    u8 mMinY; // 0x5
    u8 mMaxX; // 0x6
    u8 mMaxY; // 0x7
};
GTA2_ASSERT_SIZEOF_ALWAYS(JunctionSegment, 0x8);

class RouteSearchNode
{
  public:
    EXPORT RouteSearchNode();
    u16 mJunctionIdx; // 0x0
    u16 mCost; // 0x2
    s16 mExpanded; // 0x4
    s16 mUnused6; // 0x6
    RouteSearchNode* mpParent; // 0x8
    RouteSearchNode* mpNext; // 0xC
};
GTA2_ASSERT_SIZEOF_ALWAYS(RouteSearchNode, 0x10);

class JunctionRoute
{
  public:
    u16 mJunctions[256]; // 0x0
};
GTA2_ASSERT_SIZEOF_ALWAYS(JunctionRoute, 0x200);

class RouteFinder
{
  public:
    STATIC_MEMBER(u16, sLastRouteLength);
    EXPORT void ShowJunctionIds();
    EXPORT void RoadOff(u8 x, u8 y, u8 z);
    EXPORT void RoadOn(u8 x, u8 y, u8 z);
    EXPORT u16 IsPointInJunctionBounds(u8 x, u8 y, u16 junc_idx1, u16 junc_idx2); // 0 or 1, but u16: a bool return changes the code
    EXPORT void Load_RGEN();
    EXPORT void Reset();
    EXPORT bool HasBlockDesiredArrow(const gmp_block_info* block, s32 arrow_type, u8 direction) const;
    EXPORT bool HasBlockArrowInRoadDirection(const gmp_block_info* pBlock, s32 arrow_type, s32 road_direction) const;

  private:
    EXPORT u16 FindHorzSegmentJunction(u8 x, u8 y, u8 z, bool bIsDest, s32 arrow_type);
    EXPORT u16 FindVertSegmentJunction(u8 x, u8 y, u8 z, bool bIsDest, s32 arrow_type);
    EXPORT u16 FindSegmentJunction(u8 x_coord, u8 y_coord, u8 z_coord, bool bIsDest, s32 arrow_type);

  public:
    EXPORT void FindArrowBlockInJunction(u16 junction_idx, s32 direction, u8* xpos, u8* ypos);

  private:
    EXPORT s32 NoRefs(u8 x, u8 y, s32, u8 direction, s32, u16 junction_idx);
    EXPORT RouteSearchNode* NewChildNode(RouteSearchNode* pParent, u16 idx, s16 step_cost);
    EXPORT RouteSearchNode* NewStartNode(u16 junction_idx);
    EXPORT void InsertIntoOpenList(RouteSearchNode* pNode);
    EXPORT bool InitSearch(u8 start_x, u8 start_y, u8 start_z, u8 dest_x, u8 dest_y, u8 dest_z, s32 arrow_type);
    EXPORT bool InitSearchWithDirection(u8 x, u8 y, u8 z, s32 arrow_type, s32 direction);

  public:
    EXPORT void CancelRoute(s16 route_idx);

  private:
    EXPORT s16 GetFreeRouteIdx();
    EXPORT u16 AddChildNode(RouteSearchNode* pParent, u16 junction_idx, s16 step_cost);
    EXPORT bool ExpandNode(RouteSearchNode* pNode, s32 arrow_type);
    EXPORT bool ExpandNodeUnlessOffScreen(RouteSearchNode* pNode, s32 arrow_type);
    EXPORT RouteSearchNode* GetFirstUnexpandedNode();
    EXPORT bool SearchToDest(s32 arrow_type);
    EXPORT bool SearchUntilOffScreen(s32 arrow_type);
    EXPORT s16 StoreRoute();
    EXPORT s16 StoreRouteReversed();

  public:
    EXPORT void DebugPrintRoute(char_type route_idx);
    EXPORT Junction* GetJunction(u16 junction_idx);
    EXPORT s16 DoStartRoute(u8 x1, u8 y1, u8 z1, u8 x2, u8 y2, u8 z2, s32 arrow_type);
    EXPORT s16 FindOffScreenRoute(u8 x, s16 y, u8 z, u8* pRouteLength, s32 arrow_type, s32 direction);
    EXPORT u16 StartRoute(u8 x1, u8 y1, u8 z1, u8 x2, u8 y2, u8 z2, s32 arrow_type);
    EXPORT RouteFinder();

    u16 mRouteCount; // 0x0
    u8 mNoJunctionDirection; // 0x2
    char_type mUnused3; // 0x3
    s16 mUnused4; // 0x4
    s16 mUnused6; // 0x6
    Junction mJunctions[545]; // 0x8
    JunctionRoute mRoutes[50]; // 0x2218
    u16 mStartIdx; // 0x8618
    u16 mDestIdx; // 0x861A
    RouteSearchNode mNodes[545]; // 0x861C
    RouteSearchNode* mOpenList; // 0xA82C
    JunctionSegment mHorzSegments[545]; // 0xA830
    JunctionSegment mVertSegments[545]; // 0xB938
    bool mVisited[545]; // 0xCA40
    u16 mHorzSegmentCount; // 0xCC62
    u16 mVertSegmentCount; // 0xCC64
    u16 m545Count; // 0xCC66
};

EXTERN_GLOBAL(RouteFinder*, gRouteFinder);
