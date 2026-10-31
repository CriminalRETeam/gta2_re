#pragma once

#include "Function.hpp"

#pragma pack(push)
#pragma pack(1)
class PatrolPoint
{
  public:
    EXPORT PatrolPoint();
    EXPORT ~PatrolPoint();

    u8 mX; // 0x0
    u8 mY; // 0x1
    u8 mZ; // 0x2
};
GTA2_ASSERT_SIZEOF_ALWAYS(PatrolPoint, 0x3);
#pragma pack(pop)

// A patrol route of up to 50 waypoints
class PatrolRoute
{
  public:
    enum
    {
        k_max_points = 50
    };


    EXPORT ~PatrolRoute();
    EXPORT void Init();
    EXPORT PatrolRoute();
    PatrolPoint mPoints[k_max_points]; // 0x0
};
GTA2_ASSERT_SIZEOF_ALWAYS(PatrolRoute, 0x96);

// Pool of 50 patrol routes handed out to peds by AllocPatrolList
class PatrolRoutePool
{
  public:
    enum
    {
        k_max_routes = 50
    };


    EXPORT ~PatrolRoutePool();
    EXPORT PatrolRoutePool();
    EXPORT PatrolRoute* AllocPatrolList(u8* pRouteIdx);
    PatrolRoute mRoutes[k_max_routes]; // 0x0
    bool mbUsed[k_max_routes]; // 0x1D4C
};
GTA2_ASSERT_SIZEOF_ALWAYS(PatrolRoutePool, 0x1D7E);

EXTERN_GLOBAL(PatrolRoutePool*, gPatrolRoutePool_6FD784);
