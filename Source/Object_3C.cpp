#include "Object_3C.hpp"
#include "sprite.hpp"

STUB_FUNC(0x52ad80)
void Object_3C::ctor_52AD80()
{
}

STUB_FUNC(0x52ade0)
void Object_3C::dtor_52ADE0()
{
}

STUB_FUNC(0x52adf0)
u32* Object_3C::sub_52ADF0(u32* a2)
{
    return 0;
}

MATCH_FUNC(0x5a6cd0)
void Object_3C::sub_5A6CD0(Sprite* pSprite)
{
    Sprite_18* p18 = gSprite_1C24_703B80->field_0;
    gSprite_1C24_703B80->field_0 = gSprite_1C24_703B80->field_0->field_4_next;
    p18->field_0 = pSprite;
    p18->field_4_next = this->field_0;
    p18->field_8 = 0;
    p18->field_C = 0;
    this->field_0 = p18;
}

STUB_FUNC(0x5a6e10)
s32 Object_3C::sub_5A6E10()
{
    return 0;
}

STUB_FUNC(0x5a6e40)
s32 Object_3C::sub_5A6E40(s32 a2, s32 a3)
{
    return 0;
}

STUB_FUNC(0x5a6f70)
void Object_3C::sub_5A6F70(Sprite* a2)
{
}

STUB_FUNC(0x5a7010)
void Object_3C::sub_5A7010()
{
}

STUB_FUNC(0x5A7080)
void Object_3C::sub_5A7080()
{
}

STUB_FUNC(0x5A7110)
void Object_3C::sub_5A7110()
{
}

STUB_FUNC(0x5a7240)
void Object_3C::sub_5A7240(Sprite* a2)
{
}

STUB_FUNC(0x5a72b0)
s32* Object_3C::sub_5A72B0(Sprite* a2, char_type a3)
{
    return 0;
}

STUB_FUNC(0x5a7310)
char_type Object_3C::sub_5A7310()
{
    return 0;
}
