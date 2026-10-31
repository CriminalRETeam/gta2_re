#include "GameSession.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "Globals.hpp"
#include "Player.hpp"
#include "registry.hpp"

DEFINE_GLOBAL(GameSession, gGameSession, 0x67E8E0);
EXTERN_GLOBAL_ARRAY(wchar_t, gEmptyWStr, 2);

MATCH_FUNC(0x4C52D0)
GameSession::GameSession()
{
    strcpy(mMapName, "");
    strcpy(mStyleName, "");
    strcpy(mScriptName, "");
    strcpy(mSaveFileName, "");
    mMainStage = 0;
    mStage = 0;
    mBonusStage = 0;
    mPlayerSlotIdx = 0;
    mLevelFinishBonusType = 0;

    for (s32 i = 0; i < GTA2_COUNTOF(mStatistics); i++)
    {
        mStatistics[i] = 0;
    }

    mCarDamageCost = 0;
    mEvasionRating = 0;
    mBonusRatingTextIdx = 0;
    mbStartedFromPlayBonusMenu = false;

    init();
}

MATCH_FUNC(0x4C53D0)
void GameSession::LoadDebugSettings()
{
    char_type path[256];
    char_type reg_value[256];

    strcpy(reg_value, "");
    strcpy(path, "data\\");
    gRegistry.Get_Debug_Setting("mapname", reinterpret_cast<LPBYTE>(reg_value), GTA2_COUNTOF(reg_value));
    strcat(path, reg_value);
    if (strcmp(path, "data\\") == 0)
    {
        strcpy(path, "data\\jointmap.gmp");
    }
    gGameSession.SetMapName(path);

    strcpy(reg_value, "");
    strcpy(path, "data\\");
    gRegistry.Get_Debug_Setting("stylename", reinterpret_cast<LPBYTE>(reg_value), GTA2_COUNTOF(reg_value));
    strcat(path, reg_value);
    if (strcmp(path, "data\\") == 0)
    {
        strcpy(path, "data\\style.sty");
    }
    gGameSession.SetStyleName(path);

    strcpy(reg_value, "");
    strcpy(path, "data\\");
    gRegistry.Get_Debug_Setting("scriptname", reinterpret_cast<LPBYTE>(reg_value), GTA2_COUNTOF(reg_value));
    strcat(path, reg_value);
    if (strcmp(path, "data\\") == 0)
    {
        strcpy(path, "data\\q.scr");
    }
    gGameSession.SetScriptName(path);

    strcpy(reg_value, "");
    strcpy(path, "player\\");
    gRegistry.Get_Debug_Setting("savename", reinterpret_cast<LPBYTE>(reg_value), GTA2_COUNTOF(reg_value));
    strcat(path, reg_value);
    if (strcmp(path, "player\\") == 0)
    {
        strcpy(path, "");
    }

    gGameSession.SetSaveFileName(path);
    gGameSession.SetMainStageIdx(0);
    gGameSession.SetStage(0);
    gGameSession.SetBonusStage(0);
    gGameSession.SetPlySlotIdx(0);
    gGameSession.SetLevelFinishBonusType(0);

    for (s32 i = 0; i < GTA2_COUNTOF(mStatistics); i++)
    {
        mStatistics[i] = 0;
    }

    mCarDamageCost = 0;
    mEvasionRating = 0;
    mBonusRatingTextIdx = 0;
    mbStartedFromPlayBonusMenu = false;

    init();
}

MATCH_FUNC(0x4C5870)
char_type* GameSession::SetMapName(char_type* pName)
{
    return strncpy(mMapName, pName, 255u);
}

MATCH_FUNC(0x4C5890)
char_type* GameSession::SetStyleName(char_type* pName)
{
    return strncpy(mStyleName, pName, 0xFFu);
}

MATCH_FUNC(0x4C58B0)
char_type* GameSession::SetScriptName(char_type* pName)
{
    return strncpy(mScriptName, pName, 0xFFu);
}

MATCH_FUNC(0x4C58D0)
char_type* GameSession::SetSaveFileName(char_type* pStr)
{
    return strncpy(mSaveFileName, pStr, 0xFFu);
}

MATCH_FUNC(0x4C58F0)
void GameSession::SetMainStageIdx(char_type main_stage)
{
    mMainStage = main_stage;
}

MATCH_FUNC(0x4C5900)
void GameSession::SetStage(char_type stage)
{
    mStage = stage;
}

MATCH_FUNC(0x4C5910)
void GameSession::SetBonusStage(char_type bonus_stage)
{
    mBonusStage = bonus_stage;
}

MATCH_FUNC(0x4C5920)
void GameSession::SetPlySlotIdx(char_type player_slot_idx)
{
    mPlayerSlotIdx = player_slot_idx;
}

MATCH_FUNC(0x4C5930)
void GameSession::SetLevelFinishBonusType(char_type bonus_type)
{
    mLevelFinishBonusType = bonus_type;
}

MATCH_FUNC(0x4C5940)
char_type* GameSession::GetMapName()
{
    return mMapName;
}

MATCH_FUNC(0x4C5950)
char_type* GameSession::GetStyleName()
{
    return mStyleName;
}

MATCH_FUNC(0x4C5960)
char_type* GameSession::GetScriptName()
{
    return mScriptName;
}

MATCH_FUNC(0x4C5970)
char_type* GameSession::GetSaveFileName_4C5970()
{
    return mSaveFileName;
}

MATCH_FUNC(0x4C5980)
char_type GameSession::GetMainStageIdx() const
{
    return mMainStage;
}

MATCH_FUNC(0x4C5990)
char_type GameSession::GetStage() const
{
    return mStage;
}

MATCH_FUNC(0x4C59A0)
char_type GameSession::IsBonusStage() const
{
    return mBonusStage;
}

MATCH_FUNC(0x4C59B0)
u8 GameSession::GetPlySlotIdx() const
{
    return mPlayerSlotIdx;
}

MATCH_FUNC(0x4C59C0)
char_type GameSession::GetLevelFinishBonusType() const
{
    return mLevelFinishBonusType;
}

MATCH_FUNC(0x4C59D0)
void GameSession::SetStatistic(u8 idx, s32 value)
{
    mStatistics[idx] = value;
}

MATCH_FUNC(0x4C59F0)
s32 GameSession::GetStatistic(u8 idx) const
{
    return mStatistics[idx];
}

MATCH_FUNC(0x4C5A10)
void GameSession::StoreCrimeStats(Player* pPlayer)
{
    for (u8 i = 0; i < crime_stats_type::count_10; i++)
    {
        SetStatistic(i, pPlayer->mCrimeStats.mCrimeCountList[i]);
    }

    SetCarDamageCost(pPlayer->mCrimeStats.mCarDamageCost);
    SetEvasionRating(pPlayer->mCrimeStats.mEvasionRating);
}

MATCH_FUNC(0x4C5A70)
void GameSession::SetCarDamageCost(s32 cost)
{
    mCarDamageCost = cost;
}

MATCH_FUNC(0x4C5A80)
s32 GameSession::GetCarDamageCost() const
{
    return mCarDamageCost;
}

MATCH_FUNC(0x4C5A90)
void GameSession::SetEvasionRating(s32 rating)
{
    mEvasionRating = rating;
}

MATCH_FUNC(0x4C5AA0)
s32 GameSession::GetEvasionRating() const
{
    return mEvasionRating;
}

MATCH_FUNC(0x4C5AB0)
void GameSession::SetBonusRatingTextIdx(s16 gxt_text_idx)
{
    mBonusRatingTextIdx = gxt_text_idx;
}

MATCH_FUNC(0x4C5AC0)
s16 GameSession::GetBonusRatingTextIdx() const
{
    return mBonusRatingTextIdx;
}

MATCH_FUNC(0x4C5AD0)
void GameSession::SetStartedFromPlayBonusMenu(bool bStarted)
{
    mbStartedFromPlayBonusMenu = bStarted;
}

MATCH_FUNC(0x4C5AE0)
bool GameSession::IsStartedFromPlayBonusMenu() const
{
    return mbStartedFromPlayBonusMenu;
}

MATCH_FUNC(0x4C5AF0)
void GameSession::init()
{
    mGameType = 0;
    mPointsLimit = 0;
    mUserPlayerIdx = 0;
    mMaxPlayers = 0;
    mWinnerPlayerIdx = MAX_PLAYERS;

    for (s32 i = 0; i < GTA2_COUNTOF(mFragsList); i++)
    {
        for (s32 j = 0; j < MAX_PLAYERS; j++)
        {
            mFragsByVictim[i].mFragsOnVictim[j] = 0;
        }
        mFragsList[i] = 0;
        mPointsList[i] = 0;
        wcscpy(mPlayerNames[i].mName, gEmptyWStr);
    }
}

MATCH_FUNC(0x4C5B80)
void GameSession::SetMultiplayerParams(char_type game_type, s32 points_limit, char_type user_idx, char_type max_players, s32 time_limit)
{
    mGameType = game_type;
    mPointsLimit = points_limit;
    mUserPlayerIdx = user_idx;
    mMaxPlayers = max_players;
    mGameTimeLimit = time_limit;
}

MATCH_FUNC(0x4C5BC0)
u8 GameSession::GetMultiplayerGamemode() const
{
    return mGameType;
}

MATCH_FUNC(0x4C5BD0)
s32 GameSession::GetMultiplayerPointsLimit() const
{
    return mPointsLimit;
}

MATCH_FUNC(0x4C5BE0)
char_type GameSession::GetUserPlayerIdx() const
{
    return mUserPlayerIdx;
}

MATCH_FUNC(0x4C5BF0)
char_type GameSession::GetMaxPlayers() const
{
    return mMaxPlayers;
}

MATCH_FUNC(0x4C5C00)
void GameSession::SetWinnerIdx(char_type player_idx)
{
    if (mWinnerPlayerIdx == MAX_PLAYERS)
    {
        mWinnerPlayerIdx = player_idx;
    }
}

MATCH_FUNC(0x4C5C20)
char_type GameSession::GetWinnerIdx() const
{
    return mWinnerPlayerIdx;
}

MATCH_FUNC(0x4C5C30)
void GameSession::SetPlayerName(u16 player_idx, wchar_t* pName)
{
    wcsncpy(mPlayerNames[player_idx].mName, pName, 16u);
}

MATCH_FUNC(0x4C5C60)
PlayerName* GameSession::GetPlayerName(u16 player_idx)
{
    return &mPlayerNames[player_idx];
}

MATCH_FUNC(0x4C5C80)
void GameSession::ChangePointsForPlayerIdxByAmount(u8 player_idx, s32 amount)
{
    mPointsList[player_idx] += amount;
}

MATCH_FUNC(0x4C5CB0)
s32 GameSession::GetPointsForPlayerIdx(u8 player_idx) const
{
    return mPointsList[player_idx];
}

MATCH_FUNC(0x4C5CD0)
void GameSession::UpdateFrags(u8 player_killer_idx, u8 player_victim_idx)
{
    mFragsByVictim[player_killer_idx].mFragsOnVictim[player_victim_idx]++;
    Player* pPlayer = gGame_0x40->GetPlayer(player_killer_idx);
    if (player_killer_idx == player_victim_idx)
    {
        // player killed himself
        if (mFragsList[player_killer_idx] > 0)
        {
            mFragsList[player_killer_idx]--;
            pPlayer->mScores.ChangeFragsByAmount(-1);
        }
    }
    else
    {
        mFragsList[player_killer_idx]++;
        pPlayer->mScores.ChangeFragsByAmount(1);
    }
}

MATCH_FUNC(0x4C5D60)
u16 GameSession::GetFragsForPlayerIdx(u8 player_idx) const
{
    return mFragsList[player_idx];
}

MATCH_FUNC(0x4C5D80)
s16 GameSession::GetFragsOnPlayer(u8 player_killer_idx, u8 player_victim_idx) const
{
    return mFragsByVictim[player_killer_idx].mFragsOnVictim[player_victim_idx];
}
