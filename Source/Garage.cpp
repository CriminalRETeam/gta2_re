#include "Garage.hpp"
#include "block_side.hpp"
#include "Globals.hpp"
#include "Door.hpp"
#include "DoorPool.hpp"
#include "error.hpp"
#include "map_0x370.hpp"
#include "sprite.hpp"
#include "char.hpp"
#include "Player.hpp"
#include "Ped.hpp"
#include "CarPhysics.hpp"
#include "Car.hpp"

// Forward declarations: the functions below are in address order
EXTERN_GLOBAL(Ang16, kAng0_6FD25C);
EXTERN_GLOBAL(Ang16, kAng90_6FCFB0);
EXTERN_GLOBAL(Ang16, kAng180_6FD07E);
EXTERN_GLOBAL(Ang16, kAng270_6FD0A4);

DEFINE_GLOBAL(Garage*, gGarage_48, 0x6FD26C);

DEFINE_GLOBAL_INIT(Fix16, kFpTwo_6FD128, Fix16(2), 0x6FD128);
DEFINE_GLOBAL_INIT(Fix16, kFpQuarter_6FCF88, Fix16(0x1000, 0), 0x6FCF88);
DEFINE_GLOBAL_INIT(Fix16, kFpQuarter_6FD04C, kFpQuarter_6FCF88, 0x6FD04C);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpEighth, kFpQuarter_6FD04C / kFpTwo_6FD128, 0x6FCF60);
DEFINE_GLOBAL_INIT(Fix16, kFpOne64th_6FD0D8, Fix16(0x100, 0), 0x6FD0D8);
DEFINE_GLOBAL_INIT(Fix16, kFpOne64th_6FD1D8, kFpOne64th_6FD0D8, 0x6FD1D8);

MATCH_FUNC(0x4bbc60)
Garage::~Garage()
{
}

// The heading for a ped leaving through a door facing `face`.
MATCH_FUNC(0x5345E0)
EXPORT Ang16 __stdcall GetHeadingForDoorFace(s32 face)
{
    switch (face)
    {
        case block_side::bottom_4:
            return kAng0_6FD25C;
        case block_side::right_2:
            return kAng90_6FCFB0;
        case block_side::top_3:
            return kAng180_6FD07E;
        case block_side::left_1:
            return kAng270_6FD0A4;
    }
    return kAng0_6FD25C;
}

DEFINE_GLOBAL_INIT(Fix16, kFpOne_6FD124, Fix16(1), 0x6FD124);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_6FCF98, Fix16(0.5), 0x6FCF98);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint8, Fix16(0x3333, 0), 0x6FD218);

// Car::IsLongerThanOneBlock (9.6f), with this file's copy of the 1.0 constant
static inline bool IsLongerThanOneBlock(Car* pCar)
{
    if (pCar->mCarSprite->GetHeight() > kFpOne_6FD124)
    {
        return true;
    }
    return false;
}

// 9.6f 0x489B10
MATCH_FUNC(0x534650)
void Garage::ValidateParkCommand()
{
    // The result is unused (the out-of-line Add on a const Fix16)
    {
        const Fix16 door_z(this->mDoor->mPrimaryDoorData->mZ);
        door_z.Add(kFpEighth);
    }

    if (gMap_0x370->HasWallInArea(mParkXMin.ToInt(),
                                      (mParkXMax - kFpOne64th_6FD1D8).ToInt(),
                                      mParkYMin.ToInt(),
                                      (mParkYMax - kFpOne64th_6FD1D8).ToInt(),
                                      mDoor->mPrimaryDoorData->get_z()))
    {
        FatalError_4A38C0(Gta2Error::ErrorInSetupOfParkCommand,
                          "C:\\Splitting\\Gta2\\Source\\park.cpp",
                          62,
                          this->mDoor->mPrimaryDoorData->get_x(),
                          this->mDoor->mPrimaryDoorData->get_y(),
                          this->mDoor->mPrimaryDoorData->get_z());
    }
}

DEFINE_GLOBAL(Fix16, kFpZero_6FD120, 0x6FD120);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint1, Fix16(0x666, 0), 0x6FCF10);

DEFINE_GLOBAL_INIT(Ang16, kAng90_6FCFB0, Ang16(360), 0x6FCFB0);
DEFINE_GLOBAL_INIT(Ang16, kAng180_6FD07E, Ang16(720), 0x6FD07E);
DEFINE_GLOBAL_INIT(Ang16, kAng270_6FD0A4, Ang16(1080), 0x6FD0A4);
DEFINE_GLOBAL_INIT(Ang16, kAng0_6FD25C, Ang16(0), 0x6FD25C);

MATCH_FUNC(0x534700)
u8 Garage::ParkCarAtDoor(Car* pCar, Door* pDoor)
{
    mParkingCar = pCar;
    mWaitTicks = 0;
    mDoor = pDoor;
    mState = garage_state::waiting_for_car_1;
    mDriver = NULL;
    mSettleTicks = 30;
    mInPosition = false;
    if (mParkTicket == 255)
    {
        mParkTicket = 1;
    }
    else
    {
        mParkTicket++;
    }

    mLongCar = IsLongerThanOneBlock(pCar);

    u8 x;
    u8 y;
    u8 z;
    mDoor->get_door_xyz_face(&x, &y, &z, reinterpret_cast<u32*>(&mDoorFace));

    Fix16 w1;
    Fix16 w2;
    if (mLongCar)
    {
        // Assigned separately: `w2 = w1` lets VC6 merge the two and lose the second register (ebp)
        w1 = kFpTwo_6FD128;
        w2 = kFpTwo_6FD128;
    }
    else
    {
        w1 = kFpOne_6FD124;
        w2 = kFpOne_6FD124;
    }

    // The half size goes before the constant in the y sums (`Fix16(y) + w1 + 1`), the other order changes
    // VC6's operand order and register choices.
    // 9.6f: the park rectangle corners (field_18/1C, field_20/24) and the target (field_30/34) are
    // Fix16 pairs set with Fix16_Point_POD::SetXY
    switch (mDoorFace)
    {
        case block_side::left_1:
            if (mDoor->IsDoubleDoor())
            {
                reinterpret_cast<Fix16_Point_POD*>(&mParkXMin)->SetXY(Fix16(x), Fix16(y) - w1 - kFpOne_6FD124);
            }
            else
            {
                reinterpret_cast<Fix16_Point_POD*>(&mParkXMin)->SetXY(Fix16(x), Fix16(y) - w1);
            }
            reinterpret_cast<Fix16_Point_POD*>(&mParkXMax)->SetXY(Fix16(x) + kFpOne_6FD124 + w2, Fix16(y) + w1 + kFpOne_6FD124);
            reinterpret_cast<Fix16_Point_POD*>(&mTargetX)->SetXY(Fix16(x) - kFpPoint8, Fix16(y) + kFpHalf_6FCF98);
            break;
        case block_side::right_2:
            reinterpret_cast<Fix16_Point_POD*>(&mParkXMin)->SetXY(Fix16(x) - w2, Fix16(y) - w1);
            if (mDoor->IsDoubleDoor())
            {
                reinterpret_cast<Fix16_Point_POD*>(&mParkXMax)->SetXY(Fix16(x) + kFpOne_6FD124, Fix16(y) + w1 + kFpTwo_6FD128);
            }
            else
            {
                reinterpret_cast<Fix16_Point_POD*>(&mParkXMax)->SetXY(Fix16(x) + kFpOne_6FD124, Fix16(y) + w1 + kFpOne_6FD124);
            }
            reinterpret_cast<Fix16_Point_POD*>(&mTargetX)->SetXY(Fix16(x) + kFpOne_6FD124 + kFpPoint8, Fix16(y) + kFpHalf_6FCF98);
            break;
        case block_side::top_3:
            reinterpret_cast<Fix16_Point_POD*>(&mParkXMin)->SetXY(Fix16(x) - w1, Fix16(y));
            if (mDoor->IsDoubleDoor())
            {
                reinterpret_cast<Fix16_Point_POD*>(&mParkXMax)->SetXY(Fix16(x) + kFpTwo_6FD128 + w1, Fix16(y) + w2 + kFpOne_6FD124);
            }
            else
            {
                reinterpret_cast<Fix16_Point_POD*>(&mParkXMax)->SetXY(Fix16(x) + kFpOne_6FD124 + w1, Fix16(y) + w2 + kFpOne_6FD124);
            }
            reinterpret_cast<Fix16_Point_POD*>(&mTargetX)->SetXY(Fix16(x) + kFpHalf_6FCF98, Fix16(y) - kFpPoint8);
            break;
        case block_side::bottom_4:
            if (mDoor->IsDoubleDoor())
            {
                reinterpret_cast<Fix16_Point_POD*>(&mParkXMin)->SetXY(Fix16(x) - kFpOne_6FD124 - w1, Fix16(y) - w2);
            }
            else
            {
                reinterpret_cast<Fix16_Point_POD*>(&mParkXMin)->SetXY(Fix16(x) - w1, Fix16(y) - w2);
            }
            reinterpret_cast<Fix16_Point_POD*>(&mParkXMax)->SetXY(Fix16(x) + kFpOne_6FD124 + w1, Fix16(y) + kFpOne_6FD124);
            reinterpret_cast<Fix16_Point_POD*>(&mTargetX)->SetXY(Fix16(x) + kFpHalf_6FCF98, Fix16(y) + kFpOne_6FD124 + kFpPoint8);
            break;
    }

    ValidateParkCommand();
    return mParkTicket;
}

MATCH_FUNC(0x5349d0)
void Garage::GaragesService()
{
    u8 corner1;
    u8 corner2;
    Fix16_Point car_pos;

    switch (mState)
    {
        case garage_state::waiting_for_car_1:
        {
            if (!mParkingCar->mCarSprite)
            {
                mState = garage_state::park_finished_3;
                return;
            }
            if (mParkingCar->GetDriver() && mParkingCar->mDriver != mDriver)
            {
                mDriver = mParkingCar->GetDriver();
            }

            u8 collision = mParkingCar->mCarSprite->CountCornersInRect(&mParkXMin, &mParkXMax, &corner1, &corner2);
            if (collision == 2)
            {
                if (corner1 == 0)
                {
                    if (corner2 == 1)
                    {
                    }
                    else
                    {
                        return;
                    }
                }
                else
                {
                    if (corner1 != 2)
                    {
                        return;
                    }
                    // goto: the original lays the parked block as the jump target of both
                    // corner checks, each with its own inline epilogue after it.
                    if (corner2 == 3)
                    {
                        goto park;
                    }
                    return;
                }
            }
            else if (collision > 2)
            {
                if (collision >= 4)
                {
                    mState = garage_state::pushing_2;
                    return;
                }
            }
            else
            {
                return;
            }

        park:
            mParkingCar->SetNoCollide();
            mParkingCar->SetNoCollisionDamage();
            if (mParkingCar->IsDrivenByPlayer())
            {
                mParkingCar->GetDriverPlayer()->DisableAllControls();
            }
            mParkingCar->LockUnlessPermanent();
            mParkingCar->mPhysics->StopMoving();

            car_pos.SetXY(mParkingCar->GetX(), mParkingCar->GetY());
            SpriteShape* pBox = mParkingCar->mCarSprite->mpCollisionShape;
            mPushDir = (pBox->mCorners[corner1] + pBox->mCorners[corner2]).Divide(kFpTwo_6FD128) - car_pos;
            if (mPushDir.x == kFpZero_6FD120 && mPushDir.y == kFpZero_6FD120)
            {
                mInPosition = true;
                mState = garage_state::pushing_2;
            }
            else
            {
                mPushDir = mPushDir.NormalizeSafe().Multiply(kFpPoint1);
            }
            mParkingCar->mPhysics->ApplyForceScaledByMass(mPushDir);
            mState = garage_state::pushing_2;
            break;
        }

        case garage_state::pushing_2:
        {
            if (!mParkingCar->mPhysics)
            {
                mParkingCar->SetupCarPhysicsAndSpriteBindingWithTrailer();
            }
            if (++mWaitTicks >= 300)
            {
                mState = garage_state::park_finished_3;
                return;
            }
            if (mParkingCar->GetDriver() && !mParkingCar->IsDrivenByPlayer())
            {
                mParkingCar->GetDriver()->SetObjective(objectives_enum::wait_in_car_27);
            }

            if (mParkingCar->mCarSprite->CountCornersInRect(&mParkXMin, &mParkXMax, &corner1, &corner2) != 4 && !mInPosition)
            {
                mParkingCar->mPhysics->StopMoving();
                mParkingCar->mPhysics->ApplyForceScaledByMass(mPushDir);
                return;
            }

            mInPosition = true;
            --mSettleTicks;
            mParkingCar->mPhysics->StopMoving();
            if (mSettleTicks == 0)
            {
                mState = garage_state::park_finished_3;
                mParkingCar->mPhysics->StopMoving();
            }
            break;
        }

        case garage_state::park_finished_3:
        {
            if (mParkingCar->mCarSprite)
            {
                // 9.6f: Car 0x476230/0x4895E0 (clear f78 bits 2 and 8, inlined)
                mParkingCar->mFlags &= static_cast<u16>(~car_flag::NoCollide);
                mParkingCar->mFlags &= static_cast<u16>(~car_flag::NoCollisionDamage);
                mDoor->CloseDoors();
                mDoor->ClearF2C();
                mParkingCar->LockPermanently();
                mParkingCar->PrepareForExplosion();
                mParkingCar->mPassengersList.KillAndClearAllPeds();
                if (mParkingCar->IsDrivenByPlayer())
                {
                    mParkingCar->GetDriver()->mPlayer->EnableAllControls();
                }
                if (!mNoRespawn)
                {
                    Ped* pDriver = mParkingCar->GetDriver();
                    if (pDriver)
                    {
                        pDriver->StartPedWalking(mTargetX, mTargetY, mParkingCar->GetZ());
                        mParkingCar->ClearDriver();
                        pDriver->SetRotation(GetHeadingForDoorFace(mDoorFace));
                        mParkingCar->mDriver = NULL;
                        pDriver->mGameObject->mJumpOverTimer = 20;
                    }
                    else if (mDriver && mDriver->get_cam_x() >= mParkXMin && mDriver->get_cam_x() <= mParkXMax &&
                             mDriver->get_cam_y() >= mParkYMin && mDriver->get_cam_y() <= mParkYMax)
                    {
                        mDriver->StartPedWalking(mTargetX, mTargetY, mParkingCar->GetZ());
                        mDriver->SetRotation(GetHeadingForDoorFace(mDoorFace));
                    }
                }
                mParkingCar->SetDespawn3IfNot5_421490();
            }
            mParkedCar = mParkingCar;
            mParkedCarId = mParkedCar->mCarId;
            mParkingCar = NULL;
            Reset();
            break;
        }
    }
}

MATCH_FUNC(0x534e80)
Garage::Garage()
{
    mParkingCar = NULL;
    mDoor = NULL;
    mDriver = NULL;
    mState = garage_state::idle_0;
    mPushDir.x = 0;
    mPushDir.y = 0;
    mParkXMin = 0;
    mParkYMin = 0;
    mParkXMax = 0;
    mParkYMax = 0;
    mDoorFace = 0;
    mTargetX = 0;
    mTargetY = 0;
    mSettleTicks = 30;
    mInPosition = false;
    mParkTicket = 0;
    mNoRespawn = false;
    mLongCar = false;
    mParkedCar = NULL;
    mParkedCarId = 0;
}