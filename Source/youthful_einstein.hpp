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

    Player* field_0_fugitive;  //  the player who is "IT"
    s32 field_4_time[6]; //  it may be the timer of each player in tag mode
    s32 field_1C_tick_timer;
    u8 field_20[6];
};

EXTERN_GLOBAL(youthful_einstein, gYouthful_einstein_6F8450);
