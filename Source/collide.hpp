#pragma once

#include "Function.hpp"
#include "Pool.hpp"
#include "SpriteGrid_400.hpp"
#include "fix16.hpp"

class Sprite;

class GridSpriteLink_8
{
  public:
    void PoolAllocate()
    {
    }

    void PoolDeallocate()
    {
    }

    Sprite* field_0_pSprite;
    GridSpriteLink_8* mpNext;
};

class GridSpriteLink_Pool : public PoolBasic<GridSpriteLink_8, 4096>
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

class GridCell_Pool : public PoolBasic<GridCell_C, 6000>
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

class CollisionCounters_C
{
  public:
    EXPORT void ResetCount_478A20();
    EXPORT CollisionCounters_C();
    EXPORT ~CollisionCounters_C();
    s32 field_0_test_count;
    s32 field_4_query_id;
    s32 field_8_bUnknown;
};

EXTERN_GLOBAL(CollisionCounters_C*, gCollisionCounters_6791FC);
