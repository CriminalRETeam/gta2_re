#pragma once

#include "Function.hpp"
#include <windows.h>

class Player;

// Progress of a player slot in one stage
struct StageStats_C
{
    char_type field_0_is_stage_unlocked;
    u32 field_4_stage_best_score;
    s32 field_8_stage_latest_score;
};

// One player slot: name and the stats of the 3 main stages x 4 (main + 3 bonus) stages
struct PlySlot_A4
{
    StageStats_C field_0_plyr_stage_stats[3][4];
    wchar_t field_90_strPlayerName[9];

    EXPORT PlySlot_A4();
    EXPORT ~PlySlot_A4();

    EXPORT void ResetPlayerSlot_56B630();
    EXPORT s32 GetTotalLatestScore_56B680();
    EXPORT s32 GetTotalBestScore_56B6B0();
};

// One line (name, score) of a high score table
struct ScoreTableLine_18
{
    wchar_t field_0_player_name[10];
    u32 field_14_score;
};

// Top 10 scores
struct HighScoreTable_F0
{
    EXPORT HighScoreTable_F0();
    EXPORT ~HighScoreTable_F0();

    EXPORT void Init_56B520();

    EXPORT char_type InsertScore_56B550(const wchar_t* pFindStr, s32 findScore);

    ScoreTableLine_18 field_0_score_table_line[10];
};

// The best of each of the 10 statistics (GameSession statistics) in a main stage
struct BestStageStats_28
{
    BYTE field_0[40];
};

// Player profile and high score data (plydat.cpp): the 8 player slots (player\plyslotN.dat) and the high score tables
// (player\hiscores.hsc)
struct PlyDat_2BC0
{
    u8 field_0_file_buffer[0x1800]; // the start of the object is reused as the buffer when saving the files
    BestStageStats_28 field_1800_best_stats[3];
    s32 field_1878_best_car_damage_cost[3];
    s32 field_1884_best_evasion_rating[3];
    HighScoreTable_F0 field_1890_stage_scores[3][4];
    HighScoreTable_F0 field_23D0_total_scores;
    HighScoreTable_F0 field_24C0_alt_scores;
    HighScoreTable_F0 field_25B0;
    PlySlot_A4 field_26A0_plyr_stats[8];

    EXPORT PlyDat_2BC0();
    EXPORT ~PlyDat_2BC0();

    // todo: ordering
    EXPORT void UpdateStageScore_56BB10(Player* pPlayer);

    EXPORT void UpdateHiScores_56C010();

    EXPORT void GetPlySlotDatName_56B8A0(u16 slot_idx, char_type* pName);

    EXPORT char_type PlySlotDatExists_56B940(s32 slot_idx);

    EXPORT void GetHiScoreHscFileName_56BCF0(char_type* pName);

    EXPORT char_type HiScoreHscExists_56BCA0();

    EXPORT void LoadPlySlotDat_56B990(u16 slotIdx);

    EXPORT void SavePlySlotDat_56BA60(s16 slotIdx);

    EXPORT void LoadHiScores_56BE50();

    EXPORT void InitDefaultHiScores_56C1D0();

    EXPORT void SaveHiScores_56BF20();

    EXPORT void InitAltHiScores_56BD20();

    EXPORT void UnlockStage_56BBD0(u8 map_num, u8 bonus_num);

    EXPORT void UnlockAllStages_56BC40();

    EXPORT void DoMuchCashCheat_56C250();
    EXPORT static void Create_56C2C0();
    EXPORT static void Destroy_56C340();
};

EXTERN_GLOBAL(PlyDat_2BC0*, gPlyDat_6FEAC0);
