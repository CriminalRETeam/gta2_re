#pragma once

#include "Function.hpp"
#include "fix16.hpp"
#include "GameSession.hpp"

class Player;
class Ped;

class TagGame
{
  public:
    EXPORT void Init();
    EXPORT void SetNewFugitive(Player* pNewFugitive);
    EXPORT void ExecuteGamemodeTick();
    EXPORT void UpdateFugitive(Player* pFormerPlayerFugitive, Player* pPlayer_killer);

    // 9.6f 0x434B20
    inline bool IsTagGame_434B20()
    {
        return gGameSession_67E8E0.GetMultiplayerGamemode() == TAG_GAME_3;
    }

    // 9.6f 0x434B60, defined in Player.hpp
    inline bool IsFugitivePed_434B60(Ped* pPed);

    // 9.6f 0x453A90
    inline bool HasQuit_453A90(s32 player_idx)
    {
        return mbHasQuit[player_idx];
    }

    // 9.6f 0x4C7380, defined in Hud.cpp (needs Player)
    inline s32 GetPlayerTime_4C7380(Player* pPlayer);

    // 9.6f 0x461DD0
    inline void SetQuit_461DD0(s32 player_idx)
    {
        mbHasQuit[player_idx] = true;
    }

    // 9.6f 0x453AA0
    inline s32 GetTime_453AA0(s32 player_idx)
    {
        return mItTimeSecs[player_idx];
    }

    // 9.6f 0x453AB0, defined in Frontend.cpp
    inline s32 GetLeaderIdx_453AB0();

    Player* mFugitive;  // 0x0:  the player who is "IT"
    s32 mItTimeSecs[MAX_PLAYERS]; // 0x4:  seconds each player has spent as "IT" (counted while the net time limit is enabled)
    s32 mTickTimer; // 0x1C
    bool mbHasQuit[MAX_PLAYERS]; // 0x20
};
GTA2_ASSERT_SIZEOF_ALWAYS(TagGame, 0x28);

EXTERN_GLOBAL(TagGame, gTagGame_6F8450);
