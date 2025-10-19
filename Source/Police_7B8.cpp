#include "Police_7B8.hpp"
#include "Car_BC.hpp"
#include "Globals.hpp"
#include "Kfc_1E0.hpp"
#include "Object_5C.hpp"
#include "Game_0x40.hpp"
#include "Player.hpp"
#include "Ped.hpp"
#include "winmain.hpp"

DEFINE_GLOBAL(s32, gRoadblockGuardType_6FEDB8, 0x6FEDB8);
DEFINE_GLOBAL_INIT(Fix16, dword_6FECE8, Fix16(0), 0x6FECE8);
DEFINE_GLOBAL(u8, byte_6FEE44, 0x6FEE44);

MATCH_FUNC(0x4BEB50)
Police_7B8::~Police_7B8()
{

}

MATCH_FUNC(0x56f400)
void Police_7B8::sub_56F400()
{
    field_0 = 1;
    for (s32 i = 0; i < 4; i++)
    {
        field_464[i].field_0 = 0;
        field_464[i].field_C = 0;
        field_464[i].field_1C = 0;

        field_464[i].field_70 = 0;
        field_464[i].field_71 = 0;
        field_464[i].field_72 = 0;
        field_464[i].field_73 = 0;
        field_464[i].field_74 = 0;
        field_464[i].field_76 = 0;

        field_464[i].field_4 = 0;
        field_464[i].field_8 = 0;

        field_464[i].field_10_x = dword_6FECE8;
        field_464[i].field_14_y = dword_6FECE8;
        field_464[i].field_18_z = dword_6FECE8;

        field_464[i].field_1C = 0;
        field_464[i].field_E = 0;

        field_464[i].field_75_count = 0;
        field_464[i].field_78 = 0;
        field_464[i].field_7A_kMax900 = 0;

        memset(field_464[i].field_20, 0, 0x18);
    }
    field_654_wanted_level = 0;
    field_658 = 0;
    field_659 = 1;
    field_65C = 3;
    byte_6FEE44 = 0;
    if (bStartNetworkGame_7081F0)
    {
        field_660_wanted_star_count = 1;
    }
    else
    {
        field_660_wanted_star_count = 6;
    }
    field_7AC = 100;
    field_7AD_police_peds_in_range_screen = 0;
    field_7B0 = 0;
    field_7B4 = 0;
}

STUB_FUNC(0x56f4d0)
char_type Police_7B8::sub_56F4D0(Ped* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x56f560)
Police_38* Police_7B8::New_56F560()
{
    for (u8 i = 0; i < 20; i++)
    {
        if (!field_4[i].field_1C_used)
        {
            Police_38* pNew = &field_4[i];
            pNew->Init_5709C0();
            return pNew;
        }
    }
    return NULL;
}

MATCH_FUNC(0x56f5c0)
Ped* Police_7B8::SpawnRoadblockGuard_56F5C0(Fix16 xpos, Fix16 ypos, Fix16 zpos, Ang16 rotation)
{
    Ped* pCop = NULL;

    if (gChar_C_6787BC->field_5 >= 30)
    {
        return NULL;
    }

    switch (gRoadblockGuardType_6FEDB8)
    {
        case 3:
            pCop = gChar_C_6787BC->SpawnPedAt(xpos, ypos, zpos, 0, rotation);
            pCop->field_238 = 4;
            pCop->field_240_occupation = ped_ocupation_enum::unknown_17;
            pCop->SetObjective(objectives_enum::guard_spot_24, 0);
            pCop->field_244_remap = 8;
            pCop->field_26C_graphic_type = 1;
            pCop->ForceWeapon_46F600(weapon_type::silence_smg);
            pCop->field_216_health = 200;
            pCop->field_288_threat_search = threat_search_enum::area_2;
            pCop->field_28C_threat_reaction = threat_reaction_enum::react_as_emergency_1;
            break;
        case 1:
            pCop = gChar_C_6787BC->SpawnPedAt(xpos, ypos, zpos, 0, rotation);
            pCop->field_238 = 4;
            pCop->field_240_occupation = ped_ocupation_enum::unknown_17;
            pCop->SetObjective(objectives_enum::guard_spot_24, 0);
            pCop->field_244_remap = 0;
            pCop->field_26C_graphic_type = 2;
            pCop->field_170_selected_weapon = 0;
            pCop->GiveWeapon_46F650(weapon_type::pistol);
            pCop->field_216_health = 200;
            pCop->field_288_threat_search = threat_search_enum::area_2;
            pCop->field_28C_threat_reaction = threat_reaction_enum::react_as_emergency_1;
            break;
    }
    return pCop;
}

STUB_FUNC(0x56f6d0)
void Police_7B8::sub_56F6D0(Car_BC* a2)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x56f800)
bool Police_7B8::sub_56F800(Ped* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x56f880)
bool Police_7B8::sub_56F880(Ped* a2)
{
    for (u8 i = 0; i < 4; i++)
    {
        if (field_464[i].field_0 == a2)
        {
            if (field_464[i].field_78)
            {
                return true;
            }
            return false;
        }
    }
    return false;
}

MATCH_FUNC(0x56f8e0)
void Police_7B8::sub_56F8E0(Ped* a2, Ped* a3)
{
    for (u8 i = 0; i < 4; i++)
    {
        if (field_464[i].field_0 == a2)
        {
            field_464[i].field_0 = a2;
            return;
        }
    }
}

STUB_FUNC(0x56f940)
s32 Police_7B8::sub_56F940(u32* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x56fa40)
s16 Police_7B8::sub_56FA40()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x56faa0)
char_type Police_7B8::sub_56FAA0(Police_7C* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x56fbd0)
void Police_7B8::sub_56FBD0()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x570270)
void Police_7B8::sub_570270()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x570320)
s32 Police_7B8::sub_570320(Ped* a2, s32 a3, s32 a4, s32 a5, s16 a6)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5703e0)
char_type Police_7B8::sub_5703E0(Car_BC* a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5707b0)
char_type Police_7B8::sub_5707B0(s32 a2, s32 a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5708c0)
s32 Police_7B8::sub_5708C0(Police_7B8* a1, s32 a2, u32* a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x570940)
void Police_7B8::sub_570940(Ped* a3)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x577320)
char_type Police_7B8::sub_577320()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x577370)
void Police_7B8::sub_577370(u8 a2, s32 a3, s32 a4)
{
    NOT_IMPLEMENTED;
}