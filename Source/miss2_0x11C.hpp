#pragma once

#include "Function.hpp"
#include "ang16.hpp"
#include "fix16.hpp"
#include "Fix16_Point.hpp"
#include "miss2_xyz.hpp"
#include "Pool.hpp"

class Ped;
class Generator_2C;
class Crusher_30;
class miss2_8;
class Car_BC;
class Object_2C;
class Crane_15C;
class nostalgic_ellis_0x28;
class Door_38;
class miss2_0x11C;
class Hud_Arrow_7C;
class Sprite_18;

struct SCR_CMD_HEADER
{
    s16 field_0_cmd_this;
    u16 field_2_type;
    s16 field_4_cmd_next;
    s16 field_6_return_value;
};

struct SCR_ONEVAR_RECT : SCR_CMD_HEADER
{
    u16 field_8_varname;
    u16 field_A_unk;
    SCR_Rect_f field_C_rect;
};

struct SCR_PLAYER_PED
{
    SCR_CMD_HEADER field_0_header;
    Ped* field_8_ped;
    SCR_XYZ_f field_C_pos;
    Ang16 field_18_rot;
    s16 field_1A_remap;
};

struct SCR_CAR_DATA_DEC : SCR_CMD_HEADER
{
    u16 field_8_car_idx;
    u16 field_A_unk;
    SCR_XYZ_f field_C_pos;
    Ang16 field_18_rot;
    s16 field_1A_remap;
    s16 field_1C_car_id;
    s16 field_1E_trailer_id;
};

struct SCR_CHAR_DATA_DEC : SCR_CMD_HEADER
{
    Ped* field_8_char;
    SCR_XYZ_f field_C_pos;
    Ang16 field_18_rot;
    s16 field_1A_remap;
    s16 field_1C_occupation;
    u16 field_1E_padding;
};

struct SCR_OBJ_DATA : SCR_CMD_HEADER
{
    Object_2C* field_8_obj;
    SCR_XYZ_f field_C_pos;
    u16 field_18_obj_id;
    Ang16 field_1A_rot;
    u8 field_1C_value_shop_type;
};

struct SCR_TWO_PARAMS : SCR_CMD_HEADER
{
    union
    {
        struct
        {
            u16 field_8_unsigned_1;
            u16 field_A_unsigned_2;
        };
        struct
        {
            s16 field_8_signed_1;
            s16 field_A_signed_2;
        };
        u32 field_8_u32;
        s32 field_8_s32;
    };
};

struct SCR_FOUR_PARAMS : SCR_CMD_HEADER
{
    union
    {
        struct
        {
            u16 field_8_unsigned_1;
            u16 field_A_unsigned_2;
        };
        struct
        {
            s16 field_8_signed_1;
            s16 field_A_signed_2;
        };
    };
    union
    {
        struct
        {
            u16 field_C_unsigned_3;
            u16 field_E_unsigned_4;
        };
        struct
        {
            s16 field_C_signed_3;
            s16 field_E_signed_4;
        };
        u32 field_C_u32;
        s32 field_C_s32;
    };
};

struct SCR_POINTER : SCR_CMD_HEADER
{
    union
    {
        Ped* field_8_char;
        Car_BC* field_8_car;
        Object_2C* field_8_obj;
        Crane_15C* field_8_crane;
        nostalgic_ellis_0x28* field_8_light;
        Door_38* field_8_door;
        Crusher_30* field_8_crusher;
        s32 field_8_counter;
        u16 field_8_index;
        Generator_2C* field_8_generator;
        Hud_Arrow_7C* field_8_arrow;
        Sprite_18* field_8_sprite;
    };
};

struct SCR_THREAD : SCR_CMD_HEADER
{
    miss2_0x11C* field_8_script_thread;
    s16 field_C_unknown;
    u16 field_E;
};

struct SCR_CHAR_OBJECTIVE : SCR_CMD_HEADER
{
    u16 field_8_char_idx; //  SCR_SET_CHAR_OBJ1
    s16 field_A_objective;
    union
    {
        struct
        {
            u16 field_C_second_item_idx; //  SCR_SET_CHAR_OBJ2
            u16 field_E_variant;
        };
        struct
        {
            u16 field_C_car_idx; //  SET_CHAR_OBJ_FOLLOW
            u16 field_E_rotation;
            s32 field_12_offset;
        };
    };
};

struct SCR_CHAR_OBJ3 : SCR_CMD_HEADER
{
    u16 field_8_char_idx;
    s16 field_A_objective;
    SCR_XYZ_f field_C_pos; //  SCR_SET_CHAR_OBJ3
};

struct SCR_EXPLODE : SCR_CMD_HEADER
{
    SCR_XYZ_f field_8_pos;
};

struct SCR_EXPLODE_WALL : SCR_CMD_HEADER
{
    SCR_XYZ_f field_8_pos;
    u8 field_14_face;
    u8 field_15_padding1;
    u16 field_16_padding2;
};

struct SCR_SOUND_DECSET : SCR_CMD_HEADER
{
    u16 field_8_varname;
    u16 field_A_unk;
    SCR_XYZ_f field_C_pos;
    u8 field_18_sound_id;
    u8 field_19_play_type;
    u16 field_1B_padding;
};

struct SCR_SET_STATION : SCR_CMD_HEADER
{
    u16 field_8_platform;
    u8 field_A_num_passenger;
    u8 field_B_num_freight;
    u8 field_C_num_boxcar;
    u8 field_D_padding1;
    u16 field_E_padding2;
};

struct SCR_RADIOSTATION_DEC : SCR_CMD_HEADER
{
    SCR_XY_f field_8_pos;
    u8 field_10_station_type;
    u8 field_11_padding;
    u16 field_12_padding;
};

struct SCR_DECLARE_CRANE_POWERUP : SCR_CMD_HEADER
{
    u16 field_8_crane_idx;
    u16 field_A_generator;
    SCR_XYZ_uc field_C_pos;
    u8 field_18_padding;
};

struct SCR_MAP_ZONE_SET : SCR_CMD_HEADER
{
    u16 field_8_car_density;
    u16 field_A_goodcar_ratio;
    u16 field_C_badcar_ratio;
    u16 field_E_policecar_ratio;
    u16 field_10_ped_density;
    u16 field_12_mugger_ratio;
    u16 field_14_carthief_ratio;
    u16 field_16_elvis_ratio;
    u16 field_18_gangchar_ratio;
    u16 field_1A_policeped_ratio;
    u16 field_1C_gangcar_ratio;
    u16 field_1E_padding;
};

struct SCR_SET_GANG_INFO : SCR_CMD_HEADER
{
    u16 field_8_gangname;
    s8 field_A_remap;
    u8 field_B_weapon1;
    u8 field_C_weapon2;
    u8 field_D_weapon3;
    u8 field_E_arrow_id;
    u8 field_F_kill_respect_change;
    SCR_XYZ_f field_10_pos;
    u16 field_1C_car_model;
    s16 field_1E_car_remap;
};

struct SCR_GENERATOR : SCR_CMD_HEADER
{
    u16 field_8_unk2;
    u16 field_A_unk3;
    SCR_XYZ_f field_C_pos;
    Ang16 field_18_rot;
    u16 field_1A_obj_id;
    u16 field_1C_mindelay;
    u16 field_1E_maxdelay;
    u8 field_20_ammo;
    u8 field_21_padding;
    u16 field_22_padding;
};

struct SCR_PHONE_TEMPLATE : SCR_CMD_HEADER
{
    u16 field_8_brief_id;
    u16 field_A_counter1;
    u16 field_C_counter3b;
    u16 field_E_mission2;
    u16 field_10_counter2;
    u16 field_12_counter3;
    u16 field_14_counter4;
    u16 field_16_counter5;
    u16 field_18_gangname;
    u16 field_1A_respect;
    u16 field_1C_mission1;
    u16 field_1E_padding;
};

struct SCR_LOWER_LEVEL : SCR_CMD_HEADER
{
    SCR_XY_uc field_8_min_pos;
    SCR_XY_uc field_A_max_pos;
};

struct SCR_ADD_BLOCK : SCR_CMD_HEADER
{
    u8 field_8_xpos;
    u8 field_9_ypos;
    u8 field_A_zpos;
};

struct SCR_REMOVE_BLOCK : SCR_CMD_HEADER
{
    SCR_XYZ_uc field_8_pos;
    u8 field_B_do_drop;
};

struct SCR_CONVEYOR : SCR_CMD_HEADER
{
    u16 field_8_unk2;
    u16 field_A_unk3;
    SCR_Rect_f field_C_rect;
    SCR_XY_uc field_18_speed;
    u16 field_1A_padding;
};

struct SCR_DESTRUCTOR : SCR_CMD_HEADER
{
    u16 field_8_unk2;
    u16 field_A_unk3;
    SCR_Rect_f field_C_rect;
};

struct SCR_CRUSHER_BASIC : SCR_CMD_HEADER
{
    u16 field_8_unk2;
    u16 field_A_unk3;
    SCR_XY_f field_C_pos;
};

struct SCR_SET_AMBIENT : SCR_CMD_HEADER
{
    s32 field_8_level;
    u16 field_C_time;
    u16 field_E_padding;
};

struct SCR_IF_JUMP : SCR_CMD_HEADER
{
    u8 is_or;
    u8 unknown;
    u16 else_endif_pointer;
};

struct SCR_DOOR_DATA_DEC : SCR_CMD_HEADER
{
    Door_38* field_8;
    SCR_XYZ_uc field_C_block;
    u8 field_F_face;
    u8 field_10_gr_id;
    u8 field_11_open_type; // SCR_DOOR_OPENTYPES
    u8 field_12_close_type; // SCR_DOOR_CLOSETYPES
    u8 field_13_delay;
    SCR_Rect_f field_14_check;
    u8 field_28_flip;
    u8 field_29_reversed;
    u16 field_30_somename; // id of car/model/char or integer value (int is not supported by gta2 according to docs)
};

struct SCR_UPDATE_DOOR : SCR_CMD_HEADER
{
    u16 field_8_door_idx;
    u16 field_A_target_idx;
};

struct SCR_SET_CHAR_GRAPHIC_TYPE : SCR_CMD_HEADER
{
    u16 field_8_varname;
    s16 field_A_remap;
    u16 field_C_graphic_type;
    u16 field_E_padding;
};

struct SCR_LEVEL_END_ARROW2 : SCR_CMD_HEADER
{
    SCR_XYZ_f field_8_pos;
    u16 field_14_arrowname;
    u16 field_16_padding;
};

struct SCR_IS_CAR_IN_BLOCK : SCR_CMD_HEADER
{
    u16 field_8_varname;
    u16 field_A_unk2;
    SCR_XYZ_f field_C_pos;
};

struct SCR_CHECK_SCORE_GREATER : SCR_CMD_HEADER
{
    u32 field_8_unknown;
    s32 field_C_target_score;
};

struct SCR_WARP_FROM_CAR : SCR_CMD_HEADER
{
    u16 field_8_char_idx;
    Ang16 field_A_rotation;
    SCR_XYZ_f field_C_pos;
};

struct SCR_SET_GROUP_TYPE : SCR_CMD_HEADER
{
    u32 field_8_unk;
    u32 field_C_group_type;
};

struct SCR_CHECK_CURRENT_WEAPON : SCR_CMD_HEADER
{
    u16 field_8_ped_idx;
    u16 field_A_padding;
    u32 field_C_weapon_idx;
};

struct SCR_DECLARE_POLICELEVEL : SCR_CMD_HEADER
{
    u16 field_8_unk;
    u16 field_A_wanted_level;
};

struct SCR_ADD_ONSCREEN_COUNTER : SCR_CMD_HEADER
{
    u16 field_8_onscreen_counter_idx;
    u16 field_A_counter_idx;
};

struct SCR_SET_KF_WEAPON : SCR_CMD_HEADER
{
    u16 field_8_ped_idx;
    u16 field_A_unk;
    u32 field_C_weapon_idx;
};

struct SCR_SET_FAV_CAR : SCR_CMD_HEADER
{
    u16 field_8_ped_idx;
    u16 field_A_unk;
    u32 field_C_favourite_car;
};

struct SCR_SET_PLAYER_CONTROLS : SCR_CMD_HEADER
{
    u16 field_8_ped_idx;
    u8 field_A_new_state;
};

struct SCR_STORE_BONUS_COUNT : SCR_CMD_HEADER
{
    u16 field_8_bonus;
    u16 field_A_counter_idx;
};

struct SCR_SET_STATE : SCR_CMD_HEADER
{
    u16 field_8_unk;
    u8 field_A_status;
};

struct SCR_SET_DIR_OF_TVVAN : SCR_CMD_HEADER
{
    SCR_XY_f field_8_pos;
};

struct SCR_CHECK_WEAPONHIT : SCR_CMD_HEADER
{
    union
    {
        u16 field_8_car_idx;
        u16 field_8_ped_idx;
    };
    u16 field_A_status;
    u16 field_C_weapon;
};

struct SCR_FINISH_LEVEL : SCR_CMD_HEADER
{
    u16 field_8_unk;
    s16 field_A_bonus_type;
};

struct SCR_CHECK_HEADS_GREATER : SCR_CMD_HEADER
{
    u16 field_8_ped_idx;
    s16 field_A_wanted_level;
};

struct SCR_SET_CAR_GRAPHIC : SCR_CMD_HEADER
{
    u16 field_8_car_idx;
    u16 field_A_unk;
    u16 field_C_number;
};

struct SCR_CHANGE_BLOCK : SCR_CMD_HEADER
{
    u8 field_8_x;
    u8 field_9_y;
    u8 field_A_z;
    u8 field_B_change_type;
    u16 field_C_info_word;
};

struct SCR_CHECK_CAR_SPEED : SCR_CMD_HEADER
{
    u16 field_8_car_idx;
    s32 field_A_value;
};

struct SCR_MAP_SET_ZONE_INFO : SCR_CMD_HEADER
{
    u16 field_8_zone_idx;
    u16 field_A_unknown;
    u16 field_C_value;
};

struct SCR_START_BASIC_KF_TEMPLATE : SCR_CMD_HEADER
{
    u16 field_8_triggername;
    u16 field_A_brief_id;
    u16 field_C_objname;
    u16 field_E_playername;
    u16 field_10_weapon;
    u16 field_12_unk2;
};

struct SCR_DO_BASIC_KF_TEMPLATE : SCR_CMD_HEADER
{
    u16 field_8_bonusname;
    u16 field_A_timername;
    u32 field_C_time_limit;
    u16 field_10_onscreenname;
    u16 field_12_countername;
    u16 field_14_target_total;
    u16 field_16_brief_id;
    u16 field_18_playername;
    u16 field_1A_rewardtype;
    u32 field_1C_rewardvalue;
};

struct SCR_SET_CAR_JAMMED : SCR_CMD_HEADER
{
    u16 field_8_car_idx;
    u8 field_A_status;
};

struct SCR_SET_CAR_DAMAGE_PROOF : SCR_CMD_HEADER
{
    u16 field_8_car_idx;
    u8 field_A_status;
};

struct SCR_SET_RUN_SPEED : SCR_CMD_HEADER
{
    u16 field_8_ped_idx;
    Fix16 field_A_value;
};

struct SCR_SET_CHAR_DRIVE_AGGR : SCR_CMD_HEADER
{
    u16 field_8_ped_idx;
    u8 field_A_status;
};

struct SCR_OPERATE_COUNTER_AND_INT : SCR_CMD_HEADER
{
    u16 field_8_destination_counter_idx;
    u16 field_A_operand_counter_idx;
    s16 field_C_value;
};

struct SCR_OPERATE_COUNTER_AND_COUNTER : SCR_CMD_HEADER
{
    u16 field_8_destination_counter_idx;
    u16 field_A_first_operand_counter_idx;
    u16 field_C_second_operand_counter_idx;
};

struct SCR_PUT_CAR_ON_TRAILER : SCR_CMD_HEADER
{
    u16 field_8_car_idx;
    u16 field_A_trailer_car_idx;
};

struct SCR_CREATE_LIGHT : SCR_CMD_HEADER
{
    u16 field_8_light_idx;
    u16 field_A_unknown;
    Fix16 field_C_xpos;
    Fix16 field_10_ypos;
    Fix16 field_14_zpos;
    s32 field_18_argb;
    Fix16 field_1C_radius;
    u8 field_20_intensity;
    u8 field_21_on_time;
    u8 field_22_off_time;
    u8 field_23_shape;
};

struct SCR_ANSWER_PHONE : SCR_CMD_HEADER
{
    u16 field_8_obj_idx;
    u16 field_A;
    u16 field_C_ped_idx;
    u16 field_E;
    s16 field_10;
    u8 field_12;
};

struct SCR_ADD_PATROL_POINT : SCR_CMD_HEADER
{
    u16 field_8_varname;
    u16 field_A_unk3;
    SCR_XYZ_f field_C_pos;
};

struct SCR_CHAR_ENTER_CAR : SCR_CMD_HEADER
{
    u16 field_8_ped_idx;
    u16 field_A_car_idx;
    SCR_XYZ_f field_C_pos;
};

struct SCR_GET_SCORE : SCR_CMD_HEADER
{
    u16 field_8_ped_idx;
    u16 field_A_counter_idx;
};

struct SCR_IS_CHAR_IN_GANG_ZONE : SCR_CMD_HEADER
{
    u16 field_8_ped_idx;
    u16 field_A_gang_idx;
};

struct SCR_CHANGE_CAR_LOCK : SCR_CMD_HEADER
{
    u16 field_8_car_idx;
    s16 field_A_lock_type;
};

struct SCR_IS_POINT_ON_SCREEN : SCR_CMD_HEADER
{
	Fix16 field_8_xpos;
	Fix16 field_C_ypos;
};

struct SCR_GET_NUM_LIVES_MULTIPLIERS : SCR_CMD_HEADER
{
	u16 field_8_player_ped_idx;
	u16 field_A_counter_idx;
};

struct SCR_REMOTE_CONTROL : SCR_CMD_HEADER
{
	u16 field_8_player_ped_idx;
	u16 field_A_car_idx;
};

struct SCR_SWITCH_ROAD_ON_OFF : SCR_CMD_HEADER
{
    u8 field_8_xpos;
    u8 field_9_ypos;
    u8 field_A_zpos;
    u8 field_B_status;
};

struct SCR_CHANGE_LIGHT_INTENSITY : SCR_CMD_HEADER
{
    u16 field_8_light_idx;
    u16 field_A_intensity;
};

namespace SCR_DOOR_OPENTYPES
{
enum
{
    any_player = 0,
    any_car = 1,
    one_car = 2,
    one_model = 3,
    one_char_on_foot = 4,
    unknown1 = 5,
    any_player_one_car = 6,
};
//static_assert(sizeof(SCR_DOOR_OPENTYPES) == 1)
} // namespace SCR_DOOR_OPENTYPES

namespace SCR_DOOR_CLOSETYPES
{
enum
{
    close_never = 0,
    close_time_delay = 1,
    close_when_clear = 2,
    close_when_open_rule_fails = 3,
};
//static_assert(sizeof(SCR_DOOR_CLOSETYPES) == 1)
} // namespace SCR_DOOR_CLOSETYPES

namespace SCR_BONUSES
{
enum
{
    NO_BONUS = 0,
    BONUS_1 = 1,
    BONUS_2 = 2,
    BONUS_3 = 3,
};
} // namespace SCR_BONUSES

EXTERN_GLOBAL(Fix16, dword_6F77C0);
EXTERN_GLOBAL(Fix16, dword_6F77C4);

class miss2_0x11C
{
  public:
    EXPORT void sub_503200();
    EXPORT char_type sub_503410(u32 a1);
    EXPORT void sub_5035B0();
    EXPORT void sub_5035D0();
    EXPORT void sub_503610();
    EXPORT void Next_503620(SCR_CMD_HEADER* a2);
    EXPORT void JumpToCmd_503650(u16 a2);
    EXPORT void EndCmd_503670();
    EXPORT void SCRCMD_OBJ_DECSET_2D_3D_503680(SCR_OBJ_DATA* a1, SCR_POINTER* a2);
    EXPORT void SCRCMD_OBJ_DECSET_5038D0(SCR_OBJ_DATA* a1, SCR_POINTER* a2);
    EXPORT void SCRCMD_PLAYER_PED_503A20(SCR_PLAYER_PED* pCmd);
    EXPORT void SCRCMD_CAR_DECSET_503BC0(SCR_CAR_DATA_DEC* a1, SCR_POINTER* a2);
    EXPORT void SCRCMD_PARKED_CAR_DECSET_503F80(SCR_POINTER* a1);
    EXPORT void SCRCMD_CHAR_DECSET_2D_3D_503FB0(SCR_CHAR_DATA_DEC* a1, SCR_POINTER* a2);
    EXPORT Ped* sub_504110(SCR_CHAR_OBJECTIVE* a1, SCR_POINTER* a2);
    EXPORT void SCRCMD_MAP_ZONE_SET_504150(SCR_MAP_ZONE_SET* a1);
    EXPORT void SCRCMD_ARROW_DEC_5041B0(SCR_TWO_PARAMS* a1);
    EXPORT s32 SCRCMD_CRANE_5041C0(s32 a1, s32 a2);
    EXPORT void SCRCMD_CONVEYOR_DECSET1_2_5043A0(SCR_CONVEYOR* a1, SCR_POINTER* a2);
    EXPORT void SCRCMD_GENERATOR_DECSET_504420(SCR_GENERATOR* a1, SCR_POINTER* a2);
    EXPORT void SCRCMD_DESTRUCTOR_DECSET_504530(SCR_DESTRUCTOR* a1, SCR_POINTER* a2);
    EXPORT void SCRCMD_CRUSHER_BASIC_5045A0(SCR_CRUSHER_BASIC* a1, SCR_POINTER* a2);
    EXPORT s32 SCRCMD_THREAD_DECLARE2_5045D0(s32 a1, s16* a2);
    EXPORT void SCRCMD_THREAD_DECLARE3_504660(s32 a2);
    EXPORT void sub_504710(s32 a1);
    EXPORT s32* SCRCMD_THREAD_DECLARE4_5047C0(s32 a1, s16* a2);
    EXPORT void SCRCMD_SET_GANG_INFO1_504830(SCR_SET_GANG_INFO* a1);
    EXPORT void SCRCMD_SET_DOOR_INFO_504950(SCR_FOUR_PARAMS* a1);
    EXPORT void SCRCMD_DOOR_DECLARE_D1_S1_504970(SCR_DOOR_DATA_DEC* a1);
    EXPORT void SCRCMD_DOOR_DECLARE_D2_S2_504B80(SCR_DOOR_DATA_DEC* a3);
    EXPORT void SCRCMD_DECLARE_MISSION_504DD0(SCR_TWO_PARAMS* a1);
    EXPORT void CreateLight_504EE0(SCR_CREATE_LIGHT* pCmd, SCR_POINTER* pPointer);
    EXPORT void SCRCMD_SET_STATION_EMPTY_STATION_505030(SCR_SET_STATION* a1);
    EXPORT void SCRCMD_RADIOSTATION_DEC_5051D0(SCR_RADIOSTATION_DEC* a1);
    EXPORT void CRCMD_SET_TRAIN_STATIONS_505210(SCR_TWO_PARAMS* a1);
    EXPORT void SCRCMD_DECLARE_POLICE_5052C0(SCR_DECLARE_POLICELEVEL* a1);
    EXPORT void SCRCMD_SOUND_DECSET_505340(SCR_SOUND_DECSET* a1, SCR_POINTER* a2);
    EXPORT void SCRCMD_SET_MISSIONS_TOTAL_505430(SCR_TWO_PARAMS* a1);
    EXPORT void SCRCMD_PASSED_FAILED_FLAGS_505580(SCR_TWO_PARAMS* a1);
    EXPORT void SCRCMD_FINISH_SCORE_505710(SCR_TWO_PARAMS* a1);
    EXPORT void SCRCMD_DECLARE_CARLIST_505750(SCR_TWO_PARAMS* a1);
    EXPORT s32 DisableThread_505790(u16 a1);
    EXPORT void DeallocOrDeleteItem_505B10(u16 idx);
    EXPORT s16 sub_505EA0(u16 idx);
    EXPORT void SCRCMD_DISPLAY_MESSAGES_505F50(SCR_TWO_PARAMS* a1);
    EXPORT void SCRCMD_DEC_DEATH_BASE_506010(SCR_FOUR_PARAMS* a1);
    EXPORT void SCRCMD_DO_CRANE_POWERUP_5060D0(SCR_DECLARE_CRANE_POWERUP* a1);
    EXPORT void SCRCMD_START_BASIC_KF_506140(u16* a1);
    EXPORT void ExecOpCode_5061C0();
    EXPORT void SCRCMD_LEVELSTART_5069C0();
    EXPORT void SCRCMD_LEVELEND_5069F0();
    EXPORT void SCRCMD_CREATE_THREAD_506A00();
    EXPORT void SCRCMD_STOP_THREAD_506A60();
    EXPORT void SCRCMD_IF_JUMP_506AF0();
    EXPORT void sub_506B30();
    EXPORT void sub_506B80();
    EXPORT u8 sub_506BC0(u32 a1);
    EXPORT void sub_506D60();
    EXPORT void sub_506ED0();
    EXPORT void sub_507110();
    EXPORT void SCRCMD_NOT_507750();
    EXPORT void SCRCMD_START_EXEC_5078D0();
    EXPORT void SCRCMD_STOP_EXEC_5079A0();
    EXPORT void SCRCMD_INCREMENT_507A70();
    EXPORT void SCRCMD_DECREMENT_507B50();
    EXPORT void sub_507CE0();
    EXPORT void sub_507F80();
    EXPORT void SCRCMD_MAKE_CAR_DUMMY_508220();
    EXPORT void sub_508280();
    EXPORT void sub_508550();
    EXPORT void sub_5086F0();
    EXPORT void SCRCMD_ARROW_COLOUR_508DC0();
    EXPORT void SCRCMD_REMOVE_ARROW_508E80();
    EXPORT void SCRCMD_CHECK_CAR_DAMAGE_508F00();
    EXPORT void SCRCMD_CHECK_HEALTH_509030();
    EXPORT void SCRCMD_STORE_CAR_INFO_509180();
    EXPORT void sub_5093C0();
    EXPORT void SCRCMD_DISPLAY_TIMER_5096E0();
    EXPORT void SCR_CMD_CREATE_CHAR_509730();
    EXPORT void sub_509790();
    EXPORT void sub_5097D0();
    EXPORT void sub_509810();
    EXPORT void sub_509880();
    EXPORT void sub_5098E0();
    EXPORT void sub_509990();
    EXPORT void SCRCMD_CAR_IN_AREA_509A70();
    EXPORT void SCRCMD_HAS_CHAR_DIED_509BB0();
    EXPORT void sub_509C10();
    EXPORT void sub_509C90();
    EXPORT void sub_509D00();
    EXPORT void sub_509D60();
    EXPORT void sub_509D90();
    EXPORT void SCRCMD_ADD_SCORE2_509E00();
    EXPORT void sub_509E70();
    EXPORT void sub_509ED0();
    EXPORT void SCRCMD_EXPLODE_BUILDING_509F60();
    EXPORT void sub_509FD0();
    EXPORT void sub_50A200();
    EXPORT void sub_50A3E0();
    EXPORT void sub_50A460();
    EXPORT void SCRCMD_CHANGE_CAR_REMAP_50A570();
    EXPORT void SCRCMD_CHANGE_CHAR_REMAP_50A5B0();
    EXPORT void SCRCMD_CHECK_CAR_MODEL_50A610();
    EXPORT void SCRCMD_CHECK_CAR_REMAP_50A670();
    EXPORT void SCRCMD_CHECK_CAR_BOTH_50A6E0();
    EXPORT void sub_50A760();
    EXPORT void SCRCMD_DELAY_HERE_50A940();
    EXPORT void sub_50A980();
    EXPORT void EnableThread_50A9E0(u16 idx);
    EXPORT void SCRCMD_DISABLE_THREAD_50ABC0();
    EXPORT void SCRCMD_ENABLE_THREAD_50ABF0();
    EXPORT void SCRCMD_SET_GANG_KILL_REACTION_50AC20();
    EXPORT void sub_50ACF0();
    EXPORT void sub_50AEF0();
    EXPORT void sub_50B0E0();
    EXPORT void SCRCMD_SET_AMBIENT_50B150();
    EXPORT void sub_50B180();
    EXPORT void SCRCMD_CHECK_PHONE_50B230();
    EXPORT void SCRCMD_CHECK_PHONETIMER_50B2C0();
    EXPORT void SCRCMD_STOP_PHONE_RING_50B360();
    EXPORT void sub_50B3D0();
    EXPORT void SCRCMD_CAR_DRIVE_AWAY_50B440();
    EXPORT void SCRCMD_GET_CAR_FROM_CRANE_50B470();
    EXPORT void sub_50B4F0();
    EXPORT void sub_50B5A0();
    EXPORT void sub_50B600();
    EXPORT void SCRCMD_SET_GANG_INFO1_50B670();
    EXPORT void sub_50B690();
    EXPORT void SCRCMD_CHECK_SCORE_50B6F0();
    EXPORT void SCRCMD_GET_SCORE_50B760();
    EXPORT void SCRCMD_IS_CHAR_IN_GANG_50B7D0();
    EXPORT void sub_50B8B0();
    EXPORT void SCRCMD_IS_CHAR_FIRING_AREA_50B910();
    EXPORT void SCRCMD_GET_PASSENGER_NUM_50B9C0();
    EXPORT void sub_50BA30();
    EXPORT void sub_50BA70();
    EXPORT void SCRCMD_CAR_WRECK_IN_LOCATION_50BAD0();
    EXPORT void sub_50BB80();
    EXPORT void sub_50BBD0();
    EXPORT void SCRCMD_CHECK_NUM_ALIVE_50BC60();
    EXPORT void SCRCMD_SET_MIN_ALIVE_50BCD0();
    EXPORT void sub_50BD10();
    EXPORT void sub_50BDC0();
    EXPORT void SCRCMD_HAS_CAR_WEAPON_50BE00();
    EXPORT void SCRCMD_IS_CHAR_HORN_50BE70();
    EXPORT void SCRCMD_CHECK_MAX_PASS_50BED0();
    EXPORT void SCRCMD_IS_CHAR_IN_ZONE_50BF40();
    EXPORT void SCRCMD_SET_PHONE_DEAD_50C040();
    EXPORT void SCRCMD_IS_TRAILER_ATT_50C0E0();
    EXPORT void SCRCMD_IS_CAR_ON_TRAIL_50C1B0();
    EXPORT void SCRCMD_ENABLE_DISABLE_CRANE_50C230();
    EXPORT void SCRCMD_CAR_GOT_DRIVER_50C2A0();
    EXPORT void SCRCMD_SPOTTED_PLAYER_50C2F0();
    EXPORT void SCRCMD_GET_LAST_PUNCHED_50C350();
    EXPORT void SCRCMD_IS_CHAR_STUNNED_50C3B0();
    EXPORT void SCRCMD_KILL_ALL_PASSENG_50C410();
    EXPORT void SCRCMD_IS_GROUP_IN_CAR_50C470();
    EXPORT void SCRCMD_PUNCHED_SOMEONE_50C4E0();
    EXPORT void SCRCMD_ADD_CHAR_TO_GANG_50C540();
    EXPORT void sub_50C5A0();
    EXPORT void sub_50C6F0();
    EXPORT void SCRCMD_BEEN_PUNCHED_BY_50C760();
    EXPORT void SCRCMD_UPDATE_DOOR_50C7D0();
    EXPORT void SCRCMD_DOOR_50C8A0();
    EXPORT void SCRCMD_REMOVE_WEAPON_50C990();
    EXPORT void SCRCMD_REMOVE_BLOCK_50C9F0();
    EXPORT void SCRCMD_LOWER_LEVEL_50CA30();
    EXPORT void sub_50CA70();
    EXPORT void SCRCMD_ADD_NEW_BLOCK_50CAB0();
    EXPORT void SCRCMD_ROAD_ON_OFF_50CB20();
    EXPORT void sub_50CB70();
    EXPORT void sub_50CCB0();
    EXPORT void sub_50CD30();
    EXPORT void SCRCMD_CAR_DAMAGE_POS_50CDB0();
    EXPORT void SCRCMD_PARK_FINISHED_50CE10();
    EXPORT void sub_50CE50(SCR_CMD_HEADER* pCmd, u16 cmd_idx);
    EXPORT void SCRCMD_PHONE_TEMPLATE_50CE90();
    EXPORT void SCRCMD_REMOTE_CONTROL_50D200();
    EXPORT void SCRCMD_LAUNCH_MISSION_50D2E0();
    EXPORT void SCRCMD_SAVE_GAME_50D340();
    EXPORT void sub_50D3C0();
    EXPORT void SCRCMD_CHANGE_CAR_LOCK_50D680();
    EXPORT void SCRCMD_CHANGE_INTENSITY_50D870();
    EXPORT void SCRCMD_CHANGE_COLOUR_50D900();
    EXPORT void SCRCMD_CHANGE_RADIUS_50D9A0();
    EXPORT void sub_50DA50();
    EXPORT void sub_50DB70();
    EXPORT void sub_50DD00();
    EXPORT void SCRCMD_SET_DIR_OF_TVVAN_50DD90();
    EXPORT void SCRCMD_POINT_ONSCREEN_50DE00();
    EXPORT void SCRCMD_CHAR_IN_AIR_50DE50();
    EXPORT void SCRCMD_CHAR_SUNK_50DEB0();
    EXPORT void sub_50DF10();
    EXPORT void sub_50E0B0();
    EXPORT void sub_50E120();
    EXPORT void sub_50E150();
    EXPORT void sub_50E190();
    EXPORT void SCRCMD_CHECK_CAR_SPEED_50E360();
    EXPORT void sub_50E460();
    EXPORT void SCRCMD_CHAR_ARRESTED_50E4A0();
    EXPORT void sub_50E4F0();
    EXPORT void sub_50E610();
    EXPORT void sub_50E730();
    EXPORT void sub_50E780();
    EXPORT void sub_50E7F0();
    EXPORT void sub_50E820();
    EXPORT void sub_50E900();
    EXPORT void sub_50E9A0();
    EXPORT void sub_50E9E0();
    EXPORT void sub_50EA40();
    EXPORT void sub_50EB00();
    EXPORT void sub_50EBD0();
    EXPORT void sub_50ECE0();
    EXPORT void sub_50ED40();
    EXPORT void sub_50ED80();
    EXPORT void sub_50EDC0();
    EXPORT void SCRCMD_CHAR_INTO_CAR_50F060();
    EXPORT void SCRCMD_DECIDE_POWERUP_50F150();
    EXPORT void SCRCMD_SUPPRESS_MODEL_50F220();
    EXPORT void sub_50F270();
    EXPORT void SCRCMD_SET_GROUP_TYPE_50F3D0();
    EXPORT void SCRCMD_CHAR_DO_NOTHING_50F410();
    EXPORT void SCRCMD_EMERG_LIGHTS_50F450();
    EXPORT void SCRCMD_CHECK_OBJ_MODEL_50F4D0();
    EXPORT void SCRCMD_PED_GRAPHIC_50F550();
    EXPORT void sub_50F5E0();
    EXPORT void SCRCMD_MAKE_MUGGERS_50F770();
    EXPORT void sub_50F7B0();
    EXPORT void SCRCMD_STOP_CAR_DRIVE_50F900();
    EXPORT void SCRCMD_IS_BUS_FULL_50F940();
    EXPORT void SCRCMD_NO_CHARS_OFF_BUS_50F9B0();
    EXPORT void sub_50FA00();
    EXPORT void SCRCMD_SET_SHADING_LEV_50FA40();
    EXPORT void SCRCMD_SET_CAR_JAMMED_50FA70();
    EXPORT void SCRCMD_FINISH_MISSION_50FAD0();
    EXPORT void sub_50FAF0();
    EXPORT void sub_50FB60();
    EXPORT void sub_50FC20();
    EXPORT void sub_50FC60();
    EXPORT void sub_50FE00();
    EXPORT void sub_50FED0();
    EXPORT void sub_50FF50();
    EXPORT void sub_50FFB0();
    EXPORT void sub_510030();
    EXPORT void sub_510050();
    EXPORT void sub_510090();
    EXPORT void sub_510100();
    EXPORT void sub_510280();
    EXPORT void sub_510530();
    EXPORT void sub_510560();
    EXPORT void sub_5105B0();
    EXPORT void sub_510600();
    EXPORT void sub_510660();
    EXPORT void sub_510780();
    EXPORT void PreExecOpCode_5108D0();
    EXPORT char_type PoolUpdate();
    EXPORT void sub_511930(char_type a2, u16 levelStart);
    EXPORT miss2_0x11C* sub_511960(u16 a2);
    EXPORT void launch_mission_5119A0(SCR_CMD_HEADER* base_pointer, char_type* string);
    EXPORT miss2_0x11C();
    EXPORT void sub_511CD0();
    EXPORT ~miss2_0x11C();

    miss2_0x11C* mpNext;
    u16 field_4_next_cmd;
    char_type field_6;
    char_type field_7;
    s32 field_8;
    char_type field_C;
    char_type field_D;
    s16 field_E;
    char_type field_10;
    char_type field_11;
    s16 field_12;
    char_type field_14_str[255];
    char_type field_113;
    miss2_8* field_114;
    char_type field_118;
    char_type field_119;
    s16 field_11A;
};
GTA2_ASSERT_SIZEOF_ALWAYS(miss2_0x11C, 0x11C)

class miss2_0x11C_Pool
{
  public:
  // TODO: Pools Use pool method
    // inlined
    miss2_0x11C* sub_4767A0()
    {
        miss2_0x11C* pf_0 = field_0_pool.field_0_pStart;
        field_0_pool.field_0_pStart = pf_0->mpNext;
        pf_0->mpNext = field_0_pool.field_4_pPrev;
        field_0_pool.field_4_pPrev = pf_0;
        pf_0->sub_5035B0();
        return pf_0; // ??
    }

    // 9.6f inlined ctor 0x481310
    miss2_0x11C_Pool()
    {

    }

    void sub_47F4D0()
    {
        for (miss2_0x11C* pThread = field_0_pool.field_4_pPrev; pThread; pThread = pThread->mpNext)
        {
            pThread->sub_5035D0();
        }
    }

    EXPORT ~miss2_0x11C_Pool();

    Pool<miss2_0x11C, 8> field_0_pool;
};

EXTERN_GLOBAL(miss2_0x11C_Pool*, miss2_0x11C_Pool_6F8064);