#pragma once

#include "Function.hpp"
#include "fix16.hpp"

class Player;

class youthful_einstein
{
  public:
    EXPORT void youthful_einstein::ctor_516560();
    EXPORT void sub_516590(Player* a2);
    EXPORT void UpdateFugitive_516660();
    EXPORT void sub_516740(Player* a2, Player* a3);

    Player* field_0_fugitive;  //  the player who is "IT"
    s32 field_4_time[6]; //  it may be the timer of each player in tag mode
    s32 field_1C;
    u8 field_20[6];
};

EXTERN_GLOBAL(youthful_einstein, gYouthful_einstein_6F8450);
