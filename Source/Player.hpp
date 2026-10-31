#pragma once

#include "Camera.hpp"
#include "Draw.hpp"
#include "Function.hpp"
#include "ang16.hpp"
#include "PlayerScoreTracker.hpp"
#include "fix16.hpp"
#include "BonusTracker.hpp"
#include "TagGame.hpp"
#include "PlayerCrimeStats.hpp"
#include <windows.h>

class SoundObject;
class Ped;
class Weapon;
class Player;
struct SavedPlayerStats;

class Car;
class Gang;

namespace power_up_indices
{
enum
{
    Multiplier_0 = 0,
    ExtraLife_1 = 1,
    Health_2 = 2,
    Armor_3 = 3,
    JailCard_4 = 4,
    CopBribe_5 = 5,
    Invulnerability_6 = 6,
    DoubleDamage_7 = 7,
    FastReload_8 = 8,
    Electrofingers_9 = 9,
    Respect_10 = 10,
    Invisibility_11 = 11,
    InstantGang_12 = 12,
    Unused_13 = 13, // 13-16: pickups 241-244, whose HUD and pickup sprites are the placeholder letters N to Q
    Unused_14 = 14,
    Unused_15 = 15,
    Unused_16 = 16,
};
} // namespace power_up_indices

class Player
{
  public:
    // 9.6f 0x45B0C0
    inline bool has_player_ped() const
    {
        return mPlayerPed != NULL;
    }

    // 9.6f 0x4766D0
    inline Gang* get_gang_curr_location() const
    {
        return mGangCurrLocation;
    }

    // 9.6f 0x4A5170
    inline void set_in_use(bool in_use)
    {
        mInUse = in_use;
    }

    // 9.6f 0x4C7340
    inline gmp_map_zone* get_arrow_blocker_zone() const
    {
        return mArrowBlockerZone;
    }

    // 9.6f 0x434920
    inline bool HasPowerUp(s32 power_up_idx) const
    {
        return mPowerUpTimers[power_up_idx] != 0;
    }

    // 9.6f 0x434940
    inline void DecPowerUp(s32 power_up_idx)
    {
        mPowerUpTimers[power_up_idx]--;
    }

    // 9.6f 0x4766B0
    inline void ChangeMultipliers(s32 amount)
    {
        mMultipliers.ChangeStatByAmount(amount);
    }

    // 9.6f 0x476700
    inline bool IsWastedOrBustedNotObjective54() const
    {
        Ped* pPed;
        return mWastedOrBusted && mDeathCountdown == 2 && ((pPed = mPlayerPed) == NULL || pPed->get_objective() != objectives_enum::call_police_car_54);
    }

    // 9.6f 0x476730
    inline bool IsBustedObjective54() const
    {
        Ped* pPed;
        return mWastedOrBusted && mDeathCountdown == 2 && (pPed = mPlayerPed) != NULL && pPed->get_objective() == objectives_enum::call_police_car_54;
    }

    // 9.6f 0x4766C0
    inline s32 get_lives()
    {
        return mLives.get_value();
    }

    // 9.6f 0x421990
    inline void AddCash(s32 cash)
    {
        mScores.AddCash(cash);
    }

    // 9.6f 0x421980
    inline s32 GetScore() const
    {
        return mScores.GetScore();
    }

    // 9.6f 0x4766A0
    inline s32 get_multiplier()
    {
        return mMultipliers.get_value();
    }

    // 9.6f 0x41DC40
    inline void AddScore(s32 score)
    {
        mScores.AddCash(score * mMultipliers.mValue);
    }

    inline Ped* GetCameraModePed() const
    {
        return mCameraMode == 2 ? mAuxPed : mPlayerPed;
    }

    // 0x4CCAE0
    s32 GetThrowCharge() const
    {
        return mThrowCharge;
    }

    // 0x4CCAB0
    void IncrementThrowCharge()
    {
        s32 t = mThrowCharge;
        if (t >= 0)
        {
            this->mThrowCharge = t + 1;
        }

        if (this->mThrowCharge > 270)
        {
            this->mThrowCharge = 270;
        }
    }

    // 9.6f 0x4CCAD0
    s32 GetThrowStrength() const
    {
        if (mThrowCharge <= 0x3C)
        {
            return mThrowCharge;
        }
        else
        {
            return 0x3C;
        }
    }

    // 9.6f 0x461DB0
    inline bool GetInUse() const
    {
        return mInUse;
    }

    // 9.6f inline 0x41D020
    inline Ped* GetPlayerPed_41D020() const
    {
        return mPlayerPed;
    }

    // 9.6f 0x4CCB00
    bool IsThrowCharging() const
    {
        return this->mThrowCharge >= 0;
    }

    // 9.6f 0x4A5180
    inline void ResetThrowCharge()
    {
        mThrowCharge = 0;
    }

    // 9.6f 0x434950
    inline void SetDeathType(s32 death_type)
    {
        mDeathType = death_type;
    }

    // 9.6f 0x4A5100
    bool IsRemoteControlActive() const
    {
        s32 occupation;
        if (!mAuxCamActive || !mAuxPed ||
            (occupation = mAuxPed->get_occupation(), occupation != ped_ocupation_enum::empty))
        {
            return false;
        }
        return true;
    }

    EXPORT s32 ObjectTypeToWeaponType(u8 shop_type);

    EXPORT u8 GetIdx();
    EXPORT void AddCarToHistory(Car* pNewCar);
    EXPORT bool PromoteCarInHistory(Car* pCar, bool dont_modify);
    EXPORT void RemoveCarFromHistory(Car* pCar);
    EXPORT void SetKFCarWeapon(Car* pCar, s32 weapon_kind);
    EXPORT void SetKFWeapon(s32 weapon_idx);
    EXPORT void ClearKFWeapon();
    EXPORT Weapon* GetCurrPlayerWeapon();
    EXPORT void SetWeapon(Weapon* pWeapon);
    EXPORT bool HasAnyAmmo();
    EXPORT bool AddWeaponWithAmmo(s32 weapon_kind, u8 ammo);
    EXPORT void SelectNextOrPrevWeapon(bool go_forwards, bool go_backwards);
    EXPORT void LoadCarWeapons(Car* pCar);
    EXPORT void ClearCarWeapons();
    EXPORT void CleanupEmptyAmmoWeapons();
    EXPORT void UnloadCarWeapons();
    EXPORT void RemovePlayerWeapons();
    EXPORT void ClearPowerUps();
    EXPORT void ClearPowerUpsExceptJailCard();
    EXPORT bool CollectPowerUp(s32 power_up_idx);
    EXPORT void tick_down_powerups();
    EXPORT void RestorePowerUpsFromSave(SavedPlayerStats* pSave);
    EXPORT void TeleportToDebugCam();
    EXPORT void DebugWatchNearestCar();
    EXPORT void OnVideoModeChange();
    EXPORT void InitPlayerPed(Ped* pPed);
    EXPORT void SetInputs(u32 inputs);
    EXPORT void IncrementGangRespectFromDebugKeys(u8 count);
    EXPORT void IncreaseWantedLevelFromDebugKeys();
    EXPORT void Hud_Controls(u16 action);
    EXPORT void HandleKeyRelease(u16 key);
    EXPORT void CharacterControls();
    EXPORT void ControlInputs();
    EXPORT void HandleControls(Ped* pPed);
    EXPORT void DoCarControlInputs(Car* pCar);
    EXPORT void DoPedControlInputs(Ped* pPed);
    EXPORT void ShowDebugInfo(bool skip);
    EXPORT void RespawnPlayer();
    EXPORT void Wasted();
    EXPORT void UpdateAuxPedDeath();
    EXPORT void Busted();
    EXPORT void UpdateCurrentZones();
    EXPORT void UpdateSoundListener();
    EXPORT void HandleDebugZoom();
    EXPORT void UpdateCamera(Camera* pCam);
    EXPORT void Disconnect();
    EXPORT void Service();
    EXPORT void UpdatePaused();
    EXPORT void EndRemoteControl();
    EXPORT void ResetAuxCamera();
    EXPORT void StartRemoteControl(Car* pCar);
    EXPORT void WatchCar(Car* pCar);
    EXPORT void GetPosU8(u8& x, u8& y, u8& z);
    EXPORT Car* GetPlayerCar();
    EXPORT void get_pos(Fix16* out_x, Fix16* out_y, Fix16* out_z);
    EXPORT void ChangeLifeCountByAmount(s32 amount);
    EXPORT void ColorScoreFromRemap();
    EXPORT void SetScoreTextColour();
    EXPORT void InitializePlayerState();
    EXPORT void DebugToggleRemoteControl();
    EXPORT char* GetDeathText();
    EXPORT void DisableInputs();
    EXPORT void DisableAllControls();
    EXPORT void EnableAllControls();
    EXPORT void EnableKFMode();
    EXPORT void DisableKFMode();
    EXPORT void DisableEnterVehicles();
    EXPORT void EnableEnterVehicles();
    EXPORT void RestoreCarsFromSave();
    EXPORT void CopyPlayerDataToSave(SavedPlayerStats* pSave);
    EXPORT void UpdateGameFromSave(SavedPlayerStats* pSave);
    EXPORT void ApplyCheats();
    EXPORT void ClearInputs();

    // 0x56A740
    EXPORT Player(u8 player_idx);

    // 0x56A940
    EXPORT ~Player();

    // 9.6f 0x41DC70
    inline bool IsUser() const
    {
        return mIsUser;
    }

    inline Ped* GetPlayerPed_4A5130() const
    {
        if (mCameraMode == 2)
        {
            return mAuxPed;
        }
        else
        {
            return mPlayerPed;
        }
    }

    // 9.6f 0x4219D0
    inline u8 get_idx() const
    {
        return mPlayerIdx;
    }

    // 9.6f 0x4A5150
    inline Ped* GetActivePed() const
    {
        return (mCameraMode == 2 || mCameraMode == 3) ? mAuxPed : mPlayerPed;
    }

    // 9.6f 0x434900
    inline Camera* get_camera()
    {
        if (mCameraMode == 2 || mCameraMode == 3)
        {
            return &mAuxGameCamera;
        }
        else
        {
            return &mGameCamera;
        }
    }

    bool mIsUser; // 0x0
    char_type mUnused1; // 0x1
    char_type mUnused2; // 0x2
    u32 mInputs; // 0x4: this tick's input bits (SetInputs); zeroed once the tick has used them
    Ang16 mTurnSpeed; // 0x8
    Ang16 mTurnAccel; // 0xA
    Fix16 mMoveDirection; // 0xC
    s32 mUnused10; // 0x10
    s16 mSavedPedWeaponIdx; // 0x14
    s16 mSavedCarWeaponIdx; // 0x16

    s16 mPreKfWeaponKind; // 0x18
    s16 mPreKfAmmo; // 0x1A
    s32 mKfWeaponKind; // 0x1C
    Car* mKfCar; // 0x20
    s32 mKfCarId; // 0x24

    bool mWastedOrBusted; // 0x28
    bool mAuxPedDying; // 0x29
    char_type mPad2A; // 0x2A
    char_type mPad2B; // 0x2B
    s16 mDeathCountdown; // 0x2C
    u8 mPlayerIdx; // 0x2E
    bool mDisableAllControls; // 0x2F
    bool mDisableEnterVehicles; // 0x30
    bool mKfWeaponMode; // 0x31
    Gang* mGangCurrLocation; // 0x34
    gmp_map_zone* mLocalNavigationZone; // 0x38
    gmp_map_zone* mNavigationZone; // 0x3C
    gmp_map_zone* mArrowBlockerZone; // 0x40
    s32 mDeathType; // 0x44
    bool mDbgCamFollowPlayer; // 0x48
    SoundObject* mSoundObj; // 0x4C
    s32 mThrowCharge; // 0x50
    Car* mCarHistory[3]; // 0x54
    s32 mFinishScoreReached; // 0x60
    bool mJumping; // 0x64
    s32 mCameraMode; // 0x68
    s32 mInDebugCamMode; // 0x6C
    bool mDbgCamNorth; // 0x70
    bool mDbgCamSouth; // 0x71
    bool mDbgCamWest; // 0x72
    bool mDbgCamEast; // 0x73
    bool mDbgCamZoomingOut; // 0x74
    bool mDbgCamZoomingIn; // 0x75
    bool mDbgZoomIn; // 0x76
    bool mDbgZoomOut; // 0x77

    // Current inputs state
    bool mNowForwardPressed; // 0x78
    bool mNowDownPressed; // 0x79
    bool mNowLeftPressed; // 0x7A
    bool mNowRightPressed; // 0x7B
    bool mNowAttackPressed; // 0x7C
    bool mNowEnterExitPressed; // 0x7D
    bool mNowHandBrakeOrJumpPressed; // 0x7E
    bool mNowPrevWeaponPressed; // 0x7F
    bool mNowNextWeaponPressed; // 0x80
    bool mNowSpecial1Pressed; // 0x81
    bool mNowSpecial2Pressed; // 0x82
    bool mNowSpecial3Pressed; // 0x83

    // Previous inputs state
    bool mSpecial1Changed; // 0x84
    bool mSpecial2Changed; // 0x85
    bool mSpecial3Changed; // 0x86
    bool mNextWeaponChanged; // 0x87
    bool mPrevWeaponChanged; // 0x88
    bool mEnterExitChanged; // 0x89
    bool mHandBrakeOrJumpChanged; // 0x8A
    bool mForwardChanged; // 0x8B
    bool mDownChanged; // 0x8C
    bool mAttackChanged; // 0x8D

    bool mInUse; // 0x8E
    bool mBlockAttack; // 0x8F
    Camera mGameCamera; // 0x90
    Camera mViewCamera; // 0x14C
    Camera mAuxGameCamera; // 0x208
    Ped* mPlayerPed; // 0x2C4
    Ped* mAuxPed; // 0x2C8
    Car* mWatchedCar; // 0x2CC
    bool mAuxCamActive; // 0x2D0
    PlayerScoreTracker mScores; // 0x2D4
    bool mBusted; // 0x640
    PlayerCrimeStats mCrimeStats; // 0x644
    u16 mTrafficSpawnCounter; // 0x680
    u16 mTrafficSpawnThreshold; // 0x682
    RollingDigitCounter mLives; // 0x684
    RollingDigitCounter mMultipliers; // 0x6BC
    u16 mPowerUpTimers[17]; // 0x6F4
    s16 mUnused716; // 0x716
    Weapon* mWeapons[28]; // 0x718
    s16 mCurrWeaponIdx; // 0x788
    bool mShowQuitMessage; // 0x78A
    s32 mHudPaletteType; // 0x78C: Usage: 2 = default, 7 = multiplayer if using gang remap
    u16 mHudPalette; // 0x790
    s16 mUnused792; // 0x792
    bool mIsChatting; // 0x794
    s16 mChatText[79]; // 0x796
    s32 mUnused834; // 0x834
    s32 mChatTextIdx; // 0x838
    wchar_t mPlayerName[16]; // 0x83C
};

// 9.6f 0x434B60
inline bool TagGame::IsFugitivePed(Ped* pPed)
{
    return mFugitive && mFugitive->GetPlayerPed_41D020() == pPed;
}
