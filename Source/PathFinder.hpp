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
    char_type mIdx1; // 0x0
    char_type mIdx2; // 0x1
    u8 mXpos; // 0x2
    u8 mYpos; // 0x3
    u8 mZpos; // 0x4
    char_type mUnused5; // 0x5
    s16 mCost; // 0x6
};
GTA2_ASSERT_SIZEOF_ALWAYS(PathNode, 0x8);
#pragma pack(pop)

// Local path search for peds. Searches a 34x33 window of map tiles around the start (mGrid) with a best-first
// search over mNodes, and is spread over several frames (mIterationBudget).
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
        Init();
    }
    EXPORT bool CanMoveInDirection(s32 path_direction) const;
    EXPORT bool TestMoveToAdjacentTile(u8 curr_xpos, u8 curr_ypos, u8 curr_zpos, u8 desired_xpos, u8 desired_ypos);

  private:
    EXPORT bool CanMoveToAdjacentTile(u8 desired_xpos, u8 desired_ypos);
    EXPORT void ClearGrid();
    EXPORT void Init();

  public:
    ~PathFinder()
    {
    }
    EXPORT void RemovePed(s32 ped_id);

  private:
    EXPORT char_type EvaluateGridCell();
    EXPORT void AddGridCell();
    EXPORT bool ProcessGridCell();
    EXPORT void RestoreSavedPosition();

  public:
    EXPORT s32 IsFirstInPathQueue(Ped* pPed) const;
    EXPORT bool ComputePath(s32 ped_id,
                                        Ped* pPed,
                                        u8 x_start,
                                        u8 y_start,
                                        u8 z_start,
                                        u8 x_end,
                                        u8 y_end,
                                        u8 z_end,
                                        s32 angle_face,
                                        u8* pOutPathPointCount);
    EXPORT bool FindNearbyTileMatchingSlopeType(u8 block_type, u8* xpos, u8* ypos, u8* zpos, bool bAllowDuringPathCompute);

    s32 mPedId; // 0x0
    bool mbFindTileMode; // 0x4: FindNearbyTileMatchingSlopeType, not ComputePath
    PathNode * mpNode; // 0x8
    u16 mNodeCount; // 0xC
    u8 mXStart; // 0xE
    u8 mYStart; // 0xF
    u8 mZStart; // 0x10
    u8 mXEnd; // 0x11
    u8 mYEnd; // 0x12
    u8 mZEnd; // 0x13
    bool mbExpanding; // 0x14
    u16 mStartCost; // 0x16
    bool mbSearching; // 0x18
    char_type mUnused19; // 0x19
    char_type mUnused1A; // 0x1A
    u8 mDirection; // 0x1B
    u16 mGridIdx; // 0x1C
    s16 mCurrentCost; // 0x1E
    u8 mXpos20; // 0x20
    u8 mYpos21; // 0x21
    u8 mZpos22; // 0x22
    u8 mGridX; // 0x23
    u8 mGridY; // 0x24
    u8 mXpos25; // 0x25
    u8 mYpos26; // 0x26
    u8 mZpos27; // 0x27
    char_type mGridOriginX; // 0x28
    char_type mGridOriginY; // 0x29
    char_type mUnused2A; // 0x2A
    char_type mUnused2B; // 0x2B
    s16 mUnused2C; // 0x2C
    u16 mIterationBudget; // 0x2E
    s32 mUnused30; // 0x30
    s16 mUnused34; // 0x34
    s16 mUnused36; // 0x36
    bool mbComputePathInProgress; // 0x38
    s16 mUnused3A; // 0x3A
    Ped_List mPedList; // 0x3C
    PathNode mGrid[1122]; // 0x40
    PathNode mNodes[398]; // 0x2350
    s32 mUnused2FC0; // 0x2FC0
    s32 mUnused2FC4; // 0x2FC4
    s32 mUnused2FC8; // 0x2FC8
    s32 mUnused2FCC; // 0x2FCC
    bool mbTimedOut; // 0x2FD0
    u8 mTimeOutCounter; // 0x2FD1
};
GTA2_ASSERT_SIZEOF_ALWAYS(PathFinder, 0x2FD4);

EXTERN_GLOBAL(PathFinder*, gPathFinder_6FDEF0);
