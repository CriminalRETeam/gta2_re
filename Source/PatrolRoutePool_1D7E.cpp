#include "PatrolRoutePool_1D7E.hpp"

DEFINE_GLOBAL(PatrolRoutePool*, gPatrolRoutePool_6FD784, 0x6FD784);

MATCH_FUNC(0x463F90)
PatrolPoint::PatrolPoint()
{
    field_0_x = 0;
    field_1_y = 0;
    field_2_z = 0;
}

MATCH_FUNC(0x463FA0)
PatrolPoint::~PatrolPoint()
{
}

MATCH_FUNC(0x4bdf70)
PatrolRoutePool::~PatrolRoutePool()
{
}

MATCH_FUNC(0x4bdf90)
PatrolRoute::~PatrolRoute()
{
}

MATCH_FUNC(0x543ec0)
void PatrolRoute::sub_543EC0()
{
}

MATCH_FUNC(0x543ed0)
PatrolRoutePool::PatrolRoutePool()
{
    for (s32 i = 0; i < GTA2_COUNTOF(field_1D4C_bUsed); i++)
    {
        field_1D4C_bUsed[i] = 0;
    }
}

MATCH_FUNC(0x543f10)
PatrolRoute* PatrolRoutePool::AllocPatrolList_543F10(u8* pRet)
{
    for (u8 i = 0; i < GTA2_COUNTOF(field_1D4C_bUsed); i++)
    {
        if (!field_1D4C_bUsed[i])
        {
            field_0_routes[i].sub_543EC0();
            *pRet = i;
            field_1D4C_bUsed[i] = 1;
            return &field_0_routes[i];
        }
    }
    return 0;
}

MATCH_FUNC(0x544bf0)
PatrolRoute::PatrolRoute()
{
}