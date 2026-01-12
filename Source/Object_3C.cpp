#include "Object_3C.hpp"
#include "Car_BC.hpp"
#include "Globals.hpp"
#include "Object_5C.hpp"
#include "Wolfy_3D4.hpp"
#include "enums.hpp"
#include "rng.hpp"
#include "sprite.hpp"

// TODO: Init to correct values
DEFINE_GLOBAL(Fix16, dword_6F8BF0, 0x6F8BF0);
DEFINE_GLOBAL(Ang16, kZeroAng_6F8F68, 0x6F8F68);
DEFINE_GLOBAL(struct_4, stru_67727C, 0x67727C);

MATCH_FUNC(0x52ad80)
Object_3C::Object_3C()
{
    this->field_4 = 0;
    this->mpNext = 0;
    this->field_1C = dword_6F8BF0;
    this->field_18 = dword_6F8BF0;
    this->field_10 = dword_6F8BF0;
    this->field_C = dword_6F8BF0;
    this->field_4 = kZeroAng_6F8F68;
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
    mpNext = 0;
}

WIP_FUNC(0x52adf0)
Fix16_Point Object_3C::GetRot_52ADF0()
{
    WIP_IMPLEMENTED;

    return Fix16_Point(Ang16::sine_40F500(field_4) * field_C, Ang16::cosine_40F520(field_4) * field_C);
}

MATCH_FUNC(0x5a6a50)
Sprite_18* struct_4::GetSpriteForModel_5A6A50(s32 obj_type)
{
    Sprite_18* pIter = this->field_0_p18;
    while (pIter)
    {
        const s32 sprite_type_enum = pIter->field_0->field_30_sprite_type_enum;
        if (sprite_type_enum == sprite_types_enum::code_obj1 || sprite_type_enum == sprite_types_enum::map_obj ||
            sprite_type_enum == sprite_types_enum::unknown_1)
        {
            Object_2C* o5c = pIter->field_0->field_8_object_2C_ptr;
            if (o5c)
            {
                if (o5c->field_18_model == obj_type)
                {
                    return pIter;
                }
            }
        }
        pIter = pIter->mpNext;
    }
    return 0;
}

MATCH_FUNC(0x5a6a90)
Object_2C* struct_4::sub_5A6A90(s32 obj_type)
{
    Sprite_18* pIter = this->field_0_p18;
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
        pIter = pIter->mpNext;
    }
    return 0;
}

MATCH_FUNC(0x5a6ad0)
Sprite_18* struct_4::sub_5A6AD0()
{
    Sprite_18* pObjIter = this->field_0_p18;
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
        pObjIter = pObjIter->mpNext;
    }
    return 0;
}

MATCH_FUNC(0x5a6b10)
void struct_4::sub_5A6B10(Sprite* toFind)
{
    Sprite_18* pLast = 0;
    Sprite_18* pIter = this->field_0_p18;
    for (;;)
    {
        if (pIter->field_0 == toFind)
        {
            if (pLast)
            {
                pLast->mpNext = pIter->mpNext;
                gSprite_18_Pool_703B80->DeAllocate(pIter);
            }
            else
            {
                this->field_0_p18 = pIter->mpNext;
                gSprite_18_Pool_703B80->DeAllocate(pIter);
            }
            return;
        }
        pLast = pIter;
        pIter = pIter->mpNext;
    }
}

MATCH_FUNC(0x5a6b60)
void struct_4::sub_5A6B60(Sprite* toFind)
{
    Sprite_18* pLast = 0;
    Sprite_18* pIter = this->field_0_p18;
    while (pIter)
    {
        if (pIter->field_0 == toFind)
        {
            if (pLast)
            {
                pLast->mpNext = pIter->mpNext;
            }
            else
            {
                this->field_0_p18 = pIter->mpNext;
            }

            gSprite_18_Pool_703B80->DeAllocate(pIter);
            return;
        }
        pLast = pIter;
        pIter = pIter->mpNext;
    }
}

MATCH_FUNC(0x5a6bb0)
void struct_4::sub_5A6BB0()
{
    for (Sprite_18* p18Iter = this->field_0_p18; p18Iter; p18Iter = p18Iter->mpNext)
    {
        p18Iter->sub_5A69E0();
    }
}

MATCH_FUNC(0x5a6bd0)
void struct_4::sub_5A6BD0()
{
    for (Sprite_18* p18Iter = this->field_0_p18; p18Iter; p18Iter = p18Iter->mpNext)
    {
        p18Iter->sub_5A6A20();
    }
}

MATCH_FUNC(0x5a6bf0)
void struct_4::sub_5A6BF0(Sprite* pSprite)
{
    for (Sprite_18* p18Iter = this->field_0_p18; p18Iter; p18Iter = p18Iter->mpNext)
    {
        p18Iter->field_0->ProcessCarToCarImpactIfCar_59E910(pSprite);
    }
}

MATCH_FUNC(0x5a6c10)
char_type struct_4::sub_5A6C10(Sprite* toFind)
{
    if (field_0_p18 != NULL)
    {
        for (Sprite_18* pNext = field_0_p18; pNext != NULL; pNext = pNext->mpNext)
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
void struct_4::sub_5A6C40(s32 toFind)
{
    Sprite_18* pIter = this->field_0_p18;
    Sprite_18* pLast = 0;

    while (pIter)
    {
        if (pIter->field_14_rng == toFind)
        {
            pLast = pIter;
            pIter = pIter->mpNext;
        }
        else
        {
            Sprite_18* pSavedNext = pIter->mpNext;
            if (pLast)
            {
                pLast->mpNext = pSavedNext;
                gSprite_18_Pool_703B80->DeAllocate(pIter);
                pIter = pLast->mpNext;
            }
            else
            {
                gSprite_18_Pool_703B80->DeAllocate(pIter);
                pIter = pSavedNext;
                this->field_0_p18 = pSavedNext;
            }
        }
    }
}

MATCH_FUNC(0x5a6ca0)
Sprite* struct_4::FirstSpriteOfType_5A6CA0(s32 sprite_type)
{
    if (field_0_p18 != NULL)
    {
        for (Sprite_18* pNext = field_0_p18; pNext != NULL; pNext = pNext->mpNext)
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
void struct_4::sub_5A6CD0(Sprite* pSprite)
{
    Sprite_18* p18 = gSprite_18_Pool_703B80->Allocate();
    p18->field_0 = pSprite;
    p18->mpNext = this->field_0_p18;
    p18->field_8.reset();
    field_0_p18 = p18;
}

MATCH_FUNC(0x5a6d00)
void struct_4::PushImpactEvent_5A6D00(Sprite* pSprite1, Fix16 x, Fix16 y, Ang16 angle)
{
    Sprite_18* p18 = gSprite_18_Pool_703B80->Allocate();
    p18->field_0 = pSprite1;
    p18->mpNext = field_0_p18;
    p18->field_8.x = x;
    p18->field_8.y = y;
    p18->field_10 = angle;
    field_0_p18 = p18;
}

MATCH_FUNC(0x5a6d40)
void struct_4::PushSprite_5A6D40(Sprite* pToFind)
{
    Sprite_18* pIter = this->field_0_p18;
    while (pIter)
    {
        if (pIter->field_0 == pToFind)
        {
            return;
        }
        pIter = pIter->mpNext;
    }

    Sprite_18* pNew = gSprite_18_Pool_703B80->Allocate();
    pNew->field_0 = pToFind;

    pNew->mpNext = this->field_0_p18;
    this->field_0_p18 = pNew;
}

MATCH_FUNC(0x5a6d80)
char_type struct_4::SpriteExists_5A6D80(Sprite* pToFind)
{
    Sprite_18* pIter = this->field_0_p18;
    while (pIter)
    {
        if (pIter->field_0 == pToFind)
        {
            return 1;
        }
        pIter = pIter->mpNext;
    }
    return 0;
}

MATCH_FUNC(0x5a6da0)
Sprite* struct_4::sub_5A6DA0()
{
    Sprite_18* p18 = this->field_0_p18;
    if (!this->field_0_p18)
    {
        return 0;
    }
    Sprite* pOld = p18->field_0;
    this->field_0_p18 = p18->mpNext;
    gSprite_18_Pool_703B80->DeAllocate(p18);
    return pOld;
}

MATCH_FUNC(0x5a6dc0)
Sprite* struct_4::sub_5A6DC0()
{
    Sprite_18* pIter = this->field_0_p18;
    Sprite_18* pLast = 0;
    while (pIter->mpNext)
    {
        pLast = pIter;
        pIter = pIter->mpNext;
    }

    Sprite* result = pIter->field_0;
    if (pLast)
    {
        pLast->mpNext = 0;
    }
    else
    {
        this->field_0_p18 = 0;
    }
    gSprite_18_Pool_703B80->DeAllocate(pIter);

    return result;
}

MATCH_FUNC(0x5a6e10)
Sprite_18* struct_4::sub_5A6E10()
{
    Sprite_18* pIter = this->field_0_p18;
    while (pIter)
    {
        Sprite_18* pLast = pIter;
        pIter = pIter->mpNext;
        gSprite_18_Pool_703B80->DeAllocate(pLast);
    }
    this->field_0_p18 = 0;
    return pIter;
}

MATCH_FUNC(0x5a6e40)
Sprite* struct_4::sub_5A6E40(Fix16 xOff, Fix16 yOff)
{
    Fix16 smallest(99999);

    Sprite* new_ret = 0;
    for (Sprite_18* pIter = this->field_0_p18; pIter; pIter = pIter->mpNext)
    {
        Fix16 xd = pIter->field_0->field_14_xpos.x - xOff;
        Fix16 yd = pIter->field_0->field_14_xpos.y - yOff;
        Fix16 yDelta = Fix16::Abs(yd);
        Fix16 xDelta = Fix16::Abs(xd);

        if (xDelta > yDelta)
        {
            yDelta = xDelta;
        }

        if (yDelta < smallest)
        {
            new_ret = pIter->field_0;
            smallest = yDelta;
        }
    }
    return new_ret;
}

STUB_FUNC(0x5a6ea0)
Sprite* struct_4::TakeClosestSprite_5A6EA0(Fix16 xpos, Fix16 ypos)
{
    NOT_IMPLEMENTED;
    return 0;
}

// https://decomp.me/scratch/PB56K
STUB_FUNC(0x5a6f70)
void struct_4::PoolUpdate_5A6F70(Sprite* pSprite)
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x5a7010)
void struct_4::sub_5A7010()
{
    Sprite_18* p18Iter = this->field_0_p18;
    while (p18Iter)
    {
        Sprite* pSprite = p18Iter->field_0;
        switch (p18Iter->field_0->field_30_sprite_type_enum)
        {
            case sprite_types_enum::car:
                gCar_6C_677930->sub_446730(pSprite->field_8_car_bc_ptr);
                break;
            case sprite_types_enum::unknown_1:
            case sprite_types_enum::code_obj1:
            case sprite_types_enum::map_obj:
                gObject_5C_6F8F84->sub_52A610(pSprite->field_8_object_2C_ptr);
                break;
            default:
                break;
        }
        p18Iter = p18Iter->mpNext;
    }
    sub_5A6E10();
}

// https://decomp.me/scratch/hQof2
STUB_FUNC(0x5a7080)
void struct_4::sub_5A7080()
{
    NOT_IMPLEMENTED;

    Sprite_18* pIter; // esi
    Sprite_18* pLastOfType; // edi
    int type; // eax
    Object_2C* o2c; // ecx
    //Sprite_18_Pool* pRoot_; // eax

    pIter = this->field_0_p18;
    pLastOfType = 0;
    while (pIter)
    {
        type = pIter->field_0->field_30_sprite_type_enum;
        if ((type == 1 || type > 3 && type <= 5))
        {
            o2c = pIter->field_0->field_8_object_2C_ptr;
            if (o2c->field_18_model == 197 || o2c->sub_525AC0())
            {
                gObject_5C_6F8F84->sub_52A610((Object_2C*)o2c); // ??
                if (pLastOfType)
                {
                    pLastOfType->mpNext = pIter->mpNext;
                    gSprite_18_Pool_703B80->DeAllocate(pIter);
                    pIter = pLastOfType->mpNext;
                }
                else
                {
                    Sprite_18* pOldNext = pIter->mpNext;
                    gSprite_18_Pool_703B80->DeAllocate(pIter);
                    pIter = pOldNext;
                    this->field_0_p18 = pOldNext;
                }
            }
        }
        pLastOfType = pIter;
        pIter = pIter->mpNext;
    }
}

MATCH_FUNC(0x5a7110)
void struct_4::sub_5A7110()
{
    Sprite_18* pIter = this->field_0_p18;
    Sprite_18* pLast = 0;
    Object_2C* p5C;
    if (pIter)
    {
        while (pIter)
        {
            const s32 type = pIter->field_0->field_30_sprite_type_enum;
            if (type == 4 || type == 5 || type == 1)
            {
                p5C = pIter->field_0->field_8_object_2C_ptr;
                if (p5C)
                {
                    if (p5C->field_18_model >= 287 && p5C->field_18_model <= 293)
                    {

                        break;
                    }
                }
            }
            pLast = pIter;
            pIter = pIter->mpNext;
            if (!pIter)
            {
                return;
            }
        }

        gObject_5C_6F8F84->sub_52A610(p5C);

        if (pLast)
        {
            pLast->mpNext = pIter->mpNext;
        }
        else
        {
            this->field_0_p18 = pIter->mpNext;
        }
        gSprite_18_Pool_703B80->DeAllocate(pIter);
    }
}

MATCH_FUNC(0x5a71a0)
s32 struct_4::sub_5A71A0()
{
    Sprite_18* p18Iter = this->field_0_p18;
    while (p18Iter)
    {
        const s32 type = p18Iter->field_0->field_30_sprite_type_enum;
        if (type == 4 || type == 5 || type == 1)
        {
            Object_2C* o5c = p18Iter->field_0->field_8_object_2C_ptr;
            if (o5c)
            {
                if (o5c->field_18_model >= 287 && o5c->field_18_model <= 293)
                {
                    return o5c->field_18_model - 286;
                }
            }
        }
        p18Iter = p18Iter->mpNext;
        if (!p18Iter)
        {
            return 0;
        }
    }
    return 0;
}

MATCH_FUNC(0x5a71f0)
void struct_4::sub_5A71F0()
{
    for (Sprite_18* p18Iter = this->field_0_p18; p18Iter; p18Iter = p18Iter->mpNext)
    {
        const s32 type = p18Iter->field_0->field_30_sprite_type_enum;
        if (type == 1 || type > 3 && type <= 5)
        {
            Object_2C* o2c = p18Iter->field_0->field_8_object_2C_ptr;
            if (o2c->field_18_model == 197 || o2c->sub_525AC0())
            {
                p18Iter->field_0->field_8_object_2C_ptr->field_C_pAny.pExplosion->field_1A = 2;
            }
        }
    }
}

MATCH_FUNC(0x5a7240)
void struct_4::sub_5A7240(Sprite* pSprite)
{
    Sprite_18* pNext = this->field_0_p18;
    Sprite_18* pLast = 0;

    pSprite->UpdateCollisionBoundsIfNeeded_59E9C0();

    while (pNext)
    {
        if (pSprite->CollisionCheck_59E590(pNext->field_0))
        {
            pLast = pNext;
            pNext = pNext->mpNext;
        }
        else if (pLast)
        {
            pLast->mpNext = pNext->mpNext;
            gSprite_18_Pool_703B80->DeAllocate(pNext);
            pNext = pLast->mpNext;
        }
        else
        {
            Sprite_18* pSaveNext = pNext->mpNext;
            gSprite_18_Pool_703B80->DeAllocate(pNext);
            pNext = pSaveNext;
            this->field_0_p18 = pSaveNext;
        }
    }
}

MATCH_FUNC(0x5a72b0)
void struct_4::sub_5A72B0(Sprite* pSprite, char_type bUnknown)
{
    char start_val = pSprite->ComputeZLayer_5A1BD0();
    char max_val = start_val;

    Sprite_18* p18Iter;
    for (p18Iter = this->field_0_p18; p18Iter; p18Iter = p18Iter->mpNext)
    {
        if (p18Iter->field_0->field_30_sprite_type_enum > 1) // object_5c type
        {
            const char_type cur_val = p18Iter->field_0->ComputeZLayer_5A1BD0();
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

    for (p18Iter = this->field_0_p18; p18Iter; p18Iter = p18Iter->mpNext)
    {
        p18Iter->field_0->field_39_z_col = max_val;
    }
}

MATCH_FUNC(0x5a7310)
char_type struct_4::sub_5A7310()
{
    Sprite_18* p18Iter;
    for (p18Iter = this->field_0_p18; p18Iter; p18Iter = p18Iter->mpNext)
    {
        if (p18Iter->field_0->sub_5A1A60())
        {
            return 1;
        }
    }
    return 0;
}