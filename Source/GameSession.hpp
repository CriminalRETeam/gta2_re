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

    EXPORT void LoadDebugSettings_4C53D0();

    EXPORT char_type* SetMapName_4C5870(char_type* pName);

    EXPORT char_type* SetStyleName_4C5890(char_type* pName);

    EXPORT char_type* SetScriptName_4C58B0(char_type* pName);

    EXPORT char_type* SetSaveFileName_4C58D0(char_type* pStr);

    EXPORT void SetMainStageIdx_4C58F0(char_type main_stage);

    EXPORT void SetStage_4C5900(char_type stage);

    EXPORT void SetBonusStage_4C5910(char_type bonus_stage);

    EXPORT void SetPlySlotIdx_4C5920(char_type player_slot_idx);

    EXPORT void SetLevelFinishBonusType_4C5930(char_type bonus_type);

    EXPORT char_type* GetMapName_4C5940();

    EXPORT char_type* GetStyleName_4C5950();

    EXPORT char_type* GetScriptName_4C5960();

    EXPORT char_type* GetSaveFileName_4C5970();

    EXPORT char_type GetMainStageIdx_4C5980() const;

    EXPORT char_type GetStage_4C5990() const;

    EXPORT char_type IsBonusStage_4C59A0() const;

    EXPORT u8 GetPlySlotIdx_4C59B0() const;

    EXPORT char_type GetLevelFinishBonusType_4C59C0() const;

    EXPORT void SetStatistic_4C59D0(u8 idx, s32 value);

    EXPORT s32 GetStatistic_4C59F0(u8 idx) const;

    EXPORT void StoreCrimeStats_4C5A10(Player* pPlayer);

    EXPORT void SetCarDamageCost_4C5A70(s32 cost);

    EXPORT s32 GetCarDamageCost_4C5A80() const;

    EXPORT void SetEvasionRating_4C5A90(s32 rating);

    EXPORT s32 GetEvasionRating_4C5AA0() const;

    EXPORT void SetBonusRatingTextIdx_4C5AB0(s16 gxt_text_idx);

    EXPORT s16 GetBonusRatingTextIdx_4C5AC0() const;

    EXPORT void SetStartedFromPlayBonusMenu_4C5AD0(bool bStarted);

    EXPORT bool IsStartedFromPlayBonusMenu_4C5AE0() const;

    EXPORT void init_4C5AF0();

    EXPORT void SetMultiplayerParams_4C5B80(char_type game_type, s32 points_limit, char_type user_idx, char_type max_players, s32 time_limit);

    EXPORT u8 GetMultiplayerGamemode_4C5BC0() const;

    EXPORT s32 GetMultiplayerPointsLimit_4C5BD0() const;

    EXPORT char_type GetUserPlayerIdx_4C5BE0() const;

    EXPORT char_type GetMaxPlayers_4C5BF0() const;

    EXPORT void SetWinnerIdx_4C5C00(char_type player_idx);

    EXPORT char_type GetWinnerIdx_4C5C20() const;

    EXPORT void SetPlayerName_4C5C30(u16 player_idx, wchar_t* pName);

    EXPORT PlayerName* GetPlayerName_4C5C60(u16 player_idx);

    EXPORT void ChangePointsForPlayerIdxByAmount_4C5C80(u8 player_idx, s32 amount);

    EXPORT s32 GetPointsForPlayerIdx_4C5CB0(u8 player_idx) const;

    EXPORT void UpdateFrags_4C5CD0(u8 player_killer_idx, u8 player_victim_idx);

    EXPORT u16 GetFragsForPlayerIdx_4C5D60(u8 player_idx) const;

    EXPORT s16 GetFragsOnPlayer_4C5D80(u8 player_killer_idx, u8 player_victim_idx) const;
};
GTA2_ASSERT_SIZEOF_ALWAYS(GameSession, 0x578);

EXTERN_GLOBAL(GameSession, gGameSession_67E8E0);

// 9.6f 0x434B20
inline bool IsTagGame_434B20()
{
    return gGameSession_67E8E0.GetMultiplayerGamemode_4C5BC0() == TAG_GAME_3;
}
