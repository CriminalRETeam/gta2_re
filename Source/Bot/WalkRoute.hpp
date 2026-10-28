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
