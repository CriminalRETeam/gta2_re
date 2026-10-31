#include "collide.hpp"
#include "SpriteGrid.hpp"
#include "error.hpp"
#include "Globals.hpp"
#include "enums.hpp"

DEFINE_GLOBAL(CollisionCounters*, gCollisionCounters, 0x6791FC);
DEFINE_GLOBAL(GridSpriteLink_Pool*, gGridSpriteLink_Pool, 0x679200);
DEFINE_GLOBAL(GridCell_Pool*, gGridCell_Pool, 0x679204);

MATCH_FUNC(0x478a20)
void CollisionCounters::ResetCount()
{
    mTestCount = 0;
}

MATCH_FUNC(0x478a30)
CollisionCounters::CollisionCounters()
{
    mTestCount = 0;
    mQueryId = 0;
    
    mbDoorAccessCheck = 0;

    gGridSpriteLink_Pool = new GridSpriteLink_Pool();
    if (!gGridSpriteLink_Pool)
    {
        FatalError(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\collide.cpp", 1416);
    }

    gGridCell_Pool = new GridCell_Pool();
    if (!gGridCell_Pool)
    {
        FatalError(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\collide.cpp", 1418);
    }

    gSpriteGrid_1 = new SpriteGrid();
    if (!gSpriteGrid_1)
    {
        FatalError(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\collide.cpp", 1420);
    }

    gSpriteGrid_2 = new SpriteGrid();
    if (!gSpriteGrid_2)
    {
        FatalError(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\collide.cpp", 1422);
    }

    gSpriteGrid_3 = new SpriteGrid();
    if (!gSpriteGrid_3)
    {
        FatalError(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\collide.cpp", 1424);
    }

    gSpriteGrid_exclusion_sprite = NULL;
}

MATCH_FUNC(0x478bf0)
CollisionCounters::~CollisionCounters()
{
    GTA2_DELETE_AND_NULL(gSpriteGrid_1);
    GTA2_DELETE_AND_NULL(gSpriteGrid_2);
    GTA2_DELETE_AND_NULL(gSpriteGrid_3);
    GTA2_DELETE_AND_NULL(gGridSpriteLink_Pool);
    GTA2_DELETE_AND_NULL(gGridCell_Pool);
}
