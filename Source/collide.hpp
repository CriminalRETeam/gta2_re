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

    Sprite* field_0_pSprite;
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
      this->field_0_pHead = 0;
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
      this->field_0_pHead = 0;
    }
};

class CollisionCounters
{
  public:
    EXPORT void ResetCount_478A20();
    EXPORT CollisionCounters();
    EXPORT ~CollisionCounters();
    s32 field_0_test_count;
    s32 field_4_query_id;
    s32 field_8_bUnknown;
};
GTA2_ASSERT_SIZEOF_ALWAYS(CollisionCounters, 0xC);

EXTERN_GLOBAL(CollisionCounters*, gCollisionCounters_6791FC);
