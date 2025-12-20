#include "frosty_pasteur_0xC1EA8.hpp"
#include "Car_BC.hpp"
#include "Function.hpp"
#include "Game_0x40.hpp"
#include "Globals.hpp"
#include "Miss2_25C.hpp"
#include "Object_5C.hpp"
#include "debug.hpp"
#include "enums.hpp"
#include "error.hpp"
#include "file.hpp"
#include "lucid_hamilton.hpp"
#include "map_0x370.hpp"
#include "memory.hpp"
#include "Player.hpp"

DEFINE_GLOBAL(frosty_pasteur_0xC1EA8*, gfrosty_pasteur_6F8060, 0x6F8060);
DEFINE_GLOBAL(SaveData_748, gGameSave_6F78C8, 0x6F78C8);
DEFINE_GLOBAL(Car_BC*, gStoredCar_6F7560, 0x6F7560);
DEFINE_GLOBAL(s32, gStoredCarId_6F78B4, 0x6F78B4);
DEFINE_GLOBAL(u8, byte_6212EC, 0x6212EC);

//Car_BC* gStoredCar_6F7560;
//s32 gStoredCarId_6F78B4;

MATCH_FUNC(0x503080)
str_table_entry* frosty_pasteur_0xC1EA8::FindStringById_503080(s16 stringId)
{
    u16 idx = 0;
    str_table_entry* result = field_13350_pStringTbl->field_4[idx];
    while (result)
    {
        if (result->field_0_str_id == stringId)
        {
            return result;
        }
        idx++;
        result = field_13350_pStringTbl->field_4[idx];
    }
    return 0;
}

MATCH_FUNC(0x5030b0)
str_table_entry* frosty_pasteur_0xC1EA8::StrEntryByString_5030B0(char_type* strToFind)
{
    u16 idx = 0;
    str_table_entry* pEntry = field_13350_pStringTbl->field_4[idx];
    while (pEntry)
    {
        if (_strnicmp(strToFind, (const char*)&pEntry[1], pEntry->field_8_length) == 0) // string data is after the end of the structure
        {
            return pEntry;
        }
        idx++;
        pEntry = field_13350_pStringTbl->field_4[idx];
    }
    return 0;
}

STUB_FUNC(0x511b10)
char_type frosty_pasteur_0xC1EA8::sub_511B10(s32 idx)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x511b90)
void frosty_pasteur_0xC1EA8::sub_511B90()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x511c30)
void frosty_pasteur_0xC1EA8::sub_511C30()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x511C60)
void frosty_pasteur_0xC1EA8::sub_511C60()
{
    for (char_type bit = 0; bit < 32; bit++)
    {
        if (((1 << bit) & gGameSave_6F78C8.field_E4_car_and_script_data.field_48_flags) != 0)
        {
            SCR_POINTER* pPointer = (SCR_POINTER*)GetBasePointer_512770(field_C1E32[bit]);
            pPointer->field_8_obj->sub_5291E0(174);
            gfrosty_pasteur_6F8060->sub_512AA0(pPointer->field_8_obj->field_14_id);
        }
    }
}

MATCH_FUNC(0x511d40)
void frosty_pasteur_0xC1EA8::SaveMapInfo_511D40()
{
    strcpy(gGameSave_6F78C8.field_0_map_name, gLucid_hamilton_67E8E0.GetMapName_4C5940());
    strcpy(gGameSave_6F78C8.field_19_style_name, gLucid_hamilton_67E8E0.GetStyleName_4C5950());
    strcpy(gGameSave_6F78C8.field_32_script_name, gLucid_hamilton_67E8E0.GetScriptName_4C5960());
    gGameSave_6F78C8.field_0_map_name[24] = '\n';
    gGameSave_6F78C8.field_19_style_name[24] = '\n';
    gGameSave_6F78C8.field_32_script_name[24] = '\n';
    gGameSave_6F78C8.field_4B_main_stage = gLucid_hamilton_67E8E0.sub_4C5980();
    gGameSave_6F78C8.field_4C_lhv = gLucid_hamilton_67E8E0.sub_4C5990();
    gGameSave_6F78C8.field_4D_bonus_stage = gLucid_hamilton_67E8E0.sub_4C59A0();
}

STUB_FUNC(0x511f80)
s32 frosty_pasteur_0xC1EA8::sub_511F80(char_type* FileName)
{
    NOT_IMPLEMENTED;
    return 0;
}

// https://decomp.me/scratch/J0j6C
STUB_FUNC(0x511e10)
void frosty_pasteur_0xC1EA8::SaveGame_511E10(char_type* pFileName)
{
    size_t writeLen;
    unsigned short** pColData;
    unsigned int colBytes;

    gmp_block_info* pBlockInfo;
    unsigned int blockInfoBytes;

    Map_sub* pMapSub;
    int mapSubBytes;

    if (!pFileName || !*pFileName)
    {
        sprintf(gTmpBuffer_67C598, "player\\plyslot%d.svg", gLucid_hamilton_67E8E0.GetPlySlotIdx_4C59B0());
        pFileName = gTmpBuffer_67C598;
    }

    frosty_pasteur_0xC1EA8::sub_511B90();

    gGame_0x40_67E008->field_38_orf1->CopyPlayerDataToSave_56A1A0(&gGameSave_6F78C8.field_54_player_and_world_stats);

    gObject_5C_6F8F84->SaveObjects_52A500(&gGameSave_6F78C8.field_5E4_object_data);

    memcpy(gGameSave_6F78C8.field_5E4_object_data.field_12C_obj_5C_buffer, &gObject_5C_6F8F84->field_20, 50u);

    gGameSave_6F78C8.field_5E4_object_data.field_160_lhv = gLucid_hamilton_67E8E0.field_574;

    gMap_0x370_6F6268->sub_4E8CF0(&pColData, &colBytes, &pBlockInfo, &blockInfoBytes, &pMapSub, &mapSubBytes);

    frosty_pasteur_0xC1EA8::SaveMapInfo_511D40();

    writeLen = sizeof(SaveData_748);
    File::WriteBufferToFile_4A6E80(pFileName, &gGameSave_6F78C8, &writeLen); // gTurkishDelight_748_664590

    writeLen = 4;
    File::AppendBufferToFile_4A6F50(pFileName, &colBytes, &writeLen);
    if (colBytes)
    {
        File::AppendBufferToFile_4A6F50(pFileName, pColData, &colBytes);
    }

    writeLen = 4;
    File::AppendBufferToFile_4A6F50(pFileName, &blockInfoBytes, &writeLen);
    if (blockInfoBytes)
    {
        File::AppendBufferToFile_4A6F50(pFileName, pBlockInfo, &blockInfoBytes);
    }

    writeLen = 4;
    File::AppendBufferToFile_4A6F50(pFileName, &mapSubBytes, &writeLen);
    if (mapSubBytes)
    {
        File::AppendBufferToFile_4A6F50(pFileName, pMapSub, (unsigned int*)&mapSubBytes);
    }
}

MATCH_FUNC(0x512100)
SCR_CMD_HEADER* frosty_pasteur_0xC1EA8::sub_512100(u16 toFind, u16 startOff)
{
    u16 idx;
    SCR_CMD_HEADER* header;

    if (bSkip_mission_67D4E5)
    {
        return 0;
    }
    for (idx = 0; idx < 6000 - startOff; ++idx)
    {
        if (header = frosty_pasteur_0xC1EA8::GetBasePointer_512770(idx + startOff))
        {
            if (header->field_2_type == toFind)
            {
                return header;
            }
        }
    }
    return 0;
}

MATCH_FUNC(0x512160)
void frosty_pasteur_0xC1EA8::Update_512160()
{
    if (!bSkip_mission_67D4E5)
    {
        miss2_0x11C* pf_0 = miss2_0x11C_Pool_6F8064->sub_4767A0();
        SCR_CMD_HEADER* pLevelStart = sub_512100(59, 0);
        if (!pLevelStart)
        {
            FatalError_4A38C0(Gta2Error::ScriptfileNo_LEVELSTART_Declaration,
                              "C:\\Splitting\\Gta2\\Source\\miss2.cpp",
                              12774); // No LEVELSTART declaration in scriptfile
            return;
        }
        pf_0->ExecOpCode_5061C0();
        pf_0->sub_511930(0, pLevelStart->field_0_cmd_this);
        gGame_0x40_67E008->sub_4B9CD0();
    }
}

MATCH_FUNC(0x512330)
void frosty_pasteur_0xC1EA8::Load_512330(const char_type* pScrName)
{
    if (!bSkip_mission_67D4E5)
    {
        strcpy(field_35C_full_scr_file_name, pScrName);
        Error_SetName_4A0770(field_35C_full_scr_file_name);
        File::Global_Open_4A7060(field_35C_full_scr_file_name);
        u32 len = sizeof(field_46C_base_pointers);
        File::Global_Read_4A71C0(field_46C_base_pointers, len);
        len = sizeof(field_334C_script_data);
        File::Global_Read_4A71C0(field_334C_script_data, len);
        u16 tableSize;
        len = sizeof(tableSize);
        File::Global_Read_4A71C0(&tableSize, len);
        len = 5118;
        File::GetRemainderSize_4A7250(field_1334C_strings, &len);
        File::Global_Close_4A70C0();
        LoadStringTbl_5121E0(tableSize);
        GetScrFileName_5122D0();
        LoadSubScripts_5125F0();
    }
}

STUB_FUNC(0x512400)
void frosty_pasteur_0xC1EA8::sub_512400(s32 String1, u16* a3)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x5121E0)
void frosty_pasteur_0xC1EA8::LoadStringTbl_5121E0(u16 tableSize)
{
    NOT_IMPLEMENTED;
    u32 total_str_length = 0;
    BYTE* pStringDataIter1 = (BYTE*)field_1334C_strings;
    while (total_str_length < tableSize)
    {
        s32 str_length = pStringDataIter1[8] + 9;
        total_str_length += str_length;
        pStringDataIter1 += str_length;
    }

    field_13350_pStringTbl = reinterpret_cast<str_table_normalized*>(Memory::malloc_4FE4D0(sizeof(str_table_normalized)));
    memset(field_13350_pStringTbl, 0, sizeof(str_table_normalized));

    str_table_entry* pStringDataIter2 = field_1334C_strings;

    // s32 offset; // ebp
    if (tableSize)
    {
        //offset = 4;
        u32 total_str_length_ = 0;
        u16 str_count = 0;

        do
        {
            pStringDataIter2->field_2_zone_idx = gMap_0x370_6F6268->zone_idx_by_name_4DF050((char_type*)&pStringDataIter2[1],
                                                                                            strlen((const char_type*)&pStringDataIter2[1]));
            //offset += 4;
            field_13350_pStringTbl->field_4[str_count] = pStringDataIter2;
            s32 str_length_ = pStringDataIter2->field_8_length + 9;
            total_str_length_ += str_length_;
            pStringDataIter2 = (str_table_entry*)((char_type*)pStringDataIter2 + str_length_);
            ++str_count;
        } while (total_str_length_ < tableSize);

        field_13350_pStringTbl->field_0_string_count = str_count;
    }
    else
    {
        field_13350_pStringTbl->field_0_string_count = 0;
    }
}

MATCH_FUNC(0x5122D0)
void frosty_pasteur_0xC1EA8::GetScrFileName_5122D0()
{
    const char_type* pSlashPos = strrchr(field_35C_full_scr_file_name, '\\');
    if (pSlashPos)
    {
        pSlashPos++;

        memset(field_45C_scr_file_name, 0, sizeof(field_45C_scr_file_name));

        u32 lenAfterSlash = strlen(pSlashPos);
        while (lenAfterSlash)
        {
            if (pSlashPos[lenAfterSlash] == '.')
            {
                strncpy(field_45C_scr_file_name, pSlashPos, lenAfterSlash);
                return;
            }
            lenAfterSlash--;
        }
    }
}

STUB_FUNC(0x5125F0)
void frosty_pasteur_0xC1EA8::LoadSubScripts_5125F0()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x512770)
SCR_CMD_HEADER* frosty_pasteur_0xC1EA8::GetBasePointer_512770(u16 idx)
{
    if (!field_46C_base_pointers[idx])
    {
        return 0;
    }
    return (SCR_CMD_HEADER*)&field_334C_script_data[field_46C_base_pointers[idx]];
}

// Only waiting gMiss2Log_6F7698 & Write_Log_4D9650 to be defined
STUB_FUNC(0x5127a0)
void frosty_pasteur_0xC1EA8::ExecuteScriptThreads_5127A0()
{
    NOT_IMPLEMENTED;
    if (!bSkip_mission_67D4E5)
    {
        if (byte_6212EC)
        {
            if (gDo_miss_logging_67D6BC)
            {
                miss2_0x11C_Pool_6F8064->sub_47F4D0();
                //gMiss2Log_6F7698.Write_Log_4D9650("UTF-16LE"); //  TODO: ofstream stuff
            }
        }
        miss2_0x11C_Pool_6F8064->field_0_pool.UpdatePoolNoDeallocate();
    }
    gCar_6C_677930->field_5C = 0;
}

STUB_FUNC(0x5128a0)
s32* frosty_pasteur_0xC1EA8::sub_5128A0(s32 a2, s32 a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5128d0)
s32* frosty_pasteur_0xC1EA8::sub_5128D0(s32 a2, s32 a3, s16 a4)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x512910)
char_type frosty_pasteur_0xC1EA8::sub_512910(s32 a2, s32 a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x512980)
s32* frosty_pasteur_0xC1EA8::sub_512980(s32 a2, s32 a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x5129b0)
void frosty_pasteur_0xC1EA8::sub_5129B0(s32 a2, s32 obj_f14, u16 cmd_line)
{
    thread_C* thread = &field_188_thrds_4[0];
    for (u16 i = 0; i < 20 && thread->field_8_cmd_line != 0; i++)
    {
        thread++;
    }
    thread->field_0_unk = a2;
    thread->field_4_obj_f14 = obj_f14;
    thread->field_8_cmd_line = cmd_line;
    field_184_count++;
}

STUB_FUNC(0x5129f0)
char_type frosty_pasteur_0xC1EA8::sub_5129F0(s32 a2, s32 a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x512a70)
s32* frosty_pasteur_0xC1EA8::sub_512A70(s32 a2, s32 a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x512aa0)
char_type* frosty_pasteur_0xC1EA8::sub_512AA0(s32 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x512ad0)
char_type* frosty_pasteur_0xC1EA8::sub_512AD0(s32 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x512af0)
char_type frosty_pasteur_0xC1EA8::sub_512AF0(s32 a2, char_type a3, char_type a4)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x512ba0)
char_type* frosty_pasteur_0xC1EA8::sub_512BA0(s32 a2, char_type a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x512c00)
s32 frosty_pasteur_0xC1EA8::sub_512C00(s32 a2, s32 a3, char_type a4)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x512c70)
bool frosty_pasteur_0xC1EA8::sub_512C70(s32 a2, char_type a3, char_type a4)
{
    NOT_IMPLEMENTED;
    return 0;
}

// https://decomp.me/scratch/qh4EW
STUB_FUNC(0x512ce0)
frosty_pasteur_0xC1EA8::frosty_pasteur_0xC1EA8()
{
    NOT_IMPLEMENTED;
    field_13350_pStringTbl = 0;
    if (!bSkip_mission_67D4E5)
    {
        memset(field_334C_script_data, 0, sizeof(field_334C_script_data));
        if (!miss2_0x11C_Pool_6F8064)
        {
            miss2_0x11C_Pool_6F8064 = new miss2_0x11C_Pool();
        }
    }
    field_1334C_strings = (str_table_entry*)Memory::malloc_4FE4D0(0x1400u);
    memset(field_35C_full_scr_file_name, 0, sizeof(field_35C_full_scr_file_name));
    memset(field_334C_script_data, 0, sizeof(field_334C_script_data));
    memset(field_46C_base_pointers, 0, sizeof(field_46C_base_pointers));
    memset(field_1334C_strings, 0, 0x1400u);
    field_0 = 0;
    memset(field_4_thrds_2, 0, sizeof(field_4_thrds_2));
    field_184_count = 0;
    memset(field_188_thrds_4, 0, sizeof(field_188_thrds_4));
    field_278 = 0;
    memset(field_27C, 0, sizeof(field_27C));
    memset(&gGameSave_6F78C8, 0, sizeof(gGameSave_6F78C8));

    gMiss2_25C_6F805C = new Miss2_25C();
    if (!gMiss2_25C_6F805C)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\miss2.cpp", 13630);
    }

    memset(field_13354, 0, sizeof(field_13354));
    memset(field_AA934, 0, sizeof(field_AA934));
    memset(field_C1D34, 0, sizeof(field_C1D34));
    memset(field_C1D72, 0, sizeof(field_C1D72));
    memset(field_C1DB0, 0, sizeof(field_C1DB0));
    field_344_mission_flag = 0;
    field_2F4 = 1;
    field_C1E2C = 0;
    field_310_finish_score = 0;
    field_314_total_missions = 0;
    field_318_total_secrets = 0;
    field_31C_gang_1_missions_total = 0;
    field_320_gang_2_missions_total = 0;
    field_324_gang_3_missions_total = 0;
    field_328_passed_flag = 0;
    field_32C_1_passed_flag = 0;
    field_330_2_passed_flag = 0;
    field_334_3_passed_flag = 0;
    field_338_secrets_passed = 0;
    field_33C_secrets_failed = 0;
    field_340_car_list = 0;
    field_348_gang_1_mission_flag = 0;
    field_34C_gang_2_mission_flag = 0;
    field_350_gang_3_mission_flag = 0;
    field_354 = 0;
    field_C1E70 = 87;
    field_356 = 0;
    field_358 = 0;
    field_35A = 0;
    field_355 = 0;
    field_C1E2E_death_arrest_flag = 0;
    field_C1E2D = 0;
    field_468 = 0;

    if (gDo_miss_logging_67D6BC)
    {
        //gMiss2Log_6F7698.sub_4D9470("test\\MISS_LOG.TXT", 1);
    }

    memset(field_45C_scr_file_name, 0, sizeof(field_45C_scr_file_name));
    memset(field_C1E32, 0, sizeof(field_C1E32));
    memset(field_C1E2F, 0, sizeof(field_C1E2F));
    memset(field_C1E74_basic_kf, 0, sizeof(field_C1E74_basic_kf));

    gStoredCar_6F7560 = NULL;
    gStoredCarId_6F78B4 = 0;
}

STUB_FUNC(0x5130e0)
frosty_pasteur_0xC1EA8::~frosty_pasteur_0xC1EA8()
{
    NOT_IMPLEMENTED;
}