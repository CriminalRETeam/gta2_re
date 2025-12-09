#include "Player.hpp"
#include "BurgerKing_67F8B0.hpp"
#include "CarPhysics_B0.hpp"
#include "Car_BC.hpp"
#include "error.hpp"
#include "Frontend.hpp"
#include "Function.hpp"
#include "Game_0x40.hpp"
#include "Gang.hpp"
#include "Globals.hpp"
#include "Hud.hpp"
#include "lucid_hamilton.hpp"
#include "NetPlay.hpp"
#include "Ped.hpp"
#include "Police_7B8.hpp"
#include "Weapon_30.hpp"
#include "Weapon_8.hpp"
#include "debug.hpp"
#include "frosty_pasteur_0xC1EA8.hpp"
#include "infallible_turing.hpp"
#include "map_0x370.hpp"
#include "rng.hpp"
#include "root_sound.hpp"
#include "text_0x14.hpp"
#include "winmain.hpp"
#include "youthful_einstein.hpp"
#include "zealous_borg.hpp"
#include <DINPUT.H>

EXTERN_GLOBAL(bool, gCheatUnlimitedFlameThrower_67D6CC);
EXTERN_GLOBAL(bool, gCheatInvisibility_67D539);
EXTERN_GLOBAL(bool, gCheatUnlimitedDoubleDamage_67D57C);

DEFINE_GLOBAL_INIT(Fix16, dword_6FE610, Fix16(0), 0x6FE610);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE618, Fix16(2), 0x6FE618);
DEFINE_GLOBAL_INIT(Fix16, dword_6FE41C, dword_6FE610, 0x6FE41C);
DEFINE_GLOBAL_INIT(Ang16, word_6FE754, Ang16(0), 0x6FE754);

MATCH_FUNC(0x4881E0)
u8 Player::GetIdx_4881E0()
{
    return field_2E_idx;
}

STUB_FUNC(0x5645B0)
void Player::sub_5645B0(Car_BC* a2)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x564610)
char_type Player::sub_564610(Car_BC* a2, char_type a3)
{
    NOT_IMPLEMENTED;
    return 'a';
}

STUB_FUNC(0x564680)
u32* Player::sub_564680(s32 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x564710)
void Player::sub_564710(Car_BC* pCar, s32 weapon_kind)
{
    this->field_18 = this->field_788_curr_weapon_idx;

    Weapon_30* pWeapon = gWeapon_8_707018->find_5E3D20(pCar, weapon_kind);
    if (pWeapon)
    {
        this->field_1A_ammo = pWeapon->field_0_ammo;
    }
    else
    {
        this->field_1A_ammo = 0;
        pWeapon = gWeapon_8_707018->allocate_5E3CE0(weapon_kind, pCar, 0);
    }

    this->field_1C_weapon_kind = weapon_kind;
    this->field_20_car = pCar;
    this->field_24 = pCar->field_6C_maybe_id;

    pWeapon->field_0_ammo = -1;

    Ped* pDriver = pCar->field_54_driver;
    if (pDriver)
    {
        if (pDriver->field_15C_player == this)
        {
            this->field_718_weapons[weapon_kind] = pWeapon;
            this->field_788_curr_weapon_idx = this->field_1C_weapon_kind;
        }
    }
}

MATCH_FUNC(0x564790)
void Player::sub_564790(s32 idx)
{
    this->field_18 = this->field_788_curr_weapon_idx;
    this->field_1C_weapon_kind = idx;
    this->field_1A_ammo = this->field_718_weapons[idx]->field_0_ammo;
    this->field_718_weapons[idx]->field_0_ammo = -1;
    this->field_788_curr_weapon_idx = this->field_1C_weapon_kind;
    EnableKFMode_56A010();
}

STUB_FUNC(0x5647D0)
void Player::ClearKFWeapon_5647D0()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x5648F0)
Weapon_30* Player::GetCurrPlayerWeapon_5648F0()
{
    return field_788_curr_weapon_idx == -1 ? NULL : field_718_weapons[field_788_curr_weapon_idx];
}

MATCH_FUNC(0x564910)
void Player::sub_564910(Weapon_30* a2)
{
    s16 idx = a2->field_1C_idx;
    this->field_718_weapons[idx] = a2;
    if (!this->field_31_kf_weapon_mode)
    {
        this->field_788_curr_weapon_idx = idx;
    }
}

MATCH_FUNC(0x564940)
char_type Player::HasAnyAmmo_564940()
{
    for (s32 i = 0; i < 15; i++)
    {
        if (field_718_weapons[i]->field_0_ammo)
        {
            return 1;
        }
    }
    return 0;
}

MATCH_FUNC(0x564960)
char_type Player::sub_564960(s32 weapon_kind, u8 ammo)
{
    Ped* pPed;
    bool bHasAnyAmmo;
    char bAmmoAdded;

    if (this->field_68 == 2)
    {
        pPed = this->field_2C8_unkq;
    }
    else
    {
        pPed = this->field_2C4_player_ped;
    }

    if (pPed->field_16C_car)
    {
        bHasAnyAmmo = 0;
    }
    else
    {
        bHasAnyAmmo = HasAnyAmmo_564940() == 0;
    }

    bAmmoAdded = field_718_weapons[weapon_kind]->add_ammo_capped_5DCE40(ammo);
    if (!this->field_31_kf_weapon_mode)
    {
        if (bAmmoAdded)
        {
            if (bHasAnyAmmo)
            {
                this->field_788_curr_weapon_idx = weapon_kind;
            }
        }
    }

    return bAmmoAdded;
}

STUB_FUNC(0x5649D0)
void Player::SelectNextOrPrevWeapon_5649D0(char_type bFowards, char_type bBackwards)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x564AD0)
void Player::sub_564AD0(Car_BC* a2)
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x564B60)
void Player::sub_564B60()
{
    for (u32 i = 15; i < 28; i++)
    {
        field_718_weapons[i] = 0;
    }
}

STUB_FUNC(0x564B80)
s32 Player::sub_564B80()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x564C00)
void Player::sub_564C00()
{
    sub_564B80();

    if (field_788_curr_weapon_idx >= 15)
    {
        field_16 = field_788_curr_weapon_idx;
        field_788_curr_weapon_idx = field_14;
    }

    if (field_18 >= 15)
    {
        field_16 = field_18;
        field_18 = field_14;
    }

    SelectNextOrPrevWeapon_5649D0(0, 0);
}

MATCH_FUNC(0x564C50)
void Player::RemovePlayerWeapons_564C50()
{
    s32 i = 15;
    Weapon_30* pWeapon = this->field_718_weapons[0];
    do
    {
        if ((!gCheatUnlimitedElectroGun_67D4F7 || pWeapon->field_1C_idx != weapon_type::shocker) &&
            (!gCheatUnlimitedFlameThrower_67D6CC || pWeapon->field_1C_idx != weapon_type::flamethrower))
        {
            if (pWeapon->field_0_ammo)
            {
                pWeapon->add_ammo_5DCE20(0);
            }
        }
        ++pWeapon;
        --i;
    } while (i);

    s16 idx = this->field_788_curr_weapon_idx;
    if (idx < 15)
    {
        this->field_14 = idx;
        this->field_788_curr_weapon_idx = 0;
    }
    SelectNextOrPrevWeapon_5649D0(0, 0);
}

MATCH_FUNC(0x564CC0)
void Player::sub_564CC0()
{
    for (s32 i = 0; i < GTA2_COUNTOF_S(field_6F4); i++)
    {
        if (i == 11 && gCheatInvisibility_67D539)
        {
            continue;
        }

        if (i == 7 && gCheatUnlimitedDoubleDamage_67D57C)
        {
            continue;
        }

        field_6F4[i] = 0;
    }
}

MATCH_FUNC(0x564CF0)
void Player::sub_564CF0()
{
    u16 v2 = this->field_6F4[4];
    if (this->field_6F4[6])
    {
        field_2C4_player_ped->sub_45C050();
    }
    if (this->field_6F4[9])
    {
        this->field_2C4_player_ped->field_21C &= ~ped_bit_status_enum::k_ped_0x04000000;
    }
    if (this->field_6F4[11])
    {
        if (!gCheatInvisibility_67D539)
        {
            field_2C4_player_ped->SetVisible();
        }
    }
    sub_564CC0();
    this->field_6F4[4] = v2;
}

STUB_FUNC(0x564D60)
char_type Player::sub_564D60(s32 a2)
{
    NOT_IMPLEMENTED;
    return 'a';
}

MATCH_FUNC(0x565070)
void Player::tick_down_powerups_565070()
{
    // invulnerability
    this->field_6F4[6];
    if (field_6F4[6])
    {
        this->field_6F4[6]--;
        if (!field_6F4[6])
        {
            field_2C4_player_ped->sub_45C050();
        }
    }

    // double damage
    if (this->field_6F4[7])
    {
        if (!gCheatUnlimitedDoubleDamage_67D57C)
        {
            this->field_6F4[7]--;
        }
    }

    if (this->field_6F4[8])
    {
        this->field_6F4[8]--;
    }

    if (field_6F4[9])
    {
        this->field_6F4[9]--;
        if (!field_6F4[9])
        {
            this->field_2C4_player_ped->field_21C &= ~ped_bit_status_enum::k_ped_0x04000000;
        }
    }

    // invisiblity
    if (this->field_6F4[11])
    {
        if (!gCheatInvisibility_67D539)
        {
            this->field_6F4[11]--;
            if (!this->field_6F4[11])
            {
                field_2C4_player_ped->SetVisible();
            }
        }
    }
}

STUB_FUNC(0x5651F0)
s32 Player::sub_5651F0(s32 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x565310)
void Player::TeleportToDebugCam_565310()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x5653E0)
void Player::sub_5653E0()
{
    Car_BC* pCar = gCar_6C_677930->GetNearestCarFromCoord_444FA0(field_14C_view_camera.field_98_cam_pos2.field_0_x,
                                                                 field_14C_view_camera.field_98_cam_pos2.field_4_y,
                                                                 field_14C_view_camera.field_98_cam_pos2.field_8_z,
                                                                 0);
    if (pCar)
    {
        sub_5695A0();
        sub_5696D0(pCar);
    }
}

MATCH_FUNC(0x565460)
void Player::sub_565460()
{
    // Empty
}

STUB_FUNC(0x565490)
s32 Player::sub_565490(Ped* pPed)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x565740)
void Player::SetInputs_565740(u32 input)
{
    field_4_inputs = input;
}

STUB_FUNC(0x565770)
void Player::IncrementGangRespectFromDebugKeys_565770(u8 count)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x565860)
void Player::IncreaseWantedLevelFromDebugKeys_565860()
{
    NOT_IMPLEMENTED;
    Ped* pPed = this->field_2C4_player_ped;
    switch (pPed->get_wanted_star_count_46EF00())
    {
        case 0u:
            pPed->set_wanted_level_46EF40(600u);
            break;
        case 1u:
            pPed->set_wanted_level_46EF40(1600u);
            break;
        case 2u:
            pPed->set_wanted_level_46EF40(3000u);
            break;
        case 3u:
            pPed->set_wanted_level_46EF40(5000u);
            break;
        case 4u:
            pPed->set_wanted_level_46EF40(8000u);
            break;
        case 5u:
            pPed->set_wanted_level_46EF40(12000u);
            break;
        case 6u:
            pPed->set_wanted_level_46EF40(0);
            break;
        default:
            break;
    }

    if (pPed->get_wanted_star_count_46EF00() > gPolice_7B8_6FEE40->field_660_wanted_star_count)
    {
        pPed->set_wanted_star_count_46F070(gPolice_7B8_6FEE40->field_660_wanted_star_count);
    }
}

STUB_FUNC(0x565890)
void Player::Hud_Controls_565890(u16 action)
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x566380)
void Player::sub_566380(u16 key)
{
    switch (key)
    {
        case DIK_NEXT:
            if (bDo_debug_keys_67D6CF)
            {
                this->field_76 = 0;
            }
            break;

        case DIK_PRIOR:
            if (bDo_debug_keys_67D6CF)
            {
                this->field_77 = 0;
            }
            break;
        case DIK_NUMPAD2:
            this->field_71 = 0;
            break;

        case DIK_NUMPAD8:
            this->field_70_dbg_cam_north = 0;
            break;

        case DIK_NUMPAD4:
            this->field_72 = 0;
            break;

        case DIK_NUMPAD6:
            this->field_73 = 0;
            break;

        case DIK_NUMPAD7:
            this->field_74_dbg_cam_zooming_out = 0;
            break;

        case DIK_NUMPAD9:
            this->field_75_dbg_cam_zooming_in = 0;
            break;

        default:
            return;
    }
}

MATCH_FUNC(0x566520)
void Player::CharacterControls_566520()
{
    int input = this->field_4_inputs;

    // up arrow 0x1
    bool forwardGas = (input & 1) == 1;
    bool same = (forwardGas == this->field_78_bNowForwardPressed);
    this->field_8B_bWasForwardPressed = !same;
    this->field_78_bNowForwardPressed = forwardGas;

    // down arrow 0x2
    bool footBrake = (input & 2) == 2;
    same = (footBrake == this->field_79_bNowDownPressed);
    this->field_8C_bWasDownPressed = !same;
    this->field_79_bNowDownPressed = footBrake;

    // left arrow 0x4
    // right arrow 0x8
    this->field_7A_bNowLeftPressed = (input & 4) == 4;
    this->field_7B_bNowRightPressed = (input & 8) == 8;

    // ctrl 0x10 (attack)
    const bool ctrl10 = (input & 0x10) == 0x10;
    same = (ctrl10 == this->field_7C_bNowAttackPressed);
    this->field_8D_bWasAttackPressed = !same;
    this->field_7C_bNowAttackPressed = ctrl10;

    // space 0x40 (handbrake, jump)
    const bool prevHandBrake = this->field_7E_bNowHandBrakeOrJumpPressed;
    const bool handBrake = (input & 0x40) == 0x40;
    this->field_7E_bNowHandBrakeOrJumpPressed = handBrake;
    this->field_8A_bWasHandBrakeOrJumpPressed = (handBrake != prevHandBrake);

    // enter 0x20 (enter/exit car)
    const bool prev20 = this->field_7D_bNowEnterExitPressed;
    const bool ctrl20 = (input & 0x20) == 0x20;
    this->field_7D_bNowEnterExitPressed = ctrl20;
    this->field_89_bWasEnterExitPressed = (ctrl20 != prev20);

    // y 0x80 (prev weapon)
    const bool prev80 = this->field_7F_bNowPrevWeaponPressed;
    const bool ctrl80 = (input & 0x80) == 0x80;
    this->field_7F_bNowPrevWeaponPressed = ctrl80;
    this->field_88_bWasPrevWeaponPressed = (ctrl80 != prev80);

    // x 0x100 (next weapon)
    const bool prev100 = this->field_80_bNowNextWeaponPressed;
    this->field_80_bNowNextWeaponPressed = (input & 0x100) == 0x100;
    this->field_87_bWasNextWeaponPressed = (field_80_bNowNextWeaponPressed != prev100);

    // tab 0x200  (hold for turret movement, horn, siren, burp, fart on press)
    const bool prev200 = this->field_81_bNowSpecial_1_Pressed;
    bool ctrl200 = (input & 0x200) == 0x200;
    this->field_81_bNowSpecial_1_Pressed = ctrl200;
    this->field_84_bWasSpecial_1_Pressed = (ctrl200 != prev200);

    // alt 0x400 (special2, hold for camera pan)
    const bool ctrl400 = (input & 0x400) == 0x400;
    same = (ctrl400 == this->field_82_bNowSpecial_2_Pressed);
    this->field_85_bWasSpecial_2_Pressed = !same;
    this->field_82_bNowSpecial_2_Pressed = ctrl400;

    // right shift 0x800 (special 3??)
    const bool prev800 = this->field_83_bNowSpecial_3_Pressed;
    bool ctrl800 = (input & 0x800) == 0x800;
    this->field_83_bNowSpecial_3_Pressed = ctrl800;
    this->field_86_bWasSpecial_3_Pressed = (ctrl800 != prev800);
}

MATCH_FUNC(0x566820)
void Player::ControlInputs_566820()
{
    CharacterControls_566520();

    if (field_4_inputs)
    {
        const u16 dxKey = (field_4_inputs >> 12) & 0x1FF;
        if (dxKey != 0)
        {
            if ((field_4_inputs & 0x200000) != 0)
            {
                Hud_Controls_565890(dxKey);
            }
            else
            {
                sub_566380(dxKey);
            }
        }
        field_4_inputs = 0;
    }
}

MATCH_FUNC(0x5668D0)
void Player::HandleControls_5668D0(Ped* pPed)
{
    bool bNoPed = (pPed == NULL);
    Car_BC* pPedCar = pPed->field_16C_car;

    if (bNoPed)
    {
        return;
    }
    if (pPed->field_278 == 9)
    {
        return;
    }
    if (field_7D_bNowEnterExitPressed && field_89_bWasEnterExitPressed)
    {
        if (!pPedCar)
        {
            if (pPed->field_258_objective != objectives_enum::leave_car_36)
            {
                Car_BC* pCar = gCar_6C_677930->sub_445210(pPed->field_168_game_object->field_80_sprite_ptr, 3u);
                if (pCar)
                {
                    pPed->sub_463830(0, 9999);

                    if (pCar->is_train_model())
                    {
                        pPed->SetObjective(objectives_enum::objective_37, 9999);
                    }
                    else
                    {
                        pPed->SetObjective(objectives_enum::enter_car_as_driver_35, 9999);
                    }

                    pPed->field_150_target_objective_car = pCar;
                    pPed->field_248_enter_car_as_passenger = 0;
                    pPed->field_24C_target_car_door = 0;
                }
            }
        }
        else
        {
            if (pPedCar->CanExitCar_43AF10())
            {
                pPed->sub_463830(0, 9999);
                if (pPed->field_16C_car->field_84_car_info_idx == car_model_enum::TRAIN)
                {
                    pPed->SetObjective(objectives_enum::objective_38, 9999);
                }
                else
                {
                    pPed->SetObjective(objectives_enum::leave_car_36, 9999);
                }
                pPed->field_150_target_objective_car = pPed->field_16C_car;
            }
        }
    }
    else
    {
        s32 objective = pPed->field_258_objective;
        if (objective == objectives_enum::enter_car_as_driver_35 || objective == objectives_enum::objective_37)
        {
            if (pPed->field_278 != 10)
            {
                if (pPed->field_225 == 2 || !pPed->field_150_target_objective_car)
                {
                    pPed->SetObjective(objectives_enum::no_obj_0, 9999);
                    pPed->sub_463830(0, 9999);
                    pPed->field_168_game_object->field_38_velocity = dword_6FE41C;
                }
            }
            else
            {
                if (pPed->field_225)
                {
                    pPed->SetObjective(objectives_enum::no_obj_0, 9999);
                    pPed->sub_463830(0, 9999);
                    pPed->sub_45C500(10);
                    pPed->sub_45C540(10);
                }
            }
        }
    }

    if (field_7C_bNowAttackPressed != 1 || field_28)
    {
        pPed->field_21C_bf.b11 = 0;
    }
    else
    {
        if (!field_8D_bWasAttackPressed)
        {
            if (field_788_curr_weapon_idx == -1)
            {
                pPed->field_21C_bf.b11 = 1;
            }
            else
            {
                bNoPed = field_718_weapons[field_788_curr_weapon_idx]->sub_5E33C0() == 0;
                if (!bNoPed)
                {
                    pPed->field_21C_bf.b11 = 1;
                }
                else
                {
                    pPed->field_21C_bf.b11 = 0;
                }
            }
        }
        else
        {
            pPed->field_21C_bf.b11 = 1;
        }
    }

    if (pPedCar)
    {
        if (pPed->field_248_enter_car_as_passenger != 1)
        {
            if (pPedCar->is_train_model())
            {
                Player::DoPedControlInputs_566C80(pPed);
            }
            else
            {
                Player::DoCarControlInputs_566C30(pPedCar);
            }
        }
    }
    else
    {
        Player::DoPedControlInputs_566C80(pPed);
    }
}

MATCH_FUNC(0x566C30)
void Player::DoCarControlInputs_566C30(Car_BC* pCar)
{
    char bAttackPressed;

    if (this->field_788_curr_weapon_idx >= 15)
    {
        // Not a car weapon
        bAttackPressed = 0;
    }
    else
    {
        bAttackPressed = this->field_7C_bNowAttackPressed;
    }

    pCar->HandleUserInput_4418D0(this->field_78_bNowForwardPressed,
                     this->field_79_bNowDownPressed,
                     this->field_7A_bNowLeftPressed,
                     this->field_7B_bNowRightPressed,
                     this->field_7E_bNowHandBrakeOrJumpPressed,
                     this->field_81_bNowSpecial_1_Pressed,
                     this->field_84_bWasSpecial_1_Pressed,
                     bAttackPressed);
}

STUB_FUNC(0x566C80)
char_type Player::DoPedControlInputs_566C80(Ped* a2)
{
    NOT_IMPLEMENTED;
    return 'a';
}

MATCH_FUNC(0x566EE0)
void Player::sub_566EE0(char_type bDoNothing)
{
    if (!bDoNothing)
    {
        Ped* pPed = Get_Field_68_Ped();
        Car_BC* pCar = pPed->field_16C_car;

        if (pCar)
        {
            CarPhysics_B0* pPhysics = pCar->field_58_physics;
            if (pPhysics)
            {
                pPhysics->ShowPhysicsDebug_559430();
                pCar->field_58_physics->sub_5597B0();
            }
        }

        if (bDo_show_camera_67D58A)
        {
            // fmuls vs fmull
            double x = this->field_90_game_camera.field_98_cam_pos2.field_0_x.AsDouble();
            double y = this->field_90_game_camera.field_98_cam_pos2.field_4_y.AsDouble();
            double z = this->field_90_game_camera.field_98_cam_pos2.field_8_z.AsDouble();
            swprintf(tmpBuff_67BD9C, L"game camera: (%3.3f,%3.3f,%3.3f)", x, y, z);
            gHud_2B00_706620->field_650.sub_5D1F50(tmpBuff_67BD9C, 0, 64, word_706600, 1);

            swprintf(tmpBuff_67BD9C,
                     L"aux game camera: (%3.3f,%3.3f,%3.3f)",
                     this->field_208_aux_game_camera.field_98_cam_pos2.field_0_x.AsDouble(),
                     this->field_208_aux_game_camera.field_98_cam_pos2.field_4_y.AsDouble(),
                     this->field_208_aux_game_camera.field_98_cam_pos2.field_8_z.AsDouble());
            gHud_2B00_706620->field_650.sub_5D1F50(tmpBuff_67BD9C, 0, 80, word_706600, 1);

            swprintf(tmpBuff_67BD9C,
                     L"view camera: (%3.3f,%3.3f,%3.3f)",
                     this->field_14C_view_camera.field_98_cam_pos2.field_0_x.AsDouble(),
                     this->field_14C_view_camera.field_98_cam_pos2.field_4_y.AsDouble(),
                     this->field_14C_view_camera.field_98_cam_pos2.field_8_z.AsDouble());
            gHud_2B00_706620->field_650.sub_5D1F50(tmpBuff_67BD9C, 0, 96, word_706600, 1);
        }

        if (gShow_cycle_67D6BD)
        {
            rng_dword_67AB34->ShowCycle_48B920();
        }

        if (gDo_show_input_67D576)
        {
            gBurgerKing_67F8B0.ShowInput_4CEE10(); // input
        }

        gGame_0x40_67E008->sub_4B9270();
    }
}

MATCH_FUNC(0x5670B0)
void Player::RespawnPlayer_5670B0()
{
    if (!(u8)bStartNetworkGame_7081F0 && !field_640_busted)
    {
        Player::ChangeLifeCountByAmount_5699F0(-1);
    }
    if (field_684_lives.field_0 > 0)
    {
        gmp_map_zone* pZone;
        if ((u8)bStartNetworkGame_7081F0)
        {
            pZone = gMap_0x370_6F6268->zone_by_type_bounded_4DF0F0(0x10u);
        }
        else
        {
            s32 y_pos = field_2C4_player_ped->field_1AC_cam.y.ToInt();
            s32 x_pos = field_2C4_player_ped->field_1AC_cam.x.ToInt();
            pZone = gMap_0x370_6F6268->GetNearestZoneOfType_4DF240(x_pos, y_pos, Restart_16);
        }
        field_2C4_player_ped->RespawnPed_45C350(pZone);
    }
}

MATCH_FUNC(0x567130)
void Player::Wasted_567130()
{
    Ped* pPed_killer;
    Player* player_killer; //  this function seems to handle the player death

    if (bStartNetworkGame_7081F0)
    {
        // Ped: field_204 = the ID of the ped who killed this player/Player
        if (field_2C4_player_ped->field_204 == 0)
        {
            player_killer = NULL;
        }
        else if ((pPed_killer = gPedManager_6787BC->PedById(field_2C4_player_ped->field_204)) == 0)
        {
            player_killer = NULL;
        }
        else if (pPed_killer->sub_45EDE0(2) == 0)
        {
            player_killer = NULL;
        }
        else if ((player_killer = pPed_killer->field_15C_player) == NULL)
        {
            player_killer = NULL;
        }

        (&gYouthful_einstein_6F8450)
            ->sub_516740( //  tag mode death handler
                gGame_0x40_67E008->field_4_players[field_2E_idx],
                player_killer); //  if player_killer != NULL then 'player_killer' now is "IT"
    }

    field_29 = 0;
    field_2C4_player_ped->field_21C &= ~0x800u; //  TODO: BitSet32
    if (field_28 == 0)
    {
        if (field_0)
        {
            if (field_684_lives.field_0 > 1 || bStartNetworkGame_7081F0)
            {
                gHud_2B00_706620->field_111C.ShowMessage_5D1A00( //  It's really ugly, it's probably inlined
                    gText_0x14_704DFC->Find_5B5F90(GetDeathText_569F00()),
                    1);
                gRoot_sound_66B038.PlayVoice_40F090(29);
            }
            else
            {
                gHud_2B00_706620->field_111C.ShowMessage_5D1A00(gText_0x14_704DFC->Find_5B5F90("g_over"), 3);
                gRoot_sound_66B038.PlayVoice_40F090(21);
            }
        }
        field_44_death_type = 0;
        if (field_2D0)
        {
            Player::sub_5695A0();
        }
        field_28 = 1;
        field_2C = 70;
        if (field_684_lives.field_0 > 1)
        {
            field_2C8_unkq = gPedManager_6787BC->sub_470F90(field_2C4_player_ped);
            field_2C8_unkq->field_170_selected_weapon = 0;
            field_2C8_unkq->field_200_id = 0;
            field_2C8_unkq->field_21C &= ~0x800u; //  TODO: BitSet32
            field_2C8_unkq->field_267_varrok_idx = 0;
            field_68 = 2;
            memcpy(&field_208_aux_game_camera, &field_90_game_camera, sizeof(field_208_aux_game_camera));
            field_2D0 = 1;
        }
        Player::RespawnPlayer_5670B0();
    }
    else
    {
        if (field_2C8_unkq)
        {
            field_2C8_unkq->field_21C &= ~0x800u; //  TODO: BitSet32
        }

        if (field_2C == 0)
        {
            if (field_684_lives.field_0 > 0 || bStartNetworkGame_7081F0)
            {
                Player::ClearKFWeapon_5647D0();
                if (!bKeep_weapons_after_death_67D54D)
                {
                    Player::sub_564C00();
                    Player::RemovePlayerWeapons_564C50(); //  remove weapons from dead player
                    Player::sub_564CF0();
                }
                field_68 = 0;
                field_90_game_camera.sub_435DD0();
                field_90_game_camera.inline_sub_475B60();

                if (field_2C8_unkq != NULL)
                {
                    field_2C8_unkq->Deallocate_45EB60();
                    field_2C8_unkq = NULL;
                    field_2D0 = 0;
                }
                field_2C4_player_ped->sub_45C410();
                field_28 = 0;
            }
            else
            {
                gGame_0x40_67E008->sub_4B8C00(0, 3);
            }
        }
        else
        {
            field_2C--;
        }
    }
}

MATCH_FUNC(0x567850)
void Player::sub_567850()
{
    if (field_29 == 0)
    {
        field_29 = 1;
        field_2C = 70;
        return;
    }

    if (--field_2C == 0)
    {
        field_29 = 0;
        sub_5695A0();
    }
}

STUB_FUNC(0x5679E0)
void Player::Busted_5679E0()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x568520)
void Player::UpdateCurrentZones_568520()
{
    const Ped* pPed = field_68 == 2 ? field_2C8_unkq : field_2C4_player_ped;
    const Fix16 cam_x_fp = pPed->field_1AC_cam.x;
    const Fix16 cam_y_fp = pPed->field_1AC_cam.y;
    field_38_local_navigation_zone =
        gMap_0x370_6F6268->zone_by_pos_and_type_4DF4D0(cam_x_fp.ToInt(), cam_y_fp.ToInt(), Local_Navigation_15);
    field_3C_navigation_zone = gMap_0x370_6F6268->zone_by_pos_and_type_4DF4D0(cam_x_fp.ToInt(), cam_y_fp.ToInt(), Navigation_1);
    field_40_arrow_blocker_zone = gMap_0x370_6F6268->zone_by_pos_and_type_4DF4D0(cam_x_fp.ToInt(), cam_y_fp.ToInt(), Arrow_Blocker_5);
    field_34_gang_curr_location = gMap_0x370_6F6268->GetGangAtCoords_4DFB50(cam_x_fp, cam_y_fp);
}

MATCH_FUNC(0x568630)
void Player::sub_568630()
{
    infallible_turing* pCam = field_4C_pUnk;
    if (pCam)
    {
        pCam->field_C_pAny = &field_14C_view_camera;
    }
}

MATCH_FUNC(0x568670)
void Player::sub_568670()
{
    if (this->field_76)
    {
        field_90_game_camera.sub_435810();
        field_208_aux_game_camera.sub_435810();
    }

    if (this->field_77)
    {
        field_90_game_camera.sub_4357F0();
        field_208_aux_game_camera.sub_4357F0();
    }
}

MATCH_FUNC(0x5686D0)
void Player::UpdateCamera_5686D0(Camera_0xBC* pCam)
{
    // Camera panning
    if (this->field_82_bNowSpecial_2_Pressed)
    {
        pCam->HandlePanning_436710(this->field_78_bNowForwardPressed, this->field_79_bNowDownPressed, this->field_7A_bNowLeftPressed, this->field_7B_bNowRightPressed);
        this->field_7A_bNowLeftPressed = 0;
        this->field_7B_bNowRightPressed = 0;
        this->field_78_bNowForwardPressed = 0;
        this->field_79_bNowDownPressed = 0;
        this->field_8B_bWasForwardPressed = 0;
        this->field_8C_bWasDownPressed = 0;
    }
    else
    {
        pCam->sub_436830();
    }
}

MATCH_FUNC(0x568730)
void Player::sub_568730()
{
    Ped* pPlayerPed = field_2C4_player_ped;
    if (pPlayerPed)
    {
        pPlayerPed->sub_470300();
    }
    Ped* pPed = field_2C8_unkq;
    if (pPed)
    {
        pPed->sub_470300();
    }
    field_8E_bInUse = 0;
}

STUB_FUNC(0x5687F0)
void Player::Service_5687F0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x569410)
s32 Player::sub_569410()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x569530)
void Player::sub_569530()
{
    Car_BC* pCar = field_2C8_unkq->field_16C_car;
    if (pCar->field_54_driver)
    {
        pCar->sub_4407F0();
    }

    if (pCar->field_98 != 4)
    {
        pCar->field_98 = 3;
    }

    field_2C8_unkq->Kill_46F9D0();
    field_2C8_unkq = 0;

    sub_564C00();

    if (field_2C4_player_ped->field_16C_car)
    {
        sub_564AD0(field_2C4_player_ped->field_16C_car);
    }
}

MATCH_FUNC(0x5695A0)
void Player::sub_5695A0()
{
    if (!field_28)
    {
        field_68 = 0;
        if (field_2D0)
        {
            Ped* pPed = field_2C8_unkq;
            if (pPed)
            {
                if (pPed->field_240_occupation == 1)
                {
                    sub_569530();
                }
            }
        }
        field_2C8_unkq = 0;
        field_2CC = 0;
        field_2D0 = 0;
        field_90_game_camera.field_3C_followed_ped_id = 1;
    }
}

STUB_FUNC(0x569600)
char_type Player::sub_569600(Car_BC* a2)
{
    NOT_IMPLEMENTED;
    return 'a';
}

STUB_FUNC(0x5696D0)
void Player::sub_5696D0(Car_BC* pCar)
{
    NOT_IMPLEMENTED;
    if (!field_2D0 && !field_2C8_unkq && !field_2CC)
    {
        field_2CC = pCar;
        field_208_aux_game_camera.sub_4364A0(pCar);
        field_208_aux_game_camera.field_0_cam_pos_tgt1 = field_208_aux_game_camera.field_10_cam_pos_tgt2;
        field_208_aux_game_camera.sub_435DD0();
        field_68 = 3;
        field_2D0 = 1;
    }
}

MATCH_FUNC(0x569840)
void Player::sub_569840(u8& x, u8& y, u8& z)
{
    Ped* pPed;

    if (field_68 == 2 || field_68 == 3)
    {
        pPed = field_2C8_unkq;
    }
    else
    {
        pPed = field_2C4_player_ped;
    }
    if (pPed)
    {
        x = pPed->get_cam_x().ToUInt8();
        y = pPed->get_cam_y().ToUInt8();
        z = pPed->get_cam_z().ToUInt8();
    }
    else
    {
        Camera_0xBC* pCam;
        if (field_68 == 2 || field_68 == 3)
        {
            pCam = &field_208_aux_game_camera;
        }
        else
        {
            pCam = &field_90_game_camera;
        }
        x = pCam->field_98_cam_pos2.field_0_x.ToUInt8();
        y = pCam->field_98_cam_pos2.field_4_y.ToUInt8();
        z = (dword_6FE618 + pCam->field_98_cam_pos2.field_8_z).ToUInt8();
    }
}

MATCH_FUNC(0x5698E0)
Car_BC* Player::sub_5698E0()
{
    Ped* pPed;
    if (field_68 == 2 || field_68 == 3)
    {
        pPed = this->field_2C8_unkq;
    }
    else
    {
        pPed = this->field_2C4_player_ped;
    }

    if (pPed)
    {
        Car_BC* pCar = pPed->field_16C_car;
        if (!pCar)
        {
            pCar = pPed->sub_45BBF0();
        }
        return pCar;
    }
    return 0;
}

MATCH_FUNC(0x569920)
void Player::get_pos_569920(Fix16* pXPos, Fix16* pYPos, Fix16* pZPos)
{
    Ped* pPed = (field_68 == 2 || field_68 == 3) ? field_2C8_unkq : field_2C4_player_ped;
    if (pPed)
    {
        Car_BC* pCar = pPed->sub_45BBF0();
        if (pCar)
        {
            *pXPos = pCar->field_50_car_sprite->GetXPos();
            *pYPos = pCar->field_50_car_sprite->GetYPos();
            *pZPos = pCar->field_50_car_sprite->GetZPos();
        }
        else
        {
            *pXPos = pPed->get_cam_x();
            *pYPos = pPed->get_cam_y();
            *pZPos = pPed->get_cam_z();
        }
    }
    else
    {
        Camera_0xBC* pCam = (field_68 == 2 || field_68 == 3) ? &field_208_aux_game_camera : &field_90_game_camera;
        *pXPos = pCam->field_98_cam_pos2.field_0_x;
        *pYPos = pCam->field_98_cam_pos2.field_4_y;
        *pZPos = pCam->field_98_cam_pos2.field_8_z + dword_6FE618;
    }
}

MATCH_FUNC(0x5699F0)
void Player::ChangeLifeCountByAmount_5699F0(s32 amount)
{
    if (!gDo_infinite_lives_67D4C9 || amount > 0)
    {
        field_684_lives.ChangeStatByAmount_4921B0(amount);
    }
}

STUB_FUNC(0x569A10)
void Player::sub_569A10()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x569C20)
void Player::sub_569C20()
{
    if (bStartNetworkGame_7081F0 == false && gfrosty_pasteur_6F8060 != NULL)
    {
        if (field_60 == 0)
        {
            u32 score = field_2D4_unk.GetScore_592370();
            if (score >= gfrosty_pasteur_6F8060->field_310_finish_score)
            {
                field_60 = 1;
                field_2D4_unk.sub_592360()->sub_4921F0(palette_types_enum::user_remaps, 6);
            }
        }
        else
        {
            u32 score = field_2D4_unk.GetScore_592370();
            if (score < gfrosty_pasteur_6F8060->field_310_finish_score)
            {
                field_60 = 0;
                field_2D4_unk.sub_592360()->sub_4921F0(palette_types_enum::sprites, 0);
            }
        }
    }
}

MATCH_FUNC(0x569CB0)
void Player::sub_569CB0()
{
    if (!gMap_0x370_6F6268->first_zone_by_type_4DF1D0(Restart_16))
    {
        FatalError_4A38C0(Gta2Error::NoRestartZone, "C:\\Splitting\\Gta2\\Source\\player.cpp",
                          2905); // No Restart Zone
    }
    field_60 = 0;
    field_29 = 0;
    field_28 = 0;
    field_640_busted = 0;
    field_680 = 0;
    field_682 = 1000;
    field_2D4_unk.sub_592330();
    field_684_lives.sub_492150();
    field_6BC_multpliers.sub_492150();
    field_64 = 0;
    field_18 = -2;
    if (gfrosty_pasteur_6F8060->field_C1E2C)
    {
        Player::UpdateGameFromSave_56A310(&gMapName_6F78C8.field_54_save);
    }
    else
    {
        if (!gLucid_hamilton_67E8E0.sub_4C59A0()) // bonus level?
        {
            field_684_lives.ChangeStatByAmount_4921B0(5);
        }
        else
        {
            field_684_lives.ChangeStatByAmount_4921B0(1);
        }
        field_6BC_multpliers.ChangeStatByAmount_4921B0(1);
    }

    field_90_game_camera.sub_436540(field_2C4_player_ped);
    field_90_game_camera.inline_sub_475B60();
    field_90_game_camera.sub_41E410();
    field_90_game_camera.sub_435DD0();

    field_208_aux_game_camera.sub_436540(field_2C4_player_ped);
    field_208_aux_game_camera.inline_sub_475B60();
    field_208_aux_game_camera.sub_41E410();
    field_208_aux_game_camera.sub_435DD0();

    field_2D0 = 0;

    if (field_0)
    {
        if (!bSkip_audio_67D6BE)
        {
            field_4C_pUnk = gRoot_sound_66B038.CreateSoundObject_40EF40(&field_90_game_camera, 5);
        }
    }
    field_78C = 2;
    field_790 = 0;
    if (bStartNetworkGame_7081F0)
    {
        Player::sub_569A10();
        gNetPlay_7071E8.GetPlayerName_521100(&field_83C_player_name, field_2E_idx);
        gText_0x14_704DFC->sub_5B5910(&field_83C_player_name);
        gLucid_hamilton_67E8E0.sub_4C5C30(field_2E_idx, &field_83C_player_name);
    }
}

STUB_FUNC(0x569E70)
void Player::sub_569E70()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x569F00)
char* Player::GetDeathText_569F00()
{
    switch (field_44_death_type)
    {
        case 0:
        case 1:
            return "wasted";
            break;
        case 2:
            return "fried";
            break;
        case 3:
            return "nicked";
            break;
        case 4:
            return "shocked";
            break;
        default:
            return 0;
            break;
    }
}

STUB_FUNC(0x569F40)
s32 Player::sub_569F40()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x569FF0)
s32 Player::DisableAllControls_569FF0()
{
    field_2F_disable_all_controls = 1;
    return sub_569F40();
}

MATCH_FUNC(0x56A000)
void Player::EnableAllControls_56A000()
{
    field_2F_disable_all_controls = 0;
}

MATCH_FUNC(0x56A010)
void Player::EnableKFMode_56A010()
{
    field_31_kf_weapon_mode = 1;
}

MATCH_FUNC(0x56A020)
void Player::DisableKFMode_56A020()
{
    field_31_kf_weapon_mode = 0;
}

MATCH_FUNC(0x56A030)
void Player::DisableEnterVehicles_56A030()
{
    field_30_disable_enter_vehicles = 1;
}

MATCH_FUNC(0x56A040)
void Player::EnableEnterVehicles_56A040()
{
    field_30_disable_enter_vehicles = 0;
}

STUB_FUNC(0x56A0F0)
s32 Player::sub_56A0F0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x56A1A0)
zealous_borg* Player::CopyPlayerDataToSave_56A1A0(s32 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x56A310)
void Player::UpdateGameFromSave_56A310(save_stats_0x90* a2)
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x56A490)
void Player::ApplyCheats_56A490()
{
    if (gCheatGetPlayerPoints_67D4C8)
    {
        if (-field_2D4_unk.field_0_money.field_30 > 200000)
        {
            field_2D4_unk.field_0_money.field_0 = -field_2D4_unk.field_0_money.field_30;
        }
        else
        {
            if (field_2D4_unk.field_0_money.field_30 < 200000)
            {
                field_2D4_unk.field_0_money.field_0 = field_2D4_unk.field_0_money.field_30;
            }
            else
            {
                field_2D4_unk.field_0_money.field_0 = 200000;
            }
        }
    }
    if (gCheatGet10MillionMoney_67D6CE)
    {
        if (-field_2D4_unk.field_0_money.field_30 > 9999999)
        {
            field_2D4_unk.field_0_money.field_0 = -field_2D4_unk.field_0_money.field_30;
        }
        else
        {
            if (field_2D4_unk.field_0_money.field_30 < 9999999)
            {
                field_2D4_unk.field_0_money.field_0 = field_2D4_unk.field_0_money.field_30;
            }
            else
            {
                field_2D4_unk.field_0_money.field_0 = 9999999;
            }
        }
    }
    if (gCheatUnlimitedElectroGun_67D4F7)
    {
        field_718_weapons[weapon_type::shocker]->field_0_ammo = -1;
    }
    if (gCheatUnlimitedFlameThrower_67D6CC)
    {
        field_718_weapons[weapon_type::flamethrower]->field_0_ammo = -1;
    }
    if (gCheatAllGangMaxRespect_67D587)
    {
        for (Gang_144* pIter = gGangPool_CA8_67E274->sub_4BECA0(); pIter; pIter = gGangPool_CA8_67E274->sub_4BECE0())
        {
            pIter->SetRespect_4BEE30(field_2E_idx, 100);
        }
    }
    if (gCheatGet99Lives_67D4F1)
    {
        if (-field_684_lives.field_30 > 99)
        {
            field_684_lives.field_0 = -field_684_lives.field_30;
        }
        else
        {
            if (field_684_lives.field_30 < 99)
            {
                field_684_lives.field_0 = field_684_lives.field_30;
            }
            else
            {
                field_684_lives.field_0 = 99;
            }
        }
    }
    if (gCheat10xMultiplier_67D589)
    {
        field_6BC_multpliers.ChangeStatByAmount_4921B0(9);
    }
    if (gCheatOnlyMuggerPeds_67D5A4)
    {
        gPedManager_6787BC->field_7_make_all_muggers = true;
    }
    if (gCheatUnknown_67D4F6)
    {
        Player::sub_564D60(4);
    }
    if (gCheatInvisibility_67D539)
    {
        Player::sub_564D60(11);
    }
    if (gCheatUnlimitedDoubleDamage_67D57C)
    {
        Player::sub_564D60(7);
    }
    if (byte_67D56B)
    {
        Player::sub_564D60(4);
        Player::sub_564960(1, 50u);
        for (Gang_144* pIter2 = gGangPool_CA8_67E274->sub_4BECA0(); pIter2; pIter2 = gGangPool_CA8_67E274->sub_4BECE0())
        {
            pIter2->SetRespect_4BEE30(field_2E_idx, 80);
        }
    }
}

MATCH_FUNC(0x56A6D0)
void Player::ClearInputs_56A6D0()
{
    field_78_bNowForwardPressed = 0;
    field_79_bNowDownPressed = 0;
    field_7A_bNowLeftPressed = 0;
    field_7B_bNowRightPressed = 0;
    field_7C_bNowAttackPressed = 0;
    field_7D_bNowEnterExitPressed = 0;
    field_7E_bNowHandBrakeOrJumpPressed = 0;
    field_7F_bNowPrevWeaponPressed = 0;
    field_80_bNowNextWeaponPressed = 0;
    field_81_bNowSpecial_1_Pressed = 0;
    field_82_bNowSpecial_2_Pressed = 0;
    field_83_bNowSpecial_3_Pressed = 0;

    field_84_bWasSpecial_1_Pressed = 0;
    field_88_bWasPrevWeaponPressed = 0;
    field_87_bWasNextWeaponPressed = 0;
    field_89_bWasEnterExitPressed = 0;
    field_8D_bWasAttackPressed = 0;
    field_8A_bWasHandBrakeOrJumpPressed = 0;
    field_8B_bWasForwardPressed = 0;
    field_8C_bWasDownPressed = 0;
}

// https://decomp.me/scratch/OMzHk early %ecx load
STUB_FUNC(0x56A740)
Player::Player(u8 player_idx)
{
    field_794_is_chatting = 0;
    field_838_f796_idx = 0;
    field_796_chat_text[0] = 0;
    field_2E_idx = player_idx;
    field_28 = 0;
    field_640_busted = 0;
    field_680 = 0;
    field_682 = 1000;
    field_29 = 0;
    field_4_inputs = 0;
    field_8E_bInUse = 1;

    // early %ecx load here

    field_2C4_player_ped = 0;
    field_2C8_unkq = 0;
    field_2CC = 0;
    field_34_gang_curr_location = 0;
    field_38_local_navigation_zone = 0;
    field_3C_navigation_zone = 0;

    Player::ClearInputs_56A6D0();
    field_8 = word_6FE754;
    field_A = word_6FE754;
    field_C = dword_6FE610;
    field_10 = 0;
    field_788_curr_weapon_idx = 0;
    field_68 = 1;
    field_6C_bIn_debug_cam_mode = 0;
    field_48_bDbg_cam_follow_player = 0;
    field_8F = 0;
    field_2F_disable_all_controls = 0;
    field_30_disable_enter_vehicles = 0;
    field_31_kf_weapon_mode = 0;
    field_70_dbg_cam_north = 0;
    field_71 = 0;
    field_72 = 0;
    field_73 = 0;
    field_75_dbg_cam_zooming_in = 0;
    field_74_dbg_cam_zooming_out = 0;
    field_77 = 0;
    field_76 = 0;
    memset(field_718_weapons, 0, sizeof(field_718_weapons));

    for (s32 i = 0; i < 3; i++)
    {
        field_54_unk[i] = NULL;
    }

    field_0 = 0;
    field_2D0 = 0;
    field_2C = 0;
    field_4C_pUnk = 0;
    field_2E_idx = player_idx;
    field_2D4_unk.sub_5922F0(this, 2, 999999999, 158, 999u);
    field_684_lives.sub_492110(1, 99, 115);
    field_6BC_multpliers.sub_492110(1, 99, 116);
    Player::sub_564CC0();
    sub_4A5180();
    set_death_type_434950(0);
    field_83C_player_name = 0;
    field_78A_show_quit_message = 0;
}

STUB_FUNC(0x56A940)
Player::~Player()
{
    NOT_IMPLEMENTED;
    Car_BC** local_field_54_unk; // ecx

    local_field_54_unk = field_54_unk;
    field_2C4_player_ped = 0;
    field_2C8_unkq = 0;
    field_2CC = 0;
    field_34_gang_curr_location = 0;
    field_38_local_navigation_zone = 0;
    field_3C_navigation_zone = 0;
    local_field_54_unk[0] = 0;
    local_field_54_unk[1] = 0;
    local_field_54_unk[2] = 0;

    if (field_4C_pUnk)
    {
        gRoot_sound_66B038.DestroySoundObj_40FE60(field_4C_pUnk);
        field_4C_pUnk = 0;
    }

    /*
	zealous_borg::dtor_484EE0(&field_644_unk);
	sad_mirzakhani::dtor_431DF0(&field_2D4_unk.field_1A8_unk);
	Camera_0xBC::dtor_4369E0(&field_208_aux_game_camera);
	Camera_0xBC::dtor_4369E0(&field_14C_view_camera);
	Camera_0xBC::dtor_4369E0(&field_90_game_camera);
	*/
}