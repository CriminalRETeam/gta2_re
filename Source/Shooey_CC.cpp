#include "Shooey_CC.hpp"
#include "Globals.hpp"
#include "char.hpp"
#include "cool_nash_0x294.hpp"

EXPORT_VAR s32 dword_67A370;
GLOBAL(dword_67A370, 0x67A370);

MATCH_FUNC(0x484cb0)
Shooey_14::Shooey_14()
{
    field_0 = 0;
    field_4 = 0;
    field_8.x = dword_67A370;
    field_8.y = dword_67A370;
    field_8.z = dword_67A370;
}

MATCH_FUNC(0x484ce0)
Shooey_14::~Shooey_14()
{
}

STUB_FUNC(0x484cf0)
void Shooey_14::sub_484CF0(s32 a2, s32 a3)
{
    field_0 = a2;
    field_4 = a3;
    if (!a3)
    {
        field_8.x = dword_67A370;
        field_8.y = dword_67A370;
        field_8.z = dword_67A370;
    }
    else
    {
        cool_nash_0x294* pPed = gChar_C_6787BC->sub_4710C0(a3);
        field_8.x = pPed->field_1AC_cam.x;
        field_8.y = pPed->field_1AC_cam.y; // TODO: Regswap
        field_8.z = pPed->field_1AC_cam.z;
    }
}

MATCH_FUNC(0x484d50)
void Shooey_14::sub_484D50(s32* a2, Fix16* a3, Fix16* a4, Fix16* a5)
{
    *a2 = field_0;
    *a3 = field_8.x;
    *a4 = field_8.y;
    *a5 = field_8.z;
}

MATCH_FUNC(0x484d80)
Shooey_CC::Shooey_CC()
{
    field_0 = 0;
    field_2 = 0;
}

MATCH_FUNC(0x484db0)
Shooey_CC::~Shooey_CC()
{
}

MATCH_FUNC(0x484dd0)
void Shooey_CC::sub_484DD0(s32 a2, s32 a3)
{
    field_4[field_0].sub_484CF0(a2, a3);

    field_0++;

    if (field_0 >= GTA2_COUNTOF(field_4))
    {
        field_0 = 0;
    }

    if (field_0 == field_2)
    {
        field_2++;
        if (field_2 >= GTA2_COUNTOF(field_4))
        {
            field_2 = 0;
        }
    }
}

STUB_FUNC(0x484e20)
bool Shooey_CC::sub_484E20(s32* a2, s32* a3, s32* a4, u32* a5)
{
    return 0;
}

STUB_FUNC(0x484e90)
char_type Shooey_CC::sub_484E90(s32 a2)
{
    return 0;
}

STUB_FUNC(0x484fc0)
Shooey_CC* Shooey_CC::ctor_484FC0()
{
    return 0;
}

STUB_FUNC(0x484fd0)
void Shooey_CC::dtor_484FD0()
{
}

STUB_FUNC(0x484fe0)
void Shooey_CC::sub_484FE0(u32 a2, cool_nash_0x294* pPed)
{
}

STUB_FUNC(0x485090)
bool Shooey_CC::sub_485090(Car_BC* a2, angry_lewin_0x85C* a3)
{
    return 0;
}

STUB_FUNC(0x4850f0)
char_type Shooey_CC::sub_4850F0(Shooey_CC* a1, s32 a2, angry_lewin_0x85C* a3)
{
    return 0;
}

STUB_FUNC(0x485140)
char_type Shooey_CC::sub_485140(cool_nash_0x294* a2, angry_lewin_0x85C* a3)
{
    return 0;
}
