#pragma once

#include "Function.hpp"
#include "fix16.hpp"
#include "lucid_hamilton.hpp"

class Player;
class Ped;

class youthful_einstein
{
  public:
    EXPORT void youthful_einstein::ctor_516560();
    EXPORT void SetNewFugitive_516590(Player* a2);
    EXPORT void ExecuteGamemodeTick_516660();
    EXPORT void UpdateFugitive_516740(Player* a2, Player* a3);

    // 9.6f 0x434B20
    inline bool IsTagGame_434B20()
    {
        return gLucid_hamilton_67E8E0.GetMultiplayerGamemode_4C5BC0() == 3;
    }

    // 9.6f 0x434B60, defined in Player.hpp
    inline bool IsFugitivePed_434B60(Ped* pPed);

    // 9.6f 0x453A90
    inline u8 HasQuit_453A90(s32 player_idx)
    {
        return field_20[player_idx];
    }

    // 9.6f 0x4C7380, defined in Hud.cpp (needs Player)
    inline s32 GetPlayerTime_4C7380(Player* pPlayer);

    // 9.6f 0x461DD0
    inline void SetQuit_461DD0(s32 player_idx)
    {
        field_20[player_idx] = 1;
    }

    // 9.6f 0x453AA0
    inline s32 GetTime_453AA0(s32 player_idx)
    {
        return field_4_time[player_idx];
    }

    // 9.6f 0x453AB0, defined in Frontend.cpp
    inline s32 GetLeaderIdx_453AB0();

    Player* field_0_fugitive;  //  the player who is "IT"
    s32 field_4_time[MAX_PLAYERS]; //  it may be the timer of each player in tag mode
    s32 field_1C_tick_timer;
    u8 field_20[MAX_PLAYERS];
};

EXTERN_GLOBAL(youthful_einstein, gYouthful_einstein_6F8450);
