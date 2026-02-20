#include "Ped.hpp"
#include "Ambulance_110.hpp"
#include "CarInfo_808.hpp"
#include "CarPhysics_B0.hpp"
#include "Car_BC.hpp"
#include "Char_Pool.hpp"
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

// =================
DEFINE_GLOBAL(s8, byte_61A8A3, 0x61A8A3);
DEFINE_GLOBAL_INIT(Ang16, word_6FDB34, Ang16(0), 0x6FDB34);
DEFINE_GLOBAL_INIT(Ang16, gDummyPedAng_6787A8, Ang16(0), 0x6787A8);
DEFINE_GLOBAL_INIT(s32, dword_67866C, 0xC000, 0x67866C); // TODO: Fix16? Static init to, 0xC000, 0xUNKNOWN);
DEFINE_GLOBAL(s32, gPedId_61A89C, 0x61A89C);
DEFINE_GLOBAL_INIT(u8, gNumberMuggersSpawned_6787CA, 0, 0x6787CA);
DEFINE_GLOBAL_INIT(u8, gNumberCarThiefsSpawned_6787CB, 0, 0x6787CB);
DEFINE_GLOBAL_INIT(u8, gNumberElvisLeadersSpawned_6787CC, 0, 0x6787CC);
DEFINE_GLOBAL_INIT(u8, gNumberWalkingCopsSpawned_6787CD, 0, 0x6787CD);
DEFINE_GLOBAL(u8, byte_6787D2, 0x6787D2);
DEFINE_GLOBAL(u8, byte_61A8A0, 0x61A8A0);
DEFINE_GLOBAL(u8, byte_6787E2, 0x6787E2);
DEFINE_GLOBAL(u8, byte_6787E3, 0x6787E3);
DEFINE_GLOBAL(u8, byte_6787E4, 0x6787E4);
DEFINE_GLOBAL(u8, byte_6787D7, 0x6787D7);
DEFINE_GLOBAL(u8, byte_6787D4, 0x6787D4);
DEFINE_GLOBAL(u8, byte_678554, 0x678554);
DEFINE_GLOBAL(u8, byte_6787D8, 0x6787D8);
DEFINE_GLOBAL(u8, byte_6787D9, 0x6787D9);
DEFINE_GLOBAL(u8, byte_61A8A4, 0x61A8A4);
DEFINE_GLOBAL(u8, byte_6787C4, 0x6787C4);
DEFINE_GLOBAL(s16, word_6787D0, 0x6787D0);
DEFINE_GLOBAL(s16, word_6787F2, 0x6787F2);
DEFINE_GLOBAL(u16, word_6787E0, 0x6787E0);
DEFINE_GLOBAL(Ped*, dword_6787DC, 0x6787DC);
DEFINE_GLOBAL_INIT(Fix16, k_dword_678660, Fix16(0), 0x678660);
DEFINE_GLOBAL_INIT(Fix16, k_dword_678438, k_dword_678660, 0x678438);
DEFINE_GLOBAL_INIT(Fix16, gDistanceToTarget_678750, k_dword_678660, 0x678750);
DEFINE_GLOBAL_INIT(Fix16, dword_678678, Fix16(98304, 0), 0x678678);
DEFINE_GLOBAL_INIT(Fix16, dword_678520, dword_678678, 0x678520);
DEFINE_GLOBAL_INIT(Fix16, dword_678670, Fix16(4), 0x678670);
DEFINE_GLOBAL_INIT(Fix16, dword_6784C4, Fix16(256, 0), 0x6784C4);
DEFINE_GLOBAL_INIT(Fix16, dword_678448, dword_678670* dword_6784C4, 0x678448);
DEFINE_GLOBAL_INIT(Fix16, dword_678790, dword_6784C4 * 32, 0x678790);
DEFINE_GLOBAL_INIT(Fix16, dword_6784E8, dword_6784C4 * 8, 0x6784E8);
DEFINE_GLOBAL_INIT(Fix16, dword_6784CC, dword_6784C4 * 2, 0x6784CC);
DEFINE_GLOBAL_INIT(Fix16, dword_678434, dword_6784CC, 0x678434);
DEFINE_GLOBAL_INIT(Fix16, dword_678620, dword_6784C4 / dword_678670, 0x678620);
DEFINE_GLOBAL_INIT(Fix16, dword_678788, dword_6784C4 * 16, 0x678788);
DEFINE_GLOBAL_INIT(Fix16, k_dword_678664, Fix16(0x4000, 0), 0x678664);
DEFINE_GLOBAL_INIT(Fix16, dword_6785EC, k_dword_678664, 0x6785EC);
DEFINE_GLOBAL_INIT(Fix16, k_dword_678624, Fix16(0xA3, 0), 0x678624);
DEFINE_GLOBAL_INIT(Fix16, k_dword_67853C, Fix16(0x2000, 0), 0x67853C);
DEFINE_GLOBAL_INIT(Fix16, dword_678634, Fix16(0x333, 0), 0x678634);
DEFINE_GLOBAL_INIT(Fix16, dword_678480, Fix16(0x666, 0), 0x678480);
DEFINE_GLOBAL_INIT(Fix16, dword_6784A4, Fix16(0x3999, 0), 0x6784A4);
DEFINE_GLOBAL_INIT(Ang16, word_6784FC, Ang16(180), 0x6784FC);
DEFINE_GLOBAL_INIT(Ang16, word_678590, Ang16(720), 0x678590);
DEFINE_GLOBAL_INIT(Fix16, dword_6784DC, dword_6784C4 * 6, 0x6784DC);
DEFINE_GLOBAL_INIT(Fix16, dword_678668, Fix16(2), 0x678668);
DEFINE_GLOBAL_INIT(Fix16, gSpawnJitterScale_678618, Fix16(256, 0), 0x678618);
DEFINE_GLOBAL_INIT(Fix16, dword_678484, Fix16(0x1000, 0), 0x678484);
DEFINE_GLOBAL_INIT(Fix16, dword_678488, Fix16(0xCCC, 0), 0x678488);
DEFINE_GLOBAL(Ped*, dword_6787C0, 0x6787C0);
DEFINE_GLOBAL_INIT(Fix16, gDummyW_678530, dword_678488, 0x678530);
DEFINE_GLOBAL_INIT(Fix16, gDummyZ_67841C, dword_678484, 0x67841C);
DEFINE_GLOBAL(Object_2C*, dword_678558, 0x678558);
DEFINE_GLOBAL(char_type, byte_6787D3, 0x6787D3);
DEFINE_GLOBAL_INIT(Fix16, k_dword_678504, Fix16(0xAAA, 0), 0x678504);
DEFINE_GLOBAL_INIT(Fix16, dword_678574, dword_678484, 0x678574);
DEFINE_GLOBAL_INIT(Fix16, k_dword_67845C, dword_678574 / dword_678668, 0x67845C);
DEFINE_GLOBAL_INIT(Fix16, k_dword_678798, dword_6784C4 * 64, 0x678798);
DEFINE_GLOBAL_INIT(Fix16, k_dword_678658, dword_6784C4 * 128, 0x678658);
DEFINE_GLOBAL_INIT(Fix16, k_dword_678680, dword_6784C4 * 256, 0x678680);
DEFINE_GLOBAL_INIT(Fix16, k_dword_678430, dword_6784C4, 0x678430);
DEFINE_GLOBAL_INIT(Fix16, k_dword_678524, Fix16(0x9C3C000, 0), 0x678524);

DEFINE_GLOBAL_INIT(s16, k_word_678656, 40, 0x678656);
DEFINE_GLOBAL(u8, byte_6787CE, 0x6787CE);

DEFINE_GLOBAL_INIT(Fix16, dword_6784A0, Fix16(0x3333, 0), 0x6784A0);
DEFINE_GLOBAL_INIT(Fix16, dword_6784BC, dword_6784C4 / dword_678668, 0x6784BC);

DEFINE_GLOBAL_INIT(Ang16, word_6784C8, Ang16(40), 0x6784C8);
DEFINE_GLOBAL_INIT(Ang16, dword_6784E4, Ang16(64), 0x6784E4);
DEFINE_GLOBAL_INIT(Ang16, word_6784F0, Ang16(0), 0x6784F0);

DEFINE_GLOBAL_INIT(Fix16, dword_67856C, dword_678488, 0x67856C);
DEFINE_GLOBAL_INIT(Fix16, dword_678428, dword_678480, 0x678428);

DEFINE_GLOBAL_INIT(Fix16, dword_678778, dword_6784C4 * 10, 0x678778);
DEFINE_GLOBAL_INIT(Fix16, dword_678794, dword_6784C4 * 48, 0x678794);
DEFINE_GLOBAL_INIT(Fix16, dword_678630, Fix16(0x147, 0), 0x678630);

// TODO
EXTERN_GLOBAL(s32, bStartNetworkGame_7081F0);

// TODO: move with CarDoorAlignmentSolver_545AF0
EXTERN_GLOBAL(UnknownList, dword_6F6850);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD824, Fix16(0x666, 0), 0x6FD824);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD9C8, Fix16(0x3D7, 0), 0x6FD9C8);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD9AC, Fix16(0x147, 0), 0x6FD9AC);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD9D4, Fix16(0x51E, 0), 0x6FD9D4);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD9B8, Fix16(0x28F, 0), 0x6FD9B8);
DEFINE_GLOBAL_INIT(Fix16, dword_6FD830, Fix16(0x1333, 0), 0x6FD830);

// TODO: these are defined in char.cpp
EXTERN_GLOBAL(Fix16, dword_6FD8D8);
EXTERN_GLOBAL(Fix16, dword_6FDB20);
EXTERN_GLOBAL(Fix16, k_dword_6FD9E4);
EXTERN_GLOBAL(Fix16, dword_6FD82C);
EXTERN_GLOBAL(Fix16, dword_6FD9B0);

EXTERN_GLOBAL(Ang16, word_6FD936);
EXTERN_GLOBAL(Ang16, word_6FD854);

// TODO: move with CarDoorAlignmentSolver_545AF0
// https://decomp.me/scratch/6VBR0
inline void __stdcall sub_41FC90(Fix16& xpos, Fix16& ypos, Ang16& rotation)
{
    Fix16 old_xpos = xpos;
    xpos = xpos * Ang16::cosine_40F520(rotation) + ypos * Ang16::sine_40F500(rotation);
    ypos = old_xpos * Ang16::sine_40F500(rotation) - ypos * Ang16::cosine_40F520(rotation);
}

// TODO: move
// https://decomp.me/scratch/BzcQt
WIP_FUNC(0x545AF0)
EXPORT void __stdcall CarDoorAlignmentSolver_545AF0(s32 animPhase, Car_BC* pCar, u8 doorId, Fix16& outX, Fix16& outY, Ang16& outAng)
{
    WIP_IMPLEMENTED;
    // This func is really get_car_remap?? shouldnt be get_car_door_info?
    u8* car_door_info_array = gGtx_0x106C_703DD4->get_car_remap_5AA3D0(pCar->field_84_car_info_idx);
    Fix16 offset = dword_6FD8D8;

    s8 x_in_scale = car_door_info_array[2 * doorId + 1];
    Fix16 x_pos = dword_6F6850.sub_41FE70(x_in_scale);

    s8 y_in_scale = car_door_info_array[2 * doorId + 2];
    Fix16 y_pos = dword_6F6850.sub_41FE70(y_in_scale);

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
        else
        {
            bUnk = false;
        }
    }

    switch (doorId)
    {
        case 0:
        case 2:
            if (x_pos == k_dword_6FD9E4)
            {
                y_pos -= offset;
                switch ((u8)animPhase)
                {
                    case 0:
                        y_pos += dword_6FD82C;
                        break;
                    case 1:
                        y_pos += dword_6FD824 + dword_6FD9C8;
                        break;
                    case 2:
                        y_pos += dword_6FD824 + dword_6FD9AC;
                        break;
                    case 3:
                        y_pos += dword_6FD9D4;
                        break;
                    case 4:
                        y_pos += dword_6FD9B8;
                        break;
                    case 6:
                        y_pos -= dword_6FD9B8;
                        break;
                    case 7:
                        y_pos -= dword_6FD9C8;
                        break;
                    default:
                        break;
                }
                outAng = word_6FD936 + pCar->field_50_car_sprite->field_0;
            }
            else
            {
                if (x_pos >= k_dword_6FD9E4)
                {
                    x_pos += offset;
                }
                else
                {
                    x_pos -= offset;
                }
                if (bUnk)
                {
                    y_pos -= dword_6FD824;
                    switch (animPhase)
                    {
                        case 0:
                            x_pos -= dword_6FD82C;
                            break;
                        case 1:
                            x_pos -= dword_6FD824 + dword_6FD9C8;
                            break;
                        case 2:
                            x_pos -= dword_6FD824 + dword_6FD9AC;
                            break;
                        case 3:
                            x_pos -= dword_6FD9D4;
                            break;
                        case 4:
                            x_pos -= dword_6FD9B8;
                            break;
                        case 6:
                            x_pos += dword_6FD9B8;
                            break;
                        case 7:
                            x_pos += dword_6FD9D4;
                            break;
                        default:
                            break;
                    }
                    outAng = word_6FD854 + pCar->field_50_car_sprite->field_0;
                }
                else if ((u8)animPhase <= 99)
                {
                    switch (animPhase)
                    {
                        case 0:
                            x_pos -= dword_6FD824;
                            y_pos -= dword_6FD824;
                            outAng = pCar->field_50_car_sprite->field_0;
                            break;
                        case 1:
                            y_pos -= dword_6FD824;
                            x_pos -= dword_6FD9B0;
                            outAng = pCar->field_50_car_sprite->field_0;
                            break;
                        case 2:
                            y_pos -= dword_6FD824;
                            outAng = pCar->field_50_car_sprite->field_0;
                            break;
                        case 3:
                            y_pos -= dword_6FD824;
                            x_pos += dword_6FD9B0;
                            outAng = pCar->field_50_car_sprite->field_0;
                            break;
                        case 4:
                            y_pos -= dword_6FD9B0;
                            x_pos -= dword_6FD824;
                            outAng = pCar->field_50_car_sprite->field_0;
                            break;
                        case 5:
                            y_pos -= dword_6FD9B0;
                            x_pos -= dword_6FD824 + dword_6FD9B0;
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
                            x_pos -= dword_6FD830;
                            outAng = pCar->field_50_car_sprite->field_0;
                            break;
                        case 9:
                        case 10:
                        case 11:
                        case 12:
                            y_pos -= dword_6FD9AC + dword_6FD824;
                            x_pos -= dword_6FD9B8;
                            outAng = pCar->field_50_car_sprite->field_0;
                            break;
                        case 99:
                            x_pos += dword_6FD824;
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
                y_pos -= dword_6FD824;
                switch (animPhase)
                {
                    case 0:
                        x_pos += dword_6FD82C;
                        break;
                    case 1:
                        x_pos += dword_6FD824 + dword_6FD9C8;
                        break;
                    case 2:
                        x_pos += dword_6FD824 + dword_6FD9AC;
                        break;
                    case 3:
                        x_pos += dword_6FD9D4;
                        break;
                    case 4:
                        x_pos += dword_6FD9B8;
                        break;
                    case 6:
                        x_pos -= dword_6FD9B8;
                        break;
                    case 7:
                        x_pos -= dword_6FD9D4;
                        break;
                    default:
                        break;
                }
                outAng = pCar->field_50_car_sprite->field_0 - word_6FD854;
            }
            else if ((u8)animPhase <= 99u)
            {
                switch (animPhase) // byte_5462F8[(u8)animPhase]  // What is byte_5462F8????
                {
                    case 0:
                        x_pos += dword_6FD824;
                        y_pos -= dword_6FD824;
                        outAng = word_6FD936 + pCar->field_50_car_sprite->field_0;
                        break;
                    case 1:
                        x_pos += dword_6FD9B0;
                        y_pos -= dword_6FD824;
                        outAng = word_6FD936 + pCar->field_50_car_sprite->field_0;
                        break;
                    case 2:
                        y_pos -= dword_6FD824;
                        outAng = word_6FD936 + pCar->field_50_car_sprite->field_0;
                        break;
                    case 3:
                        x_pos -= dword_6FD9B0;
                        y_pos -= dword_6FD824;
                        outAng = word_6FD936 + pCar->field_50_car_sprite->field_0;
                        break;
                    case 4:
                        y_pos -= dword_6FD9B0;
                        x_pos += dword_6FD824;
                        outAng = pCar->field_50_car_sprite->field_0;
                        break;
                    case 5:
                        y_pos -= dword_6FD9B0;
                        x_pos += dword_6FD824 + dword_6FD9B0;
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
                    case 8:
                        x_pos += dword_6FD9B8;
                        y_pos -= dword_6FD824;
                        outAng = word_6FD854 + pCar->field_50_car_sprite->field_0;
                        break;
                    case 9:
                        x_pos -= dword_6FD824;
                        outAng = pCar->field_50_car_sprite->field_0;
                        break;
                    case 10:
                        break;
                }
            }
            break;
        default:
            break;
    }
    sub_41FC90(x_pos, y_pos, pCar->field_50_car_sprite->field_0);
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

    if (Fix16::Abs(a1 - a2) < dword_6784A4)
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
    this->field_140 = 0;
    this->field_144 = 0;
    this->field_148_objective_target_ped = 0;
    this->field_14C = 0;
    this->field_150_target_objective_car = 0;
    this->field_158_unk_car = 0;
    this->field_154_target_to_enter = 0;
    this->mpNext = 0;
    this->field_164_ped_group = 0;
    this->field_168_game_object = 0;
    this->field_16C_car = 0;
    this->field_170_selected_weapon = 0;
    this->field_174_pWeapon = 0;
    this->field_180 = 0;
    this->field_18C = 0;
    this->field_21C &= ~0x4000u;
    this->field_184 = 0;
    this->field_208_invulnerability = 0;
    this->field_204_killer_id = 0;
    this->field_290 = 0;
    this->field_264 = 0;
    this->field_268 = 0;
    this->field_198 = 0;
    this->field_1A0_objective_target_object = 0;
    this->field_1A4 = 0;
}

// https://decomp.me/scratch/2yWEK
WIP_FUNC(0x45afc0)
char_type Ped::Reset_45AFC0()
{
    field_21C_bf.b2 = 0;
    field_234_timer = 0;
    field_238 = 3;
    field_23C = 0;
    field_240_occupation = ped_ocupation_enum::dummy;
    field_216_health = 0;
    field_20e = 0;
    field_244_remap = -1;
    field_1AC_cam.x = k_dword_678660;
    field_1AC_cam.y = k_dword_678660;
    field_1AC_cam.z = k_dword_678660;
    field_12C = gDummyPedAng_6787A8;
    field_248_enter_car_as_passenger = 1;
    field_24C_target_car_door = 0;
    field_140 = 0;
    field_22C = 0;
    field_230 = 1;
    field_270 = 1;
    field_12E = gDummyPedAng_6787A8;
    field_144 = 0;
    field_130 = Ang16(0); //-dword_6787A0;
    field_225_objective_status = 0;
    field_226 = 0;
    field_258_objective = objectives_enum::no_obj_0;
    field_25C_car_state = 0;
    field_218_objective_timer = 9999;
    field_21A_car_state_timer = 9999;
    field_1B8_target_x = Fix16(-1);
    field_1BC_target_y = Fix16(-1);
    field_1C0_target_z = Fix16(-1);
    field_1C4_x = Fix16(-1);
    field_1C8_y = Fix16(-1);
    field_1CC_z = Fix16(-1);
    field_148_objective_target_ped = 0;
    field_14C = 0;
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
    field_178 = 0;
    field_21C_bf.b11 = 0;
    field_21C_bf.b12 = 0;
    field_21C_bf.b13 = 0;
    field_17C_pZone = 0;
    field_262 = 0;
    field_263 = 0;
    field_180 = 0;
    field_204_killer_id = 0;
    field_290 = 0;
    field_264 = 0;
    field_21C_bf.b14 = 0;
    field_0_patrol_points[0].field_0 = 0;
    field_0_patrol_points[0].field_1 = 0;
    field_261 = 0;
    field_18C = 0;
    field_21C &= ~0x18000u;
    field_190 = 0;
    field_194 = 0;
    field_265 = 0;
    field_1D0 = k_dword_678660;
    field_1D4 = k_dword_678660;
    field_1D8 = k_dword_678660;
    field_1DC_objective_target_x = k_dword_678660;
    field_1E0_objective_target_y = k_dword_678660;
    field_1E4_objective_target_z = k_dword_678660;
    field_134_rotation = gDummyPedAng_6787A8;
    field_1E8 = k_dword_678660;
    field_1EC = k_dword_678660.mValue;
    field_184 = 0;
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
    field_1F4 = dword_678434;
    field_1F0_maybe_max_speed = dword_678448;
    field_268 = 0;
    field_198 = 0;
    field_19C = 0;
    byte_6787C4 = 0;
    field_21C_bf.b3 = 0;
    field_1A0_objective_target_object = 0;
    field_1F8 = dword_6784A0;
    field_1A4 = 0;
    field_132 = gDummyPedAng_6787A8;
    field_1FC = k_dword_678660.mValue;
    field_269 = -1;
    field_214 = 0;
    field_26A = 0;
    field_21C_bf.b4 = 0;
    field_26C_graphic_type = 1;
    field_21C_bf.b5 = 0;
    field_21C_bf.b6 = 0;
    field_250 = 0;
    field_224 &= 0xF0u;
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
    field_21C_bf.b28 = 0;
    field_224 &= ~0x10u;
    field_21C_bf.b29 = 0;
    field_260 = 0;
    field_224 |= 0x20u;
    return 0;
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
char_type Ped::sub_45B4E0()
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
Fix16_Point Ped::sub_45B520()
{
    return Fix16_Point(field_168_game_object->field_98.x, field_168_game_object->field_98.y);
}

MATCH_FUNC(0x45b550)
void Ped::sub_45B550()
{
    field_26A = 2;
}

MATCH_FUNC(0x45b560)
void Ped::sub_45B560(Player* a2, char_type a3)
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

STUB_FUNC(0x45b5b0)
s32 Ped::CopyStatsFromPed_45B5B0(s32 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x45bbf0)
Car_BC* Ped::sub_45BBF0()
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
    Fix16 tempZ;
    gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(&tempZ, xpos, ypos);
    Car_BC* pCar = field_16C_car;
    if (pCar)
    {
        pCar->sub_443D00(xpos, ypos, tempZ);
    }
    else
    {
        field_168_game_object->sub_545530(xpos, ypos, tempZ);
    }
}

STUB_FUNC(0x5DF270);
EXPORT int __stdcall sub_5DF270(Sprite* a1, Fix16 a2, char_type a3, char_type a4, Ped* a5, s32* a6)
{
    // TODO: Is this actually a class method ?? also location is wrong in respect to address ordering
    NOT_IMPLEMENTED;
    return 0;
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
                if (field_168_game_object->field_10_char_state != Char_B4_state::Jumping_15 &&
                    field_278_ped_state_1 != ped_state_1::immobilized_8)
                {
                    ChangeNextPedState1_45C500(ped_state_1::immobilized_8);
                    ChangeNextPedState2_45C540(ped_state_2::electrocuted_27);
                    field_168_game_object->field_16 = 1;
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
                sub_5DF270(field_168_game_object->field_80_sprite_ptr, k_dword_67853C, 0, 1, this, 0);
            }
        }
    }
}

MATCH_FUNC(0x45bd20)
bool Ped::sub_45BD20(Car_BC* pCar)
{
    if (pCar == field_154_target_to_enter || pCar->GetVelocity_43A4C0() < k_dword_678624)
    {
        return true;
    }
    Car_Door_10* Door = field_154_target_to_enter->GetDoor(field_24C_target_car_door);
    Door->sub_439EA0();

    field_168_game_object->sub_553E00(word_6784FC + pCar->field_50_car_sprite->field_0, dword_678634 + dword_678480, k_dword_678660, 1);
    Ped::ChangeNextPedState1_45C500(ped_state_1::walking_0);
    Ped::ChangeNextPedState2_45C540(ped_state_2::ped2_walking_0);
    return false;
}

MATCH_FUNC(0x45be30)
s32 Ped::sub_45BE30()
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
            pB4->sub_5454B0();
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
            const bool HasDiedBefore = field_278_ped_state_1 == ped_state_1::dead_9;
            TakeDamage(1);
            field_264 = 50;

            if (field_278_ped_state_1 == ped_state_1::dead_9 && !HasDiedBefore)
            {
                Player* pWeapons = field_15C_player;
                if (pWeapons)
                {
                    pWeapons->field_44_death_type = 2;
                }
            }

            if (!field_15C_player) // not player
            {
                if (field_168_game_object)
                {
                    if (field_25C_car_state != 1) // not in flee/running state?
                    {
                        field_21C |= ped_bit_status_enum::k_ped_0x00000004;

                        sub_463830(1, 9999); // make them flee

                        field_1D0 = field_1AC_cam.x;
                        field_1D4 = field_1AC_cam.y;
                        field_1D8 = field_1AC_cam.z;
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
            pB4->sub_545430();
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
        pB4->field_80_sprite_ptr->field_2C = 0; // make sprite opaque
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
            pB4->field_80_sprite_ptr->field_2C = 0x41; // make sprite semi transparent
        }
    }
}

MATCH_FUNC(0x45c040)
void Ped::SetInvulnerable()
{
    field_208_invulnerability = 9999;
    sub_45C070();
}

MATCH_FUNC(0x45c050)
void Ped::sub_45C050()
{
    field_208_invulnerability = 0;
    Char_B4* pB4 = field_168_game_object;
    if (pB4)
    {
        pB4->field_80_sprite_ptr->field_2C &= ~4u;
    }
}

MATCH_FUNC(0x45c070)
void Ped::sub_45C070()
{
    if (this->field_208_invulnerability == 9999)
    {
        Char_B4* pB4 = this->field_168_game_object;
        if (pB4)
        {
            pB4->field_80_sprite_ptr->field_2C |= 4u;
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
void Ped::sub_45C0C0(gmp_map_zone* pZone)
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
    AllocCharB4_45C830(Fix16(xxx) + k_dword_67853C, Fix16(yyy) + k_dword_67853C, found_z + 1);
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

    sub_45C0C0(pZone);

    // TODO: missing inlines here, temp var shouldn't be needed
    Char_B4* pTmp = field_168_game_object;
    const u8 remap = get_remap_433BA0();
    pTmp->field_5_remap = remap;
    if (remap != 0xFF)
    {
        pTmp->field_80_sprite_ptr->SetRemap(remap);
    }

    field_27C_ped_state_2 = ped_state_2::Unknown_29;
    field_216_health = 100;
    field_208_invulnerability = 50;

    SetObjective(0, 9999);
    sub_463830(0, 9999);
}

MATCH_FUNC(0x45c410)
void Ped::sub_45C410()
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

    this->field_216_health = 100;
    this->field_238 = 2;
    this->field_208_invulnerability = 50;
    this->field_234_timer = 99;
    this->field_15C_player = pPlayer;
}

MATCH_FUNC(0x45c4b0)
void Ped::sub_45C4B0()
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
void Ped::sub_45C5C0()
{
    if (!this->field_16C_car && this->field_258_objective == objectives_enum::enter_car_as_driver_35 && this->field_25C_car_state == 35 &&
        this->field_168_game_object->field_10_char_state != Char_B4_state::Jumping_15 &&
        this->field_27C_ped_state_2 != ped_state_2::ped2_entering_a_car_6)
    {
        ChangeNextPedState1_45C500(ped_state_1::walking_0);
        ChangeNextPedState2_45C540(ped_state_2::ped2_walking_0);
        this->field_16C_car = 0;
        SetObjective(0, 9999);
        sub_463830(0, 9999);
        PedGroup* pGroup = this->field_164_ped_group;
        if (pGroup)
        {
            pGroup->sub_4C91B0();
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
                pPed->field_180 = pOther;
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
    pCar->field_7C_uni_num = field_238;
    pCar->field_76_last_seen_timer = 0;

    // TODO: inline ??
    if (pCar->field_88 == 2 || pCar->field_88 == 4 || pCar->field_88 == 3)
    {
        pCar->field_88 = 1;
    }
}

MATCH_FUNC(0x45c7a0)
void Ped::EnterCarAsDriver(Car_BC* pCar)
{
    sub_463830(0, 9999);
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
    field_168_game_object->field_7C_pPed = this;

    field_1AC_cam.y = ypos;
    field_1AC_cam.x = xpos;
    field_1AC_cam.z = zpos;

    DrawFlamesAndStartScreamTimer();
    SetSpriteSemiTransIfInvisible();
    sub_45C070();

    return 1;
}

MATCH_FUNC(0x45c900)
Ang16 Ped::get_field8_45C900()
{
    return field_15C_player->field_8;
}

MATCH_FUNC(0x45c920)
Fix16 Ped::GetPedVelocity_45C920()
{
    if (field_168_game_object)
    {
        return field_168_game_object->field_38_velocity; // velocity ??
    }
    else
    {
        if (field_16C_car)
        {
            return field_16C_car->GetVelocity_43A4C0();
        }
        return k_dword_678660;
    }
}

MATCH_FUNC(0x45c960)
Ang16 Ped::GetRotation()
{
    if (field_168_game_object != NULL)
    {
        return field_168_game_object->field_40_rotation;
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
    return field_15C_player->field_C;
}

WIP_FUNC(0x45c9d0)
Ang16 Ped::ComputeAimAngle_45C9D0()
{
    WIP_IMPLEMENTED;

    if (IsField238_45EDE0(2))
    {
        Ped* pNearest = gThreateningPedsList_678468.FindClosestPedInViewCone_4713C0(this->field_1AC_cam.x,
                                                                                    this->field_1AC_cam.y,
                                                                                    this->field_12C,
                                                                                    dword_6784E4);
        Ped* best = pNearest;

        if (!best)
        {
            word_6784F0 = dword_6784E4;
            best = FindBestTargetPed_Mode4_466BB0(3);
        }

        if (!best)
        {
            best = sub_466F40(3u);
        }

        if (best)
        {
            Fix16 xd = best->field_1AC_cam.x - field_1AC_cam.x;
            Fix16 yd = best->field_1AC_cam.y - field_1AC_cam.y;
            field_130 = Fix16::atan2_fixed_405320(xd, yd);
        }
        else
        {
            field_130 = field_12C;
        }
    }
    return field_130;
}

WIP_FUNC(0x45caa0)
void Ped::HandleClosePedInteraction_45CAA0()
{
    WIP_IMPLEMENTED;

    Ped* pNearPed; // eax
    Ped* pNearPed_; // esi
    PedGroup* pNearPedGroup; // eax
    s16 rng; // bp

    pNearPed = FindNearbyPed_466FB0();
    if (pNearPed)
    {
        if (abs_sub_less_than_epislon_45AE40(this->field_1AC_cam.z, pNearPed->field_1AC_cam.z))
        {
            this->field_188_last_char_punched = pNearPed_;
            pNearPedGroup = pNearPed_->field_164_ped_group;
            if (!pNearPedGroup || pNearPedGroup != this->field_164_ped_group)
            {
                pNearPed_->field_144 = this;
                if (this->field_168_game_object->field_68_animation_frame >= 3u)
                {
                    goto LABEL_27;
                }
                if (this->field_240_occupation == ped_ocupation_enum::mugger)
                {
                    if ((this->field_21C & 0x1000000) == 0)
                    {
                        this->field_250 = 8;
                    }
                    if ((pNearPed_->field_21C & 0x1000000) == 0)
                    {
                        pNearPed_->field_250 = 7;
                    }
                }
                else if ((u32)(rng_dword_67AB34->field_0_rng - pNearPed_->field_220) > 5)
                {
                    if ((pNearPed_->field_21C & 0x1000000) == 0)
                    {
                        pNearPed_->field_250 = 25;
                    }
                    pNearPed_->field_220 = rng_dword_67AB34->field_0_rng;
                }
                pNearPed_->field_204_killer_id = this->field_200_id;
                pNearPed_->field_290 = 10;
                pNearPed_->field_264 = 50;
                rng = stru_6F6784.get_int_4F7AE0(20);
                if (pNearPed_->field_216_health > 30)
                {
                    if (pNearPed_->IsField238_45EDE0(2))
                    {
                        if (this->field_240_occupation == ped_ocupation_enum::mugger)
                        {
                            pNearPed_->field_15C_player->field_2D4_scores.AddCash_592620(
                                -10 * pNearPed_->field_15C_player->field_6BC_multpliers.field_0);
                            this->field_229++;
                            if ((u8)field_229 > 9u)
                            {
                                this->field_226 = 1;
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
                    pNearPed_->field_168_game_object->field_16 = 1;
                    pNearPed_->field_168_game_object->field_10_char_state = 33;
                }
                else
                {
                LABEL_27:
                    if (pNearPed_->field_28C_threat_reaction == threat_reaction_enum::run_away_3)
                    {
                        pNearPed_->sub_463830(2, 9999);
                        pNearPed_->field_14C = this;
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
void Ped::sub_45CF20(Object_2C* a2)
{
    if (field_278_ped_state_1 != ped_state_1::dead_9 && field_278_ped_state_1 != ped_state_1::immobilized_8 && a2->field_18_model == 258 &&
        word_6787D0 < 10)
    {
        sub_45E4A0();
    }
}

STUB_FUNC(0x45d000)
char_type Ped::HandlePedHitByObject_45D000(Object_2C* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x45dd30)
char_type Ped::AddWeaponWithAmmo_45DD30(s32 weapon_kind, char_type ammo)
{
    if (ammo == 100) // max is 99, 100 means get default count
    {
        ammo = gWeapon_8_707018->get_ammo_5E3E80(weapon_kind);
    }

    if (weapon_kind >= 0xf) // car_bomb
    {
        if (field_16C_car)
        {
            return gWeapon_8_707018->allocate_5E3D50(weapon_kind, ammo, field_16C_car);
        }
        return 0;
    }

    if (field_15C_player)
    {
        return field_15C_player->sub_564960(weapon_kind, ammo);
    }

    if (field_170_selected_weapon && field_170_selected_weapon->field_1C_idx == weapon_kind)
    {
        return 0;
    }

    RemovePedWeapons_462510();
    ForceWeapon_46F600(weapon_kind);

    return 1;
}

WIP_FUNC(0x45de80)
bool Ped::HandlePickupCollision_45DE80(Object_2C* pPickUp)
{
    WIP_IMPLEMENTED;

    char_type bCollected;
    if (this->field_238 != 2) // is player ped type?
    {
        return 0;
    }

    if ((u8)bStartNetworkGame_7081F0 && gLucid_hamilton_67E8E0.GetMultiplayerGamemode_4C5BC0() == 3 &&
        gYouthful_einstein_6F8450.field_0_fugitive && gYouthful_einstein_6F8450.field_0_fugitive->field_2C4_player_ped == this)
    {
        return 0; // prevent pick ups if we are "it" in multiplayer?
    }

    s32 model = pPickUp->field_18_model;
    if (model == objects::secret_token_266)
    {
        // inc counter and remove pick up
        gLucid_hamilton_67E8E0.field_574_secret_tokens_collected++;
        gObject_5C_6F8F84->field_20_bUnCollectedTokens[pPickUp->field_26_varrok_idx] = 0;
        pPickUp->Dealloc_5291B0();
        return 1;
    }
    else
    {
        if (model <= 108)
        {
            model += 136;
        }

        if (model <= 227)
        {
            bCollected = AddWeaponWithAmmo_45DD30(model - 200, pPickUp->field_26_varrok_idx);
        }
        else
        {
            bCollected = field_15C_player->CollectPowerUp_564D60(model - 228);
        }

        if (bCollected)
        {
            if (field_15C_player->field_0_bIsUser)
            {
                gHud_2B00_706620->field_1080.sub_5D5600(model + 56);
            }
            pPickUp->Dealloc_5291B0();
        }
        return bCollected;
    }
}

WIP_FUNC(0x45e080)
void Ped::SpawnWeaponOnDeath_45E080()
{
    WIP_IMPLEMENTED;
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
                    case weapon_type::electro_batton:
                        v2 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(200, get_cam_x(), get_cam_y(), get_cam_z(), gDummyPedAng_6787A8);
                        if (v2)
                        {
                            // TODO: Prob setting the timer of Object_8 actually?
                            v2->field_C_pAny.pExplosion->field_4_idx = 9;
                        }
                        break;

                    case weapon_type::smg:
                        v2 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(201, get_cam_x(), get_cam_y(), get_cam_z(), gDummyPedAng_6787A8);
                        if (v2)
                        {
                            v2->field_C_pAny.pExplosion->field_4_idx = 9;
                        }
                        break;

                    case weapon_type::rocket:
                        v2 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(202, get_cam_x(), get_cam_y(), get_cam_z(), gDummyPedAng_6787A8);
                        if (v2)
                        {
                            v2->field_C_pAny.pExplosion->field_4_idx = 9;
                        }
                        break;

                    case weapon_type::shocker:
                        v2 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(203, get_cam_x(), get_cam_y(), get_cam_z(), gDummyPedAng_6787A8);
                        if (v2)
                        {
                            v2->field_C_pAny.pExplosion->field_4_idx = 9;
                        }
                        break;

                    case weapon_type::molotov:
                        v2 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(204, get_cam_x(), get_cam_y(), get_cam_z(), gDummyPedAng_6787A8);
                        if (v2)
                        {
                            v2->field_C_pAny.pExplosion->field_4_idx = 9;
                        }
                        break;

                    case weapon_type::grenade:
                        v2 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(205, get_cam_x(), get_cam_y(), get_cam_z(), gDummyPedAng_6787A8);
                        if (v2)
                        {
                            v2->field_C_pAny.pExplosion->field_4_idx = 9;
                        }
                        break;

                    case weapon_type::shotgun:
                        v2 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(206, get_cam_x(), get_cam_y(), get_cam_z(), gDummyPedAng_6787A8);
                        if (v2)
                        {
                            v2->field_C_pAny.pExplosion->field_4_idx = 9;
                        }
                        break;

                    case weapon_type::flamethrower:
                        v2 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(208, get_cam_x(), get_cam_y(), get_cam_z(), gDummyPedAng_6787A8);
                        if (v2)
                        {
                            v2->field_C_pAny.pExplosion->field_4_idx = 9;
                        }
                        break;

                    case weapon_type::silence_smg:
                        v2 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(209, get_cam_x(), get_cam_y(), get_cam_z(), gDummyPedAng_6787A8);
                        if (v2)
                        {
                            v2->field_C_pAny.pExplosion->field_4_idx = 9;
                        }
                        break;

                    case weapon_type::dual_pistol:
                        v2 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(210, get_cam_x(), get_cam_y(), get_cam_z(), gDummyPedAng_6787A8);
                        if (v2)
                        {
                            v2->field_C_pAny.pExplosion->field_4_idx = 9;
                        }
                        break;
                    default:
                        return;
                }
            }
        }
    }
}

STUB_FUNC(0x45e4a0)
void Ped::sub_45E4A0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x45ea00)
void Ped::sub_45EA00()
{
    NOT_IMPLEMENTED;
}

// https://decomp.me/scratch/jJ6aF
WIP_FUNC(0x45eb60)
void Ped::Deallocate_45EB60()
{
    WIP_IMPLEMENTED;
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
        case ped_ocupation_enum::unknown_10:
            --byte_6787CE;
            break;
        case ped_ocupation_enum::unknown_5:
            if (--byte_6787D3 < 0)
            {
                byte_6787D3 = 0;
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
            Ped::sub_462550();
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

    if (field_17C_pZone)
    {
        if (field_17C_pZone->field_141)
        {
            field_17C_pZone->field_141 = 0;
        }
    }
    field_164_ped_group = field_164_ped_group;
    if (field_164_ped_group)
    {
        if (field_23C == 99)
        {
            if (field_164_ped_group->IsAllMembersInSomeCar_4CAA20())
            {
                field_164_ped_group->sub_4C8E90();
            }
            else
            {
                if ((field_240_occupation != ped_ocupation_enum::elvis_leader && field_240_occupation != ped_ocupation_enum::elvis))
                {
                    field_164_ped_group->DestroyGroup_4C93A0();
                }
                else if (field_164_ped_group->sub_4C9150())
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

    // Problem Here:
    field_234_timer = 2;
    field_21C_bf.b0 = false;

    if (field_16C_car)
    {
        if (!field_248_enter_car_as_passenger)
        {
            field_16C_car->field_54_driver = 0;
            if (field_238 == 5)
            {
                if (field_16C_car->GetCarKind_4343B0() == 1)
                {
                    field_16C_car->field_7C_uni_num = 3;
                }
            }
        }
    }
    Ped::SetObjective(objectives_enum::no_obj_0, 9999);
    Ped::sub_463830(0, 9999);
    field_278_ped_state_1 = ped_state_1::dead_9;
    field_27C_ped_state_2 = ped_state_2::Unknown_15;
}

MATCH_FUNC(0x45edc0)
char_type Ped::sub_45EDC0()
{
    if (this->field_238 == 2)
    {
        if (this->field_240_occupation != ped_ocupation_enum::empty)
        {
            return 1;
        }
    }
    return 0;
}

MATCH_FUNC(0x45ede0)
bool Ped::IsField238_45EDE0(s32 a2)
{
    return field_238 == a2 ? true : false;
}

// https://decomp.me/scratch/pXF8g
MATCH_FUNC(0x45ee00)
void Ped::sub_45EE00(u32 occupation)
{
    if (field_240_occupation <= (u32) ped_ocupation_enum::bank_robber)
    {
        switch ( field_240_occupation )
        {
            case ped_ocupation_enum::player:
            case ped_ocupation_enum::empty:
            case ped_ocupation_enum::unknown_1:
            case ped_ocupation_enum::dummy:
            case ped_ocupation_enum::unknown_2:
            case ped_ocupation_enum::driver:
            case ped_ocupation_enum::unknown_3:
            case ped_ocupation_enum::unknown_4:
            case ped_ocupation_enum::unknown_6:
            case ped_ocupation_enum::driver_2:
            case ped_ocupation_enum::unknown_7:
            case ped_ocupation_enum::unknown_8:
            case ped_ocupation_enum::unknown_9:
            case ped_ocupation_enum::psycho:
            case ped_ocupation_enum::mugger:
            case ped_ocupation_enum::car_thief:
            case ped_ocupation_enum::bank_robber:
                field_240_occupation = occupation;
                break;
            case ped_ocupation_enum::unknown_5:
                if ( --byte_6787D3 < 0 )
                    byte_6787D3 = 0;
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
    for (gmp_map_zone* pZoneIter = gMap_0x370_6F6268->sub_4DF6A0(field_1AC_cam.x.ToInt(), field_1AC_cam.y.ToInt()); pZoneIter;
         pZoneIter = gMap_0x370_6F6268->next_zone_4DF770())
    {
        if (bSkip_trains_67D550 || pZoneIter->field_0_zone_type != 6)
        {
            if (!bSkip_buses_67D558)
            {
                if (stru_6F6784.get_int_4F7AE0(100) > 90 && byte_6787D3 < 5 && pZoneIter->field_0_zone_type == 7 &&
                    !gPublicTransport_181C_6FF1D4->is_bus_full_579AF0())
                {
                    if (field_25C_car_state != 37 && field_25C_car_state != 38 && this->field_278_ped_state_1 == ped_state_1::walking_0)
                    {
                        sub_45EE00(8);
                        sub_463830(30, 9999);
                        ++byte_6787D3;
                    }
                }
            }
        }
        else
        {
            TrainStation_34* pTrainStation = gPublicTransport_181C_6FF1D4->TrainStationForZone_57B4B0(pZoneIter);
            if (stru_6F6784.get_int_4F7AE0(100) > 90 && byte_6787D3 < 5)
            {
                if (field_25C_car_state != 37 && field_25C_car_state != 38 && field_25C_car_state != 12)
                {
                    Train_58* pTrain = pTrainStation->field_18;
                    if (pTrain)
                    {
                        if (pTrain->field_10_carriages[0]->field_84_car_info_idx == car_model_enum::TRAIN)
                        {
                            sub_45EE00(9);
                            sub_463830(29, 9999);
                            this->field_154_target_to_enter = pTrainStation->field_18->field_10_carriages[0];
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
    if (field_25C_car_state == 2 && field_226 == 1)
    {
        Ped::sub_463830(0, 9999);
    }
    switch (field_258_objective)
    {
        case objectives_enum::no_obj_0:
            if (!field_20e)
            {
                if (field_218_objective_timer == 0)
                {
                    Ped* pTarget = sub_467070();
                    if (!pTarget || pTarget->field_20e)
                    {
                        Ped::SetObjective(objectives_enum::no_obj_0, 9999);
                    }
                    else
                    {
                        Ped::SetObjective(objectives_enum::punch_char_23, 9999);
                        field_148_objective_target_ped = pTarget;
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
            if (field_148_objective_target_ped && field_148_objective_target_ped->field_16C_car)
            {
                Ped::SetObjective(objectives_enum::no_obj_0, 9999);
                Ped::sub_463830(0, 9999);
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
                    Ped::sub_463830(0, 0);
                    field_21C_bf.b2 = false;
                }
            }
            break;

        case objectives_enum::wait_on_foot_26:
            field_12C + word_678590; // non used
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
                Ped::sub_45EE00(3);
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

WIP_FUNC(0x45ff60)
void Ped::CarThief_AI_45FF60()
{
    WIP_IMPLEMENTED;

    Car_BC* pNearestSteal; // eax
    Car_BC* pNearestSteal_; // edi
    Car_BC* pCar_; // ecx
    Car_BC* pCar; // eax
    Fix16 xd;
    Fix16 yd;

    if (this->field_25C_car_state == 2 && this->field_226 == 1)
    {
        sub_463830(0, 9999);
    }

    switch (this->field_258_objective)
    {
        case objectives_enum::no_obj_0:
            if ((this->field_21C & 0x8000000) != 0)
            {
                if (field_150_target_objective_car)
                {
                    if (field_150_target_objective_car->field_88 == 5)
                    {
                        goto kill_and_ret;
                    }
                }
                else
                {
                    ForceDoNothing_462590();
                    this->field_240_occupation = ped_ocupation_enum::dummy;
                }
            }
            if (this->field_20e)
            {
                this->field_218_objective_timer = 40;
                ChangeNextPedState1_45C500(0);
                ChangeNextPedState2_45C540(0);
            }
            else if (!this->field_218_objective_timer)
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
                            !pNearestSteal_->is_bus_43A1F0() && !pNearestSteal_->field_4_passengers_list.field_0_pFirstPed &&
                            pNearestSteal_->field_7C_uni_num == 3)
                        {
                            SetObjective(objectives_enum::enter_car_as_driver_35, 9999);
                            this->field_150_target_objective_car = pNearestSteal_;
                            this->field_248_enter_car_as_passenger = 0;
                            this->field_24C_target_car_door = 0;
                        }
                    }
                }
            }
            return;

        case objectives_enum::flee_on_foot_till_safe_1:
            if (this->field_225_objective_status == 1)
            {
                goto LABEL_52;
            }
            return;

        case objectives_enum::time_waited_in_car_31:
            pCar = this->field_16C_car;
            if (pCar)
            {
                if (pCar->field_88 == 5)
                {
                    goto kill_and_ret;
                }
                if (this->field_218_objective_timer > 0x258u)
                {
                    SetObjective(objectives_enum::leave_car_36, 9999);
                    this->field_150_target_objective_car = this->field_16C_car;
                }
            }
            else
            {
            LABEL_52:
                SetObjective(objectives_enum::no_obj_0, 40);
            }
            return;

        case objectives_enum::enter_car_as_driver_35:
            if (field_225_objective_status == 1)
            {
                if (this->field_150_target_objective_car->field_88 != 5)
                {
                    if (this->field_27C_ped_state_2 == ped_state_2::Unknown_17)
                    {
                        SetObjective(objectives_enum::no_obj_0, 40);
                        sub_463830(0, 9999);
                    }
                    else
                    {
                        if ((this->field_21C & 0x1000000) == 0)
                        {
                            this->field_250 = 15;
                        }
                        SetObjective(objectives_enum::time_waited_in_car_31, 0);
                        pCar_ = this->field_16C_car;
                        this->field_150_target_objective_car = pCar_;
                        pCar_->InitCarAIControl_440590();
                        field_150_target_objective_car->sub_43AF40();
                    }
                    return;
                }
            kill_and_ret:
                Kill_46F9D0();
                return;
            }

            if (field_225_objective_status == 2)
            {
                SetObjective(objectives_enum::no_obj_0, 40);
                sub_463830(0, 9999);
            }
            else if (this->field_16C_car && this->field_150_target_objective_car->field_88 == 5)
            {
                goto kill_and_ret;
            }

            xd = this->field_1B8_target_x - this->field_1AC_cam.x;
            yd = this->field_1BC_target_y - this->field_1AC_cam.y;

            xd = Fix16::Abs(xd);
            yd = Fix16::Abs(yd);
            // TODO: Might be min?
            if (Fix16::Max_44E540(xd, yd) > k_dword_678680)
            {
                SetObjective(objectives_enum::no_obj_0, 9999);
                sub_463830(0, 9999);
                this->field_218_objective_timer = 40;
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
        default:
            return;
    }
}

WIP_FUNC(0x460820)
void Ped::sub_460820()
{
    WIP_IMPLEMENTED;

    s32 objective; // eax
    u8 objectiveStatus; // al
    Car_BC* pTargetObjCar; // ecx
    Car_BC* pTargetObjCar_; // ecx
    Car_BC* pTargetObjCar__; // ecx
    Car_BC* target_objective_car; // eax
    Car_BC* pCar_; // ecx
    Car_BC* pNearestTaxi; // eax
    Car_BC* pTargetCar; // edi
    Sprite* pSprite; // eax

    if (this->field_25C_car_state == 2 && this->field_226 == 1)
    {
        sub_463830(0, 9999);
    }

    objective = this->field_258_objective;
    if (objective)
    {
        if (objective == objectives_enum::time_waited_in_car_31)
        {
            if (field_150_target_objective_car->GetVelocity_43A4C0() != k_dword_678660)
            {
                this->field_218_objective_timer = 0;
            }
            target_objective_car = this->field_150_target_objective_car;
            if (target_objective_car->field_88 == 5)
            {
                Kill_46F9D0();
            }
            else
            {
                if (target_objective_car->field_8C >= 3u)
                {
                    this->field_21C |= 0x20000000u;
                }
                if (!target_objective_car->field_54_driver || (this->field_21C & 0x20000000) != 0)
                {
                    SetObjective(objectives_enum::leave_car_36, 9999);
                    sub_45EE00(3);
                    pCar_ = this->field_16C_car;
                    this->field_238 = 3;
                    this->field_150_target_objective_car = pCar_;
                }
                else if (this->field_218_objective_timer == 150)
                {
                    SetObjective(objectives_enum::flee_char_always_once_car_stopped_6, 9999);
                    this->field_148_objective_target_ped = this->field_16C_car->field_54_driver;
                }
            }
        }
        else
        {
            if (objective != objectives_enum::enter_car_as_driver_35)
            {
                return;
            }
            objectiveStatus = this->field_225_objective_status;
            if (objectiveStatus == 1)
            {
                pTargetObjCar = this->field_150_target_objective_car;
                if (pTargetObjCar->field_88 == 5)
                {
                    // kill_and_ret:
                    Kill_46F9D0();
                    return;
                }
                if ((this->field_21C & 0x1000000) == 0)
                {
                    this->field_250 = 6;
                }
                pTargetObjCar->sub_43AF40();
                SetObjective(objectives_enum::time_waited_in_car_31, 0);
                this->field_150_target_objective_car = this->field_16C_car;
            }
            else
            {
                if (objectiveStatus == 2)
                {
                    pTargetObjCar_ = this->field_150_target_objective_car;
                    if (pTargetObjCar_->field_88 != 5)
                    {
                        pTargetObjCar_->sub_43AF40();
                    }
                    SetObjective(objectives_enum::no_obj_0, 40);
                    sub_463830(0, 9999);
                    return;
                }

                if (this->field_278_ped_state_1 != ped_state_1::in_car_10)
                {
                    Fix16 dx_ = this->field_1B8_target_x - this->field_1AC_cam.x;
                    Fix16 dy_ = this->field_1BC_target_y - this->field_1AC_cam.y;

                    dx_ = Fix16::Abs(dx_);
                    dy_ = Fix16::Abs(dy_);

                    // TODO: Might be Min()?
                    if (Fix16::Max_44E540(dx_, dy_) > k_dword_678658 || (this->field_21C & 0x20000) != 0)
                    {
                        pTargetObjCar__ = this->field_150_target_objective_car;
                    }
                    else
                    {
                        pTargetObjCar__ = this->field_150_target_objective_car;
                        if (!pTargetObjCar__->field_4_passengers_list.field_0_pFirstPed && pTargetObjCar__->field_88 != 5)
                        {
                            return;
                        }
                    }
                    pTargetObjCar__->sub_43AF40();
                    SetObjective(objectives_enum::no_obj_0, 9999);
                    sub_463830(0, 9999);
                    this->field_240_occupation = ped_ocupation_enum::dummy;
                    this->field_238 = 3;
                    return;
                }

                if (this->field_150_target_objective_car->field_88 == 5)
                {
                    //goto kill_and_ret;
                    Kill_46F9D0();
                    return;
                }
            }
        }
    }
    else if (this->field_20e)
    {
        this->field_218_objective_timer = 40;
        ChangeNextPedState1_45C500(0);
        ChangeNextPedState2_45C540(0);
    }
    else if (!this->field_218_objective_timer)
    {
        pNearestTaxi = gTaxi_4_704130->GetTaxiNear_457BF0(this->field_1AC_cam.x, this->field_1AC_cam.y);
        pTargetCar = pNearestTaxi;
        if (pNearestTaxi)
        {
            pSprite = pNearestTaxi->field_50_car_sprite;
            Fix16 dx = pSprite->field_14_xy.x - this->field_1AC_cam.x;
            Fix16 dy = pSprite->field_14_xy.y - this->field_1AC_cam.y;
            Fix16 dy_abs = Fix16::Abs(dy);
            Fix16 dx_abs = Fix16::Abs(dx);
            if (Fix16::Max_44E540(dx_abs, dy_abs) < k_dword_678658)
            {
                if ((this->field_21C & 0x1000000) == 0)
                {
                    this->field_250 = 5;
                }
                sub_463830(0, 9999);
                SetObjective(objectives_enum::enter_car_as_driver_35, 9999);
                this->field_150_target_objective_car = pTargetCar;
                this->field_248_enter_car_as_passenger = 1;
                this->field_24C_target_car_door = 3;
                pTargetCar->sub_43AF60();
            }
        }
    }
}

WIP_FUNC(0x461290)
void Ped::sub_461290()
{
    WIP_IMPLEMENTED;

    Car_BC* pCar;
    Ped* field_54_driver;
    Car_BC* pTargetCar;
    Car_Door_10* pDoor;
    Car_BC* pCar_;
    Fix16 y;
    Car_BC* pBus;
    Char_B4* game_object;
    Car_BC* target_to_enter;

    if (this->field_25C_car_state == 2 && this->field_226 == 1)
    {
        sub_463830(0, 9999);
    }

    switch (this->field_258_objective)
    {
        case objectives_enum::time_waited_in_car_31:
            pCar_ = this->field_16C_car;
            goto LABEL_23;

        case objectives_enum::objective_34:
            if (this->field_25C_car_state == 36 && this->field_226 == 1)
            {
                goto LABEL_21;
            }

            pCar_ = this->field_16C_car;
            if (pCar_)
            {
            LABEL_23:
                if (pCar_->field_88 == 5)
                {
                    pCar_->field_4_passengers_list.RemovePed_471240(this);
                    Kill_46F9D0();
                }
            }
            break;

        case objectives_enum::enter_car_as_driver_35:
            if (this->field_225_objective_status == objective_status::passed_1)
            {
                if (--byte_6787D3 < 0)
                {
                    byte_6787D3 = 0;
                }
                pCar = this->field_16C_car;
                field_54_driver = pCar->field_54_driver;
                if (field_54_driver && field_54_driver->field_15C_player)
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
                pTargetCar = this->field_150_target_objective_car;
                if (pTargetCar->field_88 == 5)
                {
                    pTargetCar->sub_43AF40();
                    SetObjective(objectives_enum::no_obj_0, 9999);
                    sub_463830(0, 9999);
                    this->field_240_occupation = ped_ocupation_enum::dummy;
                    this->field_238 = 3;
                }
                else
                {
                    pDoor = pTargetCar->GetDoor(this->field_24C_target_car_door);
                    pDoor->sub_439E60();
                }
            }
            return;

        case objectives_enum::leave_train_38:
            if (this->field_225_objective_status != objective_status::not_finished_0)
            {
            LABEL_21:
                this->field_238 = 3;
                sub_45EE00(3);
                sub_463830(0, 9999);
                SetObjective(objectives_enum::flee_on_foot_till_safe_1, 9999);
                y = this->field_1AC_cam.y;
                this->field_1B8_target_x = this->field_1AC_cam.x;
                this->field_1BC_target_y = y;
            }
            else if (this->field_150_target_objective_car->field_88 == 5)
            {
                Kill_46F9D0();
            }
            break;
        default:
            pBus = gPublicTransport_181C_6FF1D4->sub_579AD0();
            if (pBus)
            {
                sub_463830(0, 9999);
                SetObjective(objectives_enum::enter_car_as_driver_35, 9999);
                game_object = this->field_168_game_object;
                target_to_enter = this->field_154_target_to_enter;
                this->field_150_target_objective_car = pBus;
                game_object->field_84 = target_to_enter;
                this->field_168_game_object->field_38_velocity = k_dword_678660;
                this->field_24C_target_car_door = 1;
            }
            break;
    }
}

MATCH_FUNC(0x461530)
void Ped::sub_461530()
{
    if (this->field_25C_car_state == 2 && this->field_226 == 1)
    {
        sub_463830(0, 9999);
    }

    switch (field_25C_car_state)
    {
        case 37:
            if (field_154_target_to_enter->GetVelocity_43A4C0() != k_dword_678660)
            {
                this->field_238 = 3;
                sub_45EE00(3);
                sub_463830(0, 9999);
                SetObjective(objectives_enum::no_obj_0, 9999);
                this->field_1B8_target_x = this->field_1AC_cam.x;
                this->field_1BC_target_y = this->field_1AC_cam.y;
            }
            break;

        case 38:
            if (this->field_226)
            {
                this->field_238 = 3;
                sub_45EE00(3);
                sub_463830(0, 9999);
                SetObjective(objectives_enum::flee_on_foot_till_safe_1, 9999);
                this->field_1B8_target_x = this->field_1AC_cam.x;
                this->field_1BC_target_y = this->field_1AC_cam.y;
            }
            break;
    }
}

WIP_FUNC(0x461630)
void Ped::sub_461630()
{
    WIP_IMPLEMENTED;

    s16 rng_val; // ax
    Ped* f180_; // ecx
    Ped* f180; // edx
    Ped* f180__; // ecx
    Car_BC* target_objective_car; // eax

    if (this->field_25C_car_state == 2 && this->field_226 == 1)
    {
        sub_463830(0, 9999);
    }

    switch (this->field_240_occupation)
    {
        case ped_ocupation_enum::driver_2:
            if (this->field_168_game_object)
            {
                if (this->field_278_ped_state_1 != ped_state_1::immobilized_8)
                {
                    rng_val = stru_6F6784.get_int_4F7AE0(40);
                    if (bDont_get_car_back_67D4F5)
                    {
                        rng_val = 6;
                    }
                    if (this->field_180->field_20e)
                    {
                        rng_val = 6;
                    }
                    else if (this->field_17C_pZone)
                    {
                        rng_val = 19;
                    }

                    switch (rng_val)
                    {
                        case 19:
                            this->field_238 = 6;
                            this->field_240_occupation = ped_ocupation_enum::unknown_9;
                            if ((field_21C & 0x1000000) == 0)
                            {
                                this->field_250 = 14;
                            }
                            ForceDoNothing_462590();
                            SetObjective(objectives_enum::kill_char_on_foot_20, 9999);
                            this->field_148_objective_target_ped = this->field_180;
                            ForceWeapon_46F600(weapon_type::pistol);
                            break;

                        case 7:
                        case 16:
                        case 32:
                            this->field_238 = 6;
                            this->field_240_occupation = ped_ocupation_enum::unknown_8;
                            if ((field_21C & 0x1000000) == 0)
                            {
                                this->field_250 = 13;
                            }
                            SetObjective(objectives_enum::enter_car_as_driver_35, 9999);
                            this->field_24C_target_car_door = 0;
                            this->field_150_target_objective_car = field_140;
                            if (field_140->field_88 == 5)
                            {
                                this->field_238 = 3;
                                this->field_240_occupation = ped_ocupation_enum::unknown_7;
                                ForceDoNothing_462590();
                                SetObjective(objectives_enum::flee_char_on_foot_till_safe_2, 9999);
                                f180_ = this->field_180;
                                this->field_28C_threat_reaction = threat_reaction_enum::run_away_3;
                                this->field_148_objective_target_ped = f180_;
                                this->field_180 = 0;
                            }
                            else
                            {
                                ForceWeapon_46F600(weapon_type::pistol);
                                this->field_248_enter_car_as_passenger = 0;
                            }
                            break;

                        default:
                            this->field_238 = 3;
                            this->field_240_occupation = ped_ocupation_enum::unknown_7;
                            this->field_28C_threat_reaction = threat_reaction_enum::run_away_3;
                            ForceDoNothing_462590();
                            SetObjective(objectives_enum::flee_char_on_foot_till_safe_2, 9999);
                            f180 = this->field_180;
                            this->field_180 = 0;
                            this->field_148_objective_target_ped = f180;
                            break;
                    }
                }
                else
                {
                    if (this->field_140->field_88 == 5)
                    {
                        this->field_238 = 3;
                        this->field_240_occupation = ped_ocupation_enum::unknown_7;
                        ForceDoNothing_462590();
                        SetObjective(objectives_enum::flee_char_on_foot_till_safe_2, 9999);
                        f180__ = this->field_180;
                        this->field_180 = 0;
                        this->field_148_objective_target_ped = f180__;
                    }
                }
            }
            return;

        case ped_ocupation_enum::unknown_8:
            if (field_225_objective_status != 1)
            {
                goto LABEL_40;
            }

            target_objective_car = this->field_150_target_objective_car;
            if (target_objective_car->field_88 == 5)
            {
                Kill_46F9D0();
            }
            else
            {
                this->field_240_occupation = ped_ocupation_enum::driver;
                this->field_238 = 3;
                if (target_objective_car)
                {
                    target_objective_car->field_7C_uni_num = 3;
                    target_objective_car->field_76_last_seen_timer = 0;
                    SetObjective(objectives_enum::no_obj_0, 9999);
                }
                else
                {
                    SetObjective(objectives_enum::no_obj_0, 9999);
                }
            }
            return;

        case ped_ocupation_enum::unknown_9:
            if (this->field_225_objective_status == 1)
            {
                this->field_238 = 6;
                this->field_240_occupation = ped_ocupation_enum::unknown_8;
                if (field_140 && field_140->field_88 != 5)
                {
                    SetObjective(objectives_enum::enter_car_as_driver_35, 9999);
                    this->field_248_enter_car_as_passenger = 0;
                    this->field_150_target_objective_car = field_140;
                    this->field_24C_target_car_door = 0;
                    return;
                }
                goto LABEL_41;
            }

            if (!field_140)
            {
                if (field_140->field_88 == 5)
                {
                    this->field_140 = 0;
                }
            }

            if (!this->field_140)
            {
                this->field_225_objective_status = 0;
            }

        LABEL_40:
            if (field_225_objective_status == 2)
            {
            LABEL_41:
                this->field_240_occupation = ped_ocupation_enum::dummy;
                SetObjective(objectives_enum::no_obj_0, 9999);
                sub_463830(0, 9999);
                this->field_238 = 3;
            }
            return;

        case ped_ocupation_enum::unknown_7:
            if (this->field_225_objective_status == 1)
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
        field_16C_car->field_A6 |= 0x20u;
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

STUB_FUNC(0x461a60)
void Ped::UpdateFacingAngle_461A60()
{
    NOT_IMPLEMENTED;
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
                    if (field_20e || byte_6787D2 || stru_6F6784.get_int_4F7AE0(1000) >= 2)
                    {
                        Ped::EnterPublicTransport_45EE70();
                    }
                    else if (gTaxi_4_704130->field_0)
                    {
                        field_240_occupation = ped_ocupation_enum::unknown_4;
                        if (field_238 == 3)
                        {
                            field_238 = 6;
                        }
                        Ped::SetObjective(objectives_enum::no_obj_0, 40);
                        ++byte_6787D2;
                    }
                }
            }
            break;
        case ped_ocupation_enum::unknown_6:
            Ped::sub_461530();
            break;
        case ped_ocupation_enum::unknown_5:
            Ped::sub_461290();
            break;
        case ped_ocupation_enum::unknown_2:
            byte_61A8A0 = 0;
            break;
        case ped_ocupation_enum::driver:
            byte_61A8A0 = 0;
            if (field_16C_car)
            {
                if (field_16C_car->field_88 == 5)
                {
                    Ped::Kill_46F9D0();
                }
            }
            else
            {
                field_240_occupation = ped_ocupation_enum::dummy;
            }
            break;
        case ped_ocupation_enum::unknown_4:
            Ped::sub_460820();
            break;

        case ped_ocupation_enum::mugger:
            Ped::Mugger_AI_45F360();
            break;
        case ped_ocupation_enum::car_thief:
            Ped::CarThief_AI_45FF60();
            break;
        case ped_ocupation_enum::driver_2:
        case ped_ocupation_enum::unknown_8:
        case ped_ocupation_enum::unknown_9:
            Ped::sub_461630();
            break;
        case ped_ocupation_enum::unknown_7:
            if (field_225_objective_status)
            {
                Ped::SetObjective(objectives_enum::no_obj_0, 9999);
                Ped::sub_463830(0, 9999);
                field_21C_bf.b2 = 0;
            }
            break;
        case ped_ocupation_enum::road_block_tank_man:
            Ped::RoadBlockTank_AI_4619F0();
            break;
        case ped_ocupation_enum::unknown_18:
            if (!gPedManager_6787BC->field_7_make_all_muggers)
            {
                field_240_occupation = ped_ocupation_enum::dummy;
                field_238 = 3;
                Ped::ForceDoNothing_462590();
            }
            break;
        case ped_ocupation_enum::unknown_10:
            if (field_25C_car_state == 20 && field_17C_pZone != NULL && field_14C->field_15C_player != NULL)
            {
                u8 idx = field_14C->field_15C_player->field_2E_idx;
                if (!field_17C_pZone->IsRespectNegativeForPlayer_4BEF10(idx))
                {
                    Ped::sub_463830(0, 9999);
                }
            }
            break;
        case ped_ocupation_enum::driver_3:
            if (field_258_objective > objectives_enum::no_obj_0)
            {
                if (field_258_objective > objectives_enum::flee_char_on_foot_till_safe_2)
                {
                    if (field_258_objective == objectives_enum::leave_car_36)
                    {
                        if (field_150_target_objective_car->field_88 == 5)
                        {
                            Ped::Kill_46F9D0();
                        }
                        else if (field_225_objective_status == objective_status::passed_1)
                        {
                            if (field_180)
                            {
                                Ped::SetObjective(objectives_enum::flee_char_on_foot_till_safe_2, 9999);
                                field_148_objective_target_ped = field_180;
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
                        field_240_occupation = ped_ocupation_enum::dummy;
                        Ped::SetObjective(objectives_enum::no_obj_0, 9999);
                        Ped::sub_463830(0, 9999);
                    }
                }
            }
            break;
        default:
            return;
    }
}

MATCH_FUNC(0x462280)
void Ped::sub_462280()
{
    byte_61A8A0 = 1;
    Ped::Occupation_AI_461F20();
    byte_61A8A3 = 0;
    if (byte_61A8A0)
    {
        if (field_164_ped_group)
        {
            if (field_23C != 99)
            {
                if (field_21C_bf.b2 == 0)
                {
                    field_164_ped_group->sub_4CA5E0(field_23C);
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
                byte_61A8A3 = field_164_ped_group->sub_433370() != 1;
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
        if (field_238 != 2)
        {
            if (field_258_objective != objectives_enum::flee_char_on_foot_always_3 &&
                field_258_objective != objectives_enum::flee_char_always_once_car_stopped_6)
            {
                if (field_25C_car_state != 3 && field_25C_car_state != 7)
                {
                    Ped::Threat_Reaction_AI_465270();
                    if (field_144)
                    {
                        if ((field_21C & 4) == 0)
                        {
                            Ped::sub_465B20();
                        }
                        field_144 = 0;
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
void Ped::sub_4624A0()
{
    if (field_164_ped_group)
    {
        field_164_ped_group->DestroyGroup_4C93A0();
    }

    if (field_168_game_object)
    {
        if (field_168_game_object->field_88_obj_2c.field_0_p18)
        {
            field_168_game_object->field_88_obj_2c.sub_5A7010();
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
            sub_462550();
        }
        field_178 = 0;
    }
}

MATCH_FUNC(0x462510)
void Ped::RemovePedWeapons_462510()
{
    if (field_170_selected_weapon)
    {
        field_21C_bf.b11 = 0;
        gWeapon_8_707018->deallocate_5E3CB0(field_170_selected_weapon);
        field_170_selected_weapon = 0;
    }
}

MATCH_FUNC(0x462550)
void Ped::sub_462550()
{
    if (field_174_pWeapon)
    {
        field_21C_bf.b11 = 0;
        gWeapon_8_707018->deallocate_5E3CB0(field_174_pWeapon);
        field_174_pWeapon = 0;
    }
}

MATCH_FUNC(0x462590)
void Ped::ForceDoNothing_462590()
{
    SetObjective(0, 9999);
    sub_463830(0, 9999);

    field_21C &= ~4u;

    if (field_16C_car)
    {
        field_278_ped_state_1 = ped_state_1::in_car_10;
        field_27C_ped_state_2 = ped_state_2::ped2_driving_10;

        if (field_16C_car->field_5C)
        {
            if (field_16C_car->field_5C->field_28_junc_idx > 0)
            {
                gRouteFinder_6FFDC8->CancelRoute_589930(field_16C_car->field_5C->field_28_junc_idx);
            }
        }

        if (field_16C_car->field_60)
        {
            gHamburger_500_678E30->Cancel_474CC0(field_16C_car->field_60);
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
        if (field_168_game_object->field_10_char_state == Char_B4_state::Jumping_15)
        {
            byte_61A8A4 = field_278_ped_state_1 == ped_state_1::entering_car_3;
        }
        field_21C_bf.b11 = false;
        field_168_game_object->field_80_sprite_ptr->field_28_num = 24;
    }
    if (field_168_game_object->IsOnScreen_545700() == true)
    {
        field_20e = 0;
        ++gNumPedsOnScreen_6787EC;
    }
}

WIP_FUNC(0x4626b0)
char_type Ped::StateMachineTick_4626B0()
{
    WIP_IMPLEMENTED;

    Weapon_30* pWeapon; // eax
    s32 occupation; // eax
    Player* field_15C_player; // ecx
    Car_BC* field_16C_car; // edx
    Sprite* field_50_car_sprite; // eax
    Char_B4* pB4_; // eax
    s32 occupation_; // eax
    u16 f20E; // dx
    s32 occupation__; // eax
    PedGroup* pGroup; // ecx
    Car_BC* pCar_; // eax
    Sprite* pCarSprite_; // eax
    Car_BC* pCar__; // eax
    Char_B4* pB4; // eax
    s32 f278_; // ecx
    Car_BC* pCar; // edx
    Sprite* pCarSprite; // eax

    switch (this->field_238)
    {
        case 2:
            if (bDo_invulnerable_67D4CB)
            {
                this->field_208_invulnerability = 9999;
            }

            pWeapon = this->field_170_selected_weapon;

            if (pWeapon)
            {
                pWeapon->field_4 = 0;
            }

            occupation = this->field_240_occupation;
            this->field_288_threat_search = threat_search_enum::no_threats_0;
            this->field_28C_threat_reaction = threat_reaction_enum::no_reaction_0;

            if ((unsigned int)occupation >= ped_ocupation_enum::unknown_1)
            {
                this->field_240_occupation = ped_ocupation_enum::player;
            }
            field_15C_player = this->field_15C_player;
            this->field_230 = 2;
            field_15C_player->field_64 = 0;

            if (!this->field_168_game_object)
            {
                this->field_210_shock_counter = 0;
                goto LABEL_37;
            }

            if (this->field_225_objective_status == 2)
            {
                Ped::SetObjective(objectives_enum::no_obj_0, 9999);
                Ped::sub_463830(0, 9999);
            }

            if (this->field_258_objective == objectives_enum::leave_car_36 &&
                (this->field_225_objective_status == 1 || this->field_168_game_object->field_10_char_state == Char_B4_state::Jumping_15))
            {
                Ped::SetObjective(objectives_enum::no_obj_0, 9999);
                Ped::sub_463830(0, 9999);
            }
            ++gNumPedsOnScreen_6787EC;
            ++this->field_20e;
            byte_6787C4 = 1;
            if (this->field_168_game_object->field_10_char_state == Char_B4_state::Jumping_15)
            {
                byte_61A8A4 = this->field_278_ped_state_1 == ped_state_1::entering_car_3;
            }

            if (field_278_ped_state_1 == ped_state_1::dead_9 || field_278_ped_state_1 == ped_state_1::immobilized_8)
            {
                byte_61A8A4 = 0;
            }
            else
            {
                this->field_168_game_object->field_80_sprite_ptr->field_28_num = 25;
            }
            if (field_168_game_object->IsOnScreen_545700())
            {
                this->field_20e = 0;
            }
            if (Ped::get_fieldC_45C9B0() == k_dword_678660 && Ped::get_field8_45C900() == gDummyPedAng_6787A8)
            {
                if (field_278_ped_state_1 == ped_state_1::walking_0 && this->field_168_game_object->field_38_velocity == k_dword_678660)
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
                    Ped::sub_463830(0, 9999);
                }
            }
            if (this->field_168_game_object->field_10_char_state == Char_B4_state::Jumping_15)
            {
                this->field_15C_player->field_64 = 1;
            }
            return 1;
        case 3:
            pB4 = this->field_168_game_object;
            this->field_212_electrocution_threshold = 100;
            if (pB4)
            {
                f278_ = this->field_278_ped_state_1;
                ++this->field_20e;
                if (f278_ == 1)
                {
                    if (this->field_20e != 200)
                    {
                        goto LABEL_42;
                    }
                }
                else if (this->field_20e < k_word_678656)
                {
                    goto LABEL_42;
                }
                goto LABEL_77;
            }
            pCar = this->field_16C_car;
            ++this->field_20e;
            pCarSprite = pCar->field_50_car_sprite;
            if (pCarSprite)
            {
                if (gGame_0x40_67E008->IsSpriteOnScreenForAnyPlayer_4B97E0(pCarSprite, k_dword_678660))
                {
                    this->field_20e = 0;
                }
            }
            if (this->field_240_occupation == ped_ocupation_enum::unknown_2)
            {
                return 0;
            }
            if (this->field_16C_car->field_88 == 5)
            {
                goto LABEL_77;
            }
            if (this->field_278_ped_state_1 == ped_state_1::dead_9)
            {
                goto LABEL_79;
            }
            return 1;
        case 4:
        case 6:
            occupation_ = this->field_240_occupation;
            this->field_212_electrocution_threshold = 100;
            if (occupation_ == ped_ocupation_enum::unknown_10 && this->field_278_ped_state_1 == ped_state_1::dead_9)
            {
                --byte_6787CE;
                this->field_240_occupation = ped_ocupation_enum::dummy;
            }
            if (this->field_168_game_object)
            {
                f20E = ++this->field_20e;
                if (f20E <= 100u)
                {
                    goto LABEL_42;
                }
                occupation__ = this->field_240_occupation;
                if (occupation__ != ped_ocupation_enum::elvis_leader)
                {
                    if (occupation__ != ped_ocupation_enum::special_groups_member && !Ped::sub_45B590())
                    {
                        goto LABEL_77;
                    }
                LABEL_42:
                    Ped::sub_462620();
                    return 1;
                }
                pGroup = this->field_164_ped_group;
                if (pGroup)
                {
                    if (f20E > 500u && pGroup->sub_4C9150())
                    {
                    LABEL_77:
                        Ped::Deallocate_45EB60();
                        return 0;
                    }
                    goto LABEL_42;
                }
            LABEL_79:
                Ped::Deallocate_45EB60();
                return 0;
            }
            else
            {
                pCar_ = this->field_16C_car;
                ++this->field_20e;
                if (pCar_)
                {
                    pCarSprite_ = pCar_->field_50_car_sprite;
                    if (pCarSprite_)
                    {
                        if (gGame_0x40_67E008->IsSpriteOnScreenForAnyPlayer_4B97E0(pCarSprite_, k_dword_678660))
                        {
                            this->field_20e = 0;
                        }
                    }
                }
                if (this->field_20e > 60u)
                {
                    pCar__ = this->field_16C_car;
                    if (pCar__)
                    {
                        if (this->field_240_occupation == ped_ocupation_enum::car_thief)
                        {
                            pCar__->field_7C_uni_num = 3;
                        }
                    }
                }
                if (this->field_278_ped_state_1 == ped_state_1::dead_9)
                {
                    goto LABEL_77;
                }
                return 1;
            }
        case 5:
            pB4_ = this->field_168_game_object;
            this->field_212_electrocution_threshold = 100;
            this->field_230 = 2;
            if (pB4_)
            {
                ++this->field_20e;
                goto LABEL_42;
            }
        LABEL_37:
            field_16C_car = this->field_16C_car;
            ++this->field_20e;
            field_50_car_sprite = field_16C_car->field_50_car_sprite;
            if (field_50_car_sprite && gGame_0x40_67E008->IsSpriteOnScreenForAnyPlayer_4B97E0(field_50_car_sprite, k_dword_678660))
            {
                this->field_20e = 0;
                return 1;
            }
            return 1;
        default:
            return 1;
    }
}

MATCH_FUNC(0x462b80)
void Ped::sub_462B80()
{
    field_168_game_object->field_8_ped_state_1 = field_278_ped_state_1;
    field_168_game_object->field_C_ped_state_2 = field_27C_ped_state_2;
    field_168_game_object->sub_545720(gDistanceToTarget_678750);
    if (field_168_game_object)
    {
        field_1AC_cam.x = field_168_game_object->get_sprite_xpos();
        field_1AC_cam.y = field_168_game_object->get_sprite_ypos();
        field_1AC_cam.z = field_168_game_object->get_sprite_zpos();
        Char_B4* pB4 = field_168_game_object;
        field_12C = pB4->get_rotation_433A40();

        if (field_278_ped_state_1 == ped_state_1::in_car_10)
        {
            field_16C_car = pB4->field_84;
            if (pB4->field_88_obj_2c.field_0_p18)
            {
                pB4->field_88_obj_2c.sub_5A7010();
            }
            gChar_B4_Pool_6FDB44->DeAllocate(field_168_game_object);

            field_168_game_object = NULL;
            if (!field_248_enter_car_as_passenger)
            {
                field_16C_car->sub_4406E0(this);
            }
            else
            {
                field_16C_car->ShowCarName_4406B0(this);
                if (field_25C_car_state == 37 && field_238 == 3 ||
                    (field_16C_car->field_4_passengers_list.AddPed_471140(this), field_238 == 3))
                {
                    if (field_25C_car_state == 37)
                    {
                        Train_58* pTrain = gPublicTransport_181C_6FF1D4->GetTrainFromCarExcludingLeadCar_57B6A0(field_16C_car);
                        ++pTrain->field_56_passenger_count;
                    }
                }
            }
            if ((field_25C_car_state == 35 || field_25C_car_state == 37) && (field_226 = 1, field_25C_car_state == 37))
            {
                if (field_238 == 3)
                {
                    Ped::Deallocate_45EB60();
                }
            }
            else
            {
                Car_Door_10* Door = field_16C_car->GetDoor(field_24C_target_car_door);
                if (field_240_occupation != ped_ocupation_enum::unknown_5 && field_240_occupation != ped_ocupation_enum::unknown_6)
                {
                    Door->sub_439EA0();
                }
                Door->set_ped_421380(NULL);
            }
        }
    }
}

MATCH_FUNC(0x462e70)
bool Ped::PoolUpdate()
{
    if (field_240_occupation == ped_ocupation_enum::elvis)
    {
        if (word_6787F2 > 0)
        {
            --word_6787F2;
        }
        else
        {
            if (field_21C_bf.b24 == 0)
            {
                field_250 = 23;
            }
            word_6787F2 = stru_6F6784.get_int_4F7AE0(300) + 450;
        }
    }

    if (field_158_unk_car)
    {
        if (field_158_unk_car->field_88 == 5)
        {
            field_158_unk_car = 0;
        }
    }

    switch (field_238)
    {
        case 3:
            ++byte_6787E2;
            break;
        case 4:
        case 6:
            field_230 = 2;
            if (field_28C_threat_reaction == threat_reaction_enum::react_as_emergency_1)
            {
                field_230 = 1;
                ++byte_6787E4;
            }
            else
            {
                ++byte_6787E2;
            }
            break;
        case 5:
            ++byte_6787E3;
            break;
        default:
            break;
    }
    if (field_21C_bf.b10)
    {
        Ped::sub_45EA00();
    }
    Ped::sub_469030();
    Ped::ManageBurning_45BEC0();
    Ped::ManageShocking_45BC70();
    if (!field_234_timer)
    {
        Ped::sub_4624A0();
        return true;
    }
    if (field_26A > 0)
    {
        field_26A--;
    }
    ++word_6787E0;
    byte_6787D7 = 0;
    dword_6787DC = 0;
    byte_6787D4 = 0;
    byte_678554 = 0;
    field_263 = field_262;
    field_262 = 0;
    field_21C_bf.b23 = 0;
    if (field_21C_bf.b5 != 0 && field_278_ped_state_1 != ped_state_1::immobilized_8)
    {
        // Ped busted
        Ped::ChangeNextPedState1_45C500(ped_state_1::immobilized_8);
        Ped::ChangeNextPedState2_45C540(ped_state_2::lying_on_floor_22); // BUSTED!
        field_168_game_object->field_16 = 1;
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
        field_168_game_object->field_16 = 1;
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
                Ped::sub_462280();
                dword_6787C0 = this;
                Ped::ManageWeapon_46F390();
                if (field_20A_wanted_points)
                {
                    if (bSkip_police_67D4F9 || field_240_occupation == ped_ocupation_enum::empty)
                    {
                        field_20A_wanted_points = 0;
                    }
                    else if (field_238 == 2 && field_200_id)
                    {
                        if (field_27C_ped_state_2 == ped_state_2::Unknown_29)
                        {
                            field_20A_wanted_points = 0;
                        }
                        else
                        {
                            gPolice_7B8_6FEE40->sub_56F940(this);
                        }
                    }
                }
            }
            else
            {
                if (field_278_ped_state_1 == ped_state_1::dead_9)
                {
                    if ((!field_164_ped_group || field_23C != 99) && field_258_objective != objectives_enum::objective_28)
                    {
                        Ped::SetObjective(objectives_enum::no_obj_0, 9999);
                        Ped::sub_463830(0, 9999);
                        field_278_ped_state_1 = ped_state_1::dead_9;
                        field_27C_ped_state_2 = ped_state_2::Unknown_15;
                    }
                    if (field_20e > 0xC8u && field_240_occupation != ped_ocupation_enum::unknown_13 && field_238 != 5)
                    {
                        Ped::Deallocate_45EB60();
                    }
                }
                else
                {
                    Ped::Occupation_AI_461F20();
                    if (field_258_objective != objectives_enum::no_obj_0 || field_25C_car_state)
                    {
                        byte_61A8A3 = 0;
                        Ped::ProcessObjective_4632E0();
                    }
                    if (field_168_game_object->field_10_char_state == Char_B4_state::Jumping_15)
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
                if (field_168_game_object->field_58_flags_bf.b0 == 0 && zpos != k_dword_678660)
                {
                    zpos -= k_dword_678664;
                }
                field_254 = gMap_0x370_6F6268->GetBlockSpec_4E00A0(get_cam_x(), get_cam_y(), zpos);
                Ped::sub_462B80();

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
void Ped::sub_463300(u8 a1)
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

STUB_FUNC(0x4633e0)
s32 Ped::sub_4633E0(char_type a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

WIP_FUNC(0x463570)
void Ped::SetObjective(s32 objective, s16 objective_timer)
{
    WIP_IMPLEMENTED;

    Marz_96* pMarz_96; // eax

    if (this->field_278_ped_state_1 != 9 || objective == objectives_enum::objective_28)
    {
        this->field_258_objective = objective;
        this->field_218_objective_timer = objective_timer;
        this->field_1B8_target_x = Fix16(-16384, 0);
        this->field_1BC_target_y = Fix16(-16384, 0);
        this->field_1C0_target_z = Fix16(-16384, 0);
        this->field_1DC_objective_target_x = k_dword_678660;
        this->field_1E0_objective_target_y = k_dword_678660;
        this->field_148_objective_target_ped = 0;
        this->field_150_target_objective_car = 0;
        this->field_1A0_objective_target_object = NULL;
        this->field_225_objective_status = 0;

        //new_flags = this->field_21C & ~0x400004u;
        //this->field_21C = new_flags;

        // TODO: Not sure if this is correct
        field_21C_bf.b2 = false;
        field_21C_bf.b22 = false;

        // TODO: Switch case ordering is wrong
        switch (objective)
        {
            case 0:
                sub_463300(field_16C_car != 0 ? 5 : 1);
                break;

            case 1:
            case 2:
            case 3:
                sub_463300(2u);
                break;

            case 8:
            case 51:
                sub_463300(1u);
                break;

            case 12:
            case 16:
            case 32:
            case 56:
                sub_463830(0, 9999);
                sub_463300(3u);
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
                sub_463300(5u);
                break;

            case 20:
            case 23:
            case 58:
            case 59:
                sub_463830(0, 9999);
                sub_463300(3u);
                break;

            case 22:
                sub_463300(3u);
                break;

            case 24:
            case 25:
            case 26:
                this->field_1E4_objective_target_z = this->field_1AC_cam.z;
                this->field_1DC_objective_target_x = this->field_1AC_cam.x;
                this->field_1E0_objective_target_y = this->field_1AC_cam.y;
                sub_463300(4u);
                break;

            case 28:
                if (gAmbulance_110_6F70A8->TryAddPatient_4FA470(this))
                {
                    sub_463300(99u);
                }
                else
                {
                    this->field_258_objective = objectives_enum::no_obj_0;
                }
                break;

            case 35:
            case 37:
                sub_463300(6u);
                break;

            case 36:
            case 38:
                sub_463300(field_168_game_object != 0 ? 1 : 7);
                break;

            case 42:
                pMarz_96 = gMarz_1D7E_6FD784->sub_543F10(&field_265);
                field_190 = pMarz_96;
                while (pMarz_96->field_0[0].field_0)
                {
                    pMarz_96->field_0[0].field_0 = 0;
                    pMarz_96++;
                }
                sub_463300(99u);
                break;

            case 50:
                ChangeNextPedState1_45C500(9);
                ChangeNextPedState2_45C540(15);
                sub_463300(99u);
                break;

            default:
                sub_463300(99u);
                break;
        }
    }
}

WIP_FUNC(0x463830)
void Ped::sub_463830(s32 car_state, s16 a3)
{
    WIP_IMPLEMENTED;

    s32 ped_state_2; // eax
    Car_BC* target_to_enter; // ecx
    Car_Door_10* pDoor; // eax
    Char_B4* game_object; // eax
    Car_Door_10* pDoor_; // edi
    u8 tmp; // [esp+4h] [ebp-10h]
    u8 x_int; // [esp+8h] [ebp-Ch] BYREF
    u8 y_int; // [esp+Ch] [ebp-8h] BYREF
    u8 z_int; // [esp+10h] [ebp-4h] BYREF

    tmp = 99;
    if (this->field_278_ped_state_1 != ped_state_1::dead_9)
    {
        ped_state_2 = this->field_27C_ped_state_2;
        if (ped_state_2 == ped_state_2::ped2_entering_a_car_6 || ped_state_2 == ped_state_2::ped2_getting_out_a_car_7)
        {
            target_to_enter = this->field_154_target_to_enter;
            if (target_to_enter)
            {
                pDoor = target_to_enter->GetDoor(this->field_24C_target_car_door);
            }
            else
            {
                game_object = this->field_168_game_object;
                if (game_object)
                {
                    pDoor = game_object->field_84->GetDoor(this->field_24C_target_car_door);
                }
                else
                {
                    pDoor = this->field_16C_car->GetDoor(this->field_24C_target_car_door);
                }
            }
            pDoor_ = pDoor;
            pDoor->sub_439EA0();
            pDoor_->field_8_pObj = 0;
        }

        this->field_21A_car_state_timer = a3;
        this->field_25C_car_state = car_state;
        this->field_1C4_x = Fix16(-16384, 0);
        this->field_1C8_y = Fix16(-16384, 0);
        this->field_1CC_z = Fix16(-16384, 0);
        this->field_1D0 = k_dword_678660;
        this->field_1D4 = k_dword_678660;
        this->field_1D8 = k_dword_678660;
        this->field_14C = 0;
        this->field_154_target_to_enter = 0;
        this->field_1A4 = 0;
        this->field_226 = 0;
        switch (car_state)
        {
            case 0:
            case 22:
            case 49:
                tmp = 1;
                Ped::sub_463300(tmp);
                break;
            case 1:
            case 2:
            case 3:
                Ped::sub_463300(2u);
                return;
            case 7:
            case 9:
            case 11:
            case 12:
            case 18:
            case 20:
            case 23:
            case 48:
                Ped::sub_463300(3u);
                return;
            case 14:
                Ped::sub_463300(5u);
                return;
            case 17:
                x_int = this->field_1AC_cam.x.ToUInt8();
                y_int = this->field_1AC_cam.y.ToUInt8();
                z_int = this->field_1AC_cam.z.ToUInt8();
                gMap_0x370_6F6268->sub_4E4930(&x_int, &y_int, &z_int, 2);
                this->field_1D0 = k_dword_67853C + Fix16(x_int);
                this->field_1D4 = k_dword_67853C + Fix16(y_int);
                this->field_1D8 = k_dword_678664 + Fix16(z_int);
                Ped::sub_463300(3u);
                return;
            case 26:
            case 29:
            case 30:
            case 44:
            case 45:
            case 46:
            case 47:
                Ped::sub_463300(4u);
                return;
            case 35:
                Ped::sub_463300(6u);
                return;
            case 36:
                Ped::sub_463300(7u);
                return;
            case 37:
                Ped::ChangeNextPedState1_45C500(5);
                Ped::ChangeNextPedState2_45C540(5);
                goto LABEL_21;
            case 38:
                Ped::ChangeNextPedState1_45C500(6);
                Ped::ChangeNextPedState2_45C540(10);
                goto LABEL_21;
            default:
            LABEL_21:
                Ped::sub_463300(tmp);
                break;
        }
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
            if (field_150_target_objective_car->field_88 == 5)
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
            else if (field_1DC_objective_target_x != k_dword_678660 && field_1E0_objective_target_y != k_dword_678660)
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
                Ped::sub_467960();
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
                Ped::sub_467AD0();
                break;

            case objectives_enum::objective_34:
                Ped::sub_467BD0();
                break;
            case objectives_enum::kill_char_on_foot_20:
                Ped::sub_467CA0();
                break;
            case objectives_enum::kill_char_any_means_19:
                Ped::KillCharAnyMeans_467E20();
                break;
            case objectives_enum::kill_car_21:
                nullsub_11();
                break;
            case objectives_enum::kill_frenzy_22:
                Ped::sub_467FB0();
                break;
            case objectives_enum::punch_char_23:
                Ped::sub_467FD0();
                break;
            case objectives_enum::wait_on_foot_26:
                Ped::sub_468040();
                break;
            case objectives_enum::guard_spot_24:
                Ped::sub_469BF0();
                break;
            case objectives_enum::guard_area_25:
                Ped::sub_469D60();
                break;
            case objectives_enum::time_waited_in_car_31:
                Ped::sub_4682A0();
                break;
            case objectives_enum::goto_area_in_car_14:
                Ped::sub_468310();
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
                Ped::sub_468E80();
                break;
            case objectives_enum::goto_area_on_foot_12:
                Ped::GotoAreaOnFoot_468DE0();
                break;
            case objectives_enum::goto_area_any_means_13:
                Ped::sub_469060();
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
                Ped::sub_469F30();
                break;
            case objectives_enum::objective_54:
                Ped::sub_469FE0();
                break;
            case objectives_enum::objective_32:
                Ped::sub_46A1F0();
                break;
            case objectives_enum::follow_car_in_car_55:
                Ped::FollowCarInCurrCar_46A290();
                break;
            case objectives_enum::follow_car_on_foot_with_offset_56:
                Ped::sub_46A350();
                break;
            case objectives_enum::fire_at_object_from_vehicle_57:
                Ped::sub_46A530();
                break;
            case objectives_enum::destroy_car_59:
                Ped::DestroyTargetCar_46A850();
                break;
            case objectives_enum::destroy_object_58:
                Ped::DestroyTargetObject_46A7C0();
                break;
            case objectives_enum::turret_put_out_car_fire_60:
                Ped::sub_46A6D0();
                break;
            case objectives_enum::objective_61:
                Ped::sub_46A5E0();
                break;
            default:
                return;
        }
    }
}

// https://decomp.me/scratch/0fIeI
WIP_FUNC(0x463fb0)
void Ped::ProcessInCarObjective_463FB0()
{
    WIP_IMPLEMENTED;
    Ang16 UnkAng(0);

    if (field_226 == 2)
    {
        this->field_21C &= ~4u;
    }

    if (field_25C_car_state && !field_226)
    {
        Ped* v4 = this->field_14C;
        if (v4)
        {
            this->field_1C4_x = v4->get_cam_x();
            this->field_1C8_y = v4->get_cam_y();
            this->field_1CC_z = v4->get_cam_z();
            switch (field_25C_car_state)
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
                        // Problem here
                        if (v4->field_168_game_object && this->field_278_ped_state_1 != ped_state_1::entering_car_3)
                        {
                            Ped::sub_4645B0();
                        }
                        else
                        {
                            byte_6787C4 = 0;
                        }
                    }
                    break;
            }
        }
        if (field_154_target_to_enter)
        {
            if (field_154_target_to_enter->field_88 == 6)
            {
                this->field_226 = 2;
                return;
            }
            u8 Remap = field_154_target_to_enter->GetRemap();
            if (this->field_24C_target_car_door >= Remap)
            {
                this->field_24C_target_car_door = Remap - 1;
            }

            if (field_25C_car_state == 18 || field_25C_car_state > 34 && field_25C_car_state <= 38)
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

        if (field_1A4)
        {
            this->field_1C4_x = field_1A4->get_x_4340D0();
            this->field_1C8_y = field_1A4->get_y_4340E0();
            this->field_1CC_z = field_1A4->get_z_4340F0();
        }

        if (field_1D0 != k_dword_678660)
        {
            if (field_1D4 != k_dword_678660)
            {
                this->field_1C4_x = field_1D0;
                this->field_1C8_y = field_1D4;
                this->field_1CC_z = field_1D8;
            }
        }

        Fix16 xd = this->field_1C4_x - this->field_1AC_cam.x;
        Fix16 yd = this->field_1C8_y - this->field_1AC_cam.y;

        xd = Fix16::Abs(xd);
        yd = Fix16::Abs(yd);

        gDistanceToTarget_678750 = Fix16::Max(xd, yd);

        switch (this->field_25C_car_state)
        {
            case 1:
                Ped::sub_46A8F0();
                break;
            case 2:
                Ped::sub_46A9C0();
                break;
            case 3:
                Ped::sub_46AAE0();
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
                Ped::sub_46BD30();
                break;
            case 35:
                Ped::EnterCarStateMachine_46BDC0();
                break;
            case 36:
                Ped::ExitCarStateMachine_46C250();
                break;
            case 12:
                Ped::sub_46C7E0();
                break;
            case 15:
                Ped::sub_46C910();
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
                Ped::sub_46C9B0();
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
                Ped::sub_46D030();
                break;
            case 30:
                nullsub_14();
                break;
            case 37:
                Ped::sub_46D0D0();
                break;
            case 38:
                Ped::sub_46D240();
                break;

            case 52:
                Ped::sub_46CA70();
                break;
            case 56:
                Ped::sub_46D300();
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

STUB_FUNC(0x4645b0)
s16* Ped::sub_4645B0()
{
    NOT_IMPLEMENTED;
    return 0;
}

// https://decomp.me/scratch/S4YP7
STUB_FUNC(0x465270)
void Ped::Threat_Reaction_AI_465270()
{
    NOT_IMPLEMENTED;
    //Weapon_30* pWeapon; // edx
    //bool v14; // cc
    Ang16 v19; // ax
    Fix16 diff_x; // ecx
    Fix16 diff_y; // edx
    Fix16 diff_x_2; // eax
    Fix16 diff_y_2; // ecx
    Ped* v44; // ecx
    char_type v45; // al

    Ped* BestTargetPed_Mode1_466B90 = 0;
    bool bUnk2 = false;
    bool bUnk = false;
    s32 old_threat_search;
    switch (field_28C_threat_reaction)
    {
        case threat_reaction_enum::no_reaction_0:
            old_threat_search = this->field_288_threat_search;
            if ((field_288_threat_search == threat_search_enum::line_of_sight_1 ||
                 field_288_threat_search == threat_search_enum::line_of_sight_player_only_6 ||
                 field_288_threat_search == threat_search_enum::line_of_sight_player_threat_only_4) &&
                field_21C_bf.b19 == false && byte_61A8A2 == 1)
            {
                this->field_288_threat_search = threat_search_enum::line_of_sight_player_only_6;
                Ped* BestTargetPed_Mode5_466BD0 = Ped::FindBestTargetPed_Mode5_466BD0(3);
                if (BestTargetPed_Mode5_466BD0)
                {
                    if (BestTargetPed_Mode5_466BD0->IsField238_45EDE0(2))
                    {
                        field_21C_bf.b23 = true;
                    }
                }
                this->field_288_threat_search = old_threat_search;
            }
            return;
        case threat_reaction_enum::react_as_emergency_1:
        case threat_reaction_enum::react_as_normal_2:
            if (this->field_240_occupation == ped_ocupation_enum::unknown_13)
            {
                return;
            }
            if (this->field_16C_car)
            {
                if (field_28C_threat_reaction == threat_reaction_enum::react_as_normal_2)
                {
                    if (this->field_258_objective)
                    {
                        if (this->field_21C_bf.b4 == false && this->field_218_objective_timer > 0x32u)
                        {
                            if (Ped::FindBestTargetPed_Mode1_466B90(3))
                            {
                                if (field_16C_car->GetVelocity_43A4C0() == k_dword_678660)
                                {
                                    this->field_21C_bf.b2 = true;
                                    Ped::sub_463830(36, 9999);
                                    field_16C_car = this->field_16C_car;
                                    this->field_218_objective_timer = 0;
                                    this->field_154_target_to_enter = field_16C_car;
                                }
                            }
                        }
                    }
                }
                else
                {
                    this->field_288_threat_search = threat_search_enum::line_of_sight_1;
                    Ped::FindBestTargetPed_Mode1_466B90(3);
                }
                //return;
            }
            else
            {
                // line 138 9.6f IDA

                if (field_25C_car_state == 20 || field_25C_car_state == 23 || field_25C_car_state == 32)
                {
                    if (field_288_threat_search == threat_search_enum::line_of_sight_1 ||
                        field_288_threat_search == threat_search_enum::line_of_sight_player_only_6 ||
                        field_288_threat_search == threat_search_enum::line_of_sight_player_threat_only_4)
                    {
                        if (field_21C_bf.b19 == false && byte_61A8A2 == 1)
                        {
                            BestTargetPed_Mode1_466B90 = Ped::FindBestTargetPed_Mode5_466BD0(3);
                            bUnk = true;
                        }
                    }
                    else
                    {
                        BestTargetPed_Mode1_466B90 = Ped::FindBestTargetPed_Mode1_466B90(3);
                    }
                    if (this->field_28C_threat_reaction == threat_reaction_enum::react_as_normal_2)
                    {
                        if (BestTargetPed_Mode1_466B90)
                        {
                            if (BestTargetPed_Mode1_466B90 != this->field_14C)
                            {
                                this->field_14C = BestTargetPed_Mode1_466B90;
                            }
                        }
                    }
                    else
                    {
                        if (BestTargetPed_Mode1_466B90)
                        {
                            if (BestTargetPed_Mode1_466B90 != this->field_14C)
                            {
                                diff_x = BestTargetPed_Mode1_466B90->get_cam_x() - this->field_1AC_cam.x;
                                diff_y = BestTargetPed_Mode1_466B90->get_cam_y() - this->field_1AC_cam.y;

                                Fix16 abs_x = Fix16::Abs(diff_x);
                                Fix16 abs_y = Fix16::Abs(diff_y);

                                Fix16 unk = abs_x;
                                if (!(abs_x > abs_y))
                                {
                                    unk = abs_y;
                                }

                                if (unk < dword_678668)
                                {
                                    this->field_14C = BestTargetPed_Mode1_466B90;
                                }
                            }
                        }
                        if (this->field_25C_car_state == 32 &&
                            (BestTargetPed_Mode1_466B90 == this->field_14C || !BestTargetPed_Mode1_466B90))
                        {
                            return;
                        }
                    }

                    // line 175 9.6f IDA

                    diff_x_2 = this->field_1C4_x - this->field_1AC_cam.x;
                    diff_y_2 = this->field_1C8_y - this->field_1AC_cam.y;

                    Fix16 abs_x_2 = Fix16::Abs(diff_x_2);
                    Fix16 abs_y_2 = Fix16::Abs(diff_y_2);

                    Fix16 unk_2 = abs_x_2;
                    if (!(abs_x_2 > abs_y_2))
                    {
                        unk_2 = abs_y_2;
                    }

                    if (field_164_ped_group)
                    {
                        if (field_23C != 99)
                        {
                            if (field_164_ped_group->sub_4CAC20(field_23C)) // OBS: arg is u8 type
                            {
                                Ped::sub_463830(7, 9999);
                                set_field_14C_403AE0(field_164_ped_group->field_2C_ped_leader);
                                SetBit2_403950();
                            }
                            if (field_164_ped_group->sub_4C9220() && field_164_ped_group->sub_4CAD40())
                            {
                                Ped::sub_463830(7, 9999);
                                set_field_14C_403AE0(field_164_ped_group->field_2C_ped_leader);
                                SetBit2_403950();
                            }
                        }
                    }

                    if (unk_2 <= dword_678520) // v40
                    {
                        //v42 = this->field_288_threat_search;
                        if (field_288_threat_search != threat_search_enum::line_of_sight_1 &&
                            field_288_threat_search != threat_search_enum::line_of_sight_player_only_6 &&
                            field_288_threat_search != threat_search_enum::line_of_sight_player_threat_only_4)
                        {
                            this->field_20C = 0;
                            goto LABEL_137;
                        }
                        if (this->field_21C_bf.b19 == false == 0 && byte_61A8A2 == 1)
                        {
                            if (gMap_0x370_6F6268->sub_4E5640(gSpawnJitterScale_678618,
                                                              dword_678484,
                                                              gSpawnJitterScale_678618,
                                                              this->field_1AC_cam.x,
                                                              this->field_1AC_cam.y,
                                                              this->field_1AC_cam.z,
                                                              this->field_14C->field_1AC_cam.x,
                                                              this->field_14C->field_1AC_cam.y,
                                                              this->field_14C->field_1AC_cam.z))
                            {
                                this->field_20C = 0;
                            }
                            else
                            {
                                ++this->field_20C;
                            }
                            bUnk = 1;
                            //goto LABEL_137;
                        }
                        else
                        {
                            ++this->field_20C;
                        }
                    }
                    else
                    {
                        ++this->field_20C;
                    }

                LABEL_137:
                    //v43 = this->field_288_threat_search;

                    if ((field_288_threat_search == threat_search_enum::line_of_sight_1 ||
                         field_288_threat_search == threat_search_enum::line_of_sight_player_only_6 ||
                         field_288_threat_search == threat_search_enum::line_of_sight_player_threat_only_4) &&
                        bUnk == 1)
                    {
                        this->field_21C_bf.b19 = true; // |= 0x80000u;
                        byte_61A8A2 = 0;
                    }
                    if (this->field_20C > 15)
                    {
                        bUnk2 = 1;
                    }

                    v44 = this->field_14C;
                    if ((v44->field_21C & 0x2000000) != 0)
                    {
                        if (v44->field_168_game_object)
                        {
                            bUnk2 = 1;
                        }
                    }
                    v45 = this->field_226;
                    if (v45 == 1)
                    {
                        bUnk2 = 1;
                    }
                    if (v45 == 2)
                    {
                        bUnk2 = 1;
                    }
                    if (this->field_28C_threat_reaction == threat_reaction_enum::react_as_emergency_1)
                    {
                        if (!v44->get_wanted_star_count_46EF00())
                        {
                            if (this->field_14C->field_15C_player)
                            {
                                bUnk2 = 1;
                            }
                        }
                        if (this->field_258_objective == objectives_enum::enter_car_as_driver_35)
                        {
                            goto LABEL_157;
                        }
                    }
                    if (bUnk2 == 1)
                    {
                    LABEL_157:
                        if (this->field_14C != this->field_148_objective_target_ped)
                        {
                            if (this->field_168_game_object->field_10_char_state != Char_B4_state::Jumping_15)
                            {
                                this->field_0_patrol_points[0].field_0 = 0;
                                this->field_0_patrol_points[0].field_1 = 0;
                                field_21C_bf.b2 = false;
                                field_21C_bf.b11 = false;
                            }
                            Ped::sub_463830(0, 9999);
                        }
                        else
                        {
                            this->field_226 = 2;
                        }
                    }
                    return;
                }
                else
                {
                    /*
                    //v25 = this->field_21C;
                    this->field_20C = 0;
                    //if ((v25 & 0x8000000) != 0)
                    if (field_21C_bf.b27)
                    {
                        field_21C_bf.b2 = false;
                        //LOBYTE(v25) = v25 & 0xFB;
                        //this->field_21C = v25;
                        return;
                    }
                    //v26 = this->field_164_ped_group;
                    if (!field_164_ped_group || !field_164_ped_group->sub_4C9220() && !field_164_ped_group->sub_4C9210())
                    {
                        threat_search = this->field_288_threat_search;
                        if (threat_search == line_of_sight_1 || threat_search == threat_search_enum::line_of_sight_player_only_6 || threat_search == threat_search_enum::line_of_sight_player_threat_only_4)
                        {
                            if ((this->field_21C & 0x80000) != 0 || byte_61A8A2 != 1)
                            {
                                goto LABEL_82;
                            }
                            v28 = Ped::FindBestTargetPed_Mode5_466BD0(3);
                            bUnk = 1;
                        }
                        else
                        {
                            v28 = Ped::FindBestTargetPed_Mode1_466B90(3);
                        }
                        if (v28 && (v28->field_21C & 1) != 0)
                        {
                            if (this->field_28C_threat_reaction == threat_reaction_enum::react_as_emergency_1 && (this->field_21C & 0x1000000) == 0)
                            {
                                this->field_250 = 17;
                            }
                            v29 = this->field_164_ped_group;
                            if (v28->field_16C_car)
                            {
                                if (!v29)
                                {
                                    field_21C_bf.b2 = true; //this->field_21C |= 4u;
                                    Ped::sub_463830(20, 9999);
                                    this->field_14C = v28;
                                    this->field_20C = 0;
                                    goto LABEL_83;
                                }
                            }
                            else if (!v29)
                            {
                                if (this->field_258_objective != objectives_enum::goto_area_any_means_13)
                                {
                                    field_21C_bf.b2 = true; //this->field_21C |= 4u;
                                    Ped::sub_463830(20, 9999);
                                    this->field_14C = v28;
                                    v28->field_144 = this;
                                    this->field_20C = 0;
                                }
                                goto LABEL_83;
                            }
                            v29->sub_4C9B60(v28); // PedGroup
                        LABEL_83:
                            //v31 = this->field_288_threat_search;
                            if ((field_288_threat_search == threat_search_enum::line_of_sight_1 || field_288_threat_search == threat_search_enum::line_of_sight_player_only_6 || field_288_threat_search == threat_search_enum::line_of_sight_player_threat_only_4) && bUnk == 1)
                            {
                                field_21C_bf.b19 = true; //this->field_21C |= k_ped_0x00080000;
                                byte_61A8A2 = 0;
                            }
                            return;
                        }
                    LABEL_82:
                        v30 = this->field_21C;
                        LOBYTE(v30) = v30 & ~k_ped_0x00000004;
                        this->field_21C = v30;
                        goto LABEL_83;
                    }
                    */
                }
                return;
                case threat_reaction_enum::run_away_3:
                    if (this->field_168_game_object)
                    {
                        /*
                        if (bHaveThreateningPeds_6787DA)
                        {
                            if (this->field_25C_car_state != 2 && (this->field_21C & 0x8000000) == 0)
                            {
                                v7 = Ped_List_4::GetFromListClosestPedToPoint_471340(&gThreateningPedsList_678468,
                                                                                    this->field_1AC_cam.x,
                                                                                    this->field_1AC_cam.y);
                                v8 = v7;
                                if (v7)
                                {
                                    pWeapon = v7->field_170_selected_weapon;
                                    if ((pWeapon || v7->field_16C_car) && v7 != this)
                                    {
                                        x = this->field_1AC_cam.x;
                                        v11 = v7->field_1AC_cam.x - x;
                                        if (v11 <= 0)
                                        {
                                            v11 = x - v8->field_1AC_cam.x;
                                        }
                                        v12 = v11;
                                        v13 = v8->field_1AC_cam.y - this->field_1AC_cam.y;
                                        v49 = v12;
                                        if (v13 <= 0)
                                        {
                                            v13 = -v13;
                                        }
                                        v50 = v13;
                                        v14 = v12 <= v13;
                                        v15 = &v49;
                                        if (v14)
                                        {
                                            v15 = &v50;
                                        }
                                        if (*v15 < k_dword_678680)
                                        {
                                            if (v8->field_16C_car)
                                            {
                                                if ((this->field_21C & 0x1000000) == 0)
                                                {
                                                    this->field_250 = 1;
                                                }
                                            }
                                            else
                                            {
                                                field_1C_idx = field_170_selected_weapon->field_1C_idx;
                                                if (field_1C_idx < 2 || field_1C_idx == 6 || field_1C_idx == 18 || field_1C_idx == 9 || field_1C_idx == 10)
                                                {
                                                    if ((this->field_21C & 0x1000000) == 0)
                                                    {
                                                        this->field_250 = 3;
                                                    }
                                                }
                                                else if ((this->field_21C & 0x1000000) == 0)
                                                {
                                                    this->field_250 = 2;
                                                }
                                            }
                                            Ped::sub_463830(2, 9999);
                                            v17 = this->field_1AC_cam.x;
                                            this->field_14C = v8;
                                            y = this->field_1AC_cam.y;
                                            pMaybeY_FP16.mValue = v17 - v8->field_1AC_cam.x;
                                            pMaybeX_FP16 = y - v8->field_1AC_cam.y;
                                            v19 = Fix16::atan2_fixed_405320((Fix16*)&pMaybeX_FP16, &pMaybeY_FP16);
                                            //field_168_game_object = this->field_168_game_object;
                                            this->field_130 = v19;
                                            field_168_game_object->field_40_rotation = v19;
                                            field_21C_bf.b2 = true;
                                        }
                                    }
                                }
                            }
                        }
                        */
                        if (this->field_258_objective == objectives_enum::flee_char_on_foot_till_safe_2 &&
                            this->field_225_objective_status == objective_status::passed_1)
                        {
                            Ped::SetObjective(objectives_enum::no_obj_0, 9999);
                        }
                        if (this->field_25C_car_state == 2 && this->field_226 == 1)
                        {
                            Ped::sub_463830(0, 9999);
                            field_21C_bf.b2 = false;
                        }
                    }
            }
            return;
        default:
            return;
    }
}

STUB_FUNC(0x465b20)
char_type Ped::sub_465B20()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x465cd0)
bool Ped::sub_465CD0()
{
    if (field_21C_bf.b2 == true)
    {
        if (field_14C)
        {
            if (field_25C_car_state == 20 || field_25C_car_state == 23)
            {
                return true;
            }
        }
    }
    return false;
}

STUB_FUNC(0x465d00)
char_type Ped::sub_465D00(Ped* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x4661F0)
char_type Ped::IsThreatToSearchingPed_4661F0()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x466b90)
Ped* Ped::FindBestTargetPed_Mode1_466B90(s32 max_x_check)
{
    byte_6787D7 = 1;
    return Ped::FindBestTargetPed_466BF0(max_x_check);
}

MATCH_FUNC(0x466bb0)
Ped* Ped::FindBestTargetPed_Mode4_466BB0(s32 max_x_check)
{
    byte_6787D7 = 4;
    return Ped::FindBestTargetPed_466BF0(max_x_check);
}

MATCH_FUNC(0x466bd0)
Ped* Ped::FindBestTargetPed_Mode5_466BD0(s32 max_x_check)
{
    byte_6787D7 = 5;
    return Ped::FindBestTargetPed_466BF0(max_x_check);
}

STUB_FUNC(0x466bf0)
Ped* Ped::FindBestTargetPed_466BF0(s32 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x466f40)
Ped* Ped::sub_466F40(u8 a2)
{
    byte_6787D7 = 4;
    return Ped::FindNearestPed_466F60(a2);
}

MATCH_FUNC(0x466f60)
Ped* Ped::FindNearestPed_466F60(u8 a2)
{
    dword_6787DC = this;
    Sprite* pSprite = gPurpleDoom_1_679208->FindNearestSprite_SpiralSearch_477C90(sprite_types_enum::ped,
                                                                                  sprite_types_enum::car,
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

STUB_FUNC(0x466fb0)
Ped* Ped::FindNearbyPed_466FB0()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x467070)
Ped* Ped::sub_467070()
{
    return dword_6787C0->field_168_game_object != 0 ? dword_6787C0 : 0;
}

WIP_FUNC(0x467090)
char_type Ped::FindUsableCarDoor_467090()
{
    WIP_IMPLEMENTED;

    Car_BC* pTargetToEnter; // edi
    Char_B4* pB4; // eax
    char_type isPedKind; // al
    Fix16 vel_to_check; // ebx
    s32 enter_car_as_passenger; // eax
    u8 target_car_door; // bl
    Car_Door_10* pDoor; // ebp
    s32 ped_state_2; // eax
    Car_Door_10* Door; // ebp
    s32 target_door_counter; // [esp+10h] [ebp-4h] BYREF

    pTargetToEnter = this->field_154_target_to_enter;
    if (pTargetToEnter ||
        (!this->field_150_target_objective_car || this->field_27C_ped_state_2 == ped_state_2::ped2_getting_out_a_car_7 ||
         this->field_258_objective == objectives_enum::leave_car_36) &&
            (pB4 = this->field_168_game_object) != 0 && (pTargetToEnter = pB4->field_84) != 0)
    {
        isPedKind = sub_45B4E0();
        vel_to_check = dword_67856C;
        if (isPedKind)
        {
            vel_to_check = dword_678428;
        }
        if ((pTargetToEnter->GetVelocity_43A4C0() <= vel_to_check // car going slow enough?
             || this->field_25C_car_state == 36 || this->field_27C_ped_state_2 == ped_state_2::Unknown_17) &&
            pTargetToEnter->field_88 != 5 && pTargetToEnter->field_74_damage != 32001 &&
            (this->field_278_ped_state_1 == ped_state_1::exiting_car_4 || !pTargetToEnter->sub_43B2B0(this)) // can enter this car?
            && pTargetToEnter->field_88 != 7)
        {
            enter_car_as_passenger = this->field_248_enter_car_as_passenger;
            target_car_door = this->field_24C_target_car_door;
            target_door_counter = target_car_door; // LOBYTE
            if (enter_car_as_passenger)
            {
                if (target_car_door < pTargetToEnter->GetRemap())
                {
                    while (!pTargetToEnter->IsDoorAccessible_43AFE0(target_door_counter))
                    {
                        Door = pTargetToEnter->GetDoor(this->field_24C_target_car_door);
                        Door->sub_439EA0();
                        Door->field_8_pObj = 0;
                        if (this->field_27C_ped_state_2 == ped_state_2::ped2_entering_a_car_6)
                        {
                            return 0;
                        }

                        target_door_counter = ++target_car_door; // LOBYTE
                        if (target_car_door >= pTargetToEnter->GetRemap())
                        {
                            goto LABEL_29;
                        }
                    }
                    goto LABEL_24;
                }

            LABEL_29:
                target_door_counter = this->field_24C_target_car_door; // LOBYTE
                target_car_door = target_door_counter;
                if ((u8)target_door_counter != 0xFF)
                {
                    while (!pTargetToEnter->IsDoorAccessible_43AFE0(target_door_counter))
                    {
                        target_door_counter = --target_car_door; // LOBYTE
                        if (target_car_door == 0xFF)
                        {
                            return 0;
                        }
                    }
                    goto LABEL_24;
                }
            }
            else if (target_car_door < pTargetToEnter->GetRemap())
            {
                while (!pTargetToEnter->IsDoorAccessible_43AFE0(target_door_counter))
                {
                    pDoor = pTargetToEnter->GetDoor(this->field_24C_target_car_door);
                    pDoor->sub_439EA0();
                    pDoor->field_8_pObj = 0;
                    ped_state_2 = this->field_27C_ped_state_2;
                    if (ped_state_2 == ped_state_2::ped2_entering_a_car_6 || ped_state_2 == ped_state_2::ped2_getting_out_a_car_7)
                    {
                        return 0;
                    }
                    target_door_counter = ++target_car_door; // LOBYTE
                    if (target_car_door >= pTargetToEnter->GetRemap())
                    {
                        return 0;
                    }
                }
            LABEL_24:
                this->field_24C_target_car_door = target_car_door;
                return 1;
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

STUB_FUNC(0x4672e0)
char_type Ped::sub_4672E0(Fix16 a2, s32 a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x4678e0)
void Ped::FleeOnFootTillSafe_4678E0()
{
    if (byte_61A8A3)
    {
        if (gDistanceToTarget_678750 > dword_678520) // far away from the threat or place?
        {
            if (field_168_game_object)
            {
                if (field_168_game_object->field_44 == 2)
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
void Ped::sub_467960()
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
        if (gDistanceToTarget_678750 > dword_678520)
        {
            if (this->field_168_game_object->field_44 == 2)
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
void Ped::sub_467AD0()
{
    if (field_16C_car)
    {
        if (field_16C_car->GetVelocity_43A4C0() == k_dword_678660 && field_25C_car_state != 36)
        {
            Ped::sub_463830(36, 9999);
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
        if (field_16C_car->GetVelocity_43A4C0() == k_dword_678660 && field_25C_car_state != 36)
        {
            Ped::sub_463830(36, 9999);
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

WIP_FUNC(0x467ca0)
void Ped::sub_467CA0()
{
    WIP_IMPLEMENTED;

    // TODO: Move this code down somehow
    if (field_148_objective_target_ped->check_bit_0())
    {
        if (field_148_objective_target_ped->field_278_ped_state_1 != ped_state_1::dead_9)
        {
            this->field_225_objective_status = objective_status::failed_2;
            return;
        }
        // LABEL_25:
        // this->field_225 = 1;
        // return;
    }

    if (field_148_objective_target_ped->field_278_ped_state_1 == ped_state_1::dead_9)
    {
        this->field_225_objective_status = objective_status::passed_1;
        return;
    }

    if (field_140)
    {
        if (field_140->field_88 == 5)
        {
            this->field_140 = 0;
        }
        else
        {
            field_140->field_76_last_seen_timer = 0;
        }
    }

    // TODO: An inline?
    if ((field_148_objective_target_ped->field_21C & 0x2000000) != 0 && field_148_objective_target_ped->field_168_game_object)
    {
        if (this->field_25C_car_state == 17)
        {
            return;
        }
        Ped::sub_463830(0, 9999);
        return;
    }

    if (!byte_61A8A3 || (this->field_21C_bf.b2))
    {
        return;
    }

    switch (field_25C_car_state)
    {
        case 0:
            Ped::sub_463830(20, 9999);
            this->field_14C = this->field_148_objective_target_ped;
            return;

        case 1:
            if (this->field_226 != 1)
            {
                return;
            }
            Ped::sub_463830(20, 9999);
            this->field_14C = this->field_148_objective_target_ped;
            return;

        case 20:
            if (this->field_226 == 1)
            {
                if (this->field_14C != field_148_objective_target_ped)
                {
                    // LABEL_10
                    Ped::sub_463830(0, 9999);
                    return;
                }
                this->field_225_objective_status = objective_status::passed_1;
                return;
            }

            if (this->field_226 == 2)
            {
                Ped::sub_463830(20, 9999);
                this->field_14C = this->field_148_objective_target_ped;
                field_21C_bf.b2 = 0;
                return;
            }
            return;

        default:
            return;
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
            Ped::sub_463830(0, 9999);
            if (field_16C_car)
            {
                // If the assassin is on a car, cancel routes
                field_278_ped_state_1 = ped_state_1::in_car_10;
                field_27C_ped_state_2 = ped_state_2::ped2_driving_10;
                if (field_16C_car->field_5C)
                {
                    char_type junc_idx = field_16C_car->field_5C->field_28_junc_idx;
                    if (junc_idx > 0)
                    {
                        gRouteFinder_6FFDC8->CancelRoute_589930(junc_idx);
                    }
                }
                if (field_16C_car->field_60)
                {
                    gHamburger_500_678E30->Cancel_474CC0(field_16C_car->field_60);
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
            Ped::sub_463830(0, 9999);
            if (field_16C_car)
            {
                field_278_ped_state_1 = ped_state_1::in_car_10;
                field_27C_ped_state_2 = ped_state_2::ped2_driving_10;
                if (field_16C_car->field_5C)
                {
                    char_type junc_idx = field_16C_car->field_5C->field_28_junc_idx;
                    if (junc_idx > 0)
                    {
                        gRouteFinder_6FFDC8->CancelRoute_589930(junc_idx);
                    }
                }
                if (field_16C_car->field_60)
                {
                    gHamburger_500_678E30->Cancel_474CC0(field_16C_car->field_60);
                    field_16C_car->field_60 = 0;
                }
            }
        }
        else if (field_168_game_object)
        {
            if (gDistanceToTarget_678750 > k_dword_678680)
            {
                field_1DC_objective_target_x = field_148_objective_target_ped->get_cam_x();
                field_1E0_objective_target_y = field_148_objective_target_ped->get_cam_y();
                field_1E4_objective_target_z = field_148_objective_target_ped->get_cam_z();
                Ped::sub_469060();
            }
            else if (field_25C_car_state != 20)
            {
                Ped::sub_463830(20, 9999);
                field_14C = field_148_objective_target_ped;
            }
        }
        else
        {
            Ped::sub_469060();
        }
    }
}

MATCH_FUNC(0x467fb0)
void Ped::sub_467FB0()
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
void Ped::sub_467FD0()
{
    if ((field_148_objective_target_ped->field_21C & 1) == 0 ||
        field_148_objective_target_ped->field_278_ped_state_1 == ped_state_1::dead_9)
    {
        this->field_225_objective_status = objective_status::failed_2;
        return;
    }

    if (byte_61A8A3)
    {
        if (field_25C_car_state != 0)
        {
            if (field_25C_car_state == 23)
            {
                if (field_226 == 1)
                {
                    this->field_225_objective_status = objective_status::passed_1;
                    return;
                }
                else if (field_226 == 2)
                {
                    this->field_225_objective_status = objective_status::failed_2;
                }
            }
        }
        else
        {
            Ped::sub_463830(23, 9999);
            this->field_14C = this->field_148_objective_target_ped;
            return;
        }
    }
}

STUB_FUNC(0x468040)
s16 Ped::sub_468040()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x4682a0)
void Ped::sub_4682A0()
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

WIP_FUNC(0x468310)
void Ped::sub_468310()
{
    NOT_IMPLEMENTED;

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
                this->field_16C_car->field_60 = gHamburger_500_678E30->sub_474810();
                this->field_16C_car->field_60->field_4_ped_owner = this;
            }

            this->field_16C_car->field_60->field_8_maybe_path_type = 1;
            this->field_16C_car->field_60->field_22 = 1;
            this->field_16C_car->field_60->field_20 = 0;
            this->field_16C_car->field_60->field_14_target_x = this->field_1DC_objective_target_x;
            this->field_16C_car->field_60->field_18_target_y = this->field_1E0_objective_target_y;
            this->field_16C_car->field_60->field_1C_target_z = this->field_1E4_objective_target_z;
            this->field_16C_car->field_A6 &= ~0x20u;

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
                gHamburger_500_678E30->Cancel_474CC0(pCar_->field_60);
                this->field_16C_car->field_60 = 0;
                this->field_16C_car->field_A6 |= 0x20u;
                this->field_1A0_objective_target_object = dword_678558; // TODO: Never written so part of a bigger global obj?
            }
            else
            {
                pCar__ = this->field_16C_car;
                if (pCar__->field_60->field_26)
                {
                    this->field_225_objective_status = objective_status::passed_1;
                    gHamburger_500_678E30->Cancel_474CC0(pCar__->field_60);
                    this->field_16C_car->field_60 = 0;
                    this->field_16C_car->field_A6 |= 0x20u;
                }
                else if (pCar__)
                {
                    if (pCar__->GetVelocity_43A4C0() == k_dword_678660)
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
                else
                {
                    this->field_218_objective_timer = 9999;
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
        field_168_game_object->field_84 = field_150_target_objective_car;
    }

    if (field_25C_car_state)
    {
        if (field_25C_car_state == 35)
        {
            if (field_226 == 1)
            {
                if (!field_16C_car)
                {
                    Car_BC* pOldCar = field_150_target_objective_car;
                    Ped::SetObjective(objectives_enum::enter_car_as_driver_35, 9999);
                    Ped::sub_463830(0, 9999);
                    field_150_target_objective_car = pOldCar;
                }
                else
                {
                    field_225_objective_status = objective_status::passed_1;
                    Ped::sub_463830(0, 9999);
                    Ped::ChangeNextPedState1_45C500(ped_state_1::in_car_10);
                    Ped::ChangeNextPedState2_45C540(ped_state_2::ped2_driving_10);
                }
                return;
            }
            if (field_226 == 2)
            {
                field_225_objective_status = objective_status::failed_2;
            }
        }
    }
    else
    {
        Ped::sub_463830(35, 9999);
        field_154_target_to_enter = field_150_target_objective_car;
    }
    if (field_150_target_objective_car->field_88 == 5 || field_150_target_objective_car->field_74_damage == 32001)
    {
        field_225_objective_status = objective_status::failed_2;
        Ped::sub_463830(0, 9999);
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

    if (field_25C_car_state)
    {
        if (field_25C_car_state == 36)
        {
            if (field_226 == 1)
            {
                if (field_238 != 2)
                {
                    field_225_objective_status = objective_status::passed_1;
                }
                else
                {
                    Ped::SetObjective(objectives_enum::no_obj_0, 9999);
                }

                Ped::sub_463830(0, 9999);
            }
            else if (field_226 == 2)
            {
                field_225_objective_status = objective_status::failed_2;
            }
        }
    }
    else if (field_16C_car)
    {
        Ped::sub_463830(36, 9999);
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
    if (field_226 == 2 || field_150_target_objective_car == 0)
    {
        field_225_objective_status = objective_status::failed_2;
    }
    else
    {
        if (field_168_game_object)
        {
            field_168_game_object->field_84 = field_150_target_objective_car;
        }

        if (field_25C_car_state)
        {
            if (field_25C_car_state == 37)
            {
                if (field_226 == 1)
                {
                    field_225_objective_status = objective_status::passed_1;
                    Ped::sub_463830(0, 9999);
                    Ped::ChangeNextPedState1_45C500(ped_state_1::in_car_10);
                    Ped::ChangeNextPedState2_45C540(ped_state_2::ped2_driving_10);
                    return;
                }
                if (field_226 == 2)
                {
                    field_225_objective_status = objective_status::failed_2;
                }
            }
        }
        else
        {
            Ped::sub_463830(37, 9999);
            field_154_target_to_enter = field_150_target_objective_car;
        }
        if (field_150_target_objective_car->field_88 == 5 || field_150_target_objective_car->field_74_damage == 32001)
        {
            field_225_objective_status = objective_status::failed_2;
            Ped::sub_463830(0, 9999);
        }
    }
}

MATCH_FUNC(0x468a00)
void Ped::LeaveTrain_468A00()
{
    if (field_25C_car_state)
    {
        if (field_25C_car_state != 12)
        {
            if (field_25C_car_state == 38)
            {
                if (field_226 == 1)
                {
                    if (field_150_target_objective_car->IsTrainModel_403BA0())
                    {
                        if (field_238 != 2)
                        {
                            Ped::sub_463830(12, 9999);
                            switch (Ang16::GetAngleFace_4F78F0(field_12C))
                            {
                                case 1:
                                    field_1D0 = field_1AC_cam.x;
                                    field_1D4 = field_1AC_cam.y - k_dword_678664;
                                    break;
                                case 3:
                                    field_1D0 = k_dword_678664 + field_1AC_cam.x;
                                    field_1D4 = field_1AC_cam.y;
                                    break;
                                case 2:
                                    field_1D0 = field_1AC_cam.x;
                                    field_1D4 = k_dword_678664 + field_1AC_cam.y;
                                    break;
                                case 4:
                                    field_1D0 = field_1AC_cam.x - k_dword_678664;
                                    field_1D4 = field_1AC_cam.y;
                                    break;
                                default:
                                    break;
                            }
                            field_1D8 = field_1AC_cam.z;
                        }
                        else
                        {
                            Ped::SetObjective(objectives_enum::no_obj_0, 9999);
                            Ped::sub_463830(0, 9999);
                        }
                    }
                    else
                    {
                        field_225_objective_status = objective_status::passed_1;
                    }
                }
                if (field_226 == 2)
                {
                    field_225_objective_status = objective_status::failed_2;
                }
            }
        }
        else
        {
            if (field_278_ped_state_1 != ped_state_1::immobilized_8 && field_226 == 1)
            {
                field_225_objective_status = objective_status::passed_1;
                Ped::sub_463830(0, 9999);
            }
        }
    }
    else if (field_16C_car)
    {
        Ped::sub_463830(38, 9999);
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
    if (field_25C_car_state)
    {
        if (field_25C_car_state == 36 && !field_16C_car)
        {
            field_168_game_object->field_16 = 1;
            field_278_ped_state_1 = ped_state_1::immobilized_8;
            field_27C_ped_state_2 = ped_state_2::Unknown_17;
            field_168_game_object->field_8_ped_state_1 = 8;
            field_168_game_object->field_C_ped_state_2 = 17;
            field_16C_car = 0;
            Ped::SetObjective(objectives_enum::no_obj_0, 9999);
            Ped::sub_463830(0, 9999);
        }
    }
    else
    {
        Ped::sub_463830(36, 9999);
        field_154_target_to_enter = field_150_target_objective_car;
    }
}

MATCH_FUNC(0x468c70)
void Ped::PatrolOnFoot_468C70()
{
    if (byte_61A8A3)
    {
        if (field_25C_car_state == 12)
        {
            if (field_21C_bf.b2 == false)
            {
                if (field_226)
                {
                    field_194 = field_194 + 1;
                    if (!field_194->field_0)
                    {
                        field_194 = field_190->field_0;
                    }
                    Ped::sub_463830(12, 9999);
                    field_1D0 = k_dword_67853C + Fix16(field_194->field_0);
                    field_1D4 = k_dword_67853C + Fix16(field_194->field_1);
                    field_1D8 = Fix16(field_194->field_2);
                }
                field_168_game_object->sub_433970(field_1F4);
            }
        }
        else if (field_21C_bf.b2 == false)
        {
            field_194 = field_190->field_0;
            Ped::sub_463830(12, 9999);
            field_1D0 = k_dword_67853C + Fix16(field_194->field_0);
            field_1D4 = k_dword_67853C + Fix16(field_194->field_1);
            field_1D8 = Fix16(field_194->field_2);
            field_168_game_object->sub_433970(field_1F4);
        }
    }
}

MATCH_FUNC(0x468de0)
void Ped::GotoAreaOnFoot_468DE0()
{
    if (field_240_occupation == ped_ocupation_enum::drone)
    {
        field_1F0_maybe_max_speed = dword_678434;
    }
    if (byte_61A8A3)
    {
        if (gDistanceToTarget_678750 < dword_678788)
        {
            if (field_168_game_object->field_10_char_state != Char_B4_state::Jumping_15)
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
        Ped::sub_4672E0(gDistanceToTarget_678750, 4);
    }
}

STUB_FUNC(0x468e80)
char_type Ped::sub_468E80()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x469010)
s32 Ped::sub_469010()
{
    return (this->field_200_id & 1) != 0 ? 56 : 51;
}

MATCH_FUNC(0x469030)
void Ped::sub_469030()
{
    if (field_264 > 0)
    {
        if (field_27C_ped_state_2 != ped_state_2::Unknown_26)
        {
            field_264--;
            if (field_264 == 0)
            {
                field_204_killer_id = 0;
                field_290 = 0;
            }
        }
    }
}

STUB_FUNC(0x469060)
void Ped::sub_469060()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x469bd0)
void Ped::sub_469BD0()
{
    if (field_168_game_object)
    {
        field_164_ped_group->sub_4CA820(field_23C);
    }
}

STUB_FUNC(0x469bf0)
char_type Ped::sub_469BF0()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x469d60)
void Ped::sub_469D60()
{
    if (byte_61A8A3)
    {
        if (gDistanceToTarget_678750 <= dword_6784DC && field_1AC_cam.z.ToUInt8() == field_1E4_objective_target_z.ToUInt8())
        {
            if (field_168_game_object->field_10_char_state != Char_B4_state::Jumping_15)
            {
                Ped::ChangeNextPedState1_45C500(ped_state_1::standing_still_7);
                Ped::ChangeNextPedState2_45C540(ped_state_2::ped2_staying_14);
                field_130 = field_134_rotation;
            }
        }
        else
        {
            field_168_game_object->SetMaxSpeed_433920(field_1F4);
            Ped::sub_4672E0(gDistanceToTarget_678750, 4);
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
        field_16C_car->field_5C->field_74 = k_dword_678664;
    }
}

MATCH_FUNC(0x469e50)
void Ped::sub_469E50()
{
    if (field_16C_car)
    {
        if (!field_16C_car->field_60)
        {
            field_16C_car->field_60 = gHamburger_500_678E30->sub_474810();
            field_16C_car->field_60->field_4_ped_owner = this;
        }
        field_16C_car->field_60->field_8_maybe_path_type = 4;
        field_16C_car->sub_421560(5);
        field_16C_car->field_60->field_30_ped_to_follow = field_148_objective_target_ped;
        field_16C_car->field_A6 &= ~0x20u;
        field_16C_car->field_5C->field_74 = dword_67866C;
        field_16C_car->field_60->field_20 = 1;
        if (field_16C_car->field_84_car_info_idx == car_model_enum::JEEP)
        {
            if (gDistanceToTarget_678750 < dword_678668)
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
void Ped::sub_469F30()
{
    if (!field_16C_car->field_60)
    {
        field_16C_car->field_60 = gHamburger_500_678E30->sub_474810();
        field_16C_car->field_60->field_4_ped_owner = this;
    }
    field_16C_car->field_60->field_8_maybe_path_type = 2;
    field_16C_car->sub_421560(5);
    field_16C_car->field_60->field_30_ped_to_follow = field_148_objective_target_ped;
    field_16C_car->field_A6 &= ~0x20u;
    field_16C_car->field_5C->field_74 = dword_67866C;
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

    if (this->field_150_target_objective_car)
    {
        if (!this->field_218_objective_timer)
        {
            this->field_24C_target_car_door = 2;
            sub_463830(objectives_enum::no_obj_0, 9999);
            SetObjective(objectives_enum::objective_33, 9999);
            this->field_150_target_objective_car = this->field_16C_car;
        }
    }
    else
    {
        Fix16 x = this->field_1AC_cam.x;
        Fix16 y = this->field_1AC_cam.y;
        Fix16 z = this->field_1AC_cam.z;
        u8 v18 = x.ToUInt8();
        u8 v17 = y.ToUInt8();
        u8 v16 = z.ToUInt8();
        gOrca_2FD4_6FDEF0->FindNearbyTileMatchingSlopeType_5552B0(1, &v18, &v17, &v16, 1);
        if (gCar_6C_677930->CanAllocateOfType_446930(10))
        {
            Car_BC* v6 = gCar_6C_677930->SpawnCarAtRoadDirection_444CF0(car_model_enum::COPCAR, v18, v17, v16);
            if (v6)
            {
                v6->IncrementCarStats_443D70(10);
                if (gPolice_7B8_6FEE40->FBI_Army_5703E0(v6))
                {
                    Char_B4* field_168_game_object = this->field_168_game_object;
                    this->field_278_ped_state_1 = ped_state_1::in_car_10;
                    this->field_27C_ped_state_2 = ped_state_2::ped2_driving_10;
                    field_168_game_object->field_84 = v6;
                    //LABEL_10:
                    this->field_248_enter_car_as_passenger = 1;
                    this->field_150_target_objective_car = v6;
                    return;
                }
            }
        }
        else if (gCar_6C_677930->CanAllocateOfType_446930(6))
        {
            Car_BC* v11 = gCar_6C_677930->SpawnCarAtRoadDirection_444CF0(car_model_enum::COPCAR, v18, v17, v16);
            if (v11)
            {
                v11->IncrementCarStats_443D70(6);
                if (gPolice_7B8_6FEE40->FBI_Army_5703E0(v11))
                {
                    Char_B4* v12 = this->field_168_game_object;
                    this->field_278_ped_state_1 = ped_state_1::in_car_10;
                    this->field_27C_ped_state_2 = ped_state_2::ped2_driving_10;
                    v12->field_84 = v11;
                    // goto LABEL_10;
                    this->field_248_enter_car_as_passenger = 1;
                    this->field_150_target_objective_car = v11;
                    return;
                }
            }
        }

        this->field_278_ped_state_1 = ped_state_1::walking_0;
        this->field_27C_ped_state_2 = ped_state_2::ped2_walking_0;
        SetObjective(objectives_enum::no_obj_0, 9999);
        sub_463830(0, 9999);
    }
}

MATCH_FUNC(0x46a1f0)
void Ped::sub_46A1F0()
{
    if (field_148_objective_target_ped->field_21C_bf.b0 == false ||
        field_148_objective_target_ped->field_278_ped_state_1 == ped_state_1::dead_9)
    {
        field_225_objective_status = objective_status::failed_2;
    }
    else
    {
        if (field_21C_bf.b2 && field_148_objective_target_ped == field_14C)
        {
            field_21C_bf.b2 = false;
            Ped::sub_463830(0, 9999);
            byte_61A8A3 = 1;
        }
        else if (!byte_61A8A3)
        {
            return;
        }

        if (field_25C_car_state)
        {
            if (field_25C_car_state == 32)
            {
                if (field_226 == 1)
                {
                    field_225_objective_status = objective_status::passed_1;
                }
                else if (field_226 == 2)
                {
                    field_225_objective_status = objective_status::failed_2;
                }
            }
        }
        else
        {
            Ped::sub_463830(32, 9999);
            field_14C = field_148_objective_target_ped;
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
            field_16C_car->field_60 = gHamburger_500_678E30->sub_474810();
            field_16C_car->field_60->field_4_ped_owner = this;
        }
        field_16C_car->field_60->field_8_maybe_path_type = 2;
        field_16C_car->sub_421560(5);
        field_16C_car->field_60->field_30_ped_to_follow = field_150_target_objective_car->field_54_driver;
        field_16C_car->field_A6 &= ~0x20u;
        field_16C_car->field_5C->field_74 = dword_67866C;
    }
}

STUB_FUNC(0x46a350)
char_type Ped::sub_46A350()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x46a530)
s32 Ped::sub_46A530()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x46a5e0)
void Ped::sub_46A5E0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x46a6d0)
s16 Ped::sub_46A6D0()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x46a7c0)
void Ped::DestroyTargetObject_46A7C0()
{
    if (byte_61A8A3 && (field_21C & 4) == 0)
    {
        switch (field_25C_car_state)
        {
            case 0:
                Ped::sub_463830(58, 9999);
                field_1A4 = field_1A0_objective_target_object;
                return;
            case 1:
                if (field_226 != 1)
                {
                    return;
                }
                break;
            case 58:
                if (field_226 == 1)
                {
                    field_225_objective_status = objective_status::passed_1;
                }
                if (field_226 == 2)
                {
                    field_225_objective_status = objective_status::failed_2;
                }
                return;
            default:
                return;
        }
        Ped::sub_463830(58, 9999);
        field_1A4 = field_1A0_objective_target_object;
    }
}

MATCH_FUNC(0x46a850)
void Ped::DestroyTargetCar_46A850()
{
    if (field_150_target_objective_car->field_74_damage == 32001)
    {
        field_225_objective_status = objective_status::passed_1;
    }
    if (byte_61A8A3 && (field_21C & 4) == 0)
    {
        switch (field_25C_car_state)
        {
            case 0:
                Ped::sub_463830(59, 9999);
                field_154_target_to_enter = field_150_target_objective_car;
                return;
            case 1:
                if (field_226 != 1)
                {
                    return;
                }
                break;
            case 59:
                if (field_226 == 1)
                {
                    field_225_objective_status = objective_status::passed_1;
                }
                if (field_226 == 2)
                {
                    field_225_objective_status = objective_status::failed_2;
                }
                return;
            default:
                return;
        }
        Ped::sub_463830(59, 9999);
        field_154_target_to_enter = field_150_target_objective_car;
    }
}

MATCH_FUNC(0x46a8f0)
void Ped::sub_46A8F0()
{
    if (gDistanceToTarget_678750 > dword_678520)
    {
        Char_B4* pB4 = field_168_game_object;
        if (pB4)
        {
            if (field_258_objective || pB4->field_44 == 2)
            {
                Ped::ChangeNextPedState1_45C500(ped_state_1::walking_0);
                Ped::ChangeNextPedState2_45C540(ped_state_2::ped2_walking_0);
                field_226 = 1;
            }
            else
            {
                pB4->field_38_velocity = get_max_speed_1F0();
            }
        }
        else
        {
            field_21C_bf.b11 = 0;
        }
    }
    else
    {
        field_168_game_object->field_38_velocity = get_max_speed_1F0();
    }
}

MATCH_FUNC(0x46a9c0)
void Ped::sub_46A9C0()
{
    field_14C->field_144 = 0;
    if (field_14C->field_278_ped_state_1 == ped_state_1::dead_9 || field_14C->field_21C_bf.b0 == false)
    {
        Ped::ChangeNextPedState1_45C500(ped_state_1::walking_0);
        Ped::ChangeNextPedState2_45C540(ped_state_2::ped2_walking_0);
        field_14C = 0;
        field_226 = 1;
    }
    else if (field_278_ped_state_1 != ped_state_1::immobilized_8)
    {
        if (gDistanceToTarget_678750 > dword_678520)
        {
            if (field_168_game_object->field_44 == 2)
            {
                Ped::ChangeNextPedState1_45C500(ped_state_1::walking_0);
                Ped::ChangeNextPedState2_45C540(ped_state_2::ped2_walking_0);
                field_14C = 0;
                field_226 = 1;
            }
        }
        else
        {
            Ped::ChangeNextPedState1_45C500(ped_state_1::flee_or_running_1);
            Ped::ChangeNextPedState2_45C540(ped_state_2::Unknown_3);
            field_168_game_object->SetMaxSpeed_433920(field_168_game_object->field_3C_run_or_jump_speed);
        }
    }
}

MATCH_FUNC(0x46aae0)
void Ped::sub_46AAE0()
{
    if (field_14C->field_278_ped_state_1 == ped_state_1::dead_9 || field_14C->field_21C_bf.b0 == false)
    {
        Ped::ChangeNextPedState1_45C500(ped_state_1::walking_0);
        Ped::ChangeNextPedState2_45C540(ped_state_2::ped2_walking_0);
        field_14C = 0;
        field_226 = 1;
        field_21C_bf.b2 = false;
    }
    else
    {
        field_14C->field_144 = 0;
        field_168_game_object->SetMaxSpeed_433920(field_168_game_object->field_3C_run_or_jump_speed);
    }
}

MATCH_FUNC(0x46ab50)
void Ped::sub_46AB50()
{
    if (field_14C->field_278_ped_state_1 == ped_state_1::dead_9 || field_14C->field_21C_bf.b0 == false)
    {
        Ped::ChangeNextPedState1_45C500(ped_state_1::walking_0);
        Ped::ChangeNextPedState2_45C540(ped_state_2::ped2_walking_0);
        field_14C = 0;
        field_226 = 1;
        Ped::sub_463830(0, 9999);
        field_21C_bf.b2 = false;
    }
    else
    {
        if (field_278_ped_state_1 != ped_state_1::immobilized_8)
        {
            field_21C_bf.b11 = false;
            field_14C->field_144 = 0;
            if (gDistanceToTarget_678750 < dword_6785EC)
            {
                Ped::ChangeNextPedState1_45C500(ped_state_1::walking_0);
                Ped::ChangeNextPedState2_45C540(ped_state_2::ped2_walking_0);
                field_14C = 0;
                field_226 = 1;
                Ped::sub_463830(0, 9999);
                field_21C_bf.b2 = false;
            }
            else
            {
                Ped::ChangeNextPedState1_45C500(ped_state_1::flee_or_running_1);
                Ped::ChangeNextPedState2_45C540(ped_state_2::Unknown_2);
                Ped::sub_4672E0(gDistanceToTarget_678750, 0);
                field_168_game_object->field_38_velocity = field_168_game_object->field_3C_run_or_jump_speed; // OBS: inline doesn't match
            }
        }
    }
}

STUB_FUNC(0x46ac20)
char_type Ped::FollowTargetStateMachine_46AC20()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x46b170)
s32 Ped::ChaseTargetStateMachine_46B170()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x46b2f0)
s32 Ped::PullDriverOutOfCarStateMachine_46B2F0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x46b670)
void Ped::MeleeAttackStateMachine_46B670()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x46bd30)
void Ped::sub_46BD30()
{
    if (field_21A_car_state_timer == 0)
    {
        field_226 = 1;
    }
}

WIP_FUNC(0x46bd50)
char_type Ped::sub_46BD50(Car_BC* pCar)
{
    WIP_IMPLEMENTED;

    u8 new_door_idx = 0;
    u8 door_idx = 0;
    if (!pCar->GetRemap())
    {
        return 0;
    }

    while (1)
    {
        Car_Door_10* pDoor = pCar->GetDoor(door_idx);
        if (pDoor)
        {
            Ped* f_8 = pDoor->field_8_pObj;
            if (f_8)
            {
                if (!f_8->field_248_enter_car_as_passenger && f_8 != this)
                {
                    break;
                }
            }
        }
        door_idx = ++new_door_idx;
        if (new_door_idx >= pCar->GetRemap())
        {
            return 0;
        }
    }
    return 1;
}

WIP_FUNC(0x46bdc0)
void Ped::EnterCarStateMachine_46BDC0()
{
    WIP_IMPLEMENTED;

    if (field_16C_car && this->field_154_target_to_enter == field_16C_car)
    {
        this->field_226 = 1;
        return;
    }

    if (this->field_154_target_to_enter == gGarage_48_6FD26C->field_0 && gGarage_48_6FD26C->field_3D)
    {
        if (IsField238_45EDE0(2))
        {
            sub_463830(0, 9999);
            return;
        }
        //goto LABEL_54;
        this->field_226 = 2;
        return;
    }

    this->field_224 &= ~0x10u;

    if (field_278_ped_state_1 == ped_state_1::immobilized_8)
    {
        return;
    }

    if (gDistanceToTarget_678750 > k_dword_678680 && this->field_238 != 5 &&
        this->field_28C_threat_reaction != threat_reaction_enum::react_as_emergency_1)
    {
        this->field_226 = 2;
        return;
    }

    if (!FindUsableCarDoor_467090())
    {
        Car_Door_10* pDoor = field_154_target_to_enter->GetDoor(this->field_24C_target_car_door);
        pDoor->sub_439EA0();
        if (this->field_27C_ped_state_2 == ped_state_2::ped2_entering_a_car_6)
        {
            Sprite* pCarSprite = this->field_154_target_to_enter->field_50_car_sprite;
            Ang16 ang_to_use = (word_6784FC + pCarSprite->field_0);
            field_168_game_object->sub_553E00(ang_to_use, dword_678634, k_dword_678660, 1);
        }
        ChangeNextPedState1_45C500(0);
        ChangeNextPedState2_45C540(0);
        return;
    }

    this->field_168_game_object->field_84 = this->field_154_target_to_enter;
    const char_type isPedKind = sub_45B4E0();
    Fix16 vel_to_check = dword_67856C;
    if (isPedKind)
    {
        vel_to_check = dword_678428;
    }

    if (field_154_target_to_enter->GetVelocity_43A4C0() >= vel_to_check ||
        gDistanceToTarget_678750 >= dword_678778 && this->field_27C_ped_state_2 != ped_state_2::ped2_entering_a_car_6)
    {
        goto LABEL_36;
    }

    if (field_168_game_object->field_80_sprite_ptr->field_C_sprite_4c_ptr->field_30_boundingBox.field_10_low_z <
        field_154_target_to_enter->field_50_car_sprite->field_C_sprite_4c_ptr->field_30_boundingBox.field_10_low_z)
    {
        if (field_168_game_object->field_80_sprite_ptr->field_C_sprite_4c_ptr->field_30_boundingBox.field_14_high_z >=
            field_154_target_to_enter->field_50_car_sprite->field_C_sprite_4c_ptr->field_30_boundingBox.field_10_low_z)
        {
            goto LABEL_30;
        }

    LABEL_36:
        this->field_168_game_object->field_38_velocity = this->field_168_game_object->field_3C_run_or_jump_speed;
        if (this->field_27C_ped_state_2 == ped_state_2::ped2_entering_a_car_6)
        {
            goto LABEL_49;
        }
        if (gDistanceToTarget_678750 <= k_dword_678658)
        {
            if (field_168_game_object->field_80_sprite_ptr->field_C_sprite_4c_ptr->field_30_boundingBox.field_10_low_z >=
                field_154_target_to_enter->field_50_car_sprite->field_C_sprite_4c_ptr->field_30_boundingBox.field_10_low_z)
            {
                if (field_168_game_object->field_80_sprite_ptr->field_C_sprite_4c_ptr->field_30_boundingBox.field_10_low_z <=
                    this->field_154_target_to_enter->field_50_car_sprite->field_C_sprite_4c_ptr->field_30_boundingBox.field_14_high_z)
                {
                LABEL_40:
                    if ((gDistanceToTarget_678750 <= dword_678794 || (field_168_game_object->field_58_flags & 1) != 1) &&
                        (gDistanceToTarget_678750 <= k_dword_678798 || field_168_game_object->field_69 == 1))
                    {
                        ChangeNextPedState1_45C500(3);
                        ChangeNextPedState2_45C540(4);
                        goto LABEL_49;
                    }
                }
            }
            else if (field_168_game_object->field_80_sprite_ptr->field_C_sprite_4c_ptr->field_30_boundingBox.field_14_high_z >=
                     field_154_target_to_enter->field_50_car_sprite->field_C_sprite_4c_ptr->field_30_boundingBox.field_10_low_z)
            {
                goto LABEL_40;
            }
        }
        sub_4672E0(gDistanceToTarget_678750, 2);
        goto LABEL_49;
    }

    if (field_168_game_object->field_80_sprite_ptr->field_C_sprite_4c_ptr->field_30_boundingBox.field_10_low_z >
        field_154_target_to_enter->field_50_car_sprite->field_C_sprite_4c_ptr->field_30_boundingBox.field_14_high_z)
    {
        goto LABEL_36;
    }

LABEL_30:
    this->field_21C |= 0x8000000u;
    field_168_game_object->field_38_velocity = k_dword_678438;
    if (field_27C_ped_state_2 == ped_state_2::ped2_staying_14 || field_27C_ped_state_2 == ped_state_2::ped2_following_a_car_4 ||
        field_27C_ped_state_2 == ped_state_2::Unknown_5)
    {
        Car_Door_10* pDoor_ = field_154_target_to_enter->GetDoor(field_24C_target_car_door);
        ChangeNextPedState2_45C540(6);
        ChangeNextPedState1_45C500(3);
        if (this->field_25C_car_state != 37)
        {
            pDoor_->field_8_pObj = this;
        }
        field_154_target_to_enter->sub_43A9F0();
    }

LABEL_49:
    if (!this->field_248_enter_car_as_passenger && this->field_27C_ped_state_2 == ped_state_2::ped2_entering_a_car_6)
    {
        if (sub_46BD50(field_154_target_to_enter))
        {
            ChangeNextPedState1_45C500(7);
            ChangeNextPedState2_45C540(14);
            Car_Door_10* pDoor__ = field_154_target_to_enter->GetDoor(this->field_24C_target_car_door);
            if (pDoor__)
            {
                pDoor__->field_8_pObj = 0;
            }
            //LABEL_54:
            this->field_226 = 2;
            return;
        }

        if (field_154_target_to_enter->field_54_driver)
        {
            if (field_168_game_object->field_6C_animation_state == 6)
            {
                if (field_168_game_object->field_68_animation_frame == 2 && this->field_15C_player &&
                    !field_154_target_to_enter->sub_4451E0(this))
                {
                    sub_463830(0, 9999);
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
                            field_15C_player->field_2D4_scores.sub_593240(field_154_target_to_enter);

                            // Is it gang car?
                            const s8 gang_car_model =
                                gGangPool_CA8_67E274->FindGangByCarModel_4BF2F0(field_154_target_to_enter->field_84_car_info_idx);
                            if (gang_car_model != -1)
                            {
                                // Well now they hate you a bit
                                Gang_144* pGang = gGangPool_CA8_67E274->GangByIdx_4BF1C0(gang_car_model);
                                pGang->sub_4BEF70(this->field_15C_player->field_2E_idx, 1u);
                            }
                        }
                    }
                }
            }
        }
    }
}

WIP_FUNC(0x46c250)
void Ped::ExitCarStateMachine_46C250()
{
    WIP_IMPLEMENTED;

    bool bUnknown = 0;
    this->field_21C |= 0x8000000u;

    if (field_27C_ped_state_2 == ped_state_2::ped2_driving_10)
    {
        if (this->field_16C_car->field_60)
        {
            gHamburger_500_678E30->Cancel_474CC0(this->field_16C_car->field_60);
            this->field_16C_car->field_60 = 0;
        }

        if (this->field_248_enter_car_as_passenger)
        {
            if (FindUsableCarDoor_467090())
            {
                bUnknown = 1;
                Car_Door_10* pDoor = field_154_target_to_enter->GetDoor(field_24C_target_car_door);
                if (!pDoor->field_8_pObj || this->field_25C_car_state == 38)
                {
                    field_16C_car->field_4_passengers_list.RemovePed_471240(this);
                    Fix16 char_x;
                    Fix16 char_y;
                    field_154_target_to_enter->GetDoorWorldPosition_43B5A0(field_24C_target_car_door, &char_x, &char_y);

                    Fix16 char_z;
                    char_z = *gMap_0x370_6F6268->sub_4E4E50(&char_z,
                                                            char_x,
                                                            char_y,
                                                            this->field_154_target_to_enter->field_50_car_sprite->field_1C_zpos);

                    AllocCharB4_45C830(char_x, char_y, char_z);
                    field_168_game_object->field_5_remap = field_244_remap;
                    if (field_244_remap != 0xFF)
                    {
                        field_168_game_object->field_80_sprite_ptr->SetRemap(field_244_remap);
                    }
                    ChangeNextPedState2_45C540(7);
                    ChangeNextPedState1_45C500(4);
                    this->field_16C_car = 0;
                    field_168_game_object->field_84 = field_154_target_to_enter;
                    this->field_154_target_to_enter = 0;
                    pDoor->field_8_pObj = this;
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

                Fix16 zpos;
                zpos = *gMap_0x370_6F6268->sub_4E4E50(&zpos,
                                                      field_154_target_to_enter->field_50_car_sprite->field_14_xy.x,
                                                      field_154_target_to_enter->field_50_car_sprite->field_14_xy.y,
                                                      field_154_target_to_enter->field_50_car_sprite->field_1C_zpos);

                AllocCharB4_45C830(field_154_target_to_enter->field_50_car_sprite->field_14_xy.x,
                                   field_154_target_to_enter->field_50_car_sprite->field_14_xy.y,
                                   zpos);
                field_168_game_object->field_5_remap = field_244_remap;
                if (field_244_remap != 0xFF)
                {
                    field_168_game_object->field_80_sprite_ptr->SetRemap(field_244_remap);
                }
                ChangeNextPedState2_45C540(0);
                ChangeNextPedState1_45C500(0);
                field_168_game_object->DoJump_5454D0();
                this->field_168_game_object->field_80_sprite_ptr->field_0 = this->field_154_target_to_enter->field_50_car_sprite->field_0;
                this->field_168_game_object->field_40_rotation = this->field_154_target_to_enter->field_50_car_sprite->field_0;
                this->field_168_game_object->field_5C = 10;
                this->field_16C_car = 0;
                field_168_game_object->field_84 = field_154_target_to_enter;
                this->field_154_target_to_enter = 0;
                this->field_226 = 1;
            }
        }
        else
        {
            if (this->field_258_objective != objectives_enum::objective_33)
            {
                field_154_target_to_enter->ClearDriver_4407F0();
            }

            if (FindUsableCarDoor_467090())
            {
                Fix16 char_x;
                Fix16 char_y;
                field_154_target_to_enter->GetDoorWorldPosition_43B5A0(field_24C_target_car_door, &char_x, &char_y);
                Fix16 zTmp;
                zTmp = *gMap_0x370_6F6268->sub_4E4E50(&zTmp,
                                                      char_x,
                                                      char_y,
                                                      this->field_154_target_to_enter->field_50_car_sprite->field_1C_zpos);
                AllocCharB4_45C830(char_x, char_y, zTmp);

                field_168_game_object->field_5_remap = field_244_remap;
                if (field_244_remap != 0xFF)
                {
                    field_168_game_object->field_80_sprite_ptr->SetRemap(field_244_remap);
                }

                ChangeNextPedState2_45C540(7);
                ChangeNextPedState1_45C500(4);

                this->field_16C_car = 0;
                field_168_game_object->field_84 = field_154_target_to_enter;
                this->field_154_target_to_enter = 0;
                return;
            }

            Fix16 zpos;
            zpos = *gMap_0x370_6F6268->sub_4E4E50(&zpos,
                                                  field_154_target_to_enter->field_50_car_sprite->field_14_xy.x,
                                                  field_154_target_to_enter->field_50_car_sprite->field_14_xy.y,
                                                  field_154_target_to_enter->field_50_car_sprite->field_1C_zpos);
            if (!AllocCharB4_45C830(field_154_target_to_enter->field_50_car_sprite->field_14_xy.x,
                                    field_154_target_to_enter->field_50_car_sprite->field_14_xy.y,
                                    zpos))
            {
                FatalError_4A38C0(1, "C:\\Splitting\\Gta2\\Source\\char.cpp", 11894);
            }

            field_168_game_object->field_5_remap = field_244_remap;
            if (field_244_remap != 0xFF)
            {
                field_168_game_object->field_80_sprite_ptr->SetRemap(field_244_remap);
            }

            ChangeNextPedState2_45C540(0);
            ChangeNextPedState1_45C500(0);

            this->field_168_game_object->field_38_velocity = k_dword_678438;
            field_168_game_object->DoJump_5454D0();
            field_168_game_object->field_80_sprite_ptr->field_0 = field_154_target_to_enter->field_50_car_sprite->field_0;
            this->field_168_game_object->field_40_rotation = this->field_154_target_to_enter->field_50_car_sprite->field_0;
            this->field_16C_car = 0;
            this->field_226 = 1;
            field_168_game_object->field_84 = field_154_target_to_enter;
            this->field_154_target_to_enter = 0;
        }
    }

    if (field_168_game_object)
    {
        if (field_154_target_to_enter && field_154_target_to_enter->field_88 == 5 ||
            (field_150_target_objective_car = this->field_150_target_objective_car) != 0 && field_150_target_objective_car->field_88 == 5 ||
            (this->field_158_unk_car) != 0 && field_158_unk_car->field_88 == 5 ||
            (field_168_game_object->field_84) != 0 && field_168_game_object->field_84->field_88 == 5)
        {
            this->field_278_ped_state_1 = ped_state_1::standing_still_7;
            this->field_27C_ped_state_2 = ped_state_2::ped2_staying_14;
            field_168_game_object->field_6C_animation_state = 2;
            this->field_168_game_object->field_68_animation_frame = 0;
        }
        else if (!FindUsableCarDoor_467090() && !bUnknown)
        {
            if (field_168_game_object)
            {
                field_168_game_object->DoJump_5454D0();
            }
        }

        if (this->field_27C_ped_state_2 != ped_state_2::ped2_getting_out_a_car_7 &&
            this->field_168_game_object->field_6C_animation_state != 7)
        {
            this->field_226 = 1;
            if (field_164_ped_group)
            {
                this->field_226 = field_164_ped_group->AreAllMembersOnFoot_4CAB80() != 0;
            }
        }
    }
}

MATCH_FUNC(0x46c770)
void Ped::sub_46C770()
{
    if (field_278_ped_state_1 != ped_state_1::immobilized_8)
    {
        if (field_168_game_object->field_44 == 2 || field_258_objective == objectives_enum::enter_car_as_driver_35 ||
            gDistanceToTarget_678750 < dword_678790)
        {
            Ped::sub_463830(0, 9999);
            field_21C_bf.b2 = false;
        }
        else
        {
            Ped::sub_4672E0(gDistanceToTarget_678750, 1);
            field_168_game_object->field_38_velocity = dword_678448; // inline doesn't match
        }
    }
}

MATCH_FUNC(0x46c7e0)
void Ped::sub_46C7E0()
{
    if (field_278_ped_state_1 != ped_state_1::immobilized_8)
    {
        if (gDistanceToTarget_678750 < dword_6784E8)
        {
            if (field_168_game_object->field_10_char_state != Char_B4_state::Jumping_15)
            {
                Ped::ChangeNextPedState1_45C500(ped_state_1::standing_still_7);
                Ped::ChangeNextPedState2_45C540(ped_state_2::ped2_staying_14);
                field_226 = 1;
            }
        }
        else
        {
            if (!field_21A_car_state_timer)
            {
                field_226 = 1;
            }
            if (field_240_occupation != ped_ocupation_enum::unknown_6 && field_258_objective != objectives_enum::patrol_on_foot_42)
            {
                field_168_game_object->SetMaxSpeed_433920(field_1F0_maybe_max_speed);
            }
            else
            {
                field_168_game_object->SetMaxSpeed_433920(field_1F4);
            }
        }
        Ped::sub_4672E0(gDistanceToTarget_678750, 1);
    }
}

MATCH_FUNC(0x46c8a0)
void Ped::sub_46C8A0()
{
    if (field_278_ped_state_1 != ped_state_1::immobilized_8)
    {
        field_168_game_object->field_38_velocity = dword_678448;
        if (gDistanceToTarget_678750 < dword_678790)
        {
            Ped::ChangeNextPedState1_45C500(ped_state_1::standing_still_7);
            Ped::ChangeNextPedState2_45C540(ped_state_2::ped2_staying_14);
            field_226 = 1;
        }
        else
        {
            field_230 = 2;
            Ped::sub_4672E0(gDistanceToTarget_678750, 2);
            field_168_game_object->field_38_velocity = dword_678448;
        }
    }
}

MATCH_FUNC(0x46c910)
void Ped::sub_46C910()
{
    if (field_278_ped_state_1 != ped_state_1::immobilized_8)
    {
        if (gDistanceToTarget_678750 < dword_6784E8)
        {
            field_18C = field_18C + 1; // next patrol point
            field_1C4_x = k_dword_67853C + Fix16(field_18C->field_0);
            field_1C8_y = k_dword_67853C + Fix16(field_18C->field_1);
            if (field_18C->field_0 == 0)
            {
                field_226 = 1;
                Ped::sub_463830(26, 9999);
            }
        }
        else
        {
            Ped::ChangeNextPedState1_45C500(ped_state_1::findind_path_2);
            Ped::ChangeNextPedState2_45C540(ped_state_2::ped2_walking_0);
            field_168_game_object->field_38_velocity = dword_678448;
        }
    }
}

MATCH_FUNC(0x46c9b0)
void Ped::sub_46C9B0()
{
    if (field_278_ped_state_1 != ped_state_1::immobilized_8)
    {
        if (gDistanceToTarget_678750 < dword_6784E8)
        {
            Ped::ChangeNextPedState1_45C500(ped_state_1::walking_0);
            Ped::ChangeNextPedState2_45C540(ped_state_2::ped2_walking_0);
            field_226 = 0;
            Ped::sub_463830(49, 100);
            if (--word_6787D0 < 0)
            {
                word_6787D0 = 0;
            }
        }
        else
        {
            if (field_21A_car_state_timer == 0)
            {
                field_226 = 1;
            }
            if (gTrafficLights_194_705958->field_192_phase == 7)
            {
                field_168_game_object->field_38_velocity = dword_678434;
            }
            else
            {
                if (field_168_game_object->field_44 == 1)
                {
                    field_168_game_object->field_38_velocity = dword_678448;
                }
                else
                {
                    field_168_game_object->field_38_velocity = dword_678434;
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
void Ped::sub_46CA70()
{
    if (!this->field_16C_car->field_60)
    {
        this->field_16C_car->field_60 = gHamburger_500_678E30->sub_474810();
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

    Car_BC* pBC = this->field_16C_car;
    pBC->field_7C_uni_num = 5;
    pBC->field_76_last_seen_timer = 0;
    this->field_16C_car->field_60->field_30_ped_to_follow = this->field_14C;
    this->field_16C_car->field_A6 &= ~0x20u;
    this->field_16C_car->field_5C->field_74 = dword_67866C;
    this->field_16C_car->field_60->field_20 = 1;
}

// 9.6f 0x43A550
WIP_FUNC(0x46cb30)
void Ped::StartPedCrossingAtTrafficLight_Y_Backward_46CB30()
{
    WIP_IMPLEMENTED;

    Fix16 y = this->field_1AC_cam.y;
    if (this->field_278_ped_state_1 != ped_state_1::immobilized_8 && gTrafficLights_194_705958->is_phase_7_434960())
    {
        for (u8 i = 0; i < 6; i++)
        {
            Fix16 z = (this->field_1AC_cam.z - k_dword_678664);
            y -= k_dword_678664;
            u8 yy = y.ToInt();
            if (gMap_0x370_6F6268->sub_433530(this->field_1AC_cam.x.ToInt(), yy, z.ToInt()))
            {
                sub_463830(48, 9999);
                Fix16 t = Fix16(yy) + k_dword_67853C;
                this->Set_F1C4_x_433C50(this->field_1AC_cam.x);
                this->Set_F1C8_y_433C60(t);
                this->Set_F1CC_z_433C70(this->field_1AC_cam.z);
                break;
            }
        }
    }
}

STUB_FUNC(0x46cc70)
char_type Ped::StartPedCrossingAtTrafficLight_X_Forwards_46CC70()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x46cdb0)
char_type Ped::StartPedCrossingAtTrafficLight_Y_Forwards_46CDB0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x46cef0)
char_type Ped::StartPedCrossingAtTrafficLight_X_Backwards_46CEF0()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x46d030)
void Ped::sub_46D030()
{
    if (field_278_ped_state_1 != ped_state_1::immobilized_8)
    {
        Train_58* pTrain = gPublicTransport_181C_6FF1D4->GetTrainFromCarExcludingLeadCar_57B6A0(field_154_target_to_enter);
        Car_BC* pOldTarget = field_154_target_to_enter;
        if (pTrain->field_4C_maybe_train_station->field_1C == 2 &&
            pTrain->field_10_carriages[0]->field_84_car_info_idx == car_model_enum::TRAIN)
        {
            Ped::sub_463830(37, 9999);
            field_154_target_to_enter = pOldTarget;
            field_168_game_object->field_84 = pOldTarget;
            field_168_game_object->field_38_velocity = dword_678448;
        }
    }
}

MATCH_FUNC(0x46d0b0)
void Ped::sub_46D0B0()
{
    if (field_21A_car_state_timer == 0)
    {
        field_226 = 1;
    }
}

WIP_FUNC(0x46d0d0)
void Ped::sub_46D0D0()
{
    WIP_IMPLEMENTED;

    s32 state1; // eax
    Fix16 curVal; // ebp
    u8 remap_num; // bl
    Fix16 door_xd; // eax
    Fix16 door_yd; // ecx
    Fix16 smaller_val; // eax
    Char_B4* pB4; // eax
    bool bUnknown; // [esp+Eh] [ebp-16h]
    u8 target_door; // [esp+Fh] [ebp-15h]
    u8 target_door_b; // [esp+10h] [ebp-14h]
    Fix16 doorX; // [esp+14h] [ebp-10h] BYREF
    Fix16 doorY; // [esp+18h] [ebp-Ch] BYREF
    Fix16 smaller;

    bUnknown = 0;
    state1 = this->field_278_ped_state_1;
    this->field_21C |= 0x8000000u;
    if (state1 != ped_state_1::immobilized_8)
    {
        if (gDistanceToTarget_678750 <= k_dword_678680)
        {
            if (gPublicTransport_181C_6FF1D4->GetTrainFromCarExcludingLeadCar_57B6A0(field_154_target_to_enter))
            {
                this->field_248_enter_car_as_passenger = 1;
            }
            else
            {
                this->field_248_enter_car_as_passenger = 0;
            }
            curVal = k_dword_678524;
            remap_num = 0;
            target_door = 0;
            target_door_b = 0;
            if (field_154_target_to_enter->GetRemap())
            {
                do
                {
                    if (field_154_target_to_enter->IsDoorAccessible_43AFE0(target_door_b))
                    {
                        field_154_target_to_enter->GetDoorWorldPosition_43B5A0(target_door_b, &doorX, &doorY);
                        door_xd = Fix16::Abs(doorX - this->field_1AC_cam.x);
                        door_yd = Fix16::Abs(doorY - this->field_1AC_cam.y);

                        // TODO: min/max ?
                        if (door_xd <= door_yd)
                        {
                            smaller = door_yd;
                        }
                        else
                        {
                            smaller = door_xd;
                        }

                        if (smaller < curVal)
                        {
                            curVal = smaller;
                            target_door = remap_num;
                            bUnknown = 1;
                        }
                    }
                    target_door_b = ++remap_num;
                } while (remap_num < field_154_target_to_enter->GetRemap());

                if (bUnknown == 1)
                {
                    pB4 = this->field_168_game_object;
                    this->field_24C_target_car_door = target_door;
                    pB4->field_38_velocity = pB4->field_3C_run_or_jump_speed;
                    EnterCarStateMachine_46BDC0();
                    if (this->field_226 == 1)
                    {
                        Deallocate_45EB60();
                    }
                }
            }
        }
        else
        {
            sub_463830(0, 9999);
            this->field_226 = 2;
        }
    }
}

WIP_FUNC(0x46d240)
void Ped::sub_46D240()
{
    WIP_IMPLEMENTED;

    bool v2; // al
    bool v7; // [esp+7h] [ebp-1h]

    v2 = 1;
    this->field_248_enter_car_as_passenger = 1;
    this->field_21C |= 0x8000000;
    if (field_27C_ped_state_2 == ped_state_2::ped2_driving_10)
    {
        v7 = 0;
        for (u8 i = 0; i < 5; i++)
        {
            if (field_154_target_to_enter->sub_43B140(field_24C_target_car_door))
            {
                v7 = 1;
                break;
            }
            this->field_24C_target_car_door++;
            if (field_24C_target_car_door > 3u)
            {
                this->field_24C_target_car_door = 0;
            }
        }
        v2 = v7;
    }

    if (this->field_226 == 1 || !v2)
    {
        if (IsField238_45EDE0(2))
        {
            SetObjective(objectives_enum::no_obj_0, 9999);
            sub_463830(0, 9999);
        }
    }
    else
    {
        ExitCarStateMachine_46C250();
    }
}

WIP_FUNC(0x46d300)
void Ped::sub_46D300()
{
    WIP_IMPLEMENTED;

    if (this->field_278_ped_state_1 != ped_state_1::immobilized_8)
    {
        if (gDistanceToTarget_678750 > dword_678788)
        {
            if (gDistanceToTarget_678750 >= dword_678790)
            {
                Char_B4* pB4__ = this->field_168_game_object;
                pB4__->field_38_velocity = dword_6784BC + pB4__->field_38_velocity;
                if (pB4__->field_38_velocity > pB4__->field_3C_run_or_jump_speed)
                {
                    pB4__->field_38_velocity = pB4__->field_3C_run_or_jump_speed;
                }
            }
            else
            {
                Fix16 total_ = k_dword_678430 + field_150_target_objective_car->GetVelocity_43A4C0();
                Char_B4* pB4_ = this->field_168_game_object;
                if (pB4_->field_38_velocity >= total_)
                {
                    if (pB4_->field_38_velocity > total_)
                    {
                        pB4_->field_38_velocity -= dword_678620;
                    }
                }
                else
                {
                    pB4_->field_38_velocity += dword_678620;
                }
            }
        }
        else
        {
            if (this->field_168_game_object->field_10_char_state != 15)
            {
                if (gDistanceToTarget_678750 >= dword_6784E8)
                {
                    Fix16 total = k_dword_678430 + field_150_target_objective_car->GetVelocity_43A4C0();
                    if (field_168_game_object->field_38_velocity >= total)
                    {
                        if (field_168_game_object->field_38_velocity > total)
                        {
                            field_168_game_object->field_38_velocity -= dword_678620;
                        }
                    }
                    else
                    {
                        field_168_game_object->field_38_velocity += dword_678620;
                    }
                }
                else
                {
                    if (field_150_target_objective_car->GetVelocity_43A4C0() <= GetPedVelocity_45C920())
                    {
                        this->field_168_game_object->field_38_velocity = field_150_target_objective_car->GetVelocity_43A4C0();
                    }
                }
            }
        }
        sub_4672E0(gDistanceToTarget_678750, 1);
    }
}

STUB_FUNC(0x46d460)
void Ped::AttackTargetStateMachine_46D460(char_type a2)
{
    NOT_IMPLEMENTED;
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
    field_17C_pZone = pToFollow->field_17C_pZone;
    sub_433BB0(1);
    sub_433BC0(1);
    SetField238_403920(4);
}

MATCH_FUNC(0x46e020)
bool Ped::sub_46E020(PedGroup* pGroup)
{
    return this->field_164_ped_group != pGroup && !this->field_15C_player &&
            (IsField238_45EDE0(3) || (IsField238_45EDE0(4) || IsField238_45EDE0(6)) && this->field_240_occupation == 35) ?
        true :
        false;
}

WIP_FUNC(0x46e080)
void Ped::RecruitNearbyPeds_46E080(s32 desiredCount, Fix16 searchRadius)
{
    WIP_IMPLEMENTED;

    PedGroup* pGroup_; // ecx
    int z_copy; // ebx
    Fix16 x; // edi
    Fix16 y; // ecx
    Sprite* pSprite; // eax
    Sprite* pNearest; // eax
    Char_B4* pB4; // eax
    Ped* pPed; // edi
    PedGroup* pGroup; // ecx
    struct_4 collision_list; // [esp+8h] [ebp-1Ch] BYREF
    Fix16_Rect rect; // [esp+Ch] [ebp-18h] BYREF

    collision_list.field_0_p18 = 0;
    pGroup_ = this->field_164_ped_group;
    if (!pGroup_)
    {
        SpawnPedGroupFollowers_46E200(0);
        z_copy = desiredCount;
    LABEL_7:
        x = this->field_1AC_cam.x;
        // desiredCount = this->field_1AC_cam.z;
        y = this->field_1AC_cam.y;
        rect.field_0_left = x - searchRadius / 2;
        rect.field_4_right = searchRadius / 2 + x;
        rect.field_C_bottom = y + searchRadius / 2;
        rect.field_8_top = y - searchRadius / 2;
        rect.field_10_low_z = searchRadius - k_dword_67845C;
        rect.field_14_high_z = searchRadius + k_dword_67845C;
        pSprite = GetSprite_46DF50();
        if (gPurpleDoom_1_679208->CollectRectCollisions_477F30(&rect, 0, 0, pSprite, &collision_list))
        {
            for (pNearest = collision_list.TakeClosestSprite_5A6EA0(this->field_1AC_cam.x, this->field_1AC_cam.y); pNearest;
                 pNearest = collision_list.TakeClosestSprite_5A6EA0(this->field_1AC_cam.x, this->field_1AC_cam.y))
            {
                pB4 = pNearest->AsCharB4_40FEA0();

                if (pB4)
                {
                    pPed = pB4->field_7C_pPed;
                    if (pPed->sub_46E020(this->field_164_ped_group))
                    {
                        pGroup = pPed->field_164_ped_group;
                        if (pGroup)
                        {
                            pGroup->RemovePed_4C9970(pPed);
                        }
                        field_164_ped_group->add_ped_to_end_of_list_4C8F90(pPed);
                        pPed->SetupFollower_46DF70(this, weapon_type::dual_pistol);
                        if (this->field_164_ped_group->field_34_count == z_copy)
                        {
                            break;
                        }
                    }
                }
            }
        }
        collision_list.ClearList_5A6E10();
        return;
    }

    z_copy = desiredCount;
    // max(desiredCount, 9)
    if (desiredCount > 9)
    {
        z_copy = 9;
    }

    if (pGroup_->field_34_count < z_copy)
    {
        goto LABEL_7;
    }
}

MATCH_FUNC(0x46e200)
void Ped::SpawnPedGroupFollowers_46E200(u8 total)
{
    PedGroup* pGroup = PedGroup::New_4CB0D0();
    pGroup->add_ped_leader_4C9B10(this);
    u8 current = 0;
    pGroup->field_36_count = total;
    pGroup->field_34_count = total;
    if (total > 0)
    {
        s32 i = 0;
        do
        {

            Ped* pNewPed = gPedPool_6787B8->field_0_pool.Allocate();

            pNewPed->field_240_occupation = this->field_240_occupation;
            pNewPed->field_244_remap = this->field_244_remap;
            pNewPed->field_26C_graphic_type = this->field_26C_graphic_type;
            pNewPed->field_238 = this->field_238;
            Fix16 xy_off = k_dword_678504 * Fix16(i);
            pNewPed->AllocCharB4_45C830(xy_off + this->field_1AC_cam.x, xy_off + this->field_1AC_cam.y, this->field_1AC_cam.z);
            Char_B4* pB4 = pNewPed->field_168_game_object;
            const u8 remap = this->field_244_remap;
            pB4->field_5_remap = field_244_remap;
            if (remap != 0xFF)
            {
                pB4->field_80_sprite_ptr->SetRemap(remap);
            }
            pNewPed->field_216_health = this->field_216_health;
            pNewPed->field_230 = this->field_230;
            pNewPed->field_22C = this->field_22C;
            pNewPed->field_288_threat_search = this->field_288_threat_search;
            pNewPed->field_28C_threat_reaction = this->field_28C_threat_reaction;
            pNewPed->field_17C_pZone = this->field_17C_pZone;
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

MATCH_FUNC(0x46f070)
void Ped::set_wanted_star_count_46F070(u8 star_count)
{
    switch (star_count)
    {
        case 0u:
            field_20A_wanted_points = 0;
            break;
        case 1u:
            field_20A_wanted_points = 600;
            break;
        case 2u:
            field_20A_wanted_points = 1600;
            break;
        case 3u:
            field_20A_wanted_points = 3000;
            break;
        case 4u:
            field_20A_wanted_points = 5000;
            break;
        case 5u:
            field_20A_wanted_points = 8000;
            break;
        case 6u:
            field_20A_wanted_points = 12000;
            break;
        default:
            return;
    }
}

MATCH_FUNC(0x46f100)
s32 Ped::sub_46F100(u8 a2)
{
    return a2 < get_wanted_star_count_46EF00();
}

WIP_FUNC(0x46f110)
Weapon_30* Ped::GetWeaponFromPed_46F110()
{
    WIP_IMPLEMENTED;

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
        if (!field_16C_car || field_238 == 2)
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
            // TODO: Needs to be one call to find_5E3D20 but compiler always give us 3
            Car_BC* t = field_16C_car;
            Weapon_30* r;
            if (t->IsFireTruck_4118F0())
            {
                r = gWeapon_8_707018->find_5E3D20(t, 20);
            }
            else if (t->IsTank_411900())
            {
                r = gWeapon_8_707018->find_5E3D20(t, 19);
            }
            else if (t->IsGunJeep_411910())
            {
                r = gWeapon_8_707018->find_5E3D20(t, 22);
            }
            else
            {
                return 0;
            }

            if (r)
            {
                return r;
            }
        }
    }

    return 0;
}

WIP_FUNC(0x46f1e0)
void Ped::sub_46F1E0(Weapon_30* a2)
{
    WIP_IMPLEMENTED;

    char_type rng_val = 0;
    if (a2->field_1C_idx <= (u32)weapon_type::smg)
    {
        if (field_270)
        {
            if (field_270 == 2)
            {
                rng_val = 0;
            }
        }
        else if (GetPedVelocity_45C920() == k_dword_678660)
        {
            rng_val = stru_6F6784.get_int_4F7AE0(3);
        }
        else
        {
            rng_val = stru_6F6784.get_int_4F7AE0(5);
            if (rng_val == 0)
            {
                rng_val = stru_6F6784.get_int_4F7AE0(5);
            }
        }

        switch (rng_val)
        {
            case 1:
                field_12E -= word_6784C8;
            case 2:
                field_12E += word_6784C8;
                break;
            case 3:
                field_12E -= dword_6784E4;
                break;
            case 4:
                field_12E += dword_6784E4;
                break;
            default:
                return;
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
            field_12E = field_168_game_object->field_80_sprite_ptr->field_0;
        }
    }
    if (field_21C_bf.b11 == true)
    {
        if (pWeapon)
        {
            if (!field_267_varrok_idx)
            {
                field_267_varrok_idx = gVarrok_7F8_703398->sub_59B060(field_200_id);
            }
            if (!field_21C_bf.b7)
            {
                if (field_168_game_object)
                {
                    if (!Ped::IsField238_45EDE0(2))
                    {
                        Ped::sub_46F1E0(pWeapon);
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
            if (field_238 == 2)
            {
                Ped::HandleClosePedInteraction_45CAA0();
            }
        }
    }
    else if (pWeapon)
    {
        pWeapon->sub_5E34B0();
    }
}

// 9.6f 0x434E60
WIP_FUNC(0x46f490)
Weapon_30* Ped::sub_46F490()
{
    WIP_IMPLEMENTED;

    Car_BC* pCar;

    switch (this->field_240_occupation)
    {
        case ped_ocupation_enum::police:
        case ped_ocupation_enum::unknown_14:
            if (!field_14C->IsField238_45EDE0(2))
            {
                this->field_21C_bf.b13 = 1;
                return this->field_174_pWeapon;
            }

            pCar = this->field_14C->field_16C_car;
            if (pCar)
            {
                if (pCar->GetVelocity_43A4C0() > k_dword_678624)
                {
                    this->field_21C_bf.b13 = 1;
                    return this->field_174_pWeapon;
                }
                this->field_21C_bf.b9 = 1;
                return 0;
            }

            if (gDistanceToTarget_678750 < k_dword_678658 + k_dword_678798)
            {
                ++unk_6787EE; // police peds in range screen
            }

            // TODO: Wrong here, check needs inverting maybe
            if (gPolice_7B8_6FEE40->field_7AD_police_peds_in_range_screen < 2u)
            {
                this->field_198 = 0;
                this->field_21C_bf.b9 = 1;
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
            if (this->field_14C->field_16C_car || gDistanceToTarget_678750 > k_dword_67853C)
            {
                this->field_21C_bf.b13 = 1;
                return this->field_174_pWeapon;
            }
            return this->field_170_selected_weapon;

        case ped_ocupation_enum::unknown_17:
            if (!field_14C->IsField238_45EDE0(2) || this->field_14C->field_16C_car)
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
        pWeapon->field_4 = 1;
        if (field_170_selected_weapon->IsExplosiveWeapon_5E3BD0())
        {
            GiveWeapon_46F650(weapon_type::pistol);
        }
    }
}

MATCH_FUNC(0x46f650)
void Ped::GiveWeapon_46F650(s32 weapon_kind)
{
    sub_462550();
    Weapon_30* pWeapon = gWeapon_8_707018->allocate_5E3C10(weapon_kind, this, 99u);
    this->field_174_pWeapon = pWeapon;
    pWeapon->field_4 = 1;
}

MATCH_FUNC(0x46f680)
void Ped::sub_46F680(Ped* pPed)
{
    if (field_17C_pZone)
    {
        if (field_17C_pZone->field_111)
        {
            if (field_290 != 3 && field_290 != 1)
            {
                field_17C_pZone->sub_4BEF70(pPed->field_15C_player->field_2E_idx, 5);
            }
            else
            {
                field_17C_pZone->sub_4BEF70(pPed->field_15C_player->field_2E_idx, 1);
            }
        }
    }
    else
    {
        if (field_19C && field_19C->field_111)
        {
            if (field_290 != 3 && field_290 != 1)
            {
                field_19C->sub_4BEF70(pPed->field_15C_player->field_2E_idx, 5);
            }
            else
            {
                field_19C->sub_4BEF70(pPed->field_15C_player->field_2E_idx, 1);
            }
        }
    }
}

WIP_FUNC(0x46f720)
void Ped::UpdateStatsForKiller_46F720()
{
    WIP_IMPLEMENTED;

    s32 ped_id; // eax
    Ped* pKillerPed; // eax
    Player* pPlayerIter; // edi
    Ped* pPlayerPed; // eax
    Ped* pPedKiller; // ecx
    PedGroup* pGroup; // ecx
    Player* pPlayer; // edx
    Ped* pPlayerPed_; // eax

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
                if (gShooey_CC_67A4B8->sub_485140(this, this->field_1A8_ped_killer->field_15C_player))
                {
                    if (this->field_17C_pZone || this->field_19C)
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
                        gLucid_hamilton_67E8E0.UpdateFrags_4C5CD0(this->field_1A8_ped_killer->field_15C_player->field_2E_idx,
                                                                  this->field_15C_player->field_2E_idx);
                        gHud_2B00_706620->field_12F0.AnnounceKill_5D5770(this->field_1A8_ped_killer->field_15C_player,
                                                                         this->field_15C_player);
                    }
                }
                else if (IsField238_45EDE0(2))
                {
                    bool doIt = true;
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
                                    gLucid_hamilton_67E8E0.UpdateFrags_4C5CD0(pPlayerIter->field_2E_idx,
                                                                              this->field_15C_player->field_2E_idx);
                                    gHud_2B00_706620->field_12F0.AnnounceKill_5D5770(pPlayerIter, this->field_15C_player);
                                    doIt = false;
                                    break;
                                }
                            }
                        }
                        //goto LABEL_25;
                    }

                    // TODO: Missing test edi, edi & jmp

                    //else
                    if (!doIt)
                    {
                        //LABEL_25:
                        gLucid_hamilton_67E8E0.UpdateFrags_4C5CD0(this->field_15C_player->field_2E_idx,
                                                                  this->field_15C_player->field_2E_idx);
                        if (!field_1A8_ped_killer->sub_45B4E0())
                        {
                            gHud_2B00_706620->field_12F0.AnnounceKill_5D5770(this->field_15C_player, this->field_15C_player);
                        }
                    }
                }
            }
            pPedKiller = this->field_1A8_ped_killer;
            if (this != pPedKiller)
            {
                if (pPedKiller->IsField238_45EDE0(2))
                {
                    field_1A8_ped_killer->field_15C_player->field_2D4_scores.sub_592660(this, this->field_1A8_ped_killer);
                    sub_46F680(this->field_1A8_ped_killer);
                }
                else
                {
                    pGroup = this->field_1A8_ped_killer->field_164_ped_group;
                    if (pGroup)
                    {
                        pPlayer = gGame_0x40_67E008->field_38_orf1;
                        pPlayerPed_ = pPlayer->field_2C4_player_ped;
                        if (pPlayerPed_)
                        {
                            if (pGroup == pPlayerPed_->field_164_ped_group)
                            {
                                sub_46F680(pPlayer->field_2C4_player_ped);
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
            // Argument loading wrong somehow
            gLucid_hamilton_67E8E0.UpdateFrags_4C5CD0(this->field_15C_player->field_2E_idx, this->field_15C_player->field_2E_idx);
            gHud_2B00_706620->field_12F0.AnnounceKill_5D5770(this->field_15C_player, this->field_15C_player);
        }
    }
}

WIP_FUNC(0x46f9d0)
void Ped::Kill_46F9D0()
{
    WIP_IMPLEMENTED;

    Char_B4* pB4; // eax
    PedGroup* pGroup; // ecx
    Char_B4* field_168_game_object; // eax
    s32 v5; // eax
    PedGroup* field_164_ped_group; // ecx
    PedGroup* v7; // ecx
    PedGroup* v9; // ecx
    s32 field_278_ped_state_1; // eax

    if ((this->field_21C & ped_bit_status_enum::k_ped_in_flames) != 0)
    {
        pB4 = this->field_168_game_object;
        if (pB4)
        {
            pB4->field_6C_animation_state = 21;
        }
        PutOutFire();
    }

    if (this->field_278_ped_state_1 != ped_state_1::dead_9)
    {
        sub_463830(0, 9999);
        if (this->field_27C_ped_state_2 == ped_state_2::lying_on_floor_22)
        {
            this->field_278_ped_state_1 = ped_state_1::dead_9;
            this->field_27C_ped_state_2 = ped_state_2::Unknown_15;
        }
        else
        {
            Ped::ChangeNextPedState1_45C500(9);
            Ped::ChangeNextPedState2_45C540(15);
        }

        if ((this->field_21C & ped_bit_status_enum::k_ped_in_flames) == 0)
        {
            this->field_250 = 4;
        }

        UpdateStatsForKiller_46F720();

        switch (this->field_240_occupation)
        {
            case ped_ocupation_enum::unknown_13:
                if (!gAmbulance_110_6F70A8->sub_4FA330(this))
                {
                    goto LABEL_22;
                }
                return;

            case ped_ocupation_enum::police:
            case ped_ocupation_enum::swat:
            case ped_ocupation_enum::fbi:
                if (!IsField238_45EDE0(4) || !gPolice_7B8_6FEE40->sub_56F4D0(this))
                {
                    goto LABEL_22;
                }
                return;

            case ped_ocupation_enum::elvis:
            case ped_ocupation_enum::elvis_leader:
                pGroup = this->field_164_ped_group;
                if (pGroup)
                {
                    if (!this->field_1A8_ped_killer)
                    {
                        this->field_1A8_ped_killer = this;
                    }
                    pGroup->DisbandGroupDueToAttack_4C94E0(this->field_1A8_ped_killer);
                }
                goto LABEL_22;

            case ped_ocupation_enum::unknown_1:
                Deallocate_45EB60();
                return;

            default:
            LABEL_22:
                field_168_game_object = this->field_168_game_object;
                if (field_168_game_object)
                {
                    field_168_game_object->field_16 = 1;
                    v5 = this->field_238;
                    field_164_ped_group = this->field_164_ped_group;
                    if (v5 == 2)
                    {
                        if (field_164_ped_group)
                        {
                            field_164_ped_group->DestroyGroup_4C93A0();
                        }
                    }
                    else if (!field_164_ped_group && v5 != 5)
                    {
                        SetObjective(objectives_enum::objective_28, 9999);
                    }
                    this->field_168_game_object->field_80_sprite_ptr->field_28_num = 6;
                }
                else if (this->field_238 == 2)
                {
                    if (this->field_16C_car)
                    {
                        field_15C_player->sub_564C00();
                    }
                    v7 = this->field_164_ped_group;
                    if (v7)
                    {
                        v7->DestroyGroup_4C93A0();
                    }
                    this->field_21C &= ~0x8;
                }
                else
                {
                    Deallocate_45EB60();
                }

                v9 = this->field_164_ped_group;
                if (!v9 || this->field_28C_threat_reaction == threat_reaction_enum::react_as_emergency_1)
                {
                    field_278_ped_state_1 = this->field_278_ped_state_1;
                    this->field_216_health = 0;
                    this->field_20A_wanted_points = 0;
                    this->field_21C &= 0x800;
                    if (field_278_ped_state_1 == ped_state_1::dead_9 && this->field_27C_ped_state_2 == ped_state_2::Unknown_15)
                    {
                        SpawnWeaponOnDeath_45E080();
                    }

                    gThreateningPedsList_678468.RemovePed_471240(this);

                    if (bDo_blood_67D5C5)
                    {
                        if (!this->field_16C_car && this->field_168_game_object->field_6C_animation_state != 17)
                        {
                            gParticle_8_6FD5E8->SpawnBlood_53E880(this->field_1AC_cam.x, this->field_1AC_cam.y, this->field_1AC_cam.z);
                        }
                    }
                }
                else
                {
                    v9->RemovePed_4C9970(this);
                }
                break;
        }
    }
}

MATCH_FUNC(0x46fc70)
void Ped::AddThreateningPedToList_46FC70()
{
    unk_6787EF = 1;
    gThreateningPedsList_678468.AddPedToFrontIfMissing_4711B0(this);
}

WIP_FUNC(0x46fc90)
void Ped::HandleShootingAtCar_46FC90(Car_BC* a2, s32 model)
{
    WIP_IMPLEMENTED;

    Weapon_30* pWeapon;
    if ((this->field_21C & 0x2000) != 0)
    {
        pWeapon = this->field_174_pWeapon;
    }
    else
    {
        pWeapon = this->field_170_selected_weapon;
    }

    Car_BC* pCar_1;

    if (pWeapon)
    {
        Car_BC* pCar = a2;
        if (a2->GetVelocity_43A4C0() == k_dword_678660)
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

        Sprite* pSprite = pCar->field_50_car_sprite;
        Fix16 xd = pSprite->field_14_xy.x - field_1AC_cam.x;
        Fix16 yd = pSprite->field_14_xy.y - field_1AC_cam.y;
        Fix16 v15 = Fix16::Abs(yd);
        Fix16 v9 = Fix16::Abs(xd);
        Fix16 v10 = Fix16::Max_44E540(v9, v15);

        if (pCar == this->field_154_target_to_enter)
        {
            goto LABEL_24;
        }

        if (field_14C)
        {
            Car_BC* f16C_car = field_14C->field_16C_car;
            if (f16C_car)
            {
                if (f16C_car == pCar)
                {
                    goto LABEL_24;
                }
            }
        }

        if (v10 < k_dword_678798 + dword_678790 && pCar->GetVelocity_43A4C0() < dword_678630)
        {
            pWeapon->field_4 = 1;
            return;
        }

        if (!pWeapon->IsExplosiveWeapon_5E3BD0())
        {
            pWeapon->field_4 = 0;
        }

        if (field_14C)
        {
            pCar_1 = field_14C->field_16C_car;
            if (pCar_1)
            {
                if (pCar == pCar_1)
                {
                LABEL_24:
                    pWeapon->field_4 = 0;
                }
            }
        }
    }
}

WIP_FUNC(0x46fe20)
void Ped::ProcessWeaponHitResponse_46FE20(Object_2C* pObj)
{
    WIP_IMPLEMENTED;

    Weapon_30* pWeapon; // ebx
    if ((this->field_21C & 0x2000) != 0)
    {
        pWeapon = this->field_174_pWeapon;
    }
    else
    {
        pWeapon = this->field_170_selected_weapon;
    }

    if (pWeapon && !Ped::IsField238_45EDE0(2))
    {
        Fix16 yd = pObj->field_4->field_14_xy.y - this->field_1AC_cam.y;
        Fix16 xd = pObj->field_4->field_14_xy.x - this->field_1AC_cam.x;

        Fix16 yd_abs = Fix16::Abs(yd);
        Fix16 xd_abs = Fix16::Abs(xd);
        Fix16 xd_yd_abs = Fix16::Max_44E540(xd_abs, yd_abs);

        if (pObj == this->field_1A4)
        {
            pWeapon->field_4 = 0;
            return;
        }

        if (xd_yd_abs < k_dword_678658)
        {
            pWeapon->field_4 = 1;
            return;
        }

        if (!pWeapon->IsExplosiveWeapon_5E3BD0())
        {
            pWeapon->field_4 = 0;
            return;
        }
    }
}

WIP_FUNC(0x46ff00)
void Ped::NotifyWeaponHit_46FF00(Fix16 xpos, Fix16 ypos, s32 model)
{
    WIP_IMPLEMENTED;

    Weapon_30* pWeapon; // edi

    if ((this->field_21C & 0x2000) != 0)
    {
        pWeapon = this->field_174_pWeapon;
    }
    else
    {
        pWeapon = this->field_170_selected_weapon;
    }

    if (pWeapon)
    {
        if (IsField238_45EDE0(2))
        {
            field_15C_player->field_2D4_scores.UpdateAccuracyCount_5934F0(1u, model, 0);
        }
        else
        {
            Fix16 xd = xpos - field_1AC_cam.x;
            Fix16 yd = ypos - field_1AC_cam.y;
            Fix16 xabs = Fix16::Abs(xd);
            Fix16 yabs = Fix16::Abs(yd);
            pWeapon->field_4 = Fix16::Max_44E540(xabs, yabs) < k_dword_678658 ? 1 : 0;
        }
    }
}

MATCH_FUNC(0x46fff0)
void Ped::sub_46FFF0(s32 model)
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
            pWeapon->field_4 = 0;
        }
    }
    else
    {
        Weapon_30* pSelectedWeapon = this->field_170_selected_weapon;
        if (pSelectedWeapon)
        {
            pSelectedWeapon->field_4 = 0;
        }
    }
}

WIP_FUNC(0x470050)
void Ped::AimRoofGun_470050()
{
    WIP_IMPLEMENTED;

    Sprite_18* pHit = 0;
    if (field_16C_car->field_84_car_info_idx == car_model_enum::FIRETRUK)
    {
        pHit = field_16C_car->field_0_qq.GetSpriteForModel_5A6A50(114);
    }
    else if (field_16C_car->field_84_car_info_idx == car_model_enum::TANK)
    {
        pHit = field_16C_car->field_0_qq.GetSpriteForModel_5A6A50(148);
    }
    else if (field_16C_car->field_84_car_info_idx == car_model_enum::GUNJEEP)
    {
        pHit = field_16C_car->field_0_qq.GetSpriteForModel_5A6A50(248);
    }

    Sprite* pHitSprite = pHit->field_0;

    Ped* objective_target_ped = this->field_148_objective_target_ped;
    Ang16 tan_v = Fix16::atan2_fixed_405320(objective_target_ped->field_1AC_cam.x - pHitSprite->field_14_xy.x,
                                            objective_target_ped->field_1AC_cam.y - pHitSprite->field_14_xy.y);

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
        else if (gDistanceToTarget_678750 < k_dword_678680)
        {
            this->field_21C |= 0x800;
        }
    }
    else
    {
        this->field_21C &= ~0x800;
    }
}

WIP_FUNC(0x470160)
void Ped::add_wanted_points_470160(s16 wanted_amount)
{
    WIP_IMPLEMENTED;

    field_20A_wanted_points += wanted_amount;

    if (field_20A_wanted_points <= 12000)
    {
        if (field_20A_wanted_points < 0)
        {
            field_20A_wanted_points = 0;
        }
    }
    else
    {
        field_20A_wanted_points = 12000;
    }

    if (get_wanted_star_count_46EF00() >= gPolice_7B8_6FEE40->field_660_wanted_star_count)
    {
        set_wanted_star_count_46F070(gPolice_7B8_6FEE40->field_660_wanted_star_count);
    }
}

MATCH_FUNC(0x4701d0)
bool Ped::sub_4701D0()
{
    Sprite* pSprite = gPurpleDoom_1_679208->FindNearestSpriteOfType_477E60(this->field_168_game_object->field_80_sprite_ptr, 0);
    if (pSprite)
    {
        return (pSprite->field_30_sprite_type_enum != sprite_types_enum::car) ? false : true;
    }
    return false;
}

MATCH_FUNC(0x470200)
void Ped::sub_470200(Fix16 a2, Fix16 a3, Fix16 a4)
{
    Ped::AllocCharB4_45C830(a2, a3, a4);
    Char_B4* pB4 = field_168_game_object;
    u8 remap = field_244_remap;
    pB4->field_5_remap = remap;
    if (remap != 0xFF)
    {
        pB4->field_80_sprite_ptr->SetRemap(remap);
    }
    if (field_238 == 2)
    {
        Ped::ChangeNextPedState2_45C540(ped_state_2::ped2_walking_0);
        Ped::ChangeNextPedState1_45C500(ped_state_1::walking_0);
        field_168_game_object->field_38_velocity = k_dword_678438;
    }
    else
    {
        Ped::ChangeNextPedState2_45C540(ped_state_2::ped2_walking_0);
        Ped::ChangeNextPedState1_45C500(ped_state_1::walking_0);
    }
    field_16C_car = 0;
}

MATCH_FUNC(0x4702d0)
void Ped::sub_4702D0(Ped* pPed)
{
    PedGroup* pPedGroup = pPed->field_164_ped_group;
    pPedGroup->replace_leader_4C8FE0(this);
    this->field_164_ped_group = pPedGroup;
}

WIP_FUNC(0x470300)
void Ped::sub_470300()
{
    WIP_IMPLEMENTED;

    this->field_15C_player = 0;
    this->field_240_occupation = 3;
    this->field_238 = 3;
    Car_BC* pCar = this->field_16C_car;
    if (pCar)
    {
        // NOTE: Split into a function chunk here
        const s32 info_idx = pCar->field_84_car_info_idx;
        if (info_idx != 59 && info_idx != 60 && info_idx != 61 && info_idx != 6)
        {
            pCar->field_7C_uni_num = 3;
            pCar->field_76_last_seen_timer = 0;
            CarPhysics_B0* pCarPhysics = pCar->field_58_physics;
            if (pCarPhysics)
            {
                pCarPhysics->field_8C_state = 1;
            }
        }
    }
}

MATCH_FUNC(0x4702A0)
void Ped::PushPatrolPoint_4702A0(s8 x, s8 y, s8 z)
{
    // Get a free patrol point
    Marz_3* pIter = this->field_190->field_0;
    while (pIter->field_0)
    {
        ++pIter;
    }

    // And populate it
    pIter->field_0 = x;
    pIter->field_1 = y;
    pIter->field_2 = z;
}

MATCH_FUNC(0x470f00)
s32 Ped::sub_470F00()
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
