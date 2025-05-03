#include "Object_3C.hpp"
#include "sprite.hpp"
#include "Object_5C.hpp"

STUB_FUNC(0x52ad80)
Object_3C::Object_3C()
{
}

MATCH_FUNC(0x52ade0)
Object_3C::~Object_3C()
{
    field_8 = 0;
}

STUB_FUNC(0x52adf0)
u32* Object_3C::sub_52ADF0(u32* a2)
{
    return 0;
}

STUB_FUNC(0x5a6a50)
Sprite_18* Object_3C::sub_5A6A50(s32 a2)
{
    return 0;
}

STUB_FUNC(0x5a6a90)
Object_2C* Object_3C::sub_5A6A90(s32 obj_type)
{
    return 0;
}

STUB_FUNC(0x5a6ad0)
Sprite_18* Object_3C::sub_5A6AD0()
{
    return 0;
}

STUB_FUNC(0x5a6b10)
void Object_3C::sub_5A6B10(Sprite* a2)
{
}

STUB_FUNC(0x5a6b60)
void Object_3C::sub_5A6B60(Sprite* a2)
{
}

STUB_FUNC(0x5a6bb0)
void Object_3C::sub_5A6BB0()
{
}

STUB_FUNC(0x5a6bd0)
void Object_3C::sub_5A6BD0()
{
}

STUB_FUNC(0x5a6bf0)
void Object_3C::sub_5A6BF0(Sprite* pSprite)
{
}

STUB_FUNC(0x5a6c10)
char_type Object_3C::sub_5A6C10(Sprite* a2)
{
    return 0;
}

STUB_FUNC(0x5a6c40)
void Object_3C::sub_5A6C40(s32 a2)
{
}

STUB_FUNC(0x5a6ca0)
Sprite* Object_3C::FirstSpriteOfType_5A6CA0(s32 sprite_type)
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

STUB_FUNC(0x5a6d00)
void Object_3C::sub_5A6D00(Sprite* pSprite1, s32 a3, Sprite* pSprite2, s16 a5)
{
}

STUB_FUNC(0x5a6d40)
void Object_3C::PushSprite_5A6D40(Sprite* pToFind)
{
}

STUB_FUNC(0x5a6d80)
char_type Object_3C::SpriteExists_5A6D80(Sprite* pToFind)
{
    return 0;
}

STUB_FUNC(0x5a6da0)
Sprite* Object_3C::sub_5A6DA0()
{
    return 0;
}

STUB_FUNC(0x5a6dc0)
Sprite* Object_3C::sub_5A6DC0()
{
    return 0;
}

STUB_FUNC(0x5a6e10)
Sprite_18* Object_3C::sub_5A6E10()
{
    return 0;
}

STUB_FUNC(0x5a6e40)
Sprite* Object_3C::sub_5A6E40(s32 a2, s32 a3)
{
    return 0;
}

STUB_FUNC(0x5a6ea0)
s32 Object_3C::sub_5A6EA0(s32 a2, s32 a3)
{
    return 0;
}

STUB_FUNC(0x5a6f70)
void Object_3C::sub_5A6F70(Sprite* a2)
{
}

MATCH_FUNC(0x5a7010)
void Object_3C::sub_5A7010()
{
    Sprite_18* p18Iter = this->field_0;
    while (p18Iter)
    {
        Sprite* pSprite = p18Iter->field_0;
        switch (p18Iter->field_0->field_30_sprite_type_enum)
        {
            case sprite_types_enum::unknown_1:
            case sprite_types_enum::code_obj1:
            case sprite_types_enum::map_obj:
                gObject_5C_6F8F84->sub_52A610(pSprite->field_8_object_2C_ptr);
                break;
            case sprite_types_enum::car:
                gCar_6C_677930->sub_446730(pSprite->field_8_car_bc_ptr);
                break;
            default:
                break;
        }
        p18Iter = p18Iter->field_4_next;
    }
    sub_5A6E10();
}

STUB_FUNC(0x5a7080)
void Object_3C::sub_5A7080()
{
}

STUB_FUNC(0x5a7110)
void Object_3C::sub_5A7110()
{
}

STUB_FUNC(0x5a71a0)
s32 Object_3C::sub_5A71A0()
{
    return 0;
}

STUB_FUNC(0x5a71f0)
void Object_3C::sub_5A71F0()
{
}

STUB_FUNC(0x5a7240)
void Object_3C::sub_5A7240(Sprite* a2)
{
}

STUB_FUNC(0x5a72b0)
void Object_3C::sub_5A72B0(Sprite* pSprite, char_type bUnknown)
{
}

STUB_FUNC(0x5a7310)
char_type Object_3C::sub_5A7310()
{
    return 0;
}
