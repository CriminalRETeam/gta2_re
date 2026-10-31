#pragma once

#include "Function.hpp"
#include <windows.h>

class Player;

// Progress of a player slot in one stage
struct StageStats
{
    char_type mIsStageUnlocked; // 0x0
    u32 mStageBestScore; // 0x4
    s32 mStageLatestScore; // 0x8
};
GTA2_ASSERT_SIZEOF_ALWAYS(StageStats, 0xC);

// One player slot: name and the stats of the 3 main stages x 4 (main + 3 bonus) stages
struct PlySlot
{
    StageStats mPlyrStageStats[3][4]; // 0x0
    wchar_t mstrPlayerName[9]; // 0x90

    EXPORT PlySlot();
    EXPORT ~PlySlot();

    EXPORT void ResetPlayerSlot();
    EXPORT s32 GetTotalLatestScore() const;
    EXPORT s32 GetTotalBestScore() const;
};
GTA2_ASSERT_SIZEOF_ALWAYS(PlySlot, 0xA4);

// One line (name, score) of a high score table
struct ScoreTableLine
{
    wchar_t mPlayerName[10]; // 0x0
    u32 mScore; // 0x14
};
GTA2_ASSERT_SIZEOF_ALWAYS(ScoreTableLine, 0x18);

// Top 10 scores
struct HighScoreTable
{
    EXPORT HighScoreTable();
    EXPORT ~HighScoreTable();

    EXPORT void Init();

    EXPORT bool InsertScore(const wchar_t* pFindStr, s32 findScore);

    ScoreTableLine mScoreTableLine[10]; // 0x0
};
GTA2_ASSERT_SIZEOF_ALWAYS(HighScoreTable, 0xF0);

// The best of each of the 10 statistics (GameSession statistics) in a main stage
struct BestStageStats
{
    BYTE mBestStatistics[40]; // 0x0
};
GTA2_ASSERT_SIZEOF_ALWAYS(BestStageStats, 0x28);

// Player profile and high score data (plydat.cpp): the 8 player slots (player\plyslotN.dat) and the high score tables
// (player\hiscores.hsc)
struct PlyDat
{
    u8 mFileBuffer[0x1800]; // 0x0: the start of the object is reused as the buffer when saving the files
    BestStageStats mBestStats[3]; // 0x1800
    s32 mBestCarDamageCost[3]; // 0x1878
    s32 mBestEvasionRating[3]; // 0x1884
    HighScoreTable mStageScores[3][4]; // 0x1890
    HighScoreTable mTotalScores; // 0x23D0
    HighScoreTable mAltScores; // 0x24C0
    HighScoreTable mMergedScores; // 0x25B0
    PlySlot mPlyrStats[8]; // 0x26A0

    EXPORT PlyDat();
    EXPORT ~PlyDat();

    EXPORT void UpdateStageScore(Player* pPlayer);

    EXPORT void UpdateHiScores();

    EXPORT void GetPlySlotDatName(u16 slot_idx, char_type* pName);

    EXPORT bool PlySlotDatExists(s32 slot_idx);

    EXPORT void GetHiScoreHscFileName(char_type* pName);

    EXPORT bool HiScoreHscExists();

    EXPORT void LoadPlySlotDat(u16 slotIdx);

    EXPORT void SavePlySlotDat(s16 slotIdx);

    EXPORT void LoadHiScores();

    EXPORT void BuildMergedHiScores();

    EXPORT void InitDefaultHiScores();

    EXPORT void SaveHiScores();

    EXPORT void InitAltHiScores();

    EXPORT void UnlockStage(u8 map_num, u8 bonus_num);

    EXPORT void UnlockAllStages();

    EXPORT void DoMuchCashCheat();
    EXPORT static void Create();
    EXPORT static void Destroy();
};
GTA2_ASSERT_SIZEOF_ALWAYS(PlyDat, 0x2BC0);

EXTERN_GLOBAL(PlyDat*, gPlyDat_6FEAC0);
