#pragma once

#include "Camera.hpp"
#include "Draw.hpp"
#include "Function.hpp"
#include "ang16.hpp"
#include "PlayerScoreTracker_36C.hpp"
#include "fix16.hpp"
#include "sad_mirzakhani.hpp"
#include "youthful_einstein.hpp"
#include "zealous_borg.hpp"
#include <windows.h>

class infallible_turing;
class Ped;
class Weapon_30;
class Player;
struct save_stats_0x90;

// TODO: add these later
class Car_BC;
class Gang_144;

namespace power_up_indices
{
enum
{
    Unk_0 = 0,
    Unk_1 = 1,
    Unk_2 = 2,
    Armor_3 = 3,
    JailCard_4 = 4,
    Unk_5 = 5,
    Invulnerability_6 = 6,
    DoubleDamage_7 = 7,
    FastReload_8 = 8,
    Electrofingers_9 = 9,
    Unk_10 = 10,
    Invisibility_11 = 11,
    InstantGang_12 = 12,
    Unk_13 = 13,
    Unk_14 = 14,
    Unk_15 = 15,
    Unk_16 = 16,
};
} // namespace power_up_indices

class Player
{
  public:
    // 9.6f 0x45B0C0
    inline bool has_player_ped_45B0C0()
    {
        return field_2C4_player_ped != NULL;
    }

    // 9.6f 0x4766D0
    inline Gang_144* get_gang_curr_location_4766D0()
    {
        return field_34_gang_curr_location;
    }

    // 9.6f 0x4A5170
    inline void set_bInUse_4A5170(bool v)
    {
        field_8E_bInUse = v;
    }

    // 9.6f 0x4C7340
    inline gmp_map_zone* get_arrow_blocker_zone_4C7340()
    {
        return field_40_arrow_blocker_zone;
    }

    // 9.6f 0x434920
    inline bool HasPowerUp_434920(s32 idx)
    {
        return field_6F4_power_up_timers[idx] != 0;
    }

    // 9.6f 0x434940
    inline void DecPowerUp_434940(s32 idx)
    {
        field_6F4_power_up_timers[idx]--;
    }

    // 9.6f 0x4766B0
    inline void ChangeMultipliers_4766B0(s32 amount)
    {
        field_6BC_multpliers.ChangeStatByAmount_4921B0(amount);
    }

    // 9.6f 0x476700
    inline bool IsBustedNotObjective54_476700()
    {
        Ped* pPed;
        return field_28_bWastedOrBusted && field_2C_death_countdown == 2 && ((pPed = field_2C4_player_ped) == NULL || pPed->get_objective_403A80() != 54);
    }

    // 9.6f 0x476730
    inline bool IsBustedObjective54_476730()
    {
        Ped* pPed;
        return field_28_bWastedOrBusted && field_2C_death_countdown == 2 && (pPed = field_2C4_player_ped) != NULL && pPed->get_objective_403A80() == 54;
    }

    // 9.6f 0x4766C0
    inline s32 get_lives_4766C0()
    {
        return field_684_lives.get_value();
    }

    // 9.6f 0x421990
    inline void AddCash_421990(s32 cash)
    {
        field_2D4_scores.AddCash_592620(cash);
    }

    // 9.6f 0x421980
    inline s32 GetScore_421980()
    {
        return field_2D4_scores.GetScore_592370();
    }

    // 9.6f 0x4766A0
    inline s32 get_multiplier_4766A0()
    {
        return field_6BC_multpliers.get_value();
    }

    // 9.6f 0x41DC40
    inline void AddScore_41DC40(s32 score)
    {
        field_2D4_scores.AddCash_592620(score * field_6BC_multpliers.field_0_value);
    }

    inline Ped* GetCameraModePed()
    {
        return field_68_camera_mode == 2 ? field_2C8_aux_ped : field_2C4_player_ped;
    }

    // 0x4CCAE0
    s32 GetThrowCharge_4CCAE0()
    {
        return field_50_throw_charge;
    }

    // 0x4CCAB0
    void IncrementThrowCharge_4CCAB0()
    {
        s32 t = field_50_throw_charge;
        if (t >= 0)
        {
            this->field_50_throw_charge = t + 1;
        }

        if (this->field_50_throw_charge > 270)
        {
            this->field_50_throw_charge = 270;
        }
    }

    s32 GetThrowStrength_4CCAD0()
    {
        if (field_50_throw_charge <= 0x3C)
        {
            return field_50_throw_charge;
        }
        else
        {
            return 0x3C;
        }
    }

    inline bool GetInUse_461DB0()
    {
        return field_8E_bInUse;
    }

    // 9.6f inline 0x41D020
    inline Ped* GetPlayerPed_41D020()
    {
        return field_2C4_player_ped;
    }

    bool IsThrowCharging_4CCB00()
    {
        return this->field_50_throw_charge >= 0;
    }

    inline void ResetThrowCharge_4A5180()
    {
        field_50_throw_charge = 0;
    }

    inline void SetDeathType_434950(s32 type)
    {
        field_44_death_type = type;
    }

    // 9.6f 0x4A5100
    bool IsRemoteControlActive_4A5100()
    {
        s32 occupation;
        if (!field_2D0_bAuxCamActive || !field_2C8_aux_ped ||
            (occupation = field_2C8_aux_ped->get_occupation_403980(), occupation != ped_ocupation_enum::empty))
        {
            return false;
        }
        return true;
    }

    // TODO: Ordering
    EXPORT s32 ObjectTypeToWeaponType_443CB0(u8 varrok);

    EXPORT u8 GetIdx_4881E0();
    EXPORT void AddCarToHistory_5645B0(Car_BC* a2);
    EXPORT bool PromoteCarInHistory_564610(Car_BC* pCar, bool bDontModify);
    EXPORT void PushCarInfo_564680(Car_BC* a2);
    EXPORT void SetKFCarWeapon_564710(Car_BC* pCar, s32 weapon_kind);
    EXPORT void SetKFWeapon_564790(s32 idx);
    EXPORT void ClearKFWeapon_5647D0();
    EXPORT Weapon_30* GetCurrPlayerWeapon_5648F0();
    EXPORT void SetWeapon_564910(Weapon_30* a2);
    EXPORT char_type HasAnyAmmo_564940();
    EXPORT char_type AddWeaponWithAmmo_564960(s32 a2, u8 a3);
    EXPORT void SelectNextOrPrevWeapon_5649D0(char_type bFowards, char_type bBackwards);
    EXPORT void LoadCarWeapons_564AD0(Car_BC* a2);
    EXPORT void ClearCarWeapons_564B60();
    EXPORT void CleanupEmptyAmmoWeapons_564B80();
    EXPORT void UnloadCarWeapons_564C00();
    EXPORT void RemovePlayerWeapons_564C50();
    EXPORT void ClearPowerUps_564CC0();
    EXPORT void ClearPowerUpsExceptJailCard_564CF0();
    EXPORT char_type CollectPowerUp_564D60(s32 a2);
    EXPORT void tick_down_powerups_565070();
    EXPORT void RestorePowerUpsFromSave_5651F0(save_stats_0x90* a2);
    EXPORT void TeleportToDebugCam_565310();
    EXPORT void DebugWatchNearestCar_5653E0();
    EXPORT void sub_565460();
    EXPORT void InitPlayerPed_565490(Ped* pPed);
    EXPORT void SetInputs_565740(u32 input);
    EXPORT void IncrementGangRespectFromDebugKeys_565770(u8 count);
    EXPORT void IncreaseWantedLevelFromDebugKeys_565860();
    EXPORT void Hud_Controls_565890(u16 action);
    EXPORT void HandleKeyRelease_566380(u16 a2);
    EXPORT void CharacterControls_566520();
    EXPORT void ControlInputs_566820();
    EXPORT void HandleControls_5668D0(Ped* a2);
    EXPORT void DoCarControlInputs_566C30(Car_BC* pCar);
    EXPORT void DoPedControlInputs_566C80(Ped* a2);
    EXPORT void ShowDebugInfo_566EE0(char_type a2);
    EXPORT void RespawnPlayer_5670B0();
    EXPORT void Wasted_567130();
    EXPORT void UpdateAuxPedDeath_567850();
    EXPORT void Busted_5679E0();
    EXPORT void UpdateCurrentZones_568520();
    EXPORT void UpdateSoundListener_568630();
    EXPORT void HandleDebugZoom_568670();
    EXPORT void UpdateCamera_5686D0(Camera_0xBC* pCam);
    EXPORT void Disconnect_568730();
    EXPORT void Service_5687F0();
    EXPORT void UpdatePaused_569410();
    EXPORT void EndRemoteControl_569530();
    EXPORT void ResetAuxCamera_5695A0();
    EXPORT void StartRemoteControl_569600(Car_BC* pCar);
    EXPORT void WatchCar_5696D0(Car_BC* pCar);
    EXPORT void GetPosU8_569840(u8& a2, u8& a3, u8& a4);
    EXPORT Car_BC* GetPlayerCar_5698E0();
    EXPORT void get_pos_569920(Fix16* a2, Fix16* a3, Fix16* a4);
    EXPORT void ChangeLifeCountByAmount_5699F0(s32 a2);
    EXPORT void ColorScoreFromRemap_569A10();
    EXPORT void SetScoreTextColour_569C20();
    EXPORT void InitializePlayerState_569CB0();
    EXPORT void DebugToggleRemoteControl_569E70();
    EXPORT char* GetDeathText_569F00();
    EXPORT void DisableInputs_569F40();
    EXPORT void DisableAllControls_569FF0();
    EXPORT void EnableAllControls_56A000();
    EXPORT void EnableKFMode_56A010();
    EXPORT void DisableKFMode_56A020();
    EXPORT void DisableEnterVehicles_56A030();
    EXPORT void EnableEnterVehicles_56A040();
    EXPORT void RestoreCarsFromSave_56A0F0();
    EXPORT void CopyPlayerDataToSave_56A1A0(save_stats_0x90* pSave);
    EXPORT void UpdateGameFromSave_56A310(save_stats_0x90* a2);
    EXPORT void ApplyCheats_56A490();
    EXPORT void ClearInputs_56A6D0();

    // 0x56A740
    EXPORT Player(u8 a2);

    // 0x56A940
    EXPORT ~Player();

    inline u8 IsUser_41DC70()
    {
        return field_0_bIsUser;
    }

    inline Ped* GetPlayerPed_4A5130()
    {
        if (field_68_camera_mode == 2)
        {
            return field_2C8_aux_ped;
        }
        else
        {
            return field_2C4_player_ped;
        }
    }

    inline u8 get_idx_4219D0()
    {
        return field_2E_idx;
    }

    // 9.6f 0x4A5150
    inline Ped* GetActivePed_4A5150()
    {
        return (field_68_camera_mode == 2 || field_68_camera_mode == 3) ? field_2C8_aux_ped : field_2C4_player_ped;
    }

    // 9.6f 0x434900
    inline Camera_0xBC* get_camera_434900()
    {
        if (field_68_camera_mode == 2 || field_68_camera_mode == 3)
        {
            return &field_208_aux_game_camera;
        }
        else
        {
            return &field_90_game_camera;
        }
    }

    u8 field_0_bIsUser;
    char_type field_1;
    char_type field_2;
    char_type field_3_pad;
    u32 field_4_inputs;
    Ang16 field_8_turn_speed;
    Ang16 field_A_turn_accel;
    Fix16 field_C_move_direction;
    s32 field_10_unused;
    s16 field_14_saved_ped_weapon_idx;
    s16 field_16_saved_car_weapon_idx;

    s16 field_18_pre_kf_weapon_kind;
    s16 field_1A_pre_kf_ammo;
    s32 field_1C_kf_weapon_kind;
    Car_BC* field_20_kf_car;
    s32 field_24_kf_car_id;

    char_type field_28_bWastedOrBusted;
    char_type field_29_bAuxPedDying;
    char_type field_2A_pad;
    char_type field_2B_pad;
    s16 field_2C_death_countdown;
    u8 field_2E_idx;
    char_type field_2F_disable_all_controls;
    char_type field_30_disable_enter_vehicles;
    char_type field_31_kf_weapon_mode;
    char_type field_32_pad;
    char_type field_33_pad;
    Gang_144* field_34_gang_curr_location;
    gmp_map_zone* field_38_local_navigation_zone;
    gmp_map_zone* field_3C_navigation_zone;
    gmp_map_zone* field_40_arrow_blocker_zone;
    s32 field_44_death_type;
    char_type field_48_bDbg_cam_follow_player;
    char_type field_49_pad;
    char_type field_4A_pad;
    char_type field_4B_pad;
    infallible_turing* field_4C_pSoundObj;
    s32 field_50_throw_charge;
    Car_BC* field_54_car_history[3];
    s32 field_60_bFinshScoreReached;
    char_type field_64_bJumping;
    char_type field_65_pad;
    char_type field_66_pad;
    char_type field_67_pad;
    s32 field_68_camera_mode;
    s32 field_6C_bIn_debug_cam_mode;
    char_type field_70_dbg_cam_north;
    char_type field_71_s;
    char_type field_72_e;
    char_type field_73_w;
    char_type field_74_dbg_cam_zooming_out;
    char_type field_75_dbg_cam_zooming_in;
    char_type field_76_dbg_zoom_in;
    char_type field_77_dbg_zoom_out;

    // Current inputs state
    bool field_78_bNowForwardPressed;
    bool field_79_bNowDownPressed;
    bool field_7A_bNowLeftPressed;
    bool field_7B_bNowRightPressed;
    bool field_7C_bNowAttackPressed;
    bool field_7D_bNowEnterExitPressed;
    bool field_7E_bNowHandBrakeOrJumpPressed;
    bool field_7F_bNowPrevWeaponPressed;
    bool field_80_bNowNextWeaponPressed;
    bool field_81_bNowSpecial_1_Pressed;
    bool field_82_bNowSpecial_2_Pressed;
    bool field_83_bNowSpecial_3_Pressed;

    // Previous inputs state
    bool field_84_bWasSpecial_1_Pressed;
    bool field_85_bWasSpecial_2_Pressed;
    bool field_86_bWasSpecial_3_Pressed;
    bool field_87_bWasNextWeaponPressed;
    bool field_88_bWasPrevWeaponPressed;
    bool field_89_bWasEnterExitPressed;
    bool field_8A_bWasHandBrakeOrJumpPressed;
    bool field_8B_bWasForwardPressed;
    bool field_8C_bWasDownPressed;
    bool field_8D_bWasAttackPressed;

    bool field_8E_bInUse;
    char_type field_8F_bBlockAttack;
    Camera_0xBC field_90_game_camera;
    Camera_0xBC field_14C_view_camera;
    Camera_0xBC field_208_aux_game_camera;
    Ped* field_2C4_player_ped;
    Ped* field_2C8_aux_ped;
    Car_BC* field_2CC_watched_car;
    char_type field_2D0_bAuxCamActive;
    char_type field_2D1_pad;
    char_type field_2D2_pad;
    char_type field_2D3_pad;
    PlayerScoreTracker_36C field_2D4_scores;
    char_type field_640_busted;
    char_type field_641_pad;
    char_type field_642_pad;
    char_type field_643_pad;
    zealous_borg field_644_crime_stats;
    u16 field_680_traffic_spawn_counter;
    u16 field_682_traffic_spawn_threshold;
    thirsty_lamarr field_684_lives;
    thirsty_lamarr field_6BC_multpliers;
    u16 field_6F4_power_up_timers[17];
    s16 field_716_unused;
    Weapon_30* field_718_weapons[28];
    s16 field_788_curr_weapon_idx;
    char_type field_78A_show_quit_message;
    char_type field_78B_pad;
    s32 field_78C_hud_palette_type; // Usage: 2 = default, 7 = multiplayer if using gang remap
    u16 field_790_hud_palette;
    s16 field_792_unused;
    char_type field_794_is_chatting;
    char_type field_795_pad;
    s16 field_796_chat_text[79];
    s32 field_834_unused;
    s32 field_838_f796_idx;
    wchar_t field_83C_player_name[16];
};

// 9.6f 0x434B60
inline bool youthful_einstein::IsFugitivePed_434B60(Ped* pPed)
{
    return field_0_fugitive && field_0_fugitive->GetPlayerPed_41D020() == pPed;
}
