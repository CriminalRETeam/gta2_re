#include "Player.hpp"
#include "Car_BC.hpp"
#include "Function.hpp"
#include "Game_0x40.hpp"
#include "Garox_2B00.hpp"
#include "Globals.hpp"
#include "Ped.hpp"
#include "Police_7B8.hpp"
#include "Weapon_30.hpp"
#include "Weapon_8.hpp"
#include "debug.hpp"
#include "frosty_pasteur_0xC1EA8.hpp"
#include "infallible_turing.hpp"
#include "map_0x370.hpp"
#include "root_sound.hpp"
#include "text_0x14.hpp"
#include "winmain.hpp"
#include "youthful_einstein.hpp"
#include "zealous_borg.hpp"

EXPORT_VAR extern bool gCheatUnlimitedElectroGun_67D4F7;
EXPORT_VAR extern bool gCheatUnlimitedFlameThrower_67D6CC;
EXPORT_VAR extern bool gCheatInvisibility_67D539;
EXPORT_VAR extern bool gCheatUnlimitedDoubleDamage_67D57C;

MATCH_FUNC(0x4881E0)
u8 Player::GetIdx_4881E0()
{
    return field_2E_idx;
}

STUB_FUNC(0x5645B0)
void Player::sub_5645B0(Car_BC* a2)
{
}

STUB_FUNC(0x564610)
char_type Player::sub_564610(Car_BC* a2, char_type a3)
{
    return 'a';
}

STUB_FUNC(0x564680)
u32* Player::sub_564680(s32 a2)
{
    return 0;
}

MATCH_FUNC(0x564710)
void Player::sub_564710(Car_BC* pCar, s32 weapon_kind)
{
    this->field_18 = this->field_788_idx;

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
            this->field_718[weapon_kind] = pWeapon;
            this->field_788_idx = this->field_1C_weapon_kind;
        }
    }
}

MATCH_FUNC(0x564790)
void Player::sub_564790(s32 idx)
{
    this->field_18 = this->field_788_idx;
    this->field_1C_weapon_kind = idx;
    this->field_1A_ammo = this->field_718[idx]->field_0_ammo;
    this->field_718[idx]->field_0_ammo = -1;
    this->field_788_idx = this->field_1C_weapon_kind;
    sub_56A010();
}

STUB_FUNC(0x5647D0)
void Player::sub_5647D0()
{
}

MATCH_FUNC(0x5648F0)
Weapon_30* Player::sub_5648F0()
{
    return field_788_idx == -1 ? NULL : field_718[field_788_idx];
}

MATCH_FUNC(0x564910)
void Player::sub_564910(Weapon_30* a2)
{
    s16 idx = a2->field_1C_idx;
    this->field_718[idx] = a2;
    if (!this->field_31)
    {
        this->field_788_idx = idx;
    }
}

MATCH_FUNC(0x564940)
char_type Player::HasAnyAmmo_564940()
{
    for (s32 i = 0; i < 15; i++)
    {
        if (field_718[i]->field_0_ammo)
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

    bAmmoAdded = field_718[weapon_kind]->add_ammo_capped_5DCE40(ammo);
    if (!this->field_31)
    {
        if (bAmmoAdded)
        {
            if (bHasAnyAmmo)
            {
                this->field_788_idx = weapon_kind;
            }
        }
    }

    return bAmmoAdded;
}

STUB_FUNC(0x5649D0)
u16 Player::sub_5649D0(char_type a2, char_type a3)
{
    return 0;
}

STUB_FUNC(0x564AD0)
void Player::sub_564AD0(Car_BC* a2)
{
}

MATCH_FUNC(0x564B60)
void Player::sub_564B60()
{
    for (u32 i = 15; i < 28; i++)
    {
        field_718[i] = 0;
    }
}

STUB_FUNC(0x564B80)
s32 Player::sub_564B80()
{
    return 0;
}

MATCH_FUNC(0x564C00)
void Player::sub_564C00()
{
    sub_564B80();

    if (field_788_idx >= 15)
    {
        field_16 = field_788_idx;
        field_788_idx = field_14;
    }

    if (field_18 >= 15)
    {
        field_16 = field_18;
        field_18 = field_14;
    }

    sub_5649D0(0, 0);
}

MATCH_FUNC(0x564C50)
void Player::RemovePlayerWeapons_564C50()
{
    s32 i = 15;
    Weapon_30* pWeapon = this->field_718[0];
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

    s16 idx = this->field_788_idx;
    if (idx < 15)
    {
        this->field_14 = idx;
        this->field_788_idx = 0;
    }
    sub_5649D0(0, 0);
}

EXPORT_VAR u8 byte_67D57C;
GLOBAL(byte_67D57C, 0x67D57C);

EXPORT_VAR u8 byte_67D539;
GLOBAL(byte_67D539, 0x67D539);

MATCH_FUNC(0x564CC0)
void Player::sub_564CC0()
{
    for (s32 i = 0; i < GTA2_COUNTOF_S(field_6F4); i++)
    {
        if (i == 11 && byte_67D539)
        {
            continue;
        }

        if (i == 7 && byte_67D57C)
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
    return 0;
}

STUB_FUNC(0x565310)
void Player::sub_565310()
{
}

MATCH_FUNC(0x5653E0)
void Player::sub_5653E0()
{
    Car_BC* pCar = gCar_6C_677930->sub_444FA0(this->field_14C_view_camera.field_98_x,
                                              this->field_14C_view_camera.field_9C_y,
                                              this->field_14C_view_camera.field_A0_z,
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
    return 0;
}

MATCH_FUNC(0x565740)
void Player::SetInputs_565740(u32 input)
{
    field_4_inputs = input;
}

STUB_FUNC(0x565770)
void Player::sub_565770(u8 count)
{
}

STUB_FUNC(0x565860)
void Player::sub_565860()
{
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
void Player::sub_565890(u16 action)
{
}

STUB_FUNC(0x566380)
void Player::sub_566380(u16 a2)
{
}

STUB_FUNC(0x566520)
char_type Player::sub_566520()
{
    return 'a';
}

STUB_FUNC(0x566820)
void Player::sub_566820()
{
}

STUB_FUNC(0x5668D0)
void Player::sub_5668D0(Ped* a2)
{
}

MATCH_FUNC(0x566C30)
void Player::sub_566C30(Car_BC* pCar)
{
    char bUnknown;

    if (this->field_788_idx >= 15)
    {
        bUnknown = 0;
    }
    else
    {
        bUnknown = this->field_7C;
    }

    pCar->sub_4418D0(this->field_78_bForwardGasOn,
                     this->field_79_bFootBrakeOn,
                     this->field_7A,
                     this->field_7B,
                     this->field_7E_bHandBrakeOn,
                     this->field_81,
                     this->field_84,
                     bUnknown);
}

STUB_FUNC(0x566C80)
char_type Player::sub_566C80(u32* a2)
{
    return 'a';
}

STUB_FUNC(0x566EE0)
void Player::sub_566EE0(char_type a2)
{
}

MATCH_FUNC(0x5670B0)
void Player::sub_5670B0()
{
    const char_type restart_zone_type = 16;

    if (!(u8)bStartNetworkGame_7081F0 && !field_640)
    {
        Player::sub_5699F0(-1);
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
            pZone = gMap_0x370_6F6268->sub_4DF240(x_pos, y_pos, restart_zone_type);
        }
        field_2C4_player_ped->sub_45C350(pZone);
    }
}

MATCH_FUNC(0x567130)
void Player::sub_567130()
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
        else if ((pPed_killer = gChar_C_6787BC->PedById(field_2C4_player_ped->field_204)) == 0)
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
                gGarox_2B00_706620->field_111C.sub_5D1A00( //  It's really ugly, it's probably inlined
                    gText_0x14_704DFC->Find_5B5F90(GetDeathText_569F00()),
                    1);
                gRoot_sound_66B038.sub_40F090(29);
            }
            else
            {
                gGarox_2B00_706620->field_111C.sub_5D1A00(gText_0x14_704DFC->Find_5B5F90("g_over"), 3);
                gRoot_sound_66B038.sub_40F090(21);
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
            field_2C8_unkq = gChar_C_6787BC->sub_470F90(field_2C4_player_ped);
            field_2C8_unkq->field_170_selected_weapon = 0;
            field_2C8_unkq->field_200_id = 0;
            field_2C8_unkq->field_21C &= ~0x800u; //  TODO: BitSet32
            field_2C8_unkq->field_267 = 0;
            field_68 = 2;
            memcpy(&field_208_aux_game_camera, &field_90_game_camera, sizeof(field_208_aux_game_camera));
            field_2D0 = 1;
        }
        Player::sub_5670B0();
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
                Player::sub_5647D0();
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
                    field_2C8_unkq->sub_45EB60();
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
void Player::sub_5679E0()
{
}

MATCH_FUNC(0x568520)
void Player::sub_568520()
{
    const Ped* pPed = field_68 == 2 ? field_2C8_unkq : field_2C4_player_ped;
    const Fix16 cam_x_fp = pPed->field_1AC_cam.x;
    const Fix16 cam_y_fp = pPed->field_1AC_cam.y;
    field_38 = gMap_0x370_6F6268->zone_by_pos_and_type_4DF4D0(cam_x_fp.ToInt(), cam_y_fp.ToInt(), 0xFu);
    field_3C = gMap_0x370_6F6268->zone_by_pos_and_type_4DF4D0(cam_x_fp.ToInt(), cam_y_fp.ToInt(), 1u);
    field_40 = gMap_0x370_6F6268->zone_by_pos_and_type_4DF4D0(cam_x_fp.ToInt(), cam_y_fp.ToInt(), 5u);
    field_34_pObj = gMap_0x370_6F6268->sub_4DFB50(cam_x_fp, cam_y_fp);
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

STUB_FUNC(0x568670)
char_type Player::sub_568670()
{
    return 'a';
}

STUB_FUNC(0x5686D0)
char_type Player::sub_5686D0(DrawUnk_0xBC* a2)
{
    return 'a';
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
void Player::sub_5687F0()
{
}

STUB_FUNC(0x569410)
s32 Player::sub_569410()
{
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

    field_2C8_unkq->sub_46F9D0();
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
        field_90_game_camera.field_3C = 1;
    }
}

STUB_FUNC(0x569600)
char_type Player::sub_569600(Car_BC* a2)
{
    return 'a';
}

STUB_FUNC(0x5696D0)
void Player::sub_5696D0(Car_BC* pCar)
{
    if (!this->field_2D0 && !this->field_2C8_unkq && !this->field_2CC)
    {
        this->field_2CC = pCar;
        this->field_208_aux_game_camera.sub_4364A0(pCar);
        this->field_208_aux_game_camera.field_0 = this->field_208_aux_game_camera.field_10;
        this->field_208_aux_game_camera.field_4_unk = this->field_208_aux_game_camera.field_14;
        this->field_208_aux_game_camera.field_8 = this->field_208_aux_game_camera.field_18;
        this->field_208_aux_game_camera.field_C = this->field_208_aux_game_camera.field_1C;
        this->field_208_aux_game_camera.sub_435DD0();
        this->field_68 = 3;
        this->field_2D0 = 1;
    }
}

STUB_FUNC(0x569840)
s32 Player::sub_569840(u8* a2, u8* a3, u8* a4)
{
    return 0;
}

STUB_FUNC(0x5698E0)
Car_BC* Player::sub_5698E0()
{
    return 0;
}

STUB_FUNC(0x569920)
s32 Player::sub_569920(u32* a2, s32* a3, s32* a4)
{
    return 0;
}

MATCH_FUNC(0x5699F0)
void Player::sub_5699F0(s32 a2)
{
    if (!gDo_infinite_lives_67D4C9 || a2 > 0)
    {
        field_684_lives.sub_4921B0(a2);
    }
}

STUB_FUNC(0x569A10)
void Player::sub_569A10()
{
}

MATCH_FUNC(0x569C20)
void Player::sub_569C20()
{
    if (bStartNetworkGame_7081F0 == false && gfrosty_pasteur_6F8060 != NULL)
    {
        if (field_60 == 0)
        {
            u32 score = field_2D4_unk.sub_592370();
            if (score >= gfrosty_pasteur_6F8060->field_310_finish_score)
            {
                field_60 = 1;
                field_2D4_unk.sub_592360()->sub_4921F0(palette_types_enum::user_remaps, 6);
            }
        }
        else
        {
            u32 score = field_2D4_unk.sub_592370();
            if (score < gfrosty_pasteur_6F8060->field_310_finish_score)
            {
                field_60 = 0;
                field_2D4_unk.sub_592360()->sub_4921F0(palette_types_enum::sprites, 0);
            }
        }
    }
}

STUB_FUNC(0x569CB0)
void Player::sub_569CB0()
{
}

STUB_FUNC(0x569E70)
void Player::sub_569E70()
{
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
    return 0;
}

MATCH_FUNC(0x569FF0)
s32 Player::sub_569FF0()
{
    field_2F = 1;
    return sub_569F40();
}

MATCH_FUNC(0x56A000)
void Player::SetUnknown_56A000()
{
    field_2F = 0;
}

MATCH_FUNC(0x56A010)
void Player::sub_56A010()
{
    field_31 = 1;
}

MATCH_FUNC(0x56A020)
void Player::sub_56A020()
{
    field_31 = 0;
}

MATCH_FUNC(0x56A030)
void Player::sub_56A030()
{
    field_30 = 1;
}

MATCH_FUNC(0x56A040)
void Player::sub_56A040()
{
    field_30 = 0;
}

STUB_FUNC(0x56A0F0)
s32 Player::sub_56A0F0()
{
    return 0;
}

STUB_FUNC(0x56A1A0)
zealous_borg* Player::sub_56A1A0(s32 a2)
{
    return 0;
}

STUB_FUNC(0x56A310)
void Player::sub_56A310(s32 a2)
{
}

STUB_FUNC(0x56A490)
void Player::sub_56A490()
{
}

MATCH_FUNC(0x56A6D0)
void Player::sub_56A6D0()
{
    // sets some car info to 0
    field_78_bForwardGasOn = 0;
    field_79_bFootBrakeOn = 0;
    field_7A = 0;
    field_7B = 0;
    field_7C = 0;
    field_7D = 0;
    field_7E_bHandBrakeOn = 0;
    field_7F = 0;
    field_80 = 0;
    field_81 = 0;
    field_82 = 0;
    field_83 = 0;
    field_84 = 0;
    field_88 = 0;
    field_87 = 0;
    field_89 = 0;
    field_8D = 0;
    field_8A = 0;
    field_8B = 0;
    field_8C = 0;
}

STUB_FUNC(0x56A740)
Player::Player(u8 a2)
{
}

STUB_FUNC(0x56A940)
Player::~Player()
{
    Car_BC** local_field_54_unk; // ecx

    local_field_54_unk = field_54_unk;
    field_2C4_player_ped = 0;
    field_2C8_unkq = 0;
    field_2CC = 0;
    field_34_pObj = 0;
    field_38 = 0;
    field_3C = 0;
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
	DrawUnk_0xBC::dtor_4369E0(&field_208_aux_game_camera);
	DrawUnk_0xBC::dtor_4369E0(&field_14C_view_camera);
	DrawUnk_0xBC::dtor_4369E0(&field_90_game_camera);
	*/
}
