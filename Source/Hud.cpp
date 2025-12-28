#include "Hud.hpp"
#include "Car_BC.hpp"
#include "Draw.hpp"
#include "Frontend.hpp"
#include "Game_0x40.hpp"
#include "Gang.hpp"
#include "Globals.hpp"
#include "Object_5C.hpp"
#include "Ped.hpp"
#include "Player.hpp"
#include "Police_7B8.hpp"
#include "Weapon_30.hpp"
#include "debug.hpp"
#include "error.hpp"
#include "frosty_pasteur_0xC1EA8.hpp"
#include "gbh_graphics.hpp"
#include "gtx_0x106C.hpp"
#include "keybrd_0x204.hpp"
#include "lucid_hamilton.hpp"
#include "registry.hpp"
#include "rng.hpp"
#include "root_sound.hpp"
#include "text_0x14.hpp"

DEFINE_GLOBAL(Hud_2B00*, gHud_2B00_706620, 0x706620);
DEFINE_GLOBAL(s16, word_706600, 0x706600); //, TODO, 0xUNKNOWN);
DEFINE_GLOBAL(s16, word_7064B8, 0x7064B8); //, TODO, 0xUNKNOWN);
DEFINE_GLOBAL(u16, word_706618, 0x706618); //, TODO, 0xUNKNOWN);
DEFINE_GLOBAL(s16, word_706508, 0x706508); //, TODO, 0xUNKNOWN);
DEFINE_GLOBAL(u16, word_70646C, 0x70646C); //, TODO, 0xUNKNOWN);
DEFINE_GLOBAL(u16, word_7065C4, 0x7065C4);
DEFINE_GLOBAL(u16, word_7062F0, 0x7062F0);
DEFINE_GLOBAL(u16, word_7063F8, 0x7063F8);
DEFINE_GLOBAL(u16, word_7062DC, 0x7062DC);
DEFINE_GLOBAL(u16, word_70643E, 0x70643E);
DEFINE_GLOBAL_ARRAY(char, byte_67CE50, 264, 0x67CE50); //, TODO, 0xUNKNOWN);
DEFINE_GLOBAL(s16, word_7064D8, 0x7064D8);
DEFINE_GLOBAL(s32, dword_7064C0, 0x7064C0);
DEFINE_GLOBAL(s32, dword_7063B0, 0x7063B0);
DEFINE_GLOBAL(s32, dword_7065B4, 0x7065B4);
DEFINE_GLOBAL(s32, dword_706338, 0x706338);

DEFINE_GLOBAL(Fix16, phone_x_67CD14, 0x67CD14);
DEFINE_GLOBAL(Fix16, phone_y_67CD0C, 0x67CD0C);
DEFINE_GLOBAL(Fix16, dword_67CD10, 0x67CD10);

DEFINE_GLOBAL_INIT(Ang16, word_706610, Ang16(0), 0x706610);

// TODO
EXTERN_GLOBAL_ARRAY(wchar_t, tmpBuff_67BD9C, 640);

// TODO
EXTERN_GLOBAL(char_type, gLighting_626A09);

// TODO: move
EXTERN_GLOBAL(s32, bStartNetworkGame_7081F0);

EXTERN_GLOBAL_ARRAY(wchar_t, word_67DC8C, 32);

STUB_FUNC(0x5cfe40)
void Garox_13C0_sub::DrawPlayerNames_5CFE40()
{
    NOT_IMPLEMENTED;
}

// ----------------------------------------------------

MATCH_FUNC(0x5d56b0)
void Garox_1_v2::sub_5D56B0()
{
    if (this->field_0_timer)
    {
        this->field_CC = Frontend::sub_5D8990(field_2_str, word_7064D8);
    }
}

STUB_FUNC(0x5d56d0)
void Garox_1_v2::sub_5D56D0()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x5d5730)
void Garox_1_v2::sub_5D5730(const wchar_t* pStr)
{
    this->field_0_timer = 120;
    wcscpy(this->field_2_str, pStr);
    sub_5D56B0();
}

MATCH_FUNC(0x5d5760)
void Garox_1_v2::sub_5D5760()
{
    if (field_0_timer)
    {
        field_0_timer--;
    }
}

STUB_FUNC(0x5d5770)
void Garox_1_v2::sub_5D5770(u8* a2, u8* a3)
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x5d58f0)
Garox_1_v2::Garox_1_v2()
{
    field_0_timer = 0;
}

// ----------------------------------------------------

MATCH_FUNC(0x5d15e0)
char_type Garox_2A25_sub::IsTypingOnChat_5D15E0(s32 action, Player* pPlayer)
{
    if (bStartNetworkGame_7081F0 && pPlayer->field_794_is_chatting)
    {
        if (action == DIK_RETURN)
        {
            pPlayer->field_794_is_chatting = 0;
            return 1;
        }
        else if (action == DIK_BACK)
        {
            if (pPlayer->field_838_f796_idx > 0)
            {
                pPlayer->field_838_f796_idx--;
                pPlayer->field_796_chat_text[pPlayer->field_838_f796_idx] = 0;
            }
            return 1;
        }
        else if (action == DIK_SPACE)
        {
            if (pPlayer->field_838_f796_idx < 79)
            {
                pPlayer->field_796_chat_text[pPlayer->field_838_f796_idx] = ' ';
                pPlayer->field_838_f796_idx++;
                pPlayer->field_796_chat_text[pPlayer->field_838_f796_idx] = 0;
            }
            return 1;
        }
        else
        {
            const u16 char_value = gText_0x14_704DFC->sub_5B58D0(gKeybrd_0x204_6F52F4->GetKey_4D5F40(action));
            if (char_value)
            {
                if (pPlayer->field_838_f796_idx < 79)
                {
                    pPlayer->field_796_chat_text[pPlayer->field_838_f796_idx] = char_value;
                    pPlayer->field_838_f796_idx++;
                    pPlayer->field_796_chat_text[pPlayer->field_838_f796_idx] = 0;
                }
                return 1;
            }
        }
    }
    return 0;
}

STUB_FUNC(0x5d16b0)
void Garox_2A25_sub::sub_5D16B0()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x5d17d0)
bool Garox_2A25_sub::sub_5D17D0(s32 key_idx)
{
    if (bStartNetworkGame_7081F0)
    {
        if (gGame_0x40_67E008->field_38_orf1->field_794_is_chatting)
        {
            if (key_idx == DIK_RETURN || key_idx == DIK_BACK || key_idx == DIK_SPACE)
            {
                return true;
            }

            if (gText_0x14_704DFC->sub_5B58D0(gKeybrd_0x204_6F52F4->GetKey_4D5F40(key_idx)))
            {
                return true;
            }
        }
    }
    return false;
}

MATCH_FUNC(0x5d1830)
void Garox_2A25_sub::StartChatting_5D1830(Player* pPlayer)
{
    pPlayer->field_794_is_chatting = true;
    pPlayer->field_838_f796_idx = 0;
    pPlayer->field_796_chat_text[0] = 0;
}

// ----------------------------------------------------

MATCH_FUNC(0x5d13c0)
char_type Garox_12EC_sub::IsOnQuitMessage_5D13C0(s32 action, Player* pPlayer)
{
    if (pPlayer->field_78A_show_quit_message)
    {
        if (action == DIK_RETURN)
        {
            pPlayer->field_78A_show_quit_message = false;
            if (pPlayer->field_0_bIsUser)
            {
                gGame_0x40_67E008->sub_4B8C00(1, 2);
            }

            if ((u8)bStartNetworkGame_7081F0)
            {
                gYouthful_einstein_6F8450.field_20[pPlayer->field_2E_idx] = 1;
            }

            return true;
        }
        else if (action == DIK_ESCAPE)
        {
            pPlayer->field_78A_show_quit_message = false;
            return true;
        }
    }
    return false;
}

MATCH_FUNC(0x5d1430)
void Garox_12EC_sub::DrawQuitMessage_5D1430()
{
    if (gGame_0x40_67E008->field_38_orf1->field_78A_show_quit_message)
    {
        wchar_t* pQuitText = gText_0x14_704DFC->Find_5B5F90("quit1");

        s32 v2 = Frontend::sub_5D8990(pQuitText, word_7064D8);
        s32 v4 = sub_5D8940(pQuitText, word_7064D8);
        s32 v6 = ((3 * (160 - v4)) / 2);

        DrawText_5D7720(pQuitText, (u32)((640 - v2) / 2), v6 - v4, word_7064D8, 2, 0, 0, 0);

        wchar_t* pQuitText2 = gText_0x14_704DFC->Find_5B5F90("quit2");
        s32 v9 = Frontend::sub_5D8990(pQuitText2, word_7064D8);

        DrawText_5D7720(pQuitText2, (u32)((640 - v9) / 2), (u32)v6, word_7064D8, 2, 0, 0, 0);

        wchar_t* pQuitText3 = gText_0x14_704DFC->Find_5B5F90("quit3");
        s32 v14 = Frontend::sub_5D8990(pQuitText3, word_7064D8);

        DrawText_5D7720(pQuitText3, (u32)((640 - v14) / 2), v6 + v4, word_7064D8, 2, 0, 0, 0);
    }
}

MATCH_FUNC(0x5d15a0)
bool Garox_12EC_sub::sub_5D15A0(s32 a1)
{
    return gGame_0x40_67E008->field_38_orf1->field_78A_show_quit_message && (a1 == 28 || a1 == 1);
}

MATCH_FUNC(0x5d15d0)
void Garox_12EC_sub::sub_5D15D0(Player* pPlayer)
{
    pPlayer->field_78A_show_quit_message = 1;
}

// ----------------------------------------------------

MATCH_FUNC(0x5cf620)
void Garox_4::sub_5CF620()
{
    do
    {
        field_0_value++;
        if (field_0_value > 9999)
        {
            field_0_value = 0;
        }
        sprintf(gTmpBuffer_67C598, "%d", field_0_value);
    } while (!gText_0x14_704DFC->sub_5B5FA0(gTmpBuffer_67C598));
    gHud_2B00_706620->field_DC.sub_5D4400(3, gTmpBuffer_67C598);
    swprintf(tmpBuff_67BD9C, L"%d", field_0_value);
    gHud_2B00_706620->field_111C.ShowMessage_5D1A00(tmpBuff_67BD9C, 3);
}

MATCH_FUNC(0x5cf6b0)
void Garox_4::sub_5CF6B0()
{
    do
    {
        field_0_value--;
        if (field_0_value < 0)
        {
            field_0_value = 9999;
        }
        sprintf(gTmpBuffer_67C598, "%d", field_0_value);
    } while (!gText_0x14_704DFC->sub_5B5FA0(gTmpBuffer_67C598));
    gHud_2B00_706620->field_DC.sub_5D4400(3, gTmpBuffer_67C598);
    swprintf(tmpBuff_67BD9C, L"%d", field_0_value);
    gHud_2B00_706620->field_111C.ShowMessage_5D1A00(tmpBuff_67BD9C, 3);
}

// ----------------------------------------------------

STUB_FUNC(0x5d63b0)
void Garox_12E4_sub::DrawPause_5D63B0()
{
    NOT_IMPLEMENTED;
}

// ----------------------------------------------------

MATCH_FUNC(0x5d1850)
void Hud_Message_1C8::ClearTimeToShow_5D1850()
{
    field_0_time_to_show = 0;
}

STUB_FUNC(0x5d1860)
void Hud_Message_1C8::sub_5D1860()
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x5d1940)
void Hud_Message_1C8::DrawMessage_5D1940()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x5d1a00)
void Hud_Message_1C8::ShowMessage_5D1A00(wchar_t* pStr, s32 type)
{
    if (field_0_time_to_show <= 0 || type >= field_1C4_type)
    {
        field_1C4_type = type;
        wcscpy(field_2_str, pStr);
        gText_0x14_704DFC->sub_5B5B80(field_2_str);
        field_0_time_to_show = 90;
        sub_5D1860();
    }
}

MATCH_FUNC(0x5d1ab0)
void Hud_Message_1C8::sub_5D1AB0()
{
    if (field_0_time_to_show != 0)
    {
        field_0_time_to_show--;
    }
}

MATCH_FUNC(0x5d1ae0)
Hud_Message_1C8::Hud_Message_1C8()
{
    field_0_time_to_show = 0;
    field_1C4_type = 1;
}

// ----------------------------------------------------

MATCH_FUNC(0x5d5c80)
void Garox_1118_sub::DrawPlayerStats_5D5C80()
{
    Player* pPlayer = gGame_0x40_67E008->field_38_orf1;

    s16 ammo_idx = pPlayer->field_788_curr_weapon_idx;
    if (ammo_idx == -1)
    {
        sub_5D6060(-1, 0);
    }
    else
    {
        u8 unk = pPlayer->field_718_weapons[ammo_idx]->get_ammo_4A4FB0();
        sub_5D6060(ammo_idx, unk);
    }

    s32 v2 = 639;
    for (s32 i = 0; i < 17; i++)
    {
        u16 unknown = pPlayer->field_6F4_power_up_timers[i];
        if (unknown)
        {
            v2 = sub_5D61A0(i, v2, unknown);
        }
    }

    thirsty_lamarr* v5 = pPlayer->field_2D4_unk.sub_592360();
    s32 v8 = v5->sub_492260(639, 4);

    if (bStartNetworkGame_7081F0)
    {
        sub_5D7670(6, 16, v8 - 8, 14, word_706610, pPlayer->field_78C, pPlayer->field_790, 0, 0);
    }
    else
    {
        if (pPlayer->field_60 == 0)
        {
            sub_5D7670(6, 16, v8 - 8, 14, word_706610, 2, 0, 0, 0);
        }
        else
        {
            sub_5D7670(6, 16, v8 - 8, 14, word_706610, 7, 6, 0, 0);
        }
    }

    wchar_t Buffer[16];

    if (bStartNetworkGame_7081F0)
    {
        if (gLucid_hamilton_67E8E0.sub_4C5BC0() == TAG_GAME_3)
        {
            swprintf(Buffer,
                     L"%2d:%02d",
                     gYouthful_einstein_6F8450.field_4_time[pPlayer->field_2E_idx] / 60,
                     gYouthful_einstein_6F8450.field_4_time[pPlayer->field_2E_idx] % 60);

            const s32 unknownn = (pPlayer->field_78C != 7) ? 2 : 8;
            DrawText_5D7720(Buffer, 420, 4, word_703BAA, unknownn, pPlayer->field_790 - 1, 0, 0);
        }
        else
        {
            thirsty_lamarr* v16 = pPlayer->field_2D4_unk.sub_5935B0();
            v16->sub_492260(490, 4);
        }

        s32 ypos = 8;
        for (Player* pMultiPlayer = gGame_0x40_67E008->sub_4B9CD0(); pMultiPlayer != NULL;
             pMultiPlayer = gGame_0x40_67E008->IterateNextPlayer_4B9D10())
        {
            if (pMultiPlayer->field_0_bIsUser == 0)
            {
                thirsty_lamarr* v19 = pMultiPlayer->field_2D4_unk.sub_592360();
                s32 v21 = v19->sub_492430(16, ypos);

                sub_5D7670(6, 16, 8, ypos + 10, word_706610, pMultiPlayer->field_78C, pMultiPlayer->field_790, 0, 0);

                if (gLucid_hamilton_67E8E0.sub_4C5BC0() == TAG_GAME_3)
                {
                    swprintf(Buffer,
                             L"%2d:%02d",
                             gYouthful_einstein_6F8450.field_4_time[pMultiPlayer->field_2E_idx] / 60,
                             gYouthful_einstein_6F8450.field_4_time[pMultiPlayer->field_2E_idx] % 60);

                    const s32 very_unknown = (pMultiPlayer->field_78C != 7) ? 2 : 8;
                    DrawText_5D7720(Buffer, v21 + 20, (u32)ypos, word_703BAA, very_unknown, pMultiPlayer->field_790 - 1, 0, 0);
                }
                else
                {
                    thirsty_lamarr* v29 = pMultiPlayer->field_2D4_unk.sub_5935B0();
                    v29->sub_492430(v21 + 20, ypos);
                }
                ypos += 27;
            }
        }
    }
    else
    {
        s32 lives_xpos = pPlayer->field_684_lives.sub_492260(523, 28);
        sub_5D7670(6, 17, lives_xpos - 7, 32, word_706610, 2, 0, 0, 0);

        s32 multiplier_xpos = pPlayer->field_6BC_multpliers.sub_492260(523, 11);
        sub_5D7670(6, 18, multiplier_xpos - 7, 18, word_706610, 2, 0, 0, 0);
    }
}

MATCH_FUNC(0x5D6060)
void __stdcall sub_5D6060(s16 ammo_idx, u8 ammo_count)
{
    if (ammo_idx != -1)
    {
        u16 v3 = gGtx_0x106C_703DD4->convert_sprite_pal_5AA460(6, ammo_idx + 85);
        s32 width = gGtx_0x106C_703DD4->get_sprite_index_5AA440(v3)->field_4_width;
        u16 v5 = gGtx_0x106C_703DD4->convert_sprite_pal_5AA460(6, ammo_idx + 85);
        s32 height = gGtx_0x106C_703DD4->get_sprite_index_5AA440(v5)->field_5_height;

        sub_5D7670(6, ammo_idx + 85, 638 - width / 2, height / 2 + 44, word_706610, 2, 0, 0, 0);

        if (ammo_idx != 21 && ammo_idx != 20)
        {
            if (ammo_count == 0xFF)
            {
                swprintf(tmpBuff_67BD9C, L"K.F.");
            }
            else
            {
                swprintf(tmpBuff_67BD9C, L"%d", ammo_count);
            }
            DrawText_5D7720(tmpBuff_67BD9C,
                            (u32)(638 - Frontend::sub_5D8990(tmpBuff_67BD9C, word_70646C)),
                            82,
                            word_70646C,
                            8,
                            6,
                            0,
                            0);
        }
    }
}

STUB_FUNC(0x5D61A0)
s32 __stdcall sub_5D61A0(s32 a1, s32 a2, u16 a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x5d6290)
void Garox_1118_sub::sub_5D6290()
{
    Player* pPlayerIter = gGame_0x40_67E008->sub_4B9CD0();
    while (pPlayerIter)
    {
        thirsty_lamarr* pLamarr1 = pPlayerIter->field_2D4_unk.sub_592360();
        pLamarr1->sub_4925E0();
        thirsty_lamarr* pLamarr2 = pPlayerIter->field_2D4_unk.sub_5935B0();
        pLamarr2->sub_4925E0();
        pPlayerIter = gGame_0x40_67E008->IterateNextPlayer_4B9D10();
    }
    Player* pPlayer = gGame_0x40_67E008->field_38_orf1;
    pPlayer->field_684_lives.sub_4925E0();
    pPlayer->field_6BC_multpliers.sub_4925E0();
}

// ----------------------------------------------------

STUB_FUNC(0x5cf730)
void Garox_110C_sub::sub_5CF730()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x5cf910)
void Garox_110C_sub::Draw_5CF910()
{
    if (field_284E)
    {
        const s32 drawtype = 2;
        Player* pPlayer = gGame_0x40_67E008->field_38_orf1;
        Camera_0xBC* pCam;
        if (pPlayer->field_68 == 2 || pPlayer->field_68 == 3)
        {
            pCam = &pPlayer->field_208_aux_game_camera;
        }
        else
        {
            pCam = &pPlayer->field_90_game_camera;
        }

        DrawFigure_5D7EC0(6, 0, field_110C, field_1110, field_1114, pCam->field_A8_ui_scale, drawtype, 0, 1, 14, 1);
    }
}

// ----------------------------------------------------

MATCH_FUNC(0x5d0260)
void Garox_1108_sub::DrawHealth_5D0260()
{
    s32 half_hearts;
    s32 xpos = 551;
    s32 health = gGame_0x40_67E008->field_38_orf1->GetPlayerPed_41D020()->get_health_433B70();

    if (health == 100)
    {
        half_hearts = 10;
    }
    else
    {
        half_hearts = health / 10 + 1;
    }

    // Draw complete hearts
    for (s32 complete_hearts = half_hearts / 2; complete_hearts > 0; complete_hearts--, xpos += 20)
    {
        sub_5D7670(6, 113, xpos, (u32)34, word_706610, 2, 0, 0, 0);
    }

    // Draw half heart
    if (half_hearts % 2 == 1)
    {
        if (half_hearts > 0)
        {
            xpos -= 2;
        }
        sub_5D7670(6, 114, xpos, (u32)34, word_706610, 2, 0, 0, 0);
    }

    // Draw debug stuff
    if (bDo_show_instruments_67D64C)
    {
        swprintf(tmpBuff_67BD9C, L"%d%%", health);
        DrawText_5D7720(tmpBuff_67BD9C, (u32)551, (u32)34, word_706600, 2, 0, 0, 0);
    }
}

// ----------------------------------------------------

MATCH_FUNC(0x5d53e0)
void Garox_1::sub_5D53E0()
{
    if (field_0_timer)
    {
        field_84 = Frontend::sub_5D8990(field_2_str, word_7064B8);
    }
}

MATCH_FUNC(0x5d5420)
void Garox_1::sub_5D5420()
{
    if (field_0_timer)
    {
        DrawText_5D7720(field_2_str, (640 - field_84) / 2, 32, word_7064B8, 8, 5, 0, 0);
    }
}

MATCH_FUNC(0x5d5600)
void Garox_1::sub_5D5600(u8 a2)
{
    sprintf(byte_67CE50, "c%02d", a2);
    field_0_timer = 90;
    wchar_t* pStr = gText_0x14_704DFC->Find_5B5F90(byte_67CE50);
    wcscpy(field_2_str, pStr);
    sub_5D53E0();
}

MATCH_FUNC(0x5d5690)
void Garox_1::sub_5D5690()
{
    if (field_0_timer)
    {
        field_0_timer--;
    }
}

MATCH_FUNC(0x5d56a0)
Garox_1::Garox_1()
{
    field_0_timer = 0;
}

// ----------------------------------------------------

MATCH_FUNC(0x5cf970)
void Garox_27B5_sub::ShowPlayerCoords_5CF970()
{
    if (field_27B5_show_coords)
    {
        Player* pPlayer = gGame_0x40_67E008->field_38_orf1;

        Ped* pPed;
        if (pPlayer->field_68 == 2 || pPlayer->field_68 == 3)
        {
            pPed = pPlayer->field_2C8_unkq;
        }
        else
        {
            pPed = pPlayer->field_2C4_player_ped;
        }

        Gang_144* pZone = pPlayer->field_34_gang_curr_location;
        wchar_t* pZoneName;
        if (pZone)
        {
            pZoneName = pZone->get_name_wide_4BED30();
        }
        else
        {
            pZoneName = word_67DC8C;
        }

        Car_BC* pCar = pPed->field_16C_car;
        wchar_t* pCarOrPedStr;
        if (pCar)
        {
            pCarOrPedStr = pCar->GetCarStr_439F80();
        }
        else
        {
            pCarOrPedStr = L"ped";
        }
        swprintf(tmpBuff_67BD9C,
                 L"%s at (%3.1f, %3.1f, %3.1f) %s",
                 pCarOrPedStr,
                 pPed->get_cam_x().AsDouble(),
                 pPed->get_cam_y().AsDouble(),
                 pPed->get_cam_z().AsDouble(),
                 pZoneName);

        Garox_C4* pC4 = gHud_2B00_706620->field_650.sub_5D1F50(tmpBuff_67BD9C, -1, 16, word_7064B8, 1);
        pC4->field_B0_drawKind = 8;
        pC4->field_B4 = 0;
    }
}

// ----------------------------------------------------

// https://decomp.me/scratch/bd2MO
STUB_FUNC(0x5cfa70)
void Garox_107C_sub::DrawGangRespectBars_5CFA70()
{
    u32 random_num = rng_dword_67AB34->field_0_rng & 0xF;
    u8 PlayerIdx = gGame_0x40_67E008->field_38_orf1->field_2E_idx;
    bool bPlusSignDark = random_num > 7u;

    s32 ypos = 11;

    for (Gang_144* pGang = gGangPool_CA8_67E274->sub_4BECA0(); pGang; pGang = gGangPool_CA8_67E274->sub_4BECE0(), ypos += 27)
    {
        s8 respect = pGang->GetRespectForPlayer_4BEEF0(PlayerIdx);

        s32 arrow_colour = pGang->field_138_arrow_colour - 1;
        sub_5D7670(6, arrow_colour + 64, 16, ypos + 1, word_706610, 2, 0, 0, 0);

        sub_5D7670(6, arrow_colour + 78, 64, ypos + 1, word_706610, 2, 0, 0, 0);

        sub_5D7670(6, arrow_colour + 71, 64, ypos + 1, word_706610, 2, 0, 0, 0);

        // Draw positive respect
        s32 curr_bar_respect = 20;
        for (s32 i = 69; i <= 84 && respect >= curr_bar_respect; i += 5)
        {
            sub_5D7670(6, arrow_colour + 71, i, ypos + 1, word_706610, 2, 0, 0, 0);
            curr_bar_respect += 20;
        }

        // Draw negative respect
        curr_bar_respect = -20;
        for (s32 j = 59; j >= 44 && respect <= curr_bar_respect; j -= 5)
        {
            sub_5D7670(6, arrow_colour + 71, j, ypos + 1, word_706610, 2, 0, 0, 0);
            curr_bar_respect -= 20;
        }

        if (respect < -19)
        {
            if (respect <= -100 && !bPlusSignDark)
            {
                sub_5D7670(6, 2 * arrow_colour + 50, 34, ypos + 1, word_706610, 2, 0, 0, 0);
            }
        }
        else
        {
            if (respect >= 100 && !bPlusSignDark)
            {
                sub_5D7670(6, 2 * arrow_colour + 51, 93, ypos + 1, word_706610, 2, 0, 0, 0);
            }
        }

        // green mission respect
        if (respect >= -19)
        {
            sub_5D7670(6, 46, 64, ypos + 8, word_706610, 2, 0, 0, 0);
        }

        // yellow mission respect
        if (respect >= 40)
        {
            sub_5D7670(6, 47, 74, ypos + 8, word_706610, 2, 0, 0, 0);
        }

        // red mission respect
        if (respect >= 80)
        {
            sub_5D7670(6, 48, 84, ypos + 8, word_706610, 2, 0, 0, 0);
        }

        // debug stuff
        if (bDo_show_instruments_67D64C)
        {
            s32 v32 = (respect >= 0) + 5;
            swprintf(tmpBuff_67BD9C, L"%d", respect);
            DrawText_5D7720(tmpBuff_67BD9C, 64, ypos - 7, word_706600, 8, v32, 0, 0);
        }
    }
}

MATCH_FUNC(0x5cfe20)
void Garox_107C_sub::sub_5CFE20()
{
}

MATCH_FUNC(0x5cfe30)
void Garox_107C_sub::Empty_5CFE30()
{
}

// ----------------------------------------------------

MATCH_FUNC(0x5d0050)
void Hud_CopHead_C::UpdateHead_5D0050(bool bShakeHead)
{
    if (!bShakeHead)
    {
        field_4_height = 0;
    }

    field_1--;

    if (field_1 == 0)
    {
        field_1 = field_2;
        field_0 = field_0 == 0;
        if (bShakeHead)
        {
            field_4_height += field_8_velocity;
            if (field_4_height == -4)
            {
                field_8_velocity = 1;
            }
            else if (field_4_height == 4)
            {
                field_8_velocity = -1;
            }
        }
    }
}

MATCH_FUNC(0x5d7510)
Hud_CopHead_C::Hud_CopHead_C()
{
    field_0 = 0;
    field_2 = 0;
    field_1 = 0;
    field_4_height = 0;
    field_8_velocity = -1;
}

// ----------------------------------------------------

MATCH_FUNC(0x5d00b0)
void Hud_CopHead_C_Array::UpdateWantedLevel_5D00B0()
{
    Ped* pPed = gGame_0x40_67E008->field_38_orf1->field_2C4_player_ped;
    field_48_cop_level = pPed->get_wanted_star_count_46EF00();

    const bool bShakeHead = gPolice_7B8_6FEE40->HasCriminalBeenFound_56F800(pPed);
    s32 i = 0;
    Hud_CopHead_C* pIter = &field_1028[0];
    while (i < field_48_cop_level)
    {
        pIter->UpdateHead_5D0050(bShakeHead);
        i++;
        pIter++;
    }
}

STUB_FUNC(0x5d0110)
void Hud_CopHead_C_Array::DrawWantedLevel_5D0110()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x5d0210)
void Hud_CopHead_C_Array::sub_5D0210()
{
    u16 converted_pal = gGtx_0x106C_703DD4->convert_sprite_pal_5AA460(6, 14);
    sprite_index* sprite_index = gGtx_0x106C_703DD4->get_sprite_index_5AA440(converted_pal);

    field_4C_w_fp.FromU8(sprite_index->field_4_width);
    field_50_h_fp.FromU8(sprite_index->field_5_height);

    for (s32 i = 0; i < GTA2_COUNTOF(field_1028); i++)
    {
        field_1028[i].field_2 = 2;
        field_1028[i].field_1 = 2;
    }
}

// ----------------------------------------------------

STUB_FUNC(0x5d1b10)
void Garox_C4::sub_5D1B10(const wchar_t* pStr, s16 a3, s16 a4, s16 a5, s32 displayTime)
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x5d1d00)
void Garox_C4::sub_5D1D00()
{
    DrawText_5D7720(field_0_str_buf,
                    field_A8_x,
                    field_AA_y,
                    field_AC_fontType,
                    field_B0_drawKind,
                    field_B4,
                    field_B8_alpha,
                    field_BC_alpha_flag);
}

MATCH_FUNC(0x5d1db0)
bool Garox_C4::sub_5D1DB0()
{
    if (field_A4_display_time != -3)
    {
        field_A4_display_time--;
        if (field_A4_display_time < 0)
        {
            return true;
        }
    }
    return false;
}

MATCH_FUNC(0x5d1e10)
bool Garox_C4::operator_equals_5D1E10(Garox_C4* pOther)
{
    return field_A4_display_time > 0 && pOther != this && field_A8_x == pOther->field_A8_x && field_AA_y == pOther->field_AA_y &&
        field_AC_fontType == pOther->field_AC_fontType && !wcscmp(field_0_str_buf, pOther->field_0_str_buf);
}

// ----------------------------------------------------

MATCH_FUNC(0x5d1eb0)
void Garox_1700_L::sub_5D1EB0(Garox_C4* String2)
{
    Garox_C4* pIter = field_960_pFirst;
    while (pIter)
    {
        if (pIter->operator_equals_5D1E10(String2))
        {
            pIter->field_A4_display_time = 0;
            return;
        }
        pIter = pIter->field_C0_pNext;
    }
}

MATCH_FUNC(0x5d1f50)
Garox_C4* Garox_1700_L::sub_5D1F50(const wchar_t* pStr, s16 maybe_x, s16 maybe_y, s16 a5, s32 a6)
{
    Garox_C4* pOld_964 = field_964;
    Garox_C4* pOldFirst = field_960_pFirst;
    field_964 = pOld_964->field_C0_pNext;
    pOld_964->field_C0_pNext = pOldFirst;
    field_960_pFirst = pOld_964;
    pOld_964->sub_5D1B10(pStr, maybe_x, maybe_y, a5, a6);
    sub_5D1EB0(pOld_964);
    return pOld_964;
}

MATCH_FUNC(0x5d2010)
void Garox_1700_L::Service_5D2010()
{
    Garox_C4* pIter = field_960_pFirst;
    while (pIter)
    {
        pIter->sub_5D1D00();
        pIter = pIter->field_C0_pNext;
    }
}

MATCH_FUNC(0x5d2050)
void Garox_1700_L::sub_5D2050()
{
    Garox_C4* pAltIter = 0;
    Garox_C4* pIter = field_960_pFirst;
    while (pIter)
    {
        if (pIter->sub_5D1DB0())
        {
            if (pAltIter)
            {
                pAltIter->field_C0_pNext = pIter->field_C0_pNext;
                pIter->field_C0_pNext = field_964;
                field_964 = pIter;
                pIter = pAltIter->field_C0_pNext;
            }
            else
            {
                Garox_C4* pOld_field_964 = field_964;
                field_960_pFirst = field_960_pFirst->field_C0_pNext;
                pIter->field_C0_pNext = pOld_field_964;
                field_964 = pIter;
                pIter = field_960_pFirst;
            }
        }
        else
        {
            pAltIter = pIter;
            pIter = pIter->field_C0_pNext;
        }
    }
}

MATCH_FUNC(0x5d2280)
Garox_1700_L::Garox_1700_L()
{
    Garox_C4* pIter = &field_0_29_ary[0];
    for (s32 i = 0; i < 30 - 1; i++)
    {
        pIter->field_C0_pNext = pIter + 1;
        pIter++;
    }

    field_964 = &field_0_29_ary[0];
    field_0_29_ary[30 - 1].field_C0_pNext = NULL;
    field_960_pFirst = 0;
}

// ----------------------------------------------------

MATCH_FUNC(0x4be650)
Hud_Pager_C::~Hud_Pager_C()
{
    field_0_timer = -1;
    field_4 = 0;

    if (field_8_sound != NULL)
    {
        gRoot_sound_66B038.DestroySoundObj_40FE60(field_8_sound);
        field_8_sound = NULL;
    }
}

MATCH_FUNC(0x5d2320)
void Hud_Pager_C::Service_5D2320()
{
    if (field_0_timer < 0)
    {
        return;
    }

    field_0_timer--;
    if (field_0_timer == -1)
    {
        field_4 = 0;
    }
}

STUB_FUNC(0x5d2380)
void Hud_Pager_C::sub_5D2380(s32 a2, s32 a3)
{
    NOT_IMPLEMENTED;
}

STUB_FUNC(0x5d2680)
s32 Hud_Pager_C::sub_5D2680(s32 a2, s32 a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5d2ab0)
s32 Hud_Pager_C::DrawPager_5D2AB0(s32 a2, s32 a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x5d3040)
void Hud_Pager_C_Array::DrawPagers_5D3040()
{
    s32 totalSpriteHeight = get_sprite_height_4C7250(117) + get_sprite_height_4C7250(118) + get_sprite_height_4C7250(119);
    s32 width = (get_sprite_width_4C7220(117) / 2) + 3;

    s32 v9 = gLucid_hamilton_67E8E0.sub_4C59A0() ? 36 : 104;
    for (s32 i = 0; i < GTA2_COUNTOF(field_0_pagers_array); i++)
    {
        field_0_pagers_array[i].DrawPager_5D2AB0(width, v9);
        v9 += totalSpriteHeight;
    }
}

MATCH_FUNC(0x5d31b0)
void Hud_Pager_C_Array::UpdatePagers_5D31B0()
{
    for (s32 i = 0; i < GTA2_COUNTOF(field_0_pagers_array); i++)
    {
        field_0_pagers_array[i].Service_5D2320();
    }
}

MATCH_FUNC(0x5d31f0)
s32 Hud_Pager_C_Array::CreateTimer_5D31F0(s32 seconds) // returns the new Pager id
{
    for (s32 id = 0; id < GTA2_COUNTOF_S(field_0_pagers_array); id++)
    {
        Hud_Pager_C* pPager = &field_0_pagers_array[id];
        if (pPager->field_0_timer >= 0 || pPager->field_4)
        {
            continue;
        }
        pPager->field_0_timer = 30 * seconds;
        return id;
    }
    return -1;
}

MATCH_FUNC(0x5d3220)
s32 Hud_Pager_C_Array::sub_5D3220(s32* a2)
{
    s32 targetIdx = -1;
    for (s32 i = 0; i < GTA2_COUNTOF_S(field_0_pagers_array); i++)
    {
        Hud_Pager_C* pPager = &field_0_pagers_array[i];
        if (!pPager->field_4 && (pPager->field_0_timer >= 0 || targetIdx == -1))
        {
            targetIdx = i;
        }
    }

    Hud_Pager_C* pTargetPager = &field_0_pagers_array[targetIdx];
    infallible_turing* pSound = pTargetPager->field_8_sound;
    pTargetPager->field_4 = a2;
    if (!pSound && !bSkip_audio_67D6BE)
    {
        pTargetPager->field_8_sound = gRoot_sound_66B038.CreateSoundObject_40EF40(pTargetPager, SoundObjectTypeEnum::Hud_Pager_C_11);
    }

    return targetIdx;
}

MATCH_FUNC(0x5d3280)
void Hud_Pager_C_Array::sub_5D3280(s32 idx)
{
    infallible_turing* pSound = field_0_pagers_array[idx].field_8_sound;
    Hud_Pager_C* pPager = &field_0_pagers_array[idx];
    pPager->field_0_timer = -1;
    pPager->field_4 = 0;

    if (pSound)
    {
        pSound->release_40EF20();
        pSound->field_C_pAny.pInfallible_turing = gRoot_sound_66B038.field_0;
        gRoot_sound_66B038.field_0 = pSound;
        pPager->field_8_sound = 0;
    }
}

MATCH_FUNC(0x5d32d0)
void Hud_Pager_C_Array::sub_5D32D0(s32 a2)
{
    field_0_pagers_array[a2].field_0_timer = -1;
}

MATCH_FUNC(0x5d32f0)
void Hud_Pager_C_Array::AddTime_5D32F0(s32 pager_idx, s32 time_to_add)
{
    Hud_Pager_C* pPager = &field_0_pagers_array[pager_idx];
    pPager->field_0_timer += time_to_add;
}

MATCH_FUNC(0x5d3310)
void Hud_Pager_C_Array::sub_5D3310(s32 a2)
{
    field_0_pagers_array[a2].field_4 = 0;
}

MATCH_FUNC(0x5d7650)
Hud_Pager_C::Hud_Pager_C()
{
    field_0_timer = -1;
    field_4 = 0;
    field_8_sound = NULL;
}

MATCH_FUNC(0x5d03c0)
void ArrowTrace_24::PointToInfoPhone_5D03C0(Gang_144* pZone)
{
    set_arrow_aim_from_pos_4767C0(pZone->field_12C_info_phone_x, pZone->field_130_info_phone_y, pZone->field_134_info_phone_z);
    field_10_type = 5;
}

MATCH_FUNC(0x5D03F0)
void ArrowTrace_24::sub_5D03F0()
{
    Ped* pPed;
    Player* pPlayer;
    Car_BC* pCar;
    Sprite* pSprite;
    Camera_0xBC* pCam;

    switch (field_10_type)
    {
        case 0:
            return;
        case 6:
            pPlayer = field_C;
            if (pPlayer->field_8E_bInUse)
            {
                field_14_aim_x = pPlayer->field_2C4_player_ped->get_cam_x();
                field_18_aim_y = pPlayer->field_2C4_player_ped->get_cam_y();
                field_1C_aim_z = pPlayer->field_2C4_player_ped->get_cam_z();
            }
            else
            {
                field_10_type = 0;
            }
            break;
        case 2:
            pPed = field_0;
            if (pPed->field_21C_bf.b0)
            {
                field_14_aim_x = pPed->get_cam_x();
                field_18_aim_y = pPed->get_cam_y();
                field_1C_aim_z = pPed->get_cam_z();
            }
            else
            {
                field_10_type = 0;
            }
            break;
        case 3:
            pCar = field_4;
            if (pCar->field_88 == 5)
            {
                field_10_type = 0;
            }
            else
            {
                pSprite = pCar->field_50_car_sprite;
                field_14_aim_x = pSprite->field_14_xpos.x;
                field_18_aim_y = pSprite->field_14_xpos.y;
                field_1C_aim_z = pSprite->field_1C_zpos;
            }
            break;
        case 4:
            if (!field_8->sub_529200())
            {
                gHud_2B00_706620->field_1F18.field_844 = 1;
                field_10_type = 0;
            }
            else
            {
                pSprite = field_8->field_4;
                field_14_aim_x = pSprite->field_14_xpos.x;
                field_18_aim_y = pSprite->field_14_xpos.y;
                field_1C_aim_z = pSprite->field_1C_zpos;
            }
            break;
        default:
            break;
    }

    Player* field_38_orf1 = gGame_0x40_67E008->field_38_orf1;
    s32 v8 = field_38_orf1->field_68;

    if (v8 == 2 || v8 == 3)
    {
        pCam = &field_38_orf1->field_208_aux_game_camera;
    }
    else
    {
        pCam = &field_38_orf1->field_90_game_camera;
    }

    field_20 = pCam->sub_435A70(field_14_aim_x, field_18_aim_y, field_1C_aim_z);
}

MATCH_FUNC(0x5d0510)
void Hud_Arrow_7C::SetArrowColour_5D0510(s32 a2)
{
    field_18.field_28_arrow_colour = a2;
}

MATCH_FUNC(0x5d0530)
bool Hud_Arrow_7C::sub_5D0530()
{
    Gang_144* field_30 = field_18.field_10.field_30;

    if (field_30)
    {
        if (!gHud_2B00_706620->field_1F18.field_83C)
        {
            return false;
        }
        if (!bShow_all_arrows_67D6E7)
        {
            s32* pMission_flag = gfrosty_pasteur_6F8060->field_344_mission_flag;
            if (pMission_flag && *pMission_flag)
            {
                return false;
            }
            Player* pPlayer = gGame_0x40_67E008->field_38_orf1;
            Gang_144* pZone144 = pPlayer->field_34_gang_curr_location;
            if (pZone144)
            {
                if (pPlayer->field_40_arrow_blocker_zone)
                {
                    pZone144 = 0;
                }
            }
            if (field_18.field_60->field_10_type == 5)
            {
                if (pZone144 == field_30)
                {
                    if (gHud_2B00_706620->field_1F18.sub_5D0E40(this))
                    {
                        return false;
                    }
                }
                else if (pZone144 && gHud_2B00_706620->field_1F18.sub_5D0F40(pZone144))
                {
                    return false;
                }
            }
            else
            {
                if (pZone144 != field_30)
                {
                    return false;
                }
                u8 player_idx = pPlayer->field_2E_idx;
                if (field_30->GetRespectForPlayer_4BEEF0(player_idx) < field_18.field_10.field_34)
                {
                    return false;
                }
                Hud_Arrow_7C* v7 = gHud_2B00_706620->field_1F18.field_840;
                if (v7 && v7 != this)
                {
                    return false;
                }
            }
        }
    }
    return true;
}

STUB_FUNC(0x5d0620)
char_type Hud_Arrow_7C::sub_5D0620()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5d0850)
s32 Hud_Arrow_7C::sub_5D0850()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x5d0c60)
void Hud_Arrow_7C::Service_5D0C60()
{
    if (!sub_5D0620())
    {
        char_type v2 = sub_5D0530();
        field_18.field_10.field_5 = v2;
        if (v2)
        {
            sub_5D0850();
        }
    }
}

MATCH_FUNC(0x5d0c90)
void Hud_Arrow_7C::DrawArrow_5D0C90()
{
    // TODO: Kinda messy, refactor this
    Player* pPlayer;
    Camera_0xBC* pCam;
    s32 drawKind_;

    if (this->field_18.field_18.field_10_type || this->field_18.field_3C.field_10_type)
    {
        if (this->field_18.field_10.field_5)
        {
            if (this->field_18.field_28_arrow_colour != 5 || ((u32)rng_dword_67AB34->field_0_rng % 6 >= 3))
            {
                drawKind_ = 2;
            }
            else
            {
                drawKind_ = 7;
            }
            pPlayer = gGame_0x40_67E008->field_38_orf1;
            if (pPlayer->field_68 == 2 || pPlayer->field_68 == 3)
            {
                pCam = &pPlayer->field_208_aux_game_camera;
            }
            else
            {
                pCam = &pPlayer->field_90_game_camera;
            }

            DrawFigure_5D7EC0(6,
                              field_18.field_2C,
                              field_0_screen_pos_x,
                              field_4_screen_pos_y,
                              field_8_rotation,
                              pCam->field_A8_ui_scale,
                              drawKind_,
                              0,
                              1,
                              14u,
                              1);

            s16 colour_related;
            switch (this->field_18.field_28_arrow_colour)
            {
                case 1:
                    colour_related = 0;
                    break;
                case 3:
                    colour_related = 1;
                    break;
                case 2:
                    colour_related = 2;
                    break;

                case 4:
                case 5:
                    return;
                default:
                    // BUG: colour_related - un-inited?
                    break;
            }

            const s32 drawKind = 2;
            pPlayer = gGame_0x40_67E008->field_38_orf1;
            if (pPlayer->field_68 == 2 || pPlayer->field_68 == 3)
            {
                pCam = &pPlayer->field_208_aux_game_camera;
            }
            else
            {
                pCam = &pPlayer->field_90_game_camera;
            }

            DrawFigure_5D7EC0(6,
                              colour_related + 8,
                              field_0_screen_pos_x,
                              field_4_screen_pos_y,
                              field_8_rotation,
                              pCam->field_A8_ui_scale,
                              drawKind,
                              0,
                              1,
                              14u,
                              1);
        }
    }
}

MATCH_FUNC(0x5d0dc0)
void Hud_Arrow_7C::sub_5D0DC0(Ped* a2)
{
    switch (a2->field_244_remap)
    {
        case 11:
            field_18.field_2C = 1;
            break;
        case 13:
            field_18.field_2C = 2;
            break;
        case 8:
            field_18.field_2C = 3;
            break;
        case 5:
        case 6:
            field_18.field_2C = 4;
            break;
        case 7:
            field_18.field_2C = 5;
            break;
        case 9:
            field_18.field_2C = 6;
            break;
        case 10:
            field_18.field_2C = 7;
            break;

        default:
            return;
    }
}

MATCH_FUNC(0x5d1350)
void Hud_Arrow_7C_Array::sub_5D1350()
{
    if ((u8)bStartNetworkGame_7081F0)
    {
        if (gLucid_hamilton_67E8E0.sub_4C5BC0() != 3)
        {
            sub_5D10B0();
            for (Player* pPlayerIter = gGame_0x40_67E008->sub_4B9CD0(); pPlayerIter;
                 pPlayerIter = gGame_0x40_67E008->IterateNextPlayer_4B9D10())
            {
                if (!pPlayerIter->field_0_bIsUser)
                {
                    Hud_Arrow_7C* p7C = AllocArrow_5D1050();
                    p7C->field_18.field_18.field_C = pPlayerIter;
                    p7C->field_18.field_18.field_10_type = 6;
                    Ped* pPlayerPed = pPlayerIter->field_2C4_player_ped;
                    if (pPlayerPed)
                    {
                        p7C->sub_5D0DC0(pPlayerPed);
                    }
                }
            }
        }
    }
}

MATCH_FUNC(0x5d7600)
Hud_Arrow_7C::Hud_Arrow_7C()
{
    field_18.field_10.field_30 = 0;
    field_18.field_10.field_34 = 0;
    field_18.field_10.field_5 = 0;

    field_18.field_18.init();
    field_18.field_3C.init();

    field_18.field_60 = &field_18.field_18;
    field_18.field_2E = 0;
    field_18.field_10.field_6 = 0;
}

// ----------------------------------------------------

MATCH_FUNC(0x5d0e40)
bool Hud_Arrow_7C_Array::sub_5D0E40(Hud_Arrow_7C* a2)
{
    Gang_144* pGang = a2->field_18.field_10.field_30;

    for (s32 i = 0; i < GTA2_COUNTOF_S(field_0_array); i++)
    {
        Hud_Arrow_7C* pArrow = &field_0_array[i];
        if (pArrow != a2 && !pArrow->sub_4C6F80() && pArrow->sub_4C7050() && pArrow->field_18.field_10.field_30 == pGang)
        {
            return true;
        }
    }
    return false;
}

MATCH_FUNC(0x5d0e90)
void Hud_Arrow_7C_Array::DrawArrows_5D0E90()
{
    if ((u8)bStartNetworkGame_7081F0)
    {
        // Limit drawn arrows in multiplayer
        for (s32 i = 0; i < GTA2_COUNTOF_S(field_0_array); i++)
        {
            if (field_0_array[i].field_18.field_18.field_C == NULL ||
                field_0_array[i].field_18.field_18.field_C->field_2C4_player_ped == NULL ||
                field_0_array[i].field_18.field_18.field_C->field_2C4_player_ped->field_21C_bf.b25 == 0)
            {
                field_0_array[i].DrawArrow_5D0C90();
            }
        }
    }
    else
    {
        for (s32 i = 0; i < 17; i++)
        {
            field_0_array[i].DrawArrow_5D0C90();
        }
    }
}

STUB_FUNC(0x5d0ef0)
Hud_Arrow_7C* Hud_Arrow_7C_Array::sub_5D0EF0()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x5d0f40)
char_type Hud_Arrow_7C_Array::sub_5D0F40(Gang_144* a2)
{
    Hud_Arrow_7C* pIter = &field_0_array[0];
    for (s32 i = 0; i < GTA2_COUNTOF_S(field_0_array); i++, pIter++)
    {
        if ((pIter->field_18.field_18.field_10_type || pIter->field_18.field_3C.field_10_type) &&
            (pIter->field_18.field_10.field_30 == a2 && pIter->field_18.field_60->field_10_type != 5))
        {
            return 1;
        }
    }
    return 0;
}

MATCH_FUNC(0x5d0f80)
void Hud_Arrow_7C_Array::sub_5D0F80()
{
    for (s32 i = 0; i < GTA2_COUNTOF_S(field_0_array); i++)
    {
        if (field_0_array[i].field_18.field_18.field_10_type || field_0_array[i].field_18.field_3C.field_10_type)
        {
            if (field_0_array[i].field_18.field_10.field_30)
            {
                if (field_0_array[i].field_18.field_60->field_10_type == 5 && !sub_5D0F40(field_0_array[i].field_18.field_10.field_30))
                {
                    field_0_array[i].field_18.field_18.field_10_type = 0;
                    field_0_array[i].field_18.field_3C.field_10_type = 0;
                }
            }
        }
    }
}

MATCH_FUNC(0x5d0fd0)
void Hud_Arrow_7C_Array::UpdateArrows_5D0FD0()
{
    sub_5D0EF0();

    for (s32 i = 0; i < GTA2_COUNTOF(field_0_array); i++)
    {
        if (field_0_array[i].field_18.field_18.field_10_type || field_0_array[i].field_18.field_3C.field_10_type)
        {
            field_0_array[i].Service_5D0C60();
        }
    }

    if (field_844)
    {
        sub_5D0F80();
        field_844 = 0;
    }
}

MATCH_FUNC(0x5d1020)
Hud_Arrow_7C* Hud_Arrow_7C_Array::sub_5D1020(s32* a2)
{
    Hud_Arrow_7C* pIter = &field_0_array[0];
    for (s32 i = 0; i < GTA2_COUNTOF_S(field_0_array); i++, pIter++)
    {
        if (!pIter->field_18.field_10.field_6)
        {
            *a2 = i;
            pIter->field_18.field_10.field_6 = 1;
            return pIter;
        }
    }
    return 0;
}

MATCH_FUNC(0x5d1050)
Hud_Arrow_7C* Hud_Arrow_7C_Array::AllocArrow_5D1050()
{
    s32 idx;
    Hud_Arrow_7C* pRet = sub_5D1020(&idx);
    pRet->field_10_radius_pos = dword_7064C0;
    pRet->field_14_reposition_speed = dword_7063B0;
    pRet->SetArrowColour_5D0510(4);
    pRet->field_18.field_10.field_5 = 1;
    pRet->field_18.field_2C = 0;
    pRet->field_18.field_10.field_30 = 0;
    pRet->field_18.field_10.field_34 = 0;
    pRet->field_C_min_radius_pos = dword_7065B4 + dword_706338 * (16 - idx);
    return pRet;
}

MATCH_FUNC(0x5d10b0)
void Hud_Arrow_7C_Array::sub_5D10B0()
{
    for (s32 i = 0; i < GTA2_COUNTOF_S(field_0_array); i++)
    {
        if (field_0_array[i].field_18.field_10.field_6)
        {
            field_0_array[i].field_18.field_18.field_10_type = 0;
            field_0_array[i].field_18.field_3C.field_10_type = 0;

            field_0_array[i].field_18.field_10.field_6 = 0;
        }
    }
}

MATCH_FUNC(0x5d10d0)
Hud_Arrow_7C* Hud_Arrow_7C_Array::sub_5D10D0(Gang_144* pZone, s32 phone_type)
{
    s32 i = 0;
    Hud_Arrow_7C* pIter = field_0_array;
    while (i < GTA2_COUNTOF_S(field_0_array))
    {
        if ((pIter->field_18.field_18.field_10_type || pIter->field_18.field_3C.field_10_type) &&
            (pIter->field_18.field_10.field_30 == pZone && pIter->field_18.field_28_arrow_colour == phone_type))
        {
            return pIter;
        }
        i++;
        pIter++;
    }
    return 0;
}

MATCH_FUNC(0x5d1110)
void Hud_Arrow_7C_Array::place_gang_phone_5D1110(Object_2C* pPhoneInfo)
{
    s32 phone_type = sub_5D1260(pPhoneInfo->field_18_model);
    Gang_144* pZone = gMap_0x370_6F6268->GetGangAtCoords_4DFB50(pPhoneInfo->field_4->GetXPos(), pPhoneInfo->field_4->GetYPos());

    if (!pZone)
    {
        phone_x_67CD14 = pPhoneInfo->field_4->GetXPos();
        phone_y_67CD0C = pPhoneInfo->field_4->GetYPos();
        dword_67CD10 = 0;
        FatalError_4A38C0(Gta2Error::PlacingGangPhoneWhenNoGang, // Placing gang phone when no gang at: (%.4f, %.4f)
                          "C:\\Splitting\\Gta2\\Source\\user.cpp",
                          1509,
                          0,
                          &phone_x_67CD14,
                          &phone_y_67CD0C,
                          &dword_67CD10);
    }
    Hud_Arrow_7C* v6 = Hud_Arrow_7C_Array::sub_5D10D0(pZone, phone_type);
    if (v6)
    {
        if (v6->field_18.field_3C.field_10_type)
        {
            strcpy(gErrStr_67C29C, get_phone_colour_5D12B0(phone_type));
            strcpy(byte_67C3A8, pZone->field_2_name);
            FatalError_4A38C0(Gta2Error::TooManyPhonesForGang, // Too many %s phones for %s gang
                              "C:\\Splitting\\Gta2\\Source\\user.cpp",
                              1513,
                              gErrStr_67C29C,
                              byte_67C3A8);
        }
        v6->field_18.field_3C.field_8 = pPhoneInfo;
        v6->field_18.field_3C.field_10_type = 4;
    }
    else
    {
        Hud_Arrow_7C* v7 = Hud_Arrow_7C_Array::AllocArrow_5D1050();
        v7->field_18.field_18.field_8 = pPhoneInfo;
        v7->field_18.field_18.field_10_type = 4;
        v7->SetArrowColour_5D0510(phone_type);
        v7->field_18.field_10.field_30 = pZone;
        v7->field_18.field_10.field_34 = sub_5D12E0(phone_type);
        v7->field_18.field_2C = pZone->field_138_arrow_colour;
    }
}

MATCH_FUNC(0x5D1260)
s32 __stdcall sub_5D1260(s32 a1)
{
    switch (a1)
    {
        case 176:
        case 177:
            return 2;
        case 180:
        case 181:
            return 3;
        case 178:
        case 179:
            return 1;
        default:
            return 0;
    }
}

MATCH_FUNC(0x5D12B0)
char_type* __stdcall get_phone_colour_5D12B0(s32 phone_type)
{
    switch (phone_type)
    {
        case 1:
            return "yellow";
        case 2:
            return "red";
        case 3:
            return "green";
    }
    return "unknown";
}

MATCH_FUNC(0x5D12E0)
u8 __stdcall sub_5D12E0(s32 phone_type)
{
    switch (phone_type)
    {
        case 1:
            return 40;
        case 2:
            return 80;
        case 3:
            return 237;
    }
    return 0;
}

MATCH_FUNC(0x5d1310)
void Hud_Arrow_7C_Array::SetNewGangArrow_5D1310(Gang_144* pZone)
{
    Hud_Arrow_7C* p7C = AllocArrow_5D1050();
    p7C->SetArrowColour_5D0510(4);
    p7C->field_18.field_10.field_30 = pZone;
    p7C->field_18.field_2C = pZone->field_138_arrow_colour;
    p7C->field_18.field_10.field_34 = 0;
    p7C->field_18.field_18.PointToInfoPhone_5D03C0(pZone);
}

// ----------------------------------------------------

MATCH_FUNC(0x5d3330)
void Garox_1E34_L::sub_5D3330()
{
    Garox_18* pGarox_18 = field_700;
    field_700 = pGarox_18->field_C;
    pGarox_18->field_C = field_6F8_prev_brief;
    field_6F8_prev_brief = pGarox_18;
}

STUB_FUNC(0x5d3350)
char_type* Garox_1E34_L::sub_5D3350()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5d3370)
s32 Garox_1E34_L::sub_5D3370()
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x5d33a0)
void Garox_1E34_L::sub_5D33A0()
{
    Garox_18* pBrief;
    for (pBrief = field_700; pBrief->field_C; pBrief = pBrief->field_C)
    {
        ;
    }
    pBrief->field_C = field_6F8_prev_brief;
    field_6F8_prev_brief->field_8_brief_priority = 0;
    field_6F8_prev_brief = field_6F8_prev_brief->field_C;
    pBrief->field_C->field_C = 0;
}

STUB_FUNC(0x5d33f0)
s32 Garox_1E34_L::sub_5D33F0()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5d3470)
size_t Garox_1E34_L::sub_5D3470()
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5d3680)
char_type Garox_1E34_L::sub_5D3680(s16 a1)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x5d39d0)
void Garox_1E34_L::sub_5D39D0()
{
    field_510_time_to_show = Garox_1E34_L::sub_5D3470();
    field_504_tick_timer = field_510_time_to_show * gHud_2B00_706620->field_13C4_text_speed;
    field_50C = 0;
    field_514_upward_timer = 0;
    field_6F8_prev_brief->field_10 = 0;
}

// https://decomp.me/scratch/exFU8
STUB_FUNC(0x5d3b80)
void Garox_1E34_L::DrawBrief_5D3B80()
{
    NOT_IMPLEMENTED;

    if (this->field_6F8_prev_brief)
    {
        sub_5D7670(6, // type
                   field_50C + 3 * field_502_face_idx + 16, // LOBYTE(v1->field_502_face_idx) pal
                   (32), // x
                   (443), // y
                   word_706610, // rot
                   2, // drawkind
                   0,
                   0,
                   0);

        s32 t = 480 - (u16)gGtx_0x106C_703DD4->sub_5AA800(&word_7065C4) * field_508_num_lines;
        DrawText_5D7720(field_0_str, // str
                        (64), // x
                        t, // y
                        word_7065C4, // fontType
                        2,
                        0, // a6
                        0, // alpha
                        0); // alpha_flag
    }
}

STUB_FUNC(0x5d3f10)
s32 Garox_1E34_L::sub_5D3F10(s32 a2, const char_type* a3, s32 a4)
{
    NOT_IMPLEMENTED;
    return 0;
}

STUB_FUNC(0x5d4400)
s32 Garox_1E34_L::sub_5D4400(s32 a2, const char_type* a3)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x5d44d0)
void Garox_1E34_L::sub_5D44D0()
{
    if (field_6F8_prev_brief)
    {
        field_504_tick_timer--;

        if (!(field_504_tick_timer % 3))
        {
            field_514_upward_timer++;
            if (field_514_upward_timer == field_510_time_to_show)
            {
                field_514_upward_timer = 0;
            }

            const s32 ary_val = field_0_str[field_514_upward_timer];
            if (!(ary_val % 20))
            {
                field_50C = 1;
            }
            else
            {
                field_50C = 2 * (ary_val % 2);
            }
        }

        field_6F8_prev_brief->field_10 = 1;

        if (field_504_tick_timer == 0)
        {
            sub_5D3370();
            if (field_6F8_prev_brief)
            {
                sub_5D39D0();
            }
        }
    }
}

MATCH_FUNC(0x5d4850)
void Garox_1E34_L::ShowBrief_5D4850()
{
    if (field_700)
    {
        Garox_18* prev_brief = field_6F8_prev_brief;
        if (prev_brief)
        {
            if (prev_brief->field_10)
            {
                Garox_1E34_L::sub_5D33A0();
            }
        }
        Garox_1E34_L::sub_5D3330();
        Garox_1E34_L::sub_5D39D0();
    }
}

STUB_FUNC(0x5d4890)
s32 Garox_1E34_L::ClearAllBriefsWithPriority_5D4890(s32 a2)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x5d4930)
Garox_1E34_L::Garox_1E34_L()
{
    field_6FC_p_start_q = &field_518_ary_19_start_q;

    field_50C = 0;
    field_510_time_to_show = 0;
    field_514_upward_timer = 0;
    field_6F8_prev_brief = 0;
    field_700 = 0;
    field_504_tick_timer = 0;

    for (s32 i = 0; i < GTA2_COUNTOF(field_524_ary_19); i++)
    {
        field_524_ary_19[i].field_0 = &field_524_ary_19[i].field_C;
    }

    field_6EC = 0;
}

// ----------------------------------------------------

MATCH_FUNC(0x5d5900)
void Hud_MapZone_98::DrawZoneName_5D5900()
{
    if (field_0_timer)
    {
        sprite_index* sprite_index_5AA440 =
            gGtx_0x106C_703DD4->get_sprite_index_5AA440(gGtx_0x106C_703DD4->convert_sprite_pal_5AA460(6, 159));
        s32 width = sprite_index_5AA440->field_4_width;

        sub_5D7670(6, 159, (u32)(320 - (width / 2) - width), (u32)27, word_706610, 2, 0, field_90, field_94_transparency);

        sub_5D7670(6, 160, 320 - (width / 2), (u32)27, word_706610, 2, 0, field_90, field_94_transparency);

        sub_5D7670(6, 161, (width / 2) + 320, (u32)27, word_706610, 2, 0, field_90, field_94_transparency);

        sub_5D7670(6, 162, (u32)((width / 2) + width + 320), (u32)27, word_706610, 2, 0, field_90, field_94_transparency);

        DrawText_5D7720(field_2_wstr,
                        (640 - field_84) / 2,
                        27 - (sub_5D7700(word_706618) / 2),
                        word_706618,
                        2,
                        0,
                        field_90,
                        field_94_transparency);
    }
}

MATCH_FUNC(0x5d5ad0)
void Hud_MapZone_98::sub_5D5AD0()
{
    if (field_0_timer)
    {
        field_84 = Frontend::sub_5D8990(field_2_wstr, word_706618);
    }
}

MATCH_FUNC(0x4DEF00)
EXPORT wchar_t* gmp_map_zone::get_zone_str_4DEF00()
{
    char buf[4];
    if (field_5_name_length == 3)
    {
        buf[0] = field_6_name[0];
        buf[1] = field_6_name[1];
        buf[2] = field_6_name[2];
        buf[3] = 0;
        return gText_0x14_704DFC->Find_5B5F90(buf);
    }
    return 0;
}

MATCH_FUNC(0x5d5af0)
void Hud_MapZone_98::sub_5D5AF0(gmp_map_zone* pZone1, gmp_map_zone* pZone2)
{
    gmp_map_zone* pArg2Or3 = pZone2;
    if (!pZone2)
    {
        pArg2Or3 = pZone1;
    }

    wchar_t* pStr = pArg2Or3->get_zone_str_4DEF00();
    if (pStr)
    {
        const wchar_t* pName = gText_0x14_704DFC->sub_5B5B80(pStr);
        wcscpy(this->field_2_wstr, pName);
        this->field_88_nav_zone = pZone1;
        this->field_8C_local_nav_zone = pZone2;
        this->field_0_timer = 90;
        sub_5D5AD0();
        this->field_90 = 1;
        this->field_94_transparency = 0;
    }
}

MATCH_FUNC(0x5d5b60)
void Hud_MapZone_98::sub_5D5B60()
{
    u8 x;
    u8 y;
    u8 z;

    gGame_0x40_67E008->field_38_orf1->sub_569840(x, y, z);
    gmp_map_zone* navigation_zone = gMap_0x370_6F6268->zone_by_pos_and_type_4DF4D0(x, y, Navigation_1);
    gmp_map_zone* local_navigation_zone = gMap_0x370_6F6268->zone_by_pos_and_type_4DF4D0(x, y, Local_Navigation_15);

    if (navigation_zone || local_navigation_zone)
    {
        if (local_navigation_zone == field_8C_local_nav_zone && (local_navigation_zone || navigation_zone == field_88_nav_zone))
        {
            if (field_0_timer)
            {
                field_0_timer--;
                if (field_0_timer > 0x39u)
                {
                    field_94_transparency++;
                    if (field_94_transparency > 0x1Fu)
                    {
                        field_90 = 0;
                        field_94_transparency = 31;
                    }
                }
                else
                {
                    if (field_0_timer < 0x1Fu)
                    {
                        field_90 = 1;
                        if (field_94_transparency > 0)
                        {
                            field_94_transparency--;
                        }
                    }
                }
            }
        }
        else
        {
            Hud_MapZone_98::sub_5D5AF0(navigation_zone, local_navigation_zone);
        }
    }
    else
    {
        field_0_timer = 0;
        field_88_nav_zone = 0;
    }
}

MATCH_FUNC(0x5d5c50)
void Hud_MapZone_98::sub_5D5C50()
{
    field_90 = 0;
    field_94_transparency = 0;
}

MATCH_FUNC(0x5d5c60)
Hud_MapZone_98::Hud_MapZone_98()
{
    field_0_timer = 0;
    field_88_nav_zone = NULL;
    field_8C_local_nav_zone = NULL;
}

// ----------------------------------------------------

MATCH_FUNC(0x5d53b0)
Hud_CarName_4C::Hud_CarName_4C()
{
    field_0_display_time = 0;
}

// ----------------------------------------------------

MATCH_FUNC(0x4bbbb0)
Hud_2B00::~Hud_2B00()
{
}

STUB_FUNC(0x5d4a10)
void Hud_2B00::sub_5D4A10()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x5d5190)
void Hud_2B00::sub_5D5190()
{
    if (field_0.field_0_display_time)
    {
        // TODO: Structure seems wrong, probablty field_2 to field_4C of Garox_2B00 is a string buffer?
        field_0.field_44 = Frontend::sub_5D8990((wchar_t*)&field_0.field_2_car_name, word_706508);
    }
}

MATCH_FUNC(0x5d5240)
void Hud_2B00::sub_5D5240(wchar_t* Source)
{
    field_0.field_0_display_time = 120;
    wcscpy(field_0.field_2_car_name, Source);
    gText_0x14_704DFC->sub_5B5B80(field_0.field_2_car_name);
    Hud_2B00::sub_5D5190();
    field_0.field_48 = -17;
}

MATCH_FUNC(0x5d5350)
void Hud_2B00::sub_5D5350()
{
    Hud_CarName_4C* pCarName = &field_0;
    if (pCarName->field_0_display_time)
    {
        pCarName->field_0_display_time--;
        if (pCarName->field_0_display_time > 80u)
        {
            ++pCarName->field_48;
        }
        else if (pCarName->field_0_display_time < 40u)
        {
            --pCarName->field_48;
        }
    }
}

MATCH_FUNC(0x5d6860)
void Hud_2B00::DrawGui_5D6860()
{
    if (!bSkip_user_67D506)
    {
        sub_5D6A70();
        field_1118_sub.DrawPlayerStats_5D5C80();
        field_110C_sub.Draw_5CF910();
        field_13C0_sub.DrawPlayerNames_5CFE40();
        field_1028.DrawWantedLevel_5D0110();
        field_107C_sub.DrawGangRespectBars_5CFA70();
        field_1108_sub.DrawHealth_5D0260();
        field_4C.DrawZoneName_5D5900();
        sub_5D4A10();
        field_1080.sub_5D5420();
        field_DC.DrawBrief_5D3B80();
        field_620.DrawPagers_5D3040();
        field_650.Service_5D2010();
        field_1F18.DrawArrows_5D0E90();
        field_12F0.sub_5D56D0();
        field_111C.DrawMessage_5D1940();
        field_12E4_sub.DrawPause_5D63B0();
        field_2A25_sub.sub_5D16B0();
        field_12EC_sub.DrawQuitMessage_5D1430();
    }
}

STUB_FUNC(0x5d69c0)
void Hud_2B00::sub_5D69C0()
{
    NOT_IMPLEMENTED;
}

MATCH_FUNC(0x5d69d0)
void Hud_2B00::UpdateHUD_5D69D0()
{
    field_1118_sub.sub_5D6290();
    field_110C_sub.sub_5CF730();
    field_27B5_show_coords.ShowPlayerCoords_5CF970();
    field_1028.UpdateWantedLevel_5D00B0();
    sub_5D5350();
    field_1080.sub_5D5690();
    field_4C.sub_5D5B60();
    field_DC.sub_5D44D0();
    field_620.UpdatePagers_5D31B0();
    field_650.sub_5D2050();
    field_1F18.UpdateArrows_5D0FD0();
    field_107C_sub.sub_5CFE20();
    field_111C.sub_5D1AB0();
    field_12F0.sub_5D5760();
}

MATCH_FUNC(0x5d6a70)
void Hud_2B00::sub_5D6A70()
{
    if (gLighting_626A09)
    {
        pgbh_SetAmbient(1.0);
    }
}

MATCH_FUNC(0x5d6a90)
void Hud_2B00::sub_5D6A90()
{
    field_13C4_text_speed = gRegistry_6FF968.Set_Option_586F70("text_speed", 3);
}

MATCH_FUNC(0x5d6ab0)
void Hud_2B00::sub_5D6AB0()
{
    sub_5D6B00();
    field_DC.sub_5D3470();
    sub_5D5190();
    field_4C.sub_5D5AD0();
    field_111C.sub_5D1860();
    field_12F0.sub_5D56B0();
    field_1080.sub_5D53E0();
}

MATCH_FUNC(0x5d6b00)
void Hud_2B00::sub_5D6B00()
{
    if (gText_0x14_704DFC->field_10_lang_code == 106)
    {
        word_7064B8 = 103;
        word_706508 = 104;
        word_706618 = 105;
        word_7065C4 = 107;
        word_70646C = word_703BAA;
        word_7062F0 = 203;
        word_706600 = 103;
        word_7063F8 = 201;
        word_7064D8 = 103;
        word_7062DC = 103;
        word_70643E = 103;
    }
    else
    {
        word_706508 = word_703D98;
        word_7064B8 = word_703BAA;
        word_706618 = word_703C9C;
        word_7065C4 = word_703BAA;
        word_70646C = word_703BAA;
        word_7062F0 = word_703D9C;
        word_706600 = word_703BAA;
        word_7064D8 = word_703BAA;
        word_7063F8 = word_703DA4;
        word_7062DC = word_703BAA;
        word_70643E = word_703BAA;
    }
}

MATCH_FUNC(0x5d6be0)
void Hud_2B00::sub_5D6BE0()
{
    sub_5D6A90();
    sub_5D6B00();
    field_4C.sub_5D5C50();
    field_107C_sub.Empty_5CFE30();
    field_1028.sub_5D0210();
    field_1F18.sub_5D1350();
}

MATCH_FUNC(0x5d6c20)
bool Hud_2B00::IsBusy_5D6C20(s32 action, Player* pPlayer)
{
    return field_12EC_sub.IsOnQuitMessage_5D13C0(action, pPlayer) || field_2A25_sub.IsTypingOnChat_5D15E0(action, pPlayer);
}

MATCH_FUNC(0x5d6c70)
s32 Hud_2B00::sub_5D6C70(s32 a1)
{
    return field_12EC_sub.sub_5D15A0(a1) || field_2A25_sub.sub_5D17D0(a1);
}

MATCH_FUNC(0x5d6cb0)
bool Hud_2B00::sub_5D6CB0(s32 a1)
{
    return field_12EC_sub.sub_5D15A0(a1);
}

// https://decomp.me/scratch/Y4V1E it matches on decompme
STUB_FUNC(0x5d6cd0)
Hud_2B00::Hud_2B00()
{
    NOT_IMPLEMENTED;
    field_13C4_text_speed = 0;
}