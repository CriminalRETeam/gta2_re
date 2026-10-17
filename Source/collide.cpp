#include "collide.hpp"
#include "SpriteGrid_400.hpp"
#include "error.hpp"
#include "Globals.hpp"
#include "enums.hpp"

DEFINE_GLOBAL(CollisionCounters_C*, gCollisionCounters_6791FC, 0x6791FC);
DEFINE_GLOBAL(GridSpriteLink_Pool*, gGridSpriteLink_Pool_679200, 0x679200);
DEFINE_GLOBAL(GridCell_Pool*, gGridCell_Pool_679204, 0x679204);

MATCH_FUNC(0x478a20)
void CollisionCounters_C::ResetCount_478A20()
{
    field_0_test_count = 0;
}

MATCH_FUNC(0x478a30)
CollisionCounters_C::CollisionCounters_C()
{
    field_0_test_count = 0;
    field_4_query_id = 0;
    
    field_8_bUnknown = 0;

    gGridSpriteLink_Pool_679200 = new GridSpriteLink_Pool();
    if (!gGridSpriteLink_Pool_679200)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\collide.cpp", 1416);
    }

    gGridCell_Pool_679204 = new GridCell_Pool();
    if (!gGridCell_Pool_679204)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\collide.cpp", 1418);
    }

    gSpriteGrid_1_679208 = new SpriteGrid_400();
    if (!gSpriteGrid_1_679208)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\collide.cpp", 1420);
    }

    gSpriteGrid_2_67920C = new SpriteGrid_400();
    if (!gSpriteGrid_2_67920C)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\collide.cpp", 1422);
    }

    gSpriteGrid_3_679210 = new SpriteGrid_400();
    if (!gSpriteGrid_3_679210)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\collide.cpp", 1424);
    }

    gSpriteGrid_exclusion_sprite_678F84 = 0;
}

MATCH_FUNC(0x478bf0)
CollisionCounters_C::~CollisionCounters_C()
{
    GTA2_DELETE_AND_NULL(gSpriteGrid_1_679208);
    GTA2_DELETE_AND_NULL(gSpriteGrid_2_67920C);
    GTA2_DELETE_AND_NULL(gSpriteGrid_3_679210);
    GTA2_DELETE_AND_NULL(gGridSpriteLink_Pool_679200);
    GTA2_DELETE_AND_NULL(gGridCell_Pool_679204);
}
