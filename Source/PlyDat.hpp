#pragma once

#include "Function.hpp"
#include <windows.h>

class Player;

// Progress of a player slot in one stage
struct StageStats
{
    char_type field_0_is_stage_unlocked;
    u32 field_4_stage_best_score;
    s32 field_8_stage_latest_score;
};
GTA2_ASSERT_SIZEOF_ALWAYS(StageStats, 0xC);

// One player slot: name and the stats of the 3 main stages x 4 (main + 3 bonus) stages
struct PlySlot
{
    StageStats field_0_plyr_stage_stats[3][4];
    wchar_t field_90_strPlayerName[9];

    EXPORT PlySlot();
    EXPORT ~PlySlot();

    EXPORT void ResetPlayerSlot_56B630();
    EXPORT s32 GetTotalLatestScore_56B680();
    EXPORT s32 GetTotalBestScore_56B6B0();
};
GTA2_ASSERT_SIZEOF_ALWAYS(PlySlot, 0xA4);

// One line (name, score) of a high score table
struct ScoreTableLine
{
    wchar_t field_0_player_name[10];
    u32 field_14_score;
};
GTA2_ASSERT_SIZEOF_ALWAYS(ScoreTableLine, 0x18);

// Top 10 scores
struct HighScoreTable
{
    EXPORT HighScoreTable();
    EXPORT ~HighScoreTable();

    EXPORT void Init_56B520();

    EXPORT bool InsertScore_56B550(const wchar_t* pFindStr, s32 findScore);

    ScoreTableLine field_0_score_table_line[10];
};
GTA2_ASSERT_SIZEOF_ALWAYS(HighScoreTable, 0xF0);

// The best of each of the 10 statistics (GameSession statistics) in a main stage
struct BestStageStats
{
    BYTE field_0_best_statistics[40];
};
GTA2_ASSERT_SIZEOF_ALWAYS(BestStageStats, 0x28);

// Player profile and high score data (plydat.cpp): the 8 player slots (player\plyslotN.dat) and the high score tables
// (player\hiscores.hsc)
struct PlyDat
{
    u8 field_0_file_buffer[0x1800]; // the start of the object is reused as the buffer when saving the files
    BestStageStats field_1800_best_stats[3];
    s32 field_1878_best_car_damage_cost[3];
    s32 field_1884_best_evasion_rating[3];
    HighScoreTable field_1890_stage_scores[3][4];
    HighScoreTable field_23D0_total_scores;
    HighScoreTable field_24C0_alt_scores;
    HighScoreTable field_25B0_merged_scores;
    PlySlot field_26A0_plyr_stats[8];

    EXPORT PlyDat();
    EXPORT ~PlyDat();

    // todo: ordering
    EXPORT void UpdateStageScore_56BB10(Player* pPlayer);

    EXPORT void UpdateHiScores_56C010();

    EXPORT void GetPlySlotDatName_56B8A0(u16 slot_idx, char_type* pName);

    EXPORT bool PlySlotDatExists_56B940(s32 slot_idx);

    EXPORT void GetHiScoreHscFileName_56BCF0(char_type* pName);

    EXPORT bool HiScoreHscExists_56BCA0();

    EXPORT void LoadPlySlotDat_56B990(u16 slotIdx);

    EXPORT void SavePlySlotDat_56BA60(s16 slotIdx);

    EXPORT void LoadHiScores_56BE50();

    EXPORT void BuildMergedHiScores_56C170();

    EXPORT void InitDefaultHiScores_56C1D0();

    EXPORT void SaveHiScores_56BF20();

    EXPORT void InitAltHiScores_56BD20();

    EXPORT void UnlockStage_56BBD0(u8 map_num, u8 bonus_num);

    EXPORT void UnlockAllStages_56BC40();

    EXPORT void DoMuchCashCheat_56C250();
    EXPORT static void Create_56C2C0();
    EXPORT static void Destroy_56C340();
};
GTA2_ASSERT_SIZEOF_ALWAYS(PlyDat, 0x2BC0);

EXTERN_GLOBAL(PlyDat*, gPlyDat_6FEAC0);
