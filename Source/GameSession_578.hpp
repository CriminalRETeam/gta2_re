#pragma once

#include "Function.hpp"
#include "multiplayer_game_type.hpp"
#include <windows.h>

struct FragsByVictim_C
{
    s16 field_0_frags_on_victim[MAX_PLAYERS];
};

struct PlayerName_20
{
    wchar_t field_0_name[16];
};

class Player;

struct GameSession_578
{
    // 9.6f 0x453A80
    inline s32 get_secret_tokens_collected_453A80()
    {
        return field_574_secret_tokens_collected;
    }

    // 9.6f 0x476B10
    inline void set_secret_tokens_collected_476B10(s32 count)
    {
        field_574_secret_tokens_collected = count;
    }

    // 9.6f 0x434A10
    inline void IncSecretTokensCollected_434A10()
    {
        field_574_secret_tokens_collected++;
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

    char_type field_0_map_name[256];
    char_type field_100_style_name[256];
    char_type field_200_script_name[256];
    char_type field_300_debug_str[256];
    char_type field_400_main_stage;
    char_type field_401_stage;
    char_type field_402_bonus_stage;
    u8 field_403_player_slot_idx;
    char_type field_404_level_finish_bonus_type;
    /*
    statistics:
    field_408[5] = vehicles hijacked
    field_408[6] = civilians run down
    field_408[7] = civilians murdered
    field_408[8] = lawmen killed
    field_408[9] = gang members killed
    */
    s32 field_408_statistics[10];
    s32 field_430_car_damage_cost;
    s32 field_434_evasion_rating;
    s16 field_438_bonus_rating_text_idx;
    bool field_43A_bStartedFromPlayBonusMenu;
    char_type field_43B_game_type;
    s32 field_43C_points_limit;
    char_type field_440_user_player_idx;
    char_type field_441_max_players;
    char_type field_442_winner_player_idx;
    s32 field_444_game_time_limit;
    FragsByVictim_C field_448_frags_by_victim[MAX_PLAYERS];
    s16 field_490_frags_list[MAX_PLAYERS];
    s32 field_49C_points_list[MAX_PLAYERS];
    PlayerName_20 field_4B4_player_names[MAX_PLAYERS];
    s32 field_574_secret_tokens_collected;

    // inlined at 45b420 in 9.6f
    // 9.6f 0x45B420
    EXPORT void clear_secret_tokens_collected()
    {
        field_574_secret_tokens_collected = 0;
    }

    inline s32 GetTimeLimit_461DC0()
    {
        return field_444_game_time_limit;
    }

    EXPORT void LoadDebugSettings_4C53D0();

    EXPORT char* SetMapName_4C5870(char_type* pName);

    EXPORT char* SetStyleName_4C5890(char_type* pName);

    EXPORT char* SetScriptName_4C58B0(char_type* pName);

    EXPORT char_type* DebugStr_4C58D0(char_type* pStr);

    EXPORT void SetMainStageIdx_4C58F0(char_type main_stage);

    EXPORT void SetStage_4C5900(char_type stage);

    EXPORT void SetBonusStage_4C5910(char_type bonus_stage);

    EXPORT void SetPlySlotIdx_4C5920(char_type player_slot_idx);

    EXPORT void SetLevelFinishBonusType_4C5930(char_type bonus_type);

    EXPORT char_type* GetMapName_4C5940();

    EXPORT char_type* GetStyleName_4C5950();

    EXPORT char_type* GetScriptName_4C5960();

    EXPORT char_type* GetDebugStr_4C5970();

    EXPORT char_type GetMainStageIdx_4C5980();

    EXPORT char_type GetStage_4C5990();

    EXPORT char_type IsBonusStage_4C59A0();

    EXPORT u8 GetPlySlotIdx_4C59B0();

    EXPORT char_type GetLevelFinishBonusType_4C59C0();

    EXPORT void SetStatistic_4C59D0(u8 idx, s32 value);

    EXPORT s32 GetStatistic_4C59F0(u8 idx);

    EXPORT void StoreCrimeStats_4C5A10(Player* pPlayer);

    EXPORT void SetCarDamageCost_4C5A70(s32 cost);

    EXPORT s32 GetCarDamageCost_4C5A80();

    EXPORT void SetEvasionRating_4C5A90(s32 rating);

    EXPORT s32 GetEvasionRating_4C5AA0();

    EXPORT void SetBonusRatingTextIdx_4C5AB0(s16 gxt_text_idx);

    EXPORT s16 GetBonusRatingTextIdx_4C5AC0();

    EXPORT void SetStartedFromPlayBonusMenu_4C5AD0(bool bStarted);

    EXPORT bool IsStartedFromPlayBonusMenu_4C5AE0();

    EXPORT void init_4C5AF0();

    EXPORT void SetMultiplayerParams_4C5B80(char_type game_type, s32 points_limit, char_type user_idx, char_type max_players, s32 time_limit);

    EXPORT u8 GetMultiplayerGamemode_4C5BC0();

    EXPORT s32 GetMultiplayerPointsLimit_4C5BD0();

    EXPORT char_type GetUserPlayerIdx_4C5BE0();

    EXPORT char_type GetMaxPlayers_4C5BF0();

    EXPORT void SetWinnerIdx_4C5C00(char_type player_idx);

    EXPORT char_type GetWinnerIdx_4C5C20();

    EXPORT void SetPlayerName_4C5C30(u16 player_idx, wchar_t* pName);

    EXPORT PlayerName_20* GetPlayerName_4C5C60(u16 player_idx);

    EXPORT void ChangePointsForPlayerIdxByAmount_4C5C80(u8 player_idx, s32 amount);

    EXPORT s32 GetPointsForPlayerIdx_4C5CB0(u8 player_idx);

    EXPORT void UpdateFrags_4C5CD0(u8 player_killer_idx, u8 player_victim_idx);

    EXPORT u16 GetFragsForPlayerIdx_4C5D60(u8 player_idx);

    EXPORT s16 GetFragsOnPlayer_4C5D80(u8 player_killer_idx, u8 player_victim_idx);
};

EXTERN_GLOBAL(GameSession_578, gGameSession_67E8E0);

// 9.6f 0x434B20
inline bool IsTagGame_434B20()
{
    return gGameSession_67E8E0.GetMultiplayerGamemode_4C5BC0() == TAG_GAME_3;
}
