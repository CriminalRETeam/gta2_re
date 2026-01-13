#include "Ped.hpp"
#include "CarPhysics_B0.hpp"
#include "Car_BC.hpp"
#include "Char_Pool.hpp"
#include "Game_0x40.hpp"
#include "Gang.hpp"
#include "Globals.hpp"
#include "Hamburger_500.hpp"
#include "Hud.hpp"
#include "Marz_1D7E.hpp"
#include "Object_5C.hpp"
#include "Orca_2FD4.hpp"
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
#include "lucid_hamilton.hpp"
#include "map_0x370.hpp"
#include "rng.hpp"
#include "sprite.hpp"
#include "youthful_einstein.hpp"

// =================
DEFINE_GLOBAL(s8, byte_61A8A3, 0x61A8A3);
DEFINE_GLOBAL_INIT(Ang16, word_6FDB34, Ang16(0), 0x6FDB34);
DEFINE_GLOBAL_INIT(Ang16, word_6787A8, Ang16(0), 0x6787A8);
DEFINE_GLOBAL_INIT(s32, dword_67866C, 0xC000, 0x67866C); // TODO: Fix16? Static init to, 0xC000, 0xUNKNOWN);
DEFINE_GLOBAL(s32, gPedId_61A89C, 0x61A89C);
DEFINE_GLOBAL(u8, gNumberMuggersSpawned_6787CA, 0x6787CA);
DEFINE_GLOBAL(u8, byte_6787CB, 0x6787CB);
DEFINE_GLOBAL(u8, byte_6787CC, 0x6787CC);
DEFINE_GLOBAL(u8, byte_6787CD, 0x6787CD);
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
DEFINE_GLOBAL_INIT(Fix16, dword_678660, Fix16(0), 0x678660);
DEFINE_GLOBAL_INIT(Fix16, dword_678438, dword_678660, 0x678438);
DEFINE_GLOBAL_INIT(Fix16, dword_678750, dword_678660, 0x678750);
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
DEFINE_GLOBAL_INIT(Fix16, dword_678664, Fix16(1), 0x678664);
DEFINE_GLOBAL_INIT(Fix16, dword_6785EC, dword_678664, 0x6785EC);
DEFINE_GLOBAL_INIT(Fix16, k_dword_678624, Fix16(0xA3, 0), 0x678624);
DEFINE_GLOBAL_INIT(Fix16, k_dword_67853C, Fix16(0x2000, 0), 0x67853C);
DEFINE_GLOBAL_INIT(Fix16, dword_678634, Fix16(0x333, 0), 0x678634);
DEFINE_GLOBAL_INIT(Fix16, dword_678480, Fix16(0x666, 0), 0x678480);
DEFINE_GLOBAL_INIT(Fix16, dword_6784A4, Fix16(0x3999, 0), 0x6784A4);
DEFINE_GLOBAL_INIT(Ang16, word_6784FC, Ang16(180), 0x6784FC);
DEFINE_GLOBAL_INIT(Ang16, word_678590, Ang16(0), 0x678590); // TODO: get correct init value
DEFINE_GLOBAL_INIT(Fix16, dword_6784DC, dword_6784C4 * 6, 0x6784DC);
DEFINE_GLOBAL_INIT(Fix16, dword_678668, Fix16(2), 0x678668);
DEFINE_GLOBAL(Ped*, dword_6787C0, 0x6787C0);
DEFINE_GLOBAL(Fix16, dword_678530, 0x678530);
DEFINE_GLOBAL(Fix16, dword_67841C, 0x67841C);
DEFINE_GLOBAL(Object_2C*, dword_678558, 0x678558);
DEFINE_GLOBAL(u8, byte_6787D3, 0x6787D3);
DEFINE_GLOBAL(Fix16, k_dword_678504, 0x678504);
DEFINE_GLOBAL(Fix16, k_dword_67845C, 0x67845C);
DEFINE_GLOBAL(Fix16, k_dword_678798, 0x678798);
DEFINE_GLOBAL(Fix16, k_dword_678658, 0x678658);
DEFINE_GLOBAL(Fix16, k_dword_678680, 0x678680);

// TODO
EXTERN_GLOBAL(s32, bStartNetworkGame_7081F0);

// TODO: move
STUB_FUNC(0x545AF0)
EXPORT void __stdcall sub_545AF0(s32 a1, Car_BC* a2, s8 a3, Fix16& a4, Fix16& a5, Ang16& a6)
{
    NOT_IMPLEMENTED;
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
    sub_45AFC0();
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

STUB_FUNC(0x45afc0)
char_type Ped::sub_45AFC0()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x45b440)
void Ped::PoolAllocate()
{
    Ped::sub_45AFC0();
    field_200_id = gPedId_61A89C++;
    field_21C |= 1;
    field_234_timer = 99;

    switch (field_240_occupation)
    {
        case 15:
            gNumberMuggersSpawned_6787CA = 0;
            break;
        case 16:
            byte_6787CB = 0;
            break;
        case 22:
            byte_6787CC = 0;
            break;
        case 29:
            byte_6787CD = 0;
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
    if (field_27C == 6 || field_27C == 7)
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
void Ped::sub_45BC70()
{
    if (field_278 != 9)
    {
        if (field_210 >= field_212)
        {
            if (field_168_game_object)
            {
                if (field_168_game_object->field_10 != 15 && field_278 != 8)
                {
                    sub_45C500(8);
                    sub_45C540(27);
                    field_168_game_object->field_16 = 1;
                }

                if (field_210 > 0)
                {
                    --field_210;
                }
            }
            else
            {
                field_210 = 0;
            }
        }
        else if (field_210 > 0)
        {
            --field_210;
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

    field_168_game_object->sub_553E00(word_6784FC + pCar->field_50_car_sprite->field_0, dword_678634 + dword_678480, dword_678660, 1);
    Ped::sub_45C500(0);
    Ped::sub_45C540(0);
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
    if ((this->field_21C & ped_bit_status_enum::k_ped_in_flames) != 0)
    {
        if (this->field_208_invulnerability > 0)
        {
            PutOutFire();
        }
        else if (this->field_16C_car)
        {
            PutOutFire();
            field_16C_car->ExplodeCar_Unknown_43D840(19);
        }
        else
        {
            const bool was9Before = this->field_278 == 9;
            TakeDamage(1);
            this->field_264 = 50;

            if (field_278 == 9 && !was9Before)
            {
                Player* pWeapons = this->field_15C_player;
                if (pWeapons)
                {
                    pWeapons->field_44_death_type = 2;
                }
            }

            if (!this->field_15C_player)
            {
                if (this->field_168_game_object)
                {
                    if (this->field_25C_car_state != 1)
                    {
                        this->field_21C |= ped_bit_status_enum::k_ped_0x00000004;

                        sub_463830(1, 9999);

                        this->field_1D0 = field_1AC_cam.x;
                        this->field_1D4 = field_1AC_cam.y;
                        this->field_1D8 = field_1AC_cam.z;
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
    else if (field_278 != 9)
    {
        field_216_health = 100;
    }
}

STUB_FUNC(0x45c0c0)
void Ped::sub_45C0C0(gmp_map_zone* a2)
{
    NOT_IMPLEMENTED;
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

    field_27C = 29;
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
    sub_45AFC0();

    this->field_244_remap = remap;
    this->field_168_game_object = pB4;

    this->field_21C |= 1;

    this->field_1AC_cam.x = pB4->field_80_sprite_ptr->field_14_xpos.x;
    this->field_1AC_cam.y = pB4->field_80_sprite_ptr->field_14_xpos.y;
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
    field_1AC_cam.x = field_16C_car->field_50_car_sprite->field_14_xpos.x;
    field_1AC_cam.y = field_16C_car->field_50_car_sprite->field_14_xpos.y;
    field_1AC_cam.z = field_16C_car->field_50_car_sprite->field_1C_zpos;
}

MATCH_FUNC(0x45c500)
void Ped::sub_45C500(s32 a2)
{
    if (field_278 != ped_state1_enum::ped_fall_on_ground)
    {
        if (a2 == ped_state1_enum::ped_fall_on_ground)
        {
            field_280 = field_278;
        }
        field_278 = a2;
        return;
    }
    if (a2 != ped_state1_enum::ped_fall_on_ground)
    {
        field_280 = a2;
    }
}

MATCH_FUNC(0x45c540)
void Ped::sub_45C540(s32 a2)
{
    if (field_278 != 8)
    {
        field_27C = a2;
    }
    else if (field_27C < 17 || field_27C > 28)
    {
        field_284 = field_27C;
        field_27C = a2;
    }
    else if (a2 < 23 || a2 > 26)
    {
        field_284 = a2;
    }
    else
    {
        field_284 = field_27C;
        field_27C = a2;
    }
}

MATCH_FUNC(0x45c5a0)
void Ped::sub_45C5A0()
{
    field_278 = field_280;
    field_27C = field_284;
}

MATCH_FUNC(0x45c5c0)
void Ped::sub_45C5C0()
{
    if (!this->field_16C_car && this->field_258_objective == 35 && this->field_25C_car_state == 35 &&
        this->field_168_game_object->field_10 != 15 && this->field_27C != 6)
    {
        sub_45C500(0);
        sub_45C540(0);
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
                Ped* pPed = gPedManager_6787BC->sub_470D60();
                pPed->field_240_occupation = 0x32;
                pCar->field_54_driver = pPed;
                pPed->field_16C_car = pCar;
                pPed->field_24C_target_car_door = 0;
                pPed->field_248_enter_car_as_passenger = 0;
                pPed->SetObjective(36, 9999);
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
    this->field_278 = 10;

    pCar->SetDriver(this);
    pCar->field_7C_uni_num = field_238;
    pCar->field_76 = 0;

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
void Ped::sub_45C7F0(Car_BC* pCar)
{
    this->field_248_enter_car_as_passenger = 1;
    this->field_16C_car = pCar;
    this->field_24C_target_car_door = 1;
    this->field_278 = 10;
    this->field_27C = 10;
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

    pChar->field_80_sprite_ptr->AllocInternal_59F950(dword_678530, dword_678530, dword_67841C);

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
Ang16& Ped::sub_45C900(Ang16& a2)
{
    a2 = field_15C_player->field_8;
    return a2;
}

MATCH_FUNC(0x45c920)
Fix16 Ped::sub_45C920()
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
        return dword_678660;
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

    return word_6787A8;
}

MATCH_FUNC(0x45c9b0)
Fix16& Ped::sub_45C9B0(Fix16& a2)
{
    a2 = field_15C_player->field_C;
    return a2;
}

STUB_FUNC(0x45c9d0)
s16* Ped::ComputeAimAngle_45C9D0(s16* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x45caa0)
void Ped::HandleClosePedInteraction_45CAA0()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x45ce50)
void Ped::TakeDamage(s16 damage)
{
    if (field_208_invulnerability <= 0)
    {
        field_216_health -= damage;
        if (field_216_health <= 0)
        {
            if (field_278 != 8)
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
    if (field_278 != 9 && field_278 != 8 && a2->field_18_model == 258 && word_6787D0 < 10)
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
char_type Ped::HandlePickupCollision_45DE80(Object_2C* pPickUp)
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

STUB_FUNC(0x45e080)
void Ped::SpawnWeaponOnDeath_45E080()
{
    NOT_IMPLEMENTED;
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
                        v2 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(200, get_cam_x(), get_cam_y(), get_cam_z(), word_6787A8);
                        if (v2)
                        {
                            v2->field_C_pAny.pExplosion->field_4_idx = 9;
                        }
                        break;

                    case weapon_type::smg:
                        v2 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(201, get_cam_x(), get_cam_y(), get_cam_z(), word_6787A8);
                        if (v2)
                        {
                            v2->field_C_pAny.pExplosion->field_4_idx = 9;
                        }
                        break;

                    case weapon_type::rocket:
                        v2 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(202, get_cam_x(), get_cam_y(), get_cam_z(), word_6787A8);
                        if (v2)
                        {
                            v2->field_C_pAny.pExplosion->field_4_idx = 9;
                        }
                        break;

                    case weapon_type::shocker:
                        v2 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(203, get_cam_x(), get_cam_y(), get_cam_z(), word_6787A8);
                        if (v2)
                        {
                            v2->field_C_pAny.pExplosion->field_4_idx = 9;
                        }
                        break;

                    case weapon_type::molotov:
                        v2 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(204, get_cam_x(), get_cam_y(), get_cam_z(), word_6787A8);
                        if (v2)
                        {
                            v2->field_C_pAny.pExplosion->field_4_idx = 9;
                        }
                        break;

                    case weapon_type::grenade:
                        v2 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(205, get_cam_x(), get_cam_y(), get_cam_z(), word_6787A8);
                        if (v2)
                        {
                            v2->field_C_pAny.pExplosion->field_4_idx = 9;
                        }
                        break;

                    case weapon_type::shotgun:
                        v2 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(206, get_cam_x(), get_cam_y(), get_cam_z(), word_6787A8);
                        if (v2)
                        {
                            v2->field_C_pAny.pExplosion->field_4_idx = 9;
                        }
                        break;

                    case weapon_type::flamethrower:
                        v2 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(208, get_cam_x(), get_cam_y(), get_cam_z(), word_6787A8);
                        if (v2)
                        {
                            v2->field_C_pAny.pExplosion->field_4_idx = 9;
                        }
                        break;

                    case weapon_type::silence_smg:
                        v2 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(209, get_cam_x(), get_cam_y(), get_cam_z(), word_6787A8);
                        if (v2)
                        {
                            v2->field_C_pAny.pExplosion->field_4_idx = 9;
                        }
                        break;

                    case weapon_type::dual_pistol:
                        v2 = gObject_5C_6F8F84->NewPhysicsObj_5299B0(210, get_cam_x(), get_cam_y(), get_cam_z(), word_6787A8);
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

STUB_FUNC(0x45eb60)
void Ped::Deallocate_45EB60()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x45edc0)
char_type Ped::sub_45EDC0()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x45ede0)
bool Ped::IsField238_45EDE0(s32 a2)
{
    return field_238 == a2 ? true : false;
}

STUB_FUNC(0x45ee00)
char_type Ped::sub_45EE00(s32 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x45ee70)
gmp_map_zone* Ped::sub_45EE70()
{
    NOT_IMPLEMENTED;
    return 0;
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
                Ped::sub_45C500(0);
                Ped::sub_45C540(0);
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
                if (field_225 == 1)
                {
                    Ped::SetObjective(objectives_enum::wait_on_foot_26, 30);
                }
                else if (field_225 == 2)
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
            if (field_225 == 1)
            {
                field_21C_bf.b11 = false;
                field_278 = 0;
                field_27C = 0;
                Ped::SetObjective(objectives_enum::no_obj_0, 9999);
                Ped::sub_45EE00(3);
                --gNumberMuggersSpawned_6787CA;
            }
            break;

        case objectives_enum::flee_char_on_foot_till_safe_2:
            if (field_225)
            {
                Ped::SetObjective(objectives_enum::no_obj_0, 9999);
            }
            break;

        default:
            return;
    }
}

STUB_FUNC(0x45ff60)
void Ped::CarThief_AI_45FF60()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x460820)
void Ped::sub_460820()
{
    NOT_IMPLEMENTED;
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
            if (this->field_225 == 1)
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
            if (this->field_225)
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
                this->field_168_game_object->field_38_velocity = dword_678660;
                this->field_24C_target_car_door = 1;
            }
            break;
    }
}

STUB_FUNC(0x461530)
char_type Ped::sub_461530()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x461630)
void Ped::sub_461630()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x4619f0)
char_type Ped::RoadBlockTank_AI_4619F0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x461a60)
s16 Ped::sub_461A60()
{
    NOT_IMPLEMENTED;
    return 0;
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
                    s16 v10;
                    if (field_20e || byte_6787D2 || (v10 = 1000, stru_6F6784.get_int_4F7AE0(&v10) >= 2))
                    {
                        Ped::sub_45EE70();
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
            if (field_225)
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
                        else if (field_225 == 1)
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
                    if (field_225 == 1)
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
                field_164_ped_group->sub_4C9F00();
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
        if (field_278 > 0 && field_278 <= 7)
        {
            Ped::sub_461A60();
        }
        if (field_238 != 2)
        {
            if (field_258_objective != objectives_enum::flee_char_on_foot_always_3 && field_258_objective != objectives_enum::flee_char_always_once_car_stopped_6)
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
        if (field_258_objective != objectives_enum::time_waited_in_car_31 && field_258_objective != objectives_enum::goto_area_any_means_13 &&
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
        field_278 = 10;
        field_27C = 10;

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
    if (field_278 == 9 || field_278 == 8)
    {
        byte_61A8A4 = 0;
        field_21C_bf.b11 = false;
    }
    else
    {
        if (field_168_game_object->field_10 == 15)
        {
            byte_61A8A4 = field_278 == 3;
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

STUB_FUNC(0x4626b0)
char_type Ped::sub_4626B0()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x462b80)
void Ped::sub_462B80()
{
    field_168_game_object->field_8_ped_state_1 = field_278;
    field_168_game_object->field_C_ped_state_2 = field_27C;
    field_168_game_object->sub_545720(dword_678750);
    if (field_168_game_object)
    {
        field_1AC_cam.x = field_168_game_object->get_sprite_xpos();
        field_1AC_cam.y = field_168_game_object->get_sprite_ypos();
        field_1AC_cam.z = field_168_game_object->get_sprite_zpos();
        Char_B4* pB4 = field_168_game_object;
        field_12C = pB4->get_rotation_433A40();

        if (field_278 == 10)
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
            s16 v18 = 300;
            word_6787F2 = stru_6F6784.get_int_4F7AE0(&v18) + 450;
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
    Ped::sub_45BC70();
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
    if (field_21C_bf.b5 != 0 && field_278 != 8)
    {
        Ped::sub_45C500(8);
        Ped::sub_45C540(22);
        field_168_game_object->field_16 = 1;
    }

    if ((u32)field_210 > field_212)
    {
        field_210 = field_212;
    }
    if (field_210 == field_212 && field_278 != 8 && field_27C != 27)
    {
        Ped::sub_45C500(8);
        Ped::sub_45C540(27);
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
        if (Ped::sub_4626B0())
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
                        if (field_27C == 29)
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
                if (field_278 == 9)
                {
                    if ((!field_164_ped_group || field_23C != 99) && field_258_objective != objectives_enum::objective_28)
                    {
                        Ped::SetObjective(objectives_enum::no_obj_0, 9999);
                        Ped::sub_463830(0, 9999);
                        field_278 = 9;
                        field_27C = 15;
                    }
                    if (field_20e > 0xC8u && field_240_occupation != ped_ocupation_enum::unknown_13 && field_238 != 5)
                    {
                        Ped::Deallocate_45EB60();
                    }
                }
                else
                {
                    Ped::Occupation_AI_461F20();
                    if (field_258_objective || field_25C_car_state)
                    {
                        byte_61A8A3 = 0;
                        Ped::ProcessObjective_4632E0();
                    }
                    if (field_168_game_object->field_10 == 15)
                    {
                        if (field_278 > 0 && field_278 <= 7)
                        {
                            Ped::sub_461A60();
                        }
                    }
                }
            }

            if (!byte_678554 && field_21C_bf.b14)
            {
                gOrca_2FD4_6FDEF0->field_3C.RemovePed_471240(this);
                field_21C_bf.b14 = 0;
            }

            if (field_168_game_object)
            {
                Fix16 zpos = get_cam_z();
                if (field_168_game_object->field_58_flags_bf.b0 == 0 && zpos != dword_678660)
                {
                    zpos -= dword_678664;
                }
                field_254 = gMap_0x370_6F6268->GetBlockSpec_4E00A0(get_cam_x(), get_cam_y(), zpos);
                Ped::sub_462B80();

                field_21C_bf.b8 = 0;
                field_21C_bf.b9 = 0;
            }
            else
            {
                field_1AC_cam.x = field_16C_car->field_50_car_sprite->field_14_xpos.x;
                field_1AC_cam.y = field_16C_car->field_50_car_sprite->field_14_xpos.y;
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
            Ped::sub_45C500(0);
            Ped::sub_45C540(0);
            break;
        case 2u:
            Ped::sub_45C500(1);
            Ped::sub_45C540(3);
            break;
        case 3u:
            Ped::sub_45C500(1);
            Ped::sub_45C540(2);
            break;
        case 4u:
            Ped::sub_45C500(7);
            Ped::sub_45C540(14);
            break;
        case 5u:
            Ped::sub_45C500(10);
            Ped::sub_45C540(10);
            break;
        case 6u:
            Ped::sub_45C500(3);
            Ped::sub_45C540(4);
            break;
        case 7u:
            Ped::sub_45C500(4);
            Ped::sub_45C540(10);
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

STUB_FUNC(0x463570)
char_type Ped::SetObjective(s32 a2, s16 a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x463830)
void Ped::sub_463830(s32 a2, s16 a3)
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x463aa0)
void Ped::ProcessOnFootObjective_463AA0()
{
    Ang16 angle = 0;
    if (field_258_objective && !field_225)
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
                field_1B8_target_x = field_150_target_objective_car->field_50_car_sprite->field_14_xpos.x;
                field_1BC_target_y = field_150_target_objective_car->field_50_car_sprite->field_14_xpos.y;
                field_1C0_target_z = field_150_target_objective_car->field_50_car_sprite->field_1C_zpos;
            }
            else
            {
                sub_545AF0(0, field_150_target_objective_car, field_24C_target_car_door, field_1B8_target_x, field_1BC_target_y, angle);
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
            else if (field_1DC_objective_target_x != dword_678660 && field_1E0_objective_target_y != dword_678660)
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
        dword_678750 = Fix16::Max(diff_x, diff_y);

        switch (field_258_objective)
        {
            case objectives_enum::flee_on_foot_till_safe_1:
                Ped::sub_4678E0();
                break;
            case objectives_enum::flee_char_on_foot_till_safe_2:
                Ped::sub_467960();
                break;
            case objectives_enum::flee_char_on_foot_always_3:
                Ped::sub_467A20();
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
                Ped::sub_467E20();
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
                Ped::sub_4686C0();
                break;
            case objectives_enum::leave_car_36:
                Ped::sub_468820();
                break;
            case objectives_enum::patrol_on_foot_42:
                Ped::sub_468C70();
                break;
            case objectives_enum::goto_char_on_foot_16:
                Ped::sub_468E80();
                break;
            case objectives_enum::goto_area_on_foot_12:
                Ped::sub_468DE0();
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
                Ped::sub_468930();
                break;
            case objectives_enum::leave_train_38:
                Ped::sub_468A00();
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
                Ped::sub_469FC0();
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
                Ped::sub_46A290();
                break;
            case objectives_enum::follow_car_on_foot_with_offset_56:
                Ped::sub_46A350();
                break;
            case objectives_enum::fire_at_object_from_vehicle_57:
                Ped::sub_46A530();
                break;
            case objectives_enum::destroy_car_59:
                Ped::sub_46A850();
                break;
            case objectives_enum::destroy_object_58:
                Ped::sub_46A7C0();
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

STUB_FUNC(0x463fb0)
void Ped::ProcessInCarObjective_463FB0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x4645b0)
s16* Ped::sub_4645B0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x465270)
void Ped::Threat_Reaction_AI_465270()
{
    NOT_IMPLEMENTED;
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
s32 Ped::FindNearbyPed_466FB0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x467070)
Ped* Ped::sub_467070()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x467090)
char_type Ped::sub_467090()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x467280)
Sprite* Ped::sub_467280()
{
    this->field_168_game_object->field_8_ped_state_1 = 0;
    this->field_168_game_object->field_C_ped_state_2 = 0;
    this->field_168_game_object->field_10 = 1;

    Char_B4* pB4 = this->field_168_game_object;
    pB4->field_6C = 0;
    pB4->field_68 = 0;

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
void Ped::sub_4678E0()
{
    if (byte_61A8A3)
    {
        if (dword_678750 > dword_678520)
        {
            if (field_168_game_object)
            {
                if (field_168_game_object->field_44 == 2)
                {
                    Ped::sub_45C500(0);
                    Ped::sub_45C540(0);
                    this->field_225 = 1;
                }
                else
                {
                    field_168_game_object->field_38_velocity = this->get_field_1F0();
                }
            }
            else
            {
                field_21C_bf.b11 = 0;
            }
        }
        else
        {
            this->field_168_game_object->field_38_velocity = this->get_field_1F0();
        }
    }
}

MATCH_FUNC(0x467960)
void Ped::sub_467960()
{
    if (field_148_objective_target_ped->field_278 == 9 || (field_148_objective_target_ped->field_21C & 1) == 0)
    {
        Ped::sub_45C500(0);
        Ped::sub_45C540(0);
        this->field_148_objective_target_ped = 0;
        this->field_225 = 1;
        return;
    }

    if (byte_61A8A3)
    {
        if (dword_678750 > dword_678520)
        {
            if (this->field_168_game_object->field_44 == 2)
            {
                Ped::sub_45C500(0);
                Ped::sub_45C540(0);
                this->field_148_objective_target_ped = 0;
                this->field_225 = 1;
            }
        }
        else
        {
            Ped::sub_45C500(1);
            Ped::sub_45C540(3);
            this->field_168_game_object->field_38_velocity = this->field_168_game_object->field_3C_run_or_jump_speed;
            field_21C_bf.b11 = 0;
        }
    }
}

MATCH_FUNC(0x467a20)
void Ped::sub_467A20()
{
    if (field_148_objective_target_ped->field_278 == 9 || (field_148_objective_target_ped->field_21C & 1) == 0)
    {
        Ped::sub_45C500(0);
        Ped::sub_45C540(0);
        this->field_148_objective_target_ped = 0;
        this->field_225 = 1;
        return;
    }

    if (byte_61A8A3)
    {
        this->field_168_game_object->field_38_velocity = this->field_168_game_object->field_3C_run_or_jump_speed;
        field_21C_bf.b11 = 0;
    }
}

MATCH_FUNC(0x467ad0)
void Ped::sub_467AD0()
{
    if (field_16C_car)
    {
        if (field_16C_car->GetVelocity_43A4C0() == dword_678660 && field_25C_car_state != 36)
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
        if (field_16C_car->GetVelocity_43A4C0() == dword_678660 && field_25C_car_state != 36)
        {
            Ped::sub_463830(36, 9999);
            field_154_target_to_enter = field_16C_car;
        }
    }
    else if (byte_61A8A3)
    {
        field_154_target_to_enter = field_150_target_objective_car;
        Ped::SetObjective(objectives_enum::flee_on_foot_till_safe_1, 9999);
        field_1B8_target_x = field_154_target_to_enter->field_50_car_sprite->field_14_xpos.x;
        field_1BC_target_y = field_154_target_to_enter->field_50_car_sprite->field_14_xpos.y;
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
        if (field_148_objective_target_ped->field_278 != 9)
        {
            this->field_225 = 2;
            return;
        }
        // LABEL_25:
        // this->field_225 = 1;
        // return;
    }

    if (field_148_objective_target_ped->field_278 == 9)
    {
        this->field_225 = 1;
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
            field_140->field_76 = 0;
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
                this->field_225 = 1;
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

STUB_FUNC(0x467e20)
void Ped::sub_467E20()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x467fb0)
void Ped::sub_467FB0()
{
    if (byte_61A8A3)
    {
        if (!field_218_objective_timer)
        {
            field_225 = 1;
        }
    }
}

MATCH_FUNC(0x467fd0)
void Ped::sub_467FD0()
{
    if ((field_148_objective_target_ped->field_21C & 1) == 0 || field_148_objective_target_ped->field_278 == 9)
    {
        this->field_225 = 2;
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
                    this->field_225 = 1;
                    return;
                }
                else if (field_226 == 2)
                {
                    this->field_225 = 2;
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

    if (this->field_225 != 1)
    {
        if (this->field_168_game_object)
        {
            this->field_225 = 2;
        }
        else
        {
            if (!this->field_16C_car->field_60)
            {
                this->field_16C_car->field_60 = gHamburger_500_678E30->sub_474810();
                this->field_16C_car->field_60->field_4 = this;
            }

            this->field_16C_car->field_60->field_8 = 1;
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
                this->field_225 = 1;
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
                    this->field_225 = 1;
                    gHamburger_500_678E30->Cancel_474CC0(pCar__->field_60);
                    this->field_16C_car->field_60 = 0;
                    this->field_16C_car->field_A6 |= 0x20u;
                }
                else if (pCar__)
                {
                    if (pCar__->GetVelocity_43A4C0() == dword_678660)
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
void Ped::sub_4686C0()
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
                    field_225 = 1;
                    Ped::sub_463830(0, 9999);
                    Ped::sub_45C500(10);
                    Ped::sub_45C540(10);
                }
                return;
            }
            if (field_226 == 2)
            {
                field_225 = 2;
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
        field_225 = 2;
        Ped::sub_463830(0, 9999);
        field_21C_bf.b2 = false;
    }
}

MATCH_FUNC(0x468820)
void Ped::sub_468820()
{
    if (field_168_game_object)
    {
        if (!field_150_target_objective_car && field_27C == 17)
        {
            field_225 = 1;
        }
    }
    if ((field_21C & 4) != 0)
    {
        if (field_168_game_object)
        {
            if (field_164_ped_group)
            {
                if (field_164_ped_group->sub_4CAB80())
                {
                    field_225 = true;
                }
                else
                {
                    field_225 = false;
                }
            }
            else
            {
                field_225 = true;
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
                    field_225 = 1;
                }
                else
                {
                    Ped::SetObjective(objectives_enum::no_obj_0, 9999);
                }

                Ped::sub_463830(0, 9999);
            }
            else if (field_226 == 2)
            {
                field_225 = 2;
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
            if (field_164_ped_group->sub_4CAB80())
            {
                field_225 = true;
            }
            else
            {
                field_225 = false;
            }
        }
        else
        {
            field_225 = true;
        }
    }
}

MATCH_FUNC(0x468930)
void Ped::sub_468930()
{
    if (field_226 == 2 || field_150_target_objective_car == 0)
    {
        field_225 = 2;
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
                    field_225 = 1;
                    Ped::sub_463830(0, 9999);
                    Ped::sub_45C500(10);
                    Ped::sub_45C540(10);
                    return;
                }
                if (field_226 == 2)
                {
                    field_225 = 2;
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
            field_225 = 2;
            Ped::sub_463830(0, 9999);
        }
    }
}

MATCH_FUNC(0x468a00)
void Ped::sub_468A00()
{
    if (field_25C_car_state)
    {
        if (field_25C_car_state != 12)
        {
            if (field_25C_car_state == 38)
            {
                if (field_226 == 1)
                {
                    if (field_150_target_objective_car->is_train_model())
                    {
                        if (field_238 != 2)
                        {
                            Ped::sub_463830(12, 9999);
                            switch (Ang16::GetAngleFace_4F78F0(field_12C))
                            {
                                case 1:
                                    field_1D0 = field_1AC_cam.x;
                                    field_1D4 = field_1AC_cam.y - dword_678664;
                                    break;
                                case 3:
                                    field_1D0 = dword_678664 + field_1AC_cam.x;
                                    field_1D4 = field_1AC_cam.y;
                                    break;
                                case 2:
                                    field_1D0 = field_1AC_cam.x;
                                    field_1D4 = dword_678664 + field_1AC_cam.y;
                                    break;
                                case 4:
                                    field_1D0 = field_1AC_cam.x - dword_678664;
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
                        field_225 = 1;
                    }
                }
                if (field_226 == 2)
                {
                    field_225 = 2;
                }
            }
        }
        else
        {
            if (field_278 != 8 && field_226 == 1)
            {
                field_225 = 1;
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
        field_225 = 1;
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
            field_278 = 8;
            field_27C = 17;
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
void Ped::sub_468C70()
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
void Ped::sub_468DE0()
{
    if (field_240_occupation == ped_ocupation_enum::drone)
    {
        field_1F0_maybe_max_speed = dword_678434;
    }
    if (byte_61A8A3)
    {
        if (dword_678750 < dword_678788)
        {
            if (field_168_game_object->field_10 != 15)
            {
                Ped::sub_45C500(7);
                Ped::sub_45C540(14);
                field_225 = 1;
            }
        }
        else
        {
            if (!field_218_objective_timer)
            {
                field_225 = 1;
            }
            field_168_game_object->SetMaxSpeed_433920(field_1F0_maybe_max_speed);
        }
        Ped::sub_4672E0(dword_678750, 4);
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
        if (field_27C != 26)
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
        if (dword_678750 <= dword_6784DC && field_1AC_cam.z.ToUInt8() == field_1E4_objective_target_z.ToUInt8())
        {
            if (field_168_game_object->field_10 != 15)
            {
                Ped::sub_45C500(7);
                Ped::sub_45C540(14);
                field_130 = field_134_rotation;
            }
        }
        else
        {
            field_168_game_object->SetMaxSpeed_433920(field_1F4);
            Ped::sub_4672E0(dword_678750, 4);
        }
    }
    else
    {
        if (field_21C_bf.b2 && dword_678750 > field_1E8)
        {
            field_21C_bf.b22 = true;
        }
    }
}

MATCH_FUNC(0x469e10)
void Ped::sub_469E10()
{
    if (!this->field_218_objective_timer)
    {
        this->field_225 = 2;
    }
}

MATCH_FUNC(0x469e30)
void Ped::sub_469E30()
{
    if (field_16C_car)
    {
        field_16C_car->field_5C->field_74 = dword_678664;
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
            field_16C_car->field_60->field_4 = this;
        }
        field_16C_car->field_60->field_8 = 4;
        field_16C_car->sub_421560(5);
        field_16C_car->field_60->field_30 = field_148_objective_target_ped;
        field_16C_car->field_A6 &= ~0x20u;
        field_16C_car->field_5C->field_74 = dword_67866C;
        field_16C_car->field_60->field_20 = 1;
        if (field_16C_car->field_84_car_info_idx == car_model_enum::JEEP)
        {
            if (dword_678750 < dword_678668)
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
        field_16C_car->field_60->field_4 = this;
    }
    field_16C_car->field_60->field_8 = 2;
    field_16C_car->sub_421560(5);
    field_16C_car->field_60->field_30 = field_148_objective_target_ped;
    field_16C_car->field_A6 &= ~0x20u;
    field_16C_car->field_5C->field_74 = dword_67866C;
}

MATCH_FUNC(0x469fc0)
void Ped::sub_469FC0()
{
    Car_BC* pBC = this->field_16C_car;
    if (pBC)
    {
        pBC->field_A6 |= 0x20u;
    }
    else
    {
        this->field_225 = 2;
    }
}

STUB_FUNC(0x469fe0)
void Ped::sub_469FE0()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x46a1f0)
void Ped::sub_46A1F0()
{
    if (field_148_objective_target_ped->field_21C_bf.b0 == false || field_148_objective_target_ped->field_278 == 9)
    {
        field_225 = 2;
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
                    field_225 = 1;
                }
                else if (field_226 == 2)
                {
                    field_225 = 2;
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
void Ped::sub_46A290()
{
    if (!field_150_target_objective_car->field_54_driver || field_168_game_object)
    {
        field_225 = 2;
    }
    else
    {
        if (!field_16C_car->field_60)
        {
            field_16C_car->field_60 = gHamburger_500_678E30->sub_474810();
            field_16C_car->field_60->field_4 = this;
        }
        field_16C_car->field_60->field_8 = 2;
        field_16C_car->sub_421560(5);
        field_16C_car->field_60->field_30 = field_150_target_objective_car->field_54_driver;
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
void Ped::sub_46A7C0()
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
                    field_225 = 1;
                }
                if (field_226 == 2)
                {
                    field_225 = 2;
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
void Ped::sub_46A850()
{
    if (field_150_target_objective_car->field_74_damage == 32001)
    {
        field_225 = 1;
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
                    field_225 = 1;
                }
                if (field_226 == 2)
                {
                    field_225 = 2;
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
    if (dword_678750 > dword_678520)
    {
        Char_B4* pB4 = field_168_game_object;
        if (pB4)
        {
            if (field_258_objective || pB4->field_44 == 2)
            {
                Ped::sub_45C500(0);
                Ped::sub_45C540(0);
                field_226 = 1;
            }
            else
            {
                pB4->field_38_velocity = get_field_1F0();
            }
        }
        else
        {
            field_21C_bf.b11 = 0;
        }
    }
    else
    {
        field_168_game_object->field_38_velocity = get_field_1F0();
    }
}

MATCH_FUNC(0x46a9c0)
void Ped::sub_46A9C0()
{
    field_14C->field_144 = 0;
    if (field_14C->field_278 == 9 || field_14C->field_21C_bf.b0 == false)
    {
        Ped::sub_45C500(0);
        Ped::sub_45C540(0);
        field_14C = 0;
        field_226 = 1;
    }
    else if (field_278 != 8)
    {
        if (dword_678750 > dword_678520)
        {
            if (field_168_game_object->field_44 == 2)
            {
                Ped::sub_45C500(0);
                Ped::sub_45C540(0);
                field_14C = 0;
                field_226 = 1;
            }
        }
        else
        {
            Ped::sub_45C500(1);
            Ped::sub_45C540(3);
            field_168_game_object->SetMaxSpeed_433920(field_168_game_object->field_3C_run_or_jump_speed);
        }
    }
}

MATCH_FUNC(0x46aae0)
void Ped::sub_46AAE0()
{
    if (field_14C->field_278 == 9 || field_14C->field_21C_bf.b0 == false)
    {
        Ped::sub_45C500(0);
        Ped::sub_45C540(0);
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
    if (field_14C->field_278 == 9 || field_14C->field_21C_bf.b0 == false)
    {
        Ped::sub_45C500(0);
        Ped::sub_45C540(0);
        field_14C = 0;
        field_226 = 1;
        Ped::sub_463830(0, 9999);
        field_21C_bf.b2 = false;
    }
    else
    {
        if (field_278 != 8)
        {
            field_21C_bf.b11 = false;
            field_14C->field_144 = 0;
            if (dword_678750 < dword_6785EC)
            {
                Ped::sub_45C500(0);
                Ped::sub_45C540(0);
                field_14C = 0;
                field_226 = 1;
                Ped::sub_463830(0, 9999);
                field_21C_bf.b2 = false;
            }
            else
            {
                Ped::sub_45C500(1);
                Ped::sub_45C540(2);
                Ped::sub_4672E0(dword_678750, 0);
                field_168_game_object->field_38_velocity = field_168_game_object->field_3C_run_or_jump_speed; // OBS: inline doesn't match
            }
        }
    }
}

STUB_FUNC(0x46ac20)
char_type Ped::sub_46AC20()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x46b170)
s32 Ped::sub_46B170()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x46b2f0)
s32 Ped::sub_46B2F0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x46b670)
void Ped::sub_46B670()
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

STUB_FUNC(0x46bdc0)
void Ped::sub_46BDC0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x46c250)
void Ped::sub_46C250()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x46c770)
void Ped::sub_46C770()
{
    if (field_278 != 8)
    {
        if (field_168_game_object->field_44 == 2 
            || field_258_objective == objectives_enum::enter_car_as_driver_35 
            || dword_678750 < dword_678790)
        {
            Ped::sub_463830(0, 9999);
            field_21C_bf.b2 = false;
        }
        else
        {
            Ped::sub_4672E0(dword_678750, 1);
            field_168_game_object->field_38_velocity = dword_678448; // inline doesn't match
        }
    }
}

MATCH_FUNC(0x46c7e0)
void Ped::sub_46C7E0()
{
    if (field_278 != 8)
    {
        if (dword_678750 < dword_6784E8)
        {
            if (field_168_game_object->field_10 != 15)
            {
                Ped::sub_45C500(7);
                Ped::sub_45C540(14);
                field_226 = 1;
            }
        }
        else
        {
            if (!field_21A_car_state_timer)
            {
                field_226 = 1;
            }
            if (field_240_occupation != ped_ocupation_enum::unknown_6 
                && field_258_objective != objectives_enum::patrol_on_foot_42)
            {
                field_168_game_object->SetMaxSpeed_433920(field_1F0_maybe_max_speed);
            }
            else
            {
                field_168_game_object->SetMaxSpeed_433920(field_1F4);
            }
        }
        Ped::sub_4672E0(dword_678750, 1);
    }
}

MATCH_FUNC(0x46c8a0)
void Ped::sub_46C8A0()
{
    if (field_278 != 8)
    {
        field_168_game_object->field_38_velocity = dword_678448;
        if (dword_678750 < dword_678790)
        {
            Ped::sub_45C500(7);
            Ped::sub_45C540(14);
            field_226 = 1;
        }
        else
        {
            field_230 = 2;
            Ped::sub_4672E0(dword_678750, 2);
            field_168_game_object->field_38_velocity = dword_678448;
        }
    }
}

MATCH_FUNC(0x46c910)
void Ped::sub_46C910()
{
    if (field_278 != 8)
    {
        if (dword_678750 < dword_6784E8)
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
            Ped::sub_45C500(2);
            Ped::sub_45C540(0);
            field_168_game_object->field_38_velocity = dword_678448;
        }
    }
}

MATCH_FUNC(0x46c9b0)
void Ped::sub_46C9B0()
{
    if (field_278 != 8)
    {
        if (dword_678750 < dword_6784E8)
        {
            Ped::sub_45C500(0);
            Ped::sub_45C540(0);
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
        this->field_16C_car->field_60->field_4 = this;
    }

    if (this->field_258_objective == 19)
    {
        field_16C_car->field_60->field_8 = 5;
        if ((field_21C & 0x80u) != 0)
        {
            this->field_21C |= 0x800;
        }
    }
    else
    {
        field_16C_car->field_60->field_8 = 2;
    }

    Car_BC* pBC = this->field_16C_car;
    pBC->field_7C_uni_num = 5;
    pBC->field_76 = 0;
    this->field_16C_car->field_60->field_30 = this->field_14C;
    this->field_16C_car->field_A6 &= ~0x20u;
    this->field_16C_car->field_5C->field_74 = dword_67866C;
    this->field_16C_car->field_60->field_20 = 1;
}

STUB_FUNC(0x46cb30)
char_type Ped::sub_46CB30()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x46cc70)
char_type Ped::sub_46CC70()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x46cdb0)
char_type Ped::sub_46CDB0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x46cef0)
char_type Ped::sub_46CEF0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x46d030)
void Ped::sub_46D030()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x46d0b0)
void Ped::sub_46D0B0()
{
    if (field_21A_car_state_timer == 0)
    {
        field_226 = 1;
    }
}

STUB_FUNC(0x46d0d0)
void Ped::sub_46D0D0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x46d240)
void Ped::sub_46D240()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x46d300)
void Ped::sub_46D300()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x46d460)
void Ped::sub_46D460(char_type a2)
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x46db60)
void Ped::sub_46DB60()
{
    sub_46D460(0);
}

MATCH_FUNC(0x46db70)
void Ped::sub_46DB70()
{
    sub_46D460(1);
}

MATCH_FUNC(0x46db80)
void Ped::sub_46DB80()
{
    sub_46D460(2);
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
        rect.field_10 = searchRadius - k_dword_67845C;
        rect.field_14 = searchRadius + k_dword_67845C;
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
        collision_list.sub_5A6E10();
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

STUB_FUNC(0x46f110)
Weapon_30* Ped::GetWeaponFromPed_46F110()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x46f1e0)
void Ped::sub_46F1E0(Weapon_30* a2)
{
    NOT_IMPLEMENTED;
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

            if (dword_678750 < k_dword_678658 + k_dword_678798)
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
            if (this->field_14C->field_16C_car || dword_678750 > k_dword_67853C)
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

STUB_FUNC(0x46f680)
void Ped::sub_46F680(Ped* a2)
{
    NOT_IMPLEMENTED;
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

STUB_FUNC(0x46f9d0)
void Ped::Kill_46F9D0()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x46fc70)
void Ped::AddThreateningPedToList_46FC70()
{
    unk_6787EF = 1;
    gThreateningPedsList_678468.AddPedToFrontIfMissing_4711B0(this);
}

STUB_FUNC(0x46fc90)
void Ped::sub_46FC90(s32 a2, s32 a3)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x46fe20)
char_type Ped::ProcessWeaponHitResponse_46FE20(Object_2C* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x46ff00)
void Ped::NotifyWeaponHit_46FF00(s32 a2, s32 a3, s32 a4)
{
    NOT_IMPLEMENTED;
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
    Ang16 tan_v = Fix16::atan2_fixed_405320(objective_target_ped->field_1AC_cam.x - pHitSprite->field_14_xpos.x,
                                            objective_target_ped->field_1AC_cam.y - pHitSprite->field_14_xpos.y);

    this->field_21C &= ~0x800;
    this->field_21C |= 0x80;

    if (field_16C_car->RotateRoofObjectTowardTarget_440C10(tan_v))
    {
        if (field_148_objective_target_ped->IsField238_45EDE0(2))
        {
            if (!this->field_16C_car->field_76)
            {
                this->field_21C |= 0x800;
            }
        }
        else if (dword_678750 < k_dword_678680)
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
        Ped::sub_45C540(0);
        Ped::sub_45C500(0);
        field_168_game_object->field_38_velocity = dword_678438;
    }
    else
    {
        Ped::sub_45C540(0);
        Ped::sub_45C500(0);
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
            pCar->field_76 = 0;
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
