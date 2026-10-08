#include "GameSession_578.hpp"
#include "Function.hpp"
#include "Game_0x40.hpp"
#include "Globals.hpp"
#include "Player.hpp"
#include "registry.hpp"

DEFINE_GLOBAL(GameSession_578, gGameSession_67E8E0, 0x67E8E0);
EXTERN_GLOBAL_ARRAY(wchar_t, gEmptyWStr_67DC8C, 32);

MATCH_FUNC(0x4C53D0)
void GameSession_578::LoadDebugSettings_4C53D0()
{
    char path[256];
    char tmp[256];

    strcpy(tmp, "");
    strcpy(path, "data\\");
    gRegistry_6FF968.Get_Debug_Setting_586E90("mapname", (LPBYTE)tmp, GTA2_COUNTOF(tmp));
    strcat(path, tmp);
    if (strcmp(path, "data\\") == 0)
    {
        strcpy(path, "data\\jointmap.gmp");
    }
    gGameSession_67E8E0.SetMapName_4C5870(path);

    strcpy(tmp, "");
    strcpy(path, "data\\");
    gRegistry_6FF968.Get_Debug_Setting_586E90("stylename", (LPBYTE)tmp, GTA2_COUNTOF(tmp));
    strcat(path, tmp);
    if (strcmp(path, "data\\") == 0)
    {
        strcpy(path, "data\\style.sty");
    }
    gGameSession_67E8E0.SetStyleName_4C5890(path);

    strcpy(tmp, "");
    strcpy(path, "data\\");
    gRegistry_6FF968.Get_Debug_Setting_586E90("scriptname", (LPBYTE)tmp, GTA2_COUNTOF(tmp));
    strcat(path, tmp);
    if (strcmp(path, "data\\") == 0)
    {
        strcpy(path, "data\\q.scr");
    }
    gGameSession_67E8E0.SetScriptName_4C58B0(path);

    strcpy(tmp, "");
    strcpy(path, "player\\");
    gRegistry_6FF968.Get_Debug_Setting_586E90("savename", (LPBYTE)tmp, GTA2_COUNTOF(tmp));
    strcat(path, tmp);
    if (strcmp(path, "player\\") == 0)
    {
        strcpy(path, "");
    }

    gGameSession_67E8E0.DebugStr_4C58D0(path);
    gGameSession_67E8E0.SetMainStageIdx_4C58F0(0);
    gGameSession_67E8E0.SetStage_4C5900(0);
    gGameSession_67E8E0.SetBonusStage_4C5910(0);
    gGameSession_67E8E0.SetPlySlotIdx_4C5920(0);
    gGameSession_67E8E0.SetLevelFinishBonusType_4C5930(0);

    for (s32 i = 0; i < GTA2_COUNTOF(field_408_statistics); i++)
    {
        field_408_statistics[i] = 0;
    }

    field_430_car_damage_cost = 0;
    field_434_evasion_rating = 0;
    field_438_bonus_rating_text_idx = 0;
    field_43A_bStartedFromPlayBonusMenu = 0;

    init_4C5AF0();
}

MATCH_FUNC(0x4C5870)
char* GameSession_578::SetMapName_4C5870(char_type* pName)
{
    return strncpy(field_0_map_name, pName, 255u);
}

MATCH_FUNC(0x4C5890)
char* GameSession_578::SetStyleName_4C5890(char_type* pName)
{
    return strncpy(field_100_style_name, pName, 0xFFu);
}

MATCH_FUNC(0x4C58B0)
char* GameSession_578::SetScriptName_4C58B0(char_type* pName)
{
    return strncpy(field_200_script_name, pName, 0xFFu);
}

MATCH_FUNC(0x4C58D0)
char_type* GameSession_578::DebugStr_4C58D0(char_type* pStr)
{
    return strncpy(field_300_debug_str, pStr, 0xFFu);
}

MATCH_FUNC(0x4C58F0)
void GameSession_578::SetMainStageIdx_4C58F0(char_type main_stage)
{
    field_400_main_stage = main_stage;
}

MATCH_FUNC(0x4C5900)
void GameSession_578::SetStage_4C5900(char_type stage)
{
    field_401_stage = stage;
}

MATCH_FUNC(0x4C5910)
void GameSession_578::SetBonusStage_4C5910(char_type bonus_stage)
{
    field_402_bonus_stage = bonus_stage;
}

MATCH_FUNC(0x4C5920)
void GameSession_578::SetPlySlotIdx_4C5920(char_type player_slot_idx)
{
    field_403_player_slot_idx = player_slot_idx;
}

MATCH_FUNC(0x4C5930)
void GameSession_578::SetLevelFinishBonusType_4C5930(char_type bonus_type)
{
    field_404_level_finish_bonus_type = bonus_type;
}

MATCH_FUNC(0x4C5940)
char* GameSession_578::GetMapName_4C5940()
{
    return field_0_map_name;
}

MATCH_FUNC(0x4C5950)
char* GameSession_578::GetStyleName_4C5950()
{
    return field_100_style_name;
}

MATCH_FUNC(0x4C5960)
char* GameSession_578::GetScriptName_4C5960()
{
    return field_200_script_name;
}

MATCH_FUNC(0x4C5970)
char* GameSession_578::GetDebugStr_4C5970()
{
    return field_300_debug_str;
}

MATCH_FUNC(0x4C5980)
char_type GameSession_578::GetMainStageIdx_4C5980()
{
    return field_400_main_stage;
}

MATCH_FUNC(0x4C5990)
char_type GameSession_578::GetStage_4C5990()
{
    return field_401_stage;
}

MATCH_FUNC(0x4C59A0)
char_type GameSession_578::IsBonusStage_4C59A0()
{
    return field_402_bonus_stage;
}

MATCH_FUNC(0x4C59B0)
u8 GameSession_578::GetPlySlotIdx_4C59B0()
{
    return field_403_player_slot_idx;
}

MATCH_FUNC(0x4C59C0)
char_type GameSession_578::GetLevelFinishBonusType_4C59C0()
{
    return field_404_level_finish_bonus_type;
}

MATCH_FUNC(0x4C59D0)
void GameSession_578::SetStatistic_4C59D0(u8 i, s32 value)
{
    field_408_statistics[i] = value;
}

MATCH_FUNC(0x4C59F0)
s32 GameSession_578::GetStatistic_4C59F0(u8 idx)
{
    return field_408_statistics[idx];
}

MATCH_FUNC(0x4C5A10)
void GameSession_578::StoreCrimeStats_4C5A10(Player* pPlayer)
{
    for (u8 i = 0; i < crime_stats_type::count_10; i++)
    {
        SetStatistic_4C59D0(i, pPlayer->field_644_crime_stats.field_0_crime_count_list[i]);
    }

    SetCarDamageCost_4C5A70(pPlayer->field_644_crime_stats.field_34_car_damage_cost);
    SetEvasionRating_4C5A90(pPlayer->field_644_crime_stats.field_38_evasion_rating);
}

MATCH_FUNC(0x4C5A70)
void GameSession_578::SetCarDamageCost_4C5A70(s32 cost)
{
    field_430_car_damage_cost = cost;
}

MATCH_FUNC(0x4C5A80)
s32 GameSession_578::GetCarDamageCost_4C5A80()
{
    return field_430_car_damage_cost;
}

MATCH_FUNC(0x4C5A90)
void GameSession_578::SetEvasionRating_4C5A90(s32 rating)
{
    field_434_evasion_rating = rating;
}

MATCH_FUNC(0x4C5AA0)
s32 GameSession_578::GetEvasionRating_4C5AA0()
{
    return field_434_evasion_rating;
}

MATCH_FUNC(0x4C5AB0)
void GameSession_578::SetBonusRatingTextIdx_4C5AB0(s16 gxt_text_idx)
{
    field_438_bonus_rating_text_idx = gxt_text_idx;
}

MATCH_FUNC(0x4C5AC0)
s16 GameSession_578::GetBonusRatingTextIdx_4C5AC0()
{
    return field_438_bonus_rating_text_idx;
}

MATCH_FUNC(0x4C5AD0)
void GameSession_578::SetStartedFromPlayBonusMenu_4C5AD0(bool bStarted)
{
    field_43A_bStartedFromPlayBonusMenu = bStarted;
}

MATCH_FUNC(0x4C5AE0)
bool GameSession_578::IsStartedFromPlayBonusMenu_4C5AE0()
{
    return field_43A_bStartedFromPlayBonusMenu;
}

MATCH_FUNC(0x4C5AF0)
void GameSession_578::init_4C5AF0()
{
    field_43B_game_type = 0;
    field_43C_points_limit = 0;
    field_440_user_player_idx = 0;
    field_441_max_players = 0;
    field_442_winner_player_idx = 6;

    for (s32 i = 0; i < GTA2_COUNTOF(field_490_frags_list); i++)
    {
        for (s32 j = 0; j < MAX_PLAYERS; j++)
        {
            field_448_frags_by_victim[i].field_0_frags_on_victim[j] = 0;
        }
        field_490_frags_list[i] = 0;
        field_49C_points_list[i] = 0;
        wcscpy(field_4B4_player_names[i].field_0_name, gEmptyWStr_67DC8C);
    }
}

MATCH_FUNC(0x4C5B80)
void GameSession_578::SetMultiplayerParams_4C5B80(char_type game_type, s32 points_limit, char_type user_idx, char_type max_players, s32 time_limit)
{
    field_43B_game_type = game_type;
    field_43C_points_limit = points_limit;
    field_440_user_player_idx = user_idx;
    field_441_max_players = max_players;
    field_444_game_time_limit = time_limit;
}

MATCH_FUNC(0x4C5BC0)
u8 GameSession_578::GetMultiplayerGamemode_4C5BC0()
{
    return field_43B_game_type;
}

MATCH_FUNC(0x4C5BD0)
s32 GameSession_578::GetMultiplayerPointsLimit_4C5BD0()
{
    return field_43C_points_limit;
}

MATCH_FUNC(0x4C5BE0)
char_type GameSession_578::GetUserPlayerIdx_4C5BE0()
{
    return field_440_user_player_idx;
}

MATCH_FUNC(0x4C5BF0)
char_type GameSession_578::GetMaxPlayers_4C5BF0()
{
    return field_441_max_players;
}

MATCH_FUNC(0x4C5C00)
void GameSession_578::SetWinnerIdx_4C5C00(char_type player_idx)
{
    if (field_442_winner_player_idx == MAX_PLAYERS)
    {
        field_442_winner_player_idx = player_idx;
    }
}

MATCH_FUNC(0x4C5C20)
char_type GameSession_578::GetWinnerIdx_4C5C20()
{
    return field_442_winner_player_idx;
}

MATCH_FUNC(0x4C5C30)
void GameSession_578::SetPlayerName_4C5C30(u16 player_idx, wchar_t* pName)
{
    wcsncpy(field_4B4_player_names[player_idx].field_0_name, pName, 16u);
}

MATCH_FUNC(0x4C5C60)
PlayerName_20* GameSession_578::GetPlayerName_4C5C60(u16 player_idx)
{
    return &field_4B4_player_names[player_idx];
}

MATCH_FUNC(0x4C5C80)
void GameSession_578::ChangePointsForPlayerIdxByAmount_4C5C80(u8 player_idx, s32 amount)
{
    field_49C_points_list[player_idx] += amount;
}

MATCH_FUNC(0x4C5CB0)
s32 GameSession_578::GetPointsForPlayerIdx_4C5CB0(u8 player_idx)
{
    return field_49C_points_list[player_idx];
}

MATCH_FUNC(0x4C5CD0)
void GameSession_578::UpdateFrags_4C5CD0(u8 player_killer_idx, u8 player_victim_idx)
{
    field_448_frags_by_victim[player_killer_idx].field_0_frags_on_victim[player_victim_idx]++;
    Player* pPlayer = gGame_0x40_67E008->get_player_4219E0(player_killer_idx);
    if (player_killer_idx == player_victim_idx)
    {
        // player killed himself
        if (field_490_frags_list[player_killer_idx] > 0)
        {
            field_490_frags_list[player_killer_idx]--;
            pPlayer->field_2D4_scores.ChangeFragsByAmount_5935D0(-1);
        }
    }
    else
    {
        field_490_frags_list[player_killer_idx]++;
        pPlayer->field_2D4_scores.ChangeFragsByAmount_5935D0(1);
    }
}

MATCH_FUNC(0x4C5D60)
u16 GameSession_578::GetFragsForPlayerIdx_4C5D60(u8 player_idx)
{
    return field_490_frags_list[player_idx];
}

MATCH_FUNC(0x4C5D80)
s16 GameSession_578::GetFragsOnPlayer_4C5D80(u8 player_killer_idx, u8 player_victim_idx)
{
    return field_448_frags_by_victim[player_killer_idx].field_0_frags_on_victim[player_victim_idx];
}
