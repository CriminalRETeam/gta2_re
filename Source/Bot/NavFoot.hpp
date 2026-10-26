#pragma once

#include "types.hpp"
#include "BotHeap.hpp"

// Foot navigation: A* over map tiles (x, y, z) where an edge exists when the game's own ped
// path finder says a ped can step between the two tiles (PathFinder::TestMoveToAdjacentTile:
// walls, slopes, step heights). The cost is tiles walked, diagonals count 1.4, climbing 2.
// Tiles can be banned, which is how a retry avoids a spot that looked passable and was not.
struct NavNode
{
    u8 x, y, z;
};

class NavFoot : public BotObject
{
  public:
    enum
    {
        kMaxPath = 1024,
        kMaxBans = 64
    };

    NavFoot();
    ~NavFoot();

    // Finds a path from (sx,sy,sz) to any z of tile (tx,ty). false when there is none.
    bool FindPath(u8 sx, u8 sy, u8 sz, u8 tx, u8 ty);

    u32 PathLength() const
    {
        return mPathLen;
    }

    const NavNode& PathAt(u32 i) const
    {
        return mPath[i];
    }

    u32 Expanded() const
    {
        return mExpanded;
    }

    void Ban(u8 x, u8 y);
    void ClearBans();
    u32 BanCount() const
    {
        return mBanCount;
    }

    // Text map of the explored area around the path: S start, G goal, o path, . explored,
    // # banned. Written to path.
    void Dump(const char* path, const char* title) const;

  private:
    bool IsBanned(u8 x, u8 y) const;

    // per (x, y, z<8) search state
    u32* mCost;
    u32* mParent; // packed index of the parent, 0xFFFFFFFF none
    u8* mState;   // 0 unseen, 1 open, 2 closed

    NavNode mPath[kMaxPath];
    u32 mPathLen;
    u32 mExpanded;
    u8 mBanX[kMaxBans];
    u8 mBanY[kMaxBans];
    u32 mBanCount;
    u8 mStartX, mStartY, mTargetX, mTargetY;
};
