#pragma once

#include "types.hpp"
#include "BotHeap.hpp"

// Driving navigation: A* over drivable tiles (anything that is not a building / high ground),
// preferring roads, driving with the lane arrows rather than against them, keeping one tile of
// clearance from walls, then string pulled into long straight legs. The game's RouteFinder
// (junction graph) was tried first: it needs both ends on arrow blocks and knows nothing of yards,
// kerbs and pavements, which is where a parked car starts.
class NavCar : public BotObject
{
  public:
    enum
    {
        kMaxWp = 256
    };

    NavCar();
    ~NavCar();

    // Reads the map: call once the level is loaded (and again after anything changes the map)
    void Build();

    // Waypoints are tile centres in world coordinates
    bool FindPath(float sx, float sy, float tx, float ty);

    u32 Count() const
    {
        return mCount;
    }

    float X(u32 i) const
    {
        return mX[i];
    }

    float Y(u32 i) const
    {
        return mY[i];
    }

    u32 Expanded() const
    {
        return mExpanded;
    }

    void Dump(const char* path, const char* title) const;

  private:
    bool Open(int x, int y) const;
    bool Safe(int x, int y, int clearance) const;
    bool LineSafe(int x0, int y0, int x1, int y1) const;
    bool Search(int sx, int sy, int tx, int ty, int clearance);

    u8* mGrid;   // 0 blocked, 1 field, 2 pavement, 3 road
    u8* mArrows; // low nibble: green arrows bit0 left, bit1 right, bit2 up, bit3 down
    u32* mCost;
    u32* mParent;
    u8* mState;
    u16* mPathTiles;
    u32 mPathTileCount;
    float mX[kMaxWp];
    float mY[kMaxWp];
    u32 mCount;
    u32 mExpanded;
    bool mBuilt;
    int mSx, mSy, mTx, mTy;
};
