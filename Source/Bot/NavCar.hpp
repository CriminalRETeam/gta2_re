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

    // The wall bits of a tile (1 left, 2 right, 4 top, 8 bottom) and whether it is open, for debugging
    u8 WallBits(int x, int y)
    {
        if (!mBuilt)
        {
            Build();
        }
        return static_cast<u8>(mWalls[y * 256 + x] | (mGrid[y * 256 + x] ? 0x10 : 0));
    }

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

    // The last FindPath ended at the nearest tile the car can reach, not at the target
    bool Approximate() const
    {
        return mApproximate;
    }

    u32 Expanded() const
    {
        return mExpanded;
    }

    // Tiles within radius of (cx, cy) are closed to FindPath until ClearBans (a drive got stuck there)
    // A pavement or field tile with clearance (not a road: cars parked there jam the traffic) between min_r and max_r tiles from (x, y), nearest first, for moving a car out of a doorway
    bool FindClearSpot(float x, float y, int min_r, int max_r, float* out_x, float* out_y) const;

    // Does the straight line cross a fence or wall edge between tiles? (the car router's wall data)
    bool CrossesWall(float x0, float y0, float x1, float y1);

    // Is the straight line between the two points open with room for a car all along it?
    bool DirectLine(float sx, float sy, float tx, float ty) const;

    // Traffic: tiles near a car that is not moving cost extra for the next searches (cleared by ClearJams)
    void ClearJams();
    void AddJam(float x, float y);

    void BanArea(int cx, int cy, int radius);
    void ClearBans();

    void Dump(const char* path, const char* title) const;

  private:
    bool Open(int x, int y) const;
    bool EdgeFree(int x, int y, int nx, int ny) const;
    void NearestOpen(int* x, int* y) const;
    bool Safe(int x, int y, int clearance) const;
    bool LineSafe(int x0, int y0, int x1, int y1) const;
    bool Search(int sx, int sy, int tx, int ty, int clearance, bool approximate);

    u8* mGrid;   // 0 blocked, 1 field, 2 pavement, 3 road
    u8* mWalls;  // bit0 left, bit1 right, bit2 top, bit3 bottom: the block face is a wall
    u8* mJam;    // tiles occupied or hemmed in by stopped cars
    u8* mBan;    // tiles a stuck drive asked to avoid
    u8* mArrows; // low nibble: green arrows bit0 left, bit1 right, bit2 up, bit3 down
    u32* mCost;
    u32* mParent;
    u8* mState;
    u16* mPathTiles;
    u16* mAltTiles;
    u32 mAltCount;
    u32 mLastCost;
    u32 mJamCount;
    u32 mPathTileCount;
    float mX[kMaxWp];
    float mY[kMaxWp];
    u32 mCount;
    u32 mExpanded;
    bool mBuilt;
    bool mApproximate;
    int mSx, mSy, mTx, mTy;
};
