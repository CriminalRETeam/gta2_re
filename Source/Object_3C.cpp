#include "Object_3C.hpp"
#include "Globals.hpp"
#include "Object_5C.hpp"
#include "rng.hpp"
#include "sprite.hpp"

// TODO: Init to correct values
EXPORT_VAR Fix16 dword_6F8BF0;
GLOBAL(dword_6F8BF0, 0x6F8BF0);

EXPORT_VAR Ang16 word_6F8F68;
GLOBAL(word_6F8F68, 0x6F8F68);

MATCH_FUNC(0x52ad80)
Object_3C::Object_3C()
{
    this->field_0 = 0;
    this->field_4 = 0;
    this->field_8 = 0;
    this->field_1C = dword_6F8BF0;
    this->field_18 = dword_6F8BF0;
    this->field_10 = dword_6F8BF0;
    this->field_C = dword_6F8BF0;
    this->field_4 = word_6F8F68;
    this->field_28 = 0;
    this->field_20 = 0;
    this->field_2C = 0;
    this->field_2A = 0;
    this->field_38 = 0;
    this->field_34 = 2;
    this->field_2E = 0;
    this->field_2F = 0;
}

MATCH_FUNC(0x52ade0)
Object_3C::~Object_3C()
{
    field_8 = 0;
}

STUB_FUNC(0x52adf0)
u32* Object_3C::sub_52ADF0(u32* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x5a6a50)
Sprite_18* Object_3C::sub_5A6A50(s32 obj_type)
{
    Sprite_18* pIter = this->field_0;
    while (pIter)
    {
        const s32 sprite_type_enum = pIter->field_0->field_30_sprite_type_enum;
        if (sprite_type_enum == sprite_types_enum::code_obj1 || sprite_type_enum == sprite_types_enum::map_obj ||
            sprite_type_enum == sprite_types_enum::unknown_1)
        {
            Object_5C* o5c = pIter->field_0->field_8_o5C;
            if (o5c)
            {
                if (o5c->field_18 == obj_type)
                {
                    return pIter;
                }
            }
        }
        pIter = pIter->field_4_next;
    }
    return 0;
}

MATCH_FUNC(0x5a6a90)
Object_2C* Object_3C::sub_5A6A90(s32 obj_type)
{
    Sprite_18* pIter = this->field_0;
    while (pIter)
    {
        const s32 sprite_type_enum = pIter->field_0->field_30_sprite_type_enum;
        if (sprite_type_enum == sprite_types_enum::code_obj1 || sprite_type_enum == sprite_types_enum::map_obj ||
            sprite_type_enum == sprite_types_enum::unknown_1)
        {
            Object_2C* p2C = pIter->field_0->field_8_object_2C_ptr;
            if (p2C)
            {
                if (p2C->field_18_model == obj_type)
                {
                    return p2C;
                }
            }
        }
        pIter = pIter->field_4_next;
    }
    return 0;
}

MATCH_FUNC(0x5a6ad0)
Sprite_18* Object_3C::sub_5A6AD0()
{
    Sprite_18* pObjIter = this->field_0;
    while (pObjIter)
    {
        const s32 sprite_type = pObjIter->field_0->field_30_sprite_type_enum;
        if (sprite_type == 4 || sprite_type == 5 || sprite_type == 1)
        {
            Object_2C* o2c = pObjIter->field_0->field_8_object_2C_ptr;
            if (o2c)
            {
                if (o2c->sub_525AC0())
                {
                    return pObjIter;
                }
            }
        }
        pObjIter = pObjIter->field_4_next;
    }
    return 0;
}

MATCH_FUNC(0x5a6b10)
void Object_3C::sub_5A6B10(Sprite* toFind)
{
    Sprite_18* pLast = 0;
    Sprite_18* pIter = this->field_0;
    for (;;)
    {
        if (pIter->field_0 == toFind)
        {
            if (pLast)
            {
                pLast->field_4_next = pIter->field_4_next;
                gSprite_1C24_703B80->DeAlloc(pIter);
            }
            else
            {
                this->field_0 = pIter->field_4_next;
                gSprite_1C24_703B80->DeAlloc(pIter);
            }
            return;
        }
        pLast = pIter;
        pIter = pIter->field_4_next;
    }
}

MATCH_FUNC(0x5a6b60)
void Object_3C::sub_5A6B60(Sprite* toFind)
{
    Sprite_18* pLast = 0;
    Sprite_18* pIter = this->field_0;
    while (pIter)
    {
        if (pIter->field_0 == toFind)
        {
            if (pLast)
            {
                pLast->field_4_next = pIter->field_4_next;
            }
            else
            {
                this->field_0 = pIter->field_4_next;
            }

            gSprite_1C24_703B80->DeAlloc(pIter);
            return;
        }
        pLast = pIter;
        pIter = pIter->field_4_next;
    }
}

MATCH_FUNC(0x5a6bb0)
void Object_3C::sub_5A6BB0()
{
    for (Sprite_18* p18Iter = this->field_0; p18Iter; p18Iter = p18Iter->field_4_next)
    {
        p18Iter->sub_5A69E0();
    }
}

MATCH_FUNC(0x5a6bd0)
void Object_3C::sub_5A6BD0()
{
    for (Sprite_18* p18Iter = this->field_0; p18Iter; p18Iter = p18Iter->field_4_next)
    {
        p18Iter->sub_5A6A20();
    }
}

MATCH_FUNC(0x5a6bf0)
void Object_3C::sub_5A6BF0(Sprite* pSprite)
{
    for (Sprite_18* p18Iter = this->field_0; p18Iter; p18Iter = p18Iter->field_4_next)
    {
        p18Iter->field_0->sub_59E910(pSprite);
    }
}

MATCH_FUNC(0x5a6c10)
char_type Object_3C::sub_5A6C10(Sprite* toFind)
{
    if (field_0 != NULL)
    {
        for (Sprite_18* pNext = field_0; pNext != NULL; pNext = pNext->field_4_next)
        {
            if (pNext->field_0 == toFind)
            {
                pNext->field_14_rng = rng_dword_67AB34->field_0_rng;
                return 1;
            }
        }
    }
    return 0;
}

MATCH_FUNC(0x5a6c40)
void Object_3C::sub_5A6C40(s32 toFind)
{
    Sprite_18* pIter = this->field_0;
    Sprite_18* pLast = 0;

    while (pIter)
    {
        if (pIter->field_14_rng == toFind)
        {
            pLast = pIter;
            pIter = pIter->field_4_next;
        }
        else
        {
            Sprite_18* pSavedNext = pIter->field_4_next;
            if (pLast)
            {
                pLast->field_4_next = pSavedNext;
                gSprite_1C24_703B80->DeAlloc(pIter);
                pIter = pLast->field_4_next;
            }
            else
            {
                gSprite_1C24_703B80->DeAlloc(pIter);
                pIter = pSavedNext;
                this->field_0 = pSavedNext;
            }
        }
    }
}

MATCH_FUNC(0x5a6ca0)
Sprite* Object_3C::FirstSpriteOfType_5A6CA0(s32 sprite_type)
{
    if (field_0 != NULL)
    {
        for (Sprite_18* pNext = field_0; pNext != NULL; pNext = pNext->field_4_next)
        {
            if (pNext->field_0->field_30_sprite_type_enum == sprite_type)
            {
                return pNext->field_0;
            }
        }
    }
    return NULL;
}

MATCH_FUNC(0x5a6cd0)
void Object_3C::sub_5A6CD0(Sprite* pSprite)
{
    Sprite_18* p18 = gSprite_1C24_703B80->Alloc();
    p18->field_0 = pSprite;
    p18->field_4_next = this->field_0;
    p18->field_8 = 0;
    p18->field_C = 0;
    this->field_0 = p18;
}

MATCH_FUNC(0x5a6d00)
void Object_3C::sub_5A6D00(Sprite* pSprite1, s32 a3, s32 pSprite2, s16 a5)
{
    Sprite_18* p18 = gSprite_1C24_703B80->Alloc();
    p18->field_0 = pSprite1;
    p18->field_4_next = field_0;
    p18->field_8 = a3;
    p18->field_C = pSprite2;
    p18->field_10 = a5;
    field_0 = p18;
}

MATCH_FUNC(0x5a6d40)
void Object_3C::PushSprite_5A6D40(Sprite* pToFind)
{
    Sprite_18* pIter = this->field_0;
    while (pIter)
    {
        if (pIter->field_0 == pToFind)
        {
            return;
        }
        pIter = pIter->field_4_next;
    }

    Sprite_18* pNew = gSprite_1C24_703B80->Alloc();
    pNew->field_0 = pToFind;

    pNew->field_4_next = this->field_0;
    this->field_0 = pNew;
}

STUB_FUNC(0x5a6d80)
char_type Object_3C::SpriteExists_5A6D80(Sprite* pToFind)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x5a6da0)
Sprite* Object_3C::sub_5A6DA0()
{
    Sprite_18* p18 = this->field_0;
    if (!this->field_0)
    {
        return 0;
    }
    Sprite* pOld = p18->field_0;
    this->field_0 = p18->field_4_next;
    gSprite_1C24_703B80->DeAlloc(p18);
    return pOld;
}

STUB_FUNC(0x5a6dc0)
Sprite* Object_3C::sub_5A6DC0()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x5a6e10)
Sprite_18* Object_3C::sub_5A6E10()
{
    Sprite_18* pIter = this->field_0;
    while (pIter)
    {
        Sprite_18* pLast = pIter;
        pIter = pIter->field_4_next;
        gSprite_1C24_703B80->DeAlloc(pLast);
    }
    this->field_0 = 0;
    return pIter;
}

STUB_FUNC(0x5a6e40)
Sprite* Object_3C::sub_5A6E40(s32 a2, s32 a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5a6ea0)
s32 Object_3C::sub_5A6EA0(s32 a2, s32 a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5a6f70)
void Object_3C::sub_5A6F70(Sprite* a2)
{
    NOT_IMPLEMENTED;
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
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x5a7110)
void Object_3C::sub_5A7110()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x5a71a0)
s32 Object_3C::sub_5A71A0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5a71f0)
void Object_3C::sub_5A71F0()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x5a7240)
void Object_3C::sub_5A7240(Sprite* pSprite)
{
    Sprite_18* pNext = this->field_0;
    Sprite_18* pLast = 0;

    pSprite->sub_59E9C0();

    while (pNext)
    {
        if (pSprite->sub_59E590(pNext->field_0))
        {
            pLast = pNext;
            pNext = pNext->field_4_next;
        }
        else if (pLast)
        {
            pLast->field_4_next = pNext->field_4_next;
            gSprite_1C24_703B80->DeAlloc(pNext);
            pNext = pLast->field_4_next;
        }
        else
        {
            Sprite_18* pSaveNext = pNext->field_4_next;
            gSprite_1C24_703B80->DeAlloc(pNext);
            pNext = pSaveNext;
            this->field_0 = pSaveNext;
        }
    }
}

MATCH_FUNC(0x5a72b0)
void Object_3C::sub_5A72B0(Sprite* pSprite, char_type bUnknown)
{
    char start_val = pSprite->sub_5A1BD0();
    char max_val = start_val;

    Sprite_18* p18Iter;
    for (p18Iter = this->field_0; p18Iter; p18Iter = p18Iter->field_4_next)
    {
        if (p18Iter->field_0->field_30_sprite_type_enum > 1) // object_5c type
        {
            const char_type cur_val = p18Iter->field_0->sub_5A1BD0();
            if (cur_val > max_val)
            {
                max_val = cur_val;
            }
        }
    }

    if (bUnknown)
    {
        pSprite->field_39_z_col = max_val;
    }

    for (p18Iter = this->field_0; p18Iter; p18Iter = p18Iter->field_4_next)
    {
        p18Iter->field_0->field_39_z_col = max_val;
    }
}

MATCH_FUNC(0x5a7310)
char_type Object_3C::sub_5A7310()
{
    Sprite_18* p18Iter;
    for (p18Iter = this->field_0; p18Iter; p18Iter = p18Iter->field_4_next)
    {
        if (p18Iter->field_0->sub_5A1A60())
        {
            return 1;
        }
    }
    return 0;
}
