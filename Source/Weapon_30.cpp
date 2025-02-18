#include "Weapon_30.hpp"
#include "Ped.hpp"
#include "Player.hpp"
#include "debug.hpp"
#include "enums.hpp"
#include "root_sound.hpp"

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

MATCH_FUNC(0x5DCD90)
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
char_type Weapon_30::add_ammo_capped_5DCE40(u8 a2)
{
    return 0;
}

STUB_FUNC(0x5dcea0)
bool Weapon_30::is_max_capacity_5DCEA0()
{
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
    Player* pPlayer = this->field_24_pPed->field_15C_player_weapons;
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
    return 0;
}

STUB_FUNC(0x5dd0f0)
void Weapon_30::flamethrower_5DD0F0()
{
}

STUB_FUNC(0x5dd290)
u8 Weapon_30::shotgun_5DD290()
{
    return 0;
}

STUB_FUNC(0x5dd860)
char_type Weapon_30::pistol_5DD860()
{
    return 0;
}

STUB_FUNC(0x5dda70)
u8 Weapon_30::dual_pistol_5DDA70()
{
    return 0;
}

STUB_FUNC(0x5ddd20)
u8 Weapon_30::smg_5DDD20()
{
    return 0;
}

STUB_FUNC(0x5ddfc0)
void Weapon_30::throwable_5DDFC0(Fix16* a2, char_type* a3, s32 a4)
{
}

STUB_FUNC(0x5de4f0)
s32 Weapon_30::sub_5DE4F0()
{
    return 0;
}

STUB_FUNC(0x5dfb60)
char_type Weapon_30::sub_5DFB60(char_type a2, Sprite* a3, s16 a4)
{
    return 0;
}

STUB_FUNC(0x5e06b0)
void Weapon_30::shocker_5E06B0()
{
}

STUB_FUNC(0x5e0740)
char_type Weapon_30::electro_batton_5E0740()
{
    return 0;
}

STUB_FUNC(0x5e0ab0)
void Weapon_30::car_bomb_5E0AB0(char_type a2)
{
}

STUB_FUNC(0x5e0b10)
char_type Weapon_30::fire_truck_flamethrower_5E0B10()
{
    return 0;
}

STUB_FUNC(0x5e0e70)
s32* Weapon_30::fire_truck_gun_5E0E70()
{
    return 0;
}

STUB_FUNC(0x5e10e0)
u8 Weapon_30::tank_main_gun_5E10E0()
{
    return 0;
}

STUB_FUNC(0x5e13e0)
u8 Weapon_30::army_gun_jeep_5E13E0()
{
    return 0;
}

STUB_FUNC(0x5e1dc0)
char_type Weapon_30::oil_stain_5E1DC0()
{
    return 0;
}

STUB_FUNC(0x5e2550)
char_type Weapon_30::car_mine_5E2550()
{
    return 0;
}

STUB_FUNC(0x5e2940)
u8 Weapon_30::car_smg_5E2940()
{
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
    return 0;
}

STUB_FUNC(0x5e3670)
void Weapon_30::pull_trigger_5E3670()
{
}

STUB_FUNC(0x5e3850)
void Weapon_30::rocket_5E3850()
{
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
