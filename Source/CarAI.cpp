// This TU's copy of the Fix16_Point length zero (see Fix16_Point.hpp)
#define FIX16_POINT_ZERO kF16Zero_677B90

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
DEFINE_GLOBAL_INIT(Fix16, kF16Zero_677B90, Fix16(0), 0x677B90);
DEFINE_GLOBAL(Fix16, gCurrCarAI_ypos_677C30, 0x677C30);
DEFINE_GLOBAL(Fix16, gCurrCarAI_xpos_677C38, 0x677C38);
DEFINE_GLOBAL(Fix16, gCurrCarAI_zpos_677C48, 0x677C48);
DEFINE_GLOBAL(Fix16, gCurrCarAI_TargetZ_6779F8, 0x6779F8);

DEFINE_GLOBAL_INIT(Fix16, kFpFour_677BA0, Fix16(4), 0x677BA0);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint02_677B5C, Fix16(0x147, 0), 0x677B5C);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint02_677AC4, kFpPoint02_677B5C, 0x677AC4);

DEFINE_GLOBAL_INIT(Fix16, gF16fOne_677B94, Fix16(1), 0x677B94); // Value = 1.0 in float
DEFINE_GLOBAL_INIT(Fix16, kFpPoint1_6779B8, Fix16(0x666, 0), 0x6779B8);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint01_677B50, Fix16(163, 0), 0x677B50);
DEFINE_GLOBAL_INIT(Fix16, kDefaultTargetSpeed_677CB4, kFpPoint1_6779B8, 0x677CB4);
DEFINE_GLOBAL_INIT(Fix16, kDefaultAcceleration_6779A4, kFpPoint01_677B50, 0x6779A4);

DEFINE_GLOBAL_INIT(Fix16, kFpPoint7_6779D4, Fix16(0x2CCC, 0), 0x6779D4);
DEFINE_GLOBAL(CarAIPool*, gCarAI_78_Pool_677CF8, 0x677CF8);

DEFINE_GLOBAL(Fix16, gCurrCarAI_Velocity_677B00, 0x677B00);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint05_677B60, Fix16(0x333, 0), 0x677B60);

DEFINE_GLOBAL(u8, gCurrCarAI_IsChasing_677BBC, 0x677BBC);
DEFINE_GLOBAL(u8, byte_677B3C, 0x677B3C);
DEFINE_GLOBAL(u8, byte_677A78, 0x677A78);
DEFINE_GLOBAL(u8, bOnGradientSlope_677C90, 0x677C90);
DEFINE_GLOBAL(u8, gCurrCarAI_AllowTrafficSteering_677A5D, 0x677A5D);
DEFINE_GLOBAL(u8, byte_677A6C, 0x677A6C);
DEFINE_GLOBAL(u8, byte_677A94, 0x677A94);
DEFINE_GLOBAL(u8, byte_677A5C, 0x677A5C);
DEFINE_GLOBAL(u8, byte_677B8C, 0x677B8C);
DEFINE_GLOBAL(u8, byte_677CA8, 0x677CA8);
DEFINE_GLOBAL(u8, gCurrCarAI_ArrowDirection_677C06, 0x677C06);

DEFINE_GLOBAL(Fix16, gCurrCarAI_ProbeX_677A74, 0x677A74);
DEFINE_GLOBAL(Fix16, gCurrCarAI_ProbeY_677A80, 0x677A80);
DEFINE_GLOBAL(Fix16, gCurrCarAI_SpriteHeight_677C9C, 0x677C9C);
DEFINE_GLOBAL(Fix16, gCurrCarAI_DesiredSpeed_677A8C, 0x677A8C);
DEFINE_GLOBAL(Fix16, gCurrCarAI_TargetX_6779F0, 0x6779F0);
DEFINE_GLOBAL(Fix16, gCurrCarAI_TargetY_6779F4, 0x6779F4);

DEFINE_GLOBAL(s32, dword_677C88, 0x677C88);
DEFINE_GLOBAL(Car*, gCurrCarAI_TargetCar_6779B0, 0x6779B0);

DEFINE_GLOBAL_INIT(Fix16, kFpPoint6_6779D0, Fix16(0x2666, 0), 0x6779D0);
DEFINE_GLOBAL_INIT(Ang16, kAng10_677A08, Ang16(40), 0x677A08);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_6779CC, Fix16(0x2000, 0), 0x6779CC);

DEFINE_GLOBAL_INIT(Fix16, kFpPoint4_6779C8, Fix16(0x1999, 0), 0x6779C8);

DEFINE_GLOBAL_INIT(Fix16, kFpPoint2_6779C0, Fix16(0xCCC, 0), 0x6779C0);
DEFINE_GLOBAL_INIT(Fix16, kFpNine_677BB8, Fix16(9), 0x677BB8);
DEFINE_GLOBAL_INIT(Fix16, kFpThree_677B9C, Fix16(3), 0x677B9C);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_677A84, Fix16(0x2000, 0), 0x677A84);
DEFINE_GLOBAL_INIT(Fix16, kFpThreeQuarters_677A4C, Fix16(0x3000, 0), 0x677A4C);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint03_677B58, Fix16(0x1EB, 0), 0x677B58);
DEFINE_GLOBAL_INIT(Fix16, kFpTwo_677B98, Fix16(2), 0x677B98);
DEFINE_GLOBAL_INIT(Fix16, kFpQuarter_6779BC, Fix16(0x1000, 0), 0x6779BC);
DEFINE_GLOBAL_INIT(Fix16, kFpEighth_677CBC, Fix16(0x800, 0), 0x677CBC);

// File-local: VC6 then propagates their value into the first use (ManageTrafficCarDirection_448CE0)
static Ang16 kAng180_677ADE = Ang16(720);
GLOBAL(kAng180_677ADE, 0x677ADE)
static Ang16 kAng90_6779E4 = Ang16(360);
GLOBAL(kAng90_6779E4, 0x6779E4)
EXTERN_GLOBAL(Ang16, kAng12_677A2E);
// Defined in Car_10.cpp: with the definition (and its dynamic initialiser) in this TU, VC6
// loads it with a 32-bit mov in FollowRoadDirection_44A1F0 instead of the original's 16-bit mov/sub.
// Same for kAng0 and kAng12_677A2E (16-bit sub/add in AlignToLaneCenter_44AF00).
EXTERN_GLOBAL(Ang16, kAng4_677CE2);

DEFINE_GLOBAL_INIT(Fix16, kFpTwoPi_677C84, Fix16(0x18F60, 0), 0x677C84);
static Ang16 kAng270_677B08 = Ang16(1080);
GLOBAL(kAng270_677B08, 0x677B08)
DEFINE_GLOBAL_INIT(Fix16, kFpFive_677BA4, Fix16(5), 0x677BA4);
DEFINE_GLOBAL_INIT(Fix16, kFpFive_677CA0, Fix16(5), 0x677CA0);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint09_677B78, Fix16(0x5C2, 0), 0x677B78);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint06_677B70, Fix16(0x3D7, 0), 0x677B70);

DEFINE_GLOBAL_INIT(Fix16, kFp255_677950, Fix16(255), 0x677950);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint8_6779D8, Fix16(0x3333, 0), 0x6779D8);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint04_677B64, Fix16(0x28F, 0), 0x677B64);
DEFINE_GLOBAL_INIT(Ang16, kAng45_677A38, Ang16(180), 0x677A38);
DEFINE_GLOBAL_INIT(Ang16, word_677A3A, Ang16(128), 0x677A3A);

EXTERN_GLOBAL(u16, word_677CFC);
EXTERN_GLOBAL(u8, byte_6771DC);

// TODO: Move
MATCH_FUNC(0x447650)
void __stdcall InitAIvars_447650()
{
    byte_677A78 = 0;
    bOnGradientSlope_677C90 = 0;
    gCurrCarAI_AllowTrafficSteering_677A5D = 1;
    byte_677B3C = 1;
    gCurrCarAI_IsChasing_677BBC = 0;
    byte_677A6C = 1;
    byte_677A94 = 0;
    byte_677A5C = 0;
    byte_677B8C = 1;
    byte_677CA8 = 0;
    gCurrCarAI_ArrowDirection_677C06 = 0;
    gCurrCarAI_xpos_677C38 = kF16Zero_677B90;
    gCurrCarAI_ypos_677C30 = kF16Zero_677B90;
    gCurrCarAI_zpos_677C48 = kF16Zero_677B90;
    gCurrCarAI_ProbeX_677A74 = kF16Zero_677B90;
    gCurrCarAI_ProbeY_677A80 = kF16Zero_677B90;
    gCurrCarAI_Velocity_677B00 = kF16Zero_677B90;
    gCurrCarAI_SpriteHeight_677C9C = kF16Zero_677B90;
    gCurrCarAI_DesiredSpeed_677A8C = kF16Zero_677B90;
    gCurrCarAI_TargetX_6779F0 = kF16Zero_677B90;
    gCurrCarAI_TargetY_6779F4 = kF16Zero_677B90;
    gCurrCarAI_TargetZ_6779F8 = kF16Zero_677B90;
    dword_677C88 = 1;
    gCurrCarAI_TargetCar_6779B0 = 0;
}

MATCH_FUNC(0x4476f0)
void CarAI::MakeAgressiveSirensAndLights_4476F0()
{
    if (field_0_car->IsPoliceCar_439EC0())
    {
        field_0_car->HonkHorn_4417F0();
    }
}

// Inlined in DoShortcutsUsingJunctions_447970 (no 9.6f function)
inline void CarAI::TurnAround_447970()
{
    field_0_car->field_58_physics->TurnAntiClockwise_42AB90();
    field_0_car->field_60_pChaseTask->field_22_following_route = 0;
    field_0_car->HaltAIDriving_43AF60();
    field_0_car->field_60_pChaseTask->field_26_route_finished = 1;
}

MATCH_FUNC(0x447710)
void CarAI::PlanTurnAtNextJunction_447710()
{
    s8 arrow_run_len = 0;
    if (field_28_route_idx >= 0)
    {
        Junction* Junction_58A0B0 =
            gRouteFinder_6FFDC8->GetJunction_58A0B0(gRouteFinder_6FFDC8->field_2218_routes[field_28_route_idx].field_0_junctions[static_cast<u16>(this->field_56_route_pos)]);
        u16 next_junction_idx = gRouteFinder_6FFDC8->field_2218_routes[this->field_28_route_idx].field_0_junctions[static_cast<u16>(this->field_56_route_pos) + 1];
        if (next_junction_idx)
        {
            if (!this->field_24_bf.bFlag0)
            {
                s16 turn_dir = Junction_58A0B0->GetDirectionToJunction_5885C0(next_junction_idx);
                switch (Ang16::GetAngleFace_4F78F0(this->field_10_angle))
                {
                    case 1:
                        this->field_0_car->ClearTurnRight_42ACA0();
                        this->field_0_car->ClearTurnLeft_42ACB0();
                        switch (turn_dir)
                        {
                            case 2:
                                this->field_38_junction_turn_direction = 2;
                                this->field_24_bf.bTurnPlanned = true;
                                break;
                            case 3:
                                //goto LABEL_14;
                                this->field_0_car->SetTurnRight_42AC70();
                                break;
                            case 4:
                                //goto LABEL_8;
                                this->field_0_car->SetTurnLeft_42AC80();
                                break;
                        }
                        break;

                    case 2:
                        this->field_0_car->ClearTurnRight_42ACA0();
                        this->field_0_car->ClearTurnLeft_42ACB0();
                        switch (turn_dir)
                        {
                            case 1:
                                this->field_38_junction_turn_direction = 1;
                                this->field_24_bf.bTurnPlanned = true;
                                break;
                            case 3:
                                //goto LABEL_8;
                                this->field_0_car->SetTurnLeft_42AC80();
                                break;
                            case 4:
                                //goto LABEL_14;
                                this->field_0_car->SetTurnRight_42AC70();
                                break;
                        }
                        break;

                    case 3:
                        this->field_0_car->ClearTurnRight_42ACA0();
                        this->field_0_car->ClearTurnLeft_42ACB0();
                        switch (turn_dir)
                        {
                            case 1:
                                //goto LABEL_14;
                                this->field_0_car->SetTurnRight_42AC70();
                                break;
                            case 2:
                                //goto LABEL_8;
                                this->field_0_car->SetTurnLeft_42AC80();
                                break;
                            case 3:
                                this->field_38_junction_turn_direction = 4;
                                this->field_24_bf.bTurnPlanned = true;
                                break;
                        }
                        break;

                    case 4:
                        this->field_0_car->ClearTurnRight_42ACA0();
                        this->field_0_car->ClearTurnLeft_42ACB0();
                        switch (turn_dir)
                        {
                            case 1:
                                //LABEL_8:
                                this->field_0_car->SetTurnLeft_42AC80();
                                break;
                            case 2:
                                //LABEL_14:
                                this->field_0_car->SetTurnRight_42AC70();
                                break;
                            case 4:
                                this->field_38_junction_turn_direction = 3;
                                this->field_24_bf.bTurnPlanned = true;
                                break;
                        }
                        break;

                    default:
                        break;
                }

                char_type arrow_blocks_found = this->field_0_car->CountConsecutiveArrowBlocks_4410D0(this->field_10_angle,
                                                                                 &arrow_run_len,
                                                                                 this->field_0_car->field_50_car_sprite->field_14_xy.x,
                                                                                 this->field_0_car->field_50_car_sprite->field_14_xy.y);
                if (arrow_run_len > -1)
                {
                    Car* pCar = this->field_0_car;
                    if (pCar->IsTurnRight_42AC20())
                    {
                        if (arrow_blocks_found)
                        {
                            if (!pCar->IsTurnRightReady_42AC40())
                            {
                                ClearTurnReady_4539B0();
                                this->field_0_car->SetTurnRightReady_42AC90();
                            }
                        }
                    }
                    else if (pCar->IsTurnLeft_42AC30() && arrow_blocks_found != arrow_run_len - 1 && !pCar->IsTurnLeftReady_42AC50())
                    {
                        ClearTurnReady_4539B0();
                        this->field_0_car->SetTurnLeftReady_421530();
                    }
                }
            }
        }
    }
}

MATCH_FUNC(0x447970)
void CarAI::DoShortcutsUsingJunctions_447970()
{
    this->field_24_flags &= ~0xC0000u;

    u8 x = field_0_car->field_60_pChaseTask->field_14_target_x.ToInt();
    u8 y = field_0_car->field_60_pChaseTask->field_18_target_y.ToInt();
    u8 z = field_0_car->field_60_pChaseTask->field_1C_target_z.ToInt();
    if (field_28_route_idx > 0)
    {
        u16 junc = gRouteFinder_6FFDC8->field_2218_routes[field_28_route_idx].field_0_junctions[static_cast<u16>(field_56_route_pos)];
        if (!junc || x == static_cast<s16>(gCurrCarAI_xpos_677C38.ToInt()) && y == static_cast<s16>(gCurrCarAI_ypos_677C30.ToInt()))
        {
            gRouteFinder_6FFDC8->CancelRoute_589930(field_28_route_idx);
            this->field_28_route_idx = -1;
            this->field_0_car->HaltAIDriving_43AF60();
            this->field_0_car->field_60_pChaseTask->field_26_route_finished = 1;
            return;
        }

        Junction* pJunction = gRouteFinder_6FFDC8->GetJunction_58A0B0(junc);
        if (x > 0)
        {
            if (gRouteFinder_6FFDC8->IsPointInJunctionBounds_588AA0(
                    x,
                    y,
                    gRouteFinder_6FFDC8->field_2218_routes[field_28_route_idx].field_0_junctions[static_cast<u16>(field_56_route_pos)],
                    gRouteFinder_6FFDC8->field_2218_routes[field_28_route_idx].field_0_junctions[static_cast<u16>(field_56_route_pos) + 1]) &&
                z == static_cast<s16>((field_0_car->field_50_car_sprite->field_1C_zpos - gF16fOne_677B94).ToInt()))
            {
                switch (static_cast<s16>(pJunction->GetDirectionToJunction_5885C0(
                    gRouteFinder_6FFDC8->field_2218_routes[field_28_route_idx].field_0_junctions[static_cast<u16>(field_56_route_pos) + 1])))
                {
                    case 1:
                        if (y >= static_cast<u8>(gCurrCarAI_ypos_677C30.ToInt()))
                        {
                            TurnAround_447970();
                            return;
                        }
                        if (x > static_cast<u8>(gCurrCarAI_xpos_677C38.ToInt()))
                        {
                            field_24_bf.bFlag18 = true;
                        }
                        else if (x < static_cast<u8>(gCurrCarAI_xpos_677C38.ToInt()))
                        {
                            field_24_bf.bFlag19 = true;
                        }
                        break;
                    case 2:
                        if (y <= static_cast<u8>(gCurrCarAI_ypos_677C30.ToInt()))
                        {
                            TurnAround_447970();
                            return;
                        }
                        if (x > static_cast<u8>(gCurrCarAI_xpos_677C38.ToInt()))
                        {
                            field_24_bf.bFlag18 = true;
                        }
                        else if (x < static_cast<u8>(gCurrCarAI_xpos_677C38.ToInt()))
                        {
                            field_24_bf.bFlag19 = true;
                        }
                        break;
                    case 3:
                        if (x >= static_cast<u8>(gCurrCarAI_xpos_677C38.ToInt()))
                        {
                            TurnAround_447970();
                            return;
                        }
                        if (y > static_cast<u8>(gCurrCarAI_ypos_677C30.ToInt()))
                        {
                            field_24_bf.bFlag19 = true;
                        }
                        else if (y < static_cast<u8>(gCurrCarAI_ypos_677C30.ToInt()))
                        {
                            field_24_bf.bFlag18 = true;
                        }
                        break;
                    case 4:
                        if (x <= static_cast<u8>(gCurrCarAI_xpos_677C38.ToInt()))
                        {
                            TurnAround_447970();
                            return;
                        }
                        if (y > static_cast<u8>(gCurrCarAI_ypos_677C30.ToInt()))
                        {
                            field_24_bf.bFlag18 = true;
                        }
                        else if (y < static_cast<u8>(gCurrCarAI_ypos_677C30.ToInt()))
                        {
                            field_24_bf.bFlag19 = true;
                        }
                        break;
                }
            }
        }

        // 9.6f: Junction::ContainsPoint (0x40CEE0, inlined)
        if (pJunction->ContainsPoint(gCurrCarAI_xpos_677C38.ToInt(), gCurrCarAI_ypos_677C30.ToInt()))
        {
            this->field_24_bf.bInsideJunction = true;
            PlanTurnAtNextJunction_447710();
        }
        else
        {
            if (field_24_bf.bInsideJunction)
            {
                this->field_24_bf.bInsideJunction = false;
            }
            PlanTurnAtNextJunction_447710();
        }
    }
}

MATCH_FUNC(0x447ca0)
bool CarAI::GoToBlock_447CA0(u8 x, u8 y, u8 z, s32 arrow_type)
{
    field_28_route_idx = static_cast<char_type>(gRouteFinder_6FFDC8->StartRoute_58A190(gCurrCarAI_xpos_677C38.ToInt(),
                                                               gCurrCarAI_ypos_677C30.ToInt(),
                                                               (gCurrCarAI_zpos_677C48 - gF16fOne_677B94).ToInt(),
                                                               x,
                                                               y,
                                                               z,
                                                               arrow_type));
    if (field_28_route_idx > 0)
    {
        field_0_car->field_60_pChaseTask->field_14_target_x = Fix16(x);
        field_0_car->field_60_pChaseTask->field_18_target_y = Fix16(y);
        field_0_car->field_60_pChaseTask->field_1C_target_z = gCurrCarAI_TargetZ_6779F8;
        field_56_route_pos = 0;
        return true;
    }
    return false;
}

MATCH_FUNC(0x447d40)
char_type CarAI::TrySetTargetDirectionFromArrows_447D40(gmp_block_info* pBlock)
{
    Ang16 angle = this->field_10_angle;

    switch (this->field_4C_curr_direction)
    {
        case car_ai_direction::north_1:
            if (field_0_car->IsTurnLeft_42AC30() || field_0_car->IsTurnRight_42AC20())
            {
                if (field_0_car->IsTurnLeft_42AC30())
                {
                    if ((pBlock->field_A_arrows & block_arrow::green_right_2) && ComputeShortestAngleDelta_4056C0(kAng90_6779E4, angle) > kAng45_677A38)
                    {
                        field_44_target_direction = car_ai_target_direction::eastwards_3;
                        return 1;
                    }
                }
                else
                {
                    if ((pBlock->field_A_arrows & block_arrow::green_left_1) && ComputeShortestAngleDelta_4056C0(kAng270_677B08, angle) > kAng45_677A38)
                    {
                        field_44_target_direction = car_ai_target_direction::westwards_4;
                        return 1;
                    }
                }
            }

            if ((pBlock->field_A_arrows & block_arrow::green_mask) == block_arrow::green_left_1)
            {
                if (ComputeShortestAngleDelta_4056C0(kAng270_677B08, angle) <= kAng45_677A38)
                {
                    return 0;
                }
                field_44_target_direction = car_ai_target_direction::westwards_4;
                return 1;
            }

            if ((pBlock->field_A_arrows & block_arrow::green_mask) != block_arrow::green_right_2)
            {
                return 0;
            }

            if (ComputeShortestAngleDelta_4056C0(kAng90_6779E4, angle) <= kAng45_677A38)
            {
                return 0;
            }
            field_44_target_direction = car_ai_target_direction::eastwards_3;
            return 1;

        case car_ai_direction::south_2:
            if (field_0_car->IsTurnLeft_42AC30() || field_0_car->IsTurnRight_42AC20())
            {
                if (field_0_car->IsTurnLeft_42AC30())
                {
                    if (byte_677CA8)
                    {
                        if ((pBlock->field_A_arrows & block_arrow::left_mask) && ComputeShortestAngleDelta_4056C0(kAng270_677B08, angle) > kAng45_677A38)
                        {
                            field_44_target_direction = car_ai_target_direction::westwards_4;
                            return 1;
                        }
                    }
                    else
                    {
                        if ((pBlock->field_A_arrows & block_arrow::green_left_1) && ComputeShortestAngleDelta_4056C0(kAng270_677B08, angle) > kAng45_677A38)
                        {
                            field_44_target_direction = car_ai_target_direction::westwards_4;
                            return 1;
                        }
                    }
                }
                else
                {
                    if (byte_677CA8)
                    {
                        // Note: the original compares against 270 here although it turns east
                        if ((pBlock->field_A_arrows & block_arrow::right_mask) && ComputeShortestAngleDelta_4056C0(kAng270_677B08, angle) > kAng45_677A38)
                        {
                            field_44_target_direction = car_ai_target_direction::eastwards_3;
                            return 1;
                        }
                    }
                    else
                    {
                        if ((pBlock->field_A_arrows & block_arrow::green_right_2) && ComputeShortestAngleDelta_4056C0(kAng90_6779E4, angle) > kAng45_677A38)
                        {
                            field_44_target_direction = car_ai_target_direction::eastwards_3;
                            return 1;
                        }
                    }
                }
            }

            if (byte_677CA8 && (pBlock->field_A_arrows & block_arrow::red_mask) && !(pBlock->field_A_arrows & block_arrow::green_mask))
            {
                if ((pBlock->field_A_arrows & block_arrow::red_mask) == block_arrow::red_left_10)
                {
                    if (ComputeShortestAngleDelta_4056C0(kAng270_677B08, angle) <= kAng45_677A38)
                    {
                        return 0;
                    }
                    field_44_target_direction = car_ai_target_direction::westwards_4;
                    return 1;
                }

                if ((pBlock->field_A_arrows & block_arrow::red_mask) != block_arrow::red_right_20)
                {
                    return 0;
                }

                if (ComputeShortestAngleDelta_4056C0(kAng90_6779E4, angle) <= kAng45_677A38)
                {
                    return 0;
                }
                field_44_target_direction = car_ai_target_direction::eastwards_3;
                return 1;
            }

            if (!(pBlock->field_A_arrows & block_arrow::green_mask) || (pBlock->field_A_arrows & block_arrow::red_mask))
            {
                return 0;
            }

            if ((pBlock->field_A_arrows & block_arrow::green_mask) == block_arrow::green_left_1)
            {
                if (ComputeShortestAngleDelta_4056C0(kAng270_677B08, angle) > kAng45_677A38)
                {
                    field_44_target_direction = car_ai_target_direction::westwards_4;
                    return 1;
                }
            }
            else if ((pBlock->field_A_arrows & block_arrow::green_mask) == block_arrow::green_right_2)
            {
                if (ComputeShortestAngleDelta_4056C0(kAng90_6779E4, angle) > kAng45_677A38)
                {
                    field_44_target_direction = car_ai_target_direction::eastwards_3;
                    return 1;
                }
            }
            return 0;

        case car_ai_direction::west_4:
            if (field_0_car->IsTurnLeft_42AC30() || field_0_car->IsTurnRight_42AC20())
            {
                if (field_0_car->IsTurnLeft_42AC30())
                {
                    if ((pBlock->field_A_arrows & block_arrow::green_up_4) && ComputeShortestAngleDelta_4056C0(kAng180_677ADE, angle) > kAng45_677A38)
                    {
                        field_44_target_direction = car_ai_target_direction::northwards_1;
                        return 1;
                    }
                }
                else
                {
                    if ((pBlock->field_A_arrows & block_arrow::green_down_8) && ComputeShortestAngleDelta_4056C0(kAng0_677CE8, angle) > kAng45_677A38)
                    {
                        field_44_target_direction = car_ai_target_direction::southwards_2;
                        return 1;
                    }
                }
            }

            if ((pBlock->field_A_arrows & block_arrow::green_mask) == block_arrow::green_down_8)
            {
                if (ComputeShortestAngleDelta_4056C0(kAng0_677CE8, angle) > kAng45_677A38)
                {
                    field_44_target_direction = car_ai_target_direction::southwards_2;
                    return 1;
                }
            }
            else if ((pBlock->field_A_arrows & block_arrow::green_mask) == block_arrow::green_up_4)
            {
                if (ComputeShortestAngleDelta_4056C0(kAng180_677ADE, angle) > kAng45_677A38)
                {
                    field_44_target_direction = car_ai_target_direction::northwards_1;
                    return 1;
                }
            }
            return 0;

        case car_ai_direction::east_3:
            if (field_0_car->IsTurnLeft_42AC30() || field_0_car->IsTurnRight_42AC20())
            {
                if (field_0_car->IsTurnLeft_42AC30())
                {
                    if ((pBlock->field_A_arrows & block_arrow::green_down_8) && ComputeShortestAngleDelta_4056C0(kAng0_677CE8, angle) > kAng45_677A38)
                    {
                        field_44_target_direction = car_ai_target_direction::southwards_2;
                        return 1;
                    }
                }
                else
                {
                    if ((pBlock->field_A_arrows & block_arrow::green_up_4) && ComputeShortestAngleDelta_4056C0(kAng180_677ADE, angle) > kAng45_677A38)
                    {
                        field_44_target_direction = car_ai_target_direction::northwards_1;
                        return 1;
                    }
                }
            }

            if ((pBlock->field_A_arrows & block_arrow::green_mask) == block_arrow::green_down_8)
            {
                if (ComputeShortestAngleDelta_4056C0(kAng0_677CE8, angle) > kAng45_677A38)
                {
                    field_44_target_direction = car_ai_target_direction::southwards_2;
                    return 1;
                }
            }
            else if ((pBlock->field_A_arrows & block_arrow::green_mask) == block_arrow::green_up_4)
            {
                if (ComputeShortestAngleDelta_4056C0(kAng180_677ADE, angle) > kAng45_677A38)
                {
                    field_44_target_direction = car_ai_target_direction::northwards_1;
                    return 1;
                }
            }
            return 0;
    }
    return 0;
}

MATCH_FUNC(0x448270)
bool CarAI::IsClockwiseTurning_448270()
{
    switch (field_4C_curr_direction)
    {
        case car_ai_direction::north_1:
            if (field_44_target_direction == car_ai_target_direction::eastwards_3)
            {
                return true;
            }
            break;
        case car_ai_direction::east_3:
            if (field_44_target_direction == car_ai_target_direction::southwards_2)
            {
                return true;
            }
            break;
        case car_ai_direction::south_2:
            if (field_44_target_direction == car_ai_target_direction::westwards_4)
            {
                return true;
            }
            break;
        case car_ai_direction::west_4:
            if (field_44_target_direction == car_ai_target_direction::northwards_1)
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
void CarAI::BrakeForBlockedRoadAhead_4482C0()
{
    Sprite* pProbe = gObject_5C_6F8F84->field_58_collision_probe_sprite;
    u8 probe_steps = 0;
    Fix16 default_probe_x = Fix16(static_cast<u8>(gCurrCarAI_ProbeX_677A74.ToInt()));
    Fix16 default_probe_y = Fix16(static_cast<u8>(gCurrCarAI_ProbeY_677A80.ToInt()));
    if (word_677CFC && !this->field_24_bf.bFlag17)
    {
        if (word_677CFC > 0)
        {
            --word_677CFC;
        }

        this->field_0_car->DoBrake_43A950();
        this->field_8_maneuver_active = 0;
        this->field_24_bf.bFlag7 = false;
    }
    else
    {
        word_677CFC = 50;
        u8 bSecondPass;
        if (!this->field_24_bf.bFlag17)
        {
            this->field_48_probe_direction = 4;
            this->field_9_probe_x = default_probe_x.ToInt();
            this->field_A_probe_y = default_probe_y.ToInt();
        }
        else
        {
            Fix16 saved_z = gCurrCarAI_zpos_677C48;
            Fix16 saved_y = gCurrCarAI_ypos_677C30;

            field_0_car->field_50_car_sprite->set_xyz_lazy_420600(gCurrCarAI_xpos_677C38, saved_y, saved_z);

            Fix16 dx;
            Fix16 dy;
            Ang16::PolarToCartesian_41FC20(this->field_10_angle, gF16fOne_677B94, dx, dy);
            field_0_car->field_50_car_sprite->set_xyz_lazy_420600(field_0_car->field_50_car_sprite->field_14_xy.x + dx,
                                                                  field_0_car->field_50_car_sprite->field_14_xy.y + dy,
                                                                  field_0_car->field_50_car_sprite->field_1C_zpos);

            Sprite* NearestSpriteOfType_477E60 =
                gSpriteGrid_1_679208->FindCollidingSpritePreferType_477E60(this->field_0_car->field_50_car_sprite, 0);
            this->field_70_nearest_entity = NearestSpriteOfType_477E60;

            if (NearestSpriteOfType_477E60)
            {
                if (NearestSpriteOfType_477E60->GetType_416B40() == sprite_types_enum::car_2)
                {
                    CarAI* pYieldAI = NearestSpriteOfType_477E60->field_8_car_bc_ptr->field_5C_AI;
                    if (pYieldAI)
                    {
                        if (pYieldAI->field_6C_yield_to_car)
                        {
                            pYieldAI->field_6C_yield_to_car = 0;
                        }
                    }
                    if (field_70_nearest_entity->field_8_car_bc_ptr->GetCarLinearSpeed_43A240() > kF16Zero_677B90)
                    {
                        field_0_car->DoBrake_43A950();
                    }
                }
            }
        }

        bSecondPass = 0;
        Fix16 probe_x;
        Fix16 probe_y;
        do
        {
            probe_x = Fix16(static_cast<u8>(this->field_9_probe_x));
            probe_y = Fix16(static_cast<u8>(this->field_A_probe_y));
            switch (this->field_4C_curr_direction)
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
                    this->field_48_probe_direction = 4;
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
                        probe_x -= gF16fOne_677B94;
                    }
                    this->field_48_probe_direction = 3;
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
                    this->field_48_probe_direction = 1;
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
                    this->field_48_probe_direction = 2;
                    break;

                default:
                    break;
            }

            pProbe->set_xyz_lazy_420600(probe_x, probe_y, field_0_car->field_50_car_sprite->field_1C_zpos);
            pProbe->set_ang_lazy_420690(kAng90_6779E4 + this->field_10_angle);
            pProbe->AllocCollisionShape_59F950(gF16fOne_677B94, gF16fOne_677B94, kFpPoint2_6779C0);
            gSpriteGrid_1_679208->FindCollidingSpritePreferType_477E60(pProbe, 0); // result not used

            for (u8 i = 0; i < probe_steps; i++)
            {
                switch (this->field_4C_curr_direction)
                {
                    case car_ai_direction::north_1:
                        probe_y -= gF16fOne_677B94;
                        break;
                    case car_ai_direction::south_2:
                        probe_y += gF16fOne_677B94;
                        break;
                    case car_ai_direction::east_3:
                        probe_x += gF16fOne_677B94;
                        break;
                    case car_ai_direction::west_4:
                        probe_x -= gF16fOne_677B94;
                        break;
                    default:
                        break;
                }

                pProbe->set_xyz_lazy_420600(probe_x, probe_y, field_0_car->field_50_car_sprite->field_1C_zpos);

                Sprite* pHit = gSpriteGrid_1_679208->FindCollidingSpritePreferType_477E60(pProbe, 0);
                if (pHit && pHit->GetType_416B40() == sprite_types_enum::car_2)
                {
                    CarAI* pAi = pHit->field_8_car_bc_ptr->field_5C_AI;
                    if (pAi && !pAi->field_24_bf.bFlag17 && pAi->field_48_probe_direction != this->field_48_probe_direction)
                    {
                        if (bSecondPass)
                        {
                            if (pAi->field_4C_curr_direction == this->field_48_probe_direction)
                            {
                                pAi->field_6C_yield_to_car = this->field_0_car;
                                pAi->field_2C_yield_timer = 5;
                                this->field_8_maneuver_active = 0;
                                this->field_24_bf.bFlag7 = false;
                            }
                        }
                        else if (pAi->field_4C_curr_direction != this->field_48_probe_direction)
                        {
                            pAi->field_6C_yield_to_car = this->field_0_car;
                            pAi->field_2C_yield_timer = 5;
                            this->field_8_maneuver_active = 0;
                            this->field_24_bf.bFlag7 = false;
                        }
                    }
                }
            }
        } while (++bSecondPass < 2);

        this->field_24_bf.bFlag17 = true;
    }
}

// 9.6f 0x430650
MATCH_FUNC(0x448770)
void CarAI::CheckRoadAhead_448770()
{
    Fix16 toUse = kF16Zero_677B90;
    gmp_block_info* pBlock_ = 0;
    Sprite* obj_5C_f58 = gObject_5C_6F8F84->field_58_collision_probe_sprite;
    Sprite* pCarSprite = this->field_0_car->field_50_car_sprite;
    gmp_block_info* pBlock_____ = 0;
    gmp_block_info* pBlock___ = 0;

    Fix16 sprite_x = pCarSprite->field_14_xy.x;
    Fix16 sprite_y = pCarSprite->field_14_xy.y;

    Fix16 new_x_ = sprite_x;
    Fix16 new_y_ = sprite_y;

    Fix16 x_off;
    Fix16 y_off;
    Ang16::PolarToCartesian_41FC20(this->field_10_angle, kFpHalf_677A84, x_off, y_off);

    gCurrCarAI_ProbeX_677A74 = this->field_0_car->field_50_car_sprite->field_14_xy.x + x_off;
    gCurrCarAI_ProbeY_677A80 = this->field_0_car->field_50_car_sprite->field_14_xy.y + y_off;

    if (gCurrCarAI_ProbeX_677A74 > kF16Zero_677B90 && gCurrCarAI_ProbeY_677A80 > kF16Zero_677B90 && gCurrCarAI_ProbeX_677A74 < kFp255_677950 && gCurrCarAI_ProbeY_677A80 < kFp255_677950)
    {
        pBlock_ = gMap_0x370_6F6268->GetBlock_4DFE10(gCurrCarAI_ProbeX_677A74.ToInt(), gCurrCarAI_ProbeY_677A80.ToInt(), field_0_car->field_50_car_sprite->field_1C_zpos.ToInt());
        if (pBlock_ && IS_GRADIENT_SLOPE(pBlock_->field_B_slope_type)
            && !IS_AIR_TYPE(pBlock_->field_B_slope_type))
        {
        }
        else
        {
            pBlock_ = gMap_0x370_6F6268->GetBlock_4DFE10(gCurrCarAI_ProbeX_677A74.ToInt(),
                                                          gCurrCarAI_ProbeY_677A80.ToInt(),
                                                          (this->field_0_car->field_50_car_sprite->field_1C_zpos.ToInt()) - 1);
        }
    }

    if (IsClockwiseTurning_448270())
    {
        toUse = kFpPoint4_6779C8;
    }
    else
    {
        toUse = kFpPoint8_6779D8;
    }

    if (gCurrCarAI_Velocity_677B00 < kFpPoint04_677B64)
    {
        toUse = kFpPoint2_6779C0;
    }

    if (pBlock_)
    {
        if (gGtx_0x106C_703DD4->IsRoadTile_42A810(pBlock_->field_8_lid & block_face::tile_idx_mask) || this->field_24_bf.bFlag17)
        {
            if (!gCurrCarAI_IsChasing_677BBC)
            {
                ChooseRandomTurn_4537D0();
            }

            if (this->field_0_car->IsTurnLeft_42AC30())
            {
                Fix16 new_x = static_cast<u8>(gCurrCarAI_ProbeX_677A74.ToInt());
                Fix16 new_y = static_cast<u8>(gCurrCarAI_ProbeY_677A80.ToInt());

                switch (this->field_4C_curr_direction)
                {
                    case car_ai_direction::north_1:
                        new_y -= kFpHalf_677A84;
                        new_x -= gF16fOne_677B94;
                        break;
                    case car_ai_direction::south_2:
                        new_y += gF16fOne_677B94;
                        new_x += kFpTwo_677B98;
                        break;
                    case car_ai_direction::east_3:
                        new_x += gF16fOne_677B94;
                        new_y += kFpTwo_677B98;
                        break;
                    case car_ai_direction::west_4:
                        new_x -= kFpHalf_677A84;
                        new_y -= kFpTwo_677B98;
                        break;
                    default:
                        break;
                }

                obj_5C_f58->set_xyz_lazy_420600(new_x, new_y, this->field_0_car->field_50_car_sprite->field_1C_zpos);
                obj_5C_f58->set_ang_lazy_420690(kAng90_6779E4 + this->field_10_angle);
                obj_5C_f58->AllocCollisionShape_59F950(gF16fOne_677B94, kFpHalf_677A84, kFpPoint2_6779C0);

                Sprite* pNearestSpriteOfType = gSpriteGrid_1_679208->FindCollidingSpritePreferType_477E60(obj_5C_f58, 0);
                if (pNearestSpriteOfType)
                {
                    if (pNearestSpriteOfType->GetType_416B40() == sprite_types_enum::car_2)
                    {
                        this->field_0_car->DoBrake_43A950();
                        this->field_8_maneuver_active = 0;
                        this->field_24_bf.bFlag7 = false;
                    }
                }
            }
            else if (this->field_0_car->IsTurnRight_42AC20())
            {
                BrakeForBlockedRoadAhead_4482C0();
            }
        }
    }

    this->field_0_car->field_50_car_sprite->set_xyz_lazy_420600(new_x_, new_y_, field_0_car->field_50_car_sprite->field_1C_zpos);

    field_0_car->field_50_car_sprite->set_ang_lazy_420690(this->field_10_angle);

    Fix16 x_off_;
    Fix16 y_off_;
    Ang16::PolarToCartesian_41FC20(this->field_10_angle, toUse, x_off_, y_off_);

    gCurrCarAI_ProbeX_677A74 = this->field_0_car->field_50_car_sprite->field_14_xy.x + x_off_;
    gCurrCarAI_ProbeY_677A80 = this->field_0_car->field_50_car_sprite->field_14_xy.y + y_off_;

    if (gCurrCarAI_ProbeX_677A74 > kF16Zero_677B90 && gCurrCarAI_ProbeY_677A80 > kF16Zero_677B90 && gCurrCarAI_ProbeX_677A74 < kFp255_677950 && gCurrCarAI_ProbeY_677A80 < kFp255_677950)
    {
        pBlock___ = gMap_0x370_6F6268->GetBlock_4DFE10(gCurrCarAI_ProbeX_677A74.ToInt(), gCurrCarAI_ProbeY_677A80.ToInt(), field_0_car->field_50_car_sprite->field_1C_zpos.ToInt());
        if (pBlock___ && IS_GRADIENT_SLOPE(pBlock___->field_B_slope_type)
            && !IS_AIR_TYPE(pBlock___->field_B_slope_type))
        {
        }
        else
        {
            pBlock___ = gMap_0x370_6F6268->GetBlock_4DFE10(gCurrCarAI_ProbeX_677A74.ToInt(),
                                                            gCurrCarAI_ProbeY_677A80.ToInt(),
                                                            (this->field_0_car->field_50_car_sprite->field_1C_zpos.ToInt()) - 1);
        }
    }

    Fix16 x_off__;
    Fix16 y_off__;
    Ang16::PolarToCartesian_41FC20(this->field_10_angle, kFpThree_677B9C, x_off__, y_off__);

    gCurrCarAI_ProbeX_677A74 = this->field_0_car->field_50_car_sprite->field_14_xy.x + x_off__;
    gCurrCarAI_ProbeY_677A80 = this->field_0_car->field_50_car_sprite->field_14_xy.y + y_off__;

    if (gCurrCarAI_ProbeX_677A74 > kF16Zero_677B90 && gCurrCarAI_ProbeY_677A80 > kF16Zero_677B90 && gCurrCarAI_ProbeX_677A74 < kFp255_677950 && gCurrCarAI_ProbeY_677A80 < kFp255_677950)
    {
        pBlock_____ =
            gMap_0x370_6F6268->GetBlock_4DFE10(gCurrCarAI_ProbeX_677A74.ToInt(), gCurrCarAI_ProbeY_677A80.ToInt(), this->field_0_car->field_50_car_sprite->field_1C_zpos.ToInt());
        if (pBlock_____ && IS_GRADIENT_SLOPE(pBlock_____->field_B_slope_type) &&
            !IS_AIR_TYPE(pBlock_____->field_B_slope_type))
        {
        }
        else
        {
            pBlock_____ = gMap_0x370_6F6268->GetBlock_4DFE10(gCurrCarAI_ProbeX_677A74.ToInt(),
                                                              gCurrCarAI_ProbeY_677A80.ToInt(),
                                                              (this->field_0_car->field_50_car_sprite->field_1C_zpos.ToInt()) - 1);
        }
    }

    if (!bOnGradientSlope_677C90 && !this->field_8_maneuver_active)
    {
        if (!pBlock___)
        {
            this->field_44_target_direction = car_ai_target_direction::go_straight_0;
        }
        else if (TrySetTargetDirectionFromArrows_447D40(pBlock___))
        {
            this->field_8_maneuver_active = 20;
            this->field_24_flags &= ~0xC000u;
            this->field_24_bf.bFlag7 = true;
        }
        else if (!pBlock_____)
        {
            this->field_44_target_direction = car_ai_target_direction::go_straight_0;
        }
        else if (TrySetTargetDirectionFromArrows_447D40(pBlock_____))
        {
            this->field_24_bf.bTargetSetFromArrows = true;
        }
        else
        {
            this->field_44_target_direction = car_ai_target_direction::go_straight_0;
        }
    }
}

// https://decomp.me/scratch/NIiIz
// 9.6f 0x430CC0. ClearSteeringIfTurning_4538B0 (9.6f 0x42FE40) is called once and inlined three times in 10.5
MATCH_FUNC(0x448ce0)
void CarAI::ManageTrafficCarDirection_448CE0()
{
    Ang16 target_angle = kAng0_677CE8;
    Ang16 cur_angle = kAng0_677CE8;
    Fix16 off;
    u8 frac;

    field_0_car->field_50_car_sprite->set_xyz_lazy_420600(gCurrCarAI_xpos_677C38, gCurrCarAI_ypos_677C30, gCurrCarAI_zpos_677C48);

    this->field_24_bf.bFlag10 = true;
    gmp_block_info* pBlock = gMap_0x370_6F6268->GetBlock_4DFE10(field_0_car->field_50_car_sprite->field_14_xy.x.ToInt(),
                                                                 field_0_car->field_50_car_sprite->field_14_xy.y.ToInt(),
                                                                 field_0_car->field_50_car_sprite->field_1C_zpos.ToInt());
    gmp_block_info* pArrowBlock = pBlock;
    if (pBlock != NULL && IS_GRADIENT_SLOPE(pBlock->field_B_slope_type) && !IS_AIR_TYPE(pBlock->field_B_slope_type))
    {
    }
    else
    {
        pArrowBlock = gMap_0x370_6F6268->GetBlock_4DFE10(field_0_car->field_50_car_sprite->field_14_xy.x.ToInt(),
                                                 field_0_car->field_50_car_sprite->field_14_xy.y.ToInt(),
                                                 field_0_car->field_50_car_sprite->field_1C_zpos.ToInt() - 1);
    }

    CheckRoadAhead_448770();

    if (pArrowBlock != NULL)
    {
        switch (field_4C_curr_direction)
        {
            case car_ai_direction::north_1:
            {
                if ((pArrowBlock->field_A_arrows & block_arrow::green_up_4) != 0)
                {
                    field_24_bf.bFlag4 = true;
                }
                else
                {
                    if ((pArrowBlock->field_A_arrows & block_arrow::red_up_40) == 0)
                    {
                        field_24_bf.bFlag4 = false;
                    }
                    else
                    {
                        field_24_bf.bFlag4 = true;
                    }
                }
                break;
            }

            case car_ai_direction::south_2:
            {
                if ((pArrowBlock->field_A_arrows & block_arrow::green_down_8) != 0)
                {
                    field_24_bf.bFlag4 = true;
                }
                else
                {
                    if ((pArrowBlock->field_A_arrows & block_arrow::red_down_80) == 0)
                    {
                        field_24_bf.bFlag4 = false;
                    }
                    else
                    {
                        field_24_bf.bFlag4 = true;
                    }
                }
                break;
            }

            case car_ai_direction::east_3:
            {
                if ((pArrowBlock->field_A_arrows & block_arrow::green_right_2) != 0)
                {
                    field_24_bf.bFlag4 = true;
                }
                else
                {
                    if ((pArrowBlock->field_A_arrows & block_arrow::red_right_20) == 0)
                    {
                        field_24_bf.bFlag4 = false;
                    }
                    else
                    {
                        field_24_bf.bFlag4 = true;
                    }
                }
                break;
            }

            case car_ai_direction::west_4:
            {
                if ((pArrowBlock->field_A_arrows & block_arrow::green_left_1) != 0)
                {
                    field_24_bf.bFlag4 = true;
                }
                else
                {
                    if ((pArrowBlock->field_A_arrows & block_arrow::red_up_40) == 0)
                    {
                        field_24_bf.bFlag4 = false;
                    }
                    else
                    {
                        field_24_bf.bFlag4 = true;
                    }
                }
                break;
            }

            default:
                field_24_bf.bFlag4 = false;
                break;
        }
    }
    Fix16 x_off;
    Fix16 y_off;

    Ang16& heading = field_10_angle;
    Ang16::PolarToCartesian_41FC20(heading, gCurrCarAI_SpriteHeight_677C9C / kFpTwo_677B98, x_off, y_off);

    gCurrCarAI_ProbeX_677A74 = field_0_car->field_50_car_sprite->field_14_xy.x + x_off;
    gCurrCarAI_ProbeY_677A80 = field_0_car->field_50_car_sprite->field_14_xy.y + y_off;

    if (field_24_bf.bFlag4 != 0)
    {
        field_24_bf.bFlag3 = false;
        field_24_bf.bFlag4 = true;
        field_24_bf.bFlag16 = true;

        switch (field_4C_curr_direction)
        {
            case car_ai_direction::north_1:
            {
                if (field_24_bf.bFlag7 != 0)
                {
                    field_40_prev_target_direction = field_44_target_direction;
                    if (field_44_target_direction == car_ai_target_direction::northwards_1)
                    {
                        ClearSteeringIfTurning_4538B0();
                        field_24_bf.bFlag7 = false;
                        field_24_bf.bFlag17 = false;
                    }
                    else if (field_44_target_direction == car_ai_target_direction::eastwards_3)
                    {
                        field_0_car->field_58_physics->TurnClockwise_42ABA0();
                    }
                    else if (field_44_target_direction == car_ai_target_direction::westwards_4)
                    {
                        field_0_car->field_58_physics->TurnAntiClockwise_42AB90();
                    }
                    break;
                }

                cur_angle = field_10_angle;
                field_40_prev_target_direction = 1;
                target_angle = kAng180_677ADE;
                if (cur_angle > target_angle - kAng10_677A08 && cur_angle < target_angle + kAng10_677A08)
                {
                    if (gCurrCarAI_IsChasing_677BBC != 0)
                    {
                        field_8_maneuver_active = 0;
                    }

                    frac = field_0_car->field_50_car_sprite->field_14_xy.x.ToUInt8();
                    off = field_0_car->field_50_car_sprite->field_14_xy.x - Fix16(frac);
                    if (!(off <= kFpPoint4_6779C8) && !(off >= kFpPoint6_6779D0))
                    {
                        if (field_10_angle == kAng180_677ADE)
                        {
                            field_8_maneuver_active = 0;
                            field_0_car->field_58_physics->field_8_total_damage_q = 0;
                            field_24_bf.bFlag10 = false;
                            field_24_bf.bFlag4 = true;
                        }
                        else
                        {
                            field_0_car->field_58_physics->SetGoStraight_42ABB0();
                        }
                        break;
                    }

                    field_24_bf.bFlag13 = false;
                    field_24_bf.bFlag16 = false;
                    if (off <= kFpPoint4_6779C8)
                    {
                        if (cur_angle > target_angle - kAng10_677A08)
                        {
                            if (cur_angle < target_angle - kAng4_677CE2)
                            {
                                field_0_car->field_58_physics->SetGoStraight_42ABB0();
                            }
                            else
                            {
                                field_0_car->field_58_physics->TurnClockwise_42ABA0();
                            }
                        }
                        else
                        {
                            field_0_car->field_58_physics->TurnAntiClockwise_42AB90();
                        }
                    }
                    else
                    {
                        if (cur_angle < target_angle + kAng10_677A08)
                        {
                            if (cur_angle > target_angle + kAng4_677CE2)
                            {
                                field_0_car->field_58_physics->SetGoStraight_42ABB0();
                                break;
                            }
                            else
                            {
                                field_0_car->field_58_physics->TurnAntiClockwise_42AB90();
                            }
                        }
                        else
                        {
                            field_0_car->field_58_physics->TurnClockwise_42ABA0();
                        }
                    }
                    break;
                }

                field_24_bf.bFlag13 = false;
                field_24_bf.bFlag16 = false;
                if (cur_angle > target_angle)
                {
                    field_0_car->field_58_physics->TurnClockwise_42ABA0();
                }
                else
                {
                    field_0_car->field_58_physics->TurnAntiClockwise_42AB90();
                    return;
                }
                break;
            }

            case car_ai_direction::south_2:
            {
                if (field_24_bf.bFlag7 != 0)
                {
                    field_40_prev_target_direction = field_44_target_direction;
                    if (field_44_target_direction != car_ai_target_direction::southwards_2)
                    {
                        if (field_44_target_direction == car_ai_target_direction::westwards_4)
                        {
                            field_0_car->field_58_physics->TurnClockwise_42ABA0();
                        }
                        else if (field_44_target_direction == car_ai_target_direction::eastwards_3)
                        {
                            field_0_car->field_58_physics->TurnAntiClockwise_42AB90();
                        }
                    }
                    else
                    {
                        ClearSteeringIfTurning_4538B0();
                        field_24_bf.bFlag7 = false;
                        field_24_bf.bFlag17 = false;
                    }
                    return;
                }

                field_40_prev_target_direction = 2;
                cur_angle = field_10_angle + kAng90_6779E4;
                target_angle = kAng90_6779E4;
                if (cur_angle > (target_angle - kAng4_677CE2) - kAng4_677CE2 && cur_angle < (target_angle + kAng4_677CE2) + kAng4_677CE2)
                {
                    if (gCurrCarAI_IsChasing_677BBC != 0)
                    {
                        field_8_maneuver_active = 0;
                    }

                    frac = field_0_car->field_50_car_sprite->field_14_xy.x.ToUInt8();
                    off = field_0_car->field_50_car_sprite->field_14_xy.x - Fix16(frac);
                    if (!(off <= kFpPoint4_6779C8) && !(off >= kFpPoint6_6779D0))
                    {
                        if (field_10_angle == kAng0_677CE8 && field_70_nearest_entity == NULL)
                        {
                            field_0_car->field_58_physics->field_8_total_damage_q = 0;
                            field_24_bf.bFlag10 = false;
                            field_24_bf.bFlag4 = true;
                            field_8_maneuver_active = 0;
                        }
                        else
                        {
                            field_0_car->field_58_physics->SetGoStraight_42ABB0();
                        }
                        return;
                    }

                    field_24_bf.bFlag13 = false;
                    field_24_bf.bFlag16 = false;
                    if (off <= kFpPoint4_6779C8)
                    {
                        if (cur_angle < target_angle + kAng10_677A08)
                        {
                            if (cur_angle > target_angle + kAng4_677CE2)
                            {
                                field_0_car->field_58_physics->SetGoStraight_42ABB0();
                            }
                            else
                            {
                                field_0_car->field_58_physics->TurnAntiClockwise_42AB90();
                                return;
                            }
                        }
                        else
                        {
                            field_0_car->field_58_physics->TurnClockwise_42ABA0();
                        }
                    }
                    else
                    {
                        if (cur_angle > target_angle - kAng10_677A08)
                        {
                            if (cur_angle < target_angle - kAng4_677CE2)
                            {
                                field_0_car->field_58_physics->SetGoStraight_42ABB0();
                            }
                            else
                            {
                                field_0_car->field_58_physics->TurnClockwise_42ABA0();
                            }
                        }
                        else
                        {
                            field_0_car->field_58_physics->TurnAntiClockwise_42AB90();
                        }
                    }
                    break;
                }

                field_24_bf.bFlag13 = false;
                field_24_bf.bFlag16 = false;
                if (cur_angle > target_angle)
                {
                    field_0_car->field_58_physics->TurnClockwise_42ABA0();
                }
                else
                {
                    field_0_car->field_58_physics->TurnAntiClockwise_42AB90();
                }
                break;
            }

            case car_ai_direction::east_3:
            {
                cur_angle = field_10_angle;
                field_40_prev_target_direction = 3;
                target_angle = kAng90_6779E4;
                if (field_24_bf.bFlag7 != 0)
                {
                    field_40_prev_target_direction = field_44_target_direction;
                    if (field_44_target_direction != car_ai_target_direction::eastwards_3)
                    {
                        if (field_44_target_direction == car_ai_target_direction::northwards_1)
                        {
                            field_0_car->field_58_physics->TurnAntiClockwise_42AB90();
                        }
                        else if (field_44_target_direction == car_ai_target_direction::southwards_2)
                        {
                            field_0_car->field_58_physics->TurnClockwise_42ABA0();
                        }
                    }
                    else
                    {
                        ClearSteeringIfTurning_4538B0();
                        field_24_bf.bFlag7 = false;
                        field_24_bf.bFlag17 = false;
                    }
                    return;
                }

                if (cur_angle > (target_angle - kAng4_677CE2) - kAng4_677CE2 && cur_angle < (target_angle + kAng4_677CE2) + kAng4_677CE2)
                {
                    if (gCurrCarAI_IsChasing_677BBC != 0)
                    {
                        field_8_maneuver_active = 0;
                    }

                    frac = field_0_car->field_50_car_sprite->field_14_xy.y.ToUInt8();
                    off = field_0_car->field_50_car_sprite->field_14_xy.y - Fix16(frac);
                    if (!(off <= kFpPoint4_6779C8) && !(off >= kFpPoint6_6779D0))
                    {
                        if (field_10_angle == kAng90_6779E4)
                        {
                            field_0_car->field_58_physics->field_8_total_damage_q = 0;
                            field_24_bf.bFlag10 = false;
                            field_24_bf.bFlag4 = true;
                            field_8_maneuver_active = 0;
                        }
                        else
                        {
                            field_0_car->field_58_physics->SetGoStraight_42ABB0();
                            break;
                        }
                        return;
                    }

                    field_24_bf.bFlag13 = false;
                    field_24_bf.bFlag16 = false;
                    if (off <= kFpPoint4_6779C8)
                    {
                        if (cur_angle < target_angle - kAng4_677CE2)
                        {
                            if (cur_angle > target_angle - kAng10_677A08)
                            {
                                field_0_car->field_58_physics->SetGoStraight_42ABB0();
                            }
                            else
                            {
                                field_0_car->field_58_physics->TurnAntiClockwise_42AB90();
                            }
                        }
                        else
                        {
                            field_0_car->field_58_physics->TurnClockwise_42ABA0();
                        }
                    }
                    else
                    {
                        if (cur_angle > target_angle + kAng4_677CE2)
                        {
                            if (cur_angle < target_angle + kAng10_677A08)
                            {
                                field_0_car->field_58_physics->SetGoStraight_42ABB0();
                            }
                            else
                            {
                                field_0_car->field_58_physics->TurnClockwise_42ABA0();
                            }
                        }
                        else
                        {
                            field_0_car->field_58_physics->TurnAntiClockwise_42AB90();
                        }
                    }
                    return;
                }

                field_24_bf.bFlag13 = false;
                field_24_bf.bFlag16 = false;
                if (cur_angle > target_angle)
                {
                    field_0_car->field_58_physics->TurnClockwise_42ABA0();
                    return;
                }
                else
                {
                    field_0_car->field_58_physics->TurnAntiClockwise_42AB90();
                }
                break;
            }

            case car_ai_direction::west_4:
            {
                if (field_24_bf.bFlag7 != 0)
                {
                    field_40_prev_target_direction = field_44_target_direction;
                    if (field_44_target_direction != car_ai_target_direction::westwards_4)
                    {
                        if (field_44_target_direction == car_ai_target_direction::northwards_1)
                        {
                            field_0_car->field_58_physics->TurnClockwise_42ABA0();
                        }
                        else if (field_44_target_direction == car_ai_target_direction::southwards_2)
                        {
                            field_0_car->field_58_physics->TurnAntiClockwise_42AB90();
                            return;
                        }
                    }
                    else
                    {
                        ClearSteeringIfTurning_4538B0();
                        field_24_bf.bFlag7 = false;
                        field_24_bf.bFlag17 = false;
                    }
                    return;
                }

                cur_angle = field_10_angle;
                field_40_prev_target_direction = 4;
                target_angle = kAng270_677B08;
                if (cur_angle > (target_angle - kAng4_677CE2) - kAng4_677CE2 && cur_angle < (target_angle + kAng4_677CE2) + kAng4_677CE2)
                {
                    if (gCurrCarAI_IsChasing_677BBC != 0)
                    {
                        field_8_maneuver_active = 0;
                    }

                    frac = field_0_car->field_50_car_sprite->field_14_xy.y.ToUInt8();
                    off = field_0_car->field_50_car_sprite->field_14_xy.y - Fix16(frac);
                    if (!(off <= kFpPoint4_6779C8) && !(off >= kFpPoint6_6779D0))
                    {
                        if (field_10_angle == kAng270_677B08)
                        {
                            field_0_car->field_58_physics->field_8_total_damage_q = 0;
                            field_24_bf.bFlag10 = false;
                            field_24_bf.bFlag4 = true;
                            field_8_maneuver_active = 0;
                        }
                        else
                        {
                            field_0_car->field_58_physics->SetGoStraight_42ABB0();
                        }
                        return;
                    }

                    field_24_bf.bFlag13 = false;
                    field_24_bf.bFlag16 = false;
                    if (off <= kFpPoint4_6779C8)
                    {
                        if (cur_angle > target_angle + kAng4_677CE2)
                        {
                            if (cur_angle < target_angle + kAng10_677A08)
                            {
                                field_0_car->field_58_physics->SetGoStraight_42ABB0();
                            }
                            else
                            {
                                field_0_car->field_58_physics->TurnClockwise_42ABA0();
                            }
                        }
                        else
                        {
                            field_0_car->field_58_physics->TurnAntiClockwise_42AB90();
                        }
                    }
                    else
                    {
                        if (cur_angle < target_angle - kAng4_677CE2)
                        {
                            if (cur_angle > target_angle - kAng10_677A08)
                            {
                                field_0_car->field_58_physics->SetGoStraight_42ABB0();
                                return;
                            }
                            else
                            {
                                field_0_car->field_58_physics->TurnAntiClockwise_42AB90();
                            }
                        }
                        else
                        {
                            field_0_car->field_58_physics->TurnClockwise_42ABA0();
                        }
                    }
                    break;
                }

                field_24_bf.bFlag13 = false;
                field_24_bf.bFlag16 = false;
                if (cur_angle > target_angle)
                {
                    field_0_car->field_58_physics->TurnClockwise_42ABA0();
                }
                else
                {
                    field_0_car->field_58_physics->TurnAntiClockwise_42AB90();
                }
                break;
            }

            default:
                break;
        }
    }
    else
    {
        FollowRoadDirection_44A1F0();
    }
}

// https://decomp.me/scratch/FqWRo
MATCH_FUNC(0x44a1f0)
void CarAI::FollowRoadDirection_44A1F0()
{
    Ang16 lane_angle = kAng0_677CE8;
    Ang16 travel_angle = kAng0_677CE8;
    Fix16 vec_x;
    Fix16 vec_y;

    vec_y = kF16Zero_677B90;
    field_24_bf.bFlag13 = false;

    Ang16 lane_angle_copy = lane_angle;

    gmp_block_info* pBlockAtPos = gMap_0x370_6F6268->GetBlock_4DFE10(gCurrCarAI_ProbeX_677A74.ToInt(), gCurrCarAI_ProbeY_677A80.ToInt(), field_0_car->field_50_car_sprite->field_1C_zpos.ToInt());
    if (!pBlockAtPos || !IS_GRADIENT_SLOPE(pBlockAtPos->field_B_slope_type) ||
        IS_AIR_TYPE(pBlockAtPos->field_B_slope_type))
    {
        pBlockAtPos = gMap_0x370_6F6268->GetBlock_4DFE10(gCurrCarAI_ProbeX_677A74.ToInt(),
                                                           gCurrCarAI_ProbeY_677A80.ToInt(),
                                                           (field_0_car->field_50_car_sprite->field_1C_zpos.ToInt()) - 1);
    }

    s32 road_direction = pBlockAtPos ? gMap_0x370_6F6268->GetArrowDirectionFromBlock_4E5FC0(pBlockAtPos, 1) : 0;
    if (road_direction && field_3C_seeking_road)
    {
        field_3C_seeking_road = 0;
        field_24_flags &= ~0xC0;
        if (gCurrCarAI_IsChasing_677BBC)
        {
            field_0_car->field_60_pChaseTask->field_22_following_route = 1;
        }
    }

    field_24_bf.bFlag10 = false;

    switch (road_direction)
    {
        case road_direction::up_1:
            switch (field_4C_curr_direction)
            {
                case car_ai_direction::north_1:
                {
                    Fix16 lane_dx =
                        field_0_car->field_50_car_sprite->field_14_xy.x - Fix16(field_0_car->field_50_car_sprite->field_14_xy.x.ToUInt8());
                    if (lane_dx <= kFpPoint4_6779C8 || lane_dx >= kFpPoint6_6779D0)
                    {
                        field_24_flags &= 0xFFFEDFFF;

                        if (lane_dx <= kFpPoint4_6779C8)
                        {
                            if (travel_angle > (lane_angle_copy - kAng10_677A08))
                            {
                                if (travel_angle < (lane_angle - kAng4_677CE2))
                                {
                                    field_0_car->field_58_physics->SetGoStraight_42ABB0();
                                }
                                else
                                {
                                    field_0_car->field_58_physics->TurnClockwise_42ABA0();
                                }
                            }
                            else
                            {
                                field_0_car->field_58_physics->TurnAntiClockwise_42AB90();
                            }
                        }
                        else if (travel_angle < (kAng10_677A08 + lane_angle_copy))
                        {
                            if (travel_angle > (lane_angle + kAng4_677CE2))
                            {
                                field_0_car->field_58_physics->SetGoStraight_42ABB0();
                            }
                            else
                            {
                                field_0_car->field_58_physics->TurnAntiClockwise_42AB90();
                            }
                        }
                        else
                        {
                            field_0_car->field_58_physics->TurnClockwise_42ABA0();
                        }
                    }

                    return;
                }

                case car_ai_direction::south_2:
                    field_0_car->field_58_physics->TurnClockwise_42ABA0();
                    return;

                case car_ai_direction::east_3:
                    if (field_24_bf.bFlag7 && field_44_target_direction == car_ai_target_direction::northwards_1)
                    {
                        field_0_car->field_58_physics->TurnAntiClockwise_42AB90();
                    }
                    else if (field_10_angle > kAng45_677A38)
                    {
                        if (field_24_bf.bFlag6)
                        {
                            field_0_car->field_58_physics->TurnClockwise_42ABA0();
                        }
                        else
                        {
                            field_0_car->field_58_physics->TurnAntiClockwise_42AB90();
                        }
                    }
                    else if (field_24_bf.bFlag6)
                    {
                        field_0_car->field_58_physics->TurnAntiClockwise_42AB90();
                    }
                    else
                    {
                        field_0_car->field_58_physics->TurnClockwise_42ABA0();
                    }

                    return;

                case car_ai_direction::west_4:
                    field_0_car->field_58_physics->TurnClockwise_42ABA0();
                    return;

                default:
                    return;
            }
            return;

        case road_direction::down_2:

            if (field_4C_curr_direction == car_ai_direction::west_4)
            {
                if (field_10_angle > kAng270_677B08)
                {
                    if (field_24_bf.bFlag6)
                    {
                        field_0_car->field_58_physics->TurnClockwise_42ABA0();
                    }
                    else
                    {
                        field_0_car->field_58_physics->TurnAntiClockwise_42AB90();
                    }
                }
                else
                {
                    if (field_24_bf.bFlag6)
                    {
                        field_0_car->field_58_physics->TurnAntiClockwise_42AB90();
                    }
                    else
                    {
                        field_0_car->field_58_physics->TurnClockwise_42ABA0();
                    }
                }
                //return;
            }
            else if (field_4C_curr_direction == car_ai_direction::east_3)
            {
                field_0_car->field_58_physics->TurnClockwise_42ABA0();
                //return;
            }
            else if (field_4C_curr_direction == car_ai_direction::south_2)
            {
                field_0_car->field_58_physics->SetGoStraight_42ABB0();
                //return;
            }
            else if (field_4C_curr_direction == car_ai_direction::north_1)
            {
                if (field_24_bf.bFlag7 && field_44_target_direction == car_ai_target_direction::westwards_4)
                {
                    field_0_car->field_58_physics->TurnAntiClockwise_42AB90();
                }
                else
                {
                    field_0_car->field_58_physics->TurnClockwise_42ABA0();
                }
            }
            return;

        case road_direction::right_3:
            switch (field_4C_curr_direction)
            {
                case car_ai_direction::north_1:
                    if (field_24_bf.bFlag6)
                    {
                        field_0_car->field_58_physics->TurnAntiClockwise_42AB90();
                    }
                    return;

                case car_ai_direction::south_2:
                    if (field_10_angle > kAng270_677B08)
                    {
                        field_0_car->field_58_physics->TurnClockwise_42ABA0();
                    }
                    else
                    {
                        field_0_car->field_58_physics->TurnAntiClockwise_42AB90();
                    }
                    return;

                case car_ai_direction::east_3:
                {
                    Fix16 lane_dy = field_0_car->field_50_car_sprite->field_14_xy.y -
                        Fix16((field_0_car->field_50_car_sprite->field_14_xy.y.ToUInt8()));
                    if (lane_dy > kFpPoint4_6779C8 && lane_dy < kFpPoint6_6779D0)
                    {
                        field_0_car->field_58_physics->SetGoStraight_42ABB0();
                    }
                    else
                    {
                        field_24_flags &= 0xFFFEDFFF;

                        if (lane_dy <= kFpPoint4_6779C8)
                        {
                            if (travel_angle < Ang16(lane_angle.rValue - kAng4_677CE2.rValue, 0))
                            {
                                if (travel_angle > Ang16(lane_angle_copy.rValue - kAng10_677A08.rValue, 0))
                                {
                                    field_0_car->field_58_physics->SetGoStraight_42ABB0();
                                    // This return (and the one in left_4/south_2) decide which identical tail VC6 keeps when cross-jumping
                                    return;
                                }
                                else
                                {
                                    field_0_car->field_58_physics->TurnAntiClockwise_42AB90();
                                }
                            }
                            else
                            {
                                field_0_car->field_58_physics->TurnClockwise_42ABA0();
                            }
                        }
                        else
                        {
                            if (travel_angle > Ang16(lane_angle.rValue + kAng4_677CE2.rValue, 0))
                            {
                                if (travel_angle < Ang16(kAng10_677A08.rValue + lane_angle_copy.rValue, 0))
                                {
                                    field_0_car->field_58_physics->SetGoStraight_42ABB0();
                                }
                                else
                                {
                                    field_0_car->field_58_physics->TurnClockwise_42ABA0();
                                }
                            }
                            else
                            {
                                field_0_car->field_58_physics->TurnAntiClockwise_42AB90();
                            }
                        }
                    }
                    return;
                }

                case car_ai_direction::west_4:
                    if (field_24_bf.bFlag7 && field_44_target_direction == car_ai_target_direction::southwards_2)
                    {
                        field_0_car->field_58_physics->TurnAntiClockwise_42AB90();
                    }
                    else
                    {
                        field_0_car->field_58_physics->TurnClockwise_42ABA0();
                    }
                    return;

                default:
                    return;
            }

            return;

        case road_direction::left_4:

            switch (field_4C_curr_direction)
            {
                case car_ai_direction::north_1:
                    if (field_10_angle > kAng180_677ADE)
                    {
                        field_0_car->field_58_physics->TurnAntiClockwise_42AB90();
                    }
                    else
                    {
                        field_0_car->field_58_physics->TurnClockwise_42ABA0();
                    }
                    return;

                case car_ai_direction::south_2:
                    if (field_24_bf.bFlag7 && field_44_target_direction == car_ai_target_direction::westwards_4)
                    {
                        field_0_car->field_58_physics->TurnClockwise_42ABA0();
                        return;
                    }
                    else
                    {
                        field_0_car->field_58_physics->TurnAntiClockwise_42AB90();
                    }
                    return;

                case car_ai_direction::east_3:
                    if (field_24_bf.bFlag7 && field_44_target_direction == car_ai_target_direction::northwards_1)
                    {
                        field_0_car->field_58_physics->TurnAntiClockwise_42AB90();
                    }
                    else
                    {
                        field_0_car->field_58_physics->TurnClockwise_42ABA0();
                    }
                    return;
            }
            return;

        default:

            field_24_bf.bFlag3 = true;
            if (!field_3C_seeking_road)
            {
                field_3C_seeking_road = 1;
                for (u8 k = 0; k < 4; k++)
                {
                    Fix16 xpos = gCurrCarAI_ProbeX_677A74;
                    Fix16 ypos = gCurrCarAI_ProbeY_677A80;
                    Fix16 zpos = field_0_car->field_50_car_sprite->field_1C_zpos;

                    switch (k)
                    {
                        case 0:
                            Ang16::PolarToCartesian_41FC20(field_10_angle, gF16fOne_677B94, vec_x, vec_y);
                            break;
                        case 1:
                            Ang16::PolarToCartesian_41FC20(field_10_angle + kAng90_6779E4, gF16fOne_677B94, vec_x, vec_y);
                            break;
                        case 2:
                            Ang16::PolarToCartesian_41FC20(field_10_angle - kAng90_6779E4, gF16fOne_677B94, vec_x, vec_y);
                            break;
                        case 3:
                            Ang16::PolarToCartesian_41FC20(field_10_angle + kAng180_677ADE, gF16fOne_677B94, vec_x, vec_y);
                            break;
                    }

                    xpos += vec_x;
                    ypos += vec_y;

                    gmp_block_info* pBlock = gMap_0x370_6F6268->GetBlock_4DFE10(xpos.ToInt(), ypos.ToInt(), zpos.ToInt());
                    if (!pBlock || !IS_GRADIENT_SLOPE(pBlock->field_B_slope_type) ||
                        IS_AIR_TYPE(pBlock->field_B_slope_type))
                    {
                        pBlock = gMap_0x370_6F6268->GetBlock_4DFE10(xpos.ToInt(), ypos.ToInt(), zpos.ToInt() - 1);
                    }
                    if (pBlock && gMap_0x370_6F6268->GetArrowDirectionFromBlock_4E5FC0(pBlock, 1))
                    {
                        field_24_flags &= ~0xC0;
                        switch (k)
                        {
                            case 0:
                                field_50_seek_road_turn = 0;
                                break;
                            case 1:
                                field_50_seek_road_turn = 1;
                                break;
                            case 2:
                                field_50_seek_road_turn = 2;
                                break;
                            case 3:
                                field_50_seek_road_turn = 3;
                                break;
                        }
                        return;
                    }
                } // end for loop
            }
            else if (field_50_seek_road_turn == 3)
            {
                field_0_car->field_58_physics->SetGoStraight_42ABB0();
            }
    }
}

// The lane angles are Ang16 operator-/operator+ temporaries. Each gets its own stack slot, and past the
// inline budget their Normalize is called out of line (0x406C20) on it. The angle global is the left
// operand (this) in every sum: with kAng12_677A2E as `this` in one site and the argument in another, VC6
// loads both with 32-bit movs instead of a 16-bit mov/sub.
MATCH_FUNC(0x44af00)
void CarAI::AlignToLaneCenter_44AF00()
{
    s8 arrow_run_len = 0;
    if (this->field_24_bf.bFlag8)
    {
        gCurrCarAI_AllowTrafficSteering_677A5D = 0;
        s8 count = this->field_0_car->CountConsecutiveArrowBlocks_4410D0(this->field_10_angle,
                                                                         &arrow_run_len,
                                                                         gCurrCarAI_xpos_677C38,
                                                                         gCurrCarAI_ypos_677C30);

        this->field_0_car->field_50_car_sprite->set_xyz_lazy_420600(gCurrCarAI_xpos_677C38,
                                                                    gCurrCarAI_ypos_677C30,
                                                                    gCurrCarAI_zpos_677C48);

        if (count == static_cast<u8>(this->field_2F_arrow_end))
        {
            switch (this->field_4C_curr_direction)
            {
                case car_ai_direction::north_1:
                {
                    Fix16 off = this->field_0_car->field_50_car_sprite->field_14_xy.x -
                        Fix16(this->field_0_car->field_50_car_sprite->field_14_xy.x.ToUInt8());
                    if (off >= kFpPoint4_6779C8 && off <= kFpPoint6_6779D0)
                    {
                        gCurrCarAI_AllowTrafficSteering_677A5D = 1;
                        this->field_24_bf.bFlag16 = true;
                    }
                    else if (off < kFpPoint4_6779C8)
                    {
                        if (this->field_10_angle > kAng180_677ADE - kAng12_677A2E)
                        {
                            this->field_0_car->field_58_physics->TurnClockwise_42ABA0();
                        }
                        else
                        {
                            this->field_0_car->field_58_physics->SetGoStraight_42ABB0();
                        }
                    }
                    else
                    {
                        if (this->field_10_angle < kAng180_677ADE + kAng12_677A2E)
                        {
                            this->field_0_car->field_58_physics->TurnAntiClockwise_42AB90();
                        }
                        else
                        {
                            this->field_0_car->field_58_physics->SetGoStraight_42ABB0();
                        }
                    }
                    break;
                }

                case car_ai_direction::south_2:
                {
                    Fix16 off = this->field_0_car->field_50_car_sprite->field_14_xy.x -
                        Fix16(this->field_0_car->field_50_car_sprite->field_14_xy.x.ToUInt8());
                    if (off >= kFpPoint4_6779C8 && off <= kFpPoint6_6779D0)
                    {
                        gCurrCarAI_AllowTrafficSteering_677A5D = 1;
                        this->field_24_bf.bFlag16 = true;
                    }
                    else if (off < kFpPoint4_6779C8)
                    {
                        if (this->field_10_angle < kAng12_677A2E)
                        {
                            this->field_0_car->field_58_physics->TurnAntiClockwise_42AB90();
                        }
                        else
                        {
                            this->field_0_car->field_58_physics->SetGoStraight_42ABB0();
                        }
                    }
                    else
                    {
                        if (this->field_10_angle > kAng0_677CE8 - kAng12_677A2E)
                        {
                            this->field_0_car->field_58_physics->TurnClockwise_42ABA0();
                        }
                        else
                        {
                            this->field_0_car->field_58_physics->SetGoStraight_42ABB0();
                        }
                    }
                    break;
                }

                case car_ai_direction::east_3:
                {
                    Fix16 off = this->field_0_car->field_50_car_sprite->field_14_xy.y -
                        Fix16(this->field_0_car->field_50_car_sprite->field_14_xy.y.ToUInt8());
                    if (off > kFpPoint4_6779C8 && off < kFpPoint6_6779D0)
                    {
                        gCurrCarAI_AllowTrafficSteering_677A5D = 1;
                        this->field_24_bf.bFlag16 = true;
                    }
                    else if (off <= kFpPoint4_6779C8)
                    {
                        if (this->field_10_angle > kAng90_6779E4 - kAng12_677A2E)
                        {
                            this->field_0_car->field_58_physics->TurnClockwise_42ABA0();
                        }
                        else
                        {
                            this->field_0_car->field_58_physics->SetGoStraight_42ABB0();
                        }
                    }
                    else
                    {
                        if (this->field_10_angle < kAng90_6779E4 + kAng12_677A2E)
                        {
                            this->field_0_car->field_58_physics->TurnAntiClockwise_42AB90();
                        }
                        else
                        {
                            this->field_0_car->field_58_physics->SetGoStraight_42ABB0();
                        }
                    }
                    break;
                }

                case car_ai_direction::west_4:
                {
                    Fix16 off = this->field_0_car->field_50_car_sprite->field_14_xy.y -
                        Fix16(this->field_0_car->field_50_car_sprite->field_14_xy.y.ToUInt8());
                    if (off > kFpPoint4_6779C8 && off < kFpPoint6_6779D0)
                    {
                        gCurrCarAI_AllowTrafficSteering_677A5D = 1;
                        this->field_24_bf.bFlag16 = true;
                    }
                    else if (off <= kFpPoint4_6779C8)
                    {
                        if (this->field_10_angle < kAng270_677B08 - kAng12_677A2E)
                        {
                            this->field_0_car->field_58_physics->TurnAntiClockwise_42AB90();
                        }
                        else
                        {
                            this->field_0_car->field_58_physics->SetGoStraight_42ABB0();
                        }
                    }
                    else
                    {
                        if (this->field_10_angle > kAng270_677B08 + kAng12_677A2E)
                        {
                            this->field_0_car->field_58_physics->TurnClockwise_42ABA0();
                        }
                        else
                        {
                            this->field_0_car->field_58_physics->SetGoStraight_42ABB0();
                        }
                    }
                    break;
                }

                default:
                    break;
            }

            if (field_24_bf.bFlag4 && field_24_bf.bFlag16)
            {
                this->field_24_bf.bFlag8 = false;
            }
        }
        else if (count < static_cast<u8>(this->field_2F_arrow_end))
        {
            // Move the sprite half a block to the right of the travel direction and check that it is free
            Fix16 x_off;
            Fix16 y_off;
            switch (this->field_4C_curr_direction)
            {
                case car_ai_direction::east_3:
                {
                    Ang16::PolarToCartesian_41FC20(kAng0_677CE8, kFpHalf_6779CC, x_off, y_off);
                    field_0_car->field_50_car_sprite->set_xyz_lazy_420600(field_0_car->field_50_car_sprite->field_14_xy.x + x_off,
                                                                          field_0_car->field_50_car_sprite->field_14_xy.y + y_off,
                                                                          field_0_car->field_50_car_sprite->field_1C_zpos);

                    if (gSpriteGrid_1_679208->FindCollidingSpritePreferType_477E60(this->field_0_car->field_50_car_sprite, 0))
                    {
                        gCurrCarAI_AllowTrafficSteering_677A5D = 1;
                    }
                    else
                    {
                        if (this->field_10_angle > kAng90_6779E4 - kAng12_677A2E)
                        {
                            this->field_0_car->field_58_physics->TurnClockwise_42ABA0();
                        }
                        else
                        {
                            this->field_0_car->field_58_physics->SetGoStraight_42ABB0();
                        }
                    }
                    break;
                }

                case car_ai_direction::west_4:
                {
                    Ang16::PolarToCartesian_41FC20(kAng180_677ADE, kFpHalf_6779CC, x_off, y_off);
                    field_0_car->field_50_car_sprite->set_xyz_lazy_420600(field_0_car->field_50_car_sprite->field_14_xy.x + x_off,
                                                                          field_0_car->field_50_car_sprite->field_14_xy.y + y_off,
                                                                          field_0_car->field_50_car_sprite->field_1C_zpos);

                    if (gSpriteGrid_1_679208->FindCollidingSpritePreferType_477E60(this->field_0_car->field_50_car_sprite, 0))
                    {
                        gCurrCarAI_AllowTrafficSteering_677A5D = 1;
                    }
                    else
                    {
                        if (this->field_10_angle > kAng270_677B08 - kAng12_677A2E)
                        {
                            this->field_0_car->field_58_physics->TurnClockwise_42ABA0();
                        }
                        else
                        {
                            this->field_0_car->field_58_physics->SetGoStraight_42ABB0();
                        }
                    }
                    break;
                }

                case car_ai_direction::south_2:
                {
                    Ang16::PolarToCartesian_41FC20(kAng270_677B08, kFpHalf_6779CC, x_off, y_off);
                    field_0_car->field_50_car_sprite->set_xyz_lazy_420600(field_0_car->field_50_car_sprite->field_14_xy.x + x_off,
                                                                          field_0_car->field_50_car_sprite->field_14_xy.y + y_off,
                                                                          field_0_car->field_50_car_sprite->field_1C_zpos);

                    if (gSpriteGrid_1_679208->FindCollidingSpritePreferType_477E60(this->field_0_car->field_50_car_sprite, 0))
                    {
                        gCurrCarAI_AllowTrafficSteering_677A5D = 1;
                    }
                    else
                    {
                        if (this->field_10_angle <= kAng0_677CE8 - kAng12_677A2E && this->field_10_angle >= kAng90_6779E4)
                        {
                            this->field_0_car->field_58_physics->SetGoStraight_42ABB0();
                        }
                        else
                        {
                            this->field_0_car->field_58_physics->TurnClockwise_42ABA0();
                        }
                    }
                    break;
                }

                case car_ai_direction::north_1:
                {
                    Ang16::PolarToCartesian_41FC20(kAng90_6779E4, kFpHalf_6779CC, x_off, y_off);
                    field_0_car->field_50_car_sprite->set_xyz_lazy_420600(field_0_car->field_50_car_sprite->field_14_xy.x + x_off,
                                                                          field_0_car->field_50_car_sprite->field_14_xy.y + y_off,
                                                                          field_0_car->field_50_car_sprite->field_1C_zpos);

                    if (gSpriteGrid_1_679208->FindCollidingSpritePreferType_477E60(this->field_0_car->field_50_car_sprite, 0))
                    {
                        gCurrCarAI_AllowTrafficSteering_677A5D = 1;
                    }
                    else
                    {
                        if (this->field_10_angle > kAng180_677ADE - kAng12_677A2E)
                        {
                            this->field_0_car->field_58_physics->TurnClockwise_42ABA0();
                        }
                        else
                        {
                            this->field_0_car->field_58_physics->SetGoStraight_42ABB0();
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
            switch (this->field_4C_curr_direction)
            {
                case car_ai_direction::east_3:
                {
                    Ang16::PolarToCartesian_41FC20(kAng180_677ADE, kFpHalf_6779CC, x_off, y_off);
                    field_0_car->field_50_car_sprite->set_xyz_lazy_420600(field_0_car->field_50_car_sprite->field_14_xy.x + x_off,
                                                                          field_0_car->field_50_car_sprite->field_14_xy.y + y_off,
                                                                          field_0_car->field_50_car_sprite->field_1C_zpos);

                    if (gSpriteGrid_1_679208->FindCollidingSpritePreferType_477E60(this->field_0_car->field_50_car_sprite, 0))
                    {
                        gCurrCarAI_AllowTrafficSteering_677A5D = 1;
                    }
                    else
                    {
                        if (this->field_10_angle < kAng90_6779E4 + kAng12_677A2E)
                        {
                            this->field_0_car->field_58_physics->TurnAntiClockwise_42AB90();
                        }
                        else
                        {
                            this->field_0_car->field_58_physics->SetGoStraight_42ABB0();
                        }
                    }
                    break;
                }

                case car_ai_direction::west_4:
                {
                    Ang16::PolarToCartesian_41FC20(kAng0_677CE8, kFpHalf_6779CC, x_off, y_off);
                    field_0_car->field_50_car_sprite->set_xyz_lazy_420600(field_0_car->field_50_car_sprite->field_14_xy.x + x_off,
                                                                          field_0_car->field_50_car_sprite->field_14_xy.y + y_off,
                                                                          field_0_car->field_50_car_sprite->field_1C_zpos);

                    if (gSpriteGrid_1_679208->FindCollidingSpritePreferType_477E60(this->field_0_car->field_50_car_sprite, 0))
                    {
                        gCurrCarAI_AllowTrafficSteering_677A5D = 1;
                    }
                    else
                    {
                        if (this->field_10_angle < kAng270_677B08 + kAng12_677A2E)
                        {
                            this->field_0_car->field_58_physics->TurnAntiClockwise_42AB90();
                        }
                        else
                        {
                            this->field_0_car->field_58_physics->SetGoStraight_42ABB0();
                        }
                    }
                    break;
                }

                case car_ai_direction::south_2:
                {
                    Ang16::PolarToCartesian_41FC20(kAng90_6779E4, kFpHalf_6779CC, x_off, y_off);
                    field_0_car->field_50_car_sprite->set_xyz_lazy_420600(field_0_car->field_50_car_sprite->field_14_xy.x + x_off,
                                                                          field_0_car->field_50_car_sprite->field_14_xy.y + y_off,
                                                                          field_0_car->field_50_car_sprite->field_1C_zpos);

                    if (gSpriteGrid_1_679208->FindCollidingSpritePreferType_477E60(this->field_0_car->field_50_car_sprite, 0))
                    {
                        gCurrCarAI_AllowTrafficSteering_677A5D = 1;
                    }
                    else
                    {
                        if (this->field_10_angle < kAng0_677CE8 + kAng12_677A2E)
                        {
                            this->field_0_car->field_58_physics->TurnAntiClockwise_42AB90();
                        }
                        else
                        {
                            this->field_0_car->field_58_physics->SetGoStraight_42ABB0();
                        }
                    }
                    break;
                }

                case car_ai_direction::north_1:
                {
                    Ang16::PolarToCartesian_41FC20(kAng270_677B08, kFpHalf_6779CC, x_off, y_off);
                    field_0_car->field_50_car_sprite->set_xyz_lazy_420600(field_0_car->field_50_car_sprite->field_14_xy.x + x_off,
                                                                          field_0_car->field_50_car_sprite->field_14_xy.y + y_off,
                                                                          field_0_car->field_50_car_sprite->field_1C_zpos);

                    if (gSpriteGrid_1_679208->FindCollidingSpritePreferType_477E60(this->field_0_car->field_50_car_sprite, 0))
                    {
                        gCurrCarAI_AllowTrafficSteering_677A5D = 1;
                    }
                    else
                    {
                        if (this->field_10_angle < kAng180_677ADE + kAng12_677A2E)
                        {
                            this->field_0_car->field_58_physics->TurnAntiClockwise_42AB90();
                        }
                        else
                        {
                            this->field_0_car->field_58_physics->SetGoStraight_42ABB0();
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
void CarAI::DetectCarAhead_44D1D0()
{
    Fix16 x_off;
    Fix16 y_off;
    s8 arrow_count = 0;
    char_type arrow_idx;
    u8 bSlowerThanTarget = 0;

    byte_677A94 = 0;
    if (this->field_0_car->field_84_car_info_idx == car_model_enum::BUS)
    {
        return;
    }

    if (this->field_2B_ticks_since_alloc <= 20)
    {
        return;
    }

    if (bOnGradientSlope_677C90)
    {
        return;
    }

    if (!field_24_bf.bFlag4)
    {
        return;
    }

    if (this->field_8_maneuver_active)
    {
        return;
    }

    if (field_24_bf.bTargetSetFromArrows)
    {
        return;
    }

    if ((field_24_flags & 0x40000) == 0x40000 || (field_24_flags & 0x80000) == 0x80000)
    {
        byte_677A94 = 1;
        this->field_70_nearest_entity = this->field_0_car->field_50_car_sprite;
    }
    else
    {
        // Look for a car 3 units ahead
        Ang16::PolarToCartesian_41FC20(field_10_angle, kFpThree_677B9C, x_off, y_off);
        field_0_car->field_50_car_sprite->set_xyz_lazy_420600(field_0_car->field_50_car_sprite->field_14_xy.x + x_off,
                                                              field_0_car->field_50_car_sprite->field_14_xy.y + y_off,
                                                              field_0_car->field_50_car_sprite->field_1C_zpos);

        if (!this->field_70_nearest_entity)
        {
            this->field_70_nearest_entity = gSpriteGrid_1_679208->FindCollidingSpritePreferType_477E60(this->field_0_car->field_50_car_sprite, 0);
        }
    }

    if (!this->field_70_nearest_entity)
    {
        return;
    }

    if (this->field_70_nearest_entity->field_30_sprite_type_enum != sprite_types_enum::car_2)
    {
        return;
    }

    if (gCurrCarAI_IsChasing_677BBC && gCurrCarAI_TargetCar_6779B0 == this->field_70_nearest_entity->field_8_car_bc_ptr)
    {
        return;
    }

    if (!byte_677A94)
    {
        if (field_70_nearest_entity->field_8_car_bc_ptr->GetCarLinearSpeed_43A240() < gCurrCarAI_DesiredSpeed_677A8C)
        {
            bSlowerThanTarget = 1;
        }

        if (byte_677A6C && this->field_70_nearest_entity->field_8_car_bc_ptr == gCurrCarAI_TargetCar_6779B0)
        {
            return;
        }

        CarAI* pOtherAI = this->field_70_nearest_entity->field_8_car_bc_ptr->field_5C_AI;
        if (pOtherAI)
        {
            if (gCurrCarAI_DesiredSpeed_677A8C > pOtherAI->field_74_max_speed)
            {
                bSlowerThanTarget = 1;
            }
            if ((pOtherAI->field_24_flags & 0x100) == 0x100)
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
        Ang16::PolarToCartesian_41FC20(field_10_angle, gF16fOne_677B94 * Fix16(i), x_off, y_off);
        gCurrCarAI_ProbeX_677A74 = field_0_car->field_50_car_sprite->field_14_xy.x + x_off;
        gCurrCarAI_ProbeY_677A80 = field_0_car->field_50_car_sprite->field_14_xy.y + y_off;

        if (gCurrCarAI_ProbeX_677A74 > kF16Zero_677B90 && gCurrCarAI_ProbeY_677A80 > kF16Zero_677B90 && gCurrCarAI_ProbeX_677A74 < kFp255_677950 &&
            gCurrCarAI_ProbeY_677A80 < kFp255_677950)
        {
            gmp_block_info* pBlock = gMap_0x370_6F6268->GetBlock_4DFE10(gCurrCarAI_ProbeX_677A74.ToInt(),
                                                                         gCurrCarAI_ProbeY_677A80.ToInt(),
                                                                         field_0_car->field_50_car_sprite->field_1C_zpos.ToInt());

            if (!pBlock ||
                !(IS_GRADIENT_SLOPE(pBlock->field_B_slope_type) &&
                  !IS_AIR_TYPE(pBlock->field_B_slope_type)))
            {
                pBlock = gMap_0x370_6F6268->GetBlock_4DFE10(gCurrCarAI_ProbeX_677A74.ToInt(),
                                                             gCurrCarAI_ProbeY_677A80.ToInt(),
                                                             field_0_car->field_50_car_sprite->field_1C_zpos.ToInt() - 1);
            }

            if (pBlock && TrySetTargetDirectionFromArrows_447D40(pBlock))
            {
                this->field_24_bf.bFlag8 = false;
                return;
            }
        }
    }

    arrow_idx = field_0_car->CountConsecutiveArrowBlocks_4410D0(field_10_angle, &arrow_count, gCurrCarAI_xpos_677C38, gCurrCarAI_ypos_677C30);
    if (arrow_count <= 1)
    {
        return;
    }

    if ((byte_677A94 && (field_24_bf.bFlag19)) || (!byte_677A94 && arrow_idx > 0))
    {
        if (arrow_idx != arrow_count - 1)
        {
            return;
        }

        if (!byte_677A94 && !(gCurrCarAI_DesiredSpeed_677A8C > kFpPoint2_6779C0) &&
            !(field_70_nearest_entity->field_8_car_bc_ptr->GetCarLinearSpeed_43A240() == kF16Zero_677B90))
        {
            gCurrCarAI_DesiredSpeed_677A8C = field_70_nearest_entity->field_8_car_bc_ptr->GetCarLinearSpeed_43A240();
            return;
        }

        field_0_car->field_50_car_sprite->set_xyz_lazy_420600(gCurrCarAI_xpos_677C38, gCurrCarAI_ypos_677C30, gCurrCarAI_zpos_677C48);

        // goto tail_1 (not break): all four cases end with byte-identical probe code, which VC6
        // folds into one copy with the others jumping in. With `break`, the fall-through (west, the
        // last case) survives and the rest jump into it; `goto tail_1` (the label `break` would reach
        // anyway) keeps the first copy (north) instead, as the original does (see matching_quirks.md,
        // "Which copy survives can also depend on how the cases leave the switch").
        switch (this->field_4C_curr_direction)
        {
            case car_ai_direction::north_1:
                Ang16::PolarToCartesian_41FC20(kAng270_677B08, kFpPoint7_6779D4, x_off, y_off);
                field_0_car->field_50_car_sprite->set_xyz_lazy_420600(field_0_car->field_50_car_sprite->field_14_xy.x + x_off,
                                                                      field_0_car->field_50_car_sprite->field_14_xy.y + y_off,
                                                                      field_0_car->field_50_car_sprite->field_1C_zpos);
                if (gSpriteGrid_1_679208->FindCollidingSpritePreferType_477E60(field_0_car->field_50_car_sprite, 0))
                {
                    return;
                }
                Ang16::PolarToCartesian_41FC20(kAng180_677ADE, kFpTwo_677B98, x_off, y_off);
                field_0_car->field_50_car_sprite->set_xyz_lazy_420600(field_0_car->field_50_car_sprite->field_14_xy.x + x_off,
                                                                      field_0_car->field_50_car_sprite->field_14_xy.y + y_off,
                                                                      field_0_car->field_50_car_sprite->field_1C_zpos);
                if (gSpriteGrid_1_679208->FindCollidingSpritePreferType_477E60(field_0_car->field_50_car_sprite, 0))
                {
                    return;
                }
                goto tail_1;

            case car_ai_direction::south_2:
                Ang16::PolarToCartesian_41FC20(kAng90_6779E4, kFpPoint7_6779D4, x_off, y_off);
                field_0_car->field_50_car_sprite->set_xyz_lazy_420600(field_0_car->field_50_car_sprite->field_14_xy.x + x_off,
                                                                      field_0_car->field_50_car_sprite->field_14_xy.y + y_off,
                                                                      field_0_car->field_50_car_sprite->field_1C_zpos);
                if (gSpriteGrid_1_679208->FindCollidingSpritePreferType_477E60(field_0_car->field_50_car_sprite, 0))
                {
                    return;
                }
                Ang16::PolarToCartesian_41FC20(kAng0_677CE8, kFpTwo_677B98, x_off, y_off);
                field_0_car->field_50_car_sprite->set_xyz_lazy_420600(field_0_car->field_50_car_sprite->field_14_xy.x + x_off,
                                                                      field_0_car->field_50_car_sprite->field_14_xy.y + y_off,
                                                                      field_0_car->field_50_car_sprite->field_1C_zpos);
                if (gSpriteGrid_1_679208->FindCollidingSpritePreferType_477E60(field_0_car->field_50_car_sprite, 0))
                {
                    return;
                }
                goto tail_1;

            case car_ai_direction::east_3:
                Ang16::PolarToCartesian_41FC20(kAng180_677ADE, kFpPoint7_6779D4, x_off, y_off);
                field_0_car->field_50_car_sprite->set_xyz_lazy_420600(field_0_car->field_50_car_sprite->field_14_xy.x + x_off,
                                                                      field_0_car->field_50_car_sprite->field_14_xy.y + y_off,
                                                                      field_0_car->field_50_car_sprite->field_1C_zpos);
                if (gSpriteGrid_1_679208->FindCollidingSpritePreferType_477E60(field_0_car->field_50_car_sprite, 0))
                {
                    return;
                }
                Ang16::PolarToCartesian_41FC20(kAng90_6779E4, kFpTwo_677B98, x_off, y_off);
                field_0_car->field_50_car_sprite->set_xyz_lazy_420600(field_0_car->field_50_car_sprite->field_14_xy.x + x_off,
                                                                      field_0_car->field_50_car_sprite->field_14_xy.y + y_off,
                                                                      field_0_car->field_50_car_sprite->field_1C_zpos);
                if (gSpriteGrid_1_679208->FindCollidingSpritePreferType_477E60(field_0_car->field_50_car_sprite, 0))
                {
                    return;
                }
                goto tail_1;

            case car_ai_direction::west_4:
                Ang16::PolarToCartesian_41FC20(kAng0_677CE8, kFpPoint7_6779D4, x_off, y_off);
                field_0_car->field_50_car_sprite->set_xyz_lazy_420600(field_0_car->field_50_car_sprite->field_14_xy.x + x_off,
                                                                      field_0_car->field_50_car_sprite->field_14_xy.y + y_off,
                                                                      field_0_car->field_50_car_sprite->field_1C_zpos);
                if (gSpriteGrid_1_679208->FindCollidingSpritePreferType_477E60(field_0_car->field_50_car_sprite, 0))
                {
                    return;
                }
                Ang16::PolarToCartesian_41FC20(kAng270_677B08, kFpTwo_677B98, x_off, y_off);
                field_0_car->field_50_car_sprite->set_xyz_lazy_420600(field_0_car->field_50_car_sprite->field_14_xy.x + x_off,
                                                                      field_0_car->field_50_car_sprite->field_14_xy.y + y_off,
                                                                      field_0_car->field_50_car_sprite->field_1C_zpos);
                if (gSpriteGrid_1_679208->FindCollidingSpritePreferType_477E60(field_0_car->field_50_car_sprite, 0))
                {
                    return;
                }
                goto tail_1;

            default:
                return;
        }

    tail_1:
        this->field_2D_arrow_start = arrow_idx;
        this->field_24_bf.bFlag8 = true;
        this->field_2E_arrow_cur = arrow_idx;
        this->field_2F_arrow_end = arrow_idx - 1;
        return;
    }

    CarAI* pOtherAI = field_70_nearest_entity->field_8_car_bc_ptr->field_5C_AI;
    // goto: the original returns through its own inline epilogue copy here, which VC6 only emits for
    // `if (c) goto ok; return;` (see docs/matching_quirks.md); `if (timer < 50) return;` jumps to the shared one
    if (pOtherAI)
    {
        if (pOtherAI->field_2A_stopped_timer >= 50)
        {
            goto probe_sides;
        }
        return;
    }
    if (field_70_nearest_entity->field_8_car_bc_ptr->GetCarLinearSpeed_43A240() > kFpPoint04_677B64)
    {
        return;
    }

probe_sides:

    if (gCurrCarAI_Velocity_677B00 < kFpPoint05_677B60 || byte_677A94)
    {
        field_0_car->field_50_car_sprite->set_xyz_lazy_420600(gCurrCarAI_xpos_677C38, gCurrCarAI_ypos_677C30, gCurrCarAI_zpos_677C48);

        // goto tail_2 (not break): same tail-merge reason as the first switch above.
        switch (this->field_4C_curr_direction)
        {
            case car_ai_direction::north_1:
                x_off = Ang16::sine_40F500(kAng90_6779E4) * kFpPoint7_6779D4;
                y_off = Ang16::cosine_40F520(kAng90_6779E4) * kFpPoint7_6779D4;
                field_0_car->field_50_car_sprite->set_xyz_lazy_420600(field_0_car->field_50_car_sprite->field_14_xy.x + x_off,
                                                                      field_0_car->field_50_car_sprite->field_14_xy.y + y_off,
                                                                      field_0_car->field_50_car_sprite->field_1C_zpos);
                if (gSpriteGrid_1_679208->FindCollidingSpritePreferType_477E60(field_0_car->field_50_car_sprite, 0))
                {
                    return;
                }
                x_off = Ang16::sine_40F500(kAng180_677ADE) * kFpTwo_677B98;
                y_off = Ang16::cosine_40F520(kAng180_677ADE) * kFpTwo_677B98;
                field_0_car->field_50_car_sprite->set_xyz_lazy_420600(field_0_car->field_50_car_sprite->field_14_xy.x + x_off,
                                                                      field_0_car->field_50_car_sprite->field_14_xy.y + y_off,
                                                                      field_0_car->field_50_car_sprite->field_1C_zpos);
                if (gSpriteGrid_1_679208->FindCollidingSpritePreferType_477E60(field_0_car->field_50_car_sprite, 0))
                {
                    return;
                }
                goto tail_2;

            case car_ai_direction::south_2:
                x_off = Ang16::sine_40F500(kAng270_677B08) * kFpPoint7_6779D4;
                y_off = Ang16::cosine_40F520(kAng270_677B08) * kFpPoint7_6779D4;
                field_0_car->field_50_car_sprite->set_xyz_lazy_420600(field_0_car->field_50_car_sprite->field_14_xy.x + x_off,
                                                                      field_0_car->field_50_car_sprite->field_14_xy.y + y_off,
                                                                      field_0_car->field_50_car_sprite->field_1C_zpos);
                if (gSpriteGrid_1_679208->FindCollidingSpritePreferType_477E60(field_0_car->field_50_car_sprite, 0))
                {
                    return;
                }
                x_off = Ang16::sine_40F500(kAng0_677CE8) * kFpTwo_677B98;
                y_off = Ang16::cosine_40F520(kAng0_677CE8) * kFpTwo_677B98;
                field_0_car->field_50_car_sprite->set_xyz_lazy_420600(field_0_car->field_50_car_sprite->field_14_xy.x + x_off,
                                                                      field_0_car->field_50_car_sprite->field_14_xy.y + y_off,
                                                                      field_0_car->field_50_car_sprite->field_1C_zpos);
                if (gSpriteGrid_1_679208->FindCollidingSpritePreferType_477E60(field_0_car->field_50_car_sprite, 0))
                {
                    return;
                }
                goto tail_2;

            case car_ai_direction::east_3:
                x_off = Ang16::sine_40F500(kAng0_677CE8) * kFpPoint7_6779D4;
                y_off = Ang16::cosine_40F520(kAng0_677CE8) * kFpPoint7_6779D4;
                field_0_car->field_50_car_sprite->set_xyz_lazy_420600(field_0_car->field_50_car_sprite->field_14_xy.x + x_off,
                                                                      field_0_car->field_50_car_sprite->field_14_xy.y + y_off,
                                                                      field_0_car->field_50_car_sprite->field_1C_zpos);
                if (gSpriteGrid_1_679208->FindCollidingSpritePreferType_477E60(field_0_car->field_50_car_sprite, 0))
                {
                    return;
                }
                x_off = Ang16::sine_40F500(kAng90_6779E4) * kFpTwo_677B98;
                y_off = Ang16::cosine_40F520(kAng90_6779E4) * kFpTwo_677B98;
                field_0_car->field_50_car_sprite->set_xyz_lazy_420600(field_0_car->field_50_car_sprite->field_14_xy.x + x_off,
                                                                      field_0_car->field_50_car_sprite->field_14_xy.y + y_off,
                                                                      field_0_car->field_50_car_sprite->field_1C_zpos);
                if (gSpriteGrid_1_679208->FindCollidingSpritePreferType_477E60(field_0_car->field_50_car_sprite, 0))
                {
                    return;
                }
                goto tail_2;

            case car_ai_direction::west_4:
                x_off = Ang16::sine_40F500(kAng180_677ADE) * kFpPoint7_6779D4;
                y_off = Ang16::cosine_40F520(kAng180_677ADE) * kFpPoint7_6779D4;
                field_0_car->field_50_car_sprite->set_xyz_lazy_420600(field_0_car->field_50_car_sprite->field_14_xy.x + x_off,
                                                                      field_0_car->field_50_car_sprite->field_14_xy.y + y_off,
                                                                      field_0_car->field_50_car_sprite->field_1C_zpos);
                if (gSpriteGrid_1_679208->FindCollidingSpritePreferType_477E60(field_0_car->field_50_car_sprite, 0))
                {
                    return;
                }
                x_off = Ang16::sine_40F500(kAng270_677B08) * kFpTwo_677B98;
                y_off = Ang16::cosine_40F520(kAng270_677B08) * kFpTwo_677B98;
                field_0_car->field_50_car_sprite->set_xyz_lazy_420600(field_0_car->field_50_car_sprite->field_14_xy.x + x_off,
                                                                      field_0_car->field_50_car_sprite->field_14_xy.y + y_off,
                                                                      field_0_car->field_50_car_sprite->field_1C_zpos);
                if (gSpriteGrid_1_679208->FindCollidingSpritePreferType_477E60(field_0_car->field_50_car_sprite, 0))
                {
                    return;
                }
                goto tail_2;

            default:
                return;
        }

    tail_2:
        this->field_24_bf.bFlag8 = true;
        this->field_2D_arrow_start = 0;
        this->field_2E_arrow_cur = 0;
        u8 next_idx = arrow_idx + 1;
        // --arrow_count (not arrow_count - 1): the dec then belongs to arrow_count, last_idx is only a copy
        // (colour priority 28 instead of 52), so next_idx (36) is coloured first and gets al, as in 10.5
        u8 last_idx = --arrow_count;
        this->field_2F_arrow_end = next_idx;
        if (next_idx > last_idx)
        {
            this->field_2F_arrow_end = last_idx;
        }
    }
}

MATCH_FUNC(0x44e0c0)
void CarAI::UpdateChase_44E0C0()
{
    gCurrCarAI_IsChasing_677BBC = 1;
    u8 t_x;
    u8 t_y;
    u8 t_z = 0;
    this->field_74_max_speed = this->field_0_car->field_54_driver->field_1F8_run_speed;
    CarChaseTask* p60_ = field_0_car->field_60_pChaseTask;

    Fix16 target_x;
    Fix16 target_y;
    Fix16 target_z;

    if (p60_->field_8_task_type != car_task_type::goto_position_1)
    {
        gCurrCarAI_TargetCar_6779B0 = p60_->field_30_pTargetPed->field_16C_car;
        target_x = this->field_0_car->field_60_pChaseTask->field_30_pTargetPed->get_cam_x();
        gCurrCarAI_TargetX_6779F0 = target_x;
        target_y = this->field_0_car->field_60_pChaseTask->field_30_pTargetPed->get_cam_y();
        gCurrCarAI_TargetY_6779F4 = target_y;
        target_z = this->field_0_car->field_60_pChaseTask->field_30_pTargetPed->get_cam_z();
    }
    else
    {
        target_x = p60_->field_14_target_x;
        gCurrCarAI_TargetX_6779F0 = target_x;
        target_y = this->field_0_car->field_60_pChaseTask->field_18_target_y;
        gCurrCarAI_TargetY_6779F4 = target_y;
        target_z = this->field_0_car->field_60_pChaseTask->field_1C_target_z;
    }

    gCurrCarAI_TargetZ_6779F8 = target_z;
    Fix16 dist;
    dist = Fix16::MaxAbsDistance_42A6B0(gCurrCarAI_xpos_677C38, gCurrCarAI_ypos_677C30, gCurrCarAI_TargetX_6779F0, gCurrCarAI_TargetY_6779F4);
    if (dist < kFpFive_677BA4 && this->field_0_car->field_60_pChaseTask->field_8_task_type != car_task_type::goto_position_1)
    {
        if (field_28_route_idx > 0)
        {
            gRouteFinder_6FFDC8->CancelRoute_589930(field_28_route_idx);
            this->field_28_route_idx = -1;
        }

        this->field_0_car->field_60_pChaseTask->field_22_following_route = 0;

        if (gPolice_7B8_6FEE40->field_654_max_wanted_level > 2 || field_0_car->IsPoliceCar_439EC0())
        {
            this->field_0_car->field_60_pChaseTask->field_34_snap_timer = 100;
        }
        else
        {
            this->field_0_car->field_60_pChaseTask->field_34_snap_timer = 300;
        }
        return;
    }

    CarChaseTask* p60 = this->field_0_car->field_60_pChaseTask;
    if (p60->field_20_can_snap_to_target)
    {
        if (p60->field_34_snap_timer == 0)
        {
            if (!gGame_0x40_67E008->IsSpriteInAnyPlayerBounds_4B97E0(this->field_0_car->field_50_car_sprite, kFpFive_677CA0))
            {
                if (this->field_0_car->field_58_physics)
                {
                    if (!this->field_24_bf.bFlag6)
                    {
                        if (field_0_car->SnapCarToGreenArrow_444E40(gCurrCarAI_TargetX_6779F0, gCurrCarAI_TargetY_6779F4, gCurrCarAI_TargetZ_6779F8))
                        {
                            if (gPolice_7B8_6FEE40->field_654_max_wanted_level > 2 || field_0_car->IsPoliceCar_439EC0())
                            {
                                this->field_0_car->field_60_pChaseTask->field_34_snap_timer = 100;
                            }
                            else
                            {
                                this->field_0_car->field_60_pChaseTask->field_34_snap_timer = 300;
                            }

                            this->field_8_maneuver_active = 0;
                            this->field_24_flags &= ~0x4540u;
                            this->field_0_car->field_60_pChaseTask->field_C_chase_state = car_chase_state::follow_behind_0;
                            this->field_0_car->field_60_pChaseTask->field_2A_settle_counter = 0;
                            this->field_0_car->field_80_abandoned = 0;
                            field_0_car->RepairDamage_43D400();
                            gPolice_7B8_6FEE40->DespawnCrewInCar_56F6D0(this->field_0_car);
                        }
                    }
                }
            }
        }
        else if (static_cast<u16>(p60->field_34_snap_timer) > 0)
        {
            p60->field_34_snap_timer--;
        }
        return;
    }
    else
    {

        if (field_28_route_idx > 0)
        {
            gRouteFinder_6FFDC8->CancelRoute_589930(field_28_route_idx);
            this->field_28_route_idx = -1;
        }

        gmp_block_info* pBlock = gMap_0x370_6F6268->GetBlock_4DFE10(gCurrCarAI_TargetX_6779F0.ToInt(), gCurrCarAI_TargetY_6779F4.ToInt(), gCurrCarAI_TargetZ_6779F8.ToInt());
        Fix16 maybe_z = gCurrCarAI_TargetZ_6779F8;
        if (!(pBlock && IS_GRADIENT_SLOPE(pBlock->field_B_slope_type) &&
              !IS_AIR_TYPE(pBlock->field_B_slope_type)))
        {
            maybe_z = gCurrCarAI_TargetZ_6779F8 - gF16fOne_677B94;
            pBlock = gMap_0x370_6F6268->GetBlock_4DFE10(gCurrCarAI_TargetX_6779F0.ToInt(), gCurrCarAI_TargetY_6779F4.ToInt(), maybe_z.ToInt());
        }

        if (!pBlock || !static_cast<u8>(gMap_0x370_6F6268->GetArrowDirectionFromBlock_4E5FC0(pBlock, 1)))
        {
            t_x = gCurrCarAI_TargetX_6779F0.ToInt();
            t_y = gCurrCarAI_TargetY_6779F4.ToInt();
            t_z = gCurrCarAI_TargetZ_6779F8.ToInt();
            if (!gPathFinder_6FDEF0->FindNearbyTileMatchingSlopeType_5552B0(1, &t_x, &t_y, &t_z, 0))
            {
                if (field_28_route_idx > 0)
                {
                    gRouteFinder_6FFDC8->CancelRoute_589930(field_28_route_idx);
                    this->field_28_route_idx = -1;
                }

                this->field_0_car->field_60_pChaseTask->field_22_following_route = 0;

                if (gPolice_7B8_6FEE40->field_654_max_wanted_level > 2 || field_0_car->IsPoliceCar_439EC0())
                {
                    this->field_0_car->field_60_pChaseTask->field_34_snap_timer = 100;
                }
                else
                {
                    this->field_0_car->field_60_pChaseTask->field_34_snap_timer = 300;
                }
                return;
            }

            gCurrCarAI_TargetX_6779F0 = Fix16(t_x);
            gCurrCarAI_TargetY_6779F4 = Fix16(t_y);
            gCurrCarAI_TargetZ_6779F8 = Fix16(t_z);
            gmp_block_info* pBlock_ = gMap_0x370_6F6268->GetBlock_4DFE10(gCurrCarAI_TargetX_6779F0.ToInt(), gCurrCarAI_TargetY_6779F4.ToInt(), gCurrCarAI_TargetZ_6779F8.ToInt());
            maybe_z = gCurrCarAI_TargetZ_6779F8;
            if (!(pBlock_ && IS_GRADIENT_SLOPE(pBlock_->field_B_slope_type) &&
                  !IS_AIR_TYPE(pBlock_->field_B_slope_type)))
            {
                maybe_z = gCurrCarAI_TargetZ_6779F8 - gF16fOne_677B94;
                pBlock_ = gMap_0x370_6F6268->GetBlock_4DFE10(gCurrCarAI_TargetX_6779F0.ToInt(), gCurrCarAI_TargetY_6779F4.ToInt(), maybe_z.ToInt());
            }
            if (pBlock_)
            {
                gMap_0x370_6F6268->GetArrowDirectionFromBlock_4E5FC0(pBlock_, 1);
            }
        }

        GoToBlock_447CA0(gCurrCarAI_TargetX_6779F0.ToInt(), gCurrCarAI_TargetY_6779F4.ToInt(), maybe_z.ToInt(), 3);
        this->field_0_car->field_60_pChaseTask->field_22_following_route = 1;
    }
}

// https://decomp.me/scratch/Auxlx
// MaxAbsDistance_42A6B0 past the inline budget: the y difference stays inline, the Abs calls
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

// Inlined in 9.6f sub_42D820 (UpdateStateMachine_44E560)
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
void CarAI::UpdateStateMachine_44E560()
{
    Ang16 target_angle = kAng0_677CE8;
    Ang16 heading_copy = kAng0_677CE8;
    Ang16 aim_angle = kAng0_677CE8;
    Ang16 heading_delta = kAng0_677CE8;
    Fix16 dy = kF16Zero_677B90;
    CarChaseTask* pChase = 0;
    s16 waitCount = 0;
    this->field_24_flags &= ~0x4580u;
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

    if (field_0_car->field_60_pChaseTask != 0)
    {
        pChase = field_0_car->field_60_pChaseTask;
        switch (pChase->field_8_task_type)
        {
            case car_task_type::goto_position_1:
                break;

            default:
                if (pChase->field_30_pTargetPed->field_168_game_object != 0)
                {
                    pChase->field_C_chase_state = car_chase_state::follow_behind_0;
                }
                else if (field_0_car->IsPoliceCar_439EC0() != 0)
                {
                    Ped* pTargetPed = pChase->field_30_pTargetPed;
                    switch (pTargetPed->get_wanted_star_count_46EF00())
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
                pChase->field_C_chase_state = car_chase_state::choose_side_5;
                break;
        }
    }

    // Saved so the target offsets applied below can be undone
    Fix16 savedTargetX = gCurrCarAI_TargetX_6779F0;
    Fix16 savedTargetY = gCurrCarAI_TargetY_6779F4;

    this->field_24_flags &= 0xFFEFFFFF;
    if (field_3C_seeking_road == 0)
    {
        assert(pChase != 0);
        gCurrCarAI_AllowTrafficSteering_677A5D = 0;
        switch (gCarChaseTaskTable_678E30->CountPursuers_474920(pChase->field_30_pTargetPed, this->field_0_car->field_54_driver))
        {
            case 1:
                speed_limit = kFpPoint2_6779C0;
                break;
            case 2:
                speed_limit = kFpPoint2_6779C0 + kFpPoint05_677B60;
                break;
            default:
                speed_limit = kFpNine_677BB8;
                break;
        }

        switch (pChase->field_C_chase_state)
        {
            case car_chase_state::follow_behind_0:
                if (gCurrCarAI_TargetCar_6779B0 != 0)
                {
                    field_0_car->field_50_car_sprite->set_xyz_lazy_420600(gCurrCarAI_TargetX_6779F0, gCurrCarAI_TargetY_6779F4, gCurrCarAI_zpos_677C48);
                    Ang16::PolarToCartesian_41FC20(gCurrCarAI_TargetCar_6779B0->field_50_car_sprite->field_0_rotation - kAng180_677ADE, kFpThree_677B9C, dx, dy);
                    gCurrCarAI_TargetX_6779F0 += dx;
                    gCurrCarAI_TargetY_6779F4 += dy;
                    target_dist = Fix16::MaxAbsDistance_42A6B0(gCurrCarAI_xpos_677C38, gCurrCarAI_ypos_677C30, gCurrCarAI_TargetX_6779F0, gCurrCarAI_TargetY_6779F4);
                    target_angle = Fix16::atan2_fixed_405320(gCurrCarAI_TargetY_6779F4 - gCurrCarAI_ypos_677C30, gCurrCarAI_TargetX_6779F0 - gCurrCarAI_xpos_677C38);
                }
                else
                {
                    target_dist = Fix16::MaxAbsDistance_42A6B0(gCurrCarAI_xpos_677C38, gCurrCarAI_ypos_677C30, gCurrCarAI_TargetX_6779F0, gCurrCarAI_TargetY_6779F4);
                    target_angle = Fix16::atan2_fixed_405320(gCurrCarAI_TargetY_6779F4 - gCurrCarAI_ypos_677C30, gCurrCarAI_TargetX_6779F0 - gCurrCarAI_xpos_677C38);
                }
                break;

            case car_chase_state::follow_behind_2:
                if (gCurrCarAI_TargetCar_6779B0 != 0)
                {
                    field_0_car->field_50_car_sprite->set_xyz_lazy_420600(gCurrCarAI_TargetX_6779F0, gCurrCarAI_TargetY_6779F4, gCurrCarAI_zpos_677C48);
                    Ang16::PolarToCartesian_41FC20(gCurrCarAI_TargetCar_6779B0->field_50_car_sprite->field_0_rotation - kAng180_677ADE, kFpThree_677B9C, dx, dy);
                    gCurrCarAI_TargetX_6779F0 += dx;
                    gCurrCarAI_TargetY_6779F4 += dy;
                    Ang16::PolarToCartesian_41FC20(gCurrCarAI_TargetCar_6779B0->field_50_car_sprite->field_0_rotation + kAng90_6779E4, kFpHalf_677A84, dx, dy);
                    gCurrCarAI_TargetX_6779F0 += dx;
                    gCurrCarAI_TargetY_6779F4 += dy;
                    target_dist = Fix16::MaxAbsDistance_42A6B0(gCurrCarAI_xpos_677C38, gCurrCarAI_ypos_677C30, gCurrCarAI_TargetX_6779F0, gCurrCarAI_TargetY_6779F4);
                    target_angle = Fix16::atan2_fixed_405320(gCurrCarAI_TargetY_6779F4 - gCurrCarAI_ypos_677C30, gCurrCarAI_TargetX_6779F0 - gCurrCarAI_xpos_677C38);
                }
                break;

            case car_chase_state::unused_1:
                if (gCurrCarAI_TargetCar_6779B0 != 0)
                {
                    field_0_car->field_50_car_sprite->set_xyz_lazy_420600(gCurrCarAI_TargetX_6779F0, gCurrCarAI_TargetY_6779F4, gCurrCarAI_zpos_677C48);
                    Ang16::PolarToCartesian_41FC20(gCurrCarAI_TargetCar_6779B0->field_50_car_sprite->field_0_rotation - kAng180_677ADE, kFpThree_677B9C, dx, dy);
                    gCurrCarAI_TargetX_6779F0 += dx;
                    gCurrCarAI_TargetY_6779F4 += dy;
                    target_dist = Fix16::MaxAbsDistance_42A6B0(gCurrCarAI_xpos_677C38, gCurrCarAI_ypos_677C30, gCurrCarAI_TargetX_6779F0, gCurrCarAI_TargetY_6779F4);
                    target_angle = Fix16::atan2_fixed_405320(gCurrCarAI_TargetY_6779F4 - gCurrCarAI_ypos_677C30, gCurrCarAI_TargetX_6779F0 - gCurrCarAI_xpos_677C38);
                }
                break;

            case car_chase_state::move_left_3:
            {
                assert(gCurrCarAI_TargetCar_6779B0 != 0);
                field_0_car->field_50_car_sprite->set_xyz_lazy_420600(gCurrCarAI_TargetX_6779F0, gCurrCarAI_TargetY_6779F4, gCurrCarAI_zpos_677C48);
                Ang16::PolarToCartesian_41FC20(gCurrCarAI_TargetCar_6779B0->field_50_car_sprite->field_0_rotation - kAng90_6779E4, gF16fOne_677B94, dx, dy);
                gCurrCarAI_TargetX_6779F0 += dx;
                gCurrCarAI_TargetY_6779F4 += dy;
                Ang16::PolarToCartesian_41FC20(gCurrCarAI_TargetCar_6779B0->field_50_car_sprite->field_0_rotation, kFpHalf_677A84, dx, dy);
                gCurrCarAI_TargetX_6779F0 += dx;
                gCurrCarAI_TargetY_6779F4 += dy;
                target_dist = Fix16::MaxAbsDistance_42A6B0(gCurrCarAI_xpos_677C38, gCurrCarAI_ypos_677C30, gCurrCarAI_TargetX_6779F0, gCurrCarAI_TargetY_6779F4);
                target_angle = Fix16::atan2_fixed_405320(gCurrCarAI_TargetY_6779F4 - gCurrCarAI_ypos_677C30, gCurrCarAI_TargetX_6779F0 - gCurrCarAI_xpos_677C38);
                break;
            }

            case car_chase_state::choose_side_5:
            {
                assert(gCurrCarAI_TargetCar_6779B0 != 0);
                if (police_level == 1)
                {
                    field_0_car->field_50_car_sprite->set_xyz_lazy_420600(gCurrCarAI_TargetX_6779F0, gCurrCarAI_TargetY_6779F4, gCurrCarAI_zpos_677C48);
                    Ang16::PolarToCartesian_41FC20(gCurrCarAI_TargetCar_6779B0->field_50_car_sprite->field_0_rotation - kAng180_677ADE, gF16fOne_677B94, dx, dy);
                    gCurrCarAI_TargetX_6779F0 += dx;
                    gCurrCarAI_TargetY_6779F4 += dy;
                }
                target_dist = Fix16::MaxAbsDistance_42A6B0(gCurrCarAI_xpos_677C38, gCurrCarAI_ypos_677C30, gCurrCarAI_TargetX_6779F0, gCurrCarAI_TargetY_6779F4);
                target_angle = Fix16::atan2_fixed_405320(gCurrCarAI_TargetY_6779F4 - gCurrCarAI_ypos_677C30, gCurrCarAI_TargetX_6779F0 - gCurrCarAI_xpos_677C38);
                break;
            }

            case car_chase_state::move_right_4:
            {
                assert(gCurrCarAI_TargetCar_6779B0 != 0);
                field_0_car->field_50_car_sprite->set_xyz_lazy_420600(gCurrCarAI_TargetX_6779F0, gCurrCarAI_TargetY_6779F4, gCurrCarAI_zpos_677C48);
                Ang16::PolarToCartesian_41FC20(gCurrCarAI_TargetCar_6779B0->field_50_car_sprite->field_0_rotation + kAng90_6779E4, gF16fOne_677B94, dx, dy);
                gCurrCarAI_TargetX_6779F0 += dx;
                gCurrCarAI_TargetY_6779F4 += dy;
                Ang16::PolarToCartesian_41FC20(gCurrCarAI_TargetCar_6779B0->field_50_car_sprite->field_0_rotation, kFpHalf_677A84, dx, dy);
                gCurrCarAI_TargetX_6779F0 += dx;
                gCurrCarAI_TargetY_6779F4 += dy;
                target_dist = Fix16::MaxAbsDistance_42A6B0(gCurrCarAI_xpos_677C38, gCurrCarAI_ypos_677C30, gCurrCarAI_TargetX_6779F0, gCurrCarAI_TargetY_6779F4);
                target_angle = Fix16::atan2_fixed_405320(gCurrCarAI_TargetY_6779F4 - gCurrCarAI_ypos_677C30, gCurrCarAI_TargetX_6779F0 - gCurrCarAI_xpos_677C38);
                break;
            }

            case car_chase_state::approach_left_6:
            {
                assert(gCurrCarAI_TargetCar_6779B0 != 0);
                MakeAgressiveSirensAndLights_4476F0();
                field_0_car->field_50_car_sprite->set_xyz_lazy_420600(gCurrCarAI_TargetX_6779F0, gCurrCarAI_TargetY_6779F4, gCurrCarAI_zpos_677C48);
                Ang16::PolarToCartesian_41FC20(gCurrCarAI_TargetCar_6779B0->field_50_car_sprite->field_0_rotation - kAng90_6779E4, kFpThreeQuarters_677A4C, dx, dy);
                gCurrCarAI_TargetX_6779F0 += dx;
                gCurrCarAI_TargetY_6779F4 += dy;
                target_dist = Fix16::MaxAbsDistance_42A6B0(gCurrCarAI_xpos_677C38, gCurrCarAI_ypos_677C30, gCurrCarAI_TargetX_6779F0, gCurrCarAI_TargetY_6779F4);
                target_angle = Fix16::atan2_fixed_405320(gCurrCarAI_TargetY_6779F4 - gCurrCarAI_ypos_677C30, gCurrCarAI_TargetX_6779F0 - gCurrCarAI_xpos_677C38);
                break;
            }

            case car_chase_state::alongside_left_8:
            {
                bOvertake = 0;
                MakeAgressiveSirensAndLights_4476F0();
                switch (pChase->field_8_task_type)
                {
                    case 3:
                        bOvertake = 1;
                        break;

                    case car_task_type::follow_target_ped_4:
                        assert(gCurrCarAI_TargetCar_6779B0 != 0);
                        if (this->field_0_car->field_84_car_info_idx != car_model_enum::EDSELFBI)
                        {
                            speed_limit = kFpNine_677BB8;
                        }

                        if (gCurrCarAI_TargetCar_6779B0->GetMaxSpeed_439F30().mValue > speed_limit.mValue)
                        {
                            // Spelling required by VC6's CSE numbering, like the one below (docs/match_attempts.md)
                            if (3u < pChase->field_3C_block_counter)
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
                            switch (gCarChaseTaskTable_678E30->CountPursuers_474920(pChase->field_30_pTargetPed, field_0_car->field_54_driver))
                            {
                                case 1:
                                    // Spelling required by VC6's CSE numbering (docs/match_attempts.md)
                                    waitCount = gPolice_7B8_6FEE40->field_654_max_wanted_level - 1 != 0 ? 10 : 50;
                                    break;

                                case 2:
                                    if (gCarChaseTaskTable_678E30->HasPursuerChoosingSide_474C30(this->field_0_car->field_54_driver) != 0)
                                    {
                                        waitCount = 0;
                                    }
                                    else
                                    {
                                        waitCount = 1;
                                        pChase->field_2E_wait_counter = 0;
                                    }
                                    break;

                                case 3:
                                case 4:
                                    if (gCarChaseTaskTable_678E30->HasPursuerAlongsideRight_474AF0(this->field_0_car->field_54_driver) != 0)
                                    {
                                        waitCount = 0;
                                    }
                                    else
                                    {
                                        waitCount = 1;
                                        pChase->field_2E_wait_counter = 0;
                                    }
                                    break;

                                default:
                                    waitCount = 10;
                                    break;
                            }
                        }
                        break;

                    case car_task_type::kill_ped_5:
                        assert(gCurrCarAI_TargetCar_6779B0 != 0);
                        if (gCurrCarAI_TargetCar_6779B0->GetMaxSpeed_439F30() > speed_limit)
                        {
                            bOvertake = 1;
                        }
                        break;
                }

                if (static_cast<u16>(pChase->field_2E_wait_counter) < waitCount)
                {
                    ++pChase->field_2E_wait_counter;
                    field_0_car->field_50_car_sprite->set_xyz_lazy_420600(gCurrCarAI_TargetX_6779F0, gCurrCarAI_TargetY_6779F4, gCurrCarAI_zpos_677C48);
                    Ang16::PolarToCartesian_41FC20(gCurrCarAI_TargetCar_6779B0->field_50_car_sprite->field_0_rotation - kAng90_6779E4, kFpThreeQuarters_677A4C, dx, dy);
                    gCurrCarAI_TargetX_6779F0 += dx;
                    gCurrCarAI_TargetY_6779F4 += dy;
                    Ang16::PolarToCartesian_41FC20(gCurrCarAI_TargetCar_6779B0->field_50_car_sprite->field_0_rotation, kFpHalf_677A84, dx, dy);
                    gCurrCarAI_TargetX_6779F0 += dx;
                    gCurrCarAI_TargetY_6779F4 += dy;
                    target_dist = Fix16::MaxAbsDistance_42A6B0(gCurrCarAI_xpos_677C38, gCurrCarAI_ypos_677C30, gCurrCarAI_TargetX_6779F0, gCurrCarAI_TargetY_6779F4);
                    target_angle = Fix16::atan2_fixed_405320(gCurrCarAI_TargetY_6779F4 - gCurrCarAI_ypos_677C30, gCurrCarAI_TargetX_6779F0 - gCurrCarAI_xpos_677C38);
                }
                else
                {
                    if (bOvertake == 1)
                    {
                        pChase->field_10_ram_target = 1;
                        field_0_car->field_50_car_sprite->set_xyz_lazy_420600(gCurrCarAI_TargetX_6779F0, gCurrCarAI_TargetY_6779F4, gCurrCarAI_zpos_677C48);
                        Ang16::PolarToCartesian_41FC20(gCurrCarAI_TargetCar_6779B0->field_50_car_sprite->field_0_rotation, gF16fOne_677B94, dx, dy);
                        gCurrCarAI_TargetX_6779F0 += dx;
                        gCurrCarAI_TargetY_6779F4 += dy;
                        target_dist = Fix16::MaxAbsDistance_42A6B0(gCurrCarAI_xpos_677C38, gCurrCarAI_ypos_677C30, gCurrCarAI_TargetX_6779F0, gCurrCarAI_TargetY_6779F4);
                        target_angle = Fix16::atan2_fixed_405320(gCurrCarAI_TargetY_6779F4 - gCurrCarAI_ypos_677C30, gCurrCarAI_TargetX_6779F0 - gCurrCarAI_xpos_677C38);
                    }
                    else
                    {
                        pChase->field_C_chase_state = car_chase_state::pull_ahead_left_10;
                        return;
                    }
                }
                break;
            }

            case car_chase_state::approach_right_7:
            {
                assert(gCurrCarAI_TargetCar_6779B0 != 0);
                MakeAgressiveSirensAndLights_4476F0();
                field_0_car->field_50_car_sprite->set_xyz_lazy_420600(gCurrCarAI_TargetX_6779F0, gCurrCarAI_TargetY_6779F4, gCurrCarAI_zpos_677C48);
                Ang16::PolarToCartesian_41FC20(gCurrCarAI_TargetCar_6779B0->field_50_car_sprite->field_0_rotation + kAng90_6779E4, kFpThreeQuarters_677A4C, dx, dy);
                gCurrCarAI_TargetX_6779F0 += dx;
                gCurrCarAI_TargetY_6779F4 += dy;
                target_dist = Fix16::MaxAbsDistance_42A6B0(gCurrCarAI_xpos_677C38, gCurrCarAI_ypos_677C30, gCurrCarAI_TargetX_6779F0, gCurrCarAI_TargetY_6779F4);
                target_angle = Fix16::atan2_fixed_405320(gCurrCarAI_TargetY_6779F4 - gCurrCarAI_ypos_677C30, gCurrCarAI_TargetX_6779F0 - gCurrCarAI_xpos_677C38);
                break;
            }

            case car_chase_state::alongside_right_9:
            {
                bOvertake = 0;
                MakeAgressiveSirensAndLights_4476F0();
                switch (pChase->field_8_task_type)
                {
                    case 3:
                        bOvertake = 1;
                        break;

                    case car_task_type::follow_target_ped_4:
                        assert(gCurrCarAI_TargetCar_6779B0 != 0);
                        bOvertake = 0;
                        if (this->field_0_car->field_84_car_info_idx != car_model_enum::EDSELFBI)
                        {
                            speed_limit = kFpNine_677BB8;
                        }

                        if (gCurrCarAI_TargetCar_6779B0->GetMaxSpeed_439F30() > speed_limit)
                        {
                            if (pChase->field_3C_block_counter > 3u)
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
                            switch (gCarChaseTaskTable_678E30->CountPursuers_474920(pChase->field_30_pTargetPed, field_0_car->field_54_driver))
                            {
                                case 1:
                                    waitCount = gPolice_7B8_6FEE40->field_654_max_wanted_level != cop_level_enum::police_1 ? 10 : 50;
                                    break;

                                case 2:
                                    if (gCarChaseTaskTable_678E30->HasPursuerChoosingSide_474C30(this->field_0_car->field_54_driver) != 0)
                                    {
                                        waitCount = 0;
                                    }
                                    else
                                    {
                                        waitCount = 1;
                                        pChase->field_2E_wait_counter = 0;
                                    }
                                    break;

                                case 3:
                                case 4:
                                    if (gCarChaseTaskTable_678E30->HasPursuerAlongsideLeft_474A20(this->field_0_car->field_54_driver) != 0)
                                    {
                                        waitCount = 0;
                                    }
                                    else
                                    {
                                        waitCount = 1;
                                        pChase->field_2E_wait_counter = 0;
                                    }
                                    break;

                                default:
                                    waitCount = 10;
                                    break;
                            }
                        }
                        break;

                    case car_task_type::kill_ped_5:
                        assert(gCurrCarAI_TargetCar_6779B0 != 0);
                        if (gCurrCarAI_TargetCar_6779B0->GetMaxSpeed_439F30() > speed_limit)
                        {
                            bOvertake = 1;
                        }
                        break;
                }

                if (static_cast<u16>(pChase->field_2E_wait_counter) < waitCount)
                {
                    ++pChase->field_2E_wait_counter;
                    field_0_car->field_50_car_sprite->set_xyz_lazy_420600(gCurrCarAI_TargetX_6779F0, gCurrCarAI_TargetY_6779F4, gCurrCarAI_zpos_677C48);
                    Ang16::PolarToCartesian_41FC20(gCurrCarAI_TargetCar_6779B0->field_50_car_sprite->field_0_rotation + kAng90_6779E4, kFpThreeQuarters_677A4C, dx, dy);
                    gCurrCarAI_TargetX_6779F0 += dx;
                    gCurrCarAI_TargetY_6779F4 += dy;
                    Ang16::PolarToCartesian_41FC20(gCurrCarAI_TargetCar_6779B0->field_50_car_sprite->field_0_rotation, gF16fOne_677B94, dx, dy);
                    gCurrCarAI_TargetX_6779F0 += dx;
                    gCurrCarAI_TargetY_6779F4 += dy;
                    target_dist = Fix16::MaxAbsDistance_42A6B0(gCurrCarAI_xpos_677C38, gCurrCarAI_ypos_677C30, gCurrCarAI_TargetX_6779F0, gCurrCarAI_TargetY_6779F4);
                    target_angle = Fix16::atan2_fixed_405320(gCurrCarAI_TargetY_6779F4 - gCurrCarAI_ypos_677C30, gCurrCarAI_TargetX_6779F0 - gCurrCarAI_xpos_677C38);
                }
                else
                {
                    if (bOvertake == 1)
                    {
                        pChase->field_10_ram_target = 1;
                        field_0_car->field_50_car_sprite->set_xyz_lazy_420600(gCurrCarAI_TargetX_6779F0, gCurrCarAI_TargetY_6779F4, gCurrCarAI_zpos_677C48);
                        Ang16::PolarToCartesian_41FC20(gCurrCarAI_TargetCar_6779B0->field_50_car_sprite->field_0_rotation, gF16fOne_677B94, dx, dy);
                        gCurrCarAI_TargetX_6779F0 += dx;
                        gCurrCarAI_TargetY_6779F4 += dy;
                        target_dist = Fix16::MaxAbsDistance_42A6B0(gCurrCarAI_xpos_677C38, gCurrCarAI_ypos_677C30, gCurrCarAI_TargetX_6779F0, gCurrCarAI_TargetY_6779F4);
                        target_angle = Fix16::atan2_fixed_405320(gCurrCarAI_TargetY_6779F4 - gCurrCarAI_ypos_677C30, gCurrCarAI_TargetX_6779F0 - gCurrCarAI_xpos_677C38);
                    }
                    else
                    {
                        pChase->field_C_chase_state = car_chase_state::pull_ahead_right_11;
                        return;
                    }
                }
                break;
            }

            case car_chase_state::pull_ahead_left_10:
            {
                assert(gCurrCarAI_TargetCar_6779B0 != 0);
                MakeAgressiveSirensAndLights_4476F0();
                field_0_car->field_50_car_sprite->set_xyz_lazy_420600(gCurrCarAI_TargetX_6779F0, gCurrCarAI_TargetY_6779F4, gCurrCarAI_zpos_677C48);
                Ang16::PolarToCartesian_41FC20(gCurrCarAI_TargetCar_6779B0->field_50_car_sprite->field_0_rotation - kAng90_6779E4, kFpThreeQuarters_677A4C, dx, dy);
                gCurrCarAI_TargetX_6779F0 += dx;
                gCurrCarAI_TargetY_6779F4 += dy;
                Ang16::PolarToCartesian_41FC20(gCurrCarAI_TargetCar_6779B0->field_50_car_sprite->field_0_rotation, gF16fOne_677B94 + kFpHalf_677A84, dx, dy);
                gCurrCarAI_TargetX_6779F0 += dx;
                gCurrCarAI_TargetY_6779F4 += dy;
                target_dist = Fix16::MaxAbsDistance_42A6B0(gCurrCarAI_xpos_677C38, gCurrCarAI_ypos_677C30, gCurrCarAI_TargetX_6779F0, gCurrCarAI_TargetY_6779F4);
                target_angle = Fix16::atan2_fixed_405320(gCurrCarAI_TargetY_6779F4 - gCurrCarAI_ypos_677C30, gCurrCarAI_TargetX_6779F0 - gCurrCarAI_xpos_677C38);
                break;
            }

            case car_chase_state::pull_ahead_right_11:
            {
                assert(gCurrCarAI_TargetCar_6779B0 != 0);
                MakeAgressiveSirensAndLights_4476F0();
                field_0_car->field_50_car_sprite->set_xyz_lazy_420600(gCurrCarAI_TargetX_6779F0, gCurrCarAI_TargetY_6779F4, gCurrCarAI_zpos_677C48);
                Ang16::PolarToCartesian_41FC20(gCurrCarAI_TargetCar_6779B0->field_50_car_sprite->field_0_rotation + kAng90_6779E4, kFpThreeQuarters_677A4C, dx, dy);
                gCurrCarAI_TargetX_6779F0 += dx;
                gCurrCarAI_TargetY_6779F4 += dy;
                Ang16::PolarToCartesian_41FC20(gCurrCarAI_TargetCar_6779B0->field_50_car_sprite->field_0_rotation, gF16fOne_677B94 + kFpHalf_677A84, dx, dy);
                gCurrCarAI_TargetX_6779F0 += dx;
                gCurrCarAI_TargetY_6779F4 += dy;
                target_dist = Fix16::MaxAbsDistance_42A6B0(gCurrCarAI_xpos_677C38, gCurrCarAI_ypos_677C30, gCurrCarAI_TargetX_6779F0, gCurrCarAI_TargetY_6779F4);
                target_angle = Fix16::atan2_fixed_405320(gCurrCarAI_TargetY_6779F4 - gCurrCarAI_ypos_677C30, gCurrCarAI_TargetX_6779F0 - gCurrCarAI_xpos_677C38);
                break;
            }

            case car_chase_state::in_front_12:
            case car_chase_state::in_front_stopping_13:
            {
                assert(gCurrCarAI_TargetCar_6779B0 != 0);
                MakeAgressiveSirensAndLights_4476F0();
                field_0_car->field_50_car_sprite->set_xyz_lazy_420600(gCurrCarAI_TargetX_6779F0, gCurrCarAI_TargetY_6779F4, gCurrCarAI_zpos_677C48);
                Ang16::PolarToCartesian_41FC20(gCurrCarAI_TargetCar_6779B0->field_50_car_sprite->field_0_rotation, kFpThree_677B9C, dx, dy);
                gCurrCarAI_TargetX_6779F0 += dx;
                gCurrCarAI_TargetY_6779F4 += dy;
                target_dist = Fix16::MaxAbsDistance_42A6B0(gCurrCarAI_xpos_677C38, gCurrCarAI_ypos_677C30, gCurrCarAI_TargetX_6779F0, gCurrCarAI_TargetY_6779F4);
                target_angle = Fix16::atan2_fixed_405320(gCurrCarAI_TargetY_6779F4 - gCurrCarAI_ypos_677C30, gCurrCarAI_TargetX_6779F0 - gCurrCarAI_xpos_677C38);
                if (pChase->field_C_chase_state == car_chase_state::in_front_stopping_13)
                {
                    // Face the un-offset target position
                    aim_angle = Fix16::atan2_fixed_405320(savedTargetY - gCurrCarAI_ypos_677C30, savedTargetX - gCurrCarAI_xpos_677C38);
                    if (gCurrCarAI_DesiredSpeed_677A8C.mValue < kFpPoint03_677B58.mValue)
                    {
                        field_0_car->DoBrake_43A950();
                        pChase->field_2C_side_counter = 0;
                        pChase->field_C_chase_state = car_chase_state::target_stopped_15;
                        return;
                    }

                    if (Ang16::IsAngleAhead_405C60(&this->field_10_angle, &aim_angle) != 0)
                    {
                        this->field_0_car->field_58_physics->TurnAntiClockwise_42AB90();
                    }
                    else
                    {
                        this->field_0_car->field_58_physics->TurnClockwise_42ABA0();
                    }

                    heading_delta = ComputeShortestAngleDelta_4056C0(this->field_10_angle, gCurrCarAI_TargetCar_6779B0->field_50_car_sprite->field_0_rotation);
                    if (heading_delta.rValue > kAng12_677A2E.rValue)
                    {
                        pChase->field_C_chase_state = car_chase_state::target_stopped_15;
                        return;
                    }
                    this->field_0_car->field_58_physics->SetHandBrakeOn_421250();
                    return;
                }
                break;
            }

            case car_chase_state::target_stopped_15:
                this->field_24_bf.bAIHalted = true;
                field_0_car->SetAIHalted_421540();
                break;

            default:
                break;
        }
    }

    ComputeShortestAngleDelta_4056C0(this->field_10_angle, target_angle);
    heading_copy = this->field_10_angle;
    aim_angle = target_angle;
    s32 face = Ang16::GetAngleFace_4F78F0(aim_angle);
    if (this->field_4C_curr_direction != face)
    {
        if (target_dist.mValue < kFpFour_677BA0.mValue && !this->field_3C_seeking_road)
        {
            if (this->field_4C_curr_direction == GetOppositeFace_42A660(face) && target_dist.mValue < kFpThree_677B9C.mValue)
            {
                assert(pPhysics != 0);
                pPhysics = this->field_0_car->field_58_physics;
                pPhysics->field_91_is_foot_brake_on = 0;
                pPhysics->field_93_is_forward_gas_on = 0;
                pPhysics->field_94_is_backward_gas_on = 0;
                pPhysics->field_95_is_coasting = 0;
            }
        }

        if (Ang16::IsAngleAhead_405C60(&heading_copy, &aim_angle) != 0)
        {
            if (this->field_24_bf.bFlag6 != 0)
            {
                this->field_0_car->field_58_physics->TurnClockwise_42ABA0();
            }
            else
            {
                this->field_0_car->field_58_physics->TurnAntiClockwise_42AB90();
            }
        }
        else
        {
            if (this->field_24_bf.bFlag6 != 0)
            {
                this->field_0_car->field_58_physics->TurnAntiClockwise_42AB90();
            }
            else
            {
                this->field_0_car->field_58_physics->TurnClockwise_42ABA0();
            }
        }
    }
    else
    {
        if (this->field_4C_curr_direction == car_ai_direction::south_2)
        {
            heading_copy = this->field_10_angle + kAng90_6779E4;
            aim_angle = target_angle + kAng90_6779E4;
        }

        if (heading_copy < aim_angle - kAng4_677CE2 - kAng4_677CE2)
        {
            if (gRng_6F6784.get_int_4F7AE0(4) == 0)
            {
                if (this->field_24_bf.bFlag6 != 0)
                {
                    this->field_0_car->field_58_physics->TurnClockwise_42ABA0();
                }
                else
                {
                    this->field_0_car->field_58_physics->TurnAntiClockwise_42AB90();
                }
            }
        }
        else
        {
            if (heading_copy > aim_angle + kAng4_677CE2 + kAng4_677CE2)
            {
                if (gRng_6F6784.get_int_4F7AE0(4) == 0)
                {
                    if (this->field_24_bf.bFlag6 != 0)
                    {
                        this->field_0_car->field_58_physics->TurnAntiClockwise_42AB90();
                    }
                    else
                    {
                        this->field_0_car->field_58_physics->TurnClockwise_42ABA0();
                    }
                }
            }
            else
            {
                this->field_0_car->field_58_physics->SetGoStraight_42ABB0();
                byte_677A5C = 1;
            }
        }
        field_0_car->field_50_car_sprite->set_xyz_lazy_420600(gCurrCarAI_xpos_677C38, gCurrCarAI_ypos_677C30, gCurrCarAI_zpos_677C48);
    }

    gCurrCarAI_TargetX_6779F0 = savedTargetX;
    gCurrCarAI_TargetY_6779F4 = savedTargetY;

    if (this->field_3C_seeking_road == 0)
    {
        // Probe one unit ahead in the direction we are turning to
        field_0_car->field_50_car_sprite->set_xyz_lazy_420600(gCurrCarAI_xpos_677C38, gCurrCarAI_ypos_677C30, gCurrCarAI_zpos_677C48);
        Ang16::PolarToCartesian_41FC20(kAng45_677A38.MultiplyByFix16_401CB0(Fix16(this->field_0_car->field_58_physics->field_AD_turn_direction)) + this->field_10_angle,
                                       gF16fOne_677B94,
                                       dx,
                                       dy);
        Car* pCar = field_0_car;
        pCar->field_50_car_sprite->set_xyz_lazy_420600(pCar->field_50_car_sprite->field_14_xy.x + dx,
                                                              pCar->field_50_car_sprite->field_14_xy.y + dy,
                                                              pCar->field_50_car_sprite->field_1C_zpos);
        if (field_0_car->field_50_car_sprite->CheckSpriteMovementRegion_5A2500() != 0)
        {
            this->field_0_car->field_58_physics->SetGoStraight_42ABB0();
        }
    }

    if (gCurrCarAI_DesiredSpeed_677A8C.mValue < kFpPoint03_677B58.mValue)
    {
        field_0_car->DoBrake_43A950();
        pChase->field_2C_side_counter = 0;
        pChase->field_C_chase_state = car_chase_state::target_stopped_15;
        return;
    }

    if (target_dist < kFpTwo_677B98 && !this->field_3C_seeking_road)
    {
        if (gCurrCarAI_TargetCar_6779B0 == 0)
        {
            return;
        }

        gCurrCarAI_DesiredSpeed_677A8C = gCurrCarAI_TargetCar_6779B0->GetCarLinearSpeed_43A240();

        switch (pChase->field_C_chase_state)
        {
            case car_chase_state::follow_behind_0:
            case car_chase_state::follow_behind_2:
                bOvertake = 0;
                if (static_cast<u16>(pChase->field_2A_settle_counter) >= 10u)
                {
                    gCurrCarAI_TargetX_6779F0 = savedTargetX;
                    gCurrCarAI_TargetY_6779F4 = savedTargetY;
                    Ang16::PolarToCartesian_41FC20(gCurrCarAI_TargetCar_6779B0->field_50_car_sprite->field_0_rotation - kAng180_677ADE, kFpTwo_677B98, dx, dy);
                    gCurrCarAI_TargetX_6779F0 += dx;
                    gCurrCarAI_TargetY_6779F4 += dy;
                    field_0_car->field_50_car_sprite->set_xyz_lazy_420600(gCurrCarAI_TargetX_6779F0,
                                                                          gCurrCarAI_TargetY_6779F4,
                                                                          field_0_car->field_50_car_sprite->field_1C_zpos);

                    if (field_0_car->field_50_car_sprite->CheckSpriteMovementRegion_5A2500() == 0)
                    {
                        Ped* pOwner = gCarChaseTaskTable_678E30->FindDriverInState_4748A0(car_chase_state::choose_side_5, pChase->field_30_pTargetPed);
                        if (pOwner != 0)
                        {
                            if (pOwner->field_16C_car->field_60_pChaseTask->field_2C_side_counter > 10u)
                            {
                                pOwner->field_16C_car->field_60_pChaseTask->field_C_chase_state = car_chase_state::move_left_3;
                                pOwner->field_16C_car->field_60_pChaseTask->field_2C_side_counter = 0;
                                pChase->field_C_chase_state = car_chase_state::choose_side_5;
                                pChase->field_2A_settle_counter = 100;
                                pChase->field_2C_side_counter = 0;
                                bOvertake = 1;
                            }
                        }
                        else
                        {
                            pChase->field_C_chase_state = car_chase_state::choose_side_5;
                            pChase->field_2A_settle_counter = 100;
                            pChase->field_2C_side_counter = 0;
                            bOvertake = 1;
                        }
                    }
                }
                if (bOvertake)
                {
                    return;
                }

                gCurrCarAI_DesiredSpeed_677A8C = gCurrCarAI_TargetCar_6779B0->GetCarLinearSpeed_43A240();
                if (target_dist < gF16fOne_677B94)
                {
                    pPhysics = this->field_0_car->field_58_physics;
                    pPhysics->field_91_is_foot_brake_on = 0;
                    pPhysics->field_93_is_forward_gas_on = 0;
                    pPhysics->field_94_is_backward_gas_on = 0;
                    pPhysics->field_95_is_coasting = 0;
                }
                else if (gCurrCarAI_Velocity_677B00 < gCurrCarAI_DesiredSpeed_677A8C + kFpPoint02_677B5C)
                {
                    pPhysics = this->field_0_car->field_58_physics;
                    pPhysics->field_93_is_forward_gas_on = 1;
                    pPhysics->field_91_is_foot_brake_on = 0;
                    pPhysics->field_94_is_backward_gas_on = 0;
                    pPhysics->field_95_is_coasting = 0;
                }
                else
                {
                    pPhysics = this->field_0_car->field_58_physics;
                    pPhysics->field_95_is_coasting = 1;
                    pPhysics->field_91_is_foot_brake_on = 0;
                    pPhysics->field_94_is_backward_gas_on = 0;
                    pPhysics->field_93_is_forward_gas_on = 0;
                }

                if (static_cast<u16>(pChase->field_2A_settle_counter) < 10u)
                {
                    pChase->field_2A_settle_counter++;
                }
                break;

            case car_chase_state::move_left_3:
                if (pChase->field_2C_side_counter == 10)
                {
                    field_0_car->field_50_car_sprite->set_xyz_lazy_420600(gCurrCarAI_TargetX_6779F0, gCurrCarAI_TargetY_6779F4, gCurrCarAI_zpos_677C48);
                    Ang16::PolarToCartesian_41FC20(gCurrCarAI_TargetCar_6779B0->field_50_car_sprite->field_0_rotation - kAng90_6779E4, gF16fOne_677B94, dx, dy);
                    assert(pSprite != 0);
                    pSprite = field_0_car->field_50_car_sprite;
                    field_0_car->field_50_car_sprite->set_xyz_lazy_420600(pSprite->field_14_xy.x + dx, pSprite->field_14_xy.y + dy, pSprite->field_1C_zpos);

                    if (field_0_car->field_50_car_sprite->CheckSpriteMovementRegion_5A2500() == 0)
                    {
                        this->field_70_nearest_entity = gSpriteGrid_1_679208->FindCollidingSpritePreferType_477E60(this->field_0_car->field_50_car_sprite, 0);
                        if (this->field_70_nearest_entity == 0)
                        {
                            pChase->field_C_chase_state = car_chase_state::approach_left_6;
                        }
                    }
                }
                else
                {
                    gCurrCarAI_DesiredSpeed_677A8C = gCurrCarAI_TargetCar_6779B0->GetCarLinearSpeed_43A240();
                    if (target_dist < gF16fOne_677B94)
                    {
                        assert(pPhysics != 0);
                        pPhysics = this->field_0_car->field_58_physics;
                        pPhysics->field_91_is_foot_brake_on = 0;
                        pPhysics->field_93_is_forward_gas_on = 0;
                        pPhysics->field_94_is_backward_gas_on = 0;
                        pPhysics->field_95_is_coasting = 0;
                    }
                    else if (gCurrCarAI_Velocity_677B00 < gCurrCarAI_DesiredSpeed_677A8C)
                    {
                        assert(pPhysics != 0);
                        pPhysics = this->field_0_car->field_58_physics;
                        pPhysics->field_93_is_forward_gas_on = 1;
                        pPhysics->field_91_is_foot_brake_on = 0;
                        pPhysics->field_94_is_backward_gas_on = 0;
                        pPhysics->field_95_is_coasting = 0;
                    }
                    else
                    {
                        assert(pPhysics != 0);
                        pPhysics = this->field_0_car->field_58_physics;
                        pPhysics->field_95_is_coasting = 1;
                        pPhysics->field_91_is_foot_brake_on = 0;
                        pPhysics->field_94_is_backward_gas_on = 0;
                        pPhysics->field_93_is_forward_gas_on = 0;
                    }

                    if (pChase->field_2C_side_counter < 10u)
                    {
                        pChase->field_2C_side_counter++;
                    }
                }
                break;

            case car_chase_state::choose_side_5:
            {
                switch (gCarChaseTaskTable_678E30->CountPursuers_474920(pChase->field_30_pTargetPed, this->field_0_car->field_54_driver))
                {
                    case 1:
                        bTryRight = 1;
                        bTryLeft = 1;
                        maxCount = gPolice_7B8_6FEE40->field_654_max_wanted_level != cop_level_enum::police_1 ? 80 : 400;
                        break;

                    case 2:
                        if (gCarChaseTaskTable_678E30->HasPursuerInFront_4749B0(this->field_0_car->field_54_driver) != 0)
                        {
                            maxCount = 10;
                            pChase->field_2C_side_counter = 0;
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
                        if (gCarChaseTaskTable_678E30->HasPursuerAlongside_474BC0(field_0_car->field_54_driver) != 0)
                        {
                            if (gCarChaseTaskTable_678E30->HasPursuerOnLeft_474A80(this->field_0_car->field_54_driver) != 0)
                            {
                                bTryLeft = 0;
                                if (gCarChaseTaskTable_678E30->HasPursuerOnRight_474B50(field_0_car->field_54_driver) != 0)
                                {
                                    pChase->field_2C_side_counter = 0;
                                    bTryRight = 0;
                                    bTryLeft = 0;
                                }
                            }
                            else if (gCarChaseTaskTable_678E30->HasPursuerOnRight_474B50(this->field_0_car->field_54_driver) != 0)
                            {
                                bTryRight = 0;
                            }
                            else
                            {
                                maxCount = 10;
                                pChase->field_2C_side_counter = 0;
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

                if (pChase->field_8_task_type == car_task_type::follow_ped_2)
                {
                    maxCount = 100;
                    pChase->field_2C_side_counter = 0;
                }

                MakeAgressiveSirensAndLights_4476F0();

                if (pChase->field_2C_side_counter >= maxCount)
                {
                    // Try to get beside the target, first on its left then on its right
                    if (bTryLeft != 0)
                    {
                        field_0_car->field_50_car_sprite->set_xyz_lazy_420600(gCurrCarAI_TargetX_6779F0, gCurrCarAI_TargetY_6779F4, gCurrCarAI_zpos_677C48);
                        Ang16::PolarToCartesian_41FC20(gCurrCarAI_TargetCar_6779B0->field_50_car_sprite->field_0_rotation - kAng90_6779E4, gF16fOne_677B94, dx, dy);
                        assert(pSprite != 0);
                        pSprite = field_0_car->field_50_car_sprite;
                        field_0_car->field_50_car_sprite->set_xyz_lazy_420600(pSprite->field_14_xy.x + dx, pSprite->field_14_xy.y + dy, pSprite->field_1C_zpos);

                        if (field_0_car->field_50_car_sprite->CheckSpriteMovementRegion_5A2500() == 0)
                        {
                            if ((field_70_nearest_entity = gSpriteGrid_1_679208->FindCollidingSpritePreferType_477E60(this->field_0_car->field_50_car_sprite, 0)) == 0)
                            {
                                Ang16::PolarToCartesian_41FC20(field_0_car->field_50_car_sprite->field_0_rotation, gF16fOne_677B94, dx, dy);
                                assert(pSprite != 0);
                                pSprite = field_0_car->field_50_car_sprite;
                                field_0_car->field_50_car_sprite->set_xyz_lazy_420600(pSprite->field_14_xy.x + dx, pSprite->field_14_xy.y + dy, pSprite->field_1C_zpos);
                                if ((field_70_nearest_entity = gSpriteGrid_1_679208->FindCollidingSpritePreferType_477E60(this->field_0_car->field_50_car_sprite, 0)) == 0)
                                {
                                    pChase->field_C_chase_state = car_chase_state::approach_left_6;
                                    return;
                                }
                            }
                        }
                    }

                    if (bTryRight != 0)
                    {
                        field_0_car->field_50_car_sprite->set_xyz_lazy_420600(gCurrCarAI_TargetX_6779F0, gCurrCarAI_TargetY_6779F4, gCurrCarAI_zpos_677C48);
                        Ang16::PolarToCartesian_41FC20(gCurrCarAI_TargetCar_6779B0->field_50_car_sprite->field_0_rotation + kAng90_6779E4, gF16fOne_677B94, dx, dy);
                        assert(pSprite != 0);
                        pSprite = field_0_car->field_50_car_sprite;
                        field_0_car->field_50_car_sprite->set_xyz_lazy_420600(pSprite->field_14_xy.x + dx, pSprite->field_14_xy.y + dy, pSprite->field_1C_zpos);

                        if (field_0_car->field_50_car_sprite->CheckSpriteMovementRegion_5A2500() == 0)
                        {
                            if ((field_70_nearest_entity = gSpriteGrid_1_679208->FindCollidingSpritePreferType_477E60(this->field_0_car->field_50_car_sprite, 0)) == 0)
                            {
                                Ang16::PolarToCartesian_41FC20(field_0_car->field_50_car_sprite->field_0_rotation, gF16fOne_677B94, dx, dy);
                                assert(pSprite != 0);
                                pSprite = field_0_car->field_50_car_sprite;
                                field_0_car->field_50_car_sprite->set_xyz_lazy_420600(pSprite->field_14_xy.x + dx, pSprite->field_14_xy.y + dy, pSprite->field_1C_zpos);
                                if ((field_70_nearest_entity = gSpriteGrid_1_679208->FindCollidingSpritePreferType_477E60(this->field_0_car->field_50_car_sprite, 0)) == 0)
                                {
                                    pChase->field_C_chase_state = car_chase_state::approach_right_7;
                                }
                            }
                        }
                    }
                }
                else
                {
                    gCurrCarAI_DesiredSpeed_677A8C = gCurrCarAI_TargetCar_6779B0->GetCarLinearSpeed_43A240();
                    if (this->field_74_max_speed < gCurrCarAI_DesiredSpeed_677A8C)
                    {
                        gCurrCarAI_DesiredSpeed_677A8C = this->field_74_max_speed;
                    }

                    if (target_dist < gF16fOne_677B94)
                    {
                        if (gCurrCarAI_Velocity_677B00 > gCurrCarAI_DesiredSpeed_677A8C + kFpPoint02_677B5C)
                        {
                            field_0_car->DoBrake_43A950();
                            return;
                        }
                        pPhysics = this->field_0_car->field_58_physics;
                        pPhysics->field_91_is_foot_brake_on = 0;
                        pPhysics->field_93_is_forward_gas_on = 0;
                        pPhysics->field_94_is_backward_gas_on = 0;
                        pPhysics->field_95_is_coasting = 0;
                        return;
                    }

                    if (gCurrCarAI_Velocity_677B00 < gCurrCarAI_DesiredSpeed_677A8C)
                    {
                        pPhysics = this->field_0_car->field_58_physics;
                        pPhysics->field_93_is_forward_gas_on = 1;
                        pPhysics->field_91_is_foot_brake_on = 0;
                        pPhysics->field_94_is_backward_gas_on = 0;
                        pPhysics->field_95_is_coasting = 0;
                    }
                    else if (gCurrCarAI_Velocity_677B00 > gCurrCarAI_DesiredSpeed_677A8C + kFpPoint02_677B5C)
                    {
                        field_0_car->DoBrake_43A950();
                    }
                    else
                    {
                        field_0_car->field_58_physics->ForceNeutralInput_42ABC0();
                    }

                    if (pChase->field_2C_side_counter < maxCount)
                    {
                        ++pChase->field_2C_side_counter;
                    }
                    pChase->field_2A_settle_counter = 100;
                }
                break;
            }

            case car_chase_state::move_right_4:
                assert(gCurrCarAI_TargetCar_6779B0 != 0);
                if (pChase->field_2C_side_counter >= 10u)
                {
                    field_0_car->field_50_car_sprite->set_xyz_lazy_420600(gCurrCarAI_TargetX_6779F0, gCurrCarAI_TargetY_6779F4, gCurrCarAI_zpos_677C48);
                    Ang16::PolarToCartesian_41FC20(gCurrCarAI_TargetCar_6779B0->field_50_car_sprite->field_0_rotation + kAng90_6779E4, gF16fOne_677B94, dx, dy);
                    field_0_car->field_50_car_sprite->set_xyz_lazy_420600(field_0_car->field_50_car_sprite->field_14_xy.x + dx, field_0_car->field_50_car_sprite->field_14_xy.y + dy, field_0_car->field_50_car_sprite->field_1C_zpos);
                    if (field_0_car->field_50_car_sprite->CheckSpriteMovementRegion_5A2500() == 0)
                    {
                        if ((this->field_70_nearest_entity = gSpriteGrid_1_679208->FindCollidingSpritePreferType_477E60(this->field_0_car->field_50_car_sprite, 0)) == 0)
                        {
                            pChase->field_C_chase_state = car_chase_state::approach_right_7;
                        }
                    }
                }
                else
                {
                    gCurrCarAI_DesiredSpeed_677A8C = gCurrCarAI_TargetCar_6779B0->GetCarLinearSpeed_43A240();
                    if (target_dist < gF16fOne_677B94)
                    {
                        field_0_car->field_58_physics->NeutralGear_42AC00();
                    }
                    else if (gCurrCarAI_Velocity_677B00 < gCurrCarAI_DesiredSpeed_677A8C)
                    {
                        field_0_car->field_58_physics->Accelerate_421210();
                    }
                    else
                    {
                        field_0_car->field_58_physics->ForceNeutralInput_42ABC0();
                    }

                    if (pChase->field_2C_side_counter < 10u)
                    {
                        pChase->field_2C_side_counter++;
                    }
                }
                break;

            case car_chase_state::approach_left_6:
                if (target_dist < kFpHalf_677A84 + kFpEighth_677CBC)
                {
                    pChase->field_C_chase_state = car_chase_state::alongside_left_8;
                }
                else
                {
                    field_0_car->field_58_physics->Accelerate_421210();
                }
                break;

            case car_chase_state::approach_right_7:
                if (target_dist < kFpHalf_677A84 + kFpEighth_677CBC)
                {
                    pChase->field_C_chase_state = car_chase_state::alongside_right_9;
                }
                else
                {
                    field_0_car->field_58_physics->Accelerate_421210();
                }
                break;

            case car_chase_state::alongside_left_8:
            case car_chase_state::alongside_right_9:
                assert(gCurrCarAI_TargetCar_6779B0 != 0);
                gCurrCarAI_DesiredSpeed_677A8C = gCurrCarAI_TargetCar_6779B0->GetCarLinearSpeed_43A240();
                if (target_dist < kFpQuarter_6779BC)
                {
                    if (gCurrCarAI_Velocity_677B00 > gCurrCarAI_DesiredSpeed_677A8C)
                    {
                        field_0_car->DoBrake_43A950();
                    }
                    else
                    {
                        field_0_car->field_58_physics->NeutralGear_42AC00();
                    }
                }
                else if (target_dist > kFpThreeQuarters_677A4C)
                {
                    field_0_car->field_58_physics->Accelerate_421210();
                }
                else if (gCurrCarAI_Velocity_677B00 < gCurrCarAI_DesiredSpeed_677A8C)
                {
                    field_0_car->field_58_physics->Accelerate_421210();
                }
                else if (gCurrCarAI_Velocity_677B00 > gCurrCarAI_DesiredSpeed_677A8C + kFpPoint02_677B5C)
                {
                    field_0_car->field_58_physics->NeutralGear_42AC00();
                }
                else
                {
                    field_0_car->field_58_physics->ForceNeutralInput_42ABC0();
                }
                break;

            case car_chase_state::pull_ahead_left_10:
            case car_chase_state::pull_ahead_right_11:
                if (target_dist < kFpQuarter_6779BC)
                {
                    pChase->field_C_chase_state = car_chase_state::in_front_12;
                }
                break;

            case car_chase_state::in_front_12:
            case car_chase_state::in_front_stopping_13:
                if (target_dist < gF16fOne_677B94 + kFpHalf_677A84)
                {
                    ++pChase->field_3C_block_counter;
                    field_0_car->field_58_physics->NeutralGear_42AC00();
                    pChase->field_C_chase_state = car_chase_state::in_front_stopping_13;
                }
                break;

            default:
                break;
        }
    }
    else
    {
        if (this->field_0_car->field_60_pChaseTask != 0)
        {
            if (static_cast<u16>(pChase->field_2A_settle_counter) > 0)
            {
                pChase->field_2A_settle_counter--;
            }
            else if (pChase->field_8_task_type != car_task_type::goto_position_1)
            {
                pChase->field_C_chase_state = car_chase_state::in_front_stopping_13;
                if (gCarChaseTaskTable_678E30->FindDriverInState_4748A0(car_chase_state::follow_behind_0, pChase->field_30_pTargetPed) != 0)
                {
                    pChase->field_C_chase_state = car_chase_state::follow_behind_2;
                }
                else
                {
                    pChase->field_C_chase_state = gCarChaseTaskTable_678E30->FindDriverInState_4748A0(car_chase_state::choose_side_5, pChase->field_30_pTargetPed) ? 0 : 2;
                }
                pChase->field_2E_wait_counter = 0;
            }
        }
    }
}

MATCH_FUNC(0x451980)
void CarAI::ReactToNearbyCar_451980()
{
    Fix16 near_dist = gF16fOne_677B94;
    bool flag1 = false;
    u8 bUnknown = 0;
    Car* cBC = field_70_nearest_entity->field_8_car_bc_ptr;

    if (field_8_maneuver_active)
    {
        if (ComputeShortestAngleDelta_4056C0(cBC->field_50_car_sprite->field_0_rotation, field_0_car->field_50_car_sprite->field_0_rotation) > kAng90_6779E4)
        {
            flag1 = true;
        }
    }

    if (cBC)
    {
        if (cBC->field_54_driver)
        {
            if (cBC->field_54_driver->field_28C_threat_reaction == threat_reaction_enum::react_as_emergency_1)
            {
                bUnknown = 1;
            }
        }
    }

    Fix16 dist_to_target = MaxAbsDistanceRawY_451980(gCurrCarAI_xpos_677C38,
                                         gCurrCarAI_ypos_677C30,
                                         cBC->field_50_car_sprite->field_14_xy.x,
                                         cBC->field_50_car_sprite->field_14_xy.y);

    if (dist_to_target > kFpTwo_677B98)
    {
        if (gCurrCarAI_IsChasing_677BBC || field_24_bf.bFlag21)
        {
            flag1 = 1;
        }
        if (cBC->field_7C_ped_type == ped_type::player_2)
        {
            DetectCarAhead_44D1D0();
        }
        else if (ComputeShortestAngleDelta_4056C0(cBC->field_50_car_sprite->field_0_rotation, field_0_car->field_50_car_sprite->field_0_rotation) > kAng45_677A38)
        {
            flag1 = 1;
        }
        if (bUnknown)
        {
            flag1 = 0;
        }
    }
    else
    {
        if (dist_to_target > near_dist)
        {
            flag1 = 1;
            if (ComputeShortestAngleDelta_4056C0(cBC->field_50_car_sprite->field_0_rotation, field_0_car->field_50_car_sprite->field_0_rotation) > kAng90_6779E4)
            {
                if (field_24_bf.bFlag21)
                {
                    flag1 = 1;
                }
                else
                {
                    byte_677B3C = 0;
                    field_0_car->field_58_physics->NeutralGear_42AC00();
                }
                field_0_car->TryHonkHorn_4416D0(2);
            }
            else if (field_24_bf.bFlag17)
            {
                byte_677B3C = 0;
                field_0_car->DoBrake_43A950();
            }
            else if (gCurrCarAI_IsChasing_677BBC && gCurrCarAI_TargetCar_6779B0)
            {
                if (gCurrCarAI_TargetCar_6779B0 == cBC)
                {
                    gCurrCarAI_DesiredSpeed_677A8C = cBC->GetCarLinearSpeed_43A240();

                    if (field_0_car->field_60_pChaseTask->field_8_task_type == car_task_type::follow_ped_2)
                    {
                        if (gCurrCarAI_DesiredSpeed_677A8C == kF16Zero_677B90)
                        {
                            field_0_car->DoBrake_43A950();
                        }
                        else if (gCurrCarAI_Velocity_677B00 >= gCurrCarAI_DesiredSpeed_677A8C)
                        {
                            field_0_car->field_58_physics->ForceNeutralInput_42ABC0();
                        }
                        else
                        {
                            field_0_car->field_58_physics->Accelerate_421210();
                        }
                    }
                    else if (gCurrCarAI_Velocity_677B00 < gCurrCarAI_DesiredSpeed_677A8C)
                    {
                        field_0_car->field_58_physics->Accelerate_421210();
                    }
                    else
                    {
                        field_0_car->field_58_physics->ForceNeutralInput_42ABC0();
                    }
                }
                else
                {
                    byte_677B3C = 0;
                    field_0_car->field_58_physics->NeutralGear_42AC00();
                }
            }
            else if (field_24_bf.bFlag21)
            {
                flag1 = 1;
            }
            else
            {
                byte_677B3C = 0;
                field_0_car->field_58_physics->NeutralGear_42AC00();
            }
        }
        else if (cBC->field_7C_ped_type == ped_type::player_2)
        {
            if (field_24_bf.bFlag21)
            {
                flag1 = 1;
            }
            else
            {
                byte_677B3C = 0;
                field_0_car->field_58_physics->NeutralGear_42AC00();
            }
            field_0_car->TryHonkHorn_4416D0(2);
        }
        else
        {
            if (ComputeShortestAngleDelta_4056C0(cBC->field_50_car_sprite->field_0_rotation, field_0_car->field_50_car_sprite->field_0_rotation) > kAng90_6779E4)
            {
                flag1 = 1;
            }

            if ((field_24_flags & 0x20000) == 0x20000)
            {
                flag1 = 1;
            }
        }
    }

    if (field_24_bf.bFlag8)
    {
        flag1 = true;
    }

    if (flag1)
    {
        if (gCurrCarAI_IsChasing_677BBC)
        {
            if (!field_0_car->field_60_pChaseTask->field_22_following_route)
            {
                if (!cBC->field_60_pChaseTask || field_0_car->field_60_pChaseTask->field_30_pTargetPed != cBC->field_60_pChaseTask->field_30_pTargetPed)
                {
                    if (cBC)
                    {
                        if (cBC != gCurrCarAI_TargetCar_6779B0)
                        {
                            if (cBC->field_76_last_seen_timer > 5)
                            {
                                if (cBC->field_7C_ped_type == ped_type::dummy_3)
                                {
                                    cBC->MarkForDespawn_421470();
                                }
                            }
                            else
                            {
                                Fix16 pMaybeY_FP16 = cBC->field_50_car_sprite->field_14_xy.x - gCurrCarAI_xpos_677C38;
                                Fix16 pMaybeX_FP16 = cBC->field_50_car_sprite->field_14_xy.y - gCurrCarAI_ypos_677C30;

                                // Copied out of the returned temporary, so bearing_to_other lives in a register (bp)
                                Ang16 bearing_to_other;
                                bearing_to_other = Fix16::atan2_fixed_405320(pMaybeX_FP16, pMaybeY_FP16);

                                if (field_0_car->field_60_pChaseTask)
                                {
                                    switch (field_0_car->field_60_pChaseTask->field_C_chase_state)
                                    {
                                        case car_chase_state::move_left_3:
                                        case car_chase_state::approach_left_6:
                                        case car_chase_state::alongside_left_8:
                                            field_0_car->field_58_physics->TurnAntiClockwise_42AB90();
                                            gCurrCarAI_IsChasing_677BBC = 0;
                                            gCurrCarAI_AllowTrafficSteering_677A5D = 0;
                                            return;
                                        case car_chase_state::move_right_4:
                                        case car_chase_state::approach_right_7:
                                        case car_chase_state::alongside_right_9:
                                            field_0_car->field_58_physics->TurnClockwise_42ABA0();
                                            gCurrCarAI_IsChasing_677BBC = 0;
                                            gCurrCarAI_AllowTrafficSteering_677A5D = 0;
                                            return;
                                        default:
                                            break;
                                    }
                                }

                                if (flag1)
                                {
                                    Ang16 our_heading;
                                    Ang16 their_heading;

                                    if (field_4C_curr_direction != car_ai_direction::south_2)
                                    {
                                        our_heading = field_10_angle;
                                        their_heading = bearing_to_other;
                                    }
                                    else
                                    {
                                        our_heading = field_10_angle + kAng180_677ADE;
                                        their_heading = bearing_to_other + kAng180_677ADE;
                                    }

                                    if (our_heading < their_heading)
                                    {
                                        field_0_car->field_58_physics->TurnClockwise_42ABA0();
                                    }
                                    else
                                    {
                                        field_0_car->field_58_physics->TurnAntiClockwise_42AB90();
                                    }
                                }

                                gCurrCarAI_IsChasing_677BBC = 0;
                                gCurrCarAI_AllowTrafficSteering_677A5D = 0;
                            }
                        }
                    }
                }
            }
        }
    }
    else if (gCurrCarAI_Velocity_677B00 == kF16Zero_677B90)
    {
        if ((field_24_flags & 0x90) == 0)
        {
            ReverseOrNeutral_453C00();
        }
        else
        {
            field_0_car->field_58_physics->NeutralGear_42AC00();
        }
        byte_677B3C = 0;
    }
    else
    {
        byte_677B3C = 0;
        if (!field_24_bf.bFlag12 || field_68_car_in_collision->field_7C_ped_type != ped_type::player_2)
        {
            if (gCurrCarAI_Velocity_677B00 > kFpPoint4_6779C8)
            {
                field_0_car->DoBrakeAndHandbrake_43A970();
            }
            else
            {
                field_0_car->DoBrake_43A950();
            }
        }
    }
}

MATCH_FUNC(0x451fa0)
void CarAI::ReactToNearbyObject_451FA0()
{
    if (!gCurrCarAI_IsChasing_677BBC)
    {
        Object* pObj = field_70_nearest_entity->AsObject2C_40FEC0();
        if (pObj->field_18_model == objects::roadblock_21 || pObj->field_18_model == objects::pedestrian_crossing_marker_122)
        {
            field_0_car->DoBrakeAndHandbrake_43A970();
            byte_677B3C = 0;
            this->field_24_bf.bFlag4 = 1;
        }
    }
}

MATCH_FUNC(0x451ff0)
void CarAI::ReactToNearbyPed_451FF0()
{
    if (!field_24_bf.bFlag21)
    {
        if (!gCurrCarAI_IsChasing_677BBC || (field_0_car->field_60_pChaseTask->field_8_task_type != car_task_type::kill_ped_5) ||
            field_0_car->field_60_pChaseTask->field_30_pTargetPed != field_70_nearest_entity->field_8_char_b4_ptr->field_7C_pPed)
        {
            if (gCurrCarAI_Velocity_677B00 > kFpPoint4_6779C8)
            {
                field_0_car->DoBrakeAndHandbrake_43A970();
                byte_677B3C = 0;
                field_0_car->TryHonkHorn_4416D0(2);
                return;
            }
            field_0_car->DoBrake_43A950();
            byte_677B3C = 0;
        }
    }
    field_0_car->TryHonkHorn_4416D0(2);
}

// 9.6f 0x42ACE0 (a Car method there): sets the flag Object_5C reads while the car probes ahead
static inline void SetByte6771DC_42ACE0(u8 value)
{
    byte_6771DC = value;
}

MATCH_FUNC(0x452060)
void CarAI::ScanAheadForObstacles_452060()
{
    Fix16 probe_height;
    probe_height = gF16fOne_677B94;

    byte_677B8C = 1;
    byte_677B3C = 1;

    Fix16 probe_step = gF16fOne_677B94;

    Car* pCar = this->field_0_car;

    if (pCar->field_7C_ped_type == ped_type::player_2)
    {
        this->field_24_bf.bFlag4 = 0;
        probe_height = gF16fOne_677B94;
    }

    Fix16 new_z;
    if (bOnGradientSlope_677C90)
    {
        new_z = probe_height + pCar->field_50_car_sprite->field_1C_zpos;
    }
    else
    {
        new_z = pCar->field_50_car_sprite->field_1C_zpos;
    }

    Fix16 zpos_ = new_z;

    Fix16 offset_x;
    Fix16 offset_y;
    Ang16::PolarToCartesian_41FC20(this->field_10_angle, probe_height + kFpThreeQuarters_677A4C, offset_x, offset_y);
    field_0_car->field_50_car_sprite->set_xyz_lazy_420600(offset_x + field_0_car->field_50_car_sprite->field_14_xy.x,
                                                          offset_y + field_0_car->field_50_car_sprite->field_14_xy.y,
                                                          new_z);

    SetByte6771DC_42ACE0(1);

    if (this->field_24_bf.bFlag7)
    {
        field_0_car->field_50_car_sprite->set_xyz_lazy_420600(gCurrCarAI_xpos_677C38, gCurrCarAI_ypos_677C30, gCurrCarAI_zpos_677C48);

        Ang16::PolarToCartesian_41FC20(this->field_10_angle + word_677A3A.MultiplyByFix16_401CB0_ctor_ool(Fix16(this->field_0_car->field_58_physics->field_AD_turn_direction)),
                                       gF16fOne_677B94, offset_x, offset_y);
        this->field_0_car->field_50_car_sprite->set_xyz_lazy_420600(offset_x + field_0_car->field_50_car_sprite->field_14_xy.x,
                                                                    offset_y + field_0_car->field_50_car_sprite->field_14_xy.y,
                                                                    new_z);

        this->field_70_nearest_entity = gSpriteGrid_1_679208->FindCollidingSpritePreferType_477E60(this->field_0_car->field_50_car_sprite, 0);
    }
    else
    {
        this->field_70_nearest_entity = gSpriteGrid_1_679208->FindCollidingSpritePreferType_477E60(this->field_0_car->field_50_car_sprite, 0);
        if (!this->field_70_nearest_entity || bOnGradientSlope_677C90)
        {
            field_0_car->field_50_car_sprite->set_xyz_lazy_420600(gCurrCarAI_xpos_677C38, gCurrCarAI_ypos_677C30, gCurrCarAI_zpos_677C48);

            Ang16::PolarToCartesian_41FC20(this->field_10_angle, probe_step, offset_x, offset_y);
            field_0_car->field_50_car_sprite->set_xyz_lazy_420600(offset_x + field_0_car->field_50_car_sprite->field_14_xy.x,
                                                                  offset_y + field_0_car->field_50_car_sprite->field_14_xy.y,
                                                                  new_z);

            this->field_70_nearest_entity = gSpriteGrid_1_679208->FindCollidingSpritePreferType_477E60(this->field_0_car->field_50_car_sprite, 0);
        }
    }

    if (!this->field_70_nearest_entity)
    {
        if (bOnGradientSlope_677C90)
        {
            Fix16 new_y_5 = offset_y + field_0_car->field_50_car_sprite->field_14_xy.y;
            Fix16 new_x_3 = offset_x + field_0_car->field_50_car_sprite->field_14_xy.x;

            field_0_car->field_50_car_sprite->set_xyz_lazy_420600(new_x_3, new_y_5, zpos_ - gF16fOne_677B94);
            new_z = zpos_;
            this->field_70_nearest_entity = gSpriteGrid_1_679208->FindCollidingSpritePreferType_477E60(this->field_0_car->field_50_car_sprite, 0);
        }
    }

    field_0_car->field_50_car_sprite->set_xyz_lazy_420600(gCurrCarAI_xpos_677C38, gCurrCarAI_ypos_677C30, gCurrCarAI_zpos_677C48);

    char_type bCheckMovement;

    if (field_24_bf.bFlag6)
    {
        Ang16::PolarToCartesian_41FC20(this->field_10_angle, kFpHalf_677A84, offset_x, offset_y);
        field_0_car->field_50_car_sprite->set_xyz_lazy_420600(offset_x + field_0_car->field_50_car_sprite->field_14_xy.x,
                                                              offset_y + field_0_car->field_50_car_sprite->field_14_xy.y,
                                                              new_z);
        bCheckMovement = field_0_car->field_50_car_sprite->CheckSpriteMovementRegion_5A2500();
        if (!bCheckMovement)
        {
            Ang16::PolarToCartesian_41FC20(this->field_10_angle, kFpHalf_677A84, offset_x, offset_y);
            field_0_car->field_50_car_sprite->set_xyz_lazy_420600(offset_x + field_0_car->field_50_car_sprite->field_14_xy.x,
                                                                  offset_y + field_0_car->field_50_car_sprite->field_14_xy.y,
                                                                  new_z);
            bCheckMovement = field_0_car->field_50_car_sprite->CheckSpriteMovementRegion_5A2500();
            if (!bCheckMovement)
            {
                Ang16::PolarToCartesian_41FC20(this->field_10_angle, kFpHalf_677A84, offset_x, offset_y);
                field_0_car->field_50_car_sprite->set_xyz_lazy_420600(offset_x + field_0_car->field_50_car_sprite->field_14_xy.x,
                                                                      offset_y + field_0_car->field_50_car_sprite->field_14_xy.y,
                                                                      new_z);
                bCheckMovement = field_0_car->field_50_car_sprite->CheckSpriteMovementRegion_5A2500();
            }
        }
    }
    else
    {
        Ang16::PolarToCartesian_41FC20(this->field_10_angle, gF16fOne_677B94, offset_x, offset_y);
        field_0_car->field_50_car_sprite->set_xyz_lazy_420600(offset_x + field_0_car->field_50_car_sprite->field_14_xy.x,
                                                              offset_y + field_0_car->field_50_car_sprite->field_14_xy.y,
                                                              new_z);
        bCheckMovement = field_0_car->field_50_car_sprite->CheckSpriteMovementRegion_5A2500();
    }

    SetByte6771DC_42ACE0(0);
    if (bOnGradientSlope_677C90)
    {
        if (bCheckMovement)
        {
            bCheckMovement = 0;
        }
    }

    Sprite* f70 = this->field_70_nearest_entity;
    if (!f70 && !bCheckMovement)
    {
        if (field_24_bf.bFlag6)
        {
            if (this->field_5A_obstacle_timer > 0xAu)
            {
                this->field_5A_obstacle_timer = 0;
                this->field_24_bf.bFlag6 = 0;
            }
            else
            {
                byte_677B8C = 0;
            }
            ++this->field_5A_obstacle_timer;
        }

        if (!gCurrCarAI_IsChasing_677BBC || this->field_0_car->field_60_pChaseTask->field_22_following_route)
        {
            DetectCarAhead_44D1D0();
        }
    }
    else if (bCheckMovement)
    {
        gCurrCarAI_IsChasing_677BBC = 0;
        this->field_24_flags &= ~0x80u;
        if (gCurrCarAI_Velocity_677B00 == kF16Zero_677B90)
        {
            this->field_5A_obstacle_timer = 0;
            this->field_24_flags |= 0x40;
            byte_677B8C = 0;
            ReverseOrNeutral_453C00();
        }
        else if (!this->field_24_bf.bFlag6)
        {
            byte_677B8C = 1;
            byte_677B3C = 0;
            if (gCurrCarAI_Velocity_677B00 > kFpPoint4_6779C8)
            {
                field_0_car->DoBrakeAndHandbrake_43A970();
            }
            else
            {
                field_0_car->DoBrake_43A950();
            }
        }
        else
        {
            byte_677B8C = 0;
        }
    }
    else
    {
        // The four block checks each jump to the shared react switch, as in the original.
        if (f70->field_1C_zpos.GetFracValue() == kF16Zero_677B90)
        {
            if (bOnGradientSlope_677C90)
            {
                if (gMap_0x370_6F6268->IsBlockRoadType_42A8C0(f70->field_14_xy.x.ToInt(),
                                                              f70->field_14_xy.y.ToInt(),
                                                              (f70->field_1C_zpos - gF16fOne_677B94).ToInt()))
                {
                    goto react;
                }
            }
            else if (gMap_0x370_6F6268->IsBlockRoadType_42A8C0(f70->field_14_xy.x.ToInt(),
                                                               f70->field_14_xy.y.ToInt(),
                                                               (f70->field_1C_zpos - gF16fOne_677B94).ToInt()))
            {
                goto react;
            }
        }
        else if (bOnGradientSlope_677C90)
        {
            if (gMap_0x370_6F6268->IsBlockRoadType_42A8C0(f70->field_14_xy.x.ToInt(), f70->field_14_xy.y.ToInt(), f70->field_1C_zpos.ToInt()))
            {
                goto react;
            }
        }
        else if (gMap_0x370_6F6268->IsBlockRoadTypeInlined_433470(f70->field_14_xy.x.ToInt(),
                                                                   f70->field_14_xy.y.ToInt(),
                                                                   f70->field_1C_zpos.ToInt()))
        {
        react:
            switch (this->field_70_nearest_entity->GetType_416B40())
            {
                case sprite_types_enum::object_1:
                case sprite_types_enum::code_obj1_4:
                case sprite_types_enum::map_obj_5:
                    ReactToNearbyObject_451FA0();
                    break;

                case sprite_types_enum::car_2:
                    ReactToNearbyCar_451980();
                    break;

                case sprite_types_enum::ped_3:
                    if (!field_24_bf.bFlag7)
                    {
                        ReactToNearbyPed_451FF0();
                    }
                    break;
            }
        }
    }

    if (this->field_6C_yield_to_car)
    {
        if (this->field_6C_yield_to_car->field_5C_AI)
        {
            if (this->field_6C_yield_to_car->field_5C_AI->field_24_bf.bFlag17)
            {
                byte_677B3C = 0;
                field_0_car->DoBrakeAndHandbrake_43A970();
            }
        }

        if (this->field_2C_yield_timer > 0)
        {
            this->field_2C_yield_timer--;
        }
        else
        {
            this->field_6C_yield_to_car = 0;
        }
    }

    AlignToLaneCenter_44AF00();

    if (byte_677B8C)
    {
        if (!byte_677B3C)
        {
            return;
        }

        if ((field_24_flags & 0x80u) == 0 && (field_24_flags & 0x4500) == 0)
        {
            if (gCurrCarAI_Velocity_677B00 < gCurrCarAI_DesiredSpeed_677A8C)
            {
                CarPhysics* pPhysics = this->field_0_car->field_58_physics;
                pPhysics->field_93_is_forward_gas_on = 1;
                pPhysics->field_91_is_foot_brake_on = 0;
                pPhysics->field_94_is_backward_gas_on = 0;
                pPhysics->field_95_is_coasting = 0;
                return;
            }

            CarPhysics* pPhysics_ = this->field_0_car->field_58_physics;
            pPhysics_->field_95_is_coasting = 1;
            pPhysics_->field_91_is_foot_brake_on = 0;
            pPhysics_->field_94_is_backward_gas_on = 0;
            pPhysics_->field_93_is_forward_gas_on = 0;
            return;
        }

        Fix16 target_speed = kFpPoint06_677B70;
        if (kFpPoint06_677B70 > this->field_74_max_speed)
        {
            target_speed = this->field_74_max_speed;
        }

        if (gCurrCarAI_Velocity_677B00 < target_speed)
        {
            CarPhysics* pPhysics = this->field_0_car->field_58_physics;
            pPhysics->field_93_is_forward_gas_on = 1;
            pPhysics->field_91_is_foot_brake_on = 0;
            pPhysics->field_94_is_backward_gas_on = 0;
            pPhysics->field_95_is_coasting = 0;
            return;
        }

        if (gCurrCarAI_Velocity_677B00 > kFpPoint1_6779B8)
        {
            field_0_car->DoBrakeAndHandbrake_43A970();
        }
        else if (gCurrCarAI_Velocity_677B00 > kFpPoint09_677B78)
        {
            field_0_car->DoBrake_43A950();
        }
        else if (gCurrCarAI_Velocity_677B00 > target_speed)
        {
            CarPhysics* pPhysics_ = this->field_0_car->field_58_physics;
            pPhysics_->field_95_is_coasting = 1;
            pPhysics_->field_91_is_foot_brake_on = 0;
            pPhysics_->field_94_is_backward_gas_on = 0;
            pPhysics_->field_93_is_forward_gas_on = 0;
        }
    }
    else
    {
        ReverseOrNeutral_453C00();
    }
}

// https://decomp.me/scratch/zCa7m
MATCH_FUNC(0x452a20)
void CarAI::ManageCollisions_452A20()
{
    Fix16_Point impact_point;

    if (field_24_bf.bFlag12)
    {
        if (field_68_car_in_collision->IsDespawning_4215B0())
        {
            return;
        }

        if (field_3C_seeking_road == 1)
        {
            field_3C_seeking_road = 0;
            field_24_bf.bFlag6 = false;
            field_24_bf.bFlag7 = false;
        }

        if (gCurrCarAI_IsChasing_677BBC)
        {
            if (field_0_car->field_60_pChaseTask)
            {
                if (field_0_car->field_60_pChaseTask->field_10_ram_target)
                {
                    // Past the inline budget: the second multiply is out of line
                    impact_point.FromPolar_41E210(kF16Zero_677B90, kAng0_677CE8);
                    if (gCurrCarAI_TargetCar_6779B0)
                    {
                        if (gCurrCarAI_TargetCar_6779B0 == field_68_car_in_collision)
                        {
                            gCurrCarAI_TargetCar_6779B0->AccumulateDamage_43DA90(500, &impact_point);
                        }
                    }
                }
            }
        }
        field_24_flags &= ~0x30100u;
        gCurrCarAI_IsChasing_677BBC = 0;
        if (field_0_car->field_60_pChaseTask)
        {
            if (field_68_car_in_collision->field_60_pChaseTask)
            {
                field_0_car->field_60_pChaseTask->field_2E_wait_counter = 0;
                field_0_car->field_60_pChaseTask->field_2A_settle_counter = 0;
                field_0_car->field_60_pChaseTask->field_2C_side_counter = 0;
            }
            else
            {
                field_0_car->field_60_pChaseTask->field_2E_wait_counter = 0;
                field_0_car->field_60_pChaseTask->field_2A_settle_counter = 0;
                field_0_car->field_60_pChaseTask->field_2C_side_counter = 0;
            }
        }

        Fix16 rotX;
        Fix16 rotY;
        // Past the inline budget: Normalize and all but the first Fix16 operator go out of line
        RotateAndTranslatePoint_42A720(field_68_car_in_collision->field_50_car_sprite->field_14_xy.x,
                                       field_68_car_in_collision->field_50_car_sprite->field_14_xy.y,
                                       -field_10_angle,
                                       gCurrCarAI_xpos_677C38,
                                       gCurrCarAI_ypos_677C30,
                                       rotX,
                                       rotY);

        // rotX is reused for the sum (a separate local moves it to the stack)
        rotX = rotX + field_0_car->field_50_car_sprite->field_14_xy.x;

        Ped* pDriver = field_68_car_in_collision->field_54_driver;

        if (pDriver && pDriver->PedTypeIs_45EDE0(ped_type::player_2) && field_24_bf.bFlag21 == false)
        {
            field_0_car->field_58_physics->NeutralGear_42AC00();
            gCurrCarAI_AllowTrafficSteering_677A5D = 0;
            return;
        }

        if (field_24_bf.bFlag21)
        {
            if (rotX > field_0_car->field_50_car_sprite->field_14_xy.x)
            {
                field_0_car->field_58_physics->TurnClockwise_42ABA0();
            }
            else
            {
                field_0_car->field_58_physics->TurnAntiClockwise_42AB90();
            }
        }
        else if (field_2A_stopped_timer > 0 && field_2A_stopped_timer < 20)
        {
            if (rotX > field_0_car->field_50_car_sprite->field_14_xy.x)
            {
                field_0_car->field_58_physics->TurnClockwise_42ABA0();
            }
            else
            {
                field_0_car->field_58_physics->TurnAntiClockwise_42AB90();
            }
        }
        else if (field_2A_stopped_timer > 20)
        {
            if (rotX > field_0_car->field_50_car_sprite->field_14_xy.x)
            {
                field_0_car->field_58_physics->TurnAntiClockwise_42AB90();
            }
            else
            {
                field_0_car->field_58_physics->TurnClockwise_42ABA0();
            }
        }
        else
        {
            field_0_car->field_58_physics->NeutralGear_42AC00();
            return;
        }

        if (field_70_nearest_entity)
        {
            if (field_70_nearest_entity->field_8_car_bc_ptr == field_68_car_in_collision)
            {
                field_0_car->field_58_physics->Accelerate_421210();
            }
            else
            {
                field_0_car->field_58_physics->NeutralGear_42AC00();
            }
        }
        else
        {
            field_0_car->field_58_physics->NeutralGear_42AC00();
        }
        gCurrCarAI_AllowTrafficSteering_677A5D = 0;

        if (field_24_bf.bFlag21 && field_2A_stopped_timer < 60 && gCurrCarAI_Velocity_677B00 < kFpPoint03_677B58)
        {
            field_0_car->field_58_physics->Accelerate_421210();
        }

        if (!field_54_accel_cooldown)
        {
            field_54_accel_cooldown = 4;
        }
    }

    if (field_54_accel_cooldown > 0)
    {
        field_54_accel_cooldown--;
    }
}

// https://decomp.me/scratch/uZlwP
MATCH_FUNC(0x452df0)
void CarAI::UpdateDrivingAI_452DF0()
{

    gCurrCarAI_AllowTrafficSteering_677A5D = 1;
    bool bUpdateStateMachine = false;
    gCurrCarAI_xpos_677C38 = field_0_car->field_50_car_sprite->field_14_xy.x;
    gCurrCarAI_ypos_677C30 = field_0_car->field_50_car_sprite->field_14_xy.y;
    gCurrCarAI_zpos_677C48 = field_0_car->field_50_car_sprite->field_1C_zpos;
    gCurrCarAI_TargetCar_6779B0 = 0;
    byte_677CA8 = 0;

    if (field_0_car->field_60_pChaseTask && !field_3C_seeking_road)
    {
        UpdateChase_44E0C0();
        byte_677CA8 = 1;
    }
    else
    {
        gCurrCarAI_IsChasing_677BBC = 0;
    }

    gmp_block_info* pBlock = gMap_0x370_6F6268->GetBlock_4DFE10(field_0_car->field_50_car_sprite->field_14_xy.x.ToInt(),
                                                                 field_0_car->field_50_car_sprite->field_14_xy.y.ToInt(),
                                                                 field_0_car->field_50_car_sprite->field_1C_zpos.ToInt());

    if (pBlock && IS_GRADIENT_SLOPE(pBlock->field_B_slope_type) &&
        GET_BLOCK_TYPE(pBlock->field_B_slope_type) != AIR)
    {
        bOnGradientSlope_677C90 = true;
    }
    else
    {
        pBlock = gMap_0x370_6F6268->GetBlock_4DFE10(field_0_car->field_50_car_sprite->field_14_xy.x.ToInt(),
                                                     field_0_car->field_50_car_sprite->field_14_xy.y.ToInt(),
                                                     (field_0_car->field_50_car_sprite->field_1C_zpos.ToInt()) - 1);
        bOnGradientSlope_677C90 = false;
    }

    if (pBlock)
    {
        gCurrCarAI_ArrowDirection_677C06 = gMap_0x370_6F6268->GetArrowDirectionFromBlock_4E5FC0(pBlock, 1);
    }
    else
    {
        gCurrCarAI_ArrowDirection_677C06 = 0;
    }

    field_24_bf.bFlag13 = true;
    byte_677A5C = 0;

    field_4C_curr_direction = Ang16::GetAngleFace_4F78F0(field_10_angle);
    gCurrCarAI_DesiredSpeed_677A8C = field_74_max_speed;
    gCurrCarAI_SpriteHeight_677C9C = gPixelsToFix16_6F6850.list[gGtx_0x106C_703DD4->get_car_info_5AA3B0(field_0_car->field_84_car_info_idx)->h];

    field_24_flags = field_24_flags & ~0x200000u | (field_0_car->field_54_driver->field_21C_bf.bDriveAggressively << 21);

    if (gCurrCarAI_IsChasing_677BBC)
    {
        if (!field_0_car->field_60_pChaseTask->field_22_following_route)
        {
            bUpdateStateMachine = true;
            field_24_bf.bFlag21 = true;
        }
    }

    if (field_3C_seeking_road)
    {
        ++field_58_off_road_timer;
        if (field_0_car->field_7C_ped_type != ped_type::script_created_5)
        {
            if (field_58_off_road_timer > 200)
            {
                field_0_car->field_80_abandoned = 1;
            }
        }
        else
        {
            field_58_off_road_timer = 0;
        }

        bUpdateStateMachine = false;
        if (field_24_bf.bFlag6)
        {
            field_50_seek_road_turn = 3;
        }

        switch (field_50_seek_road_turn)
        {
            case 0:
                field_24_flags &= ~0x20C0;
                field_0_car->field_58_physics->SetGoStraight_42ABB0();
                break;

            case 1:
                field_24_flags &= ~0x2040;
                field_0_car->field_58_physics->TurnAntiClockwise_42AB90();
                field_24_bf.bFlag7 = true;
                break;

            case 2:
                field_24_flags &= ~0x2040;
                field_0_car->field_58_physics->TurnClockwise_42ABA0();
                field_24_bf.bFlag7 = true;
                break;

            case 3:
                field_24_flags = (field_24_flags & ~0x2080) | 0x40;
                break;

            default:
                break;
        }
    }

    ScanAheadForObstacles_452060();

    if (gCurrCarAI_Velocity_677B00 != kF16Zero_677B90)
    {
        if (field_0_car->field_58_physics->field_0_vel_read_only.GetLength_41E260() == kF16Zero_677B90)
        {
            ++field_2A_stopped_timer;
        }
        else
        {
            field_2A_stopped_timer = 0;
        }
    }
    else
    {
        ++field_2A_stopped_timer;
    }

    ManageCollisions_452A20();

    if (bUpdateStateMachine)
    {
        UpdateStateMachine_44E560();
    }

    //CarChaseTask* v15 = field_0->field_60;
    if (field_0_car->field_60_pChaseTask && field_0_car->field_60_pChaseTask->field_8_task_type == car_task_type::follow_ped_2)
    {
        gCurrCarAI_IsChasing_677BBC = 1;
    }
    else if (!gCurrCarAI_IsChasing_677BBC)
    {
        if (field_70_nearest_entity && field_70_nearest_entity->GetType_416B40() == sprite_types_enum::object_1 &&
            field_70_nearest_entity->AsObject2C_40FEC0()->field_18_model == objects::pedestrian_crossing_marker_122)
        {
            field_24_bf.bFlag4 = true;
            field_70_nearest_entity = 0;
        }
        else
        {
            field_24_bf.bFlag4 = false;
        }
    }

    if (field_28_route_idx > 0 && field_24_bf.bFlag4)
    {
        //CarChaseTask* v19 = field_0->field_60;
        if (field_0_car->field_60_pChaseTask)
        {
            if (field_0_car->field_60_pChaseTask->field_22_following_route == 1)
            {
                DoShortcutsUsingJunctions_447970();
            }
        }
    }

    if (!gCurrCarAI_AllowTrafficSteering_677A5D || field_54_accel_cooldown)
    {
        field_24_bf.bFlag13 = false;
    }
    else
    {
        ManageTrafficCarDirection_448CE0();
    }

    if (byte_677A5C)
    {
        field_24_bf.bFlag13 = true;
    }

    field_68_car_in_collision = 0;
    field_24_bf.bFlag12 = false;
}

MATCH_FUNC(0x453470)
void CarAI::UpdateDriving_453470()
{
    this->field_0_car->field_58_physics->SetHandBrakeOff_421260();
    Fix16 t = field_0_car->field_58_physics->GetLinearSpeed_4211A0();
    gCurrCarAI_Velocity_677B00 = t;

    if (this->field_0_car->field_80_abandoned)
    {
        this->field_0_car->SetAIHalted_421540(); // stop the vehicle
    }

    if (this->field_30_forced_stop_timer > 0)
    {
        this->field_30_forced_stop_timer--;

        if (this->field_30_forced_stop_timer)
        {
            this->field_0_car->SetAIHalted_421540(); // stop the vehicle
        }
        else
        {
            this->field_0_car->ClearAIHalted_421550();
        }
    }

    if (this->field_0_car->IsAIHalted_42AC60()) // if the vehicle is forced to stop
    {
        if (this->field_24_bf.bAIHalted)
        {
            if (gCurrCarAI_Velocity_677B00 > kFpPoint05_677B60)
            {
                field_0_car->DoBrakeAndHandbrake_43A970();
            }
            else
            {
                field_0_car->DoBrake_43A950();
            }
            this->field_68_car_in_collision = 0;
            this->field_24_bf.bFlag12 = 0;
        }
        else
        {
            if (gCurrCarAI_Velocity_677B00 > kFpPoint05_677B60)
            {
                field_0_car->DoBrakeAndHandbrake_43A970();
            }
            else
            {
                field_0_car->field_58_physics->NeutralGear_42AC00();
            }
            this->field_68_car_in_collision = 0;
            this->field_24_bf.bFlag12 = 0;
        }
    }
    else
    {
        UpdateDrivingAI_452DF0(); // if the vehicle is free to go: do AI stuff
    }
}

MATCH_FUNC(0x4537d0)
void CarAI::ChooseRandomTurn_4537D0()
{
    if (!this->field_0_car->IsTurnLeft_42AC30() && !this->field_0_car->IsTurnRight_42AC20())
    {
        if (gRng_6F6784.get_int_4F7AE0(2) > 0)
        {
            field_0_car->field_A6_bf.bTurnLeft = true;
        }
        else
        {
            field_0_car->field_A6_bf.bTurnRight = true;
        }
    }
}

MATCH_FUNC(0x4538b0)
void CarAI::ClearSteeringIfTurning_4538B0()
{
    if (this->field_24_bf.bFlag7)
    {
        this->field_0_car->field_A6_bf.bTurnLeft = false;
        this->field_0_car->field_A6_bf.bTurnRight = false;
    }
}

MATCH_FUNC(0x453990)
void CarAI::RaiseSpeedTo_453990(Fix16 target_speed)
{
    if (this->field_14_speed < target_speed)
    {
        this->field_14_speed = target_speed;
    }
}

MATCH_FUNC(0x4539b0)
void CarAI::ClearTurnReady_4539B0()
{
    this->field_0_car->field_A6_bf.bTurnRightReady = false;
    this->field_0_car->field_A6_bf.bTurnLeftReady = false;
}

MATCH_FUNC(0x4539d0)
void CarAI::UpdateSpeedTowardTarget_4539D0()
{
    Fix16 f14 = this->field_14_speed;
    Fix16 f18 = this->field_18_target_speed;
    if (f14 < f18)
    {
        this->field_14_speed = field_1C_acceleration + f14;
        if (field_1C_acceleration < kF16Zero_677B90)
        {
            RaiseSpeedTo_453990(this->field_18_target_speed);
        }
    }
    else
    {
        if (f14 > f18)
        {
            this->field_14_speed = f14 - ((kFpFour_677BA0 * kFpPoint02_677AC4));
            RaiseSpeedTo_453990(field_18_target_speed);
        }
    }

    CarPhysics* pPhysics = this->field_0_car->field_58_physics;
    pPhysics->field_91_is_foot_brake_on = 0;
    pPhysics->field_93_is_forward_gas_on = 0;
    pPhysics->field_94_is_backward_gas_on = 0;
    pPhysics->field_95_is_coasting = 0;
}

MATCH_FUNC(0x453a40)
void CarAI::UpdateTrainMovement_453A40()
{
    Fix16 zpos = kF16Zero_677B90;
    s32 rotation_mode = 0;
    Car* pCar = this->field_0_car;

    this->field_10_angle = this->field_0_car->field_50_car_sprite->field_0_rotation;

    Fix16 xpos = pCar->field_50_car_sprite->field_14_xy.x;
    Fix16 ypos = pCar->field_50_car_sprite->field_14_xy.y;
    zpos = pCar->field_50_car_sprite->field_1C_zpos;

    s32 track_dir;
    if (gPublicTransport_181C_6FF1D4->GetTrainSpeed_579B90(pCar, &this->field_18_target_speed))
    {
        track_dir = gMap_0x370_6F6268->AdvanceAlongRailway_4E6660(&xpos, &ypos, &zpos, this->field_14_speed);
    }
    else
    {
        track_dir = gMap_0x370_6F6268->AdvanceAlongRailwayChecked_4E7190(&xpos, &ypos, &zpos, this->field_14_speed);
    }

    s32 AngleFace_4F78F0 = Ang16::GetAngleFace_4F78F0(field_10_angle);
    s32 cur_face = Ang16::GetAngleFace_4F78F0(field_10_angle);
    if (track_dir != cur_face)
    {
        switch (track_dir)
        {
            case road_direction::up_1:
                rotation_mode = (AngleFace_4F78F0 != 3) + 1;
                break;
            case road_direction::down_2:
                rotation_mode = (AngleFace_4F78F0 == 3) + 1;
                break;
            case road_direction::right_3:
                rotation_mode = (AngleFace_4F78F0 != 2) + 1;
                break;
            case road_direction::left_4:
                rotation_mode = (AngleFace_4F78F0 == 2) + 1;
                break;
            default:
                break;
        }
    }

    Fix16 i;
    for (i = Ang16::Ang16_to_Fix16(ReturnAngleFromRoadDirection_4F7940(track_dir)); i < kF16Zero_677B90; i += kFpTwoPi_677C84)
    {
        ;
    }

    for (; i >= kFpTwoPi_677C84; i -= kFpTwoPi_677C84)
    {
        ;
    }

    UpdateSpeedTowardTarget_4539D0();
    field_0_car->field_58_physics->SetVelocityTowardTarget_55A1D0(xpos, ypos, i, &rotation_mode);
}

MATCH_FUNC(0x453bb0)
void CarAI::AI_Service_453BB0()
{
    InitAIvars_447650();
    this->field_10_angle = this->field_0_car->field_50_car_sprite->field_0_rotation;
    this->field_4C_curr_direction = Ang16::GetAngleFace_4F78F0(field_10_angle);
    if (field_2B_ticks_since_alloc < 0xff)
    {
        field_2B_ticks_since_alloc++;
    }
    UpdateDriving_453470();
}

MATCH_FUNC(0x453bf0)
void CarAI::SetCar_453BF0(Car* pCar)
{
    field_0_car = pCar;
}

// https://decomp.me/scratch/kidaG
// Keeps the angle temporaries in their own scope, so the speed temporary below can reuse
// their stack slot (as in the original)
static inline bool IsMovingForwards_453C00(CarPhysics* pPhysics)
{
    Ang16 heading_delta = pPhysics->field_40_linvel_1.atan2_40F790() - pPhysics->field_58_theta;
    return heading_delta <= kAng90_6779E4 || heading_delta >= kAng270_677B08;
}

MATCH_FUNC(0x453c00)
void CarAI::ReverseOrNeutral_453C00()
{
    if (IsMovingForwards_453C00(field_0_car->field_58_physics) || field_0_car->GetCarLinearSpeed_43A240() < gCurrCarAI_DesiredSpeed_677A8C)
    {
        field_0_car->field_58_physics->ForceReverseInput();
    }
    else
    {
        field_0_car->field_58_physics->ForceNeutralInput_42ABC0();
    }
}

MATCH_FUNC(0x453cb0)
CarAI::CarAI()
{
    this->field_10_angle = 0;
    this->field_0_car = 0;
    this->field_4_unused = 0;
    this->field_8_maneuver_active = 0;
    this->field_9_probe_x = 0;
    this->field_A_probe_y = 0;
    this->mpNext = 0;
    this->field_10_angle = kAng0_677CE8;
    this->field_14_speed = kF16Zero_677B90;
    this->field_18_target_speed = kDefaultTargetSpeed_677CB4;
    this->field_1C_acceleration = kDefaultAcceleration_6779A4;
    this->field_20_unused = 0;
    this->field_56_route_pos = 0;
    this->field_28_route_idx = -1;
    this->field_29_pad = 0;
    this->field_58_off_road_timer = 0;
    this->field_24_flags &= ~0x2DFEFu;
    this->field_24_flags |= 0x12010u;
    this->field_2D_arrow_start = 0;
    this->field_2E_arrow_cur = 0;
    this->field_2F_arrow_end = 0;
    this->field_5C_unused = kF16Zero_677B90;
    this->field_60_unused = kF16Zero_677B90;
    this->field_64_unused = kF16Zero_677B90;
    this->field_68_car_in_collision = 0;
    this->field_6C_yield_to_car = 0;
    this->field_34_unused = 0;
    this->field_38_junction_turn_direction = 0;
    this->field_3C_seeking_road = 0;
    this->field_40_prev_target_direction = 0;
    this->field_44_target_direction = car_ai_target_direction::go_straight_0;
    this->field_48_probe_direction = 0;
    this->field_4C_curr_direction = car_ai_direction::none_0;
    this->field_70_nearest_entity = 0;
    this->field_74_max_speed = kF16Zero_677B90;
    this->field_54_accel_cooldown = 0;
    this->field_2A_stopped_timer = 0;
    this->field_2B_ticks_since_alloc = 0;
    this->field_2C_yield_timer = 0;
    this->field_5A_obstacle_timer = 0;
    this->field_50_seek_road_turn = 0;
    this->field_24_flags &= ~0x3C0000u;
}

MATCH_FUNC(0x453d80)
void CarAI::PoolAllocate()
{
    this->field_0_car = 0;
    this->field_4_unused = 0;
    this->field_8_maneuver_active = 0;
    this->field_9_probe_x = 0;
    this->field_A_probe_y = 0;
    this->field_10_angle = kAng0_677CE8;
    this->field_14_speed = kF16Zero_677B90;
    this->field_18_target_speed = kDefaultTargetSpeed_677CB4;
    this->field_1C_acceleration = kDefaultAcceleration_6779A4;
    this->field_20_unused = 0;
    this->field_56_route_pos = 0;
    this->field_28_route_idx = -1;
    this->field_29_pad = 0;
    this->field_58_off_road_timer = 0;
    this->field_24_flags &= ~0x2DFEFu;
    this->field_24_flags |= 0x12010u;
    this->field_2D_arrow_start = 0;
    this->field_2E_arrow_cur = 0;
    this->field_2F_arrow_end = 0;
    this->field_5C_unused = kF16Zero_677B90;
    this->field_60_unused = kF16Zero_677B90;
    this->field_64_unused = kF16Zero_677B90;
    this->field_68_car_in_collision = 0;
    this->field_6C_yield_to_car = 0;
    this->field_34_unused = 0;
    this->field_38_junction_turn_direction = 0;
    this->field_3C_seeking_road = 0;
    this->field_40_prev_target_direction = 0;
    this->field_44_target_direction = car_ai_target_direction::go_straight_0;
    this->field_48_probe_direction = 0;
    this->field_4C_curr_direction = car_ai_direction::none_0;
    this->field_70_nearest_entity = 0;
    this->field_74_max_speed = kFpPoint7_6779D4;
    this->field_54_accel_cooldown = 0;
    this->field_2A_stopped_timer = 0;
    this->field_2B_ticks_since_alloc = 0;
    this->field_2C_yield_timer = 0;
    this->field_24_flags &= ~0x3C0000u;
    this->field_50_seek_road_turn = 0;
    this->field_5A_obstacle_timer = 0;
    this->field_30_forced_stop_timer = 0;
}
