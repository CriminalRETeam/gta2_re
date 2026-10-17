#pragma once

#include "Function.hpp"

#pragma pack(push)
#pragma pack(1)
class PatrolPoint_3
{
  public:
    // ctor 463F90
    // dtor 463FA0
    EXPORT PatrolPoint_3();
    EXPORT ~PatrolPoint_3();

    u8 field_0_x;
    u8 field_1_y;
    u8 field_2_z;
};
#pragma pack(pop)

// A patrol route of up to 50 waypoints
class PatrolRoute_96
{
  public:
    enum
    {
        k_max_points = 50
    };


    EXPORT ~PatrolRoute_96();
    EXPORT void sub_543EC0();
    EXPORT PatrolRoute_96();
    PatrolPoint_3 field_0_points[k_max_points];
};

// Pool of 50 patrol routes handed out to peds by AllocPatrolList_543F10
class PatrolRoutePool_1D7E
{
  public:
    enum
    {
        k_max_routes = 50
    };


    EXPORT ~PatrolRoutePool_1D7E();
    EXPORT PatrolRoutePool_1D7E();
    EXPORT PatrolRoute_96* AllocPatrolList_543F10(u8* pRet);
    PatrolRoute_96 field_0_routes[k_max_routes];
    u8 field_1D4C_bUsed[k_max_routes];
};

EXTERN_GLOBAL(PatrolRoutePool_1D7E*, gPatrolRoutePool_6FD784);
