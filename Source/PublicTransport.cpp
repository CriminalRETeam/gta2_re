#include "PublicTransport.hpp"
#include "Car_BC.hpp"
#include "Char_Pool.hpp"
#include "Game_0x40.hpp"
#include "Globals.hpp"
#include "debug.hpp"
#include "error.hpp"
#include "map_0x370.hpp"
#include "rng.hpp"
#include "CarAI_78.hpp"
#include "CarPhysics_B0.hpp"
#include "Object_5C.hpp"
#include "Player.hpp"
#include "PurpleDoom.hpp"

DEFINE_GLOBAL(PublicTransport_181C*, gPublicTransport_181C_6FF1D4, 0x6FF1D4);
DEFINE_GLOBAL(TrainStationList, dword_6FEE68, 0x6FEE68);
DEFINE_GLOBAL(u8, gStationCount_6FF1CC, 0x6FF1CC);
DEFINE_GLOBAL_INIT(Fix16, dword_6FF078, 0, 0x6FF078);
DEFINE_GLOBAL_INIT(Fix16_Point, stru_6FF150, Fix16_Point(Fix16(0), Fix16(0)), 0x6FF150);
DEFINE_GLOBAL(u8, dword_6FF158, 0x6FF158);
DEFINE_GLOBAL(u8, byte_6FF1CD, 0x6FF1CD);
DEFINE_GLOBAL(s32, dword_6FF1D0, 0x6FF1D0);
DEFINE_GLOBAL_INIT(Fix16, dword_6FF07C, Fix16(1), 0x6FF07C);
DEFINE_GLOBAL_INIT(Fix16, dword_6FF04C, Fix16(0x333, 0), 0x6FF04C);

Fix16 dword_6FEEE8 = Fix16(0.5); //DEFINE_GLOBAL_INIT(Fix16, dword_6FEEE8, Fix16(0.5), 0x6FEEE8);
Ang16 word_6FF1BC = Ang16(0); //DEFINE_GLOBAL_INIT(Ang16, word_6FF1BC, Ang16(0), 0x6FF1BC);

// Train spawning: the values aren't known yet
DEFINE_GLOBAL(Fix16, dword_6FEF88, 0x6FEF88); // offset into the stop block
DEFINE_GLOBAL(Fix16, dword_6FF088, 0x6FF088); // wagon spacing along x
DEFINE_GLOBAL(Fix16, dword_6FF080, 0x6FF080); // wagon spacing along y
DEFINE_GLOBAL(Ang16, word_6FEFFE, 0x6FEFFE);
DEFINE_GLOBAL(Ang16, word_6FEF04, 0x6FEF04);
DEFINE_GLOBAL(Ang16, word_6FEFD6, 0x6FEFD6);

Fix16 dword_6FEEE0 = Fix16(0x1333, 0); //DEFINE_GLOBAL_INIT(Fix16, dword_6FEEE0, Fix16(0x1333, 0), 0x6FEEE0);
Fix16 dword_6FEED4 = Fix16(0x666, 0); //DEFINE_GLOBAL_INIT(Fix16, dword_6FEED4, Fix16(0x666, 0), 0x6FEED4);
Fix16 dword_6FEEDC = Fix16(0xCCC, 0); //DEFINE_GLOBAL_INIT(Fix16, dword_6FEEDC, Fix16(0xCCC, 0), 0x6FEEDC);
Fix16 dword_6FEEE4 = Fix16(0x1999, 0); //DEFINE_GLOBAL_INIT(Fix16, dword_6FEEE4, Fix16(0x1999, 0), 0x6FEEE4);

MATCH_FUNC(0x577E20)
char __stdcall HasBlockGreenArrowAtDirection_577E20(s32 direction, gmp_block_info* param_2)
{
    switch (direction)
    {
        case road_direction::up_1:
            return param_2->field_A_arrows >> 2 & 1; // up
        case road_direction::down_2:
            return param_2->field_A_arrows >> 3 & 1; // down
        case road_direction::right_3:
            return param_2->field_A_arrows >> 1 & 1; // right
        case road_direction::left_4:
            return param_2->field_A_arrows & 1; // left
        default:
            return true;
    }
}

MATCH_FUNC(0x577E90)
bool __stdcall sub_577E90(char_type* pChar1, char_type* pChar2)
{
    for (u8 i = 0; i < dword_6FF158; i++)
    {
        if (pChar1[i] != pChar2[i])
        {
            return false;
        }
    }
    return true;
}

MATCH_FUNC(0x577EE0)
gmp_map_zone* __stdcall sub_577EE0(char_type* pChar, u8 case_value)
{
    u32 zone_type;

    switch (case_value)
    {
        case 0:
            zone_type = Railway_Station_Entry_Point_11;
            break;
        case 1:
            zone_type = Railway_Station_Exit_Point_12;
            break;
        case 2:
            zone_type = Railway_Station_Stop_Point_13;
            break;
        default:
            FatalError_4A38C0(1006, "C:\\Splitting\\Gta2\\Source\\pubtrans.cpp", 98); // invalid case
            break;
    }

    gmp_map_zone* pZone = gMap_0x370_6F6268->first_zone_by_type_4DF1D0(zone_type);
    dword_6FF158 = 6;
    while (!sub_577E90((char_type*)&pZone->field_6_name, pChar))
    {
        pZone = gMap_0x370_6F6268->next_zone_4DF770();
        if (!pZone)
        {
            return NULL;
        }
        dword_6FF158 = 6;
    }
    return pZone;
}

WIP_FUNC(0x578030)
void Train_58::ReassignTrainHead_578030()
{
    WIP_IMPLEMENTED;

    if (!bSkip_trains_67D550)
    {
        Car_BC* pFirst = this->field_C_carriages[0];
        this->field_C_carriages[0] = this->field_C_carriages[this->field_43_idx];
        this->field_C_carriages[this->field_43_idx] = pFirst;

        gCar_BC_Pool_67792C->UpdateNextPrev(this->field_C_carriages[0]);

        gCar_BC_Pool_67792C->field_0_pool.sub_420F30(this->field_C_carriages[this->field_43_idx]);

        Car_BC* pTrainCar = this->field_C_carriages[this->field_43_idx];
        if (pTrainCar->field_58_physics) // BUG?
        {
            pTrainCar->AllocCarPhysics_4419E0();
        }

        this->field_C_carriages[this->field_43_idx]->field_58_physics->Init_5637A0();
        if (this->field_C_carriages[0]->field_58_physics) // BUG?
        {
            this->field_C_carriages[0]->AllocCarPhysics_4419E0();
        }

        this->field_C_carriages[0]->field_58_physics->SetSprite_563560(this->field_C_carriages[0]->field_50_car_sprite);
        this->field_C_carriages[0]->field_54_driver = pFirst->field_54_driver;
        
        if (this->field_C_carriages[0]->field_58_physics) // BUG?
        {
            this->field_C_carriages[0]->AllocCarPhysics_4419E0();
        }

        Car_BC* pHead = this->field_C_carriages[0];
        if (pHead->is_driven_by_player())
        {
            pHead->field_58_physics->SetField8C_to_2();
        }
        else
        {
            pHead->field_58_physics->SetField8C_to_1();
        }
        pFirst->field_54_driver = 0;

        // 9.6f: Car_BC::sub_421510
        if (!this->field_C_carriages[0]->field_5C_AI)
        {
            this->field_C_carriages[0]->field_5C_AI = gCarAI_78_Pool_677CF8->Allocate();
        }

        this->field_C_carriages[0]->field_5C_AI->SetCar_453BF0(this->field_C_carriages[0]);

        pFirst->DeAllocateAI_4446E0();

        this->field_C_carriages[0]->sub_421560(this->field_C_carriages[0]->field_54_driver->GetPedType_420B70());
    }
}

MATCH_FUNC(0x578180)
void Train_58::sub_578180()
{
    if (!bSkip_trains_67D550)
    {
        switch (this->field_50_state)
        {
            case 0:
                this->field_50_state = 1;
                break;
            case 1:
                ReassignTrainHead_578030();
                this->field_50_state = 2;
                break;
            case 2:
                this->field_50_state = 3;
                break;
            case 3:
                this->field_50_state = 4;
                break;
            case 4:
                this->field_50_state = 5;
                break;
            default:
                return;
        }
    }
}

MATCH_FUNC(0x5781f0)
void Train_58::sub_5781F0()
{
    if (!bSkip_trains_67D550)
    {
        switch (this->field_50_state)
        {
            case 1:
                this->field_50_state = 0;
                break;
            case 2:
                ReassignTrainHead_578030();
                this->field_50_state = 1;
                break;
            case 3:
                this->field_50_state = 2;
                break;
            case 4:
                this->field_50_state = 3;
                break;
            case 5:
                this->field_50_state = 4;
                break;
            default:
                return;
        }
    }
}

MATCH_FUNC(0x578260)
Train_58::Train_58()
{
    Car_BC** ppCVar1;
    char* pcVar2;
    int iVar3;

    field_8 = 0;
    field_C_carriages[0] = NULL;
    field_44 = 0;
    field_4C_maybe_train_station = 0;
    field_4 = 0;
    field_0 = 0;
    field_2 = 0;
    field_48 = 0;
    field_50_state = 2;
    field_54 = 0;
    field_55 = 0;
    field_56_passenger_count = 0;
    field_57 = 0;
    pcVar2 = field_38;
    ppCVar1 = field_C_carriages + 1;
    iVar3 = 10;
    do
    {
        *ppCVar1 = NULL;
        *pcVar2 = -1;
        ppCVar1++;
        pcVar2++;
        iVar3--;
    } while (iVar3 != 0);
    field_38[10] = -1;
    field_43_idx = 0;
    field_1 = 0;
}

MATCH_FUNC(0x5782c0)
Train_58::~Train_58()
{
    this->field_C_carriages[0] = 0;
    this->field_4C_maybe_train_station = 0;
}

MATCH_FUNC(0x5782d0)
void Train_58::sub_5782D0()
{
    if (!bSkip_trains_67D550)
    {
        if (this->field_C_carriages[0]->field_54_driver)
        {
            this->field_50_state = 3;
        }
        else
        {
            this->field_50_state = 2;
        }
    }
}

MATCH_FUNC(0x578300)
void Train_58::sub_578300()
{
    if (!bSkip_trains_67D550)
    {
        if (this->field_50_state < 2)
        {
            this->field_1 = 1;
            ReassignTrainHead_578030();
        }
        this->field_50_state = 2;
    }
}

MATCH_FUNC(0x578330)
void Train_58::sub_578330()
{
    if (!bSkip_trains_67D550)
    {
        for (s32 i = 0; i < 2; i++)
        {
            if (field_C_carriages[i + 1])
            {
                if (field_C_carriages[i + 1]->GetCarInfoIdx_411940() == car_model_enum::TRAIN)
                {
                    field_C_carriages[i + 1]->sub_43B3D0();
                }
            }
        }
    }
}

MATCH_FUNC(0x578360)
void Train_58::sub_578360()
{
    if (!bSkip_trains_67D550)
    {
        for (s32 i = 0; i < 2; i++)
        {
            if (field_C_carriages[i + 1])
            {
                if (field_C_carriages[i + 1]->GetCarInfoIdx_411940() == car_model_enum::TRAIN)
                {
                    field_C_carriages[i + 1]->sub_43B380();
                }
            }
        }
    }
}

WIP_FUNC(0x578390)
void Train_58::UpdatePassengerAI_578390()
{
    WIP_IMPLEMENTED;

    if (!bSkip_trains_67D550 && !bSkip_dummies_67D4EF && gPedManager_6787BC->field_2 < 50u)
    {
        if (this->field_8 == 2)
        {
            u8 i = 0;
            if (this->field_43_idx)
            {
                do
                {
                    Car_BC** pTrainCar = &this->field_C_carriages[i + 1];
                    if ((*pTrainCar)->GetCarInfoIdx_411940() == car_model_enum::TRAIN)
                    {
                        this->field_56_passenger_count = 1;
                        if (gGame_0x40_67E008->IsSpriteOnScreenForAnyPlayer_4B97E0((*pTrainCar)->field_50_car_sprite, dword_6FF078))
                        {
                            if (this->field_54 <= 0)
                            {
                                u8 gTargetCarDoor_6FF1D8 = stru_6F6784.get_int_4F7AE0(4);
                                u8 remap = (*pTrainCar)->GetRemap();
                                u8 target_door = gTargetCarDoor_6FF1D8;
                                if ((u8)gTargetCarDoor_6FF1D8 < remap)
                                {
                                    u8 door_counter;
                                    do
                                    {
                                        if ((*pTrainCar)->sub_43B140(target_door) && this->field_56_passenger_count > 0)
                                        {
                                            Ped* pNewPed = gPedManager_6787BC->SpawnTrainLeaver_470E30();
                                            pNewPed->field_16C_car = *pTrainCar;
                                            pNewPed->SetObjective(objectives_enum::leave_train_38, 9999);
                                            Ped_List_4* pLink = &pNewPed->field_16C_car->field_4_passengers_list;
                                            pNewPed->set_field_150_target_objective_car(*pTrainCar);
                                            pLink->AddPed_471140(pNewPed);
                                            pNewPed->set_target_car_door_403A70(gTargetCarDoor_6FF1D8);
                                            --this->field_56_passenger_count;
                                        }
                                        ++gTargetCarDoor_6FF1D8;
                                        door_counter = (*pTrainCar)->GetRemap();
                                        target_door = gTargetCarDoor_6FF1D8;
                                    } while ((u8)gTargetCarDoor_6FF1D8 < door_counter);
                                }
                                this->field_54 = 7;
                            }
                        }
                    }
                    ++i;
                } while (i < this->field_43_idx);
            }
        }
        else
        {
            if (gGame_0x40_67E008->IsSpriteOnScreenForAnyPlayer_4B97E0(this->field_C_carriages[0]->field_50_car_sprite, dword_6FF078) &&
                this->field_54 <= 0 && gPublicTransport_181C_6FF1D4->field_1818_stop_getting_off_bus)
            {
                if (this->field_C_carriages[0]->sub_43B140(2))
                {
                    Ped_List_4* pPedList = &this->field_C_carriages[0]->field_4_passengers_list;
                    if (pPedList->IsEmpty_420EA0())
                    {
                        if (this->field_56_passenger_count <= 6 && this->field_0)
                        {
                            this->field_54 = stru_6F6784.get_int_4F7AE0(20) + 40;
                            goto LABEL_32;
                        }
                        else
                        {
                            Ped* pNewPed_1 = gPedManager_6787BC->SpawnTrainLeaver_470E30();
                            pNewPed_1->field_16C_car = this->field_C_carriages[0];
                            this->field_C_carriages[0]->field_4_passengers_list.AddPed_471140(pNewPed_1);
                            pNewPed_1->SetObjective(objectives_enum::leave_train_38, 9999);
                            Car_BC* pTargetCar = this->field_C_carriages[0];
                            pNewPed_1->set_target_car_door_403A70(2);
                            pNewPed_1->set_field_150_target_objective_car(pTargetCar);
                            pNewPed_1->set_occupation_403970(8);
                            if (this->field_0 == 1)
                            {
                                this->field_56_passenger_count--;
                            }
                        }
                    }
                    else
                    {
                        Ped* pRemoved = pPedList->RemoveFirstPed_471320();
                        pRemoved->field_16C_car = this->field_C_carriages[0];
                        pRemoved->SetObjective(objectives_enum::leave_train_38, 9999);
                        Car_BC* pTargetCar_ = this->field_C_carriages[0];
                        pRemoved->set_target_car_door_403A70(2);
                        pRemoved->set_field_150_target_objective_car(pTargetCar_);
                        pRemoved->set_occupation_403970(8);
                        if (this->field_0 == 1)
                        {
                            if (this->field_56_passenger_count > 0)
                            {
                                this->field_56_passenger_count--;
                            }
                        }
                    }
                }

                if (!this->field_0)
                {
                    this->field_54 = stru_6F6784.get_int_4F7AE0(20) + 20;
                }
                else
                {
                    this->field_54 = stru_6F6784.get_int_4F7AE0(20) + 40;
                }
            }
        }
    LABEL_32:
        --this->field_54;
    }
}

MATCH_FUNC(0x578670)
void Train_58::ProcessTrainExplosionChain_578670()
{
    Car_BC* pCars = field_C_carriages[1];
    if (!bSkip_trains_67D550)
    {
        Car_BC* pFirst = field_C_carriages[0];
        if (pFirst->field_74_damage >= 32000 && field_38[10] == -1)
        {
            field_38[10] = 10;
        }
        if (field_38[0] == 1 && pCars->field_74_damage >= 32000)
        {
            field_38[10] = 10;
        }
        if (field_38[10] > 0)
        {
            --field_38[10];
            if (!field_38[10])
            {
                if (pFirst->field_74_damage >= 32000)
                {
                    pCars->AccumulateDamage_43DA90(32000, &stru_6FF150);
                }
                else
                {
                    pFirst->AccumulateDamage_43DA90(32000, &stru_6FF150);
                }
            }
        }

        for (u8 car_idx = 0; car_idx < field_43_idx; pCars++, car_idx++)
        {
            if (field_38[car_idx] == -1 && pCars->field_74_damage >= 32000)
            {
                if (car_idx > 0)
                {
                    *((u8*)&field_C_carriages[10] + car_idx + 3) = 10;
                }
                else if (car_idx < field_43_idx - 1)
                {
                    field_38[car_idx + 1] = 10;
                }
                else if (car_idx == 0)
                {
                    field_38[10] = 10;
                }
                field_38[car_idx] = 0;
            }
            if (field_38[car_idx] > 0)
            {
                --field_38[car_idx];
                if (!field_38[car_idx])
                {
                    field_C_carriages[car_idx + 1]->AccumulateDamage_43DA90(32000, &stru_6FF150);
                    if (car_idx > 0)
                    {
                        *((u8*)&field_C_carriages[10] + car_idx + 3) = 10;
                    }
                    if (car_idx < field_43_idx - 1)
                    {
                        field_38[car_idx + 1] = 10;
                    }
                    if (!car_idx)
                    {
                        field_38[10] = 10;
                    }
                }
            }
        }
    }
}

MATCH_FUNC(0x577f80)
s32 TrainStation_34::GetWagonType_577f80(u8 idx)
{
    if (field_24_train_wagons[0] != 0)
    {
        switch (field_24_train_wagons[idx])
        {
            case 1:
                return car_model_enum::TRAIN;
            case 2:
                return car_model_enum::TRAINFB;
            case 3:
                return car_model_enum::boxcar;
        }
    }
    return car_model_enum::none;
}

MATCH_FUNC(0x577fd0)
TrainStation_34::TrainStation_34()
{
    field_0_station_type = 0;
    field_4_entry_point = NULL;
    field_8_exit_point = NULL;
    field_C_stop_point = NULL;
    field_10_pZone = NULL;
    field_14 = 0;
    field_18 = 0;
    field_1C = 0;
    field_20_next_station = NULL;
    field_2E_wagons_number = 0;
    field_2F = 0;

    for (u8 i = 0; i < 10; i++)
    {
        field_24_train_wagons[i] = 1;
    }
}

MATCH_FUNC(0x578010)
TrainStation_34::~TrainStation_34()
{
    field_18 = 0;
    field_4_entry_point = NULL;
    field_8_exit_point = NULL;
    field_C_stop_point = NULL;
    field_10_pZone = 0;
    field_20_next_station = NULL;
}

MATCH_FUNC(0x578790)
Train_58* PublicTransport_181C::AllocateTrain_578790()
{
    if (bSkip_trains_67D550)
    {
        return 0;
    }

    for (u16 i = 0; i < GTA2_COUNTOF(field_1450_train_array); i++)
    {
        if (!field_1450_train_array[i].field_8)
        {
            return &this->field_1450_train_array[i];
        }
    }
    return 0;
}

MATCH_FUNC(0x5787e0)
TrainStation_34* PublicTransport_181C::AllocateTrainStation_5787E0()
{
    for (u16 i = 0; i < GTA2_COUNTOF(field_0_stations); i++)
    {
        if (!this->field_0_stations[i].field_14)
        {
            return &this->field_0_stations[i];
        }
    }
    return 0;
}

MATCH_FUNC(0x578820)
void TrainStation_34::CalculateWagonCount_578820(u8* a2)
{
    if (!bSkip_trains_67D550)
    {
        memcpy(field_24_train_wagons, a2, sizeof(field_24_train_wagons));

        for (u8 i = 0; i < 10; i++)
        {
            if (field_24_train_wagons[i])
            {
                field_2E_wagons_number++;
            }
        }
    }
}

WIP_FUNC(0x578860)
void PublicTransport_181C::SpawnTrainsFromStations_578860()
{
    if (bSkip_trains_67D550)
    {
        return;
    }

    for (s32 i = 0; i < 10; i++)
    {
        TrainStation_34* pStation = &field_0_stations[i];
        char_type wagons = pStation->field_2E_wagons_number;
        if (pStation->field_0_station_type && wagons)
        {
            switch (pStation->field_0_station_type)
            {
                case 2:
                {
                    if (!pStation->field_10_pZone)
                    {
                        FatalError_4A38C0(Gta2Error::IllegalTrainStationNoPlatformZone, "C:\\Splitting\\Gta2\\Source\\pubtrans.cpp", 734);
                    }
                    if (!pStation->field_4_entry_point)
                    {
                        FatalError_4A38C0(Gta2Error::IllegalTrainStationNoEntryZone,
                                          "C:\\Splitting\\Gta2\\Source\\pubtrans.cpp",
                                          738,
                                          pStation->field_10_pZone->field_1_x,
                                          pStation->field_10_pZone->field_2_y);
                    }
                    if (!pStation->field_8_exit_point)
                    {
                        FatalError_4A38C0(Gta2Error::IllegalTrainStationNoExitZone,
                                          "C:\\Splitting\\Gta2\\Source\\pubtrans.cpp",
                                          740,
                                          pStation->field_10_pZone->field_1_x,
                                          pStation->field_10_pZone->field_2_y);
                    }
                    if (!pStation->field_C_stop_point)
                    {
                        FatalError_4A38C0(Gta2Error::IllegalTrainStationNoStopZone,
                                          "C:\\Splitting\\Gta2\\Source\\pubtrans.cpp",
                                          742,
                                          pStation->field_10_pZone->field_1_x,
                                          pStation->field_10_pZone->field_2_y);
                    }

                    Train_58* pTrain = AllocateTrain_578790();
                    if (!pTrain)
                    {
                        FatalError_4A38C0(Gta2Error::NoMoreTrainSpace, "C:\\Splitting\\Gta2\\Source\\pubtrans.cpp", 746);
                    }
                    pTrain->field_8 = 2;

                    s32 rail_z;
                    gmp_block_info* pBlock = gMap_0x370_6F6268->FindRailwayAtCoord_4E62D0(pStation->field_C_stop_point->field_1_x,
                                                                                          pStation->field_C_stop_point->field_2_y,
                                                                                          rail_z);
                    u8 j;
                    if (HasBlockGreenArrowAtDirection_577E20(4, pBlock))
                    {
                        for (j = 0; j < wagons; j++)
                        {
                            pTrain->field_C_carriages[j + 1] = gCar_6C_677930->SpawnCarAtCorrectZ_Scaled(
                                Fix16(pStation->field_C_stop_point->field_1_x) + dword_6FF088 * Fix16(j + 1),
                                Fix16(pStation->field_C_stop_point->field_2_y) + dword_6FEF88,
                                word_6FEFFE,
                                pStation->GetWagonType_577f80(j),
                                dword_6FF07C);
                            if (!pTrain->field_C_carriages[j + 1])
                            {
                                FatalError_4A38C0(Gta2Error::FailedToCreateCarriage,
                                                  "C:\\Splitting\\Gta2\\Source\\pubtrans.cpp",
                                                  761,
                                                  (Fix16(pStation->field_C_stop_point->field_1_x) + dword_6FF088 * Fix16(j + 1)).ToInt(),
                                                  (Fix16(pStation->field_C_stop_point->field_2_y) + dword_6FEF88).ToInt(),
                                                  rail_z);
                            }
                        }
                        pTrain->field_C_carriages[0] =
                            gCar_6C_677930->SpawnCarAtCorrectZ_Scaled(Fix16(pStation->field_C_stop_point->field_1_x),
                                                                      Fix16(pStation->field_C_stop_point->field_2_y) + dword_6FEF88,
                                                                      word_6FEFFE,
                                                                      car_model_enum::TRAINCAB,
                                                                      dword_6FF07C);
                    }
                    else if (HasBlockGreenArrowAtDirection_577E20(2, pBlock))
                    {
                        for (j = 0; j < wagons; j++)
                        {
                            pTrain->field_C_carriages[j + 1] = gCar_6C_677930->SpawnCarAtCorrectZ_Scaled(
                                Fix16(pStation->field_C_stop_point->field_1_x) + dword_6FEF88,
                                Fix16(pStation->field_C_stop_point->field_2_y) - dword_6FF080 * Fix16(j + 1),
                                word_6FF1BC,
                                pStation->GetWagonType_577f80(j),
                                dword_6FF07C);
                            if (!pTrain->field_C_carriages[j + 1])
                            {
                                FatalError_4A38C0(Gta2Error::FailedToCreateCarriage,
                                                  "C:\\Splitting\\Gta2\\Source\\pubtrans.cpp",
                                                  773,
                                                  (Fix16(pStation->field_C_stop_point->field_1_x) + dword_6FEF88).ToInt(),
                                                  (Fix16(pStation->field_C_stop_point->field_2_y) - dword_6FF080 * Fix16(j + 1)).ToInt(),
                                                  rail_z);
                            }
                        }
                        pTrain->field_C_carriages[0] =
                            gCar_6C_677930->SpawnCarAtCorrectZ_Scaled(Fix16(pStation->field_C_stop_point->field_1_x) + dword_6FEF88,
                                                                      Fix16(pStation->field_C_stop_point->field_2_y),
                                                                      word_6FF1BC,
                                                                      car_model_enum::TRAINCAB,
                                                                      dword_6FF07C);
                    }
                    else if (HasBlockGreenArrowAtDirection_577E20(3, pBlock))
                    {
                        for (j = 0; j < wagons; j++)
                        {
                            pTrain->field_C_carriages[j + 1] = gCar_6C_677930->SpawnCarAtCorrectZ_Scaled(
                                Fix16(pStation->field_C_stop_point->field_1_x) - dword_6FF088 * Fix16(j + 1),
                                Fix16(pStation->field_C_stop_point->field_2_y) + dword_6FEF88,
                                word_6FEFFE,
                                pStation->GetWagonType_577f80(j),
                                dword_6FF07C);
                            if (!pTrain->field_C_carriages[j + 1])
                            {
                                FatalError_4A38C0(Gta2Error::FailedToCreateCarriage,
                                                  "C:\\Splitting\\Gta2\\Source\\pubtrans.cpp",
                                                  785,
                                                  (Fix16(pStation->field_C_stop_point->field_1_x) - dword_6FF088 * Fix16(j + 1)).ToInt(),
                                                  (Fix16(pStation->field_C_stop_point->field_2_y) + dword_6FEF88).ToInt(),
                                                  rail_z);
                            }
                        }
                        pTrain->field_C_carriages[0] =
                            gCar_6C_677930->SpawnCarAtCorrectZ_Scaled(Fix16(pStation->field_C_stop_point->field_1_x),
                                                                      Fix16(pStation->field_C_stop_point->field_2_y) + dword_6FEF88,
                                                                      word_6FEF04,
                                                                      car_model_enum::TRAINCAB,
                                                                      dword_6FF07C);
                    }
                    else if (HasBlockGreenArrowAtDirection_577E20(1, pBlock))
                    {
                        for (j = 0; j < wagons; j++)
                        {
                            pTrain->field_C_carriages[j + 1] = gCar_6C_677930->SpawnCarAtCorrectZ_Scaled(
                                Fix16(pStation->field_C_stop_point->field_1_x) + dword_6FEF88,
                                Fix16(pStation->field_C_stop_point->field_2_y) + dword_6FF080 * Fix16(j + 1),
                                word_6FEFD6,
                                pStation->GetWagonType_577f80(j),
                                dword_6FF07C);
                            if (!pTrain->field_C_carriages[j + 1])
                            {
                                FatalError_4A38C0(Gta2Error::FailedToCreateCarriage,
                                                  "C:\\Splitting\\Gta2\\Source\\pubtrans.cpp",
                                                  796,
                                                  (Fix16(pStation->field_C_stop_point->field_1_x) + dword_6FEF88).ToInt(),
                                                  (Fix16(pStation->field_C_stop_point->field_2_y) + dword_6FF080 * Fix16(j + 1)).ToInt(),
                                                  rail_z);
                            }
                        }
                        pTrain->field_C_carriages[0] =
                            gCar_6C_677930->SpawnCarAtCorrectZ_Scaled(Fix16(pStation->field_C_stop_point->field_1_x) + dword_6FEF88,
                                                                      Fix16(pStation->field_C_stop_point->field_2_y),
                                                                      word_6FEFD6,
                                                                      car_model_enum::TRAINCAB,
                                                                      dword_6FF07C);
                    }
                    else
                    {
                        FatalError_4A38C0(Gta2Error::IllegalBlockForTrainCreation,
                                          "C:\\Splitting\\Gta2\\Source\\pubtrans.cpp",
                                          802,
                                          pStation->field_C_stop_point->field_1_x,
                                          pStation->field_C_stop_point->field_2_y,
                                          rail_z);
                    }

                    pTrain->field_43_idx = wagons;
                    for (j = 0; j < wagons; j++)
                    {
                        gCar_BC_Pool_67792C->field_0_pool.sub_420F30(pTrain->field_C_carriages[j + 1]);
                    }

                    Car_BC* pEngine = pTrain->field_C_carriages[0];
                    if (!pEngine->field_5C_AI)
                    {
                        pEngine->field_5C_AI = gCarAI_78_Pool_677CF8->Allocate();
                    }
                    pTrain->field_C_carriages[0]->field_5C_AI->SetCar_453BF0(pTrain->field_C_carriages[0]);
                    pTrain->field_C_carriages[0]->SpawnDriverPed();
                    pTrain->field_C_carriages[0]->sub_421560(5);
                    Object_2C* pLight = gObject_5C_6F8F84->NewLight_529A40(94, 138, 2, 0xFF8000, 3, 255);
                    pTrain->field_C_carriages[0]->field_0_qq.PushImpactEvent_5A6D00(pLight->field_4, 0, 2, word_6FF1BC);
                    pTrain->field_C_carriages[0]->SetField98To4_475C30();

                    for (j = 0; j < wagons; j++)
                    {
                        pTrain->field_C_carriages[j + 1]->SpawnDriverPed();
                        pTrain->field_C_carriages[j + 1]->sub_421560(5);
                        pTrain->field_C_carriages[j + 1]->SetupCarPhysicsAndSpriteBinding_43BCA0();
                        pTrain->field_C_carriages[j + 1]->sub_426E00();
                        if (pTrain->field_C_carriages[j + 1]->field_84_car_info_idx == car_model_enum::TRAINFB)
                        {
                            pTrain->field_C_carriages[j + 1]->SetField98To4_475C30();
                        }
                    }

                    pTrain->field_44 = 0;
                    pTrain->field_48 = 4;
                    pTrain->field_4C_maybe_train_station = pStation;
                    pTrain->field_56_passenger_count = 6;
                    pTrain->field_57 = pStation->field_2F;
                    pTrain->field_C_carriages[0]->InitCarAIControl_440590();
                    pTrain->field_C_carriages[0]->sub_43AF60();
                    pStation->field_14 = 2;
                    pStation->field_1C = 1;
                    pStation->field_18 = pTrain;
                    break;
                }
            }
        }
    }
}

MATCH_FUNC(0x5793e0)
void PublicTransport_181C::InitStationsLinkedList_5793E0()
{
    if (!bSkip_trains_67D550)
    {
        u16 idx = 0;
        for (; idx < dword_6FEE68.field_194_count - 1; idx++)
        {
            dword_6FEE68.field_0_list[idx]->field_20_next_station = dword_6FEE68.field_0_list[idx + 1];
        }
        dword_6FEE68.field_0_list[idx]->field_20_next_station = dword_6FEE68.field_0_list[0];
    }
}

MATCH_FUNC(0x579440)
void PublicTransport_181C::InitTrainStations_579440()
{
    for (s32 station_idx = 0; station_idx < 100; station_idx++)
    {
        TrainStation_34* pStation = &field_0_stations[station_idx];
        switch (pStation->field_0_station_type)
        {
            case 0:
                break;
            case 2:
                pStation->field_4_entry_point = sub_577EE0((char_type*)&pStation->field_10_pZone->field_6_name, 0);
                pStation->field_8_exit_point = sub_577EE0((char_type*)&pStation->field_10_pZone->field_6_name, 1);
                pStation->field_C_stop_point = sub_577EE0((char_type*)&pStation->field_10_pZone->field_6_name, 2);
                break;
            case 1:
                break;
            default:
                FatalError_4A38C0(0x3EE, "C:\\Splitting\\Gta2\\Source\\pubtrans.cpp", 928);
                break;
        }
    }
}

// https://decomp.me/scratch/kgg76
WIP_FUNC(0x5794b0)
void PublicTransport_181C::SetupTrainAndBusStops_5794B0()
{
    WIP_IMPLEMENTED;
    char Buffer[8];
    byte_6FF1CD = 0;
    dword_6FF1D0 = 0;
    if (!bSkip_trains_67D550)
    {
        for (u8 station_zone_kind = 0; station_zone_kind < 5; station_zone_kind++)
        {
            switch (station_zone_kind)
            {
                case 0:
                    sprintf(Buffer, "trak0");
                    break;
                case 1:
                    sprintf(Buffer, "trak1");
                    break;
                case 2:
                    sprintf(Buffer, "trak2");
                    break;
                case 3:
                    sprintf(Buffer, "trak3");
                    break;
                case 4:
                    sprintf(Buffer, "trak4");
                    break;
                default:
                    break;
            }
            dword_6FEE68.field_0_list[0] = NULL;
            dword_6FEE68.field_0_list[1] = NULL;
            dword_6FEE68.field_0_list[2] = NULL;
            dword_6FEE68.field_0_list[3] = NULL;
            dword_6FEE68.field_0_list[4] = NULL;
            dword_6FEE68.field_194_count = 0;

            gmp_map_zone* i;
            for (i = gMap_0x370_6F6268->first_zone_by_type_4DF1D0(gmp_zone_type_enum::railway_station_platform); i != NULL;
                 i = gMap_0x370_6F6268->next_zone_4DF770())
            {
                *(u8*)&dword_6FF158 = 5; //LOBYTE(dword_6FF158) = 5; part of a object???????
                if (sub_577E90(Buffer, i->field_6_name))
                {
                    TrainStation_34* pStation = gPublicTransport_181C_6FF1D4->AllocateTrainStation_5787E0();
                    pStation->field_0_station_type = 2;
                    pStation->field_14 = 1;
                    pStation->field_1C = 1;
                    pStation->field_10_pZone = i;
                    pStation->field_18 = 0;
                    pStation->field_2F = station_zone_kind;
                    ++gStationCount_6FF1CC;
                    switch (i->field_6_name[(u8)dword_6FF158])
                    {
                        case '0':
                            dword_6FEE68.field_0_list[0] = pStation;
                            break;
                        case '1':
                            dword_6FEE68.field_0_list[1] = pStation;
                            break;
                        case '2':
                            dword_6FEE68.field_0_list[2] = pStation;
                            break;
                        case '3':
                            dword_6FEE68.field_0_list[3] = pStation;
                            break;
                        case '4':
                            dword_6FEE68.field_0_list[4] = pStation;
                            break;
                        default:
                            break;
                    }
                    ++dword_6FEE68.field_194_count;
                }
            }
            if (dword_6FEE68.field_194_count > 0)
            {
                PublicTransport_181C::InitStationsLinkedList_5793E0();
            }
        }
    }

    if (!bSkip_buses_67D558)
    {
        for (gmp_map_zone* pZone = gMap_0x370_6F6268->first_zone_by_type_4DF1D0(gmp_zone_type_enum::bus_stop_pavement); pZone != NULL;)
        {
            TrainStation_34* pBusStop = gPublicTransport_181C_6FF1D4->AllocateTrainStation_5787E0();
            pBusStop->field_0_station_type = 1;
            pBusStop->field_14 = 1;
            pBusStop->field_1C = 1;
            pBusStop->field_10_pZone = pZone;
            pBusStop->field_18 = 0;
            ++gStationCount_6FF1CC;
            pZone = gMap_0x370_6F6268->next_zone_4DF770();

            s32 highest_zpos;

            Fix16 xpos = Fix16(pBusStop->field_10_pZone->field_1_x);
            Fix16 ypos = Fix16(pBusStop->field_10_pZone->field_2_y);

            // called twice?
            gMap_0x370_6F6268->FindHighestBlockForCoord_4E4C30(
                Fix16(pBusStop->field_10_pZone->field_1_x).ToInt(), //pBusStop->field_10_pZone->field_1_x << 14 >> 14,
                Fix16(pBusStop->field_10_pZone->field_2_y).ToInt(), //pBusStop->field_10_pZone->field_2_y << 14 >> 14,
                &highest_zpos);
            gMap_0x370_6F6268->FindHighestBlockForCoord_4E4C30(
                Fix16(pBusStop->field_10_pZone->field_1_x).ToInt(), //pBusStop->field_10_pZone->field_1_x << 14 >> 14,
                Fix16(pBusStop->field_10_pZone->field_2_y).ToInt(), //pBusStop->field_10_pZone->field_2_y << 14 >> 14,
                &highest_zpos);

            for (u8 j = 0; j < 4; j++)
            {
                switch (j)
                {
                    case 0:
                        if (gMap_0x370_6F6268->IsNorthBlockRoadType_433470(xpos.ToInt(), ypos.ToInt(), highest_zpos))
                        {
                            ypos -= dword_6FF07C;
                            j = 4;
                        }
                        break;

                    case 1:
                        if (gMap_0x370_6F6268->IsEastBlockRoadType_4334A0(xpos.ToInt(), ypos.ToInt(), highest_zpos))
                        {
                            xpos += dword_6FF07C;
                            j = 4;
                        }

                        break;
                    case 2:
                        if (gMap_0x370_6F6268->IsSouthBlockRoadType_4334D0(xpos.ToInt(), ypos.ToInt(), highest_zpos))
                        {
                            ypos += dword_6FF07C;
                            j = 4;
                        }

                        break;
                    case 3:
                        if (gMap_0x370_6F6268->IsWestBlockRoadType_433500(xpos.ToInt(), ypos.ToInt(), highest_zpos))
                        {
                            xpos -= dword_6FF07C;
                            j = 4;
                        }

                        break;
                    default:
                        continue;
                }
            }

            gMap_0x370_6F6268->FindHighestBlockForCoord_4E4C30(xpos.ToInt(), ypos.ToInt(), &highest_zpos);
            gObject_5C_6F8F84->NewPhysicsObj_5299B0(objects::bus_stop_marker_129,
                                                    dword_6FEEE8 + xpos,
                                                    dword_6FEEE8 + ypos,
                                                    dword_6FF07C + Fix16(highest_zpos),
                                                    word_6FF1BC);
        }
    }
    PublicTransport_181C::InitTrainStations_579440();
}

MATCH_FUNC(0x5799b0)
TrainStation_34* PublicTransport_181C::GetBusStopOnScreen_5799B0()
{
    if (!bSkip_buses_67D558)
    {
        for (u16 station_idx = 0; station_idx < gStationCount_6FF1CC; station_idx++)
        {
            TrainStation_34* pStation = &field_0_stations[station_idx];
            if (pStation->field_0_station_type == 1)
            {
                if (gGame_0x40_67E008->is_point_on_screen_4B9A80(pStation->field_10_pZone->field_1_x, pStation->field_10_pZone->field_2_y))
                {
                    return pStation;
                }
            }
        }
    }
    return NULL;
}

MATCH_FUNC(0x579a30)
void PublicTransport_181C::sub_579A30(Car_BC* pToFind)
{
    if (!bSkip_buses_67D558)
    {
        Car_BC* pLeadCar = field_17C0_bus.field_C_carriages[0];
        if (pLeadCar)
        {
            if (pToFind == pLeadCar)
            {
                s32 bus_state = field_17C0_bus.field_48;
                if (bus_state)
                {
                    if (bus_state == 14)
                    {
                        field_17C0_bus.field_4 = 10;
                    }
                }
                else if (field_17C0_bus.field_0 != 1 || pLeadCar->GetCarLinearSpeed_43A240() == dword_6FF078)
                {
                    field_17C0_bus.field_48 = 12;
                    field_17C0_bus.field_4 = 10;
                }
            }
        }
    }
}

MATCH_FUNC(0x579aa0)
bool PublicTransport_181C::is_bus_579AA0(Car_BC* pCar)
{
    if (!bSkip_buses_67D558)
    {
        Car_BC* pBus = this->field_17C0_bus.field_C_carriages[0];
        if (pBus)
        {
            if (pCar == pBus)
            {
                return true;
            }
        }
    }

    return false;
}

MATCH_FUNC(0x579ad0)
Car_BC* PublicTransport_181C::sub_579AD0()
{
    if (bSkip_buses_67D558)
    {
        return 0;
    }

    Car_BC* result = this->field_17C0_bus.field_C_carriages[0];
    if (!result || this->field_17C0_bus.field_48 != 13)
    {
        return 0;
    }
    return result;
}

MATCH_FUNC(0x579af0)
bool PublicTransport_181C::is_bus_full_579AF0()
{
    if (bSkip_buses_67D558)
    {
        return false;
    }

    if (field_17C0_bus.field_56_passenger_count >= 10)
    {
        return true;
    }

    return false;
}

MATCH_FUNC(0x579b10)
void PublicTransport_181C::IncrementBusPassengerCount_579B10()
{
    if (!bSkip_buses_67D558)
    {
        field_17C0_bus.field_56_passenger_count++;
    }
}

MATCH_FUNC(0x579b20)
void PublicTransport_181C::KillAllPassengers_579B20()
{
    if (!bSkip_buses_67D558)
    {
        this->field_17C0_bus.field_56_passenger_count = 0;
        field_17C0_bus.field_C_carriages[0]->field_4_passengers_list.KillAllPedsFromList_4715A0();
    }
}

MATCH_FUNC(0x579b40)
Car_BC** PublicTransport_181C::GetCarArrayFromLeadCar_579B40(Car_BC* toFind)
{
    for (u8 i = 0; i < GTA2_COUNTOF(field_1450_train_array); i++)
    {
        Train_58* pIter = &field_1450_train_array[i];
        if (pIter->field_C_carriages[0] == toFind)
        {
            return &pIter->field_C_carriages[1];
        }
    }
    return NULL;
}

MATCH_FUNC(0x579b90)
bool PublicTransport_181C::sub_579B90(Car_BC* pToFind, Fix16* pF16Unk)
{
    if (!bSkip_trains_67D550)
    {
        for (u8 i = 0; i < 10; i++)
        {
            Train_58* pTrain = &field_1450_train_array[i];
            Car_BC* pCar = pTrain->field_C_carriages[0];
            if (pCar == pToFind)
            {
                if (!pCar->Is_engine_status_on_3_4118C0())
                {
                    *pF16Unk = dword_6FF078;
                    if (pTrain->field_50_state != 0 && pTrain->field_50_state != 1)
                    {
                        return true;
                    }
                    return false;
                }

                switch (pTrain->field_50_state)
                {
                    case 0:
                        *pF16Unk = dword_6FEEE0;
                        return false;
                        break;
                    case 1:
                        *pF16Unk = dword_6FEED4;
                        return false;
                        break;
                    case 2:
                        *pF16Unk = dword_6FF078;
                        return true;
                        break;
                    case 3:
                        *pF16Unk = dword_6FEED4;
                        return true;
                        break;
                    case 4:
                        *pF16Unk = dword_6FEEDC;
                        return true;
                        break;
                    case 5:
                        *pF16Unk = dword_6FEEE4;
                        return true;
                        break;
                    default:
                        break;
                }
            }
        }
    }
    return false;
}

// https://decomp.me/scratch/5m4jV
WIP_FUNC(0x579ca0)
void PublicTransport_181C::BusesService_579CA0()
{
    WIP_IMPLEMENTED;
    Car_BC* pBusCar;
    Fix16 xpos;
    Fix16 ypos;

    if (!bSkip_buses_67D558)
    {
        s32 found_z;
        if (dword_6FF1D0 || byte_6FF1CD || !gCar_6C_677930->CanAllocateOfType_446930(1))
        {
            if (--dword_6FF1D0 < 0)
            {
                dword_6FF1D0 = 0;
            }
        }
        else
        {
            TrainStation_34* pBusStop = PublicTransport_181C::GetBusStopOnScreen_5799B0();
            if (pBusStop)
            {
                ypos = Fix16(pBusStop->field_10_pZone->field_2_y);
                xpos = Fix16(pBusStop->field_10_pZone->field_1_x);
                gMap_0x370_6F6268->FindHighestBlockForCoord_4E4C30(xpos.ToInt(), ypos.ToInt(), &found_z);
                s16 v7 = 0;
                u16 v33 = 0;
                do
                {
                    switch (v7)
                    {
                        case 0:
                            if (gMap_0x370_6F6268->IsNorthBlockRoadType_433470(xpos.ToInt(), ypos.ToInt(), found_z))
                            {
                                ypos -= dword_6FEEE8;
                                v33 = 4;
                            }
                            break;
                        case 1:
                            if (gMap_0x370_6F6268->IsEastBlockRoadType_4334A0(xpos.ToInt(), ypos.ToInt(), found_z))
                            {
                                xpos += dword_6FF07C;
                                v33 = 4;
                            }
                            break;
                        case 2:
                            // inlined
                            if (gMap_0x370_6F6268->IsSouthBlockRoadType_4334D0(xpos.ToInt(), ypos.ToInt(), found_z))
                            {
                                ypos += dword_6FF07C;
                                v33 = 4;
                            }
                            break;
                        case 3:
                            // inlined
                            if (gMap_0x370_6F6268->IsWestBlockRoadType_433500(xpos.ToInt(), ypos.ToInt(), found_z))
                            {
                                xpos -= dword_6FEEE8;
                                v33 = 4;
                            }
                            break;
                        default:
                            break;
                    }
                    v7 = ++v33;

                } while (v33 < 4);

                gmp_block_info* HighestBlockForCoord_4E4C30 =
                    gMap_0x370_6F6268->FindHighestBlockForCoord_4E4C30(xpos.ToInt(), ypos.ToInt(), &found_z);
                if (HasBlockGreenArrowAtDirection_577E20(4, HighestBlockForCoord_4E4C30))
                {
                    field_17C0_bus.field_C_carriages[0] =
                        gCar_6C_677930->SpawnBusAtValidRoadPosition_4453E0(xpos, ypos + dword_6FEEE8, 4, car_model_enum::BUS);
                }
                if (HasBlockGreenArrowAtDirection_577E20(2, HighestBlockForCoord_4E4C30))
                {
                    field_17C0_bus.field_C_carriages[0] =
                        gCar_6C_677930->SpawnBusAtValidRoadPosition_4453E0(xpos + dword_6FEEE8, ypos, 2, car_model_enum::BUS);
                }
                if (HasBlockGreenArrowAtDirection_577E20(3, HighestBlockForCoord_4E4C30))
                {
                    field_17C0_bus.field_C_carriages[0] =
                        gCar_6C_677930->SpawnBusAtValidRoadPosition_4453E0(xpos, ypos, 3, car_model_enum::BUS);
                }
                if (HasBlockGreenArrowAtDirection_577E20(1, HighestBlockForCoord_4E4C30))
                {
                    field_17C0_bus.field_C_carriages[0] =
                        gCar_6C_677930->SpawnBusAtValidRoadPosition_4453E0(xpos, ypos, 1, car_model_enum::BUS);
                }
                pBusCar = field_17C0_bus.field_C_carriages[0];
                if (pBusCar)
                {
                    // inline here: pBusCar.6f Car_BC::sub_421510
                    if (!pBusCar->field_5C_AI)
                    {
                        pBusCar->field_5C_AI = gCarAI_78_Pool_677CF8->Allocate();
                    }
                    field_17C0_bus.field_C_carriages[0]->field_5C_AI->SetCar_453BF0(field_17C0_bus.field_C_carriages[0]);
                    field_17C0_bus.field_C_carriages[0]->SpawnDriverPed();
                    field_17C0_bus.field_C_carriages[0]->sub_421560(4);
                    field_17C0_bus.field_C_carriages[0]->InitCarAIControl_440590();
                    field_17C0_bus.field_C_carriages[0]->sub_426E00();

                    byte_6FF1CD = 1;
                    field_17C0_bus.field_48 = 0;
                    field_17C0_bus.field_56_passenger_count = 0;
                    field_17C0_bus.field_C_carriages[0]->IncrementCarStats_443D70(1);
                }
            }
            dword_6FF1D0 = 200;
        }

        pBusCar = field_17C0_bus.field_C_carriages[0];
        if (pBusCar)
        {
            if (!field_17C0_bus.field_0)
            {
                if (pBusCar->is_driven_by_player())
                {
                    field_17C0_bus.field_0 = 1;
                    pBusCar->field_54_driver->field_15C_player->field_2D4_scores.sub_593370(pBusCar);
                }
                else
                {
                    if (pBusCar->field_54_driver)
                    {
                        if (pBusCar->field_54_driver->get_occupation_403980() != ped_ocupation_enum::unknown_2 &&
                            pBusCar->field_54_driver->get_occupation_403980() != ped_ocupation_enum::driver)
                        {
                            field_17C0_bus.field_0 = 1;
                        }
                    }
                }
            }
            else
            {
                if (!field_17C0_bus.field_C_carriages[0]->is_driven_by_player())
                {
                    if (pBusCar->field_54_driver)
                    {
                        if (pBusCar->field_54_driver->get_occupation_403980() == ped_ocupation_enum::angry_armed_robbed_driver_12)
                        {
                            field_17C0_bus.field_0 = 0;
                            field_17C0_bus.field_2 = 0;
                            pBusCar->field_54_driver->set_occupation_403970(ped_ocupation_enum::driver);
                            field_17C0_bus.field_C_carriages[0]->field_54_driver->SetField238_403920(3);
                        }
                    }
                }
                if (!field_17C0_bus.field_2 && !field_1818_stop_getting_off_bus)
                {
                    if (field_17C0_bus.field_C_carriages[0]->GetCarLinearSpeed_43A240() > dword_6FEED4 + dword_6FF04C)
                    {
                        field_17C0_bus.field_2 = 1;
                        field_17C0_bus.field_C_carriages[0]->field_4_passengers_list.ApplyPassengerBusStopBehavior_471630();
                    }
                }
            }

            if (field_17C0_bus.field_C_carriages[0]->Get_F76_4A9AD0() > 200)
            {
                field_17C0_bus.field_C_carriages[0]->sub_421470();
                field_17C0_bus.field_C_carriages[0] = 0;
                byte_6FF1CD = 0;
                dword_6FF1D0 = 100;
            }
            else
            {
                if (field_17C0_bus.field_C_carriages[0]->IsDespawning_4215B0() ||
                    field_17C0_bus.field_C_carriages[0]->field_74_damage == 32000)
                {
                    field_17C0_bus.field_C_carriages[0] = 0;
                    byte_6FF1CD = 0;
                    dword_6FF1D0 = 0;
                }
                else
                {
                    switch (field_17C0_bus.field_48)
                    {
                        case 12:
                            --field_17C0_bus.field_4;
                            field_17C0_bus.field_C_carriages[0]->sub_43AF60();
                            if (!field_17C0_bus.field_4)
                            {
                                field_17C0_bus.field_48 = 5;
                                field_17C0_bus.field_4 = 10;
                            }
                            break;

                        case 5:
                            --field_17C0_bus.field_4;
                            field_17C0_bus.field_C_carriages[0]->sub_43B380();
                            if (!field_17C0_bus.field_4)
                            {
                                field_17C0_bus.field_4 = 100;
                                field_17C0_bus.field_48 = 13;
                            }
                            break;

                        case 13:
                            if (!field_17C0_bus.field_2)
                            {
                                field_17C0_bus.UpdatePassengerAI_578390();
                            }
                            if (!field_17C0_bus.field_0)
                            {
                                if (!--field_17C0_bus.field_4)
                                {
                                    field_17C0_bus.field_48 = 9;
                                    field_17C0_bus.field_4 = 10;
                                }
                            }
                            else if (field_17C0_bus.field_C_carriages[0]->GetCarLinearSpeed_43A240() > dword_6FF078)
                            {
                                field_17C0_bus.field_48 = 9;
                                field_17C0_bus.field_4 = 10;
                            }
                            break;

                        case 9:
                            field_17C0_bus.field_C_carriages[0]->sub_43B3D0();
                            if (!--field_17C0_bus.field_4)
                            {
                                field_17C0_bus.field_48 = 14;
                                field_17C0_bus.field_4 = 10;
                            }
                            break;

                        case 14:
                            field_17C0_bus.field_C_carriages[0]->sub_43AF40();
                            if (!--field_17C0_bus.field_4)
                            {
                                field_17C0_bus.field_48 = 0;
                            }
                            break;
                        default:
                            break;
                    }
                    gPurpleDoom_2_67920C->CheckAndHandleCollisionInStrips_477BD0(field_17C0_bus.field_C_carriages[0]->field_50_car_sprite);
                }
            }
        }
    }
}

// Is the lead carriage on the block of `pZone`?
static inline bool IsTrainAtZone(Train_58* pTrain, gmp_map_zone* pZone)
{
    return (u8)pTrain->field_C_carriages[0]->field_50_car_sprite->field_14_xy.x.ToInt() == pZone->field_1_x &&
        (u8)pTrain->field_C_carriages[0]->field_50_car_sprite->field_14_xy.y.ToInt() == pZone->field_2_y;
}

// Tells the player driving the train which station it is heading for
static inline void SetDriverStation(Train_58* pTrain, TrainStation_34* pStation)
{
    Car_BC* pCar = pTrain->field_C_carriages[0];
    if (pCar)
    {
        Ped* pDriver = pCar->field_54_driver;
        if (pDriver && pTrain->field_43_idx > 0)
        {
            pDriver->SetTrainStation_4AF860(pStation);
        }
    }
}

WIP_FUNC(0x57a7a0)
void PublicTransport_181C::PublicTransportService_57A7A0()
{
    u8 bStopped = 0;
    BusesService_579CA0();
    if (!bSkip_trains_67D550)
    {
        for (u16 i = 0; i < 10; i++)
        {
            Train_58* pTrain = &field_1450_train_array[i];
            if (!pTrain->field_C_carriages[0])
            {
                continue;
            }

            pTrain->ProcessTrainExplosionChain_578670();
            Car_BC* pCar = pTrain->field_C_carriages[0];
            if (pCar->is_driven_by_player())
            {
                if (!pTrain->field_C_carriages[0]->field_54_driver)
                {
                    pCar->sub_421560(1);
                    pTrain->field_0 = 0;
                }
                else
                {
                    pTrain->field_0 = 1;
                    if (pCar->field_54_driver->get_fieldC_45C9B0() > dword_6FF078)
                    {
                        if (pTrain->field_C_carriages[0]->field_54_driver->field_15C_player->field_8B_bWasForwardPressed)
                        {
                            pTrain->sub_578180();
                        }
                    }
                    else if (pTrain->field_C_carriages[0]->field_54_driver->get_fieldC_45C9B0() < dword_6FF078)
                    {
                        if (pTrain->field_C_carriages[0]->field_54_driver->field_15C_player->field_8C_bWasDownPressed)
                        {
                            pTrain->sub_5781F0();
                        }
                    }
                }
            }

            switch (pTrain->field_50_state)
            {
                case 0:
                case 1:
                    bStopped = 1;
                    break;
                case 2:
                    bStopped = 0;
                    break;
                case 3:
                case 4:
                case 5:
                    bStopped = 1;
                    break;
            }

            TrainStation_34* pStation;
            switch (pTrain->field_48)
            {
                case 10:
                    pTrain->sub_578330();
                    pStation = pTrain->field_4C_maybe_train_station;
                    if (!pTrain->field_0)
                    {
                        pTrain->sub_5782D0();
                    }
                    else if (!bStopped)
                    {
                        if (IsTrainAtZone(pTrain, pStation->field_C_stop_point))
                        {
                            pTrain->field_48 = 4;
                            break;
                        }
                        pTrain->field_48 = 11;
                    }
                    if (IsTrainAtZone(pTrain, pStation->field_8_exit_point))
                    {
                        pTrain->field_48 = 0;
                        pTrain->sub_578180();
                        pStation = pTrain->field_4C_maybe_train_station;
                        pStation->field_18 = 0;
                        pStation = pStation->field_20_next_station;
                        pTrain->field_4C_maybe_train_station = pStation;
                        pStation->field_1C = 1;
                    }
                    break;

                case 11:
                    if (bStopped)
                    {
                        pTrain->field_48 = 10;
                    }
                    break;

                case 0:
                    pStation = pTrain->field_4C_maybe_train_station;
                    if (!pTrain->field_0)
                    {
                        if (pStation->field_18)
                        {
                            pTrain->sub_578300();
                            pTrain->field_48 = 1;
                        }
                    }
                    else if (!bStopped)
                    {
                        pTrain->field_48 = 1;
                    }
                    if (!pTrain->field_0)
                    {
                        if (IsTrainAtZone(pTrain, pStation->field_4_entry_point))
                        {
                            SetDriverStation(pTrain, pStation);
                            pTrain->field_48 = 2;
                            pTrain->sub_5781F0();
                        }
                        else if (pTrain->field_50_state != 2 && bStopped)
                        {
                            pTrain->sub_578180();
                        }
                    }
                    break;

                case 1:
                    if (!pTrain->field_0)
                    {
                        if (!pTrain->field_4C_maybe_train_station->field_18)
                        {
                            pTrain->sub_5782D0();
                            pTrain->field_48 = 0;
                            pTrain->sub_578180();
                        }
                    }
                    else if (bStopped)
                    {
                        pTrain->field_48 = 0;
                    }
                    break;

                case 2:
                    pStation = pTrain->field_4C_maybe_train_station;
                    pStation->field_18 = pTrain;
                    if (IsTrainAtZone(pTrain, pStation->field_C_stop_point))
                    {
                        pTrain->field_48 = 4;
                    }
                    else if (pTrain->field_0 == 1 && !bStopped)
                    {
                        pTrain->field_48 = 3;
                    }
                    break;

                case 3:
                    if (pTrain->field_0 == 1 && bStopped)
                    {
                        pTrain->field_48 = 2;
                    }
                    break;

                case 4:
                    if (!pTrain->field_0)
                    {
                        pTrain->sub_578300();
                        pTrain->field_48 = 5;
                        pTrain->field_4 = 10;
                        pTrain->sub_578360();
                    }
                    else if (bStopped)
                    {
                        pTrain->field_48 = 10;
                    }
                    else
                    {
                        pTrain->field_48 = 5;
                        pTrain->sub_578360();
                        pTrain->field_4 = 10;
                    }
                    break;

                case 5:
                    pTrain->field_4--;
                    pTrain->sub_578360();
                    if (pTrain->field_0 == 1 && bStopped)
                    {
                        pTrain->field_48 = 10;
                    }
                    else if (pTrain->field_4 == 0)
                    {
                        pTrain->field_48 = 6;
                        pTrain->field_4 = 10;
                        pTrain->field_4C_maybe_train_station->field_18 = pTrain;
                        pTrain->field_4C_maybe_train_station->field_1C = 4;
                    }
                    break;

                case 6:
                    if (pTrain->field_0 == 1)
                    {
                        if (bStopped)
                        {
                            pTrain->field_48 = 10;
                            break;
                        }
                        pTrain->field_4--;
                        if (pTrain->field_4 == 0)
                        {
                            pTrain->field_4 = 50;
                            pTrain->field_48 = 7;
                            pTrain->field_4C_maybe_train_station->field_1C = 2;
                        }
                    }
                    else
                    {
                        pTrain->field_4--;
                        if (pTrain->field_4 == 0)
                        {
                            pTrain->field_4 = 50;
                            pTrain->field_48 = 7;
                            pTrain->field_4C_maybe_train_station->field_1C = 2;
                        }
                    }
                    break;

                case 7:
                    if (pTrain->field_0 == 1)
                    {
                        if (bStopped)
                        {
                            pTrain->field_48 = 10;
                            break;
                        }
                        pTrain->UpdatePassengerAI_578390();
                        pTrain->field_4--;
                        if (pTrain->field_4 == 0)
                        {
                            pTrain->field_48 = 8;
                            pTrain->field_4 = 50;
                            pTrain->field_4C_maybe_train_station->field_1C = 2;
                        }
                    }
                    else
                    {
                        pTrain->UpdatePassengerAI_578390();
                        pTrain->field_4--;
                        if (pTrain->field_4 == 0)
                        {
                            pTrain->field_48 = 8;
                            pTrain->field_4 = 50;
                            pTrain->field_4C_maybe_train_station->field_1C = 2;
                        }
                    }
                    break;

                case 8:
                    if (pTrain->field_0 == 1)
                    {
                        if (bStopped)
                        {
                            pTrain->field_48 = 10;
                            break;
                        }
                        pTrain->field_4--;
                        if (pTrain->field_4 == 0)
                        {
                            pTrain->field_48 = 9;
                            pTrain->field_4 = 10;
                            pTrain->field_4C_maybe_train_station->field_1C = 3;
                        }
                    }
                    else
                    {
                        pTrain->field_4--;
                        if (pTrain->field_4 == 0)
                        {
                            SetDriverStation(pTrain, pTrain->field_4C_maybe_train_station);
                            pTrain->field_48 = 9;
                            pTrain->field_4 = 10;
                            pTrain->field_4C_maybe_train_station->field_1C = 3;
                        }
                    }
                    break;

                case 9:
                    pTrain->sub_578330();
                    if (pTrain->field_0 == 1)
                    {
                        if (bStopped)
                        {
                            pTrain->field_48 = 10;
                        }
                    }
                    else
                    {
                        pTrain->field_4--;
                        if (pTrain->field_4 == 0)
                        {
                            pTrain->field_48 = 10;
                            pTrain->field_4C_maybe_train_station->field_1C = 1;
                        }
                    }
                    break;
            }
        }
    }
}

MATCH_FUNC(0x57b4b0)
TrainStation_34* PublicTransport_181C::TrainStationForZone_57B4B0(gmp_map_zone* pZone)
{
    TrainStation_34* pIter = &field_0_stations[0];
    for (u16 i = 0; i < GTA2_COUNTOF(field_0_stations); i++)
    {
        if (pIter->field_10_pZone == pZone)
        {
            return pIter;
        }
        pIter++;
    }
    return 0;
}

MATCH_FUNC(0x57b540)
Car_BC* PublicTransport_181C::GetLeadTrainCar_57B540(Car_BC* a2)
{
    if (!bSkip_trains_67D550 && a2->IsTrainModel_403BA0())
    {
        return GetTrainFromCar_57B5C0(a2)->field_C_carriages[0];
    }
    else
    {
        return 0;
    }
}

MATCH_FUNC(0x57b5c0)
Train_58* PublicTransport_181C::GetTrainFromCar_57B5C0(Car_BC* pToFind)
{
    if (!bSkip_trains_67D550)
    {
        for (u8 i = 0; i < 10; i++)
        {
            Train_58* pTrain = &field_1450_train_array[i];
            Car_BC* pWagon = pTrain->field_C_carriages[0];
            if (pWagon == pToFind)
            {
                return pTrain;
            }
            for (u8 wagon_idx = 0; wagon_idx < pTrain->field_43_idx; wagon_idx++)
            {
                pWagon = pTrain->field_C_carriages[wagon_idx + 1];
                if (pWagon == pToFind)
                {
                    return pTrain;
                }
            }
        }
    }
    return NULL;
}

MATCH_FUNC(0x57b6a0)
Train_58* PublicTransport_181C::GetTrainFromCarExcludingLeadCar_57B6A0(Car_BC* pToFind)
{
    if (!bSkip_trains_67D550)
    {
        for (u8 i = 0; i < 10; i++)
        {
            Train_58* pTrain = &field_1450_train_array[i];
            for (u8 wagon_idx = 0; wagon_idx < pTrain->field_43_idx; wagon_idx++)
            {
                Car_BC* pWagon = pTrain->field_C_carriages[wagon_idx + 1];
                if (pWagon == pToFind)
                {
                    return pTrain;
                }
            }
        }
    }
    return NULL;
}

MATCH_FUNC(0x57b740)
bool PublicTransport_181C::AreCarsInDifferentTrains_57B740(Car_BC* pCar1, Car_BC* pCar2)
{
    if (pCar1->IsTrainModel_403BA0() && pCar2->IsTrainModel_403BA0())
    {
        Train_58* pTrain1 = GetTrainFromCar_57B5C0(pCar1);
        Train_58* pTrain2 = GetTrainFromCar_57B5C0(pCar2);
        if (pTrain1->field_57 != pTrain2->field_57)
        {
            return 1;
        }
    }
    return 0;
}

MATCH_FUNC(0x57b7b0)
PublicTransport_181C::PublicTransport_181C()
{
}

MATCH_FUNC(0x57b820)
PublicTransport_181C::~PublicTransport_181C()
{
}