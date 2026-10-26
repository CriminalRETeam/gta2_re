#pragma once

#include "Function.hpp"

#pragma pack(push)
#pragma pack(1)
class PatrolPoint
{
  public:
    EXPORT PatrolPoint();
    EXPORT ~PatrolPoint();

    u8 field_0_x;
    u8 field_1_y;
    u8 field_2_z;
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
    EXPORT void Init_543EC0();
    EXPORT PatrolRoute();
    PatrolPoint field_0_points[k_max_points];
};
GTA2_ASSERT_SIZEOF_ALWAYS(PatrolRoute, 0x96);

// Pool of 50 patrol routes handed out to peds by AllocPatrolList_543F10
class PatrolRoutePool
{
  public:
    enum
    {
        k_max_routes = 50
    };


    EXPORT ~PatrolRoutePool();
    EXPORT PatrolRoutePool();
    EXPORT PatrolRoute* AllocPatrolList_543F10(u8* pRouteIdx);
    PatrolRoute field_0_routes[k_max_routes];
    bool field_1D4C_bUsed[k_max_routes];
};
GTA2_ASSERT_SIZEOF_ALWAYS(PatrolRoutePool, 0x1D7E);

EXTERN_GLOBAL(PatrolRoutePool*, gPatrolRoutePool_6FD784);
