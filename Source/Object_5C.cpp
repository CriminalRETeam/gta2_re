// GetLength_41E260 compares against this TU's zero
#define FIX16_POINT_ZERO kFpZero_6F8E10
#include "Object_5C.hpp"
#include "explosion_type.hpp"
#include "ped_death_cause.hpp"
#include "CarPhysics.hpp"
#include "Car.hpp"
#include "Char_Pool.hpp"
#include "DoorPool.hpp"
#include "Game.hpp"
#include "Globals.hpp"
#include "LightPool.hpp"
#include "ObjectPool.hpp"
#include "Object_3C_Pool.hpp"
#include "Object_8_Pool.hpp"
#include "ParticleManager.hpp"
#include "ObjectDefinitions.hpp"
#include "SpriteGrid.hpp"
#include "CollisionSoundQueue.hpp"
#include "TrafficLights.hpp"
#include "PedRefTable.hpp"
#include "WeaponManager.hpp"
#include "Explosion.hpp"
#include "collide.hpp"
#include "enums.hpp"
#include "error.hpp"
#include "ScriptManager.hpp"
#include "map_0x370.hpp"
#include "sprite.hpp"
#include "block_face.hpp"

DEFINE_GLOBAL(Object_5C*, gObject_5C_6F8F84, 0x6F8F84);
DEFINE_GLOBAL(s32, dword_6F8F88, 0x6f8f88);
DEFINE_GLOBAL(s32, gObject2C_WallHitSide_6F8F90, 0x6F8F90);
DEFINE_GLOBAL_INIT(Ang16, kAng180_6F8D62, Ang16(0x2D0), 0x6F8D62);

DEFINE_GLOBAL(Fix16_Point, gZeroVector_6F8EF0, 0x6F8EF0);
DEFINE_GLOBAL_INIT(Fix16, kFpZero_6F8E10, Fix16(0), 0x6F8E10);
DEFINE_GLOBAL_INIT(Fix16, kFpOne64th_6F8C9C, Fix16(0x100, 0), 0x6F8C9C);

DEFINE_GLOBAL(u8, byte_6F8C68, 0x6F8C68);
DEFINE_GLOBAL(u8, byte_6F8C4C, 0x6F8C4C);
DEFINE_GLOBAL(u8, byte_6F8F40, 0x6F8F40);
DEFINE_GLOBAL(u32, gObj3C_live_count_6F8E54, 0x6F8E54);
DEFINE_GLOBAL(u32, gObj8_count_6F8F18, 0x6F8F18);
DEFINE_GLOBAL(u32, gObj8_dealloc_count_6F8DC0, 0x6F8DC0);
DEFINE_GLOBAL(u32, gObj3C_dealloc_count_6F8F0C, 0x6F8F0C);
DEFINE_GLOBAL(u8, byte_6F8EDC, 0x6F8EDC);

DEFINE_GLOBAL_INIT(Ang16, kAng315_6F8D8C, Ang16(0x4EC), 0x6F8D8C);
DEFINE_GLOBAL_INIT(Ang16, kAng135_6F8D80, Ang16(0x21C), 0x6F8D80);
DEFINE_GLOBAL_INIT(Ang16, kAng225_6F8D54, Ang16(0x384), 0x6F8D54);
DEFINE_GLOBAL_INIT(Ang16, kAng45_6F8CD0, Ang16(0xB4), 0x6F8CD0);

DEFINE_GLOBAL(Sprite*, gObject2C_LastCollisionSprite_6F8F8C, 0x6F8F8C);
DEFINE_GLOBAL(u8, gObject2C_HitWallOrDoor_6F8F94, 0x6F8F94);
DEFINE_GLOBAL_INIT(Fix16, kFpOneThirtySecond_6F8F5C, Fix16(0x200, 0), 0x6F8F5C);
DEFINE_GLOBAL_INIT(Fix16, kObjectGravity_6F8DA8, Fix16(0xFFFFFFF8, 0), 0x6F8DA8);
DEFINE_GLOBAL_INIT(Fix16, kFP16One_6F8E14, Fix16(1), 0x6F8E14);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint1_6F8C58, Fix16(0x666, 0), 0x6F8C58);
DEFINE_GLOBAL_INIT(u8, byte_623EC4, 1, 0x623EC4);

DEFINE_GLOBAL_INIT(Fix16, kFpOne64th_6F8DC8, Fix16(256, 0), 0x6F8DC8);
DEFINE_GLOBAL_INIT(Fix16, kFpEighteen_6F8CE8, Fix16(18), 0x6F8CE8);
DEFINE_GLOBAL_INIT(Fix16, kFP16One_6F8CEC, Fix16(1), 0x6F8CEC);
DEFINE_GLOBAL_INIT(Fix16, kFpOne64th_6F8ECC, kFpOne64th_6F8DC8, 0x6F8ECC);

DEFINE_GLOBAL_INIT(Ang16, kAng90_6F8C88, Ang16(0x168), 0x6F8C88); // TODO: Init via func 0x5269F0
DEFINE_GLOBAL_INIT(Ang16, kAng270_6F8D88, Ang16(0x438), 0x6F8D88); // TODO: Init via func 0x526E70

DEFINE_GLOBAL_INIT(Fix16, kFpOne256th_6F8CF0, Fix16(0x40, 0), 0x6F8CF0);
DEFINE_GLOBAL_INIT(Fix16, kMaxZ_6F8BFC, Fix16(0x1FF00, 0), 0x6F8BFC);
DEFINE_GLOBAL_INIT(Fix16, kFP16Half_6F8D10, Fix16(0x2000, 0), 0x6F8D10);

DEFINE_GLOBAL_INIT(u8, byte_6771DC, 0, 0x6771DC);
DEFINE_GLOBAL_INIT(s32, gObj2C_id_623EC0, 1, 0x623EC0);

DEFINE_GLOBAL_INIT(Fix16, kFP16One_6F8D38, kFP16One_6F8E14, 0x6F8D38);
DEFINE_GLOBAL_INIT(Fix16, kFP16One_6F8CE0, kFP16One_6F8E14, 0x6F8CE0);
DEFINE_GLOBAL_INIT(Fix16, kFP16One_6F8F74, kFP16One_6F8E14, 0x6F8F74);
DEFINE_GLOBAL_INIT(Fix16, kFP16Half_6F8EE4, kFP16Half_6F8D10, 0x6F8EE4);

DEFINE_GLOBAL_INIT(Fix16, kFP16Half_6F8D3C, kFP16Half_6F8D10, 0x6F8D3C);
DEFINE_GLOBAL_INIT(Fix16, kFP16MinusOne_6F8BE8, Fix16(-1), 0x6F8BE8); // Fix16(0xFFFFC000, 0)

DEFINE_GLOBAL_INIT(Fix16, kFP16Half_6F8F2C, kFP16Half_6F8D10, 0x6F8F2C);
DEFINE_GLOBAL_INIT(Fix16, kFP16One_6F8D24, kFP16One_6F8E14, 0x6F8D24);

DEFINE_GLOBAL(Fix16, gObj2C_LastValidX_6F8F00, 0x6F8F00);
DEFINE_GLOBAL(Fix16, gObj2C_LastValidY_6F8EF8, 0x6F8EF8);
DEFINE_GLOBAL(Fix16, gObj2C_LastValidZ_6F8EFC, 0x6F8EFC);
DEFINE_GLOBAL_INIT(Ang16, gObj2C_LastValidAng_6F8D1C, Ang16(0), 0x6F8D1C);
DEFINE_GLOBAL_INIT(Fix16, kMapSize_6F8BF4, Fix16(256), 0x6F8BF4);

DEFINE_GLOBAL_INIT(Fix16, kFpPoint7_6F8C78, Fix16(0x2CCC, 0), 0x6F8C78);
DEFINE_GLOBAL_INIT(Fix16, kObjectBounceRestitution_6F8DBC, kFpPoint7_6F8C78, 0x6F8DBC);

// 9.6f 0x482C10
inline bool Object::IsAnimFinished_482C10() const
{
    return !field_C_pAny.o8->field_4_timer && !field_C_pAny.o8->field_7_frame_idx;
}

MATCH_FUNC(0x522140)
Object::Object()
{
    mpNext = NULL;
    field_4_sprite = NULL;
    field_18_model = 0;
    field_8_definition = NULL;
    field_C_pAny.o8 = NULL;
    field_10_obj_3c = NULL;
    field_14_id = 99;
    field_24_touched_result = 0;
    field_25_removal_state = object_removal_state::none_0;
    field_26_ped_ref_idx = 99;
    field_20_pool_list_state = object_pool_state::none_0;
    field_27_thread_trigger_idx = 0;
    field_28_unused = -51;
    field_1C_has_explosion = false;
}

MATCH_FUNC(0x522180)
void Object::PoolDeallocate()
{
    this->field_18_model = 0;

    // TODO: Local required, inline?
    const s32 limit_kind = field_8_definition->field_5C_instance_limit_kind;
    if (limit_kind == 2)
    {
        --gObject_5C_6F8F84->field_10_limited_object_count;
    }
    else if (limit_kind == 3)
    {
        --gObject_5C_6F8F84->field_14_listed_sprite_count;
        gObject_5C_6F8F84->field_1C_sprite_list.RemoveSpriteSafe_5A6B60(this->field_4_sprite);
    }

    --dword_6F8F88;

    if (UsesParamAsPedRef_421080())
    {
        if (field_26_ped_ref_idx > 0)
        {
            gPedRefTable_7F8_703398->DecrementRefCount_59B0D0(field_26_ped_ref_idx);
            this->field_26_ped_ref_idx = 0;
        }
    }

    Object::ReleaseSubObjects_527F10();
    if (field_4_sprite)
    {
        gSpritePool_703818->remove(field_4_sprite);
        this->field_4_sprite = NULL;
    }
    this->field_14_id = 0;
}

MATCH_FUNC(0x522250)
bool Object::IsSpriteOwnerPed_522250(Sprite* pSprite) const
{
    if (UsesParamAsPedRef_421080())
    {
        u8 ped_ref_idx = this->get_field_26_420FF0();
        if (ped_ref_idx > 0)
        {
            if (pSprite)
            {
                Char* pChar = pSprite->AsCharB4_40FEA0();
                if (pChar)
                {
                    if (pChar->field_7C_pPed->get_ped_ref_idx_420B50() == ped_ref_idx)
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
s32 Object::GetMovementRegionSize_5222B0() const
{
    return this->field_8_definition->field_68_uses_bullet_walls != 0 ? 2048 : 1024;
}

MATCH_FUNC(0x5222d0)
void Object::UpdatePhysics_5222D0()
{
    if (field_10_obj_3c->field_2A_bAirborne == 1)
    {
        field_10_obj_3c->field_1C_z_accel += kObjectGravity_6F8DA8;
        field_10_obj_3c->field_10_z_speed += field_10_obj_3c->field_1C_z_accel;
    }
    else
    {
        field_10_obj_3c->field_C_speed += field_10_obj_3c->field_18_friction;

        if (field_10_obj_3c->field_C_speed < kFpZero_6F8E10)
        {
            field_10_obj_3c->field_C_speed = kFpZero_6F8E10;
            field_10_obj_3c->field_18_friction = kFpZero_6F8E10;
        }

        if (field_10_obj_3c->field_10_z_speed < kFpZero_6F8E10)
        {
            field_10_obj_3c->field_10_z_speed = kFpZero_6F8E10;
        }
    }
}

MATCH_FUNC(0x522340)
void Object::PoolGive_522340()
{
    if (field_20_pool_list_state == object_pool_state::out_of_pool_list_2)
    {
        // TODO: Missing inline here
        TObjectPool* pRoot = gObject_2C_Pool_6F8F80;
        mpNext = pRoot->field_4_pPrev;
        pRoot->field_4_pPrev = this;
        field_20_pool_list_state = object_pool_state::in_pool_list_1;
    }
}

MATCH_FUNC(0x522360)
void Object::PoolTake_522360()
{
    Object* pLast;
    Object* pIter;

    if (field_20_pool_list_state == object_pool_state::in_pool_list_1)
    {
        pLast = NULL;
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
                pIter->mpNext = NULL;
                break;
            }
            else
            {
                pLast = pIter;
            }
        }
        field_20_pool_list_state = object_pool_state::out_of_pool_list_2;
    }
}

MATCH_FUNC(0x5223c0)
bool Object::ShouldCollideWith_5223C0(Sprite* pSprite) const
{
    s32 sprite_type;

    if (!pSprite)
    {
        return false;
    }
    switch (field_8_definition->field_54_react_to_collisions_with)
    {
        case CollisionReaction::Always_0:
            // Always
            return true;
        case CollisionReaction::OnlyCars_1:
            // Only cars
            if (pSprite->GetType_416B40() == sprite_types_enum::car_2)
            {
                return false;
            }
            break;
        case CollisionReaction::OnlyPeds_2:
            // Only peds
            if (pSprite->GetType_416B40() == sprite_types_enum::ped_3)
            {
                return false;
            }
            break;
        case CollisionReaction::OnlyObjects_3:
            // Only objects?
            sprite_type = pSprite->GetType_416B40();
            if (sprite_type != sprite_types_enum::code_obj1_4 && sprite_type != sprite_types_enum::map_obj_5 &&
                sprite_type != sprite_types_enum::object_1)
            {
                return true;
            }
            else
            {
                return false;
            }
            break;
        case CollisionReaction::Never_4:
            // Never
            return false;
            break;
        default:
            return true;
            break;
    }
    return true;
}

MATCH_FUNC(0x522430)
bool Object::ShouldCollideWithSprite_522430(Sprite* pSprite) const
{
    return (pSprite && ShouldCollideWith_5223C0(pSprite) && !IsSpriteOwnerPed_522250(pSprite)) ? true : false;
}

MATCH_FUNC(0x522460)
bool Object::SelectCollisionSprite_522460(Sprite* pProbe)
{
    gObject2C_HitWallOrDoor_6F8F94 = 0;

    Sprite* pSprite = pProbe->QuerySpriteCollision_59E7D0(2);
    if (pSprite && gObject2C_LastCollisionSprite_6F8F8C && pSprite->GetType_416B40() == sprite_types_enum::car_2 // IsCar
        || !ShouldCollideWithSprite_522430(pSprite) || pSprite == gObject2C_LastCollisionSprite_6F8F8C)
    {
        return false;
    }

    if (pSprite->field_8_object_2C_ptr->field_18_model == objects::diagonal_wall_collision_obj_166 ||
        pSprite->field_8_object_2C_ptr->field_18_model == objects::door_collision_obj_169)
    {
        gObject2C_HitWallOrDoor_6F8F94 = 1;
    }
    gCollisionTarget_679188.SetSprite_40FEE0(pSprite);
    return true;
}

MATCH_FUNC(0x5224e0)
void Object::SetMovementVector_5224E0(Fix16_Point& speed)
{
    Fix16_Point new_velocity;
    if (field_10_obj_3c)
    {
        new_velocity = GetSpeedVector_482C50() + speed;
        this->field_10_obj_3c->field_C_speed = new_velocity.GetLength_41E260();
        this->field_10_obj_3c->field_4_angle = new_velocity.atan2_40F790();
        this->field_10_obj_3c->field_18_friction = this->field_8_definition->field_14_friction;
    }
    else
    {
        NewObj3C_528130(speed);
    }
}

MATCH_FUNC(0x522640)
void Object::SetMovementVectorWithRandomState_522640(Fix16_Point& point)
{
    SetMovementVector_5224E0(point);

    if (field_8_definition->field_4C == 3 && field_10_obj_3c->field_34 == 2)
    {
        const s16 rng = gRng_6F6784.get_int_4F7AE0(9);
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

MATCH_FUNC(0x5226a0)
void Object::SetConveyorPush_5226A0(char_type conveyor_speed)
{
    Fix16_Point point;
    if (!field_10_obj_3c)
    {
        NewObj3C_528130(gZeroVector_6F8EF0);
    }
    field_10_obj_3c->field_38_conveyor_speed = conveyor_speed;
}

MATCH_FUNC(0x522710)
void Object::ResolveCollisionWithObject_522710(Object* pObj, Fix16_Point* pPoint)
{
    Fix16_Point offset;
    Fix16_Point rel_velocity;
    Fix16_Point impulse;

    Fix16_Point other_pos = pObj->GetXY_52AE70();
    Fix16_Point my_pos = GetXY_52AE70();

    Ang16 impact_angle = 0;
    offset = (my_pos - *pPoint);

    if (pObj->CanBePushed_482C90())
    {
        rel_velocity = (GetSpeedVector_52AE90() - pObj->GetSpeedVector_52AE90());
        impulse = ComputeCollisionImpulse_55F3B0(GetMass_482C80(),
                                                 pObj->GetMass_482C80(),
                                                 rel_velocity,
                                                 offset,
                                                 *pPoint,
                                                 my_pos,
                                                 other_pos,
                                                 kFP16One_6F8D38,
                                                 kFP16One_6F8D38,
                                                 kFP16One_6F8D24);

        pObj->SetMovementVectorWithRandomState_522640(impulse.Negate_40ACB0() / pObj->GetMass_482C80());
    }
    else
    {
        rel_velocity = GetSpeedVector_52AE90();
        if (offset.x == kFpZero_6F8E10 && offset.y == kFpZero_6F8E10)
        {
            offset = rel_velocity;
        }

        impulse = ComputeCollisionImpulse_55F3B0(GetMass_482C80(),
                                                 kFP16MinusOne_6F8BE8,
                                                 rel_velocity,
                                                 offset,
                                                 *pPoint,
                                                 my_pos,
                                                 other_pos,
                                                 kFP16One_6F8D38,
                                                 kFP16One_6F8D38,
                                                 kFP16Half_6F8F2C);
    }

    if (field_10_obj_3c)
    {
        impact_angle = field_10_obj_3c->field_4_angle;
    }

    SetMovementVectorWithRandomState_522640(impulse / GetMass_482C80());

    if (gObject2C_HitWallOrDoor_6F8F94)
    {
        if (field_10_obj_3c)
        {
            if (ComputeShortestAngleDelta_4056C0(field_10_obj_3c->field_4_angle, impact_angle) < kAng90_6F8C88)
            {
                Ang16& angle = field_10_obj_3c->field_4_angle;
                angle.rValue += kAng180_6F8D62.rValue;
                angle.Normalize_406C20();
            }
        }
    }

    pObj->HandleImpact_528E50(field_4_sprite);
    HandleImpact_528E50(pObj->field_4_sprite);
}

// 9.6f 0x4867E0
MATCH_FUNC(0x5229b0)
void Object::ResolveCollisionWithPed_5229B0(Char* pChar, Fix16_Point* pPoint, u8 not_used)
{
    Fix16_Point posDelta;
    Fix16_Point impulse;
    Fix16_Point ped_impulse;
    Ped* pPed = pChar->field_7C_pPed;
    Fix16_Point camPos(pPed->get_cam_x(), pPed->get_cam_y());

    Fix16_Point spritePos = GetXY_52AE70();
    posDelta = (spritePos - *pPoint);

    impulse = ComputeCollisionImpulse_55F3B0(GetMass_482C80(),
                                                    kFP16Half_6F8EE4,
                                                    pChar->GetRunVelocity_545580(),
                                                    posDelta,
                                                    *pPoint,
                                                    spritePos,
                                                    camPos,
                                                    kFP16One_6F8D38,
                                                    kFP16One_6F8CE0,
                                                    kFP16One_6F8F74);
    SetMovementVectorWithRandomState_522640(impulse.Divide_442CB0(GetMass_482C80()));
    ped_impulse = impulse.Negate_40ACB0().Divide_442CB0(kFP16Half_6F8EE4);
    // The ped impact was compiled out: ped_impulse is overwritten with zero
    ped_impulse.x = kFpZero_6F8E10;
    ped_impulse.y = kFpZero_6F8E10;

    HandleImpact_528E50(pChar->field_80_sprite_ptr); // TODO: sub_4338D0
}

MATCH_FUNC(0x522b20)
void Object::ResolveCollisionWithWorld_522B20(Fix16_Point* pCollisionPoint, Fix16_Point* pPoint, Fix16_Point* speed)
{
    // GetXY_52AE70() is a temporary argument, not a local: that gives the original EH states (0, 2, 3) and frame
    Fix16_Point impulse = ComputeCollisionImpulse_55F3B0(GetMass_482C80(),
                                                        kFP16MinusOne_6F8BE8,
                                                        *speed,
                                                        *pPoint,
                                                        *pCollisionPoint,
                                                        GetXY_52AE70(),
                                                        gZeroVector_6F8EF0,
                                                        kFP16One_6F8D38,
                                                        kFpZero_6F8E10,
                                                        kFP16Half_6F8D3C);
    SetMovementVectorWithRandomState_522640(impulse.Divide_442CB0(GetMass_482C80()));
    HandleImpact_528E50(NULL);
}

MATCH_FUNC(0x522be0)
void Object::ResolveCollisionWithHorizontalEdge_522BE0(Fix16_Point* pPoint)
{
    Fix16_Point reflect_dir;
    Fix16_Point hit_point;
    Fix16_Point obj_speed;
    obj_speed = GetSpeedVector_52AE90();
    u8 edge_flag;
    if (this->field_4_sprite->GetNearestHorizontalEdgeToCoordinate_5A0A70(gCollisionTarget_679188.field_18_hseg_y, hit_point, edge_flag))
    {
        reflect_dir.SetXY_432860(Fix16(0), field_4_sprite->field_14_xy.y - gCollisionTarget_679188.field_18_hseg_y);
    }
    else
    {
        hit_point.SetXY_432860(gCollisionTarget_679188.field_14_vseg_x, gCollisionTarget_679188.field_18_hseg_y);
        hit_point -= *pPoint;
        hit_point.x += field_4_sprite->field_14_xy.x;
        hit_point.y = gCollisionTarget_679188.field_18_hseg_y;

        Fix16 abs1 = Fix16::Abs(hit_point.x - gCollisionTarget_679188.field_8_hseg_x_max);
        Fix16 abs2 = Fix16::Abs(hit_point.x - gCollisionTarget_679188.field_4_hseg_x_min);

        if (abs2 < abs1)
        {
            hit_point.x = gCollisionTarget_679188.field_4_hseg_x_min;
        }
        else
        {
            hit_point.x = gCollisionTarget_679188.field_8_hseg_x_max;
        }

        reflect_dir = -obj_speed;
    }

    ResolveCollisionWithWorld_522B20(&hit_point, &reflect_dir, &obj_speed);
}

// https://decomp.me/scratch/PMCb4
MATCH_FUNC(0x522d00)
void Object::ResolveCollisionWithVerticalEdge_522D00(Fix16_Point* pPoint)
{
    u8 edge_flag;
    Fix16_Point reflect_dir;
    Fix16_Point hit_point;
    Fix16_Point obj_speed;
    obj_speed = Object::GetSpeedVector_52AE90();
    if (field_4_sprite->GetNearestVerticalEdgeToCoordinate_5A1030(gCollisionTarget_679188.field_14_vseg_x, hit_point, edge_flag))
    {
        reflect_dir.SetXY_432860(field_4_sprite->field_14_xy.x - gCollisionTarget_679188.field_14_vseg_x, Fix16(0));
    }
    else
    {
        hit_point.SetXY_432860(gCollisionTarget_679188.field_14_vseg_x, gCollisionTarget_679188.field_18_hseg_y);
        hit_point -= *pPoint;
        hit_point.y += field_4_sprite->field_14_xy.y;
        hit_point.x = gCollisionTarget_679188.field_14_vseg_x;

        Fix16 abs1 = Fix16::Abs(hit_point.y - gCollisionTarget_679188.field_10_vseg_y_max);
        Fix16 abs2 = Fix16::Abs(hit_point.y - gCollisionTarget_679188.field_C_vseg_y_min);

        if (abs2 < abs1)
        {
            hit_point.y = gCollisionTarget_679188.field_C_vseg_y_min;
        }
        else
        {
            hit_point.y = gCollisionTarget_679188.field_10_vseg_y_max;
        }

        reflect_dir = -obj_speed;
    }
    Object::ResolveCollisionWithWorld_522B20(&hit_point, &reflect_dir, &obj_speed);
}

MATCH_FUNC(0x522e10)
void Object::HandleCollision_522E10(Fix16_Point* pPoint)
{

    Fix16_Point hit_point;
    switch (gCollisionTarget_679188.field_0_type)
    {
        case collision_target_type::horizontal_edge_1:
            ResolveCollisionWithHorizontalEdge_522BE0(pPoint);
            break;
        case collision_target_type::vertical_edge_2:
            ResolveCollisionWithVerticalEdge_522D00(pPoint);
            break;

        case collision_target_type::sprite_3:
        {
            u8 side_self;
            u8 side_other;
            u8 hit_type;
            hit_point = field_4_sprite->FindCollisionIntersectionPoint_5A2710(gCollisionTarget_679188.field_20_pHitSprite, *pPoint, field_4_sprite->field_0_rotation, side_self, side_other, hit_type);
            Car* pCar = gCollisionTarget_679188.field_20_pHitSprite->AsCar_40FEB0();
            if (pCar)
            {
                pCar->SetupCarPhysicsAndSpriteBinding_43BCA0();
                pCar->field_58_physics->HandleObjectCollisionAt_482CC0(this, hit_point, side_other);
            }
            else
            {
                Char* pChar = gCollisionTarget_679188.field_20_pHitSprite->AsCharB4_40FEA0();
                if (pChar)
                {
                    ResolveCollisionWithPed_5229B0(pChar, &hit_point, hit_type);
                    HandleImpact_528E50(gCollisionTarget_679188.field_20_pHitSprite);
                }
                else
                {
                    Object* pObj = gCollisionTarget_679188.field_20_pHitSprite->AsObject2C_40FEC0();
                    ResolveCollisionWithObject_522710(pObj, &hit_point);
                }
            }
            break;
        }
    }
}

MATCH_FUNC(0x522FA0)
void Object::Sprite_UpdateZFromSlopeAndTile_522FA0(Sprite* pSprite)
{
    Fix16 z_val = pSprite->field_1C_zpos;
    if (gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(pSprite->field_14_xy.x.ToInt(), pSprite->field_14_xy.y.ToInt(), (z_val.ToInt()) - 1))
    {
        z_val -= Fix16(1);
    }

    if (gMap_0x370_6F6268->UpdateZFromSlopeAtCoord_4E5BF0(pSprite->field_14_xy.x, pSprite->field_14_xy.y, z_val) == 0)
    {
        Fix16 z_frac = z_val.GetFracValue();
        if (z_frac != kFpZero_6F8E10)
        {
            z_val = z_val.GetFloorValue();
            if (z_frac > kFP16Half_6F8D10)
            {
                z_val += Fix16(1);
            }

            if (z_val > kMaxZ_6F8BFC)
            {
                z_val = kMaxZ_6F8BFC;
            }
        }
    }

    pSprite->set_z_lazy_420660(z_val);
}

MATCH_FUNC(0x5233a0)
bool Object::TryStartSlide_5233A0(Fix16 value)
{
    gObject2C_LastCollisionSprite_6F8F8C = NULL;

    if (SelectCollisionSprite_522460(this->field_4_sprite))
    {

        byte_623EC4 = 0;
        if (gObject2C_HitWallOrDoor_6F8F94)
        {
            return false;
        }

        gObject2C_LastCollisionSprite_6F8F8C = gCollisionTarget_679188.field_20_pHitSprite;
        if (value > kFpPoint1_6F8C58)
        {
            return false;
        }

        if (this->field_10_obj_3c->field_34 == 1)
        {
            SetTopDrawLayer_482BF0();
        }
        else
        {
            this->field_4_sprite->SetDrawLayer_40F7B0(6);
        }
        this->field_10_obj_3c->field_C_speed = kFpPoint1_6F8C58;
        this->field_10_obj_3c->field_18_friction = this->field_8_definition->field_14_friction;
        return true;
    }
    this->field_10_obj_3c->field_34 = 2;
    return false;
}

MATCH_FUNC(0x523440)
void Object::HandleCollisionOutcome_523440(Fix16_Point point, bool bHitFloorBelow, bool bHitBlockAtZ)
{
    if (static_cast<u8>(field_4_sprite->IsOnWater_59E1D0()))
    {
        this->field_10_obj_3c->field_C_speed = kFpZero_6F8E10;
        this->field_10_obj_3c->field_10_z_speed = kFpZero_6F8E10;
        return;
    }

    switch (this->field_8_definition->field_4C)
    {
        case 4:
            if (gCollisionTarget_679188.field_20_pHitSprite)
            {
                field_4_sprite->set_z_lazy_420660(gCollisionTarget_679188.field_20_pHitSprite->field_1C_zpos);
            }

            if (!bHitFloorBelow || gCollisionTarget_679188.field_0_type == collision_target_type::sprite_3)
            {
                if (bHitBlockAtZ && gCollisionTarget_679188.field_0_type != collision_target_type::sprite_3)
                {
                    gCollisionTarget_679188.SetType5_482A80();
                }
            }
            else
            {
                gCollisionTarget_679188.SetType4_482A70();
            }

            HandleImpact_528E50(gCollisionTarget_679188.field_20_pHitSprite);
            return;

        case 0:
        case 1:
            if (bHitFloorBelow)
            {
                gCollisionTarget_679188.SetType4_482A70();
            }
            else if (bHitBlockAtZ)
            {
                gCollisionTarget_679188.SetType5_482A80();
            }
            HandleImpact_528E50(gCollisionTarget_679188.field_20_pHitSprite);
            return;

        case 2:
        case 3:
            HandleCollision_522E10(&point);
            return;
        default:
            return;
    }
}

MATCH_FUNC(0x5235b0)
bool Object::HandleSpriteGroundAndCollision_5235B0(Sprite* pSprite, Fix16_Point* pPoint, bool* pHitWall, Fix16 found_z)
{
    Fix16 ground_z;
    bool bOnGround;
    field_10_obj_3c->field_2A_bAirborne = 0;
    if (gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(pSprite->field_14_xy.x.ToInt(), pSprite->field_14_xy.y.ToInt(), pSprite->field_1C_zpos.ToInt()))
    {
        ground_z = gMap_0x370_6F6268->GetGroundZBelowCoordEx_4E5050(pSprite->field_14_xy.x, pSprite->field_14_xy.y, pSprite->field_1C_zpos, bOnGround);

        if (!bOnGround)
        {
            if (ground_z < pSprite->field_1C_zpos)
            {
                Sprite_UpdateZFromSlopeAndTile_522FA0(pSprite);
                field_10_obj_3c->field_10_z_speed = kObjectGravity_6F8DA8;
            }
        }
        else
        {
            Sprite_UpdateZFromSlopeAndTile_522FA0(pSprite);
        }
        field_10_obj_3c->field_2F_bOnSlope = true;
    }
    else
    {
        if (gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(pSprite->field_14_xy.x.ToInt(),
                                                          pSprite->field_14_xy.y.ToInt(),
                                                          pSprite->field_1C_zpos.ToInt() - 1) ||
            field_10_obj_3c->field_2F_bOnSlope)
        {
            if (field_10_obj_3c->field_2F_bOnSlope)
            {
                Sprite_UpdateZFromSlopeAndTile_522FA0(pSprite);
            }
            field_10_obj_3c->field_10_z_speed = kFpZero_6F8E10;
            field_10_obj_3c->field_2A_bAirborne = 0;
        }
        else
        {
            field_10_obj_3c->field_10_z_speed = kObjectGravity_6F8DA8;
            field_10_obj_3c->field_2A_bAirborne = 1;
        }
        field_10_obj_3c->field_2F_bOnSlope = false;
    }

    if (pSprite->CheckSpriteMovementRegion_5A2500())
    {
        *pPoint = pSprite->get_x_y();
        *pHitWall = true;
        ComputeWallHitSide_524550();
        return true;
    }
    else if (SelectCollisionSprite_522460(pSprite))
    {
        field_10_obj_3c->field_14_saved_speed = field_10_obj_3c->field_C_speed.mValue;
        field_10_obj_3c->field_10_z_speed = kFpZero_6F8E10;
        field_10_obj_3c->field_2A_bAirborne = 0;
        *pPoint = pSprite->get_x_y();
        return true;
    }
    else
    {
        return false;
    }
}

// https://decomp.me/scratch/soZJL
MATCH_FUNC(0x523770)
bool Object::HandleSpriteGroundAndCollisionSimple_523770(Sprite* pSprite, Fix16_Point* pPoint, bool* pHitWall, bool* pHitBlockAtZ)
{
    field_10_obj_3c->field_2A_bAirborne = 1;

    if (pSprite->field_1C_zpos > kMaxZ_6F8BFC)
    {
        pSprite->field_1C_zpos = kMaxZ_6F8BFC;
        pSprite->InvalidateZColAndBBoxes_59E7B0();
        field_10_obj_3c->field_10_z_speed = kFpZero_6F8E10;
        field_10_obj_3c->field_1C_z_accel = kFpZero_6F8E10;
    }

    if (pSprite->CheckMapZCollision_5A21F0())
    {
        *pHitBlockAtZ = true;
        field_10_obj_3c->field_10_z_speed = kFpZero_6F8E10;
        field_10_obj_3c->field_1C_z_accel = kFpZero_6F8E10;
        if (field_8_definition->field_50_z_collision_response == 4)
        {
            return true;
        }
    }

    if (pSprite->CheckSpriteMovementRegion_5A2500())
    {
        field_10_obj_3c->field_14_saved_speed = field_10_obj_3c->field_C_speed;
        *pPoint = pSprite->get_x_y();
        *pHitWall = true;
        ComputeWallHitSide_524550();
        return true;
    }
    else if (SelectCollisionSprite_522460(pSprite))
    {
        if (gObject2C_HitWallOrDoor_6F8F94 != 0)
        {
            field_10_obj_3c->field_14_saved_speed = field_10_obj_3c->field_C_speed;
            *pPoint = pSprite->get_x_y();
            *pHitWall = true;
        }
        else
        {
            field_10_obj_3c->field_14_saved_speed = field_10_obj_3c->field_C_speed;
            *pPoint = pSprite->get_x_y();
        }
        return true;
    }
    else
    {
        return false;
    }
}

// https://decomp.me/scratch/1RN5H
MATCH_FUNC(0x5238B0)
bool Object::HandleSpriteZCollision_5238B0(Sprite* pSprite, Fix16_Point* pPoint, bool* pHitWall, bool* pHitFloorBelow, Fix16 found_z)
{
    Fix16 ground_z;
    bool bOverGap = false;
    // The landing code after the slope/non-slope split is reached through this flag, not a goto: VC6
    // threads the `if (bLanded)` test away on every path, which puts the `field_50_z_collision_response != 1` arms after the
    // non-slope `bAirborne = 1` block (and lets the two `bAirborne = 1` tails merge there).
    bool bLanded = false;

    if (gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(pSprite->field_14_xy.x.ToInt(), pSprite->field_14_xy.y.ToInt(), pSprite->field_1C_zpos.ToInt()))
    {
        if (gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(pSprite->field_14_xy.x.ToInt(),
                                                           pSprite->field_14_xy.y.ToInt(),
                                                           pSprite->field_1C_zpos.ToInt()) != AIR)
        {
            ground_z = gMap_0x370_6F6268->GetGroundZBelowCoordEx_4E5050(pSprite->field_14_xy.x, pSprite->field_14_xy.y, pSprite->field_1C_zpos, bOverGap);
            if (!bOverGap)
            {
                if (ground_z > pSprite->field_1C_zpos)
                {
                    pSprite->set_xyz_lazy_420600(pSprite->field_14_xy.x, pSprite->field_14_xy.y, ground_z);
                    Object::Sprite_UpdateZFromSlopeAndTile_522FA0(pSprite);
                    field_10_obj_3c->field_2A_bAirborne = 0;
                    bLanded = true;
                }
                else
                {
                    field_10_obj_3c->field_2A_bAirborne = 1;
                }
            }
            else
            {
                Object::Sprite_UpdateZFromSlopeAndTile_522FA0(pSprite);
                field_10_obj_3c->field_2A_bAirborne = 0;
                bLanded = true;
            }
        }
        else
        {
            field_10_obj_3c->field_2A_bAirborne = 1;
        }
    }
    else
    {
        if (found_z > pSprite->field_1C_zpos)
        {
            pSprite->set_xyz_lazy_420600(pSprite->field_14_xy.x, pSprite->field_14_xy.y, found_z);
            Object::Sprite_UpdateZFromSlopeAndTile_522FA0(pSprite);
            field_10_obj_3c->field_2A_bAirborne = 0;

            if (field_10_obj_3c->field_10_z_speed < kObjectGravity_6F8DA8)
            {
                if (!gObject2C_LastCollisionSprite_6F8F8C)
                {
                    if (field_10_obj_3c->field_10_z_speed >= -kFpPoint1_6F8C58)
                    {
                        *pHitFloorBelow = true;
                        field_10_obj_3c->field_10_z_speed = kFpZero_6F8E10;
                        field_10_obj_3c->field_C_speed = kFpZero_6F8E10;
                    }
                    else
                    {
                        bLanded = true;
                    }
                }
                else
                {
                    field_10_obj_3c->field_10_z_speed = kFpZero_6F8E10;
                    pSprite->set_xyz_lazy_420600(pSprite->field_14_xy.x, pSprite->field_14_xy.y, field_4_sprite->field_1C_zpos);
                    *pHitFloorBelow = true;
                }
            }
            else
            {
                if (field_10_obj_3c->field_C_speed == kFpZero_6F8E10 && field_10_obj_3c->field_10_z_speed == kObjectGravity_6F8DA8)
                {
                    field_10_obj_3c->field_10_z_speed = kFpZero_6F8E10;
                }
            }
        }
        else
        {
            field_10_obj_3c->field_2A_bAirborne = 1;
        }
    }

    if (bLanded)
    {
        if (field_8_definition->field_50_z_collision_response == 1)
        {
            *pHitFloorBelow = true;
            field_10_obj_3c->field_10_z_speed = kFpZero_6F8E10;
            field_10_obj_3c->field_C_speed = kFpZero_6F8E10;
        }
        else
        {
            if (field_8_definition->field_50_z_collision_response == 4)
            {
                *pHitFloorBelow = true;
                return true;
            }
            *pHitFloorBelow = true;
            field_10_obj_3c->field_10_z_speed = kObjectBounceRestitution_6F8DBC * (-field_10_obj_3c->field_10_z_speed);
        }
    }

    if (pSprite->CheckSpriteMovementRegion_5A2500())
    {
        field_10_obj_3c->field_14_saved_speed = field_10_obj_3c->field_C_speed;
        field_10_obj_3c->field_10_z_speed = kFpZero_6F8E10;
        field_10_obj_3c->field_2A_bAirborne = 0;
        *pPoint = pSprite->get_x_y();
        *pHitWall = true;
        Object::ComputeWallHitSide_524550();
        return true;
    }
    else if (Object::SelectCollisionSprite_522460(pSprite))
    {
        *pHitFloorBelow = true;
        if (field_8_definition->field_50_z_collision_response == 4)
        {
            return true;
        }
        else if (gObject2C_HitWallOrDoor_6F8F94)
        {
            field_10_obj_3c->field_14_saved_speed = field_10_obj_3c->field_C_speed.mValue;
            *pPoint = pSprite->get_x_y();
            *pHitWall = true;
            return true;
        }
        else
        {
            Object_3C* pObj3C = field_10_obj_3c;
            if (pObj3C->field_34 == 2)
            {
                pObj3C->field_34 = 1;
                ApplyDefinitionDrawLayer_482BE0();
                field_10_obj_3c->field_10_z_speed = kObjectBounceRestitution_6F8DBC * (-field_10_obj_3c->field_10_z_speed);
            }
            return true;
        }
    }
    else
    {
        return false;
    }
}

MATCH_FUNC(0x523BF0)
void Object::IntegrateMovementAndCollisions_523BF0(Fix16 mov_speed, Ang16 angle)
{
    // Declaration order matters: the scalars are initialised before the two points are
    // constructed (EH state 1 is set after the byte stores)
    Ang16 prev_rotation(0);
    Fix16 mov_speed_copy = mov_speed;
    bool bHitSomething = false;
    bool bHitBlockAtZ = false;
    bool bHitWall = false;
    bool bHandled = false;
    Fix16_Point point;
    Fix16_Point point2;
    Fix16 step_count;
    Sprite* pSprt = gObject_5C_6F8F84->field_58_collision_probe_sprite;
    gObject2C_WallHitSide_6F8F90 = 0;
    ApplyDefinitionDrawLayer_482BE0();
    if (Object::TryStartSlide_5233A0(mov_speed))
    {
        mov_speed_copy = field_10_obj_3c->field_C_speed;
    }
    if (mov_speed_copy != kFpZero_6F8E10 || field_10_obj_3c->field_2A_bAirborne || field_10_obj_3c->field_10_z_speed != kFpZero_6F8E10)
    {
        gCollisionTarget_679188.Reset_4637B0();
        pSprt->set_xyz_lazy_420600(field_4_sprite->field_14_xy.x, field_4_sprite->field_14_xy.y, field_4_sprite->field_1C_zpos);
        pSprt->set_ang_lazy_420690(field_4_sprite->field_0_rotation);
        pSprt->AllocCollisionShape_59F950(field_8_definition->field_0_width, field_8_definition->field_4_height, field_8_definition->field_8_depth);
        pSprt->SetType_4206F0(field_4_sprite->GetType_416B40());
        pSprt->SetObject2C_482A30(field_4_sprite->field_8_object_2C_ptr);
        if (gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(pSprt->field_14_xy.x.ToInt(), pSprt->field_14_xy.y.ToInt(), pSprt->field_1C_zpos.ToInt()))
        {
            field_10_obj_3c->field_2F_bOnSlope = true;
        }
        else
        {
            field_10_obj_3c->field_2F_bOnSlope = false;
        }
        Fix16 radius;
        Fix16 step_dz;
        if (mov_speed_copy != kFpZero_6F8E10)
        {
            step_count = mov_speed_copy / field_8_definition->field_C_min_size;
            radius = mov_speed_copy / step_count;
            step_dz = field_10_obj_3c->field_10_z_speed / step_count;
        }
        else
        {
            step_count = kFpZero_6F8E10;
            radius = kFpZero_6F8E10;
        }

        if (step_count < kFP16One_6F8E14)
        {
            step_count = kFP16One_6F8E14;
            radius = mov_speed_copy;
            step_dz = field_10_obj_3c->field_10_z_speed;
        }

        Fix16 step_dx;
        Fix16 step_dy;
        Fix16 prev_x;
        Fix16 prev_y;
        Fix16 prev_z;
        {
            // The original passes a block-scoped copy of radius (9.6f copies it too); its slot is
            // shared with tmp_radius below
            Fix16 r = radius;
            Ang16::PolarToCartesian_41FC20(angle, r, step_dx, step_dy);
        }

        for (u8 i = 1; i <= step_count.ToInt(); i++)
        {
            prev_x = pSprt->field_14_xy.x;
            prev_y = pSprt->field_14_xy.y;
            prev_z = pSprt->field_1C_zpos;
            prev_rotation = pSprt->field_0_rotation;
            Fix16 found_z;
            found_z = gMap_0x370_6F6268->FindGroundZBelowCoord_4E4D40(pSprt->field_14_xy.x, pSprt->field_14_xy.y, pSprt->field_1C_zpos);
            pSprt->set_xyz_lazy_420600(pSprt->field_14_xy.x + step_dx, pSprt->field_14_xy.y + step_dy, pSprt->field_1C_zpos + step_dz);
            pSprt->set_ang_lazy_420690(angle);
            if (step_dz == kFpZero_6F8E10)
            {
                bHandled = Object::HandleSpriteGroundAndCollision_5235B0(pSprt, &point, &bHitWall, found_z);
            }
            else if (step_dz > kFpZero_6F8E10)
            {
                bHandled = Object::HandleSpriteGroundAndCollisionSimple_523770(pSprt, &point, &bHitWall, &bHitBlockAtZ);
            }
            else
            {
                bHandled = Object::HandleSpriteZCollision_5238B0(pSprt, &point, &bHitWall, &bHitSomething, found_z);
            }
        }

        if (bHitSomething)
        {
            gCollisionSoundQueue_66AFE0->AddFloorCollision_40BC40(field_4_sprite);
        }
        if (!bHandled)
        {
            field_4_sprite->set_xyz_lazy_420600(pSprt->field_14_xy.x, pSprt->field_14_xy.y, pSprt->field_1C_zpos);
            field_4_sprite->set_ang_lazy_420690(pSprt->field_0_rotation);
            if (bHitSomething)
            {
                if (static_cast<u8>(field_4_sprite->IsOnWater_59E1D0()))
                {
                    field_10_obj_3c->field_C_speed = kFpZero_6F8E10;
                    field_10_obj_3c->field_10_z_speed = kFpZero_6F8E10;
                }
                gCollisionSoundQueue_66AFE0->AddFloorCollision_40BC40(field_4_sprite);
            }
            else if (bHitBlockAtZ)
            {
                gCollisionSoundQueue_66AFE0->AddBlockCollision_40BD10(field_4_sprite);
            }
        }
        else
        {
            gObj2C_LastValidX_6F8F00 = prev_x;
            gObj2C_LastValidY_6F8EF8 = prev_y;
            gObj2C_LastValidZ_6F8EFC = prev_z;
            gObj2C_LastValidAng_6F8D1C = prev_rotation;
            Fix16 tmp_radius = radius;
            pSprt->set_xyz_lazy_420600(gObj2C_LastValidX_6F8F00, gObj2C_LastValidY_6F8EF8, gObj2C_LastValidZ_6F8EFC);
            pSprt->set_ang_lazy_420690(prev_rotation);
            Object::Sprite_UpdateZFromSlopeAndTile_522FA0(pSprt);
            bool bLastStepClear;

            for (s32 j = 0; j < 3; j++)
            {
                tmp_radius = tmp_radius / 2;
                Ang16::PolarToCartesian_41FC20(angle, tmp_radius, step_dx, step_dy);
                pSprt->set_xyz_lazy_420600(pSprt->field_14_xy.x + step_dx, pSprt->field_14_xy.y + step_dy, pSprt->field_1C_zpos);
                pSprt->set_ang_lazy_420690(angle);
                Object::Sprite_UpdateZFromSlopeAndTile_522FA0(pSprt);
                bool bHitSprite = false;
                if (field_10_obj_3c->field_34 == 2 && Object::SelectCollisionSprite_522460(pSprt))
                {
                    bHitSprite = true;
                }
                if (pSprt->CheckSpriteMovementRegion_5A2500() || bHitSprite)
                {
                    point = pSprt->get_x_y_443580();
                    pSprt->set_xyz_lazy_420600(gObj2C_LastValidX_6F8F00, gObj2C_LastValidY_6F8EF8, gObj2C_LastValidZ_6F8EFC);
                    pSprt->set_ang_lazy_420690(gObj2C_LastValidAng_6F8D1C);
                    bLastStepClear = false;
                }
                else
                {
                    bLastStepClear = true;
                    gObj2C_LastValidX_6F8F00 = pSprt->field_14_xy.x;
                    gObj2C_LastValidY_6F8EF8 = pSprt->field_14_xy.y;
                    gObj2C_LastValidZ_6F8EFC = pSprt->field_1C_zpos;
                    gObj2C_LastValidAng_6F8D1C = pSprt->field_0_rotation;
                }
            }
            if (!bLastStepClear)
            {
                pSprt->set_xyz_lazy_420600(gObj2C_LastValidX_6F8F00, gObj2C_LastValidY_6F8EF8, gObj2C_LastValidZ_6F8EFC);
                pSprt->set_ang_lazy_420690(gObj2C_LastValidAng_6F8D1C);
            }
            field_4_sprite->set_xyz_lazy_420600(pSprt->field_14_xy.x, pSprt->field_14_xy.y, pSprt->field_1C_zpos);
            field_4_sprite->set_ang_lazy_420690(pSprt->field_0_rotation);
            if (bHitSomething || bHitWall || bHitBlockAtZ)
            {
                Object::HandleCollisionOutcome_523440(point, bHitSomething, bHitBlockAtZ);
            }
        }

        Object::TryStartSlide_5233A0(mov_speed);
        if (field_4_sprite->field_14_xy.x < kFpZero_6F8E10 || field_4_sprite->field_14_xy.x >= kMapSize_6F8BF4 || field_4_sprite->field_14_xy.y < kFpZero_6F8E10 ||
            field_4_sprite->field_14_xy.y >= kMapSize_6F8BF4)
        {
            Object::RequestRemoval_5290A0();
        }
    }
}

MATCH_FUNC(0x524550)
void Object::ComputeWallHitSide_524550()
{
    if (gCollisionTarget_679188.field_0_type == collision_target_type::horizontal_edge_1)
    {
        if (field_10_obj_3c->field_4_angle >= kAng270_6F8D88 || field_10_obj_3c->field_4_angle <= kAng90_6F8C88)
        {
            gObject2C_WallHitSide_6F8F90 = 3;
        }
        else
        {
            gObject2C_WallHitSide_6F8F90 = 4;
        }
    }
    else if (gCollisionTarget_679188.field_0_type == collision_target_type::vertical_edge_2)
    {
        if (field_10_obj_3c->field_4_angle >= kAng180_6F8D62)
        {
            gObject2C_WallHitSide_6F8F90 = 2;
        }
        else
        {
            gObject2C_WallHitSide_6F8F90 = 1;
        }
    }
}

MATCH_FUNC(0x524630)
void Object::IntegrateHorizontalMovementAndCollisions_524630(Fix16 move_len, Ang16 move_angle)
{
    bool bOnSlope = false;
    Fix16_Point hitPoint;
    Sprite* pProbe = gObject_5C_6F8F84->field_58_collision_probe_sprite;
    gObject2C_LastCollisionSprite_6F8F8C = NULL;
    if (move_len == kFpZero_6F8E10 && !this->field_10_obj_3c->field_2A_bAirborne)
    {
        return;
    }
    gCollisionTarget_679188.Reset_4637B0();

    pProbe->set_xyz_lazy_420600(field_4_sprite->field_14_xy.x, field_4_sprite->field_14_xy.y, field_4_sprite->field_1C_zpos);
    pProbe->set_ang_lazy_420690(field_4_sprite->field_0_rotation);
    pProbe->AllocCollisionShape_59F950(this->field_8_definition->field_0_width, this->field_8_definition->field_4_height, this->field_8_definition->field_8_depth);
    pProbe->SetType_4206F0(this->field_4_sprite->GetType_416B40());
    pProbe->SetObject2C_482A30(this->field_4_sprite->field_8_object_2C_ptr);

    Fix16 step_count;
    Fix16 step_len;
    if (move_len != kFpZero_6F8E10)
    {
        step_count = (move_len) / this->field_8_definition->field_C_min_size;
        step_len = (move_len) / step_count;
    }
    else
    {
        step_count = kFpZero_6F8E10;
        step_len = kFpZero_6F8E10;
    }

    if (step_count < kFP16One_6F8E14)
    {
        step_count = kFP16One_6F8E14;
        step_len = move_len;
    }

    Fix16 step_dx;
    Fix16 step_dy;
    {
        // 9.6f: PolarToCartesian_41FC20 on a copy of the step
        Fix16 radius = step_len;
        step_dx = Ang16::sine_40F500(move_angle) * radius;
        step_dy = Ang16::cosine_40F520(move_angle).Multiply_408680(radius);
    } // 9.6f inlined: PolarToCartesian_41FC20

    Ang16 start_rotation;
    Fix16 start_y;
    Fix16 start_x;

    s32 num_steps = step_count.ToInt();
    // The while (1) never iterates (the body returns), but its dead exit edge makes VC6 visit the LABEL_48
    // code first in its reverse-postorder layout, which puts the no-hit tail between the hit blocks and the
    // half-step loop; a rotated for/while test gives the `jle top; jmp tail` latch. Both are needed to match.
    while (1)
    {
        for (u8 step_idx = 1; step_idx <= num_steps; step_idx++)
        {
            start_x = pProbe->field_14_xy.x;
            start_y = pProbe->field_14_xy.y;
            start_rotation = pProbe->field_0_rotation;
            field_10_obj_3c->field_2A_bAirborne = 0;

            if (gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(pProbe->field_14_xy.x.ToInt(), pProbe->field_14_xy.y.ToInt(), pProbe->field_1C_zpos.ToInt()))
            {
                bOnSlope = true;
            }

            pProbe->set_xy_lazy_447E20(pProbe->field_14_xy.x + step_dx, pProbe->field_14_xy.y + step_dy);
            pProbe->set_ang_lazy_420690(move_angle);

            if (gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(pProbe->field_14_xy.x.ToInt(), pProbe->field_14_xy.y.ToInt(), pProbe->field_1C_zpos.ToInt()))
            {
                Sprite_UpdateZFromSlopeAndTile_522FA0(pProbe);
            }

            if (!gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(pProbe->field_14_xy.x.ToInt(),
                                                               pProbe->field_14_xy.y.ToInt(),
                                                               (pProbe->field_1C_zpos - kFP16One_6F8E14).ToInt()))
            {
                if (gMap_0x370_6F6268->IsGradientSlopeAt_466CF0(pProbe->field_14_xy.x.ToInt(),
                                                                pProbe->field_14_xy.y.ToInt(),
                                                                (pProbe->field_1C_zpos - kFP16One_6F8E14).ToInt()))
                {
                    Sprite_UpdateZFromSlopeAndTile_522FA0(pProbe);
                }
            }

            if (pProbe->CheckSpriteMovementRegion_5A2500())
            {
                if (bOnSlope == 1)
                {
                    pProbe->field_1C_zpos = pProbe->field_1C_zpos.GetFloorValue();
                    if (pProbe->field_1C_zpos > kFP16Half_6F8D10)
                    {
                        pProbe->field_1C_zpos++;
                    }

                    if (pProbe->CheckSpriteMovementRegion_5A2500() || SelectCollisionSprite_522460(pProbe))
                    {
                        hitPoint = pProbe->get_x_y();
                        ComputeWallHitSide_524550();
                        goto LABEL_48;
                    }
                }
                else
                {
                    hitPoint = pProbe->get_x_y();
                    ComputeWallHitSide_524550();
                    goto LABEL_48;
                }
            }
            else if (SelectCollisionSprite_522460(pProbe))
            {
                hitPoint = pProbe->get_x_y();
                goto LABEL_48;
            }
        }
        field_4_sprite->set_xyz_lazy_420600(pProbe->field_14_xy.x, pProbe->field_14_xy.y, pProbe->field_1C_zpos);
        field_4_sprite->set_ang_lazy_420690(pProbe->field_0_rotation);
        return;
    }

LABEL_48:
    Fix16 step = step_len;

    pProbe->set_xy_lazy_447E20(start_x, start_y);
    pProbe->set_ang_lazy_420690(start_rotation);

    Sprite_UpdateZFromSlopeAndTile_522FA0(pProbe);
    s32 tries = 3;
    do
    {
        Fix16 old_x = pProbe->field_14_xy.x;
        Fix16 old_y = pProbe->field_14_xy.y;
        start_rotation = pProbe->field_0_rotation;
        step = step / 2;
        step_dx = Ang16::sine_40F500(move_angle) * step;
        step_dy = Ang16::cosine_40F520(move_angle) * step;
        pProbe->set_xy_lazy_447E20(pProbe->field_14_xy.x + step_dx, pProbe->field_14_xy.y + step_dy);
        pProbe->set_ang_lazy_420690(move_angle);

        if (pProbe->CheckSpriteMovementRegion_5A2500() || SelectCollisionSprite_522460(pProbe))
        {
            hitPoint = pProbe->get_x_y();
            pProbe->set_xy_lazy_447E20(old_x, old_y);
            pProbe->set_ang_lazy_420690(start_rotation);
            Sprite_UpdateZFromSlopeAndTile_522FA0(pProbe);
        }
    } while (--tries);

    field_4_sprite->set_xyz_lazy_420600(pProbe->field_14_xy.x, pProbe->field_14_xy.y, pProbe->field_1C_zpos);
    field_4_sprite->set_ang_lazy_420690(pProbe->field_0_rotation);

    switch (this->field_8_definition->field_4C)
    {
        case 0:
        case 1:
            HandleImpact_528E50(gCollisionTarget_679188.field_20_pHitSprite);
            break;
        case 2:
        case 3:
            HandleCollision_522E10(&hitPoint);
            break;
        default:
            return;
    }
}

MATCH_FUNC(0x525100)
void Object::OnAttachedSpriteDestroyed_525100()
{
    const s32 behavior = field_8_definition->field_34_behavior_type;
    if (behavior >= object_behavior_type::static_object_0 && behavior <= object_behavior_type::static_collision_check_1)
    {
        if (field_18_model != objects::tank_cannon_148)
        {
            RequestRemoval_5290A0();
        }
        else
        {
            SetSpriteIdxOffset_5290C0(1u);
        }
    }
}

// https://decomp.me/scratch/jLuSq
MATCH_FUNC(0x525190)
void Object::OnExplosionNearby_525190(u8 ped_ref_idx)
{

    if (IsExplosiveItemDefinition_482400(field_8_definition->field_3C_next_definition_idx))
    {
        PoolGiveAndQueueObjectType_5291E0(field_8_definition->field_3C_next_definition_idx);
    }
    else if (field_8_definition->field_48_projectile_hit_type == 13)
    {
        PoolGiveAndMarkDone_5291D0();
        Object* pExplosion = gObject_5C_6F8F84->CreateExplosion_52A3D0(this->field_4_sprite->field_14_xy.x,
                                                                          this->field_4_sprite->field_14_xy.y,
                                                                          this->field_4_sprite->field_1C_zpos,
                                                                          kZeroAng_6F8F68,
                                                                          explosion_type::item_19,
                                                                          gPedRefTable_7F8_703398->GetPedId_420F10(ped_ref_idx));
        if (pExplosion)
        {
            pExplosion->SetDamageOwner_529080(ped_ref_idx);
        }
    }
}

// Turns traffic lights into collision objects so AI stops at red lights
MATCH_FUNC(0x525290)
bool Object::ShouldStopAtTrafficLight_525290(Sprite* pSprite) const
{
    Car* pCar = pSprite->AsCar_40FEB0();
    if (pCar)
    {
        if (byte_6771DC)
        {
            if ((field_4_sprite->field_0_rotation != kZeroAng_6F8F68 || gTrafficLights_194_705958->field_192_phase != traffic_light_phase::north_south_green_1) &&
                (field_4_sprite->field_0_rotation != kAng90_6F8C88 || gTrafficLights_194_705958->field_192_phase != traffic_light_phase::east_west_green_4))
            {
                return true;
            }
        }
    }
    return false;
}

MATCH_FUNC(0x525370)
char_type Object::ShouldCollideWithSprite_525370(Sprite* pSprite) const
{
    switch (this->field_18_model)
    {
        case objects::pedestrian_crossing_marker_122:
            if (pSprite)
            {
                return ShouldStopAtTrafficLight_525290(pSprite);
            }
            break;

        case objects::conveyor_139:
            break;

        case objects::door_collision_obj_169:
            if (pSprite)
            {
                return gDoor_4D4_67BD2C->CheckDoorAccess_49D3C0(pSprite, this->get_field_26_420FF0());
            }
            else
            {
                return byte_6F8EDC;
            }

        case objects::tunnel_blocker_294:
            if (pSprite)
            {
                Car* pCar = pSprite->AsCar_40FEB0();
                if (pCar)
                {
                    return !pCar->IsTrainModel_403BA0();
                }
            }
            // Fall through
        default:
            if (gCollisionCounters_6791FC->field_8_bDoorAccessCheck == 1)
            {
                if (CanBePushed_482C90())
                {
                    return 0;
                }
            }

            if (field_10_obj_3c)
            {
                if (field_10_obj_3c->field_34 != 2)
                {
                    return 0;
                }

                if (pSprite)
                {
                    if (pSprite->GetType_416B40() != sprite_types_enum::ped_3)
                    {
                        if (field_8_definition->field_4C == 3)
                        {
                            return 0;
                        }

                        if (field_8_definition->field_4C == 0)
                        {
                            return 0;
                        }
                    }
                }
                else
                {
                    if (field_8_definition->field_4C == 3)
                    {
                        return 0;
                    }
                    if (field_8_definition->field_4C == 0)
                    {
                        return 0;
                    }
                }
            }

            if (pSprite)
            {
                Object* pObject = pSprite->AsObject2C_40FEC0();
                if (pObject)
                {
                    if (pObject->field_8_definition->field_44_touch_behavior == 7 || pObject->field_8_definition->field_44_touch_behavior == 8)
                    {
                        switch (field_8_definition->field_48_projectile_hit_type)
                        {
                            case 0:
                                return 0;
                            case 12:
                                return 1;
                            case 13:
                                return 1;
                        }
                    }

                    if (pObject->field_18_model == objects::conveyor_139)
                    {
                        break;
                    }

                    // Only pickups collide with pickups
                    if (pObject->IsPickup_421060() && !IsPickup_421060())
                    {
                        return 0;
                    }
                }
            }
            return 1;
    }
    return 0;
}

MATCH_FUNC(0x5257d0)
void Object::UpdateAnimation_5257D0()
{
    ++this->field_C_pAny.o8->field_6_frame_counter;
    if (field_C_pAny.o8->field_6_frame_counter >= this->field_8_definition->field_64_next_frame_max)
    {
        field_C_pAny.o8->field_6_frame_counter = 0;
        ++this->field_C_pAny.o8->field_7_frame_idx;
        if (field_C_pAny.o8->field_7_frame_idx >= this->field_8_definition->field_6C_num_frames)
        {
            field_C_pAny.o8->field_7_frame_idx = 0;
            if (field_C_pAny.o8->field_4_timer > 0)
            {
                field_C_pAny.o8->field_4_timer--;
            }
        }
        const s16 target_id = this->field_8_definition->field_1E_base_sprite_idx + this->field_C_pAny.o8->field_7_frame_idx;

        field_4_sprite->set_id_lazy_4206C0(target_id);

        if (IsAnimFinished_482C10())
        {
            Object::TickObject_5283C0(this->field_8_definition->field_3C_next_definition_idx);
        }
    }
}

MATCH_FUNC(0x525910)
bool Object::DispatchFrameAction_525910()
{
    if (field_24_touched_result)
    {
        switch (field_8_definition->field_44_touch_behavior)
        {
            case 1:
            case 2:
            case 5:
            case 6:
            case 8:
            case 11:
                if (field_24_touched_result == 1)
                {
                    TickObject_5283C0(field_8_definition->field_38);
                }
                else
                {
                    TickObject_5283C0(field_24_touched_result);
                }
                field_24_touched_result = 0;
                return true;
            case 4:
                if (field_24_touched_result != 1)
                {
                    TickObject_5283C0(field_24_touched_result);
                }
                field_24_touched_result = 0;
                return true;
            case 7:
            case 10:
                field_24_touched_result = 0;
                RequestRemoval_5290A0();
                return true;
            default:
                field_24_touched_result = 0;
                return false;
        }
    }
    else if (field_8_definition->field_44_touch_behavior == 3 || field_8_definition->field_44_touch_behavior == 4)
    {
        TickObject_5283C0(field_8_definition->field_38);
        return true;
    }

    return false;
}

MATCH_FUNC(0x525AC0)
bool Object::IsFireExplosion_525AC0() const
{
    if (field_18_model == objects::explosion_113)
    {
        return field_C_pAny.pExplosion->IsFireType_5435D0();
    }
    else
    {
        return false;
    }
}

MATCH_FUNC(0x525AE0)
EXPORT void Object::CheckCollisionForModel_139_And_141_525AE0()
{
    switch (field_18_model)
    {
        case objects::conveyor_139:
        case objects::destructor_141:
            gSpriteGrid_1_679208->CheckAndHandleCollisionInStrips_477BD0(field_4_sprite);
            gSpriteGrid_2_67920C->CheckAndHandleCollisionInStrips_477BD0(field_4_sprite);
            break;
        default:
            break;
    }
}

MATCH_FUNC(0x525B20)
void Object::UpdateEffectPool_525B20()
{
    if (field_10_obj_3c)
    {
        if (field_10_obj_3c->field_0_attached_sprites.field_0_p18)
        {
            field_10_obj_3c->field_0_attached_sprites.PoolUpdate_5A6F70(field_4_sprite);
        }
    }
}

MATCH_FUNC(0x525b40)
void Object::SpawnSpriteParticlesForRocketBullet_525B40()
{
    if (field_18_model == objects::rocket_bullet_128)
    {
        gParticle_8_6FD5E8->SpawnParticleSprite_5405D0(field_4_sprite);
    }
}

MATCH_FUNC(0x525B60)
bool Object::CheckWaterDeath_525B60()
{
    // TODO: Forced eax -> al
    const u8 isWater = field_4_sprite->IsOnWater_59E1D0();
    if (isWater)
    {
        HandleWaterDeath_528900();
        return true;
    }
    return false;
}

MATCH_FUNC(0x525b80)
void Object::UpdatePhysicsAndMovement_525B80()
{
    Ang16 ang;

    UpdatePhysics_5222D0();
    Fix16 mov_speed;
    field_10_obj_3c->GetMovementSpeedAndAngle_521FD0(mov_speed, ang);

    if (!field_8_definition->field_58_uses_3d_movement)
    {
        IntegrateHorizontalMovementAndCollisions_524630(mov_speed, ang);
    }
    else
    {
        IntegrateMovementAndCollisions_523BF0(mov_speed, ang);
    }
    SpawnSpriteParticlesForRocketBullet_525B40();

    field_10_obj_3c->field_C_speed = field_10_obj_3c->field_C_speed.ZeroIfNegligible_482730();

    if (!DispatchFrameAction_525910())
    {
        if (field_10_obj_3c->field_0_attached_sprites.field_0_p18)
        {
            field_10_obj_3c->field_0_attached_sprites.PoolUpdate_5A6F70(field_4_sprite);
        }

        if (mov_speed != kFpZero_6F8E10 || (field_10_obj_3c->field_2A_bAirborne) || field_10_obj_3c->field_34 == 2 || CheckWaterDeath_525B60())
        {
            if (field_10_obj_3c->field_28_next_definition_timer > 0)
            {
                field_10_obj_3c->field_28_next_definition_timer--;
                if (field_10_obj_3c->field_28_next_definition_timer == 0)
                {
                    if (get_field_26_420FF0() > 0)
                    {
                        s32 ped_id = gPedRefTable_7F8_703398->GetPedId_420F10(get_field_26_420FF0());
                        if (ped_id)
                        {
                            Ped* pPed = gPedManager_6787BC->PedById(ped_id);
                            if (pPed)
                            {
                                pPed->HandleWeaponFireEnd_46FFF0(field_18_model);
                            }
                        }
                    }
                    TickObject_5283C0(field_8_definition->field_3C_next_definition_idx);
                }
            }
        }
        else
        {
            this->field_10_obj_3c->field_1C_z_accel = kFpZero_6F8E10;
            this->field_10_obj_3c->field_10_z_speed = kFpZero_6F8E10;

            TickObject_5283C0(field_8_definition->field_38);
            if (UsesParamAsPedRef_421080())
            {
                if (get_field_26_420FF0() > 0)
                {
                    gPedRefTable_7F8_703398->DecrementRefCount_59B0D0(field_26_ped_ref_idx);
                    this->field_26_ped_ref_idx = 0;
                }
            }
        }
    }
}

MATCH_FUNC(0x525d90)
void Object::UpdatePhysicsMovementAndAnimation_525D90()
{
    Ang16 ang;
    Sprite* pSprite = this->field_4_sprite;
    Fix16 x_val = pSprite->field_14_xy.x;
    Fix16 y_val = pSprite->field_14_xy.y;
    Fix16 zpos = pSprite->field_1C_zpos;

    UpdatePhysics_5222D0();

    Fix16 mov_speed;
    field_10_obj_3c->GetMovementSpeedAndAngle_521FD0(mov_speed, ang);

    if (!field_8_definition->field_58_uses_3d_movement)
    {
        IntegrateHorizontalMovementAndCollisions_524630(mov_speed, ang);
    }
    else
    {
        IntegrateMovementAndCollisions_523BF0(mov_speed, ang);
    }

    field_10_obj_3c->field_C_speed = field_10_obj_3c->field_C_speed.ZeroIfNegligible_482730();

    ObjectDefinition* pDefinition = this->field_8_definition;
    if (pDefinition->field_65_next_definition_timer != -1 || pDefinition->field_34_behavior_type == object_behavior_type::pickup_physics_animated_9 || (field_4_sprite->field_14_xy.x != x_val) || field_4_sprite->field_14_xy.y != y_val ||
        field_4_sprite->field_1C_zpos != zpos)
    {
        if (!this->field_10_obj_3c->field_30_bSkipAnim)
        {
            UpdateAnimation_5257D0();
        }
    }

    if (!DispatchFrameAction_525910())
    {
        if (field_10_obj_3c->field_0_attached_sprites.field_0_p18)
        {
            field_10_obj_3c->field_0_attached_sprites.PoolUpdate_5A6F70(field_4_sprite);
            if (!field_10_obj_3c->field_0_attached_sprites.field_0_p18 && field_18_model == objects::explosion_fragments_127)
            {
                RequestRemoval_5290A0();
            }
        }

        if (mov_speed == kFpZero_6F8E10)
        {
            if (!field_10_obj_3c->field_2A_bAirborne && field_10_obj_3c->field_34 == 2 && !CheckWaterDeath_525B60() && !gObject2C_LastCollisionSprite_6F8F8C)
            {
                field_10_obj_3c->field_1C_z_accel = kFpZero_6F8E10;
                field_10_obj_3c->field_10_z_speed = kFpZero_6F8E10;

                if (field_18_model != objects::grenade_obj_183)
                {
                    if (UsesParamAsPedRef_421080())
                    {
                        // 9.6f: Object::get_field_26_420FF0 (inlined, using it changes the code)
                        if (field_26_ped_ref_idx > 0)
                        {
                            gPedRefTable_7F8_703398->DecrementRefCount_59B0D0(field_26_ped_ref_idx);
                            this->field_26_ped_ref_idx = 0;
                        }
                    }
                }
                TickObject_5283C0(field_8_definition->field_38);
            }
        }
    }
}

MATCH_FUNC(0x525f30)
void Object::Update_525F30()
{
    Explosion* pExplosion;

    gObject2C_LastCollisionSprite_6F8F8C = NULL;
    while (2)
    {
        switch (this->field_8_definition->field_34_behavior_type)
        {
            case object_behavior_type::static_object_0:
                if (!DispatchFrameAction_525910())
                {
                    UpdateEffectPool_525B20();
                    return;
                }
                return;
            case object_behavior_type::static_collision_check_1:
                if (!DispatchFrameAction_525910())
                {
                    UpdateEffectPool_525B20();
                    CheckCollisionForModel_139_And_141_525AE0();
                }
                return;

            case object_behavior_type::explosion_5:
                pExplosion = this->field_C_pAny.pExplosion;
                if (pExplosion)
                {
                    if (pExplosion->Update_5434A0(kFpZero_6F8E10, kZeroAng_6F8F68))
                    {
                        this->field_25_removal_state = object_removal_state::requested_1;
                    }
                }
                return;

            case object_behavior_type::physics_3:
            case object_behavior_type::pickup_physics_7:
                RemoveFromCollisionBuckets_527D00();
                byte_6F8C4C = 1;
                UpdatePhysicsAndMovement_525B80();
                if (!byte_6F8F40)
                {
                    AssignToBucket_527AE0();
                }
                return;

            case object_behavior_type::animated_2:
            case object_behavior_type::pickup_animated_8:
                if (!DispatchFrameAction_525910())
                {
                    UpdateEffectPool_525B20();
                    UpdateAnimation_5257D0();
                }
                return;

            case object_behavior_type::physics_animated_4:
            case object_behavior_type::pickup_physics_animated_9:
                RemoveFromCollisionBuckets_527D00();
                byte_6F8C4C = 1;
                UpdatePhysicsMovementAndAnimation_525D90();
                if (!byte_6F8F40)
                {
                    AssignToBucket_527AE0();
                }
                return;

            case object_behavior_type::pickup_6:
            case object_behavior_type::shop_10:
            case object_behavior_type::light_11:
                DispatchFrameAction_525910();
                UpdateEffectPool_525B20();
                return;

            case object_behavior_type::effect_only_12:
                UpdateEffectPool_525B20();
                return;

            default:
                continue;
        }
    }
}

MATCH_FUNC(0x5263d0)
bool Object::PoolUpdate()
{
    byte_6F8C4C = 0;
    byte_6F8F40 = 0;
    if (field_25_removal_state != object_removal_state::requested_1)
    {
        Object::Update_525F30();
    }

    if (field_25_removal_state <= object_removal_state::none_0 || (field_25_removal_state == object_removal_state::when_off_screen_2 && gGame_0x40_67E008->IsSpriteInAnyPlayerBounds_4B97E0(field_4_sprite, kFpZero_6F8E10)))
    {
        return false;
    }
    else
    {
        Object::RemoveFromCollisionBuckets_527D00();
        return true;
    }
}

// 9.6f 0x4837F0
MATCH_FUNC(0x526790)
void Object::TriggerCarExplosionIfApplicable_526790(Sprite* pSprite)
{
    ObjectDefinition* pDefinition = this->field_8_definition;
    if (IsAnimatedBehavior_475A80())
    {
        if (IsAnimFinished_482C10())
        {
            if (IsExplosiveItemDefinition_482400(pDefinition->field_3C_next_definition_idx)) // car
            {
                Car* pCar = pSprite->AsCar_40FEB0();
                if (pCar)
                {
                    if (this->field_18_model == objects::car_bomb_132)
                    {

                        s32 id = gPedRefTable_7F8_703398->GetPedId_420F10(get_field_26_420FF0());
                        if (id)
                        {
                            pCar->field_70_exploder_ped_id = id;
                            pCar->field_90_death_cause = ped_death_cause::bomb_12;
                            pCar->field_94_exploder_timer = 50;
                        }
                    }
                    s32 t = GetExplosionTypeForItemDefinition_526830(field_8_definition->field_3C_next_definition_idx);
                    pCar->HandleCarExplosion_43D840(t);
                }
            }
        }
    }
}

MATCH_FUNC(0x526830)
s32 __stdcall Object::GetExplosionTypeForItemDefinition_526830(s32 type)
{
    switch (type)
    {
        case 39:
            return 18;
        case 40:
            return 33;
        case 41:
            return 19;
        case 42:
            return 20;
    }
    // The original reloads type from its stack slot here instead of keeping it in a register
    return *static_cast<volatile s32*>(&type);
}

MATCH_FUNC(0x526b40)
void Object::InheritMotionFromSprite_526B40(Sprite* pSprite)
{
    Object_3C* pObject3C;

    switch (pSprite->GetType_416B40())
    {
        case sprite_types_enum::ped_3:
            this->field_10_obj_3c->field_C_speed = (pSprite->field_8_char_b4_ptr->get_velocity_41B080() * kFpOne64th_6F8C9C);
            this->field_10_obj_3c->field_4_angle = pSprite->field_0_rotation;
            this->field_4_sprite->SetDrawLayer_40F7B0(27);
            break;

        case sprite_types_enum::car_2:
            this->field_10_obj_3c->field_C_speed = pSprite->field_8_car_bc_ptr->GetCarLinearSpeed_43A240();
            this->field_10_obj_3c->field_4_angle = pSprite->field_8_car_bc_ptr->GetOrientationAngle_43A3E0();
            this->field_4_sprite->SetDrawLayer_40F7B0(pSprite->AsCar_40FEB0()->GetAttachedDrawLayer_4435B0());
            break;

        case sprite_types_enum::code_obj1_4:
        case sprite_types_enum::map_obj_5:
            pObject3C = pSprite->field_8_object_2C_ptr->field_10_obj_3c;
            if (pObject3C)
            {
                this->field_10_obj_3c->field_C_speed = pObject3C->field_C_speed;
                this->field_10_obj_3c->field_4_angle = pSprite->field_8_object_2C_ptr->field_10_obj_3c->field_4_angle;
                this->field_10_obj_3c->field_10_z_speed = pSprite->field_8_object_2C_ptr->field_10_obj_3c->field_10_z_speed;
            }
            this->field_4_sprite->SetDrawLayer_40F7B0(12);
            break;

        default:
            return;
    }
}

// https://decomp.me/scratch/zqssg
MATCH_FUNC(0x527070)
bool Object::UpdateMovementAndEffects_527070(Sprite* pSprite, Fix16 x, Fix16 y, Ang16 rot)
{
    byte_6F8C68 = 1;

    if (field_10_obj_3c)
    {
        InheritMotionFromSprite_526B40(pSprite);
    }

    if (field_8_definition->field_34_behavior_type != object_behavior_type::light_11)
    {
        gSpriteGrid_3_679210->Remove_477B00(field_4_sprite);
    }

    PoolTake_522360();

    if (!field_25_removal_state)
    {
        Fix16 xpos;
        Fix16 ypos;
        if (x == kFpZero_6F8E10 && y == kFpZero_6F8E10)
        {
            xpos = pSprite->field_14_xy.x;
            ypos = pSprite->field_14_xy.y;
        }
        else
        {
            xpos = x;
            ypos = y;
            Ang16::RotateVector_41FC90(xpos, ypos, pSprite->field_0_rotation);
            xpos += pSprite->field_14_xy.x;
            ypos += pSprite->field_14_xy.y;
        }

        field_4_sprite->set_xyz_lazy_420600(xpos, ypos, pSprite->field_1C_zpos);
        field_4_sprite->set_ang_lazy_420690(pSprite->field_0_rotation + rot);

        switch (field_8_definition->field_34_behavior_type)
        {
            case object_behavior_type::animated_2:
                UpdateAnimation_5257D0();
                break;

            case object_behavior_type::explosion_5:
            {
                if (field_C_pAny.pExplosion)
                {
                    switch (pSprite->GetType_416B40())
                    {
                        case sprite_types_enum::code_obj1_4:
                        case sprite_types_enum::map_obj_5:
                            if (field_C_pAny.pExplosion->Update_5434A0(field_10_obj_3c->field_C_speed, field_10_obj_3c->field_4_angle))
                            {
                                byte_6F8C68 = 0;
                                return true;
                            }
                            break;

                        case sprite_types_enum::car_2:
                            if (field_C_pAny.pExplosion->IsFireType_5435D0())
                            {
                                field_4_sprite->set_z_lazy_420660(pSprite->field_8_car_bc_ptr->GetZPos_441330());
                                pSprite->field_8_car_bc_ptr->AccumulateDamage_43DA90(1, &gZeroVector_6F8EF0);
                            }

                            field_C_pAny.pExplosion->field_1C_pAttachedSprite = pSprite;

                            if (field_C_pAny.pExplosion->Update_5434A0(field_10_obj_3c->field_C_speed, field_10_obj_3c->field_4_angle))
                            {
                                byte_6F8C68 = 0;
                                return true;
                            }
                            break;

                        case sprite_types_enum::ped_3:
                            if (field_C_pAny.pExplosion->Update_5434A0(field_10_obj_3c->field_C_speed, field_10_obj_3c->field_4_angle))
                            {
                                byte_6F8C68 = 0;
                                return true;
                            }
                            break;

                        default:
                            break;
                    }
                }
                break;
            }

            case object_behavior_type::light_11:
                field_C_pAny.pLight->SetPosition_482D30(field_4_sprite->field_14_xy.x, field_4_sprite->field_14_xy.y, field_4_sprite->field_1C_zpos);
                break;

            default:
                break;
        }

        if (field_10_obj_3c && field_10_obj_3c->field_0_attached_sprites.field_0_p18)
        {
            field_10_obj_3c->field_0_attached_sprites.PoolUpdate_5A6F70(field_4_sprite);
        }

        if (field_8_definition->field_34_behavior_type != object_behavior_type::light_11)
        {
            gSpriteGrid_3_679210->AddToSingleBucket_477AE0(field_4_sprite);
        }
        byte_6F8C68 = 0;
        return false;
    }
    else
    {
        TriggerCarExplosionIfApplicable_526790(pSprite);
        byte_6F8C68 = 0;
        return true;
    }
}

MATCH_FUNC(0x527630)
void Object::InitializeObject_527630(s32 object_type, Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation)
{
    ObjectDefinition* pDefinition = gObjectDefinitions_6FCF00->GetObjectDefinition_534360(object_type);
    field_8_definition = pDefinition;
    field_18_model = object_type;
    field_24_touched_result = 0;

    if (field_4_sprite)
    {
        pDefinition->ApplyDefinitionToSprite_5331A0(field_4_sprite);
    }
    else
    {
        field_4_sprite = pDefinition->CreateSpriteFromDefinition_533170();
    }

    field_4_sprite->set_xyz_lazy_420600(xpos, ypos, zpos);
    field_4_sprite->set_ang_lazy_420690(rotation);

    field_4_sprite->SetObject2C_482A30(this);
}

MATCH_FUNC(0x527990)
void Object::TurnOffLight_527990()
{
    field_C_pAny.pLight->field_0_intensity_radius.flag &= ~0xFF;
}

MATCH_FUNC(0x527A30)
void Object::UpdateLight_527A30()
{
    field_C_pAny.pLight->SetCurrentIntensity_45B2D0(field_C_pAny.pLight->field_18_intensity);
}

MATCH_FUNC(0x527ae0)
void Object::AssignToBucket_527AE0()
{
    switch (field_8_definition->field_40_collision_bucket_category)
    {
        case collision_bucket_category::sprite_grid_3_single_cell_0:
        case collision_bucket_category::sprite_grid_3_single_cell_1:
            gSpriteGrid_3_679210->AddToSingleBucket_477AE0(field_4_sprite);
            return;
        case collision_bucket_category::sprite_grid_2_region_3:
            dword_6F8F88++;
            gSpriteGrid_2_67920C->AddToRegionBuckets_477B20(field_4_sprite);
            return;
        case collision_bucket_category::sprite_grid_1_region_4:
            gSpriteGrid_1_679208->AddToRegionBuckets_477B20(field_4_sprite);
            return;
        case collision_bucket_category::none_2:
            return;
    }
}

MATCH_FUNC(0x527d00)
void Object::RemoveFromCollisionBuckets_527D00()
{
    switch (field_8_definition->field_40_collision_bucket_category)
    {
        case collision_bucket_category::sprite_grid_3_single_cell_0:
        case collision_bucket_category::sprite_grid_3_single_cell_1:
            gSpriteGrid_3_679210->Remove_477B00(field_4_sprite);
            break;
        case collision_bucket_category::sprite_grid_2_region_3:
            --dword_6F8F88;
            gSpriteGrid_2_67920C->RemoveFromRegionBuckets_477B60(field_4_sprite);
            break;
        case collision_bucket_category::sprite_grid_1_region_4:
            gSpriteGrid_1_679208->RemoveFromRegionBuckets_477B60(field_4_sprite);
            break;
        default:
            return;
    }
}

// 9.6f 0x484760
MATCH_FUNC(0x527f10)
void Object::ReleaseSubObjects_527F10()
{

    if (field_C_pAny.o8)
    {
        if (this->field_8_definition->field_34_behavior_type == object_behavior_type::light_11)
        {
            gLight_1D4CC_6F5520->DeallocLight_47F4F0(field_C_pAny.pLight);
            this->field_C_pAny.o8 = NULL;
        }
        else if (this->field_1C_has_explosion)
        {
            field_C_pAny.pExplosion->Release_543610();
            this->field_C_pAny.o8 = NULL;
        }
        else
        {
            gObject_8_Pool_6F8F78->DeAllocate(field_C_pAny.o8);
            this->field_C_pAny.o8 = NULL;
        }
    }

    if (field_10_obj_3c)
    {
        if (field_10_obj_3c->field_0_attached_sprites.field_0_p18)
        {
            field_10_obj_3c->field_0_attached_sprites.DestroyAllSprites_5A7010();
        }

        gObject_3C_Pool_6F8F7C->DeAllocate(field_10_obj_3c);
        field_10_obj_3c = NULL;
    }
}

// 9.6f 0x4847D0
MATCH_FUNC(0x528130)
void Object::NewObj3C_528130(Fix16_Point& speed)
{
    Object_3C* pNewObj = gObject_3C_Pool_6F8F7C->Allocate();

    this->field_10_obj_3c = pNewObj;
    pNewObj->field_20_obj2c_id = field_14_id;

    this->field_10_obj_3c->field_C_speed = speed.GetLength_41E260();
    this->field_10_obj_3c->field_4_angle = speed.atan2_40F790();
}

MATCH_FUNC(0x528240)
bool Object::ChangeInstanceLimitKind_528240(s32 current, s32 desired)
{
    switch (current)
    {
        case 1:
            switch (desired)
            {
                case 2:
                    if (gObject_5C_6F8F84->field_10_limited_object_count == 360)
                    {
                        RequestRemoval_5290A0();
                        return true;
                    }
                    gObject_5C_6F8F84->field_10_limited_object_count++;
                    break;

                case 3:
                    gObject_5C_6F8F84->field_14_listed_sprite_count++;
                    gObject_5C_6F8F84->field_1C_sprite_list.AddSprite_5A6CD0(field_4_sprite);
                    break;
            }
            break;

        case 2:
            switch (desired)
            {
                case 1:
                    gObject_5C_6F8F84->field_10_limited_object_count--;
                    break;

                case 3:
                    gObject_5C_6F8F84->field_10_limited_object_count--;
                    gObject_5C_6F8F84->field_14_listed_sprite_count++;
                    gObject_5C_6F8F84->field_1C_sprite_list.AddSprite_5A6CD0(field_4_sprite);
                    break;
            }
            break;

        case 3:
            switch (desired)
            {
                case 1:
                    gObject_5C_6F8F84->field_14_listed_sprite_count--;
                    gObject_5C_6F8F84->field_1C_sprite_list.RemoveSprite_5A6B10(field_4_sprite);
                    break;

                case 2:
                    if (gObject_5C_6F8F84->field_10_limited_object_count == 360)
                    {
                        RequestRemoval_5290A0();
                        return true;
                    }

                    gObject_5C_6F8F84->field_14_listed_sprite_count--;
                    gObject_5C_6F8F84->field_1C_sprite_list.RemoveSprite_5A6B10(field_4_sprite);
                    gObject_5C_6F8F84->field_10_limited_object_count++;
                    break;
            }
            break;
    }
    return false;
}

MATCH_FUNC(0x5283c0)
void Object::TickObject_5283C0(s32 obj_type)
{

    Fix16_Point dir;

    if (!obj_type)
    {
        Object::RequestRemoval_5290A0();
    }
    else
    {
        if (IsExplosiveItemDefinition_482400(obj_type))
        {
            Object* pExplosion = gObject_5C_6F8F84->CreateExplosion_52A3D0(field_4_sprite->field_14_xy.x,
                                                                              field_4_sprite->field_14_xy.y,
                                                                              field_4_sprite->field_1C_zpos,
                                                                              kZeroAng_6F8F68,
                                                                              GetExplosionTypeForItemDefinition_526830(obj_type),
                                                                              gPedRefTable_7F8_703398->GetPedId_420F10(field_26_ped_ref_idx));
            if (pExplosion)
            {
                pExplosion->SetDamageOwner_529080(this->field_26_ped_ref_idx);
            }
            Object::RequestRemoval_5290A0();
        }
        else
        {
            if (obj_type == 259)
            {
                dir.SetFromPolar_41E210(kFpOne64th_6F8C9C, field_4_sprite->field_0_rotation);
                gParticle_8_6FD5E8->EmitImpactParticles_53FE40(field_4_sprite->field_14_xy.x,
                                                               field_4_sprite->field_14_xy.y,
                                                               field_4_sprite->field_1C_zpos,
                                                               dir);
                Object::RequestRemoval_5290A0();
            }
            else
            {
                ObjectDefinition* pDefinition = gObjectDefinitions_6FCF00->GetObjectDefinition_534360(obj_type);
                if (field_8_definition->field_5C_instance_limit_kind == pDefinition->field_5C_instance_limit_kind ||
                    !Object::ChangeInstanceLimitKind_528240(field_8_definition->field_5C_instance_limit_kind, pDefinition->field_5C_instance_limit_kind))
                {
                    if (!byte_6F8C4C && !byte_6F8C68)
                    {
                        Object::RemoveFromCollisionBuckets_527D00();
                    }
                    field_4_sprite->FreeShapes_59FAD0();

                    switch (pDefinition->field_34_behavior_type)
                    {
                        case object_behavior_type::physics_3:
                        case object_behavior_type::pickup_physics_7:
                            if (!this->field_10_obj_3c)
                            {
                                Object_3C* pNew3C = gObject_3C_Pool_6F8F7C->Allocate();
                                this->field_10_obj_3c = pNew3C;
                                pNew3C->field_20_obj2c_id = field_14_id;
                            }

                            if (field_C_pAny.o8)
                            {
                                gObject_8_Pool_6F8F78->DeAllocate(field_C_pAny.o8);
                                this->field_C_pAny.o8 = NULL;
                            }

                            this->field_10_obj_3c->field_18_friction = pDefinition->field_14_friction;
                            this->field_10_obj_3c->field_1C_z_accel = kFpZero_6F8E10;
                            this->field_10_obj_3c->field_10_z_speed = kFpZero_6F8E10;
                            this->field_10_obj_3c->field_28_next_definition_timer = pDefinition->field_65_next_definition_timer;
                            Object::InitializeObject_527630(obj_type,
                                                               this->field_4_sprite->field_14_xy.x,
                                                               this->field_4_sprite->field_14_xy.y,
                                                               this->field_4_sprite->field_1C_zpos,
                                                               this->field_4_sprite->field_0_rotation);
                            break;

                        case object_behavior_type::animated_2:
                        case object_behavior_type::pickup_animated_8:
                        {
                            if (field_10_obj_3c)
                            {
                                if (!field_10_obj_3c->field_0_attached_sprites.field_0_p18)
                                {
                                    gObject_3C_Pool_6F8F7C->DeAllocate(field_10_obj_3c);
                                    this->field_10_obj_3c = NULL;
                                }
                            }

                            bool bNewObj8;
                            if (this->field_C_pAny.o8)
                            {
                                bNewObj8 = false;
                            }
                            else
                            {
                                bNewObj8 = true;
                                Object_8* pNewObj8 = gObject_8_Pool_6F8F78->Allocate();
                                this->field_C_pAny.o8 = pNewObj8;
                            }

                            if (pDefinition->field_65_next_definition_timer != field_8_definition->field_65_next_definition_timer || bNewObj8)
                            {
                                this->field_C_pAny.o8->field_4_timer = pDefinition->field_65_next_definition_timer;
                                this->field_C_pAny.o8->field_7_frame_idx = 0;
                                this->field_C_pAny.o8->field_6_frame_counter = 0;
                            }
                            else if (pDefinition->field_6C_num_frames < field_8_definition->field_6C_num_frames)
                            {
                                this->field_C_pAny.o8->field_7_frame_idx = 0;
                            }
                            Object::InitializeObject_527630(obj_type,
                                                               this->field_4_sprite->field_14_xy.x,
                                                               this->field_4_sprite->field_14_xy.y,
                                                               this->field_4_sprite->field_1C_zpos,
                                                               this->field_4_sprite->field_0_rotation);
                            field_4_sprite->set_id_lazy_4206C0(this->field_8_definition->field_1E_base_sprite_idx + this->field_C_pAny.o8->field_7_frame_idx);
                            break;
                        }

                        case object_behavior_type::physics_animated_4:
                        case object_behavior_type::pickup_physics_animated_9:
                        {
                            if (!this->field_10_obj_3c)
                            {
                                Object_3C* pNew3C = gObject_3C_Pool_6F8F7C->Allocate();
                                this->field_10_obj_3c = pNew3C;
                                pNew3C->field_20_obj2c_id = field_14_id;
                            }

                            bool bNewObj8;
                            if (this->field_C_pAny.o8)
                            {
                                bNewObj8 = false;
                            }
                            else
                            {
                                bNewObj8 = true;
                                Object_8* pNewObj8 = gObject_8_Pool_6F8F78->Allocate();
                                this->field_C_pAny.o8 = pNewObj8;
                            }

                            if (pDefinition->field_65_next_definition_timer != field_8_definition->field_65_next_definition_timer || bNewObj8)
                            {
                                this->field_C_pAny.o8->field_4_timer = pDefinition->field_65_next_definition_timer;
                                this->field_C_pAny.o8->field_7_frame_idx = 0;
                                this->field_C_pAny.o8->field_6_frame_counter = 0;
                            }
                            else if (pDefinition->field_6C_num_frames < field_8_definition->field_6C_num_frames)
                            {
                                this->field_C_pAny.o8->field_7_frame_idx = 0;
                            }

                            Object::InitializeObject_527630(obj_type,
                                                               this->field_4_sprite->field_14_xy.x,
                                                               this->field_4_sprite->field_14_xy.y,
                                                               this->field_4_sprite->field_1C_zpos,
                                                               this->field_4_sprite->field_0_rotation);
                            field_4_sprite->set_id_lazy_4206C0(this->field_8_definition->field_1E_base_sprite_idx + this->field_C_pAny.o8->field_7_frame_idx);
                            this->field_10_obj_3c->field_18_friction = pDefinition->field_14_friction;
                            this->field_10_obj_3c->field_1C_z_accel = kFpZero_6F8E10;
                            this->field_10_obj_3c->field_10_z_speed = kFpZero_6F8E10;
                            break;
                        }

                        case object_behavior_type::static_object_0:
                        case object_behavior_type::static_collision_check_1:
                        case object_behavior_type::pickup_6:
                        case object_behavior_type::shop_10:
                        case object_behavior_type::effect_only_12:
                            Object::ReleaseSubObjects_527F10();
                            Object::InitializeObject_527630(obj_type,
                                                               this->field_4_sprite->field_14_xy.x,
                                                               this->field_4_sprite->field_14_xy.y,
                                                               this->field_4_sprite->field_1C_zpos,
                                                               this->field_4_sprite->field_0_rotation);
                            if (this->field_10_obj_3c)
                            {
                                return;
                            }
                            break;

                        default:
                            break;
                    }

                    if (!pDefinition->field_61_in_pool_list && !this->field_25_removal_state)
                    {
                        Object::PoolTake_522360();
                    }
                    if (!byte_6F8C68)
                    {
                        byte_6F8F40 = 1;
                        Object::AssignToBucket_527AE0();
                    }
                }
            }
        }
    }
}

MATCH_FUNC(0x5288B0)
bool Object::OnObjectTouched_5288B0(Sprite* pSprite)
{
    if (!pSprite)
    {
        return false;
    }

    Char* pChar = pSprite->AsCharB4_40FEA0();
    if (pChar)
    {
        return pChar->OnObjectTouched_553640(this);
    }

    Car* pCar = pSprite->AsCar_40FEB0();
    if (pCar)
    {
        return pCar->OnObjectTouched_43EA60(this);
    }

    return false;
}

MATCH_FUNC(0x528900)
void Object::HandleWaterDeath_528900()
{
    if (field_10_obj_3c)
    {
        field_10_obj_3c->field_30_bSkipAnim = true;
    }

    if ((gpRng_67AB34->get_cur_rng_41CFE0() & 3) == 0)
    {
        field_4_sprite->IncreaseZoom_59E320(1);
        if (field_4_sprite->ShrinkSprite_59E390(kFpOneThirtySecond_6F8F5C, kFpOneThirtySecond_6F8F5C, 0))
        {
            field_4_sprite->set_z_lazy_420660(kFpZero_6F8E10);
            RequestRemoval_5290A0();
        }
    }
}

MATCH_FUNC(0x528960)
bool Object::HandleObjectHitIfExplosive_528960(Object* pOther)
{
    // Check is explosive range
    if (field_8_definition->field_48_projectile_hit_type < 12 || field_8_definition->field_48_projectile_hit_type > 13)
    {
        return false;
    }

    pOther->ProcessObjectExplosionImpact_528A20(this);
    return true;
}

MATCH_FUNC(0x528990)
bool Object::HandleObjectHit_528990(Sprite* pSprite)
{
    Char* pChar = pSprite->AsCharB4_40FEA0();
    if (pChar)
    {
        return pChar->HandlePedObjectHit_5537F0(this);
    }

    Car* pCar = pSprite->AsCar_40FEB0();
    if (pCar)
    {
        return pCar->HandleCarHitByObject_43F130(this);
    }

    Object* pObject = pSprite->AsObject2C_40FEC0();

    if (gPedRefTable_7F8_703398->GetPedId_420F10(get_field_26_420FF0()))
    {
        Ped* pPed = gPedManager_6787BC->PedById(gPedRefTable_7F8_703398->GetPedId_420F10(get_field_26_420FF0()));
        if (pPed)
        {
            pPed->ProcessWeaponHitResponse_46FE20(pObject);
        }
    }
    return pObject->HandleObjectHitIfExplosive_528960(this);
}

MATCH_FUNC(0x528A20)
void Object::ProcessObjectExplosionImpact_528A20(Object* pObj)
{
    switch (this->field_18_model)
    {
        case objects::rocket_bullet_128:
        case objects::moving_molotov_138:
        {
            s32 explosion_kind;
            if (pObj->field_18_model == objects::diagonal_wall_collision_obj_166)
            {
                explosion_kind = pObj->GetExplosionTypeForDiagonalWall_529210();
            }
            else
            {
                if (pObj->field_8_definition->field_48_projectile_hit_type == 13)
                {
                    explosion_kind = 19;
                }
                else
                {
                    explosion_kind = 18;
                }
            }
            Object* pExplosion = gObject_5C_6F8F84->CreateExplosion_52A3D0(this->field_4_sprite->field_14_xy.x,
                                                                              this->field_4_sprite->field_14_xy.y,
                                                                              this->field_4_sprite->field_1C_zpos,
                                                                              kZeroAng_6F8F68,
                                                                              explosion_kind,
                                                                              gPedRefTable_7F8_703398->GetPedId_420F10(get_field_26_420FF0()));
            if (pExplosion)
            {
                pExplosion->SetDamageOwner_529080(get_field_26_420FF0());
            }
            break;
        }

        case objects::invisible_bullet_154:
        case objects::invisible_rocket_159:
        case objects::shotgun_bullet_192:
        case objects::tanktop_193:
        case objects::fire_hitting_194:
        case objects::object_195:
        case objects::water_hitting_198:
        case objects::object_199:
        case objects::machine_gun_bullet_254:
        case objects::pistol_bullet_265:
        case objects::electrobaton_bullet_277:
            break;
        default:
            return;
    }

    Ped* pPed;
    s32 pedId = gPedRefTable_7F8_703398->GetPedId_420F10(get_field_26_420FF0());
    if (pedId)
    {
        pPed = gPedManager_6787BC->PedById(pedId);
    }
    else
    {
        pPed = NULL;
    }

    if (pObj->field_18_model == objects::diagonal_wall_collision_obj_166)
    {
        if (pPed)
        {
            pPed->NotifyWeaponHit_46FF00(this->field_4_sprite->field_14_xy.x, this->field_4_sprite->field_14_xy.y, this->field_18_model);
        }
    }
}

MATCH_FUNC(0x528ba0)
void Object::HandleImpactNoSprite_528BA0()
{
    Fix16_Point point;

    switch (this->field_18_model)
    {
        case objects::rocket_bullet_128:
        case objects::moving_molotov_138:
        {
            Object* pExplosion = gObject_5C_6F8F84->CreateExplosion_52A3D0(field_4_sprite->field_14_xy.x,
                                                                              field_4_sprite->field_14_xy.y,
                                                                              field_4_sprite->field_1C_zpos,
                                                                              kZeroAng_6F8F68,
                                                                              GetExplosionTypeForWallSide_528E00(gObject2C_WallHitSide_6F8F90),
                                                                              gPedRefTable_7F8_703398->GetPedId_420F10(get_field_26_420FF0()));
            if (pExplosion)
            {
                pExplosion->SetDamageOwner_529080(get_field_26_420FF0());
            }
            // Fall through
        }

        case objects::machine_gun_bullet_254:
        case objects::pistol_bullet_265:
        {
            point.FromPolar_41E210(kFpOne64th_6F8C9C, field_4_sprite->field_0_rotation + kAng180_6F8D62);
            gParticle_8_6FD5E8->EmitImpactParticles_53FE40(field_4_sprite->field_14_xy.x,
                                                           field_4_sprite->field_14_xy.y,
                                                           field_4_sprite->field_1C_zpos,
                                                           point);
            break;
        }

        case objects::invisible_bullet_154:
        case objects::invisible_rocket_159:
        case objects::shotgun_bullet_192:
        case objects::tanktop_193:
        case objects::fire_hitting_194:
        case objects::object_195:
        case objects::water_hitting_198:
        case objects::object_199:
        case objects::electrobaton_bullet_277:
            break;

        default:
            return;
    }

    const s32 id = gPedRefTable_7F8_703398->GetPedId_420F10(get_field_26_420FF0());
    if (id)
    {
        Ped* pPed = gPedManager_6787BC->PedById(id);
        if (pPed)
        {
            pPed->NotifyWeaponHit_46FF00(field_4_sprite->field_14_xy.x, field_4_sprite->field_14_xy.y, field_18_model);
        }
    }
}

MATCH_FUNC(0x528E00)
s32 __stdcall Object::GetExplosionTypeForWallSide_528E00(s32 wall_side)
{
    s32 result;

    switch (wall_side)
    {
        case 0:
            result = 18;
            break;
        case 4:
            result = 25;
            break;
        case 1:
            result = 23;
            break;
        case 2:
            result = 22;
            break;
        default:
            result = 24;
            break;
    }
    return result;
}

MATCH_FUNC(0x528e50)
void Object::HandleImpact_528E50(Sprite* pSprite)
{
    // Case order decides VC6's block layout and tail merging; case 11 has to come before case 3.
    if (!this->field_24_touched_result && (!pSprite || ShouldCollideWith_5223C0(pSprite)))
    {
        this->field_24_touched_result = 1;
        switch (field_8_definition->field_44_touch_behavior)
        {
            case 1:
            case 2:
                PoolGive_522340(); // destroy
                break;

            case 7:
            case 8:
                if (pSprite)
                {
                    this->field_24_touched_result = HandleObjectHit_528990(pSprite);
                }
                else
                {
                    HandleImpactNoSprite_528BA0();
                }

                if (!field_24_touched_result)
                {
                    return;
                }
                PoolGive_522340();
                break;

            case 6:
                this->field_24_touched_result = OnObjectTouched_5288B0(pSprite);
                if (field_24_touched_result)
                {
                    PoolGive_522340(); // destroy
                }
                break;

            case 9:
                this->field_24_touched_result = OnObjectTouched_5288B0(pSprite);
                break;

            case 11:
                this->field_24_touched_result = 0;
                break;

            case 3:
                if (pSprite->AsCar_40FEB0())
                {
                    this->field_24_touched_result = 1;
                }
                else
                {
                    this->field_24_touched_result = 0;
                }

                if (!field_24_touched_result)
                {
                    return;
                }
                PoolGive_522340();
                break;

            case 4:
            case 5:
                if (pSprite->AsCharB4_40FEA0())
                {
                    this->field_24_touched_result = 1;
                }
                else
                {
                    this->field_24_touched_result = 0;
                }

                if (!field_24_touched_result)
                {
                    return;
                }
                PoolGive_522340();
                break;

            case 10:
                if (!pSprite)
                {
                    break;
                }
                if (pSprite->AsCharB4_40FEA0())
                {
                    this->field_24_touched_result = 1;
                }
                else
                {
                    this->field_24_touched_result = 0;
                }
                break;

            default:
                break;
        }

        if (this->field_24_touched_result)
        {
            if (pSprite)
            {
                if (!IsPickup_421060() || !pSprite->IsControlledByRemotePlayer_59E170())
                {
                    gCollisionSoundQueue_66AFE0->AddSpriteCollision_40BDD0(field_4_sprite, pSprite);
                }
            }
            else if (gCollisionTarget_679188.field_0_type == collision_target_type::floor_below_4)
            {
                gCollisionSoundQueue_66AFE0->AddFloorCollision_40BC40(field_4_sprite);
            }
            else if (gCollisionTarget_679188.field_0_type == collision_target_type::block_at_z_5)
            {
                gCollisionSoundQueue_66AFE0->AddBlockCollision_40BD10(field_4_sprite);
            }
            else
            {
                gCollisionSoundQueue_66AFE0->AddCollision_40BBA0(field_4_sprite, kFpZero_6F8E10);
            }
        }
    }
}

MATCH_FUNC(0x529000)
void Object::HandleCollisionWithObject_529000(Object* pObj)
{
    switch (pObj->field_18_model)
    {
        case objects::conveyor_139:
            ApplyConveyorSpeedFrom_529070(pObj);
            break;

        case objects::destructor_141:
            if (field_8_definition->field_60_destroyed_by_destructor)
            {
                Dealloc_5291B0();
            }
            break;
    }
}

MATCH_FUNC(0x529030)
void Object::SetConveyorSpeed_529030(s8 speed_x, s8 speed_y)
{
    field_26_ped_ref_idx = (speed_y + 7) | (16 * (speed_x + 7)); // x and y as nibbles, read back by UnpackSignedNibbles_529050
}

MATCH_FUNC(0x529070)
void Object::ApplyConveyorSpeedFrom_529070(Object* pObj)
{
    SetConveyorPush_5226A0(pObj->field_26_ped_ref_idx);
}

MATCH_FUNC(0x529080)
void Object::SetDamageOwner_529080(u8 ped_ref_idx)
{
    field_26_ped_ref_idx = ped_ref_idx;
    gPedRefTable_7F8_703398->IncrementRefCount_59B0B0(ped_ref_idx);
}

MATCH_FUNC(0x5290a0)
void Object::RequestRemoval_5290A0()
{
    field_25_removal_state = object_removal_state::requested_1;
}

MATCH_FUNC(0x5290B0)
void Object::RequestRemovalWhenOffScreen_5290B0()
{
    field_25_removal_state = object_removal_state::when_off_screen_2;
}

MATCH_FUNC(0x5290C0)
void Object::SetSpriteIdxOffset_5290C0(u8 offset)
{
    s16 new_id = offset + field_8_definition->field_1E_base_sprite_idx;
    Sprite* pSprite = field_4_sprite;
    if (pSprite->field_22_type_sprite_idx != new_id)
    {
        pSprite->field_22_type_sprite_idx = new_id;
        pSprite->UpdateDimensionsFromSpriteIndex_59FA40();
    }
}

MATCH_FUNC(0x5290F0)
Fix16 Object::GetEffectiveSpeed_5290F0() const
{
    if (field_10_obj_3c)
    {
        if (!field_10_obj_3c->field_2A_bAirborne)
        {
            if (field_10_obj_3c->field_C_speed == kFpZero_6F8E10)
            {
                return field_10_obj_3c->field_10_z_speed;
            }
            return field_10_obj_3c->field_C_speed;
        }
        else
        {
            if (field_10_obj_3c->field_C_speed == kFpZero_6F8E10)
            {
                if (field_10_obj_3c->field_10_z_speed == kFpZero_6F8E10)
                {
                    if (this->field_18_model == objects::explosion_113)
                    {
                        return kFpZero_6F8E10;
                    }
                    return kFP16One_6F8E14;
                }
                return field_10_obj_3c->field_10_z_speed;
            }
            return field_10_obj_3c->field_C_speed;
        }
    }
    return kFpZero_6F8E10;
}

MATCH_FUNC(0x5291b0)
void Object::Dealloc_5291B0()
{
    PoolGive_522340();
    RequestRemoval_5290A0();
}

MATCH_FUNC(0x5291d0)
void Object::PoolGiveAndMarkDone_5291D0()
{
    PoolGive_522340();
    field_24_touched_result = 1;
}

MATCH_FUNC(0x5291E0)
void Object::PoolGiveAndQueueObjectType_5291E0(s32 object_type)
{
    PoolGive_522340();
    field_24_touched_result = object_type;
}

MATCH_FUNC(0x529200)
bool Object::IsNotModel_174_529200() const
{
    return (field_18_model != 174) ? true : false;
}

// https://decomp.me/scratch/lXvKN
MATCH_FUNC(0x529210)
s32 Object::GetExplosionTypeForDiagonalWall_529210() const
{
    switch (field_26_ped_ref_idx)
    {
        case 46: // diagonal up right
        case 48: // diagonal down right
            return 22;
        case 45: // diagonal up left
        case 47: // diagonal down left
            return 23;
    }
    return 23;
}

MATCH_FUNC(0x529240)
s32 Object::GetMapBlockSoundSpec_529240() const
{
    s32 result;
    gmp_block_info* pBlockInfo =
        gMap_0x370_6F6268->GetBlock_4DFE10(field_4_sprite->field_14_xy.x.ToInt(), field_4_sprite->field_14_xy.y.ToInt(), field_4_sprite->field_1C_zpos.ToInt());
    switch (get_field_26_420FF0())
    {
        case 45u:
        case 47u:
            result = gGtx_0x106C_703DD4->GetSpecIgnoringRoadJunction_462FD0(pBlockInfo->field_0_left & block_face::tile_idx_mask);
            break;

        case 46u:
        case 48u:
            result = gGtx_0x106C_703DD4->GetSpecIgnoringRoadJunction_462FD0(pBlockInfo->field_2_right & block_face::tile_idx_mask);
            break;

        default:
            result = 0;
            break;
    }
    return result;
}

MATCH_FUNC(0x5292D0)
void Object::SetDefaultWeaponAmmo_5292D0()
{
    s32 weapon_kind;
    if (field_18_model >= 200)
    {
        weapon_kind = field_18_model - 200;
    }
    else
    {
        weapon_kind = field_18_model - 64;
    }

    if (weapon_kind <= 27)
    {
        set_field_26(gWeapon_8_707018->get_defalt_ammo_5E3E80(weapon_kind));
    }
}

MATCH_FUNC(0x529300)
void Object_5C::TrimSpriteList_529300()
{
    for (s32 i = field_14_listed_sprite_count - 88; i >= 0; i--)
    {
        Sprite* pSprite = field_1C_sprite_list.PopBackSprite_5A6DC0();
        Object* pObject = pSprite->AsObject2C_40FEC0();
        if (pObject->field_18_model == objects::mine_10)
        {
            if (gGame_0x40_67E008->IsSpriteInAnyPlayerBounds_4B97E0(pSprite, kFpZero_6F8E10))
            {
                Object_5C::CreateExplosion_52A3D0(pSprite->field_14_xy.x,
                                                  pSprite->field_14_xy.y,
                                                  pSprite->field_1C_zpos,
                                                  kZeroAng_6F8F68,
                                                  explosion_type::small_18,
                                                  gPedRefTable_7F8_703398->GetPedId_420F10(pObject->get_field_26_420FF0()));
            }
        }
        pObject->Dealloc_5291B0();
    }
}

MATCH_FUNC(0x5293a0)
void Object_5C::ObjectsService_5293A0()
{
    TrimSpriteList_529300();

    gObject_2C_Pool_6F8F80->UpdatePool();
}

MATCH_FUNC(0x529430)
Object_5C::Object_5C()
{
    for (u8 i = 0; i < 50; i++)
    {
        field_20_uncollected_tokens[i] = 1;
    }

    field_54_uncollected_token_index = 0;
    byte_6F8C68 = 0;
    byte_6F8C4C = 0;
    byte_6F8F40 = 0;
    field_10_limited_object_count = 0;
    field_14_listed_sprite_count = 0;
    gObj3C_live_count_6F8E54 = 0;
    gObj8_count_6F8F18 = 0;
    gObj8_dealloc_count_6F8DC0 = 0;
    gObj3C_dealloc_count_6F8F0C = 0;
    field_58_collision_probe_sprite = NULL;
    field_18_pending_damage_owner = 0;
    byte_6F8EDC = 0;

    if (!gObject_2C_Pool_6F8F80)
    {
        gObject_2C_Pool_6F8F80 = new TObjectPool();
        if (!gObject_2C_Pool_6F8F80)
        {
            FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\object.cpp", 4239);
        }
    }

    gObject_8_Pool_6F8F78 = new TObject_8_Pool();
    if (!gObject_8_Pool_6F8F78)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\object.cpp", 4243);
    }

    gObject_3C_Pool_6F8F7C = new TObject_3C_Pool();
    if (!gObject_3C_Pool_6F8F7C)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\object.cpp", 4245);
    }

    field_58_collision_probe_sprite = gSpritePool_703818->get_new_sprite();
    field_58_collision_probe_sprite->set_xyz_lazy_451950(0, 0, 0);

    field_58_collision_probe_sprite->set_ang_lazy_420690(kZeroAng_6F8F68);
    field_58_collision_probe_sprite->AllocCollisionShape_59F950(0, 0, 0);

    field_0_diagonal_wall_ang315 = NULL;
    field_C_diagonal_wall_ang45 = NULL;
    field_8_diagonal_wall_ang225 = NULL;
    field_4_diagonal_wall_ang135 = NULL;
}

MATCH_FUNC(0x529750)
Object_5C::~Object_5C()
{
    if (field_58_collision_probe_sprite)
    {
        gSpritePool_703818->remove(field_58_collision_probe_sprite);
        field_58_collision_probe_sprite = NULL;
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

    field_0_diagonal_wall_ang315 = NULL;
    field_C_diagonal_wall_ang45 = NULL;
    field_8_diagonal_wall_ang225 = NULL;
    field_4_diagonal_wall_ang135 = NULL;
}

MATCH_FUNC(0x5297f0)
void Object_5C::CreateDiagonalWallObjects_5297F0()
{
    field_0_diagonal_wall_ang315 = Object_5C::NewPhysicsObj_5299B0(objects::diagonal_wall_collision_obj_166, 0, 0, 0, kAng315_6F8D8C);
    field_0_diagonal_wall_ang315->set_field_26(45);

    field_4_diagonal_wall_ang135 = Object_5C::NewPhysicsObj_5299B0(objects::diagonal_wall_collision_obj_166, 0, 0, 0, kAng135_6F8D80);
    field_4_diagonal_wall_ang135->set_field_26(48);

    field_8_diagonal_wall_ang225 = Object_5C::NewPhysicsObj_5299B0(objects::diagonal_wall_collision_obj_166, 0, 0, 0, kAng225_6F8D54);
    field_8_diagonal_wall_ang225->set_field_26(46);

    field_C_diagonal_wall_ang45 = Object_5C::NewPhysicsObj_5299B0(objects::diagonal_wall_collision_obj_166, 0, 0, 0, kAng45_6F8CD0);
    field_C_diagonal_wall_ang45->set_field_26(47);
}

MATCH_FUNC(0x5298e0)
Object* Object_5C::GetDirectionalObject_5298E0(s32 slope_type) const
{
    Object* result;
    switch (slope_type)
    {
        case 180:
        case 196:
            result = this->field_0_diagonal_wall_ang315;
            break;
        case 184:
        case 200:
            result = this->field_8_diagonal_wall_ang225;
            break;
        case 188:
        case 204:
            result = this->field_C_diagonal_wall_ang45;
            break;
        case 192:
        case 208:
            result = this->field_4_diagonal_wall_ang135;
            break;
        default:
            result = NULL;
            break;
    }
    return result;
}

MATCH_FUNC(0x529950)
Object* Object_5C::NewTouchPoint_529950(s32 object_type, Fix16 x, Fix16 y, Fix16 z, Ang16 rot, Fix16 w, Fix16 h, Fix16 depth)
{
    Object* pNewObj = CreateObject_529C00(object_type, x, y, z, rot, false);
    if (pNewObj)
    {
        pNewObj->field_4_sprite->SetDimensions_482A40(w, h, depth);
        pNewObj->AssignToBucket_527AE0();
    }
    return pNewObj;
}

MATCH_FUNC(0x5299b0)
Object* Object_5C::NewPhysicsObj_5299B0(s32 object_type, Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation)
{
    Object* pNewObj = CreateObject_529C00(object_type, xpos, ypos, zpos, rotation, false);
    if (pNewObj)
    {
        pNewObj->AssignToBucket_527AE0();
    }
    return pNewObj;
}

MATCH_FUNC(0x5299f0)
Object* Object_5C::NewWithPedRefIdx_5299F0(s32 object_type, u32 ped_ref_idx, Fix16 xpos, Fix16 ypos, Fix16 zpos)
{
    Object* pNewObj = CreateObject_529C00(object_type, xpos, ypos, zpos, kZeroAng_6F8F68, false);
    if (pNewObj)
    {
        pNewObj->set_field_26(ped_ref_idx);
        if (object_type == objects::sound_object_type_2_279)
        {
            pNewObj->RequestRemoval_5290A0();
        }
    }
    return pNewObj;
}

MATCH_FUNC(0x529a40)
Object* Object_5C::NewLight_529A40(Fix16 xpos, Fix16 ypos, Fix16 zpos, s32 argb, Fix16 radius, u8 intensity)
{
    Object* pNewObj = CreateObject_529C00(objects::light_165, xpos, ypos, zpos, kZeroAng_6F8F68, false);
    if (pNewObj)
    {
        pNewObj->field_C_pAny.pLight->SetColourRadiusIntensity_482D60(argb, radius, intensity);
    }
    return pNewObj;
}

MATCH_FUNC(0x529ab0)
Object* Object_5C::NewLight_529AB0(s32 light_type, Fix16 xpos, Fix16 ypos, Fix16 zpos, u32 argb, Fix16 radius, u8 intensity)
{
    Object* pNewObj = Object_5C::CreateObject_529C00(light_type, xpos, ypos, zpos, kZeroAng_6F8F68, false);
    if (pNewObj)
    {
        pNewObj->field_C_pAny.pLight->SetColourRadiusIntensity_482D60(argb, radius, intensity);
    }
    return pNewObj;
}

MATCH_FUNC(0x529B20)
Object* Object_5C::NewFlashingLight_529B20(Fix16 xpos,
                                             Fix16 ypos,
                                             Fix16 zpos,
                                             s32 argb,
                                             Fix16 radius,
                                             u8 intensity,
                                             u8 on_time,
                                             u8 off_time,
                                             u8 shape)
{
    Object* pNewObj = CreateObject_529C00(objects::light_165, xpos, ypos, zpos, kZeroAng_6F8F68, false);
    if (pNewObj)
    {
        pNewObj->field_C_pAny.pLight->SetColourRadiusIntensity_482D60(argb, radius, intensity);
        gLight_1D4CC_6F5520->SetFlashing_469070(pNewObj->field_C_pAny.pLight, on_time, off_time, shape);
    }
    return pNewObj;
}

MATCH_FUNC(0x529bc0)
Object* Object_5C::NewPhysicsObjUnlessBlocked_529BC0(s32 object_type, Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation)
{
    Object* pNewObj = CreateObject_529C00(object_type, xpos, ypos, zpos, rotation, true);
    if (pNewObj)
    {
        pNewObj->AssignToBucket_527AE0();
    }
    return pNewObj;
}

// 9.6f 0x484E00
MATCH_FUNC(0x529c00)
Object* Object_5C::CreateObject_529C00(s32 object_type, Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation, bool fail_if_blocked)
{
    if (object_type == objects::secret_token_266)
    {
        if (!field_20_uncollected_tokens[field_54_uncollected_token_index])
        {
            field_54_uncollected_token_index++;
            return NULL;
        }
    }

    ObjectDefinition* pDefinition = gObjectDefinitions_6FCF00->GetObjectDefinition_534360(object_type);
    if (pDefinition->field_5C_instance_limit_kind == 2)
    {
        if (field_10_limited_object_count == 360)
        {
            return NULL;
        }
        field_10_limited_object_count++;
    }

    Object* pNewObject;
    if (pDefinition->field_61_in_pool_list)
    {
        pNewObject = gObject_2C_Pool_6F8F80->Allocate();
        pNewObject->field_20_pool_list_state = object_pool_state::in_pool_list_1;
    }
    else
    {
        pNewObject = gObject_2C_Pool_6F8F80->AllocateUntracked();
        pNewObject->field_20_pool_list_state = object_pool_state::out_of_pool_list_2;
    }

    pNewObject->InitializeObject_527630(object_type, xpos, ypos, zpos, rotation);
    if (field_18_pending_damage_owner)
    {
        pNewObject->SetDamageOwner_529080(field_18_pending_damage_owner);
        field_18_pending_damage_owner = 0;
    }

    if (fail_if_blocked &&
        (pNewObject->field_4_sprite->QuerySpriteCollision_59E7D0(0) ||
         (pDefinition->field_40_collision_bucket_category == collision_bucket_category::sprite_grid_2_region_3 &&
          gSpriteGrid_2_67920C->FindCollidingSpritePreferType_477E60(pNewObject->field_4_sprite, 0))))
    {
        if (pNewObject->field_20_pool_list_state == object_pool_state::in_pool_list_1)
        {
            gObject_2C_Pool_6F8F80->FindAndDeAllocate(pNewObject);
        }
        else
        {
            gObject_2C_Pool_6F8F80->DeAllocate(pNewObject);
        }
        return NULL;
    }

    if (pDefinition->field_5C_instance_limit_kind == 3)
    {
        ++field_14_listed_sprite_count;
        field_1C_sprite_list.AddSprite_5A6CD0(pNewObject->field_4_sprite);
    }

    switch (pDefinition->field_34_behavior_type)
    {

        case object_behavior_type::static_object_0:
        case object_behavior_type::static_collision_check_1:
        case object_behavior_type::pickup_6:
        case object_behavior_type::shop_10:
        case object_behavior_type::effect_only_12:
            pNewObject->field_C_pAny.o8 = NULL;
            pNewObject->field_10_obj_3c = NULL;
            break;

        case object_behavior_type::explosion_5:
        {
            Explosion* pExplosion;
            pExplosion = gExplosionPool_7A8_6FD5F0->Allocate_543800();
            pNewObject->field_C_pAny.pExplosion = pExplosion;
            if (pExplosion)
            {
                pNewObject->field_1C_has_explosion = true;
            }
            else
            {
                return NULL;
            }

            break;
        }

        case object_behavior_type::animated_2:
        case object_behavior_type::pickup_animated_8:
        {
            Object_8* pNew8;
            pNew8 = gObject_8_Pool_6F8F78->Allocate();
            pNewObject->field_C_pAny.o8 = pNew8;
            pNewObject->field_C_pAny.o8->field_7_frame_idx = 0;
            pNewObject->field_C_pAny.o8->field_4_timer = pDefinition->field_65_next_definition_timer;
            pNewObject->field_C_pAny.o8->field_6_frame_counter = 0;
            break;
        }

        case object_behavior_type::physics_3:
        case object_behavior_type::pickup_physics_7:
        {
            Object_3C* pNew3C;
            pNew3C = gObject_3C_Pool_6F8F7C->Allocate();
            pNewObject->field_10_obj_3c = pNew3C;
            pNew3C->field_20_obj2c_id = pNewObject->field_14_id;
            pNewObject->field_10_obj_3c->field_C_speed = pNewObject->field_8_definition->field_10_speed;
            pNewObject->field_10_obj_3c->field_18_friction = pDefinition->field_14_friction;
            pNewObject->field_10_obj_3c->field_4_angle = rotation;
            pNewObject->field_10_obj_3c->field_28_next_definition_timer = pNewObject->field_8_definition->field_65_next_definition_timer;
            pNewObject->field_10_obj_3c->field_10_z_speed = kFpZero_6F8E10;
            pNewObject->field_10_obj_3c->field_1C_z_accel = kFpZero_6F8E10;
            break;
        }

        case object_behavior_type::physics_animated_4:
        case object_behavior_type::pickup_physics_animated_9:
        {
            Object_3C* pNew3C;
            pNew3C = gObject_3C_Pool_6F8F7C->Allocate();
            pNewObject->field_10_obj_3c = pNew3C;
            pNew3C->field_20_obj2c_id = pNewObject->field_14_id;
            pNewObject->field_10_obj_3c->field_C_speed = pNewObject->field_8_definition->field_10_speed;
            pNewObject->field_10_obj_3c->field_18_friction = pDefinition->field_14_friction;
            pNewObject->field_10_obj_3c->field_10_z_speed = kFpZero_6F8E10;
            pNewObject->field_10_obj_3c->field_1C_z_accel = kFpZero_6F8E10;
            pNewObject->field_10_obj_3c->field_4_angle = rotation;
            pNewObject->field_10_obj_3c->field_28_next_definition_timer = pNewObject->field_8_definition->field_65_next_definition_timer;

            Object_8* pNew8;
            pNew8 = gObject_8_Pool_6F8F78->Allocate();
            pNewObject->field_C_pAny.o8 = pNew8;
            pNewObject->field_C_pAny.o8->field_7_frame_idx = 0;
            pNewObject->field_C_pAny.o8->field_4_timer = pDefinition->field_65_next_definition_timer;
            pNewObject->field_C_pAny.o8->field_6_frame_counter = 0;
            break;
        }

        case object_behavior_type::light_11:
            pNewObject->field_C_pAny.pLight = gLight_1D4CC_6F5520->CreateLight_52B2A0(xpos, ypos, zpos, 0, 0, 0);
            break;

        default:
            break;
    }
    ++dword_6F8F88;

    pNewObject->field_18_model = object_type;
    if (pNewObject->IsPickup_421060())
    {
        pNewObject->SetDefaultWeaponAmmo_5292D0();
    }

    if (pNewObject->field_18_model == objects::power_generator_281)
    {
        Object* pLight = NewPhysicsObj_5299B0(objects::powergen_light_284, kFpZero_6F8E10, kFpZero_6F8E10, kFpZero_6F8E10, kZeroAng_6F8F68);
        pNewObject->field_4_sprite->AttachSprite_5A3100(pLight->field_4_sprite,
                                                       (kFpEighteen_6F8CE8 * kFpOne64th_6F8ECC),
                                                       (kFP16One_6F8CEC * kFpOne64th_6F8ECC),
                                                       kZeroAng_6F8F68); // ang?
    }
    else
    {
        if (pNewObject->field_18_model == objects::secret_token_266)
        {
            pNewObject->set_field_26(field_54_uncollected_token_index);
            field_54_uncollected_token_index++;
        }
    }
    return pNewObject;
}

MATCH_FUNC(0x52a210)
u8 Object_5C::SetPendingDamageOwner_52A210(u8 ped_ref_idx)
{
    field_18_pending_damage_owner = ped_ref_idx;
    return ped_ref_idx;
}

MATCH_FUNC(0x52a240)
Object* Object_5C::NewMovingObj_52A240(s32 object_type,
                                        Fix16 xpos,
                                        Fix16 ypos,
                                        Fix16 zpos,
                                        Ang16 move_angle,
                                        Ang16 rotation,
                                        Fix16 speed,
                                        Fix16 friction,
                                        Fix16 z_speed)
{
    return Object_5C::CreateMovingObj_52A2C0(object_type, xpos, ypos, zpos, move_angle, rotation, speed, friction, z_speed, false);
}

MATCH_FUNC(0x52a280)
Object* Object_5C::NewMovingObjUnlessBlocked_52A280(s32 object_type,
                                 Fix16 xpos,
                                 Fix16 ypos,
                                 Fix16 zpos,
                                 Ang16 move_angle,
                                 Ang16 rotation,
                                 Fix16 speed,
                                 Fix16 friction,
                                 Fix16 z_speed)
{
    return Object_5C::CreateMovingObj_52A2C0(object_type, xpos, ypos, zpos, move_angle, rotation, speed, friction, z_speed, true);
}

MATCH_FUNC(0x52a2c0)
Object* Object_5C::CreateMovingObj_52A2C0(s32 object_type,
                                 Fix16 xpos,
                                 Fix16 ypos,
                                 Fix16 zpos,
                                 Ang16 move_angle,
                                 Ang16 rotation,
                                 Fix16 speed,
                                 Fix16 friction,
                                 Fix16 z_speed,
                                 bool fail_if_blocked)
{
    Object* pNewObj = Object_5C::CreateObject_529C00(object_type, xpos, ypos, zpos, rotation, fail_if_blocked);

    if (pNewObj)
    {
        pNewObj->AssignToBucket_527AE0();
        ObjectDefinition* pDefinition = gObjectDefinitions_6FCF00->GetObjectDefinition_534360(object_type);
        if (!pNewObj->field_10_obj_3c)
        {
            Object_3C* pObject3C = gObject_3C_Pool_6F8F7C->Allocate();
            pNewObj->field_10_obj_3c = pObject3C;
            if (!pObject3C)
            {
                pNewObj->RequestRemoval_5290A0();
                return NULL;
            }
        }
        pNewObj->field_10_obj_3c->field_C_speed = speed;
        pNewObj->field_10_obj_3c->field_10_z_speed = z_speed;
        pNewObj->field_10_obj_3c->field_14_saved_speed = speed;
        pNewObj->field_10_obj_3c->field_1C_z_accel = kObjectGravity_6F8DA8;
        pNewObj->field_10_obj_3c->field_18_friction = friction;
        pNewObj->field_10_obj_3c->field_4_angle = move_angle;
        pNewObj->field_10_obj_3c->field_28_next_definition_timer = pDefinition->field_65_next_definition_timer;
        Object_3C* pObj3C = pNewObj->field_10_obj_3c;
        if (pObj3C->field_10_z_speed != kFpZero_6F8E10)
        {
            pObj3C->field_2A_bAirborne = 1;
        }
    }
    return pNewObj;
}

MATCH_FUNC(0x52a3d0)
Object* Object_5C::CreateExplosion_52A3D0(Fix16 x, Fix16 y, Fix16 z, Ang16 rot, s32 kind, s32 ped_id)
{
    Object* pNewObject = NewPhysicsObj_5299B0(objects::explosion_113, x, y, z, rot);
    if (pNewObject)
    {
        pNewObject->field_1C_has_explosion = true;

        if (!pNewObject->field_C_pAny.pExplosion)
        {
            pNewObject->field_C_pAny.pExplosion = gExplosionPool_7A8_6FD5F0->Allocate_543800();
        }

        if (!pNewObject->field_C_pAny.pExplosion)
        {
            return NULL;
        }

        pNewObject->field_C_pAny.pExplosion->SetType_482A90(kind);
        pNewObject->field_C_pAny.pExplosion->SetObject_543680(pNewObject);
        pNewObject->field_C_pAny.pExplosion->field_2C_owner_ped_id = ped_id;

        switch (kind)
        {
            case explosion_type::small_18:
            case explosion_type::item_19:
            case explosion_type::large_20:
            case explosion_type::building_45_22:
            case explosion_type::building_225_23:
            case explosion_type::building_135_24:
            case explosion_type::building_315_25:
            case explosion_type::no_ring_32:
            case explosion_type::small_33:
                pNewObject->field_C_pAny.pExplosion->field_1A_timer = explosion_timer::start_100;
                pNewObject->field_C_pAny.pExplosion->field_24_particle_spread = 0;
                break;
            default:
                pNewObject->field_C_pAny.pExplosion->field_1A_timer = explosion_timer::forever_9999;
                break;
        }

        if (!pNewObject->field_10_obj_3c)
        {
            Object_3C* pNew3C = gObject_3C_Pool_6F8F7C->Allocate();
            pNewObject->field_10_obj_3c = pNew3C;
            pNew3C->field_20_obj2c_id = pNewObject->field_14_id;
            pNewObject->field_10_obj_3c->field_C_speed = kFpZero_6F8E10;
            pNewObject->field_10_obj_3c->field_10_z_speed = kFpZero_6F8E10;
        }
        return pNewObject;
    }
    return NULL;
}

MATCH_FUNC(0x52A500)
void Object_5C::SaveObjects_52A500(SavedObjectData* pData)
{
    u16 saved_count = 0;
    Object* pObj = gObject_2C_Pool_6F8F80->field_8_pool;
    for (u16 i = 0; i < 3825; pObj++, i++)
    {
        if (pObj->field_18_model && pObj->IsPickup_421060())
        {
            pData->field_0_obj_x[0][saved_count].field_0_value = pObj->GetX_4340D0();
            pData->field_0_obj_x[1][saved_count].field_0_value = pObj->GetY_4340E0();
            pData->field_0_obj_x[2][saved_count].field_0_value = pObj->GetZ_4340F0();
            pData->field_F0_model[saved_count] = pObj->field_18_model;
            pData->field_118_ped_ref_idx[saved_count] = pObj->field_26_ped_ref_idx;
            saved_count++;
            if (saved_count >= 20)
            {
                return;
            }
        }
    }
}

// TODO: This is a mess, probably a fake match
// https://decomp.me/scratch/juEUq
MATCH_FUNC(0x52A590)
void Object_5C::RestoreObjects_52A590(SavedObjectData* pData)
{
    u8* pPedRefIdx = &pData->field_118_ped_ref_idx[0];
    u16* pModel = &pData->field_F0_model[0];

    s32 i = 19;
    while (i >= 0)
    {
        if (pData->field_0_obj_x[0][19 - i].field_0_value != kFpZero_6F8E10 &&
            pData->field_0_obj_x[1][19 - i].field_0_value != kFpZero_6F8E10)
        {
            Object* pNewObj = NewPhysicsObj_5299B0(*pModel,
                                                      pData->field_0_obj_x[0][19 - i].field_0_value,
                                                      pData->field_0_obj_x[1][19 - i].field_0_value,
                                                      pData->field_0_obj_x[2][19 - i].field_0_value,
                                                      kZeroAng_6F8F68);
            pNewObj->set_field_26(*pPedRefIdx);
        }
        pModel++;
        i--;
        pPedRefIdx++;
    }

    memset(pData, 0, 0x12Cu); // everything before field_12C_obj_5C_bUnCollectedTokens
}

MATCH_FUNC(0x52A610)
void Object_5C::RemoveAndFree_52A610(Object* pObject)
{
    if (pObject->field_8_definition->field_34_behavior_type != object_behavior_type::light_11)
    {
        gSpriteGrid_3_679210->Remove_477B00(pObject->field_4_sprite);
    }
    gObject_2C_Pool_6F8F80->DeAllocate(pObject);
}

MATCH_FUNC(0x52A650)
void Object::EnsureObject3C_52A650()
{
    if (!field_10_obj_3c)
    {
        Object_3C* pObject3C = gObject_3C_Pool_6F8F7C->Allocate();
        field_10_obj_3c = pObject3C;
        pObject3C->field_20_obj2c_id = field_14_id;
        field_10_obj_3c->field_C_speed = kFpZero_6F8E10;
        field_10_obj_3c->field_10_z_speed = kFpZero_6F8E10;
    }
    Object::PoolGive_522340();
}

MATCH_FUNC(0x52a6d0)
void Object::OnAttachedToSprite_52A6D0(Sprite* pSprite)
{
    RemoveFromCollisionBuckets_527D00();

    if (field_8_definition->field_34_behavior_type != object_behavior_type::light_11)
    {
        gSpriteGrid_3_679210->AddToSingleBucket_477AE0(field_4_sprite);
    }

    PoolTake_522360();

    Car* pCar = pSprite->AsCar_40FEB0();
    if (pCar)
    {
        field_4_sprite->SetDrawLayer_40F7B0(pCar->GetAttachedDrawLayer_4435B0());
    }
}

MATCH_FUNC(0x52ae60)
Object::~Object()
{
    mpNext = NULL;
    field_4_sprite = NULL;
    field_8_definition = NULL;
    field_10_obj_3c = NULL;
}

MATCH_FUNC(0x52ae70)
Fix16_Point Object::GetXY_52AE70() const
{
    return field_4_sprite->get_x_y();
}

MATCH_FUNC(0x52ae90)
Fix16_Point Object::GetSpeedVector_52AE90() const
{
    if (field_10_obj_3c)
    {
        return field_10_obj_3c->GetSpeedVector_52ADF0();
    }
    return gZeroVector_6F8EF0;
}