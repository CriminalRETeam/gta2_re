#include "NetUi.hpp"
#include "Frontend.hpp"
#include "Globals.hpp"
#include "NetPlay.hpp"
#include "registry.hpp"
#include "TextTable.hpp"
#include <COMMCTRL.H>
#include <direct.h>

DEFINE_STATIC_MEMBER(UINT_PTR, NetUi, sTimerId, 0x6F8A18);

EXTERN_GLOBAL_ARRAY(char_type, gTmpBuffer, 256);

struct Network_UI_Control_Data_Array
{
    Network_UI_Control_Data_Array()
    {
        // Window 0

        data[NETWORK_MENU_0][0].mWindowParams.mX = 10;
        data[NETWORK_MENU_0][0].mWindowParams.mY = 10;
        data[NETWORK_MENU_0][0].mWindowParams.mW = 300;
        data[NETWORK_MENU_0][0].mWindowParams.mH = 300;
        data[NETWORK_MENU_0][0].mWindowParams.mhMenu = reinterpret_cast<HMENU>(LISTBOX_ALL_NET_GAMES_1000);
        data[NETWORK_MENU_0][0].mWindowParams.mStyle = 0x40010001;
        data[NETWORK_MENU_0][0].mWindowParams.mExStyle = 0x200;
        strcpy(data[NETWORK_MENU_0][0].mWindowClassName, "LISTBOX");
        strcpy(data[NETWORK_MENU_0][0].mGtxStringName, "");

        data[NETWORK_MENU_0][1].mWindowParams.mX = 10;
        data[NETWORK_MENU_0][1].mWindowParams.mY = 320;
        data[NETWORK_MENU_0][1].mWindowParams.mW = 100;
        data[NETWORK_MENU_0][1].mWindowParams.mH = 30;
        data[NETWORK_MENU_0][1].mWindowParams.mhMenu = reinterpret_cast<HMENU>(BUTTON_JOIN_GAME_1001);
        data[NETWORK_MENU_0][1].mWindowParams.mStyle = 0x48030001;
        data[NETWORK_MENU_0][1].mWindowParams.mExStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][1].mWindowClassName, "BUTTON");
        strcpy(data[NETWORK_MENU_0][1].mGtxStringName, "netui01");

        data[NETWORK_MENU_0][2].mWindowParams.mX = 110;
        data[NETWORK_MENU_0][2].mWindowParams.mY = 320;
        data[NETWORK_MENU_0][2].mWindowParams.mW = 100;
        data[NETWORK_MENU_0][2].mWindowParams.mH = 30;
        data[NETWORK_MENU_0][2].mWindowParams.mhMenu = reinterpret_cast<HMENU>(BUTTON_CREATE_GAME_1002);
        data[NETWORK_MENU_0][2].mWindowParams.mStyle = 0x40010000;
        data[NETWORK_MENU_0][2].mWindowParams.mExStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][2].mWindowClassName, "BUTTON");
        strcpy(data[NETWORK_MENU_0][2].mGtxStringName, "netui02");

        data[NETWORK_MENU_0][3].mWindowParams.mX = 210;
        data[NETWORK_MENU_0][3].mWindowParams.mY = 320;
        data[NETWORK_MENU_0][3].mWindowParams.mW = 100;
        data[NETWORK_MENU_0][3].mWindowParams.mH = 30;
        data[NETWORK_MENU_0][3].mWindowParams.mhMenu = reinterpret_cast<HMENU>(IDCANCEL);
        data[NETWORK_MENU_0][3].mWindowParams.mStyle = 0x40010000;
        data[NETWORK_MENU_0][3].mWindowParams.mExStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][3].mWindowClassName, "BUTTON");
        strcpy(data[NETWORK_MENU_0][3].mGtxStringName, "netui03");

        data[NETWORK_MENU_0][4].mWindowParams.mX = 320;
        data[NETWORK_MENU_0][4].mWindowParams.mY = 10;
        data[NETWORK_MENU_0][4].mWindowParams.mW = 50;
        data[NETWORK_MENU_0][4].mWindowParams.mH = 30;
        data[NETWORK_MENU_0][4].mWindowParams.mhMenu = reinterpret_cast<HMENU>(LABEL_PLAYER_NAME_1003);
        data[NETWORK_MENU_0][4].mWindowParams.mStyle = 0x40000000;
        data[NETWORK_MENU_0][4].mWindowParams.mExStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][4].mWindowClassName, "STATIC");
        strcpy(data[NETWORK_MENU_0][4].mGtxStringName, "netui04");

        data[NETWORK_MENU_0][5].mWindowParams.mX = 380;
        data[NETWORK_MENU_0][5].mWindowParams.mY = 10;
        data[NETWORK_MENU_0][5].mWindowParams.mW = 100;
        data[NETWORK_MENU_0][5].mWindowParams.mH = 25;
        data[NETWORK_MENU_0][5].mWindowParams.mhMenu = reinterpret_cast<HMENU>(TEXTBOX_PLAYER_NAME_1004);
        data[NETWORK_MENU_0][5].mWindowParams.mStyle = 0x40010000;
        data[NETWORK_MENU_0][5].mWindowParams.mExStyle = 0x200;
        strcpy(data[NETWORK_MENU_0][5].mWindowClassName, "EDIT");
        strcpy(data[NETWORK_MENU_0][5].mGtxStringName, "");

        // Clear remaining array for windows 0:

        data[NETWORK_MENU_0][6].mWindowParams.mX = 0;
        data[NETWORK_MENU_0][6].mWindowParams.mY = 0;
        data[NETWORK_MENU_0][6].mWindowParams.mW = 0;
        data[NETWORK_MENU_0][6].mWindowParams.mH = 0;
        data[NETWORK_MENU_0][6].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[NETWORK_MENU_0][6].mWindowParams.mStyle = 0x0;
        data[NETWORK_MENU_0][6].mWindowParams.mExStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][6].mWindowClassName, "");
        strcpy(data[NETWORK_MENU_0][6].mGtxStringName, "");

        data[NETWORK_MENU_0][7].mWindowParams.mX = 0;
        data[NETWORK_MENU_0][7].mWindowParams.mY = 0;
        data[NETWORK_MENU_0][7].mWindowParams.mW = 0;
        data[NETWORK_MENU_0][7].mWindowParams.mH = 0;
        data[NETWORK_MENU_0][7].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[NETWORK_MENU_0][7].mWindowParams.mStyle = 0x0;
        data[NETWORK_MENU_0][7].mWindowParams.mExStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][7].mWindowClassName, "");
        strcpy(data[NETWORK_MENU_0][7].mGtxStringName, "");

        data[NETWORK_MENU_0][8].mWindowParams.mX = 0;
        data[NETWORK_MENU_0][8].mWindowParams.mY = 0;
        data[NETWORK_MENU_0][8].mWindowParams.mW = 0;
        data[NETWORK_MENU_0][8].mWindowParams.mH = 0;
        data[NETWORK_MENU_0][8].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[NETWORK_MENU_0][8].mWindowParams.mStyle = 0x0;
        data[NETWORK_MENU_0][8].mWindowParams.mExStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][8].mWindowClassName, "");
        strcpy(data[NETWORK_MENU_0][8].mGtxStringName, "");

        data[NETWORK_MENU_0][9].mWindowParams.mX = 0;
        data[NETWORK_MENU_0][9].mWindowParams.mY = 0;
        data[NETWORK_MENU_0][9].mWindowParams.mW = 0;
        data[NETWORK_MENU_0][9].mWindowParams.mH = 0;
        data[NETWORK_MENU_0][9].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[NETWORK_MENU_0][9].mWindowParams.mStyle = 0x0;
        data[NETWORK_MENU_0][9].mWindowParams.mExStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][9].mWindowClassName, "");
        strcpy(data[NETWORK_MENU_0][9].mGtxStringName, "");

        data[NETWORK_MENU_0][10].mWindowParams.mX = 0;
        data[NETWORK_MENU_0][10].mWindowParams.mY = 0;
        data[NETWORK_MENU_0][10].mWindowParams.mW = 0;
        data[NETWORK_MENU_0][10].mWindowParams.mH = 0;
        data[NETWORK_MENU_0][10].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[NETWORK_MENU_0][10].mWindowParams.mStyle = 0x0;
        data[NETWORK_MENU_0][10].mWindowParams.mExStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][10].mWindowClassName, "");
        strcpy(data[NETWORK_MENU_0][10].mGtxStringName, "");

        data[NETWORK_MENU_0][11].mWindowParams.mX = 0;
        data[NETWORK_MENU_0][11].mWindowParams.mY = 0;
        data[NETWORK_MENU_0][11].mWindowParams.mW = 0;
        data[NETWORK_MENU_0][11].mWindowParams.mH = 0;
        data[NETWORK_MENU_0][11].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[NETWORK_MENU_0][11].mWindowParams.mStyle = 0x0;
        data[NETWORK_MENU_0][11].mWindowParams.mExStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][11].mWindowClassName, "");
        strcpy(data[NETWORK_MENU_0][11].mGtxStringName, "");

        data[NETWORK_MENU_0][12].mWindowParams.mX = 0;
        data[NETWORK_MENU_0][12].mWindowParams.mY = 0;
        data[NETWORK_MENU_0][12].mWindowParams.mW = 0;
        data[NETWORK_MENU_0][12].mWindowParams.mH = 0;
        data[NETWORK_MENU_0][12].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[NETWORK_MENU_0][12].mWindowParams.mStyle = 0x0;
        data[NETWORK_MENU_0][12].mWindowParams.mExStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][12].mWindowClassName, "");
        strcpy(data[NETWORK_MENU_0][12].mGtxStringName, "");

        data[NETWORK_MENU_0][13].mWindowParams.mX = 0;
        data[NETWORK_MENU_0][13].mWindowParams.mY = 0;
        data[NETWORK_MENU_0][13].mWindowParams.mW = 0;
        data[NETWORK_MENU_0][13].mWindowParams.mH = 0;
        data[NETWORK_MENU_0][13].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[NETWORK_MENU_0][13].mWindowParams.mStyle = 0x0;
        data[NETWORK_MENU_0][13].mWindowParams.mExStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][13].mWindowClassName, "");
        strcpy(data[NETWORK_MENU_0][13].mGtxStringName, "");

        data[NETWORK_MENU_0][14].mWindowParams.mX = 0;
        data[NETWORK_MENU_0][14].mWindowParams.mY = 0;
        data[NETWORK_MENU_0][14].mWindowParams.mW = 0;
        data[NETWORK_MENU_0][14].mWindowParams.mH = 0;
        data[NETWORK_MENU_0][14].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[NETWORK_MENU_0][14].mWindowParams.mStyle = 0x0;
        data[NETWORK_MENU_0][14].mWindowParams.mExStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][14].mWindowClassName, "");
        strcpy(data[NETWORK_MENU_0][14].mGtxStringName, "");

        data[NETWORK_MENU_0][15].mWindowParams.mX = 0;
        data[NETWORK_MENU_0][15].mWindowParams.mY = 0;
        data[NETWORK_MENU_0][15].mWindowParams.mW = 0;
        data[NETWORK_MENU_0][15].mWindowParams.mH = 0;
        data[NETWORK_MENU_0][15].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[NETWORK_MENU_0][15].mWindowParams.mStyle = 0x0;
        data[NETWORK_MENU_0][15].mWindowParams.mExStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][15].mWindowClassName, "");
        strcpy(data[NETWORK_MENU_0][15].mGtxStringName, "");

        data[NETWORK_MENU_0][16].mWindowParams.mX = 0;
        data[NETWORK_MENU_0][16].mWindowParams.mY = 0;
        data[NETWORK_MENU_0][16].mWindowParams.mW = 0;
        data[NETWORK_MENU_0][16].mWindowParams.mH = 0;
        data[NETWORK_MENU_0][16].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[NETWORK_MENU_0][16].mWindowParams.mStyle = 0x0;
        data[NETWORK_MENU_0][16].mWindowParams.mExStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][16].mWindowClassName, "");
        strcpy(data[NETWORK_MENU_0][16].mGtxStringName, "");

        data[NETWORK_MENU_0][17].mWindowParams.mX = 0;
        data[NETWORK_MENU_0][17].mWindowParams.mY = 0;
        data[NETWORK_MENU_0][17].mWindowParams.mW = 0;
        data[NETWORK_MENU_0][17].mWindowParams.mH = 0;
        data[NETWORK_MENU_0][17].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[NETWORK_MENU_0][17].mWindowParams.mStyle = 0x0;
        data[NETWORK_MENU_0][17].mWindowParams.mExStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][17].mWindowClassName, "");
        strcpy(data[NETWORK_MENU_0][17].mGtxStringName, "");

        data[NETWORK_MENU_0][18].mWindowParams.mX = 0;
        data[NETWORK_MENU_0][18].mWindowParams.mY = 0;
        data[NETWORK_MENU_0][18].mWindowParams.mW = 0;
        data[NETWORK_MENU_0][18].mWindowParams.mH = 0;
        data[NETWORK_MENU_0][18].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[NETWORK_MENU_0][18].mWindowParams.mStyle = 0x0;
        data[NETWORK_MENU_0][18].mWindowParams.mExStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][18].mWindowClassName, "");
        strcpy(data[NETWORK_MENU_0][18].mGtxStringName, "");

        data[NETWORK_MENU_0][19].mWindowParams.mX = 0;
        data[NETWORK_MENU_0][19].mWindowParams.mY = 0;
        data[NETWORK_MENU_0][19].mWindowParams.mW = 0;
        data[NETWORK_MENU_0][19].mWindowParams.mH = 0;
        data[NETWORK_MENU_0][19].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[NETWORK_MENU_0][19].mWindowParams.mStyle = 0x0;
        data[NETWORK_MENU_0][19].mWindowParams.mExStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][19].mWindowClassName, "");
        strcpy(data[NETWORK_MENU_0][19].mGtxStringName, "");

        data[NETWORK_MENU_0][20].mWindowParams.mX = 0;
        data[NETWORK_MENU_0][20].mWindowParams.mY = 0;
        data[NETWORK_MENU_0][20].mWindowParams.mW = 0;
        data[NETWORK_MENU_0][20].mWindowParams.mH = 0;
        data[NETWORK_MENU_0][20].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[NETWORK_MENU_0][20].mWindowParams.mStyle = 0x0;
        data[NETWORK_MENU_0][20].mWindowParams.mExStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][20].mWindowClassName, "");
        strcpy(data[NETWORK_MENU_0][20].mGtxStringName, "");

        data[NETWORK_MENU_0][21].mWindowParams.mX = 0;
        data[NETWORK_MENU_0][21].mWindowParams.mY = 0;
        data[NETWORK_MENU_0][21].mWindowParams.mW = 0;
        data[NETWORK_MENU_0][21].mWindowParams.mH = 0;
        data[NETWORK_MENU_0][21].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[NETWORK_MENU_0][21].mWindowParams.mStyle = 0x0;
        data[NETWORK_MENU_0][21].mWindowParams.mExStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][21].mWindowClassName, "");
        strcpy(data[NETWORK_MENU_0][21].mGtxStringName, "");

        data[NETWORK_MENU_0][22].mWindowParams.mX = 0;
        data[NETWORK_MENU_0][22].mWindowParams.mY = 0;
        data[NETWORK_MENU_0][22].mWindowParams.mW = 0;
        data[NETWORK_MENU_0][22].mWindowParams.mH = 0;
        data[NETWORK_MENU_0][22].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[NETWORK_MENU_0][22].mWindowParams.mStyle = 0x0;
        data[NETWORK_MENU_0][22].mWindowParams.mExStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][22].mWindowClassName, "");
        strcpy(data[NETWORK_MENU_0][22].mGtxStringName, "");

        data[NETWORK_MENU_0][23].mWindowParams.mX = 0;
        data[NETWORK_MENU_0][23].mWindowParams.mY = 0;
        data[NETWORK_MENU_0][23].mWindowParams.mW = 0;
        data[NETWORK_MENU_0][23].mWindowParams.mH = 0;
        data[NETWORK_MENU_0][23].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[NETWORK_MENU_0][23].mWindowParams.mStyle = 0x0;
        data[NETWORK_MENU_0][23].mWindowParams.mExStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][23].mWindowClassName, "");
        strcpy(data[NETWORK_MENU_0][23].mGtxStringName, "");

        data[NETWORK_MENU_0][24].mWindowParams.mX = 0;
        data[NETWORK_MENU_0][24].mWindowParams.mY = 0;
        data[NETWORK_MENU_0][24].mWindowParams.mW = 0;
        data[NETWORK_MENU_0][24].mWindowParams.mH = 0;
        data[NETWORK_MENU_0][24].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[NETWORK_MENU_0][24].mWindowParams.mStyle = 0x0;
        data[NETWORK_MENU_0][24].mWindowParams.mExStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][24].mWindowClassName, "");
        strcpy(data[NETWORK_MENU_0][24].mGtxStringName, "");

        data[NETWORK_MENU_0][25].mWindowParams.mX = 0;
        data[NETWORK_MENU_0][25].mWindowParams.mY = 0;
        data[NETWORK_MENU_0][25].mWindowParams.mW = 0;
        data[NETWORK_MENU_0][25].mWindowParams.mH = 0;
        data[NETWORK_MENU_0][25].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[NETWORK_MENU_0][25].mWindowParams.mStyle = 0x0;
        data[NETWORK_MENU_0][25].mWindowParams.mExStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][25].mWindowClassName, "");
        strcpy(data[NETWORK_MENU_0][25].mGtxStringName, "");

        data[NETWORK_MENU_0][26].mWindowParams.mX = 0;
        data[NETWORK_MENU_0][26].mWindowParams.mY = 0;
        data[NETWORK_MENU_0][26].mWindowParams.mW = 0;
        data[NETWORK_MENU_0][26].mWindowParams.mH = 0;
        data[NETWORK_MENU_0][26].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[NETWORK_MENU_0][26].mWindowParams.mStyle = 0x0;
        data[NETWORK_MENU_0][26].mWindowParams.mExStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][26].mWindowClassName, "");
        strcpy(data[NETWORK_MENU_0][26].mGtxStringName, "");

        data[NETWORK_MENU_0][27].mWindowParams.mX = 0;
        data[NETWORK_MENU_0][27].mWindowParams.mY = 0;
        data[NETWORK_MENU_0][27].mWindowParams.mW = 0;
        data[NETWORK_MENU_0][27].mWindowParams.mH = 0;
        data[NETWORK_MENU_0][27].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[NETWORK_MENU_0][27].mWindowParams.mStyle = 0x0;
        data[NETWORK_MENU_0][27].mWindowParams.mExStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][27].mWindowClassName, "");
        strcpy(data[NETWORK_MENU_0][27].mGtxStringName, "");

        data[NETWORK_MENU_0][28].mWindowParams.mX = 0;
        data[NETWORK_MENU_0][28].mWindowParams.mY = 0;
        data[NETWORK_MENU_0][28].mWindowParams.mW = 0;
        data[NETWORK_MENU_0][28].mWindowParams.mH = 0;
        data[NETWORK_MENU_0][28].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[NETWORK_MENU_0][28].mWindowParams.mStyle = 0x0;
        data[NETWORK_MENU_0][28].mWindowParams.mExStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][28].mWindowClassName, "");
        strcpy(data[NETWORK_MENU_0][28].mGtxStringName, "");

        data[NETWORK_MENU_0][29].mWindowParams.mX = 0;
        data[NETWORK_MENU_0][29].mWindowParams.mY = 0;
        data[NETWORK_MENU_0][29].mWindowParams.mW = 0;
        data[NETWORK_MENU_0][29].mWindowParams.mH = 0;
        data[NETWORK_MENU_0][29].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[NETWORK_MENU_0][29].mWindowParams.mStyle = 0x0;
        data[NETWORK_MENU_0][29].mWindowParams.mExStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][29].mWindowClassName, "");
        strcpy(data[NETWORK_MENU_0][29].mGtxStringName, "");

        // Window 1

        data[HOST_LOBBY_1][0].mWindowParams.mX = 10;
        data[HOST_LOBBY_1][0].mWindowParams.mY = 10;
        data[HOST_LOBBY_1][0].mWindowParams.mW = 210;
        data[HOST_LOBBY_1][0].mWindowParams.mH = 130;
        data[HOST_LOBBY_1][0].mWindowParams.mhMenu = reinterpret_cast<HMENU>(LISTVIEW_HOST_PLAYERS_1024);
        data[HOST_LOBBY_1][0].mWindowParams.mStyle = 0x40008005;
        data[HOST_LOBBY_1][0].mWindowParams.mExStyle = 0x200;
        strcpy(data[HOST_LOBBY_1][0].mWindowClassName, "SysListView32");
        strcpy(data[HOST_LOBBY_1][0].mGtxStringName, "");

        data[HOST_LOBBY_1][1].mWindowParams.mX = 230;
        data[HOST_LOBBY_1][1].mWindowParams.mY = 10;
        data[HOST_LOBBY_1][1].mWindowParams.mW = 270;
        data[HOST_LOBBY_1][1].mWindowParams.mH = 130;
        data[HOST_LOBBY_1][1].mWindowParams.mhMenu = reinterpret_cast<HMENU>(CHATBOX_HOST_1022);
        data[HOST_LOBBY_1][1].mWindowParams.mStyle = 0x400018c4;
        data[HOST_LOBBY_1][1].mWindowParams.mExStyle = 0x200;
        strcpy(data[HOST_LOBBY_1][1].mWindowClassName, "EDIT");
        strcpy(data[HOST_LOBBY_1][1].mGtxStringName, "");

        data[HOST_LOBBY_1][2].mWindowParams.mX = 10;
        data[HOST_LOBBY_1][2].mWindowParams.mY = 170;
        data[HOST_LOBBY_1][2].mWindowParams.mW = 100;
        data[HOST_LOBBY_1][2].mWindowParams.mH = 30;
        data[HOST_LOBBY_1][2].mWindowParams.mhMenu = reinterpret_cast<HMENU>(BUTTON_REJECT_PLAYER_1020);
        data[HOST_LOBBY_1][2].mWindowParams.mStyle = 0x48030000;
        data[HOST_LOBBY_1][2].mWindowParams.mExStyle = 0x0;
        strcpy(data[HOST_LOBBY_1][2].mWindowClassName, "BUTTON");
        strcpy(data[HOST_LOBBY_1][2].mGtxStringName, "netui05");

        data[HOST_LOBBY_1][3].mWindowParams.mX = 10;
        data[HOST_LOBBY_1][3].mWindowParams.mY = 210;
        data[HOST_LOBBY_1][3].mWindowParams.mW = 100;
        data[HOST_LOBBY_1][3].mWindowParams.mH = 30;
        data[HOST_LOBBY_1][3].mWindowParams.mhMenu = reinterpret_cast<HMENU>(BUTTON_START_GAME_1021);
        data[HOST_LOBBY_1][3].mWindowParams.mStyle = 0x48010000;
        data[HOST_LOBBY_1][3].mWindowParams.mExStyle = 0x0;
        strcpy(data[HOST_LOBBY_1][3].mWindowClassName, "BUTTON");
        strcpy(data[HOST_LOBBY_1][3].mGtxStringName, "netui06");

        data[HOST_LOBBY_1][4].mWindowParams.mX = 10;
        data[HOST_LOBBY_1][4].mWindowParams.mY = 250;
        data[HOST_LOBBY_1][4].mWindowParams.mW = 100;
        data[HOST_LOBBY_1][4].mWindowParams.mH = 30;
        data[HOST_LOBBY_1][4].mWindowParams.mhMenu = reinterpret_cast<HMENU>(IDCANCEL);
        data[HOST_LOBBY_1][4].mWindowParams.mStyle = 0x40010000;
        data[HOST_LOBBY_1][4].mWindowParams.mExStyle = 0x0;
        strcpy(data[HOST_LOBBY_1][4].mWindowClassName, "BUTTON");
        strcpy(data[HOST_LOBBY_1][4].mGtxStringName, "netui03");

        data[HOST_LOBBY_1][5].mWindowParams.mX = 230;
        data[HOST_LOBBY_1][5].mWindowParams.mY = 150;
        data[HOST_LOBBY_1][5].mWindowParams.mW = 270;
        data[HOST_LOBBY_1][5].mWindowParams.mH = 30;
        data[HOST_LOBBY_1][5].mWindowParams.mhMenu = reinterpret_cast<HMENU>(TEXTFIELD_CHAT_HOST_1025);
        data[HOST_LOBBY_1][5].mWindowParams.mStyle = 0x40031080;
        data[HOST_LOBBY_1][5].mWindowParams.mExStyle = 0x200;
        strcpy(data[HOST_LOBBY_1][5].mWindowClassName, "EDIT");
        strcpy(data[HOST_LOBBY_1][5].mGtxStringName, "");

        data[HOST_LOBBY_1][6].mWindowParams.mX = 230;
        data[HOST_LOBBY_1][6].mWindowParams.mY = 190;
        data[HOST_LOBBY_1][6].mWindowParams.mW = 270;
        data[HOST_LOBBY_1][6].mWindowParams.mH = 30;
        data[HOST_LOBBY_1][6].mWindowParams.mhMenu = reinterpret_cast<HMENU>(BUTTON_SEND_CHAT_MESSAGE_HOST_1023);
        data[HOST_LOBBY_1][6].mWindowParams.mStyle = 0x40010000;
        data[HOST_LOBBY_1][6].mWindowParams.mExStyle = 0x0;
        strcpy(data[HOST_LOBBY_1][6].mWindowClassName, "BUTTON");
        strcpy(data[HOST_LOBBY_1][6].mGtxStringName, "netui07");

        data[HOST_LOBBY_1][7].mWindowParams.mX = 230;
        data[HOST_LOBBY_1][7].mWindowParams.mY = 230;
        data[HOST_LOBBY_1][7].mWindowParams.mW = 270;
        data[HOST_LOBBY_1][7].mWindowParams.mH = 130;
        data[HOST_LOBBY_1][7].mWindowParams.mhMenu = reinterpret_cast<HMENU>(COMBO_MAP_LIST_1026);
        data[HOST_LOBBY_1][7].mWindowParams.mStyle = 0x40210243;
        data[HOST_LOBBY_1][7].mWindowParams.mExStyle = 0x0;
        strcpy(data[HOST_LOBBY_1][7].mWindowClassName, "COMBOBOX");
        strcpy(data[HOST_LOBBY_1][7].mGtxStringName, "");

        data[HOST_LOBBY_1][8].mWindowParams.mX = 230;
        data[HOST_LOBBY_1][8].mWindowParams.mY = 260;
        data[HOST_LOBBY_1][8].mWindowParams.mW = 200;
        data[HOST_LOBBY_1][8].mWindowParams.mH = 30;
        data[HOST_LOBBY_1][8].mWindowParams.mhMenu = reinterpret_cast<HMENU>(LABEL_PLAYERS_REQUIRED_1033);
        data[HOST_LOBBY_1][8].mWindowParams.mStyle = 0x40000000;
        data[HOST_LOBBY_1][8].mWindowParams.mExStyle = 0x0;
        strcpy(data[HOST_LOBBY_1][8].mWindowClassName, "STATIC");
        strcpy(data[HOST_LOBBY_1][8].mGtxStringName, "");

        data[HOST_LOBBY_1][9].mWindowParams.mX = 10;
        data[HOST_LOBBY_1][9].mWindowParams.mY = 300;
        data[HOST_LOBBY_1][9].mWindowParams.mW = 200;
        data[HOST_LOBBY_1][9].mWindowParams.mH = 30;
        data[HOST_LOBBY_1][9].mWindowParams.mhMenu = reinterpret_cast<HMENU>(TRACKBAR_GAME_SPEED_1031);
        data[HOST_LOBBY_1][9].mWindowParams.mStyle = 0x40000001;
        data[HOST_LOBBY_1][9].mWindowParams.mExStyle = 0x0;
        strcpy(data[HOST_LOBBY_1][9].mWindowClassName, "msctls_trackbar32");
        strcpy(data[HOST_LOBBY_1][9].mGtxStringName, "netui09");

        data[HOST_LOBBY_1][10].mWindowParams.mX = 10;
        data[HOST_LOBBY_1][10].mWindowParams.mY = 340;
        data[HOST_LOBBY_1][10].mWindowParams.mW = 200;
        data[HOST_LOBBY_1][10].mWindowParams.mH = 30;
        data[HOST_LOBBY_1][10].mWindowParams.mhMenu = reinterpret_cast<HMENU>(LABEL_GAME_SPEED_TEXT_1032);
        data[HOST_LOBBY_1][10].mWindowParams.mStyle = 0x40000000;
        data[HOST_LOBBY_1][10].mWindowParams.mExStyle = 0x0;
        strcpy(data[HOST_LOBBY_1][10].mWindowClassName, "STATIC");
        strcpy(data[HOST_LOBBY_1][10].mGtxStringName, "");

        data[HOST_LOBBY_1][11].mWindowParams.mX = 230;
        data[HOST_LOBBY_1][11].mWindowParams.mY = 290;
        data[HOST_LOBBY_1][11].mWindowParams.mW = 130;
        data[HOST_LOBBY_1][11].mWindowParams.mH = 130;
        data[HOST_LOBBY_1][11].mWindowParams.mhMenu = reinterpret_cast<HMENU>(COMBO_GAME_TYPE_1036);
        data[HOST_LOBBY_1][11].mWindowParams.mStyle = 0x40210243;
        data[HOST_LOBBY_1][11].mWindowParams.mExStyle = 0x0;
        strcpy(data[HOST_LOBBY_1][11].mWindowClassName, "COMBOBOX");
        strcpy(data[HOST_LOBBY_1][11].mGtxStringName, "");

        data[HOST_LOBBY_1][12].mWindowParams.mX = 230;
        data[HOST_LOBBY_1][12].mWindowParams.mY = 330;
        data[HOST_LOBBY_1][12].mWindowParams.mW = 60;
        data[HOST_LOBBY_1][12].mWindowParams.mH = 26;
        data[HOST_LOBBY_1][12].mWindowParams.mhMenu = reinterpret_cast<HMENU>(TEXTBOX_FRAG_LIMIT_NUMBER_1059);
        data[HOST_LOBBY_1][12].mWindowParams.mStyle = 0x40033000;
        data[HOST_LOBBY_1][12].mWindowParams.mExStyle = 0x1200;
        strcpy(data[HOST_LOBBY_1][12].mWindowClassName, "EDIT");
        strcpy(data[HOST_LOBBY_1][12].mGtxStringName, "");

        data[HOST_LOBBY_1][13].mWindowParams.mX = 300;
        data[HOST_LOBBY_1][13].mWindowParams.mY = 330;
        data[HOST_LOBBY_1][13].mWindowParams.mW = 200;
        data[HOST_LOBBY_1][13].mWindowParams.mH = 30;
        data[HOST_LOBBY_1][13].mWindowParams.mhMenu = reinterpret_cast<HMENU>(LABEL_FRAG_LIMIT_TEXT_1035);
        data[HOST_LOBBY_1][13].mWindowParams.mStyle = 0x40000000;
        data[HOST_LOBBY_1][13].mWindowParams.mExStyle = 0x0;
        strcpy(data[HOST_LOBBY_1][13].mWindowClassName, "STATIC");
        strcpy(data[HOST_LOBBY_1][13].mGtxStringName, "");

        data[HOST_LOBBY_1][14].mWindowParams.mX = 230;
        data[HOST_LOBBY_1][14].mWindowParams.mY = 370;
        data[HOST_LOBBY_1][14].mWindowParams.mW = 60;
        data[HOST_LOBBY_1][14].mWindowParams.mH = 26;
        data[HOST_LOBBY_1][14].mWindowParams.mhMenu = reinterpret_cast<HMENU>(TEXTBOX_GAME_TIME_LIMIT_1038);
        data[HOST_LOBBY_1][14].mWindowParams.mStyle = 0x40033000;
        data[HOST_LOBBY_1][14].mWindowParams.mExStyle = 0x1200;
        strcpy(data[HOST_LOBBY_1][14].mWindowClassName, "EDIT");
        strcpy(data[HOST_LOBBY_1][14].mGtxStringName, "");

        data[HOST_LOBBY_1][15].mWindowParams.mX = 300;
        data[HOST_LOBBY_1][15].mWindowParams.mY = 370;
        data[HOST_LOBBY_1][15].mWindowParams.mW = 200;
        data[HOST_LOBBY_1][15].mWindowParams.mH = 30;
        data[HOST_LOBBY_1][15].mWindowParams.mhMenu = reinterpret_cast<HMENU>(LABEL_TIME_LIMIT_MINUTES_1037);
        data[HOST_LOBBY_1][15].mWindowParams.mStyle = 0x40000000;
        data[HOST_LOBBY_1][15].mWindowParams.mExStyle = 0x0;
        strcpy(data[HOST_LOBBY_1][15].mWindowClassName, "STATIC");
        strcpy(data[HOST_LOBBY_1][15].mGtxStringName, "");

        data[HOST_LOBBY_1][16].mWindowParams.mX = 230;
        data[HOST_LOBBY_1][16].mWindowParams.mY = 400;
        data[HOST_LOBBY_1][16].mWindowParams.mW = 20;
        data[HOST_LOBBY_1][16].mWindowParams.mH = 30;
        data[HOST_LOBBY_1][16].mWindowParams.mhMenu = reinterpret_cast<HMENU>(CHECKBOX_POLICE_ENABLED_1027);
        data[HOST_LOBBY_1][16].mWindowParams.mStyle = 0x40000003;
        data[HOST_LOBBY_1][16].mWindowParams.mExStyle = 0x0;
        strcpy(data[HOST_LOBBY_1][16].mWindowClassName, "BUTTON");
        strcpy(data[HOST_LOBBY_1][16].mGtxStringName, "");

        data[HOST_LOBBY_1][17].mWindowParams.mX = 250;
        data[HOST_LOBBY_1][17].mWindowParams.mY = 405;
        data[HOST_LOBBY_1][17].mWindowParams.mW = 200;
        data[HOST_LOBBY_1][17].mWindowParams.mH = 30;
        data[HOST_LOBBY_1][17].mWindowParams.mhMenu = reinterpret_cast<HMENU>(LABEL_COPS_1028);
        data[HOST_LOBBY_1][17].mWindowParams.mStyle = 0x40000000;
        data[HOST_LOBBY_1][17].mWindowParams.mExStyle = 0x0;
        strcpy(data[HOST_LOBBY_1][17].mWindowClassName, "STATIC");
        strcpy(data[HOST_LOBBY_1][17].mGtxStringName, "");

        // Clear remaining array for windows 1:

        data[HOST_LOBBY_1][18].mWindowParams.mX = 0;
        data[HOST_LOBBY_1][18].mWindowParams.mY = 0;
        data[HOST_LOBBY_1][18].mWindowParams.mW = 0;
        data[HOST_LOBBY_1][18].mWindowParams.mH = 0;
        data[HOST_LOBBY_1][18].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[HOST_LOBBY_1][18].mWindowParams.mStyle = 0x0;
        data[HOST_LOBBY_1][18].mWindowParams.mExStyle = 0x0;
        strcpy(data[HOST_LOBBY_1][18].mWindowClassName, "");
        strcpy(data[HOST_LOBBY_1][18].mGtxStringName, "");

        data[HOST_LOBBY_1][19].mWindowParams.mX = 0;
        data[HOST_LOBBY_1][19].mWindowParams.mY = 0;
        data[HOST_LOBBY_1][19].mWindowParams.mW = 0;
        data[HOST_LOBBY_1][19].mWindowParams.mH = 0;
        data[HOST_LOBBY_1][19].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[HOST_LOBBY_1][19].mWindowParams.mStyle = 0x0;
        data[HOST_LOBBY_1][19].mWindowParams.mExStyle = 0x0;
        strcpy(data[HOST_LOBBY_1][19].mWindowClassName, "");
        strcpy(data[HOST_LOBBY_1][19].mGtxStringName, "");

        data[HOST_LOBBY_1][20].mWindowParams.mX = 0;
        data[HOST_LOBBY_1][20].mWindowParams.mY = 0;
        data[HOST_LOBBY_1][20].mWindowParams.mW = 0;
        data[HOST_LOBBY_1][20].mWindowParams.mH = 0;
        data[HOST_LOBBY_1][20].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[HOST_LOBBY_1][20].mWindowParams.mStyle = 0x0;
        data[HOST_LOBBY_1][20].mWindowParams.mExStyle = 0x0;
        strcpy(data[HOST_LOBBY_1][20].mWindowClassName, "");
        strcpy(data[HOST_LOBBY_1][20].mGtxStringName, "");

        data[HOST_LOBBY_1][21].mWindowParams.mX = 0;
        data[HOST_LOBBY_1][21].mWindowParams.mY = 0;
        data[HOST_LOBBY_1][21].mWindowParams.mW = 0;
        data[HOST_LOBBY_1][21].mWindowParams.mH = 0;
        data[HOST_LOBBY_1][21].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[HOST_LOBBY_1][21].mWindowParams.mStyle = 0x0;
        data[HOST_LOBBY_1][21].mWindowParams.mExStyle = 0x0;
        strcpy(data[HOST_LOBBY_1][21].mWindowClassName, "");
        strcpy(data[HOST_LOBBY_1][21].mGtxStringName, "");

        data[HOST_LOBBY_1][22].mWindowParams.mX = 0;
        data[HOST_LOBBY_1][22].mWindowParams.mY = 0;
        data[HOST_LOBBY_1][22].mWindowParams.mW = 0;
        data[HOST_LOBBY_1][22].mWindowParams.mH = 0;
        data[HOST_LOBBY_1][22].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[HOST_LOBBY_1][22].mWindowParams.mStyle = 0x0;
        data[HOST_LOBBY_1][22].mWindowParams.mExStyle = 0x0;
        strcpy(data[HOST_LOBBY_1][22].mWindowClassName, "");
        strcpy(data[HOST_LOBBY_1][22].mGtxStringName, "");

        data[HOST_LOBBY_1][23].mWindowParams.mX = 0;
        data[HOST_LOBBY_1][23].mWindowParams.mY = 0;
        data[HOST_LOBBY_1][23].mWindowParams.mW = 0;
        data[HOST_LOBBY_1][23].mWindowParams.mH = 0;
        data[HOST_LOBBY_1][23].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[HOST_LOBBY_1][23].mWindowParams.mStyle = 0x0;
        data[HOST_LOBBY_1][23].mWindowParams.mExStyle = 0x0;
        strcpy(data[HOST_LOBBY_1][23].mWindowClassName, "");
        strcpy(data[HOST_LOBBY_1][23].mGtxStringName, "");

        data[HOST_LOBBY_1][24].mWindowParams.mX = 0;
        data[HOST_LOBBY_1][24].mWindowParams.mY = 0;
        data[HOST_LOBBY_1][24].mWindowParams.mW = 0;
        data[HOST_LOBBY_1][24].mWindowParams.mH = 0;
        data[HOST_LOBBY_1][24].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[HOST_LOBBY_1][24].mWindowParams.mStyle = 0x0;
        data[HOST_LOBBY_1][24].mWindowParams.mExStyle = 0x0;
        strcpy(data[HOST_LOBBY_1][24].mWindowClassName, "");
        strcpy(data[HOST_LOBBY_1][24].mGtxStringName, "");

        data[HOST_LOBBY_1][25].mWindowParams.mX = 0;
        data[HOST_LOBBY_1][25].mWindowParams.mY = 0;
        data[HOST_LOBBY_1][25].mWindowParams.mW = 0;
        data[HOST_LOBBY_1][25].mWindowParams.mH = 0;
        data[HOST_LOBBY_1][25].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[HOST_LOBBY_1][25].mWindowParams.mStyle = 0x0;
        data[HOST_LOBBY_1][25].mWindowParams.mExStyle = 0x0;
        strcpy(data[HOST_LOBBY_1][25].mWindowClassName, "");
        strcpy(data[HOST_LOBBY_1][25].mGtxStringName, "");

        data[HOST_LOBBY_1][26].mWindowParams.mX = 0;
        data[HOST_LOBBY_1][26].mWindowParams.mY = 0;
        data[HOST_LOBBY_1][26].mWindowParams.mW = 0;
        data[HOST_LOBBY_1][26].mWindowParams.mH = 0;
        data[HOST_LOBBY_1][26].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[HOST_LOBBY_1][26].mWindowParams.mStyle = 0x0;
        data[HOST_LOBBY_1][26].mWindowParams.mExStyle = 0x0;
        strcpy(data[HOST_LOBBY_1][26].mWindowClassName, "");
        strcpy(data[HOST_LOBBY_1][26].mGtxStringName, "");

        data[HOST_LOBBY_1][27].mWindowParams.mX = 0;
        data[HOST_LOBBY_1][27].mWindowParams.mY = 0;
        data[HOST_LOBBY_1][27].mWindowParams.mW = 0;
        data[HOST_LOBBY_1][27].mWindowParams.mH = 0;
        data[HOST_LOBBY_1][27].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[HOST_LOBBY_1][27].mWindowParams.mStyle = 0x0;
        data[HOST_LOBBY_1][27].mWindowParams.mExStyle = 0x0;
        strcpy(data[HOST_LOBBY_1][27].mWindowClassName, "");
        strcpy(data[HOST_LOBBY_1][27].mGtxStringName, "");

        data[HOST_LOBBY_1][28].mWindowParams.mX = 0;
        data[HOST_LOBBY_1][28].mWindowParams.mY = 0;
        data[HOST_LOBBY_1][28].mWindowParams.mW = 0;
        data[HOST_LOBBY_1][28].mWindowParams.mH = 0;
        data[HOST_LOBBY_1][28].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[HOST_LOBBY_1][28].mWindowParams.mStyle = 0x0;
        data[HOST_LOBBY_1][28].mWindowParams.mExStyle = 0x0;
        strcpy(data[HOST_LOBBY_1][28].mWindowClassName, "");
        strcpy(data[HOST_LOBBY_1][28].mGtxStringName, "");

        data[HOST_LOBBY_1][29].mWindowParams.mX = 0;
        data[HOST_LOBBY_1][29].mWindowParams.mY = 0;
        data[HOST_LOBBY_1][29].mWindowParams.mW = 0;
        data[HOST_LOBBY_1][29].mWindowParams.mH = 0;
        data[HOST_LOBBY_1][29].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[HOST_LOBBY_1][29].mWindowParams.mStyle = 0x0;
        data[HOST_LOBBY_1][29].mWindowParams.mExStyle = 0x0;
        strcpy(data[HOST_LOBBY_1][29].mWindowClassName, "");
        strcpy(data[HOST_LOBBY_1][29].mGtxStringName, "");

        // Window 2

        data[GUEST_LOBBY_2][0].mWindowParams.mX = 10;
        data[GUEST_LOBBY_2][0].mWindowParams.mY = 10;
        data[GUEST_LOBBY_2][0].mWindowParams.mW = 210;
        data[GUEST_LOBBY_2][0].mWindowParams.mH = 130;
        data[GUEST_LOBBY_2][0].mWindowParams.mhMenu = reinterpret_cast<HMENU>(LISTVIEW_JOINED_PLAYERS_1050);
        data[GUEST_LOBBY_2][0].mWindowParams.mStyle = 0x40008005;
        data[GUEST_LOBBY_2][0].mWindowParams.mExStyle = 0x200;
        strcpy(data[GUEST_LOBBY_2][0].mWindowClassName, "SysListView32");
        strcpy(data[GUEST_LOBBY_2][0].mGtxStringName, "");

        data[GUEST_LOBBY_2][1].mWindowParams.mX = 230;
        data[GUEST_LOBBY_2][1].mWindowParams.mY = 10;
        data[GUEST_LOBBY_2][1].mWindowParams.mW = 270;
        data[GUEST_LOBBY_2][1].mWindowParams.mH = 130;
        data[GUEST_LOBBY_2][1].mWindowParams.mhMenu = reinterpret_cast<HMENU>(CHATBOX_GUEST_1051);
        data[GUEST_LOBBY_2][1].mWindowParams.mStyle = 0x400018c4;
        data[GUEST_LOBBY_2][1].mWindowParams.mExStyle = 0x200;
        strcpy(data[GUEST_LOBBY_2][1].mWindowClassName, "EDIT");
        strcpy(data[GUEST_LOBBY_2][1].mGtxStringName, "");

        data[GUEST_LOBBY_2][2].mWindowParams.mX = 10;
        data[GUEST_LOBBY_2][2].mWindowParams.mY = 170;
        data[GUEST_LOBBY_2][2].mWindowParams.mW = 150;
        data[GUEST_LOBBY_2][2].mWindowParams.mH = 30;
        data[GUEST_LOBBY_2][2].mWindowParams.mhMenu = reinterpret_cast<HMENU>(IDCANCEL);
        data[GUEST_LOBBY_2][2].mWindowParams.mStyle = 0x40000000;
        data[GUEST_LOBBY_2][2].mWindowParams.mExStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][2].mWindowClassName, "BUTTON");
        strcpy(data[GUEST_LOBBY_2][2].mGtxStringName, "netui03");

        data[GUEST_LOBBY_2][3].mWindowParams.mX = 230;
        data[GUEST_LOBBY_2][3].mWindowParams.mY = 150;
        data[GUEST_LOBBY_2][3].mWindowParams.mW = 270;
        data[GUEST_LOBBY_2][3].mWindowParams.mH = 30;
        data[GUEST_LOBBY_2][3].mWindowParams.mhMenu = reinterpret_cast<HMENU>(TEXTFIELD_CHAT_GUEST_1053);
        data[GUEST_LOBBY_2][3].mWindowParams.mStyle = 0x40000080;
        data[GUEST_LOBBY_2][3].mWindowParams.mExStyle = 0x200;
        strcpy(data[GUEST_LOBBY_2][3].mWindowClassName, "EDIT");
        strcpy(data[GUEST_LOBBY_2][3].mGtxStringName, "");

        data[GUEST_LOBBY_2][4].mWindowParams.mX = 230;
        data[GUEST_LOBBY_2][4].mWindowParams.mY = 190;
        data[GUEST_LOBBY_2][4].mWindowParams.mW = 270;
        data[GUEST_LOBBY_2][4].mWindowParams.mH = 30;
        data[GUEST_LOBBY_2][4].mWindowParams.mhMenu = reinterpret_cast<HMENU>(BUTTON_SEND_CHAT_MESSAGE_GUEST_1052);
        data[GUEST_LOBBY_2][4].mWindowParams.mStyle = 0x40000000;
        data[GUEST_LOBBY_2][4].mWindowParams.mExStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][4].mWindowClassName, "BUTTON");
        strcpy(data[GUEST_LOBBY_2][4].mGtxStringName, "netui07");

        data[GUEST_LOBBY_2][5].mWindowParams.mX = 230;
        data[GUEST_LOBBY_2][5].mWindowParams.mY = 230;
        data[GUEST_LOBBY_2][5].mWindowParams.mW = 270;
        data[GUEST_LOBBY_2][5].mWindowParams.mH = 30;
        data[GUEST_LOBBY_2][5].mWindowParams.mhMenu = reinterpret_cast<HMENU>(LABEL_JOINED_GAME_MAP_1054);
        data[GUEST_LOBBY_2][5].mWindowParams.mStyle = 0x40000000;
        data[GUEST_LOBBY_2][5].mWindowParams.mExStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][5].mWindowClassName, "STATIC");
        strcpy(data[GUEST_LOBBY_2][5].mGtxStringName, "");

        data[GUEST_LOBBY_2][6].mWindowParams.mX = 230;
        data[GUEST_LOBBY_2][6].mWindowParams.mY = 260;
        data[GUEST_LOBBY_2][6].mWindowParams.mW = 270;
        data[GUEST_LOBBY_2][6].mWindowParams.mH = 30;
        data[GUEST_LOBBY_2][6].mWindowParams.mhMenu = reinterpret_cast<HMENU>(LABEL_JOINED_GAME_TYPE_1057);
        data[GUEST_LOBBY_2][6].mWindowParams.mStyle = 0x40000000;
        data[GUEST_LOBBY_2][6].mWindowParams.mExStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][6].mWindowClassName, "STATIC");
        strcpy(data[GUEST_LOBBY_2][6].mGtxStringName, "");

        data[GUEST_LOBBY_2][7].mWindowParams.mX = 230;
        data[GUEST_LOBBY_2][7].mWindowParams.mY = 290;
        data[GUEST_LOBBY_2][7].mWindowParams.mW = 270;
        data[GUEST_LOBBY_2][7].mWindowParams.mH = 30;
        data[GUEST_LOBBY_2][7].mWindowParams.mhMenu = reinterpret_cast<HMENU>(LABEL_JOINED_GAME_FRAG_LIMIT_1058);
        data[GUEST_LOBBY_2][7].mWindowParams.mStyle = 0x40000000;
        data[GUEST_LOBBY_2][7].mWindowParams.mExStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][7].mWindowClassName, "STATIC");
        strcpy(data[GUEST_LOBBY_2][7].mGtxStringName, "");

        data[GUEST_LOBBY_2][8].mWindowParams.mX = 230;
        data[GUEST_LOBBY_2][8].mWindowParams.mY = 320;
        data[GUEST_LOBBY_2][8].mWindowParams.mW = 270;
        data[GUEST_LOBBY_2][8].mWindowParams.mH = 30;
        data[GUEST_LOBBY_2][8].mWindowParams.mhMenu = reinterpret_cast<HMENU>(LABEL_JOINED_GAME_POLICE_ENABLED_1056);
        data[GUEST_LOBBY_2][8].mWindowParams.mStyle = 0x40000000;
        data[GUEST_LOBBY_2][8].mWindowParams.mExStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][8].mWindowClassName, "STATIC");
        strcpy(data[GUEST_LOBBY_2][8].mGtxStringName, "");

        data[GUEST_LOBBY_2][9].mWindowParams.mX = 230;
        data[GUEST_LOBBY_2][9].mWindowParams.mY = 350;
        data[GUEST_LOBBY_2][9].mWindowParams.mW = 270;
        data[GUEST_LOBBY_2][9].mWindowParams.mH = 30;
        data[GUEST_LOBBY_2][9].mWindowParams.mhMenu = reinterpret_cast<HMENU>(LABEL_GAME_SPEED_1055);
        data[GUEST_LOBBY_2][9].mWindowParams.mStyle = 0x40000000;
        data[GUEST_LOBBY_2][9].mWindowParams.mExStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][9].mWindowClassName, "STATIC");
        strcpy(data[GUEST_LOBBY_2][9].mGtxStringName, "");

        data[GUEST_LOBBY_2][10].mWindowParams.mX = 230;
        data[GUEST_LOBBY_2][10].mWindowParams.mY = 380;
        data[GUEST_LOBBY_2][10].mWindowParams.mW = 270;
        data[GUEST_LOBBY_2][10].mWindowParams.mH = 30;
        data[GUEST_LOBBY_2][10].mWindowParams.mhMenu = reinterpret_cast<HMENU>(LABEL_JOINED_GAME_TIME_LIMIT_1060);
        data[GUEST_LOBBY_2][10].mWindowParams.mStyle = 0x40000000;
        data[GUEST_LOBBY_2][10].mWindowParams.mExStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][10].mWindowClassName, "STATIC");
        strcpy(data[GUEST_LOBBY_2][10].mGtxStringName, "");

        // Clear remaining array for windows 2:

        data[GUEST_LOBBY_2][11].mWindowParams.mX = 0;
        data[GUEST_LOBBY_2][11].mWindowParams.mY = 0;
        data[GUEST_LOBBY_2][11].mWindowParams.mW = 0;
        data[GUEST_LOBBY_2][11].mWindowParams.mH = 0;
        data[GUEST_LOBBY_2][11].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[GUEST_LOBBY_2][11].mWindowParams.mStyle = 0x0;
        data[GUEST_LOBBY_2][11].mWindowParams.mExStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][11].mWindowClassName, "0");
        strcpy(data[GUEST_LOBBY_2][11].mGtxStringName, "0");

        data[GUEST_LOBBY_2][12].mWindowParams.mX = 0;
        data[GUEST_LOBBY_2][12].mWindowParams.mY = 0;
        data[GUEST_LOBBY_2][12].mWindowParams.mW = 0;
        data[GUEST_LOBBY_2][12].mWindowParams.mH = 0;
        data[GUEST_LOBBY_2][12].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[GUEST_LOBBY_2][12].mWindowParams.mStyle = 0x0;
        data[GUEST_LOBBY_2][12].mWindowParams.mExStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][12].mWindowClassName, "0");
        strcpy(data[GUEST_LOBBY_2][12].mGtxStringName, "0");

        data[GUEST_LOBBY_2][13].mWindowParams.mX = 0;
        data[GUEST_LOBBY_2][13].mWindowParams.mY = 0;
        data[GUEST_LOBBY_2][13].mWindowParams.mW = 0;
        data[GUEST_LOBBY_2][13].mWindowParams.mH = 0;
        data[GUEST_LOBBY_2][13].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[GUEST_LOBBY_2][13].mWindowParams.mStyle = 0x0;
        data[GUEST_LOBBY_2][13].mWindowParams.mExStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][13].mWindowClassName, "0");
        strcpy(data[GUEST_LOBBY_2][13].mGtxStringName, "0");

        data[GUEST_LOBBY_2][14].mWindowParams.mX = 0;
        data[GUEST_LOBBY_2][14].mWindowParams.mY = 0;
        data[GUEST_LOBBY_2][14].mWindowParams.mW = 0;
        data[GUEST_LOBBY_2][14].mWindowParams.mH = 0;
        data[GUEST_LOBBY_2][14].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[GUEST_LOBBY_2][14].mWindowParams.mStyle = 0x0;
        data[GUEST_LOBBY_2][14].mWindowParams.mExStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][14].mWindowClassName, "0");
        strcpy(data[GUEST_LOBBY_2][14].mGtxStringName, "0");

        data[GUEST_LOBBY_2][15].mWindowParams.mX = 0;
        data[GUEST_LOBBY_2][15].mWindowParams.mY = 0;
        data[GUEST_LOBBY_2][15].mWindowParams.mW = 0;
        data[GUEST_LOBBY_2][15].mWindowParams.mH = 0;
        data[GUEST_LOBBY_2][15].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[GUEST_LOBBY_2][15].mWindowParams.mStyle = 0x0;
        data[GUEST_LOBBY_2][15].mWindowParams.mExStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][15].mWindowClassName, "0");
        strcpy(data[GUEST_LOBBY_2][15].mGtxStringName, "0");

        data[GUEST_LOBBY_2][16].mWindowParams.mX = 0;
        data[GUEST_LOBBY_2][16].mWindowParams.mY = 0;
        data[GUEST_LOBBY_2][16].mWindowParams.mW = 0;
        data[GUEST_LOBBY_2][16].mWindowParams.mH = 0;
        data[GUEST_LOBBY_2][16].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[GUEST_LOBBY_2][16].mWindowParams.mStyle = 0x0;
        data[GUEST_LOBBY_2][16].mWindowParams.mExStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][16].mWindowClassName, "0");
        strcpy(data[GUEST_LOBBY_2][16].mGtxStringName, "0");

        data[GUEST_LOBBY_2][17].mWindowParams.mX = 0;
        data[GUEST_LOBBY_2][17].mWindowParams.mY = 0;
        data[GUEST_LOBBY_2][17].mWindowParams.mW = 0;
        data[GUEST_LOBBY_2][17].mWindowParams.mH = 0;
        data[GUEST_LOBBY_2][17].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[GUEST_LOBBY_2][17].mWindowParams.mStyle = 0x0;
        data[GUEST_LOBBY_2][17].mWindowParams.mExStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][17].mWindowClassName, "0");
        strcpy(data[GUEST_LOBBY_2][17].mGtxStringName, "0");

        data[GUEST_LOBBY_2][18].mWindowParams.mX = 0;
        data[GUEST_LOBBY_2][18].mWindowParams.mY = 0;
        data[GUEST_LOBBY_2][18].mWindowParams.mW = 0;
        data[GUEST_LOBBY_2][18].mWindowParams.mH = 0;
        data[GUEST_LOBBY_2][18].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[GUEST_LOBBY_2][18].mWindowParams.mStyle = 0x0;
        data[GUEST_LOBBY_2][18].mWindowParams.mExStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][18].mWindowClassName, "0");
        strcpy(data[GUEST_LOBBY_2][18].mGtxStringName, "0");

        data[GUEST_LOBBY_2][19].mWindowParams.mX = 0;
        data[GUEST_LOBBY_2][19].mWindowParams.mY = 0;
        data[GUEST_LOBBY_2][19].mWindowParams.mW = 0;
        data[GUEST_LOBBY_2][19].mWindowParams.mH = 0;
        data[GUEST_LOBBY_2][19].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[GUEST_LOBBY_2][19].mWindowParams.mStyle = 0x0;
        data[GUEST_LOBBY_2][19].mWindowParams.mExStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][19].mWindowClassName, "0");
        strcpy(data[GUEST_LOBBY_2][19].mGtxStringName, "0");

        data[GUEST_LOBBY_2][20].mWindowParams.mX = 0;
        data[GUEST_LOBBY_2][20].mWindowParams.mY = 0;
        data[GUEST_LOBBY_2][20].mWindowParams.mW = 0;
        data[GUEST_LOBBY_2][20].mWindowParams.mH = 0;
        data[GUEST_LOBBY_2][20].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[GUEST_LOBBY_2][20].mWindowParams.mStyle = 0x0;
        data[GUEST_LOBBY_2][20].mWindowParams.mExStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][20].mWindowClassName, "0");
        strcpy(data[GUEST_LOBBY_2][20].mGtxStringName, "0");

        data[GUEST_LOBBY_2][21].mWindowParams.mX = 0;
        data[GUEST_LOBBY_2][21].mWindowParams.mY = 0;
        data[GUEST_LOBBY_2][21].mWindowParams.mW = 0;
        data[GUEST_LOBBY_2][21].mWindowParams.mH = 0;
        data[GUEST_LOBBY_2][21].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[GUEST_LOBBY_2][21].mWindowParams.mStyle = 0x0;
        data[GUEST_LOBBY_2][21].mWindowParams.mExStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][21].mWindowClassName, "0");
        strcpy(data[GUEST_LOBBY_2][21].mGtxStringName, "0");

        data[GUEST_LOBBY_2][22].mWindowParams.mX = 0;
        data[GUEST_LOBBY_2][22].mWindowParams.mY = 0;
        data[GUEST_LOBBY_2][22].mWindowParams.mW = 0;
        data[GUEST_LOBBY_2][22].mWindowParams.mH = 0;
        data[GUEST_LOBBY_2][22].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[GUEST_LOBBY_2][22].mWindowParams.mStyle = 0x0;
        data[GUEST_LOBBY_2][22].mWindowParams.mExStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][22].mWindowClassName, "0");
        strcpy(data[GUEST_LOBBY_2][22].mGtxStringName, "0");

        data[GUEST_LOBBY_2][23].mWindowParams.mX = 0;
        data[GUEST_LOBBY_2][23].mWindowParams.mY = 0;
        data[GUEST_LOBBY_2][23].mWindowParams.mW = 0;
        data[GUEST_LOBBY_2][23].mWindowParams.mH = 0;
        data[GUEST_LOBBY_2][23].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[GUEST_LOBBY_2][23].mWindowParams.mStyle = 0x0;
        data[GUEST_LOBBY_2][23].mWindowParams.mExStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][23].mWindowClassName, "0");
        strcpy(data[GUEST_LOBBY_2][23].mGtxStringName, "0");

        data[GUEST_LOBBY_2][24].mWindowParams.mX = 0;
        data[GUEST_LOBBY_2][24].mWindowParams.mY = 0;
        data[GUEST_LOBBY_2][24].mWindowParams.mW = 0;
        data[GUEST_LOBBY_2][24].mWindowParams.mH = 0;
        data[GUEST_LOBBY_2][24].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[GUEST_LOBBY_2][24].mWindowParams.mStyle = 0x0;
        data[GUEST_LOBBY_2][24].mWindowParams.mExStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][24].mWindowClassName, "0");
        strcpy(data[GUEST_LOBBY_2][24].mGtxStringName, "0");

        data[GUEST_LOBBY_2][25].mWindowParams.mX = 0;
        data[GUEST_LOBBY_2][25].mWindowParams.mY = 0;
        data[GUEST_LOBBY_2][25].mWindowParams.mW = 0;
        data[GUEST_LOBBY_2][25].mWindowParams.mH = 0;
        data[GUEST_LOBBY_2][25].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[GUEST_LOBBY_2][25].mWindowParams.mStyle = 0x0;
        data[GUEST_LOBBY_2][25].mWindowParams.mExStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][25].mWindowClassName, "0");
        strcpy(data[GUEST_LOBBY_2][25].mGtxStringName, "0");

        data[GUEST_LOBBY_2][26].mWindowParams.mX = 0;
        data[GUEST_LOBBY_2][26].mWindowParams.mY = 0;
        data[GUEST_LOBBY_2][26].mWindowParams.mW = 0;
        data[GUEST_LOBBY_2][26].mWindowParams.mH = 0;
        data[GUEST_LOBBY_2][26].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[GUEST_LOBBY_2][26].mWindowParams.mStyle = 0x0;
        data[GUEST_LOBBY_2][26].mWindowParams.mExStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][26].mWindowClassName, "0");
        strcpy(data[GUEST_LOBBY_2][26].mGtxStringName, "0");

        data[GUEST_LOBBY_2][27].mWindowParams.mX = 0;
        data[GUEST_LOBBY_2][27].mWindowParams.mY = 0;
        data[GUEST_LOBBY_2][27].mWindowParams.mW = 0;
        data[GUEST_LOBBY_2][27].mWindowParams.mH = 0;
        data[GUEST_LOBBY_2][27].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[GUEST_LOBBY_2][27].mWindowParams.mStyle = 0x0;
        data[GUEST_LOBBY_2][27].mWindowParams.mExStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][27].mWindowClassName, "0");
        strcpy(data[GUEST_LOBBY_2][27].mGtxStringName, "0");

        data[GUEST_LOBBY_2][28].mWindowParams.mX = 0;
        data[GUEST_LOBBY_2][28].mWindowParams.mY = 0;
        data[GUEST_LOBBY_2][28].mWindowParams.mW = 0;
        data[GUEST_LOBBY_2][28].mWindowParams.mH = 0;
        data[GUEST_LOBBY_2][28].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[GUEST_LOBBY_2][28].mWindowParams.mStyle = 0x0;
        data[GUEST_LOBBY_2][28].mWindowParams.mExStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][28].mWindowClassName, "0");
        strcpy(data[GUEST_LOBBY_2][28].mGtxStringName, "0");

        data[GUEST_LOBBY_2][29].mWindowParams.mX = 0;
        data[GUEST_LOBBY_2][29].mWindowParams.mY = 0;
        data[GUEST_LOBBY_2][29].mWindowParams.mW = 0;
        data[GUEST_LOBBY_2][29].mWindowParams.mH = 0;
        data[GUEST_LOBBY_2][29].mWindowParams.mhMenu = static_cast<HMENU>(NULL);
        data[GUEST_LOBBY_2][29].mWindowParams.mStyle = 0x0;
        data[GUEST_LOBBY_2][29].mWindowParams.mExStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][29].mWindowClassName, "0");
        strcpy(data[GUEST_LOBBY_2][29].mGtxStringName, "0");
    }
    Network_UI_Control_Data data[3][30];
};

Network_UI_Control_Data_Array gUiControlDefinitions_621430;

MATCH_FUNC(0x519960)
void __stdcall WideToDbcs(char_type* pDst, wchar_t* pSrc)
{
    wchar_t c = *pSrc;
    if (c)
    {
        do
        {
            if (c > 0xFF)
            {
                *pDst = HIBYTE(c);
                pDst += 2;
                pDst[-1] = *reinterpret_cast<char_type*>(pSrc);
            }
            else
            {
                *pDst = *reinterpret_cast<char_type*>(pSrc);
                pDst++;
            }
            c = *++pSrc;
        } while (c);
        *pDst = static_cast<char_type>(c);
    }
    else
    {
        *pDst = 0;
    }
}

MATCH_FUNC(0x5199b0)
void NetUi::DbcsToWide(wchar_t* pDst, char_type* pSrc)
{
    char_type c = *pSrc;
    if (c)
    {
        do
        {
            if (c & 0x80)
            {
                *pDst = MAKEWORD(pSrc[1], c);
                pSrc += 2;
            }
            else
            {
                *pDst = static_cast<u8>(c);
                pSrc++;
            }
            c = *pSrc;
            pDst++;
        } while (c);
        *pDst = 0;
    }
    else
    {
        *pDst = 0;
    }
}

MATCH_FUNC(0x519a00)
char_type* __stdcall GetString_519A00(const char_type* pKey)
{
    LOCAL_STATIC_ARRAY(char_type, sDest, 256, 0x6F88A4);
    wchar_t* pText = gtext_0x14->Find(pKey);

    if (gtext_0x14->LangIsJapanese_452E60())
    {
        WideToDbcs(sDest, pText);
    }
    else
    {
        wcstombs(sDest, pText, 256u);
    }
    return sDest;
}

MATCH_FUNC(0x519a50)
void NetUi::GetString(wchar_t* Dest, char_type* Source, size_t MaxCount)
{
    if (gtext_0x14->LangIsJapanese_452E60())
    {
        DbcsToWide(Dest, Source);
    }
    else
    {
        mbstowcs(Dest, Source, MaxCount);
    }
}

MATCH_FUNC(0x519a90)
NetUi::NetUi()
{
    this->mhInstance = NULL;
    this->mDlgHwnd = NULL;
    this->mUnused20318 = 0;
    this->mShowingSpecificWindowIdx = 0;
    this->mLastShowingSpecificWindowIdx = 3;
    this->mUnused20084 = 0;
    this->mCount = 0;
    this->mTotalMapCount = 0;
    this->mbEndDialog = 0;
    this->mGameSettings.mTickCount = GetTickCount();
    this->mGameSettings.mGameType = 1;
    this->mGameSettings.mFragLimit = 1;
    this->mGameSettings.mMapIdx = 0;
    this->mGameSettings.mPoliceOn = 0;
    this->mGameSettings.mGameTimeLimit = 0;
    memset(mMaps, 0, sizeof(mMaps));
    gtext_0x14 = new TextTable();
    gtext_0x14->Load();
    LoadPlayerNameFromRegistry();
    EnumerateMaps();
}

MATCH_FUNC(0x519ba0)
NetUi::~NetUi()
{
    if (gtext_0x14)
    {
        delete gtext_0x14;
        gtext_0x14 = NULL;
    }
}

MATCH_FUNC(0x519bd0)
s32 NetUi::ShowNetworkUiBlocking(HINSTANCE hInstance)
{
    mhInstance = hInstance;
    gNetPlay.Set15(reinterpret_cast<int>(NetUi::cb_FillSessionList), this);
    gNetPlay.Set6(static_cast<void*>(NetUi::cb_AddPlayerToList), static_cast<void*>(this));
    gNetPlay.Set9(reinterpret_cast<int>(NetUi::cb_RemovePlayerFromList), reinterpret_cast<int>(this));
    gNetPlay.Set3_Disconnect(reinterpret_cast<int>(NetUi::cb_Disconnect), reinterpret_cast<int>(this));
    gNetPlay.Set18(reinterpret_cast<int>(NetUi::cb_SetPlayerPing), reinterpret_cast<int>(this));
    gNetPlay.Set21(reinterpret_cast<int>(NetUi::cb_AddChatMessage), reinterpret_cast<int>(this));
    gNetPlay.Set24(reinterpret_cast<int>(NetUi::cb_GameSettingsChanged), reinterpret_cast<int>(this));
    gNetPlay.Set27SavePlayerName(reinterpret_cast<int>(NetUi::cb_SavePlayerName), reinterpret_cast<int>(this));
    return DialogBoxParamA(mhInstance,
                           reinterpret_cast<LPCSTR>(103), // probably IDD_NETWORK_DIALOG
                           NULL,
                           NetUi::DialogFunc,
                           reinterpret_cast<LPARAM>(this)) == 1;
}

MATCH_FUNC(0x519c80)
int __stdcall NetUi::DialogFunc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_TIMER:
            NetUi::OnTimer(hDlg, wParam);
            break;

        case WM_HSCROLL:
        {
            HWND hwndCtl = reinterpret_cast<HWND>(lParam);
            s32 scrollCode = LOWORD(wParam);
            s32 thumbPos = static_cast<s16>(HIWORD(wParam));

            NetUi::OnWmHScroll(hDlg, hwndCtl, scrollCode, thumbPos);
            return 0;
        }
        break;

        case WM_COMMAND:
        {
            WORD notify = HIWORD(wParam);
            WORD id = LOWORD(wParam);

            NetUi::OnWmCommand(hDlg, id, reinterpret_cast<HWND>(lParam), notify);
        }
        break;

        case WM_PAINT:
            NetUi::OnPaint(hDlg);
            break;
        case WM_INITDIALOG:
            return OnInitDialog(hDlg, wParam, reinterpret_cast<NetUi*>(lParam));

        default:
            break;
    }

    return 0;
}

MATCH_FUNC(0x519d30)
s32 NetUi::cb_FillSessionList(NetUi* pThis, Network_NameList* pSessions)
{
    char_type name[260];

    SendDlgItemMessageA(pThis->Get_2031C_HWND(), LISTBOX_ALL_NET_GAMES_1000, LB_RESETCONTENT, 0, 0);
    for (u32 i = 0; i < pSessions->mCount; i++)
    {
        wcstombs(name, pSessions->mNames[i], sizeof(name));
        LRESULT item = SendDlgItemMessageA(pThis->Get_2031C_HWND(), LISTBOX_ALL_NET_GAMES_1000, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(name));
        if (item != LB_ERR && item != LB_ERRSPACE)
        {
            SendDlgItemMessageA(pThis->Get_2031C_HWND(), LISTBOX_ALL_NET_GAMES_1000, LB_SETITEMDATA, item, i);
        }
    }

    if (pSessions->mCount > 0 || gNetPlay.mbModem)
    {
        return EnableWindow(GetDlgItem(pThis->Get_2031C_HWND(), BUTTON_JOIN_GAME_1001), TRUE);
    }
    return EnableWindow(GetDlgItem(pThis->Get_2031C_HWND(), BUTTON_JOIN_GAME_1001), FALSE);
}

MATCH_FUNC(0x519e10)
void NetUi::SetDlgHwnd(HWND hDlg)
{
    mDlgHwnd = hDlg;
}

MATCH_FUNC(0x519e20)
HWND NetUi::Get_2031C_HWND()
{
    return mDlgHwnd;
}

MATCH_FUNC(0x519e30)
void NetUi::cb_AddPlayerToList(NetUi* pNetUi, wchar_t* pName, s32 player_idx)
{
    LVITEMA item;
    char_type name[260];

    if (gtext_0x14->LangIsJapanese_452E60())
    {
        WideToDbcs(name, pName);
    }
    else
    {
        wcstombs(name, pName, 260u);
    }

    switch (pNetUi->Get_20310_showing_window_idx())
    {
        case 2:
        {
            memset(&item, 0, sizeof(item));
            HWND hList = GetDlgItem(pNetUi->Get_2031C_HWND(), LISTVIEW_JOINED_PLAYERS_1050);
            item.iItem = SendMessageA(hList, LVM_GETITEMCOUNT, 0, 0);
            item.mask = LVIF_TEXT;
            item.iSubItem = 0;
            item.pszText = name;
            SendMessageA(hList, LVM_INSERTITEMA, 0, reinterpret_cast<LPARAM>(&item));
            break;
        }

        case 1:
        {
            memset(&item, 0, sizeof(item));
            HWND hList = GetDlgItem(pNetUi->Get_2031C_HWND(), LISTVIEW_HOST_PLAYERS_1024);
            s32 count = SendMessageA(hList, LVM_GETITEMCOUNT, 0, 0);
            item.mask = LVIF_TEXT;
            item.iItem = count;
            item.iSubItem = 0;
            item.pszText = name;
            SendMessageA(hList, LVM_INSERTITEMA, 0, reinterpret_cast<LPARAM>(&item));
            pNetUi->IncCount();
            pNetUi->UpdateButtonsEnabledState();
            break;
        }

        case 0:
        {
            memset(&item, 0, sizeof(item));
            HWND hList = GetDlgItem(pNetUi->Get_2031C_HWND(), LISTVIEW_JOINED_PLAYERS_1050);
            item.iItem = SendMessageA(hList, LVM_GETITEMCOUNT, 0, 0);
            item.mask = LVIF_TEXT;
            item.iSubItem = 0;
            item.pszText = name;
            SendMessageA(hList, LVM_INSERTITEMA, 0, reinterpret_cast<LPARAM>(&item));
            break;
        }
    }
}

MATCH_FUNC(0x519fd0)
void __stdcall NetUi::OnPaint(HWND hWnd)
{
    // Empty
}

MATCH_FUNC(0x519fe0)
void __stdcall NetUi::OnWmCommand(HWND hDlg, s32 id, HWND hCtl, s32 notify)
{
    Network_Buffer groupData;
    Network_Buffer playerData;
    s32 value;
    char_type chatText[128];
    char_type playerName[260];
    wchar_t textW[260];
    char_type sessionName[260];
    wchar_t hostPlayerNameW[260];

    switch (id)
    {
        case BUTTON_JOIN_GAME_1001: // "Join" button
            if (gNetPlay.mbModem)
            {
                gNetPlay.EnumSessions();
                wchar_t playerNameW[260];
                playerData.mData = NULL;
                playerData.mLen = 0;
                groupData.mData = NULL;
                groupData.mLen = 0;
                GetDlgItemTextA(hDlg, TEXTBOX_PLAYER_NAME_1004, playerName, 259);
                NetUi::GetString(playerNameW, playerName, 260);
                NetUi* pThis = reinterpret_cast<NetUi*>(GetWindowLongA(hDlg, 8));
                pThis->SetPlayNameAndSaveToRegistry(playerNameW, playerName);
                if (gNetPlay.mSessions.mSessionCount > 0)
                {
                    if (gNetPlay.JoinSession(0, playerNameW, &playerData, &groupData))
                    {
                        pThis->ApplyHostGameSettings(static_cast<const char_type*>(groupData.mData));
                        pThis->ShowSpecificWindow(2);
                    }
                }
            }
            else
            {
                wchar_t playerNameW[260];
                playerData.mData = NULL;
                playerData.mLen = 0;
                groupData.mData = NULL;
                groupData.mLen = 0;
                GetDlgItemTextA(hDlg, TEXTBOX_PLAYER_NAME_1004, playerName, 259);
                NetUi::GetString(playerNameW, playerName, 260);
                NetUi* pThis = reinterpret_cast<NetUi*>(GetWindowLongA(hDlg, 8));
                pThis->SetPlayNameAndSaveToRegistry(playerNameW, playerName);
                LRESULT sel = SendDlgItemMessageA(hDlg, LISTBOX_ALL_NET_GAMES_1000, LB_GETCURSEL, 0, 0);
                if (sel != -1)
                {
                    LRESULT idx = SendDlgItemMessageA(hDlg, LISTBOX_ALL_NET_GAMES_1000, LB_GETITEMDATA, sel, 0);
                    if (gNetPlay.JoinSession(idx, playerNameW, &playerData, &groupData))
                    {
                        pThis->ApplyHostGameSettings(static_cast<const char_type*>(groupData.mData));
                        pThis->ShowSpecificWindow(2);
                    }
                }
            }
            break;

        case LISTBOX_ALL_NET_GAMES_1000:
            if (notify == LBN_DBLCLK)   // Double click on the network game session
            {
                wchar_t playerNameW[260];
                playerData.mData = NULL;
                playerData.mLen = 0;
                groupData.mData = NULL;
                groupData.mLen = 0;
                GetDlgItemTextA(hDlg, TEXTBOX_PLAYER_NAME_1004, playerName, 259);
                NetUi::GetString(playerNameW, playerName, 260);
                NetUi* pThis = reinterpret_cast<NetUi*>(GetWindowLongA(hDlg, 8));
                pThis->SetPlayNameAndSaveToRegistry(playerNameW, playerName);
                LRESULT sel = SendDlgItemMessageA(hDlg, LISTBOX_ALL_NET_GAMES_1000, LB_GETCURSEL, 0, 0);
                if (sel != -1)
                {
                    LRESULT idx = SendDlgItemMessageA(hDlg, LISTBOX_ALL_NET_GAMES_1000, LB_GETITEMDATA, sel, 0);
                    if (gNetPlay.JoinSession(idx, playerNameW, &playerData, &groupData))
                    {
                        pThis->ShowSpecificWindow(2);
                    }
                }
            }
            break;

        case IDCANCEL:
        {
            NetUi* pThis = reinterpret_cast<NetUi*>(GetWindowLongA(hDlg, 8));
            switch (pThis->Get_20310_showing_window_idx())
            {
                case 0:
                    KillTimer(hDlg, sTimerId);
                    EndDialog(hDlg, 0);
                    break;
                case 1:
                    gNetPlay.Disconnect();
                    SendMessageA(GetDlgItem(hDlg, LISTVIEW_HOST_PLAYERS_1024), LVM_DELETEALLITEMS, 0, 0);
                    SetDlgItemTextA(hDlg, TEXTFIELD_CHAT_HOST_1025, "");
                    SetDlgItemTextA(hDlg, CHATBOX_HOST_1022, "");
                    pThis->ResetCount();
                    pThis->ShowSpecificWindow(0);
                    break;
                case 2:
                    gNetPlay.Disconnect();
                    SendMessageA(GetDlgItem(hDlg, LISTVIEW_JOINED_PLAYERS_1050), LVM_DELETEALLITEMS, 0, 0);
                    SetDlgItemTextA(hDlg, TEXTFIELD_CHAT_GUEST_1053, "");
                    SetDlgItemTextA(hDlg, CHATBOX_GUEST_1051, "");
                    pThis->ResetCount();
                    pThis->ShowSpecificWindow(0);
                    break;
            }
            break;
        }

        case BUTTON_CREATE_GAME_1002: // "Create" button
        {
            NetUi* pThis = reinterpret_cast<NetUi*>(GetWindowLongA(hDlg, 8));
            NetworkGameSettings settings = pThis->mGameSettings;
            groupData.mData = &settings;
            groupData.mLen = sizeof(NetworkGameSettings);
            playerData.mData = NULL;
            playerData.mLen = 0;
            GetDlgItemTextA(hDlg, TEXTBOX_PLAYER_NAME_1004, playerName, 259);
            sprintf(sessionName, "%s's Game", playerName);
            NetUi::GetString(hostPlayerNameW, playerName, 260);
            NetUi::GetString(textW, sessionName, 260);
            if (gNetPlay.HostSession(textW, hostPlayerNameW, &groupData, &playerData))
            {
                pThis->SetPlayNameAndSaveToRegistry(hostPlayerNameW, playerName);
                pThis->ShowSpecificWindow(1);
                pThis->SetFragsNumberAndLabel(pThis->mGameSettings.mGameType,
                                                     pThis->mGameSettings.mFragLimit,
                                                     hDlg);
            }
            break;
        }

        case BUTTON_SEND_CHAT_MESSAGE_HOST_1023: // "Send" host chat button
        {
            GetWindowLongA(hDlg, 8);
            GetDlgItemTextA(hDlg, TEXTFIELD_CHAT_HOST_1025, chatText, 127);
            NetUi::GetString(textW, chatText, 260);
            gNetPlay.SendChatMessage(textW, -1);
            SetDlgItemTextA(hDlg, TEXTFIELD_CHAT_HOST_1025, "");
            NetUi* pThis = reinterpret_cast<NetUi*>(GetWindowLongA(hDlg, 8));
            pThis->AppendChatMessage(CHATBOX_HOST_1022, chatText);
            break;
        }

        case BUTTON_START_GAME_1021: // "Start" button
        {
            gNetPlay.NetworkTick();
            Sleep(100);
            GetDlgItemTextA(hDlg, TEXTBOX_PLAYER_NAME_1004, playerName, 260);
            NetUi::GetString(textW, playerName, 260);
            NetUi* pThis = reinterpret_cast<NetUi*>(GetWindowLongA(hDlg, 8));
            pThis->SetPlayNameAndSaveToRegistry(textW, playerName);
            gNetPlay.DisableJoining();
            EndDialog(hDlg, 1); // close lobby returning 1
            break;
        }

        case BUTTON_REJECT_PLAYER_1020: // "Reject" button
        {
            NetUi* pThis = reinterpret_cast<NetUi*>(GetWindowLongA(hDlg, 8));
            HWND hList = GetDlgItem(pThis->Get_2031C_HWND(), LISTVIEW_HOST_PLAYERS_1024);
            LRESULT item = SendMessageA(hList, LVM_GETNEXTITEM, -1, LVNI_SELECTED);
            LVITEMA lvItem;
            lvItem.iSubItem = 0;
            lvItem.cchTextMax = 260;
            lvItem.pszText = sessionName;
            SendMessageA(hList, LVM_GETITEMTEXTA, item, reinterpret_cast<LPARAM>(&lvItem));
            NetUi::GetString(textW, sessionName, 260);
            if (gNetPlay.RemovePlayerByName(textW))
            {
                SendMessageA(hList, LVM_DELETEITEM, item, 0);
            }
            break;
        }

        case COMBO_MAP_LIST_1026: // Map list
            switch (notify)
            {
                case CBN_SELCHANGE:
                {
                    NetUi* pThis = reinterpret_cast<NetUi*>(GetWindowLongA(hDlg, 8));
                    LRESULT sel = SendDlgItemMessageA(hDlg, COMBO_MAP_LIST_1026, CB_GETCURSEL, 0, 0);
                    pThis->SetSetting(1, SendDlgItemMessageA(hDlg, COMBO_MAP_LIST_1026, CB_GETITEMDATA, sel, 0));
                    pThis->UpdateButtonsEnabledState();
                    break;
                }
            }
            break;

        case BUTTON_SEND_CHAT_MESSAGE_GUEST_1052: // "Send" guest chat button
        {
            NetUi* pThis = reinterpret_cast<NetUi*>(GetWindowLongA(hDlg, 8));
            GetDlgItemTextA(hDlg, TEXTFIELD_CHAT_GUEST_1053, chatText, 127);
            NetUi::GetString(textW, chatText, 260);
            gNetPlay.SendChatMessage(textW, -1);
            SetDlgItemTextA(hDlg, TEXTFIELD_CHAT_GUEST_1053, "");
            pThis->AppendChatMessage(CHATBOX_GUEST_1051, chatText);
            break;
        }

        case COMBO_GAME_TYPE_1036: // Game type
            switch (notify)
            {
                case CBN_SELCHANGE:
                {
                    NetUi* pThis = reinterpret_cast<NetUi*>(GetWindowLongA(hDlg, 8));
                    s32 gameType = SendDlgItemMessageA(hDlg, COMBO_GAME_TYPE_1036, CB_GETCURSEL, 0, 0) + 1;
                    pThis->SetSetting(3, gameType);
                    pThis->SetFragsNumberAndLabel(gameType, pThis->mGameSettings.mFragLimit, hDlg);
                    break;
                }
            }
            break;

        case TEXTBOX_FRAG_LIMIT_NUMBER_1059: // Frags/Points to win
            if (notify == EN_CHANGE)
            {
                GetDlgItemTextA(hDlg, TEXTBOX_FRAG_LIMIT_NUMBER_1059, chatText, 127);
                if (strlen(chatText))
                {
                    sscanf(chatText, "%d", &value);
                    NetUi* pThis = reinterpret_cast<NetUi*>(GetWindowLongA(hDlg, 8));
                    pThis->SetSetting(2, value);
                }
            }
            break;

        case CHECKBOX_POLICE_ENABLED_1027:
        {
            LRESULT state = SendDlgItemMessageA(hDlg, CHECKBOX_POLICE_ENABLED_1027, BM_GETSTATE, 0, 0);
            NetUi* pThis = reinterpret_cast<NetUi*>(GetWindowLongA(hDlg, 8));
            if ((state & 3) == BST_CHECKED)
            {
                pThis->SetSetting(6, 1);
            }
            else
            {
                pThis->SetSetting(6, 0);
            }
            break;
        }

        case TRACKBAR_GAME_SPEED_1031:
            if (notify == 4)
            {
                MessageBoxA(NULL, "TB_THUMBPOSITION ", "", 0);
            }
            break;

        case TEXTBOX_GAME_TIME_LIMIT_1038: // Game time limit
            if (notify == EN_CHANGE)
            {
                GetDlgItemTextA(hDlg, TEXTBOX_GAME_TIME_LIMIT_1038, chatText, 127);
                if (strlen(chatText))
                {
                    sscanf(chatText, "%d", &value);
                    NetUi* pThis = reinterpret_cast<NetUi*>(GetWindowLongA(hDlg, 8));
                    pThis->SetSetting(7, value);
                }
            }
            break;
    }
}

MATCH_FUNC(0x51a9d0)
void __stdcall NetUi::OnTimer(HWND hWnd, s32 timer_id)
{
    NetUi* pThis = reinterpret_cast<NetUi*>(GetWindowLongA(hWnd, 8));
    s32 hadSelection = 0;
    if (!pThis->GetEndDialog())
    {
        char textBuf[260];

        const LRESULT selIndex = SendDlgItemMessageA(hWnd, LISTBOX_UNKNOWN_1000, LB_GETCURSEL, 0, 0);

        if (selIndex != LB_ERR)
        {
            SendDlgItemMessageA(hWnd, LISTBOX_ALL_NET_GAMES_1000, LB_GETTEXT, selIndex, reinterpret_cast<LPARAM>(textBuf));

            hadSelection = 1;
        }

        gNetPlay.NetworkTick();

        if (hadSelection)
        {
            SendDlgItemMessageA(hWnd, LISTBOX_ALL_NET_GAMES_1000, LB_SELECTSTRING, static_cast<WPARAM>(-1), reinterpret_cast<LPARAM>(textBuf));
        }
        else
        {
            SendDlgItemMessageA(hWnd, LISTBOX_ALL_NET_GAMES_1000, LB_SETCURSEL, 0, 0);
        }
    }
    else
    {
        EndDialog(hWnd, 1);
    }
}

MATCH_FUNC(0x51aa90)
void NetUi::CreateMainUi(HWND hWndParent)
{
    for (u32 i = 0; i < 3; i++)
    {
        for (u32 j = 0; j < 30; j++)
        {
            char* pNewWindowStr;
            if (gUiControlDefinitions_621430.data[i][j].mGtxStringName[0])
            {
                pNewWindowStr = GetString_519A00(gUiControlDefinitions_621430.data[i][j].mGtxStringName);
            }
            else
            {
                pNewWindowStr = ""; // the original's empty string literal at 0x67DC88
            }

            mUiControlHwnds[i][j] = CreateWindowExA(gUiControlDefinitions_621430.data[i][j].mWindowParams.mExStyle,
                                                                 gUiControlDefinitions_621430.data[i][j].mWindowClassName,
                                                                 pNewWindowStr,
                                                                 gUiControlDefinitions_621430.data[i][j].mWindowParams.mStyle,
                                                                 gUiControlDefinitions_621430.data[i][j].mWindowParams.mX,
                                                                 gUiControlDefinitions_621430.data[i][j].mWindowParams.mY,
                                                                 gUiControlDefinitions_621430.data[i][j].mWindowParams.mW,
                                                                 gUiControlDefinitions_621430.data[i][j].mWindowParams.mH,
                                                                 hWndParent,
                                                                 gUiControlDefinitions_621430.data[i][j].mWindowParams.mhMenu,
                                                                 this->mhInstance,
                                                                 NULL);
        }
    }

    SetWindowTextA(hWndParent, GetString_519A00("netui08")); // "Network GTA2"

    // Subclass to handle enter key pressing
    mOldProc1FD70 = reinterpret_cast<WNDPROC>(GetWindowLongA(GetDlgItem(hWndParent, TEXTFIELD_CHAT_GUEST_1053), GWL_WNDPROC));
    mOldProc1FD74 = reinterpret_cast<WNDPROC>(GetWindowLongA(GetDlgItem(hWndParent, TEXTFIELD_CHAT_HOST_1025), GWL_WNDPROC));
    SetWindowLongA(GetDlgItem(hWndParent, TEXTFIELD_CHAT_GUEST_1053), GWL_WNDPROC, reinterpret_cast<LONG>(NetUi::subclass_proc));
    SetWindowLongA(GetDlgItem(hWndParent, TEXTFIELD_CHAT_HOST_1025), GWL_WNDPROC, reinterpret_cast<LONG>(NetUi::subclass_proc));

    // set range of game speed slider: 0 to 2 (that is, 3 options)
    SendDlgItemMessageA(hWndParent, TRACKBAR_GAME_SPEED_1031, TBM_SETRANGE, TRUE, MAKELONG(0, 2)); 

    SetDlgItemTextA(hWndParent, LABEL_COPS_1028, GetString_519A00("netui14")); // "Cops"
    SetDlgItemTextA(hWndParent, LABEL_TIME_LIMIT_MINUTES_1037, GetString_519A00("netui21")); // "Time Limit (Minutes)"
    NetUi::SetPlayerCountText(0, hWndParent);
    if (gNetPlay.mbModem)
    {
        EnableWindow(GetDlgItem(hWndParent, BUTTON_JOIN_GAME_1001), 1);
    }
}

MATCH_FUNC(0x51abf0)
void NetUi::ShowSpecificWindow(s32 showing_specific_window_idx)
{
    u32 i;

    // Hide old
    for (i = 0; i < 30; ++i)
    {
        ShowWindow(this->mUiControlHwnds[this->mShowingSpecificWindowIdx][i], SW_HIDE);
    }

    // Show new (using param, not the mem var)
    for (i = 0; i < 30; ++i)
    {
        ShowWindow(this->mUiControlHwnds[showing_specific_window_idx][i], SW_SHOW);
    }

    this->mLastShowingSpecificWindowIdx = this->mShowingSpecificWindowIdx;
    this->mShowingSpecificWindowIdx = showing_specific_window_idx;
}

MATCH_FUNC(0x51ac60)
s32 NetUi::OnInitDialog(HWND hWnd, s32 lParam, NetUi* thisPtr)
{
    SetWindowLongA(hWnd, 8, reinterpret_cast<LONG>(thisPtr));
    sTimerId = SetTimer(hWnd, 0xAu, 0xAu, NULL);
    thisPtr->SetDlgHwnd(hWnd);
    thisPtr->CreateMainUi(hWnd);
    thisPtr->PopulateMainUI();
    thisPtr->SetPlayerNameText();
    thisPtr->ShowSpecificWindow(0);
    return 1;
}

MATCH_FUNC(0x51acc0)
s32 NetUi::Get_20310_showing_window_idx()
{
    return mShowingSpecificWindowIdx;
}

MATCH_FUNC(0x51acd0)
void NetUi::cb_RemovePlayerFromList(NetUi* pNetUi, wchar_t* Source)
{
    LVFINDINFOA findInfo;
    char Dest[260];

    wcstombs(Dest, Source, 260u);

    switch (pNetUi->Get_20310_showing_window_idx())
    {
        case 1:
        {
            memset(&findInfo, 0, sizeof(findInfo));

            HWND hItem = GetDlgItem(pNetUi->Get_2031C_HWND(), LISTVIEW_HOST_PLAYERS_1024);

            findInfo.flags = LVFI_STRING;
            findInfo.psz = Dest;

            LRESULT sendRet = SendMessageA(hItem, LVM_FINDITEMA, 0xFFFFFFFF, reinterpret_cast<LPARAM>(&findInfo));
            SendMessageA(hItem, LVM_DELETEITEM, sendRet, 0);

            pNetUi->DecCount();
            pNetUi->UpdateButtonsEnabledState();
        }
        break;

        case 2:
        {
            memset(&findInfo, 0, sizeof(findInfo));

            HWND hItem = GetDlgItem(pNetUi->Get_2031C_HWND(), LISTVIEW_JOINED_PLAYERS_1050);

            findInfo.flags = LVFI_STRING;
            findInfo.psz = Dest;

            LRESULT sendRet = SendMessageA(hItem, LVM_FINDITEMA, 0xFFFFFFFF, reinterpret_cast<LPARAM>(&findInfo));
            SendMessageA(hItem, LVM_DELETEITEM, sendRet, 0);
        }
        break;
    }
}

MATCH_FUNC(0x51ade0)
void NetUi::cb_Disconnect(NetUi* pNetUi)
{
    gNetPlay.Disconnect();
    pNetUi->ClearListView(LISTVIEW_JOINED_PLAYERS_1050);
    pNetUi->ClearListView(LISTVIEW_HOST_PLAYERS_1024);
    pNetUi->ShowSpecificWindow(0);
}

MATCH_FUNC(0x51ae20)
void NetUi::ClearListView(s32 nIDDlgItem)
{
    HWND hwndDlg = this->mDlgHwnd;
    if (hwndDlg)
    {
        SendDlgItemMessageA(hwndDlg, nIDDlgItem, LVM_DELETEALLITEMS, 0, 0);
    }
}

#pragma warning(push)
#pragma warning(disable : 4715) // not all control paths return a value
MATCH_FUNC(0x51ae50)
s32 NetUi::cb_SetPlayerPing(NetUi* pNetUi, Network_PlayerPing* pPing)
{
    LVFINDINFOA findInfo;
    LVITEMA item;
    char_type pingText[260];
    char_type name[260];

    wcstombs(name, pPing->mName, 260u);

    switch (pNetUi->Get_20310_showing_window_idx())
    {
        case 1:
        {
            memset(&findInfo, 0, sizeof(findInfo));
            sprintf(pingText, "%d", pPing->mAvgPing);
            HWND hItem = GetDlgItem(pNetUi->Get_2031C_HWND(), LISTVIEW_HOST_PLAYERS_1024);
            findInfo.flags = LVFI_STRING;
            findInfo.psz = name;
            LRESULT idx = SendMessageA(hItem, LVM_FINDITEMA, 0xFFFFFFFF, reinterpret_cast<LPARAM>(&findInfo));
            item.iSubItem = 1;
            item.pszText = pingText;
            return SendMessageA(hItem, LVM_SETITEMTEXTA, idx, reinterpret_cast<LPARAM>(&item));
        }

        case 2:
        {
            memset(&findInfo, 0, sizeof(findInfo));
            sprintf(pingText, "%d", pPing->mAvgPing);
            HWND hItem = GetDlgItem(pNetUi->Get_2031C_HWND(), LISTVIEW_JOINED_PLAYERS_1050);
            findInfo.flags = LVFI_STRING;
            findInfo.psz = name;
            LRESULT idx = SendMessageA(hItem, LVM_FINDITEMA, 0xFFFFFFFF, reinterpret_cast<LPARAM>(&findInfo));
            item.iSubItem = 1;
            item.pszText = pingText;
            return SendMessageA(hItem, LVM_SETITEMTEXTA, idx, reinterpret_cast<LPARAM>(&item));
        }
    }
    // No return here in the original either
}
#pragma warning(pop)

MATCH_FUNC(0x51afa0)
void NetUi::PopulateMainUI()
{
    LPARAM columnInfo[8];
    memset(columnInfo, 0, sizeof(columnInfo));

    u16 colWidth = 3 * (gUiControlDefinitions_621430.data[HOST_LOBBY_1][0].mWindowParams.mW >> 2);

    columnInfo[0] = 36;
    columnInfo[3] = reinterpret_cast<LPARAM>("Player Name");
    columnInfo[7] = 0;
    SendDlgItemMessageA(mDlgHwnd, LISTVIEW_HOST_PLAYERS_1024, 0x101B, 0, reinterpret_cast<LPARAM>(columnInfo)); // 0x101B = LVM_INSERTCOLUMNA
    SendDlgItemMessageA(mDlgHwnd, LISTVIEW_HOST_PLAYERS_1024, 0x101E, 0, colWidth); // 0x101E = LVM_SETCOLUMNWIDTH
    memset(columnInfo, 0, sizeof(columnInfo));
    columnInfo[0] = 37;
    columnInfo[1] = 0;
    columnInfo[3] = reinterpret_cast<LPARAM>("Ping");
    columnInfo[7] = 1;
    SendDlgItemMessageA(mDlgHwnd, LISTVIEW_HOST_PLAYERS_1024, 0x101B, 1u, reinterpret_cast<LPARAM>(columnInfo));
    SendDlgItemMessageA(mDlgHwnd, LISTVIEW_HOST_PLAYERS_1024, 0x101E, 1u, 65534);
    memset(columnInfo, 0, sizeof(columnInfo));
    columnInfo[0] = 36;
    columnInfo[3] = reinterpret_cast<LPARAM>("Player Name");
    u16 col_width = 3 * (gUiControlDefinitions_621430.data[GUEST_LOBBY_2][0].mWindowParams.mW >> 2);
    columnInfo[7] = 0;
    SendDlgItemMessageA(mDlgHwnd, LISTVIEW_JOINED_PLAYERS_1050, 0x101B, 0, reinterpret_cast<LPARAM>(columnInfo));
    SendDlgItemMessageA(mDlgHwnd, LISTVIEW_JOINED_PLAYERS_1050, 0x101Eu, 0, col_width);
    memset(columnInfo, 0, sizeof(columnInfo));
    columnInfo[0] = 37;
    columnInfo[1] = 0;
    columnInfo[3] = reinterpret_cast<LPARAM>("Ping");
    columnInfo[7] = 1;
    SendDlgItemMessageA(mDlgHwnd, LISTVIEW_JOINED_PLAYERS_1050, 0x101B, 1u, reinterpret_cast<LPARAM>(columnInfo));
    SendDlgItemMessageA(mDlgHwnd, LISTVIEW_JOINED_PLAYERS_1050, 0x101E, 1u, 65534);

    for (u32 map_idx = 0; map_idx < static_cast<u32>(mTotalMapCount); map_idx++)
    {
        LRESULT item_idx = SendDlgItemMessageA(mDlgHwnd, COMBO_MAP_LIST_1026, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(mMaps[map_idx].mDescription));
        SendDlgItemMessageA(mDlgHwnd, COMBO_MAP_LIST_1026, CB_SETITEMDATA, item_idx, map_idx);
    }

    LRESULT item_a = SendDlgItemMessageA(mDlgHwnd, COMBO_GAME_TYPE_1036, 323u, 0, reinterpret_cast<LPARAM>(GetString_519A00("netui19")));
    SendDlgItemMessageA(mDlgHwnd, COMBO_GAME_TYPE_1036, CB_SETITEMDATA, item_a, 0);
    LRESULT item_b = SendDlgItemMessageA(mDlgHwnd, COMBO_GAME_TYPE_1036, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(GetString_519A00("netui20")));
    SendDlgItemMessageA(mDlgHwnd, COMBO_GAME_TYPE_1036, CB_SETITEMDATA, item_b, 1);
    LRESULT item_c = SendDlgItemMessageA(mDlgHwnd, COMBO_GAME_TYPE_1036, 0x143u, 0, reinterpret_cast<LPARAM>(GetString_519A00("netui22")));
    SendDlgItemMessageA(mDlgHwnd, COMBO_GAME_TYPE_1036, CB_SETITEMDATA, item_c, 2);
    NetUi::LoadGameSettingsFromRegistry();
    SendDlgItemMessageA(mDlgHwnd, COMBO_MAP_LIST_1026, CB_SETCURSEL, mGameSettings.mMapIdx, 0);
    SendDlgItemMessageA(mDlgHwnd, COMBO_GAME_TYPE_1036, CB_SETCURSEL, mGameSettings.mGameType - 1, 0);
    NetUi::SetPlayerCountText(mMaps[mGameSettings.mMapIdx].mPlayerCount, mDlgHwnd);
    NetUi::SetPoliceEnabledCheckBox(mGameSettings.mPoliceOn, mDlgHwnd);
    NetUi::SetFragsNumberAndLabel(mGameSettings.mGameType,
                                                 mGameSettings.mFragLimit,
                                                 mDlgHwnd);
    NetUi::SetGameSpeedTextLabelAndSlider(mGameSettings.mGameSpeed, mDlgHwnd);
    NetUi::SetGameTimeLimitTextBox(mGameSettings.mGameTimeLimit, mDlgHwnd);
}

#pragma warning(push)
#pragma warning(disable : 4715) // not all control paths return a value
MATCH_FUNC(0x51b2f0)
LRESULT NetUi::cb_AddChatMessage(NetUi* pNetUi, Network_ChatMessage* pMsg)
{
    char_type line[260];
    char_type name[260];
    char_type message[260];
    char_type text[2048];
    char_type newText[2048];

    wcstombs(message, pMsg->mMessage, 260u);
    wcstombs(name, pMsg->mName, 260u);
    sprintf(line, "%s : %s", name, message);

    switch (pNetUi->Get_20310_showing_window_idx())
    {
        case 1:
        {
            GetDlgItemTextA(pNetUi->Get_2031C_HWND(), CHATBOX_HOST_1022, text, 0x7FF);
            if (strlen(text))
            {
                sprintf(newText, "%s\r\n%s", text, line);
            }
            else
            {
                sprintf(newText, "%s", line);
            }
            SetDlgItemTextA(pNetUi->Get_2031C_HWND(), CHATBOX_HOST_1022, newText);
            LRESULT lineCount = SendDlgItemMessageA(pNetUi->Get_2031C_HWND(), CHATBOX_HOST_1022, EM_GETLINECOUNT, 0, 0);
            return SendDlgItemMessageA(pNetUi->Get_2031C_HWND(), CHATBOX_HOST_1022, EM_LINESCROLL, 0, lineCount);
        }

        case 2:
        {
            GetDlgItemTextA(pNetUi->Get_2031C_HWND(), CHATBOX_GUEST_1051, text, 0x7FF);
            if (strlen(text))
            {
                sprintf(newText, "%s\r\n%s", text, line);
            }
            else
            {
                sprintf(newText, "%s", line);
            }
            SetDlgItemTextA(pNetUi->Get_2031C_HWND(), CHATBOX_GUEST_1051, newText);
            LRESULT lineCount = SendDlgItemMessageA(pNetUi->Get_2031C_HWND(), CHATBOX_GUEST_1051, EM_GETLINECOUNT, 0, 0);
            return SendDlgItemMessageA(pNetUi->Get_2031C_HWND(), CHATBOX_GUEST_1051, EM_LINESCROLL, 0, lineCount);
        }
    }
    // No return here in the original either
}
#pragma warning(pop)

#pragma warning(push)
#pragma warning(disable : 4715) // not all control paths return a value
#pragma warning(disable : 4700) // name is never filled in, as in the original
MATCH_FUNC(0x51b4f0)
s32 NetUi::AppendChatMessage(s32 hmenu, const char_type* pMsg)
{
    LOCAL_STATIC(s32, sChatLineCount, 0x6F8A2C);
    char_type line[260];
    char_type name[260];
    char_type newText[2048];
    char_type text[2048];
    wchar_t playerName[260];
    char_type allText[10000];

    // The original gets our name as a wide string but then formats with the (unset) narrow buffer
    gNetPlay.GetPlayerName(playerName, 7);
    sprintf(line, "%s : %s", name, pMsg);
    sChatLineCount++;

    switch (mShowingSpecificWindowIdx)
    {
        case GUEST_LOBBY_2: // Guest
        {
            if (sChatLineCount >= 20)
            {
                // Keep only the last 10 lines
                sChatLineCount = 10;
                s32 pos = GetWindowTextA(GetDlgItem(Get_2031C_HWND(), CHATBOX_GUEST_1051), allText, sizeof(allText));
                s32 lines = 0;
                while (pos > 0 && lines < 10)
                {
                    if (allText[pos] == '\n')
                    {
                        lines++;
                    }
                    pos--;
                }
                SetWindowTextA(GetDlgItem(Get_2031C_HWND(), CHATBOX_GUEST_1051), &allText[pos]);
            }

            GetDlgItemTextA(Get_2031C_HWND(), CHATBOX_GUEST_1051, text, 0x7FF);
            if (strlen(text))
            {
                sprintf(newText, "%s\r\n%s", text, line);
            }
            else
            {
                sprintf(newText, "%s", line);
            }
            SetDlgItemTextA(Get_2031C_HWND(), CHATBOX_GUEST_1051, newText);
            LRESULT lineCount = SendDlgItemMessageA(Get_2031C_HWND(), CHATBOX_GUEST_1051, EM_GETLINECOUNT, 0, 0);
            return SendDlgItemMessageA(Get_2031C_HWND(), CHATBOX_GUEST_1051, EM_LINESCROLL, 0, lineCount);
        }

        case HOST_LOBBY_1: // Host
        {
            if (sChatLineCount >= 20)
            {
                // Keep only the last 10 lines
                sChatLineCount = 10;
                s32 pos = GetWindowTextA(GetDlgItem(Get_2031C_HWND(), CHATBOX_HOST_1022), allText, sizeof(allText));
                s32 lines = 0;
                while (pos > 0 && lines < 10)
                {
                    if (allText[pos] == '\n')
                    {
                        lines++;
                    }
                    pos--;
                }
                SetWindowTextA(GetDlgItem(Get_2031C_HWND(), CHATBOX_HOST_1022), &allText[pos]);
            }

            GetDlgItemTextA(Get_2031C_HWND(), CHATBOX_HOST_1022, text, 0x7FF);
            if (strlen(text))
            {
                sprintf(newText, "%s\r\n%s", text, line);
            }
            else
            {
                sprintf(newText, "%s", line);
            }
            SetDlgItemTextA(Get_2031C_HWND(), CHATBOX_HOST_1022, newText);
            LRESULT lineCount = SendDlgItemMessageA(Get_2031C_HWND(), CHATBOX_HOST_1022, EM_GETLINECOUNT, 0, 0);
            return SendDlgItemMessageA(Get_2031C_HWND(), CHATBOX_HOST_1022, EM_LINESCROLL, 0, lineCount);
        }
    }
    // No return here in the original either
}
#pragma warning(pop)

MATCH_FUNC(0x51b7c0)
void NetUi::SetPlayerNameText()
{
    SetDlgItemTextA(mDlgHwnd, TEXTBOX_PLAYER_NAME_1004, mPlayerName);
}

MATCH_FUNC(0x51b7e0)
void NetUi::cb_GameSettingsChanged(NetUi* pNetUi, const char_type** ppData)
{
    const char_type* pData = *ppData;
    if (pNetUi->Get_20310_showing_window_idx() == 2)
    {
        pNetUi->ApplyHostGameSettings(pData);
    }
}

#pragma warning(push)
#pragma warning(disable : 4701) // pSpeedText is used uninitialised for an unknown speed, as in the original
MATCH_FUNC(0x51b810)
void NetUi::ApplyHostGameSettings(const char_type* pData)
{
    char_type msg[520];
    wchar_t wmsg[520];
    char_type* pSpeedText;

    const NetworkGameSettings* pSettings = reinterpret_cast<const NetworkGameSettings*>(pData);
    if (pSettings)
    {
        mGameSettings = *pSettings;

        u32 i = 0;
        while (1)
        {
            if (i < static_cast<u32>(mTotalMapCount))
            {
                if (!strcmp(mMaps[i].mMmpName, pSettings->mDefaultMap))
                {
                    mGameSettings = *pSettings;
                    mGameSettings.mMapIdx = i;
                    SetDlgItemTextA(mDlgHwnd, LABEL_JOINED_GAME_MAP_1054, mMaps[i].mDescription);
                    break;
                }
                i++;
            }
            else
            {
                sprintf(msg, "### ERROR ### \r\r\nDo Not Have Map %s", pSettings->mDefaultMap);
                GetString(wmsg, msg, 260);
                gNetPlay.SendChatMessage(wmsg, -1);
                AppendChatMessage(CHATBOX_HOST_1022, msg);
                break;
            }
        }

        switch (pSettings->mGameSpeed)
        {
            case 2:
                pSpeedText = GetString_519A00("netui12");
                break;
            case 1:
                pSpeedText = GetString_519A00("netui11");
                break;
            case 0:
                pSpeedText = GetString_519A00("netui10");
                break;
        }
        SetDlgItemTextA(mDlgHwnd, LABEL_GAME_SPEED_1055, pSpeedText);

        SetJoinedGamePoliceEnabledText(mGameSettings.mPoliceOn, mDlgHwnd);
        SetJoinedGameTypeAndFragLimitText(mGameSettings.mGameType,
                                                 mGameSettings.mFragLimit,
                                                 mDlgHwnd);
        SetJoinedGameTimeLimitText(mGameSettings.mGameTimeLimit, mDlgHwnd);
    }
}
#pragma warning(pop)

#pragma warning(push)
#pragma warning(disable : 4701) // pSpeedText is used uninitialised for an unknown speed, as in the original
MATCH_FUNC(0x51b9c0)
void NetUi::SetSetting(s32 setting, s32 value)
{
    Network_Buffer data;
    char_type* pSpeedText;

    switch (setting)
    {
        case 1:
            mGameSettings.mMapIdx = value;
            strcpy(mGameSettings.mDefaultMap, mMaps[value].mMmpName);
            SetPlayerCountText(mMaps[value].mPlayerCount, mDlgHwnd);
            gRegistry.Set_Network_Setting("map_index", value);
            break;

        case 2:
            mGameSettings.mFragLimit = value;
            if (mGameSettings.mGameType == 1)
            {
                gRegistry.Set_Network_Setting("f_limit", value);
            }
            else if (mGameSettings.mGameType == 2)
            {
                gRegistry.Set_Network_Setting("s_limit", value);
            }
            break;

        case 3:
            mGameSettings.mGameType = value;
            gRegistry.Set_Network_Setting("game_type", value);
            if (mGameSettings.mGameType == 1)
            {
                mGameSettings.mFragLimit = gRegistry.Get_Network_Setting("f_limit", 3);
            }
            else if (mGameSettings.mGameType == 2)
            {
                mGameSettings.mFragLimit = gRegistry.Get_Network_Setting("s_limit", 10000);
            }
            break;

        case 6: // 10.5.exe's jump table: 6 is the police flag, 4 the tick count
            mGameSettings.mPoliceOn = value;
            gRegistry.Set_Network_Setting("police", value);
            break;

        case 4:
            mGameSettings.mTickCount = value;
            break;

        case 5:
            mGameSettings.mGameSpeed = value;
            switch (value)
            {
                case 2:
                    pSpeedText = GetString_519A00("netui12");
                    break;
                case 1:
                    pSpeedText = GetString_519A00("netui11");
                    break;
                case 0:
                    pSpeedText = GetString_519A00("netui10");
                    break;
            }
            SetDlgItemTextA(mDlgHwnd, LABEL_GAME_SPEED_TEXT_1032, pSpeedText);
            gRegistry.Set_Network_Setting("game_speed", value);
            break;

        case 7:
            mGameSettings.mGameTimeLimit = value;
            gRegistry.Set_Network_Setting("game_time_limit", value);
            break;
    }

    data.mData = &mGameSettings;
    data.mLen = sizeof(mGameSettings);
    gNetPlay.SetGameSettings(&data);
}
#pragma warning(pop)

MATCH_FUNC(0x51bbc0)
void NetUi::IncCount()
{
    mCount++;
}

MATCH_FUNC(0x51bbd0)
u32 NetUi::GetCount()
{
    return mCount;
}

MATCH_FUNC(0x51bbe0)
void NetUi::DecCount()
{
    mCount--;
}

MATCH_FUNC(0x51bbf0)
void NetUi::ResetCount()
{
    mCount = 0;
}

MATCH_FUNC(0x51bc00)
void NetUi::cb_SavePlayerName(NetUi* pThis)
{
    char String[260];
    wchar_t Dest[260];

    GetDlgItemTextA(pThis->Get_2031C_HWND(), TEXTBOX_PLAYER_NAME_1004, String, GTA2_COUNTOF(String));
    GetString(Dest, String, GTA2_COUNTOF(String));
    pThis->SetPlayNameAndSaveToRegistry(Dest, String);
    pThis->SetEndDialog(1);
}

MATCH_FUNC(0x51bc70)
void NetUi::SetEndDialog(s32 bEnd)
{
    mbEndDialog = bEnd;
}

MATCH_FUNC(0x51bc80)
s32 NetUi::GetEndDialog()
{
    return mbEndDialog;
}

#pragma function(strcpy)

MATCH_FUNC(0x51bc90)
void NetUi::LoadPlayerNameFromRegistry()
{
    DWORD Type = REG_SZ;
    BYTE Data[260];
    DWORD cbData = GTA2_COUNTOF(Data);

    HKEY phkResult;

    DWORD dwDisposition;

    if (RegCreateKeyExA(HKEY_LOCAL_MACHINE,
                        "Software\\DMA Design Ltd\\GTA2\\Network",
                        0,
                        NULL,
                        0,
                        KEY_ALL_ACCESS,
                        NULL,
                        &phkResult,
                        &dwDisposition) == ERROR_SUCCESS)
    {

        if (RegQueryValueExA(phkResult, "PlayerName", NULL, &Type, Data, &cbData) == ERROR_SUCCESS)
        {
            strcpy(mPlayerName, reinterpret_cast<char*>(Data));
            GetString(mPlayerName2, reinterpret_cast<char*>(Data), GTA2_COUNTOF(mPlayerName2));
        }
        RegCloseKey(phkResult);
    }
}

MATCH_FUNC(0x51bd40)
void NetUi::SetPlayNameAndSaveToRegistry(const wchar_t* pPlayerNameW, const char* pPlayerNameA)
{
    strcpy(mPlayerName, pPlayerNameA);
    wcscpy(mPlayerName2, pPlayerNameW);

    HKEY hKey;
    DWORD dwDisposition;
    if (RegCreateKeyExA(HKEY_LOCAL_MACHINE, "Software\\DMA Design Ltd\\GTA2\\Network", 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition) ==
        ERROR_SUCCESS)
    {
        RegSetValueExA(hKey,
                       "PlayerName",
                       0,
                       REG_SZ,
                       reinterpret_cast<const u8*>(pPlayerNameA),
                       strlen(pPlayerNameA) + 1); // +1 so the NULL is also written
        RegCloseKey(hKey);
    }
}

#pragma intrinsic(strcpy)

MATCH_FUNC(0x51bdd0)
LRESULT __stdcall NetUi::subclass_proc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    HWND parentHwnd = GetParent(hWnd);
    if (parentHwnd)
    {
        NetUi* pThis = reinterpret_cast<NetUi*>(GetWindowLongA(parentHwnd, 8));
        if (Msg == WM_KEYUP && wParam == VK_RETURN && (lParam & (KF_MENUMODE << 16)) == 0)
        {
            switch (pThis->Get_20310_showing_window_idx())
            {
                case 2:
                    pThis->OnEnterPressed(TEXTFIELD_CHAT_GUEST_1053, CHATBOX_GUEST_1051);
                    break;

                case 1:
                    pThis->OnEnterPressed(TEXTFIELD_CHAT_HOST_1025, CHATBOX_HOST_1022);
                    break;
            }
        }

        switch (pThis->Get_20310_showing_window_idx())
        {
            case 2:
                return CallWindowProcA(pThis->mOldProc1FD70, hWnd, Msg, wParam, lParam);
            case 1:
                return CallWindowProcA(pThis->mOldProc1FD74, hWnd, Msg, wParam, lParam);
        }
    }
    return 0;
}

MATCH_FUNC(0x51beb0)
void NetUi::OnEnterPressed(s32 nIDDlgItem, s32 hmenu)
{
    char_type String[128];
    wchar_t Dest[260];
    GetDlgItemTextA(mDlgHwnd, nIDDlgItem, String, 127);
    NetUi::GetString(Dest, String, 260);
    gNetPlay.SendChatMessage(Dest, -1);
    SetDlgItemTextA(mDlgHwnd, nIDDlgItem, "");
    NetUi::AppendChatMessage(hmenu, String);
}

// https://decomp.me/scratch/tQkqa
MATCH_FUNC(0x51bfa0)
void NetUi::EnumerateMaps()
{
    CHAR FileName[260];
    _WIN32_FIND_DATAA findFileData;
    Network_Enumerated_Map tmp;
    Network_Enumerated_Map maps[100];

    memset(&findFileData, 0, sizeof(findFileData));
    strcpy(FileName, "data\\");
    strcat(FileName, "*.mmp");

    u32 map_count = 0;
    HANDLE hFindFile = FindFirstFileA(FileName, &findFileData);
    if (hFindFile != INVALID_HANDLE_VALUE)
    {
        map_count = 1;
        strcpy(maps[0].mMmpName, findFileData.cFileName);
        while (FindNextFileA(hFindFile, &findFileData))
        {
            if (map_count >= 100)
            {
                break;
            }
            strcpy(maps[map_count].mMmpName, findFileData.cFileName);
            map_count++;
        }
        FindClose(hFindFile);
    }

    for (u32 i = 0; i < map_count; i++)
    {
        strcpy(FileName, "data\\");
        strcat(FileName, maps[i].mMmpName);
        GetPrivateProfileStringA("MapFiles", "GMPFile", "", maps[i].mMapName, 0x103u, FileName);
        GetPrivateProfileStringA("MapFiles", "STYFile", "", maps[i].mStyleName, 0x103u, FileName);
        GetPrivateProfileStringA("MapFiles", "SCRFile", "", maps[i].mScriptName, 0x103u, FileName);
        GetPrivateProfileStringA("MapFiles", "Description", "", maps[i].mDescription, 0x103u, FileName);
        GetPrivateProfileStringA("MapFiles", "PlayerCount", "", gTmpBuffer, 2u, FileName);
        maps[i].mPlayerCount = atoi(gTmpBuffer);

        // Only keep maps whose files all exist
        _chdir("data");
        if (GetFileAttributesA(maps[i].mMapName) != -1)
        {
            if (GetFileAttributesA(maps[i].mStyleName) != -1)
            {
                if (GetFileAttributesA(maps[i].mScriptName) != -1)
                {
                    OutputDebugStringA(mMaps[i].mMapName);
                    memcpy(&mMaps[mTotalMapCount], &maps[i], sizeof(Network_Enumerated_Map));
                    mTotalMapCount++;
                }
                else
                {
                    NetUi::ShowUnableToOpenFileError(maps[i].mMmpName, maps[i].mScriptName);
                }
            }
            else
            {
                NetUi::ShowUnableToOpenFileError(maps[i].mMmpName, maps[i].mStyleName);
            }
        }
        else
        {
            NetUi::ShowUnableToOpenFileError(maps[i].mMmpName, maps[i].mMapName);
        }
        _chdir("..");
    }

    // Bubble sort by description
    if (map_count > 0)
    {
        u32 last = map_count - 1;
        s32 bSwapped;
        do
        {
            bSwapped = 0;
            for (u16 j = 0; j < last; j++)
            {
                if (_strcmpi(mMaps[j].mDescription, mMaps[j + 1].mDescription) > 0)
                {
                    memcpy(&tmp, &mMaps[j], sizeof(Network_Enumerated_Map));
                    memcpy(&mMaps[j], &mMaps[j + 1], sizeof(Network_Enumerated_Map));
                    memcpy(&mMaps[j + 1], &tmp, sizeof(Network_Enumerated_Map));
                    bSwapped = 1;
                }
            }
        } while (bSwapped == 1);
    }
}

MATCH_FUNC(0x51c630)
void __stdcall NetUi::OnWmHScroll(HWND hWnd, HWND hTrackBar, s32 code, s32 pos)
{
    NetUi* pThis = reinterpret_cast<NetUi*>(GetWindowLongA(hWnd, 8));
    if (GetDlgCtrlID(hTrackBar) == TRACKBAR_GAME_SPEED_1031)
    {
        switch (code)
        {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 6:
            case 7:
            {
                const s32 pos2 = SendDlgItemMessageA(hWnd, TRACKBAR_GAME_SPEED_1031, TBM_GETPOS, 0, 0);
                pThis->SetSetting(5, static_cast<s32>(pos2)); // set game speed
            }
            break;

            case 5:
                return;
        }
    }
}

MATCH_FUNC(0x51c7f0)
void NetUi::CopyGameSettings(NetworkGameSettings* pSettings)
{
    if (pSettings)
    {
        memcpy(pSettings, &mGameSettings, sizeof(NetworkGameSettings));
    }
}

MATCH_FUNC(0x51c830)
void NetUi::LoadGameSettingsFromRegistry()
{
    mGameSettings.mMapIdx = gRegistry.Get_Network_Setting("map_index", 0);
    if (mGameSettings.mMapIdx >= static_cast<u32>(mTotalMapCount))
    {
        mGameSettings.mMapIdx = 0;
    }
    mGameSettings.mGameTimeLimit = gRegistry.Get_Network_Setting("game_time_limit", 1);
    mGameSettings.mGameSpeed = gRegistry.Get_Network_Setting("game_speed", 1);
    mGameSettings.mGameType = gRegistry.Get_Network_Setting("game_type", 1);
    mGameSettings.mPoliceOn = gRegistry.Get_Network_Setting("police", 0);

    if (mGameSettings.mGameType == 1)
    {
        mGameSettings.mFragLimit = gRegistry.Get_Network_Setting("f_limit", 3);
    }
    else
    {
        mGameSettings.mFragLimit = gRegistry.Get_Network_Setting("s_limit", 10000);
    }

    strcpy(mGameSettings.mDefaultMap,
           mMaps[mGameSettings.mMapIdx].mMmpName);
}

MATCH_FUNC(0x51ca10)
char_type* NetUi::GetMapName()
{
    return mMaps[mGameSettings.mMapIdx].mMapName;
}

MATCH_FUNC(0x51ca50)
char_type* NetUi::GetMapStyName()
{
    return mMaps[mGameSettings.mMapIdx].mStyleName;
}

MATCH_FUNC(0x51ca90)
char_type* NetUi::GetMapScrName()
{
    return mMaps[mGameSettings.mMapIdx].mScriptName;
}

MATCH_FUNC(0x51cad0)
void __stdcall NetUi::ShowUnableToOpenFileError(const char_type* pMmpName, const char_type* pFileName)
{
    char Text[256];
    sprintf(Text, "Unable to open file: %s in mmpfile: %s", pFileName, pMmpName);
    MessageBoxA(NULL, Text, "", MB_ICONEXCLAMATION); // TODO: the original passes its empty string global 0x67DC88
}

MATCH_FUNC(0x51cb30)
void NetUi::SetPlayerCountText(s32 count, HWND hDlg)
{
    char String[260];
    const char* pStr = GetString_519A00("netui13");
    sprintf(String, "%s %d", pStr, count);
    SetDlgItemTextA(hDlg, LABEL_PLAYERS_REQUIRED_1033, String);
}

MATCH_FUNC(0x51cbc0)
void NetUi::UpdateButtonsEnabledState()
{
    if (NetUi::GetCount() == mMaps[mGameSettings.mMapIdx].mPlayerCount)
    {
        EnableWindow(GetDlgItem(NetUi::Get_2031C_HWND(), BUTTON_START_GAME_1021), true);
    }
    else
    {
        EnableWindow(GetDlgItem(NetUi::Get_2031C_HWND(), BUTTON_START_GAME_1021), false);
    }

    if (NetUi::GetCount() > 1)
    {
        EnableWindow(GetDlgItem(NetUi::Get_2031C_HWND(), BUTTON_REJECT_PLAYER_1020), true);
    }
    else
    {
        EnableWindow(GetDlgItem(NetUi::Get_2031C_HWND(), BUTTON_REJECT_PLAYER_1020), false);
    }
}

MATCH_FUNC(0x51ccb0)
void NetUi::SetPoliceEnabledCheckBox(s32 bPoliceOn, HWND hDlg)
{
    if (bPoliceOn)
    {
        SendDlgItemMessageA(hDlg, CHECKBOX_POLICE_ENABLED_1027, BM_SETCHECK, 1, 0);
    }
    else
    {
        SendDlgItemMessageA(hDlg, CHECKBOX_POLICE_ENABLED_1027, BM_SETCHECK, 0, 0);
    }
}

MATCH_FUNC(0x51cd30)
void NetUi::SetJoinedGamePoliceEnabledText(s32 bPoliceOn, HWND hDlg)
{
    if (bPoliceOn == 1)
    {
        SetDlgItemTextA(hDlg, LABEL_JOINED_GAME_POLICE_ENABLED_1056, GetString_519A00("netui15"));
    }
    else
    {
        SetDlgItemTextA(hDlg, LABEL_JOINED_GAME_POLICE_ENABLED_1056, GetString_519A00("netui16"));
    }
}

MATCH_FUNC(0x51cdc0)
void NetUi::SetFragsNumberAndLabel(s32 gameType, s32 fragLimit, HWND hDlg)
{
    char_type* fragLimitString;
    char_type tmpBuffer[260];

    bool bHasFragLimit = false;

    if (gameType == FRAG_GAME_1)
    {
        fragLimitString = GetString_519A00("netui17");
        bHasFragLimit = true;
    }
    else if (gameType == POINTS_GAME_2)
    {
        fragLimitString = GetString_519A00("netui18");
        bHasFragLimit = true;
    }

    if (bHasFragLimit)
    {
        strcpy(tmpBuffer, fragLimitString);
        SetDlgItemTextA(hDlg, LABEL_FRAG_LIMIT_TEXT_1035, tmpBuffer);
        sprintf(tmpBuffer, "%d", fragLimit);
        SetDlgItemTextA(hDlg, TEXTBOX_FRAG_LIMIT_NUMBER_1059, tmpBuffer);
        if (NetUi::Get_20310_showing_window_idx() == 1)
        {
            ShowWindow(GetDlgItem(hDlg, TEXTBOX_FRAG_LIMIT_NUMBER_1059), SW_SHOW);
            ShowWindow(GetDlgItem(hDlg, LABEL_FRAG_LIMIT_TEXT_1035), SW_SHOW);
        }
    }
    else
    {
        if (NetUi::Get_20310_showing_window_idx() == 1) // hide if its not a frag game?
        {
            ShowWindow(GetDlgItem(hDlg, TEXTBOX_FRAG_LIMIT_NUMBER_1059), SW_HIDE);
            ShowWindow(GetDlgItem(hDlg, LABEL_FRAG_LIMIT_TEXT_1035), SW_HIDE);
        }
    }
}

// https://decomp.me/scratch/gzeUC
// The original leaves pText uninitialised for an unknown speed (it pushes whatever is in the
// argument slot), and that is what gives each case its own copy of the call.
#pragma warning(push)
#pragma warning(disable : 4701) // local variable 'pText' may be used without having been initialized
MATCH_FUNC(0x51cfc0)
void NetUi::SetGameSpeedTextLabelAndSlider(LPARAM game_speed, HWND hDlg)
{
    SendDlgItemMessageA(hDlg, TRACKBAR_GAME_SPEED_1031, 0x405, 1u, game_speed); // 0x405 = TBM_SETPOS

    LPCSTR pText;
    switch (game_speed)
    {
        case 2:
            pText = GetString_519A00("netui12");
            break;
        case 1:
            pText = GetString_519A00("netui11");
            break;
        case 0:
            pText = GetString_519A00("netui10");
            break;
    }
    SetDlgItemTextA(hDlg, LABEL_GAME_SPEED_TEXT_1032, pText);
}
#pragma warning(pop)

MATCH_FUNC(0x51d0c0)
void NetUi::SetJoinedGameTypeAndFragLimitText(s32 game_type, s32 frag_limit, HWND hDlg)
{
    char_type String[260];
    char_type Buffer[260];

    bool bIsFragOrPoints = false;

    if (game_type == FRAG_GAME_1)
    {
        strcpy(String, GetString_519A00("netui19"));
        strcpy(Buffer, GetString_519A00("netui17"));
        bIsFragOrPoints = true;
    }
    else if (game_type == POINTS_GAME_2)
    {
        strcpy(String, GetString_519A00("netui20"));
        strcpy(Buffer, GetString_519A00("netui18"));
        bIsFragOrPoints = true;
    }

    if (bIsFragOrPoints)
    {
        SetDlgItemTextA(hDlg, LABEL_JOINED_GAME_TYPE_1057, String);
        sprintf(Buffer, "%d %s", frag_limit, String);
        SetDlgItemTextA(hDlg, LABEL_JOINED_GAME_FRAG_LIMIT_1058, Buffer);
    }
    else
    {
        strcpy(String, GetString_519A00("netui22"));
        SetDlgItemTextA(hDlg, LABEL_JOINED_GAME_TYPE_1057, String);
        SetDlgItemTextA(hDlg, LABEL_JOINED_GAME_FRAG_LIMIT_1058, "");
    }
}

MATCH_FUNC(0x51d2f0)
void NetUi::SetJoinedGameTimeLimitText(s32 gameTimeLimit, HWND hDlg)
{
    char_type Buffer[260];
    char_type String[260];
    strcpy(Buffer, GetString_519A00("netui21"));
    sprintf(String, "%s: %d", Buffer, gameTimeLimit);
    SetDlgItemTextA(hDlg, LABEL_JOINED_GAME_TIME_LIMIT_1060, String);
}

MATCH_FUNC(0x51d3b0)
void NetUi::SetGameTimeLimitTextBox(s32 timeLimitMins, HWND hDlg)
{
    char_type String[260];
    sprintf(String, "%d", timeLimitMins);
    SetDlgItemTextA(hDlg, TEXTBOX_GAME_TIME_LIMIT_1038, String);
}