#include "Shooey_CC.hpp"
#include "Globals.hpp"
#include "Ped.hpp"
#include "char.hpp"
#include "Player.hpp"
#include "Police_7B8.hpp"

EXPORT_VAR s32 dword_67A370;
GLOBAL(dword_67A370, 0x67A370);

MATCH_FUNC(0x484cb0)
Shooey_14::Shooey_14()
{
    field_0_crime_type = 0;
    field_4_ped_id = 0;
    field_8_pos.x = dword_67A370;
    field_8_pos.y = dword_67A370;
    field_8_pos.z = dword_67A370;
}

MATCH_FUNC(0x484ce0)
Shooey_14::~Shooey_14()
{
}

STUB_FUNC(0x484cf0)
void Shooey_14::ReportCrimeForPedAtLocation(s32 crime_type, s32 ped_id)
{
    field_0_crime_type = crime_type;
    field_4_ped_id = ped_id;
    if (!ped_id)
    {
        field_8_pos.x = dword_67A370;
        field_8_pos.y = dword_67A370;
        field_8_pos.z = dword_67A370;
    }
    else
    {
        Ped* pPed = gChar_C_6787BC->PedById(ped_id);
        field_8_pos.x = pPed->field_1AC_cam.x;
        field_8_pos.y = pPed->field_1AC_cam.y; // TODO: Regswap
        field_8_pos.z = pPed->field_1AC_cam.z;
    }
}

MATCH_FUNC(0x484d50)
void Shooey_14::GetCrimeTypeAndLocation(s32* pCrimeType, Fix16* pXPos, Fix16* yPos, Fix16* zPos)
{
    *pCrimeType = field_0_crime_type;
    *pXPos = field_8_pos.x;
    *yPos = field_8_pos.y;
    *zPos = field_8_pos.z;
}

MATCH_FUNC(0x484d80)
Shooey_CC::Shooey_CC()
{
    field_0 = 0;
    field_2 = 0;
}

MATCH_FUNC(0x484db0)
Shooey_CC::~Shooey_CC()
{
}

MATCH_FUNC(0x484dd0)
void Shooey_CC::ReportCrime(s32 crime_type, s32 ped_id)
{
    field_4[field_0].ReportCrimeForPedAtLocation(crime_type, ped_id);

    field_0++;

    if (field_0 >= GTA2_COUNTOF(field_4))
    {
        field_0 = 0;
    }

    if (field_0 == field_2)
    {
        field_2++;
        if (field_2 >= GTA2_COUNTOF(field_4))
        {
            field_2 = 0;
        }
    }
}

STUB_FUNC(0x484e20)
bool Shooey_CC::GetLatestReportedCrime(s32* pCrimeType, s32* pXPos, s32* pYPos, u32* pZPos)
{
    return 0;
}

STUB_FUNC(0x484e90)
char_type Shooey_CC::CanReportCrime(s32 crime_type)
{
    u16 idx = this->field_2;
    if (idx == this->field_0)
    {
        return 0;
    }
    while (this->field_4[idx].field_0_crime_type != crime_type)
    {
        if (++idx >= 10u)
        {
            idx = 0;
        }
        if (idx == this->field_0)
        {
            return 0;
        }
    }
    return 1;
}

STUB_FUNC(0x484fc0)
Shooey_CC* Shooey_CC::ctor_484FC0()
{
    return 0;
}

STUB_FUNC(0x484fd0)
void Shooey_CC::dtor_484FD0()
{
}

// TODO: Just wrong case order I think
STUB_FUNC(0x484fe0)
void Shooey_CC::ReportCrimeForPed(u32 crime_type, Ped* pPed)
{
    switch (pPed->field_240_occupation)
    {
        case ped_ocupation_enum::police:
        case ped_ocupation_enum::swat:
        case ped_ocupation_enum::fbi:
        case ped_ocupation_enum::army_army:
        case ped_ocupation_enum::unknown_14:
        case ped_ocupation_enum::unknown_15:
        case ped_ocupation_enum::unknown_16:
        case ped_ocupation_enum::unknown_17:
            // Feds are allowed to do crime
            return;

        default:
        {
            if (crime_type <= 2)
            {
                if (!CanReportCrime(crime_type))
                {
                    ReportCrime(crime_type, pPed->field_200_id);
                }
            }
            else
            {
                pPed->sub_45B550();
                ReportCrime(crime_type, pPed->field_200_id);
                if (pPed->field_15C_player)
                {
                    gPolice_7B8_6FEE40->sub_570940(pPed);
                }
            }

            Player* pPlayer = pPed->field_15C_player;
            if (pPlayer)
            {
                pPlayer->field_644_unk.report_crime(crime_type);
            }

            break;
        }
    }
}

STUB_FUNC(0x485090)
bool Shooey_CC::sub_485090(Car_BC* a2, Player* a3)
{
    return 0;
}

STUB_FUNC(0x4850f0)
char_type Shooey_CC::sub_4850F0(Char_B4* a2, Player* a3)
{
    return 0;
}

MATCH_FUNC(0x485140)
char_type Shooey_CC::sub_485140(Ped* pPed, Player* pPlayer)
{
    Char_B4* pB4 = pPed->field_168_game_object;
    if (pB4)
    {
        return sub_4850F0(pB4, pPlayer);
    }
    else
    {
        return sub_485090(pPed->field_16C_car, pPlayer);
    }
}
