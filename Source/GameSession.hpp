#pragma once

#include "Function.hpp"
#include "Game.hpp"
#include "multiplayer_game_type.hpp"
#include <windows.h>

struct FragsByVictim
{
    s16 mFragsOnVictim[MAX_PLAYERS]; // 0x0
};
GTA2_ASSERT_SIZEOF_ALWAYS(FragsByVictim, 0xC);

struct PlayerName
{
    wchar_t mName[16]; // 0x0
};
GTA2_ASSERT_SIZEOF_ALWAYS(PlayerName, 0x20);

class Player;

struct GameSession
{
    // 9.6f 0x453A80
    inline s32 get_secret_tokens_collected_453A80() const
    {
        return mSecretTokensCollected;
    }

    // 9.6f 0x476B10
    inline void set_secret_tokens_collected_476B10(s32 count)
    {
        mSecretTokensCollected = count;
    }

    // 9.6f 0x434A10
    inline void IncSecretTokensCollected_434A10()
    {
        mSecretTokensCollected++;
    }

    // 9.6f 0x453A40
    inline u8 EncodeStage_453A40(u8 main_stage_idx, u8 bonus_stage_idx)
    {
        main_stage_idx <<= 4;
        main_stage_idx |= bonus_stage_idx;
        return main_stage_idx;
    }

    // 9.6f 0x453A60
    inline void DecodeStage_453A60(u8 stage, u8* pMainStageIdx, u8* pBonusStageIdx)
    {
        *pMainStageIdx = stage >> 4;
        *pBonusStageIdx = stage & 0xF;
    }

    char_type mMapName[256]; // 0x0
    char_type mStyleName[256]; // 0x100
    char_type mScriptName[256]; // 0x200
    char_type mSaveFileName[256]; // 0x300
    char_type mMainStage; // 0x400
    char_type mStage; // 0x401
    char_type mBonusStage; // 0x402
    u8 mPlayerSlotIdx; // 0x403
    char_type mLevelFinishBonusType; // 0x404
    /*
    statistics:
    field_408[5] = vehicles hijacked
    field_408[6] = civilians run down
    field_408[7] = civilians murdered
    field_408[8] = lawmen killed
    field_408[9] = gang members killed
    */
    s32 mStatistics[10]; // 0x408
    s32 mCarDamageCost; // 0x430
    s32 mEvasionRating; // 0x434
    s16 mBonusRatingTextIdx; // 0x438
    bool mbStartedFromPlayBonusMenu; // 0x43A
    char_type mGameType; // 0x43B
    s32 mPointsLimit; // 0x43C
    char_type mUserPlayerIdx; // 0x440
    char_type mMaxPlayers; // 0x441
    char_type mWinnerPlayerIdx; // 0x442
    s32 mGameTimeLimit; // 0x444
    FragsByVictim mFragsByVictim[MAX_PLAYERS]; // 0x448
    s16 mFragsList[MAX_PLAYERS]; // 0x490
    s32 mPointsList[MAX_PLAYERS]; // 0x49C
    PlayerName mPlayerNames[MAX_PLAYERS]; // 0x4B4
    s32 mSecretTokensCollected; // 0x574

    // 9.6f 0x45B420
    EXPORT void clear_secret_tokens_collected()
    {
        mSecretTokensCollected = 0;
    }

    inline s32 GetTimeLimit_461DC0() const
    {
        return mGameTimeLimit;
    }

    EXPORT GameSession();

    EXPORT void LoadDebugSettings();

    EXPORT char_type* SetMapName(char_type* pName);

    EXPORT char_type* SetStyleName(char_type* pName);

    EXPORT char_type* SetScriptName(char_type* pName);

    EXPORT char_type* SetSaveFileName(char_type* pStr);

    EXPORT void SetMainStageIdx(char_type main_stage);

    EXPORT void SetStage(char_type stage);

    EXPORT void SetBonusStage(char_type bonus_stage);

    EXPORT void SetPlySlotIdx(char_type player_slot_idx);

    EXPORT void SetLevelFinishBonusType(char_type bonus_type);

    EXPORT char_type* GetMapName();

    EXPORT char_type* GetStyleName();

    EXPORT char_type* GetScriptName();

    EXPORT char_type* GetSaveFileName_4C5970();

    EXPORT char_type GetMainStageIdx() const;

    EXPORT char_type GetStage() const;

    EXPORT char_type IsBonusStage() const;

    EXPORT u8 GetPlySlotIdx() const;

    EXPORT char_type GetLevelFinishBonusType() const;

    EXPORT void SetStatistic(u8 idx, s32 value);

    EXPORT s32 GetStatistic(u8 idx) const;

    EXPORT void StoreCrimeStats(Player* pPlayer);

    EXPORT void SetCarDamageCost(s32 cost);

    EXPORT s32 GetCarDamageCost() const;

    EXPORT void SetEvasionRating(s32 rating);

    EXPORT s32 GetEvasionRating() const;

    EXPORT void SetBonusRatingTextIdx(s16 gxt_text_idx);

    EXPORT s16 GetBonusRatingTextIdx() const;

    EXPORT void SetStartedFromPlayBonusMenu(bool bStarted);

    EXPORT bool IsStartedFromPlayBonusMenu() const;

    EXPORT void init();

    EXPORT void SetMultiplayerParams(char_type game_type, s32 points_limit, char_type user_idx, char_type max_players, s32 time_limit);

    EXPORT u8 GetMultiplayerGamemode() const;

    EXPORT s32 GetMultiplayerPointsLimit() const;

    EXPORT char_type GetUserPlayerIdx() const;

    EXPORT char_type GetMaxPlayers() const;

    EXPORT void SetWinnerIdx(char_type player_idx);

    EXPORT char_type GetWinnerIdx() const;

    EXPORT void SetPlayerName(u16 player_idx, wchar_t* pName);

    EXPORT PlayerName* GetPlayerName(u16 player_idx);

    EXPORT void ChangePointsForPlayerIdxByAmount(u8 player_idx, s32 amount);

    EXPORT s32 GetPointsForPlayerIdx(u8 player_idx) const;

    EXPORT void UpdateFrags(u8 player_killer_idx, u8 player_victim_idx);

    EXPORT u16 GetFragsForPlayerIdx(u8 player_idx) const;

    EXPORT s16 GetFragsOnPlayer(u8 player_killer_idx, u8 player_victim_idx) const;
};
GTA2_ASSERT_SIZEOF_ALWAYS(GameSession, 0x578);

EXTERN_GLOBAL(GameSession, gGameSession_67E8E0);

// 9.6f 0x434B20
inline bool IsTagGame_434B20()
{
    return gGameSession_67E8E0.GetMultiplayerGamemode() == TAG_GAME_3;
}
