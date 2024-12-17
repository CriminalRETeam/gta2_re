#include "eager_benz.hpp"
#include <string.h>

// https://decomp.me/scratch/lON9a
STUB_FUNC(0x591bd0)
eager_benz::eager_benz()
{
    //thirsty_lamarr::sub_4920B0(&this->field_0_money);
    //thirsty_lamarr::sub_4920B0(&this->field_38_unk);
    //sad_mirzakhani::ctor_431DC0(&this->field_1A8_unk);

    this->field_74 = 1;
    this->field_75 = 1;
    this->field_368_pObj = 0;
    this->field_18C = 0;
    this->field_190 = 0;
    this->field_70 = 0;
    this->field_78 = 0;
    this->field_7C = 0;
    this->field_86 = 0;
    this->field_88 = 0;
    this->field_8A = 0;
    this->field_80 = 0;
    this->field_84 = 0;
    this->field_194 = 0;
    this->field_198_accuracy_count = 0;
    this->field_19C_reverse_count = 0;
    this->field_1A0 = 0;
    this->field_1A4 = 0;
    /*
    for (s32 i=0; i < GTA2_COUNTOF(field_8C); i++)
    {
        field_8C[i] = 0;
    }*/
    memset(this->field_8C, 0, sizeof(this->field_8C));
}

STUB_FUNC(0x591c70)
u32 eager_benz::sub_591C70()
{
    return 0;
}

MATCH_FUNC(0x5922f0)
void eager_benz::sub_5922F0(angry_lewin_0x85C* a2, s16 a3, s32 a4, s16 a5, u16 a6)
{
    field_368_pObj = a2;
    field_0_unk.sub_492110(a3, a4, a5);
    field_38_unk.sub_492110(a3, a6, a5);
}

MATCH_FUNC(0x592330)
void eager_benz::sub_592330()
{
    field_0_unk.sub_492150();
    field_38_unk.sub_492150();
    field_1A8_unk.sub_431E10(this);
    sub_592380(3);
}

MATCH_FUNC(0x592360)
thirsty_lamarr* eager_benz::sub_592360()
{
    return &field_0_unk;
}

MATCH_FUNC(0x592370)
s32 eager_benz::sub_592370()
{
    return field_0_unk.field_0;
}

STUB_FUNC(0x592380)
void eager_benz::sub_592380(char_type a2)
{
}

STUB_FUNC(0x592430)
void eager_benz::sub_592430(char_type a2)
{
}

STUB_FUNC(0x592570)
void eager_benz::sub_592570(char_type a2, s32 a3)
{
}

STUB_FUNC(0x5925b0)
s32 eager_benz::sub_5925B0(u8 a1, char_type a2)
{
    return 0;
}

STUB_FUNC(0x592620)
void eager_benz::AddCash_592620(s32 cash)
{
}

STUB_FUNC(0x592660)
void eager_benz::sub_592660(cool_nash_0x294* a2, cool_nash_0x294* a3)
{
}

STUB_FUNC(0x592dd0)
void eager_benz::sub_592DD0(Car_BC* a2, u32* a3)
{
}

STUB_FUNC(0x593030)
char_type eager_benz::sub_593030(Car_BC* a2, s16 a3)
{
    return 0;
}

STUB_FUNC(0x593150)
void eager_benz::sub_593150(Car_BC* a2, s16 a3)
{
}

STUB_FUNC(0x593220)
void eager_benz::sub_593220()
{
}

STUB_FUNC(0x593240)
void eager_benz::sub_593240(Car_BC* a2)
{
}

STUB_FUNC(0x593370)
char_type eager_benz::sub_593370(Car_BC* a2)
{
    return 0;
}

STUB_FUNC(0x593410)
void eager_benz::sub_593410(Car_BC* pCar)
{
}

STUB_FUNC(0x5934f0)
void eager_benz::UpdateAccuracyCount_5934F0(u32 a2, s32 a3, cool_nash_0x294* a4)
{
}

STUB_FUNC(0x5935b0)
thirsty_lamarr* eager_benz::sub_5935B0()
{
    return 0;
}

STUB_FUNC(0x5935c0)
s32 eager_benz::sub_5935C0()
{
    return 0;
}

STUB_FUNC(0x5935d0)
s32 eager_benz::sub_5935D0(s32 a2)
{
    return 0;
}
