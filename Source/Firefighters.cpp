#include "Firefighters.hpp"
#include "car_despawn_status.hpp"
#include "Car_BC.hpp"
#include "CarAI_78.hpp"
#include "CarPhysics_B0.hpp"
#include "Char_Pool.hpp"
#include "Game_0x40.hpp"
#include "Hamburger_500.hpp"
#include "Orca_2FD4.hpp"
#include "debug.hpp"

// Forward declarations: the functions below are in address order
EXTERN_GLOBAL(Fix16, dword_67D384);

DEFINE_GLOBAL(FirefighterPool_54*, gFirefighterPool_54_67D4C0, 0x67D4C0);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_67D1F0, Fix16(0.5f), 0x67D1F0);
DEFINE_GLOBAL_INIT(Fix16, kFpZero_67D378, Fix16(0), 0x67D378);

DEFINE_GLOBAL_INIT(Ang16, kAng0_67D4B0, Ang16(0), 0x67D4B0);
DEFINE_GLOBAL_INIT(Ang16, kAng90_67D208, Ang16(360), 0x67D208);
DEFINE_GLOBAL_INIT(Ang16, kAng180_67D2D6, Ang16(720), 0x67D2D6);
DEFINE_GLOBAL_INIT(Ang16, kAng270_67D2FC, Ang16(1080), 0x67D2FC);

// https://decomp.me/scratch/X8G5z mov 0x4(%esp) vs mov (%eax)
MATCH_FUNC(0x4a7fc0)
bool Firefighter_28::sub_4A7FC0()
{
    if (!field_1C_car->field_58_physics)
    {
        Firefighter_28::deinit_4A81A0();
        return 0;
    }

    if (field_1C_car->field_58_physics->get_car_velocity_4211C0() == kFpZero_67D378)
    {
        if (++field_24_next_state_timer >= 1000)
        {
            Firefighter_28::deinit_4A81A0();
            return 0;
        }
    }
    else
    {
        field_24_next_state_timer = 0;
    }

    if (field_1C_car && field_1C_car->field_76_last_seen_timer > 5000)
    {
        field_8_state = firefighter_state::abort_6;
    }
    if (field_1C_car && field_1C_car->field_74_damage > 32000)
    {
        field_8_state = firefighter_state::abort_6;
    }

    if (field_C_target_car)
    {
        if (field_C_target_car->field_88_despawn_status == car_despawn_status::despawned_6 || field_C_target_car->field_88_despawn_status == car_despawn_status::deactivated_7 ||
            field_C_target_car->IsDespawning_4215B0() || field_C_target_car->IsMarkedForDespawn_4214B0())
        {
            if (field_20_ped)
            {
                field_20_ped->SetObjective(objectives_enum::no_obj_0, 9999);
            }
            field_8_state = firefighter_state::abort_6;
        }
    }

    if (field_20_ped && field_20_ped->bHasGameObject_403B70())
    {
        field_8_state = firefighter_state::abort_6;
    }

    if (field_8_state != firefighter_state::drive_to_fire_2)
    {
        if (field_8_state > firefighter_state::drive_to_fire_2 && field_8_state <= firefighter_state::put_out_fire_4)
        {
            if (field_C_target_car)
            {
                if (!field_C_target_car->field_0_qq.FindFirstActiveObject_5A6AD0())
                {
                    field_8_state = firefighter_state::finished_5;
                }
            }
        }
    }
    else
    {
        if (field_C_target_car)
        {
            if (field_C_target_car->field_88_despawn_status != car_despawn_status::despawned_6 && !field_C_target_car->IsDespawning_4215B0())
            {
                if (field_20_ped)
                {
                    if (field_C_target_car->get_x_41E430().ToUInt8() != field_20_ped->field_1DC_objective_target_x.ToUInt8() ||
                        field_C_target_car->get_y_41E440().ToUInt8() != field_20_ped->field_1E0_objective_target_y.ToUInt8())
                    {
                        field_20_ped->SetObjective(objectives_enum::goto_area_in_car_14, 9999);
                        field_20_ped->field_1DC_objective_target_x = Fix16(field_C_target_car->get_x_41E430().ToUInt8());
                        field_20_ped->field_1E0_objective_target_y = Fix16(field_C_target_car->get_y_41E440().ToUInt8());
                        field_20_ped->field_1E4_objective_target_z = Fix16(field_C_target_car->get_z_41E450().ToUInt8());
                    }
                }
            }
        }
    }

    if (field_8_state == firefighter_state::abort_6)
    {
        Firefighter_28::deinit_4A81A0();
    }
    return 1;
}

MATCH_FUNC(0x4a81a0)
void Firefighter_28::deinit_4A81A0()
{
    Car_BC* pCar = this->field_1C_car;
    if (pCar)
    {
        Hamburger_40* pRoute = pCar->field_60;
        if (pRoute)
        {
            gHamburger_500_678E30->FreeEntry_474CC0(pRoute);
            this->field_1C_car->field_60 = 0;
        }
    }
    Ped* pPed = this->field_20_ped;
    if (pPed)
    {
        pPed->SetObjective(objectives_enum::no_obj_0, 9999);
    }

    Car_BC* pCar2 = this->field_C_target_car;
    this->field_8_state = firefighter_state::finished_5;
    if (pCar2)
    {
        pCar2->field_0_qq.CleanupSpriteList_5A7080();
    }
}

MATCH_FUNC(0x4a81f0)
void Firefighter_28::Update_4A81F0()
{
    if (!field_4_bActive)
    {
        return;
    }

    switch (field_8_state)
    {
        case firefighter_state::spawn_truck_1:
            if (field_C_target_car && field_C_target_car->field_88_despawn_status != car_despawn_status::despawned_6 &&
                !field_C_target_car->IsDespawning_4215B0() && gCar_6C_677930->CanAllocateOfType_446930(5))
            {
                field_1C_car = gCar_6C_677930->SpawnCarAtRoadDirection_444CF0(car_model_enum::FIRETRUK, field_10_xpos, field_14_ypos, field_18_zpos);
                if (field_1C_car)
                {
                    field_1C_car->IncrementCarStats_443D70(5);
                    field_1C_car->SetUniNum_421560(4);
                    Car_BC* pCar = field_1C_car;
                    if (!pCar->field_5C_AI)
                    {
                        pCar->field_5C_AI = gCarAI_78_Pool_677CF8->Allocate();
                    }
                    field_1C_car->field_5C_AI->SetCar_453BF0(field_1C_car);

                    field_20_ped = gPedManager_6787BC->AllocatePed_470F30();
                    field_20_ped->SetField238_403920(ped_type::special_ped_4);
                    field_20_ped->set_occupation_403970(ped_ocupation_enum::fireman);
                    field_20_ped->SpawnPedInCar_45C730(field_1C_car);
                    field_20_ped->SetObjective(objectives_enum::goto_area_in_car_14, 9999);
                    field_20_ped->field_1DC_objective_target_x = Fix16(field_C_target_car->get_x_41E430().ToUInt8());
                    field_20_ped->field_1E0_objective_target_y = Fix16(field_C_target_car->get_y_41E440().ToUInt8());
                    field_20_ped->field_1E4_objective_target_z = Fix16(field_C_target_car->get_z_41E450().ToUInt8());
                    field_20_ped->field_21C_bf.b7 = 1;
                    field_1C_car->ActivateEmergencyLights_43C920();
                    field_1C_car->SetupCarPhysicsAndSpriteBinding_43BCA0();
                    field_20_ped = field_1C_car->get_driver_4118B0();
                    field_24_next_state_timer = 0;
                    field_8_state = firefighter_state::drive_to_fire_2;
                    break;
                }

                if (++field_24_next_state_timer >= 50)
                {
                    field_8_state = firefighter_state::abort_6;
                }
                break;
            }
            field_8_state = firefighter_state::abort_6;
            break;

        case firefighter_state::drive_to_fire_2:
            if (!field_1C_car->field_58_physics)
            {
                field_8_state = firefighter_state::abort_6;
            }
            else if (sub_4A7FC0())
            {
                if (Fix16::MaxAbsDistance_42A6B0(field_1C_car->get_x_41E430(),
                                          field_1C_car->get_y_41E440(),
                                          field_C_target_car->get_x_41E430(),
                                          field_C_target_car->get_y_41E440()) < dword_67D384 &&
                    field_24_next_state_timer > 100)
                {
                    field_8_state = firefighter_state::arrived_3;
                }

                if (field_20_ped)
                {
                    switch (field_20_ped->GetObjectiveStatus_450CB0())
                    {
                        case 1:
                            field_8_state = firefighter_state::arrived_3;
                            break;
                        case 2:
                            field_8_state = firefighter_state::abort_6;
                            break;
                    }
                }
                else
                {
                    field_8_state = firefighter_state::abort_6;
                }
            }
            break;

        case firefighter_state::arrived_3:
            if (sub_4A7FC0() && field_20_ped && field_8_state == firefighter_state::arrived_3)
            {
                field_20_ped->SetObjective(objectives_enum::turret_put_out_car_fire_60, 9999);
                field_20_ped->set_field_150_target_objective_car(field_C_target_car);
                field_8_state = firefighter_state::put_out_fire_4;
            }
            break;

        case firefighter_state::put_out_fire_4:
            if (sub_4A7FC0())
            {
                switch (field_20_ped->GetObjectiveStatus_450CB0())
                {
                    case 1:
                        field_20_ped->SetObjective(objectives_enum::no_obj_0, 9999);
                        field_8_state = firefighter_state::finished_5;
                        break;
                    case 2:
                        field_20_ped->SetObjective(objectives_enum::no_obj_0, 9999);
                        field_8_state = firefighter_state::finished_5;
                        break;
                }
            }
            break;

        case firefighter_state::finished_5:
            if (field_1C_car && field_1C_car->field_88_despawn_status != car_despawn_status::despawned_6 && !field_1C_car->IsDespawning_4215B0() &&
                !field_1C_car->IsMarkedForDespawn_4214B0())
            {
                if (field_1C_car->field_54_driver)
                {
                    field_1C_car->field_54_driver->field_21C_bf.b3 = 1;
                }
                field_1C_car->SetUniNum_421560(3);
                field_1C_car->InitCarAIControl_440590();
                field_1C_car->sub_43AF40();
                field_1C_car->DeactivateEmergencyLights_43C9D0();
                if (field_1C_car->field_54_driver)
                {
                    field_1C_car->field_54_driver->set_field_150_target_objective_car(0);
                    field_1C_car->field_54_driver->SetObjective(objectives_enum::no_obj_0, 9999);
                    field_1C_car->field_54_driver->ClearBit11_403A40();
                }
            }
            Reset_4A85E0();
            break;
    }

    if (field_8_state == firefighter_state::abort_6)
    {
        deinit_4A81A0();
    }
}

MATCH_FUNC(0x4a85c0)
void Firefighter_28::init_4A85C0()
{
    Clear_450C10();
}

MATCH_FUNC(0x4a85e0)
void Firefighter_28::Reset_4A85E0()
{
    this->field_C_target_car = 0;
    this->field_8_state = firefighter_state::idle_0;
    this->field_4_bActive = 0;
    this->field_1C_car = 0;
}

MATCH_FUNC(0x4a85f0)
void FirefighterPool_54::FireEnginesService_4A85F0()
{
    Firefighter_28* p = field_0_firefighters;
    if (!bSkip_fire_engines_67D53A)
    {
        for (u32 i = 0; i < 2; i++)
        {
            if (p->field_4_bActive)
            {
                p->Update_4A81F0();
            }
            p++;
        }
    }
}

MATCH_FUNC(0x4a8620)
Firefighter_28* FirefighterPool_54::DispatchFirefighters_4A8620(Car_BC* pCar, Fix16 xpos, Fix16 ypos, Fix16 zpos)
{
    if (bSkip_fire_engines_67D53A)
    {
        return NULL;
    }
    u8 xpos_int = xpos.ToUInt8();
    u8 ypos_int = ypos.ToUInt8();
    u8 zpos_int = (zpos + kFpHalf_67D1F0).ToUInt8();
    if (gOrca_2FD4_6FDEF0->FindNearbyTileMatchingSlopeType_5552B0(1, &xpos_int, &ypos_int, &zpos_int, 0) != 1)
    {
        return NULL;
    }
    s8 sUnk;
    if (pCar->CountConsecutiveArrowBlocks_4410D0(kAng0_67D4B0, &sUnk, xpos_int, ypos_int) < 0 || sUnk <= 1)
    {
        if (pCar->CountConsecutiveArrowBlocks_4410D0(kAng90_67D208, &sUnk, xpos_int, ypos_int) < 0 || sUnk <= 1)
        {
            if (pCar->CountConsecutiveArrowBlocks_4410D0(kAng180_67D2D6, &sUnk, xpos_int, ypos_int) < 0 || sUnk <= 1)
            {
                if (pCar->CountConsecutiveArrowBlocks_4410D0(kAng270_67D2FC, &sUnk, xpos_int, ypos_int) < 0 || sUnk <= 1)
                {
                    return NULL;
                }
            }
        }
    }
    Firefighter_28* pNewFireFighter = FirefighterPool_54::New28_4A8800();
    if (!pNewFireFighter)
    {
        return NULL;
    }
    pNewFireFighter->field_0_id = field_50_count;
    ++field_50_count;
    pNewFireFighter->field_10_xpos = kFpHalf_67D1F0 + Fix16(xpos_int);
    pNewFireFighter->field_14_ypos = kFpHalf_67D1F0 + Fix16(ypos_int);
    pNewFireFighter->field_18_zpos = Fix16(zpos_int);
    pNewFireFighter->field_4_bActive = 1;
    pNewFireFighter->field_8_state = firefighter_state::spawn_truck_1;
    pNewFireFighter->field_24_next_state_timer = 0;
    pNewFireFighter->field_C_target_car = pCar;
    return pNewFireFighter;
}

// https://decomp.me/scratch/ZcdAk
DEFINE_GLOBAL_INIT(Fix16, dword_67D384, Fix16(3), 0x67D384);

MATCH_FUNC(0x4a8800)
Firefighter_28* FirefighterPool_54::New28_4A8800()
{
    Firefighter_28* p = field_0_firefighters;
    for (s16 i = 0; i < 2; i++)
    {
        if (!p->field_4_bActive)
        {
            return p;
        }
        p++;
    }
    return 0;
}

MATCH_FUNC(0x4a8820)
char_type FirefighterPool_54::TryDispatchFirefightersToCar_4A8820(Car_BC* pCar)
{
    if (!pCar)
    {
        return 0;
    }
    const s32 f88 = pCar->field_88_despawn_status;
    if (f88 == car_despawn_status::despawned_6 || f88 == car_despawn_status::deactivated_7 || f88 == car_despawn_status::despawning_5)
    {
        return 0;
    }

    if (pCar->IsTrainModel_403BA0())
    {
        return 0;
    }

    Firefighter_28* pFoundCar =
        DispatchFirefighters_4A8620(pCar, pCar->field_50_car_sprite->GetXPos(), pCar->field_50_car_sprite->GetYPos(), pCar->field_50_car_sprite->GetZPos());

    if (!pFoundCar)
    {
        return 0;
    }
    pFoundCar->field_C_target_car = pCar;
    return 1;
}

MATCH_FUNC(0x4a88d0)
void FirefighterPool_54::ResetCount_4A88D0()
{
    field_50_count = 0;
}