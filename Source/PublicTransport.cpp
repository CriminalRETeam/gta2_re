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

DEFINE_GLOBAL(PublicTransport*, gPublicTransport_181C, 0x6FF1D4);
DEFINE_GLOBAL(TrainStationList, gTrainStationList, 0x6FEE68);
DEFINE_GLOBAL(TrainStopCount, gTrainStopCount, 0x6FEFFC); // name sets a 4-aligned .bss offset: InitStationsLinkedList reads it as a dword
DEFINE_STATIC_MEMBER(u8, PublicTransport, sStationCount, 0x6FF1CC);
DEFINE_GLOBAL(u8, gTargetCarDoor, 0x6FF1D8);
DEFINE_GLOBAL_INIT(Fix16, kFpZero_6FF078, 0, 0x6FF078);
DEFINE_STATIC_GLOBAL_INIT(Fix16_Point, kZeroPoint, Fix16_Point(Fix16(0), Fix16(0)), 0x6FF150);
DEFINE_GLOBAL(u8, gNameCompareLen, 0x6FF158);
DEFINE_STATIC_MEMBER(bool, PublicTransport, sBusSpawned, 0x6FF1CD);
DEFINE_STATIC_MEMBER(s32, PublicTransport, sBusSpawnDelay, 0x6FF1D0);
DEFINE_GLOBAL_INIT(Fix16, kFpOne_6FF07C, Fix16(1), 0x6FF07C);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint05_6FF04C, Fix16(0x333, 0), 0x6FF04C);

DEFINE_GLOBAL_INIT(Fix16, kFpHalf_6FEEE8, Fix16(0.5), 0x6FEEE8);
DEFINE_GLOBAL_INIT(Ang16, kAng0_6FF1BC, Ang16(0), 0x6FF1BC);

// Train spawning values
DEFINE_GLOBAL_INIT(Fix16, kTrainStopBlockOffset, Fix16(0.5), 0x6FEF88);
DEFINE_GLOBAL_INIT(Fix16, kWagonSpacingX, Fix16(4), 0x6FF088);
DEFINE_GLOBAL_INIT(Fix16, kWagonSpacingY, Fix16(2), 0x6FF080);
DEFINE_GLOBAL_INIT(Ang16, kAng270_6FEFFE, Ang16(1080), 0x6FEFFE);
DEFINE_GLOBAL_INIT(Ang16, kAng90_6FEF04, Ang16(360), 0x6FEF04);
DEFINE_GLOBAL_INIT(Ang16, kAng180_6FEFD6, Ang16(720), 0x6FEFD6);

DEFINE_STATIC_GLOBAL_INIT(Fix16, kTrainSpeedState0, Fix16(0x1333, 0), 0x6FEEE0);
DEFINE_GLOBAL_INIT(Fix16, kTrainSpeedState1And3, Fix16(0x666, 0), 0x6FEED4);
DEFINE_GLOBAL_INIT(Fix16, kTrainSpeedState4, Fix16(0xCCC, 0), 0x6FEEDC);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kTrainSpeedState5, Fix16(0x1999, 0), 0x6FEEE4);

MATCH_FUNC(0x577E20)
bool __stdcall HasBlockGreenArrowAtDirection(s32 direction, const gmp_block_info* pBlock)
{
    switch (direction)
    {
        case road_direction::up_1:
            return pBlock->mArrows >> 2 & 1;
        case road_direction::down_2:
            return pBlock->mArrows >> 3 & 1;
        case road_direction::right_3:
            return pBlock->mArrows >> 1 & 1;
        case road_direction::left_4:
            return pBlock->mArrows & block_arrow::green_left_1;
        default:
            return true;
    }
}

MATCH_FUNC(0x577E90)
bool __stdcall CompareNameChars(const char_type* pChar1, const char_type* pChar2)
{
    for (u8 i = 0; i < gNameCompareLen; i++)
    {
        if (pChar1[i] != pChar2[i])
        {
            return false;
        }
    }
    return true;
}

MATCH_FUNC(0x577EE0)
gmp_map_zone* __stdcall FindStationZoneByName(const char_type* pChar, u8 case_value)
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
            FatalError(Gta2Error::InvalidCase, "C:\\Splitting\\Gta2\\Source\\pubtrans.cpp", 98);
            break;
    }

    gmp_map_zone* pZone = gMap_0x370->GetFirstZoneOfType(zone_type);
    gNameCompareLen = 6;
    while (!CompareNameChars(reinterpret_cast<char_type*>(&pZone->mName), pChar))
    {
        pZone = gMap_0x370->GetNextZone();
        if (!pZone)
        {
            return NULL;
        }
        gNameCompareLen = 6;
    }
    return pZone;
}

// 9.6f 0x421510 (Car member there), inlined
static inline void EnsureAI_421510(Car* pCar)
{
    pCar->AllocateAI();
}

MATCH_FUNC(0x577f80)
s32 TrainStation::GetWagonType(u8 idx) const
{
    if (mTrainWagons[0] != 0)
    {
        switch (mTrainWagons[idx])
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
    mStationType = 0;
    mEntryPoint = NULL;
    mExitPoint = NULL;
    mStopPoint = NULL;
    mpZone = NULL;
    mUsed = 0;
    mpStoppedTrain = NULL;
    mPlatformState = 0;
    mNextStation = NULL;
    mWagonsNumber = 0;
    mTrackIdx = 0;

    for (u8 i = 0; i < 10; i++)
    {
        mTrainWagons[i] = 1;
    }
}

MATCH_FUNC(0x578010)
TrainStation::~TrainStation()
{
    mpStoppedTrain = NULL;
    mEntryPoint = NULL;
    mExitPoint = NULL;
    mStopPoint = NULL;
    mpZone = NULL;
    mNextStation = NULL;
}

MATCH_FUNC(0x578030)
void Train::ReassignTrainHead()
{
    if (!bSkip_trains)
    {
        Car* pFirst = this->mCarriages[0];
        this->mCarriages[0] = this->mCarriages[this->mWagonCount];
        this->mCarriages[this->mWagonCount] = pFirst;

        gCar_BC_Pool->UpdateNextPrev(this->mCarriages[0]);

        gCar_BC_Pool->mPool.UnlinkFromActiveList(this->mCarriages[this->mWagonCount]);

        // AllocCarPhysics only allocates when there is no physics, so these guarded calls do nothing
        Car* pTrainCar = this->mCarriages[this->mWagonCount];
        if (pTrainCar->mPhysics)
        {
            pTrainCar->AllocCarPhysics();
        }

        this->mCarriages[this->mWagonCount]->mPhysics->Init();
        if (this->mCarriages[0]->mPhysics)
        {
            this->mCarriages[0]->AllocCarPhysics();
        }

        this->mCarriages[0]->mPhysics->SetSprite(this->mCarriages[0]->mCarSprite);
        this->mCarriages[0]->mDriver = pFirst->mDriver;
        
        if (this->mCarriages[0]->mPhysics)
        {
            this->mCarriages[0]->AllocCarPhysics();
        }

        Car* pHead = this->mCarriages[0];
        if (pHead->IsDrivenByPlayer())
        {
            pHead->mPhysics->SetField8C_to_2();
        }
        else
        {
            pHead->mPhysics->SetField8C_to_1();
        }
        pFirst->mDriver = NULL;

        EnsureAI_421510(this->mCarriages[0]);

        this->mCarriages[0]->mAI->SetCar(this->mCarriages[0]);

        pFirst->DeAllocateAI();

        this->mCarriages[0]->SetDriverPedType(this->mCarriages[0]->mDriver->GetPedType());
    }
}

MATCH_FUNC(0x578180)
void Train::IncrementState()
{
    if (!bSkip_trains)
    {
        switch (this->mState)
        {
            case 0:
                this->mState = 1;
                break;
            case 1:
                ReassignTrainHead();
                this->mState = 2;
                break;
            case 2:
                this->mState = 3;
                break;
            case 3:
                this->mState = 4;
                break;
            case 4:
                this->mState = 5;
                break;
            default:
                return;
        }
    }
}

MATCH_FUNC(0x5781f0)
void Train::DecrementState()
{
    if (!bSkip_trains)
    {
        switch (this->mState)
        {
            case 1:
                this->mState = 0;
                break;
            case 2:
                ReassignTrainHead();
                this->mState = 1;
                break;
            case 3:
                this->mState = 2;
                break;
            case 4:
                this->mState = 3;
                break;
            case 5:
                this->mState = 4;
                break;
            default:
                return;
        }
    }
}

MATCH_FUNC(0x578260)
Train::Train()
{
    mAllocState = 0;
    mCarriages[0] = NULL;
    mUnused44 = 0;
    mStation = NULL;
    mWaitTimer = 0;
    mIsHijacked = false;
    mPassengersFleeing = false;
    mStopPhase = 0;
    mState = 2;
    mPassengerTimer = 0;
    mUnused55 = 0;
    mPassengerCount = 0;
    mTrackIdx = 0;
    char_type* pTimer = mExplosionTimers;
    Car** ppCarriage = mCarriages + 1;
    s32 remaining = 10;
    do
    {
        *ppCarriage = NULL;
        *pTimer = -1;
        ppCarriage++;
        pTimer++;
        remaining--;
    } while (remaining != 0);
    mExplosionTimers[10] = -1;
    mWagonCount = 0;
    mUnused1 = 0;
}

MATCH_FUNC(0x5782c0)
Train::~Train()
{
    this->mCarriages[0] = NULL;
    this->mStation = NULL;
}

MATCH_FUNC(0x5782d0)
void Train::StartIfHasDriver()
{
    if (!bSkip_trains)
    {
        if (this->mCarriages[0]->mDriver)
        {
            this->mState = 3;
        }
        else
        {
            this->mState = 2;
        }
    }
}

MATCH_FUNC(0x578300)
void Train::Stop()
{
    if (!bSkip_trains)
    {
        if (this->mState < 2)
        {
            this->mUnused1 = 1;
            ReassignTrainHead();
        }
        this->mState = 2;
    }
}

MATCH_FUNC(0x578330)
void Train::CloseCarriageDoors()
{
    if (!bSkip_trains)
    {
        for (s32 i = 0; i < 2; i++)
        {
            if (mCarriages[i + 1])
            {
                if (mCarriages[i + 1]->GetCarInfoIdx() == car_model_enum::TRAIN)
                {
                    mCarriages[i + 1]->CloseAllDoors();
                }
            }
        }
    }
}

MATCH_FUNC(0x578360)
void Train::OpenCarriageDoors()
{
    if (!bSkip_trains)
    {
        for (s32 i = 0; i < 2; i++)
        {
            if (mCarriages[i + 1])
            {
                if (mCarriages[i + 1]->GetCarInfoIdx() == car_model_enum::TRAIN)
                {
                    mCarriages[i + 1]->OpenAllDoors();
                }
            }
        }
    }
}

MATCH_FUNC(0x578390)
void Train::UpdatePassengerAI()
{
    if (!bSkip_trains && !bSkip_dummies && gPedManager->mNumDummyChars < 50u)
    {
        if (this->mAllocState == 2)
        {
            for (u8 i = 0; i < this->mWagonCount; i++)
            {
                Car** ppWagon = &this->mCarriages[i + 1];
                if ((*ppWagon)->GetCarInfoIdx() == car_model_enum::TRAIN)
                {
                    this->mPassengerCount = 1;
                    if (gGame_0x40->IsSpriteInAnyPlayerBounds((*ppWagon)->mCarSprite, kFpZero_6FF078))
                    {
                        if (this->mPassengerTimer <= 0)
                        {
                            for (gTargetCarDoor = static_cast<u8>(gRng.get_int(4)); gTargetCarDoor < static_cast<u8>((*ppWagon)->GetDoorCount());
                                 gTargetCarDoor++)
                            {
                                if ((*ppWagon)->IsStoppedWithPavementAtDoor(gTargetCarDoor) && this->mPassengerCount > 0)
                                {
                                    Ped* pPassenger = gPedManager->SpawnTrainLeaver();
                                    pPassenger->mCar = *ppWagon;
                                    pPassenger->SetObjective(objectives_enum::leave_train_38);
                                    Ped_List* pLink = &pPassenger->mCar->mPassengersList;
                                    pPassenger->SetTargetObjectiveCar(*ppWagon);
                                    pLink->AddPedToFront(pPassenger);
                                    pPassenger->set_target_car_door(gTargetCarDoor);
                                    --this->mPassengerCount;
                                }
                            }
                            this->mPassengerTimer = 7;
                        }
                    }
                }
            }
            --this->mPassengerTimer;
        }
        else
        {
            if (gGame_0x40->IsSpriteInAnyPlayerBounds(this->mCarriages[0]->mCarSprite, kFpZero_6FF078) &&
                this->mPassengerTimer <= 0 && !gPublicTransport_181C->mStopGettingOffBus)
            {
                if (this->mCarriages[0]->IsStoppedWithPavementAtDoor(2))
                {
                    if (this->mCarriages[0]->mPassengersList.HasNoPeds())
                    {
                        if (this->mPassengerCount > 6 || !this->mIsHijacked)
                        {
                            Ped* pPassenger = gPedManager->SpawnTrainLeaver();
                            pPassenger->mCar = this->mCarriages[0];
                            this->mCarriages[0]->mPassengersList.AddPedToFront(pPassenger);
                            pPassenger->SetObjective(objectives_enum::leave_train_38);
                            Car* pBus = this->mCarriages[0];
                            pPassenger->SetTargetObjectiveCar(pBus);
                            pPassenger->set_target_car_door(2);
                            pPassenger->set_occupation(ped_ocupation_enum::bus_customer_8);
                            if (this->mIsHijacked == 1)
                            {
                                this->mPassengerCount--;
                            }
                        }
                    }
                    else
                    {
                        Ped* pPassenger = this->mCarriages[0]->mPassengersList.RemoveFirstPed();
                        pPassenger->mCar = this->mCarriages[0];
                        pPassenger->SetObjective(objectives_enum::leave_train_38);
                        Car* pBus = this->mCarriages[0];
                        pPassenger->SetTargetObjectiveCar(pBus);
                        pPassenger->set_target_car_door(2);
                        pPassenger->set_occupation(ped_ocupation_enum::bus_customer_8);
                        if (this->mIsHijacked == 1)
                        {
                            if (this->mPassengerCount > 0)
                            {
                                this->mPassengerCount--;
                            }
                        }
                    }
                }

                if (!this->mIsHijacked)
                {
                    this->mPassengerTimer = gRng.get_int(20) + 20;
                }
                else
                {
                    this->mPassengerTimer = gRng.get_int(20) + 40;
                }
            }
            --this->mPassengerTimer;
        }
    }
}

MATCH_FUNC(0x578670)
void Train::ProcessTrainExplosionChain()
{
    Car* pWagon = mCarriages[1];
    if (!bSkip_trains)
    {
        Car* pFirst = mCarriages[0];
        if (pFirst->mDamage >= 32000 && mExplosionTimers[10] == -1)
        {
            mExplosionTimers[10] = 10;
        }
        if (mExplosionTimers[0] == 1 && pWagon->mDamage >= 32000)
        {
            mExplosionTimers[10] = 10;
        }
        if (mExplosionTimers[10] > 0)
        {
            --mExplosionTimers[10];
            if (!mExplosionTimers[10])
            {
                if (pFirst->mDamage >= 32000)
                {
                    pWagon->AccumulateDamage(32000, &kZeroPoint);
                }
                else
                {
                    pFirst->AccumulateDamage(32000, &kZeroPoint);
                }
            }
        }

        for (u8 car_idx = 0; car_idx < mWagonCount; pWagon++, car_idx++)
        {
            if (mExplosionTimers[car_idx] == -1 && pWagon->mDamage >= 32000)
            {
                if (car_idx > 0)
                {
                    mExplosionTimers[car_idx - 1] = 10;
                }
                else if (car_idx < mWagonCount - 1)
                {
                    mExplosionTimers[car_idx + 1] = 10;
                }
                else if (car_idx == 0)
                {
                    mExplosionTimers[10] = 10;
                }
                mExplosionTimers[car_idx] = 0;
            }
            if (mExplosionTimers[car_idx] > 0)
            {
                --mExplosionTimers[car_idx];
                if (!mExplosionTimers[car_idx])
                {
                    mCarriages[car_idx + 1]->AccumulateDamage(32000, &kZeroPoint);
                    if (car_idx > 0)
                    {
                        mExplosionTimers[car_idx - 1] = 10;
                    }
                    if (car_idx < mWagonCount - 1)
                    {
                        mExplosionTimers[car_idx + 1] = 10;
                    }
                    if (!car_idx)
                    {
                        mExplosionTimers[10] = 10;
                    }
                }
            }
        }
    }
}

MATCH_FUNC(0x578790)
Train* PublicTransport::AllocateTrain()
{
    if (bSkip_trains)
    {
        return NULL;
    }

    for (u16 i = 0; i < GTA2_COUNTOF(mTrainArray); i++)
    {
        if (!mTrainArray[i].mAllocState)
        {
            return &this->mTrainArray[i];
        }
    }
    return NULL;
}

MATCH_FUNC(0x5787e0)
TrainStation* PublicTransport::AllocateTrainStation()
{
    for (u16 i = 0; i < GTA2_COUNTOF(mStations); i++)
    {
        if (!this->mStations[i].mUsed)
        {
            return &this->mStations[i];
        }
    }
    return NULL;
}

MATCH_FUNC(0x578820)
void TrainStation::CalculateWagonCount(const u8* pWagons)
{
    if (!bSkip_trains)
    {
        memcpy(mTrainWagons, pWagons, sizeof(mTrainWagons));

        for (u8 i = 0; i < 10; i++)
        {
            if (mTrainWagons[i])
            {
                mWagonsNumber++;
            }
        }
    }
}

// The wagon spawns pass the "+ half block" coordinate as Fix16::Add_ref of two temporaries (9.6f copies
// kTrainStopBlockOffset into a temp first): VC6 then builds that argument only after the GetWagonType call,
// keeping the u8 coordinate and the copy across it, as the original does. The member operator+ builds it first.
MATCH_FUNC(0x578860)
void PublicTransport::SpawnTrainsFromStations()
{
    if (bSkip_trains)
    {
        return;
    }

    for (s32 i = 0; i < 10; i++)
    {
        TrainStation* pStation = &mStations[i];
        char_type wagons = pStation->mWagonsNumber;
        if (pStation->mStationType && wagons)
        {
            switch (pStation->mStationType)
            {
                case 2:
                {
                    if (!pStation->mpZone)
                    {
                        FatalError(Gta2Error::IllegalTrainStationNoPlatformZone, "C:\\Splitting\\Gta2\\Source\\pubtrans.cpp", 734);
                    }
                    if (!pStation->mEntryPoint)
                    {
                        FatalError(Gta2Error::IllegalTrainStationNoEntryZone,
                                   "C:\\Splitting\\Gta2\\Source\\pubtrans.cpp",
                                   738,
                                   pStation->mpZone->mX,
                                   pStation->mpZone->mY);
                    }
                    if (!pStation->mExitPoint)
                    {
                        FatalError(Gta2Error::IllegalTrainStationNoExitZone,
                                   "C:\\Splitting\\Gta2\\Source\\pubtrans.cpp",
                                   740,
                                   pStation->mpZone->mX,
                                   pStation->mpZone->mY);
                    }
                    if (!pStation->mStopPoint)
                    {
                        FatalError(Gta2Error::IllegalTrainStationNoStopZone,
                                   "C:\\Splitting\\Gta2\\Source\\pubtrans.cpp",
                                   742,
                                   pStation->mpZone->mX,
                                   pStation->mpZone->mY);
                    }

                    Train* pTrain = AllocateTrain();
                    if (!pTrain)
                    {
                        FatalError(Gta2Error::NoMoreTrainSpace, "C:\\Splitting\\Gta2\\Source\\pubtrans.cpp", 746);
                    }
                    pTrain->mAllocState = 2;

                    s32 rail_z;
                    gmp_block_info* pBlock = gMap_0x370->FindRailwayAtCoord(pStation->mStopPoint->mX,
                                                                            pStation->mStopPoint->mY,
                                                                            rail_z);
                    u8 j;
                    if (HasBlockGreenArrowAtDirection(4, pBlock))
                    {
                        for (j = 0; j < wagons; j++)
                        {
                            pTrain->mCarriages[j + 1] = gCarManager_6C->SpawnCarAtCorrectZ_Scaled(
                                Fix16(pStation->mStopPoint->mX) + Fix16(j + 1) * kWagonSpacingX,
                                Fix16::Add_ref(Fix16(pStation->mStopPoint->mY), Fix16(kTrainStopBlockOffset)),
                                kAng270_6FEFFE,
                                pStation->GetWagonType(j),
                                kFpOne_6FF07C);
                            if (!pTrain->mCarriages[j + 1])
                            {
                                FatalError(Gta2Error::FailedToCreateCarriage,
                                           "C:\\Splitting\\Gta2\\Source\\pubtrans.cpp",
                                           761,
                                           (Fix16(pStation->mStopPoint->mX) + Fix16(j + 1) * kWagonSpacingX).ToInt(),
                                           (Fix16(pStation->mStopPoint->mY) + kTrainStopBlockOffset).ToInt(),
                                           rail_z);
                            }
                        }
                        pTrain->mCarriages[0] =
                            gCarManager_6C->SpawnCarAtCorrectZ_Scaled(Fix16(pStation->mStopPoint->mX),
                                                                      Fix16(pStation->mStopPoint->mY) + kTrainStopBlockOffset,
                                                                      kAng270_6FEFFE,
                                                                      car_model_enum::TRAINCAB,
                                                                      kFpOne_6FF07C);
                    }
                    else if (HasBlockGreenArrowAtDirection(2, pBlock))
                    {
                        for (j = 0; j < wagons; j++)
                        {
                            pTrain->mCarriages[j + 1] = gCarManager_6C->SpawnCarAtCorrectZ_Scaled(
                                Fix16::Add_ref(Fix16(pStation->mStopPoint->mX), Fix16(kTrainStopBlockOffset)),
                                Fix16(pStation->mStopPoint->mY) - Fix16(j + 1) * kWagonSpacingY,
                                kAng0_6FF1BC,
                                pStation->GetWagonType(j),
                                kFpOne_6FF07C);
                            if (!pTrain->mCarriages[j + 1])
                            {
                                FatalError(Gta2Error::FailedToCreateCarriage,
                                           "C:\\Splitting\\Gta2\\Source\\pubtrans.cpp",
                                           773,
                                           (Fix16(pStation->mStopPoint->mX) + kTrainStopBlockOffset).ToInt(),
                                           (Fix16(pStation->mStopPoint->mY) - Fix16(j + 1) * kWagonSpacingY).ToInt(),
                                           rail_z);
                            }
                        }
                        pTrain->mCarriages[0] =
                            gCarManager_6C->SpawnCarAtCorrectZ_Scaled(Fix16(pStation->mStopPoint->mX) + kTrainStopBlockOffset,
                                                                      Fix16(pStation->mStopPoint->mY),
                                                                      kAng0_6FF1BC,
                                                                      car_model_enum::TRAINCAB,
                                                                      kFpOne_6FF07C);
                    }
                    else if (HasBlockGreenArrowAtDirection(3, pBlock))
                    {
                        for (j = 0; j < wagons; j++)
                        {
                            pTrain->mCarriages[j + 1] = gCarManager_6C->SpawnCarAtCorrectZ_Scaled(
                                Fix16(pStation->mStopPoint->mX) - Fix16(j + 1) * kWagonSpacingX,
                                Fix16::Add_ref(Fix16(pStation->mStopPoint->mY), Fix16(kTrainStopBlockOffset)),
                                kAng270_6FEFFE,
                                pStation->GetWagonType(j),
                                kFpOne_6FF07C);
                            if (!pTrain->mCarriages[j + 1])
                            {
                                FatalError(Gta2Error::FailedToCreateCarriage,
                                           "C:\\Splitting\\Gta2\\Source\\pubtrans.cpp",
                                           785,
                                           (Fix16(pStation->mStopPoint->mX) - Fix16(j + 1) * kWagonSpacingX).ToInt(),
                                           (Fix16(pStation->mStopPoint->mY) + kTrainStopBlockOffset).ToInt(),
                                           rail_z);
                            }
                        }
                        pTrain->mCarriages[0] =
                            gCarManager_6C->SpawnCarAtCorrectZ_Scaled(Fix16(pStation->mStopPoint->mX),
                                                                      Fix16(pStation->mStopPoint->mY) + kTrainStopBlockOffset,
                                                                      kAng90_6FEF04,
                                                                      car_model_enum::TRAINCAB,
                                                                      kFpOne_6FF07C);
                    }
                    else if (HasBlockGreenArrowAtDirection(1, pBlock))
                    {
                        for (j = 0; j < wagons; j++)
                        {
                            pTrain->mCarriages[j + 1] = gCarManager_6C->SpawnCarAtCorrectZ_Scaled(
                                Fix16::Add_ref(Fix16(pStation->mStopPoint->mX), Fix16(kTrainStopBlockOffset)),
                                Fix16(pStation->mStopPoint->mY) + Fix16(j + 1) * kWagonSpacingY,
                                kAng180_6FEFD6,
                                pStation->GetWagonType(j),
                                kFpOne_6FF07C);
                            if (!pTrain->mCarriages[j + 1])
                            {
                                FatalError(Gta2Error::FailedToCreateCarriage,
                                           "C:\\Splitting\\Gta2\\Source\\pubtrans.cpp",
                                           796,
                                           (Fix16(pStation->mStopPoint->mX) + kTrainStopBlockOffset).ToInt(),
                                           (Fix16(pStation->mStopPoint->mY) + Fix16(j + 1) * kWagonSpacingY).ToInt(),
                                           rail_z);
                            }
                        }
                        pTrain->mCarriages[0] =
                            gCarManager_6C->SpawnCarAtCorrectZ_Scaled(Fix16(pStation->mStopPoint->mX) + kTrainStopBlockOffset,
                                                                      Fix16(pStation->mStopPoint->mY),
                                                                      kAng180_6FEFD6,
                                                                      car_model_enum::TRAINCAB,
                                                                      kFpOne_6FF07C);
                    }
                    else
                    {
                        FatalError(Gta2Error::IllegalBlockForTrainCreation,
                                   "C:\\Splitting\\Gta2\\Source\\pubtrans.cpp",
                                   802,
                                   pStation->mStopPoint->mX,
                                   pStation->mStopPoint->mY,
                                   rail_z);
                    }

                    pTrain->mWagonCount = wagons;
                    for (j = 0; j < wagons; j++)
                    {
                        gCar_BC_Pool->mPool.UnlinkFromActiveList(pTrain->mCarriages[j + 1]);
                    }

                    EnsureAI_421510(pTrain->mCarriages[0]);
                    pTrain->mCarriages[0]->mAI->SetCar(pTrain->mCarriages[0]);
                    pTrain->mCarriages[0]->SpawnDriverPed();
                    pTrain->mCarriages[0]->SetDriverPedType(ped_type::script_created_5);
                    Object* pLight = gObject_5C->NewLight(94, 138, 2, 0xFF8000, 3, 255);
                    pTrain->mCarriages[0]->mAttachments.PushAttachedSprite(pLight->mSprite, 0, 2, kAng0_6FF1BC);
                    pTrain->mCarriages[0]->LockPermanently();

                    for (j = 0; j < wagons; j++)
                    {
                        pTrain->mCarriages[j + 1]->SpawnDriverPed();
                        pTrain->mCarriages[j + 1]->SetDriverPedType(ped_type::script_created_5);
                        pTrain->mCarriages[j + 1]->SetupCarPhysicsAndSpriteBindingWithTrailer();
                        pTrain->mCarriages[j + 1]->StartEngine();
                        if (pTrain->mCarriages[j + 1]->mCarInfoIdx == car_model_enum::TRAINFB)
                        {
                            pTrain->mCarriages[j + 1]->LockPermanently();
                        }
                    }

                    pTrain->mUnused44 = 0;
                    pTrain->mStopPhase = 4;
                    pTrain->mStation = pStation;
                    pTrain->mPassengerCount = 6;
                    pTrain->mTrackIdx = pStation->mTrackIdx;
                    pTrain->mCarriages[0]->InitCarAIControl();
                    pTrain->mCarriages[0]->HaltAIDriving();
                    pStation->mUsed = 2;
                    pStation->mPlatformState = 1;
                    pStation->mpStoppedTrain = pTrain;
                    break;
                }
            }
        }
    }
}

MATCH_FUNC(0x5793e0)
void PublicTransport::InitStationsLinkedList()
{
    if (!bSkip_trains)
    {
        u16 idx = 0;
        for (; idx < gTrainStopCount.mCount - 1; idx++)
        {
            gTrainStationList.mList[idx]->mNextStation = gTrainStationList.mList[idx + 1];
        }
        gTrainStationList.mList[idx]->mNextStation = gTrainStationList.mList[0];
    }
}

MATCH_FUNC(0x579440)
void PublicTransport::InitTrainStations()
{
    for (s32 station_idx = 0; station_idx < 100; station_idx++)
    {
        TrainStation* pStation = &mStations[station_idx];
        switch (pStation->mStationType)
        {
            case 0:
                break;
            case 2:
                pStation->mEntryPoint = FindStationZoneByName(reinterpret_cast<char_type*>(&pStation->mpZone->mName), 0);
                pStation->mExitPoint = FindStationZoneByName(reinterpret_cast<char_type*>(&pStation->mpZone->mName), 1);
                pStation->mStopPoint = FindStationZoneByName(reinterpret_cast<char_type*>(&pStation->mpZone->mName), 2);
                break;
            case 1:
                break;
            default:
                FatalError(Gta2Error::InvalidCase, "C:\\Splitting\\Gta2\\Source\\pubtrans.cpp", 928);
                break;
        }
    }
}

// https://decomp.me/scratch/kgg76
MATCH_FUNC(0x5794b0)
void PublicTransport::SetupTrainAndBusStops()
{
    char_type track_name[8];
    sBusSpawned = false;
    sBusSpawnDelay = 0;
    if (!bSkip_trains)
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
                gTrainStationList.mList[k] = NULL;
            }
            gTrainStopCount.mCount = 0;

            gmp_map_zone* pPlatform;
            for (pPlatform = gMap_0x370->GetFirstZoneOfType(gmp_zone_type_enum::railway_station_platform); pPlatform != NULL;
                 pPlatform = gMap_0x370->GetNextZone())
            {
                gNameCompareLen = 5;
                if (CompareNameChars(track_name, pPlatform->mName))
                {
                    TrainStation* pStation = gPublicTransport_181C->AllocateTrainStation();
                    pStation->mStationType = 2;
                    pStation->mUsed = 1;
                    pStation->mPlatformState = 1;
                    pStation->mpZone = pPlatform;
                    pStation->mpStoppedTrain = NULL;
                    pStation->mTrackIdx = track_idx;
                    ++sStationCount;
                    switch (pPlatform->mName[static_cast<u8>(gNameCompareLen)])
                    {
                        case '0':
                            gTrainStationList.mList[0] = pStation;
                            break;
                        case '1':
                            gTrainStationList.mList[1] = pStation;
                            break;
                        case '2':
                            gTrainStationList.mList[2] = pStation;
                            break;
                        case '3':
                            gTrainStationList.mList[3] = pStation;
                            break;
                        case '4':
                            gTrainStationList.mList[4] = pStation;
                            break;
                        default:
                            break;
                    }
                    ++gTrainStopCount.mCount;
                }
            }
            if (gTrainStopCount.mCount > 0)
            {
                PublicTransport::InitStationsLinkedList();
            }
        }
    }

    if (!bSkip_buses)
    {
        for (gmp_map_zone* pZone = gMap_0x370->GetFirstZoneOfType(gmp_zone_type_enum::bus_stop_pavement); pZone != NULL;)
        {
            TrainStation* pBusStop = gPublicTransport_181C->AllocateTrainStation();
            pBusStop->mStationType = 1;
            pBusStop->mUsed = 1;
            pBusStop->mPlatformState = 1;
            pBusStop->mpZone = pZone;
            pBusStop->mpStoppedTrain = NULL;
            ++sStationCount;
            pZone = gMap_0x370->GetNextZone();

            s32 highest_zpos;

            // The original looks the block up twice
            gMap_0x370->FindHighestBlockForCoord(Fix16(pBusStop->mpZone->mX).ToInt(),
                                                 Fix16(pBusStop->mpZone->mY).ToInt(),
                                                 &highest_zpos);

            Fix16 xpos = Fix16(pBusStop->mpZone->mX);
            Fix16 ypos = Fix16(pBusStop->mpZone->mY);
            gMap_0x370->FindHighestBlockForCoord(xpos.ToInt(), ypos.ToInt(), &highest_zpos);

            for (u8 j = 0; j < 4; j++)
            {
                switch (j)
                {
                    case 0:
                        if (gMap_0x370->IsNorthBlockRoadType(xpos.ToInt(), ypos.ToInt(), highest_zpos))
                        {
                            ypos -= kFpOne_6FF07C;
                            j = 4;
                        }
                        break;

                    case 1:
                        if (gMap_0x370->IsBlockRoadTypeInlined_433470(xpos.ToInt() + 1, ypos.ToInt(), highest_zpos))
                        {
                            xpos += kFpOne_6FF07C;
                            j = 4;
                        }

                        break;
                    case 2:
                        if (gMap_0x370->IsBlockRoadTypeInlined_433470(xpos.ToInt(), ypos.ToInt() + 1, highest_zpos))
                        {
                            ypos += kFpOne_6FF07C;
                            j = 4;
                        }

                        break;
                    case 3:
                        if (gMap_0x370->IsBlockRoadTypeInlined_433470(xpos.ToInt() - 1, ypos.ToInt(), highest_zpos))
                        {
                            xpos -= kFpHalf_6FEEE8; // the original moves only half a block west
                            j = 4;
                        }

                        break;
                    default:
                        continue;
                }
            }

            gMap_0x370->FindHighestBlockForCoord(xpos.ToInt(), ypos.ToInt(), &highest_zpos);
            gObject_5C->NewPhysicsObj(objects::bus_stop_marker_129,
                                      kFpHalf_6FEEE8 + xpos,
                                      kFpHalf_6FEEE8 + ypos,
                                      kFpOne_6FF07C + Fix16(highest_zpos),
                                      kAng0_6FF1BC);
        }
    }
    PublicTransport::InitTrainStations();
}

MATCH_FUNC(0x5799b0)
TrainStation* PublicTransport::GetBusStopOnScreen()
{
    if (!bSkip_buses)
    {
        for (u16 station_idx = 0; station_idx < sStationCount; station_idx++)
        {
            TrainStation* pStation = &mStations[station_idx];
            if (pStation->mStationType == 1)
            {
                if (gGame_0x40->IsPointOnScreen(pStation->mpZone->mX, pStation->mpZone->mY))
                {
                    return pStation;
                }
            }
        }
    }
    return NULL;
}

MATCH_FUNC(0x579a30)
void PublicTransport::OnBusStopMarkerHit(Car* pToFind)
{
    if (!bSkip_buses)
    {
        Car* pLeadCar = mBus.mCarriages[0];
        if (pLeadCar)
        {
            if (pToFind == pLeadCar)
            {
                s32 bus_state = mBus.mStopPhase;
                if (bus_state)
                {
                    if (bus_state == 14)
                    {
                        mBus.mWaitTimer = 10;
                    }
                }
                else if (mBus.mIsHijacked != 1 || pLeadCar->GetCarLinearSpeed() == kFpZero_6FF078)
                {
                    mBus.mStopPhase = 12;
                    mBus.mWaitTimer = 10;
                }
            }
        }
    }
}

MATCH_FUNC(0x579aa0)
bool PublicTransport::is_bus(Car* pCar) const
{
    if (!bSkip_buses)
    {
        Car* pBus = this->mBus.mCarriages[0];
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
Car* PublicTransport::GetBusIfBoarding() const
{
    if (bSkip_buses)
    {
        return NULL;
    }

    Car* result = this->mBus.mCarriages[0];
    if (!result || this->mBus.mStopPhase != 13)
    {
        return NULL;
    }
    return result;
}

MATCH_FUNC(0x579af0)
bool PublicTransport::is_bus_full() const
{
    if (bSkip_buses)
    {
        return false;
    }

    if (mBus.mPassengerCount >= 10)
    {
        return true;
    }

    return false;
}

MATCH_FUNC(0x579b10)
void PublicTransport::IncrementBusPassengerCount()
{
    if (!bSkip_buses)
    {
        mBus.mPassengerCount++;
    }
}

MATCH_FUNC(0x579b20)
void PublicTransport::KillAllPassengers()
{
    if (!bSkip_buses)
    {
        this->mBus.mPassengerCount = 0;
        mBus.mCarriages[0]->mPassengersList.KillAndClearAllPeds();
    }
}

MATCH_FUNC(0x579b40)
Car** PublicTransport::GetCarArrayFromLeadCar(Car* toFind)
{
    for (u8 i = 0; i < GTA2_COUNTOF(mTrainArray); i++)
    {
        Train* pIter = &mTrainArray[i];
        if (pIter->mCarriages[0] == toFind)
        {
            return &pIter->mCarriages[1];
        }
    }
    return NULL;
}

MATCH_FUNC(0x579b90)
bool PublicTransport::GetTrainSpeed(Car* pToFind, Fix16* pOutSpeed)
{
    if (!bSkip_trains)
    {
        for (u8 i = 0; i < 10; i++)
        {
            Train* pTrain = &mTrainArray[i];
            Car* pCar = pTrain->mCarriages[0];
            if (pCar == pToFind)
            {
                if (!pCar->IsEngineOn())
                {
                    *pOutSpeed = kFpZero_6FF078;
                    if (pTrain->mState != 0 && pTrain->mState != 1)
                    {
                        return true;
                    }
                    return false;
                }

                switch (pTrain->mState)
                {
                    case 0:
                        *pOutSpeed = kTrainSpeedState0;
                        return false;
                    case 1:
                        *pOutSpeed = kTrainSpeedState1And3;
                        return false;
                    case 2:
                        *pOutSpeed = kFpZero_6FF078;
                        return true;
                    case 3:
                        *pOutSpeed = kTrainSpeedState1And3;
                        return true;
                    case 4:
                        *pOutSpeed = kTrainSpeedState4;
                        return true;
                    case 5:
                        *pOutSpeed = kTrainSpeedState5;
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
void PublicTransport::BusesService()
{
    Car* pBusCar;

    if (!bSkip_buses)
    {
        if (sBusSpawnDelay || sBusSpawned || !gCarManager_6C->CanAllocateCarOfKind(car_kind::recycled_1))
        {
            if (--sBusSpawnDelay < 0)
            {
                sBusSpawnDelay = 0;
            }
        }
        else
        {
            TrainStation* pBusStop = PublicTransport::GetBusStopOnScreen();
            if (pBusStop)
            {
                Fix16 xpos = Fix16(pBusStop->mpZone->mX);
                Fix16 ypos = Fix16(pBusStop->mpZone->mY);
                s32 found_z;
                gMap_0x370->FindHighestBlockForCoord(xpos.ToInt(), ypos.ToInt(), &found_z);
                for (u16 i = 0; i < 4; i++)
                {
                    switch (i)
                    {
                        case 0:
                            if (gMap_0x370->IsNorthBlockRoadType(xpos.ToInt(), ypos.ToInt(), found_z))
                            {
                                ypos -= kFpHalf_6FEEE8;
                                i = 4;
                            }
                            break;
                        case 1:
                            if (gMap_0x370->IsEastBlockRoadType(xpos.ToInt(), ypos.ToInt(), found_z))
                            {
                                xpos += kFpOne_6FF07C;
                                i = 4;
                            }
                            break;
                        case 2:
                            if (gMap_0x370->IsSouthBlockRoadType(xpos.ToInt(), ypos.ToInt(), found_z))
                            {
                                ypos += kFpOne_6FF07C;
                                i = 4;
                            }
                            break;
                        case 3:
                            if (gMap_0x370->IsWestBlockRoadType(xpos.ToInt(), ypos.ToInt(), found_z))
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
                    gMap_0x370->FindHighestBlockForCoord(xpos.ToInt(), ypos.ToInt(), &found_z);
                if (HasBlockGreenArrowAtDirection(4, pBlock))
                {
                    mBus.mCarriages[0] =
                        gCarManager_6C->SpawnBusAtValidRoadPosition(xpos, ypos + kFpHalf_6FEEE8, 4, car_model_enum::BUS);
                }
                if (HasBlockGreenArrowAtDirection(2, pBlock))
                {
                    mBus.mCarriages[0] =
                        gCarManager_6C->SpawnBusAtValidRoadPosition(xpos + kFpHalf_6FEEE8, ypos, 2, car_model_enum::BUS);
                }
                if (HasBlockGreenArrowAtDirection(3, pBlock))
                {
                    mBus.mCarriages[0] =
                        gCarManager_6C->SpawnBusAtValidRoadPosition(xpos, ypos, 3, car_model_enum::BUS);
                }
                if (HasBlockGreenArrowAtDirection(1, pBlock))
                {
                    mBus.mCarriages[0] =
                        gCarManager_6C->SpawnBusAtValidRoadPosition(xpos, ypos, 1, car_model_enum::BUS);
                }
                pBusCar = mBus.mCarriages[0];
                if (pBusCar)
                {
                    EnsureAI_421510(pBusCar);
                    mBus.mCarriages[0]->mAI->SetCar(mBus.mCarriages[0]);
                    mBus.mCarriages[0]->SpawnDriverPed();
                    mBus.mCarriages[0]->SetDriverPedType(ped_type::special_ped_4);
                    mBus.mCarriages[0]->InitCarAIControl();
                    mBus.mCarriages[0]->StartEngine();

                    sBusSpawned = true;
                    mBus.mStopPhase = 0;
                    mBus.mPassengerCount = 0;
                    mBus.mCarriages[0]->IncrementCarStats(car_kind::recycled_1);
                }
            }
            sBusSpawnDelay = 200;
        }

        pBusCar = mBus.mCarriages[0];
        if (pBusCar)
        {
            if (!mBus.mIsHijacked)
            {
                if (mBus.mCarriages[0]->IsDrivenByPlayer())
                {
                    mBus.mIsHijacked = true;
                    pBusCar->mDriver->mPlayer->mScores.AwardBusStolenScore(pBusCar);
                }
                else
                {
                    if (pBusCar->mDriver)
                    {
                        if (pBusCar->mDriver->get_occupation() != ped_ocupation_enum::dummy_driver_4 &&
                            pBusCar->mDriver->get_occupation() != ped_ocupation_enum::driver)
                        {
                            mBus.mIsHijacked = true;
                        }
                    }
                }
            }
            else
            {
                if (!mBus.mCarriages[0]->IsDrivenByPlayer())
                {
                    if (pBusCar->mDriver)
                    {
                        if (pBusCar->mDriver->get_occupation() == ped_ocupation_enum::angry_armed_robbed_driver_12)
                        {
                            mBus.mIsHijacked = false;
                            mBus.mPassengersFleeing = false;
                            pBusCar->mDriver->set_occupation(ped_ocupation_enum::driver);
                            mBus.mCarriages[0]->mDriver->SetPedType(ped_type::dummy_3);
                        }
                    }
                }
                if (!mBus.mPassengersFleeing && !mStopGettingOffBus)
                {
                    if (mBus.mCarriages[0]->GetCarLinearSpeed() > kTrainSpeedState1And3 + kFpPoint05_6FF04C)
                    {
                        mBus.mPassengersFleeing = true;
                        mBus.mCarriages[0]->mPassengersList.ApplyPassengerBusStopBehavior();
                    }
                }
            }

            if (mBus.mCarriages[0]->GetLastSeenTimer() > 200)
            {
                mBus.mCarriages[0]->MarkForDespawn();
                mBus.mCarriages[0] = NULL;
                sBusSpawned = false;
                sBusSpawnDelay = 100;
            }
            else
            {
                if (mBus.mCarriages[0]->IsDespawning() ||
                    mBus.mCarriages[0]->mDamage == 32000)
                {
                    mBus.mCarriages[0] = NULL;
                    sBusSpawned = false;
                    sBusSpawnDelay = 0;
                }
                else
                {
                    switch (mBus.mStopPhase)
                    {
                        case 12:
                            --mBus.mWaitTimer;
                            mBus.mCarriages[0]->HaltAIDriving();
                            if (!mBus.mWaitTimer)
                            {
                                mBus.mStopPhase = 5;
                                mBus.mWaitTimer = 10;
                            }
                            break;

                        case 5:
                            --mBus.mWaitTimer;
                            mBus.mCarriages[0]->OpenAllDoors();
                            if (!mBus.mWaitTimer)
                            {
                                mBus.mWaitTimer = 100;
                                mBus.mStopPhase = 13;
                            }
                            break;

                        case 13:
                            if (!mBus.mPassengersFleeing)
                            {
                                mBus.UpdatePassengerAI();
                            }
                            if (!mBus.mIsHijacked)
                            {
                                if (!--mBus.mWaitTimer)
                                {
                                    mBus.mStopPhase = 9;
                                    mBus.mWaitTimer = 10;
                                }
                            }
                            else if (mBus.mCarriages[0]->GetCarLinearSpeed() > kFpZero_6FF078)
                            {
                                mBus.mStopPhase = 9;
                                mBus.mWaitTimer = 10;
                            }
                            break;

                        case 9:
                            mBus.mCarriages[0]->CloseAllDoors();
                            if (!--mBus.mWaitTimer)
                            {
                                mBus.mStopPhase = 14;
                                mBus.mWaitTimer = 10;
                            }
                            break;

                        case 14:
                            mBus.mCarriages[0]->ResumeAIDriving();
                            if (!--mBus.mWaitTimer)
                            {
                                mBus.mStopPhase = 0;
                            }
                            break;
                        default:
                            break;
                    }
                    gSpriteGrid_2->CheckAndHandleCollisionInStrips(mBus.mCarriages[0]->mCarSprite);
                }
            }
        }
    }
}

// Tells the player driving the train which station it is heading for
static inline void SetDriverStation(Train* pTrain, TrainStation* pStation)
{
    Car* pCar = pTrain->mCarriages[0];
    if (pCar)
    {
        Ped* pDriver = pCar->mDriver;
        if (pDriver && pTrain->mWagonCount > 0)
        {
            pDriver->SetTrainStation(pStation);
        }
    }
}

MATCH_FUNC(0x57a7a0)
void PublicTransport::PublicTransportService()
{
    bool bStopped = false;
    BusesService();
    if (!bSkip_trains)
    {
        for (u16 i = 0; i < 10; i++)
        {
            Train* pTrain = &mTrainArray[i];
            if (!pTrain->mCarriages[0])
            {
                continue;
            }

            pTrain->ProcessTrainExplosionChain();
            Car* pCar = pTrain->mCarriages[0];
            if (pCar->IsDrivenByPlayer())
            {
                if (!pTrain->mCarriages[0]->mDriver)
                {
                    pCar->SetDriverPedType(ped_type::unknown_1);
                    pTrain->mIsHijacked = false;
                }
                else
                {
                    pTrain->mIsHijacked = true;
                    if (pCar->mDriver->GetMoveDirection() > kFpZero_6FF078)
                    {
                        if (pTrain->mCarriages[0]->mDriver->mPlayer->mForwardChanged)
                        {
                            pTrain->IncrementState();
                        }
                    }
                    else if (pTrain->mCarriages[0]->mDriver->GetMoveDirection() < kFpZero_6FF078)
                    {
                        if (pTrain->mCarriages[0]->mDriver->mPlayer->mDownChanged)
                        {
                            pTrain->DecrementState();
                        }
                    }
                }
            }

            switch (pTrain->mState)
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
            switch (pTrain->mStopPhase)
            {
                case 10:
                    pTrain->CloseCarriageDoors();
                    pStation = pTrain->mStation;
                    if (!pTrain->mIsHijacked)
                    {
                        pTrain->StartIfHasDriver();
                    }
                    else if (!bStopped)
                    {
                        if ((static_cast<u8>(pTrain->mCarriages[0]->mCarSprite->mXy.x.ToInt()) == pStation->mStopPoint->mX && static_cast<u8>(pTrain->mCarriages[0]->mCarSprite->mXy.y.ToInt()) == pStation->mStopPoint->mY))
                        {
                            pTrain->mStopPhase = 4;
                            break;
                        }
                        pTrain->mStopPhase = 11;
                    }
                    if ((static_cast<u8>(pTrain->mCarriages[0]->mCarSprite->mXy.x.ToInt()) == pStation->mExitPoint->mX && static_cast<u8>(pTrain->mCarriages[0]->mCarSprite->mXy.y.ToInt()) == pStation->mExitPoint->mY))
                    {
                        pTrain->mStopPhase = 0;
                        pTrain->IncrementState();
                        pStation = pTrain->mStation;
                        pStation->mpStoppedTrain = NULL;
                        pStation = pStation->mNextStation;
                        pTrain->mStation = pStation;
                        pStation->mPlatformState = 1;
                    }
                    break;

                case 11:
                    if (bStopped)
                    {
                        pTrain->mStopPhase = 10;
                    }
                    break;

                case 0:
                    pStation = pTrain->mStation;
                    if (!pTrain->mIsHijacked)
                    {
                        if (pStation->mpStoppedTrain)
                        {
                            pTrain->Stop();
                            pTrain->mStopPhase = 1;
                        }
                    }
                    else if (!bStopped)
                    {
                        pTrain->mStopPhase = 1;
                    }
                    if (!pTrain->mIsHijacked)
                    {
                        if ((static_cast<u8>(pTrain->mCarriages[0]->mCarSprite->mXy.x.ToInt()) == pStation->mEntryPoint->mX && static_cast<u8>(pTrain->mCarriages[0]->mCarSprite->mXy.y.ToInt()) == pStation->mEntryPoint->mY))
                        {
                            SetDriverStation(pTrain, pStation);
                            pTrain->mStopPhase = 2;
                            pTrain->DecrementState();
                        }
                        else if (pTrain->mState != 2 && bStopped)
                        {
                            pTrain->IncrementState();
                        }
                    }
                    break;

                case 1:
                    if (!pTrain->mIsHijacked)
                    {
                        if (!pTrain->mStation->mpStoppedTrain)
                        {
                            pTrain->StartIfHasDriver();
                            pTrain->mStopPhase = 0;
                            pTrain->IncrementState();
                        }
                    }
                    else if (bStopped)
                    {
                        pTrain->mStopPhase = 0;
                    }
                    break;

                case 2:
                    pStation = pTrain->mStation;
                    pStation->mpStoppedTrain = pTrain;
                    if ((static_cast<u8>(pTrain->mCarriages[0]->mCarSprite->mXy.x.ToInt()) == pStation->mStopPoint->mX && static_cast<u8>(pTrain->mCarriages[0]->mCarSprite->mXy.y.ToInt()) == pStation->mStopPoint->mY))
                    {
                        pTrain->mStopPhase = 4;
                    }
                    else if (pTrain->mIsHijacked == 1 && !bStopped)
                    {
                        pTrain->mStopPhase = 3;
                    }
                    break;

                case 3:
                    if (pTrain->mIsHijacked == 1 && bStopped)
                    {
                        pTrain->mStopPhase = 2;
                    }
                    break;

                case 4:
                    if (!pTrain->mIsHijacked)
                    {
                        pTrain->Stop();
                        pTrain->mStopPhase = 5;
                        pTrain->mWaitTimer = 10;
                        pTrain->OpenCarriageDoors();
                    }
                    else if (bStopped)
                    {
                        pTrain->mStopPhase = 10;
                    }
                    else
                    {
                        pTrain->mStopPhase = 5;
                        pTrain->OpenCarriageDoors();
                        pTrain->mWaitTimer = 10;
                    }
                    break;

                case 5:
                    pTrain->mWaitTimer--;
                    pTrain->OpenCarriageDoors();
                    if (pTrain->mIsHijacked == 1 && bStopped)
                    {
                        pTrain->mStopPhase = 10;
                    }
                    else if (pTrain->mWaitTimer == 0)
                    {
                        pStation = pTrain->mStation;
                        pTrain->mStopPhase = 6;
                        pTrain->mWaitTimer = 10;
                        pStation->mpStoppedTrain = pTrain;
                        pStation->mPlatformState = 4;
                    }
                    break;

                case 6:
                    if (pTrain->mIsHijacked == 1)
                    {
                        if (bStopped)
                        {
                            pTrain->mStopPhase = 10;
                            break;
                        }
                        pTrain->mWaitTimer--;
                        if (pTrain->mWaitTimer == 0)
                        {
                            pTrain->mWaitTimer = 50;
                            pTrain->mStopPhase = 7;
                            pTrain->mStation->mPlatformState = 2;
                        }
                    }
                    else
                    {
                        pTrain->mWaitTimer--;
                        if (pTrain->mWaitTimer == 0)
                        {
                            pTrain->mWaitTimer = 50;
                            pTrain->mStopPhase = 7;
                            pTrain->mStation->mPlatformState = 2;
                        }
                    }
                    break;

                case 7:
                    if (pTrain->mIsHijacked == 1)
                    {
                        if (bStopped)
                        {
                            pTrain->mStopPhase = 10;
                            break;
                        }
                        pTrain->UpdatePassengerAI();
                        pTrain->mWaitTimer--;
                        if (pTrain->mWaitTimer == 0)
                        {
                            pTrain->mStopPhase = 8;
                            pTrain->mWaitTimer = 50;
                            pTrain->mStation->mPlatformState = 2;
                        }
                    }
                    else
                    {
                        pTrain->UpdatePassengerAI();
                        pTrain->mWaitTimer--;
                        if (pTrain->mWaitTimer == 0)
                        {
                            pTrain->mStopPhase = 8;
                            pTrain->mWaitTimer = 50;
                            pTrain->mStation->mPlatformState = 2;
                        }
                    }
                    break;

                case 8:
                    if (pTrain->mIsHijacked == 1)
                    {
                        if (bStopped)
                        {
                            pTrain->mStopPhase = 10;
                            break;
                        }
                        pTrain->mWaitTimer--;
                        if (pTrain->mWaitTimer == 0)
                        {
                            pTrain->mStopPhase = 9;
                            pTrain->mWaitTimer = 10;
                            pTrain->mStation->mPlatformState = 3;
                        }
                    }
                    else
                    {
                        pTrain->mWaitTimer--;
                        if (pTrain->mWaitTimer == 0)
                        {
                            pStation = pTrain->mStation;
                            SetDriverStation(pTrain, pStation);
                            pTrain->mStopPhase = 9;
                            pTrain->mWaitTimer = 10;
                            pStation->mPlatformState = 3;
                        }
                    }
                    break;

                case 9:
                    pTrain->CloseCarriageDoors();
                    if (pTrain->mIsHijacked == 1)
                    {
                        if (bStopped)
                        {
                            pTrain->mStopPhase = 10;
                        }
                    }
                    else
                    {
                        pTrain->mWaitTimer--;
                        if (pTrain->mWaitTimer == 0)
                        {
                            pTrain->mStopPhase = 10;
                            pTrain->mStation->mPlatformState = 1;
                        }
                    }
                    break;
            }
        }
    }
}

MATCH_FUNC(0x57b4b0)
TrainStation* PublicTransport::TrainStationForZone(gmp_map_zone* pZone)
{
    TrainStation* pIter = &mStations[0];
    for (u16 i = 0; i < GTA2_COUNTOF(mStations); i++)
    {
        if (pIter->mpZone == pZone)
        {
            return pIter;
        }
        pIter++;
    }
    return NULL;
}

MATCH_FUNC(0x57b540)
Car* PublicTransport::GetLeadTrainCar(Car* pCar)
{
    if (!bSkip_trains && pCar->IsTrainModel())
    {
        return GetTrainFromCar(pCar)->mCarriages[0];
    }
    else
    {
        return NULL;
    }
}

MATCH_FUNC(0x57b5c0)
Train* PublicTransport::GetTrainFromCar(Car* pToFind)
{
    if (!bSkip_trains)
    {
        for (u8 i = 0; i < 10; i++)
        {
            Train* pTrain = &mTrainArray[i];
            Car* pWagon = pTrain->mCarriages[0];
            if (pWagon == pToFind)
            {
                return pTrain;
            }
            for (u8 wagon_idx = 0; wagon_idx < pTrain->mWagonCount; wagon_idx++)
            {
                pWagon = pTrain->mCarriages[wagon_idx + 1];
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
Train* PublicTransport::GetTrainFromCarExcludingLeadCar(Car* pToFind)
{
    if (!bSkip_trains)
    {
        for (u8 i = 0; i < 10; i++)
        {
            Train* pTrain = &mTrainArray[i];
            for (u8 wagon_idx = 0; wagon_idx < pTrain->mWagonCount; wagon_idx++)
            {
                Car* pWagon = pTrain->mCarriages[wagon_idx + 1];
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
bool PublicTransport::AreTrainsOnDifferentTracks(Car* pCar1, Car* pCar2)
{
    if (pCar1->IsTrainModel() && pCar2->IsTrainModel())
    {
        Train* pTrain1 = GetTrainFromCar(pCar1);
        Train* pTrain2 = GetTrainFromCar(pCar2);
        if (pTrain1->mTrackIdx != pTrain2->mTrackIdx)
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