#include "Ped.hpp"
#include "Car_B0.hpp"
#include "Car_BC.hpp"
#include "Game_0x40.hpp"
#include "Globals.hpp"
#include "Hamburger_500.hpp"
#include "Marz_1D7E.hpp"
#include "Object_5C.hpp"
#include "PedGroup.hpp"
#include "Player.hpp"
#include "Police_7B8.hpp"
#include "PurpleDoom.hpp"
#include "Weapon_30.hpp"
#include "Weapon_8.hpp"
#include "Wolfy_3D4.hpp"
#include "char.hpp"
#include "map_0x370.hpp"

// =================
EXPORT_VAR s8 byte_61A8A3;
GLOBAL(byte_61A8A3, 0x61A8A3);

EXPORT_VAR Ang16 word_6FDB34;
GLOBAL(word_6FDB34, 0x6FDB34);

EXPORT_VAR Ang16 word_6787A8;
GLOBAL(word_6787A8, 0x6787A8);

EXPORT_VAR s32 dword_67866C; // TODO: Fix16? Static init to 0xC000
GLOBAL(dword_67866C, 0x67866C);

EXPORT_VAR Fix16 dword_678438;
GLOBAL(dword_678438, 0x678438);

MATCH_FUNC(0x45ae70)
Ped::Ped()
{
    field_200_id = 0;
    sub_45AFC0();
    field_160_next_ped = 0;
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
    this->field_158 = 0;
    this->field_154_target_to_enter = 0;
    this->field_160_next_ped = 0;
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
    this->field_204 = 0;
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
    return 0;
}

STUB_FUNC(0x45b440)
s32 Ped::sub_45B440()
{
    return 0;
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

STUB_FUNC(0x45b520)
u32* Ped::sub_45B520(u32* a2)
{
    return 0;
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
s32 Ped::sub_45B5B0(s32 a2)
{
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
void Ped::sub_45BC10(Fix16 xpos, Fix16 ypos)
{
    Fix16 tempZ;
    gMap_0x370_6F6268->FindGroundZForCoord_4E5B60(&tempZ, xpos, ypos);
    Car_BC* pCar = this->field_16C_car;
    if (pCar)
    {
        pCar->sub_443D00(xpos, ypos, tempZ);
    }
    else
    {
        field_168_game_object->sub_545530(xpos, ypos, tempZ);
    }
}

STUB_FUNC(0x45bc70)
void Ped::sub_45BC70()
{
}

STUB_FUNC(0x45bd20)
char_type Ped::sub_45BD20(Car_BC* a2)
{
    return 0;
}

STUB_FUNC(0x45be30)
s32 Ped::sub_45BE30()
{
    return 0;
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
void Ped::sub_45BEC0()
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
            field_16C_car->sub_43D840(19);
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

STUB_FUNC(0x45bfb0)
void Ped::DrawFlamesAndStartScreamTimer()
{
    if ((field_21C & ped_bit_status_enum::k_ped_in_flames) != 0)
    {
        // TODO: This is actually another function
        Char_B4* pB4 = field_168_game_object;
        if (pB4)
        {
            // Spawn fire
            Object_2C* p2C = gObject_5C_6F8F84->sub_5299B0(197, 0, 0, 0, word_6FDB34); // dead_rubbish_197 ?? but its actually fire
            pB4->field_80_sprite_ptr->sub_5A3100(p2C->field_4, 0, 0, word_6FDB34);
            pB4->field_b0 = 10; // Start screaming timer
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
    this->field_208_invulnerability = 0;
    Char_B4* pB4 = this->field_168_game_object;
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
}

MATCH_FUNC(0x45c310)
void Ped::sub_45C310()
{
    if (field_168_game_object)
    {
        gChar_11944_6FDB44->sub_4355C0(field_168_game_object);
        field_168_game_object = 0;
    }
}

STUB_FUNC(0x45c350)
void Ped::sub_45C350(gmp_map_zone* a2)
{
}

STUB_FUNC(0x45c410)
s32 Ped::sub_45C410()
{
    return 0;
}

STUB_FUNC(0x45c4b0)
void Ped::sub_45C4B0()
{
    // TODO: Needs Sprite stubs
    /*
    Car_BC* pBc = field_16C_car;
    field_1AC_cam.x = pBc->field_50_car_sprite->field_14_xpos;
    field_1AC_cam.y = pBc->field_50_car_sprite->field_18_ypos;
    field_1AC_cam.z = pBc->field_50_car_sprite->field_1C_zpos;
    */
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

STUB_FUNC(0x45C650)
void Ped::SpawnDriverRunAway_45C650(Car_BC* pCar)
{
}

STUB_FUNC(0x45c730)
s32 Ped::sub_45C730(Car_BC* a2)
{
    return 0;
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

STUB_FUNC(0x45c7f0)
Char_8* Ped::sub_45C7F0(Car_BC* pCar)
{
    // TODO: ordering
    field_248_enter_car_as_passenger = 1;
    field_24C_target_car_door = 1;
    field_16C_car = pCar;
    field_278 = 10;
    field_27C = 10;
    return pCar->field_4.sub_471140(this);
}

STUB_FUNC(0x45c830)
char_type Ped::sub_45C830(Fix16 xpos, Fix16 ypos, Fix16 zpos)
{
    return 0;
}

STUB_FUNC(0x45c900)
s16* Ped::sub_45C900(s16* a2)
{
    return 0;
}

STUB_FUNC(0x45c920)
s32* Ped::sub_45C920(s32* a2)
{
    return 0;
}

MATCH_FUNC(0x45c960)
Ang16& Ped::GetRotation(Ang16& a2)
{
    if (field_168_game_object != NULL)
    {
        a2 = *(Ang16*)&field_168_game_object->field_40_rotation;
    }
    else
    {
        if (field_16C_car != NULL)
        {
            a2 = field_16C_car->field_50_car_sprite->field_0;
        }
        else
        {
            a2 = word_6787A8;
        }
    }
    return a2;
}

STUB_FUNC(0x45c9b0)
s32* Ped::sub_45C9B0(s32* a2)
{
    return 0;
}

STUB_FUNC(0x45c9d0)
s16* Ped::sub_45C9D0(s16* a2)
{
    return 0;
}

STUB_FUNC(0x45caa0)
void Ped::sub_45CAA0()
{
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
                sub_46F9D0();
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

STUB_FUNC(0x45cf20)
void Ped::sub_45CF20(s32 a2)
{
}

STUB_FUNC(0x45d000)
char_type Ped::sub_45D000(s32 a2)
{
    return 0;
}

STUB_FUNC(0x45dd30)
char_type Ped::sub_45DD30(s32 a2, char_type a3)
{
    return 0;
}

STUB_FUNC(0x45de80)
char_type Ped::sub_45DE80(s32 a2)
{
    return 0;
}

STUB_FUNC(0x45e080)
void Ped::sub_45E080()
{
    Object_2C* v2; // eax

    if ((this->field_224 & 0x20) != 0 && !this->field_16C_car)
    {
        if (this->field_170_selected_weapon)
        {
            if (!sub_45EDE0(2))
            {
                switch (this->field_170_selected_weapon->field_1C_idx)
                {
                    case weapon_type::pistol:
                    case weapon_type::electro_batton:
                        v2 = gObject_5C_6F8F84->sub_5299B0(200, get_cam_x(), get_cam_y(), get_cam_z(), word_6787A8);
                        if (v2)
                        {
                            v2->field_C->field_4_idx = 9;
                        }
                        break;

                    case weapon_type::smg:
                        v2 = gObject_5C_6F8F84->sub_5299B0(201, get_cam_x(), get_cam_y(), get_cam_z(), word_6787A8);
                        if (v2)
                        {
                            v2->field_C->field_4_idx = 9;
                        }
                        break;

                    case weapon_type::rocket:
                        v2 = gObject_5C_6F8F84->sub_5299B0(202, get_cam_x(), get_cam_y(), get_cam_z(), word_6787A8);
                        if (v2)
                        {
                            v2->field_C->field_4_idx = 9;
                        }
                        break;

                    case weapon_type::shocker:
                        v2 = gObject_5C_6F8F84->sub_5299B0(203, get_cam_x(), get_cam_y(), get_cam_z(), word_6787A8);
                        if (v2)
                        {
                            v2->field_C->field_4_idx = 9;
                        }
                        break;

                    case weapon_type::molotov:
                        v2 = gObject_5C_6F8F84->sub_5299B0(204, get_cam_x(), get_cam_y(), get_cam_z(), word_6787A8);
                        if (v2)
                        {
                            v2->field_C->field_4_idx = 9;
                        }
                        break;

                    case weapon_type::grenade:
                        v2 = gObject_5C_6F8F84->sub_5299B0(205, get_cam_x(), get_cam_y(), get_cam_z(), word_6787A8);
                        if (v2)
                        {
                            v2->field_C->field_4_idx = 9;
                        }
                        break;

                    case weapon_type::shotgun:
                        v2 = gObject_5C_6F8F84->sub_5299B0(206, get_cam_x(), get_cam_y(), get_cam_z(), word_6787A8);
                        if (v2)
                        {
                            v2->field_C->field_4_idx = 9;
                        }
                        break;

                    case weapon_type::flamethrower:
                        v2 = gObject_5C_6F8F84->sub_5299B0(208, get_cam_x(), get_cam_y(), get_cam_z(), word_6787A8);
                        if (v2)
                        {
                            v2->field_C->field_4_idx = 9;
                        }
                        break;

                    case weapon_type::silence_smg:
                        v2 = gObject_5C_6F8F84->sub_5299B0(209, get_cam_x(), get_cam_y(), get_cam_z(), word_6787A8);
                        if (v2)
                        {
                            v2->field_C->field_4_idx = 9;
                        }
                        break;

                    case weapon_type::dual_pistol:
                        v2 = gObject_5C_6F8F84->sub_5299B0(210, get_cam_x(), get_cam_y(), get_cam_z(), word_6787A8);
                        if (v2)
                        {
                            v2->field_C->field_4_idx = 9;
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
}

STUB_FUNC(0x45ea00)
void Ped::sub_45EA00()
{
}

STUB_FUNC(0x45eb60)
void Ped::sub_45EB60()
{
}

STUB_FUNC(0x45edc0)
char_type Ped::sub_45EDC0()
{
    return 0;
}

MATCH_FUNC(0x45ede0)
bool Ped::sub_45EDE0(s32 a2)
{
    return field_238 == a2 ? true : false;
}

STUB_FUNC(0x45ee00)
char_type Ped::sub_45EE00(s32 a2)
{
    return 0;
}

STUB_FUNC(0x45ee70)
gmp_map_zone* Ped::sub_45EE70()
{
    return 0;
}

STUB_FUNC(0x45f360)
void Ped::sub_45F360()
{
}

STUB_FUNC(0x45ff60)
void Ped::sub_45FF60()
{
}

STUB_FUNC(0x460820)
void Ped::sub_460820()
{
}

STUB_FUNC(0x461290)
void Ped::sub_461290()
{
}

STUB_FUNC(0x461530)
char_type Ped::sub_461530()
{
    return 0;
}

STUB_FUNC(0x461630)
void Ped::sub_461630()
{
}

STUB_FUNC(0x4619f0)
char_type Ped::sub_4619F0()
{
    return 0;
}

STUB_FUNC(0x461a60)
s16 Ped::sub_461A60()
{
    return 0;
}

STUB_FUNC(0x461f20)
void Ped::sub_461F20()
{
}

STUB_FUNC(0x462280)
s16 Ped::sub_462280()
{
    return 0;
}

STUB_FUNC(0x4624a0)
s32 Ped::sub_4624A0()
{
    return 0;
}

STUB_FUNC(0x462510)
s32 Ped::RemovePedWeapons_462510()
{
    return 0;
}

STUB_FUNC(0x462550)
s32 Ped::sub_462550()
{
    return 0;
}

STUB_FUNC(0x462590)
void Ped::sub_462590()
{
}

STUB_FUNC(0x462620)
s32 Ped::sub_462620()
{
    return 0;
}

STUB_FUNC(0x4626b0)
char_type Ped::sub_4626B0()
{
    return 0;
}

STUB_FUNC(0x462b80)
void Ped::sub_462B80()
{
}

STUB_FUNC(0x462e70)
char_type Ped::sub_462E70()
{
    return 0;
}

MATCH_FUNC(0x4632e0)
void Ped::sub_4632E0()
{
    sub_463AA0();
    sub_463FB0();
}

STUB_FUNC(0x463300)
s32 Ped::sub_463300(u8 a1)
{
    return 0;
}

STUB_FUNC(0x4633e0)
s32 Ped::sub_4633E0(char_type a2)
{
    return 0;
}

STUB_FUNC(0x463570)
char_type Ped::SetObjective(s32 a2, s16 a3)
{
    return 0;
}

STUB_FUNC(0x463830)
void Ped::sub_463830(s32 a2, s16 a3)
{
}

STUB_FUNC(0x463aa0)
void Ped::sub_463AA0()
{
}

STUB_FUNC(0x463fb0)
void Ped::sub_463FB0()
{
}

STUB_FUNC(0x4645b0)
s16* Ped::sub_4645B0()
{
    return 0;
}

STUB_FUNC(0x465270)
void Ped::sub_465270()
{
}

STUB_FUNC(0x465b20)
char_type Ped::sub_465B20()
{
    return 0;
}

STUB_FUNC(0x465cd0)
bool Ped::sub_465CD0()
{
    return 0;
}

STUB_FUNC(0x465d00)
char_type Ped::sub_465D00(Ped* a2)
{
    return 0;
}

STUB_FUNC(0x466b90)
Ped* Ped::sub_466B90(Ped* a2)
{
    return 0;
}

STUB_FUNC(0x466bb0)
Ped* Ped::sub_466BB0(s32 a2)
{
    return 0;
}

STUB_FUNC(0x466bd0)
Ped* Ped::sub_466BD0(Ped* a2)
{
    return 0;
}

STUB_FUNC(0x466bf0)
Ped* Ped::sub_466BF0(s32 a2)
{
    return 0;
}

STUB_FUNC(0x466f40)
s32 Ped::sub_466F40(u8 a2)
{
    return 0;
}

STUB_FUNC(0x466f60)
s32 Ped::sub_466F60(u8 a2)
{
    return 0;
}

STUB_FUNC(0x466fb0)
s32 Ped::sub_466FB0()
{
    return 0;
}

STUB_FUNC(0x467090)
char_type Ped::sub_467090()
{
    return 0;
}

MATCH_FUNC(0x467280)
Sprite* Ped::sub_467280()
{
    this->field_168_game_object->field_8_ped_state_1 = 0;
    this->field_168_game_object->field_C_ped_state_2 = 0;
    this->field_168_game_object->field_10 = 1;

    Char_B4* pB4 = this->field_168_game_object;
    pB4->field_6c = 0;
    pB4->field_68 = 0;

    this->field_216_health = 50;
    return gPurpleDoom_1_679208->sub_477E60(this->field_168_game_object->field_80_sprite_ptr, 2);
}

STUB_FUNC(0x4672e0)
char_type Ped::sub_4672E0(s32 a2, s32 a3)
{
    return 0;
}

STUB_FUNC(0x4678e0)
char_type Ped::sub_4678E0()
{
    return 0;
}

STUB_FUNC(0x467960)
char_type Ped::sub_467960()
{
    return 0;
}

STUB_FUNC(0x467a20)
char_type Ped::sub_467A20()
{
    return 0;
}

STUB_FUNC(0x467ad0)
void Ped::sub_467AD0()
{
}

STUB_FUNC(0x467bd0)
void Ped::sub_467BD0()
{
}

STUB_FUNC(0x467ca0)
void Ped::sub_467CA0()
{
}

STUB_FUNC(0x467e20)
void Ped::sub_467E20()
{
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

STUB_FUNC(0x467fd0)
char_type Ped::sub_467FD0()
{
    return 0;
}

STUB_FUNC(0x468040)
s16 Ped::sub_468040()
{
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

STUB_FUNC(0x468310)
void Ped::sub_468310()
{
}

STUB_FUNC(0x4686c0)
void Ped::sub_4686C0()
{
}

STUB_FUNC(0x468820)
void Ped::sub_468820()
{
}

STUB_FUNC(0x468930)
void Ped::sub_468930()
{
}

STUB_FUNC(0x468a00)
void Ped::sub_468A00()
{
}

STUB_FUNC(0x468bd0)
void Ped::sub_468BD0()
{
}

STUB_FUNC(0x468c70)
char_type Ped::sub_468C70()
{
    return 0;
}

STUB_FUNC(0x468de0)
char_type Ped::sub_468DE0()
{
    return 0;
}

STUB_FUNC(0x468e80)
char_type Ped::sub_468E80()
{
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
                field_204 = 0;
                field_290 = 0;
            }
        }
    }
}

STUB_FUNC(0x469060)
void Ped::sub_469060()
{
}

STUB_FUNC(0x469bd0)
void Ped::sub_469BD0()
{
}

STUB_FUNC(0x469bf0)
char_type Ped::sub_469BF0()
{
    return 0;
}

STUB_FUNC(0x469d60)
s32 Ped::sub_469D60()
{
    return 0;
}

MATCH_FUNC(0x469e10)
void Ped::sub_469E10()
{
    if (!this->field_218_objective_timer)
    {
        this->field_225 = 2;
    }
}

STUB_FUNC(0x469e30)
s32 Ped::sub_469E30()
{
    return 0;
}

STUB_FUNC(0x469e50)
s32 Ped::sub_469E50()
{
    return 0;
}

STUB_FUNC(0x469f30)
s32 Ped::sub_469F30()
{
    return 0;
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
}

STUB_FUNC(0x46a1f0)
void Ped::sub_46A1F0()
{
}

STUB_FUNC(0x46a290)
s32 Ped::sub_46A290()
{
    return 0;
}

STUB_FUNC(0x46a350)
char_type Ped::sub_46A350()
{
    return 0;
}

STUB_FUNC(0x46a530)
s32 Ped::sub_46A530()
{
    return 0;
}

STUB_FUNC(0x46a5e0)
void Ped::sub_46A5E0()
{
}

STUB_FUNC(0x46a6d0)
s16 Ped::sub_46A6D0()
{
    return 0;
}

STUB_FUNC(0x46a7c0)
void Ped::sub_46A7C0()
{
}

STUB_FUNC(0x46a850)
void Ped::sub_46A850()
{
}

STUB_FUNC(0x46a8f0)
s16 Ped::sub_46A8F0()
{
    return 0;
}

STUB_FUNC(0x46a9c0)
s32 Ped::sub_46A9C0()
{
    return 0;
}

STUB_FUNC(0x46aae0)
char_type Ped::sub_46AAE0()
{
    return 0;
}

STUB_FUNC(0x46ab50)
char_type Ped::sub_46AB50()
{
    return 0;
}

STUB_FUNC(0x46ac20)
char_type Ped::sub_46AC20()
{
    return 0;
}

STUB_FUNC(0x46b170)
s32 Ped::sub_46B170()
{
    return 0;
}

STUB_FUNC(0x46b2f0)
s32 Ped::sub_46B2F0()
{
    return 0;
}

STUB_FUNC(0x46b670)
void Ped::sub_46B670()
{
}

MATCH_FUNC(0x46bd30)
void Ped::sub_46BD30()
{
    if (!this->field_21A)
    {
        this->field_226 = 1;
    }
}

STUB_FUNC(0x46bd50)
char_type Ped::sub_46BD50(u8* a2)
{
    return 0;
}

STUB_FUNC(0x46bdc0)
void Ped::sub_46BDC0()
{
}

STUB_FUNC(0x46c250)
void Ped::sub_46C250()
{
}

STUB_FUNC(0x46c770)
char_type Ped::sub_46C770()
{
    return 0;
}

STUB_FUNC(0x46c7e0)
s32 Ped::sub_46C7E0()
{
    return 0;
}

STUB_FUNC(0x46c8a0)
s32 Ped::sub_46C8A0()
{
    return 0;
}

STUB_FUNC(0x46c910)
void Ped::sub_46C910()
{
}

STUB_FUNC(0x46c9b0)
void Ped::sub_46C9B0()
{
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
    return 0;
}

STUB_FUNC(0x46cc70)
char_type Ped::sub_46CC70()
{
    return 0;
}

STUB_FUNC(0x46cdb0)
char_type Ped::sub_46CDB0()
{
    return 0;
}

STUB_FUNC(0x46cef0)
char_type Ped::sub_46CEF0()
{
    return 0;
}

STUB_FUNC(0x46d030)
void Ped::sub_46D030()
{
}

MATCH_FUNC(0x46d0b0)
void Ped::sub_46D0B0()
{
    if (!this->field_21A)
    {
        this->field_226 = 1;
    }
}

STUB_FUNC(0x46d0d0)
void Ped::sub_46D0D0()
{
}

STUB_FUNC(0x46d240)
void Ped::sub_46D240()
{
}

STUB_FUNC(0x46d300)
void Ped::sub_46D300()
{
}

STUB_FUNC(0x46d460)
void Ped::sub_46D460(char_type a2)
{
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
Sprite* Ped::sub_46DF50()
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

STUB_FUNC(0x46df70)
s32 Ped::sub_46DF70(s32 a2, s32 a3)
{
    return 0;
}

STUB_FUNC(0x46e020)
bool Ped::sub_46E020(PedGroup* pGroup)
{
    return this->field_164_ped_group != pGroup && !this->field_15C_player &&
        (sub_45EDE0(3) || (sub_45EDE0(4) || sub_45EDE0(6)) && this->field_240_occupation == 35);
}

STUB_FUNC(0x46e080)
s32 Ped::sub_46E080(s32 a2, s32 a3)
{
    return 0;
}

STUB_FUNC(0x46e200)
u8 Ped::sub_46E200(u8 a2)
{
    return 0;
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
Weapon_30* Ped::sub_46F110()
{
    return 0;
}

STUB_FUNC(0x46f1e0)
void Ped::sub_46F1E0(Weapon_30* a2)
{
}

STUB_FUNC(0x46f390)
void Ped::sub_46F390()
{
}

STUB_FUNC(0x46f490)
s32 Ped::sub_46F490()
{
    return 0;
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
        if (field_170_selected_weapon->sub_5E3BD0())
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
}

STUB_FUNC(0x46f720)
void Ped::sub_46F720()
{
}

STUB_FUNC(0x46f9d0)
void Ped::sub_46F9D0()
{
}

STUB_FUNC(0x46fc70)
u32* Ped::sub_46FC70()
{
    return 0;
}

STUB_FUNC(0x46fc90)
void Ped::sub_46FC90(s32 a2, s32 a3)
{
}

STUB_FUNC(0x46fe20)
char_type Ped::sub_46FE20(s32 a2)
{
    return 0;
}

STUB_FUNC(0x46ff00)
void Ped::sub_46FF00(s32 a2, s32 a3, s32 a4)
{
}

STUB_FUNC(0x46fff0)
void Ped::sub_46FFF0(s32 a2)
{
}

STUB_FUNC(0x470050)
s16 Ped::sub_470050()
{
    return 0;
}

STUB_FUNC(0x470160)
void Ped::add_wanted_points_470160(s16 wanted_amount)
{
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
    Sprite* pSprite = gPurpleDoom_1_679208->sub_477E60(this->field_168_game_object->field_80_sprite_ptr, 0);
    if (pSprite)
    {
        return (pSprite->field_30_sprite_type_enum != sprite_types_enum::car) ? false : true;
    }
    return false;
}

MATCH_FUNC(0x470200)
void Ped::sub_470200(Fix16 a2, Fix16 a3, Fix16 a4)
{
    Ped::sub_45C830(a2, a3, a4);
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
        field_168_game_object->field_38 = dword_678438;
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

STUB_FUNC(0x470300)
void Ped::sub_470300()
{
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
            Car_B0* pB0 = pCar->field_58_uni_Car78_or_Car_B0;
            if (pB0)
            {
                pB0->field_8C = 1;
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
