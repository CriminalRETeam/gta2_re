#include "PlayerScoreTracker.hpp"
#include "hud_message_priority.hpp"
#include "voice_line.hpp"
#include "PlayerCrimeStats.hpp"
#include "ped_death_cause.hpp"
#include "accuracy_event.hpp"
#include "car_score_tier.hpp"
#include "emergency_car_kill_flag.hpp"
#include "car_wreck.hpp"
#include "car_model_flag.hpp"
#include "bonus_event_type.hpp"
#include "CarInfoTable.hpp"
#include "CarPhysics.hpp"
#include "Car.hpp"
#include "ExplodingScore_100.hpp"
#include "Frontend.hpp"
#include "Game.hpp"
#include "Hud.hpp"
#include "Globals.hpp"
#include "Player.hpp"
#include "CrimeReportQueue.hpp"
#include "Gang.hpp"
#include "debug.hpp"
#include "gtx_0x106C.hpp"
#include "GameSession.hpp"
#include "rng.hpp"
#include "root_sound.hpp"
#include "TextTable.hpp"
#include "Ped.inl"
#include <string.h>

EXTERN_GLOBAL(s32, bStartNetworkGame_7081F0);

DEFINE_GLOBAL_INIT(Fix16, kFlyCarMinVelocity_7028BC, Fix16(0x666, 0), 0x7028BC);


MATCH_FUNC(0x591bd0)
PlayerScoreTracker::PlayerScoreTracker()
{
    mPlayer = NULL;

    mCarKillCombo = 1;
    mScoreMult = 1;

    mOneSecondTimerMs = 0;
    mFlyCarTimeMs = 0;
    mLastCarKillTime = 0;
    mLastKillTime = 0;
    mExecutionCount = 0;
    mTotalKills = 0;
    mCopCarsDestroyed = 0;
    mCarsStolenCount = 0;
    mLastElvisKillTime = 0;
    mNumElvisKilled = 0;
    mLastAccuracyEvent = 0;
    mAccuracyCount = 0;
    mReverseTimeMs = 0;
    mLastEmergencyCarKillTime = 0;
    mKilledCarsFlags = emergency_car_kill_flag::none_0;

    for (s32 i = 0; i < GTA2_COUNTOF(mCarModelFlags); i++)
    {
        mCarModelFlags[i] = 0;
    }
}

MATCH_FUNC(0x591c70)
void PlayerScoreTracker::Service_591C70()
{
    mBonuses.Service_431E30();
    Ped* player_ped = mPlayer->GetPlayerPed_41D020();
    mOneSecondTimerMs += gGame_0x40_67E008->GetFrameDurationMs_4B8BB0();

    if (mOneSecondTimerMs >= 1000)
    {
        mOneSecondTimerMs -= 1000;
        Car* pCar = player_ped->get_car_416B60();

        if (pCar)
        {
            if (player_ped->not_enter_car_as_passenger_4A5040() && pCar->HasTaxiLight_4216C0())
            {
                if (!pCar->mPassengersList.IsEmpty_420EA0())
                {
                    mPlayer->AddScore_41DC40(1);
                }
            }
        }
        if (player_ped->get_wanted_points_433DC0() >= cop_level_ped_enum::cop_4_stars)
        {
            mPlayer->AddScore_41DC40(1);
        }

        mPlayer->mCrimeStats.AddEvasionRating_484FB0(player_ped->get_wanted_star_count_46EF00());
    }

    if (mExecutionCount >= 20u)
    {
        mExecutionCount = 0;
        mPlayer->AddScore_41DC40(100000);

        if (mPlayer->IsUser_41DC70())
        {
            gHud_2B00_706620->mMessage.ShowMessage_5D1A00(gText_0x14_704DFC->Find_5B5F90("excutin"), hud_message_priority::normal_1);
            gRoot_sound_66B038.PlayVoice_40F090(voice_line::expiditious_4);
        }
    }

    if (mNumElvisKilled >= 6u)
    {
        mNumElvisKilled = 0;
        mPlayer->AddScore_41DC40(30000);
        if (mPlayer->IsUser_41DC70())
        {
            gHud_2B00_706620->mMessage.ShowMessage_5D1A00(gText_0x14_704DFC->Find_5B5F90("elvis_d"), hud_message_priority::normal_1);
            gRoot_sound_66B038.PlayVoice_40F090(voice_line::elvis_8);
        }
    }

    if (mKilledCarsFlags == emergency_car_kill_flag::all_7)
    {
        mKilledCarsFlags = emergency_car_kill_flag::none_0;
        mPlayer->AddScore_41DC40(10000);
        if (mPlayer->IsUser_41DC70())
        {
            gHud_2B00_706620->mMessage.ShowMessage_5D1A00(gText_0x14_704DFC->Find_5B5F90("em_dest"), hud_message_priority::normal_1);
            gRoot_sound_66B038.PlayVoice_40F090(voice_line::medicalemer_11);
        }
    }

    if (mTotalKills >= 1000)
    {
        mTotalKills = 0;
        mPlayer->AddScore_41DC40(30000);
        if (mPlayer->IsUser_41DC70())
        {
            gHud_2B00_706620->mMessage.ShowMessage_5D1A00(gText_0x14_704DFC->Find_5B5F90("gencide"), hud_message_priority::normal_1);
            gRoot_sound_66B038.PlayVoice_40F090(voice_line::genocide_5);
        }
    }

    if (mCopCarsDestroyed >= 20u)
    {
        mCopCarsDestroyed = 0;
        mPlayer->AddScore_41DC40(5000);
        if (mPlayer->IsUser_41DC70())
        {
            gHud_2B00_706620->mMessage.ShowMessage_5D1A00(gText_0x14_704DFC->Find_5B5F90("copkill"), hud_message_priority::normal_1);
            gRoot_sound_66B038.PlayVoice_40F090(voice_line::copkilla_6);
        }
    }

    if (mCarsStolenCount >= 100)
    {
        mCarsStolenCount = 0;
        mPlayer->AddScore_41DC40(10000);
        if (mPlayer->IsUser_41DC70())
        {
            gHud_2B00_706620->mMessage.ShowMessage_5D1A00(gText_0x14_704DFC->Find_5B5F90("carjaka"), hud_message_priority::normal_1);
            gRoot_sound_66B038.PlayVoice_40F090(voice_line::carjacker_7);
        }
    }

    if (mAccuracyCount >= 25)
    {
        mAccuracyCount = 0;
        mPlayer->AddScore_41DC40(5000);
        if (mPlayer->IsUser_41DC70())
        {
            gHud_2B00_706620->mMessage.ShowMessage_5D1A00(gText_0x14_704DFC->Find_5B5F90("accurcy"), hud_message_priority::normal_1);
            gRoot_sound_66B038.PlayVoice_40F090(voice_line::accuracyb_9);
        }
    }

    Car* pReversingCar;
    if (player_ped->get_wanted_points_433DC0() > cop_level_ped_enum::cop_3_stars && (player_ped->has_car_403B80()) &&
        player_ped->not_enter_car_as_passenger_4A5040() &&
        (pReversingCar = player_ped->get_car_416B60()) != NULL && // already tested by has_car_403B80, so the compiler drops it
        (pReversingCar->mPhysics) != NULL &&

        pReversingCar->mPhysics->is_backward_gas_on_411810())
    {
        mReverseTimeMs += gGame_0x40_67E008->GetFrameDurationMs_4B8BB0();
    }
    else
    {
        mReverseTimeMs = 0;
    }

    if (mReverseTimeMs >= 60000u)
    {
        mReverseTimeMs = 0;
        mPlayer->AddScore_41DC40(1000);
        if (mPlayer->IsUser_41DC70())
        {
            gHud_2B00_706620->mMessage.ShowMessage_5D1A00(gText_0x14_704DFC->Find_5B5F90("wrngway"), hud_message_priority::normal_1);
            gRoot_sound_66B038.PlayVoice_40F090(voice_line::back2front_10);
        }
    }

    Car* pFlyingCar = player_ped->get_car_416B60();
    if (pFlyingCar && player_ped->not_enter_car_as_passenger_4A5040() && (pFlyingCar->mPhysics) != NULL &&
        pFlyingCar->mPhysics->IsInAir_55A0B0()
        && pFlyingCar->GetVelocity_43A4C0() > kFlyCarMinVelocity_7028BC)
    {
        mFlyCarTimeMs += gGame_0x40_67E008->GetFrameDurationMs_4B8BB0();
    }
    else
    {
        mFlyCarTimeMs = 0;
    }

    if (mFlyCarTimeMs >= 1250 && mFlyCarTimeMs < 2250)
    {
        mPlayer->AddScore_41DC40(1000);
        mFlyCarTimeMs = 2250;
        if (mPlayer->IsUser_41DC70())
        {
            gHud_2B00_706620->mMessage.ShowMessage_5D1A00(gText_0x14_704DFC->Find_5B5F90("fly_car"), hud_message_priority::normal_1);
            gRoot_sound_66B038.PlayVoice_40F090(voice_line::insanestunt_1);
        }
    }

    if (bStartNetworkGame_7081F0)
    {
        s32 frags_or_points;
        u8 player_idx = mPlayer->get_idx_4219D0();
        u8 gamemode = gGameSession_67E8E0.GetMultiplayerGamemode_4C5BC0();
        s32 points_limit = gGameSession_67E8E0.GetMultiplayerPointsLimit_4C5BD0();

        if (gamemode == FRAG_GAME_1) // di vs bl
        {
            s16 frags = gGameSession_67E8E0.GetFragsForPlayerIdx_4C5D60(player_idx);
            frags_or_points = frags;
            GetFrags_5935C0();
        }
        else if (gamemode == POINTS_GAME_2)
        {
            frags_or_points = gGameSession_67E8E0.GetPointsForPlayerIdx_4C5CB0(player_idx);
            GetScore_592370();
        }

        if (gamemode != TAG_GAME_3)
        {
            if (frags_or_points >= points_limit)
            {
                gGameSession_67E8E0.SetWinnerIdx_4C5C00(player_idx);
                if (!gGame_0x40_67E008->IsExitTimerSet_4B7590())
                {
                    gHud_2B00_706620->mMessage.ShowMessage_5D1A00(gText_0x14_704DFC->Find_5B5F90("g_over"), hud_message_priority::important_3);
                }
                gGame_0x40_67E008->ExitGameNoBonus_4B8C00(2, GameExitType::MultiplayerExit_5);
            }
        }
    }

    const s32 cur_rng = gpRng_67AB34->get_cur_rng_41CFE0();

    if (static_cast<u32>(gpRng_67AB34->get_cur_rng_41CFE0() - mLastCarKillTime) > 15)
    {
        mCarKillCombo = 1;
    }

    if (static_cast<u32>(cur_rng - mLastKillTime) > 15)
    {
        mScoreMult = 1;
    }
}

MATCH_FUNC(0x5922f0)
void PlayerScoreTracker::Init_5922F0(Player* pPlayer, s16 digit_transition_speed, s32 max_score_value, s16 palette, u16 max_frag_value)
{
    mPlayer = pPlayer;
    mMoney.SetupDigitsParams_492110(digit_transition_speed, max_score_value, palette);
    mMultiplayerFrags.SetupDigitsParams_492110(digit_transition_speed, max_frag_value, palette);
}

MATCH_FUNC(0x592330)
void PlayerScoreTracker::Reset_592330()
{
    mMoney.InitDigitSprites_492150();
    mMultiplayerFrags.InitDigitSprites_492150();
    mBonuses.Init_431E10(this);
    ResetCarModelFlags_592380(car_model_flag::both_3);
}

MATCH_FUNC(0x592360)
RollingDigitCounter* PlayerScoreTracker::GetScoreDigits_592360()
{
    return &mMoney;
}

MATCH_FUNC(0x592370)
s32 PlayerScoreTracker::GetScore_592370() const
{
    return mMoney.mValue;
}

MATCH_FUNC(0x592380)
void PlayerScoreTracker::ResetCarModelFlags_592380(char_type bits)
{
    if ((bits & car_model_flag::stolen_1) != 0)
    {
        for (u16 i = 0; i < GTA2_COUNTOF(mCarModelFlags); i++)
        {
            if (gGtx_0x106C_703DD4->does_car_exist(static_cast<u8>(i)) && gGtx_0x106C_703DD4->IsCarModelInRecycleList_5AB380(static_cast<u8>(i)))
            {
                mCarModelFlags[i] &= ~car_model_flag::stolen_1;
            }
            else
            {
                mCarModelFlags[i] |= car_model_flag::stolen_1;
            }
        }
    }

    if ((bits & car_model_flag::destroyed_2) != 0)
    {
        for (u16 i = 0; i < GTA2_COUNTOF(mCarModelFlags); i++)
        {
            if (gGtx_0x106C_703DD4->does_car_exist(static_cast<u8>(i)) && gGtx_0x106C_703DD4->IsCarModelInRecycleList_5AB380(static_cast<u8>(i)))
            {
                const u8 wreck = gGtx_0x106C_703DD4->get_car_info_5AA3B0(static_cast<u8>(i))->wreck;

                if (wreck == car_wreck::none_99)
                {
                    mCarModelFlags[i] |= car_model_flag::destroyed_2;
                }
                else
                {
                    mCarModelFlags[i] &= ~car_model_flag::destroyed_2;
                }
            }
            else
            {
                mCarModelFlags[i] |= car_model_flag::destroyed_2;
            }
        }
    }
}

MATCH_FUNC(0x592430)
void PlayerScoreTracker::CheckAllCarModelsFlagged_592430(char_type bits)
{
    u16 i;

    if ((bits & car_model_flag::stolen_1) != 0)
    {
        for (i = 0; i < GTA2_COUNTOF(mCarModelFlags); i++)
        {
            if ((mCarModelFlags[i] & car_model_flag::stolen_1) == 0)
            {
                return;
            }
        }

        mPlayer->AddScore_41DC40(30000);
        if (mPlayer->IsUser_41DC70())
        {
            gHud_2B00_706620->mMessage.ShowMessage_5D1A00(gText_0x14_704DFC->Find_5B5F90("stl_all"), hud_message_priority::normal_1);
            gRoot_sound_66B038.PlayVoice_40F090(voice_line::gta_2);
        }
        ResetCarModelFlags_592380(car_model_flag::stolen_1);
    }
    else if ((bits & car_model_flag::destroyed_2) != 0)
    {
        for (i = 0; i < GTA2_COUNTOF(mCarModelFlags); i++)
        {
            if ((mCarModelFlags[i] & car_model_flag::destroyed_2) == 0)
            {
                return;
            }
        }

        mPlayer->AddScore_41DC40(50000);
        if (mPlayer->IsUser_41DC70())
        {
            gHud_2B00_706620->mMessage.ShowMessage_5D1A00(gText_0x14_704DFC->Find_5B5F90("dst_all"), hud_message_priority::normal_1);
            gRoot_sound_66B038.PlayVoice_40F090(voice_line::wipeout_3);
        }
        ResetCarModelFlags_592380(car_model_flag::destroyed_2);
    }
}

MATCH_FUNC(0x592570)
void PlayerScoreTracker::SetCarModelFlag_592570(char_type flag, s32 car_model)
{
    mCarModelFlags[car_model] |= flag;
    CheckAllCarModelsFlagged_592430(flag);
}

MATCH_FUNC(0x5925b0)
s32 PlayerScoreTracker::GetCarScoreValue_5925B0(u32 car_model, u8 reward_tier)
{
    u32 result = gCarInfo_808_678098->GetModelPhysicsFromIdx_4546B0(car_model)->mValue;

    switch (reward_tier)
    {
        case car_score_tier::hijacked_0:
            return result;
        case car_score_tier::unused_x2_1:
            return result * 2;
        case car_score_tier::destroyed_2:
            return result * 5;
        default:
            return 0;
    }
}

MATCH_FUNC(0x592620)
void PlayerScoreTracker::AddCash_592620(s32 cash)
{
    mMoney.ChangeStatByAmount_4921B0(cash);

    if (bStartNetworkGame_7081F0)
    {
        gGameSession_67E8E0.ChangePointsForPlayerIdxByAmount_4C5C80(mPlayer->get_idx_4219D0(), cash);
    }
}

// Scores the player killing pVictim (pKiller is the killer's ped): points by occupation and kill
// type, exploding score, cash, and reports the crime
MATCH_FUNC(0x592660)
void PlayerScoreTracker::AwardPedKilledScore_592660(Ped* pVictim, Ped* pKiller)
{
    const s32 multiplier = mPlayer->get_multiplier_4766A0();
    gmp_map_zone* pZone = gMap_0x370_6F6268->GetFirstZoneAtPos_4DF6A0(pKiller->get_cam_x().ToInt(), pKiller->get_cam_y().ToInt());

    s16 gang_idx;
    if (pVictim->mpGang)
    {
        gang_idx = pVictim->mpGang->mGangIdx;
    }
    else if (pVictim->mDummyGang)
    {
        gang_idx = pVictim->mDummyGang->mGangIdx;
    }
    else
    {
        gang_idx = -1;
    }

    s32 killer_car_model;
    Car* pKillerCar = pKiller->get_car_416B60();
    if (pKillerCar)
    {
        killer_car_model = pKillerCar->mCarInfoIdx;
    }
    else
    {
        killer_car_model = car_model_enum::none;
    }
    mBonuses.ProcessBonusEvent_4320D0(bonus_event_type::ped_killed_0,
                             car_model_enum::none,
                             pVictim->get_occupation_403980(),
                             gang_idx,
                             pVictim->get_remap_433BA0(),
                             pVictim->mDeathCause,
                             killer_car_model,
                             pZone);

    s32 rng = gpRng_67AB34->get_cur_rng_41CFE0();
    if (static_cast<u32>(rng - mLastKillTime) > 15)
    {
        mExecutionCount = 1;
    }
    else
    {
        mExecutionCount++;
    }
    mTotalKills++;
    mLastKillTime = rng;

    u32 score = 0;
    bool bOtherGang = false;
    bool bHasGameObject = pVictim->mGameObject != NULL;
    bool bCop;
    bool bSwat;
    bool bFbi;
    bool bArmy;
    bool bGangA;
    bool bGangB;

    if (bStartNetworkGame_7081F0 && pVictim->PedTypeIs_45EDE0(ped_type::player_2) && pVictim->mPlayer)
    {
        switch (pVictim->mDeathCause)
        {
            case ped_death_cause::explosion_4:
            case ped_death_cause::projectile_default_9:
            case ped_death_cause::punched_10:
            case ped_death_cause::bullet_11:
            case ped_death_cause::bomb_12:
            case ped_death_cause::fire_13:
            case ped_death_cause::fire_hit_14:
            case ped_death_cause::grenade_15:
            case ped_death_cause::molotov_16:
            case ped_death_cause::rocket_bullet_17:
            case ped_death_cause::rocket_18:
            case ped_death_cause::shotgun_19:
            case ped_death_cause::burning_20:
                score = 2000;
                break;
            case ped_death_cause::run_over_1:
                score = 1000;
                break;
            case ped_death_cause::run_over_by_stolen_car_3:
                score = 10000;
                break;
            case ped_death_cause::electrocuted_2:
            case ped_death_cause::drowned_5:
                score = 5000;
                break;
        }
    }
    else
    {
        if (pVictim->mpGang && (!pKiller->mpGang || pKiller->mpGang != pVictim->mpGang))
        {
            bOtherGang = true;
        }
        if (pVictim->mDummyGang && (!pKiller->mpGang || pKiller->mpGang != pVictim->mDummyGang))
        {
            bOtherGang = true;
        }

        switch (pVictim->get_occupation_403980())
        {
            case ped_ocupation_enum::paramedic_23:
            case ped_ocupation_enum::police:
            case ped_ocupation_enum::walking_guard_29:
            case ped_ocupation_enum::roadblock_cop_37:
            case ped_ocupation_enum::fireman:
                bCop = true;
                bFbi = false;
                bSwat = false;
                bArmy = false;
                bGangA = false;
                bGangB = false;
                break;
            case ped_ocupation_enum::swat:
            case ped_ocupation_enum::walking_swat_guard_30:
                bSwat = true;
                bFbi = false;
                bArmy = false;
                bCop = false;
                bGangA = false;
                bGangB = false;
                break;
            case ped_ocupation_enum::fbi:
                bFbi = true;
                bSwat = false;
                bArmy = false;
                bCop = false;
                bGangA = false;
                bGangB = false;
                break;
            case ped_ocupation_enum::army_army:
            case ped_ocupation_enum::walking_army_guard_31:
            case ped_ocupation_enum::tank_driver:
            case ped_ocupation_enum::road_block_tank_man:
                bArmy = true;
                bFbi = false;
                bSwat = false;
                bCop = false;
                bGangA = false;
                bGangB = false;
                break;
            case ped_ocupation_enum::elvis:
            case ped_ocupation_enum::elvis_leader:
                bCop = false;
                bFbi = false;
                bSwat = false;
                bArmy = false;
                score = 100;
                break;
            case ped_ocupation_enum::mugger:
                bGangA = true;
                bCop = false;
                bFbi = false;
                bSwat = false;
                bArmy = false;
                bGangB = false;
                break;
            case ped_ocupation_enum::car_thief:
                bGangB = true;
                bCop = false;
                bFbi = false;
                bSwat = false;
                bArmy = false;
                bGangA = false;
                break;
            default:
                bCop = false;
                bFbi = false;
                bSwat = false;
                bArmy = false;
                bGangA = false;
                bGangB = false;
                break;
        }

        // Only the Elvis case has a score by now
        if (score)
        {
            if (static_cast<u32>(rng - mLastElvisKillTime) > 15)
            {
                mNumElvisKilled = 1;
            }
            else
            {
                mNumElvisKilled++;
            }
            mLastElvisKillTime = rng;
        }
        else
        {
            switch (pVictim->mDeathCause)
            {
                case ped_death_cause::projectile_default_9:
                case ped_death_cause::punched_10:
                case ped_death_cause::bullet_11:
                case ped_death_cause::bomb_12:
                case ped_death_cause::fire_13:
                case ped_death_cause::fire_hit_14:
                case ped_death_cause::grenade_15:
                case ped_death_cause::molotov_16:
                case ped_death_cause::rocket_bullet_17:
                case ped_death_cause::rocket_18:
                case ped_death_cause::shotgun_19:
                case ped_death_cause::burning_20:
                    if (bOtherGang)
                        score = 50;
                    else if (bCop)
                        score = 200;
                    else if (bArmy)
                        score = 500;
                    else if (bSwat)
                        score = 300;
                    else if (bFbi)
                        score = 400;
                    else if (bGangA)
                        score = 40;
                    else
                        score = bGangB ? 40 : 20;
                    break;
                case ped_death_cause::run_over_1:
                    if (bOtherGang)
                        score = 20;
                    else if (bCop)
                        score = 100;
                    else if (bArmy)
                        score = 250;
                    else if (bSwat)
                        score = 150;
                    else if (bFbi)
                        score = 200;
                    else if (bGangA)
                        score = 20;
                    else
                        score = bGangB ? 20 : 10;
                    break;
                case ped_death_cause::electrocuted_2:
                    if (bOtherGang)
                        score = 200;
                    else if (bCop)
                        score = 500;
                    else if (bArmy)
                        score = 1250;
                    else if (bSwat)
                        score = 750;
                    else if (bFbi)
                        score = 1000;
                    else if (bGangA)
                        score = 100;
                    else
                        score = bGangB ? 100 : 50;
                    break;
                case ped_death_cause::run_over_by_stolen_car_3:
                    if (bOtherGang)
                        score = 200;
                    else if (bCop)
                        score = 1000;
                    else if (bArmy)
                        score = 2500;
                    else if (bSwat)
                        score = 1500;
                    else if (bFbi)
                        score = 2000;
                    else if (bGangA)
                        score = 200;
                    else
                        score = bGangB ? 200 : 100;
                    break;
                case ped_death_cause::explosion_4:
                    score = 20;
                    break;
                case ped_death_cause::drowned_5:
                    score = 50;
                    break;
            }
        }
    }

    bool bAwardScore = true;
    if (bIsFrench_67D53C)
    {
        s32 occupation = pVictim->get_occupation_403980();
        if (occupation == ped_ocupation_enum::police || occupation == ped_ocupation_enum::walking_guard_29 || occupation == ped_ocupation_enum::roadblock_cop_37 || bFbi || bSwat || bArmy)
        {
            bAwardScore = false;
        }
    }

    if (score > 0)
    {
        u32 total = mScoreMult * score;
        if (!bExplodingScoresOff_67D4FB && bHasGameObject && bAwardScore && mPlayer->IsUser_41DC70())
        {
            gExplodingScorePool->PushScore_596890(pVictim->get_cam_x(), pVictim->get_cam_y(), pVictim->get_cam_z(), total * multiplier);
        }
        if (bAwardScore)
        {
            mPlayer->AddScore_41DC40(total);
        }
        if (mScoreMult < 5)
        {
            mScoreMult++;
        }
    }

    if (gCrimeReportQueue_67A4B8->ShouldReportPedCrime_485140(pVictim, mPlayer))
    {
        if (bOtherGang)
        {
            gCrimeReportQueue_67A4B8->ReportCrimeForPed(crime_stats_type::gang_members_killed_9, mPlayer->GetPlayerPed_4A5130());
        }
        else if (bCop || bArmy || bSwat || bFbi)
        {
            gCrimeReportQueue_67A4B8->ReportCrimeForPed(crime_stats_type::lawmen_killed_8, mPlayer->GetPlayerPed_4A5130());
        }
        else if (pVictim->mDeathCause == ped_death_cause::run_over_1 || pVictim->mDeathCause == ped_death_cause::run_over_by_stolen_car_3)
        {
            gCrimeReportQueue_67A4B8->ReportCrimeForPed(crime_stats_type::civilians_run_down_6, mPlayer->GetPlayerPed_4A5130());
        }
        else
        {
            gCrimeReportQueue_67A4B8->ReportCrimeForPed(crime_stats_type::civilians_murdered_7, mPlayer->GetPlayerPed_4A5130());
        }
    }
}

MATCH_FUNC(0x592dd0)
void PlayerScoreTracker::AwardCarDestroyedScore_592DD0(Car* pCar, Ped* pKiller)
{
    const s32 multiplier = mPlayer->get_multiplier_4766A0();
    gmp_map_zone* pZone = gMap_0x370_6F6268->GetFirstZoneAtPos_4DF6A0(pKiller->get_cam_x().ToInt(), pKiller->get_cam_y().ToInt());

    u32 killer_car_model = pKiller->get_car_model();

    u16 gang_idx = gGangPool_CA8_67E274->FindGangByCarModel_4BF2F0(pCar->mCarInfoIdx);

    mBonuses.ProcessBonusEvent_4320D0(bonus_event_type::car_destroyed_1,
                             pCar->mCarInfoIdx,
                             ped_ocupation_enum::no_occupation,
                             gang_idx,
                             pCar->mCarSprite->get_remap_41C1F0(),
                             pCar->mDeathCause,
                             killer_car_model,
                             pZone);

    bool bAwardScore = true;
    if (bIsFrench_67D53C)
    {
        if (pCar->IsPoliceCar_439EC0())
        {
            bAwardScore = false;
        }
    }

    s32 cur_rng = gpRng_67AB34->get_cur_rng_41CFE0();
    if (pCar->IsFireTruck_4118F0() || pCar->IsCopCar_421790() ||
        pCar->IsMediCar_421780() || pCar->IsSwatVan_4217A0() ||
        pCar->IsFbiCar_411920())
    {

        if (static_cast<u32>(gpRng_67AB34->get_cur_rng_41CFE0() - mLastEmergencyCarKillTime) > 150)
        {
            mKilledCarsFlags = emergency_car_kill_flag::none_0;
        }

        u32 destroyed_car_model = pCar->mCarInfoIdx;

        if (destroyed_car_model == car_model_enum::MEDICAR)
        {
            mKilledCarsFlags |= emergency_car_kill_flag::medicar_1;
        }
        else if (destroyed_car_model == car_model_enum::COPCAR || destroyed_car_model == car_model_enum::SWATVAN || destroyed_car_model == car_model_enum::EDSELFBI)
        {
            mKilledCarsFlags |= emergency_car_kill_flag::cop_car_2;
        }
        else if (destroyed_car_model == car_model_enum::FIRETRUK)
        {
            mKilledCarsFlags |= emergency_car_kill_flag::fire_truck_4;
        }

        mLastEmergencyCarKillTime = cur_rng;
    }
    if (pCar->IsCopCar_421790() && bAwardScore)
    {
        mCopCarsDestroyed++;
    }

    u32 car_score_value = GetCarScoreValue_5925B0(pCar->mCarInfoIdx, car_score_tier::destroyed_2);
    u8 combo = mCarKillCombo;

    u32 kill_car_score = car_score_value * combo;
    if (!bExplodingScoresOff_67D4FB)
    {
        if (bAwardScore)
        {
            if (mPlayer->IsUser_41DC70())
            {
                gExplodingScorePool->PushScore_596890(pCar->GetX_41E430(),
                                                       pCar->GetY_41E440(),
                                                       pCar->GetZ_41E450(),
                                                       multiplier * kill_car_score);
            }
        }
    }

    if (bAwardScore)
    {
        mPlayer->AddScore_41DC40(kill_car_score);
    }

    mLastCarKillTime = cur_rng;
    if (mCarKillCombo < 5u)
    {
        mCarKillCombo++;
    }

    if (gCrimeReportQueue_67A4B8->ShouldReportCarCrime_485090(pCar, this->mPlayer))
    {
        gCrimeReportQueue_67A4B8->ReportCrimeForPed(crime_stats_type::car_destroyed_3, mPlayer->GetPlayerPed_4A5130());
    }
    mPlayer->mCrimeStats.AddCarDamageCost_484FA0(multiplier * kill_car_score);
    SetCarModelFlag_592570(car_model_flag::destroyed_2, pCar->mCarInfoIdx);
}

MATCH_FUNC(0x593030)
void PlayerScoreTracker::AwardCarDamageScore_593030(Car* pCar, s16 damage)
{
    bool bAddScore = true;
    s32 multiplier = mPlayer->get_multiplier_4766A0();

    if (bIsFrench_67D53C)
    {
        if (pCar->IsPoliceCar_439EC0())
        {
            bAddScore = false;
        }
    }

    u32 damage_u = damage;
    if (damage_u > 0)
    {
        s32 base_score;
        if (damage_u < 300)
        {
            base_score = 1;
        }
        else
        {
            base_score = damage_u < 400 ? 10 : 100;
        }

        if (!bExplodingScoresOff_67D4FB)
        {
            if (bAddScore)
            {
                if (this->mPlayer->IsUser_41DC70())
                {
                    gExplodingScorePool->PushScore_596890(pCar->GetX_41E430(),
                                                           pCar->GetY_41E440(),
                                                           pCar->GetZ_41E450(),
                                                           multiplier * base_score);
                }
            }
        }

        if (bAddScore)
        {
            mPlayer->AddScore_41DC40(base_score);
        }

        mPlayer->mCrimeStats.AddCarDamageCost_484FA0(multiplier * base_score);
        if (gCrimeReportQueue_67A4B8->ShouldReportCarCrime_485090(pCar, mPlayer))
        {
            gCrimeReportQueue_67A4B8->ReportCrimeForPed(crime_stats_type::car_damaged_1, mPlayer->GetPlayerPed_4A5130());
        }
    }
}

MATCH_FUNC(0x593150)
void PlayerScoreTracker::AwardCarDamageScoreHit_593150(Car* pCar, s16 damage)
{
    if (!pCar->IsMaxDamage_40F890())
    {
        const s32 multiplier = mPlayer->get_multiplier_4766A0();
        u32 damage_u = damage;
        if (damage_u > 0)
        {
            s32 base_score;
            if (damage_u < 300)
            {
                base_score = 1;
            }
            else
            {
                base_score = damage_u < 400 ? 10 : 100;
            }
            if (!bIsFrench_67D53C || !pCar->IsPoliceCar_439EC0())
            {
                mPlayer->AddScore_41DC40(base_score);
            }
            mPlayer->mCrimeStats.AddCarDamageCost_484FA0(multiplier * base_score);

            gCrimeReportQueue_67A4B8->ReportCrimeForPed(crime_stats_type::car_damaged_1, mPlayer->GetPlayerPed_4A5130());
        }
    }
}

MATCH_FUNC(0x593220)
void PlayerScoreTracker::AddCashWithMultiplier_593220()
{
    mPlayer->AddScore_41DC40(20);
}

MATCH_FUNC(0x593240)
void PlayerScoreTracker::AwardCarHijackedScore_593240(Car* pCar)
{
    const s32 multiplier = mPlayer->get_multiplier_4766A0();
    gmp_map_zone* pZone = gMap_0x370_6F6268->GetFirstZoneAtPos_4DF6A0(mPlayer->mPlayerPed->get_cam_x().ToInt(),
                                                           mPlayer->mPlayerPed->get_cam_y().ToInt());

    const u16 gang_idx = gGangPool_CA8_67E274->FindGangByCarModel_4BF2F0(pCar->mCarInfoIdx);
    mBonuses.ProcessBonusEvent_4320D0(bonus_event_type::car_hijacked_2, pCar->mCarInfoIdx, ped_ocupation_enum::no_occupation, gang_idx, pCar->mCarSprite->get_remap_41C1F0(), ped_death_cause::any_23, car_model_enum::none, pZone);

    mCarsStolenCount++;

    const s32 base_score = GetCarScoreValue_5925B0(pCar->mCarInfoIdx, car_score_tier::hijacked_0);
    if (!bExplodingScoresOff_67D4FB && mPlayer->IsUser_41DC70())
    {
        gExplodingScorePool->PushScore_596890(pCar->GetX_41E430(),
                                               pCar->GetY_41E440(),
                                               pCar->GetZ_41E450(),
                                               multiplier * base_score);
    }
    mPlayer->AddScore_41DC40(base_score);

    gCrimeReportQueue_67A4B8->ReportCrimeForPed(crime_stats_type::vehicles_hijacked_5, mPlayer->GetPlayerPed_4A5130());
    SetCarModelFlag_592570(car_model_flag::stolen_1, pCar->mCarInfoIdx);
}

MATCH_FUNC(0x593370)
void PlayerScoreTracker::AwardBusStolenScore_593370(Car* pCar)
{
    if (!bExplodingScoresOff_67D4FB && mPlayer->IsUser_41DC70())
    {
        gExplodingScorePool->PushScore_596890(pCar->GetX_41E430(),
                                               pCar->GetY_41E440(),
                                               pCar->GetZ_41E450(),
                                               mPlayer->get_multiplier_4766A0() * 10);
    }

    mPlayer->AddScore_41DC40(10);
    gCrimeReportQueue_67A4B8->ReportCrimeForPed(crime_stats_type::bus_stolen_4, mPlayer->GetPlayerPed_4A5130());
}

MATCH_FUNC(0x593410)
void PlayerScoreTracker::AwardFullBusDestroyedScore_593410(Car* pCar)
{
    const s32 multiplier = mPlayer->get_multiplier_4766A0();
    if (!bExplodingScoresOff_67D4FB)
    {
        if (mPlayer->IsUser_41DC70())
        {
            gExplodingScorePool->PushScore_596890(pCar->GetX_41E430(),
                                                   pCar->GetY_41E440(),
                                                   pCar->GetZ_41E450(),
                                                   multiplier * 100);
        }
    }

    mPlayer->AddScore_41DC40(100);
    mPlayer->mCrimeStats.AddCarDamageCost_484FA0(multiplier * 100);

    if (gCrimeReportQueue_67A4B8->ShouldReportCarCrime_485090(pCar, mPlayer))
    {
        gCrimeReportQueue_67A4B8->ReportCrimeForPed(crime_stats_type::car_destroyed_3, mPlayer->GetPlayerPed_4A5130());
    }
}

MATCH_FUNC(0x5934f0)
void PlayerScoreTracker::UpdateAccuracyCount_5934F0(u32 event, s32 weapon_model, Ped* pTarget)
{
    mLastAccuracyEvent = event;
    if (pTarget && bIsFrench_67D53C)
    {
        switch (pTarget->get_occupation_403980())
        {
            case ped_ocupation_enum::police:
            case ped_ocupation_enum::swat:
            case ped_ocupation_enum::fbi:
            case ped_ocupation_enum::army_army:
            case ped_ocupation_enum::walking_guard_29:
            case ped_ocupation_enum::walking_swat_guard_30:
            case ped_ocupation_enum::walking_army_guard_31:
            case ped_ocupation_enum::tank_driver:
            case ped_ocupation_enum::roadblock_cop_37:
            case ped_ocupation_enum::road_block_tank_man:
                mAccuracyCount = 0;
                return;
            default:
                break;
        }
    }

    if (weapon_model == objects::fire_hitting_194 || weapon_model == objects::water_hitting_198 ||
        weapon_model == objects::invisible_bullet_154 || weapon_model == objects::invisible_shotgun_bullet_193 || weapon_model == objects::flamethrower_bullet_195 ||
        weapon_model == objects::invisible_rocket_159 || weapon_model == objects::fire_truck_gun_bullet_199 || event == accuracy_event::fire_ended_0 ||
        event == accuracy_event::weapon_hit_1 || event == accuracy_event::hit_other_ped_3)
    {
        mAccuracyCount = 0;
    }
    else if (event == accuracy_event::hit_threat_ped_2)
    {
        mAccuracyCount++;
    }
}

MATCH_FUNC(0x5935b0)
RollingDigitCounter* PlayerScoreTracker::GetMultiplayerFragDigits_5935B0()
{
    return &mMultiplayerFrags;
}

MATCH_FUNC(0x5935c0)
s32 PlayerScoreTracker::GetFrags_5935C0() const
{
    return mMultiplayerFrags.mValue;
}

MATCH_FUNC(0x5935d0)
void PlayerScoreTracker::ChangeFragsByAmount_5935D0(s32 amount)
{
    mMultiplayerFrags.ChangeStatByAmount_4921B0(amount);
}