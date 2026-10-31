#pragma once

#include "Function.hpp"
#include "BonusTracker.hpp"
#include "RollingDigitCounter.hpp"

class Player;
class Car;
class Ped;

class PlayerScoreTracker
{
  public:
    // 9.6f 0x4A50E0
    inline void SetMoney_4A50E0(s32 money)
    {
        field_0_money.SetValueClamped_4A50B0(money);
    }

    // 9.6f 0x45B0A0
    inline u8 get_accuracy_count_45B0A0() const
    {
        return field_198_accuracy_count;
    }

    // 9.6f 0x45B0B0
    inline s32 get_reverse_count_45B0B0() const
    {
        return field_19C_reverse_time_ms;
    }

    EXPORT PlayerScoreTracker();
    EXPORT void Service_591C70();
    EXPORT void Init_5922F0(Player* pPlayer, s16 digit_transition_speed, s32 max_score_value, s16 palette, u16 max_frag_value);
    EXPORT void Reset_592330();
    EXPORT RollingDigitCounter* GetScoreDigits_592360();
    EXPORT s32 GetScore_592370() const;
    EXPORT void ResetCarModelFlags_592380(char_type bits);
    EXPORT void CheckAllCarModelsFlagged_592430(char_type bits);
    EXPORT void SetCarModelFlag_592570(char_type flag, s32 car_model);
    EXPORT s32 GetCarScoreValue_5925B0(u32 car_model, u8 reward_tier);
    EXPORT void AddCash_592620(s32 cash);
    EXPORT void AwardPedKilledScore_592660(Ped* pVictim, Ped* pKiller);
    EXPORT void AwardCarDestroyedScore_592DD0(Car* pCar, Ped* pKiller);
    EXPORT void AwardCarDamageScore_593030(Car* pCar, s16 damage);
    EXPORT void AwardCarDamageScoreHit_593150(Car* pCar, s16 damage);
    EXPORT void AddCashWithMultiplier_593220();
    EXPORT void AwardCarHijackedScore_593240(Car* pCar);
    EXPORT void AwardBusStolenScore_593370(Car* pCar);
    EXPORT void AwardFullBusDestroyedScore_593410(Car* pCar);
    EXPORT void UpdateAccuracyCount_5934F0(u32 event, s32 weapon_model, Ped* pTarget);
    EXPORT RollingDigitCounter* GetMultiplayerFragDigits_5935B0();
    EXPORT s32 GetFrags_5935C0() const;
    EXPORT void ChangeFragsByAmount_5935D0(s32 amount);

    RollingDigitCounter field_0_money; // 0x0
    RollingDigitCounter field_38_multiplayer_frags; // 0x38
    s32 field_70_last_car_kill_time; // 0x70
    u8 field_74_car_kill_combo; // 0x74
    u8 field_75_score_mult; // 0x75
    s32 field_78_last_kill_time; // 0x78
    s16 field_7C_execution_count; // 0x7C
    s32 field_80_last_elvis_kill_time; // 0x80
    s16 field_84_num_elvis_killed; // 0x84
    u16 field_86_total_kills; // 0x86
    s16 field_88_cop_cars_destroyed; // 0x88
    u16 field_8A_cars_stolen_count; // 0x8A
    u8 field_8C_car_model_flags[256]; // 0x8C: car_model_flag bits (car_model_flag.hpp)
    u32 field_18C_one_second_timer_ms; // 0x18C
    u32 field_190_fly_car_time_ms; // 0x190
    s32 field_194_last_accuracy_event; // 0x194: accuracy_event (accuracy_event.hpp)
    u8 field_198_accuracy_count; // 0x198
    s32 field_19C_reverse_time_ms; // 0x19C
    s32 field_1A0_last_emergency_car_kill_time; // 0x1A0
    char_type field_1A4_killed_cars_flags; // 0x1A4: emergency_car_kill_flag bits (emergency_car_kill_flag.hpp)
    BonusTracker field_1A8_bonuses; // 0x1A8
    Player* field_368_player; // 0x368
};
GTA2_ASSERT_SIZEOF_ALWAYS(PlayerScoreTracker, 0x36C);