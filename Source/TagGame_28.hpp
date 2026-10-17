#pragma once

#include "Function.hpp"
#include "fix16.hpp"
#include "GameSession_578.hpp"

class Player;
class Ped;

class TagGame_28
{
  public:
    EXPORT void Init_516560();
    EXPORT void SetNewFugitive_516590(Player* pNewFugitive);
    EXPORT void ExecuteGamemodeTick_516660();
    EXPORT void UpdateFugitive_516740(Player* pFormerPlayerFugitive, Player* pPlayer_killer);

    // 9.6f 0x434B20
    inline bool IsTagGame_434B20()
    {
        return gGameSession_67E8E0.GetMultiplayerGamemode_4C5BC0() == TAG_GAME_3;
    }

    // 9.6f 0x434B60, defined in Player.hpp
    inline bool IsFugitivePed_434B60(Ped* pPed);

    // 9.6f 0x453A90
    inline u8 HasQuit_453A90(s32 player_idx)
    {
        return field_20_bHasQuit[player_idx];
    }

    // 9.6f 0x4C7380, defined in Hud.cpp (needs Player)
    inline s32 GetPlayerTime_4C7380(Player* pPlayer);

    // 9.6f 0x461DD0
    inline void SetQuit_461DD0(s32 player_idx)
    {
        field_20_bHasQuit[player_idx] = 1;
    }

    // 9.6f 0x453AA0
    inline s32 GetTime_453AA0(s32 player_idx)
    {
        return field_4_it_time_secs[player_idx];
    }

    // 9.6f 0x453AB0, defined in Frontend.cpp
    inline s32 GetLeaderIdx_453AB0();

    Player* field_0_fugitive;  //  the player who is "IT"
    s32 field_4_it_time_secs[MAX_PLAYERS]; //  seconds each player has spent as "IT" (counted while the net time limit is enabled)
    s32 field_1C_tick_timer;
    u8 field_20_bHasQuit[MAX_PLAYERS];
};

EXTERN_GLOBAL(TagGame_28, gTagGame_6F8450);
