#define FIX16_POINT_ZERO kFpZero_7064C0
#include "Hud.hpp"
#include "hud_message_priority.hpp"
#include "hud_brief_priority.hpp"
#include "voice_line.hpp"
#include "Car.hpp"
#include "Draw.hpp"
#include "Frontend.hpp"
#include "Game.hpp"
#include "Gang.hpp"
#include "Globals.hpp"
#include "Object_5C.hpp"
#include "Ped.hpp"
#include "Player.hpp"
#include "Police.hpp"
#include "Weapon.hpp"
#include "debug.hpp"
#include "error.hpp"
#include "ScriptManager.hpp"
#include "gbh_graphics.hpp"
#include "gtx_0x106C.hpp"
#include "KeyboardLayout.hpp"
#include "GameSession.hpp"
#include "registry.hpp"
#include "rng.hpp"
#include "root_sound.hpp"
#include "TextTable.hpp"
#include "winmain.hpp"

DEFINE_GLOBAL(Hud*, gHud_2B00_706620, 0x706620);
DEFINE_GLOBAL(s16, gDebugFont_706600, 0x706600);
DEFINE_GLOBAL(s16, gPickupTextFont_7064B8, 0x7064B8);
DEFINE_GLOBAL(u16, gZoneNameFont_706618, 0x706618);
DEFINE_GLOBAL(s16, gCarNameFont_706508, 0x706508);
DEFINE_GLOBAL(u16, gPlayerStatsFont_70646C, 0x70646C);
DEFINE_GLOBAL(u16, gBriefFont_7065C4, 0x7065C4);
DEFINE_GLOBAL(u16, gMessageFont_7062F0, 0x7062F0);
DEFINE_GLOBAL(u16, gPauseFont_7063F8, 0x7063F8);
DEFINE_GLOBAL(u16, gPlayerNameFont_7062DC, 0x7062DC);
// The chat font. Renaming it (for example to gChatFont_70643E) changes the code of
// Hud_ChatInput::DrawChatMessages_5D16B0, so it keeps its old name.
DEFINE_GLOBAL(u16, gChatDisplayFont_70643E, 0x70643E);
DEFINE_GLOBAL_ARRAY(char_type, gTmpGxtKey_67CE50, 256, 0x67CE50);
DEFINE_GLOBAL(s16, word_7064D8, 0x7064D8);
DEFINE_GLOBAL_INIT(Fix16, kFpZero_7064C0, 0, 0x7064C0);
DEFINE_GLOBAL_INIT(Fix16, kArrowBaseRepositionSpeed_7063B0, Fix16(0x400, 0), 0x7063B0);
DEFINE_GLOBAL_INIT(Fix16, kArrowRadiusOffset_7065B4, Fix16(0x1C00, 0), 0x7065B4);
DEFINE_GLOBAL_INIT(Fix16, kArrowMinRadiusStep_706338, Fix16(0x100, 0), 0x706338);

DEFINE_GLOBAL(Fix16, phone_x_67CD14, 0x67CD14);
DEFINE_GLOBAL(Fix16, phone_y_67CD0C, 0x67CD0C);
DEFINE_GLOBAL(Fix16, phone_z_67CD10, 0x67CD10);

DEFINE_GLOBAL_INIT(Ang16, kAngZero_706610, Ang16(0), 0x706610);

DEFINE_GLOBAL_INIT(Ang16, kAng180_706412, Ang16(720), 0x706412);
DEFINE_GLOBAL_INIT(Fix16, kFpOne_7064C4, Fix16(1), 0x7064C4);
DEFINE_GLOBAL_INIT(Fix16, kFpEight_7064E8, Fix16(8), 0x7064E8);
DEFINE_GLOBAL_INIT(Fix16, kFpQuarter_706300, Fix16(0x1000, 0), 0x706300);
DEFINE_GLOBAL_INIT(Fix16, kArrowMaxRepositionSpeed_706298, Fix16(0xC00, 0), 0x706298);
DEFINE_GLOBAL_INIT(Fix16, kArrowRepositionAccel_7065A8, Fix16(0x100, 0), 0x7065A8);

EXTERN_GLOBAL_ARRAY(wchar_t, gTmpWideStr_67C7D8, 640);
DEFINE_GLOBAL_INIT(s32, MaxLineWidth_62689C, 576, 0x62689C);

EXTERN_GLOBAL_ARRAY(wchar_t, tmpBuff_67BD9C, 640);

EXTERN_GLOBAL(char_type, gLighting_626A09);

EXTERN_GLOBAL_ARRAY(wchar_t, gEmptyWStr_67DC8C, 2);

// 9.6f inline
static inline void DrawFigureScaled_4C71B0(s32 sprite_type, s16 sprite_idx, Fix16 x_pos, Fix16 y_pos, const Ang16& rotation, const s32& palette_type, s16 palette, s32 alpha_value, u8 flags)
{
    DrawFigure_5D7EC0(sprite_type,
                      sprite_idx,
                      x_pos * gViewCamera_676978->field_A8_ui_scale,
                      y_pos * gViewCamera_676978->field_A8_ui_scale,
                      rotation,
                      gViewCamera_676978->field_A8_ui_scale,
                      palette_type,
                      palette,
                      alpha_value,
                      flags,
                      false);
}

// Camera::WorldToScreen_40CFC0 (9.6f 0x40CFC0: x/y/z by value, writes through two out pointers)
// with this file's copies of the 1 and 8 constants
static inline void WorldToScreen_Hud_40CFC0(Camera* pCam, Fix16 x, Fix16 y, Fix16 z, Fix16* pOutX, Fix16* pOutY)
{
    Fix16 u = pCam->field_98_cur_pos.field_8_z - z;
    Fix16 t(kFpOne_7064C4 / Fix16(u.mValue + kFpEight_7064E8.mValue, 0));
    *pOutX = (((x - pCam->field_98_cur_pos.field_0_x) * pCam->field_60_screen_scale.y) * t) + Fix16(320);
    *pOutY = (((y - pCam->field_98_cur_pos.field_4_y) * pCam->field_60_screen_scale.y) * t) + Fix16(240);
}

// 9.6f 0x4C7280: DrawText_5D8A10 at a position and size scaled by the UI scale
static inline void DrawTextScaled_4C7280(const wchar_t* pStr, Fix16 x, Fix16 y, u16 font, const s32& palette_type, u16 palette, s32 alpha, u8 flags)
{
    DrawText_5D8A10(pStr,
                    x * gViewCamera_676978->field_A8_ui_scale,
                    y * gViewCamera_676978->field_A8_ui_scale,
                    font,
                    gViewCamera_676978->field_A8_ui_scale,
                    palette_type,
                    palette,
                    alpha,
                    flags);
}

// Camera::ProjectWorldToScreen_4B90E0 (9.6f 0x4B90E0, inlined in 10.5) with this file's copies of the
// 1 and 8 constants
static inline void ProjectWorldToScreen_Hud_4B90E0(Camera* pCam, Fix16 x, Fix16 y, Fix16 z, Fix16* pOut1, Fix16* pOut2)
{
    Fix16 scale = kFpOne_7064C4 / ((kFpEight_7064E8 - z) + pCam->field_98_cur_pos.field_8_z);
    *pOut1 = (((x - pCam->field_98_cur_pos.field_0_x) * pCam->field_60_screen_scale.x) * scale) + Fix16(pCam->field_70_screen_px_center_x);
    *pOut2 = (((y - pCam->field_98_cur_pos.field_4_y) * pCam->field_60_screen_scale.x) * scale) + Fix16(pCam->field_74_screen_px_center_y);
}

MATCH_FUNC(0x4bbbb0)
Hud::~Hud()
{
}

MATCH_FUNC(0x4be650)
Hud_Pager::~Hud_Pager()
{
    field_0_timer = -1;
    field_4_ptr_counter = NULL;

    if (field_8_sound != NULL)
    {
        gRoot_sound_66B038.DestroySoundObj_40FE60(field_8_sound);
        field_8_sound = NULL;
    }
}

// ----------------------------------------------------

MATCH_FUNC(0x4DEF00)
EXPORT wchar_t* gmp_map_zone::get_zone_str_4DEF00()
{
    char_type buf[4];
    if (field_5_name_length == 3)
    {
        buf[0] = field_6_name[0];
        buf[1] = field_6_name[1];
        buf[2] = field_6_name[2];
        buf[3] = 0;
        return gText_0x14_704DFC->Find_5B5F90(buf);
    }
    return NULL;
}

MATCH_FUNC(0x5cf620)
void Hud_BriefSelector::ShowNextNumberedBrief_5CF620()
{
    do
    {
        field_0_value++;
        if (field_0_value > 9999)
        {
            field_0_value = 0;
        }
        sprintf(gTmpBuffer_67C598, "%d", field_0_value);
    } while (!gText_0x14_704DFC->KeyExists_5B5FA0(gTmpBuffer_67C598));
    gHud_2B00_706620->field_DC_brief.SetHudBrief_5D4400(hud_brief_priority::now_3, gTmpBuffer_67C598);
    swprintf(tmpBuff_67BD9C, L"%d", field_0_value);
    gHud_2B00_706620->field_111C_message.ShowMessage_5D1A00(tmpBuff_67BD9C, hud_message_priority::important_3);
}

MATCH_FUNC(0x5cf6b0)
void Hud_BriefSelector::ShowPrevNumberedBrief_5CF6B0()
{
    do
    {
        field_0_value--;
        if (field_0_value < 0)
        {
            field_0_value = 9999;
        }
        sprintf(gTmpBuffer_67C598, "%d", field_0_value);
    } while (!gText_0x14_704DFC->KeyExists_5B5FA0(gTmpBuffer_67C598));
    gHud_2B00_706620->field_DC_brief.SetHudBrief_5D4400(hud_brief_priority::now_3, gTmpBuffer_67C598);
    swprintf(tmpBuff_67BD9C, L"%d", field_0_value);
    gHud_2B00_706620->field_111C_message.ShowMessage_5D1A00(tmpBuff_67BD9C, hud_message_priority::important_3);
}

MATCH_FUNC(0x5cf730)
void Hud_UnderRoofArrowMarker::Update_5CF730()
{
    Ped* pPed = gGame_0x40_67E008->field_38_local_player->GetCameraModePed();

    if (!pPed || static_cast<u8>(pPed->IsInTrain_470F00()))
    {
        field_A_ped_under_solid = false;
    }
    else
    {
        field_A_ped_under_solid =
            gMap_0x370_6F6268->CheckColumnHasSolidAbove_4E7FC0(pPed->get_cam_x(), pPed->get_cam_y(), pPed->get_cam_z());
        if (field_A_ped_under_solid)
        {
            this->field_8_rotation = pPed->GetRotation() + kAng180_706412;

            Fix16 camz = pPed->field_1AC_cam.z;
            Fix16 camy = pPed->field_1AC_cam.y;
            Fix16 camx = pPed->field_1AC_cam.x;

            Camera* pCam = gGame_0x40_67E008->field_38_local_player->get_camera_434900();
            ProjectWorldToScreen_Hud_4B90E0(pCam, camx, camy, camz, &field_0_screen_x, &field_4_screen_y);
        }
    }
}

MATCH_FUNC(0x5cf910)
void Hud_UnderRoofArrowMarker::Draw_5CF910()
{
    if (field_A_ped_under_solid)
    {
        const s32 palette_type = palette_types_enum::sprites_2;
        Player* pPlayer = gGame_0x40_67E008->field_38_local_player;
        Camera* pCam = pPlayer->get_camera_434900();

        DrawFigure_5D7EC0(sprite_types_enum::user_6, 0, field_0_screen_x, field_4_screen_y, field_8_rotation, pCam->field_A8_ui_scale, palette_type, 0, 1, 14, true);
    }
}

MATCH_FUNC(0x5cf970)
void Hud_ShowCoords::ShowPlayerCoords_5CF970()
{
    if (field_0_show_coords)
    {
        Player* pPlayer = gGame_0x40_67E008->field_38_local_player;

        Ped* pPed = pPlayer->GetActivePed_4A5150();

        Gang* pZone = pPlayer->get_gang_curr_location_4766D0();
        wchar_t* pZoneName;
        if (pZone)
        {
            pZoneName = pZone->GetNameWide_4BED30();
        }
        else
        {
            pZoneName = gEmptyWStr_67DC8C;
        }

        Car* pCar = pPed->field_16C_car;
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

        Hud_TextEntry* pTextEntry = gHud_2B00_706620->field_650_texts.DisplayText_5D1F50(tmpBuff_67BD9C, -1, 16, gPickupTextFont_7064B8, 1);
        pTextEntry->SetDrawKind8_45AFD0(0);
    }
}

// ----------------------------------------------------

// https://decomp.me/scratch/bd2MO
MATCH_FUNC(0x5cfa70)
void Hud_GangRespectBars::DrawGangRespectBars_5CFA70()
{
    u32 random_num = gpRng_67AB34->get_cur_rng_41CFE0() & 0xF;
    u8 PlayerIdx = gGame_0x40_67E008->field_38_local_player->get_idx_4219D0();
    bool bPlusSignDark = random_num > 7u;

    // 64 and the bar x go through the Fix16(u32) constructor (out-of-line copy 0x4926F0)
    s32 ypos = 11;

    for (Gang* pGang = gGangPool_CA8_67E274->FirstGang_4BECA0(); pGang; ypos += 27, pGang = gGangPool_CA8_67E274->NextGang_4BECE0())
    {
        s8 respect = pGang->GetRespectForPlayer_4BEEF0(PlayerIdx);

        s32 arrow_colour = pGang->field_138_arrow_colour - 1;
        DrawFigureScaled_5D7670(sprite_types_enum::user_6,
                                arrow_colour + 64,
                                16,
                                static_cast<u32>(ypos + 2),
                                kAngZero_706610,
                                palette_types_enum::sprites_2,
                                0,
                                0,
                                0);

        DrawFigureScaled_5D7670(sprite_types_enum::user_6,
                                arrow_colour + 78,
                                64u,
                                static_cast<u32>(ypos + 2),
                                kAngZero_706610,
                                palette_types_enum::sprites_2,
                                0,
                                0,
                                0);

        DrawFigureScaled_5D7670(sprite_types_enum::user_6,
                                arrow_colour + 71,
                                64u,
                                ypos + 1,
                                kAngZero_706610,
                                palette_types_enum::sprites_2,
                                0,
                                0,
                                0);

        // Draw positive respect
        s32 respect_i = respect;
        s32 curr_bar_respect = 20;
        for (s32 i = 69; i <= 84 && respect_i >= curr_bar_respect; i += 5)
        {
            DrawFigureScaled_5D7670(sprite_types_enum::user_6,
                                    arrow_colour + 71,
                                    static_cast<u32>(i),
                                    ypos + 1,
                                    kAngZero_706610,
                                    palette_types_enum::sprites_2,
                                    0,
                                    0,
                                    0);
            curr_bar_respect += 20;
        }

        // Draw negative respect
        curr_bar_respect = -20;
        for (s32 j = 59; j >= 44 && respect_i <= curr_bar_respect; j -= 5)
        {
            DrawFigureScaled_5D7670(sprite_types_enum::user_6,
                                    arrow_colour + 71,
                                    static_cast<u32>(j),
                                    ypos + 1,
                                    kAngZero_706610,
                                    palette_types_enum::sprites_2,
                                    0,
                                    0,
                                    0);
            curr_bar_respect -= 20;
        }

        if (respect < -19)
        {
            if (respect > -100 || !bPlusSignDark)
            {
                DrawFigureScaled_5D7670(sprite_types_enum::user_6,
                                        2 * arrow_colour + 50,
                                        34,
                                        ypos + 1,
                                        kAngZero_706610,
                                        palette_types_enum::sprites_2,
                                        0,
                                        0,
                                        0);
            }
        }
        else
        {
            if (respect < 100 || !bPlusSignDark)
            {
                DrawFigureScaled_5D7670(sprite_types_enum::user_6,
                                        2 * arrow_colour + 51,
                                        93,
                                        ypos + 1,
                                        kAngZero_706610,
                                        palette_types_enum::sprites_2,
                                        0,
                                        0,
                                        0);
            }

            // green mission respect
            // Always true here, but the original re-tests it on this path (written this way VC6 keeps the test)
            if (-19 <= respect)
            {
                DrawFigureScaled_5D7670(sprite_types_enum::user_6,
                                        46,
                                        64u,
                                        ypos + 9,
                                        kAngZero_706610,
                                        palette_types_enum::sprites_2,
                                        0,
                                        0,
                                        0);
            }
        }

        // yellow mission respect
        if (respect >= 40)
        {
            DrawFigureScaled_5D7670(sprite_types_enum::user_6, 47, 74, ypos + 9, kAngZero_706610, palette_types_enum::sprites_2, 0, 0, 0);
        }

        // red mission respect
        if (respect >= 80)
        {
            DrawFigureScaled_5D7670(sprite_types_enum::user_6, 48, 84, ypos + 9, kAngZero_706610, palette_types_enum::sprites_2, 0, 0, 0);
        }

        // debug stuff
        if (bDo_show_instruments_67D64C)
        {
            s32 text_palette = (respect >= 0) + 5;
            swprintf(tmpBuff_67BD9C, L"%d", respect);
            DrawText_5D7720(tmpBuff_67BD9C, 64u, ypos - 6, gDebugFont_706600, palette_types_enum::font_remaps_8, text_palette, 0, 0);
        }
    }
}

MATCH_FUNC(0x5cfe20)
void Hud_GangRespectBars::Empty_5CFE20()
{
}

MATCH_FUNC(0x5cfe30)
void Hud_GangRespectBars::Empty_5CFE30()
{
}

MATCH_FUNC(0x5cfe40)
void Hud_PlayerNames::DrawPlayerNames_5CFE40()
{
    if (bStartNetworkGame_7081F0 && bShow_player_names_67D54C)
    {
        Camera* pCam = gGame_0x40_67E008->field_38_local_player->get_camera_434900();

        for (Player* pIter = gGame_0x40_67E008->IterateFirstPlayer_4B9CD0(); pIter; pIter = gGame_0x40_67E008->IterateNextPlayer_4B9D10())
        {
            if (!pIter->field_0_is_user)
            {
                Ped* pPlayerPed = pIter->field_2C4_player_ped;
                if (!pPlayerPed || !pPlayerPed->field_21C_bf.bInvisible)
                {
                    Fix16 x = pPlayerPed->field_1AC_cam.x;
                    Fix16 y = pPlayerPed->field_1AC_cam.y;
                    Fix16 z = pPlayerPed->field_1AC_cam.z;
                    // The original tests only al (bool return?)
                    if (static_cast<u8>(pCam->IsCoordsPosVisible_435A70(x, y, z)))
                    {
                        Fix16 xCalc;
                        Fix16 yCalc;
                        WorldToScreen_Hud_40CFC0(pCam, x, y, z, &xCalc, &yCalc);

                        DrawTextScaled_4C7280(pIter->field_83C_player_name,
                                              xCalc,
                                              yCalc,
                                              gPlayerNameFont_7062DC,
                                              pIter->field_78C_hud_palette_type != palette_types_enum::user_remaps_7 ? palette_types_enum::sprites_2 : palette_types_enum::font_remaps_8,
                                              pIter->field_790_hud_palette - 1,
                                              0,
                                              0);
                    }
                }
            }
        }
    }
}

// ----------------------------------------------------

MATCH_FUNC(0x5d0050)
void Hud_CopHead::UpdateHead_5D0050(bool bShakeHead)
{
    if (!bShakeHead)
    {
        field_4_height = 0;
    }

    field_1_frame_timer--;

    if (field_1_frame_timer == 0)
    {
        field_1_frame_timer = field_2_frame_delay;
        field_0_frame = field_0_frame == 0;
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

MATCH_FUNC(0x5d00b0)
void Hud_CopHead_C_Array::UpdateWantedLevel_5D00B0()
{
    Ped* pPed = gGame_0x40_67E008->field_38_local_player->GetPlayerPed_41D020();
    field_48_cop_level = pPed->get_wanted_star_count_46EF00();

    const bool bShakeHead = gPolice_7B8_6FEE40->IsBeingPursued_56F800(pPed);
    s32 i = 0;
    Hud_CopHead* pIter = &field_1028_cop_heads[0];
    while (i < field_48_cop_level)
    {
        pIter->UpdateHead_5D0050(bShakeHead);
        i++;
        pIter++;
    }
}

// https://decomp.me/scratch/QYlEW
MATCH_FUNC(0x5d0110)
void Hud_CopHead_C_Array::DrawWantedLevel_5D0110()
{
    Fix16 xpos = (Fix16(640) - (field_4C_w_fp * field_48_cop_level)) / 2;
    Fix16 y_base = field_50_h_fp / 2;

    s32 cop_head_idx = 0;
    Hud_CopHead* pHead = field_1028_cop_heads;
    for (; cop_head_idx < field_48_cop_level; cop_head_idx++, pHead++, xpos += field_4C_w_fp)
    {
        DrawFigureScaled_4C71B0(sprite_types_enum::user_6, pHead->field_0_frame + 14, xpos, y_base + Fix16(pHead->field_4_height), kAngZero_706610, palette_types_enum::sprites_2, 0, 0, 0);
    }
}

MATCH_FUNC(0x5d0210)
void Hud_CopHead_C_Array::Init_5D0210()
{
    u16 sprite_idx = gGtx_0x106C_703DD4->GetSpriteTrueIndex_5AA460(sprite_types_enum::user_6, 14);
    sprite_index* sprite_index = gGtx_0x106C_703DD4->get_sprite_index_5AA440(sprite_idx);

    field_4C_w_fp.FromU8(sprite_index->field_4_width);
    field_50_h_fp.FromU8(sprite_index->field_5_height);

    for (s32 i = 0; i < GTA2_COUNTOF(field_1028_cop_heads); i++)
    {
        field_1028_cop_heads[i].field_2_frame_delay = 2;
        field_1028_cop_heads[i].field_1_frame_timer = 2;
    }
}

// ----------------------------------------------------

MATCH_FUNC(0x5d0260)
void Hud_Health::DrawHealth_5D0260()
{
    s32 half_hearts;
    s32 xpos = 551;
    s32 health = gGame_0x40_67E008->field_38_local_player->GetPlayerPed_41D020()->get_health_433B70();

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
        DrawFigureScaled_5D7670(sprite_types_enum::user_6, 113, xpos, static_cast<u32>(34), kAngZero_706610, palette_types_enum::sprites_2, 0, 0, 0);
    }

    // Draw half heart
    if (half_hearts % 2 == 1)
    {
        if (half_hearts > 0)
        {
            xpos -= 2;
        }
        DrawFigureScaled_5D7670(sprite_types_enum::user_6, 114, xpos, static_cast<u32>(34), kAngZero_706610, palette_types_enum::sprites_2, 0, 0, 0);
    }

    // Draw debug stuff
    if (bDo_show_instruments_67D64C)
    {
        swprintf(tmpBuff_67BD9C, L"%d%%", health);
        DrawText_5D7720(tmpBuff_67BD9C, static_cast<u32>(551), static_cast<u32>(34), gDebugFont_706600, palette_types_enum::sprites_2, 0, 0, 0);
    }
}

MATCH_FUNC(0x5d03c0)
void ArrowTrace::PointToInfoPhone_5D03C0(Gang* pZone)
{
    set_arrow_aim_from_pos_4767C0(pZone->field_12C_info_phone_x, pZone->field_130_info_phone_y, pZone->field_134_info_phone_z);
    field_10_target_type = ArrowTargetType::InfoPhone_5;
}

// ----------------------------------------------------

MATCH_FUNC(0x5D03F0)
void ArrowTrace::UpdateAimCoordinates_5D03F0()
{
    Ped* pPed;
    Player* pPlayer;
    Car* pCar;

    switch (field_10_target_type)
    {
        case ArrowTargetType::Nothing_0:
            return;
        case ArrowTargetType::Player_6:
            pPlayer = field_C_player;
            if (pPlayer->GetInUse_461DB0())
            {
                field_14_aim_x = pPlayer->field_2C4_player_ped->get_cam_x();
                field_18_aim_y = pPlayer->field_2C4_player_ped->get_cam_y();
                field_1C_aim_z = pPlayer->field_2C4_player_ped->get_cam_z();
            }
            else
            {
                ClearTarget_476830();
            }
            break;
        case ArrowTargetType::Ped_2:
            pPed = field_0_ped;
            if (pPed->IsActive_433B40())
            {
                field_14_aim_x = pPed->get_cam_x();
                field_18_aim_y = pPed->get_cam_y();
                field_1C_aim_z = pPed->get_cam_z();
            }
            else
            {
                ClearTarget_476830();
            }
            break;
        case ArrowTargetType::Car_3:
            pCar = field_4_car;
            if (pCar->IsDespawning_4215B0())
            {
                ClearTarget_476830();
            }
            else
            {
                pCar->field_50_car_sprite->GetPosition_4117B0(&field_14_aim_x, &field_18_aim_y, &field_1C_aim_z);
            }
            break;
        case ArrowTargetType::Object_4:
            if (!field_8_obj->IsNotModel_174_529200())
            {
                gHud_2B00_706620->field_1F18_arrows.field_844_check_info_phones = true;
                ClearTarget_476830();
            }
            else
            {
                field_8_obj->field_4_sprite->GetPosition_4117B0(&field_14_aim_x, &field_18_aim_y, &field_1C_aim_z);
            }
            break;
        default:
            break;
    }

    Player* pLocalPlayer = gGame_0x40_67E008->field_38_local_player;
    Camera* pCam = pLocalPlayer->get_camera_434900();

    field_20_bIsTargetVisible = pCam->IsCoordsPosVisible_435A70(field_14_aim_x, field_18_aim_y, field_1C_aim_z);
}

// ----------------------------------------------------

MATCH_FUNC(0x5d0510)
void Hud_Arrow::SetArrowColour_5D0510(s32 arrow_colour)
{
    field_18_targets.field_28_arrow_colour = arrow_colour;
}

MATCH_FUNC(0x5d0530)
bool Hud_Arrow::CheckVisibility_5D0530()
{
    Gang* pThisGang = field_18_targets.field_10_gang_info.field_30_gang;

    if (pThisGang)
    {
        if (!gHud_2B00_706620->field_1F18_arrows.ShowGangArrows_4C70B0())
        {
            return false;
        }
        if (!bShow_all_arrows_67D6E7)
        {
            if (gScriptManager_6F8060->IsOnMission_4C7350())
            {
                return false; // player is on mission, so do not display gang phone arrows
            }
            Player* pPlayer = gGame_0x40_67E008->field_38_local_player;
            Gang* pCurrLocationGang = pPlayer->get_gang_curr_location_4766D0();
            if (pCurrLocationGang)
            {
                if (pPlayer->get_arrow_blocker_zone_4C7340())
                {
                    pCurrLocationGang = NULL;
                }
            }
            if (field_18_targets.field_60_curr_target->field_10_target_type == ArrowTargetType::InfoPhone_5)
            {
                if (pCurrLocationGang == pThisGang)
                {
                    if (gHud_2B00_706620->field_1F18_arrows.IsThereAnyOtherArrowsInSameGang_5D0E40(this))
                    {
                        return false;
                    }
                }
                else if (pCurrLocationGang && gHud_2B00_706620->field_1F18_arrows.IsThereAnyMissionPhoneArrowForGang_5D0F40(pCurrLocationGang))
                {
                    return false;
                }
            }
            else
            {
                if (pCurrLocationGang != pThisGang)
                {
                    return false;
                }
                u8 player_idx = pPlayer->get_idx_4219D0();
                if (pThisGang->GetRespectForPlayer_4BEEF0(player_idx) < field_18_targets.field_10_gang_info.field_34_min_respect)
                {
                    return false;
                }
                Hud_Arrow* pVisibleGangArrow = gHud_2B00_706620->field_1F18_arrows.field_840_visible_gang_arrow;
                if (pVisibleGangArrow && pVisibleGangArrow != this)
                {
                    return false;
                }
            }
        }
    }
    return true;
}

// https://decomp.me/scratch/pp6SY
MATCH_FUNC(0x5d0620)
bool Hud_Arrow::UpdateTargets_5D0620()
{
    Fix16_Point diff;
    field_18_targets.field_18_primary_target.UpdateAimCoordinates_5D03F0();
    field_18_targets.field_3C_secondary_target.UpdateAimCoordinates_5D03F0();

    if (field_18_targets.field_60_curr_target->HasNoTarget_4C6F20())
    {
        swap_arrows_4C7060(); // Swap arrow traces
        if (field_18_targets.field_60_curr_target->HasNoTarget_4C6F20())
        {
            return true;
        }
    }
    if (field_18_targets.HasBothTargets_4C6FB0() && field_18_targets.field_18_primary_target.field_20_bIsTargetVisible &&
        field_18_targets.field_3C_secondary_target.field_20_bIsTargetVisible)
    {
        if (field_18_targets.field_2E_target_swap_timer > 0)
        {
            field_18_targets.field_2E_target_swap_timer--;
            return false;
        }

        Fix16 xpos;
        Fix16 ypos;
        Fix16 zpos;
        gGame_0x40_67E008->field_38_local_player->get_pos_569920(&xpos, &ypos, &zpos);

        diff.SetXY_432860(xpos - field_18_targets.field_60_curr_target->field_14_aim_x, ypos - field_18_targets.field_60_curr_target->field_18_aim_y);
        Fix16 distance_1 = diff.GetLength_41E260() - field_10_radius_pos;

        swap_arrows_4C7060();

        diff.SetXY_432860(xpos - field_18_targets.field_60_curr_target->field_14_aim_x, ypos - field_18_targets.field_60_curr_target->field_18_aim_y);
        Fix16 new_radius = diff.GetLength_41E260() - distance_1;
        field_18_targets.field_2E_target_swap_timer = 20;
        field_10_radius_pos = new_radius;
    }
    return false;
}

// https://decomp.me/scratch/CoKn3
MATCH_FUNC(0x5d0850)
void Hud_Arrow::UpdateScreenPos_5D0850()
{
    Fix16_Point displacement;
    Fix16 player_xpos;
    Fix16 player_ypos;
    Fix16 player_zpos;

    gGame_0x40_67E008->field_38_local_player->get_pos_569920(&player_xpos, &player_ypos, &player_zpos);
    displacement.SetXY_432860(player_xpos - field_18_targets.field_60_curr_target->field_14_aim_x,
                              player_ypos - field_18_targets.field_60_curr_target->field_18_aim_y);

    field_8_rotation = displacement.atan2_40F790();

    // Assigned, not initialised: GetLength's result goes through a temporary that shares the atan2 temp's slot
    Fix16 distance;
    distance = displacement.GetLength_41E260();
    Fix16 intended_radius;

    if (field_18_targets.field_60_curr_target->field_20_bIsTargetVisible)
    {
        intended_radius = distance - kFpQuarter_706300;
        if (intended_radius < kFpZero_7064C0)
        {
            intended_radius = kFpZero_7064C0;
        }
    }
    else
    {
        intended_radius = field_C_min_radius_pos;
        if (gGame_0x40_67E008->field_38_local_player->GetPlayerCar_5698E0())
        {
            intended_radius += kArrowRadiusOffset_7065B4; // increment a little when in a car
        }
    }

    if (field_10_radius_pos > intended_radius)
    {
        field_10_radius_pos -= field_14_reposition_speed;
        if (field_10_radius_pos <= intended_radius)
        {
            field_10_radius_pos = intended_radius;
            field_14_reposition_speed = kArrowBaseRepositionSpeed_7063B0;
        }
        else if (field_14_reposition_speed < kArrowMaxRepositionSpeed_706298) // below the maximum
        {
            field_14_reposition_speed += kArrowRepositionAccel_7065A8;
        }
    }
    else if (field_10_radius_pos < intended_radius)
    {
        field_10_radius_pos += field_14_reposition_speed;
        if (field_10_radius_pos >= intended_radius)
        {
            field_10_radius_pos = intended_radius;
            field_14_reposition_speed = kArrowBaseRepositionSpeed_7063B0;
        }
        else if (field_14_reposition_speed < kArrowMaxRepositionSpeed_706298) // below the maximum
        {
            field_14_reposition_speed += kArrowRepositionAccel_7065A8;
        }
    }
    else
    {
        field_14_reposition_speed = kArrowBaseRepositionSpeed_7063B0;
    }

    // get_camera_434900 written out: as an inline call it shifts the inline budget and the projection's
    // operators no longer go out of line as in the original
    Camera* pCamera;
    Player* pPlayer = gGame_0x40_67E008->field_38_local_player;
    if (pPlayer->field_68_camera_mode == 2 || pPlayer->field_68_camera_mode == 3)
    {
        pCamera = &pPlayer->field_208_aux_game_camera;
    }
    else
    {
        pCamera = &pPlayer->field_90_game_camera;
    }

    Fix16 factor;
    factor = kFpOne_7064C4 / ((kFpEight_7064E8 - field_18_targets.field_60_curr_target->field_1C_aim_z) + pCamera->field_98_cur_pos.field_8_z);
    // multiply by 64
    Fix16& radius = field_10_radius_pos;
    Fix16 projected_radius = ((radius * 64) / (pCamera->field_60_screen_scale.x * factor)) * pCamera->field_A8_ui_scale;

    Fix16 zpos_2;
    if (distance != kFpZero_7064C0 && !field_18_targets.field_60_curr_target->field_20_bIsTargetVisible)
    {
        zpos_2 = player_zpos + ((radius / distance) * (field_18_targets.field_60_curr_target->field_1C_aim_z - player_zpos));
    }
    else
    {
        zpos_2 = field_18_targets.field_60_curr_target->field_1C_aim_z;
    }
    ProjectWorldToScreen_Hud_4B90E0(pCamera,
                                              player_xpos - (Ang16::sine_40F500(field_8_rotation) * projected_radius),
                                              player_ypos - (Ang16::cosine_40F520(field_8_rotation) * projected_radius),
                                              zpos_2,
                                              &field_0_screen_pos_x,
                                              &field_4_screen_pos_y);
}

MATCH_FUNC(0x5d0c60)
void Hud_Arrow::Service_5D0C60()
{
    if (!UpdateTargets_5D0620())
    {
        char_type bVisible = CheckVisibility_5D0530();
        field_18_targets.field_10_gang_info.field_5_is_visible = bVisible;
        if (bVisible)
        {
            UpdateScreenPos_5D0850();
        }
    }
}

MATCH_FUNC(0x5d0c90)
void Hud_Arrow::DrawArrow_5D0C90()
{
    Player* pPlayer;
    Camera* pCam;
    s32 arrow_palette_type;

    if (!IsType0_4C6F80())
    {
        if (IsVisible_4C7050())
        {
            if (field_18_targets.field_28_arrow_colour != 5 || (static_cast<u32>(gpRng_67AB34->get_cur_rng_41CFE0()) % 6 >= 3))
            {
                arrow_palette_type = palette_types_enum::sprites_2;
            }
            else
            {
                arrow_palette_type = palette_types_enum::user_remaps_7;
            }
            pPlayer = gGame_0x40_67E008->field_38_local_player;
            pCam = pPlayer->get_camera_434900();

            DrawFigure_5D7EC0(sprite_types_enum::user_6,
                              field_18_targets.field_2C_arrow_sprt_idx,
                              field_0_screen_pos_x,
                              field_4_screen_pos_y,
                              field_8_rotation,
                              pCam->field_A8_ui_scale,
                              arrow_palette_type,
                              0,
                              1,
                              14u,
                              true);

            s16 colour_related;
            switch (this->field_18_targets.field_28_arrow_colour)
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

            const s32 palette_type = palette_types_enum::sprites_2;
            pPlayer = gGame_0x40_67E008->field_38_local_player;
            pCam = pPlayer->get_camera_434900();

            DrawFigure_5D7EC0(sprite_types_enum::user_6,
                              colour_related + 8,
                              field_0_screen_pos_x,
                              field_4_screen_pos_y,
                              field_8_rotation,
                              pCam->field_A8_ui_scale,
                              palette_type,
                              0,
                              1,
                              14u,
                              true);
        }
    }
}

// ----------------------------------------------------

// 9.6f 0x4C7380
inline s32 TagGame::GetPlayerTime_4C7380(Player* pPlayer)
{
    return field_4_it_time_secs[pPlayer->get_idx_4219D0()];
}

MATCH_FUNC(0x5d0dc0)
void Hud_Arrow::SetPlayerArrowColour_5D0DC0(Ped* pPed)
{
    switch (pPed->field_244_remap)
    {
        case 11:
            field_18_targets.field_2C_arrow_sprt_idx = 1;
            break;
        case 13:
            field_18_targets.field_2C_arrow_sprt_idx = 2;
            break;
        case 8:
            field_18_targets.field_2C_arrow_sprt_idx = 3;
            break;
        case 5:
        case 6:
            field_18_targets.field_2C_arrow_sprt_idx = 4;
            break;
        case 7:
            field_18_targets.field_2C_arrow_sprt_idx = 5;
            break;
        case 9:
            field_18_targets.field_2C_arrow_sprt_idx = 6;
            break;
        case 10:
            field_18_targets.field_2C_arrow_sprt_idx = 7;
            break;

        default:
            return;
    }
}

MATCH_FUNC(0x5d0e40)
bool Hud_Arrow_7C_Array::IsThereAnyOtherArrowsInSameGang_5D0E40(Hud_Arrow* pArgArrow)
{
    Gang* pGang = pArgArrow->field_18_targets.field_10_gang_info.field_30_gang;

    for (s32 i = 0; i < GTA2_COUNTOF_S(field_0_array); i++)
    {
        Hud_Arrow* pArrow = &field_0_array[i];
        if (pArrow != pArgArrow && !pArrow->IsType0_4C6F80() && pArrow->IsVisible_4C7050() &&
            pArrow->field_18_targets.field_10_gang_info.field_30_gang == pGang)
        {
            return true;
        }
    }
    return false;
}

MATCH_FUNC(0x5d0e90)
void Hud_Arrow_7C_Array::DrawArrows_5D0E90()
{
    if (IsNetworkGame_434B10())
    {
        // Limit drawn arrows in multiplayer
        for (s32 i = 0; i < GTA2_COUNTOF_S(field_0_array); i++)
        {
            if (field_0_array[i].field_18_targets.field_18_primary_target.GetTargetPlayer_4C6F30() == NULL ||
                field_0_array[i].field_18_targets.field_18_primary_target.GetTargetPlayer_4C6F30()->GetPlayerPed_41D020() == NULL ||
                field_0_array[i].field_18_targets.field_18_primary_target.GetTargetPlayer_4C6F30()->GetPlayerPed_41D020()->field_21C_bf.bInvisible == 0)
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

MATCH_FUNC(0x5d0ef0)
void Hud_Arrow_7C_Array::FindVisibleGangArrow_5D0EF0()
{
    s32 idx = 0;
    Hud_Arrow* pIter = &field_0_array[0];
    while (idx < GTA2_COUNTOF_S(field_0_array))
    {
        if (pIter->IsType0_4C6F80() ||
            !pIter->field_18_targets.field_10_gang_info.field_30_gang || !pIter->field_18_targets.field_60_curr_target->field_20_bIsTargetVisible)
        {
            idx++;
            pIter++;
        }
        else
        {
            this->field_840_visible_gang_arrow = pIter;
            return;
        }
    }
    this->field_840_visible_gang_arrow = NULL;
}

// ----------------------------------------------------

MATCH_FUNC(0x5d0f40)
bool Hud_Arrow_7C_Array::IsThereAnyMissionPhoneArrowForGang_5D0F40(Gang* pArgGang)
{
    Hud_Arrow* pIter = &field_0_array[0];
    for (s32 i = 0; i < GTA2_COUNTOF_S(field_0_array); i++, pIter++)
    {
        if (!pIter->IsType0_4C6F80() &&
            (pIter->field_18_targets.field_10_gang_info.field_30_gang == pArgGang &&
             pIter->field_18_targets.field_60_curr_target->field_10_target_type != ArrowTargetType::InfoPhone_5))
        {
            return true;
        }
    }
    return false;
}

MATCH_FUNC(0x5d0f80)
void Hud_Arrow_7C_Array::ClearOrphanInfoPhoneArrows_5D0F80()
{
    for (s32 i = 0; i < GTA2_COUNTOF_S(field_0_array); i++)
    {
        if (!field_0_array[i].IsType0_4C6F80())
        {
            if (field_0_array[i].field_18_targets.field_10_gang_info.field_30_gang)
            {
                if (field_0_array[i].field_18_targets.field_60_curr_target->field_10_target_type == ArrowTargetType::InfoPhone_5 &&
                    !IsThereAnyMissionPhoneArrowForGang_5D0F40(field_0_array[i].field_18_targets.field_10_gang_info.field_30_gang))
                {
                    field_0_array[i].field_18_targets.field_18_primary_target.field_10_target_type = ArrowTargetType::Nothing_0;
                    field_0_array[i].field_18_targets.field_3C_secondary_target.field_10_target_type = ArrowTargetType::Nothing_0;
                }
            }
        }
    }
}

// ----------------------------------------------------

MATCH_FUNC(0x5d0fd0)
void Hud_Arrow_7C_Array::UpdateArrows_5D0FD0()
{
    FindVisibleGangArrow_5D0EF0();

    for (s32 i = 0; i < GTA2_COUNTOF(field_0_array); i++)
    {
        if (!field_0_array[i].IsType0_4C6F80())
        {
            field_0_array[i].Service_5D0C60();
        }
    }

    if (field_844_check_info_phones)
    {
        ClearOrphanInfoPhoneArrows_5D0F80();
        field_844_check_info_phones = false;
    }
}

// ----------------------------------------------------

MATCH_FUNC(0x5d1020)
Hud_Arrow* Hud_Arrow_7C_Array::FindFreeArrow_5D1020(s32* pOutIdx)
{
    Hud_Arrow* pIter = &field_0_array[0];
    for (s32 i = 0; i < GTA2_COUNTOF_S(field_0_array); i++, pIter++)
    {
        if (!pIter->field_18_targets.field_10_gang_info.field_6_in_use)
        {
            *pOutIdx = i;
            pIter->field_18_targets.field_10_gang_info.field_6_in_use = true;
            return pIter;
        }
    }
    return NULL;
}

MATCH_FUNC(0x5d1050)
Hud_Arrow* Hud_Arrow_7C_Array::AllocArrow_5D1050()
{
    s32 idx;
    Hud_Arrow* pRet = FindFreeArrow_5D1020(&idx);
    pRet->Reset_4CA610();
    pRet->SetMinRadiusPos_4C6FF0(16 - idx);
    return pRet;
}

MATCH_FUNC(0x5d10b0)
void Hud_Arrow_7C_Array::ReleaseAllArrows_5D10B0()
{
    for (s32 i = 0; i < GTA2_COUNTOF_S(field_0_array); i++)
    {
        if (field_0_array[i].field_18_targets.field_10_gang_info.field_6_in_use)
        {
            field_0_array[i].field_18_targets.field_18_primary_target.field_10_target_type = ArrowTargetType::Nothing_0;
            field_0_array[i].field_18_targets.field_3C_secondary_target.field_10_target_type = ArrowTargetType::Nothing_0;

            field_0_array[i].field_18_targets.field_10_gang_info.field_6_in_use = false;
        }
    }
}

MATCH_FUNC(0x5d10d0)
Hud_Arrow* Hud_Arrow_7C_Array::FindGangPhoneArrow_5D10D0(Gang* pZone, s32 phone_type)
{
    s32 i = 0;
    Hud_Arrow* pIter = field_0_array;
    while (i < GTA2_COUNTOF_S(field_0_array))
    {
        if ((pIter->field_18_targets.field_18_primary_target.field_10_target_type ||
             pIter->field_18_targets.field_3C_secondary_target.field_10_target_type) &&
            (pIter->field_18_targets.field_10_gang_info.field_30_gang == pZone && pIter->field_18_targets.field_28_arrow_colour == phone_type))
        {
            return pIter;
        }
        i++;
        pIter++;
    }
    return NULL;
}

MATCH_FUNC(0x5d1110)
void Hud_Arrow_7C_Array::place_gang_phone_5D1110(Object* pPhoneInfo)
{
    s32 phone_type = GetPhoneTypeFromObjModel_5D1260(pPhoneInfo->field_18_model);
    Gang* pZone = gMap_0x370_6F6268->GetGangAtCoords_4DFB50(pPhoneInfo->field_4_sprite->GetXPos(), pPhoneInfo->field_4_sprite->GetYPos());

    if (!pZone)
    {
        phone_x_67CD14 = pPhoneInfo->field_4_sprite->GetXPos();
        phone_y_67CD0C = pPhoneInfo->field_4_sprite->GetYPos();
        phone_z_67CD10 = 0;
        FatalError_4A38C0(Gta2Error::PlacingGangPhoneWhenNoGang, // Placing gang phone when no gang at: (%.4f, %.4f)
                          "C:\\Splitting\\Gta2\\Source\\user.cpp",
                          1509,
                          0,
                          &phone_x_67CD14,
                          &phone_y_67CD0C,
                          &phone_z_67CD10);
    }
    Hud_Arrow* pExistingArrow = Hud_Arrow_7C_Array::FindGangPhoneArrow_5D10D0(pZone, phone_type);
    if (pExistingArrow)
    {
        if (pExistingArrow->field_18_targets.field_3C_secondary_target.field_10_target_type)
        {
            strcpy(gErrStr_67C29C, get_phone_colour_5D12B0(phone_type));
            strcpy(gErrStr2_67C3A8, pZone->field_2_name);
            FatalError_4A38C0(Gta2Error::TooManyPhonesForGang, // Too many %s phones for %s gang
                              "C:\\Splitting\\Gta2\\Source\\user.cpp",
                              1513,
                              gErrStr_67C29C,
                              gErrStr2_67C3A8);
        }
        pExistingArrow->field_18_targets.field_3C_secondary_target.field_8_obj = pPhoneInfo;
        pExistingArrow->field_18_targets.field_3C_secondary_target.field_10_target_type = ArrowTargetType::Object_4;
    }
    else
    {
        Hud_Arrow* pNewArrow = Hud_Arrow_7C_Array::AllocArrow_5D1050();
        pNewArrow->field_18_targets.field_18_primary_target.field_8_obj = pPhoneInfo;
        pNewArrow->field_18_targets.field_18_primary_target.field_10_target_type = ArrowTargetType::Object_4;
        pNewArrow->SetArrowColour_5D0510(phone_type);
        pNewArrow->field_18_targets.field_10_gang_info.field_30_gang = pZone;
        pNewArrow->field_18_targets.field_10_gang_info.field_34_min_respect = GetMinRespectForPhoneType_5D12E0(phone_type);
        pNewArrow->field_18_targets.field_2C_arrow_sprt_idx = pZone->field_138_arrow_colour;
    }
}

// ----------------------------------------------------

MATCH_FUNC(0x5D1260)
s32 __stdcall GetPhoneTypeFromObjModel_5D1260(s32 phone_model_idx)
{
    switch (phone_model_idx)
    {
        case 176:
        case objects::red_phone_177:
            return 2;
        case 180:
        case objects::green_phone_181:
            return 3;
        case 178:
        case objects::yellow_phone_179:
            return 1;
        default:
            return 0;
    }
}

// ----------------------------------------------------

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
u8 __stdcall GetMinRespectForPhoneType_5D12E0(s32 phone_type)
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
void Hud_Arrow_7C_Array::SetNewGangArrow_5D1310(Gang* pZone)
{
    Hud_Arrow* pArrow = AllocArrow_5D1050();
    pArrow->SetArrowColour_5D0510(4);
    pArrow->field_18_targets.field_10_gang_info.field_30_gang = pZone;
    pArrow->field_18_targets.field_2C_arrow_sprt_idx = pZone->field_138_arrow_colour;
    pArrow->field_18_targets.field_10_gang_info.field_34_min_respect = 0;
    pArrow->field_18_targets.field_18_primary_target.PointToInfoPhone_5D03C0(pZone);
}

// ----------------------------------------------------

MATCH_FUNC(0x5d1350)
void Hud_Arrow_7C_Array::CreatePlayerArrows_5D1350()
{
    if (static_cast<u8>(bStartNetworkGame_7081F0))
    {
        if (gGameSession_67E8E0.GetMultiplayerGamemode_4C5BC0() != TAG_GAME_3)
        {
            ReleaseAllArrows_5D10B0();
            for (Player* pPlayerIter = gGame_0x40_67E008->IterateFirstPlayer_4B9CD0(); pPlayerIter;
                 pPlayerIter = gGame_0x40_67E008->IterateNextPlayer_4B9D10())
            {
                if (!pPlayerIter->field_0_is_user)
                {
                    Hud_Arrow* pArrow = AllocArrow_5D1050();
                    pArrow->field_18_targets.field_18_primary_target.field_C_player = pPlayerIter;
                    pArrow->field_18_targets.field_18_primary_target.field_10_target_type = ArrowTargetType::Player_6;
                    Ped* pPlayerPed = pPlayerIter->field_2C4_player_ped;
                    if (pPlayerPed)
                    {
                        pArrow->SetPlayerArrowColour_5D0DC0(pPlayerPed);
                    }
                }
            }
        }
    }
}

MATCH_FUNC(0x5d13c0)
bool Hud_QuitMessage::HandleQuitMessageKey_5D13C0(s32 action, Player* pPlayer)
{
    if (pPlayer->field_78A_show_quit_message)
    {
        if (action == DIK_RETURN)
        {
            pPlayer->field_78A_show_quit_message = false;
            if (pPlayer->IsUser_41DC70())
            {
                gGame_0x40_67E008->ExitGameNoBonus_4B8C00(1, GameExitType::PlayerQuit_2);
            }

            if (IsNetworkGame_434B10())
            {
                gTagGame_6F8450.SetQuit_461DD0(pPlayer->get_idx_4219D0());
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

// ----------------------------------------------------

MATCH_FUNC(0x5d1430)
void Hud_QuitMessage::DrawQuitMessage_5D1430()
{
    if (gGame_0x40_67E008->field_38_local_player->field_78A_show_quit_message)
    {
        wchar_t* pQuitText = gText_0x14_704DFC->Find_5B5F90("quit1");

        s32 max_text_width = Frontend::GetMaxTextWidth_5D8990(pQuitText, word_7064D8);
        s32 line_spacing_added = CountLineSpacing_5D8940(pQuitText, word_7064D8);
        s32 line2_ypos = ((3 * (160 - line_spacing_added)) / 2);

        DrawText_5D7720(pQuitText,
                        static_cast<u32>((640 - max_text_width) / 2),
                        line2_ypos - line_spacing_added,
                        word_7064D8,
                        palette_types_enum::sprites_2,
                        0,
                        0,
                        0);

        wchar_t* pQuitText2 = gText_0x14_704DFC->Find_5B5F90("quit2");
        s32 max_text_width_2 = Frontend::GetMaxTextWidth_5D8990(pQuitText2, word_7064D8);

        DrawText_5D7720(pQuitText2,
                        static_cast<u32>((640 - max_text_width_2) / 2),
                        static_cast<u32>(line2_ypos),
                        word_7064D8,
                        palette_types_enum::sprites_2,
                        0,
                        0,
                        0);

        wchar_t* pQuitText3 = gText_0x14_704DFC->Find_5B5F90("quit3");
        s32 max_text_width_3 = Frontend::GetMaxTextWidth_5D8990(pQuitText3, word_7064D8);

        DrawText_5D7720(pQuitText3,
                        static_cast<u32>((640 - max_text_width_3) / 2),
                        line2_ypos + line_spacing_added,
                        word_7064D8,
                        palette_types_enum::sprites_2,
                        0,
                        0,
                        0);
    }
}

MATCH_FUNC(0x5d15a0)
bool Hud_QuitMessage::IsQuitMessageKey_5D15A0(s32 action)
{
    return gGame_0x40_67E008->field_38_local_player->field_78A_show_quit_message && (action == DIK_RETURN || action == DIK_ESCAPE);
}

MATCH_FUNC(0x5d15d0)
void Hud_QuitMessage::ShowQuitMessage_5D15D0(Player* pPlayer)
{
    pPlayer->field_78A_show_quit_message = true;
}

// ----------------------------------------------------

MATCH_FUNC(0x5d15e0)
bool Hud_ChatInput::HandleChatInputKey_5D15E0(s32 action, Player* pPlayer)
{
    if (bStartNetworkGame_7081F0 && pPlayer->field_794_is_chatting)
    {
        if (action == DIK_RETURN)
        {
            pPlayer->field_794_is_chatting = false;
            return true;
        }
        else if (action == DIK_BACK)
        {
            if (pPlayer->field_838_chat_text_idx > 0)
            {
                pPlayer->field_838_chat_text_idx--;
                pPlayer->field_796_chat_text[pPlayer->field_838_chat_text_idx] = 0;
            }
            return true;
        }
        else if (action == DIK_SPACE)
        {
            if (pPlayer->field_838_chat_text_idx < 79)
            {
                pPlayer->field_796_chat_text[pPlayer->field_838_chat_text_idx] = ' ';
                pPlayer->field_838_chat_text_idx++;
                pPlayer->field_796_chat_text[pPlayer->field_838_chat_text_idx] = 0;
            }
            return true;
        }
        else
        {
            const u16 char_value = gText_0x14_704DFC->RemapExtendedChar_5B58D0(gKeybrd_0x204_6F52F4->GetKey_4D5F40(action));
            if (char_value)
            {
                if (pPlayer->field_838_chat_text_idx < 79)
                {
                    pPlayer->field_796_chat_text[pPlayer->field_838_chat_text_idx] = char_value;
                    pPlayer->field_838_chat_text_idx++;
                    pPlayer->field_796_chat_text[pPlayer->field_838_chat_text_idx] = 0;
                }
                return true;
            }
        }
    }
    return false;
}

// https://decomp.me/scratch/gMsUi
MATCH_FUNC(0x5d16b0)
void Hud_ChatInput::DrawChatMessages_5D16B0()
{
    s32 line_spacing = GetLineSpacingFromFontType_5D7700_inlined(gChatDisplayFont_70643E);
    s32 text_ypos = 480 - line_spacing;
    if (bStartNetworkGame_7081F0)
    {
        for (Player* pPlayerIter = gGame_0x40_67E008->IterateFirstPlayer_4B9CD0(); pPlayerIter != NULL;
             pPlayerIter = gGame_0x40_67E008->IterateNextPlayer_4B9D10())
        {
            if (pPlayerIter->field_794_is_chatting)
            {
                if ((gpRng_67AB34->get_cur_rng_41CFE0() & 7u) < 4)
                {
                    swprintf(tmpBuff_67BD9C, L"%s:%s_", pPlayerIter->field_83C_player_name, reinterpret_cast<wchar_t*>(pPlayerIter->field_796_chat_text));
                }
                else
                {
                    swprintf(tmpBuff_67BD9C, L"%s:%s ", pPlayerIter->field_83C_player_name, reinterpret_cast<wchar_t*>(pPlayerIter->field_796_chat_text));
                }
                s32 max_text_width = Frontend::GetMaxTextWidth_5D8990(tmpBuff_67BD9C, gChatDisplayFont_70643E);
                s32 start_xpos;
                if (max_text_width > 640)
                {
                    start_xpos = 640 - max_text_width;
                }
                else
                {
                    start_xpos = 0;
                }
                DrawText_5D7720(tmpBuff_67BD9C, start_xpos, text_ypos, gChatDisplayFont_70643E, palette_types_enum::font_remaps_8, 5, 0, 0);
                text_ypos -= line_spacing;
            }
        }
    }
}

MATCH_FUNC(0x5d17d0)
bool Hud_ChatInput::IsChatInputKey_5D17D0(s32 key_idx)
{
    if (bStartNetworkGame_7081F0)
    {
        if (gGame_0x40_67E008->field_38_local_player->field_794_is_chatting)
        {
            if (key_idx == DIK_RETURN || key_idx == DIK_BACK || key_idx == DIK_SPACE)
            {
                return true;
            }

            if (gText_0x14_704DFC->RemapExtendedChar_5B58D0(gKeybrd_0x204_6F52F4->GetKey_4D5F40(key_idx)))
            {
                return true;
            }
        }
    }
    return false;
}

MATCH_FUNC(0x5d1830)
void Hud_ChatInput::StartChatting_5D1830(Player* pPlayer)
{
    pPlayer->field_794_is_chatting = true;
    pPlayer->field_838_chat_text_idx = 0;
    pPlayer->field_796_chat_text[0] = 0;
}

// ----------------------------------------------------

MATCH_FUNC(0x5d1850)
void Hud_Message::ClearTimeToShow_5D1850()
{
    field_0_time_to_show = 0;
}

MATCH_FUNC(0x5d1860)
void Hud_Message::FormatMessage_5D1860()
{
    if (this->field_0_time_to_show)
    {
        TextTable::InsertLineBreaksAndGetNumLines_5B5BC0(&this->field_2_str[100], this->field_2_str, 580, gMessageFont_7062F0);
        this->field_1BC_xpos = static_cast<u16>((640 - Frontend::GetMaxTextWidth_5D8990(&this->field_2_str[100], gMessageFont_7062F0)) / 2);
        this->field_1C0_ypos = static_cast<u16>((480 - CountLineSpacing_5D8940(&this->field_2_str[100], gMessageFont_7062F0)) / 4);
    }
}

MATCH_FUNC(0x5d1940)
void Hud_Message::DrawMessage_5D1940()
{
    if (field_0_time_to_show)
    {
        DrawText_5D7720(&field_2_str[100], field_1BC_xpos, field_1C0_ypos, gMessageFont_7062F0, palette_types_enum::sprites_2, 0, 0, 0);
    }
}

MATCH_FUNC(0x5d1a00)
void Hud_Message::ShowMessage_5D1A00(const wchar_t* pStr, s32 priority)
{
    if (field_0_time_to_show <= 0 || priority >= field_1C4_priority)
    {
        field_1C4_priority = priority;
        wcscpy(field_2_str, pStr);
        gText_0x14_704DFC->StrToUpper_5B5B80(field_2_str);
        field_0_time_to_show = 90;
        FormatMessage_5D1860();
    }
}

MATCH_FUNC(0x5d1ab0)
void Hud_Message::DecrementTimeToShow_5D1AB0()
{
    if (field_0_time_to_show != 0)
    {
        field_0_time_to_show--;
    }
}

// ----------------------------------------------------

MATCH_FUNC(0x5d1ae0)
Hud_Message::Hud_Message()
{
    field_0_time_to_show = 0;
    field_1C4_priority = hud_message_priority::normal_1;
}

MATCH_FUNC(0x5d1b10)
void Hud_TextEntry::FormatAndSetupText_5D1B10(const wchar_t* pStr, s16 xpos, s16 ypos, s16 fontType, s32 displayTime)
{
    this->field_AC_fontType = fontType;

    TextTable::InsertLineBreaksAndGetNumLines_5B5BC0(field_0_str_buf, pStr, 640, fontType);

    if (field_AC_fontType == gStyleZoneNameFont_703C9C || field_AC_fontType == gStyleMessageFont_703D9C)
    {
        gText_0x14_704DFC->StrToUpper_5B5B80(field_0_str_buf);
    }

    this->field_B0_drawKind = 2;
    this->field_B4_palette = 0;

    if (xpos == -1)
    {
        this->field_A8_x = (640 - Frontend::GetMaxTextWidth_5D8990(field_0_str_buf, this->field_AC_fontType)) / 2;
    }
    else
    {
        this->field_A8_x = xpos;
    }

    if (ypos == -1)
    {
        this->field_AA_y = (480 - CountLineSpacing_5D8940(field_0_str_buf, field_AC_fontType)) / 2;
    }
    else
    {
        this->field_AA_y = ypos;
    }

    // Stores in both branches: one store after the if leaves 0 out of a register (ebx)
    if (displayTime == -2)
    {
        this->field_A4_display_time = gHud_2B00_706620->field_13C4_text_speed * wcslen(field_0_str_buf);
    }
    else
    {
        this->field_A4_display_time = displayTime;
    }

    ClearAlpha_4C70E0();
}

MATCH_FUNC(0x5d1d00)
void Hud_TextEntry::Draw_5D1D00()
{
    DrawText_5D7720(field_0_str_buf,
                    field_A8_x,
                    field_AA_y,
                    field_AC_fontType,
                    field_B0_drawKind,
                    field_B4_palette,
                    field_B8_alpha,
                    field_BC_alpha_flag);
}

MATCH_FUNC(0x5d1db0)
bool Hud_TextEntry::DecrementDisplayTime_5D1DB0()
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
bool Hud_TextEntry::operator_equals_5D1E10(const Hud_TextEntry* pOther) const
{
    return field_A4_display_time > 0 && pOther != this && field_A8_x == pOther->field_A8_x && field_AA_y == pOther->field_AA_y &&
        field_AC_fontType == pOther->field_AC_fontType && !wcscmp(field_0_str_buf, pOther->field_0_str_buf);
}

MATCH_FUNC(0x5d1eb0)
void Hud_TextList::ExpireDuplicates_5D1EB0(const Hud_TextEntry* pEntry)
{
    Hud_TextEntry* pIter = field_960_pFirst;
    while (pIter)
    {
        if (pIter->operator_equals_5D1E10(pEntry))
        {
            pIter->Expire_4C70F0();
            return;
        }
        pIter = pIter->field_C0_pNext;
    }
}

MATCH_FUNC(0x5d1f50)
Hud_TextEntry* Hud_TextList::DisplayText_5D1F50(const wchar_t* pStr, s16 xpos, s16 ypos, s16 font_type, s32 display_time)
{
    Hud_TextEntry* pEntry = field_964_pFreeList;
    Hud_TextEntry* pOldFirst = field_960_pFirst;
    field_964_pFreeList = pEntry->field_C0_pNext;
    pEntry->field_C0_pNext = pOldFirst;
    field_960_pFirst = pEntry;
    pEntry->FormatAndSetupText_5D1B10(pStr, xpos, ypos, font_type, display_time);
    ExpireDuplicates_5D1EB0(pEntry);
    return pEntry;
}

MATCH_FUNC(0x5d2010)
void Hud_TextList::Service_5D2010()
{
    Hud_TextEntry* pIter = field_960_pFirst;
    while (pIter)
    {
        pIter->Draw_5D1D00();
        pIter = pIter->field_C0_pNext;
    }
}

// 9.6f 0x4C7130
inline void Hud_Pager::SetCounter_4C7130(s32* pCounter)
{
    field_4_ptr_counter = pCounter;
    if (!field_8_sound && !bSkip_audio_67D6BE)
    {
        field_8_sound = gRoot_sound_66B038.CreateSoundObject_40EF40(this, SoundObjectTypeEnum::Hud_Pager_C_11);
    }
}

MATCH_FUNC(0x5d2050)
void Hud_TextList::RemoveExpired_5D2050()
{
    Hud_TextEntry* pPrev = NULL;
    Hud_TextEntry* pIter = field_960_pFirst;
    while (pIter)
    {
        if (pIter->DecrementDisplayTime_5D1DB0())
        {
            if (pPrev)
            {
                pPrev->field_C0_pNext = pIter->field_C0_pNext;
                pIter->field_C0_pNext = field_964_pFreeList;
                field_964_pFreeList = pIter;
                pIter = pPrev->field_C0_pNext;
            }
            else
            {
                Hud_TextEntry* pOldFreeList = field_964_pFreeList;
                field_960_pFirst = field_960_pFirst->field_C0_pNext;
                pIter->field_C0_pNext = pOldFreeList;
                field_964_pFreeList = pIter;
                pIter = field_960_pFirst;
            }
        }
        else
        {
            pPrev = pIter;
            pIter = pIter->field_C0_pNext;
        }
    }
}

MATCH_FUNC(0x5d2280)
Hud_TextList::Hud_TextList()
{
    Hud_TextEntry* pIter = &field_0_29_ary[0];
    for (s32 i = 0; i < 30 - 1; i++)
    {
        pIter->field_C0_pNext = pIter + 1;
        pIter++;
    }

    field_964_pFreeList = &field_0_29_ary[0];
    field_0_29_ary[30 - 1].field_C0_pNext = NULL;
    field_960_pFirst = NULL;
}

MATCH_FUNC(0x5d2320)
void Hud_Pager::Service_5D2320()
{
    if (field_0_timer < 0)
    {
        return;
    }

    field_0_timer--;
    if (field_0_timer == -1)
    {
        field_4_ptr_counter = NULL;
    }
}

MATCH_FUNC(0x5d2380)
void Hud_Pager::DrawCounterDigits_5D2380(s32 xpos, s32 ypos)
{
    s32 counter = *field_4_ptr_counter;
    s32 ones = counter % 10;
    s32 tens = (counter % 100 - ones) / 10;
    s32 hundreds = (counter % 1000 - tens - ones) / 100;
    s32 thousands = (counter - hundreds - tens - ones) / 1000;

    if (!thousands)
    {
        thousands = -1;
        if (!hundreds)
        {
            hundreds = -1;
            if (!tens)
            {
                tens = -1;
            }
        }
    }
    DrawFigureScaled_5D7670(sprite_types_enum::user_6, 123 + ones, xpos + 11, ypos + 2, kAngZero_706610, palette_types_enum::sprites_2, 0, 0, 0);

    DrawFigureScaled_5D7670(sprite_types_enum::user_6, 123 + tens, xpos + 4, ypos + 2, kAngZero_706610, palette_types_enum::sprites_2, 0, 0, 0);

    DrawFigureScaled_5D7670(sprite_types_enum::user_6, 123 + hundreds, xpos - 3, ypos + 2, kAngZero_706610, palette_types_enum::sprites_2, 0, 0, 0);

    DrawFigureScaled_5D7670(sprite_types_enum::user_6, 123 + thousands, xpos - 10, ypos + 2, kAngZero_706610, palette_types_enum::sprites_2, 0, 0, 0);
}

MATCH_FUNC(0x5d2680)
void Hud_Pager::DrawDigits_5D2680(s32 xpos, s32 ypos)
{
    // (field_0_timer / 30 fps) = total time in seconds
    s32 minutes = (field_0_timer / 30) / 60;
    s32 seconds = (field_0_timer / 30) % 60;
    DrawFigureScaled_5D7670(sprite_types_enum::user_6,
                            123 + minutes / 10,
                            xpos - 13,
                            ypos + 3,
                            kAngZero_706610,
                            palette_types_enum::sprites_2,
                            0,
                            0,
                            0);

    DrawFigureScaled_5D7670(sprite_types_enum::user_6,
                            123 + minutes % 10,
                            xpos - 6,
                            ypos + 3,
                            kAngZero_706610,
                            palette_types_enum::sprites_2,
                            0,
                            0,
                            0);

    if (field_0_timer % 15 <= 7)
    {
        DrawFigureScaled_5D7670(sprite_types_enum::user_6, 121, xpos - 1, ypos + 3, kAngZero_706610, palette_types_enum::sprites_2, 0, 0, 0);

        if (field_0_timer < 300) // less than 10 seconds: draw flashing light
        {
            DrawFigureScaled_5D7670(sprite_types_enum::user_6,
                                    133,
                                    xpos + 20,
                                    ypos + 14,
                                    kAngZero_706610,
                                    palette_types_enum::sprites_2,
                                    0,
                                    0,
                                    0);
        }
    }
    else
    {
        DrawFigureScaled_5D7670(sprite_types_enum::user_6, 120, xpos - 1, ypos + 3, kAngZero_706610, palette_types_enum::sprites_2, 0, 0, 0);
    }

    DrawFigureScaled_5D7670(sprite_types_enum::user_6,
                            123 + seconds / 10,
                            xpos + 4,
                            ypos + 3,
                            kAngZero_706610,
                            palette_types_enum::sprites_2,
                            0,
                            0,
                            0);

    DrawFigureScaled_5D7670(sprite_types_enum::user_6,
                            123 + seconds % 10,
                            xpos + 11,
                            ypos + 3,
                            kAngZero_706610,
                            palette_types_enum::sprites_2,
                            0,
                            0,
                            0);
}

// https://decomp.me/scratch/3IY3c
MATCH_FUNC(0x5d2ab0)
void Hud_Pager::DrawPager_5D2AB0(u32 xpos, s32 ypos)
{
    const s32 palette_type = palette_types_enum::sprites_2;
    if (field_0_timer < 0 && !field_4_ptr_counter)
    {
        return;
    }

    if (field_0_timer >= 0 && field_4_ptr_counter)
    {
        s32 top_height = get_sprite_height_4C7250(117);
        s32 bottom_height = get_sprite_height_4C7250(118);
        s32 middle_height = get_sprite_height_4C7250(119);

        DrawFigureScaled_5D7670(sprite_types_enum::user_6, 117, xpos, ypos - top_height / 2 - middle_height / 2, kAngZero_706610, palette_type, 0, 0, 0);

        // The middle sprite's y goes through the Fix16(u32) constructor (out-of-line copy 0x4926F0) like x does
        DrawFigureScaled_5D7670(sprite_types_enum::user_6, 119, xpos, static_cast<u32>(ypos), kAngZero_706610, palette_type, 0, 0, 0);

        DrawFigureScaled_5D7670(sprite_types_enum::user_6, 118, xpos, ypos + middle_height / 2 + bottom_height / 2, kAngZero_706610, palette_type, 0, 0, 0);
        Hud_Pager::DrawCounterDigits_5D2380(xpos, ypos - 6);
        Hud_Pager::DrawDigits_5D2680(xpos, ypos + 6);
    }
    else if (field_0_timer >= 0)
    {
        s32 top_height = get_sprite_height_4C7250(117);
        s32 bottom_height = get_sprite_height_4C7250(118);

        DrawFigureScaled_5D7670(sprite_types_enum::user_6, 117, xpos, ypos - top_height / 2, kAngZero_706610, palette_type, 0, 0, 0);

        DrawFigureScaled_5D7670(sprite_types_enum::user_6, 118, xpos, ypos + bottom_height / 2, kAngZero_706610, palette_type, 0, 0, 0);
        Hud_Pager::DrawDigits_5D2680(xpos, ypos);
    }
    else
    {
        s32 top_height = get_sprite_height_4C7250(117);
        s32 bottom_height = get_sprite_height_4C7250(118);

        DrawFigureScaled_5D7670(sprite_types_enum::user_6, 117, xpos, ypos - top_height / 2, kAngZero_706610, palette_type, 0, 0, 0);

        DrawFigureScaled_5D7670(sprite_types_enum::user_6, 118, xpos, ypos + bottom_height / 2, kAngZero_706610, palette_type, 0, 0, 0);
        Hud_Pager::DrawCounterDigits_5D2380(xpos, ypos);
    }
}

MATCH_FUNC(0x5d3040)
void Hud_Pager_C_Array::DrawPagers_5D3040()
{
    s32 totalSpriteHeight = get_sprite_height_4C7250(117) + get_sprite_height_4C7250(118) + get_sprite_height_4C7250(119);
    s32 width = (get_sprite_width_4C7220(117) / 2) + 3;

    s32 ypos = gGameSession_67E8E0.IsBonusStage_4C59A0() ? 36 : 104;
    for (s32 i = 0; i < GTA2_COUNTOF(field_0_pagers_array); i++)
    {
        field_0_pagers_array[i].DrawPager_5D2AB0(width, ypos);
        ypos += totalSpriteHeight;
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
        Hud_Pager* pPager = &field_0_pagers_array[id];
        if (!pPager->IsTimerOff_4C7170() || !pPager->no_ptr_counter_4C7160())
        {
            continue;
        }
        pPager->SetTimer_4C7120(30 * seconds);
        return id;
    }
    return -1;
}

MATCH_FUNC(0x5d3220)
s32 Hud_Pager_C_Array::AddOnScreenCounter_5D3220(s32* pCounter)
{
    s32 targetIdx = -1;
    for (s32 i = 0; i < GTA2_COUNTOF_S(field_0_pagers_array); i++)
    {
        Hud_Pager* pPager = &field_0_pagers_array[i];
        if (pPager->no_ptr_counter_4C7160() && (!pPager->IsTimerOff_4C7170() || targetIdx == -1))
        {
            targetIdx = i;
        }
    }

    field_0_pagers_array[targetIdx].SetCounter_4C7130(pCounter);

    return targetIdx;
}

MATCH_FUNC(0x5d3280)
void Hud_Pager_C_Array::ClearPager_5D3280(s32 idx)
{
    SoundObject* pSound = field_0_pagers_array[idx].field_8_sound;
    Hud_Pager* pPager = &field_0_pagers_array[idx];
    pPager->field_0_timer = -1;
    pPager->field_4_ptr_counter = NULL;

    if (pSound)
    {
        pSound->Release_40EF20();
        pSound->field_C_pAny.pNextFree = gRoot_sound_66B038.field_0_pFreeList;
        gRoot_sound_66B038.field_0_pFreeList = pSound;
        pPager->field_8_sound = NULL;
    }
}

MATCH_FUNC(0x5d32d0)
void Hud_Pager_C_Array::ClearClockOnly_5D32D0(s32 pager_idx)
{
    field_0_pagers_array[pager_idx].field_0_timer = -1;
}

MATCH_FUNC(0x5d32f0)
void Hud_Pager_C_Array::AddTime_5D32F0(s32 pager_idx, s32 time_to_add)
{
    Hud_Pager* pPager = &field_0_pagers_array[pager_idx];
    pPager->field_0_timer += time_to_add;
}

MATCH_FUNC(0x5d3310)
void Hud_Pager_C_Array::ClearCounterOnly_5D3310(s32 pager_idx)
{
    field_0_pagers_array[pager_idx].field_4_ptr_counter = NULL;
}

MATCH_FUNC(0x5d3330)
void Hud_Brief::MovePrevBriefToCurrent_5D3330()
{
    Hud_BriefEntry* pBrief = field_700_prev_briefs;
    field_700_prev_briefs = pBrief->field_C_pNext;
    pBrief->field_C_pNext = field_6F8_curr_briefs;
    field_6F8_curr_briefs = pBrief;
}

MATCH_FUNC(0x5d3350)
void Hud_Brief::FreeCurrentBrief_5D3350()
{
    Hud_BriefEntry* pBrief = this->field_6F8_curr_briefs;
    this->field_6F8_curr_briefs = pBrief->field_C_pNext;
    pBrief->field_C_pNext = this->field_6FC_free_briefs;
    this->field_6FC_free_briefs = pBrief;
}

MATCH_FUNC(0x5d3370)
void Hud_Brief::MoveCurrentBriefToPrev_5D3370()
{
    Hud_BriefEntry* pPrev = this->field_6F8_curr_briefs;
    this->field_6F8_curr_briefs = pPrev->field_C_pNext;
    pPrev->field_C_pNext = this->field_700_prev_briefs;
    this->field_700_prev_briefs = pPrev;
    pPrev->field_8_brief_priority = 0;
}

// ----------------------------------------------------

MATCH_FUNC(0x5d33a0)
void Hud_Brief::AppendCurrentBriefToPrev_5D33A0()
{
    Hud_BriefEntry* pBrief;
    for (pBrief = field_700_prev_briefs; pBrief->field_C_pNext; pBrief = pBrief->field_C_pNext)
    {
        ;
    }
    pBrief->field_C_pNext = field_6F8_curr_briefs;
    field_6F8_curr_briefs->field_8_brief_priority = 0;
    field_6F8_curr_briefs = field_6F8_curr_briefs->field_C_pNext;
    pBrief->field_C_pNext->field_C_pNext = NULL;
}

// https://decomp.me/scratch/L1e5G reg swap
MATCH_FUNC(0x5d33f0)
Hud_BriefEntry* Hud_Brief::AllocBrief_5D33F0()
{
    Hud_BriefEntry* pBrief = field_6FC_free_briefs;
    if (pBrief)
    {
        field_6FC_free_briefs = pBrief->field_C_pNext;
    }
    else
    {
        Hud_BriefEntry* pPrev;
        Hud_BriefEntry* pIter;
        pBrief = field_700_prev_briefs;
        if (field_700_prev_briefs)
        {
            pIter = field_700_prev_briefs->field_C_pNext;
            if (pIter)
            {
                do
                {
                    pPrev = pBrief;
                    pBrief = pBrief->field_C_pNext;
                } while (pBrief->field_C_pNext);
                if (pPrev)
                {
                    pPrev->field_C_pNext = NULL;
                }
                else
                {
                    field_700_prev_briefs = NULL;
                }
            }
            else
            {
                field_700_prev_briefs = NULL;
            }
        }
        else
        {
            pBrief = field_6F8_curr_briefs;
            pIter = pBrief->field_C_pNext;
            if (!pIter)
            {
                field_6F8_curr_briefs = NULL;
            }
            else
            {
                do
                {
                    pPrev = pBrief;
                    pBrief = pBrief->field_C_pNext;
                } while (pBrief->field_C_pNext);
                if (pPrev)
                {
                    pPrev->field_C_pNext = NULL;
                }
                else
                {
                    field_6F8_curr_briefs = NULL;
                }
            }
        }
    }
    return pBrief;
}

MATCH_FUNC(0x5d3470)
size_t Hud_Brief::FormatCurrentBrief_5D3470()
{
    size_t num_chars;
    char_type brief_face_idx;

    if (field_6F8_curr_briefs)
    {
        if (field_6F8_curr_briefs->field_14_cost_param != -1)
        {
            swprintf(tmpBuff_67BD9C,
                     gText_0x14_704DFC->Find_5B5F90(field_6F8_curr_briefs->field_0_brief_id_str),
                     field_6F8_curr_briefs->field_14_cost_param);
            brief_face_idx = GetBriefFaceIdx_5D3680(tmpBuff_67BD9C[0]);
            field_502_face_idx = brief_face_idx;

            wchar_t* pStartStr;
            if (brief_face_idx)
            {
                pStartStr = &tmpBuff_67BD9C[1]; // ignore next two chars
            }
            else
            {
                pStartStr = tmpBuff_67BD9C;
                field_502_face_idx = 8; // neutral face
            }

            field_508_num_lines = gText_0x14_704DFC->InsertLineBreaksAndGetNumLines_5B5BC0(field_0_str, pStartStr, MaxLineWidth_62689C, gBriefFont_7065C4);
            num_chars = wcslen(field_0_str);
            if (bShow_brief_number_67D504)
            {
                swprintf(tmpBuff_67BD9C,
                         gText_0x14_704DFC->Find_5B5F90(field_6F8_curr_briefs->field_0_brief_id_str),
                         field_6F8_curr_briefs->field_14_cost_param);
                brief_face_idx = GetBriefFaceIdx_5D3680(tmpBuff_67BD9C[0]);
                field_502_face_idx = brief_face_idx;

                wchar_t* pStartStr_2;
                if (brief_face_idx)
                {
                    pStartStr_2 = &tmpBuff_67BD9C[1]; // ignore next two chars
                }
                else
                {
                    field_502_face_idx = 8; // neutral face
                    pStartStr_2 = tmpBuff_67BD9C;
                }
                swprintf(gTmpWideStr_67C7D8, L"(%s)%s", TextTable::Ascii2Wide_5B5DF0(field_6F8_curr_briefs->field_0_brief_id_str), pStartStr_2);
                field_508_num_lines = TextTable::InsertLineBreaksAndGetNumLines_5B5BC0(field_0_str, gTmpWideStr_67C7D8, MaxLineWidth_62689C, gBriefFont_7065C4);
            }
        }
        else
        {
            wchar_t* pBriefText = gText_0x14_704DFC->Find_5B5F90(field_6F8_curr_briefs->field_0_brief_id_str);
            brief_face_idx = GetBriefFaceIdx_5D3680(pBriefText[0]);
            field_502_face_idx = brief_face_idx;

            if (brief_face_idx)
            {
                ++pBriefText; // ignore next two chars
            }
            else
            {
                field_502_face_idx = 8; // neutral face
            }
            field_508_num_lines = gText_0x14_704DFC->InsertLineBreaksAndGetNumLines_5B5BC0(field_0_str, static_cast<const wchar_t*>(pBriefText), MaxLineWidth_62689C, gBriefFont_7065C4);
            num_chars = wcslen(field_0_str);

            if (bShow_brief_number_67D504)
            {
                wchar_t* pString = gText_0x14_704DFC->Find_5B5F90(field_6F8_curr_briefs->field_0_brief_id_str);
                brief_face_idx = GetBriefFaceIdx_5D3680(pString[0]);
                field_502_face_idx = brief_face_idx;

                if (brief_face_idx)
                {
                    ++pString; // ignore next two chars
                }
                else
                {
                    field_502_face_idx = 8; // neutral face
                }
                swprintf(gTmpWideStr_67C7D8, L"(%s)%s", TextTable::Ascii2Wide_5B5DF0(field_6F8_curr_briefs->field_0_brief_id_str), pString);
                field_508_num_lines = TextTable::InsertLineBreaksAndGetNumLines_5B5BC0(field_0_str, gTmpWideStr_67C7D8, MaxLineWidth_62689C, gBriefFont_7065C4);
            }
        }
    }
    return num_chars;
}

MATCH_FUNC(0x5d3680)
char_type __stdcall GetBriefFaceIdx_5D3680(u16 face_char)
{
    switch (face_char)
    {
        case 8556: // "!l" = loonies
            return 1;
        case 8569: // "!y" = yakuza
            return 2;
        case 8570: // "!z" = zaibatsu
            return 3;
        case 8562: // "!r" = rednecks
            return 4;
        case 8563: // "!s" = scientists
            return 5;
        case 8555: // "!k" = krishna
            return 6;
        case 8557: // "!m" = mafia (russian)
            return 7;
        case 8558: // "!n" = neutral
            return 8;
        case 8560: // "!p" = police
            return 9;
        default:
            return 0;
    }
}

MATCH_FUNC(0x5d39d0)
void Hud_Brief::StartCurrentBrief_5D39D0()
{
    field_510_num_chars = Hud_Brief::FormatCurrentBrief_5D3470();
    field_504_tick_timer = field_510_num_chars * gHud_2B00_706620->field_13C4_text_speed;
    field_50C_face_variant = 0;
    field_514_face_anim_char_idx = 0;
    field_6F8_curr_briefs->field_10_was_displayed = 0;
}

// https://decomp.me/scratch/exFU8
MATCH_FUNC(0x5d3b80)
void Hud_Brief::DrawBrief_5D3B80()
{
    if (field_6F8_curr_briefs)
    {
        DrawFigureScaled_5D7670(sprite_types_enum::user_6,
                                field_50C_face_variant + 3 * field_502_face_idx + 16,
                                (32), // x
                                (443), // y
                                kAngZero_706610, // rot
                                palette_types_enum::sprites_2,
                                0,
                                0,
                                0);

        // u32: converts with the Fix16(u32) constructor, whose out-of-line copy is 0x4926F0
        u32 first_line_ypos = 480 - GetLineSpacingFromFontType_5D7700_inlined(gBriefFont_7065C4) * field_508_num_lines;
        DrawText_5D7720(field_0_str, // str
                        (64), // x
                        first_line_ypos, // y
                        gBriefFont_7065C4, // fontType
                        palette_types_enum::sprites_2,
                        0, // palette
                        0, // alpha
                        0); // alpha_flag
    }
}

MATCH_FUNC(0x5d3f10)
void Hud_Brief::SetHudBrief_5D3F10(s32 priority, const char_type* pText, s32 cost_param)
{
    Hud_BriefEntry* pNewBrief = AllocBrief_5D33F0();
    strcpy(pNewBrief->field_0_brief_id_str, pText);
    pNewBrief->field_8_brief_priority = priority;
    pNewBrief->field_10_was_displayed = 0;
    pNewBrief->field_14_cost_param = cost_param;

    if (!this->field_6F8_curr_briefs)
    {
        this->field_6F8_curr_briefs = pNewBrief;
        pNewBrief->field_C_pNext = NULL;
        StartCurrentBrief_5D39D0();
    }
    else if (this->field_6F8_curr_briefs->field_8_brief_priority >= priority && priority != hud_brief_priority::now_3)
    {
        Hud_BriefEntry* pIter = this->field_6F8_curr_briefs;
        while (pIter->field_C_pNext && pIter->field_C_pNext->field_8_brief_priority >= priority)
        {
            pIter = pIter->field_C_pNext;
        }

        Hud_BriefEntry* pNext = pIter->field_C_pNext;
        if (pNext)
        {
            pNewBrief->field_C_pNext = pNext;
            pIter->field_C_pNext = pNewBrief;
        }
        else
        {
            pIter->field_C_pNext = pNewBrief;
            pNewBrief->field_C_pNext = NULL;
        }
    }
    else
    {
        if (this->field_6F8_curr_briefs->field_10_was_displayed)
        {
            MoveCurrentBriefToPrev_5D3370();
        }
        pNewBrief->field_C_pNext = this->field_6F8_curr_briefs;
        this->field_6F8_curr_briefs = pNewBrief;
        StartCurrentBrief_5D39D0();
    }
}

// 9.6f 0x4CA610
inline void Hud_Arrow::Reset_4CA610()
{
    field_10_radius_pos = kFpZero_7064C0;
    field_14_reposition_speed = kArrowBaseRepositionSpeed_7063B0;
    SetArrowColour_5D0510(4);
    field_18_targets.field_10_gang_info.field_5_is_visible = true;
    field_18_targets.field_2C_arrow_sprt_idx = 0;
    field_18_targets.field_10_gang_info.field_30_gang = NULL;
    field_18_targets.field_10_gang_info.field_34_min_respect = 0;
}

// 9.6f 0x4C6FF0
inline void Hud_Arrow::SetMinRadiusPos_4C6FF0(s32 steps)
{
    field_C_min_radius_pos = kArrowRadiusOffset_7065B4 + kArrowMinRadiusStep_706338 * steps;
}

MATCH_FUNC(0x5d4400)
void Hud_Brief::SetHudBrief_5D4400(s32 priority, const char_type* pTextIdStr)
{
    SetHudBrief_5D3F10(priority, pTextIdStr, -1);
}

MATCH_FUNC(0x5d44d0)
void Hud_Brief::UpdateBrief_5D44D0()
{
    if (field_6F8_curr_briefs)
    {
        field_504_tick_timer--;

        if (!(field_504_tick_timer % 3))
        {
            field_514_face_anim_char_idx++;
            if (field_514_face_anim_char_idx == field_510_num_chars)
            {
                field_514_face_anim_char_idx = 0;
            }

            const s32 ary_val = field_0_str[field_514_face_anim_char_idx];
            if (!(ary_val % 20)) // (, <, P, d, x
            {
                field_50C_face_variant = 1; // blinking (closed eyes)
            }
            else
            {
                field_50C_face_variant = 2 * (ary_val % 2); // mouth open and closed, but with open eyes
            }
        }

        field_6F8_curr_briefs->field_10_was_displayed = 1;

        if (field_504_tick_timer == 0)
        {
            MoveCurrentBriefToPrev_5D3370();
            if (field_6F8_curr_briefs)
            {
                StartCurrentBrief_5D39D0();
            }
        }
    }
}

MATCH_FUNC(0x5d4850)
void Hud_Brief::ShowBrief_5D4850()
{
    if (field_700_prev_briefs)
    {
        Hud_BriefEntry* pCurrBrief = field_6F8_curr_briefs;
        if (pCurrBrief)
        {
            if (pCurrBrief->field_10_was_displayed)
            {
                Hud_Brief::AppendCurrentBriefToPrev_5D33A0();
            }
        }
        Hud_Brief::MovePrevBriefToCurrent_5D3330();
        Hud_Brief::StartCurrentBrief_5D39D0();
    }
}

// https://decomp.me/scratch/N327U
MATCH_FUNC(0x5d4890)
void Hud_Brief::ClearAllBriefsWithPriority_5D4890(s32 priority)
{
    Hud_BriefEntry* pLast = NULL;
    Hud_BriefEntry* pIter = field_6F8_curr_briefs;
    while (pIter)
    {
        if (pIter->field_8_brief_priority == priority)
        {
            if (pLast)
            {
                pLast->field_C_pNext = pIter->field_C_pNext;
                pIter->field_C_pNext = field_6FC_free_briefs;
                field_6FC_free_briefs = pIter;
                pIter = pLast->field_C_pNext;
            }
            else
            {
                if (field_6F8_curr_briefs->field_10_was_displayed)
                {
                    Hud_Brief::MoveCurrentBriefToPrev_5D3370();
                }
                else
                {
                    Hud_Brief::FreeCurrentBrief_5D3350();
                }
                pIter = field_6F8_curr_briefs;
                // Testing the field rather than pIter lets VC6 push ebp only after the first null check
                if (field_6F8_curr_briefs)
                {
                    Hud_Brief::StartCurrentBrief_5D39D0();
                }
            }
        }
        else
        {
            pLast = pIter;
            pIter = pIter->field_C_pNext;
        }
    }
}

MATCH_FUNC(0x5d4930)
Hud_Brief::Hud_Brief()
{
    field_6FC_free_briefs = &field_518_briefs[0];

    field_50C_face_variant = 0;
    field_510_num_chars = 0;
    field_514_face_anim_char_idx = 0;
    field_6F8_curr_briefs = NULL;
    field_700_prev_briefs = NULL;
    field_504_tick_timer = 0;

    for (s32 i = 0; i < GTA2_COUNTOF(field_518_briefs) - 1; i++)
    {
        field_518_briefs[i].field_C_pNext = &field_518_briefs[i + 1];
    }

    field_518_briefs[GTA2_COUNTOF(field_518_briefs) - 1].field_C_pNext = NULL;
}

MATCH_FUNC(0x5d4a10)
void Hud_CarName::DrawCarName_5D4A10()
{
    if (field_0_display_time)
    {
        // u32: converts with the Fix16(u32) constructor, whose out-of-line copy is 0x4926F0
        u32 sprite_w = get_sprite_width_4C7220(11);
        if (field_44_text_width > static_cast<s32>(sprite_w * 2 - 10))
        {
            DrawFigureScaled_5D7670(sprite_types_enum::user_6,
                                    13,
                                    320 + sprite_w,
                                    static_cast<u32>(field_48_ypos),
                                    kAngZero_706610,
                                    palette_types_enum::sprites_2,
                                    0,
                                    0,
                                    0);
            DrawFigureScaled_5D7670(sprite_types_enum::user_6,
                                    12,
                                    320,
                                    static_cast<u32>(field_48_ypos),
                                    kAngZero_706610,
                                    palette_types_enum::sprites_2,
                                    0,
                                    0,
                                    0);
            DrawFigureScaled_5D7670(sprite_types_enum::user_6,
                                    11,
                                    320 - sprite_w,
                                    static_cast<u32>(field_48_ypos),
                                    kAngZero_706610,
                                    palette_types_enum::sprites_2,
                                    0,
                                    0,
                                    0);
        }
        else
        {
            DrawFigureScaled_5D7670(sprite_types_enum::user_6,
                                    11,
                                    320 - (static_cast<s32>(sprite_w) / 2),
                                    static_cast<u32>(field_48_ypos),
                                    kAngZero_706610,
                                    palette_types_enum::sprites_2,
                                    0,
                                    0,
                                    0);
            DrawFigureScaled_5D7670(sprite_types_enum::user_6,
                                    13,
                                    320 + (static_cast<s32>(sprite_w) / 2),
                                    static_cast<u32>(field_48_ypos),
                                    kAngZero_706610,
                                    palette_types_enum::sprites_2,
                                    0,
                                    0,
                                    0);
        }

        DrawTextScaled_5D77A0(field_2_car_name, ((640 - field_44_text_width) / 2), (field_48_ypos - GetLineSpacingFromFontType_5D7700(gCarNameFont_706508) / 2), gCarNameFont_706508);
    }
}

MATCH_FUNC(0x5d5190)
void Hud::CalcCarNameTextWidth_5D5190()
{
    if (field_0_car_name.field_0_display_time)
    {
        field_0_car_name.field_44_text_width = Frontend::GetMaxTextWidth_5D8990(field_0_car_name.field_2_car_name, gCarNameFont_706508);
    }
}

MATCH_FUNC(0x5d5240)
void Hud::ShowCarName_5D5240(const wchar_t* pName)
{
    field_0_car_name.field_0_display_time = 120;
    wcscpy(field_0_car_name.field_2_car_name, pName);
    gText_0x14_704DFC->StrToUpper_5B5B80(field_0_car_name.field_2_car_name);
    Hud::CalcCarNameTextWidth_5D5190();
    field_0_car_name.field_48_ypos = -17;
}

// ----------------------------------------------------

MATCH_FUNC(0x5d5350)
void Hud::UpdateCarName_5D5350()
{
    Hud_CarName* pCarName = &field_0_car_name;
    if (pCarName->field_0_display_time)
    {
        pCarName->field_0_display_time--;
        if (pCarName->field_0_display_time > 80u)
        {
            ++pCarName->field_48_ypos;
        }
        else if (pCarName->field_0_display_time < 40u)
        {
            --pCarName->field_48_ypos;
        }
    }
}

MATCH_FUNC(0x5d53b0)
Hud_CarName::Hud_CarName()
{
    field_0_display_time = 0;
}

MATCH_FUNC(0x5d53e0)
void Hud_PickupText::CalcTextWidth_5D53E0()
{
    if (field_0_timer)
    {
        field_84_text_width = Frontend::GetMaxTextWidth_5D8990(field_2_str, gPickupTextFont_7064B8);
    }
}

MATCH_FUNC(0x5d5420)
void Hud_PickupText::Draw_5D5420()
{
    if (field_0_timer)
    {
        DrawText_5D7720(field_2_str, (640 - field_84_text_width) / 2, 32, gPickupTextFont_7064B8, palette_types_enum::font_remaps_8, 5, 0, 0);
    }
}

MATCH_FUNC(0x5d5600)
void Hud_PickupText::ShowPickupText_5D5600(u8 pickup_idx)
{
    sprintf(gTmpGxtKey_67CE50, "c%02d", pickup_idx);
    field_0_timer = 90;
    wchar_t* pStr = gText_0x14_704DFC->Find_5B5F90(gTmpGxtKey_67CE50);
    wcscpy(field_2_str, pStr);
    CalcTextWidth_5D53E0();
}

MATCH_FUNC(0x5d5690)
void Hud_PickupText::DecrementTimer_5D5690()
{
    if (field_0_timer)
    {
        field_0_timer--;
    }
}

MATCH_FUNC(0x5d56a0)
Hud_PickupText::Hud_PickupText()
{
    field_0_timer = 0;
}

MATCH_FUNC(0x5d56b0)
void Hud_MpMessage::CalcTextWidth_5D56B0()
{
    if (this->field_0_timer)
    {
        this->field_CC_text_width = Frontend::GetMaxTextWidth_5D8990(field_2_str, word_7064D8);
    }
}

//https://decomp.me/scratch/QUvWb
MATCH_FUNC(0x5d56d0)
void Hud_MpMessage::Draw_5D56D0()
{
    if (this->field_0_timer)
    {
        DrawText_5D7720(this->field_2_str,
                        static_cast<u32>((640 - this->field_CC_text_width) / 2),
                        static_cast<u32>(16),
                        word_7064D8,
                        palette_types_enum::font_remaps_8,
                        5,
                        0,
                        0);
    }
}

MATCH_FUNC(0x5d5730)
void Hud_MpMessage::ShowText_5D5730(const wchar_t* pStr)
{
    this->field_0_timer = 120;
    wcscpy(this->field_2_str, pStr);
    CalcTextWidth_5D56B0();
}

MATCH_FUNC(0x5d5760)
void Hud_MpMessage::DecrementTimer_5D5760()
{
    if (field_0_timer)
    {
        field_0_timer--;
    }
}

//https://decomp.me/scratch/3hVt8
MATCH_FUNC(0x5d5770)
void Hud_MpMessage::AnnounceKill_5D5770(Player* killer, Player* victim)
{
    if (killer->IsUser_41DC70())
    {
        if (victim->IsUser_41DC70())
        {
            swprintf(tmpBuff_67BD9C, gText_0x14_704DFC->Find_5B5F90("mpkill1"));
            this->ShowText_5D5730(tmpBuff_67BD9C);
            return;
        }

        swprintf(tmpBuff_67BD9C, L"%s %s", gText_0x14_704DFC->Find_5B5F90("mpkill2"), victim->field_83C_player_name);

        gRoot_sound_66B038.PlayVoice_40F090(voice_line::random_laugh_32);
    }
    else if (victim->IsUser_41DC70())
    {
        swprintf(tmpBuff_67BD9C, L"%s %s", killer->field_83C_player_name, gText_0x14_704DFC->Find_5B5F90("mpkill3"));
    }
    else if (victim == killer)
    {
        swprintf(tmpBuff_67BD9C, L"%s %s", victim->field_83C_player_name, gText_0x14_704DFC->Find_5B5F90("mpkill5"));
    }
    else
    {
        swprintf(tmpBuff_67BD9C,
                 L"%s %s %s",
                 killer->field_83C_player_name,
                 gText_0x14_704DFC->Find_5B5F90("mpkill4"),
                 &victim->field_83C_player_name);

        gRoot_sound_66B038.PlayVoice_40F090(voice_line::random_laugh_31);
    }

    this->ShowText_5D5730(tmpBuff_67BD9C);
}

MATCH_FUNC(0x5d58f0)
Hud_MpMessage::Hud_MpMessage()
{
    field_0_timer = 0;
}

MATCH_FUNC(0x5d5900)
void Hud_MapZone::DrawZoneName_5D5900()
{
    if (field_0_timer)
    {
        s32 width = get_sprite_width_4C7220(159);

        DrawFigureScaled_5D7670(sprite_types_enum::user_6,
                                159,
                                static_cast<u32>(320 - (width / 2) - width),
                                static_cast<u32>(27),
                                kAngZero_706610,
                                palette_types_enum::sprites_2,
                                0,
                                field_90_alpha_flag,
                                field_94_transparency);

        DrawFigureScaled_5D7670(sprite_types_enum::user_6,
                                160,
                                320 - (width / 2),
                                static_cast<u32>(27),
                                kAngZero_706610,
                                palette_types_enum::sprites_2,
                                0,
                                field_90_alpha_flag,
                                field_94_transparency);

        DrawFigureScaled_5D7670(sprite_types_enum::user_6,
                                161,
                                (width / 2) + 320,
                                static_cast<u32>(27),
                                kAngZero_706610,
                                palette_types_enum::sprites_2,
                                0,
                                field_90_alpha_flag,
                                field_94_transparency);

        DrawFigureScaled_5D7670(sprite_types_enum::user_6,
                                162,
                                static_cast<u32>((width / 2) + width + 320),
                                static_cast<u32>(27),
                                kAngZero_706610,
                                palette_types_enum::sprites_2,
                                0,
                                field_90_alpha_flag,
                                field_94_transparency);

        DrawText_5D7720(field_2_wstr,
                        (640 - field_84_text_width) / 2,
                        27 - (GetLineSpacingFromFontType_5D7700(gZoneNameFont_706618) / 2),
                        gZoneNameFont_706618,
                        palette_types_enum::sprites_2,
                        0,
                        field_90_alpha_flag,
                        field_94_transparency);
    }
}

MATCH_FUNC(0x5d5ad0)
void Hud_MapZone::CalcTextWidth_5D5AD0()
{
    if (field_0_timer)
    {
        field_84_text_width = Frontend::GetMaxTextWidth_5D8990(field_2_wstr, gZoneNameFont_706618);
    }
}

// ----------------------------------------------------

MATCH_FUNC(0x5d5af0)
void Hud_MapZone::ShowZoneName_5D5AF0(gmp_map_zone* pZone1, gmp_map_zone* pZone2)
{
    gmp_map_zone* pNameZone = pZone2;
    if (!pZone2)
    {
        pNameZone = pZone1;
    }

    wchar_t* pStr = pNameZone->get_zone_str_4DEF00();
    if (pStr)
    {
        const wchar_t* pName = gText_0x14_704DFC->StrToUpper_5B5B80(pStr);
        wcscpy(this->field_2_wstr, pName);
        this->field_88_nav_zone = pZone1;
        this->field_8C_local_nav_zone = pZone2;
        this->field_0_timer = 90;
        CalcTextWidth_5D5AD0();
        this->field_90_alpha_flag = 1;
        this->field_94_transparency = 0;
    }
}

MATCH_FUNC(0x5d5b60)
void Hud_MapZone::UpdateZoneName_5D5B60()
{
    u8 x;
    u8 y;
    u8 z;

    gGame_0x40_67E008->field_38_local_player->GetPosU8_569840(x, y, z);
    gmp_map_zone* navigation_zone = gMap_0x370_6F6268->GetZoneAtPosOfType_4DF4D0(x, y, Navigation_1);
    gmp_map_zone* local_navigation_zone = gMap_0x370_6F6268->GetZoneAtPosOfType_4DF4D0(x, y, Local_Navigation_15);

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
                        field_90_alpha_flag = 0;
                        field_94_transparency = 31;
                    }
                }
                else
                {
                    if (field_0_timer < 0x1Fu)
                    {
                        field_90_alpha_flag = 1;
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
            Hud_MapZone::ShowZoneName_5D5AF0(navigation_zone, local_navigation_zone);
        }
    }
    else
    {
        field_0_timer = 0;
        field_88_nav_zone = NULL;
    }
}

MATCH_FUNC(0x5d5c50)
void Hud_MapZone::ResetTransparency_5D5C50()
{
    field_90_alpha_flag = 0;
    field_94_transparency = 0;
}

MATCH_FUNC(0x5d5c60)
Hud_MapZone::Hud_MapZone()
{
    field_0_timer = 0;
    field_88_nav_zone = NULL;
    field_8C_local_nav_zone = NULL;
}

MATCH_FUNC(0x5d5c80)
void Hud_PlayerStats::DrawPlayerStats_5D5C80()
{
    Player* pPlayer = gGame_0x40_67E008->field_38_local_player;

    s16 ammo_idx = pPlayer->field_788_curr_weapon_idx;
    if (ammo_idx == -1)
    {
        DrawAmmo_5D6060(-1, 0);
    }
    else
    {
        u8 ammo = pPlayer->field_718_weapons[ammo_idx]->get_ammo_4A4FB0();
        DrawAmmo_5D6060(ammo_idx, ammo);
    }

    s32 powerup_xpos = 639;
    for (s32 powerup_idx = 0; powerup_idx < 17; powerup_idx++)
    {
        u16 powerup_timer = pPlayer->field_6F4_power_up_timers[powerup_idx];
        if (powerup_timer)
        {
            powerup_xpos = DrawPlayerStatsHelper_5D61A0(powerup_idx, powerup_xpos, powerup_timer);
        }
    }

    RollingDigitCounter* pScoreDigits = pPlayer->field_2D4_scores.GetScoreDigits_592360();
    s32 dollar_sign_xpos = pScoreDigits->DrawDigitsRightAligned_492260(639, 4);

    // Now draw $ symbol

    if (bStartNetworkGame_7081F0)
    {
        DrawFigureScaled_5D7670(sprite_types_enum::user_6,
                                16,
                                dollar_sign_xpos - 8,
                                14,
                                kAngZero_706610,
                                pPlayer->field_78C_hud_palette_type,
                                pPlayer->field_790_hud_palette,
                                0,
                                0);
    }
    else
    {
        if (pPlayer->field_60_finish_score_reached == 0)
        {
            DrawFigureScaled_5D7670(sprite_types_enum::user_6,
                                    16,
                                    dollar_sign_xpos - 8,
                                    14,
                                    kAngZero_706610,
                                    palette_types_enum::sprites_2,
                                    0,
                                    0,
                                    0); // default color
        }
        else
        {
            DrawFigureScaled_5D7670(sprite_types_enum::user_6,
                                    16,
                                    dollar_sign_xpos - 8,
                                    14,
                                    kAngZero_706610,
                                    palette_types_enum::user_remaps_7,
                                    6,
                                    0,
                                    0); // red color
        }
    }

    wchar_t buffer[16];

    if (bStartNetworkGame_7081F0)
    {
        if (IsTagGame_434B20())
        {
            swprintf(buffer,
                     L"%2d:%02d",
                     gTagGame_6F8450.GetPlayerTime_4C7380(pPlayer) / 60,
                     gTagGame_6F8450.GetPlayerTime_4C7380(pPlayer) % 60);

            const s32 text_palette_type = (pPlayer->field_78C_hud_palette_type != palette_types_enum::user_remaps_7) ? palette_types_enum::sprites_2 : palette_types_enum::font_remaps_8;
            DrawText_5D7720(buffer, 420, 4, gStyleTextFont_703BAA, text_palette_type, pPlayer->field_790_hud_palette - 1, 0, 0);
        }
        else
        {
            RollingDigitCounter* pFragDigits = pPlayer->field_2D4_scores.GetMultiplayerFragDigits_5935B0();
            pFragDigits->DrawDigitsRightAligned_492260(490, 4);
        }

        s32 ypos = 8;
        for (Player* pMultiPlayer = gGame_0x40_67E008->IterateFirstPlayer_4B9CD0(); pMultiPlayer != NULL;
             pMultiPlayer = gGame_0x40_67E008->IterateNextPlayer_4B9D10())
        {
            if (!pMultiPlayer->IsUser_41DC70())
            {
                RollingDigitCounter* pMultiScoreDigits = pMultiPlayer->field_2D4_scores.GetScoreDigits_592360();
                s32 score_end_xpos = pMultiScoreDigits->DrawDigitsLeftAligned_492430(16, ypos);

                DrawFigureScaled_5D7670(sprite_types_enum::user_6,
                                        16,
                                        8,
                                        ypos + 10,
                                        kAngZero_706610,
                                        pMultiPlayer->field_78C_hud_palette_type,
                                        pMultiPlayer->field_790_hud_palette,
                                        0,
                                        0);

                if (IsTagGame_434B20())
                {
                    swprintf(buffer,
                             L"%2d:%02d",
                             gTagGame_6F8450.GetPlayerTime_4C7380(pMultiPlayer) / 60,
                             gTagGame_6F8450.GetPlayerTime_4C7380(pMultiPlayer) % 60);

                    const s32 text_palette_type = (pMultiPlayer->field_78C_hud_palette_type != palette_types_enum::user_remaps_7) ? palette_types_enum::sprites_2 : palette_types_enum::font_remaps_8;
                    DrawText_5D7720(buffer, score_end_xpos + 20, static_cast<u32>(ypos), gStyleTextFont_703BAA, text_palette_type, pMultiPlayer->field_790_hud_palette - 1, 0, 0);
                }
                else
                {
                    RollingDigitCounter* pFragDigits = pMultiPlayer->field_2D4_scores.GetMultiplayerFragDigits_5935B0();
                    pFragDigits->DrawDigitsLeftAligned_492430(score_end_xpos + 20, ypos);
                }
                ypos += 27;
            }
        }
    }
    else
    {
        s32 lives_xpos = pPlayer->field_684_lives.DrawDigitsRightAligned_492260(523, 28);
        DrawFigureScaled_5D7670(sprite_types_enum::user_6, 17, lives_xpos - 7, 32, kAngZero_706610, palette_types_enum::sprites_2, 0, 0, 0);

        s32 multiplier_xpos = pPlayer->field_6BC_multipliers.DrawDigitsRightAligned_492260(523, 11);
        DrawFigureScaled_5D7670(sprite_types_enum::user_6,
                                18,
                                multiplier_xpos - 7,
                                18,
                                kAngZero_706610,
                                palette_types_enum::sprites_2,
                                0,
                                0,
                                0);
    }
}

MATCH_FUNC(0x5D6060)
void __stdcall DrawAmmo_5D6060(s16 ammo_idx, u8 ammo_count)
{
    if (ammo_idx != -1)
    {
        s32 width = get_sprite_width_4C7220(ammo_idx + 85);
        s32 height = get_sprite_height_4C7250(ammo_idx + 85);

        DrawFigureScaled_5D7670(sprite_types_enum::user_6,
                                ammo_idx + 85,
                                638 - width / 2,
                                height / 2 + 44,
                                kAngZero_706610,
                                palette_types_enum::sprites_2,
                                0,
                                0,
                                0);

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
                            static_cast<u32>(638 - Frontend::GetMaxTextWidth_5D8990(tmpBuff_67BD9C, gPlayerStatsFont_70646C)),
                            82,
                            gPlayerStatsFont_70646C,
                            palette_types_enum::font_remaps_8,
                            6,
                            0,
                            0);
        }
    }
}

// https://decomp.me/scratch/1IbxU
// The original passes the gtx pointer through an inlined helper: its parameter is one more live range in
// the first block, which tips the register priorities so width gets ebx and base_xpos ebp
static inline u16 GetPowerupSpriteIndex_5D61A0(gtx_0x106C* pGtx, s32 sprite_pal)
{
    return pGtx->GetSpriteTrueIndex_5AA460(sprite_types_enum::user_6, sprite_pal);
}

MATCH_FUNC(0x5D61A0)
s32 __stdcall DrawPlayerStatsHelper_5D61A0(s32 powerup_idx, s32 base_xpos, u16 optional_number)
{
    u16 sprite_idx = GetPowerupSpriteIndex_5D61A0(gGtx_0x106C_703DD4, powerup_idx + 141);
    s32 width = gGtx_0x106C_703DD4->get_sprite_width_420220(sprite_idx);
    DrawFigureScaled_5D7670(sprite_types_enum::user_6,
                            powerup_idx + 141,
                            base_xpos - (width / 2),
                            117,
                            kAngZero_706610,
                            palette_types_enum::sprites_2,
                            0,
                            0,
                            0);

    if (powerup_idx == power_up_indices::Armor_3)
    {
        swprintf(tmpBuff_67BD9C, L"%d", optional_number);
        // The x goes through the Fix16(u32) constructor (out-of-line copy 0x4926F0). The offset is
        // computed before the y constructor call, so it is a local
        s32 x_offset = optional_number < 10 ? 18 : 22;
        DrawText_5D7720(tmpBuff_67BD9C,
                        static_cast<u32>(base_xpos - x_offset),
                        127,
                        gPlayerStatsFont_70646C,
                        palette_types_enum::font_remaps_8,
                        6,
                        0,
                        0);
    }
    return base_xpos - width;
}

// ----------------------------------------------------

MATCH_FUNC(0x5d6290)
void Hud_PlayerStats::UpdateRollingDigits_5D6290()
{
    Player* pPlayerIter = gGame_0x40_67E008->IterateFirstPlayer_4B9CD0();
    while (pPlayerIter)
    {
        RollingDigitCounter* pScoreDigits = pPlayerIter->field_2D4_scores.GetScoreDigits_592360();
        pScoreDigits->UpdateRollingDigits_4925E0();
        RollingDigitCounter* pFragDigits = pPlayerIter->field_2D4_scores.GetMultiplayerFragDigits_5935B0();
        pFragDigits->UpdateRollingDigits_4925E0();
        pPlayerIter = gGame_0x40_67E008->IterateNextPlayer_4B9D10();
    }
    Player* pPlayer = gGame_0x40_67E008->field_38_local_player;
    pPlayer->field_684_lives.UpdateRollingDigits_4925E0();
    pPlayer->field_6BC_multipliers.UpdateRollingDigits_4925E0();
}

// ----------------------------------------------------

// Not in IDA's function list: only reached through the UpdatePauseSection_5D69C0 thunk below.
MATCH_FUNC(0x5D6300)
void Hud_PauseScreen::UpdatePauseSection_5D6300()
{
    if (!gGameSession_67E8E0.IsBonusStage_4C59A0())
    {
        field_1_timer--;
        if (field_1_timer == 0)
        {
            field_1_timer = 45;
            field_0_current_pause_section++;

            if (field_0_current_pause_section > 6)
            {
                field_0_current_pause_section = 0;
            }

            if (field_0_current_pause_section == HudPauseSection::gang_1_missions_done_1)
            {
                if (!gGangPool_CA8_67E274->FirstGang_4BECA0())
                {
                    field_0_current_pause_section = HudPauseSection::all_missions_done_4;
                }
                return;
            }

            if (field_0_current_pause_section == HudPauseSection::gang_2_missions_done_2)
            {
                if (gGangPool_CA8_67E274->FirstGang_4BECA0())
                {
                    if (!gGangPool_CA8_67E274->NextGang_4BECE0())
                    {
                        field_0_current_pause_section = HudPauseSection::all_missions_done_4;
                    }
                    return;
                }
                field_0_current_pause_section = HudPauseSection::all_missions_done_4;
                return;
            }

            if (field_0_current_pause_section == HudPauseSection::gang_3_missions_done_3 &&
                (!gGangPool_CA8_67E274->FirstGang_4BECA0() || !gGangPool_CA8_67E274->NextGang_4BECE0() || !gGangPool_CA8_67E274->NextGang_4BECE0()))
            {
                field_0_current_pause_section = HudPauseSection::all_missions_done_4;
                return;
            }
        }
    }
}

// https://decomp.me/scratch/Uq97l
MATCH_FUNC(0x5d63b0)
void Hud_PauseScreen::DrawPause_5D63B0()
{

    u32 sprite_type;
    u16 sprite_pal = 0;
    if (gGame_0x40_67E008->IsPaused_416BC0() && !gGame_0x40_67E008->field_38_local_player->field_78A_show_quit_message)
    {
        DrawFigureScaled_5D7670(sprite_types_enum::user_6, 134, 227, 180, kAngZero_706610, palette_types_enum::sprites_2, 0, 0, 0);
        DrawFigureScaled_5D7670(sprite_types_enum::user_6, 136, 320, 180, kAngZero_706610, palette_types_enum::sprites_2, 0, 0, 0);
        DrawFigureScaled_5D7670(sprite_types_enum::user_6, 135, 413, 180, kAngZero_706610, palette_types_enum::sprites_2, 0, 0, 0);

        wchar_t* pPauseText = gText_0x14_704DFC->Find_5B5F90("pause");
        s32 max_width = Frontend::GetMaxTextWidth_5D8990(pPauseText, gPauseFont_7063F8);

        // u32: converts with the Fix16(u32) constructor, whose out-of-line copy is 0x4926F0
        u32 y_offset = !gText_0x14_704DFC->LangIsJapanese_452E60() ? 158 : 164;

        DrawText_5D7720(pPauseText, (640 - max_width) / 2, y_offset, gPauseFont_7063F8, palette_types_enum::sprites_2, 0, 0, 0);

        if (!gGameSession_67E8E0.IsBonusStage_4C59A0())
        {
            s32 passed_count;
            Gang* pGang;
            switch (field_0_current_pause_section)
            {
                case HudPauseSection::target_score_0:
                    swprintf(tmpBuff_67BD9C, gText_0x14_704DFC->Find_5B5F90("pscore"), gScriptManager_6F8060->field_310_finish_score);
                    break;

                case HudPauseSection::gang_1_missions_done_1:
                    pGang = gGangPool_CA8_67E274->FirstGang_4BECA0();
                    if (gScriptManager_6F8060->field_32C_1_passed_flag)
                    {
                        passed_count = *gScriptManager_6F8060->field_32C_1_passed_flag;
                    }
                    else
                    {
                        passed_count = 0;
                    }
                    swprintf(tmpBuff_67BD9C,
                             gText_0x14_704DFC->Find_5B5F90("pgmiss"),
                             pGang->GetArrowColourText_4BF340(),
                             passed_count,
                             gScriptManager_6F8060->field_31C_gang_1_missions_total);
                    sprite_type = sprite_types_enum::user_6;
                    sprite_pal = pGang->field_138_arrow_colour + 63;
                    break;

                case HudPauseSection::gang_2_missions_done_2:
                    gGangPool_CA8_67E274->FirstGang_4BECA0();
                    pGang = gGangPool_CA8_67E274->NextGang_4BECE0();
                    if (gScriptManager_6F8060->field_330_2_passed_flag)
                    {
                        passed_count = *gScriptManager_6F8060->field_330_2_passed_flag;
                    }
                    else
                    {
                        passed_count = 0;
                    }
                    swprintf(tmpBuff_67BD9C,
                             gText_0x14_704DFC->Find_5B5F90("pgmiss"),
                             pGang->GetArrowColourText_4BF340(),
                             passed_count,
                             gScriptManager_6F8060->field_320_gang_2_missions_total);
                    sprite_type = sprite_types_enum::user_6;
                    sprite_pal = pGang->field_138_arrow_colour + 63;
                    break;

                case HudPauseSection::gang_3_missions_done_3:
                    gGangPool_CA8_67E274->FirstGang_4BECA0();
                    gGangPool_CA8_67E274->NextGang_4BECE0();
                    pGang = gGangPool_CA8_67E274->NextGang_4BECE0();
                    if (gScriptManager_6F8060->field_334_3_passed_flag)
                    {
                        passed_count = *gScriptManager_6F8060->field_334_3_passed_flag;
                    }
                    else
                    {
                        passed_count = 0;
                    }
                    swprintf(tmpBuff_67BD9C,
                             gText_0x14_704DFC->Find_5B5F90("pgmiss"),
                             pGang->GetArrowColourText_4BF340(),
                             passed_count,
                             gScriptManager_6F8060->field_324_gang_3_missions_total);
                    sprite_type = sprite_types_enum::user_6;
                    sprite_pal = pGang->field_138_arrow_colour + 63;
                    break;

                case HudPauseSection::all_missions_done_4:
                    if (gScriptManager_6F8060->field_328_passed_flag)
                    {
                        passed_count = *gScriptManager_6F8060->field_328_passed_flag;
                    }
                    else
                    {
                        passed_count = 0;
                    }
                    swprintf(tmpBuff_67BD9C,
                             gText_0x14_704DFC->Find_5B5F90("pmiss"),
                             passed_count,
                             gScriptManager_6F8060->field_314_total_missions);
                    break;

                case HudPauseSection::kill_frenzies_completed_5:
                    if (gScriptManager_6F8060->field_338_secrets_passed)
                    {
                        passed_count = *gScriptManager_6F8060->field_338_secrets_passed;
                    }
                    else
                    {
                        passed_count = 0;
                    }
                    swprintf(tmpBuff_67BD9C,
                             gText_0x14_704DFC->Find_5B5F90("psec"),
                             passed_count,
                             gScriptManager_6F8060->field_318_total_secrets);
                    sprite_type = sprite_types_enum::code_obj1_4;
                    sprite_pal = gObjectDefinitions_6FCF00->GetObjectBaseSpriteIdx_4C6E30(objects::kill_frenzy_icon_286);
                    break;

                case HudPauseSection::tokens_collected_6:
                    swprintf(tmpBuff_67BD9C,
                             gText_0x14_704DFC->Find_5B5F90("pbon"),
                             gGameSession_67E8E0.get_secret_tokens_collected_453A80(),
                             50);
                    sprite_type = sprite_types_enum::code_obj1_4;
                    sprite_pal = gObjectDefinitions_6FCF00->GetObjectBaseSpriteIdx_4C6E30(objects::secret_token_266);
                    break;
                default:
                    break;
            }

            s32 max_width_2 = Frontend::GetMaxTextWidth_5D8990(tmpBuff_67BD9C, word_7064D8);
            s32 text_xpos;
            if (sprite_pal != 0)
            {
                u16 sprite_idx = gGtx_0x106C_703DD4->GetSpriteTrueIndex_5AA460(sprite_type, sprite_pal);
                s32 icon_width = gGtx_0x106C_703DD4->get_sprite_width_420220(sprite_idx) + 10;
                text_xpos = (640 - icon_width - max_width_2) / 2;
                DrawFigureScaled_5D7670(sprite_type,
                                        sprite_pal,
                                        text_xpos + icon_width / 2,
                                        235,
                                        kAngZero_706610,
                                        palette_types_enum::sprites_2,
                                        0,
                                        0,
                                        0);
                text_xpos += icon_width;
            }
            else
            {
                text_xpos = (640 - max_width_2) / 2;
            }
            // (u32): Fix16(u32) constructor (0x4926F0), the 220 goes through Fix16(s32) (0x4369F0)
            DrawText_5D7720(tmpBuff_67BD9C, static_cast<u32>(text_xpos), 220, word_7064D8, palette_types_enum::font_remaps_8, 6, 0, 0);
        }
    }
}

MATCH_FUNC(0x5d6860)
void Hud::DrawGui_5D6860()
{
    if (!bSkip_user_67D506)
    {
        SetFullAmbient_5D6A70();
        field_1118_player_stats.DrawPlayerStats_5D5C80();
        field_110C_under_roof_arrow_marker.Draw_5CF910();
        field_13C0_player_names.DrawPlayerNames_5CFE40();
        field_1028_wanted_level.DrawWantedLevel_5D0110();
        field_107C_gang_respect_bars.DrawGangRespectBars_5CFA70();
        field_1108_health.DrawHealth_5D0260();
        field_4C_zone_name.DrawZoneName_5D5900();
        field_0_car_name.DrawCarName_5D4A10();
        field_1080_pickup_text.Draw_5D5420();
        field_DC_brief.DrawBrief_5D3B80();
        field_620_pagers.DrawPagers_5D3040();
        field_650_texts.Service_5D2010();
        field_1F18_arrows.DrawArrows_5D0E90();
        field_12F0_mp_message.Draw_5D56D0();
        field_111C_message.DrawMessage_5D1940();
        field_12E4_pause_screen.DrawPause_5D63B0();
        field_2A25_chat_input.DrawChatMessages_5D16B0();
        field_12EC_quit_message.DrawQuitMessage_5D1430();
    }
}

MATCH_FUNC(0x5d69c0)
void Hud::UpdatePauseSection_5D69C0()
{
    field_12E4_pause_screen.UpdatePauseSection_5D6300();
}

MATCH_FUNC(0x5d69d0)
void Hud::UpdateHUD_5D69D0()
{
    field_1118_player_stats.UpdateRollingDigits_5D6290();
    field_110C_under_roof_arrow_marker.Update_5CF730();
    field_27B5_show_coords.ShowPlayerCoords_5CF970();
    field_1028_wanted_level.UpdateWantedLevel_5D00B0();
    UpdateCarName_5D5350();
    field_1080_pickup_text.DecrementTimer_5D5690();
    field_4C_zone_name.UpdateZoneName_5D5B60();
    field_DC_brief.UpdateBrief_5D44D0();
    field_620_pagers.UpdatePagers_5D31B0();
    field_650_texts.RemoveExpired_5D2050();
    field_1F18_arrows.UpdateArrows_5D0FD0();
    field_107C_gang_respect_bars.Empty_5CFE20();
    field_111C_message.DecrementTimeToShow_5D1AB0();
    field_12F0_mp_message.DecrementTimer_5D5760();
}

MATCH_FUNC(0x5d6a70)
void Hud::SetFullAmbient_5D6A70()
{
    if (gLighting_626A09)
    {
        pgbh_SetAmbient(1.0);
    }
}

MATCH_FUNC(0x5d6a90)
void Hud::GetTextSpeed_5D6A90()
{
    field_13C4_text_speed = gRegistry_6FF968.Get_Option_Int_586F70("text_speed", 3);
}

MATCH_FUNC(0x5d6ab0)
void Hud::RecalcTextLayout_5D6AB0()
{
    SetFontTypes_5D6B00();
    field_DC_brief.FormatCurrentBrief_5D3470();
    CalcCarNameTextWidth_5D5190();
    field_4C_zone_name.CalcTextWidth_5D5AD0();
    field_111C_message.FormatMessage_5D1860();
    field_12F0_mp_message.CalcTextWidth_5D56B0();
    field_1080_pickup_text.CalcTextWidth_5D53E0();
}

MATCH_FUNC(0x5d6b00)
void Hud::SetFontTypes_5D6B00()
{
    if (gText_0x14_704DFC->LangIsJapanese_452E60())
    {
        gPickupTextFont_7064B8 = 103;
        gCarNameFont_706508 = 104;
        gZoneNameFont_706618 = 105;
        gBriefFont_7065C4 = 107;
        gPlayerStatsFont_70646C = gStyleTextFont_703BAA;
        gMessageFont_7062F0 = 203;
        gDebugFont_706600 = 103;
        gPauseFont_7063F8 = 201;
        word_7064D8 = 103;
        gPlayerNameFont_7062DC = 103;
        gChatDisplayFont_70643E = 103;
    }
    else
    {
        gCarNameFont_706508 = gStyleCarNameFont_703D98;
        gPickupTextFont_7064B8 = gStyleTextFont_703BAA;
        gZoneNameFont_706618 = gStyleZoneNameFont_703C9C;
        gBriefFont_7065C4 = gStyleTextFont_703BAA;
        gPlayerStatsFont_70646C = gStyleTextFont_703BAA;
        gMessageFont_7062F0 = gStyleMessageFont_703D9C;
        gDebugFont_706600 = gStyleTextFont_703BAA;
        word_7064D8 = gStyleTextFont_703BAA;
        gPauseFont_7063F8 = gStylePauseFont_703DA4;
        gPlayerNameFont_7062DC = gStyleTextFont_703BAA;
        gChatDisplayFont_70643E = gStyleTextFont_703BAA;
    }
}

MATCH_FUNC(0x5d6be0)
void Hud::Init_5D6BE0()
{
    GetTextSpeed_5D6A90();
    SetFontTypes_5D6B00();
    field_4C_zone_name.ResetTransparency_5D5C50();
    field_107C_gang_respect_bars.Empty_5CFE30();
    field_1028_wanted_level.Init_5D0210();
    field_1F18_arrows.CreatePlayerArrows_5D1350();
}

MATCH_FUNC(0x5d6c20)
bool Hud::HandleInputKey_5D6C20(s32 action, Player* pPlayer)
{
    return field_12EC_quit_message.HandleQuitMessageKey_5D13C0(action, pPlayer) || field_2A25_chat_input.HandleChatInputKey_5D15E0(action, pPlayer);
}

MATCH_FUNC(0x5d6c70)
bool Hud::IsInputKeyConsumed_5D6C70(s32 action)
{
    return field_12EC_quit_message.IsQuitMessageKey_5D15A0(action) || field_2A25_chat_input.IsChatInputKey_5D17D0(action);
}

MATCH_FUNC(0x5d6cb0)
bool Hud::IsQuitMessageInputKey_5D6CB0(s32 action)
{
    return field_12EC_quit_message.IsQuitMessageKey_5D15A0(action);
}

// Defined before the member ctors it calls: when VC6 has already compiled them in this TU (and seen
// that they can't throw) it drops the EH frame and the state for the pager array that the original has.
MATCH_FUNC(0x5d6cd0)
Hud::Hud()
{
    field_13C4_text_speed = 0;
}

MATCH_FUNC(0x5d7510)
Hud_CopHead::Hud_CopHead()
{
    field_0_frame = 0;
    field_2_frame_delay = 0;
    field_1_frame_timer = 0;
    field_4_height = 0;
    field_8_velocity = -1;
}

MATCH_FUNC(0x5d7600)
Hud_Arrow::Hud_Arrow()
{
    field_18_targets.field_10_gang_info.field_30_gang = NULL;
    field_18_targets.field_10_gang_info.field_34_min_respect = 0;
    field_18_targets.field_10_gang_info.field_5_is_visible = false;

    field_18_targets.field_18_primary_target.init();
    field_18_targets.field_3C_secondary_target.init();

    field_18_targets.field_60_curr_target = &field_18_targets.field_18_primary_target;
    field_18_targets.field_2E_target_swap_timer = 0;
    field_18_targets.field_10_gang_info.field_6_in_use = false;
}

MATCH_FUNC(0x5d7650)
Hud_Pager::Hud_Pager()
{
    field_0_timer = -1;
    field_4_ptr_counter = NULL;
    field_8_sound = NULL;
}

