#include "NetUi.hpp"
#include "Frontend.hpp"
#include "Globals.hpp"
#include "NetPlay.hpp"
#include "registry.hpp"
#include "TextTable.hpp"
#include <COMMCTRL.H>
#include <direct.h>

DEFINE_GLOBAL(UINT_PTR, gTimerId_6F8A18, 0x6F8A18);
DEFINE_GLOBAL_ARRAY(char_type, Dest_6F88A4, 256, 0x6F88A4);
DEFINE_GLOBAL(s32, gChatLineCount_6F8A2C, 0x6F8A2C);

EXTERN_GLOBAL_ARRAY(char_type, gTmpBuffer_67C598, 256);

struct Network_UI_Control_Data_Array
{
    Network_UI_Control_Data_Array()
    {
        // Window 0

        data[NETWORK_MENU_0][0].field_0_windowParams.field_0_x = 10;
        data[NETWORK_MENU_0][0].field_0_windowParams.field_4_y = 10;
        data[NETWORK_MENU_0][0].field_0_windowParams.field_8_w = 300;
        data[NETWORK_MENU_0][0].field_0_windowParams.field_C_h = 300;
        data[NETWORK_MENU_0][0].field_0_windowParams.field_10_hMenu = reinterpret_cast<HMENU>(LISTBOX_ALL_NET_GAMES_1000);
        data[NETWORK_MENU_0][0].field_0_windowParams.field_14_style = 0x40010001;
        data[NETWORK_MENU_0][0].field_0_windowParams.field_18_exStyle = 0x200;
        strcpy(data[NETWORK_MENU_0][0].field_1C_window_class_name, "LISTBOX");
        strcpy(data[NETWORK_MENU_0][0].field_6C_gtx_string_name, "");

        data[NETWORK_MENU_0][1].field_0_windowParams.field_0_x = 10;
        data[NETWORK_MENU_0][1].field_0_windowParams.field_4_y = 320;
        data[NETWORK_MENU_0][1].field_0_windowParams.field_8_w = 100;
        data[NETWORK_MENU_0][1].field_0_windowParams.field_C_h = 30;
        data[NETWORK_MENU_0][1].field_0_windowParams.field_10_hMenu = reinterpret_cast<HMENU>(BUTTON_JOIN_GAME_1001);
        data[NETWORK_MENU_0][1].field_0_windowParams.field_14_style = 0x48030001;
        data[NETWORK_MENU_0][1].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][1].field_1C_window_class_name, "BUTTON");
        strcpy(data[NETWORK_MENU_0][1].field_6C_gtx_string_name, "netui01");

        data[NETWORK_MENU_0][2].field_0_windowParams.field_0_x = 110;
        data[NETWORK_MENU_0][2].field_0_windowParams.field_4_y = 320;
        data[NETWORK_MENU_0][2].field_0_windowParams.field_8_w = 100;
        data[NETWORK_MENU_0][2].field_0_windowParams.field_C_h = 30;
        data[NETWORK_MENU_0][2].field_0_windowParams.field_10_hMenu = reinterpret_cast<HMENU>(BUTTON_CREATE_GAME_1002);
        data[NETWORK_MENU_0][2].field_0_windowParams.field_14_style = 0x40010000;
        data[NETWORK_MENU_0][2].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][2].field_1C_window_class_name, "BUTTON");
        strcpy(data[NETWORK_MENU_0][2].field_6C_gtx_string_name, "netui02");

        data[NETWORK_MENU_0][3].field_0_windowParams.field_0_x = 210;
        data[NETWORK_MENU_0][3].field_0_windowParams.field_4_y = 320;
        data[NETWORK_MENU_0][3].field_0_windowParams.field_8_w = 100;
        data[NETWORK_MENU_0][3].field_0_windowParams.field_C_h = 30;
        data[NETWORK_MENU_0][3].field_0_windowParams.field_10_hMenu = reinterpret_cast<HMENU>(IDCANCEL);
        data[NETWORK_MENU_0][3].field_0_windowParams.field_14_style = 0x40010000;
        data[NETWORK_MENU_0][3].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][3].field_1C_window_class_name, "BUTTON");
        strcpy(data[NETWORK_MENU_0][3].field_6C_gtx_string_name, "netui03");

        data[NETWORK_MENU_0][4].field_0_windowParams.field_0_x = 320;
        data[NETWORK_MENU_0][4].field_0_windowParams.field_4_y = 10;
        data[NETWORK_MENU_0][4].field_0_windowParams.field_8_w = 50;
        data[NETWORK_MENU_0][4].field_0_windowParams.field_C_h = 30;
        data[NETWORK_MENU_0][4].field_0_windowParams.field_10_hMenu = reinterpret_cast<HMENU>(LABEL_PLAYER_NAME_1003);
        data[NETWORK_MENU_0][4].field_0_windowParams.field_14_style = 0x40000000;
        data[NETWORK_MENU_0][4].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][4].field_1C_window_class_name, "STATIC");
        strcpy(data[NETWORK_MENU_0][4].field_6C_gtx_string_name, "netui04");

        data[NETWORK_MENU_0][5].field_0_windowParams.field_0_x = 380;
        data[NETWORK_MENU_0][5].field_0_windowParams.field_4_y = 10;
        data[NETWORK_MENU_0][5].field_0_windowParams.field_8_w = 100;
        data[NETWORK_MENU_0][5].field_0_windowParams.field_C_h = 25;
        data[NETWORK_MENU_0][5].field_0_windowParams.field_10_hMenu = reinterpret_cast<HMENU>(TEXTBOX_PLAYER_NAME_1004);
        data[NETWORK_MENU_0][5].field_0_windowParams.field_14_style = 0x40010000;
        data[NETWORK_MENU_0][5].field_0_windowParams.field_18_exStyle = 0x200;
        strcpy(data[NETWORK_MENU_0][5].field_1C_window_class_name, "EDIT");
        strcpy(data[NETWORK_MENU_0][5].field_6C_gtx_string_name, "");

        // Clear remaining array for windows 0:

        data[NETWORK_MENU_0][6].field_0_windowParams.field_0_x = 0;
        data[NETWORK_MENU_0][6].field_0_windowParams.field_4_y = 0;
        data[NETWORK_MENU_0][6].field_0_windowParams.field_8_w = 0;
        data[NETWORK_MENU_0][6].field_0_windowParams.field_C_h = 0;
        data[NETWORK_MENU_0][6].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[NETWORK_MENU_0][6].field_0_windowParams.field_14_style = 0x0;
        data[NETWORK_MENU_0][6].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][6].field_1C_window_class_name, "");
        strcpy(data[NETWORK_MENU_0][6].field_6C_gtx_string_name, "");

        data[NETWORK_MENU_0][7].field_0_windowParams.field_0_x = 0;
        data[NETWORK_MENU_0][7].field_0_windowParams.field_4_y = 0;
        data[NETWORK_MENU_0][7].field_0_windowParams.field_8_w = 0;
        data[NETWORK_MENU_0][7].field_0_windowParams.field_C_h = 0;
        data[NETWORK_MENU_0][7].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[NETWORK_MENU_0][7].field_0_windowParams.field_14_style = 0x0;
        data[NETWORK_MENU_0][7].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][7].field_1C_window_class_name, "");
        strcpy(data[NETWORK_MENU_0][7].field_6C_gtx_string_name, "");

        data[NETWORK_MENU_0][8].field_0_windowParams.field_0_x = 0;
        data[NETWORK_MENU_0][8].field_0_windowParams.field_4_y = 0;
        data[NETWORK_MENU_0][8].field_0_windowParams.field_8_w = 0;
        data[NETWORK_MENU_0][8].field_0_windowParams.field_C_h = 0;
        data[NETWORK_MENU_0][8].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[NETWORK_MENU_0][8].field_0_windowParams.field_14_style = 0x0;
        data[NETWORK_MENU_0][8].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][8].field_1C_window_class_name, "");
        strcpy(data[NETWORK_MENU_0][8].field_6C_gtx_string_name, "");

        data[NETWORK_MENU_0][9].field_0_windowParams.field_0_x = 0;
        data[NETWORK_MENU_0][9].field_0_windowParams.field_4_y = 0;
        data[NETWORK_MENU_0][9].field_0_windowParams.field_8_w = 0;
        data[NETWORK_MENU_0][9].field_0_windowParams.field_C_h = 0;
        data[NETWORK_MENU_0][9].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[NETWORK_MENU_0][9].field_0_windowParams.field_14_style = 0x0;
        data[NETWORK_MENU_0][9].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][9].field_1C_window_class_name, "");
        strcpy(data[NETWORK_MENU_0][9].field_6C_gtx_string_name, "");

        data[NETWORK_MENU_0][10].field_0_windowParams.field_0_x = 0;
        data[NETWORK_MENU_0][10].field_0_windowParams.field_4_y = 0;
        data[NETWORK_MENU_0][10].field_0_windowParams.field_8_w = 0;
        data[NETWORK_MENU_0][10].field_0_windowParams.field_C_h = 0;
        data[NETWORK_MENU_0][10].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[NETWORK_MENU_0][10].field_0_windowParams.field_14_style = 0x0;
        data[NETWORK_MENU_0][10].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][10].field_1C_window_class_name, "");
        strcpy(data[NETWORK_MENU_0][10].field_6C_gtx_string_name, "");

        data[NETWORK_MENU_0][11].field_0_windowParams.field_0_x = 0;
        data[NETWORK_MENU_0][11].field_0_windowParams.field_4_y = 0;
        data[NETWORK_MENU_0][11].field_0_windowParams.field_8_w = 0;
        data[NETWORK_MENU_0][11].field_0_windowParams.field_C_h = 0;
        data[NETWORK_MENU_0][11].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[NETWORK_MENU_0][11].field_0_windowParams.field_14_style = 0x0;
        data[NETWORK_MENU_0][11].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][11].field_1C_window_class_name, "");
        strcpy(data[NETWORK_MENU_0][11].field_6C_gtx_string_name, "");

        data[NETWORK_MENU_0][12].field_0_windowParams.field_0_x = 0;
        data[NETWORK_MENU_0][12].field_0_windowParams.field_4_y = 0;
        data[NETWORK_MENU_0][12].field_0_windowParams.field_8_w = 0;
        data[NETWORK_MENU_0][12].field_0_windowParams.field_C_h = 0;
        data[NETWORK_MENU_0][12].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[NETWORK_MENU_0][12].field_0_windowParams.field_14_style = 0x0;
        data[NETWORK_MENU_0][12].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][12].field_1C_window_class_name, "");
        strcpy(data[NETWORK_MENU_0][12].field_6C_gtx_string_name, "");

        data[NETWORK_MENU_0][13].field_0_windowParams.field_0_x = 0;
        data[NETWORK_MENU_0][13].field_0_windowParams.field_4_y = 0;
        data[NETWORK_MENU_0][13].field_0_windowParams.field_8_w = 0;
        data[NETWORK_MENU_0][13].field_0_windowParams.field_C_h = 0;
        data[NETWORK_MENU_0][13].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[NETWORK_MENU_0][13].field_0_windowParams.field_14_style = 0x0;
        data[NETWORK_MENU_0][13].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][13].field_1C_window_class_name, "");
        strcpy(data[NETWORK_MENU_0][13].field_6C_gtx_string_name, "");

        data[NETWORK_MENU_0][14].field_0_windowParams.field_0_x = 0;
        data[NETWORK_MENU_0][14].field_0_windowParams.field_4_y = 0;
        data[NETWORK_MENU_0][14].field_0_windowParams.field_8_w = 0;
        data[NETWORK_MENU_0][14].field_0_windowParams.field_C_h = 0;
        data[NETWORK_MENU_0][14].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[NETWORK_MENU_0][14].field_0_windowParams.field_14_style = 0x0;
        data[NETWORK_MENU_0][14].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][14].field_1C_window_class_name, "");
        strcpy(data[NETWORK_MENU_0][14].field_6C_gtx_string_name, "");

        data[NETWORK_MENU_0][15].field_0_windowParams.field_0_x = 0;
        data[NETWORK_MENU_0][15].field_0_windowParams.field_4_y = 0;
        data[NETWORK_MENU_0][15].field_0_windowParams.field_8_w = 0;
        data[NETWORK_MENU_0][15].field_0_windowParams.field_C_h = 0;
        data[NETWORK_MENU_0][15].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[NETWORK_MENU_0][15].field_0_windowParams.field_14_style = 0x0;
        data[NETWORK_MENU_0][15].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][15].field_1C_window_class_name, "");
        strcpy(data[NETWORK_MENU_0][15].field_6C_gtx_string_name, "");

        data[NETWORK_MENU_0][16].field_0_windowParams.field_0_x = 0;
        data[NETWORK_MENU_0][16].field_0_windowParams.field_4_y = 0;
        data[NETWORK_MENU_0][16].field_0_windowParams.field_8_w = 0;
        data[NETWORK_MENU_0][16].field_0_windowParams.field_C_h = 0;
        data[NETWORK_MENU_0][16].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[NETWORK_MENU_0][16].field_0_windowParams.field_14_style = 0x0;
        data[NETWORK_MENU_0][16].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][16].field_1C_window_class_name, "");
        strcpy(data[NETWORK_MENU_0][16].field_6C_gtx_string_name, "");

        data[NETWORK_MENU_0][17].field_0_windowParams.field_0_x = 0;
        data[NETWORK_MENU_0][17].field_0_windowParams.field_4_y = 0;
        data[NETWORK_MENU_0][17].field_0_windowParams.field_8_w = 0;
        data[NETWORK_MENU_0][17].field_0_windowParams.field_C_h = 0;
        data[NETWORK_MENU_0][17].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[NETWORK_MENU_0][17].field_0_windowParams.field_14_style = 0x0;
        data[NETWORK_MENU_0][17].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][17].field_1C_window_class_name, "");
        strcpy(data[NETWORK_MENU_0][17].field_6C_gtx_string_name, "");

        data[NETWORK_MENU_0][18].field_0_windowParams.field_0_x = 0;
        data[NETWORK_MENU_0][18].field_0_windowParams.field_4_y = 0;
        data[NETWORK_MENU_0][18].field_0_windowParams.field_8_w = 0;
        data[NETWORK_MENU_0][18].field_0_windowParams.field_C_h = 0;
        data[NETWORK_MENU_0][18].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[NETWORK_MENU_0][18].field_0_windowParams.field_14_style = 0x0;
        data[NETWORK_MENU_0][18].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][18].field_1C_window_class_name, "");
        strcpy(data[NETWORK_MENU_0][18].field_6C_gtx_string_name, "");

        data[NETWORK_MENU_0][19].field_0_windowParams.field_0_x = 0;
        data[NETWORK_MENU_0][19].field_0_windowParams.field_4_y = 0;
        data[NETWORK_MENU_0][19].field_0_windowParams.field_8_w = 0;
        data[NETWORK_MENU_0][19].field_0_windowParams.field_C_h = 0;
        data[NETWORK_MENU_0][19].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[NETWORK_MENU_0][19].field_0_windowParams.field_14_style = 0x0;
        data[NETWORK_MENU_0][19].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][19].field_1C_window_class_name, "");
        strcpy(data[NETWORK_MENU_0][19].field_6C_gtx_string_name, "");

        data[NETWORK_MENU_0][20].field_0_windowParams.field_0_x = 0;
        data[NETWORK_MENU_0][20].field_0_windowParams.field_4_y = 0;
        data[NETWORK_MENU_0][20].field_0_windowParams.field_8_w = 0;
        data[NETWORK_MENU_0][20].field_0_windowParams.field_C_h = 0;
        data[NETWORK_MENU_0][20].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[NETWORK_MENU_0][20].field_0_windowParams.field_14_style = 0x0;
        data[NETWORK_MENU_0][20].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][20].field_1C_window_class_name, "");
        strcpy(data[NETWORK_MENU_0][20].field_6C_gtx_string_name, "");

        data[NETWORK_MENU_0][21].field_0_windowParams.field_0_x = 0;
        data[NETWORK_MENU_0][21].field_0_windowParams.field_4_y = 0;
        data[NETWORK_MENU_0][21].field_0_windowParams.field_8_w = 0;
        data[NETWORK_MENU_0][21].field_0_windowParams.field_C_h = 0;
        data[NETWORK_MENU_0][21].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[NETWORK_MENU_0][21].field_0_windowParams.field_14_style = 0x0;
        data[NETWORK_MENU_0][21].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][21].field_1C_window_class_name, "");
        strcpy(data[NETWORK_MENU_0][21].field_6C_gtx_string_name, "");

        data[NETWORK_MENU_0][22].field_0_windowParams.field_0_x = 0;
        data[NETWORK_MENU_0][22].field_0_windowParams.field_4_y = 0;
        data[NETWORK_MENU_0][22].field_0_windowParams.field_8_w = 0;
        data[NETWORK_MENU_0][22].field_0_windowParams.field_C_h = 0;
        data[NETWORK_MENU_0][22].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[NETWORK_MENU_0][22].field_0_windowParams.field_14_style = 0x0;
        data[NETWORK_MENU_0][22].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][22].field_1C_window_class_name, "");
        strcpy(data[NETWORK_MENU_0][22].field_6C_gtx_string_name, "");

        data[NETWORK_MENU_0][23].field_0_windowParams.field_0_x = 0;
        data[NETWORK_MENU_0][23].field_0_windowParams.field_4_y = 0;
        data[NETWORK_MENU_0][23].field_0_windowParams.field_8_w = 0;
        data[NETWORK_MENU_0][23].field_0_windowParams.field_C_h = 0;
        data[NETWORK_MENU_0][23].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[NETWORK_MENU_0][23].field_0_windowParams.field_14_style = 0x0;
        data[NETWORK_MENU_0][23].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][23].field_1C_window_class_name, "");
        strcpy(data[NETWORK_MENU_0][23].field_6C_gtx_string_name, "");

        data[NETWORK_MENU_0][24].field_0_windowParams.field_0_x = 0;
        data[NETWORK_MENU_0][24].field_0_windowParams.field_4_y = 0;
        data[NETWORK_MENU_0][24].field_0_windowParams.field_8_w = 0;
        data[NETWORK_MENU_0][24].field_0_windowParams.field_C_h = 0;
        data[NETWORK_MENU_0][24].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[NETWORK_MENU_0][24].field_0_windowParams.field_14_style = 0x0;
        data[NETWORK_MENU_0][24].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][24].field_1C_window_class_name, "");
        strcpy(data[NETWORK_MENU_0][24].field_6C_gtx_string_name, "");

        data[NETWORK_MENU_0][25].field_0_windowParams.field_0_x = 0;
        data[NETWORK_MENU_0][25].field_0_windowParams.field_4_y = 0;
        data[NETWORK_MENU_0][25].field_0_windowParams.field_8_w = 0;
        data[NETWORK_MENU_0][25].field_0_windowParams.field_C_h = 0;
        data[NETWORK_MENU_0][25].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[NETWORK_MENU_0][25].field_0_windowParams.field_14_style = 0x0;
        data[NETWORK_MENU_0][25].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][25].field_1C_window_class_name, "");
        strcpy(data[NETWORK_MENU_0][25].field_6C_gtx_string_name, "");

        data[NETWORK_MENU_0][26].field_0_windowParams.field_0_x = 0;
        data[NETWORK_MENU_0][26].field_0_windowParams.field_4_y = 0;
        data[NETWORK_MENU_0][26].field_0_windowParams.field_8_w = 0;
        data[NETWORK_MENU_0][26].field_0_windowParams.field_C_h = 0;
        data[NETWORK_MENU_0][26].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[NETWORK_MENU_0][26].field_0_windowParams.field_14_style = 0x0;
        data[NETWORK_MENU_0][26].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][26].field_1C_window_class_name, "");
        strcpy(data[NETWORK_MENU_0][26].field_6C_gtx_string_name, "");

        data[NETWORK_MENU_0][27].field_0_windowParams.field_0_x = 0;
        data[NETWORK_MENU_0][27].field_0_windowParams.field_4_y = 0;
        data[NETWORK_MENU_0][27].field_0_windowParams.field_8_w = 0;
        data[NETWORK_MENU_0][27].field_0_windowParams.field_C_h = 0;
        data[NETWORK_MENU_0][27].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[NETWORK_MENU_0][27].field_0_windowParams.field_14_style = 0x0;
        data[NETWORK_MENU_0][27].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][27].field_1C_window_class_name, "");
        strcpy(data[NETWORK_MENU_0][27].field_6C_gtx_string_name, "");

        data[NETWORK_MENU_0][28].field_0_windowParams.field_0_x = 0;
        data[NETWORK_MENU_0][28].field_0_windowParams.field_4_y = 0;
        data[NETWORK_MENU_0][28].field_0_windowParams.field_8_w = 0;
        data[NETWORK_MENU_0][28].field_0_windowParams.field_C_h = 0;
        data[NETWORK_MENU_0][28].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[NETWORK_MENU_0][28].field_0_windowParams.field_14_style = 0x0;
        data[NETWORK_MENU_0][28].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][28].field_1C_window_class_name, "");
        strcpy(data[NETWORK_MENU_0][28].field_6C_gtx_string_name, "");

        data[NETWORK_MENU_0][29].field_0_windowParams.field_0_x = 0;
        data[NETWORK_MENU_0][29].field_0_windowParams.field_4_y = 0;
        data[NETWORK_MENU_0][29].field_0_windowParams.field_8_w = 0;
        data[NETWORK_MENU_0][29].field_0_windowParams.field_C_h = 0;
        data[NETWORK_MENU_0][29].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[NETWORK_MENU_0][29].field_0_windowParams.field_14_style = 0x0;
        data[NETWORK_MENU_0][29].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[NETWORK_MENU_0][29].field_1C_window_class_name, "");
        strcpy(data[NETWORK_MENU_0][29].field_6C_gtx_string_name, "");

        // Window 1

        data[HOST_LOBBY_1][0].field_0_windowParams.field_0_x = 10;
        data[HOST_LOBBY_1][0].field_0_windowParams.field_4_y = 10;
        data[HOST_LOBBY_1][0].field_0_windowParams.field_8_w = 210;
        data[HOST_LOBBY_1][0].field_0_windowParams.field_C_h = 130;
        data[HOST_LOBBY_1][0].field_0_windowParams.field_10_hMenu = reinterpret_cast<HMENU>(LISTVIEW_HOST_PLAYERS_1024);
        data[HOST_LOBBY_1][0].field_0_windowParams.field_14_style = 0x40008005;
        data[HOST_LOBBY_1][0].field_0_windowParams.field_18_exStyle = 0x200;
        strcpy(data[HOST_LOBBY_1][0].field_1C_window_class_name, "SysListView32");
        strcpy(data[HOST_LOBBY_1][0].field_6C_gtx_string_name, "");

        data[HOST_LOBBY_1][1].field_0_windowParams.field_0_x = 230;
        data[HOST_LOBBY_1][1].field_0_windowParams.field_4_y = 10;
        data[HOST_LOBBY_1][1].field_0_windowParams.field_8_w = 270;
        data[HOST_LOBBY_1][1].field_0_windowParams.field_C_h = 130;
        data[HOST_LOBBY_1][1].field_0_windowParams.field_10_hMenu = reinterpret_cast<HMENU>(CHATBOX_HOST_1022);
        data[HOST_LOBBY_1][1].field_0_windowParams.field_14_style = 0x400018c4;
        data[HOST_LOBBY_1][1].field_0_windowParams.field_18_exStyle = 0x200;
        strcpy(data[HOST_LOBBY_1][1].field_1C_window_class_name, "EDIT");
        strcpy(data[HOST_LOBBY_1][1].field_6C_gtx_string_name, "");

        data[HOST_LOBBY_1][2].field_0_windowParams.field_0_x = 10;
        data[HOST_LOBBY_1][2].field_0_windowParams.field_4_y = 170;
        data[HOST_LOBBY_1][2].field_0_windowParams.field_8_w = 100;
        data[HOST_LOBBY_1][2].field_0_windowParams.field_C_h = 30;
        data[HOST_LOBBY_1][2].field_0_windowParams.field_10_hMenu = reinterpret_cast<HMENU>(BUTTON_REJECT_PLAYER_1020);
        data[HOST_LOBBY_1][2].field_0_windowParams.field_14_style = 0x48030000;
        data[HOST_LOBBY_1][2].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[HOST_LOBBY_1][2].field_1C_window_class_name, "BUTTON");
        strcpy(data[HOST_LOBBY_1][2].field_6C_gtx_string_name, "netui05");

        data[HOST_LOBBY_1][3].field_0_windowParams.field_0_x = 10;
        data[HOST_LOBBY_1][3].field_0_windowParams.field_4_y = 210;
        data[HOST_LOBBY_1][3].field_0_windowParams.field_8_w = 100;
        data[HOST_LOBBY_1][3].field_0_windowParams.field_C_h = 30;
        data[HOST_LOBBY_1][3].field_0_windowParams.field_10_hMenu = reinterpret_cast<HMENU>(BUTTON_START_GAME_1021);
        data[HOST_LOBBY_1][3].field_0_windowParams.field_14_style = 0x48010000;
        data[HOST_LOBBY_1][3].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[HOST_LOBBY_1][3].field_1C_window_class_name, "BUTTON");
        strcpy(data[HOST_LOBBY_1][3].field_6C_gtx_string_name, "netui06");

        data[HOST_LOBBY_1][4].field_0_windowParams.field_0_x = 10;
        data[HOST_LOBBY_1][4].field_0_windowParams.field_4_y = 250;
        data[HOST_LOBBY_1][4].field_0_windowParams.field_8_w = 100;
        data[HOST_LOBBY_1][4].field_0_windowParams.field_C_h = 30;
        data[HOST_LOBBY_1][4].field_0_windowParams.field_10_hMenu = reinterpret_cast<HMENU>(IDCANCEL);
        data[HOST_LOBBY_1][4].field_0_windowParams.field_14_style = 0x40010000;
        data[HOST_LOBBY_1][4].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[HOST_LOBBY_1][4].field_1C_window_class_name, "BUTTON");
        strcpy(data[HOST_LOBBY_1][4].field_6C_gtx_string_name, "netui03");

        data[HOST_LOBBY_1][5].field_0_windowParams.field_0_x = 230;
        data[HOST_LOBBY_1][5].field_0_windowParams.field_4_y = 150;
        data[HOST_LOBBY_1][5].field_0_windowParams.field_8_w = 270;
        data[HOST_LOBBY_1][5].field_0_windowParams.field_C_h = 30;
        data[HOST_LOBBY_1][5].field_0_windowParams.field_10_hMenu = reinterpret_cast<HMENU>(TEXTFIELD_CHAT_HOST_1025);
        data[HOST_LOBBY_1][5].field_0_windowParams.field_14_style = 0x40031080;
        data[HOST_LOBBY_1][5].field_0_windowParams.field_18_exStyle = 0x200;
        strcpy(data[HOST_LOBBY_1][5].field_1C_window_class_name, "EDIT");
        strcpy(data[HOST_LOBBY_1][5].field_6C_gtx_string_name, "");

        data[HOST_LOBBY_1][6].field_0_windowParams.field_0_x = 230;
        data[HOST_LOBBY_1][6].field_0_windowParams.field_4_y = 190;
        data[HOST_LOBBY_1][6].field_0_windowParams.field_8_w = 270;
        data[HOST_LOBBY_1][6].field_0_windowParams.field_C_h = 30;
        data[HOST_LOBBY_1][6].field_0_windowParams.field_10_hMenu = reinterpret_cast<HMENU>(BUTTON_SEND_CHAT_MESSAGE_HOST_1023);
        data[HOST_LOBBY_1][6].field_0_windowParams.field_14_style = 0x40010000;
        data[HOST_LOBBY_1][6].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[HOST_LOBBY_1][6].field_1C_window_class_name, "BUTTON");
        strcpy(data[HOST_LOBBY_1][6].field_6C_gtx_string_name, "netui07");

        data[HOST_LOBBY_1][7].field_0_windowParams.field_0_x = 230;
        data[HOST_LOBBY_1][7].field_0_windowParams.field_4_y = 230;
        data[HOST_LOBBY_1][7].field_0_windowParams.field_8_w = 270;
        data[HOST_LOBBY_1][7].field_0_windowParams.field_C_h = 130;
        data[HOST_LOBBY_1][7].field_0_windowParams.field_10_hMenu = reinterpret_cast<HMENU>(COMBO_MAP_LIST_1026);
        data[HOST_LOBBY_1][7].field_0_windowParams.field_14_style = 0x40210243;
        data[HOST_LOBBY_1][7].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[HOST_LOBBY_1][7].field_1C_window_class_name, "COMBOBOX");
        strcpy(data[HOST_LOBBY_1][7].field_6C_gtx_string_name, "");

        data[HOST_LOBBY_1][8].field_0_windowParams.field_0_x = 230;
        data[HOST_LOBBY_1][8].field_0_windowParams.field_4_y = 260;
        data[HOST_LOBBY_1][8].field_0_windowParams.field_8_w = 200;
        data[HOST_LOBBY_1][8].field_0_windowParams.field_C_h = 30;
        data[HOST_LOBBY_1][8].field_0_windowParams.field_10_hMenu = reinterpret_cast<HMENU>(LABEL_PLAYERS_REQUIRED_1033);
        data[HOST_LOBBY_1][8].field_0_windowParams.field_14_style = 0x40000000;
        data[HOST_LOBBY_1][8].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[HOST_LOBBY_1][8].field_1C_window_class_name, "STATIC");
        strcpy(data[HOST_LOBBY_1][8].field_6C_gtx_string_name, "");

        data[HOST_LOBBY_1][9].field_0_windowParams.field_0_x = 10;
        data[HOST_LOBBY_1][9].field_0_windowParams.field_4_y = 300;
        data[HOST_LOBBY_1][9].field_0_windowParams.field_8_w = 200;
        data[HOST_LOBBY_1][9].field_0_windowParams.field_C_h = 30;
        data[HOST_LOBBY_1][9].field_0_windowParams.field_10_hMenu = reinterpret_cast<HMENU>(TRACKBAR_GAME_SPEED_1031);
        data[HOST_LOBBY_1][9].field_0_windowParams.field_14_style = 0x40000001;
        data[HOST_LOBBY_1][9].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[HOST_LOBBY_1][9].field_1C_window_class_name, "msctls_trackbar32");
        strcpy(data[HOST_LOBBY_1][9].field_6C_gtx_string_name, "netui09");

        data[HOST_LOBBY_1][10].field_0_windowParams.field_0_x = 10;
        data[HOST_LOBBY_1][10].field_0_windowParams.field_4_y = 340;
        data[HOST_LOBBY_1][10].field_0_windowParams.field_8_w = 200;
        data[HOST_LOBBY_1][10].field_0_windowParams.field_C_h = 30;
        data[HOST_LOBBY_1][10].field_0_windowParams.field_10_hMenu = reinterpret_cast<HMENU>(LABEL_GAME_SPEED_TEXT_1032);
        data[HOST_LOBBY_1][10].field_0_windowParams.field_14_style = 0x40000000;
        data[HOST_LOBBY_1][10].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[HOST_LOBBY_1][10].field_1C_window_class_name, "STATIC");
        strcpy(data[HOST_LOBBY_1][10].field_6C_gtx_string_name, "");

        data[HOST_LOBBY_1][11].field_0_windowParams.field_0_x = 230;
        data[HOST_LOBBY_1][11].field_0_windowParams.field_4_y = 290;
        data[HOST_LOBBY_1][11].field_0_windowParams.field_8_w = 130;
        data[HOST_LOBBY_1][11].field_0_windowParams.field_C_h = 130;
        data[HOST_LOBBY_1][11].field_0_windowParams.field_10_hMenu = reinterpret_cast<HMENU>(COMBO_GAME_TYPE_1036);
        data[HOST_LOBBY_1][11].field_0_windowParams.field_14_style = 0x40210243;
        data[HOST_LOBBY_1][11].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[HOST_LOBBY_1][11].field_1C_window_class_name, "COMBOBOX");
        strcpy(data[HOST_LOBBY_1][11].field_6C_gtx_string_name, "");

        data[HOST_LOBBY_1][12].field_0_windowParams.field_0_x = 230;
        data[HOST_LOBBY_1][12].field_0_windowParams.field_4_y = 330;
        data[HOST_LOBBY_1][12].field_0_windowParams.field_8_w = 60;
        data[HOST_LOBBY_1][12].field_0_windowParams.field_C_h = 26;
        data[HOST_LOBBY_1][12].field_0_windowParams.field_10_hMenu = reinterpret_cast<HMENU>(TEXTBOX_FRAG_LIMIT_NUMBER_1059);
        data[HOST_LOBBY_1][12].field_0_windowParams.field_14_style = 0x40033000;
        data[HOST_LOBBY_1][12].field_0_windowParams.field_18_exStyle = 0x1200;
        strcpy(data[HOST_LOBBY_1][12].field_1C_window_class_name, "EDIT");
        strcpy(data[HOST_LOBBY_1][12].field_6C_gtx_string_name, "");

        data[HOST_LOBBY_1][13].field_0_windowParams.field_0_x = 300;
        data[HOST_LOBBY_1][13].field_0_windowParams.field_4_y = 330;
        data[HOST_LOBBY_1][13].field_0_windowParams.field_8_w = 200;
        data[HOST_LOBBY_1][13].field_0_windowParams.field_C_h = 30;
        data[HOST_LOBBY_1][13].field_0_windowParams.field_10_hMenu = reinterpret_cast<HMENU>(LABEL_FRAG_LIMIT_TEXT_1035);
        data[HOST_LOBBY_1][13].field_0_windowParams.field_14_style = 0x40000000;
        data[HOST_LOBBY_1][13].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[HOST_LOBBY_1][13].field_1C_window_class_name, "STATIC");
        strcpy(data[HOST_LOBBY_1][13].field_6C_gtx_string_name, "");

        data[HOST_LOBBY_1][14].field_0_windowParams.field_0_x = 230;
        data[HOST_LOBBY_1][14].field_0_windowParams.field_4_y = 370;
        data[HOST_LOBBY_1][14].field_0_windowParams.field_8_w = 60;
        data[HOST_LOBBY_1][14].field_0_windowParams.field_C_h = 26;
        data[HOST_LOBBY_1][14].field_0_windowParams.field_10_hMenu = reinterpret_cast<HMENU>(TEXTBOX_GAME_TIME_LIMIT_1038);
        data[HOST_LOBBY_1][14].field_0_windowParams.field_14_style = 0x40033000;
        data[HOST_LOBBY_1][14].field_0_windowParams.field_18_exStyle = 0x1200;
        strcpy(data[HOST_LOBBY_1][14].field_1C_window_class_name, "EDIT");
        strcpy(data[HOST_LOBBY_1][14].field_6C_gtx_string_name, "");

        data[HOST_LOBBY_1][15].field_0_windowParams.field_0_x = 300;
        data[HOST_LOBBY_1][15].field_0_windowParams.field_4_y = 370;
        data[HOST_LOBBY_1][15].field_0_windowParams.field_8_w = 200;
        data[HOST_LOBBY_1][15].field_0_windowParams.field_C_h = 30;
        data[HOST_LOBBY_1][15].field_0_windowParams.field_10_hMenu = reinterpret_cast<HMENU>(LABEL_TIME_LIMIT_MINUTES_1037);
        data[HOST_LOBBY_1][15].field_0_windowParams.field_14_style = 0x40000000;
        data[HOST_LOBBY_1][15].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[HOST_LOBBY_1][15].field_1C_window_class_name, "STATIC");
        strcpy(data[HOST_LOBBY_1][15].field_6C_gtx_string_name, "");

        data[HOST_LOBBY_1][16].field_0_windowParams.field_0_x = 230;
        data[HOST_LOBBY_1][16].field_0_windowParams.field_4_y = 400;
        data[HOST_LOBBY_1][16].field_0_windowParams.field_8_w = 20;
        data[HOST_LOBBY_1][16].field_0_windowParams.field_C_h = 30;
        data[HOST_LOBBY_1][16].field_0_windowParams.field_10_hMenu = reinterpret_cast<HMENU>(CHECKBOX_POLICE_ENABLED_1027);
        data[HOST_LOBBY_1][16].field_0_windowParams.field_14_style = 0x40000003;
        data[HOST_LOBBY_1][16].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[HOST_LOBBY_1][16].field_1C_window_class_name, "BUTTON");
        strcpy(data[HOST_LOBBY_1][16].field_6C_gtx_string_name, "");

        data[HOST_LOBBY_1][17].field_0_windowParams.field_0_x = 250;
        data[HOST_LOBBY_1][17].field_0_windowParams.field_4_y = 405;
        data[HOST_LOBBY_1][17].field_0_windowParams.field_8_w = 200;
        data[HOST_LOBBY_1][17].field_0_windowParams.field_C_h = 30;
        data[HOST_LOBBY_1][17].field_0_windowParams.field_10_hMenu = reinterpret_cast<HMENU>(LABEL_COPS_1028);
        data[HOST_LOBBY_1][17].field_0_windowParams.field_14_style = 0x40000000;
        data[HOST_LOBBY_1][17].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[HOST_LOBBY_1][17].field_1C_window_class_name, "STATIC");
        strcpy(data[HOST_LOBBY_1][17].field_6C_gtx_string_name, "");

        // Clear remaining array for windows 1:

        data[HOST_LOBBY_1][18].field_0_windowParams.field_0_x = 0;
        data[HOST_LOBBY_1][18].field_0_windowParams.field_4_y = 0;
        data[HOST_LOBBY_1][18].field_0_windowParams.field_8_w = 0;
        data[HOST_LOBBY_1][18].field_0_windowParams.field_C_h = 0;
        data[HOST_LOBBY_1][18].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[HOST_LOBBY_1][18].field_0_windowParams.field_14_style = 0x0;
        data[HOST_LOBBY_1][18].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[HOST_LOBBY_1][18].field_1C_window_class_name, "");
        strcpy(data[HOST_LOBBY_1][18].field_6C_gtx_string_name, "");

        data[HOST_LOBBY_1][19].field_0_windowParams.field_0_x = 0;
        data[HOST_LOBBY_1][19].field_0_windowParams.field_4_y = 0;
        data[HOST_LOBBY_1][19].field_0_windowParams.field_8_w = 0;
        data[HOST_LOBBY_1][19].field_0_windowParams.field_C_h = 0;
        data[HOST_LOBBY_1][19].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[HOST_LOBBY_1][19].field_0_windowParams.field_14_style = 0x0;
        data[HOST_LOBBY_1][19].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[HOST_LOBBY_1][19].field_1C_window_class_name, "");
        strcpy(data[HOST_LOBBY_1][19].field_6C_gtx_string_name, "");

        data[HOST_LOBBY_1][20].field_0_windowParams.field_0_x = 0;
        data[HOST_LOBBY_1][20].field_0_windowParams.field_4_y = 0;
        data[HOST_LOBBY_1][20].field_0_windowParams.field_8_w = 0;
        data[HOST_LOBBY_1][20].field_0_windowParams.field_C_h = 0;
        data[HOST_LOBBY_1][20].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[HOST_LOBBY_1][20].field_0_windowParams.field_14_style = 0x0;
        data[HOST_LOBBY_1][20].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[HOST_LOBBY_1][20].field_1C_window_class_name, "");
        strcpy(data[HOST_LOBBY_1][20].field_6C_gtx_string_name, "");

        data[HOST_LOBBY_1][21].field_0_windowParams.field_0_x = 0;
        data[HOST_LOBBY_1][21].field_0_windowParams.field_4_y = 0;
        data[HOST_LOBBY_1][21].field_0_windowParams.field_8_w = 0;
        data[HOST_LOBBY_1][21].field_0_windowParams.field_C_h = 0;
        data[HOST_LOBBY_1][21].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[HOST_LOBBY_1][21].field_0_windowParams.field_14_style = 0x0;
        data[HOST_LOBBY_1][21].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[HOST_LOBBY_1][21].field_1C_window_class_name, "");
        strcpy(data[HOST_LOBBY_1][21].field_6C_gtx_string_name, "");

        data[HOST_LOBBY_1][22].field_0_windowParams.field_0_x = 0;
        data[HOST_LOBBY_1][22].field_0_windowParams.field_4_y = 0;
        data[HOST_LOBBY_1][22].field_0_windowParams.field_8_w = 0;
        data[HOST_LOBBY_1][22].field_0_windowParams.field_C_h = 0;
        data[HOST_LOBBY_1][22].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[HOST_LOBBY_1][22].field_0_windowParams.field_14_style = 0x0;
        data[HOST_LOBBY_1][22].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[HOST_LOBBY_1][22].field_1C_window_class_name, "");
        strcpy(data[HOST_LOBBY_1][22].field_6C_gtx_string_name, "");

        data[HOST_LOBBY_1][23].field_0_windowParams.field_0_x = 0;
        data[HOST_LOBBY_1][23].field_0_windowParams.field_4_y = 0;
        data[HOST_LOBBY_1][23].field_0_windowParams.field_8_w = 0;
        data[HOST_LOBBY_1][23].field_0_windowParams.field_C_h = 0;
        data[HOST_LOBBY_1][23].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[HOST_LOBBY_1][23].field_0_windowParams.field_14_style = 0x0;
        data[HOST_LOBBY_1][23].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[HOST_LOBBY_1][23].field_1C_window_class_name, "");
        strcpy(data[HOST_LOBBY_1][23].field_6C_gtx_string_name, "");

        data[HOST_LOBBY_1][24].field_0_windowParams.field_0_x = 0;
        data[HOST_LOBBY_1][24].field_0_windowParams.field_4_y = 0;
        data[HOST_LOBBY_1][24].field_0_windowParams.field_8_w = 0;
        data[HOST_LOBBY_1][24].field_0_windowParams.field_C_h = 0;
        data[HOST_LOBBY_1][24].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[HOST_LOBBY_1][24].field_0_windowParams.field_14_style = 0x0;
        data[HOST_LOBBY_1][24].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[HOST_LOBBY_1][24].field_1C_window_class_name, "");
        strcpy(data[HOST_LOBBY_1][24].field_6C_gtx_string_name, "");

        data[HOST_LOBBY_1][25].field_0_windowParams.field_0_x = 0;
        data[HOST_LOBBY_1][25].field_0_windowParams.field_4_y = 0;
        data[HOST_LOBBY_1][25].field_0_windowParams.field_8_w = 0;
        data[HOST_LOBBY_1][25].field_0_windowParams.field_C_h = 0;
        data[HOST_LOBBY_1][25].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[HOST_LOBBY_1][25].field_0_windowParams.field_14_style = 0x0;
        data[HOST_LOBBY_1][25].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[HOST_LOBBY_1][25].field_1C_window_class_name, "");
        strcpy(data[HOST_LOBBY_1][25].field_6C_gtx_string_name, "");

        data[HOST_LOBBY_1][26].field_0_windowParams.field_0_x = 0;
        data[HOST_LOBBY_1][26].field_0_windowParams.field_4_y = 0;
        data[HOST_LOBBY_1][26].field_0_windowParams.field_8_w = 0;
        data[HOST_LOBBY_1][26].field_0_windowParams.field_C_h = 0;
        data[HOST_LOBBY_1][26].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[HOST_LOBBY_1][26].field_0_windowParams.field_14_style = 0x0;
        data[HOST_LOBBY_1][26].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[HOST_LOBBY_1][26].field_1C_window_class_name, "");
        strcpy(data[HOST_LOBBY_1][26].field_6C_gtx_string_name, "");

        data[HOST_LOBBY_1][27].field_0_windowParams.field_0_x = 0;
        data[HOST_LOBBY_1][27].field_0_windowParams.field_4_y = 0;
        data[HOST_LOBBY_1][27].field_0_windowParams.field_8_w = 0;
        data[HOST_LOBBY_1][27].field_0_windowParams.field_C_h = 0;
        data[HOST_LOBBY_1][27].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[HOST_LOBBY_1][27].field_0_windowParams.field_14_style = 0x0;
        data[HOST_LOBBY_1][27].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[HOST_LOBBY_1][27].field_1C_window_class_name, "");
        strcpy(data[HOST_LOBBY_1][27].field_6C_gtx_string_name, "");

        data[HOST_LOBBY_1][28].field_0_windowParams.field_0_x = 0;
        data[HOST_LOBBY_1][28].field_0_windowParams.field_4_y = 0;
        data[HOST_LOBBY_1][28].field_0_windowParams.field_8_w = 0;
        data[HOST_LOBBY_1][28].field_0_windowParams.field_C_h = 0;
        data[HOST_LOBBY_1][28].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[HOST_LOBBY_1][28].field_0_windowParams.field_14_style = 0x0;
        data[HOST_LOBBY_1][28].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[HOST_LOBBY_1][28].field_1C_window_class_name, "");
        strcpy(data[HOST_LOBBY_1][28].field_6C_gtx_string_name, "");

        data[HOST_LOBBY_1][29].field_0_windowParams.field_0_x = 0;
        data[HOST_LOBBY_1][29].field_0_windowParams.field_4_y = 0;
        data[HOST_LOBBY_1][29].field_0_windowParams.field_8_w = 0;
        data[HOST_LOBBY_1][29].field_0_windowParams.field_C_h = 0;
        data[HOST_LOBBY_1][29].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[HOST_LOBBY_1][29].field_0_windowParams.field_14_style = 0x0;
        data[HOST_LOBBY_1][29].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[HOST_LOBBY_1][29].field_1C_window_class_name, "");
        strcpy(data[HOST_LOBBY_1][29].field_6C_gtx_string_name, "");

        // Window 2

        data[GUEST_LOBBY_2][0].field_0_windowParams.field_0_x = 10;
        data[GUEST_LOBBY_2][0].field_0_windowParams.field_4_y = 10;
        data[GUEST_LOBBY_2][0].field_0_windowParams.field_8_w = 210;
        data[GUEST_LOBBY_2][0].field_0_windowParams.field_C_h = 130;
        data[GUEST_LOBBY_2][0].field_0_windowParams.field_10_hMenu = reinterpret_cast<HMENU>(LISTVIEW_JOINED_PLAYERS_1050);
        data[GUEST_LOBBY_2][0].field_0_windowParams.field_14_style = 0x40008005;
        data[GUEST_LOBBY_2][0].field_0_windowParams.field_18_exStyle = 0x200;
        strcpy(data[GUEST_LOBBY_2][0].field_1C_window_class_name, "SysListView32");
        strcpy(data[GUEST_LOBBY_2][0].field_6C_gtx_string_name, "");

        data[GUEST_LOBBY_2][1].field_0_windowParams.field_0_x = 230;
        data[GUEST_LOBBY_2][1].field_0_windowParams.field_4_y = 10;
        data[GUEST_LOBBY_2][1].field_0_windowParams.field_8_w = 270;
        data[GUEST_LOBBY_2][1].field_0_windowParams.field_C_h = 130;
        data[GUEST_LOBBY_2][1].field_0_windowParams.field_10_hMenu = reinterpret_cast<HMENU>(CHATBOX_GUEST_1051);
        data[GUEST_LOBBY_2][1].field_0_windowParams.field_14_style = 0x400018c4;
        data[GUEST_LOBBY_2][1].field_0_windowParams.field_18_exStyle = 0x200;
        strcpy(data[GUEST_LOBBY_2][1].field_1C_window_class_name, "EDIT");
        strcpy(data[GUEST_LOBBY_2][1].field_6C_gtx_string_name, "");

        data[GUEST_LOBBY_2][2].field_0_windowParams.field_0_x = 10;
        data[GUEST_LOBBY_2][2].field_0_windowParams.field_4_y = 170;
        data[GUEST_LOBBY_2][2].field_0_windowParams.field_8_w = 150;
        data[GUEST_LOBBY_2][2].field_0_windowParams.field_C_h = 30;
        data[GUEST_LOBBY_2][2].field_0_windowParams.field_10_hMenu = reinterpret_cast<HMENU>(IDCANCEL);
        data[GUEST_LOBBY_2][2].field_0_windowParams.field_14_style = 0x40000000;
        data[GUEST_LOBBY_2][2].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][2].field_1C_window_class_name, "BUTTON");
        strcpy(data[GUEST_LOBBY_2][2].field_6C_gtx_string_name, "netui03");

        data[GUEST_LOBBY_2][3].field_0_windowParams.field_0_x = 230;
        data[GUEST_LOBBY_2][3].field_0_windowParams.field_4_y = 150;
        data[GUEST_LOBBY_2][3].field_0_windowParams.field_8_w = 270;
        data[GUEST_LOBBY_2][3].field_0_windowParams.field_C_h = 30;
        data[GUEST_LOBBY_2][3].field_0_windowParams.field_10_hMenu = reinterpret_cast<HMENU>(TEXTFIELD_CHAT_GUEST_1053);
        data[GUEST_LOBBY_2][3].field_0_windowParams.field_14_style = 0x40000080;
        data[GUEST_LOBBY_2][3].field_0_windowParams.field_18_exStyle = 0x200;
        strcpy(data[GUEST_LOBBY_2][3].field_1C_window_class_name, "EDIT");
        strcpy(data[GUEST_LOBBY_2][3].field_6C_gtx_string_name, "");

        data[GUEST_LOBBY_2][4].field_0_windowParams.field_0_x = 230;
        data[GUEST_LOBBY_2][4].field_0_windowParams.field_4_y = 190;
        data[GUEST_LOBBY_2][4].field_0_windowParams.field_8_w = 270;
        data[GUEST_LOBBY_2][4].field_0_windowParams.field_C_h = 30;
        data[GUEST_LOBBY_2][4].field_0_windowParams.field_10_hMenu = reinterpret_cast<HMENU>(BUTTON_SEND_CHAT_MESSAGE_GUEST_1052);
        data[GUEST_LOBBY_2][4].field_0_windowParams.field_14_style = 0x40000000;
        data[GUEST_LOBBY_2][4].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][4].field_1C_window_class_name, "BUTTON");
        strcpy(data[GUEST_LOBBY_2][4].field_6C_gtx_string_name, "netui07");

        data[GUEST_LOBBY_2][5].field_0_windowParams.field_0_x = 230;
        data[GUEST_LOBBY_2][5].field_0_windowParams.field_4_y = 230;
        data[GUEST_LOBBY_2][5].field_0_windowParams.field_8_w = 270;
        data[GUEST_LOBBY_2][5].field_0_windowParams.field_C_h = 30;
        data[GUEST_LOBBY_2][5].field_0_windowParams.field_10_hMenu = reinterpret_cast<HMENU>(LABEL_JOINED_GAME_MAP_1054);
        data[GUEST_LOBBY_2][5].field_0_windowParams.field_14_style = 0x40000000;
        data[GUEST_LOBBY_2][5].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][5].field_1C_window_class_name, "STATIC");
        strcpy(data[GUEST_LOBBY_2][5].field_6C_gtx_string_name, "");

        data[GUEST_LOBBY_2][6].field_0_windowParams.field_0_x = 230;
        data[GUEST_LOBBY_2][6].field_0_windowParams.field_4_y = 260;
        data[GUEST_LOBBY_2][6].field_0_windowParams.field_8_w = 270;
        data[GUEST_LOBBY_2][6].field_0_windowParams.field_C_h = 30;
        data[GUEST_LOBBY_2][6].field_0_windowParams.field_10_hMenu = reinterpret_cast<HMENU>(LABEL_JOINED_GAME_TYPE_1057);
        data[GUEST_LOBBY_2][6].field_0_windowParams.field_14_style = 0x40000000;
        data[GUEST_LOBBY_2][6].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][6].field_1C_window_class_name, "STATIC");
        strcpy(data[GUEST_LOBBY_2][6].field_6C_gtx_string_name, "");

        data[GUEST_LOBBY_2][7].field_0_windowParams.field_0_x = 230;
        data[GUEST_LOBBY_2][7].field_0_windowParams.field_4_y = 290;
        data[GUEST_LOBBY_2][7].field_0_windowParams.field_8_w = 270;
        data[GUEST_LOBBY_2][7].field_0_windowParams.field_C_h = 30;
        data[GUEST_LOBBY_2][7].field_0_windowParams.field_10_hMenu = reinterpret_cast<HMENU>(LABEL_JOINED_GAME_FRAG_LIMIT_1058);
        data[GUEST_LOBBY_2][7].field_0_windowParams.field_14_style = 0x40000000;
        data[GUEST_LOBBY_2][7].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][7].field_1C_window_class_name, "STATIC");
        strcpy(data[GUEST_LOBBY_2][7].field_6C_gtx_string_name, "");

        data[GUEST_LOBBY_2][8].field_0_windowParams.field_0_x = 230;
        data[GUEST_LOBBY_2][8].field_0_windowParams.field_4_y = 320;
        data[GUEST_LOBBY_2][8].field_0_windowParams.field_8_w = 270;
        data[GUEST_LOBBY_2][8].field_0_windowParams.field_C_h = 30;
        data[GUEST_LOBBY_2][8].field_0_windowParams.field_10_hMenu = reinterpret_cast<HMENU>(LABEL_JOINED_GAME_POLICE_ENABLED_1056);
        data[GUEST_LOBBY_2][8].field_0_windowParams.field_14_style = 0x40000000;
        data[GUEST_LOBBY_2][8].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][8].field_1C_window_class_name, "STATIC");
        strcpy(data[GUEST_LOBBY_2][8].field_6C_gtx_string_name, "");

        data[GUEST_LOBBY_2][9].field_0_windowParams.field_0_x = 230;
        data[GUEST_LOBBY_2][9].field_0_windowParams.field_4_y = 350;
        data[GUEST_LOBBY_2][9].field_0_windowParams.field_8_w = 270;
        data[GUEST_LOBBY_2][9].field_0_windowParams.field_C_h = 30;
        data[GUEST_LOBBY_2][9].field_0_windowParams.field_10_hMenu = reinterpret_cast<HMENU>(LABEL_GAME_SPEED_1055);
        data[GUEST_LOBBY_2][9].field_0_windowParams.field_14_style = 0x40000000;
        data[GUEST_LOBBY_2][9].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][9].field_1C_window_class_name, "STATIC");
        strcpy(data[GUEST_LOBBY_2][9].field_6C_gtx_string_name, "");

        data[GUEST_LOBBY_2][10].field_0_windowParams.field_0_x = 230;
        data[GUEST_LOBBY_2][10].field_0_windowParams.field_4_y = 380;
        data[GUEST_LOBBY_2][10].field_0_windowParams.field_8_w = 270;
        data[GUEST_LOBBY_2][10].field_0_windowParams.field_C_h = 30;
        data[GUEST_LOBBY_2][10].field_0_windowParams.field_10_hMenu = reinterpret_cast<HMENU>(LABEL_JOINED_GAME_TIME_LIMIT_1060);
        data[GUEST_LOBBY_2][10].field_0_windowParams.field_14_style = 0x40000000;
        data[GUEST_LOBBY_2][10].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][10].field_1C_window_class_name, "STATIC");
        strcpy(data[GUEST_LOBBY_2][10].field_6C_gtx_string_name, "");

        // Clear remaining array for windows 2:

        data[GUEST_LOBBY_2][11].field_0_windowParams.field_0_x = 0;
        data[GUEST_LOBBY_2][11].field_0_windowParams.field_4_y = 0;
        data[GUEST_LOBBY_2][11].field_0_windowParams.field_8_w = 0;
        data[GUEST_LOBBY_2][11].field_0_windowParams.field_C_h = 0;
        data[GUEST_LOBBY_2][11].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[GUEST_LOBBY_2][11].field_0_windowParams.field_14_style = 0x0;
        data[GUEST_LOBBY_2][11].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][11].field_1C_window_class_name, "0");
        strcpy(data[GUEST_LOBBY_2][11].field_6C_gtx_string_name, "0");

        data[GUEST_LOBBY_2][12].field_0_windowParams.field_0_x = 0;
        data[GUEST_LOBBY_2][12].field_0_windowParams.field_4_y = 0;
        data[GUEST_LOBBY_2][12].field_0_windowParams.field_8_w = 0;
        data[GUEST_LOBBY_2][12].field_0_windowParams.field_C_h = 0;
        data[GUEST_LOBBY_2][12].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[GUEST_LOBBY_2][12].field_0_windowParams.field_14_style = 0x0;
        data[GUEST_LOBBY_2][12].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][12].field_1C_window_class_name, "0");
        strcpy(data[GUEST_LOBBY_2][12].field_6C_gtx_string_name, "0");

        data[GUEST_LOBBY_2][13].field_0_windowParams.field_0_x = 0;
        data[GUEST_LOBBY_2][13].field_0_windowParams.field_4_y = 0;
        data[GUEST_LOBBY_2][13].field_0_windowParams.field_8_w = 0;
        data[GUEST_LOBBY_2][13].field_0_windowParams.field_C_h = 0;
        data[GUEST_LOBBY_2][13].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[GUEST_LOBBY_2][13].field_0_windowParams.field_14_style = 0x0;
        data[GUEST_LOBBY_2][13].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][13].field_1C_window_class_name, "0");
        strcpy(data[GUEST_LOBBY_2][13].field_6C_gtx_string_name, "0");

        data[GUEST_LOBBY_2][14].field_0_windowParams.field_0_x = 0;
        data[GUEST_LOBBY_2][14].field_0_windowParams.field_4_y = 0;
        data[GUEST_LOBBY_2][14].field_0_windowParams.field_8_w = 0;
        data[GUEST_LOBBY_2][14].field_0_windowParams.field_C_h = 0;
        data[GUEST_LOBBY_2][14].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[GUEST_LOBBY_2][14].field_0_windowParams.field_14_style = 0x0;
        data[GUEST_LOBBY_2][14].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][14].field_1C_window_class_name, "0");
        strcpy(data[GUEST_LOBBY_2][14].field_6C_gtx_string_name, "0");

        data[GUEST_LOBBY_2][15].field_0_windowParams.field_0_x = 0;
        data[GUEST_LOBBY_2][15].field_0_windowParams.field_4_y = 0;
        data[GUEST_LOBBY_2][15].field_0_windowParams.field_8_w = 0;
        data[GUEST_LOBBY_2][15].field_0_windowParams.field_C_h = 0;
        data[GUEST_LOBBY_2][15].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[GUEST_LOBBY_2][15].field_0_windowParams.field_14_style = 0x0;
        data[GUEST_LOBBY_2][15].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][15].field_1C_window_class_name, "0");
        strcpy(data[GUEST_LOBBY_2][15].field_6C_gtx_string_name, "0");

        data[GUEST_LOBBY_2][16].field_0_windowParams.field_0_x = 0;
        data[GUEST_LOBBY_2][16].field_0_windowParams.field_4_y = 0;
        data[GUEST_LOBBY_2][16].field_0_windowParams.field_8_w = 0;
        data[GUEST_LOBBY_2][16].field_0_windowParams.field_C_h = 0;
        data[GUEST_LOBBY_2][16].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[GUEST_LOBBY_2][16].field_0_windowParams.field_14_style = 0x0;
        data[GUEST_LOBBY_2][16].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][16].field_1C_window_class_name, "0");
        strcpy(data[GUEST_LOBBY_2][16].field_6C_gtx_string_name, "0");

        data[GUEST_LOBBY_2][17].field_0_windowParams.field_0_x = 0;
        data[GUEST_LOBBY_2][17].field_0_windowParams.field_4_y = 0;
        data[GUEST_LOBBY_2][17].field_0_windowParams.field_8_w = 0;
        data[GUEST_LOBBY_2][17].field_0_windowParams.field_C_h = 0;
        data[GUEST_LOBBY_2][17].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[GUEST_LOBBY_2][17].field_0_windowParams.field_14_style = 0x0;
        data[GUEST_LOBBY_2][17].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][17].field_1C_window_class_name, "0");
        strcpy(data[GUEST_LOBBY_2][17].field_6C_gtx_string_name, "0");

        data[GUEST_LOBBY_2][18].field_0_windowParams.field_0_x = 0;
        data[GUEST_LOBBY_2][18].field_0_windowParams.field_4_y = 0;
        data[GUEST_LOBBY_2][18].field_0_windowParams.field_8_w = 0;
        data[GUEST_LOBBY_2][18].field_0_windowParams.field_C_h = 0;
        data[GUEST_LOBBY_2][18].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[GUEST_LOBBY_2][18].field_0_windowParams.field_14_style = 0x0;
        data[GUEST_LOBBY_2][18].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][18].field_1C_window_class_name, "0");
        strcpy(data[GUEST_LOBBY_2][18].field_6C_gtx_string_name, "0");

        data[GUEST_LOBBY_2][19].field_0_windowParams.field_0_x = 0;
        data[GUEST_LOBBY_2][19].field_0_windowParams.field_4_y = 0;
        data[GUEST_LOBBY_2][19].field_0_windowParams.field_8_w = 0;
        data[GUEST_LOBBY_2][19].field_0_windowParams.field_C_h = 0;
        data[GUEST_LOBBY_2][19].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[GUEST_LOBBY_2][19].field_0_windowParams.field_14_style = 0x0;
        data[GUEST_LOBBY_2][19].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][19].field_1C_window_class_name, "0");
        strcpy(data[GUEST_LOBBY_2][19].field_6C_gtx_string_name, "0");

        data[GUEST_LOBBY_2][20].field_0_windowParams.field_0_x = 0;
        data[GUEST_LOBBY_2][20].field_0_windowParams.field_4_y = 0;
        data[GUEST_LOBBY_2][20].field_0_windowParams.field_8_w = 0;
        data[GUEST_LOBBY_2][20].field_0_windowParams.field_C_h = 0;
        data[GUEST_LOBBY_2][20].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[GUEST_LOBBY_2][20].field_0_windowParams.field_14_style = 0x0;
        data[GUEST_LOBBY_2][20].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][20].field_1C_window_class_name, "0");
        strcpy(data[GUEST_LOBBY_2][20].field_6C_gtx_string_name, "0");

        data[GUEST_LOBBY_2][21].field_0_windowParams.field_0_x = 0;
        data[GUEST_LOBBY_2][21].field_0_windowParams.field_4_y = 0;
        data[GUEST_LOBBY_2][21].field_0_windowParams.field_8_w = 0;
        data[GUEST_LOBBY_2][21].field_0_windowParams.field_C_h = 0;
        data[GUEST_LOBBY_2][21].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[GUEST_LOBBY_2][21].field_0_windowParams.field_14_style = 0x0;
        data[GUEST_LOBBY_2][21].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][21].field_1C_window_class_name, "0");
        strcpy(data[GUEST_LOBBY_2][21].field_6C_gtx_string_name, "0");

        data[GUEST_LOBBY_2][22].field_0_windowParams.field_0_x = 0;
        data[GUEST_LOBBY_2][22].field_0_windowParams.field_4_y = 0;
        data[GUEST_LOBBY_2][22].field_0_windowParams.field_8_w = 0;
        data[GUEST_LOBBY_2][22].field_0_windowParams.field_C_h = 0;
        data[GUEST_LOBBY_2][22].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[GUEST_LOBBY_2][22].field_0_windowParams.field_14_style = 0x0;
        data[GUEST_LOBBY_2][22].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][22].field_1C_window_class_name, "0");
        strcpy(data[GUEST_LOBBY_2][22].field_6C_gtx_string_name, "0");

        data[GUEST_LOBBY_2][23].field_0_windowParams.field_0_x = 0;
        data[GUEST_LOBBY_2][23].field_0_windowParams.field_4_y = 0;
        data[GUEST_LOBBY_2][23].field_0_windowParams.field_8_w = 0;
        data[GUEST_LOBBY_2][23].field_0_windowParams.field_C_h = 0;
        data[GUEST_LOBBY_2][23].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[GUEST_LOBBY_2][23].field_0_windowParams.field_14_style = 0x0;
        data[GUEST_LOBBY_2][23].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][23].field_1C_window_class_name, "0");
        strcpy(data[GUEST_LOBBY_2][23].field_6C_gtx_string_name, "0");

        data[GUEST_LOBBY_2][24].field_0_windowParams.field_0_x = 0;
        data[GUEST_LOBBY_2][24].field_0_windowParams.field_4_y = 0;
        data[GUEST_LOBBY_2][24].field_0_windowParams.field_8_w = 0;
        data[GUEST_LOBBY_2][24].field_0_windowParams.field_C_h = 0;
        data[GUEST_LOBBY_2][24].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[GUEST_LOBBY_2][24].field_0_windowParams.field_14_style = 0x0;
        data[GUEST_LOBBY_2][24].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][24].field_1C_window_class_name, "0");
        strcpy(data[GUEST_LOBBY_2][24].field_6C_gtx_string_name, "0");

        data[GUEST_LOBBY_2][25].field_0_windowParams.field_0_x = 0;
        data[GUEST_LOBBY_2][25].field_0_windowParams.field_4_y = 0;
        data[GUEST_LOBBY_2][25].field_0_windowParams.field_8_w = 0;
        data[GUEST_LOBBY_2][25].field_0_windowParams.field_C_h = 0;
        data[GUEST_LOBBY_2][25].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[GUEST_LOBBY_2][25].field_0_windowParams.field_14_style = 0x0;
        data[GUEST_LOBBY_2][25].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][25].field_1C_window_class_name, "0");
        strcpy(data[GUEST_LOBBY_2][25].field_6C_gtx_string_name, "0");

        data[GUEST_LOBBY_2][26].field_0_windowParams.field_0_x = 0;
        data[GUEST_LOBBY_2][26].field_0_windowParams.field_4_y = 0;
        data[GUEST_LOBBY_2][26].field_0_windowParams.field_8_w = 0;
        data[GUEST_LOBBY_2][26].field_0_windowParams.field_C_h = 0;
        data[GUEST_LOBBY_2][26].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[GUEST_LOBBY_2][26].field_0_windowParams.field_14_style = 0x0;
        data[GUEST_LOBBY_2][26].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][26].field_1C_window_class_name, "0");
        strcpy(data[GUEST_LOBBY_2][26].field_6C_gtx_string_name, "0");

        data[GUEST_LOBBY_2][27].field_0_windowParams.field_0_x = 0;
        data[GUEST_LOBBY_2][27].field_0_windowParams.field_4_y = 0;
        data[GUEST_LOBBY_2][27].field_0_windowParams.field_8_w = 0;
        data[GUEST_LOBBY_2][27].field_0_windowParams.field_C_h = 0;
        data[GUEST_LOBBY_2][27].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[GUEST_LOBBY_2][27].field_0_windowParams.field_14_style = 0x0;
        data[GUEST_LOBBY_2][27].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][27].field_1C_window_class_name, "0");
        strcpy(data[GUEST_LOBBY_2][27].field_6C_gtx_string_name, "0");

        data[GUEST_LOBBY_2][28].field_0_windowParams.field_0_x = 0;
        data[GUEST_LOBBY_2][28].field_0_windowParams.field_4_y = 0;
        data[GUEST_LOBBY_2][28].field_0_windowParams.field_8_w = 0;
        data[GUEST_LOBBY_2][28].field_0_windowParams.field_C_h = 0;
        data[GUEST_LOBBY_2][28].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[GUEST_LOBBY_2][28].field_0_windowParams.field_14_style = 0x0;
        data[GUEST_LOBBY_2][28].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][28].field_1C_window_class_name, "0");
        strcpy(data[GUEST_LOBBY_2][28].field_6C_gtx_string_name, "0");

        data[GUEST_LOBBY_2][29].field_0_windowParams.field_0_x = 0;
        data[GUEST_LOBBY_2][29].field_0_windowParams.field_4_y = 0;
        data[GUEST_LOBBY_2][29].field_0_windowParams.field_8_w = 0;
        data[GUEST_LOBBY_2][29].field_0_windowParams.field_C_h = 0;
        data[GUEST_LOBBY_2][29].field_0_windowParams.field_10_hMenu = static_cast<HMENU>(NULL);
        data[GUEST_LOBBY_2][29].field_0_windowParams.field_14_style = 0x0;
        data[GUEST_LOBBY_2][29].field_0_windowParams.field_18_exStyle = 0x0;
        strcpy(data[GUEST_LOBBY_2][29].field_1C_window_class_name, "0");
        strcpy(data[GUEST_LOBBY_2][29].field_6C_gtx_string_name, "0");
    }
    Network_UI_Control_Data data[3][30];
};

Network_UI_Control_Data_Array gUiControlDefinitions_621430;

MATCH_FUNC(0x519960)
void __stdcall WideToDbcs_519960(char_type* pDst, wchar_t* pSrc)
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
void NetUi::DbcsToWide_5199B0(wchar_t* pDst, char_type* pSrc)
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
    wchar_t* pText = gtext_0x14_6F87F0->Find_5B5F90(pKey);

    if (gtext_0x14_6F87F0->field_10_lang_code == 'j')
    {
        WideToDbcs_519960(Dest_6F88A4, pText);
    }
    else
    {
        wcstombs(Dest_6F88A4, pText, 256u);
    }
    return Dest_6F88A4;
}

MATCH_FUNC(0x519a50)
void NetUi::GetString_519A50(wchar_t* Dest, char_type* Source, size_t MaxCount)
{
    if (gtext_0x14_6F87F0->field_10_lang_code == 'j')
    {
        DbcsToWide_5199B0(Dest, Source);
    }
    else
    {
        mbstowcs(Dest, Source, MaxCount);
    }
}

MATCH_FUNC(0x519a90)
NetUi::NetUi()
{
    this->field_20320_hInstance = NULL;
    this->field_2031C_dlg_hwnd = NULL;
    this->field_20318_unused = 0;
    this->field_20310_showing_specific_window_idx = 0;
    this->field_20314_last_showing_specific_window_idx = 3;
    this->field_20084_unused = 0;
    this->field_1FD6C_count = 0;
    this->field_1FD64_total_map_count = 0;
    this->field_1FD68_bEndDialog = 0;
    this->field_20088_game_settings.field_2019C_tick_count = GetTickCount();
    this->field_20088_game_settings.field_20198_game_type = 1;
    this->field_20088_game_settings.field_20194_frag_limit = 1;
    this->field_20088_game_settings.field_2018C_map_idx = 0;
    this->field_20088_game_settings.field_201A0_police_on = 0;
    this->field_20088_game_settings.field_201A4_game_time_limit = 0;
    memset(field_4_maps, 0, sizeof(field_4_maps));
    gtext_0x14_6F87F0 = new TextTable();
    gtext_0x14_6F87F0->Load_5B5E90();
    LoadPlayerNameFromRegistry_51BC90();
    EnumerateMaps_51BFA0();
}

MATCH_FUNC(0x519ba0)
NetUi::~NetUi()
{
    if (gtext_0x14_6F87F0)
    {
        delete gtext_0x14_6F87F0;
        gtext_0x14_6F87F0 = NULL;
    }
}

MATCH_FUNC(0x519bd0)
s32 NetUi::ShowNetworkUiBlocking_519BD0(HINSTANCE hInstance)
{
    field_20320_hInstance = hInstance;
    gNetPlay_7071E8.Set15_51ECD0(reinterpret_cast<int>(NetUi::cb_FillSessionList_519D30), this);
    gNetPlay_7071E8.Set6_520530(static_cast<void*>(NetUi::cb_AddPlayerToList_519E30), static_cast<void*>(this));
    gNetPlay_7071E8.Set9_520E60(reinterpret_cast<int>(NetUi::cb_RemovePlayerFromList_51ACD0), reinterpret_cast<int>(this));
    gNetPlay_7071E8.Set3_Disconnect_520E80(reinterpret_cast<int>(NetUi::cb_Disconnect_51ADE0), reinterpret_cast<int>(this));
    gNetPlay_7071E8.Set18_520F50(reinterpret_cast<int>(NetUi::cb_SetPlayerPing_51AE50), reinterpret_cast<int>(this));
    gNetPlay_7071E8.Set21_5210D0(reinterpret_cast<int>(NetUi::cb_AddChatMessage_51B2F0), reinterpret_cast<int>(this));
    gNetPlay_7071E8.Set24_521140(reinterpret_cast<int>(NetUi::cb_GameSettingsChanged_51B7E0), reinterpret_cast<int>(this));
    gNetPlay_7071E8.Set27SavePlayerName_5211F0(reinterpret_cast<int>(NetUi::cb_SavePlayerName_51BC00), reinterpret_cast<int>(this));
    return DialogBoxParamA(field_20320_hInstance,
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
            NetUi::OnTimer_51A9D0(hDlg, wParam);
            break;

        case WM_HSCROLL:
        {
            HWND hwndCtl = reinterpret_cast<HWND>(lParam);
            int scrollCode = LOWORD(wParam);
            int thumbPos = static_cast<short>(HIWORD(wParam));

            NetUi::OnWmHScroll_51C630(hDlg, hwndCtl, scrollCode, thumbPos);
            return 0;
        }
        break;

        case WM_COMMAND:
        {
            WORD notify = HIWORD(wParam);
            WORD id = LOWORD(wParam);

            NetUi::OnWmCommand_519FE0(hDlg, id, reinterpret_cast<HWND>(lParam), notify);
        }
        break;

        case WM_PAINT:
            NetUi::OnPaint_519FD0(hDlg);
            break;
        case WM_INITDIALOG:
            return OnInitDialog_51AC60(hDlg, wParam, reinterpret_cast<NetUi*>(lParam));

        default:
            break;
    }

    return 0;
}

MATCH_FUNC(0x519d30)
s32 NetUi::cb_FillSessionList_519D30(NetUi* pThis, Network_NameList* pSessions)
{
    char_type name[260];

    SendDlgItemMessageA(pThis->Get_2031C_HWND_519E20(), LISTBOX_ALL_NET_GAMES_1000, LB_RESETCONTENT, 0, 0);
    for (u32 i = 0; i < pSessions->field_40_count; i++)
    {
        wcstombs(name, pSessions->field_0_names[i], sizeof(name));
        LRESULT item = SendDlgItemMessageA(pThis->Get_2031C_HWND_519E20(), LISTBOX_ALL_NET_GAMES_1000, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(name));
        if (item != LB_ERR && item != LB_ERRSPACE)
        {
            SendDlgItemMessageA(pThis->Get_2031C_HWND_519E20(), LISTBOX_ALL_NET_GAMES_1000, LB_SETITEMDATA, item, i);
        }
    }

    if (pSessions->field_40_count > 0 || gNetPlay_7071E8.field_4_bModem)
    {
        return EnableWindow(GetDlgItem(pThis->Get_2031C_HWND_519E20(), BUTTON_JOIN_GAME_1001), TRUE);
    }
    return EnableWindow(GetDlgItem(pThis->Get_2031C_HWND_519E20(), BUTTON_JOIN_GAME_1001), FALSE);
}

MATCH_FUNC(0x519e10)
void NetUi::SetDlgHwnd_519E10(HWND hDlg)
{
    field_2031C_dlg_hwnd = hDlg;
}

MATCH_FUNC(0x519e20)
HWND NetUi::Get_2031C_HWND_519E20()
{
    return field_2031C_dlg_hwnd;
}

MATCH_FUNC(0x519e30)
void NetUi::cb_AddPlayerToList_519E30(NetUi* pNetUi, wchar_t* pName, s32 player_idx)
{
    LVITEMA item;
    char_type name[260];

    if (gtext_0x14_6F87F0->field_10_lang_code == 'j')
    {
        WideToDbcs_519960(name, pName);
    }
    else
    {
        wcstombs(name, pName, 260u);
    }

    switch (pNetUi->Get_20310_showing_window_idx_51ACC0())
    {
        case 2:
        {
            memset(&item, 0, sizeof(item));
            HWND hList = GetDlgItem(pNetUi->Get_2031C_HWND_519E20(), LISTVIEW_JOINED_PLAYERS_1050);
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
            HWND hList = GetDlgItem(pNetUi->Get_2031C_HWND_519E20(), LISTVIEW_HOST_PLAYERS_1024);
            s32 count = SendMessageA(hList, LVM_GETITEMCOUNT, 0, 0);
            item.mask = LVIF_TEXT;
            item.iItem = count;
            item.iSubItem = 0;
            item.pszText = name;
            SendMessageA(hList, LVM_INSERTITEMA, 0, reinterpret_cast<LPARAM>(&item));
            pNetUi->IncCount_51BBC0();
            pNetUi->UpdateButtonsEnabledState_51CBC0();
            break;
        }

        case 0:
        {
            memset(&item, 0, sizeof(item));
            HWND hList = GetDlgItem(pNetUi->Get_2031C_HWND_519E20(), LISTVIEW_JOINED_PLAYERS_1050);
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
void __stdcall NetUi::OnPaint_519FD0(HWND hWnd)
{
    // Empty
}

MATCH_FUNC(0x519fe0)
void __stdcall NetUi::OnWmCommand_519FE0(HWND hDlg, s32 id, HWND hCtl, s32 notify)
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
            if (gNetPlay_7071E8.field_4_bModem)
            {
                gNetPlay_7071E8.EnumSessions_51E650();
                wchar_t playerNameW[260];
                playerData.field_0_data = NULL;
                playerData.field_4_len = 0;
                groupData.field_0_data = NULL;
                groupData.field_4_len = 0;
                GetDlgItemTextA(hDlg, TEXTBOX_PLAYER_NAME_1004, playerName, 259);
                NetUi::GetString_519A50(playerNameW, playerName, 260);
                NetUi* pThis = reinterpret_cast<NetUi*>(GetWindowLongA(hDlg, 8));
                pThis->SetPlayNameAndSaveToRegistry_51BD40(playerNameW, playerName);
                if (gNetPlay_7071E8.field_C4_sessions.field_5C4_session_count > 0)
                {
                    if (gNetPlay_7071E8.JoinSession_520570(0, playerNameW, &playerData, &groupData))
                    {
                        pThis->ApplyHostGameSettings_51B810(static_cast<const char_type*>(groupData.field_0_data));
                        pThis->ShowSpecificWindow_51ABF0(2);
                    }
                }
            }
            else
            {
                wchar_t playerNameW[260];
                playerData.field_0_data = NULL;
                playerData.field_4_len = 0;
                groupData.field_0_data = NULL;
                groupData.field_4_len = 0;
                GetDlgItemTextA(hDlg, TEXTBOX_PLAYER_NAME_1004, playerName, 259);
                NetUi::GetString_519A50(playerNameW, playerName, 260);
                NetUi* pThis = reinterpret_cast<NetUi*>(GetWindowLongA(hDlg, 8));
                pThis->SetPlayNameAndSaveToRegistry_51BD40(playerNameW, playerName);
                LRESULT sel = SendDlgItemMessageA(hDlg, LISTBOX_ALL_NET_GAMES_1000, LB_GETCURSEL, 0, 0);
                if (sel != -1)
                {
                    LRESULT idx = SendDlgItemMessageA(hDlg, LISTBOX_ALL_NET_GAMES_1000, LB_GETITEMDATA, sel, 0);
                    if (gNetPlay_7071E8.JoinSession_520570(idx, playerNameW, &playerData, &groupData))
                    {
                        pThis->ApplyHostGameSettings_51B810(static_cast<const char_type*>(groupData.field_0_data));
                        pThis->ShowSpecificWindow_51ABF0(2);
                    }
                }
            }
            break;

        case LISTBOX_ALL_NET_GAMES_1000:
            if (notify == LBN_DBLCLK)   // Double click on the network game session
            {
                wchar_t playerNameW[260];
                playerData.field_0_data = NULL;
                playerData.field_4_len = 0;
                groupData.field_0_data = NULL;
                groupData.field_4_len = 0;
                GetDlgItemTextA(hDlg, TEXTBOX_PLAYER_NAME_1004, playerName, 259);
                NetUi::GetString_519A50(playerNameW, playerName, 260);
                NetUi* pThis = reinterpret_cast<NetUi*>(GetWindowLongA(hDlg, 8));
                pThis->SetPlayNameAndSaveToRegistry_51BD40(playerNameW, playerName);
                LRESULT sel = SendDlgItemMessageA(hDlg, LISTBOX_ALL_NET_GAMES_1000, LB_GETCURSEL, 0, 0);
                if (sel != -1)
                {
                    LRESULT idx = SendDlgItemMessageA(hDlg, LISTBOX_ALL_NET_GAMES_1000, LB_GETITEMDATA, sel, 0);
                    if (gNetPlay_7071E8.JoinSession_520570(idx, playerNameW, &playerData, &groupData))
                    {
                        pThis->ShowSpecificWindow_51ABF0(2);
                    }
                }
            }
            break;

        case IDCANCEL:
        {
            NetUi* pThis = reinterpret_cast<NetUi*>(GetWindowLongA(hDlg, 8));
            switch (pThis->Get_20310_showing_window_idx_51ACC0())
            {
                case 0:
                    KillTimer(hDlg, gTimerId_6F8A18);
                    EndDialog(hDlg, 0);
                    break;
                case 1:
                    gNetPlay_7071E8.Disconnect_520D10();
                    SendMessageA(GetDlgItem(hDlg, LISTVIEW_HOST_PLAYERS_1024), LVM_DELETEALLITEMS, 0, 0);
                    SetDlgItemTextA(hDlg, TEXTFIELD_CHAT_HOST_1025, "");
                    SetDlgItemTextA(hDlg, CHATBOX_HOST_1022, "");
                    pThis->ResetCount_51BBF0();
                    pThis->ShowSpecificWindow_51ABF0(0);
                    break;
                case 2:
                    gNetPlay_7071E8.Disconnect_520D10();
                    SendMessageA(GetDlgItem(hDlg, LISTVIEW_JOINED_PLAYERS_1050), LVM_DELETEALLITEMS, 0, 0);
                    SetDlgItemTextA(hDlg, TEXTFIELD_CHAT_GUEST_1053, "");
                    SetDlgItemTextA(hDlg, CHATBOX_GUEST_1051, "");
                    pThis->ResetCount_51BBF0();
                    pThis->ShowSpecificWindow_51ABF0(0);
                    break;
            }
            break;
        }

        case BUTTON_CREATE_GAME_1002: // "Create" button
        {
            NetUi* pThis = reinterpret_cast<NetUi*>(GetWindowLongA(hDlg, 8));
            NetworkGameSettings settings = pThis->field_20088_game_settings;
            groupData.field_0_data = &settings;
            groupData.field_4_len = sizeof(NetworkGameSettings);
            playerData.field_0_data = NULL;
            playerData.field_4_len = 0;
            GetDlgItemTextA(hDlg, TEXTBOX_PLAYER_NAME_1004, playerName, 259);
            sprintf(sessionName, "%s's Game", playerName);
            NetUi::GetString_519A50(hostPlayerNameW, playerName, 260);
            NetUi::GetString_519A50(textW, sessionName, 260);
            if (gNetPlay_7071E8.HostSession_51E7A0(textW, hostPlayerNameW, &groupData, &playerData))
            {
                pThis->SetPlayNameAndSaveToRegistry_51BD40(hostPlayerNameW, playerName);
                pThis->ShowSpecificWindow_51ABF0(1);
                pThis->SetFragsNumberAndLabel_51CDC0(pThis->field_20088_game_settings.field_20198_game_type,
                                                     pThis->field_20088_game_settings.field_20194_frag_limit,
                                                     hDlg);
            }
            break;
        }

        case BUTTON_SEND_CHAT_MESSAGE_HOST_1023: // "Send" host chat button
        {
            GetWindowLongA(hDlg, 8);
            GetDlgItemTextA(hDlg, TEXTFIELD_CHAT_HOST_1025, chatText, 127);
            NetUi::GetString_519A50(textW, chatText, 260);
            gNetPlay_7071E8.SendChatMessage_521060(textW, -1);
            SetDlgItemTextA(hDlg, TEXTFIELD_CHAT_HOST_1025, "");
            NetUi* pThis = reinterpret_cast<NetUi*>(GetWindowLongA(hDlg, 8));
            pThis->AppendChatMessage_51B4F0(CHATBOX_HOST_1022, chatText);
            break;
        }

        case BUTTON_START_GAME_1021: // "Start" button
        {
            gNetPlay_7071E8.NetworkTick_51ED00();
            Sleep(100);
            GetDlgItemTextA(hDlg, TEXTBOX_PLAYER_NAME_1004, playerName, 260);
            NetUi::GetString_519A50(textW, playerName, 260);
            NetUi* pThis = reinterpret_cast<NetUi*>(GetWindowLongA(hDlg, 8));
            pThis->SetPlayNameAndSaveToRegistry_51BD40(textW, playerName);
            gNetPlay_7071E8.DisableJoining_521220();
            EndDialog(hDlg, 1); // close lobby returning 1
            break;
        }

        case BUTTON_REJECT_PLAYER_1020: // "Reject" button
        {
            NetUi* pThis = reinterpret_cast<NetUi*>(GetWindowLongA(hDlg, 8));
            HWND hList = GetDlgItem(pThis->Get_2031C_HWND_519E20(), LISTVIEW_HOST_PLAYERS_1024);
            LRESULT item = SendMessageA(hList, LVM_GETNEXTITEM, -1, LVNI_SELECTED);
            LVITEMA lvItem;
            lvItem.iSubItem = 0;
            lvItem.cchTextMax = 260;
            lvItem.pszText = sessionName;
            SendMessageA(hList, LVM_GETITEMTEXTA, item, reinterpret_cast<LPARAM>(&lvItem));
            NetUi::GetString_519A50(textW, sessionName, 260);
            if (gNetPlay_7071E8.RemovePlayerByName_520F80(textW))
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
                    pThis->SetSetting_51B9C0(1, SendDlgItemMessageA(hDlg, COMBO_MAP_LIST_1026, CB_GETITEMDATA, sel, 0));
                    pThis->UpdateButtonsEnabledState_51CBC0();
                    break;
                }
            }
            break;

        case BUTTON_SEND_CHAT_MESSAGE_GUEST_1052: // "Send" guest chat button
        {
            NetUi* pThis = reinterpret_cast<NetUi*>(GetWindowLongA(hDlg, 8));
            GetDlgItemTextA(hDlg, TEXTFIELD_CHAT_GUEST_1053, chatText, 127);
            NetUi::GetString_519A50(textW, chatText, 260);
            gNetPlay_7071E8.SendChatMessage_521060(textW, -1);
            SetDlgItemTextA(hDlg, TEXTFIELD_CHAT_GUEST_1053, "");
            pThis->AppendChatMessage_51B4F0(CHATBOX_GUEST_1051, chatText);
            break;
        }

        case COMBO_GAME_TYPE_1036: // Game type
            switch (notify)
            {
                case CBN_SELCHANGE:
                {
                    NetUi* pThis = reinterpret_cast<NetUi*>(GetWindowLongA(hDlg, 8));
                    s32 gameType = SendDlgItemMessageA(hDlg, COMBO_GAME_TYPE_1036, CB_GETCURSEL, 0, 0) + 1;
                    pThis->SetSetting_51B9C0(3, gameType);
                    pThis->SetFragsNumberAndLabel_51CDC0(gameType, pThis->field_20088_game_settings.field_20194_frag_limit, hDlg);
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
                    pThis->SetSetting_51B9C0(2, value);
                }
            }
            break;

        case CHECKBOX_POLICE_ENABLED_1027:
        {
            LRESULT state = SendDlgItemMessageA(hDlg, CHECKBOX_POLICE_ENABLED_1027, BM_GETSTATE, 0, 0);
            NetUi* pThis = reinterpret_cast<NetUi*>(GetWindowLongA(hDlg, 8));
            if ((state & 3) == BST_CHECKED)
            {
                pThis->SetSetting_51B9C0(6, 1);
            }
            else
            {
                pThis->SetSetting_51B9C0(6, 0);
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
                    pThis->SetSetting_51B9C0(7, value);
                }
            }
            break;
    }
}

MATCH_FUNC(0x51a9d0)
void __stdcall NetUi::OnTimer_51A9D0(HWND hWnd, s32 timer_id)
{
    NetUi* pThis = reinterpret_cast<NetUi*>(GetWindowLongA(hWnd, 8));
    int hadSelection = 0;
    if (!pThis->GetEndDialog_51BC80())
    {
        char textBuf[260];

        const LRESULT selIndex = SendDlgItemMessageA(hWnd, LISTBOX_UNKNOWN_1000, LB_GETCURSEL, 0, 0);

        if (selIndex != LB_ERR)
        {
            SendDlgItemMessageA(hWnd, LISTBOX_ALL_NET_GAMES_1000, LB_GETTEXT, selIndex, reinterpret_cast<LPARAM>(textBuf));

            hadSelection = 1;
        }

        gNetPlay_7071E8.NetworkTick_51ED00();

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
void NetUi::CreateMainUi_51AA90(HWND hWndParent)
{
    for (u32 i = 0; i < 3; i++)
    {
        for (u32 j = 0; j < 30; j++)
        {
            char* pNewWindowStr;
            if (gUiControlDefinitions_621430.data[i][j].field_6C_gtx_string_name[0])
            {
                pNewWindowStr = GetString_519A00(gUiControlDefinitions_621430.data[i][j].field_6C_gtx_string_name);
            }
            else
            {
                pNewWindowStr = ""; // byte_67DC88
            }

            field_201A8_ui_control_hwnds[i][j] = CreateWindowExA(gUiControlDefinitions_621430.data[i][j].field_0_windowParams.field_18_exStyle,
                                                                 gUiControlDefinitions_621430.data[i][j].field_1C_window_class_name,
                                                                 pNewWindowStr,
                                                                 gUiControlDefinitions_621430.data[i][j].field_0_windowParams.field_14_style,
                                                                 gUiControlDefinitions_621430.data[i][j].field_0_windowParams.field_0_x,
                                                                 gUiControlDefinitions_621430.data[i][j].field_0_windowParams.field_4_y,
                                                                 gUiControlDefinitions_621430.data[i][j].field_0_windowParams.field_8_w,
                                                                 gUiControlDefinitions_621430.data[i][j].field_0_windowParams.field_C_h,
                                                                 hWndParent,
                                                                 gUiControlDefinitions_621430.data[i][j].field_0_windowParams.field_10_hMenu,
                                                                 this->field_20320_hInstance,
                                                                 NULL);
        }
    }

    SetWindowTextA(hWndParent, GetString_519A00("netui08")); // "Network GTA2"

    // Subclass to handle enter key pressing
    field_1FD70_old_proc = reinterpret_cast<WNDPROC>(GetWindowLongA(GetDlgItem(hWndParent, TEXTFIELD_CHAT_GUEST_1053), GWL_WNDPROC));
    field_1FD74_old_proc = reinterpret_cast<WNDPROC>(GetWindowLongA(GetDlgItem(hWndParent, TEXTFIELD_CHAT_HOST_1025), GWL_WNDPROC));
    SetWindowLongA(GetDlgItem(hWndParent, TEXTFIELD_CHAT_GUEST_1053), GWL_WNDPROC, reinterpret_cast<LONG>(NetUi::subclass_proc_51BDD0));
    SetWindowLongA(GetDlgItem(hWndParent, TEXTFIELD_CHAT_HOST_1025), GWL_WNDPROC, reinterpret_cast<LONG>(NetUi::subclass_proc_51BDD0));

    // set range of game speed slider: 0 to 2 (that is, 3 options)
    SendDlgItemMessageA(hWndParent, TRACKBAR_GAME_SPEED_1031, TBM_SETRANGE, TRUE, MAKELONG(0, 2)); 

    SetDlgItemTextA(hWndParent, LABEL_COPS_1028, GetString_519A00("netui14")); // "Cops"
    SetDlgItemTextA(hWndParent, LABEL_TIME_LIMIT_MINUTES_1037, GetString_519A00("netui21")); // "Time Limit (Minutes)"
    NetUi::SetPlayerCountText_51CB30(0, hWndParent);
    if (gNetPlay_7071E8.field_4_bModem)
    {
        EnableWindow(GetDlgItem(hWndParent, BUTTON_JOIN_GAME_1001), 1);
    }
}

MATCH_FUNC(0x51abf0)
void NetUi::ShowSpecificWindow_51ABF0(s32 showing_specific_window_idx)
{
    u32 i;

    // Hide old
    for (i = 0; i < 30; ++i)
    {
        ShowWindow(this->field_201A8_ui_control_hwnds[this->field_20310_showing_specific_window_idx][i], SW_HIDE);
    }

    // Show new (using param, not the mem var)
    for (i = 0; i < 30; ++i)
    {
        ShowWindow(this->field_201A8_ui_control_hwnds[showing_specific_window_idx][i], SW_SHOW);
    }

    this->field_20314_last_showing_specific_window_idx = this->field_20310_showing_specific_window_idx;
    this->field_20310_showing_specific_window_idx = showing_specific_window_idx;
}

MATCH_FUNC(0x51ac60)
s32 NetUi::OnInitDialog_51AC60(HWND hWnd, s32 lParam, NetUi* thisPtr)
{
    SetWindowLongA(hWnd, 8, reinterpret_cast<LONG>(thisPtr));
    gTimerId_6F8A18 = SetTimer(hWnd, 0xAu, 0xAu, NULL);
    thisPtr->SetDlgHwnd_519E10(hWnd);
    thisPtr->CreateMainUi_51AA90(hWnd);
    thisPtr->PopulateMainUI_51AFA0();
    thisPtr->SetPlayerNameText_51B7C0();
    thisPtr->ShowSpecificWindow_51ABF0(0);
    return 1;
}

MATCH_FUNC(0x51acc0)
s32 NetUi::Get_20310_showing_window_idx_51ACC0()
{
    return field_20310_showing_specific_window_idx;
}

MATCH_FUNC(0x51acd0)
void NetUi::cb_RemovePlayerFromList_51ACD0(NetUi* pNetUi, wchar_t* Source)
{
    LVFINDINFOA findInfo;
    char Dest[260];

    wcstombs(Dest, Source, 260u);

    switch (pNetUi->Get_20310_showing_window_idx_51ACC0())
    {
        case 1:
        {
            memset(&findInfo, 0, sizeof(findInfo));

            HWND hItem = GetDlgItem(pNetUi->Get_2031C_HWND_519E20(), LISTVIEW_HOST_PLAYERS_1024);

            findInfo.flags = LVFI_STRING;
            findInfo.psz = Dest;

            LRESULT sendRet = SendMessageA(hItem, LVM_FINDITEMA, 0xFFFFFFFF, reinterpret_cast<LPARAM>(&findInfo));
            SendMessageA(hItem, LVM_DELETEITEM, sendRet, 0);

            pNetUi->DecCount_51BBE0();
            pNetUi->UpdateButtonsEnabledState_51CBC0();
        }
        break;

        case 2:
        {
            memset(&findInfo, 0, sizeof(findInfo));

            HWND hItem = GetDlgItem(pNetUi->Get_2031C_HWND_519E20(), LISTVIEW_JOINED_PLAYERS_1050);

            findInfo.flags = LVFI_STRING;
            findInfo.psz = Dest;

            LRESULT sendRet = SendMessageA(hItem, LVM_FINDITEMA, 0xFFFFFFFF, reinterpret_cast<LPARAM>(&findInfo));
            SendMessageA(hItem, LVM_DELETEITEM, sendRet, 0);
        }
        break;
    }
}

MATCH_FUNC(0x51ade0)
void NetUi::cb_Disconnect_51ADE0(NetUi* pNetUi)
{
    gNetPlay_7071E8.Disconnect_520D10();
    pNetUi->ClearListView_51AE20(LISTVIEW_JOINED_PLAYERS_1050);
    pNetUi->ClearListView_51AE20(LISTVIEW_HOST_PLAYERS_1024);
    pNetUi->ShowSpecificWindow_51ABF0(0);
}

MATCH_FUNC(0x51ae20)
void NetUi::ClearListView_51AE20(s32 nIDDlgItem)
{
    HWND hwndDlg = this->field_2031C_dlg_hwnd;
    if (hwndDlg)
    {
        SendDlgItemMessageA(hwndDlg, nIDDlgItem, LVM_DELETEALLITEMS, 0, 0);
    }
}

#pragma warning(push)
#pragma warning(disable : 4715) // not all control paths return a value
MATCH_FUNC(0x51ae50)
s32 NetUi::cb_SetPlayerPing_51AE50(NetUi* pNetUi, Network_PlayerPing* pPing)
{
    LVFINDINFOA findInfo;
    LVITEMA item;
    char_type pingText[260];
    char_type name[260];

    wcstombs(name, pPing->field_0_name, 260u);

    switch (pNetUi->Get_20310_showing_window_idx_51ACC0())
    {
        case 1:
        {
            memset(&findInfo, 0, sizeof(findInfo));
            sprintf(pingText, "%d", pPing->field_208_avg_ping);
            HWND hItem = GetDlgItem(pNetUi->Get_2031C_HWND_519E20(), LISTVIEW_HOST_PLAYERS_1024);
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
            sprintf(pingText, "%d", pPing->field_208_avg_ping);
            HWND hItem = GetDlgItem(pNetUi->Get_2031C_HWND_519E20(), LISTVIEW_JOINED_PLAYERS_1050);
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
void NetUi::PopulateMainUI_51AFA0()
{
    LPARAM columnInfo[8];
    memset(columnInfo, 0, sizeof(columnInfo));

    u16 colWidth = 3 * (gUiControlDefinitions_621430.data[HOST_LOBBY_1][0].field_0_windowParams.field_8_w >> 2);

    columnInfo[0] = 36;
    columnInfo[3] = reinterpret_cast<LPARAM>("Player Name");
    columnInfo[7] = 0;
    SendDlgItemMessageA(field_2031C_dlg_hwnd, LISTVIEW_HOST_PLAYERS_1024, 0x101B, 0, reinterpret_cast<LPARAM>(columnInfo)); // 0x101B = LVM_INSERTCOLUMNA
    SendDlgItemMessageA(field_2031C_dlg_hwnd, LISTVIEW_HOST_PLAYERS_1024, 0x101E, 0, colWidth); // 0x101E = LVM_SETCOLUMNWIDTH
    memset(columnInfo, 0, sizeof(columnInfo));
    columnInfo[0] = 37;
    columnInfo[1] = 0;
    columnInfo[3] = reinterpret_cast<LPARAM>("Ping");
    columnInfo[7] = 1;
    SendDlgItemMessageA(field_2031C_dlg_hwnd, LISTVIEW_HOST_PLAYERS_1024, 0x101B, 1u, reinterpret_cast<LPARAM>(columnInfo));
    SendDlgItemMessageA(field_2031C_dlg_hwnd, LISTVIEW_HOST_PLAYERS_1024, 0x101E, 1u, 65534);
    memset(columnInfo, 0, sizeof(columnInfo));
    columnInfo[0] = 36;
    columnInfo[3] = reinterpret_cast<LPARAM>("Player Name");
    u16 col_width = 3 * (gUiControlDefinitions_621430.data[GUEST_LOBBY_2][0].field_0_windowParams.field_8_w >> 2);
    columnInfo[7] = 0;
    SendDlgItemMessageA(field_2031C_dlg_hwnd, LISTVIEW_JOINED_PLAYERS_1050, 0x101B, 0, reinterpret_cast<LPARAM>(columnInfo));
    SendDlgItemMessageA(field_2031C_dlg_hwnd, LISTVIEW_JOINED_PLAYERS_1050, 0x101Eu, 0, col_width);
    memset(columnInfo, 0, sizeof(columnInfo));
    columnInfo[0] = 37;
    columnInfo[1] = 0;
    columnInfo[3] = reinterpret_cast<LPARAM>("Ping");
    columnInfo[7] = 1;
    SendDlgItemMessageA(field_2031C_dlg_hwnd, LISTVIEW_JOINED_PLAYERS_1050, 0x101B, 1u, reinterpret_cast<LPARAM>(columnInfo));
    SendDlgItemMessageA(field_2031C_dlg_hwnd, LISTVIEW_JOINED_PLAYERS_1050, 0x101E, 1u, 65534);

    for (u32 map_idx = 0; map_idx < static_cast<u32>(field_1FD64_total_map_count); map_idx++)
    {
        LRESULT item_idx = SendDlgItemMessageA(field_2031C_dlg_hwnd, COMBO_MAP_LIST_1026, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(field_4_maps[map_idx].field_30C_description));
        SendDlgItemMessageA(field_2031C_dlg_hwnd, COMBO_MAP_LIST_1026, CB_SETITEMDATA, item_idx, map_idx);
    }

    LRESULT item_a = SendDlgItemMessageA(field_2031C_dlg_hwnd, COMBO_GAME_TYPE_1036, 323u, 0, reinterpret_cast<LPARAM>(GetString_519A00("netui19")));
    SendDlgItemMessageA(field_2031C_dlg_hwnd, COMBO_GAME_TYPE_1036, CB_SETITEMDATA, item_a, 0);
    LRESULT item_b = SendDlgItemMessageA(field_2031C_dlg_hwnd, COMBO_GAME_TYPE_1036, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(GetString_519A00("netui20")));
    SendDlgItemMessageA(field_2031C_dlg_hwnd, COMBO_GAME_TYPE_1036, CB_SETITEMDATA, item_b, 1);
    LRESULT item_c = SendDlgItemMessageA(field_2031C_dlg_hwnd, COMBO_GAME_TYPE_1036, 0x143u, 0, reinterpret_cast<LPARAM>(GetString_519A00("netui22")));
    SendDlgItemMessageA(field_2031C_dlg_hwnd, COMBO_GAME_TYPE_1036, CB_SETITEMDATA, item_c, 2);
    NetUi::LoadGameSettingsFromRegistry_51C830();
    SendDlgItemMessageA(field_2031C_dlg_hwnd, COMBO_MAP_LIST_1026, CB_SETCURSEL, field_20088_game_settings.field_2018C_map_idx, 0);
    SendDlgItemMessageA(field_2031C_dlg_hwnd, COMBO_GAME_TYPE_1036, CB_SETCURSEL, field_20088_game_settings.field_20198_game_type - 1, 0);
    NetUi::SetPlayerCountText_51CB30(field_4_maps[field_20088_game_settings.field_2018C_map_idx].field_514_player_count, field_2031C_dlg_hwnd);
    NetUi::SetPoliceEnabledCheckBox_51CCB0(field_20088_game_settings.field_201A0_police_on, field_2031C_dlg_hwnd);
    NetUi::SetFragsNumberAndLabel_51CDC0(field_20088_game_settings.field_20198_game_type,
                                                 field_20088_game_settings.field_20194_frag_limit,
                                                 field_2031C_dlg_hwnd);
    NetUi::SetGameSpeedTextLabelAndSlider_51CFC0(field_20088_game_settings.field_20190_game_speed, field_2031C_dlg_hwnd);
    NetUi::SetGameTimeLimitTextBox_51D3B0(field_20088_game_settings.field_201A4_game_time_limit, field_2031C_dlg_hwnd);
}

#pragma warning(push)
#pragma warning(disable : 4715) // not all control paths return a value
MATCH_FUNC(0x51b2f0)
LRESULT NetUi::cb_AddChatMessage_51B2F0(NetUi* pNetUi, Network_ChatMessage* pMsg)
{
    char_type line[260];
    char_type name[260];
    char_type message[260];
    char_type text[2048];
    char_type newText[2048];

    wcstombs(message, pMsg->field_0_message, 260u);
    wcstombs(name, pMsg->field_100_name, 260u);
    sprintf(line, "%s : %s", name, message);

    switch (pNetUi->Get_20310_showing_window_idx_51ACC0())
    {
        case 1:
        {
            GetDlgItemTextA(pNetUi->Get_2031C_HWND_519E20(), CHATBOX_HOST_1022, text, 0x7FF);
            if (strlen(text))
            {
                sprintf(newText, "%s\r\n%s", text, line);
            }
            else
            {
                sprintf(newText, "%s", line);
            }
            SetDlgItemTextA(pNetUi->Get_2031C_HWND_519E20(), CHATBOX_HOST_1022, newText);
            LRESULT lineCount = SendDlgItemMessageA(pNetUi->Get_2031C_HWND_519E20(), CHATBOX_HOST_1022, EM_GETLINECOUNT, 0, 0);
            return SendDlgItemMessageA(pNetUi->Get_2031C_HWND_519E20(), CHATBOX_HOST_1022, EM_LINESCROLL, 0, lineCount);
        }

        case 2:
        {
            GetDlgItemTextA(pNetUi->Get_2031C_HWND_519E20(), CHATBOX_GUEST_1051, text, 0x7FF);
            if (strlen(text))
            {
                sprintf(newText, "%s\r\n%s", text, line);
            }
            else
            {
                sprintf(newText, "%s", line);
            }
            SetDlgItemTextA(pNetUi->Get_2031C_HWND_519E20(), CHATBOX_GUEST_1051, newText);
            LRESULT lineCount = SendDlgItemMessageA(pNetUi->Get_2031C_HWND_519E20(), CHATBOX_GUEST_1051, EM_GETLINECOUNT, 0, 0);
            return SendDlgItemMessageA(pNetUi->Get_2031C_HWND_519E20(), CHATBOX_GUEST_1051, EM_LINESCROLL, 0, lineCount);
        }
    }
    // No return here in the original either
}
#pragma warning(pop)

#pragma warning(push)
#pragma warning(disable : 4715) // not all control paths return a value
#pragma warning(disable : 4700) // name is never filled in, as in the original
MATCH_FUNC(0x51b4f0)
s32 NetUi::AppendChatMessage_51B4F0(s32 hmenu, const char_type* pMsg)
{
    char_type line[260];
    char_type name[260];
    char_type newText[2048];
    char_type text[2048];
    wchar_t playerName[260];
    char_type allText[10000];

    // The original gets our name as a wide string but then formats with the (unset) narrow buffer
    gNetPlay_7071E8.GetPlayerName_521100(playerName, 7);
    sprintf(line, "%s : %s", name, pMsg);
    gChatLineCount_6F8A2C++;

    switch (field_20310_showing_specific_window_idx)
    {
        case GUEST_LOBBY_2: // Guest
        {
            if (gChatLineCount_6F8A2C >= 20)
            {
                // Keep only the last 10 lines
                gChatLineCount_6F8A2C = 10;
                s32 pos = GetWindowTextA(GetDlgItem(Get_2031C_HWND_519E20(), CHATBOX_GUEST_1051), allText, sizeof(allText));
                s32 lines = 0;
                while (pos > 0 && lines < 10)
                {
                    if (allText[pos] == '\n')
                    {
                        lines++;
                    }
                    pos--;
                }
                SetWindowTextA(GetDlgItem(Get_2031C_HWND_519E20(), CHATBOX_GUEST_1051), &allText[pos]);
            }

            GetDlgItemTextA(Get_2031C_HWND_519E20(), CHATBOX_GUEST_1051, text, 0x7FF);
            if (strlen(text))
            {
                sprintf(newText, "%s\r\n%s", text, line);
            }
            else
            {
                sprintf(newText, "%s", line);
            }
            SetDlgItemTextA(Get_2031C_HWND_519E20(), CHATBOX_GUEST_1051, newText);
            LRESULT lineCount = SendDlgItemMessageA(Get_2031C_HWND_519E20(), CHATBOX_GUEST_1051, EM_GETLINECOUNT, 0, 0);
            return SendDlgItemMessageA(Get_2031C_HWND_519E20(), CHATBOX_GUEST_1051, EM_LINESCROLL, 0, lineCount);
        }

        case HOST_LOBBY_1: // Host
        {
            if (gChatLineCount_6F8A2C >= 20)
            {
                // Keep only the last 10 lines
                gChatLineCount_6F8A2C = 10;
                s32 pos = GetWindowTextA(GetDlgItem(Get_2031C_HWND_519E20(), CHATBOX_HOST_1022), allText, sizeof(allText));
                s32 lines = 0;
                while (pos > 0 && lines < 10)
                {
                    if (allText[pos] == '\n')
                    {
                        lines++;
                    }
                    pos--;
                }
                SetWindowTextA(GetDlgItem(Get_2031C_HWND_519E20(), CHATBOX_HOST_1022), &allText[pos]);
            }

            GetDlgItemTextA(Get_2031C_HWND_519E20(), CHATBOX_HOST_1022, text, 0x7FF);
            if (strlen(text))
            {
                sprintf(newText, "%s\r\n%s", text, line);
            }
            else
            {
                sprintf(newText, "%s", line);
            }
            SetDlgItemTextA(Get_2031C_HWND_519E20(), CHATBOX_HOST_1022, newText);
            LRESULT lineCount = SendDlgItemMessageA(Get_2031C_HWND_519E20(), CHATBOX_HOST_1022, EM_GETLINECOUNT, 0, 0);
            return SendDlgItemMessageA(Get_2031C_HWND_519E20(), CHATBOX_HOST_1022, EM_LINESCROLL, 0, lineCount);
        }
    }
    // No return here in the original either
}
#pragma warning(pop)

MATCH_FUNC(0x51b7c0)
void NetUi::SetPlayerNameText_51B7C0()
{
    SetDlgItemTextA(field_2031C_dlg_hwnd, TEXTBOX_PLAYER_NAME_1004, field_1FF80_player_name);
}

MATCH_FUNC(0x51b7e0)
void NetUi::cb_GameSettingsChanged_51B7E0(NetUi* pNetUi, const char_type** ppData)
{
    const char_type* pData = *ppData;
    if (pNetUi->Get_20310_showing_window_idx_51ACC0() == 2)
    {
        pNetUi->ApplyHostGameSettings_51B810(pData);
    }
}

#pragma warning(push)
#pragma warning(disable : 4701) // pSpeedText is used uninitialised for an unknown speed, as in the original
MATCH_FUNC(0x51b810)
void NetUi::ApplyHostGameSettings_51B810(const char_type* pData)
{
    char_type msg[520];
    wchar_t wmsg[520];
    char_type* pSpeedText;

    const NetworkGameSettings* pSettings = reinterpret_cast<const NetworkGameSettings*>(pData);
    if (pSettings)
    {
        field_20088_game_settings = *pSettings;

        u32 i = 0;
        while (1)
        {
            if (i >= static_cast<u32>(field_1FD64_total_map_count))
            {
                break;
            }

            if (!strcmp(field_4_maps[i].field_410_mmp_name, pSettings->field_20088_default_map))
            {
                field_20088_game_settings = *pSettings;
                field_20088_game_settings.field_2018C_map_idx = i;
                SetDlgItemTextA(field_2031C_dlg_hwnd, LABEL_JOINED_GAME_MAP_1054, field_4_maps[i].field_30C_description);
                // A found flag, an i >= count check after the loop or a break all change the code
                goto map_done;
            }
            i++;
        }

        sprintf(msg, "### ERROR ### \r\r\nDo Not Have Map %s", pSettings->field_20088_default_map);
        GetString_519A50(wmsg, msg, 260);
        gNetPlay_7071E8.SendChatMessage_521060(wmsg, -1);
        AppendChatMessage_51B4F0(CHATBOX_HOST_1022, msg);

    map_done:
        switch (pSettings->field_20190_game_speed)
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
        SetDlgItemTextA(field_2031C_dlg_hwnd, LABEL_GAME_SPEED_1055, pSpeedText);

        SetJoinedGamePoliceEnabledText_51CD30(field_20088_game_settings.field_201A0_police_on, field_2031C_dlg_hwnd);
        SetJoinedGameTypeAndFragLimitText_51D0C0(field_20088_game_settings.field_20198_game_type,
                                                 field_20088_game_settings.field_20194_frag_limit,
                                                 field_2031C_dlg_hwnd);
        SetJoinedGameTimeLimitText_51D2F0(field_20088_game_settings.field_201A4_game_time_limit, field_2031C_dlg_hwnd);
    }
}
#pragma warning(pop)

#pragma warning(push)
#pragma warning(disable : 4701) // pSpeedText is used uninitialised for an unknown speed, as in the original
MATCH_FUNC(0x51b9c0)
void NetUi::SetSetting_51B9C0(s32 setting, s32 value)
{
    Network_Buffer data;
    char_type* pSpeedText;

    switch (setting)
    {
        case 1:
            field_20088_game_settings.field_2018C_map_idx = value;
            strcpy(field_20088_game_settings.field_20088_default_map, field_4_maps[value].field_410_mmp_name);
            SetPlayerCountText_51CB30(field_4_maps[value].field_514_player_count, field_2031C_dlg_hwnd);
            gRegistry_6FF968.Set_Network_Setting_587730("map_index", value);
            break;

        case 2:
            field_20088_game_settings.field_20194_frag_limit = value;
            if (field_20088_game_settings.field_20198_game_type == 1)
            {
                gRegistry_6FF968.Set_Network_Setting_587730("f_limit", value);
            }
            else if (field_20088_game_settings.field_20198_game_type == 2)
            {
                gRegistry_6FF968.Set_Network_Setting_587730("s_limit", value);
            }
            break;

        case 3:
            field_20088_game_settings.field_20198_game_type = value;
            gRegistry_6FF968.Set_Network_Setting_587730("game_type", value);
            if (field_20088_game_settings.field_20198_game_type == 1)
            {
                field_20088_game_settings.field_20194_frag_limit = gRegistry_6FF968.Get_Network_Setting_587690("f_limit", 3);
            }
            else if (field_20088_game_settings.field_20198_game_type == 2)
            {
                field_20088_game_settings.field_20194_frag_limit = gRegistry_6FF968.Get_Network_Setting_587690("s_limit", 10000);
            }
            break;

        case 6: // 10.5.exe's jump table: 6 is the police flag, 4 the tick count
            field_20088_game_settings.field_201A0_police_on = value;
            gRegistry_6FF968.Set_Network_Setting_587730("police", value);
            break;

        case 4:
            field_20088_game_settings.field_2019C_tick_count = value;
            break;

        case 5:
            field_20088_game_settings.field_20190_game_speed = value;
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
            SetDlgItemTextA(field_2031C_dlg_hwnd, LABEL_GAME_SPEED_TEXT_1032, pSpeedText);
            gRegistry_6FF968.Set_Network_Setting_587730("game_speed", value);
            break;

        case 7:
            field_20088_game_settings.field_201A4_game_time_limit = value;
            gRegistry_6FF968.Set_Network_Setting_587730("game_time_limit", value);
            break;
    }

    data.field_0_data = &field_20088_game_settings;
    data.field_4_len = sizeof(field_20088_game_settings);
    gNetPlay_7071E8.SetGameSettings_521170(&data);
}
#pragma warning(pop)

MATCH_FUNC(0x51bbc0)
void NetUi::IncCount_51BBC0()
{
    field_1FD6C_count++;
}

MATCH_FUNC(0x51bbd0)
u32 NetUi::GetCount_51BBD0()
{
    return field_1FD6C_count;
}

MATCH_FUNC(0x51bbe0)
void NetUi::DecCount_51BBE0()
{
    field_1FD6C_count--;
}

MATCH_FUNC(0x51bbf0)
void NetUi::ResetCount_51BBF0()
{
    field_1FD6C_count = 0;
}

MATCH_FUNC(0x51bc00)
void NetUi::cb_SavePlayerName_51BC00(NetUi* pThis)
{
    char String[260];
    wchar_t Dest[260];

    GetDlgItemTextA(pThis->Get_2031C_HWND_519E20(), TEXTBOX_PLAYER_NAME_1004, String, GTA2_COUNTOF(String));
    GetString_519A50(Dest, String, GTA2_COUNTOF(String));
    pThis->SetPlayNameAndSaveToRegistry_51BD40(Dest, String);
    pThis->SetEndDialog_51BC70(1);
}

MATCH_FUNC(0x51bc70)
void NetUi::SetEndDialog_51BC70(s32 bEnd)
{
    field_1FD68_bEndDialog = bEnd;
}

MATCH_FUNC(0x51bc80)
s32 NetUi::GetEndDialog_51BC80()
{
    return field_1FD68_bEndDialog;
}

#pragma function(strcpy)

MATCH_FUNC(0x51bc90)
void NetUi::LoadPlayerNameFromRegistry_51BC90()
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
            strcpy(field_1FF80_player_name, reinterpret_cast<char*>(Data));
            GetString_519A50(field_1FD78_player_name_2, reinterpret_cast<char*>(Data), GTA2_COUNTOF(field_1FD78_player_name_2));
        }
        RegCloseKey(phkResult);
    }
}

MATCH_FUNC(0x51bd40)
void NetUi::SetPlayNameAndSaveToRegistry_51BD40(const wchar_t* pPlayerNameW, const char* pPlayerNameA)
{
    strcpy(field_1FF80_player_name, pPlayerNameA);
    wcscpy(field_1FD78_player_name_2, pPlayerNameW);

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
LRESULT __stdcall NetUi::subclass_proc_51BDD0(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    HWND parentHwnd = GetParent(hWnd);
    if (parentHwnd)
    {
        NetUi* pThis = reinterpret_cast<NetUi*>(GetWindowLongA(parentHwnd, 8));
        if (Msg == WM_KEYUP && wParam == VK_RETURN && (lParam & (KF_MENUMODE << 16)) == 0)
        {
            switch (pThis->Get_20310_showing_window_idx_51ACC0())
            {
                case 2:
                    pThis->OnEnterPressed_51BEB0(TEXTFIELD_CHAT_GUEST_1053, CHATBOX_GUEST_1051);
                    break;

                case 1:
                    pThis->OnEnterPressed_51BEB0(TEXTFIELD_CHAT_HOST_1025, CHATBOX_HOST_1022);
                    break;
            }
        }

        switch (pThis->Get_20310_showing_window_idx_51ACC0())
        {
            case 2:
                return CallWindowProcA(pThis->field_1FD70_old_proc, hWnd, Msg, wParam, lParam);
            case 1:
                return CallWindowProcA(pThis->field_1FD74_old_proc, hWnd, Msg, wParam, lParam);
        }
    }
    return 0;
}

MATCH_FUNC(0x51beb0)
void NetUi::OnEnterPressed_51BEB0(s32 nIDDlgItem, s32 hmenu)
{
    char_type String[128];
    wchar_t Dest[260];
    GetDlgItemTextA(field_2031C_dlg_hwnd, nIDDlgItem, String, 127);
    NetUi::GetString_519A50(Dest, String, 260);
    gNetPlay_7071E8.SendChatMessage_521060(Dest, -1);
    SetDlgItemTextA(field_2031C_dlg_hwnd, nIDDlgItem, "");
    NetUi::AppendChatMessage_51B4F0(hmenu, String);
}

// https://decomp.me/scratch/tQkqa
MATCH_FUNC(0x51bfa0)
void NetUi::EnumerateMaps_51BFA0()
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
        strcpy(maps[0].field_410_mmp_name, findFileData.cFileName);
        while (FindNextFileA(hFindFile, &findFileData))
        {
            if (map_count >= 100)
            {
                break;
            }
            strcpy(maps[map_count].field_410_mmp_name, findFileData.cFileName);
            map_count++;
        }
        FindClose(hFindFile);
    }

    for (u32 i = 0; i < map_count; i++)
    {
        strcpy(FileName, "data\\");
        strcat(FileName, maps[i].field_410_mmp_name);
        GetPrivateProfileStringA("MapFiles", "GMPFile", "", maps[i].field_0_map_name, 0x103u, FileName);
        GetPrivateProfileStringA("MapFiles", "STYFile", "", maps[i].field_104_style_name, 0x103u, FileName);
        GetPrivateProfileStringA("MapFiles", "SCRFile", "", maps[i].field_208_script_name, 0x103u, FileName);
        GetPrivateProfileStringA("MapFiles", "Description", "", maps[i].field_30C_description, 0x103u, FileName);
        GetPrivateProfileStringA("MapFiles", "PlayerCount", "", gTmpBuffer_67C598, 2u, FileName);
        maps[i].field_514_player_count = atoi(gTmpBuffer_67C598);

        // Only keep maps whose files all exist
        _chdir("data");
        if (GetFileAttributesA(maps[i].field_0_map_name) != -1)
        {
            if (GetFileAttributesA(maps[i].field_104_style_name) != -1)
            {
                if (GetFileAttributesA(maps[i].field_208_script_name) != -1)
                {
                    OutputDebugStringA(field_4_maps[i].field_0_map_name);
                    memcpy(&field_4_maps[field_1FD64_total_map_count], &maps[i], sizeof(Network_Enumerated_Map));
                    field_1FD64_total_map_count++;
                }
                else
                {
                    NetUi::ShowUnableToOpenFileError_51CAD0(maps[i].field_410_mmp_name, maps[i].field_208_script_name);
                }
            }
            else
            {
                NetUi::ShowUnableToOpenFileError_51CAD0(maps[i].field_410_mmp_name, maps[i].field_104_style_name);
            }
        }
        else
        {
            NetUi::ShowUnableToOpenFileError_51CAD0(maps[i].field_410_mmp_name, maps[i].field_0_map_name);
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
                if (_strcmpi(field_4_maps[j].field_30C_description, field_4_maps[j + 1].field_30C_description) > 0)
                {
                    memcpy(&tmp, &field_4_maps[j], sizeof(Network_Enumerated_Map));
                    memcpy(&field_4_maps[j], &field_4_maps[j + 1], sizeof(Network_Enumerated_Map));
                    memcpy(&field_4_maps[j + 1], &tmp, sizeof(Network_Enumerated_Map));
                    bSwapped = 1;
                }
            }
        } while (bSwapped == 1);
    }
}

MATCH_FUNC(0x51c630)
void __stdcall NetUi::OnWmHScroll_51C630(HWND hWnd, HWND hTrackBar, s32 code, s32 pos)
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
                const int pos2 = SendDlgItemMessageA(hWnd, TRACKBAR_GAME_SPEED_1031, TBM_GETPOS, 0, 0);
                pThis->SetSetting_51B9C0(5, static_cast<s32>(pos2)); // set game speed
            }
            break;

            case 5:
                return;
        }
    }
}

MATCH_FUNC(0x51c7f0)
void NetUi::CopyGameSettings_51C7F0(NetworkGameSettings* pSettings)
{
    if (pSettings)
    {
        memcpy(pSettings, &field_20088_game_settings, sizeof(NetworkGameSettings));
    }
}

MATCH_FUNC(0x51c830)
void NetUi::LoadGameSettingsFromRegistry_51C830()
{
    field_20088_game_settings.field_2018C_map_idx = gRegistry_6FF968.Get_Network_Setting_587690("map_index", 0);
    if (field_20088_game_settings.field_2018C_map_idx >= static_cast<u32>(field_1FD64_total_map_count))
    {
        field_20088_game_settings.field_2018C_map_idx = 0;
    }
    field_20088_game_settings.field_201A4_game_time_limit = gRegistry_6FF968.Get_Network_Setting_587690("game_time_limit", 1);
    field_20088_game_settings.field_20190_game_speed = gRegistry_6FF968.Get_Network_Setting_587690("game_speed", 1);
    field_20088_game_settings.field_20198_game_type = gRegistry_6FF968.Get_Network_Setting_587690("game_type", 1);
    field_20088_game_settings.field_201A0_police_on = gRegistry_6FF968.Get_Network_Setting_587690("police", 0);

    if (field_20088_game_settings.field_20198_game_type == 1)
    {
        field_20088_game_settings.field_20194_frag_limit = gRegistry_6FF968.Get_Network_Setting_587690("f_limit", 3);
    }
    else
    {
        field_20088_game_settings.field_20194_frag_limit = gRegistry_6FF968.Get_Network_Setting_587690("s_limit", 10000);
    }

    strcpy(field_20088_game_settings.field_20088_default_map,
           field_4_maps[field_20088_game_settings.field_2018C_map_idx].field_410_mmp_name);
}

MATCH_FUNC(0x51ca10)
char_type* NetUi::GetMapName_51CA10()
{
    return field_4_maps[field_20088_game_settings.field_2018C_map_idx].field_0_map_name;
}

MATCH_FUNC(0x51ca50)
char_type* NetUi::GetMapStyName_51CA50()
{
    return field_4_maps[field_20088_game_settings.field_2018C_map_idx].field_104_style_name;
}

MATCH_FUNC(0x51ca90)
char_type* NetUi::GetMapScrName_51CA90()
{
    return field_4_maps[field_20088_game_settings.field_2018C_map_idx].field_208_script_name;
}

MATCH_FUNC(0x51cad0)
void __stdcall NetUi::ShowUnableToOpenFileError_51CAD0(const char_type* pMmpName, const char_type* pFileName)
{
    char Text[256];
    sprintf(Text, "Unable to open file: %s in mmpfile: %s", pFileName, pMmpName);
    MessageBoxA(NULL, Text, "", MB_ICONEXCLAMATION); // TODO: the original passes its empty string global 0x67DC88
}

MATCH_FUNC(0x51cb30)
void NetUi::SetPlayerCountText_51CB30(s32 count, HWND hDlg)
{
    char String[260];
    const char* pStr = GetString_519A00("netui13");
    sprintf(String, "%s %d", pStr, count);
    SetDlgItemTextA(hDlg, LABEL_PLAYERS_REQUIRED_1033, String);
}

MATCH_FUNC(0x51cbc0)
void NetUi::UpdateButtonsEnabledState_51CBC0()
{
    if (NetUi::GetCount_51BBD0() == field_4_maps[field_20088_game_settings.field_2018C_map_idx].field_514_player_count)
    {
        EnableWindow(GetDlgItem(NetUi::Get_2031C_HWND_519E20(), BUTTON_START_GAME_1021), true);
    }
    else
    {
        EnableWindow(GetDlgItem(NetUi::Get_2031C_HWND_519E20(), BUTTON_START_GAME_1021), false);
    }

    if (NetUi::GetCount_51BBD0() > 1)
    {
        EnableWindow(GetDlgItem(NetUi::Get_2031C_HWND_519E20(), BUTTON_REJECT_PLAYER_1020), true);
    }
    else
    {
        EnableWindow(GetDlgItem(NetUi::Get_2031C_HWND_519E20(), BUTTON_REJECT_PLAYER_1020), false);
    }
}

MATCH_FUNC(0x51ccb0)
void NetUi::SetPoliceEnabledCheckBox_51CCB0(s32 bPoliceOn, HWND hDlg)
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
void NetUi::SetJoinedGamePoliceEnabledText_51CD30(s32 bPoliceOn, HWND hDlg)
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
void NetUi::SetFragsNumberAndLabel_51CDC0(s32 gameType, s32 fragLimit, HWND hDlg)
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
        if (NetUi::Get_20310_showing_window_idx_51ACC0() == 1)
        {
            ShowWindow(GetDlgItem(hDlg, TEXTBOX_FRAG_LIMIT_NUMBER_1059), SW_SHOW);
            ShowWindow(GetDlgItem(hDlg, LABEL_FRAG_LIMIT_TEXT_1035), SW_SHOW);
        }
    }
    else
    {
        if (NetUi::Get_20310_showing_window_idx_51ACC0() == 1) // hide if its not a frag game?
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
void NetUi::SetGameSpeedTextLabelAndSlider_51CFC0(LPARAM game_speed, HWND hDlg)
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
void NetUi::SetJoinedGameTypeAndFragLimitText_51D0C0(s32 game_type, s32 frag_limit, HWND hDlg)
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
void NetUi::SetJoinedGameTimeLimitText_51D2F0(s32 gameTimeLimit, HWND hDlg)
{
    char_type Buffer[260];
    char_type String[260];
    strcpy(Buffer, GetString_519A00("netui21"));
    sprintf(String, "%s: %d", Buffer, gameTimeLimit);
    SetDlgItemTextA(hDlg, LABEL_JOINED_GAME_TIME_LIMIT_1060, String);
}

MATCH_FUNC(0x51d3b0)
void NetUi::SetGameTimeLimitTextBox_51D3B0(s32 timeLimitMins, HWND hDlg)
{
    char_type String[260];
    sprintf(String, "%d", timeLimitMins);
    SetDlgItemTextA(hDlg, TEXTBOX_GAME_TIME_LIMIT_1038, String);
}