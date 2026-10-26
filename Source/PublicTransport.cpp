#include "PublicTransport.hpp"
#include "Car.hpp"
#include "Char_Pool.hpp"
#include "Game.hpp"
#include "Globals.hpp"
#include "debug.hpp"
#include "error.hpp"
#include "map_0x370.hpp"
#include "rng.hpp"
#include "CarAI.hpp"
#include "CarPhysics.hpp"
#include "Object_5C.hpp"
#include "Player.hpp"
#include "SpriteGrid.hpp"

DEFINE_GLOBAL(PublicTransport*, gPublicTransport_181C_6FF1D4, 0x6FF1D4);
DEFINE_GLOBAL(TrainStationList, gTrainStationList_6FEE68, 0x6FEE68);
DEFINE_GLOBAL(TrainStopCount, gTrainStopCount_6FEFFC, 0x6FEFFC); // name sets a 4-aligned .bss offset: InitStationsLinkedList_5793E0 reads it as a dword
DEFINE_GLOBAL(u8, gStationCount_6FF1CC, 0x6FF1CC);
DEFINE_GLOBAL(u8, gTargetCarDoor_6FF1D8, 0x6FF1D8);
DEFINE_GLOBAL_INIT(Fix16, kFpZero_6FF078, 0, 0x6FF078);
DEFINE_GLOBAL_INIT(Fix16_Point, kZeroPoint_6FF150, Fix16_Point(Fix16(0), Fix16(0)), 0x6FF150);
DEFINE_GLOBAL(u8, gNameCompareLen_6FF158, 0x6FF158);
DEFINE_GLOBAL(u8, gBusSpawned_6FF1CD, 0x6FF1CD);
DEFINE_GLOBAL(s32, gBusSpawnDelay_6FF1D0, 0x6FF1D0);
DEFINE_GLOBAL_INIT(Fix16, kFpOne_6FF07C, Fix16(1), 0x6FF07C);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint05_6FF04C, Fix16(0x333, 0), 0x6FF04C);

DEFINE_GLOBAL_INIT(Fix16, kFpHalf_6FEEE8, Fix16(0.5), 0x6FEEE8);
DEFINE_GLOBAL_INIT(Ang16, kAng0_6FF1BC, Ang16(0), 0x6FF1BC);

// Train spawning values
DEFINE_GLOBAL_INIT(Fix16, kTrainStopBlockOffset_6FEF88, Fix16(0.5), 0x6FEF88);
DEFINE_GLOBAL_INIT(Fix16, kWagonSpacingX_6FF088, Fix16(4), 0x6FF088);
DEFINE_GLOBAL_INIT(Fix16, kWagonSpacingY_6FF080, Fix16(2), 0x6FF080);
DEFINE_GLOBAL_INIT(Ang16, kAng270_6FEFFE, Ang16(1080), 0x6FEFFE);
DEFINE_GLOBAL_INIT(Ang16, kAng90_6FEF04, Ang16(360), 0x6FEF04);
DEFINE_GLOBAL_INIT(Ang16, kAng180_6FEFD6, Ang16(720), 0x6FEFD6);

DEFINE_GLOBAL_INIT(Fix16, kTrainSpeedState0_6FEEE0, Fix16(0x1333, 0), 0x6FEEE0);
DEFINE_GLOBAL_INIT(Fix16, kTrainSpeedState1And3_6FEED4, Fix16(0x666, 0), 0x6FEED4);
DEFINE_GLOBAL_INIT(Fix16, kTrainSpeedState4_6FEEDC, Fix16(0xCCC, 0), 0x6FEEDC);
DEFINE_GLOBAL_INIT(Fix16, kTrainSpeedState5_6FEEE4, Fix16(0x1999, 0), 0x6FEEE4);

MATCH_FUNC(0x577E20)
bool __stdcall HasBlockGreenArrowAtDirection_577E20(s32 direction, const gmp_block_info* pBlock)
{
    switch (direction)
    {
        case road_direction::up_1:
            return pBlock->field_A_arrows >> 2 & 1;
        case road_direction::down_2:
            return pBlock->field_A_arrows >> 3 & 1;
        case road_direction::right_3:
            return pBlock->field_A_arrows >> 1 & 1;
        case road_direction::left_4:
            return pBlock->field_A_arrows & block_arrow::green_left_1;
        default:
            return true;
    }
}

MATCH_FUNC(0x577E90)
bool __stdcall CompareNameChars_577E90(const char_type* pChar1, const char_type* pChar2)
{
    for (u8 i = 0; i < gNameCompareLen_6FF158; i++)
    {
        if (pChar1[i] != pChar2[i])
        {
            return false;
        }
    }
    return true;
}

MATCH_FUNC(0x577EE0)
gmp_map_zone* __stdcall FindStationZoneByName_577EE0(const char_type* pChar, u8 case_value)
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
            FatalError_4A38C0(Gta2Error::InvalidCase, "C:\\Splitting\\Gta2\\Source\\pubtrans.cpp", 98);
            break;
    }

    gmp_map_zone* pZone = gMap_0x370_6F6268->GetFirstZoneOfType_4DF1D0(zone_type);
    gNameCompareLen_6FF158 = 6;
    while (!CompareNameChars_577E90(reinterpret_cast<char_type*>(&pZone->field_6_name), pChar))
    {
        pZone = gMap_0x370_6F6268->GetNextZone_4DF770();
        if (!pZone)
        {
            return NULL;
        }
        gNameCompareLen_6FF158 = 6;
    }
    return pZone;
}

// 9.6f 0x421510 (Car member there), inlined
static inline void EnsureAI_421510(Car* pCar)
{
    if (!pCar->field_5C_AI)
    {
        pCar->field_5C_AI = gCarAI_78_Pool_677CF8->Allocate();
    }
}

MATCH_FUNC(0x577f80)
s32 TrainStation::GetWagonType_577f80(u8 idx) const
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
TrainStation::TrainStation()
{
    field_0_station_type = 0;
    field_4_entry_point = NULL;
    field_8_exit_point = NULL;
    field_C_stop_point = NULL;
    field_10_pZone = NULL;
    field_14_used = 0;
    field_18_pStoppedTrain = NULL;
    field_1C_platform_state = 0;
    field_20_next_station = NULL;
    field_2E_wagons_number = 0;
    field_2F_track_idx = 0;

    for (u8 i = 0; i < 10; i++)
    {
        field_24_train_wagons[i] = 1;
    }
}

MATCH_FUNC(0x578010)
TrainStation::~TrainStation()
{
    field_18_pStoppedTrain = NULL;
    field_4_entry_point = NULL;
    field_8_exit_point = NULL;
    field_C_stop_point = NULL;
    field_10_pZone = NULL;
    field_20_next_station = NULL;
}

MATCH_FUNC(0x578030)
void Train::ReassignTrainHead_578030()
{
    if (!bSkip_trains_67D550)
    {
        Car* pFirst = this->field_C_carriages[0];
        this->field_C_carriages[0] = this->field_C_carriages[this->field_43_wagon_count];
        this->field_C_carriages[this->field_43_wagon_count] = pFirst;

        gCar_BC_Pool_67792C->UpdateNextPrev(this->field_C_carriages[0]);

        gCar_BC_Pool_67792C->field_0_pool.UnlinkFromActiveList_420F30(this->field_C_carriages[this->field_43_wagon_count]);

        // AllocCarPhysics_4419E0 only allocates when there is no physics, so these guarded calls do nothing
        Car* pTrainCar = this->field_C_carriages[this->field_43_wagon_count];
        if (pTrainCar->field_58_physics)
        {
            pTrainCar->AllocCarPhysics_4419E0();
        }

        this->field_C_carriages[this->field_43_wagon_count]->field_58_physics->Init_5637A0();
        if (this->field_C_carriages[0]->field_58_physics)
        {
            this->field_C_carriages[0]->AllocCarPhysics_4419E0();
        }

        this->field_C_carriages[0]->field_58_physics->SetSprite_563560(this->field_C_carriages[0]->field_50_car_sprite);
        this->field_C_carriages[0]->field_54_driver = pFirst->field_54_driver;
        
        if (this->field_C_carriages[0]->field_58_physics)
        {
            this->field_C_carriages[0]->AllocCarPhysics_4419E0();
        }

        Car* pHead = this->field_C_carriages[0];
        if (pHead->IsDrivenByPlayer_4118D0())
        {
            pHead->field_58_physics->SetField8C_to_2();
        }
        else
        {
            pHead->field_58_physics->SetField8C_to_1();
        }
        pFirst->field_54_driver = NULL;

        EnsureAI_421510(this->field_C_carriages[0]);

        this->field_C_carriages[0]->field_5C_AI->SetCar_453BF0(this->field_C_carriages[0]);

        pFirst->DeAllocateAI_4446E0();

        this->field_C_carriages[0]->SetDriverPedType_421560(this->field_C_carriages[0]->field_54_driver->GetPedType_420B70());
    }
}

MATCH_FUNC(0x578180)
void Train::IncrementState_578180()
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
void Train::DecrementState_5781F0()
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
Train::Train()
{
    field_8_alloc_state = 0;
    field_C_carriages[0] = NULL;
    field_44_unused = 0;
    field_4C_station = NULL;
    field_4_wait_timer = 0;
    field_0_is_hijacked = false;
    field_2_passengers_fleeing = false;
    field_48_stop_phase = 0;
    field_50_state = 2;
    field_54_passenger_timer = 0;
    field_55_unused = 0;
    field_56_passenger_count = 0;
    field_57_track_idx = 0;
    char_type* pTimer = field_38_explosion_timers;
    Car** ppCarriage = field_C_carriages + 1;
    s32 remaining = 10;
    do
    {
        *ppCarriage = NULL;
        *pTimer = -1;
        ppCarriage++;
        pTimer++;
        remaining--;
    } while (remaining != 0);
    field_38_explosion_timers[10] = -1;
    field_43_wagon_count = 0;
    field_1_unused = 0;
}

MATCH_FUNC(0x5782c0)
Train::~Train()
{
    this->field_C_carriages[0] = NULL;
    this->field_4C_station = NULL;
}

MATCH_FUNC(0x5782d0)
void Train::StartIfHasDriver_5782D0()
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
void Train::Stop_578300()
{
    if (!bSkip_trains_67D550)
    {
        if (this->field_50_state < 2)
        {
            this->field_1_unused = 1;
            ReassignTrainHead_578030();
        }
        this->field_50_state = 2;
    }
}

MATCH_FUNC(0x578330)
void Train::CloseCarriageDoors_578330()
{
    if (!bSkip_trains_67D550)
    {
        for (s32 i = 0; i < 2; i++)
        {
            if (field_C_carriages[i + 1])
            {
                if (field_C_carriages[i + 1]->GetCarInfoIdx_411940() == car_model_enum::TRAIN)
                {
                    field_C_carriages[i + 1]->CloseAllDoors_43B3D0();
                }
            }
        }
    }
}

MATCH_FUNC(0x578360)
void Train::OpenCarriageDoors_578360()
{
    if (!bSkip_trains_67D550)
    {
        for (s32 i = 0; i < 2; i++)
        {
            if (field_C_carriages[i + 1])
            {
                if (field_C_carriages[i + 1]->GetCarInfoIdx_411940() == car_model_enum::TRAIN)
                {
                    field_C_carriages[i + 1]->OpenAllDoors_43B380();
                }
            }
        }
    }
}

MATCH_FUNC(0x578390)
void Train::UpdatePassengerAI_578390()
{
    if (!bSkip_trains_67D550 && !bSkip_dummies_67D4EF && gPedManager_6787BC->field_2_num_dummy_chars < 50u)
    {
        if (this->field_8_alloc_state == 2)
        {
            for (u8 i = 0; i < this->field_43_wagon_count; i++)
            {
                Car** ppWagon = &this->field_C_carriages[i + 1];
                if ((*ppWagon)->GetCarInfoIdx_411940() == car_model_enum::TRAIN)
                {
                    this->field_56_passenger_count = 1;
                    if (gGame_0x40_67E008->IsSpriteInAnyPlayerBounds_4B97E0((*ppWagon)->field_50_car_sprite, kFpZero_6FF078))
                    {
                        if (this->field_54_passenger_timer <= 0)
                        {
                            for (gTargetCarDoor_6FF1D8 = static_cast<u8>(gRng_6F6784.get_int_4F7AE0(4)); gTargetCarDoor_6FF1D8 < static_cast<u8>((*ppWagon)->GetDoorCount_43B360());
                                 gTargetCarDoor_6FF1D8++)
                            {
                                if ((*ppWagon)->IsStoppedWithPavementAtDoor_43B140(gTargetCarDoor_6FF1D8) && this->field_56_passenger_count > 0)
                                {
                                    Ped* pPassenger = gPedManager_6787BC->SpawnTrainLeaver_470E30();
                                    pPassenger->field_16C_car = *ppWagon;
                                    pPassenger->SetObjective(objectives_enum::leave_train_38);
                                    Ped_List* pLink = &pPassenger->field_16C_car->field_4_passengers_list;
                                    pPassenger->SetTargetObjectiveCar(*ppWagon);
                                    pLink->AddPedToFront_471140(pPassenger);
                                    pPassenger->set_target_car_door_403A70(gTargetCarDoor_6FF1D8);
                                    --this->field_56_passenger_count;
                                }
                            }
                            this->field_54_passenger_timer = 7;
                        }
                    }
                }
            }
            --this->field_54_passenger_timer;
        }
        else
        {
            if (gGame_0x40_67E008->IsSpriteInAnyPlayerBounds_4B97E0(this->field_C_carriages[0]->field_50_car_sprite, kFpZero_6FF078) &&
                this->field_54_passenger_timer <= 0 && !gPublicTransport_181C_6FF1D4->field_1818_stop_getting_off_bus)
            {
                if (this->field_C_carriages[0]->IsStoppedWithPavementAtDoor_43B140(2))
                {
                    if (this->field_C_carriages[0]->field_4_passengers_list.IsEmpty_420EA0())
                    {
                        if (this->field_56_passenger_count > 6 || !this->field_0_is_hijacked)
                        {
                            Ped* pPassenger = gPedManager_6787BC->SpawnTrainLeaver_470E30();
                            pPassenger->field_16C_car = this->field_C_carriages[0];
                            this->field_C_carriages[0]->field_4_passengers_list.AddPedToFront_471140(pPassenger);
                            pPassenger->SetObjective(objectives_enum::leave_train_38);
                            Car* pBus = this->field_C_carriages[0];
                            pPassenger->SetTargetObjectiveCar(pBus);
                            pPassenger->set_target_car_door_403A70(2);
                            pPassenger->set_occupation_403970(ped_ocupation_enum::bus_customer_8);
                            if (this->field_0_is_hijacked == 1)
                            {
                                this->field_56_passenger_count--;
                            }
                        }
                    }
                    else
                    {
                        Ped* pPassenger = this->field_C_carriages[0]->field_4_passengers_list.RemoveFirstPed_471320();
                        pPassenger->field_16C_car = this->field_C_carriages[0];
                        pPassenger->SetObjective(objectives_enum::leave_train_38);
                        Car* pBus = this->field_C_carriages[0];
                        pPassenger->SetTargetObjectiveCar(pBus);
                        pPassenger->set_target_car_door_403A70(2);
                        pPassenger->set_occupation_403970(ped_ocupation_enum::bus_customer_8);
                        if (this->field_0_is_hijacked == 1)
                        {
                            if (this->field_56_passenger_count > 0)
                            {
                                this->field_56_passenger_count--;
                            }
                        }
                    }
                }

                if (!this->field_0_is_hijacked)
                {
                    this->field_54_passenger_timer = gRng_6F6784.get_int_4F7AE0(20) + 20;
                }
                else
                {
                    this->field_54_passenger_timer = gRng_6F6784.get_int_4F7AE0(20) + 40;
                }
            }
            --this->field_54_passenger_timer;
        }
    }
}

MATCH_FUNC(0x578670)
void Train::ProcessTrainExplosionChain_578670()
{
    Car* pWagon = field_C_carriages[1];
    if (!bSkip_trains_67D550)
    {
        Car* pFirst = field_C_carriages[0];
        if (pFirst->field_74_damage >= 32000 && field_38_explosion_timers[10] == -1)
        {
            field_38_explosion_timers[10] = 10;
        }
        if (field_38_explosion_timers[0] == 1 && pWagon->field_74_damage >= 32000)
        {
            field_38_explosion_timers[10] = 10;
        }
        if (field_38_explosion_timers[10] > 0)
        {
            --field_38_explosion_timers[10];
            if (!field_38_explosion_timers[10])
            {
                if (pFirst->field_74_damage >= 32000)
                {
                    pWagon->AccumulateDamage_43DA90(32000, &kZeroPoint_6FF150);
                }
                else
                {
                    pFirst->AccumulateDamage_43DA90(32000, &kZeroPoint_6FF150);
                }
            }
        }

        for (u8 car_idx = 0; car_idx < field_43_wagon_count; pWagon++, car_idx++)
        {
            if (field_38_explosion_timers[car_idx] == -1 && pWagon->field_74_damage >= 32000)
            {
                if (car_idx > 0)
                {
                    field_38_explosion_timers[car_idx - 1] = 10;
                }
                else if (car_idx < field_43_wagon_count - 1)
                {
                    field_38_explosion_timers[car_idx + 1] = 10;
                }
                else if (car_idx == 0)
                {
                    field_38_explosion_timers[10] = 10;
                }
                field_38_explosion_timers[car_idx] = 0;
            }
            if (field_38_explosion_timers[car_idx] > 0)
            {
                --field_38_explosion_timers[car_idx];
                if (!field_38_explosion_timers[car_idx])
                {
                    field_C_carriages[car_idx + 1]->AccumulateDamage_43DA90(32000, &kZeroPoint_6FF150);
                    if (car_idx > 0)
                    {
                        field_38_explosion_timers[car_idx - 1] = 10;
                    }
                    if (car_idx < field_43_wagon_count - 1)
                    {
                        field_38_explosion_timers[car_idx + 1] = 10;
                    }
                    if (!car_idx)
                    {
                        field_38_explosion_timers[10] = 10;
                    }
                }
            }
        }
    }
}

MATCH_FUNC(0x578790)
Train* PublicTransport::AllocateTrain_578790()
{
    if (bSkip_trains_67D550)
    {
        return NULL;
    }

    for (u16 i = 0; i < GTA2_COUNTOF(field_1450_train_array); i++)
    {
        if (!field_1450_train_array[i].field_8_alloc_state)
        {
            return &this->field_1450_train_array[i];
        }
    }
    return NULL;
}

MATCH_FUNC(0x5787e0)
TrainStation* PublicTransport::AllocateTrainStation_5787E0()
{
    for (u16 i = 0; i < GTA2_COUNTOF(field_0_stations); i++)
    {
        if (!this->field_0_stations[i].field_14_used)
        {
            return &this->field_0_stations[i];
        }
    }
    return NULL;
}

MATCH_FUNC(0x578820)
void TrainStation::CalculateWagonCount_578820(const u8* pWagons)
{
    if (!bSkip_trains_67D550)
    {
        memcpy(field_24_train_wagons, pWagons, sizeof(field_24_train_wagons));

        for (u8 i = 0; i < 10; i++)
        {
            if (field_24_train_wagons[i])
            {
                field_2E_wagons_number++;
            }
        }
    }
}

// The wagon spawns pass the "+ half block" coordinate as Fix16::Add_ref of two temporaries (9.6f copies
// kTrainStopBlockOffset_6FEF88 into a temp first): VC6 then builds that argument only after the GetWagonType_577f80 call,
// keeping the u8 coordinate and the copy across it, as the original does. The member operator+ builds it first.
MATCH_FUNC(0x578860)
void PublicTransport::SpawnTrainsFromStations_578860()
{
    if (bSkip_trains_67D550)
    {
        return;
    }

    for (s32 i = 0; i < 10; i++)
    {
        TrainStation* pStation = &field_0_stations[i];
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

                    Train* pTrain = AllocateTrain_578790();
                    if (!pTrain)
                    {
                        FatalError_4A38C0(Gta2Error::NoMoreTrainSpace, "C:\\Splitting\\Gta2\\Source\\pubtrans.cpp", 746);
                    }
                    pTrain->field_8_alloc_state = 2;

                    s32 rail_z;
                    gmp_block_info* pBlock = gMap_0x370_6F6268->FindRailwayAtCoord_4E62D0(pStation->field_C_stop_point->field_1_x,
                                                                                          pStation->field_C_stop_point->field_2_y,
                                                                                          rail_z);
                    u8 j;
                    if (HasBlockGreenArrowAtDirection_577E20(4, pBlock))
                    {
                        for (j = 0; j < wagons; j++)
                        {
                            pTrain->field_C_carriages[j + 1] = gCarManager_6C_677930->SpawnCarAtCorrectZ_Scaled(
                                Fix16(pStation->field_C_stop_point->field_1_x) + Fix16(j + 1) * kWagonSpacingX_6FF088,
                                Fix16::Add_ref(Fix16(pStation->field_C_stop_point->field_2_y), Fix16(kTrainStopBlockOffset_6FEF88)),
                                kAng270_6FEFFE,
                                pStation->GetWagonType_577f80(j),
                                kFpOne_6FF07C);
                            if (!pTrain->field_C_carriages[j + 1])
                            {
                                FatalError_4A38C0(Gta2Error::FailedToCreateCarriage,
                                                  "C:\\Splitting\\Gta2\\Source\\pubtrans.cpp",
                                                  761,
                                                  (Fix16(pStation->field_C_stop_point->field_1_x) + Fix16(j + 1) * kWagonSpacingX_6FF088).ToInt(),
                                                  (Fix16(pStation->field_C_stop_point->field_2_y) + kTrainStopBlockOffset_6FEF88).ToInt(),
                                                  rail_z);
                            }
                        }
                        pTrain->field_C_carriages[0] =
                            gCarManager_6C_677930->SpawnCarAtCorrectZ_Scaled(Fix16(pStation->field_C_stop_point->field_1_x),
                                                                      Fix16(pStation->field_C_stop_point->field_2_y) + kTrainStopBlockOffset_6FEF88,
                                                                      kAng270_6FEFFE,
                                                                      car_model_enum::TRAINCAB,
                                                                      kFpOne_6FF07C);
                    }
                    else if (HasBlockGreenArrowAtDirection_577E20(2, pBlock))
                    {
                        for (j = 0; j < wagons; j++)
                        {
                            pTrain->field_C_carriages[j + 1] = gCarManager_6C_677930->SpawnCarAtCorrectZ_Scaled(
                                Fix16::Add_ref(Fix16(pStation->field_C_stop_point->field_1_x), Fix16(kTrainStopBlockOffset_6FEF88)),
                                Fix16(pStation->field_C_stop_point->field_2_y) - Fix16(j + 1) * kWagonSpacingY_6FF080,
                                kAng0_6FF1BC,
                                pStation->GetWagonType_577f80(j),
                                kFpOne_6FF07C);
                            if (!pTrain->field_C_carriages[j + 1])
                            {
                                FatalError_4A38C0(Gta2Error::FailedToCreateCarriage,
                                                  "C:\\Splitting\\Gta2\\Source\\pubtrans.cpp",
                                                  773,
                                                  (Fix16(pStation->field_C_stop_point->field_1_x) + kTrainStopBlockOffset_6FEF88).ToInt(),
                                                  (Fix16(pStation->field_C_stop_point->field_2_y) - Fix16(j + 1) * kWagonSpacingY_6FF080).ToInt(),
                                                  rail_z);
                            }
                        }
                        pTrain->field_C_carriages[0] =
                            gCarManager_6C_677930->SpawnCarAtCorrectZ_Scaled(Fix16(pStation->field_C_stop_point->field_1_x) + kTrainStopBlockOffset_6FEF88,
                                                                      Fix16(pStation->field_C_stop_point->field_2_y),
                                                                      kAng0_6FF1BC,
                                                                      car_model_enum::TRAINCAB,
                                                                      kFpOne_6FF07C);
                    }
                    else if (HasBlockGreenArrowAtDirection_577E20(3, pBlock))
                    {
                        for (j = 0; j < wagons; j++)
                        {
                            pTrain->field_C_carriages[j + 1] = gCarManager_6C_677930->SpawnCarAtCorrectZ_Scaled(
                                Fix16(pStation->field_C_stop_point->field_1_x) - Fix16(j + 1) * kWagonSpacingX_6FF088,
                                Fix16::Add_ref(Fix16(pStation->field_C_stop_point->field_2_y), Fix16(kTrainStopBlockOffset_6FEF88)),
                                kAng270_6FEFFE,
                                pStation->GetWagonType_577f80(j),
                                kFpOne_6FF07C);
                            if (!pTrain->field_C_carriages[j + 1])
                            {
                                FatalError_4A38C0(Gta2Error::FailedToCreateCarriage,
                                                  "C:\\Splitting\\Gta2\\Source\\pubtrans.cpp",
                                                  785,
                                                  (Fix16(pStation->field_C_stop_point->field_1_x) - Fix16(j + 1) * kWagonSpacingX_6FF088).ToInt(),
                                                  (Fix16(pStation->field_C_stop_point->field_2_y) + kTrainStopBlockOffset_6FEF88).ToInt(),
                                                  rail_z);
                            }
                        }
                        pTrain->field_C_carriages[0] =
                            gCarManager_6C_677930->SpawnCarAtCorrectZ_Scaled(Fix16(pStation->field_C_stop_point->field_1_x),
                                                                      Fix16(pStation->field_C_stop_point->field_2_y) + kTrainStopBlockOffset_6FEF88,
                                                                      kAng90_6FEF04,
                                                                      car_model_enum::TRAINCAB,
                                                                      kFpOne_6FF07C);
                    }
                    else if (HasBlockGreenArrowAtDirection_577E20(1, pBlock))
                    {
                        for (j = 0; j < wagons; j++)
                        {
                            pTrain->field_C_carriages[j + 1] = gCarManager_6C_677930->SpawnCarAtCorrectZ_Scaled(
                                Fix16::Add_ref(Fix16(pStation->field_C_stop_point->field_1_x), Fix16(kTrainStopBlockOffset_6FEF88)),
                                Fix16(pStation->field_C_stop_point->field_2_y) + Fix16(j + 1) * kWagonSpacingY_6FF080,
                                kAng180_6FEFD6,
                                pStation->GetWagonType_577f80(j),
                                kFpOne_6FF07C);
                            if (!pTrain->field_C_carriages[j + 1])
                            {
                                FatalError_4A38C0(Gta2Error::FailedToCreateCarriage,
                                                  "C:\\Splitting\\Gta2\\Source\\pubtrans.cpp",
                                                  796,
                                                  (Fix16(pStation->field_C_stop_point->field_1_x) + kTrainStopBlockOffset_6FEF88).ToInt(),
                                                  (Fix16(pStation->field_C_stop_point->field_2_y) + Fix16(j + 1) * kWagonSpacingY_6FF080).ToInt(),
                                                  rail_z);
                            }
                        }
                        pTrain->field_C_carriages[0] =
                            gCarManager_6C_677930->SpawnCarAtCorrectZ_Scaled(Fix16(pStation->field_C_stop_point->field_1_x) + kTrainStopBlockOffset_6FEF88,
                                                                      Fix16(pStation->field_C_stop_point->field_2_y),
                                                                      kAng180_6FEFD6,
                                                                      car_model_enum::TRAINCAB,
                                                                      kFpOne_6FF07C);
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

                    pTrain->field_43_wagon_count = wagons;
                    for (j = 0; j < wagons; j++)
                    {
                        gCar_BC_Pool_67792C->field_0_pool.UnlinkFromActiveList_420F30(pTrain->field_C_carriages[j + 1]);
                    }

                    EnsureAI_421510(pTrain->field_C_carriages[0]);
                    pTrain->field_C_carriages[0]->field_5C_AI->SetCar_453BF0(pTrain->field_C_carriages[0]);
                    pTrain->field_C_carriages[0]->SpawnDriverPed();
                    pTrain->field_C_carriages[0]->SetDriverPedType_421560(ped_type::script_created_5);
                    Object* pLight = gObject_5C_6F8F84->NewLight_529A40(94, 138, 2, 0xFF8000, 3, 255);
                    pTrain->field_C_carriages[0]->field_0_attachments.PushAttachedSprite_5A6D00(pLight->field_4_sprite, 0, 2, kAng0_6FF1BC);
                    pTrain->field_C_carriages[0]->LockPermanently_475C30();

                    for (j = 0; j < wagons; j++)
                    {
                        pTrain->field_C_carriages[j + 1]->SpawnDriverPed();
                        pTrain->field_C_carriages[j + 1]->SetDriverPedType_421560(ped_type::script_created_5);
                        pTrain->field_C_carriages[j + 1]->SetupCarPhysicsAndSpriteBinding_43BCA0();
                        pTrain->field_C_carriages[j + 1]->StartEngine_426E00();
                        if (pTrain->field_C_carriages[j + 1]->field_84_car_info_idx == car_model_enum::TRAINFB)
                        {
                            pTrain->field_C_carriages[j + 1]->LockPermanently_475C30();
                        }
                    }

                    pTrain->field_44_unused = 0;
                    pTrain->field_48_stop_phase = 4;
                    pTrain->field_4C_station = pStation;
                    pTrain->field_56_passenger_count = 6;
                    pTrain->field_57_track_idx = pStation->field_2F_track_idx;
                    pTrain->field_C_carriages[0]->InitCarAIControl_440590();
                    pTrain->field_C_carriages[0]->HaltAIDriving_43AF60();
                    pStation->field_14_used = 2;
                    pStation->field_1C_platform_state = 1;
                    pStation->field_18_pStoppedTrain = pTrain;
                    break;
                }
            }
        }
    }
}

MATCH_FUNC(0x5793e0)
void PublicTransport::InitStationsLinkedList_5793E0()
{
    if (!bSkip_trains_67D550)
    {
        u16 idx = 0;
        for (; idx < gTrainStopCount_6FEFFC.field_0_count - 1; idx++)
        {
            gTrainStationList_6FEE68.field_0_list[idx]->field_20_next_station = gTrainStationList_6FEE68.field_0_list[idx + 1];
        }
        gTrainStationList_6FEE68.field_0_list[idx]->field_20_next_station = gTrainStationList_6FEE68.field_0_list[0];
    }
}

MATCH_FUNC(0x579440)
void PublicTransport::InitTrainStations_579440()
{
    for (s32 station_idx = 0; station_idx < 100; station_idx++)
    {
        TrainStation* pStation = &field_0_stations[station_idx];
        switch (pStation->field_0_station_type)
        {
            case 0:
                break;
            case 2:
                pStation->field_4_entry_point = FindStationZoneByName_577EE0(reinterpret_cast<char_type*>(&pStation->field_10_pZone->field_6_name), 0);
                pStation->field_8_exit_point = FindStationZoneByName_577EE0(reinterpret_cast<char_type*>(&pStation->field_10_pZone->field_6_name), 1);
                pStation->field_C_stop_point = FindStationZoneByName_577EE0(reinterpret_cast<char_type*>(&pStation->field_10_pZone->field_6_name), 2);
                break;
            case 1:
                break;
            default:
                FatalError_4A38C0(Gta2Error::InvalidCase, "C:\\Splitting\\Gta2\\Source\\pubtrans.cpp", 928);
                break;
        }
    }
}

// https://decomp.me/scratch/kgg76
MATCH_FUNC(0x5794b0)
void PublicTransport::SetupTrainAndBusStops_5794B0()
{
    char_type track_name[8];
    gBusSpawned_6FF1CD = 0;
    gBusSpawnDelay_6FF1D0 = 0;
    if (!bSkip_trains_67D550)
    {
        for (u8 track_idx = 0; track_idx < 5; track_idx++)
        {
            switch (track_idx)
            {
                case 0:
                    sprintf(track_name, "trak0");
                    break;
                case 1:
                    sprintf(track_name, "trak1");
                    break;
                case 2:
                    sprintf(track_name, "trak2");
                    break;
                case 3:
                    sprintf(track_name, "trak3");
                    break;
                case 4:
                    sprintf(track_name, "trak4");
                    break;
                default:
                    break;
            }
            for (s32 k = 0; k < 5; k++)
            {
                gTrainStationList_6FEE68.field_0_list[k] = NULL;
            }
            gTrainStopCount_6FEFFC.field_0_count = 0;

            gmp_map_zone* pPlatform;
            for (pPlatform = gMap_0x370_6F6268->GetFirstZoneOfType_4DF1D0(gmp_zone_type_enum::railway_station_platform); pPlatform != NULL;
                 pPlatform = gMap_0x370_6F6268->GetNextZone_4DF770())
            {
                gNameCompareLen_6FF158 = 5;
                if (CompareNameChars_577E90(track_name, pPlatform->field_6_name))
                {
                    TrainStation* pStation = gPublicTransport_181C_6FF1D4->AllocateTrainStation_5787E0();
                    pStation->field_0_station_type = 2;
                    pStation->field_14_used = 1;
                    pStation->field_1C_platform_state = 1;
                    pStation->field_10_pZone = pPlatform;
                    pStation->field_18_pStoppedTrain = NULL;
                    pStation->field_2F_track_idx = track_idx;
                    ++gStationCount_6FF1CC;
                    switch (pPlatform->field_6_name[static_cast<u8>(gNameCompareLen_6FF158)])
                    {
                        case '0':
                            gTrainStationList_6FEE68.field_0_list[0] = pStation;
                            break;
                        case '1':
                            gTrainStationList_6FEE68.field_0_list[1] = pStation;
                            break;
                        case '2':
                            gTrainStationList_6FEE68.field_0_list[2] = pStation;
                            break;
                        case '3':
                            gTrainStationList_6FEE68.field_0_list[3] = pStation;
                            break;
                        case '4':
                            gTrainStationList_6FEE68.field_0_list[4] = pStation;
                            break;
                        default:
                            break;
                    }
                    ++gTrainStopCount_6FEFFC.field_0_count;
                }
            }
            if (gTrainStopCount_6FEFFC.field_0_count > 0)
            {
                PublicTransport::InitStationsLinkedList_5793E0();
            }
        }
    }

    if (!bSkip_buses_67D558)
    {
        for (gmp_map_zone* pZone = gMap_0x370_6F6268->GetFirstZoneOfType_4DF1D0(gmp_zone_type_enum::bus_stop_pavement); pZone != NULL;)
        {
            TrainStation* pBusStop = gPublicTransport_181C_6FF1D4->AllocateTrainStation_5787E0();
            pBusStop->field_0_station_type = 1;
            pBusStop->field_14_used = 1;
            pBusStop->field_1C_platform_state = 1;
            pBusStop->field_10_pZone = pZone;
            pBusStop->field_18_pStoppedTrain = NULL;
            ++gStationCount_6FF1CC;
            pZone = gMap_0x370_6F6268->GetNextZone_4DF770();

            s32 highest_zpos;

            // The original looks the block up twice
            gMap_0x370_6F6268->FindHighestBlockForCoord_4E4C30(Fix16(pBusStop->field_10_pZone->field_1_x).ToInt(),
                                                               Fix16(pBusStop->field_10_pZone->field_2_y).ToInt(),
                                                               &highest_zpos);

            Fix16 xpos = Fix16(pBusStop->field_10_pZone->field_1_x);
            Fix16 ypos = Fix16(pBusStop->field_10_pZone->field_2_y);
            gMap_0x370_6F6268->FindHighestBlockForCoord_4E4C30(xpos.ToInt(), ypos.ToInt(), &highest_zpos);

            for (u8 j = 0; j < 4; j++)
            {
                switch (j)
                {
                    case 0:
                        if (gMap_0x370_6F6268->IsNorthBlockRoadType_433470(xpos.ToInt(), ypos.ToInt(), highest_zpos))
                        {
                            ypos -= kFpOne_6FF07C;
                            j = 4;
                        }
                        break;

                    case 1:
                        if (gMap_0x370_6F6268->IsBlockRoadTypeInlined_433470(xpos.ToInt() + 1, ypos.ToInt(), highest_zpos))
                        {
                            xpos += kFpOne_6FF07C;
                            j = 4;
                        }

                        break;
                    case 2:
                        if (gMap_0x370_6F6268->IsBlockRoadTypeInlined_433470(xpos.ToInt(), ypos.ToInt() + 1, highest_zpos))
                        {
                            ypos += kFpOne_6FF07C;
                            j = 4;
                        }

                        break;
                    case 3:
                        if (gMap_0x370_6F6268->IsBlockRoadTypeInlined_433470(xpos.ToInt() - 1, ypos.ToInt(), highest_zpos))
                        {
                            xpos -= kFpHalf_6FEEE8; // the original moves only half a block west
                            j = 4;
                        }

                        break;
                    default:
                        continue;
                }
            }

            gMap_0x370_6F6268->FindHighestBlockForCoord_4E4C30(xpos.ToInt(), ypos.ToInt(), &highest_zpos);
            gObject_5C_6F8F84->NewPhysicsObj_5299B0(objects::bus_stop_marker_129,
                                                    kFpHalf_6FEEE8 + xpos,
                                                    kFpHalf_6FEEE8 + ypos,
                                                    kFpOne_6FF07C + Fix16(highest_zpos),
                                                    kAng0_6FF1BC);
        }
    }
    PublicTransport::InitTrainStations_579440();
}

MATCH_FUNC(0x5799b0)
TrainStation* PublicTransport::GetBusStopOnScreen_5799B0()
{
    if (!bSkip_buses_67D558)
    {
        for (u16 station_idx = 0; station_idx < gStationCount_6FF1CC; station_idx++)
        {
            TrainStation* pStation = &field_0_stations[station_idx];
            if (pStation->field_0_station_type == 1)
            {
                if (gGame_0x40_67E008->IsPointOnScreen_4B9A80(pStation->field_10_pZone->field_1_x, pStation->field_10_pZone->field_2_y))
                {
                    return pStation;
                }
            }
        }
    }
    return NULL;
}

MATCH_FUNC(0x579a30)
void PublicTransport::OnBusStopMarkerHit_579A30(Car* pToFind)
{
    if (!bSkip_buses_67D558)
    {
        Car* pLeadCar = field_17C0_bus.field_C_carriages[0];
        if (pLeadCar)
        {
            if (pToFind == pLeadCar)
            {
                s32 bus_state = field_17C0_bus.field_48_stop_phase;
                if (bus_state)
                {
                    if (bus_state == 14)
                    {
                        field_17C0_bus.field_4_wait_timer = 10;
                    }
                }
                else if (field_17C0_bus.field_0_is_hijacked != 1 || pLeadCar->GetCarLinearSpeed_43A240() == kFpZero_6FF078)
                {
                    field_17C0_bus.field_48_stop_phase = 12;
                    field_17C0_bus.field_4_wait_timer = 10;
                }
            }
        }
    }
}

MATCH_FUNC(0x579aa0)
bool PublicTransport::is_bus_579AA0(Car* pCar) const
{
    if (!bSkip_buses_67D558)
    {
        Car* pBus = this->field_17C0_bus.field_C_carriages[0];
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
Car* PublicTransport::GetBusIfBoarding_579AD0() const
{
    if (bSkip_buses_67D558)
    {
        return NULL;
    }

    Car* result = this->field_17C0_bus.field_C_carriages[0];
    if (!result || this->field_17C0_bus.field_48_stop_phase != 13)
    {
        return NULL;
    }
    return result;
}

MATCH_FUNC(0x579af0)
bool PublicTransport::is_bus_full_579AF0() const
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
void PublicTransport::IncrementBusPassengerCount_579B10()
{
    if (!bSkip_buses_67D558)
    {
        field_17C0_bus.field_56_passenger_count++;
    }
}

MATCH_FUNC(0x579b20)
void PublicTransport::KillAllPassengers_579B20()
{
    if (!bSkip_buses_67D558)
    {
        this->field_17C0_bus.field_56_passenger_count = 0;
        field_17C0_bus.field_C_carriages[0]->field_4_passengers_list.KillAndClearAllPeds_4715A0();
    }
}

MATCH_FUNC(0x579b40)
Car** PublicTransport::GetCarArrayFromLeadCar_579B40(Car* toFind)
{
    for (u8 i = 0; i < GTA2_COUNTOF(field_1450_train_array); i++)
    {
        Train* pIter = &field_1450_train_array[i];
        if (pIter->field_C_carriages[0] == toFind)
        {
            return &pIter->field_C_carriages[1];
        }
    }
    return NULL;
}

MATCH_FUNC(0x579b90)
bool PublicTransport::GetTrainSpeed_579B90(Car* pToFind, Fix16* pOutSpeed)
{
    if (!bSkip_trains_67D550)
    {
        for (u8 i = 0; i < 10; i++)
        {
            Train* pTrain = &field_1450_train_array[i];
            Car* pCar = pTrain->field_C_carriages[0];
            if (pCar == pToFind)
            {
                if (!pCar->IsEngineOn_4118C0())
                {
                    *pOutSpeed = kFpZero_6FF078;
                    if (pTrain->field_50_state != 0 && pTrain->field_50_state != 1)
                    {
                        return true;
                    }
                    return false;
                }

                switch (pTrain->field_50_state)
                {
                    case 0:
                        *pOutSpeed = kTrainSpeedState0_6FEEE0;
                        return false;
                    case 1:
                        *pOutSpeed = kTrainSpeedState1And3_6FEED4;
                        return false;
                    case 2:
                        *pOutSpeed = kFpZero_6FF078;
                        return true;
                    case 3:
                        *pOutSpeed = kTrainSpeedState1And3_6FEED4;
                        return true;
                    case 4:
                        *pOutSpeed = kTrainSpeedState4_6FEEDC;
                        return true;
                    case 5:
                        *pOutSpeed = kTrainSpeedState5_6FEEE4;
                        return true;
                    default:
                        break;
                }
            }
        }
    }
    return false;
}

// https://decomp.me/scratch/5m4jV
MATCH_FUNC(0x579ca0)
void PublicTransport::BusesService_579CA0()
{
    Car* pBusCar;

    if (!bSkip_buses_67D558)
    {
        if (gBusSpawnDelay_6FF1D0 || gBusSpawned_6FF1CD || !gCarManager_6C_677930->CanAllocateCarOfKind_446930(car_kind::recycled_1))
        {
            if (--gBusSpawnDelay_6FF1D0 < 0)
            {
                gBusSpawnDelay_6FF1D0 = 0;
            }
        }
        else
        {
            TrainStation* pBusStop = PublicTransport::GetBusStopOnScreen_5799B0();
            if (pBusStop)
            {
                Fix16 xpos = Fix16(pBusStop->field_10_pZone->field_1_x);
                Fix16 ypos = Fix16(pBusStop->field_10_pZone->field_2_y);
                s32 found_z;
                gMap_0x370_6F6268->FindHighestBlockForCoord_4E4C30(xpos.ToInt(), ypos.ToInt(), &found_z);
                for (u16 i = 0; i < 4; i++)
                {
                    switch (i)
                    {
                        case 0:
                            if (gMap_0x370_6F6268->IsNorthBlockRoadType_433470(xpos.ToInt(), ypos.ToInt(), found_z))
                            {
                                ypos -= kFpHalf_6FEEE8;
                                i = 4;
                            }
                            break;
                        case 1:
                            if (gMap_0x370_6F6268->IsEastBlockRoadType_4334A0(xpos.ToInt(), ypos.ToInt(), found_z))
                            {
                                xpos += kFpOne_6FF07C;
                                i = 4;
                            }
                            break;
                        case 2:
                            // inlined
                            if (gMap_0x370_6F6268->IsSouthBlockRoadType_4334D0(xpos.ToInt(), ypos.ToInt(), found_z))
                            {
                                ypos += kFpOne_6FF07C;
                                i = 4;
                            }
                            break;
                        case 3:
                            // inlined
                            if (gMap_0x370_6F6268->IsWestBlockRoadType_433500(xpos.ToInt(), ypos.ToInt(), found_z))
                            {
                                xpos -= kFpHalf_6FEEE8;
                                i = 4;
                            }
                            break;
                        default:
                            break;
                    }
                }

                gmp_block_info* pBlock =
                    gMap_0x370_6F6268->FindHighestBlockForCoord_4E4C30(xpos.ToInt(), ypos.ToInt(), &found_z);
                if (HasBlockGreenArrowAtDirection_577E20(4, pBlock))
                {
                    field_17C0_bus.field_C_carriages[0] =
                        gCarManager_6C_677930->SpawnBusAtValidRoadPosition_4453E0(xpos, ypos + kFpHalf_6FEEE8, 4, car_model_enum::BUS);
                }
                if (HasBlockGreenArrowAtDirection_577E20(2, pBlock))
                {
                    field_17C0_bus.field_C_carriages[0] =
                        gCarManager_6C_677930->SpawnBusAtValidRoadPosition_4453E0(xpos + kFpHalf_6FEEE8, ypos, 2, car_model_enum::BUS);
                }
                if (HasBlockGreenArrowAtDirection_577E20(3, pBlock))
                {
                    field_17C0_bus.field_C_carriages[0] =
                        gCarManager_6C_677930->SpawnBusAtValidRoadPosition_4453E0(xpos, ypos, 3, car_model_enum::BUS);
                }
                if (HasBlockGreenArrowAtDirection_577E20(1, pBlock))
                {
                    field_17C0_bus.field_C_carriages[0] =
                        gCarManager_6C_677930->SpawnBusAtValidRoadPosition_4453E0(xpos, ypos, 1, car_model_enum::BUS);
                }
                pBusCar = field_17C0_bus.field_C_carriages[0];
                if (pBusCar)
                {
                    EnsureAI_421510(pBusCar);
                    field_17C0_bus.field_C_carriages[0]->field_5C_AI->SetCar_453BF0(field_17C0_bus.field_C_carriages[0]);
                    field_17C0_bus.field_C_carriages[0]->SpawnDriverPed();
                    field_17C0_bus.field_C_carriages[0]->SetDriverPedType_421560(ped_type::special_ped_4);
                    field_17C0_bus.field_C_carriages[0]->InitCarAIControl_440590();
                    field_17C0_bus.field_C_carriages[0]->StartEngine_426E00();

                    gBusSpawned_6FF1CD = 1;
                    field_17C0_bus.field_48_stop_phase = 0;
                    field_17C0_bus.field_56_passenger_count = 0;
                    field_17C0_bus.field_C_carriages[0]->IncrementCarStats_443D70(car_kind::recycled_1);
                }
            }
            gBusSpawnDelay_6FF1D0 = 200;
        }

        pBusCar = field_17C0_bus.field_C_carriages[0];
        if (pBusCar)
        {
            if (!field_17C0_bus.field_0_is_hijacked)
            {
                if (field_17C0_bus.field_C_carriages[0]->IsDrivenByPlayer_4118D0())
                {
                    field_17C0_bus.field_0_is_hijacked = true;
                    pBusCar->field_54_driver->field_15C_player->field_2D4_scores.AwardBusStolenScore_593370(pBusCar);
                }
                else
                {
                    if (pBusCar->field_54_driver)
                    {
                        if (pBusCar->field_54_driver->get_occupation_403980() != ped_ocupation_enum::dummy_driver_4 &&
                            pBusCar->field_54_driver->get_occupation_403980() != ped_ocupation_enum::driver)
                        {
                            field_17C0_bus.field_0_is_hijacked = true;
                        }
                    }
                }
            }
            else
            {
                if (!field_17C0_bus.field_C_carriages[0]->IsDrivenByPlayer_4118D0())
                {
                    if (pBusCar->field_54_driver)
                    {
                        if (pBusCar->field_54_driver->get_occupation_403980() == ped_ocupation_enum::angry_armed_robbed_driver_12)
                        {
                            field_17C0_bus.field_0_is_hijacked = false;
                            field_17C0_bus.field_2_passengers_fleeing = false;
                            pBusCar->field_54_driver->set_occupation_403970(ped_ocupation_enum::driver);
                            field_17C0_bus.field_C_carriages[0]->field_54_driver->SetPedType_403920(ped_type::dummy_3);
                        }
                    }
                }
                if (!field_17C0_bus.field_2_passengers_fleeing && !field_1818_stop_getting_off_bus)
                {
                    if (field_17C0_bus.field_C_carriages[0]->GetCarLinearSpeed_43A240() > kTrainSpeedState1And3_6FEED4 + kFpPoint05_6FF04C)
                    {
                        field_17C0_bus.field_2_passengers_fleeing = true;
                        field_17C0_bus.field_C_carriages[0]->field_4_passengers_list.ApplyPassengerBusStopBehavior_471630();
                    }
                }
            }

            if (field_17C0_bus.field_C_carriages[0]->GetLastSeenTimer_4A9AD0() > 200)
            {
                field_17C0_bus.field_C_carriages[0]->MarkForDespawn_421470();
                field_17C0_bus.field_C_carriages[0] = NULL;
                gBusSpawned_6FF1CD = 0;
                gBusSpawnDelay_6FF1D0 = 100;
            }
            else
            {
                if (field_17C0_bus.field_C_carriages[0]->IsDespawning_4215B0() ||
                    field_17C0_bus.field_C_carriages[0]->field_74_damage == 32000)
                {
                    field_17C0_bus.field_C_carriages[0] = NULL;
                    gBusSpawned_6FF1CD = 0;
                    gBusSpawnDelay_6FF1D0 = 0;
                }
                else
                {
                    switch (field_17C0_bus.field_48_stop_phase)
                    {
                        case 12:
                            --field_17C0_bus.field_4_wait_timer;
                            field_17C0_bus.field_C_carriages[0]->HaltAIDriving_43AF60();
                            if (!field_17C0_bus.field_4_wait_timer)
                            {
                                field_17C0_bus.field_48_stop_phase = 5;
                                field_17C0_bus.field_4_wait_timer = 10;
                            }
                            break;

                        case 5:
                            --field_17C0_bus.field_4_wait_timer;
                            field_17C0_bus.field_C_carriages[0]->OpenAllDoors_43B380();
                            if (!field_17C0_bus.field_4_wait_timer)
                            {
                                field_17C0_bus.field_4_wait_timer = 100;
                                field_17C0_bus.field_48_stop_phase = 13;
                            }
                            break;

                        case 13:
                            if (!field_17C0_bus.field_2_passengers_fleeing)
                            {
                                field_17C0_bus.UpdatePassengerAI_578390();
                            }
                            if (!field_17C0_bus.field_0_is_hijacked)
                            {
                                if (!--field_17C0_bus.field_4_wait_timer)
                                {
                                    field_17C0_bus.field_48_stop_phase = 9;
                                    field_17C0_bus.field_4_wait_timer = 10;
                                }
                            }
                            else if (field_17C0_bus.field_C_carriages[0]->GetCarLinearSpeed_43A240() > kFpZero_6FF078)
                            {
                                field_17C0_bus.field_48_stop_phase = 9;
                                field_17C0_bus.field_4_wait_timer = 10;
                            }
                            break;

                        case 9:
                            field_17C0_bus.field_C_carriages[0]->CloseAllDoors_43B3D0();
                            if (!--field_17C0_bus.field_4_wait_timer)
                            {
                                field_17C0_bus.field_48_stop_phase = 14;
                                field_17C0_bus.field_4_wait_timer = 10;
                            }
                            break;

                        case 14:
                            field_17C0_bus.field_C_carriages[0]->ResumeAIDriving_43AF40();
                            if (!--field_17C0_bus.field_4_wait_timer)
                            {
                                field_17C0_bus.field_48_stop_phase = 0;
                            }
                            break;
                        default:
                            break;
                    }
                    gSpriteGrid_2_67920C->CheckAndHandleCollisionInStrips_477BD0(field_17C0_bus.field_C_carriages[0]->field_50_car_sprite);
                }
            }
        }
    }
}

// Tells the player driving the train which station it is heading for
static inline void SetDriverStation(Train* pTrain, TrainStation* pStation)
{
    Car* pCar = pTrain->field_C_carriages[0];
    if (pCar)
    {
        Ped* pDriver = pCar->field_54_driver;
        if (pDriver && pTrain->field_43_wagon_count > 0)
        {
            pDriver->SetTrainStation_4AF860(pStation);
        }
    }
}

MATCH_FUNC(0x57a7a0)
void PublicTransport::PublicTransportService_57A7A0()
{
    bool bStopped = false;
    BusesService_579CA0();
    if (!bSkip_trains_67D550)
    {
        for (u16 i = 0; i < 10; i++)
        {
            Train* pTrain = &field_1450_train_array[i];
            if (!pTrain->field_C_carriages[0])
            {
                continue;
            }

            pTrain->ProcessTrainExplosionChain_578670();
            Car* pCar = pTrain->field_C_carriages[0];
            if (pCar->IsDrivenByPlayer_4118D0())
            {
                if (!pTrain->field_C_carriages[0]->field_54_driver)
                {
                    pCar->SetDriverPedType_421560(ped_type::unknown_1);
                    pTrain->field_0_is_hijacked = false;
                }
                else
                {
                    pTrain->field_0_is_hijacked = true;
                    if (pCar->field_54_driver->GetMoveDirection_45C9B0() > kFpZero_6FF078)
                    {
                        if (pTrain->field_C_carriages[0]->field_54_driver->field_15C_player->field_8B_forward_changed)
                        {
                            pTrain->IncrementState_578180();
                        }
                    }
                    else if (pTrain->field_C_carriages[0]->field_54_driver->GetMoveDirection_45C9B0() < kFpZero_6FF078)
                    {
                        if (pTrain->field_C_carriages[0]->field_54_driver->field_15C_player->field_8C_down_changed)
                        {
                            pTrain->DecrementState_5781F0();
                        }
                    }
                }
            }

            switch (pTrain->field_50_state)
            {
                case 0:
                case 1:
                    bStopped = true;
                    break;
                case 2:
                    bStopped = false;
                    break;
                case 3:
                case 4:
                case 5:
                    bStopped = true;
                    break;
            }

            TrainStation* pStation;
            switch (pTrain->field_48_stop_phase)
            {
                case 10:
                    pTrain->CloseCarriageDoors_578330();
                    pStation = pTrain->field_4C_station;
                    if (!pTrain->field_0_is_hijacked)
                    {
                        pTrain->StartIfHasDriver_5782D0();
                    }
                    else if (!bStopped)
                    {
                        if ((static_cast<u8>(pTrain->field_C_carriages[0]->field_50_car_sprite->field_14_xy.x.ToInt()) == pStation->field_C_stop_point->field_1_x && static_cast<u8>(pTrain->field_C_carriages[0]->field_50_car_sprite->field_14_xy.y.ToInt()) == pStation->field_C_stop_point->field_2_y))
                        {
                            pTrain->field_48_stop_phase = 4;
                            break;
                        }
                        pTrain->field_48_stop_phase = 11;
                    }
                    if ((static_cast<u8>(pTrain->field_C_carriages[0]->field_50_car_sprite->field_14_xy.x.ToInt()) == pStation->field_8_exit_point->field_1_x && static_cast<u8>(pTrain->field_C_carriages[0]->field_50_car_sprite->field_14_xy.y.ToInt()) == pStation->field_8_exit_point->field_2_y))
                    {
                        pTrain->field_48_stop_phase = 0;
                        pTrain->IncrementState_578180();
                        pStation = pTrain->field_4C_station;
                        pStation->field_18_pStoppedTrain = NULL;
                        pStation = pStation->field_20_next_station;
                        pTrain->field_4C_station = pStation;
                        pStation->field_1C_platform_state = 1;
                    }
                    break;

                case 11:
                    if (bStopped)
                    {
                        pTrain->field_48_stop_phase = 10;
                    }
                    break;

                case 0:
                    pStation = pTrain->field_4C_station;
                    if (!pTrain->field_0_is_hijacked)
                    {
                        if (pStation->field_18_pStoppedTrain)
                        {
                            pTrain->Stop_578300();
                            pTrain->field_48_stop_phase = 1;
                        }
                    }
                    else if (!bStopped)
                    {
                        pTrain->field_48_stop_phase = 1;
                    }
                    if (!pTrain->field_0_is_hijacked)
                    {
                        if ((static_cast<u8>(pTrain->field_C_carriages[0]->field_50_car_sprite->field_14_xy.x.ToInt()) == pStation->field_4_entry_point->field_1_x && static_cast<u8>(pTrain->field_C_carriages[0]->field_50_car_sprite->field_14_xy.y.ToInt()) == pStation->field_4_entry_point->field_2_y))
                        {
                            SetDriverStation(pTrain, pStation);
                            pTrain->field_48_stop_phase = 2;
                            pTrain->DecrementState_5781F0();
                        }
                        else if (pTrain->field_50_state != 2 && bStopped)
                        {
                            pTrain->IncrementState_578180();
                        }
                    }
                    break;

                case 1:
                    if (!pTrain->field_0_is_hijacked)
                    {
                        if (!pTrain->field_4C_station->field_18_pStoppedTrain)
                        {
                            pTrain->StartIfHasDriver_5782D0();
                            pTrain->field_48_stop_phase = 0;
                            pTrain->IncrementState_578180();
                        }
                    }
                    else if (bStopped)
                    {
                        pTrain->field_48_stop_phase = 0;
                    }
                    break;

                case 2:
                    pStation = pTrain->field_4C_station;
                    pStation->field_18_pStoppedTrain = pTrain;
                    if ((static_cast<u8>(pTrain->field_C_carriages[0]->field_50_car_sprite->field_14_xy.x.ToInt()) == pStation->field_C_stop_point->field_1_x && static_cast<u8>(pTrain->field_C_carriages[0]->field_50_car_sprite->field_14_xy.y.ToInt()) == pStation->field_C_stop_point->field_2_y))
                    {
                        pTrain->field_48_stop_phase = 4;
                    }
                    else if (pTrain->field_0_is_hijacked == 1 && !bStopped)
                    {
                        pTrain->field_48_stop_phase = 3;
                    }
                    break;

                case 3:
                    if (pTrain->field_0_is_hijacked == 1 && bStopped)
                    {
                        pTrain->field_48_stop_phase = 2;
                    }
                    break;

                case 4:
                    if (!pTrain->field_0_is_hijacked)
                    {
                        pTrain->Stop_578300();
                        pTrain->field_48_stop_phase = 5;
                        pTrain->field_4_wait_timer = 10;
                        pTrain->OpenCarriageDoors_578360();
                    }
                    else if (bStopped)
                    {
                        pTrain->field_48_stop_phase = 10;
                    }
                    else
                    {
                        pTrain->field_48_stop_phase = 5;
                        pTrain->OpenCarriageDoors_578360();
                        pTrain->field_4_wait_timer = 10;
                    }
                    break;

                case 5:
                    pTrain->field_4_wait_timer--;
                    pTrain->OpenCarriageDoors_578360();
                    if (pTrain->field_0_is_hijacked == 1 && bStopped)
                    {
                        pTrain->field_48_stop_phase = 10;
                    }
                    else if (pTrain->field_4_wait_timer == 0)
                    {
                        pStation = pTrain->field_4C_station;
                        pTrain->field_48_stop_phase = 6;
                        pTrain->field_4_wait_timer = 10;
                        pStation->field_18_pStoppedTrain = pTrain;
                        pStation->field_1C_platform_state = 4;
                    }
                    break;

                case 6:
                    if (pTrain->field_0_is_hijacked == 1)
                    {
                        if (bStopped)
                        {
                            pTrain->field_48_stop_phase = 10;
                            break;
                        }
                        pTrain->field_4_wait_timer--;
                        if (pTrain->field_4_wait_timer == 0)
                        {
                            pTrain->field_4_wait_timer = 50;
                            pTrain->field_48_stop_phase = 7;
                            pTrain->field_4C_station->field_1C_platform_state = 2;
                        }
                    }
                    else
                    {
                        pTrain->field_4_wait_timer--;
                        if (pTrain->field_4_wait_timer == 0)
                        {
                            pTrain->field_4_wait_timer = 50;
                            pTrain->field_48_stop_phase = 7;
                            pTrain->field_4C_station->field_1C_platform_state = 2;
                        }
                    }
                    break;

                case 7:
                    if (pTrain->field_0_is_hijacked == 1)
                    {
                        if (bStopped)
                        {
                            pTrain->field_48_stop_phase = 10;
                            break;
                        }
                        pTrain->UpdatePassengerAI_578390();
                        pTrain->field_4_wait_timer--;
                        if (pTrain->field_4_wait_timer == 0)
                        {
                            pTrain->field_48_stop_phase = 8;
                            pTrain->field_4_wait_timer = 50;
                            pTrain->field_4C_station->field_1C_platform_state = 2;
                        }
                    }
                    else
                    {
                        pTrain->UpdatePassengerAI_578390();
                        pTrain->field_4_wait_timer--;
                        if (pTrain->field_4_wait_timer == 0)
                        {
                            pTrain->field_48_stop_phase = 8;
                            pTrain->field_4_wait_timer = 50;
                            pTrain->field_4C_station->field_1C_platform_state = 2;
                        }
                    }
                    break;

                case 8:
                    if (pTrain->field_0_is_hijacked == 1)
                    {
                        if (bStopped)
                        {
                            pTrain->field_48_stop_phase = 10;
                            break;
                        }
                        pTrain->field_4_wait_timer--;
                        if (pTrain->field_4_wait_timer == 0)
                        {
                            pTrain->field_48_stop_phase = 9;
                            pTrain->field_4_wait_timer = 10;
                            pTrain->field_4C_station->field_1C_platform_state = 3;
                        }
                    }
                    else
                    {
                        pTrain->field_4_wait_timer--;
                        if (pTrain->field_4_wait_timer == 0)
                        {
                            pStation = pTrain->field_4C_station;
                            SetDriverStation(pTrain, pStation);
                            pTrain->field_48_stop_phase = 9;
                            pTrain->field_4_wait_timer = 10;
                            pStation->field_1C_platform_state = 3;
                        }
                    }
                    break;

                case 9:
                    pTrain->CloseCarriageDoors_578330();
                    if (pTrain->field_0_is_hijacked == 1)
                    {
                        if (bStopped)
                        {
                            pTrain->field_48_stop_phase = 10;
                        }
                    }
                    else
                    {
                        pTrain->field_4_wait_timer--;
                        if (pTrain->field_4_wait_timer == 0)
                        {
                            pTrain->field_48_stop_phase = 10;
                            pTrain->field_4C_station->field_1C_platform_state = 1;
                        }
                    }
                    break;
            }
        }
    }
}

MATCH_FUNC(0x57b4b0)
TrainStation* PublicTransport::TrainStationForZone_57B4B0(gmp_map_zone* pZone)
{
    TrainStation* pIter = &field_0_stations[0];
    for (u16 i = 0; i < GTA2_COUNTOF(field_0_stations); i++)
    {
        if (pIter->field_10_pZone == pZone)
        {
            return pIter;
        }
        pIter++;
    }
    return NULL;
}

MATCH_FUNC(0x57b540)
Car* PublicTransport::GetLeadTrainCar_57B540(Car* pCar)
{
    if (!bSkip_trains_67D550 && pCar->IsTrainModel_403BA0())
    {
        return GetTrainFromCar_57B5C0(pCar)->field_C_carriages[0];
    }
    else
    {
        return NULL;
    }
}

MATCH_FUNC(0x57b5c0)
Train* PublicTransport::GetTrainFromCar_57B5C0(Car* pToFind)
{
    if (!bSkip_trains_67D550)
    {
        for (u8 i = 0; i < 10; i++)
        {
            Train* pTrain = &field_1450_train_array[i];
            Car* pWagon = pTrain->field_C_carriages[0];
            if (pWagon == pToFind)
            {
                return pTrain;
            }
            for (u8 wagon_idx = 0; wagon_idx < pTrain->field_43_wagon_count; wagon_idx++)
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
Train* PublicTransport::GetTrainFromCarExcludingLeadCar_57B6A0(Car* pToFind)
{
    if (!bSkip_trains_67D550)
    {
        for (u8 i = 0; i < 10; i++)
        {
            Train* pTrain = &field_1450_train_array[i];
            for (u8 wagon_idx = 0; wagon_idx < pTrain->field_43_wagon_count; wagon_idx++)
            {
                Car* pWagon = pTrain->field_C_carriages[wagon_idx + 1];
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
bool PublicTransport::AreTrainsOnDifferentTracks_57B740(Car* pCar1, Car* pCar2)
{
    if (pCar1->IsTrainModel_403BA0() && pCar2->IsTrainModel_403BA0())
    {
        Train* pTrain1 = GetTrainFromCar_57B5C0(pCar1);
        Train* pTrain2 = GetTrainFromCar_57B5C0(pCar2);
        if (pTrain1->field_57_track_idx != pTrain2->field_57_track_idx)
        {
            return true;
        }
    }
    return false;
}

MATCH_FUNC(0x57b7b0)
PublicTransport::PublicTransport()
{
}

MATCH_FUNC(0x57b820)
PublicTransport::~PublicTransport()
{
}