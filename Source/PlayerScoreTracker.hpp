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
    inline void SetMoney(s32 money)
    {
        mMoney.SetValueClamped(money);
    }

    // 9.6f 0x45B0A0
    inline u8 get_accuracy_count() const
    {
        return mAccuracyCount;
    }

    // 9.6f 0x45B0B0
    inline s32 get_reverse_count() const
    {
        return mReverseTimeMs;
    }

    EXPORT PlayerScoreTracker();
    EXPORT void Service();
    EXPORT void Init(Player* pPlayer, s16 digit_transition_speed, s32 max_score_value, s16 palette, u16 max_frag_value);
    EXPORT void Reset();
    EXPORT RollingDigitCounter* GetScoreDigits();
    EXPORT s32 GetScore() const;
    EXPORT void ResetCarModelFlags(char_type bits);
    EXPORT void CheckAllCarModelsFlagged(char_type bits);
    EXPORT void SetCarModelFlag(char_type flag, s32 car_model);
    EXPORT s32 GetCarScoreValue(u32 car_model, u8 reward_tier);
    EXPORT void AddCash(s32 cash);
    EXPORT void AwardPedKilledScore(Ped* pVictim, Ped* pKiller);
    EXPORT void AwardCarDestroyedScore(Car* pCar, Ped* pKiller);
    EXPORT void AwardCarDamageScore(Car* pCar, s16 damage);
    EXPORT void AwardCarDamageScoreHit(Car* pCar, s16 damage);
    EXPORT void AddCashWithMultiplier();
    EXPORT void AwardCarHijackedScore(Car* pCar);
    EXPORT void AwardBusStolenScore(Car* pCar);
    EXPORT void AwardFullBusDestroyedScore(Car* pCar);
    EXPORT void UpdateAccuracyCount(u32 event, s32 weapon_model, Ped* pTarget);
    EXPORT RollingDigitCounter* GetMultiplayerFragDigits();
    EXPORT s32 GetFrags() const;
    EXPORT void ChangeFragsByAmount(s32 amount);

    RollingDigitCounter mMoney; // 0x0
    RollingDigitCounter mMultiplayerFrags; // 0x38
    s32 mLastCarKillTime; // 0x70
    u8 mCarKillCombo; // 0x74
    u8 mScoreMult; // 0x75
    s32 mLastKillTime; // 0x78
    s16 mExecutionCount; // 0x7C
    s32 mLastElvisKillTime; // 0x80
    s16 mNumElvisKilled; // 0x84
    u16 mTotalKills; // 0x86
    s16 mCopCarsDestroyed; // 0x88
    u16 mCarsStolenCount; // 0x8A
    u8 mCarModelFlags[256]; // 0x8C: car_model_flag bits (car_model_flag.hpp)
    u32 mOneSecondTimerMs; // 0x18C
    u32 mFlyCarTimeMs; // 0x190
    s32 mLastAccuracyEvent; // 0x194: accuracy_event (accuracy_event.hpp)
    u8 mAccuracyCount; // 0x198
    s32 mReverseTimeMs; // 0x19C
    s32 mLastEmergencyCarKillTime; // 0x1A0
    char_type mKilledCarsFlags; // 0x1A4: emergency_car_kill_flag bits (emergency_car_kill_flag.hpp)
    BonusTracker mBonuses; // 0x1A8
    Player* mPlayer; // 0x368
};
GTA2_ASSERT_SIZEOF_ALWAYS(PlayerScoreTracker, 0x36C);