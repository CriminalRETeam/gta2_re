#pragma once

#include "Function.hpp"
#include "fix16.hpp"

class cool_nash_0x294;
class Maccies_2C;
class Snooky_30;
class miss2_8;
class Car_BC;
class Object_2C;

struct SCR_CMD_HEADER
{
    s16 field_0_cmd_this;
    u16 field_2_type;
    s16 field_4_cmd_next;
    s16 field_6_return_value;
};

struct SCR_XYZ_f
{
    Fix16 field_0_x;
    Fix16 field_4_y;
    Fix16 field_8_z;
};

struct SCR_PLAYER_PED
{
    SCR_CMD_HEADER field_0_header;
    cool_nash_0x294* field_8_ped;
    SCR_XYZ_f field_C_pos;
    s16 field_18_rot;
    s16 field_1A_remap;
};

struct SCR_CAR_DATA_DEC : SCR_CMD_HEADER
{
    Car_BC* field_8_car;
    SCR_XYZ_f field_C_pos;
    u16 field_18_rot;
    u16 field_1A_remap;
    s16 field_1C_car_id;
    u16 field_1E_trailer_id;
};

struct SCR_CHAR_DATA_DEC : SCR_CMD_HEADER
{
    cool_nash_0x294* field_8_char;
    SCR_XYZ_f field_C_pos;
    u16 field_18_rot;
    s16 field_1A_remap;
    s16 field_1C_occupation;
    u16 field_1E_padding;
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

struct SCR_POINTER : SCR_CMD_HEADER
{
    union
    {
        cool_nash_0x294* field_8_char;
        Car_BC* field_8_car;
        Object_2C* field_8_obj;
    };
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

struct SCR_SOUND_DECSET : SCR_CMD_HEADER
{
    u16 field_8_varname;
    u16 field_A_unk;
    SCR_XYZ_f field_C_pos;
    u8 field_18_sound_id;
    u8 field_19_play_type;
    u16 field_1B_padding;
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

struct SCR_IF_JUMP : SCR_CMD_HEADER
{
    u8 is_or;
    u8 unknown;
    u16 else_endif_pointer;
};

class miss2_0x11C
{
  public:
    EXPORT void sub_503200();
    EXPORT char_type sub_503410(u32 a1);
    EXPORT void sub_5035B0();
    EXPORT void sub_5035D0();
    EXPORT void Next_503620(SCR_CMD_HEADER* a2);
    EXPORT void sub_503650(u16 a2);
    EXPORT void sub_503670();
    EXPORT s32 SCRCMD_OBJ_DECSET_2D_3D_503680(s32* a1, s32 a2);
    EXPORT void SCRCMD_OBJ_DECSET_5038D0(s32* a1, s32 a2);
    EXPORT void SCRCMD_PLAYER_PED_503A20(SCR_PLAYER_PED* pCmd);
    EXPORT void SCRCMD_CAR_DECSET_503BC0(SCR_CAR_DATA_DEC* a1, SCR_POINTER* a2);
    EXPORT void SCRCMD_PARKED_CAR_DECSET_503F80(SCR_POINTER* a1);
    EXPORT void SCRCMD_CHAR_DECSET_2D_3D_503FB0(SCR_CHAR_DATA_DEC* a1, SCR_POINTER* a2);
    EXPORT cool_nash_0x294* sub_504110(SCR_CHAR_OBJECTIVE* a1, SCR_POINTER* a2);
    EXPORT void SCRCMD_MAP_ZONE_SET_504150(SCR_MAP_ZONE_SET* a1);
    EXPORT void SCRCMD_ARROW_DEC_5041B0(SCR_TWO_PARAMS* a1);
    EXPORT s32 SCRCMD_CRANE_5041C0(s32 a1, s32 a2);
    EXPORT char_type SCRCMD_CONVEYOR_DECSET1_2_5043A0(s32* a1, s32 a2);
    EXPORT Maccies_2C* SCRCMD_GENERATOR_DECSET_504420(s32* a1, s32 a2);
    EXPORT s32* SCRCMD_DESTRUCTOR_DECSET_504530(s32* a1, s32 a2);
    EXPORT Snooky_30* SCRCMD_CRUSHER_BASIC_5045A0(s32 a1, s32 a2);
    EXPORT s32 SCRCMD_THREAD_DECLARE2_5045D0(s32 a1, s16* a2);
    EXPORT void SCRCMD_THREAD_DECLARE3_504660(miss2_0x11C* a1, s32 a2);
    EXPORT void sub_504710(s32 a1);
    EXPORT s32* SCRCMD_THREAD_DECLARE4_5047C0(s32 a1, s16* a2);
    EXPORT void SCRCMD_SET_GANG_INFO1_504830(s32 a1);
    EXPORT s16 SCRCMD_SET_DOOR_INFO_504950(s32 a1);
    EXPORT s32 SCRCMD_DOOR_DECLARE_D1_S1_504970(s32);
    EXPORT void SCRCMD_DOOR_DECLARE_D2_S2_504B80(s32 a1, s32 a2, s32 a3);
    EXPORT void SCRCMD_DECLARE_MISSION_504DD0(u16* a1);
    EXPORT void sub_504EE0(s32 a1, s32 a2);
    EXPORT void SCRCMD_SET_STATION_EMPTY_STATION_505030(s32 a1);
    EXPORT void SCRCMD_RADIOSTATION_DEC_5051D0(s32 a1);
    EXPORT void CRCMD_SET_TRAIN_STATIONS_505210(s32 a1);
    EXPORT void SCRCMD_OBJ_DECSET_2D_STR_5052C0(s32 a1);
    EXPORT void SCRCMD_SOUND_DECSET_505340(SCR_SOUND_DECSET* a1, SCR_POINTER* a2);
    EXPORT void SCRCMD_SET_MISSIONS_TOTAL_505430(s32 a1);
    EXPORT void SCRCMD_PASSED_FAILED_FLAGS_505580(s32 a1);
    EXPORT void SCRCMD_FINISH_SCORE_505710(SCR_TWO_PARAMS* a1);
    EXPORT void SCRCMD_DECLARE_CARLIST_505750(s32 a1);
    EXPORT s32 sub_505790(u16 a1);
    EXPORT void sub_505B10(u16 idx);
    EXPORT u16 sub_505EA0(u16 idx);
    EXPORT void SCRCMD_DISPLAY_MESSAGES_505F50(s32 a1);
    EXPORT s16 SCRCMD_DEC_DEATH_BASE_506010(s32 a1);
    EXPORT SCR_CMD_HEADER* SCRCMD_DO_CRANE_POWERUP_5060D0(u16* a1);
    EXPORT void SCRCMD_START_BASIC_KF_506140(u16* a1);
    EXPORT void ExecOpCode_5061C0();
    EXPORT void SCRCMD_LEVELSTART_5069C0();
    EXPORT void SCRCMD_LEVELEND_5069F0();
    EXPORT void SCRCMD_CREATE_THREAD_506A00();
    EXPORT void SCRCMD_STOP_THREAD_506A60();
    EXPORT void SCRCMD_IF_JUMP_506AF0();
    EXPORT void sub_506B30();
    EXPORT void sub_506B80();
    EXPORT char_type sub_506BC0(u32 a1);
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
    EXPORT void sub_50A9E0(u16 idx);
    EXPORT void SCRCMD_DISABLE_THREAD_50ABC0();
    EXPORT void SCRCMD_ENABLE_THREAD_50ABF0();
    EXPORT void SCRCMD_SET_GANG_RESPECT_50AC20();
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
    EXPORT void sub_50CE50(s32 a2, u16 a3);
    EXPORT void SCRCMD_PHONE_TEMPLATE_50CE90();
    EXPORT void SCRCMD_REMOTE_CONTROL_50D200();
    EXPORT s32 SCRCMD_LAUNCH_MISSION_50D2E0();
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
    EXPORT s32 sub_50F270();
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
    EXPORT char_type sub_511840();
    EXPORT void sub_511930(char_type a2, u16 levelStart);
    EXPORT miss2_0x11C* sub_511960(s16 a2);
    EXPORT s32 launch_mission_5119A0(s32 a2, char_type* String1);
    EXPORT miss2_0x11C();
    EXPORT void sub_511CD0();
    EXPORT ~miss2_0x11C();

    miss2_0x11C* field_0;
    u16 field_4_level_start;
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

class Miss2_8EC
{
  public:
    // inlined
    miss2_0x11C* sub_4767A0()
    {
        miss2_0x11C* pf_0 = field_0;
        field_0 = pf_0->field_0;
        pf_0->field_0 = field_4;
        field_4 = pf_0;
        pf_0->sub_5035B0();
        return pf_0; // ??
    }

    miss2_0x11C* field_0;
    miss2_0x11C* field_4;
    miss2_0x11C field_8[8];
    s16 field_8E8;
    s16 field_8EA;
};
EXPORT_VAR extern Miss2_8EC* gMiss2_8EC_6F8064;
