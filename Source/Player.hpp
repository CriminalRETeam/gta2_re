#pragma once

#include "Draw.hpp"
#include "DrawUnk_0xBC.hpp"
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

// TODO: add these later
class Car_BC;
class Gang_144;

class Player
{
  public:
    inline void Add_2D4(s32 score)
    {
        field_2D4_unk.AddCash_592620(score * field_6BC_multpliers.field_0);
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

    EXPORT u8 GetIdx_4881E0();
    EXPORT void sub_5645B0(Car_BC* a2);
    EXPORT char_type sub_564610(Car_BC* a2, char_type a3);
    EXPORT u32* sub_564680(s32 a2);
    EXPORT void sub_564710(Car_BC* pCar, s32 weapon_kind);
    EXPORT void sub_564790(s32 idx);
    EXPORT void ClearKFWeapon_5647D0();
    EXPORT Weapon_30* GetCurrPlayerWeapon_5648F0();
    EXPORT void sub_564910(Weapon_30* a2);
    EXPORT char_type HasAnyAmmo_564940();
    EXPORT char_type sub_564960(s32 a2, u8 a3);
    EXPORT u16 sub_5649D0(char_type a2, char_type a3);
    EXPORT void sub_564AD0(Car_BC* a2);
    EXPORT void sub_564B60();
    EXPORT s32 sub_564B80();
    EXPORT void sub_564C00();
    EXPORT void RemovePlayerWeapons_564C50();
    EXPORT void sub_564CC0();
    EXPORT void sub_564CF0();
    EXPORT char_type sub_564D60(s32 a2);
    EXPORT void tick_down_powerups_565070();
    EXPORT s32 sub_5651F0(s32 a2);
    EXPORT void sub_565310();
    EXPORT void sub_5653E0();
    EXPORT void sub_565460();
    EXPORT s32 sub_565490(Ped* pPed);
    EXPORT void SetInputs_565740(u32 input);
    EXPORT void sub_565770(u8 count);
    EXPORT void sub_565860();
    EXPORT void Hud_Controls_565890(u16 action);
    EXPORT void sub_566380(u16 a2);
    EXPORT char_type CharacterControls_566520();
    EXPORT void ControlInputs_566820();
    EXPORT void sub_5668D0(Ped* a2);
    EXPORT void sub_566C30(Car_BC* pCar);
    EXPORT char_type sub_566C80(Ped* a2);
    EXPORT void sub_566EE0(char_type a2);
    EXPORT void sub_5670B0();
    EXPORT void sub_567130();
    EXPORT void sub_567850();
    EXPORT void sub_5679E0();
    EXPORT void sub_568520();
    EXPORT void sub_568630();
    EXPORT char_type sub_568670();
    EXPORT void sub_5686D0(DrawUnk_0xBC* pCam);
    EXPORT void sub_568730();
    EXPORT void sub_5687F0();
    EXPORT s32 sub_569410();
    EXPORT void sub_569530();
    EXPORT void sub_5695A0();
    EXPORT char_type sub_569600(Car_BC* a2);
    EXPORT void sub_5696D0(Car_BC* pCar);
    EXPORT void sub_569840(u8& a2, u8& a3, u8& a4);
    EXPORT Car_BC* sub_5698E0();
    EXPORT s32 sub_569920(u32* a2, int* a3, int* a4);
    EXPORT void sub_5699F0(s32 a2);
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
    EXPORT s32 sub_56A0F0();
    EXPORT zealous_borg* CopyPlayerDataToSave_56A1A0(s32 a2);
    EXPORT void UpdateGameFromSave_56A310(s32 a2);
    EXPORT void sub_56A490();
    EXPORT void sub_56A6D0();

    // 0x56A740
    EXPORT Player(u8 a2);

    // 0x56A940
    EXPORT ~Player();

    char_type field_0;
    char_type field_1;
    char_type field_2;
    char_type field_3;
    s32 field_4_inputs;
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
    Gang_144* field_34_pObj;
    gmp_map_zone* field_38;
    gmp_map_zone* field_3C;
    gmp_map_zone* field_40;
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
    char_type field_71;
    char_type field_72;
    char_type field_73;
    char_type field_74_dbg_cam_zooming_out;
    char_type field_75_dbg_cam_zooming_in;
    char_type field_76;
    char_type field_77;
    char_type field_78_bForwardGasOn;
    char_type field_79_bFootBrakeOn;
    char_type field_7A;
    char_type field_7B;
    char_type field_7C;
    char_type field_7D;
    char_type field_7E_bHandBrakeOn;
    char_type field_7F;
    char_type field_80;
    char_type field_81;
    char_type field_82;
    char_type field_83;
    char_type field_84;
    char_type field_85;
    char_type field_86;
    char_type field_87;
    char_type field_88;
    char_type field_89;
    char_type field_8A;
    char_type field_8B;
    char_type field_8C;
    char_type field_8D;
    char_type field_8E_bInUse;
    char_type field_8F;
    DrawUnk_0xBC field_90_game_camera;
    DrawUnk_0xBC field_14C_view_camera;
    DrawUnk_0xBC field_208_aux_game_camera;
    Ped* field_2C4_player_ped;
    Ped* field_2C8_unkq;
    Car_BC* field_2CC;
    char_type field_2D0;
    char_type field_2D1;
    char_type field_2D2;
    char_type field_2D3;
    eager_benz field_2D4_unk;
    char_type field_640;
    char_type field_641;
    char_type field_642;
    char_type field_643;
    zealous_borg field_644_unk;
    u16 field_680;
    u16 field_682;
    thirsty_lamarr field_684_lives;
    thirsty_lamarr field_6BC_multpliers;
    u16 field_6F4[17];
    s16 field_716;
    Weapon_30* field_718_weapons[28];
    s16 field_788_curr_weapon_idx;
    char_type field_78A_show_quit_message;
    char_type field_78B;
    DrawKind field_78C;
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