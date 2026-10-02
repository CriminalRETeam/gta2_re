#pragma once

#include "Function.hpp"
#include "sad_mirzakhani.hpp"
#include "thirsty_lamarr.hpp"

class Player;
class Car_BC;
class Ped;

class eager_benz
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
        return field_19C_reverse_count;
    }

    EXPORT eager_benz();
    EXPORT void Service_591C70();
    EXPORT void Init_5922F0(Player* a2, s16 a3, s32 a4, s16 a5, u16 a6);
    EXPORT void Reset_592330();
    EXPORT thirsty_lamarr* GetScoreDigits_592360();
    EXPORT s32 GetScore_592370();
    EXPORT void ResetCarModelFlags_592380(char_type bits);
    EXPORT void CheckAllCarModelsFlagged_592430(char_type bits);
    EXPORT void SetCarModelFlag_592570(char_type a2, s32 a3);
    EXPORT s32 GetCarScoreValue_5925B0(u32 car_info_idx, u8 a4);
    EXPORT void AddCash_592620(s32 cash);
    EXPORT void OnPedKilled_592660(Ped* pPed1, Ped* pPed2);
    EXPORT void OnCarDestroyed_592DD0(Car_BC* pCar, Ped* pPed);
    EXPORT void AwardCarDamageScore_593030(Car_BC* pCar, s16 score_default);
    EXPORT void sub_593150(Car_BC* a2, s16 a3);
    EXPORT void sub_593220();
    EXPORT void OnCarHijacked_593240(Car_BC* pCar);
    EXPORT void sub_593370(Car_BC* pCar);
    EXPORT void OnFullBusDestroyed_593410(Car_BC* pCar);
    EXPORT void UpdateAccuracyCount_5934F0(u32 a2, s32 a3, Ped* a4);
    EXPORT thirsty_lamarr* GetMultiplayerFragDigits_5935B0();
    EXPORT s32 GetFrags_5935C0();
    EXPORT void ChangeFragsByAmount_5935D0(s32 a2);

    thirsty_lamarr field_0_money;
    thirsty_lamarr field_38_multiplayer_frags;
    s32 field_70_last_car_kill_time;
    char_type field_74_car_kill_combo;
    char_type field_75_score_mult;
    char_type field_76;
    char_type field_77;
    s32 field_78;
    s16 field_7C_e_execution_count;
    s16 field_7E;
    s32 field_80;
    s16 field_84_num_elvis_killed;
    u16 field_86_total_kills;
    s16 field_88_killed_cops;
    u16 field_8A_cars_stolen_count;
    u8 field_8C_car_model_flags[256];
    u32 field_18C_one_second_timer;
    u32 field_190_fly_car_count;
    s32 field_194;
    u8 field_198_accuracy_count;
    char_type field_199;
    char_type field_19A;
    char_type field_19B;
    s32 field_19C_reverse_count;
    s32 field_1A0_last_emergency_car_kill_time;
    char_type field_1A4_killed_cars_flags;
    char_type field_1A5;
    char_type field_1A6;
    char_type field_1A7;
    sad_mirzakhani field_1A8_bonuses;
    Player* field_368_player;
};