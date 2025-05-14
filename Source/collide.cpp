#include "collide.hpp"
#include "PurpleDoom.hpp"
#include "error.hpp"
#include "Globals.hpp"


EXPORT_VAR Collide_8004* gCollide_8004_679200;
GLOBAL(gCollide_8004_679200, 0x679200);

EXPORT_VAR Collide_11944* gCollide_11944_679204;
GLOBAL(gCollide_11944_679204, 0x679204);

EXPORT_VAR s32 dword_678F84;
GLOBAL(dword_678F84, 0x678F84);

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

    gCollide_8004_679200 = new Collide_8004();
    if (!gCollide_8004_679200)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\collide.cpp", 1416);
    }

    gCollide_11944_679204 = new Collide_11944();
    if (!gCollide_11944_679204)
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

STUB_FUNC(0x478bf0)
Collide_C::~Collide_C()
{
    NOT_IMPLEMENTED;
    if (gPurpleDoom_1_679208)
    {
        delete gPurpleDoom_1_679208;
    }
    gPurpleDoom_1_679208 = 0;

    if (gPurpleDoom_2_67920C)
    {
        delete gPurpleDoom_2_67920C;
    }
    gPurpleDoom_2_67920C = 0;

    if (gPurpleDoom_3_679210)
    {
        delete gPurpleDoom_3_679210;
    }
    gPurpleDoom_3_679210 = 0;

    if (gCollide_8004_679200)
    {
        gCollide_8004_679200->field_0 = 0;
        delete gCollide_8004_679200;
    }
    gCollide_8004_679200 = 0;

    if (gCollide_11944_679204)
    {
        gCollide_11944_679204->field_0 = 0;
        delete gCollide_11944_679204;
    }
    gCollide_11944_679204 = 0;
}
