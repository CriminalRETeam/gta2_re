#include "Wolfy_3D4.hpp"
#include "Globals.hpp"

// GLOBAL: 105 0x6FD5F4
DEFINE_GLOBAL(s16, gParticleInstCount_6FD5F4);
// GLOBAL: 105 0x6FD49C
DEFINE_GLOBAL(s32, dword_6FD49C);
// GLOBAL: 105 0x6FD5D4
DEFINE_GLOBAL(s16, word_6FD5D4);

// STUB: 105 0x543690
s32 Wolfy_7A8::sub_543690()
{
    NOT_IMPLEMENTED;
    return 0;
}

// STUB: 105 0x543800
Wolfy_30* Wolfy_7A8::sub_543800()
{
    NOT_IMPLEMENTED;
    return 0;
}

// FUNCTION: 105 0x5438b0
Wolfy_7A8::Wolfy_7A8()
{
    for (u8 i = 0; i < 40; i++)
    {
        field_0[i].field_4_idx = i;
        field_780[i] = 0;
    }

    gParticleInstCount_6FD5F4 = 0;
}

// FUNCTION: 105 0x5438f0
Wolfy_7A8::~Wolfy_7A8()
{
}

// FUNCTION: 105 0x5408f0
Wolfy_30::Wolfy_30()
{
    this->field_C = 0;
    this->field_20 = 0;
    this->field_22 = 0;
    this->field_4_idx = 0;
    this->field_10 = 0;
    this->field_14 = 0;
    this->field_18 = 0;
    this->field_1A = 0;
    this->field_8 = dword_6FD49C;
    this->field_20 = word_6FD5D4;
    this->field_22 = word_6FD5D4;
    this->field_24 = dword_6FD49C;
    this->field_28 = dword_6FD49C;
    this->field_C = word_6FD5D4;
    this->field_1C = 0;
    this->field_6 = 0;
    this->field_2C_ped_id = 0;
}

// FUNCTION: 105 0x540a10
Wolfy_30::~Wolfy_30()
{
    field_14 = 0;
    field_1C = 0;
}

// STUB: 105 0x540a40
void Wolfy_30::sub_540A40()
{
    NOT_IMPLEMENTED;
}

// STUB: 105 0x540d30
void Wolfy_30::sub_540D30(s32 a2, s32 a3)
{
    NOT_IMPLEMENTED;
}

// STUB: 105 0x540f90
void Wolfy_30::state_4_540F90(s32 a2, s32 a3)
{
    NOT_IMPLEMENTED;
}

// STUB: 105 0x5411e0
void Wolfy_30::sub_5411E0(s32 a2, s32 a3)
{
    NOT_IMPLEMENTED;
}

// STUB: 105 0x541430
void Wolfy_30::sub_541430(s32 a2, s32 a3)
{
    NOT_IMPLEMENTED;
}

// STUB: 105 0x541680
u32* Wolfy_30::sub_541680(u32* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

// STUB: 105 0x541710
u32* Wolfy_30::sub_541710(u32* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

// STUB: 105 0x541760
char_type Wolfy_30::sub_541760()
{
    NOT_IMPLEMENTED;
    return 0;
}

// STUB: 105 0x541850
void Wolfy_30::sub_541850(u16 a2)
{
    NOT_IMPLEMENTED;
}

// STUB: 105 0x541d60
char_type Wolfy_30::sub_541D60()
{
    NOT_IMPLEMENTED;
    return 0;
}

// STUB: 105 0x542060
char_type Wolfy_30::sub_542060()
{
    NOT_IMPLEMENTED;
    return 0;
}

// STUB: 105 0x542340
char_type Wolfy_30::sub_542340()
{
    NOT_IMPLEMENTED;
    return 0;
}

// STUB: 105 0x542790
s16 Wolfy_30::sub_542790()
{
    NOT_IMPLEMENTED;
    return 0;
}

// STUB: 105 0x542e30
void Wolfy_30::sub_542E30(char_type a2)
{
    NOT_IMPLEMENTED;
}

// STUB: 105 0x5434a0
char_type Wolfy_30::sub_5434A0(s32 a2, s32 a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

// STUB: 105 0x5435d0
char_type Wolfy_30::sub_5435D0()
{
    NOT_IMPLEMENTED;
    return 0;
}

// STUB: 105 0x543610
void Wolfy_30::sub_543610()
{
    NOT_IMPLEMENTED;
}

// STUB: 105 0x543650
void Wolfy_30::sub_543650()
{
    NOT_IMPLEMENTED;
}

// FUNCTION: 105 0x543680
void Wolfy_30::sub_543680(Object_2C* a2)
{
    this->field_14 = a2;
}

// FUNCTION: 105 0x543980
Wolfy_3D4::Wolfy_3D4()
{
    for (u8 i = 0; i < GTA2_COUNTOF(field_3C0); i++)
    {
        field_0[i].field_4_idx = i;
        field_3C0[i] = 0;
    }
    gParticleInstCount_6FD5F4 = 0;
}

// FUNCTION: 105 0x5439c0
Wolfy_3D4::~Wolfy_3D4()
{
}