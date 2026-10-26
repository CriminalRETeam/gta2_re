#include "CrimeReportQueue.hpp"
#include "PlayerCrimeStats.hpp"
#include "Char_Pool.hpp"
#include "Game.hpp"
#include "Globals.hpp"
#include "Ped.hpp"
#include "Player.hpp"
#include "Police.hpp"
#include "char.hpp"

DEFINE_GLOBAL(CrimeReportQueue*, gCrimeReportQueue_67A4B8, 0x67A4B8);

DEFINE_GLOBAL_INIT(Fix16, kFP16Zero_67A370, Fix16(0), 0x67A370);

EXTERN_GLOBAL(u8, gCharB4_HitByMine_6FDB59);


MATCH_FUNC(0x484cb0)
CrimeReport::CrimeReport()
{
    field_0_crime_type = crime_stats_type::none_0;
    field_4_ped_id = 0;
    field_8_pos.x = kFP16Zero_67A370;
    field_8_pos.y = kFP16Zero_67A370;
    field_8_pos.z = kFP16Zero_67A370;
}

MATCH_FUNC(0x484ce0)
CrimeReport::~CrimeReport()
{
}

MATCH_FUNC(0x484cf0)
void CrimeReport::SetReport(s32 crime_type, s32 ped_id)
{
    field_0_crime_type = crime_type;
    field_4_ped_id = ped_id;
    if (!ped_id)
    {
        field_8_pos.x = kFP16Zero_67A370;
        field_8_pos.y = kFP16Zero_67A370;
        field_8_pos.z = kFP16Zero_67A370;
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
void CrimeReport::GetReport(s32* pCrimeType, Fix16* pXPos, Fix16* pYPos, Fix16* pZPos) const
{
    *pCrimeType = field_0_crime_type;
    *pXPos = field_8_pos.x;
    *pYPos = field_8_pos.y;
    *pZPos = field_8_pos.z;
}

MATCH_FUNC(0x484d80)
CrimeReportQueue::CrimeReportQueue()
{
    field_0_write_idx = 0;
    field_2_read_idx = 0;
}

MATCH_FUNC(0x484db0)
CrimeReportQueue::~CrimeReportQueue()
{
}

MATCH_FUNC(0x484dd0)
void CrimeReportQueue::QueueReport(s32 crime_type, s32 ped_id)
{
    field_4_reports[field_0_write_idx].SetReport(crime_type, ped_id);

    field_0_write_idx++;

    if (field_0_write_idx >= GTA2_COUNTOF(field_4_reports))
    {
        field_0_write_idx = 0;
    }

    if (field_0_write_idx == field_2_read_idx)
    {
        field_2_read_idx++;
        if (field_2_read_idx >= GTA2_COUNTOF(field_4_reports))
        {
            field_2_read_idx = 0;
        }
    }
}

MATCH_FUNC(0x484e20)
bool CrimeReportQueue::TryPopOldestReport(s32* pCrimeType, Fix16* pXPos, Fix16* pYPos, Fix16* pZPos)
{
    field_4_reports[field_2_read_idx].GetReport(pCrimeType, pXPos, pYPos, pZPos);

    field_4_reports[field_2_read_idx].SetReport(crime_stats_type::none_0, 0);

    if (field_2_read_idx != field_0_write_idx)
    {
        field_2_read_idx++;
        if (field_2_read_idx >= GTA2_COUNTOF(field_4_reports))
        {
            field_2_read_idx = 0;
        }
    }

    return *pCrimeType != crime_stats_type::none_0 ? true : false;
}

MATCH_FUNC(0x484e90)
bool CrimeReportQueue::IsCrimeQueued(s32 crime_type) const
{
    u16 idx = field_2_read_idx;
    while (idx != field_0_write_idx)
    {
        if (field_4_reports[idx].field_0_crime_type == crime_type)
        {
            return true;
        }

        if (++idx >= GTA2_COUNTOF(field_4_reports))
        {
            idx = 0;
        }
    }
    return false;
}

MATCH_FUNC(0x484fc0)
CrimeReportQueue_CC_Sub::CrimeReportQueue_CC_Sub()
{
}

MATCH_FUNC(0x484fd0)
CrimeReportQueue_CC_Sub::~CrimeReportQueue_CC_Sub()
{
}

// https://decomp.me/scratch/0XcCw
MATCH_FUNC(0x484fe0)
void CrimeReportQueue::ReportCrimeForPed(u32 crime_type, Ped* pPed)
{
    switch (pPed->get_occupation_403980())
    {
        case ped_ocupation_enum::police:
        case ped_ocupation_enum::swat:
        case ped_ocupation_enum::fbi:
        case ped_ocupation_enum::army_army:
        case ped_ocupation_enum::walking_guard_29:
        case ped_ocupation_enum::walking_swat_guard_30:
        case ped_ocupation_enum::walking_army_guard_31:
        case ped_ocupation_enum::roadblock_cop_37:
            // Feds are allowed to do crime
            return;

        default:
        {
            bool bQueueOnce = false;
            switch (crime_type)
            {

                case crime_stats_type::none_0:
                case crime_stats_type::car_damaged_1:
                case crime_stats_type::weapon_fired_2:
                    bQueueOnce = true;
                    break;

                default:
                    pPed->SetRecentCrimeTimer_45B550();
                    QueueReport(crime_type, pPed->field_200_id);
                    if (pPed->is_player_41B0A0())
                    {
                        gPolice_7B8_6FEE40->UpdateCriminalLatestPosition_570940(pPed);
                    }
                    break;
            }

            if (bQueueOnce)
            {
                if (!IsCrimeQueued(crime_type))
                {
                    QueueReport(crime_type, pPed->field_200_id);
                }
            }

            Player* pPlayer = pPed->field_15C_player;
            if (pPlayer)
            {
                pPlayer->field_644_crime_stats.IncrementCrimeCount_484F50(crime_type);
            }

            break;
        }
    }
}

// https://decomp.me/scratch/xN2BK
MATCH_FUNC(0x485090)
bool CrimeReportQueue::ShouldReportCarCrime_485090(Car* pCar, Player* pPlayer)
{
    bool bInRange = true;
    if (gCarManager_6C_677930->field_68_object_hit_active)
    {
        if (!gGame_0x40_67E008->IsSpriteInPlayerBounds_4B9950(pCar->field_50_car_sprite, pPlayer->GetIdx_4881E0(), 0))
        {
            bInRange = false;
        }
    }

    if (pCar->IsBeingCrushed_43DD50())
    {
        bInRange = false;
    }

    return bInRange;
}

// https://decomp.me/scratch/KvTvv
MATCH_FUNC(0x4850f0)
bool CrimeReportQueue::ShouldReportCharCrime_4850F0(Char* pChar, Player* pPlayer)
{
    bool bInRange = true;
    if (gCharB4_HitByMine_6FDB59)
    {
        if (!gGame_0x40_67E008->IsSpriteInPlayerBounds_4B9950(pChar->field_80_sprite_ptr, pPlayer->GetIdx_4881E0(), 0))
        {
            bInRange = false;
        }
    }
    return bInRange;
}

MATCH_FUNC(0x485140)
bool CrimeReportQueue::ShouldReportPedCrime_485140(Ped* pPed, Player* pPlayer)
{
    Char* pChar = pPed->field_168_game_object;
    if (pChar)
    {
        return ShouldReportCharCrime_4850F0(pChar, pPlayer);
    }
    else
    {
        return ShouldReportCarCrime_485090(pPed->field_16C_car, pPlayer);
    }
}