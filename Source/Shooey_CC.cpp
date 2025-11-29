#include "Shooey_CC.hpp"
#include "Game_0x40.hpp"
#include "Globals.hpp"
#include "Ped.hpp"
#include "Player.hpp"
#include "Police_7B8.hpp"
#include "char.hpp"

DEFINE_GLOBAL(Shooey_CC*, gShooey_CC_67A4B8, 0x67A4B8);

DEFINE_GLOBAL_INIT(Fix16, dword_67A370, Fix16(0), 0x67A370);

EXTERN_GLOBAL(u8, byte_6FDB59);


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

MATCH_FUNC(0x484cf0)
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
        Ped* pPed = gPedManager_6787BC->PedById(ped_id);
        field_8_pos.x = pPed->get_cam_x();
        field_8_pos.y = pPed->get_cam_y();
        field_8_pos.z = pPed->get_cam_z();
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
    field_0_idx = 0;
    field_2_report_count = 0;
}

MATCH_FUNC(0x484db0)
Shooey_CC::~Shooey_CC()
{
}

MATCH_FUNC(0x484dd0)
void Shooey_CC::ReportCrime(s32 crime_type, s32 ped_id)
{
    field_4_crimes[field_0_idx].ReportCrimeForPedAtLocation(crime_type, ped_id);

    field_0_idx++;

    if (field_0_idx >= GTA2_COUNTOF(field_4_crimes))
    {
        field_0_idx = 0;
    }

    if (field_0_idx == field_2_report_count)
    {
        field_2_report_count++;
        if (field_2_report_count >= GTA2_COUNTOF(field_4_crimes))
        {
            field_2_report_count = 0;
        }
    }
}

MATCH_FUNC(0x484e20)
bool Shooey_CC::GetLatestReportedCrime(s32* pCrimeType, Fix16* pXPos, Fix16* pYPos, Fix16* pZPos)
{
    // Get it
    field_4_crimes[field_2_report_count].GetCrimeTypeAndLocation(pCrimeType, pXPos, pYPos, pZPos);

    // But then also clear it?
    field_4_crimes[field_2_report_count].ReportCrimeForPedAtLocation(0, 0);

    // Tick the count
    if (field_2_report_count != field_0_idx)
    {
        field_2_report_count++;
        if (field_2_report_count >= GTA2_COUNTOF(field_4_crimes))
        {
            field_2_report_count = 0;
        }
    }

    // Did we fill in the info?
    return *pCrimeType != 0 ? true : false;
}

MATCH_FUNC(0x484e90)
char_type Shooey_CC::CanReportCrime(s32 crime_type)
{
    // Circular loop around
    u16 idx = field_2_report_count;
    while (idx != field_0_idx)
    {
        if (field_4_crimes[idx].field_0_crime_type == crime_type)
        {
            return 1;
        }

        if (++idx >= GTA2_COUNTOF(field_4_crimes))
        {
            idx = 0;
        }
    }
    return 0;
}

STUB_FUNC(0x484fc0)
Shooey_CC* Shooey_CC::ctor_484FC0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x484fd0)
void Shooey_CC::dtor_484FD0()
{
    NOT_IMPLEMENTED;
}

// https://decomp.me/scratch/0XcCw
MATCH_FUNC(0x484fe0)
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
            bool doit = false;
            switch (crime_type)
            {

                case 0:
                case 1:
                case 2:
                    doit = true;
                    break;

                default:
                    pPed->sub_45B550();
                    ReportCrime(crime_type, pPed->field_200_id);
                    if (pPed->field_15C_player)
                    {
                        gPolice_7B8_6FEE40->sub_570940(pPed);
                    }
                    break;
            }

            if (doit)
            {
                if (!CanReportCrime(crime_type))
                {
                    ReportCrime(crime_type, pPed->field_200_id);
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

// https://decomp.me/scratch/xN2BK
MATCH_FUNC(0x485090)
bool Shooey_CC::sub_485090(Car_BC* pCar, Player* pPlayer)
{
    bool bInRange = true;
    if (gCar_6C_677930->field_68)
    {
        if (gGame_0x40_67E008->sub_4B9950(pCar->field_50_car_sprite, pPlayer->GetIdx_4881E0(), 0) == 0)
        {
            bInRange = false;
        }
    }

    if (pCar->sub_43DD50())
    {
        bInRange = false;
    }

    return bInRange;
}

// https://decomp.me/scratch/KvTvv
MATCH_FUNC(0x4850f0)
char_type Shooey_CC::sub_4850F0(Char_B4* pB4, Player* pPlayer)
{
    bool result = true;
    if (byte_6FDB59)
    {
        if (gGame_0x40_67E008->sub_4B9950(pB4->field_80_sprite_ptr, pPlayer->GetIdx_4881E0(), 0) == 0)
        {
            result = false;
        }
    }
    return result;
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