#include "jolly_poitras_0x2BC0.hpp"
#include "Function.hpp"
#include "Game_0x40.hpp"
#include "Globals.hpp"
#include "Player.hpp"
#include "error.hpp"
#include "file.hpp"
#include "lucid_hamilton.hpp"
#include <io.h>

// GLOBAL: 105 0x6FEAC0
DEFINE_GLOBAL(jolly_poitras_0x2BC0*, gJolly_poitras_0x2BC0_6FEAC0);
EXTERN_GLOBAL_ARRAY(wchar_t, word_67DC8C, 32);
// GLOBAL: 105 0x6242B0
DEFINE_GLOBAL_ARRAY(score_table_line, stru_6242B0, 10);
// GLOBAL: 105 0x6243A0
DEFINE_GLOBAL_ARRAY(score_table_line, stru_6243A0, 120); // [3][4][10]

// TODO
EXTERN_GLOBAL(s32, bStartNetworkGame_7081F0);


// FUNCTION: 105 0x56B6E0
jolly_poitras_0x2BC0::jolly_poitras_0x2BC0()
{
    for (s32 i = 0; i < 3; i++)
    {
        memset(&field_1800_best_stats[i], 0, sizeof(struc_221));
        field_1878[i] = 0;
        field_1884[i] = 0;
    }

    for (u32 j = 0; (u16)j < 8; j++)
    {
        if (PlySlotDatExists_56B940(j))
        {
            sub_56B990(j);
        }
        else
        {
            sub_56BA60(j);
        }
    }

    if (HiScoreHscExists_56BCA0())
    {
        sub_56BE50();
    }
    else
    {
        sub_56C1D0();
        sub_56BF20();
    }

    sub_56BD20();
}

// STUB: 105 0x56B810
jolly_poitras_0x2BC0::~jolly_poitras_0x2BC0()
{
    NOT_IMPLEMENTED;

    // Should match but doesn't
}

// https://decomp.me/scratch/oIJET
// STUB: 105 0x56BB10
void jolly_poitras_0x2BC0::sub_56BB10(Player* pPlayer)
{
    NOT_IMPLEMENTED;

    const s32 slot_idx = gLucid_hamilton_67E8E0.GetPlySlotIdx_4C59B0();
    u8 map_num;
    u8 bonus_num;
    if (!gLucid_hamilton_67E8E0.sub_4C59A0())
    {
        map_num = gLucid_hamilton_67E8E0.sub_4C5980();
        bonus_num = 0;
    }
    else
    {
        const u8 map_and_bonus_nibbles = gLucid_hamilton_67E8E0.sub_4C5990();
        map_num = map_and_bonus_nibbles >> 4;
        bonus_num = map_and_bonus_nibbles & 0xF;
    }

    stage_stats* pStageStats = &this->field_26A0_plyr_stats[slot_idx].field_0_plyr_stage_stats[map_num][bonus_num];
    const u32 latest_score = pPlayer->field_2D4_unk.sub_592370();
    if (latest_score > pStageStats->field_4_stage_best_score)
    {
        pStageStats->field_4_stage_best_score = latest_score;
    }
    pStageStats->field_8_stage_latest_score = latest_score;
    sub_56BA60(slot_idx);
}

// STUB: 105 0x56C010
void jolly_poitras_0x2BC0::sub_56C010()
{
    NOT_IMPLEMENTED;
    // todo
}

// FUNCTION: 105 0x56B8A0
void jolly_poitras_0x2BC0::GetPlySlotDatName_56B8A0(u16 a1, char_type* a2)
{
    char_type Buffer[8];
    _itoa(a1, Buffer, 10);
    strcpy(a2, "player\\plyslot");
    strcat(a2, Buffer);
    strcat(a2, ".dat");
}

// FUNCTION: 105 0x56B940
char_type jolly_poitras_0x2BC0::PlySlotDatExists_56B940(s32 a1)
{
    char_type FileName[356];
    GetPlySlotDatName_56B8A0(a1, FileName);

    _finddata_t findData;
    long hFind = _findfirst(FileName, &findData);
    if (hFind == -1)
    {
        return 0;
    }
    _findclose(hFind);
    return 1;
}

// FUNCTION: 105 0x56BCF0
void jolly_poitras_0x2BC0::GetHiScoreHscFileName_56BCF0(char_type* pName)
{
    strcpy(pName, "player\\hiscores.hsc");
}

// FUNCTION: 105 0x56BCA0
char_type jolly_poitras_0x2BC0::HiScoreHscExists_56BCA0()
{
    char_type FileName[356];
    GetHiScoreHscFileName_56BCF0(FileName);

    _finddata_t findData;
    long hFind = _findfirst(FileName, &findData);
    if (hFind == -1)
    {
        return 0;
    }
    _findclose(hFind);
    return 1;
}

// FUNCTION: 105 0x56B990
void jolly_poitras_0x2BC0::sub_56B990(u16 slotIdx)
{
    char_type FileName[356];

    player_stats_0xA4* pTmp = &field_26A0_plyr_stats[slotIdx];

    GetPlySlotDatName_56B8A0(slotIdx, FileName);
    File::Global_Open_4A7060(FileName);

    for (s32 i = 0; i < 9; i++)
    {
        u32 len_read = 2;
        File::Global_Read_4A71C0(&pTmp->field_90_strPlayerName[i], len_read);
    }

    for (s32 k = 0; k < 3; k++)
    {
        for (s32 j = 0; j < 4; j++)
        {
            u32 len_read = 1;
            File::Global_Read_4A71C0(&field_26A0_plyr_stats[slotIdx].field_0_plyr_stage_stats[k][j].field_0_is_stage_unlocked, len_read);

            len_read = 4;
            File::Global_Read_4A71C0(&field_26A0_plyr_stats[slotIdx].field_0_plyr_stage_stats[k][j].field_4_stage_best_score, len_read);

            len_read = 4;
            File::Global_Read_4A71C0(&field_26A0_plyr_stats[slotIdx].field_0_plyr_stage_stats[k][j].field_8_stage_latest_score, len_read);
        }
    }
    File::Global_Close_4A70C0();
}

// STUB: 105 0x56BA60
void jolly_poitras_0x2BC0::sub_56BA60(s16 a2)
{
    NOT_IMPLEMENTED;
    // todo
}

/*
s32 len;
    s32 k3Counter;
    s32 k4Counter;

    char_type FileName[256];
    GetHiScoreHscFileName_56BCF0(FileName);
    File::Global_Open_4A7060(FileName);

    len = 240;
    File::Global_Read_4A71C0(&field_23D0.field_0, &len);


    for (k3Counter = 0; k3Counter < 3; k3Counter++)
    {
        for (k4Counter = 0; k4Counter < 4; k4Counter++)
        {
            len = 240;
            File::Global_Read_4A71C0(&field_1890_stage_scores[k3Counter][k4Counter].field_0, &len);
        }

        len = 40;
        File::Global_Read_4A71C0(&field_1800_best_stats[k3Counter], &len); // 3 40 byte objs

        len = 4;
        File::Global_Read_4A71C0(&field_1878[k3Counter], &len);

        len = 4;
        File::Global_Read_4A71C0(&field_1884[k3Counter], &len);
    }

    File::Global_Close_4A70C0();
*/

// FUNCTION: 105 0x56BE50
void jolly_poitras_0x2BC0::sub_56BE50()
{
    char_type FileName[256];
    GetHiScoreHscFileName_56BCF0(FileName);
    File::Global_Open_4A7060(FileName);

    File::Global_Read_4A71C0(&field_23D0.field_0_score_table_line, 240);

    for (s32 k3Counter = 0; k3Counter < 3; k3Counter++)
    {
        for (s32 k4Counter = 0; k4Counter < 4; k4Counter++)
        {
            File::Global_Read_4A71C0(&field_1890_stage_scores[k3Counter][k4Counter], 240);
        }

        File::Global_Read_4A71C0(&field_1800_best_stats[k3Counter], 40); // 3 40 byte objs

        File::Global_Read_4A71C0(&field_1878[k3Counter], 4);

        File::Global_Read_4A71C0(&field_1884[k3Counter], 4);
    }

    File::Global_Close_4A70C0();
}

// FUNCTION: 105 0x56C1D0
void jolly_poitras_0x2BC0::sub_56C1D0()
{
    field_23D0.Init_56B520();
    score_table_line* p10StruIter = stru_6242B0;
    for (s32 k10 = 0; k10 < 10; k10++)
    {
        field_23D0.sub_56B550(p10StruIter->field_0_player_name, p10StruIter->field_14_score);
        ++p10StruIter;
    }

    score_table_line* pSruIter = stru_6243A0;
    for (s32 k3 = 0; k3 < 3; k3++)
    {
        for (s32 k4 = 0; k4 < 4; k4++)
        {
            field_1890_stage_scores[k3][k4].Init_56B520();
            for (s32 k10 = 0; k10 < 10; k10++)
            {
                field_1890_stage_scores[k3][k4].sub_56B550(pSruIter->field_0_player_name, pSruIter->field_14_score);
                ++pSruIter;
            }
        }
    }
}

// STUB: 105 0x56BF20
void jolly_poitras_0x2BC0::sub_56BF20()
{
    NOT_IMPLEMENTED;
    // todo
}

// FUNCTION: 105 0x56BD20
void jolly_poitras_0x2BC0::sub_56BD20()
{
    wcsncpy(field_24C0.field_0_score_table_line[0].field_0_player_name, L"ALAN", 9u);
    field_24C0.field_0_score_table_line[0].field_14_score = 1000000;
    wcsncpy(field_24C0.field_0_score_table_line[1].field_0_player_name, L"BRIAN", 9u);
    field_24C0.field_0_score_table_line[1].field_14_score = 500000;
    wcsncpy(field_24C0.field_0_score_table_line[2].field_0_player_name, L"COLIN", 9u);
    field_24C0.field_0_score_table_line[2].field_14_score = 400000;
    wcsncpy(field_24C0.field_0_score_table_line[3].field_0_player_name, L"DAVE", 9u);
    field_24C0.field_0_score_table_line[3].field_14_score = 300000;
    wcsncpy(field_24C0.field_0_score_table_line[4].field_0_player_name, L"ERIC", 9u);
    field_24C0.field_0_score_table_line[4].field_14_score = 250000;
    wcsncpy(field_24C0.field_0_score_table_line[5].field_0_player_name, L"FRANK", 9u);
    field_24C0.field_0_score_table_line[5].field_14_score = 200000;
    wcsncpy(field_24C0.field_0_score_table_line[6].field_0_player_name, L"GRAEME", 9u);
    field_24C0.field_0_score_table_line[6].field_14_score = 100000;
    wcsncpy(field_24C0.field_0_score_table_line[7].field_0_player_name, L"HECTOR", 9u);
    field_24C0.field_0_score_table_line[7].field_14_score = 50000;
    wcsncpy(field_24C0.field_0_score_table_line[8].field_0_player_name, L"IMOGEN", 9u);
    field_24C0.field_0_score_table_line[8].field_14_score = 25000;
    wcsncpy(field_24C0.field_0_score_table_line[9].field_0_player_name, L"JACKSON", 9u);
    field_24C0.field_0_score_table_line[9].field_14_score = 10000;
}

// FUNCTION: 105 0x56BC40
void jolly_poitras_0x2BC0::sub_56BC40()
{
    const u8 slot_idx = gLucid_hamilton_67E8E0.GetPlySlotIdx_4C59B0();
    player_stats_0xA4* pStats = &this->field_26A0_plyr_stats[slot_idx];
    for (s32 k3 = 0; k3 < 3; k3++)
    {
        for (s32 k4 = 0; k4 < 4; k4++)
        {
            pStats->field_0_plyr_stage_stats[k3][k4].field_0_is_stage_unlocked = 1;
        }
    }

    if (!bStartNetworkGame_7081F0)
    {
        sub_56BA60(slot_idx);
    }
}

// FUNCTION: 105 0x56BBD0
void jolly_poitras_0x2BC0::sub_56BBD0(u8 map_num, u8 bonus_num)
{
    const u8 slot_idx = gLucid_hamilton_67E8E0.GetPlySlotIdx_4C59B0();
    this->field_26A0_plyr_stats[slot_idx].field_0_plyr_stage_stats[map_num][bonus_num].field_0_is_stage_unlocked = 1;
    if (!bStartNetworkGame_7081F0)
    {
        sub_56BA60(slot_idx);
    }
}

// FUNCTION: 105 0x56C250
void jolly_poitras_0x2BC0::DoMuchCashCheat_56C250()
{
    if (!bStartNetworkGame_7081F0)
    {
        player_stats_0xA4* pStats = &this->field_26A0_plyr_stats[gLucid_hamilton_67E8E0.GetPlySlotIdx_4C59B0()];
        if (wcscmp(pStats->field_90_strPlayerName, L"MUCHCASH") == 0)
        {
            Player* pPlayer = gGame_0x40_67E008->field_38_orf1;
            pPlayer->field_2D4_unk.AddCash_592620(pPlayer->field_6BC_multpliers.field_0 * 500000);
        }
    }
}

// FUNCTION: 105 0x56C2C0
void jolly_poitras_0x2BC0::create_56C2C0()
{
    if (!gJolly_poitras_0x2BC0_6FEAC0)
    {
        gJolly_poitras_0x2BC0_6FEAC0 = new jolly_poitras_0x2BC0();
        if (!gJolly_poitras_0x2BC0_6FEAC0)
        {
            FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\plydat.cpp", 1269);
        }
    }
}

// FUNCTION: 105 0x56C340
void jolly_poitras_0x2BC0::destroy_56C340()
{
    if (gJolly_poitras_0x2BC0_6FEAC0)
    {
        delete gJolly_poitras_0x2BC0_6FEAC0;
        gJolly_poitras_0x2BC0_6FEAC0 = 0;
    }
}

// =====================================================================

// FUNCTION: 105 0x56B500
high_score_table_0xF0::high_score_table_0xF0()
{
    Init_56B520();
}

// FUNCTION: 105 0x56B510
high_score_table_0xF0::~high_score_table_0xF0()
{
}

// FUNCTION: 105 0x56B520
void high_score_table_0xF0::Init_56B520()
{
    for (s32 i = 0; i < 10; i++)
    {
        wcscpy(field_0_score_table_line[i].field_0_player_name, word_67DC8C);
        field_0_score_table_line[i].field_14_score = 0;
    }
}

// STUB: 105 0x56B550
char_type high_score_table_0xF0::sub_56B550(const wchar_t* pFindStr, s32 findScore)
{
    NOT_IMPLEMENTED;
    u16 startIdx = 10;
    for (s16 i = 9; i != -1; --i)
    {
        if (findScore > field_0_score_table_line[i].field_14_score)
        {
            startIdx = i;
        }

        if (findScore == field_0_score_table_line[i].field_14_score &&
            wcscmp(pFindStr, field_0_score_table_line[i].field_0_player_name) == 0)
        {
            return 0;
        }
    }

    if (startIdx != 10)
    {

        if (startIdx < 9u)
        {
            // ??????
            score_table_line* pIter = &field_0_score_table_line[8]; // .field_14_score;
            //  int* pIter = &field_0[9 - 1].field_14_score;

            s32 remainderCount = 9 - startIdx;
            do
            {
                wcsncpy((pIter + 1)->field_0_player_name, (pIter)->field_0_player_name, 9u);
                (pIter + 1)->field_14_score = (pIter)->field_14_score;

                pIter--;

                --remainderCount;
            } while (remainderCount);
            //newScore = findScore;
        }

        wcsncpy(field_0_score_table_line[startIdx].field_0_player_name, pFindStr, 9u);
        field_0_score_table_line[startIdx].field_14_score = findScore;

        return 1;
    }
    return 0;
}

// FUNCTION: 105 0x56B610
player_stats_0xA4::player_stats_0xA4()
{
    sub_56B630();
}

// FUNCTION: 105 0x56B620
player_stats_0xA4::~player_stats_0xA4()
{
}

// FUNCTION: 105 0x56B630
void player_stats_0xA4::sub_56B630()
{
    for (u16 k = 0; k < 9; k++)
    {
        field_90_strPlayerName[k] = 0;
    }

    for (u32 i = 0; i < 3; i++)
    {
        for (u32 j = 0; j < 4; j++)
        {
            field_0_plyr_stage_stats[i][j].field_0_is_stage_unlocked = false;
            field_0_plyr_stage_stats[i][j].field_4_stage_best_score = 0;
            field_0_plyr_stage_stats[i][j].field_8_stage_latest_score = 0;
        }
    }
    field_0_plyr_stage_stats[0][0].field_0_is_stage_unlocked = true;
}

// FUNCTION: 105 0x56B680
s32 player_stats_0xA4::sub_56B680()
{
    s32 result = 0;

    for (u32 i = 0; i < 3; i++)
    {
        for (u32 j = 0; j < 4; j++)
        {
            result += field_0_plyr_stage_stats[i][j].field_8_stage_latest_score;
        }
    }
    return result;
}

// FUNCTION: 105 0x56B6B0
s32 player_stats_0xA4::sub_56B6B0()
{
    s32 result = 0;

    for (u32 i = 0; i < 3; i++)
    {
        for (u32 j = 0; j < 4; j++)
        {
            result += field_0_plyr_stage_stats[i][j].field_4_stage_best_score;
        }
    }
    return result;
}