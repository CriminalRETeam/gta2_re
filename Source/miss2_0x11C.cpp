#include "miss2_0x11c.hpp"
#include "Car_BC.hpp"
#include "Globals.hpp"
#include "cool_nash_0x294.hpp"
#include "error.hpp"
#include "frosty_pasteur_0xC1EA8.hpp"
#include "miss2_8.hpp"
#include "root_sound.hpp"

#if defined(EXPORT_VARS) || defined(IMPORT_VARS)
EXPORT_VAR s16 word_6212EE;
GLOBAL(word_6212EE, 0x6212EE);
#else
s16 word_6212EE = 1;
#endif

EXPORT_VAR Miss2_8EC* gMiss2_8EC_6F8064;
GLOBAL(gMiss2_8EC_6F8064, 0x6F8064);

EXPORT_VAR SCR_CMD_HEADER* gBasePtr_6F8070;
GLOBAL(gBasePtr_6F8070, 0x6F8070);

STUB_FUNC(0x503200)
void miss2_0x11C::sub_503200()
{
}

STUB_FUNC(0x503410)
char_type miss2_0x11C::sub_503410(u32 a1)
{
    return 0;
}

MATCH_FUNC(0x5035b0)
void miss2_0x11C::sub_5035B0()
{
    field_118 = 0;
    field_11A = word_6212EE++;
}

STUB_FUNC(0x5035d0)
void miss2_0x11C::sub_5035D0()
{
}

EXPORT_VAR s32 dword_6F806C;
GLOBAL(dword_6F806C, 0x6F806C);

MATCH_FUNC(0x503620)
void miss2_0x11C::Next_503620(SCR_CMD_HEADER* a2)
{
    if ((u16)a2->field_4_cmd_next != 0xFFFF)
    { // FF FF (low endian) is the script terminator
        dword_6F806C = this->field_4_level_start;
        this->field_4_level_start = a2->field_4_cmd_next;
        this->field_C = 0;
    }
    else
    {
        miss2_0x11C::sub_503670();
    }
}

MATCH_FUNC(0x503650)
void miss2_0x11C::sub_503650(u16 a2)
{
    if (a2 != 0xFFFF)
    {
        this->field_4_level_start = a2;
    }
    else
    {
        miss2_0x11C::sub_503670();
    }
}

STUB_FUNC(0x503670)
void miss2_0x11C::sub_503670()
{
}

STUB_FUNC(0x503680)
s32 miss2_0x11C::SCRCMD_OBJ_DECSET_2D_3D_503680(s32* a1, s32 a2)
{
    return 0;
}

STUB_FUNC(0x5038d0)
void miss2_0x11C::SCRCMD_OBJ_DECSET_5038D0(s32* a1, s32 a2)
{
}

STUB_FUNC(0x503a20)
void miss2_0x11C::SCRCMD_PLAYER_PED_503A20(SCR_PLAYER_PED* pCmd)
{
}

STUB_FUNC(0x503bc0)
void miss2_0x11C::SCRCMD_CAR_DECSET_503BC0(s32* a1, s32 a2)
{
}

MATCH_FUNC(0x503f80)
void miss2_0x11C::SCRCMD_PARKED_CAR_DECSET_503F80(s32 a1)
{
    s32 v1;
    miss2_0x11C::SCRCMD_CAR_DECSET_503BC0((s32*)a1, a1);
    (*(Car_BC**)(a1 + 8))->sub_443EB0(9);
    v1 = *(s32*)(a1 + 8);
    *(s32*)(v1 + 124) = 4;
    *(s16*)(v1 + 118) = 0;
}

STUB_FUNC(0x503fb0)
void miss2_0x11C::SCRCMD_CHAR_DECSET_2D_3D_503FB0(s32* a1, s32 a2)
{
}

MATCH_FUNC(0x504110)
s32 miss2_0x11C::sub_504110(s32 a1, s32 a2)
{
    s32 v2;
    s32 v3;

    (*(cool_nash_0x294**)(a2 + 8))->sub_463570(*(s16*)(a1 + 10), 9999);
    v2 = *(s32*)(a2 + 8);

    v3 = *(s32*)(v2 + 540);
    v3 &= ~0x400u;

    *(s32*)(v2 + 540) = v3;
    return v2;
}

STUB_FUNC(0x504150)
void miss2_0x11C::SCRCMD_MAP_ZONE_SET_504150(s16* a1)
{
}

STUB_FUNC(0x5041b0)
s32 miss2_0x11C::SCRCMD_ARROW_DEC_5041B0(s32 a1)
{
    return 0;
}

STUB_FUNC(0x5041c0)
s32 miss2_0x11C::SCRCMD_CRANE_5041C0(s32 a1, s32 a2)
{
    return 0;
}

STUB_FUNC(0x5043a0)
char_type miss2_0x11C::SCRCMD_CONVEYOR_DECSET1_2_5043A0(s32* a1, s32 a2)
{
    return 0;
}

STUB_FUNC(0x504420)
Maccies_2C* miss2_0x11C::SCRCMD_GENERATOR_DECSET_504420(s32* a1, s32 a2)
{
    return 0;
}

STUB_FUNC(0x504530)
s32* miss2_0x11C::SCRCMD_DESTRUCTOR_DECSET_504530(s32* a1, s32 a2)
{
    return 0;
}

STUB_FUNC(0x5045a0)
Snooky_30* miss2_0x11C::SCRCMD_CRUSHER_BASIC_5045A0(s32 a1, s32 a2)
{
    return 0;
}

STUB_FUNC(0x5045d0)
s32 miss2_0x11C::SCRCMD_THREAD_DECLARE2_5045D0(s32 a1, s16* a2)
{
    return 0;
}

STUB_FUNC(0x504660)
void miss2_0x11C::SCRCMD_THREAD_DECLARE3_504660(miss2_0x11C* a1, s32 a2)
{
}

STUB_FUNC(0x504710)
void miss2_0x11C::sub_504710(s32 a1)
{
}

STUB_FUNC(0x5047c0)
s32* miss2_0x11C::SCRCMD_THREAD_DECLARE4_5047C0(s32 a1, s16* a2)
{
    return 0;
}

STUB_FUNC(0x504830)
void miss2_0x11C::SCRCMD_SET_GANG_INFO1_504830(s32 a1)
{
}

STUB_FUNC(0x504950)
s16 miss2_0x11C::SCRCMD_SET_DOOR_INFO_504950(s32 a1)
{
    return 0;
}

STUB_FUNC(0x504970)
s32 miss2_0x11C::SCRCMD_DOOR_DECLARE_D1_S1_504970(s32)
{
    return 0;
}

STUB_FUNC(0x504b80)
void miss2_0x11C::SCRCMD_DOOR_DECLARE_D2_S2_504B80(s32 a1, s32 a2, s32 a3)
{
}

STUB_FUNC(0x504dd0)
void miss2_0x11C::SCRCMD_DECLARE_MISSION_504DD0(u16* a1)
{
}

STUB_FUNC(0x504ee0)
void miss2_0x11C::sub_504EE0(s32 a1, s32 a2)
{
}

STUB_FUNC(0x505030)
void miss2_0x11C::SCRCMD_SET_STATION_EMPTY_STATION_505030(s32 a1)
{
}

STUB_FUNC(0x5051d0)
void miss2_0x11C::SCRCMD_RADIOSTATION_DEC_5051D0(s32 a1)
{
}

STUB_FUNC(0x505210)
void miss2_0x11C::CRCMD_SET_TRAIN_STATIONS_505210(s32 a1)
{
}

STUB_FUNC(0x5052c0)
void miss2_0x11C::SCRCMD_OBJ_DECSET_2D_STR_5052C0(s32 a1)
{
}

STUB_FUNC(0x505340)
void miss2_0x11C::SCRCMD_SOUND_DECSET_505340(s32 a1, s32 a2)
{
}

STUB_FUNC(0x505430)
void miss2_0x11C::SCRCMD_SET_MISSIONS_TOTAL_505430(s32 a1)
{
}

STUB_FUNC(0x505580)
void miss2_0x11C::SCRCMD_PASSED_FAILED_FLAGS_505580(s32 a1)
{
}

STUB_FUNC(0x505710)
void miss2_0x11C::SCRCMD_FINISH_SCORE_505710(s32 a1)
{
}

STUB_FUNC(0x505750)
void miss2_0x11C::SCRCMD_DECLARE_CARLIST_505750(s32 a1)
{
}

STUB_FUNC(0x505790)
s32 miss2_0x11C::sub_505790(u16 a1)
{
    return 0;
}

STUB_FUNC(0x505b10)
void miss2_0x11C::sub_505B10(u16 idx)
{
}

STUB_FUNC(0x505ea0)
u16 miss2_0x11C::sub_505EA0(u16 idx)
{
    return 0;
}

STUB_FUNC(0x505f50)
void miss2_0x11C::SCRCMD_DISPLAY_MESSAGES_505F50(s32 a1)
{
}

STUB_FUNC(0x506010)
s16 miss2_0x11C::SCRCMD_DEC_DEATH_BASE_506010(s32 a1)
{
    return 0;
}

STUB_FUNC(0x5060d0)
SCR_CMD_HEADER* miss2_0x11C::SCRCMD_DO_CRANE_POWERUP_5060D0(u16* a1)
{
    return 0;
}

STUB_FUNC(0x506140)
void miss2_0x11C::SCRCMD_START_BASIC_KF_506140(u16* a1)
{
}

STUB_FUNC(0x5061c0)
void miss2_0x11C::ExecOpCode_5061C0()
{
}

MATCH_FUNC(0x5069c0)
void miss2_0x11C::SCRCMD_LEVELSTART_5069C0()
{
    gRoot_sound_66B038.sub_40F090(24); //  "And remember, respect is everything!"
    this->field_118 = 1;
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

STUB_FUNC(0x5069f0)
void miss2_0x11C::SCRCMD_LEVELEND_5069F0()
{
}

STUB_FUNC(0x506a00)
void miss2_0x11C::SCRCMD_CREATE_THREAD_506A00()
{
}

STUB_FUNC(0x506a60)
void miss2_0x11C::SCRCMD_STOP_THREAD_506A60()
{
}

STUB_FUNC(0x506af0)
void miss2_0x11C::SCRCMD_IF_JUMP_506AF0()
{
}

STUB_FUNC(0x506b30)
void miss2_0x11C::sub_506B30()
{
}

STUB_FUNC(0x506b80)
void miss2_0x11C::sub_506B80()
{
}

STUB_FUNC(0x506bc0)
char_type miss2_0x11C::sub_506BC0(u32 a1)
{
    return 0;
}

STUB_FUNC(0x506d60)
void miss2_0x11C::sub_506D60()
{
}

STUB_FUNC(0x506ed0)
void miss2_0x11C::sub_506ED0()
{
}

STUB_FUNC(0x507110)
void miss2_0x11C::sub_507110()
{
}

MATCH_FUNC(0x507750)
void miss2_0x11C::SCRCMD_NOT_507750()
{
    this->field_8 = this->field_8 == 0; //  just toggle the boolean value
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x5078d0)
void miss2_0x11C::SCRCMD_START_EXEC_5078D0()
{
    s16 exec_flag;

    exec_flag = this->field_12;

    //  Only increase EXEC flag if it wasn't set before
    if (exec_flag <= 0)
    {
        this->field_12 = exec_flag + 1; //  increase EXEC flag
    }

    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x5079a0)
void miss2_0x11C::SCRCMD_STOP_EXEC_5079A0()
{
    s16 exec_flag;

    exec_flag = this->field_12;

    //  Only decrease EXEC flag if it was set before
    if (exec_flag >= 0)
    {
        this->field_12 = exec_flag - 1; //  decrease EXEC flag
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

STUB_FUNC(0x507a70)
void miss2_0x11C::SCRCMD_INCREMENT_507A70()
{
}

STUB_FUNC(0x507b50)
void miss2_0x11C::SCRCMD_DECREMENT_507B50()
{
}

STUB_FUNC(0x507ce0)
void miss2_0x11C::sub_507CE0()
{
}

STUB_FUNC(0x507f80)
void miss2_0x11C::sub_507F80()
{
}

STUB_FUNC(0x508220)
void miss2_0x11C::SCRCMD_MAKE_CAR_DUMMY_508220()
{
}

STUB_FUNC(0x508280)
void miss2_0x11C::sub_508280()
{
}

STUB_FUNC(0x508550)
void miss2_0x11C::sub_508550()
{
}

STUB_FUNC(0x5086f0)
void miss2_0x11C::sub_5086F0()
{
}

STUB_FUNC(0x508dc0)
void miss2_0x11C::SCRCMD_ARROW_COLOUR_508DC0()
{
}

STUB_FUNC(0x508e80)
void miss2_0x11C::SCRCMD_REMOVE_ARROW_508E80()
{
}

STUB_FUNC(0x508f00)
void miss2_0x11C::SCRCMD_CHECK_CAR_DAMAGE_508F00()
{
}

STUB_FUNC(0x509030)
void miss2_0x11C::SCRCMD_CHECK_HEALTH_509030()
{
}

STUB_FUNC(0x509180)
void miss2_0x11C::SCRCMD_STORE_CAR_INFO_509180()
{
}

STUB_FUNC(0x5093c0)
void miss2_0x11C::sub_5093C0()
{
}

STUB_FUNC(0x5096e0)
void miss2_0x11C::SCRCMD_DISPLAY_TIMER_5096E0()
{
}

STUB_FUNC(0x509730)
void miss2_0x11C::SCR_CMD_CREATE_CHAR_509730()
{
}

STUB_FUNC(0x509790)
void miss2_0x11C::sub_509790()
{
}

STUB_FUNC(0x5097d0)
void miss2_0x11C::sub_5097D0()
{
}

STUB_FUNC(0x509810)
void miss2_0x11C::sub_509810()
{
}

STUB_FUNC(0x509880)
void miss2_0x11C::sub_509880()
{
}

STUB_FUNC(0x5098e0)
void miss2_0x11C::sub_5098E0()
{
}

STUB_FUNC(0x509990)
void miss2_0x11C::sub_509990()
{
}

STUB_FUNC(0x509a70)
void miss2_0x11C::SCRCMD_CAR_IN_AREA_509A70()
{
}

STUB_FUNC(0x509bb0)
void miss2_0x11C::SCRCMD_HAS_CHAR_DIED_509BB0()
{
}

STUB_FUNC(0x509c10)
void miss2_0x11C::sub_509C10()
{
}

STUB_FUNC(0x509c90)
void miss2_0x11C::sub_509C90()
{
}

STUB_FUNC(0x509d00)
void miss2_0x11C::sub_509D00()
{
}

STUB_FUNC(0x509d60)
void miss2_0x11C::sub_509D60()
{
}

STUB_FUNC(0x509d90)
void miss2_0x11C::sub_509D90()
{
}

STUB_FUNC(0x509e00)
void miss2_0x11C::SCRCMD_ADD_SCORE2_509E00()
{
}

STUB_FUNC(0x509e70)
void miss2_0x11C::sub_509E70()
{
}

STUB_FUNC(0x509ed0)
void miss2_0x11C::sub_509ED0()
{
}

STUB_FUNC(0x509f60)
void miss2_0x11C::SCRCMD_EXPLODE_BUILDING_509F60()
{
}

STUB_FUNC(0x509fd0)
void miss2_0x11C::sub_509FD0()
{
}

STUB_FUNC(0x50a200)
void miss2_0x11C::sub_50A200()
{
}

STUB_FUNC(0x50a3e0)
void miss2_0x11C::sub_50A3E0()
{
}

STUB_FUNC(0x50a460)
void miss2_0x11C::sub_50A460()
{
}

STUB_FUNC(0x50a570)
void miss2_0x11C::SCRCMD_CHANGE_CAR_REMAP_50A570()
{
}

STUB_FUNC(0x50a5b0)
void miss2_0x11C::SCRCMD_CHANGE_CHAR_REMAP_50A5B0()
{
}

STUB_FUNC(0x50a610)
void miss2_0x11C::SCRCMD_CHECK_CAR_MODEL_50A610()
{
}

STUB_FUNC(0x50a670)
void miss2_0x11C::SCRCMD_CHECK_CAR_REMAP_50A670()
{
}

STUB_FUNC(0x50a6e0)
void miss2_0x11C::SCRCMD_CHECK_CAR_BOTH_50A6E0()
{
}

STUB_FUNC(0x50a760)
void miss2_0x11C::sub_50A760()
{
}

STUB_FUNC(0x50a940)
void miss2_0x11C::SCRCMD_DELAY_HERE_50A940()
{
}

STUB_FUNC(0x50a980)
void miss2_0x11C::sub_50A980()
{
}

STUB_FUNC(0x50a9e0)
void miss2_0x11C::sub_50A9E0(u16 idx)
{
}

STUB_FUNC(0x50abc0)
void miss2_0x11C::SCRCMD_DISABLE_THREAD_50ABC0()
{
}

STUB_FUNC(0x50abf0)
void miss2_0x11C::SCRCMD_ENABLE_THREAD_50ABF0()
{
}

STUB_FUNC(0x50ac20)
void miss2_0x11C::SCRCMD_SET_GANG_RESPECT_50AC20()
{
}

STUB_FUNC(0x50acf0)
void miss2_0x11C::sub_50ACF0()
{
}

STUB_FUNC(0x50aef0)
void miss2_0x11C::sub_50AEF0()
{
}

STUB_FUNC(0x50b0e0)
void miss2_0x11C::sub_50B0E0()
{
}

STUB_FUNC(0x50b150)
void miss2_0x11C::SCRCMD_SET_AMBIENT_50B150()
{
}

STUB_FUNC(0x50b180)
void miss2_0x11C::sub_50B180()
{
}

STUB_FUNC(0x50b230)
void miss2_0x11C::SCRCMD_CHECK_PHONE_50B230()
{
}

STUB_FUNC(0x50b2c0)
void miss2_0x11C::SCRCMD_CHECK_PHONETIMER_50B2C0()
{
}

STUB_FUNC(0x50b360)
void miss2_0x11C::SCRCMD_STOP_PHONE_RING_50B360()
{
}

STUB_FUNC(0x50b3d0)
void miss2_0x11C::sub_50B3D0()
{
}

STUB_FUNC(0x50b440)
void miss2_0x11C::SCRCMD_CAR_DRIVE_AWAY_50B440()
{
}

STUB_FUNC(0x50b470)
void miss2_0x11C::SCRCMD_GET_CAR_FROM_CRANE_50B470()
{
}

STUB_FUNC(0x50b4f0)
void miss2_0x11C::sub_50B4F0()
{
}

STUB_FUNC(0x50b5a0)
void miss2_0x11C::sub_50B5A0()
{
}

STUB_FUNC(0x50b600)
void miss2_0x11C::sub_50B600()
{
}

STUB_FUNC(0x50b670)
void miss2_0x11C::SCRCMD_SET_GANG_INFO1_50B670()
{
}

STUB_FUNC(0x50b690)
void miss2_0x11C::sub_50B690()
{
}

STUB_FUNC(0x50b6f0)
void miss2_0x11C::SCRCMD_CHECK_SCORE_50B6F0()
{
}

STUB_FUNC(0x50b760)
void miss2_0x11C::SCRCMD_GET_SCORE_50B760()
{
}

STUB_FUNC(0x50b7d0)
void miss2_0x11C::SCRCMD_IS_CHAR_IN_GANG_50B7D0()
{
}

STUB_FUNC(0x50b8b0)
void miss2_0x11C::sub_50B8B0()
{
}

STUB_FUNC(0x50b910)
void miss2_0x11C::SCRCMD_IS_CHAR_FIRING_AREA_50B910()
{
}

STUB_FUNC(0x50b9c0)
void miss2_0x11C::SCRCMD_GET_PASSENGER_NUM_50B9C0()
{
}

STUB_FUNC(0x50ba30)
void miss2_0x11C::sub_50BA30()
{
}

STUB_FUNC(0x50ba70)
void miss2_0x11C::sub_50BA70()
{
}

STUB_FUNC(0x50bad0)
void miss2_0x11C::SCRCMD_CAR_WRECK_IN_LOCATION_50BAD0()
{
}

STUB_FUNC(0x50bb80)
void miss2_0x11C::sub_50BB80()
{
}

STUB_FUNC(0x50bbd0)
void miss2_0x11C::sub_50BBD0()
{
}

STUB_FUNC(0x50bc60)
void miss2_0x11C::SCRCMD_CHECK_NUM_ALIVE_50BC60()
{
}

STUB_FUNC(0x50bcd0)
void miss2_0x11C::SCRCMD_SET_MIN_ALIVE_50BCD0()
{
}

STUB_FUNC(0x50bd10)
void miss2_0x11C::sub_50BD10()
{
}

STUB_FUNC(0x50bdc0)
void miss2_0x11C::sub_50BDC0()
{
}

STUB_FUNC(0x50be00)
void miss2_0x11C::SCRCMD_HAS_CAR_WEAPON_50BE00()
{
}

STUB_FUNC(0x50be70)
void miss2_0x11C::SCRCMD_IS_CHAR_HORN_50BE70()
{
}

STUB_FUNC(0x50bed0)
void miss2_0x11C::SCRCMD_CHECK_MAX_PASS_50BED0()
{
}

STUB_FUNC(0x50bf40)
void miss2_0x11C::SCRCMD_IS_CHAR_IN_ZONE_50BF40()
{
}

STUB_FUNC(0x50c040)
void miss2_0x11C::SCRCMD_SET_PHONE_DEAD_50C040()
{
}

STUB_FUNC(0x50c0e0)
void miss2_0x11C::SCRCMD_IS_TRAILER_ATT_50C0E0()
{
}

STUB_FUNC(0x50c1b0)
void miss2_0x11C::SCRCMD_IS_CAR_ON_TRAIL_50C1B0()
{
}

STUB_FUNC(0x50c230)
void miss2_0x11C::SCRCMD_ENABLE_DISABLE_CRANE_50C230()
{
}

STUB_FUNC(0x50c2a0)
void miss2_0x11C::SCRCMD_CAR_GOT_DRIVER_50C2A0()
{
}

STUB_FUNC(0x50c2f0)
void miss2_0x11C::SCRCMD_SPOTTED_PLAYER_50C2F0()
{
}

STUB_FUNC(0x50c350)
void miss2_0x11C::SCRCMD_GET_LAST_PUNCHED_50C350()
{
}

STUB_FUNC(0x50c3b0)
void miss2_0x11C::SCRCMD_IS_CHAR_STUNNED_50C3B0()
{
}

STUB_FUNC(0x50c410)
void miss2_0x11C::SCRCMD_KILL_ALL_PASSENG_50C410()
{
}

STUB_FUNC(0x50c470)
void miss2_0x11C::SCRCMD_IS_GROUP_IN_CAR_50C470()
{
}

STUB_FUNC(0x50c4e0)
void miss2_0x11C::SCRCMD_PUNCHED_SOMEONE_50C4E0()
{
}

STUB_FUNC(0x50c540)
void miss2_0x11C::SCRCMD_ADD_CHAR_TO_GANG_50C540()
{
}

STUB_FUNC(0x50c5a0)
void miss2_0x11C::sub_50C5A0()
{
}

STUB_FUNC(0x50c6f0)
void miss2_0x11C::sub_50C6F0()
{
}

STUB_FUNC(0x50c760)
void miss2_0x11C::SCRCMD_BEEN_PUNCHED_BY_50C760()
{
}

STUB_FUNC(0x50c7d0)
void miss2_0x11C::SCRCMD_UPDATE_DOOR_50C7D0()
{
}

STUB_FUNC(0x50c8a0)
void miss2_0x11C::SCRCMD_DOOR_50C8A0()
{
}

STUB_FUNC(0x50c990)
void miss2_0x11C::SCRCMD_REMOVE_WEAPON_50C990()
{
}

STUB_FUNC(0x50c9f0)
void miss2_0x11C::SCRCMD_REMOVE_BLOCK_50C9F0()
{
}

STUB_FUNC(0x50ca30)
void miss2_0x11C::SCRCMD_LOWER_LEVEL_50CA30()
{
}

STUB_FUNC(0x50ca70)
void miss2_0x11C::sub_50CA70()
{
}

STUB_FUNC(0x50cab0)
void miss2_0x11C::SCRCMD_ADD_NEW_BLOCK_50CAB0()
{
}

STUB_FUNC(0x50cb20)
void miss2_0x11C::SCRCMD_ROAD_ON_OFF_50CB20()
{
}

STUB_FUNC(0x50cb70)
void miss2_0x11C::sub_50CB70()
{
}

STUB_FUNC(0x50ccb0)
void miss2_0x11C::sub_50CCB0()
{
}

STUB_FUNC(0x50cd30)
void miss2_0x11C::sub_50CD30()
{
}

STUB_FUNC(0x50cdb0)
void miss2_0x11C::SCRCMD_CAR_DAMAGE_POS_50CDB0()
{
}

STUB_FUNC(0x50ce10)
void miss2_0x11C::SCRCMD_PARK_FINISHED_50CE10()
{
}

STUB_FUNC(0x50ce50)
void miss2_0x11C::sub_50CE50(s32 a2, u16 a3)
{
}

STUB_FUNC(0x50ce90)
void miss2_0x11C::SCRCMD_PHONE_TEMPLATE_50CE90()
{
}

STUB_FUNC(0x50d200)
void miss2_0x11C::SCRCMD_REMOTE_CONTROL_50D200()
{
}

STUB_FUNC(0x50d2e0)
s32 miss2_0x11C::SCRCMD_LAUNCH_MISSION_50D2E0()
{
    return 0;
}

STUB_FUNC(0x50d340)
void miss2_0x11C::SCRCMD_SAVE_GAME_50D340()
{
}

STUB_FUNC(0x50d3c0)
void miss2_0x11C::sub_50D3C0()
{
}

STUB_FUNC(0x50d680)
void miss2_0x11C::SCRCMD_CHANGE_CAR_LOCK_50D680()
{
}

STUB_FUNC(0x50d870)
void miss2_0x11C::SCRCMD_CHANGE_INTENSITY_50D870()
{
}

STUB_FUNC(0x50d900)
void miss2_0x11C::SCRCMD_CHANGE_COLOUR_50D900()
{
}

STUB_FUNC(0x50d9a0)
void miss2_0x11C::SCRCMD_CHANGE_RADIUS_50D9A0()
{
}

STUB_FUNC(0x50da50)
void miss2_0x11C::sub_50DA50()
{
}

STUB_FUNC(0x50db70)
void miss2_0x11C::sub_50DB70()
{
}

STUB_FUNC(0x50dd00)
void miss2_0x11C::sub_50DD00()
{
}

STUB_FUNC(0x50dd90)
void miss2_0x11C::SCRCMD_SET_DIR_OF_TVVAN_50DD90()
{
}

STUB_FUNC(0x50de00)
void miss2_0x11C::SCRCMD_POINT_ONSCREEN_50DE00()
{
}

STUB_FUNC(0x50de50)
void miss2_0x11C::SCRCMD_CHAR_IN_AIR_50DE50()
{
}

STUB_FUNC(0x50deb0)
void miss2_0x11C::SCRCMD_CHAR_SUNK_50DEB0()
{
}

STUB_FUNC(0x50df10)
void miss2_0x11C::sub_50DF10()
{
}

STUB_FUNC(0x50e0b0)
void miss2_0x11C::sub_50E0B0()
{
}

STUB_FUNC(0x50e120)
void miss2_0x11C::sub_50E120()
{
}

STUB_FUNC(0x50e150)
void miss2_0x11C::sub_50E150()
{
}

STUB_FUNC(0x50e190)
void miss2_0x11C::sub_50E190()
{
}

STUB_FUNC(0x50e360)
void miss2_0x11C::SCRCMD_CHECK_CAR_SPEED_50E360()
{
}

STUB_FUNC(0x50e460)
void miss2_0x11C::sub_50E460()
{
}

STUB_FUNC(0x50e4a0)
void miss2_0x11C::SCRCMD_CHAR_ARRESTED_50E4A0()
{
}

STUB_FUNC(0x50e4f0)
void miss2_0x11C::sub_50E4F0()
{
}

STUB_FUNC(0x50e610)
void miss2_0x11C::sub_50E610()
{
}

STUB_FUNC(0x50e730)
void miss2_0x11C::sub_50E730()
{
}

STUB_FUNC(0x50e780)
void miss2_0x11C::sub_50E780()
{
}

STUB_FUNC(0x50e7f0)
void miss2_0x11C::sub_50E7F0()
{
}

STUB_FUNC(0x50e820)
void miss2_0x11C::sub_50E820()
{
}

STUB_FUNC(0x50e900)
void miss2_0x11C::sub_50E900()
{
}

STUB_FUNC(0x50e9a0)
void miss2_0x11C::sub_50E9A0()
{
}

STUB_FUNC(0x50e9e0)
void miss2_0x11C::sub_50E9E0()
{
}

STUB_FUNC(0x50ea40)
void miss2_0x11C::sub_50EA40()
{
}

STUB_FUNC(0x50eb00)
void miss2_0x11C::sub_50EB00()
{
}

STUB_FUNC(0x50ebd0)
void miss2_0x11C::sub_50EBD0()
{
}

STUB_FUNC(0x50ece0)
void miss2_0x11C::sub_50ECE0()
{
}

STUB_FUNC(0x50ed40)
void miss2_0x11C::sub_50ED40()
{
}

STUB_FUNC(0x50ed80)
void miss2_0x11C::sub_50ED80()
{
}

STUB_FUNC(0x50edc0)
void miss2_0x11C::sub_50EDC0()
{
}

STUB_FUNC(0x50f060)
void miss2_0x11C::SCRCMD_CHAR_INTO_CAR_50F060()
{
}

STUB_FUNC(0x50f150)
void miss2_0x11C::SCRCMD_DECIDE_POWERUP_50F150()
{
}

STUB_FUNC(0x50f220)
void miss2_0x11C::SCRCMD_SUPPRESS_MODEL_50F220()
{
}

STUB_FUNC(0x50f270)
s32 miss2_0x11C::sub_50F270()
{
    return 0;
}

STUB_FUNC(0x50f3d0)
void miss2_0x11C::SCRCMD_SET_GROUP_TYPE_50F3D0()
{
}

STUB_FUNC(0x50f410)
void miss2_0x11C::SCRCMD_CHAR_DO_NOTHING_50F410()
{
}

STUB_FUNC(0x50f450)
void miss2_0x11C::SCRCMD_EMERG_LIGHTS_50F450()
{
}

STUB_FUNC(0x50f4d0)
void miss2_0x11C::SCRCMD_CHECK_OBJ_MODEL_50F4D0()
{
}

STUB_FUNC(0x50f550)
void miss2_0x11C::SCRCMD_PED_GRAPHIC_50F550()
{
}

STUB_FUNC(0x50f5e0)
void miss2_0x11C::sub_50F5E0()
{
}

STUB_FUNC(0x50f770)
void miss2_0x11C::SCRCMD_MAKE_MUGGERS_50F770()
{
}

STUB_FUNC(0x50f7b0)
void miss2_0x11C::sub_50F7B0()
{
}

STUB_FUNC(0x50f900)
void miss2_0x11C::SCRCMD_STOP_CAR_DRIVE_50F900()
{
}

STUB_FUNC(0x50f940)
void miss2_0x11C::SCRCMD_IS_BUS_FULL_50F940()
{
}

STUB_FUNC(0x50f9b0)
void miss2_0x11C::SCRCMD_NO_CHARS_OFF_BUS_50F9B0()
{
}

STUB_FUNC(0x50fa00)
void miss2_0x11C::sub_50FA00()
{
}

STUB_FUNC(0x50fa40)
void miss2_0x11C::SCRCMD_SET_SHADING_LEV_50FA40()
{
}

STUB_FUNC(0x50fa70)
void miss2_0x11C::SCRCMD_SET_CAR_JAMMED_50FA70()
{
}

STUB_FUNC(0x50fad0)
void miss2_0x11C::SCRCMD_FINISH_MISSION_50FAD0()
{
}

STUB_FUNC(0x50faf0)
void miss2_0x11C::sub_50FAF0()
{
}

STUB_FUNC(0x50fb60)
void miss2_0x11C::sub_50FB60()
{
}

STUB_FUNC(0x50fc20)
void miss2_0x11C::sub_50FC20()
{
}

STUB_FUNC(0x50fc60)
void miss2_0x11C::sub_50FC60()
{
}

STUB_FUNC(0x50fe00)
void miss2_0x11C::sub_50FE00()
{
}

STUB_FUNC(0x50fed0)
void miss2_0x11C::sub_50FED0()
{
}

STUB_FUNC(0x50ff50)
void miss2_0x11C::sub_50FF50()
{
}

STUB_FUNC(0x50ffb0)
void miss2_0x11C::sub_50FFB0()
{
}

STUB_FUNC(0x510030)
void miss2_0x11C::sub_510030()
{
}

STUB_FUNC(0x510050)
void miss2_0x11C::sub_510050()
{
}

STUB_FUNC(0x510090)
void miss2_0x11C::sub_510090()
{
}

STUB_FUNC(0x510100)
void miss2_0x11C::sub_510100()
{
}

STUB_FUNC(0x510280)
void miss2_0x11C::sub_510280()
{
}

STUB_FUNC(0x510530)
void miss2_0x11C::sub_510530()
{
}

STUB_FUNC(0x510560)
void miss2_0x11C::sub_510560()
{
}

STUB_FUNC(0x5105b0)
void miss2_0x11C::sub_5105B0()
{
}

STUB_FUNC(0x510600)
void miss2_0x11C::sub_510600()
{
}

STUB_FUNC(0x510660)
void miss2_0x11C::sub_510660()
{
}

STUB_FUNC(0x510780)
void miss2_0x11C::sub_510780()
{
}

MATCH_FUNC(0x5108d0)
void miss2_0x11C::PreExecOpCode_5108D0()
{
    if (this->field_10 != 1)
    {
        SCR_CMD_HEADER* pCmd = gfrosty_pasteur_6F8060->GetBasePointer_512770(this->field_4_level_start);
        gBasePtr_6F8070 = pCmd;
        switch (pCmd->field_2_type)
        {
            case 41:
            case 42:
                miss2_0x11C::SCR_CMD_CREATE_CHAR_509730();
                break;
            case 43:
            case 44:
            case 45:
            case 46:
            case 394:
            case 395:
            case 396:
            case 397:
                miss2_0x11C::sub_507F80();
                break;
            case 47:
            case 48:
            case 49:
            case 50:
            case 51:
            case 52:
                miss2_0x11C::sub_507CE0();
                break;
            case 59:
                miss2_0x11C::SCRCMD_LEVELSTART_5069C0();
                break;
            case 60:
                miss2_0x11C::SCRCMD_LEVELEND_5069F0();
                break;
            case 61:
                miss2_0x11C::SCRCMD_CREATE_THREAD_506A00();
                break;
            case 62:
                miss2_0x11C::SCRCMD_STOP_THREAD_506A60();
                break;
            case 63:
                miss2_0x11C::SCRCMD_START_EXEC_5078D0();
                break;
            case 64:
                miss2_0x11C::SCRCMD_STOP_EXEC_5079A0();
                break;
            case 68:
            case 273:
                miss2_0x11C::sub_506B80();
                break;
            case 71:
                miss2_0x11C::SCRCMD_NOT_507750();
                break;
            case 78:
                miss2_0x11C::sub_506B30();
                break;
            case 79:
            case 82:
            case 86:
            case 88:
            case 90:
            case 92:
            case 94:
                miss2_0x11C::sub_506D60();
                break;
            case 80:
            case 83:
                miss2_0x11C::sub_507110();
                break;
            case 81:
            case 84:
            case 87:
            case 89:
            case 91:
            case 93:
            case 95:
                miss2_0x11C::sub_506ED0();
                break;
            case 96:
                miss2_0x11C::SCRCMD_INCREMENT_507A70();
                break;
            case 97:
                miss2_0x11C::SCRCMD_DECREMENT_507B50();
                break;
            case 98:
                miss2_0x11C::SCRCMD_IF_JUMP_506AF0(); // IF_JUMP?
                break;
            case 100:
                miss2_0x11C::SCRCMD_MAKE_CAR_DUMMY_508220();
                break;
            case 103:
            case 104:
            case 105:
            case 106:
            case 107:
            case 108:
            case 109:
            case 110:
            case 111:
            case 112:
            case 372:
                miss2_0x11C::sub_508280();
                break;
            case 113:
            case 440:
                miss2_0x11C::sub_5086F0();
                break;
            case 114:
            case 441:
                miss2_0x11C::sub_508550();
                break;
            case 115:
                miss2_0x11C::SCRCMD_ARROW_COLOUR_508DC0();
                break;
            case 116:
                miss2_0x11C::SCRCMD_REMOVE_ARROW_508E80();
                break;
            case 117:
            case 118:
            case 279:
            case 321:
                miss2_0x11C::sub_5093C0();
                break;
            case 119:
                miss2_0x11C::SCRCMD_DISPLAY_TIMER_5096E0();
                break;
            case 120:
                miss2_0x11C::sub_50B690();
                break;
            case 121:
                miss2_0x11C::sub_509C10();
                break;
            case 122:
                miss2_0x11C::sub_509C90();
                break;
            case 123:
                miss2_0x11C::sub_509D00();
                break;
            case 124:
                miss2_0x11C::sub_509E70();
                break;
            case 125:
                miss2_0x11C::SCRCMD_IS_CHAR_STUNNED_50C3B0();
                break;
            case 126:
                miss2_0x11C::SCRCMD_CHECK_HEALTH_509030();
                break;
            case 127:
                miss2_0x11C::SCRCMD_HAS_CHAR_DIED_509BB0();
                break;
            case 128:
                miss2_0x11C::SCRCMD_STORE_CAR_INFO_509180();
                break;
            case 129:
                miss2_0x11C::SCRCMD_CHECK_CAR_DAMAGE_508F00();
                break;
            case 130:
            case 154:
            case 190:
            case 191:
            case 267:
            case 305:
            case 417:
                miss2_0x11C::sub_50CB70();
                break;
            case 131:
                miss2_0x11C::sub_5097D0();
                break;
            case 132:
                miss2_0x11C::sub_50A200();
                break;
            case 133:
                miss2_0x11C::sub_50A3E0();
                break;
            case 134:
                miss2_0x11C::sub_509810();
                break;
            case 135:
                miss2_0x11C::sub_509880();
                break;
            case 138:
            case 266:
                miss2_0x11C::sub_5098E0();
                break;
            case 139:
                miss2_0x11C::sub_509990();
                break;
            case 140:
                miss2_0x11C::sub_509D60();
                break;
            case 141:
            case 392:
                miss2_0x11C::sub_509D90();
                break;
            case 142:
            case 399:
            case 404:
            case 406:
                miss2_0x11C::sub_509ED0();
                break;
            case 143:
                miss2_0x11C::SCRCMD_EXPLODE_BUILDING_509F60();
                break;
            case 144:
            case 398:
            case 403:
            case 405:
                miss2_0x11C::sub_50C5A0();
                break;
            case 145:
            case 146:
            case 147:
            case 148:
            case 149:
            case 150:
                miss2_0x11C::sub_509FD0();
                break;
            case 151:
            case 152:
            case 153:
            case 238:
            case 239:
                miss2_0x11C::sub_50BBD0();
                break;
            case 155:
                miss2_0x11C::SCRCMD_CHANGE_CAR_REMAP_50A570();
                break;
            case 156:
                miss2_0x11C::SCRCMD_CHANGE_CHAR_REMAP_50A5B0();
                break;
            case 157:
                miss2_0x11C::SCRCMD_CHECK_CAR_MODEL_50A610();
                break;
            case 158:
                miss2_0x11C::SCRCMD_CHECK_CAR_REMAP_50A670();
                break;
            case 159:
                miss2_0x11C::SCRCMD_CHECK_CAR_BOTH_50A6E0();
                break;
            case 160:
            case 356:
                miss2_0x11C::sub_50A760();
                break;
            case 161:
                miss2_0x11C::SCRCMD_DELAY_HERE_50A940();
                break;
            case 162:
                miss2_0x11C::sub_50A980();
                break;
            case 163:
                miss2_0x11C::sub_50BA30();
                break;
            case 164:
            case 421:
                miss2_0x11C::sub_50BA70();
                break;
            case 165:
                miss2_0x11C::sub_50B3D0();
                break;
            case 166:
                miss2_0x11C::sub_50B5A0();
                break;
            case 167:
            case 171:
                miss2_0x11C::sub_50B4F0();
                break;
            case 168:
                miss2_0x11C::sub_50B180();
                break;
            case 169:
                miss2_0x11C::sub_50BB80();
                break;
            case 170:
                miss2_0x11C::sub_50B600();
                break;
            case 173:
                miss2_0x11C::sub_50B8B0();
                break;
            case 175:
                miss2_0x11C::SCRCMD_CAR_DRIVE_AWAY_50B440();
                break;
            case 176:
                miss2_0x11C::SCRCMD_IS_CHAR_FIRING_AREA_50B910();
                break;
            case 177:
            case 178:
            case 179:
            case 180:
                miss2_0x11C::SCRCMD_DOOR_50C8A0();
                break;
            case 181:
                miss2_0x11C::SCRCMD_ROAD_ON_OFF_50CB20();
                break;
            case 182:
                miss2_0x11C::SCRCMD_ADD_NEW_BLOCK_50CAB0();
                break;
            case 183:
                miss2_0x11C::SCRCMD_REMOVE_BLOCK_50C9F0();
                break;
            case 184:
                miss2_0x11C::SCRCMD_LOWER_LEVEL_50CA30();
                break;
            case 185:
            case 186:
            case 187:
                miss2_0x11C::sub_50CA70();
                break;
            case 188:
                miss2_0x11C::SCRCMD_GET_CAR_FROM_CRANE_50B470();
                break;
            case 189:
                miss2_0x11C::SCRCMD_CAR_WRECK_IN_LOCATION_50BAD0();
                break;
            case 192:
            case 196:
            case 281:
            case 282:
                miss2_0x11C::sub_50DB70();
                break;
            case 193:
            case 197:
                miss2_0x11C::sub_50DD00();
                break;
            case 194:
                miss2_0x11C::SCRCMD_CHECK_SCORE_50B6F0();
                break;
            case 195:
                miss2_0x11C::SCRCMD_GET_SCORE_50B760();
                break;
            case 198:
            case 199:
            case 231:
                miss2_0x11C::sub_50AEF0();
                break;
            case 200:
                miss2_0x11C::SCRCMD_CAR_DAMAGE_POS_50CDB0();
                break;
            case 201:
                miss2_0x11C::SCRCMD_GET_PASSENGER_NUM_50B9C0();
                break;
            case 202:
                miss2_0x11C::SCRCMD_CHAR_IN_AIR_50DE50();
                break;
            case 203:
                miss2_0x11C::SCRCMD_CHAR_SUNK_50DEB0();
                break;
            case 205:
                miss2_0x11C::sub_50B0E0();
                break;
            case 206:
            case 207:
            case 209:
                miss2_0x11C::sub_50E190();
                break;
            case 208:
                miss2_0x11C::SCRCMD_CHECK_CAR_SPEED_50E360();
                break;
            case 215:
                miss2_0x11C::SCRCMD_ENABLE_THREAD_50ABF0();
                break;
            case 216:
                miss2_0x11C::SCRCMD_DISABLE_THREAD_50ABC0();
                break;
            case 219:
            case 301:
                miss2_0x11C::sub_50DA50();
                break;
            case 220:
                miss2_0x11C::SCRCMD_CHANGE_INTENSITY_50D870();
                break;
            case 221:
                miss2_0x11C::SCRCMD_CHANGE_COLOUR_50D900();
                break;
            case 222:
                miss2_0x11C::SCRCMD_CHANGE_RADIUS_50D9A0();
                break;
            case 223:
                miss2_0x11C::SCRCMD_SET_GANG_INFO1_50B670();
                break;
            case 224:
                miss2_0x11C::SCRCMD_SET_GANG_RESPECT_50AC20();
                break;
            case 225:
            case 262:
            case 393:
                miss2_0x11C::sub_50ACF0();
                break;
            case 226:
                miss2_0x11C::SCRCMD_SET_AMBIENT_50B150();
                break;
            case 227:
                miss2_0x11C::SCRCMD_CHECK_PHONE_50B230();
                break;
            case 228:
                miss2_0x11C::SCRCMD_CHECK_PHONETIMER_50B2C0();
                break;
            case 229:
                miss2_0x11C::SCRCMD_STOP_PHONE_RING_50B360();
                break;
            case 232:
                miss2_0x11C::SCRCMD_IS_CHAR_IN_GANG_50B7D0();
                break;
            case 234:
                miss2_0x11C::SCRCMD_CHECK_NUM_ALIVE_50BC60();
                break;
            case 235:
            case 236:
            case 259:
                miss2_0x11C::sub_50BD10();
                break;
            case 237:
                miss2_0x11C::SCRCMD_SET_MIN_ALIVE_50BCD0();
                break;
            case 241:
                miss2_0x11C::SCRCMD_HAS_CAR_WEAPON_50BE00();
                break;
            case 242:
                miss2_0x11C::SCRCMD_IS_CHAR_IN_ZONE_50BF40();
                break;
            case 243:
                miss2_0x11C::SCRCMD_IS_CHAR_HORN_50BE70();
                break;
            case 244:
                miss2_0x11C::SCRCMD_CHECK_MAX_PASS_50BED0();
                break;
            case 245:
                miss2_0x11C::SCRCMD_SET_PHONE_DEAD_50C040();
                break;
            case 246:
                miss2_0x11C::SCRCMD_IS_TRAILER_ATT_50C0E0();
                break;
            case 247:
                miss2_0x11C::SCRCMD_IS_CAR_ON_TRAIL_50C1B0();
                break;
            case 248:
            case 249:
                miss2_0x11C::SCRCMD_ENABLE_DISABLE_CRANE_50C230();
                break;
            case 250:
                miss2_0x11C::SCRCMD_CAR_GOT_DRIVER_50C2A0();
                break;
            case 251:
                miss2_0x11C::SCRCMD_SPOTTED_PLAYER_50C2F0();
                break;
            case 252:
                miss2_0x11C::SCRCMD_GET_LAST_PUNCHED_50C350();
                break;
            case 253:
                miss2_0x11C::SCRCMD_KILL_ALL_PASSENG_50C410();
                break;
            case 254:
                miss2_0x11C::SCRCMD_IS_GROUP_IN_CAR_50C470();
                break;
            case 255:
                miss2_0x11C::SCRCMD_PUNCHED_SOMEONE_50C4E0();
                break;
            case 256:
                miss2_0x11C::SCRCMD_REMOVE_WEAPON_50C990();
                break;
            case 258:
                miss2_0x11C::SCRCMD_ADD_CHAR_TO_GANG_50C540();
                break;
            case 260:
            case 280:
                miss2_0x11C::sub_50C6F0();
                break;
            case 261:
                miss2_0x11C::SCRCMD_PARK_FINISHED_50CE10();
                break;
            case 263:
                miss2_0x11C::SCRCMD_PHONE_TEMPLATE_50CE90();
                break;
            case 264:
                miss2_0x11C::SCRCMD_BEEN_PUNCHED_BY_50C760();
                break;
            case 265:
                miss2_0x11C::SCRCMD_UPDATE_DOOR_50C7D0();
                break;
            case 268:
            case 269:
                miss2_0x11C::sub_50CCB0();
                break;
            case 270:
                miss2_0x11C::SCRCMD_CAR_IN_AREA_509A70();
                break;
            case 271:
                miss2_0x11C::SCRCMD_REMOTE_CONTROL_50D200();
                break;
            case 274:
                miss2_0x11C::SCRCMD_LAUNCH_MISSION_50D2E0();
                break;
            case 277:
                miss2_0x11C::SCRCMD_SAVE_GAME_50D340();
                break;
            case 278:
                miss2_0x11C::SCRCMD_CHANGE_CAR_LOCK_50D680();
                break;
            case 283:
                miss2_0x11C::SCRCMD_SET_DIR_OF_TVVAN_50DD90();
                break;
            case 284:
                miss2_0x11C::SCRCMD_POINT_ONSCREEN_50DE00();
                break;
            case 288:
            case 289:
            case 290:
            case 291:
                miss2_0x11C::sub_50DF10();
                break;
            case 292:
            case 293:
            case 294:
                miss2_0x11C::sub_50E0B0();
                break;
            case 299:
                miss2_0x11C::sub_50E120();
                break;
            case 300:
                miss2_0x11C::sub_50E150();
                break;
            case 303:
                miss2_0x11C::sub_50E460();
                break;
            case 306:
                miss2_0x11C::sub_50E730();
                break;
            case 307:
            case 383:
                miss2_0x11C::sub_50E780();
                break;
            case 308:
            case 309:
            case 337:
            case 338:
            case 339:
                miss2_0x11C::sub_50E4F0();
                break;
            case 310:
                miss2_0x11C::sub_50E7F0();
                break;
            case 311:
            case 312:
            case 313:
                miss2_0x11C::sub_50E820();
                break;
            case 315:
                miss2_0x11C::sub_50A460();
                break;
            case 316:
                miss2_0x11C::sub_50E900();
                break;
            case 317:
                miss2_0x11C::sub_50E9A0();
                break;
            case 318:
                miss2_0x11C::sub_50E9E0();
                break;
            case 319:
                miss2_0x11C::sub_50EA40();
                break;
            case 320:
                miss2_0x11C::sub_50EB00();
                break;
            case 324:
                miss2_0x11C::sub_50ECE0();
                break;
            case 325:
                miss2_0x11C::sub_50ED40();
                break;
            case 328:
                miss2_0x11C::sub_50ED80();
                break;
            case 329:
                miss2_0x11C::sub_50EDC0();
                break;
            case 330:
                miss2_0x11C::SCRCMD_CHAR_INTO_CAR_50F060();
                break;
            case 332:
            case 333:
            case 334:
            case 335:
            case 336:
                miss2_0x11C::sub_50E610();
                break;
            case 340:
                miss2_0x11C::sub_50BDC0();
                break;
            case 342:
                miss2_0x11C::SCRCMD_ADD_SCORE2_509E00();
                break;
            case 350:
                miss2_0x11C::SCRCMD_SUPPRESS_MODEL_50F220();
                break;
            case 351:
            case 352:
                miss2_0x11C::sub_50CD30();
                break;
            case 354:
                miss2_0x11C::SCRCMD_DECIDE_POWERUP_50F150();
                break;
            case 355:
                miss2_0x11C::SCRCMD_CHAR_ARRESTED_50E4A0();
                break;
            case 357:
                miss2_0x11C::sub_50F270();
                break;
            case 358:
            case 366:
                miss2_0x11C::sub_50EBD0();
                break;
            case 359:
                miss2_0x11C::SCRCMD_SET_GROUP_TYPE_50F3D0();
                break;
            case 360:
                miss2_0x11C::SCRCMD_CHAR_DO_NOTHING_50F410();
                break;
            case 361:
                miss2_0x11C::SCRCMD_EMERG_LIGHTS_50F450();
                break;
            case 362:
                miss2_0x11C::SCRCMD_CHECK_OBJ_MODEL_50F4D0();
                break;
            case 363:
            case 384:
            case 385:
            case 432:
                miss2_0x11C::sub_50F5E0();
                break;
            case 364:
                miss2_0x11C::SCRCMD_PED_GRAPHIC_50F550();
                break;
            case 365:
                miss2_0x11C::SCRCMD_MAKE_MUGGERS_50F770();
                break;
            case 367:
            case 381:
            case 382:
                miss2_0x11C::sub_50F7B0();
                break;
            case 368:
                miss2_0x11C::SCRCMD_STOP_CAR_DRIVE_50F900();
                break;
            case 369:
                miss2_0x11C::SCRCMD_IS_BUS_FULL_50F940();
                break;
            case 370:
                miss2_0x11C::SCRCMD_NO_CHARS_OFF_BUS_50F9B0();
                break;
            case 371:
                miss2_0x11C::sub_50FA00();
                break;
            case 373:
                miss2_0x11C::SCRCMD_SET_SHADING_LEV_50FA40();
                break;
            case 374:
                miss2_0x11C::SCRCMD_SET_CAR_JAMMED_50FA70();
                break;
            case 391:
                miss2_0x11C::SCRCMD_FINISH_MISSION_50FAD0();
                break;
            case 400:
            case 407:
                miss2_0x11C::sub_50FB60();
                break;
            case 402:
                miss2_0x11C::sub_50FAF0();
                break;
            case 408:
                miss2_0x11C::sub_50FC20();
                break;
            case 409:
                miss2_0x11C::sub_50FC60();
                break;
            case 410:
                miss2_0x11C::sub_509790();
                break;
            case 411:
                miss2_0x11C::sub_50FE00();
                break;
            case 412:
                miss2_0x11C::sub_50FED0();
                break;
            case 414:
                miss2_0x11C::sub_50FF50();
                break;
            case 415:
            case 416:
                miss2_0x11C::sub_50FFB0();
                break;
            case 418:
                miss2_0x11C::sub_510030();
                break;
            case 419:
                miss2_0x11C::sub_510050();
                break;
            case 420:
                miss2_0x11C::sub_510090();
                break;
            case 422:
                miss2_0x11C::sub_510100();
                break;
            case 423:
                miss2_0x11C::sub_510280();
                break;
            case 424:
                miss2_0x11C::sub_510530();
                break;
            case 433:
                miss2_0x11C::sub_510560();
                break;
            case 438:
                miss2_0x11C::sub_50D3C0();
                break;
            case 442:
                miss2_0x11C::sub_5105B0();
                break;
            case 443:
                miss2_0x11C::sub_510600();
                break;
            case 444:
                miss2_0x11C::sub_510660();
                break;
            case 445:
            case 446:
                miss2_0x11C::sub_510780();
                break;
            default:
                miss2_0x11C::Next_503620(pCmd);
                break;
        }
    }
}

MATCH_FUNC(0x511840)
char_type miss2_0x11C::sub_511840()
{ //  return boolean: true if it has reached LEVELEND, false otherwise
    SCR_CMD_HEADER* BasePointer_512770;
    bool v4;
    SCR_CMD_HEADER* v5;

    //  I guess it's the script executed in mid-game on the current frame
    //  Normally each opcode is executed in one frame, unless there are WHILE_EXECs or EXECs blocks

    if (this->field_10 == 1)
    {
        return true;
    }

    BasePointer_512770 = gfrosty_pasteur_6F8060->GetBasePointer_512770(this->field_4_level_start);

    if (!BasePointer_512770)
    {
        sprintf(gTmpBuffer_67C598, "Miss2: accessing nonexistant mission line. Current uid: %d", this->field_4_level_start);
    }

    miss2_0x11C::sub_503200();

    if (BasePointer_512770->field_2_type == 0x3F) //  3F 00 = EXEC opcode
    {
        do
        {
            miss2_0x11C::PreExecOpCode_5108D0();
            gfrosty_pasteur_6F8060->GetBasePointer_512770(this->field_4_level_start);
        } while (this->field_12 > 0); //  execute opcodes in the same frame until an ENDEXEC
    }
    else //  It isn't an EXEC opcode
    {
        // field_6_return_value == 1  means it's in a WHILE_EXEC block
        if (BasePointer_512770->field_6_return_value == 1)
        {
            while (1) // execute commands in the same frame
            {
                v5 = gfrosty_pasteur_6F8060->GetBasePointer_512770(this->field_4_level_start);
                miss2_0x11C::PreExecOpCode_5108D0();

                //  If the script has reached a command out of the WHILE_EXEC block, or if it reaches LEVELEND
                if (v5->field_6_return_value != 1 || this->field_4_level_start == 0xFFFF)
                {
                    break;
                }

                //  Maybe if it reaches an ENDEXEC?
                if (this->field_12)
                {
                    break;
                }
            }
        }
        else
        {
            //  or else, execute only one command on this frame
            miss2_0x11C::PreExecOpCode_5108D0();
        }
    }

    v4 = this->field_4_level_start == 0xFFFF; // Did it reached the LEVELEND?
    this->field_12 = 0; // Clear EXEC flag?
    return v4;
}

STUB_FUNC(0x511930)
s32 miss2_0x11C::sub_511930(char_type a2, u16 levelStart)
{
    return 0;
}

STUB_FUNC(0x511960)
miss2_0x11C* miss2_0x11C::sub_511960(s16 a2)
{
    return 0;
}

STUB_FUNC(0x5119a0)
s32 miss2_0x11C::launch_mission_5119A0(s32 a2, char_type* String1)
{
    return 0;
}

MATCH_FUNC(0x511a00)
miss2_0x11C::miss2_0x11C()
{
    this->field_0 = 0;
    this->field_114 = new miss2_8();
    this->field_4_level_start = 0;
    this->field_6 = 0;
    this->field_8 = 0;
    this->field_C = 0;
    this->field_E = 0;
    this->field_10 = 0;
    this->field_12 = 0;
    this->field_11A = 0;

    memset(this->field_14_str, 0, sizeof(this->field_14_str));

    this->field_118 = 0;
}

STUB_FUNC(0x511cd0)
void miss2_0x11C::sub_511CD0()
{
}

STUB_FUNC(0x512fd0)
miss2_0x11C::~miss2_0x11C()
{
}
