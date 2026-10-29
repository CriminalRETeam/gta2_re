#include "Crushers.hpp"
#include "explosion_type.hpp"
#include "ped_death_cause.hpp"
#include "car_despawn_status.hpp"
#include "Camera.hpp"
#include "Car.hpp"
#include "Globals.hpp"
#include "SoundObject.hpp"
#include "root_sound.hpp"
#include "Object_5C.hpp"
#include "debug.hpp"
#include "map_0x370.hpp"
#include "winmain.hpp"
#include "Game.hpp"
#include "Player.hpp"
#include <stdio.h>

DEFINE_GLOBAL(CrusherPool*, gCrusherPool_94_67A830, 0x67A830);

DEFINE_GLOBAL_INIT(Fix16, kCrusherOpenDist_67A7D0, Fix16(0x3C00, 0), 0x67A7D0);
DEFINE_GLOBAL_INIT(Fix16, kCrusherClosedDist_67A4E4, Fix16(0x2400, 0), 0x67A4E4);
DEFINE_GLOBAL_INIT(Fix16, kCrusherPistonBaseDist_67A664, Fix16(0x4800, 0), 0x67A664);
DEFINE_GLOBAL_INIT(Fix16, kCrusherSpeed_67A5F4, Fix16(0x100, 0), 0x67A5F4);
DEFINE_GLOBAL_INIT(Fix16, kCrusherPistonHalfDepth_67A810, Fix16(0x1C00, 0), 0x67A810);

DEFINE_GLOBAL(Ang16, kAngZero_67A820, 0x67A820);
DEFINE_GLOBAL_INIT(Ang16, kAng270_67A660, Ang16(0x438), 0x67A660);
DEFINE_GLOBAL_INIT(Ang16, kAng180_67A63A, Ang16(0x2D0), 0x67A63A);
DEFINE_GLOBAL_INIT(Ang16, kAng90_67A568, Ang16(0x168), 0x67A568);

MATCH_FUNC(0x4882d0)
Crusher::Crusher()
{
    field_18_sound = NULL;
}

MATCH_FUNC(0x4882e0)
Crusher::~Crusher()
{
    if (field_18_sound)
    {
        gRoot_sound_66B038.DestroySoundObj_40FE60(field_18_sound);
        field_18_sound = NULL;
    }
}

MATCH_FUNC(0x488310)
void Crusher::CrushCar_488310(Car* pCar)
{
    if (field_2C_state == CrusherStates::Idle_0 && !pCar->IsDespawning_4215B0() && !pCar->IsSpriteShrunk_43DC00() && !pCar->IsEngineCrushed_447EB0())
    {
        field_14_pCarBeingCrushed = pCar;
        field_2C_state = CrusherStates::CrushW_1;
        pCar->AddFlags_421890(car_flag::NoCollide);
    }
}

MATCH_FUNC(0x488350)
void Crusher::Service_488350()
{
    if (field_2C_state != CrusherStates::Idle_0)
    {
        if (field_14_pCarBeingCrushed->field_88_despawn_status == car_despawn_status::despawn_pending_2 || field_14_pCarBeingCrushed->field_88_despawn_status == car_despawn_status::marked_for_despawn_4 || field_14_pCarBeingCrushed->field_88_despawn_status == car_despawn_status::despawn_soon_3)
        {
            field_14_pCarBeingCrushed->field_88_despawn_status = car_despawn_status::active_1;
        }

        switch (field_2C_state)
        {
            case CrusherStates::CrushW_1:
                field_1C_w -= kCrusherSpeed_67A5F4;
                if (field_1C_w == kCrusherClosedDist_67A4E4)
                {
                    field_2C_state = CrusherStates::UnCrushW_2;
                    if (!bStartNetworkGame_7081F0)
                    {
                        field_14_pCarBeingCrushed->field_70_exploder_ped_id = gGame_0x40_67E008->field_38_local_player->field_2C4_player_ped->field_200_id;
                        field_14_pCarBeingCrushed->field_90_death_cause = ped_death_cause::explosion_4;
                        field_14_pCarBeingCrushed->field_94_exploder_timer = 50;
                    }
                    field_14_pCarBeingCrushed->HandleCarExplosion_43D840(explosion_type::item_19);
                    field_14_pCarBeingCrushed->field_0_attachments.CleanupSpriteList_5A7080();
                }
                if (field_1C_w - kCrusherPistonHalfDepth_67A810 <= field_14_pCarBeingCrushed->GetCarWidth_48A930() / 2)
                {
                    field_14_pCarBeingCrushed->CarShrinkSprite_43DC80(2, 0);
                }
                break;

            case CrusherStates::UnCrushW_2:
                field_1C_w += kCrusherSpeed_67A5F4;
                if (field_1C_w == kCrusherOpenDist_67A7D0)
                {
                    field_2C_state = CrusherStates::CrushH_3;
                }
                break;

            case CrusherStates::CrushH_3:
            {
                field_20_h -= kCrusherSpeed_67A5F4;
                if (field_20_h == kCrusherClosedDist_67A4E4)
                {
                    field_14_pCarBeingCrushed->field_9C_engine_status = car_engine_status::crushed_7;
                    field_2C_state = CrusherStates::UnCrushH_4;
                }

                if (field_20_h - kCrusherPistonHalfDepth_67A810 <= (field_14_pCarBeingCrushed->GetCarHeight_48A950() / 2))
                {
                    field_14_pCarBeingCrushed->CarShrinkSprite_43DC80(0, 2);
                }
            }
            break;

            case CrusherStates::UnCrushH_4:
                field_20_h += kCrusherSpeed_67A5F4;
                if (field_20_h == kCrusherOpenDist_67A7D0)
                {
                    field_14_pCarBeingCrushed = NULL;
                    field_2C_state = CrusherStates::Idle_0;
                }
                break;

            default:
                break;
        }

        Object* Obj_0 = field_0_piston_top;
        Obj_0->RemoveFromCollisionBuckets_527D00();
        Obj_0->field_4_sprite->set_xy_lazy_447E20(field_24_xpos, field_28_ypos - field_20_h);
        Obj_0->AssignToBucket_527AE0();

        Object* Obj_4 = field_4_piston_right;
        Obj_4->RemoveFromCollisionBuckets_527D00();
        Obj_4->field_4_sprite->set_xy_lazy_447E20(field_24_xpos + field_1C_w, field_28_ypos);
        Obj_4->AssignToBucket_527AE0();

        Object* Obj_8 = field_8_piston_bottom;
        Obj_8->RemoveFromCollisionBuckets_527D00();
        Obj_8->field_4_sprite->set_xy_lazy_447E20(field_24_xpos, field_20_h + field_28_ypos);
        Obj_8->AssignToBucket_527AE0();

        Object* Obj_C = field_C_piston_left;
        Obj_C->RemoveFromCollisionBuckets_527D00();
        Obj_C->field_4_sprite->set_xy_lazy_447E20(field_24_xpos - field_1C_w, field_28_ypos);
        Obj_C->AssignToBucket_527AE0();
    }
}

MATCH_FUNC(0x4885a0)
void Crusher::InitCrusher_4885A0(Fix16 xpos, Fix16 ypos, char_type crusher_idx)
{
    Fix16 zpos = gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(xpos, ypos);

    field_24_xpos = xpos;
    field_28_ypos = ypos;

    field_10_central_spot = gObject_5C_6F8F84->NewPhysicsObj_5299B0(objects::crusher_central_spot_143, xpos, ypos, zpos, kAngZero_67A820);
    field_10_central_spot->field_26_ped_ref_idx = crusher_idx;

    field_0_piston_top = gObject_5C_6F8F84->NewPhysicsObj_5299B0(objects::crusher_piston_142,
                                                                 field_24_xpos,
                                                                 field_28_ypos - kCrusherOpenDist_67A7D0,
                                                                 zpos,
                                                                 kAngZero_67A820);
    field_4_piston_right = gObject_5C_6F8F84->NewPhysicsObj_5299B0(objects::crusher_piston_142,
                                                                   field_24_xpos + kCrusherOpenDist_67A7D0,
                                                                   field_28_ypos,
                                                                   zpos,
                                                                   kAng270_67A660);
    field_8_piston_bottom = gObject_5C_6F8F84->NewPhysicsObj_5299B0(objects::crusher_piston_142,
                                                                    field_24_xpos,
                                                                    kCrusherOpenDist_67A7D0 + field_28_ypos,
                                                                    zpos,
                                                                    kAng180_67A63A);
    field_C_piston_left = gObject_5C_6F8F84->NewPhysicsObj_5299B0(objects::crusher_piston_142,
                                                                  field_24_xpos - kCrusherOpenDist_67A7D0,
                                                                  field_28_ypos,
                                                                  zpos,
                                                                  kAng90_67A568);

    gObject_5C_6F8F84->NewPhysicsObj_5299B0(objects::crusher_piston_base_150,
                                            field_24_xpos,
                                            field_28_ypos - kCrusherPistonBaseDist_67A664,
                                            zpos,
                                            kAngZero_67A820);
    gObject_5C_6F8F84->NewPhysicsObj_5299B0(objects::crusher_piston_base_150,
                                            field_24_xpos + kCrusherPistonBaseDist_67A664,
                                            field_28_ypos,
                                            zpos,
                                            kAng270_67A660);
    gObject_5C_6F8F84->NewPhysicsObj_5299B0(objects::crusher_piston_base_150,
                                            field_24_xpos,
                                            kCrusherPistonBaseDist_67A664 + field_28_ypos,
                                            zpos,
                                            kAng180_67A63A);
    gObject_5C_6F8F84->NewPhysicsObj_5299B0(objects::crusher_piston_base_150,
                                            field_24_xpos - kCrusherPistonBaseDist_67A664,
                                            field_28_ypos,
                                            zpos,
                                            kAng90_67A568);

    field_1C_w = kCrusherOpenDist_67A7D0;
    field_20_h = kCrusherOpenDist_67A7D0;

    field_2C_state = CrusherStates::Idle_0;
    field_14_pCarBeingCrushed = NULL;

    if (!field_18_sound && !bSkip_audio_67D6BE)
    {
        field_18_sound = gRoot_sound_66B038.CreateSoundObject_40EF40(this, SoundObjectTypeEnum::Crusher_30_9);
    }
}

MATCH_FUNC(0x4887a0)
bool CrusherPool::IsCarBeingCrushed_4887A0(Car* pCar)
{
    for (s32 i = 0; i < field_90_count; i++)
    {
        if (field_0_crushers[i].GetCarBeingCrushed() == pCar)
        {
            return true;
        }
    }
    return false;
}

MATCH_FUNC(0x4887d0)
void CrusherPool::CrushCarWithCrusher_4887D0(Car* pCar, u8 crusher_idx)
{
    field_0_crushers[crusher_idx].CrushCar_488310(pCar);
}

MATCH_FUNC(0x4887f0)
void CrusherPool::CrushersService_4887F0()
{
    s32 idx = 0;
    Crusher* pIter = &field_0_crushers[0];
    while (idx < field_90_count)
    {
        pIter->Service_488350();
        idx++;
        pIter++;
    }
}

MATCH_FUNC(0x488820)
Crusher* CrusherPool::CreateCrusher_488820(Fix16 xpos, Fix16 ypos)
{
    Crusher* pNew = &field_0_crushers[field_90_count];
    pNew->InitCrusher_4885A0(xpos, ypos, field_90_count);
    field_90_count++;
    return pNew;
}

MATCH_FUNC(0x488860)
CrusherPool::CrusherPool()
{
    field_90_count = 0;
}

MATCH_FUNC(0x4bbc30)
CrusherPool::~CrusherPool()
{
}
