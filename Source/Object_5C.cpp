#include "Object_5C.hpp"
#include "CarPhysics_B0.hpp"
#include "Car_BC.hpp"
#include "Char_Pool.hpp"
#include "Door_4D4.hpp"
#include "Game_0x40.hpp"
#include "Globals.hpp"
#include "Light_1D4CC.hpp"
#include "Object_2C_Pool.hpp"
#include "Object_3C_Pool.hpp"
#include "Object_8_Pool.hpp"
#include "Particle_8.hpp"
#include "Phi_8CA8.hpp"
#include "PurpleDoom.hpp"
#include "Rozza_C88.hpp"
#include "TrafficLights_194.hpp"
#include "Varrok_7F8.hpp"
#include "Weapon_8.hpp"
#include "Wolfy_3D4.hpp"
#include "collide.hpp"
#include "enums.hpp"
#include "error.hpp"
#include "frosty_pasteur_0xC1EA8.hpp"
#include "map_0x370.hpp"
#include "sprite.hpp"

EXTERN_GLOBAL(Varrok_7F8*, gVarrok_7F8_703398);
EXTERN_GLOBAL(Ang16, kZeroAng_6F8F68);

DEFINE_GLOBAL(Object_5C*, gObject_5C_6F8F84, 0x6F8F84);
DEFINE_GLOBAL(s32, dword_6F8F88, 0x6f8f88);
DEFINE_GLOBAL(s32, dword_6F8F90, 0x6F8F90);
DEFINE_GLOBAL_INIT(Ang16, word_6F8D62, Ang16(0x2D0), 0x6F8D62);

DEFINE_GLOBAL(Fix16_Point, stru_6F8EF0, 0x6F8EF0);
DEFINE_GLOBAL(Fix16, kFpZero_6F8E10, 0x6F8E10);
DEFINE_GLOBAL_INIT(Fix16, k_dword_6F8C9C, Fix16(0x100, 0), 0x6F8C9C);

DEFINE_GLOBAL(u8, byte_6F8C68, 0x6F8C68);
DEFINE_GLOBAL(u8, byte_6F8C4C, 0x6F8C4C);
DEFINE_GLOBAL(u8, byte_6F8F40, 0x6F8F40);
DEFINE_GLOBAL(u32, gObj3C_id_6F8E54, 0x6F8E54);
DEFINE_GLOBAL(u32, dword_6F8F18, 0x6F8F18);
DEFINE_GLOBAL(u32, dword_6F8DC0, 0x6F8DC0);
DEFINE_GLOBAL(u32, dword_6F8F0C, 0x6F8F0C);
DEFINE_GLOBAL(u8, byte_6F8EDC, 0x6F8EDC);

DEFINE_GLOBAL_INIT(Ang16, word_6F8D8C, Ang16(0x4EC), 0x6F8D8C);
DEFINE_GLOBAL_INIT(Ang16, dword_6F8D80, Ang16(0x21C), 0x6F8D80);
DEFINE_GLOBAL_INIT(Ang16, word_6F8D54, Ang16(0x384), 0x6F8D54);
DEFINE_GLOBAL_INIT(Ang16, dword_6F8CD0, Ang16(0xB4), 0x6F8CD0);

DEFINE_GLOBAL(Sprite*, dword_6F8F8C, 0x6F8F8C);
DEFINE_GLOBAL(u8, byte_6F8F94, 0x6F8F94);
DEFINE_GLOBAL_INIT(Fix16, dword_6F8F5C, Fix16(0x200, 0), 0x6F8F5C);
DEFINE_GLOBAL_INIT(Fix16, dword_6F8DA8, Fix16(0xFFFFFFF8, 0), 0x6F8DA8);
DEFINE_GLOBAL_INIT(Fix16, dword_6F8E14, Fix16(0x4000, 0), 0x6F8E14);
DEFINE_GLOBAL_INIT(Fix16, k_dword_6F8C58, Fix16(0x666, 0), 0x6F8C58);
DEFINE_GLOBAL_INIT(u8, byte_623EC4, 1, 0x623EC4);

DEFINE_GLOBAL_INIT(Fix16, dword_6F8DC8, Fix16(256, 0), 0x6F8DC8);
DEFINE_GLOBAL_INIT(Fix16, dword_6F8CE8, Fix16(0x48000, 0), 0x6F8CE8);
DEFINE_GLOBAL_INIT(Fix16, dword_6F8CEC, Fix16(1), 0x6F8CEC);
DEFINE_GLOBAL_INIT(Fix16, dword_6F8ECC, dword_6F8DC8, 0x6F8ECC);

DEFINE_GLOBAL_INIT(Ang16, word_6F8C88, Ang16(0x168), 0x6F8C88); // TODO: Init via func 0x5269F0
DEFINE_GLOBAL_INIT(Ang16, word_6F8D88, Ang16(0x438), 0x6F8D88); // TODO: Init via func 0x526E70

DEFINE_GLOBAL_INIT(Fix16, dword_6F8CF0, Fix16(0x40, 0), 0x6F8CF0);
DEFINE_GLOBAL_INIT(Fix16, k_dword_6F8BFC, Fix16(0x1FF00, 0), 0x6F8BFC);
DEFINE_GLOBAL_INIT(Fix16, dword_6F8D10, Fix16(0x2000, 0), 0x6F8D10);

DEFINE_GLOBAL_INIT(u8, byte_6771DC, 0, 0x6771DC);
DEFINE_GLOBAL_INIT(s32, gObj2C_id_623EC0, 1, 0x623EC0);

DEFINE_GLOBAL(Fix16, k_dword_6F8D38, 0x6F8D38);
DEFINE_GLOBAL(Fix16, k_dword_6F8CE0, 0x6F8CE0);
DEFINE_GLOBAL(Fix16, k_dword_6F8F74, 0x6F8F74);
DEFINE_GLOBAL(Fix16, k_dword_6F8EE4, 0x6F8EE4);

DEFINE_GLOBAL(Fix16, k_dword_6F8D3C, 0x6F8D3C);
DEFINE_GLOBAL(Fix16, k_dword_6F8BE8, 0x6F8BE8);

DEFINE_GLOBAL(Fix16, k_dword_6F8F2C, 0x6F8F2C);
DEFINE_GLOBAL(Fix16, k_dword_6F8D24, 0x6F8D24);

// TODO: From CarPhysics_B0
EXTERN_GLOBAL(Fix16_Point, CollisionIntersectionPoint_6FE1A0);

MATCH_FUNC(0x522140)
Object_2C::Object_2C()
{
    mpNext = 0;
    field_4 = 0;
    field_18_model = 0;
    field_8 = 0;
    field_C_pAny.o8 = 0;
    field_10_obj_3c = 0;
    field_14_id = 99;
    field_24_bDoneThisFrame = 0;
    field_25 = 0;
    field_26_varrok_idx = 99;
    field_20 = 0;
    field_27 = 0;
    field_28 = -51;
    field_1C = 0;
}

MATCH_FUNC(0x522180)
void Object_2C::PoolDeallocate()
{
    this->field_18_model = 0;

    // TODO: Local required, inline?
    const s32 f5C = field_8->field_5C;
    if (f5C == 2)
    {
        --gObject_5C_6F8F84->field_10_rotation_counter;
    }
    else if (f5C == 3)
    {
        --gObject_5C_6F8F84->field_14_sprites_in_list;
        gObject_5C_6F8F84->field_1C.RemoveSpriteSafe_5A6B60(this->field_4);
    }

    --dword_6F8F88;

    const s32 phi_type = this->field_8->field_34_behavior_type;
    if (phi_type != 6 && phi_type != 7 && phi_type != 8 && phi_type != 9 && phi_type != 10 && phi_type != 1 && phi_type != 12)
    {
        if (field_26_varrok_idx > 0)
        {
            gVarrok_7F8_703398->sub_59B0D0(field_26_varrok_idx);
            this->field_26_varrok_idx = 0;
        }
    }

    Object_2C::sub_527F10();
    if (field_4)
    {
        gSprite_Pool_703818->remove(field_4);
        this->field_4 = 0;
    }
    this->field_14_id = 0;
}

MATCH_FUNC(0x522250)
bool Object_2C::CanCollideWithSpriteByVarrok_522250(Sprite* pSprite)
{
    const u32 phi_type = this->field_8->field_34_behavior_type;
    if (phi_type != 6 && phi_type != 7 && phi_type != 8 && phi_type != 9 && phi_type != 10 && phi_type != 1 && phi_type != 12)
    {
        u8 varrok_idx = this->field_26_varrok_idx;
        if (varrok_idx > 0)
        {
            if (pSprite)
            {
                Char_B4* cB4 = pSprite->AsCharB4_40FEA0();
                if (cB4)
                {
                    if (cB4->field_7C_pPed->field_267_varrok_idx == varrok_idx)
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

MATCH_FUNC(0x5222b0)
s32 Object_2C::sub_5222B0()
{
    return this->field_8->field_68 != 0 ? 2048 : 1024;
}

MATCH_FUNC(0x5222d0)
void Object_2C::UpdatePhysics_5222D0()
{
    if (field_10_obj_3c->field_2A == 1)
    {
        field_10_obj_3c->field_1C += dword_6F8DA8;
        field_10_obj_3c->field_10 += field_10_obj_3c->field_1C;
    }
    else
    {
        field_10_obj_3c->field_C += field_10_obj_3c->field_18;

        if (field_10_obj_3c->field_C < kFpZero_6F8E10)
        {
            field_10_obj_3c->field_C = kFpZero_6F8E10;
            field_10_obj_3c->field_18 = kFpZero_6F8E10;
        }

        if (field_10_obj_3c->field_10 < kFpZero_6F8E10)
        {
            field_10_obj_3c->field_10 = kFpZero_6F8E10;
        }
    }
}

MATCH_FUNC(0x522340)
void Object_2C::PoolGive_522340()
{
    if (field_20 == 2)
    {
        // TODO: Missing inline here
        TObject_2C_Pool* pRoot = gObject_2C_Pool_6F8F80;
        mpNext = pRoot->field_4_pPrev;
        pRoot->field_4_pPrev = this;
        field_20 = 1;
    }
}

MATCH_FUNC(0x522360)
void Object_2C::PoolTake_522360()
{
    Object_2C* pLast;
    Object_2C* pIter;

    if (field_20 == 1)
    {
        pLast = 0;
        for (pIter = gObject_2C_Pool_6F8F80->field_4_pPrev; pIter; pIter = pIter->mpNext)
        {
            if (pIter == this)
            {
                if (pLast)
                {
                    pLast->mpNext = pIter->mpNext;
                }
                else
                {
                    gObject_2C_Pool_6F8F80->field_4_pPrev = pIter->mpNext;
                }
                pIter->mpNext = 0;
                break;
            }
            else
            {
                pLast = pIter;
            }
        }
        field_20 = 2;
    }
}

MATCH_FUNC(0x5223c0)
char Object_2C::ShouldCollideWith_5223C0(Sprite* pSprite)
{
    s32 sprite_type;

    if (!pSprite)
    {
        return 0;
    }
    switch (field_8->field_54_react_to_collisions_with)
    {
        case CollisionReaction::Always_0:
            // Always
            return true;
        case CollisionReaction::OnlyCars_1:
            // Only cars
            if (pSprite->field_30_sprite_type_enum == sprite_types_enum::car)
            {
                return false;
            }
            break;
        case CollisionReaction::OnlyPeds_2:
            // Only peds
            if (pSprite->field_30_sprite_type_enum == sprite_types_enum::ped)
            {
                return false;
            }
            break;
        case CollisionReaction::OnlyObjects_3:
            // Only objects?
            sprite_type = pSprite->field_30_sprite_type_enum;
            if (sprite_type != sprite_types_enum::code_obj1 && sprite_type != sprite_types_enum::map_obj &&
                sprite_type != sprite_types_enum::unknown_1)
            {
                return 1;
            }
            else
            {
                return 0;
            }
            break;
        case CollisionReaction::Never_4:
            // Never
            return 0;
            break;
        default:
            return 1;
            break;
    }
    return true;
}

MATCH_FUNC(0x522430)
bool Object_2C::ShouldCollideWithSprite_522430(Sprite* pSprite)
{
    return (pSprite && ShouldCollideWith_5223C0(pSprite) && !CanCollideWithSpriteByVarrok_522250(pSprite)) ? true : false;
}

MATCH_FUNC(0x522460)
char_type Object_2C::SelectCollisionSprite_522460(Sprite* a2)
{
    byte_6F8F94 = 0;

    Sprite* pSprite = a2->QuerySpriteCollision_59E7D0(2);
    if (pSprite && dword_6F8F8C && pSprite->field_30_sprite_type_enum == 2 // IsCar
        || !ShouldCollideWithSprite_522430(pSprite) || pSprite == dword_6F8F8C)
    {
        return 0;
    }

    if (pSprite->field_8_object_2C_ptr->field_18_model == 166 || pSprite->field_8_object_2C_ptr->field_18_model == 169)
    {
        byte_6F8F94 = 1;
    }
    gRozza_679188.field_20_pSprite = pSprite;
    gRozza_679188.field_0_type = 3;
    return 1;
}

WIP_FUNC(0x5224e0)
void Object_2C::SetMovementVector_5224E0(Fix16_Point* a2)
{
    WIP_IMPLEMENTED;

    if (field_10_obj_3c)
    {
        Fix16_Point v5 = (GetRot_52AE90() + *a2);
        Fix16 v5_len = v5.GetLength_2(); // TODO: Should be using kFpZero_6F8E10
        this->field_10_obj_3c->field_C = v5_len;
        this->field_10_obj_3c->field_4 = v5.atan2_40F790();
        this->field_10_obj_3c->field_18 = this->field_8->field_14_friction;
    }
    else
    {
        NewObj3C_528130(a2);
    }
}

MATCH_FUNC(0x522640)
void Object_2C::SetMovementVectorWithRandomState_522640(Fix16_Point* a2)
{
    SetMovementVector_5224E0(a2);

    if (field_8->field_4C == 3 && field_10_obj_3c->field_34 == 2)
    {
        const s16 rng = stru_6F6784.get_int_4F7AE0(9);
        if (rng < 6)
        {
            if (rng < 3)
            {
                field_10_obj_3c->field_34 = 1;
            }
            else
            {
                field_10_obj_3c->field_34 = 0;
            }
        }
    }
}

WIP_FUNC(0x5226a0)
void Object_2C::sub_5226A0(char_type a2)
{
    // TODO: Missing SEH
    WIP_IMPLEMENTED;

    if (field_10_obj_3c)
    {
        field_10_obj_3c->field_38 = a2;
    }
    else
    {
        NewObj3C_528130(&stru_6F8EF0);
        field_10_obj_3c->field_38 = a2;
    }
}

WIP_FUNC(0x522710)
void Object_2C::ResolveCollisionWithObject_522710(Object_2C* a2, Fix16_Point* a3)
{
    WIP_IMPLEMENTED;

    Fix16_Point v30 = a2->GetXY_52AE70();
    Fix16_Point v29 = GetXY_52AE70();

    Ang16 v25;
    Fix16_Point v6 = (v29 - *a3);
    Fix16_Point v26;
    v26.x = v6.x;
    v26.y = v6.y;

    Fix16_Point v28;
    if (a2->field_8->field_34_behavior_type == 3 || a2->field_8->field_34_behavior_type == 4 ||
        a2->field_8->field_34_behavior_type <= 2 && a2->field_8->field_44 == 2)
    {
        Fix16_Point v11 = (GetRot_52AE90() - a2->GetRot_52AE90());
        Fix16_Point v27;
        v27.x = v11.x;
        v27.y = v11.y;
        Fix16_Point v13 = ComputeLineLineIntersection_55F3B0(field_8->field_18,
                                                             a2->field_8->field_18,
                                                             v27,
                                                             v26,
                                                             *a3,
                                                             v29,
                                                             v30,
                                                             k_dword_6F8D38,
                                                             k_dword_6F8D38,
                                                             k_dword_6F8D24);

        v28.x = v13.x;
        v28.y = v13.y;
        Fix16_Point v17 = (-v28 / a2->field_8->field_18);
        a2->SetMovementVectorWithRandomState_522640(&v17);
    }
    else
    {
        Fix16_Point v18 = GetRot_52AE90();
        Fix16_Point v27;
        v27.x = v18.x;
        v27.y = v18.y;
        if (v26.x == kFpZero_6F8E10 && v26.y == kFpZero_6F8E10)
        {
            v26.x = v18.x;
            v26.y = v18.y;
        }

        v28 = ComputeLineLineIntersection_55F3B0(field_8->field_18,
                                                 k_dword_6F8BE8,
                                                 v27,
                                                 v26,
                                                 *a3,
                                                 v29,
                                                 v30,
                                                 k_dword_6F8D38,
                                                 k_dword_6F8D38,
                                                 k_dword_6F8F2C);
    }

    if (field_10_obj_3c)
    {
        v25 = field_10_obj_3c->field_4;
    }

    Fix16_Point v22 = (v28 / field_8->field_18);
    SetMovementVectorWithRandomState_522640(&v22);

    if (byte_6F8F94)
    {
        if (field_10_obj_3c)
        {
            if (ComputeShortestAngleDelta_4056C0(field_10_obj_3c->field_4, v25) < word_6F8C88)
            {
                field_10_obj_3c->field_4 += word_6F8D62;
            }
        }
    }

    a2->HandleImpact_528E50(field_4);
    HandleImpact_528E50(a2->field_4);
}

// 9.6f 0x4867E0
WIP_FUNC(0x5229b0)
void Object_2C::ResolveCollisionWithPed_5229B0(Char_B4* pB4, Fix16_Point* pPoint, s32 not_used)
{
    WIP_IMPLEMENTED;

    Ped* pPed = pB4->field_7C_pPed;
    Fix16_Point camPos(pPed->get_cam_x(), pPed->get_cam_y());

    //LOBYTE(seh) = 3;
    Fix16_Point spritePos = GetXY_52AE70();
    //LOBYTE(seh) = 4;
    Fix16_Point posDelta = (spritePos - *pPoint);

    Fix16_Point tmp;
    pB4->sub_545580(&tmp);
    //LOBYTE(seh) = 5;

    Fix16_Point lineHitPos = ComputeLineLineIntersection_55F3B0(field_8->field_18,
                                                                k_dword_6F8EE4,
                                                                tmp,
                                                                posDelta,
                                                                *pPoint,
                                                                spritePos,
                                                                camPos,
                                                                k_dword_6F8D38,
                                                                k_dword_6F8CE0,
                                                                k_dword_6F8F74);
    //LOBYTE(seh) = 4;
    Fix16_Point nrmHitPos = (lineHitPos / field_8->field_18); // TODO: sub_482C80
    //LOBYTE(seh) = 6;
    SetMovementVectorWithRandomState_522640(&nrmHitPos);
    //LOBYTE(seh) = 4;
    Fix16_Point v15 = (-lineHitPos);
    //LOBYTE(seh) = 7;
    Fix16_Point v16 = (v15 / k_dword_6F8EE4);
    //mValue = v16->x;
    //v18 = v16->y;
    Fix16_Point t;
    t.x = kFpZero_6F8E10;
    t.y = kFpZero_6F8E10;
    if (t.x != kFpZero_6F8E10 && t.y != kFpZero_6F8E10)
    {
        pB4->HandleCarImpact_5538A0(0, 0, kFpZero_6F8E10, kFpZero_6F8E10);
    }
    //LOBYTE(seh) = 4;
    //v21 = kFpZero_6F8E10;
    //v22 = kFpZero_6F8E10;

    HandleImpact_528E50(pB4->field_80_sprite_ptr); // TODO: sub_4338D0
}

WIP_FUNC(0x522b20)
void Object_2C::ResolveCollisionWithWorld_522B20(Fix16_Point* f18, Fix16_Point* a3, Fix16_Point* a4)
{
    WIP_IMPLEMENTED;

    Fix16_Point t;
    Fix16_Point v9;
    Fix16_Point obj_xy = GetXY_52AE70();
    v9 = ComputeLineLineIntersection_55F3B0(field_8->field_18,
                                            k_dword_6F8BE8,
                                            *a4,
                                            *a3,
                                            t,
                                            obj_xy,
                                            stru_6F8EF0,
                                            k_dword_6F8D38,
                                            kFpZero_6F8E10,
                                            k_dword_6F8D3C);
    Fix16_Point v7 = (v9 / this->field_8->field_18);
    SetMovementVectorWithRandomState_522640(&v7);
    HandleImpact_528E50(0);
}

WIP_FUNC(0x522be0)
void Object_2C::ResolveCollisionWithMapTile_522BE0(Fix16_Point* a2)
{
    WIP_IMPLEMENTED;

    Fix16_Point v13 = GetRot_52AE90();
    Fix16_Point v12;
    Fix16_Point t1;
    u8 v9;
    if (this->field_4->GetNearestHorizontalEdgeToCoordinate_5A0A70(gRozza_679188.field_18_mapy_t1, t1, v9))
    {
        v12.x = 0;
        v12.y = this->field_4->field_14_xy.y - gRozza_679188.field_18_mapy_t1;
    }
    else
    {
        Fix16 xt1d = field_4->field_14_xy.x + (gRozza_679188.field_14_mapx_t2 - a2->x);
        Fix16 ttt = (gRozza_679188.field_18_mapy_t1 - a2->y); // not used, from an inline ??

        Fix16 v7 = xt1d - gRozza_679188.field_8;
        if (v7 <= 0)
        {
            v7 = gRozza_679188.field_8 - xt1d;
        }

        Fix16 v8 = xt1d - gRozza_679188.field_4_mapx_t1;
        if (v8 <= 0)
        {
            v8 = -v8;
        }

        t1.x = gRozza_679188.field_4_mapx_t1;
        if (v8 >= v7)
        {
            t1.x = gRozza_679188.field_8;
        }

        v12 = -v13;
    }

    ResolveCollisionWithWorld_522B20(&t1, &v12, &v13);
}

STUB_FUNC(0x522d00)
void Object_2C::ResolveCollisionWithMapTileHorizontal_522D00(Fix16_Point* a2)
{
    NOT_IMPLEMENTED;
}

WIP_FUNC(0x522e10)
void Object_2C::HandleCollision_522E10(Fix16_Point* a4)
{
    WIP_IMPLEMENTED;

    switch (gRozza_679188.field_0_type)
    {
        case 1:
            ResolveCollisionWithMapTile_522BE0(a4);
            break;
        case 2:
            ResolveCollisionWithMapTileHorizontal_522D00(a4);
            break;

        case 3:
        {
            u8 a7;
            u8 a8;
            u8 a9;
            Fix16_Point v13 = field_4->FindCollisionIntersectionPoint_5A2710(gRozza_679188.field_20_pSprite, *a4, field_4->field_0, a7, a8, a9);
            Car_BC* pCar = gRozza_679188.field_20_pSprite->AsCar_40FEB0();
            if (pCar)
            {
                pCar->SetupCarPhysicsAndSpriteBinding_43BCA0();
                pCar->field_58_physics->SetCurrentCarInfoAndModelPhysics_562EF0();
                CollisionIntersectionPoint_6FE1A0 = v13;
                pCar->field_58_physics->HandleObjectCollision_5606C0(this, a9); // a4?
            }
            else
            {
                Char_B4* pChar = gRozza_679188.field_20_pSprite->AsCharB4_40FEA0();
                if (pChar)
                {
                    ResolveCollisionWithPed_5229B0(pChar, &v13, a8);
                    HandleImpact_528E50(gRozza_679188.field_20_pSprite);
                }
                else
                {
                    Object_2C* pObj = gRozza_679188.field_20_pSprite->As2C_40FEC0();
                    ResolveCollisionWithObject_522710(pObj, &v13);
                }
            }
            break;
        }
    }
}

MATCH_FUNC(0x5233a0)
char_type Object_2C::sub_5233A0(Fix16 a2)
{
    dword_6F8F8C = 0;

    if (SelectCollisionSprite_522460(this->field_4))
    {

        byte_623EC4 = 0;
        if (byte_6F8F94)
        {
            return 0;
        }

        dword_6F8F8C = gRozza_679188.field_20_pSprite;
        if (a2 > k_dword_6F8C58)
        {
            return 0;
        }

        if (this->field_10_obj_3c->field_34 == 1)
        {
            this->field_4->field_28_num = 29;
        }
        else
        {
            this->field_4->field_28_num = 6;
        }
        this->field_10_obj_3c->field_C = k_dword_6F8C58;
        this->field_10_obj_3c->field_18 = this->field_8->field_14_friction;
        return 1;
    }
    this->field_10_obj_3c->field_34 = 2;
    return 0;
}

MATCH_FUNC(0x523440)
void Object_2C::HandleCollisionOutcome_523440(Fix16_Point point, char_type bUnknown1, char_type bUnknown2)
{
    if ((u8)field_4->IsOnWater_59E1D0())
    {
        this->field_10_obj_3c->field_C = kFpZero_6F8E10;
        this->field_10_obj_3c->field_10 = kFpZero_6F8E10;
        return;
    }

    switch (this->field_8->field_4C)
    {
        case 4:
            if (gRozza_679188.field_20_pSprite)
            {
                field_4->set_z_lazy_420660(gRozza_679188.field_20_pSprite->field_1C_zpos);
            }

            if (!bUnknown1 || gRozza_679188.field_0_type == 3)
            {
                if (bUnknown2 && gRozza_679188.field_0_type != 3)
                {
                    gRozza_679188.field_0_type = 5;
                    gRozza_679188.field_20_pSprite = 0;
                }
            }
            else
            {
                gRozza_679188.field_0_type = 4;
                gRozza_679188.field_20_pSprite = 0;
            }

            HandleImpact_528E50(gRozza_679188.field_20_pSprite);
            return;

        case 0:
        case 1:
            if (bUnknown1)
            {
                gRozza_679188.field_0_type = 4;
                gRozza_679188.field_20_pSprite = 0;
            }
            else if (bUnknown2)
            {
                gRozza_679188.field_0_type = 5;
                gRozza_679188.field_20_pSprite = 0;
            }
            HandleImpact_528E50(gRozza_679188.field_20_pSprite);
            return;

        case 2:
        case 3:
            HandleCollision_522E10(&point);
            return;
        default:
            return;
    }
}

WIP_FUNC(0x5235b0)
char_type Object_2C::HandleSpriteGroundAndCollision_5235B0(Sprite* a2, Fix16_Point* a3, u8* a4, s32 a5)
{
    WIP_IMPLEMENTED;

    Sprite* v5 = a2;
    this->field_10_obj_3c->field_2A = 0;
    gmp_block_info* pBlock =
        gMap_0x370_6F6268->get_block_4DFE10(v5->field_14_xy.x.ToInt(), v5->field_14_xy.y.ToInt(), v5->field_1C_zpos.ToInt());
    if (pBlock && ((pBlock->field_B_slope_type & 0xFC) != 0) && (pBlock->field_B_slope_type & 0xFCu) < 0xB4 &&
        (pBlock->field_B_slope_type & 3) != 0)
    {
        Fix16 temp_z;
        bool v15;
        temp_z = *gMap_0x370_6F6268->sub_4E5050(&temp_z, v5->field_14_xy.x, v5->field_14_xy.y, v5->field_1C_zpos, v15);
        if (v15)
        {
            Sprite_UpdateZFromSlopeAndTile_522FA0(v5);
        }
        else if (temp_z < v5->field_1C_zpos)
        {
            Sprite_UpdateZFromSlopeAndTile_522FA0(v5);
            this->field_10_obj_3c->field_10 = dword_6F8DA8;
            this->field_10_obj_3c->field_2F = 1;
            goto LABEL_18;
        }
        this->field_10_obj_3c->field_2F = 1;
    }
    else
    {
        gmp_block_info* block_4DFE10 =
            gMap_0x370_6F6268->get_block_4DFE10(v5->field_14_xy.x.ToInt(), v5->field_14_xy.y.ToInt(), (v5->field_1C_zpos.ToInt()) - 1);
        if (block_4DFE10 && (block_4DFE10->field_B_slope_type & 3) != 0 || field_10_obj_3c->field_2F)
        {
            if (this->field_10_obj_3c->field_2F)
            {
                Sprite_UpdateZFromSlopeAndTile_522FA0(v5);
            }
            this->field_10_obj_3c->field_10 = kFpZero_6F8E10;
            this->field_10_obj_3c->field_2A = 0;
        }
        else
        {
            field_10_obj_3c->field_10 = dword_6F8DA8;
            this->field_10_obj_3c->field_2A = 1;
        }
        this->field_10_obj_3c->field_2F = 0;
    }

LABEL_18:
    if (v5->CheckSpriteMovementRegion_5A2500())
    {
        *a3 = v5->get_x_y_443580();
        *a4 = 1;
        sub_524550();
        return 1;
    }
    else if (SelectCollisionSprite_522460(v5))
    {
        this->field_10_obj_3c->field_14 = this->field_10_obj_3c->field_C.mValue;
        this->field_10_obj_3c->field_10 = kFpZero_6F8E10;
        this->field_10_obj_3c->field_2A = 0;
        *a3 = v5->get_x_y_443580();
        return 1;
    }
    else
    {
        return 0;
    }
}

WIP_FUNC(0x523770)
char_type Object_2C::HandleSpriteGroundAndCollisionSimple_523770(Sprite* pSprite, Fix16_Point* pPoint, u8* a4, u8* a5)
{
    WIP_IMPLEMENTED;

    this->field_10_obj_3c->field_2A = 1;

    if (pSprite->field_1C_zpos > k_dword_6F8BFC)
    {
        pSprite->field_1C_zpos = k_dword_6F8BFC;
        pSprite->ResetZCollisionAndDebugBoxes_59E7B0();
        this->field_10_obj_3c->field_10 = kFpZero_6F8E10;
        this->field_10_obj_3c->field_1C = kFpZero_6F8E10;
    }

    if (pSprite->sub_5A21F0())
    {
        *a5 = 1;
        this->field_10_obj_3c->field_10 = kFpZero_6F8E10;
        this->field_10_obj_3c->field_1C = kFpZero_6F8E10;
        if (this->field_8->field_50 == 4)
        {
            return 1;
        }
    }

    if (pSprite->CheckSpriteMovementRegion_5A2500())
    {
        this->field_10_obj_3c->field_14 = this->field_10_obj_3c->field_C;
        *pPoint = pSprite->get_x_y_443580();
        *a4 = 1;
        sub_524550();
        return 1;
    }
    else if (SelectCollisionSprite_522460(pSprite))
    {
        this->field_10_obj_3c->field_14 = this->field_10_obj_3c->field_C;
        if (byte_6F8F94 == 0)
        {
            *pPoint = pSprite->get_x_y_443580();
            return 1;
        }
        else
        {
            *pPoint = pSprite->get_x_y_443580();
            *a4 = 1;
            return 1;
        }
        return 0;
    }
    else
    {
        return 0;
    }
}

MATCH_FUNC(0x524550)
void Object_2C::sub_524550()
{
    if (gRozza_679188.field_0_type == 1)
    {
        if (field_10_obj_3c->field_4 >= word_6F8D88 || field_10_obj_3c->field_4 <= word_6F8C88)
        {
            dword_6F8F90 = 3;
        }
        else
        {
            dword_6F8F90 = 4;
        }
    }
    else if (gRozza_679188.field_0_type == 2)
    {
        if (field_10_obj_3c->field_4 >= word_6F8D62)
        {
            dword_6F8F90 = 2;
        }
        else
        {
            dword_6F8F90 = 1;
        }
    }
}

WIP_FUNC(0x524630)
void Object_2C::IntegrateHorizontalMovementAndCollisions_524630(Fix16 a2, Ang16 a3)
{
    WIP_IMPLEMENTED;

    bool v50 = 0;
    //v4 = a2;
    Sprite* v5 = gObject_5C_6F8F84->field_58;
    //v63 = 0;
    dword_6F8F8C = 0;
    if (a2 == kFpZero_6F8E10 && !this->field_10_obj_3c->field_2A)
    {
        return;
    }
    gRozza_679188.field_0_type = 0;
    gRozza_679188.field_20_pSprite = 0;
    gRozza_679188.field_24 = 0;

    v5->set_xyz_lazy_420600(field_4->field_14_xy.x, field_4->field_14_xy.y, field_4->field_1C_zpos);
    v5->set_ang_lazy_420690(field_4->field_0);
    v5->AllocInternal_59F950(this->field_8->field_0, this->field_8->field_4, this->field_8->field_8);
    v5->field_30_sprite_type_enum = this->field_4->field_30_sprite_type_enum;
    v5->sub_59E960();
    v5->field_8_pSprite = this->field_4->field_8_pSprite;

    Fix16 v11;
    Fix16 v52;
    if (a2 == kFpZero_6F8E10)
    {
        v11 = kFpZero_6F8E10;
        v52 = kFpZero_6F8E10;
    }
    else
    {
        v11 = (a2) / this->field_8->field_C;
        v52 = (a2) / v11;
    }

    if (v11 < dword_6F8E14)
    {
        v11 = dword_6F8E14;
        v52 = a2;
    }

    Fix16 v53 = (v52 * gSin_table_667A80[a3.rValue]);
    Fix16 v13 = (v52 * gCos_table_669260[a3.rValue]);

    u8 a2_ = 1;
    //v59.x = v13;
    s32 t = v11.ToInt();
    //    v60.x = v11.ToInt();
    if (v11.ToInt() < 1)
    {
    LABEL_56:
        field_4->set_xyz_lazy_420600(v5->field_14_xy.x, v5->field_14_xy.y, v5->field_1C_zpos);
        field_4->set_ang_lazy_420690(v5->field_0);
        return;
    }

    Fix16_Point hitPoint;

    Ang16 v55;
    Fix16 v57;
    Fix16 v61;

    gmp_block_info* block_4DFE10;
    gmp_block_info* v23;
    gmp_block_info* v25;

    while (1)
    {
        v61 = v5->field_14_xy.x;
        v55 = v5->field_0;
        v57 = v5->field_14_xy.y;
        field_10_obj_3c->field_2A = 0;

        block_4DFE10 = gMap_0x370_6F6268->get_block_4DFE10(v5->field_14_xy.x.ToInt(), v5->field_14_xy.y.ToInt(), v5->field_1C_zpos.ToInt());
        if (block_4DFE10)
        {
            if ((block_4DFE10->field_B_slope_type & 0xFC) != 0 && (block_4DFE10->field_B_slope_type & 0xFCu) < 0xB4 &&
                (block_4DFE10->field_B_slope_type & 3) != 0)
            {
                v50 = 1;
            }
        }

        v5->set_xy_lazy_447E20(v5->field_14_xy.x + v53, v5->field_14_xy.y + v13);
        v5->set_ang_lazy_420690(a3);

        v23 = gMap_0x370_6F6268->get_block_4DFE10(v5->field_14_xy.x.ToInt(), v5->field_14_xy.y.ToInt(), v5->field_1C_zpos.ToInt());
        if (v23)
        {
            if ((v23->field_B_slope_type & 0xFC) != 0 && (v23->field_B_slope_type & 0xFCu) < 0xB4 && (v23->field_B_slope_type & 3) != 0)
            {
                Sprite_UpdateZFromSlopeAndTile_522FA0(v5);
            }
        }

        v25 = gMap_0x370_6F6268->get_block_4DFE10(v5->field_14_xy.x.ToInt(),
                                                  v5->field_14_xy.y.ToInt(),
                                                  (v5->field_1C_zpos - dword_6F8E14).ToInt());
        if (!v25 || (v25->field_B_slope_type & 3) == 0)
        {
            gmp_block_info* v26 = gMap_0x370_6F6268->get_block_4DFE10(v5->field_14_xy.x.ToInt(),
                                                                      v5->field_14_xy.y.ToInt(),
                                                                      (v5->field_1C_zpos - dword_6F8E14).ToInt());
            if (v26)
            {
                if ((v26->field_B_slope_type & 0xFC) != 0 && (v26->field_B_slope_type & 0xFCu) < 0xB4 && (v26->field_B_slope_type & 3) != 0)
                {
                    Sprite_UpdateZFromSlopeAndTile_522FA0(v5);
                }
            }
        }

        if (!v5->CheckSpriteMovementRegion_5A2500())
        {
            break;
        }

        if (v50 != 1)
        {
            goto LABEL_47;
        }

        v5->field_1C_zpos = v5->field_1C_zpos.GetRoundValue();
        if (v5->field_1C_zpos > dword_6F8D10)
        {
            v5->field_1C_zpos = v5->field_1C_zpos + Fix16(0x4000, 0);
        }

        if (v5->CheckSpriteMovementRegion_5A2500() || SelectCollisionSprite_522460(v5))
        {
        LABEL_47:
            hitPoint = v5->get_x_y_443580();
            sub_524550();
            goto LABEL_48;
        }
    LABEL_45:
        a2_ = a2_ + 1;
        if (a2_ > t)
        {
            goto LABEL_56;
        }
    }

    if (!SelectCollisionSprite_522460(v5))
    {
        goto LABEL_45;
    }

    hitPoint = v5->get_x_y_443580();

LABEL_48:

    v5->set_xy_lazy_447E20(v61, v57);
    v5->set_ang_lazy_420690(v55);

    Sprite_UpdateZFromSlopeAndTile_522FA0(v5);
    a2_ = 3;

    while (1)
    {
        Fix16 old_x = v5->field_14_xy.x;
        Fix16 old_y = v5->field_14_xy.y;
        Ang16 old_ang = v5->field_0;
        // Search closer?
        v61 = v52 / 2;

        v5->set_xy_lazy_447E20(v5->field_14_xy.x + (gSin_table_667A80[a3.rValue] * (v61)),
                               v5->field_14_xy.y + (gCos_table_669260[a3.rValue] * (v61)));
        v5->set_ang_lazy_420690(a3);

        if (v5->CheckSpriteMovementRegion_5A2500() || SelectCollisionSprite_522460(v5))
        {
            hitPoint = v5->get_x_y_443580();
            v5->set_xy_lazy_447E20(old_x, old_y);
            v5->set_ang_lazy_420690(old_ang);
            Sprite_UpdateZFromSlopeAndTile_522FA0(v5);
        }

        if (!--a2_)
        {
            break;
        }
        v52 = v61;
    }

    field_4->set_xyz_lazy_420600(v5->field_14_xy.x, v5->field_14_xy.y, v5->field_1C_zpos);
    field_4->set_ang_lazy_420690(v5->field_0);

    switch (this->field_8->field_4C)
    {
        case 0:
        case 1:
            HandleImpact_528E50(gRozza_679188.field_20_pSprite);
            break;
        case 2:
        case 3:
            HandleCollision_522E10(&hitPoint);
            break;
        default:
            return;
    }
}

// https://decomp.me/scratch/jLuSq
WIP_FUNC(0x525190)
void Object_2C::sub_525190(u8 varrok_idx)
{
    WIP_IMPLEMENTED;

    if (field_8->field_3C_next_definition_idx < 39 || field_8->field_3C_next_definition_idx > 42)
    {
        if (field_8->field_48 == 13)
        {
            sub_5291D0();
            Object_2C* pExplosion = gObject_5C_6F8F84->CreateExplosion_52A3D0(this->field_4->field_14_xy.x,
                                                                              this->field_4->field_14_xy.y,
                                                                              this->field_4->field_1C_zpos,
                                                                              kZeroAng_6F8F68,
                                                                              19,
                                                                              gVarrok_7F8_703398->field_0[varrok_idx].field_0_ped_id);
            if (pExplosion)
            {
                pExplosion->SetDamageOwner_529080(varrok_idx);
            }
        }
    }
    else
    {
        sub_5291E0(field_8->field_3C_next_definition_idx);
    }
}

MATCH_FUNC(0x5257d0)
void Object_2C::UpdateAninmation_5257D0()
{
    ++this->field_C_pAny.o8->field_6_frame_counter;
    if (field_C_pAny.o8->field_6_frame_counter >= this->field_8->field_64_next_frame_max)
    {
        field_C_pAny.o8->field_6_frame_counter = 0;
        ++this->field_C_pAny.o8->field_7_anim_speed_counter;
        if (field_C_pAny.o8->field_7_anim_speed_counter >= this->field_8->field_6C_sprite_anim_speed)
        {
            field_C_pAny.o8->field_7_anim_speed_counter = 0;
            if (field_C_pAny.o8->field_4_timer > 0)
            {
                field_C_pAny.o8->field_4_timer--;
            }
        }
        const s16 target_id = this->field_8->field_1E + this->field_C_pAny.o8->field_7_anim_speed_counter;

        // TODO: Inline?
        Sprite* pSprite = this->field_4;
        if (pSprite->field_22_sprite_id != target_id)
        {
            pSprite->field_22_sprite_id = target_id;
            pSprite->UpdateDimensionsFromSpriteIndex_59FA40();
        }

        if (!field_C_pAny.o8->field_4_timer && !field_C_pAny.o8->field_7_anim_speed_counter)
        {
            Object_2C::TickObject_5283C0(this->field_8->field_3C_next_definition_idx);
        }
    }
}

MATCH_FUNC(0x525910)
bool Object_2C::DispatchFrameAction_525910()
{
    if (field_24_bDoneThisFrame)
    {
        switch (field_8->field_44)
        {
            case 1:
            case 2:
            case 5:
            case 6:
            case 8:
            case 11:
                if (field_24_bDoneThisFrame == 1)
                {
                    TickObject_5283C0(field_8->field_38);
                }
                else
                {
                    TickObject_5283C0(field_24_bDoneThisFrame);
                }
                field_24_bDoneThisFrame = 0;
                return true;
            case 4:
                if (field_24_bDoneThisFrame != 1)
                {
                    TickObject_5283C0(field_24_bDoneThisFrame);
                }
                field_24_bDoneThisFrame = 0;
                return true;
            case 7:
            case 10:
                field_24_bDoneThisFrame = 0;
                sub_5290A0();
                return true;
            default:
                field_24_bDoneThisFrame = 0;
                return false;
        }
    }
    else if (field_8->field_44 == 3 || field_8->field_44 == 4)
    {
        TickObject_5283C0(field_8->field_38);
        return true;
    }

    return false;
}

// Turns traffic lights into collision objects so AI stops at red lights
MATCH_FUNC(0x525290)
bool Object_2C::ShouldStopAtTrafficLight_525290(Sprite* pSprite)
{
    Car_BC* pCar = pSprite->AsCar_40FEB0();
    if (pCar)
    {
        if (byte_6771DC)
        {
            if ((field_4->field_0 != kZeroAng_6F8F68 || gTrafficLights_194_705958->field_192_phase != 1) &&
                (field_4->field_0 != word_6F8C88 || gTrafficLights_194_705958->field_192_phase != 4))
            {
                return true;
            }
        }
    }
    return false;
}

WIP_FUNC(0x525370)
char Object_2C::ShouldCollideWithSprite_525370(Sprite* pSprite)
{
    WIP_IMPLEMENTED;

    switch (this->field_18_model)
    {
        case 122:
            if (!pSprite)
            {
                return 0;
            }
            return ShouldStopAtTrafficLight_525290(pSprite);

        case 139:
            return 0;

        case 169:
            if (pSprite)
            {
                return gDoor_4D4_67BD2C->CheckDoorAccess_49D3C0(pSprite, this->field_26_varrok_idx);
            }
            else
            {
                return byte_6F8EDC;
            }

        case 294:
            if (pSprite)
            {
                Car_BC* pCar = pSprite->AsCar_40FEB0();
                if (pCar)
                {
                    return !pSprite->field_8_car_bc_ptr->IsTrainModel_403BA0();
                }
            }
            // Fall through
        default:
            if (gCollide_C_6791FC->field_8_bUnknown == 1)
            {
                if (field_8->field_34_behavior_type == 3 || field_8->field_34_behavior_type == 4 ||
                    field_8->field_34_behavior_type <= 2 && field_8->field_44 == 2)
                {
                    return 0;
                }
            }

            if (!field_10_obj_3c)
            {
                goto LABEL_35;
            }

            if (field_10_obj_3c->field_34 != 2)
            {
                return 0;
            }

            if (pSprite)
            {
                if (pSprite->field_30_sprite_type_enum != sprite_types_enum::ped)
                {
                    if (field_8->field_4C == 3)
                    {
                        return 0;
                    }

                    if (field_8->field_4C == 0)
                    {
                        return 0;
                    }
                }

            LABEL_35:
                if (pSprite)
                {
                    Object_2C* pObj2C = pSprite->As2C_40FEC0();
                    if (pObj2C)
                    {
                        if (pObj2C->field_8->field_44 != 7 && pObj2C->field_8->field_44 != 8)
                        {
                            goto LABEL_44;
                        }

                        if (!field_8->field_48)
                        {
                            return 0;
                        }

                        if ((u32)(field_8->field_48 - 12) >= 2)
                        {
                        LABEL_44:
                            if (pObj2C->field_18_model == 139)
                            {
                                return 0;
                            }

                            // Don't collide shops, I guess
                            if (pObj2C->check_is_shop_421060() && !check_is_shop_421060())
                            {
                                return 0;
                            }
                        }
                    }
                }
            }
            else
            {
                if (field_8->field_4C == 3)
                {
                    return 0;
                }
                if (field_8->field_4C == 0)
                {
                    return 0;
                }
            }
            return 1;
    }
}

MATCH_FUNC(0x525AE0)
EXPORT void Object_2C::CheckCollisionForModel_139_And_141_525AE0()
{
    switch (field_18_model)
    {
        case 139:
        case 141:
            gPurpleDoom_1_679208->CheckAndHandleCollisionInStrips_477BD0(field_4);
            gPurpleDoom_2_67920C->CheckAndHandleCollisionInStrips_477BD0(field_4);
            break;
        default:
            break;
    }
}

MATCH_FUNC(0x525b40)
void Object_2C::SpawnSpriteParticlesForModel128_525B40()
{
    if (field_18_model == 128)
    {
        gParticle_8_6FD5E8->SpawnParticleSprite_5405D0(field_4);
    }
}

MATCH_FUNC(0x525B60)
char_type Object_2C::CheckWaterDeath_525B60()
{
    // TODO: Forced eax -> al
    const u8 isWater = field_4->IsOnWater_59E1D0();
    if (isWater)
    {
        HandleWaterDeath_528900();
        return 1;
    }
    return 0;
}

MATCH_FUNC(0x525b80)
void Object_2C::UpdatePhysicsAndMovement_525B80()
{
    Ang16 ang;

    UpdatePhysics_5222D0();
    Fix16 mov_speed;
    field_10_obj_3c->GetMovementSpeedAndAngle_521FD0(&mov_speed, &ang);

    if (!field_8->field_58)
    {
        IntegrateHorizontalMovementAndCollisions_524630(mov_speed, ang);
    }
    else
    {
        IntegrateMovementAndCollisions_523BF0(mov_speed, ang);
    }
    SpawnSpriteParticlesForModel128_525B40();

    field_10_obj_3c->field_C = field_10_obj_3c->field_C.sub_482730();

    if (!DispatchFrameAction_525910())
    {
        if (field_10_obj_3c->field_0.field_0_p18)
        {
            field_10_obj_3c->field_0.PoolUpdate_5A6F70(field_4);
        }

        if (mov_speed != kFpZero_6F8E10 || (field_10_obj_3c->field_2A) || field_10_obj_3c->field_34 == 2 || CheckWaterDeath_525B60())
        {
            if (field_10_obj_3c->field_28 > 0)
            {
                field_10_obj_3c->field_28--;
                if (field_10_obj_3c->field_28 == 0)
                {
                    if (get_field_26_420FF0() > 0)
                    {
                        s32 ped_id = gVarrok_7F8_703398->GetPedId_420F10(get_field_26_420FF0());
                        if (ped_id)
                        {
                            Ped* pPed = gPedManager_6787BC->PedById(ped_id);
                            if (pPed)
                            {
                                pPed->HandleWeaponFireEnd_46FFF0(field_18_model);
                            }
                        }
                    }
                    TickObject_5283C0(field_8->field_3C_next_definition_idx);
                }
            }
        }
        else
        {
            this->field_10_obj_3c->field_1C = kFpZero_6F8E10;
            this->field_10_obj_3c->field_10 = kFpZero_6F8E10;

            TickObject_5283C0(field_8->field_38);
            if (is_not_type6_to_12_421080())
            {
                if (get_field_26_420FF0() > 0)
                {
                    gVarrok_7F8_703398->sub_59B0D0(field_26_varrok_idx); // reduce field4 of varrok at idx
                    this->field_26_varrok_idx = 0;
                }
            }
        }
    }
}

MATCH_FUNC(0x525d90)
void Object_2C::UpdatePhysicsMovementAndAnimation_525D90()
{
    Ang16 ang;
    Sprite* pPhi_ = this->field_4;
    Fix16 x_val = pPhi_->field_14_xy.x;
    Fix16 y_val = pPhi_->field_14_xy.y;
    Fix16 zpos = pPhi_->field_1C_zpos;

    UpdatePhysics_5222D0();

    Fix16 mov_speed;
    field_10_obj_3c->GetMovementSpeedAndAngle_521FD0(&mov_speed, &ang);

    if (!field_8->field_58)
    {
        IntegrateHorizontalMovementAndCollisions_524630(mov_speed, ang);
    }
    else
    {
        IntegrateMovementAndCollisions_523BF0(mov_speed, ang);
    }

    field_10_obj_3c->field_C = field_10_obj_3c->field_C.sub_482730();

    Phi_74* pPhi = this->field_8;
    if (pPhi->field_65 != -1 || pPhi->field_34_behavior_type == 9 || (field_4->field_14_xy.x != x_val) || field_4->field_14_xy.y != y_val ||
        field_4->field_1C_zpos != zpos)
    {
        if (!this->field_10_obj_3c->field_30_bSkipAnim)
        {
            UpdateAninmation_5257D0();
        }
    }

    if (!DispatchFrameAction_525910())
    {
        if (field_10_obj_3c->field_0.field_0_p18)
        {
            field_10_obj_3c->field_0.PoolUpdate_5A6F70(field_4);
            if (!field_10_obj_3c->field_0.field_0_p18 && field_18_model == 127)
            {
                sub_5290A0();
            }
        }

        if (mov_speed == kFpZero_6F8E10)
        {
            if (!field_10_obj_3c->field_2A && field_10_obj_3c->field_34 == 2 && !CheckWaterDeath_525B60() && !dword_6F8F8C)
            {
                field_10_obj_3c->field_1C = kFpZero_6F8E10;
                field_10_obj_3c->field_10 = kFpZero_6F8E10;

                if (field_18_model != 183)
                {
                    if (is_not_type6_to_12_421080())
                    {
                        if (field_26_varrok_idx > 0)
                        {
                            gVarrok_7F8_703398->sub_59B0D0(field_26_varrok_idx);
                            this->field_26_varrok_idx = 0;
                        }
                    }
                }
                TickObject_5283C0(field_8->field_38);
            }
        }
    }
}

MATCH_FUNC(0x525f30)
void Object_2C::Update_525F30()
{
    Wolfy_30* pWolfy;

    dword_6F8F8C = 0;
    while (2)
    {
        switch (this->field_8->field_34_behavior_type)
        {
            case object_behavior_type::behavior_0:
                if (!DispatchFrameAction_525910())
                {
                    UpdateEffectPool_525B20();
                    return;
                }
                return;
            case object_behavior_type::behavior_1:
                if (!DispatchFrameAction_525910())
                {
                    UpdateEffectPool_525B20();
                    CheckCollisionForModel_139_And_141_525AE0();
                }
                return;

            case object_behavior_type::behavior_5:
                pWolfy = this->field_C_pAny.pExplosion;
                if (pWolfy)
                {
                    if (pWolfy->Update_5434A0(kFpZero_6F8E10, kZeroAng_6F8F68))
                    {
                        this->field_25 = 1;
                    }
                }
                return;

            case object_behavior_type::behavior_3:
            case object_behavior_type::behavior_7:
                RemoveFromCollisionBuckets_527D00();
                byte_6F8C4C = 1;
                UpdatePhysicsAndMovement_525B80();
                if (!byte_6F8F40)
                {
                    AssignToBucket_527AE0();
                }
                return;

            case object_behavior_type::behavior_2:
            case object_behavior_type::behavior_8:
                if (!DispatchFrameAction_525910())
                {
                    UpdateEffectPool_525B20();
                    UpdateAninmation_5257D0();
                }
                return;

            case object_behavior_type::behavior_4:
            case object_behavior_type::behavior_9:
                RemoveFromCollisionBuckets_527D00();
                byte_6F8C4C = 1;
                UpdatePhysicsMovementAndAnimation_525D90();
                if (!byte_6F8F40)
                {
                    AssignToBucket_527AE0();
                }
                return;

            case object_behavior_type::behavior_6:
            case object_behavior_type::behavior_10:
            case object_behavior_type::behavior_11:
                DispatchFrameAction_525910();
                UpdateEffectPool_525B20();
                return;

            case object_behavior_type::behavior_12:
                UpdateEffectPool_525B20();
                return;

            default:
                continue;
        }
    }
}

MATCH_FUNC(0x5263d0)
bool Object_2C::PoolUpdate()
{
    byte_6F8C4C = 0;
    byte_6F8F40 = 0;
    if (field_25 != 1)
    {
        Object_2C::Update_525F30();
    }

    if (field_25 <= 0 || (field_25 == 2 && gGame_0x40_67E008->IsSpriteOnScreenForAnyPlayer_4B97E0(field_4, kFpZero_6F8E10)))
    {
        return false;
    }
    else
    {
        Object_2C::RemoveFromCollisionBuckets_527D00();
        return true;
    }
}

// 9.6f 0x4837F0
WIP_FUNC(0x526790)
void Object_2C::TriggerCarExplosionIfApplicable_526790(Sprite* pSprite)
{
    WIP_IMPLEMENTED;

    Phi_74* pPhi = this->field_8;
    if (sub_475A80())
    {
        Object_8* o8 = this->field_C_pAny.o8;
        if (!o8->field_4_timer && !o8->field_7_anim_speed_counter)
        {
            s32 phi_idx = pPhi->field_3C_next_definition_idx;
            if (phi_idx >= 39 && phi_idx <= 42) // car
            {
                Car_BC* pCar = pSprite->AsCar_40FEB0();
                if (pCar)
                {
                    if (this->field_18_model == objects::moving_collect_36_132)
                    {

                        s32 id = gVarrok_7F8_703398->GetPedId_420F10(get_field_26_420FF0());
                        if (id)
                        {
                            pCar->field_70_exploder_ped_id = id;
                            pCar->field_90 = 12;
                            pCar->field_94 = 50;
                        }
                    }
                    s32 t = sub_526830(field_8->field_3C_next_definition_idx);
                    pCar->HandleCarExplosion_43D840(t);
                }
            }
        }
    }
}

WIP_FUNC(0x526830)
s32 Object_2C::sub_526830(s32 a1)
{
    WIP_IMPLEMENTED;

    int result;
    switch (a1)
    {
        case 39:
            result = 18;
            break;
        case 40:
            result = 33;
            break;
        case 41:
            result = 19;
            break;
        case 42:
            result = 20;
            break;
        default:
            result = a1;
            break;
    }
    return result;
}

WIP_FUNC(0x526b40)
void Object_2C::sub_526B40(Sprite* pSprite)
{
    WIP_IMPLEMENTED;

    Object_3C* p3C; // eax

    switch (pSprite->field_30_sprite_type_enum)
    {
        case 3: //sprite_type_3_Char_B4:
            this->field_10_obj_3c->field_C = (pSprite->field_8_char_b4_ptr->field_38_velocity * k_dword_6F8C9C);
            this->field_10_obj_3c->field_4 = pSprite->field_0;
            this->field_4->field_28_num = 27;
            break;

        case sprite_types_enum::car: // 2
            this->field_10_obj_3c->field_C = pSprite->field_8_car_bc_ptr->sub_43A240();
            this->field_10_obj_3c->field_4 = pSprite->field_8_car_bc_ptr->GetOrientationAngle_43A3E0();
            this->field_4->field_28_num = pSprite->AsCar_40FEB0()->GetCrashSoundCategory_4435B0();
            break;

        case 4: //sprite_type_4_Object_5C:
        case 5: //sprite_type_5_Object_5C:
            p3C = pSprite->field_8_object_2C_ptr->field_10_obj_3c;
            if (p3C)
            {
                this->field_10_obj_3c->field_C = p3C->field_C;
                this->field_10_obj_3c->field_4 = pSprite->field_8_object_2C_ptr->field_10_obj_3c->field_4;
                this->field_10_obj_3c->field_10 = pSprite->field_8_object_2C_ptr->field_10_obj_3c->field_10;
            }
            this->field_4->field_28_num = 12;
            break;

        default:
            return;
    }
}

WIP_FUNC(0x527070)
bool Object_2C::UpdateMovementAndEffects_527070(Sprite* pSprite, Fix16 x, Fix16 y, Ang16 rot)
{
    WIP_IMPLEMENTED;

    byte_6F8C68 = 1;

    if (field_10_obj_3c)
    {
        sub_526B40(pSprite);
    }

    if (this->field_8->field_34_behavior_type != object_behavior_type::behavior_11)
    {
        gPurpleDoom_3_679210->Remove_477B00(field_4);
    }

    PoolTake_522360();

    if (field_25)
    {
        if (x == kFpZero_6F8E10 && y == kFpZero_6F8E10)
        {
            x = pSprite->field_14_xy.x;
            y = pSprite->field_14_xy.y;
        }
        else
        {
            Ang16::RotateVector_41FC90(x, y, pSprite->field_0);
            x += pSprite->field_14_xy.x;
            y += pSprite->field_14_xy.y;
        }

        field_4->set_xyz_lazy_420600(x, y, pSprite->field_1C_zpos);
        field_4->set_ang_lazy_420690(pSprite->field_0 + rot);

        nostalgic_ellis_0x28* pLight;
        switch (this->field_8->field_34_behavior_type)
        {
            case object_behavior_type::behavior_2:
                UpdateAninmation_5257D0();
                break;

            case object_behavior_type::behavior_5:
            {
                Wolfy_30* w30 = this->field_C_pAny.pExplosion;
                if (w30)
                {
                    switch (pSprite->field_30_sprite_type_enum)
                    {
                        case sprite_types_enum::car:
                            if (w30->IsState_5435D0())
                            {
                                field_4->set_z_lazy_420660(pSprite->field_8_car_bc_ptr->GetZPos_441330());
                                pSprite->field_8_car_bc_ptr->AccumulateDamage_43DA90(1, &stru_6F8EF0);
                            }

                            this->field_C_pAny.pExplosion->field_1C = pSprite;

                            if (field_C_pAny.pExplosion->Update_5434A0(field_10_obj_3c->field_C, field_10_obj_3c->field_4))
                            {
                                byte_6F8C68 = 0;
                                return 1;
                            }
                            break;

                        case 3:
                        case 4:
                        case 5:
                            if (w30->Update_5434A0(field_10_obj_3c->field_C, field_10_obj_3c->field_4))
                            {
                                byte_6F8C68 = 0;
                                return 1;
                            }
                            break;

                        default:
                            break;
                    }
                }
                break;
            }

            case object_behavior_type::behavior_11:
                pLight = this->field_C_pAny.pLight;
                pLight->sub_482D30(field_4->field_14_xy.x, field_4->field_14_xy.y, field_4->field_1C_zpos);
                break;

            default:
                break;
        }

        if (field_10_obj_3c)
        {
            if (field_10_obj_3c->field_0.field_0_p18)
            {
                field_10_obj_3c->field_0.PoolUpdate_5A6F70(field_4);
            }
        }

        if (this->field_8->field_34_behavior_type != 11)
        {
            gPurpleDoom_3_679210->AddToSingleBucket_477AE0(field_4);
        }
        byte_6F8C68 = 0;
        return 0;
    }
    else
    {
        TriggerCarExplosionIfApplicable_526790(pSprite);
        byte_6F8C68 = 0;
        return 1;
    }
}

MATCH_FUNC(0x527630)
void Object_2C::InitializeObject_527630(s32 object_type, Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation)
{
    Phi_74* phi74 = gPhi_8CA8_6FCF00->GetObjectDefinition_534360(object_type);
    field_8 = phi74;
    field_18_model = object_type;
    field_24_bDoneThisFrame = 0;

    if (field_4)
    {
        phi74->ApplyDefinitionToSprite_5331A0(field_4);
    }
    else
    {
        field_4 = phi74->CreateSpriteFromDefinition_533170();
    }

    field_4->set_xyz_lazy_420600(xpos, ypos, zpos);
    field_4->set_ang_lazy_420690(rotation);

    field_4->field_8_object_2C_ptr = this;
}

MATCH_FUNC(0x527990)
void Object_2C::Light_527990()
{
    field_C_pAny.pLight->field_0.flag &= ~0xFF;
}

MATCH_FUNC(0x527ae0)
void Object_2C::AssignToBucket_527AE0()
{
    switch (field_8->field_40_collision_bucket_category)
    {
        case collision_bucket_category::purple_doom_3_single_bucket_0:
        case collision_bucket_category::purple_doom_3_single_bucket_1:
            gPurpleDoom_3_679210->AddToSingleBucket_477AE0(field_4);
            return;
        case collision_bucket_category::purple_doom_2_region_bucket_3:
            dword_6F8F88++;
            gPurpleDoom_2_67920C->AddToRegionBuckets_477B20(field_4);
            return;
        case collision_bucket_category::purple_doom_1_region_bucket_4:
            gPurpleDoom_1_679208->AddToRegionBuckets_477B20(field_4);
            return;
        case collision_bucket_category::purple_doom_none_2:
            return;
    }
}

MATCH_FUNC(0x527d00)
void Object_2C::RemoveFromCollisionBuckets_527D00()
{
    switch (field_8->field_40_collision_bucket_category)
    {
        case collision_bucket_category::purple_doom_3_single_bucket_0:
        case collision_bucket_category::purple_doom_3_single_bucket_1:
            gPurpleDoom_3_679210->Remove_477B00(field_4);
            break;
        case collision_bucket_category::purple_doom_2_region_bucket_3:
            --dword_6F8F88;
            gPurpleDoom_2_67920C->AddToSpriteRectBuckets_477B60(field_4);
            break;
        case collision_bucket_category::purple_doom_1_region_bucket_4:
            gPurpleDoom_1_679208->AddToSpriteRectBuckets_477B60(field_4);
            break;
        default:
            return;
    }
}

// 9.6f 0x484760
WIP_FUNC(0x527f10)
void Object_2C::sub_527F10()
{
    WIP_IMPLEMENTED;

    if (field_C_pAny.o8)
    {
        if (this->field_8->field_34_behavior_type == object_behavior_type::behavior_11)
        {
            gLight_1D4CC_6F5520->DeallocLight_47F4F0(field_C_pAny.pLight);
        }
        else if (this->field_1C)
        {
            field_C_pAny.pExplosion->DeInit_543610();
        }
        else
        {
            gObject_8_Pool_6F8F78->DeAllocate(field_C_pAny.o8);
        }
        this->field_C_pAny.o8 = 0;
    }

    if (field_10_obj_3c)
    {
        if (field_10_obj_3c->field_0.field_0_p18)
        {
            field_10_obj_3c->field_0.DestroyAllSprites_5A7010();
        }

        gObject_3C_Pool_6F8F7C->DeAllocate(field_10_obj_3c);
        field_10_obj_3c = 0;
    }
}

// 9.6f 0x4847D0
WIP_FUNC(0x528130)
void Object_2C::NewObj3C_528130(Fix16_Point* a2)
{
    WIP_IMPLEMENTED;

    Object_3C* pNewObj = gObject_3C_Pool_6F8F7C->Allocate();

    pNewObj->field_20 = field_14_id;

    this->field_10_obj_3c = pNewObj;

    this->field_10_obj_3c->field_C =
        a2->GetLength_41E260(); // TODO: Uses wrong zero constants?? Artifact of func being inlined into each TU??
    this->field_10_obj_3c->field_4 = Fix16::atan2_fixed_405320(a2->y, a2->x);
}

MATCH_FUNC(0x528240)
char_type Object_2C::HandleRotationStateTransition_528240(s32 current, s32 desired)
{
    switch (current)
    {
        case 1:
            switch (desired)
            {
                case 2:
                    if (gObject_5C_6F8F84->field_10_rotation_counter == 360)
                    {
                        sub_5290A0();
                        return 1;
                    }
                    gObject_5C_6F8F84->field_10_rotation_counter++;
                    break;

                case 3:
                    gObject_5C_6F8F84->field_14_sprites_in_list++;
                    gObject_5C_6F8F84->field_1C.AddSprite_5A6CD0(field_4);
                    break;
            }
            break;

        case 2:
            switch (desired)
            {
                case 1:
                    gObject_5C_6F8F84->field_10_rotation_counter--;
                    break;

                case 3:
                    gObject_5C_6F8F84->field_10_rotation_counter--;
                    gObject_5C_6F8F84->field_14_sprites_in_list++;
                    gObject_5C_6F8F84->field_1C.AddSprite_5A6CD0(field_4);
                    break;
            }
            break;

        case 3:
            switch (desired)
            {
                case 1:
                    gObject_5C_6F8F84->field_14_sprites_in_list--;
                    gObject_5C_6F8F84->field_1C.RemoveSprite_5A6B10(field_4);
                    break;

                case 2:
                    if (gObject_5C_6F8F84->field_10_rotation_counter == 360)
                    {
                        sub_5290A0();
                        return 1;
                    }

                    gObject_5C_6F8F84->field_14_sprites_in_list--;
                    gObject_5C_6F8F84->field_1C.RemoveSprite_5A6B10(field_4);
                    gObject_5C_6F8F84->field_10_rotation_counter++;
                    break;
            }
            break;
    }
    return 0;
}

WIP_FUNC(0x5283c0)
void Object_2C::TickObject_5283C0(s32 obj_type)
{
    WIP_IMPLEMENTED;

    if (obj_type)
    {
        if (obj_type < 39 || obj_type > 42) // sub_482400
        {
            if (obj_type == 259)
            {
                gParticle_8_6FD5E8->EmitImpactParticles_53FE40(field_4->field_14_xy.x,
                                                               field_4->field_14_xy.y,
                                                               field_4->field_1C_zpos,
                                                               (gSin_table_667A80[field_4->field_0.rValue] * k_dword_6F8C9C),
                                                               (gCos_table_669260[field_4->field_0.rValue] * k_dword_6F8C9C));
                Object_2C::sub_5290A0();
            }
            else
            {
                Phi_74* pPhi = gPhi_8CA8_6FCF00->GetObjectDefinition_534360(obj_type);
                if (field_8->field_5C == pPhi->field_5C ||
                    !Object_2C::HandleRotationStateTransition_528240(field_8->field_5C, pPhi->field_5C))
                {
                    if (!byte_6F8C4C && !byte_6F8C68)
                    {
                        Object_2C::RemoveFromCollisionBuckets_527D00();
                    }
                    field_4->FreeSprite4CChildren_59FAD0();

                    switch (pPhi->field_34_behavior_type)
                    {
                        case object_behavior_type::behavior_0:
                        case object_behavior_type::behavior_1:
                        case object_behavior_type::behavior_6:
                        case object_behavior_type::behavior_10:
                        case object_behavior_type::behavior_12:
                            Object_2C::sub_527F10();
                            Object_2C::InitializeObject_527630(obj_type,
                                                               this->field_4->field_14_xy.x,
                                                               this->field_4->field_14_xy.y,
                                                               this->field_4->field_1C_zpos,
                                                               this->field_4->field_0);
                            if (this->field_10_obj_3c)
                            {
                                return;
                            }
                            break;

                        case object_behavior_type::behavior_2:
                        case object_behavior_type::behavior_8:
                        {
                            if (field_10_obj_3c)
                            {
                                if (!field_10_obj_3c->field_0.field_0_p18)
                                {
                                    gObject_3C_Pool_6F8F7C->DeAllocate(field_10_obj_3c);
                                    this->field_10_obj_3c = 0;
                                }
                            }

                            s32 bUnknown;
                            if (this->field_C_pAny.o8)
                            {
                                bUnknown = 0;
                            }
                            else
                            {
                                bUnknown = 1;
                                Object_8* pNewObj8 = gObject_8_Pool_6F8F78->Allocate();
                                this->field_C_pAny.o8 = pNewObj8;
                            }

                            if (pPhi->field_65 != field_8->field_65 || bUnknown)
                            {
                                this->field_C_pAny.o8->field_4_timer = pPhi->field_65;
                                this->field_C_pAny.o8->field_7_anim_speed_counter = 0;
                                this->field_C_pAny.o8->field_6_frame_counter = 0;
                            }
                            else if (pPhi->field_6C_sprite_anim_speed < field_8->field_6C_sprite_anim_speed)
                            {
                                this->field_C_pAny.o8->field_7_anim_speed_counter = 0;
                            }
                            Object_2C::InitializeObject_527630(obj_type,
                                                               this->field_4->field_14_xy.x,
                                                               this->field_4->field_14_xy.y,
                                                               this->field_4->field_1C_zpos,
                                                               this->field_4->field_0);
                            field_4->set_id_lazy_4206C0(this->field_8->field_1E + this->field_C_pAny.o8->field_7_anim_speed_counter);
                            break;
                        }

                        case object_behavior_type::behavior_3:
                        case object_behavior_type::behavior_7:
                            if (!this->field_10_obj_3c)
                            {
                                Object_3C* pNew3C_ = gObject_3C_Pool_6F8F7C->Allocate();
                                pNew3C_->field_20 = field_14_id;
                                this->field_10_obj_3c = pNew3C_;
                            }

                            if (field_C_pAny.o8)
                            {
                                gObject_8_Pool_6F8F78->DeAllocate(field_C_pAny.o8);
                                this->field_C_pAny.o8 = 0;
                            }

                            this->field_10_obj_3c->field_18 = pPhi->field_14_friction;
                            this->field_10_obj_3c->field_1C = kFpZero_6F8E10;
                            this->field_10_obj_3c->field_10 = kFpZero_6F8E10;
                            this->field_10_obj_3c->field_28 = pPhi->field_65;
                            Object_2C::InitializeObject_527630(obj_type,
                                                               this->field_4->field_14_xy.x,
                                                               this->field_4->field_14_xy.y,
                                                               this->field_4->field_1C_zpos,
                                                               this->field_4->field_0);
                            break;

                        case object_behavior_type::behavior_4:
                        case object_behavior_type::behavior_9:
                        {
                            if (!this->field_10_obj_3c)
                            {
                                Object_3C* pNew3C = gObject_3C_Pool_6F8F7C->Allocate();
                                pNew3C->field_20 = field_14_id;
                                this->field_10_obj_3c = pNew3C;
                            }

                            s32 bUnknown2;
                            if (this->field_C_pAny.o8)
                            {
                                bUnknown2 = 0;
                            }
                            else
                            {
                                bUnknown2 = 1;
                                Object_8* pNewObj8 = gObject_8_Pool_6F8F78->Allocate();
                                this->field_C_pAny.o8 = pNewObj8;
                            }

                            if (pPhi->field_65 != field_8->field_65 || bUnknown2)
                            {
                                this->field_C_pAny.o8->field_4_timer = pPhi->field_65;
                                this->field_C_pAny.o8->field_7_anim_speed_counter = 0;
                                this->field_C_pAny.o8->field_6_frame_counter = 0;
                            }
                            else if (pPhi->field_6C_sprite_anim_speed < field_8->field_6C_sprite_anim_speed)
                            {
                                this->field_C_pAny.o8->field_7_anim_speed_counter = 0;
                            }

                            Object_2C::InitializeObject_527630(obj_type,
                                                               this->field_4->field_14_xy.x,
                                                               this->field_4->field_14_xy.y,
                                                               this->field_4->field_1C_zpos,
                                                               this->field_4->field_0);
                            field_4->set_id_lazy_4206C0(this->field_8->field_1E + this->field_C_pAny.o8->field_7_anim_speed_counter);
                            this->field_10_obj_3c->field_18 = pPhi->field_14_friction;
                            this->field_10_obj_3c->field_1C = kFpZero_6F8E10;
                            this->field_10_obj_3c->field_10 = kFpZero_6F8E10;
                            break;
                        }

                        default:
                            break;
                    }

                    //LABEL_48:
                    if (!pPhi->field_61 && !this->field_25)
                    {
                        Object_2C::PoolTake_522360();
                    }
                    if (!byte_6F8C68)
                    {
                        byte_6F8F40 = 1;
                        Object_2C::AssignToBucket_527AE0();
                    }
                    //break;
                }
            }
        }
        else
        {
            s32 ped_id = gVarrok_7F8_703398->field_0[field_26_varrok_idx].field_0_ped_id;
            s32 type_or_state = sub_526830(obj_type);
            Object_2C* pExplosion = gObject_5C_6F8F84->CreateExplosion_52A3D0(field_4->field_14_xy.x,
                                                                              field_4->field_14_xy.y,
                                                                              field_4->field_1C_zpos,
                                                                              kZeroAng_6F8F68,
                                                                              type_or_state,
                                                                              ped_id);
            if (pExplosion)
            {
                pExplosion->SetDamageOwner_529080(this->field_26_varrok_idx);
            }
            Object_2C::sub_5290A0();
        }
    }
    else
    {
        Object_2C::sub_5290A0();
    }
}

MATCH_FUNC(0x5288B0)
bool Object_2C::OnObjectTouched_5288B0(Sprite* a2)
{
    if (!a2)
    {
        return false;
    }

    Char_B4* pCharB4 = a2->AsCharB4_40FEA0();
    if (pCharB4)
    {
        return pCharB4->OnObjectTouched_553640(this);
    }

    Car_BC* pCarBc = a2->AsCar_40FEB0();
    if (pCarBc)
    {
        return pCarBc->OnObjectTouched_43EA60(this);
    }

    return false;
}

MATCH_FUNC(0x528900)
void Object_2C::HandleWaterDeath_528900()
{
    if (field_10_obj_3c)
    {
        field_10_obj_3c->field_30_bSkipAnim = 1;
    }

    if ((rng_dword_67AB34->field_0_rng & 3) == 0)
    {
        field_4->sub_59E320(1);
        if (field_4->ShrinkSprite_59E390(dword_6F8F5C, dword_6F8F5C, 0))
        {
            // inline - because has to be a local here?
            Sprite* pSprite = this->field_4;
            if (pSprite->field_1C_zpos != kFpZero_6F8E10)
            {
                pSprite->field_1C_zpos = kFpZero_6F8E10;
                pSprite->ResetZCollisionAndDebugBoxes_59E7B0();
            }
            sub_5290A0();
        }
    }
}

MATCH_FUNC(0x528960)
char_type Object_2C::HandleObjectHitIfExplosive_528960(Object_2C* pOther)
{
    // Check is explosive range
    if (field_8->field_48 < 12 || field_8->field_48 > 13)
    {
        return 0;
    }

    pOther->ProcessObjectExplosionImpact_528A20(this);
    return 1;
}

MATCH_FUNC(0x528990)
char_type Object_2C::HandleObjectHit_528990(Sprite* pSprite)
{
    Char_B4* cB4 = pSprite->AsCharB4_40FEA0();
    if (cB4)
    {
        return cB4->HandlePedObjectHit_5537F0(this);
    }

    Car_BC* cBC = pSprite->AsCar_40FEB0();
    if (cBC)
    {
        return cBC->HandleCarHitByObject_43F130(this);
    }

    Object_2C* o2c = pSprite->As2C_40FEC0();

    if (gVarrok_7F8_703398->field_0[field_26_varrok_idx].field_0_ped_id)
    {
        Ped* pPed = gPedManager_6787BC->PedById(gVarrok_7F8_703398->field_0[field_26_varrok_idx].field_0_ped_id);
        if (pPed)
        {
            pPed->ProcessWeaponHitResponse_46FE20(o2c);
        }
    }
    return o2c->HandleObjectHitIfExplosive_528960(this);
}

MATCH_FUNC(0x528A20)
void Object_2C::ProcessObjectExplosionImpact_528A20(Object_2C* pObj)
{
    switch (this->field_18_model)
    {
        case 128:
        case 138:
        {
            s32 remapped;
            if (pObj->field_18_model == objects::object_166)
            {
                remapped = pObj->sub_529210();
            }
            else
            {
                if (pObj->field_8->field_48 == 13)
                {
                    remapped = 19;
                }
                else
                {
                    remapped = 18;
                }
            }
            Object_2C* pExplosion = gObject_5C_6F8F84->CreateExplosion_52A3D0(this->field_4->field_14_xy.x,
                                                                              this->field_4->field_14_xy.y,
                                                                              this->field_4->field_1C_zpos,
                                                                              kZeroAng_6F8F68,
                                                                              remapped,
                                                                              gVarrok_7F8_703398->GetPedId_420F10(field_26_varrok_idx));
            if (pExplosion)
            {
                pExplosion->SetDamageOwner_529080(field_26_varrok_idx);
            }
            break;
        }

        case 154:
        case 159:
        case 192:
        case 193:
        case 194:
        case 195:
        case 198:
        case 199:
        case 254:
        case 265:
        case 277:
            break;
        default:
            return;
    }

    Ped* pPed; // eax
    s32 pedId = gVarrok_7F8_703398->GetPedId_420F10(field_26_varrok_idx);
    if (pedId)
    {
        pPed = gPedManager_6787BC->PedById(pedId);
    }
    else
    {
        pPed = 0;
    }

    if (pObj->field_18_model == objects::object_166)
    {
        if (pPed)
        {
            pPed->NotifyWeaponHit_46FF00(this->field_4->field_14_xy.x, this->field_4->field_14_xy.y, this->field_18_model);
        }
    }
}

WIP_FUNC(0x528E00)
s32 __stdcall Object_2C::sub_528E00(s32 a1)
{
    WIP_IMPLEMENTED;

    int result; // eax

    switch (a1)
    {
        case 0:
            result = 18;
            break;
        case 1:
            result = 23;
            break;
        case 2:
            result = 22;
            break;
        case 4:
            result = 25;
            break;
        default:
            result = 24;
            break;
    }
    return result;
}

MATCH_FUNC(0x528ba0)
void Object_2C::HandleImpactNoSprite_528BA0()
{
    Fix16_Point point;

    switch (this->field_18_model)
    {
        case 128:
        case 138:
        {
            Object_2C* pExplosion = gObject_5C_6F8F84->CreateExplosion_52A3D0(field_4->field_14_xy.x,
                                                                              field_4->field_14_xy.y,
                                                                              field_4->field_1C_zpos,
                                                                              kZeroAng_6F8F68,
                                                                              sub_528E00(dword_6F8F90),
                                                                              gVarrok_7F8_703398->GetPedId_420F10(field_26_varrok_idx));
            if (pExplosion)
            {
                pExplosion->SetDamageOwner_529080(field_26_varrok_idx);
            }
            // Fall through
        }

        case 254:
        case 265:
        {
            point.FromPolar_41E210(k_dword_6F8C9C, field_4->field_0 + word_6F8D62);
            gParticle_8_6FD5E8->EmitImpactParticles_53FE40(field_4->field_14_xy.x,
                                                           field_4->field_14_xy.y,
                                                           field_4->field_1C_zpos,
                                                           point.x,
                                                           point.y);
            break;
        }

        case 154:
        case 159:
        case 192:
        case 193:
        case 194:
        case 195:
        case 198:
        case 199:
        case 277:
            break;

        default:
            return;
    }

    const s32 id = gVarrok_7F8_703398->GetPedId_420F10(field_26_varrok_idx);
    if (id)
    {
        Ped* pPed = gPedManager_6787BC->PedById(id);
        if (pPed)
        {
            pPed->NotifyWeaponHit_46FF00(field_4->field_14_xy.x, field_4->field_14_xy.y, field_18_model);
        }
    }
}

WIP_FUNC(0x528e50)
void Object_2C::HandleImpact_528E50(Sprite* pSprite)
{
    WIP_IMPLEMENTED;

    // Mostly bad switch ordering I think

    if (!this->field_24_bDoneThisFrame && (!pSprite || ShouldCollideWith_5223C0(pSprite)))
    {
        this->field_24_bDoneThisFrame = 1;
        switch (field_8->field_44)
        {
            case 1:
            case 2:
                PoolGive_522340(); // destroy
                break;

            case 3:
                if (pSprite->AsCar_40FEB0())
                {
                    this->field_24_bDoneThisFrame = 1;
                }
                else
                {
                    this->field_24_bDoneThisFrame = 0;
                }

                if (!field_24_bDoneThisFrame)
                {
                    return;
                }
                PoolGive_522340();
                break;

            case 4:
            case 5:
                if (pSprite->AsCharB4_40FEA0())
                {
                    this->field_24_bDoneThisFrame = 1; // is_charb4
                }
                else
                {
                    this->field_24_bDoneThisFrame = 0;
                }

                if (!field_24_bDoneThisFrame)
                {
                    return;
                }
                PoolGive_522340();
                break;

            case 6:
                this->field_24_bDoneThisFrame = OnObjectTouched_5288B0(pSprite);
                if (field_24_bDoneThisFrame)
                {
                    PoolGive_522340(); // destroy
                }
                break;

            case 7:
            case 8:
                if (pSprite)
                {
                    this->field_24_bDoneThisFrame = HandleObjectHit_528990(pSprite);
                }
                else
                {
                    HandleImpactNoSprite_528BA0();
                }

                if (!field_24_bDoneThisFrame)
                {
                    return;
                }
                PoolGive_522340();
                break;

            case 9:
                this->field_24_bDoneThisFrame = OnObjectTouched_5288B0(pSprite);
                break;

            case 10:
                if (!pSprite)
                {
                    break;
                }
                if (pSprite->AsCharB4_40FEA0())
                {
                    this->field_24_bDoneThisFrame = 1;
                }
                else
                {
                    this->field_24_bDoneThisFrame = 0;
                }
                break;

            case 11:
                this->field_24_bDoneThisFrame = 0;
                break;

            default:
                break;
        }

        if (this->field_24_bDoneThisFrame)
        {
            if (pSprite)
            {
                if (!check_is_shop_421060() || !pSprite->IsControlledByActivePlayer_59E170())
                {
                    gRozza_C88_66AFE0->Type3_40BDD0(field_4, pSprite);
                }
            }
            else if (gRozza_679188.field_0_type == 4)
            {
                gRozza_C88_66AFE0->Type4_40BC40(field_4);
            }
            else if (gRozza_679188.field_0_type == 5)
            {
                gRozza_C88_66AFE0->Type5_40BD10(field_4);
            }
            else
            {
                gRozza_C88_66AFE0->OtherType_40BBA0(field_4, kFpZero_6F8E10);
            }
        }
    }
}

MATCH_FUNC(0x529000)
void Object_2C::HandleCollisionWithObject_529000(Object_2C* pObj)
{
    switch (pObj->field_18_model)
    {
        case 139:
            sub_529070(pObj);
            break;

        case 141:
            if (field_8->field_60)
            {
                Dealloc_5291B0();
            }
            break;
    }
}

MATCH_FUNC(0x529030)
void Object_2C::sub_529030(s8 speed_x, s8 speed_y)
{
    field_26_varrok_idx = (speed_y + 7) | (16 * (speed_x + 7)); //  two variables in the same byte?
}

MATCH_FUNC(0x529070)
void Object_2C::sub_529070(Object_2C* pObj)
{
    sub_5226A0(pObj->field_26_varrok_idx);
}

// https://decomp.me/scratch/lXvKN
MATCH_FUNC(0x529210)
s32 Object_2C::sub_529210()
{
    switch (field_26_varrok_idx)
    {
        case 46:
        case 48:
            return 22;
        case 45:
        case 47:
            return 23;
    }
    return 23;
}

MATCH_FUNC(0x529240)
s32 Object_2C::sub_529240()
{
    s32 result;
    gmp_block_info* pBlockInfo =
        gMap_0x370_6F6268->get_block_4DFE10(field_4->field_14_xy.x.ToInt(), field_4->field_14_xy.y.ToInt(), field_4->field_1C_zpos.ToInt());
    switch (field_26_varrok_idx)
    {
        case 45u:
        case 47u:
            result = gGtx_0x106C_703DD4->field_6C_spec[pBlockInfo->field_0_left & 0x3FF];
            if (result == 3)
            {
                return 1;
            }
            break;

        case 46u:
        case 48u:
            result = gGtx_0x106C_703DD4->field_6C_spec[pBlockInfo->field_2_right & 0x3FF];
            if (result == 3)
            {
                result = 1;
            }
            break;

        default:
            result = 0;
            break;
    }
    return result;
}

MATCH_FUNC(0x5292D0)
void Object_2C::get_weapon_default_ammo_5292D0()
{
    s32 wepon_kind;
    if (field_18_model >= 200)
    {
        wepon_kind = field_18_model - 200;
    }
    else
    {
        wepon_kind = field_18_model - 64;
    }

    if (wepon_kind <= 27)
    {
        field_26_varrok_idx = gWeapon_8_707018->get_defalt_ammo_5E3E80(wepon_kind);
    }
}

MATCH_FUNC(0x529080)
void Object_2C::SetDamageOwner_529080(u8 idx)
{
    field_26_varrok_idx = idx;
    gVarrok_7F8_703398->IncrementRefCount_59B0B0(idx);
}

MATCH_FUNC(0x5290a0)
void Object_2C::sub_5290A0()
{
    field_25 = 1;
}

MATCH_FUNC(0x5290B0)
void Object_2C::sub_5290B0()
{
    field_25 = 2;
}

MATCH_FUNC(0x5291b0)
void Object_2C::Dealloc_5291B0()
{
    PoolGive_522340();
    sub_5290A0();
}

MATCH_FUNC(0x5291d0)
void Object_2C::sub_5291D0()
{
    PoolGive_522340();
    field_24_bDoneThisFrame = 1;
}

MATCH_FUNC(0x5291E0)
void Object_2C::sub_5291E0(u8 a2)
{
    PoolGive_522340();
    field_24_bDoneThisFrame = a2;
}

MATCH_FUNC(0x529200)
bool Object_2C::IsNotModel_174_529200()
{
    return (field_18_model != 174) ? true : false;
}

MATCH_FUNC(0x52ae60)
Object_2C::~Object_2C()
{
    mpNext = 0;
    field_4 = 0;
    field_8 = 0;
    field_10_obj_3c = 0;
}

MATCH_FUNC(0x52ae70)
Fix16_Point Object_2C::GetXY_52AE70()
{
    return field_4->get_x_y_443580();
}

MATCH_FUNC(0x52ae90)
Fix16_Point Object_2C::GetRot_52AE90()
{
    if (field_10_obj_3c)
    {
        return field_10_obj_3c->GetRot_52ADF0();
    }
    return stru_6F8EF0;
}

MATCH_FUNC(0x5290C0)
void Object_2C::sub_5290C0(u8 id_base)
{
    s16 new_id = id_base + field_8->field_1E;
    Sprite* pSprite = field_4;
    if (pSprite->field_22_sprite_id != new_id)
    {
        pSprite->field_22_sprite_id = new_id;
        pSprite->UpdateDimensionsFromSpriteIndex_59FA40();
    }
}

MATCH_FUNC(0x5290F0)
Fix16 Object_2C::sub_5290F0()
{
    if (field_10_obj_3c)
    {
        if (!field_10_obj_3c->field_2A)
        {
            if (field_10_obj_3c->field_C == kFpZero_6F8E10)
            {
                return field_10_obj_3c->field_10;
            }
            return field_10_obj_3c->field_C;
        }
        else
        {
            if (field_10_obj_3c->field_C == kFpZero_6F8E10)
            {
                if (field_10_obj_3c->field_10 == kFpZero_6F8E10)
                {
                    if (this->field_18_model == 113)
                    {
                        return kFpZero_6F8E10;
                    }
                    return dword_6F8E14;
                }
                return field_10_obj_3c->field_10;
            }
            return field_10_obj_3c->field_C;
        }
    }
    return kFpZero_6F8E10;
}

MATCH_FUNC(0x525AC0)
char Object_2C::sub_525AC0()
{
    if (field_18_model == 113)
    {
        return field_C_pAny.pExplosion->IsState_5435D0();
    }
    else
    {
        return 0;
    }
}

MATCH_FUNC(0x525B20)
void Object_2C::UpdateEffectPool_525B20()
{
    if (field_10_obj_3c)
    {
        if (field_10_obj_3c->field_0.field_0_p18)
        {
            field_10_obj_3c->field_0.PoolUpdate_5A6F70(field_4);
        }
    }
}

MATCH_FUNC(0x527A30)
void Object_2C::UpdateLight_527A30()
{
    field_C_pAny.pLight->sub_45B2D0(field_C_pAny.pLight->field_18_intensity);
}

STUB_FUNC(0x523BF0)
void Object_2C::IntegrateMovementAndCollisions_523BF0(Fix16 a2, Ang16 a)
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x522FA0)
void Object_2C::Sprite_UpdateZFromSlopeAndTile_522FA0(Sprite* pSprite)
{
    Fix16 z_val = pSprite->field_1C_zpos;
    if (gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(pSprite->field_14_xy.x.ToInt(), pSprite->field_14_xy.y.ToInt(), (z_val.ToInt()) - 1))
    {
        z_val -= Fix16(0x4000, 0);
    }

    if (gMap_0x370_6F6268->UpdateZFromSlopeAtCoord_4E5BF0(pSprite->field_14_xy.x, pSprite->field_14_xy.y, z_val) == 0)
    {
        Fix16 z_frac = z_val.GetFracValue();
        if (z_frac != kFpZero_6F8E10)
        {
            z_val = z_val.GetRoundValue();
            if (z_frac > dword_6F8D10)
            {
                z_val += Fix16(0x4000, 0);
            }

            if (z_val > k_dword_6F8BFC)
            {
                z_val = k_dword_6F8BFC;
            }
        }
    }

    pSprite->set_z_lazy_420660(z_val);
}

WIP_FUNC(0x525100)
void Object_2C::sub_525100()
{
    WIP_IMPLEMENTED;

    if (field_8->field_34_behavior_type <= 1u)
    {
        if (get_model_40FEF0() == 148)
        {
            sub_5290C0(1u);
        }
        else
        {
            sub_5290A0();
        }
    }
}

MATCH_FUNC(0x529300)
void Object_5C::sub_529300()
{
    for (s32 i = field_14_sprites_in_list - 88; i >= 0; i--)
    {
        Sprite* pSprite = field_1C.PopBackSprite_5A6DC0();
        Object_2C* o2c = pSprite->As2C_40FEC0();
        if (o2c->field_18_model == 10)
        {
            if (gGame_0x40_67E008->IsSpriteOnScreenForAnyPlayer_4B97E0(pSprite, kFpZero_6F8E10))
            {
                Object_5C::CreateExplosion_52A3D0(pSprite->field_14_xy.x,
                                                  pSprite->field_14_xy.y,
                                                  pSprite->field_1C_zpos,
                                                  kZeroAng_6F8F68,
                                                  18,
                                                  gVarrok_7F8_703398->field_0[o2c->field_26_varrok_idx].field_0_ped_id);
            }
        }
        o2c->Dealloc_5291B0();
    }
}

MATCH_FUNC(0x5293a0)
void Object_5C::ObjectsService_5293A0()
{
    sub_529300();

    gObject_2C_Pool_6F8F80->UpdatePool();
}

MATCH_FUNC(0x529430)
Object_5C::Object_5C()
{
    for (u8 i = 0; i < 50; i++)
    {
        field_20_bUnCollectedTokens[i] = 1;
    }

    field_54_uncollected_token_index = 0;
    byte_6F8C68 = 0;
    byte_6F8C4C = 0;
    byte_6F8F40 = 0;
    field_10_rotation_counter = 0;
    field_14_sprites_in_list = 0;
    gObj3C_id_6F8E54 = 0;
    dword_6F8F18 = 0;
    dword_6F8DC0 = 0;
    dword_6F8F0C = 0;
    field_58 = 0;
    field_18 = 0;
    byte_6F8EDC = 0;

    if (!gObject_2C_Pool_6F8F80)
    {
        gObject_2C_Pool_6F8F80 = new TObject_2C_Pool();
        if (!gObject_2C_Pool_6F8F80)
        {
            FatalError_4A38C0(0x20, "C:\\Splitting\\Gta2\\Source\\object.cpp", 4239);
        }
    }

    gObject_8_Pool_6F8F78 = new TObject_8_Pool();
    if (!gObject_8_Pool_6F8F78)
    {
        FatalError_4A38C0(0x20, "C:\\Splitting\\Gta2\\Source\\object.cpp", 4243);
    }

    gObject_3C_Pool_6F8F7C = new TObject_3C_Pool();
    if (!gObject_3C_Pool_6F8F7C)
    {
        FatalError_4A38C0(0x20, "C:\\Splitting\\Gta2\\Source\\object.cpp", 4245);
    }

    field_58 = gSprite_Pool_703818->get_new_sprite();
    field_58->set_xyz_lazy_451950(0, 0, 0);

    field_58->set_ang_lazy_420690(kZeroAng_6F8F68);
    field_58->AllocInternal_59F950(0, 0, 0);

    field_0 = 0;
    field_C = 0;
    field_8 = 0;
    field_4 = 0;
}

MATCH_FUNC(0x529750)
Object_5C::~Object_5C()
{
    if (field_58)
    {
        gSprite_Pool_703818->remove(field_58);
        field_58 = 0;
    }

    if (gObject_2C_Pool_6F8F80)
    {
        GTA2_DELETE_AND_NULL(gObject_2C_Pool_6F8F80);
    }

    if (gObject_8_Pool_6F8F78)
    {
        GTA2_DELETE_AND_NULL(gObject_8_Pool_6F8F78);
    }

    if (gObject_3C_Pool_6F8F7C)
    {
        GTA2_DELETE_AND_NULL(gObject_3C_Pool_6F8F7C);
    }

    field_0 = 0;
    field_C = 0;
    field_8 = 0;
    field_4 = 0;
}

MATCH_FUNC(0x5297f0)
void Object_5C::sub_5297F0()
{
    field_0 = Object_5C::NewPhysicsObj_5299B0(0xA6, 0, 0, 0, word_6F8D8C);
    field_0->field_26_varrok_idx = 45;

    field_4 = Object_5C::NewPhysicsObj_5299B0(0xA6, 0, 0, 0, dword_6F8D80);
    field_4->field_26_varrok_idx = 48;

    field_8 = Object_5C::NewPhysicsObj_5299B0(0xA6, 0, 0, 0, word_6F8D54);
    field_8->field_26_varrok_idx = 46;

    field_C = Object_5C::NewPhysicsObj_5299B0(0xA6, 0, 0, 0, dword_6F8CD0);
    field_C->field_26_varrok_idx = 47;
}

MATCH_FUNC(0x5298e0)
Object_2C* Object_5C::GetDirectionalObject_5298E0(s32 maybe_slope)
{
    Object_2C* result;
    switch (maybe_slope)
    {
        case 180:
        case 196:
            result = this->field_0;
            break;
        case 184:
        case 200:
            result = this->field_8;
            break;
        case 188:
        case 204:
            result = this->field_C;
            break;
        case 192:
        case 208:
            result = this->field_4;
            break;
        default:
            result = 0;
            break;
    }
    return result;
}

MATCH_FUNC(0x529950)
Object_2C* Object_5C::NewTouchPoint_529950(s32 object_type, Fix16 x, Fix16 y, Fix16 z, Ang16 rot, Fix16 w, Fix16 h, Fix16 a9)
{
    Object_2C* pNewObj = New_529C00(object_type, x, y, z, rot, 0);
    if (pNewObj)
    {
        Sprite* pSprite = pNewObj->field_4;
        Sprite_4C* sprite_4c_ptr = pSprite->field_C_sprite_4c_ptr;
        sprite_4c_ptr->field_0_width = w;
        sprite_4c_ptr->field_4_height = h;
        sprite_4c_ptr->field_8 = a9;
        pSprite->ResetZCollisionAndDebugBoxes_59E7B0();
        pNewObj->AssignToBucket_527AE0();
    }
    return pNewObj;
}

MATCH_FUNC(0x5299b0)
Object_2C* Object_5C::NewPhysicsObj_5299B0(s32 object_type, Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 maybe_rotation)
{
    Object_2C* pNewObj = New_529C00(object_type, xpos, ypos, zpos, maybe_rotation, 0);
    if (pNewObj)
    {
        pNewObj->AssignToBucket_527AE0();
    }
    return pNewObj;
}

MATCH_FUNC(0x5299f0)
Object_2C* Object_5C::sub_5299F0(s32 object_type, u32 varrok_idx, Fix16 xpos, Fix16 ypos, Fix16 zpos)
{
    Object_2C* pNewObj = New_529C00(object_type, xpos, ypos, zpos, kZeroAng_6F8F68, 0);
    if (pNewObj)
    {
        pNewObj->field_26_varrok_idx = varrok_idx;
        if (object_type == 279)
        {
            pNewObj->sub_5290A0();
        }
    }
    return pNewObj;
}

MATCH_FUNC(0x529a40)
Object_2C* Object_5C::NewLight_529A40(Fix16 xpos, Fix16 ypos, Fix16 zpos, s32 argb, Fix16 radius, u8 intensity)
{
    Object_2C* pNewObj = New_529C00(165, xpos, ypos, zpos, kZeroAng_6F8F68, 0);
    if (pNewObj)
    {
        pNewObj->field_C_pAny.pLight->sub_482D60(argb, radius, intensity);
    }
    return pNewObj;
}

MATCH_FUNC(0x529ab0)
Object_2C* Object_5C::NewLight_529AB0(s32 light_type, Fix16 xpos, Fix16 ypos, Fix16 zpos, u32 argb, Fix16 radius, u8 intensity)
{
    Object_2C* pNewObj = Object_5C::New_529C00(light_type, xpos, ypos, zpos, kZeroAng_6F8F68, 0);
    if (pNewObj)
    {
        pNewObj->field_C_pAny.pLight->sub_482D60(argb, radius, intensity);
    }
    return pNewObj;
}

MATCH_FUNC(0x529bc0)
Object_2C* Object_5C::sub_529BC0(s32 object_type, Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation)
{
    Object_2C* tmp = New_529C00(object_type, xpos, ypos, zpos, rotation, 1);
    if (tmp)
    {
        tmp->AssignToBucket_527AE0();
    }
    return tmp;
}

// 9.6f 0x484E00
MATCH_FUNC(0x529c00)
Object_2C* Object_5C::New_529C00(int object_type, Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation, char bUnknown)
{
    if (object_type == objects::secret_token_266)
    {
        if (!field_20_bUnCollectedTokens[field_54_uncollected_token_index]) // 20
        {
            field_54_uncollected_token_index++;
            return 0;
        }
    }

    Phi_74* pPhi = gPhi_8CA8_6FCF00->GetObjectDefinition_534360(object_type);
    if (pPhi->field_5C == 2)
    {
        if (field_10_rotation_counter == 360)
        {
            return 0;
        }
        field_10_rotation_counter++;
    }

    Object_2C* pNew2C; // esi
    if (pPhi->field_61) // 6c
    {
        pNew2C = gObject_2C_Pool_6F8F80->Allocate();
        pNew2C->field_20 = 1;
    }
    else
    {
        pNew2C = gObject_2C_Pool_6F8F80->AllocateUntracked();
        pNew2C->field_20 = 2;
    }

    pNew2C->InitializeObject_527630(object_type, xpos, ypos, zpos, rotation);
    if (field_18)
    {
        pNew2C->SetDamageOwner_529080(field_18);
        field_18 = 0;
    }

    if (bUnknown &&
        (pNew2C->field_4->QuerySpriteCollision_59E7D0(0) ||
         (pPhi->field_40_collision_bucket_category == collision_bucket_category::purple_doom_2_region_bucket_3 &&
          gPurpleDoom_2_67920C->FindNearestSpriteOfType_477E60(pNew2C->field_4, 0))))
    {
        if (pNew2C->field_20 == 1) // 154: ~> cmpl    $0x1,0x0(%ebp)
        {
            gObject_2C_Pool_6F8F80->unknown_func(pNew2C);
        }
        else
        {
            gObject_2C_Pool_6F8F80->DeAllocate(pNew2C);
        }
        return 0;
    }

    if (pPhi->field_5C == 3) // 1e0
    {
        ++field_14_sprites_in_list;
        field_1C.AddSprite_5A6CD0(pNew2C->field_4);
    }

    switch (pPhi->field_34_behavior_type)
    {

        case object_behavior_type::behavior_0:
        case object_behavior_type::behavior_1:
        case object_behavior_type::behavior_6:
        case object_behavior_type::behavior_10:
        case object_behavior_type::behavior_12:
            pNew2C->field_C_pAny.o8 = 0;
            pNew2C->field_10_obj_3c = 0;
            break;

        case object_behavior_type::behavior_5:
        {
            Wolfy_30* pNew30; // eax
            pNew30 = gWolfy_7A8_6FD5F0->New_40_543800();
            pNew2C->field_C_pAny.pExplosion = pNew30;
            if (pNew30) // 225
            {
                pNew2C->field_1C = 1;
            }
            else
            {
                return 0;
            }

            break;
        }

        case object_behavior_type::behavior_2:
        case object_behavior_type::behavior_8:
        {
            Object_8* pNew8; // eax
            pNew8 = gObject_8_Pool_6F8F78->Allocate();
            pNew2C->field_C_pAny.o8 = pNew8;
            pNew2C->field_C_pAny.o8->field_7_anim_speed_counter = 0;
            pNew2C->field_C_pAny.o8->field_4_timer = pPhi->field_65;
            pNew2C->field_C_pAny.o8->field_6_frame_counter = 0;
            break;
        }

        case object_behavior_type::behavior_3:
        case object_behavior_type::behavior_7:
        {
            Object_3C* pNew3C; // eax
            pNew3C = gObject_3C_Pool_6F8F7C->Allocate();
            pNew2C->field_10_obj_3c = pNew3C;
            pNew3C->field_20 = pNew2C->field_14_id;
            pNew2C->field_10_obj_3c->field_C = pNew2C->field_8->field_10;
            pNew2C->field_10_obj_3c->field_18 = pPhi->field_14_friction;
            pNew2C->field_10_obj_3c->field_4 = rotation;
            pNew2C->field_10_obj_3c->field_28 = pNew2C->field_8->field_65;
            pNew2C->field_10_obj_3c->field_10 = kFpZero_6F8E10;
            pNew2C->field_10_obj_3c->field_1C = kFpZero_6F8E10;
            break;
        }

        case object_behavior_type::behavior_4:
        case object_behavior_type::behavior_9:
        {
            Object_3C* pNew3C; // eax
            pNew3C = gObject_3C_Pool_6F8F7C->Allocate();
            pNew2C->field_10_obj_3c = pNew3C;
            pNew3C->field_20 = pNew2C->field_14_id;
            pNew2C->field_10_obj_3c->field_C = pNew2C->field_8->field_10;
            pNew2C->field_10_obj_3c->field_18 = pPhi->field_14_friction;
            pNew2C->field_10_obj_3c->field_10 = kFpZero_6F8E10;
            pNew2C->field_10_obj_3c->field_1C = kFpZero_6F8E10;
            pNew2C->field_10_obj_3c->field_4 = rotation;
            pNew2C->field_10_obj_3c->field_28 = pNew2C->field_8->field_65;

            Object_8* pNew8; // eax
            pNew8 = gObject_8_Pool_6F8F78->Allocate();
            pNew2C->field_C_pAny.o8 = pNew8;
            pNew2C->field_C_pAny.o8->field_7_anim_speed_counter = 0; // ??
            pNew2C->field_C_pAny.o8->field_4_timer = pPhi->field_65;
            pNew2C->field_C_pAny.o8->field_6_frame_counter = 0;
            break;
        }

        case object_behavior_type::behavior_11:
            pNew2C->field_C_pAny.pLight = gLight_1D4CC_6F5520->sub_52B2A0(xpos, ypos, zpos, 0, 0, 0);
            break;

        default:
            break;
    }
    ++dword_6F8F88;

    pNew2C->field_18_model = object_type;
    if (pNew2C->check_is_shop_421060())
    {
        pNew2C->get_weapon_default_ammo_5292D0();
    }

    if (pNew2C->field_18_model == 281)
    {
        Object_2C* v34 = NewPhysicsObj_5299B0(284, kFpZero_6F8E10, kFpZero_6F8E10, kFpZero_6F8E10, kZeroAng_6F8F68);
        pNew2C->field_4->DispatchCollisionEvent_5A3100(v34->field_4,
                                                       (dword_6F8CE8 * dword_6F8ECC), // x?
                                                       (dword_6F8CEC * dword_6F8ECC), // y?
                                                       kZeroAng_6F8F68); // ang?
    }
    else
    {
        if (pNew2C->field_18_model == objects::secret_token_266)
        {
            pNew2C->set_field_26(field_54_uncollected_token_index);
            field_54_uncollected_token_index++;
        }
    }
    return pNew2C;
}

MATCH_FUNC(0x52a210)
char_type Object_5C::sub_52A210(char_type a2)
{
    // TODO: Why is it a byte here? enum ??
    *(u8*)&field_18 = a2;
    return a2;
}

MATCH_FUNC(0x52a240)
Object_2C* Object_5C::NewUnknown_52A240(s32 object_type,
                                        Fix16 maybe_x,
                                        Fix16 maybe_y,
                                        Fix16 maybe_z,
                                        Ang16 unk_ang,
                                        Ang16 maybe_ang,
                                        Fix16 a8,
                                        Fix16 a9,
                                        Fix16 a10)
{
    return Object_5C::New_52A2C0(object_type, maybe_x, maybe_y, maybe_z, unk_ang, maybe_ang, a8, a9, a10, 0);
}

MATCH_FUNC(0x52a280)
Object_2C* Object_5C::sub_52A280(s32 object_type,
                                 Fix16 xpos,
                                 Fix16 ypos,
                                 Fix16 zpos,
                                 Ang16 unk_ang,
                                 Ang16 rotation,
                                 Fix16 a8,
                                 Fix16 a9,
                                 Fix16 a10)
{
    return Object_5C::New_52A2C0(object_type, xpos, ypos, zpos, unk_ang, rotation, a8, a9, a10, 1);
}

MATCH_FUNC(0x52a2c0)
Object_2C* Object_5C::New_52A2C0(s32 object_type,
                                 Fix16 xpos,
                                 Fix16 ypos,
                                 Fix16 zpos,
                                 Ang16 unk_ang,
                                 Ang16 maybe_rotation,
                                 Fix16 a8,
                                 Fix16 a9,
                                 Fix16 a10,
                                 char_type a11)
{
    Object_2C* pNewObj = Object_5C::New_529C00(object_type, xpos, ypos, zpos, maybe_rotation, a11);

    if (pNewObj)
    {
        pNewObj->AssignToBucket_527AE0();
        Phi_74* pPhi74 = gPhi_8CA8_6FCF00->GetObjectDefinition_534360(object_type);
        if (!pNewObj->field_10_obj_3c)
        {
            Object_3C* p3C = gObject_3C_Pool_6F8F7C->Allocate();
            pNewObj->field_10_obj_3c = p3C;
            if (!p3C)
            {
                pNewObj->sub_5290A0();
                return 0;
            }
        }
        pNewObj->field_10_obj_3c->field_C = a8;
        pNewObj->field_10_obj_3c->field_10 = a10;
        pNewObj->field_10_obj_3c->field_14 = a8;
        pNewObj->field_10_obj_3c->field_1C = dword_6F8DA8;
        pNewObj->field_10_obj_3c->field_18 = a9;
        pNewObj->field_10_obj_3c->field_4 = unk_ang;
        pNewObj->field_10_obj_3c->field_28 = pPhi74->field_65;
        Object_3C* field_10_obj_3c = pNewObj->field_10_obj_3c;
        if (field_10_obj_3c->field_10 != kFpZero_6F8E10)
        {
            field_10_obj_3c->field_2A = 1;
        }
    }
    return pNewObj;
}

MATCH_FUNC(0x52a3d0)
Object_2C* Object_5C::CreateExplosion_52A3D0(Fix16 x, Fix16 y, Fix16 z, Ang16 rot, s32 a6, s32 pedId)
{
    Object_2C* pNew2C = NewPhysicsObj_5299B0(objects::moving_collect_17_113, x, y, z, rot);
    if (pNew2C)
    {
        pNew2C->field_1C = 1;

        if (!pNew2C->field_C_pAny.pExplosion)
        {
            pNew2C->field_C_pAny.pExplosion = gWolfy_7A8_6FD5F0->New_40_543800();
        }

        if (!pNew2C->field_C_pAny.pExplosion)
        {
            return 0;
        }

        pNew2C->field_C_pAny.pExplosion->field_10_type_or_state = a6;
        pNew2C->field_C_pAny.pExplosion->Set_Obj2C_543680(pNew2C);
        pNew2C->field_C_pAny.pExplosion->field_2C_ped_id = pedId;

        switch (a6)
        {
            case 18:
            case 19:
            case 20:
            case 22:
            case 23:
            case 24:
            case 25:
            case 32:
            case 33:
                pNew2C->field_C_pAny.pExplosion->field_1A = 100;
                pNew2C->field_C_pAny.pExplosion->field_24 = 0;
                break;
            default:
                pNew2C->field_C_pAny.pExplosion->field_1A = 9999;
                break;
        }

        if (!pNew2C->field_10_obj_3c)
        {
            Object_3C* pNew3C = gObject_3C_Pool_6F8F7C->Allocate();
            pNew2C->field_10_obj_3c = pNew3C;
            pNew3C->field_20 = pNew2C->field_14_id;
            pNew2C->field_10_obj_3c->field_C = kFpZero_6F8E10;
            pNew2C->field_10_obj_3c->field_10 = kFpZero_6F8E10;
        }
        return pNew2C;
    }
    return 0;
}

STUB_FUNC(0x52A500)
void Object_5C::SaveObjects_52A500(TurkishDelight_164* pUnknownObj)
{
    NOT_IMPLEMENTED;
}

// TODO: This is a mess, probably a fake match
// https://decomp.me/scratch/juEUq
MATCH_FUNC(0x52A590)
void Object_5C::RestoreObjects_52A590(TurkishDelight_164* pData)
{
    u8* pVarrok = &pData->field_118_varrok_idx[0];
    // TurkishDelight_4* pX = &pData->field_0_obj_x[0][0];
    u16* pModel = &pData->field_F0_model[0];

    s32 i = 19;
    while (i >= 0)
    {
        if (pData->field_0_obj_x[0][19 - i].field_0 != kFpZero_6F8E10 &&
            pData->field_0_obj_x[1][19 - i].field_0 != kFpZero_6F8E10) // x and y not zero?
        {
            Object_2C* pNewObj = NewPhysicsObj_5299B0(*pModel,
                                                      pData->field_0_obj_x[0][19 - i].field_0, // x?
                                                      pData->field_0_obj_x[1][19 - i].field_0,
                                                      pData->field_0_obj_x[2][19 - i].field_0, // z?
                                                      kZeroAng_6F8F68);
            pNewObj->set_field_26(*pVarrok);
        }
        // pX++;
        pModel++; // add $0x2,$esi
        i--;
        pVarrok++;
    }

    memset(pData, 0, 0x12Cu); // everything before field_12C_obj_5C_bUnCollectedTokens
}

MATCH_FUNC(0x52A610)
void Object_5C::RemoveAndFree_52A610(Object_2C* p2C)
{
    if (p2C->field_8->field_34_behavior_type != 11)
    {
        gPurpleDoom_3_679210->Remove_477B00(p2C->field_4);
    }
    gObject_2C_Pool_6F8F80->DeAllocate(p2C);
}

MATCH_FUNC(0x52A650)
void Object_2C::EnsureObject3C_52A650()
{
    if (!field_10_obj_3c)
    {
        Object_3C* p3C = gObject_3C_Pool_6F8F7C->Allocate();
        field_10_obj_3c = p3C;
        p3C->field_20 = field_14_id;
        field_10_obj_3c->field_C = kFpZero_6F8E10;
        field_10_obj_3c->field_10 = kFpZero_6F8E10;
    }
    Object_2C::PoolGive_522340();
}

MATCH_FUNC(0x52a6d0)
void Object_2C::ReactivateObjectAfterImpact_52A6D0(Sprite* pSprite)
{
    RemoveFromCollisionBuckets_527D00();

    if (field_8->field_34_behavior_type != 11)
    {
        gPurpleDoom_3_679210->AddToSingleBucket_477AE0(field_4);
    }

    PoolTake_522360();

    Car_BC* pObj = pSprite->AsCar_40FEB0();
    if (pObj)
    {
        field_4->field_28_num = pObj->GetCrashSoundCategory_4435B0();
    }
}