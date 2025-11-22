#include "collide.hpp"
#include "PurpleDoom.hpp"
#include "error.hpp"
#include "Globals.hpp"

DEFINE_GLOBAL(T_Collide_8_Pool*, gCollide_8_Pool_679200, 0x679200);
DEFINE_GLOBAL(PurpleDoom_C_Pool*, gPurpleDoom_C_Pool_679204, 0x679204);
DEFINE_GLOBAL(s32, dword_678F84, 0x678F84);

MATCH_FUNC(0x478a20)
void Collide_C::sub_478A20()
{
    field_0_count = 0;
}

MATCH_FUNC(0x478a30)
Collide_C::Collide_C()
{
    field_0_count = 0;
    field_4_count = 0;
    
    field_8_bUnknown = 0;

    gCollide_8_Pool_679200 = new T_Collide_8_Pool();
    if (!gCollide_8_Pool_679200)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\collide.cpp", 1416);
    }

    gPurpleDoom_C_Pool_679204 = new PurpleDoom_C_Pool();
    if (!gPurpleDoom_C_Pool_679204)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\collide.cpp", 1418);
    }

    gPurpleDoom_1_679208 = new PurpleDoom();
    if (!gPurpleDoom_1_679208)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\collide.cpp", 1420);
    }

    gPurpleDoom_2_67920C = new PurpleDoom();
    if (!gPurpleDoom_2_67920C)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\collide.cpp", 1422);
    }

    gPurpleDoom_3_679210 = new PurpleDoom();
    if (!gPurpleDoom_3_679210)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\collide.cpp", 1424);
    }

    dword_678F84 = 0;
}

MATCH_FUNC(0x478bf0)
Collide_C::~Collide_C()
{
    GTA2_DELETE_AND_NULL(gPurpleDoom_1_679208);
    GTA2_DELETE_AND_NULL(gPurpleDoom_2_67920C);
    GTA2_DELETE_AND_NULL(gPurpleDoom_3_679210);
    GTA2_DELETE_AND_NULL(gCollide_8_Pool_679200);
    GTA2_DELETE_AND_NULL(gPurpleDoom_C_Pool_679204);
}