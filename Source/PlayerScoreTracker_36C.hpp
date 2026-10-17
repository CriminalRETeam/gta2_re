#pragma once

#include "Function.hpp"
#include "BonusTracker_1C0.hpp"
#include "RollingDigitCounter_38.hpp"

class Player;
class Car_BC;
class Ped;

class PlayerScoreTracker_36C
{
  public:
    // 9.6f 0x4A50E0
    inline void SetMoney_4A50E0(s32 money)
    {
        field_0_money.SetValueClamped_4A50B0(money);
    }

    // 9.6f 0x45B0A0
    inline u8 get_accuracy_count_45B0A0()
    {
        return field_198_accuracy_count;
    }

    // 9.6f 0x45B0B0
    inline s32 get_reverse_count_45B0B0()
    {
        return field_19C_reverse_time_ms;
    }

    EXPORT PlayerScoreTracker_36C();
    EXPORT void Service_591C70();
    EXPORT void Init_5922F0(Player* pPlayer, s16 digit_transition_speed, s32 max_score_value, s16 palette, u16 max_frag_value);
    EXPORT void Reset_592330();
    EXPORT RollingDigitCounter_38* GetScoreDigits_592360();
    EXPORT s32 GetScore_592370();
    EXPORT void ResetCarModelFlags_592380(char_type bits);
    EXPORT void CheckAllCarModelsFlagged_592430(char_type bits);
    EXPORT void SetCarModelFlag_592570(char_type flag, s32 car_model);
    EXPORT s32 GetCarScoreValue_5925B0(u32 car_model, u8 reward_tier);
    EXPORT void AddCash_592620(s32 cash);
    EXPORT void AwardPedKilledScore_592660(Ped* pVictim, Ped* pKiller);
    EXPORT void AwardCarDestroyedScore_592DD0(Car_BC* pCar, Ped* pKiller);
    EXPORT void AwardCarDamageScore_593030(Car_BC* pCar, s16 damage);
    EXPORT void AwardCarDamageScoreHit_593150(Car_BC* pCar, s16 damage);
    EXPORT void AddCashWithMultiplier_593220();
    EXPORT void AwardCarHijackedScore_593240(Car_BC* pCar);
    EXPORT void AwardBusStolenScore_593370(Car_BC* pCar);
    EXPORT void AwardFullBusDestroyedScore_593410(Car_BC* pCar);
    EXPORT void UpdateAccuracyCount_5934F0(u32 event, s32 weapon_model, Ped* pTarget);
    EXPORT RollingDigitCounter_38* GetMultiplayerFragDigits_5935B0();
    EXPORT s32 GetFrags_5935C0();
    EXPORT void ChangeFragsByAmount_5935D0(s32 amount);

    RollingDigitCounter_38 field_0_money;
    RollingDigitCounter_38 field_38_multiplayer_frags;
    s32 field_70_last_car_kill_time;
    u8 field_74_car_kill_combo;
    u8 field_75_score_mult;
    s32 field_78_last_kill_time;
    s16 field_7C_execution_count;
    s32 field_80_last_elvis_kill_time;
    s16 field_84_num_elvis_killed;
    u16 field_86_total_kills;
    s16 field_88_killed_cops;
    u16 field_8A_cars_stolen_count;
    u8 field_8C_car_model_flags[256]; // car_model_flag bits (car_model_flag.hpp)
    u32 field_18C_one_second_timer_ms;
    u32 field_190_fly_car_time_ms;
    s32 field_194_last_accuracy_event; // accuracy_event (accuracy_event.hpp)
    u8 field_198_accuracy_count;
    s32 field_19C_reverse_time_ms;
    s32 field_1A0_last_emergency_car_kill_time;
    char_type field_1A4_killed_cars_flags; // emergency_car_kill_flag bits (emergency_car_kill_flag.hpp)
    BonusTracker_1C0 field_1A8_bonuses;
    Player* field_368_player;
};