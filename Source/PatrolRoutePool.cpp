#include "PatrolRoutePool.hpp"

DEFINE_GLOBAL(PatrolRoutePool*, gPatrolRoutePool_6FD784, 0x6FD784);

MATCH_FUNC(0x463F90)
PatrolPoint::PatrolPoint()
{
    mX = 0;
    mY = 0;
    mZ = 0;
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
void PatrolRoute::Init_543EC0()
{
}

MATCH_FUNC(0x543ed0)
PatrolRoutePool::PatrolRoutePool()
{
    for (s32 i = 0; i < GTA2_COUNTOF(mbUsed); i++)
    {
        mbUsed[i] = false;
    }
}

MATCH_FUNC(0x543f10)
PatrolRoute* PatrolRoutePool::AllocPatrolList_543F10(u8* pRouteIdx)
{
    for (u8 i = 0; i < GTA2_COUNTOF(mbUsed); i++)
    {
        if (!mbUsed[i])
        {
            mRoutes[i].Init_543EC0();
            *pRouteIdx = i;
            mbUsed[i] = true;
            return &mRoutes[i];
        }
    }
    return NULL;
}

MATCH_FUNC(0x544bf0)
PatrolRoute::PatrolRoute()
{
}