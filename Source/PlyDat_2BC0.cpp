#include "PlyDat_2BC0.hpp"
#include "Function.hpp"
#include "Game_0x40.hpp"
#include "Globals.hpp"
#include "Player.hpp"
#include "error.hpp"
#include "file.hpp"
#include "GameSession_578.hpp"
#include <io.h>

DEFINE_GLOBAL(PlyDat_2BC0*, gPlyDat_6FEAC0, 0x6FEAC0);
EXTERN_GLOBAL_ARRAY(wchar_t, gEmptyWStr_67DC8C, 32);
DEFINE_GLOBAL_ARRAY_INIT(ScoreTableLine_18, gDefaultHiScores_6242B0, 10, 0x6242B0,
    { L"ALISDAIR" COMMA 50000 } COMMA
    { L"BILLY"    COMMA 40000 } COMMA
    { L"BRIAN"    COMMA 30000 } COMMA
    { L"COLIN"    COMMA 20000 } COMMA
    { L"IAIN"     COMMA 10000 } COMMA
    { L"IAN"      COMMA  9000 } COMMA
    { L"KEITH"    COMMA  8000 } COMMA
    { L"MARTIN"   COMMA  7000 } COMMA
    { L"STEPHEN"  COMMA  6000 } COMMA
    { L"WILLIAM"  COMMA  5000 } COMMA
);
DEFINE_GLOBAL_ARRAY_INIT(ScoreTableLine_18, gDefaultStageHiScores_6243A0, 120, 0x6243A0,   //, , 3][4][10, 0xUNKNOWN);
    {L"ALISDAIR" COMMA 50000 } COMMA
    {L"BILLY"    COMMA 40000 } COMMA
    {L"BRIAN"    COMMA 30000 } COMMA
    {L"COLIN"    COMMA 20000 } COMMA
    {L"IAIN"     COMMA 10000 } COMMA
    {L"IAN"      COMMA  9000 } COMMA
    {L"KEITH"    COMMA  8000 } COMMA
    {L"MARTIN"   COMMA  7000 } COMMA
    {L"STEPHEN"  COMMA  6000 } COMMA
    {L"WILLIAM"  COMMA  5000 } COMMA
    {L"ALISDAIR" COMMA 50000 } COMMA
    {L"BILLY"    COMMA 40000 } COMMA
    {L"BRIAN"    COMMA 30000 } COMMA
    {L"COLIN"    COMMA 20000 } COMMA
    {L"IAIN"     COMMA 10000 } COMMA
    {L"IAN"      COMMA  9000 } COMMA
    {L"KEITH"    COMMA  8000 } COMMA
    {L"MARTIN"   COMMA  7000 } COMMA
    {L"STEPHEN"  COMMA  6000 } COMMA
    {L"WILLIAM"  COMMA  5000 } COMMA
    {L"ALISDAIR" COMMA 50000 } COMMA
    {L"BILLY"    COMMA 40000 } COMMA
    {L"BRIAN"    COMMA 30000 } COMMA
    {L"COLIN"    COMMA 20000 } COMMA
    {L"IAIN"     COMMA 10000 } COMMA
    {L"IAN"      COMMA  9000 } COMMA
    {L"KEITH"    COMMA  8000 } COMMA
    {L"MARTIN"   COMMA  7000 } COMMA
    {L"STEPHEN"  COMMA  6000 } COMMA
    {L"WILLIAM"  COMMA  5000 } COMMA
    {L"ALISDAIR" COMMA 50000 } COMMA
    {L"BILLY"    COMMA 40000 } COMMA
    {L"BRIAN"    COMMA 30000 } COMMA
    {L"COLIN"    COMMA 20000 } COMMA
    {L"IAIN"     COMMA 10000 } COMMA
    {L"IAN"      COMMA  9000 } COMMA
    {L"KEITH"    COMMA  8000 } COMMA
    {L"MARTIN"   COMMA  7000 } COMMA
    {L"STEPHEN"  COMMA  6000 } COMMA
    {L"WILLIAM"  COMMA  5000 } COMMA
    {L"ALISDAIR" COMMA 50000 } COMMA
    {L"BILLY"    COMMA 40000 } COMMA
    {L"BRIAN"    COMMA 30000 } COMMA
    {L"COLIN"    COMMA 20000 } COMMA
    {L"IAIN"     COMMA 10000 } COMMA
    {L"IAN"      COMMA  9000 } COMMA
    {L"KEITH"    COMMA  8000 } COMMA
    {L"MARTIN"   COMMA  7000 } COMMA
    {L"STEPHEN"  COMMA  6000 } COMMA
    {L"WILLIAM"  COMMA  5000 } COMMA
    {L"ALISDAIR" COMMA 50000 } COMMA
    {L"BILLY"    COMMA 40000 } COMMA
    {L"BRIAN"    COMMA 30000 } COMMA
    {L"COLIN"    COMMA 20000 } COMMA
    {L"IAIN"     COMMA 10000 } COMMA
    {L"IAN"      COMMA  9000 } COMMA
    {L"KEITH"    COMMA  8000 } COMMA
    {L"MARTIN"   COMMA  7000 } COMMA
    {L"STEPHEN"  COMMA  6000 } COMMA
    {L"WILLIAM"  COMMA  5000 } COMMA
    {L"ALISDAIR" COMMA 50000 } COMMA
    {L"BILLY"    COMMA 40000 } COMMA
    {L"BRIAN"    COMMA 30000 } COMMA
    {L"COLIN"    COMMA 20000 } COMMA
    {L"IAIN"     COMMA 10000 } COMMA
    {L"IAN"      COMMA  9000 } COMMA
    {L"KEITH"    COMMA  8000 } COMMA
    {L"MARTIN"   COMMA  7000 } COMMA
    {L"STEPHEN"  COMMA  6000 } COMMA
    {L"WILLIAM"  COMMA  5000 } COMMA
    {L"ALISDAIR" COMMA 50000 } COMMA
    {L"BILLY"    COMMA 40000 } COMMA
    {L"BRIAN"    COMMA 30000 } COMMA
    {L"COLIN"    COMMA 20000 } COMMA
    {L"IAIN"     COMMA 10000 } COMMA
    {L"IAN"      COMMA  9000 } COMMA
    {L"KEITH"    COMMA  8000 } COMMA
    {L"MARTIN"   COMMA  7000 } COMMA
    {L"STEPHEN"  COMMA  6000 } COMMA
    {L"WILLIAM"  COMMA  5000 } COMMA
    {L"ALISDAIR" COMMA 50000 } COMMA
    {L"BILLY"    COMMA 40000 } COMMA
    {L"BRIAN"    COMMA 30000 } COMMA
    {L"COLIN"    COMMA 20000 } COMMA
    {L"IAIN"     COMMA 10000 } COMMA
    {L"IAN"      COMMA  9000 } COMMA
    {L"KEITH"    COMMA  8000 } COMMA
    {L"MARTIN"   COMMA  7000 } COMMA
    {L"STEPHEN"  COMMA  6000 } COMMA
    {L"WILLIAM"  COMMA  5000 } COMMA
    {L"ALISDAIR" COMMA 50000 } COMMA
    {L"BILLY"    COMMA 40000 } COMMA
    {L"BRIAN"    COMMA 30000 } COMMA
    {L"COLIN"    COMMA 20000 } COMMA
    {L"IAIN"     COMMA 10000 } COMMA
    {L"IAN"      COMMA  9000 } COMMA
    {L"KEITH"    COMMA  8000 } COMMA
    {L"MARTIN"   COMMA  7000 } COMMA
    {L"STEPHEN"  COMMA  6000 } COMMA
    {L"WILLIAM"  COMMA  5000 } COMMA
    {L"ALISDAIR" COMMA 50000 } COMMA
    {L"BILLY"    COMMA 40000 } COMMA
    {L"BRIAN"    COMMA 30000 } COMMA
    {L"COLIN"    COMMA 20000 } COMMA
    {L"IAIN"     COMMA 10000 } COMMA
    {L"IAN"      COMMA  9000 } COMMA
    {L"KEITH"    COMMA  8000 } COMMA
    {L"MARTIN"   COMMA  7000 } COMMA
    {L"STEPHEN"  COMMA  6000 } COMMA
    {L"WILLIAM"  COMMA  5000 } COMMA
    {L"ALISDAIR" COMMA 50000 } COMMA
    {L"BILLY"    COMMA 40000 } COMMA
    {L"BRIAN"    COMMA 30000 } COMMA
    {L"COLIN"    COMMA 20000 } COMMA
    {L"IAIN"     COMMA 10000 } COMMA
    {L"IAN"      COMMA  9000 } COMMA
    {L"KEITH"    COMMA  8000 } COMMA
    {L"MARTIN"   COMMA  7000 } COMMA
    {L"STEPHEN"  COMMA  6000 } COMMA
    {L"WILLIAM"  COMMA  5000 } COMMA
);

// TODO
EXTERN_GLOBAL(s32, bStartNetworkGame_7081F0);


MATCH_FUNC(0x56B500)
HighScoreTable_F0::HighScoreTable_F0()
{
    Init_56B520();
}

MATCH_FUNC(0x56B510)
HighScoreTable_F0::~HighScoreTable_F0()
{
}

MATCH_FUNC(0x56B520)
void HighScoreTable_F0::Init_56B520()
{
    for (s32 i = 0; i < 10; i++)
    {
        wcscpy(field_0_score_table_line[i].field_0_player_name, gEmptyWStr_67DC8C);
        field_0_score_table_line[i].field_14_score = 0;
    }
}

MATCH_FUNC(0x56B550)
char_type HighScoreTable_F0::InsertScore_56B550(const wchar_t* pFindStr, s32 findScore)
{
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
            for (u16 k = 9; k > startIdx; --k)
            {
                wcsncpy(field_0_score_table_line[k].field_0_player_name, field_0_score_table_line[k - 1].field_0_player_name, 9u);
                field_0_score_table_line[k].field_14_score = field_0_score_table_line[k - 1].field_14_score;
            }
        }

        wcsncpy(field_0_score_table_line[startIdx].field_0_player_name, pFindStr, 9u);
        field_0_score_table_line[startIdx].field_14_score = findScore;

        return 1;
    }
    return 0;
}

MATCH_FUNC(0x56B610)
PlySlot_A4::PlySlot_A4()
{
    ResetPlayerSlot_56B630();
}

MATCH_FUNC(0x56B620)
PlySlot_A4::~PlySlot_A4()
{
}

MATCH_FUNC(0x56B630)
void PlySlot_A4::ResetPlayerSlot_56B630()
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

MATCH_FUNC(0x56B680)
s32 PlySlot_A4::GetTotalLatestScore_56B680()
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

MATCH_FUNC(0x56B6B0)
s32 PlySlot_A4::GetTotalBestScore_56B6B0()
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

/*
s32 len;
    s32 k3Counter;
    s32 k4Counter;

    char_type FileName[256];
    GetHiScoreHscFileName_56BCF0(FileName);
    File::Global_Open_4A7060(FileName);

    len = 240;
    File::Global_Read_4A71C0(&field_23D0_total_scores.field_0, &len);


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
        File::Global_Read_4A71C0(&field_1878_best_car_damage_cost[k3Counter], &len);

        len = 4;
        File::Global_Read_4A71C0(&field_1884_best_evasion_rating[k3Counter], &len);
    }

    File::Global_Close_4A70C0();
*/

MATCH_FUNC(0x56B6E0)
PlyDat_2BC0::PlyDat_2BC0()
{
    for (s32 i = 0; i < 3; i++)
    {
        memset(&field_1800_best_stats[i], 0, sizeof(BestStageStats_28));
        field_1878_best_car_damage_cost[i] = 0;
        field_1884_best_evasion_rating[i] = 0;
    }

    for (u32 j = 0; (u16)j < 8; j++)
    {
        if (PlySlotDatExists_56B940(j))
        {
            LoadPlySlotDat_56B990(j);
        }
        else
        {
            SavePlySlotDat_56BA60(j);
        }
    }

    if (HiScoreHscExists_56BCA0())
    {
        LoadHiScores_56BE50();
    }
    else
    {
        InitDefaultHiScores_56C1D0();
        SaveHiScores_56BF20();
    }

    InitAltHiScores_56BD20();
}

// Defined after ~HighScoreTable_F0 so VC6 knows that destructor can't throw and
// drops the EH state updates between the calls to it.
MATCH_FUNC(0x56B810)
PlyDat_2BC0::~PlyDat_2BC0()
{
}

MATCH_FUNC(0x56B8A0)
void PlyDat_2BC0::GetPlySlotDatName_56B8A0(u16 slot_idx, char_type* pName)
{
    char_type Buffer[8];
    _itoa(slot_idx, Buffer, 10);
    strcpy(pName, "player\\plyslot");
    strcat(pName, Buffer);
    strcat(pName, ".dat");
}

MATCH_FUNC(0x56B940)
char_type PlyDat_2BC0::PlySlotDatExists_56B940(s32 slot_idx)
{
    char_type FileName[356];
    GetPlySlotDatName_56B8A0(slot_idx, FileName);

    _finddata_t findData;
    long hFind = _findfirst(FileName, &findData);
    if (hFind == -1)
    {
        return 0;
    }
    _findclose(hFind);
    return 1;
}

MATCH_FUNC(0x56B990)
void PlyDat_2BC0::LoadPlySlotDat_56B990(u16 slotIdx)
{
    char_type FileName[356];

    PlySlot_A4* pTmp = &field_26A0_plyr_stats[slotIdx];

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

MATCH_FUNC(0x56BA60)
void PlyDat_2BC0::SavePlySlotDat_56BA60(s16 slotIdx)
{
    char_type FileName[356];
    size_t len;

    GetPlySlotDatName_56B8A0(slotIdx, FileName);

    // The start of this object is reused as the buffer for the player slot .dat file
    memcpy(this,
           field_26A0_plyr_stats[(u16)slotIdx].field_90_strPlayerName,
           sizeof(field_26A0_plyr_stats[0].field_90_strPlayerName));
    len = sizeof(field_26A0_plyr_stats[0].field_90_strPlayerName);

    // len is accumulated like in SaveHiScores_56BF20: VC6 folds its final value (126) into one store
    // that it emits after the loop counter init, which a plain "len = 126" never does.
    u8* pDst = reinterpret_cast<u8*>(this) + sizeof(field_26A0_plyr_stats[0].field_90_strPlayerName);
    for (s32 k = 0; k < 3; k++)
    {
        for (s32 j = 0; j < 4; j++)
        {
            *pDst = field_26A0_plyr_stats[(u16)slotIdx].field_0_plyr_stage_stats[k][j].field_0_is_stage_unlocked;
            pDst++;
            len++;
            *reinterpret_cast<u32*>(pDst) = field_26A0_plyr_stats[(u16)slotIdx].field_0_plyr_stage_stats[k][j].field_4_stage_best_score;
            pDst += 4;
            len += 4;
            *reinterpret_cast<s32*>(pDst) = field_26A0_plyr_stats[(u16)slotIdx].field_0_plyr_stage_stats[k][j].field_8_stage_latest_score;
            pDst += 4;
            len += 4;
        }
    }

    File::WriteBufferToFile_4A6E80(FileName, this, &len);
}

// https://decomp.me/scratch/oIJET
MATCH_FUNC(0x56BB10)
void PlyDat_2BC0::UpdateStageScore_56BB10(Player* pPlayer)
{
    const u8 slot_idx = gGameSession_67E8E0.GetPlySlotIdx_4C59B0();
    PlySlot_A4* pPlayerStats = &field_26A0_plyr_stats[slot_idx];
    u8 map_num;
    u8 bonus_num;
    if (!gGameSession_67E8E0.IsBonusStage_4C59A0())
    {
        map_num = gGameSession_67E8E0.GetMainStageIdx_4C5980();
        bonus_num = 0;
    }
    else
    {
        gGameSession_67E8E0.DecodeStage_453A60(gGameSession_67E8E0.GetStage_4C5990(), &map_num, &bonus_num);
    }

    StageStats_C* pStageStats = &pPlayerStats->field_0_plyr_stage_stats[map_num][bonus_num];
    const u32 latest_score = pPlayer->field_2D4_scores.GetScore_592370();
    if (latest_score > pStageStats->field_4_stage_best_score)
    {
        pStageStats->field_4_stage_best_score = latest_score;
    }
    pStageStats->field_8_stage_latest_score = latest_score;
    SavePlySlotDat_56BA60(slot_idx);
}

MATCH_FUNC(0x56BBD0)
void PlyDat_2BC0::UnlockStage_56BBD0(u8 map_num, u8 bonus_num)
{
    const u8 slot_idx = gGameSession_67E8E0.GetPlySlotIdx_4C59B0();
    this->field_26A0_plyr_stats[slot_idx].field_0_plyr_stage_stats[map_num][bonus_num].field_0_is_stage_unlocked = 1;
    if (!bStartNetworkGame_7081F0)
    {
        SavePlySlotDat_56BA60(slot_idx);
    }
}

MATCH_FUNC(0x56BC40)
void PlyDat_2BC0::UnlockAllStages_56BC40()
{
    const u8 slot_idx = gGameSession_67E8E0.GetPlySlotIdx_4C59B0();
    PlySlot_A4* pStats = &this->field_26A0_plyr_stats[slot_idx];
    for (s32 k3 = 0; k3 < 3; k3++)
    {
        for (s32 k4 = 0; k4 < 4; k4++)
        {
            pStats->field_0_plyr_stage_stats[k3][k4].field_0_is_stage_unlocked = 1;
        }
    }

    if (!bStartNetworkGame_7081F0)
    {
        SavePlySlotDat_56BA60(slot_idx);
    }
}

// =====================================================================

MATCH_FUNC(0x56BCA0)
char_type PlyDat_2BC0::HiScoreHscExists_56BCA0()
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

MATCH_FUNC(0x56BCF0)
void PlyDat_2BC0::GetHiScoreHscFileName_56BCF0(char_type* pName)
{
    strcpy(pName, "player\\hiscores.hsc");
}

MATCH_FUNC(0x56BD20)
void PlyDat_2BC0::InitAltHiScores_56BD20()
{
    wcsncpy(field_24C0_alt_scores.field_0_score_table_line[0].field_0_player_name, L"ALAN", 9u);
    field_24C0_alt_scores.field_0_score_table_line[0].field_14_score = 1000000;
    wcsncpy(field_24C0_alt_scores.field_0_score_table_line[1].field_0_player_name, L"BRIAN", 9u);
    field_24C0_alt_scores.field_0_score_table_line[1].field_14_score = 500000;
    wcsncpy(field_24C0_alt_scores.field_0_score_table_line[2].field_0_player_name, L"COLIN", 9u);
    field_24C0_alt_scores.field_0_score_table_line[2].field_14_score = 400000;
    wcsncpy(field_24C0_alt_scores.field_0_score_table_line[3].field_0_player_name, L"DAVE", 9u);
    field_24C0_alt_scores.field_0_score_table_line[3].field_14_score = 300000;
    wcsncpy(field_24C0_alt_scores.field_0_score_table_line[4].field_0_player_name, L"ERIC", 9u);
    field_24C0_alt_scores.field_0_score_table_line[4].field_14_score = 250000;
    wcsncpy(field_24C0_alt_scores.field_0_score_table_line[5].field_0_player_name, L"FRANK", 9u);
    field_24C0_alt_scores.field_0_score_table_line[5].field_14_score = 200000;
    wcsncpy(field_24C0_alt_scores.field_0_score_table_line[6].field_0_player_name, L"GRAEME", 9u);
    field_24C0_alt_scores.field_0_score_table_line[6].field_14_score = 100000;
    wcsncpy(field_24C0_alt_scores.field_0_score_table_line[7].field_0_player_name, L"HECTOR", 9u);
    field_24C0_alt_scores.field_0_score_table_line[7].field_14_score = 50000;
    wcsncpy(field_24C0_alt_scores.field_0_score_table_line[8].field_0_player_name, L"IMOGEN", 9u);
    field_24C0_alt_scores.field_0_score_table_line[8].field_14_score = 25000;
    wcsncpy(field_24C0_alt_scores.field_0_score_table_line[9].field_0_player_name, L"JACKSON", 9u);
    field_24C0_alt_scores.field_0_score_table_line[9].field_14_score = 10000;
}

MATCH_FUNC(0x56BE50)
void PlyDat_2BC0::LoadHiScores_56BE50()
{
    char_type FileName[256];
    GetHiScoreHscFileName_56BCF0(FileName);
    File::Global_Open_4A7060(FileName);

    File::Global_Read_4A71C0(&field_23D0_total_scores.field_0_score_table_line, 240);

    for (s32 k3Counter = 0; k3Counter < 3; k3Counter++)
    {
        for (s32 k4Counter = 0; k4Counter < 4; k4Counter++)
        {
            File::Global_Read_4A71C0(&field_1890_stage_scores[k3Counter][k4Counter], 240);
        }

        File::Global_Read_4A71C0(&field_1800_best_stats[k3Counter], 40); // 3 40 byte objs

        File::Global_Read_4A71C0(&field_1878_best_car_damage_cost[k3Counter], 4);

        File::Global_Read_4A71C0(&field_1884_best_evasion_rating[k3Counter], 4);
    }

    File::Global_Close_4A70C0();
}

MATCH_FUNC(0x56BF20)
void PlyDat_2BC0::SaveHiScores_56BF20()
{
    char_type FileName[256];
    size_t len;

    GetHiScoreHscFileName_56BCF0(FileName);

    // The start of this object is reused as the buffer for the high score file
    u8* pDst = reinterpret_cast<u8*>(this);
    memcpy(pDst, &field_23D0_total_scores, sizeof(HighScoreTable_F0));
    pDst += sizeof(HighScoreTable_F0);
    len = sizeof(HighScoreTable_F0);

    for (s32 k = 0; k < 3; k++)
    {
        for (s32 j = 0; j < 4; j++)
        {
            memcpy(pDst, &field_1890_stage_scores[k][j], sizeof(HighScoreTable_F0));
            pDst += sizeof(HighScoreTable_F0);
            len += sizeof(HighScoreTable_F0);
        }

        memcpy(pDst, &field_1800_best_stats[k], sizeof(BestStageStats_28));
        pDst += sizeof(BestStageStats_28);
        len += sizeof(BestStageStats_28);

        *reinterpret_cast<s32*>(pDst) = field_1878_best_car_damage_cost[k];
        pDst += 4;
        len += 4;

        *reinterpret_cast<s32*>(pDst) = field_1884_best_evasion_rating[k];
        pDst += 4;
        len += 4;
    }

    File::WriteBufferToFile_4A6E80(FileName, this, &len);
}

// TODO: logic matches, only register allocation differs
MATCH_FUNC(0x56C010)
void PlyDat_2BC0::UpdateHiScores_56C010()
{
    u8 map_num;
    u8 bonus_num;
    char_type bBestStatsChanged = 0;

    if (!gGameSession_67E8E0.IsBonusStage_4C59A0())
    {
        map_num = gGameSession_67E8E0.GetMainStageIdx_4C5980();
        bonus_num = 0;
    }
    else
    {
        gGameSession_67E8E0.DecodeStage_453A60(gGameSession_67E8E0.GetStage_4C5990(), &map_num, &bonus_num);
    }

    PlySlot_A4* pPlayerStats = &field_26A0_plyr_stats[gGameSession_67E8E0.GetPlySlotIdx_4C59B0()];
    s32 latest = pPlayerStats->field_0_plyr_stage_stats[map_num][bonus_num].field_8_stage_latest_score;
    const char_type bNewStageScore =
        field_1890_stage_scores[map_num][bonus_num].InsertScore_56B550(pPlayerStats->field_90_strPlayerName, latest);
    const char_type bNewTotalScore = field_23D0_total_scores.InsertScore_56B550(pPlayerStats->field_90_strPlayerName, pPlayerStats->GetTotalBestScore_56B6B0());

    if (!bonus_num)
    {
        u32* pBestStats = reinterpret_cast<u32*>(&field_1800_best_stats[map_num]);
        for (u8 i = 0; i < 10; i++, pBestStats++)
        {
            const u32 value = gGameSession_67E8E0.GetStatistic_4C59F0(i);
            if (value > *pBestStats)
            {
                *pBestStats = value;
                bBestStatsChanged = 1;
            }
        }

        const u32 value1 = gGameSession_67E8E0.GetCarDamageCost_4C5A80();
        if (value1 > (u32)field_1878_best_car_damage_cost[map_num])
        {
            field_1878_best_car_damage_cost[map_num] = value1;
            bBestStatsChanged = 1;
        }

        const u32 value2 = gGameSession_67E8E0.GetEvasionRating_4C5AA0();
        if (value2 > (u32)field_1884_best_evasion_rating[map_num])
        {
            field_1884_best_evasion_rating[map_num] = value2;
            bBestStatsChanged = 1;
        }
    }

    if (bNewStageScore || bNewTotalScore || bBestStatsChanged)
    {
        SaveHiScores_56BF20();
    }
}

MATCH_FUNC(0x56C1D0)
void PlyDat_2BC0::InitDefaultHiScores_56C1D0()
{
    field_23D0_total_scores.Init_56B520();
    ScoreTableLine_18* p10StruIter = gDefaultHiScores_6242B0;
    for (s32 k10 = 0; k10 < 10; k10++)
    {
        field_23D0_total_scores.InsertScore_56B550(p10StruIter->field_0_player_name, p10StruIter->field_14_score);
        ++p10StruIter;
    }

    ScoreTableLine_18* pSruIter = gDefaultStageHiScores_6243A0;
    for (s32 k3 = 0; k3 < 3; k3++)
    {
        for (s32 k4 = 0; k4 < 4; k4++)
        {
            field_1890_stage_scores[k3][k4].Init_56B520();
            for (s32 k10 = 0; k10 < 10; k10++)
            {
                field_1890_stage_scores[k3][k4].InsertScore_56B550(pSruIter->field_0_player_name, pSruIter->field_14_score);
                ++pSruIter;
            }
        }
    }
}

MATCH_FUNC(0x56C250)
void PlyDat_2BC0::DoMuchCashCheat_56C250()
{
    if (!bStartNetworkGame_7081F0)
    {
        PlySlot_A4* pStats = &this->field_26A0_plyr_stats[gGameSession_67E8E0.GetPlySlotIdx_4C59B0()];
        if (wcscmp(pStats->field_90_strPlayerName, L"MUCHCASH") == 0)
        {
            Player* pPlayer = gGame_0x40_67E008->field_38_orf1;
            pPlayer->field_2D4_scores.AddCash_592620(pPlayer->field_6BC_multpliers.field_0_value * 500000);
        }
    }
}

MATCH_FUNC(0x56C2C0)
void PlyDat_2BC0::Create_56C2C0()
{
    if (!gPlyDat_6FEAC0)
    {
        gPlyDat_6FEAC0 = new PlyDat_2BC0();
        if (!gPlyDat_6FEAC0)
        {
            FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\plydat.cpp", 1269);
        }
    }
}

MATCH_FUNC(0x56C340)
void PlyDat_2BC0::Destroy_56C340()
{
    if (gPlyDat_6FEAC0)
    {
        delete gPlyDat_6FEAC0;
        gPlyDat_6FEAC0 = 0;
    }
}