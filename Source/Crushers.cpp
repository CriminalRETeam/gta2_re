#include "Crushers.hpp"
#include "Camera.hpp"
#include "Car_BC.hpp"
#include "Globals.hpp"
#include "infallible_turing.hpp"
#include "root_sound.hpp"
#include "Object_5C.hpp"
#include "debug.hpp"
#include "map_0x370.hpp"
#include "winmain.hpp"
#include "Game_0x40.hpp"
#include "Player.hpp"
#include <stdio.h>

DEFINE_GLOBAL(CrusherPool_94*, gCrusherPool_94_67A830, 0x67A830);

// TODO: Name and init these to the correct values
DEFINE_GLOBAL(Fix16, kCrusherTargetW_67A7D0, 0x67A7D0);
DEFINE_GLOBAL(Fix16, kCrusherTargetH_67A4E4, 0x67A4E4);
DEFINE_GLOBAL(Fix16, dword_67A664, 0x67A664);
DEFINE_GLOBAL(Fix16, kCrusherSpeed_67A5F4, 0x67A5F4);
DEFINE_GLOBAL(Fix16, kCrusher_67A810, 0x67A810);

DEFINE_GLOBAL(Ang16, word_67A820, 0x67A820);
DEFINE_GLOBAL(Ang16, word_67A660, 0x67A660);
DEFINE_GLOBAL(Ang16, word_67A63A, 0x67A63A);
DEFINE_GLOBAL(Ang16, dword_67A568, 0x67A568);

MATCH_FUNC(0x4882d0)
Crusher_30::Crusher_30()
{
    field_18_sound = NULL;
}

MATCH_FUNC(0x4882e0)
Crusher_30::~Crusher_30()
{
    if (field_18_sound)
    {
        gRoot_sound_66B038.DestroySoundObj_40FE60(field_18_sound);
        field_18_sound = NULL;
    }
}

MATCH_FUNC(0x488310)
void Crusher_30::CrushCar_488310(Car_BC* pCar)
{
    if (field_2C_state == CrusherStates::Idle_0 && pCar->field_88 != 5 && !pCar->sub_43DC00() && pCar->field_9C != 7)
    {
        field_14_pCarBeingCrushed = pCar;
        field_2C_state = CrusherStates::CrushW_1;
        pCar->field_78_flags |= 2u;
    }
}

MATCH_FUNC(0x488350)
void Crusher_30::Service_488350()
{
    if (field_2C_state != CrusherStates::Idle_0)
    {
        if (field_14_pCarBeingCrushed->field_88 == 2 || field_14_pCarBeingCrushed->field_88 == 4 || field_14_pCarBeingCrushed->field_88 == 3)
        {
            field_14_pCarBeingCrushed->field_88 = 1;
        }

        switch (field_2C_state)
        {
            case CrusherStates::CrushW_1:
                field_1C_w -= kCrusherSpeed_67A5F4;
                if (field_1C_w == kCrusherTargetH_67A4E4)
                {
                    field_2C_state = CrusherStates::UnCrushW_2;
                    if (!bStartNetworkGame_7081F0)
                    {
                        field_14_pCarBeingCrushed->field_70_exploder_ped_id = gGame_0x40_67E008->field_38_orf1->field_2C4_player_ped->field_200_id;
                        field_14_pCarBeingCrushed->field_90 = 4;
                        field_14_pCarBeingCrushed->field_94 = 50;
                    }
                    field_14_pCarBeingCrushed->ExplodeCar_Unknown_43D840(19);
                    field_14_pCarBeingCrushed->field_0_qq.sub_5A7080();
                }
                if (field_1C_w - kCrusher_67A810 <= field_14_pCarBeingCrushed->get_car_width() / 2)
                {
                    field_14_pCarBeingCrushed->sub_43DC80(2, 0);
                }
                break;

            case CrusherStates::UnCrushW_2:
                field_1C_w += kCrusherSpeed_67A5F4;
                if (field_1C_w == kCrusherTargetW_67A7D0)
                {
                    field_2C_state = CrusherStates::CrushH_3;
                }
                break;

            case CrusherStates::CrushH_3:
            {
                field_20_h -= kCrusherSpeed_67A5F4;
                if (field_20_h == kCrusherTargetH_67A4E4)
                {
                    field_14_pCarBeingCrushed->field_9C = 7;
                    field_2C_state = CrusherStates::UnCrushH_4;
                }

                if (field_20_h - kCrusher_67A810 <= (field_14_pCarBeingCrushed->get_car_height() / 2))
                {
                    field_14_pCarBeingCrushed->sub_43DC80(0, 2);
                }
            }
            break;

            case CrusherStates::UnCrushH_4:
                field_20_h += kCrusherSpeed_67A5F4;
                if (field_20_h == kCrusherTargetW_67A7D0)
                {
                    field_14_pCarBeingCrushed = NULL;
                    field_2C_state = CrusherStates::Idle_0;
                }
                break;

            default:
                break;
        }

        Object_2C* Obj_0 = field_0;
        Obj_0->RemoveFromCollisionBuckets_527D00();
        Obj_0->field_4->set_xy_lazy_447E20(field_24_xpos, field_28_ypos - field_20_h);
        Obj_0->AssignToBucket_527AE0();

        Object_2C* Obj_4 = field_4;
        Obj_4->RemoveFromCollisionBuckets_527D00();
        Obj_4->field_4->set_xy_lazy_447E20(field_24_xpos + field_1C_w, field_28_ypos);
        Obj_4->AssignToBucket_527AE0();

        Object_2C* Obj_8 = field_8;
        Obj_8->RemoveFromCollisionBuckets_527D00();
        Obj_8->field_4->set_xy_lazy_447E20(field_24_xpos, field_20_h + field_28_ypos);
        Obj_8->AssignToBucket_527AE0();

        Object_2C* Obj_C = field_C;
        Obj_C->RemoveFromCollisionBuckets_527D00();
        Obj_C->field_4->set_xy_lazy_447E20(field_24_xpos - field_1C_w, field_28_ypos);
        Obj_C->AssignToBucket_527AE0();
    }
}

MATCH_FUNC(0x4885a0)
void Crusher_30::InitCrusher_4885A0(Fix16 xpos, Fix16 ypos, char_type crusher_idx)
{
    Fix16 temp_z;
    gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(&temp_z, xpos, ypos);

    field_24_xpos = xpos;
    field_28_ypos = ypos;

    field_10 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(143, xpos, ypos, temp_z, word_67A820);
    field_10->field_26_varrok_idx = crusher_idx;

    field_0 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(142, field_24_xpos, field_28_ypos - kCrusherTargetW_67A7D0, temp_z, word_67A820);
    field_4 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(142, field_24_xpos + kCrusherTargetW_67A7D0, field_28_ypos, temp_z, word_67A660);
    field_8 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(142, field_24_xpos, kCrusherTargetW_67A7D0 + field_28_ypos, temp_z, word_67A63A);
    field_C = gObject_5C_6F8F84->NewPhysicsObj_5299B0(142, field_24_xpos - kCrusherTargetW_67A7D0, field_28_ypos, temp_z, dword_67A568);

    gObject_5C_6F8F84->NewPhysicsObj_5299B0(150, field_24_xpos, field_28_ypos - dword_67A664, temp_z, word_67A820);
    gObject_5C_6F8F84->NewPhysicsObj_5299B0(150, field_24_xpos + dword_67A664, field_28_ypos, temp_z, word_67A660);
    gObject_5C_6F8F84->NewPhysicsObj_5299B0(150, field_24_xpos, dword_67A664 + field_28_ypos, temp_z, word_67A63A);
    gObject_5C_6F8F84->NewPhysicsObj_5299B0(150, field_24_xpos - dword_67A664, field_28_ypos, temp_z, dword_67A568);

    field_1C_w = kCrusherTargetW_67A7D0;
    field_20_h = kCrusherTargetW_67A7D0;

    field_2C_state = CrusherStates::Idle_0;
    field_14_pCarBeingCrushed = NULL;

    if (!field_18_sound && !bSkip_audio_67D6BE)
    {
        field_18_sound = gRoot_sound_66B038.CreateSoundObject_40EF40(this, SoundObjectTypeEnum::Crusher_30_9);
    }
}

MATCH_FUNC(0x4887a0)
bool CrusherPool_94::IsCarBeingCrushed_4887A0(Car_BC* pCar)
{
    for (s32 i = 0; i < field_90_count; i++)
    {
        if (field_0[i].GetCarBeingCrushed() == pCar)
        {
            return true;
        }
    }
    return false;
}

MATCH_FUNC(0x4887d0)
void CrusherPool_94::CrushCarWithCrusher_4887D0(Car_BC* pCar, u8 crusher_idx)
{
    field_0[crusher_idx].CrushCar_488310(pCar);
}

MATCH_FUNC(0x4887f0)
void CrusherPool_94::CrushersService_4887F0()
{
    s32 idx = 0;
    Crusher_30* pIter = &field_0[0];
    while (idx < field_90_count)
    {
        pIter->Service_488350();
        idx++;
        pIter++;
    }
}

MATCH_FUNC(0x488820)
Crusher_30* CrusherPool_94::CreateCrusher_488820(Fix16 xpos, Fix16 ypos)
{
    Crusher_30* pNew = &field_0[field_90_count];
    pNew->InitCrusher_4885A0(xpos, ypos, field_90_count);
    field_90_count++;
    return pNew;
}

MATCH_FUNC(0x488860)
CrusherPool_94::CrusherPool_94()
{
    field_90_count = 0;
}

MATCH_FUNC(0x4bbc30)
CrusherPool_94::~CrusherPool_94()
{
}
