#include "lucid_hamilton.hpp"
#include "Function.hpp"
#include "Game_0x40.hpp"
#include "Globals.hpp"
#include "Player.hpp"
#include "registry.hpp"

// GLOBAL: 105 0x67E8E0
DEFINE_GLOBAL(lucid_hamilton, gLucid_hamilton_67E8E0);
EXTERN_GLOBAL_ARRAY(char_type, byte_67DC88, 4); //, TODO, 0xUNKNOWN);
EXTERN_GLOBAL_ARRAY(wchar_t, word_67DC8C, 32);

// FUNCTION: 105 0x4C53D0
void lucid_hamilton::sub_4C53D0()
{
    char path[256];
    char tmp[256];

    strcpy(tmp, byte_67DC88);
    strcpy(path, "data\\");
    gRegistry_6FF968.Get_Debug_Setting_586E90("mapname", (LPBYTE)tmp, GTA2_COUNTOF(tmp));
    strcat(path, tmp);
    if (strcmp(path, "data\\") == 0)
    {
        strcpy(path, "data\\jointmap.gmp");
    }
    gLucid_hamilton_67E8E0.SetMapName_4C5870(path);

    strcpy(tmp, byte_67DC88);
    strcpy(path, "data\\");
    gRegistry_6FF968.Get_Debug_Setting_586E90("stylename", (LPBYTE)tmp, GTA2_COUNTOF(tmp));
    strcat(path, tmp);
    if (strcmp(path, "data\\") == 0)
    {
        strcpy(path, "data\\style.sty");
    }
    gLucid_hamilton_67E8E0.SetStyleName_4C5890(path);

    strcpy(tmp, byte_67DC88);
    strcpy(path, "data\\");
    gRegistry_6FF968.Get_Debug_Setting_586E90("scriptname", (LPBYTE)tmp, GTA2_COUNTOF(tmp));
    strcat(path, tmp);
    if (strcmp(path, "data\\") == 0)
    {
        strcpy(path, "data\\q.scr");
    }
    gLucid_hamilton_67E8E0.SetScriptName_4C58B0(path);

    strcpy(tmp, byte_67DC88);
    strcpy(path, "player\\");
    gRegistry_6FF968.Get_Debug_Setting_586E90("savename", (LPBYTE)tmp, GTA2_COUNTOF(tmp));
    strcat(path, tmp);
    if (strcmp(path, "player\\") == 0)
    {
        strcpy(path, byte_67DC88);
    }

    gLucid_hamilton_67E8E0.DebugStr_4C58D0(path);
    gLucid_hamilton_67E8E0.sub_4C58F0(0);
    gLucid_hamilton_67E8E0.sub_4C5900(0);
    gLucid_hamilton_67E8E0.sub_4C5910(0);
    gLucid_hamilton_67E8E0.sub_4C5920(0);
    gLucid_hamilton_67E8E0.sub_4C5930(0);

    for (s32 i = 0; i < GTA2_COUNTOF(field_408); i++)
    {
        field_408[i] = 0;
    }

    field_430 = 0;
    field_434 = 0;
    field_438 = 0;
    field_43A = 0;

    init_4C5AF0();
}

// FUNCTION: 105 0x4C5870
char* lucid_hamilton::SetMapName_4C5870(char_type* Source)
{
    return strncpy(field_0_map_name, Source, 255u);
}

// FUNCTION: 105 0x4C5890
char* lucid_hamilton::SetStyleName_4C5890(char_type* Source)
{
    return strncpy(field_100_style_name, Source, 0xFFu);
}

// FUNCTION: 105 0x4C58B0
char* lucid_hamilton::SetScriptName_4C58B0(char_type* Source)
{
    return strncpy(field_200_script_name, Source, 0xFFu);
}

// FUNCTION: 105 0x4C58D0
char_type* lucid_hamilton::DebugStr_4C58D0(char_type* Source)
{
    return strncpy(field_300_debug_str, Source, 0xFFu);
}

// FUNCTION: 105 0x4C58F0
void lucid_hamilton::sub_4C58F0(char_type a2)
{
    field_400 = a2;
}

// FUNCTION: 105 0x4C5900
void lucid_hamilton::sub_4C5900(char_type a2)
{
    field_401 = a2;
}

// FUNCTION: 105 0x4C5910
void lucid_hamilton::sub_4C5910(char_type a2)
{
    field_402 = a2;
}

// FUNCTION: 105 0x4C5920
void lucid_hamilton::sub_4C5920(char_type a2)
{
    field_403 = a2;
}

// FUNCTION: 105 0x4C5930
void lucid_hamilton::sub_4C5930(char_type a2)
{
    field_404 = a2;
}

// FUNCTION: 105 0x4C5940
char* lucid_hamilton::GetMapName_4C5940()
{
    return field_0_map_name;
}

// FUNCTION: 105 0x4C5950
char* lucid_hamilton::GetStyleName_4C5950()
{
    return field_100_style_name;
}

// FUNCTION: 105 0x4C5960
char* lucid_hamilton::GetScriptName_4C5960()
{
    return field_200_script_name;
}

// FUNCTION: 105 0x4C5970
char* lucid_hamilton::GetDebugStr_4C5970()
{
    return field_300_debug_str;
}

// FUNCTION: 105 0x4C5980
char_type lucid_hamilton::sub_4C5980()
{
    return field_400;
}

// FUNCTION: 105 0x4C5990
char_type lucid_hamilton::sub_4C5990()
{
    return field_401;
}

// FUNCTION: 105 0x4C59A0
char_type lucid_hamilton::sub_4C59A0()
{
    return field_402;
}

// FUNCTION: 105 0x4C59B0
u8 lucid_hamilton::GetPlySlotIdx_4C59B0()
{
    return field_403;
}

// FUNCTION: 105 0x4C59C0
char_type lucid_hamilton::sub_4C59C0()
{
    return field_404;
}

// FUNCTION: 105 0x4C59D0
void lucid_hamilton::sub_4C59D0(u8 a2, s32 a3)
{
    field_408[a2] = a3;
}

// FUNCTION: 105 0x4C59F0
s32 lucid_hamilton::sub_4C59F0(u8 idx)
{
    return field_408[idx];
}

// FUNCTION: 105 0x4C5A10
void lucid_hamilton::sub_4C5A10(Player* pPlayer)
{
    for (u8 i = 0; i < 10; i++)
    {
        sub_4C59D0(i, pPlayer->field_644_unk.field_0[i]);
    }

    sub_4C5A70(pPlayer->field_644_unk.field_34);
    sub_4C5A90(pPlayer->field_644_unk.field_38);
}

// FUNCTION: 105 0x4C5A70
void lucid_hamilton::sub_4C5A70(s32 a2)
{
    field_430 = a2;
}

// FUNCTION: 105 0x4C5A80
s32 lucid_hamilton::sub_4C5A80()
{
    return field_430;
}

// FUNCTION: 105 0x4C5A90
void lucid_hamilton::sub_4C5A90(s32 a2)
{
    field_434 = a2;
}

// FUNCTION: 105 0x4C5AA0
s32 lucid_hamilton::sub_4C5AA0()
{
    return field_434;
}

// FUNCTION: 105 0x4C5AB0
void lucid_hamilton::sub_4C5AB0(s16 a2)
{
    field_438 = a2;
}

// FUNCTION: 105 0x4C5AC0
s16 lucid_hamilton::sub_4C5AC0()
{
    return field_438;
}

// FUNCTION: 105 0x4C5AD0
void lucid_hamilton::sub_4C5AD0(char_type a2)
{
    field_43A = a2;
}

// FUNCTION: 105 0x4C5AE0
char_type lucid_hamilton::sub_4C5AE0()
{
    return field_43A;
}

// FUNCTION: 105 0x4C5AF0
void lucid_hamilton::init_4C5AF0()
{
    field_43B = 0;
    field_43C = 0;
    field_440 = 0;
    field_441 = 0;
    field_442 = 6;

    for (s32 i = 0; i < GTA2_COUNTOF(field_490); i++)
    {
        for (s32 j = 0; j < 6; j++)
        {
            field_448[i].field_0[j] = 0;
        }
        field_490[i] = 0;
        field_49C[i] = 0;
        wcscpy(field_4B4[i].field_0_str, word_67DC8C);
    }
}

// FUNCTION: 105 0x4C5B80
void lucid_hamilton::sub_4C5B80(char_type a2, s32 a3, char_type a4, char_type a5, s32 a6)
{
    field_43B = a2;
    field_43C = a3;
    field_440 = a4;
    field_441 = a5;
    field_444 = a6;
}

// FUNCTION: 105 0x4C5BC0
char_type lucid_hamilton::sub_4C5BC0()
{
    return field_43B;
}

// FUNCTION: 105 0x4C5BD0
s32 lucid_hamilton::sub_4C5BD0()
{
    return field_43C;
}

// FUNCTION: 105 0x4C5BE0
char_type lucid_hamilton::sub_4C5BE0()
{
    return field_440;
}

// FUNCTION: 105 0x4C5BF0
char_type lucid_hamilton::sub_4C5BF0()
{
    return field_441;
}

// FUNCTION: 105 0x4C5C00
void lucid_hamilton::sub_4C5C00(char_type a2)
{
    if (field_442 == 6)
    {
        field_442 = a2;
    }
}

// FUNCTION: 105 0x4C5C20
char_type lucid_hamilton::sub_4C5C20()
{
    return field_442;
}

// FUNCTION: 105 0x4C5C30
void lucid_hamilton::sub_4C5C30(s32 a2, wchar_t* Source)
{
    wcsncpy(field_4B4[static_cast<u16>(a2)].field_0_str, Source, 16u);
}

// FUNCTION: 105 0x4C5C60
blissful_ganguly_0x20* lucid_hamilton::sub_4C5C60(u16 a2)
{
    return &field_4B4[a2];
}

// FUNCTION: 105 0x4C5C80
void lucid_hamilton::sub_4C5C80(u8 a2, s32 a3)
{
    field_49C[a2] += a3;
}

// FUNCTION: 105 0x4C5CB0
s32 lucid_hamilton::sub_4C5CB0(u8 a2)
{
    return field_49C[a2];
}

// FUNCTION: 105 0x4C5CD0
void lucid_hamilton::sub_4C5CD0(u8 player_idx, u8 f0_idx)
{
    field_448[player_idx].field_0[f0_idx]++;
    Player* pPlayer = gGame_0x40_67E008->field_4_players[player_idx];
    if (player_idx == f0_idx)
    {
        if (field_490[player_idx] > 0)
        {
            field_490[player_idx]--;
            pPlayer->field_2D4_unk.sub_5935D0(-1);
        }
    }
    else
    {
        field_490[player_idx]++;
        pPlayer->field_2D4_unk.sub_5935D0(1);
    }
}

// FUNCTION: 105 0x4C5D60
u16 lucid_hamilton::sub_4C5D60(u8 a2)
{
    return field_490[a2];
}

// FUNCTION: 105 0x4C5D80
s16 lucid_hamilton::sub_4C5D80(u8 a2, u8 a3)
{
    return field_448[a2].field_0[a3];
}