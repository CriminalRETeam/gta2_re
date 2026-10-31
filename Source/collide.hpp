#pragma once

#include "Function.hpp"
#include "Pool.hpp"
#include "SpriteGrid.hpp"
#include "fix16.hpp"

class Sprite;

class GridSpriteLink
{
  public:
    void PoolAllocate()
    {
    }

    void PoolDeallocate()
    {
    }

    Sprite* mpSprite; // 0x0
    GridSpriteLink* mpNext;
};
GTA2_ASSERT_SIZEOF_ALWAYS(GridSpriteLink, 0x8);

class GridSpriteLink_Pool : public PoolBasic<GridSpriteLink, 4096>
{
  public:
    GridSpriteLink_Pool()
    {

    }

    ~GridSpriteLink_Pool()
    {
      this->mpHead = NULL;
    }
};

class GridCell_Pool : public PoolBasic<GridCell, 6000>
{
  public:
    GridCell_Pool()
    {

    }

    ~GridCell_Pool()
    {
      this->mpHead = NULL;
    }
};

class CollisionCounters
{
  public:
    EXPORT void ResetCount_478A20();
    EXPORT CollisionCounters();
    EXPORT ~CollisionCounters();
    s32 mTestCount; // 0x0
    s32 mQueryId; // 0x4
    s32 mbDoorAccessCheck; // 0x8
};
GTA2_ASSERT_SIZEOF_ALWAYS(CollisionCounters, 0xC);

EXTERN_GLOBAL(CollisionCounters*, gCollisionCounters_6791FC);
