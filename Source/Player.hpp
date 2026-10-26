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

// TODO: add these later
class Car;
class Gang;

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
    inline bool has_player_ped_45B0C0() const
    {
        return field_2C4_player_ped != NULL;
    }

    // 9.6f 0x4766D0
    inline Gang* get_gang_curr_location_4766D0() const
    {
        return field_34_gang_curr_location;
    }

    // 9.6f 0x4A5170
    inline void set_in_use_4A5170(bool in_use)
    {
        field_8E_in_use = in_use;
    }

    // 9.6f 0x4C7340
    inline gmp_map_zone* get_arrow_blocker_zone_4C7340() const
    {
        return field_40_arrow_blocker_zone;
    }

    // 9.6f 0x434920
    inline bool HasPowerUp_434920(s32 power_up_idx) const
    {
        return field_6F4_power_up_timers[power_up_idx] != 0;
    }

    // 9.6f 0x434940
    inline void DecPowerUp_434940(s32 power_up_idx)
    {
        field_6F4_power_up_timers[power_up_idx]--;
    }

    // 9.6f 0x4766B0
    inline void ChangeMultipliers_4766B0(s32 amount)
    {
        field_6BC_multipliers.ChangeStatByAmount_4921B0(amount);
    }

    // 9.6f 0x476700
    inline bool IsWastedOrBustedNotObjective54_476700() const
    {
        Ped* pPed;
        return field_28_wasted_or_busted && field_2C_death_countdown == 2 && ((pPed = field_2C4_player_ped) == NULL || pPed->get_objective_403A80() != objectives_enum::objective_54);
    }

    // 9.6f 0x476730
    inline bool IsBustedObjective54_476730() const
    {
        Ped* pPed;
        return field_28_wasted_or_busted && field_2C_death_countdown == 2 && (pPed = field_2C4_player_ped) != NULL && pPed->get_objective_403A80() == objectives_enum::objective_54;
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
    inline s32 GetScore_421980() const
    {
        return field_2D4_scores.GetScore_592370();
    }

    // 9.6f 0x4766A0
    inline s32 get_multiplier_4766A0()
    {
        return field_6BC_multipliers.get_value();
    }

    // 9.6f 0x41DC40
    inline void AddScore_41DC40(s32 score)
    {
        field_2D4_scores.AddCash_592620(score * field_6BC_multipliers.field_0_value);
    }

    inline Ped* GetCameraModePed() const
    {
        return field_68_camera_mode == 2 ? field_2C8_aux_ped : field_2C4_player_ped;
    }

    // 0x4CCAE0
    s32 GetThrowCharge_4CCAE0() const
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

    s32 GetThrowStrength_4CCAD0() const
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

    inline bool GetInUse_461DB0() const
    {
        return field_8E_in_use;
    }

    // 9.6f inline 0x41D020
    inline Ped* GetPlayerPed_41D020() const
    {
        return field_2C4_player_ped;
    }

    bool IsThrowCharging_4CCB00() const
    {
        return this->field_50_throw_charge >= 0;
    }

    inline void ResetThrowCharge_4A5180()
    {
        field_50_throw_charge = 0;
    }

    inline void SetDeathType_434950(s32 death_type)
    {
        field_44_death_type = death_type;
    }

    // 9.6f 0x4A5100
    bool IsRemoteControlActive_4A5100() const
    {
        s32 occupation;
        if (!field_2D0_aux_cam_active || !field_2C8_aux_ped ||
            (occupation = field_2C8_aux_ped->get_occupation_403980(), occupation != ped_ocupation_enum::empty))
        {
            return false;
        }
        return true;
    }

    // TODO: Ordering
    EXPORT s32 ObjectTypeToWeaponType_443CB0(u8 shop_type);

    EXPORT u8 GetIdx_4881E0();
    EXPORT void AddCarToHistory_5645B0(Car* pNewCar);
    EXPORT bool PromoteCarInHistory_564610(Car* pCar, bool dont_modify);
    EXPORT void RemoveCarFromHistory_564680(Car* pCar);
    EXPORT void SetKFCarWeapon_564710(Car* pCar, s32 weapon_kind);
    EXPORT void SetKFWeapon_564790(s32 weapon_idx);
    EXPORT void ClearKFWeapon_5647D0();
    EXPORT Weapon* GetCurrPlayerWeapon_5648F0();
    EXPORT void SetWeapon_564910(Weapon* pWeapon);
    EXPORT bool HasAnyAmmo_564940();
    EXPORT bool AddWeaponWithAmmo_564960(s32 weapon_kind, u8 ammo);
    EXPORT void SelectNextOrPrevWeapon_5649D0(bool go_forwards, bool go_backwards);
    EXPORT void LoadCarWeapons_564AD0(Car* pCar);
    EXPORT void ClearCarWeapons_564B60();
    EXPORT void CleanupEmptyAmmoWeapons_564B80();
    EXPORT void UnloadCarWeapons_564C00();
    EXPORT void RemovePlayerWeapons_564C50();
    EXPORT void ClearPowerUps_564CC0();
    EXPORT void ClearPowerUpsExceptJailCard_564CF0();
    EXPORT bool CollectPowerUp_564D60(s32 power_up_idx);
    EXPORT void tick_down_powerups_565070();
    EXPORT void RestorePowerUpsFromSave_5651F0(SavedPlayerStats* pSave);
    EXPORT void TeleportToDebugCam_565310();
    EXPORT void DebugWatchNearestCar_5653E0();
    EXPORT void sub_565460();
    EXPORT void InitPlayerPed_565490(Ped* pPed);
    EXPORT void SetInputs_565740(u32 inputs);
    EXPORT void IncrementGangRespectFromDebugKeys_565770(u8 count);
    EXPORT void IncreaseWantedLevelFromDebugKeys_565860();
    EXPORT void Hud_Controls_565890(u16 action);
    EXPORT void HandleKeyRelease_566380(u16 key);
    EXPORT void CharacterControls_566520();
    EXPORT void ControlInputs_566820();
    EXPORT void HandleControls_5668D0(Ped* pPed);
    EXPORT void DoCarControlInputs_566C30(Car* pCar);
    EXPORT void DoPedControlInputs_566C80(Ped* pPed);
    EXPORT void ShowDebugInfo_566EE0(bool skip);
    EXPORT void RespawnPlayer_5670B0();
    EXPORT void Wasted_567130();
    EXPORT void UpdateAuxPedDeath_567850();
    EXPORT void Busted_5679E0();
    EXPORT void UpdateCurrentZones_568520();
    EXPORT void UpdateSoundListener_568630();
    EXPORT void HandleDebugZoom_568670();
    EXPORT void UpdateCamera_5686D0(Camera* pCam);
    EXPORT void Disconnect_568730();
    EXPORT void Service_5687F0();
    EXPORT void UpdatePaused_569410();
    EXPORT void EndRemoteControl_569530();
    EXPORT void ResetAuxCamera_5695A0();
    EXPORT void StartRemoteControl_569600(Car* pCar);
    EXPORT void WatchCar_5696D0(Car* pCar);
    EXPORT void GetPosU8_569840(u8& x, u8& y, u8& z);
    EXPORT Car* GetPlayerCar_5698E0();
    EXPORT void get_pos_569920(Fix16* out_x, Fix16* out_y, Fix16* out_z);
    EXPORT void ChangeLifeCountByAmount_5699F0(s32 amount);
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
    EXPORT void CopyPlayerDataToSave_56A1A0(SavedPlayerStats* pSave);
    EXPORT void UpdateGameFromSave_56A310(SavedPlayerStats* pSave);
    EXPORT void ApplyCheats_56A490();
    EXPORT void ClearInputs_56A6D0();

    // 0x56A740
    EXPORT Player(u8 player_idx);

    // 0x56A940
    EXPORT ~Player();

    inline bool IsUser_41DC70() const
    {
        return field_0_is_user;
    }

    inline Ped* GetPlayerPed_4A5130() const
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

    inline u8 get_idx_4219D0() const
    {
        return field_2E_idx;
    }

    // 9.6f 0x4A5150
    inline Ped* GetActivePed_4A5150() const
    {
        return (field_68_camera_mode == 2 || field_68_camera_mode == 3) ? field_2C8_aux_ped : field_2C4_player_ped;
    }

    // 9.6f 0x434900
    inline Camera* get_camera_434900()
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

    bool field_0_is_user;
    char_type field_1_unused;
    char_type field_2_unused;
    u32 field_4_inputs; // this tick's input bits (SetInputs_565740); zeroed once the tick has used them
    Ang16 field_8_turn_speed;
    Ang16 field_A_turn_accel;
    Fix16 field_C_move_direction;
    s32 field_10_unused;
    s16 field_14_saved_ped_weapon_idx;
    s16 field_16_saved_car_weapon_idx;

    s16 field_18_pre_kf_weapon_kind;
    s16 field_1A_pre_kf_ammo;
    s32 field_1C_kf_weapon_kind;
    Car* field_20_kf_car;
    s32 field_24_kf_car_id;

    bool field_28_wasted_or_busted;
    bool field_29_aux_ped_dying;
    char_type field_2A_pad;
    char_type field_2B_pad;
    s16 field_2C_death_countdown;
    u8 field_2E_idx;
    bool field_2F_disable_all_controls;
    bool field_30_disable_enter_vehicles;
    bool field_31_kf_weapon_mode;
    Gang* field_34_gang_curr_location;
    gmp_map_zone* field_38_local_navigation_zone;
    gmp_map_zone* field_3C_navigation_zone;
    gmp_map_zone* field_40_arrow_blocker_zone;
    s32 field_44_death_type;
    bool field_48_dbg_cam_follow_player;
    SoundObject* field_4C_sound_obj;
    s32 field_50_throw_charge;
    Car* field_54_car_history[3];
    s32 field_60_finish_score_reached;
    bool field_64_jumping;
    s32 field_68_camera_mode;
    s32 field_6C_in_debug_cam_mode;
    bool field_70_dbg_cam_north;
    bool field_71_dbg_cam_south;
    bool field_72_dbg_cam_west;
    bool field_73_dbg_cam_east;
    bool field_74_dbg_cam_zooming_out;
    bool field_75_dbg_cam_zooming_in;
    bool field_76_dbg_zoom_in;
    bool field_77_dbg_zoom_out;

    // Current inputs state
    bool field_78_now_forward_pressed;
    bool field_79_now_down_pressed;
    bool field_7A_now_left_pressed;
    bool field_7B_now_right_pressed;
    bool field_7C_now_attack_pressed;
    bool field_7D_now_enter_exit_pressed;
    bool field_7E_now_hand_brake_or_jump_pressed;
    bool field_7F_now_prev_weapon_pressed;
    bool field_80_now_next_weapon_pressed;
    bool field_81_now_special_1_pressed;
    bool field_82_now_special_2_pressed;
    bool field_83_now_special_3_pressed;

    // Previous inputs state
    bool field_84_special_1_changed;
    bool field_85_special_2_changed;
    bool field_86_special_3_changed;
    bool field_87_next_weapon_changed;
    bool field_88_prev_weapon_changed;
    bool field_89_enter_exit_changed;
    bool field_8A_hand_brake_or_jump_changed;
    bool field_8B_forward_changed;
    bool field_8C_down_changed;
    bool field_8D_attack_changed;

    bool field_8E_in_use;
    bool field_8F_block_attack;
    Camera field_90_game_camera;
    Camera field_14C_view_camera;
    Camera field_208_aux_game_camera;
    Ped* field_2C4_player_ped;
    Ped* field_2C8_aux_ped;
    Car* field_2CC_watched_car;
    bool field_2D0_aux_cam_active;
    PlayerScoreTracker field_2D4_scores;
    bool field_640_busted;
    PlayerCrimeStats field_644_crime_stats;
    u16 field_680_traffic_spawn_counter;
    u16 field_682_traffic_spawn_threshold;
    RollingDigitCounter field_684_lives;
    RollingDigitCounter field_6BC_multipliers;
    u16 field_6F4_power_up_timers[17];
    s16 field_716_unused;
    Weapon* field_718_weapons[28];
    s16 field_788_curr_weapon_idx;
    bool field_78A_show_quit_message;
    s32 field_78C_hud_palette_type; // Usage: 2 = default, 7 = multiplayer if using gang remap
    u16 field_790_hud_palette;
    s16 field_792_unused;
    bool field_794_is_chatting;
    s16 field_796_chat_text[79];
    s32 field_834_unused;
    s32 field_838_chat_text_idx;
    wchar_t field_83C_player_name[16];
};

// 9.6f 0x434B60
inline bool TagGame::IsFugitivePed_434B60(Ped* pPed)
{
    return field_0_fugitive && field_0_fugitive->GetPlayerPed_41D020() == pPed;
}
