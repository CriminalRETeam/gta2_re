#include "Ped.hpp"
#include "Ambulance_110.hpp"
#include "CarInfo_808.hpp"
#include "CarPhysics_B0.hpp"
#include "Car_BC.hpp"
#include "Char_Pool.hpp"
#include "frosty_pasteur_0xC1EA8.hpp"
#include "Game_0x40.hpp"
#include "Gang.hpp"
#include "Garage_48.hpp"
#include "Globals.hpp"
#include "Hamburger_500.hpp"
#include "Hud.hpp"
#include "Marz_1D7E.hpp"
#include "Object_5C.hpp"
#include "Orca_2FD4.hpp"
#include "Particle_8.hpp"
#include "PedGroup.hpp"
#include "Player.hpp"
#include "Police_7B8.hpp"
#include "PublicTransport.hpp"
#include "PurpleDoom.hpp"
#include "RouteFinder.hpp"
#include "Shooey_CC.hpp"
#include "Taxi_4.hpp"
#include "TrafficLights_194.hpp"
#include "Varrok_7F8.hpp"
#include "Weapon_30.hpp"
#include "Weapon_8.hpp"
#include "Wolfy_3D4.hpp"
#include "char.hpp"
#include "debug.hpp"
#include "error.hpp"
#include "lucid_hamilton.hpp"
#include "map_0x370.hpp"
#include "rng.hpp"
#include "sprite.hpp"
#include "youthful_einstein.hpp"
#include "CarAI_78.hpp"
#include "winmain.hpp"

// =================
DEFINE_GLOBAL_INIT(s8, byte_61A8A3, 1, 0x61A8A3);
DEFINE_GLOBAL_INIT(Ang16, kAng0_6FDB34, Ang16(0), 0x6FDB34);
DEFINE_GLOBAL_INIT(Ang16, gDummyPedAng_6787A8, Ang16(0), 0x6787A8);
DEFINE_GLOBAL_INIT(Ang16, gPedAng_6787A0, Ang16(0), 0x6787A0);
DEFINE_GLOBAL_INIT(Fix16, kFpThree_67866C, Fix16(0xC000, 0), 0x67866C); // TODO: Fix16? Static init to, 0xC000, 0xUNKNOWN);
DEFINE_GLOBAL_INIT(s32, gPedId_61A89C, 0x7, 0x61A89C);
DEFINE_GLOBAL_INIT(u8, gNumberMuggersSpawned_6787CA, 0, 0x6787CA);
DEFINE_GLOBAL_INIT(u8, gNumberCarThiefsSpawned_6787CB, 0, 0x6787CB);
DEFINE_GLOBAL_INIT(u8, gNumberElvisLeadersSpawned_6787CC, 0, 0x6787CC);
DEFINE_GLOBAL_INIT(u8, gNumberWalkingCopsSpawned_6787CD, 0, 0x6787CD);
DEFINE_GLOBAL(u8, gNewTaxiCustomersThisTick_6787D2, 0x6787D2);
DEFINE_GLOBAL_INIT(u8, byte_61A8A0, 1, 0x61A8A0);
DEFINE_GLOBAL(u8, gNumDummyChars_6787E2, 0x6787E2);
DEFINE_GLOBAL(u8, gNumScriptCreatedPeds_6787E3, 0x6787E3);
DEFINE_GLOBAL(u8, gNumEmergencyPeds_6787E4, 0x6787E4);
DEFINE_GLOBAL(u8, gTargetSearchMode_6787D7, 0x6787D7);
DEFINE_GLOBAL(u8, byte_6787D4, 0x6787D4);
DEFINE_GLOBAL(u8, byte_678554, 0x678554);
DEFINE_GLOBAL(u8, byte_6787D8, 0x6787D8);
DEFINE_GLOBAL(u8, byte_6787D9, 0x6787D9);
DEFINE_GLOBAL_INIT(u8, byte_61A8A4, 1, 0x61A8A4);
DEFINE_GLOBAL(u8, byte_6787C4, 0x6787C4);
DEFINE_GLOBAL(s16, gNumPedsCrossingRoad_6787D0, 0x6787D0);
DEFINE_GLOBAL(s16, word_6787F2, 0x6787F2);
DEFINE_GLOBAL(u16, gNumPedsUpdated_6787E0, 0x6787E0);
DEFINE_GLOBAL(Ped*, gSearchingPed_6787DC, 0x6787DC);
DEFINE_GLOBAL_INIT(Fix16, kFpZero_678660, Fix16(0), 0x678660);
DEFINE_GLOBAL_INIT(Fix16, kFpZero_678438, kFpZero_678660, 0x678438);
DEFINE_GLOBAL_INIT(Fix16, gDistanceToTarget_678750, kFpZero_678660, 0x678750);
DEFINE_GLOBAL_INIT(Fix16, kFpSix_678678, Fix16(98304, 0), 0x678678);
DEFINE_GLOBAL_INIT(Fix16, kFpSix_678520, kFpSix_678678, 0x678520);
DEFINE_GLOBAL_INIT(Fix16, kFpFour_678670, Fix16(4), 0x678670);
DEFINE_GLOBAL_INIT(Fix16, kFpOne64th_6784C4, Fix16(256, 0), 0x6784C4);
DEFINE_GLOBAL_INIT(Fix16, kFpOneSixteenth_678448, kFpFour_678670* kFpOne64th_6784C4, 0x678448);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_678790, kFpOne64th_6784C4 * 32, 0x678790);
DEFINE_GLOBAL_INIT(Fix16, kFpOneEighth_6784E8, kFpOne64th_6784C4 * 8, 0x6784E8);
DEFINE_GLOBAL_INIT(Fix16, kFpOneThirtySecond_6784CC, kFpOne64th_6784C4 * 2, 0x6784CC);
DEFINE_GLOBAL_INIT(Fix16, kFpOneThirtySecond_678434, kFpOneThirtySecond_6784CC, 0x678434);
DEFINE_GLOBAL_INIT(Fix16, kFpOne256th_678620, kFpOne64th_6784C4 / kFpFour_678670, 0x678620);
DEFINE_GLOBAL_INIT(Fix16, kFpQuarter_678788, kFpOne64th_6784C4 * 16, 0x678788);
DEFINE_GLOBAL_INIT(Fix16, kFpOne_678664, Fix16(0x4000, 0), 0x678664);
DEFINE_GLOBAL_INIT(Fix16, kFpOne_6785EC, kFpOne_678664, 0x6785EC);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint01_678624, Fix16(0xA3, 0), 0x678624);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_67853C, Fix16(0x2000, 0), 0x67853C);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint05_678634, Fix16(0x333, 0), 0x678634);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint1_678480, Fix16(0x666, 0), 0x678480);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint9_6784A4, Fix16(0x3999, 0), 0x6784A4);
DEFINE_GLOBAL_INIT(Ang16, kAng45_6784FC, Ang16(180), 0x6784FC);
DEFINE_GLOBAL_INIT(Ang16, kAng180_678590, Ang16(720), 0x678590);
DEFINE_GLOBAL_INIT(Fix16, kFpThreeThirtySeconds_6784DC, kFpOne64th_6784C4 * 6, 0x6784DC);
DEFINE_GLOBAL_INIT(Fix16, kFpTwo_678668, Fix16(2), 0x678668);
DEFINE_GLOBAL_INIT(Fix16, gSpawnJitterScale_678618, Fix16(256, 0), 0x678618);
DEFINE_GLOBAL_INIT(Fix16, kFpQuarter_678484, Fix16(0x1000, 0), 0x678484);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint2_678488, Fix16(0xCCC, 0), 0x678488);
DEFINE_GLOBAL(Ped*, gLastProcessedPed_6787C0, 0x6787C0);
DEFINE_GLOBAL_INIT(Fix16, gDummyW_678530, kFpPoint2_678488, 0x678530);
DEFINE_GLOBAL_INIT(Fix16, gDummyZ_67841C, kFpQuarter_678484, 0x67841C);
DEFINE_GLOBAL(Object_2C*, dword_678558, 0x678558);
DEFINE_GLOBAL(char_type, gNumberBusCustomers_6787D3, 0x6787D3);
DEFINE_GLOBAL_INIT(Fix16, kFpOneSixth_678504, Fix16(0xAAA, 0), 0x678504);
DEFINE_GLOBAL_INIT(Fix16, kFpQuarter_678574, kFpQuarter_678484, 0x678574);
DEFINE_GLOBAL_INIT(Fix16, kFpOneEighth_67845C, kFpQuarter_678574 / kFpTwo_678668, 0x67845C);
DEFINE_GLOBAL_INIT(Fix16, kFpOne_678798, kFpOne64th_6784C4 * 64, 0x678798);
DEFINE_GLOBAL_INIT(Fix16, kFpTwo_678658, kFpOne64th_6784C4 * 128, 0x678658);
DEFINE_GLOBAL_INIT(Fix16, kFpFour_678680, kFpOne64th_6784C4 * 256, 0x678680);
DEFINE_GLOBAL_INIT(Fix16, kFpOne64th_678430, kFpOne64th_6784C4, 0x678430);
DEFINE_GLOBAL_INIT(Fix16, kFp9999_678524, Fix16(0x9C3C000, 0), 0x678524);

DEFINE_GLOBAL_INIT(s16, k_word_678656, 40, 0x678656);
DEFINE_GLOBAL(u8, gNumberArmedGangMembers_6787CE, 0x6787CE);

DEFINE_GLOBAL_INIT(Fix16, kFpPoint8_6784A0, Fix16(0x3333, 0), 0x6784A0);
DEFINE_GLOBAL_INIT(Fix16, kFpOne128th_6784BC, kFpOne64th_6784C4 / kFpTwo_678668, 0x6784BC);
DEFINE_GLOBAL_INIT(Fix16, kFpThreeSixtyFourths_678444, kFpThree_67866C * kFpOne64th_6784C4, 0x678444);
DEFINE_GLOBAL_INIT(Fix16, kFpFiveSixteenths_678784, kFpOne64th_6784C4 * 20, 0x678784);

DEFINE_GLOBAL_INIT(Ang16, kAng10_6784C8, Ang16(40), 0x6784C8);
DEFINE_GLOBAL_INIT(Ang16, kAng16_6784E4, Ang16(64), 0x6784E4);
DEFINE_GLOBAL_INIT(Ang16, word_6784F0, Ang16(0), 0x6784F0);

DEFINE_GLOBAL_INIT(Ang16, kAng90_678502, Ang16(360), 0x678502);
DEFINE_GLOBAL_INIT(Ang16, kAng270_6785D0, Ang16(1080), 0x6785D0);
DEFINE_GLOBAL_INIT(Ang16, kAng225_6786B8, Ang16(900), 0x6786B8);
DEFINE_GLOBAL_INIT(Ang16, kAng45_6784E2, Ang16(180), 0x6784E2);
DEFINE_GLOBAL_INIT(Ang16, kAng315_6785A8, Ang16(1260), 0x6785A8);
DEFINE_GLOBAL_INIT(Ang16, kAng135_67844C, Ang16(540), 0x67844C);

DEFINE_GLOBAL_INIT(Fix16, kFpPoint2_67856C, kFpPoint2_678488, 0x67856C);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint1_678428, kFpPoint1_678480, 0x678428);

DEFINE_GLOBAL_INIT(Fix16, kFpFiveThirtySeconds_678778, kFpOne64th_6784C4 * 10, 0x678778);
DEFINE_GLOBAL_INIT(Fix16, kFpThreeQuarters_678794, kFpOne64th_6784C4 * 48, 0x678794);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint02_678630, Fix16(0x147, 0), 0x678630);

DEFINE_GLOBAL_INIT(Ang16, kAng180_6785A6, Ang16(0x2D0), 0x6785A6); // TODO: Init via 0x45FCB0 func
DEFINE_GLOBAL_INIT(Fix16, kFpThreeSixteenths_678780, kFpOne64th_6784C4 * 12, 0x678780);
DEFINE_GLOBAL_INIT(Ang16, kAng90_6784B0, Ang16(360), 0x6784B0); // TODO: Init via 0x45FAA0 func

EXTERN_GLOBAL(u8, bHaveThreateningPeds_6787DA);
EXTERN_GLOBAL(u8, byte_61A8A1);

// TODO
EXTERN_GLOBAL(s32, bStartNetworkGame_7081F0);

// TODO: move with CarDoorAlignmentSolver_545AF0
EXTERN_GLOBAL(UnknownList, gPixelsToFix16_6F6850);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint1_6FD824, Fix16(0x666, 0), 0x6FD824);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint06_6FD9C8, Fix16(0x3D7, 0), 0x6FD9C8);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint02_6FD9AC, Fix16(0x147, 0), 0x6FD9AC);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint08_6FD9D4, Fix16(0x51E, 0), 0x6FD9D4);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint04_6FD9B8, Fix16(0x28F, 0), 0x6FD9B8);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint3_6FD830, Fix16(0x1333, 0), 0x6FD830);

DEFINE_GLOBAL_INIT(Fix16, kFpFive_678674, Fix16(5), 0x678674);
DEFINE_GLOBAL_INIT(Fix16, kFpThreeEighths_67878C, kFpOne64th_6784C4 * 24, 0x67878C);
DEFINE_GLOBAL_INIT(Fix16, kFpEight_6786C0, kFpOne64th_6784C4 * 512, 0x6786C0);
DEFINE_GLOBAL_INIT(Fix16, kFpFiveSixtyFourths_67843C, kFpFive_678674 * kFpOne64th_6784C4, 0x67843C);

// TODO: these are defined in char.cpp
EXTERN_GLOBAL(Fix16, gCharB4_WorldCollisionOffset_6FD8D8);
EXTERN_GLOBAL(Fix16, dword_6FDB20);
EXTERN_GLOBAL(Fix16, kFP16Zero_6FD9E4);
EXTERN_GLOBAL(Fix16, dword_6FD82C);
EXTERN_GLOBAL(Fix16, dword_6FD9B0);

EXTERN_GLOBAL(Ang16, kAng180_6FD936);
EXTERN_GLOBAL(Ang16, kAng90_6FD854);

EXTERN_GLOBAL(Ang16, gSpawnRotationLeft_6786E0);
EXTERN_GLOBAL(Ang16, gSpawnRotationTop_6787B0);
EXTERN_GLOBAL(Ang16, gSpawnRotationRight_678578);
EXTERN_GLOBAL(Ang16, gSpawnRotationBottom_678540);

// TODO: move
// https://decomp.me/scratch/BzcQt
WIP_FUNC(0x545AF0)
EXPORT void __stdcall CarDoorAlignmentSolver_545AF0(u8 animPhase, Car_BC* pCar, u8 doorId, Fix16& outX, Fix16& outY, Ang16& outAng)
{
    WIP_IMPLEMENTED;
    // This func is really get_car_remap?? shouldnt be get_car_door_info?
    u8* car_door_info_array = gGtx_0x106C_703DD4->get_car_remap_5AA3D0(pCar->field_84_car_info_idx);
    Fix16 offset = gCharB4_WorldCollisionOffset_6FD8D8;

    Fix16 x_pos;
    Fix16 y_pos;
    s8 x_in_scale = car_door_info_array[2 * doorId + 1];
    x_pos = gPixelsToFix16_6F6850.SignedPixelsToFix16_41FE70(x_in_scale);

    s8 y_in_scale = car_door_info_array[2 * doorId + 2];
    y_pos = gPixelsToFix16_6F6850.SignedPixelsToFix16_41FE70(y_in_scale);

    bool bUnk = false;

    if (x_pos >= dword_6FDB20)
    {
        x_pos -= dword_6FDB20;
        bUnk = true;
    }
    else
    {
        if (x_pos <= -dword_6FDB20)
        {
            x_pos += dword_6FDB20;
            bUnk = true;
        }
    }

    switch (doorId)
    {
        case 0:
        case 2:
            if (x_pos == kFP16Zero_6FD9E4)
            {
                y_pos -= offset;
                switch ((u8)animPhase)
                {
                    case 0:
                        y_pos += dword_6FD82C;
                        break;
                    case 1:
                        y_pos += kFpPoint1_6FD824 + kFpPoint06_6FD9C8;
                        break;
                    case 2:
                        y_pos += kFpPoint1_6FD824 + kFpPoint02_6FD9AC;
                        break;
                    case 3:
                        y_pos += kFpPoint08_6FD9D4;
                        break;
                    case 4:
                        y_pos += kFpPoint04_6FD9B8;
                        break;
                    case 6:
                        y_pos -= kFpPoint04_6FD9B8;
                        break;
                    case 7:
                        y_pos -= kFpPoint06_6FD9C8;
                        break;
                    default:
                        break;
                }
                // Original adds field_0 + kAng180 raw and calls the out-of-line Normalize_406C20 (tail merged)
                outAng = Ang16(pCar->field_50_car_sprite->field_0.rValue + kAng180_6FD936.rValue).Normalized_406C20();
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
                if (bUnk)
                {
                    y_pos -= kFpPoint1_6FD824;
                    switch ((u8)animPhase)
                    {
                        case 0:
                            x_pos -= dword_6FD82C;
                            break;
                        case 1:
                            x_pos -= kFpPoint1_6FD824 + kFpPoint06_6FD9C8;
                            break;
                        case 2:
                            x_pos -= kFpPoint1_6FD824 + kFpPoint02_6FD9AC;
                            break;
                        case 3:
                            x_pos -= kFpPoint08_6FD9D4;
                            break;
                        case 4:
                            x_pos -= kFpPoint04_6FD9B8;
                            break;
                        case 6:
                            x_pos += kFpPoint04_6FD9B8;
                            break;
                        case 7:
                            x_pos += kFpPoint08_6FD9D4;
                            break;
                        default:
                            break;
                    }
                    outAng = Ang16(pCar->field_50_car_sprite->field_0.rValue + kAng90_6FD854.rValue).Normalized_406C20();
                }
                else
                {
                    switch ((u8)animPhase)
                    {
                        case 0:
                            y_pos -= kFpPoint1_6FD824;
                            x_pos -= kFpPoint1_6FD824;
                            outAng = pCar->field_50_car_sprite->field_0;
                            break;
                        case 1:
                            y_pos -= kFpPoint1_6FD824;
                            x_pos -= dword_6FD9B0;
                            outAng = pCar->field_50_car_sprite->field_0;
                            break;
                        case 2:
                            y_pos -= kFpPoint1_6FD824;
                            outAng = pCar->field_50_car_sprite->field_0;
                            break;
                        case 3:
                            y_pos -= kFpPoint1_6FD824;
                            x_pos += dword_6FD9B0;
                            outAng = pCar->field_50_car_sprite->field_0;
                            break;
                        case 4:
                            y_pos -= dword_6FD9B0;
                            x_pos -= kFpPoint1_6FD824;
                            outAng = pCar->field_50_car_sprite->field_0;
                            break;
                        case 5:
                            y_pos -= dword_6FD9B0;
                            x_pos -= kFpPoint1_6FD824 + dword_6FD9B0;
                            outAng = pCar->field_50_car_sprite->field_0;
                            break;
                        case 6:
                            y_pos -= dword_6FD9B0;
                            x_pos -= dword_6FD82C;
                            outAng = pCar->field_50_car_sprite->field_0;
                            break;
                        case 7:
                            y_pos -= dword_6FD9B0;
                            x_pos -= dword_6FD82C + dword_6FD9B0;
                            outAng = pCar->field_50_car_sprite->field_0;
                            break;
                        case 8:
                            y_pos -= dword_6FD9B0;
                            x_pos -= kFpPoint3_6FD830;
                            outAng = pCar->field_50_car_sprite->field_0;
                            break;
                        case 9:
                        case 10:
                        case 11:
                        case 12:
                            y_pos -= kFpPoint02_6FD9AC + kFpPoint1_6FD824;
                            x_pos -= kFpPoint04_6FD9B8;
                            outAng = pCar->field_50_car_sprite->field_0;
                            break;
                        case 99:
                            x_pos += kFpPoint1_6FD824;
                            outAng = pCar->field_50_car_sprite->field_0;
                            break;
                    }
                }
            }
            break;
        case 1:
        case 3:
            x_pos -= offset;
            if (bUnk)
            {
                y_pos -= kFpPoint1_6FD824;
                switch ((u8)animPhase)
                {
                    case 0:
                        x_pos += dword_6FD82C;
                        break;
                    case 1:
                        x_pos += kFpPoint1_6FD824 + kFpPoint06_6FD9C8;
                        break;
                    case 2:
                        x_pos += kFpPoint1_6FD824 + kFpPoint02_6FD9AC;
                        break;
                    case 3:
                        x_pos += kFpPoint08_6FD9D4;
                        break;
                    case 4:
                        x_pos += kFpPoint04_6FD9B8;
                        break;
                    case 6:
                        x_pos -= kFpPoint04_6FD9B8;
                        break;
                    case 7:
                        x_pos -= kFpPoint08_6FD9D4;
                        break;
                    default:
                        break;
                }
                outAng = Ang16(pCar->field_50_car_sprite->field_0.rValue - kAng90_6FD854.rValue, (u8)0);
            }
            else
            {
                switch ((u8)animPhase)
                {
                    case 0:
                        y_pos -= kFpPoint1_6FD824;
                        x_pos += kFpPoint1_6FD824;
                        outAng = Ang16(pCar->field_50_car_sprite->field_0.rValue + kAng180_6FD936.rValue).Normalized_406C20();
                        break;
                    case 1:
                        y_pos -= kFpPoint1_6FD824;
                        x_pos += dword_6FD9B0;
                        outAng = Ang16(pCar->field_50_car_sprite->field_0.rValue + kAng180_6FD936.rValue, (u8)0);
                        break;
                    case 2:
                        y_pos -= kFpPoint1_6FD824;
                        outAng = Ang16(pCar->field_50_car_sprite->field_0.rValue + kAng180_6FD936.rValue, (u8)0);
                        break;
                    case 3:
                        y_pos -= kFpPoint1_6FD824;
                        x_pos -= dword_6FD9B0;
                        outAng = Ang16(pCar->field_50_car_sprite->field_0.rValue + kAng180_6FD936.rValue, (u8)0);
                        break;
                    case 4:
                    case 8:
                        y_pos -= dword_6FD9B0;
                        x_pos += kFpPoint1_6FD824;
                        outAng = pCar->field_50_car_sprite->field_0;
                        break;
                    case 5:
                        y_pos -= dword_6FD9B0;
                        x_pos += kFpPoint1_6FD824 + dword_6FD9B0;
                        outAng = pCar->field_50_car_sprite->field_0;
                        break;
                    case 6:
                        y_pos -= dword_6FD9B0;
                        x_pos += dword_6FD82C;
                        outAng = pCar->field_50_car_sprite->field_0;
                        break;
                    case 7:
                        y_pos -= dword_6FD9B0;
                        x_pos += dword_6FD82C + dword_6FD9B0;
                        outAng = pCar->field_50_car_sprite->field_0;
                        break;
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                        y_pos -= kFpPoint1_6FD824;
                        x_pos += kFpPoint04_6FD9B8;
                        outAng = Ang16(pCar->field_50_car_sprite->field_0.rValue + kAng90_6FD854.rValue, (u8)0);
                        break;
                    case 99:
                        x_pos -= kFpPoint1_6FD824;
                        outAng = pCar->field_50_car_sprite->field_0;
                        break;
                }
            }
            break;
        default:
            break;
    }
    Ang16::RotateVector_41FC90(x_pos, y_pos, pCar->field_50_car_sprite->field_0);
    outX = pCar->field_50_car_sprite->field_14_xy.x + x_pos;
    outY = pCar->field_50_car_sprite->field_14_xy.y + y_pos;
}

MATCH_FUNC(0x45AE40)
EXPORT bool __stdcall abs_sub_less_than_epislon_45AE40(Fix16 a1, Fix16 a2)
{
    if (a1 == a2)
    {
        return true;
    }

    if (Fix16::Abs(a1 - a2) < kFpPoint9_6784A4)
    {
        return true;
    }
    return false;
}

MATCH_FUNC(0x45ae70)
Ped::Ped()
{
    field_200_id = 0;
    Reset_45AFC0();
    mpNext = 0;
}

MATCH_FUNC(0x45af00)
Ped::~Ped()
{
    this->field_15C_player = 0;
    this->field_140_stolen_car = 0;
    this->field_144_attacker = 0;
    this->field_148_objective_target_ped = 0;
    this->field_14C_internal_target_ped = 0;
    this->field_150_target_objective_car = 0;
    this->field_158_unk_car = 0;
    this->field_154_target_to_enter = 0;
    this->mpNext = 0;
    this->field_164_ped_group = 0;
    this->field_168_game_object = 0;
    this->field_16C_car = 0;
    this->field_170_selected_weapon = 0;
    this->field_174_pWeapon = 0;
    this->field_180_car_thief = 0;
    this->field_18C_current_path_point = 0;
    this->field_21C &= ~0x4000u;
    this->field_184_pObj2C = 0;
    this->field_208_invulnerability = 0;
    this->field_204_killer_id = 0;
    this->field_290 = 0;
    this->field_264_killer_id_timer = 0;
    this->field_268 = 0;
    this->field_198 = NULL;
    this->field_1A0_objective_target_object = 0;
    this->field_1A4_internal_target_object = 0;
}

// https://decomp.me/scratch/2yWEK
MATCH_FUNC(0x45afc0)
void Ped::Reset_45AFC0()
{
    field_21C_bf.b0 = 0;
    field_21C_bf.b1 = 0;
    field_234_timer = 0;
    field_238_ped_type = ped_type::dummy_3;
    field_23C_group_idx = 0;
    field_240_occupation = ped_ocupation_enum::dummy;
    field_216_health = 0;
    field_20e_offscreen_counter = 0;
    field_244_remap = -1;
    field_1AC_cam.x = kFpZero_678660;
    field_1AC_cam.y = kFpZero_678660;
    field_1AC_cam.z = kFpZero_678660;
    field_12C = gDummyPedAng_6787A8;
    field_248_enter_car_as_passenger = 1;
    field_24C_target_car_door = 0;
    field_140_stolen_car = 0;
    field_22C = 0;
    field_230 = 1;
    field_270 = 1;
    field_12E_aim_angle = gDummyPedAng_6787A8;
    field_21C_bf.b2 = 0;
    field_144_attacker = 0;
    field_130 = -gPedAng_6787A0;
    field_225_objective_status = 0;
    field_226_internal_objective_status = 0;
    field_258_objective = objectives_enum::no_obj_0;
    field_25C_internal_objective = 0;
    field_218_objective_timer = 9999;
    field_21A_car_state_timer = 9999;
    field_1B8_target_x = Fix16(-1);
    field_1BC_target_y = Fix16(-1);
    field_1C0_target_z = Fix16(-1);
    field_1C4_x = Fix16(-1);
    field_1C8_y = Fix16(-1);
    field_1CC_z = Fix16(-1);
    field_148_objective_target_ped = 0;
    field_14C_internal_target_ped = 0;
    field_150_target_objective_car = 0;
    field_154_target_to_enter = 0;
    field_158_unk_car = 0;
    field_227 = 0;
    field_228 = 0;
    field_15C_player = 0;
    field_278_ped_state_1 = 0;
    field_27C_ped_state_2 = 0;
    field_164_ped_group = 0;
    field_168_game_object = 0;
    field_16C_car = 0;
    field_170_selected_weapon = 0;
    field_174_pWeapon = 0;
    field_178_car_weapon = 0;
    field_21C_bf.b11 = 0;
    field_21C_bf.b12 = 0;
    field_21C_bf.b13 = 0;
    field_17C_pGang = 0;
    field_262_attackers_count = 0;
    field_263_prev_attackers_count = 0;
    field_180_car_thief = 0;
    field_204_killer_id = 0;
    field_290 = 0;
    field_264_killer_id_timer = 0;
    field_21C_bf.b14 = 0;
    field_0_patrol_points[0].field_0_x = 0;
    field_0_patrol_points[0].field_1_y = 0;
    field_261 = 0;
    field_18C_current_path_point = 0;
    field_21C &= ~0x18000u;
    field_190_patrol_route = 0;
    field_194_current_patrol_point = 0;
    field_265 = 0;
    field_1D0_internal_target_x = kFpZero_678660;
    field_1D4_internal_target_y = kFpZero_678660;
    field_1D8_internal_target_z = kFpZero_678660;
    field_1DC_objective_target_x = kFpZero_678660;
    field_1E0_objective_target_y = kFpZero_678660;
    field_1E4_objective_target_z = kFpZero_678660;
    field_134_rotation = gDummyPedAng_6787A8;
    field_1E8 = kFpZero_678660;
    field_1EC = kFpZero_678660.mValue;
    field_184_pObj2C = 0;
    field_267_varrok_idx = 0;
    field_280_stored_ped_state_1 = 11;
    field_284_stored_ped_state_2 = 28;
    field_208_invulnerability = 0;
    field_20A_wanted_points = 0;
    field_20C = 0;
    field_288_threat_search = threat_search_enum::area_2;
    field_28C_threat_reaction = threat_reaction_enum::run_away_3;
    field_188_last_char_punched = 0;
    field_266 = 0;
    field_21C_bf.b17 = 0;
    field_21C_bf.b18 = 0;
    field_21C_bf.b19 = 0;
    field_21C_bf.b20 = 0;
    field_21C_bf.b21 = 0;
    field_21C_bf.b22 = 0;
    field_21C_bf.b23 = 0;
    field_21C_bf.b24 = 0;
    field_21C_bf.b25 = 0;
    field_21C_bf.b26 = 0;
    field_21C_bf.b27 = 0;
    field_210_shock_counter = 0;
    field_212_electrocution_threshold = 100;
    field_1F4 = kFpOneThirtySecond_678434;
    field_1F0_maybe_max_speed = kFpOneSixteenth_678448;
    field_268 = 0;
    field_198 = NULL;
    field_19C = 0;
    byte_6787C4 = 0;
    field_21C_bf.b3 = 0;
    field_1F8_run_speed = kFpPoint8_6784A0;
    field_1A0_objective_target_object = 0;
    field_1A4_internal_target_object = 0;
    field_132_follow_car_offset_angle = gDummyPedAng_6787A8;
    field_1FC_follow_car_offset_distance = kFpZero_678660.mValue;
    field_269 = -1;
    field_214 = 0;
    field_26A_recent_crime_timer = 0;
    field_21C_bf.b4 = 0;
    field_26C_graphic_type = 1;
    field_21C_bf.b5 = 0;
    field_21C_bf.b6 = 0;
    field_250 = 0;
    field_224_bf.b0 = 0;
    field_224_bf.b1 = 0;
    field_224_bf.b2 = 0;
    field_224_bf.b3 = 0;
    field_138 = 0;
    field_13C_pTrainStation = 0;
    field_220 = 0;
    field_21C_bf.b7 = 0;
    field_21C_bf.b8 = 0;
    field_21C_bf.b9 = 0;
    field_229 = 0;
    field_21C_bf.b10 = 0;
    field_274_gang_car_model = car_model_enum::MERC;
    field_1A8_ped_killer = 0;
    field_224_bf.b4 = 0;
    field_21C_bf.b28 = 0;
    field_21C_bf.b29 = 0;
    field_260 = 0;
    field_224_bf.b5 = 1;
}

MATCH_FUNC(0x45b440)
void Ped::PoolAllocate()
{
    Ped::Reset_45AFC0();
    field_200_id = gPedId_61A89C++;
    field_21C |= 1;
    field_234_timer = 99;

    switch (field_240_occupation)
    {
        case 15:
            gNumberMuggersSpawned_6787CA = 0;
            break;
        case 16:
            gNumberCarThiefsSpawned_6787CB = 0;
            break;
        case 22:
            gNumberElvisLeadersSpawned_6787CC = 0;
            break;
        case 29:
            gNumberWalkingCopsSpawned_6787CD = 0;
            break;
    }
}

MATCH_FUNC(0x45b4e0)
char_type Ped::IsLawEnforcement_45B4E0()
{
    switch (field_240_occupation)
    {
        case 24:
        case 25:
        case 26:
        case 27:
        case 29:
        case 30:
        case 31:
        case 37:
            return 1;
        default:
            return 0;
    }
    return 0;
}

MATCH_FUNC(0x45b520)
Fix16_Point Ped::GetVelocityVector_45B520()
{
    return Fix16_Point(field_168_game_object->field_98_velocity_vector.x, field_168_game_object->field_98_velocity_vector.y);
}

MATCH_FUNC(0x45b550)
void Ped::SetRecentCrimeTimer_45B550()
{
    field_26A_recent_crime_timer = 2;
}

MATCH_FUNC(0x45b560)
void Ped::SetPlayer_45B560(Player* a2, char_type a3)
{
    field_15C_player = a2;
    field_200_id = a2->field_2E_idx + 1;
    if (a3)
    {
        field_200_id += 6;
    }
}

MATCH_FUNC(0x45b590)
bool Ped::sub_45B590()
{
    // TODO: was probably a switch case rather than checking a "between" on occupation?
    if (field_240_occupation >= 23 && (field_240_occupation <= 27 || field_240_occupation == 37))
    {
        return true;
    }
    return false;
}

MATCH_FUNC(0x45b5b0)
void Ped::CopyStatsFromPed_45B5B0(Ped* pSrc)
{
    field_21C_bf.b0 = pSrc->field_21C_bf.b0;
    field_21C_bf.b1 = pSrc->field_21C_bf.b1;
    field_234_timer = pSrc->field_234_timer;
    field_238_ped_type = pSrc->field_238_ped_type;
    field_15C_player = pSrc->field_15C_player;
    field_23C_group_idx = pSrc->field_23C_group_idx;
    field_240_occupation = pSrc->field_240_occupation;
    field_278_ped_state_1 = pSrc->field_278_ped_state_1;
    field_27C_ped_state_2 = pSrc->field_27C_ped_state_2;
    field_216_health = pSrc->field_216_health;
    field_20e_offscreen_counter = pSrc->field_20e_offscreen_counter;
    field_244_remap = pSrc->field_244_remap;
    field_21C_bf.b11 = pSrc->field_21C_bf.b11;
    field_1AC_cam.x = pSrc->field_1AC_cam.x;
    field_1AC_cam.y = pSrc->field_1AC_cam.y;
    field_1AC_cam.z = pSrc->field_1AC_cam.z;
    field_12C = pSrc->field_12C;
    field_248_enter_car_as_passenger = pSrc->field_248_enter_car_as_passenger;
    field_24C_target_car_door = pSrc->field_24C_target_car_door;
    field_140_stolen_car = pSrc->field_140_stolen_car;
    field_22C = pSrc->field_22C;
    field_230 = pSrc->field_230;
    field_270 = pSrc->field_270;
    field_28C_threat_reaction = pSrc->field_28C_threat_reaction;
    field_21C_bf.b2 = pSrc->field_21C_bf.b2;
    field_144_attacker = pSrc->field_144_attacker;
    field_130 = pSrc->field_130;
    field_225_objective_status = pSrc->field_225_objective_status;
    field_226_internal_objective_status = pSrc->field_226_internal_objective_status;
    field_258_objective = pSrc->field_258_objective;
    field_25C_internal_objective = pSrc->field_25C_internal_objective;
    field_218_objective_timer = pSrc->field_218_objective_timer;
    field_21A_car_state_timer = pSrc->field_21A_car_state_timer;
    field_1B8_target_x = pSrc->field_1B8_target_x;
    field_1BC_target_y = pSrc->field_1BC_target_y;
    field_1C0_target_z = pSrc->field_1C0_target_z;
    field_1C4_x = pSrc->field_1C4_x;
    field_1C8_y = pSrc->field_1C8_y;
    field_1CC_z = pSrc->field_1CC_z;
    field_148_objective_target_ped = pSrc->field_148_objective_target_ped;
    field_14C_internal_target_ped = pSrc->field_14C_internal_target_ped;
    field_150_target_objective_car = pSrc->field_150_target_objective_car;
    field_1A0_objective_target_object = pSrc->field_1A0_objective_target_object;
    field_1A4_internal_target_object = pSrc->field_1A4_internal_target_object;
    field_158_unk_car = pSrc->field_158_unk_car;
    field_154_target_to_enter = pSrc->field_154_target_to_enter;
    field_227 = pSrc->field_227;
    field_228 = pSrc->field_228;
    field_164_ped_group = pSrc->field_164_ped_group;
    field_168_game_object = pSrc->field_168_game_object;
    field_16C_car = pSrc->field_16C_car;
    field_170_selected_weapon = pSrc->field_170_selected_weapon;
    field_174_pWeapon = pSrc->field_174_pWeapon;
    field_17C_pGang = pSrc->field_17C_pGang;
    field_262_attackers_count = pSrc->field_262_attackers_count;
    field_263_prev_attackers_count = pSrc->field_263_prev_attackers_count;
    field_180_car_thief = pSrc->field_180_car_thief;
    field_0_patrol_points[0].field_0_x = 0;
    field_0_patrol_points[0].field_1_y = 0;
    field_261 = pSrc->field_261;
    field_18C_current_path_point = pSrc->field_18C_current_path_point;
    field_21C_bf.b15 = pSrc->field_21C_bf.b15;
    field_21C_bf.b16 = pSrc->field_21C_bf.b16;
    field_1D0_internal_target_x = pSrc->field_1D0_internal_target_x;
    field_1D4_internal_target_y = pSrc->field_1D4_internal_target_y;
    field_1D8_internal_target_z = pSrc->field_1D8_internal_target_z;
    field_1DC_objective_target_x = pSrc->field_1DC_objective_target_x;
    field_1E0_objective_target_y = pSrc->field_1E0_objective_target_y;
    field_1E4_objective_target_z = pSrc->field_1E4_objective_target_z;
    field_184_pObj2C = pSrc->field_184_pObj2C;
    field_267_varrok_idx = pSrc->field_267_varrok_idx;
    field_280_stored_ped_state_1 = pSrc->field_280_stored_ped_state_1;
    field_284_stored_ped_state_2 = pSrc->field_284_stored_ped_state_2;
    field_208_invulnerability = pSrc->field_208_invulnerability;
    field_21C_bf.b17 = pSrc->field_21C_bf.b17;
    field_20A_wanted_points = pSrc->field_20A_wanted_points;
    field_21C_bf.b24 = pSrc->field_21C_bf.b24;
    field_21C_bf.b25 = pSrc->field_21C_bf.b25;
    field_21C_bf.b26 = pSrc->field_21C_bf.b26;
    field_1F0_maybe_max_speed = pSrc->field_1F0_maybe_max_speed;
    field_1F4 = pSrc->field_1F4;
    DrawFlamesAndStartScreamTimer();
    SetSpriteSemiTransIfInvisible();
    SetSpriteFlagIfInvulnerable_45C070();
    field_26C_graphic_type = pSrc->field_26C_graphic_type;
    field_250 = pSrc->field_250;
    field_224 = ((field_224 ^ pSrc->field_224) & 0x1) ^ field_224;
    field_224 = ((field_224 ^ pSrc->field_224) & 0x2) ^ field_224;
    field_224 = ((field_224 ^ pSrc->field_224) & 0x4) ^ field_224;
    field_224 = ((field_224 ^ pSrc->field_224) & 0x8) ^ field_224;
    field_138 = pSrc->field_138;
    field_13C_pTrainStation = pSrc->field_13C_pTrainStation;
    field_26A_recent_crime_timer = pSrc->field_26A_recent_crime_timer;
    field_21C_bf.b4 = pSrc->field_21C_bf.b4;
    field_26C_graphic_type = pSrc->field_26C_graphic_type;
    field_21C_bf.b5 = pSrc->field_21C_bf.b5;
    field_21C_bf.b6 = pSrc->field_21C_bf.b6;
    field_21C_bf.b7 = pSrc->field_21C_bf.b7;
    field_21C_bf.b8 = pSrc->field_21C_bf.b8;
    field_21C_bf.b9 = pSrc->field_21C_bf.b9;
    field_229 = pSrc->field_229;
    field_21C_bf.b10 = pSrc->field_21C_bf.b10;
    field_274_gang_car_model = pSrc->field_274_gang_car_model;
    field_1A8_ped_killer = pSrc->field_1A8_ped_killer;
    field_224 = ((field_224 ^ pSrc->field_224) & 0x10) ^ field_224;
    field_21C_bf.b28 = pSrc->field_21C_bf.b28;
    field_21C_bf.b29 = pSrc->field_21C_bf.b29;
    field_260 = pSrc->field_260;
}

MATCH_FUNC(0x45bbf0)
Car_BC* Ped::GetCarBeingEnteredOrExited_45BBF0()
{
    if (field_27C_ped_state_2 == ped_state_2::ped2_entering_a_car_6 || field_27C_ped_state_2 == ped_state_2::ped2_getting_out_a_car_7)
    {
        return field_154_target_to_enter;
    }
    else
    {
        return 0;
    }
}

MATCH_FUNC(0x45bc10)
void Ped::TeleportToCoord_45BC10(Fix16 xpos, Fix16 ypos)
{
    Fix16 zpos = gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(xpos, ypos);
    Car_BC* pCar = get_car_416B60();
    if (pCar)
    {
        pCar->SetPosition_443D00(xpos, ypos, zpos);
    }
    else
    {
        field_168_game_object->Teleport_545530(xpos, ypos, zpos);
    }
}

MATCH_FUNC(0x45bc70)
void Ped::ManageShocking_45BC70()
{
    if (field_278_ped_state_1 != ped_state_1::dead_9)
    {
        if (field_210_shock_counter >= field_212_electrocution_threshold)
        {
            if (field_168_game_object)
            {
                if (field_168_game_object->GetCharState_433A80() != Char_B4_state::Jumping_15 &&
                    field_278_ped_state_1 != ped_state_1::immobilized_8)
                {
                    ChangeNextPedState1_45C500(ped_state_1::immobilized_8);
                    ChangeNextPedState2_45C540(ped_state_2::electrocuted_27);
                    Set_B4_F16_To_1_433B50();
                }

                if (field_210_shock_counter > 0)
                {
                    --field_210_shock_counter;
                }
            }
            else
            {
                field_210_shock_counter = 0;
            }
        }
        else if (field_210_shock_counter > 0)
        {
            --field_210_shock_counter;
        }

        if (field_168_game_object)
        {
            if ((field_21C & 0x4000000) != 0)
            {
                sub_5DF270(field_168_game_object->get_sprite_ptr_4338D0(), kFpHalf_67853C, 0, 1, this, 0);
            }
        }
    }
}

MATCH_FUNC(0x45bd20)
bool Ped::sub_45BD20(Car_BC* pCar)
{
    if (pCar == field_154_target_to_enter || pCar->GetVelocity_43A4C0() < kFpPoint01_678624)
    {
        return true;
    }
    Car_Door_10* Door = field_154_target_to_enter->GetDoor(field_24C_target_car_door);
    Door->Close_439EA0();

    field_168_game_object->HandleGenericImpact_553E00(kAng45_6784FC + pCar->field_50_car_sprite->field_0,
                                                      kFpPoint05_678634 + kFpPoint1_678480,
                                                      kFpZero_678660,
                                                      1);
    Ped::ChangeNextPedState1_45C500(ped_state_1::walking_0);
    Ped::ChangeNextPedState2_45C540(ped_state_2::ped2_walking_0);
    return false;
}

MATCH_FUNC(0x45be30)
s32 Ped::GetBulletSpriteOffset_45BE30()
{
    Ped* pLeader;
    Player* pPlayer;
    if ((field_15C_player && field_15C_player->IsUser_41DC70()) ||
        (field_164_ped_group && (pLeader = field_164_ped_group->field_2C_ped_leader) != NULL &&
         (pPlayer = pLeader->field_15C_player) != NULL && pPlayer->IsUser_41DC70()))
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
    if ((field_21C & ped_bit_status_enum::k_ped_in_flames) == 0)
    {
        field_21C |= ped_bit_status_enum::k_ped_in_flames;
        DrawFlamesAndStartScreamTimer();
    }
}

MATCH_FUNC(0x45be90)
void Ped::PutOutFire()
{
    if ((this->field_21C & ped_bit_status_enum::k_ped_in_flames) != 0)
    {
        Char_B4* pB4 = this->field_168_game_object;
        if (pB4)
        {
            pB4->RemoveFireSprites_5454B0();
        }
        this->field_21C &= ~ped_bit_status_enum::k_ped_in_flames;
    }
}

MATCH_FUNC(0x45bec0)
void Ped::ManageBurning_45BEC0()
{
    if ((field_21C & ped_bit_status_enum::k_ped_in_flames) != 0)
    {
        if (field_208_invulnerability > 0)
        {
            PutOutFire();
        }
        else if (field_16C_car)
        {
            PutOutFire();
            field_16C_car->HandleCarExplosion_43D840(19);
        }
        else
        {
            const bool HasDiedBefore = isDead_403B60();
            TakeDamage(1);
            field_264_killer_id_timer = 50;

            if (isDead_403B60() && !HasDiedBefore)
            {
                Player* pWeapons = field_15C_player;
                if (pWeapons)
                {
                    pWeapons->SetDeathType_434950(2);
                }
            }

            if (!is_player_41B0A0()) // not player
            {
                if (field_168_game_object)
                {
                    if (field_25C_internal_objective != 1) // not in flee/running state?
                    {
                        field_21C |= ped_bit_status_enum::k_ped_0x00000004;

                        SetObjective2_463830(1, 9999); // make them flee

                        field_1D0_internal_target_x = field_1AC_cam.x;
                        field_1D4_internal_target_y = field_1AC_cam.y;
                        field_1D8_internal_target_z = field_1AC_cam.z;
                    }
                }
            }
        }
    }
}

MATCH_FUNC(0x45bfb0)
void Ped::DrawFlamesAndStartScreamTimer()
{
    if ((field_21C & ped_bit_status_enum::k_ped_in_flames) != 0)
    {
        Char_B4* pB4 = field_168_game_object;
        if (pB4)
        {
            pB4->DrawFlamesAndStartScreamTimer_545430();
        }
    }
}

MATCH_FUNC(0x45bfd0)
void Ped::SetInvisible()
{
    field_21C |= ped_bit_status_enum::k_ped_invisible;
    SetSpriteSemiTransIfInvisible();
}

MATCH_FUNC(0x45bfe0)
void Ped::SetVisible()
{
    this->field_21C &= ~ped_bit_status_enum::k_ped_invisible;
    Char_B4* pB4 = this->field_168_game_object;
    if (pB4)
    {
        pB4->field_80_sprite_ptr->field_2C_flags = 0; // make sprite opaque
    }
}

MATCH_FUNC(0x45c010)
void Ped::SetSpriteSemiTransIfInvisible()
{
    if ((this->field_21C & ped_bit_status_enum::k_ped_invisible) != 0)
    {
        Char_B4* pB4 = this->field_168_game_object;
        if (pB4)
        {
            pB4->field_80_sprite_ptr->field_2C_flags = 0x41; // make sprite semi transparent
        }
    }
}

MATCH_FUNC(0x45c040)
void Ped::SetInvulnerable()
{
    field_208_invulnerability = 9999;
    SetSpriteFlagIfInvulnerable_45C070();
}

MATCH_FUNC(0x45c050)
void Ped::ClearInvulnerable_45C050()
{
    field_208_invulnerability = 0;
    Char_B4* pB4 = field_168_game_object;
    if (pB4)
    {
        pB4->field_80_sprite_ptr->field_2C_flags &= ~4u;
    }
}

MATCH_FUNC(0x45c070)
void Ped::SetSpriteFlagIfInvulnerable_45C070()
{
    if (this->field_208_invulnerability == 9999)
    {
        Char_B4* pB4 = this->field_168_game_object;
        if (pB4)
        {
            pB4->field_80_sprite_ptr->field_2C_flags |= 4u;
        }
    }
}

MATCH_FUNC(0x45c090)
void Ped::RestoreCarOrPedHealth()
{
    Car_BC* pBc = field_16C_car;
    if (pBc)
    {
        pBc->RemoveAllDamage();
    }
    else if (field_278_ped_state_1 != ped_state1_enum::ped_wasted)
    {
        field_216_health = 100;
    }
}

MATCH_FUNC(0x45c0c0)
void Ped::SpawnCharInZone_45C0C0(gmp_map_zone* pZone)
{
    u8 next = 0;

    u8 xs = pZone->field_1_x + (pZone->field_3_w >> 1);
    u8 ys = pZone->field_2_y + (pZone->field_4_h >> 1);
    u8 xxx = xs;
    u8 yyy = ys;

    s32 found_z;

    u8 a2 = 1;
    u8 a0 = 1;
    u8 a3 = 1;
    u8 a1 = 1;

    while (true)
    {
        if (gMap_0x370_6F6268->FindHighestBlockForCoord_4E4C30(xxx, yyy, &found_z))
        {
            break;
        }
        switch (next)
        {
            case 0:
                if (!--a3)
                {
                    next = 1;
                    a1 = a0;
                }
                if (--xxx < xs - (pZone->field_3_w >> 1))
                {
                    ++xxx;
                }
                break;
            case 1:
                --a1;
                if (!a1)
                {
                    next = 2;
                    a3 = ++a2;
                }
                if (--yyy < ys - (pZone->field_4_h >> 1))
                {
                    ++yyy;
                }
                break;
            case 2:
                if (!--a3)
                {
                    ++a0;
                    next = 3;
                    a1 = a0;
                }
                if (++xxx > xs + (pZone->field_3_w >> 1))
                {
                    ++xxx;
                }
                break;
            case 3:
                --a1;
                if (!a1)
                {
                    next = 0;
                    a3 = a2;
                }
                if (++yyy > ys + (pZone->field_4_h >> 1))
                {
                    --yyy;
                }
                break;
            default:
                continue;
        }
    }
    AllocCharB4_45C830(Fix16(xxx) + kFpHalf_67853C, Fix16(yyy) + kFpHalf_67853C, found_z + 1);
}

MATCH_FUNC(0x45c310)
void Ped::PoolDeallocate()
{
    if (field_168_game_object)
    {
        gChar_B4_Pool_6FDB44->DeAllocate(field_168_game_object);
        field_168_game_object = 0;
    }
}

MATCH_FUNC(0x45c350)
void Ped::RespawnPed_45C350(gmp_map_zone* pZone)
{
    if (field_168_game_object)
    {
        gChar_B4_Pool_6FDB44->DeAllocate(field_168_game_object);
    }
    field_168_game_object = 0;

    field_16C_car = 0;

    if (field_164_ped_group)
    {
        field_164_ped_group->DestroyGroup_4C93A0();
    }

    SpawnCharInZone_45C0C0(pZone);

    // TODO: missing inlines here, temp var shouldn't be needed
    // 9.6f: SetRemap_433C10(get_remap_433BA0()) (inlined, using it changes the code)
    Char_B4* pTmp = field_168_game_object;
    const u8 remap = get_remap_433BA0();
    pTmp->field_5_remap = remap;
    if (remap != 0xFF)
    {
        pTmp->field_80_sprite_ptr->SetRemap(remap);
    }

    field_27C_ped_state_2 = ped_state_2::Unknown_29;
    set_health_4039A0(100);
    field_208_invulnerability = 50;

    SetObjective(objectives_enum::no_obj_0, 9999);
    SetObjective2_463830(objectives_enum::no_obj_0, 9999);
}

MATCH_FUNC(0x45c410)
void Ped::ResetForPlayerRespawn_45C410()
{
    Char_B4* pB4 = this->field_168_game_object;
    Player* pPlayer = this->field_15C_player;

    PutOutFire();
    const u8 remap = this->field_244_remap;
    Reset_45AFC0();

    this->field_244_remap = remap;
    this->field_168_game_object = pB4;

    this->field_21C |= 1;

    this->field_1AC_cam.x = pB4->field_80_sprite_ptr->field_14_xy.x;
    this->field_1AC_cam.y = pB4->field_80_sprite_ptr->field_14_xy.y;
    this->field_1AC_cam.z = pB4->field_80_sprite_ptr->field_1C_zpos;

    this->set_health_4039A0(100);
    this->SetField238_403920(ped_type::player_2);
    this->field_208_invulnerability = 50;
    this->field_234_timer = 99;
    this->field_15C_player = pPlayer;
}

MATCH_FUNC(0x45c4b0)
void Ped::UpdatePositionFromCar_45C4B0()
{
    field_1AC_cam.x = field_16C_car->field_50_car_sprite->field_14_xy.x;
    field_1AC_cam.y = field_16C_car->field_50_car_sprite->field_14_xy.y;
    field_1AC_cam.z = field_16C_car->field_50_car_sprite->field_1C_zpos;
}

MATCH_FUNC(0x45c500)
void Ped::ChangeNextPedState1_45C500(s32 new_state)
{
    // If the ped is immobilized, store the new ped state for later use
    if (field_278_ped_state_1 != ped_state1_enum::ped_fall_on_ground)
    {
        // Ped is currently not immobilized
        if (new_state == ped_state1_enum::ped_fall_on_ground)
        {
            // Ped now must be immobilized, so store the previous state
            field_280_stored_ped_state_1 = field_278_ped_state_1;
        }
        field_278_ped_state_1 = new_state; // update state
    }
    else if (new_state != ped_state1_enum::ped_fall_on_ground)
    {
        // current state of the ped is immobilized, so store the new state (if not the same "immobilized")
        field_280_stored_ped_state_1 = new_state;
    }
}

MATCH_FUNC(0x45c540)
void Ped::ChangeNextPedState2_45C540(s32 new_state)
{
    if (field_278_ped_state_1 != ped_state_1::immobilized_8)
    {
        field_27C_ped_state_2 = new_state; // just update, dont store previous
    }
    else if (field_27C_ped_state_2 < 17 || field_27C_ped_state_2 > 28)
    {
        field_284_stored_ped_state_2 = field_27C_ped_state_2; // store previous state
        field_27C_ped_state_2 = new_state; // update
    }
    else if (new_state < 23 || new_state > 26)
    {
        field_284_stored_ped_state_2 = new_state; // only store the next state, but do not change it yet
    }
    else
    {
        // store previous state & update
        field_284_stored_ped_state_2 = field_27C_ped_state_2;
        field_27C_ped_state_2 = new_state;
    }
}

MATCH_FUNC(0x45c5a0)
void Ped::RestorePreviousPedState_45C5A0()
{
    field_278_ped_state_1 = field_280_stored_ped_state_1;
    field_27C_ped_state_2 = field_284_stored_ped_state_2;
}

MATCH_FUNC(0x45c5c0)
void Ped::CancelEnterCarObjective_45C5C0()
{
    if (!this->field_16C_car && this->field_258_objective == objectives_enum::enter_car_as_driver_35 && this->field_25C_internal_objective == 35 &&
        this->field_168_game_object->field_10_char_state != Char_B4_state::Jumping_15 &&
        this->field_27C_ped_state_2 != ped_state_2::ped2_entering_a_car_6)
    {
        ChangeNextPedState1_45C500(ped_state_1::walking_0);
        ChangeNextPedState2_45C540(ped_state_2::ped2_walking_0);
        this->field_16C_car = 0;
        SetObjective(objectives_enum::no_obj_0, 9999);
        SetObjective2_463830(objectives_enum::no_obj_0, 9999);
        PedGroup* pGroup = this->field_164_ped_group;
        if (pGroup)
        {
            pGroup->ResetMembersToFollowLeader_4C91B0();
        }
    }
}

MATCH_FUNC(0x45C650)
void Ped::SpawnDriverRunAway_45C650(Car_BC* pCar, Ped* pOther)
{
    switch (pCar->field_84_car_info_idx)
    {
        case 3:
        case 4:
        case 6:
        case 7:
        case 17:
        case 22:
        case 30:
        case 54:
        case 59:
        case 60:
        case 61:
            return;
        default:
            if (pCar->GetRemap())
            {
                Ped* pPed = gPedManager_6787BC->SpawnRunAwayGuy_470D60();
                pPed->field_240_occupation = 0x32;
                pCar->field_54_driver = pPed;
                pPed->field_16C_car = pCar;
                pPed->field_24C_target_car_door = 0;
                pPed->field_248_enter_car_as_passenger = 0;
                pPed->SetObjective(objectives_enum::leave_car_36, 9999);
                pPed->field_150_target_objective_car = pPed->field_16C_car;
                pPed->field_180_car_thief = pOther;
                pPed->field_28C_threat_reaction = 3;
            }
            break;
    }
}

MATCH_FUNC(0x45c730)
void Ped::SpawnPedInCar_45C730(Car_BC* pCar)
{
    this->field_16C_car = pCar;
    this->field_248_enter_car_as_passenger = 0;
    this->field_24C_target_car_door = 0;
    this->field_278_ped_state_1 = ped_state_1::in_car_10;

    pCar->SetDriver(this);
    pCar->field_7C_uni_num = field_238_ped_type;
    pCar->field_76_last_seen_timer = 0;

    // TODO: inline ??
    if (pCar->field_88_despawn_status == 2 || pCar->field_88_despawn_status == 4 || pCar->field_88_despawn_status == 3)
    {
        pCar->field_88_despawn_status = 1;
    }
}

MATCH_FUNC(0x45c7a0)
void Ped::EnterCarAsDriver(Car_BC* pCar)
{
    SetObjective2_463830(objectives_enum::no_obj_0, 9999);
    SetObjective(objectives_enum::enter_car_as_driver_35, 9999);
    field_248_enter_car_as_passenger = 1;
    field_150_target_objective_car = pCar;
    field_24C_target_car_door = pCar->GetRemap() - 1;
}

MATCH_FUNC(0x45c7f0)
void Ped::EnterCarAsPassenger_45C7F0(Car_BC* pCar)
{
    this->field_248_enter_car_as_passenger = 1;
    this->field_16C_car = pCar;
    this->field_24C_target_car_door = 1;
    this->field_278_ped_state_1 = ped_state_1::in_car_10;
    this->field_27C_ped_state_2 = ped_state_2::ped2_driving_10;
    pCar->field_4_passengers_list.AddPed_471140(this);
}

MATCH_FUNC(0x45c830)
char_type Ped::AllocCharB4_45C830(Fix16 xpos, Fix16 ypos, Fix16 zpos)
{
    Char_B4* pChar = gChar_B4_Pool_6FDB44->field_0_pool.Allocate();

    field_168_game_object = pChar;
    if (!pChar)
    {
        return 0;
    }

    Sprite* pSprite = pSprite = pChar->field_80_sprite_ptr;
    pSprite->set_xyz_lazy_420600(xpos, ypos, zpos);

    pChar->field_80_sprite_ptr->AllocInternal_59F950(gDummyW_678530, gDummyW_678530, gDummyZ_67841C);

    gPurpleDoom_1_679208->AddToRegionBuckets_477B20(pChar->field_80_sprite_ptr);
    field_168_game_object->set_pPed_4338E0(this);

    field_1AC_cam.y = ypos;
    field_1AC_cam.x = xpos;
    field_1AC_cam.z = zpos;

    DrawFlamesAndStartScreamTimer();
    SetSpriteSemiTransIfInvisible();
    SetSpriteFlagIfInvulnerable_45C070();

    return 1;
}

MATCH_FUNC(0x45c900)
Ang16 Ped::get_field8_45C900()
{
    return field_15C_player->field_8_turn_speed;
}

MATCH_FUNC(0x45c920)
Fix16 Ped::GetPedVelocity_45C920()
{
    if (field_168_game_object)
    {
        return field_168_game_object->get_velocity_41B080(); // velocity ??
    }
    else
    {
        if (field_16C_car)
        {
            return field_16C_car->GetVelocity_43A4C0();
        }
        return kFpZero_678660;
    }
}

MATCH_FUNC(0x45c960)
Ang16 Ped::GetRotation()
{
    if (field_168_game_object != NULL)
    {
        return field_168_game_object->get_rotation_433A40();
    }

    if (field_16C_car != NULL)
    {
        return field_16C_car->field_50_car_sprite->field_0;
    }

    return gDummyPedAng_6787A8;
}

MATCH_FUNC(0x45c9b0)
Fix16 Ped::get_fieldC_45C9B0()
{
    return field_15C_player->field_C_move_direction;
}

// 9.6f 0x43E3A0
MATCH_FUNC(0x45c9d0)
Ang16 Ped::ComputeAimAngle_45C9D0()
{
    if (IsField238_45EDE0(2))
    {
        Ped* pNearest = gThreateningPedsList_678468.FindClosestPedInViewCone_4713C0(this->field_1AC_cam.x,
                                                                                    this->field_1AC_cam.y,
                                                                                    this->field_12C,
                                                                                    kAng16_6784E4);
        Ped* best = pNearest;

        if (!best)
        {
            word_6784F0 = kAng16_6784E4;
            best = FindBestTargetPed_Mode4_466BB0(3);
        }

        if (!best)
        {
            best = FindNearestPed_Mode4_466F40(3u);
        }

        // field_130 stored in each arm, as 9.6f does; the 21-byte return tail is jumped to, not copied
        if (best)
        {
            field_130 = Fix16::atan2_fixed_405320(best->field_1AC_cam.y - field_1AC_cam.y,
                                                  best->field_1AC_cam.x - field_1AC_cam.x);
        }
        else
        {
            field_130 = field_12C;
        }
    }
    return field_130;
}

MATCH_FUNC(0x45caa0)
void Ped::HandleClosePedInteraction_45CAA0()
{

    PedGroup* pNearPedGroup; // eax
    s16 rng; // bp

    Ped* pNearPed_ = FindNearbyPed_466FB0();
    if (pNearPed_)
    {
        if (abs_sub_less_than_epislon_45AE40(this->field_1AC_cam.z, pNearPed_->get_cam_z()))
        {
            this->field_188_last_char_punched = pNearPed_;
            pNearPedGroup = pNearPed_->field_164_ped_group;
            if (!pNearPedGroup || pNearPedGroup != this->field_164_ped_group)
            {
                pNearPed_->field_144_attacker = this;
                if (this->field_168_game_object->field_68_animation_frame >= 3u)
                {
                    goto LABEL_27;
                }
                if (this->field_240_occupation == ped_ocupation_enum::mugger)
                {
                    Set_F250_IfBit_433DD0(8);
                    pNearPed_->Set_F250_IfBit_433DD0(7);
                }
                else
                {
                    pNearPed_->sub_433E50();
                }
                pNearPed_->field_204_killer_id = this->field_200_id;
                pNearPed_->field_290 = 10;
                pNearPed_->field_264_killer_id_timer = 50;
                rng = gRng_6F6784.get_int_4F7AE0(20);
                if (pNearPed_->get_health_433B70() > 30)
                {
                    if (pNearPed_->IsField238_45EDE0(2))
                    {
                        if (this->field_240_occupation == ped_ocupation_enum::mugger)
                        {
                            pNearPed_->field_15C_player->Add_2D4(-10);
                            this->field_229++;
                            if ((u8)field_229 > 9u)
                            {
                                this->field_226_internal_objective_status = 1;
                            }
                        }
                        else if (pNearPed_->field_240_occupation != ped_ocupation_enum::criminal_type_1)
                        {
                            pNearPed_->TakeDamage(10);
                        }
                    }
                    else if (pNearPed_->field_240_occupation != ped_ocupation_enum::criminal_type_1)
                    {
                        pNearPed_->TakeDamage(20);
                    }

                    if (rng >= 2)
                    {
                        goto LABEL_27;
                    }
                }
                if (IsField238_45EDE0(2))
                {
                    pNearPed_->ChangeNextPedState2_45C540(22);
                    pNearPed_->ChangeNextPedState1_45C500(8);
                    pNearPed_->Set_B4_F16_To_1_433B50();
                    pNearPed_->field_168_game_object->SetCharState_433A60(33);
                }
                else
                {
                LABEL_27:
                    if (pNearPed_->field_28C_threat_reaction == threat_reaction_enum::run_away_3)
                    {
                        pNearPed_->SetObjective2_463830(2, 9999);
                        pNearPed_->set_field_14C_403AE0(this);
                        this->field_21C |= 4;
                    }
                }
            }
        }
    }
    else
    {
        this->field_188_last_char_punched = 0;
    }
}

MATCH_FUNC(0x45ce50)
void Ped::TakeDamage(s16 damage)
{
    if (field_208_invulnerability <= 0)
    {
        field_216_health -= damage;
        if (field_216_health <= 0)
        {
            if (field_278_ped_state_1 != ped_state_1::immobilized_8)
            {
                field_216_health = 0;
                Kill_46F9D0();
            }
            else
            {
                if (field_216_health < 0)
                {
                    field_216_health = 0;
                }
            }
        }
    }
}

MATCH_FUNC(0x45cf20)
void Ped::HandlePedCrossingTrigger_45CF20(Object_2C* a2)
{
    if (field_278_ped_state_1 != ped_state_1::dead_9 && field_278_ped_state_1 != ped_state_1::immobilized_8 && a2->field_18_model == objects::ped_crossing_trigger_258 &&
        gNumPedsCrossingRoad_6787D0 < 10)
    {
        StartCrossingRoad_45E4A0();
    }
}

Fix16 __stdcall sub_4614E0(Fix16& x1, Fix16& y1, Fix16& x2, Fix16& y2);

// 10.5 inlines this into HandlePedHitByObject_45D000 (the first call there; the second one
// calls 0x465D00), so it has to be defined before it. 0x465D00 is the out-of-line copy.
inline bool Ped::IsPedAThreat_Inline_465D00(Ped* a2)
{

    char_type flag = 0;

    if ((a2->field_21C & 0x2000000) != 0)
    {
        if (a2->field_168_game_object != 0)
        {
            goto ret_false;
        }
    }

    if (this->field_288_threat_search == 3 || this->field_288_threat_search == 4)
    {
        if (!a2->IsField238_45EDE0(2))
        {
            goto ret_false;
        }
    }

    if (this->field_288_threat_search == 5 || this->field_288_threat_search == 6)
    {
        if (a2->sub_45EDC0())
        {
            goto ret_true;
        }
    }

    if (this->sub_45EDC0())
    {
        flag = 1;
    }

    if (this->field_164_ped_group == a2->field_164_ped_group && this->field_164_ped_group != 0)
    {
        goto ret_false;
    }

    if (this->field_240_occupation == 0x28)
    {
        Car_BC* pCar = a2->field_16C_car;

        if (pCar == 0)
        {
            goto ret_true;
        }

        if (!pCar->IsPoliceCar_439EC0())
        {
            goto ret_true;
        }

        if (a2->field_238_ped_type == 4)
        {
            goto ret_false;
        }

        return 1;
    }

    {
        Gang_144* pMyGang = this->field_17C_pGang;

        if (pMyGang != 0)
        {
            Gang_144* pOtherGang = a2->field_17C_pGang;

            if (pOtherGang != 0)
            {
                if (pOtherGang == pMyGang)
                {
                    goto merge_178;
                }

                if (!pMyGang->IsHostileToGang_4BEDF0(pOtherGang->field_1_gang_idx))
                {
                    goto ret_false;
                }

                if (this->field_238_ped_type == 4)
                {
                    goto check_threat_level;
                }

                if (this->field_238_ped_type != 6)
                {
                    goto ret_true;
                }

            check_threat_level:
                if ((u8)a2->field_263_prev_attackers_count >= 4)
                {
                    goto ret_false;
                }

                if ((u8)a2->field_262_attackers_count >= 4)
                {
                    goto ret_false;
                }

                return 1;
            }

            if (pMyGang->field_110_high_respect != 0)
            {
                switch (a2->field_240_occupation)
                {
                    case 0x18:
                    case 0x19:
                    case 0x1A:
                    case 0x1B:
                    case 0x1D:
                    case 0x1E:
                    case 0x1F:
                    case 0x25:
                        goto ret_true;
                    default:
                        break;
                }
            }

            if (((BitSet32*)&a2->field_21C)->check_bit(0xB))
            {
                if (!a2->sub_45EDC0() && a2->field_240_occupation != 1)
                {
                    if (a2->field_28C_threat_reaction == 1)
                    {
                        goto merge_178;
                    }

                    if (this->field_170_selected_weapon != 0)
                    {
                        goto ret_true;
                    }

                    Fix16 candY = a2->field_1AC_cam.y;
                    Fix16 candX = a2->field_1AC_cam.x;

                    if (sub_4614E0(this->field_1AC_cam.x, this->field_1AC_cam.y, candX, candY) <= kFpOne_678798)
                    {
                        goto ret_true;
                    }

                    goto merge_178;
                }

                {
                    if (this->field_17C_pGang->IsRespectNegativeForPlayer_4BEF10(a2->field_15C_player->field_2E_idx))
                    {
                        if (gPolice_7B8_6FEE40->field_7B4 == 0)
                        {
                            goto ret_true;
                        }

                        return 0;
                    }

                    goto ret_false;
                }
            }

        block_465F75:
            if (a2->sub_45EDC0() || a2->field_240_occupation == 1)
            {
                u8 player_idx = a2->field_15C_player->field_2E_idx;

                if (this->field_17C_pGang->IsRespectNegativeForPlayer_4BEF10(player_idx))
                {
                    if (gPolice_7B8_6FEE40->field_7B4 == 0)
                    {
                        goto ret_true;
                    }
                    return 0;
                }

                goto ret_false;
            }

            goto merge_178;
        }
    }

no_my_gang:
    switch (this->field_240_occupation)
    {
        case 0x21:
            if (a2->field_240_occupation == 0x21)
            {
                return 0;
            }

            goto merge_178;
        case 0x22:
            if (a2->field_240_occupation == 0x22)
            {
                return 0;
            }

            goto merge_178;
        case 0x16:
            if (a2->IsField238_45EDE0(2))
            {
                return 0;
            }

            goto merge_178;
        case 0x18:
        case 0x19:
        case 0x1A:
        case 0x1B:
        case 0x1D:
        case 0x1E:
        case 0x1F:
        case 0x25:
            goto block_466022;
        default:
            goto block_46613E;
    }

block_466022:
    if (a2->sub_45EDC0())
    {
        if (a2->field_168_game_object != 0 && a2->field_168_game_object->field_10_char_state == 0xF)
        {
            goto ret_false;
        }

        if (!((((BitSet32*)&a2->field_21C)->check_bit(0xB)) && a2->field_170_selected_weapon != 0 &&
              a2->field_170_selected_weapon->sub_5DCEF0()))
        {
            if (a2->field_20A_wanted_points < 0x258 && this->field_144_attacker != a2 && a2->field_26A_recent_crime_timer <= 0u)
            {
                goto ret_false;
            }
        }

        this->field_144_attacker = 0;
        gPolice_7B8_6FEE40->UpdateLastSeenCoordsForCriminal_5708C0(a2);

        if (this->field_258_objective == 0x2B)
        {
            if (gPolice_7B8_6FEE40->PromptCrewAtCarToPurseCriminal_5707B0(this->field_16C_car, a2))
            {
                goto ret_true;
            }

            if (a2->field_20A_wanted_points < 0x258)
            {
                a2->field_20A_wanted_points = 0x258;
            }

            return 0;
        }

        if (a2->field_20A_wanted_points >= 0x258)
        {
            goto ret_true;
        }

        a2->field_20A_wanted_points = 0x258;
        return 1;
    }

block_4660FB:
{
    s32 a2_state = a2->field_240_occupation;
    switch (a2_state)
    {
        case 0x17:
        case 0x18:
        case 0x19:
        case 0x1A:
        case 0x1B:
        case 0x1D:
        case 0x1E:
        case 0x1F:
        case 0x25:
        case 0x27:
            goto merge_178;
        default:
            break;
    }

    if (a2_state == 1)
    {
        goto ret_true;
    }

    if (((BitSet32*)&a2->field_21C)->check_bit(0xB))
    {
        goto ret_true;
    }

    if (a2->field_25C_internal_objective != 0x14)
    {
        goto merge_178;
    }

    return 1;
}

block_46613E:
    if (this->field_164_ped_group != 0)
    {
        if (this->field_164_ped_group->field_2C_ped_leader != a2->field_14C_internal_target_ped)
        {
            goto merge_178;
        }

        if (a2->field_25C_internal_objective == 0x14)
        {
            goto ret_true;
        }

        if (a2->field_25C_internal_objective != 0x17)
        {
            goto merge_178;
        }

        return 1;
    }

    if (((BitSet32*)&a2->field_21C)->check_bit(0xB))
    {
        goto ret_true;
    }

merge_178:
    if (flag == 1)
    {
        goto ret_true;
    }

    goto ret_false;

ret_true:
    return 1;

ret_false:
    return 0;
}

// https://decomp.me/scratch/Yg4cw
WIP_FUNC(0x45d000)
char_type Ped::HandlePedHitByObject_45D000(Object_2C* pObj)
{
    WIP_IMPLEMENTED;

    Ang16 angle; // default ctor here
    Ped* pBlamedPed = NULL;

    if ((field_278_ped_state_1 == ped_state_1::immobilized_8 ||
         field_278_ped_state_1 == ped_state_1::dead_9 && pObj->field_18_model != objects::maybe_bullet_on_fire_198) &&
        field_27C_ped_state_2 != ped_state_2::lying_on_floor_22)
    {
        return false;
    }

    if (pObj->get_field_26_420FF0())
    {
        s32 ped_id = gVarrok_7F8_703398->GetPedId_420F10(pObj->get_field_26_420FF0());
        if (ped_id)
        {
            pBlamedPed = gPedManager_6787BC->PedById(ped_id);
            if (pBlamedPed)
            {
                if (pBlamedPed->IsPedAThreat_Inline_465D00(this) || this == pBlamedPed->field_14C_internal_target_ped)
                {
                    if (pBlamedPed->field_21C_bf.b13)
                    {
                        if (pBlamedPed->field_174_pWeapon)
                        {
                            pBlamedPed->field_174_pWeapon->Set_F4_433810(0);
                        }
                    }
                    else
                    {
                        if (pBlamedPed->field_170_selected_weapon)
                        {
                            pBlamedPed->field_170_selected_weapon->Set_F4_433810(0);
                        }
                    }

                    if (pBlamedPed->IsField238_45EDE0(2))
                    {
                        pBlamedPed->field_15C_player->field_2D4_scores.UpdateAccuracyCount_5934F0(2, pObj->field_18_model, this);
                    }
                    field_144_attacker = pBlamedPed;

                    if (!sub_48E720(pObj->field_18_model))
                    {
                        field_204_killer_id = pBlamedPed->field_200_id;
                        field_290 = sub_48E780(pObj->field_18_model);
                        field_264_killer_id_timer = 50;
                    }
                }
                else if (pBlamedPed->IsField238_45EDE0(2))
                {
                    if (pBlamedPed->field_21C_bf.b13)
                    {
                        if (pBlamedPed->field_174_pWeapon)
                        {
                            pBlamedPed->field_174_pWeapon->Set_F4_433810(0);
                        }
                    }
                    else
                    {
                        if (pBlamedPed->field_170_selected_weapon)
                        {
                            pBlamedPed->field_170_selected_weapon->Set_F4_433810(0);
                        }
                    }
                    pBlamedPed->field_15C_player->field_2D4_scores.UpdateAccuracyCount_5934F0(3, pObj->field_18_model, this);
                    field_144_attacker = pBlamedPed;
                    if (sub_48E720(pObj->field_18_model))
                    {
                        return true;
                    }
                    field_204_killer_id = pBlamedPed->field_200_id;
                    field_290 = sub_48E780(pObj->field_18_model);
                    field_264_killer_id_timer = 50;
                    return true;
                }
                else
                {
                    if (IsPedAThreat_465D00(pBlamedPed))
                    {
                        field_144_attacker = pBlamedPed;
                    }
                    if (pBlamedPed->field_21C_bf.b13)
                    {
                        if (pBlamedPed->field_174_pWeapon)
                        {
                            pBlamedPed->field_174_pWeapon->Set_F4_433810(1);
                        }
                    }
                    else
                    {
                        if (pBlamedPed->field_170_selected_weapon)
                        {
                            pBlamedPed->field_170_selected_weapon->Set_F4_433810(1);
                        }
                    }
                    if (field_144_attacker != pBlamedPed)
                    {
                        return true;
                    }
                }
            }
        }
    }

    gfrosty_pasteur_6F8060->RecordWeaponHit_512C00(field_200_id, pObj->field_18_model, 1);

    switch (pObj->field_18_model)
    {
        case objects::fire_hitting_194:
            Ped::SetOnFire();
            return true;

        case objects::maybe_bullet_on_fire_198:
            Ped::PutOutFire();
            if (ComputeShortestAngleDelta_4056C0(field_168_game_object->field_80_sprite_ptr->field_0, pObj->field_4->field_0) >
                kAng90_6784B0)
            {
                field_168_game_object->SetCharState_433A60(33); // TODO: include and use Char_B4_state enum
            }
            else
            {
                field_168_game_object->SetCharState_433A60(34); // TODO: include and use Char_B4_state enum
            }

            if (field_208_invulnerability)
            {
                return true;
            }

            if (field_238_ped_type == ped_type::player_2)
            {
                // Player ped
                if (field_15C_player->HasPowerUp_434920(power_up_indices::Armor_3))
                {
                    field_15C_player->DecPowerUp_434940(power_up_indices::Armor_3);
                }
                else
                {
                    Ped::TakeDamage(2 * GetDamageMultiplier_45CF90(pBlamedPed));
                }
            }
            else
            {
                Ped::TakeDamage(5 * GetDamageMultiplier_45CF90(pBlamedPed));
            }
            break;

        case objects::rocket_bullet_128:
        case objects::moving_molotov_138:
        case objects::shotgun_bullet_192:
        case objects::object_195:
        case objects::machine_gun_bullet_254:
        case objects::pistol_bullet_265:
        {
            if (ComputeShortestAngleDelta_4056C0(field_168_game_object->field_80_sprite_ptr->field_0, pObj->field_4->field_0) >
                kAng90_6784B0)
            {
                field_168_game_object->SetCharState_433A60(33); // TODO: include and use Char_B4_state enum
            }
            else
            {
                field_168_game_object->SetCharState_433A60(34); // TODO: include and use Char_B4_state enum
            }
            if (field_208_invulnerability)
            {
                return true;
            }

            char_type damage_amplifier;
            if (pObj->field_18_model == objects::pistol_bullet_265)
            {
                damage_amplifier = 2;
            }
            else
            {
                damage_amplifier = 1;
            }

            if (field_238_ped_type == ped_type::player_2)
            {
                // Player ped
                if (field_15C_player->HasPowerUp_434920(power_up_indices::Armor_3))
                {
                    field_15C_player->DecPowerUp_434940(power_up_indices::Armor_3);
                }
                else
                {
                    Ped::TakeDamage(10 * damage_amplifier * GetDamageMultiplier_45CF90(pBlamedPed));
                }
            }
            else
            {
                // Non-player ped
                Ped::TakeDamage(25 * damage_amplifier * GetDamageMultiplier_45CF90(pBlamedPed));
            }
            break;
        }

        case objects::electrobaton_bullet_277:
            if (pBlamedPed)
            {
                pBlamedPed->field_198 = this;
                pBlamedPed->field_268 = 20;
            }
            return true;

        default:
            return true;
    }
    gParticle_8_6FD5E8->EmitBloodBurst_53E450(field_1AC_cam.x, field_1AC_cam.y, field_1AC_cam.z, pObj->field_4->field_0);
    return true;
}

MATCH_FUNC(0x45dd30)
char_type Ped::AddWeaponWithAmmo_45DD30(s32 weapon_kind, char_type ammo)
{
    if (ammo == 100) // max is 99, 100 means get default count
    {
        ammo = gWeapon_8_707018->get_defalt_ammo_5E3E80(weapon_kind);
    }

    if (gWeapon_8_707018->is_car_weapon_433820(weapon_kind)) // car_bomb
    {
        if (field_16C_car)
        {
            return gWeapon_8_707018->allocate_5E3D50(weapon_kind, ammo, field_16C_car);
        }
        return 0;
    }

    if (field_15C_player)
    {
        return field_15C_player->AddWeaponWithAmmo_564960(weapon_kind, ammo);
    }

    if (field_170_selected_weapon && field_170_selected_weapon->field_1C_idx == weapon_kind)
    {
        return 0;
    }

    RemovePedWeapons_462510();
    ForceWeapon_46F600(weapon_kind);

    return 1;
}

MATCH_FUNC(0x45de80)
char_type Ped::HandlePickupCollision_45DE80(Object_2C* pPickUp)
{
    char_type bCollected;
    if (this->field_238_ped_type != ped_type::player_2)
    {
        return 0;
    }

    if (IsNetworkGame_434B10() && gYouthful_einstein_6F8450.IsTagGame_434B20() &&
        gYouthful_einstein_6F8450.IsFugitivePed_434B60(this))
    {
        return 0; // prevent pick ups if we are "it" in multiplayer?
    }

    s32 model = pPickUp->field_18_model;
    if (model == objects::secret_token_266)
    {
        // inc counter and remove pick up
        gLucid_hamilton_67E8E0.IncSecretTokensCollected_434A10();
        gObject_5C_6F8F84->field_20_bUnCollectedTokens[pPickUp->get_field_26_420FF0()] = 0;
        bCollected = 1;
    }
    else
    {
        if (model <= 108)
        {
            model += 136;
        }

        if (model <= 227)
        {
            bCollected = AddWeaponWithAmmo_45DD30(model - 200, pPickUp->get_field_26_420FF0());
        }
        else
        {
            bCollected = field_15C_player->CollectPowerUp_564D60(model - 228);
        }

        if (bCollected)
        {
            if (field_15C_player->IsUser_41DC70())
            {
                gHud_2B00_706620->field_1080_pickup_text.ShowPickupText_5D5600(model + 56);
            }
        }
    }
    if (bCollected)
    {
        pPickUp->Dealloc_5291B0();
    }
    return bCollected;
}

MATCH_FUNC(0x45e080)
void Ped::SpawnWeaponOnDeath_45E080()
{
    Object_2C* v2; // eax

    if ((this->field_224 & 0x20) != 0 && !this->field_16C_car)
    {
        if (this->field_170_selected_weapon)
        {
            if (!IsField238_45EDE0(2))
            {
                switch (this->field_170_selected_weapon->field_1C_idx)
                {
                    case weapon_type::pistol:
                        v2 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(200, field_1AC_cam.x, field_1AC_cam.y, field_1AC_cam.z, gDummyPedAng_6787A8);
                        break;

                    case weapon_type::smg:
                        v2 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(201, field_1AC_cam.x, field_1AC_cam.y, field_1AC_cam.z, gDummyPedAng_6787A8);
                        break;

                    case weapon_type::rocket:
                        v2 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(202, field_1AC_cam.x, field_1AC_cam.y, field_1AC_cam.z, gDummyPedAng_6787A8);
                        break;

                    case weapon_type::shocker:
                        v2 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(203, field_1AC_cam.x, field_1AC_cam.y, field_1AC_cam.z, gDummyPedAng_6787A8);
                        break;

                    case weapon_type::molotov:
                        v2 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(204, field_1AC_cam.x, field_1AC_cam.y, field_1AC_cam.z, gDummyPedAng_6787A8);
                        break;

                    case weapon_type::grenade:
                        v2 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(205, field_1AC_cam.x, field_1AC_cam.y, field_1AC_cam.z, gDummyPedAng_6787A8);
                        break;

                    case weapon_type::shotgun:
                        v2 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(206, field_1AC_cam.x, field_1AC_cam.y, field_1AC_cam.z, gDummyPedAng_6787A8);
                        break;

                    case weapon_type::electro_batton:
                        v2 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(200, field_1AC_cam.x, field_1AC_cam.y, field_1AC_cam.z, gDummyPedAng_6787A8);
                        break;

                    case weapon_type::flamethrower:
                        v2 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(208, field_1AC_cam.x, field_1AC_cam.y, field_1AC_cam.z, gDummyPedAng_6787A8);
                        break;

                    case weapon_type::silence_smg:
                        v2 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(209, field_1AC_cam.x, field_1AC_cam.y, field_1AC_cam.z, gDummyPedAng_6787A8);
                        break;

                    case weapon_type::dual_pistol:
                        v2 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(210, field_1AC_cam.x, field_1AC_cam.y, field_1AC_cam.z, gDummyPedAng_6787A8);
                        break;
                    default:
                        return;
                }

                if (v2)
                {
                    v2->SetO8Timer_434130(9);
                }
            }
        }
    }
}

// https://decomp.me/scratch/PNiZo
WIP_FUNC(0x45e4a0)
void Ped::StartCrossingRoad_45E4A0()
{
    WIP_IMPLEMENTED;

    if (field_240_occupation != 3 || field_258_objective != 0 || field_25C_internal_objective != 0)
    {
        return;
    }

    u8 x = field_1AC_cam.x.ToInt();
    s32 y = field_1AC_cam.y.ToInt();
    u8 z = field_1AC_cam.z.ToInt() - 1;

    s8 direction;

    // 9.6f: 0x433470/0x4334A0/0x4334D0/0x433500 (N/E/S/W). The first two rng results are
    // tested as u8 (test al,al; ja), the last two as s16 (test ax,ax; jg), in both builds.
    if (gMap_0x370_6F6268->IsBlockRoadTypeInlined_433470(x, y - 1, z))
    {
        direction = 0;
        if ((u8)gRng_6F6784.get_int_4F7AE0(2) > 0)
        {
            goto dispatch;
        }
    }

    if (gMap_0x370_6F6268->IsBlockRoadTypeInlined_433470(x + 1, y, z))
    {
        direction = 1;
        if ((u8)gRng_6F6784.get_int_4F7AE0(2) > 0)
        {
            goto dispatch;
        }
    }

    if (gMap_0x370_6F6268->IsBlockRoadTypeInlined_433470(x, y + 1, z))
    {
        direction = 2;
        if (gRng_6F6784.get_int_4F7AE0(2) > 0)
        {
            goto dispatch;
        }
    }

    if (gMap_0x370_6F6268->IsBlockRoadTypeInlined_433470(x - 1, y, z))
    {
        direction = 3;
        if (gRng_6F6784.get_int_4F7AE0(2) > 0)
        {
            goto dispatch;
        }
    }

    direction = 4;

dispatch:
    switch (direction)
    {
        case 0:
            SetObjective2_463830(0x2C, 0x270F);
            field_130 = gSpawnRotationBottom_678540;
            ++gNumPedsCrossingRoad_6787D0;
            break;
        case 1:
            SetObjective2_463830(0x2D, 0x270F);
            field_130 = gSpawnRotationLeft_6786E0;
            ++gNumPedsCrossingRoad_6787D0;
            break;
        case 2:
            SetObjective2_463830(0x2E, 0x270F);
            field_130 = gSpawnRotationTop_6787B0;
            ++gNumPedsCrossingRoad_6787D0;
            break;
        case 3:
            SetObjective2_463830(0x2F, 0x270F);
            field_130 = gSpawnRotationRight_678578;
            ++gNumPedsCrossingRoad_6787D0;
            break;
    }
}

MATCH_FUNC(0x45ea00)
void Ped::DeallocateWithGroupCleanup_45EA00()
{
    if (field_164_ped_group)
    {
        if (get_field_20e() <= 0x1E)
        {
            return;
        }

        if (field_23C_group_idx == 99)
        {
            if (field_164_ped_group->IsAllMembersInSomeCar_4CAA20())
            {
                u8 i = 0;
                Ped* member = field_164_ped_group->field_4_ped_list[0];

                while (member)
                {
                    member->reset_ped_group();
                    member->Deallocate_45EB60();
                    ++i;
                    member = field_164_ped_group->field_4_ped_list[i];
                }

                field_164_ped_group->ClearGroupData_4C8E90();
            }
            else
            {
                bool all_members_in_car = true;
                u8 i = 0;
                Ped* member = field_164_ped_group->field_4_ped_list[0];

                while (member)
                {
                    if (member->get_field_20e() < 0x1E && member->field_168_game_object)
                    {
                        all_members_in_car = false;
                    }

                    ++i;
                    member = field_164_ped_group->field_4_ped_list[i];
                }

                if (!all_members_in_car)
                {
                    return;
                }

                member = field_164_ped_group->field_4_ped_list[0];
                i = 0;

                while (member)
                {
                    member->reset_ped_group();
                    member->Deallocate_45EB60();
                    ++i;
                    member = field_164_ped_group->field_4_ped_list[i];
                }

                field_164_ped_group->ClearGroupData_4C8E90();
            }
        }
        else
        {
            field_164_ped_group->RemovePed_4C9970(this);
        }
    }
    else
    {
        if (get_field_20e() <= 0x1E)
        {
            return;
        }
    }

    Deallocate_45EB60();
    field_21C_bf.b10 = 0;
}

// https://decomp.me/scratch/jJ6aF
// Clearing the flag through a reference keeps VC6 from hoisting the field_16C_car load above it.
// The flags word is passed and returned by value: VC6 then loads it into eax before the timer store
// and clears the bit with the short `and al, 0xFE` form, like the original
static inline CompilerBitField32 ClearBit0_45EB60(CompilerBitField32 bf)
{
    bf.b0 = 0;
    return bf;
}

MATCH_FUNC(0x45eb60)
void Ped::Deallocate_45EB60()
{
    switch (field_240_occupation)
    {
        case ped_ocupation_enum::mugger:
            --gNumberMuggersSpawned_6787CA;
            break;
        case ped_ocupation_enum::car_thief:
            --gNumberCarThiefsSpawned_6787CB;
            break;
        case ped_ocupation_enum::elvis_leader:
            if (gNumberElvisLeadersSpawned_6787CC > 0)
            {
                --gNumberElvisLeadersSpawned_6787CC;
            }
            break;
        case ped_ocupation_enum::guard:
            --gNumberWalkingCopsSpawned_6787CD;
            break;
        case ped_ocupation_enum::armed_gang_member_19:
            --gNumberArmedGangMembers_6787CE;
            break;
        case ped_ocupation_enum::bus_customer_8:
            if (--gNumberBusCustomers_6787D3 < 0)
            {
                gNumberBusCustomers_6787D3 = 0;
            }
            break;
        default:
            break;
    }
    if (field_200_id)
    {
        if (field_170_selected_weapon)
        {
            Ped::RemovePedWeapons_462510();
        }
        if (field_174_pWeapon)
        {
            Ped::RemoveSecondaryWeapon_462550();
        }
    }

    if (field_267_varrok_idx)
    {
        gVarrok_7F8_703398->Clear_434070(field_267_varrok_idx);
    }
    if (field_21C_bf.b14)
    {
        gOrca_2FD4_6FDEF0->field_3C_ped_list.RemovePed_471240(this);
    }
    gOrca_2FD4_6FDEF0->remove_ped_554620(field_200_id);
    gThreateningPedsList_678468.RemovePed_471240(this);

    if (field_17C_pGang)
    {
        if (field_17C_pGang->field_141)
        {
            field_17C_pGang->field_141 = 0;
        }
    }
    field_164_ped_group = field_164_ped_group;
    if (field_164_ped_group)
    {
        if (field_23C_group_idx == 99)
        {
            if (field_164_ped_group->IsAllMembersInSomeCar_4CAA20())
            {
                field_164_ped_group->ClearGroupData_4C8E90();
            }
            else
            {
                if ((field_240_occupation != ped_ocupation_enum::elvis_leader && field_240_occupation != ped_ocupation_enum::elvis))
                {
                    field_164_ped_group->DestroyGroup_4C93A0();
                }
                else if (field_164_ped_group->AreAllMembersOffScreen_4C9150())
                {
                    field_164_ped_group->DestroyGroup_4C93A0();
                }
                else
                {
                    field_164_ped_group->DisbandGroupDueToAttack_4C94E0(field_1A8_ped_killer);
                }
            }
        }
        else if (field_240_occupation == ped_ocupation_enum::elvis)
        {
            field_164_ped_group->DisbandGroupDueToAttack_4C94E0(field_1A8_ped_killer);
        }
        else
        {
            field_164_ped_group->RemovePed_4C9970(this);
        }
    }

    field_234_timer = 2;
    field_21C_bf = ClearBit0_45EB60(field_21C_bf);

    if (field_16C_car)
    {
        if (!field_248_enter_car_as_passenger)
        {
            field_16C_car->field_54_driver = 0;
            if (field_238_ped_type == ped_type::script_created_5)
            {
                if (field_16C_car->GetCarKind_4343B0() == 1)
                {
                    field_16C_car->field_7C_uni_num = 3;
                }
            }
        }
    }
    Ped::SetObjective(objectives_enum::no_obj_0, 9999);
    Ped::SetObjective2_463830(objectives_enum::no_obj_0, 9999);
    field_278_ped_state_1 = ped_state_1::dead_9;
    field_27C_ped_state_2 = ped_state_2::Unknown_15;
}

MATCH_FUNC(0x45edc0)
char_type Ped::sub_45EDC0()
{
    if (field_238_ped_type == ped_type::player_2)
    {
        if (field_240_occupation != ped_ocupation_enum::empty)
        {
            return true;
        }
    }
    return false;
}

MATCH_FUNC(0x45ede0)
bool Ped::IsField238_45EDE0(s32 ped_type)
{
    return field_238_ped_type == ped_type ? true : false;
}

// https://decomp.me/scratch/pXF8g
MATCH_FUNC(0x45ee00)
void Ped::SetOccupation_45EE00(u32 occupation)
{
    if (field_240_occupation <= (u32)ped_ocupation_enum::bank_robber)
    {
        switch (field_240_occupation)
        {
            case ped_ocupation_enum::player:
            case ped_ocupation_enum::empty:
            case ped_ocupation_enum::unknown_1:
            case ped_ocupation_enum::dummy:
            case ped_ocupation_enum::unknown_2:
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
                field_240_occupation = occupation;
                break;
            case ped_ocupation_enum::bus_customer_8:
                if (--gNumberBusCustomers_6787D3 < 0)
                    gNumberBusCustomers_6787D3 = 0;
                field_240_occupation = occupation;
                break;
        }
    }
    else
        field_240_occupation = occupation;
}

MATCH_FUNC(0x45ee70)
void Ped::EnterPublicTransport_45EE70()
{
    for (gmp_map_zone* pZoneIter = gMap_0x370_6F6268->first_zone_by_pos_4DF6A0(field_1AC_cam.x.ToInt(), field_1AC_cam.y.ToInt()); pZoneIter;
         pZoneIter = gMap_0x370_6F6268->next_zone_4DF770())
    {
        if (bSkip_trains_67D550 || pZoneIter->field_0_zone_type != Railway_Station_Platform_6)
        {
            if (!bSkip_buses_67D558)
            {
                if (gRng_6F6784.get_int_4F7AE0(100) > 90 && gNumberBusCustomers_6787D3 < 5 && pZoneIter->field_0_zone_type == 7 &&
                    !gPublicTransport_181C_6FF1D4->is_bus_full_579AF0())
                {
                    if (field_25C_internal_objective != 37 && field_25C_internal_objective != 38 && this->field_278_ped_state_1 == ped_state_1::walking_0)
                    {
                        SetOccupation_45EE00(8);
                        SetObjective2_463830(30, 9999);
                        ++gNumberBusCustomers_6787D3;
                    }
                }
            }
        }
        else
        {
            TrainStation_34* pTrainStation = gPublicTransport_181C_6FF1D4->TrainStationForZone_57B4B0(pZoneIter);
            if (gRng_6F6784.get_int_4F7AE0(100) > 90 && gNumberBusCustomers_6787D3 < 5)
            {
                if (field_25C_internal_objective != 37 && field_25C_internal_objective != 38 && field_25C_internal_objective != 12)
                {
                    Train_58* pTrain = pTrainStation->field_18;
                    if (pTrain)
                    {
                        if (pTrain->field_C_carriages[1]->GetCarInfoIdx_411940() == car_model_enum::TRAIN)
                        {
                            SetOccupation_45EE00(9);
                            SetObjective2_463830(29, 9999);
                            this->field_154_target_to_enter = pTrainStation->field_18->field_C_carriages[1];
                        }
                    }
                }
            }
        }
    }
}

MATCH_FUNC(0x45f360)
void Ped::Mugger_AI_45F360()
{
    if (field_25C_internal_objective == 2 && field_226_internal_objective_status == 1)
    {
        Ped::SetObjective2_463830(objectives_enum::no_obj_0, 9999);
    }
    switch (field_258_objective)
    {
        case objectives_enum::no_obj_0:
            if (!field_20e_offscreen_counter)
            {
                if (field_218_objective_timer == 0)
                {
                    Ped* pTarget = GetLastProcessedPedOnFoot_467070();
                    if (!pTarget || pTarget->Get_F20E_4039F0())
                    {
                        Ped::SetObjective(objectives_enum::no_obj_0, 9999);
                    }
                    else
                    {
                        Ped::SetObjective(objectives_enum::punch_char_23, 9999);
                        set_objective_target_ped_403AC0(pTarget);
                        field_229 = 0;
                        field_21C_bf.b11 = false;
                    }
                }
            }
            else
            {
                field_218_objective_timer = 40;
                Ped::ChangeNextPedState1_45C500(ped_state_1::walking_0);
                Ped::ChangeNextPedState2_45C540(ped_state_2::ped2_walking_0);
            }

            break;
        case objectives_enum::punch_char_23:
            if (field_148_objective_target_ped && field_148_objective_target_ped->has_car_403B80())
            {
                Ped::SetObjective(objectives_enum::no_obj_0, 9999);
                Ped::SetObjective2_463830(objectives_enum::no_obj_0, 9999);
            }
            else
            {
                if (field_225_objective_status == objective_status::passed_1)
                {
                    Ped::SetObjective(objectives_enum::wait_on_foot_26, 30);
                }
                else if (field_225_objective_status == objective_status::failed_2)
                {
                    Ped::SetObjective(objectives_enum::no_obj_0, 0);
                    Ped::SetObjective2_463830(objectives_enum::no_obj_0, 0);
                    field_21C_bf.b2 = false;
                }
            }
            break;

        case objectives_enum::wait_on_foot_26:
            field_12C + kAng180_678590; // non used
            field_21C_bf.b11 = true;
            Ped::SetObjective(objectives_enum::flee_on_foot_till_safe_1, 9999);
            field_1DC_objective_target_x = field_1AC_cam.x;
            field_1E0_objective_target_y = field_1AC_cam.y;
            field_1E4_objective_target_z = field_1AC_cam.z;
            break;

        case objectives_enum::flee_on_foot_till_safe_1:
            if (field_225_objective_status == objective_status::passed_1)
            {
                field_21C_bf.b11 = false;
                field_278_ped_state_1 = ped_state_1::walking_0;
                field_27C_ped_state_2 = ped_state_2::ped2_walking_0;
                Ped::SetObjective(objectives_enum::no_obj_0, 9999);
                Ped::SetOccupation_45EE00(3);
                --gNumberMuggersSpawned_6787CA;
            }
            break;

        case objectives_enum::flee_char_on_foot_till_safe_2:
            if (field_225_objective_status != objective_status::not_finished_0)
            {
                Ped::SetObjective(objectives_enum::no_obj_0, 9999);
            }
            break;

        default:
            return;
    }
}

MATCH_FUNC(0x45ff60)
void Ped::CarThief_AI_45FF60()
{

    Car_BC* pNearestSteal; // eax
    Car_BC* pNearestSteal_; // edi
    Car_BC* pCar_; // ecx
    Car_BC* pCar; // eax
    Fix16 xd;
    Fix16 yd;

    if (this->field_25C_internal_objective == 2 && this->field_226_internal_objective_status == 1)
    {
        SetObjective2_463830(objectives_enum::no_obj_0, 9999);
    }

    switch (this->field_258_objective)
    {
        case objectives_enum::no_obj_0:
            if ((this->field_21C & 0x8000000) != 0)
            {
                if (field_150_target_objective_car)
                {
                    if (field_150_target_objective_car->IsDespawning_4215B0())
                    {
                        Kill_46F9D0();
                        return;
                    }
                }
                else
                {
                    ForceDoNothing_462590();
                    this->field_240_occupation = ped_ocupation_enum::dummy;
                }
            }
            if (!this->field_20e_offscreen_counter)
            {
                if (!this->field_218_objective_timer)
                {
                    pNearestSteal = gCar_6C_677930->GetNearestEnterableCarFromCoord_444FA0(this->field_1AC_cam.x,
                                                                                           this->field_1AC_cam.y,
                                                                                           this->field_1AC_cam.z,
                                                                                           this);
                    pNearestSteal_ = pNearestSteal;
                    if (pNearestSteal)
                    {
                        if (pNearestSteal->field_7C_uni_num != 2)
                        {
                            if (!pNearestSteal->IsTrainModel_403BA0() && !pNearestSteal_->IsPoliceCar_439EC0() &&
                                pNearestSteal_->field_84_car_info_idx != car_model_enum::BUS &&
                                pNearestSteal_->field_4_passengers_list.IsEmpty_420EA0() && pNearestSteal_->field_7C_uni_num == 3)
                            {
                                SetObjective(objectives_enum::enter_car_as_driver_35, 9999);
                                this->field_150_target_objective_car = pNearestSteal_;
                                this->field_248_enter_car_as_passenger = 0;
                                this->field_24C_target_car_door = 0;
                            }
                        }
                    }
                }
            }
            else
            {
                this->field_218_objective_timer = 40;
                ChangeNextPedState1_45C500(0);
                ChangeNextPedState2_45C540(0);
            }
            return;

        case objectives_enum::enter_car_as_driver_35:
            if (field_225_objective_status == 1)
            {
                if (this->field_150_target_objective_car->IsDespawning_4215B0())
                {
                    Kill_46F9D0();
                    return;
                }
                if (this->field_27C_ped_state_2 == ped_state_2::Unknown_17)
                {
                    SetObjective(objectives_enum::no_obj_0, 40);
                    SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                }
                else
                {
                    Set_F250_IfBit_433DD0(15);
                    SetObjective(objectives_enum::time_waited_in_car_31, 0);
                    pCar_ = this->field_16C_car;
                    this->field_150_target_objective_car = pCar_;
                    pCar_->InitCarAIControl_440590();
                    field_150_target_objective_car->sub_43AF40();
                }
                return;
            }

            if (field_225_objective_status == 2)
            {
                SetObjective(objectives_enum::no_obj_0, 40);
                SetObjective2_463830(objectives_enum::no_obj_0, 9999);
            }
            else if (this->field_16C_car && this->field_150_target_objective_car->IsDespawning_4215B0())
            {
                Kill_46F9D0();
                return;
            }

            xd = this->field_1B8_target_x - this->field_1AC_cam.x;
            yd = this->field_1BC_target_y - this->field_1AC_cam.y;

            xd = Fix16::Abs(xd);
            yd = Fix16::Abs(yd);
            // The larger of the two, picked through a pointer
            if (*(xd > yd ? &xd : &yd) > kFpFour_678680)
            {
                SetObjective(objectives_enum::no_obj_0, 9999);
                SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                this->field_218_objective_timer = 40;
            }
            return;

        case objectives_enum::time_waited_in_car_31:
            pCar = this->field_16C_car;
            if (!pCar)
            {
                goto set_no_obj; // shared with flee_on_foot_till_safe_1
            }
            if (pCar->IsDespawning_4215B0())
            {
                Kill_46F9D0();
                return;
            }
            if (this->field_218_objective_timer > 0x258u)
            {
                SetObjective(objectives_enum::leave_car_36, 9999);
                this->field_150_target_objective_car = this->field_16C_car;
            }
            return;

        case objectives_enum::leave_car_36:
            if (this->field_225_objective_status == 1)
            {
                SetObjective(objectives_enum::flee_on_foot_till_safe_1, 9999);
                this->field_1B8_target_x = this->field_1AC_cam.x;
                this->field_1BC_target_y = this->field_1AC_cam.y;
                this->field_1C0_target_z = this->field_1AC_cam.z;
            }
            return;

        case objectives_enum::flee_on_foot_till_safe_1:
            if (this->field_225_objective_status == 1)
            {
            set_no_obj:
                SetObjective(objectives_enum::no_obj_0, 40);
            }
            return;

        default:
            return;
    }
}

MATCH_FUNC(0x460820)
void Ped::TaxiCustomer_AI_460820()
{
    if (this->field_25C_internal_objective == objectives_enum::flee_char_on_foot_till_safe_2)
    {
        if (this->field_226_internal_objective_status == 1)
        {
            SetObjective2_463830(objectives_enum::no_obj_0, 9999);
        }
    }

    switch (this->field_258_objective)
    {
        // This case comes first: the other cases' inline sites after MaxAbsDistance_42A6B0 leave its
        // y difference and Abs out of line, as in the original.
        case objectives_enum::no_obj_0:
            // It has no objective
            if (!field_20e_offscreen_counter)
            {
                if (field_218_objective_timer == 0)
                {
                    // Look for a near taxi
                    Car_BC* pTaxi = gTaxi_4_704130->GetTaxiNear_457BF0(this->field_1AC_cam.x, this->field_1AC_cam.y);
                    if (pTaxi)
                    {
                        Fix16 dist;
                        dist = Fix16::MaxAbsDistance_42A6B0(this->field_1AC_cam.x,
                                                            this->field_1AC_cam.y,
                                                            pTaxi->field_50_car_sprite->field_14_xy.x,
                                                            pTaxi->field_50_car_sprite->field_14_xy.y);
                        if (dist < kFpTwo_678658)
                        {
                            Set_F250_IfBit_433DD0(5);
                            SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                            SetObjective(objectives_enum::enter_car_as_driver_35, 9999);
                            this->field_150_target_objective_car = pTaxi;
                            this->field_248_enter_car_as_passenger = 1;
                            this->field_24C_target_car_door = 3;
                            pTaxi->sub_43AF60();
                        }
                    }
                }
            }
            else
            {
                // Set a little timer before looking for a taxi
                field_218_objective_timer = 40;
                ChangeNextPedState1_45C500(ped_state_1::walking_0);
                ChangeNextPedState2_45C540(ped_state_2::ped2_walking_0);
            }
            break;

        case objectives_enum::enter_car_as_driver_35: // TODO: shouldn't it be enter car as passenger?
        {
            // It is on foot
            u8 status = this->field_225_objective_status;
            if (status == objective_status::passed_1)
            {
                // It entered the taxi
                if (this->field_150_target_objective_car->IsDespawning_4215B0())
                {
                    // Taxi is wreck, kill it
                    Kill_46F9D0();
                    return;
                }
                Set_F250_IfBit_433DD0(6);
                this->field_150_target_objective_car->sub_43AF40();
                SetObjective(objectives_enum::time_waited_in_car_31, 0);
                this->field_150_target_objective_car = this->field_16C_car;
            }
            else if (status == objective_status::failed_2)
            {
                // Ped failed to reach car
                Car_BC* pTaxi = this->field_150_target_objective_car;
                if (!pTaxi->IsDespawning_4215B0())
                {
                    // reinit taxi AI?
                    pTaxi->sub_43AF40();
                }
                SetObjective(objectives_enum::no_obj_0, 40);
                SetObjective2_463830(objectives_enum::no_obj_0, 9999);
            }
            else if (field_278_ped_state_1 != ped_state_1::in_car_10)
            {
                // It not entered the taxi yet
                Fix16 dx;
                Fix16 dy;
                dx = this->field_1B8_target_x - this->field_1AC_cam.x;
                dy = this->field_1BC_target_y - this->field_1AC_cam.y;
                dx = Fix16::Abs(dx);
                dy = Fix16::Abs(dy);

                if (((dx > dy) ? dx : dy) > kFpTwo_678658 || (this->field_21C & 0x20000) != 0)
                {
                    this->field_150_target_objective_car->sub_43AF40();
                    SetObjective(objectives_enum::no_obj_0, 9999);
                    SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                    this->set_occupation_403970(ped_ocupation_enum::dummy);
                    this->SetField238_403920(ped_type::dummy_3);
                    return;
                }
                Car_BC* pCar = this->field_150_target_objective_car;
                if (pCar->field_4_passengers_list.IsEmpty_420EA0() && !pCar->IsDespawning_4215B0())
                {
                    break;
                }
                pCar->sub_43AF40();
                SetObjective(objectives_enum::no_obj_0, 9999);
                SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                this->set_occupation_403970(ped_ocupation_enum::dummy);
                this->SetField238_403920(ped_type::dummy_3);
            }
            else if (this->field_150_target_objective_car->IsDespawning_4215B0())
            {
                Kill_46F9D0();
            }
            break;
        }

        case objectives_enum::time_waited_in_car_31:
        {
            // It is in the taxi
            if (field_150_target_objective_car->GetVelocity_43A4C0() != kFpZero_678660)
            {
                field_218_objective_timer = 0; // taxi is moving, reset timer
            }
            Car_BC* pTaxi = this->field_150_target_objective_car;
            if (pTaxi->IsDespawning_4215B0())
            {
                Kill_46F9D0(); // taxi is wreck/destroyed, kill the passenger
            }
            else
            {
                if (pTaxi->field_8C_damage_level >= 3)
                {
                    this->field_21C |= 0x20000000u;
                }
                if (pTaxi->field_54_driver && (this->field_21C & 0x20000000) == 0)
                {
                    if (this->field_218_objective_timer == 150)
                    {
                        // taxi is stopped long enough, exit the car
                        SetObjective(objectives_enum::flee_char_always_once_car_stopped_6, 9999);
                        this->field_148_objective_target_ped = this->field_16C_car->field_54_driver;
                    }
                }
                else
                {
                    // taxi without driver -> exit
                    SetObjective(objectives_enum::leave_car_36, 9999);
                    SetOccupation_45EE00(3);
                    this->SetField238_403920(ped_type::dummy_3);
                    this->field_150_target_objective_car = this->field_16C_car;
                }
            }
            break;
        }
    }
}

MATCH_FUNC(0x461290)
void Ped::BusCustomer_AI_461290()
{
    Car_BC* pCar_;

    if (this->field_25C_internal_objective == 2 && this->field_226_internal_objective_status == 1)
    {
        SetObjective2_463830(objectives_enum::no_obj_0, 9999);
    }

    switch (this->field_258_objective)
    {
        case objectives_enum::leave_train_38:
            if (this->field_225_objective_status != objective_status::not_finished_0)
            {
                this->SetField238_403920(ped_type::dummy_3);
                SetOccupation_45EE00(3);
                SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                SetObjective(objectives_enum::flee_on_foot_till_safe_1, 9999);
                this->field_1B8_target_x = this->field_1AC_cam.x;
                this->field_1BC_target_y = this->field_1AC_cam.y;
            }
            else if (this->field_150_target_objective_car->IsDespawning_4215B0())
            {
                Kill_46F9D0();
            }
            break;

        case objectives_enum::enter_car_as_driver_35:
            if (this->field_225_objective_status == objective_status::passed_1)
            {
                if (--gNumberBusCustomers_6787D3 < 0)
                {
                    gNumberBusCustomers_6787D3 = 0;
                }
                Car_BC* pCar = this->field_16C_car;
                if (pCar->is_driven_by_player())
                {
                    gPublicTransport_181C_6FF1D4->IncrementBusPassengerCount_579B10();
                    SetObjective(objectives_enum::time_waited_in_car_31, 0);
                }
                else
                {
                    pCar->field_4_passengers_list.RemovePed_471240(this);
                    Kill_46F9D0();
                }
            }
            else
            {
                Car_BC* pTargetCar = this->field_150_target_objective_car;
                if (pTargetCar->IsDespawning_4215B0())
                {
                    pTargetCar->sub_43AF40();
                    SetObjective(objectives_enum::no_obj_0, 9999);
                    SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                    this->set_occupation_403970(3);
                    this->SetField238_403920(ped_type::dummy_3);
                }
                else
                {
                    pTargetCar->GetDoor(get_target_car_door_403A60())->Open_439E60();
                }
            }
            return;

        case objectives_enum::time_waited_in_car_31:
            pCar_ = this->field_16C_car;
            if (pCar_->IsDespawning_4215B0())
            {
                pCar_->field_4_passengers_list.RemovePed_471240(this);
                Kill_46F9D0();
            }
            break;

        case objectives_enum::objective_34:
            if (this->field_25C_internal_objective == 36 && this->field_226_internal_objective_status == 1)
            {
                this->SetField238_403920(ped_type::dummy_3);
                SetOccupation_45EE00(3);
                SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                SetObjective(objectives_enum::flee_on_foot_till_safe_1, 9999);
                this->field_1B8_target_x = this->field_1AC_cam.x;
                this->field_1BC_target_y = this->field_1AC_cam.y;
            }
            else
            {
                pCar_ = this->field_16C_car;
                if (pCar_ && pCar_->IsDespawning_4215B0())
                {
                    pCar_->field_4_passengers_list.RemovePed_471240(this);
                    Kill_46F9D0();
                }
            }
            break;

        default:
            Car_BC* pBus = gPublicTransport_181C_6FF1D4->sub_579AD0();
            if (pBus)
            {
                SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                SetObjective(objectives_enum::enter_car_as_driver_35, 9999);
                this->field_150_target_objective_car = pBus;
                this->field_168_game_object->Set_F84_433900(this->field_154_target_to_enter);
                this->field_168_game_object->SetMaxSpeedByRef_433920(kFpZero_678660);
                set_target_car_door_403A70(1);
            }
            break;
    }
}

MATCH_FUNC(0x461530)
void Ped::TrainCustomer_AI_461530()
{
    if (this->field_25C_internal_objective == 2 && this->field_226_internal_objective_status == 1)
    {
        SetObjective2_463830(objectives_enum::no_obj_0, 9999);
    }

    switch (field_25C_internal_objective)
    {
        case 37:
            if (field_154_target_to_enter->GetVelocity_43A4C0() != kFpZero_678660)
            {
                this->SetField238_403920(ped_type::dummy_3);
                SetOccupation_45EE00(3);
                SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                SetObjective(objectives_enum::no_obj_0, 9999);
                this->field_1B8_target_x = this->field_1AC_cam.x;
                this->field_1BC_target_y = this->field_1AC_cam.y;
            }
            break;

        case 38:
            if (this->field_226_internal_objective_status)
            {
                this->SetField238_403920(ped_type::dummy_3);
                SetOccupation_45EE00(3);
                SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                SetObjective(objectives_enum::flee_on_foot_till_safe_1, 9999);
                this->field_1B8_target_x = this->field_1AC_cam.x;
                this->field_1BC_target_y = this->field_1AC_cam.y;
            }
            break;
    }
}

MATCH_FUNC(0x461630)
void Ped::RobbedDriver_AI_461630()
{
    if (field_25C_internal_objective == 2 && field_226_internal_objective_status == 1)
    {
        SetObjective2_463830(objectives_enum::no_obj_0, 9999);
    }

    switch (field_240_occupation)
    {
        case ped_ocupation_enum::robbed_driver_10:
            if (field_168_game_object)
            {
                if (field_278_ped_state_1 != ped_state_1::immobilized_8)
                {
                    u16 rng_val = gRng_6F6784.get_int_4F7AE0(40);
                    if (bDont_get_car_back_67D4F5)
                    {
                        rng_val = 6;
                    }
                    if (!field_180_car_thief->field_20e_offscreen_counter)
                    {
                        if (field_17C_pGang)
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
                            this->SetField238_403920(ped_type::dummy_with_occupation_6);
                            this->field_240_occupation = ped_ocupation_enum::very_angry_armed_robbed_driver_13;
                            Set_F250_IfBit_433DD0(14);
                            ForceDoNothing_462590();
                            SetObjective(objectives_enum::kill_char_on_foot_20, 9999);
                            this->field_148_objective_target_ped = this->field_180_car_thief;
                            ForceWeapon_46F600(weapon_type::pistol);
                            break;

                        case 7:
                        case 16:
                        case 32:
                            // 3 over 40 = 7.5% of chance of being an armed and angry driver
                            this->SetField238_403920(ped_type::dummy_with_occupation_6);
                            this->field_240_occupation = ped_ocupation_enum::angry_armed_robbed_driver_12;
                            Set_F250_IfBit_433DD0(13);
                            SetObjective(objectives_enum::enter_car_as_driver_35, 9999);
                            this->field_24C_target_car_door = 0;
                            this->field_150_target_objective_car = field_140_stolen_car;
                            if (field_140_stolen_car->IsDespawning_4215B0())
                            {
                                this->SetField238_403920(ped_type::dummy_3);
                                this->field_240_occupation = ped_ocupation_enum::fleeing_robbed_driver_11;
                                ForceDoNothing_462590();
                                SetObjective(objectives_enum::flee_char_on_foot_till_safe_2, 9999);
                                this->field_148_objective_target_ped = this->field_180_car_thief;
                                this->field_28C_threat_reaction = threat_reaction_enum::run_away_3;
                                this->field_180_car_thief = 0;
                            }
                            else
                            {
                                ForceWeapon_46F600(weapon_type::pistol);
                                this->field_248_enter_car_as_passenger = 0;
                            }
                            break;

                        default:
                            // normal behaviour: flee
                            this->SetField238_403920(ped_type::dummy_3);
                            this->field_240_occupation = ped_ocupation_enum::fleeing_robbed_driver_11;
                            this->field_28C_threat_reaction = threat_reaction_enum::run_away_3;
                            ForceDoNothing_462590();
                            SetObjective(objectives_enum::flee_char_on_foot_till_safe_2, 9999);
                            this->field_148_objective_target_ped = this->field_180_car_thief;
                            this->field_180_car_thief = 0;
                            break;
                    }
                }
                else
                {
                    if (this->field_140_stolen_car->IsDespawning_4215B0())
                    {
                        this->SetField238_403920(ped_type::dummy_3);
                        this->field_240_occupation = ped_ocupation_enum::fleeing_robbed_driver_11;
                        ForceDoNothing_462590();
                        SetObjective(objectives_enum::flee_char_on_foot_till_safe_2, 9999);
                        this->field_148_objective_target_ped = this->field_180_car_thief;
                        this->field_180_car_thief = 0;
                    }
                }
            }
            return;

        case ped_ocupation_enum::angry_armed_robbed_driver_12:
            if (field_225_objective_status == objective_status::passed_1)
            {
                Car_BC* target_objective_car = this->field_150_target_objective_car;
                if (target_objective_car->IsDespawning_4215B0())
                {
                    Kill_46F9D0();
                }
                else
                {
                    this->field_240_occupation = ped_ocupation_enum::driver;
                    this->SetField238_403920(ped_type::dummy_3);
                    if (target_objective_car)
                    {
                        target_objective_car->SetUniNum_421560(3);
                        SetObjective(objectives_enum::no_obj_0, 9999);
                    }
                    else
                    {
                        SetObjective(objectives_enum::no_obj_0, 9999);
                    }
                }
            }
            else if (field_225_objective_status == objective_status::failed_2)
            {
                this->field_240_occupation = ped_ocupation_enum::dummy;
                SetObjective(objectives_enum::no_obj_0, 9999);
                SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                this->SetField238_403920(ped_type::dummy_3);
            }
            return;

        case ped_ocupation_enum::very_angry_armed_robbed_driver_13:
            if (this->field_225_objective_status == objective_status::passed_1)
            {
                this->SetField238_403920(ped_type::dummy_with_occupation_6);
                this->field_240_occupation = ped_ocupation_enum::angry_armed_robbed_driver_12;
                if (field_140_stolen_car && !field_140_stolen_car->IsDespawning_4215B0())
                {
                    SetObjective(objectives_enum::enter_car_as_driver_35, 9999);
                    this->field_248_enter_car_as_passenger = 0;
                    this->field_150_target_objective_car = field_140_stolen_car;
                    this->field_24C_target_car_door = 0;
                }
                else
                {
                    this->field_240_occupation = ped_ocupation_enum::dummy;
                    SetObjective(objectives_enum::no_obj_0, 9999);
                    SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                    this->SetField238_403920(ped_type::dummy_3);
                }
            }
            else
            {
                if (field_140_stolen_car && field_140_stolen_car->IsDespawning_4215B0())
                {
                    this->field_140_stolen_car = 0;
                }

                if (!this->field_140_stolen_car)
                {
                    this->field_225_objective_status = objective_status::not_finished_0;
                }

                if (field_225_objective_status == objective_status::failed_2)
                {
                    this->field_240_occupation = ped_ocupation_enum::dummy;
                    SetObjective(objectives_enum::no_obj_0, 9999);
                    SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                    this->SetField238_403920(ped_type::dummy_3);
                }
            }
            return;

        case ped_ocupation_enum::fleeing_robbed_driver_11:
            if (this->field_225_objective_status == objective_status::passed_1)
            {
                this->field_240_occupation = ped_ocupation_enum::dummy;
                SetObjective(objectives_enum::no_obj_0, 9999);
            }
            return;

        default:
            return;
    }
}

MATCH_FUNC(0x4619f0)
void Ped::RoadBlockTank_AI_4619F0()
{
    if (!field_16C_car)
    {
        this->field_240_occupation = ped_ocupation_enum::dummy;
    }
    else
    {
        field_16C_car->SetA6Bit5_421540();
    }

    if (this->field_28C_threat_reaction != threat_reaction_enum::react_as_emergency_1 || gPolice_7B8_6FEE40->field_654_wanted_level == 6)
    {
        if (this->field_258_objective == objectives_enum::no_obj_0)
        {
            SetObjective(objectives_enum::objective_61, 9999);
        }
    }
    else
    {
        SetObjective(objectives_enum::no_obj_0, 9999);
        this->field_21C &= ~0x800;
    }
}

// For entering a car angles the player ped towards the car door
MATCH_FUNC(0x461a60)
void Ped::UpdateFacingAngle_461A60()
{
    if (this->field_258_objective)
    {
        if (!this->field_25C_internal_objective)
        {
            this->field_1C4_x = field_1B8_target_x;
            this->field_1C8_y = field_1BC_target_y;
            this->field_1CC_z = field_1C0_target_z;
        }
    }

    switch (this->field_278_ped_state_1)
    {
        case ped_state_1::findind_path_2:
        {
            Fix16 x = this->field_1C4_x;
            Fix16 y = this->field_1C8_y;
            this->field_130 = Fix16::atan2_fixed_405320(y - this->field_1AC_cam.y, x - this->field_1AC_cam.x);
            break;
        }

        case ped_state_1::entering_car_3:
        {
            this->field_130 = Fix16::atan2_fixed_405320(this->field_1C8_y - this->field_1AC_cam.y, this->field_1C4_x - this->field_1AC_cam.x);
            break;
        }

        case ped_state_1::flee_or_running_1:
        {
            if (this->field_27C_ped_state_2 == ped_state_2::Unknown_3)
            {
                this->field_130 = Fix16::atan2_fixed_405320(this->field_1AC_cam.y - this->field_1C8_y, this->field_1AC_cam.x - this->field_1C4_x);
            }

            if (this->field_27C_ped_state_2 == ped_state_2::Unknown_2)
            {
                this->field_130 = Fix16::atan2_fixed_405320(this->field_1C8_y - this->field_1AC_cam.y, this->field_1C4_x - this->field_1AC_cam.x);

                if (byte_6787C4 && this->field_14C_internal_target_ped && gDistanceToTarget_678750 < kFpThreeSixteenths_678780 &&
                    ComputeShortestAngleDelta_4056C0(field_130, field_12C) > kAng90_6784B0)
                {
                    field_130 = this->field_12C;
                    this->field_168_game_object->SetMaxSpeed_433920(field_14C_internal_target_ped->GetPedVelocity_45C920() -
                                                                     kFpOne64th_678430);
                    return;
                }

                if (byte_6787D4 == 1)
                {
                    this->field_168_game_object->field_6A = 1;
                    Ang16 jitter;
                    if ((this->field_200_id & 1) != 0)
                    {
                        jitter = Ang16::Fix16_To_Ang16_40F540(kFpOne64th_6784C4 * Fix16(gRng_6F6784.get_int_4F7AE0(45)));
                        this->field_168_game_object->field_74 = field_130 + kAng90_6784B0 + jitter;
                    }
                    else
                    {
                        // 9.6f uses Fix16_To_Ang16 and operator- here too, but 10.5 needs the
                        // conversion written out and the subtraction split to inline like the original
                        jitter.rValue = (kFpOne64th_6784C4 * Fix16(gRng_6F6784.get_int_4F7AE0(45))).GetRaw_40F4B0() / 71;
                        jitter.Normalize();
                        Ang16 tmp = field_130 - kAng90_6784B0;
                        tmp = tmp - jitter;
                        this->field_168_game_object->field_74 = tmp;
                    }
                }
                else if (GetPedVelocity_45C920() < kFpZero_678660)
                {
                    field_130 += kAng180_6785A6;
                }
            }
            break;
        }

        case ped_state_1::standing_still_7:
        {
            if (this->field_27C_ped_state_2 == 11) // ped_state_2::Unknown_11)
            {
                this->field_130 = Fix16::atan2_fixed_405320(this->field_1C8_y - this->field_1AC_cam.y, this->field_1C4_x - this->field_1AC_cam.x);
            }
            break;
        }

        // Needed for the case 3 -> case 2 tail merge
        default:
            break;
    }
    this->field_12E_aim_angle = this->field_130;
}

MATCH_FUNC(0x461f20)
void Ped::Occupation_AI_461F20()
{
    switch (field_240_occupation)
    {
        case ped_ocupation_enum::dummy:
            if ((field_21C & 4) == 0 && field_258_objective == objectives_enum::no_obj_0 && !field_164_ped_group)
            {
                if (field_168_game_object)
                {
                    if (field_20e_offscreen_counter || gNewTaxiCustomersThisTick_6787D2 || gRng_6F6784.get_int_4F7AE0(1000) >= 2)
                    {
                        Ped::EnterPublicTransport_45EE70();
                    }
                    else if (!gTaxi_4_704130->IsEmpty_434970())
                    {
                        set_occupation_403970(ped_ocupation_enum::taxi_customer_7);
                        if (field_238_ped_type == ped_type::dummy_3)
                        {
                            SetField238_403920(ped_type::dummy_with_occupation_6);
                        }
                        Ped::SetObjective(objectives_enum::no_obj_0, 40);
                        ++gNewTaxiCustomersThisTick_6787D2;
                    }
                }
            }
            break;
        case ped_ocupation_enum::train_customer_9:
            Ped::TrainCustomer_AI_461530();
            break;
        case ped_ocupation_enum::bus_customer_8:
            Ped::BusCustomer_AI_461290();
            break;
        case ped_ocupation_enum::unknown_2:
            byte_61A8A0 = 0;
            break;
        case ped_ocupation_enum::driver:
            byte_61A8A0 = 0;
            if (field_16C_car)
            {
                if (field_16C_car->IsDespawning_4215B0())
                {
                    Ped::Kill_46F9D0();
                }
            }
            else
            {
                set_occupation_403970(ped_ocupation_enum::dummy);
            }
            break;
        case ped_ocupation_enum::taxi_customer_7:
            Ped::TaxiCustomer_AI_460820();
            break;

        case ped_ocupation_enum::mugger:
            Ped::Mugger_AI_45F360();
            break;
        case ped_ocupation_enum::car_thief:
            Ped::CarThief_AI_45FF60();
            break;
        case ped_ocupation_enum::robbed_driver_10:
        case ped_ocupation_enum::angry_armed_robbed_driver_12:
        case ped_ocupation_enum::very_angry_armed_robbed_driver_13:
            Ped::RobbedDriver_AI_461630();
            break;
        case ped_ocupation_enum::fleeing_robbed_driver_11:
            if (field_225_objective_status)
            {
                Ped::SetObjective(objectives_enum::no_obj_0, 9999);
                Ped::SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                unset_bitset_0x04();
            }
            break;
        case ped_ocupation_enum::road_block_tank_man:
            Ped::RoadBlockTank_AI_4619F0();
            break;
        case ped_ocupation_enum::mad_mugger_40:
            if (!gPedManager_6787BC->field_7_make_all_muggers)
            {
                set_occupation_403970(ped_ocupation_enum::dummy);
                SetField238_403920(ped_type::dummy_3);
                Ped::ForceDoNothing_462590();
            }
            break;
        case ped_ocupation_enum::armed_gang_member_19:
            if (field_25C_internal_objective == 20 && field_17C_pGang != NULL && field_14C_internal_target_ped->is_player_41B0A0())
            {
                u8 idx = field_14C_internal_target_ped->field_15C_player->get_idx_4219D0();
                if (!field_17C_pGang->IsRespectNegativeForPlayer_4BEF10(idx))
                {
                    Ped::SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                }
            }
            break;
        case ped_ocupation_enum::scared_driver_50:
            if (field_258_objective > objectives_enum::no_obj_0)
            {
                if (field_258_objective > objectives_enum::flee_char_on_foot_till_safe_2)
                {
                    if (field_258_objective == objectives_enum::leave_car_36)
                    {
                        if (field_150_target_objective_car->IsDespawning_4215B0())
                        {
                            Ped::Kill_46F9D0();
                        }
                        else if (field_225_objective_status == objective_status::passed_1)
                        {
                            if (field_180_car_thief)
                            {
                                Ped::SetObjective(objectives_enum::flee_char_on_foot_till_safe_2, 9999);
                                set_objective_target_ped_403AC0(field_180_car_thief);
                            }
                            else
                            {
                                Ped::SetObjective(objectives_enum::flee_on_foot_till_safe_1, 9999);
                                field_1DC_objective_target_x = field_1AC_cam.x;
                                field_1E0_objective_target_y = field_1AC_cam.y;
                                field_1E4_objective_target_z = field_1AC_cam.z;
                            }
                        }
                    }
                }
                else
                {
                    if (field_225_objective_status == objective_status::passed_1)
                    {
                        set_occupation_403970(ped_ocupation_enum::dummy);
                        Ped::SetObjective(objectives_enum::no_obj_0, 9999);
                        Ped::SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                    }
                }
            }
            break;
        default:
            return;
    }
}

MATCH_FUNC(0x462280)
void Ped::UpdateAI_462280()
{
    byte_61A8A0 = 1;
    Ped::Occupation_AI_461F20();
    byte_61A8A3 = 0;
    if (byte_61A8A0)
    {
        if (field_164_ped_group)
        {
            if (field_23C_group_idx != 99)
            {
                if (field_21C_bf.b2 == 0)
                {
                    field_164_ped_group->UpdateMemberAIState_4CA5E0(field_23C_group_idx);
                    byte_61A8A3 = 1;
                    byte_6787C4 = field_164_ped_group->field_38_group_type != 1;
                }
                else
                {
                    byte_61A8A3 = 0;
                    byte_6787C4 = 0;
                }
            }
            else if (field_21C_bf.b2 == 0)
            {
                byte_61A8A3 = field_164_ped_group->Get_F3C_433370() != 1;
                field_164_ped_group->CoordinateGroupCarEntry_4C9F00();
                byte_6787C4 = 1;
            }
            else
            {
                byte_61A8A3 = 0;
                byte_6787C4 = 0;
            }
        }
        else if (field_21C_bf.b2 == 0)
        {
            byte_61A8A3 = 1;
            byte_6787C4 = 1;
        }
        else
        {
            byte_6787C4 = 0;
        }

        field_21C_bf.b27 = 0;
        Ped::ProcessOnFootObjective_463AA0();
        Ped::ProcessInCarObjective_463FB0();
        if (field_278_ped_state_1 > 0 && field_278_ped_state_1 <= 7)
        {
            Ped::UpdateFacingAngle_461A60();
        }
        if (field_238_ped_type != ped_type::player_2)
        {
            if (field_258_objective != objectives_enum::flee_char_on_foot_always_3 &&
                field_258_objective != objectives_enum::flee_char_always_once_car_stopped_6)
            {
                if (field_25C_internal_objective != 3 && field_25C_internal_objective != 7)
                {
                    Ped::Threat_Reaction_AI_465270();
                    if (field_144_attacker)
                    {
                        if ((field_21C & 4) == 0)
                        {
                            Ped::ReactToAttacker_465B20();
                        }
                        field_144_attacker = 0;
                    }
                }
            }
        }
    }

    if (field_21A_car_state_timer != 9999)
    {
        if (field_21A_car_state_timer > 0)
        {
            field_21A_car_state_timer--;
        }
    }

    if (field_218_objective_timer != 9999)
    {
        if (field_258_objective != objectives_enum::time_waited_in_car_31 &&
            field_258_objective != objectives_enum::goto_area_any_means_13 &&
            field_258_objective != objectives_enum::kill_char_any_means_19 && field_258_objective != objectives_enum::goto_area_in_car_14)
        {
            if (field_218_objective_timer > 0)
            {
                field_218_objective_timer = field_218_objective_timer - 1;
            }
        }
    }
}

MATCH_FUNC(0x4624a0)
void Ped::ReleaseGroupSpritesAndWeapons_4624A0()
{
    if (field_164_ped_group)
    {
        field_164_ped_group->DestroyGroup_4C93A0();
    }

    if (field_168_game_object)
    {
        if (field_168_game_object->field_88_obj_2c.field_0_p18)
        {
            field_168_game_object->field_88_obj_2c.DestroyAllSprites_5A7010();
        }
    }

    if (field_200_id)
    {
        if (field_170_selected_weapon)
        {
            RemovePedWeapons_462510();
        }
        if (field_174_pWeapon)
        {
            RemoveSecondaryWeapon_462550();
        }
        field_178_car_weapon = 0;
    }
}

MATCH_FUNC(0x462510)
void Ped::RemovePedWeapons_462510()
{
    if (field_170_selected_weapon)
    {
        ClearBit11_403A40();
        gWeapon_8_707018->deallocate_5E3CB0(field_170_selected_weapon);
        field_170_selected_weapon = 0;
    }
}

MATCH_FUNC(0x462550)
void Ped::RemoveSecondaryWeapon_462550()
{
    if (field_174_pWeapon)
    {
        ClearBit11_403A40();
        gWeapon_8_707018->deallocate_5E3CB0(field_174_pWeapon);
        field_174_pWeapon = 0;
    }
}

MATCH_FUNC(0x462590)
void Ped::ForceDoNothing_462590()
{
    SetObjective(objectives_enum::no_obj_0, 9999);
    SetObjective2_463830(objectives_enum::no_obj_0, 9999);

    field_21C &= ~4u;

    if (field_16C_car)
    {
        field_278_ped_state_1 = ped_state_1::in_car_10;
        field_27C_ped_state_2 = ped_state_2::ped2_driving_10;

        if (field_16C_car->field_5C_AI)
        {
            if (field_16C_car->field_5C_AI->field_28_junc_idx > 0)
            {
                gRouteFinder_6FFDC8->CancelRoute_589930(field_16C_car->field_5C_AI->field_28_junc_idx);
            }
        }

        if (field_16C_car->field_60)
        {
            gHamburger_500_678E30->FreeEntry_474CC0(field_16C_car->field_60);
            field_16C_car->field_60 = 0;
        }
    }
}

MATCH_FUNC(0x462620)
void Ped::sub_462620()
{
    if (field_278_ped_state_1 == ped_state_1::dead_9 || field_278_ped_state_1 == ped_state_1::immobilized_8)
    {
        byte_61A8A4 = 0;
        field_21C_bf.b11 = false;
    }
    else
    {
        if (field_168_game_object->GetCharState_433A80() == Char_B4_state::Jumping_15)
        {
            byte_61A8A4 = field_278_ped_state_1 == ped_state_1::entering_car_3;
        }
        field_21C_bf.b11 = false;
        field_168_game_object->SetSpriteNum_4338F0(24);
    }
    if (field_168_game_object->IsOnScreen_545700() == true)
    {
        field_20e_offscreen_counter = 0;
        ++gNumPedsOnScreen_6787EC;
    }
}

MATCH_FUNC(0x4626b0)
char_type Ped::StateMachineTick_4626B0()
{
    switch (this->field_238_ped_type)
    {
        case ped_type::player_2:
            if (bDo_invulnerable_67D4CB)
            {
                this->field_208_invulnerability = 9999;
            }

            if (this->field_170_selected_weapon)
            {
                this->field_170_selected_weapon->Set_F4_433810(0);
            }

            this->field_288_threat_search = threat_search_enum::no_threats_0;
            this->field_28C_threat_reaction = threat_reaction_enum::no_reaction_0;

            if (this->field_240_occupation != ped_ocupation_enum::empty && this->field_240_occupation != ped_ocupation_enum::player)
            {
                this->field_240_occupation = ped_ocupation_enum::player;
            }
            this->field_230 = 2;
            this->field_15C_player->field_64_bJumping = 0;

            if (this->field_168_game_object)
            {
                if (this->field_225_objective_status == 2)
                {
                    Ped::SetObjective(objectives_enum::no_obj_0, 9999);
                    Ped::SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                }

                if (this->field_258_objective == objectives_enum::leave_car_36 &&
                    (this->field_225_objective_status == 1 || this->field_168_game_object->GetCharState_433A80() == Char_B4_state::Jumping_15))
                {
                    Ped::SetObjective(objectives_enum::no_obj_0, 9999);
                    Ped::SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                }
                ++gNumPedsOnScreen_6787EC;
                ++this->field_20e_offscreen_counter;
                byte_6787C4 = 1;
                if (this->field_168_game_object->GetCharState_433A80() == Char_B4_state::Jumping_15)
                {
                    byte_61A8A4 = this->field_278_ped_state_1 == ped_state_1::entering_car_3;
                }

                if (field_278_ped_state_1 == ped_state_1::dead_9 || field_278_ped_state_1 == ped_state_1::immobilized_8)
                {
                    byte_61A8A4 = 0;
                }
                else
                {
                    this->field_168_game_object->SetSpriteNum_4338F0(25);
                }
                if (field_168_game_object->IsOnScreen_545700() == 1)
                {
                    this->field_20e_offscreen_counter = 0;
                }
                if (Ped::get_fieldC_45C9B0() == kFpZero_678660 && Ped::get_field8_45C900() == gDummyPedAng_6787A8)
                {
                    if (field_278_ped_state_1 == ped_state_1::walking_0 && GetCharVelocity_433C20() == kFpZero_678660)
                    {
                        Ped::ChangeNextPedState1_45C500(ped_state_1::standing_still_7);
                        Ped::ChangeNextPedState2_45C540(ped_state_2::ped2_staying_14);
                    }
                }
                else
                {
                    if (this->field_278_ped_state_1 == ped_state_1::standing_still_7)
                    {
                        Ped::ChangeNextPedState1_45C500(ped_state_1::walking_0);
                        Ped::ChangeNextPedState2_45C540(ped_state_2::ped2_walking_0);
                    }
                    if (this->field_278_ped_state_1 == ped_state_1::entering_car_3 &&
                        this->field_27C_ped_state_2 == ped_state_2::ped2_following_a_car_4)
                    {
                        Ped::ChangeNextPedState1_45C500(ped_state_1::walking_0);
                        Ped::ChangeNextPedState2_45C540(ped_state_2::ped2_walking_0);
                        Ped::SetObjective(objectives_enum::no_obj_0, 9999);
                        Ped::SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                    }
                }
                if (this->field_168_game_object->GetCharState_433A80() == Char_B4_state::Jumping_15)
                {
                    this->field_15C_player->field_64_bJumping = 1;
                }
                return 1;
            }
            this->field_210_shock_counter = 0;
            ++this->field_20e_offscreen_counter;
            if (this->field_16C_car->field_50_car_sprite &&
                gGame_0x40_67E008->IsSpriteOnScreenForAnyPlayer_4B97E0(this->field_16C_car->field_50_car_sprite, kFpZero_678660))
            {
                this->field_20e_offscreen_counter = 0;
            }
            return 1;

        case ped_type::script_created_5:
            this->field_212_electrocution_threshold = 100;
            this->field_230 = 2;
            if (this->field_168_game_object)
            {
                ++this->field_20e_offscreen_counter;
                Ped::sub_462620();
                return 1;
            }
            ++this->field_20e_offscreen_counter;
            if (this->field_16C_car->field_50_car_sprite &&
                gGame_0x40_67E008->IsSpriteOnScreenForAnyPlayer_4B97E0(this->field_16C_car->field_50_car_sprite, kFpZero_678660))
            {
                this->field_20e_offscreen_counter = 0;
            }
            return 1;

        case ped_type::special_ped_4:
        case ped_type::dummy_with_occupation_6:
            this->field_212_electrocution_threshold = 100;
            if (this->field_240_occupation == ped_ocupation_enum::armed_gang_member_19 && this->field_278_ped_state_1 == ped_state_1::dead_9)
            {
                --gNumberArmedGangMembers_6787CE;
                this->field_240_occupation = ped_ocupation_enum::dummy;
            }
            if (this->field_168_game_object)
            {
                ++this->field_20e_offscreen_counter;
                if (this->field_20e_offscreen_counter > 100u)
                {
                    if (this->field_240_occupation == ped_ocupation_enum::elvis_leader)
                    {
                        if (!this->field_164_ped_group)
                        {
                            // The original jumps to the last Deallocate block of the dummy_3 case here.
                            goto deallocate_dead;
                        }
                        if (this->field_20e_offscreen_counter > 500u && this->field_164_ped_group->AreAllMembersOffScreen_4C9150())
                        {
                            Ped::Deallocate_45EB60();
                            return 0;
                        }
                    }
                    else if (this->field_240_occupation != ped_ocupation_enum::special_groups_member && !Ped::sub_45B590())
                    {
                        Ped::Deallocate_45EB60();
                        return 0;
                    }
                }
                Ped::sub_462620();
                return 1;
            }
            ++this->field_20e_offscreen_counter;
            if (this->field_16C_car && this->field_16C_car->field_50_car_sprite &&
                gGame_0x40_67E008->IsSpriteOnScreenForAnyPlayer_4B97E0(this->field_16C_car->field_50_car_sprite, kFpZero_678660))
            {
                this->field_20e_offscreen_counter = 0;
            }
            if (this->field_20e_offscreen_counter > 60u && this->field_16C_car && this->field_240_occupation == ped_ocupation_enum::car_thief)
            {
                this->field_16C_car->field_7C_uni_num = 3;
            }
            if (this->field_278_ped_state_1 == ped_state_1::dead_9)
            {
                Ped::Deallocate_45EB60();
                return 0;
            }
            return 1;

        case ped_type::dummy_3:
            this->field_212_electrocution_threshold = 100;
            if (this->field_168_game_object)
            {
                u16 f20E = ++this->field_20e_offscreen_counter;
                if (this->field_278_ped_state_1 == 1)
                {
                    if (f20E == 200)
                    {
                        Ped::Deallocate_45EB60();
                        return 0;
                    }
                }
                else if (f20E >= k_word_678656)
                {
                    Ped::Deallocate_45EB60();
                    return 0;
                }
                Ped::sub_462620();
                return 1;
            }
            ++this->field_20e_offscreen_counter;
            if (this->field_16C_car->field_50_car_sprite &&
                gGame_0x40_67E008->IsSpriteOnScreenForAnyPlayer_4B97E0(this->field_16C_car->field_50_car_sprite, kFpZero_678660))
            {
                this->field_20e_offscreen_counter = 0;
            }
            if (this->field_240_occupation != ped_ocupation_enum::unknown_2)
            {
                if (this->field_16C_car->IsDespawning_4215B0())
                {
                    Ped::Deallocate_45EB60();
                    return 0;
                }
                if (this->field_278_ped_state_1 == ped_state_1::dead_9)
                {
                deallocate_dead:
                    Ped::Deallocate_45EB60();
                    return 0;
                }
                return 1;
            }
            return 0;

        default:
            return 1;
    }
}

MATCH_FUNC(0x462b80)
void Ped::UpdateCharB4_462B80()
{
    field_168_game_object->Set_F8_ped_state_1_433910(field_278_ped_state_1);
    field_168_game_object->SetPedState2_433A50(field_27C_ped_state_2);
    field_168_game_object->Update_545720(gDistanceToTarget_678750);
    if (field_168_game_object)
    {
        field_1AC_cam.x = field_168_game_object->get_sprite_xpos();
        field_1AC_cam.y = field_168_game_object->get_sprite_ypos();
        field_1AC_cam.z = field_168_game_object->get_sprite_zpos();
        Char_B4* pB4 = field_168_game_object;
        field_12C = pB4->get_rotation_433A40();

        if (field_278_ped_state_1 == ped_state_1::in_car_10)
        {
            field_16C_car = pB4->Get_F84_403900();
            if (pB4->field_88_obj_2c.field_0_p18)
            {
                pB4->field_88_obj_2c.DestroyAllSprites_5A7010();
            }
            gChar_B4_Pool_6FDB44->DeAllocate(field_168_game_object);

            field_168_game_object = NULL;
            if (!field_248_enter_car_as_passenger)
            {
                field_16C_car->AssignDriver_4406E0(this);
            }
            else
            {
                field_16C_car->ShowCarName_4406B0(this);
                if (field_25C_internal_objective == 37 && field_238_ped_type == 3 ||
                    (field_16C_car->field_4_passengers_list.AddPed_471140(this), field_238_ped_type == 3))
                {
                    if (field_25C_internal_objective == 37)
                    {
                        Train_58* pTrain = gPublicTransport_181C_6FF1D4->GetTrainFromCarExcludingLeadCar_57B6A0(field_16C_car);
                        ++pTrain->field_56_passenger_count;
                    }
                }
            }
            if ((field_25C_internal_objective == 35 || field_25C_internal_objective == 37) && (field_226_internal_objective_status = 1, field_25C_internal_objective == 37))
            {
                if (field_238_ped_type == 3)
                {
                    Ped::Deallocate_45EB60();
                }
            }
            else
            {
                Car_Door_10* Door = field_16C_car->GetDoor(field_24C_target_car_door);
                if (field_240_occupation != ped_ocupation_enum::bus_customer_8 && field_240_occupation != ped_ocupation_enum::train_customer_9)
                {
                    Door->Close_439EA0();
                }
                Door->set_ped_421380(NULL);
            }
        }
    }
}

MATCH_FUNC(0x462e70)
bool Ped::PoolUpdate()
{
    if (Is_occupation_elvis_433C90())
    {
        if (word_6787F2 > 0)
        {
            --word_6787F2;
        }
        else
        {
            Set_F250_IfBit_433DD0(23);
            word_6787F2 = gRng_6F6784.get_int_4F7AE0(300) + 450;
        }
    }

    if (field_158_unk_car)
    {
        if (field_158_unk_car->IsDespawning_4215B0())
        {
            field_158_unk_car = 0;
        }
    }

    switch (field_238_ped_type)
    {
        case ped_type::dummy_3:
            ++gNumDummyChars_6787E2;
            break;
        case ped_type::special_ped_4:
        case ped_type::dummy_with_occupation_6:
            field_230 = 2;
            if (field_28C_threat_reaction == threat_reaction_enum::react_as_emergency_1)
            {
                field_230 = 1;
                ++gNumEmergencyPeds_6787E4;
            }
            else
            {
                ++gNumDummyChars_6787E2;
            }
            break;
        case ped_type::script_created_5:
            ++gNumScriptCreatedPeds_6787E3;
            break;
        default:
            break;
    }
    if (field_21C_bf.b10)
    {
        Ped::DeallocateWithGroupCleanup_45EA00();
    }
    Ped::UpdateKillerIdTimer_469030();
    Ped::ManageBurning_45BEC0();
    Ped::ManageShocking_45BC70();
    if (!field_234_timer)
    {
        Ped::ReleaseGroupSpritesAndWeapons_4624A0();
        return true;
    }
    if (field_26A_recent_crime_timer > 0)
    {
        field_26A_recent_crime_timer--;
    }
    ++gNumPedsUpdated_6787E0;
    gTargetSearchMode_6787D7 = 0;
    gSearchingPed_6787DC = 0;
    byte_6787D4 = 0;
    byte_678554 = 0;
    field_263_prev_attackers_count = field_262_attackers_count;
    field_262_attackers_count = 0;
    field_21C_bf.b23 = 0;
    if (field_21C_bf.b5 != 0 && field_278_ped_state_1 != ped_state_1::immobilized_8)
    {
        // Ped busted
        Ped::ChangeNextPedState1_45C500(ped_state_1::immobilized_8);
        Ped::ChangeNextPedState2_45C540(ped_state_2::lying_on_floor_22); // BUSTED!
        Set_B4_F16_To_1_433B50();
    }

    if ((u32)field_210_shock_counter > field_212_electrocution_threshold)
    {
        field_210_shock_counter = field_212_electrocution_threshold;
    }
    if (field_210_shock_counter == field_212_electrocution_threshold && field_278_ped_state_1 != ped_state_1::immobilized_8 &&
        field_27C_ped_state_2 != ped_state_2::electrocuted_27)
    {
        // Ped electrocuted
        Ped::ChangeNextPedState1_45C500(ped_state_1::immobilized_8); // immobilize it
        Ped::ChangeNextPedState2_45C540(ped_state_2::electrocuted_27); // electrocute ped
        Set_B4_F16_To_1_433B50();
    }
    if (byte_6787D8 == 1)
    {
        field_21C_bf.b16 = 0;
    }
    if (byte_6787D9 == 1)
    {
        field_21C_bf.b19 = 0;
    }
    if (field_21C_bf.b0 == 1)
    {
        byte_61A8A4 = 1;
        if (Ped::StateMachineTick_4626B0())
        {
            if (byte_61A8A4)
            {
                Ped::UpdateAI_462280();
                gLastProcessedPed_6787C0 = this;
                Ped::ManageWeapon_46F390();
                if (field_20A_wanted_points)
                {
                    if (bSkip_police_67D4F9 || field_240_occupation == ped_ocupation_enum::empty)
                    {
                        field_20A_wanted_points = 0;
                    }
                    else if (field_238_ped_type == ped_type::player_2 && field_200_id)
                    {
                        if (field_27C_ped_state_2 == ped_state_2::Unknown_29)
                        {
                            field_20A_wanted_points = 0;
                        }
                        else
                        {
                            gPolice_7B8_6FEE40->RegisterCriminal_56F940(this);
                        }
                    }
                }
            }
            else
            {
                if (field_278_ped_state_1 == ped_state_1::dead_9)
                {
                    if ((!field_164_ped_group || field_23C_group_idx != 99) && field_258_objective != objectives_enum::objective_28)
                    {
                        Ped::SetObjective(objectives_enum::no_obj_0, 9999);
                        Ped::SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                        field_278_ped_state_1 = ped_state_1::dead_9;
                        field_27C_ped_state_2 = ped_state_2::Unknown_15;
                    }
                    if (field_20e_offscreen_counter > 0xC8u && field_240_occupation != ped_ocupation_enum::paramedic_23 && field_238_ped_type != ped_type::script_created_5)
                    {
                        Ped::Deallocate_45EB60();
                    }
                }
                else
                {
                    Ped::Occupation_AI_461F20();
                    if (field_258_objective != objectives_enum::no_obj_0 || field_25C_internal_objective)
                    {
                        byte_61A8A3 = 0;
                        Ped::ProcessObjective_4632E0();
                    }
                    if (field_168_game_object->GetCharState_433A80() == Char_B4_state::Jumping_15)
                    {
                        if (field_278_ped_state_1 > 0 && field_278_ped_state_1 <= 7) // not walking neither dead/immobilized
                        {
                            Ped::UpdateFacingAngle_461A60();
                        }
                    }
                }
            }

            if (!byte_678554 && field_21C_bf.b14)
            {
                gOrca_2FD4_6FDEF0->field_3C_ped_list.RemovePed_471240(this);
                field_21C_bf.b14 = 0;
            }

            if (field_168_game_object)
            {
                Fix16 zpos = get_cam_z();
                if (field_168_game_object->field_58_flags_bf.b0 == 0 && zpos != kFpZero_678660)
                {
                    zpos -= kFpOne_678664;
                }
                field_254_block_spec = gMap_0x370_6F6268->GetBlockSpec_4E00A0(get_cam_x(), get_cam_y(), zpos);
                Ped::UpdateCharB4_462B80();

                field_21C_bf.b8 = 0;
                field_21C_bf.b9 = 0;
            }
            else
            {
                field_1AC_cam.x = field_16C_car->field_50_car_sprite->field_14_xy.x;
                field_1AC_cam.y = field_16C_car->field_50_car_sprite->field_14_xy.y;
                field_1AC_cam.z = field_16C_car->field_50_car_sprite->field_1C_zpos;
            }
        }
        else
        {
            return false;
        }
    }
    else if (field_234_timer != 99)
    {
        --field_234_timer;

        if (field_234_timer == 0)
        {
            field_234_timer = 0; // ????????????????
        }
    }

    if (field_208_invulnerability)
    {
        if (field_208_invulnerability != 9999)
        {
            field_208_invulnerability--;
        }
    }
    return false;
}

MATCH_FUNC(0x4632e0)
void Ped::ProcessObjective_4632E0()
{
    ProcessOnFootObjective_463AA0();
    ProcessInCarObjective_463FB0();
}

MATCH_FUNC(0x463300)
void Ped::ChangePedStatesByMode_463300(u8 a1)
{
    switch (a1)
    {
        case 1u:
            Ped::ChangeNextPedState1_45C500(ped_state_1::walking_0);
            Ped::ChangeNextPedState2_45C540(ped_state_2::ped2_walking_0);
            break;
        case 2u:
            Ped::ChangeNextPedState1_45C500(ped_state_1::flee_or_running_1);
            Ped::ChangeNextPedState2_45C540(ped_state_2::Unknown_3);
            break;
        case 3u:
            Ped::ChangeNextPedState1_45C500(ped_state_1::flee_or_running_1);
            Ped::ChangeNextPedState2_45C540(ped_state_2::Unknown_2);
            break;
        case 4u:
            Ped::ChangeNextPedState1_45C500(ped_state_1::standing_still_7);
            Ped::ChangeNextPedState2_45C540(ped_state_2::ped2_staying_14);
            break;
        case 5u:
            Ped::ChangeNextPedState1_45C500(ped_state_1::in_car_10);
            Ped::ChangeNextPedState2_45C540(ped_state_2::ped2_driving_10);
            break;
        case 6u:
            Ped::ChangeNextPedState1_45C500(ped_state_1::entering_car_3);
            Ped::ChangeNextPedState2_45C540(ped_state_2::ped2_following_a_car_4);
            break;
        case 7u:
            Ped::ChangeNextPedState1_45C500(ped_state_1::exiting_car_4);
            Ped::ChangeNextPedState2_45C540(ped_state_2::ped2_driving_10);
            break;
        default:
            return;
    }
}

MATCH_FUNC(0x4633e0)
void Ped::SetStatesForObjective_4633E0(char_type bMainObj)
{
    u8 state = 99;
    s32 obj;
    if (bMainObj)
    {
        obj = field_258_objective;
    }
    else
    {
        obj = field_25C_internal_objective;
    }
    switch (obj)
    {
        case objectives_enum::no_obj_0:
            state = field_16C_car ? 5 : 1;
            break;
        case objectives_enum::flee_on_foot_till_safe_1:
        case objectives_enum::flee_char_on_foot_till_safe_2:
        case objectives_enum::flee_char_on_foot_always_3:
            state = 2;
            break;
        case objectives_enum::guard_spot_24:
        case objectives_enum::guard_area_25:
        case objectives_enum::wait_on_foot_26:
        case objectives_enum::objective_29:
        case objectives_enum::objective_30:
        case objectives_enum::objective_44:
        case objectives_enum::objective_45:
        case objectives_enum::objective_46:
        case objectives_enum::objective_47:
            state = 4;
            break;
        case objectives_enum::goto_area_in_car_14:
        case objectives_enum::wait_in_car_27:
        case objectives_enum::time_waited_in_car_31:
        case objectives_enum::objective_43:
        case objectives_enum::objective_52:
        case objectives_enum::objective_54:
        case objectives_enum::follow_car_in_car_55:
        case objectives_enum::fire_at_object_from_vehicle_57:
            state = 5;
            break;
        case objectives_enum::enter_car_as_driver_35:
        case objectives_enum::enter_train_37:
            state = 6;
            break;
        case objectives_enum::leave_car_36:
        case objectives_enum::leave_train_38:
            state = field_168_game_object ? 1 : 7;
            break;
        case objectives_enum::objective_7:
        case objectives_enum::objective_9:
        case objectives_enum::objective_11:
        case objectives_enum::goto_area_on_foot_12:
        case objectives_enum::goto_char_on_foot_16:
        case objectives_enum::objective_17:
        case objectives_enum::objective_18:
        case objectives_enum::kill_char_on_foot_20:
        case objectives_enum::punch_char_23:
        case objectives_enum::objective_32:
        case objectives_enum::objective_48:
        case objectives_enum::follow_car_on_foot_with_offset_56:
        case objectives_enum::destroy_object_58:
        case objectives_enum::destroy_car_59:
            state = 3;
            break;
        case objectives_enum::objective_8:
        case objectives_enum::kill_frenzy_22:
        case objectives_enum::objective_49:
        case objectives_enum::objective_51:
            state = 1;
            break;
        case objectives_enum::objective_50:
            Ped::ChangeNextPedState1_45C500(ped_state_1::dead_9);
            Ped::ChangeNextPedState2_45C540(ped_state_2::Unknown_15);
            break;
        default:
            break;
    }
    Ped::ChangePedStatesByMode_463300(state);
}

MATCH_FUNC(0x463570)
void Ped::SetObjective(s32 objective, s16 objective_timer)
{
    Marz_96* pMarz_96; // eax
    Marz_3* pPoint;

    if (this->field_278_ped_state_1 != 9 || objective == objectives_enum::objective_28)
    {
        this->field_258_objective = objective;
        this->field_218_objective_timer = objective_timer;
        this->field_1B8_target_x = Fix16(-16384, 0);
        this->field_1BC_target_y = Fix16(-16384, 0);
        this->field_1C0_target_z = Fix16(-16384, 0);
        this->field_1DC_objective_target_x = kFpZero_678660;
        this->field_1E0_objective_target_y = kFpZero_678660;
        this->field_148_objective_target_ped = 0;
        this->field_150_target_objective_car = 0;
        this->field_1A0_objective_target_object = NULL;
        this->field_225_objective_status = 0;

        //new_flags = this->field_21C & ~0x400004u;
        //this->field_21C = new_flags;

        u8 mode = 99;

        // TODO: Not sure if this is correct
        field_21C_bf.b2 = false;
        field_21C_bf.b22 = false;

        switch (objective)
        {
            case 0:
                mode = field_16C_car != 0 ? 5 : 1;
                break;

            case 1:
            case 2:
            case 3:
                mode = 2;
                break;

            case 20:
            case 23:
            case 58:
            case 59:
                mode = 3;
                SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                break;

            case 24:
            case 25:
            case 26:
                this->field_1DC_objective_target_x = this->field_1AC_cam.x;
                this->field_1E0_objective_target_y = this->field_1AC_cam.y;
                this->field_1E4_objective_target_z = this->field_1AC_cam.z;
                mode = 4;
                break;

            case 14:
            case 27:
            case 31:
            case 43:
            case 52:
            case 54:
            case 55:
            case 57:
            case 60:
            case 61:
                mode = 5;
                break;

            case 35:
            case 37:
                mode = 6;
                break;

            case 36:
            case 38:
                mode = field_168_game_object != 0 ? 1 : 7;
                break;

            case 42:
                pMarz_96 = gMarz_1D7E_6FD784->AllocPatrolList_543F10(&field_265);
                field_190_patrol_route = pMarz_96;
                pPoint = pMarz_96->field_0_points;
                while (pPoint->field_0_x)
                {
                    pPoint->field_0_x = 0;
                    pPoint++;
                }
                break;

            case 12:
            case 16:
            case 32:
            case 56:
                SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                // fall through
            case 22:
                mode = 3;
                break;

            case 28:
                if (!gAmbulance_110_6F70A8->TryAddPatient_4FA470(this))
                {
                    this->field_258_objective = objectives_enum::no_obj_0;
                    return;
                }
                break;

            case 8:
            case 51:
                mode = 1;
                break;

            case 50:
                ChangeNextPedState1_45C500(ped_state_1::dead_9);
                ChangeNextPedState2_45C540(ped_state_2::Unknown_15);
                break;

            default:
                break;
        }
        ChangePedStatesByMode_463300(mode);
    }
}

MATCH_FUNC(0x463830)
void Ped::SetObjective2_463830(s32 car_state, s16 a3)
{

    u8 x_int;
    u8 y_int;
    u8 z_int;
    u8 mode = 99;
    if (this->field_278_ped_state_1 != ped_state_1::dead_9)
    {
        if (this->field_27C_ped_state_2 == ped_state_2::ped2_entering_a_car_6 ||
            this->field_27C_ped_state_2 == ped_state_2::ped2_getting_out_a_car_7)
        {
            Car_Door_10* pDoor;
            if (this->field_154_target_to_enter)
            {
                pDoor = this->field_154_target_to_enter->GetDoor(this->field_24C_target_car_door);
            }
            else if (this->field_168_game_object)
            {
                pDoor = this->field_168_game_object->field_84_target_car->GetDoor(this->field_24C_target_car_door);
            }
            else
            {
                pDoor = this->field_16C_car->GetDoor(this->field_24C_target_car_door);
            }
            pDoor->Close_439EA0();
            pDoor->set_ped_421380(0);
        }

        this->field_25C_internal_objective = car_state;
        this->field_21A_car_state_timer = a3;
        this->field_1C4_x = Fix16(-16384, 0);
        this->field_1C8_y = Fix16(-16384, 0);
        this->field_1CC_z = Fix16(-16384, 0);
        this->field_1D0_internal_target_x = kFpZero_678660;
        this->field_1D4_internal_target_y = kFpZero_678660;
        this->field_1D8_internal_target_z = kFpZero_678660;
        this->field_14C_internal_target_ped = 0;
        this->field_154_target_to_enter = 0;
        this->field_1A4_internal_target_object = 0;
        this->field_226_internal_objective_status = 0;
        switch (car_state)
        {
            case objectives_enum::flee_on_foot_till_safe_1:
            case objectives_enum::flee_char_on_foot_till_safe_2:
            case objectives_enum::flee_char_on_foot_always_3:
                mode = 2;
                break;
            case objectives_enum::objective_7:
            case objectives_enum::objective_9:
            case objectives_enum::objective_11:
            case objectives_enum::goto_area_on_foot_12:
            case objectives_enum::objective_18:
            case objectives_enum::kill_char_on_foot_20:
            case objectives_enum::punch_char_23:
            case objectives_enum::objective_48:
                mode = 3;
                break;
            case objectives_enum::enter_car_as_driver_35:
                mode = 6;
                break;
            case objectives_enum::enter_train_37:
                Ped::ChangeNextPedState1_45C500(ped_state_1::unknown_5);
                Ped::ChangeNextPedState2_45C540(ped_state_2::Unknown_5);
                break;
            case objectives_enum::leave_train_38:
                Ped::ChangeNextPedState1_45C500(ped_state_1::unknown_6);
                Ped::ChangeNextPedState2_45C540(ped_state_2::ped2_driving_10); // ??
                break;
            case objectives_enum::leave_car_36:
                mode = 7;
                break;
            case objectives_enum::objective_17:
            {
                mode = 3;
                x_int = this->field_1AC_cam.x.ToUInt8();
                y_int = this->field_1AC_cam.y.ToUInt8();
                z_int = this->field_1AC_cam.z.ToUInt8();
                gMap_0x370_6F6268->FindNearbyBlockOfType_4E4930(&x_int, &y_int, &z_int, 2);
                this->field_1D0_internal_target_x = kFpHalf_67853C + Fix16(x_int);
                this->field_1D4_internal_target_y = kFpHalf_67853C + Fix16(y_int);
                this->field_1D8_internal_target_z = kFpOne_678664 + Fix16(z_int);
                break;
            }
            case objectives_enum::goto_area_in_car_14:
                mode = 5;
                break;
            case objectives_enum::wait_on_foot_26:
            case objectives_enum::objective_29:
            case objectives_enum::objective_30:
            case objectives_enum::objective_44:
            case objectives_enum::objective_45:
            case objectives_enum::objective_46:
            case objectives_enum::objective_47:
                mode = 4;
                break;
            case objectives_enum::no_obj_0:
            case objectives_enum::kill_frenzy_22:
            case objectives_enum::objective_49:
                mode = 1;
                break;
            default:
                break;
        }
        Ped::ChangePedStatesByMode_463300(mode);
    }
}

MATCH_FUNC(0x463aa0)
void Ped::ProcessOnFootObjective_463AA0()
{
    Ang16 angle = 0;
    if (field_258_objective && field_225_objective_status == objective_status::not_finished_0)
    {
        if (field_148_objective_target_ped)
        {
            field_1B8_target_x = field_148_objective_target_ped->get_cam_x();
            field_1BC_target_y = field_148_objective_target_ped->get_cam_y();
            field_1C0_target_z = field_148_objective_target_ped->get_cam_z();
        }
        else if (field_150_target_objective_car)
        {
            u8 Remap = field_150_target_objective_car->GetRemap();
            if (field_24C_target_car_door >= Remap)
            {
                field_24C_target_car_door = Remap - 1;
            }
            if (field_150_target_objective_car->IsDespawning_4215B0())
            {
                field_1B8_target_x = field_1AC_cam.x;
                field_1BC_target_y = field_1AC_cam.y;
                field_1C0_target_z = field_1AC_cam.z;
            }
            else if (field_258_objective != objectives_enum::objective_18 &&
                     (field_258_objective <= objectives_enum::objective_34 || field_258_objective > objectives_enum::leave_train_38))
            {
                field_1B8_target_x = field_150_target_objective_car->field_50_car_sprite->field_14_xy.x;
                field_1BC_target_y = field_150_target_objective_car->field_50_car_sprite->field_14_xy.y;
                field_1C0_target_z = field_150_target_objective_car->field_50_car_sprite->field_1C_zpos;
            }
            else
            {
                CarDoorAlignmentSolver_545AF0(0,
                                              field_150_target_objective_car,
                                              field_24C_target_car_door,
                                              field_1B8_target_x,
                                              field_1BC_target_y,
                                              angle);
                field_1C0_target_z = field_150_target_objective_car->field_50_car_sprite->field_1C_zpos;
            }
        }
        else
        {
            if (field_1A0_objective_target_object)
            {
                field_1B8_target_x = field_1A0_objective_target_object->field_4->GetXPos();
                field_1BC_target_y = field_1A0_objective_target_object->field_4->GetYPos();
                field_1C0_target_z = field_1A0_objective_target_object->field_4->GetZPos();
            }
            else if (field_1DC_objective_target_x != kFpZero_678660 && field_1E0_objective_target_y != kFpZero_678660)
            {
                field_1BC_target_y = field_1E0_objective_target_y;
                field_1B8_target_x = field_1DC_objective_target_x;
                field_1C0_target_z = field_1E4_objective_target_z;
            }
        }

        Fix16 diff_x = field_1B8_target_x - field_1AC_cam.x;
        Fix16 diff_y = field_1BC_target_y - field_1AC_cam.y;
        diff_x = Fix16::Abs(diff_x);
        diff_y = Fix16::Abs(diff_y);
        gDistanceToTarget_678750 = Fix16::Max(diff_x, diff_y);

        switch (field_258_objective)
        {
            case objectives_enum::flee_on_foot_till_safe_1:
                Ped::FleeOnFootTillSafe_4678E0();
                break;
            case objectives_enum::flee_char_on_foot_till_safe_2:
                Ped::FleeCharOnFootTillSafe_467960();
                break;
            case objectives_enum::flee_char_on_foot_always_3:
                Ped::FleeFromCharOnFootAlways_467A20();
                break;
            case objectives_enum::flee_char_any_means_till_safe_4:
                nullsub_9();
                break;
            case objectives_enum::flee_char_any_means_always_5:
                nullsub_10();
                break;
            case objectives_enum::flee_char_always_once_car_stopped_6:
                Ped::FleeCharAlwaysOnceCarStopped_467AD0();
                break;

            case objectives_enum::objective_34:
                Ped::sub_467BD0();
                break;
            case objectives_enum::kill_char_on_foot_20:
                Ped::KillCharOnFoot_467CA0();
                break;
            case objectives_enum::kill_char_any_means_19:
                Ped::KillCharAnyMeans_467E20();
                break;
            case objectives_enum::kill_car_21:
                nullsub_11();
                break;
            case objectives_enum::kill_frenzy_22:
                Ped::KillFrenzy_467FB0();
                break;
            case objectives_enum::punch_char_23:
                Ped::PunchChar_467FD0();
                break;
            case objectives_enum::wait_on_foot_26:
                Ped::ProcessAirborneMovement_468040();
                break;
            case objectives_enum::guard_spot_24:
                Ped::GuardSpot_469BF0();
                break;
            case objectives_enum::guard_area_25:
                Ped::GuardArea_469D60();
                break;
            case objectives_enum::time_waited_in_car_31:
                Ped::TimeWaitedInCar_4682A0();
                break;
            case objectives_enum::goto_area_in_car_14:
                Ped::GotoAreaInCar_468310();
                break;
            case objectives_enum::enter_car_as_driver_35:
                Ped::EnterTargetObjectiveCar_4686C0();
                break;
            case objectives_enum::leave_car_36:
                Ped::LeaveTargetObjectiveCar_468820();
                break;
            case objectives_enum::patrol_on_foot_42:
                Ped::PatrolOnFoot_468C70();
                break;
            case objectives_enum::goto_char_on_foot_16:
                Ped::UpdateFollowPedObjective_468E80();
                break;
            case objectives_enum::goto_area_on_foot_12:
                Ped::GotoAreaOnFoot_468DE0();
                break;
            case objectives_enum::goto_area_any_means_13:
                Ped::GotoAreaByAnyMeans_469060();
                break;
            case objectives_enum::objective_51:
                Ped::sub_469E10();
                break;
            case objectives_enum::objective_8:
                Ped::sub_469BD0();
                break;
            case objectives_enum::enter_train_37:
                Ped::EnterTrain_468930();
                break;
            case objectives_enum::leave_train_38:
                Ped::LeaveTrain_468A00();
                break;
            case objectives_enum::objective_33:
                Ped::sub_468BD0();
                break;
            case objectives_enum::objective_50:
                nullsub_12();
                break;
            case objectives_enum::objective_43:
                Ped::sub_469E30();
                break;
            case objectives_enum::objective_52:
                Ped::sub_469E50();
                break;
            case objectives_enum::wait_in_car_27:
                Ped::WaitInCurrentCar_469FC0();
                break;
            case objectives_enum::objective_10:
                Ped::FollowPedInCar_469F30();
                break;
            case objectives_enum::objective_54:
                Ped::sub_469FE0();
                break;
            case objectives_enum::objective_32:
                Ped::PullDriverOutOfCar_46A1F0();
                break;
            case objectives_enum::follow_car_in_car_55:
                Ped::FollowCarInCurrCar_46A290();
                break;
            case objectives_enum::follow_car_on_foot_with_offset_56:
                Ped::FollowCarOnFootWithOffset_46A350();
                break;
            case objectives_enum::fire_at_object_from_vehicle_57:
                Ped::FireAtObject_46A530();
                break;
            case objectives_enum::destroy_car_59:
                Ped::DestroyTargetCar_46A850();
                break;
            case objectives_enum::destroy_object_58:
                Ped::DestroyTargetObject_46A7C0();
                break;
            case objectives_enum::turret_put_out_car_fire_60:
                Ped::AimVehicleTurretStateMachine_46A6D0();
                break;
            case objectives_enum::objective_61:
                Ped::FireAtPlayer_46A5E0();
                break;
            default:
                return;
        }
    }
}

// https://decomp.me/scratch/0fIeI
MATCH_FUNC(0x463fb0)
void Ped::ProcessInCarObjective_463FB0()
{
    Ang16 UnkAng(0);

    if (field_226_internal_objective_status == 2)
    {
        this->field_21C &= ~4u;
    }

    if (field_25C_internal_objective && !field_226_internal_objective_status)
    {
        Ped* v4 = this->field_14C_internal_target_ped;
        if (v4)
        {
            this->field_1C4_x = v4->get_cam_x();
            this->field_1C8_y = v4->get_cam_y();
            this->field_1CC_z = v4->get_cam_z();
            switch (field_25C_internal_objective)
            {
                case 11:
                case 18:
                case 20:
                case 23:
                    byte_6787C4 = 0;
                    break;
                default:
                    if (byte_6787C4)
                    {
                        // Through a bool local with an early break: written directly, VC6 lays the
                        // clear after the call and doesn't share it with the case 11/18/20/23 block
                        bool bStop = v4->field_168_game_object == NULL || this->field_278_ped_state_1 == ped_state_1::entering_car_3;
                        if (bStop)
                        {
                            byte_6787C4 = 0;
                            break;
                        }
                        Ped::sub_4645B0();
                    }
                    break;
            }
        }
        if (field_154_target_to_enter)
        {
            if (field_154_target_to_enter->field_88_despawn_status == 6)
            {
                this->field_226_internal_objective_status = 2;
                return;
            }
            u8 Remap = field_154_target_to_enter->GetRemap();
            if (this->field_24C_target_car_door >= Remap)
            {
                this->field_24C_target_car_door = Remap - 1;
            }

            if (field_25C_internal_objective == 18 || field_25C_internal_objective > 34 && field_25C_internal_objective <= 38)
            {
                CarDoorAlignmentSolver_545AF0(0,
                                              this->field_154_target_to_enter,
                                              field_24C_target_car_door,
                                              this->field_1C4_x,
                                              this->field_1C8_y,
                                              UnkAng);
                this->field_1CC_z = this->field_154_target_to_enter->field_50_car_sprite->field_1C_zpos;
            }
            else
            {
                Car_BC* pCar = this->field_154_target_to_enter;
                field_1C4_x = pCar->field_50_car_sprite->field_14_xy.x;
                field_1C8_y = pCar->field_50_car_sprite->field_14_xy.y;
                field_1CC_z = pCar->field_50_car_sprite->field_1C_zpos;
            }
        }

        if (field_1A4_internal_target_object)
        {
            this->field_1C4_x = field_1A4_internal_target_object->get_x_4340D0();
            this->field_1C8_y = field_1A4_internal_target_object->get_y_4340E0();
            this->field_1CC_z = field_1A4_internal_target_object->get_z_4340F0();
        }

        if (field_1D0_internal_target_x != kFpZero_678660)
        {
            if (field_1D4_internal_target_y != kFpZero_678660)
            {
                this->field_1C4_x = field_1D0_internal_target_x;
                this->field_1C8_y = field_1D4_internal_target_y;
                this->field_1CC_z = field_1D8_internal_target_z;
            }
        }

        Fix16 xd = this->field_1C4_x - this->field_1AC_cam.x;
        Fix16 yd = this->field_1C8_y - this->field_1AC_cam.y;

        xd = Fix16::Abs(xd);
        yd = Fix16::Abs(yd);

        gDistanceToTarget_678750 = Fix16::Max(xd, yd);

        switch (this->field_25C_internal_objective)
        {
            case 1:
                Ped::FleeOnFootTillSafe_46A8F0();
                break;
            case 2:
                Ped::FleeFromPedTillSafe_46A9C0();
                break;
            case 3:
                Ped::FleeFromPedAlways_46AAE0();
                break;
            case 7:
                Ped::sub_46AB50();
                break;
            case 9:
                Ped::FollowTargetStateMachine_46AC20();
                break;
            case 11:
                Ped::ChaseTargetStateMachine_46B170();
                break;
            case 32:
                Ped::PullDriverOutOfCarStateMachine_46B2F0();
                break;
            case 23:
                Ped::MeleeAttackStateMachine_46B670();
                break;
            case 20:
                Ped::AttackPed_46DB60();
                break;
            case 26:
                Ped::WaitOnFoot_46BD30();
                break;
            case 35:
                Ped::EnterCarStateMachine_46BDC0();
                break;
            case 36:
                Ped::ExitCarStateMachine_46C250();
                break;
            case 12:
                Ped::GotoAreaOnFoot_46C7E0();
                break;
            case 15:
                Ped::FollowPathPoints_46C910();
                break;
            case 17:
                Ped::sub_46C770();
                break;
            case 14:
                Ped::sub_46CA60();
                break;
            case 18:
                Ped::sub_46C8A0();
                break;
            case 48:
                Ped::CrossRoad_46C9B0();
                break;
            case 44:
                Ped::StartPedCrossingAtTrafficLight_Y_Backward_46CB30();
                break;
            case 45:
                Ped::StartPedCrossingAtTrafficLight_X_Forwards_46CC70();
                break;
            case 46:
                Ped::StartPedCrossingAtTrafficLight_Y_Forwards_46CDB0();
                break;
            case 47:
                Ped::StartPedCrossingAtTrafficLight_X_Backwards_46CEF0();
                break;
            case 49:
                Ped::sub_46D0B0();
                break;
            case 29:
                Ped::WaitForTrain_46D030();
                break;
            case 30:
                nullsub_14();
                break;
            case 37:
                Ped::EnterTrainStateMachine_46D0D0();
                break;
            case 38:
                Ped::ExitTrainStateMachine_46D240();
                break;

            case 52:
                Ped::FollowPedInCar_46CA70();
                break;
            case 56:
                Ped::FollowCarOnFoot_46D300();
                break;
            case 59:
                Ped::AttackCar_46DB70();
                break;
            case 58:
                Ped::AttackObject_46DB80();
                break;

            default:
                return;
        }
    }
}

// https://decomp.me/scratch/cMSHI
// Polar to cartesian with the out-of-line Fix16 multiply
static inline void PolarToCartesianMul_4645B0(Ang16& angle, Fix16& radius, Fix16& x, Fix16& y)
{
    x = Ang16::sine_40F500(angle).Multiply_408680(radius);
    y = Ang16::cosine_40F520(angle).Multiply_408680(radius);
}

// Same, with the sine multiply inlined
static inline void PolarToCartesianMulInlSin_4645B0(Ang16& angle, Fix16& radius, Fix16& x, Fix16& y)
{
    x = Ang16::sine_40F500(angle) * radius;
    y = Ang16::cosine_40F520(angle).Multiply_408680(radius);
}

// Ang16::operator+ with the normalizing ctor called out of line (AssignNormalized_409300)
static inline Ang16 AddAng16_ool_4645B0(const Ang16& a, const Ang16& b)
{
    s16 value = a.rValue + b.rValue;
    return Ang16(&value, 0);
}

WIP_FUNC(0x4645b0)
void Ped::sub_4645B0()
{
    WIP_IMPLEMENTED;
    Ang16 angle;
    Fix16 radius;
    Fix16 vec_x;
    Fix16 vec_y;

    u8 bUnk = false;

    if (field_14C_internal_target_ped->GetPedVelocity_45C920() > kFpZero_678660)
    {
        angle = kAng180_6785A6 + field_14C_internal_target_ped->field_168_game_object->field_40_rotation;
        radius = kFpThreeEighths_67878C;
    }
    else
    {
        angle = gDummyPedAng_6787A8;
        radius = kFpHalf_678790;
    }

    if (field_164_ped_group)
    {
        if (field_204_killer_id == field_164_ped_group->field_2C_ped_leader->get_id() &&
            field_164_ped_group->field_2C_ped_leader->field_21C_bf.b11)
        {
            bUnk = true;
            field_264_killer_id_timer = 50;
        }
    }

    if (field_14C_internal_target_ped->GetPedVelocity_45C920() == kFpZero_678660)
    {
        switch (field_23C_group_idx)
        {
            case 0:
                angle += kAng90_678502;
                if (bUnk)
                {
                    angle += kAng180_6785A6;
                    radius = kFpThreeQuarters_678794;
                }
                PolarToCartesianMul_4645B0(angle, radius, vec_x, vec_y);
                field_1C4_x += vec_x;
                field_1C8_y += vec_y;
                break;
            case 1:
                angle += kAng270_6785D0;
                if (bUnk)
                {
                    angle += kAng180_6785A6;
                    radius = kFpThreeQuarters_678794;
                }
                PolarToCartesianMul_4645B0(angle, radius, vec_x, vec_y);
                field_1C4_x += vec_x;
                field_1C8_y += vec_y;
                break;
            case 2:
                angle = AddAng16_ool_4645B0(kAng180_6785A6, angle);
                if (bUnk)
                {
                    angle += kAng180_6785A6;
                    radius = kFpThreeQuarters_678794;
                }
                PolarToCartesianMul_4645B0(angle, radius, vec_x, vec_y);
                field_1C4_x += vec_x;
                field_1C8_y += vec_y;
                break;

            case 3:
                PolarToCartesianMul_4645B0(angle, radius, vec_x, vec_y);
                if (bUnk)
                {
                    angle += kAng180_6785A6;
                    radius = kFpThreeQuarters_678794;
                }
                field_1C4_x += vec_x;
                field_1C8_y += vec_y;
                break;

            case 4:
                angle += kAng225_6786B8;
                if (bUnk)
                {
                    angle += kAng180_6785A6;
                    radius = kFpThreeQuarters_678794;
                }
                else
                {
                    radius = kFpThreeEighths_67878C;
                }
                PolarToCartesianMul_4645B0(angle, radius, vec_x, vec_y);
                field_1C4_x += vec_x;
                field_1C8_y += vec_y;
                break;

            case 5:
                angle += kAng45_6784E2;
                if (bUnk)
                {
                    angle += kAng180_6785A6;
                    radius = kFpThreeQuarters_678794;
                }
                else
                {
                    radius = kFpThreeEighths_67878C;
                }
                PolarToCartesianMul_4645B0(angle, radius, vec_x, vec_y);
                field_1C4_x += vec_x;
                field_1C8_y += vec_y;
                break;

            case 6:
                angle += kAng315_6785A8;
                if (bUnk)
                {
                    angle += kAng180_6785A6;
                    radius = kFpThreeQuarters_678794;
                }
                else
                {
                    radius = kFpThreeEighths_67878C;
                }
                PolarToCartesianMul_4645B0(angle, radius, vec_x, vec_y);
                field_1C4_x += vec_x;
                field_1C8_y += vec_y;
                break;

            case 7:
                angle += kAng135_67844C;
                if (bUnk)
                {
                    angle += kAng180_6785A6;
                    radius = kFpThreeQuarters_678794;
                }
                else
                {
                    radius = kFpThreeEighths_67878C;
                }
                PolarToCartesianMulInlSin_4645B0(angle, radius, vec_x, vec_y);
                field_1C4_x += vec_x;
                field_1C8_y += vec_y;
                break;

            default:
                angle += kAng225_6786B8;
                if (bUnk)
                {
                    angle += kAng180_6785A6;
                    radius = kFpThreeQuarters_678794;
                }
                else
                {
                    radius = kFpThreeEighths_67878C;
                }
                PolarToCartesianMulInlSin_4645B0(angle, radius, vec_x, vec_y);
                field_1C4_x += vec_x;
                field_1C8_y += vec_y;
                break;
        }

        field_130 = Fix16::atan2_fixed_405320(field_1AC_cam.y - field_14C_internal_target_ped->get_cam_y(), field_1AC_cam.x - field_14C_internal_target_ped->get_cam_x());
    }
    else
    {
        radius = kFpFiveSixteenths_678784;
        switch (field_23C_group_idx)
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
                radius = kFpHalf_678790;
                break;
            case 7:
                angle += kAng45_6784FC;
                radius = kFpHalf_678790;
                break;
            default:
                radius = kFpFiveSixteenths_678784;
                break;
        }
        PolarToCartesianMul_4645B0(angle, radius, vec_x, vec_y);
        field_1C4_x += vec_x;
        field_1C8_y += vec_y;
    }
}

// https://decomp.me/scratch/LvHfw
MATCH_FUNC(0x465270)
void Ped::Threat_Reaction_AI_465270()
{
    Ped* pTarget = 0;
    bool bGiveUp = false;
    bool bLineOfSightChecked = false;
    Fix16 abs_x;
    Fix16 abs_y;

    switch (field_28C_threat_reaction)
    {
        case threat_reaction_enum::no_reaction_0:
        {
            s32 old_threat_search = field_288_threat_search;
            if ((field_288_threat_search == threat_search_enum::line_of_sight_1 ||
                 field_288_threat_search == threat_search_enum::line_of_sight_player_only_6 ||
                 field_288_threat_search == threat_search_enum::line_of_sight_player_threat_only_4) &&
                !field_21C_bf.b19 && byte_61A8A2 == 1)
            {
                field_288_threat_search = threat_search_enum::line_of_sight_player_only_6;
                Ped* pPlayerPed = Ped::FindBestTargetPed_Mode5_466BD0(3);
                if (pPlayerPed && pPlayerPed->IsField238_45EDE0(2))
                {
                    field_21C_bf.b23 = true;
                }
                field_288_threat_search = old_threat_search;
            }
            break;
        }

        case threat_reaction_enum::run_away_3:
            if (field_168_game_object)
            {
                if (bHaveThreateningPeds_6787DA && field_25C_internal_objective != objectives_enum::flee_char_on_foot_till_safe_2 &&
                    !field_21C_bf.b27)
                {
                    Ped* pDangerousPed = gThreateningPedsList_678468.GetFromListClosestPedToPoint_471340(field_1AC_cam.x, field_1AC_cam.y);
                    if (pDangerousPed && (pDangerousPed->field_170_selected_weapon || pDangerousPed->field_16C_car) && pDangerousPed != this)
                    {
                        abs_x = Fix16::Abs(pDangerousPed->get_cam_x() - field_1AC_cam.x);
                        abs_y = Fix16::Abs(pDangerousPed->get_cam_y() - field_1AC_cam.y);
                        if (Fix16::Max(abs_x, abs_y) < kFpFour_678680)
                        {
                            if (pDangerousPed->field_16C_car)
                            {
                                Set_F250_IfBit_433DD0(1);
                            }
                            else if (pDangerousPed->field_170_selected_weapon->GetWeaponType_41CC90() == weapon_type::pistol ||
                                     pDangerousPed->field_170_selected_weapon->GetWeaponType_41CC90() == weapon_type::smg ||
                                     pDangerousPed->field_170_selected_weapon->GetWeaponType_41CC90() == weapon_type::shotgun ||
                                     pDangerousPed->field_170_selected_weapon->GetWeaponType_41CC90() == weapon_type::car_smg ||
                                     pDangerousPed->field_170_selected_weapon->GetWeaponType_41CC90() == weapon_type::silence_smg ||
                                     pDangerousPed->field_170_selected_weapon->GetWeaponType_41CC90() == weapon_type::dual_pistol)
                            {
                                Set_F250_IfBit_433DD0(3);
                            }
                            else
                            {
                                Set_F250_IfBit_433DD0(2);
                            }
                            Ped::SetObjective2_463830(objectives_enum::flee_char_on_foot_till_safe_2, 9999);
                            set_field_14C_403AE0(pDangerousPed);

                            field_130 = Fix16::atan2_fixed_405320(field_1AC_cam.y - pDangerousPed->get_cam_y(),
                                                                  field_1AC_cam.x - pDangerousPed->get_cam_x());
                            field_168_game_object->set_rotation_433A30(field_130);
                            field_21C_bf.b2 = true;
                        }
                    }
                }

                if (field_258_objective == objectives_enum::flee_char_on_foot_till_safe_2 &&
                    field_225_objective_status == objective_status::passed_1)
                {
                    Ped::SetObjective(objectives_enum::no_obj_0, 9999);
                }
                if (field_25C_internal_objective == objectives_enum::flee_char_on_foot_till_safe_2 &&
                    field_226_internal_objective_status == 1)
                {
                    Ped::SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                    field_21C_bf.b2 = false;
                }
            }
            break;

        case threat_reaction_enum::react_as_emergency_1:
        case threat_reaction_enum::react_as_normal_2:
            if (field_240_occupation == ped_ocupation_enum::paramedic_23)
            {
                break;
            }

            if (field_16C_car)
            {
                if (field_28C_threat_reaction == threat_reaction_enum::react_as_normal_2)
                {
                    if (field_258_objective && !field_21C_bf.b4 && field_218_objective_timer > 50 &&
                        Ped::FindBestTargetPed_Mode1_466B90(3) && field_16C_car->GetVelocity_43A4C0() == kFpZero_678660)
                    {
                        field_21C_bf.b2 = true;
                        Ped::SetObjective2_463830(objectives_enum::leave_car_36, 9999);
                        field_218_objective_timer = 0;
                        set_target_to_enter_403B00(field_16C_car);
                    }
                }
                else
                {
                    field_288_threat_search = threat_search_enum::line_of_sight_1;
                    Ped::FindBestTargetPed_Mode1_466B90(3);
                }
            }
            else if (field_25C_internal_objective == objectives_enum::kill_char_on_foot_20 || field_25C_internal_objective == 23 ||
                     field_25C_internal_objective == 32)
            {
                if (field_288_threat_search == threat_search_enum::line_of_sight_1 ||
                    field_288_threat_search == threat_search_enum::line_of_sight_player_only_6 ||
                    field_288_threat_search == threat_search_enum::line_of_sight_player_threat_only_4)
                {
                    if (!field_21C_bf.b19 && byte_61A8A2 == 1)
                    {
                        pTarget = Ped::FindBestTargetPed_Mode5_466BD0(3);
                        bLineOfSightChecked = true;
                    }
                }
                else
                {
                    pTarget = Ped::FindBestTargetPed_Mode1_466B90(3);
                }

                if (field_28C_threat_reaction == threat_reaction_enum::react_as_normal_2)
                {
                    if (pTarget && pTarget != field_14C_internal_target_ped)
                    {
                        field_14C_internal_target_ped = pTarget;
                    }
                }
                else
                {
                    if (pTarget && pTarget != field_14C_internal_target_ped)
                    {
                        Fix16 diff_x = pTarget->get_cam_x() - field_1AC_cam.x;
                        Fix16 diff_y = pTarget->get_cam_y() - field_1AC_cam.y;
                        abs_x = Fix16::Abs(diff_x);
                        abs_y = Fix16::Abs(diff_y);
                        if (Fix16::Max(abs_x, abs_y) < kFpTwo_678668)
                        {
                            field_14C_internal_target_ped = pTarget;
                        }
                    }
                    if (field_25C_internal_objective == 32 && (pTarget == field_14C_internal_target_ped || !pTarget))
                    {
                        break;
                    }
                }

                Fix16 diff_x = field_1C4_x - field_1AC_cam.x;
                Fix16 diff_y = field_1C8_y - field_1AC_cam.y;
                abs_x = Fix16::Abs(diff_x);
                abs_y = Fix16::Abs(diff_y);
                Fix16 dist = Fix16::Max(abs_x, abs_y);

                if (field_164_ped_group && field_23C_group_idx != 99)
                {
                    if (field_164_ped_group->IsMemberTooFarFromLeader_4CAC20(field_23C_group_idx))
                    {
                        Ped::SetObjective2_463830(objectives_enum::objective_7, 9999);
                        set_field_14C_403AE0(field_164_ped_group->field_2C_ped_leader);
                        SetBit2_403950();
                    }
                    if (field_164_ped_group->IsLeaderEnteringCarOrUnknown5_4C9220() && field_164_ped_group->IsLeaderCloseToTargetCar_4CAD40())
                    {
                        Ped::SetObjective2_463830(objectives_enum::objective_7, 9999);
                        set_field_14C_403AE0(field_164_ped_group->field_2C_ped_leader);
                        SetBit2_403950();
                    }
                }

                if (dist <= kFpSix_678520)
                {
                    if (field_288_threat_search != threat_search_enum::line_of_sight_1 &&
                        field_288_threat_search != threat_search_enum::line_of_sight_player_only_6 &&
                        field_288_threat_search != threat_search_enum::line_of_sight_player_threat_only_4)
                    {
                        field_20C = 0;
                    }
                    else if (!field_21C_bf.b19 && byte_61A8A2 == 1)
                    {
                        if (!gMap_0x370_6F6268->sub_4E5640(gSpawnJitterScale_678618,
                                                           kFpQuarter_678484,
                                                           gSpawnJitterScale_678618,
                                                           field_1AC_cam.x,
                                                           field_1AC_cam.y,
                                                           field_1AC_cam.z,
                                                           field_14C_internal_target_ped->get_cam_x(),
                                                           field_14C_internal_target_ped->get_cam_y(),
                                                           field_14C_internal_target_ped->get_cam_z()))
                        {
                            ++field_20C;
                        }
                        else
                        {
                            field_20C = 0;
                        }
                        bLineOfSightChecked = true;
                    }
                    else
                    {
                        ++field_20C;
                    }
                }
                else
                {
                    ++field_20C;
                }

                if ((field_288_threat_search == threat_search_enum::line_of_sight_1 ||
                     field_288_threat_search == threat_search_enum::line_of_sight_player_only_6 ||
                     field_288_threat_search == threat_search_enum::line_of_sight_player_threat_only_4) &&
                    bLineOfSightChecked == true)
                {
                    field_21C_bf.b19 = true;
                    byte_61A8A2 = 0;
                }

                if (field_20C > 15)
                {
                    bGiveUp = true;
                }
                if (field_14C_internal_target_ped->sub_433DA0())
                {
                    bGiveUp = true;
                }
                if (field_226_internal_objective_status == 1)
                {
                    bGiveUp = true;
                }
                if (field_226_internal_objective_status == 2)
                {
                    bGiveUp = true;
                }
                if (field_28C_threat_reaction == threat_reaction_enum::react_as_emergency_1)
                {
                    if (field_14C_internal_target_ped->get_wanted_star_count_46EF00() == 0 && field_14C_internal_target_ped->is_player_41B0A0())
                    {
                        bGiveUp = true;
                    }
                    if (field_258_objective == objectives_enum::enter_car_as_driver_35)
                    {
                        bGiveUp = true;
                    }
                }

                if (bGiveUp == true)
                {
                    if (field_14C_internal_target_ped != field_148_objective_target_ped)
                    {
                        if (field_168_game_object->GetCharState_433A80() != Char_B4_state::Jumping_15)
                        {
                            field_0_patrol_points[0].field_0_x = 0;
                            field_0_patrol_points[0].field_1_y = 0;
                            field_21C_bf.b2 = false;
                            field_21C_bf.b11 = false;
                        }
                        Ped::SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                    }
                    else
                    {
                        field_226_internal_objective_status = 2;
                    }
                }
            }
            else
            {
                field_20C = 0;
                if (field_21C_bf.b27)
                {
                    field_21C_bf.b2 = false;
                }
                else if (!field_164_ped_group ||
                         (!field_164_ped_group->IsLeaderEnteringCarOrUnknown5_4C9220() && !field_164_ped_group->IsLeaderInCar_4C9210()))
                {
                    if (field_288_threat_search == threat_search_enum::line_of_sight_1 ||
                        field_288_threat_search == threat_search_enum::line_of_sight_player_only_6 ||
                        field_288_threat_search == threat_search_enum::line_of_sight_player_threat_only_4)
                    {
                        if (!field_21C_bf.b19 && byte_61A8A2 == 1)
                        {
                            pTarget = Ped::FindBestTargetPed_Mode5_466BD0(3);
                            bLineOfSightChecked = true;
                        }
                    }
                    else
                    {
                        pTarget = Ped::FindBestTargetPed_Mode1_466B90(3);
                    }

                    if (pTarget && pTarget->field_21C_bf.b0)
                    {
                        if (field_28C_threat_reaction == threat_reaction_enum::react_as_emergency_1)
                        {
                            Set_F250_IfBit_433DD0(17);
                        }
                        if (!pTarget->get_car_416B60())
                        {
                            if (field_164_ped_group)
                            {
                                field_164_ped_group->MergeWithOtherGroup_4C9B60(pTarget);
                            }
                            else if (field_258_objective != objectives_enum::goto_area_any_means_13)
                            {
                                field_21C_bf.b2 = true;
                                Ped::SetObjective2_463830(objectives_enum::kill_char_on_foot_20, 9999);
                                field_14C_internal_target_ped = pTarget;
                                pTarget->field_144_attacker = this;
                                field_20C = 0;
                            }
                        }
                        else if (!field_164_ped_group)
                        {
                            field_21C_bf.b2 = true;
                            Ped::SetObjective2_463830(objectives_enum::kill_char_on_foot_20, 9999);
                            field_14C_internal_target_ped = pTarget;
                            field_20C = 0;
                        }
                        else
                        {
                            field_164_ped_group->MergeWithOtherGroup_4C9B60(pTarget);
                        }
                    }
                    else
                    {
                        field_21C_bf.b2 = false;
                    }

                    if ((field_288_threat_search == threat_search_enum::line_of_sight_1 ||
                         field_288_threat_search == threat_search_enum::line_of_sight_player_only_6 ||
                         field_288_threat_search == threat_search_enum::line_of_sight_player_threat_only_4) &&
                        bLineOfSightChecked == true)
                    {
                        field_21C_bf.b19 = true;
                        byte_61A8A2 = 0;
                    }
                }
            }
            break;

        default:
            break;
    }
}

MATCH_FUNC(0x465b20)
void Ped::ReactToAttacker_465B20()
{
    if (field_144_attacker->isDead_403B60() || !field_144_attacker->CheckBit0_433B40())
    {
        this->field_144_attacker = 0;
        this->field_21C &= ~4;
    }
    else if (!field_144_attacker->sub_433DA0())
    {
        Gang_144* pZone = this->field_17C_pGang;
        if (pZone && pZone == field_144_attacker->field_17C_pGang)
        {
            this->field_144_attacker = 0;
            this->field_21C &= ~4;
        }
        else
        {
            PedGroup* pGroup = this->field_164_ped_group;
            if (pGroup && pGroup == field_144_attacker->field_164_ped_group)
            {
                this->field_144_attacker = 0;
                this->field_21C &= ~4;
            }
            else
            {
                if (field_28C_threat_reaction > threat_reaction_enum::no_reaction_0)
                {
                    if (field_28C_threat_reaction > threat_reaction_enum::react_as_normal_2)
                    {
                        if (field_28C_threat_reaction == threat_reaction_enum::run_away_3)
                        {
                            if (this->field_168_game_object)
                            {
                                SetObjective2_463830(2, 9999);
                                this->field_14C_internal_target_ped = this->field_144_attacker;
                            }
                        }
                    }
                    else if (this->field_240_occupation != ped_ocupation_enum::paramedic_23)
                    {
                        if (this->field_168_game_object)
                        {
                            if (!pGroup)
                            {
                                this->field_218_objective_timer = 0;
                                if (field_170_selected_weapon)
                                {
                                    Ped::SetObjective2_463830(20, 9999);
                                }
                                else
                                {
                                    Ped::SetObjective2_463830(23, 9999);
                                }
                                this->field_14C_internal_target_ped = this->field_144_attacker;
                                this->field_21C |= 4;
                            }
                            else
                            {
                                if (pGroup->field_2C_ped_leader->is_player_41B0A0())
                                {
                                    if (!field_144_attacker->field_20e_offscreen_counter && this->field_20C < 5u &&
                                        Fix16::Abs(field_144_attacker->get_cam_z() - field_1AC_cam.z) < kFpOne_678664)
                                    {
                                        pGroup->MergeWithOtherGroup_4C9B60(field_144_attacker);
                                    }
                                    else
                                    {
                                        this->field_144_attacker = 0;
                                    }
                                }
                                else
                                {
                                    pGroup->MergeWithOtherGroup_4C9B60(field_144_attacker);
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
bool Ped::sub_465CD0()
{
    if (field_21C_bf.b2 == true)
    {
        if (field_14C_internal_target_ped)
        {
            if (field_25C_internal_objective == objectives_enum::kill_char_on_foot_20 
                || field_25C_internal_objective == objectives_enum::punch_char_23)
            {
                return true;
            }
        }
    }
    return false;
}

// https://decomp.me/scratch/Fh1iq
MATCH_FUNC(0x466b70)
char_type Ped::sub_466B70()
{
    if ((field_21C & 0x2000000) != 0 && field_168_game_object != 0)
    {
        return 1;
    }

    return 0;
}

// Returns Fix16 by value (hidden out pointer, ret $0x14). Max of the two Abs written out with
// the y abs first: Fix16::Max takes references and spills both to the stack.
MATCH_FUNC(0x4614e0)
Fix16 __stdcall sub_4614E0(Fix16& x1, Fix16& y1, Fix16& x2, Fix16& y2)
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
WIP_FUNC(0x465d00)
bool Ped::IsPedAThreat_465D00(Ped* a2)
{
    WIP_IMPLEMENTED;


    char_type flag = 0;

    if ((a2->field_21C & 0x2000000) != 0)
    {
        if (a2->field_168_game_object != 0)
        {
            goto ret_false;
        }
    }

    if (this->field_288_threat_search == 3 || this->field_288_threat_search == 4)
    {
        if (!a2->IsField238_45EDE0(2))
        {
            goto ret_false;
        }
    }

    if (this->field_288_threat_search == 5 || this->field_288_threat_search == 6)
    {
        if (a2->sub_45EDC0())
        {
            goto ret_true;
        }
    }

    if (this->sub_45EDC0())
    {
        flag = 1;
    }

    if (this->field_164_ped_group == a2->field_164_ped_group && this->field_164_ped_group != 0)
    {
        goto ret_false;
    }

    if (this->field_240_occupation == 0x28)
    {
        Car_BC* pCar = a2->field_16C_car;

        if (pCar == 0)
        {
            goto ret_true;
        }

        if (!pCar->IsPoliceCar_439EC0())
        {
            goto ret_true;
        }

        if (a2->field_238_ped_type == 4)
        {
            goto ret_false;
        }

        return 1;
    }

    {
        Gang_144* pMyGang = this->field_17C_pGang;

        if (pMyGang != 0)
        {
            Gang_144* pOtherGang = a2->field_17C_pGang;

            if (pOtherGang != 0)
            {
                if (pOtherGang == pMyGang)
                {
                    goto merge_178;
                }

                if (!pMyGang->IsHostileToGang_4BEDF0(pOtherGang->field_1_gang_idx))
                {
                    goto ret_false;
                }

                if (this->field_238_ped_type == 4)
                {
                    goto check_threat_level;
                }

                if (this->field_238_ped_type != 6)
                {
                    goto ret_true;
                }

            check_threat_level:
                if ((u8)a2->field_263_prev_attackers_count >= 4)
                {
                    goto ret_false;
                }

                if ((u8)a2->field_262_attackers_count >= 4)
                {
                    goto ret_false;
                }

                return 1;
            }

            if (pMyGang->field_110_high_respect != 0)
            {
                switch (a2->field_240_occupation)
                {
                    case 0x18:
                    case 0x19:
                    case 0x1A:
                    case 0x1B:
                    case 0x1D:
                    case 0x1E:
                    case 0x1F:
                    case 0x25:
                        goto ret_true;
                    default:
                        break;
                }
            }

            if (((BitSet32*)&a2->field_21C)->check_bit(0xB))
            {
                if (!a2->sub_45EDC0() && a2->field_240_occupation != 1)
                {
                    if (a2->field_28C_threat_reaction == 1)
                    {
                        goto merge_178;
                    }

                    if (this->field_170_selected_weapon != 0)
                    {
                        goto ret_true;
                    }

                    Fix16 dy = a2->field_1AC_cam.y - this->field_1AC_cam.y;
                    Fix16 dx = a2->field_1AC_cam.x - this->field_1AC_cam.x;
                    Fix16 dyabs = Fix16::Abs_negate_out_of_line(dy);
                    Fix16 dxabs = Fix16::Abs_negate_out_of_line(dx);

                    if (Fix16::Max_44E540(dxabs, dyabs) <= kFpOne_678798)
                    {
                        goto ret_true;
                    }

                    goto merge_178;
                }

                {
                    u8 player_idx = a2->field_15C_player->field_2E_idx;

                    if (this->field_17C_pGang->IsRespectNegativeForPlayer_4BEF10(player_idx))
                    {
                        if (gPolice_7B8_6FEE40->field_7B4 == 0)
                        {
                            goto ret_true;
                        }

                        return 0;
                    }

                    goto ret_false;
                }
            }

        block_465F75:
            if (a2->sub_45EDC0() || a2->field_240_occupation == 1)
            {
                u8 player_idx = a2->field_15C_player->field_2E_idx;

                if (this->field_17C_pGang->IsRespectNegativeForPlayer_4BEF10(player_idx))
                {
                    if (gPolice_7B8_6FEE40->field_7B4 == 0)
                    {
                        goto ret_true;
                    }
                    return 0;
                }

                goto ret_false;
            }

            goto merge_178;
        }
    }

no_my_gang:
    switch (this->field_240_occupation)
    {
        case 0x21:
            if (a2->field_240_occupation == 0x21)
            {
                return 0;
            }

            goto merge_178;
        case 0x22:
            if (a2->field_240_occupation == 0x22)
            {
                return 0;
            }

            goto merge_178;
        case 0x16:
            if (a2->IsField238_45EDE0(2))
            {
                return 0;
            }

            goto merge_178;
        case 0x18:
        case 0x19:
        case 0x1A:
        case 0x1B:
        case 0x1D:
        case 0x1E:
        case 0x1F:
        case 0x25:
            goto block_466022;
        default:
            goto block_46613E;
    }

block_466022:
    if (a2->sub_45EDC0())
    {
        if (a2->field_168_game_object != 0 && a2->field_168_game_object->field_10_char_state == 0xF)
        {
            goto ret_false;
        }

        if (!((((BitSet32*)&a2->field_21C)->check_bit(0xB)) && a2->field_170_selected_weapon != 0 &&
              a2->field_170_selected_weapon->sub_5DCEF0()))
        {
            if (a2->field_20A_wanted_points < 0x258 && this->field_144_attacker != a2 && a2->field_26A_recent_crime_timer <= 0u)
            {
                goto ret_false;
            }
        }

        this->field_144_attacker = 0;
        gPolice_7B8_6FEE40->UpdateLastSeenCoordsForCriminal_5708C0(a2);

        if (this->field_258_objective == 0x2B)
        {
            if (gPolice_7B8_6FEE40->PromptCrewAtCarToPurseCriminal_5707B0(this->field_16C_car, a2))
            {
                goto ret_true;
            }

            if (a2->field_20A_wanted_points < 0x258)
            {
                a2->field_20A_wanted_points = 0x258;
            }

            return 0;
        }

        if (a2->field_20A_wanted_points >= 0x258)
        {
            goto ret_true;
        }

        a2->field_20A_wanted_points = 0x258;
        return 1;
    }

block_4660FB:
{
    s32 a2_state = a2->field_240_occupation;
    switch (a2_state)
    {
        case 0x17:
        case 0x18:
        case 0x19:
        case 0x1A:
        case 0x1B:
        case 0x1D:
        case 0x1E:
        case 0x1F:
        case 0x25:
        case 0x27:
            goto merge_178;
        default:
            break;
    }

    if (a2_state == 1)
    {
        goto ret_true;
    }

    if (((BitSet32*)&a2->field_21C)->check_bit(0xB))
    {
        goto ret_true;
    }

    if (a2->field_25C_internal_objective != 0x14)
    {
        goto merge_178;
    }

    return 1;
}

block_46613E:
    if (this->field_164_ped_group != 0)
    {
        if (this->field_164_ped_group->field_2C_ped_leader != a2->field_14C_internal_target_ped)
        {
            goto merge_178;
        }

        if (a2->field_25C_internal_objective == 0x14)
        {
            goto ret_true;
        }

        if (a2->field_25C_internal_objective != 0x17)
        {
            goto merge_178;
        }

        return 1;
    }

    if (((BitSet32*)&a2->field_21C)->check_bit(0xB))
    {
        goto ret_true;
    }

merge_178:
    if (flag == 1)
    {
        goto ret_true;
    }

    goto ret_false;

ret_true:
    return 1;

ret_false:
    return 0;
}

// https://decomp.me/scratch/0VTTk
WIP_FUNC(0x4661F0)
char_type Ped::IsThreatToSearchingPed_4661F0()
{
    WIP_IMPLEMENTED;

    Ped* pSearcher;
    char_type flag;

    switch (gTargetSearchMode_6787D7)
    {
        case 1:
            if (field_238_ped_type == 3)
            {
                goto ret_false;
            }

            if (field_278_ped_state_1 == 9 || field_278_ped_state_1 == 8)
            {
                goto ret_false;
            }

            if ((field_21C & 1) == 0)
            {
                goto ret_false;
            }

            pSearcher = gSearchingPed_6787DC;
            flag = 0;

            if (sub_466B70())
            {
                goto ret_false;
            }

            if (pSearcher->field_288_threat_search == 3 || pSearcher->field_288_threat_search == 4)
            {
                if (!this->IsField238_45EDE0(2))
                {
                    goto ret_false;
                }
            }

            if (pSearcher->field_288_threat_search == 5 || pSearcher->field_288_threat_search == 6)
            {
                if (this->sub_45EDC0())
                {
                    goto ret_true;
                }
            }

            if (pSearcher->sub_45EDC0())
            {
                flag = 1;
            }

            if (pSearcher->field_164_ped_group == this->field_164_ped_group && pSearcher->field_164_ped_group != 0)
            {
                goto ret_false;
            }

            if (pSearcher->field_240_occupation == 0x28)
            {
                Car_BC* pCar = this->field_16C_car;

                if (pCar == 0)
                {
                    goto ret_true;
                }

                if (!pCar->IsPoliceCar_439EC0())
                {
                    goto ret_true;
                }

                if (this->field_238_ped_type == 4)
                {
                    goto ret_false;
                }

                return 1;
            }

            {
                Gang_144* pMyGang = pSearcher->field_17C_pGang;

                if (pMyGang != 0)
                {
                    Gang_144* pOtherGang = this->field_17C_pGang;

                    if (pOtherGang != 0)
                    {
                        if (pOtherGang == pMyGang)
                        {
                            goto merge_178;
                        }

                        if (!pMyGang->IsHostileToGang_4BEDF0(pOtherGang->field_1_gang_idx))
                        {
                            goto ret_false;
                        }

                        if (pSearcher->field_238_ped_type == 4)
                        {
                            goto check_threat_level;
                        }

                        if (pSearcher->field_238_ped_type != 6)
                        {
                            goto ret_true;
                        }

                    check_threat_level:
                        if ((u8)this->field_263_prev_attackers_count >= 4)
                        {
                            goto ret_false;
                        }

                        if ((u8)this->field_262_attackers_count >= 4)
                        {
                            goto ret_false;
                        }

                        return 1;
                    }

                    if (pMyGang->field_110_high_respect != 0)
                    {
                        switch (this->field_240_occupation)
                        {
                            case 0x18:
                            case 0x19:
                            case 0x1A:
                            case 0x1B:
                            case 0x1D:
                            case 0x1E:
                            case 0x1F:
                            case 0x25:
                                goto ret_true;
                            default:
                                break;
                        }
                    }

                    if (((BitSet32*)&this->field_21C)->check_bit(0xB))
                    {
                        if (!this->sub_45EDC0() && this->field_240_occupation != 1)
                        {
                            if (this->field_28C_threat_reaction == 1)
                            {
                                goto merge_178;
                            }

                            if (pSearcher->field_170_selected_weapon != 0)
                            {
                                goto ret_true;
                            }

                            Fix16 candY = this->field_1AC_cam.y;
                            Fix16 candX = this->field_1AC_cam.x;

                            if (sub_4614E0(pSearcher->field_1AC_cam.x, pSearcher->field_1AC_cam.y, candX, candY).mValue <=
                                kFpOne_678798.mValue)
                            {
                                goto ret_true;
                            }

                            goto merge_178;
                        }

                        {
                            if (pSearcher->field_17C_pGang->IsRespectNegativeForPlayer_4BEF10(this->field_15C_player->field_2E_idx))
                            {
                                if (gPolice_7B8_6FEE40->field_7B4 == 0)
                                {
                                    goto ret_true;
                                }

                                return 0;
                            }

                            goto ret_false;
                        }
                    }

                    if (this->sub_45EDC0() || this->field_240_occupation == 1)
                    {
                        u8 player_idx = this->field_15C_player->field_2E_idx;

                        if (pSearcher->field_17C_pGang->IsRespectNegativeForPlayer_4BEF10(player_idx))
                        {
                            if (gPolice_7B8_6FEE40->field_7B4 == 0)
                            {
                                goto ret_true;
                            }

                            return 0;
                        }

                        goto ret_false;
                    }

                    goto merge_178;
                }
            }

            switch (pSearcher->field_240_occupation)
            {
                case 0x21:
                    if (this->field_240_occupation == 0x21)
                    {
                        goto ret_false;
                    }

                    goto merge_178;
                case 0x22:
                    if (this->field_240_occupation == 0x22)
                    {
                        goto ret_false;
                    }

                    goto merge_178;
                case 0x16:
                    if (this->IsField238_45EDE0(2))
                    {
                        goto ret_false;
                    }

                    goto merge_178;
                case 0x18:
                case 0x19:
                case 0x1A:
                case 0x1B:
                case 0x1D:
                case 0x1E:
                case 0x1F:
                case 0x25:
                    goto block_466022;
                default:
                    goto block_46613E;
            }

        block_466022:
            if (this->sub_45EDC0())
            {
                if (this->field_168_game_object != 0 && this->field_168_game_object->field_10_char_state == 0xF)
                {
                    goto ret_false;
                }

                if (!((((BitSet32*)&this->field_21C)->check_bit(0xB)) && this->field_170_selected_weapon != 0 &&
                      this->field_170_selected_weapon->sub_5DCEF0()))
                {
                    if (this->field_20A_wanted_points < 0x258 && pSearcher->field_144_attacker != this && this->field_26A_recent_crime_timer <= 0u)
                    {
                        goto ret_false;
                    }
                }

                pSearcher->field_144_attacker = 0;
                gPolice_7B8_6FEE40->UpdateLastSeenCoordsForCriminal_5708C0(this);

                if (pSearcher->field_258_objective == 0x2B)
                {
                    if (gPolice_7B8_6FEE40->PromptCrewAtCarToPurseCriminal_5707B0(pSearcher->field_16C_car, this))
                    {
                        goto ret_true;
                    }

                    if (this->field_20A_wanted_points < 0x258)
                    {
                        this->field_20A_wanted_points = 0x258;
                    }

                    return 0;
                }

                if (this->field_20A_wanted_points >= 0x258)
                {
                    goto ret_true;
                }

                this->field_20A_wanted_points = 0x258;
                return 1;
            }

            {
                s32 a2_state = this->field_240_occupation;
                switch (a2_state)
                {
                    case 0x17:
                    case 0x18:
                    case 0x19:
                    case 0x1A:
                    case 0x1B:
                    case 0x1D:
                    case 0x1E:
                    case 0x1F:
                    case 0x25:
                    case 0x27:
                        goto merge_178;
                    default:
                        break;
                }

                if (a2_state == 1)
                {
                    goto ret_true;
                }

                if (((BitSet32*)&this->field_21C)->check_bit(0xB))
                {
                    goto ret_true;
                }

                if (this->field_25C_internal_objective != 0x14)
                {
                    goto merge_178;
                }

                return 1;
            }

        block_46613E:
            if (pSearcher->field_164_ped_group != 0)
            {
                if (pSearcher->field_164_ped_group->field_2C_ped_leader != this->field_14C_internal_target_ped)
                {
                    goto merge_178;
                }

                if (this->field_25C_internal_objective == 0x14)
                {
                    goto ret_true;
                }

                if (this->field_25C_internal_objective != 0x17)
                {
                    goto merge_178;
                }

                return 1;
            }

            if (((BitSet32*)&this->field_21C)->check_bit(0xB))
            {
                goto ret_true;
            }

        merge_178:
            if (flag == 1)
            {
                goto ret_true;
            }

            goto ret_false;

        case 2:
            if (field_16C_car != 0)
            {
                goto ret_false;
            }

            if (field_278_ped_state_1 == 9 || field_278_ped_state_1 == 8)
            {
                goto ret_false;
            }

            if ((field_21C & 1) == 0)
            {
                goto ret_false;
            }

            if (gSearchingPed_6787DC == this)
            {
                goto ret_false;
            }

            return 1;

        case 3:
            if (field_16C_car != 0)
            {
                goto ret_false;
            }

            if (field_278_ped_state_1 == 9 || field_278_ped_state_1 == 8)
            {
                goto ret_false;
            }

            if ((field_21C & 1) == 0)
            {
                goto ret_false;
            }

            {
                Ped* pS = gSearchingPed_6787DC;

                if (pS == this)
                {
                    goto ret_false;
                }

                Fix16 sy = pS->field_1AC_cam.y;
                Fix16 sx = pS->field_1AC_cam.x;
                Fix16 dx;
                dx = sx.Subtract_436A00(this->field_1AC_cam.x);
                Fix16 dy;
                dy = sy.Subtract_436A00(this->field_1AC_cam.y);

                if (Fix16::Max_44E540(Fix16::Abs_436A50(dx), Fix16::Abs_436A50(dy)).mValue > kFpQuarter_678788.mValue)
                {
                    goto ret_false;
                }

                return 1;
            }

        case 4:
            if (field_16C_car != 0)
            {
                goto ret_false;
            }

            if (field_278_ped_state_1 == 9 || field_278_ped_state_1 == 8)
            {
                goto ret_false;
            }

            if ((field_21C & 1) == 0)
            {
                goto ret_false;
            }

            {
                Ped* pS = gSearchingPed_6787DC;

                if (pS == this)
                {
                    goto ret_false;
                }

                Fix16 dx = this->field_1AC_cam.x - pS->field_1AC_cam.x;
                Fix16 dy = this->field_1AC_cam.y - pS->field_1AC_cam.y;
                Ang16 rel(Ang16(Fix16::atan2_fixed_405320(dy, dx).rValue - gSearchingPed_6787DC->GetRotation().rValue), 0);
                Ang16 range(Ang16(gDummyPedAng_6787A8.rValue - word_6784F0.rValue), 0);

                if (!(rel < word_6784F0))
                {
                    if (rel <= range)
                    {
                        goto ret_false;
                    }
                }
                return 1;
            }

        case 5:
            if (field_278_ped_state_1 == 9 || field_278_ped_state_1 == 8)
            {
                goto ret_false;
            }

            if ((field_21C & 1) == 0)
            {
                goto ret_false;
            }

            if (field_238_ped_type != 2)
            {
                if (!gSearchingPed_6787DC->IsPedAThreat_465D00(this))
                {
                    return 0;
                }
            }
            else
            {
                Player* pPlayer = this->field_15C_player;
                Camera_0xBC* pCam;

                if (pPlayer->field_68_camera_mode == 2 || pPlayer->field_68_camera_mode == 3)
                {
                    pCam = &pPlayer->field_208_aux_game_camera;
                }
                else
                {
                    pCam = &pPlayer->field_90_game_camera;
                }

                Char_B4* pObj = gSearchingPed_6787DC->field_168_game_object;

                if (pObj != 0)
                {
                    if (!pCam->IsSpriteInView_435630(pObj->field_80_sprite_ptr, 1))
                    {
                        return 0;
                    }
                }
                else
                {
                    if (!pCam->IsSpriteInView_435630(gSearchingPed_6787DC->field_16C_car->field_50_car_sprite, 1))
                    {
                        goto ret_false;
                    }
                }
            }

            word_6784F0.rValue = *(s16*)&kAng90_6784B0;

            {
                Ped* pS = gSearchingPed_6787DC;

                if (pS == this)
                {
                    goto ret_false;
                }

                Fix16 dx = this->field_1AC_cam.x - pS->field_1AC_cam.x;
                Fix16 dy = this->field_1AC_cam.y - pS->field_1AC_cam.y;
                Ang16 rel(Ang16(Fix16::atan2_fixed_405320(dy, dx).rValue - gSearchingPed_6787DC->GetRotation().rValue), 0);
                Ang16 range(Ang16(gDummyPedAng_6787A8.rValue - word_6784F0.rValue), 0);

                if (!(rel < word_6784F0))
                {
                    if (rel <= range)
                    {
                        goto ret_false;
                    }
                }
            }

            {
                if (gSearchingPed_6787DC->field_164_ped_group != 0 && gSearchingPed_6787DC->field_164_ped_group->field_2C_ped_leader->field_15C_player != 0)
                {
                    Fix16 dz = gSearchingPed_6787DC->field_1AC_cam.z - this->field_1AC_cam.z;

                    if (Fix16::Abs_negate_out_of_line(dz) >= kFpOne_678664)
                    {
                        goto ret_false;
                    }
                }

                Fix16 sx = gSearchingPed_6787DC->field_1AC_cam.x;
                Fix16 sy = gSearchingPed_6787DC->field_1AC_cam.y;
                Fix16 sz = gSearchingPed_6787DC->field_1AC_cam.z;

                if (field_238_ped_type != 2)
                {
                    return gMap_0x370_6F6268->sub_4E5640(gSpawnJitterScale_678618 * 2,
                                                         kFpQuarter_678484,
                                                         gSpawnJitterScale_678618,
                                                         sx,
                                                         sy,
                                                         sz,
                                                         this->field_1AC_cam.x,
                                                         this->field_1AC_cam.y,
                                                         this->field_1AC_cam.z);
                }

                if (gMap_0x370_6F6268->sub_4E5640(kFpQuarter_678484,
                                                  kFpQuarter_678484,
                                                  gSpawnJitterScale_678618,
                                                  sx,
                                                  sy,
                                                  sz,
                                                  this->field_1AC_cam.x,
                                                  this->field_1AC_cam.y,
                                                  this->field_1AC_cam.z))
                {
                    gSearchingPed_6787DC->field_21C |= 0x800000;
                    return gSearchingPed_6787DC->IsPedAThreat_465D00(this);
                }

                gSearchingPed_6787DC->field_21C &= ~0x800000;
                return 0;
            }
    }

    goto ret_false;

ret_true:
    return 1;

ret_false:
    return 0;
}

MATCH_FUNC(0x466b90)
Ped* Ped::FindBestTargetPed_Mode1_466B90(s32 max_x_check)
{
    gTargetSearchMode_6787D7 = 1;
    return Ped::FindBestTargetPed_466BF0(max_x_check);
}

MATCH_FUNC(0x466bb0)
Ped* Ped::FindBestTargetPed_Mode4_466BB0(s32 max_x_check)
{
    gTargetSearchMode_6787D7 = 4;
    return Ped::FindBestTargetPed_466BF0(max_x_check);
}

MATCH_FUNC(0x466bd0)
Ped* Ped::FindBestTargetPed_Mode5_466BD0(s32 max_x_check)
{
    gTargetSearchMode_6787D7 = 5;
    return Ped::FindBestTargetPed_466BF0(max_x_check);
}

// https://decomp.me/scratch/jl40w
// 9.6f 0x437BE0
MATCH_FUNC(0x466bf0)
Ped* Ped::FindBestTargetPed_466BF0(s32 a2)
{
    gSearchingPed_6787DC = this;

    // Two calls in the source: VC6 merges their identical heads and tails
    Sprite* pNear;
    if (field_168_game_object)
    {
        pNear = gPurpleDoom_1_679208->FindNearestSprite_SpiralSearch_477C90(sprite_types_enum::ped_3,
                                                                            sprite_types_enum::car_2,
                                                                            field_168_game_object->field_80_sprite_ptr,
                                                                            a2,
                                                                            0,
                                                                            0);
    }
    else
    {
        pNear = gPurpleDoom_1_679208->FindNearestSprite_SpiralSearch_477C90(sprite_types_enum::ped_3,
                                                                            sprite_types_enum::car_2,
                                                                            field_16C_car->field_50_car_sprite,
                                                                            a2,
                                                                            0,
                                                                            0);
    }

    if (!pNear)
    {
        Ped* pClosest;
        if (field_164_ped_group)
        {
            if (field_164_ped_group->field_2C_ped_leader->is_player_41B0A0())
            {
                pClosest = 0;
            }
            else
            {
                pClosest = gThreateningPedsList_678468.GetFromListClosestPedToPoint_471340(field_1AC_cam.x, field_1AC_cam.y);
            }
        }
        else
        {
            pClosest = gThreateningPedsList_678468.GetFromListClosestPedToPoint_471340(field_1AC_cam.x, field_1AC_cam.y);
        }

        if (IsNetworkGame_434B10() && field_164_ped_group)
        {
            Fix16 best = kFpFour_678670;
            Ped* pBestPed = 0;
            for (Player* pPlayer = gGame_0x40_67E008->IterateFirstPlayer_4B9CD0(); pPlayer;
                 pPlayer = gGame_0x40_67E008->IterateNextPlayer_4B9D10())
            {
                Ped* pPlayerPed = pPlayer->field_2C4_player_ped;
                if (pPlayerPed && field_164_ped_group != pPlayerPed->field_164_ped_group)
                {
                    Fix16 dist;
                    dist = Fix16::MaxAbsDistance_42A6B0(get_cam_x(), get_cam_y(), pPlayerPed->get_cam_x(), pPlayerPed->get_cam_y());
                    if (dist < best)
                    {
                        pBestPed = pPlayerPed;
                        best = dist;
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
            dz = Fix16::Abs(field_1AC_cam.z - pClosest->field_1AC_cam.z);
            if (dz < kFpOne_678664 && pClosest != gSearchingPed_6787DC &&
                Fix16::MaxAbsDistance_42A6B0(pClosest->get_cam_x(),
                                             pClosest->get_cam_y(),
                                             gSearchingPed_6787DC->get_cam_x(),
                                             gSearchingPed_6787DC->get_cam_y()) < kFpFour_678670)
            {
                if (pClosest->IsField238_45EDE0(2))
                {
                    Camera_0xBC* pCam = pClosest->field_15C_player->get_camera_434900();
                    if (gSearchingPed_6787DC->field_168_game_object)
                    {
                        if (!pCam->IsSpriteInView_435630(gSearchingPed_6787DC->field_168_game_object->field_80_sprite_ptr, 1))
                        {
                            return 0;
                        }
                    }
                    else if (!pCam->IsSpriteInView_435630(gSearchingPed_6787DC->field_16C_car->field_50_car_sprite, 1))
                    {
                        return 0;
                    }
                }

                if (IsPedAThreat_465D00(pClosest))
                {
                    return pClosest;
                }
            }
        }
    }

    if (pNear)
    {
        switch (pNear->get_type_416B40())
        {
            case sprite_types_enum::car_2:
                return pNear->AsCar_40FEB0()->get_driver_4118B0();
            case sprite_types_enum::ped_3:
                return pNear->AsCharB4_40FEA0()->field_7C_pPed;
        }
    }
    return 0;
}

MATCH_FUNC(0x466f40)
Ped* Ped::FindNearestPed_Mode4_466F40(u8 a2)
{
    gTargetSearchMode_6787D7 = 4;
    return Ped::FindNearestPed_466F60(a2);
}

MATCH_FUNC(0x466f60)
Ped* Ped::FindNearestPed_466F60(u8 a2)
{
    gSearchingPed_6787DC = this;
    Sprite* pSprite = gPurpleDoom_1_679208->FindNearestSprite_SpiralSearch_477C90(sprite_types_enum::ped_3,
                                                                                  sprite_types_enum::car_2,
                                                                                  field_168_game_object->field_80_sprite_ptr,
                                                                                  a2,
                                                                                  0,
                                                                                  0);
    if (pSprite)
    {
        // @OG_BUG: Null de-ref
        return pSprite->AsCharB4_40FEA0()->field_7C_pPed;
    }
    return 0;
}

MATCH_FUNC(0x466fb0)
Ped* Ped::FindNearbyPed_466FB0()
{
    gTargetSearchMode_6787D7 = 3;
    gSearchingPed_6787DC = this;
    Sprite* pNearest = gPurpleDoom_1_679208->FindNearestSprite_SpiralSearch_477C90(sprite_types_enum::ped_3,
                                                                                   sprite_types_enum::car_2,
                                                                                   field_168_game_object->field_80_sprite_ptr,
                                                                                   3u,
                                                                                   1,
                                                                                   0);
    if (pNearest)
    {
        if (Fix16::MaxAbsDistance_42A6B0(field_1AC_cam.x, field_1AC_cam.y, pNearest->field_14_xy.x, pNearest->field_14_xy.y) <
            kFpQuarter_678788)
        {
            // @OG_BUG: Null de-ref
            return pNearest->AsCharB4_40FEA0()->field_7C_pPed;
        }
    }

    return 0;
}

MATCH_FUNC(0x467070)
Ped* Ped::GetLastProcessedPedOnFoot_467070()
{
    return gLastProcessedPed_6787C0->field_168_game_object != 0 ? gLastProcessedPed_6787C0 : 0;
}

WIP_FUNC(0x467090)
char_type Ped::FindUsableCarDoor_467090()
{
    WIP_IMPLEMENTED;
    Car_BC* pTargetToEnter = this->field_154_target_to_enter;
    if (pTargetToEnter ||
        (!this->field_150_target_objective_car || this->field_27C_ped_state_2 == ped_state_2::ped2_getting_out_a_car_7 ||
         this->field_258_objective == objectives_enum::leave_car_36) &&
            this->field_168_game_object && (pTargetToEnter = this->field_168_game_object->field_84_target_car) != 0)
    {
        char_type isPedKind = IsLawEnforcement_45B4E0();
        Fix16 vel_to_check = kFpPoint2_67856C;
        if (isPedKind)
        {
            vel_to_check = kFpPoint1_678428;
        }
        if ((pTargetToEnter->GetVelocity_43A4C0() <= vel_to_check // car going slow enough?
             || this->field_25C_internal_objective == 36 || this->field_27C_ped_state_2 == ped_state_2::Unknown_17) &&
            !pTargetToEnter->IsDespawning_4215B0() && !pTargetToEnter->IsMaxDamage_40F890() &&
            (this->field_278_ped_state_1 == ped_state_1::exiting_car_4 || pTargetToEnter->IsDoorLockedForPed_43B2B0(this) != true) // can enter this car?
            && !pTargetToEnter->sub_4214D0())
        {
            u8 door = this->field_24C_target_car_door;
            if (!this->field_248_enter_car_as_passenger)
            {
                if (door < (u8)pTargetToEnter->GetRemap())
                {
                    do
                    {
                        if (pTargetToEnter->IsDoorAccessible_43AFE0(door))
                        {
                            goto found;
                        }
                        Car_Door_10* pDoor = pTargetToEnter->GetDoor(this->field_24C_target_car_door);
                        pDoor->Close_439EA0();
                        pDoor->set_ped_421380(0);
                        if (this->field_27C_ped_state_2 == ped_state_2::ped2_entering_a_car_6 ||
                            this->field_27C_ped_state_2 == ped_state_2::ped2_getting_out_a_car_7)
                        {
                            return 0;
                        }
                    } while (++door < (u8)pTargetToEnter->GetRemap());
                    return 0;
                found: // the passenger searches below jump here too
                    this->field_24C_target_car_door = door;
                    return 1;
                }
            }
            else
            {
                for (; door < (u8)pTargetToEnter->GetRemap(); door++)
                {
                    if (pTargetToEnter->IsDoorAccessible_43AFE0(door))
                    {
                        goto found;
                    }
                    Car_Door_10* pDoor = pTargetToEnter->GetDoor(this->field_24C_target_car_door);
                    pDoor->Close_439EA0();
                    pDoor->set_ped_421380(0);
                    if (this->field_27C_ped_state_2 == ped_state_2::ped2_entering_a_car_6)
                    {
                        return 0;
                    }
                }

                for (door = this->field_24C_target_car_door; door != 0xFF; door--)
                {
                    if (pTargetToEnter->IsDoorAccessible_43AFE0(door))
                    {
                        goto found;
                    }
                }
            }
        }
    }
    return 0;
}

MATCH_FUNC(0x467280)
Sprite* Ped::sub_467280()
{
    this->field_168_game_object->field_8_ped_state_1 = 0;
    this->field_168_game_object->field_C_ped_state_2 = 0;
    this->field_168_game_object->field_10_char_state = 1;

    Char_B4* pB4 = this->field_168_game_object;
    pB4->field_6C_animation_state = 0;
    pB4->field_68_animation_frame = 0;

    this->field_216_health = 50;
    return gPurpleDoom_1_679208->FindNearestSpriteOfType_477E60(this->field_168_game_object->field_80_sprite_ptr, 2);
}

// https://decomp.me/scratch/ec0hn
WIP_FUNC(0x4672e0)
void Ped::UpdateMovementTowardsTarget_4672E0(Fix16 distance, u8 type)
{
    WIP_IMPLEMENTED;
    Ang16 angle;
    Fix16 x;
    Fix16 y;
    Fix16 z;
    u8 bUnk2 = false;
    u8 bUnk1 = true;
    field_21C_bf.b17 = false;

    switch (type)
    {
        case 0:
            x = field_14C_internal_target_ped->get_cam_x();
            y = field_14C_internal_target_ped->get_cam_y();
            z = field_14C_internal_target_ped->get_cam_z();
            break;
        case 1:
            x = field_1D0_internal_target_x;
            y = field_1D4_internal_target_y;
            z = field_1D8_internal_target_z;
            break;
        case 2:
            x = field_1C4_x;
            y = field_1C8_y;
            z = field_154_target_to_enter->field_50_car_sprite->field_1C_zpos;
            break;
        case 3:
            x = field_148_objective_target_ped->get_cam_x();
            y = field_148_objective_target_ped->get_cam_y();
            z = field_148_objective_target_ped->get_cam_z();
            break;
        case 4:
            x = field_1DC_objective_target_x;
            y = field_1E0_objective_target_y;
            z = field_1E4_objective_target_z;
            break;
        case 5:
            x = field_150_target_objective_car->field_50_car_sprite->field_14_xy.x;
            y = field_150_target_objective_car->field_50_car_sprite->field_14_xy.y;
            z = field_150_target_objective_car->field_50_car_sprite->field_1C_zpos;
            break;
        case 6:
            x = field_1A4_internal_target_object->get_x_4340D0();
            y = field_1A4_internal_target_object->get_y_4340E0();
            z = field_1A4_internal_target_object->get_z_4340F0();
            break;
        case 7:
            x = field_1A0_objective_target_object->get_x_4340D0();
            y = field_1A0_objective_target_object->get_y_4340E0();
            z = field_1A0_objective_target_object->get_z_4340F0();
            break;
        default:
            break;
    }
    angle = Fix16::atan2_fixed_405320(y - field_1AC_cam.y, x - field_1AC_cam.x);
    field_21C_bf.b15 = true;
    if (field_21C_bf.b15 == true)
    {
        if (distance < kFpTwo_678658)
        {
            if (distance < kFpOne_678798 &&
                (field_1AC_cam.z == z ||
                 (Fix16::Abs(field_1AC_cam.z - z) <= kFpHalf_67853C && field_1AC_cam.x.ToUInt8() == x.ToUInt8() &&
                  field_1AC_cam.y.ToUInt8() == y.ToUInt8())))
            {
                bUnk1 = false;
                field_266 = 0;
            }
            else
            {
                bUnk1 = true;
            }
        }

        if (field_168_game_object->field_69_is_colliding_with_sprite && field_18C_current_path_point != NULL &&
            field_1AC_cam.x.ToUInt8() == field_18C_current_path_point->field_0_x && field_1AC_cam.y.ToUInt8() == field_18C_current_path_point->field_1_y)
        {
            field_18C_current_path_point = field_18C_current_path_point + 1;
            bUnk2 = true;
            field_1C4_x = kFpHalf_67853C + Fix16(field_18C_current_path_point->field_0_x);
            field_1C8_y = kFpHalf_67853C + Fix16(field_18C_current_path_point->field_1_y);
        }
        else if (!bUnk1)
        {
            Ped::ChangeNextPedState1_45C500(1);
            Ped::ChangeNextPedState2_45C540(2);
            field_21C_bf.b15 = false;
            field_21C_bf.b16 = false;
            return;
        }
        // line 22d
        byte_678554 = 1;
        Ped::ChangeNextPedState1_45C500(2);
        Ped::ChangeNextPedState2_45C540(0);
        if (!field_21C_bf.b14)
        {
            gOrca_2FD4_6FDEF0->field_3C_ped_list.AddPedToBackIfMissing_471160(this);
            field_21C_bf.b14 = true;
        }

        switch (gOrca_2FD4_6FDEF0->IsFirstPassenger_554A90(this))
        {
            case 1:
                if (!gOrca_2FD4_6FDEF0->ComputePath_554AB0(field_200_id,
                                                           this,
                                                           field_1AC_cam.x.ToUInt8(),
                                                           field_1AC_cam.y.ToUInt8(),
                                                           field_1AC_cam.z.ToUInt8(),
                                                           x.ToInt(),
                                                           y.ToInt(),
                                                           z.ToInt(),
                                                           Ang16::GetAngleFace_4F78F0(angle),
                                                           &this->field_266))
                {
                    goto LINE_38E;
                }

                gOrca_2FD4_6FDEF0->field_3C_ped_list.RemovePed_4711F0(this);
                field_18C_current_path_point = &field_0_patrol_points[0];
                field_21C_bf.b14 = false;

                while (1)
                {
                    if (field_18C_current_path_point->field_0_x == 0)
                    {
                        field_18C_current_path_point = &field_0_patrol_points[0];
                        break;
                    }
                    if (field_18C_current_path_point->field_0_x == field_1AC_cam.x.ToUInt8() &&
                        field_18C_current_path_point->field_1_y == field_1AC_cam.y.ToUInt8() &&
                        field_18C_current_path_point->field_2_z == field_1AC_cam.z.ToUInt8())
                    {
                        break;
                    }
                    field_18C_current_path_point++;
                }
                // fall through

            default:
                // line 361
                if (field_18C_current_path_point->field_0_x != 0 || field_18C_current_path_point->field_1_y != 0)
                {
                    goto LINE_54E;
                }
                field_21C_bf.b17 = true;
                goto LINE_3A9;

            case 0:
            LINE_38E:
                if (field_18C_current_path_point)
                {
                    if (field_18C_current_path_point->field_0_x == 0 && field_18C_current_path_point->field_1_y == 0)
                    {
                    LINE_3A9:
                        Ped::ChangeNextPedState1_45C500(1);
                        Ped::ChangeNextPedState2_45C540(2);
                        field_21C_bf.b15 = false;
                        field_0_patrol_points[0].field_0_x = 0;
                        field_0_patrol_points[0].field_1_y = 0;
                    }
                    else
                    {
                        field_1C4_x = Fix16(field_18C_current_path_point->field_0_x);
                        field_1C8_y = Fix16(field_18C_current_path_point->field_1_y);

                        Fix16 dist_1 = (kFpHalf_67853C + Fix16(field_1C4_x.ToUInt8())) - field_1AC_cam.x;
                        Fix16 dist_2 = (kFpHalf_67853C + Fix16(field_1C8_y.ToUInt8())) - field_1AC_cam.y;

                        Fix16* pGreater_abs = &dist_1;
                        if (Fix16::Abs(dist_1) <= Fix16::Abs(dist_2))
                        {
                            pGreater_abs = &dist_2;
                        }

                        if (*pGreater_abs < kFpHalf_678790 || ((field_168_game_object->field_58_flags & 0x40) != 0))
                        {
                            field_18C_current_path_point++;
                            field_1C4_x = kFpHalf_67853C + Fix16(field_18C_current_path_point->field_0_x);
                            field_1C8_y = kFpHalf_67853C + Fix16(field_18C_current_path_point->field_1_y);
                            field_1CC_z = Fix16(field_18C_current_path_point->field_2_z);
                        }
                        else
                        {
                            field_1C4_x = kFpHalf_67853C + Fix16(field_18C_current_path_point->field_0_x);
                            field_1C8_y = kFpHalf_67853C + Fix16(field_18C_current_path_point->field_1_y);
                            field_1CC_z = Fix16(field_18C_current_path_point->field_2_z);
                        }
                        goto LINE_3D9;
                    }
                }
                else
                {
                    field_0_patrol_points[0].field_0_x = 0;
                    field_0_patrol_points[0].field_1_y = 0;
                    Ped::ChangeNextPedState1_45C500(1);
                    Ped::ChangeNextPedState2_45C540(2);
                    field_21C_bf.b15 = false;
                }

                if (bUnk2)
                {
                LINE_3D9:
                    if (type >= 3 && (type <= 5 || type == 7))
                    {
                        field_1B8_target_x = field_1C4_x;
                        field_1BC_target_y = field_1C8_y;
                        field_1C0_target_z = field_1CC_z;
                    }
                }
                break;

            // Line 361's else, placed at the end in the original
            LINE_54E:
                field_1C4_x = kFpHalf_67853C + Fix16(field_18C_current_path_point->field_0_x);
                field_1C8_y = kFpHalf_67853C + Fix16(field_18C_current_path_point->field_1_y);
                field_1CC_z = Fix16(field_18C_current_path_point->field_2_z);
                field_21C_bf.b16 = true;
                byte_61A8A1 = 0;
                goto LINE_3D9;
        }
    }
    else
    {
        // 5a5
        Ped::ChangeNextPedState1_45C500(1);
        Ped::ChangeNextPedState2_45C540(2);
        field_21C_bf.b15 = false;
        field_21C_bf.b16 = false;
    }
}

MATCH_FUNC(0x4678e0)
void Ped::FleeOnFootTillSafe_4678E0()
{
    if (byte_61A8A3)
    {
        if (gDistanceToTarget_678750 > kFpSix_678520) // far away from the threat or place?
        {
            if (field_168_game_object)
            {
                if (field_168_game_object->field_44_block_type == 2)
                {
                    // back to normality
                    Ped::ChangeNextPedState1_45C500(ped_state_1::walking_0);
                    Ped::ChangeNextPedState2_45C540(ped_state_2::ped2_walking_0);
                    field_225_objective_status = objective_status::passed_1;
                }
                else
                {
                    // RUN
                    field_168_game_object->field_38_velocity = get_max_speed_1F0();
                }
            }
            else
            {
                field_21C_bf.b11 = 0;
            }
        }
        else
        {
            // It's very close to the threat, so run!
            field_168_game_object->field_38_velocity = get_max_speed_1F0();
        }
    }
}

MATCH_FUNC(0x467960)
void Ped::FleeCharOnFootTillSafe_467960()
{
    if (field_148_objective_target_ped->field_278_ped_state_1 == ped_state_1::dead_9 ||
        (field_148_objective_target_ped->field_21C & 1) == 0)
    {
        Ped::ChangeNextPedState1_45C500(ped_state_1::walking_0);
        Ped::ChangeNextPedState2_45C540(ped_state_2::ped2_walking_0);
        this->field_148_objective_target_ped = 0;
        this->field_225_objective_status = objective_status::passed_1;
        return;
    }

    if (byte_61A8A3)
    {
        if (gDistanceToTarget_678750 > kFpSix_678520)
        {
            if (this->field_168_game_object->field_44_block_type == 2)
            {
                Ped::ChangeNextPedState1_45C500(ped_state_1::walking_0);
                Ped::ChangeNextPedState2_45C540(ped_state_2::ped2_walking_0);
                this->field_148_objective_target_ped = 0;
                this->field_225_objective_status = objective_status::passed_1;
            }
        }
        else
        {
            Ped::ChangeNextPedState1_45C500(ped_state_1::flee_or_running_1);
            Ped::ChangeNextPedState2_45C540(ped_state_2::Unknown_3);
            this->field_168_game_object->field_38_velocity = this->field_168_game_object->field_3C_run_or_jump_speed;
            field_21C_bf.b11 = 0;
        }
    }
}

MATCH_FUNC(0x467a20)
void Ped::FleeFromCharOnFootAlways_467A20()
{
    if (field_148_objective_target_ped->field_278_ped_state_1 == ped_state_1::dead_9 ||
        field_148_objective_target_ped->field_21C_bf.b0 == false)
    {
        // Only back to normality if the menacing ped is died or (maybe) cannot move
        Ped::ChangeNextPedState1_45C500(ped_state_1::walking_0);
        Ped::ChangeNextPedState2_45C540(ped_state_2::ped2_walking_0);
        field_148_objective_target_ped = 0;
        field_225_objective_status = objective_status::passed_1;
    }
    else if (byte_61A8A3)
    {
        // Run
        field_168_game_object->field_38_velocity = field_168_game_object->field_3C_run_or_jump_speed;
        field_21C_bf.b11 = 0;
    }
}

MATCH_FUNC(0x467ad0)
void Ped::FleeCharAlwaysOnceCarStopped_467AD0()
{
    if (field_16C_car)
    {
        if (field_16C_car->GetVelocity_43A4C0() == kFpZero_678660 && field_25C_internal_objective != 36)
        {
            Ped::SetObjective2_463830(36, 9999);
            field_154_target_to_enter = field_16C_car;
        }
    }
    else if (byte_61A8A3)
    {
        Ped* pOldTarget = field_148_objective_target_ped;
        Ped::SetObjective(objectives_enum::flee_char_on_foot_always_3, 9999);
        field_148_objective_target_ped = pOldTarget;
    }
}

MATCH_FUNC(0x467bd0)
void Ped::sub_467BD0()
{
    if (field_16C_car)
    {
        if (field_16C_car->GetVelocity_43A4C0() == kFpZero_678660 && field_25C_internal_objective != 36)
        {
            Ped::SetObjective2_463830(36, 9999);
            field_154_target_to_enter = field_16C_car;
        }
    }
    else if (byte_61A8A3)
    {
        field_154_target_to_enter = field_150_target_objective_car;
        Ped::SetObjective(objectives_enum::flee_on_foot_till_safe_1, 9999);
        field_1B8_target_x = field_154_target_to_enter->field_50_car_sprite->field_14_xy.x;
        field_1BC_target_y = field_154_target_to_enter->field_50_car_sprite->field_14_xy.y;
        field_1C0_target_z = field_168_game_object->field_80_sprite_ptr->field_1C_zpos;
    }
}

MATCH_FUNC(0x467ca0)
void Ped::KillCharOnFoot_467CA0()
{
    if (!field_148_objective_target_ped->CheckBit0_433B40() || field_148_objective_target_ped->GetPedState_403990() == ped_state_1::dead_9)
    {
        if (field_148_objective_target_ped->GetPedState_403990() == ped_state_1::dead_9)
        {
            field_225_objective_status = objective_status::passed_1;
        }
        else
        {
            field_225_objective_status = objective_status::failed_2;
        }
    }
    else
    {
        if (field_140_stolen_car)
        {
            if (field_140_stolen_car->field_88_despawn_status == 5)
            {
                field_140_stolen_car = 0;
            }
            else
            {
                field_140_stolen_car->field_76_last_seen_timer = 0;
            }
        }

        if (field_148_objective_target_ped->sub_433DA0())
        {
            if (field_25C_internal_objective == objectives_enum::objective_17)
            {
                return;
            }
            Ped::SetObjective2_463830(objectives_enum::no_obj_0, 9999);
            return;
        }

        if (!byte_61A8A3 || field_21C_bf.b2)
        {
            return;
        }

        switch (field_25C_internal_objective)
        {
            case objectives_enum::kill_char_on_foot_20:
                if (field_226_internal_objective_status == 1)
                {
                    if (field_14C_internal_target_ped != field_148_objective_target_ped)
                    {
                        Ped::SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                        return;
                    }
                    field_225_objective_status = objective_status::passed_1;
                    return;
                }
                if (field_226_internal_objective_status == 2)
                {
                    Ped::SetObjective2_463830(objectives_enum::kill_char_on_foot_20, 9999);
                    field_14C_internal_target_ped = field_148_objective_target_ped;
                    field_21C_bf.b2 = 0;
                }
                return;

            case objectives_enum::flee_on_foot_till_safe_1:
                if (field_226_internal_objective_status == 1)
                {
                    Ped::SetObjective2_463830(objectives_enum::kill_char_on_foot_20, 9999);
                    field_14C_internal_target_ped = field_148_objective_target_ped;
                }
                return;

            case objectives_enum::no_obj_0:
                Ped::SetObjective2_463830(objectives_enum::kill_char_on_foot_20, 9999);
                field_14C_internal_target_ped = field_148_objective_target_ped;
                return;
        }
    }
}

MATCH_FUNC(0x467e20)
void Ped::KillCharAnyMeans_467E20()
{
    if (!field_148_objective_target_ped->CheckBit0_433B40() || field_148_objective_target_ped->GetPedState_403990() == ped_state_1::dead_9)
    {
        if (field_148_objective_target_ped->GetPedState_403990() == ped_state_1::dead_9)
        {
            // Target ped was killed/died
            field_225_objective_status = objective_status::passed_1;
            Ped::SetObjective2_463830(objectives_enum::no_obj_0, 9999);
            if (field_16C_car)
            {
                // If the assassin is on a car, cancel routes
                field_278_ped_state_1 = ped_state_1::in_car_10;
                field_27C_ped_state_2 = ped_state_2::ped2_driving_10;
                if (field_16C_car->field_5C_AI)
                {
                    char_type junc_idx = field_16C_car->field_5C_AI->field_28_junc_idx;
                    if (junc_idx > 0)
                    {
                        gRouteFinder_6FFDC8->CancelRoute_589930(junc_idx);
                    }
                }
                if (field_16C_car->field_60)
                {
                    gHamburger_500_678E30->FreeEntry_474CC0(field_16C_car->field_60);
                    field_16C_car->field_60 = 0;
                }
            }
        }
        else
        {
            // Target ped is alive
            field_225_objective_status = objective_status::failed_2;
        }
    }
    else
    {
        if (field_148_objective_target_ped->sub_433DA0()) // Target is on foot?
        {
            Ped::SetObjective2_463830(objectives_enum::no_obj_0, 9999);
            if (field_16C_car)
            {
                field_278_ped_state_1 = ped_state_1::in_car_10;
                field_27C_ped_state_2 = ped_state_2::ped2_driving_10;
                if (field_16C_car->field_5C_AI)
                {
                    char_type junc_idx = field_16C_car->field_5C_AI->field_28_junc_idx;
                    if (junc_idx > 0)
                    {
                        gRouteFinder_6FFDC8->CancelRoute_589930(junc_idx);
                    }
                }
                if (field_16C_car->field_60)
                {
                    gHamburger_500_678E30->FreeEntry_474CC0(field_16C_car->field_60);
                    field_16C_car->field_60 = 0;
                }
            }
        }
        else if (field_168_game_object)
        {
            if (gDistanceToTarget_678750 > kFpFour_678680)
            {
                field_1DC_objective_target_x = field_148_objective_target_ped->get_cam_x();
                field_1E0_objective_target_y = field_148_objective_target_ped->get_cam_y();
                field_1E4_objective_target_z = field_148_objective_target_ped->get_cam_z();
                Ped::GotoAreaByAnyMeans_469060();
            }
            else if (field_25C_internal_objective != 20)
            {
                Ped::SetObjective2_463830(20, 9999);
                field_14C_internal_target_ped = field_148_objective_target_ped;
            }
        }
        else
        {
            Ped::GotoAreaByAnyMeans_469060();
        }
    }
}

MATCH_FUNC(0x467fb0)
void Ped::KillFrenzy_467FB0()
{
    if (byte_61A8A3)
    {
        if (!field_218_objective_timer)
        {
            field_225_objective_status = objective_status::passed_1;
        }
    }
}

MATCH_FUNC(0x467fd0)
void Ped::PunchChar_467FD0()
{
    if (!field_148_objective_target_ped->CheckBit0_433B40() ||
        field_148_objective_target_ped->GetPedState_403990() == ped_state_1::dead_9)
    {
        this->field_225_objective_status = objective_status::failed_2;
        return;
    }

    if (byte_61A8A3)
    {
        if (field_25C_internal_objective != 0)
        {
            if (field_25C_internal_objective == 23)
            {
                if (field_226_internal_objective_status == 1)
                {
                    this->field_225_objective_status = objective_status::passed_1;
                    return;
                }
                else if (field_226_internal_objective_status == 2)
                {
                    this->field_225_objective_status = objective_status::failed_2;
                }
            }
        }
        else
        {
            Ped::SetObjective2_463830(23, 9999);
            this->field_14C_internal_target_ped = this->field_148_objective_target_ped;
            return;
        }
    }
}

MATCH_FUNC(0x468040)
void Ped::ProcessAirborneMovement_468040()
{
    u8 bUnknown = 1;
    if (this->field_240_occupation == ped_ocupation_enum::drone)
    {
        gDistanceToTarget_678750 = kFpZero_678660;
        this->field_1E4_objective_target_z = this->field_1AC_cam.z;
    }

    if (!byte_61A8A3)
    {
        bUnknown = this->field_168_game_object->GetCharState_433A80() == 15 && (this->field_21C & 4) == 0;
    }

    if ((field_224 & 0x10) != 0 && (this->field_21C & 4) != 0)
    {
        this->field_260 = 0;
        this->field_224 &= ~0x10;
    }

    if (bUnknown)
    {
        if ((this->field_224 & 0x10) != 0 ||
            gDistanceToTarget_678750 <= kFpThreeSixteenths_678780 &&
                abs_sub_less_than_epislon_45AE40(this->field_1AC_cam.z, this->field_1E4_objective_target_z))
        {
            if (field_168_game_object->GetCharState_433A80() != 15)
            {
                if ((this->field_224 & 0x10) != 0)
                {
                    field_168_game_object->RegulateVelocityByRef_433970(kFpZero_678438);
                }

                ChangeNextPedState1_45C500(7);
                ChangeNextPedState2_45C540(14);
            }
            else
            {
                this->field_224 |= 0x10u;
            }
        }
        else
        {
            if (field_168_game_object->GetCharState_433A80() != 15)
            {
                if (gDistanceToTarget_678750 > kFpThreeEighths_67878C)
                {
                    field_168_game_object->SetMaxSpeed_433920(this->field_1F0_maybe_max_speed);
                    UpdateMovementTowardsTarget_4672E0(gDistanceToTarget_678750, 4);
                }
                else
                {
                    field_168_game_object->SetMaxSpeed_433920(this->field_1F4);
                    UpdateMovementTowardsTarget_4672E0(gDistanceToTarget_678750, 4);
                }
            }
        }
    }

    this->field_130 = this->field_134_rotation;

    if (field_218_objective_timer == 0)
    {
        this->field_225_objective_status = 1;
    }
}

MATCH_FUNC(0x4682a0)
void Ped::TimeWaitedInCar_4682A0()
{
    if (this->field_16C_car == 0)
    {
        this->field_218_objective_timer = 9999;
    }
    else
    {
        ++this->field_218_objective_timer;
        if (this->field_218_objective_timer > 9999u)
        {
            this->field_218_objective_timer = 9999;
        }
    }
}

MATCH_FUNC(0x468310)
void Ped::GotoAreaInCar_468310()
{
    Ped* pDriver;
    Car_BC* pCar;
    Car_BC* pCar_;
    Car_BC* pCar__;

    if (this->field_225_objective_status != objective_status::passed_1)
    {
        if (this->field_168_game_object)
        {
            this->field_225_objective_status = objective_status::failed_2;
        }
        else
        {
            if (!this->field_16C_car->field_60)
            {
                this->field_16C_car->field_60 = gHamburger_500_678E30->AllocateEntry_474810();
                this->field_16C_car->field_60->field_4_ped_owner = this;
            }

            this->field_16C_car->field_60->field_8_maybe_path_type = 1;
            this->field_16C_car->field_60->field_22 = 1;
            this->field_16C_car->field_60->field_20 = 0;
            this->field_16C_car->field_60->field_14_target_x = this->field_1DC_objective_target_x;
            this->field_16C_car->field_60->field_18_target_y = this->field_1E0_objective_target_y;
            this->field_16C_car->field_60->field_1C_target_z = this->field_1E4_objective_target_z;
            this->field_16C_car->ClearA6Bit5_421550();

            pDriver = this->field_16C_car->field_54_driver;
            if (pDriver)
            {
                if (pDriver->IsField238_45EDE0(4) || this->field_16C_car->field_54_driver->IsField238_45EDE0(6))
                {
                    pCar = this->field_16C_car;
                    if (pCar->field_54_driver->field_26C_graphic_type == 2)
                    {
                        pCar->field_60->field_20 = 1;
                        this->field_16C_car->field_60->field_22 = 1;
                    }
                }
            }

            if ((u8)(this->field_1AC_cam.x.ToInt()) == (u8)(this->field_1DC_objective_target_x.ToInt()) &&
                (u8)(this->field_1AC_cam.y.ToInt()) == (u8)(this->field_1E0_objective_target_y.ToInt()) &&
                this->field_1AC_cam.z == this->field_1E4_objective_target_z)
            {
                pCar_ = this->field_16C_car;
                this->field_225_objective_status = objective_status::passed_1;
                gHamburger_500_678E30->FreeEntry_474CC0(pCar_->field_60);
                this->field_16C_car->field_60 = 0;
                this->field_16C_car->SetA6Bit5_421540();
                this->field_1A0_objective_target_object = dword_678558; // TODO: Never written so part of a bigger global obj?
            }
            else
            {
                pCar__ = this->field_16C_car;
                if (pCar__->field_60->field_26)
                {
                    this->field_225_objective_status = objective_status::passed_1;
                    gHamburger_500_678E30->FreeEntry_474CC0(pCar__->field_60);
                    this->field_16C_car->field_60 = 0;
                    this->field_16C_car->SetA6Bit5_421540();
                }
                else if (!pCar__)
                {
                    this->field_218_objective_timer = 9999;
                }
                else
                {
                    if (pCar__->GetVelocity_43A4C0() == kFpZero_678660)
                    {
                        ++this->field_218_objective_timer;
                    }
                    else
                    {
                        this->field_218_objective_timer = 0;
                    }
                    if (this->field_218_objective_timer > 9999u)
                    {
                        this->field_218_objective_timer = 9999;
                    }
                }
            }
        }
    }
}

MATCH_FUNC(0x4686c0)
void Ped::EnterTargetObjectiveCar_4686C0()
{
    if (field_168_game_object)
    {
        field_168_game_object->Set_F84_433900(field_150_target_objective_car);
    }

    if (field_25C_internal_objective)
    {
        if (field_25C_internal_objective == 35)
        {
            if (field_226_internal_objective_status == 1)
            {
                if (!field_16C_car)
                {
                    Car_BC* pOldCar = field_150_target_objective_car;
                    Ped::SetObjective(objectives_enum::enter_car_as_driver_35, 9999);
                    Ped::SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                    field_150_target_objective_car = pOldCar;
                }
                else
                {
                    field_225_objective_status = objective_status::passed_1;
                    Ped::SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                    Ped::ChangeNextPedState1_45C500(ped_state_1::in_car_10);
                    Ped::ChangeNextPedState2_45C540(ped_state_2::ped2_driving_10);
                }
                return;
            }
            if (field_226_internal_objective_status == 2)
            {
                field_225_objective_status = objective_status::failed_2;
            }
        }
    }
    else
    {
        Ped::SetObjective2_463830(35, 9999);
        field_154_target_to_enter = field_150_target_objective_car;
    }
    if (field_150_target_objective_car->IsDespawning_4215B0() || field_150_target_objective_car->IsMaxDamage_40F890())
    {
        field_225_objective_status = objective_status::failed_2;
        Ped::SetObjective2_463830(objectives_enum::no_obj_0, 9999);
        field_21C_bf.b2 = false;
    }
}

MATCH_FUNC(0x468820)
void Ped::LeaveTargetObjectiveCar_468820()
{
    if (field_168_game_object)
    {
        if (!field_150_target_objective_car && field_27C_ped_state_2 == ped_state_2::Unknown_17)
        {
            field_225_objective_status = objective_status::passed_1;
        }
    }
    if ((field_21C & 4) != 0)
    {
        if (field_168_game_object)
        {
            if (field_164_ped_group)
            {
                if (field_164_ped_group->AreAllMembersOnFoot_4CAB80())
                {
                    field_225_objective_status = objective_status::passed_1;
                }
                else
                {
                    field_225_objective_status = objective_status::not_finished_0;
                }
            }
            else
            {
                field_225_objective_status = objective_status::passed_1;
            }
        }
    }

    if (field_25C_internal_objective)
    {
        if (field_25C_internal_objective == 36)
        {
            if (field_226_internal_objective_status == 1)
            {
                if (field_238_ped_type != ped_type::player_2)
                {
                    field_225_objective_status = objective_status::passed_1;
                }
                else
                {
                    Ped::SetObjective(objectives_enum::no_obj_0, 9999);
                }

                Ped::SetObjective2_463830(objectives_enum::no_obj_0, 9999);
            }
            else if (field_226_internal_objective_status == 2)
            {
                field_225_objective_status = objective_status::failed_2;
            }
        }
    }
    else if (field_16C_car)
    {
        Ped::SetObjective2_463830(36, 9999);
        field_154_target_to_enter = field_150_target_objective_car;
    }
    else
    {
        if (field_164_ped_group)
        {
            if (field_164_ped_group->AreAllMembersOnFoot_4CAB80())
            {
                field_225_objective_status = objective_status::passed_1;
            }
            else
            {
                field_225_objective_status = objective_status::not_finished_0;
            }
        }
        else
        {
            field_225_objective_status = objective_status::passed_1;
        }
    }
}

MATCH_FUNC(0x468930)
void Ped::EnterTrain_468930()
{
    if (field_226_internal_objective_status == 2 || field_150_target_objective_car == 0)
    {
        field_225_objective_status = objective_status::failed_2;
    }
    else
    {
        if (field_168_game_object)
        {
            field_168_game_object->Set_F84_433900(field_150_target_objective_car);
        }

        if (field_25C_internal_objective)
        {
            if (field_25C_internal_objective == 37)
            {
                if (field_226_internal_objective_status == 1)
                {
                    field_225_objective_status = objective_status::passed_1;
                    Ped::SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                    Ped::ChangeNextPedState1_45C500(ped_state_1::in_car_10);
                    Ped::ChangeNextPedState2_45C540(ped_state_2::ped2_driving_10);
                    return;
                }
                if (field_226_internal_objective_status == 2)
                {
                    field_225_objective_status = objective_status::failed_2;
                }
            }
        }
        else
        {
            Ped::SetObjective2_463830(37, 9999);
            field_154_target_to_enter = field_150_target_objective_car;
        }
        if (field_150_target_objective_car->IsDespawning_4215B0() || field_150_target_objective_car->IsMaxDamage_40F890())
        {
            field_225_objective_status = objective_status::failed_2;
            Ped::SetObjective2_463830(objectives_enum::no_obj_0, 9999);
        }
    }
}

MATCH_FUNC(0x468a00)
void Ped::LeaveTrain_468A00()
{
    if (field_25C_internal_objective)
    {
        if (field_25C_internal_objective != 12)
        {
            if (field_25C_internal_objective == 38)
            {
                if (field_226_internal_objective_status == 1)
                {
                    if (field_150_target_objective_car->IsTrainModel_403BA0())
                    {
                        if (field_238_ped_type != ped_type::player_2)
                        {
                            Ped::SetObjective2_463830(12, 9999);
                            switch (Ang16::GetAngleFace_4F78F0(field_12C))
                            {
                                case 1:
                                    field_1D0_internal_target_x = field_1AC_cam.x;
                                    field_1D4_internal_target_y = field_1AC_cam.y - kFpOne_678664;
                                    break;
                                case 3:
                                    field_1D0_internal_target_x = kFpOne_678664 + field_1AC_cam.x;
                                    field_1D4_internal_target_y = field_1AC_cam.y;
                                    break;
                                case 2:
                                    field_1D0_internal_target_x = field_1AC_cam.x;
                                    field_1D4_internal_target_y = kFpOne_678664 + field_1AC_cam.y;
                                    break;
                                case 4:
                                    field_1D0_internal_target_x = field_1AC_cam.x - kFpOne_678664;
                                    field_1D4_internal_target_y = field_1AC_cam.y;
                                    break;
                                default:
                                    break;
                            }
                            field_1D8_internal_target_z = field_1AC_cam.z;
                        }
                        else
                        {
                            Ped::SetObjective(objectives_enum::no_obj_0, 9999);
                            Ped::SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                        }
                    }
                    else
                    {
                        field_225_objective_status = objective_status::passed_1;
                    }
                }
                if (field_226_internal_objective_status == 2)
                {
                    field_225_objective_status = objective_status::failed_2;
                }
            }
        }
        else
        {
            if (field_278_ped_state_1 != ped_state_1::immobilized_8 && field_226_internal_objective_status == 1)
            {
                field_225_objective_status = objective_status::passed_1;
                Ped::SetObjective2_463830(objectives_enum::no_obj_0, 9999);
            }
        }
    }
    else if (field_16C_car)
    {
        Ped::SetObjective2_463830(38, 9999);
        field_154_target_to_enter = field_150_target_objective_car;
    }
    else
    {
        field_225_objective_status = objective_status::passed_1;
    }
}

MATCH_FUNC(0x468bd0)
void Ped::sub_468BD0()
{
    if (field_25C_internal_objective)
    {
        if (field_25C_internal_objective == 36 && !field_16C_car)
        {
            Set_B4_F16_To_1_433B50();
            field_278_ped_state_1 = ped_state_1::immobilized_8;
            field_27C_ped_state_2 = ped_state_2::Unknown_17;
            field_168_game_object->Set_F8_ped_state_1_433910(8);
            field_168_game_object->SetPedState2_433A50(17);
            field_16C_car = 0;
            Ped::SetObjective(objectives_enum::no_obj_0, 9999);
            Ped::SetObjective2_463830(objectives_enum::no_obj_0, 9999);
        }
    }
    else
    {
        Ped::SetObjective2_463830(36, 9999);
        set_target_to_enter_403B00(field_150_target_objective_car);
    }
}

MATCH_FUNC(0x468c70)
void Ped::PatrolOnFoot_468C70()
{
    if (byte_61A8A3)
    {
        if (field_25C_internal_objective == 12)
        {
            if (field_21C_bf.b2 == false)
            {
                if (field_226_internal_objective_status)
                {
                    field_194_current_patrol_point = field_194_current_patrol_point + 1;
                    if (!field_194_current_patrol_point->field_0_x)
                    {
                        field_194_current_patrol_point = field_190_patrol_route->field_0_points;
                    }
                    Ped::SetObjective2_463830(12, 9999);
                    field_1D0_internal_target_x = kFpHalf_67853C + Fix16(field_194_current_patrol_point->field_0_x);
                    field_1D4_internal_target_y = kFpHalf_67853C + Fix16(field_194_current_patrol_point->field_1_y);
                    field_1D8_internal_target_z = Fix16(field_194_current_patrol_point->field_2_z);
                }
                field_168_game_object->RegulateVelocity_433970(field_1F4);
            }
        }
        else if (field_21C_bf.b2 == false)
        {
            field_194_current_patrol_point = field_190_patrol_route->field_0_points;
            Ped::SetObjective2_463830(12, 9999);
            field_1D0_internal_target_x = kFpHalf_67853C + Fix16(field_194_current_patrol_point->field_0_x);
            field_1D4_internal_target_y = kFpHalf_67853C + Fix16(field_194_current_patrol_point->field_1_y);
            field_1D8_internal_target_z = Fix16(field_194_current_patrol_point->field_2_z);
            field_168_game_object->RegulateVelocity_433970(field_1F4);
        }
    }
}

MATCH_FUNC(0x468de0)
void Ped::GotoAreaOnFoot_468DE0()
{
    if (field_240_occupation == ped_ocupation_enum::drone)
    {
        field_1F0_maybe_max_speed = kFpOneThirtySecond_678434;
    }
    if (byte_61A8A3)
    {
        if (gDistanceToTarget_678750 < kFpQuarter_678788)
        {
            if (field_168_game_object->GetCharState_433A80() != Char_B4_state::Jumping_15)
            {
                // ped reached its destination
                Ped::ChangeNextPedState1_45C500(ped_state_1::standing_still_7);
                Ped::ChangeNextPedState2_45C540(ped_state_2::ped2_staying_14);
                field_225_objective_status = objective_status::passed_1;
            }
        }
        else
        {
            if (field_218_objective_timer == 0)
            {
                // Time out. Objective passed anyway O.o, must be failed? OG bug?
                field_225_objective_status = objective_status::passed_1;
            }
            field_168_game_object->SetMaxSpeed_433920(field_1F0_maybe_max_speed);
        }
        Ped::UpdateMovementTowardsTarget_4672E0(gDistanceToTarget_678750, 4);
    }
}

MATCH_FUNC(0x468e80)
void Ped::UpdateFollowPedObjective_468E80()
{
    u8 bUnknown1 = 0;
    u8 bUnknown2 = 1;

    Ped* objective_target_ped = this->field_148_objective_target_ped;
    if (objective_target_ped->GetPedState_403990() == ped_state_1::dead_9 &&
        objective_target_ped->get_objective_403A80() != objectives_enum::objective_28)
    {
        bUnknown1 = 1;
    }

    if (!objective_target_ped->CheckBit0_433B40() || bUnknown1)
    {
        this->field_225_objective_status = 2;
    }
    else
    {
        if (!byte_61A8A3)
        {
            bUnknown2 = this->field_168_game_object->GetCharState_433A80() == 15 && (this->field_21C & 4) == 0;
        }

        if ((field_224 & 0x10) != 0 && ((this->field_21C & 4) != 0 || this->field_260 > 0xC8u))
        {
            this->field_260 = 0;
            this->field_224 &= ~0x10;
        }

        if (bUnknown2)
        {
            if ((this->field_224 & 0x10) != 0 ||
                gDistanceToTarget_678750 <= kFpThreeSixteenths_678780 &&
                    abs_sub_less_than_epislon_45AE40(this->field_1AC_cam.z, objective_target_ped->get_cam_z()))
            {
                if (field_168_game_object->GetCharState_433A80() != 15)
                {
                    if ((this->field_224 & 0x10) != 0)
                    {
                        field_168_game_object->RegulateVelocityByRef_433970(kFpZero_678438);
                        ++this->field_260;
                    }
                    Ped::ChangeNextPedState1_45C500(7);
                    Ped::ChangeNextPedState2_45C540(14);
                    this->field_225_objective_status = 1;
                }
                else
                {
                    this->field_224 |= 0x10u;
                }
            }
            else
            {
                if (field_168_game_object->GetCharState_433A80() != 15)
                {
                    if (!this->field_218_objective_timer)
                    {
                        this->field_225_objective_status = 1;
                    }

                    if (gDistanceToTarget_678750 < kFpHalf_678790)
                    {
                        field_168_game_object->SetMaxSpeed_433920(this->field_1F4);
                    }
                    else
                    {
                        field_168_game_object->SetMaxSpeed_433920(this->field_1F0_maybe_max_speed);
                    }
                    Ped::UpdateMovementTowardsTarget_4672E0(gDistanceToTarget_678750, 3);
                }
            }
        }
    }
}

MATCH_FUNC(0x469010)
s32 Ped::sub_469010()
{
    return (this->field_200_id & 1) != 0 ? 56 : 51;
}

MATCH_FUNC(0x469030)
void Ped::UpdateKillerIdTimer_469030()
{
    if (field_264_killer_id_timer > 0)
    {
        if (field_27C_ped_state_2 != ped_state_2::Unknown_26)
        {
            field_264_killer_id_timer--;
            if (field_264_killer_id_timer == 0)
            {
                field_204_killer_id = 0;
                field_290 = 0;
            }
        }
    }
}

// https://decomp.me/scratch/rHsAD
WIP_FUNC(0x469060)
void Ped::GotoAreaByAnyMeans_469060()
{
    WIP_IMPLEMENTED;

    u8 bCanAllocate;
    u8 xpos;
    u8 ypos;
    u8 zpos;
    u8 target_x;
    u8 target_y;
    u8 target_z;

    if (gDistanceToTarget_678750 > kFpFour_678680 && !field_21C_bf.b2)
    {
        if (!field_16C_car)
        {
            switch (field_25C_internal_objective)
            {
                case objectives_enum::enter_car_as_driver_35:
                    if (field_154_target_to_enter->IsDespawning_4215B0())
                    {
                        Ped::SetObjective2_463830(objectives_enum::goto_area_on_foot_12, 128);
                        field_158_unk_car = 0;
                        field_1D0_internal_target_x = field_1DC_objective_target_x;
                        field_1D4_internal_target_y = field_1E0_objective_target_y;
                        field_1D8_internal_target_z = field_1E4_objective_target_z;
                    }
                    else
                    {
                        Sprite* pSprite = field_154_target_to_enter->field_50_car_sprite;
                        if (Fix16::MaxAbsDistance_42A6B0(field_1AC_cam.x, field_1AC_cam.y, pSprite->field_14_xy.x, pSprite->field_14_xy.y) >
                            kFpTwo_678658)
                        {
                            if (field_144_attacker)
                            {
                                Ped::ReactToAttacker_465B20();
                            }
                            field_144_attacker = 0;
                        }
                        if (field_25C_internal_objective == objectives_enum::enter_car_as_driver_35 &&
                            (field_154_target_to_enter->IsDespawning_4215B0() || field_226_internal_objective_status == 2))
                        {
                            Ped::SetObjective2_463830(objectives_enum::goto_area_on_foot_12, 64);
                            field_1D0_internal_target_x = field_1DC_objective_target_x;
                            field_1D4_internal_target_y = field_1E0_objective_target_y;
                            field_1D8_internal_target_z = field_1E4_objective_target_z;
                        }
                    }
                    break;

                case objectives_enum::leave_car_36:
                    if (field_226_internal_objective_status == 1)
                    {
                        Ped::SetObjective2_463830(objectives_enum::goto_area_on_foot_12, 128);
                        field_1D0_internal_target_x = field_1DC_objective_target_x;
                        field_1D4_internal_target_y = field_1E0_objective_target_y;
                        field_1D8_internal_target_z = field_1E4_objective_target_z;
                    }
                    break;

                case objectives_enum::goto_area_on_foot_12:
                    if (field_226_internal_objective_status == 1)
                    {
                        field_226_internal_objective_status = 0;
                        Ped::SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                    }
                    else
                    {
                        if (gDistanceToTarget_678750 > kFpSix_678678)
                        {
                            Ped::SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                        }
                        if (field_144_attacker)
                        {
                            Ped::ReactToAttacker_465B20();
                        }
                        field_144_attacker = 0;
                    }
                    break;

                case objectives_enum::flee_char_on_foot_till_safe_2:
                    if (field_158_unk_car)
                    {
                        Sprite* pSprite = field_158_unk_car->field_50_car_sprite;
                        if (Fix16::MaxAbsDistance_42A6B0(field_1AC_cam.x, field_1AC_cam.y, pSprite->field_14_xy.x, pSprite->field_14_xy.y) <
                                kFpFour_678680 ||
                            field_226_internal_objective_status == 1)
                        {
                            Ped::SetObjective2_463830(objectives_enum::enter_car_as_driver_35, 9999);
                            field_154_target_to_enter = field_158_unk_car;
                        }
                    }
                    else
                    {
                        Ped::SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                    }
                    break;

                case objectives_enum::kill_char_on_foot_20:
                {
                    if (Fix16::MaxAbsDistance_42A6B0(field_1AC_cam.x, field_1AC_cam.y, field_14C_internal_target_ped->field_1AC_cam.x, field_14C_internal_target_ped->field_1AC_cam.y) >
                            kFpTwo_678658 ||
                        field_226_internal_objective_status == 1)
                    {
                        field_21C_bf.b11 = false;
                        if (field_158_unk_car && field_158_unk_car->field_54_driver)
                        {
                            field_158_unk_car = 0;
                        }
                        if (field_158_unk_car)
                        {
                            Ped::SetObjective2_463830(objectives_enum::enter_car_as_driver_35, 9999);
                            field_154_target_to_enter = field_158_unk_car;
                        }
                        else
                        {
                            Ped::SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                        }
                    }
                    break;
                }

                case objectives_enum::objective_9:
                    if (field_164_ped_group->Get_F3C_433370() != 1)
                    {
                        if (field_158_unk_car)
                        {
                            Ped::SetObjective2_463830(objectives_enum::enter_car_as_driver_35, 9999);
                            field_154_target_to_enter = field_158_unk_car;
                        }
                        else
                        {
                            Ped::SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                        }
                    }
                    break;

                default:
                {
                    Car_BC* pCar;
                    if (!field_21C_bf.b6)
                    {
                        pCar = gCar_6C_677930->GetNearestEnterableCarFromCoord_444FA0(field_1AC_cam.x, field_1AC_cam.y, field_1AC_cam.z, this);
                        if (pCar)
                        {
                            Sprite* pSprite = pCar->field_50_car_sprite;
                            if (Fix16::MaxAbsDistance_42A6B0(field_1AC_cam.x, field_1AC_cam.y, pSprite->field_14_xy.x, pSprite->field_14_xy.y) >
                                kFpFour_678680)
                            {
                                pCar = 0;
                            }
                        }

                        if (!pCar)
                        {
                            xpos = field_1AC_cam.x.ToUInt8();
                            ypos = field_1AC_cam.y.ToUInt8();
                            zpos = field_1AC_cam.z.ToUInt8();

                            bCanAllocate = gCar_6C_677930->CanAllocateOfType_446930(1);
                            if (bCanAllocate && gOrca_2FD4_6FDEF0->FindNearbyTileMatchingSlopeType_5552B0(1, &xpos, &ypos, &zpos, 1))
                            {
                                pCar = gCar_6C_677930->SpawnCarAtRoadDirection_444CF0(field_274_gang_car_model, xpos, ypos, zpos);
                                if (pCar)
                                {
                                    pCar->IncrementCarStats_443D70(1);
                                    if (field_140_stolen_car && !field_140_stolen_car->field_54_driver)
                                    {
                                        field_140_stolen_car->SetUniNum_421560(3);
                                    }
                                    s8 gang_idx = gGangPool_CA8_67E274->FindGangByCarModel_4BF2F0(field_274_gang_car_model);
                                    if (gang_idx > -1)
                                    {
                                        Gang_144* pGang = gGangPool_CA8_67E274->GangByIdx_4BF1C0(gang_idx);
                                        pCar->AttachGangIcon_440660(pGang->field_138_arrow_colour);
                                        if (pGang->field_140_gang_car_remap > -1)
                                        {
                                            pCar->SetCarRemap(pGang->field_140_gang_car_remap);
                                        }
                                    }
                                    pCar->SetUniNum_421560(6);
                                    field_140_stolen_car = pCar;
                                }
                            }
                            else
                            {
                                if (gDistanceToTarget_678750 < kFpFour_678680)
                                {
                                    Ped::SetObjective2_463830(objectives_enum::goto_area_on_foot_12, 9999);
                                    field_1D0_internal_target_x = field_1DC_objective_target_x;
                                    field_1D4_internal_target_y = field_1E0_objective_target_y;
                                    field_1D8_internal_target_z = field_1E4_objective_target_z;
                                }
                                if (!bCanAllocate)
                                {
                                    gCar_6C_677930->field_54 = 2;
                                }
                            }
                        }
                    }
                    else
                    {
                        pCar = gTaxi_4_704130->GetTaxiNear_457BF0(field_1AC_cam.x, field_1AC_cam.y);
                        if (!pCar)
                        {
                            xpos = field_1AC_cam.x.ToUInt8();
                            ypos = field_1AC_cam.y.ToUInt8();
                            zpos = field_1AC_cam.z.ToUInt8();

                            if (gCar_6C_677930->CanAllocateOfType_446930(1) &&
                                gOrca_2FD4_6FDEF0->FindNearbyTileMatchingSlopeType_5552B0(1, &xpos, &ypos, &zpos, 1))
                            {
                                pCar = gCar_6C_677930->SpawnCarAtRoadDirection_444CF0(Ped::sub_469010(), xpos, ypos, zpos);
                                if (pCar)
                                {
                                    pCar->IncrementCarStats_443D70(1);
                                }
                            }
                        }

                        if (pCar)
                        {
                            // 9.6f: Car_BC::sub_421510 (allocates field_5C_AI if needed)
                            if (!pCar->field_5C_AI)
                            {
                                pCar->field_5C_AI = gCarAI_78_Pool_677CF8->Allocate();
                            }
                            pCar->field_5C_AI->SetCar_453BF0(pCar);
                            pCar->SpawnDriverPed();
                            pCar->SetUniNum_421560(6);
                            pCar->InitCarAIControl_440590();
                            // 9.6f: Car_BC::sub_426E00
                            pCar->field_9C_engine_status = 3;
                            pCar->HeadlightsOn_43BFE0();
                            pCar->sub_43AF60();
                        }
                    }

                    if (pCar && !pCar->field_80 && !pCar->IsDespawning_4215B0())
                    {
                        Ped::SetObjective2_463830(objectives_enum::enter_car_as_driver_35, 9999);
                        if (field_21C_bf.b6)
                        {
                            field_248_enter_car_as_passenger = 1;
                            pCar->sub_43AF60();
                            field_24C_target_car_door = 2;
                        }
                        else
                        {
                            field_248_enter_car_as_passenger = 0;
                        }
                        field_154_target_to_enter = pCar;
                        field_158_unk_car = pCar;
                        // 9.6f: Char_B4::Set_F84_433900
                        field_168_game_object->field_84_target_car = pCar;
                    }
                    else if (gDistanceToTarget_678750 < kFpFour_678680)
                    {
                        Ped::SetObjective2_463830(objectives_enum::goto_area_on_foot_12, 128);
                        field_1D0_internal_target_x = field_1DC_objective_target_x;
                        field_1D4_internal_target_y = field_1E0_objective_target_y;
                        field_1D8_internal_target_z = field_1E4_objective_target_z;
                    }
                    break;
                }
            }
        }
        else if (field_21C_bf.b6 && !field_16C_car->field_54_driver)
        {
            field_16C_car->field_80 = 1;
            field_16C_car->SetUniNum_421560(3);
            Ped::SetObjective2_463830(objectives_enum::leave_car_36, 9999);
            field_218_objective_timer = 0;
            field_154_target_to_enter = field_16C_car;
            field_158_unk_car = 0;
        }
        else if (field_25C_internal_objective == objectives_enum::goto_area_in_car_14)
        {
            if (field_16C_car->field_8C_damage_level >= 4)
            {
                field_16C_car->field_80 = 1;
                field_16C_car->SetUniNum_421560(3);
                Ped::SetObjective2_463830(objectives_enum::leave_car_36, 9999);
                field_218_objective_timer = 0;
                field_154_target_to_enter = field_16C_car;
                field_158_unk_car = 0;
            }
            else if (field_16C_car->GetVelocity_43A4C0() == kFpZero_678660)
            {
                if (++field_218_objective_timer > 500)
                {
                    field_16C_car->field_80 = 1;
                    field_16C_car->SetUniNum_421560(3);
                    Ped::SetObjective2_463830(objectives_enum::leave_car_36, 9999);
                    field_218_objective_timer = 0;
                    field_154_target_to_enter = field_16C_car;
                    field_158_unk_car = 0;
                }
            }
            else if (field_16C_car->GetVelocity_43A4C0() > kFpZero_678660)
            {
                field_218_objective_timer = 0;
            }
        }
        else
        {
            if (field_21C_bf.b6)
            {
                field_16C_car->SetUniNum_421560(6);
            }
            bool bGroupReady = field_23C_group_idx != 99 || field_164_ped_group->IsAllMembersInSomeCar_4CAA20();
            if (bGroupReady)
            {
                if (field_258_objective == objectives_enum::kill_char_any_means_19)
                {
                    Ped::SetObjective2_463830(objectives_enum::objective_52, 9999);
                    // 9.6f: set_field_14C_403AE0
                    field_14C_internal_target_ped = field_148_objective_target_ped;
                }
                else
                {
                    Ped::SetObjective2_463830(objectives_enum::goto_area_in_car_14, 9999);

                    target_x = field_1DC_objective_target_x.ToUInt8();
                    target_y = field_1E0_objective_target_y.ToUInt8();
                    target_z = field_1E4_objective_target_z.ToUInt8();

                    field_218_objective_timer = 0;
                    if (!gOrca_2FD4_6FDEF0->FindNearbyTileMatchingSlopeType_5552B0(1, &target_x, &target_y, &target_z, 0))
                    {
                        field_16C_car->field_80 = 1;
                        field_16C_car->SetUniNum_421560(3);
                        Ped::SetObjective2_463830(objectives_enum::leave_car_36, 9999);
                        field_218_objective_timer = 0;
                        field_154_target_to_enter = field_16C_car;
                        field_158_unk_car = 0;
                    }
                }
            }
            else
            {
                field_16C_car->sub_43AF60();
            }
        }
    }
    else if (!field_21C_bf.b2)
    {
        if (field_258_objective == objectives_enum::kill_char_any_means_19)
        {
            if (field_16C_car)
            {
                if (field_16C_car->GetVelocity_43A4C0() < kFpPoint02_678630 && !field_21C_bf.b4)
                {
                    Ped::SetObjective2_463830(objectives_enum::leave_car_36, 9999);
                    field_218_objective_timer = 0;
                    field_154_target_to_enter = field_16C_car;
                    if (field_16C_car->field_60)
                    {
                        gHamburger_500_678E30->FreeEntry_474CC0(field_16C_car->field_60);
                        field_16C_car->field_60 = 0;
                    }
                    field_16C_car->SetUniNum_421560(3);
                    field_16C_car->field_76_last_seen_timer = 0;
                }
            }
            else if (field_148_objective_target_ped->field_16C_car)
            {
                if (gDistanceToTarget_678750 > kFpFour_678680 && field_158_unk_car)
                {
                    Ped::SetObjective2_463830(objectives_enum::enter_car_as_driver_35, 9999);
                    field_154_target_to_enter = field_158_unk_car;
                }
                else
                {
                    Ped::SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                }
            }
        }
        else if (gDistanceToTarget_678750 < kFpOneEighth_6784E8 && !field_16C_car)
        {
            field_225_objective_status = 1;
            Ped::SetObjective2_463830(objectives_enum::wait_on_foot_26, 9999);
        }
        else if (field_16C_car)
        {
            if (field_16C_car->GetVelocity_43A4C0() == kFpZero_678660)
            {
                if (field_21C_bf.b6)
                {
                    field_16C_car->SetUniNum_421560(3);
                }
                Ped::SetObjective2_463830(objectives_enum::leave_car_36, 9999);
                field_218_objective_timer = 0;
                field_154_target_to_enter = field_16C_car;
            }
        }
        else
        {
            Ped::SetObjective2_463830(objectives_enum::goto_area_on_foot_12, 9999);
            field_1D0_internal_target_x = field_1DC_objective_target_x;
            field_1D4_internal_target_y = field_1E0_objective_target_y;
            field_1D8_internal_target_z = field_1E4_objective_target_z;
        }
    }
}

MATCH_FUNC(0x469bd0)
void Ped::sub_469BD0()
{
    if (field_168_game_object)
    {
        field_164_ped_group->UpdateMemberTightFollowState_4CA820(field_23C_group_idx);
    }
}

MATCH_FUNC(0x469bf0)
void Ped::GuardSpot_469BF0()
{
    u8 bUnknown = 1;

    this->field_21C |= 0x400000;

    if (!byte_61A8A3)
    {
        bUnknown = this->field_168_game_object->GetCharState_433A80() == 15 && (field_21C & 4) == 0;
    }

    if ((field_224 & 0x10) != 0 && ((field_21C & 4) != 0 || this->field_260 > 200u))
    {
        this->field_260 = 0;
        this->field_224 &= ~0x10;
    }

    if (bUnknown)
    {
        if ((this->field_224 & 0x10) != 0 ||
            gDistanceToTarget_678750 <= kFpThreeSixteenths_678780 &&
                abs_sub_less_than_epislon_45AE40(this->field_1AC_cam.z, this->field_1E4_objective_target_z))
        {
            if (this->field_168_game_object->GetCharState_433A80() != 15)
            {
                if ((field_224 & 0x10) != 0)
                {
                    Ped::ChangeNextPedState1_45C500(7);
                    Ped::ChangeNextPedState2_45C540(14);
                    field_168_game_object->RegulateVelocityByRef_433970(kFpZero_678438);
                    ++this->field_260;
                    this->field_130 = this->field_134_rotation;
                }
                else
                {
                    Ped::ChangeNextPedState1_45C500(7);
                    Ped::ChangeNextPedState2_45C540(14);
                    this->field_130 = this->field_134_rotation;
                }
            }
            else
            {
                this->field_224 |= 0x10;
            }
        }
        else
        {
            if (field_168_game_object->GetCharState_433A80() != 15)
            {
                field_168_game_object->SetMaxSpeed_433920(this->field_1F4);
                UpdateMovementTowardsTarget_4672E0(gDistanceToTarget_678750, 4);
            }
        }
    }
}

MATCH_FUNC(0x469d60)
void Ped::GuardArea_469D60()
{
    if (byte_61A8A3)
    {
        if (gDistanceToTarget_678750 <= kFpThreeThirtySeconds_6784DC && field_1AC_cam.z.ToUInt8() == field_1E4_objective_target_z.ToUInt8())
        {
            if (field_168_game_object->GetCharState_433A80() != Char_B4_state::Jumping_15)
            {
                Ped::ChangeNextPedState1_45C500(ped_state_1::standing_still_7);
                Ped::ChangeNextPedState2_45C540(ped_state_2::ped2_staying_14);
                field_130 = field_134_rotation;
            }
        }
        else
        {
            field_168_game_object->SetMaxSpeed_433920(field_1F4);
            Ped::UpdateMovementTowardsTarget_4672E0(gDistanceToTarget_678750, 4);
        }
    }
    else
    {
        if (field_21C_bf.b2 && gDistanceToTarget_678750 > field_1E8)
        {
            field_21C_bf.b22 = true;
        }
    }
}

MATCH_FUNC(0x469e10)
void Ped::sub_469E10()
{
    if (field_218_objective_timer == 0)
    {
        this->field_225_objective_status = objective_status::failed_2;
    }
}

MATCH_FUNC(0x469e30)
void Ped::sub_469E30()
{
    if (field_16C_car)
    {
        field_16C_car->field_5C_AI->field_74_unk_speed = kFpOne_678664;
    }
}

MATCH_FUNC(0x469e50)
void Ped::sub_469E50()
{
    if (field_16C_car)
    {
        if (!field_16C_car->field_60)
        {
            field_16C_car->field_60 = gHamburger_500_678E30->AllocateEntry_474810();
            field_16C_car->field_60->field_4_ped_owner = this;
        }
        field_16C_car->field_60->field_8_maybe_path_type = 4;
        field_16C_car->SetUniNum_421560(5);
        field_16C_car->field_60->field_30_ped_to_follow = field_148_objective_target_ped;
        field_16C_car->ClearA6Bit5_421550();
        field_16C_car->field_5C_AI->field_74_unk_speed = kFpThree_67866C;
        field_16C_car->field_60->field_20 = 1;
        if (field_16C_car->field_84_car_info_idx == car_model_enum::JEEP)
        {
            if (gDistanceToTarget_678750 < kFpTwo_678668)
            {
                field_21C_bf.b11 = true;
            }
            else
            {
                field_21C_bf.b11 = false;
            }
        }
    }
}

MATCH_FUNC(0x469f30)
void Ped::FollowPedInCar_469F30()
{
    if (!field_16C_car->field_60)
    {
        field_16C_car->field_60 = gHamburger_500_678E30->AllocateEntry_474810();
        field_16C_car->field_60->field_4_ped_owner = this;
    }
    field_16C_car->field_60->field_8_maybe_path_type = 2;
    field_16C_car->SetUniNum_421560(5);
    field_16C_car->field_60->field_30_ped_to_follow = field_148_objective_target_ped;
    field_16C_car->ClearA6Bit5_421550();
    field_16C_car->field_5C_AI->field_74_unk_speed = kFpThree_67866C;
}

MATCH_FUNC(0x469fc0)
void Ped::WaitInCurrentCar_469FC0()
{
    Car_BC* pBC = this->field_16C_car;
    if (pBC)
    {
        pBC->field_A6 |= 0x20u;
    }
    else
    {
        // He is out of the car
        this->field_225_objective_status = objective_status::failed_2;
    }
}

WIP_FUNC(0x469fe0)
void Ped::sub_469FE0()
{
    WIP_IMPLEMENTED;

    Car_BC* pCar;
    if (!this->field_150_target_objective_car)
    {
        u8 x = this->field_1AC_cam.x.ToUInt8();
        u8 y = this->field_1AC_cam.y.ToUInt8();
        u8 z = this->field_1AC_cam.z.ToUInt8();
        gOrca_2FD4_6FDEF0->FindNearbyTileMatchingSlopeType_5552B0(1, &x, &y, &z, 1);
        if (gCar_6C_677930->CanAllocateOfType_446930(car_kind::Unknown_10))
        {
            pCar = gCar_6C_677930->SpawnCarAtRoadDirection_444CF0(car_model_enum::COPCAR, x, y, z);
            if (pCar)
            {
                pCar->IncrementCarStats_443D70(car_kind::Unknown_10);
                if (gPolice_7B8_6FEE40->FBI_Army_5703E0(pCar))
                {
                    this->field_278_ped_state_1 = ped_state_1::in_car_10;
                    this->field_27C_ped_state_2 = ped_state_2::ped2_driving_10;
                    this->field_168_game_object->field_84_target_car = pCar;
                    this->field_248_enter_car_as_passenger = 1;
                    this->field_150_target_objective_car = pCar;
                    return;
                }
            }
        }
        else if (gCar_6C_677930->CanAllocateOfType_446930(car_kind::police_6))
        {
            pCar = gCar_6C_677930->SpawnCarAtRoadDirection_444CF0(car_model_enum::COPCAR, x, y, z);
            if (pCar)
            {
                pCar->IncrementCarStats_443D70(car_kind::police_6);
                if (gPolice_7B8_6FEE40->FBI_Army_5703E0(pCar))
                {
                    this->field_278_ped_state_1 = ped_state_1::in_car_10;
                    this->field_27C_ped_state_2 = ped_state_2::ped2_driving_10;
                    this->field_168_game_object->field_84_target_car = pCar;
                    this->field_248_enter_car_as_passenger = 1;
                    this->field_150_target_objective_car = pCar;
                    return;
                }
            }
        }

        this->field_278_ped_state_1 = ped_state_1::walking_0;
        this->field_27C_ped_state_2 = ped_state_2::ped2_walking_0;
        SetObjective(objectives_enum::no_obj_0, 9999);
        SetObjective2_463830(objectives_enum::no_obj_0, 9999);
    }
    else if (!this->field_218_objective_timer)
    {
        this->field_24C_target_car_door = 2;
        SetObjective2_463830(objectives_enum::no_obj_0, 9999);
        SetObjective(objectives_enum::objective_33, 9999);
        this->set_field_150_target_objective_car(this->field_16C_car);
    }
}

MATCH_FUNC(0x46a1f0)
void Ped::PullDriverOutOfCar_46A1F0()
{
    if (!field_148_objective_target_ped->CheckBit0_433B40() ||
        field_148_objective_target_ped->GetPedState_403990() == ped_state_1::dead_9)
    {
        field_225_objective_status = objective_status::failed_2;
    }
    else
    {
        if (field_21C_bf.b2 && field_148_objective_target_ped == field_14C_internal_target_ped)
        {
            field_21C_bf.b2 = false;
            Ped::SetObjective2_463830(objectives_enum::no_obj_0, 9999);
            byte_61A8A3 = 1;
        }
        else if (!byte_61A8A3)
        {
            return;
        }

        if (field_25C_internal_objective)
        {
            if (field_25C_internal_objective == 32)
            {
                if (field_226_internal_objective_status == 1)
                {
                    field_225_objective_status = objective_status::passed_1;
                }
                else if (field_226_internal_objective_status == 2)
                {
                    field_225_objective_status = objective_status::failed_2;
                }
            }
        }
        else
        {
            Ped::SetObjective2_463830(32, 9999);
            field_14C_internal_target_ped = field_148_objective_target_ped;
        }
    }
}

MATCH_FUNC(0x46a290)
void Ped::FollowCarInCurrCar_46A290()
{
    if (!field_150_target_objective_car->field_54_driver || field_168_game_object)
    {
        // If the car it's following doesnt have a driver OR this ped (which will follow) is on foot (not in a car)
        field_225_objective_status = objective_status::failed_2;
    }
    else
    {
        if (!field_16C_car->field_60)
        {
            // If no path, create one
            field_16C_car->field_60 = gHamburger_500_678E30->AllocateEntry_474810();
            field_16C_car->field_60->field_4_ped_owner = this;
        }
        field_16C_car->field_60->field_8_maybe_path_type = 2;
        field_16C_car->SetUniNum_421560(5);
        field_16C_car->field_60->field_30_ped_to_follow = field_150_target_objective_car->field_54_driver;
        field_16C_car->ClearA6Bit5_421550();
        field_16C_car->field_5C_AI->field_74_unk_speed = kFpThree_67866C;
    }
}

MATCH_FUNC(0x46a350)
void Ped::FollowCarOnFootWithOffset_46A350()
{

    if (field_150_target_objective_car->field_88_despawn_status == 5)
    {
        this->field_225_objective_status = objective_status::failed_2;
    }
    else
    {
        Ang16 ang = field_150_target_objective_car->field_50_car_sprite->field_0;
        ang -= field_132_follow_car_offset_angle;
        Fix16 sin_v = Ang16::sine_40F500(ang) * this->field_1FC_follow_car_offset_distance;
        Fix16 cos_v = Ang16::cosine_40F520(ang) * this->field_1FC_follow_car_offset_distance;

        if (byte_61A8A3)
        {
            if (field_25C_internal_objective)
            {
                if (field_25C_internal_objective == objectives_enum::follow_car_on_foot_with_offset_56)
                {
                    this->field_1D0_internal_target_x = sin_v + field_150_target_objective_car->field_50_car_sprite->field_14_xy.x;
                    this->field_1D4_internal_target_y = cos_v + field_150_target_objective_car->field_50_car_sprite->field_14_xy.y;
                    this->field_1D8_internal_target_z = field_150_target_objective_car->field_50_car_sprite->field_1C_zpos;
                    if (this->field_226_internal_objective_status == 1)
                    {
                        if (field_168_game_object->field_38_velocity < kFpZero_678660)
                        {
                            field_168_game_object->field_38_velocity += kFpOne256th_678620;
                        }
                        else if (field_168_game_object->field_38_velocity > kFpZero_678660)
                        {
                            field_168_game_object->field_38_velocity -= kFpOne256th_678620;
                        }
                        this->field_226_internal_objective_status = 0;
                    }
                }
            }
            else
            {
                Ped::SetObjective2_463830(objectives_enum::follow_car_on_foot_with_offset_56, 9999);
                this->field_1D0_internal_target_x = sin_v + field_150_target_objective_car->field_50_car_sprite->field_14_xy.x;
                this->field_1D4_internal_target_y = cos_v + field_150_target_objective_car->field_50_car_sprite->field_14_xy.y;
                this->field_1D8_internal_target_z = field_150_target_objective_car->field_50_car_sprite->field_1C_zpos;
            }
        }
    }
}

MATCH_FUNC(0x46a530)
void Ped::FireAtObject_46A530()
{
    Sprite_18* pSprite_148 = field_16C_car->field_0_qq.GetSpriteForModel_5A6A50(148);
    Sprite* pSprite_18 = pSprite_148->field_0;
    Fix16 x_v = pSprite_18->field_14_xy.x;
    Fix16 y_v = pSprite_18->field_14_xy.y;
    Ang16 v7;
    v7 = Fix16::atan2_fixed_405320(field_1A0_objective_target_object->field_4->field_14_xy.y - y_v,
                                   field_1A0_objective_target_object->field_4->field_14_xy.x - x_v);

    field_21C |= 0x80;

    if (field_16C_car->RotateRoofObjectTowardTarget_440C10(v7))
    {
        field_21C |= 0x800;
    }
    else
    {
        field_21C &= ~0x800;
    }
}

MATCH_FUNC(0x46a5e0)
void Ped::FireAtPlayer_46A5E0()
{
    if (!field_16C_car)
    {
        this->field_225_objective_status = 0;
        return;
    }

    Ped* pPlayerPed = gGame_0x40_67E008->field_38_orf1->field_2C4_player_ped;
    if (!pPlayerPed->sub_433DA0())
    {
        Sprite* pSprite_148 = field_16C_car->field_0_qq.GetSpriteForModel_5A6A50(148)->field_0;
        Fix16 x = pSprite_148->field_14_xy.x;
        Fix16 y = pSprite_148->field_14_xy.y;
        Ang16 v6;
        v6 = Fix16::atan2_fixed_405320(pPlayerPed->get_cam_y() - y, pPlayerPed->get_cam_x() - x);

        this->field_21C |= 0x80;

        if (field_16C_car->RotateRoofObjectTowardTarget_440C10(v6))
        {
            if (!this->field_16C_car->field_76_last_seen_timer)
            {
                this->field_21C |= 0x800;
            }
            return;
        }
    }
    this->field_21C &= ~0x800u;
}


MATCH_FUNC(0x46a6d0)
void Ped::AimVehicleTurretStateMachine_46A6D0()
{
    if (field_150_target_objective_car->IsDespawning_4215B0() || field_16C_car == 0)
    {
        field_225_objective_status = objective_status::failed_2;
    }
    else
    {
        Sprite_18* p18 = field_16C_car->field_0_qq.GetSpriteForModel_5A6A50(114);
        field_21C |= 0x80;

        Fix16 x = p18->field_0->field_14_xy.x;
        Fix16 y = p18->field_0->field_14_xy.y;
        Ang16 angle;
        angle = Fix16::atan2_fixed_405320(field_150_target_objective_car->field_50_car_sprite->field_14_xy.y - y,
                                          field_150_target_objective_car->field_50_car_sprite->field_14_xy.x - x);
        if (field_16C_car->RotateRoofObjectTowardTarget_440C10(angle))
        {
            field_21C |= 0x800;

            if (field_218_objective_timer == 9999)
            {
                field_218_objective_timer = 50;
            }
        }
        else
        {
            field_21C &= ~0x800u;
        }

        if (field_218_objective_timer == 0)
        {
            field_225_objective_status = objective_status::passed_1;
        }
    }
}

MATCH_FUNC(0x46a7c0)
void Ped::DestroyTargetObject_46A7C0()
{
    if (byte_61A8A3 && (field_21C & 4) == 0)
    {
        switch (field_25C_internal_objective)
        {
            case 0:
                Ped::SetObjective2_463830(58, 9999);
                field_1A4_internal_target_object = field_1A0_objective_target_object;
                return;
            case 1:
                if (field_226_internal_objective_status != 1)
                {
                    return;
                }
                break;
            case 58:
                if (field_226_internal_objective_status == 1)
                {
                    field_225_objective_status = objective_status::passed_1;
                }
                if (field_226_internal_objective_status == 2)
                {
                    field_225_objective_status = objective_status::failed_2;
                }
                return;
            default:
                return;
        }
        Ped::SetObjective2_463830(58, 9999);
        field_1A4_internal_target_object = field_1A0_objective_target_object;
    }
}

MATCH_FUNC(0x46a850)
void Ped::DestroyTargetCar_46A850()
{
    if (field_150_target_objective_car->IsMaxDamage_40F890())
    {
        field_225_objective_status = objective_status::passed_1;
    }
    if (byte_61A8A3 && (field_21C & 4) == 0)
    {
        switch (field_25C_internal_objective)
        {
            case 0:
                Ped::SetObjective2_463830(59, 9999);
                field_154_target_to_enter = field_150_target_objective_car;
                return;
            case 1:
                if (field_226_internal_objective_status != 1)
                {
                    return;
                }
                break;
            case 59:
                if (field_226_internal_objective_status == 1)
                {
                    field_225_objective_status = objective_status::passed_1;
                }
                if (field_226_internal_objective_status == 2)
                {
                    field_225_objective_status = objective_status::failed_2;
                }
                return;
            default:
                return;
        }
        Ped::SetObjective2_463830(59, 9999);
        field_154_target_to_enter = field_150_target_objective_car;
    }
}

MATCH_FUNC(0x46a8f0)
void Ped::FleeOnFootTillSafe_46A8F0()
{
    if (gDistanceToTarget_678750 > kFpSix_678520)
    {
        Char_B4* pB4 = field_168_game_object;
        if (pB4)
        {
            if (field_258_objective || pB4->Get_F44_433A90() == 2)
            {
                Ped::ChangeNextPedState1_45C500(ped_state_1::walking_0);
                Ped::ChangeNextPedState2_45C540(ped_state_2::ped2_walking_0);
                field_226_internal_objective_status = 1;
            }
            else
            {
                pB4->SetMaxSpeed_433920(get_max_speed_1F0());
            }
        }
        else
        {
            field_21C_bf.b11 = 0;
        }
    }
    else
    {
        field_168_game_object->SetMaxSpeed_433920(get_max_speed_1F0());
    }
}

MATCH_FUNC(0x46a9c0)
void Ped::FleeFromPedTillSafe_46A9C0()
{
    field_14C_internal_target_ped->ClearF144_433BE0();
    if (field_14C_internal_target_ped->isDead_403B60() || !field_14C_internal_target_ped->CheckBit0_433B40())
    {
        Ped::ChangeNextPedState1_45C500(ped_state_1::walking_0);
        Ped::ChangeNextPedState2_45C540(ped_state_2::ped2_walking_0);
        field_14C_internal_target_ped = 0;
        field_226_internal_objective_status = 1;
    }
    else if (field_278_ped_state_1 != ped_state_1::immobilized_8)
    {
        if (gDistanceToTarget_678750 > kFpSix_678520)
        {
            if (field_168_game_object->Get_F44_433A90() == 2)
            {
                Ped::ChangeNextPedState1_45C500(ped_state_1::walking_0);
                Ped::ChangeNextPedState2_45C540(ped_state_2::ped2_walking_0);
                field_14C_internal_target_ped = 0;
                field_226_internal_objective_status = 1;
            }
        }
        else
        {
            Ped::ChangeNextPedState1_45C500(ped_state_1::flee_or_running_1);
            Ped::ChangeNextPedState2_45C540(ped_state_2::Unknown_3);
            field_168_game_object->UseRunOrJumpSpeed_433930();
        }
    }
}

MATCH_FUNC(0x46aae0)
void Ped::FleeFromPedAlways_46AAE0()
{
    if (field_14C_internal_target_ped->isDead_403B60() || !field_14C_internal_target_ped->CheckBit0_433B40())
    {
        Ped::ChangeNextPedState1_45C500(ped_state_1::walking_0);
        Ped::ChangeNextPedState2_45C540(ped_state_2::ped2_walking_0);
        field_14C_internal_target_ped = 0;
        field_226_internal_objective_status = 1;
        field_21C_bf.b2 = false;
    }
    else
    {
        field_14C_internal_target_ped->ClearF144_433BE0();
        field_168_game_object->UseRunOrJumpSpeed_433930();
    }
}

MATCH_FUNC(0x46ab50)
void Ped::sub_46AB50()
{
    if (field_14C_internal_target_ped->isDead_403B60() || !field_14C_internal_target_ped->CheckBit0_433B40())
    {
        Ped::ChangeNextPedState1_45C500(ped_state_1::walking_0);
        Ped::ChangeNextPedState2_45C540(ped_state_2::ped2_walking_0);
        field_14C_internal_target_ped = 0;
        field_226_internal_objective_status = 1;
        Ped::SetObjective2_463830(objectives_enum::no_obj_0, 9999);
        field_21C_bf.b2 = false;
    }
    else
    {
        if (field_278_ped_state_1 != ped_state_1::immobilized_8)
        {
            field_21C_bf.b11 = false;
            field_14C_internal_target_ped->ClearF144_433BE0();
            if (gDistanceToTarget_678750 < kFpOne_6785EC)
            {
                Ped::ChangeNextPedState1_45C500(ped_state_1::walking_0);
                Ped::ChangeNextPedState2_45C540(ped_state_2::ped2_walking_0);
                field_14C_internal_target_ped = 0;
                field_226_internal_objective_status = 1;
                Ped::SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                field_21C_bf.b2 = false;
            }
            else
            {
                Ped::ChangeNextPedState1_45C500(ped_state_1::flee_or_running_1);
                Ped::ChangeNextPedState2_45C540(ped_state_2::Unknown_2);
                Ped::UpdateMovementTowardsTarget_4672E0(gDistanceToTarget_678750, 0);
                field_168_game_object->UseRunOrJumpSpeed_433930();
            }
        }
    }
}

// https://decomp.me/scratch/ahbj8
MATCH_FUNC(0x46ac20)
void Ped::FollowTargetStateMachine_46AC20()
{
    bool bUnknown = false;
    if (field_14C_internal_target_ped->isDead_403B60() || !field_14C_internal_target_ped->CheckBit0_433B40())
    {
        field_226_internal_objective_status = 0;
        field_21C_bf.b2 = false;
        return;
    }
    if (field_14C_internal_target_ped->field_168_game_object)
    {
        if (field_14C_internal_target_ped->field_168_game_object->GetCharState_433A80() == Char_B4_state::Jumping_15)
        {
            bUnknown = true;
        }
    }
    if (field_14C_internal_target_ped->GetPedVelocity_45C920() != kFpZero_678660)
    {
        bUnknown = true;
    }
    if (field_164_ped_group)
    {
        if (field_164_ped_group->field_2C_ped_leader)
        {
            if (field_164_ped_group->field_2C_ped_leader->GetPedVelocity_45C920() != kFpZero_678660)
            {
                bUnknown = true;
            }
        }
    }

    if (field_14C_internal_target_ped->field_25C_internal_objective == objectives_enum::objective_18 || bUnknown)
    {
        field_224 &= ~0x10u;
    }

    if ((field_224 & 0x10) != 0 && field_14C_internal_target_ped->GetPedVelocity_45C920() != kFpZero_678660)
    {
        field_224 &= ~0x10u;
    }

    if (field_278_ped_state_1 != ped_state_1::immobilized_8)
    {
        if (gDistanceToTarget_678750 < kFpTwo_678658)
        {
            field_21C_bf.b2 = false;
        }

        if (gDistanceToTarget_678750 >= kFpThreeSixteenths_678780 && (field_224 & 0x10) == 0)
        {
            if (gDistanceToTarget_678750 > kFpHalf_678790)
            {
                field_168_game_object->IncreaseSpeedIfAllowed_433940();
            }
            else if (gDistanceToTarget_678750 > kFpThreeEighths_67878C)
            {
                field_168_game_object->RegulateVelocityByRef_433970(kFpThreeSixtyFourths_678444);
            }
            else if (gDistanceToTarget_678750 > kFpFiveSixteenths_678784)
            {
                field_168_game_object->RegulateVelocityByRef_433970(kFpOneThirtySecond_678434);
            }
            else
            {
                field_168_game_object->RegulateVelocityByRef_433970(kFpOne64th_678430);
            }

            if (byte_6787C4)
            {
                if (gDistanceToTarget_678750 < kFpHalf_678790)
                {
                    if (field_14C_internal_target_ped->GetPedVelocity_45C920() == kFpZero_678660)
                    {
                        field_168_game_object->RegulateVelocityByRef_433970(kFpOne64th_678430);
                    }
                    else
                    {
                        field_168_game_object->RegulateVelocityByRef_433970(kFpOne64th_678430 + field_14C_internal_target_ped->GetPedVelocity_45C920());
                    }
                }
                else
                {
                    field_168_game_object->RegulateVelocityByRef_433970(kFpFiveSixtyFourths_67843C);
                }
            }
            Ped::UpdateMovementTowardsTarget_4672E0(gDistanceToTarget_678750, 0);
        }
        else
        {
            if (field_168_game_object->GetCharState_433A80() != Char_B4_state::Jumping_15)
            {
                if (byte_6787C4)
                {
                    if ((field_224 & 0x10) != 0)
                    {
                        Ped::ChangeNextPedState1_45C500(7);
                        Ped::ChangeNextPedState2_45C540(14);
                        field_168_game_object->RegulateVelocityByRef_433970(kFpZero_678438);
                    }
                    else if (gDistanceToTarget_678750 > kFpOneThirtySecond_6784CC)
                    {
                        if (field_14C_internal_target_ped->GetPedVelocity_45C920() == kFpZero_678660)
                        {
                            field_168_game_object->RegulateVelocityByRef_433970(kFpOne64th_678430);
                        }
                        else if (gDistanceToTarget_678750 > kFpThreeSixteenths_678780)
                        {
                            field_168_game_object->RegulateVelocityByRef_433970(kFpFiveSixtyFourths_67843C);
                        }
                        else
                        {
                            field_168_game_object->RegulateVelocityByRef_433970(field_14C_internal_target_ped->GetPedVelocity_45C920());
                        }
                    }
                    else
                    {
                        if (field_14C_internal_target_ped->GetPedVelocity_45C920() == kFpZero_678660)
                        {
                            Ped::ChangeNextPedState1_45C500(7);
                            Ped::ChangeNextPedState2_45C540(14);
                            field_168_game_object->RegulateVelocityByRef_433970(kFpZero_678438);
                        }
                        else
                        {
                            field_168_game_object->RegulateVelocityByRef_433970(field_14C_internal_target_ped->GetPedVelocity_45C920());
                        }
                    }
                }
                else
                {
                    Ped::ChangeNextPedState1_45C500(7);
                    Ped::ChangeNextPedState2_45C540(14);
                    field_168_game_object->RegulateVelocityByRef_433970(kFpZero_678438);
                }
            }
            else
            {
                field_224 |= 0x10u;
            }
        }
    }
}

MATCH_FUNC(0x46b170)
void Ped::ChaseTargetStateMachine_46B170()
{
    if (!field_14C_internal_target_ped->isDead_403B60() && field_14C_internal_target_ped->CheckBit0_433B40())
    {
        if (field_278_ped_state_1 != ped_state_1::immobilized_8)
        {
            if (gDistanceToTarget_678750 < kFpOneEighth_6784E8)
            {
                if (field_168_game_object->GetCharState_433A80() != 15)
                {
                    Ped::ChangeNextPedState1_45C500(ped_state_1::standing_still_7);
                    Ped::ChangeNextPedState2_45C540(ped_state_2::ped2_staying_14);

                    field_168_game_object->RegulateVelocityByRef_433970(kFpZero_678438);
                }
            }
            else
            {
                if (gDistanceToTarget_678750 > kFpHalf_678790)
                {
                    field_168_game_object->IncreaseSpeedIfAllowed_433940();
                }
                else if (gDistanceToTarget_678750 > kFpThreeEighths_67878C)
                {
                    field_168_game_object->RegulateVelocityByRef_433970(kFpThreeSixtyFourths_678444);
                }
                else if (gDistanceToTarget_678750 > kFpFiveSixteenths_678784)
                {
                    field_168_game_object->RegulateVelocityByRef_433970(kFpOneThirtySecond_678434);
                }
                else
                {
                    field_168_game_object->RegulateVelocityByRef_433970(kFpOne64th_678430);
                }

                if (field_168_game_object->GetCharState_433A80() == 10)
                {
                    field_168_game_object->SetMaxSpeedByRef_433920(kFpZero_678438);
                }
                Ped::UpdateMovementTowardsTarget_4672E0(gDistanceToTarget_678750, 0);
            }
        }
    }
    else
    {
        field_226_internal_objective_status = 1;
    }
}

// https://decomp.me/scratch/1MwH3
MATCH_FUNC(0x46b2f0)
void Ped::PullDriverOutOfCarStateMachine_46B2F0()
{

    u8 door_num;
    Car_BC* pCar = field_14C_internal_target_ped->field_16C_car;
    field_24C_target_car_door = 0;
    if (field_14C_internal_target_ped->field_168_game_object)
    {
        field_226_internal_objective_status = 2;
        Ped::ChangeNextPedState1_45C500(0);
        Ped::ChangeNextPedState2_45C540(0);
        return;
    }
    if (pCar->IsTrainModel_403BA0())
    {
        field_226_internal_objective_status = 2;
        Ped::ChangeNextPedState1_45C500(0);
        Ped::ChangeNextPedState2_45C540(0);
        return;
    }

    for (door_num = 0; door_num < (u8)pCar->GetRemap(); door_num++)
    {
        if (pCar->IsDoorAccessible_43AFE0(door_num))
        {
            field_24C_target_car_door = door_num;
            pCar->GetDoorWorldPosition_43B5A0(door_num, &field_1C4_x, &field_1C8_y);
            field_1CC_z = field_14C_internal_target_ped->field_1AC_cam.z;
            field_168_game_object->Set_F84_433900(pCar);

            Fix16 d_x = field_1C4_x - field_1AC_cam.x;
            Fix16 d_y = field_1C8_y - field_1AC_cam.y;
            Fix16 d_x_abs = Fix16::Abs(d_x);
            Fix16 d_y_abs = Fix16::Abs(d_y);
            gDistanceToTarget_678750 = Fix16::Max(d_x_abs, d_y_abs);

            if (field_14C_internal_target_ped->field_16C_car->field_84_car_info_idx == car_model_enum::BUS && field_27C_ped_state_2 == ped_state_2::Unknown_8)
            {
                gDistanceToTarget_678750 = kFpOne64th_6784C4;
            }
            Fix16 v14;
            if (!Ped::IsLawEnforcement_45B4E0())
            {
                v14 = kFpPoint2_67856C;
            }
            else
            {
                v14 = kFpPoint1_678428;
            }
            if (field_27C_ped_state_2 != ped_state_2::Unknown_9 &&
                (gDistanceToTarget_678750 >= kFpThreeSixteenths_678780 ||
                 !abs_sub_less_than_epislon_45AE40(field_1AC_cam.z, field_14C_internal_target_ped->field_1AC_cam.z) ||
                 field_278_ped_state_1 == ped_state_1::immobilized_8))
            {
                field_168_game_object->SetMaxSpeedByRef_433920(kFpOneSixteenth_678448);
                Ped::UpdateMovementTowardsTarget_4672E0(gDistanceToTarget_678750, 0);
            }
            else
            {
                switch (field_27C_ped_state_2)
                {
                    case ped_state_2::Unknown_8:
                        if (pCar->GetVelocity_43A4C0() > v14)
                        {
                            field_226_internal_objective_status = 2;
                            Ped::ChangeNextPedState1_45C500(0);
                            Ped::ChangeNextPedState2_45C540(0);
                        }
                        else
                        {
                            field_21C_bf.b27 = true;
                            if (field_168_game_object->field_68_animation_frame == 4)
                            {
                                Ped::ChangeNextPedState2_45C540(9);
                                field_168_game_object->field_68_animation_frame = 9;
                            }
                            field_168_game_object->SetCharState_433A60(36);
                            field_168_game_object->SetMaxSpeedByRef_433920(kFpZero_678438);
                        }
                        break;

                    case ped_state_2::Unknown_9:
                        if (field_168_game_object->field_68_animation_frame == 13)
                        {
                            field_168_game_object->field_68_animation_frame = 4;
                            Car_Door_10* Door = pCar->GetDoor(field_24C_target_car_door);
                            Door->Close_439EA0();
                            Door->set_ped_421380(0);
                            Ped::ChangeNextPedState2_45C540(14);
                        }
                        else
                        {
                            field_21C_bf.b27 = true;
                        }
                        field_168_game_object->SetCharState_433A60(36);
                        field_168_game_object->SetMaxSpeedByRef_433920(kFpZero_678438);
                        break;

                    default:
                        if (pCar->GetVelocity_43A4C0() > v14)
                        {
                            field_226_internal_objective_status = 2;
                            Ped::ChangeNextPedState1_45C500(0);
                            Ped::ChangeNextPedState2_45C540(0);
                            return;
                        }
                        else
                        {
                            field_21C_bf.b27 = true;
                            field_168_game_object->field_6C_animation_state = Char_Anim_state::Unknown_9;
                            field_168_game_object->field_68_animation_frame = 0;
                            Ped::ChangeNextPedState1_45C500(7);
                            Ped::ChangeNextPedState2_45C540(8);
                            field_168_game_object->SetCharState_433A60(36);
                            field_168_game_object->SetMaxSpeedByRef_433920(kFpZero_678438);
                        }
                        break;
                }
            }
            return;
        }
    }

    Ped::ChangeNextPedState1_45C500(0);
    Ped::ChangeNextPedState2_45C540(0);
    field_226_internal_objective_status = 2;
}

// https://decomp.me/scratch/aE2Ac
// Logic follows the 10.5 asm. Only remaining difference: VC6 tail-merges the mugging block of the
// health >= 20 path into the one in the `target == field_148` path, the original keeps its own copy.
WIP_FUNC(0x46b670)
void Ped::MeleeAttackStateMachine_46B670()
{
    WIP_IMPLEMENTED;
    if (field_14C_internal_target_ped->GetPedState_403990() == ped_state_1::dead_9 || !field_14C_internal_target_ped->CheckBit0_433B40())
    {
        if (field_25C_internal_objective == objectives_enum::punch_char_23)
        {
            field_226_internal_objective_status = 2;
        }
        else
        {
            field_226_internal_objective_status = 1;
        }
        return;
    }

    if (field_278_ped_state_1 == ped_state_1::immobilized_8 ||
        !abs_sub_less_than_epislon_45AE40(field_1AC_cam.z, field_14C_internal_target_ped->field_1AC_cam.z))
    {
        Ped::UpdateMovementTowardsTarget_4672E0(gDistanceToTarget_678750, 0);
        return;
    }

    field_14C_internal_target_ped->SetAttacker_433BF0(this);

    if (field_14C_internal_target_ped->field_16C_car)
    {
        Ped::PullDriverOutOfCarStateMachine_46B2F0();
        return;
    }

    gDistanceToTarget_678750 =
        Fix16::MaxAbsDistance_42A6B0(field_1AC_cam.x, field_1AC_cam.y, field_14C_internal_target_ped->get_cam_x(), field_14C_internal_target_ped->get_cam_y());

    if (gDistanceToTarget_678750 <= kFpQuarter_678788)
    {
        // Ped is close
        field_21C_bf.b9 = true;
        field_21C_bf.b11 = true;
        if (field_27C_ped_state_2 == ped_state_2::Unknown_9)
        {
            Sprite* pNearestSprt = gPurpleDoom_1_679208->FindNearestSpriteOfType_477E60(field_168_game_object->field_80_sprite_ptr, 2);
            if (pNearestSprt && pNearestSprt->get_type_416B40() == sprite_types_enum::car_2)
            {
                field_278_ped_state_1 = ped_state_1::immobilized_8;
                field_27C_ped_state_2 = ped_state_2::Unknown_17;
                field_168_game_object->field_68_animation_frame = 3;
            }
            else
            {
                field_278_ped_state_1 = ped_state_1::standing_still_7;
                field_27C_ped_state_2 = ped_state_2::ped2_staying_14;
                field_168_game_object->field_6C_animation_state = 2;
                field_168_game_object->field_68_animation_frame = 0;
            }
            return;
        }
        field_168_game_object->SetMaxSpeed_433920(field_14C_internal_target_ped->GetPedVelocity_45C920());
        if (field_168_game_object->GetCharState_433A80() != Char_B4_state::Jumping_15 &&
            field_168_game_object->get_velocity_41B080() == kFpZero_678660)
        {
            Ped::ChangeNextPedState1_45C500(ped_state_1::standing_still_7);
            Ped::ChangeNextPedState2_45C540(ped_state_2::ped2_staying_14);
        }
        ++field_14C_internal_target_ped->field_228;

        if (field_25C_internal_objective != objectives_enum::punch_char_23 &&
            field_28C_threat_reaction != threat_reaction_enum::react_as_emergency_1)
        {
            if (field_168_game_object->field_68_animation_frame == 0)
            {
                if (field_14C_internal_target_ped->IsField238_45EDE0(2) && field_240_occupation == ped_ocupation_enum::mugger)
                {
                    field_14C_internal_target_ped->field_15C_player->Add_2D4(-10);
                    ++field_229;
                    if (field_229 > 9)
                    {
                        field_226_internal_objective_status = 1;
                    }
                }
                else if (field_14C_internal_target_ped->field_240_occupation != ped_ocupation_enum::criminal_type_1)
                {
                    field_14C_internal_target_ped->TakeDamage(10);
                }
            }
            return;
        }

        if (field_14C_internal_target_ped->get_health_433B70() >= 20)
        {
            if (field_168_game_object->field_68_animation_frame < 1 || field_14C_internal_target_ped->GetPedState2_433B60() == ped_state_2::lying_on_floor_22)
            {
                field_14C_internal_target_ped->field_204_killer_id = field_200_id;
                field_14C_internal_target_ped->field_290 = 10;
                field_14C_internal_target_ped->field_264_killer_id_timer = 50;

                if (field_14C_internal_target_ped->IsField238_45EDE0(2))
                {
                    if (field_240_occupation == ped_ocupation_enum::mugger)
                    {
                        field_14C_internal_target_ped->field_15C_player->Add_2D4(-10);
                        ++field_229;
                        if (field_229 > 9)
                        {
                            field_226_internal_objective_status = 1;
                        }
                    }
                    else
                    {
                        field_14C_internal_target_ped->sub_433E50();
                        if (field_14C_internal_target_ped->field_240_occupation != ped_ocupation_enum::criminal_type_1)
                        {
                            field_14C_internal_target_ped->TakeDamage(10);
                        }
                    }
                }
                else
                {
                    field_14C_internal_target_ped->sub_433E50();
                    if (field_14C_internal_target_ped->field_240_occupation != ped_ocupation_enum::criminal_type_1)
                    {
                        field_14C_internal_target_ped->TakeDamage(10);
                    }
                }
            }
        }
        else
        {
            field_188_last_char_punched = field_14C_internal_target_ped;
            if (field_14C_internal_target_ped != field_148_objective_target_ped)
            {
                field_14C_internal_target_ped->ChangeNextPedState1_45C500(ped_state_1::immobilized_8);
                field_14C_internal_target_ped->ChangeNextPedState2_45C540(ped_state_2::lying_on_floor_22);
                field_14C_internal_target_ped->Set_B4_F16_To_1_433B50();
                field_226_internal_objective_status = 1;
                field_144_attacker = 0;
                field_228 = 0;
                field_21C_bf.b2 = false;

                if (field_28C_threat_reaction == threat_reaction_enum::react_as_emergency_1 && field_14C_internal_target_ped->IsField238_45EDE0(2))
                {
                    if (bStartNetworkGame_7081F0)
                    {
                        field_14C_internal_target_ped->Kill_46F9D0();
                    }
                    else
                    {
                        field_14C_internal_target_ped->field_21C_bf.b5 = true;
                        Set_F250_IfBit_433DD0(18);
                    }
                }
            }
            else if (field_258_objective != objectives_enum::punch_char_23 &&
                     field_28C_threat_reaction != threat_reaction_enum::react_as_emergency_1)
            {
                field_14C_internal_target_ped->field_204_killer_id = field_200_id;
                field_14C_internal_target_ped->field_290 = 10;
                field_14C_internal_target_ped->field_264_killer_id_timer = 50;

                if (field_14C_internal_target_ped->IsField238_45EDE0(2) && field_240_occupation == ped_ocupation_enum::mugger)
                {
                    field_14C_internal_target_ped->field_15C_player->Add_2D4(-10);
                    ++field_229;
                    if (field_229 > 9)
                    {
                        field_226_internal_objective_status = 1;
                    }
                }
                else if (field_14C_internal_target_ped->field_240_occupation != ped_ocupation_enum::criminal_type_1)
                {
                    field_14C_internal_target_ped->TakeDamage(10);
                }
            }
            else
            {
                field_14C_internal_target_ped->ChangeNextPedState1_45C500(ped_state_1::immobilized_8);
                field_14C_internal_target_ped->ChangeNextPedState2_45C540(ped_state_2::lying_on_floor_22);
                field_14C_internal_target_ped->Set_B4_F16_To_1_433B50();
                field_226_internal_objective_status = 1;
                field_144_attacker = 0;
                field_21C_bf.b2 = false;
                field_228 = 0;

                if (field_28C_threat_reaction == threat_reaction_enum::react_as_emergency_1 && field_14C_internal_target_ped->IsField238_45EDE0(2))
                {
                    if (bStartNetworkGame_7081F0)
                    {
                        field_14C_internal_target_ped->Kill_46F9D0();
                    }
                    else
                    {
                        field_14C_internal_target_ped->field_21C_bf.b5 = true;
                        Set_F250_IfBit_433DD0(18);
                    }
                }
            }
        }
    }
    else
    {
        // Ped is too far
        if (field_14C_internal_target_ped->GetPedVelocity_45C920() == kFpZero_678660)
        {
            if (gDistanceToTarget_678750 < kFpThreeEighths_67878C)
            {
                field_168_game_object->SetMaxSpeed_433920(field_1F4);
            }
            else
            {
                field_168_game_object->RegulateVelocity_433970(field_1F0_maybe_max_speed);
            }
        }
        else
        {
            if (field_14C_internal_target_ped->field_148_objective_target_ped == this || field_14C_internal_target_ped->field_14C_internal_target_ped == this)
            {
                field_168_game_object->SetMaxSpeed_433920(field_1F4);
            }
            else
            {
                field_168_game_object->RegulateVelocity_433970(field_1F0_maybe_max_speed);
            }
        }

        if (field_27C_ped_state_2 == ped_state_2::Unknown_9)
        {
            Car_Door_10* pDoor = field_168_game_object->field_84_target_car->GetDoor(field_24C_target_car_door);
            pDoor->Close_439EA0();
            pDoor->set_ped_421380(0);
        }
        Ped::UpdateMovementTowardsTarget_4672E0(gDistanceToTarget_678750, 0);
    }
}

MATCH_FUNC(0x46bd30)
void Ped::WaitOnFoot_46BD30()
{
    if (field_21A_car_state_timer == 0)
    {
        field_226_internal_objective_status = 1;
    }
}

MATCH_FUNC(0x46bd50)
char_type Ped::IsOtherPedEnteringAsDriver_46BD50(Car_BC* pCar)
{
    u8 new_door_idx = 0;
    u8 door_idx = 0;
    while (new_door_idx < (u8)pCar->GetRemap())
    {
        Car_Door_10* pDoor = pCar->GetDoor(door_idx);
        if (pDoor)
        {
            Ped* pDoorPed = pDoor->field_8_pObj;
            if (pDoorPed)
            {
                if (!pDoorPed->field_248_enter_car_as_passenger && pDoorPed != this)
                {
                    return 1;
                }
            }
        }
        door_idx = ++new_door_idx;
    }
    return 0;
}

MATCH_FUNC(0x46bdc0)
void Ped::EnterCarStateMachine_46BDC0()
{
    Car_Door_10* pDoor;
    if (field_16C_car && this->field_154_target_to_enter == field_16C_car)
    {
        this->field_226_internal_objective_status = 1;
        return;
    }

    if (gGarage_48_6FD26C->IsParkingCarAndF3D_434AF0(this->field_154_target_to_enter))
    {
        if (IsField238_45EDE0(2))
        {
            SetObjective2_463830(objectives_enum::no_obj_0, 9999);
            return;
        }
        //goto LABEL_54;
        this->field_226_internal_objective_status = 2;
        return;
    }

    this->field_224 &= ~0x10u;

    if (field_278_ped_state_1 == ped_state_1::immobilized_8)
    {
        return;
    }

    if (gDistanceToTarget_678750 > kFpFour_678680 && this->field_238_ped_type != ped_type::script_created_5 &&
        this->field_28C_threat_reaction != threat_reaction_enum::react_as_emergency_1)
    {
        this->field_226_internal_objective_status = 2;
        return;
    }

    if (!FindUsableCarDoor_467090())
    {
        pDoor = field_154_target_to_enter->GetDoor(this->field_24C_target_car_door);
        pDoor->Close_439EA0();
        if (this->field_27C_ped_state_2 == ped_state_2::ped2_entering_a_car_6)
        {
            Sprite* pCarSprite = this->field_154_target_to_enter->field_50_car_sprite;
            Ang16 ang_to_use = (kAng45_6784FC + pCarSprite->field_0);
            field_168_game_object->HandleGenericImpact_553E00(ang_to_use, kFpPoint05_678634, kFpZero_678660, 1);
        }
        ChangeNextPedState1_45C500(0);
        ChangeNextPedState2_45C540(0);
        return;
    }

    this->field_168_game_object->Set_F84_433900(this->field_154_target_to_enter);
    const char_type isPedKind = IsLawEnforcement_45B4E0();
    Fix16 vel_to_check = kFpPoint2_67856C;
    if (isPedKind)
    {
        vel_to_check = kFpPoint1_678428;
    }

    if (field_154_target_to_enter->GetVelocity_43A4C0() < vel_to_check &&
        (gDistanceToTarget_678750 < kFpFiveThirtySeconds_678778 || this->field_27C_ped_state_2 == ped_state_2::ped2_entering_a_car_6) &&
        field_168_game_object->field_80_sprite_ptr->field_C_sprite_4c_ptr->field_30_boundingBox.OverlapsZ_433560(
            &field_154_target_to_enter->field_50_car_sprite->field_C_sprite_4c_ptr->field_30_boundingBox))
    {
        this->field_21C |= 0x8000000u;
        field_168_game_object->SetMaxSpeedByRef_433920(kFpZero_678438);
        if (field_27C_ped_state_2 == ped_state_2::ped2_staying_14 || field_27C_ped_state_2 == ped_state_2::ped2_following_a_car_4 ||
            field_27C_ped_state_2 == ped_state_2::Unknown_5)
        {
            pDoor = field_154_target_to_enter->GetDoor(field_24C_target_car_door);
            ChangeNextPedState2_45C540(6);
            ChangeNextPedState1_45C500(3);
            if (this->field_25C_internal_objective != 37)
            {
                pDoor->set_ped_421380(this);
            }
            field_154_target_to_enter->ApplyVisualDamage_43A9F0();
        }
    }
    else
    {
        this->field_168_game_object->UseRunOrJumpSpeed_433930();
        if (this->field_27C_ped_state_2 != ped_state_2::ped2_entering_a_car_6)
        {
            if (gDistanceToTarget_678750 > kFpTwo_678658)
            {
                UpdateMovementTowardsTarget_4672E0(gDistanceToTarget_678750, 2);
            }
            else if (!field_168_game_object->field_80_sprite_ptr->field_C_sprite_4c_ptr->field_30_boundingBox.OverlapsZ_433560(
                         &field_154_target_to_enter->field_50_car_sprite->field_C_sprite_4c_ptr->field_30_boundingBox))
            {
                UpdateMovementTowardsTarget_4672E0(gDistanceToTarget_678750, 2);
            }
            else if (gDistanceToTarget_678750 > kFpThreeQuarters_678794 && field_168_game_object->field_58_flags_bf.b0 == 1)
            {
                UpdateMovementTowardsTarget_4672E0(gDistanceToTarget_678750, 2);
            }
            else if (gDistanceToTarget_678750 > kFpOne_678798 && field_168_game_object->field_69_is_colliding_with_sprite != 1)
            {
                UpdateMovementTowardsTarget_4672E0(gDistanceToTarget_678750, 2);
            }
            else
            {
                ChangeNextPedState1_45C500(ped_state_1::entering_car_3);
                ChangeNextPedState2_45C540(ped_state_2::ped2_following_a_car_4);
            }
        }
    }

    if (!this->field_248_enter_car_as_passenger && this->field_27C_ped_state_2 == ped_state_2::ped2_entering_a_car_6)
    {
        if (IsOtherPedEnteringAsDriver_46BD50(field_154_target_to_enter))
        {
            ChangeNextPedState1_45C500(7);
            ChangeNextPedState2_45C540(14);
            pDoor = field_154_target_to_enter->GetDoor(this->field_24C_target_car_door);
            if (pDoor)
            {
                pDoor->set_ped_421380(0);
            }
            //LABEL_54:
            this->field_226_internal_objective_status = 2;
            return;
        }

        if (field_154_target_to_enter->get_driver_4118B0())
        {
            if (field_168_game_object->field_6C_animation_state == 6)
            {
                if (field_168_game_object->field_68_animation_frame == 2 && is_player_41B0A0() &&
                    !field_154_target_to_enter->CanBeEnteredByPed_4451E0(this))
                {
                    SetObjective2_463830(objectives_enum::no_obj_0, 9999);
                    SetObjective(objectives_enum::no_obj_0, 9999);
                }
                else
                {
                    if (field_168_game_object->field_68_animation_frame == 4)
                    {
                        field_168_game_object->field_68_animation_frame = 9;
                        if (field_15C_player)
                        {
                            // Get score/report stolen etc
                            field_15C_player->field_2D4_scores.OnCarHijacked_593240(field_154_target_to_enter);

                            // Is it gang car?
                            const s16 gang_car_model =
                                gGangPool_CA8_67E274->FindGangByCarModel_4BF2F0(field_154_target_to_enter->field_84_car_info_idx);
                            if (gang_car_model != -1)
                            {
                                // Well now they hate you a bit
                                Gang_144* pGang = gGangPool_CA8_67E274->GangByIdx_4BF1C0(gang_car_model);
                                pGang->ApplyKillRespectChange_4BEF70(this->field_15C_player->get_idx_4219D0(), 1u);
                            }
                        }
                    }
                }
            }
        }
    }
}

MATCH_FUNC(0x46c250)
void Ped::ExitCarStateMachine_46C250()
{
    bool bUnknown = 0;
    // Shared by both door paths; the z temporaries are block scoped (this gives the frame layout)
    Fix16 char_x;
    Fix16 char_y;
    this->field_21C |= 0x8000000u;

    if (field_27C_ped_state_2 == ped_state_2::ped2_driving_10)
    {
        if (this->field_16C_car->field_60)
        {
            gHamburger_500_678E30->FreeEntry_474CC0(this->field_16C_car->field_60);
            this->field_16C_car->field_60 = 0;
        }

        if (!this->field_248_enter_car_as_passenger)
        {
            if (this->field_258_objective != objectives_enum::objective_33)
            {
                field_154_target_to_enter->ClearDriver_4407F0();
            }

            if (FindUsableCarDoor_467090())
            {
                field_154_target_to_enter->GetDoorWorldPosition_43B5A0(field_24C_target_car_door, &char_x, &char_y);
                Fix16 zTmp;
                AllocCharB4_45C830(char_x,
                                   char_y,
                                   *gMap_0x370_6F6268->sub_4E4E50(&zTmp,
                                                                  char_x,
                                                                  char_y,
                                                                  this->field_154_target_to_enter->field_50_car_sprite->field_1C_zpos));

                {
                    // SetRemap_433C10 written out (as in StartPedWalking_470200)
                    Char_B4* pB4 = field_168_game_object;
                    u8 remap = field_244_remap;
                    pB4->field_5_remap = remap;
                    if (remap != 0xFF)
                    {
                        pB4->field_80_sprite_ptr->SetRemap(remap);
                    }
                }

                ChangeNextPedState2_45C540(7);
                ChangeNextPedState1_45C500(4);

                this->field_16C_car = 0;
                field_168_game_object->Set_F84_433900(field_154_target_to_enter);
                this->field_154_target_to_enter = 0;
                return;
            }

            {
                Fix16 zpos;
                if (!AllocCharB4_45C830(field_154_target_to_enter->field_50_car_sprite->field_14_xy.x,
                                        field_154_target_to_enter->field_50_car_sprite->field_14_xy.y,
                                        *gMap_0x370_6F6268->sub_4E4E50(&zpos,
                                                                       field_154_target_to_enter->field_50_car_sprite->field_14_xy.x,
                                                                       field_154_target_to_enter->field_50_car_sprite->field_14_xy.y,
                                                                       field_154_target_to_enter->field_50_car_sprite->field_1C_zpos)))
                {
                    FatalError_4A38C0(1, "C:\\Splitting\\Gta2\\Source\\char.cpp", 11894);
                }
            }

            {
                // SetRemap_433C10 written out (as in StartPedWalking_470200)
                Char_B4* pB4 = field_168_game_object;
                u8 remap = field_244_remap;
                pB4->field_5_remap = remap;
                if (remap != 0xFF)
                {
                    pB4->field_80_sprite_ptr->SetRemap(remap);
                }
            }

            ChangeNextPedState2_45C540(0);
            ChangeNextPedState1_45C500(0);

            this->field_168_game_object->SetMaxSpeedByRef_433920(kFpZero_678438);
            field_168_game_object->DoJump_5454D0();
            field_168_game_object->field_80_sprite_ptr->field_0 = field_154_target_to_enter->field_50_car_sprite->field_0;
            this->field_168_game_object->set_rotation_433A30(this->field_154_target_to_enter->field_50_car_sprite->field_0);
            this->field_16C_car = 0;
            this->field_226_internal_objective_status = 1;
            field_168_game_object->Set_F84_433900(field_154_target_to_enter);
            this->field_154_target_to_enter = 0;
        }
        else
        {
            if (FindUsableCarDoor_467090())
            {
                bUnknown = 1;
                Car_Door_10* pDoor = field_154_target_to_enter->GetDoor(field_24C_target_car_door);
                if (!pDoor->get_pObj_4341B0() || this->field_25C_internal_objective == 38)
                {
                    field_16C_car->field_4_passengers_list.RemovePed_471240(this);
                    field_154_target_to_enter->GetDoorWorldPosition_43B5A0(field_24C_target_car_door, &char_x, &char_y);

                    Fix16 zTmp;
                    AllocCharB4_45C830(char_x,
                                       char_y,
                                       *gMap_0x370_6F6268->sub_4E4E50(&zTmp,
                                                                      char_x,
                                                                      char_y,
                                                                      this->field_154_target_to_enter->field_50_car_sprite->field_1C_zpos));
                    {
                        // SetRemap_433C10 written out (as in StartPedWalking_470200)
                        Char_B4* pB4 = field_168_game_object;
                        u8 remap = field_244_remap;
                        pB4->field_5_remap = remap;
                        if (remap != 0xFF)
                        {
                            pB4->field_80_sprite_ptr->SetRemap(remap);
                        }
                    }
                    ChangeNextPedState2_45C540(7);
                    ChangeNextPedState1_45C500(4);
                    this->field_16C_car = 0;
                    field_168_game_object->Set_F84_433900(field_154_target_to_enter);
                    this->field_154_target_to_enter = 0;
                    pDoor->set_ped_421380(this);
                }
            }
            else
            {
                if (this->field_240_occupation == 9)
                {
                    Kill_46F9D0();
                    return;
                }

                field_16C_car->field_4_passengers_list.RemovePed_471240(this);

                {
                    Fix16 zpos;
                    AllocCharB4_45C830(field_154_target_to_enter->field_50_car_sprite->field_14_xy.x,
                                       field_154_target_to_enter->field_50_car_sprite->field_14_xy.y,
                                       *gMap_0x370_6F6268->sub_4E4E50(&zpos,
                                                                      field_154_target_to_enter->field_50_car_sprite->field_14_xy.x,
                                                                      field_154_target_to_enter->field_50_car_sprite->field_14_xy.y,
                                                                      field_154_target_to_enter->field_50_car_sprite->field_1C_zpos));
                }
                {
                    // SetRemap_433C10 written out (as in StartPedWalking_470200)
                    Char_B4* pB4 = field_168_game_object;
                    u8 remap = field_244_remap;
                    pB4->field_5_remap = remap;
                    if (remap != 0xFF)
                    {
                        pB4->field_80_sprite_ptr->SetRemap(remap);
                    }
                }
                ChangeNextPedState2_45C540(0);
                ChangeNextPedState1_45C500(0);
                field_168_game_object->DoJump_5454D0();
                this->field_168_game_object->field_80_sprite_ptr->field_0 = this->field_154_target_to_enter->field_50_car_sprite->field_0;
                this->field_168_game_object->set_rotation_433A30(this->field_154_target_to_enter->field_50_car_sprite->field_0);
                this->field_168_game_object->field_5C = 10;
                this->field_16C_car = 0;
                field_168_game_object->Set_F84_433900(field_154_target_to_enter);
                this->field_154_target_to_enter = 0;
                this->field_226_internal_objective_status = 1;
            }
        }
    }

    if (field_168_game_object)
    {
        if (field_154_target_to_enter && field_154_target_to_enter->IsDespawning_4215B0() ||
            (field_150_target_objective_car = this->field_150_target_objective_car) != 0 && field_150_target_objective_car->IsDespawning_4215B0() ||
            (this->field_158_unk_car) != 0 && field_158_unk_car->IsDespawning_4215B0() ||
            (field_168_game_object->field_84_target_car) != 0 && field_168_game_object->field_84_target_car->IsDespawning_4215B0())
        {
            this->field_278_ped_state_1 = ped_state_1::standing_still_7;
            this->field_27C_ped_state_2 = ped_state_2::ped2_staying_14;
            field_168_game_object->field_6C_animation_state = 2;
            this->field_168_game_object->field_68_animation_frame = 0;
        }
        else if (!FindUsableCarDoor_467090() && !bUnknown)
        {
            DoJump_433C40();
        }

        if (this->field_27C_ped_state_2 != ped_state_2::ped2_getting_out_a_car_7 &&
            this->field_168_game_object->field_6C_animation_state != 7)
        {
            this->field_226_internal_objective_status = 1;
            if (field_164_ped_group)
            {
                this->field_226_internal_objective_status = field_164_ped_group->AreAllMembersOnFoot_4CAB80() != 0;
            }
        }
    }
}

MATCH_FUNC(0x46c770)
void Ped::sub_46C770()
{
    if (field_278_ped_state_1 != ped_state_1::immobilized_8)
    {
        if (field_168_game_object->Get_F44_433A90() == 2 || field_258_objective == objectives_enum::enter_car_as_driver_35 ||
            gDistanceToTarget_678750 < kFpHalf_678790)
        {
            Ped::SetObjective2_463830(objectives_enum::no_obj_0, 9999);
            field_21C_bf.b2 = false;
        }
        else
        {
            Ped::UpdateMovementTowardsTarget_4672E0(gDistanceToTarget_678750, 1);
            field_168_game_object->SetMaxSpeedByRef_433920(kFpOneSixteenth_678448);
        }
    }
}

MATCH_FUNC(0x46c7e0)
void Ped::GotoAreaOnFoot_46C7E0()
{
    if (field_278_ped_state_1 != ped_state_1::immobilized_8)
    {
        if (gDistanceToTarget_678750 < kFpOneEighth_6784E8)
        {
            if (field_168_game_object->GetCharState_433A80() != Char_B4_state::Jumping_15)
            {
                Ped::ChangeNextPedState1_45C500(ped_state_1::standing_still_7);
                Ped::ChangeNextPedState2_45C540(ped_state_2::ped2_staying_14);
                field_226_internal_objective_status = 1;
            }
        }
        else
        {
            if (!field_21A_car_state_timer)
            {
                field_226_internal_objective_status = 1;
            }
            if (field_240_occupation != ped_ocupation_enum::train_customer_9 && field_258_objective != objectives_enum::patrol_on_foot_42)
            {
                field_168_game_object->SetMaxSpeed_433920(field_1F0_maybe_max_speed);
            }
            else
            {
                field_168_game_object->SetMaxSpeed_433920(field_1F4);
            }
        }
        Ped::UpdateMovementTowardsTarget_4672E0(gDistanceToTarget_678750, 1);
    }
}

MATCH_FUNC(0x46c8a0)
void Ped::sub_46C8A0()
{
    if (field_278_ped_state_1 != ped_state_1::immobilized_8)
    {
        field_168_game_object->SetMaxSpeedByRef_433920(kFpOneSixteenth_678448);
        if (gDistanceToTarget_678750 < kFpHalf_678790)
        {
            Ped::ChangeNextPedState1_45C500(ped_state_1::standing_still_7);
            Ped::ChangeNextPedState2_45C540(ped_state_2::ped2_staying_14);
            field_226_internal_objective_status = 1;
        }
        else
        {
            field_230 = 2;
            Ped::UpdateMovementTowardsTarget_4672E0(gDistanceToTarget_678750, 2);
            field_168_game_object->SetMaxSpeedByRef_433920(kFpOneSixteenth_678448);
        }
    }
}

MATCH_FUNC(0x46c910)
void Ped::FollowPathPoints_46C910()
{
    if (field_278_ped_state_1 != ped_state_1::immobilized_8)
    {
        if (gDistanceToTarget_678750 < kFpOneEighth_6784E8)
        {
            field_18C_current_path_point = field_18C_current_path_point + 1; // next patrol point
            field_1C4_x = kFpHalf_67853C + Fix16(field_18C_current_path_point->field_0_x);
            field_1C8_y = kFpHalf_67853C + Fix16(field_18C_current_path_point->field_1_y);
            if (field_18C_current_path_point->field_0_x == 0)
            {
                field_226_internal_objective_status = 1;
                Ped::SetObjective2_463830(26, 9999);
            }
        }
        else
        {
            Ped::ChangeNextPedState1_45C500(ped_state_1::findind_path_2);
            Ped::ChangeNextPedState2_45C540(ped_state_2::ped2_walking_0);
            field_168_game_object->SetMaxSpeedByRef_433920(kFpOneSixteenth_678448);
        }
    }
}

MATCH_FUNC(0x46c9b0)
void Ped::CrossRoad_46C9B0()
{
    if (field_278_ped_state_1 != ped_state_1::immobilized_8)
    {
        if (gDistanceToTarget_678750 < kFpOneEighth_6784E8)
        {
            Ped::ChangeNextPedState1_45C500(ped_state_1::walking_0);
            Ped::ChangeNextPedState2_45C540(ped_state_2::ped2_walking_0);
            field_226_internal_objective_status = 0;
            Ped::SetObjective2_463830(49, 100);
            if (--gNumPedsCrossingRoad_6787D0 < 0)
            {
                gNumPedsCrossingRoad_6787D0 = 0;
            }
        }
        else
        {
            if (field_21A_car_state_timer == 0)
            {
                field_226_internal_objective_status = 1;
            }
            if (gTrafficLights_194_705958->field_192_phase == 7)
            {
                field_168_game_object->SetMaxSpeedByRef_433920(kFpOneThirtySecond_678434);
            }
            else
            {
                if (field_168_game_object->Get_F44_433A90() == 1)
                {
                    field_168_game_object->SetMaxSpeedByRef_433920(kFpOneSixteenth_678448);
                }
                else
                {
                    field_168_game_object->SetMaxSpeedByRef_433920(kFpOneThirtySecond_678434);
                }
            }
        }
    }
}

MATCH_FUNC(0x46ca60)
void Ped::sub_46CA60()
{
}

MATCH_FUNC(0x46ca70)
void Ped::FollowPedInCar_46CA70()
{
    if (!this->field_16C_car->field_60)
    {
        this->field_16C_car->field_60 = gHamburger_500_678E30->AllocateEntry_474810();
        this->field_16C_car->field_60->field_4_ped_owner = this;
    }

    if (this->field_258_objective == objectives_enum::kill_char_any_means_19)
    {
        field_16C_car->field_60->field_8_maybe_path_type = 5;
        if ((field_21C & 0x80u) != 0)
        {
            this->field_21C |= 0x800;
        }
    }
    else
    {
        field_16C_car->field_60->field_8_maybe_path_type = 2;
    }

    this->field_16C_car->SetUniNum_421560(5);
    this->field_16C_car->field_60->field_30_ped_to_follow = this->field_14C_internal_target_ped;
    this->field_16C_car->ClearA6Bit5_421550();
    this->field_16C_car->field_5C_AI->field_74_unk_speed = kFpThree_67866C;
    this->field_16C_car->field_60->field_20 = 1;
}

// 9.6f 0x43A550
MATCH_FUNC(0x46cb30)
void Ped::StartPedCrossingAtTrafficLight_Y_Backward_46CB30()
{
    Fix16 y_iter = field_1AC_cam.y;
    if (field_278_ped_state_1 != ped_state_1::immobilized_8 && gTrafficLights_194_705958->is_phase_7_434960())
    {
        for (u8 i = 0; i < 6; i++)
        {
            y_iter -= kFpOne_678664;
            u8 ypos = y_iter.ToInt();
            if (gMap_0x370_6F6268->IsBlockPavementTypeInlined_433530(field_1AC_cam.x.ToInt(),
                                                                     ypos,
                                                                     (field_1AC_cam.z - kFpOne_678664).ToInt()))
            {
                SetObjective2_463830(objectives_enum::objective_48, 9999);
                Fix16 t = Fix16(ypos) + kFpHalf_67853C;
                Set_F1C4_x_433C50(field_1AC_cam.x);
                Set_F1C8_y_433C60(t);
                Set_F1CC_z_433C70(field_1AC_cam.z);
                break;
            }
        }
    }
}

MATCH_FUNC(0x46cc70)
void Ped::StartPedCrossingAtTrafficLight_X_Forwards_46CC70()
{
    Fix16 x_iter = field_1AC_cam.x;
    if (field_278_ped_state_1 != ped_state_1::immobilized_8 && gTrafficLights_194_705958->is_phase_7_434960())
    {
        for (u8 i = 0; i < 6; i++)
        {
            x_iter += kFpOne_678664;
            u8 x = x_iter.ToUInt8();
            if (gMap_0x370_6F6268->IsBlockPavementTypeInlined_433530(x,
                                                                     field_1AC_cam.y.ToInt(),
                                                                     (field_1AC_cam.z - kFpOne_678664).ToInt()))
            {
                Ped::SetObjective2_463830(objectives_enum::objective_48, 9999);
                Fix16 xpos(x);
                xpos += kFpHalf_67853C;
                Set_F1C4_x_433C50(xpos);
                Set_F1C8_y_433C60(field_1AC_cam.y);
                Set_F1CC_z_433C70(field_1AC_cam.z);
                break;
            }
        }
    }
}

MATCH_FUNC(0x46cdb0)
void Ped::StartPedCrossingAtTrafficLight_Y_Forwards_46CDB0()
{
    Fix16 y_iter = field_1AC_cam.y;
    if (field_278_ped_state_1 != ped_state_1::immobilized_8 && gTrafficLights_194_705958->is_phase_7_434960())
    {
        for (u8 i = 0; i < 6; i++)
        {
            y_iter += kFpOne_678664;
            u8 y = y_iter.ToUInt8();
            if (gMap_0x370_6F6268->IsBlockPavementTypeInlined_433530(field_1AC_cam.x.ToInt(),
                                                                     y,
                                                                     (field_1AC_cam.z - kFpOne_678664).ToInt()))
            {
                Ped::SetObjective2_463830(48, 9999);
                Fix16 ypos(y);
                ypos += kFpHalf_67853C;
                Set_F1C4_x_433C50(field_1AC_cam.x);
                Set_F1C8_y_433C60(ypos);
                Set_F1CC_z_433C70(field_1AC_cam.z);
                break;
            }
        }
    }
}

MATCH_FUNC(0x46cef0)
void Ped::StartPedCrossingAtTrafficLight_X_Backwards_46CEF0()
{
    Fix16 x_iter = field_1AC_cam.x;
    if (field_278_ped_state_1 != ped_state_1::immobilized_8 && gTrafficLights_194_705958->is_phase_7_434960())
    {
        for (u8 i = 0; i < 6; i++)
        {
            x_iter -= kFpOne_678664;
            u8 x = x_iter.ToUInt8();
            if (gMap_0x370_6F6268->IsBlockPavementTypeInlined_433530(x,
                                                                     field_1AC_cam.y.ToInt(),
                                                                     (field_1AC_cam.z - kFpOne_678664).ToInt()))
            {
                Ped::SetObjective2_463830(48, 9999);
                Fix16 xpos(x);
                xpos += kFpHalf_67853C;
                Set_F1C4_x_433C50(xpos);
                Set_F1C8_y_433C60(field_1AC_cam.y);
                Set_F1CC_z_433C70(field_1AC_cam.z);
                break;
            }
        }
    }
}

MATCH_FUNC(0x46d030)
void Ped::WaitForTrain_46D030()
{
    if (field_278_ped_state_1 != ped_state_1::immobilized_8)
    {
        Train_58* pTrain = gPublicTransport_181C_6FF1D4->GetTrainFromCarExcludingLeadCar_57B6A0(field_154_target_to_enter);
        Car_BC* pOldTarget = field_154_target_to_enter;
        if (pTrain->field_4C_maybe_train_station->field_1C == 2 &&
            pTrain->field_C_carriages[1]->GetCarInfoIdx_411940() == car_model_enum::TRAIN)
        {
            Ped::SetObjective2_463830(37, 9999);
            set_target_to_enter_403B00(pOldTarget);
            field_168_game_object->Set_F84_433900(pOldTarget);
            field_168_game_object->SetMaxSpeedByRef_433920(kFpOneSixteenth_678448);
        }
    }
}

MATCH_FUNC(0x46d0b0)
void Ped::sub_46D0B0()
{
    if (field_21A_car_state_timer == 0)
    {
        field_226_internal_objective_status = 1;
    }
}

MATCH_FUNC(0x46d0d0)
void Ped::EnterTrainStateMachine_46D0D0()
{
    s32 state1;
    Fix16 best_dist;
    u8 new_door_idx;
    Fix16 door_xd;
    Fix16 door_yd;
    Fix16 dist;
    Char_B4* pB4;
    bool found_door;
    u8 best_door;
    u8 door_idx;
    Fix16 door_x;
    Fix16 door_y;
    Fix16 abs_xd;
    Fix16 abs_yd;

    found_door = 0;
    state1 = field_278_ped_state_1;
    field_21C |= 0x8000000u;
    if (state1 != ped_state_1::immobilized_8)
    {
        if (gDistanceToTarget_678750 > kFpFour_678680)
        {
            SetObjective2_463830(objectives_enum::no_obj_0, 9999);
            field_226_internal_objective_status = 2;
        }
        else
        {
            if (!gPublicTransport_181C_6FF1D4->GetTrainFromCarExcludingLeadCar_57B6A0(field_154_target_to_enter))
            {
                field_248_enter_car_as_passenger = 0;
            }
            else
            {
                field_248_enter_car_as_passenger = 1;
            }
            best_dist = kFp9999_678524;
            best_door = 0;
            new_door_idx = 0;
            door_idx = new_door_idx;
            while (new_door_idx < (u8)field_154_target_to_enter->GetRemap())
            {
                if (field_154_target_to_enter->IsDoorAccessible_43AFE0(door_idx))
                {
                    field_154_target_to_enter->GetDoorWorldPosition_43B5A0(door_idx, &door_x, &door_y);
                    door_xd = door_x - field_1AC_cam.x;
                    door_yd = door_y - field_1AC_cam.y;
                    abs_xd = Fix16::Abs(door_xd);
                    abs_yd = Fix16::Abs(door_yd);
                    dist = Fix16::Max(abs_xd, abs_yd);
                    if (dist < best_dist)
                    {
                        best_dist = dist;
                        best_door = new_door_idx;
                        found_door = 1;
                    }
                }
                door_idx = ++new_door_idx;
            }
            if (found_door == 1)
            {
                field_24C_target_car_door = best_door;
                pB4 = field_168_game_object;
                pB4->field_38_velocity = pB4->field_3C_run_or_jump_speed;
                EnterCarStateMachine_46BDC0();
                if (field_226_internal_objective_status == 1)
                {
                    Deallocate_45EB60();
                }
            }
        }
    }
}

MATCH_FUNC(0x46d240)
void Ped::ExitTrainStateMachine_46D240()
{
    bool door_available;
    volatile bool found_usable_door; // NOTE: volatile keeps the flag in its stack slot like OG

    door_available = 1;
    field_248_enter_car_as_passenger = 1;
    field_21C |= 0x8000000;
    if (field_27C_ped_state_2 == ped_state_2::ped2_driving_10)
    {
        found_usable_door = 0;
        u8 attempt = 0;
        while (1)
        {
            attempt++;
            if (field_154_target_to_enter->IsStoppedWithPavementAtDoor_43B140(field_24C_target_car_door))
            {
                found_usable_door = 1;
                break;
            }
            field_24C_target_car_door++;
            if (field_24C_target_car_door > 3u)
            {
                field_24C_target_car_door = 0;
            }
            if (attempt >= 5)
            {
                break;
            }
        }
        door_available = found_usable_door;
    }

    if (field_226_internal_objective_status == 1 || !door_available)
    {
        if (IsField238_45EDE0(2))
        {
            SetObjective(objectives_enum::no_obj_0, 9999);
            SetObjective2_463830(objectives_enum::no_obj_0, 9999);
        }
    }
    else
    {
        ExitCarStateMachine_46C250();
    }
}

MATCH_FUNC(0x46d300)
void Ped::FollowCarOnFoot_46D300()
{
    if (this->field_278_ped_state_1 != ped_state_1::immobilized_8)
    {
        if (gDistanceToTarget_678750 <= kFpQuarter_678788)
        {
            if (this->field_168_game_object->GetCharState_433A80() != 15)
            {
                if (gDistanceToTarget_678750 < kFpOneEighth_6784E8)
                {
                    if (field_150_target_objective_car->GetVelocity_43A4C0() <= GetPedVelocity_45C920())
                    {
                        this->field_168_game_object->SetMaxSpeed_433920(field_150_target_objective_car->GetVelocity_43A4C0());
                    }
                }
                else
                {
                    this->field_168_game_object->RegulateVelocity_433970(field_150_target_objective_car->GetVelocity_43A4C0() + kFpOne64th_678430);
                }
            }
        }
        else if (gDistanceToTarget_678750 < kFpHalf_678790)
        {
            this->field_168_game_object->RegulateVelocity_433970(field_150_target_objective_car->GetVelocity_43A4C0() + kFpOne64th_678430);
        }
        else
        {
            this->field_168_game_object->IncreaseSpeedIfAllowed_433940();
        }
        UpdateMovementTowardsTarget_4672E0(gDistanceToTarget_678750, 1);
    }
}


// https://decomp.me/scratch/5y8iN
WIP_FUNC(0x46d460)
void Ped::AttackTargetStateMachine_46D460(u8 targetType)
{
    WIP_IMPLEMENTED;
    Fix16 v6;

    u8 v40 = 0;
    u8 v41 = 0;
    u8 v3 = 0;
    u8 v39 = 0;

    field_21C_bf.b13 = false;
    Weapon_30* pWeapon = Ped::ChooseAttackWeapon_46F490();
    Fix16 v5 = kFpOne_678664;
    Fix16 v42 = kFpOne_678798 + kFpQuarter_678788;
    if (field_168_game_object)
    {
        if (pWeapon)
        {
            switch (pWeapon->field_1C_idx)
            {
                case weapon_type::flamethrower:
                    v6 = kFpTwo_678658;
                    v42 = kFpOne_678798;
                    break;

                case weapon_type::molotov:
                    v6 = kFpTwo_678658 * 2;
                    v42 = kFpOne_678798 + kFpHalf_678790;
                    v40 = 1;
                    break;

                case weapon_type::grenade:
                    v6 = kFpHalf_678790 + kFpTwo_678658;
                    v42 = kFpTwo_678658;
                    v40 = 1;
                    break;

                case weapon_type::electro_batton:
                    v5 = kFpHalf_67853C;
                    v6 = kFpTwo_678658;
                    v41 = 1;
                    break;

                case weapon_type::shotgun:
                    v5 = kFpHalf_67853C;
                    v6 = kFpOne_678798;
                    v41 = 1;
                    break;

                default:
                    v6 = kFpFour_678680;
                    break;
            }
        }
        else
        {
            v6 = kFpTwo_678658; // no weapon
        }
    }
    else
    {
        v6 = kFpFour_678680; // probably in a car
    }

    field_21C_bf.b15 = false;
    Fix16 zpos;

    switch (targetType)
    {
        case 0: // target is a ped
            zpos = field_14C_internal_target_ped->get_cam_z();
            if (field_14C_internal_target_ped->isDead_403B60() || !field_14C_internal_target_ped->CheckBit0_433B40())
            {
                v3 = 1;
            }
            if (field_14C_internal_target_ped->get_car_416B60())
            {
                if (field_14C_internal_target_ped->field_16C_car->GetVelocity_43A4C0() <= kFpPoint02_678630)
                {
                    v6 = kFpTwo_678668;
                    v5 = kFpTwo_678668 + kFpHalf_67853C;
                    v39 = 1;
                }
            }

            break;

        case 1: // target is a car
            zpos = field_154_target_to_enter->get_z_41E450();
            if (field_154_target_to_enter->IsMaxDamage_40F890())
            {
                v3 = 1;
            }
            v5 = kFpThree_67866C;
            v6 = kFpTwo_678668 + kFpHalf_67853C;
            v39 = 1;
            break;

        case 2: // target is a object
            zpos = field_1A4_internal_target_object->get_z_4340F0();
            if (field_1A4_internal_target_object->IsDestroyedPowergen_434140())
            {
                field_226_internal_objective_status = 1;
            }
            if (field_240_occupation != ped_ocupation_enum::stand_still_bloke)
            {
                v5 = kFpThree_67866C;
                v6 = kFpTwo_678668 + kFpHalf_67853C;
                v39 = 1;
            }
            break;
    }

    if (v3)
    {
        field_21C_bf.b11 = false;
        field_226_internal_objective_status = 1;
        if (field_258_objective == objectives_enum::no_obj_0)
        {
            Ped::SetObjective2_463830(0, 9999);
            field_21C_bf.b2 = false;
        }
        return;
    }

    if (field_278_ped_state_1 == ped_state_1::immobilized_8)
    {
        return;
    }

    if (targetType == 0) // target is a ped
    {
        field_14C_internal_target_ped->SetAttacker_433BF0(this);
        field_14C_internal_target_ped->Increment_F262_433BD0();
    }

    if (field_198) // line 185
    {
        Ped::ChangeNextPedState1_45C500(7);
        Ped::ChangeNextPedState2_45C540(11);
        field_21C_bf.b11 = true;
    }
    else
    {
        if (field_240_occupation == ped_ocupation_enum::stand_still_bloke) // line 294
        {
            pWeapon->Set_F4_433810(0);
        }

        if (field_266 < 4 && gDistanceToTarget_678750 < v6 && abs_sub_less_than_epislon_45AE40(field_1AC_cam.z, zpos) ||
            field_21C_bf.b22 == true)
        {
            // correct
            if (pWeapon) // correct
            {
                if (pWeapon->Get_F4_41CC70() == 1) // correct
                {
                    // weapon->field_4 == 1
                    if (gDistanceToTarget_678750 < v5 && !field_168_game_object->IsCollidingWithASprite_433AA0())
                    {
                        Ped::ChangeNextPedState1_45C500(1);
                        Ped::ChangeNextPedState2_45C540(2);
                        field_21C_bf.b11 = true;
                        byte_6787D4 = 1;
                        field_168_game_object->RegulateVelocity_433970(field_1F0_maybe_max_speed);

                        if ((field_168_game_object->field_58_flags & 0x80) != 0) // line 375
                        {
                            Ped::SetObjective2_463830(1, 9999); // line 462
                            field_1C4_x = field_1AC_cam.x;
                            field_1C8_y = field_1AC_cam.y;
                        }
                        return;
                    }

                    if (field_21C_bf.b22 == false)
                    {
                        Ped::ChangeNextPedState1_45C500(1);
                        Ped::ChangeNextPedState2_45C540(2);
                        field_21C_bf.b11 = true;
                        field_168_game_object->RegulateVelocity_433970(field_1F0_maybe_max_speed);
                        return;
                    }
                    else
                    {
                        //goto GetBlockTypeAtCoord_420420
                    }
                }
                else
                {
                    if (pWeapon->IsExplosiveWeapon_5E3BD0() && gDistanceToTarget_678750 < v42)
                    {
                        if (field_174_pWeapon)
                        {
                            field_21C_bf.b11 = true;
                            field_21C_bf.b13 = true;
                        }
                        else
                        {
                            field_21C_bf.b11 = false;
                        }

                        if (field_21C_bf.b22 == false)
                        {
                            if ((field_168_game_object->field_58_flags & 0x80) != 0) // line 45c
                            {
                                Ped::SetObjective2_463830(1, 9999); // line 384
                                field_1C4_x = field_1AC_cam.x;
                                field_1C8_y = field_1AC_cam.y;
                            }
                            else
                            {
                                if (field_12C == field_130)
                                {
                                    field_168_game_object->SetMaxSpeed_433920(-field_1F4);
                                }
                                Ped::ChangeNextPedState1_45C500(1);
                                Ped::ChangeNextPedState2_45C540(2);
                            }
                        }
                        return;
                    }
                    // no else here
                }

                // line 4c9 and 4d2

                if (v40)
                {
                    if (gMap_0x370_6F6268->GetBlockTypeAtCoord_420420(
                            (field_168_game_object->field_80_sprite_ptr->field_14_xy.x).ToInt(),
                            (field_168_game_object->field_80_sprite_ptr->field_14_xy.y).ToInt(),
                            (field_168_game_object->field_80_sprite_ptr->field_1C_zpos + kFpOne_678664).ToInt()) != AIR)
                    {
                        Ped::ChangeNextPedState1_45C500(1);
                        Ped::ChangeNextPedState2_45C540(2);
                        field_21C_bf.b11 = false;
                        field_168_game_object->RegulateVelocity_433970(field_1F0_maybe_max_speed);
                    }
                }

                if ((field_240_occupation == ped_ocupation_enum::fbi || field_23C_group_idx == 99 || v41) &&
                    field_240_occupation != ped_ocupation_enum::stand_still_bloke && !v39)
                {
                    if (gDistanceToTarget_678750 < kFpHalf_67853C)
                    {
                        if (field_168_game_object->GetCharState_433A80() != 15) // jumping
                        {
                            Ped::ChangeNextPedState1_45C500(7); // line 5b5
                            Ped::ChangeNextPedState2_45C540(11);
                            field_21C_bf.b11 = true;
                        }
                        else
                        {
                            field_21C_bf.b11 = false;
                        }
                    }
                    else
                    {
                        field_21C_bf.b11 = true;
                    }
                }
                else if (field_168_game_object->GetCharState_433A80() != 15) // line 5fe
                {
                    Ped::ChangeNextPedState1_45C500(7);
                    Ped::ChangeNextPedState2_45C540(11);
                    field_21C_bf.b11 = true;
                    // 9.6f: Char_B4::SetMaxSpeed_433920 (inlined, using it makes the diff worse)
                    field_168_game_object->field_38_velocity = kFpZero_678438;
                }
                else
                {
                    field_21C_bf.b11 = false; // something weird here
                }
            }
            else
            {
                // no weapon
                Ped::MeleeAttackStateMachine_46B670();
            }
        }
        else
        {
            switch (targetType)
            {
                case 0:  // target is a ped
                    Ped::UpdateMovementTowardsTarget_4672E0(gDistanceToTarget_678750, 0);
                    break;
                case 1:  // target is a car
                    Ped::UpdateMovementTowardsTarget_4672E0(gDistanceToTarget_678750, 2);
                    break;
                case 2:  // target is a object
                    Ped::UpdateMovementTowardsTarget_4672E0(gDistanceToTarget_678750, 6);
                    break;
            }

            if (gDistanceToTarget_678750 > kFpEight_6786C0)
            {
                field_21C_bf.b11 = false;
            }
            field_168_game_object->RegulateVelocity_433970(field_1F0_maybe_max_speed);
        }
    }
}

MATCH_FUNC(0x46db60)
void Ped::AttackPed_46DB60()
{
    AttackTargetStateMachine_46D460(0);
}

MATCH_FUNC(0x46db70)
void Ped::AttackCar_46DB70()
{
    AttackTargetStateMachine_46D460(1);
}

MATCH_FUNC(0x46db80)
void Ped::AttackObject_46DB80()
{
    AttackTargetStateMachine_46D460(2);
}

MATCH_FUNC(0x46df50)
Sprite* Ped::GetSprite_46DF50()
{
    Car_BC* pBC = this->field_16C_car;
    if (pBC)
    {
        return pBC->field_50_car_sprite;
    }
    else
    {
        return this->field_168_game_object->field_80_sprite_ptr;
    }
}

MATCH_FUNC(0x46df70)
void Ped::SetupFollower_46DF70(Ped* pToFollow, s32 weaponIdx)
{
    set_remap_433B90(pToFollow->field_244_remap);
    SetRemap_433C10(pToFollow->field_244_remap);
    field_26C_graphic_type = pToFollow->field_26C_graphic_type;
    Ped::RemovePedWeapons_462510();
    Ped::ForceWeapon_46F600(weaponIdx);
    set_occupation_403970(ped_ocupation_enum::special_groups_member);
    field_288_threat_search = pToFollow->field_288_threat_search;
    field_28C_threat_reaction = pToFollow->field_28C_threat_reaction;
    field_17C_pGang = pToFollow->field_17C_pGang;
    sub_433BB0(1);
    sub_433BC0(1);
    SetField238_403920(4);
}

MATCH_FUNC(0x46e020)
bool Ped::CanBeRecruitedToGroup_46E020(PedGroup* pGroup)
{
    return this->field_164_ped_group != pGroup && !this->field_15C_player &&
            (IsField238_45EDE0(3) || (IsField238_45EDE0(4) || IsField238_45EDE0(6)) && this->field_240_occupation == 35) ?
        true :
        false;
}

// 9.6f: Fix16_Rect::ComputeCollisionPrism_4204D0(x, y, offset, z), inlined. The 10.5 version uses
// kFpOneEighth_67845C (the Fix16_Rect.hpp one uses kCollisionPrismHalfHeight_6771E4), inline x/y
// arithmetic and the out-of-line Subtract_436A00/operator+ for z. The by-value z parameter is what
// puts z in desiredCount's dead stack slot.
static inline void ComputeRecruitPrism(Fix16_Rect& r, Fix16 x, Fix16 y, Fix16 offset, Fix16 z)
{
    s32 half = offset.mValue / 2;
    r.field_0_left.mValue = x.mValue - half;
    r.field_4_right.mValue = half + x.mValue;
    r.field_8_top.mValue = y.mValue - half;
    r.field_C_bottom.mValue = y.mValue + half;
    r.field_10_low_z = z.Subtract_436A00(kFpOneEighth_67845C);
    r.field_14_high_z = (z).Add_408660(kFpOneEighth_67845C);
}

MATCH_FUNC(0x46e080)
void Ped::RecruitNearbyPeds_46E080(s32 desiredCount, Fix16 searchRadius)
{
    PedGroup* pGroup_; // ecx
    s32 maxCount; // ebx
    Sprite* pNearest; // eax
    Char_B4* pB4; // eax
    Ped* pPed; // edi
    PedGroup* pGroup; // ecx
    struct_4 collision_list; // [esp+8h] [ebp-1Ch] BYREF
    Fix16_Rect rect; // [esp+Ch] [ebp-18h] BYREF

    pGroup_ = this->field_164_ped_group;
    if (pGroup_)
    {
        maxCount = desiredCount;
        // max(desiredCount, 9)
        if (desiredCount > 9)
        {
            maxCount = 9;
        }

        if (pGroup_->field_34_count >= maxCount)
        {
            return;
        }
    }
    else
    {
        SpawnPedGroupFollowers_46E200(0);
        maxCount = desiredCount;
    }

    ComputeRecruitPrism(rect, this->field_1AC_cam.x, this->field_1AC_cam.y, searchRadius, this->field_1AC_cam.z);
    if (gPurpleDoom_1_679208->CollectRectCollisions_477F30(&rect, 0, 0, GetSprite_46DF50(), &collision_list))
    {
        for (pNearest = collision_list.TakeClosestSprite_5A6EA0(this->field_1AC_cam.x, this->field_1AC_cam.y); pNearest;
             pNearest = collision_list.TakeClosestSprite_5A6EA0(this->field_1AC_cam.x, this->field_1AC_cam.y))
        {
            pB4 = pNearest->AsCharB4_40FEA0();

            if (pB4)
            {
                pPed = pB4->get_ped_433A20();
                if (pPed->CanBeRecruitedToGroup_46E020(this->field_164_ped_group))
                {
                    pGroup = pPed->field_164_ped_group;
                    if (pGroup)
                    {
                        pGroup->RemovePed_4C9970(pPed);
                    }
                    field_164_ped_group->add_ped_to_end_of_list_4C8F90(pPed);
                    pPed->SetupFollower_46DF70(this, weapon_type::dual_pistol);
                    if (this->field_164_ped_group->field_34_count == maxCount)
                    {
                        break;
                    }
                }
            }
        }
    }
    collision_list.ClearList_5A6E10();
}

MATCH_FUNC(0x46e200)
void Ped::SpawnPedGroupFollowers_46E200(u8 total)
{
    PedGroup* pGroup = PedGroup::New_4CB0D0();
    pGroup->add_ped_leader_4C9B10(this);
    u8 current = 0;
    pGroup->SetCounts_433360(total);
    if (total > 0)
    {
        s32 i = 0;
        do
        {

            Ped* pNewPed = gPedPool_6787B8->Allocate();

            pNewPed->set_occupation_403970(this->field_240_occupation);
            pNewPed->set_remap_433B90(this->field_244_remap);
            pNewPed->field_26C_graphic_type = this->field_26C_graphic_type;
            pNewPed->SetField238_403920(this->field_238_ped_type);
            Fix16 xy_off = kFpOneSixth_678504 * Fix16(i);
            pNewPed->AllocCharB4_45C830(xy_off + this->field_1AC_cam.x, xy_off + this->field_1AC_cam.y, this->field_1AC_cam.z);
            Char_B4* pB4 = pNewPed->field_168_game_object;
            const u8 remap = this->field_244_remap;
            pB4->field_5_remap = field_244_remap;
            if (remap != 0xFF)
            {
                pB4->field_80_sprite_ptr->SetRemap(remap);
            }
            pNewPed->set_health_4039A0(this->field_216_health);
            pNewPed->sub_433BB0(this->field_230);
            pNewPed->sub_433BC0(this->field_22C);
            pNewPed->field_288_threat_search = this->field_288_threat_search;
            pNewPed->field_28C_threat_reaction = this->field_28C_threat_reaction;
            pNewPed->field_17C_pGang = this->field_17C_pGang;
            pGroup->add_ped_to_list_4C9B30(pNewPed, current);

            Weapon_30* pWeapon = this->field_170_selected_weapon;
            if (pWeapon)
            {
                pNewPed->ForceWeapon_46F600(pWeapon->field_1C_idx);
            }

            ++current;
            ++i;
        } while (current < total);
    }
}

MATCH_FUNC(0x46ef00)
u8 Ped::get_wanted_star_count_46EF00()
{
    short cVar1 = field_20A_wanted_points;
    if (cVar1 < cop_level_ped_enum::cop_6_stars)
    {
        if (cVar1 < cop_level_ped_enum::cop_5_stars)
        {
            if (cVar1 < cop_level_ped_enum::cop_4_stars)
            {
                if (cVar1 < cop_level_ped_enum::cop_3_stars)
                {
                    if (cVar1 < cop_level_ped_enum::cop_2_stars)
                    {
                        if (cVar1 < cop_level_ped_enum::cop_1_stars)
                        {
                            return cop_level_enum::cops_0;
                        }
                        else
                        {
                            return cop_level_enum::cops_1;
                        }
                    }
                    else
                    {
                        return cop_level_enum::cops_2;
                    }
                }
                else
                {
                    return cop_level_enum::cops_3;
                }
            }
            else
            {
                return cop_level_enum::cops_4;
            }
        }
        else
        {
            return cop_level_enum::cops_5;
        }
    }
    else
    {
        return cop_level_enum::cops_6;
    }
}

MATCH_FUNC(0x46ef40)
void Ped::set_wanted_level_46EF40(u16 wanted)
{
    switch (wanted)
    {
        case 0u:
            field_20A_wanted_points = 0;
            break;

        case 600u:
            field_20A_wanted_points = 600u;
            break;

        case 1600u:
            field_20A_wanted_points = 1600u;
            break;

        case 3000u:
            field_20A_wanted_points = 3000;
            break;

        case 5000u:
            field_20A_wanted_points = 5000;
            break;
        case 8000u:
            field_20A_wanted_points = 8000;
            break;
        case 12000u:
            field_20A_wanted_points = 12000;
            break;
    }
}

MATCH_FUNC(0x46EFD0)
void Ped::IncreaseWantedLevelFromDebugKeys_46EFD0()
{
    switch (get_wanted_star_count_46EF00())
    {
        case 0u:
            set_wanted_level_46EF40(600u);
            break;
        case 1u:
            set_wanted_level_46EF40(1600u);
            break;
        case 2u:
            set_wanted_level_46EF40(3000u);
            break;
        case 3u:
            set_wanted_level_46EF40(5000u);
            break;
        case 4u:
            set_wanted_level_46EF40(8000u);
            break;
        case 5u:
            set_wanted_level_46EF40(12000u);
            break;
        case 6u:
            set_wanted_level_46EF40(0);
            break;
        default:
            break;
    }

    u8 stars = get_wanted_star_count_46EF00();
    u8 max_stars = gPolice_7B8_6FEE40->field_660_wanted_star_count;
    if (stars > max_stars)
    {
        set_wanted_star_count_46F070(max_stars);
    }
}

MATCH_FUNC(0x46f070)
void Ped::set_wanted_star_count_46F070(u8 star_count)
{
    switch (star_count)
    {
        case 0:
            field_20A_wanted_points = 0;
            break;
        case 1:
            field_20A_wanted_points = 600;
            break;
        case 2:
            field_20A_wanted_points = 1600;
            break;
        case 3:
            field_20A_wanted_points = 3000;
            break;
        case 4:
            field_20A_wanted_points = 5000;
            break;
        case 5:
            field_20A_wanted_points = 8000;
            break;
        case 6:
            field_20A_wanted_points = 12000;
            break;
        default:
            return;
    }
}

MATCH_FUNC(0x46f100)
bool Ped::WantedStartCountLessThan_46F100(u8 a2)
{
    return a2 < get_wanted_star_count_46EF00();
}

MATCH_FUNC(0x46f110)
Weapon_30* Ped::GetWeaponFromPed_46F110()
{
    if (IsField238_45EDE0(2))
    {
        field_170_selected_weapon = field_15C_player->GetCurrPlayerWeapon_5648F0();

        if (field_16C_car)
        {
            if (field_170_selected_weapon && field_170_selected_weapon->field_1C_idx >= weapon_type::car_bomb)
            {
                field_21C |= 0x80;
                return field_170_selected_weapon;
            }
        }
        else
        {
            if (!field_170_selected_weapon)
            {
                field_21C |= 0x200;
            }
            else
            {
                return field_170_selected_weapon;
            }
        }
        return 0;
    }
    else
    {
        if (!field_16C_car || field_238_ped_type == ped_type::player_2)
        {
            if ((field_21C & 0x2000) != 0)
            {
                return field_174_pWeapon;
            }

            if ((field_21C & 0x200) != 0)
            {
                return 0;
            }
            return field_170_selected_weapon;
        }
        else
        {
            Car_BC* pCar = field_16C_car;
            Weapon_30* pCarWeapon = 0;
            if (pCar->IsFireTruck_4118F0())
            {
                pCarWeapon = gWeapon_8_707018->find_5E3D20(pCar, 20);
            }
            else if (pCar->IsTank_411900())
            {
                pCarWeapon = gWeapon_8_707018->find_5E3D20(pCar, 19);
            }
            else if (pCar->IsGunJeep_411910())
            {
                pCarWeapon = gWeapon_8_707018->find_5E3D20(pCar, 22);
            }

            if (!pCarWeapon)
            {
                return 0;
            }
            return pCarWeapon;
        }
    }

    return 0;
}

MATCH_FUNC(0x46f1e0)
void Ped::ApplyAimJitter_46F1E0(Weapon_30* a2)
{
    u8 rng_val = 0;
    // Named locals: the original gives each max a stack slot of its own (temporaries share the param slot)
    s16 max_still;
    s16 max_moving;
    s16 max_moving_retry;
    if (a2->field_1C_idx >= weapon_type::pistol && a2->field_1C_idx <= weapon_type::smg)
    {
        if (field_270 == 0)
        {
            if (GetPedVelocity_45C920() == kFpZero_678660)
            {
                max_still = 3;
                rng_val = gRng_6F6784.get_int_4F7AE0(max_still);
            }
            else
            {
                max_moving = 5;
                rng_val = gRng_6F6784.get_int_4F7AE0(max_moving);
                if (rng_val == 0)
                {
                    max_moving_retry = 5;
                    rng_val = gRng_6F6784.get_int_4F7AE0(max_moving_retry);
                }
            }
        }
        else if (field_270 == 2)
        {
            rng_val = 0;
        }

        switch (rng_val)
        {
            case 1:
                field_12E_aim_angle = field_12E_aim_angle - kAng10_6784C8;
                break;
            case 2:
                field_12E_aim_angle = field_12E_aim_angle + kAng10_6784C8;
                break;
            case 3:
                field_12E_aim_angle = field_12E_aim_angle - kAng16_6784E4;
                break;
            case 4:
                field_12E_aim_angle = field_12E_aim_angle + kAng16_6784E4;
                break;
            default:
                break;
        }
    }
}

MATCH_FUNC(0x46f390)
void Ped::ManageWeapon_46F390()
{
    Weapon_30* pWeapon = Ped::GetWeaponFromPed_46F110();
    if (Ped::IsField238_45EDE0(2))
    {
        if (field_168_game_object)
        {
            field_12E_aim_angle = field_168_game_object->field_80_sprite_ptr->field_0;
        }
    }
    if (field_21C_bf.b11 == true)
    {
        if (pWeapon)
        {
            if (!field_267_varrok_idx)
            {
                field_267_varrok_idx = gVarrok_7F8_703398->AllocForPed_59B060(field_200_id);
            }
            if (!field_21C_bf.b7)
            {
                if (field_168_game_object)
                {
                    if (!Ped::IsField238_45EDE0(2))
                    {
                        Ped::ApplyAimJitter_46F1E0(pWeapon);
                    }
                    pWeapon->pull_trigger_5E3670();
                }
            }
            else if (field_16C_car)
            {
                if (field_258_objective == objectives_enum::kill_char_any_means_19)
                {
                    Ped::AimRoofGun_470050();
                }
                if (field_21C_bf.b11)
                {
                    pWeapon->pull_trigger_5E3670();
                }
            }
            else
            {
                pWeapon->pull_trigger_5E3670();
            }
        }
        else if (field_168_game_object)
        {
            if (field_238_ped_type == 2)
            {
                Ped::HandleClosePedInteraction_45CAA0();
            }
        }
    }
    else if (pWeapon)
    {
        pWeapon->ChuckThrowable_5E34B0();
    }
}

// 9.6f 0x434E60
MATCH_FUNC(0x46f490)
Weapon_30* Ped::ChooseAttackWeapon_46F490()
{
    Car_BC* pCar;

    switch (this->field_240_occupation)
    {
        case ped_ocupation_enum::police:
        case ped_ocupation_enum::walking_guard_29:
            if (!field_14C_internal_target_ped->IsField238_45EDE0(2))
            {
                this->field_21C_bf.b13 = 1;
                return this->field_174_pWeapon;
            }

            pCar = this->field_14C_internal_target_ped->field_16C_car;
            if (pCar)
            {
                if (pCar->GetVelocity_43A4C0() > kFpPoint01_678624)
                {
                    this->field_21C_bf.b13 = 1;
                    return this->field_174_pWeapon;
                }
                this->field_21C_bf.b9 = 1;
                return 0;
            }

            if (gDistanceToTarget_678750 < kFpTwo_678658 + kFpOne_678798)
            {
                ++gNumPolicePedsInRangeScreen_6787EE; // police peds in range screen
            }

            if (gPolice_7B8_6FEE40->field_7AD_police_peds_in_range_screen < 2u)
            {
                this->field_21C_bf.b9 = 1;
                this->field_198 = 0;
                return 0;
            }

            if (!gPolice_7B8_6FEE40->field_7B0 || gPolice_7B8_6FEE40->field_7B0 == this)
            {
                return this->field_170_selected_weapon;
            }
            this->field_198 = 0;
            this->field_21C_bf.b9 = 1;
            return 0;

        case ped_ocupation_enum::fbi:
            if (this->field_14C_internal_target_ped->field_16C_car || gDistanceToTarget_678750 > kFpHalf_67853C)
            {
                this->field_21C_bf.b13 = 1;
                return this->field_174_pWeapon;
            }
            return this->field_170_selected_weapon;

        case ped_ocupation_enum::roadblock_cop_37:
            if (!field_14C_internal_target_ped->IsField238_45EDE0(2) || this->field_14C_internal_target_ped->field_16C_car)
            {
                this->field_21C_bf.b13 = 1;
                return this->field_174_pWeapon;
            }
            return this->field_170_selected_weapon;

        default:
            break;
    }
    return this->field_170_selected_weapon;
}

MATCH_FUNC(0x46f600)
void Ped::ForceWeapon_46F600(s32 weapon_kind)
{
    RemovePedWeapons_462510();
    if (weapon_kind != 28)
    {
        Weapon_30* pWeapon = gWeapon_8_707018->allocate_5E3C10(weapon_kind, this, 99u);
        this->field_170_selected_weapon = pWeapon;
        pWeapon->Set_F4_433810(1);
        if (field_170_selected_weapon->IsExplosiveWeapon_5E3BD0())
        {
            GiveWeapon_46F650(weapon_type::pistol);
        }
    }
}

MATCH_FUNC(0x46f650)
void Ped::GiveWeapon_46F650(s32 weapon_kind)
{
    RemoveSecondaryWeapon_462550();
    Weapon_30* pWeapon = gWeapon_8_707018->allocate_5E3C10(weapon_kind, this, 99u);
    this->field_174_pWeapon = pWeapon;
    pWeapon->Set_F4_433810(1);
}

MATCH_FUNC(0x46f680)
void Ped::ApplyGangRespectForKill_46F680(Ped* pPed)
{
    if (field_17C_pGang)
    {
        if (field_17C_pGang->get_field_111_433B30())
        {
            if (field_290 != 3 && field_290 != 1)
            {
                field_17C_pGang->ApplyKillRespectChange_4BEF70(pPed->field_15C_player->get_idx_4219D0(), 5);
            }
            else
            {
                field_17C_pGang->ApplyKillRespectChange_4BEF70(pPed->field_15C_player->get_idx_4219D0(), 1);
            }
        }
    }
    else
    {
        if (field_19C && field_19C->field_111)
        {
            if (field_290 != 3 && field_290 != 1)
            {
                field_19C->ApplyKillRespectChange_4BEF70(pPed->field_15C_player->get_idx_4219D0(), 5);
            }
            else
            {
                field_19C->ApplyKillRespectChange_4BEF70(pPed->field_15C_player->get_idx_4219D0(), 1);
            }
        }
    }
}

MATCH_FUNC(0x46f720)
void Ped::UpdateStatsForKiller_46F720()
{

    s32 ped_id; // eax
    Ped* pKillerPed; // eax
    Player* pPlayerIter; // edi
    Ped* pPlayerPed; // eax
    Ped* pPedKiller; // ecx
    PedGroup* pGroup; // ecx

    ped_id = this->field_204_killer_id;
    this->field_1A8_ped_killer = 0;
    if (ped_id)
    {
        pKillerPed = gPedManager_6787BC->PedById(ped_id);
        this->field_1A8_ped_killer = pKillerPed;
        if (pKillerPed)
        {
            if (pKillerPed->field_28C_threat_reaction != threat_reaction_enum::react_as_emergency_1 && pKillerPed->IsField238_45EDE0(2))
            {
                if (gShooey_CC_67A4B8->ShouldReportPedCrime_485140(this, this->field_1A8_ped_killer->field_15C_player))
                {
                    if (this->field_17C_pGang || this->field_19C)
                    {
                        field_1A8_ped_killer->add_wanted_points_470160(1); // gang guy killed
                    }
                    else if (this->field_28C_threat_reaction == threat_reaction_enum::react_as_emergency_1)
                    {
                        field_1A8_ped_killer->add_wanted_points_470160(500); // police?
                    }
                    else
                    {
                        field_1A8_ped_killer->add_wanted_points_470160(100); // normal ped?
                    }
                }
            }
            if (bStartNetworkGame_7081F0)
            {
                if (field_1A8_ped_killer->IsField238_45EDE0(2))
                {
                    if (IsField238_45EDE0(2))
                    {
                        gLucid_hamilton_67E8E0.UpdateFrags_4C5CD0(this->field_1A8_ped_killer->field_15C_player->get_idx_4219D0(),
                                                                  this->field_15C_player->get_idx_4219D0());
                        gHud_2B00_706620->field_12F0_mp_message.AnnounceKill_5D5770(this->field_1A8_ped_killer->field_15C_player,
                                                                         this->field_15C_player);
                    }
                }
                else if (IsField238_45EDE0(2))
                {
                    pPlayerIter = NULL;
                    if (this->field_1A8_ped_killer->field_164_ped_group)
                    {
                        for (pPlayerIter = gGame_0x40_67E008->IterateFirstPlayer_4B9CD0(); pPlayerIter != NULL;
                             pPlayerIter = gGame_0x40_67E008->IterateNextPlayer_4B9D10())
                        {
                            pPlayerPed = pPlayerIter->field_2C4_player_ped;
                            if (pPlayerPed)
                            {
                                if (pPlayerPed->field_164_ped_group == this->field_1A8_ped_killer->field_164_ped_group &&
                                    pPlayerIter != this->field_15C_player)
                                {
                                    gLucid_hamilton_67E8E0.UpdateFrags_4C5CD0(pPlayerIter->get_idx_4219D0(),
                                                                              this->field_15C_player->get_idx_4219D0());
                                    gHud_2B00_706620->field_12F0_mp_message.AnnounceKill_5D5770(pPlayerIter, this->field_15C_player);
                                    break;
                                }
                            }
                        }
                    }

                    if (!pPlayerIter)
                    {
                        gLucid_hamilton_67E8E0.UpdateFrags_4C5CD0(this->field_15C_player->get_idx_4219D0(),
                                                                  this->field_15C_player->get_idx_4219D0());
                        if (!field_1A8_ped_killer->IsLawEnforcement_45B4E0())
                        {
                            gHud_2B00_706620->field_12F0_mp_message.AnnounceKill_5D5770(this->field_15C_player, this->field_15C_player);
                        }
                    }
                }
            }
            pPedKiller = this->field_1A8_ped_killer;
            if (this != pPedKiller)
            {
                if (pPedKiller->IsField238_45EDE0(2))
                {
                    field_1A8_ped_killer->field_15C_player->field_2D4_scores.OnPedKilled_592660(this, this->field_1A8_ped_killer);
                    ApplyGangRespectForKill_46F680(this->field_1A8_ped_killer);
                }
                else
                {
                    pGroup = this->field_1A8_ped_killer->field_164_ped_group;
                    if (pGroup)
                    {
                        Ped* pLocalPed = gGame_0x40_67E008->field_38_orf1->field_2C4_player_ped;
                        if (pLocalPed)
                        {
                            if (pGroup == pLocalPed->field_164_ped_group)
                            {
                                ApplyGangRespectForKill_46F680(pLocalPed);
                            }
                        }
                    }
                }
            }
        }
    }
    if (bStartNetworkGame_7081F0)
    {
        if (!this->field_1A8_ped_killer && IsField238_45EDE0(2))
        {
            gLucid_hamilton_67E8E0.UpdateFrags_4C5CD0(this->field_15C_player->get_idx_4219D0(), this->field_15C_player->get_idx_4219D0());
            gHud_2B00_706620->field_12F0_mp_message.AnnounceKill_5D5770(this->field_15C_player, this->field_15C_player);
        }
    }
}

MATCH_FUNC(0x46f9d0)
void Ped::Kill_46F9D0()
{
    if ((field_21C & ped_bit_status_enum::k_ped_in_flames) != 0)
    {
        if (field_168_game_object)
        {
            field_168_game_object->field_6C_animation_state = 21;
        }
        PutOutFire();
    }

    if (!isDead_403B60())
    {
        SetObjective2_463830(objectives_enum::no_obj_0, 9999);
        if (field_27C_ped_state_2 == ped_state_2::lying_on_floor_22)
        {
            field_278_ped_state_1 = ped_state_1::dead_9;
            field_27C_ped_state_2 = ped_state_2::Unknown_15;
        }
        else
        {
            Ped::ChangeNextPedState1_45C500(ped_state_1::dead_9);
            Ped::ChangeNextPedState2_45C540(ped_state_2::Unknown_15);
        }

        Set_F250_IfBit_433DD0(4);

        UpdateStatsForKiller_46F720();

        switch (field_240_occupation)
        {
            case ped_ocupation_enum::paramedic_23:
                if (gAmbulance_110_6F70A8->HandlePedDeath_4FA330(this))
                {
                    return;
                }
                break;

            case ped_ocupation_enum::police:
            case ped_ocupation_enum::swat:
            case ped_ocupation_enum::fbi:
                if (!IsField238_45EDE0(4) || !gPolice_7B8_6FEE40->HandlePedDeath_56F4D0(this))
                {
                    break;
                }
                return;

            case ped_ocupation_enum::unknown_1:
                Deallocate_45EB60();
                return;

            case ped_ocupation_enum::elvis:
            case ped_ocupation_enum::elvis_leader:
                if (field_164_ped_group)
                {
                    if (!field_1A8_ped_killer)
                    {
                        field_1A8_ped_killer = this;
                    }
                    field_164_ped_group->DisbandGroupDueToAttack_4C94E0(field_1A8_ped_killer);
                }
                break;

            default:
                break;
        }

        if (field_168_game_object)
        {
            Set_B4_F16_To_1_433B50();
            if (field_238_ped_type == ped_type::player_2)
            {
                if (field_164_ped_group)
                {
                    field_164_ped_group->DestroyGroup_4C93A0();
                }
            }
            else if (!field_164_ped_group && field_238_ped_type != ped_type::script_created_5)
            {
                SetObjective(objectives_enum::objective_28, 9999);
            }
            field_168_game_object->SetSpriteNum_4338F0(6);
        }
        else if (field_238_ped_type == ped_type::player_2)
        {
            if (field_16C_car)
            {
                field_15C_player->UnloadCarWeapons_564C00();
            }
            if (field_164_ped_group)
            {
                field_164_ped_group->DestroyGroup_4C93A0();
            }
            field_21C_bf.b0 = false;
        }
        else
        {
            Deallocate_45EB60();
        }

        if (!field_164_ped_group || field_28C_threat_reaction == threat_reaction_enum::react_as_emergency_1)
        {
            set_health_4039A0(0);
            field_20A_wanted_points = 0;
            field_21C_bf.b11 = false;
            if (isDead_403B60() && field_27C_ped_state_2 == ped_state_2::Unknown_15)
            {
                SpawnWeaponOnDeath_45E080();
            }

            gThreateningPedsList_678468.RemovePed_471240(this);

            if (bDo_blood_67D5C5)
            {
                if (!field_16C_car && field_168_game_object->field_6C_animation_state != 17)
                {
                    gParticle_8_6FD5E8->SpawnBlood_53E880(field_1AC_cam.x, field_1AC_cam.y, field_1AC_cam.z);
                }
            }
        }
        else
        {
            field_164_ped_group->RemovePed_4C9970(this);
        }
    }
}

MATCH_FUNC(0x46fc70)
void Ped::AddThreateningPedToList_46FC70()
{
    bThreateningPedAdded_6787EF = 1;
    gThreateningPedsList_678468.AddPedToFrontIfMissing_4711B0(this);
}

MATCH_FUNC(0x46fc90)
void Ped::HandleShootingAtCar_46FC90(Car_BC* pCar, s32 model)
{
    Weapon_30* pWeapon;
    if ((field_21C & 0x2000) != 0)
    {
        pWeapon = field_174_pWeapon;
    }
    else
    {
        pWeapon = field_170_selected_weapon;
    }

    if (pWeapon)
    {
        if (pCar->GetVelocity_43A4C0() == kFpZero_678660)
        {
            Ped* pDriver = pCar->field_54_driver;
            if (pDriver)
            {
                if (pDriver->IsField238_45EDE0(3))
                {
                    Ped* pDriver2 = pCar->field_54_driver;
                    if (pDriver2->field_258_objective != objectives_enum::leave_car_36)
                    {
                        pDriver2->SpawnDriverRunAway_45C650(pCar, this);
                    }
                }
            }
        }

        if (IsField238_45EDE0(2))
        {
            field_15C_player->field_2D4_scores.UpdateAccuracyCount_5934F0(4u, model, 0);
            return;
        }

        Fix16 dist_to_cam;
        dist_to_cam = Fix16::MaxAbsDistance_42A6B0(field_1AC_cam.x,
                                                        field_1AC_cam.y,
                                                        pCar->field_50_car_sprite->field_14_xy.x,
                                                        pCar->field_50_car_sprite->field_14_xy.y);

        if (pCar == field_154_target_to_enter)
        {
            pWeapon->Set_F4_433810(0);
        }
        else if (field_14C_internal_target_ped && field_14C_internal_target_ped->field_16C_car && field_14C_internal_target_ped->field_16C_car == pCar)
        {
            pWeapon->Set_F4_433810(0);
        }
        else
        {
            Fix16 max_range;
            max_range = kFpHalf_678790 + kFpOne_678798;
            if (dist_to_cam < max_range && pCar->GetVelocity_43A4C0() < kFpPoint02_678630)
            {
                pWeapon->Set_F4_433810(1);
                return;
            }

            if (!pWeapon->IsExplosiveWeapon_5E3BD0())
            {
                pWeapon->Set_F4_433810(0);
            }

            if (field_14C_internal_target_ped && field_14C_internal_target_ped->get_car_416B60() && pCar == field_14C_internal_target_ped->get_car_416B60())
            {
                pWeapon->Set_F4_433810(0);
            }
        }
    }
}

MATCH_FUNC(0x46fe20)
void Ped::ProcessWeaponHitResponse_46FE20(Object_2C* pObj)
{
    Weapon_30* pWeapon;
    Fix16 dist;
    if ((field_21C & 0x2000) != 0)
    {
        pWeapon = field_174_pWeapon;
    }
    else
    {
        pWeapon = field_170_selected_weapon;
    }

    if (pWeapon && !Ped::IsField238_45EDE0(2))
    {
        dist = Fix16::MaxAbsDistance_42A6B0(field_1AC_cam.x, field_1AC_cam.y, pObj->get_x_4340D0(), pObj->get_y_4340E0());

        if (pObj == field_1A4_internal_target_object)
        {
            pWeapon->Set_F4_433810(0);
        }
        else if (dist < kFpTwo_678658)
        {
            pWeapon->Set_F4_433810(1);
        }
        else if (!pWeapon->IsExplosiveWeapon_5E3BD0())
        {
            pWeapon->Set_F4_433810(0);
        }
    }
}

MATCH_FUNC(0x46ff00)
void Ped::NotifyWeaponHit_46FF00(Fix16 xpos, Fix16 ypos, s32 model)
{
    Weapon_30* pWeapon;

    if ((field_21C & ped_bit_status_enum::k_ped_0x00002000) != 0)
    {
        pWeapon = field_174_pWeapon;
    }
    else
    {
        pWeapon = field_170_selected_weapon;
    }

    if (pWeapon)
    {
        if (IsField238_45EDE0(2))
        {
            field_15C_player->field_2D4_scores.UpdateAccuracyCount_5934F0(1u, model, 0);
        }
        else
        {
            if (Fix16::MaxAbsDistance_42A6B0(field_1AC_cam.x, field_1AC_cam.y, xpos, ypos) < kFpTwo_678658)
            {
                pWeapon->Set_F4_433810(1);
            }
            else
            {
                pWeapon->Set_F4_433810(0);
            }
        }
    }
}

MATCH_FUNC(0x46fff0)
void Ped::HandleWeaponFireEnd_46FFF0(s32 model)
{
    if (IsField238_45EDE0(2))
    {
        field_15C_player->field_2D4_scores.UpdateAccuracyCount_5934F0(0, model, 0);
    }

    if ((this->field_21C & ped_bit_status_enum::k_ped_0x00002000) != 0)
    {
        Weapon_30* pWeapon = this->field_174_pWeapon;
        if (pWeapon)
        {
            pWeapon->Set_F4_433810(0);
        }
    }
    else
    {
        Weapon_30* pSelectedWeapon = this->field_170_selected_weapon;
        if (pSelectedWeapon)
        {
            pSelectedWeapon->Set_F4_433810(0);
        }
    }
}

MATCH_FUNC(0x470050)
void Ped::AimRoofGun_470050()
{
    Sprite_18* pHit = 0;
    if (field_16C_car->IsFireTruck_4118F0())
    {
        pHit = field_16C_car->field_0_qq.GetSpriteForModel_5A6A50(114);
    }
    else if (field_16C_car->IsTank_411900())
    {
        pHit = field_16C_car->field_0_qq.GetSpriteForModel_5A6A50(148);
    }
    else if (field_16C_car->IsGunJeep_411910())
    {
        pHit = field_16C_car->field_0_qq.GetSpriteForModel_5A6A50(248);
    }

    Fix16 x = pHit->field_0->field_14_xy.x;
    Fix16 y = pHit->field_0->field_14_xy.y;
    Ped* objective_target_ped = this->field_148_objective_target_ped;
    Ang16 tan_v;
    tan_v = Fix16::atan2_fixed_405320(objective_target_ped->get_cam_y() - y, objective_target_ped->get_cam_x() - x);

    this->field_21C &= ~0x800;
    this->field_21C |= 0x80;

    if (field_16C_car->RotateRoofObjectTowardTarget_440C10(tan_v))
    {
        if (field_148_objective_target_ped->IsField238_45EDE0(2))
        {
            if (!this->field_16C_car->field_76_last_seen_timer)
            {
                this->field_21C |= 0x800;
            }
        }
        else if (gDistanceToTarget_678750 < kFpFour_678680)
        {
            this->field_21C |= 0x800;
        }
    }
    else
    {
        this->field_21C &= ~0x800;
    }
}

MATCH_FUNC(0x470160)
void Ped::add_wanted_points_470160(s16 wanted_amount)
{
    field_20A_wanted_points += wanted_amount;

    if (field_20A_wanted_points > 12000)
    {
        field_20A_wanted_points = 12000;
    }
    else if (field_20A_wanted_points < 0)
    {
        field_20A_wanted_points = 0;
    }

    s16 star_count = gPolice_7B8_6FEE40->field_660_wanted_star_count;
    if (get_wanted_star_count_46EF00() >= star_count)
    {
        set_wanted_star_count_46F070(static_cast<u8>(star_count));
    }
}

MATCH_FUNC(0x4701d0)
bool Ped::IsNearestSpriteACar_4701D0()
{
    Sprite* pSprite = gPurpleDoom_1_679208->FindNearestSpriteOfType_477E60(this->field_168_game_object->field_80_sprite_ptr, 0);
    if (pSprite)
    {
        return (pSprite->get_type_416B40() != sprite_types_enum::car_2) ? false : true;
    }
    return false;
}

MATCH_FUNC(0x470200)
void Ped::StartPedWalking_470200(Fix16 a2, Fix16 a3, Fix16 a4)
{
    Ped::AllocCharB4_45C830(a2, a3, a4);
    // 9.6f: SetRemap_433C10 (inlined, using it changes the code)
    Char_B4* pB4 = field_168_game_object;
    u8 remap = field_244_remap;
    pB4->field_5_remap = remap;
    if (remap != 0xFF)
    {
        pB4->field_80_sprite_ptr->SetRemap(remap);
    }
    if (field_238_ped_type == ped_type::player_2)
    {
        Ped::ChangeNextPedState2_45C540(ped_state_2::ped2_walking_0);
        Ped::ChangeNextPedState1_45C500(ped_state_1::walking_0);
        field_168_game_object->SetMaxSpeedByRef_433920(kFpZero_678438);
    }
    else
    {
        Ped::ChangeNextPedState2_45C540(ped_state_2::ped2_walking_0);
        Ped::ChangeNextPedState1_45C500(ped_state_1::walking_0);
    }
    field_16C_car = 0;
}

MATCH_FUNC(0x4702d0)
void Ped::BecomeLeaderOfGroup_4702D0(Ped* pPed)
{
    PedGroup* pPedGroup = pPed->field_164_ped_group;
    pPedGroup->replace_leader_4C8FE0(this);
    this->field_164_ped_group = pPedGroup;
}

MATCH_FUNC(0x470300)
void Ped::BecomeDummyOnPlayerDisconnect_470300()
{
    field_15C_player = 0;
    field_240_occupation = 3;
    field_238_ped_type = ped_type::dummy_3;
    Car_BC* pCar = field_16C_car;
    if (pCar)
    {
        // NOTE: OG tail calls a function chunk here
        pCar->sub_43AA20();
    }
}

MATCH_FUNC(0x4702A0)
void Ped::PushPatrolPoint_4702A0(s8 x, s8 y, s8 z)
{
    // Get a free patrol point
    Marz_3* pIter = this->field_190_patrol_route->field_0_points;
    while (pIter->field_0_x)
    {
        ++pIter;
    }

    // And populate it
    pIter->field_0_x = x;
    pIter->field_1_y = y;
    pIter->field_2_z = z;
}

MATCH_FUNC(0x470f00)
s32 Ped::IsInTrain_470F00()
{
    Car_BC* pBC = this->field_16C_car;
    if (pBC)
    {
        const s32 info_idx = pBC->field_84_car_info_idx;
        if (info_idx == 59 || info_idx == 60 || info_idx == 61 || info_idx == 6)
        {
            return 1;
        }
    }
    return 0;
}

EXPORT void Ped::nullsub_9()
{
    ;
}

EXPORT void Ped::nullsub_10()
{
    ;
}

EXPORT void Ped::nullsub_11()
{
    ;
}

EXPORT void Ped::nullsub_12()
{
    ;
}

EXPORT void Ped::nullsub_14()
{
    ;
}
