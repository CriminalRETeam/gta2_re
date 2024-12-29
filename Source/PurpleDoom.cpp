#include "PurpleDoom.hpp"
#include "Car_BC.hpp"
#include "Globals.hpp"

EXPORT_VAR PurpleDoom* gPurpleDoom_1_679208;
GLOBAL(gPurpleDoom_1_679208, 0x679208);

EXPORT_VAR PurpleDoom* gPurpleDoom_2_67920C;
GLOBAL(gPurpleDoom_2_67920C, 0x67920C);

EXPORT_VAR PurpleDoom* gPurpleDoom_3_679210;
GLOBAL(gPurpleDoom_3_679210, 0x679210);

EXPORT_VAR s32 dword_6F5F38;
GLOBAL(dword_6F5F38, 0x6F5F38);

EXPORT_VAR s32 dword_6F6108;
GLOBAL(dword_6F6108, 0x6F6108);

STUB_FUNC(0x477a40)
s32 PurpleDoom::sub_477A40()
{
    return 0;
}

STUB_FUNC(0x477ae0)
char_type PurpleDoom::sub_477AE0(Car_3C* a1)
{
    return 0;
}

MATCH_FUNC(0x477b00)
void PurpleDoom::sub_477B00(Car_3C* a1)
{
    sub_4782C0(a1->field_14_xpos.ToInt(), a1->field_18_ypos.ToInt(), a1);
}

MATCH_FUNC(0x477b20)
s32 PurpleDoom::sub_477B20(Car_3C* a1a)
{
    s32 result;
    a1a->sub_59E9C0();
    a1a->field_C_car_or_sprite->sub_5A4D90();
    s32 v3 = dword_6F6108;
    for (result = dword_6F5F38; v3 <= dword_6F5F38; v3++)
    {
        sub_4784D0(v3, a1a);
        result = dword_6F5F38;
    }
    return result;
}

STUB_FUNC(0x477b60)
s32 PurpleDoom::sub_477B60(Car_3C* a1)
{
    return 0;
}

STUB_FUNC(0x477ba0)
void PurpleDoom::sub_477BA0()
{
}

STUB_FUNC(0x477bd0)
char_type PurpleDoom::sub_477BD0(Car_3C* a2)
{
    return 0;
}

STUB_FUNC(0x477c30)
char_type PurpleDoom::sub_477C30(Car_3C* a2)
{
    return 0;
}

STUB_FUNC(0x477c90)
s32 PurpleDoom::sub_477C90(s32 a1, s32 a2, s32* a3, u8 a4, s32 a5, char_type a6)
{
    return 0;
}

STUB_FUNC(0x477e60)
s32 PurpleDoom::sub_477E60(Car_3C* a2, s32 a3)
{
    return 0;
}

STUB_FUNC(0x477f30)
char_type PurpleDoom::sub_477F30(s32* a2, char_type a3, s32 a4, Car_3C* a5, Object_3C* a6)
{
    return 0;
}

STUB_FUNC(0x477f60)
char_type PurpleDoom::sub_477F60(s32* a2, char_type a3, s32 a4, Car_3C* a5)
{
    return 0;
}

STUB_FUNC(0x478040)
PurpleDoom::PurpleDoom()
{

}

STUB_FUNC(0x478050)
PurpleDoom::~PurpleDoom()
{

}

STUB_FUNC(0x478160)
u32 PurpleDoom::sub_478160(u8 a2)
{
    return 0;
}

STUB_FUNC(0x478240)
s32 PurpleDoom::sub_478240(s32 a2, s32 a3, s32 a4, s32 a5)
{
    return 0;
}

STUB_FUNC(0x4782c0)
void PurpleDoom::sub_4782C0(s32 a2, s32 idx, Car_3C* a4)
{
}

STUB_FUNC(0x478370)
u8* PurpleDoom::sub_478370(s32 a2, s32 a3)
{
    return 0;
}

STUB_FUNC(0x478440)
char_type PurpleDoom::sub_478440(s32 a2, s32 idx, Car_3C* a4)
{
    return 0;
}

STUB_FUNC(0x4784d0)
void PurpleDoom::sub_4784D0(s32 idx, Car_3C* a3)
{
}

STUB_FUNC(0x478590)
u8* PurpleDoom::sub_478590(u32 a2)
{
    return 0;
}

STUB_FUNC(0x4785d0)
char_type PurpleDoom::sub_4785D0(u32 a2, s32* a3)
{
    return 0;
}

STUB_FUNC(0x478750)
char_type PurpleDoom::sub_478750(u32 a2, u32* a3)
{
    return 0;
}

STUB_FUNC(0x4787e0)
char_type PurpleDoom::sub_4787E0(u32 a2, u32* a3)
{
    return 0;
}

STUB_FUNC(0x478880)
s32 PurpleDoom::sub_478880(u32 a2, u32* a3)
{
    return 0;
}

STUB_FUNC(0x478950)
void PurpleDoom::sub_478950(s32 a2, s32 a3)
{
}

STUB_FUNC(0x4789f0)
PurpleDoom* PurpleDoom::sub_4789F0()
{
    return 0;
}
