#include "Weapon_30.hpp"
#include "Object_3C.hpp"
#include "Ped.hpp"
#include "Player.hpp"
#include "Shooey_CC.hpp"
#include "Weapon_8.hpp"
#include "char.hpp"
#include "debug.hpp"
#include "enums.hpp"
#include "root_sound.hpp"
#include "sprite.hpp"

// TODO: move
EXTERN_GLOBAL(Shooey_CC*, gShooey_CC_67A4B8);

MATCH_FUNC(0x5DCD10)
Weapon_30::Weapon_30()
{
    field_0_ammo = 0;
    field_24_pPed = 0;
    field_14_car = 0;
    field_2_reload_speed = 0;
    field_4 = 0;
    field_18_pNext = 0;
    field_1C_idx = 0;
    field_10 = 0;
    field_8 = 0;
    field_C = -1;
    field_20 = 0;
    field_21 = 0;
    field_2C = 0;
    field_28_pSound = 0;
}

MATCH_FUNC(0x5DCD50)
Weapon_30::~Weapon_30()
{
    field_24_pPed = 0;
    field_18_pNext = 0;
    field_14_car = 0;
    field_8 = 0;
    if (field_28_pSound)
    {
        gRoot_sound_66B038.DestroySoundObj_40FE60(field_28_pSound);
        field_28_pSound = 0;
    }
}

//MATCH_FUNC(0x5DCD90)

// FUNCTION: 105 0x5DCD90
void Weapon_30::init_5DCD90()
{
    field_24_pPed = 0;
    field_14_car = 0;
    field_1C_idx = 0;
    field_0_ammo = 0;
    field_2_reload_speed = 0;
    field_4 = 0;
    field_21 = 0;
    field_8 = 0;
    field_C = -1;
    field_20 = 0;
    field_2C = 0;
    if (!field_28_pSound && !bSkip_audio_67D6BE)
    {
        field_28_pSound = gRoot_sound_66B038.CreateSoundObject_40EF40(this, 7);
    }
}

MATCH_FUNC(0x5DCDE0)
void Weapon_30::clear_5DCDE0()
{
    init_5DCD90();

    field_8 = 0;

    if (field_28_pSound)
    {
        gRoot_sound_66B038.DestroySoundObj_40FE60(field_28_pSound);
        field_28_pSound = 0;
    }
}

MATCH_FUNC(0x5dce20)
void Weapon_30::add_ammo_5DCE20(u8 a2)
{
    field_0_ammo = a2 * 10;
}

STUB_FUNC(0x5dce40)
char_type Weapon_30::add_ammo_capped_5DCE40(u8 to_add)
{
    NOT_IMPLEMENTED;
    s32 cap_total = max_ammo_capacity_5FF75C[this->field_1C_idx] * 10;
    if (this->field_0_ammo == 0xFFFF)
    {
        return 0;
    }

    s32 cur_amount = this->field_0_ammo;
    if (cur_amount == cap_total)
    {
        return 0;
    }

    s32 new_amount = cur_amount + (to_add * 10);
    if (new_amount <= cap_total)
    {
        this->field_0_ammo = new_amount;
    }
    else
    {
        this->field_0_ammo = cap_total;
    }
    return 1;
}

STUB_FUNC(0x5dcea0)
bool Weapon_30::is_max_capacity_5DCEA0()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x5dcef0)
bool Weapon_30::sub_5DCEF0()
{
    bool result;
    switch (this->field_1C_idx)
    {
        case weapon_type::car_bomb:
        case weapon_type::oil_stain:
        case weapon_type::car_mines:
        case weapon_type::fire_truck_gun:
        case weapon_type::weapon_0x17:
            result = 0;
            break;
        case weapon_type::car_smg:
        case weapon_type::tank_main_gun:
        case weapon_type::fire_truck_flamethrower:
        case weapon_type::army_gun_jeep:
            result = 1;
            break;
        default:
            result = this->field_24_pPed->field_16C_car == 0;
            break;
    }
    return result;
}

MATCH_FUNC(0x5dcf40)
void Weapon_30::sub_5DCF40()
{
    Player* pPlayer = this->field_24_pPed->field_15C_player;
    if (pPlayer)
    {
        if (pPlayer->field_6F4[8]) // Fast reload?
        {
            this->field_2_reload_speed /= 2;
        }
    }
}

STUB_FUNC(0x5dcf60)
Object_2C* Weapon_30::sub_5DCF60(s32 a2, s32 a3, s32 a4, s32 a5, s16 a6, s32* a7)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5dd0f0)
void Weapon_30::flamethrower_5DD0F0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x5dd290)
u8 Weapon_30::shotgun_5DD290()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5dd860)
char_type Weapon_30::pistol_5DD860()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5dda70)
u8 Weapon_30::dual_pistol_5DDA70()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5ddd20)
u8 Weapon_30::smg_5DDD20()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5ddfc0)
void Weapon_30::throwable_5DDFC0(s32 a2, s32 a3, s32 a4)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x5de4f0)
s32 Weapon_30::sub_5DE4F0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5dfb60)
char_type Weapon_30::sub_5DFB60(char_type a2, Sprite* a3, Ang16 a4)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x5e06b0)
void Weapon_30::shocker_5E06B0()
{
    gWeapon_8_707018->field_0.sub_5A6E10();

    sub_5DFB60(0,
               this->field_24_pPed->field_168_game_object->field_80_sprite_ptr,
               this->field_24_pPed->field_168_game_object->field_80_sprite_ptr->field_0);
    Ped* pPed = this->field_24_pPed;
    if (pPed->field_15C_player)
    {
        gShooey_CC_67A4B8->ReportCrimeForPed(2u, pPed);
    }
}

STUB_FUNC(0x5e0740)
char_type Weapon_30::electro_batton_5E0740()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5e0ab0)
void Weapon_30::car_bomb_5E0AB0(char_type a2)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x5e0b10)
char_type Weapon_30::fire_truck_flamethrower_5E0B10()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5e0e70)
s32* Weapon_30::fire_truck_gun_5E0E70()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5e10e0)
u8 Weapon_30::tank_main_gun_5E10E0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5e13e0)
u8 Weapon_30::army_gun_jeep_5E13E0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5e1dc0)
char_type Weapon_30::oil_stain_5E1DC0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5e2550)
char_type Weapon_30::car_mine_5E2550()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5e2940)
u8 Weapon_30::car_smg_5E2940()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x5e33c0)
char_type Weapon_30::sub_5E33C0()
{
    char result;
    switch (this->field_1C_idx)
    {
        case weapon_type::car_bomb:
        case weapon_type::oil_stain:
        case weapon_type::car_mines:
        case weapon_type::tank_main_gun:
        case weapon_type::weapon_0x17:
            result = 0;
            break;
        default:
            result = 1;
            break;
    }
    return result;
}

STUB_FUNC(0x5e34b0)
char_type Weapon_30::sub_5E34B0()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x5e3670)
void Weapon_30::pull_trigger_5E3670()
{
    switch (field_1C_idx)
    {
        case weapon_type::pistol:
            pistol_5DD860();
            break;

        case weapon_type::smg:
        case weapon_type::silence_smg:
            smg_5DDD20();
            break;

        case weapon_type::rocket:
            rocket_5E3850();
            break;

        case weapon_type::car_bomb:
            car_bomb_5E0AB0(0);
            break;

        case weapon_type::oil_stain:
            oil_stain_5E1DC0();
            break;

        case weapon_type::car_mines:
            car_mine_5E2550();
            break;

        case weapon_type::tank_main_gun:
            tank_main_gun_5E10E0();
            break;

        case weapon_type::army_gun_jeep:
            army_gun_jeep_5E13E0();
            break;

        case weapon_type::electro_batton:
            electro_batton_5E0740();
            break;

        case weapon_type::shocker:
            shocker_5E06B0();
            break;

        case weapon_type::molotov:
            if (field_24_pPed && (field_24_pPed->field_15C_player) != 0)
            {
                field_24_pPed->field_15C_player->sub_4CCAB0();
            }
            else
            {
                throwable_5DDFC0(138, 0x1E, 45);
            }
            break;

        case weapon_type::grenade:
            if (field_24_pPed && (field_24_pPed->field_15C_player) != 0)
            {
                Player* p = field_24_pPed->field_15C_player;
                p->sub_4CCAB0();

                // This is really whacky, using p results in most of these inlines being optimized out
                Player* pp = field_24_pPed->field_15C_player;

                if (pp->Get_Field_50() == 0x60)
                {
                    throwable_5DDFC0(183, field_24_pPed->field_15C_player->sub_4CCAD0(), pp->Get_Field_50());
                    this->field_24_pPed->field_15C_player->field_50 = -1;
                }
            }
            else
            {
                throwable_5DDFC0(183, 0x1E, 45);
            }
            break;

        case weapon_type::dual_pistol:
            dual_pistol_5DDA70();
            break;

        case weapon_type::shotgun:
            shotgun_5DD290();
            break;

        case weapon_type::car_smg:
            car_smg_5E2940();
            break;

        case weapon_type::flamethrower:
            flamethrower_5DD0F0();
            break;

        case weapon_type::fire_truck_gun:
            fire_truck_gun_5E0E70();
            break;

        case weapon_type::fire_truck_flamethrower:
            fire_truck_flamethrower_5E0B10();
            break;

        case weapon_type::weapon_0x17:
            car_bomb_5E0AB0(1);
            break;

        default:
            return;
    }
}

STUB_FUNC(0x5e3850)
void Weapon_30::rocket_5E3850()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x5e3bd0)
char_type Weapon_30::sub_5E3BD0()
{
    char result;
    switch (this->field_1C_idx)
    {
        case weapon_type::rocket:
        case weapon_type::molotov:
        case weapon_type::car_bomb:
        case weapon_type::car_mines:
        case weapon_type::tank_main_gun:
            result = 1;
            break;
        default:
            result = 0;
            break;
    }
    return result;
}