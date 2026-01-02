#pragma once

#include "Function.hpp"
#include <windows.h>

struct wonderful_knuth_0xC
{
    s16 field_0[6];
};

struct blissful_ganguly_0x20
{
    wchar_t field_0_str[16];
};

class Player;

struct lucid_hamilton
{
    char_type field_0_map_name[256];
    char_type field_100_style_name[256];
    char_type field_200_script_name[256];
    char_type field_300_debug_str[256];
    char_type field_400;
    char_type field_401;
    char_type field_402;
    u8 field_403;
    char_type field_404;
    char_type field_405;
    char_type field_406;
    char_type field_407;
    /*
    statistics:
    field_408[5] = vehicles hijacked
    field_408[6] = civilians run down
    field_408[7] = civilians murdered
    field_408[8] = lawmen killed
    field_408[9] = gang members killed
    */
    s32 field_408_statistics[10];
    s32 field_430;
    s32 field_434;
    s16 field_438;
    char_type field_43A;
    char_type field_43B_game_type;
    s32 field_43C_points_limit;
    char_type field_440_user_player_idx;
    char_type field_441_max_players;
    char_type field_442;
    char_type field_443;
    s32 field_444_game_time_limit;
    wonderful_knuth_0xC field_448[6];
    s16 field_490_frags_list[6];
    s32 field_49C_points_list[6];
    blissful_ganguly_0x20 field_4B4[6];
    s32 field_574;

    // inlined at 45b420 in 9.6f
    EXPORT void reset_field_574()
    {
        field_574 = 0;
    }

    EXPORT void sub_4C53D0();

    EXPORT char* SetMapName_4C5870(char_type* Source);

    EXPORT char* SetStyleName_4C5890(char_type* Source);

    EXPORT char* SetScriptName_4C58B0(char_type* Source);

    EXPORT char_type* DebugStr_4C58D0(char_type* Source);

    EXPORT void sub_4C58F0(char_type a2);

    EXPORT void sub_4C5900(char_type a2);

    EXPORT void sub_4C5910(char_type a2);

    EXPORT void sub_4C5920(char_type a2);

    EXPORT void sub_4C5930(char_type a2);

    EXPORT char_type* GetMapName_4C5940();

    EXPORT char_type* GetStyleName_4C5950();

    EXPORT char_type* GetScriptName_4C5960();

    EXPORT char_type* GetDebugStr_4C5970();

    EXPORT char_type sub_4C5980();

    EXPORT char_type sub_4C5990();

    EXPORT char_type sub_4C59A0();

    EXPORT u8 GetPlySlotIdx_4C59B0();

    EXPORT char_type sub_4C59C0();

    EXPORT void sub_4C59D0(u8 a2, s32 a3);

    EXPORT s32 sub_4C59F0(u8 idx);

    EXPORT void sub_4C5A10(Player* a1);

    EXPORT void sub_4C5A70(s32 a2);

    EXPORT s32 sub_4C5A80();

    EXPORT void sub_4C5A90(s32 a2);

    EXPORT s32 sub_4C5AA0();

    EXPORT void sub_4C5AB0(s16 a2);

    EXPORT s16 sub_4C5AC0();

    EXPORT void sub_4C5AD0(char_type a2);

    EXPORT char_type sub_4C5AE0();

    EXPORT void init_4C5AF0();

    EXPORT void sub_4C5B80(char_type a2, s32 a3, char_type a4, char_type a5, s32 a6);

    EXPORT u8 GetMultiplayerGamemode_4C5BC0();

    EXPORT s32 GetMultiplayerPointsLimit_4C5BD0();

    EXPORT char_type GetUserPlayerIdx_4C5BE0();

    EXPORT char_type GetMaxPlayers_4C5BF0();

    EXPORT void sub_4C5C00(char_type a2);

    EXPORT char_type sub_4C5C20();

    EXPORT void sub_4C5C30(u16 a2, wchar_t* Source);

    EXPORT blissful_ganguly_0x20* sub_4C5C60(u16 a2);

    EXPORT void sub_4C5C80(u8 a2, s32 a3);

    EXPORT s32 GetPointsForPlayerIdx_4C5CB0(u8 a2);

    EXPORT void UpdateFrags_4C5CD0(u8 a2, u8 a3);

    EXPORT u16 GetFragsForPlayerIdx_4C5D60(u8 a2);

    EXPORT s16 sub_4C5D80(u8 a2, u8 a3);
};

EXTERN_GLOBAL(lucid_hamilton, gLucid_hamilton_67E8E0);
