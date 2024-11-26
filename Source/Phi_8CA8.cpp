#include "Phi_8CA8.hpp"
#include "gtx_0x106C.hpp"
#include <memory.h>

MATCH_FUNC(0x4bdf60)
Phi_74::~Phi_74()
{
}

STUB_FUNC(0x533060)
s32 Phi_74::sub_533060(s32 a2, s32 a3, s32 a4)
{
    return 0;
}

STUB_FUNC(0x533090)
s32 Phi_74::sub_533090()
{
    return 0;
}

STUB_FUNC(0x533110)
void Phi_74::sub_533110(s16 a2)
{
}

STUB_FUNC(0x533150)
s16 Phi_74::sub_533150(s16 a2, char_type a3)
{
    return 0;
}

STUB_FUNC(0x533170)
Car_3C* Phi_74::sub_533170()
{
    return 0;
}

STUB_FUNC(0x5331a0)
s32 Phi_74::sub_5331A0(Car_3C* a2)
{
    return 0;
}

// fix16?
s32 dword_6F8FA4 = 0x3FC000;
s32 dword_6FCE08 = 0;

MATCH_FUNC(0x533220)
Phi_74::Phi_74()
{
    field_0 = dword_6F8FA4;
    field_4 = dword_6F8FA4;
    field_8 = dword_6F8FA4;
    field_24_idx = 0;
    field_28 = 0;
    field_2C = 0;
    field_30 = 0;
    field_34 = 0;
    field_38 = 0;
    field_3C = 0;
    field_40 = 0;
    field_44 = 0;
    field_48 = 0;
    field_4C = 1;
    field_50 = 1;
    field_58 = 0;
    field_54 = 0;
    field_C = dword_6F8FA4;
    field_10 = dword_6F8FA4;
    field_14 = dword_6F8FA4;
    field_1C = 9999;
    field_61 = 0;
    field_64 = 99;
    field_65 = 99;
    field_1E = 99;
    field_6C = 99;
    s32 v1 = dword_6FCE08;
    field_68 = 0;
    field_18 = v1;
    field_20 = 0;
    field_60 = 0;
    field_5C = 1;
    field_70 = 0;
}

MATCH_FUNC(0x4bdf40)
Phi_8CA8::~Phi_8CA8()
{
}

MATCH_FUNC(0x5332d0)
Phi_74* Phi_8CA8::sub_5332D0(s32 idx, s32 a3, s16 a4, u8 a5)
{
    Phi_74* result = sub_5343C0(idx);
    result->field_28 = a3;
    result->field_1E = a4;
    result->field_6C = a5;
    result->field_30 = 2;
    return result;
}

STUB_FUNC(0x533300)
object_info* Phi_8CA8::sub_533300()
{
    return 0;
}

STUB_FUNC(0x533360)
void Phi_8CA8::sub_533360()
{
}

STUB_FUNC(0x533420)
Phi_74* Phi_8CA8::sub_533420()
{
    return 0;
}

STUB_FUNC(0x533b30)
s32 Phi_8CA8::sub_533B30()
{
    return 0;
}

STUB_FUNC(0x533c90)
u8 Phi_8CA8::sub_533C90()
{
    return 0;
}

MATCH_FUNC(0x534270)
void Phi_8CA8::sub_534270()
{
    sub_5342F0(287);
    sub_5342F0(288);
    sub_5342F0(289);
    sub_5342F0(290);
    sub_5342F0(291);
    sub_5342F0(292);
    sub_5342F0(293);
}

MATCH_FUNC(0x5342d0)
void Phi_8CA8::sub_5342D0()
{
    field_8CA4 = sub_534360(112)->field_1E;
}

MATCH_FUNC(0x5342f0)
void Phi_8CA8::sub_5342F0(s32 idx)
{
    Phi_74* v2 = sub_534360(idx);
    u16 v3 = gGtx_0x106C_703DD4->convert_sprite_pal_5AA460(4, v2->field_1E);
    sprite_index* psprite_index = gGtx_0x106C_703DD4->get_sprite_index_5AA440(v3);
    psprite_index->sub_5ABAA0(1);
}

MATCH_FUNC(0x534330)
void Phi_8CA8::sub_534330()
{
    sub_533300();
    sub_533360();
    sub_533420();
    sub_533B30();
    sub_533C90();
    sub_5342D0();
}

MATCH_FUNC(0x534360)
Phi_74* Phi_8CA8::sub_534360(s32 idx)
{
    return field_87F4[idx];
}

MATCH_FUNC(0x534370)
Phi_74* Phi_8CA8::sub_534370(s32 idx1, s32 idx2)
{
    Phi_74* result = &field_4[field_0_next_idx];
    field_87F4[idx1] = result;
    ++field_0_next_idx;
    memcpy(result, field_87F4[idx2], sizeof(Phi_74));
    result->field_24_idx = idx1;
    return result;
}

MATCH_FUNC(0x5343c0)
Phi_74* Phi_8CA8::sub_5343C0(s32 idx)
{
    Phi_74* result = &field_4[field_0_next_idx];
    field_87F4[idx] = result;
    ++field_0_next_idx;
    result->field_24_idx = idx;
    return result;
}

STUB_FUNC(0x5343f0)
Phi_8CA8::Phi_8CA8()
{
    memset(field_87F4, 0, sizeof(field_87F4));
    field_0_next_idx = 0;
    field_8CA4 = 99;
}
