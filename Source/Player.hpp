#pragma once

#include "Draw.hpp"
#include "Camera.hpp"
#include "Function.hpp"
#include "ang16.hpp"
#include "eager_benz.hpp"
#include "fix16.hpp"
#include "sad_mirzakhani.hpp"
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

enum power_up_indices
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
    Unk_12 = 12,
    Unk_13 = 13,
    Unk_14 = 14,
    Unk_15 = 15,
    Unk_16 = 16,
};

class Player
{
  public:
    inline void Add_2D4(s32 score)
    {
        field_2D4_scores.AddCash_592620(score * field_6BC_multpliers.field_0);
    }

    inline Ped* Get_Field_68_Ped()
    {
        return field_68 == 2 ? field_2C8_unkq : field_2C4_player_ped;
    }

    // 0x4CCAE0
    s32 Get_Field_50()
    {
        return field_50;
    }

    // 0x4CCAB0
    void sub_4CCAB0()
    {
        s32 t = field_50;
        if (t >= 0)
        {
            this->field_50 = t + 1;
        }

        if (this->field_50 > 270)
        {
            this->field_50 = 270;
        }
    }

    s32 sub_4CCAD0()
    {
        s32 f50_or_k60 = this->field_50;
        if (f50_or_k60 <= 0x3C)
        {
            return f50_or_k60;
        }
        else
        {
            return 0x3C;
        }
    }

    // 9.6f inline 0x41D020
    inline Ped* GetPlayerPed_41D020()
    {
        return field_2C4_player_ped;
    }

    inline void sub_4A5180()
    {
        field_50 = 0;
    }

    inline void set_death_type_434950(s32 type)
    {
        field_44_death_type = type;
    }

    bool sub_4A5100()
    {
        s32 occupation;
        if (!field_2D0 || !field_2C8_unkq || (occupation = field_2C8_unkq->get_occupation_403980(), occupation != ped_ocupation_enum::empty))
        {
            return false;
        }
        return true;
    }

    // TODO: Ordering
    EXPORT s32 sub_443CB0(u8 varrok);

    EXPORT u8 GetIdx_4881E0();
    EXPORT void sub_5645B0(Car_BC* a2);
    EXPORT char_type sub_564610(Car_BC* a2, char_type a3);
    EXPORT u32* sub_564680(Car_BC* a2);
    EXPORT void sub_564710(Car_BC* pCar, s32 weapon_kind);
    EXPORT void sub_564790(s32 idx);
    EXPORT void ClearKFWeapon_5647D0();
    EXPORT Weapon_30* GetCurrPlayerWeapon_5648F0();
    EXPORT void sub_564910(Weapon_30* a2);
    EXPORT char_type HasAnyAmmo_564940();
    EXPORT char_type sub_564960(s32 a2, u8 a3);
    EXPORT void SelectNextOrPrevWeapon_5649D0(char_type bFowards, char_type bBackwards);
    EXPORT void sub_564AD0(Car_BC* a2);
    EXPORT void sub_564B60();
    EXPORT s32 sub_564B80();
    EXPORT void sub_564C00();
    EXPORT void RemovePlayerWeapons_564C50();
    EXPORT void sub_564CC0();
    EXPORT void sub_564CF0();
    EXPORT char_type sub_564D60(s32 a2);
    EXPORT void tick_down_powerups_565070();
    EXPORT s32 RestorePowerUpsFromSave_5651F0(save_stats_0x90* a2);
    EXPORT void TeleportToDebugCam_565310();
    EXPORT void sub_5653E0();
    EXPORT void sub_565460();
    EXPORT void InitPlayerPed_565490(Ped* pPed);
    EXPORT void SetInputs_565740(u32 input);
    EXPORT void IncrementGangRespectFromDebugKeys_565770(u8 count);
    EXPORT void IncreaseWantedLevelFromDebugKeys_565860();
    EXPORT void Hud_Controls_565890(u16 action);
    EXPORT void sub_566380(u16 a2);
    EXPORT void CharacterControls_566520();
    EXPORT void ControlInputs_566820();
    EXPORT void HandleControls_5668D0(Ped* a2);
    EXPORT void DoCarControlInputs_566C30(Car_BC* pCar);
    EXPORT void DoPedControlInputs_566C80(Ped* a2);
    EXPORT void sub_566EE0(char_type a2);
    EXPORT void RespawnPlayer_5670B0();
    EXPORT void Wasted_567130();
    EXPORT void sub_567850();
    EXPORT void Busted_5679E0();
    EXPORT void UpdateCurrentZones_568520();
    EXPORT void sub_568630();
    EXPORT void sub_568670();
    EXPORT void UpdateCamera_5686D0(Camera_0xBC* pCam);
    EXPORT void sub_568730();
    EXPORT void Service_5687F0();
    EXPORT void sub_569410();
    EXPORT void sub_569530();
    EXPORT void sub_5695A0();
    EXPORT void sub_569600(Car_BC* pCar);
    EXPORT void sub_5696D0(Car_BC* pCar);
    EXPORT void sub_569840(u8& a2, u8& a3, u8& a4);
    EXPORT Car_BC* sub_5698E0();
    EXPORT void get_pos_569920(Fix16* a2, Fix16* a3, Fix16* a4);
    EXPORT void ChangeLifeCountByAmount_5699F0(s32 a2);
    EXPORT void sub_569A10();
    EXPORT void sub_569C20();
    EXPORT void sub_569CB0();
    EXPORT void sub_569E70();
    EXPORT char* GetDeathText_569F00();
    EXPORT s32 sub_569F40();
    EXPORT s32 DisableAllControls_569FF0();
    EXPORT void EnableAllControls_56A000();
    EXPORT void EnableKFMode_56A010();
    EXPORT void DisableKFMode_56A020();
    EXPORT void DisableEnterVehicles_56A030();
    EXPORT void EnableEnterVehicles_56A040();
    EXPORT s32 RestoreCarsFromSave_56A0F0();
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

    u8 field_0_bIsUser;
    char_type field_1;
    char_type field_2;
    char_type field_3;
    u32 field_4_inputs;
    Ang16 field_8;
    Ang16 field_A;
    Fix16 field_C;
    s32 field_10;
    s16 field_14;
    s16 field_16;
    s16 field_18;
    s16 field_1A_ammo; // of last weapon ?
    s32 field_1C_weapon_kind;
    Car_BC* field_20_car;
    s32 field_24;
    char_type field_28;
    char_type field_29;
    char_type field_2A;
    char_type field_2B;
    s16 field_2C;
    u8 field_2E_idx;
    char_type field_2F_disable_all_controls;
    char_type field_30_disable_enter_vehicles;
    char_type field_31_kf_weapon_mode;
    char_type field_32;
    char_type field_33;
    Gang_144* field_34_gang_curr_location;
    gmp_map_zone* field_38_local_navigation_zone;
    gmp_map_zone* field_3C_navigation_zone;
    gmp_map_zone* field_40_arrow_blocker_zone;
    s32 field_44_death_type;
    char_type field_48_bDbg_cam_follow_player;
    char_type field_49;
    char_type field_4A;
    char_type field_4B;
    infallible_turing* field_4C_pUnk;
    s32 field_50;
    Car_BC* field_54_unk[3];
    s32 field_60;
    char_type field_64;
    char_type field_65;
    char_type field_66;
    char_type field_67;
    s32 field_68;
    s32 field_6C_bIn_debug_cam_mode;
    char_type field_70_dbg_cam_north;
    char_type field_71_s;
    char_type field_72_e;
    char_type field_73_w;
    char_type field_74_dbg_cam_zooming_out;
    char_type field_75_dbg_cam_zooming_in;
    char_type field_76;
    char_type field_77;

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

    char_type field_8E_bInUse;
    char_type field_8F;
    Camera_0xBC field_90_game_camera;
    Camera_0xBC field_14C_view_camera;
    Camera_0xBC field_208_aux_game_camera;
    Ped* field_2C4_player_ped;
    Ped* field_2C8_unkq;
    Car_BC* field_2CC;
    char_type field_2D0;
    char_type field_2D1;
    char_type field_2D2;
    char_type field_2D3;
    eager_benz field_2D4_scores;
    char_type field_640_busted;
    char_type field_641;
    char_type field_642;
    char_type field_643;
    zealous_borg field_644_unk;
    u16 field_680;
    u16 field_682;
    thirsty_lamarr field_684_lives;
    thirsty_lamarr field_6BC_multpliers;
    u16 field_6F4_power_up_timers[17];
    s16 field_716;
    Weapon_30* field_718_weapons[28];
    s16 field_788_curr_weapon_idx;
    char_type field_78A_show_quit_message;
    char_type field_78B;
    s32 field_78C;
    u16 field_790;
    s16 field_792;
    char_type field_794_is_chatting;
    char_type field_795;
    s16 field_796_chat_text[79];
    s32 field_834;
    s32 field_838_f796_idx;
    wchar_t field_83C_player_name;
    char_type field_83E;
    char_type field_83F;
    s32 field_840;
    s32 field_844;
    s32 field_848;
    s32 field_84C;
    s32 field_850;
    s32 field_854;
    s32 field_858;
};