#include "Ped.hpp"
#include "ped_move_target_type.hpp"
#include "ped_attack_target_type.hpp"
#include "ped_class.hpp"
#include "ped_fire_mode.hpp"
#include "ped_jump_over_mode.hpp"
#include "ped_lifetime.hpp"
#include "explosion_type.hpp"
#include "car_despawn_status.hpp"
#include "ped_death_cause.hpp"
#include "char_b4_state_more.hpp"
#include "char_anim_state_more.hpp"
#include "ped_state_2_more.hpp"
#include "ped_group_type.hpp"
#include "ped_voice_event.hpp"
#include "accuracy_event.hpp"
#include "AmbulanceService.hpp"
#include "CarInfoTable.hpp"
#include "CarPhysics.hpp"
#include "Car.hpp"
#include "Char_Pool.hpp"
#include "ScriptManager.hpp"
#include "Game.hpp"
#include "Gang.hpp"
#include "Garage.hpp"
#include "Globals.hpp"
#include "CarChaseTaskTable.hpp"
#include "Hud.hpp"
#include "PatrolRoutePool.hpp"
#include "Object_5C.hpp"
#include "PathFinder.hpp"
#include "ParticleManager.hpp"
#include "PedGroup.hpp"
#include "Player.hpp"
#include "Police.hpp"
#include "PublicTransport.hpp"
#include "SpriteGrid.hpp"
#include "RouteFinder.hpp"
#include "CrimeReportQueue.hpp"
#include "TaxiList.hpp"
#include "TrafficLights.hpp"
#include "PedRefTable.hpp"
#include "Weapon.hpp"
#include "WeaponManager.hpp"
#include "Explosion.hpp"
#include "char.hpp"
#include "debug.hpp"
#include "error.hpp"
#include "GameSession.hpp"
#include "map_0x370.hpp"
#include "rng.hpp"
#include "sprite.hpp"
#include "TagGame.hpp"
#include "CarAI.hpp"
#include "winmain.hpp"
#include "ped_graphic_type.hpp"

// Modes of ChangePedStatesByMode: the ped_state_1/ped_state_2 pair each one sets
namespace ped_states_mode
{
enum
{
    walk_1 = 1,
    flee_2 = 2,
    run_to_target_3 = 3,
    stand_still_4 = 4,
    drive_5 = 5,
    go_to_car_6 = 6,
    exit_car_7 = 7,
    unchanged_99 = 99,
};
}

// =================
DEFINE_GLOBAL_INIT(s8, gObjectiveMayAct, 1, 0x61A8A3);
DEFINE_GLOBAL_INIT(Ang16, kAng0_6FDB34, Ang16(0), 0x6FDB34);
DEFINE_GLOBAL_INIT(Ang16, gDummyPedAng, Ang16(0), 0x6787A8);
DEFINE_GLOBAL_INIT(Ang16, gPedAng, Ang16(4), 0x6787A0);
DEFINE_GLOBAL_INIT(Fix16, kFpThree_67866C, Fix16(3), 0x67866C);
DEFINE_GLOBAL_INIT(s32, gPedId, 0x7, 0x61A89C);
DEFINE_GLOBAL_INIT(u8, gNumberMuggersSpawned, 0, 0x6787CA);
DEFINE_GLOBAL_INIT(u8, gNumberCarThiefsSpawned, 0, 0x6787CB);
DEFINE_GLOBAL_INIT(u8, gNumberElvisLeadersSpawned, 0, 0x6787CC);
DEFINE_GLOBAL_INIT(u8, gNumberWalkingCopsSpawned, 0, 0x6787CD);
DEFINE_GLOBAL(u8, gNewTaxiCustomersThisTick, 0x6787D2);
DEFINE_GLOBAL_INIT(u8, gRunPedObjectiveAI, 1, 0x61A8A0);
DEFINE_GLOBAL(u8, gNumDummyChars, 0x6787E2);
DEFINE_GLOBAL(u8, gNumScriptCreatedPeds, 0x6787E3);
DEFINE_GLOBAL(u8, gNumEmergencyPeds, 0x6787E4);
EXTERN_GLOBAL(u8, gTargetSearchMode);
DEFINE_GLOBAL(u8, gCloseRangeHit, 0x6787D4);
DEFINE_STATIC_MEMBER(u8, Ped, sPedUsedPathThisTick, 0x678554);
DEFINE_GLOBAL(u8, gPathStepRoundDone, 0x6787D8);
DEFINE_GLOBAL(u8, gLosCheckRoundDone, 0x6787D9);
DEFINE_GLOBAL_INIT(u8, gRunPedThink, 1, 0x61A8A4);
DEFINE_STATIC_MEMBER(u8, Ped, sKeepPaceWithTarget, 0x6787C4);
DEFINE_GLOBAL(s16, gNumPedsCrossingRoad, 0x6787D0);
DEFINE_GLOBAL(s16, gElvisVoiceTimer, 0x6787F2);
DEFINE_GLOBAL(u16, gNumPedsUpdated, 0x6787E0);
DEFINE_GLOBAL(Ped*, gSearchingPed, 0x6787DC);
DEFINE_GLOBAL_INIT(Fix16, kFpZero_678660, Fix16(0), 0x678660);
DEFINE_GLOBAL_INIT(Fix16, kFpZero_678438, kFpZero_678660, 0x678438);
DEFINE_GLOBAL_INIT(Fix16, gDistanceToTarget, kFpZero_678660, 0x678750);
DEFINE_GLOBAL_INIT(Fix16, kFpSix_678678, Fix16(6), 0x678678);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpSix, kFpSix_678678, 0x678520);
DEFINE_GLOBAL_INIT(Fix16, kFpFour_678670, Fix16(4), 0x678670);
DEFINE_GLOBAL_INIT(Fix16, kFpOne64th_6784C4, Fix16(256, 0), 0x6784C4);
DEFINE_GLOBAL_INIT(Fix16, kFpOneSixteenth_678448, kFpFour_678670* kFpOne64th_6784C4, 0x678448);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpHalf, kFpOne64th_6784C4 * 32, 0x678790);
DEFINE_GLOBAL_INIT(Fix16, kFpOneEighth_6784E8, kFpOne64th_6784C4 * 8, 0x6784E8);
DEFINE_GLOBAL_INIT(Fix16, kFpOneThirtySecond_6784CC, kFpOne64th_6784C4 * 2, 0x6784CC);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpOneThirtySecond, kFpOneThirtySecond_6784CC, 0x678434);
DEFINE_GLOBAL_INIT(Fix16, kFpOne256th_678620, kFpOne64th_6784C4 / kFpFour_678670, 0x678620);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpQuarter, kFpOne64th_6784C4 * 16, 0x678788);
DEFINE_GLOBAL_INIT(Fix16, kFpOne_678664, Fix16(1), 0x678664);
DEFINE_GLOBAL_INIT(Fix16, kFpOne_6785EC, kFpOne_678664, 0x6785EC);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint01_678624, Fix16(0xA3, 0), 0x678624);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_67853C, Fix16(0x2000, 0), 0x67853C);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint05_678634, Fix16(0x333, 0), 0x678634);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint1_678480, Fix16(0x666, 0), 0x678480);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint9, Fix16(0x3999, 0), 0x6784A4);
DEFINE_GLOBAL_INIT(Ang16, kAng45_6784FC, Ang16(180), 0x6784FC);
DEFINE_STATIC_GLOBAL_INIT(Ang16, kAng180, Ang16(720), 0x678590);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpThreeThirtySeconds, kFpOne64th_6784C4 * 6, 0x6784DC);
DEFINE_GLOBAL_INIT(Fix16, kFpTwo_678668, Fix16(2), 0x678668);
DEFINE_GLOBAL_INIT(Fix16, gSpawnJitterScale, Fix16(256, 0), 0x678618);
DEFINE_GLOBAL_INIT(Fix16, kFpQuarter_678484, Fix16(0x1000, 0), 0x678484);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint2_678488, Fix16(0xCCC, 0), 0x678488);
DEFINE_GLOBAL(Ped*, gLastProcessedPed, 0x6787C0);
DEFINE_GLOBAL_INIT(Fix16, gDummyW, kFpPoint2_678488, 0x678530);
DEFINE_GLOBAL_INIT(Fix16, gDummyZ, kFpQuarter_678484, 0x67841C);
DEFINE_GLOBAL(char_type, gNumberBusCustomers, 0x6787D3);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpOneSixth, Fix16(0xAAA, 0), 0x678504);
DEFINE_GLOBAL_INIT(Fix16, kFpQuarter_678574, kFpQuarter_678484, 0x678574);
DEFINE_GLOBAL_INIT(Fix16, kFpOneEighth_67845C, kFpQuarter_678574 / kFpTwo_678668, 0x67845C);
DEFINE_GLOBAL_INIT(Fix16, kFpOne_678798, kFpOne64th_6784C4 * 64, 0x678798);
DEFINE_GLOBAL_INIT(Fix16, kFpTwo_678658, kFpOne64th_6784C4 * 128, 0x678658);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpFour, kFpOne64th_6784C4 * 256, 0x678680);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpOne64th, kFpOne64th_6784C4, 0x678430);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFp9999, Fix16(9999), 0x678524);

DEFINE_GLOBAL_INIT(s16, gPedOffscreenDespawnTime, 40, 0x678656);
DEFINE_GLOBAL(u8, gNumberArmedGangMembers, 0x6787CE);

DEFINE_GLOBAL_INIT(Fix16, kFpPoint8_6784A0, Fix16(0x3333, 0), 0x6784A0);
DEFINE_GLOBAL_INIT(Fix16, kFpOne128th_6784BC, kFpOne64th_6784C4 / kFpTwo_678668, 0x6784BC);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpThreeSixtyFourths, kFpThree_67866C * kFpOne64th_6784C4, 0x678444);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpFiveSixteenths, kFpOne64th_6784C4 * 20, 0x678784);

DEFINE_STATIC_GLOBAL_INIT(Ang16, kAng10, Ang16(40), 0x6784C8);
DEFINE_GLOBAL_INIT(Ang16, kAng16_6784E4, Ang16(64), 0x6784E4);
DEFINE_GLOBAL_INIT(Ang16, gSearchConeHalfAngle, Ang16(0), 0x6784F0);

DEFINE_STATIC_GLOBAL_INIT(Ang16, kAng90, Ang16(360), 0x678502);
DEFINE_GLOBAL_INIT(Ang16, kAng270_6785D0, Ang16(1080), 0x6785D0);
DEFINE_GLOBAL_INIT(Ang16, kAng225_6786B8, Ang16(900), 0x6786B8);
DEFINE_STATIC_GLOBAL_INIT(Ang16, kAng45, Ang16(180), 0x6784E2);
DEFINE_GLOBAL_INIT(Ang16, kAng315_6785A8, Ang16(1260), 0x6785A8);
DEFINE_STATIC_GLOBAL_INIT(Ang16, kAng135, Ang16(540), 0x67844C);

DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint2, kFpPoint2_678488, 0x67856C);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint1, kFpPoint1_678480, 0x678428);

DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpFiveThirtySeconds, kFpOne64th_6784C4 * 10, 0x678778);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpThreeQuarters, kFpOne64th_6784C4 * 48, 0x678794);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint02, Fix16(0x147, 0), 0x678630);

EXTERN_GLOBAL(Ang16, kAng180_6785A6);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpThreeSixteenths, kFpOne64th_6784C4 * 12, 0x678780);
DEFINE_GLOBAL_INIT(Ang16, kAng90_6784B0, Ang16(360), 0x6784B0); // TODO: Init via 0x45FAA0 func

EXTERN_GLOBAL(u8, bHaveThreateningPeds);
EXTERN_GLOBAL(u8, gPathStepSlotFree);

EXTERN_GLOBAL(PixelsToFix16Table, gPixelsToFix16);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint1_6FD824, Fix16(0x666, 0), 0x6FD824);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint06, Fix16(0x3D7, 0), 0x6FD9C8);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint02_6FD9AC, Fix16(0x147, 0), 0x6FD9AC);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint08, Fix16(0x51E, 0), 0x6FD9D4);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint04, Fix16(0x28F, 0), 0x6FD9B8);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint3_6FD830, Fix16(0x1333, 0), 0x6FD830);

DEFINE_GLOBAL_INIT(Fix16, kFpFive_678674, Fix16(5), 0x678674);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpThreeEighths, kFpOne64th_6784C4 * 24, 0x67878C);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpEight, kFpOne64th_6784C4 * 512, 0x6786C0);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpFiveSixtyFourths, kFpFive_678674 * kFpOne64th_6784C4, 0x67843C);

EXTERN_GLOBAL(Fix16, gCharB4_WorldCollisionOffset);
EXTERN_GLOBAL(Fix16, kFpOne_6FDB20);
EXTERN_GLOBAL(Fix16, kFP16Zero_6FD9E4);
EXTERN_GLOBAL(Fix16, kFpPoint2_6FD82C);
EXTERN_GLOBAL(Fix16, kFpPoint05_6FD9B0);

EXTERN_GLOBAL(Ang16, kAng180_6FD936);
EXTERN_GLOBAL(Ang16, kAng90_6FD854);

EXTERN_GLOBAL(Ang16, gSpawnRotationLeft);
EXTERN_GLOBAL(Ang16, gSpawnRotationTop);
EXTERN_GLOBAL(Ang16, gSpawnRotationRight);
EXTERN_GLOBAL(Ang16, gSpawnRotationBottom);

// https://decomp.me/scratch/BzcQt
MATCH_FUNC(0x545AF0)
EXPORT void __stdcall CarDoorAlignmentSolver(u8 animPhase, Car* pCar, u8 doorId, Fix16& outX, Fix16& outY, Ang16& outAng)
{
    u8* car_door_info_array = gGtx_0x106C->get_car_door_data(pCar->mCarInfoIdx);
    Fix16 offset = gCharB4_WorldCollisionOffset;

    Fix16 x_pos;
    Fix16 y_pos;
    s8 x_in_scale = car_door_info_array[2 * doorId + 1];
    x_pos = gPixelsToFix16.SignedPixelsToFix16(x_in_scale);

    s8 y_in_scale = car_door_info_array[2 * doorId + 2];
    y_pos = gPixelsToFix16.SignedPixelsToFix16(y_in_scale);

    bool bXOffsetPastTile = false;
    bool bFront = false;

    if (x_pos >= kFpOne_6FDB20)
    {
        x_pos -= kFpOne_6FDB20;
        bXOffsetPastTile = true;
    }
    else
    {
        if (x_pos <= -kFpOne_6FDB20)
        {
            x_pos += kFpOne_6FDB20;
            bXOffsetPastTile = true;
        }
    }

    switch (doorId)
    {
        case 0:
        case 2:
            if (x_pos == kFP16Zero_6FD9E4)
            {
                y_pos -= offset;
                bFront = true;
            }
            else
            {
                if (x_pos < kFP16Zero_6FD9E4)
                {
                    x_pos -= offset;
                }
                else
                {
                    x_pos += offset;
                }
                if (bXOffsetPastTile)
                {
                    y_pos -= kFpPoint1_6FD824;
                    switch (animPhase)
                    {
                        case 0:
                            x_pos -= kFpPoint2_6FD82C;
                            break;
                        case 1:
                            x_pos -= kFpPoint1_6FD824 + kFpPoint06;
                            break;
                        case 2:
                            x_pos -= kFpPoint1_6FD824 + kFpPoint02_6FD9AC;
                            break;
                        case 3:
                            x_pos -= kFpPoint08;
                            break;
                        case 4:
                            x_pos -= kFpPoint04;
                            break;
                        case 6:
                            x_pos += kFpPoint04;
                            break;
                        case 7:
                            x_pos += kFpPoint08;
                            break;
                        default:
                            break;
                    }
                }
            }
            // VC6 threads the bFront/bXOffsetPastTile tests below away, but they set the block and register order
            if (bFront)
            {
                switch (animPhase)
                {
                    case 0:
                        y_pos += kFpPoint2_6FD82C;
                        break;
                    case 1:
                        y_pos += kFpPoint1_6FD824 + kFpPoint06;
                        break;
                    case 2:
                        y_pos += kFpPoint1_6FD824 + kFpPoint02_6FD9AC;
                        break;
                    case 3:
                        y_pos += kFpPoint08;
                        break;
                    case 4:
                        y_pos += kFpPoint04;
                        break;
                    case 6:
                        y_pos -= kFpPoint04;
                        break;
                    case 7:
                        y_pos -= kFpPoint06;
                        break;
                    default:
                        break;
                }
            }
            else if (!bXOffsetPastTile)
            {
                switch (animPhase)
                {
                    case 0:
                        y_pos -= kFpPoint1_6FD824;
                        x_pos -= kFpPoint1_6FD824;
                        outAng = pCar->mCarSprite->mRotation;
                        break;
                    case 1:
                        y_pos -= kFpPoint1_6FD824;
                        x_pos -= kFpPoint05_6FD9B0;
                        outAng = pCar->mCarSprite->mRotation;
                        break;
                    case 2:
                        y_pos -= kFpPoint1_6FD824;
                        outAng = pCar->mCarSprite->mRotation;
                        break;
                    case 3:
                        y_pos -= kFpPoint1_6FD824;
                        x_pos += kFpPoint05_6FD9B0;
                        outAng = pCar->mCarSprite->mRotation;
                        break;
                    case 4:
                        y_pos -= kFpPoint05_6FD9B0;
                        x_pos -= kFpPoint1_6FD824;
                        outAng = pCar->mCarSprite->mRotation;
                        break;
                    case 5:
                        y_pos -= kFpPoint05_6FD9B0;
                        x_pos -= kFpPoint1_6FD824 + kFpPoint05_6FD9B0;
                        outAng = pCar->mCarSprite->mRotation;
                        break;
                    case 6:
                        y_pos -= kFpPoint05_6FD9B0;
                        x_pos -= kFpPoint2_6FD82C;
                        outAng = pCar->mCarSprite->mRotation;
                        break;
                    case 7:
                        y_pos -= kFpPoint05_6FD9B0;
                        x_pos -= kFpPoint2_6FD82C + kFpPoint05_6FD9B0;
                        outAng = pCar->mCarSprite->mRotation;
                        break;
                    case 8:
                        y_pos -= kFpPoint05_6FD9B0;
                        x_pos -= kFpPoint3_6FD830;
                        outAng = pCar->mCarSprite->mRotation;
                        break;
                    case 9:
                        y_pos -= kFpPoint02_6FD9AC + kFpPoint1_6FD824;
                        x_pos -= kFpPoint04;
                        outAng = pCar->mCarSprite->mRotation;
                        break;
                    case 10:
                        y_pos -= kFpPoint02_6FD9AC + kFpPoint1_6FD824;
                        x_pos -= kFpPoint04;
                        outAng = pCar->mCarSprite->mRotation;
                        break;
                    case 11:
                        y_pos -= kFpPoint02_6FD9AC + kFpPoint1_6FD824;
                        x_pos -= kFpPoint04;
                        outAng = pCar->mCarSprite->mRotation;
                        break;
                    case 12:
                        y_pos -= kFpPoint02_6FD9AC + kFpPoint1_6FD824;
                        x_pos -= kFpPoint04;
                        outAng = pCar->mCarSprite->mRotation;
                        break;
                    case 99:
                        x_pos += kFpPoint1_6FD824;
                        outAng = pCar->mCarSprite->mRotation;
                        break;
                }
                break;
            }
            if (!bFront)
            {
                outAng = pCar->mCarSprite->mRotation + kAng90_6FD854;
            }
            else
            {
                outAng = pCar->mCarSprite->mRotation + kAng180_6FD936;
            }
            break;
        case 1:
        case 3:
            x_pos -= offset;
            if (bXOffsetPastTile)
            {
                y_pos -= kFpPoint1_6FD824;
                switch (animPhase)
                {
                    case 0:
                        x_pos += kFpPoint2_6FD82C;
                        break;
                    case 1:
                        x_pos += kFpPoint1_6FD824 + kFpPoint06;
                        break;
                    case 2:
                        x_pos += kFpPoint1_6FD824 + kFpPoint02_6FD9AC;
                        break;
                    case 3:
                        x_pos += kFpPoint08;
                        break;
                    case 4:
                        x_pos += kFpPoint04;
                        break;
                    case 6:
                        x_pos -= kFpPoint04;
                        break;
                    case 7:
                        x_pos -= kFpPoint08;
                        break;
                    default:
                        break;
                }
            }
            else
            {
                switch (animPhase)
                {
                    case 0:
                        y_pos -= kFpPoint1_6FD824;
                        x_pos += kFpPoint1_6FD824;
                        outAng = pCar->mCarSprite->mRotation + kAng180_6FD936;
                        break;
                    case 1:
                        y_pos -= kFpPoint1_6FD824;
                        x_pos += kFpPoint05_6FD9B0;
                        outAng = pCar->mCarSprite->mRotation + kAng180_6FD936;
                        break;
                    case 2:
                        y_pos -= kFpPoint1_6FD824;
                        outAng = pCar->mCarSprite->mRotation + kAng180_6FD936;
                        break;
                    case 3:
                        y_pos -= kFpPoint1_6FD824;
                        x_pos -= kFpPoint05_6FD9B0;
                        outAng = pCar->mCarSprite->mRotation + kAng180_6FD936;
                        break;
                    case 5:
                        y_pos -= kFpPoint05_6FD9B0;
                        x_pos += kFpPoint1_6FD824 + kFpPoint05_6FD9B0;
                        outAng = pCar->mCarSprite->mRotation;
                        break;
                    case 6:
                        y_pos -= kFpPoint05_6FD9B0;
                        x_pos += kFpPoint2_6FD82C;
                        outAng = pCar->mCarSprite->mRotation;
                        break;
                    case 7:
                        y_pos -= kFpPoint05_6FD9B0;
                        x_pos += kFpPoint2_6FD82C + kFpPoint05_6FD9B0;
                        outAng = pCar->mCarSprite->mRotation;
                        break;
                    case 4:
                    case 8:
                        y_pos -= kFpPoint05_6FD9B0;
                        x_pos += kFpPoint1_6FD824;
                        outAng = pCar->mCarSprite->mRotation;
                        break;
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                        y_pos -= kFpPoint1_6FD824;
                        x_pos += kFpPoint04;
                        outAng = pCar->mCarSprite->mRotation + kAng90_6FD854;
                        break;
                    case 99:
                        x_pos -= kFpPoint1_6FD824;
                        outAng = pCar->mCarSprite->mRotation;
                        break;
                }
            }
            if (bXOffsetPastTile)
            {
                outAng = pCar->mCarSprite->mRotation - kAng90_6FD854;
            }
            break;
        default:
            break;
    }
    Ang16::RotateVector(x_pos, y_pos, pCar->mCarSprite->mRotation);
    outX = pCar->mCarSprite->mXy.x + x_pos;
    outY = pCar->mCarSprite->mXy.y + y_pos;
}

MATCH_FUNC(0x45AE40)
EXPORT bool __stdcall abs_sub_less_than_epislon(Fix16 lhs, Fix16 rhs)
{
    if (lhs == rhs)
    {
        return true;
    }

    if (Fix16::Abs(lhs - rhs) < kFpPoint9)
    {
        return true;
    }
    return false;
}

MATCH_FUNC(0x45ae70)
Ped::Ped()
{
    mId = 0;
    Reset();
    mpNext = NULL;
}

MATCH_FUNC(0x45af00)
Ped::~Ped()
{
    this->mPlayer = NULL;
    this->mStolenCar = NULL;
    this->mAttacker = NULL;
    this->mObjectiveTargetPed = NULL;
    this->mInternalTargetPed = NULL;
    this->mTargetObjectiveCar = NULL;
    this->mEscapeCar = NULL;
    this->mTargetToEnter = NULL;
    this->mpNext = NULL;
    this->mPedGroup = NULL;
    this->mGameObject = NULL;
    this->mCar = NULL;
    this->mSelectedWeapon = NULL;
    this->mpWeapon = NULL;
    this->mCarThief = NULL;
    this->mCurrentPathPoint = NULL;
    this->mBf21C.bInPathList = false;
    this->mpObj2C = NULL;
    this->mInvulnerability = 0;
    this->mKillerId = 0;
    this->mDeathCause = ped_death_cause::none_0;
    this->mKillerIdTimer = 0;
    this->mElectrocutionTimer = 0;
    this->mHitTargetPed = NULL;
    this->mObjectiveTargetObject = NULL;
    this->mInternalTargetObject = NULL;
}

// https://decomp.me/scratch/2yWEK
MATCH_FUNC(0x45afc0)
void Ped::Reset()
{
    mBf21C.bActive = false;
    mBf21C.bUnused1 = false;
    mLifetimeTimer = ped_lifetime::remove_0;
    mPedType = ped_type::dummy_3;
    mGroupIdx = 0;
    mOccupation = ped_ocupation_enum::dummy;
    mHealth = 0;
    mOffscreenCounter = 0;
    mRemap = -1;
    mCam.x = kFpZero_678660;
    mCam.y = kFpZero_678660;
    mCam.z = kFpZero_678660;
    mFacingAngle = gDummyPedAng;
    mEnterCarAsPassenger = 1;
    mTargetCarDoor = 0;
    mStolenCar = NULL;
    mPedClass = ped_class::gang_dummy_0;
    mJumpOverMode = ped_jump_over_mode::cannot_jump_1;
    mFireMode = ped_fire_mode::normal_1;
    mAimAngle = gDummyPedAng;
    mBf21C.bPanicking = false;
    mAttacker = NULL;
    mTargetFacingAngle = -gPedAng;
    mObjectiveStatus = objective_status::not_finished_0;
    mInternalObjectiveStatus = objective_status::not_finished_0;
    mObjective = objectives_enum::no_obj_0;
    mInternalObjective = objectives_enum::no_obj_0;
    mObjectiveTimer = 9999;
    mInternalObjectiveTimer = 9999;
    mTargetX = Fix16(-1);
    mTargetY = Fix16(-1);
    mTargetZ = Fix16(-1);
    mMoveTargetX = Fix16(-1);
    mMoveTargetY = Fix16(-1);
    mMoveTargetZ = Fix16(-1);
    mObjectiveTargetPed = NULL;
    mInternalTargetPed = NULL;
    mTargetObjectiveCar = NULL;
    mTargetToEnter = NULL;
    mEscapeCar = NULL;
    mUnused227 = 0;
    mHitCount = 0;
    mPlayer = NULL;
    mPedState1 = ped_state_1::walking_0;
    mPedState2 = ped_state_2::ped2_walking_0;
    mPedGroup = NULL;
    mGameObject = NULL;
    mCar = NULL;
    mSelectedWeapon = NULL;
    mpWeapon = NULL;
    mCarWeapon = NULL;
    mBf21C.bAttacking = false;
    mBf21C.bUnused12 = false;
    mBf21C.bWeaponChosen = false;
    mpGang = NULL;
    mAttackersCount = 0;
    mPrevAttackersCount = 0;
    mCarThief = NULL;
    mKillerId = 0;
    mDeathCause = ped_death_cause::none_0;
    mKillerIdTimer = 0;
    mBf21C.bInPathList = false;
    mPatrolPoints[0].mX = 0;
    mPatrolPoints[0].mY = 0;
    mUnused261 = 0;
    mCurrentPathPoint = NULL;
    mBf21C.bFindingPath = false;
    mBf21C.bPathSteppedThisRound = false;
    mPatrolRoute = NULL;
    mCurrentPatrolPoint = NULL;
    mPatrolListIdx = 0;
    mInternalTargetX = kFpZero_678660;
    mInternalTargetY = kFpZero_678660;
    mInternalTargetZ = kFpZero_678660;
    mObjectiveTargetX = kFpZero_678660;
    mObjectiveTargetY = kFpZero_678660;
    mObjectiveTargetZ = kFpZero_678660;
    mRotation = gDummyPedAng;
    mTargetDistanceLimit = kFpZero_678660;
    mUnused1EC = kFpZero_678660.mValue;
    mpObj2C = NULL;
    mPedRefIdx = 0;
    mStoredPedState1 = 11;
    mStoredPedState2 = ped_state_2::initial_28;
    mInvulnerability = 0;
    mWantedPoints = cop_level_ped_enum::cop_0_stars;
    mTargetUnseenCounter = 0;
    mThreatSearch = threat_search_enum::area_2;
    mThreatReaction = threat_reaction_enum::run_away_3;
    mLastCharPunched = NULL;
    mPathPointCount = 0;
    mBf21C.bNoPathFound = false;
    mBf21C.bUnused18 = false;
    mBf21C.bLosCheckedThisRound = false;
    mBf21C.bUnused20 = false;
    mBf21C.bUnused21 = false;
    mBf21C.bHoldPosition = false;
    mBf21C.bSpottedPlayer = false;
    mBf21C.bOnFire = false;
    mBf21C.bInvisible = false;
    mBf21C.bElectroFingers = false;
    mBf21C.bLeftVehicle = false;
    mShockCounter = 0;
    mElectrocutionThreshold = 100;
    mWalkSpeed = kFpOneThirtySecond;
    mMaxSpeed = kFpOneSixteenth_678448;
    mElectrocutionTimer = 0;
    mHitTargetPed = NULL;
    mDummyGang = NULL;
    sKeepPaceWithTarget = 0;
    mBf21C.bDriveAggressively = false;
    mRunSpeed = kFpPoint8_6784A0;
    mObjectiveTargetObject = NULL;
    mInternalTargetObject = NULL;
    mFollowCarOffsetAngle = gDummyPedAng;
    mFollowCarOffsetDistance = kFpZero_678660.mValue;
    mUnused269 = -1;
    mUnused214 = 0;
    mRecentCrimeTimer = 0;
    mBf21C.bStayInCar = false;
    mGraphicType = ped_graphic_type::character_1;
    mBf21C.bBusted = false;
    mBf21C.bSkipCarSearch = false;
    mVoiceEvent = 0;
    mBf224.bUnused0 = false;
    mBf224.bUnused1 = false;
    mBf224.bCollidedWithPed = false;
    mBf224.bUnused3 = false;
    mCollidedCharB4 = 0;
    mpTrainStation = NULL;
    mLastVoiceRng = 0;
    mBf21C.bUseCarWeapon = false;
    mBf21C.bHitByAttacker = false;
    mBf21C.bNoWeapon = false;
    mMugCount = 0;
    mBf21C.bScheduledForRemoval = false;
    mGangCarModel = car_model_enum::MERC;
    mPedKiller = NULL;
    mBf224.bWaitingOnTarget = false;
    mBf21C.bArmedGangMember = false;
    mBf21C.bForcedOutOfTaxi = false;
    mWaitCounter = 0;
    mBf224.bDropsWeaponOnDeath = true;
}

MATCH_FUNC(0x45b440)
void Ped::PoolAllocate()
{
    Ped::Reset();
    mId = gPedId++;
    mBf21C.bActive = true;
    mLifetimeTimer = ped_lifetime::never_expires_99;

    switch (mOccupation)
    {
        case ped_ocupation_enum::mugger:
            gNumberMuggersSpawned = 0;
            break;
        case ped_ocupation_enum::car_thief:
            gNumberCarThiefsSpawned = 0;
            break;
        case ped_ocupation_enum::elvis:
            gNumberElvisLeadersSpawned = 0;
            break;
        case ped_ocupation_enum::walking_guard_29:
            gNumberWalkingCopsSpawned = 0;
            break;
    }
}

MATCH_FUNC(0x45b4e0)
bool Ped::IsLawEnforcement() const
{
    switch (mOccupation)
    {
        case ped_ocupation_enum::police:
        case ped_ocupation_enum::swat:
        case ped_ocupation_enum::fbi:
        case ped_ocupation_enum::army_army:
        case ped_ocupation_enum::walking_guard_29:
        case ped_ocupation_enum::walking_swat_guard_30:
        case ped_ocupation_enum::walking_army_guard_31:
        case ped_ocupation_enum::roadblock_cop_37:
            return true;
        default:
            return false;
    }
    return false;
}

MATCH_FUNC(0x45b520)
Fix16_Point Ped::GetVelocityVector() const
{
    return Fix16_Point(mGameObject->mVelocityVector.x, mGameObject->mVelocityVector.y);
}

MATCH_FUNC(0x45b550)
void Ped::SetRecentCrimeTimer()
{
    mRecentCrimeTimer = 2;
}

MATCH_FUNC(0x45b560)
void Ped::SetPlayer(Player* pPlayer, bool bIsAuxPed)
{
    mPlayer = pPlayer;
    mId = pPlayer->mPlayerIdx + 1;
    if (bIsAuxPed)
    {
        mId += 6;
    }
}

MATCH_FUNC(0x45b590)
bool Ped::IsEmergencyOccupation() const
{
    if (mOccupation >= ped_ocupation_enum::paramedic_23 && (mOccupation <= ped_ocupation_enum::army_army || mOccupation == ped_ocupation_enum::roadblock_cop_37))
    {
        return true;
    }
    return false;
}

MATCH_FUNC(0x45b5b0)
void Ped::CopyStatsFromPed(const Ped* pSrc)
{
    mBf21C.bActive = pSrc->mBf21C.bActive;
    mBf21C.bUnused1 = pSrc->mBf21C.bUnused1;
    mLifetimeTimer = pSrc->mLifetimeTimer;
    mPedType = pSrc->mPedType;
    mPlayer = pSrc->mPlayer;
    mGroupIdx = pSrc->mGroupIdx;
    mOccupation = pSrc->mOccupation;
    mPedState1 = pSrc->mPedState1;
    mPedState2 = pSrc->mPedState2;
    mHealth = pSrc->mHealth;
    mOffscreenCounter = pSrc->mOffscreenCounter;
    mRemap = pSrc->mRemap;
    mBf21C.bAttacking = pSrc->mBf21C.bAttacking;
    mCam.x = pSrc->mCam.x;
    mCam.y = pSrc->mCam.y;
    mCam.z = pSrc->mCam.z;
    mFacingAngle = pSrc->mFacingAngle;
    mEnterCarAsPassenger = pSrc->mEnterCarAsPassenger;
    mTargetCarDoor = pSrc->mTargetCarDoor;
    mStolenCar = pSrc->mStolenCar;
    mPedClass = pSrc->mPedClass;
    mJumpOverMode = pSrc->mJumpOverMode;
    mFireMode = pSrc->mFireMode;
    mThreatReaction = pSrc->mThreatReaction;
    mBf21C.bPanicking = pSrc->mBf21C.bPanicking;
    mAttacker = pSrc->mAttacker;
    mTargetFacingAngle = pSrc->mTargetFacingAngle;
    mObjectiveStatus = pSrc->mObjectiveStatus;
    mInternalObjectiveStatus = pSrc->mInternalObjectiveStatus;
    mObjective = pSrc->mObjective;
    mInternalObjective = pSrc->mInternalObjective;
    mObjectiveTimer = pSrc->mObjectiveTimer;
    mInternalObjectiveTimer = pSrc->mInternalObjectiveTimer;
    mTargetX = pSrc->mTargetX;
    mTargetY = pSrc->mTargetY;
    mTargetZ = pSrc->mTargetZ;
    mMoveTargetX = pSrc->mMoveTargetX;
    mMoveTargetY = pSrc->mMoveTargetY;
    mMoveTargetZ = pSrc->mMoveTargetZ;
    mObjectiveTargetPed = pSrc->mObjectiveTargetPed;
    mInternalTargetPed = pSrc->mInternalTargetPed;
    mTargetObjectiveCar = pSrc->mTargetObjectiveCar;
    mObjectiveTargetObject = pSrc->mObjectiveTargetObject;
    mInternalTargetObject = pSrc->mInternalTargetObject;
    mEscapeCar = pSrc->mEscapeCar;
    mTargetToEnter = pSrc->mTargetToEnter;
    mUnused227 = pSrc->mUnused227;
    mHitCount = pSrc->mHitCount;
    mPedGroup = pSrc->mPedGroup;
    mGameObject = pSrc->mGameObject;
    mCar = pSrc->mCar;
    mSelectedWeapon = pSrc->mSelectedWeapon;
    mpWeapon = pSrc->mpWeapon;
    mpGang = pSrc->mpGang;
    mAttackersCount = pSrc->mAttackersCount;
    mPrevAttackersCount = pSrc->mPrevAttackersCount;
    mCarThief = pSrc->mCarThief;
    mPatrolPoints[0].mX = 0;
    mPatrolPoints[0].mY = 0;
    mUnused261 = pSrc->mUnused261;
    mCurrentPathPoint = pSrc->mCurrentPathPoint;
    mBf21C.bFindingPath = pSrc->mBf21C.bFindingPath;
    mBf21C.bPathSteppedThisRound = pSrc->mBf21C.bPathSteppedThisRound;
    mInternalTargetX = pSrc->mInternalTargetX;
    mInternalTargetY = pSrc->mInternalTargetY;
    mInternalTargetZ = pSrc->mInternalTargetZ;
    mObjectiveTargetX = pSrc->mObjectiveTargetX;
    mObjectiveTargetY = pSrc->mObjectiveTargetY;
    mObjectiveTargetZ = pSrc->mObjectiveTargetZ;
    mpObj2C = pSrc->mpObj2C;
    mPedRefIdx = pSrc->mPedRefIdx;
    mStoredPedState1 = pSrc->mStoredPedState1;
    mStoredPedState2 = pSrc->mStoredPedState2;
    mInvulnerability = pSrc->mInvulnerability;
    mBf21C.bNoPathFound = pSrc->mBf21C.bNoPathFound;
    mWantedPoints = pSrc->mWantedPoints;
    mBf21C.bOnFire = pSrc->mBf21C.bOnFire;
    mBf21C.bInvisible = pSrc->mBf21C.bInvisible;
    mBf21C.bElectroFingers = pSrc->mBf21C.bElectroFingers;
    mMaxSpeed = pSrc->mMaxSpeed;
    mWalkSpeed = pSrc->mWalkSpeed;
    DrawFlamesAndStartScreamTimer();
    SetSpriteSemiTransIfInvisible();
    SetSpriteFlagIfInvulnerable();
    mGraphicType = pSrc->mGraphicType;
    mVoiceEvent = pSrc->mVoiceEvent;
    mBf224.bUnused0 = pSrc->mBf224.bUnused0;
    mBf224.bUnused1 = pSrc->mBf224.bUnused1;
    mBf224.bCollidedWithPed = pSrc->mBf224.bCollidedWithPed;
    mBf224.bUnused3 = pSrc->mBf224.bUnused3;
    mCollidedCharB4 = pSrc->mCollidedCharB4;
    mpTrainStation = pSrc->mpTrainStation;
    mRecentCrimeTimer = pSrc->mRecentCrimeTimer;
    mBf21C.bStayInCar = pSrc->mBf21C.bStayInCar;
    mGraphicType = pSrc->mGraphicType;
    mBf21C.bBusted = pSrc->mBf21C.bBusted;
    mBf21C.bSkipCarSearch = pSrc->mBf21C.bSkipCarSearch;
    mBf21C.bUseCarWeapon = pSrc->mBf21C.bUseCarWeapon;
    mBf21C.bHitByAttacker = pSrc->mBf21C.bHitByAttacker;
    mBf21C.bNoWeapon = pSrc->mBf21C.bNoWeapon;
    mMugCount = pSrc->mMugCount;
    mBf21C.bScheduledForRemoval = pSrc->mBf21C.bScheduledForRemoval;
    mGangCarModel = pSrc->mGangCarModel;
    mPedKiller = pSrc->mPedKiller;
    mBf224.bWaitingOnTarget = pSrc->mBf224.bWaitingOnTarget;
    mBf21C.bArmedGangMember = pSrc->mBf21C.bArmedGangMember;
    mBf21C.bForcedOutOfTaxi = pSrc->mBf21C.bForcedOutOfTaxi;
    mWaitCounter = pSrc->mWaitCounter;
}

MATCH_FUNC(0x45bbf0)
Car* Ped::GetCarBeingEnteredOrExited() const
{
    if (mPedState2 == ped_state_2::ped2_entering_a_car_6 || mPedState2 == ped_state_2::ped2_getting_out_a_car_7)
    {
        return mTargetToEnter;
    }
    else
    {
        return NULL;
    }
}

MATCH_FUNC(0x45bc10)
void Ped::TeleportToCoord(Fix16 xpos, Fix16 ypos)
{
    Fix16 zpos = gMap_0x370->FindGroundZForCoord(xpos, ypos);
    Car* pCar = get_car();
    if (pCar)
    {
        pCar->SetPosition(xpos, ypos, zpos);
    }
    else
    {
        mGameObject->Teleport(xpos, ypos, zpos);
    }
}

MATCH_FUNC(0x45bc70)
void Ped::ManageShocking()
{
    if (mPedState1 != ped_state_1::dead_9)
    {
        if (mShockCounter >= mElectrocutionThreshold)
        {
            if (mGameObject)
            {
                if (mGameObject->GetCharState() != Char_B4_state::Jumping_15 &&
                    mPedState1 != ped_state_1::immobilized_8)
                {
                    ChangeNextPedState1(ped_state_1::immobilized_8);
                    ChangeNextPedState2(ped_state_2::electrocuted_27);
                    SetStateInitPending();
                }

                if (mShockCounter > 0)
                {
                    --mShockCounter;
                }
            }
            else
            {
                mShockCounter = 0;
            }
        }
        else if (mShockCounter > 0)
        {
            --mShockCounter;
        }

        if (mGameObject)
        {
            if (mBf21C.bElectroFingers)
            {
                ShockPedsAroundSprite(mGameObject->get_sprite_ptr(), kFpHalf_67853C, 0, 1, this, NULL);
            }
        }
    }
}

MATCH_FUNC(0x45bd20)
bool Ped::CloseDoorIfCarApproaching(Car* pCar)
{
    if (pCar == mTargetToEnter || pCar->GetVelocity() < kFpPoint01_678624)
    {
        return true;
    }
    Car_Door* pDoor = mTargetToEnter->GetDoor(mTargetCarDoor);
    pDoor->Close();

    mGameObject->HandleGenericImpact(kAng45_6784FC + pCar->mCarSprite->mRotation,
                                     kFpPoint05_678634 + kFpPoint1_678480,
                                     kFpZero_678660,
                                     1);
    Ped::ChangeNextPedState1(ped_state_1::walking_0);
    Ped::ChangeNextPedState2(ped_state_2::ped2_walking_0);
    return false;
}

MATCH_FUNC(0x45be30)
s32 Ped::GetBulletSpriteOffset() const
{
    Ped* pLeader;
    Player* pPlayer;
    if ((mPlayer && mPlayer->IsUser()) ||
        (mPedGroup && (pLeader = mPedGroup->mPedLeader) != NULL &&
         (pPlayer = pLeader->mPlayer) != NULL && pPlayer->IsUser()))
    {
        return 0;
    }
    else
    {
        return 2;
    }
}

MATCH_FUNC(0x45be70)
void Ped::SetOnFire()
{
    if (!mBf21C.bOnFire)
    {
        mBf21C.bOnFire = true;
        DrawFlamesAndStartScreamTimer();
    }
}

MATCH_FUNC(0x45be90)
void Ped::PutOutFire()
{
    if (this->mBf21C.bOnFire)
    {
        Char* pChar = this->mGameObject;
        if (pChar)
        {
            pChar->RemoveFireSprites();
        }
        this->mBf21C.bOnFire = false;
    }
}

MATCH_FUNC(0x45bec0)
void Ped::ManageBurning()
{
    if (mBf21C.bOnFire)
    {
        if (mInvulnerability > 0)
        {
            PutOutFire();
        }
        else if (mCar)
        {
            PutOutFire();
            mCar->HandleCarExplosion(explosion_type::item_19);
        }
        else
        {
            const bool bWasDead = isDead();
            TakeDamage(1);
            mKillerIdTimer = 50;

            if (isDead() && !bWasDead)
            {
                Player* pPlayer = mPlayer;
                if (pPlayer)
                {
                    pPlayer->SetDeathType(2);
                }
            }

            if (!is_player())
            {
                if (mGameObject)
                {
                    if (mInternalObjective != objectives_enum::flee_on_foot_till_safe_1)
                    {
                        mBf21C.bPanicking = true;

                        SetObjective2(objectives_enum::flee_on_foot_till_safe_1);

                        mInternalTargetX = mCam.x;
                        mInternalTargetY = mCam.y;
                        mInternalTargetZ = mCam.z;
                    }
                }
            }
        }
    }
}

MATCH_FUNC(0x45bfb0)
void Ped::DrawFlamesAndStartScreamTimer()
{
    if (mBf21C.bOnFire)
    {
        Char* pChar = mGameObject;
        if (pChar)
        {
            pChar->DrawFlamesAndStartScreamTimer();
        }
    }
}

MATCH_FUNC(0x45bfd0)
void Ped::SetInvisible()
{
    mBf21C.bInvisible = true;
    SetSpriteSemiTransIfInvisible();
}

MATCH_FUNC(0x45bfe0)
void Ped::SetVisible()
{
    this->mBf21C.bInvisible = false;
    Char* pChar = this->mGameObject;
    if (pChar)
    {
        pChar->mSpritePtr->mFlags = 0; // make sprite opaque
    }
}

MATCH_FUNC(0x45c010)
void Ped::SetSpriteSemiTransIfInvisible()
{
    if (this->mBf21C.bInvisible)
    {
        Char* pChar = this->mGameObject;
        if (pChar)
        {
            pChar->mSpritePtr->mFlags = 0x41; // make sprite semi transparent
        }
    }
}

MATCH_FUNC(0x45c040)
void Ped::SetInvulnerable()
{
    mInvulnerability = 9999;
    SetSpriteFlagIfInvulnerable();
}

MATCH_FUNC(0x45c050)
void Ped::ClearInvulnerable()
{
    mInvulnerability = 0;
    Char* pChar = mGameObject;
    if (pChar)
    {
        pChar->mSpritePtr->mBf2C.bNoLighting = false;
    }
}

MATCH_FUNC(0x45c070)
void Ped::SetSpriteFlagIfInvulnerable()
{
    if (this->mInvulnerability == 9999)
    {
        Char* pChar = this->mGameObject;
        if (pChar)
        {
            pChar->mSpritePtr->mBf2C.bNoLighting = true;
        }
    }
}

MATCH_FUNC(0x45c090)
void Ped::RestoreCarOrPedHealth()
{
    Car* pCar = mCar;
    if (pCar)
    {
        pCar->RemoveAllDamage();
    }
    else if (mPedState1 != ped_state1_enum::ped_wasted)
    {
        mHealth = 100;
    }
}

MATCH_FUNC(0x45c0c0)
void Ped::SpawnCharInZone(gmp_map_zone* pZone)
{
    u8 spiral_side = 0;

    u8 center_x = pZone->mX + (pZone->mW >> 1);
    u8 center_y = pZone->mY + (pZone->mH >> 1);
    u8 x = center_x;
    u8 y = center_y;

    s32 found_z;

    u8 x_run_len = 1;
    u8 y_run_len = 1;
    u8 x_steps_left = 1;
    u8 y_steps_left = 1;

    while (true)
    {
        if (gMap_0x370->FindHighestBlockForCoord(x, y, &found_z))
        {
            break;
        }
        switch (spiral_side)
        {
            case 0:
                if (!--x_steps_left)
                {
                    spiral_side = 1;
                    y_steps_left = y_run_len;
                }
                if (--x < center_x - (pZone->mW >> 1))
                {
                    ++x;
                }
                break;
            case 1:
                --y_steps_left;
                if (!y_steps_left)
                {
                    spiral_side = 2;
                    x_steps_left = ++x_run_len;
                }
                if (--y < center_y - (pZone->mH >> 1))
                {
                    ++y;
                }
                break;
            case 2:
                if (!--x_steps_left)
                {
                    ++y_run_len;
                    spiral_side = 3;
                    y_steps_left = y_run_len;
                }
                if (++x > center_x + (pZone->mW >> 1))
                {
                    ++x;
                }
                break;
            case 3:
                --y_steps_left;
                if (!y_steps_left)
                {
                    spiral_side = 0;
                    x_steps_left = x_run_len;
                }
                if (++y > center_y + (pZone->mH >> 1))
                {
                    --y;
                }
                break;
            default:
                continue;
        }
    }
    AllocCharB4(Fix16(x) + kFpHalf_67853C, Fix16(y) + kFpHalf_67853C, found_z + 1);
}

MATCH_FUNC(0x45c310)
void Ped::PoolDeallocate()
{
    if (mGameObject)
    {
        gChar_B4_Pool->DeAllocate(mGameObject);
        mGameObject = NULL;
    }
}

MATCH_FUNC(0x45c350)
void Ped::RespawnPed(gmp_map_zone* pZone)
{
    if (mGameObject)
    {
        gChar_B4_Pool->DeAllocate(mGameObject);
    }
    mGameObject = NULL;

    mCar = NULL;

    if (mPedGroup)
    {
        mPedGroup->DestroyGroup();
    }

    SpawnCharInZone(pZone);

    SetRemap(get_remap());

    mPedState2 = ped_state_2::ped2_respawned_29;
    set_health(100);
    mInvulnerability = 50;

    SetObjective(objectives_enum::no_obj_0);
    SetObjective2(objectives_enum::no_obj_0);
}

MATCH_FUNC(0x45c410)
void Ped::ResetForPlayerRespawn()
{
    Char* pChar = this->mGameObject;
    Player* pPlayer = this->mPlayer;

    PutOutFire();
    const u8 remap = this->mRemap;
    Reset();

    this->mRemap = remap;
    this->mGameObject = pChar;

    this->mBf21C.bActive = true;

    this->mCam.x = pChar->mSpritePtr->mXy.x;
    this->mCam.y = pChar->mSpritePtr->mXy.y;
    this->mCam.z = pChar->mSpritePtr->mZpos;

    this->set_health(100);
    this->SetPedType(ped_type::player_2);
    this->mInvulnerability = 50;
    this->mLifetimeTimer = ped_lifetime::never_expires_99;
    this->mPlayer = pPlayer;
}

MATCH_FUNC(0x45c4b0)
void Ped::UpdatePositionFromCar()
{
    mCam.x = mCar->mCarSprite->mXy.x;
    mCam.y = mCar->mCarSprite->mXy.y;
    mCam.z = mCar->mCarSprite->mZpos;
}

MATCH_FUNC(0x45c500)
void Ped::ChangeNextPedState1(s32 new_state)
{
    // If the ped is immobilized, store the new ped state for later use
    if (mPedState1 != ped_state1_enum::ped_fall_on_ground)
    {
        // Ped is currently not immobilized
        if (new_state == ped_state1_enum::ped_fall_on_ground)
        {
            // Ped now must be immobilized, so store the previous state
            mStoredPedState1 = mPedState1;
        }
        mPedState1 = new_state; // update state
    }
    else if (new_state != ped_state1_enum::ped_fall_on_ground)
    {
        // current state of the ped is immobilized, so store the new state (if not the same "immobilized")
        mStoredPedState1 = new_state;
    }
}

MATCH_FUNC(0x45c540)
void Ped::ChangeNextPedState2(s32 new_state)
{
    if (mPedState1 != ped_state_1::immobilized_8)
    {
        mPedState2 = new_state; // just update, dont store previous
    }
    else if (mPedState2 < ped_state_2::ped2_at_car_door_17 || mPedState2 > ped_state_2::initial_28)
    {
        mStoredPedState2 = mPedState2; // store previous state
        mPedState2 = new_state; // update
    }
    else if (new_state < ped_state_2::Unknown_23 || new_state > ped_state_2::knocked_fatal_26)
    {
        mStoredPedState2 = new_state; // only store the next state, but do not change it yet
    }
    else
    {
        // store previous state & update
        mStoredPedState2 = mPedState2;
        mPedState2 = new_state;
    }
}

MATCH_FUNC(0x45c5a0)
void Ped::RestorePreviousPedState()
{
    mPedState1 = mStoredPedState1;
    mPedState2 = mStoredPedState2;
}

MATCH_FUNC(0x45c5c0)
void Ped::CancelEnterCarObjective()
{
    if (!this->mCar && this->mObjective == objectives_enum::enter_car_as_driver_35 && this->mInternalObjective == objectives_enum::enter_car_as_driver_35 &&
        this->mGameObject->mCharState != Char_B4_state::Jumping_15 &&
        this->mPedState2 != ped_state_2::ped2_entering_a_car_6)
    {
        ChangeNextPedState1(ped_state_1::walking_0);
        ChangeNextPedState2(ped_state_2::ped2_walking_0);
        this->mCar = NULL;
        SetObjective(objectives_enum::no_obj_0);
        SetObjective2(objectives_enum::no_obj_0);
        PedGroup* pGroup = this->mPedGroup;
        if (pGroup)
        {
            pGroup->ResetMembersToFollowLeader();
        }
    }
}

MATCH_FUNC(0x45C650)
void Ped::SpawnDriverRunAway(Car* pCar, Ped* pCarThief)
{
    switch (pCar->mCarInfoIdx)
    {
        case car_model_enum::apc:
        case car_model_enum::bank_van:
        case car_model_enum::boxcar:
        case car_model_enum::boxtruck:
        case car_model_enum::FIRETRUK:
        case car_model_enum::GUNJEEP:
        case car_model_enum::JEEP:
        case car_model_enum::TANK:
        case car_model_enum::TRAIN:
        case car_model_enum::TRAINCAB:
        case car_model_enum::TRAINFB:
            return;
        default:
            if (pCar->GetDoorCount())
            {
                Ped* pPed = gPedManager->SpawnRunAwayGuy();
                pPed->set_occupation(ped_ocupation_enum::scared_driver_50);
                pCar->mDriver = pPed;
                pPed->mCar = pCar;
                pPed->set_target_car_door(0);
                pPed->set_enter_car_as_passenger(0);
                pPed->SetObjective(objectives_enum::leave_car_36);
                pPed->SetTargetObjectiveCar(pPed->mCar);
                pPed->mCarThief = pCarThief;
                pPed->mThreatReaction = threat_reaction_enum::run_away_3;
            }
            break;
    }
}

MATCH_FUNC(0x45c730)
void Ped::SpawnPedInCar(Car* pCar)
{
    this->mCar = pCar;
    this->mEnterCarAsPassenger = 0;
    this->mTargetCarDoor = 0;
    this->mPedState1 = ped_state_1::in_car_10;

    pCar->SetDriver(this);
    pCar->SetDriverPedType(mPedType);

    pCar->CancelDespawn();
}

MATCH_FUNC(0x45c7a0)
void Ped::EnterCarByBackDoor(Car* pCar)
{
    SetObjective2(objectives_enum::no_obj_0);
    SetObjective(objectives_enum::enter_car_as_driver_35);
    mEnterCarAsPassenger = 1;
    mTargetObjectiveCar = pCar;
    mTargetCarDoor = pCar->GetDoorCount() - 1;
}

MATCH_FUNC(0x45c7f0)
void Ped::EnterCarAsPassenger(Car* pCar)
{
    this->mEnterCarAsPassenger = 1;
    this->mCar = pCar;
    this->mTargetCarDoor = 1;
    this->mPedState1 = ped_state_1::in_car_10;
    this->mPedState2 = ped_state_2::ped2_driving_10;
    pCar->mPassengersList.AddPedToFront(this);
}

MATCH_FUNC(0x45c830)
bool Ped::AllocCharB4(Fix16 xpos, Fix16 ypos, Fix16 zpos)
{
    Char* pChar = gChar_B4_Pool->mPool.Allocate();

    mGameObject = pChar;
    if (!pChar)
    {
        return false;
    }

    Sprite* pSprite = pChar->mSpritePtr;
    pSprite->set_xyz_lazy(xpos, ypos, zpos);

    pChar->mSpritePtr->AllocCollisionShape(gDummyW, gDummyW, gDummyZ);

    gSpriteGrid_1->AddToRegionBuckets(pChar->mSpritePtr);
    mGameObject->set_pPed(this);

    mCam.y = ypos;
    mCam.x = xpos;
    mCam.z = zpos;

    DrawFlamesAndStartScreamTimer();
    SetSpriteSemiTransIfInvisible();
    SetSpriteFlagIfInvulnerable();

    return true;
}

MATCH_FUNC(0x45c900)
Ang16 Ped::GetTurnSpeed() const
{
    return mPlayer->mTurnSpeed;
}

MATCH_FUNC(0x45c920)
Fix16 Ped::GetPedVelocity() const
{
    if (mGameObject)
    {
        return mGameObject->get_velocity();
    }
    else
    {
        if (mCar)
        {
            return mCar->GetVelocity();
        }
        return kFpZero_678660;
    }
}

MATCH_FUNC(0x45c960)
Ang16 Ped::GetRotation() const
{
    if (mGameObject != NULL)
    {
        return mGameObject->get_rotation();
    }

    if (mCar != NULL)
    {
        return mCar->mCarSprite->mRotation;
    }

    return gDummyPedAng;
}

MATCH_FUNC(0x45c9b0)
Fix16 Ped::GetMoveDirection() const
{
    return mPlayer->mMoveDirection;
}

// 9.6f 0x43E3A0
MATCH_FUNC(0x45c9d0)
Ang16 Ped::ComputeAimAngle()
{
    if (PedTypeIs(ped_type::player_2))
    {
        Ped* best = gThreateningPedsList.FindClosestPedInViewCone(this->mCam.x,
                                                                  this->mCam.y,
                                                                  this->mFacingAngle,
                                                                  kAng16_6784E4);

        if (!best)
        {
            gSearchConeHalfAngle = kAng16_6784E4;
            best = FindBestTargetPed_Mode4(3);
        }

        if (!best)
        {
            best = FindNearestPed_Mode4(3u);
        }

        // mTargetFacingAngle stored in each arm, as 9.6f does; the 21-byte return tail is jumped to, not copied
        if (best)
        {
            mTargetFacingAngle = Fix16::atan2_fixed(best->mCam.y - mCam.y,
                                                  best->mCam.x - mCam.x);
        }
        else
        {
            mTargetFacingAngle = mFacingAngle;
        }
    }
    return mTargetFacingAngle;
}

MATCH_FUNC(0x45caa0)
void Ped::HandleClosePedInteraction()
{
    s16 rng;

    Ped* pNearPed = FindNearbyPed();
    if (pNearPed)
    {
        if (abs_sub_less_than_epislon(this->mCam.z, pNearPed->get_cam_z()))
        {
            this->mLastCharPunched = pNearPed;
            PedGroup* pNearPedGroup = pNearPed->mPedGroup;
            if (!pNearPedGroup || pNearPedGroup != this->mPedGroup)
            {
                pNearPed->mAttacker = this;
                if (this->mGameObject->mAnimationFrame >= 3u)
                {
                    goto react_to_punch;
                }
                if (this->mOccupation == ped_ocupation_enum::mugger)
                {
                    SetVoiceEventIfNotOnFire(8);
                    pNearPed->SetVoiceEventIfNotOnFire(7);
                }
                else
                {
                    pNearPed->TriggerVoiceEventRateLimited();
                }
                pNearPed->mKillerId = this->mId;
                pNearPed->mDeathCause = ped_death_cause::punched_10;
                pNearPed->mKillerIdTimer = 50;
                rng = gRng.get_int(20);
                if (pNearPed->get_health() > 30)
                {
                    if (pNearPed->PedTypeIs(ped_type::player_2))
                    {
                        if (this->mOccupation == ped_ocupation_enum::mugger)
                        {
                            pNearPed->mPlayer->AddScore(-10);
                            this->mMugCount++;
                            if (mMugCount > 9u)
                            {
                                this->mInternalObjectiveStatus = objective_status::passed_1;
                            }
                        }
                        else if (pNearPed->mOccupation != ped_ocupation_enum::criminal_type_1)
                        {
                            pNearPed->TakeDamage(10);
                        }
                    }
                    else if (pNearPed->mOccupation != ped_ocupation_enum::criminal_type_1)
                    {
                        pNearPed->TakeDamage(20);
                    }

                    if (rng >= 2)
                    {
                        goto react_to_punch;
                    }
                }
                if (PedTypeIs(ped_type::player_2))
                {
                    pNearPed->ChangeNextPedState2(ped_state_2::lying_on_floor_22);
                    pNearPed->ChangeNextPedState1(ped_state_1::immobilized_8);
                    pNearPed->SetStateInitPending();
                    pNearPed->mGameObject->SetCharState(Char_B4_state::Hit_From_Front_33);
                }
                else
                {
                // shared with the frame >= 3 and rng >= 2 exits: goto-free forms change the code
                react_to_punch:
                    if (pNearPed->mThreatReaction == threat_reaction_enum::run_away_3)
                    {
                        pNearPed->SetObjective2(objectives_enum::flee_char_on_foot_till_safe_2);
                        pNearPed->SetInternalTargetPed(this);
                        this->mBf21C.bPanicking = true;
                    }
                }
            }
        }
    }
    else
    {
        this->mLastCharPunched = NULL;
    }
}

MATCH_FUNC(0x45ce50)
void Ped::TakeDamage(s16 damage)
{
    if (mInvulnerability <= 0)
    {
        mHealth -= damage;
        if (mHealth <= 0)
        {
            if (mPedState1 != ped_state_1::immobilized_8)
            {
                mHealth = 0;
                Kill();
            }
            else
            {
                if (mHealth < 0)
                {
                    mHealth = 0;
                }
            }
        }
    }
}

MATCH_FUNC(0x45cf20)
void Ped::HandlePedCrossingTrigger(const Object* pObj)
{
    if (mPedState1 != ped_state_1::dead_9 && mPedState1 != ped_state_1::immobilized_8 && pObj->mModel == objects::ped_crossing_trigger_258 &&
        gNumPedsCrossingRoad < 10)
    {
        StartCrossingRoad();
    }
}

Fix16 __stdcall MaxAbsDistance_4614E0(Fix16& x1, Fix16& y1, Fix16& x2, Fix16& y2);

// 10.5 inlines this into HandlePedHitByObject (the first call there; the second one
// calls 0x465D00) and into IsThreatToSearchingPed (search mode 1), so it has to be defined
// before them. 0x465D00 is the out-of-line copy. IsInvisibleOnFoot_433DA0 is called here (not written
// out as in the copy): 4661F0's nested budget (42) sends it out of line, 45D000's (71) inlines it.
inline bool Ped::IsPedAThreat_Inline_465D00(Ped* pTargetPed)
{
    char_type bThisIsActivePlayer = 0;

    if (pTargetPed->IsInvisibleOnFoot_433DA0())
    {
        return false;
    }

    if (this->mThreatSearch == threat_search_enum::area_player_threat_only_3 || this->mThreatSearch == threat_search_enum::line_of_sight_player_threat_only_4)
    {
        if (!pTargetPed->PedTypeIs(ped_type::player_2))
        {
            return false;
        }
    }

    if (this->mThreatSearch == threat_search_enum::area_player_only_5 || this->mThreatSearch == threat_search_enum::line_of_sight_player_only_6)
    {
        if (pTargetPed->IsActivePlayerPed())
        {
            return true;
        }
    }

    if (this->IsActivePlayerPed())
    {
        bThisIsActivePlayer = 1;
    }

    if (this->mPedGroup == pTargetPed->mPedGroup && this->mPedGroup != NULL)
    {
        return false;
    }

    if (this->mOccupation == ped_ocupation_enum::mad_mugger_40)
    {
        Car* pCar = pTargetPed->mCar;

        if (pCar == NULL)
        {
            return true;
        }

        if (!pCar->IsPoliceCar())
        {
            return true;
        }

        if (pTargetPed->mPedType == ped_type::special_ped_4)
        {
            return false;
        }

        return true;
    }

    {
        Gang* pMyGang = this->mpGang;

        if (pMyGang != NULL)
        {
            Gang* pOtherGang = pTargetPed->mpGang;

            if (pOtherGang != NULL)
            {
                if (pOtherGang == pMyGang)
                {
                    goto ret_if_active_player;
                }

                if (!pMyGang->IsHostileToGang(pOtherGang->mGangIdx))
                {
                    return false;
                }

                if (this->mPedType != ped_type::special_ped_4 && this->mPedType != ped_type::dummy_with_occupation_6)
                {
                    return true;
                }

                if (pTargetPed->mPrevAttackersCount >= 4)
                {
                    return false;
                }

                if (pTargetPed->mAttackersCount >= 4)
                {
                    return false;
                }

                return true;
            }

            if (pMyGang->mHasHighRespect)
            {
                switch (pTargetPed->mOccupation)
                {
                    case ped_ocupation_enum::police:
                    case ped_ocupation_enum::swat:
                    case ped_ocupation_enum::fbi:
                    case ped_ocupation_enum::army_army:
                    case ped_ocupation_enum::walking_guard_29:
                    case ped_ocupation_enum::walking_swat_guard_30:
                    case ped_ocupation_enum::walking_army_guard_31:
                    case ped_ocupation_enum::roadblock_cop_37:
                        return true;
                    default:
                        break;
                }
            }

            if (pTargetPed->GetAttacking())
            {
                if (!pTargetPed->IsActivePlayerPed() && pTargetPed->mOccupation != ped_ocupation_enum::empty)
                {
                    if (pTargetPed->mThreatReaction == threat_reaction_enum::react_as_emergency_1)
                    {
                        goto ret_if_active_player;
                    }

                    if (this->mSelectedWeapon != NULL)
                    {
                        return true;
                    }

                    Fix16 distance;
                    distance = MaxAbsDistance_4614E0(this->mCam.x, this->mCam.y, pTargetPed->get_cam_x(), pTargetPed->get_cam_y());
                    if (distance <= kFpOne_678798)
                    {
                        return true;
                    }

                    goto ret_if_active_player;
                }

                {
                    u8 player_idx = pTargetPed->mPlayer->get_idx();

                    if (this->mpGang->IsRespectNegativeForPlayer(player_idx))
                    {
                        if (gPolice_7B8->mCrewPedOnscreen == 0)
                        {
                            return true;
                        }

                        return false;
                    }

                    return false;
                }
            }

            if (pTargetPed->IsActivePlayerPed() || pTargetPed->mOccupation == ped_ocupation_enum::empty)
            {
                u8 player_idx = pTargetPed->mPlayer->mPlayerIdx;

                if (this->mpGang->IsRespectNegativeForPlayer(player_idx))
                {
                    if (gPolice_7B8->mCrewPedOnscreen == 0)
                    {
                        return true;
                    }
                    return false;
                }

                return false;
            }

            goto ret_if_active_player;
        }
    }

    switch (this->mOccupation)
    {
        case ped_ocupation_enum::criminal_type_1:
            if (pTargetPed->mOccupation == ped_ocupation_enum::criminal_type_1)
            {
                return false;
            }

            goto ret_if_active_player;
        case ped_ocupation_enum::criminal_type_2:
            if (pTargetPed->mOccupation == ped_ocupation_enum::criminal_type_2)
            {
                return false;
            }

            goto ret_if_active_player;
        case ped_ocupation_enum::elvis:
            if (pTargetPed->PedTypeIs(ped_type::player_2))
            {
                return false;
            }

            goto ret_if_active_player;
        case ped_ocupation_enum::police:
        case ped_ocupation_enum::swat:
        case ped_ocupation_enum::fbi:
        case ped_ocupation_enum::army_army:
        case ped_ocupation_enum::walking_guard_29:
        case ped_ocupation_enum::walking_swat_guard_30:
        case ped_ocupation_enum::walking_army_guard_31:
        case ped_ocupation_enum::roadblock_cop_37:
            break;
        default:
            if (this->mPedGroup != NULL)
            {
                if (this->mPedGroup->mPedLeader != pTargetPed->mInternalTargetPed)
                {
                    goto ret_if_active_player;
                }

                if (pTargetPed->GetInternalObjective() == objectives_enum::kill_char_on_foot_20)
                {
                    return true;
                }

                if (pTargetPed->GetInternalObjective() != objectives_enum::punch_char_23)
                {
                    goto ret_if_active_player;
                }

                return true;
            }

            if (pTargetPed->GetAttacking())
            {
                return true;
            }

            goto ret_if_active_player;
    }

    if (pTargetPed->IsActivePlayerPed())
    {
        if (pTargetPed->mGameObject != NULL && pTargetPed->mGameObject->GetCharState() == Char_B4_state::Jumping_15)
        {
            return false;
        }

        if (!((pTargetPed->GetAttacking()) && pTargetPed->mSelectedWeapon != NULL &&
              pTargetPed->mSelectedWeapon->CountsAsArmedAttack()))
        {
            if (pTargetPed->mWantedPoints < cop_level_ped_enum::cop_1_stars && this->mAttacker != pTargetPed && pTargetPed->mRecentCrimeTimer <= 0u)
            {
                return false;
            }
        }

        this->mAttacker = NULL;
        gPolice_7B8->UpdateLastSeenCoordsForCriminal(pTargetPed);

        if (this->mObjective == objectives_enum::patrol_in_car_43)
        {
            if (gPolice_7B8->TryAssignCarCrewToCriminal(this->mCar, pTargetPed))
            {
                return true;
            }

            if (pTargetPed->mWantedPoints < cop_level_ped_enum::cop_1_stars)
            {
                pTargetPed->mWantedPoints = cop_level_ped_enum::cop_1_stars;
            }

            return false;
        }

        if (pTargetPed->mWantedPoints >= cop_level_ped_enum::cop_1_stars)
        {
            return true;
        }

        pTargetPed->mWantedPoints = cop_level_ped_enum::cop_1_stars;
        return true;
    }

{
    s32 target_occupation = pTargetPed->mOccupation;
    switch (target_occupation)
    {
        case ped_ocupation_enum::paramedic_23:
        case ped_ocupation_enum::police:
        case ped_ocupation_enum::swat:
        case ped_ocupation_enum::fbi:
        case ped_ocupation_enum::army_army:
        case ped_ocupation_enum::walking_guard_29:
        case ped_ocupation_enum::walking_swat_guard_30:
        case ped_ocupation_enum::walking_army_guard_31:
        case ped_ocupation_enum::roadblock_cop_37:
        case ped_ocupation_enum::road_block_tank_man:
            goto ret_if_active_player;
        default:
            break;
    }

    if (target_occupation == ped_ocupation_enum::empty)
    {
        return true;
    }

    if (pTargetPed->GetAttacking())
    {
        return true;
    }

    if (pTargetPed->mInternalObjective != objectives_enum::kill_char_on_foot_20)
    {
        goto ret_if_active_player;
    }

    return true;
}

// shared exit: a return of the flag test at each goto changes the code
ret_if_active_player:
    if (bThisIsActivePlayer == 1)
    {
        return true;
    }

    return false;
}

// https://decomp.me/scratch/Yg4cw
MATCH_FUNC(0x45d000)
bool Ped::HandlePedHitByObject(Object* pObj)
{

    Ang16 angle;
    Ped* pBlamedPed = NULL;

    if ((mPedState1 == ped_state_1::immobilized_8 ||
         mPedState1 == ped_state_1::dead_9 && pObj->mModel != objects::water_hitting_198) &&
        mPedState2 != ped_state_2::lying_on_floor_22)
    {
        return false;
    }

    if (pObj->GetPedRefIdx())
    {
        s32 ped_id = gPedRefTable_7F8->GetPedId(pObj->GetPedRefIdx());
        if (ped_id)
        {
            pBlamedPed = gPedManager->PedById(ped_id);
            if (pBlamedPed)
            {
                if (pBlamedPed->IsPedAThreat_Inline_465D00(this) || this == pBlamedPed->mInternalTargetPed)
                {
                    if (pBlamedPed->mBf21C.bWeaponChosen)
                    {
                        if (pBlamedPed->mpWeapon)
                        {
                            pBlamedPed->mpWeapon->SetTargetHit(0);
                        }
                    }
                    else
                    {
                        if (pBlamedPed->mSelectedWeapon)
                        {
                            pBlamedPed->mSelectedWeapon->SetTargetHit(0);
                        }
                    }

                    if (pBlamedPed->PedTypeIs(ped_type::player_2))
                    {
                        pBlamedPed->mPlayer->mScores.UpdateAccuracyCount(accuracy_event::hit_threat_ped_2, pObj->mModel, this);
                    }
                    mAttacker = pBlamedPed;

                    if (!IsNoKillCreditObject(pObj->mModel))
                    {
                        mKillerId = pBlamedPed->mId;
                        mDeathCause = GetDeathCauseForObjectModel(pObj->mModel);
                        mKillerIdTimer = 50;
                    }
                }
                else if (pBlamedPed->PedTypeIs(ped_type::player_2))
                {
                    if (pBlamedPed->mBf21C.bWeaponChosen)
                    {
                        if (pBlamedPed->mpWeapon)
                        {
                            pBlamedPed->mpWeapon->SetTargetHit(0);
                        }
                    }
                    else
                    {
                        if (pBlamedPed->mSelectedWeapon)
                        {
                            pBlamedPed->mSelectedWeapon->SetTargetHit(0);
                        }
                    }
                    pBlamedPed->mPlayer->mScores.UpdateAccuracyCount(accuracy_event::hit_other_ped_3, pObj->mModel, this);
                    mAttacker = pBlamedPed;
                    if (IsNoKillCreditObject(pObj->mModel))
                    {
                        return true;
                    }
                    mKillerId = pBlamedPed->mId;
                    mDeathCause = GetDeathCauseForObjectModel(pObj->mModel);
                    mKillerIdTimer = 50;
                    return true;
                }
                else
                {
                    if (IsPedAThreat(pBlamedPed))
                    {
                        mAttacker = pBlamedPed;
                    }
                    if (pBlamedPed->mBf21C.bWeaponChosen)
                    {
                        if (pBlamedPed->mpWeapon)
                        {
                            pBlamedPed->mpWeapon->SetTargetHit(1);
                        }
                    }
                    else
                    {
                        if (pBlamedPed->mSelectedWeapon)
                        {
                            pBlamedPed->mSelectedWeapon->SetTargetHit(1);
                        }
                    }
                    if (mAttacker != pBlamedPed)
                    {
                        return true;
                    }
                }
            }
        }
    }

    gScriptManager->RecordWeaponHit(mId, pObj->mModel, 1);

    switch (pObj->mModel)
    {
        case objects::fire_hitting_194:
            Ped::SetOnFire();
            return true;

        case objects::water_hitting_198:
            Ped::PutOutFire();
            angle = ComputeShortestAngleDelta(mGameObject->mSpritePtr->mRotation, pObj->mSprite->mRotation);
            if (angle > kAng90_6784B0)
            {
                mGameObject->SetCharState(Char_B4_state::Hit_From_Front_33);
            }
            else
            {
                mGameObject->SetCharState(Char_B4_state::Hit_From_Behind_34);
            }

            if (mInvulnerability)
            {
                return true;
            }

            if (mPedType == ped_type::player_2)
            {
                if (mPlayer->HasPowerUp(power_up_indices::Armor_3))
                {
                    mPlayer->DecPowerUp(power_up_indices::Armor_3);
                }
                else
                {
                    Ped::TakeDamage(2 * GetDamageMultiplier(pBlamedPed));
                }
            }
            else
            {
                Ped::TakeDamage(5 * GetDamageMultiplier(pBlamedPed));
            }
            gParticle_8->EmitBloodBurst(mCam.x, mCam.y, mCam.z, pObj->mSprite->mRotation);
            break;

        case objects::rocket_bullet_128:
        case objects::moving_molotov_138:
        case objects::shotgun_bullet_192:
        case objects::flamethrower_bullet_195:
        case objects::machine_gun_bullet_254:
        case objects::pistol_bullet_265:
        {
            angle = ComputeShortestAngleDelta(mGameObject->mSpritePtr->mRotation, pObj->mSprite->mRotation);
            if (angle > kAng90_6784B0)
            {
                mGameObject->SetCharState(Char_B4_state::Hit_From_Front_33);
            }
            else
            {
                mGameObject->SetCharState(Char_B4_state::Hit_From_Behind_34);
            }
            if (mInvulnerability)
            {
                return true;
            }

            char_type damage_amplifier;
            if (pObj->mModel == objects::pistol_bullet_265)
            {
                damage_amplifier = 2;
            }
            else
            {
                damage_amplifier = 1;
            }

            if (mPedType == ped_type::player_2)
            {
                if (mPlayer->HasPowerUp(power_up_indices::Armor_3))
                {
                    mPlayer->DecPowerUp(power_up_indices::Armor_3);
                }
                else
                {
                    Ped::TakeDamage(10 * damage_amplifier * GetDamageMultiplier(pBlamedPed));
                }
            }
            else
            {
                Ped::TakeDamage(25 * damage_amplifier * GetDamageMultiplier(pBlamedPed));
            }
            gParticle_8->EmitBloodBurst(mCam.x, mCam.y, mCam.z, pObj->mSprite->mRotation);
            break;
        }

        case objects::electrobaton_bullet_277:
            if (pBlamedPed)
            {
                pBlamedPed->mHitTargetPed = this;
                pBlamedPed->mElectrocutionTimer = 20;
            }
            return true;

        default:
            return true;
    }
    return true;
}

MATCH_FUNC(0x45dd30)
bool Ped::AddWeaponWithAmmo(s32 weapon_kind, char_type ammo)
{
    if (ammo == 100) // max is 99, 100 means get default count
    {
        ammo = gWeapon_8->get_defalt_ammo(weapon_kind);
    }

    if (gWeapon_8->is_car_weapon_kind(weapon_kind))
    {
        if (mCar)
        {
            return gWeapon_8->add_car_weapon(weapon_kind, ammo, mCar);
        }
        return false;
    }

    if (mPlayer)
    {
        return mPlayer->AddWeaponWithAmmo(weapon_kind, ammo);
    }

    if (mSelectedWeapon && mSelectedWeapon->mWeaponType == weapon_kind)
    {
        return false;
    }

    RemovePedWeapons();
    ForceWeapon(weapon_kind);

    return true;
}

MATCH_FUNC(0x45de80)
bool Ped::HandlePickupCollision(Object* pPickUp)
{
    bool bCollected;
    if (this->mPedType != ped_type::player_2)
    {
        return false;
    }

    if (IsNetworkGame() && gTagGame.IsTagGame() &&
        gTagGame.IsFugitivePed(this))
    {
        return false; // prevent pick ups if we are "it" in multiplayer?
    }

    s32 model = pPickUp->mModel;
    if (model == objects::secret_token_266)
    {
        // inc counter and remove pick up
        gGameSession.IncSecretTokensCollected();
        gObject_5C->mUncollectedTokens[pPickUp->GetParam()] = false;
        bCollected = true;
    }
    else
    {
        if (model <= 108)
        {
            model += 136;
        }

        if (model <= 227)
        {
            bCollected = AddWeaponWithAmmo(model - 200, pPickUp->GetAmmo());
        }
        else
        {
            bCollected = mPlayer->CollectPowerUp(model - 228);
        }

        if (bCollected)
        {
            if (mPlayer->IsUser())
            {
                gHud_2B00->mPickupText.ShowPickupText(model + 56);
            }
        }
    }
    if (bCollected)
    {
        pPickUp->Dealloc();
    }
    return bCollected;
}

MATCH_FUNC(0x45e080)
void Ped::SpawnWeaponOnDeath()
{
    Object* pWeaponObj;

    if (this->mBf224.bDropsWeaponOnDeath && !this->mCar)
    {
        if (this->mSelectedWeapon)
        {
            if (!PedTypeIs(ped_type::player_2))
            {
                switch (this->mSelectedWeapon->mWeaponType)
                {
                    case weapon_type::pistol:
                        pWeaponObj = gObject_5C->NewPhysicsObj(objects::pistol_weapon_200,
                                                               mCam.x,
                                                               mCam.y,
                                                               mCam.z,
                                                               gDummyPedAng);
                        break;

                    case weapon_type::smg:
                        pWeaponObj = gObject_5C->NewPhysicsObj(objects::machine_gun_weapon_201,
                                                               mCam.x,
                                                               mCam.y,
                                                               mCam.z,
                                                               gDummyPedAng);
                        break;

                    case weapon_type::rocket:
                        pWeaponObj = gObject_5C->NewPhysicsObj(202, mCam.x, mCam.y, mCam.z, gDummyPedAng);
                        break;

                    case weapon_type::shocker:
                        pWeaponObj = gObject_5C->NewPhysicsObj(203, mCam.x, mCam.y, mCam.z, gDummyPedAng);
                        break;

                    case weapon_type::molotov:
                        pWeaponObj = gObject_5C->NewPhysicsObj(objects::molotov_weapon_204,
                                                               mCam.x,
                                                               mCam.y,
                                                               mCam.z,
                                                               gDummyPedAng);
                        break;

                    case weapon_type::grenade:
                        pWeaponObj = gObject_5C->NewPhysicsObj(205, mCam.x, mCam.y, mCam.z, gDummyPedAng);
                        break;

                    case weapon_type::shotgun:
                        pWeaponObj = gObject_5C->NewPhysicsObj(objects::shotgun_weapon_206,
                                                               mCam.x,
                                                               mCam.y,
                                                               mCam.z,
                                                               gDummyPedAng);
                        break;

                    case weapon_type::electro_batton:
                        pWeaponObj = gObject_5C->NewPhysicsObj(objects::pistol_weapon_200,
                                                               mCam.x,
                                                               mCam.y,
                                                               mCam.z,
                                                               gDummyPedAng);
                        break;

                    case weapon_type::flamethrower:
                        pWeaponObj = gObject_5C->NewPhysicsObj(objects::flamethrower_weapon_208,
                                                               mCam.x,
                                                               mCam.y,
                                                               mCam.z,
                                                               gDummyPedAng);
                        break;

                    case weapon_type::silence_smg:
                        pWeaponObj = gObject_5C->NewPhysicsObj(209, mCam.x, mCam.y, mCam.z, gDummyPedAng);
                        break;

                    case weapon_type::dual_pistol:
                        pWeaponObj = gObject_5C->NewPhysicsObj(210, mCam.x, mCam.y, mCam.z, gDummyPedAng);
                        break;
                    default:
                        return;
                }

                if (pWeaponObj)
                {
                    pWeaponObj->SetO8Timer(9);
                }
            }
        }
    }
}

// https://decomp.me/scratch/PNiZo
MATCH_FUNC(0x45e4a0)
void Ped::StartCrossingRoad()
{
    if (mOccupation != ped_ocupation_enum::dummy || mObjective != objectives_enum::no_obj_0 || mInternalObjective != objectives_enum::no_obj_0)
    {
        return;
    }

    u8 x = mCam.x.ToInt();
    u8 y = mCam.y.ToInt();
    u8 z = mCam.z.ToInt();

    s8 direction;
    // Named locals: the original gives each max its own stack slot (as in ApplyAimJitter).
    s16 max_north;
    s16 max_east;
    s16 max_south;
    s16 max_west;

    // 9.6f: 0x433470/0x4334A0/0x4334D0/0x433500 (N/E/S/W). The first two rng results are
    // tested as u8 (test al,al; ja), the last two as s16 (test ax,ax; jg), in both builds.
    // z - 1 is passed as an expression: a plain u8 local would give one (int)z for all four
    // block lookups, the original converts it again at each one.
    do
    {
        if (gMap_0x370->IsBlockRoadTypeInlined_433470(x, y - 1, z - 1))
        {
            direction = 0;
            max_north = 2;
            if (static_cast<u8>(gRng.get_int(max_north)) > 0)
            {
                break;
            }
        }

        if (gMap_0x370->IsBlockRoadTypeInlined_433470(x + 1, y, z - 1))
        {
            direction = 1;
            max_east = 2;
            if (static_cast<u8>(gRng.get_int(max_east)) > 0)
            {
                break;
            }
        }

        if (gMap_0x370->IsBlockRoadTypeInlined_433470(x, y + 1, z - 1))
        {
            direction = 2;
            max_south = 2;
            if (gRng.get_int(max_south) > 0)
            {
                break;
            }
        }

        if (gMap_0x370->IsBlockRoadTypeInlined_433470(x - 1, y, z - 1))
        {
            direction = 3;
            max_west = 2;
            if (gRng.get_int(max_west) > 0)
            {
                break;
            }
        }

        direction = 4;
    } while (0);

    switch (direction)
    {
        case 0:
            SetObjective2(objectives_enum::wait_to_cross_y_backwards_44, 0x270F);
            mTargetFacingAngle = gSpawnRotationBottom;
            ++gNumPedsCrossingRoad;
            break;
        case 1:
            SetObjective2(objectives_enum::wait_to_cross_x_forwards_45, 0x270F);
            mTargetFacingAngle = gSpawnRotationLeft;
            ++gNumPedsCrossingRoad;
            break;
        case 2:
            SetObjective2(objectives_enum::wait_to_cross_y_forwards_46, 0x270F);
            mTargetFacingAngle = gSpawnRotationTop;
            ++gNumPedsCrossingRoad;
            break;
        case 3:
            SetObjective2(objectives_enum::wait_to_cross_x_backwards_47, 0x270F);
            mTargetFacingAngle = gSpawnRotationRight;
            ++gNumPedsCrossingRoad;
            break;
    }
}

MATCH_FUNC(0x45ea00)
void Ped::DeallocateWithGroupCleanup()
{
    if (mPedGroup)
    {
        if (GetOffscreenCounter() <= 0x1E)
        {
            return;
        }

        if (IsGroupLeader())
        {
            if (mPedGroup->IsAllMembersInSomeCar())
            {
                u8 i = 0;
                Ped* member = mPedGroup->mPedList[0];

                while (member)
                {
                    member->reset_ped_group();
                    member->Deallocate();
                    ++i;
                    member = mPedGroup->mPedList[i];
                }

                mPedGroup->ClearGroupData();
            }
            else
            {
                bool can_remove_group = true;
                u8 i = 0;
                Ped* member = mPedGroup->mPedList[0];

                while (member)
                {
                    if (member->GetOffscreenCounter() < 0x1E && member->mGameObject)
                    {
                        can_remove_group = false;
                    }

                    ++i;
                    member = mPedGroup->mPedList[i];
                }

                if (!can_remove_group)
                {
                    return;
                }

                member = mPedGroup->mPedList[0];
                i = 0;

                while (member)
                {
                    member->reset_ped_group();
                    member->Deallocate();
                    ++i;
                    member = mPedGroup->mPedList[i];
                }

                mPedGroup->ClearGroupData();
            }
        }
        else
        {
            mPedGroup->RemovePed(this);
        }
    }
    else
    {
        if (GetOffscreenCounter() <= 0x1E)
        {
            return;
        }
    }

    Deallocate();
    mBf21C.bScheduledForRemoval = false;
}

// https://decomp.me/scratch/jJ6aF
// Clearing the flag through a reference keeps VC6 from hoisting the mCar load above it.
// The flags word is passed and returned by value: VC6 then loads it into eax before the timer store
// and clears the bit with the short `and al, 0xFE` form, like the original
static inline PedFlags32 ClearActive(PedFlags32 bf)
{
    bf.bActive = false;
    return bf;
}

MATCH_FUNC(0x45eb60)
void Ped::Deallocate()
{
    switch (mOccupation)
    {
        case ped_ocupation_enum::mugger:
            --gNumberMuggersSpawned;
            break;
        case ped_ocupation_enum::car_thief:
            --gNumberCarThiefsSpawned;
            break;
        case ped_ocupation_enum::elvis_leader:
            if (gNumberElvisLeadersSpawned > 0)
            {
                --gNumberElvisLeadersSpawned;
            }
            break;
        case ped_ocupation_enum::walking_guard_29:
            --gNumberWalkingCopsSpawned;
            break;
        case ped_ocupation_enum::armed_gang_member_19:
            --gNumberArmedGangMembers;
            break;
        case ped_ocupation_enum::bus_customer_8:
            if (--gNumberBusCustomers < 0)
            {
                gNumberBusCustomers = 0;
            }
            break;
        default:
            break;
    }
    if (mId)
    {
        if (mSelectedWeapon)
        {
            Ped::RemovePedWeapons();
        }
        if (mpWeapon)
        {
            Ped::RemoveSecondaryWeapon();
        }
    }

    if (mPedRefIdx)
    {
        gPedRefTable_7F8->Clear(mPedRefIdx);
    }
    if (mBf21C.bInPathList)
    {
        gPathFinder->mPedList.RemovePed_471240(this);
    }
    gPathFinder->RemovePed(mId);
    gThreateningPedsList.RemovePed_471240(this);

    if (mpGang)
    {
        if (mpGang->mAlwaysBasicWeapon)
        {
            mpGang->mAlwaysBasicWeapon = false;
        }
    }
    if (mPedGroup)
    {
        if (IsGroupLeader())
        {
            if (mPedGroup->IsAllMembersInSomeCar())
            {
                mPedGroup->ClearGroupData();
            }
            else
            {
                if ((mOccupation != ped_ocupation_enum::elvis_leader && mOccupation != ped_ocupation_enum::elvis))
                {
                    mPedGroup->DestroyGroup();
                }
                else if (mPedGroup->AreAllMembersOffScreen())
                {
                    mPedGroup->DestroyGroup();
                }
                else
                {
                    mPedGroup->DisbandGroupDueToAttack(mPedKiller);
                }
            }
        }
        else if (mOccupation == ped_ocupation_enum::elvis)
        {
            mPedGroup->DisbandGroupDueToAttack(mPedKiller);
        }
        else
        {
            mPedGroup->RemovePed(this);
        }
    }

    mLifetimeTimer = 2;
    mBf21C = ClearActive(mBf21C);

    if (mCar)
    {
        if (!mEnterCarAsPassenger)
        {
            mCar->mDriver = NULL; // whether or not this ped is the car's driver
            if (mPedType == ped_type::script_created_5)
            {
                if (mCar->GetCarKind() == 1)
                {
                    mCar->mPedType = ped_type::dummy_3;
                }
            }
        }
    }
    Ped::SetObjective(objectives_enum::no_obj_0);
    Ped::SetObjective2(objectives_enum::no_obj_0);
    mPedState1 = ped_state_1::dead_9;
    mPedState2 = ped_state_2::ped2_dead_15;
}

MATCH_FUNC(0x45edc0)
bool Ped::IsActivePlayerPed() const
{
    if (mPedType == ped_type::player_2)
    {
        if (mOccupation != ped_ocupation_enum::empty)
        {
            return true;
        }
    }
    return false;
}

MATCH_FUNC(0x45ede0)
bool Ped::PedTypeIs(s32 type) const
{
    return mPedType == type ? true : false;
}

// https://decomp.me/scratch/pXF8g
MATCH_FUNC(0x45ee00)
void Ped::SetOccupation(u32 occupation)
{
    if (mOccupation <= static_cast<u32>(ped_ocupation_enum::bank_robber))
    {
        switch (mOccupation)
        {
            case ped_ocupation_enum::player:
            case ped_ocupation_enum::empty:
            case ped_ocupation_enum::deallocate_on_death_2:
            case ped_ocupation_enum::dummy:
            case ped_ocupation_enum::dummy_driver_4:
            case ped_ocupation_enum::driver:
            case ped_ocupation_enum::unknown_3:
            case ped_ocupation_enum::taxi_customer_7:
            case ped_ocupation_enum::train_customer_9:
            case ped_ocupation_enum::robbed_driver_10:
            case ped_ocupation_enum::fleeing_robbed_driver_11:
            case ped_ocupation_enum::angry_armed_robbed_driver_12:
            case ped_ocupation_enum::very_angry_armed_robbed_driver_13:
            case ped_ocupation_enum::psycho:
            case ped_ocupation_enum::mugger:
            case ped_ocupation_enum::car_thief:
            case ped_ocupation_enum::bank_robber:
                mOccupation = occupation;
                break;
            case ped_ocupation_enum::bus_customer_8:
                if (--gNumberBusCustomers < 0)
                    gNumberBusCustomers = 0;
                mOccupation = occupation;
                break;
        }
    }
    else
        mOccupation = occupation;
}

MATCH_FUNC(0x45ee70)
void Ped::EnterPublicTransport()
{
    for (gmp_map_zone* pZoneIter = gMap_0x370->GetFirstZoneAtPos(mCam.x.ToInt(), mCam.y.ToInt()); pZoneIter;
         pZoneIter = gMap_0x370->GetNextZone())
    {
        if (bSkip_trains || pZoneIter->mZoneType != Railway_Station_Platform_6)
        {
            if (!bSkip_buses)
            {
                if (gRng.get_int(100) > 90 && gNumberBusCustomers < 5 && pZoneIter->mZoneType == Bus_Stop_7 &&
                    !gPublicTransport_181C->is_bus_full())
                {
                    if (mInternalObjective != objectives_enum::enter_train_37 && mInternalObjective != objectives_enum::leave_train_38 && this->mPedState1 == ped_state_1::walking_0)
                    {
                        SetOccupation(ped_ocupation_enum::bus_customer_8);
                        SetObjective2(objectives_enum::enter_bus_30);
                        ++gNumberBusCustomers;
                    }
                }
            }
        }
        else
        {
            TrainStation* pTrainStation = gPublicTransport_181C->TrainStationForZone(pZoneIter);
            if (gRng.get_int(100) > 90 && gNumberBusCustomers < 5)
            {
                if (mInternalObjective != objectives_enum::enter_train_37 && mInternalObjective != objectives_enum::leave_train_38 && mInternalObjective != objectives_enum::goto_area_on_foot_12)
                {
                    Train* pTrain = pTrainStation->mpStoppedTrain;
                    if (pTrain)
                    {
                        if (pTrain->mCarriages[1]->GetCarInfoIdx() == car_model_enum::TRAIN)
                        {
                            SetOccupation(ped_ocupation_enum::train_customer_9);
                            SetObjective2(objectives_enum::wait_for_train_29);
                            this->mTargetToEnter = pTrainStation->mpStoppedTrain->mCarriages[1];
                        }
                    }
                }
            }
        }
    }
}

MATCH_FUNC(0x45f360)
void Ped::Mugger_AI()
{
    if (mInternalObjective == objectives_enum::flee_char_on_foot_till_safe_2 && mInternalObjectiveStatus == objective_status::passed_1)
    {
        Ped::SetObjective2(objectives_enum::no_obj_0);
    }
    switch (mObjective)
    {
        case objectives_enum::no_obj_0:
            if (!mOffscreenCounter)
            {
                if (mObjectiveTimer == 0)
                {
                    Ped* pTarget = GetLastProcessedPedOnFoot();
                    if (!pTarget || pTarget->GetOffscreenCounter())
                    {
                        Ped::SetObjective(objectives_enum::no_obj_0);
                    }
                    else
                    {
                        Ped::SetObjective(objectives_enum::punch_char_23);
                        set_objective_target_ped(pTarget);
                        mMugCount = 0;
                        mBf21C.bAttacking = false;
                    }
                }
            }
            else
            {
                mObjectiveTimer = 40;
                Ped::ChangeNextPedState1(ped_state_1::walking_0);
                Ped::ChangeNextPedState2(ped_state_2::ped2_walking_0);
            }

            break;
        case objectives_enum::punch_char_23:
            if (mObjectiveTargetPed && mObjectiveTargetPed->has_car())
            {
                Ped::SetObjective(objectives_enum::no_obj_0);
                Ped::SetObjective2(objectives_enum::no_obj_0);
            }
            else
            {
                if (mObjectiveStatus == objective_status::passed_1)
                {
                    Ped::SetObjective(objectives_enum::wait_on_foot_26, 30);
                }
                else if (mObjectiveStatus == objective_status::failed_2)
                {
                    Ped::SetObjective(objectives_enum::no_obj_0, 0);
                    Ped::SetObjective2(objectives_enum::no_obj_0, 0);
                    mBf21C.bPanicking = false;
                }
            }
            break;

        case objectives_enum::wait_on_foot_26:
            mFacingAngle + kAng180; // non used
            mBf21C.bAttacking = true;
            Ped::SetObjective(objectives_enum::flee_on_foot_till_safe_1);
            mObjectiveTargetX = mCam.x;
            mObjectiveTargetY = mCam.y;
            mObjectiveTargetZ = mCam.z;
            break;

        case objectives_enum::flee_on_foot_till_safe_1:
            if (mObjectiveStatus == objective_status::passed_1)
            {
                mBf21C.bAttacking = false;
                mPedState1 = ped_state_1::walking_0;
                mPedState2 = ped_state_2::ped2_walking_0;
                Ped::SetObjective(objectives_enum::no_obj_0);
                Ped::SetOccupation(ped_ocupation_enum::dummy);
                --gNumberMuggersSpawned;
            }
            break;

        case objectives_enum::flee_char_on_foot_till_safe_2:
            if (mObjectiveStatus != objective_status::not_finished_0)
            {
                Ped::SetObjective(objectives_enum::no_obj_0);
            }
            break;

        default:
            return;
    }
}

MATCH_FUNC(0x45ff60)
void Ped::CarThief_AI()
{
    Car* pStolenCar;
    Car* pCar;
    Fix16 xd;
    Fix16 yd;

    if (this->mInternalObjective == objectives_enum::flee_char_on_foot_till_safe_2 && this->mInternalObjectiveStatus == objective_status::passed_1)
    {
        SetObjective2(objectives_enum::no_obj_0);
    }

    switch (this->mObjective)
    {
        case objectives_enum::no_obj_0:
            if (this->mBf21C.bLeftVehicle)
            {
                if (mTargetObjectiveCar)
                {
                    if (mTargetObjectiveCar->IsDespawning())
                    {
                        Kill();
                        return;
                    }
                }
                else
                {
                    ForceDoNothing();
                    this->mOccupation = ped_ocupation_enum::dummy;
                }
            }
            if (!this->mOffscreenCounter)
            {
                if (!this->mObjectiveTimer)
                {
                    Car* pNearestCar = gCarManager_6C->GetNearestEnterableCarFromCoord(this->mCam.x,
                                                                                                  this->mCam.y,
                                                                                                  this->mCam.z,
                                                                                                  this);
                    if (pNearestCar)
                    {
                        if (pNearestCar->mPedType != ped_type::player_2)
                        {
                            if (!pNearestCar->IsTrainModel() && !pNearestCar->IsPoliceCar() &&
                                pNearestCar->mCarInfoIdx != car_model_enum::BUS &&
                                pNearestCar->mPassengersList.HasNoPeds() && pNearestCar->mPedType == ped_type::dummy_3)
                            {
                                SetObjective(objectives_enum::enter_car_as_driver_35);
                                this->mTargetObjectiveCar = pNearestCar;
                                this->mEnterCarAsPassenger = 0;
                                this->mTargetCarDoor = 0;
                            }
                        }
                    }
                }
            }
            else
            {
                this->mObjectiveTimer = 40;
                ChangeNextPedState1(ped_state_1::walking_0);
                ChangeNextPedState2(ped_state_2::ped2_walking_0);
            }
            return;

        case objectives_enum::enter_car_as_driver_35:
            if (mObjectiveStatus == objective_status::passed_1)
            {
                if (this->mTargetObjectiveCar->IsDespawning())
                {
                    Kill();
                    return;
                }
                if (this->mPedState2 == ped_state_2::ped2_at_car_door_17)
                {
                    SetObjective(objectives_enum::no_obj_0, 40);
                    SetObjective2(objectives_enum::no_obj_0);
                }
                else
                {
                    SetVoiceEventIfNotOnFire(15);
                    SetObjective(objectives_enum::time_waited_in_car_31, 0);
                    pStolenCar = this->mCar;
                    this->mTargetObjectiveCar = pStolenCar;
                    pStolenCar->InitCarAIControl();
                    mTargetObjectiveCar->ResumeAIDriving();
                }
                return;
            }

            if (mObjectiveStatus == objective_status::failed_2)
            {
                SetObjective(objectives_enum::no_obj_0, 40);
                SetObjective2(objectives_enum::no_obj_0);
            }
            else if (this->mCar && this->mTargetObjectiveCar->IsDespawning())
            {
                Kill();
                return;
            }

            xd = this->mTargetX - this->mCam.x;
            yd = this->mTargetY - this->mCam.y;

            xd = Fix16::Abs(xd);
            yd = Fix16::Abs(yd);
            // The larger of the two, picked through a pointer
            if (*(xd > yd ? &xd : &yd) > kFpFour)
            {
                SetObjective(objectives_enum::no_obj_0);
                SetObjective2(objectives_enum::no_obj_0);
                this->mObjectiveTimer = 40;
            }
            return;

        case objectives_enum::time_waited_in_car_31:
            pCar = this->mCar;
            if (!pCar)
            {
                SetObjective(objectives_enum::no_obj_0, 40);
                return;
            }
            if (pCar->IsDespawning())
            {
                Kill();
                return;
            }
            if (this->mObjectiveTimer > 0x258u)
            {
                SetObjective(objectives_enum::leave_car_36);
                this->mTargetObjectiveCar = this->mCar;
            }
            return;

        case objectives_enum::leave_car_36:
            if (this->mObjectiveStatus == objective_status::passed_1)
            {
                SetObjective(objectives_enum::flee_on_foot_till_safe_1);
                this->mTargetX = this->mCam.x;
                this->mTargetY = this->mCam.y;
                this->mTargetZ = this->mCam.z;
            }
            return;

        case objectives_enum::flee_on_foot_till_safe_1:
            if (this->mObjectiveStatus == objective_status::passed_1)
            {
                SetObjective(objectives_enum::no_obj_0, 40);
            }
            return;

        default:
            return;
    }
}

MATCH_FUNC(0x460820)
void Ped::TaxiCustomer_AI()
{
    if (this->mInternalObjective == objectives_enum::flee_char_on_foot_till_safe_2)
    {
        if (this->mInternalObjectiveStatus == objective_status::passed_1)
        {
            SetObjective2(objectives_enum::no_obj_0);
        }
    }

    switch (this->mObjective)
    {
        // This case comes first: the other cases' inline sites after MaxAbsDistance leave its
        // y difference and Abs out of line, as in the original.
        case objectives_enum::no_obj_0:
            // It has no objective
            if (!mOffscreenCounter)
            {
                if (mObjectiveTimer == 0)
                {
                    // Look for a near taxi
                    Car* pTaxi = gTaxi_4->GetTaxiNear(this->mCam.x, this->mCam.y);
                    if (pTaxi)
                    {
                        Fix16 dist;
                        dist = Fix16::MaxAbsDistance(this->mCam.x,
                                                     this->mCam.y,
                                                     pTaxi->mCarSprite->mXy.x,
                                                     pTaxi->mCarSprite->mXy.y);
                        if (dist < kFpTwo_678658)
                        {
                            SetVoiceEventIfNotOnFire(5);
                            SetObjective2(objectives_enum::no_obj_0);
                            SetObjective(objectives_enum::enter_car_as_driver_35);
                            this->mTargetObjectiveCar = pTaxi;
                            this->mEnterCarAsPassenger = 1;
                            this->mTargetCarDoor = 3;
                            pTaxi->HaltAIDriving();
                        }
                    }
                }
            }
            else
            {
                // Set a little timer before looking for a taxi
                mObjectiveTimer = 40;
                ChangeNextPedState1(ped_state_1::walking_0);
                ChangeNextPedState2(ped_state_2::ped2_walking_0);
            }
            break;

        case objectives_enum::enter_car_as_driver_35: // as a passenger: mEnterCarAsPassenger is set
        {
            // It is on foot
            u8 status = this->mObjectiveStatus;
            if (status == objective_status::passed_1)
            {
                // It entered the taxi
                if (this->mTargetObjectiveCar->IsDespawning())
                {
                    // Taxi is wreck, kill it
                    Kill();
                    return;
                }
                SetVoiceEventIfNotOnFire(6);
                this->mTargetObjectiveCar->ResumeAIDriving();
                SetObjective(objectives_enum::time_waited_in_car_31, 0);
                this->mTargetObjectiveCar = this->mCar;
            }
            else if (status == objective_status::failed_2)
            {
                // Ped failed to reach car
                Car* pTaxi = this->mTargetObjectiveCar;
                if (!pTaxi->IsDespawning())
                {
                    pTaxi->ResumeAIDriving();
                }
                SetObjective(objectives_enum::no_obj_0, 40);
                SetObjective2(objectives_enum::no_obj_0);
            }
            else if (mPedState1 != ped_state_1::in_car_10)
            {
                // It not entered the taxi yet
                Fix16 dx;
                Fix16 dy;
                dx = this->mTargetX - this->mCam.x;
                dy = this->mTargetY - this->mCam.y;
                dx = Fix16::Abs(dx);
                dy = Fix16::Abs(dy);

                if (((dx > dy) ? dx : dy) > kFpTwo_678658 || this->mBf21C.bNoPathFound)
                {
                    this->mTargetObjectiveCar->ResumeAIDriving();
                    SetObjective(objectives_enum::no_obj_0);
                    SetObjective2(objectives_enum::no_obj_0);
                    this->set_occupation(ped_ocupation_enum::dummy);
                    this->SetPedType(ped_type::dummy_3);
                    return;
                }
                Car* pCar = this->mTargetObjectiveCar;
                if (pCar->mPassengersList.HasNoPeds() && !pCar->IsDespawning())
                {
                    break;
                }
                pCar->ResumeAIDriving();
                SetObjective(objectives_enum::no_obj_0);
                SetObjective2(objectives_enum::no_obj_0);
                this->set_occupation(ped_ocupation_enum::dummy);
                this->SetPedType(ped_type::dummy_3);
            }
            else if (this->mTargetObjectiveCar->IsDespawning())
            {
                Kill();
            }
            break;
        }

        case objectives_enum::time_waited_in_car_31:
        {
            // It is in the taxi
            if (mTargetObjectiveCar->GetVelocity() != kFpZero_678660)
            {
                mObjectiveTimer = 0; // taxi is moving, reset timer
            }
            Car* pTaxi = this->mTargetObjectiveCar;
            if (pTaxi->IsDespawning())
            {
                Kill(); // taxi is wreck/destroyed, kill the passenger
            }
            else
            {
                if (pTaxi->mDamageLevel >= 3)
                {
                    this->mBf21C.bForcedOutOfTaxi = true;
                }
                if (pTaxi->mDriver && !this->mBf21C.bForcedOutOfTaxi)
                {
                    if (this->mObjectiveTimer == 150)
                    {
                        // taxi is stopped long enough, exit the car
                        SetObjective(objectives_enum::flee_char_always_once_car_stopped_6);
                        this->mObjectiveTargetPed = this->mCar->mDriver;
                    }
                }
                else
                {
                    // taxi without driver -> exit
                    SetObjective(objectives_enum::leave_car_36);
                    SetOccupation(ped_ocupation_enum::dummy);
                    this->SetPedType(ped_type::dummy_3);
                    this->mTargetObjectiveCar = this->mCar;
                }
            }
            break;
        }
    }
}

MATCH_FUNC(0x461290)
void Ped::BusCustomer_AI()
{
    Car* pCurrentCar;

    if (this->mInternalObjective == objectives_enum::flee_char_on_foot_till_safe_2 && this->mInternalObjectiveStatus == objective_status::passed_1)
    {
        SetObjective2(objectives_enum::no_obj_0);
    }

    switch (this->mObjective)
    {
        case objectives_enum::leave_train_38:
            if (this->mObjectiveStatus != objective_status::not_finished_0)
            {
                this->SetPedType(ped_type::dummy_3);
                SetOccupation(ped_ocupation_enum::dummy);
                SetObjective2(objectives_enum::no_obj_0);
                SetObjective(objectives_enum::flee_on_foot_till_safe_1);
                this->mTargetX = this->mCam.x;
                this->mTargetY = this->mCam.y;
            }
            else if (this->mTargetObjectiveCar->IsDespawning())
            {
                Kill();
            }
            break;

        case objectives_enum::enter_car_as_driver_35:
            if (this->mObjectiveStatus == objective_status::passed_1)
            {
                if (--gNumberBusCustomers < 0)
                {
                    gNumberBusCustomers = 0;
                }
                Car* pCar = this->mCar;
                if (pCar->IsDrivenByPlayer())
                {
                    gPublicTransport_181C->IncrementBusPassengerCount();
                    SetObjective(objectives_enum::time_waited_in_car_31, 0);
                }
                else
                {
                    pCar->mPassengersList.RemovePed_471240(this);
                    Kill();
                }
            }
            else
            {
                Car* pTargetCar = this->mTargetObjectiveCar;
                if (pTargetCar->IsDespawning())
                {
                    pTargetCar->ResumeAIDriving();
                    SetObjective(objectives_enum::no_obj_0);
                    SetObjective2(objectives_enum::no_obj_0);
                    this->set_occupation(ped_ocupation_enum::dummy);
                    this->SetPedType(ped_type::dummy_3);
                }
                else
                {
                    pTargetCar->GetDoor(get_target_car_door())->Open();
                }
            }
            return;

        case objectives_enum::time_waited_in_car_31:
            pCurrentCar = this->mCar;
            if (pCurrentCar->IsDespawning())
            {
                pCurrentCar->mPassengersList.RemovePed_471240(this);
                Kill();
            }
            break;

        case objectives_enum::leave_car_and_flee_34:
            if (this->mInternalObjective == objectives_enum::leave_car_36 && this->mInternalObjectiveStatus == objective_status::passed_1)
            {
                this->SetPedType(ped_type::dummy_3);
                SetOccupation(ped_ocupation_enum::dummy);
                SetObjective2(objectives_enum::no_obj_0);
                SetObjective(objectives_enum::flee_on_foot_till_safe_1);
                this->mTargetX = this->mCam.x;
                this->mTargetY = this->mCam.y;
            }
            else
            {
                pCurrentCar = this->mCar;
                if (pCurrentCar && pCurrentCar->IsDespawning())
                {
                    pCurrentCar->mPassengersList.RemovePed_471240(this);
                    Kill();
                }
            }
            break;

        default:
            Car* pBus = gPublicTransport_181C->GetBusIfBoarding();
            if (pBus)
            {
                SetObjective2(objectives_enum::no_obj_0);
                SetObjective(objectives_enum::enter_car_as_driver_35);
                this->mTargetObjectiveCar = pBus;
                this->mGameObject->Set_F84(this->mTargetToEnter);
                this->mGameObject->SetVelocityByRef_433920(kFpZero_678660);
                set_target_car_door(1);
            }
            break;
    }
}

MATCH_FUNC(0x461530)
void Ped::TrainCustomer_AI()
{
    if (this->mInternalObjective == objectives_enum::flee_char_on_foot_till_safe_2 && this->mInternalObjectiveStatus == objective_status::passed_1)
    {
        SetObjective2(objectives_enum::no_obj_0);
    }

    switch (mInternalObjective)
    {
        case objectives_enum::enter_train_37:
            if (mTargetToEnter->GetVelocity() != kFpZero_678660)
            {
                this->SetPedType(ped_type::dummy_3);
                SetOccupation(ped_ocupation_enum::dummy);
                SetObjective2(objectives_enum::no_obj_0);
                SetObjective(objectives_enum::no_obj_0);
                this->mTargetX = this->mCam.x;
                this->mTargetY = this->mCam.y;
            }
            break;

        case objectives_enum::leave_train_38:
            if (this->mInternalObjectiveStatus)
            {
                this->SetPedType(ped_type::dummy_3);
                SetOccupation(ped_ocupation_enum::dummy);
                SetObjective2(objectives_enum::no_obj_0);
                SetObjective(objectives_enum::flee_on_foot_till_safe_1);
                this->mTargetX = this->mCam.x;
                this->mTargetY = this->mCam.y;
            }
            break;
    }
}

MATCH_FUNC(0x461630)
void Ped::RobbedDriver_AI()
{
    if (mInternalObjective == objectives_enum::flee_char_on_foot_till_safe_2 && mInternalObjectiveStatus == objective_status::passed_1)
    {
        SetObjective2(objectives_enum::no_obj_0);
    }

    switch (mOccupation)
    {
        case ped_ocupation_enum::robbed_driver_10:
            if (mGameObject)
            {
                if (mPedState1 != ped_state_1::immobilized_8)
                {
                    u16 rng_val = gRng.get_int(40);
                    if (bDont_get_car_back)
                    {
                        rng_val = 6;
                    }
                    if (!mCarThief->mOffscreenCounter)
                    {
                        if (mpGang)
                        {
                            rng_val = 19;
                        }
                    }
                    else
                    {
                        rng_val = 6;
                    }

                    switch (rng_val)
                    {
                        case 19:
                            // gang member or very angry driver (2.5% chance): shoot at the thief
                            this->SetPedType(ped_type::dummy_with_occupation_6);
                            this->mOccupation = ped_ocupation_enum::very_angry_armed_robbed_driver_13;
                            SetVoiceEventIfNotOnFire(ped_voice_event::very_angry_robbed_driver_14);
                            ForceDoNothing();
                            SetObjective(objectives_enum::kill_char_on_foot_20);
                            this->mObjectiveTargetPed = this->mCarThief;
                            ForceWeapon(weapon_type::pistol);
                            break;

                        case 7:
                        case 16:
                        case 32:
                            // 3 over 40 = 7.5% of chance of being an armed and angry driver
                            this->SetPedType(ped_type::dummy_with_occupation_6);
                            this->mOccupation = ped_ocupation_enum::angry_armed_robbed_driver_12;
                            SetVoiceEventIfNotOnFire(ped_voice_event::angry_robbed_driver_13);
                            SetObjective(objectives_enum::enter_car_as_driver_35);
                            this->mTargetCarDoor = 0;
                            this->mTargetObjectiveCar = mStolenCar;
                            if (mStolenCar->IsDespawning())
                            {
                                this->SetPedType(ped_type::dummy_3);
                                this->mOccupation = ped_ocupation_enum::fleeing_robbed_driver_11;
                                ForceDoNothing();
                                SetObjective(objectives_enum::flee_char_on_foot_till_safe_2);
                                this->mObjectiveTargetPed = this->mCarThief;
                                this->mThreatReaction = threat_reaction_enum::run_away_3;
                                this->mCarThief = NULL;
                            }
                            else
                            {
                                ForceWeapon(weapon_type::pistol);
                                this->mEnterCarAsPassenger = 0;
                            }
                            break;

                        default:
                            // normal behaviour: flee
                            this->SetPedType(ped_type::dummy_3);
                            this->mOccupation = ped_ocupation_enum::fleeing_robbed_driver_11;
                            this->mThreatReaction = threat_reaction_enum::run_away_3;
                            ForceDoNothing();
                            SetObjective(objectives_enum::flee_char_on_foot_till_safe_2);
                            this->mObjectiveTargetPed = this->mCarThief;
                            this->mCarThief = NULL;
                            break;
                    }
                }
                else
                {
                    if (this->mStolenCar->IsDespawning())
                    {
                        this->SetPedType(ped_type::dummy_3);
                        this->mOccupation = ped_ocupation_enum::fleeing_robbed_driver_11;
                        ForceDoNothing();
                        SetObjective(objectives_enum::flee_char_on_foot_till_safe_2);
                        this->mObjectiveTargetPed = this->mCarThief;
                        this->mCarThief = NULL;
                    }
                }
            }
            return;

        case ped_ocupation_enum::angry_armed_robbed_driver_12:
            if (mObjectiveStatus == objective_status::passed_1)
            {
                Car* pTargetCar = this->mTargetObjectiveCar;
                if (pTargetCar->IsDespawning())
                {
                    Kill();
                }
                else
                {
                    this->mOccupation = ped_ocupation_enum::driver;
                    this->SetPedType(ped_type::dummy_3);
                    if (pTargetCar)
                    {
                        pTargetCar->SetDriverPedType(ped_type::dummy_3);
                        SetObjective(objectives_enum::no_obj_0);
                    }
                    else
                    {
                        SetObjective(objectives_enum::no_obj_0);
                    }
                }
            }
            else if (mObjectiveStatus == objective_status::failed_2)
            {
                this->mOccupation = ped_ocupation_enum::dummy;
                SetObjective(objectives_enum::no_obj_0);
                SetObjective2(objectives_enum::no_obj_0);
                this->SetPedType(ped_type::dummy_3);
            }
            return;

        case ped_ocupation_enum::very_angry_armed_robbed_driver_13:
            if (this->mObjectiveStatus == objective_status::passed_1)
            {
                this->SetPedType(ped_type::dummy_with_occupation_6);
                this->mOccupation = ped_ocupation_enum::angry_armed_robbed_driver_12;
                if (mStolenCar && !mStolenCar->IsDespawning())
                {
                    SetObjective(objectives_enum::enter_car_as_driver_35);
                    this->mEnterCarAsPassenger = 0;
                    this->mTargetObjectiveCar = mStolenCar;
                    this->mTargetCarDoor = 0;
                }
                else
                {
                    this->mOccupation = ped_ocupation_enum::dummy;
                    SetObjective(objectives_enum::no_obj_0);
                    SetObjective2(objectives_enum::no_obj_0);
                    this->SetPedType(ped_type::dummy_3);
                }
            }
            else
            {
                if (mStolenCar && mStolenCar->IsDespawning())
                {
                    this->mStolenCar = NULL;
                }

                if (!this->mStolenCar)
                {
                    this->mObjectiveStatus = objective_status::not_finished_0;
                }

                if (mObjectiveStatus == objective_status::failed_2)
                {
                    this->mOccupation = ped_ocupation_enum::dummy;
                    SetObjective(objectives_enum::no_obj_0);
                    SetObjective2(objectives_enum::no_obj_0);
                    this->SetPedType(ped_type::dummy_3);
                }
            }
            return;

        case ped_ocupation_enum::fleeing_robbed_driver_11:
            if (this->mObjectiveStatus == objective_status::passed_1)
            {
                this->mOccupation = ped_ocupation_enum::dummy;
                SetObjective(objectives_enum::no_obj_0);
            }
            return;

        default:
            return;
    }
}

MATCH_FUNC(0x4619f0)
void Ped::RoadBlockTank_AI()
{
    if (!mCar)
    {
        this->mOccupation = ped_ocupation_enum::dummy;
    }
    else
    {
        mCar->SetAIHalted();
    }

    if (this->mThreatReaction != threat_reaction_enum::react_as_emergency_1 || gPolice_7B8->mMaxWantedLevel == cop_level_enum::army_6)
    {
        if (this->mObjective == objectives_enum::no_obj_0)
        {
            SetObjective(objectives_enum::tank_fire_at_player_61);
        }
    }
    else
    {
        SetObjective(objectives_enum::no_obj_0);
        this->mBf21C.bAttacking = false;
    }
}

// For entering a car angles the player ped towards the car door
MATCH_FUNC(0x461a60)
void Ped::UpdateFacingAngle()
{
    if (this->mObjective)
    {
        if (!this->mInternalObjective)
        {
            this->mMoveTargetX = mTargetX;
            this->mMoveTargetY = mTargetY;
            this->mMoveTargetZ = mTargetZ;
        }
    }

    switch (this->mPedState1)
    {
        case ped_state_1::findind_path_2:
        {
            Fix16 x = this->mMoveTargetX;
            Fix16 y = this->mMoveTargetY;
            this->mTargetFacingAngle = Fix16::atan2_fixed(y - this->mCam.y, x - this->mCam.x);
            break;
        }

        case ped_state_1::entering_car_3:
        {
            this->mTargetFacingAngle = Fix16::atan2_fixed(this->mMoveTargetY - this->mCam.y, this->mMoveTargetX - this->mCam.x);
            break;
        }

        case ped_state_1::flee_or_running_1:
        {
            if (this->mPedState2 == ped_state_2::running_away_3)
            {
                this->mTargetFacingAngle = Fix16::atan2_fixed(this->mCam.y - this->mMoveTargetY, this->mCam.x - this->mMoveTargetX);
            }

            if (this->mPedState2 == ped_state_2::running_to_target_2)
            {
                this->mTargetFacingAngle = Fix16::atan2_fixed(this->mMoveTargetY - this->mCam.y, this->mMoveTargetX - this->mCam.x);

                if (sKeepPaceWithTarget && this->mInternalTargetPed && gDistanceToTarget < kFpThreeSixteenths &&
                    ComputeShortestAngleDelta(mTargetFacingAngle, mFacingAngle) > kAng90_6784B0)
                {
                    mTargetFacingAngle = this->mFacingAngle;
                    this->mGameObject->SetVelocity(mInternalTargetPed->GetPedVelocity() -
                                                                     kFpOne64th);
                    return;
                }

                if (gCloseRangeHit == 1)
                {
                    this->mGameObject->mRepulsionTimer = 1;
                    Ang16 jitter;
                    if ((this->mId & 1) != 0)
                    {
                        jitter = Ang16::Fix16_To_Ang16(kFpOne64th_6784C4 * Fix16(gRng.get_int(45)));
                        this->mGameObject->mRepulsionRotation = mTargetFacingAngle + kAng90_6784B0 + jitter;
                    }
                    else
                    {
                        // 9.6f uses Fix16_To_Ang16 and operator- here too, but 10.5 needs the
                        // conversion written out and the subtraction split to inline like the original
                        jitter.rValue = (kFpOne64th_6784C4 * Fix16(gRng.get_int(45))).GetRaw() / 71;
                        jitter.Normalize();
                        Ang16 repulsion_angle = mTargetFacingAngle - kAng90_6784B0;
                        repulsion_angle = repulsion_angle - jitter;
                        this->mGameObject->mRepulsionRotation = repulsion_angle;
                    }
                }
                else if (GetPedVelocity() < kFpZero_678660)
                {
                    mTargetFacingAngle += kAng180_6785A6;
                }
            }
            break;
        }

        case ped_state_1::standing_still_7:
        {
            if (this->mPedState2 == ped_state_2::attacking_standing_11)
            {
                this->mTargetFacingAngle = Fix16::atan2_fixed(this->mMoveTargetY - this->mCam.y, this->mMoveTargetX - this->mCam.x);
            }
            break;
        }

        // Needed for the case 3 -> case 2 tail merge
        default:
            break;
    }
    this->mAimAngle = this->mTargetFacingAngle;
}

MATCH_FUNC(0x461f20)
void Ped::Occupation_AI()
{
    switch (mOccupation)
    {
        case ped_ocupation_enum::dummy:
            if (!mBf21C.bPanicking && mObjective == objectives_enum::no_obj_0 && !mPedGroup)
            {
                if (mGameObject)
                {
                    if (mOffscreenCounter || gNewTaxiCustomersThisTick || gRng.get_int(1000) >= 2)
                    {
                        Ped::EnterPublicTransport();
                    }
                    else if (!gTaxi_4->IsEmpty())
                    {
                        set_occupation(ped_ocupation_enum::taxi_customer_7);
                        if (mPedType == ped_type::dummy_3)
                        {
                            SetPedType(ped_type::dummy_with_occupation_6);
                        }
                        Ped::SetObjective(objectives_enum::no_obj_0, 40);
                        ++gNewTaxiCustomersThisTick;
                    }
                }
            }
            break;
        case ped_ocupation_enum::train_customer_9:
            Ped::TrainCustomer_AI();
            break;
        case ped_ocupation_enum::bus_customer_8:
            Ped::BusCustomer_AI();
            break;
        case ped_ocupation_enum::dummy_driver_4:
            gRunPedObjectiveAI = 0;
            break;
        case ped_ocupation_enum::driver:
            gRunPedObjectiveAI = 0;
            if (mCar)
            {
                if (mCar->IsDespawning())
                {
                    Ped::Kill();
                }
            }
            else
            {
                set_occupation(ped_ocupation_enum::dummy);
            }
            break;
        case ped_ocupation_enum::taxi_customer_7:
            Ped::TaxiCustomer_AI();
            break;

        case ped_ocupation_enum::mugger:
            Ped::Mugger_AI();
            break;
        case ped_ocupation_enum::car_thief:
            Ped::CarThief_AI();
            break;
        case ped_ocupation_enum::robbed_driver_10:
        case ped_ocupation_enum::angry_armed_robbed_driver_12:
        case ped_ocupation_enum::very_angry_armed_robbed_driver_13:
            Ped::RobbedDriver_AI();
            break;
        case ped_ocupation_enum::fleeing_robbed_driver_11:
            if (mObjectiveStatus)
            {
                Ped::SetObjective(objectives_enum::no_obj_0);
                Ped::SetObjective2(objectives_enum::no_obj_0);
                ClearPanicking();
            }
            break;
        case ped_ocupation_enum::road_block_tank_man:
            Ped::RoadBlockTank_AI();
            break;
        case ped_ocupation_enum::mad_mugger_40:
            if (!gPedManager->mMakeAllMuggers)
            {
                set_occupation(ped_ocupation_enum::dummy);
                SetPedType(ped_type::dummy_3);
                Ped::ForceDoNothing();
            }
            break;
        case ped_ocupation_enum::armed_gang_member_19:
            if (mInternalObjective == objectives_enum::kill_char_on_foot_20 && mpGang != NULL && mInternalTargetPed->is_player())
            {
                u8 player_idx = mInternalTargetPed->mPlayer->get_idx();
                if (!mpGang->IsRespectNegativeForPlayer(player_idx))
                {
                    Ped::SetObjective2(objectives_enum::no_obj_0);
                }
            }
            break;
        case ped_ocupation_enum::scared_driver_50:
            if (mObjective > objectives_enum::no_obj_0)
            {
                if (mObjective > objectives_enum::flee_char_on_foot_till_safe_2)
                {
                    if (mObjective == objectives_enum::leave_car_36)
                    {
                        if (mTargetObjectiveCar->IsDespawning())
                        {
                            Ped::Kill();
                        }
                        else if (mObjectiveStatus == objective_status::passed_1)
                        {
                            if (mCarThief)
                            {
                                Ped::SetObjective(objectives_enum::flee_char_on_foot_till_safe_2);
                                set_objective_target_ped(mCarThief);
                            }
                            else
                            {
                                Ped::SetObjective(objectives_enum::flee_on_foot_till_safe_1);
                                mObjectiveTargetX = mCam.x;
                                mObjectiveTargetY = mCam.y;
                                mObjectiveTargetZ = mCam.z;
                            }
                        }
                    }
                }
                else
                {
                    if (mObjectiveStatus == objective_status::passed_1)
                    {
                        set_occupation(ped_ocupation_enum::dummy);
                        Ped::SetObjective(objectives_enum::no_obj_0);
                        Ped::SetObjective2(objectives_enum::no_obj_0);
                    }
                }
            }
            break;
        default:
            return;
    }
}

MATCH_FUNC(0x462280)
void Ped::UpdateAI()
{
    gRunPedObjectiveAI = 1;
    Ped::Occupation_AI();
    gObjectiveMayAct = 0;
    if (gRunPedObjectiveAI)
    {
        if (mPedGroup)
        {
            if (!IsGroupLeader())
            {
                if (!mBf21C.bPanicking)
                {
                    mPedGroup->UpdateMemberAIState(mGroupIdx);
                    gObjectiveMayAct = 1;
                    sKeepPaceWithTarget = mPedGroup->mGroupType != ped_group_type::chain_1;
                }
                else
                {
                    gObjectiveMayAct = 0;
                    sKeepPaceWithTarget = 0;
                }
            }
            else if (!mBf21C.bPanicking)
            {
                gObjectiveMayAct = mPedGroup->Get_F3C() != 1;
                mPedGroup->CoordinateGroupCarEntry();
                sKeepPaceWithTarget = 1;
            }
            else
            {
                gObjectiveMayAct = 0;
                sKeepPaceWithTarget = 0;
            }
        }
        else if (!mBf21C.bPanicking)
        {
            gObjectiveMayAct = 1;
            sKeepPaceWithTarget = 1;
        }
        else
        {
            sKeepPaceWithTarget = 0;
        }

        mBf21C.bLeftVehicle = false;
        Ped::ProcessMainObjective();
        Ped::ProcessInternalObjective();
        if (mPedState1 > ped_state_1::walking_0 && mPedState1 <= ped_state_1::standing_still_7)
        {
            Ped::UpdateFacingAngle();
        }
        if (mPedType != ped_type::player_2)
        {
            if (mObjective != objectives_enum::flee_char_on_foot_always_3 &&
                mObjective != objectives_enum::flee_char_always_once_car_stopped_6)
            {
                if (mInternalObjective != objectives_enum::flee_char_on_foot_always_3 && mInternalObjective != objectives_enum::run_to_char_7)
                {
                    Ped::Threat_Reaction_AI();
                    if (mAttacker)
                    {
                        if (!mBf21C.bPanicking)
                        {
                            Ped::ReactToAttacker();
                        }
                        mAttacker = NULL;
                    }
                }
            }
        }
    }

    if (mInternalObjectiveTimer != 9999)
    {
        if (mInternalObjectiveTimer > 0)
        {
            mInternalObjectiveTimer--;
        }
    }

    if (mObjectiveTimer != 9999)
    {
        if (mObjective != objectives_enum::time_waited_in_car_31 &&
            mObjective != objectives_enum::goto_area_any_means_13 &&
            mObjective != objectives_enum::kill_char_any_means_19 && mObjective != objectives_enum::goto_area_in_car_14)
        {
            if (mObjectiveTimer > 0)
            {
                mObjectiveTimer = mObjectiveTimer - 1;
            }
        }
    }
}

MATCH_FUNC(0x4624a0)
void Ped::ReleaseGroupSpritesAndWeapons()
{
    if (mPedGroup)
    {
        mPedGroup->DestroyGroup();
    }

    if (mGameObject)
    {
        if (mGameObject->mObj2c.mP18)
        {
            mGameObject->mObj2c.DestroyAllSprites();
        }
    }

    if (mId)
    {
        if (mSelectedWeapon)
        {
            RemovePedWeapons();
        }
        if (mpWeapon)
        {
            RemoveSecondaryWeapon();
        }
        mCarWeapon = NULL;
    }
}

MATCH_FUNC(0x462510)
void Ped::RemovePedWeapons()
{
    if (mSelectedWeapon)
    {
        ClearAttacking_403A40();
        gWeapon_8->deallocate(mSelectedWeapon);
        mSelectedWeapon = NULL;
    }
}

MATCH_FUNC(0x462550)
void Ped::RemoveSecondaryWeapon()
{
    if (mpWeapon)
    {
        ClearAttacking_403A40();
        gWeapon_8->deallocate(mpWeapon);
        mpWeapon = NULL;
    }
}

MATCH_FUNC(0x462590)
void Ped::ForceDoNothing()
{
    SetObjective(objectives_enum::no_obj_0);
    SetObjective2(objectives_enum::no_obj_0);

    mBf21C.bPanicking = false;

    if (mCar)
    {
        mPedState1 = ped_state_1::in_car_10;
        mPedState2 = ped_state_2::ped2_driving_10;

        if (mCar->mAI)
        {
            if (mCar->mAI->mRouteIdx > 0)
            {
                gRouteFinder->CancelRoute(mCar->mAI->mRouteIdx);
            }
        }

        if (mCar->mpChaseTask)
        {
            gCarChaseTaskTable->FreeEntry(mCar->mpChaseTask);
            mCar->mpChaseTask = NULL;
        }
    }
}

MATCH_FUNC(0x462620)
void Ped::UpdateOnScreenCounter()
{
    if (mPedState1 == ped_state_1::dead_9 || mPedState1 == ped_state_1::immobilized_8)
    {
        gRunPedThink = 0;
        mBf21C.bAttacking = false;
    }
    else
    {
        if (mGameObject->GetCharState() == Char_B4_state::Jumping_15)
        {
            gRunPedThink = mPedState1 == ped_state_1::entering_car_3;
        }
        mBf21C.bAttacking = false;
        mGameObject->SetSpriteNum(24);
    }
    if (mGameObject->IsOnScreen() == true)
    {
        mOffscreenCounter = 0;
        ++gNumPedsOnScreen;
    }
}

MATCH_FUNC(0x4626b0)
bool Ped::StateMachineTick()
{
    switch (this->mPedType)
    {
        case ped_type::player_2:
            if (bDo_invulnerable)
            {
                this->mInvulnerability = 9999;
            }

            if (this->mSelectedWeapon)
            {
                this->mSelectedWeapon->SetTargetHit(0);
            }

            this->mThreatSearch = threat_search_enum::no_threats_0;
            this->mThreatReaction = threat_reaction_enum::no_reaction_0;

            if (this->mOccupation != ped_ocupation_enum::empty && this->mOccupation != ped_ocupation_enum::player)
            {
                this->mOccupation = ped_ocupation_enum::player;
            }
            this->mJumpOverMode = ped_jump_over_mode::can_jump_2;
            this->mPlayer->mJumping = false;

            if (this->mGameObject)
            {
                if (this->mObjectiveStatus == objective_status::failed_2)
                {
                    Ped::SetObjective(objectives_enum::no_obj_0);
                    Ped::SetObjective2(objectives_enum::no_obj_0);
                }

                if (this->mObjective == objectives_enum::leave_car_36 &&
                    (this->mObjectiveStatus == objective_status::passed_1 ||
                     this->mGameObject->GetCharState() == Char_B4_state::Jumping_15))
                {
                    Ped::SetObjective(objectives_enum::no_obj_0);
                    Ped::SetObjective2(objectives_enum::no_obj_0);
                }
                ++gNumPedsOnScreen;
                ++this->mOffscreenCounter;
                sKeepPaceWithTarget = 1;
                if (this->mGameObject->GetCharState() == Char_B4_state::Jumping_15)
                {
                    gRunPedThink = this->mPedState1 == ped_state_1::entering_car_3;
                }

                if (mPedState1 == ped_state_1::dead_9 || mPedState1 == ped_state_1::immobilized_8)
                {
                    gRunPedThink = 0;
                }
                else
                {
                    this->mGameObject->SetSpriteNum(25);
                }
                if (mGameObject->IsOnScreen() == true)
                {
                    this->mOffscreenCounter = 0;
                }
                if (Ped::GetMoveDirection() == kFpZero_678660 && Ped::GetTurnSpeed() == gDummyPedAng)
                {
                    if (mPedState1 == ped_state_1::walking_0 && GetGameObjectVelocity() == kFpZero_678660)
                    {
                        Ped::ChangeNextPedState1(ped_state_1::standing_still_7);
                        Ped::ChangeNextPedState2(ped_state_2::ped2_staying_14);
                    }
                }
                else
                {
                    if (this->mPedState1 == ped_state_1::standing_still_7)
                    {
                        Ped::ChangeNextPedState1(ped_state_1::walking_0);
                        Ped::ChangeNextPedState2(ped_state_2::ped2_walking_0);
                    }
                    if (this->mPedState1 == ped_state_1::entering_car_3 &&
                        this->mPedState2 == ped_state_2::ped2_following_a_car_4)
                    {
                        Ped::ChangeNextPedState1(ped_state_1::walking_0);
                        Ped::ChangeNextPedState2(ped_state_2::ped2_walking_0);
                        Ped::SetObjective(objectives_enum::no_obj_0);
                        Ped::SetObjective2(objectives_enum::no_obj_0);
                    }
                }
                if (this->mGameObject->GetCharState() == Char_B4_state::Jumping_15)
                {
                    this->mPlayer->mJumping = true;
                }
                return true;
            }
            this->mShockCounter = 0;
            ++this->mOffscreenCounter;
            if (this->mCar->mCarSprite &&
                gGame_0x40->IsSpriteInAnyPlayerBounds(this->mCar->mCarSprite, kFpZero_678660))
            {
                this->mOffscreenCounter = 0;
            }
            return true;

        case ped_type::script_created_5:
            this->mElectrocutionThreshold = 100;
            this->mJumpOverMode = ped_jump_over_mode::can_jump_2;
            if (this->mGameObject)
            {
                ++this->mOffscreenCounter;
                Ped::UpdateOnScreenCounter();
                return true;
            }
            ++this->mOffscreenCounter;
            if (this->mCar->mCarSprite &&
                gGame_0x40->IsSpriteInAnyPlayerBounds(this->mCar->mCarSprite, kFpZero_678660))
            {
                this->mOffscreenCounter = 0;
            }
            return true;

        case ped_type::special_ped_4:
        case ped_type::dummy_with_occupation_6:
            this->mElectrocutionThreshold = 100;
            if (this->mOccupation == ped_ocupation_enum::armed_gang_member_19 && this->mPedState1 == ped_state_1::dead_9)
            {
                --gNumberArmedGangMembers;
                this->mOccupation = ped_ocupation_enum::dummy;
            }
            if (this->mGameObject)
            {
                ++this->mOffscreenCounter;
                if (this->mOffscreenCounter > 100u)
                {
                    if (this->mOccupation == ped_ocupation_enum::elvis_leader)
                    {
                        if (!this->mPedGroup)
                        {
                            // The original jumps to the last Deallocate block of the dummy_3 case here.
                            goto deallocate_dead;
                        }
                        if (this->mOffscreenCounter > 500u && this->mPedGroup->AreAllMembersOffScreen())
                        {
                            Ped::Deallocate();
                            return false;
                        }
                    }
                    else if (this->mOccupation != ped_ocupation_enum::special_groups_member && !Ped::IsEmergencyOccupation())
                    {
                        Ped::Deallocate();
                        return false;
                    }
                }
                Ped::UpdateOnScreenCounter();
                return true;
            }
            ++this->mOffscreenCounter;
            if (this->mCar && this->mCar->mCarSprite &&
                gGame_0x40->IsSpriteInAnyPlayerBounds(this->mCar->mCarSprite, kFpZero_678660))
            {
                this->mOffscreenCounter = 0;
            }
            if (this->mOffscreenCounter > 60u && this->mCar && this->mOccupation == ped_ocupation_enum::car_thief)
            {
                this->mCar->mPedType = ped_type::dummy_3;
            }
            if (this->mPedState1 == ped_state_1::dead_9)
            {
                Ped::Deallocate();
                return false;
            }
            return true;

        case ped_type::dummy_3:
            this->mElectrocutionThreshold = 100;
            if (this->mGameObject)
            {
                u16 offscreen_counter = ++this->mOffscreenCounter;
                if (this->mPedState1 == ped_state_1::flee_or_running_1)
                {
                    if (offscreen_counter == 200)
                    {
                        Ped::Deallocate();
                        return false;
                    }
                }
                else if (offscreen_counter >= gPedOffscreenDespawnTime)
                {
                    Ped::Deallocate();
                    return false;
                }
                Ped::UpdateOnScreenCounter();
                return true;
            }
            ++this->mOffscreenCounter;
            if (this->mCar->mCarSprite &&
                gGame_0x40->IsSpriteInAnyPlayerBounds(this->mCar->mCarSprite, kFpZero_678660))
            {
                this->mOffscreenCounter = 0;
            }
            if (this->mOccupation != ped_ocupation_enum::dummy_driver_4)
            {
                if (this->mCar->IsDespawning())
                {
                    Ped::Deallocate();
                    return false;
                }
                if (this->mPedState1 == ped_state_1::dead_9)
                {
                deallocate_dead:
                    Ped::Deallocate();
                    return false;
                }
                return true;
            }
            return false;

        default:
            return true;
    }
}

MATCH_FUNC(0x462b80)
void Ped::UpdateCharB4()
{
    mGameObject->Set_F8_ped_state_1_433910(mPedState1);
    mGameObject->SetPedState2(mPedState2);
    mGameObject->Update(gDistanceToTarget);
    if (mGameObject)
    {
        mCam.x = mGameObject->get_sprite_xpos();
        mCam.y = mGameObject->get_sprite_ypos();
        mCam.z = mGameObject->get_sprite_zpos();
        Char* pChar = mGameObject;
        mFacingAngle = pChar->get_rotation();

        if (mPedState1 == ped_state_1::in_car_10)
        {
            mCar = pChar->Get_F84();
            if (pChar->mObj2c.mP18)
            {
                pChar->mObj2c.DestroyAllSprites();
            }
            gChar_B4_Pool->DeAllocate(mGameObject);

            mGameObject = NULL;
            if (!mEnterCarAsPassenger)
            {
                mCar->AssignDriver(this);
            }
            else
            {
                mCar->ShowCarName(this);
                if (mInternalObjective == objectives_enum::enter_train_37 && mPedType == ped_type::dummy_3 ||
                    (mCar->mPassengersList.AddPedToFront(this), mPedType == ped_type::dummy_3))
                {
                    if (mInternalObjective == objectives_enum::enter_train_37)
                    {
                        Train* pTrain = gPublicTransport_181C->GetTrainFromCarExcludingLeadCar(mCar);
                        ++pTrain->mPassengerCount;
                    }
                }
            }
            if ((mInternalObjective == objectives_enum::enter_car_as_driver_35 || mInternalObjective == objectives_enum::enter_train_37) && (mInternalObjectiveStatus = objective_status::passed_1, mInternalObjective == objectives_enum::enter_train_37))
            {
                if (mPedType == ped_type::dummy_3)
                {
                    Ped::Deallocate();
                }
            }
            else
            {
                Car_Door* pDoor = mCar->GetDoor(mTargetCarDoor);
                if (mOccupation != ped_ocupation_enum::bus_customer_8 && mOccupation != ped_ocupation_enum::train_customer_9)
                {
                    pDoor->Close();
                }
                pDoor->set_ped(NULL);
            }
        }
    }
}

MATCH_FUNC(0x462e70)
bool Ped::PoolUpdate()
{
    if (Is_occupation_elvis())
    {
        if (gElvisVoiceTimer > 0)
        {
            --gElvisVoiceTimer;
        }
        else
        {
            SetVoiceEventIfNotOnFire(ped_voice_event::elvis_ambient_23);
            gElvisVoiceTimer = gRng.get_int(300) + 450;
        }
    }

    if (mEscapeCar)
    {
        if (mEscapeCar->IsDespawning())
        {
            mEscapeCar = NULL;
        }
    }

    switch (mPedType)
    {
        case ped_type::dummy_3:
            ++gNumDummyChars;
            break;
        case ped_type::special_ped_4:
        case ped_type::dummy_with_occupation_6:
            mJumpOverMode = ped_jump_over_mode::can_jump_2;
            if (mThreatReaction == threat_reaction_enum::react_as_emergency_1)
            {
                mJumpOverMode = ped_jump_over_mode::cannot_jump_1;
                ++gNumEmergencyPeds;
            }
            else
            {
                ++gNumDummyChars;
            }
            break;
        case ped_type::script_created_5:
            ++gNumScriptCreatedPeds;
            break;
        default:
            break;
    }
    if (mBf21C.bScheduledForRemoval)
    {
        Ped::DeallocateWithGroupCleanup();
    }
    Ped::UpdateKillerIdTimer();
    Ped::ManageBurning();
    Ped::ManageShocking();
    if (!mLifetimeTimer)
    {
        Ped::ReleaseGroupSpritesAndWeapons();
        return true;
    }
    if (mRecentCrimeTimer > 0)
    {
        mRecentCrimeTimer--;
    }
    ++gNumPedsUpdated;
    gTargetSearchMode = threat_search_enum::no_threats_0;
    gSearchingPed = NULL;
    gCloseRangeHit = 0;
    sPedUsedPathThisTick = 0;
    mPrevAttackersCount = mAttackersCount;
    mAttackersCount = 0;
    mBf21C.bSpottedPlayer = false;
    if (mBf21C.bBusted && mPedState1 != ped_state_1::immobilized_8)
    {
        Ped::ChangeNextPedState1(ped_state_1::immobilized_8);
        Ped::ChangeNextPedState2(ped_state_2::lying_on_floor_22);
        SetStateInitPending();
    }

    if (static_cast<u32>(mShockCounter) > mElectrocutionThreshold)
    {
        mShockCounter = mElectrocutionThreshold;
    }
    if (mShockCounter == mElectrocutionThreshold && mPedState1 != ped_state_1::immobilized_8 &&
        mPedState2 != ped_state_2::electrocuted_27)
    {
        Ped::ChangeNextPedState1(ped_state_1::immobilized_8);
        Ped::ChangeNextPedState2(ped_state_2::electrocuted_27);
        SetStateInitPending();
    }
    if (gPathStepRoundDone == 1)
    {
        mBf21C.bPathSteppedThisRound = false;
    }
    if (gLosCheckRoundDone == 1)
    {
        mBf21C.bLosCheckedThisRound = false;
    }
    if (mBf21C.bActive == 1)
    {
        gRunPedThink = 1;
        if (Ped::StateMachineTick())
        {
            if (gRunPedThink)
            {
                Ped::UpdateAI();
                gLastProcessedPed = this;
                Ped::ManageWeapon();
                if (mWantedPoints)
                {
                    if (bSkip_police || mOccupation == ped_ocupation_enum::empty)
                    {
                        mWantedPoints = cop_level_ped_enum::cop_0_stars;
                    }
                    else if (mPedType == ped_type::player_2 && mId)
                    {
                        if (mPedState2 == ped_state_2::ped2_respawned_29)
                        {
                            mWantedPoints = cop_level_ped_enum::cop_0_stars;
                        }
                        else
                        {
                            gPolice_7B8->RegisterCriminal(this);
                        }
                    }
                }
            }
            else
            {
                if (mPedState1 == ped_state_1::dead_9)
                {
                    if ((!mPedGroup || !IsGroupLeader()) && mObjective != objectives_enum::dead_patient_28)
                    {
                        Ped::SetObjective(objectives_enum::no_obj_0);
                        Ped::SetObjective2(objectives_enum::no_obj_0);
                        mPedState1 = ped_state_1::dead_9;
                        mPedState2 = ped_state_2::ped2_dead_15;
                    }
                    if (mOffscreenCounter > 200u && mOccupation != ped_ocupation_enum::paramedic_23 && mPedType != ped_type::script_created_5)
                    {
                        Ped::Deallocate();
                    }
                }
                else
                {
                    Ped::Occupation_AI();
                    if (mObjective != objectives_enum::no_obj_0 || mInternalObjective)
                    {
                        gObjectiveMayAct = 0;
                        Ped::ProcessObjective();
                    }
                    if (mGameObject->GetCharState() == Char_B4_state::Jumping_15)
                    {
                        if (mPedState1 > ped_state_1::walking_0 && mPedState1 <= ped_state_1::standing_still_7) // not walking neither dead/immobilized
                        {
                            Ped::UpdateFacingAngle();
                        }
                    }
                }
            }

            if (!sPedUsedPathThisTick && mBf21C.bInPathList)
            {
                gPathFinder->mPedList.RemovePed_471240(this);
                mBf21C.bInPathList = false;
            }

            if (mGameObject)
            {
                Fix16 zpos = get_cam_z();
                if (!mGameObject->mFlagsBf.bOnSlope && zpos != kFpZero_678660)
                {
                    zpos -= kFpOne_678664;
                }
                mBlockSpec = gMap_0x370->GetBlockSpec(get_cam_x(), get_cam_y(), zpos);
                Ped::UpdateCharB4();

                mBf21C.bHitByAttacker = false;
                mBf21C.bNoWeapon = false;
            }
            else
            {
                mCam.x = mCar->mCarSprite->mXy.x;
                mCam.y = mCar->mCarSprite->mXy.y;
                mCam.z = mCar->mCarSprite->mZpos;
            }
        }
        else
        {
            return false;
        }
    }
    else if (mLifetimeTimer != ped_lifetime::never_expires_99)
    {
        --mLifetimeTimer;

        if (mLifetimeTimer == ped_lifetime::remove_0)
        {
            mLifetimeTimer = ped_lifetime::remove_0;
        }
    }

    if (mInvulnerability)
    {
        if (mInvulnerability != 9999)
        {
            mInvulnerability--;
        }
    }
    return false;
}

MATCH_FUNC(0x4632e0)
void Ped::ProcessObjective()
{
    ProcessMainObjective();
    ProcessInternalObjective();
}

MATCH_FUNC(0x463300)
void Ped::ChangePedStatesByMode(u8 mode)
{
    switch (mode)
    {
        case ped_states_mode::walk_1:
            Ped::ChangeNextPedState1(ped_state_1::walking_0);
            Ped::ChangeNextPedState2(ped_state_2::ped2_walking_0);
            break;
        case ped_states_mode::flee_2:
            Ped::ChangeNextPedState1(ped_state_1::flee_or_running_1);
            Ped::ChangeNextPedState2(ped_state_2::running_away_3);
            break;
        case ped_states_mode::run_to_target_3:
            Ped::ChangeNextPedState1(ped_state_1::flee_or_running_1);
            Ped::ChangeNextPedState2(ped_state_2::running_to_target_2);
            break;
        case ped_states_mode::stand_still_4:
            Ped::ChangeNextPedState1(ped_state_1::standing_still_7);
            Ped::ChangeNextPedState2(ped_state_2::ped2_staying_14);
            break;
        case ped_states_mode::drive_5:
            Ped::ChangeNextPedState1(ped_state_1::in_car_10);
            Ped::ChangeNextPedState2(ped_state_2::ped2_driving_10);
            break;
        case ped_states_mode::go_to_car_6:
            Ped::ChangeNextPedState1(ped_state_1::entering_car_3);
            Ped::ChangeNextPedState2(ped_state_2::ped2_following_a_car_4);
            break;
        case ped_states_mode::exit_car_7:
            Ped::ChangeNextPedState1(ped_state_1::exiting_car_4);
            Ped::ChangeNextPedState2(ped_state_2::ped2_driving_10);
            break;
        default:
            return;
    }
}

MATCH_FUNC(0x4633e0)
void Ped::SetStatesForObjective(bool bMainObj)
{
    u8 mode = ped_states_mode::unchanged_99;
    s32 objective;
    if (bMainObj)
    {
        objective = mObjective;
    }
    else
    {
        objective = mInternalObjective;
    }
    switch (objective)
    {
        case objectives_enum::no_obj_0:
            mode = mCar ? ped_states_mode::drive_5 : ped_states_mode::walk_1;
            break;
        case objectives_enum::flee_on_foot_till_safe_1:
        case objectives_enum::flee_char_on_foot_till_safe_2:
        case objectives_enum::flee_char_on_foot_always_3:
            mode = ped_states_mode::flee_2;
            break;
        case objectives_enum::guard_spot_24:
        case objectives_enum::guard_area_25:
        case objectives_enum::wait_on_foot_26:
        case objectives_enum::wait_for_train_29:
        case objectives_enum::enter_bus_30:
        case objectives_enum::wait_to_cross_y_backwards_44:
        case objectives_enum::wait_to_cross_x_forwards_45:
        case objectives_enum::wait_to_cross_y_forwards_46:
        case objectives_enum::wait_to_cross_x_backwards_47:
            mode = ped_states_mode::stand_still_4;
            break;
        case objectives_enum::goto_area_in_car_14:
        case objectives_enum::wait_in_car_27:
        case objectives_enum::time_waited_in_car_31:
        case objectives_enum::patrol_in_car_43:
        case objectives_enum::chase_char_in_car_52:
        case objectives_enum::call_police_car_54:
        case objectives_enum::follow_car_in_car_55:
        case objectives_enum::fire_at_object_from_vehicle_57:
            mode = ped_states_mode::drive_5;
            break;
        case objectives_enum::enter_car_as_driver_35:
        case objectives_enum::enter_train_37:
            mode = ped_states_mode::go_to_car_6;
            break;
        case objectives_enum::leave_car_36:
        case objectives_enum::leave_train_38:
            mode = mGameObject ? ped_states_mode::walk_1 : ped_states_mode::exit_car_7;
            break;
        case objectives_enum::run_to_char_7:
        case objectives_enum::follow_char_9:
        case objectives_enum::stay_close_to_char_11:
        case objectives_enum::goto_area_on_foot_12:
        case objectives_enum::goto_char_on_foot_16:
        case objectives_enum::walk_to_point_17:
        case objectives_enum::walk_to_car_door_18:
        case objectives_enum::kill_char_on_foot_20:
        case objectives_enum::punch_char_23:
        case objectives_enum::pull_driver_out_32:
        case objectives_enum::crossing_road_48:
        case objectives_enum::follow_car_on_foot_with_offset_56:
        case objectives_enum::destroy_object_58:
        case objectives_enum::destroy_car_59:
            mode = ped_states_mode::run_to_target_3;
            break;
        case objectives_enum::left_behind_by_leader_8:
        case objectives_enum::kill_frenzy_22:
        case objectives_enum::wait_after_crossing_49:
        case objectives_enum::wander_until_timeout_51:
            mode = ped_states_mode::walk_1;
            break;
        case objectives_enum::dead_no_ambulance_50:
            Ped::ChangeNextPedState1(ped_state_1::dead_9);
            Ped::ChangeNextPedState2(ped_state_2::ped2_dead_15);
            break;
        default:
            break;
    }
    Ped::ChangePedStatesByMode(mode);
}

MATCH_FUNC(0x463570)
void Ped::SetObjective(s32 objective, s16 objective_timer)
{
    if (this->mPedState1 != ped_state_1::dead_9 || objective == objectives_enum::dead_patient_28)
    {
        this->mObjective = objective;
        this->mObjectiveTimer = objective_timer;
        this->mTargetX = Fix16(-1);
        this->mTargetY = Fix16(-1);
        this->mTargetZ = Fix16(-1);
        this->mObjectiveTargetX = kFpZero_678660;
        this->mObjectiveTargetY = kFpZero_678660;
        this->mObjectiveTargetPed = NULL;
        this->mTargetObjectiveCar = NULL;
        this->mObjectiveTargetObject = NULL;
        this->mObjectiveStatus = objective_status::not_finished_0;

        u8 mode = ped_states_mode::unchanged_99;

        mBf21C.bPanicking = false;
        mBf21C.bHoldPosition = false;

        switch (objective)
        {
            case objectives_enum::no_obj_0:
                mode = mCar != NULL ? ped_states_mode::drive_5 : ped_states_mode::walk_1;
                break;

            case objectives_enum::flee_on_foot_till_safe_1:
            case objectives_enum::flee_char_on_foot_till_safe_2:
            case objectives_enum::flee_char_on_foot_always_3:
                mode = ped_states_mode::flee_2;
                break;

            case objectives_enum::kill_char_on_foot_20:
            case objectives_enum::punch_char_23:
            case objectives_enum::destroy_object_58:
            case objectives_enum::destroy_car_59:
                mode = ped_states_mode::run_to_target_3;
                SetObjective2(objectives_enum::no_obj_0);
                break;

            case objectives_enum::guard_spot_24:
            case objectives_enum::guard_area_25:
            case objectives_enum::wait_on_foot_26:
                this->mObjectiveTargetX = this->mCam.x;
                this->mObjectiveTargetY = this->mCam.y;
                this->mObjectiveTargetZ = this->mCam.z;
                mode = ped_states_mode::stand_still_4;
                break;

            case objectives_enum::goto_area_in_car_14:
            case objectives_enum::wait_in_car_27:
            case objectives_enum::time_waited_in_car_31:
            case objectives_enum::patrol_in_car_43:
            case objectives_enum::chase_char_in_car_52:
            case objectives_enum::call_police_car_54:
            case objectives_enum::follow_car_in_car_55:
            case objectives_enum::fire_at_object_from_vehicle_57:
            case objectives_enum::turret_put_out_car_fire_60:
            case objectives_enum::tank_fire_at_player_61:
                mode = ped_states_mode::drive_5;
                break;

            case objectives_enum::enter_car_as_driver_35:
            case objectives_enum::enter_train_37:
                mode = ped_states_mode::go_to_car_6;
                break;

            case objectives_enum::leave_car_36:
            case objectives_enum::leave_train_38:
                mode = mGameObject != NULL ? ped_states_mode::walk_1 : ped_states_mode::exit_car_7;
                break;

            case objectives_enum::patrol_on_foot_42:
            {
                PatrolRoute* pPatrolRoute = gPatrolRoutePool->AllocPatrolList(&mPatrolListIdx);
                mPatrolRoute = pPatrolRoute;
                PatrolPoint* pPoint = pPatrolRoute->mPoints;
                while (pPoint->mX)
                {
                    pPoint->mX = 0;
                    pPoint++;
                }
                break;
            }

            case objectives_enum::goto_area_on_foot_12:
            case objectives_enum::goto_char_on_foot_16:
            case objectives_enum::pull_driver_out_32:
            case objectives_enum::follow_car_on_foot_with_offset_56:
                SetObjective2(objectives_enum::no_obj_0);
                // fall through
            case objectives_enum::kill_frenzy_22:
                mode = ped_states_mode::run_to_target_3;
                break;

            case objectives_enum::dead_patient_28:
                if (!gAmbulance_110->TryAddPatient(this))
                {
                    this->mObjective = objectives_enum::no_obj_0;
                    return;
                }
                break;

            case objectives_enum::left_behind_by_leader_8:
            case objectives_enum::wander_until_timeout_51:
                mode = ped_states_mode::walk_1;
                break;

            case objectives_enum::dead_no_ambulance_50:
                ChangeNextPedState1(ped_state_1::dead_9);
                ChangeNextPedState2(ped_state_2::ped2_dead_15);
                break;

            default:
                break;
        }
        ChangePedStatesByMode(mode);
    }
}

MATCH_FUNC(0x463830)
void Ped::SetObjective2(s32 internal_objective, s16 internal_objective_timer)
{

    u8 x_int;
    u8 y_int;
    u8 z_int;
    u8 mode = ped_states_mode::unchanged_99;
    if (this->mPedState1 != ped_state_1::dead_9)
    {
        if (this->mPedState2 == ped_state_2::ped2_entering_a_car_6 ||
            this->mPedState2 == ped_state_2::ped2_getting_out_a_car_7)
        {
            Car_Door* pDoor;
            if (this->mTargetToEnter)
            {
                pDoor = this->mTargetToEnter->GetDoor(this->mTargetCarDoor);
            }
            else if (this->mGameObject)
            {
                pDoor = this->mGameObject->mTargetCar->GetDoor(this->mTargetCarDoor);
            }
            else
            {
                pDoor = this->mCar->GetDoor(this->mTargetCarDoor);
            }
            pDoor->Close();
            pDoor->set_ped(NULL);
        }

        this->mInternalObjective = internal_objective;
        this->mInternalObjectiveTimer = internal_objective_timer;
        this->mMoveTargetX = Fix16(-1);
        this->mMoveTargetY = Fix16(-1);
        this->mMoveTargetZ = Fix16(-1);
        this->mInternalTargetX = kFpZero_678660;
        this->mInternalTargetY = kFpZero_678660;
        this->mInternalTargetZ = kFpZero_678660;
        this->mInternalTargetPed = NULL;
        this->mTargetToEnter = NULL;
        this->mInternalTargetObject = NULL;
        this->mInternalObjectiveStatus = objective_status::not_finished_0;
        switch (internal_objective)
        {
            case objectives_enum::flee_on_foot_till_safe_1:
            case objectives_enum::flee_char_on_foot_till_safe_2:
            case objectives_enum::flee_char_on_foot_always_3:
                mode = ped_states_mode::flee_2;
                break;
            case objectives_enum::run_to_char_7:
            case objectives_enum::follow_char_9:
            case objectives_enum::stay_close_to_char_11:
            case objectives_enum::goto_area_on_foot_12:
            case objectives_enum::walk_to_car_door_18:
            case objectives_enum::kill_char_on_foot_20:
            case objectives_enum::punch_char_23:
            case objectives_enum::crossing_road_48:
                mode = ped_states_mode::run_to_target_3;
                break;
            case objectives_enum::enter_car_as_driver_35:
                mode = ped_states_mode::go_to_car_6;
                break;
            case objectives_enum::enter_train_37:
                Ped::ChangeNextPedState1(ped_state_1::entering_train_5);
                Ped::ChangeNextPedState2(ped_state_2::entering_train_5);
                break;
            case objectives_enum::leave_train_38:
                Ped::ChangeNextPedState1(ped_state_1::leaving_train_6);
                Ped::ChangeNextPedState2(ped_state_2::ped2_driving_10);
                break;
            case objectives_enum::leave_car_36:
                mode = ped_states_mode::exit_car_7;
                break;
            case objectives_enum::walk_to_point_17:
            {
                mode = ped_states_mode::run_to_target_3;
                x_int = this->mCam.x.ToUInt8();
                y_int = this->mCam.y.ToUInt8();
                z_int = this->mCam.z.ToUInt8();
                gMap_0x370->FindNearbyBlockOfType(&x_int, &y_int, &z_int, 2);
                this->mInternalTargetX = kFpHalf_67853C + Fix16(x_int);
                this->mInternalTargetY = kFpHalf_67853C + Fix16(y_int);
                this->mInternalTargetZ = kFpOne_678664 + Fix16(z_int);
                break;
            }
            case objectives_enum::goto_area_in_car_14:
                mode = ped_states_mode::drive_5;
                break;
            case objectives_enum::wait_on_foot_26:
            case objectives_enum::wait_for_train_29:
            case objectives_enum::enter_bus_30:
            case objectives_enum::wait_to_cross_y_backwards_44:
            case objectives_enum::wait_to_cross_x_forwards_45:
            case objectives_enum::wait_to_cross_y_forwards_46:
            case objectives_enum::wait_to_cross_x_backwards_47:
                mode = ped_states_mode::stand_still_4;
                break;
            case objectives_enum::no_obj_0:
            case objectives_enum::kill_frenzy_22:
            case objectives_enum::wait_after_crossing_49:
                mode = ped_states_mode::walk_1;
                break;
            default:
                break;
        }
        Ped::ChangePedStatesByMode(mode);
    }
}

MATCH_FUNC(0x463aa0)
void Ped::ProcessMainObjective()
{
    Ang16 door_angle = 0;
    if (mObjective && mObjectiveStatus == objective_status::not_finished_0)
    {
        if (mObjectiveTargetPed)
        {
            mTargetX = mObjectiveTargetPed->get_cam_x();
            mTargetY = mObjectiveTargetPed->get_cam_y();
            mTargetZ = mObjectiveTargetPed->get_cam_z();
        }
        else if (mTargetObjectiveCar)
        {
            u8 door_count = mTargetObjectiveCar->GetDoorCount();
            if (mTargetCarDoor >= door_count)
            {
                mTargetCarDoor = door_count - 1;
            }
            if (mTargetObjectiveCar->IsDespawning())
            {
                mTargetX = mCam.x;
                mTargetY = mCam.y;
                mTargetZ = mCam.z;
            }
            else if (mObjective != objectives_enum::walk_to_car_door_18 &&
                     (mObjective <= objectives_enum::leave_car_and_flee_34 || mObjective > objectives_enum::leave_train_38))
            {
                mTargetX = mTargetObjectiveCar->mCarSprite->mXy.x;
                mTargetY = mTargetObjectiveCar->mCarSprite->mXy.y;
                mTargetZ = mTargetObjectiveCar->mCarSprite->mZpos;
            }
            else
            {
                CarDoorAlignmentSolver(0,
                                       mTargetObjectiveCar,
                                       mTargetCarDoor,
                                       mTargetX,
                                       mTargetY,
                                       door_angle);
                mTargetZ = mTargetObjectiveCar->mCarSprite->mZpos;
            }
        }
        else
        {
            if (mObjectiveTargetObject)
            {
                mTargetX = mObjectiveTargetObject->GetX();
                mTargetY = mObjectiveTargetObject->GetY();
                mTargetZ = mObjectiveTargetObject->GetZ();
            }
            else if (mObjectiveTargetX != kFpZero_678660 && mObjectiveTargetY != kFpZero_678660)
            {
                mTargetY = mObjectiveTargetY;
                mTargetX = mObjectiveTargetX;
                mTargetZ = mObjectiveTargetZ;
            }
        }

        Fix16 diff_x = mTargetX - mCam.x;
        Fix16 diff_y = mTargetY - mCam.y;
        diff_x = Fix16::Abs(diff_x);
        diff_y = Fix16::Abs(diff_y);
        gDistanceToTarget = Fix16::Max(diff_x, diff_y);

        switch (mObjective)
        {
            case objectives_enum::flee_on_foot_till_safe_1:
                Ped::ProcessObjectiveFleeOnFootTillSafe();
                break;
            case objectives_enum::flee_char_on_foot_till_safe_2:
                Ped::FleeCharOnFootTillSafe();
                break;
            case objectives_enum::flee_char_on_foot_always_3:
                Ped::FleeFromCharOnFootAlways();
                break;
            case objectives_enum::flee_char_any_means_till_safe_4:
                FleeCharAnyMeansTillSafe_Nullsub();
                break;
            case objectives_enum::flee_char_any_means_always_5:
                FleeCharAnyMeansAlways_Nullsub();
                break;
            case objectives_enum::flee_char_always_once_car_stopped_6:
                Ped::FleeCharAlwaysOnceCarStopped();
                break;

            case objectives_enum::leave_car_and_flee_34:
                Ped::LeaveCarAndFleeFromIt();
                break;
            case objectives_enum::kill_char_on_foot_20:
                Ped::KillCharOnFoot();
                break;
            case objectives_enum::kill_char_any_means_19:
                Ped::KillCharAnyMeans();
                break;
            case objectives_enum::kill_car_21:
                KillCar_Nullsub();
                break;
            case objectives_enum::kill_frenzy_22:
                Ped::KillFrenzy();
                break;
            case objectives_enum::punch_char_23:
                Ped::PunchChar();
                break;
            case objectives_enum::wait_on_foot_26:
                Ped::WaitOnFootAtTarget();
                break;
            case objectives_enum::guard_spot_24:
                Ped::GuardSpot();
                break;
            case objectives_enum::guard_area_25:
                Ped::GuardArea();
                break;
            case objectives_enum::time_waited_in_car_31:
                Ped::TimeWaitedInCar();
                break;
            case objectives_enum::goto_area_in_car_14:
                Ped::ProcessObjectiveGotoAreaInCar();
                break;
            case objectives_enum::enter_car_as_driver_35:
                Ped::EnterTargetObjectiveCar();
                break;
            case objectives_enum::leave_car_36:
                Ped::LeaveTargetObjectiveCar();
                break;
            case objectives_enum::patrol_on_foot_42:
                Ped::PatrolOnFoot();
                break;
            case objectives_enum::goto_char_on_foot_16:
                Ped::UpdateFollowPedObjective();
                break;
            case objectives_enum::goto_area_on_foot_12:
                Ped::ProcessObjectiveGotoAreaOnFoot();
                break;
            case objectives_enum::goto_area_any_means_13:
                Ped::GotoAreaByAnyMeans();
                break;
            case objectives_enum::wander_until_timeout_51:
                Ped::FailWhenObjectiveTimerExpired();
                break;
            case objectives_enum::left_behind_by_leader_8:
                Ped::UpdateGroupMemberFollow();
                break;
            case objectives_enum::enter_train_37:
                Ped::EnterTrain();
                break;
            case objectives_enum::leave_train_38:
                Ped::LeaveTrain();
                break;
            case objectives_enum::leave_car_and_stop_33:
                Ped::LeaveCarThenStop();
                break;
            case objectives_enum::dead_no_ambulance_50:
                Objective50_Nullsub();
                break;
            case objectives_enum::patrol_in_car_43:
                Ped::SetCarAiSpeedOne();
                break;
            case objectives_enum::chase_char_in_car_52:
                Ped::SetupCarFollowTargetPed();
                break;
            case objectives_enum::wait_in_car_27:
                Ped::WaitInCurrentCar();
                break;
            case objectives_enum::follow_char_in_car_10:
                Ped::ProcessObjectiveFollowPedInCar();
                break;
            case objectives_enum::call_police_car_54:
                Ped::CallPoliceCar();
                break;
            case objectives_enum::pull_driver_out_32:
                Ped::PullDriverOutOfCar();
                break;
            case objectives_enum::follow_car_in_car_55:
                Ped::FollowCarInCurrCar();
                break;
            case objectives_enum::follow_car_on_foot_with_offset_56:
                Ped::FollowCarOnFootWithOffset();
                break;
            case objectives_enum::fire_at_object_from_vehicle_57:
                Ped::FireAtObject();
                break;
            case objectives_enum::destroy_car_59:
                Ped::DestroyTargetCar();
                break;
            case objectives_enum::destroy_object_58:
                Ped::DestroyTargetObject();
                break;
            case objectives_enum::turret_put_out_car_fire_60:
                Ped::AimVehicleTurretStateMachine();
                break;
            case objectives_enum::tank_fire_at_player_61:
                Ped::FireAtPlayer();
                break;
            default:
                return;
        }
    }
}

// https://decomp.me/scratch/0fIeI
MATCH_FUNC(0x463fb0)
void Ped::ProcessInternalObjective()
{
    Ang16 door_angle(0);

    if (mInternalObjectiveStatus == objective_status::failed_2)
    {
        this->mBf21C.bPanicking = false;
    }

    if (mInternalObjective && !mInternalObjectiveStatus)
    {
        Ped* pTargetPed = this->mInternalTargetPed;
        if (pTargetPed)
        {
            this->mMoveTargetX = pTargetPed->get_cam_x();
            this->mMoveTargetY = pTargetPed->get_cam_y();
            this->mMoveTargetZ = pTargetPed->get_cam_z();
            switch (mInternalObjective)
            {
                case objectives_enum::stay_close_to_char_11:
                case objectives_enum::walk_to_car_door_18:
                case objectives_enum::kill_char_on_foot_20:
                case objectives_enum::punch_char_23:
                    sKeepPaceWithTarget = 0;
                    break;
                default:
                    if (sKeepPaceWithTarget)
                    {
                        // Through a bool local with an early break: written directly, VC6 lays the
                        // clear after the call and doesn't share it with the case 11/18/20/23 block
                        bool bStop = pTargetPed->mGameObject == NULL || this->mPedState1 == ped_state_1::entering_car_3;
                        if (bStop)
                        {
                            sKeepPaceWithTarget = 0;
                            break;
                        }
                        Ped::CalcApproachPointNearTargetPed();
                    }
                    break;
            }
        }
        if (mTargetToEnter)
        {
            if (mTargetToEnter->mDespawnStatus == car_despawn_status::despawned_6)
            {
                this->mInternalObjectiveStatus = objective_status::failed_2;
                return;
            }
            u8 door_count = mTargetToEnter->GetDoorCount();
            if (this->mTargetCarDoor >= door_count)
            {
                this->mTargetCarDoor = door_count - 1;
            }

            if (mInternalObjective == objectives_enum::walk_to_car_door_18 || mInternalObjective > objectives_enum::leave_car_and_flee_34 && mInternalObjective <= objectives_enum::leave_train_38)
            {
                CarDoorAlignmentSolver(0,
                                       this->mTargetToEnter,
                                       mTargetCarDoor,
                                       this->mMoveTargetX,
                                       this->mMoveTargetY,
                                       door_angle);
                this->mMoveTargetZ = this->mTargetToEnter->mCarSprite->mZpos;
            }
            else
            {
                Car* pCar = this->mTargetToEnter;
                mMoveTargetX = pCar->mCarSprite->mXy.x;
                mMoveTargetY = pCar->mCarSprite->mXy.y;
                mMoveTargetZ = pCar->mCarSprite->mZpos;
            }
        }

        if (mInternalTargetObject)
        {
            this->mMoveTargetX = mInternalTargetObject->GetX();
            this->mMoveTargetY = mInternalTargetObject->GetY();
            this->mMoveTargetZ = mInternalTargetObject->GetZ();
        }

        if (mInternalTargetX != kFpZero_678660)
        {
            if (mInternalTargetY != kFpZero_678660)
            {
                this->mMoveTargetX = mInternalTargetX;
                this->mMoveTargetY = mInternalTargetY;
                this->mMoveTargetZ = mInternalTargetZ;
            }
        }

        Fix16 xd = this->mMoveTargetX - this->mCam.x;
        Fix16 yd = this->mMoveTargetY - this->mCam.y;

        xd = Fix16::Abs(xd);
        yd = Fix16::Abs(yd);

        gDistanceToTarget = Fix16::Max(xd, yd);

        switch (this->mInternalObjective)
        {
            case objectives_enum::flee_on_foot_till_safe_1:
                Ped::ProcessInternalObjectiveFleeOnFootTillSafe();
                break;
            case objectives_enum::flee_char_on_foot_till_safe_2:
                Ped::FleeFromPedTillSafe();
                break;
            case objectives_enum::flee_char_on_foot_always_3:
                Ped::FleeFromPedAlways();
                break;
            case objectives_enum::run_to_char_7:
                Ped::CheckFollowTargetPedStillValid();
                break;
            case objectives_enum::follow_char_9:
                Ped::FollowTargetStateMachine();
                break;
            case objectives_enum::stay_close_to_char_11:
                Ped::StayCloseToTargetPed();
                break;
            case objectives_enum::pull_driver_out_32:
                Ped::PullDriverOutOfCarStateMachine();
                break;
            case objectives_enum::punch_char_23:
                Ped::MeleeAttackStateMachine();
                break;
            case objectives_enum::kill_char_on_foot_20:
                Ped::AttackPed();
                break;
            case objectives_enum::wait_on_foot_26:
                Ped::WaitOnFoot();
                break;
            case objectives_enum::enter_car_as_driver_35:
                Ped::EnterCarStateMachine();
                break;
            case objectives_enum::leave_car_36:
                Ped::ExitCarStateMachine();
                break;
            case objectives_enum::goto_area_on_foot_12:
                Ped::ProcessInternalObjectiveGotoAreaOnFoot();
                break;
            case objectives_enum::follow_path_points_15:
                Ped::FollowPathPoints();
                break;
            case objectives_enum::walk_to_point_17:
                Ped::WalkToTargetPoint();
                break;
            case objectives_enum::goto_area_in_car_14:
                Ped::ProcessInternalObjectiveGotoAreaInCar();
                break;
            case objectives_enum::walk_to_car_door_18:
                Ped::WalkToTargetThenStop();
                break;
            case objectives_enum::crossing_road_48:
                Ped::CrossRoad();
                break;
            case objectives_enum::wait_to_cross_y_backwards_44:
                Ped::StartPedCrossingAtTrafficLight_Y_Backward();
                break;
            case objectives_enum::wait_to_cross_x_forwards_45:
                Ped::StartPedCrossingAtTrafficLight_X_Forwards();
                break;
            case objectives_enum::wait_to_cross_y_forwards_46:
                Ped::StartPedCrossingAtTrafficLight_Y_Forwards();
                break;
            case objectives_enum::wait_to_cross_x_backwards_47:
                Ped::StartPedCrossingAtTrafficLight_X_Backwards();
                break;
            case objectives_enum::wait_after_crossing_49:
                Ped::WaitForInternalObjectiveTimer();
                break;
            case objectives_enum::wait_for_train_29:
                Ped::WaitForTrain();
                break;
            case objectives_enum::enter_bus_30:
                State30_Nullsub();
                break;
            case objectives_enum::enter_train_37:
                Ped::EnterTrainStateMachine();
                break;
            case objectives_enum::leave_train_38:
                Ped::ExitTrainStateMachine();
                break;

            case objectives_enum::chase_char_in_car_52:
                Ped::ProcessInternalObjectiveChasePedInCar();
                break;
            case objectives_enum::follow_car_on_foot_with_offset_56:
                Ped::FollowCarOnFoot();
                break;
            case objectives_enum::destroy_car_59:
                Ped::AttackCar();
                break;
            case objectives_enum::destroy_object_58:
                Ped::AttackObject();
                break;

            default:
                return;
        }
    }
}

MATCH_FUNC(0x4645b0)
void Ped::CalcApproachPointNearTargetPed()
{
    Ang16 angle;
    Fix16 radius;
    Fix16 vec_x;
    Fix16 vec_y;

    bool bHurtByAttackingLeader = false;

    if (mInternalTargetPed->GetPedVelocity() > kFpZero_678660)
    {
        angle = mInternalTargetPed->mGameObject->get_rotation();
        angle += kAng180_6785A6;
        radius = kFpThreeEighths;
    }
    else
    {
        angle = gDummyPedAng;
        radius = kFpHalf;
    }

    // Comma form here and in the switch: braces would grow the caller size the inline budget is computed from
    if (mPedGroup && static_cast<u32>(mKillerId) == mPedGroup->mPedLeader->get_id() &&
        mPedGroup->mPedLeader->mBf21C.bAttacking)
        bHurtByAttackingLeader = true, mKillerIdTimer = 50;

    if (mInternalTargetPed->GetPedVelocity() == kFpZero_678660)
    {
        switch (mGroupIdx)
        {
            case 0:
                angle += kAng90;
                if (bHurtByAttackingLeader)
                    angle += kAng180_6785A6, radius = kFpThreeQuarters;
                Ang16::PolarToCartesian_41FC20(angle, radius, vec_x, vec_y);
                mMoveTargetX += vec_x;
                mMoveTargetY += vec_y;
                break;
            case 1:
                angle += kAng270_6785D0;
                if (bHurtByAttackingLeader)
                    angle += kAng180_6785A6, radius = kFpThreeQuarters;
                Ang16::PolarToCartesian_41FC20(angle, radius, vec_x, vec_y);
                mMoveTargetX += vec_x;
                mMoveTargetY += vec_y;
                break;
            case 2:
                angle = angle + kAng180_6785A6;
                if (bHurtByAttackingLeader)
                    angle += kAng180_6785A6, radius = kFpThreeQuarters;
                Ang16::PolarToCartesian_41FC20(angle, radius, vec_x, vec_y);
                mMoveTargetX += vec_x;
                mMoveTargetY += vec_y;
                break;
            case 3:
                Ang16::PolarToCartesian_41FC20(angle, radius, vec_x, vec_y);
                if (bHurtByAttackingLeader)
                    angle += kAng180_6785A6, radius = kFpThreeQuarters;
                mMoveTargetX += vec_x;
                mMoveTargetY += vec_y;
                break;
            case 4:
                angle += kAng225_6786B8;
                if (bHurtByAttackingLeader)
                    angle += kAng180_6785A6, radius = kFpThreeQuarters;
                else
                    radius = kFpThreeEighths;
                Ang16::PolarToCartesian_41FC20(angle, radius, vec_x, vec_y);
                mMoveTargetX += vec_x;
                mMoveTargetY += vec_y;
                break;
            case 5:
                angle += kAng45;
                if (bHurtByAttackingLeader)
                    angle += kAng180_6785A6, radius = kFpThreeQuarters;
                else
                    radius = kFpThreeEighths;
                Ang16::PolarToCartesian_41FC20(angle, radius, vec_x, vec_y);
                mMoveTargetX += vec_x;
                mMoveTargetY += vec_y;
                break;
            case 6:
                angle += kAng315_6785A8;
                if (bHurtByAttackingLeader)
                    angle += kAng180_6785A6, radius = kFpThreeQuarters;
                else
                    radius = kFpThreeEighths;
                Ang16::PolarToCartesian_41FC20(angle, radius, vec_x, vec_y);
                mMoveTargetX += vec_x;
                mMoveTargetY += vec_y;
                break;
            case 7:
                angle += kAng135;
                if (bHurtByAttackingLeader)
                    angle += kAng180_6785A6, radius = kFpThreeQuarters;
                else
                    radius = kFpThreeEighths;
                Ang16::PolarToCartesian_41FC20(angle, radius, vec_x, vec_y);
                mMoveTargetX += vec_x;
                mMoveTargetY += vec_y;
                break;
            default:
                angle += kAng225_6786B8;
                if (bHurtByAttackingLeader)
                    angle += kAng180_6785A6, radius = kFpThreeQuarters;
                else
                    radius = kFpThreeEighths;
                Ang16::PolarToCartesian_41FC20(angle, radius, vec_x, vec_y);
                // Raw add: one inline site fewer than Fix16::operator+=, required by the inline budget (docs/match_attempts.md)
                mMoveTargetX.mValue += vec_x.mValue;
                mMoveTargetY += vec_y;
                break;
        }

        mTargetFacingAngle = Fix16::atan2_fixed(mCam.y - mInternalTargetPed->get_cam_y(), mCam.x - mInternalTargetPed->get_cam_x());
        // Reconstructed dead code for VC6's inline budget: the original computed something here that the compiler removed (docs/match_attempts.md)
        Fix16::Abs(radius);
    }
    else
    {
        radius = kFpFiveSixteenths;
        switch (mGroupIdx)
        {
            case 0:
                angle -= kAng45_6784FC;
                break;
            case 1:
                angle += kAng45_6784FC;
                break;
            case 2:
                break;
            case 6:
                angle -= kAng45_6784FC;
                radius = kFpHalf;
                break;
            case 7:
                angle += kAng45_6784FC;
                radius = kFpHalf;
                break;
            default:
                radius = kFpFiveSixteenths;
                break;
        }
        Ang16::PolarToCartesian_41FC20(angle, radius, vec_x, vec_y);
        mMoveTargetX += vec_x;
        mMoveTargetY += vec_y;
    }
}

// https://decomp.me/scratch/LvHfw
MATCH_FUNC(0x465270)
void Ped::Threat_Reaction_AI()
{
    Ped* pTarget = NULL;
    bool bGiveUp = false;
    bool bLineOfSightChecked = false;
    Fix16 abs_x;
    Fix16 abs_y;

    switch (mThreatReaction)
    {
        case threat_reaction_enum::no_reaction_0:
        {
            s32 old_threat_search = mThreatSearch;
            if ((mThreatSearch == threat_search_enum::line_of_sight_1 ||
                 mThreatSearch == threat_search_enum::line_of_sight_player_only_6 ||
                 mThreatSearch == threat_search_enum::line_of_sight_player_threat_only_4) &&
                !mBf21C.bLosCheckedThisRound && gLosCheckSlotFree == 1)
            {
                mThreatSearch = threat_search_enum::line_of_sight_player_only_6;
                Ped* pPlayerPed = Ped::FindBestTargetPed_Mode5(3);
                if (pPlayerPed && pPlayerPed->PedTypeIs(ped_type::player_2))
                {
                    mBf21C.bSpottedPlayer = true;
                }
                mThreatSearch = old_threat_search;
            }
            break;
        }

        case threat_reaction_enum::run_away_3:
            if (mGameObject)
            {
                if (bHaveThreateningPeds && mInternalObjective != objectives_enum::flee_char_on_foot_till_safe_2 &&
                    !mBf21C.bLeftVehicle)
                {
                    Ped* pDangerousPed = gThreateningPedsList.FindClosestActivePed(mCam.x, mCam.y);
                    if (pDangerousPed && (pDangerousPed->mSelectedWeapon || pDangerousPed->mCar) && pDangerousPed != this)
                    {
                        abs_x = Fix16::Abs(pDangerousPed->get_cam_x() - mCam.x);
                        abs_y = Fix16::Abs(pDangerousPed->get_cam_y() - mCam.y);
                        if (Fix16::Max(abs_x, abs_y) < kFpFour)
                        {
                            if (pDangerousPed->mCar)
                            {
                                SetVoiceEventIfNotOnFire(1);
                            }
                            else if (pDangerousPed->mSelectedWeapon->GetWeaponType() == weapon_type::pistol ||
                                     pDangerousPed->mSelectedWeapon->GetWeaponType() == weapon_type::smg ||
                                     pDangerousPed->mSelectedWeapon->GetWeaponType() == weapon_type::shotgun ||
                                     pDangerousPed->mSelectedWeapon->GetWeaponType() == weapon_type::car_smg ||
                                     pDangerousPed->mSelectedWeapon->GetWeaponType() == weapon_type::silence_smg ||
                                     pDangerousPed->mSelectedWeapon->GetWeaponType() == weapon_type::dual_pistol)
                            {
                                SetVoiceEventIfNotOnFire(3);
                            }
                            else
                            {
                                SetVoiceEventIfNotOnFire(2);
                            }
                            Ped::SetObjective2(objectives_enum::flee_char_on_foot_till_safe_2);
                            SetInternalTargetPed(pDangerousPed);

                            mTargetFacingAngle = Fix16::atan2_fixed(mCam.y - pDangerousPed->get_cam_y(),
                                                                  mCam.x - pDangerousPed->get_cam_x());
                            mGameObject->set_rotation(mTargetFacingAngle);
                            mBf21C.bPanicking = true;
                        }
                    }
                }

                if (mObjective == objectives_enum::flee_char_on_foot_till_safe_2 &&
                    mObjectiveStatus == objective_status::passed_1)
                {
                    Ped::SetObjective(objectives_enum::no_obj_0);
                }
                if (mInternalObjective == objectives_enum::flee_char_on_foot_till_safe_2 &&
                    mInternalObjectiveStatus == objective_status::passed_1)
                {
                    Ped::SetObjective2(objectives_enum::no_obj_0);
                    mBf21C.bPanicking = false;
                }
            }
            break;

        case threat_reaction_enum::react_as_emergency_1:
        case threat_reaction_enum::react_as_normal_2:
            if (mOccupation == ped_ocupation_enum::paramedic_23)
            {
                break;
            }

            if (mCar)
            {
                if (mThreatReaction == threat_reaction_enum::react_as_normal_2)
                {
                    if (mObjective && !mBf21C.bStayInCar && mObjectiveTimer > 50 &&
                        Ped::FindBestTargetPed_Mode1(3) && mCar->GetVelocity() == kFpZero_678660)
                    {
                        mBf21C.bPanicking = true;
                        Ped::SetObjective2(objectives_enum::leave_car_36);
                        mObjectiveTimer = 0;
                        set_target_to_enter(mCar);
                    }
                }
                else
                {
                    mThreatSearch = threat_search_enum::line_of_sight_1;
                    Ped::FindBestTargetPed_Mode1(3);
                }
            }
            else if (mInternalObjective == objectives_enum::kill_char_on_foot_20 || mInternalObjective == objectives_enum::punch_char_23 ||
                     mInternalObjective == objectives_enum::pull_driver_out_32)
            {
                if (mThreatSearch == threat_search_enum::line_of_sight_1 ||
                    mThreatSearch == threat_search_enum::line_of_sight_player_only_6 ||
                    mThreatSearch == threat_search_enum::line_of_sight_player_threat_only_4)
                {
                    if (!mBf21C.bLosCheckedThisRound && gLosCheckSlotFree == 1)
                    {
                        pTarget = Ped::FindBestTargetPed_Mode5(3);
                        bLineOfSightChecked = true;
                    }
                }
                else
                {
                    pTarget = Ped::FindBestTargetPed_Mode1(3);
                }

                if (mThreatReaction == threat_reaction_enum::react_as_normal_2)
                {
                    if (pTarget && pTarget != mInternalTargetPed)
                    {
                        mInternalTargetPed = pTarget;
                    }
                }
                else
                {
                    if (pTarget && pTarget != mInternalTargetPed)
                    {
                        Fix16 diff_x = pTarget->get_cam_x() - mCam.x;
                        Fix16 diff_y = pTarget->get_cam_y() - mCam.y;
                        abs_x = Fix16::Abs(diff_x);
                        abs_y = Fix16::Abs(diff_y);
                        if (Fix16::Max(abs_x, abs_y) < kFpTwo_678668)
                        {
                            mInternalTargetPed = pTarget;
                        }
                    }
                    if (mInternalObjective == objectives_enum::pull_driver_out_32 && (pTarget == mInternalTargetPed || !pTarget))
                    {
                        break;
                    }
                }

                Fix16 diff_x = mMoveTargetX - mCam.x;
                Fix16 diff_y = mMoveTargetY - mCam.y;
                abs_x = Fix16::Abs(diff_x);
                abs_y = Fix16::Abs(diff_y);
                Fix16 dist = Fix16::Max(abs_x, abs_y);

                if (mPedGroup && !IsGroupLeader())
                {
                    if (mPedGroup->IsMemberTooFarFromLeader(mGroupIdx))
                    {
                        Ped::SetObjective2(objectives_enum::run_to_char_7);
                        SetInternalTargetPed(mPedGroup->mPedLeader);
                        SetPanicking();
                    }
                    if (mPedGroup->IsLeaderEnteringCarOrTrain() && mPedGroup->IsLeaderCloseToTargetCar())
                    {
                        Ped::SetObjective2(objectives_enum::run_to_char_7);
                        SetInternalTargetPed(mPedGroup->mPedLeader);
                        SetPanicking();
                    }
                }

                if (dist <= kFpSix)
                {
                    if (mThreatSearch != threat_search_enum::line_of_sight_1 &&
                        mThreatSearch != threat_search_enum::line_of_sight_player_only_6 &&
                        mThreatSearch != threat_search_enum::line_of_sight_player_threat_only_4)
                    {
                        mTargetUnseenCounter = 0;
                    }
                    else if (!mBf21C.bLosCheckedThisRound && gLosCheckSlotFree == 1)
                    {
                        if (!gMap_0x370->IsProbePathClear(gSpawnJitterScale,
                                                           kFpQuarter_678484,
                                                           gSpawnJitterScale,
                                                           mCam.x,
                                                           mCam.y,
                                                           mCam.z,
                                                           mInternalTargetPed->get_cam_x(),
                                                           mInternalTargetPed->get_cam_y(),
                                                           mInternalTargetPed->get_cam_z()))
                        {
                            ++mTargetUnseenCounter;
                        }
                        else
                        {
                            mTargetUnseenCounter = 0;
                        }
                        bLineOfSightChecked = true;
                    }
                    else
                    {
                        ++mTargetUnseenCounter;
                    }
                }
                else
                {
                    ++mTargetUnseenCounter;
                }

                if ((mThreatSearch == threat_search_enum::line_of_sight_1 ||
                     mThreatSearch == threat_search_enum::line_of_sight_player_only_6 ||
                     mThreatSearch == threat_search_enum::line_of_sight_player_threat_only_4) &&
                    bLineOfSightChecked == true)
                {
                    mBf21C.bLosCheckedThisRound = true;
                    gLosCheckSlotFree = 0;
                }

                if (mTargetUnseenCounter > 15)
                {
                    bGiveUp = true;
                }
                if (mInternalTargetPed->IsInvisibleOnFoot_433DA0())
                {
                    bGiveUp = true;
                }
                if (mInternalObjectiveStatus == objective_status::passed_1)
                {
                    bGiveUp = true;
                }
                if (mInternalObjectiveStatus == objective_status::failed_2)
                {
                    bGiveUp = true;
                }
                if (mThreatReaction == threat_reaction_enum::react_as_emergency_1)
                {
                    if (mInternalTargetPed->get_wanted_star_count() == 0 && mInternalTargetPed->is_player())
                    {
                        bGiveUp = true;
                    }
                    if (mObjective == objectives_enum::enter_car_as_driver_35)
                    {
                        bGiveUp = true;
                    }
                }

                if (bGiveUp == true)
                {
                    if (mInternalTargetPed != mObjectiveTargetPed)
                    {
                        if (mGameObject->GetCharState() != Char_B4_state::Jumping_15)
                        {
                            mPatrolPoints[0].mX = 0;
                            mPatrolPoints[0].mY = 0;
                            mBf21C.bPanicking = false;
                            mBf21C.bAttacking = false;
                        }
                        Ped::SetObjective2(objectives_enum::no_obj_0);
                    }
                    else
                    {
                        mInternalObjectiveStatus = objective_status::failed_2;
                    }
                }
            }
            else
            {
                mTargetUnseenCounter = 0;
                if (mBf21C.bLeftVehicle)
                {
                    mBf21C.bPanicking = false;
                }
                else if (!mPedGroup ||
                         (!mPedGroup->IsLeaderEnteringCarOrTrain() && !mPedGroup->IsLeaderInCar()))
                {
                    if (mThreatSearch == threat_search_enum::line_of_sight_1 ||
                        mThreatSearch == threat_search_enum::line_of_sight_player_only_6 ||
                        mThreatSearch == threat_search_enum::line_of_sight_player_threat_only_4)
                    {
                        if (!mBf21C.bLosCheckedThisRound && gLosCheckSlotFree == 1)
                        {
                            pTarget = Ped::FindBestTargetPed_Mode5(3);
                            bLineOfSightChecked = true;
                        }
                    }
                    else
                    {
                        pTarget = Ped::FindBestTargetPed_Mode1(3);
                    }

                    if (pTarget && pTarget->mBf21C.bActive)
                    {
                        if (mThreatReaction == threat_reaction_enum::react_as_emergency_1)
                        {
                            SetVoiceEventIfNotOnFire(17);
                        }
                        if (!pTarget->get_car())
                        {
                            if (mPedGroup)
                            {
                                mPedGroup->GroupAttackPed(pTarget);
                            }
                            else if (mObjective != objectives_enum::goto_area_any_means_13)
                            {
                                mBf21C.bPanicking = true;
                                Ped::SetObjective2(objectives_enum::kill_char_on_foot_20);
                                mInternalTargetPed = pTarget;
                                pTarget->mAttacker = this;
                                mTargetUnseenCounter = 0;
                            }
                        }
                        else if (!mPedGroup)
                        {
                            mBf21C.bPanicking = true;
                            Ped::SetObjective2(objectives_enum::kill_char_on_foot_20);
                            mInternalTargetPed = pTarget;
                            mTargetUnseenCounter = 0;
                        }
                        else
                        {
                            mPedGroup->GroupAttackPed(pTarget);
                        }
                    }
                    else
                    {
                        mBf21C.bPanicking = false;
                    }

                    if ((mThreatSearch == threat_search_enum::line_of_sight_1 ||
                         mThreatSearch == threat_search_enum::line_of_sight_player_only_6 ||
                         mThreatSearch == threat_search_enum::line_of_sight_player_threat_only_4) &&
                        bLineOfSightChecked == true)
                    {
                        mBf21C.bLosCheckedThisRound = true;
                        gLosCheckSlotFree = 0;
                    }
                }
            }
            break;

        default:
            break;
    }
}

MATCH_FUNC(0x465b20)
void Ped::ReactToAttacker()
{
    if (mAttacker->isDead() || !mAttacker->IsActive())
    {
        this->mAttacker = NULL;
        this->mBf21C.bPanicking = false;
    }
    else if (!mAttacker->IsInvisibleOnFoot_433DA0())
    {
        Gang* pGang = this->mpGang;
        if (pGang && pGang == mAttacker->mpGang)
        {
            this->mAttacker = NULL;
            this->mBf21C.bPanicking = false;
        }
        else
        {
            PedGroup* pGroup = this->mPedGroup;
            if (pGroup && pGroup == mAttacker->mPedGroup)
            {
                this->mAttacker = NULL;
                this->mBf21C.bPanicking = false;
            }
            else
            {
                if (mThreatReaction > threat_reaction_enum::no_reaction_0)
                {
                    if (mThreatReaction > threat_reaction_enum::react_as_normal_2)
                    {
                        if (mThreatReaction == threat_reaction_enum::run_away_3)
                        {
                            if (this->mGameObject)
                            {
                                SetObjective2(objectives_enum::flee_char_on_foot_till_safe_2);
                                this->mInternalTargetPed = this->mAttacker;
                            }
                        }
                    }
                    else if (this->mOccupation != ped_ocupation_enum::paramedic_23)
                    {
                        if (this->mGameObject)
                        {
                            if (!pGroup)
                            {
                                this->mObjectiveTimer = 0;
                                if (mSelectedWeapon)
                                {
                                    Ped::SetObjective2(objectives_enum::kill_char_on_foot_20);
                                }
                                else
                                {
                                    Ped::SetObjective2(objectives_enum::punch_char_23);
                                }
                                this->mInternalTargetPed = this->mAttacker;
                                this->mBf21C.bPanicking = true;
                            }
                            else
                            {
                                if (pGroup->mPedLeader->is_player())
                                {
                                    if (!mAttacker->mOffscreenCounter && this->mTargetUnseenCounter < 5u &&
                                        Fix16::Abs(mAttacker->get_cam_z() - mCam.z) < kFpOne_678664)
                                    {
                                        pGroup->GroupAttackPed(mAttacker);
                                    }
                                    else
                                    {
                                        this->mAttacker = NULL;
                                    }
                                }
                                else
                                {
                                    pGroup->GroupAttackPed(mAttacker);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

MATCH_FUNC(0x465cd0)
bool Ped::IsAttackingTargetPed() const
{
    if (mBf21C.bPanicking == true)
    {
        if (mInternalTargetPed)
        {
            if (mInternalObjective == objectives_enum::kill_char_on_foot_20 
                || mInternalObjective == objectives_enum::punch_char_23)
            {
                return true;
            }
        }
    }
    return false;
}

// https://decomp.me/scratch/Fh1iq
MATCH_FUNC(0x466b70)
bool Ped::IsInvisibleOnFoot_466B70() const
{
    if (mBf21C.bInvisible && mGameObject != NULL)
    {
        return true;
    }

    return false;
}

// Returns Fix16 by value (hidden out pointer, ret $0x14). Max of the two Abs written out with
// the y abs first: Fix16::Max takes references and spills both to the stack.
MATCH_FUNC(0x4614e0)
Fix16 __stdcall MaxAbsDistance_4614E0(Fix16& x1, Fix16& y1, Fix16& x2, Fix16& y2)
{
    Fix16 diff_x = x2 - x1;
    Fix16 diff_y = y2 - y1;
    Fix16 ay = Fix16::Abs(diff_y);
    Fix16 ax = Fix16::Abs(diff_x);
    if (ax > ay)
    {
        return ax;
    }
    return ay;
}

// https://decomp.me/scratch/Fh1iq
MATCH_FUNC(0x465d00)
bool Ped::IsPedAThreat(Ped* pTargetPed)
{
    char_type bThisIsActivePlayer = 0;

    if (pTargetPed->IsInvisibleOnFoot_433DA0())
    {
        return false;
    }

    if (this->mThreatSearch == threat_search_enum::area_player_threat_only_3 || this->mThreatSearch == threat_search_enum::line_of_sight_player_threat_only_4)
    {
        if (!pTargetPed->PedTypeIs(ped_type::player_2))
        {
            return false;
        }
    }

    if (this->mThreatSearch == threat_search_enum::area_player_only_5 || this->mThreatSearch == threat_search_enum::line_of_sight_player_only_6)
    {
        if (pTargetPed->IsActivePlayerPed())
        {
            goto ret_true;
        }
    }

    if (this->IsActivePlayerPed())
    {
        bThisIsActivePlayer = 1;
    }

    if (this->mPedGroup == pTargetPed->mPedGroup && this->mPedGroup != NULL)
    {
        return false;
    }

    if (this->mOccupation == ped_ocupation_enum::mad_mugger_40)
    {
        Car* pCar = pTargetPed->mCar;

        if (pCar == NULL)
        {
            goto ret_true;
        }

        if (!pCar->IsPoliceCar())
        {
            goto ret_true;
        }

        if (pTargetPed->mPedType == ped_type::special_ped_4)
        {
            goto ret_false;
        }

        return true;
    }

    {
        Gang* pMyGang = this->mpGang;

        if (pMyGang != NULL)
        {
            Gang* pOtherGang = pTargetPed->mpGang;

            if (pOtherGang != NULL)
            {
                if (pOtherGang == pMyGang)
                {
                    goto ret_if_active_player;
                }

                if (!pMyGang->IsHostileToGang(pOtherGang->mGangIdx))
                {
                    return false;
                }

                if (this->mPedType != ped_type::special_ped_4 && this->mPedType != ped_type::dummy_with_occupation_6)
                {
                    goto ret_true;
                }

                if (pTargetPed->mPrevAttackersCount >= 4)
                {
                    return false;
                }

                if (pTargetPed->mAttackersCount >= 4)
                {
                    goto ret_false;
                }

                return true;
            }

            if (pMyGang->mHasHighRespect)
            {
                switch (pTargetPed->mOccupation)
                {
                    case ped_ocupation_enum::police:
                    case ped_ocupation_enum::swat:
                    case ped_ocupation_enum::fbi:
                    case ped_ocupation_enum::army_army:
                    case ped_ocupation_enum::walking_guard_29:
                    case ped_ocupation_enum::walking_swat_guard_30:
                    case ped_ocupation_enum::walking_army_guard_31:
                    case ped_ocupation_enum::roadblock_cop_37:
                        return true;
                    default:
                        break;
                }
            }

            if (pTargetPed->GetAttacking())
            {
                if (!pTargetPed->IsActivePlayerPed() && pTargetPed->mOccupation != ped_ocupation_enum::empty)
                {
                    if (pTargetPed->mThreatReaction == threat_reaction_enum::react_as_emergency_1)
                    {
                        goto ret_if_active_player;
                    }

                    if (this->mSelectedWeapon != NULL)
                    {
                        goto ret_true;
                    }

                    if (Fix16::MaxAbsDistance(this->mCam.x, this->mCam.y, pTargetPed->get_cam_x(), pTargetPed->get_cam_y()) <= kFpOne_678798)
                    {
                        goto ret_true;
                    }

                    goto ret_if_active_player;
                }

                {
                    u8 player_idx = pTargetPed->mPlayer->get_idx();

                    if (this->mpGang->IsRespectNegativeForPlayer(player_idx))
                    {
                        if (!gPolice_7B8->mCrewPedOnscreen)
                        {
                            goto ret_true;
                        }

                        return false;
                    }

                    return false;
                }
            }

            if (pTargetPed->IsActivePlayerPed() || pTargetPed->mOccupation == ped_ocupation_enum::empty)
            {
                u8 player_idx = pTargetPed->mPlayer->get_idx();

                if (this->mpGang->IsRespectNegativeForPlayer(player_idx))
                {
                    if (!gPolice_7B8->mCrewPedOnscreen)
                    {
                        goto ret_true;
                    }
                    return false;
                }

                return false;
            }

            goto ret_if_active_player;
        }
    }

    switch (this->mOccupation)
    {
        case ped_ocupation_enum::criminal_type_1:
            if (pTargetPed->mOccupation == ped_ocupation_enum::criminal_type_1)
            {
                return false;
            }

            goto ret_if_active_player;
        case ped_ocupation_enum::criminal_type_2:
            if (pTargetPed->mOccupation == ped_ocupation_enum::criminal_type_2)
            {
                return false;
            }

            goto ret_if_active_player;
        case ped_ocupation_enum::elvis:
            if (pTargetPed->PedTypeIs(ped_type::player_2))
            {
                return false;
            }

            goto ret_if_active_player;
        case ped_ocupation_enum::police:
        case ped_ocupation_enum::swat:
        case ped_ocupation_enum::fbi:
        case ped_ocupation_enum::army_army:
        case ped_ocupation_enum::walking_guard_29:
        case ped_ocupation_enum::walking_swat_guard_30:
        case ped_ocupation_enum::walking_army_guard_31:
        case ped_ocupation_enum::roadblock_cop_37:
            break;
        default:
            if (this->mPedGroup != NULL)
            {
                if (this->mPedGroup->mPedLeader != pTargetPed->mInternalTargetPed)
                {
                    goto ret_if_active_player;
                }

                if (pTargetPed->mInternalObjective == objectives_enum::kill_char_on_foot_20)
                {
                    goto ret_true;
                }

                if (pTargetPed->mInternalObjective != objectives_enum::punch_char_23)
                {
                    goto ret_if_active_player;
                }

                return true;
            }

            if (pTargetPed->GetAttacking())
            {
                goto ret_true;
            }

            goto ret_if_active_player;
    }

    if (pTargetPed->IsActivePlayerPed())
    {
        if (pTargetPed->mGameObject != NULL && pTargetPed->mGameObject->GetCharState() == Char_B4_state::Jumping_15)
        {
            return false;
        }

        if (!((pTargetPed->GetAttacking()) && pTargetPed->mSelectedWeapon != NULL &&
              pTargetPed->mSelectedWeapon->CountsAsArmedAttack()))
        {
            if (pTargetPed->get_wanted_points() < cop_level_ped_enum::cop_1_stars && this->mAttacker != pTargetPed && pTargetPed->mRecentCrimeTimer <= 0u)
            {
                return false;
            }
        }

        this->mAttacker = NULL;
        gPolice_7B8->UpdateLastSeenCoordsForCriminal(pTargetPed);

        if (this->mObjective == objectives_enum::patrol_in_car_43)
        {
            if (gPolice_7B8->TryAssignCarCrewToCriminal(this->mCar, pTargetPed))
            {
                goto ret_true;
            }

            if (pTargetPed->get_wanted_points() < cop_level_ped_enum::cop_1_stars)
            {
                pTargetPed->mWantedPoints = cop_level_ped_enum::cop_1_stars;
            }

            return false;
        }

        if (pTargetPed->get_wanted_points() >= cop_level_ped_enum::cop_1_stars)
        {
            goto ret_true;
        }

        pTargetPed->mWantedPoints = cop_level_ped_enum::cop_1_stars;
        return true;
    }

{
    s32 target_occupation = pTargetPed->mOccupation;
    switch (target_occupation)
    {
        case ped_ocupation_enum::paramedic_23:
        case ped_ocupation_enum::police:
        case ped_ocupation_enum::swat:
        case ped_ocupation_enum::fbi:
        case ped_ocupation_enum::army_army:
        case ped_ocupation_enum::walking_guard_29:
        case ped_ocupation_enum::walking_swat_guard_30:
        case ped_ocupation_enum::walking_army_guard_31:
        case ped_ocupation_enum::roadblock_cop_37:
        case ped_ocupation_enum::road_block_tank_man:
            goto ret_if_active_player;
        default:
            break;
    }

    if (target_occupation == ped_ocupation_enum::empty)
    {
        goto ret_true;
    }

    if (pTargetPed->GetAttacking())
    {
        goto ret_true;
    }

    if (pTargetPed->mInternalObjective != objectives_enum::kill_char_on_foot_20)
    {
        goto ret_if_active_player;
    }

    return true;
}

// shared exits: a return in place of these gotos gets its own epilogue or folds into a setcc
ret_if_active_player:
    if (bThisIsActivePlayer == 1)
    {
        goto ret_true;
    }

ret_false:
    return false;

ret_true:
    return true;
}

// The normalizing Ang16 ctor (out of line, AssignNormalized) on a by-value parameter: the
// original stores the 16-bit difference as a dword before the call (as Particle_4C's NormalizedAng)
static inline Ang16 NormalizedAng(s16 value)
{
    return Ang16(&value, 0);
}

// Ang16::operator- with the normalizing ctor called out of line and its argument stored as a dword
static inline Ang16 SubAng16(const Ang16& lhs, const Ang16& rhs)
{
    return NormalizedAng(lhs.rValue - rhs.rValue);
}

// https://decomp.me/scratch/0VTTk
// 9.6f 0x437670. Mode 1 is IsPedAThreat (9.6f 0x4371D0) inlined, with the nested budget (42) sending
// IsInvisibleOnFoot and MaxAbsDistance out of line. The angle and distance locals are at function scope
// (9.6f default-constructs three Ang16 at the top): the original's 4-byte slots for the mode 4 and 5
// differences never share with the mode 1 and 3 temporaries, and the 2-byte Ang16 temps stay 2 bytes.
MATCH_FUNC(0x4661F0)
bool Ped::IsThreatToSearchingPed()
{
    Ang16 a;
    Ang16 rel;
    Ang16 range;
    Fix16 dx;
    Fix16 dy;
    Fix16 dx2;
    Fix16 dy2;
    Fix16 dz;

    switch (gTargetSearchMode)
    {
        case threat_search_enum::line_of_sight_1:
            if (mPedType == ped_type::dummy_3)
            {
                return false;
            }

            if (mPedState1 == ped_state_1::dead_9 || mPedState1 == ped_state_1::immobilized_8)
            {
                return false;
            }

            if (!mBf21C.bActive)
            {
                return false;
            }

            // 9.6f 0x437670: a call to IsPedAThreat (0x4371D0), inlined here in 10.5
            if (gSearchingPed->IsPedAThreat_Inline_465D00(this))
            {
                return true;
            }

            return false;

        case threat_search_enum::area_2:
            if (mCar != NULL)
            {
                return false;
            }

            if (mPedState1 == ped_state_1::dead_9 || mPedState1 == ped_state_1::immobilized_8)
            {
                return false;
            }

            if (!mBf21C.bActive)
            {
                return false;
            }

            if (gSearchingPed != this)
            {
                return true;
            }

            break;

        case threat_search_enum::area_player_threat_only_3:
            if (mCar != NULL)
            {
                return false;
            }

            if (mPedState1 == ped_state_1::dead_9 || mPedState1 == ped_state_1::immobilized_8)
            {
                return false;
            }

            if (!mBf21C.bActive)
            {
                return false;
            }

            {
                Ped* pSearcher = gSearchingPed;

                if (pSearcher == this)
                {
                    return false;
                }

                if (Fix16::MaxAbsDistance(this->mCam.x, this->mCam.y, pSearcher->get_cam_x(), pSearcher->get_cam_y()) <= kFpQuarter)
                {
                    return true;
                }

                break;
            }

        case threat_search_enum::line_of_sight_player_threat_only_4:
            if (mCar != NULL)
            {
                return false;
            }

            if (mPedState1 == ped_state_1::dead_9 || mPedState1 == ped_state_1::immobilized_8)
            {
                return false;
            }

            if (!mBf21C.bActive)
            {
                return false;
            }

            {
                Ped* pSearcher = gSearchingPed;

                if (pSearcher == this)
                {
                    return false;
                }

                dx = this->mCam.x - pSearcher->get_cam_x();
                dy = this->mCam.y - pSearcher->get_cam_y();
                a = Fix16::atan2_fixed(dy, dx);
                rel = SubAng16(a, gSearchingPed->GetRotation());
                range = SubAng16(gDummyPedAng, gSearchConeHalfAngle);

                if (!(rel < gSearchConeHalfAngle))
                {
                    if (rel <= range)
                    {
                        return false;
                    }
                }
                return true;
            }

        case threat_search_enum::area_player_only_5:
            if (mPedState1 == ped_state_1::dead_9 || mPedState1 == ped_state_1::immobilized_8)
            {
                return false;
            }

            if (!mBf21C.bActive)
            {
                return false;
            }

            if (mPedType != ped_type::player_2)
            {
                if (!gSearchingPed->IsPedAThreat(this))
                {
                    return false;
                }
            }
            else
            {
                Player* pPlayer = this->mPlayer;
                Camera* pCam = &(pPlayer->mCameraMode == 2 || pPlayer->mCameraMode == 3 ? pPlayer->mAuxGameCamera : pPlayer->mGameCamera);

                Char* pSearcherChar = gSearchingPed->mGameObject;

                if (pSearcherChar != NULL)
                {
                    if (!pCam->IsSpriteInView(pSearcherChar->mSpritePtr, 1))
                    {
                        return false;
                    }
                }
                else
                {
                    if (!pCam->IsSpriteInView(gSearchingPed->mCar->mCarSprite, 1))
                    {
                        return false;
                    }
                }
            }

            gSearchConeHalfAngle.rValue = *reinterpret_cast<s16*>(&kAng90_6784B0);

            {
                Ped* pSearcher = gSearchingPed;

                if (pSearcher == this)
                {
                    return false;
                }

                dx2 = this->mCam.x - pSearcher->get_cam_x();
                dy2 = this->mCam.y - pSearcher->get_cam_y();
                a = Fix16::atan2_fixed(dy2, dx2);
                rel = SubAng16(a, gSearchingPed->GetRotation());
                range = SubAng16(gDummyPedAng, gSearchConeHalfAngle);

                if (!(rel < gSearchConeHalfAngle))
                {
                    if (rel <= range)
                    {
                        return false;
                    }
                }
            }

            {
                if (gSearchingPed->mPedGroup != NULL && gSearchingPed->mPedGroup->mPedLeader->mPlayer != NULL)
                {
                    dz = gSearchingPed->mCam.z - this->mCam.z;

                    if (Fix16::Abs_negate_out_of_line(dz) >= kFpOne_678664)
                    {
                        return false;
                    }
                }

                if (mPedType != ped_type::player_2)
                {
                    return gMap_0x370->IsProbePathClear(gSpawnJitterScale * 2,
                                                         kFpQuarter_678484,
                                                         gSpawnJitterScale,
                                                         gSearchingPed->get_cam_x(),
                                                         gSearchingPed->get_cam_y(),
                                                         gSearchingPed->get_cam_z(),
                                                         this->mCam.x,
                                                         this->mCam.y,
                                                         this->mCam.z);
                }

                if (gMap_0x370->IsProbePathClear(kFpQuarter_678484,
                                                  kFpQuarter_678484,
                                                  gSpawnJitterScale,
                                                  gSearchingPed->get_cam_x(),
                                                  gSearchingPed->get_cam_y(),
                                                  gSearchingPed->get_cam_z(),
                                                  this->mCam.x,
                                                  this->mCam.y,
                                                  this->mCam.z))
                {
                    gSearchingPed->mBf21C.bSpottedPlayer = true;
                    return gSearchingPed->IsPedAThreat(this);
                }

                gSearchingPed->mBf21C.bSpottedPlayer = false;
                return false;
            }
    }

    return false;
}

MATCH_FUNC(0x466b90)
Ped* Ped::FindBestTargetPed_Mode1(s32 max_x_check)
{
    gTargetSearchMode = threat_search_enum::line_of_sight_1;
    return Ped::FindBestTargetPed(max_x_check);
}

MATCH_FUNC(0x466bb0)
Ped* Ped::FindBestTargetPed_Mode4(s32 max_x_check)
{
    gTargetSearchMode = threat_search_enum::line_of_sight_player_threat_only_4;
    return Ped::FindBestTargetPed(max_x_check);
}

MATCH_FUNC(0x466bd0)
Ped* Ped::FindBestTargetPed_Mode5(s32 max_x_check)
{
    gTargetSearchMode = threat_search_enum::area_player_only_5;
    return Ped::FindBestTargetPed(max_x_check);
}

// https://decomp.me/scratch/jl40w
// 9.6f 0x437BE0
MATCH_FUNC(0x466bf0)
Ped* Ped::FindBestTargetPed(s32 max_x_check)
{
    gSearchingPed = this;

    // Two calls in the source: VC6 merges their identical heads and tails
    Sprite* pNear;
    if (mGameObject)
    {
        pNear = gSpriteGrid_1->FindNearestSprite_SpiralSearch(sprite_types_enum::ped_3,
                                                              sprite_types_enum::car_2,
                                                              mGameObject->mSpritePtr,
                                                              max_x_check,
                                                              0,
                                                              0);
    }
    else
    {
        pNear = gSpriteGrid_1->FindNearestSprite_SpiralSearch(sprite_types_enum::ped_3,
                                                              sprite_types_enum::car_2,
                                                              mCar->mCarSprite,
                                                              max_x_check,
                                                              0,
                                                              0);
    }

    if (!pNear)
    {
        Ped* pClosest;
        if (mPedGroup)
        {
            if (mPedGroup->mPedLeader->is_player())
            {
                pClosest = NULL;
            }
            else
            {
                pClosest = gThreateningPedsList.FindClosestActivePed(mCam.x, mCam.y);
            }
        }
        else
        {
            pClosest = gThreateningPedsList.FindClosestActivePed(mCam.x, mCam.y);
        }

        if (IsNetworkGame() && mPedGroup)
        {
            Fix16 best_dist = kFpFour_678670;
            Ped* pBestPed = NULL;
            for (Player* pPlayer = gGame_0x40->IterateFirstPlayer(); pPlayer;
                 pPlayer = gGame_0x40->IterateNextPlayer())
            {
                Ped* pPlayerPed = pPlayer->mPlayerPed;
                if (pPlayerPed && mPedGroup != pPlayerPed->mPedGroup)
                {
                    Fix16 dist;
                    dist = Fix16::MaxAbsDistance(get_cam_x(), get_cam_y(), pPlayerPed->get_cam_x(), pPlayerPed->get_cam_y());
                    if (dist < best_dist)
                    {
                        pBestPed = pPlayerPed;
                        best_dist = dist;
                    }
                }
            }

            if (pBestPed)
            {
                return pBestPed;
            }
        }

        if (pClosest)
        {
            Fix16 dz;
            dz = Fix16::Abs(mCam.z - pClosest->mCam.z);
            if (dz < kFpOne_678664 && pClosest != gSearchingPed &&
                Fix16::MaxAbsDistance(pClosest->get_cam_x(),
                                      pClosest->get_cam_y(),
                                      gSearchingPed->get_cam_x(),
                                      gSearchingPed->get_cam_y()) < kFpFour_678670)
            {
                if (pClosest->PedTypeIs(ped_type::player_2))
                {
                    Camera* pCam = pClosest->mPlayer->get_camera();
                    if (gSearchingPed->mGameObject)
                    {
                        if (!pCam->IsSpriteInView(gSearchingPed->mGameObject->mSpritePtr, 1))
                        {
                            return NULL;
                        }
                    }
                    else if (!pCam->IsSpriteInView(gSearchingPed->mCar->mCarSprite, 1))
                    {
                        return NULL;
                    }
                }

                if (IsPedAThreat(pClosest))
                {
                    return pClosest;
                }
            }
        }
    }

    if (pNear)
    {
        switch (pNear->GetType())
        {
            case sprite_types_enum::car_2:
                return pNear->AsCar()->GetDriver();
            case sprite_types_enum::ped_3:
                return pNear->AsCharB4()->mpPed;
        }
    }
    return NULL;
}

MATCH_FUNC(0x466f40)
Ped* Ped::FindNearestPed_Mode4(u8 max_x_check)
{
    gTargetSearchMode = threat_search_enum::line_of_sight_player_threat_only_4;
    return Ped::FindNearestPed(max_x_check);
}

MATCH_FUNC(0x466f60)
Ped* Ped::FindNearestPed(u8 max_x_check)
{
    gSearchingPed = this;
    Sprite* pSprite = gSpriteGrid_1->FindNearestSprite_SpiralSearch(sprite_types_enum::ped_3,
                                                                    sprite_types_enum::car_2,
                                                                    mGameObject->mSpritePtr,
                                                                    max_x_check,
                                                                    0,
                                                                    0);
    if (pSprite)
    {
        // @OG_BUG: Null de-ref
        return pSprite->AsCharB4()->mpPed;
    }
    return NULL;
}

MATCH_FUNC(0x466fb0)
Ped* Ped::FindNearbyPed()
{
    gTargetSearchMode = threat_search_enum::area_player_threat_only_3;
    gSearchingPed = this;
    Sprite* pNearest = gSpriteGrid_1->FindNearestSprite_SpiralSearch(sprite_types_enum::ped_3,
                                                                     sprite_types_enum::car_2,
                                                                     mGameObject->mSpritePtr,
                                                                     3u,
                                                                     1,
                                                                     0);
    if (pNearest)
    {
        if (Fix16::MaxAbsDistance(mCam.x, mCam.y, pNearest->mXy.x, pNearest->mXy.y) <
            kFpQuarter)
        {
            // @OG_BUG: Null de-ref
            return pNearest->AsCharB4()->mpPed;
        }
    }

    return NULL;
}

MATCH_FUNC(0x467070)
Ped* Ped::GetLastProcessedPedOnFoot()
{
    return gLastProcessedPed->mGameObject != NULL ? gLastProcessedPed : NULL;
}

MATCH_FUNC(0x467090)
bool Ped::FindUsableCarDoor()
{
    Car* pTargetToEnter = this->mTargetToEnter;
    if (pTargetToEnter ||
        (!this->mTargetObjectiveCar || this->mPedState2 == ped_state_2::ped2_getting_out_a_car_7 ||
         this->mObjective == objectives_enum::leave_car_36) &&
            this->mGameObject && (pTargetToEnter = this->mGameObject->mTargetCar) != NULL)
    {
        bool bIsLawEnforcement = IsLawEnforcement();
        Fix16 vel_to_check = kFpPoint2;
        if (bIsLawEnforcement)
        {
            vel_to_check = kFpPoint1;
        }
        if ((pTargetToEnter->GetVelocity() <= vel_to_check // car going slow enough?
             || this->mInternalObjective == objectives_enum::leave_car_36 || this->mPedState2 == ped_state_2::ped2_at_car_door_17) &&
            !pTargetToEnter->IsDespawning() && !pTargetToEnter->IsMaxDamage() &&
            (this->mPedState1 == ped_state_1::exiting_car_4 || pTargetToEnter->IsDoorLockedForPed(this) != true) // can enter this car?
            && !pTargetToEnter->IsDeactivated())
        {
            u8 door = this->mTargetCarDoor;
            if (!this->mEnterCarAsPassenger)
            {
                for (; door < static_cast<u8>(pTargetToEnter->GetDoorCount()); door++)
                {
                    if (pTargetToEnter->IsDoorAccessible(door))
                    {
                        this->mTargetCarDoor = door;
                        return true;
                    }
                    Car_Door* pDoor = pTargetToEnter->GetDoor(this->mTargetCarDoor);
                    pDoor->Close();
                    pDoor->set_ped(NULL);
                    if (this->mPedState2 == ped_state_2::ped2_entering_a_car_6 ||
                        this->mPedState2 == ped_state_2::ped2_getting_out_a_car_7)
                    {
                        return false;
                    }
                }
                return false;
            }
            else
            {
                for (; door < static_cast<u8>(pTargetToEnter->GetDoorCount()); door++)
                {
                    if (pTargetToEnter->IsDoorAccessible(door))
                    {
                        this->mTargetCarDoor = door;
                        return true;
                    }
                    Car_Door* pDoor = pTargetToEnter->GetDoor(this->mTargetCarDoor);
                    pDoor->Close();
                    pDoor->set_ped(NULL);
                    if (this->mPedState2 == ped_state_2::ped2_entering_a_car_6)
                    {
                        return false;
                    }
                }

                for (door = this->mTargetCarDoor; door != 0xFF; door--)
                {
                    if (pTargetToEnter->IsDoorAccessible(door))
                    {
                        this->mTargetCarDoor = door;
                        return true;
                    }
                }
                return false;
            }
        }
    }
    return false;
}

MATCH_FUNC(0x467280)
Sprite* Ped::ResetAnimAndFindNearestSprite()
{
    this->mGameObject->mPedState1 = ped_state_1::walking_0;
    this->mGameObject->mPedState2 = ped_state_2::ped2_walking_0;
    this->mGameObject->mCharState = Char_B4_state::Walking_1;

    Char* pChar = this->mGameObject;
    pChar->mAnimationState = Char_Anim_state::Walking_0;
    pChar->mAnimationFrame = 0;

    this->mHealth = 50;
    return gSpriteGrid_1->FindCollidingSpritePreferType(this->mGameObject->mSpritePtr,
                                                        sprite_types_enum::car_2);
}

// https://decomp.me/scratch/ec0hn
MATCH_FUNC(0x4672e0)
void Ped::UpdateMovementTowardsTarget(Fix16 distance, u8 type)
{
    Ang16 angle;
    Fix16 x;
    Fix16 y;
    Fix16 z;
    bool bMoveTargetSet = false;
    bool bTargetNotReached = true;
    // Flags VC6 threads away: none of them appears in the code, but they decide the block layout (the
    // bNextPoint block sits after the null-path block, the give-up block is laid out last with its own
    // epilogue and the exit is moved up behind the type check). See docs/matching_quirks.md.
    bool bNextPoint = false;
    bool bUseCurrentPath = false;
    bool bGiveUp = false;
    mBf21C.bNoPathFound = false;

    switch (type)
    {
        case ped_move_target_type::internal_target_ped_0:
            x = mInternalTargetPed->get_cam_x();
            y = mInternalTargetPed->get_cam_y();
            z = mInternalTargetPed->get_cam_z();
            break;
        case ped_move_target_type::internal_target_pos_1:
            x = mInternalTargetX;
            y = mInternalTargetY;
            z = mInternalTargetZ;
            break;
        case ped_move_target_type::move_target_pos_2:
            x = mMoveTargetX;
            y = mMoveTargetY;
            z = mTargetToEnter->mCarSprite->mZpos;
            break;
        case ped_move_target_type::objective_target_ped_3:
            x = mObjectiveTargetPed->get_cam_x();
            y = mObjectiveTargetPed->get_cam_y();
            z = mObjectiveTargetPed->get_cam_z();
            break;
        case ped_move_target_type::objective_target_pos_4:
            x = mObjectiveTargetX;
            y = mObjectiveTargetY;
            z = mObjectiveTargetZ;
            break;
        case ped_move_target_type::objective_target_car_5:
            x = mTargetObjectiveCar->mCarSprite->mXy.x;
            y = mTargetObjectiveCar->mCarSprite->mXy.y;
            z = mTargetObjectiveCar->mCarSprite->mZpos;
            break;
        case ped_move_target_type::internal_target_object_6:
            x = mInternalTargetObject->GetX();
            y = mInternalTargetObject->GetY();
            z = mInternalTargetObject->GetZ();
            break;
        case ped_move_target_type::objective_target_object_7:
            x = mObjectiveTargetObject->GetX();
            y = mObjectiveTargetObject->GetY();
            z = mObjectiveTargetObject->GetZ();
            break;
        default:
            break;
    }
    angle = Fix16::atan2_fixed(y - mCam.y, x - mCam.x);
    mBf21C.bFindingPath = true;
    if (mBf21C.bFindingPath == true)
    {
        if (distance < kFpTwo_678658)
        {
            if (distance < kFpOne_678798)
            {
                if (mCam.z == z ||
                    (Fix16::Abs(mCam.z - z) <= kFpHalf_67853C && mCam.x.ToUInt8() == x.ToUInt8() &&
                     mCam.y.ToUInt8() == y.ToUInt8()))
                {
                    bTargetNotReached = false;
                    mPathPointCount = 0;
                }
                else
                {
                    bTargetNotReached = true;
                }
            }
            else
            {
                // Every case does the same thing. The original (9.6f too) keeps a dead load of type here,
                // which a switch with two case labels folded into its default leaves behind.
                switch (type)
                {
                    case ped_move_target_type::objective_target_ped_3:
                    case ped_move_target_type::objective_target_pos_4:
                        bTargetNotReached = true;
                        break;
                    default:
                        bTargetNotReached = true;
                        break;
                }
            }
        }

        if (mGameObject->mIsCollidingWithSprite && mCurrentPathPoint != NULL &&
            mCam.x.ToUInt8() == mCurrentPathPoint->mX && mCam.y.ToUInt8() == mCurrentPathPoint->mY)
        {
            mCurrentPathPoint = mCurrentPathPoint + 1;
            bMoveTargetSet = true;
            mMoveTargetX = kFpHalf_67853C + Fix16(mCurrentPathPoint->mX);
            mMoveTargetY = kFpHalf_67853C + Fix16(mCurrentPathPoint->mY);
        }
        else if (!bTargetNotReached)
        {
            bGiveUp = true;
        }

        if (!bGiveUp)
        {
            sPedUsedPathThisTick = 1;
            Ped::ChangeNextPedState1(ped_state_1::findind_path_2);
            Ped::ChangeNextPedState2(ped_state_2::ped2_walking_0);
            if (!mBf21C.bInPathList)
            {
                gPathFinder->mPedList.AddPedToBackIfMissing(this);
                mBf21C.bInPathList = true;
            }

            switch (gPathFinder->IsFirstInPathQueue(this))
            {
                case 1:
                    if (!gPathFinder->ComputePath(mId,
                                                               this,
                                                               mCam.x.ToUInt8(),
                                                               mCam.y.ToUInt8(),
                                                               mCam.z.ToUInt8(),
                                                               x.ToInt(),
                                                               y.ToInt(),
                                                               z.ToInt(),
                                                               Ang16::GetAngleFace(angle),
                                                               &this->mPathPointCount))
                    {
                        bUseCurrentPath = true;
                        break;
                    }

                    gPathFinder->mPedList.RemovePed_4711F0(this);
                    mCurrentPathPoint = &mPatrolPoints[0];
                    mBf21C.bInPathList = false;

                    while (true)
                    {
                        if (mCurrentPathPoint->mX == 0)
                        {
                            mCurrentPathPoint = &mPatrolPoints[0];
                            break;
                        }
                        if (mCurrentPathPoint->mX == mCam.x.ToUInt8() &&
                            mCurrentPathPoint->mY == mCam.y.ToUInt8() &&
                            mCurrentPathPoint->mZ == mCam.z.ToUInt8())
                        {
                            break;
                        }
                        mCurrentPathPoint++;
                    }
                    // fall through

                default:
                    if (mCurrentPathPoint->mX != 0 || mCurrentPathPoint->mY != 0)
                    {
                        bNextPoint = true;
                    }
                    else
                    {
                        mBf21C.bNoPathFound = true;
                        Ped::ChangeNextPedState1(ped_state_1::flee_or_running_1);
                        Ped::ChangeNextPedState2(ped_state_2::running_to_target_2);
                        mBf21C.bFindingPath = false;
                        mPatrolPoints[0].mX = 0;
                        mPatrolPoints[0].mY = 0;
                    }
                    break;

                case 0:
                    bUseCurrentPath = true;
                    break;
            }

            if (bUseCurrentPath)
            {
                if (mCurrentPathPoint)
                {
                    if (mCurrentPathPoint->mX == 0 && mCurrentPathPoint->mY == 0)
                    {
                        Ped::ChangeNextPedState1(ped_state_1::flee_or_running_1);
                        Ped::ChangeNextPedState2(ped_state_2::running_to_target_2);
                        mBf21C.bFindingPath = false;
                        mPatrolPoints[0].mX = 0;
                        mPatrolPoints[0].mY = 0;
                    }
                    else
                    {
                        mMoveTargetX = Fix16(mCurrentPathPoint->mX);
                        mMoveTargetY = Fix16(mCurrentPathPoint->mY);

                        Fix16 dist_1 = (kFpHalf_67853C + Fix16(mMoveTargetX.ToUInt8())) - mCam.x;
                        Fix16 dist_2 = (kFpHalf_67853C + Fix16(mMoveTargetY.ToUInt8())) - mCam.y;

                        // Assigned, not initialised: both stay in memory for the pointer select of the Max.
                        Fix16 abs_dx;
                        Fix16 abs_dy;
                        abs_dx = Fix16::Abs(dist_1);
                        abs_dy = Fix16::Abs(dist_2);

                        if (Fix16::Max(abs_dx, abs_dy) < kFpHalf || (mGameObject->mFlagsBf.bJumpedOffEdge))
                        {
                            mCurrentPathPoint++;
                            mMoveTargetX = kFpHalf_67853C + Fix16(mCurrentPathPoint->mX);
                            mMoveTargetY = kFpHalf_67853C + Fix16(mCurrentPathPoint->mY);
                            mMoveTargetZ = Fix16(mCurrentPathPoint->mZ);
                        }
                        else
                        {
                            mMoveTargetX = kFpHalf_67853C + Fix16(mCurrentPathPoint->mX);
                            mMoveTargetY = kFpHalf_67853C + Fix16(mCurrentPathPoint->mY);
                            mMoveTargetZ = Fix16(mCurrentPathPoint->mZ);
                        }
                        bMoveTargetSet = true;
                    }
                }
                else
                {
                    mPatrolPoints[0].mX = 0;
                    mPatrolPoints[0].mY = 0;
                    Ped::ChangeNextPedState1(ped_state_1::flee_or_running_1);
                    Ped::ChangeNextPedState2(ped_state_2::running_to_target_2);
                    mBf21C.bFindingPath = false;
                }
            }

            if (bNextPoint)
            {
                mMoveTargetX = kFpHalf_67853C + Fix16(mCurrentPathPoint->mX);
                mMoveTargetY = kFpHalf_67853C + Fix16(mCurrentPathPoint->mY);
                mMoveTargetZ = Fix16(mCurrentPathPoint->mZ);
                mBf21C.bPathSteppedThisRound = true;
                gPathStepSlotFree = 0;
                bMoveTargetSet = true;
            }
        }
    }
    else
    {
        bGiveUp = true;
    }

    if (bGiveUp)
    {
        Ped::ChangeNextPedState1(ped_state_1::flee_or_running_1);
        Ped::ChangeNextPedState2(ped_state_2::running_to_target_2);
        mBf21C.bFindingPath = false;
        mBf21C.bPathSteppedThisRound = false;
    }

    if (bMoveTargetSet)
    {
        if (type >= 3 && (type <= 5 || type == 7))
        {
            mTargetX = mMoveTargetX;
            mTargetY = mMoveTargetY;
            mTargetZ = mMoveTargetZ;
        }
    }
}

MATCH_FUNC(0x4678e0)
void Ped::ProcessObjectiveFleeOnFootTillSafe()
{
    if (gObjectiveMayAct)
    {
        if (gDistanceToTarget > kFpSix)
        {
            if (mGameObject)
            {
                if (mGameObject->mBlockType == 2)
                {
                    // back to normality
                    Ped::ChangeNextPedState1(ped_state_1::walking_0);
                    Ped::ChangeNextPedState2(ped_state_2::ped2_walking_0);
                    mObjectiveStatus = objective_status::passed_1;
                }
                else
                {
                    // RUN
                    mGameObject->mVelocity = get_max_speed_1F0();
                }
            }
            else
            {
                mBf21C.bAttacking = false;
            }
        }
        else
        {
            // It's very close to the threat, so run!
            mGameObject->mVelocity = get_max_speed_1F0();
        }
    }
}

MATCH_FUNC(0x467960)
void Ped::FleeCharOnFootTillSafe()
{
    if (mObjectiveTargetPed->mPedState1 == ped_state_1::dead_9 ||
        !mObjectiveTargetPed->mBf21C.bActive)
    {
        Ped::ChangeNextPedState1(ped_state_1::walking_0);
        Ped::ChangeNextPedState2(ped_state_2::ped2_walking_0);
        this->mObjectiveTargetPed = NULL;
        this->mObjectiveStatus = objective_status::passed_1;
        return;
    }

    if (gObjectiveMayAct)
    {
        if (gDistanceToTarget > kFpSix)
        {
            if (this->mGameObject->mBlockType == 2)
            {
                Ped::ChangeNextPedState1(ped_state_1::walking_0);
                Ped::ChangeNextPedState2(ped_state_2::ped2_walking_0);
                this->mObjectiveTargetPed = NULL;
                this->mObjectiveStatus = objective_status::passed_1;
            }
        }
        else
        {
            Ped::ChangeNextPedState1(ped_state_1::flee_or_running_1);
            Ped::ChangeNextPedState2(ped_state_2::running_away_3);
            this->mGameObject->UseRunOrJumpSpeed();
            mBf21C.bAttacking = false;
        }
    }
}

MATCH_FUNC(0x467a20)
void Ped::FleeFromCharOnFootAlways()
{
    if (mObjectiveTargetPed->mPedState1 == ped_state_1::dead_9 ||
        !mObjectiveTargetPed->mBf21C.bActive)
    {
        // Only back to normality if the menacing ped is died or (maybe) cannot move
        Ped::ChangeNextPedState1(ped_state_1::walking_0);
        Ped::ChangeNextPedState2(ped_state_2::ped2_walking_0);
        mObjectiveTargetPed = NULL;
        mObjectiveStatus = objective_status::passed_1;
    }
    else if (gObjectiveMayAct)
    {
        // Run
        mGameObject->UseRunOrJumpSpeed();
        mBf21C.bAttacking = false;
    }
}

MATCH_FUNC(0x467ad0)
void Ped::FleeCharAlwaysOnceCarStopped()
{
    if (mCar)
    {
        if (mCar->GetVelocity() == kFpZero_678660 && mInternalObjective != objectives_enum::leave_car_36)
        {
            Ped::SetObjective2(objectives_enum::leave_car_36);
            mTargetToEnter = mCar;
        }
    }
    else if (gObjectiveMayAct)
    {
        Ped* pOldTarget = mObjectiveTargetPed;
        Ped::SetObjective(objectives_enum::flee_char_on_foot_always_3);
        mObjectiveTargetPed = pOldTarget;
    }
}

MATCH_FUNC(0x467bd0)
void Ped::LeaveCarAndFleeFromIt()
{
    if (mCar)
    {
        if (mCar->GetVelocity() == kFpZero_678660 && mInternalObjective != objectives_enum::leave_car_36)
        {
            Ped::SetObjective2(objectives_enum::leave_car_36);
            mTargetToEnter = mCar;
        }
    }
    else if (gObjectiveMayAct)
    {
        mTargetToEnter = mTargetObjectiveCar;
        Ped::SetObjective(objectives_enum::flee_on_foot_till_safe_1);
        mTargetX = mTargetToEnter->mCarSprite->mXy.x;
        mTargetY = mTargetToEnter->mCarSprite->mXy.y;
        mTargetZ = mGameObject->mSpritePtr->mZpos;
    }
}

MATCH_FUNC(0x467ca0)
void Ped::KillCharOnFoot()
{
    if (!mObjectiveTargetPed->IsActive() || mObjectiveTargetPed->GetPedState() == ped_state_1::dead_9)
    {
        if (mObjectiveTargetPed->GetPedState() == ped_state_1::dead_9)
        {
            mObjectiveStatus = objective_status::passed_1;
        }
        else
        {
            mObjectiveStatus = objective_status::failed_2;
        }
    }
    else
    {
        if (mStolenCar)
        {
            if (mStolenCar->mDespawnStatus == car_despawn_status::despawning_5)
            {
                mStolenCar = NULL;
            }
            else
            {
                mStolenCar->mLastSeenTimer = 0;
            }
        }

        if (mObjectiveTargetPed->IsInvisibleOnFoot_433DA0())
        {
            if (mInternalObjective == objectives_enum::walk_to_point_17)
            {
                return;
            }
            Ped::SetObjective2(objectives_enum::no_obj_0);
            return;
        }

        if (!gObjectiveMayAct || mBf21C.bPanicking)
        {
            return;
        }

        switch (mInternalObjective)
        {
            case objectives_enum::kill_char_on_foot_20:
                if (mInternalObjectiveStatus == objective_status::passed_1)
                {
                    if (mInternalTargetPed != mObjectiveTargetPed)
                    {
                        Ped::SetObjective2(objectives_enum::no_obj_0);
                        return;
                    }
                    mObjectiveStatus = objective_status::passed_1;
                    return;
                }
                if (mInternalObjectiveStatus == objective_status::failed_2)
                {
                    Ped::SetObjective2(objectives_enum::kill_char_on_foot_20);
                    mInternalTargetPed = mObjectiveTargetPed;
                    mBf21C.bPanicking = false;
                }
                return;

            case objectives_enum::flee_on_foot_till_safe_1:
                if (mInternalObjectiveStatus == objective_status::passed_1)
                {
                    Ped::SetObjective2(objectives_enum::kill_char_on_foot_20);
                    mInternalTargetPed = mObjectiveTargetPed;
                }
                return;

            case objectives_enum::no_obj_0:
                Ped::SetObjective2(objectives_enum::kill_char_on_foot_20);
                mInternalTargetPed = mObjectiveTargetPed;
                return;
        }
    }
}

MATCH_FUNC(0x467e20)
void Ped::KillCharAnyMeans()
{
    if (!mObjectiveTargetPed->IsActive() || mObjectiveTargetPed->GetPedState() == ped_state_1::dead_9)
    {
        if (mObjectiveTargetPed->GetPedState() == ped_state_1::dead_9)
        {
            // Target ped was killed/died
            mObjectiveStatus = objective_status::passed_1;
            Ped::SetObjective2(objectives_enum::no_obj_0);
            if (mCar)
            {
                // If the assassin is on a car, cancel routes
                mPedState1 = ped_state_1::in_car_10;
                mPedState2 = ped_state_2::ped2_driving_10;
                if (mCar->mAI)
                {
                    char_type route_idx = mCar->mAI->mRouteIdx;
                    if (route_idx > 0)
                    {
                        gRouteFinder->CancelRoute(route_idx);
                    }
                }
                if (mCar->mpChaseTask)
                {
                    gCarChaseTaskTable->FreeEntry(mCar->mpChaseTask);
                    mCar->mpChaseTask = NULL;
                }
            }
        }
        else
        {
            // Target ped is alive
            mObjectiveStatus = objective_status::failed_2;
        }
    }
    else
    {
        if (mObjectiveTargetPed->IsInvisibleOnFoot_433DA0())
        {
            Ped::SetObjective2(objectives_enum::no_obj_0);
            if (mCar)
            {
                mPedState1 = ped_state_1::in_car_10;
                mPedState2 = ped_state_2::ped2_driving_10;
                if (mCar->mAI)
                {
                    char_type route_idx = mCar->mAI->mRouteIdx;
                    if (route_idx > 0)
                    {
                        gRouteFinder->CancelRoute(route_idx);
                    }
                }
                if (mCar->mpChaseTask)
                {
                    gCarChaseTaskTable->FreeEntry(mCar->mpChaseTask);
                    mCar->mpChaseTask = NULL;
                }
            }
        }
        else if (mGameObject)
        {
            if (gDistanceToTarget > kFpFour)
            {
                mObjectiveTargetX = mObjectiveTargetPed->get_cam_x();
                mObjectiveTargetY = mObjectiveTargetPed->get_cam_y();
                mObjectiveTargetZ = mObjectiveTargetPed->get_cam_z();
                Ped::GotoAreaByAnyMeans();
            }
            else if (mInternalObjective != objectives_enum::kill_char_on_foot_20)
            {
                Ped::SetObjective2(objectives_enum::kill_char_on_foot_20);
                mInternalTargetPed = mObjectiveTargetPed;
            }
        }
        else
        {
            Ped::GotoAreaByAnyMeans();
        }
    }
}

MATCH_FUNC(0x467fb0)
void Ped::KillFrenzy()
{
    if (gObjectiveMayAct)
    {
        if (!mObjectiveTimer)
        {
            mObjectiveStatus = objective_status::passed_1;
        }
    }
}

MATCH_FUNC(0x467fd0)
void Ped::PunchChar()
{
    if (!mObjectiveTargetPed->IsActive() ||
        mObjectiveTargetPed->GetPedState() == ped_state_1::dead_9)
    {
        this->mObjectiveStatus = objective_status::failed_2;
        return;
    }

    if (gObjectiveMayAct)
    {
        if (mInternalObjective != objectives_enum::no_obj_0)
        {
            if (mInternalObjective == objectives_enum::punch_char_23)
            {
                if (mInternalObjectiveStatus == objective_status::passed_1)
                {
                    this->mObjectiveStatus = objective_status::passed_1;
                    return;
                }
                else if (mInternalObjectiveStatus == objective_status::failed_2)
                {
                    this->mObjectiveStatus = objective_status::failed_2;
                }
            }
        }
        else
        {
            Ped::SetObjective2(objectives_enum::punch_char_23);
            this->mInternalTargetPed = this->mObjectiveTargetPed;
            return;
        }
    }
}

MATCH_FUNC(0x468040)
void Ped::WaitOnFootAtTarget()
{
    bool bUpdateMovement = true;
    if (this->mOccupation == ped_ocupation_enum::drone)
    {
        gDistanceToTarget = kFpZero_678660;
        this->mObjectiveTargetZ = this->mCam.z;
    }

    if (!gObjectiveMayAct)
    {
        bUpdateMovement = this->mGameObject->GetCharState() == Char_B4_state::Jumping_15 && !this->mBf21C.bPanicking;
    }

    if (mBf224.bWaitingOnTarget && this->mBf21C.bPanicking)
    {
        this->mWaitCounter = 0;
        this->mBf224.bWaitingOnTarget = false;
    }

    if (bUpdateMovement)
    {
        if (this->mBf224.bWaitingOnTarget ||
            gDistanceToTarget <= kFpThreeSixteenths &&
                abs_sub_less_than_epislon(this->mCam.z, this->mObjectiveTargetZ))
        {
            if (mGameObject->GetCharState() != Char_B4_state::Jumping_15)
            {
                if (this->mBf224.bWaitingOnTarget)
                {
                    mGameObject->RegulateVelocityByRef_433970(kFpZero_678438);
                }

                ChangeNextPedState1(ped_state_1::standing_still_7);
                ChangeNextPedState2(ped_state_2::ped2_staying_14);
            }
            else
            {
                this->mBf224.bWaitingOnTarget = true;
            }
        }
        else
        {
            if (mGameObject->GetCharState() != Char_B4_state::Jumping_15)
            {
                if (gDistanceToTarget > kFpThreeEighths)
                {
                    mGameObject->SetVelocity(this->mMaxSpeed);
                    UpdateMovementTowardsTarget(gDistanceToTarget, ped_move_target_type::objective_target_pos_4);
                }
                else
                {
                    mGameObject->SetVelocity(this->mWalkSpeed);
                    UpdateMovementTowardsTarget(gDistanceToTarget, ped_move_target_type::objective_target_pos_4);
                }
            }
        }
    }

    this->mTargetFacingAngle = this->mRotation;

    if (mObjectiveTimer == 0)
    {
        this->mObjectiveStatus = objective_status::passed_1;
    }
}

MATCH_FUNC(0x4682a0)
void Ped::TimeWaitedInCar()
{
    if (this->mCar == NULL)
    {
        this->mObjectiveTimer = 9999;
    }
    else
    {
        ++this->mObjectiveTimer;
        if (this->mObjectiveTimer > 9999u)
        {
            this->mObjectiveTimer = 9999;
        }
    }
}

MATCH_FUNC(0x468310)
void Ped::ProcessObjectiveGotoAreaInCar()
{
    LOCAL_STATIC(Object*, sNullTargetObject, 0x678558);
    if (this->mObjectiveStatus != objective_status::passed_1)
    {
        if (this->mGameObject)
        {
            this->mObjectiveStatus = objective_status::failed_2;
        }
        else
        {
            if (!this->mCar->mpChaseTask)
            {
                this->mCar->mpChaseTask = gCarChaseTaskTable->AllocateEntry();
                this->mCar->mpChaseTask->mpDriver = this;
            }

            this->mCar->mpChaseTask->mTaskType = car_task_type::goto_position_1;
            this->mCar->mpChaseTask->mFollowingRoute = true;
            this->mCar->mpChaseTask->mCanSnapToTarget = false;
            this->mCar->mpChaseTask->mTargetX = this->mObjectiveTargetX;
            this->mCar->mpChaseTask->mTargetY = this->mObjectiveTargetY;
            this->mCar->mpChaseTask->mTargetZ = this->mObjectiveTargetZ;
            this->mCar->ClearAIHalted();

            Ped* pDriver = this->mCar->mDriver;
            if (pDriver)
            {
                if (pDriver->PedTypeIs(ped_type::special_ped_4) || this->mCar->mDriver->PedTypeIs(ped_type::dummy_with_occupation_6))
                {
                    Car* pCar = this->mCar;
                    if (pCar->mDriver->mGraphicType == ped_graphic_type::cop_2)
                    {
                        pCar->mpChaseTask->mCanSnapToTarget = true;
                        this->mCar->mpChaseTask->mFollowingRoute = true;
                    }
                }
            }

            if (static_cast<u8>(this->mCam.x.ToInt()) == static_cast<u8>(this->mObjectiveTargetX.ToInt()) &&
                static_cast<u8>(this->mCam.y.ToInt()) == static_cast<u8>(this->mObjectiveTargetY.ToInt()) &&
                this->mCam.z == this->mObjectiveTargetZ)
            {
                Car* pCar = this->mCar;
                this->mObjectiveStatus = objective_status::passed_1;
                gCarChaseTaskTable->FreeEntry(pCar->mpChaseTask);
                this->mCar->mpChaseTask = NULL;
                this->mCar->SetAIHalted();
                this->mObjectiveTargetObject = sNullTargetObject;
            }
            else
            {
                Car* pCar = this->mCar;
                if (pCar->mpChaseTask->mRouteFinished)
                {
                    this->mObjectiveStatus = objective_status::passed_1;
                    gCarChaseTaskTable->FreeEntry(pCar->mpChaseTask);
                    this->mCar->mpChaseTask = NULL;
                    this->mCar->SetAIHalted();
                }
                else if (!pCar)
                {
                    this->mObjectiveTimer = 9999;
                }
                else
                {
                    if (pCar->GetVelocity() == kFpZero_678660)
                    {
                        ++this->mObjectiveTimer;
                    }
                    else
                    {
                        this->mObjectiveTimer = 0;
                    }
                    if (this->mObjectiveTimer > 9999u)
                    {
                        this->mObjectiveTimer = 9999;
                    }
                }
            }
        }
    }
}

MATCH_FUNC(0x4686c0)
void Ped::EnterTargetObjectiveCar()
{
    if (mGameObject)
    {
        mGameObject->Set_F84(mTargetObjectiveCar);
    }

    if (mInternalObjective)
    {
        if (mInternalObjective == objectives_enum::enter_car_as_driver_35)
        {
            if (mInternalObjectiveStatus == objective_status::passed_1)
            {
                if (!mCar)
                {
                    Car* pOldCar = mTargetObjectiveCar;
                    Ped::SetObjective(objectives_enum::enter_car_as_driver_35);
                    Ped::SetObjective2(objectives_enum::no_obj_0);
                    mTargetObjectiveCar = pOldCar;
                }
                else
                {
                    mObjectiveStatus = objective_status::passed_1;
                    Ped::SetObjective2(objectives_enum::no_obj_0);
                    Ped::ChangeNextPedState1(ped_state_1::in_car_10);
                    Ped::ChangeNextPedState2(ped_state_2::ped2_driving_10);
                }
                return;
            }
            if (mInternalObjectiveStatus == objective_status::failed_2)
            {
                mObjectiveStatus = objective_status::failed_2;
            }
        }
    }
    else
    {
        Ped::SetObjective2(objectives_enum::enter_car_as_driver_35);
        mTargetToEnter = mTargetObjectiveCar;
    }
    if (mTargetObjectiveCar->IsDespawning() || mTargetObjectiveCar->IsMaxDamage())
    {
        mObjectiveStatus = objective_status::failed_2;
        Ped::SetObjective2(objectives_enum::no_obj_0);
        mBf21C.bPanicking = false;
    }
}

MATCH_FUNC(0x468820)
void Ped::LeaveTargetObjectiveCar()
{
    if (mGameObject)
    {
        if (!mTargetObjectiveCar && mPedState2 == ped_state_2::ped2_at_car_door_17)
        {
            mObjectiveStatus = objective_status::passed_1;
        }
    }
    if (mBf21C.bPanicking)
    {
        if (mGameObject)
        {
            if (mPedGroup)
            {
                if (mPedGroup->AreAllMembersOnFoot())
                {
                    mObjectiveStatus = objective_status::passed_1;
                }
                else
                {
                    mObjectiveStatus = objective_status::not_finished_0;
                }
            }
            else
            {
                mObjectiveStatus = objective_status::passed_1;
            }
        }
    }

    if (mInternalObjective)
    {
        if (mInternalObjective == objectives_enum::leave_car_36)
        {
            if (mInternalObjectiveStatus == objective_status::passed_1)
            {
                if (mPedType != ped_type::player_2)
                {
                    mObjectiveStatus = objective_status::passed_1;
                }
                else
                {
                    Ped::SetObjective(objectives_enum::no_obj_0);
                }

                Ped::SetObjective2(objectives_enum::no_obj_0);
            }
            else if (mInternalObjectiveStatus == objective_status::failed_2)
            {
                mObjectiveStatus = objective_status::failed_2;
            }
        }
    }
    else if (mCar)
    {
        Ped::SetObjective2(objectives_enum::leave_car_36);
        mTargetToEnter = mTargetObjectiveCar;
    }
    else
    {
        if (mPedGroup)
        {
            if (mPedGroup->AreAllMembersOnFoot())
            {
                mObjectiveStatus = objective_status::passed_1;
            }
            else
            {
                mObjectiveStatus = objective_status::not_finished_0;
            }
        }
        else
        {
            mObjectiveStatus = objective_status::passed_1;
        }
    }
}

MATCH_FUNC(0x468930)
void Ped::EnterTrain()
{
    if (mInternalObjectiveStatus == objective_status::failed_2 || mTargetObjectiveCar == NULL)
    {
        mObjectiveStatus = objective_status::failed_2;
    }
    else
    {
        if (mGameObject)
        {
            mGameObject->Set_F84(mTargetObjectiveCar);
        }

        if (mInternalObjective)
        {
            if (mInternalObjective == objectives_enum::enter_train_37)
            {
                if (mInternalObjectiveStatus == objective_status::passed_1)
                {
                    mObjectiveStatus = objective_status::passed_1;
                    Ped::SetObjective2(objectives_enum::no_obj_0);
                    Ped::ChangeNextPedState1(ped_state_1::in_car_10);
                    Ped::ChangeNextPedState2(ped_state_2::ped2_driving_10);
                    return;
                }
                if (mInternalObjectiveStatus == objective_status::failed_2)
                {
                    mObjectiveStatus = objective_status::failed_2;
                }
            }
        }
        else
        {
            Ped::SetObjective2(objectives_enum::enter_train_37);
            mTargetToEnter = mTargetObjectiveCar;
        }
        if (mTargetObjectiveCar->IsDespawning() || mTargetObjectiveCar->IsMaxDamage())
        {
            mObjectiveStatus = objective_status::failed_2;
            Ped::SetObjective2(objectives_enum::no_obj_0);
        }
    }
}

MATCH_FUNC(0x468a00)
void Ped::LeaveTrain()
{
    if (mInternalObjective)
    {
        if (mInternalObjective != objectives_enum::goto_area_on_foot_12)
        {
            if (mInternalObjective == objectives_enum::leave_train_38)
            {
                if (mInternalObjectiveStatus == objective_status::passed_1)
                {
                    if (mTargetObjectiveCar->IsTrainModel())
                    {
                        if (mPedType != ped_type::player_2)
                        {
                            Ped::SetObjective2(objectives_enum::goto_area_on_foot_12);
                            switch (Ang16::GetAngleFace(mFacingAngle))
                            {
                                case path_direction::up_1:
                                    mInternalTargetX = mCam.x;
                                    mInternalTargetY = mCam.y - kFpOne_678664;
                                    break;
                                case path_direction::right_3:
                                    mInternalTargetX = kFpOne_678664 + mCam.x;
                                    mInternalTargetY = mCam.y;
                                    break;
                                case path_direction::down_2:
                                    mInternalTargetX = mCam.x;
                                    mInternalTargetY = kFpOne_678664 + mCam.y;
                                    break;
                                case path_direction::left_4:
                                    mInternalTargetX = mCam.x - kFpOne_678664;
                                    mInternalTargetY = mCam.y;
                                    break;
                                default:
                                    break;
                            }
                            mInternalTargetZ = mCam.z;
                        }
                        else
                        {
                            Ped::SetObjective(objectives_enum::no_obj_0);
                            Ped::SetObjective2(objectives_enum::no_obj_0);
                        }
                    }
                    else
                    {
                        mObjectiveStatus = objective_status::passed_1;
                    }
                }
                if (mInternalObjectiveStatus == objective_status::failed_2)
                {
                    mObjectiveStatus = objective_status::failed_2;
                }
            }
        }
        else
        {
            if (mPedState1 != ped_state_1::immobilized_8 && mInternalObjectiveStatus == objective_status::passed_1)
            {
                mObjectiveStatus = objective_status::passed_1;
                Ped::SetObjective2(objectives_enum::no_obj_0);
            }
        }
    }
    else if (mCar)
    {
        Ped::SetObjective2(objectives_enum::leave_train_38);
        mTargetToEnter = mTargetObjectiveCar;
    }
    else
    {
        mObjectiveStatus = objective_status::passed_1;
    }
}

MATCH_FUNC(0x468bd0)
void Ped::LeaveCarThenStop()
{
    if (mInternalObjective)
    {
        if (mInternalObjective == objectives_enum::leave_car_36 && !mCar)
        {
            SetStateInitPending();
            mPedState1 = ped_state_1::immobilized_8;
            mPedState2 = ped_state_2::ped2_at_car_door_17;
            mGameObject->Set_F8_ped_state_1_433910(ped_state_1::immobilized_8);
            mGameObject->SetPedState2(ped_state_2::ped2_at_car_door_17);
            mCar = NULL;
            Ped::SetObjective(objectives_enum::no_obj_0);
            Ped::SetObjective2(objectives_enum::no_obj_0);
        }
    }
    else
    {
        Ped::SetObjective2(objectives_enum::leave_car_36);
        set_target_to_enter(mTargetObjectiveCar);
    }
}

MATCH_FUNC(0x468c70)
void Ped::PatrolOnFoot()
{
    if (gObjectiveMayAct)
    {
        if (mInternalObjective == objectives_enum::goto_area_on_foot_12)
        {
            if (!mBf21C.bPanicking)
            {
                if (mInternalObjectiveStatus)
                {
                    mCurrentPatrolPoint = mCurrentPatrolPoint + 1;
                    if (!mCurrentPatrolPoint->mX)
                    {
                        mCurrentPatrolPoint = mPatrolRoute->mPoints;
                    }
                    Ped::SetObjective2(objectives_enum::goto_area_on_foot_12);
                    mInternalTargetX = kFpHalf_67853C + Fix16(mCurrentPatrolPoint->mX);
                    mInternalTargetY = kFpHalf_67853C + Fix16(mCurrentPatrolPoint->mY);
                    mInternalTargetZ = Fix16(mCurrentPatrolPoint->mZ);
                }
                mGameObject->RegulateVelocity(mWalkSpeed);
            }
        }
        else if (!mBf21C.bPanicking)
        {
            mCurrentPatrolPoint = mPatrolRoute->mPoints;
            Ped::SetObjective2(objectives_enum::goto_area_on_foot_12);
            mInternalTargetX = kFpHalf_67853C + Fix16(mCurrentPatrolPoint->mX);
            mInternalTargetY = kFpHalf_67853C + Fix16(mCurrentPatrolPoint->mY);
            mInternalTargetZ = Fix16(mCurrentPatrolPoint->mZ);
            mGameObject->RegulateVelocity(mWalkSpeed);
        }
    }
}

MATCH_FUNC(0x468de0)
void Ped::ProcessObjectiveGotoAreaOnFoot()
{
    if (mOccupation == ped_ocupation_enum::drone)
    {
        mMaxSpeed = kFpOneThirtySecond;
    }
    if (gObjectiveMayAct)
    {
        if (gDistanceToTarget < kFpQuarter)
        {
            if (mGameObject->GetCharState() != Char_B4_state::Jumping_15)
            {
                // ped reached its destination
                Ped::ChangeNextPedState1(ped_state_1::standing_still_7);
                Ped::ChangeNextPedState2(ped_state_2::ped2_staying_14);
                mObjectiveStatus = objective_status::passed_1;
            }
        }
        else
        {
            if (mObjectiveTimer == 0)
            {
                // Time out. Objective passed anyway O.o, must be failed? OG bug?
                mObjectiveStatus = objective_status::passed_1;
            }
            mGameObject->SetVelocity(mMaxSpeed);
        }
        Ped::UpdateMovementTowardsTarget(gDistanceToTarget, ped_move_target_type::objective_target_pos_4);
    }
}

MATCH_FUNC(0x468e80)
void Ped::UpdateFollowPedObjective()
{
    bool bTargetDead = false;
    bool bUpdateMovement = true;

    Ped* pTargetPed = this->mObjectiveTargetPed;
    if (pTargetPed->GetPedState() == ped_state_1::dead_9 &&
        pTargetPed->get_objective() != objectives_enum::dead_patient_28)
    {
        bTargetDead = true;
    }

    if (!pTargetPed->IsActive() || bTargetDead)
    {
        this->mObjectiveStatus = objective_status::failed_2;
    }
    else
    {
        if (!gObjectiveMayAct)
        {
            bUpdateMovement = this->mGameObject->GetCharState() == Char_B4_state::Jumping_15 && !this->mBf21C.bPanicking;
        }

        if (mBf224.bWaitingOnTarget && (this->mBf21C.bPanicking || this->mWaitCounter > 200u))
        {
            this->mWaitCounter = 0;
            this->mBf224.bWaitingOnTarget = false;
        }

        if (bUpdateMovement)
        {
            if (this->mBf224.bWaitingOnTarget ||
                gDistanceToTarget <= kFpThreeSixteenths &&
                    abs_sub_less_than_epislon(this->mCam.z, pTargetPed->get_cam_z()))
            {
                if (mGameObject->GetCharState() != Char_B4_state::Jumping_15)
                {
                    if (this->mBf224.bWaitingOnTarget)
                    {
                        mGameObject->RegulateVelocityByRef_433970(kFpZero_678438);
                        ++this->mWaitCounter;
                    }
                    Ped::ChangeNextPedState1(ped_state_1::standing_still_7);
                    Ped::ChangeNextPedState2(ped_state_2::ped2_staying_14);
                    this->mObjectiveStatus = objective_status::passed_1;
                }
                else
                {
                    this->mBf224.bWaitingOnTarget = true;
                }
            }
            else
            {
                if (mGameObject->GetCharState() != Char_B4_state::Jumping_15)
                {
                    if (!this->mObjectiveTimer)
                    {
                        this->mObjectiveStatus = objective_status::passed_1;
                    }

                    if (gDistanceToTarget < kFpHalf)
                    {
                        mGameObject->SetVelocity(this->mWalkSpeed);
                    }
                    else
                    {
                        mGameObject->SetVelocity(this->mMaxSpeed);
                    }
                    Ped::UpdateMovementTowardsTarget(gDistanceToTarget, ped_move_target_type::objective_target_ped_3);
                }
            }
        }
    }
}

MATCH_FUNC(0x469010)
s32 Ped::GetValueByIdParity() const
{
    return (this->mId & 1) != 0 ? 56 : 51;
}

MATCH_FUNC(0x469030)
void Ped::UpdateKillerIdTimer()
{
    if (mKillerIdTimer > 0)
    {
        if (mPedState2 != ped_state_2::knocked_fatal_26)
        {
            mKillerIdTimer--;
            if (mKillerIdTimer == 0)
            {
                mKillerId = 0;
                mDeathCause = ped_death_cause::none_0;
            }
        }
    }
}

// https://decomp.me/scratch/rHsAD
MATCH_FUNC(0x469060)
void Ped::GotoAreaByAnyMeans()
{
    bool bCanAllocate;
    u8 xpos;
    u8 ypos;
    u8 zpos;
    u8 target_x;
    u8 target_y;
    u8 target_z;

    if (gDistanceToTarget > kFpFour && !mBf21C.bPanicking)
    {
        if (!mCar)
        {
            switch (mInternalObjective)
            {
                case objectives_enum::enter_car_as_driver_35:
                    if (mTargetToEnter->IsDespawning())
                    {
                        Ped::SetObjective2(objectives_enum::goto_area_on_foot_12, 128);
                        mEscapeCar = NULL;
                        mInternalTargetX = mObjectiveTargetX;
                        mInternalTargetY = mObjectiveTargetY;
                        mInternalTargetZ = mObjectiveTargetZ;
                    }
                    else
                    {
                        Sprite* pSprite = mTargetToEnter->mCarSprite;
                        if (Fix16::MaxAbsDistance(mCam.x, mCam.y, pSprite->mXy.x, pSprite->mXy.y) >
                            kFpTwo_678658)
                        {
                            if (mAttacker)
                            {
                                Ped::ReactToAttacker();
                            }
                            mAttacker = NULL;
                        }
                        if (mInternalObjective == objectives_enum::enter_car_as_driver_35 &&
                            (mTargetToEnter->IsDespawning() || mInternalObjectiveStatus == objective_status::failed_2))
                        {
                            Ped::SetObjective2(objectives_enum::goto_area_on_foot_12, 64);
                            mInternalTargetX = mObjectiveTargetX;
                            mInternalTargetY = mObjectiveTargetY;
                            mInternalTargetZ = mObjectiveTargetZ;
                        }
                    }
                    break;

                case objectives_enum::leave_car_36:
                    if (mInternalObjectiveStatus == objective_status::passed_1)
                    {
                        Ped::SetObjective2(objectives_enum::goto_area_on_foot_12, 128);
                        mInternalTargetX = mObjectiveTargetX;
                        mInternalTargetY = mObjectiveTargetY;
                        mInternalTargetZ = mObjectiveTargetZ;
                    }
                    break;

                case objectives_enum::goto_area_on_foot_12:
                    if (mInternalObjectiveStatus == objective_status::passed_1)
                    {
                        mInternalObjectiveStatus = objective_status::not_finished_0;
                        Ped::SetObjective2(objectives_enum::no_obj_0);
                    }
                    else
                    {
                        if (gDistanceToTarget > kFpSix_678678)
                        {
                            Ped::SetObjective2(objectives_enum::no_obj_0);
                        }
                        if (mAttacker)
                        {
                            Ped::ReactToAttacker();
                        }
                        mAttacker = NULL;
                    }
                    break;

                case objectives_enum::flee_char_on_foot_till_safe_2:
                    if (mEscapeCar)
                    {
                        Sprite* pSprite = mEscapeCar->mCarSprite;
                        if (Fix16::MaxAbsDistance(mCam.x, mCam.y, pSprite->mXy.x, pSprite->mXy.y) <
                                kFpFour ||
                            mInternalObjectiveStatus == objective_status::passed_1)
                        {
                            Ped::SetObjective2(objectives_enum::enter_car_as_driver_35);
                            mTargetToEnter = mEscapeCar;
                        }
                    }
                    else
                    {
                        Ped::SetObjective2(objectives_enum::no_obj_0);
                    }
                    break;

                case objectives_enum::kill_char_on_foot_20:
                {
                    // 9.6f calls get_cam_x/get_cam_y here; with bFound below they give the original's register allocation
                    if (Fix16::MaxAbsDistance(mCam.x, mCam.y, mInternalTargetPed->get_cam_x(), mInternalTargetPed->get_cam_y()) >
                            kFpTwo_678658 ||
                        mInternalObjectiveStatus == objective_status::passed_1)
                    {
                        mBf21C.bAttacking = false;
                        if (mEscapeCar && mEscapeCar->mDriver)
                        {
                            mEscapeCar = NULL;
                        }
                        if (mEscapeCar)
                        {
                            Ped::SetObjective2(objectives_enum::enter_car_as_driver_35);
                            mTargetToEnter = mEscapeCar;
                        }
                        else
                        {
                            Ped::SetObjective2(objectives_enum::no_obj_0);
                        }
                    }
                    break;
                }

                case objectives_enum::follow_char_9:
                    if (mPedGroup->Get_F3C() != 1)
                    {
                        if (mEscapeCar)
                        {
                            Ped::SetObjective2(objectives_enum::enter_car_as_driver_35);
                            mTargetToEnter = mEscapeCar;
                        }
                        else
                        {
                            Ped::SetObjective2(objectives_enum::no_obj_0);
                        }
                    }
                    break;

                default:
                {
                    Car* pCar;
                    if (!mBf21C.bSkipCarSearch)
                    {
                        pCar = gCarManager_6C->GetNearestEnterableCarFromCoord(mCam.x, mCam.y, mCam.z, this);
                        if (pCar)
                        {
                            Sprite* pSprite = pCar->mCarSprite;
                            if (Fix16::MaxAbsDistance(mCam.x, mCam.y, pSprite->mXy.x, pSprite->mXy.y) >
                                kFpFour)
                            {
                                pCar = NULL;
                            }
                        }

                        if (!pCar)
                        {
                            xpos = mCam.x.ToUInt8();
                            ypos = mCam.y.ToUInt8();
                            zpos = mCam.z.ToUInt8();

                            // bFound reaches another block, so the original's `cmp %bl,%al` (zero register) instead of `test`
                            u8 bFound = 0;
                            bCanAllocate = gCarManager_6C->CanAllocateCarOfKind(car_kind::recycled_1);
                            if (bCanAllocate)
                            {
                                bFound = gPathFinder->FindNearbyTileMatchingSlopeType(ROAD, &xpos, &ypos, &zpos, true);
                            }
                            if (bFound)
                            {
                                pCar = gCarManager_6C->SpawnCarAtRoadDirection(mGangCarModel, xpos, ypos, zpos);
                                if (pCar)
                                {
                                    pCar->IncrementCarStats(car_kind::recycled_1);
                                    if (mStolenCar && !mStolenCar->mDriver)
                                    {
                                        mStolenCar->SetDriverPedType(ped_type::dummy_3);
                                    }
                                    s8 gang_idx = gGangPool_CA8->FindGangByCarModel(mGangCarModel);
                                    if (gang_idx > -1)
                                    {
                                        Gang* pGang = gGangPool_CA8->GangByIdx(gang_idx);
                                        pCar->AttachGangIcon(pGang->mArrowColour);
                                        if (pGang->mGangCarRemap > -1)
                                        {
                                            pCar->SetCarRemap(pGang->mGangCarRemap);
                                        }
                                    }
                                    pCar->SetDriverPedType(ped_type::dummy_with_occupation_6);
                                    mStolenCar = pCar;
                                }
                            }
                            else
                            {
                                if (gDistanceToTarget < kFpFour)
                                {
                                    Ped::SetObjective2(objectives_enum::goto_area_on_foot_12);
                                    mInternalTargetX = mObjectiveTargetX;
                                    mInternalTargetY = mObjectiveTargetY;
                                    mInternalTargetZ = mObjectiveTargetZ;
                                }
                                if (!bCanAllocate)
                                {
                                    gCarManager_6C->mSpawnBlock = 2;
                                }
                            }
                        }
                    }
                    else
                    {
                        pCar = gTaxi_4->GetTaxiNear(mCam.x, mCam.y);
                        if (!pCar)
                        {
                            xpos = mCam.x.ToUInt8();
                            ypos = mCam.y.ToUInt8();
                            zpos = mCam.z.ToUInt8();

                            u8 bFound = 0;
                            if (gCarManager_6C->CanAllocateCarOfKind(car_kind::recycled_1))
                            {
                                bFound = gPathFinder->FindNearbyTileMatchingSlopeType(ROAD, &xpos, &ypos, &zpos, true);
                            }
                            if (bFound)
                            {
                                pCar = gCarManager_6C->SpawnCarAtRoadDirection(Ped::GetValueByIdParity(), xpos, ypos, zpos);
                                if (pCar)
                                {
                                    pCar->IncrementCarStats(car_kind::recycled_1);
                                }
                            }
                        }

                        if (pCar)
                        {
                            pCar->AllocateAI();
                            pCar->mAI->SetCar(pCar);
                            pCar->SpawnDriverPed();
                            pCar->SetDriverPedType(ped_type::dummy_with_occupation_6);
                            pCar->InitCarAIControl();
                            pCar->StartEngine();
                            pCar->HaltAIDriving();
                        }
                    }

                    if (pCar && !pCar->mAbandoned && !pCar->IsDespawning())
                    {
                        Ped::SetObjective2(objectives_enum::enter_car_as_driver_35);
                        if (mBf21C.bSkipCarSearch)
                        {
                            mEnterCarAsPassenger = 1;
                            pCar->HaltAIDriving();
                            mTargetCarDoor = 2;
                        }
                        else
                        {
                            mEnterCarAsPassenger = 0;
                        }
                        mTargetToEnter = pCar;
                        mEscapeCar = pCar;
                        mGameObject->Set_F84(pCar);
                    }
                    else if (gDistanceToTarget < kFpFour)
                    {
                        Ped::SetObjective2(objectives_enum::goto_area_on_foot_12, 128);
                        mInternalTargetX = mObjectiveTargetX;
                        mInternalTargetY = mObjectiveTargetY;
                        mInternalTargetZ = mObjectiveTargetZ;
                    }
                    break;
                }
            }
        }
        else if (mBf21C.bSkipCarSearch && !mCar->mDriver)
        {
            mCar->mAbandoned = true;
            mCar->SetDriverPedType(ped_type::dummy_3);
            Ped::SetObjective2(objectives_enum::leave_car_36);
            mObjectiveTimer = 0;
            mTargetToEnter = mCar;
            mEscapeCar = NULL;
        }
        else if (mInternalObjective == objectives_enum::goto_area_in_car_14)
        {
            if (mCar->mDamageLevel >= 4)
            {
                mCar->mAbandoned = true;
                mCar->SetDriverPedType(ped_type::dummy_3);
                Ped::SetObjective2(objectives_enum::leave_car_36);
                mObjectiveTimer = 0;
                mTargetToEnter = mCar;
                mEscapeCar = NULL;
            }
            else if (mCar->GetVelocity() == kFpZero_678660)
            {
                if (++mObjectiveTimer > 500)
                {
                    mCar->mAbandoned = true;
                    mCar->SetDriverPedType(ped_type::dummy_3);
                    Ped::SetObjective2(objectives_enum::leave_car_36);
                    mObjectiveTimer = 0;
                    mTargetToEnter = mCar;
                    mEscapeCar = NULL;
                }
            }
            else if (mCar->GetVelocity() > kFpZero_678660)
            {
                mObjectiveTimer = 0;
            }
        }
        else
        {
            if (mBf21C.bSkipCarSearch)
            {
                mCar->SetDriverPedType(ped_type::dummy_with_occupation_6);
            }
            bool bGroupReady = !IsGroupLeader() || mPedGroup->IsAllMembersInSomeCar();
            if (bGroupReady)
            {
                if (mObjective == objectives_enum::kill_char_any_means_19)
                {
                    Ped::SetObjective2(objectives_enum::chase_char_in_car_52);
                    SetInternalTargetPed(mObjectiveTargetPed);
                }
                else
                {
                    Ped::SetObjective2(objectives_enum::goto_area_in_car_14);

                    target_x = mObjectiveTargetX.ToUInt8();
                    target_y = mObjectiveTargetY.ToUInt8();
                    target_z = mObjectiveTargetZ.ToUInt8();

                    mObjectiveTimer = 0;
                    if (!gPathFinder->FindNearbyTileMatchingSlopeType(ROAD, &target_x, &target_y, &target_z, false))
                    {
                        mCar->mAbandoned = true;
                        mCar->SetDriverPedType(ped_type::dummy_3);
                        Ped::SetObjective2(objectives_enum::leave_car_36);
                        mObjectiveTimer = 0;
                        mTargetToEnter = mCar;
                        mEscapeCar = NULL;
                    }
                }
            }
            else
            {
                mCar->HaltAIDriving();
            }
        }
    }
    else if (!mBf21C.bPanicking)
    {
        if (mObjective == objectives_enum::kill_char_any_means_19)
        {
            if (mCar)
            {
                if (mCar->GetVelocity() < kFpPoint02 && !mBf21C.bStayInCar)
                {
                    Ped::SetObjective2(objectives_enum::leave_car_36);
                    mObjectiveTimer = 0;
                    mTargetToEnter = mCar;
                    if (mCar->mpChaseTask)
                    {
                        gCarChaseTaskTable->FreeEntry(mCar->mpChaseTask);
                        mCar->mpChaseTask = NULL;
                    }
                    mCar->SetDriverPedType(ped_type::dummy_3);
                    mCar->mLastSeenTimer = 0;
                }
            }
            else if (mObjectiveTargetPed->mCar)
            {
                if (gDistanceToTarget > kFpFour && mEscapeCar)
                {
                    Ped::SetObjective2(objectives_enum::enter_car_as_driver_35);
                    mTargetToEnter = mEscapeCar;
                }
                else
                {
                    Ped::SetObjective2(objectives_enum::no_obj_0);
                }
            }
        }
        else if (gDistanceToTarget < kFpOneEighth_6784E8 && !mCar)
        {
            mObjectiveStatus = objective_status::passed_1;
            Ped::SetObjective2(objectives_enum::wait_on_foot_26);
        }
        else if (mCar)
        {
            if (mCar->GetVelocity() == kFpZero_678660)
            {
                if (mBf21C.bSkipCarSearch)
                {
                    mCar->SetDriverPedType(ped_type::dummy_3);
                }
                Ped::SetObjective2(objectives_enum::leave_car_36);
                mObjectiveTimer = 0;
                mTargetToEnter = mCar;
            }
        }
        else
        {
            Ped::SetObjective2(objectives_enum::goto_area_on_foot_12);
            mInternalTargetX = mObjectiveTargetX;
            mInternalTargetY = mObjectiveTargetY;
            mInternalTargetZ = mObjectiveTargetZ;
        }
    }
}

MATCH_FUNC(0x469bd0)
void Ped::UpdateGroupMemberFollow()
{
    if (mGameObject)
    {
        mPedGroup->UpdateMemberTightFollowState(mGroupIdx);
    }
}

MATCH_FUNC(0x469bf0)
void Ped::GuardSpot()
{
    bool bUpdateMovement = true;

    this->mBf21C.bHoldPosition = true;

    if (!gObjectiveMayAct)
    {
        bUpdateMovement = this->mGameObject->GetCharState() == Char_B4_state::Jumping_15 && !mBf21C.bPanicking;
    }

    if (mBf224.bWaitingOnTarget && (mBf21C.bPanicking || this->mWaitCounter > 200u))
    {
        this->mWaitCounter = 0;
        this->mBf224.bWaitingOnTarget = false;
    }

    if (bUpdateMovement)
    {
        if (this->mBf224.bWaitingOnTarget ||
            gDistanceToTarget <= kFpThreeSixteenths &&
                abs_sub_less_than_epislon(this->mCam.z, this->mObjectiveTargetZ))
        {
            if (this->mGameObject->GetCharState() != Char_B4_state::Jumping_15)
            {
                if (mBf224.bWaitingOnTarget)
                {
                    Ped::ChangeNextPedState1(ped_state_1::standing_still_7);
                    Ped::ChangeNextPedState2(ped_state_2::ped2_staying_14);
                    mGameObject->RegulateVelocityByRef_433970(kFpZero_678438);
                    ++this->mWaitCounter;
                    this->mTargetFacingAngle = this->mRotation;
                }
                else
                {
                    Ped::ChangeNextPedState1(ped_state_1::standing_still_7);
                    Ped::ChangeNextPedState2(ped_state_2::ped2_staying_14);
                    this->mTargetFacingAngle = this->mRotation;
                }
            }
            else
            {
                this->mBf224.bWaitingOnTarget = true;
            }
        }
        else
        {
            if (mGameObject->GetCharState() != Char_B4_state::Jumping_15)
            {
                mGameObject->SetVelocity(this->mWalkSpeed);
                UpdateMovementTowardsTarget(gDistanceToTarget, ped_move_target_type::objective_target_pos_4);
            }
        }
    }
}

MATCH_FUNC(0x469d60)
void Ped::GuardArea()
{
    if (gObjectiveMayAct)
    {
        if (gDistanceToTarget <= kFpThreeThirtySeconds && mCam.z.ToUInt8() == mObjectiveTargetZ.ToUInt8())
        {
            if (mGameObject->GetCharState() != Char_B4_state::Jumping_15)
            {
                Ped::ChangeNextPedState1(ped_state_1::standing_still_7);
                Ped::ChangeNextPedState2(ped_state_2::ped2_staying_14);
                mTargetFacingAngle = mRotation;
            }
        }
        else
        {
            mGameObject->SetVelocity(mWalkSpeed);
            Ped::UpdateMovementTowardsTarget(gDistanceToTarget, ped_move_target_type::objective_target_pos_4);
        }
    }
    else
    {
        if (mBf21C.bPanicking && gDistanceToTarget > mTargetDistanceLimit)
        {
            mBf21C.bHoldPosition = true;
        }
    }
}

MATCH_FUNC(0x469e10)
void Ped::FailWhenObjectiveTimerExpired()
{
    if (mObjectiveTimer == 0)
    {
        this->mObjectiveStatus = objective_status::failed_2;
    }
}

MATCH_FUNC(0x469e30)
void Ped::SetCarAiSpeedOne()
{
    if (mCar)
    {
        mCar->mAI->mMaxSpeed = kFpOne_678664;
    }
}

MATCH_FUNC(0x469e50)
void Ped::SetupCarFollowTargetPed()
{
    if (mCar)
    {
        if (!mCar->mpChaseTask)
        {
            mCar->mpChaseTask = gCarChaseTaskTable->AllocateEntry();
            mCar->mpChaseTask->mpDriver = this;
        }
        mCar->mpChaseTask->mTaskType = car_task_type::follow_target_ped_4;
        mCar->SetDriverPedType(ped_type::script_created_5);
        mCar->mpChaseTask->mpTargetPed = mObjectiveTargetPed;
        mCar->ClearAIHalted();
        mCar->mAI->mMaxSpeed = kFpThree_67866C;
        mCar->mpChaseTask->mCanSnapToTarget = true;
        if (mCar->mCarInfoIdx == car_model_enum::JEEP)
        {
            if (gDistanceToTarget < kFpTwo_678668)
            {
                mBf21C.bAttacking = true;
            }
            else
            {
                mBf21C.bAttacking = false;
            }
        }
    }
}

MATCH_FUNC(0x469f30)
void Ped::ProcessObjectiveFollowPedInCar()
{
    if (!mCar->mpChaseTask)
    {
        mCar->mpChaseTask = gCarChaseTaskTable->AllocateEntry();
        mCar->mpChaseTask->mpDriver = this;
    }
    mCar->mpChaseTask->mTaskType = car_task_type::follow_ped_2;
    mCar->SetDriverPedType(ped_type::script_created_5);
    mCar->mpChaseTask->mpTargetPed = mObjectiveTargetPed;
    mCar->ClearAIHalted();
    mCar->mAI->mMaxSpeed = kFpThree_67866C;
}

MATCH_FUNC(0x469fc0)
void Ped::WaitInCurrentCar()
{
    Car* pCar = this->mCar;
    if (pCar)
    {
        pCar->mBfA6.bAIHalted = true;
    }
    else
    {
        // He is out of the car
        this->mObjectiveStatus = objective_status::failed_2;
    }
}

// The fail tail is written out per branch (VC6 cross-jumps the copies back into one): each copy adds to the
// zero constant's regalloc priority, so it keeps 0 in ebx. bFound reaching another block gives `cmp %bl,%al`.
MATCH_FUNC(0x469fe0)
void Ped::CallPoliceCar()
{
    Car* pCar;
    if (!this->mTargetObjectiveCar)
    {
        u8 x = this->mCam.x.ToUInt8();
        u8 y = this->mCam.y.ToUInt8();
        u8 z = this->mCam.z.ToUInt8();
        gPathFinder->FindNearbyTileMatchingSlopeType(ROAD, &x, &y, &z, true);
        if (gCarManager_6C->CanAllocateCarOfKind(car_kind::call_police_car_10))
        {
            pCar = gCarManager_6C->SpawnCarAtRoadDirection(car_model_enum::COPCAR, x, y, z);
            u8 bFound = 0;
            if (pCar)
            {
                pCar->IncrementCarStats(car_kind::call_police_car_10);
                bFound = gPolice_7B8->SpawnCrewInCar(pCar);
            }
            if (bFound)
            {
                this->mPedState1 = ped_state_1::in_car_10;
                this->mPedState2 = ped_state_2::ped2_driving_10;
                this->mGameObject->mTargetCar = pCar;
                this->mEnterCarAsPassenger = 1;
                this->mTargetObjectiveCar = pCar;
                return;
            }
            this->mPedState1 = ped_state_1::walking_0;
            this->mPedState2 = ped_state_2::ped2_walking_0;
            SetObjective(objectives_enum::no_obj_0);
            SetObjective2(objectives_enum::no_obj_0);
        }
        else if (gCarManager_6C->CanAllocateCarOfKind(car_kind::police_6))
        {
            pCar = gCarManager_6C->SpawnCarAtRoadDirection(car_model_enum::COPCAR, x, y, z);
            u8 bFound = 0;
            if (pCar)
            {
                pCar->IncrementCarStats(car_kind::police_6);
                bFound = gPolice_7B8->SpawnCrewInCar(pCar);
            }
            if (bFound)
            {
                this->mPedState1 = ped_state_1::in_car_10;
                this->mPedState2 = ped_state_2::ped2_driving_10;
                this->mGameObject->mTargetCar = pCar;
                this->mEnterCarAsPassenger = 1;
                this->mTargetObjectiveCar = pCar;
                return;
            }
            this->mPedState1 = ped_state_1::walking_0;
            this->mPedState2 = ped_state_2::ped2_walking_0;
            SetObjective(objectives_enum::no_obj_0);
            SetObjective2(objectives_enum::no_obj_0);
        }
        else
        {
            this->mPedState1 = ped_state_1::walking_0;
            this->mPedState2 = ped_state_2::ped2_walking_0;
            SetObjective(objectives_enum::no_obj_0);
            SetObjective2(objectives_enum::no_obj_0);
        }
    }
    else if (!this->mObjectiveTimer)
    {
        this->mTargetCarDoor = 2;
        SetObjective2(objectives_enum::no_obj_0);
        SetObjective(objectives_enum::leave_car_and_stop_33);
        this->SetTargetObjectiveCar(this->mCar);
    }
}

MATCH_FUNC(0x46a1f0)
void Ped::PullDriverOutOfCar()
{
    if (!mObjectiveTargetPed->IsActive() ||
        mObjectiveTargetPed->GetPedState() == ped_state_1::dead_9)
    {
        mObjectiveStatus = objective_status::failed_2;
    }
    else
    {
        if (mBf21C.bPanicking && mObjectiveTargetPed == mInternalTargetPed)
        {
            mBf21C.bPanicking = false;
            Ped::SetObjective2(objectives_enum::no_obj_0);
            gObjectiveMayAct = 1;
        }
        else if (!gObjectiveMayAct)
        {
            return;
        }

        if (mInternalObjective)
        {
            if (mInternalObjective == objectives_enum::pull_driver_out_32)
            {
                if (mInternalObjectiveStatus == objective_status::passed_1)
                {
                    mObjectiveStatus = objective_status::passed_1;
                }
                else if (mInternalObjectiveStatus == objective_status::failed_2)
                {
                    mObjectiveStatus = objective_status::failed_2;
                }
            }
        }
        else
        {
            Ped::SetObjective2(objectives_enum::pull_driver_out_32);
            mInternalTargetPed = mObjectiveTargetPed;
        }
    }
}

MATCH_FUNC(0x46a290)
void Ped::FollowCarInCurrCar()
{
    if (!mTargetObjectiveCar->mDriver || mGameObject)
    {
        // If the car it's following doesnt have a driver OR this ped (which will follow) is on foot (not in a car)
        mObjectiveStatus = objective_status::failed_2;
    }
    else
    {
        if (!mCar->mpChaseTask)
        {
            mCar->mpChaseTask = gCarChaseTaskTable->AllocateEntry();
            mCar->mpChaseTask->mpDriver = this;
        }
        mCar->mpChaseTask->mTaskType = car_task_type::follow_ped_2;
        mCar->SetDriverPedType(ped_type::script_created_5);
        mCar->mpChaseTask->mpTargetPed = mTargetObjectiveCar->mDriver;
        mCar->ClearAIHalted();
        mCar->mAI->mMaxSpeed = kFpThree_67866C;
    }
}

MATCH_FUNC(0x46a350)
void Ped::FollowCarOnFootWithOffset()
{

    if (mTargetObjectiveCar->mDespawnStatus == car_despawn_status::despawning_5)
    {
        this->mObjectiveStatus = objective_status::failed_2;
    }
    else
    {
        Ang16 ang = mTargetObjectiveCar->mCarSprite->mRotation;
        ang -= mFollowCarOffsetAngle;
        Fix16 sin_v = Ang16::sine(ang) * this->mFollowCarOffsetDistance;
        Fix16 cos_v = Ang16::cosine(ang) * this->mFollowCarOffsetDistance;

        if (gObjectiveMayAct)
        {
            if (mInternalObjective)
            {
                if (mInternalObjective == objectives_enum::follow_car_on_foot_with_offset_56)
                {
                    this->mInternalTargetX = sin_v + mTargetObjectiveCar->mCarSprite->mXy.x;
                    this->mInternalTargetY = cos_v + mTargetObjectiveCar->mCarSprite->mXy.y;
                    this->mInternalTargetZ = mTargetObjectiveCar->mCarSprite->mZpos;
                    if (this->mInternalObjectiveStatus == objective_status::passed_1)
                    {
                        if (mGameObject->mVelocity < kFpZero_678660)
                        {
                            mGameObject->mVelocity += kFpOne256th_678620;
                        }
                        else if (mGameObject->mVelocity > kFpZero_678660)
                        {
                            mGameObject->mVelocity -= kFpOne256th_678620;
                        }
                        this->mInternalObjectiveStatus = objective_status::not_finished_0;
                    }
                }
            }
            else
            {
                Ped::SetObjective2(objectives_enum::follow_car_on_foot_with_offset_56);
                this->mInternalTargetX = sin_v + mTargetObjectiveCar->mCarSprite->mXy.x;
                this->mInternalTargetY = cos_v + mTargetObjectiveCar->mCarSprite->mXy.y;
                this->mInternalTargetZ = mTargetObjectiveCar->mCarSprite->mZpos;
            }
        }
    }
}

MATCH_FUNC(0x46a530)
void Ped::FireAtObject()
{
    AttachedSprite* pCannon = mCar->mAttachments.GetSpriteForModel(objects::tank_cannon_148);
    Sprite* pCannonSprite = pCannon->mpSprite;
    Fix16 x_v = pCannonSprite->mXy.x;
    Fix16 y_v = pCannonSprite->mXy.y;
    Ang16 aim_angle;
    aim_angle = Fix16::atan2_fixed(mObjectiveTargetObject->mSprite->mXy.y - y_v,
                                   mObjectiveTargetObject->mSprite->mXy.x - x_v);

    mBf21C.bUseCarWeapon = true;

    if (mCar->RotateRoofObjectTowardTarget(aim_angle))
    {
        mBf21C.bAttacking = true;
    }
    else
    {
        mBf21C.bAttacking = false;
    }
}

MATCH_FUNC(0x46a5e0)
void Ped::FireAtPlayer()
{
    if (!mCar)
    {
        this->mObjectiveStatus = objective_status::not_finished_0;
        return;
    }

    Ped* pPlayerPed = gGame_0x40->mLocalPlayer->mPlayerPed;
    if (!pPlayerPed->IsInvisibleOnFoot_433DA0())
    {
        Sprite* pCannonSprite = mCar->mAttachments.GetSpriteForModel(objects::tank_cannon_148)->mpSprite;
        Fix16 x = pCannonSprite->mXy.x;
        Fix16 y = pCannonSprite->mXy.y;
        Ang16 aim_angle;
        aim_angle = Fix16::atan2_fixed(pPlayerPed->get_cam_y() - y, pPlayerPed->get_cam_x() - x);

        this->mBf21C.bUseCarWeapon = true;

        if (mCar->RotateRoofObjectTowardTarget(aim_angle))
        {
            if (!this->mCar->mLastSeenTimer)
            {
                this->mBf21C.bAttacking = true;
            }
            return;
        }
    }
    this->mBf21C.bAttacking = false;
}


MATCH_FUNC(0x46a6d0)
void Ped::AimVehicleTurretStateMachine()
{
    if (mTargetObjectiveCar->IsDespawning() || mCar == NULL)
    {
        mObjectiveStatus = objective_status::failed_2;
    }
    else
    {
        AttachedSprite* pAttached = mCar->mAttachments.GetSpriteForModel(objects::water_cannon_114);
        mBf21C.bUseCarWeapon = true;

        Fix16 x = pAttached->mpSprite->mXy.x;
        Fix16 y = pAttached->mpSprite->mXy.y;
        Ang16 angle;
        angle = Fix16::atan2_fixed(mTargetObjectiveCar->mCarSprite->mXy.y - y,
                                   mTargetObjectiveCar->mCarSprite->mXy.x - x);
        if (mCar->RotateRoofObjectTowardTarget(angle))
        {
            mBf21C.bAttacking = true;

            if (mObjectiveTimer == 9999)
            {
                mObjectiveTimer = 50;
            }
        }
        else
        {
            mBf21C.bAttacking = false;
        }

        if (mObjectiveTimer == 0)
        {
            mObjectiveStatus = objective_status::passed_1;
        }
    }
}

MATCH_FUNC(0x46a7c0)
void Ped::DestroyTargetObject()
{
    if (gObjectiveMayAct && !mBf21C.bPanicking)
    {
        switch (mInternalObjective)
        {
            case objectives_enum::no_obj_0:
                Ped::SetObjective2(objectives_enum::destroy_object_58);
                mInternalTargetObject = mObjectiveTargetObject;
                return;
            case objectives_enum::flee_on_foot_till_safe_1:
                if (mInternalObjectiveStatus != objective_status::passed_1)
                {
                    return;
                }
                break;
            case objectives_enum::destroy_object_58:
                if (mInternalObjectiveStatus == objective_status::passed_1)
                {
                    mObjectiveStatus = objective_status::passed_1;
                }
                if (mInternalObjectiveStatus == objective_status::failed_2)
                {
                    mObjectiveStatus = objective_status::failed_2;
                }
                return;
            default:
                return;
        }
        Ped::SetObjective2(objectives_enum::destroy_object_58);
        mInternalTargetObject = mObjectiveTargetObject;
    }
}

MATCH_FUNC(0x46a850)
void Ped::DestroyTargetCar()
{
    if (mTargetObjectiveCar->IsMaxDamage())
    {
        mObjectiveStatus = objective_status::passed_1;
    }
    if (gObjectiveMayAct && !mBf21C.bPanicking)
    {
        switch (mInternalObjective)
        {
            case objectives_enum::no_obj_0:
                Ped::SetObjective2(objectives_enum::destroy_car_59);
                mTargetToEnter = mTargetObjectiveCar;
                return;
            case objectives_enum::flee_on_foot_till_safe_1:
                if (mInternalObjectiveStatus != objective_status::passed_1)
                {
                    return;
                }
                break;
            case objectives_enum::destroy_car_59:
                if (mInternalObjectiveStatus == objective_status::passed_1)
                {
                    mObjectiveStatus = objective_status::passed_1;
                }
                if (mInternalObjectiveStatus == objective_status::failed_2)
                {
                    mObjectiveStatus = objective_status::failed_2;
                }
                return;
            default:
                return;
        }
        Ped::SetObjective2(objectives_enum::destroy_car_59);
        mTargetToEnter = mTargetObjectiveCar;
    }
}

MATCH_FUNC(0x46a8f0)
void Ped::ProcessInternalObjectiveFleeOnFootTillSafe()
{
    if (gDistanceToTarget > kFpSix)
    {
        Char* pChar = mGameObject;
        if (pChar)
        {
            if (mObjective || pChar->Get_F44() == 2)
            {
                Ped::ChangeNextPedState1(ped_state_1::walking_0);
                Ped::ChangeNextPedState2(ped_state_2::ped2_walking_0);
                mInternalObjectiveStatus = objective_status::passed_1;
            }
            else
            {
                pChar->SetVelocity(get_max_speed_1F0());
            }
        }
        else
        {
            mBf21C.bAttacking = false;
        }
    }
    else
    {
        mGameObject->SetVelocity(get_max_speed_1F0());
    }
}

MATCH_FUNC(0x46a9c0)
void Ped::FleeFromPedTillSafe()
{
    mInternalTargetPed->ClearAttacker();
    if (mInternalTargetPed->isDead() || !mInternalTargetPed->IsActive())
    {
        Ped::ChangeNextPedState1(ped_state_1::walking_0);
        Ped::ChangeNextPedState2(ped_state_2::ped2_walking_0);
        mInternalTargetPed = NULL;
        mInternalObjectiveStatus = objective_status::passed_1;
    }
    else if (mPedState1 != ped_state_1::immobilized_8)
    {
        if (gDistanceToTarget > kFpSix)
        {
            if (mGameObject->Get_F44() == 2)
            {
                Ped::ChangeNextPedState1(ped_state_1::walking_0);
                Ped::ChangeNextPedState2(ped_state_2::ped2_walking_0);
                mInternalTargetPed = NULL;
                mInternalObjectiveStatus = objective_status::passed_1;
            }
        }
        else
        {
            Ped::ChangeNextPedState1(ped_state_1::flee_or_running_1);
            Ped::ChangeNextPedState2(ped_state_2::running_away_3);
            mGameObject->UseRunOrJumpSpeed();
        }
    }
}

MATCH_FUNC(0x46aae0)
void Ped::FleeFromPedAlways()
{
    if (mInternalTargetPed->isDead() || !mInternalTargetPed->IsActive())
    {
        Ped::ChangeNextPedState1(ped_state_1::walking_0);
        Ped::ChangeNextPedState2(ped_state_2::ped2_walking_0);
        mInternalTargetPed = NULL;
        mInternalObjectiveStatus = objective_status::passed_1;
        mBf21C.bPanicking = false;
    }
    else
    {
        mInternalTargetPed->ClearAttacker();
        mGameObject->UseRunOrJumpSpeed();
    }
}

MATCH_FUNC(0x46ab50)
void Ped::CheckFollowTargetPedStillValid()
{
    if (mInternalTargetPed->isDead() || !mInternalTargetPed->IsActive())
    {
        Ped::ChangeNextPedState1(ped_state_1::walking_0);
        Ped::ChangeNextPedState2(ped_state_2::ped2_walking_0);
        mInternalTargetPed = NULL;
        mInternalObjectiveStatus = objective_status::passed_1;
        Ped::SetObjective2(objectives_enum::no_obj_0);
        mBf21C.bPanicking = false;
    }
    else
    {
        if (mPedState1 != ped_state_1::immobilized_8)
        {
            mBf21C.bAttacking = false;
            mInternalTargetPed->ClearAttacker();
            if (gDistanceToTarget < kFpOne_6785EC)
            {
                Ped::ChangeNextPedState1(ped_state_1::walking_0);
                Ped::ChangeNextPedState2(ped_state_2::ped2_walking_0);
                mInternalTargetPed = NULL;
                mInternalObjectiveStatus = objective_status::passed_1;
                Ped::SetObjective2(objectives_enum::no_obj_0);
                mBf21C.bPanicking = false;
            }
            else
            {
                Ped::ChangeNextPedState1(ped_state_1::flee_or_running_1);
                Ped::ChangeNextPedState2(ped_state_2::running_to_target_2);
                Ped::UpdateMovementTowardsTarget(gDistanceToTarget, ped_move_target_type::internal_target_ped_0);
                mGameObject->UseRunOrJumpSpeed();
            }
        }
    }
}

// https://decomp.me/scratch/ahbj8
MATCH_FUNC(0x46ac20)
void Ped::FollowTargetStateMachine()
{
    bool bTargetOrLeaderMoving = false;
    if (mInternalTargetPed->isDead() || !mInternalTargetPed->IsActive())
    {
        mInternalObjectiveStatus = objective_status::not_finished_0;
        mBf21C.bPanicking = false;
        return;
    }
    if (mInternalTargetPed->mGameObject)
    {
        if (mInternalTargetPed->mGameObject->GetCharState() == Char_B4_state::Jumping_15)
        {
            bTargetOrLeaderMoving = true;
        }
    }
    if (mInternalTargetPed->GetPedVelocity() != kFpZero_678660)
    {
        bTargetOrLeaderMoving = true;
    }
    if (mPedGroup)
    {
        if (mPedGroup->mPedLeader)
        {
            if (mPedGroup->mPedLeader->GetPedVelocity() != kFpZero_678660)
            {
                bTargetOrLeaderMoving = true;
            }
        }
    }

    if (mInternalTargetPed->mInternalObjective == objectives_enum::walk_to_car_door_18 || bTargetOrLeaderMoving)
    {
        mBf224.bWaitingOnTarget = false;
    }

    if (mBf224.bWaitingOnTarget && mInternalTargetPed->GetPedVelocity() != kFpZero_678660)
    {
        mBf224.bWaitingOnTarget = false;
    }

    if (mPedState1 != ped_state_1::immobilized_8)
    {
        if (gDistanceToTarget < kFpTwo_678658)
        {
            mBf21C.bPanicking = false;
        }

        if (gDistanceToTarget >= kFpThreeSixteenths && !mBf224.bWaitingOnTarget)
        {
            if (gDistanceToTarget > kFpHalf)
            {
                mGameObject->IncreaseSpeedIfAllowed();
            }
            else if (gDistanceToTarget > kFpThreeEighths)
            {
                mGameObject->RegulateVelocityByRef_433970(kFpThreeSixtyFourths);
            }
            else if (gDistanceToTarget > kFpFiveSixteenths)
            {
                mGameObject->RegulateVelocityByRef_433970(kFpOneThirtySecond);
            }
            else
            {
                mGameObject->RegulateVelocityByRef_433970(kFpOne64th);
            }

            if (sKeepPaceWithTarget)
            {
                if (gDistanceToTarget < kFpHalf)
                {
                    if (mInternalTargetPed->GetPedVelocity() == kFpZero_678660)
                    {
                        mGameObject->RegulateVelocityByRef_433970(kFpOne64th);
                    }
                    else
                    {
                        mGameObject->RegulateVelocityByRef_433970(kFpOne64th + mInternalTargetPed->GetPedVelocity());
                    }
                }
                else
                {
                    mGameObject->RegulateVelocityByRef_433970(kFpFiveSixtyFourths);
                }
            }
            Ped::UpdateMovementTowardsTarget(gDistanceToTarget, ped_move_target_type::internal_target_ped_0);
        }
        else
        {
            if (mGameObject->GetCharState() != Char_B4_state::Jumping_15)
            {
                if (sKeepPaceWithTarget)
                {
                    if (mBf224.bWaitingOnTarget)
                    {
                        Ped::ChangeNextPedState1(ped_state_1::standing_still_7);
                        Ped::ChangeNextPedState2(ped_state_2::ped2_staying_14);
                        mGameObject->RegulateVelocityByRef_433970(kFpZero_678438);
                    }
                    else if (gDistanceToTarget > kFpOneThirtySecond_6784CC)
                    {
                        if (mInternalTargetPed->GetPedVelocity() == kFpZero_678660)
                        {
                            mGameObject->RegulateVelocityByRef_433970(kFpOne64th);
                        }
                        else if (gDistanceToTarget > kFpThreeSixteenths)
                        {
                            mGameObject->RegulateVelocityByRef_433970(kFpFiveSixtyFourths);
                        }
                        else
                        {
                            mGameObject->RegulateVelocityByRef_433970(mInternalTargetPed->GetPedVelocity());
                        }
                    }
                    else
                    {
                        if (mInternalTargetPed->GetPedVelocity() == kFpZero_678660)
                        {
                            Ped::ChangeNextPedState1(ped_state_1::standing_still_7);
                            Ped::ChangeNextPedState2(ped_state_2::ped2_staying_14);
                            mGameObject->RegulateVelocityByRef_433970(kFpZero_678438);
                        }
                        else
                        {
                            mGameObject->RegulateVelocityByRef_433970(mInternalTargetPed->GetPedVelocity());
                        }
                    }
                }
                else
                {
                    Ped::ChangeNextPedState1(ped_state_1::standing_still_7);
                    Ped::ChangeNextPedState2(ped_state_2::ped2_staying_14);
                    mGameObject->RegulateVelocityByRef_433970(kFpZero_678438);
                }
            }
            else
            {
                mBf224.bWaitingOnTarget = true;
            }
        }
    }
}

MATCH_FUNC(0x46b170)
void Ped::StayCloseToTargetPed()
{
    if (!mInternalTargetPed->isDead() && mInternalTargetPed->IsActive())
    {
        if (mPedState1 != ped_state_1::immobilized_8)
        {
            if (gDistanceToTarget < kFpOneEighth_6784E8)
            {
                if (mGameObject->GetCharState() != Char_B4_state::Jumping_15)
                {
                    Ped::ChangeNextPedState1(ped_state_1::standing_still_7);
                    Ped::ChangeNextPedState2(ped_state_2::ped2_staying_14);

                    mGameObject->RegulateVelocityByRef_433970(kFpZero_678438);
                }
            }
            else
            {
                if (gDistanceToTarget > kFpHalf)
                {
                    mGameObject->IncreaseSpeedIfAllowed();
                }
                else if (gDistanceToTarget > kFpThreeEighths)
                {
                    mGameObject->RegulateVelocityByRef_433970(kFpThreeSixtyFourths);
                }
                else if (gDistanceToTarget > kFpFiveSixteenths)
                {
                    mGameObject->RegulateVelocityByRef_433970(kFpOneThirtySecond);
                }
                else
                {
                    mGameObject->RegulateVelocityByRef_433970(kFpOne64th);
                }

                if (mGameObject->GetCharState() == Char_B4_state::Avoiding_Ped_10)
                {
                    mGameObject->SetVelocityByRef_433920(kFpZero_678438);
                }
                Ped::UpdateMovementTowardsTarget(gDistanceToTarget, ped_move_target_type::internal_target_ped_0);
            }
        }
    }
    else
    {
        mInternalObjectiveStatus = objective_status::passed_1;
    }
}

// https://decomp.me/scratch/1MwH3
MATCH_FUNC(0x46b2f0)
void Ped::PullDriverOutOfCarStateMachine()
{
    Car* pCar = mInternalTargetPed->mCar;
    mTargetCarDoor = 0;
    if (mInternalTargetPed->mGameObject)
    {
        mInternalObjectiveStatus = objective_status::failed_2;
        Ped::ChangeNextPedState1(ped_state_1::walking_0);
        Ped::ChangeNextPedState2(ped_state_2::ped2_walking_0);
        return;
    }
    if (pCar->IsTrainModel())
    {
        mInternalObjectiveStatus = objective_status::failed_2;
        Ped::ChangeNextPedState1(ped_state_1::walking_0);
        Ped::ChangeNextPedState2(ped_state_2::ped2_walking_0);
        return;
    }

    for (u8 door_idx = 0; door_idx < static_cast<u8>(pCar->GetDoorCount()); door_idx++)
    {
        if (pCar->IsDoorAccessible(door_idx))
        {
            mTargetCarDoor = door_idx;
            pCar->GetDoorWorldPosition(door_idx, &mMoveTargetX, &mMoveTargetY);
            mMoveTargetZ = mInternalTargetPed->mCam.z;
            mGameObject->Set_F84(pCar);

            Fix16 d_x = mMoveTargetX - mCam.x;
            Fix16 d_y = mMoveTargetY - mCam.y;
            Fix16 d_x_abs = Fix16::Abs(d_x);
            Fix16 d_y_abs = Fix16::Abs(d_y);
            gDistanceToTarget = Fix16::Max(d_x_abs, d_y_abs);

            if (mInternalTargetPed->mCar->mCarInfoIdx == car_model_enum::BUS && mPedState2 == ped_state_2::carjack_opening_door_8)
            {
                gDistanceToTarget = kFpOne64th_6784C4;
            }
            Fix16 max_speed;
            if (!Ped::IsLawEnforcement())
            {
                max_speed = kFpPoint2;
            }
            else
            {
                max_speed = kFpPoint1;
            }
            if (mPedState2 != ped_state_2::carjack_pulling_out_9 &&
                (gDistanceToTarget >= kFpThreeSixteenths ||
                 !abs_sub_less_than_epislon(mCam.z, mInternalTargetPed->mCam.z) ||
                 mPedState1 == ped_state_1::immobilized_8))
            {
                mGameObject->SetVelocityByRef_433920(kFpOneSixteenth_678448);
                Ped::UpdateMovementTowardsTarget(gDistanceToTarget, ped_move_target_type::internal_target_ped_0);
            }
            else
            {
                switch (mPedState2)
                {
                    case ped_state_2::carjack_opening_door_8:
                        if (pCar->GetVelocity() > max_speed)
                        {
                            mInternalObjectiveStatus = objective_status::failed_2;
                            Ped::ChangeNextPedState1(ped_state_1::walking_0);
                            Ped::ChangeNextPedState2(ped_state_2::ped2_walking_0);
                        }
                        else
                        {
                            mBf21C.bLeftVehicle = true;
                            if (mGameObject->mAnimationFrame == 4)
                            {
                                Ped::ChangeNextPedState2(ped_state_2::carjack_pulling_out_9);
                                mGameObject->mAnimationFrame = 9;
                            }
                            mGameObject->SetCharState(Char_B4_state::Interacting_Car_Door_36);
                            mGameObject->SetVelocityByRef_433920(kFpZero_678438);
                        }
                        break;

                    case ped_state_2::carjack_pulling_out_9:
                        if (mGameObject->mAnimationFrame == 13)
                        {
                            mGameObject->mAnimationFrame = 4;
                            Car_Door* pDoor = pCar->GetDoor(mTargetCarDoor);
                            pDoor->Close();
                            pDoor->set_ped(NULL);
                            Ped::ChangeNextPedState2(ped_state_2::ped2_staying_14);
                        }
                        else
                        {
                            mBf21C.bLeftVehicle = true;
                        }
                        mGameObject->SetCharState(Char_B4_state::Interacting_Car_Door_36);
                        mGameObject->SetVelocityByRef_433920(kFpZero_678438);
                        break;

                    default:
                        if (pCar->GetVelocity() > max_speed)
                        {
                            mInternalObjectiveStatus = objective_status::failed_2;
                            Ped::ChangeNextPedState1(ped_state_1::walking_0);
                            Ped::ChangeNextPedState2(ped_state_2::ped2_walking_0);
                            return;
                        }
                        else
                        {
                            mBf21C.bLeftVehicle = true;
                            mGameObject->mAnimationState = Char_Anim_state::Carjacking_9;
                            mGameObject->mAnimationFrame = 0;
                            Ped::ChangeNextPedState1(ped_state_1::standing_still_7);
                            Ped::ChangeNextPedState2(ped_state_2::carjack_opening_door_8);
                            mGameObject->SetCharState(Char_B4_state::Interacting_Car_Door_36);
                            mGameObject->SetVelocityByRef_433920(kFpZero_678438);
                        }
                        break;
                }
            }
            return;
        }
    }

    Ped::ChangeNextPedState1(ped_state_1::walking_0);
    Ped::ChangeNextPedState2(ped_state_2::ped2_walking_0);
    mInternalObjectiveStatus = objective_status::failed_2;
}

// https://decomp.me/scratch/aE2Ac
// Logic follows the 10.5 asm. The mugging and criminal checks of the hurt block are nested per player
// test, so VC6 keeps the original's separate copies.
MATCH_FUNC(0x46b670)
void Ped::MeleeAttackStateMachine()
{
    if (mInternalTargetPed->GetPedState() == ped_state_1::dead_9 || !mInternalTargetPed->IsActive())
    {
        if (mInternalObjective != objectives_enum::punch_char_23)
        {
            mInternalObjectiveStatus = objective_status::passed_1;
            return;
        }
        mInternalObjectiveStatus = objective_status::failed_2;
        return;
    }

    if (mPedState1 == ped_state_1::immobilized_8 ||
        !abs_sub_less_than_epislon(mCam.z, mInternalTargetPed->mCam.z))
    {
        Ped::UpdateMovementTowardsTarget(gDistanceToTarget, ped_move_target_type::internal_target_ped_0);
        return;
    }

    mInternalTargetPed->SetAttacker(this);

    if (mInternalTargetPed->mCar)
    {
        Ped::PullDriverOutOfCarStateMachine();
        return;
    }

    gDistanceToTarget =
        Fix16::MaxAbsDistance(mCam.x, mCam.y, mInternalTargetPed->get_cam_x(), mInternalTargetPed->get_cam_y());

    if (gDistanceToTarget <= kFpQuarter)
    {
        // Ped is close
        mBf21C.bNoWeapon = true;
        mBf21C.bAttacking = true;
        if (mPedState2 == ped_state_2::carjack_pulling_out_9)
        {
            Sprite* pNearestSprt = gSpriteGrid_1->FindCollidingSpritePreferType(mGameObject->mSpritePtr,
                                                                                sprite_types_enum::car_2);
            if (pNearestSprt && pNearestSprt->GetType() == sprite_types_enum::car_2)
            {
                mPedState1 = ped_state_1::immobilized_8;
                mPedState2 = ped_state_2::ped2_at_car_door_17;
                mGameObject->mAnimationFrame = 3;
            }
            else
            {
                mPedState1 = ped_state_1::standing_still_7;
                mPedState2 = ped_state_2::ped2_staying_14;
                mGameObject->mAnimationState = Char_Anim_state::Standing_2;
                mGameObject->mAnimationFrame = 0;
            }
            return;
        }
        mGameObject->SetVelocity(mInternalTargetPed->GetPedVelocity());
        if (mGameObject->GetCharState() != Char_B4_state::Jumping_15 &&
            mGameObject->get_velocity() == kFpZero_678660)
        {
            Ped::ChangeNextPedState1(ped_state_1::standing_still_7);
            Ped::ChangeNextPedState2(ped_state_2::ped2_staying_14);
        }
        ++mInternalTargetPed->mHitCount;

        if (mInternalObjective != objectives_enum::punch_char_23 &&
            mThreatReaction != threat_reaction_enum::react_as_emergency_1)
        {
            if (mGameObject->mAnimationFrame == 0)
            {
                if (mInternalTargetPed->PedTypeIs(ped_type::player_2))
                {
                    if (mOccupation == ped_ocupation_enum::mugger)
                    {
                        mInternalTargetPed->mPlayer->AddScore(-10);
                        ++mMugCount;
                        if (mMugCount > 9)
                        {
                            mInternalObjectiveStatus = objective_status::passed_1;
                        }
                    }
                    else if (mInternalTargetPed->mOccupation != ped_ocupation_enum::criminal_type_1)
                    {
                        mInternalTargetPed->TakeDamage(10);
                    }
                }
                else if (mInternalTargetPed->mOccupation != ped_ocupation_enum::criminal_type_1)
                {
                    mInternalTargetPed->TakeDamage(10);
                }
            }
            return;
        }

        if (mInternalTargetPed->get_health() >= 20)
        {
            if (mGameObject->mAnimationFrame < 1 || mInternalTargetPed->GetPedState2() == ped_state_2::lying_on_floor_22)
            {
                mInternalTargetPed->mKillerId = mId;
                mInternalTargetPed->mDeathCause = ped_death_cause::punched_10;
                mInternalTargetPed->mKillerIdTimer = 50;

                if (mInternalTargetPed->PedTypeIs(ped_type::player_2))
                {
                    if (mOccupation == ped_ocupation_enum::mugger)
                    {
                        mInternalTargetPed->mPlayer->AddScore(-10);
                        ++mMugCount;
                        if (mMugCount > 9)
                        {
                            mInternalObjectiveStatus = objective_status::passed_1;
                        }
                    }
                    else
                    {
                        mInternalTargetPed->TriggerVoiceEventRateLimited();
                        if (mInternalTargetPed->mOccupation != ped_ocupation_enum::criminal_type_1)
                        {
                            mInternalTargetPed->TakeDamage(10);
                        }
                    }
                }
                else
                {
                    mInternalTargetPed->TriggerVoiceEventRateLimited();
                    if (mInternalTargetPed->mOccupation != ped_ocupation_enum::criminal_type_1)
                    {
                        mInternalTargetPed->TakeDamage(10);
                    }
                }
            }
        }
        else
        {
            mLastCharPunched = mInternalTargetPed;
            if (mInternalTargetPed != mObjectiveTargetPed)
            {
                mInternalTargetPed->ChangeNextPedState1(ped_state_1::immobilized_8);
                mInternalTargetPed->ChangeNextPedState2(ped_state_2::lying_on_floor_22);
                mInternalTargetPed->SetStateInitPending();
                mInternalObjectiveStatus = objective_status::passed_1;
                mAttacker = NULL;
                mHitCount = 0;
                mBf21C.bPanicking = false;

                if (mThreatReaction == threat_reaction_enum::react_as_emergency_1 && mInternalTargetPed->PedTypeIs(ped_type::player_2))
                {
                    if (bStartNetworkGame)
                    {
                        mInternalTargetPed->Kill();
                    }
                    else
                    {
                        mInternalTargetPed->mBf21C.bBusted = true;
                        SetVoiceEventIfNotOnFire(18);
                    }
                }
            }
            else if (mObjective != objectives_enum::punch_char_23 &&
                     mThreatReaction != threat_reaction_enum::react_as_emergency_1)
            {
                mInternalTargetPed->mKillerId = mId;
                mInternalTargetPed->mDeathCause = ped_death_cause::punched_10;
                mInternalTargetPed->mKillerIdTimer = 50;

                if (mInternalTargetPed->PedTypeIs(ped_type::player_2) && mOccupation == ped_ocupation_enum::mugger)
                {
                    mInternalTargetPed->mPlayer->AddScore(-10);
                    ++mMugCount;
                    if (mMugCount > 9)
                    {
                        mInternalObjectiveStatus = objective_status::passed_1;
                    }
                }
                else if (mInternalTargetPed->mOccupation != ped_ocupation_enum::criminal_type_1)
                {
                    mInternalTargetPed->TakeDamage(10);
                }
            }
            else
            {
                mInternalTargetPed->ChangeNextPedState1(ped_state_1::immobilized_8);
                mInternalTargetPed->ChangeNextPedState2(ped_state_2::lying_on_floor_22);
                mInternalTargetPed->SetStateInitPending();
                mInternalObjectiveStatus = objective_status::passed_1;
                mAttacker = NULL;
                mBf21C.bPanicking = false;
                mHitCount = 0;

                if (mThreatReaction == threat_reaction_enum::react_as_emergency_1 && mInternalTargetPed->PedTypeIs(ped_type::player_2))
                {
                    if (bStartNetworkGame)
                    {
                        mInternalTargetPed->Kill();
                    }
                    else
                    {
                        mInternalTargetPed->mBf21C.bBusted = true;
                        SetVoiceEventIfNotOnFire(18);
                    }
                }
            }
        }
    }
    else
    {
        // Ped is too far
        if (mInternalTargetPed->GetPedVelocity() == kFpZero_678660)
        {
            if (gDistanceToTarget < kFpThreeEighths)
            {
                mGameObject->SetVelocity(mWalkSpeed);
            }
            else
            {
                mGameObject->RegulateVelocity(mMaxSpeed);
            }
        }
        else
        {
            if (mInternalTargetPed->mObjectiveTargetPed == this || mInternalTargetPed->mInternalTargetPed == this)
            {
                mGameObject->SetVelocity(mWalkSpeed);
            }
            else
            {
                mGameObject->RegulateVelocity(mMaxSpeed);
            }
        }

        if (mPedState2 == ped_state_2::carjack_pulling_out_9)
        {
            Car_Door* pDoor = mGameObject->mTargetCar->GetDoor(mTargetCarDoor);
            pDoor->Close();
            pDoor->set_ped(NULL);
        }
        Ped::UpdateMovementTowardsTarget(gDistanceToTarget, ped_move_target_type::internal_target_ped_0);
    }
}

MATCH_FUNC(0x46bd30)
void Ped::WaitOnFoot()
{
    if (mInternalObjectiveTimer == 0)
    {
        mInternalObjectiveStatus = objective_status::passed_1;
    }
}

MATCH_FUNC(0x46bd50)
bool Ped::IsOtherPedEnteringAsDriver(Car* pCar)
{
    u8 new_door_idx = 0;
    u8 door_idx = 0;
    while (new_door_idx < static_cast<u8>(pCar->GetDoorCount()))
    {
        Car_Door* pDoor = pCar->GetDoor(door_idx);
        if (pDoor)
        {
            Ped* pDoorPed = pDoor->get_pObj();
            if (pDoorPed)
            {
                if (!pDoorPed->mEnterCarAsPassenger && pDoorPed != this)
                {
                    return true;
                }
            }
        }
        door_idx = ++new_door_idx;
    }
    return false;
}

MATCH_FUNC(0x46bdc0)
void Ped::EnterCarStateMachine()
{
    Car_Door* pDoor;
    if (mCar && this->mTargetToEnter == mCar)
    {
        this->mInternalObjectiveStatus = objective_status::passed_1;
        return;
    }

    if (gGarage_48->IsParkingCarInPosition(this->mTargetToEnter))
    {
        if (PedTypeIs(ped_type::player_2))
        {
            SetObjective2(objectives_enum::no_obj_0);
            return;
        }
        this->mInternalObjectiveStatus = objective_status::failed_2;
        return;
    }

    this->mBf224.bWaitingOnTarget = false;

    if (mPedState1 == ped_state_1::immobilized_8)
    {
        return;
    }

    if (gDistanceToTarget > kFpFour && this->mPedType != ped_type::script_created_5 &&
        this->mThreatReaction != threat_reaction_enum::react_as_emergency_1)
    {
        this->mInternalObjectiveStatus = objective_status::failed_2;
        return;
    }

    if (!FindUsableCarDoor())
    {
        pDoor = mTargetToEnter->GetDoor(this->mTargetCarDoor);
        pDoor->Close();
        if (this->mPedState2 == ped_state_2::ped2_entering_a_car_6)
        {
            Sprite* pCarSprite = this->mTargetToEnter->mCarSprite;
            Ang16 ang_to_use = (kAng45_6784FC + pCarSprite->mRotation);
            mGameObject->HandleGenericImpact(ang_to_use, kFpPoint05_678634, kFpZero_678660, 1);
        }
        ChangeNextPedState1(ped_state_1::walking_0);
        ChangeNextPedState2(ped_state_2::ped2_walking_0);
        return;
    }

    this->mGameObject->Set_F84(this->mTargetToEnter);
    const bool bIsLawEnforcement = IsLawEnforcement();
    Fix16 vel_to_check = kFpPoint2;
    if (bIsLawEnforcement)
    {
        vel_to_check = kFpPoint1;
    }

    if (mTargetToEnter->GetVelocity() < vel_to_check &&
        (gDistanceToTarget < kFpFiveThirtySeconds || this->mPedState2 == ped_state_2::ped2_entering_a_car_6) &&
        mGameObject->mSpritePtr->mpCollisionShape->mBoundingBox.OverlapsZ(
            &mTargetToEnter->mCarSprite->mpCollisionShape->mBoundingBox))
    {
        this->mBf21C.bLeftVehicle = true;
        mGameObject->SetVelocityByRef_433920(kFpZero_678438);
        if (mPedState2 == ped_state_2::ped2_staying_14 || mPedState2 == ped_state_2::ped2_following_a_car_4 ||
            mPedState2 == ped_state_2::entering_train_5)
        {
            pDoor = mTargetToEnter->GetDoor(mTargetCarDoor);
            ChangeNextPedState2(ped_state_2::ped2_entering_a_car_6);
            ChangeNextPedState1(ped_state_1::entering_car_3);
            if (this->mInternalObjective != objectives_enum::enter_train_37)
            {
                pDoor->set_ped(this);
            }
            mTargetToEnter->TriggerCarAlarm();
        }
    }
    else
    {
        this->mGameObject->UseRunOrJumpSpeed();
        if (this->mPedState2 != ped_state_2::ped2_entering_a_car_6)
        {
            if (gDistanceToTarget > kFpTwo_678658)
            {
                UpdateMovementTowardsTarget(gDistanceToTarget, ped_move_target_type::move_target_pos_2);
            }
            else if (!mGameObject->mSpritePtr->mpCollisionShape->mBoundingBox.OverlapsZ(
                         &mTargetToEnter->mCarSprite->mpCollisionShape->mBoundingBox))
            {
                UpdateMovementTowardsTarget(gDistanceToTarget, ped_move_target_type::move_target_pos_2);
            }
            else if (gDistanceToTarget > kFpThreeQuarters && mGameObject->mFlagsBf.bOnSlope == 1)
            {
                UpdateMovementTowardsTarget(gDistanceToTarget, ped_move_target_type::move_target_pos_2);
            }
            else if (gDistanceToTarget > kFpOne_678798 && mGameObject->mIsCollidingWithSprite != 1)
            {
                UpdateMovementTowardsTarget(gDistanceToTarget, ped_move_target_type::move_target_pos_2);
            }
            else
            {
                ChangeNextPedState1(ped_state_1::entering_car_3);
                ChangeNextPedState2(ped_state_2::ped2_following_a_car_4);
            }
        }
    }

    if (!this->mEnterCarAsPassenger && this->mPedState2 == ped_state_2::ped2_entering_a_car_6)
    {
        if (IsOtherPedEnteringAsDriver(mTargetToEnter))
        {
            ChangeNextPedState1(ped_state_1::standing_still_7);
            ChangeNextPedState2(ped_state_2::ped2_staying_14);
            pDoor = mTargetToEnter->GetDoor(this->mTargetCarDoor);
            if (pDoor)
            {
                pDoor->set_ped(NULL);
            }
            this->mInternalObjectiveStatus = objective_status::failed_2;
            return;
        }

        if (mTargetToEnter->GetDriver())
        {
            if (mGameObject->mAnimationState == Char_Anim_state::Entering_Car_6)
            {
                if (mGameObject->mAnimationFrame == 2 && is_player() &&
                    !mTargetToEnter->CanBeEnteredByPed(this))
                {
                    SetObjective2(objectives_enum::no_obj_0);
                    SetObjective(objectives_enum::no_obj_0);
                }
                else
                {
                    if (mGameObject->mAnimationFrame == 4)
                    {
                        mGameObject->mAnimationFrame = 9;
                        if (mPlayer)
                        {
                            // Get score/report stolen etc
                            mPlayer->mScores.AwardCarHijackedScore(mTargetToEnter);

                            // Is it gang car?
                            const s16 gang_idx =
                                gGangPool_CA8->FindGangByCarModel(mTargetToEnter->mCarInfoIdx);
                            if (gang_idx != -1)
                            {
                                // Well now they hate you a bit
                                Gang* pGang = gGangPool_CA8->GangByIdx(static_cast<u8>(gang_idx));
                                pGang->ApplyKillRespectChange(this->mPlayer->get_idx(), 1u);
                            }
                        }
                    }
                }
            }
        }
    }
}

MATCH_FUNC(0x46c250)
void Ped::ExitCarStateMachine()
{
    bool bDoorFound = false;
    // Shared by both door paths; the z temporaries are block scoped (this gives the frame layout)
    Fix16 char_x;
    Fix16 char_y;
    this->mBf21C.bLeftVehicle = true;

    if (mPedState2 == ped_state_2::ped2_driving_10)
    {
        if (this->mCar->mpChaseTask)
        {
            gCarChaseTaskTable->FreeEntry(this->mCar->mpChaseTask);
            this->mCar->mpChaseTask = NULL;
        }

        if (!this->mEnterCarAsPassenger)
        {
            if (this->mObjective != objectives_enum::leave_car_and_stop_33)
            {
                mTargetToEnter->ClearDriver();
            }

            if (FindUsableCarDoor())
            {
                mTargetToEnter->GetDoorWorldPosition(mTargetCarDoor, &char_x, &char_y);
                AllocCharB4(char_x,
                            char_y,
                            gMap_0x370->FindGroundZAboveCoord(char_x,
                                                                  char_y,
                                                                  this->mTargetToEnter->mCarSprite->mZpos));

                {
                    SetRemap(get_remap());
                }

                ChangeNextPedState2(ped_state_2::ped2_getting_out_a_car_7);
                ChangeNextPedState1(ped_state_1::exiting_car_4);

                this->mCar = NULL;
                mGameObject->Set_F84(mTargetToEnter);
                this->mTargetToEnter = NULL;
                return;
            }

            {
                if (!AllocCharB4(mTargetToEnter->mCarSprite->mXy.x,
                                 mTargetToEnter->mCarSprite->mXy.y,
                                 gMap_0x370->FindGroundZAboveCoord(mTargetToEnter->mCarSprite->mXy.x,
                                                                       mTargetToEnter->mCarSprite->mXy.y,
                                                                       mTargetToEnter->mCarSprite->mZpos)))
                {
                    FatalError(1, "C:\\Splitting\\Gta2\\Source\\char.cpp", 11894);
                }
            }

            {
                SetRemap(get_remap());
            }

            ChangeNextPedState2(ped_state_2::ped2_walking_0);
            ChangeNextPedState1(ped_state_1::walking_0);

            this->mGameObject->SetVelocityByRef_433920(kFpZero_678438);
            mGameObject->DoJump();
            mGameObject->mSpritePtr->mRotation = mTargetToEnter->mCarSprite->mRotation;
            this->mGameObject->set_rotation(this->mTargetToEnter->mCarSprite->mRotation);
            this->mCar = NULL;
            this->mInternalObjectiveStatus = objective_status::passed_1;
            mGameObject->Set_F84(mTargetToEnter);
            this->mTargetToEnter = NULL;
        }
        else
        {
            if (FindUsableCarDoor())
            {
                bDoorFound = true;
                Car_Door* pDoor = mTargetToEnter->GetDoor(mTargetCarDoor);
                if (!pDoor->get_pObj() || this->mInternalObjective == objectives_enum::leave_train_38)
                {
                    mCar->mPassengersList.RemovePed_471240(this);
                    mTargetToEnter->GetDoorWorldPosition(mTargetCarDoor, &char_x, &char_y);

                    AllocCharB4(char_x,
                                char_y,
                                gMap_0x370->FindGroundZAboveCoord(char_x,
                                                                      char_y,
                                                                      this->mTargetToEnter->mCarSprite->mZpos));
                    {
                        SetRemap(get_remap());
                    }
                    ChangeNextPedState2(ped_state_2::ped2_getting_out_a_car_7);
                    ChangeNextPedState1(ped_state_1::exiting_car_4);
                    this->mCar = NULL;
                    mGameObject->Set_F84(mTargetToEnter);
                    this->mTargetToEnter = NULL;
                    pDoor->set_ped(this);
                }
            }
            else
            {
                if (this->mOccupation == ped_ocupation_enum::train_customer_9)
                {
                    Kill();
                    return;
                }

                mCar->mPassengersList.RemovePed_471240(this);

                {
                    AllocCharB4(mTargetToEnter->mCarSprite->mXy.x,
                                mTargetToEnter->mCarSprite->mXy.y,
                                gMap_0x370->FindGroundZAboveCoord(mTargetToEnter->mCarSprite->mXy.x,
                                                                      mTargetToEnter->mCarSprite->mXy.y,
                                                                      mTargetToEnter->mCarSprite->mZpos));
                }
                {
                    SetRemap(get_remap());
                }
                ChangeNextPedState2(ped_state_2::ped2_walking_0);
                ChangeNextPedState1(ped_state_1::walking_0);
                mGameObject->DoJump();
                this->mGameObject->mSpritePtr->mRotation = this->mTargetToEnter->mCarSprite->mRotation;
                this->mGameObject->set_rotation(this->mTargetToEnter->mCarSprite->mRotation);
                this->mGameObject->mJumpOverTimer = 10;
                this->mCar = NULL;
                mGameObject->Set_F84(mTargetToEnter);
                this->mTargetToEnter = NULL;
                this->mInternalObjectiveStatus = objective_status::passed_1;
            }
        }
    }

    if (mGameObject)
    {
        if (mTargetToEnter && mTargetToEnter->IsDespawning() ||
            mTargetObjectiveCar != NULL && mTargetObjectiveCar->IsDespawning() ||
            (this->mEscapeCar) != NULL && mEscapeCar->IsDespawning() ||
            (mGameObject->mTargetCar) != NULL && mGameObject->mTargetCar->IsDespawning())
        {
            this->mPedState1 = ped_state_1::standing_still_7;
            this->mPedState2 = ped_state_2::ped2_staying_14;
            mGameObject->mAnimationState = Char_Anim_state::Standing_2;
            this->mGameObject->mAnimationFrame = 0;
        }
        else if (!FindUsableCarDoor() && !bDoorFound)
        {
            DoJump();
        }

        if (this->mPedState2 != ped_state_2::ped2_getting_out_a_car_7 &&
            this->mGameObject->mAnimationState != Char_Anim_state::Exiting_Car_7)
        {
            this->mInternalObjectiveStatus = objective_status::passed_1;
            if (mPedGroup)
            {
                this->mInternalObjectiveStatus = mPedGroup->AreAllMembersOnFoot() != 0;
            }
        }
    }
}

MATCH_FUNC(0x46c770)
void Ped::WalkToTargetPoint()
{
    if (mPedState1 != ped_state_1::immobilized_8)
    {
        if (mGameObject->Get_F44() == 2 || mObjective == objectives_enum::enter_car_as_driver_35 ||
            gDistanceToTarget < kFpHalf)
        {
            Ped::SetObjective2(objectives_enum::no_obj_0);
            mBf21C.bPanicking = false;
        }
        else
        {
            Ped::UpdateMovementTowardsTarget(gDistanceToTarget, ped_move_target_type::internal_target_pos_1);
            mGameObject->SetVelocityByRef_433920(kFpOneSixteenth_678448);
        }
    }
}

MATCH_FUNC(0x46c7e0)
void Ped::ProcessInternalObjectiveGotoAreaOnFoot()
{
    if (mPedState1 != ped_state_1::immobilized_8)
    {
        if (gDistanceToTarget < kFpOneEighth_6784E8)
        {
            if (mGameObject->GetCharState() != Char_B4_state::Jumping_15)
            {
                Ped::ChangeNextPedState1(ped_state_1::standing_still_7);
                Ped::ChangeNextPedState2(ped_state_2::ped2_staying_14);
                mInternalObjectiveStatus = objective_status::passed_1;
            }
        }
        else
        {
            if (!mInternalObjectiveTimer)
            {
                mInternalObjectiveStatus = objective_status::passed_1;
            }
            if (mOccupation != ped_ocupation_enum::train_customer_9 && mObjective != objectives_enum::patrol_on_foot_42)
            {
                mGameObject->SetVelocity(mMaxSpeed);
            }
            else
            {
                mGameObject->SetVelocity(mWalkSpeed);
            }
        }
        Ped::UpdateMovementTowardsTarget(gDistanceToTarget, ped_move_target_type::internal_target_pos_1);
    }
}

MATCH_FUNC(0x46c8a0)
void Ped::WalkToTargetThenStop()
{
    if (mPedState1 != ped_state_1::immobilized_8)
    {
        mGameObject->SetVelocityByRef_433920(kFpOneSixteenth_678448);
        if (gDistanceToTarget < kFpHalf)
        {
            Ped::ChangeNextPedState1(ped_state_1::standing_still_7);
            Ped::ChangeNextPedState2(ped_state_2::ped2_staying_14);
            mInternalObjectiveStatus = objective_status::passed_1;
        }
        else
        {
            mJumpOverMode = ped_jump_over_mode::can_jump_2;
            Ped::UpdateMovementTowardsTarget(gDistanceToTarget, ped_move_target_type::move_target_pos_2);
            mGameObject->SetVelocityByRef_433920(kFpOneSixteenth_678448);
        }
    }
}

MATCH_FUNC(0x46c910)
void Ped::FollowPathPoints()
{
    if (mPedState1 != ped_state_1::immobilized_8)
    {
        if (gDistanceToTarget < kFpOneEighth_6784E8)
        {
            mCurrentPathPoint = mCurrentPathPoint + 1; // next patrol point
            mMoveTargetX = kFpHalf_67853C + Fix16(mCurrentPathPoint->mX);
            mMoveTargetY = kFpHalf_67853C + Fix16(mCurrentPathPoint->mY);
            if (mCurrentPathPoint->mX == 0)
            {
                mInternalObjectiveStatus = objective_status::passed_1;
                Ped::SetObjective2(objectives_enum::wait_on_foot_26);
            }
        }
        else
        {
            Ped::ChangeNextPedState1(ped_state_1::findind_path_2);
            Ped::ChangeNextPedState2(ped_state_2::ped2_walking_0);
            mGameObject->SetVelocityByRef_433920(kFpOneSixteenth_678448);
        }
    }
}

MATCH_FUNC(0x46c9b0)
void Ped::CrossRoad()
{
    if (mPedState1 != ped_state_1::immobilized_8)
    {
        if (gDistanceToTarget < kFpOneEighth_6784E8)
        {
            Ped::ChangeNextPedState1(ped_state_1::walking_0);
            Ped::ChangeNextPedState2(ped_state_2::ped2_walking_0);
            mInternalObjectiveStatus = objective_status::not_finished_0;
            Ped::SetObjective2(objectives_enum::wait_after_crossing_49, 100);
            if (--gNumPedsCrossingRoad < 0)
            {
                gNumPedsCrossingRoad = 0;
            }
        }
        else
        {
            if (mInternalObjectiveTimer == 0)
            {
                mInternalObjectiveStatus = objective_status::passed_1;
            }
            if (gTrafficLights_194->ArePedsCrossing())
            {
                mGameObject->SetVelocityByRef_433920(kFpOneThirtySecond);
            }
            else
            {
                if (mGameObject->Get_F44() == 1)
                {
                    mGameObject->SetVelocityByRef_433920(kFpOneSixteenth_678448);
                }
                else
                {
                    mGameObject->SetVelocityByRef_433920(kFpOneThirtySecond);
                }
            }
        }
    }
}

MATCH_FUNC(0x46ca60)
void Ped::ProcessInternalObjectiveGotoAreaInCar()
{
}

MATCH_FUNC(0x46ca70)
void Ped::ProcessInternalObjectiveChasePedInCar()
{
    if (!this->mCar->mpChaseTask)
    {
        this->mCar->mpChaseTask = gCarChaseTaskTable->AllocateEntry();
        this->mCar->mpChaseTask->mpDriver = this;
    }

    if (this->mObjective == objectives_enum::kill_char_any_means_19)
    {
        mCar->mpChaseTask->mTaskType = car_task_type::kill_ped_5;
        if (mBf21C.bUseCarWeapon)
        {
            this->mBf21C.bAttacking = true;
        }
    }
    else
    {
        mCar->mpChaseTask->mTaskType = car_task_type::follow_ped_2;
    }

    this->mCar->SetDriverPedType(ped_type::script_created_5);
    this->mCar->mpChaseTask->mpTargetPed = this->mInternalTargetPed;
    this->mCar->ClearAIHalted();
    this->mCar->mAI->mMaxSpeed = kFpThree_67866C;
    this->mCar->mpChaseTask->mCanSnapToTarget = true;
}

// 9.6f 0x43A550
MATCH_FUNC(0x46cb30)
void Ped::StartPedCrossingAtTrafficLight_Y_Backward()
{
    Fix16 y_iter = mCam.y;
    if (mPedState1 != ped_state_1::immobilized_8 && gTrafficLights_194->ArePedsCrossing())
    {
        for (u8 i = 0; i < 6; i++)
        {
            y_iter -= kFpOne_678664;
            u8 ypos = y_iter.ToInt();
            if (gMap_0x370->IsBlockPavementTypeInlined_433530(mCam.x.ToInt(),
                                                              ypos,
                                                              (mCam.z - kFpOne_678664).ToInt()))
            {
                SetObjective2(objectives_enum::crossing_road_48);
                Fix16 t = Fix16(ypos) + kFpHalf_67853C;
                SetMoveTargetX(mCam.x);
                SetMoveTargetY(t);
                SetMoveTargetZ(mCam.z);
                break;
            }
        }
    }
}

MATCH_FUNC(0x46cc70)
void Ped::StartPedCrossingAtTrafficLight_X_Forwards()
{
    Fix16 x_iter = mCam.x;
    if (mPedState1 != ped_state_1::immobilized_8 && gTrafficLights_194->ArePedsCrossing())
    {
        for (u8 i = 0; i < 6; i++)
        {
            x_iter += kFpOne_678664;
            u8 x = x_iter.ToUInt8();
            if (gMap_0x370->IsBlockPavementTypeInlined_433530(x,
                                                              mCam.y.ToInt(),
                                                              (mCam.z - kFpOne_678664).ToInt()))
            {
                Ped::SetObjective2(objectives_enum::crossing_road_48);
                Fix16 xpos(x);
                xpos += kFpHalf_67853C;
                SetMoveTargetX(xpos);
                SetMoveTargetY(mCam.y);
                SetMoveTargetZ(mCam.z);
                break;
            }
        }
    }
}

MATCH_FUNC(0x46cdb0)
void Ped::StartPedCrossingAtTrafficLight_Y_Forwards()
{
    Fix16 y_iter = mCam.y;
    if (mPedState1 != ped_state_1::immobilized_8 && gTrafficLights_194->ArePedsCrossing())
    {
        for (u8 i = 0; i < 6; i++)
        {
            y_iter += kFpOne_678664;
            u8 y = y_iter.ToUInt8();
            if (gMap_0x370->IsBlockPavementTypeInlined_433530(mCam.x.ToInt(),
                                                              y,
                                                              (mCam.z - kFpOne_678664).ToInt()))
            {
                Ped::SetObjective2(objectives_enum::crossing_road_48);
                Fix16 ypos(y);
                ypos += kFpHalf_67853C;
                SetMoveTargetX(mCam.x);
                SetMoveTargetY(ypos);
                SetMoveTargetZ(mCam.z);
                break;
            }
        }
    }
}

MATCH_FUNC(0x46cef0)
void Ped::StartPedCrossingAtTrafficLight_X_Backwards()
{
    Fix16 x_iter = mCam.x;
    if (mPedState1 != ped_state_1::immobilized_8 && gTrafficLights_194->ArePedsCrossing())
    {
        for (u8 i = 0; i < 6; i++)
        {
            x_iter -= kFpOne_678664;
            u8 x = x_iter.ToUInt8();
            if (gMap_0x370->IsBlockPavementTypeInlined_433530(x,
                                                              mCam.y.ToInt(),
                                                              (mCam.z - kFpOne_678664).ToInt()))
            {
                Ped::SetObjective2(objectives_enum::crossing_road_48);
                Fix16 xpos(x);
                xpos += kFpHalf_67853C;
                SetMoveTargetX(xpos);
                SetMoveTargetY(mCam.y);
                SetMoveTargetZ(mCam.z);
                break;
            }
        }
    }
}

MATCH_FUNC(0x46d030)
void Ped::WaitForTrain()
{
    if (mPedState1 != ped_state_1::immobilized_8)
    {
        Train* pTrain = gPublicTransport_181C->GetTrainFromCarExcludingLeadCar(mTargetToEnter);
        Car* pOldTarget = mTargetToEnter;
        if (pTrain->mStation->mPlatformState == 2 &&
            pTrain->mCarriages[1]->GetCarInfoIdx() == car_model_enum::TRAIN)
        {
            Ped::SetObjective2(objectives_enum::enter_train_37);
            set_target_to_enter(pOldTarget);
            mGameObject->Set_F84(pOldTarget);
            mGameObject->SetVelocityByRef_433920(kFpOneSixteenth_678448);
        }
    }
}

MATCH_FUNC(0x46d0b0)
void Ped::WaitForInternalObjectiveTimer()
{
    if (mInternalObjectiveTimer == 0)
    {
        mInternalObjectiveStatus = objective_status::passed_1;
    }
}

MATCH_FUNC(0x46d0d0)
void Ped::EnterTrainStateMachine()
{
    s32 state1;
    Fix16 best_dist;
    u8 new_door_idx;
    Fix16 door_xd;
    Fix16 door_yd;
    Fix16 dist;
    Char* pChar;
    bool found_door;
    u8 best_door;
    u8 door_idx;
    Fix16 door_x;
    Fix16 door_y;
    Fix16 abs_xd;
    Fix16 abs_yd;

    found_door = false;
    state1 = mPedState1;
    mBf21C.bLeftVehicle = true;
    if (state1 != ped_state_1::immobilized_8)
    {
        if (gDistanceToTarget > kFpFour)
        {
            SetObjective2(objectives_enum::no_obj_0);
            mInternalObjectiveStatus = objective_status::failed_2;
        }
        else
        {
            if (!gPublicTransport_181C->GetTrainFromCarExcludingLeadCar(mTargetToEnter))
            {
                mEnterCarAsPassenger = 0;
            }
            else
            {
                mEnterCarAsPassenger = 1;
            }
            best_dist = kFp9999;
            best_door = 0;
            new_door_idx = 0;
            door_idx = new_door_idx;
            while (new_door_idx < static_cast<u8>(mTargetToEnter->GetDoorCount()))
            {
                if (mTargetToEnter->IsDoorAccessible(door_idx))
                {
                    mTargetToEnter->GetDoorWorldPosition(door_idx, &door_x, &door_y);
                    door_xd = door_x - mCam.x;
                    door_yd = door_y - mCam.y;
                    abs_xd = Fix16::Abs(door_xd);
                    abs_yd = Fix16::Abs(door_yd);
                    dist = Fix16::Max(abs_xd, abs_yd);
                    if (dist < best_dist)
                    {
                        best_dist = dist;
                        best_door = new_door_idx;
                        found_door = true;
                    }
                }
                door_idx = ++new_door_idx;
            }
            if (found_door == 1)
            {
                mTargetCarDoor = best_door;
                pChar = mGameObject;
                pChar->UseRunOrJumpSpeed();
                EnterCarStateMachine();
                if (mInternalObjectiveStatus == objective_status::passed_1)
                {
                    Deallocate();
                }
            }
        }
    }
}

MATCH_FUNC(0x46d240)
void Ped::ExitTrainStateMachine()
{
    bool door_available;
    volatile bool found_usable_door; // NOTE: volatile keeps the flag in its stack slot like OG

    door_available = true;
    mEnterCarAsPassenger = 1;
    mBf21C.bLeftVehicle = true;
    if (mPedState2 == ped_state_2::ped2_driving_10)
    {
        found_usable_door = false;
        u8 attempt = 0;
        while (true)
        {
            attempt++;
            if (mTargetToEnter->IsStoppedWithPavementAtDoor(mTargetCarDoor))
            {
                found_usable_door = true;
                break;
            }
            mTargetCarDoor++;
            if (mTargetCarDoor > 3u)
            {
                mTargetCarDoor = 0;
            }
            if (attempt >= 5)
            {
                break;
            }
        }
        door_available = found_usable_door;
    }

    if (mInternalObjectiveStatus == objective_status::passed_1 || !door_available)
    {
        if (PedTypeIs(ped_type::player_2))
        {
            SetObjective(objectives_enum::no_obj_0);
            SetObjective2(objectives_enum::no_obj_0);
        }
    }
    else
    {
        ExitCarStateMachine();
    }
}

MATCH_FUNC(0x46d300)
void Ped::FollowCarOnFoot()
{
    if (this->mPedState1 != ped_state_1::immobilized_8)
    {
        if (gDistanceToTarget <= kFpQuarter)
        {
            if (this->mGameObject->GetCharState() != Char_B4_state::Jumping_15)
            {
                if (gDistanceToTarget < kFpOneEighth_6784E8)
                {
                    if (mTargetObjectiveCar->GetVelocity() <= GetPedVelocity())
                    {
                        this->mGameObject->SetVelocity(mTargetObjectiveCar->GetVelocity());
                    }
                }
                else
                {
                    this->mGameObject->RegulateVelocity(mTargetObjectiveCar->GetVelocity() + kFpOne64th);
                }
            }
        }
        else if (gDistanceToTarget < kFpHalf)
        {
            this->mGameObject->RegulateVelocity(mTargetObjectiveCar->GetVelocity() + kFpOne64th);
        }
        else
        {
            this->mGameObject->IncreaseSpeedIfAllowed();
        }
        UpdateMovementTowardsTarget(gDistanceToTarget, ped_move_target_type::internal_target_pos_1);
    }
}


// https://decomp.me/scratch/5y8iN
MATCH_FUNC(0x46d460)
void Ped::AttackTargetStateMachine(u8 targetType)
{
    Fix16 attack_range;

    bool bThrownWeapon = false;
    bool bMeleeWeapon = false;
    bool bTargetGone = false;
    bool bAttackingVehicleOrObject = false;

    mBf21C.bWeaponChosen = false;
    Weapon* pWeapon = Ped::ChooseAttackWeapon();
    Fix16 close_range = kFpOne_678664;
    Fix16 explosive_range = kFpOne_678798 + kFpQuarter;
    if (mGameObject)
    {
        if (pWeapon)
        {
            switch (pWeapon->mWeaponType)
            {
                case weapon_type::flamethrower:
                    attack_range = kFpTwo_678658;
                    explosive_range = kFpOne_678798;
                    break;

                case weapon_type::molotov:
                    attack_range = kFpTwo_678658 * 2;
                    explosive_range = kFpOne_678798 + kFpHalf;
                    bThrownWeapon = true;
                    break;

                case weapon_type::grenade:
                    attack_range = kFpHalf + kFpTwo_678658;
                    explosive_range = kFpTwo_678658;
                    bThrownWeapon = true;
                    break;

                case weapon_type::electro_batton:
                    close_range = kFpHalf_67853C;
                    attack_range = kFpTwo_678658;
                    bMeleeWeapon = true;
                    break;

                case weapon_type::shotgun:
                    close_range = kFpHalf_67853C;
                    attack_range = kFpOne_678798;
                    bMeleeWeapon = true;
                    break;

                default:
                    attack_range = kFpFour;
                    break;
            }
        }
        else
        {
            attack_range = kFpTwo_678658; // no weapon
        }
    }
    else
    {
        attack_range = kFpFour; // probably in a car
    }

    mBf21C.bFindingPath = false;
    Fix16 zpos;

    switch (targetType)
    {
        case ped_attack_target_type::ped_0: // target is a ped
            zpos = mInternalTargetPed->get_cam_z();
            if (mInternalTargetPed->isDead() || !mInternalTargetPed->IsActive())
            {
                bTargetGone = true;
            }
            if (mInternalTargetPed->get_car())
            {
                if (mInternalTargetPed->mCar->GetVelocity() <= kFpPoint02)
                {
                    attack_range = kFpTwo_678668;
                    close_range = kFpTwo_678668 + kFpHalf_67853C;
                    bAttackingVehicleOrObject = true;
                }
            }

            break;

        case ped_attack_target_type::car_1: // target is a car
            zpos = mTargetToEnter->GetZ();
            if (mTargetToEnter->IsMaxDamage())
            {
                bTargetGone = true;
            }
            close_range = kFpThree_67866C;
            attack_range = kFpTwo_678668 + kFpHalf_67853C;
            bAttackingVehicleOrObject = true;
            break;

        case ped_attack_target_type::object_2: // target is a object
            zpos = mInternalTargetObject->GetZ();
            if (mInternalTargetObject->IsDestroyedPowergen())
            {
                mInternalObjectiveStatus = objective_status::passed_1;
            }
            if (mOccupation != ped_ocupation_enum::stand_still_bloke)
            {
                close_range = kFpThree_67866C;
                attack_range = kFpTwo_678668 + kFpHalf_67853C;
                bAttackingVehicleOrObject = true;
            }
            break;
    }

    if (bTargetGone)
    {
        mBf21C.bAttacking = false;
        mInternalObjectiveStatus = objective_status::passed_1;
        if (mObjective == objectives_enum::no_obj_0)
        {
            Ped::SetObjective2(objectives_enum::no_obj_0);
            mBf21C.bPanicking = false;
        }
        return;
    }

    if (mPedState1 == ped_state_1::immobilized_8)
    {
        return;
    }

    if (targetType == 0) // target is a ped
    {
        mInternalTargetPed->SetAttacker(this);
        mInternalTargetPed->IncrementAttackersCount();
    }

    if (mHitTargetPed)
    {
        Ped::ChangeNextPedState1(ped_state_1::standing_still_7);
        Ped::ChangeNextPedState2(ped_state_2::attacking_standing_11);
        mBf21C.bAttacking = true;
    }
    else
    {
        if (mOccupation == ped_ocupation_enum::stand_still_bloke)
        {
            pWeapon->SetTargetHit(0);
        }

        if (mPathPointCount < 4 && gDistanceToTarget < attack_range && abs_sub_less_than_epislon(mCam.z, zpos) ||
            mBf21C.bHoldPosition == true)
        {
            if (pWeapon)
            {
                if (pWeapon->IsTargetHit() == 1)
                {
                    if (gDistanceToTarget < close_range && !mGameObject->IsCollidingWithASprite())
                    {
                        Ped::ChangeNextPedState1(ped_state_1::flee_or_running_1);
                        Ped::ChangeNextPedState2(ped_state_2::running_to_target_2);
                        mBf21C.bAttacking = true;
                        gCloseRangeHit = 1;
                        mGameObject->RegulateVelocity(mMaxSpeed);

                        if (mGameObject->mFlagsBf.bDetouring)
                        {
                            Ped::SetObjective2(objectives_enum::flee_on_foot_till_safe_1);
                            mMoveTargetX = mCam.x;
                            mMoveTargetY = mCam.y;
                        }
                        return;
                    }

                    if (!mBf21C.bHoldPosition)
                    {
                        Ped::ChangeNextPedState1(ped_state_1::flee_or_running_1);
                        Ped::ChangeNextPedState2(ped_state_2::running_to_target_2);
                        mBf21C.bAttacking = true;
                        mGameObject->RegulateVelocity(mMaxSpeed);
                        return;
                    }
                }
                else
                {
                    if (pWeapon->IsExplosiveWeapon() && gDistanceToTarget < explosive_range)
                    {
                        if (mpWeapon)
                        {
                            mBf21C.bAttacking = true;
                            mBf21C.bWeaponChosen = true;
                        }
                        else
                        {
                            mBf21C.bAttacking = false;
                        }

                        if (!mBf21C.bHoldPosition)
                        {
                            if (mGameObject->mFlagsBf.bDetouring)
                            {
                                Ped::SetObjective2(objectives_enum::flee_on_foot_till_safe_1);
                                mMoveTargetX = mCam.x;
                                mMoveTargetY = mCam.y;
                            }
                            else
                            {
                                if (mFacingAngle == mTargetFacingAngle)
                                {
                                    mGameObject->SetVelocity(-mWalkSpeed);
                                }
                                Ped::ChangeNextPedState1(ped_state_1::flee_or_running_1);
                                Ped::ChangeNextPedState2(ped_state_2::running_to_target_2);
                            }
                        }
                        return;
                    }
                }

                if (bThrownWeapon)
                {
                    if (gMap_0x370->GetBlockTypeAtCoord(
                            (mGameObject->mSpritePtr->mXy.x).ToInt(),
                            (mGameObject->mSpritePtr->mXy.y).ToInt(),
                            (mGameObject->mSpritePtr->mZpos + kFpOne_678664).ToInt()) != AIR)
                    {
                        Ped::ChangeNextPedState1(ped_state_1::flee_or_running_1);
                        Ped::ChangeNextPedState2(ped_state_2::running_to_target_2);
                        mBf21C.bAttacking = false;
                        mGameObject->RegulateVelocity(mMaxSpeed);
                    }
                }

                if ((mOccupation == ped_ocupation_enum::fbi || IsGroupLeader() || bMeleeWeapon) &&
                    mOccupation != ped_ocupation_enum::stand_still_bloke && !bAttackingVehicleOrObject)
                {
                    if (gDistanceToTarget < kFpHalf_67853C)
                    {
                        if (mGameObject->GetCharState() != Char_B4_state::Jumping_15)
                        {
                            Ped::ChangeNextPedState1(ped_state_1::standing_still_7);
                            Ped::ChangeNextPedState2(ped_state_2::attacking_standing_11);
                            mBf21C.bAttacking = true;
                        }
                        else
                        {
                            mBf21C.bAttacking = false;
                        }
                    }
                    else
                    {
                        mBf21C.bAttacking = true;
                        return;
                    }
                }
                else if (mGameObject->GetCharState() != Char_B4_state::Jumping_15)
                {
                    Ped::ChangeNextPedState1(ped_state_1::standing_still_7);
                    Ped::ChangeNextPedState2(ped_state_2::attacking_standing_11);
                    mBf21C.bAttacking = true;
                    // 9.6f: Char::SetVelocity (inlined, using it makes the diff worse)
                    mGameObject->mVelocity = kFpZero_678438;
                    return;
                }
                else
                {
                    mBf21C.bAttacking = false;
                    return;
                }
            }
            else
            {
                // no weapon
                Ped::MeleeAttackStateMachine();
            }
        }
        else
        {
            switch (targetType)
            {
                case ped_attack_target_type::ped_0:  // target is a ped
                    Ped::UpdateMovementTowardsTarget(gDistanceToTarget, ped_move_target_type::internal_target_ped_0);
                    break;
                case ped_attack_target_type::car_1:  // target is a car
                    Ped::UpdateMovementTowardsTarget(gDistanceToTarget, ped_move_target_type::move_target_pos_2);
                    break;
                case ped_attack_target_type::object_2:  // target is a object
                    Ped::UpdateMovementTowardsTarget(gDistanceToTarget, ped_move_target_type::internal_target_object_6);
                    break;
            }

            if (gDistanceToTarget > kFpEight)
            {
                mBf21C.bAttacking = false;
            }
            mGameObject->RegulateVelocity(mMaxSpeed);
        }
    }
}

MATCH_FUNC(0x46db60)
void Ped::AttackPed()
{
    AttackTargetStateMachine(ped_attack_target_type::ped_0);
}

MATCH_FUNC(0x46db70)
void Ped::AttackCar()
{
    AttackTargetStateMachine(ped_attack_target_type::car_1);
}

MATCH_FUNC(0x46db80)
void Ped::AttackObject()
{
    AttackTargetStateMachine(ped_attack_target_type::object_2);
}

MATCH_FUNC(0x46df50)
Sprite* Ped::GetSprite() const
{
    Car* pCar = this->mCar;
    if (pCar)
    {
        return pCar->mCarSprite;
    }
    else
    {
        return this->mGameObject->mSpritePtr;
    }
}

MATCH_FUNC(0x46df70)
void Ped::SetupFollower(const Ped* pToFollow, s32 weaponIdx)
{
    set_remap(pToFollow->mRemap);
    SetRemap(pToFollow->mRemap);
    mGraphicType = pToFollow->mGraphicType;
    Ped::RemovePedWeapons();
    Ped::ForceWeapon(weaponIdx);
    set_occupation(ped_ocupation_enum::special_groups_member);
    mThreatSearch = pToFollow->mThreatSearch;
    mThreatReaction = pToFollow->mThreatReaction;
    mpGang = pToFollow->mpGang;
    SetJumpOverMode(ped_jump_over_mode::cannot_jump_1);
    SetPedClass(ped_class::gang_member_1);
    SetPedType(ped_type::special_ped_4);
}

MATCH_FUNC(0x46e020)
bool Ped::CanBeRecruitedToGroup(const PedGroup* pGroup) const
{
    return this->mPedGroup != pGroup && !this->mPlayer &&
            (PedTypeIs(ped_type::dummy_3) || (PedTypeIs(ped_type::special_ped_4) || PedTypeIs(ped_type::dummy_with_occupation_6)) && this->mOccupation == ped_ocupation_enum::special_groups_member) ?
        true :
        false;
}

// 9.6f: Fix16_Rect::ComputeCollisionPrism(x, y, offset, z), inlined. The 10.5 version uses
// kFpOneEighth_67845C (the Fix16_Rect.hpp one uses kCollisionPrismHalfHeight), inline x/y
// arithmetic and the out-of-line Subtract/operator+ for z. The by-value z parameter is what
// puts z in desiredCount's dead stack slot.
static inline void ComputeRecruitPrism(Fix16_Rect& r, Fix16 x, Fix16 y, Fix16 offset, Fix16 z)
{
    s32 half = offset.mValue / 2;
    r.mLeft.mValue = x.mValue - half;
    r.mRight.mValue = half + x.mValue;
    r.mTop.mValue = y.mValue - half;
    r.mBottom.mValue = y.mValue + half;
    r.mLowZ = z.Subtract(kFpOneEighth_67845C);
    r.mHighZ = (z).Add(kFpOneEighth_67845C);
}

MATCH_FUNC(0x46e080)
void Ped::RecruitNearbyPeds(s32 desiredCount, Fix16 searchRadius)
{
    PedGroup* pMyGroup;
    s32 maxCount;
    Sprite* pNearest;
    Char* pChar;
    Ped* pPed;
    PedGroup* pOldGroup;
    SpriteList collision_list;
    Fix16_Rect rect;

    pMyGroup = this->mPedGroup;
    if (pMyGroup)
    {
        maxCount = desiredCount;
        if (desiredCount > 9)
        {
            maxCount = 9;
        }

        if (pMyGroup->mCount34 >= maxCount)
        {
            return;
        }
    }
    else
    {
        SpawnPedGroupFollowers(0);
        maxCount = desiredCount;
    }

    ComputeRecruitPrism(rect, this->mCam.x, this->mCam.y, searchRadius, this->mCam.z);
    if (gSpriteGrid_1->CollectRectCollisions(&rect, false, sprite_types_enum::none_0, GetSprite(), &collision_list))
    {
        for (pNearest = collision_list.TakeClosestSprite(this->mCam.x, this->mCam.y); pNearest;
             pNearest = collision_list.TakeClosestSprite(this->mCam.x, this->mCam.y))
        {
            pChar = pNearest->AsCharB4();

            if (pChar)
            {
                pPed = pChar->get_ped();
                if (pPed->CanBeRecruitedToGroup(this->mPedGroup))
                {
                    pOldGroup = pPed->mPedGroup;
                    if (pOldGroup)
                    {
                        pOldGroup->RemovePed(pPed);
                    }
                    mPedGroup->add_ped_to_end_of_list(pPed);
                    pPed->SetupFollower(this, weapon_type::dual_pistol);
                    if (this->mPedGroup->mCount34 == maxCount)
                    {
                        break;
                    }
                }
            }
        }
    }
    collision_list.ClearList();
}

MATCH_FUNC(0x46e200)
void Ped::SpawnPedGroupFollowers(u8 total)
{
    PedGroup* pGroup = PedGroup::New();
    pGroup->add_ped_leader(this);
    u8 current = 0;
    pGroup->SetCounts(total);
    if (total > 0)
    {
        s32 i = 0;
        do
        {

            Ped* pNewPed = gPedPool->Allocate();

            pNewPed->set_occupation(this->mOccupation);
            pNewPed->set_remap(this->mRemap);
            pNewPed->mGraphicType = this->mGraphicType;
            pNewPed->SetPedType(this->mPedType);
            Fix16 xy_off = kFpOneSixth * Fix16(i);
            pNewPed->AllocCharB4(xy_off + this->mCam.x, xy_off + this->mCam.y, this->mCam.z);
            pNewPed->SetRemap(this->get_remap());
            pNewPed->set_health(this->mHealth);
            pNewPed->SetJumpOverMode(this->mJumpOverMode);
            pNewPed->SetPedClass(this->mPedClass);
            pNewPed->mThreatSearch = this->mThreatSearch;
            pNewPed->mThreatReaction = this->mThreatReaction;
            pNewPed->mpGang = this->mpGang;
            pGroup->add_ped_to_list(pNewPed, current);

            Weapon* pWeapon = this->mSelectedWeapon;
            if (pWeapon)
            {
                pNewPed->ForceWeapon(pWeapon->mWeaponType);
            }

            ++current;
            ++i;
        } while (current < total);
    }
}

MATCH_FUNC(0x46ef00)
u8 Ped::get_wanted_star_count() const
{
    s16 wanted_points = mWantedPoints;
    if (wanted_points < cop_level_ped_enum::cop_6_stars)
    {
        if (wanted_points < cop_level_ped_enum::cop_5_stars)
        {
            if (wanted_points < cop_level_ped_enum::cop_4_stars)
            {
                if (wanted_points < cop_level_ped_enum::cop_3_stars)
                {
                    if (wanted_points < cop_level_ped_enum::cop_2_stars)
                    {
                        if (wanted_points < cop_level_ped_enum::cop_1_stars)
                        {
                            return cop_level_enum::none_0;
                        }
                        else
                        {
                            return cop_level_enum::police_1;
                        }
                    }
                    else
                    {
                        return cop_level_enum::police_2;
                    }
                }
                else
                {
                    return cop_level_enum::police_3;
                }
            }
            else
            {
                return cop_level_enum::swat_4;
            }
        }
        else
        {
            return cop_level_enum::fbi_5;
        }
    }
    else
    {
        return cop_level_enum::army_6;
    }
}

MATCH_FUNC(0x46ef40)
void Ped::set_wanted_level(u16 wanted)
{
    switch (wanted)
    {
        case cop_level_ped_enum::cop_0_stars:
            mWantedPoints = cop_level_ped_enum::cop_0_stars;
            break;

        case cop_level_ped_enum::cop_1_stars:
            mWantedPoints = cop_level_ped_enum::cop_1_stars;
            break;

        case cop_level_ped_enum::cop_2_stars:
            mWantedPoints = cop_level_ped_enum::cop_2_stars;
            break;

        case cop_level_ped_enum::cop_3_stars:
            mWantedPoints = cop_level_ped_enum::cop_3_stars;
            break;

        case cop_level_ped_enum::cop_4_stars:
            mWantedPoints = cop_level_ped_enum::cop_4_stars;
            break;
        case cop_level_ped_enum::cop_5_stars:
            mWantedPoints = cop_level_ped_enum::cop_5_stars;
            break;
        case cop_level_ped_enum::cop_6_stars:
            mWantedPoints = cop_level_ped_enum::cop_6_stars;
            break;
    }
}

MATCH_FUNC(0x46EFD0)
void Ped::IncreaseWantedLevelFromDebugKeys()
{
    switch (get_wanted_star_count())
    {
        case cop_level_enum::none_0:
            set_wanted_level(600u);
            break;
        case cop_level_enum::police_1:
            set_wanted_level(1600u);
            break;
        case cop_level_enum::police_2:
            set_wanted_level(3000u);
            break;
        case cop_level_enum::police_3:
            set_wanted_level(5000u);
            break;
        case cop_level_enum::swat_4:
            set_wanted_level(8000u);
            break;
        case cop_level_enum::fbi_5:
            set_wanted_level(12000u);
            break;
        case cop_level_enum::army_6:
            set_wanted_level(0);
            break;
        default:
            break;
    }

    u8 stars = get_wanted_star_count();
    u8 max_stars = gPolice_7B8->mMaxWantedStars;
    if (stars > max_stars)
    {
        set_wanted_star_count(max_stars);
    }
}

MATCH_FUNC(0x46f070)
void Ped::set_wanted_star_count(u8 star_count)
{
    switch (star_count)
    {
        case cop_level_enum::none_0:
            mWantedPoints = cop_level_ped_enum::cop_0_stars;
            break;
        case cop_level_enum::police_1:
            mWantedPoints = cop_level_ped_enum::cop_1_stars;
            break;
        case cop_level_enum::police_2:
            mWantedPoints = cop_level_ped_enum::cop_2_stars;
            break;
        case cop_level_enum::police_3:
            mWantedPoints = cop_level_ped_enum::cop_3_stars;
            break;
        case cop_level_enum::swat_4:
            mWantedPoints = cop_level_ped_enum::cop_4_stars;
            break;
        case cop_level_enum::fbi_5:
            mWantedPoints = cop_level_ped_enum::cop_5_stars;
            break;
        case cop_level_enum::army_6:
            mWantedPoints = cop_level_ped_enum::cop_6_stars;
            break;
        default:
            return;
    }
}

MATCH_FUNC(0x46f100)
bool Ped::WantedStarCountGreaterThan(u8 star_count) const
{
    return star_count < get_wanted_star_count();
}

MATCH_FUNC(0x46f110)
Weapon* Ped::GetWeaponFromPed()
{
    if (PedTypeIs(ped_type::player_2))
    {
        mSelectedWeapon = mPlayer->GetCurrPlayerWeapon();

        if (mCar)
        {
            if (mSelectedWeapon && gWeapon_8->is_car_weapon_kind(mSelectedWeapon->mWeaponType))
            {
                mBf21C.bUseCarWeapon = true;
                return mSelectedWeapon;
            }
        }
        else
        {
            if (!mSelectedWeapon)
            {
                mBf21C.bNoWeapon = true;
            }
            else
            {
                return mSelectedWeapon;
            }
        }
        return NULL;
    }
    else
    {
        if (!mCar || mPedType == ped_type::player_2)
        {
            if (mBf21C.bWeaponChosen)
            {
                return mpWeapon;
            }

            if (mBf21C.bNoWeapon)
            {
                return NULL;
            }
            return mSelectedWeapon;
        }
        else
        {
            Car* pCar = mCar;
            Weapon* pCarWeapon = NULL;
            if (pCar->IsFireTruck())
            {
                pCarWeapon = gWeapon_8->find(pCar, 20);
            }
            else if (pCar->IsTank())
            {
                pCarWeapon = gWeapon_8->find(pCar, 19);
            }
            else if (pCar->IsGunJeep())
            {
                pCarWeapon = gWeapon_8->find(pCar, 22);
            }

            if (!pCarWeapon)
            {
                return NULL;
            }
            return pCarWeapon;
        }
    }

    return NULL;
}

MATCH_FUNC(0x46f1e0)
void Ped::ApplyAimJitter(Weapon* pWeapon)
{
    u8 rng_val = 0;
    // Named locals: the original gives each max a stack slot of its own (temporaries share the param slot)
    s16 max_still;
    s16 max_moving;
    s16 max_moving_retry;
    if (pWeapon->mWeaponType >= weapon_type::pistol && pWeapon->mWeaponType <= weapon_type::smg)
    {
        if (mFireMode == ped_fire_mode::random_shots_0)
        {
            if (GetPedVelocity() == kFpZero_678660)
            {
                max_still = 3;
                rng_val = static_cast<u8>(gRng.get_int(max_still));
            }
            else
            {
                max_moving = 5;
                rng_val = static_cast<u8>(gRng.get_int(max_moving));
                if (rng_val == 0)
                {
                    max_moving_retry = 5;
                    rng_val = static_cast<u8>(gRng.get_int(max_moving_retry));
                }
            }
        }
        else if (mFireMode == ped_fire_mode::never_fires_2)
        {
            rng_val = 0;
        }

        switch (rng_val)
        {
            case 1:
                mAimAngle = mAimAngle - kAng10;
                break;
            case 2:
                mAimAngle = mAimAngle + kAng10;
                break;
            case 3:
                mAimAngle = mAimAngle - kAng16_6784E4;
                break;
            case 4:
                mAimAngle = mAimAngle + kAng16_6784E4;
                break;
            default:
                break;
        }
    }
}

MATCH_FUNC(0x46f390)
void Ped::ManageWeapon()
{
    Weapon* pWeapon = Ped::GetWeaponFromPed();
    if (Ped::PedTypeIs(ped_type::player_2))
    {
        if (mGameObject)
        {
            mAimAngle = mGameObject->mSpritePtr->mRotation;
        }
    }
    if (mBf21C.bAttacking == true)
    {
        if (pWeapon)
        {
            if (!mPedRefIdx)
            {
                mPedRefIdx = gPedRefTable_7F8->AllocForPed(mId);
            }
            if (!mBf21C.bUseCarWeapon)
            {
                if (mGameObject)
                {
                    if (!Ped::PedTypeIs(ped_type::player_2))
                    {
                        Ped::ApplyAimJitter(pWeapon);
                    }
                    pWeapon->pull_trigger();
                }
            }
            else if (mCar)
            {
                if (mObjective == objectives_enum::kill_char_any_means_19)
                {
                    Ped::AimRoofGun();
                }
                if (mBf21C.bAttacking)
                {
                    pWeapon->pull_trigger();
                }
            }
            else
            {
                pWeapon->pull_trigger();
            }
        }
        else if (mGameObject)
        {
            if (mPedType == ped_type::player_2)
            {
                Ped::HandleClosePedInteraction();
            }
        }
    }
    else if (pWeapon)
    {
        pWeapon->ChuckThrowable();
    }
}

// 9.6f 0x434E60
MATCH_FUNC(0x46f490)
Weapon* Ped::ChooseAttackWeapon()
{
    Car* pCar;

    switch (this->mOccupation)
    {
        case ped_ocupation_enum::police:
        case ped_ocupation_enum::walking_guard_29:
            if (!mInternalTargetPed->PedTypeIs(ped_type::player_2))
            {
                this->mBf21C.bWeaponChosen = true;
                return this->mpWeapon;
            }

            pCar = this->mInternalTargetPed->mCar;
            if (pCar)
            {
                if (pCar->GetVelocity() > kFpPoint01_678624)
                {
                    this->mBf21C.bWeaponChosen = true;
                    return this->mpWeapon;
                }
                this->mBf21C.bNoWeapon = true;
                return NULL;
            }

            if (gDistanceToTarget < kFpTwo_678658 + kFpOne_678798)
            {
                ++gNumPolicePedsInRangeScreen;
            }

            if (gPolice_7B8->mPolicePedsInRangeScreen < 2u)
            {
                this->mBf21C.bNoWeapon = true;
                this->mHitTargetPed = NULL;
                return NULL;
            }

            if (!gPolice_7B8->mLastFiringEmergencyPed || gPolice_7B8->mLastFiringEmergencyPed == this)
            {
                return this->mSelectedWeapon;
            }
            this->mHitTargetPed = NULL;
            this->mBf21C.bNoWeapon = true;
            return NULL;

        case ped_ocupation_enum::fbi:
            if (this->mInternalTargetPed->mCar || gDistanceToTarget > kFpHalf_67853C)
            {
                this->mBf21C.bWeaponChosen = true;
                return this->mpWeapon;
            }
            return this->mSelectedWeapon;

        case ped_ocupation_enum::roadblock_cop_37:
            if (!mInternalTargetPed->PedTypeIs(ped_type::player_2) || this->mInternalTargetPed->mCar)
            {
                this->mBf21C.bWeaponChosen = true;
                return this->mpWeapon;
            }
            return this->mSelectedWeapon;

        default:
            break;
    }
    return this->mSelectedWeapon;
}

MATCH_FUNC(0x46f600)
void Ped::ForceWeapon(s32 weapon_kind)
{
    RemovePedWeapons();
    if (weapon_kind != 28)
    {
        Weapon* pWeapon = gWeapon_8->allocate_ped_weapon(weapon_kind, this, 99u);
        this->mSelectedWeapon = pWeapon;
        pWeapon->SetTargetHit(1);
        if (mSelectedWeapon->IsExplosiveWeapon())
        {
            GiveWeapon(weapon_type::pistol);
        }
    }
}

MATCH_FUNC(0x46f650)
void Ped::GiveWeapon(s32 weapon_kind)
{
    RemoveSecondaryWeapon();
    Weapon* pWeapon = gWeapon_8->allocate_ped_weapon(weapon_kind, this, 99u);
    this->mpWeapon = pWeapon;
    pWeapon->SetTargetHit(1);
}

MATCH_FUNC(0x46f680)
void Ped::ApplyGangRespectForKill(const Ped* pPed)
{
    if (mpGang)
    {
        if (mpGang->DoesKillAffectRespect())
        {
            if (mDeathCause != ped_death_cause::run_over_by_stolen_car_3 && mDeathCause != ped_death_cause::run_over_1)
            {
                mpGang->ApplyKillRespectChange(pPed->mPlayer->get_idx(), 5);
            }
            else
            {
                mpGang->ApplyKillRespectChange(pPed->mPlayer->get_idx(), 1);
            }
        }
    }
    else
    {
        if (mDummyGang && mDummyGang->mKillAffectsRespect)
        {
            if (mDeathCause != ped_death_cause::run_over_by_stolen_car_3 && mDeathCause != ped_death_cause::run_over_1)
            {
                mDummyGang->ApplyKillRespectChange(pPed->mPlayer->get_idx(), 5);
            }
            else
            {
                mDummyGang->ApplyKillRespectChange(pPed->mPlayer->get_idx(), 1);
            }
        }
    }
}

MATCH_FUNC(0x46f720)
void Ped::UpdateStatsForKiller()
{

    s32 ped_id;
    Ped* pKillerPed;
    Player* pPlayerIter;
    Ped* pPlayerPed;
    Ped* pPedKiller;
    PedGroup* pGroup;

    ped_id = this->mKillerId;
    this->mPedKiller = NULL;
    if (ped_id)
    {
        pKillerPed = gPedManager->PedById(ped_id);
        this->mPedKiller = pKillerPed;
        if (pKillerPed)
        {
            if (pKillerPed->mThreatReaction != threat_reaction_enum::react_as_emergency_1 && pKillerPed->PedTypeIs(ped_type::player_2))
            {
                if (gCrimeReportQueue->ShouldReportPedCrime(this, this->mPedKiller->mPlayer))
                {
                    if (this->mpGang || this->mDummyGang)
                    {
                        mPedKiller->add_wanted_points(1); // gang guy killed
                    }
                    else if (this->mThreatReaction == threat_reaction_enum::react_as_emergency_1)
                    {
                        mPedKiller->add_wanted_points(500);
                    }
                    else
                    {
                        mPedKiller->add_wanted_points(100);
                    }
                }
            }
            if (bStartNetworkGame)
            {
                if (mPedKiller->PedTypeIs(ped_type::player_2))
                {
                    if (PedTypeIs(ped_type::player_2))
                    {
                        gGameSession.UpdateFrags(this->mPedKiller->mPlayer->get_idx(),
                                                                  this->mPlayer->get_idx());
                        gHud_2B00->mMpMessage.AnnounceKill(this->mPedKiller->mPlayer,
                                                                         this->mPlayer);
                    }
                }
                else if (PedTypeIs(ped_type::player_2))
                {
                    pPlayerIter = NULL;
                    if (this->mPedKiller->mPedGroup)
                    {
                        for (pPlayerIter = gGame_0x40->IterateFirstPlayer(); pPlayerIter != NULL;
                             pPlayerIter = gGame_0x40->IterateNextPlayer())
                        {
                            pPlayerPed = pPlayerIter->mPlayerPed;
                            if (pPlayerPed)
                            {
                                if (pPlayerPed->mPedGroup == this->mPedKiller->mPedGroup &&
                                    pPlayerIter != this->mPlayer)
                                {
                                    gGameSession.UpdateFrags(pPlayerIter->get_idx(),
                                                                              this->mPlayer->get_idx());
                                    gHud_2B00->mMpMessage.AnnounceKill(pPlayerIter, this->mPlayer);
                                    break;
                                }
                            }
                        }
                    }

                    if (!pPlayerIter)
                    {
                        gGameSession.UpdateFrags(this->mPlayer->get_idx(),
                                                                  this->mPlayer->get_idx());
                        if (!mPedKiller->IsLawEnforcement())
                        {
                            gHud_2B00->mMpMessage.AnnounceKill(this->mPlayer, this->mPlayer);
                        }
                    }
                }
            }
            pPedKiller = this->mPedKiller;
            if (this != pPedKiller)
            {
                if (pPedKiller->PedTypeIs(ped_type::player_2))
                {
                    mPedKiller->mPlayer->mScores.AwardPedKilledScore(this, this->mPedKiller);
                    ApplyGangRespectForKill(this->mPedKiller);
                }
                else
                {
                    pGroup = this->mPedKiller->mPedGroup;
                    if (pGroup)
                    {
                        Ped* pLocalPed = gGame_0x40->mLocalPlayer->mPlayerPed;
                        if (pLocalPed)
                        {
                            if (pGroup == pLocalPed->mPedGroup)
                            {
                                ApplyGangRespectForKill(pLocalPed);
                            }
                        }
                    }
                }
            }
        }
    }
    if (bStartNetworkGame)
    {
        if (!this->mPedKiller && PedTypeIs(ped_type::player_2))
        {
            gGameSession.UpdateFrags(this->mPlayer->get_idx(), this->mPlayer->get_idx());
            gHud_2B00->mMpMessage.AnnounceKill(this->mPlayer, this->mPlayer);
        }
    }
}

MATCH_FUNC(0x46f9d0)
void Ped::Kill()
{
    if (mBf21C.bOnFire)
    {
        if (mGameObject)
        {
            mGameObject->mAnimationState = Char_Anim_state::Charred_Corpse_21;
        }
        PutOutFire();
    }

    if (!isDead())
    {
        SetObjective2(objectives_enum::no_obj_0);
        if (mPedState2 == ped_state_2::lying_on_floor_22)
        {
            mPedState1 = ped_state_1::dead_9;
            mPedState2 = ped_state_2::ped2_dead_15;
        }
        else
        {
            Ped::ChangeNextPedState1(ped_state_1::dead_9);
            Ped::ChangeNextPedState2(ped_state_2::ped2_dead_15);
        }

        SetVoiceEventIfNotOnFire(4);

        UpdateStatsForKiller();

        switch (mOccupation)
        {
            case ped_ocupation_enum::paramedic_23:
                if (gAmbulance_110->HandlePedDeath(this))
                {
                    return;
                }
                break;

            case ped_ocupation_enum::police:
            case ped_ocupation_enum::swat:
            case ped_ocupation_enum::fbi:
                if (!PedTypeIs(ped_type::special_ped_4) || !gPolice_7B8->TryReplaceCrewLeaderOnDeath(this))
                {
                    break;
                }
                return;

            case ped_ocupation_enum::deallocate_on_death_2:
                Deallocate();
                return;

            case ped_ocupation_enum::elvis:
            case ped_ocupation_enum::elvis_leader:
                if (mPedGroup)
                {
                    if (!mPedKiller)
                    {
                        mPedKiller = this;
                    }
                    mPedGroup->DisbandGroupDueToAttack(mPedKiller);
                }
                break;

            default:
                break;
        }

        if (mGameObject)
        {
            SetStateInitPending();
            if (mPedType == ped_type::player_2)
            {
                if (mPedGroup)
                {
                    mPedGroup->DestroyGroup();
                }
            }
            else if (!mPedGroup && mPedType != ped_type::script_created_5)
            {
                SetObjective(objectives_enum::dead_patient_28);
            }
            mGameObject->SetSpriteNum(6);
        }
        else if (mPedType == ped_type::player_2)
        {
            if (mCar)
            {
                mPlayer->UnloadCarWeapons();
            }
            if (mPedGroup)
            {
                mPedGroup->DestroyGroup();
            }
            mBf21C.bActive = false;
        }
        else
        {
            Deallocate();
        }

        if (!mPedGroup || mThreatReaction == threat_reaction_enum::react_as_emergency_1)
        {
            set_health(0);
            mWantedPoints = cop_level_ped_enum::cop_0_stars;
            mBf21C.bAttacking = false;
            if (isDead() && mPedState2 == ped_state_2::ped2_dead_15)
            {
                SpawnWeaponOnDeath();
            }

            gThreateningPedsList.RemovePed_471240(this);

            if (bDo_blood)
            {
                if (!mCar && mGameObject->mAnimationState != Char_Anim_state::Electrocuted_17)
                {
                    gParticle_8->SpawnBlood(mCam.x, mCam.y, mCam.z);
                }
            }
        }
        else
        {
            mPedGroup->RemovePed(this);
        }
    }
}

MATCH_FUNC(0x46fc70)
void Ped::AddThreateningPedToList()
{
    bThreateningPedAdded = 1;
    gThreateningPedsList.AddPedToFrontIfMissing(this);
}

MATCH_FUNC(0x46fc90)
void Ped::HandleShootingAtCar(Car* pCar, s32 model)
{
    Weapon* pWeapon;
    if (mBf21C.bWeaponChosen)
    {
        pWeapon = mpWeapon;
    }
    else
    {
        pWeapon = mSelectedWeapon;
    }

    if (pWeapon)
    {
        if (pCar->GetVelocity() == kFpZero_678660)
        {
            Ped* pDriver = pCar->mDriver;
            if (pDriver)
            {
                if (pDriver->PedTypeIs(ped_type::dummy_3))
                {
                    Ped* pDriver2 = pCar->mDriver;
                    if (pDriver2->get_objective() != objectives_enum::leave_car_36)
                    {
                        pDriver2->SpawnDriverRunAway(pCar, this);
                    }
                }
            }
        }

        if (PedTypeIs(ped_type::player_2))
        {
            mPlayer->mScores.UpdateAccuracyCount(accuracy_event::shot_at_car_4, model, NULL);
            return;
        }

        Fix16 dist_to_cam;
        dist_to_cam = Fix16::MaxAbsDistance(mCam.x,
                                                        mCam.y,
                                                        pCar->mCarSprite->mXy.x,
                                                        pCar->mCarSprite->mXy.y);

        if (pCar == mTargetToEnter)
        {
            pWeapon->SetTargetHit(0);
        }
        else if (mInternalTargetPed && mInternalTargetPed->mCar && mInternalTargetPed->mCar == pCar)
        {
            pWeapon->SetTargetHit(0);
        }
        else
        {
            Fix16 max_range;
            max_range = kFpHalf + kFpOne_678798;
            if (dist_to_cam < max_range && pCar->GetVelocity() < kFpPoint02)
            {
                pWeapon->SetTargetHit(1);
                return;
            }

            if (!pWeapon->IsExplosiveWeapon())
            {
                pWeapon->SetTargetHit(0);
            }

            if (mInternalTargetPed && mInternalTargetPed->get_car() && pCar == mInternalTargetPed->get_car())
            {
                pWeapon->SetTargetHit(0);
            }
        }
    }
}

MATCH_FUNC(0x46fe20)
void Ped::ProcessWeaponHitResponse(Object* pObj)
{
    Weapon* pWeapon;
    Fix16 dist;
    if (mBf21C.bWeaponChosen)
    {
        pWeapon = mpWeapon;
    }
    else
    {
        pWeapon = mSelectedWeapon;
    }

    if (pWeapon && !Ped::PedTypeIs(ped_type::player_2))
    {
        dist = Fix16::MaxAbsDistance(mCam.x, mCam.y, pObj->GetX(), pObj->GetY());

        if (pObj == mInternalTargetObject)
        {
            pWeapon->SetTargetHit(0);
        }
        else if (dist < kFpTwo_678658)
        {
            pWeapon->SetTargetHit(1);
        }
        else if (!pWeapon->IsExplosiveWeapon())
        {
            pWeapon->SetTargetHit(0);
        }
    }
}

MATCH_FUNC(0x46ff00)
void Ped::NotifyWeaponHit(Fix16 xpos, Fix16 ypos, s32 model)
{
    Weapon* pWeapon;

    if (mBf21C.bWeaponChosen)
    {
        pWeapon = mpWeapon;
    }
    else
    {
        pWeapon = mSelectedWeapon;
    }

    if (pWeapon)
    {
        if (PedTypeIs(ped_type::player_2))
        {
            mPlayer->mScores.UpdateAccuracyCount(accuracy_event::weapon_hit_1, model, NULL);
        }
        else
        {
            if (Fix16::MaxAbsDistance(mCam.x, mCam.y, xpos, ypos) < kFpTwo_678658)
            {
                pWeapon->SetTargetHit(1);
            }
            else
            {
                pWeapon->SetTargetHit(0);
            }
        }
    }
}

MATCH_FUNC(0x46fff0)
void Ped::HandleWeaponFireEnd(s32 model)
{
    if (PedTypeIs(ped_type::player_2))
    {
        mPlayer->mScores.UpdateAccuracyCount(accuracy_event::fire_ended_0, model, NULL);
    }

    if (this->mBf21C.bWeaponChosen)
    {
        Weapon* pWeapon = this->mpWeapon;
        if (pWeapon)
        {
            pWeapon->SetTargetHit(0);
        }
    }
    else
    {
        Weapon* pSelectedWeapon = this->mSelectedWeapon;
        if (pSelectedWeapon)
        {
            pSelectedWeapon->SetTargetHit(0);
        }
    }
}

MATCH_FUNC(0x470050)
void Ped::AimRoofGun()
{
    AttachedSprite* pTurret = NULL;
    if (mCar->IsFireTruck())
    {
        pTurret = mCar->mAttachments.GetSpriteForModel(objects::water_cannon_114);
    }
    else if (mCar->IsTank())
    {
        pTurret = mCar->mAttachments.GetSpriteForModel(objects::tank_cannon_148);
    }
    else if (mCar->IsGunJeep())
    {
        pTurret = mCar->mAttachments.GetSpriteForModel(248);
    }

    Fix16 x = pTurret->mpSprite->mXy.x;
    Fix16 y = pTurret->mpSprite->mXy.y;
    Ped* pTargetPed = this->mObjectiveTargetPed;
    Ang16 aim_angle;
    aim_angle = Fix16::atan2_fixed(pTargetPed->get_cam_y() - y, pTargetPed->get_cam_x() - x);

    this->mBf21C.bAttacking = false;
    this->mBf21C.bUseCarWeapon = true;

    if (mCar->RotateRoofObjectTowardTarget(aim_angle))
    {
        if (mObjectiveTargetPed->PedTypeIs(ped_type::player_2))
        {
            if (!this->mCar->mLastSeenTimer)
            {
                this->mBf21C.bAttacking = true;
            }
        }
        else if (gDistanceToTarget < kFpFour)
        {
            this->mBf21C.bAttacking = true;
        }
    }
    else
    {
        this->mBf21C.bAttacking = false;
    }
}

MATCH_FUNC(0x470160)
void Ped::add_wanted_points(s16 wanted_amount)
{
    mWantedPoints += wanted_amount;

    if (mWantedPoints > cop_level_ped_enum::cop_6_stars)
    {
        mWantedPoints = cop_level_ped_enum::cop_6_stars;
    }
    else if (mWantedPoints < 0)
    {
        mWantedPoints = cop_level_ped_enum::cop_0_stars;
    }

    s16 max_stars = gPolice_7B8->mMaxWantedStars;
    if (get_wanted_star_count() >= max_stars)
    {
        set_wanted_star_count(static_cast<u8>(max_stars));
    }
}

MATCH_FUNC(0x4701d0)
bool Ped::IsNearestSpriteACar() const
{
    Sprite* pSprite = gSpriteGrid_1->FindCollidingSpritePreferType(this->mGameObject->mSpritePtr, sprite_types_enum::none_0);
    if (pSprite)
    {
        return (pSprite->GetType() != sprite_types_enum::car_2) ? false : true;
    }
    return false;
}

MATCH_FUNC(0x470200)
void Ped::StartPedWalking(Fix16 xpos, Fix16 ypos, Fix16 zpos)
{
    Ped::AllocCharB4(xpos, ypos, zpos);
    SetRemap(get_remap());
    if (mPedType == ped_type::player_2)
    {
        Ped::ChangeNextPedState2(ped_state_2::ped2_walking_0);
        Ped::ChangeNextPedState1(ped_state_1::walking_0);
        mGameObject->SetVelocityByRef_433920(kFpZero_678438);
    }
    else
    {
        Ped::ChangeNextPedState2(ped_state_2::ped2_walking_0);
        Ped::ChangeNextPedState1(ped_state_1::walking_0);
    }
    mCar = NULL;
}

MATCH_FUNC(0x4702d0)
void Ped::BecomeLeaderOfGroup(const Ped* pPed)
{
    PedGroup* pPedGroup = pPed->mPedGroup;
    pPedGroup->replace_leader(this);
    this->mPedGroup = pPedGroup;
}

MATCH_FUNC(0x470300)
void Ped::BecomeDummyOnPlayerDisconnect()
{
    mPlayer = NULL;
    mOccupation = ped_ocupation_enum::dummy;
    mPedType = ped_type::dummy_3;
    Car* pCar = mCar;
    if (pCar)
    {
        // NOTE: OG tail calls a function chunk here
        pCar->SetDummyControl();
    }
}

MATCH_FUNC(0x4702A0)
void Ped::PushPatrolPoint(s8 x, s8 y, s8 z)
{
    // Get a free patrol point
    PatrolPoint* pIter = this->mPatrolRoute->mPoints;
    while (pIter->mX)
    {
        ++pIter;
    }

    // And populate it
    pIter->mX = x;
    pIter->mY = y;
    pIter->mZ = z;
}

MATCH_FUNC(0x470f00)
s32 Ped::IsInTrain() const
{
    Car* pCar = this->mCar;
    if (pCar)
    {
        const s32 info_idx = pCar->mCarInfoIdx;
        if (info_idx == car_model_enum::TRAIN || info_idx == car_model_enum::TRAINCAB || info_idx == car_model_enum::TRAINFB || info_idx == car_model_enum::boxcar)
        {
            return 1;
        }
    }
    return 0;
}

EXPORT void Ped::FleeCharAnyMeansTillSafe_Nullsub()
{
    ;
}

EXPORT void Ped::FleeCharAnyMeansAlways_Nullsub()
{
    ;
}

EXPORT void Ped::KillCar_Nullsub()
{
    ;
}

EXPORT void Ped::Objective50_Nullsub()
{
    ;
}

EXPORT void Ped::State30_Nullsub()
{
    ;
}
