#pragma once

// The walker's own copy of the route it plans. The navigation object that plans it is shared with the trip goal and the
// car choice, which plan other routes through it while the walker is still walking: following the shared path then means
// following some other route (the walker aimed at "waypoint 20 of 11" and wandered off). Plain data, no game types, so
// Scripts/bot/sim/walk_route_sim.cpp tests it natively.

struct RouteNode
{
    unsigned char x, y, z;
};

class WalkRoute
{
  public:
    enum
    {
        kMax = 1024
    };
    WalkRoute()
        : mLen(0)
        , mApprox(false)
    {
        mNodes[0].x = mNodes[0].y = mNodes[0].z = 0;
    }
    // Copies a path (longer than kMax: the first kMax nodes)
    void Set(const RouteNode* src, unsigned len, bool approximate)
    {
        mLen = len < kMax ? len : kMax;
        for (unsigned i = 0; i < mLen; i++)
        {
            mNodes[i] = src[i];
        }
        mApprox = approximate;
    }
    unsigned Length() const
    {
        return mLen;
    }
    bool Approximate() const
    {
        return mApprox;
    }
    // An index past the end is the last node, never memory that belongs to something else
    const RouteNode& At(unsigned i) const
    {
        return mNodes[mLen == 0 ? 0 : (i < mLen ? i : mLen - 1)];
    }

  private:
    RouteNode mNodes[kMax];
    unsigned mLen;
    bool mApprox;
};

// A step to a diagonal neighbour that has a closed tile (a fence, a wall, a building) at either side cuts that tile's corner: the
// walker, aiming straight at the next node's middle, snags on the corner and presses against it. Such a step becomes two straight
// ones, through the side that is open. Writes at most dst_max nodes and returns how many; the nodes keep the height of the node they
// come after. solid(user, x, y) says whether a tile is closed.
inline unsigned WalkRoute_AvoidCorners(const RouteNode* src, unsigned len, RouteNode* dst, unsigned dst_max, bool (*solid)(void* user, int x, int y), void* user)
{
    unsigned n = 0;
    for (unsigned i = 0; i < len && n < dst_max; i++)
    {
        if (i > 0 && n < dst_max)
        {
            const int dx = static_cast<int>(src[i].x) - static_cast<int>(src[i - 1].x);
            const int dy = static_cast<int>(src[i].y) - static_cast<int>(src[i - 1].y);
            if ((dx == 1 || dx == -1) && (dy == 1 || dy == -1))
            {
                const int ax = static_cast<int>(src[i - 1].x) + dx; // the corner through the x side
                const int ay = static_cast<int>(src[i - 1].y);
                const int bx = static_cast<int>(src[i - 1].x);
                const int by = static_cast<int>(src[i - 1].y) + dy;
                const bool a_closed = solid(user, ax, ay);
                const bool b_closed = solid(user, bx, by);
                if ((a_closed || b_closed) && !(a_closed && b_closed) && n + 1 < dst_max)
                {
                    dst[n].x = static_cast<unsigned char>(a_closed ? bx : ax);
                    dst[n].y = static_cast<unsigned char>(a_closed ? by : ay);
                    dst[n].z = src[i - 1].z;
                    n++;
                }
            }
        }
        dst[n++] = src[i];
    }
    return n;
}

