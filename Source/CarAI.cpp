// This TU's copy of the Fix16_Point length zero (see Fix16_Point.hpp)
#define FIX16_POINT_ZERO kF16Zero

#include <assert.h>

#include "CarAI.hpp"
#include "CarPhysics.hpp"
#include "Car.hpp"
#include "Game.hpp"
#include "CarChaseTaskTable.hpp"
#include "Object_5C.hpp"
#include "PathFinder.hpp"
#include "Police.hpp"
#include "PublicTransport.hpp"
#include "SpriteGrid.hpp"
#include "RouteFinder.hpp"
#include "map_0x370.hpp"
#include "rng.hpp"
#include "block_face.hpp"

EXTERN_GLOBAL(Ang16, kAng0_677CE8);
DEFINE_GLOBAL_INIT(Fix16, kF16Zero, Fix16(0), 0x677B90);
DEFINE_GLOBAL(Fix16, gCurrCarAI_ypos, 0x677C30);
DEFINE_GLOBAL(Fix16, gCurrCarAI_xpos, 0x677C38);
DEFINE_GLOBAL(Fix16, gCurrCarAI_zpos, 0x677C48);
DEFINE_STATIC_GLOBAL(Fix16, sCurrCarAITargetZ, 0x6779F8);

DEFINE_GLOBAL_INIT(Fix16, kFpFour_677BA0, Fix16(4), 0x677BA0);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint02_677B5C, Fix16(0x147, 0), 0x677B5C);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint02, kFpPoint02_677B5C, 0x677AC4);

DEFINE_GLOBAL_INIT(Fix16, gF16fOne, Fix16(1), 0x677B94);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint1_6779B8, Fix16(0x666, 0), 0x6779B8);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint01_677B50, Fix16(163, 0), 0x677B50);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kDefaultTargetSpeed, kFpPoint1_6779B8, 0x677CB4);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kDefaultAcceleration, kFpPoint01_677B50, 0x6779A4);

DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint7, Fix16(0x2CCC, 0), 0x6779D4);
DEFINE_GLOBAL(CarAIPool*, gCarAI_78_Pool, 0x677CF8);

DEFINE_STATIC_GLOBAL(Fix16, sCurrCarAIVelocity, 0x677B00);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint05_677B60, Fix16(0x333, 0), 0x677B60);

DEFINE_STATIC_GLOBAL(bool, sCurrCarAIIsChasing, 0x677BBC);
DEFINE_STATIC_GLOBAL(u8, sCurrCarAIPedalsFree, 0x677B3C);
DEFINE_GLOBAL(u8, gCurrCarAI_Unused_677A78, 0x677A78);
DEFINE_STATIC_GLOBAL(bool, sbOnGradientSlope, 0x677C90);
DEFINE_STATIC_GLOBAL(bool, sCurrCarAIAllowTrafficSteering, 0x677A5D);
DEFINE_STATIC_GLOBAL(u8, sIgnoreTargetCarAhead, 0x677A6C);
DEFINE_STATIC_GLOBAL(u8, sCurrCarAITargetToSide, 0x677A94);
DEFINE_STATIC_GLOBAL(u8, sCurrCarAIWentStraight, 0x677A5C);
DEFINE_STATIC_GLOBAL(u8, sCurrCarAIDoSpeedControl, 0x677B8C);
DEFINE_GLOBAL(u8, gCurrCarAI_HasChaseUpdate_677CA8, 0x677CA8);
DEFINE_STATIC_GLOBAL(u8, sCurrCarAIArrowDirection, 0x677C06);

DEFINE_GLOBAL(Fix16, gCurrCarAI_ProbeX, 0x677A74);
DEFINE_GLOBAL(Fix16, gCurrCarAI_ProbeY, 0x677A80);
DEFINE_STATIC_GLOBAL(Fix16, sCurrCarAISpriteHeight, 0x677C9C);
DEFINE_STATIC_GLOBAL(Fix16, sCurrCarAIDesiredSpeed, 0x677A8C);
DEFINE_GLOBAL(Fix16, gCurrCarAI_TargetX, 0x6779F0);
DEFINE_STATIC_GLOBAL(Fix16, sCurrCarAITargetY, 0x6779F4);

DEFINE_GLOBAL(s32, gCurrCarAI_Unused_677C88, 0x677C88);
DEFINE_STATIC_GLOBAL(Car*, sCurrCarAITargetCar, 0x6779B0);

DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint6, Fix16(0x2666, 0), 0x6779D0);
DEFINE_STATIC_GLOBAL_INIT(Ang16, kAng10, Ang16(40), 0x677A08);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_6779CC, Fix16(0x2000, 0), 0x6779CC);

DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint4, Fix16(0x1999, 0), 0x6779C8);

DEFINE_GLOBAL_INIT(Fix16, kFpPoint2_6779C0, Fix16(0xCCC, 0), 0x6779C0);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpNine, Fix16(9), 0x677BB8);
DEFINE_GLOBAL_INIT(Fix16, kFpThree_677B9C, Fix16(3), 0x677B9C);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_677A84, Fix16(0x2000, 0), 0x677A84);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpThreeQuarters, Fix16(0x3000, 0), 0x677A4C);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint03, Fix16(0x1EB, 0), 0x677B58);
DEFINE_GLOBAL_INIT(Fix16, kFpTwo_677B98, Fix16(2), 0x677B98);
DEFINE_GLOBAL_INIT(Fix16, kFpQuarter_6779BC, Fix16(0x1000, 0), 0x6779BC);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpEighth, Fix16(0x800, 0), 0x677CBC);

// File-local: VC6 then propagates their value into the first use (ManageTrafficCarDirection)
static Ang16 kAng180_677ADE = Ang16(720);
GLOBAL(kAng180_677ADE, 0x677ADE)
static Ang16 kAng90_6779E4 = Ang16(360);
GLOBAL(kAng90_6779E4, 0x6779E4)
EXTERN_GLOBAL(Ang16, kAng12_677A2E);
// Defined in Car_10.cpp: with the definition (and its dynamic initialiser) in this TU, VC6
// loads it with a 32-bit mov in FollowRoadDirection instead of the original's 16-bit mov/sub.
// Same for kAng0 and kAng12_677A2E (16-bit sub/add in AlignToLaneCenter).
EXTERN_GLOBAL(Ang16, kAng4_677CE2);

DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpTwoPi, Fix16(0x18F60, 0), 0x677C84);
static Ang16 kAng270_677B08 = Ang16(1080);
GLOBAL(kAng270_677B08, 0x677B08)
DEFINE_GLOBAL_INIT(Fix16, kFpFive_677BA4, Fix16(5), 0x677BA4);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpFive, Fix16(5), 0x677CA0);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint09_677B78, Fix16(0x5C2, 0), 0x677B78);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint06, Fix16(0x3D7, 0), 0x677B70);

DEFINE_GLOBAL_INIT(Fix16, kFp255_677950, Fix16(255), 0x677950);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint8, Fix16(0x3333, 0), 0x6779D8);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint04, Fix16(0x28F, 0), 0x677B64);
DEFINE_GLOBAL_INIT(Ang16, kAng45_677A38, Ang16(180), 0x677A38);
DEFINE_GLOBAL_INIT(Ang16, kAng32, Ang16(128), 0x677A3A);

EXTERN_GLOBAL(u16, gBlockedRoadBrakeTimer);
EXTERN_GLOBAL(u8, gCarAIProbingAhead);

MATCH_FUNC(0x447650)
void __stdcall InitAIvars()
{
    gCurrCarAI_Unused_677A78 = 0;
    sbOnGradientSlope = false;
    sCurrCarAIAllowTrafficSteering = true;
    sCurrCarAIPedalsFree = 1;
    sCurrCarAIIsChasing = false;
    sIgnoreTargetCarAhead = 1;
    sCurrCarAITargetToSide = 0;
    sCurrCarAIWentStraight = 0;
    sCurrCarAIDoSpeedControl = 1;
    gCurrCarAI_HasChaseUpdate_677CA8 = 0;
    sCurrCarAIArrowDirection = 0;
    gCurrCarAI_xpos = kF16Zero;
    gCurrCarAI_ypos = kF16Zero;
    gCurrCarAI_zpos = kF16Zero;
    gCurrCarAI_ProbeX = kF16Zero;
    gCurrCarAI_ProbeY = kF16Zero;
    sCurrCarAIVelocity = kF16Zero;
    sCurrCarAISpriteHeight = kF16Zero;
    sCurrCarAIDesiredSpeed = kF16Zero;
    gCurrCarAI_TargetX = kF16Zero;
    sCurrCarAITargetY = kF16Zero;
    sCurrCarAITargetZ = kF16Zero;
    gCurrCarAI_Unused_677C88 = 1;
    sCurrCarAITargetCar = NULL;
}

MATCH_FUNC(0x4476f0)
void CarAI::MakeAgressiveSirensAndLights()
{
    if (mCar->IsPoliceCar())
    {
        mCar->HonkHorn();
    }
}

// Inlined in DoShortcutsUsingJunctions (no 9.6f function)
inline void CarAI::TurnAround()
{
    mCar->mPhysics->TurnAntiClockwise();
    mCar->mpChaseTask->mFollowingRoute = false;
    mCar->HaltAIDriving();
    mCar->mpChaseTask->mRouteFinished = true;
}

MATCH_FUNC(0x447710)
void CarAI::PlanTurnAtNextJunction()
{
    s8 arrow_run_len = 0;
    if (mRouteIdx >= 0)
    {
        Junction* pJunction =
            gRouteFinder->GetJunction(gRouteFinder->mRoutes[mRouteIdx].mJunctions[static_cast<u16>(this->mRoutePos)]);
        u16 next_junction_idx = gRouteFinder->mRoutes[this->mRouteIdx].mJunctions[static_cast<u16>(this->mRoutePos) + 1];
        if (next_junction_idx)
        {
            if (!this->mBf24.bFlag0)
            {
                s16 turn_dir = pJunction->GetDirectionToJunction(next_junction_idx);
                switch (Ang16::GetAngleFace(this->mAngle))
                {
                    case road_direction::up_1:
                        this->mCar->ClearTurnRight();
                        this->mCar->ClearTurnLeft();
                        switch (turn_dir)
                        {
                            case 2:
                                this->mJunctionTurnDirection = 2;
                                this->mBf24.bTurnPlanned = true;
                                break;
                            case 3:
                                this->mCar->SetTurnRight();
                                break;
                            case 4:
                                this->mCar->SetTurnLeft();
                                break;
                        }
                        break;

                    case road_direction::down_2:
                        this->mCar->ClearTurnRight();
                        this->mCar->ClearTurnLeft();
                        switch (turn_dir)
                        {
                            case 1:
                                this->mJunctionTurnDirection = 1;
                                this->mBf24.bTurnPlanned = true;
                                break;
                            case 3:
                                this->mCar->SetTurnLeft();
                                break;
                            case 4:
                                this->mCar->SetTurnRight();
                                break;
                        }
                        break;

                    case road_direction::right_3:
                        this->mCar->ClearTurnRight();
                        this->mCar->ClearTurnLeft();
                        switch (turn_dir)
                        {
                            case 1:
                                this->mCar->SetTurnRight();
                                break;
                            case 2:
                                this->mCar->SetTurnLeft();
                                break;
                            case 3:
                                this->mJunctionTurnDirection = 4;
                                this->mBf24.bTurnPlanned = true;
                                break;
                        }
                        break;

                    case road_direction::left_4:
                        this->mCar->ClearTurnRight();
                        this->mCar->ClearTurnLeft();
                        switch (turn_dir)
                        {
                            case 1:
                                this->mCar->SetTurnLeft();
                                break;
                            case 2:
                                this->mCar->SetTurnRight();
                                break;
                            case 4:
                                this->mJunctionTurnDirection = 3;
                                this->mBf24.bTurnPlanned = true;
                                break;
                        }
                        break;

                    default:
                        break;
                }

                char_type arrow_blocks_found = this->mCar->CountConsecutiveArrowBlocks(this->mAngle,
                                                                                 &arrow_run_len,
                                                                                 this->mCar->mCarSprite->mXy.x,
                                                                                 this->mCar->mCarSprite->mXy.y);
                if (arrow_run_len > -1)
                {
                    Car* pCar = this->mCar;
                    if (pCar->IsTurnRight())
                    {
                        if (arrow_blocks_found)
                        {
                            if (!pCar->IsTurnRightReady())
                            {
                                ClearTurnReady();
                                this->mCar->SetTurnRightReady();
                            }
                        }
                    }
                    else if (pCar->IsTurnLeft() && arrow_blocks_found != arrow_run_len - 1 && !pCar->IsTurnLeftReady())
                    {
                        ClearTurnReady();
                        this->mCar->SetTurnLeftReady();
                    }
                }
            }
        }
    }
}

MATCH_FUNC(0x447970)
void CarAI::DoShortcutsUsingJunctions()
{
    this->mFlags &= ~0xC0000u;

    u8 x = mCar->mpChaseTask->mTargetX.ToInt();
    u8 y = mCar->mpChaseTask->mTargetY.ToInt();
    u8 z = mCar->mpChaseTask->mTargetZ.ToInt();
    if (mRouteIdx > 0)
    {
        u16 junction_idx = gRouteFinder->mRoutes[mRouteIdx].mJunctions[static_cast<u16>(mRoutePos)];
        if (!junction_idx || x == static_cast<s16>(gCurrCarAI_xpos.ToInt()) && y == static_cast<s16>(gCurrCarAI_ypos.ToInt()))
        {
            gRouteFinder->CancelRoute(mRouteIdx);
            this->mRouteIdx = -1;
            this->mCar->HaltAIDriving();
            this->mCar->mpChaseTask->mRouteFinished = true;
            return;
        }

        Junction* pJunction = gRouteFinder->GetJunction(junction_idx);
        if (x > 0)
        {
            if (gRouteFinder->IsPointInJunctionBounds(
                    x,
                    y,
                    gRouteFinder->mRoutes[mRouteIdx].mJunctions[static_cast<u16>(mRoutePos)],
                    gRouteFinder->mRoutes[mRouteIdx].mJunctions[static_cast<u16>(mRoutePos) + 1]) &&
                z == static_cast<s16>((mCar->mCarSprite->mZpos - gF16fOne).ToInt()))
            {
                switch (static_cast<s16>(pJunction->GetDirectionToJunction(
                    gRouteFinder->mRoutes[mRouteIdx].mJunctions[static_cast<u16>(mRoutePos) + 1])))
                {
                    case 1:
                        if (y >= static_cast<u8>(gCurrCarAI_ypos.ToInt()))
                        {
                            TurnAround();
                            return;
                        }
                        if (x > static_cast<u8>(gCurrCarAI_xpos.ToInt()))
                        {
                            mBf24.bFlag18 = true;
                        }
                        else if (x < static_cast<u8>(gCurrCarAI_xpos.ToInt()))
                        {
                            mBf24.bFlag19 = true;
                        }
                        break;
                    case 2:
                        if (y <= static_cast<u8>(gCurrCarAI_ypos.ToInt()))
                        {
                            TurnAround();
                            return;
                        }
                        if (x > static_cast<u8>(gCurrCarAI_xpos.ToInt()))
                        {
                            mBf24.bFlag18 = true;
                        }
                        else if (x < static_cast<u8>(gCurrCarAI_xpos.ToInt()))
                        {
                            mBf24.bFlag19 = true;
                        }
                        break;
                    case 3:
                        if (x >= static_cast<u8>(gCurrCarAI_xpos.ToInt()))
                        {
                            TurnAround();
                            return;
                        }
                        if (y > static_cast<u8>(gCurrCarAI_ypos.ToInt()))
                        {
                            mBf24.bFlag19 = true;
                        }
                        else if (y < static_cast<u8>(gCurrCarAI_ypos.ToInt()))
                        {
                            mBf24.bFlag18 = true;
                        }
                        break;
                    case 4:
                        if (x <= static_cast<u8>(gCurrCarAI_xpos.ToInt()))
                        {
                            TurnAround();
                            return;
                        }
                        if (y > static_cast<u8>(gCurrCarAI_ypos.ToInt()))
                        {
                            mBf24.bFlag18 = true;
                        }
                        else if (y < static_cast<u8>(gCurrCarAI_ypos.ToInt()))
                        {
                            mBf24.bFlag19 = true;
                        }
                        break;
                }
            }
        }

        // 9.6f: Junction::ContainsPoint (0x40CEE0, inlined)
        if (pJunction->ContainsPoint(gCurrCarAI_xpos.ToInt(), gCurrCarAI_ypos.ToInt()))
        {
            this->mBf24.bInsideJunction = true;
            PlanTurnAtNextJunction();
        }
        else
        {
            if (mBf24.bInsideJunction)
            {
                this->mBf24.bInsideJunction = false;
            }
            PlanTurnAtNextJunction();
        }
    }
}

MATCH_FUNC(0x447ca0)
bool CarAI::GoToBlock(u8 x, u8 y, u8 z, s32 arrow_type)
{
    mRouteIdx = static_cast<char_type>(gRouteFinder->StartRoute(gCurrCarAI_xpos.ToInt(),
                                                               gCurrCarAI_ypos.ToInt(),
                                                               (gCurrCarAI_zpos - gF16fOne).ToInt(),
                                                               x,
                                                               y,
                                                               z,
                                                               arrow_type));
    if (mRouteIdx > 0)
    {
        mCar->mpChaseTask->mTargetX = Fix16(x);
        mCar->mpChaseTask->mTargetY = Fix16(y);
        mCar->mpChaseTask->mTargetZ = sCurrCarAITargetZ;
        mRoutePos = 0;
        return true;
    }
    return false;
}

MATCH_FUNC(0x447d40)
bool CarAI::TrySetTargetDirectionFromArrows(const gmp_block_info* pBlock)
{
    Ang16 angle = this->mAngle;

    switch (this->mCurrDirection)
    {
        case car_ai_direction::north_1:
            if (mCar->IsTurnLeft() || mCar->IsTurnRight())
            {
                if (mCar->IsTurnLeft())
                {
                    if ((pBlock->mArrows & block_arrow::green_right_2) && ComputeShortestAngleDelta(kAng90_6779E4, angle) > kAng45_677A38)
                    {
                        mTargetDirection = car_ai_target_direction::eastwards_3;
                        return true;
                    }
                }
                else
                {
                    if ((pBlock->mArrows & block_arrow::green_left_1) && ComputeShortestAngleDelta(kAng270_677B08, angle) > kAng45_677A38)
                    {
                        mTargetDirection = car_ai_target_direction::westwards_4;
                        return true;
                    }
                }
            }

            if ((pBlock->mArrows & block_arrow::green_mask) == block_arrow::green_left_1)
            {
                if (ComputeShortestAngleDelta(kAng270_677B08, angle) <= kAng45_677A38)
                {
                    return false;
                }
                mTargetDirection = car_ai_target_direction::westwards_4;
                return true;
            }

            if ((pBlock->mArrows & block_arrow::green_mask) != block_arrow::green_right_2)
            {
                return false;
            }

            if (ComputeShortestAngleDelta(kAng90_6779E4, angle) <= kAng45_677A38)
            {
                return false;
            }
            mTargetDirection = car_ai_target_direction::eastwards_3;
            return true;

        case car_ai_direction::south_2:
            if (mCar->IsTurnLeft() || mCar->IsTurnRight())
            {
                if (mCar->IsTurnLeft())
                {
                    if (gCurrCarAI_HasChaseUpdate_677CA8)
                    {
                        if ((pBlock->mArrows & block_arrow::left_mask) && ComputeShortestAngleDelta(kAng270_677B08, angle) > kAng45_677A38)
                        {
                            mTargetDirection = car_ai_target_direction::westwards_4;
                            return true;
                        }
                    }
                    else
                    {
                        if ((pBlock->mArrows & block_arrow::green_left_1) && ComputeShortestAngleDelta(kAng270_677B08, angle) > kAng45_677A38)
                        {
                            mTargetDirection = car_ai_target_direction::westwards_4;
                            return true;
                        }
                    }
                }
                else
                {
                    if (gCurrCarAI_HasChaseUpdate_677CA8)
                    {
                        // Note: the original compares against 270 here although it turns east
                        if ((pBlock->mArrows & block_arrow::right_mask) && ComputeShortestAngleDelta(kAng270_677B08, angle) > kAng45_677A38)
                        {
                            mTargetDirection = car_ai_target_direction::eastwards_3;
                            return true;
                        }
                    }
                    else
                    {
                        if ((pBlock->mArrows & block_arrow::green_right_2) && ComputeShortestAngleDelta(kAng90_6779E4, angle) > kAng45_677A38)
                        {
                            mTargetDirection = car_ai_target_direction::eastwards_3;
                            return true;
                        }
                    }
                }
            }

            if (gCurrCarAI_HasChaseUpdate_677CA8 && (pBlock->mArrows & block_arrow::red_mask) && !(pBlock->mArrows & block_arrow::green_mask))
            {
                if ((pBlock->mArrows & block_arrow::red_mask) == block_arrow::red_left_10)
                {
                    if (ComputeShortestAngleDelta(kAng270_677B08, angle) <= kAng45_677A38)
                    {
                        return false;
                    }
                    mTargetDirection = car_ai_target_direction::westwards_4;
                    return true;
                }

                if ((pBlock->mArrows & block_arrow::red_mask) != block_arrow::red_right_20)
                {
                    return false;
                }

                if (ComputeShortestAngleDelta(kAng90_6779E4, angle) <= kAng45_677A38)
                {
                    return false;
                }
                mTargetDirection = car_ai_target_direction::eastwards_3;
                return true;
            }

            if (!(pBlock->mArrows & block_arrow::green_mask) || (pBlock->mArrows & block_arrow::red_mask))
            {
                return false;
            }

            if ((pBlock->mArrows & block_arrow::green_mask) == block_arrow::green_left_1)
            {
                if (ComputeShortestAngleDelta(kAng270_677B08, angle) > kAng45_677A38)
                {
                    mTargetDirection = car_ai_target_direction::westwards_4;
                    return true;
                }
            }
            else if ((pBlock->mArrows & block_arrow::green_mask) == block_arrow::green_right_2)
            {
                if (ComputeShortestAngleDelta(kAng90_6779E4, angle) > kAng45_677A38)
                {
                    mTargetDirection = car_ai_target_direction::eastwards_3;
                    return true;
                }
            }
            return false;

        case car_ai_direction::west_4:
            if (mCar->IsTurnLeft() || mCar->IsTurnRight())
            {
                if (mCar->IsTurnLeft())
                {
                    if ((pBlock->mArrows & block_arrow::green_up_4) && ComputeShortestAngleDelta(kAng180_677ADE, angle) > kAng45_677A38)
                    {
                        mTargetDirection = car_ai_target_direction::northwards_1;
                        return true;
                    }
                }
                else
                {
                    if ((pBlock->mArrows & block_arrow::green_down_8) && ComputeShortestAngleDelta(kAng0_677CE8, angle) > kAng45_677A38)
                    {
                        mTargetDirection = car_ai_target_direction::southwards_2;
                        return true;
                    }
                }
            }

            if ((pBlock->mArrows & block_arrow::green_mask) == block_arrow::green_down_8)
            {
                if (ComputeShortestAngleDelta(kAng0_677CE8, angle) > kAng45_677A38)
                {
                    mTargetDirection = car_ai_target_direction::southwards_2;
                    return true;
                }
            }
            else if ((pBlock->mArrows & block_arrow::green_mask) == block_arrow::green_up_4)
            {
                if (ComputeShortestAngleDelta(kAng180_677ADE, angle) > kAng45_677A38)
                {
                    mTargetDirection = car_ai_target_direction::northwards_1;
                    return true;
                }
            }
            return false;

        case car_ai_direction::east_3:
            if (mCar->IsTurnLeft() || mCar->IsTurnRight())
            {
                if (mCar->IsTurnLeft())
                {
                    if ((pBlock->mArrows & block_arrow::green_down_8) && ComputeShortestAngleDelta(kAng0_677CE8, angle) > kAng45_677A38)
                    {
                        mTargetDirection = car_ai_target_direction::southwards_2;
                        return true;
                    }
                }
                else
                {
                    if ((pBlock->mArrows & block_arrow::green_up_4) && ComputeShortestAngleDelta(kAng180_677ADE, angle) > kAng45_677A38)
                    {
                        mTargetDirection = car_ai_target_direction::northwards_1;
                        return true;
                    }
                }
            }

            if ((pBlock->mArrows & block_arrow::green_mask) == block_arrow::green_down_8)
            {
                if (ComputeShortestAngleDelta(kAng0_677CE8, angle) > kAng45_677A38)
                {
                    mTargetDirection = car_ai_target_direction::southwards_2;
                    return true;
                }
            }
            else if ((pBlock->mArrows & block_arrow::green_mask) == block_arrow::green_up_4)
            {
                if (ComputeShortestAngleDelta(kAng180_677ADE, angle) > kAng45_677A38)
                {
                    mTargetDirection = car_ai_target_direction::northwards_1;
                    return true;
                }
            }
            return false;
    }
    return false;
}

MATCH_FUNC(0x448270)
bool CarAI::IsClockwiseTurning() const
{
    switch (mCurrDirection)
    {
        case car_ai_direction::north_1:
            if (mTargetDirection == car_ai_target_direction::eastwards_3)
            {
                return true;
            }
            break;
        case car_ai_direction::east_3:
            if (mTargetDirection == car_ai_target_direction::southwards_2)
            {
                return true;
            }
            break;
        case car_ai_direction::south_2:
            if (mTargetDirection == car_ai_target_direction::westwards_4)
            {
                return true;
            }
            break;
        case car_ai_direction::west_4:
            if (mTargetDirection == car_ai_target_direction::northwards_1)
            {
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

MATCH_FUNC(0x4482C0)
void CarAI::BrakeForBlockedRoadAhead()
{
    Sprite* pProbe = gObject_5C->mCollisionProbeSprite;
    u8 probe_steps = 0;
    Fix16 default_probe_x = Fix16(static_cast<u8>(gCurrCarAI_ProbeX.ToInt()));
    Fix16 default_probe_y = Fix16(static_cast<u8>(gCurrCarAI_ProbeY.ToInt()));
    if (gBlockedRoadBrakeTimer && !this->mBf24.bHasRightOfWay)
    {
        if (gBlockedRoadBrakeTimer > 0)
        {
            --gBlockedRoadBrakeTimer;
        }

        this->mCar->DoBrake();
        this->mManeuverActive = 0;
        this->mBf24.bTurning = false;
    }
    else
    {
        gBlockedRoadBrakeTimer = 50;
        u8 bSecondPass;
        if (!this->mBf24.bHasRightOfWay)
        {
            this->mProbeDirection = car_ai_direction::west_4;
            this->mProbeX = default_probe_x.ToInt();
            this->mProbeY = default_probe_y.ToInt();
        }
        else
        {
            Fix16 saved_z = gCurrCarAI_zpos;
            Fix16 saved_y = gCurrCarAI_ypos;

            mCar->mCarSprite->set_xyz_lazy(gCurrCarAI_xpos, saved_y, saved_z);

            Fix16 dx;
            Fix16 dy;
            Ang16::PolarToCartesian_41FC20(this->mAngle, gF16fOne, dx, dy);
            mCar->mCarSprite->set_xyz_lazy(mCar->mCarSprite->mXy.x + dx,
                                                                  mCar->mCarSprite->mXy.y + dy,
                                                                  mCar->mCarSprite->mZpos);

            Sprite* pNearest =
                gSpriteGrid_1->FindCollidingSpritePreferType(this->mCar->mCarSprite, sprite_types_enum::none_0);
            this->mNearestEntity = pNearest;

            if (pNearest)
            {
                if (pNearest->GetType() == sprite_types_enum::car_2)
                {
                    CarAI* pYieldAI = pNearest->mCarBcPtr->mAI;
                    if (pYieldAI)
                    {
                        if (pYieldAI->mYieldToCar)
                        {
                            pYieldAI->mYieldToCar = NULL;
                        }
                    }
                    if (mNearestEntity->mCarBcPtr->GetCarLinearSpeed() > kF16Zero)
                    {
                        mCar->DoBrake();
                    }
                }
            }
        }

        bSecondPass = 0;
        Fix16 probe_x;
        Fix16 probe_y;
        do
        {
            probe_x = Fix16(static_cast<u8>(this->mProbeX));
            probe_y = Fix16(static_cast<u8>(this->mProbeY));
            switch (this->mCurrDirection)
            {
                case car_ai_direction::north_1:
                    probe_y += kFpHalf_677A84;
                    if (!bSecondPass)
                    {
                        probe_x -= kFpTwo_677B98;
                        probe_steps = 3;
                    }
                    else
                    {
                        probe_x += kFpTwo_677B98;
                        probe_steps = 6;
                    }
                    this->mProbeDirection = car_ai_direction::west_4;
                    break;

                case car_ai_direction::south_2:
                    probe_y += kFpHalf_677A84;
                    if (!bSecondPass)
                    {
                        probe_steps = 3;
                        probe_x += kFpThree_677B9C;
                    }
                    else
                    {
                        probe_steps = 6;
                        probe_x -= gF16fOne;
                    }
                    this->mProbeDirection = car_ai_direction::east_3;
                    break;

                case car_ai_direction::east_3:
                    probe_x += kFpHalf_677A84;
                    if (!bSecondPass)
                    {
                        probe_y -= kFpTwo_677B98;
                        probe_steps = 3;
                    }
                    else
                    {
                        probe_y += kFpTwo_677B98;
                        probe_steps = 6;
                    }
                    this->mProbeDirection = car_ai_direction::north_1;
                    break;

                case car_ai_direction::west_4:
                    probe_x += kFpHalf_677A84;
                    if (!bSecondPass)
                    {
                        probe_steps = 3;
                        probe_y += kFpThree_677B9C;
                    }
                    else
                    {
                        probe_y -= kFpTwo_677B98;
                        probe_steps = 6;
                    }
                    this->mProbeDirection = car_ai_direction::south_2;
                    break;

                default:
                    break;
            }

            pProbe->set_xyz_lazy(probe_x, probe_y, mCar->mCarSprite->mZpos);
            pProbe->set_ang_lazy(kAng90_6779E4 + this->mAngle);
            pProbe->AllocCollisionShape(gF16fOne, gF16fOne, kFpPoint2_6779C0);
            gSpriteGrid_1->FindCollidingSpritePreferType(pProbe, sprite_types_enum::none_0); // result not used

            for (u8 i = 0; i < probe_steps; i++)
            {
                switch (this->mCurrDirection)
                {
                    case car_ai_direction::north_1:
                        probe_y -= gF16fOne;
                        break;
                    case car_ai_direction::south_2:
                        probe_y += gF16fOne;
                        break;
                    case car_ai_direction::east_3:
                        probe_x += gF16fOne;
                        break;
                    case car_ai_direction::west_4:
                        probe_x -= gF16fOne;
                        break;
                    default:
                        break;
                }

                pProbe->set_xyz_lazy(probe_x, probe_y, mCar->mCarSprite->mZpos);

                Sprite* pHit = gSpriteGrid_1->FindCollidingSpritePreferType(pProbe, sprite_types_enum::none_0);
                if (pHit && pHit->GetType() == sprite_types_enum::car_2)
                {
                    CarAI* pAi = pHit->mCarBcPtr->mAI;
                    if (pAi && !pAi->mBf24.bHasRightOfWay && pAi->mProbeDirection != this->mProbeDirection)
                    {
                        if (bSecondPass)
                        {
                            if (pAi->mCurrDirection == this->mProbeDirection)
                            {
                                pAi->mYieldToCar = this->mCar;
                                pAi->mYieldTimer = 5;
                                this->mManeuverActive = 0;
                                this->mBf24.bTurning = false;
                            }
                        }
                        else if (pAi->mCurrDirection != this->mProbeDirection)
                        {
                            pAi->mYieldToCar = this->mCar;
                            pAi->mYieldTimer = 5;
                            this->mManeuverActive = 0;
                            this->mBf24.bTurning = false;
                        }
                    }
                }
            }
        } while (++bSecondPass < 2);

        this->mBf24.bHasRightOfWay = true;
    }
}

// 9.6f 0x430650
MATCH_FUNC(0x448770)
void CarAI::CheckRoadAhead()
{
    Fix16 probe_dist = kF16Zero;
    gmp_block_info* pHalfAheadBlock = NULL;
    Sprite* pProbeSprite = gObject_5C->mCollisionProbeSprite;
    Sprite* pCarSprite = this->mCar->mCarSprite;
    gmp_block_info* pFarBlock = NULL;
    gmp_block_info* pProbeBlock = NULL;

    Fix16 sprite_x = pCarSprite->mXy.x;
    Fix16 sprite_y = pCarSprite->mXy.y;

    Fix16 restore_x = sprite_x;
    Fix16 restore_y = sprite_y;

    Fix16 x_off;
    Fix16 y_off;
    Ang16::PolarToCartesian_41FC20(this->mAngle, kFpHalf_677A84, x_off, y_off);

    gCurrCarAI_ProbeX = this->mCar->mCarSprite->mXy.x + x_off;
    gCurrCarAI_ProbeY = this->mCar->mCarSprite->mXy.y + y_off;

    if (gCurrCarAI_ProbeX > kF16Zero && gCurrCarAI_ProbeY > kF16Zero && gCurrCarAI_ProbeX < kFp255_677950 && gCurrCarAI_ProbeY < kFp255_677950)
    {
        pHalfAheadBlock = gMap_0x370->GetBlock(gCurrCarAI_ProbeX.ToInt(), gCurrCarAI_ProbeY.ToInt(), mCar->mCarSprite->mZpos.ToInt());
        if (pHalfAheadBlock && IS_GRADIENT_SLOPE(pHalfAheadBlock->mSlopeType)
            && !IS_AIR_TYPE(pHalfAheadBlock->mSlopeType))
        {
        }
        else
        {
            pHalfAheadBlock = gMap_0x370->GetBlock(gCurrCarAI_ProbeX.ToInt(),
                                                          gCurrCarAI_ProbeY.ToInt(),
                                                          (this->mCar->mCarSprite->mZpos.ToInt()) - 1);
        }
    }

    if (IsClockwiseTurning())
    {
        probe_dist = kFpPoint4;
    }
    else
    {
        probe_dist = kFpPoint8;
    }

    if (sCurrCarAIVelocity < kFpPoint04)
    {
        probe_dist = kFpPoint2_6779C0;
    }

    if (pHalfAheadBlock)
    {
        if (gGtx_0x106C->IsRoadTile(pHalfAheadBlock->mLid & block_face::tile_idx_mask) || this->mBf24.bHasRightOfWay)
        {
            if (!sCurrCarAIIsChasing)
            {
                ChooseRandomTurn();
            }

            if (this->mCar->IsTurnLeft())
            {
                Fix16 new_x = static_cast<u8>(gCurrCarAI_ProbeX.ToInt());
                Fix16 new_y = static_cast<u8>(gCurrCarAI_ProbeY.ToInt());

                switch (this->mCurrDirection)
                {
                    case car_ai_direction::north_1:
                        new_y -= kFpHalf_677A84;
                        new_x -= gF16fOne;
                        break;
                    case car_ai_direction::south_2:
                        new_y += gF16fOne;
                        new_x += kFpTwo_677B98;
                        break;
                    case car_ai_direction::east_3:
                        new_x += gF16fOne;
                        new_y += kFpTwo_677B98;
                        break;
                    case car_ai_direction::west_4:
                        new_x -= kFpHalf_677A84;
                        new_y -= kFpTwo_677B98;
                        break;
                    default:
                        break;
                }

                pProbeSprite->set_xyz_lazy(new_x, new_y, this->mCar->mCarSprite->mZpos);
                pProbeSprite->set_ang_lazy(kAng90_6779E4 + this->mAngle);
                pProbeSprite->AllocCollisionShape(gF16fOne, kFpHalf_677A84, kFpPoint2_6779C0);

                Sprite* pHit = gSpriteGrid_1->FindCollidingSpritePreferType(pProbeSprite, sprite_types_enum::none_0);
                if (pHit)
                {
                    if (pHit->GetType() == sprite_types_enum::car_2)
                    {
                        this->mCar->DoBrake();
                        this->mManeuverActive = 0;
                        this->mBf24.bTurning = false;
                    }
                }
            }
            else if (this->mCar->IsTurnRight())
            {
                BrakeForBlockedRoadAhead();
            }
        }
    }

    this->mCar->mCarSprite->set_xyz_lazy(restore_x, restore_y, mCar->mCarSprite->mZpos);

    mCar->mCarSprite->set_ang_lazy(this->mAngle);

    Fix16 probe_x_off;
    Fix16 probe_y_off;
    Ang16::PolarToCartesian_41FC20(this->mAngle, probe_dist, probe_x_off, probe_y_off);

    gCurrCarAI_ProbeX = this->mCar->mCarSprite->mXy.x + probe_x_off;
    gCurrCarAI_ProbeY = this->mCar->mCarSprite->mXy.y + probe_y_off;

    if (gCurrCarAI_ProbeX > kF16Zero && gCurrCarAI_ProbeY > kF16Zero && gCurrCarAI_ProbeX < kFp255_677950 && gCurrCarAI_ProbeY < kFp255_677950)
    {
        pProbeBlock = gMap_0x370->GetBlock(gCurrCarAI_ProbeX.ToInt(), gCurrCarAI_ProbeY.ToInt(), mCar->mCarSprite->mZpos.ToInt());
        if (pProbeBlock && IS_GRADIENT_SLOPE(pProbeBlock->mSlopeType)
            && !IS_AIR_TYPE(pProbeBlock->mSlopeType))
        {
        }
        else
        {
            pProbeBlock = gMap_0x370->GetBlock(gCurrCarAI_ProbeX.ToInt(),
                                                            gCurrCarAI_ProbeY.ToInt(),
                                                            (this->mCar->mCarSprite->mZpos.ToInt()) - 1);
        }
    }

    Fix16 far_x_off;
    Fix16 far_y_off;
    Ang16::PolarToCartesian_41FC20(this->mAngle, kFpThree_677B9C, far_x_off, far_y_off);

    gCurrCarAI_ProbeX = this->mCar->mCarSprite->mXy.x + far_x_off;
    gCurrCarAI_ProbeY = this->mCar->mCarSprite->mXy.y + far_y_off;

    if (gCurrCarAI_ProbeX > kF16Zero && gCurrCarAI_ProbeY > kF16Zero && gCurrCarAI_ProbeX < kFp255_677950 && gCurrCarAI_ProbeY < kFp255_677950)
    {
        pFarBlock =
            gMap_0x370->GetBlock(gCurrCarAI_ProbeX.ToInt(), gCurrCarAI_ProbeY.ToInt(), this->mCar->mCarSprite->mZpos.ToInt());
        if (pFarBlock && IS_GRADIENT_SLOPE(pFarBlock->mSlopeType) &&
            !IS_AIR_TYPE(pFarBlock->mSlopeType))
        {
        }
        else
        {
            pFarBlock = gMap_0x370->GetBlock(gCurrCarAI_ProbeX.ToInt(),
                                                              gCurrCarAI_ProbeY.ToInt(),
                                                              (this->mCar->mCarSprite->mZpos.ToInt()) - 1);
        }
    }

    if (!sbOnGradientSlope && !this->mManeuverActive)
    {
        if (!pProbeBlock)
        {
            this->mTargetDirection = car_ai_target_direction::go_straight_0;
        }
        else if (TrySetTargetDirectionFromArrows(pProbeBlock))
        {
            this->mManeuverActive = 20;
            this->mFlags &= ~0xC000u;
            this->mBf24.bTurning = true;
        }
        else if (!pFarBlock)
        {
            this->mTargetDirection = car_ai_target_direction::go_straight_0;
        }
        else if (TrySetTargetDirectionFromArrows(pFarBlock))
        {
            this->mBf24.bTargetSetFromArrows = true;
        }
        else
        {
            this->mTargetDirection = car_ai_target_direction::go_straight_0;
        }
    }
}

// https://decomp.me/scratch/NIiIz
// 9.6f 0x430CC0. ClearSteeringIfTurning (9.6f 0x42FE40) is called once and inlined three times in 10.5
MATCH_FUNC(0x448ce0)
void CarAI::ManageTrafficCarDirection()
{
    Ang16 target_angle = kAng0_677CE8;
    Ang16 cur_angle = kAng0_677CE8;
    Fix16 off;
    u8 frac;

    mCar->mCarSprite->set_xyz_lazy(gCurrCarAI_xpos, gCurrCarAI_ypos, gCurrCarAI_zpos);

    this->mBf24.bFlag10 = true;
    gmp_block_info* pBlock = gMap_0x370->GetBlock(mCar->mCarSprite->mXy.x.ToInt(),
                                                                 mCar->mCarSprite->mXy.y.ToInt(),
                                                                 mCar->mCarSprite->mZpos.ToInt());
    gmp_block_info* pArrowBlock = pBlock;
    if (pBlock != NULL && IS_GRADIENT_SLOPE(pBlock->mSlopeType) && !IS_AIR_TYPE(pBlock->mSlopeType))
    {
    }
    else
    {
        pArrowBlock = gMap_0x370->GetBlock(mCar->mCarSprite->mXy.x.ToInt(),
                                                 mCar->mCarSprite->mXy.y.ToInt(),
                                                 mCar->mCarSprite->mZpos.ToInt() - 1);
    }

    CheckRoadAhead();

    if (pArrowBlock != NULL)
    {
        switch (mCurrDirection)
        {
            case car_ai_direction::north_1:
            {
                if ((pArrowBlock->mArrows & block_arrow::green_up_4) != 0)
                {
                    mBf24.bFlag4 = true;
                }
                else
                {
                    if ((pArrowBlock->mArrows & block_arrow::red_up_40) == 0)
                    {
                        mBf24.bFlag4 = false;
                    }
                    else
                    {
                        mBf24.bFlag4 = true;
                    }
                }
                break;
            }

            case car_ai_direction::south_2:
            {
                if ((pArrowBlock->mArrows & block_arrow::green_down_8) != 0)
                {
                    mBf24.bFlag4 = true;
                }
                else
                {
                    if ((pArrowBlock->mArrows & block_arrow::red_down_80) == 0)
                    {
                        mBf24.bFlag4 = false;
                    }
                    else
                    {
                        mBf24.bFlag4 = true;
                    }
                }
                break;
            }

            case car_ai_direction::east_3:
            {
                if ((pArrowBlock->mArrows & block_arrow::green_right_2) != 0)
                {
                    mBf24.bFlag4 = true;
                }
                else
                {
                    if ((pArrowBlock->mArrows & block_arrow::red_right_20) == 0)
                    {
                        mBf24.bFlag4 = false;
                    }
                    else
                    {
                        mBf24.bFlag4 = true;
                    }
                }
                break;
            }

            case car_ai_direction::west_4:
            {
                if ((pArrowBlock->mArrows & block_arrow::green_left_1) != 0)
                {
                    mBf24.bFlag4 = true;
                }
                else
                {
                    if ((pArrowBlock->mArrows & block_arrow::red_up_40) == 0)
                    {
                        mBf24.bFlag4 = false;
                    }
                    else
                    {
                        mBf24.bFlag4 = true;
                    }
                }
                break;
            }

            default:
                mBf24.bFlag4 = false;
                break;
        }
    }
    Fix16 x_off;
    Fix16 y_off;

    Ang16& heading = mAngle;
    Ang16::PolarToCartesian_41FC20(heading, sCurrCarAISpriteHeight / kFpTwo_677B98, x_off, y_off);

    gCurrCarAI_ProbeX = mCar->mCarSprite->mXy.x + x_off;
    gCurrCarAI_ProbeY = mCar->mCarSprite->mXy.y + y_off;

    if (mBf24.bFlag4 != 0)
    {
        mBf24.bFlag3 = false;
        mBf24.bFlag4 = true;
        mBf24.bFlag16 = true;

        switch (mCurrDirection)
        {
            case car_ai_direction::north_1:
            {
                if (mBf24.bTurning != 0)
                {
                    mPrevTargetDirection = mTargetDirection;
                    if (mTargetDirection == car_ai_target_direction::northwards_1)
                    {
                        ClearSteeringIfTurning();
                        mBf24.bTurning = false;
                        mBf24.bHasRightOfWay = false;
                    }
                    else if (mTargetDirection == car_ai_target_direction::eastwards_3)
                    {
                        mCar->mPhysics->TurnClockwise();
                    }
                    else if (mTargetDirection == car_ai_target_direction::westwards_4)
                    {
                        mCar->mPhysics->TurnAntiClockwise();
                    }
                    break;
                }

                cur_angle = mAngle;
                mPrevTargetDirection = car_ai_target_direction::northwards_1;
                target_angle = kAng180_677ADE;
                if (cur_angle > target_angle - kAng10 && cur_angle < target_angle + kAng10)
                {
                    if (sCurrCarAIIsChasing != 0)
                    {
                        mManeuverActive = 0;
                    }

                    frac = mCar->mCarSprite->mXy.x.ToUInt8();
                    off = mCar->mCarSprite->mXy.x - Fix16(frac);
                    if (!(off <= kFpPoint4) && !(off >= kFpPoint6))
                    {
                        if (mAngle == kAng180_677ADE)
                        {
                            mManeuverActive = 0;
                            mCar->mPhysics->mDamagedUntilCycle = 0;
                            mBf24.bFlag10 = false;
                            mBf24.bFlag4 = true;
                        }
                        else
                        {
                            mCar->mPhysics->SetGoStraight();
                        }
                        break;
                    }

                    mBf24.bArrowSteerAssist = false;
                    mBf24.bFlag16 = false;
                    if (off <= kFpPoint4)
                    {
                        if (cur_angle > target_angle - kAng10)
                        {
                            if (cur_angle < target_angle - kAng4_677CE2)
                            {
                                mCar->mPhysics->SetGoStraight();
                            }
                            else
                            {
                                mCar->mPhysics->TurnClockwise();
                            }
                        }
                        else
                        {
                            mCar->mPhysics->TurnAntiClockwise();
                        }
                    }
                    else
                    {
                        if (cur_angle < target_angle + kAng10)
                        {
                            if (cur_angle > target_angle + kAng4_677CE2)
                            {
                                mCar->mPhysics->SetGoStraight();
                                break;
                            }
                            else
                            {
                                mCar->mPhysics->TurnAntiClockwise();
                            }
                        }
                        else
                        {
                            mCar->mPhysics->TurnClockwise();
                        }
                    }
                    break;
                }

                mBf24.bArrowSteerAssist = false;
                mBf24.bFlag16 = false;
                if (cur_angle > target_angle)
                {
                    mCar->mPhysics->TurnClockwise();
                }
                else
                {
                    mCar->mPhysics->TurnAntiClockwise();
                    return;
                }
                break;
            }

            case car_ai_direction::south_2:
            {
                if (mBf24.bTurning != 0)
                {
                    mPrevTargetDirection = mTargetDirection;
                    if (mTargetDirection != car_ai_target_direction::southwards_2)
                    {
                        if (mTargetDirection == car_ai_target_direction::westwards_4)
                        {
                            mCar->mPhysics->TurnClockwise();
                        }
                        else if (mTargetDirection == car_ai_target_direction::eastwards_3)
                        {
                            mCar->mPhysics->TurnAntiClockwise();
                        }
                    }
                    else
                    {
                        ClearSteeringIfTurning();
                        mBf24.bTurning = false;
                        mBf24.bHasRightOfWay = false;
                    }
                    return;
                }

                mPrevTargetDirection = car_ai_target_direction::southwards_2;
                cur_angle = mAngle + kAng90_6779E4;
                target_angle = kAng90_6779E4;
                if (cur_angle > (target_angle - kAng4_677CE2) - kAng4_677CE2 && cur_angle < (target_angle + kAng4_677CE2) + kAng4_677CE2)
                {
                    if (sCurrCarAIIsChasing != 0)
                    {
                        mManeuverActive = 0;
                    }

                    frac = mCar->mCarSprite->mXy.x.ToUInt8();
                    off = mCar->mCarSprite->mXy.x - Fix16(frac);
                    if (!(off <= kFpPoint4) && !(off >= kFpPoint6))
                    {
                        if (mAngle == kAng0_677CE8 && mNearestEntity == NULL)
                        {
                            mCar->mPhysics->mDamagedUntilCycle = 0;
                            mBf24.bFlag10 = false;
                            mBf24.bFlag4 = true;
                            mManeuverActive = 0;
                        }
                        else
                        {
                            mCar->mPhysics->SetGoStraight();
                        }
                        return;
                    }

                    mBf24.bArrowSteerAssist = false;
                    mBf24.bFlag16 = false;
                    if (off <= kFpPoint4)
                    {
                        if (cur_angle < target_angle + kAng10)
                        {
                            if (cur_angle > target_angle + kAng4_677CE2)
                            {
                                mCar->mPhysics->SetGoStraight();
                            }
                            else
                            {
                                mCar->mPhysics->TurnAntiClockwise();
                                return;
                            }
                        }
                        else
                        {
                            mCar->mPhysics->TurnClockwise();
                        }
                    }
                    else
                    {
                        if (cur_angle > target_angle - kAng10)
                        {
                            if (cur_angle < target_angle - kAng4_677CE2)
                            {
                                mCar->mPhysics->SetGoStraight();
                            }
                            else
                            {
                                mCar->mPhysics->TurnClockwise();
                            }
                        }
                        else
                        {
                            mCar->mPhysics->TurnAntiClockwise();
                        }
                    }
                    break;
                }

                mBf24.bArrowSteerAssist = false;
                mBf24.bFlag16 = false;
                if (cur_angle > target_angle)
                {
                    mCar->mPhysics->TurnClockwise();
                }
                else
                {
                    mCar->mPhysics->TurnAntiClockwise();
                }
                break;
            }

            case car_ai_direction::east_3:
            {
                cur_angle = mAngle;
                mPrevTargetDirection = car_ai_target_direction::eastwards_3;
                target_angle = kAng90_6779E4;
                if (mBf24.bTurning != 0)
                {
                    mPrevTargetDirection = mTargetDirection;
                    if (mTargetDirection != car_ai_target_direction::eastwards_3)
                    {
                        if (mTargetDirection == car_ai_target_direction::northwards_1)
                        {
                            mCar->mPhysics->TurnAntiClockwise();
                        }
                        else if (mTargetDirection == car_ai_target_direction::southwards_2)
                        {
                            mCar->mPhysics->TurnClockwise();
                        }
                    }
                    else
                    {
                        ClearSteeringIfTurning();
                        mBf24.bTurning = false;
                        mBf24.bHasRightOfWay = false;
                    }
                    return;
                }

                if (cur_angle > (target_angle - kAng4_677CE2) - kAng4_677CE2 && cur_angle < (target_angle + kAng4_677CE2) + kAng4_677CE2)
                {
                    if (sCurrCarAIIsChasing != 0)
                    {
                        mManeuverActive = 0;
                    }

                    frac = mCar->mCarSprite->mXy.y.ToUInt8();
                    off = mCar->mCarSprite->mXy.y - Fix16(frac);
                    if (!(off <= kFpPoint4) && !(off >= kFpPoint6))
                    {
                        if (mAngle == kAng90_6779E4)
                        {
                            mCar->mPhysics->mDamagedUntilCycle = 0;
                            mBf24.bFlag10 = false;
                            mBf24.bFlag4 = true;
                            mManeuverActive = 0;
                        }
                        else
                        {
                            mCar->mPhysics->SetGoStraight();
                            break;
                        }
                        return;
                    }

                    mBf24.bArrowSteerAssist = false;
                    mBf24.bFlag16 = false;
                    if (off <= kFpPoint4)
                    {
                        if (cur_angle < target_angle - kAng4_677CE2)
                        {
                            if (cur_angle > target_angle - kAng10)
                            {
                                mCar->mPhysics->SetGoStraight();
                            }
                            else
                            {
                                mCar->mPhysics->TurnAntiClockwise();
                            }
                        }
                        else
                        {
                            mCar->mPhysics->TurnClockwise();
                        }
                    }
                    else
                    {
                        if (cur_angle > target_angle + kAng4_677CE2)
                        {
                            if (cur_angle < target_angle + kAng10)
                            {
                                mCar->mPhysics->SetGoStraight();
                            }
                            else
                            {
                                mCar->mPhysics->TurnClockwise();
                            }
                        }
                        else
                        {
                            mCar->mPhysics->TurnAntiClockwise();
                        }
                    }
                    return;
                }

                mBf24.bArrowSteerAssist = false;
                mBf24.bFlag16 = false;
                if (cur_angle > target_angle)
                {
                    mCar->mPhysics->TurnClockwise();
                    return;
                }
                else
                {
                    mCar->mPhysics->TurnAntiClockwise();
                }
                break;
            }

            case car_ai_direction::west_4:
            {
                if (mBf24.bTurning != 0)
                {
                    mPrevTargetDirection = mTargetDirection;
                    if (mTargetDirection != car_ai_target_direction::westwards_4)
                    {
                        if (mTargetDirection == car_ai_target_direction::northwards_1)
                        {
                            mCar->mPhysics->TurnClockwise();
                        }
                        else if (mTargetDirection == car_ai_target_direction::southwards_2)
                        {
                            mCar->mPhysics->TurnAntiClockwise();
                            return;
                        }
                    }
                    else
                    {
                        ClearSteeringIfTurning();
                        mBf24.bTurning = false;
                        mBf24.bHasRightOfWay = false;
                    }
                    return;
                }

                cur_angle = mAngle;
                mPrevTargetDirection = car_ai_target_direction::westwards_4;
                target_angle = kAng270_677B08;
                if (cur_angle > (target_angle - kAng4_677CE2) - kAng4_677CE2 && cur_angle < (target_angle + kAng4_677CE2) + kAng4_677CE2)
                {
                    if (sCurrCarAIIsChasing != 0)
                    {
                        mManeuverActive = 0;
                    }

                    frac = mCar->mCarSprite->mXy.y.ToUInt8();
                    off = mCar->mCarSprite->mXy.y - Fix16(frac);
                    if (!(off <= kFpPoint4) && !(off >= kFpPoint6))
                    {
                        if (mAngle == kAng270_677B08)
                        {
                            mCar->mPhysics->mDamagedUntilCycle = 0;
                            mBf24.bFlag10 = false;
                            mBf24.bFlag4 = true;
                            mManeuverActive = 0;
                        }
                        else
                        {
                            mCar->mPhysics->SetGoStraight();
                        }
                        return;
                    }

                    mBf24.bArrowSteerAssist = false;
                    mBf24.bFlag16 = false;
                    if (off <= kFpPoint4)
                    {
                        if (cur_angle > target_angle + kAng4_677CE2)
                        {
                            if (cur_angle < target_angle + kAng10)
                            {
                                mCar->mPhysics->SetGoStraight();
                            }
                            else
                            {
                                mCar->mPhysics->TurnClockwise();
                            }
                        }
                        else
                        {
                            mCar->mPhysics->TurnAntiClockwise();
                        }
                    }
                    else
                    {
                        if (cur_angle < target_angle - kAng4_677CE2)
                        {
                            if (cur_angle > target_angle - kAng10)
                            {
                                mCar->mPhysics->SetGoStraight();
                                return;
                            }
                            else
                            {
                                mCar->mPhysics->TurnAntiClockwise();
                            }
                        }
                        else
                        {
                            mCar->mPhysics->TurnClockwise();
                        }
                    }
                    break;
                }

                mBf24.bArrowSteerAssist = false;
                mBf24.bFlag16 = false;
                if (cur_angle > target_angle)
                {
                    mCar->mPhysics->TurnClockwise();
                }
                else
                {
                    mCar->mPhysics->TurnAntiClockwise();
                }
                break;
            }

            default:
                break;
        }
    }
    else
    {
        FollowRoadDirection();
    }
}

// https://decomp.me/scratch/FqWRo
MATCH_FUNC(0x44a1f0)
void CarAI::FollowRoadDirection()
{
    Ang16 lane_angle = kAng0_677CE8;
    Ang16 travel_angle = kAng0_677CE8;
    Fix16 vec_x;
    Fix16 vec_y;

    vec_y = kF16Zero;
    mBf24.bArrowSteerAssist = false;

    Ang16 lane_angle_copy = lane_angle;

    gmp_block_info* pBlockAtPos = gMap_0x370->GetBlock(gCurrCarAI_ProbeX.ToInt(), gCurrCarAI_ProbeY.ToInt(), mCar->mCarSprite->mZpos.ToInt());
    if (!pBlockAtPos || !IS_GRADIENT_SLOPE(pBlockAtPos->mSlopeType) ||
        IS_AIR_TYPE(pBlockAtPos->mSlopeType))
    {
        pBlockAtPos = gMap_0x370->GetBlock(gCurrCarAI_ProbeX.ToInt(),
                                                           gCurrCarAI_ProbeY.ToInt(),
                                                           (mCar->mCarSprite->mZpos.ToInt()) - 1);
    }

    s32 road_direction = pBlockAtPos ? gMap_0x370->GetArrowDirectionFromBlock(pBlockAtPos, true) : 0;
    if (road_direction && mSeekingRoad)
    {
        mSeekingRoad = 0;
        mFlags &= ~0xC0;
        if (sCurrCarAIIsChasing)
        {
            mCar->mpChaseTask->mFollowingRoute = true;
        }
    }

    mBf24.bFlag10 = false;

    switch (road_direction)
    {
        case road_direction::up_1:
            switch (mCurrDirection)
            {
                case car_ai_direction::north_1:
                {
                    Fix16 lane_dx =
                        mCar->mCarSprite->mXy.x - Fix16(mCar->mCarSprite->mXy.x.ToUInt8());
                    if (lane_dx <= kFpPoint4 || lane_dx >= kFpPoint6)
                    {
                        mFlags &= 0xFFFEDFFF;

                        if (lane_dx <= kFpPoint4)
                        {
                            if (travel_angle > (lane_angle_copy - kAng10))
                            {
                                if (travel_angle < (lane_angle - kAng4_677CE2))
                                {
                                    mCar->mPhysics->SetGoStraight();
                                }
                                else
                                {
                                    mCar->mPhysics->TurnClockwise();
                                }
                            }
                            else
                            {
                                mCar->mPhysics->TurnAntiClockwise();
                            }
                        }
                        else if (travel_angle < (kAng10 + lane_angle_copy))
                        {
                            if (travel_angle > (lane_angle + kAng4_677CE2))
                            {
                                mCar->mPhysics->SetGoStraight();
                            }
                            else
                            {
                                mCar->mPhysics->TurnAntiClockwise();
                            }
                        }
                        else
                        {
                            mCar->mPhysics->TurnClockwise();
                        }
                    }

                    return;
                }

                case car_ai_direction::south_2:
                    mCar->mPhysics->TurnClockwise();
                    return;

                case car_ai_direction::east_3:
                    if (mBf24.bTurning && mTargetDirection == car_ai_target_direction::northwards_1)
                    {
                        mCar->mPhysics->TurnAntiClockwise();
                    }
                    else if (mAngle > kAng45_677A38)
                    {
                        if (mBf24.bReversing)
                        {
                            mCar->mPhysics->TurnClockwise();
                        }
                        else
                        {
                            mCar->mPhysics->TurnAntiClockwise();
                        }
                    }
                    else if (mBf24.bReversing)
                    {
                        mCar->mPhysics->TurnAntiClockwise();
                    }
                    else
                    {
                        mCar->mPhysics->TurnClockwise();
                    }

                    return;

                case car_ai_direction::west_4:
                    mCar->mPhysics->TurnClockwise();
                    return;

                default:
                    return;
            }
            return;

        case road_direction::down_2:

            if (mCurrDirection == car_ai_direction::west_4)
            {
                if (mAngle > kAng270_677B08)
                {
                    if (mBf24.bReversing)
                    {
                        mCar->mPhysics->TurnClockwise();
                    }
                    else
                    {
                        mCar->mPhysics->TurnAntiClockwise();
                    }
                }
                else
                {
                    if (mBf24.bReversing)
                    {
                        mCar->mPhysics->TurnAntiClockwise();
                    }
                    else
                    {
                        mCar->mPhysics->TurnClockwise();
                    }
                }
            }
            else if (mCurrDirection == car_ai_direction::east_3)
            {
                mCar->mPhysics->TurnClockwise();
            }
            else if (mCurrDirection == car_ai_direction::south_2)
            {
                mCar->mPhysics->SetGoStraight();
            }
            else if (mCurrDirection == car_ai_direction::north_1)
            {
                if (mBf24.bTurning && mTargetDirection == car_ai_target_direction::westwards_4)
                {
                    mCar->mPhysics->TurnAntiClockwise();
                }
                else
                {
                    mCar->mPhysics->TurnClockwise();
                }
            }
            return;

        case road_direction::right_3:
            switch (mCurrDirection)
            {
                case car_ai_direction::north_1:
                    if (mBf24.bReversing)
                    {
                        mCar->mPhysics->TurnAntiClockwise();
                    }
                    return;

                case car_ai_direction::south_2:
                    if (mAngle > kAng270_677B08)
                    {
                        mCar->mPhysics->TurnClockwise();
                    }
                    else
                    {
                        mCar->mPhysics->TurnAntiClockwise();
                    }
                    return;

                case car_ai_direction::east_3:
                {
                    Fix16 lane_dy = mCar->mCarSprite->mXy.y -
                        Fix16((mCar->mCarSprite->mXy.y.ToUInt8()));
                    if (lane_dy > kFpPoint4 && lane_dy < kFpPoint6)
                    {
                        mCar->mPhysics->SetGoStraight();
                    }
                    else
                    {
                        mFlags &= 0xFFFEDFFF;

                        if (lane_dy <= kFpPoint4)
                        {
                            if (travel_angle < Ang16(lane_angle.rValue - kAng4_677CE2.rValue, 0))
                            {
                                if (travel_angle > Ang16(lane_angle_copy.rValue - kAng10.rValue, 0))
                                {
                                    mCar->mPhysics->SetGoStraight();
                                    // This return (and the one in left_4/south_2) decide which identical tail VC6 keeps when cross-jumping
                                    return;
                                }
                                else
                                {
                                    mCar->mPhysics->TurnAntiClockwise();
                                }
                            }
                            else
                            {
                                mCar->mPhysics->TurnClockwise();
                            }
                        }
                        else
                        {
                            if (travel_angle > Ang16(lane_angle.rValue + kAng4_677CE2.rValue, 0))
                            {
                                if (travel_angle < Ang16(kAng10.rValue + lane_angle_copy.rValue, 0))
                                {
                                    mCar->mPhysics->SetGoStraight();
                                }
                                else
                                {
                                    mCar->mPhysics->TurnClockwise();
                                }
                            }
                            else
                            {
                                mCar->mPhysics->TurnAntiClockwise();
                            }
                        }
                    }
                    return;
                }

                case car_ai_direction::west_4:
                    if (mBf24.bTurning && mTargetDirection == car_ai_target_direction::southwards_2)
                    {
                        mCar->mPhysics->TurnAntiClockwise();
                    }
                    else
                    {
                        mCar->mPhysics->TurnClockwise();
                    }
                    return;

                default:
                    return;
            }

            return;

        case road_direction::left_4:

            switch (mCurrDirection)
            {
                case car_ai_direction::north_1:
                    if (mAngle > kAng180_677ADE)
                    {
                        mCar->mPhysics->TurnAntiClockwise();
                    }
                    else
                    {
                        mCar->mPhysics->TurnClockwise();
                    }
                    return;

                case car_ai_direction::south_2:
                    if (mBf24.bTurning && mTargetDirection == car_ai_target_direction::westwards_4)
                    {
                        mCar->mPhysics->TurnClockwise();
                        return;
                    }
                    else
                    {
                        mCar->mPhysics->TurnAntiClockwise();
                    }
                    return;

                case car_ai_direction::east_3:
                    if (mBf24.bTurning && mTargetDirection == car_ai_target_direction::northwards_1)
                    {
                        mCar->mPhysics->TurnAntiClockwise();
                    }
                    else
                    {
                        mCar->mPhysics->TurnClockwise();
                    }
                    return;
            }
            return;

        default:

            mBf24.bFlag3 = true;
            if (!mSeekingRoad)
            {
                mSeekingRoad = 1;
                for (u8 k = 0; k < 4; k++)
                {
                    Fix16 xpos = gCurrCarAI_ProbeX;
                    Fix16 ypos = gCurrCarAI_ProbeY;
                    Fix16 zpos = mCar->mCarSprite->mZpos;

                    switch (k)
                    {
                        case 0:
                            Ang16::PolarToCartesian_41FC20(mAngle, gF16fOne, vec_x, vec_y);
                            break;
                        case 1:
                            Ang16::PolarToCartesian_41FC20(mAngle + kAng90_6779E4, gF16fOne, vec_x, vec_y);
                            break;
                        case 2:
                            Ang16::PolarToCartesian_41FC20(mAngle - kAng90_6779E4, gF16fOne, vec_x, vec_y);
                            break;
                        case 3:
                            Ang16::PolarToCartesian_41FC20(mAngle + kAng180_677ADE, gF16fOne, vec_x, vec_y);
                            break;
                    }

                    xpos += vec_x;
                    ypos += vec_y;

                    gmp_block_info* pBlock = gMap_0x370->GetBlock(xpos.ToInt(), ypos.ToInt(), zpos.ToInt());
                    if (!pBlock || !IS_GRADIENT_SLOPE(pBlock->mSlopeType) ||
                        IS_AIR_TYPE(pBlock->mSlopeType))
                    {
                        pBlock = gMap_0x370->GetBlock(xpos.ToInt(), ypos.ToInt(), zpos.ToInt() - 1);
                    }
                    if (pBlock && gMap_0x370->GetArrowDirectionFromBlock(pBlock, true))
                    {
                        mFlags &= ~0xC0;
                        switch (k)
                        {
                            case 0:
                                mSeekRoadTurn = 0;
                                break;
                            case 1:
                                mSeekRoadTurn = 1;
                                break;
                            case 2:
                                mSeekRoadTurn = 2;
                                break;
                            case 3:
                                mSeekRoadTurn = 3;
                                break;
                        }
                        return;
                    }
                }
            }
            else if (mSeekRoadTurn == 3)
            {
                mCar->mPhysics->SetGoStraight();
            }
    }
}

// The lane angles are Ang16 operator-/operator+ temporaries. Each gets its own stack slot, and past the
// inline budget their Normalize is called out of line (0x406C20) on it. The angle global is the left
// operand (this) in every sum: with kAng12_677A2E as `this` in one site and the argument in another, VC6
// loads both with 32-bit movs instead of a 16-bit mov/sub.
MATCH_FUNC(0x44af00)
void CarAI::AlignToLaneCenter()
{
    s8 arrow_run_len = 0;
    if (this->mBf24.bChangingLane)
    {
        sCurrCarAIAllowTrafficSteering = false;
        s8 count = this->mCar->CountConsecutiveArrowBlocks(this->mAngle,
                                                                         &arrow_run_len,
                                                                         gCurrCarAI_xpos,
                                                                         gCurrCarAI_ypos);

        this->mCar->mCarSprite->set_xyz_lazy(gCurrCarAI_xpos,
                                                                    gCurrCarAI_ypos,
                                                                    gCurrCarAI_zpos);

        if (count == static_cast<u8>(this->mArrowEnd))
        {
            switch (this->mCurrDirection)
            {
                case car_ai_direction::north_1:
                {
                    Fix16 off = this->mCar->mCarSprite->mXy.x -
                        Fix16(this->mCar->mCarSprite->mXy.x.ToUInt8());
                    if (off >= kFpPoint4 && off <= kFpPoint6)
                    {
                        sCurrCarAIAllowTrafficSteering = true;
                        this->mBf24.bFlag16 = true;
                    }
                    else if (off < kFpPoint4)
                    {
                        if (this->mAngle > kAng180_677ADE - kAng12_677A2E)
                        {
                            this->mCar->mPhysics->TurnClockwise();
                        }
                        else
                        {
                            this->mCar->mPhysics->SetGoStraight();
                        }
                    }
                    else
                    {
                        if (this->mAngle < kAng180_677ADE + kAng12_677A2E)
                        {
                            this->mCar->mPhysics->TurnAntiClockwise();
                        }
                        else
                        {
                            this->mCar->mPhysics->SetGoStraight();
                        }
                    }
                    break;
                }

                case car_ai_direction::south_2:
                {
                    Fix16 off = this->mCar->mCarSprite->mXy.x -
                        Fix16(this->mCar->mCarSprite->mXy.x.ToUInt8());
                    if (off >= kFpPoint4 && off <= kFpPoint6)
                    {
                        sCurrCarAIAllowTrafficSteering = true;
                        this->mBf24.bFlag16 = true;
                    }
                    else if (off < kFpPoint4)
                    {
                        if (this->mAngle < kAng12_677A2E)
                        {
                            this->mCar->mPhysics->TurnAntiClockwise();
                        }
                        else
                        {
                            this->mCar->mPhysics->SetGoStraight();
                        }
                    }
                    else
                    {
                        if (this->mAngle > kAng0_677CE8 - kAng12_677A2E)
                        {
                            this->mCar->mPhysics->TurnClockwise();
                        }
                        else
                        {
                            this->mCar->mPhysics->SetGoStraight();
                        }
                    }
                    break;
                }

                case car_ai_direction::east_3:
                {
                    Fix16 off = this->mCar->mCarSprite->mXy.y -
                        Fix16(this->mCar->mCarSprite->mXy.y.ToUInt8());
                    if (off > kFpPoint4 && off < kFpPoint6)
                    {
                        sCurrCarAIAllowTrafficSteering = true;
                        this->mBf24.bFlag16 = true;
                    }
                    else if (off <= kFpPoint4)
                    {
                        if (this->mAngle > kAng90_6779E4 - kAng12_677A2E)
                        {
                            this->mCar->mPhysics->TurnClockwise();
                        }
                        else
                        {
                            this->mCar->mPhysics->SetGoStraight();
                        }
                    }
                    else
                    {
                        if (this->mAngle < kAng90_6779E4 + kAng12_677A2E)
                        {
                            this->mCar->mPhysics->TurnAntiClockwise();
                        }
                        else
                        {
                            this->mCar->mPhysics->SetGoStraight();
                        }
                    }
                    break;
                }

                case car_ai_direction::west_4:
                {
                    Fix16 off = this->mCar->mCarSprite->mXy.y -
                        Fix16(this->mCar->mCarSprite->mXy.y.ToUInt8());
                    if (off > kFpPoint4 && off < kFpPoint6)
                    {
                        sCurrCarAIAllowTrafficSteering = true;
                        this->mBf24.bFlag16 = true;
                    }
                    else if (off <= kFpPoint4)
                    {
                        if (this->mAngle < kAng270_677B08 - kAng12_677A2E)
                        {
                            this->mCar->mPhysics->TurnAntiClockwise();
                        }
                        else
                        {
                            this->mCar->mPhysics->SetGoStraight();
                        }
                    }
                    else
                    {
                        if (this->mAngle > kAng270_677B08 + kAng12_677A2E)
                        {
                            this->mCar->mPhysics->TurnClockwise();
                        }
                        else
                        {
                            this->mCar->mPhysics->SetGoStraight();
                        }
                    }
                    break;
                }

                default:
                    break;
            }

            if (mBf24.bFlag4 && mBf24.bFlag16)
            {
                this->mBf24.bChangingLane = false;
            }
        }
        else if (count < static_cast<u8>(this->mArrowEnd))
        {
            // Move the sprite half a block to the right of the travel direction and check that it is free
            Fix16 x_off;
            Fix16 y_off;
            switch (this->mCurrDirection)
            {
                case car_ai_direction::east_3:
                {
                    Ang16::PolarToCartesian_41FC20(kAng0_677CE8, kFpHalf_6779CC, x_off, y_off);
                    mCar->mCarSprite->set_xyz_lazy(mCar->mCarSprite->mXy.x + x_off,
                                                                          mCar->mCarSprite->mXy.y + y_off,
                                                                          mCar->mCarSprite->mZpos);

                    if (gSpriteGrid_1->FindCollidingSpritePreferType(this->mCar->mCarSprite, sprite_types_enum::none_0))
                    {
                        sCurrCarAIAllowTrafficSteering = true;
                    }
                    else
                    {
                        if (this->mAngle > kAng90_6779E4 - kAng12_677A2E)
                        {
                            this->mCar->mPhysics->TurnClockwise();
                        }
                        else
                        {
                            this->mCar->mPhysics->SetGoStraight();
                        }
                    }
                    break;
                }

                case car_ai_direction::west_4:
                {
                    Ang16::PolarToCartesian_41FC20(kAng180_677ADE, kFpHalf_6779CC, x_off, y_off);
                    mCar->mCarSprite->set_xyz_lazy(mCar->mCarSprite->mXy.x + x_off,
                                                                          mCar->mCarSprite->mXy.y + y_off,
                                                                          mCar->mCarSprite->mZpos);

                    if (gSpriteGrid_1->FindCollidingSpritePreferType(this->mCar->mCarSprite, sprite_types_enum::none_0))
                    {
                        sCurrCarAIAllowTrafficSteering = true;
                    }
                    else
                    {
                        if (this->mAngle > kAng270_677B08 - kAng12_677A2E)
                        {
                            this->mCar->mPhysics->TurnClockwise();
                        }
                        else
                        {
                            this->mCar->mPhysics->SetGoStraight();
                        }
                    }
                    break;
                }

                case car_ai_direction::south_2:
                {
                    Ang16::PolarToCartesian_41FC20(kAng270_677B08, kFpHalf_6779CC, x_off, y_off);
                    mCar->mCarSprite->set_xyz_lazy(mCar->mCarSprite->mXy.x + x_off,
                                                                          mCar->mCarSprite->mXy.y + y_off,
                                                                          mCar->mCarSprite->mZpos);

                    if (gSpriteGrid_1->FindCollidingSpritePreferType(this->mCar->mCarSprite, sprite_types_enum::none_0))
                    {
                        sCurrCarAIAllowTrafficSteering = true;
                    }
                    else
                    {
                        if (this->mAngle <= kAng0_677CE8 - kAng12_677A2E && this->mAngle >= kAng90_6779E4)
                        {
                            this->mCar->mPhysics->SetGoStraight();
                        }
                        else
                        {
                            this->mCar->mPhysics->TurnClockwise();
                        }
                    }
                    break;
                }

                case car_ai_direction::north_1:
                {
                    Ang16::PolarToCartesian_41FC20(kAng90_6779E4, kFpHalf_6779CC, x_off, y_off);
                    mCar->mCarSprite->set_xyz_lazy(mCar->mCarSprite->mXy.x + x_off,
                                                                          mCar->mCarSprite->mXy.y + y_off,
                                                                          mCar->mCarSprite->mZpos);

                    if (gSpriteGrid_1->FindCollidingSpritePreferType(this->mCar->mCarSprite, sprite_types_enum::none_0))
                    {
                        sCurrCarAIAllowTrafficSteering = true;
                    }
                    else
                    {
                        if (this->mAngle > kAng180_677ADE - kAng12_677A2E)
                        {
                            this->mCar->mPhysics->TurnClockwise();
                        }
                        else
                        {
                            this->mCar->mPhysics->SetGoStraight();
                        }
                    }
                    break;
                }

                default:
                    break;
            }
        }
        else
        {
            // Same, half a block to the left
            Fix16 x_off;
            Fix16 y_off;
            switch (this->mCurrDirection)
            {
                case car_ai_direction::east_3:
                {
                    Ang16::PolarToCartesian_41FC20(kAng180_677ADE, kFpHalf_6779CC, x_off, y_off);
                    mCar->mCarSprite->set_xyz_lazy(mCar->mCarSprite->mXy.x + x_off,
                                                                          mCar->mCarSprite->mXy.y + y_off,
                                                                          mCar->mCarSprite->mZpos);

                    if (gSpriteGrid_1->FindCollidingSpritePreferType(this->mCar->mCarSprite, sprite_types_enum::none_0))
                    {
                        sCurrCarAIAllowTrafficSteering = true;
                    }
                    else
                    {
                        if (this->mAngle < kAng90_6779E4 + kAng12_677A2E)
                        {
                            this->mCar->mPhysics->TurnAntiClockwise();
                        }
                        else
                        {
                            this->mCar->mPhysics->SetGoStraight();
                        }
                    }
                    break;
                }

                case car_ai_direction::west_4:
                {
                    Ang16::PolarToCartesian_41FC20(kAng0_677CE8, kFpHalf_6779CC, x_off, y_off);
                    mCar->mCarSprite->set_xyz_lazy(mCar->mCarSprite->mXy.x + x_off,
                                                                          mCar->mCarSprite->mXy.y + y_off,
                                                                          mCar->mCarSprite->mZpos);

                    if (gSpriteGrid_1->FindCollidingSpritePreferType(this->mCar->mCarSprite, sprite_types_enum::none_0))
                    {
                        sCurrCarAIAllowTrafficSteering = true;
                    }
                    else
                    {
                        if (this->mAngle < kAng270_677B08 + kAng12_677A2E)
                        {
                            this->mCar->mPhysics->TurnAntiClockwise();
                        }
                        else
                        {
                            this->mCar->mPhysics->SetGoStraight();
                        }
                    }
                    break;
                }

                case car_ai_direction::south_2:
                {
                    Ang16::PolarToCartesian_41FC20(kAng90_6779E4, kFpHalf_6779CC, x_off, y_off);
                    mCar->mCarSprite->set_xyz_lazy(mCar->mCarSprite->mXy.x + x_off,
                                                                          mCar->mCarSprite->mXy.y + y_off,
                                                                          mCar->mCarSprite->mZpos);

                    if (gSpriteGrid_1->FindCollidingSpritePreferType(this->mCar->mCarSprite, sprite_types_enum::none_0))
                    {
                        sCurrCarAIAllowTrafficSteering = true;
                    }
                    else
                    {
                        if (this->mAngle < kAng0_677CE8 + kAng12_677A2E)
                        {
                            this->mCar->mPhysics->TurnAntiClockwise();
                        }
                        else
                        {
                            this->mCar->mPhysics->SetGoStraight();
                        }
                    }
                    break;
                }

                case car_ai_direction::north_1:
                {
                    Ang16::PolarToCartesian_41FC20(kAng270_677B08, kFpHalf_6779CC, x_off, y_off);
                    mCar->mCarSprite->set_xyz_lazy(mCar->mCarSprite->mXy.x + x_off,
                                                                          mCar->mCarSprite->mXy.y + y_off,
                                                                          mCar->mCarSprite->mZpos);

                    if (gSpriteGrid_1->FindCollidingSpritePreferType(this->mCar->mCarSprite, sprite_types_enum::none_0))
                    {
                        sCurrCarAIAllowTrafficSteering = true;
                    }
                    else
                    {
                        if (this->mAngle < kAng180_677ADE + kAng12_677A2E)
                        {
                            this->mCar->mPhysics->TurnAntiClockwise();
                        }
                        else
                        {
                            this->mCar->mPhysics->SetGoStraight();
                        }
                    }
                    break;
                }

                default:
                    break;
            }
        }
    }
}

MATCH_FUNC(0x44d1d0)
void CarAI::DetectCarAhead()
{
    Fix16 x_off;
    Fix16 y_off;
    s8 arrow_count = 0;
    char_type arrow_idx;
    bool bSlowerThanTarget = false;

    sCurrCarAITargetToSide = 0;
    if (this->mCar->mCarInfoIdx == car_model_enum::BUS)
    {
        return;
    }

    if (this->mTicksSinceAlloc <= 20)
    {
        return;
    }

    if (sbOnGradientSlope)
    {
        return;
    }

    if (!mBf24.bFlag4)
    {
        return;
    }

    if (this->mManeuverActive)
    {
        return;
    }

    if (mBf24.bTargetSetFromArrows)
    {
        return;
    }

    if ((mFlags & 0x40000) == 0x40000 || (mFlags & 0x80000) == 0x80000)
    {
        sCurrCarAITargetToSide = 1;
        this->mNearestEntity = this->mCar->mCarSprite;
    }
    else
    {
        // Look for a car 3 units ahead
        Ang16::PolarToCartesian_41FC20(mAngle, kFpThree_677B9C, x_off, y_off);
        mCar->mCarSprite->set_xyz_lazy(mCar->mCarSprite->mXy.x + x_off,
                                                              mCar->mCarSprite->mXy.y + y_off,
                                                              mCar->mCarSprite->mZpos);

        if (!this->mNearestEntity)
        {
            this->mNearestEntity = gSpriteGrid_1->FindCollidingSpritePreferType(this->mCar->mCarSprite, sprite_types_enum::none_0);
        }
    }

    if (!this->mNearestEntity)
    {
        return;
    }

    if (this->mNearestEntity->mSpriteTypeEnum != sprite_types_enum::car_2)
    {
        return;
    }

    if (sCurrCarAIIsChasing && sCurrCarAITargetCar == this->mNearestEntity->mCarBcPtr)
    {
        return;
    }

    if (!sCurrCarAITargetToSide)
    {
        if (mNearestEntity->mCarBcPtr->GetCarLinearSpeed() < sCurrCarAIDesiredSpeed)
        {
            bSlowerThanTarget = true;
        }

        if (sIgnoreTargetCarAhead && this->mNearestEntity->mCarBcPtr == sCurrCarAITargetCar)
        {
            return;
        }

        CarAI* pOtherAI = this->mNearestEntity->mCarBcPtr->mAI;
        if (pOtherAI)
        {
            if (sCurrCarAIDesiredSpeed > pOtherAI->mMaxSpeed)
            {
                bSlowerThanTarget = true;
            }
            if ((pOtherAI->mFlags & 0x100) == 0x100)
            {
                return;
            }
        }

        if (!bSlowerThanTarget)
        {
            return;
        }
    }

    // Look for a slope in the 5 blocks in front of the car
    for (u8 i = 0; i < 5; i++)
    {
        Ang16::PolarToCartesian_41FC20(mAngle, gF16fOne * Fix16(i), x_off, y_off);
        gCurrCarAI_ProbeX = mCar->mCarSprite->mXy.x + x_off;
        gCurrCarAI_ProbeY = mCar->mCarSprite->mXy.y + y_off;

        if (gCurrCarAI_ProbeX > kF16Zero && gCurrCarAI_ProbeY > kF16Zero && gCurrCarAI_ProbeX < kFp255_677950 &&
            gCurrCarAI_ProbeY < kFp255_677950)
        {
            gmp_block_info* pBlock = gMap_0x370->GetBlock(gCurrCarAI_ProbeX.ToInt(),
                                                                         gCurrCarAI_ProbeY.ToInt(),
                                                                         mCar->mCarSprite->mZpos.ToInt());

            if (!pBlock ||
                !(IS_GRADIENT_SLOPE(pBlock->mSlopeType) &&
                  !IS_AIR_TYPE(pBlock->mSlopeType)))
            {
                pBlock = gMap_0x370->GetBlock(gCurrCarAI_ProbeX.ToInt(),
                                                             gCurrCarAI_ProbeY.ToInt(),
                                                             mCar->mCarSprite->mZpos.ToInt() - 1);
            }

            if (pBlock && TrySetTargetDirectionFromArrows(pBlock))
            {
                this->mBf24.bChangingLane = false;
                return;
            }
        }
    }

    arrow_idx = mCar->CountConsecutiveArrowBlocks(mAngle, &arrow_count, gCurrCarAI_xpos, gCurrCarAI_ypos);
    if (arrow_count <= 1)
    {
        return;
    }

    if ((sCurrCarAITargetToSide && (mBf24.bFlag19)) || (!sCurrCarAITargetToSide && arrow_idx > 0))
    {
        if (arrow_idx != arrow_count - 1)
        {
            return;
        }

        if (!sCurrCarAITargetToSide && !(sCurrCarAIDesiredSpeed > kFpPoint2_6779C0) &&
            !(mNearestEntity->mCarBcPtr->GetCarLinearSpeed() == kF16Zero))
        {
            sCurrCarAIDesiredSpeed = mNearestEntity->mCarBcPtr->GetCarLinearSpeed();
            return;
        }

        mCar->mCarSprite->set_xyz_lazy(gCurrCarAI_xpos, gCurrCarAI_ypos, gCurrCarAI_zpos);

        // goto tail_1 (not break): all four cases end with byte-identical probe code, which VC6
        // folds into one copy with the others jumping in. With `break`, the fall-through (west, the
        // last case) survives and the rest jump into it; `goto tail_1` (the label `break` would reach
        // anyway) keeps the first copy (north) instead, as the original does (see matching_quirks.md,
        // "Which copy survives can also depend on how the cases leave the switch").
        switch (this->mCurrDirection)
        {
            case car_ai_direction::north_1:
                Ang16::PolarToCartesian_41FC20(kAng270_677B08, kFpPoint7, x_off, y_off);
                mCar->mCarSprite->set_xyz_lazy(mCar->mCarSprite->mXy.x + x_off,
                                                                      mCar->mCarSprite->mXy.y + y_off,
                                                                      mCar->mCarSprite->mZpos);
                if (gSpriteGrid_1->FindCollidingSpritePreferType(mCar->mCarSprite, sprite_types_enum::none_0))
                {
                    return;
                }
                Ang16::PolarToCartesian_41FC20(kAng180_677ADE, kFpTwo_677B98, x_off, y_off);
                mCar->mCarSprite->set_xyz_lazy(mCar->mCarSprite->mXy.x + x_off,
                                                                      mCar->mCarSprite->mXy.y + y_off,
                                                                      mCar->mCarSprite->mZpos);
                if (gSpriteGrid_1->FindCollidingSpritePreferType(mCar->mCarSprite, sprite_types_enum::none_0))
                {
                    return;
                }
                goto tail_1;

            case car_ai_direction::south_2:
                Ang16::PolarToCartesian_41FC20(kAng90_6779E4, kFpPoint7, x_off, y_off);
                mCar->mCarSprite->set_xyz_lazy(mCar->mCarSprite->mXy.x + x_off,
                                                                      mCar->mCarSprite->mXy.y + y_off,
                                                                      mCar->mCarSprite->mZpos);
                if (gSpriteGrid_1->FindCollidingSpritePreferType(mCar->mCarSprite, sprite_types_enum::none_0))
                {
                    return;
                }
                Ang16::PolarToCartesian_41FC20(kAng0_677CE8, kFpTwo_677B98, x_off, y_off);
                mCar->mCarSprite->set_xyz_lazy(mCar->mCarSprite->mXy.x + x_off,
                                                                      mCar->mCarSprite->mXy.y + y_off,
                                                                      mCar->mCarSprite->mZpos);
                if (gSpriteGrid_1->FindCollidingSpritePreferType(mCar->mCarSprite, sprite_types_enum::none_0))
                {
                    return;
                }
                goto tail_1;

            case car_ai_direction::east_3:
                Ang16::PolarToCartesian_41FC20(kAng180_677ADE, kFpPoint7, x_off, y_off);
                mCar->mCarSprite->set_xyz_lazy(mCar->mCarSprite->mXy.x + x_off,
                                                                      mCar->mCarSprite->mXy.y + y_off,
                                                                      mCar->mCarSprite->mZpos);
                if (gSpriteGrid_1->FindCollidingSpritePreferType(mCar->mCarSprite, sprite_types_enum::none_0))
                {
                    return;
                }
                Ang16::PolarToCartesian_41FC20(kAng90_6779E4, kFpTwo_677B98, x_off, y_off);
                mCar->mCarSprite->set_xyz_lazy(mCar->mCarSprite->mXy.x + x_off,
                                                                      mCar->mCarSprite->mXy.y + y_off,
                                                                      mCar->mCarSprite->mZpos);
                if (gSpriteGrid_1->FindCollidingSpritePreferType(mCar->mCarSprite, sprite_types_enum::none_0))
                {
                    return;
                }
                goto tail_1;

            case car_ai_direction::west_4:
                Ang16::PolarToCartesian_41FC20(kAng0_677CE8, kFpPoint7, x_off, y_off);
                mCar->mCarSprite->set_xyz_lazy(mCar->mCarSprite->mXy.x + x_off,
                                                                      mCar->mCarSprite->mXy.y + y_off,
                                                                      mCar->mCarSprite->mZpos);
                if (gSpriteGrid_1->FindCollidingSpritePreferType(mCar->mCarSprite, sprite_types_enum::none_0))
                {
                    return;
                }
                Ang16::PolarToCartesian_41FC20(kAng270_677B08, kFpTwo_677B98, x_off, y_off);
                mCar->mCarSprite->set_xyz_lazy(mCar->mCarSprite->mXy.x + x_off,
                                                                      mCar->mCarSprite->mXy.y + y_off,
                                                                      mCar->mCarSprite->mZpos);
                if (gSpriteGrid_1->FindCollidingSpritePreferType(mCar->mCarSprite, sprite_types_enum::none_0))
                {
                    return;
                }
                goto tail_1;

            default:
                return;
        }

    tail_1:
        this->mArrowStart = arrow_idx;
        this->mBf24.bChangingLane = true;
        this->mArrowCur = arrow_idx;
        this->mArrowEnd = arrow_idx - 1;
        return;
    }

    CarAI* pOtherAI = mNearestEntity->mCarBcPtr->mAI;
    // goto: the original returns through its own inline epilogue copy here, which VC6 only emits for
    // `if (c) goto ok; return;` (see docs/matching_quirks.md); `if (timer < 50) return;` jumps to the shared one
    if (pOtherAI)
    {
        if (pOtherAI->mStoppedTimer >= 50)
        {
            goto probe_sides;
        }
        return;
    }
    if (mNearestEntity->mCarBcPtr->GetCarLinearSpeed() > kFpPoint04)
    {
        return;
    }

probe_sides:

    if (sCurrCarAIVelocity < kFpPoint05_677B60 || sCurrCarAITargetToSide)
    {
        mCar->mCarSprite->set_xyz_lazy(gCurrCarAI_xpos, gCurrCarAI_ypos, gCurrCarAI_zpos);

        // goto tail_2 (not break): same tail-merge reason as the first switch above.
        switch (this->mCurrDirection)
        {
            case car_ai_direction::north_1:
                x_off = Ang16::sine(kAng90_6779E4) * kFpPoint7;
                y_off = Ang16::cosine(kAng90_6779E4) * kFpPoint7;
                mCar->mCarSprite->set_xyz_lazy(mCar->mCarSprite->mXy.x + x_off,
                                                                      mCar->mCarSprite->mXy.y + y_off,
                                                                      mCar->mCarSprite->mZpos);
                if (gSpriteGrid_1->FindCollidingSpritePreferType(mCar->mCarSprite, sprite_types_enum::none_0))
                {
                    return;
                }
                x_off = Ang16::sine(kAng180_677ADE) * kFpTwo_677B98;
                y_off = Ang16::cosine(kAng180_677ADE) * kFpTwo_677B98;
                mCar->mCarSprite->set_xyz_lazy(mCar->mCarSprite->mXy.x + x_off,
                                                                      mCar->mCarSprite->mXy.y + y_off,
                                                                      mCar->mCarSprite->mZpos);
                if (gSpriteGrid_1->FindCollidingSpritePreferType(mCar->mCarSprite, sprite_types_enum::none_0))
                {
                    return;
                }
                goto tail_2;

            case car_ai_direction::south_2:
                x_off = Ang16::sine(kAng270_677B08) * kFpPoint7;
                y_off = Ang16::cosine(kAng270_677B08) * kFpPoint7;
                mCar->mCarSprite->set_xyz_lazy(mCar->mCarSprite->mXy.x + x_off,
                                                                      mCar->mCarSprite->mXy.y + y_off,
                                                                      mCar->mCarSprite->mZpos);
                if (gSpriteGrid_1->FindCollidingSpritePreferType(mCar->mCarSprite, sprite_types_enum::none_0))
                {
                    return;
                }
                x_off = Ang16::sine(kAng0_677CE8) * kFpTwo_677B98;
                y_off = Ang16::cosine(kAng0_677CE8) * kFpTwo_677B98;
                mCar->mCarSprite->set_xyz_lazy(mCar->mCarSprite->mXy.x + x_off,
                                                                      mCar->mCarSprite->mXy.y + y_off,
                                                                      mCar->mCarSprite->mZpos);
                if (gSpriteGrid_1->FindCollidingSpritePreferType(mCar->mCarSprite, sprite_types_enum::none_0))
                {
                    return;
                }
                goto tail_2;

            case car_ai_direction::east_3:
                x_off = Ang16::sine(kAng0_677CE8) * kFpPoint7;
                y_off = Ang16::cosine(kAng0_677CE8) * kFpPoint7;
                mCar->mCarSprite->set_xyz_lazy(mCar->mCarSprite->mXy.x + x_off,
                                                                      mCar->mCarSprite->mXy.y + y_off,
                                                                      mCar->mCarSprite->mZpos);
                if (gSpriteGrid_1->FindCollidingSpritePreferType(mCar->mCarSprite, sprite_types_enum::none_0))
                {
                    return;
                }
                x_off = Ang16::sine(kAng90_6779E4) * kFpTwo_677B98;
                y_off = Ang16::cosine(kAng90_6779E4) * kFpTwo_677B98;
                mCar->mCarSprite->set_xyz_lazy(mCar->mCarSprite->mXy.x + x_off,
                                                                      mCar->mCarSprite->mXy.y + y_off,
                                                                      mCar->mCarSprite->mZpos);
                if (gSpriteGrid_1->FindCollidingSpritePreferType(mCar->mCarSprite, sprite_types_enum::none_0))
                {
                    return;
                }
                goto tail_2;

            case car_ai_direction::west_4:
                x_off = Ang16::sine(kAng180_677ADE) * kFpPoint7;
                y_off = Ang16::cosine(kAng180_677ADE) * kFpPoint7;
                mCar->mCarSprite->set_xyz_lazy(mCar->mCarSprite->mXy.x + x_off,
                                                                      mCar->mCarSprite->mXy.y + y_off,
                                                                      mCar->mCarSprite->mZpos);
                if (gSpriteGrid_1->FindCollidingSpritePreferType(mCar->mCarSprite, sprite_types_enum::none_0))
                {
                    return;
                }
                x_off = Ang16::sine(kAng270_677B08) * kFpTwo_677B98;
                y_off = Ang16::cosine(kAng270_677B08) * kFpTwo_677B98;
                mCar->mCarSprite->set_xyz_lazy(mCar->mCarSprite->mXy.x + x_off,
                                                                      mCar->mCarSprite->mXy.y + y_off,
                                                                      mCar->mCarSprite->mZpos);
                if (gSpriteGrid_1->FindCollidingSpritePreferType(mCar->mCarSprite, sprite_types_enum::none_0))
                {
                    return;
                }
                goto tail_2;

            default:
                return;
        }

    tail_2:
        this->mBf24.bChangingLane = true;
        this->mArrowStart = 0;
        this->mArrowCur = 0;
        u8 next_idx = arrow_idx + 1;
        // --arrow_count (not arrow_count - 1): the dec then belongs to arrow_count, last_idx is only a copy
        // (colour priority 28 instead of 52), so next_idx (36) is coloured first and gets al, as in 10.5
        u8 last_idx = --arrow_count;
        this->mArrowEnd = next_idx;
        if (next_idx > last_idx)
        {
            this->mArrowEnd = last_idx;
        }
    }
}

MATCH_FUNC(0x44e0c0)
void CarAI::UpdateChase()
{
    sCurrCarAIIsChasing = true;
    u8 t_x;
    u8 t_y;
    u8 t_z = 0;
    this->mMaxSpeed = this->mCar->mDriver->mRunSpeed;
    CarChaseTask* pChaseTask = mCar->mpChaseTask;

    Fix16 target_x;
    Fix16 target_y;
    Fix16 target_z;

    if (pChaseTask->mTaskType != car_task_type::goto_position_1)
    {
        sCurrCarAITargetCar = pChaseTask->mpTargetPed->mCar;
        target_x = this->mCar->mpChaseTask->mpTargetPed->get_cam_x();
        gCurrCarAI_TargetX = target_x;
        target_y = this->mCar->mpChaseTask->mpTargetPed->get_cam_y();
        sCurrCarAITargetY = target_y;
        target_z = this->mCar->mpChaseTask->mpTargetPed->get_cam_z();
    }
    else
    {
        target_x = pChaseTask->mTargetX;
        gCurrCarAI_TargetX = target_x;
        target_y = this->mCar->mpChaseTask->mTargetY;
        sCurrCarAITargetY = target_y;
        target_z = this->mCar->mpChaseTask->mTargetZ;
    }

    sCurrCarAITargetZ = target_z;
    Fix16 dist;
    dist = Fix16::MaxAbsDistance(gCurrCarAI_xpos, gCurrCarAI_ypos, gCurrCarAI_TargetX, sCurrCarAITargetY);
    if (dist < kFpFive_677BA4 && this->mCar->mpChaseTask->mTaskType != car_task_type::goto_position_1)
    {
        if (mRouteIdx > 0)
        {
            gRouteFinder->CancelRoute(mRouteIdx);
            this->mRouteIdx = -1;
        }

        this->mCar->mpChaseTask->mFollowingRoute = false;

        if (gPolice_7B8->mMaxWantedLevel > cop_level_enum::police_2 || mCar->IsPoliceCar())
        {
            this->mCar->mpChaseTask->mSnapTimer = 100;
        }
        else
        {
            this->mCar->mpChaseTask->mSnapTimer = 300;
        }
        return;
    }

    CarChaseTask* pTask = this->mCar->mpChaseTask;
    if (pTask->mCanSnapToTarget)
    {
        if (pTask->mSnapTimer == 0)
        {
            if (!gGame_0x40->IsSpriteInAnyPlayerBounds(this->mCar->mCarSprite, kFpFive))
            {
                if (this->mCar->mPhysics)
                {
                    if (!this->mBf24.bReversing)
                    {
                        if (mCar->SnapCarToGreenArrow(gCurrCarAI_TargetX, sCurrCarAITargetY, sCurrCarAITargetZ))
                        {
                            if (gPolice_7B8->mMaxWantedLevel > cop_level_enum::police_2 || mCar->IsPoliceCar())
                            {
                                this->mCar->mpChaseTask->mSnapTimer = 100;
                            }
                            else
                            {
                                this->mCar->mpChaseTask->mSnapTimer = 300;
                            }

                            this->mManeuverActive = 0;
                            this->mFlags &= ~0x4540u;
                            this->mCar->mpChaseTask->mChaseState = car_chase_state::follow_behind_0;
                            this->mCar->mpChaseTask->mSettleCounter = 0;
                            this->mCar->mAbandoned = false;
                            mCar->RepairDamage();
                            gPolice_7B8->DespawnCrewInCar(this->mCar);
                        }
                    }
                }
            }
        }
        else if (static_cast<u16>(pTask->mSnapTimer) > 0)
        {
            pTask->mSnapTimer--;
        }
        return;
    }
    else
    {

        if (mRouteIdx > 0)
        {
            gRouteFinder->CancelRoute(mRouteIdx);
            this->mRouteIdx = -1;
        }

        gmp_block_info* pBlock = gMap_0x370->GetBlock(gCurrCarAI_TargetX.ToInt(), sCurrCarAITargetY.ToInt(), sCurrCarAITargetZ.ToInt());
        Fix16 target_block_z = sCurrCarAITargetZ;
        if (!(pBlock && IS_GRADIENT_SLOPE(pBlock->mSlopeType) &&
              !IS_AIR_TYPE(pBlock->mSlopeType)))
        {
            target_block_z = sCurrCarAITargetZ - gF16fOne;
            pBlock = gMap_0x370->GetBlock(gCurrCarAI_TargetX.ToInt(), sCurrCarAITargetY.ToInt(), target_block_z.ToInt());
        }

        if (!pBlock || !static_cast<u8>(gMap_0x370->GetArrowDirectionFromBlock(pBlock, true)))
        {
            t_x = gCurrCarAI_TargetX.ToInt();
            t_y = sCurrCarAITargetY.ToInt();
            t_z = sCurrCarAITargetZ.ToInt();
            if (!gPathFinder->FindNearbyTileMatchingSlopeType(ROAD, &t_x, &t_y, &t_z, false))
            {
                if (mRouteIdx > 0)
                {
                    gRouteFinder->CancelRoute(mRouteIdx);
                    this->mRouteIdx = -1;
                }

                this->mCar->mpChaseTask->mFollowingRoute = false;

                if (gPolice_7B8->mMaxWantedLevel > cop_level_enum::police_2 || mCar->IsPoliceCar())
                {
                    this->mCar->mpChaseTask->mSnapTimer = 100;
                }
                else
                {
                    this->mCar->mpChaseTask->mSnapTimer = 300;
                }
                return;
            }

            gCurrCarAI_TargetX = Fix16(t_x);
            sCurrCarAITargetY = Fix16(t_y);
            sCurrCarAITargetZ = Fix16(t_z);
            gmp_block_info* pTargetBlock = gMap_0x370->GetBlock(gCurrCarAI_TargetX.ToInt(), sCurrCarAITargetY.ToInt(), sCurrCarAITargetZ.ToInt());
            target_block_z = sCurrCarAITargetZ;
            if (!(pTargetBlock && IS_GRADIENT_SLOPE(pTargetBlock->mSlopeType) &&
                  !IS_AIR_TYPE(pTargetBlock->mSlopeType)))
            {
                target_block_z = sCurrCarAITargetZ - gF16fOne;
                pTargetBlock = gMap_0x370->GetBlock(gCurrCarAI_TargetX.ToInt(), sCurrCarAITargetY.ToInt(), target_block_z.ToInt());
            }
            if (pTargetBlock)
            {
                gMap_0x370->GetArrowDirectionFromBlock(pTargetBlock, true);
            }
        }

        GoToBlock(gCurrCarAI_TargetX.ToInt(), sCurrCarAITargetY.ToInt(), target_block_z.ToInt(), green_or_red_3);
        this->mCar->mpChaseTask->mFollowingRoute = true;
    }
}

// https://decomp.me/scratch/Auxlx
// MaxAbsDistance past the inline budget: the y difference stays inline, the Abs calls
// go out of line (budget)
static inline Fix16 __stdcall MaxAbsDistanceRawY_451980(Fix16& x1, Fix16& y1, Fix16& x2, Fix16& y2)
{
    Fix16 diff_x = x2 - x1;
    Fix16 diff_y;
    diff_y.mValue = y2.mValue - y1.mValue;

    Fix16 result;
    result = Fix16::Max_44E540(Fix16::Abs(diff_x), Fix16::Abs(diff_y));
    return result;
}

// Inlined in UpdateStateMachine (9.6f UpdateStateMachine_42D820 calls it)
static inline s32 __stdcall GetOppositeFace_42A660(const s32& face)
{
    switch (face)
    {
        case 1:
            return 2;
        case 2:
            return 1;
        case 4:
            return 3;
        case 3:
            return 4;
        default:
            return 0;
    }
}

MATCH_FUNC(0x44e560)
void CarAI::UpdateStateMachine()
{
    Ang16 target_angle = kAng0_677CE8;
    Ang16 heading_copy = kAng0_677CE8;
    Ang16 aim_angle = kAng0_677CE8;
    Ang16 heading_delta = kAng0_677CE8;
    Fix16 dy = kF16Zero;
    CarChaseTask* pChase = NULL;
    s16 waitCount = 0;
    this->mFlags &= ~0x4580u;
    s16 maxCount = 0;
    u8 bTryLeft = 0;
    u8 bTryRight = 0;
    u8 police_level = 0;
    u8 bOvertake;
    Fix16 dx;
    Fix16 target_dist;
    Fix16 speed_limit;
    Sprite* pSprite;
    CarPhysics* pPhysics;

    if (mCar->mpChaseTask != NULL)
    {
        pChase = mCar->mpChaseTask;
        switch (pChase->mTaskType)
        {
            case car_task_type::goto_position_1:
                break;

            default:
                if (pChase->mpTargetPed->mGameObject != NULL)
                {
                    pChase->mChaseState = car_chase_state::follow_behind_0;
                }
                else if (mCar->IsPoliceCar() != 0)
                {
                    Ped* pTargetPed = pChase->mpTargetPed;
                    switch (pTargetPed->get_wanted_star_count())
                    {
                        case cop_level_enum::none_0:
                        case cop_level_enum::police_1:
                            police_level = 1;
                            break;
                        case cop_level_enum::police_2:
                        case cop_level_enum::police_3:
                            police_level = 2;
                            break;
                        case cop_level_enum::swat_4:
                            police_level = 3;
                            break;
                        case cop_level_enum::fbi_5:
                            police_level = 4;
                            break;
                    }
                }
                break;

            case car_task_type::follow_ped_2:
                pChase->mChaseState = car_chase_state::choose_side_5;
                break;
        }
    }

    // Saved so the target offsets applied below can be undone
    Fix16 savedTargetX = gCurrCarAI_TargetX;
    Fix16 savedTargetY = sCurrCarAITargetY;

    this->mFlags &= 0xFFEFFFFF;
    if (mSeekingRoad == 0)
    {
        assert(pChase != NULL);
        sCurrCarAIAllowTrafficSteering = false;
        switch (gCarChaseTaskTable->CountPursuers(pChase->mpTargetPed, this->mCar->mDriver))
        {
            case 1:
                speed_limit = kFpPoint2_6779C0;
                break;
            case 2:
                speed_limit = kFpPoint2_6779C0 + kFpPoint05_677B60;
                break;
            default:
                speed_limit = kFpNine;
                break;
        }

        switch (pChase->mChaseState)
        {
            case car_chase_state::follow_behind_0:
                if (sCurrCarAITargetCar != NULL)
                {
                    mCar->mCarSprite->set_xyz_lazy(gCurrCarAI_TargetX, sCurrCarAITargetY, gCurrCarAI_zpos);
                    Ang16::PolarToCartesian_41FC20(sCurrCarAITargetCar->mCarSprite->mRotation - kAng180_677ADE, kFpThree_677B9C, dx, dy);
                    gCurrCarAI_TargetX += dx;
                    sCurrCarAITargetY += dy;
                    target_dist = Fix16::MaxAbsDistance(gCurrCarAI_xpos, gCurrCarAI_ypos, gCurrCarAI_TargetX, sCurrCarAITargetY);
                    target_angle = Fix16::atan2_fixed(sCurrCarAITargetY - gCurrCarAI_ypos, gCurrCarAI_TargetX - gCurrCarAI_xpos);
                }
                else
                {
                    target_dist = Fix16::MaxAbsDistance(gCurrCarAI_xpos, gCurrCarAI_ypos, gCurrCarAI_TargetX, sCurrCarAITargetY);
                    target_angle = Fix16::atan2_fixed(sCurrCarAITargetY - gCurrCarAI_ypos, gCurrCarAI_TargetX - gCurrCarAI_xpos);
                }
                break;

            case car_chase_state::follow_behind_2:
                if (sCurrCarAITargetCar != NULL)
                {
                    mCar->mCarSprite->set_xyz_lazy(gCurrCarAI_TargetX, sCurrCarAITargetY, gCurrCarAI_zpos);
                    Ang16::PolarToCartesian_41FC20(sCurrCarAITargetCar->mCarSprite->mRotation - kAng180_677ADE, kFpThree_677B9C, dx, dy);
                    gCurrCarAI_TargetX += dx;
                    sCurrCarAITargetY += dy;
                    Ang16::PolarToCartesian_41FC20(sCurrCarAITargetCar->mCarSprite->mRotation + kAng90_6779E4, kFpHalf_677A84, dx, dy);
                    gCurrCarAI_TargetX += dx;
                    sCurrCarAITargetY += dy;
                    target_dist = Fix16::MaxAbsDistance(gCurrCarAI_xpos, gCurrCarAI_ypos, gCurrCarAI_TargetX, sCurrCarAITargetY);
                    target_angle = Fix16::atan2_fixed(sCurrCarAITargetY - gCurrCarAI_ypos, gCurrCarAI_TargetX - gCurrCarAI_xpos);
                }
                break;

            case car_chase_state::unused_1:
                if (sCurrCarAITargetCar != NULL)
                {
                    mCar->mCarSprite->set_xyz_lazy(gCurrCarAI_TargetX, sCurrCarAITargetY, gCurrCarAI_zpos);
                    Ang16::PolarToCartesian_41FC20(sCurrCarAITargetCar->mCarSprite->mRotation - kAng180_677ADE, kFpThree_677B9C, dx, dy);
                    gCurrCarAI_TargetX += dx;
                    sCurrCarAITargetY += dy;
                    target_dist = Fix16::MaxAbsDistance(gCurrCarAI_xpos, gCurrCarAI_ypos, gCurrCarAI_TargetX, sCurrCarAITargetY);
                    target_angle = Fix16::atan2_fixed(sCurrCarAITargetY - gCurrCarAI_ypos, gCurrCarAI_TargetX - gCurrCarAI_xpos);
                }
                break;

            case car_chase_state::move_left_3:
            {
                assert(sCurrCarAITargetCar != NULL);
                mCar->mCarSprite->set_xyz_lazy(gCurrCarAI_TargetX, sCurrCarAITargetY, gCurrCarAI_zpos);
                Ang16::PolarToCartesian_41FC20(sCurrCarAITargetCar->mCarSprite->mRotation - kAng90_6779E4, gF16fOne, dx, dy);
                gCurrCarAI_TargetX += dx;
                sCurrCarAITargetY += dy;
                Ang16::PolarToCartesian_41FC20(sCurrCarAITargetCar->mCarSprite->mRotation, kFpHalf_677A84, dx, dy);
                gCurrCarAI_TargetX += dx;
                sCurrCarAITargetY += dy;
                target_dist = Fix16::MaxAbsDistance(gCurrCarAI_xpos, gCurrCarAI_ypos, gCurrCarAI_TargetX, sCurrCarAITargetY);
                target_angle = Fix16::atan2_fixed(sCurrCarAITargetY - gCurrCarAI_ypos, gCurrCarAI_TargetX - gCurrCarAI_xpos);
                break;
            }

            case car_chase_state::choose_side_5:
            {
                assert(sCurrCarAITargetCar != NULL);
                if (police_level == 1)
                {
                    mCar->mCarSprite->set_xyz_lazy(gCurrCarAI_TargetX, sCurrCarAITargetY, gCurrCarAI_zpos);
                    Ang16::PolarToCartesian_41FC20(sCurrCarAITargetCar->mCarSprite->mRotation - kAng180_677ADE, gF16fOne, dx, dy);
                    gCurrCarAI_TargetX += dx;
                    sCurrCarAITargetY += dy;
                }
                target_dist = Fix16::MaxAbsDistance(gCurrCarAI_xpos, gCurrCarAI_ypos, gCurrCarAI_TargetX, sCurrCarAITargetY);
                target_angle = Fix16::atan2_fixed(sCurrCarAITargetY - gCurrCarAI_ypos, gCurrCarAI_TargetX - gCurrCarAI_xpos);
                break;
            }

            case car_chase_state::move_right_4:
            {
                assert(sCurrCarAITargetCar != NULL);
                mCar->mCarSprite->set_xyz_lazy(gCurrCarAI_TargetX, sCurrCarAITargetY, gCurrCarAI_zpos);
                Ang16::PolarToCartesian_41FC20(sCurrCarAITargetCar->mCarSprite->mRotation + kAng90_6779E4, gF16fOne, dx, dy);
                gCurrCarAI_TargetX += dx;
                sCurrCarAITargetY += dy;
                Ang16::PolarToCartesian_41FC20(sCurrCarAITargetCar->mCarSprite->mRotation, kFpHalf_677A84, dx, dy);
                gCurrCarAI_TargetX += dx;
                sCurrCarAITargetY += dy;
                target_dist = Fix16::MaxAbsDistance(gCurrCarAI_xpos, gCurrCarAI_ypos, gCurrCarAI_TargetX, sCurrCarAITargetY);
                target_angle = Fix16::atan2_fixed(sCurrCarAITargetY - gCurrCarAI_ypos, gCurrCarAI_TargetX - gCurrCarAI_xpos);
                break;
            }

            case car_chase_state::approach_left_6:
            {
                assert(sCurrCarAITargetCar != NULL);
                MakeAgressiveSirensAndLights();
                mCar->mCarSprite->set_xyz_lazy(gCurrCarAI_TargetX, sCurrCarAITargetY, gCurrCarAI_zpos);
                Ang16::PolarToCartesian_41FC20(sCurrCarAITargetCar->mCarSprite->mRotation - kAng90_6779E4, kFpThreeQuarters, dx, dy);
                gCurrCarAI_TargetX += dx;
                sCurrCarAITargetY += dy;
                target_dist = Fix16::MaxAbsDistance(gCurrCarAI_xpos, gCurrCarAI_ypos, gCurrCarAI_TargetX, sCurrCarAITargetY);
                target_angle = Fix16::atan2_fixed(sCurrCarAITargetY - gCurrCarAI_ypos, gCurrCarAI_TargetX - gCurrCarAI_xpos);
                break;
            }

            case car_chase_state::alongside_left_8:
            {
                bOvertake = 0;
                MakeAgressiveSirensAndLights();
                switch (pChase->mTaskType)
                {
                    case 3:
                        bOvertake = 1;
                        break;

                    case car_task_type::follow_target_ped_4:
                        assert(sCurrCarAITargetCar != NULL);
                        if (this->mCar->mCarInfoIdx != car_model_enum::EDSELFBI)
                        {
                            speed_limit = kFpNine;
                        }

                        if (sCurrCarAITargetCar->GetMaxSpeed().mValue > speed_limit.mValue)
                        {
                            // Spelling required by VC6's CSE numbering, like the one below (docs/match_attempts.md)
                            if (3u < pChase->mBlockCounter)
                            {
                                bOvertake = 1;
                                waitCount = 10;
                            }
                            else
                            {
                                bOvertake = 0;
                            }
                        }
                        else
                        {
                            switch (gCarChaseTaskTable->CountPursuers(pChase->mpTargetPed, mCar->mDriver))
                            {
                                case 1:
                                    // Spelling required by VC6's CSE numbering (docs/match_attempts.md)
                                    waitCount = gPolice_7B8->mMaxWantedLevel - 1 != 0 ? 10 : 50;
                                    break;

                                case 2:
                                    if (gCarChaseTaskTable->HasPursuerChoosingSide(this->mCar->mDriver) != 0)
                                    {
                                        waitCount = 0;
                                    }
                                    else
                                    {
                                        waitCount = 1;
                                        pChase->mWaitCounter = 0;
                                    }
                                    break;

                                case 3:
                                case 4:
                                    if (gCarChaseTaskTable->HasPursuerAlongsideRight(this->mCar->mDriver) != 0)
                                    {
                                        waitCount = 0;
                                    }
                                    else
                                    {
                                        waitCount = 1;
                                        pChase->mWaitCounter = 0;
                                    }
                                    break;

                                default:
                                    waitCount = 10;
                                    break;
                            }
                        }
                        break;

                    case car_task_type::kill_ped_5:
                        assert(sCurrCarAITargetCar != NULL);
                        if (sCurrCarAITargetCar->GetMaxSpeed() > speed_limit)
                        {
                            bOvertake = 1;
                        }
                        break;
                }

                if (static_cast<u16>(pChase->mWaitCounter) < waitCount)
                {
                    ++pChase->mWaitCounter;
                    mCar->mCarSprite->set_xyz_lazy(gCurrCarAI_TargetX, sCurrCarAITargetY, gCurrCarAI_zpos);
                    Ang16::PolarToCartesian_41FC20(sCurrCarAITargetCar->mCarSprite->mRotation - kAng90_6779E4, kFpThreeQuarters, dx, dy);
                    gCurrCarAI_TargetX += dx;
                    sCurrCarAITargetY += dy;
                    Ang16::PolarToCartesian_41FC20(sCurrCarAITargetCar->mCarSprite->mRotation, kFpHalf_677A84, dx, dy);
                    gCurrCarAI_TargetX += dx;
                    sCurrCarAITargetY += dy;
                    target_dist = Fix16::MaxAbsDistance(gCurrCarAI_xpos, gCurrCarAI_ypos, gCurrCarAI_TargetX, sCurrCarAITargetY);
                    target_angle = Fix16::atan2_fixed(sCurrCarAITargetY - gCurrCarAI_ypos, gCurrCarAI_TargetX - gCurrCarAI_xpos);
                }
                else
                {
                    if (bOvertake == 1)
                    {
                        pChase->mRamTarget = true;
                        mCar->mCarSprite->set_xyz_lazy(gCurrCarAI_TargetX, sCurrCarAITargetY, gCurrCarAI_zpos);
                        Ang16::PolarToCartesian_41FC20(sCurrCarAITargetCar->mCarSprite->mRotation, gF16fOne, dx, dy);
                        gCurrCarAI_TargetX += dx;
                        sCurrCarAITargetY += dy;
                        target_dist = Fix16::MaxAbsDistance(gCurrCarAI_xpos, gCurrCarAI_ypos, gCurrCarAI_TargetX, sCurrCarAITargetY);
                        target_angle = Fix16::atan2_fixed(sCurrCarAITargetY - gCurrCarAI_ypos, gCurrCarAI_TargetX - gCurrCarAI_xpos);
                    }
                    else
                    {
                        pChase->mChaseState = car_chase_state::pull_ahead_left_10;
                        return;
                    }
                }
                break;
            }

            case car_chase_state::approach_right_7:
            {
                assert(sCurrCarAITargetCar != NULL);
                MakeAgressiveSirensAndLights();
                mCar->mCarSprite->set_xyz_lazy(gCurrCarAI_TargetX, sCurrCarAITargetY, gCurrCarAI_zpos);
                Ang16::PolarToCartesian_41FC20(sCurrCarAITargetCar->mCarSprite->mRotation + kAng90_6779E4, kFpThreeQuarters, dx, dy);
                gCurrCarAI_TargetX += dx;
                sCurrCarAITargetY += dy;
                target_dist = Fix16::MaxAbsDistance(gCurrCarAI_xpos, gCurrCarAI_ypos, gCurrCarAI_TargetX, sCurrCarAITargetY);
                target_angle = Fix16::atan2_fixed(sCurrCarAITargetY - gCurrCarAI_ypos, gCurrCarAI_TargetX - gCurrCarAI_xpos);
                break;
            }

            case car_chase_state::alongside_right_9:
            {
                bOvertake = 0;
                MakeAgressiveSirensAndLights();
                switch (pChase->mTaskType)
                {
                    case 3:
                        bOvertake = 1;
                        break;

                    case car_task_type::follow_target_ped_4:
                        assert(sCurrCarAITargetCar != NULL);
                        bOvertake = 0;
                        if (this->mCar->mCarInfoIdx != car_model_enum::EDSELFBI)
                        {
                            speed_limit = kFpNine;
                        }

                        if (sCurrCarAITargetCar->GetMaxSpeed() > speed_limit)
                        {
                            if (pChase->mBlockCounter > 3u)
                            {
                                bOvertake = 1;
                                waitCount = 10;
                            }
                            else
                            {
                                bOvertake = 0;
                            }
                        }
                        else
                        {
                            switch (gCarChaseTaskTable->CountPursuers(pChase->mpTargetPed, mCar->mDriver))
                            {
                                case 1:
                                    waitCount = gPolice_7B8->mMaxWantedLevel != cop_level_enum::police_1 ? 10 : 50;
                                    break;

                                case 2:
                                    if (gCarChaseTaskTable->HasPursuerChoosingSide(this->mCar->mDriver) != 0)
                                    {
                                        waitCount = 0;
                                    }
                                    else
                                    {
                                        waitCount = 1;
                                        pChase->mWaitCounter = 0;
                                    }
                                    break;

                                case 3:
                                case 4:
                                    if (gCarChaseTaskTable->HasPursuerAlongsideLeft(this->mCar->mDriver) != 0)
                                    {
                                        waitCount = 0;
                                    }
                                    else
                                    {
                                        waitCount = 1;
                                        pChase->mWaitCounter = 0;
                                    }
                                    break;

                                default:
                                    waitCount = 10;
                                    break;
                            }
                        }
                        break;

                    case car_task_type::kill_ped_5:
                        assert(sCurrCarAITargetCar != NULL);
                        if (sCurrCarAITargetCar->GetMaxSpeed() > speed_limit)
                        {
                            bOvertake = 1;
                        }
                        break;
                }

                if (static_cast<u16>(pChase->mWaitCounter) < waitCount)
                {
                    ++pChase->mWaitCounter;
                    mCar->mCarSprite->set_xyz_lazy(gCurrCarAI_TargetX, sCurrCarAITargetY, gCurrCarAI_zpos);
                    Ang16::PolarToCartesian_41FC20(sCurrCarAITargetCar->mCarSprite->mRotation + kAng90_6779E4, kFpThreeQuarters, dx, dy);
                    gCurrCarAI_TargetX += dx;
                    sCurrCarAITargetY += dy;
                    Ang16::PolarToCartesian_41FC20(sCurrCarAITargetCar->mCarSprite->mRotation, gF16fOne, dx, dy);
                    gCurrCarAI_TargetX += dx;
                    sCurrCarAITargetY += dy;
                    target_dist = Fix16::MaxAbsDistance(gCurrCarAI_xpos, gCurrCarAI_ypos, gCurrCarAI_TargetX, sCurrCarAITargetY);
                    target_angle = Fix16::atan2_fixed(sCurrCarAITargetY - gCurrCarAI_ypos, gCurrCarAI_TargetX - gCurrCarAI_xpos);
                }
                else
                {
                    if (bOvertake == 1)
                    {
                        pChase->mRamTarget = true;
                        mCar->mCarSprite->set_xyz_lazy(gCurrCarAI_TargetX, sCurrCarAITargetY, gCurrCarAI_zpos);
                        Ang16::PolarToCartesian_41FC20(sCurrCarAITargetCar->mCarSprite->mRotation, gF16fOne, dx, dy);
                        gCurrCarAI_TargetX += dx;
                        sCurrCarAITargetY += dy;
                        target_dist = Fix16::MaxAbsDistance(gCurrCarAI_xpos, gCurrCarAI_ypos, gCurrCarAI_TargetX, sCurrCarAITargetY);
                        target_angle = Fix16::atan2_fixed(sCurrCarAITargetY - gCurrCarAI_ypos, gCurrCarAI_TargetX - gCurrCarAI_xpos);
                    }
                    else
                    {
                        pChase->mChaseState = car_chase_state::pull_ahead_right_11;
                        return;
                    }
                }
                break;
            }

            case car_chase_state::pull_ahead_left_10:
            {
                assert(sCurrCarAITargetCar != NULL);
                MakeAgressiveSirensAndLights();
                mCar->mCarSprite->set_xyz_lazy(gCurrCarAI_TargetX, sCurrCarAITargetY, gCurrCarAI_zpos);
                Ang16::PolarToCartesian_41FC20(sCurrCarAITargetCar->mCarSprite->mRotation - kAng90_6779E4, kFpThreeQuarters, dx, dy);
                gCurrCarAI_TargetX += dx;
                sCurrCarAITargetY += dy;
                Ang16::PolarToCartesian_41FC20(sCurrCarAITargetCar->mCarSprite->mRotation, gF16fOne + kFpHalf_677A84, dx, dy);
                gCurrCarAI_TargetX += dx;
                sCurrCarAITargetY += dy;
                target_dist = Fix16::MaxAbsDistance(gCurrCarAI_xpos, gCurrCarAI_ypos, gCurrCarAI_TargetX, sCurrCarAITargetY);
                target_angle = Fix16::atan2_fixed(sCurrCarAITargetY - gCurrCarAI_ypos, gCurrCarAI_TargetX - gCurrCarAI_xpos);
                break;
            }

            case car_chase_state::pull_ahead_right_11:
            {
                assert(sCurrCarAITargetCar != NULL);
                MakeAgressiveSirensAndLights();
                mCar->mCarSprite->set_xyz_lazy(gCurrCarAI_TargetX, sCurrCarAITargetY, gCurrCarAI_zpos);
                Ang16::PolarToCartesian_41FC20(sCurrCarAITargetCar->mCarSprite->mRotation + kAng90_6779E4, kFpThreeQuarters, dx, dy);
                gCurrCarAI_TargetX += dx;
                sCurrCarAITargetY += dy;
                Ang16::PolarToCartesian_41FC20(sCurrCarAITargetCar->mCarSprite->mRotation, gF16fOne + kFpHalf_677A84, dx, dy);
                gCurrCarAI_TargetX += dx;
                sCurrCarAITargetY += dy;
                target_dist = Fix16::MaxAbsDistance(gCurrCarAI_xpos, gCurrCarAI_ypos, gCurrCarAI_TargetX, sCurrCarAITargetY);
                target_angle = Fix16::atan2_fixed(sCurrCarAITargetY - gCurrCarAI_ypos, gCurrCarAI_TargetX - gCurrCarAI_xpos);
                break;
            }

            case car_chase_state::in_front_12:
            case car_chase_state::in_front_stopping_13:
            {
                assert(sCurrCarAITargetCar != NULL);
                MakeAgressiveSirensAndLights();
                mCar->mCarSprite->set_xyz_lazy(gCurrCarAI_TargetX, sCurrCarAITargetY, gCurrCarAI_zpos);
                Ang16::PolarToCartesian_41FC20(sCurrCarAITargetCar->mCarSprite->mRotation, kFpThree_677B9C, dx, dy);
                gCurrCarAI_TargetX += dx;
                sCurrCarAITargetY += dy;
                target_dist = Fix16::MaxAbsDistance(gCurrCarAI_xpos, gCurrCarAI_ypos, gCurrCarAI_TargetX, sCurrCarAITargetY);
                target_angle = Fix16::atan2_fixed(sCurrCarAITargetY - gCurrCarAI_ypos, gCurrCarAI_TargetX - gCurrCarAI_xpos);
                if (pChase->mChaseState == car_chase_state::in_front_stopping_13)
                {
                    // Face the un-offset target position
                    aim_angle = Fix16::atan2_fixed(savedTargetY - gCurrCarAI_ypos, savedTargetX - gCurrCarAI_xpos);
                    if (sCurrCarAIDesiredSpeed.mValue < kFpPoint03.mValue)
                    {
                        mCar->DoBrake();
                        pChase->mSideCounter = 0;
                        pChase->mChaseState = car_chase_state::target_stopped_15;
                        return;
                    }

                    if (Ang16::IsAngleAhead(&this->mAngle, &aim_angle) != 0)
                    {
                        this->mCar->mPhysics->TurnAntiClockwise();
                    }
                    else
                    {
                        this->mCar->mPhysics->TurnClockwise();
                    }

                    heading_delta = ComputeShortestAngleDelta(this->mAngle, sCurrCarAITargetCar->mCarSprite->mRotation);
                    if (heading_delta.rValue > kAng12_677A2E.rValue)
                    {
                        pChase->mChaseState = car_chase_state::target_stopped_15;
                        return;
                    }
                    this->mCar->mPhysics->SetHandBrakeOn();
                    return;
                }
                break;
            }

            case car_chase_state::target_stopped_15:
                this->mBf24.bAIHalted = true;
                mCar->SetAIHalted();
                break;

            default:
                break;
        }
    }

    ComputeShortestAngleDelta(this->mAngle, target_angle);
    heading_copy = this->mAngle;
    aim_angle = target_angle;
    s32 face = Ang16::GetAngleFace(aim_angle);
    if (this->mCurrDirection != face)
    {
        if (target_dist.mValue < kFpFour_677BA0.mValue && !this->mSeekingRoad)
        {
            if (this->mCurrDirection == GetOppositeFace_42A660(face) && target_dist.mValue < kFpThree_677B9C.mValue)
            {
                assert(pPhysics != NULL);
                pPhysics = this->mCar->mPhysics;
                pPhysics->mIsFootBrakeOn = 0;
                pPhysics->mIsForwardGasOn = 0;
                pPhysics->mIsBackwardGasOn = 0;
                pPhysics->mIsCoasting = false;
            }
        }

        if (Ang16::IsAngleAhead(&heading_copy, &aim_angle) != 0)
        {
            if (this->mBf24.bReversing != 0)
            {
                this->mCar->mPhysics->TurnClockwise();
            }
            else
            {
                this->mCar->mPhysics->TurnAntiClockwise();
            }
        }
        else
        {
            if (this->mBf24.bReversing != 0)
            {
                this->mCar->mPhysics->TurnAntiClockwise();
            }
            else
            {
                this->mCar->mPhysics->TurnClockwise();
            }
        }
    }
    else
    {
        if (this->mCurrDirection == car_ai_direction::south_2)
        {
            heading_copy = this->mAngle + kAng90_6779E4;
            aim_angle = target_angle + kAng90_6779E4;
        }

        if (heading_copy < aim_angle - kAng4_677CE2 - kAng4_677CE2)
        {
            if (gRng.get_int(4) == 0)
            {
                if (this->mBf24.bReversing != 0)
                {
                    this->mCar->mPhysics->TurnClockwise();
                }
                else
                {
                    this->mCar->mPhysics->TurnAntiClockwise();
                }
            }
        }
        else
        {
            if (heading_copy > aim_angle + kAng4_677CE2 + kAng4_677CE2)
            {
                if (gRng.get_int(4) == 0)
                {
                    if (this->mBf24.bReversing != 0)
                    {
                        this->mCar->mPhysics->TurnAntiClockwise();
                    }
                    else
                    {
                        this->mCar->mPhysics->TurnClockwise();
                    }
                }
            }
            else
            {
                this->mCar->mPhysics->SetGoStraight();
                sCurrCarAIWentStraight = 1;
            }
        }
        mCar->mCarSprite->set_xyz_lazy(gCurrCarAI_xpos, gCurrCarAI_ypos, gCurrCarAI_zpos);
    }

    gCurrCarAI_TargetX = savedTargetX;
    sCurrCarAITargetY = savedTargetY;

    if (this->mSeekingRoad == 0)
    {
        // Probe one unit ahead in the direction we are turning to
        mCar->mCarSprite->set_xyz_lazy(gCurrCarAI_xpos, gCurrCarAI_ypos, gCurrCarAI_zpos);
        Ang16::PolarToCartesian_41FC20(kAng45_677A38.MultiplyByFix16(Fix16(this->mCar->mPhysics->mTurnDirection)) + this->mAngle,
                                       gF16fOne,
                                       dx,
                                       dy);
        Car* pCar = mCar;
        pCar->mCarSprite->set_xyz_lazy(pCar->mCarSprite->mXy.x + dx,
                                                              pCar->mCarSprite->mXy.y + dy,
                                                              pCar->mCarSprite->mZpos);
        if (mCar->mCarSprite->CheckSpriteMovementRegion() != 0)
        {
            this->mCar->mPhysics->SetGoStraight();
        }
    }

    if (sCurrCarAIDesiredSpeed.mValue < kFpPoint03.mValue)
    {
        mCar->DoBrake();
        pChase->mSideCounter = 0;
        pChase->mChaseState = car_chase_state::target_stopped_15;
        return;
    }

    if (target_dist < kFpTwo_677B98 && !this->mSeekingRoad)
    {
        if (sCurrCarAITargetCar == NULL)
        {
            return;
        }

        sCurrCarAIDesiredSpeed = sCurrCarAITargetCar->GetCarLinearSpeed();

        switch (pChase->mChaseState)
        {
            case car_chase_state::follow_behind_0:
            case car_chase_state::follow_behind_2:
                bOvertake = 0;
                if (static_cast<u16>(pChase->mSettleCounter) >= 10u)
                {
                    gCurrCarAI_TargetX = savedTargetX;
                    sCurrCarAITargetY = savedTargetY;
                    Ang16::PolarToCartesian_41FC20(sCurrCarAITargetCar->mCarSprite->mRotation - kAng180_677ADE, kFpTwo_677B98, dx, dy);
                    gCurrCarAI_TargetX += dx;
                    sCurrCarAITargetY += dy;
                    mCar->mCarSprite->set_xyz_lazy(gCurrCarAI_TargetX,
                                                                          sCurrCarAITargetY,
                                                                          mCar->mCarSprite->mZpos);

                    if (mCar->mCarSprite->CheckSpriteMovementRegion() == 0)
                    {
                        Ped* pOwner = gCarChaseTaskTable->FindDriverInState(car_chase_state::choose_side_5, pChase->mpTargetPed);
                        if (pOwner != NULL)
                        {
                            if (pOwner->mCar->mpChaseTask->mSideCounter > 10u)
                            {
                                pOwner->mCar->mpChaseTask->mChaseState = car_chase_state::move_left_3;
                                pOwner->mCar->mpChaseTask->mSideCounter = 0;
                                pChase->mChaseState = car_chase_state::choose_side_5;
                                pChase->mSettleCounter = 100;
                                pChase->mSideCounter = 0;
                                bOvertake = 1;
                            }
                        }
                        else
                        {
                            pChase->mChaseState = car_chase_state::choose_side_5;
                            pChase->mSettleCounter = 100;
                            pChase->mSideCounter = 0;
                            bOvertake = 1;
                        }
                    }
                }
                if (bOvertake)
                {
                    return;
                }

                sCurrCarAIDesiredSpeed = sCurrCarAITargetCar->GetCarLinearSpeed();
                if (target_dist < gF16fOne)
                {
                    pPhysics = this->mCar->mPhysics;
                    pPhysics->mIsFootBrakeOn = 0;
                    pPhysics->mIsForwardGasOn = 0;
                    pPhysics->mIsBackwardGasOn = 0;
                    pPhysics->mIsCoasting = false;
                }
                else if (sCurrCarAIVelocity < sCurrCarAIDesiredSpeed + kFpPoint02_677B5C)
                {
                    pPhysics = this->mCar->mPhysics;
                    pPhysics->mIsForwardGasOn = 1;
                    pPhysics->mIsFootBrakeOn = 0;
                    pPhysics->mIsBackwardGasOn = 0;
                    pPhysics->mIsCoasting = false;
                }
                else
                {
                    pPhysics = this->mCar->mPhysics;
                    pPhysics->mIsCoasting = true;
                    pPhysics->mIsFootBrakeOn = 0;
                    pPhysics->mIsBackwardGasOn = 0;
                    pPhysics->mIsForwardGasOn = 0;
                }

                if (static_cast<u16>(pChase->mSettleCounter) < 10u)
                {
                    pChase->mSettleCounter++;
                }
                break;

            case car_chase_state::move_left_3:
                if (pChase->mSideCounter == 10)
                {
                    mCar->mCarSprite->set_xyz_lazy(gCurrCarAI_TargetX, sCurrCarAITargetY, gCurrCarAI_zpos);
                    Ang16::PolarToCartesian_41FC20(sCurrCarAITargetCar->mCarSprite->mRotation - kAng90_6779E4, gF16fOne, dx, dy);
                    assert(pSprite != NULL);
                    pSprite = mCar->mCarSprite;
                    mCar->mCarSprite->set_xyz_lazy(pSprite->mXy.x + dx, pSprite->mXy.y + dy, pSprite->mZpos);

                    if (mCar->mCarSprite->CheckSpriteMovementRegion() == 0)
                    {
                        this->mNearestEntity = gSpriteGrid_1->FindCollidingSpritePreferType(this->mCar->mCarSprite, sprite_types_enum::none_0);
                        if (this->mNearestEntity == NULL)
                        {
                            pChase->mChaseState = car_chase_state::approach_left_6;
                        }
                    }
                }
                else
                {
                    sCurrCarAIDesiredSpeed = sCurrCarAITargetCar->GetCarLinearSpeed();
                    if (target_dist < gF16fOne)
                    {
                        assert(pPhysics != NULL);
                        pPhysics = this->mCar->mPhysics;
                        pPhysics->mIsFootBrakeOn = 0;
                        pPhysics->mIsForwardGasOn = 0;
                        pPhysics->mIsBackwardGasOn = 0;
                        pPhysics->mIsCoasting = false;
                    }
                    else if (sCurrCarAIVelocity < sCurrCarAIDesiredSpeed)
                    {
                        assert(pPhysics != NULL);
                        pPhysics = this->mCar->mPhysics;
                        pPhysics->mIsForwardGasOn = 1;
                        pPhysics->mIsFootBrakeOn = 0;
                        pPhysics->mIsBackwardGasOn = 0;
                        pPhysics->mIsCoasting = false;
                    }
                    else
                    {
                        assert(pPhysics != NULL);
                        pPhysics = this->mCar->mPhysics;
                        pPhysics->mIsCoasting = true;
                        pPhysics->mIsFootBrakeOn = 0;
                        pPhysics->mIsBackwardGasOn = 0;
                        pPhysics->mIsForwardGasOn = 0;
                    }

                    if (pChase->mSideCounter < 10u)
                    {
                        pChase->mSideCounter++;
                    }
                }
                break;

            case car_chase_state::choose_side_5:
            {
                switch (gCarChaseTaskTable->CountPursuers(pChase->mpTargetPed, this->mCar->mDriver))
                {
                    case 1:
                        bTryRight = 1;
                        bTryLeft = 1;
                        maxCount = gPolice_7B8->mMaxWantedLevel != cop_level_enum::police_1 ? 80 : 400;
                        break;

                    case 2:
                        if (gCarChaseTaskTable->HasPursuerInFront(this->mCar->mDriver) != 0)
                        {
                            maxCount = 10;
                            pChase->mSideCounter = 0;
                            bTryRight = 0;
                            bTryLeft = 0;
                        }
                        else
                        {
                            maxCount = 80;
                            bTryRight = 1;
                            bTryLeft = 1;
                        }
                        break;

                    case 3:
                    case 4:
                        bTryRight = 1;
                        bTryLeft = 1;
                        if (gCarChaseTaskTable->HasPursuerAlongside(mCar->mDriver) != 0)
                        {
                            if (gCarChaseTaskTable->HasPursuerOnLeft(this->mCar->mDriver) != 0)
                            {
                                bTryLeft = 0;
                                if (gCarChaseTaskTable->HasPursuerOnRight(mCar->mDriver) != 0)
                                {
                                    pChase->mSideCounter = 0;
                                    bTryRight = 0;
                                    bTryLeft = 0;
                                }
                            }
                            else if (gCarChaseTaskTable->HasPursuerOnRight(this->mCar->mDriver) != 0)
                            {
                                bTryRight = 0;
                            }
                            else
                            {
                                maxCount = 10;
                                pChase->mSideCounter = 0;
                            }
                        }
                        else
                        {
                            maxCount = 80;
                        }
                        break;

                    default:
                        maxCount = 10;
                        break;
                }

                if (pChase->mTaskType == car_task_type::follow_ped_2)
                {
                    maxCount = 100;
                    pChase->mSideCounter = 0;
                }

                MakeAgressiveSirensAndLights();

                if (pChase->mSideCounter >= maxCount)
                {
                    // Try to get beside the target, first on its left then on its right
                    if (bTryLeft != 0)
                    {
                        mCar->mCarSprite->set_xyz_lazy(gCurrCarAI_TargetX, sCurrCarAITargetY, gCurrCarAI_zpos);
                        Ang16::PolarToCartesian_41FC20(sCurrCarAITargetCar->mCarSprite->mRotation - kAng90_6779E4, gF16fOne, dx, dy);
                        assert(pSprite != NULL);
                        pSprite = mCar->mCarSprite;
                        mCar->mCarSprite->set_xyz_lazy(pSprite->mXy.x + dx, pSprite->mXy.y + dy, pSprite->mZpos);

                        if (mCar->mCarSprite->CheckSpriteMovementRegion() == 0)
                        {
                            if ((mNearestEntity = gSpriteGrid_1->FindCollidingSpritePreferType(this->mCar->mCarSprite, sprite_types_enum::none_0)) == NULL)
                            {
                                Ang16::PolarToCartesian_41FC20(mCar->mCarSprite->mRotation, gF16fOne, dx, dy);
                                assert(pSprite != NULL);
                                pSprite = mCar->mCarSprite;
                                mCar->mCarSprite->set_xyz_lazy(pSprite->mXy.x + dx, pSprite->mXy.y + dy, pSprite->mZpos);
                                if ((mNearestEntity = gSpriteGrid_1->FindCollidingSpritePreferType(this->mCar->mCarSprite, sprite_types_enum::none_0)) == NULL)
                                {
                                    pChase->mChaseState = car_chase_state::approach_left_6;
                                    return;
                                }
                            }
                        }
                    }

                    if (bTryRight != 0)
                    {
                        mCar->mCarSprite->set_xyz_lazy(gCurrCarAI_TargetX, sCurrCarAITargetY, gCurrCarAI_zpos);
                        Ang16::PolarToCartesian_41FC20(sCurrCarAITargetCar->mCarSprite->mRotation + kAng90_6779E4, gF16fOne, dx, dy);
                        assert(pSprite != NULL);
                        pSprite = mCar->mCarSprite;
                        mCar->mCarSprite->set_xyz_lazy(pSprite->mXy.x + dx, pSprite->mXy.y + dy, pSprite->mZpos);

                        if (mCar->mCarSprite->CheckSpriteMovementRegion() == 0)
                        {
                            if ((mNearestEntity = gSpriteGrid_1->FindCollidingSpritePreferType(this->mCar->mCarSprite, sprite_types_enum::none_0)) == NULL)
                            {
                                Ang16::PolarToCartesian_41FC20(mCar->mCarSprite->mRotation, gF16fOne, dx, dy);
                                assert(pSprite != NULL);
                                pSprite = mCar->mCarSprite;
                                mCar->mCarSprite->set_xyz_lazy(pSprite->mXy.x + dx, pSprite->mXy.y + dy, pSprite->mZpos);
                                if ((mNearestEntity = gSpriteGrid_1->FindCollidingSpritePreferType(this->mCar->mCarSprite, sprite_types_enum::none_0)) == NULL)
                                {
                                    pChase->mChaseState = car_chase_state::approach_right_7;
                                }
                            }
                        }
                    }
                }
                else
                {
                    sCurrCarAIDesiredSpeed = sCurrCarAITargetCar->GetCarLinearSpeed();
                    if (this->mMaxSpeed < sCurrCarAIDesiredSpeed)
                    {
                        sCurrCarAIDesiredSpeed = this->mMaxSpeed;
                    }

                    if (target_dist < gF16fOne)
                    {
                        if (sCurrCarAIVelocity > sCurrCarAIDesiredSpeed + kFpPoint02_677B5C)
                        {
                            mCar->DoBrake();
                            return;
                        }
                        pPhysics = this->mCar->mPhysics;
                        pPhysics->mIsFootBrakeOn = 0;
                        pPhysics->mIsForwardGasOn = 0;
                        pPhysics->mIsBackwardGasOn = 0;
                        pPhysics->mIsCoasting = false;
                        return;
                    }

                    if (sCurrCarAIVelocity < sCurrCarAIDesiredSpeed)
                    {
                        pPhysics = this->mCar->mPhysics;
                        pPhysics->mIsForwardGasOn = 1;
                        pPhysics->mIsFootBrakeOn = 0;
                        pPhysics->mIsBackwardGasOn = 0;
                        pPhysics->mIsCoasting = false;
                    }
                    else if (sCurrCarAIVelocity > sCurrCarAIDesiredSpeed + kFpPoint02_677B5C)
                    {
                        mCar->DoBrake();
                    }
                    else
                    {
                        mCar->mPhysics->ForceNeutralInput();
                    }

                    if (pChase->mSideCounter < maxCount)
                    {
                        ++pChase->mSideCounter;
                    }
                    pChase->mSettleCounter = 100;
                }
                break;
            }

            case car_chase_state::move_right_4:
                assert(sCurrCarAITargetCar != NULL);
                if (pChase->mSideCounter >= 10u)
                {
                    mCar->mCarSprite->set_xyz_lazy(gCurrCarAI_TargetX, sCurrCarAITargetY, gCurrCarAI_zpos);
                    Ang16::PolarToCartesian_41FC20(sCurrCarAITargetCar->mCarSprite->mRotation + kAng90_6779E4, gF16fOne, dx, dy);
                    mCar->mCarSprite->set_xyz_lazy(mCar->mCarSprite->mXy.x + dx, mCar->mCarSprite->mXy.y + dy, mCar->mCarSprite->mZpos);
                    if (mCar->mCarSprite->CheckSpriteMovementRegion() == 0)
                    {
                        if ((this->mNearestEntity = gSpriteGrid_1->FindCollidingSpritePreferType(this->mCar->mCarSprite, sprite_types_enum::none_0)) == NULL)
                        {
                            pChase->mChaseState = car_chase_state::approach_right_7;
                        }
                    }
                }
                else
                {
                    sCurrCarAIDesiredSpeed = sCurrCarAITargetCar->GetCarLinearSpeed();
                    if (target_dist < gF16fOne)
                    {
                        mCar->mPhysics->NeutralGear();
                    }
                    else if (sCurrCarAIVelocity < sCurrCarAIDesiredSpeed)
                    {
                        mCar->mPhysics->Accelerate();
                    }
                    else
                    {
                        mCar->mPhysics->ForceNeutralInput();
                    }

                    if (pChase->mSideCounter < 10u)
                    {
                        pChase->mSideCounter++;
                    }
                }
                break;

            case car_chase_state::approach_left_6:
                if (target_dist < kFpHalf_677A84 + kFpEighth)
                {
                    pChase->mChaseState = car_chase_state::alongside_left_8;
                }
                else
                {
                    mCar->mPhysics->Accelerate();
                }
                break;

            case car_chase_state::approach_right_7:
                if (target_dist < kFpHalf_677A84 + kFpEighth)
                {
                    pChase->mChaseState = car_chase_state::alongside_right_9;
                }
                else
                {
                    mCar->mPhysics->Accelerate();
                }
                break;

            case car_chase_state::alongside_left_8:
            case car_chase_state::alongside_right_9:
                assert(sCurrCarAITargetCar != NULL);
                sCurrCarAIDesiredSpeed = sCurrCarAITargetCar->GetCarLinearSpeed();
                if (target_dist < kFpQuarter_6779BC)
                {
                    if (sCurrCarAIVelocity > sCurrCarAIDesiredSpeed)
                    {
                        mCar->DoBrake();
                    }
                    else
                    {
                        mCar->mPhysics->NeutralGear();
                    }
                }
                else if (target_dist > kFpThreeQuarters)
                {
                    mCar->mPhysics->Accelerate();
                }
                else if (sCurrCarAIVelocity < sCurrCarAIDesiredSpeed)
                {
                    mCar->mPhysics->Accelerate();
                }
                else if (sCurrCarAIVelocity > sCurrCarAIDesiredSpeed + kFpPoint02_677B5C)
                {
                    mCar->mPhysics->NeutralGear();
                }
                else
                {
                    mCar->mPhysics->ForceNeutralInput();
                }
                break;

            case car_chase_state::pull_ahead_left_10:
            case car_chase_state::pull_ahead_right_11:
                if (target_dist < kFpQuarter_6779BC)
                {
                    pChase->mChaseState = car_chase_state::in_front_12;
                }
                break;

            case car_chase_state::in_front_12:
            case car_chase_state::in_front_stopping_13:
                if (target_dist < gF16fOne + kFpHalf_677A84)
                {
                    ++pChase->mBlockCounter;
                    mCar->mPhysics->NeutralGear();
                    pChase->mChaseState = car_chase_state::in_front_stopping_13;
                }
                break;

            default:
                break;
        }
    }
    else
    {
        if (this->mCar->mpChaseTask != NULL)
        {
            if (static_cast<u16>(pChase->mSettleCounter) > 0)
            {
                pChase->mSettleCounter--;
            }
            else if (pChase->mTaskType != car_task_type::goto_position_1)
            {
                pChase->mChaseState = car_chase_state::in_front_stopping_13;
                if (gCarChaseTaskTable->FindDriverInState(car_chase_state::follow_behind_0, pChase->mpTargetPed) != NULL)
                {
                    pChase->mChaseState = car_chase_state::follow_behind_2;
                }
                else
                {
                    pChase->mChaseState = gCarChaseTaskTable->FindDriverInState(car_chase_state::choose_side_5, pChase->mpTargetPed) ? 0 : 2;
                }
                pChase->mWaitCounter = 0;
            }
        }
    }
}

MATCH_FUNC(0x451980)
void CarAI::ReactToNearbyCar()
{
    Fix16 near_dist = gF16fOne;
    bool bNoBrake = false;
    bool bEmergencyDriver = false;
    Car* pCar = mNearestEntity->mCarBcPtr;

    if (mManeuverActive)
    {
        if (ComputeShortestAngleDelta(pCar->mCarSprite->mRotation, mCar->mCarSprite->mRotation) > kAng90_6779E4)
        {
            bNoBrake = true;
        }
    }

    if (pCar)
    {
        if (pCar->mDriver)
        {
            if (pCar->mDriver->mThreatReaction == threat_reaction_enum::react_as_emergency_1)
            {
                bEmergencyDriver = true;
            }
        }
    }

    Fix16 dist_to_target = MaxAbsDistanceRawY_451980(gCurrCarAI_xpos,
                                         gCurrCarAI_ypos,
                                         pCar->mCarSprite->mXy.x,
                                         pCar->mCarSprite->mXy.y);

    if (dist_to_target > kFpTwo_677B98)
    {
        if (sCurrCarAIIsChasing || mBf24.bDriveAggressively)
        {
            bNoBrake = true;
        }
        if (pCar->mPedType == ped_type::player_2)
        {
            DetectCarAhead();
        }
        else if (ComputeShortestAngleDelta(pCar->mCarSprite->mRotation, mCar->mCarSprite->mRotation) > kAng45_677A38)
        {
            bNoBrake = true;
        }
        if (bEmergencyDriver)
        {
            bNoBrake = false;
        }
    }
    else
    {
        if (dist_to_target > near_dist)
        {
            bNoBrake = true;
            if (ComputeShortestAngleDelta(pCar->mCarSprite->mRotation, mCar->mCarSprite->mRotation) > kAng90_6779E4)
            {
                if (mBf24.bDriveAggressively)
                {
                    bNoBrake = true;
                }
                else
                {
                    sCurrCarAIPedalsFree = 0;
                    mCar->mPhysics->NeutralGear();
                }
                mCar->TryHonkHorn(2);
            }
            else if (mBf24.bHasRightOfWay)
            {
                sCurrCarAIPedalsFree = 0;
                mCar->DoBrake();
            }
            else if (sCurrCarAIIsChasing && sCurrCarAITargetCar)
            {
                if (sCurrCarAITargetCar == pCar)
                {
                    sCurrCarAIDesiredSpeed = pCar->GetCarLinearSpeed();

                    if (mCar->mpChaseTask->mTaskType == car_task_type::follow_ped_2)
                    {
                        if (sCurrCarAIDesiredSpeed == kF16Zero)
                        {
                            mCar->DoBrake();
                        }
                        else if (sCurrCarAIVelocity >= sCurrCarAIDesiredSpeed)
                        {
                            mCar->mPhysics->ForceNeutralInput();
                        }
                        else
                        {
                            mCar->mPhysics->Accelerate();
                        }
                    }
                    else if (sCurrCarAIVelocity < sCurrCarAIDesiredSpeed)
                    {
                        mCar->mPhysics->Accelerate();
                    }
                    else
                    {
                        mCar->mPhysics->ForceNeutralInput();
                    }
                }
                else
                {
                    sCurrCarAIPedalsFree = 0;
                    mCar->mPhysics->NeutralGear();
                }
            }
            else if (mBf24.bDriveAggressively)
            {
                bNoBrake = true;
            }
            else
            {
                sCurrCarAIPedalsFree = 0;
                mCar->mPhysics->NeutralGear();
            }
        }
        else if (pCar->mPedType == ped_type::player_2)
        {
            if (mBf24.bDriveAggressively)
            {
                bNoBrake = true;
            }
            else
            {
                sCurrCarAIPedalsFree = 0;
                mCar->mPhysics->NeutralGear();
            }
            mCar->TryHonkHorn(2);
        }
        else
        {
            if (ComputeShortestAngleDelta(pCar->mCarSprite->mRotation, mCar->mCarSprite->mRotation) > kAng90_6779E4)
            {
                bNoBrake = true;
            }

            if ((mFlags & 0x20000) == 0x20000)
            {
                bNoBrake = true;
            }
        }
    }

    if (mBf24.bChangingLane)
    {
        bNoBrake = true;
    }

    if (bNoBrake)
    {
        if (sCurrCarAIIsChasing)
        {
            if (!mCar->mpChaseTask->mFollowingRoute)
            {
                if (!pCar->mpChaseTask || mCar->mpChaseTask->mpTargetPed != pCar->mpChaseTask->mpTargetPed)
                {
                    if (pCar)
                    {
                        if (pCar != sCurrCarAITargetCar)
                        {
                            if (pCar->mLastSeenTimer > 5)
                            {
                                if (pCar->mPedType == ped_type::dummy_3)
                                {
                                    pCar->MarkForDespawn();
                                }
                            }
                            else
                            {
                                Fix16 dx = pCar->mCarSprite->mXy.x - gCurrCarAI_xpos;
                                Fix16 dy = pCar->mCarSprite->mXy.y - gCurrCarAI_ypos;

                                // Copied out of the returned temporary, so bearing_to_other lives in a register (bp)
                                Ang16 bearing_to_other;
                                bearing_to_other = Fix16::atan2_fixed(dy, dx);

                                if (mCar->mpChaseTask)
                                {
                                    switch (mCar->mpChaseTask->mChaseState)
                                    {
                                        case car_chase_state::move_left_3:
                                        case car_chase_state::approach_left_6:
                                        case car_chase_state::alongside_left_8:
                                            mCar->mPhysics->TurnAntiClockwise();
                                            sCurrCarAIIsChasing = false;
                                            sCurrCarAIAllowTrafficSteering = false;
                                            return;
                                        case car_chase_state::move_right_4:
                                        case car_chase_state::approach_right_7:
                                        case car_chase_state::alongside_right_9:
                                            mCar->mPhysics->TurnClockwise();
                                            sCurrCarAIIsChasing = false;
                                            sCurrCarAIAllowTrafficSteering = false;
                                            return;
                                        default:
                                            break;
                                    }
                                }

                                if (bNoBrake)
                                {
                                    Ang16 our_heading;
                                    Ang16 their_heading;

                                    if (mCurrDirection != car_ai_direction::south_2)
                                    {
                                        our_heading = mAngle;
                                        their_heading = bearing_to_other;
                                    }
                                    else
                                    {
                                        our_heading = mAngle + kAng180_677ADE;
                                        their_heading = bearing_to_other + kAng180_677ADE;
                                    }

                                    if (our_heading < their_heading)
                                    {
                                        mCar->mPhysics->TurnClockwise();
                                    }
                                    else
                                    {
                                        mCar->mPhysics->TurnAntiClockwise();
                                    }
                                }

                                sCurrCarAIIsChasing = false;
                                sCurrCarAIAllowTrafficSteering = false;
                            }
                        }
                    }
                }
            }
        }
    }
    else if (sCurrCarAIVelocity == kF16Zero)
    {
        if ((mFlags & 0x90) == 0)
        {
            ReverseOrNeutral();
        }
        else
        {
            mCar->mPhysics->NeutralGear();
        }
        sCurrCarAIPedalsFree = 0;
    }
    else
    {
        sCurrCarAIPedalsFree = 0;
        if (!mBf24.bCollidedWithCar || mCarInCollision->mPedType != ped_type::player_2)
        {
            if (sCurrCarAIVelocity > kFpPoint4)
            {
                mCar->DoBrakeAndHandbrake();
            }
            else
            {
                mCar->DoBrake();
            }
        }
    }
}

MATCH_FUNC(0x451fa0)
void CarAI::ReactToNearbyObject()
{
    if (!sCurrCarAIIsChasing)
    {
        Object* pObj = mNearestEntity->AsObject2C();
        if (pObj->mModel == objects::roadblock_21 || pObj->mModel == objects::pedestrian_crossing_marker_122)
        {
            mCar->DoBrakeAndHandbrake();
            sCurrCarAIPedalsFree = 0;
            this->mBf24.bFlag4 = true;
        }
    }
}

MATCH_FUNC(0x451ff0)
void CarAI::ReactToNearbyPed()
{
    if (!mBf24.bDriveAggressively)
    {
        if (!sCurrCarAIIsChasing || (mCar->mpChaseTask->mTaskType != car_task_type::kill_ped_5) ||
            mCar->mpChaseTask->mpTargetPed != mNearestEntity->mCharB4Ptr->mpPed)
        {
            if (sCurrCarAIVelocity > kFpPoint4)
            {
                mCar->DoBrakeAndHandbrake();
                sCurrCarAIPedalsFree = 0;
                mCar->TryHonkHorn(2);
                return;
            }
            mCar->DoBrake();
            sCurrCarAIPedalsFree = 0;
        }
    }
    mCar->TryHonkHorn(2);
}

// 9.6f 0x42ACE0 (a Car method there): sets the flag Object_5C reads while the car probes ahead
static inline void SetProbingAhead(u8 value)
{
    gCarAIProbingAhead = value;
}

MATCH_FUNC(0x452060)
void CarAI::ScanAheadForObstacles()
{
    Fix16 probe_height;
    probe_height = gF16fOne;

    sCurrCarAIDoSpeedControl = 1;
    sCurrCarAIPedalsFree = 1;

    Fix16 probe_step = gF16fOne;

    Car* pCar = this->mCar;

    if (pCar->mPedType == ped_type::player_2)
    {
        this->mBf24.bFlag4 = false;
        probe_height = gF16fOne;
    }

    Fix16 new_z;
    if (sbOnGradientSlope)
    {
        new_z = probe_height + pCar->mCarSprite->mZpos;
    }
    else
    {
        new_z = pCar->mCarSprite->mZpos;
    }

    Fix16 saved_z = new_z;

    Fix16 offset_x;
    Fix16 offset_y;
    Ang16::PolarToCartesian_41FC20(this->mAngle, probe_height + kFpThreeQuarters, offset_x, offset_y);
    mCar->mCarSprite->set_xyz_lazy(offset_x + mCar->mCarSprite->mXy.x,
                                                          offset_y + mCar->mCarSprite->mXy.y,
                                                          new_z);

    SetProbingAhead(1);

    if (this->mBf24.bTurning)
    {
        mCar->mCarSprite->set_xyz_lazy(gCurrCarAI_xpos, gCurrCarAI_ypos, gCurrCarAI_zpos);

        Ang16::PolarToCartesian_41FC20(this->mAngle + kAng32.MultiplyByFix16_401CB0_ctor_ool(Fix16(this->mCar->mPhysics->mTurnDirection)),
                                       gF16fOne, offset_x, offset_y);
        this->mCar->mCarSprite->set_xyz_lazy(offset_x + mCar->mCarSprite->mXy.x,
                                                                    offset_y + mCar->mCarSprite->mXy.y,
                                                                    new_z);

        this->mNearestEntity = gSpriteGrid_1->FindCollidingSpritePreferType(this->mCar->mCarSprite, sprite_types_enum::none_0);
    }
    else
    {
        this->mNearestEntity = gSpriteGrid_1->FindCollidingSpritePreferType(this->mCar->mCarSprite, sprite_types_enum::none_0);
        if (!this->mNearestEntity || sbOnGradientSlope)
        {
            mCar->mCarSprite->set_xyz_lazy(gCurrCarAI_xpos, gCurrCarAI_ypos, gCurrCarAI_zpos);

            Ang16::PolarToCartesian_41FC20(this->mAngle, probe_step, offset_x, offset_y);
            mCar->mCarSprite->set_xyz_lazy(offset_x + mCar->mCarSprite->mXy.x,
                                                                  offset_y + mCar->mCarSprite->mXy.y,
                                                                  new_z);

            this->mNearestEntity = gSpriteGrid_1->FindCollidingSpritePreferType(this->mCar->mCarSprite, sprite_types_enum::none_0);
        }
    }

    if (!this->mNearestEntity)
    {
        if (sbOnGradientSlope)
        {
            Fix16 probe_y = offset_y + mCar->mCarSprite->mXy.y;
            Fix16 probe_x = offset_x + mCar->mCarSprite->mXy.x;

            mCar->mCarSprite->set_xyz_lazy(probe_x, probe_y, saved_z - gF16fOne);
            new_z = saved_z;
            this->mNearestEntity = gSpriteGrid_1->FindCollidingSpritePreferType(this->mCar->mCarSprite, sprite_types_enum::none_0);
        }
    }

    mCar->mCarSprite->set_xyz_lazy(gCurrCarAI_xpos, gCurrCarAI_ypos, gCurrCarAI_zpos);

    char_type bCheckMovement;

    if (mBf24.bReversing)
    {
        Ang16::PolarToCartesian_41FC20(this->mAngle, kFpHalf_677A84, offset_x, offset_y);
        mCar->mCarSprite->set_xyz_lazy(offset_x + mCar->mCarSprite->mXy.x,
                                                              offset_y + mCar->mCarSprite->mXy.y,
                                                              new_z);
        bCheckMovement = mCar->mCarSprite->CheckSpriteMovementRegion();
        if (!bCheckMovement)
        {
            Ang16::PolarToCartesian_41FC20(this->mAngle, kFpHalf_677A84, offset_x, offset_y);
            mCar->mCarSprite->set_xyz_lazy(offset_x + mCar->mCarSprite->mXy.x,
                                                                  offset_y + mCar->mCarSprite->mXy.y,
                                                                  new_z);
            bCheckMovement = mCar->mCarSprite->CheckSpriteMovementRegion();
            if (!bCheckMovement)
            {
                Ang16::PolarToCartesian_41FC20(this->mAngle, kFpHalf_677A84, offset_x, offset_y);
                mCar->mCarSprite->set_xyz_lazy(offset_x + mCar->mCarSprite->mXy.x,
                                                                      offset_y + mCar->mCarSprite->mXy.y,
                                                                      new_z);
                bCheckMovement = mCar->mCarSprite->CheckSpriteMovementRegion();
            }
        }
    }
    else
    {
        Ang16::PolarToCartesian_41FC20(this->mAngle, gF16fOne, offset_x, offset_y);
        mCar->mCarSprite->set_xyz_lazy(offset_x + mCar->mCarSprite->mXy.x,
                                                              offset_y + mCar->mCarSprite->mXy.y,
                                                              new_z);
        bCheckMovement = mCar->mCarSprite->CheckSpriteMovementRegion();
    }

    SetProbingAhead(0);
    if (sbOnGradientSlope)
    {
        if (bCheckMovement)
        {
            bCheckMovement = 0;
        }
    }

    Sprite* pNearestEntity = this->mNearestEntity;
    if (!pNearestEntity && !bCheckMovement)
    {
        if (mBf24.bReversing)
        {
            if (this->mObstacleTimer > 0xAu)
            {
                this->mObstacleTimer = 0;
                this->mBf24.bReversing = false;
            }
            else
            {
                sCurrCarAIDoSpeedControl = 0;
            }
            ++this->mObstacleTimer;
        }

        if (!sCurrCarAIIsChasing || this->mCar->mpChaseTask->mFollowingRoute)
        {
            DetectCarAhead();
        }
    }
    else if (bCheckMovement)
    {
        sCurrCarAIIsChasing = false;
        this->mFlags &= ~0x80u;
        if (sCurrCarAIVelocity == kF16Zero)
        {
            this->mObstacleTimer = 0;
            this->mFlags |= 0x40;
            sCurrCarAIDoSpeedControl = 0;
            ReverseOrNeutral();
        }
        else if (!this->mBf24.bReversing)
        {
            sCurrCarAIDoSpeedControl = 1;
            sCurrCarAIPedalsFree = 0;
            if (sCurrCarAIVelocity > kFpPoint4)
            {
                mCar->DoBrakeAndHandbrake();
            }
            else
            {
                mCar->DoBrake();
            }
        }
        else
        {
            sCurrCarAIDoSpeedControl = 0;
        }
    }
    else
    {
        // The four block checks each jump to the shared react switch, as in the original.
        if (pNearestEntity->mZpos.GetFracValue() == kF16Zero)
        {
            if (sbOnGradientSlope)
            {
                if (gMap_0x370->IsBlockRoadType(pNearestEntity->mXy.x.ToInt(),
                                                              pNearestEntity->mXy.y.ToInt(),
                                                              (pNearestEntity->mZpos - gF16fOne).ToInt()))
                {
                    goto react;
                }
            }
            else if (gMap_0x370->IsBlockRoadType(pNearestEntity->mXy.x.ToInt(),
                                                               pNearestEntity->mXy.y.ToInt(),
                                                               (pNearestEntity->mZpos - gF16fOne).ToInt()))
            {
                goto react;
            }
        }
        else if (sbOnGradientSlope)
        {
            if (gMap_0x370->IsBlockRoadType(pNearestEntity->mXy.x.ToInt(), pNearestEntity->mXy.y.ToInt(), pNearestEntity->mZpos.ToInt()))
            {
                goto react;
            }
        }
        else if (gMap_0x370->IsBlockRoadTypeInlined_433470(pNearestEntity->mXy.x.ToInt(),
                                                                   pNearestEntity->mXy.y.ToInt(),
                                                                   pNearestEntity->mZpos.ToInt()))
        {
        react:
            switch (this->mNearestEntity->GetType())
            {
                case sprite_types_enum::object_1:
                case sprite_types_enum::code_obj1_4:
                case sprite_types_enum::map_obj_5:
                    ReactToNearbyObject();
                    break;

                case sprite_types_enum::car_2:
                    ReactToNearbyCar();
                    break;

                case sprite_types_enum::ped_3:
                    if (!mBf24.bTurning)
                    {
                        ReactToNearbyPed();
                    }
                    break;
            }
        }
    }

    if (this->mYieldToCar)
    {
        if (this->mYieldToCar->mAI)
        {
            if (this->mYieldToCar->mAI->mBf24.bHasRightOfWay)
            {
                sCurrCarAIPedalsFree = 0;
                mCar->DoBrakeAndHandbrake();
            }
        }

        if (this->mYieldTimer > 0)
        {
            this->mYieldTimer--;
        }
        else
        {
            this->mYieldToCar = NULL;
        }
    }

    AlignToLaneCenter();

    if (sCurrCarAIDoSpeedControl)
    {
        if (!sCurrCarAIPedalsFree)
        {
            return;
        }

        if ((mFlags & 0x80u) == 0 && (mFlags & 0x4500) == 0)
        {
            if (sCurrCarAIVelocity < sCurrCarAIDesiredSpeed)
            {
                CarPhysics* pPhysics = this->mCar->mPhysics;
                pPhysics->mIsForwardGasOn = 1;
                pPhysics->mIsFootBrakeOn = 0;
                pPhysics->mIsBackwardGasOn = 0;
                pPhysics->mIsCoasting = false;
                return;
            }

            CarPhysics* pPhysics = this->mCar->mPhysics;
            pPhysics->mIsCoasting = true;
            pPhysics->mIsFootBrakeOn = 0;
            pPhysics->mIsBackwardGasOn = 0;
            pPhysics->mIsForwardGasOn = 0;
            return;
        }

        Fix16 target_speed = kFpPoint06;
        if (kFpPoint06 > this->mMaxSpeed)
        {
            target_speed = this->mMaxSpeed;
        }

        if (sCurrCarAIVelocity < target_speed)
        {
            CarPhysics* pPhysics = this->mCar->mPhysics;
            pPhysics->mIsForwardGasOn = 1;
            pPhysics->mIsFootBrakeOn = 0;
            pPhysics->mIsBackwardGasOn = 0;
            pPhysics->mIsCoasting = false;
            return;
        }

        if (sCurrCarAIVelocity > kFpPoint1_6779B8)
        {
            mCar->DoBrakeAndHandbrake();
        }
        else if (sCurrCarAIVelocity > kFpPoint09_677B78)
        {
            mCar->DoBrake();
        }
        else if (sCurrCarAIVelocity > target_speed)
        {
            CarPhysics* pPhysics = this->mCar->mPhysics;
            pPhysics->mIsCoasting = true;
            pPhysics->mIsFootBrakeOn = 0;
            pPhysics->mIsBackwardGasOn = 0;
            pPhysics->mIsForwardGasOn = 0;
        }
    }
    else
    {
        ReverseOrNeutral();
    }
}

// https://decomp.me/scratch/zCa7m
MATCH_FUNC(0x452a20)
void CarAI::ManageCollisions()
{
    Fix16_Point impact_point;

    if (mBf24.bCollidedWithCar)
    {
        if (mCarInCollision->IsDespawning())
        {
            return;
        }

        if (mSeekingRoad == 1)
        {
            mSeekingRoad = 0;
            mBf24.bReversing = false;
            mBf24.bTurning = false;
        }

        if (sCurrCarAIIsChasing)
        {
            if (mCar->mpChaseTask)
            {
                if (mCar->mpChaseTask->mRamTarget)
                {
                    // Past the inline budget: the second multiply is out of line
                    impact_point.FromPolar_41E210(kF16Zero, kAng0_677CE8);
                    if (sCurrCarAITargetCar)
                    {
                        if (sCurrCarAITargetCar == mCarInCollision)
                        {
                            sCurrCarAITargetCar->AccumulateDamage(500, &impact_point);
                        }
                    }
                }
            }
        }
        mFlags &= ~0x30100u;
        sCurrCarAIIsChasing = false;
        if (mCar->mpChaseTask)
        {
            if (mCarInCollision->mpChaseTask)
            {
                mCar->mpChaseTask->mWaitCounter = 0;
                mCar->mpChaseTask->mSettleCounter = 0;
                mCar->mpChaseTask->mSideCounter = 0;
            }
            else
            {
                mCar->mpChaseTask->mWaitCounter = 0;
                mCar->mpChaseTask->mSettleCounter = 0;
                mCar->mpChaseTask->mSideCounter = 0;
            }
        }

        Fix16 rotX;
        Fix16 rotY;
        // Past the inline budget: Normalize and all but the first Fix16 operator go out of line
        RotateAndTranslatePoint(mCarInCollision->mCarSprite->mXy.x,
                                       mCarInCollision->mCarSprite->mXy.y,
                                       -mAngle,
                                       gCurrCarAI_xpos,
                                       gCurrCarAI_ypos,
                                       rotX,
                                       rotY);

        // rotX is reused for the sum (a separate local moves it to the stack)
        rotX = rotX + mCar->mCarSprite->mXy.x;

        Ped* pDriver = mCarInCollision->mDriver;

        if (pDriver && pDriver->PedTypeIs(ped_type::player_2) && !mBf24.bDriveAggressively)
        {
            mCar->mPhysics->NeutralGear();
            sCurrCarAIAllowTrafficSteering = false;
            return;
        }

        if (mBf24.bDriveAggressively)
        {
            if (rotX > mCar->mCarSprite->mXy.x)
            {
                mCar->mPhysics->TurnClockwise();
            }
            else
            {
                mCar->mPhysics->TurnAntiClockwise();
            }
        }
        else if (mStoppedTimer > 0 && mStoppedTimer < 20)
        {
            if (rotX > mCar->mCarSprite->mXy.x)
            {
                mCar->mPhysics->TurnClockwise();
            }
            else
            {
                mCar->mPhysics->TurnAntiClockwise();
            }
        }
        else if (mStoppedTimer > 20)
        {
            if (rotX > mCar->mCarSprite->mXy.x)
            {
                mCar->mPhysics->TurnAntiClockwise();
            }
            else
            {
                mCar->mPhysics->TurnClockwise();
            }
        }
        else
        {
            mCar->mPhysics->NeutralGear();
            return;
        }

        if (mNearestEntity)
        {
            if (mNearestEntity->mCarBcPtr == mCarInCollision)
            {
                mCar->mPhysics->Accelerate();
            }
            else
            {
                mCar->mPhysics->NeutralGear();
            }
        }
        else
        {
            mCar->mPhysics->NeutralGear();
        }
        sCurrCarAIAllowTrafficSteering = false;

        if (mBf24.bDriveAggressively && mStoppedTimer < 60 && sCurrCarAIVelocity < kFpPoint03)
        {
            mCar->mPhysics->Accelerate();
        }

        if (!mAccelCooldown)
        {
            mAccelCooldown = 4;
        }
    }

    if (mAccelCooldown > 0)
    {
        mAccelCooldown--;
    }
}

// https://decomp.me/scratch/uZlwP
MATCH_FUNC(0x452df0)
void CarAI::UpdateDrivingAI()
{

    sCurrCarAIAllowTrafficSteering = true;
    bool bUpdateStateMachine = false;
    gCurrCarAI_xpos = mCar->mCarSprite->mXy.x;
    gCurrCarAI_ypos = mCar->mCarSprite->mXy.y;
    gCurrCarAI_zpos = mCar->mCarSprite->mZpos;
    sCurrCarAITargetCar = NULL;
    gCurrCarAI_HasChaseUpdate_677CA8 = 0;

    if (mCar->mpChaseTask && !mSeekingRoad)
    {
        UpdateChase();
        gCurrCarAI_HasChaseUpdate_677CA8 = 1;
    }
    else
    {
        sCurrCarAIIsChasing = false;
    }

    gmp_block_info* pBlock = gMap_0x370->GetBlock(mCar->mCarSprite->mXy.x.ToInt(),
                                                                 mCar->mCarSprite->mXy.y.ToInt(),
                                                                 mCar->mCarSprite->mZpos.ToInt());

    if (pBlock && IS_GRADIENT_SLOPE(pBlock->mSlopeType) &&
        GET_BLOCK_TYPE(pBlock->mSlopeType) != AIR)
    {
        sbOnGradientSlope = true;
    }
    else
    {
        pBlock = gMap_0x370->GetBlock(mCar->mCarSprite->mXy.x.ToInt(),
                                                     mCar->mCarSprite->mXy.y.ToInt(),
                                                     (mCar->mCarSprite->mZpos.ToInt()) - 1);
        sbOnGradientSlope = false;
    }

    if (pBlock)
    {
        sCurrCarAIArrowDirection = gMap_0x370->GetArrowDirectionFromBlock(pBlock, true);
    }
    else
    {
        sCurrCarAIArrowDirection = 0;
    }

    mBf24.bArrowSteerAssist = true;
    sCurrCarAIWentStraight = 0;

    mCurrDirection = Ang16::GetAngleFace(mAngle);
    sCurrCarAIDesiredSpeed = mMaxSpeed;
    sCurrCarAISpriteHeight = gPixelsToFix16.list[gGtx_0x106C->get_car_info_5AA3B0(mCar->mCarInfoIdx)->h];

    mFlags = mFlags & ~0x200000u | (mCar->mDriver->mBf21C.bDriveAggressively << 21);

    if (sCurrCarAIIsChasing)
    {
        if (!mCar->mpChaseTask->mFollowingRoute)
        {
            bUpdateStateMachine = true;
            mBf24.bDriveAggressively = true;
        }
    }

    if (mSeekingRoad)
    {
        ++mOffRoadTimer;
        if (mCar->mPedType != ped_type::script_created_5)
        {
            if (mOffRoadTimer > 200)
            {
                mCar->mAbandoned = true;
            }
        }
        else
        {
            mOffRoadTimer = 0;
        }

        bUpdateStateMachine = false;
        if (mBf24.bReversing)
        {
            mSeekRoadTurn = 3;
        }

        switch (mSeekRoadTurn)
        {
            case 0:
                mFlags &= ~0x20C0;
                mCar->mPhysics->SetGoStraight();
                break;

            case 1:
                mFlags &= ~0x2040;
                mCar->mPhysics->TurnAntiClockwise();
                mBf24.bTurning = true;
                break;

            case 2:
                mFlags &= ~0x2040;
                mCar->mPhysics->TurnClockwise();
                mBf24.bTurning = true;
                break;

            case 3:
                mFlags = (mFlags & ~0x2080) | 0x40;
                break;

            default:
                break;
        }
    }

    ScanAheadForObstacles();

    if (sCurrCarAIVelocity != kF16Zero)
    {
        if (mCar->mPhysics->mVelReadOnly.GetLength_41E260() == kF16Zero)
        {
            ++mStoppedTimer;
        }
        else
        {
            mStoppedTimer = 0;
        }
    }
    else
    {
        ++mStoppedTimer;
    }

    ManageCollisions();

    if (bUpdateStateMachine)
    {
        UpdateStateMachine();
    }

    if (mCar->mpChaseTask && mCar->mpChaseTask->mTaskType == car_task_type::follow_ped_2)
    {
        sCurrCarAIIsChasing = true;
    }
    else if (!sCurrCarAIIsChasing)
    {
        if (mNearestEntity && mNearestEntity->GetType() == sprite_types_enum::object_1 &&
            mNearestEntity->AsObject2C()->mModel == objects::pedestrian_crossing_marker_122)
        {
            mBf24.bFlag4 = true;
            mNearestEntity = NULL;
        }
        else
        {
            mBf24.bFlag4 = false;
        }
    }

    if (mRouteIdx > 0 && mBf24.bFlag4)
    {
        if (mCar->mpChaseTask)
        {
            if (mCar->mpChaseTask->mFollowingRoute == 1)
            {
                DoShortcutsUsingJunctions();
            }
        }
    }

    if (!sCurrCarAIAllowTrafficSteering || mAccelCooldown)
    {
        mBf24.bArrowSteerAssist = false;
    }
    else
    {
        ManageTrafficCarDirection();
    }

    if (sCurrCarAIWentStraight)
    {
        mBf24.bArrowSteerAssist = true;
    }

    mCarInCollision = NULL;
    mBf24.bCollidedWithCar = false;
}

MATCH_FUNC(0x453470)
void CarAI::UpdateDriving()
{
    this->mCar->mPhysics->SetHandBrakeOff();
    Fix16 speed = mCar->mPhysics->GetLinearSpeed();
    sCurrCarAIVelocity = speed;

    if (this->mCar->mAbandoned)
    {
        this->mCar->SetAIHalted();
    }

    if (this->mForcedStopTimer > 0)
    {
        this->mForcedStopTimer--;

        if (this->mForcedStopTimer)
        {
            this->mCar->SetAIHalted();
        }
        else
        {
            this->mCar->ClearAIHalted();
        }
    }

    if (this->mCar->IsAIHalted())
    {
        if (this->mBf24.bAIHalted)
        {
            if (sCurrCarAIVelocity > kFpPoint05_677B60)
            {
                mCar->DoBrakeAndHandbrake();
            }
            else
            {
                mCar->DoBrake();
            }
            this->mCarInCollision = NULL;
            this->mBf24.bCollidedWithCar = false;
        }
        else
        {
            if (sCurrCarAIVelocity > kFpPoint05_677B60)
            {
                mCar->DoBrakeAndHandbrake();
            }
            else
            {
                mCar->mPhysics->NeutralGear();
            }
            this->mCarInCollision = NULL;
            this->mBf24.bCollidedWithCar = false;
        }
    }
    else
    {
        UpdateDrivingAI();
    }
}

MATCH_FUNC(0x4537d0)
void CarAI::ChooseRandomTurn()
{
    if (!this->mCar->IsTurnLeft() && !this->mCar->IsTurnRight())
    {
        if (gRng.get_int(2) > 0)
        {
            mCar->SetTurnLeft();
        }
        else
        {
            mCar->SetTurnRight();
        }
    }
}

MATCH_FUNC(0x4538b0)
void CarAI::ClearSteeringIfTurning()
{
    if (this->mBf24.bTurning)
    {
        this->mCar->ClearTurnLeft();
        this->mCar->ClearTurnRight();
    }
}

MATCH_FUNC(0x453990)
void CarAI::RaiseSpeedTo(Fix16 target_speed)
{
    if (this->mSpeed < target_speed)
    {
        this->mSpeed = target_speed;
    }
}

MATCH_FUNC(0x4539b0)
void CarAI::ClearTurnReady()
{
    this->mCar->mBfA6.bTurnRightReady = false;
    this->mCar->mBfA6.bTurnLeftReady = false;
}

MATCH_FUNC(0x4539d0)
void CarAI::UpdateSpeedTowardTarget()
{
    Fix16 speed = this->mSpeed;
    Fix16 target_speed = this->mTargetSpeed;
    if (speed < target_speed)
    {
        this->mSpeed = mAcceleration + speed;
        if (mAcceleration < kF16Zero)
        {
            RaiseSpeedTo(this->mTargetSpeed);
        }
    }
    else
    {
        if (speed > target_speed)
        {
            this->mSpeed = speed - ((kFpFour_677BA0 * kFpPoint02));
            RaiseSpeedTo(mTargetSpeed);
        }
    }

    this->mCar->mPhysics->NeutralGear();
}

MATCH_FUNC(0x453a40)
void CarAI::UpdateTrainMovement()
{
    Fix16 zpos = kF16Zero;
    s32 rotation_mode = 0;
    Car* pCar = this->mCar;

    this->mAngle = this->mCar->mCarSprite->mRotation;

    Fix16 xpos = pCar->mCarSprite->mXy.x;
    Fix16 ypos = pCar->mCarSprite->mXy.y;
    zpos = pCar->mCarSprite->mZpos;

    s32 track_dir;
    if (gPublicTransport_181C->GetTrainSpeed(pCar, &this->mTargetSpeed))
    {
        track_dir = gMap_0x370->AdvanceAlongRailway(&xpos, &ypos, &zpos, this->mSpeed);
    }
    else
    {
        track_dir = gMap_0x370->AdvanceAlongRailwayChecked(&xpos, &ypos, &zpos, this->mSpeed);
    }

    s32 face = Ang16::GetAngleFace(mAngle);
    s32 cur_face = Ang16::GetAngleFace(mAngle);
    if (track_dir != cur_face)
    {
        switch (track_dir)
        {
            case road_direction::up_1:
                rotation_mode = (face != 3) + 1;
                break;
            case road_direction::down_2:
                rotation_mode = (face == 3) + 1;
                break;
            case road_direction::right_3:
                rotation_mode = (face != 2) + 1;
                break;
            case road_direction::left_4:
                rotation_mode = (face == 2) + 1;
                break;
            default:
                break;
        }
    }

    Fix16 track_angle;
    for (track_angle = Ang16::Ang16_to_Fix16(ReturnAngleFromRoadDirection(track_dir)); track_angle < kF16Zero; track_angle += kFpTwoPi)
    {
        ;
    }

    for (; track_angle >= kFpTwoPi; track_angle -= kFpTwoPi)
    {
        ;
    }

    UpdateSpeedTowardTarget();
    mCar->mPhysics->SetVelocityTowardTarget(xpos, ypos, track_angle, &rotation_mode);
}

MATCH_FUNC(0x453bb0)
void CarAI::AI_Service()
{
    InitAIvars();
    this->mAngle = this->mCar->mCarSprite->mRotation;
    this->mCurrDirection = Ang16::GetAngleFace(mAngle);
    if (mTicksSinceAlloc < 0xff)
    {
        mTicksSinceAlloc++;
    }
    UpdateDriving();
}

MATCH_FUNC(0x453bf0)
void CarAI::SetCar(Car* pCar)
{
    mCar = pCar;
}

// https://decomp.me/scratch/kidaG
// Keeps the angle temporaries in their own scope, so the speed temporary below can reuse
// their stack slot (as in the original)
static inline bool IsMovingForwards(CarPhysics* pPhysics)
{
    Ang16 heading_delta = pPhysics->mLinvel1.GetAngle() - pPhysics->mTheta;
    return heading_delta <= kAng90_6779E4 || heading_delta >= kAng270_677B08;
}

MATCH_FUNC(0x453c00)
void CarAI::ReverseOrNeutral()
{
    if (IsMovingForwards(mCar->mPhysics) || mCar->GetCarLinearSpeed() < sCurrCarAIDesiredSpeed)
    {
        mCar->mPhysics->ForceReverseInput();
    }
    else
    {
        mCar->mPhysics->ForceNeutralInput();
    }
}

MATCH_FUNC(0x453cb0)
CarAI::CarAI()
{
    this->mAngle = 0;
    this->mCar = NULL;
    this->mUnused4 = 0;
    this->mManeuverActive = 0;
    this->mProbeX = 0;
    this->mProbeY = 0;
    this->mpNext = NULL;
    this->mAngle = kAng0_677CE8;
    this->mSpeed = kF16Zero;
    this->mTargetSpeed = kDefaultTargetSpeed;
    this->mAcceleration = kDefaultAcceleration;
    this->mUnused20 = 0;
    this->mRoutePos = 0;
    this->mRouteIdx = -1;
    this->mPad29 = 0;
    this->mOffRoadTimer = 0;
    this->mFlags &= ~0x2DFEFu;
    this->mFlags |= 0x12010u;
    this->mArrowStart = 0;
    this->mArrowCur = 0;
    this->mArrowEnd = 0;
    this->mUnused5C = kF16Zero;
    this->mUnused60 = kF16Zero;
    this->mUnused64 = kF16Zero;
    this->mCarInCollision = NULL;
    this->mYieldToCar = NULL;
    this->mUnused34 = 0;
    this->mJunctionTurnDirection = 0;
    this->mSeekingRoad = 0;
    this->mPrevTargetDirection = car_ai_target_direction::go_straight_0;
    this->mTargetDirection = car_ai_target_direction::go_straight_0;
    this->mProbeDirection = car_ai_direction::none_0;
    this->mCurrDirection = car_ai_direction::none_0;
    this->mNearestEntity = NULL;
    this->mMaxSpeed = kF16Zero;
    this->mAccelCooldown = 0;
    this->mStoppedTimer = 0;
    this->mTicksSinceAlloc = 0;
    this->mYieldTimer = 0;
    this->mObstacleTimer = 0;
    this->mSeekRoadTurn = 0;
    this->mFlags &= ~0x3C0000u;
}

MATCH_FUNC(0x453d80)
void CarAI::PoolAllocate()
{
    this->mCar = NULL;
    this->mUnused4 = 0;
    this->mManeuverActive = 0;
    this->mProbeX = 0;
    this->mProbeY = 0;
    this->mAngle = kAng0_677CE8;
    this->mSpeed = kF16Zero;
    this->mTargetSpeed = kDefaultTargetSpeed;
    this->mAcceleration = kDefaultAcceleration;
    this->mUnused20 = 0;
    this->mRoutePos = 0;
    this->mRouteIdx = -1;
    this->mPad29 = 0;
    this->mOffRoadTimer = 0;
    this->mFlags &= ~0x2DFEFu;
    this->mFlags |= 0x12010u;
    this->mArrowStart = 0;
    this->mArrowCur = 0;
    this->mArrowEnd = 0;
    this->mUnused5C = kF16Zero;
    this->mUnused60 = kF16Zero;
    this->mUnused64 = kF16Zero;
    this->mCarInCollision = NULL;
    this->mYieldToCar = NULL;
    this->mUnused34 = 0;
    this->mJunctionTurnDirection = 0;
    this->mSeekingRoad = 0;
    this->mPrevTargetDirection = car_ai_target_direction::go_straight_0;
    this->mTargetDirection = car_ai_target_direction::go_straight_0;
    this->mProbeDirection = car_ai_direction::none_0;
    this->mCurrDirection = car_ai_direction::none_0;
    this->mNearestEntity = NULL;
    this->mMaxSpeed = kFpPoint7;
    this->mAccelCooldown = 0;
    this->mStoppedTimer = 0;
    this->mTicksSinceAlloc = 0;
    this->mYieldTimer = 0;
    this->mFlags &= ~0x3C0000u;
    this->mSeekRoadTurn = 0;
    this->mObstacleTimer = 0;
    this->mForcedStopTimer = 0;
}
