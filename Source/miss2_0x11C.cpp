#include "miss2_0x11c.hpp"
#include "Car_BC.hpp"
#include "Globals.hpp"
#include "cool_nash_0x294.hpp"
#include "error.hpp"
#include "frosty_pasteur_0xC1EA8.hpp"
#include "miss2_8.hpp"
#include "root_sound.hpp"
#include "char.hpp"
#include "Game_0x40.hpp"
#include "fix16.hpp"
#include "ang16.hpp"
#include "map_0x370.hpp"
#include "angry_lewin_0x85C.hpp"
#include "Object_5C.hpp"
#include "Sero_181C.hpp"
#include "Zones_CA8.hpp"
#include "Zheal_D9C.hpp"
#include "ChickenLegend_48.hpp"
#include "Miss2_25C.hpp"
#include "nostalgic_ellis_0x28.hpp"
#include "Weapon_8.hpp"
#include "Police_7B8.hpp"
#include "Snooky_94.hpp"
#include "text_0x14.hpp"
#include "debug.hpp"

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

EXPORT_VAR Fix16 dword_6F7920;
GLOBAL(dword_6F7920, 0x6F7920);

EXPORT_VAR Fix16 dword_6F76DC;
GLOBAL(dword_6F76DC, 0x6F76DC);

EXPORT_VAR Fix16 dword_6F75F0;
GLOBAL(dword_6F75F0, 0x6F75F0);

EXPORT_VAR Fix16 dword_6F791C;
GLOBAL(dword_6F791C, 0x6F791C);

EXPORT_VAR Ang16 word_6F8044;
GLOBAL(word_6F8044, 0x6F8044);

EXPORT_VAR Fix16 dword_6F7924;
GLOBAL(dword_6F7924, 0x6F7924);

EXPORT_VAR Fix16 dword_6F7570;
GLOBAL(dword_6F7570, 0x6F7570);

EXPORT_VAR Ang16 dword_6F804C;
GLOBAL(dword_6F804C, 0x6F804C);

EXPORT_VAR u8 byte_6F799B;
GLOBAL(byte_6F799B, 0x6F799B);

static inline bool Is_it_in_area(SCR_XYZ_f* pos, SCR_Rect_f* rect)
{
    Fix16 x_pos = pos->field_0_x;
    Fix16 width = rect->field_C_size.field_0_x;
    Fix16 y_pos, z_pos, z_target, height;

    return (x_pos >= rect->field_0_pos.field_0_x - width 
        && x_pos <= rect->field_0_pos.field_0_x + width 
        && (y_pos = pos->field_4_y, 
        height = rect->field_C_size.field_4_y, 
        y_pos >= rect->field_0_pos.field_4_y - height) 
        && pos->field_4_y <= rect->field_0_pos.field_4_y + height 
        && (z_pos = pos->field_8_z, 
        z_target = rect->field_0_pos.field_8_z, 
        z_pos.ToUInt8() == z_target.ToUInt8()));
}

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
        dword_6F806C = field_4_level_start;
        field_4_level_start = a2->field_4_cmd_next;
        field_C = 0;
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
        field_4_level_start = a2;
    }
    else
    {
        miss2_0x11C::sub_503670();
    }
}

MATCH_FUNC(0x503670)
void miss2_0x11C::sub_503670()
{
    field_4_level_start = 0xFFFFu;
}

STUB_FUNC(0x503680)
s32 miss2_0x11C::SCRCMD_OBJ_DECSET_2D_3D_503680(s32* a1, s32 a2)
{
    return 0;
}

MATCH_FUNC(0x5038d0)
void miss2_0x11C::SCRCMD_OBJ_DECSET_5038D0(SCR_OBJ_DATA* pCmd, SCR_POINTER* a2)
{
    if (pCmd->field_C_pos.field_8_z == dword_6F7570)
    {
        Fix16 temp_z;
        pCmd->field_C_pos.field_8_z = *gMap_0x370_6F6268->sub_4E5B60(&temp_z, 
                                                        pCmd->field_C_pos.field_0_x, 
                                                        pCmd->field_C_pos.field_4_y);
    }

    if (pCmd->field_18_obj_id < 0xC8u 
        || pCmd->field_18_obj_id > 0xF4u)
    {
        Ang16 rotation(NULL, NULL); //  TODO: fix default ctor
        rotation.ConvertAndMultiply(&word_6F8044, &pCmd->field_1A_rot);
        rotation.sub_406C20();

        a2->field_8_obj = gObject_5C_6F8F84->sub_5299B0(pCmd->field_18_obj_id,
                                                        pCmd->field_C_pos.field_0_x,
                                                        pCmd->field_C_pos.field_4_y,
                                                        pCmd->field_C_pos.field_8_z,
                                                        rotation);
    }
    else
    {
        Ang16 rotation(NULL, NULL); //  TODO: fix default ctor
        rotation.ConvertAndMultiply(&word_6F8044, &pCmd->field_1A_rot);
        rotation.sub_406C20();

        a2->field_8_obj = gObject_5C_6F8F84->sub_529BC0(pCmd->field_18_obj_id,
                                                        pCmd->field_C_pos.field_0_x,
                                                        pCmd->field_C_pos.field_4_y,
                                                        pCmd->field_C_pos.field_8_z,
                                                        rotation);
    }

    if (a2->field_8_obj != NULL)
    {
        Object_2C* pObj;

        if (a2->field_8_obj->field_8->field_34 == 1)
        {
            a2->field_8_obj->field_26 = pCmd->field_1C_value_shop_type;
        }
        else if (pObj = a2->field_8_obj, pObj->check_is_shop())
        {
            pObj->field_26 = pCmd->field_1C_value_shop_type;
        }
        else
        {
            if (a2->field_8_obj->check_is_busy_shop())
            {
                a2->field_8_obj->field_26 = pCmd->field_1C_value_shop_type;
            }
        }
    }
}

MATCH_FUNC(0x503a20)
void miss2_0x11C::SCRCMD_PLAYER_PED_503A20(SCR_PLAYER_PED* pCmd)
{

    angry_lewin_0x85C* v1 = gGame_0x40_67E008->sub_4B9750();

    if (v1)
    {
        cool_nash_0x294* pPed;
        if (gfrosty_pasteur_6F8060->field_C1E2C)
        {
            Fix16 weird_y = dword_6F76DC.ConcatenateWord(dword_6F7920);
            Fix16 weird_x = dword_6F75F0.ConcatenateWord(dword_6F791C);

            pPed = gChar_C_6787BC->sub_470A50(weird_x,
                                            weird_y, 
                                            dword_6F7924, 
                                            byte_6F799B, 
                                            dword_6F804C);
        }
        else
        {
            if (pCmd->field_C_pos.field_8_z == dword_6F7570) //  dword_6F7570 is 255.0
            {
                Fix16 temp_z;
                //  Calculate the real Z position at (X,Y) based on the map
                pCmd->field_C_pos.field_8_z =
                    *gMap_0x370_6F6268->sub_4E5B60(&temp_z,
                                                    pCmd->field_C_pos.field_0_x,
                                                    pCmd->field_C_pos.field_4_y);
            }

            Ang16 rotation;
            rotation.ConvertAndMultiply(&word_6F8044, &pCmd->field_18_rot);
            rotation.Normalize();

            pPed = gChar_C_6787BC->sub_470A50(pCmd->field_C_pos.field_0_x,
                                             pCmd->field_C_pos.field_4_y,
                                             pCmd->field_C_pos.field_8_z,
                                             pCmd->field_1A_remap,
                                             rotation);
        }

        if (pPed != NULL)
        {
            pPed->field_238 = 2;
            if (!gfrosty_pasteur_6F8060->field_C1E2C)
            {
                pPed->field_216_health = 100;
            }

            v1->sub_565490(pPed);
            pPed->field_26C = 1;
            pCmd->field_8_ped = pPed;

            Car_3C* v6 = pPed->sub_46DF50();
            v6->sub_5A2A30();
        }
    }
}

STUB_FUNC(0x503bc0)
void miss2_0x11C::SCRCMD_CAR_DECSET_503BC0(SCR_CAR_DATA_DEC* a1, SCR_POINTER* a2)
{
}

MATCH_FUNC(0x503f80)
void miss2_0x11C::SCRCMD_PARKED_CAR_DECSET_503F80(SCR_POINTER* pCmd)
{
    miss2_0x11C::SCRCMD_CAR_DECSET_503BC0((SCR_CAR_DATA_DEC*)pCmd, pCmd);
    (pCmd->field_8_car)->sub_443EB0(9);
    Car_BC* v1 = pCmd->field_8_car;
    v1->field_7C_uni_num = 4;
    v1->field_76 = 0;
}

MATCH_FUNC(0x503fb0)
void miss2_0x11C::SCRCMD_CHAR_DECSET_2D_3D_503FB0(SCR_CHAR_DATA_DEC* pCmd, SCR_POINTER* a2)
{
    cool_nash_0x294* pPed;

    if (pCmd->field_C_pos.field_8_z == dword_6F7570)
    {
        Fix16 temp_z;
        pCmd->field_C_pos.field_8_z = *gMap_0x370_6F6268->sub_4E5B60(&temp_z,
                                                                    pCmd->field_C_pos.field_0_x,
                                                                    pCmd->field_C_pos.field_4_y);
    }

    Ang16 rotation;
    rotation.ConvertAndMultiply(&word_6F8044, &pCmd->field_18_rot);
    rotation.Normalize();

    pPed = gChar_C_6787BC->sub_470A50(pCmd->field_C_pos.field_0_x,
                                      pCmd->field_C_pos.field_4_y,
                                      pCmd->field_C_pos.field_8_z,
                                      (u8)pCmd->field_1A_remap,
                                      rotation);
    a2->field_8_char = pPed;

    if (pPed)
    {
        a2->field_8_char->field_238 = 5;
        a2->field_8_char->field_240_occupation = pCmd->field_1C_occupation;
        a2->field_8_char->field_26C = 1;
        a2->field_8_char->sub_463570(26, 9999);
        a2->field_8_char->field_216_health = 100;
        Car_3C* v6 = a2->field_8_char->sub_46DF50();
        v6->sub_5A2A30();
    }
}

MATCH_FUNC(0x504110)
cool_nash_0x294* miss2_0x11C::sub_504110(SCR_CHAR_OBJECTIVE* a1, SCR_POINTER* a2)
{
    (a2->field_8_char)->sub_463570(a1->field_A_objective, 9999);
    cool_nash_0x294* v2 = a2->field_8_char;
    BitSet32 flag = v2->field_21C;
    flag.clear_bit(10);
    v2->field_21C = flag.m_var;
    return v2;
}

MATCH_FUNC(0x504150)
void miss2_0x11C::SCRCMD_MAP_ZONE_SET_504150(SCR_MAP_ZONE_SET* pCmd)
{
    str_table_entry* StringById_503080;

    StringById_503080 = gfrosty_pasteur_6F8060->FindStringById_503080(pCmd->field_0_cmd_this);
    gMap_0x370_6F6268->set_nav_unknown_data_4DF8C0(StringById_503080->field_2_zone_idx,
                                                   pCmd->field_8_car_density,
                                                   pCmd->field_A_goodcar_ratio,
                                                   pCmd->field_C_badcar_ratio,
                                                   pCmd->field_E_policecar_ratio,
                                                   pCmd->field_1C_gangcar_ratio,
                                                   pCmd->field_10_ped_density,
                                                   pCmd->field_12_mugger_ratio,
                                                   pCmd->field_14_carthief_ratio,
                                                   pCmd->field_16_elvis_ratio,
                                                   pCmd->field_18_gangchar_ratio,
                                                   pCmd->field_1A_policeped_ratio);
}

MATCH_FUNC(0x5041b0)
void miss2_0x11C::SCRCMD_ARROW_DEC_5041B0(SCR_TWO_PARAMS* pCmd)
{
    pCmd->field_8_u32 = 0;
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

MATCH_FUNC(0x5045a0)
void miss2_0x11C::SCRCMD_CRUSHER_BASIC_5045A0(SCR_CRUSHER_BASIC* a1, SCR_POINTER* a2)
{
    a2->field_8_crusher = gSnooky_94_67A830->sub_488820(a1->field_C_pos.field_0_x, 
                                                        a1->field_C_pos.field_4_y);
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

MATCH_FUNC(0x505030)
void miss2_0x11C::SCRCMD_SET_STATION_EMPTY_STATION_505030(SCR_SET_STATION* pCmd)
{
    str_table_entry* StringById_503080;
    gmp_map_zone* station_zone;
    Sero_34* v4;
    s32 num_passenger;
    s32 num_freight;

    if (!bSkip_trains_67D550)
    {
        StringById_503080 = gfrosty_pasteur_6F8060->FindStringById_503080(pCmd->field_8_platform);
        station_zone = gMap_0x370_6F6268->zone_by_name_4DEFD0((const char*)&StringById_503080[1]);
        v4 = gSero_181C_6FF1D4->sub_57B4B0(station_zone);

        for (u8 wagon_idx = 0; wagon_idx < 10; wagon_idx++)
        {
            if (wagon_idx < pCmd->field_A_num_passenger)
            {
                v4->field_24_train_wagons[wagon_idx] = 1; // passenger wagon
            }
            else
            {
                num_freight = pCmd->field_B_num_freight;
                num_passenger = pCmd->field_A_num_passenger;

                if (wagon_idx < num_passenger + num_freight)
                {
                    v4->field_24_train_wagons[wagon_idx] = 2; // freight wagon
                }
                else
                {
                    if (wagon_idx < num_passenger + num_freight + pCmd->field_C_num_boxcar)
                    {
                        v4->field_24_train_wagons[wagon_idx] = 3; // boxcar wagon
                    }
                }
            }
        }
        v4->field_2E_wagons_number = pCmd->field_A_num_passenger 
                                    + pCmd->field_B_num_freight 
                                    + pCmd->field_C_num_boxcar;
    }
}

MATCH_FUNC(0x5051d0)
void miss2_0x11C::SCRCMD_RADIOSTATION_DEC_5051D0(SCR_RADIOSTATION_DEC* a1)
{
    gRoot_sound_66B038.sub_40F030(a1->field_10_station_type, 
                                    a1->field_8_pos.field_0_x, 
                                    a1->field_8_pos.field_4_y);
}

MATCH_FUNC(0x505210)
void miss2_0x11C::CRCMD_SET_TRAIN_STATIONS_505210(SCR_TWO_PARAMS* pCmd)
{
    str_table_entry* StringById;

    if (!bSkip_trains_67D550)
    {
        StringById = gfrosty_pasteur_6F8060->FindStringById_503080(pCmd->field_8_unsigned_1);
        const char* station_zone_name = (const char*)&StringById[1];
        gmp_map_zone* station_zone = gMap_0x370_6F6268->zone_by_name_4DEFD0(station_zone_name);
        Sero_181C* v3 = (Sero_181C*)gSero_181C_6FF1D4->sub_57B4B0(station_zone);
        v3->sub_578820((u8*)&pCmd->field_A_unsigned_2);  //  The parameter may be the train wagon array, not sure
    }
}

MATCH_FUNC(0x5052c0)
void miss2_0x11C::SCRCMD_OBJ_DECSET_2D_STR_5052C0(SCR_TWO_PARAMS* pCmd) // OBS: Actually this is SCRCMD_DECLARE_POLICE_5052C0
{
    u8 max_wanted_level = pCmd->field_A_unsigned_2;
    if (max_wanted_level == 0)
    {
        bSkip_police_67D4F9 = 0;
    }
    gPolice_7B8_6FEE40->field_660_wanted_star_count = pCmd->field_A_unsigned_2; // max_wanted_level
}

MATCH_FUNC(0x505340)
void miss2_0x11C::SCRCMD_SOUND_DECSET_505340(SCR_SOUND_DECSET* a1, SCR_POINTER* a2)
{
    if (!a1->field_19_play_type)
    {
        a2->field_8_obj = gObject_5C_6F8F84->sub_5299F0(278,
                                                        a1->field_18_sound_id,
                                                        a1->field_C_pos.field_0_x,
                                                        a1->field_C_pos.field_4_y,
                                                        a1->field_C_pos.field_8_z);
    }
    else
    {
        a2->field_8_obj = gObject_5C_6F8F84->sub_5299F0(279,
                                                        a1->field_18_sound_id,
                                                        a1->field_C_pos.field_0_x,
                                                        a1->field_C_pos.field_4_y,
                                                        a1->field_C_pos.field_8_z);
    }
}

MATCH_FUNC(0x505430)
void miss2_0x11C::SCRCMD_SET_MISSIONS_TOTAL_505430(SCR_TWO_PARAMS* pCmd)
{
    switch (pCmd->field_2_type)
    {
        case SCRCMD_TOTAL_MISSIONS:
            gfrosty_pasteur_6F8060->field_314_total_missions = pCmd->field_A_signed_2;
            break;
        case SCRCMD_TOTAL_SECRETS:
            gfrosty_pasteur_6F8060->field_318_total_secrets = pCmd->field_A_signed_2;
            break;
        case SCRCMD_GANG_1_MISSION_TOTAL:
            gfrosty_pasteur_6F8060->field_31C_gang_1_missions_total = pCmd->field_A_signed_2;
            break;
        case SCRCMD_GANG_2_MISSION_TOTAL:
            gfrosty_pasteur_6F8060->field_320_gang_2_missions_total = pCmd->field_A_signed_2;
            break;
        case SCRCMD_GANG_3_MISSION_TOTAL:
            gfrosty_pasteur_6F8060->field_324_gang_3_missions_total = pCmd->field_A_signed_2;
            break;
        default:
            return;
    }
}

MATCH_FUNC(0x505580)
void miss2_0x11C::SCRCMD_PASSED_FAILED_FLAGS_505580(SCR_TWO_PARAMS* a1)
{
    SCR_POINTER* pCmd = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(a1->field_8_unsigned_1);

    switch (a1->field_2_type)
    {
        case SCRCMD_PASSED_FLAG:
            gfrosty_pasteur_6F8060->field_328_passed_flag = &pCmd->field_8_counter;
            break;
        case SCRCMD_1_PASSED_FLAG:
            gfrosty_pasteur_6F8060->field_32C_1_passed_flag = &pCmd->field_8_counter;
            break;
        case SCRCMD_2_PASSED_FLAG:
            gfrosty_pasteur_6F8060->field_330_2_passed_flag = &pCmd->field_8_counter;
            break;
        case SCRCMD_3_PASSED_FLAG:
            gfrosty_pasteur_6F8060->field_334_3_passed_flag = &pCmd->field_8_counter;
            break;
        case SCRCMD_SECRETS_PASSED:
            gfrosty_pasteur_6F8060->field_338_secrets_passed = &pCmd->field_8_counter;
            break;
        case SCRCMD_SECRETS_FAILED:
            gfrosty_pasteur_6F8060->field_33C_secrets_failed = &pCmd->field_8_counter;
            break;
        default:
            return;
    }
}

MATCH_FUNC(0x505710)
void miss2_0x11C::SCRCMD_FINISH_SCORE_505710(SCR_TWO_PARAMS* pCmd)
{
    gfrosty_pasteur_6F8060->field_310_finish_score = pCmd->field_8_s32;
}

MATCH_FUNC(0x505750)
void miss2_0x11C::SCRCMD_DECLARE_CARLIST_505750(SCR_TWO_PARAMS* pCmd)
{
    gfrosty_pasteur_6F8060->field_340_car_list = &pCmd->field_8_u32;
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

MATCH_FUNC(0x505f50)
void miss2_0x11C::SCRCMD_DISPLAY_MESSAGES_505F50(SCR_TWO_PARAMS* a1)
{
    if (gDo_text_id_test_67D6D0)
    {
        sprintf(gTmpBuffer_67C598, "%d", a1->field_8_unsigned_1);
        if (!gText_0x14_704DFC->Find_5B5F90(gTmpBuffer_67C598))
        {
            FatalError_4A38C0(0x47C,
                              "C:\\Splitting\\Gta2\\Source\\miss2.cpp",
                              2460,
                              a1->field_8_unsigned_1); // Mission script is referring to a 'text id' that doesn't exist. ID
        }
    }
}

MATCH_FUNC(0x506010)
void miss2_0x11C::SCRCMD_DEC_DEATH_BASE_506010(SCR_FOUR_PARAMS* pCmd)
{
    if (pCmd->field_2_type == SCRCMD_DEC_DEATH_BASE_1)
    {
        gfrosty_pasteur_6F8060->field_356 = pCmd->field_C_unsigned_3;
    }
    else if (pCmd->field_2_type == SCRCMD_DEC_DEATH_BASE_2)
    {
        gfrosty_pasteur_6F8060->field_358 = pCmd->field_C_unsigned_3;
    }
    else if (pCmd->field_2_type == SCRCMD_DEC_DEATH_BASE_3)
    {
        gfrosty_pasteur_6F8060->field_35A = pCmd->field_C_unsigned_3;
    }
}

MATCH_FUNC(0x5060d0)
void miss2_0x11C::SCRCMD_DO_CRANE_POWERUP_5060D0(SCR_DECLARE_CRANE_POWERUP* a1)
{
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(
                                        a1->field_8_crane_idx);
    pPointer->field_8_crane->field_140 = a1->field_0_cmd_this;
}

MATCH_FUNC(0x506140)
void miss2_0x11C::SCRCMD_START_BASIC_KF_506140(u16* a1)
{
    for (u16 idx = 0; idx < 25; idx++)
    {
        if (gfrosty_pasteur_6F8060->field_C1E74_basic_kf[idx] == 0)
        {
            gfrosty_pasteur_6F8060->field_C1E74_basic_kf[idx] = *a1;
            break;
        }
    }
}

STUB_FUNC(0x5061c0)
void miss2_0x11C::ExecOpCode_5061C0()
{
}

MATCH_FUNC(0x5069c0)
void miss2_0x11C::SCRCMD_LEVELSTART_5069C0()
{
    gRoot_sound_66B038.sub_40F090(24); //  "And remember, respect is everything!"
    field_118 = 1;
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x5069f0)
void miss2_0x11C::SCRCMD_LEVELEND_5069F0()
{
    field_118 = 0;
    miss2_0x11C::sub_503670();
}

MATCH_FUNC(0x506a00)
void miss2_0x11C::SCRCMD_CREATE_THREAD_506A00()
{
    SCR_CMD_HEADER* v1;
    SCR_CMD_HEADER* BasePointer_512770;
    miss2_0x11C* v4;

    v1 = gBasePtr_6F8070;
    BasePointer_512770 = gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_2_type);
    v4 = miss2_0x11C::sub_511960(v1[1].field_0_cmd_this);
    *(u32*)&BasePointer_512770[1].field_0_cmd_this = reinterpret_cast<unsigned int>(v4);
    BasePointer_512770[1].field_4_cmd_next = v4->field_11A;
    v4->sub_511930(field_6, v1[1].field_0_cmd_this);
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

STUB_FUNC(0x506a60)
void miss2_0x11C::SCRCMD_STOP_THREAD_506A60()
{
}

MATCH_FUNC(0x506af0)
void miss2_0x11C::SCRCMD_IF_JUMP_506AF0()
{
    //  field_8 = 1  means the boolean opcode has returned true
    //  field_8 = 0  means the boolean opcode has returned false

    //  If it's a OR and boolean is true, or if it's a AND and boolean is false, jump
    SCR_IF_JUMP* base_pointer = (SCR_IF_JUMP*)gBasePtr_6F8070;
    
    if ( ( base_pointer->is_or == 1 && field_8 )
        || (!base_pointer->is_or && !field_8 ) ) {
        
        sub_503650(base_pointer->else_endif_pointer);  //  Jump to the last IF_JUMP or go to ELSE section
        return;
    }

    Next_503620(gBasePtr_6F8070); // go to field_4_cmd_next
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
    field_8 = field_8 == 0; //  just toggle the boolean value
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x5078d0)
void miss2_0x11C::SCRCMD_START_EXEC_5078D0()
{
    s16 exec_flag;

    exec_flag = field_12;

    //  Only increase EXEC flag if it wasn't set before
    if (exec_flag <= 0)
    {
        field_12 = exec_flag + 1; //  increase EXEC flag
    }

    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x5079a0)
void miss2_0x11C::SCRCMD_STOP_EXEC_5079A0()
{
    s16 exec_flag;

    exec_flag = field_12;

    //  Only decrease EXEC flag if it was set before
    if (exec_flag >= 0)
    {
        field_12 = exec_flag - 1; //  decrease EXEC flag
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x507a70)
void miss2_0x11C::SCRCMD_INCREMENT_507A70()
{
    SCR_POINTER* pCmd;

    pCmd = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(
                        gBasePtr_6F8070[1].field_0_cmd_this);

    ++pCmd->field_8_counter; //  Increment Counter
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x507b50)
void miss2_0x11C::SCRCMD_DECREMENT_507B50()
{
    SCR_POINTER* pCmd;

    pCmd = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(
                        gBasePtr_6F8070[1].field_0_cmd_this);

    --pCmd->field_8_counter; //  Decrement Counter
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
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

MATCH_FUNC(0x509730)
void miss2_0x11C::SCR_CMD_CREATE_CHAR_509730()
{
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(
                                        gBasePtr_6F8070[1].field_0_cmd_this);
    miss2_0x11C::SCRCMD_CHAR_DECSET_2D_3D_503FB0((SCR_CHAR_DATA_DEC*)gBasePtr_6F8070, 
                                                    pPointer);
    if (!field_118)
    {
        gMiss2_25C_6F805C->sub_502FB0(pPointer->field_8_char);
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

STUB_FUNC(0x509790)
void miss2_0x11C::sub_509790()
{
}

MATCH_FUNC(0x5097d0)
void miss2_0x11C::sub_5097D0()
{
    SCR_POINTER* BasePointer_512770;
    
    BasePointer_512770 = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(
                                    gBasePtr_6F8070[1].field_0_cmd_this); //  TODO: fix gBasePtr_6F8070
    miss2_0x11C::sub_504110((SCR_CHAR_OBJECTIVE*)gBasePtr_6F8070, BasePointer_512770);
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
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

MATCH_FUNC(0x509bb0)
void miss2_0x11C::SCRCMD_HAS_CHAR_DIED_509BB0()
{
    SCR_POINTER* v2;

    v2 = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);

    if (v2->field_8_char 
        && v2->field_8_char->field_278 == 9)
    {
        field_8 = 1;
    }
    else
    {
        field_8 = 0;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
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

MATCH_FUNC(0x50a3e0)
void miss2_0x11C::sub_50A3E0()
{
    SCR_CHAR_OBJ3* v1;
    SCR_POINTER* pCmd;

    v1 = (SCR_CHAR_OBJ3*)gBasePtr_6F8070;
    pCmd = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    miss2_0x11C::sub_504110((SCR_CHAR_OBJECTIVE*)gBasePtr_6F8070, pCmd);

    if (pCmd->field_8_char)
    {
        (pCmd->field_8_char)->field_1DC_objective_target_x = v1->field_C_pos.field_0_x.mValue; //  TODO: May be Fix16 in cool_nash
        (pCmd->field_8_char)->field_1E0_objective_target_y = v1->field_C_pos.field_4_y.mValue;
        (pCmd->field_8_char)->field_1E4_objective_target_z = v1->field_C_pos.field_8_z.mValue;

        (pCmd->field_8_char)->field_21C &= ~0x400u; // TODO: Maybe BitSet32
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50a460)
void miss2_0x11C::sub_50A460()
{
    SCR_CHAR_OBJECTIVE* v1;
    SCR_POINTER* pCmd;

    v1 = (SCR_CHAR_OBJECTIVE*)gBasePtr_6F8070;
    pCmd = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(
                    gBasePtr_6F8070[1].field_0_cmd_this); //  TODO: fix gBasePtr_6F8070 type
    miss2_0x11C::sub_504110((SCR_CHAR_OBJECTIVE*)gBasePtr_6F8070, pCmd);

    SCR_POINTER* v4 = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(
                                v1->field_C_car_idx);

    if (pCmd->field_8_char)
    {
        (pCmd->field_8_char)->field_150_target_objective_car = v4->field_8_car;

        Ang16 CmdRotation;
        CmdRotation.rValue = v1->field_E_rotation;

        Fix16 fix_1;
        fix_1.mValue = CmdRotation.FromUnsignedToFloat();
        Fix16 fix_2;
        fix_2.mValue = word_6F8044.ToFloat();
        Fix16 fix_3;
        fix_3.mValue = fix_2.MultiplyInt64(fix_1);

        Ang16 rotation;
        rotation.rValue = fix_3.ToInt();
        rotation.Normalize();

        (pCmd->field_8_char)->field_132 = rotation.rValue;
        (pCmd->field_8_char)->field_1FC = v1->field_12_offset;
        (pCmd->field_8_char)->field_21C &= ~0x400u; // TODO: Maybe BitSet32
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50a570)
void miss2_0x11C::SCRCMD_CHANGE_CAR_REMAP_50A570()
{
    SCR_TWO_PARAMS* v1 = (SCR_TWO_PARAMS*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(
                                        gBasePtr_6F8070[1].field_0_cmd_this);
    if (v1->field_A_unsigned_2 != 0xFFu) //  field_A_unsigned_2 = remap
    {
        pPointer->field_8_car->sub_43A780(v1->field_A_unsigned_2);
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50a5b0)
void miss2_0x11C::SCRCMD_CHANGE_CHAR_REMAP_50A5B0()
{
    SCR_TWO_PARAMS* v1 = (SCR_TWO_PARAMS*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(
                                        gBasePtr_6F8070[1].field_0_cmd_this);

    if (pPointer->field_8_char)
    {
        Char_B4* pChar_b4 = pPointer->field_8_char->field_168_game_object;
        u8 remap = v1->field_A_unsigned_2;
        pChar_b4->field_5_remap = remap;
        if (pChar_b4->field_5_remap != 0xFFu)
        {
            pChar_b4->field_80_sprite_ptr->sub_59EA00(remap);
        }
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50a610)
void miss2_0x11C::SCRCMD_CHECK_CAR_MODEL_50A610()
{
    SCR_TWO_PARAMS* v1 = (SCR_TWO_PARAMS*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(
                                        gBasePtr_6F8070[1].field_0_cmd_this);

    Car_BC* pCar = pPointer->field_8_car;
    s16 car_model_index = v1->field_A_signed_2;

    if (pCar != NULL 
        && (s8)pCar->field_84_car_info_idx == car_model_index)
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50a670)
void miss2_0x11C::SCRCMD_CHECK_CAR_REMAP_50A670()
{
    SCR_TWO_PARAMS* v1 = (SCR_TWO_PARAMS*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(
                                        gBasePtr_6F8070[1].field_0_cmd_this);

    Car_BC* pCar = pPointer->field_8_car;
    s16 remap = v1->field_A_signed_2;

    if (pCar != NULL 
        && pCar->field_88 != 6 
        && (u16)pCar->field_50_car_sprite->field_24_remap == remap)
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50a6e0)
void miss2_0x11C::SCRCMD_CHECK_CAR_BOTH_50A6E0()
{
    SCR_FOUR_PARAMS* v1 = (SCR_FOUR_PARAMS*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(
                                        gBasePtr_6F8070[1].field_0_cmd_this);

    Car_BC* pCar = pPointer->field_8_car;
    s16 car_model_idx = v1->field_A_signed_2;
    s16 remap = v1->field_C_signed_3;

    if (pCar != NULL 
        && pCar->field_88 != 6 
        && (s8)pCar->field_84_car_info_idx == car_model_idx 
        && (u16)pCar->field_50_car_sprite->field_24_remap == remap)
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
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

MATCH_FUNC(0x50abc0)
void miss2_0x11C::SCRCMD_DISABLE_THREAD_50ABC0()
{
    miss2_0x11C::sub_505790(gBasePtr_6F8070[1].field_0_cmd_this);
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50abf0)
void miss2_0x11C::SCRCMD_ENABLE_THREAD_50ABF0()
{
    miss2_0x11C::sub_50A9E0(gBasePtr_6F8070[1].field_0_cmd_this);
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

STUB_FUNC(0x50ac20)
void miss2_0x11C::SCRCMD_SET_GANG_RESPECT_50AC20()
{
}

MATCH_FUNC(0x50acf0)
void miss2_0x11C::sub_50ACF0()
{
    SCR_FOUR_PARAMS* v1 = (SCR_FOUR_PARAMS*)gBasePtr_6F8070;

    str_table_entry* StringById_503080 = gfrosty_pasteur_6F8060->FindStringById_503080(
                                            gBasePtr_6F8070[1].field_0_cmd_this);

    char* gang_zone_name = (char*)&StringById_503080[1];
    Zone_144* v4 = gZones_CA8_67E274->zone_by_name_4BF100(gang_zone_name);
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(
                                        v1->field_C_unsigned_3);

    // v1->field_A_signed_2 = respect parameter

    switch (gBasePtr_6F8070->field_2_type)
    {
        case SCRCMD_SET_CHAR_RESPECT:

            v4->sub_4BEE30(pPointer->field_8_char->field_15C_player_weapons->field_2E_idx, 
                            20 * ((u8)v1->field_A_signed_2));
            v4->field_111 = 1;
            break;
        case SCRCMD_CHANGE_RESPECT:
            if (v1->field_A_signed_2 > 0)
            {
                v4->sub_4BEE50(pPointer->field_8_char->field_15C_player_weapons->field_2E_idx, 
                                20 * (v1->field_A_signed_2));
            }
            else
            {
                v4->sub_4BEEA0(pPointer->field_8_char->field_15C_player_weapons->field_2E_idx, 
                                20 * abs(v1->field_A_signed_2));
            }
            break;
        case SCRCMD_CHANGE_GANG_RESP:
            v4->sub_4BF000(pPointer->field_8_char->field_15C_player_weapons->field_2E_idx, 
                                20 * ((u8)v1->field_A_signed_2));
            break;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
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

MATCH_FUNC(0x50b910)
void miss2_0x11C::SCRCMD_IS_CHAR_FIRING_AREA_50B910()
{
    SCR_ONEVAR_RECT* v1 = (SCR_ONEVAR_RECT*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(
                                        gBasePtr_6F8070[1].field_0_cmd_this);

    cool_nash_0x294* pPed = pPointer->field_8_char;

    BitSet32 flag = pPointer->field_8_char->field_21C;

    if (flag.check_bit(11) 
        && Is_it_in_area((SCR_XYZ_f*)&pPed->field_1AC_cam, 
                        &v1->field_C_rect))
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
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

MATCH_FUNC(0x50be00)
void miss2_0x11C::SCRCMD_HAS_CAR_WEAPON_50BE00()
{
    SCR_TWO_PARAMS* v1 = (SCR_TWO_PARAMS*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(
                                        gBasePtr_6F8070[1].field_0_cmd_this);
    s16 weapon_idx = v1->field_A_signed_2;

    if (gWeapon_8_707018->sub_5E3D20(pPointer->field_8_car, 
                                        weapon_idx))
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50be70)
void miss2_0x11C::SCRCMD_IS_CHAR_HORN_50BE70()
{
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(
                                        gBasePtr_6F8070[1].field_0_cmd_this);
    cool_nash_0x294* pPed = pPointer->field_8_char;
    Car_BC* pCar = pPed->field_16C_car;

    if (pCar != NULL 
        && pCar->field_A7_horn > 0 
        && pCar->field_A7_horn <= 0xF8u)
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
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

MATCH_FUNC(0x50c230)
void miss2_0x11C::SCRCMD_ENABLE_DISABLE_CRANE_50C230()
{
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(
                                        gBasePtr_6F8070[1].field_0_cmd_this);
    s16 type = gBasePtr_6F8070->field_2_type;
    
    if (type == SCRCMD_ENABLE_CRANE)
    {
        pPointer->field_8_crane->field_148 = 0;
    }
    else if (type == SCRCMD_DISABLE_CRANE)
    {
        pPointer->field_8_crane->field_148 = 1;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c2a0)
void miss2_0x11C::SCRCMD_CAR_GOT_DRIVER_50C2A0()
{
    SCR_POINTER* pCmd = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);
    if (pCmd->field_8_car->field_54_driver)
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c2f0)
void miss2_0x11C::SCRCMD_SPOTTED_PLAYER_50C2F0()
{
    SCR_POINTER* pCmd = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(
                                    gBasePtr_6F8070[1].field_0_cmd_this);
    BitSet32 flag;
    flag.m_var = pCmd->field_8_char->field_21C;
    if (flag.check_bit(23))
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c350)
void miss2_0x11C::SCRCMD_GET_LAST_PUNCHED_50C350()
{
    SCR_TWO_PARAMS* v1 = (SCR_TWO_PARAMS*)gBasePtr_6F8070;
    SCR_POINTER* pCharTargetPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(
                                                gBasePtr_6F8070[1].field_0_cmd_this);
    SCR_POINTER* pCharPunchedPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(
                                                v1->field_A_unsigned_2);

    pCharPunchedPointer->field_8_char = pCharTargetPointer->field_8_char->field_188_last_char_punched;
    pCharTargetPointer->field_8_char->field_188_last_char_punched = 0; //  reset

    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c3b0)
void miss2_0x11C::SCRCMD_IS_CHAR_STUNNED_50C3B0()
{
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(
                                        gBasePtr_6F8070[1].field_0_cmd_this);
    cool_nash_0x294* pPed = pPointer->field_8_char;

    if (pPed->field_27C == 22 
        || pPed->field_216_health <= 25)
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c410)
void miss2_0x11C::SCRCMD_KILL_ALL_PASSENG_50C410()
{
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(
                                        gBasePtr_6F8070[1].field_0_cmd_this);
    pPointer->field_8_car->field_4.sub_4715A0();

    if (gSero_181C_6FF1D4->is_bus_579AA0(pPointer->field_8_car))
    {
        gSero_181C_6FF1D4->sub_579B20();
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

STUB_FUNC(0x50c470)
void miss2_0x11C::SCRCMD_IS_GROUP_IN_CAR_50C470()
{
}

MATCH_FUNC(0x50c4e0)
void miss2_0x11C::SCRCMD_PUNCHED_SOMEONE_50C4E0()
{
    SCR_POINTER* pCmd = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(
                                    gBasePtr_6F8070[1].field_0_cmd_this);
    if (pCmd->field_8_char->field_188_last_char_punched != NULL)
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c540)
void miss2_0x11C::SCRCMD_ADD_CHAR_TO_GANG_50C540()
{
    SCR_TWO_PARAMS* v1 = (SCR_TWO_PARAMS*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(
                                        gBasePtr_6F8070[1].field_0_cmd_this);
    str_table_entry* StringById_503080 = gfrosty_pasteur_6F8060->FindStringById_503080(
                                        v1->field_A_unsigned_2);

    char* gang_zone_name = (char*)&StringById_503080[1];

    pPointer->field_8_char->field_17C_pZone = gZones_CA8_67E274->zone_by_name_4BF100(gang_zone_name);

    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

STUB_FUNC(0x50c5a0)
void miss2_0x11C::sub_50C5A0()
{
}

MATCH_FUNC(0x50c6f0)
void miss2_0x11C::sub_50C6F0() // PARK and PARK_NO_RESPAWN
{
    SCR_TWO_PARAMS* v1 = (SCR_TWO_PARAMS*)gBasePtr_6F8070;
    SCR_POINTER* pParam1 = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(
                                        gBasePtr_6F8070[1].field_0_cmd_this);
    SCR_POINTER* pParam2 = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(
                                        v1->field_A_unsigned_2);

    gChickenLegend_48_6FD26C->sub_534700(pParam1->field_8_car, 
                                        pParam2->field_8_door);
    if (gBasePtr_6F8070->field_2_type == SCRCMD_PARK_NO_RESPAWN)
    {
        gChickenLegend_48_6FD26C->field_3F = 1;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c760)
void miss2_0x11C::SCRCMD_BEEN_PUNCHED_BY_50C760()
{
    SCR_TWO_PARAMS* v1 = (SCR_TWO_PARAMS*)gBasePtr_6F8070;
    SCR_POINTER* pPointerVictim = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(
                                            gBasePtr_6F8070[1].field_0_cmd_this);
    SCR_POINTER* pPointerAttacker = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(
                                            v1->field_A_unsigned_2);

    if (pPointerAttacker->field_8_char->field_188_last_char_punched == pPointerVictim->field_8_char)
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

STUB_FUNC(0x50c7d0)
void miss2_0x11C::SCRCMD_UPDATE_DOOR_50C7D0()
{
}

STUB_FUNC(0x50c8a0)
void miss2_0x11C::SCRCMD_DOOR_50C8A0()
{
}

MATCH_FUNC(0x50c990)
void miss2_0x11C::SCRCMD_REMOVE_WEAPON_50C990()
{
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(
                                        gBasePtr_6F8070[1].field_0_cmd_this);

    cool_nash_0x294* pPed = pPointer->field_8_char;
    angry_lewin_0x85C* player_weapons = pPed->field_15C_player_weapons;

    if (player_weapons != NULL)
    {
        player_weapons->sub_564C50();
    }
    else
    {
        pPed->sub_462510();
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50c9f0)
void miss2_0x11C::SCRCMD_REMOVE_BLOCK_50C9F0()
{
    SCR_REMOVE_BLOCK* pCmd = (SCR_REMOVE_BLOCK*)gBasePtr_6F8070;

    gMap_0x370_6F6268->sub_4E8940(pCmd->field_8_pos.field_0_x,
                                  pCmd->field_8_pos.field_1_y,
                                  pCmd->field_8_pos.field_2_z,
                                  pCmd->field_B_do_drop);
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50ca30)
void miss2_0x11C::SCRCMD_LOWER_LEVEL_50CA30()
{
    SCR_LOWER_LEVEL* pCmd = (SCR_LOWER_LEVEL*)gBasePtr_6F8070;

    gMap_0x370_6F6268->sub_4E8B70(pCmd->field_8_min_pos.field_0_x,
                                  pCmd->field_A_max_pos.field_0_x,
                                  pCmd->field_8_min_pos.field_1_y,
                                  pCmd->field_A_max_pos.field_1_y);
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
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

MATCH_FUNC(0x50cdb0)
void miss2_0x11C::SCRCMD_CAR_DAMAGE_POS_50CDB0()
{
    SCR_TWO_PARAMS* v1;
    SCR_POINTER* pCmd;

    v1 = (SCR_TWO_PARAMS*)gBasePtr_6F8070;
    pCmd = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(gBasePtr_6F8070[1].field_0_cmd_this);

    if ((pCmd->field_8_car)->sub_43D1C0(v1->field_A_signed_2)) // field_A_signed_2 = damage pos
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50ce10)
void miss2_0x11C::SCRCMD_PARK_FINISHED_50CE10()
{
    if ( gChickenLegend_48_6FD26C->field_C == 3 )
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
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

MATCH_FUNC(0x50d900)
void miss2_0x11C::SCRCMD_CHANGE_COLOUR_50D900()
{
    SCR_FOUR_PARAMS* v1 = (SCR_FOUR_PARAMS*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(
                                        gBasePtr_6F8070[1].field_0_cmd_this);

    u32 rgb = v1->field_C_u32;
    pPointer->field_8_light->field_10_argb = rgb;
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
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

MATCH_FUNC(0x50de50)
void miss2_0x11C::SCRCMD_CHAR_IN_AIR_50DE50()
{
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(
                                        gBasePtr_6F8070[1].field_0_cmd_this);
    cool_nash_0x294* pPed = pPointer->field_8_char;

    if (pPed->field_27C == 19 
        && pPed->field_278 == 8)
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x50deb0)
void miss2_0x11C::SCRCMD_CHAR_SUNK_50DEB0()
{
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(
                                        gBasePtr_6F8070[1].field_0_cmd_this);
    cool_nash_0x294* pPed = pPointer->field_8_char;

    if (pPed->field_27C == 20 
        && pPed->field_278 == 8)
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
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

MATCH_FUNC(0x50e4a0)
void miss2_0x11C::SCRCMD_CHAR_ARRESTED_50E4A0()
{
    SCR_POINTER* pCmd = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(
                                gBasePtr_6F8070[1].field_0_cmd_this);
    if ((pCmd->field_8_char->field_21C & 0x20) != 0)
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
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

MATCH_FUNC(0x50f220)
void miss2_0x11C::SCRCMD_SUPPRESS_MODEL_50F220()
{
    if ((u16)gBasePtr_6F8070[1].field_0_cmd_this == 0xFFFFu) //  TODO: fix gBasePtr_6F8070
    {
        gCar_6C_677930->field_C = 87; //  No model suppressed
    }
    else
    {
        gCar_6C_677930->field_C = (u16)gBasePtr_6F8070[1].field_0_cmd_this;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
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

MATCH_FUNC(0x50f410)
void miss2_0x11C::SCRCMD_CHAR_DO_NOTHING_50F410()
{
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(
                                        gBasePtr_6F8070[1].field_0_cmd_this);
    cool_nash_0x294* pPed = pPointer->field_8_char;

    if (pPed != NULL)
    {
        pPed->sub_462590();
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

STUB_FUNC(0x50f450)
void miss2_0x11C::SCRCMD_EMERG_LIGHTS_50F450()
{
}

MATCH_FUNC(0x50f4d0)
void miss2_0x11C::SCRCMD_CHECK_OBJ_MODEL_50F4D0()
{
    SCR_FOUR_PARAMS* v1 = (SCR_FOUR_PARAMS*)gBasePtr_6F8070;
    SCR_POINTER* pPointer = (SCR_POINTER*)gfrosty_pasteur_6F8060->GetBasePointer_512770(
                                        gBasePtr_6F8070[1].field_0_cmd_this);

    Object_2C* pObj = pPointer->field_8_obj;
    u32 model_idx = v1->field_C_u32;

    if (pObj != NULL
        && pObj->field_18_model == model_idx)
    {
        field_8 = true;
    }
    else
    {
        field_8 = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
}

STUB_FUNC(0x50f550)
void miss2_0x11C::SCRCMD_PED_GRAPHIC_50F550()
{
}

STUB_FUNC(0x50f5e0)
void miss2_0x11C::sub_50F5E0()
{
}

MATCH_FUNC(0x50f770)
void miss2_0x11C::SCRCMD_MAKE_MUGGERS_50F770()
{
    SCR_TWO_PARAMS* pCmd = (SCR_TWO_PARAMS*)gBasePtr_6F8070;
    if ((u8)pCmd->field_A_unsigned_2 == 1)
    {
        gChar_C_6787BC->field_7_make_all_muggers = true;
    }
    else
    {
        gChar_C_6787BC->field_7_make_all_muggers = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
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

MATCH_FUNC(0x50f9b0)
void miss2_0x11C::SCRCMD_NO_CHARS_OFF_BUS_50F9B0()
{
    SCR_TWO_PARAMS* pCmd = (SCR_TWO_PARAMS*)gBasePtr_6F8070;
    if ((u8)pCmd->field_A_unsigned_2 == 1)
    {
        gSero_181C_6FF1D4->field_1818_stop_getting_off_bus = true;
    }
    else
    {
        gSero_181C_6FF1D4->field_1818_stop_getting_off_bus = false;
    }
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
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

MATCH_FUNC(0x50fad0)
void miss2_0x11C::SCRCMD_FINISH_MISSION_50FAD0()
{
    gMiss2_25C_6F805C->sub_502DC0();
    miss2_0x11C::Next_503620(gBasePtr_6F8070);
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
    if (field_10 != 1)
    {
        SCR_CMD_HEADER* pCmd = gfrosty_pasteur_6F8060->GetBasePointer_512770(field_4_level_start);
        gBasePtr_6F8070 = pCmd;
        switch (pCmd->field_2_type)
        {
            case SCRCMD_CREATE_CHAR_2D:
            case SCRCMD_CREATE_CHAR_3D:
                SCR_CMD_CREATE_CHAR_509730();
                break;
            case SCRCMD_CREATE_CAR_2D:
            case SCRCMD_CREATE_CAR_3D:
            case SCRCMD_CREATE_CAR_2D_STR:
            case SCRCMD_CREATE_CAR_3D_STR:
            case SCRCMD_CREATE_GANG_CAR1:
            case SCRCMD_CREATE_GANG_CAR2:
            case SCRCMD_CREATE_GANG_CAR3:
            case SCRCMD_CREATE_GANG_CAR4:
                sub_507F80();
                break;
            case SCRCMD_CREATE_OBJ_2D:
            case SCRCMD_CREATE_OBJ_3D:
            case SCRCMD_CREATE_OBJ_3D_s32:
            case SCRCMD_CREATE_OBJ_2D_s32:
            case SCRCMD_CREATE_OBJ_3D_STR:
            case SCRCMD_CREATE_OBJ_2D_STR:
                sub_507CE0();
                break;
            case SCRCMD_LEVELSTART:
                SCRCMD_LEVELSTART_5069C0();
                break;
            case SCRCMD_LEVELEND:
                SCRCMD_LEVELEND_5069F0();
                break;
            case 61:
                SCRCMD_CREATE_THREAD_506A00();
                break;
            case 62:
                SCRCMD_STOP_THREAD_506A60();
                break;
            case 63:
                SCRCMD_START_EXEC_5078D0();
                break;
            case 64:
                SCRCMD_STOP_EXEC_5079A0();
                break;
            case SCRCMD_RETURN:
            case SCRCMD_MISSIONEND:
                sub_506B80();
                break;
            case SCRCMD_NOT:
                SCRCMD_NOT_507750();
                break;
            case SCRCMD_GOSUB:
                sub_506B30();
                break;
            case SCRCMD_S_PLUS_I:
            case SCRCMD_S_MINUS_I:
            case SCRCMD_S_LESS_I:
            case SCRCMD_S_LEQUAL_I:
            case SCRCMD_S_GREATER_I:
            case SCRCMD_S_GEQUAL_I:
            case SCRCMD_S_EQUAL_I:
                sub_506D60();
                break;
            case SCRCMD_I_PLUS_S:
            case SCRCMD_I_MINUS_S:
                sub_507110();
                break;
            case SCRCMD_S_PLUS_S:
            case SCRCMD_S_MINUS_S:
            case SCRCMD_S_LESS_S:
            case SCRCMD_S_LEQUAL_S:
            case SCRCMD_S_GREATER_S:
            case SCRCMD_S_GEQUAL_S:
            case SCRCMD_S_EQUAL_S:
                sub_506ED0();
                break;
            case SCRCMD_INCREMENT:
                SCRCMD_INCREMENT_507A70();
                break;
            case SCRCMD_DECREMENT:
                SCRCMD_DECREMENT_507B50();
                break;
            case SCRCMD_IF_JUMP:
                SCRCMD_IF_JUMP_506AF0(); // IF_JUMP?
                break;
            case SCRCMD_MAKE_CAR_DUMMY:
                SCRCMD_MAKE_CAR_DUMMY_508220();
                break;
            case SCRCMD_SET_CAR_DENSITY:
            case SCRCMD_SET_GOOD_CAR:
            case SCRCMD_SET_BAD_CAR:
            case SCRCMD_SET_POLICE_CAR:
            case SCRCMD_SET_PED_DENSITY:
            case SCRCMD_SET_MUGGER:
            case SCRCMD_SET_CARTHIEF:
            case SCRCMD_SET_ELVIS:
            case SCRCMD_SET_GANG:
            case SCRCMD_SET_POLICE_PED:
            case SCRCMD_SET_GANGCARRATIO:
                sub_508280();
                break;
            case SCRCMD_POINT_ARROW_AT:
            case SCRCMD_LEVEL_END_ARROW1:
                sub_5086F0();
                break;
            case SCRCMD_POINT_ARROW_3D:
            case SCRCMD_LEVEL_END_ARROW2:
                sub_508550();
                break;
            case SCRCMD_ARROW_COLOUR:
                SCRCMD_ARROW_COLOUR_508DC0();
                break;
            case SCRCMD_REMOVE_ARROW:
                SCRCMD_REMOVE_ARROW_508E80();
                break;
            case SCRCMD_DISPLAY_MESSAGE:
            case SCRCMD_DISPLAY_BRIEF:
            case SCRCMD_DISPLAY_BRIEF_NOW:
            case SCRCMD_DISPLAY_BRIEF_SOON:
                sub_5093C0();
                break;
            case SCRCMD_DISPLAY_TIMER:
                SCRCMD_DISPLAY_TIMER_5096E0();
                break;
            case SCRCMD_CLEAR_TIMERS:
                sub_50B690();
                break;
            case SCRCMD_IS_CHAR_IN_CAR:
                sub_509C10();
                break;
            case SCRCMD_IS_CHAR_IN_MODEL:
                sub_509C90();
                break;
            case SCRCMD_IS_CHAR_IN_ANY_CAR:
                sub_509D00();
                break;
            case SCRCMD_IS_CHAR_STOPPED:
                sub_509E70();
                break;
            case 125:
                SCRCMD_IS_CHAR_STUNNED_50C3B0();
                break;
            case 126:
                SCRCMD_CHECK_HEALTH_509030();
                break;
            case 127:
                SCRCMD_HAS_CHAR_DIED_509BB0();
                break;
            case 128:
                SCRCMD_STORE_CAR_INFO_509180();
                break;
            case 129:
                SCRCMD_CHECK_CAR_DAMAGE_508F00();
                break;
            case SCRCMD_CHECK_CAR_DRIVER:
            case SCRCMD_IS_CAR_WRECKED:
            case SCRCMD_CAR_SUNK:
            case SCRCMD_CAR_IN_AIR:
            case SCRCMD_IS_CAR_CRUSHED:
            case SCRCMD_CARBOMB_ACTIVE:
            case SCRCMD_EMERG_LIGHTS_ON:
                sub_50CB70();
                break;
            case SCRCMD_SET_CHAR_OBJ1:
                sub_5097D0();
                break;
            case SCRCMD_SET_CHAR_OBJ2:
                sub_50A200();
                break;
            case SCRCMD_SET_CHAR_OBJ3:
                sub_50A3E0();
                break;
            case SCRCMD_IS_CHAR_OBJ_PASS:
                sub_509810();
                break;
            case SCRCMD_IS_CHAR_OBJ_FAIL:
                sub_509880();
                break;
            case SCRCMD_GIVE_WEAPON1:
            case SCRCMD_GIVE_WEAPON2:
                sub_5098E0();
                break;
            case SCRCMD_IS_CAR_IN_BLOCK:
                sub_509990();
                break;
            case SCRCMD_DELETE_ITEM:
                sub_509D60();
                break;
            case SCRCMD_ADD_SCORE1:
            case SCRCMD_ADDSCORE_NO_MULT:
                sub_509D90();
                break;
            case SCRCMD_EXPLODE:
            case SCRCMD_EXPLODE_LARGE2:
            case SCRCMD_EXPLODE_SMALL2:
            case SCRCMD_EXPLODE_NO_RING2:
                sub_509ED0();
                break;
            case SCRCMD_EXPLODE_BUILDING:
                SCRCMD_EXPLODE_BUILDING_509F60();
                break;
            case SCRCMD_EXPLODE_ITEM:
            case SCRCMD_EXPLODE_LARGE1:
            case SCRCMD_EXPLODE_SMALL1:
            case SCRCMD_EXPLODE_NO_RING1:
                sub_50C5A0();
                break;
            case SCRCMD_LOCATE_CHAR_ANY:
            case SCRCMD_LOCATE_CHAR_ONFOOT:
            case SCRCMD_LOCATE_CHAR_BY_CAR:
            case SCRCMD_STOP_LOCATE_CHAR_ANY:
            case SCRCMD_STOP_LOCATE_CHAR_FOOT:
            case SCRCMD_STOP_LOCATE_CHAR_CAR:
                sub_509FD0();
                break;
            case SCRCMD_SET_THREAT_SEARCH:
            case SCRCMD_SET_THREAT_REACT:
            case SCRCMD_ADD_GROUP:
            case SCRCMD_SET_CHAR_SHOOT:
            case SCRCMD_SET_CHAR_BRAVERY:
                sub_50BBD0();
                break;
            case 155:
                SCRCMD_CHANGE_CAR_REMAP_50A570();
                break;
            case 156:
                SCRCMD_CHANGE_CHAR_REMAP_50A5B0();
                break;
            case 157:
                SCRCMD_CHECK_CAR_MODEL_50A610();
                break;
            case 158:
                SCRCMD_CHECK_CAR_REMAP_50A670();
                break;
            case 159:
                SCRCMD_CHECK_CAR_BOTH_50A6E0();
                break;
            case SCRCMD_IS_ITEM_ONSCREEN:
            case SCRCMD_ONSCREEN_ACCURACY:
                sub_50A760();
                break;
            case 161:
                SCRCMD_DELAY_HERE_50A940();
                break;
            case SCRCMD_DELAY:
                sub_50A980();
                break;
            case SCRCMD_CLEAR_WANTED_LEVEL:
                sub_50BA30();
                break;
            case SCRCMD_ALT_WANTED_LEVEL:
            case SCRCMD_ALTER_WANTED_LEVEL:
                sub_50BA70();
                break;
            case SCRCMD_IS_CHAR_FIRE_ONSCREEN:
                sub_50B3D0();
                break;
            case SCRCMD_DRIVER_OUT_CAR:
                sub_50B5A0();
                break;
            case SCRCMD_CHAR_TO_DRIVE_CAR:
            case SCRCMD_CHAR_TO_BACKDOOR:
                sub_50B4F0();
                break;
            case SCRCMD_ANSWER_PHONE:
                sub_50B180();
                break;
            case SCRCMD_SEND_CAR_TO_BLOCK:
                sub_50BB80();
                break;
            case SCRCMD_GIVE_DRIVER_BRAKE:
                sub_50B600();
                break;
            case SCRCMD_SET_NO_COLLIDE:
                sub_50B8B0();
                break;
            case 175:
                SCRCMD_CAR_DRIVE_AWAY_50B440();
                break;
            case 176:
                SCRCMD_IS_CHAR_FIRING_AREA_50B910();
                break;
            case 177:
            case 178:
            case 179:
            case 180:
                SCRCMD_DOOR_50C8A0();
                break;
            case 181:
                SCRCMD_ROAD_ON_OFF_50CB20();
                break;
            case 182:
                SCRCMD_ADD_NEW_BLOCK_50CAB0();
                break;
            case 183:
                SCRCMD_REMOVE_BLOCK_50C9F0();
                break;
            case 184:
                SCRCMD_LOWER_LEVEL_50CA30();
                break;
            case SCRCMD_CHANGE_BLOCK_SIDE:
            case SCRCMD_CHANGE_BLOCK_LID:
            case SCRCMD_CHANGE_BLOCK_TYPE:
                sub_50CA70();
                break;
            case 188:
                SCRCMD_GET_CAR_FROM_CRANE_50B470();
                break;
            case 189:
                SCRCMD_CAR_WRECK_IN_LOCATION_50BAD0();
                break;
            case SCRCMD_CHECK_NUM_LIVES:
            case SCRCMD_CHECK_MULT:
            case SCRCMD_ADD_LIVES:
            case SCRCMD_ADD_MULTIPLIER:
                sub_50DB70();
                break;
            case SCRCMD_GET_NUM_LIVES:
            case SCRCMD_GET_MULT:
                sub_50DD00();
                break;
            case 194:
                SCRCMD_CHECK_SCORE_50B6F0();
                break;
            case 195:
                SCRCMD_GET_SCORE_50B760();
                break;
            case SCRCMD_CHECK_RESPECT_GREATER:
            case SCRCMD_CHECK_RESPECT_LESS:
            case SCRCMD_CHECK_RESPECT_IS:
                sub_50AEF0();
                break;
            case 200:
                SCRCMD_CAR_DAMAGE_POS_50CDB0();
                break;
            case 201:
                SCRCMD_GET_PASSENGER_NUM_50B9C0();
                break;
            case 202:
                SCRCMD_CHAR_IN_AIR_50DE50();
                break;
            case 203:
                SCRCMD_CHAR_SUNK_50DEB0();
                break;
            case SCRCMD_ADD_PATROL_POs32:
                sub_50B0E0();
                break;
            case SCRCMD_GET_CAR_SPEED:
            case SCRCMD_GET_CHAR_CAR_SPEED:
            case SCRCMD_GET_MAX_SPEED:
                sub_50E190();
                break;
            case 208:
                SCRCMD_CHECK_CAR_SPEED_50E360();
                break;
            case 215:
                SCRCMD_ENABLE_THREAD_50ABF0();
                break;
            case 216:
                SCRCMD_DISABLE_THREAD_50ABC0();
                break;
            case SCRCMD_CREATE_LIGHT1:
            case SCRCMD_LIGHT_DECSET2:
                sub_50DA50();
                break;
            case 220:
                SCRCMD_CHANGE_INTENSITY_50D870();
                break;
            case 221:
                SCRCMD_CHANGE_COLOUR_50D900();
                break;
            case 222:
                SCRCMD_CHANGE_RADIUS_50D9A0();
                break;
            case 223:
                SCRCMD_SET_GANG_INFO1_50B670();
                break;
            case 224:
                SCRCMD_SET_GANG_RESPECT_50AC20();
                break;
            case SCRCMD_SET_CHAR_RESPECT:
            case SCRCMD_CHANGE_RESPECT:
            case SCRCMD_CHANGE_GANG_RESP:
                sub_50ACF0();
                break;
            case 226:
                SCRCMD_SET_AMBIENT_50B150();
                break;
            case 227:
                SCRCMD_CHECK_PHONE_50B230();
                break;
            case 228:
                SCRCMD_CHECK_PHONETIMER_50B2C0();
                break;
            case 229:
                SCRCMD_STOP_PHONE_RING_50B360();
                break;
            case 232:
                SCRCMD_IS_CHAR_IN_GANG_50B7D0();
                break;
            case 234:
                SCRCMD_CHECK_NUM_ALIVE_50BC60();
                break;
            case SCRCMD_ADD_CHAR_TO_GROUP:
            case SCRCMD_REMOVE_char_type:
            case SCRCMD_MAKE_LEADER:
                sub_50BD10();
                break;
            case 237:
                SCRCMD_SET_MIN_ALIVE_50BCD0();
                break;
            case 241:
                SCRCMD_HAS_CAR_WEAPON_50BE00();
                break;
            case 242:
                SCRCMD_IS_CHAR_IN_ZONE_50BF40();
                break;
            case 243:
                SCRCMD_IS_CHAR_HORN_50BE70();
                break;
            case 244:
                SCRCMD_CHECK_MAX_PASS_50BED0();
                break;
            case 245:
                SCRCMD_SET_PHONE_DEAD_50C040();
                break;
            case 246:
                SCRCMD_IS_TRAILER_ATT_50C0E0();
                break;
            case 247:
                SCRCMD_IS_CAR_ON_TRAIL_50C1B0();
                break;
            case 248:
            case 249:
                SCRCMD_ENABLE_DISABLE_CRANE_50C230();
                break;
            case 250:
                SCRCMD_CAR_GOT_DRIVER_50C2A0();
                break;
            case 251:
                SCRCMD_SPOTTED_PLAYER_50C2F0();
                break;
            case 252:
                SCRCMD_GET_LAST_PUNCHED_50C350();
                break;
            case 253:
                SCRCMD_KILL_ALL_PASSENG_50C410();
                break;
            case 254:
                SCRCMD_IS_GROUP_IN_CAR_50C470();
                break;
            case 255:
                SCRCMD_PUNCHED_SOMEONE_50C4E0();
                break;
            case 256:
                SCRCMD_REMOVE_WEAPON_50C990();
                break;
            case 258:
                SCRCMD_ADD_CHAR_TO_GANG_50C540();
                break;
            case SCRCMD_PARK:
            case SCRCMD_PARK_NO_RESPAWN:
                sub_50C6F0();
                break;
            case 261:
                SCRCMD_PARK_FINISHED_50CE10();
                break;
            case 263:
                SCRCMD_PHONE_TEMPLATE_50CE90();
                break;
            case 264:
                SCRCMD_BEEN_PUNCHED_BY_50C760();
                break;
            case 265:
                SCRCMD_UPDATE_DOOR_50C7D0();
                break;
            case SCRCMD_SWITCH_GENERATOR1:
            case SCRCMD_SWITCH_GENERATOR2:
                sub_50CCB0();
                break;
            case 270:
                SCRCMD_CAR_IN_AREA_509A70();
                break;
            case 271:
                SCRCMD_REMOTE_CONTROL_50D200();
                break;
            case 274:
                SCRCMD_LAUNCH_MISSION_50D2E0();
                break;
            case 277:
                SCRCMD_SAVE_GAME_50D340();
                break;
            case 278:
                SCRCMD_CHANGE_CAR_LOCK_50D680();
                break;
            case 283:
                SCRCMD_SET_DIR_OF_TVVAN_50DD90();
                break;
            case 284:
                SCRCMD_POINT_ONSCREEN_50DE00();
                break;
            case SCRCMD_START_BONUS1:
            case SCRCMD_START_BONUS2:
            case SCRCMD_START_BONUS3:
            case SCRCMD_START_BONUS4:
                sub_50DF10();
                break;
            case SCRCMD_CHECK_BONUS1:
            case SCRCMD_CHECK_BONUS2:
            case SCRCMD_CHECK_BONUS3:
                sub_50E0B0();
                break;
            case SCRCMD_SETUP_MODEL_CHECK:
                sub_50E120();
                break;
            case SCRCMD_MODEL_CHECK:
                sub_50E150();
                break;
            case SCRCMD_SET_CAR_GRAPHIC:
                sub_50E460();
                break;
            case SCRCMD_CHAR_DRIVE_AGGR:
                sub_50E730();
                break;
            case SCRCMD_CHAR_DRIVE_SPEED:
            case SCRCMD_SET_RUN_SPEED:
                sub_50E780();
                break;
            case SCRCMD_S_IS_S_MINUS_S:
            case SCRCMD_S_IS_S_PLUS_S:
            case SCRCMD_S_IS_S_DIV_S:
            case SCRCMD_S_IS_S_MULT_S:
            case SCRCMD_S_IS_S_MOD_S:
                sub_50E4F0();
                break;
            case SCRCMD_GIVE_CAR_ALARM:
                sub_50E7F0();
                break;
            case SCRCMD_CAR_BULLETPROOF:
            case SCRCMD_CAR_ROCKETPROOF:
            case SCRCMD_CAR_FLAMEPROOF:
                sub_50E820();
                break;
            case SCRCMD_SET_CHAR_OBJ_FOLLOW:
                sub_50A460();
                break;
            case SCRCMD_PUT_CAR_ON_TRAILER:
                sub_50E900();
                break;
            case SCRCMD_CLEAR_BRIEFS:
                sub_50E9A0();
                break;
            case SCRCMD_CHECK_HEADS:
                sub_50E9E0();
                break;
            case SCRCMD_FINISH_LEVEL:
                sub_50EA40();
                break;
            case SCRCMD_CHECK_WEAPONHIT:
                sub_50EB00();
                break;
            case SCRCMD_IS_CHAR_ON_FIRE:
                sub_50ECE0();
                break;
            case SCRCMD_BRIEF_ONSCREEN:
                sub_50ED40();
                break;
            case SCRCMD_CREATE_SOUND:
                sub_50ED80();
                break;
            case SCRCMD_DO_EASY_PHONE:
                sub_50EDC0();
                break;
            case SCRCMD_CHAR_INTO_CAR:
                SCRCMD_CHAR_INTO_CAR_50F060();
                break;
            case SCRCMD_S_IS_S_MINUS_I:
            case SCRCMD_S_IS_S_PLUS_I:
            case SCRCMD_S_IS_S_DIV_I:
            case SCRCMD_S_IS_S_MULT_I:
            case SCRCMD_S_IS_S_MOD_I:
                sub_50E610();
                break;
            case SCRCMD_SET_COUNTER_s32:
                sub_50BDC0();
                break;
            case 342:
                SCRCMD_ADD_SCORE2_509E00();
                break;
            case 350:
                SCRCMD_SUPPRESS_MODEL_50F220();
                break;
            case SCRCMD_SWITCH_GENERATOR3:
            case SCRCMD_SWITCH_GENERATOR4:
                sub_50CD30();
                break;
            case 354:
                SCRCMD_DECIDE_POWERUP_50F150();
                break;
            case 355:
                SCRCMD_CHAR_ARRESTED_50E4A0();
                break;
            case SCRCMD_WARP_char_type:
                sub_50F270();
                break;
            case SCRCMD_WEAP_HIT_CAR:
            case SCRCMD_ANY_WEAPON_HIT_CAR:
                sub_50EBD0();
                break;
            case 359:
                SCRCMD_SET_GROUP_TYPE_50F3D0();
                break;
            case 360:
                SCRCMD_CHAR_DO_NOTHING_50F410();
                break;
            case 361:
                SCRCMD_EMERG_LIGHTS_50F450();
                break;
            case 362:
                SCRCMD_CHECK_OBJ_MODEL_50F4D0();
                break;
            case SCRCMD_CHAR_INVINCIBLE:
            case SCRCMD_SET_STAY_IN_CAR:
            case SCRCMD_SET_USE_CAR_WEAPON:
            case SCRCMD_DEATH_ARR_STATE:
                sub_50F5E0();
                break;
            case 364:
                SCRCMD_PED_GRAPHIC_50F550();
                break;
            case 365:
                SCRCMD_MAKE_MUGGERS_50F770();
                break;
            case SCRCMD_LOC_SECOND_char_type:
            case SCRCMD_LOC_SEC_CHAR_CAR:
            case SCRCMD_LOC_SEC_CHAR_ANY:
                sub_50F7B0();
                break;
            case 368:
                SCRCMD_STOP_CAR_DRIVE_50F900();
                break;
            case 369:
                SCRCMD_IS_BUS_FULL_50F940();
                break;
            case 370:
                SCRCMD_NO_CHARS_OFF_BUS_50F9B0();
                break;
            case SCRCMD_KILL_char_type:
                sub_50FA00();
                break;
            case 373:
                SCRCMD_SET_SHADING_LEV_50FA40();
                break;
            case 374:
                SCRCMD_SET_CAR_JAMMED_50FA70();
                break;
            case 391:
                SCRCMD_FINISH_MISSION_50FAD0();
                break;
            case 400:
            case 407:
                sub_50FB60();
                break;
            case 402:
                sub_50FAF0();
                break;
            case 408:
                sub_50FC20();
                break;
            case 409:
                sub_50FC60();
                break;
            case 410:
                sub_509790();
                break;
            case 411:
                sub_50FE00();
                break;
            case 412:
                sub_50FED0();
                break;
            case 414:
                sub_50FF50();
                break;
            case 415:
            case 416:
                sub_50FFB0();
                break;
            case 418:
                sub_510030();
                break;
            case 419:
                sub_510050();
                break;
            case 420:
                sub_510090();
                break;
            case 422:
                sub_510100();
                break;
            case 423:
                sub_510280();
                break;
            case 424:
                sub_510530();
                break;
            case 433:
                sub_510560();
                break;
            case 438:
                sub_50D3C0();
                break;
            case 442:
                sub_5105B0();
                break;
            case 443:
                sub_510600();
                break;
            case 444:
                sub_510660();
                break;
            case 445:
            case 446:
                sub_510780();
                break;
            default:
                Next_503620(pCmd);
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

    if (field_10 == 1)
    {
        return true;
    }

    BasePointer_512770 = gfrosty_pasteur_6F8060->GetBasePointer_512770(field_4_level_start);

    if (!BasePointer_512770)
    {
        sprintf(gTmpBuffer_67C598, "Miss2: accessing nonexistant mission line. Current uid: %d", field_4_level_start);
    }

    miss2_0x11C::sub_503200();

    if (BasePointer_512770->field_2_type == 0x3F) //  3F 00 = EXEC opcode
    {
        do
        {
            miss2_0x11C::PreExecOpCode_5108D0();
            gfrosty_pasteur_6F8060->GetBasePointer_512770(field_4_level_start);
        } while (field_12 > 0); //  execute opcodes in the same frame until an ENDEXEC
    }
    else //  It isn't an EXEC opcode
    {
        // field_6_return_value == 1  means it's in a WHILE_EXEC block
        if (BasePointer_512770->field_6_return_value == 1)
        {
            while (1) // execute commands in the same frame
            {
                v5 = gfrosty_pasteur_6F8060->GetBasePointer_512770(field_4_level_start);
                miss2_0x11C::PreExecOpCode_5108D0();

                //  If the script has reached a command out of the WHILE_EXEC block, or if it reaches LEVELEND
                if (v5->field_6_return_value != 1 || field_4_level_start == 0xFFFF)
                {
                    break;
                }

                //  Maybe if it reaches an ENDEXEC?
                if (field_12)
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

    v4 = field_4_level_start == 0xFFFF; // Did it reached the LEVELEND?
    field_12 = 0; // Clear EXEC flag?
    return v4;
}

MATCH_FUNC(0x511930)
void miss2_0x11C::sub_511930(char_type a2, u16 levelStart)
{
    field_4_level_start = levelStart;
    field_6 = a2;
    field_E = 0;
    field_C = 0;
    field_12 = 0;
    field_8 = false;
    field_10 = 0;
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
    field_0 = 0;
    field_114 = new miss2_8();
    field_4_level_start = 0;
    field_6 = 0;
    field_8 = false;
    field_C = 0;
    field_E = 0;
    field_10 = 0;
    field_12 = 0;
    field_11A = 0;

    memset(field_14_str, 0, sizeof(field_14_str));

    field_118 = 0;
}

STUB_FUNC(0x511cd0)
void miss2_0x11C::sub_511CD0()
{
}

STUB_FUNC(0x512fd0)
miss2_0x11C::~miss2_0x11C()
{
}
