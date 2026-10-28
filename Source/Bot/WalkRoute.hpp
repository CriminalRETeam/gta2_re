#pragma once

// The walker's own copy of the route it plans. The navigation object that plans it is shared with the trip goal and the
// car choice, which plan other routes through it while the walker is still walking: following the shared path then means
// following some other route (the walker aimed at "waypoint 20 of 11" and wandered off). Plain data, no game types, so
// Scripts/bot/sim/walk_route_sim.cpp tests it natively.

struct RouteNode
{
    unsigned char x, y, z;
    unsigned char tight; // 1: to be reached close to its middle (the approach to a ramp, and the ramp itself), not within the usual 0.6
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
                    dst[n].tight = 0;
                    n++;
                }
            }
        }
        dst[n++] = src[i];
    }
    return n;
}

// Ramps. A ped goes up or down a ramp along its own axis and through the middle of its tiles: arriving at the foot at an angle, or off to
// one side, puts it against the wall along the ramp's edge and it stands pushing at it. The axis of a slope by its id (the block's slope
// type >> 2): 0 none, 1 along x (west-east), 2 along y (north-south)
inline int WalkRoute_SlopeAxisOfId(int id)
{
    if (id >= 1 && id <= 4)
    {
        return 2; // 26 degrees up north / down south
    }
    if (id >= 5 && id <= 8)
    {
        return 1; // left / right
    }
    if (id >= 9 && id <= 24)
    {
        return 2; // 7 degrees: up north, down south
    }
    if (id >= 25 && id <= 40)
    {
        return 1; // 7 degrees: left, right
    }
    if (id == 41 || id == 42)
    {
        return 2; // 45 degrees
    }
    if (id == 43 || id == 44)
    {
        return 1;
    }
    return 0;
}

// Makes a route fit the ramps on it. A diagonal step into a ramp tile becomes two: across the ramp's axis first, then along it; a diagonal
// step out of one: along the axis first, then across. The nodes on a ramp, and the node before one, are marked tight (the walker has to
// get close to their middle before going on, which lines it up with the ramp). axis(user, x, y, z) is the slope axis of a tile (0 none);
// solid(user, x, y) says whether a tile is closed (no tile is added on one). Writes at most dst_max nodes and returns how many
inline unsigned WalkRoute_AlignSlopes(const RouteNode* src, unsigned len, RouteNode* dst, unsigned dst_max, int (*axis)(void* user, int x, int y, int z),
                                      bool (*solid)(void* user, int x, int y), void* user)
{
    unsigned n = 0;
    for (unsigned i = 0; i < len && n < dst_max; i++)
    {
        if (i > 0 && n + 1 < dst_max)
        {
            const int px = static_cast<int>(src[i - 1].x);
            const int py = static_cast<int>(src[i - 1].y);
            const int dx = static_cast<int>(src[i].x) - px;
            const int dy = static_cast<int>(src[i].y) - py;
            if ((dx == 1 || dx == -1) && (dy == 1 || dy == -1))
            {
                const int into = axis(user, src[i].x, src[i].y, src[i].z);
                const int out_of = axis(user, src[i - 1].x, src[i - 1].y, src[i - 1].z);
                int mx = -1;
                int my = -1;
                if (into == 1)
                {
                    mx = px; // along x into it: across (y) first
                    my = py + dy;
                }
                else if (into == 2)
                {
                    mx = px + dx;
                    my = py;
                }
                else if (out_of == 1)
                {
                    mx = px + dx; // along x out of it: along x first
                    my = py;
                }
                else if (out_of == 2)
                {
                    mx = px;
                    my = py + dy;
                }
                if (mx >= 0 && !solid(user, mx, my))
                {
                    dst[n].x = static_cast<unsigned char>(mx);
                    dst[n].y = static_cast<unsigned char>(my);
                    dst[n].z = src[i - 1].z;
                    dst[n].tight = 0;
                    n++;
                }
            }
        }
        dst[n] = src[i];
        dst[n].tight = 0;
        n++;
    }
    for (unsigned k = 0; k < n; k++)
    {
        const bool on_ramp = axis(user, dst[k].x, dst[k].y, dst[k].z) != 0;
        const bool before_ramp = k + 1 < n && axis(user, dst[k + 1].x, dst[k + 1].y, dst[k + 1].z) != 0;
        dst[k].tight = (on_ramp || before_ramp) ? 1 : 0;
    }
    return n;
}

// Has the walker at (px, py) reached the node? Within 0.6 tiles of its middle; a tight node (a ramp, or the approach to one) within 0.3, so
// that the walker is lined up with the ramp before it turns on to it. (At 0.6 a walker at the tile's edge, 0.57 off, turned for the ramp
// while still low in the tile, and pressed against the wall beside the ramp.)
inline bool WalkRoute_Reached(const RouteNode& n, float px, float py)
{
    const float reach = n.tight ? 0.3f : 0.6f;
    const float dx = static_cast<float>(n.x) + 0.5f - px;
    const float dy = static_cast<float>(n.y) + 0.5f - py;
    return dx * dx + dy * dy <= reach * reach;
}

// May the walker jump to get past a snag? Not close to its goal, where a jump overshoots a marker: close by the route, which is not the
// straight line (a token on a platform above the walker is 1.6 tiles away in a line and 19 route nodes away: a ledge in the way needs a
// jump). straight: tiles to the goal; idx, len: the walker's node and the route's length
inline bool WalkRoute_MayJump(float straight, unsigned idx, unsigned len, float no_jump_near)
{
    return straight > no_jump_near || idx + 3 < len;
}

// The extra cost, in tenths of a tile, of walking onto a tile of this kind (WorldModel::TileType: 1 road, 2 pavement, 3 field,
// 4 high ground/bridge): a walker keeps to the pavement and crosses a road where it has to, or where the pavement is much longer
inline int WalkRoute_TerrainCost(int tile_type)
{
    return tile_type == 1 ? 8 : 0;
}
