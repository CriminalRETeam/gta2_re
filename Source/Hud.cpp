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

DEFINE_GLOBAL(Hud*, gHud_2B00, 0x706620);
DEFINE_GLOBAL(s16, gDebugFont, 0x706600);
DEFINE_STATIC_GLOBAL(s16, sPickupTextFont, 0x7064B8);
DEFINE_STATIC_GLOBAL(u16, sZoneNameFont, 0x706618);
DEFINE_STATIC_GLOBAL(s16, sCarNameFont, 0x706508);
DEFINE_STATIC_GLOBAL(u16, sPlayerStatsFont, 0x70646C);
DEFINE_GLOBAL(u16, gBriefFont, 0x7065C4);
DEFINE_GLOBAL(u16, gMessageFont_7062F0, 0x7062F0);
DEFINE_STATIC_GLOBAL(u16, sPauseFont, 0x7063F8);
DEFINE_STATIC_GLOBAL(u16, sPlayerNameFont, 0x7062DC);
// The chat font. Renaming it (for example to gChatFont_70643E) changes the code of
// Hud_ChatInput::DrawChatMessages, so it keeps its old name.
DEFINE_STATIC_GLOBAL(u16, sChatDisplayFont, 0x70643E);
DEFINE_GLOBAL_ARRAY(char_type, gTmpGxtKey, 256, 0x67CE50);
DEFINE_GLOBAL(s16, word_7064D8, 0x7064D8);
DEFINE_GLOBAL_INIT(Fix16, kFpZero_7064C0, 0, 0x7064C0);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kArrowBaseRepositionSpeed, Fix16(0x400, 0), 0x7063B0);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kArrowRadiusOffset, Fix16(0x1C00, 0), 0x7065B4);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kArrowMinRadiusStep, Fix16(0x100, 0), 0x706338);

DEFINE_STATIC_GLOBAL(Fix16, sPhoneX, 0x67CD14);
DEFINE_STATIC_GLOBAL(Fix16, sPhoneY, 0x67CD0C);
DEFINE_STATIC_GLOBAL(Fix16, sPhoneZ, 0x67CD10);

DEFINE_GLOBAL_INIT(Ang16, kAngZero_706610, Ang16(0), 0x706610);

DEFINE_GLOBAL_INIT(Ang16, kAng180_706412, Ang16(720), 0x706412);
DEFINE_GLOBAL_INIT(Fix16, kFpOne_7064C4, Fix16(1), 0x7064C4);
DEFINE_GLOBAL_INIT(Fix16, kFpEight_7064E8, Fix16(8), 0x7064E8);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpQuarter, Fix16(0x1000, 0), 0x706300);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kArrowMaxRepositionSpeed, Fix16(0xC00, 0), 0x706298);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kArrowRepositionAccel, Fix16(0x100, 0), 0x7065A8);

EXTERN_GLOBAL_ARRAY(wchar_t, gTmpWideStr, 640);

EXTERN_GLOBAL_ARRAY(wchar_t, tmpBuff, 640);

EXTERN_GLOBAL(char_type, gLighting);

EXTERN_GLOBAL_ARRAY(wchar_t, gEmptyWStr, 2);

// 9.6f inline
static inline void DrawFigureScaled_4C71B0(s32 sprite_type, s16 sprite_idx, Fix16 x_pos, Fix16 y_pos, const Ang16& rotation, const s32& palette_type, s16 palette, s32 alpha_value, u8 flags)
{
    DrawFigure(sprite_type,
               sprite_idx,
               x_pos * gViewCamera->mUiScale,
               y_pos * gViewCamera->mUiScale,
               rotation,
               gViewCamera->mUiScale,
               palette_type,
               palette,
               alpha_value,
               flags,
               false);
}

// Camera::WorldToScreen (9.6f 0x40CFC0: x/y/z by value, writes through two out pointers)
// with this file's copies of the 1 and 8 constants
static inline void WorldToScreen_Hud_40CFC0(Camera* pCam, Fix16 x, Fix16 y, Fix16 z, Fix16* pOutX, Fix16* pOutY)
{
    Fix16 u = pCam->mCurPos.mZ - z;
    Fix16 t(kFpOne_7064C4 / Fix16(u.mValue + kFpEight_7064E8.mValue, 0));
    *pOutX = (((x - pCam->mCurPos.mX) * pCam->mScreenScale.y) * t) + Fix16(320);
    *pOutY = (((y - pCam->mCurPos.mY) * pCam->mScreenScale.y) * t) + Fix16(240);
}

// 9.6f 0x4C7280: DrawString at a position and size scaled by the UI scale
static inline void DrawTextScaled_4C7280(const wchar_t* pStr, Fix16 x, Fix16 y, u16 font, const s32& palette_type, u16 palette, s32 alpha, u8 flags)
{
    DrawString(pStr,
               x * gViewCamera->mUiScale,
               y * gViewCamera->mUiScale,
               font,
               gViewCamera->mUiScale,
               palette_type,
               palette,
               alpha,
               flags);
}

// Camera::ProjectWorldToScreen_4B90E0 (9.6f 0x4B90E0, inlined in 10.5) with this file's copies of the
// 1 and 8 constants
static inline void ProjectWorldToScreen_Hud_4B90E0(Camera* pCam, Fix16 x, Fix16 y, Fix16 z, Fix16* pOut1, Fix16* pOut2)
{
    Fix16 scale = kFpOne_7064C4 / ((kFpEight_7064E8 - z) + pCam->mCurPos.mZ);
    *pOut1 = (((x - pCam->mCurPos.mX) * pCam->mScreenScale.x) * scale) + Fix16(pCam->mScreenPxCenterX);
    *pOut2 = (((y - pCam->mCurPos.mY) * pCam->mScreenScale.x) * scale) + Fix16(pCam->mScreenPxCenterY);
}

MATCH_FUNC(0x4bbbb0)
Hud::~Hud()
{
}

MATCH_FUNC(0x4be650)
Hud_Pager::~Hud_Pager()
{
    mTimer = -1;
    mPtrCounter = NULL;

    if (mSound != NULL)
    {
        gRoot_sound.DestroySoundObj(mSound);
        mSound = NULL;
    }
}

// ----------------------------------------------------

MATCH_FUNC(0x4DEF00)
EXPORT wchar_t* gmp_map_zone::get_zone_str()
{
    char_type buf[4];
    if (mNameLength == 3)
    {
        buf[0] = mName[0];
        buf[1] = mName[1];
        buf[2] = mName[2];
        buf[3] = 0;
        return gText_0x14->Find(buf);
    }
    return NULL;
}

MATCH_FUNC(0x5cf620)
void Hud_BriefSelector::ShowNextNumberedBrief()
{
    do
    {
        mValue++;
        if (mValue > 9999)
        {
            mValue = 0;
        }
        sprintf(gTmpBuffer, "%d", mValue);
    } while (!gText_0x14->KeyExists(gTmpBuffer));
    gHud_2B00->mBrief.SetHudBrief(hud_brief_priority::now_3, gTmpBuffer);
    swprintf(tmpBuff, L"%d", mValue);
    gHud_2B00->mMessage.ShowMessage(tmpBuff, hud_message_priority::important_3);
}

MATCH_FUNC(0x5cf6b0)
void Hud_BriefSelector::ShowPrevNumberedBrief()
{
    do
    {
        mValue--;
        if (mValue < 0)
        {
            mValue = 9999;
        }
        sprintf(gTmpBuffer, "%d", mValue);
    } while (!gText_0x14->KeyExists(gTmpBuffer));
    gHud_2B00->mBrief.SetHudBrief(hud_brief_priority::now_3, gTmpBuffer);
    swprintf(tmpBuff, L"%d", mValue);
    gHud_2B00->mMessage.ShowMessage(tmpBuff, hud_message_priority::important_3);
}

MATCH_FUNC(0x5cf730)
void Hud_UnderRoofArrowMarker::Update()
{
    Ped* pPed = gGame_0x40->mLocalPlayer->GetCameraModePed();

    if (!pPed || static_cast<u8>(pPed->IsInTrain()))
    {
        mPedUnderSolid = false;
    }
    else
    {
        mPedUnderSolid =
            gMap_0x370->CheckColumnHasSolidAbove(pPed->get_cam_x(), pPed->get_cam_y(), pPed->get_cam_z());
        if (mPedUnderSolid)
        {
            this->mRotation = pPed->GetRotation() + kAng180_706412;

            Fix16 camz = pPed->mCam.z;
            Fix16 camy = pPed->mCam.y;
            Fix16 camx = pPed->mCam.x;

            Camera* pCam = gGame_0x40->mLocalPlayer->get_camera();
            ProjectWorldToScreen_Hud_4B90E0(pCam, camx, camy, camz, &mScreenX, &mScreenY);
        }
    }
}

MATCH_FUNC(0x5cf910)
void Hud_UnderRoofArrowMarker::Draw()
{
    if (mPedUnderSolid)
    {
        const s32 palette_type = palette_types_enum::sprites_2;
        Player* pPlayer = gGame_0x40->mLocalPlayer;
        Camera* pCam = pPlayer->get_camera();

        DrawFigure(sprite_types_enum::user_6, 0, mScreenX, mScreenY, mRotation, pCam->mUiScale, palette_type, 0, 1, 14, true);
    }
}

MATCH_FUNC(0x5cf970)
void Hud_ShowCoords::ShowPlayerCoords()
{
    if (mShowCoords)
    {
        Player* pPlayer = gGame_0x40->mLocalPlayer;

        Ped* pPed = pPlayer->GetActivePed();

        Gang* pZone = pPlayer->get_gang_curr_location();
        wchar_t* pZoneName;
        if (pZone)
        {
            pZoneName = pZone->GetNameWide();
        }
        else
        {
            pZoneName = gEmptyWStr;
        }

        Car* pCar = pPed->mCar;
        wchar_t* pCarOrPedStr;
        if (pCar)
        {
            pCarOrPedStr = pCar->GetCarStr();
        }
        else
        {
            pCarOrPedStr = L"ped";
        }
        swprintf(tmpBuff,
                 L"%s at (%3.1f, %3.1f, %3.1f) %s",
                 pCarOrPedStr,
                 pPed->get_cam_x().AsDouble(),
                 pPed->get_cam_y().AsDouble(),
                 pPed->get_cam_z().AsDouble(),
                 pZoneName);

        Hud_TextEntry* pTextEntry = gHud_2B00->mTexts.DisplayText(tmpBuff, -1, 16, sPickupTextFont, 1);
        pTextEntry->SetFontRemap(0);
    }
}

// ----------------------------------------------------

// https://decomp.me/scratch/bd2MO
MATCH_FUNC(0x5cfa70)
void Hud_GangRespectBars::DrawGangRespectBars()
{
    u32 random_num = gpRng->get_cur_rng() & 0xF;
    u8 PlayerIdx = gGame_0x40->mLocalPlayer->get_idx();
    bool bPlusSignDark = random_num > 7u;

    // 64 and the bar x go through the Fix16(u32) constructor (out-of-line copy 0x4926F0)
    s32 ypos = 11;

    for (Gang* pGang = gGangPool_CA8->FirstGang(); pGang; ypos += 27, pGang = gGangPool_CA8->NextGang())
    {
        s8 respect = pGang->GetRespectForPlayer(PlayerIdx);

        s32 arrow_colour = pGang->mArrowColour - 1;
        DrawFigureScaled(sprite_types_enum::user_6,
                         arrow_colour + 64,
                         16,
                         static_cast<u32>(ypos + 2),
                         kAngZero_706610,
                         palette_types_enum::sprites_2,
                         0,
                         0,
                         0);

        DrawFigureScaled(sprite_types_enum::user_6,
                         arrow_colour + 78,
                         64u,
                         static_cast<u32>(ypos + 2),
                         kAngZero_706610,
                         palette_types_enum::sprites_2,
                         0,
                         0,
                         0);

        DrawFigureScaled(sprite_types_enum::user_6,
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
            DrawFigureScaled(sprite_types_enum::user_6,
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
            DrawFigureScaled(sprite_types_enum::user_6,
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
                DrawFigureScaled(sprite_types_enum::user_6,
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
                DrawFigureScaled(sprite_types_enum::user_6,
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
                DrawFigureScaled(sprite_types_enum::user_6,
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
            DrawFigureScaled(sprite_types_enum::user_6, 47, 74, ypos + 9, kAngZero_706610, palette_types_enum::sprites_2, 0, 0, 0);
        }

        // red mission respect
        if (respect >= 80)
        {
            DrawFigureScaled(sprite_types_enum::user_6, 48, 84, ypos + 9, kAngZero_706610, palette_types_enum::sprites_2, 0, 0, 0);
        }

        // debug stuff
        if (bDo_show_instruments)
        {
            s32 text_palette = (respect >= 0) + 5;
            swprintf(tmpBuff, L"%d", respect);
            DrawStringScaledPalette(tmpBuff, 64u, ypos - 6, gDebugFont, palette_types_enum::font_remaps_8, text_palette, 0, 0);
        }
    }
}

MATCH_FUNC(0x5cfe20)
void Hud_GangRespectBars::UpdateGangRespectBars()
{
}

MATCH_FUNC(0x5cfe30)
void Hud_GangRespectBars::InitGangRespectBars()
{
}

MATCH_FUNC(0x5cfe40)
void Hud_PlayerNames::DrawPlayerNames()
{
    if (bStartNetworkGame && bShow_player_names)
    {
        Camera* pCam = gGame_0x40->mLocalPlayer->get_camera();

        for (Player* pIter = gGame_0x40->IterateFirstPlayer(); pIter; pIter = gGame_0x40->IterateNextPlayer())
        {
            if (!pIter->mIsUser)
            {
                Ped* pPlayerPed = pIter->mPlayerPed;
                if (!pPlayerPed || !pPlayerPed->mBf21C.bInvisible)
                {
                    Fix16 x = pPlayerPed->mCam.x;
                    Fix16 y = pPlayerPed->mCam.y;
                    Fix16 z = pPlayerPed->mCam.z;
                    // The original tests only al (bool return?)
                    if (static_cast<u8>(pCam->IsCoordsPosVisible(x, y, z)))
                    {
                        Fix16 xCalc;
                        Fix16 yCalc;
                        WorldToScreen_Hud_40CFC0(pCam, x, y, z, &xCalc, &yCalc);

                        DrawTextScaled_4C7280(pIter->mPlayerName,
                                              xCalc,
                                              yCalc,
                                              sPlayerNameFont,
                                              pIter->mHudPaletteType != palette_types_enum::user_remaps_7 ? palette_types_enum::sprites_2 : palette_types_enum::font_remaps_8,
                                              pIter->mHudPalette - 1,
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
void Hud_CopHead::UpdateHead(bool bShakeHead)
{
    if (!bShakeHead)
    {
        mHeight = 0;
    }

    mFrameTimer--;

    if (mFrameTimer == 0)
    {
        mFrameTimer = mFrameDelay;
        mFrame = mFrame == 0;
        if (bShakeHead)
        {
            mHeight += mVelocity;
            if (mHeight == -4)
            {
                mVelocity = 1;
            }
            else if (mHeight == 4)
            {
                mVelocity = -1;
            }
        }
    }
}

MATCH_FUNC(0x5d00b0)
void Hud_CopHead_C_Array::UpdateWantedLevel()
{
    Ped* pPed = gGame_0x40->mLocalPlayer->GetPlayerPed();
    mCopLevel = pPed->get_wanted_star_count();

    const bool bShakeHead = gPolice_7B8->IsBeingPursued(pPed);
    s32 i = 0;
    Hud_CopHead* pIter = &mCopHeads[0];
    while (i < mCopLevel)
    {
        pIter->UpdateHead(bShakeHead);
        i++;
        pIter++;
    }
}

// https://decomp.me/scratch/QYlEW
MATCH_FUNC(0x5d0110)
void Hud_CopHead_C_Array::DrawWantedLevel()
{
    Fix16 xpos = (Fix16(640) - (mWFp * mCopLevel)) / 2;
    Fix16 y_base = mHFp / 2;

    s32 cop_head_idx = 0;
    Hud_CopHead* pHead = mCopHeads;
    for (; cop_head_idx < mCopLevel; cop_head_idx++, pHead++, xpos += mWFp)
    {
        DrawFigureScaled_4C71B0(sprite_types_enum::user_6, pHead->mFrame + 14, xpos, y_base + Fix16(pHead->mHeight), kAngZero_706610, palette_types_enum::sprites_2, 0, 0, 0);
    }
}

MATCH_FUNC(0x5d0210)
void Hud_CopHead_C_Array::Init()
{
    u16 sprite_idx = gGtx_0x106C->GetSpriteTrueIndex(sprite_types_enum::user_6, 14);
    sprite_index* sprite_index = gGtx_0x106C->get_sprite_index(sprite_idx);

    mWFp.FromU8(sprite_index->mWidth);
    mHFp.FromU8(sprite_index->mHeight);

    for (s32 i = 0; i < GTA2_COUNTOF(mCopHeads); i++)
    {
        mCopHeads[i].mFrameDelay = 2;
        mCopHeads[i].mFrameTimer = 2;
    }
}

// ----------------------------------------------------

MATCH_FUNC(0x5d0260)
void Hud_Health::DrawHealth()
{
    s32 half_hearts;
    s32 xpos = 551;
    s32 health = gGame_0x40->mLocalPlayer->GetPlayerPed()->get_health();

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
        DrawFigureScaled(sprite_types_enum::user_6, 113, xpos, static_cast<u32>(34), kAngZero_706610, palette_types_enum::sprites_2, 0, 0, 0);
    }

    // Draw half heart
    if (half_hearts % 2 == 1)
    {
        if (half_hearts > 0)
        {
            xpos -= 2;
        }
        DrawFigureScaled(sprite_types_enum::user_6, 114, xpos, static_cast<u32>(34), kAngZero_706610, palette_types_enum::sprites_2, 0, 0, 0);
    }

    // Draw debug stuff
    if (bDo_show_instruments)
    {
        swprintf(tmpBuff, L"%d%%", health);
        DrawStringScaledPalette(tmpBuff, static_cast<u32>(551), static_cast<u32>(34), gDebugFont, palette_types_enum::sprites_2, 0, 0, 0);
    }
}

MATCH_FUNC(0x5d03c0)
void ArrowTrace::PointToInfoPhone(Gang* pZone)
{
    set_arrow_aim_from_pos(pZone->mInfoPhoneX, pZone->mInfoPhoneY, pZone->mInfoPhoneZ);
    mTargetType = ArrowTargetType::InfoPhone_5;
}

// ----------------------------------------------------

MATCH_FUNC(0x5D03F0)
void ArrowTrace::UpdateAimCoordinates()
{
    Ped* pPed;
    Player* pPlayer;
    Car* pCar;

    switch (mTargetType)
    {
        case ArrowTargetType::Nothing_0:
            return;
        case ArrowTargetType::Player_6:
            pPlayer = mPlayer;
            if (pPlayer->GetInUse())
            {
                mAimX = pPlayer->mPlayerPed->get_cam_x();
                mAimY = pPlayer->mPlayerPed->get_cam_y();
                mAimZ = pPlayer->mPlayerPed->get_cam_z();
            }
            else
            {
                ClearTarget();
            }
            break;
        case ArrowTargetType::Ped_2:
            pPed = mPed;
            if (pPed->IsActive())
            {
                mAimX = pPed->get_cam_x();
                mAimY = pPed->get_cam_y();
                mAimZ = pPed->get_cam_z();
            }
            else
            {
                ClearTarget();
            }
            break;
        case ArrowTargetType::Car_3:
            pCar = mCar;
            if (pCar->IsDespawning())
            {
                ClearTarget();
            }
            else
            {
                pCar->mCarSprite->GetPosition(&mAimX, &mAimY, &mAimZ);
            }
            break;
        case ArrowTargetType::Object_4:
            if (!mObj->IsNotModel_174())
            {
                gHud_2B00->mArrows.mCheckInfoPhones = true;
                ClearTarget();
            }
            else
            {
                mObj->mSprite->GetPosition(&mAimX, &mAimY, &mAimZ);
            }
            break;
        default:
            break;
    }

    Player* pLocalPlayer = gGame_0x40->mLocalPlayer;
    Camera* pCam = pLocalPlayer->get_camera();

    mbIsTargetVisible = pCam->IsCoordsPosVisible(mAimX, mAimY, mAimZ);
}

// ----------------------------------------------------

MATCH_FUNC(0x5d0510)
void Hud_Arrow::SetArrowColour(s32 arrow_colour)
{
    mTargets.mArrowColour = arrow_colour;
}

MATCH_FUNC(0x5d0530)
bool Hud_Arrow::CheckVisibility()
{
    Gang* pThisGang = mTargets.mGangInfo.mGang;

    if (pThisGang)
    {
        if (!gHud_2B00->mArrows.ShowGangArrows())
        {
            return false;
        }
        if (!bShow_all_arrows)
        {
            if (gScriptManager->IsOnMission())
            {
                return false; // player is on mission, so do not display gang phone arrows
            }
            Player* pPlayer = gGame_0x40->mLocalPlayer;
            Gang* pCurrLocationGang = pPlayer->get_gang_curr_location();
            if (pCurrLocationGang)
            {
                if (pPlayer->get_arrow_blocker_zone())
                {
                    pCurrLocationGang = NULL;
                }
            }
            if (mTargets.mCurrTarget->mTargetType == ArrowTargetType::InfoPhone_5)
            {
                if (pCurrLocationGang == pThisGang)
                {
                    if (gHud_2B00->mArrows.IsThereAnyOtherArrowsInSameGang(this))
                    {
                        return false;
                    }
                }
                else if (pCurrLocationGang && gHud_2B00->mArrows.IsThereAnyMissionPhoneArrowForGang(pCurrLocationGang))
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
                u8 player_idx = pPlayer->get_idx();
                if (pThisGang->GetRespectForPlayer(player_idx) < mTargets.mGangInfo.mMinRespect)
                {
                    return false;
                }
                Hud_Arrow* pVisibleGangArrow = gHud_2B00->mArrows.mVisibleGangArrow;
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
bool Hud_Arrow::UpdateTargets()
{
    Fix16_Point diff;
    mTargets.mPrimaryTarget.UpdateAimCoordinates();
    mTargets.mSecondaryTarget.UpdateAimCoordinates();

    if (mTargets.mCurrTarget->HasNoTarget())
    {
        swap_arrows(); // Swap arrow traces
        if (mTargets.mCurrTarget->HasNoTarget())
        {
            return true;
        }
    }
    if (mTargets.HasBothTargets() && mTargets.mPrimaryTarget.mbIsTargetVisible &&
        mTargets.mSecondaryTarget.mbIsTargetVisible)
    {
        if (mTargets.mTargetSwapTimer > 0)
        {
            mTargets.mTargetSwapTimer--;
            return false;
        }

        Fix16 xpos;
        Fix16 ypos;
        Fix16 zpos;
        gGame_0x40->mLocalPlayer->get_pos(&xpos, &ypos, &zpos);

        diff.SetXY(xpos - mTargets.mCurrTarget->mAimX, ypos - mTargets.mCurrTarget->mAimY);
        Fix16 distance_1 = diff.GetLength_41E260() - mRadiusPos;

        swap_arrows();

        diff.SetXY(xpos - mTargets.mCurrTarget->mAimX, ypos - mTargets.mCurrTarget->mAimY);
        Fix16 new_radius = diff.GetLength_41E260() - distance_1;
        mTargets.mTargetSwapTimer = 20;
        mRadiusPos = new_radius;
    }
    return false;
}

// https://decomp.me/scratch/CoKn3
MATCH_FUNC(0x5d0850)
void Hud_Arrow::UpdateScreenPos()
{
    Fix16_Point displacement;
    Fix16 player_xpos;
    Fix16 player_ypos;
    Fix16 player_zpos;

    gGame_0x40->mLocalPlayer->get_pos(&player_xpos, &player_ypos, &player_zpos);
    displacement.SetXY(player_xpos - mTargets.mCurrTarget->mAimX,
                       player_ypos - mTargets.mCurrTarget->mAimY);

    mRotation = displacement.GetAngle();

    // Assigned, not initialised: GetLength's result goes through a temporary that shares the atan2 temp's slot
    Fix16 distance;
    distance = displacement.GetLength_41E260();
    Fix16 intended_radius;

    if (mTargets.mCurrTarget->mbIsTargetVisible)
    {
        intended_radius = distance - kFpQuarter;
        if (intended_radius < kFpZero_7064C0)
        {
            intended_radius = kFpZero_7064C0;
        }
    }
    else
    {
        intended_radius = mMinRadiusPos;
        if (gGame_0x40->mLocalPlayer->GetPlayerCar())
        {
            intended_radius += kArrowRadiusOffset; // increment a little when in a car
        }
    }

    if (mRadiusPos > intended_radius)
    {
        mRadiusPos -= mRepositionSpeed;
        if (mRadiusPos <= intended_radius)
        {
            mRadiusPos = intended_radius;
            mRepositionSpeed = kArrowBaseRepositionSpeed;
        }
        else if (mRepositionSpeed < kArrowMaxRepositionSpeed) // below the maximum
        {
            mRepositionSpeed += kArrowRepositionAccel;
        }
    }
    else if (mRadiusPos < intended_radius)
    {
        mRadiusPos += mRepositionSpeed;
        if (mRadiusPos >= intended_radius)
        {
            mRadiusPos = intended_radius;
            mRepositionSpeed = kArrowBaseRepositionSpeed;
        }
        else if (mRepositionSpeed < kArrowMaxRepositionSpeed) // below the maximum
        {
            mRepositionSpeed += kArrowRepositionAccel;
        }
    }
    else
    {
        mRepositionSpeed = kArrowBaseRepositionSpeed;
    }

    // get_camera written out: as an inline call it shifts the inline budget and the projection's
    // operators no longer go out of line as in the original
    Camera* pCamera;
    Player* pPlayer = gGame_0x40->mLocalPlayer;
    if (pPlayer->mCameraMode == 2 || pPlayer->mCameraMode == 3)
    {
        pCamera = &pPlayer->mAuxGameCamera;
    }
    else
    {
        pCamera = &pPlayer->mGameCamera;
    }

    Fix16 factor;
    factor = kFpOne_7064C4 / ((kFpEight_7064E8 - mTargets.mCurrTarget->mAimZ) + pCamera->mCurPos.mZ);
    // multiply by 64
    Fix16& radius = mRadiusPos;
    Fix16 projected_radius = ((radius * 64) / (pCamera->mScreenScale.x * factor)) * pCamera->mUiScale;

    Fix16 zpos_2;
    if (distance != kFpZero_7064C0 && !mTargets.mCurrTarget->mbIsTargetVisible)
    {
        zpos_2 = player_zpos + ((radius / distance) * (mTargets.mCurrTarget->mAimZ - player_zpos));
    }
    else
    {
        zpos_2 = mTargets.mCurrTarget->mAimZ;
    }
    ProjectWorldToScreen_Hud_4B90E0(pCamera,
                                              player_xpos - (Ang16::sine(mRotation) * projected_radius),
                                              player_ypos - (Ang16::cosine(mRotation) * projected_radius),
                                              zpos_2,
                                              &mScreenPosX,
                                              &mScreenPosY);
}

MATCH_FUNC(0x5d0c60)
void Hud_Arrow::Service()
{
    if (!UpdateTargets())
    {
        char_type bVisible = CheckVisibility();
        mTargets.mGangInfo.mIsVisible = bVisible;
        if (bVisible)
        {
            UpdateScreenPos();
        }
    }
}

MATCH_FUNC(0x5d0c90)
void Hud_Arrow::DrawArrow()
{
    Player* pPlayer;
    Camera* pCam;
    s32 arrow_palette_type;

    if (!HasNeitherTarget())
    {
        if (IsVisible())
        {
            if (mTargets.mArrowColour != 5 || (static_cast<u32>(gpRng->get_cur_rng()) % 6 >= 3))
            {
                arrow_palette_type = palette_types_enum::sprites_2;
            }
            else
            {
                arrow_palette_type = palette_types_enum::user_remaps_7;
            }
            pPlayer = gGame_0x40->mLocalPlayer;
            pCam = pPlayer->get_camera();

            DrawFigure(sprite_types_enum::user_6,
                       mTargets.mArrowSprtIdx,
                       mScreenPosX,
                       mScreenPosY,
                       mRotation,
                       pCam->mUiScale,
                       arrow_palette_type,
                       0,
                       1,
                       14u,
                       true);

            s16 colour_related;
            switch (this->mTargets.mArrowColour)
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
            pPlayer = gGame_0x40->mLocalPlayer;
            pCam = pPlayer->get_camera();

            DrawFigure(sprite_types_enum::user_6,
                       colour_related + 8,
                       mScreenPosX,
                       mScreenPosY,
                       mRotation,
                       pCam->mUiScale,
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
inline s32 TagGame::GetPlayerTime(Player* pPlayer)
{
    return mItTimeSecs[pPlayer->get_idx()];
}

MATCH_FUNC(0x5d0dc0)
void Hud_Arrow::SetPlayerArrowColour(Ped* pPed)
{
    switch (pPed->mRemap)
    {
        case 11:
            mTargets.mArrowSprtIdx = 1;
            break;
        case 13:
            mTargets.mArrowSprtIdx = 2;
            break;
        case 8:
            mTargets.mArrowSprtIdx = 3;
            break;
        case 5:
        case 6:
            mTargets.mArrowSprtIdx = 4;
            break;
        case 7:
            mTargets.mArrowSprtIdx = 5;
            break;
        case 9:
            mTargets.mArrowSprtIdx = 6;
            break;
        case 10:
            mTargets.mArrowSprtIdx = 7;
            break;

        default:
            return;
    }
}

MATCH_FUNC(0x5d0e40)
bool Hud_Arrow_7C_Array::IsThereAnyOtherArrowsInSameGang(Hud_Arrow* pArgArrow)
{
    Gang* pGang = pArgArrow->mTargets.mGangInfo.mGang;

    for (s32 i = 0; i < GTA2_COUNTOF_S(mArray); i++)
    {
        Hud_Arrow* pArrow = &mArray[i];
        if (pArrow != pArgArrow && !pArrow->HasNeitherTarget() && pArrow->IsVisible() &&
            pArrow->mTargets.mGangInfo.mGang == pGang)
        {
            return true;
        }
    }
    return false;
}

MATCH_FUNC(0x5d0e90)
void Hud_Arrow_7C_Array::DrawArrows()
{
    if (IsNetworkGame())
    {
        // Limit drawn arrows in multiplayer
        for (s32 i = 0; i < GTA2_COUNTOF_S(mArray); i++)
        {
            if (mArray[i].mTargets.mPrimaryTarget.GetTargetPlayer() == NULL ||
                mArray[i].mTargets.mPrimaryTarget.GetTargetPlayer()->GetPlayerPed() == NULL ||
                mArray[i].mTargets.mPrimaryTarget.GetTargetPlayer()->GetPlayerPed()->mBf21C.bInvisible == 0)
            {
                mArray[i].DrawArrow();
            }
        }
    }
    else
    {
        for (s32 i = 0; i < 17; i++)
        {
            mArray[i].DrawArrow();
        }
    }
}

MATCH_FUNC(0x5d0ef0)
void Hud_Arrow_7C_Array::FindVisibleGangArrow()
{
    s32 idx = 0;
    Hud_Arrow* pIter = &mArray[0];
    while (idx < GTA2_COUNTOF_S(mArray))
    {
        if (pIter->HasNeitherTarget() ||
            !pIter->mTargets.mGangInfo.mGang || !pIter->mTargets.mCurrTarget->mbIsTargetVisible)
        {
            idx++;
            pIter++;
        }
        else
        {
            this->mVisibleGangArrow = pIter;
            return;
        }
    }
    this->mVisibleGangArrow = NULL;
}

// ----------------------------------------------------

MATCH_FUNC(0x5d0f40)
bool Hud_Arrow_7C_Array::IsThereAnyMissionPhoneArrowForGang(Gang* pArgGang)
{
    Hud_Arrow* pIter = &mArray[0];
    for (s32 i = 0; i < GTA2_COUNTOF_S(mArray); i++, pIter++)
    {
        if (!pIter->HasNeitherTarget() &&
            (pIter->mTargets.mGangInfo.mGang == pArgGang &&
             pIter->mTargets.mCurrTarget->mTargetType != ArrowTargetType::InfoPhone_5))
        {
            return true;
        }
    }
    return false;
}

MATCH_FUNC(0x5d0f80)
void Hud_Arrow_7C_Array::ClearOrphanInfoPhoneArrows()
{
    for (s32 i = 0; i < GTA2_COUNTOF_S(mArray); i++)
    {
        if (!mArray[i].HasNeitherTarget())
        {
            if (mArray[i].mTargets.mGangInfo.mGang)
            {
                if (mArray[i].mTargets.mCurrTarget->mTargetType == ArrowTargetType::InfoPhone_5 &&
                    !IsThereAnyMissionPhoneArrowForGang(mArray[i].mTargets.mGangInfo.mGang))
                {
                    mArray[i].ClearTargets();
                }
            }
        }
    }
}

// ----------------------------------------------------

MATCH_FUNC(0x5d0fd0)
void Hud_Arrow_7C_Array::UpdateArrows()
{
    FindVisibleGangArrow();

    for (s32 i = 0; i < GTA2_COUNTOF(mArray); i++)
    {
        if (!mArray[i].HasNeitherTarget())
        {
            mArray[i].Service();
        }
    }

    if (mCheckInfoPhones)
    {
        ClearOrphanInfoPhoneArrows();
        mCheckInfoPhones = false;
    }
}

// ----------------------------------------------------

MATCH_FUNC(0x5d1020)
Hud_Arrow* Hud_Arrow_7C_Array::FindFreeArrow(s32* pOutIdx)
{
    Hud_Arrow* pIter = &mArray[0];
    for (s32 i = 0; i < GTA2_COUNTOF_S(mArray); i++, pIter++)
    {
        if (!pIter->mTargets.mGangInfo.mInUse)
        {
            *pOutIdx = i;
            pIter->mTargets.mGangInfo.mInUse = true;
            return pIter;
        }
    }
    return NULL;
}

MATCH_FUNC(0x5d1050)
Hud_Arrow* Hud_Arrow_7C_Array::AllocArrow()
{
    s32 idx;
    Hud_Arrow* pRet = FindFreeArrow(&idx);
    pRet->Reset();
    pRet->SetMinRadiusPos(16 - idx);
    return pRet;
}

MATCH_FUNC(0x5d10b0)
void Hud_Arrow_7C_Array::ReleaseAllArrows()
{
    for (s32 i = 0; i < GTA2_COUNTOF_S(mArray); i++)
    {
        if (mArray[i].mTargets.mGangInfo.mInUse)
        {
            mArray[i].ClearTargets();

            mArray[i].mTargets.mGangInfo.mInUse = false;
        }
    }
}

MATCH_FUNC(0x5d10d0)
Hud_Arrow* Hud_Arrow_7C_Array::FindGangPhoneArrow(Gang* pZone, s32 phone_type)
{
    s32 i = 0;
    Hud_Arrow* pIter = mArray;
    while (i < GTA2_COUNTOF_S(mArray))
    {
        if ((pIter->mTargets.mPrimaryTarget.mTargetType ||
             pIter->mTargets.mSecondaryTarget.mTargetType) &&
            (pIter->mTargets.mGangInfo.mGang == pZone && pIter->mTargets.mArrowColour == phone_type))
        {
            return pIter;
        }
        i++;
        pIter++;
    }
    return NULL;
}

MATCH_FUNC(0x5d1110)
void Hud_Arrow_7C_Array::place_gang_phone(Object* pPhoneInfo)
{
    s32 phone_type = GetPhoneTypeFromObjModel(pPhoneInfo->mModel);
    Gang* pZone = gMap_0x370->GetGangAtCoords(pPhoneInfo->mSprite->GetXPos(), pPhoneInfo->mSprite->GetYPos());

    if (!pZone)
    {
        sPhoneX = pPhoneInfo->mSprite->GetXPos();
        sPhoneY = pPhoneInfo->mSprite->GetYPos();
        sPhoneZ = 0;
        FatalError(Gta2Error::PlacingGangPhoneWhenNoGang, // Placing gang phone when no gang at: (%.4f, %.4f)
                   "C:\\Splitting\\Gta2\\Source\\user.cpp",
                   1509,
                   0,
                   &sPhoneX,
                   &sPhoneY,
                   &sPhoneZ);
    }
    Hud_Arrow* pExistingArrow = Hud_Arrow_7C_Array::FindGangPhoneArrow(pZone, phone_type);
    if (pExistingArrow)
    {
        if (pExistingArrow->mTargets.mSecondaryTarget.mTargetType)
        {
            strcpy(gErrStr, get_phone_colour(phone_type));
            strcpy(gErrStr2, pZone->mName);
            FatalError(Gta2Error::TooManyPhonesForGang, // Too many %s phones for %s gang
                       "C:\\Splitting\\Gta2\\Source\\user.cpp",
                       1513,
                       gErrStr,
                       gErrStr2);
        }
        pExistingArrow->mTargets.mSecondaryTarget.mObj = pPhoneInfo;
        pExistingArrow->mTargets.mSecondaryTarget.mTargetType = ArrowTargetType::Object_4;
    }
    else
    {
        Hud_Arrow* pNewArrow = Hud_Arrow_7C_Array::AllocArrow();
        pNewArrow->mTargets.mPrimaryTarget.mObj = pPhoneInfo;
        pNewArrow->mTargets.mPrimaryTarget.mTargetType = ArrowTargetType::Object_4;
        pNewArrow->SetArrowColour(phone_type);
        pNewArrow->mTargets.mGangInfo.mGang = pZone;
        pNewArrow->mTargets.mGangInfo.mMinRespect = GetMinRespectForPhoneType(phone_type);
        pNewArrow->mTargets.mArrowSprtIdx = pZone->mArrowColour;
    }
}

// ----------------------------------------------------

MATCH_FUNC(0x5D1260)
s32 __stdcall GetPhoneTypeFromObjModel(s32 phone_model_idx)
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
char_type* __stdcall get_phone_colour(s32 phone_type)
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
u8 __stdcall GetMinRespectForPhoneType(s32 phone_type)
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
void Hud_Arrow_7C_Array::SetNewGangArrow(Gang* pZone)
{
    Hud_Arrow* pArrow = AllocArrow();
    pArrow->SetArrowColour(4);
    pArrow->mTargets.mGangInfo.mGang = pZone;
    pArrow->mTargets.mArrowSprtIdx = pZone->mArrowColour;
    pArrow->mTargets.mGangInfo.mMinRespect = 0;
    pArrow->mTargets.mPrimaryTarget.PointToInfoPhone(pZone);
}

// ----------------------------------------------------

MATCH_FUNC(0x5d1350)
void Hud_Arrow_7C_Array::CreatePlayerArrows()
{
    if (static_cast<u8>(bStartNetworkGame))
    {
        if (gGameSession.GetMultiplayerGamemode() != TAG_GAME_3)
        {
            ReleaseAllArrows();
            for (Player* pPlayerIter = gGame_0x40->IterateFirstPlayer(); pPlayerIter;
                 pPlayerIter = gGame_0x40->IterateNextPlayer())
            {
                if (!pPlayerIter->mIsUser)
                {
                    Hud_Arrow* pArrow = AllocArrow();
                    pArrow->mTargets.mPrimaryTarget.mPlayer = pPlayerIter;
                    pArrow->mTargets.mPrimaryTarget.mTargetType = ArrowTargetType::Player_6;
                    Ped* pPlayerPed = pPlayerIter->mPlayerPed;
                    if (pPlayerPed)
                    {
                        pArrow->SetPlayerArrowColour(pPlayerPed);
                    }
                }
            }
        }
    }
}

MATCH_FUNC(0x5d13c0)
bool Hud_QuitMessage::HandleQuitMessageKey(s32 action, Player* pPlayer)
{
    if (pPlayer->mShowQuitMessage)
    {
        if (action == DIK_RETURN)
        {
            pPlayer->mShowQuitMessage = false;
            if (pPlayer->IsUser())
            {
                gGame_0x40->ExitGameNoBonus(1, GameExitType::PlayerQuit_2);
            }

            if (IsNetworkGame())
            {
                gTagGame.SetQuit(pPlayer->get_idx());
            }

            return true;
        }
        else if (action == DIK_ESCAPE)
        {
            pPlayer->mShowQuitMessage = false;
            return true;
        }
    }
    return false;
}

// ----------------------------------------------------

MATCH_FUNC(0x5d1430)
void Hud_QuitMessage::DrawQuitMessage()
{
    if (gGame_0x40->mLocalPlayer->mShowQuitMessage)
    {
        wchar_t* pQuitText = gText_0x14->Find("quit1");

        s32 max_text_width = Frontend::GetMaxTextWidth(pQuitText, word_7064D8);
        s32 line_spacing_added = CountLineSpacing(pQuitText, word_7064D8);
        s32 line2_ypos = ((3 * (160 - line_spacing_added)) / 2);

        DrawStringScaledPalette(pQuitText,
                                static_cast<u32>((640 - max_text_width) / 2),
                                line2_ypos - line_spacing_added,
                                word_7064D8,
                                palette_types_enum::sprites_2,
                                0,
                                0,
                                0);

        wchar_t* pQuitText2 = gText_0x14->Find("quit2");
        s32 max_text_width_2 = Frontend::GetMaxTextWidth(pQuitText2, word_7064D8);

        DrawStringScaledPalette(pQuitText2,
                                static_cast<u32>((640 - max_text_width_2) / 2),
                                static_cast<u32>(line2_ypos),
                                word_7064D8,
                                palette_types_enum::sprites_2,
                                0,
                                0,
                                0);

        wchar_t* pQuitText3 = gText_0x14->Find("quit3");
        s32 max_text_width_3 = Frontend::GetMaxTextWidth(pQuitText3, word_7064D8);

        DrawStringScaledPalette(pQuitText3,
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
bool Hud_QuitMessage::IsQuitMessageKey(s32 action)
{
    return gGame_0x40->mLocalPlayer->mShowQuitMessage && (action == DIK_RETURN || action == DIK_ESCAPE);
}

MATCH_FUNC(0x5d15d0)
void Hud_QuitMessage::ShowQuitMessage(Player* pPlayer)
{
    pPlayer->mShowQuitMessage = true;
}

// ----------------------------------------------------

MATCH_FUNC(0x5d15e0)
bool Hud_ChatInput::HandleChatInputKey(s32 action, Player* pPlayer)
{
    if (bStartNetworkGame && pPlayer->mIsChatting)
    {
        if (action == DIK_RETURN)
        {
            pPlayer->mIsChatting = false;
            return true;
        }
        else if (action == DIK_BACK)
        {
            if (pPlayer->mChatTextIdx > 0)
            {
                pPlayer->mChatTextIdx--;
                pPlayer->mChatText[pPlayer->mChatTextIdx] = 0;
            }
            return true;
        }
        else if (action == DIK_SPACE)
        {
            if (pPlayer->mChatTextIdx < 79)
            {
                pPlayer->mChatText[pPlayer->mChatTextIdx] = ' ';
                pPlayer->mChatTextIdx++;
                pPlayer->mChatText[pPlayer->mChatTextIdx] = 0;
            }
            return true;
        }
        else
        {
            const u16 char_value = gText_0x14->RemapExtendedChar(gKeybrd_0x204->GetKey(action));
            if (char_value)
            {
                if (pPlayer->mChatTextIdx < 79)
                {
                    pPlayer->mChatText[pPlayer->mChatTextIdx] = char_value;
                    pPlayer->mChatTextIdx++;
                    pPlayer->mChatText[pPlayer->mChatTextIdx] = 0;
                }
                return true;
            }
        }
    }
    return false;
}

// https://decomp.me/scratch/gMsUi
MATCH_FUNC(0x5d16b0)
void Hud_ChatInput::DrawChatMessages()
{
    s32 line_spacing = GetLineSpacingFromFontType_5D7700_inlined(sChatDisplayFont);
    s32 text_ypos = 480 - line_spacing;
    if (bStartNetworkGame)
    {
        for (Player* pPlayerIter = gGame_0x40->IterateFirstPlayer(); pPlayerIter != NULL;
             pPlayerIter = gGame_0x40->IterateNextPlayer())
        {
            if (pPlayerIter->mIsChatting)
            {
                if ((gpRng->get_cur_rng() & 7u) < 4)
                {
                    swprintf(tmpBuff, L"%s:%s_", pPlayerIter->mPlayerName, reinterpret_cast<wchar_t*>(pPlayerIter->mChatText));
                }
                else
                {
                    swprintf(tmpBuff, L"%s:%s ", pPlayerIter->mPlayerName, reinterpret_cast<wchar_t*>(pPlayerIter->mChatText));
                }
                s32 max_text_width = Frontend::GetMaxTextWidth(tmpBuff, sChatDisplayFont);
                s32 start_xpos;
                if (max_text_width > 640)
                {
                    start_xpos = 640 - max_text_width;
                }
                else
                {
                    start_xpos = 0;
                }
                DrawStringScaledPalette(tmpBuff, start_xpos, text_ypos, sChatDisplayFont, palette_types_enum::font_remaps_8, 5, 0, 0);
                text_ypos -= line_spacing;
            }
        }
    }
}

MATCH_FUNC(0x5d17d0)
bool Hud_ChatInput::IsChatInputKey(s32 key_idx)
{
    if (bStartNetworkGame)
    {
        if (gGame_0x40->mLocalPlayer->mIsChatting)
        {
            if (key_idx == DIK_RETURN || key_idx == DIK_BACK || key_idx == DIK_SPACE)
            {
                return true;
            }

            if (gText_0x14->RemapExtendedChar(gKeybrd_0x204->GetKey(key_idx)))
            {
                return true;
            }
        }
    }
    return false;
}

MATCH_FUNC(0x5d1830)
void Hud_ChatInput::StartChatting(Player* pPlayer)
{
    pPlayer->mIsChatting = true;
    pPlayer->mChatTextIdx = 0;
    pPlayer->mChatText[0] = 0;
}

// ----------------------------------------------------

MATCH_FUNC(0x5d1850)
void Hud_Message::ClearTimeToShow()
{
    mTimeToShow = 0;
}

MATCH_FUNC(0x5d1860)
void Hud_Message::LayoutMessage()
{
    if (this->mTimeToShow)
    {
        TextTable::InsertLineBreaksAndGetNumLines(&this->mStr[100], this->mStr, 580, gMessageFont_7062F0);
        this->mXpos = static_cast<u16>((640 - Frontend::GetMaxTextWidth(&this->mStr[100], gMessageFont_7062F0)) / 2);
        this->mYpos = static_cast<u16>((480 - CountLineSpacing(&this->mStr[100], gMessageFont_7062F0)) / 4);
    }
}

MATCH_FUNC(0x5d1940)
void Hud_Message::DrawMessage()
{
    if (mTimeToShow)
    {
        DrawStringScaledPalette(&mStr[100], mXpos, mYpos, gMessageFont_7062F0, palette_types_enum::sprites_2, 0, 0, 0);
    }
}

MATCH_FUNC(0x5d1a00)
void Hud_Message::ShowMessage(const wchar_t* pStr, s32 priority)
{
    if (mTimeToShow <= 0 || priority >= mPriority)
    {
        mPriority = priority;
        wcscpy(mStr, pStr);
        gText_0x14->StrToUpper(mStr);
        mTimeToShow = 90;
        LayoutMessage();
    }
}

MATCH_FUNC(0x5d1ab0)
void Hud_Message::DecrementTimeToShow()
{
    if (mTimeToShow != 0)
    {
        mTimeToShow--;
    }
}

// ----------------------------------------------------

MATCH_FUNC(0x5d1ae0)
Hud_Message::Hud_Message()
{
    mTimeToShow = 0;
    mPriority = hud_message_priority::normal_1;
}

MATCH_FUNC(0x5d1b10)
void Hud_TextEntry::FormatAndSetupText(const wchar_t* pStr, s16 xpos, s16 ypos, s16 fontType, s32 displayTime)
{
    this->mFontType = fontType;

    TextTable::InsertLineBreaksAndGetNumLines(mStrBuf, pStr, 640, fontType);

    if (mFontType == gStyleZoneNameFont || mFontType == gStyleMessageFont)
    {
        gText_0x14->StrToUpper(mStrBuf);
    }

    this->mDrawKind = 2;
    this->mPalette = 0;

    if (xpos == -1)
    {
        this->mX = (640 - Frontend::GetMaxTextWidth(mStrBuf, this->mFontType)) / 2;
    }
    else
    {
        this->mX = xpos;
    }

    if (ypos == -1)
    {
        this->mY = (480 - CountLineSpacing(mStrBuf, mFontType)) / 2;
    }
    else
    {
        this->mY = ypos;
    }

    // Stores in both branches: one store after the if leaves 0 out of a register (ebx)
    if (displayTime == -2)
    {
        this->mDisplayTime = gHud_2B00->mTextSpeed * wcslen(mStrBuf);
    }
    else
    {
        this->mDisplayTime = displayTime;
    }

    ClearAlpha();
}

MATCH_FUNC(0x5d1d00)
void Hud_TextEntry::Draw()
{
    DrawStringScaledPalette(mStrBuf,
                            mX,
                            mY,
                            mFontType,
                            mDrawKind,
                            mPalette,
                            mAlpha,
                            mAlphaFlag);
}

MATCH_FUNC(0x5d1db0)
bool Hud_TextEntry::DecrementDisplayTime()
{
    if (mDisplayTime != -3)
    {
        mDisplayTime--;
        if (mDisplayTime < 0)
        {
            return true;
        }
    }
    return false;
}

MATCH_FUNC(0x5d1e10)
bool Hud_TextEntry::operator_equals(const Hud_TextEntry* pOther) const
{
    return mDisplayTime > 0 && pOther != this && mX == pOther->mX && mY == pOther->mY &&
        mFontType == pOther->mFontType && !wcscmp(mStrBuf, pOther->mStrBuf);
}

MATCH_FUNC(0x5d1eb0)
void Hud_TextList::ExpireDuplicates(const Hud_TextEntry* pEntry)
{
    Hud_TextEntry* pIter = mpFirst;
    while (pIter)
    {
        if (pIter->operator_equals(pEntry))
        {
            pIter->Expire();
            return;
        }
        pIter = pIter->mpNext;
    }
}

MATCH_FUNC(0x5d1f50)
Hud_TextEntry* Hud_TextList::DisplayText(const wchar_t* pStr, s16 xpos, s16 ypos, s16 font_type, s32 display_time)
{
    Hud_TextEntry* pEntry = mpFreeList;
    Hud_TextEntry* pOldFirst = mpFirst;
    mpFreeList = pEntry->mpNext;
    pEntry->mpNext = pOldFirst;
    mpFirst = pEntry;
    pEntry->FormatAndSetupText(pStr, xpos, ypos, font_type, display_time);
    ExpireDuplicates(pEntry);
    return pEntry;
}

MATCH_FUNC(0x5d2010)
void Hud_TextList::Service()
{
    Hud_TextEntry* pIter = mpFirst;
    while (pIter)
    {
        pIter->Draw();
        pIter = pIter->mpNext;
    }
}

// 9.6f 0x4C7130
inline void Hud_Pager::SetCounter(s32* pCounter)
{
    mPtrCounter = pCounter;
    if (!mSound && !bSkip_audio)
    {
        mSound = gRoot_sound.CreateSoundObject(this, SoundObjectTypeEnum::Hud_Pager_C_11);
    }
}

MATCH_FUNC(0x5d2050)
void Hud_TextList::RemoveExpired()
{
    Hud_TextEntry* pPrev = NULL;
    Hud_TextEntry* pIter = mpFirst;
    while (pIter)
    {
        if (pIter->DecrementDisplayTime())
        {
            if (pPrev)
            {
                pPrev->mpNext = pIter->mpNext;
                pIter->mpNext = mpFreeList;
                mpFreeList = pIter;
                pIter = pPrev->mpNext;
            }
            else
            {
                Hud_TextEntry* pOldFreeList = mpFreeList;
                mpFirst = mpFirst->mpNext;
                pIter->mpNext = pOldFreeList;
                mpFreeList = pIter;
                pIter = mpFirst;
            }
        }
        else
        {
            pPrev = pIter;
            pIter = pIter->mpNext;
        }
    }
}

MATCH_FUNC(0x5d2280)
Hud_TextList::Hud_TextList()
{
    Hud_TextEntry* pIter = &m29Ary[0];
    for (s32 i = 0; i < 30 - 1; i++)
    {
        pIter->mpNext = pIter + 1;
        pIter++;
    }

    mpFreeList = &m29Ary[0];
    m29Ary[30 - 1].mpNext = NULL;
    mpFirst = NULL;
}

MATCH_FUNC(0x5d2320)
void Hud_Pager::Service()
{
    if (mTimer < 0)
    {
        return;
    }

    mTimer--;
    if (mTimer == -1)
    {
        mPtrCounter = NULL;
    }
}

MATCH_FUNC(0x5d2380)
void Hud_Pager::DrawCounterDigits(s32 xpos, s32 ypos)
{
    s32 counter = *mPtrCounter;
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
    DrawFigureScaled(sprite_types_enum::user_6, 123 + ones, xpos + 11, ypos + 2, kAngZero_706610, palette_types_enum::sprites_2, 0, 0, 0);

    DrawFigureScaled(sprite_types_enum::user_6, 123 + tens, xpos + 4, ypos + 2, kAngZero_706610, palette_types_enum::sprites_2, 0, 0, 0);

    DrawFigureScaled(sprite_types_enum::user_6, 123 + hundreds, xpos - 3, ypos + 2, kAngZero_706610, palette_types_enum::sprites_2, 0, 0, 0);

    DrawFigureScaled(sprite_types_enum::user_6, 123 + thousands, xpos - 10, ypos + 2, kAngZero_706610, palette_types_enum::sprites_2, 0, 0, 0);
}

MATCH_FUNC(0x5d2680)
void Hud_Pager::DrawDigits(s32 xpos, s32 ypos)
{
    // (mTimer / 30 fps) = total time in seconds
    s32 minutes = (mTimer / 30) / 60;
    s32 seconds = (mTimer / 30) % 60;
    DrawFigureScaled(sprite_types_enum::user_6,
                     123 + minutes / 10,
                     xpos - 13,
                     ypos + 3,
                     kAngZero_706610,
                     palette_types_enum::sprites_2,
                     0,
                     0,
                     0);

    DrawFigureScaled(sprite_types_enum::user_6,
                     123 + minutes % 10,
                     xpos - 6,
                     ypos + 3,
                     kAngZero_706610,
                     palette_types_enum::sprites_2,
                     0,
                     0,
                     0);

    if (mTimer % 15 <= 7)
    {
        DrawFigureScaled(sprite_types_enum::user_6, 121, xpos - 1, ypos + 3, kAngZero_706610, palette_types_enum::sprites_2, 0, 0, 0);

        if (mTimer < 300) // less than 10 seconds: draw flashing light
        {
            DrawFigureScaled(sprite_types_enum::user_6,
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
        DrawFigureScaled(sprite_types_enum::user_6, 120, xpos - 1, ypos + 3, kAngZero_706610, palette_types_enum::sprites_2, 0, 0, 0);
    }

    DrawFigureScaled(sprite_types_enum::user_6,
                     123 + seconds / 10,
                     xpos + 4,
                     ypos + 3,
                     kAngZero_706610,
                     palette_types_enum::sprites_2,
                     0,
                     0,
                     0);

    DrawFigureScaled(sprite_types_enum::user_6,
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
void Hud_Pager::DrawPager(u32 xpos, s32 ypos)
{
    const s32 palette_type = palette_types_enum::sprites_2;
    if (mTimer < 0 && !mPtrCounter)
    {
        return;
    }

    if (mTimer >= 0 && mPtrCounter)
    {
        s32 top_height = get_user_sprite_height(117);
        s32 bottom_height = get_user_sprite_height(118);
        s32 middle_height = get_user_sprite_height(119);

        DrawFigureScaled(sprite_types_enum::user_6, 117, xpos, ypos - top_height / 2 - middle_height / 2, kAngZero_706610, palette_type, 0, 0, 0);

        // The middle sprite's y goes through the Fix16(u32) constructor (out-of-line copy 0x4926F0) like x does
        DrawFigureScaled(sprite_types_enum::user_6, 119, xpos, static_cast<u32>(ypos), kAngZero_706610, palette_type, 0, 0, 0);

        DrawFigureScaled(sprite_types_enum::user_6, 118, xpos, ypos + middle_height / 2 + bottom_height / 2, kAngZero_706610, palette_type, 0, 0, 0);
        Hud_Pager::DrawCounterDigits(xpos, ypos - 6);
        Hud_Pager::DrawDigits(xpos, ypos + 6);
    }
    else if (mTimer >= 0)
    {
        s32 top_height = get_user_sprite_height(117);
        s32 bottom_height = get_user_sprite_height(118);

        DrawFigureScaled(sprite_types_enum::user_6, 117, xpos, ypos - top_height / 2, kAngZero_706610, palette_type, 0, 0, 0);

        DrawFigureScaled(sprite_types_enum::user_6, 118, xpos, ypos + bottom_height / 2, kAngZero_706610, palette_type, 0, 0, 0);
        Hud_Pager::DrawDigits(xpos, ypos);
    }
    else
    {
        s32 top_height = get_user_sprite_height(117);
        s32 bottom_height = get_user_sprite_height(118);

        DrawFigureScaled(sprite_types_enum::user_6, 117, xpos, ypos - top_height / 2, kAngZero_706610, palette_type, 0, 0, 0);

        DrawFigureScaled(sprite_types_enum::user_6, 118, xpos, ypos + bottom_height / 2, kAngZero_706610, palette_type, 0, 0, 0);
        Hud_Pager::DrawCounterDigits(xpos, ypos);
    }
}

MATCH_FUNC(0x5d3040)
void Hud_Pager_C_Array::DrawPagers()
{
    s32 totalSpriteHeight = get_user_sprite_height(117) + get_user_sprite_height(118) + get_user_sprite_height(119);
    s32 width = (get_user_sprite_width(117) / 2) + 3;

    s32 ypos = gGameSession.IsBonusStage() ? 36 : 104;
    for (s32 i = 0; i < GTA2_COUNTOF(mPagersArray); i++)
    {
        mPagersArray[i].DrawPager(width, ypos);
        ypos += totalSpriteHeight;
    }
}

MATCH_FUNC(0x5d31b0)
void Hud_Pager_C_Array::UpdatePagers()
{
    for (s32 i = 0; i < GTA2_COUNTOF(mPagersArray); i++)
    {
        mPagersArray[i].Service();
    }
}

MATCH_FUNC(0x5d31f0)
s32 Hud_Pager_C_Array::CreateTimer(s32 seconds) // returns the new Pager id
{
    for (s32 id = 0; id < GTA2_COUNTOF_S(mPagersArray); id++)
    {
        Hud_Pager* pPager = &mPagersArray[id];
        if (!pPager->IsTimerOff() || !pPager->no_ptr_counter())
        {
            continue;
        }
        pPager->set_timer(30 * seconds);
        return id;
    }
    return -1;
}

MATCH_FUNC(0x5d3220)
s32 Hud_Pager_C_Array::AddOnScreenCounter(s32* pCounter)
{
    s32 targetIdx = -1;
    for (s32 i = 0; i < GTA2_COUNTOF_S(mPagersArray); i++)
    {
        Hud_Pager* pPager = &mPagersArray[i];
        if (pPager->no_ptr_counter() && (!pPager->IsTimerOff() || targetIdx == -1))
        {
            targetIdx = i;
        }
    }

    mPagersArray[targetIdx].SetCounter(pCounter);

    return targetIdx;
}

MATCH_FUNC(0x5d3280)
void Hud_Pager_C_Array::ClearPager(s32 idx)
{
    SoundObject* pSound = mPagersArray[idx].mSound;
    Hud_Pager* pPager = &mPagersArray[idx];
    pPager->mTimer = -1;
    pPager->mPtrCounter = NULL;

    if (pSound)
    {
        pSound->Release();
        pSound->mpAny.pNextFree = gRoot_sound.mpFreeList;
        gRoot_sound.mpFreeList = pSound;
        pPager->mSound = NULL;
    }
}

MATCH_FUNC(0x5d32d0)
void Hud_Pager_C_Array::ClearClockOnly(s32 pager_idx)
{
    mPagersArray[pager_idx].mTimer = -1;
}

MATCH_FUNC(0x5d32f0)
void Hud_Pager_C_Array::AddTime(s32 pager_idx, s32 time_to_add)
{
    Hud_Pager* pPager = &mPagersArray[pager_idx];
    pPager->mTimer += time_to_add;
}

MATCH_FUNC(0x5d3310)
void Hud_Pager_C_Array::ClearCounterOnly(s32 pager_idx)
{
    mPagersArray[pager_idx].mPtrCounter = NULL;
}

MATCH_FUNC(0x5d3330)
void Hud_Brief::MovePrevBriefToCurrent()
{
    Hud_BriefEntry* pBrief = mPrevBriefs;
    mPrevBriefs = pBrief->mpNext;
    pBrief->mpNext = mCurrBriefs;
    mCurrBriefs = pBrief;
}

MATCH_FUNC(0x5d3350)
void Hud_Brief::FreeCurrentBrief()
{
    Hud_BriefEntry* pBrief = this->mCurrBriefs;
    this->mCurrBriefs = pBrief->mpNext;
    pBrief->mpNext = this->mFreeBriefs;
    this->mFreeBriefs = pBrief;
}

MATCH_FUNC(0x5d3370)
void Hud_Brief::MoveCurrentBriefToPrev()
{
    Hud_BriefEntry* pPrev = this->mCurrBriefs;
    this->mCurrBriefs = pPrev->mpNext;
    pPrev->mpNext = this->mPrevBriefs;
    this->mPrevBriefs = pPrev;
    pPrev->mBriefPriority = 0;
}

// ----------------------------------------------------

MATCH_FUNC(0x5d33a0)
void Hud_Brief::AppendCurrentBriefToPrev()
{
    Hud_BriefEntry* pBrief;
    for (pBrief = mPrevBriefs; pBrief->mpNext; pBrief = pBrief->mpNext)
    {
        ;
    }
    pBrief->mpNext = mCurrBriefs;
    mCurrBriefs->mBriefPriority = 0;
    mCurrBriefs = mCurrBriefs->mpNext;
    pBrief->mpNext->mpNext = NULL;
}

// https://decomp.me/scratch/L1e5G reg swap
MATCH_FUNC(0x5d33f0)
Hud_BriefEntry* Hud_Brief::AllocBrief()
{
    Hud_BriefEntry* pBrief = mFreeBriefs;
    if (pBrief)
    {
        mFreeBriefs = pBrief->mpNext;
    }
    else
    {
        Hud_BriefEntry* pPrev;
        Hud_BriefEntry* pIter;
        pBrief = mPrevBriefs;
        if (mPrevBriefs)
        {
            pIter = mPrevBriefs->mpNext;
            if (pIter)
            {
                do
                {
                    pPrev = pBrief;
                    pBrief = pBrief->mpNext;
                } while (pBrief->mpNext);
                if (pPrev)
                {
                    pPrev->mpNext = NULL;
                }
                else
                {
                    mPrevBriefs = NULL;
                }
            }
            else
            {
                mPrevBriefs = NULL;
            }
        }
        else
        {
            pBrief = mCurrBriefs;
            pIter = pBrief->mpNext;
            if (!pIter)
            {
                mCurrBriefs = NULL;
            }
            else
            {
                do
                {
                    pPrev = pBrief;
                    pBrief = pBrief->mpNext;
                } while (pBrief->mpNext);
                if (pPrev)
                {
                    pPrev->mpNext = NULL;
                }
                else
                {
                    mCurrBriefs = NULL;
                }
            }
        }
    }
    return pBrief;
}

MATCH_FUNC(0x5d3470)
size_t Hud_Brief::FormatCurrentBrief()
{
    LOCAL_STATIC_INIT(s32, sMaxLineWidth, 576, 0x62689C);
    size_t num_chars;
    char_type brief_face_idx;

    if (mCurrBriefs)
    {
        if (mCurrBriefs->mCostParam != -1)
        {
            swprintf(tmpBuff,
                     gText_0x14->Find(mCurrBriefs->mBriefIdStr),
                     mCurrBriefs->mCostParam);
            brief_face_idx = GetBriefFaceIdx(tmpBuff[0]);
            mFaceIdx = brief_face_idx;

            wchar_t* pStartStr;
            if (brief_face_idx)
            {
                pStartStr = &tmpBuff[1]; // ignore next two chars
            }
            else
            {
                pStartStr = tmpBuff;
                mFaceIdx = 8; // neutral face
            }

            mNumLines = gText_0x14->InsertLineBreaksAndGetNumLines(mStr, pStartStr, sMaxLineWidth, gBriefFont);
            num_chars = wcslen(mStr);
            if (bShow_brief_number)
            {
                swprintf(tmpBuff,
                         gText_0x14->Find(mCurrBriefs->mBriefIdStr),
                         mCurrBriefs->mCostParam);
                brief_face_idx = GetBriefFaceIdx(tmpBuff[0]);
                mFaceIdx = brief_face_idx;

                wchar_t* pStartStr_2;
                if (brief_face_idx)
                {
                    pStartStr_2 = &tmpBuff[1]; // ignore next two chars
                }
                else
                {
                    mFaceIdx = 8; // neutral face
                    pStartStr_2 = tmpBuff;
                }
                swprintf(gTmpWideStr, L"(%s)%s", TextTable::Ascii2Wide(mCurrBriefs->mBriefIdStr), pStartStr_2);
                mNumLines = TextTable::InsertLineBreaksAndGetNumLines(mStr, gTmpWideStr, sMaxLineWidth, gBriefFont);
            }
        }
        else
        {
            wchar_t* pBriefText = gText_0x14->Find(mCurrBriefs->mBriefIdStr);
            brief_face_idx = GetBriefFaceIdx(pBriefText[0]);
            mFaceIdx = brief_face_idx;

            if (brief_face_idx)
            {
                ++pBriefText; // ignore next two chars
            }
            else
            {
                mFaceIdx = 8; // neutral face
            }
            mNumLines = gText_0x14->InsertLineBreaksAndGetNumLines(mStr, static_cast<const wchar_t*>(pBriefText), sMaxLineWidth, gBriefFont);
            num_chars = wcslen(mStr);

            if (bShow_brief_number)
            {
                wchar_t* pString = gText_0x14->Find(mCurrBriefs->mBriefIdStr);
                brief_face_idx = GetBriefFaceIdx(pString[0]);
                mFaceIdx = brief_face_idx;

                if (brief_face_idx)
                {
                    ++pString; // ignore next two chars
                }
                else
                {
                    mFaceIdx = 8; // neutral face
                }
                swprintf(gTmpWideStr, L"(%s)%s", TextTable::Ascii2Wide(mCurrBriefs->mBriefIdStr), pString);
                mNumLines = TextTable::InsertLineBreaksAndGetNumLines(mStr, gTmpWideStr, sMaxLineWidth, gBriefFont);
            }
        }
    }
    return num_chars;
}

MATCH_FUNC(0x5d3680)
char_type __stdcall GetBriefFaceIdx(u16 face_char)
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
void Hud_Brief::StartCurrentBrief()
{
    mNumChars = Hud_Brief::FormatCurrentBrief();
    mTickTimer = mNumChars * gHud_2B00->mTextSpeed;
    mFaceVariant = 0;
    mFaceAnimCharIdx = 0;
    mCurrBriefs->mWasDisplayed = 0;
}

// https://decomp.me/scratch/exFU8
MATCH_FUNC(0x5d3b80)
void Hud_Brief::DrawBrief()
{
    if (mCurrBriefs)
    {
        DrawFigureScaled(sprite_types_enum::user_6,
                         mFaceVariant + 3 * mFaceIdx + 16,
                         (32), // x
                         (443), // y
                         kAngZero_706610, // rot
                         palette_types_enum::sprites_2,
                         0,
                         0,
                         0);

        // u32: converts with the Fix16(u32) constructor, whose out-of-line copy is 0x4926F0
        u32 first_line_ypos = 480 - GetLineSpacingFromFontType_5D7700_inlined(gBriefFont) * mNumLines;
        DrawStringScaledPalette(mStr, // str
                                (64), // x
                                first_line_ypos, // y
                                gBriefFont, // fontType
                                palette_types_enum::sprites_2,
                                0, // palette
                                0, // alpha
                                0); // alpha_flag
    }
}

MATCH_FUNC(0x5d3f10)
void Hud_Brief::SetHudBriefWithCost(s32 priority, const char_type* pText, s32 cost_param)
{
    Hud_BriefEntry* pNewBrief = AllocBrief();
    strcpy(pNewBrief->mBriefIdStr, pText);
    pNewBrief->mBriefPriority = priority;
    pNewBrief->mWasDisplayed = 0;
    pNewBrief->mCostParam = cost_param;

    if (!this->mCurrBriefs)
    {
        this->mCurrBriefs = pNewBrief;
        pNewBrief->mpNext = NULL;
        StartCurrentBrief();
    }
    else if (this->mCurrBriefs->mBriefPriority >= priority && priority != hud_brief_priority::now_3)
    {
        Hud_BriefEntry* pIter = this->mCurrBriefs;
        while (pIter->mpNext && pIter->mpNext->mBriefPriority >= priority)
        {
            pIter = pIter->mpNext;
        }

        Hud_BriefEntry* pNext = pIter->mpNext;
        if (pNext)
        {
            pNewBrief->mpNext = pNext;
            pIter->mpNext = pNewBrief;
        }
        else
        {
            pIter->mpNext = pNewBrief;
            pNewBrief->mpNext = NULL;
        }
    }
    else
    {
        if (this->mCurrBriefs->mWasDisplayed)
        {
            MoveCurrentBriefToPrev();
        }
        pNewBrief->mpNext = this->mCurrBriefs;
        this->mCurrBriefs = pNewBrief;
        StartCurrentBrief();
    }
}

// 9.6f 0x4CA610
inline void Hud_Arrow::Reset()
{
    mRadiusPos = kFpZero_7064C0;
    mRepositionSpeed = kArrowBaseRepositionSpeed;
    SetArrowColour(4);
    mTargets.mGangInfo.mIsVisible = true;
    mTargets.mArrowSprtIdx = 0;
    mTargets.mGangInfo.mGang = NULL;
    mTargets.mGangInfo.mMinRespect = 0;
}

// 9.6f 0x4C6FF0
inline void Hud_Arrow::SetMinRadiusPos(s32 steps)
{
    mMinRadiusPos = kArrowRadiusOffset + kArrowMinRadiusStep * steps;
}

MATCH_FUNC(0x5d4400)
void Hud_Brief::SetHudBrief(s32 priority, const char_type* pTextIdStr)
{
    SetHudBriefWithCost(priority, pTextIdStr, -1);
}

MATCH_FUNC(0x5d44d0)
void Hud_Brief::UpdateBrief()
{
    if (mCurrBriefs)
    {
        mTickTimer--;

        if (!(mTickTimer % 3))
        {
            mFaceAnimCharIdx++;
            if (mFaceAnimCharIdx == mNumChars)
            {
                mFaceAnimCharIdx = 0;
            }

            const s32 ary_val = mStr[mFaceAnimCharIdx];
            if (!(ary_val % 20)) // (, <, P, d, x
            {
                mFaceVariant = 1; // blinking (closed eyes)
            }
            else
            {
                mFaceVariant = 2 * (ary_val % 2); // mouth open and closed, but with open eyes
            }
        }

        mCurrBriefs->mWasDisplayed = 1;

        if (mTickTimer == 0)
        {
            MoveCurrentBriefToPrev();
            if (mCurrBriefs)
            {
                StartCurrentBrief();
            }
        }
    }
}

MATCH_FUNC(0x5d4850)
void Hud_Brief::ShowBrief()
{
    if (mPrevBriefs)
    {
        Hud_BriefEntry* pCurrBrief = mCurrBriefs;
        if (pCurrBrief)
        {
            if (pCurrBrief->mWasDisplayed)
            {
                Hud_Brief::AppendCurrentBriefToPrev();
            }
        }
        Hud_Brief::MovePrevBriefToCurrent();
        Hud_Brief::StartCurrentBrief();
    }
}

// https://decomp.me/scratch/N327U
MATCH_FUNC(0x5d4890)
void Hud_Brief::ClearAllBriefsWithPriority(s32 priority)
{
    Hud_BriefEntry* pLast = NULL;
    Hud_BriefEntry* pIter = mCurrBriefs;
    while (pIter)
    {
        if (pIter->mBriefPriority == priority)
        {
            if (pLast)
            {
                pLast->mpNext = pIter->mpNext;
                pIter->mpNext = mFreeBriefs;
                mFreeBriefs = pIter;
                pIter = pLast->mpNext;
            }
            else
            {
                if (mCurrBriefs->mWasDisplayed)
                {
                    Hud_Brief::MoveCurrentBriefToPrev();
                }
                else
                {
                    Hud_Brief::FreeCurrentBrief();
                }
                pIter = mCurrBriefs;
                // Testing the field rather than pIter lets VC6 push ebp only after the first null check
                if (mCurrBriefs)
                {
                    Hud_Brief::StartCurrentBrief();
                }
            }
        }
        else
        {
            pLast = pIter;
            pIter = pIter->mpNext;
        }
    }
}

MATCH_FUNC(0x5d4930)
Hud_Brief::Hud_Brief()
{
    mFreeBriefs = &mBriefs[0];

    mFaceVariant = 0;
    mNumChars = 0;
    mFaceAnimCharIdx = 0;
    mCurrBriefs = NULL;
    mPrevBriefs = NULL;
    mTickTimer = 0;

    for (s32 i = 0; i < GTA2_COUNTOF(mBriefs) - 1; i++)
    {
        mBriefs[i].mpNext = &mBriefs[i + 1];
    }

    mBriefs[GTA2_COUNTOF(mBriefs) - 1].mpNext = NULL;
}

MATCH_FUNC(0x5d4a10)
void Hud_CarName::DrawCarName()
{
    if (mDisplayTime)
    {
        // u32: converts with the Fix16(u32) constructor, whose out-of-line copy is 0x4926F0
        u32 sprite_w = get_user_sprite_width(11);
        if (mTextWidth > static_cast<s32>(sprite_w * 2 - 10))
        {
            DrawFigureScaled(sprite_types_enum::user_6,
                             13,
                             320 + sprite_w,
                             static_cast<u32>(mYpos),
                             kAngZero_706610,
                             palette_types_enum::sprites_2,
                             0,
                             0,
                             0);
            DrawFigureScaled(sprite_types_enum::user_6,
                             12,
                             320,
                             static_cast<u32>(mYpos),
                             kAngZero_706610,
                             palette_types_enum::sprites_2,
                             0,
                             0,
                             0);
            DrawFigureScaled(sprite_types_enum::user_6,
                             11,
                             320 - sprite_w,
                             static_cast<u32>(mYpos),
                             kAngZero_706610,
                             palette_types_enum::sprites_2,
                             0,
                             0,
                             0);
        }
        else
        {
            DrawFigureScaled(sprite_types_enum::user_6,
                             11,
                             320 - (static_cast<s32>(sprite_w) / 2),
                             static_cast<u32>(mYpos),
                             kAngZero_706610,
                             palette_types_enum::sprites_2,
                             0,
                             0,
                             0);
            DrawFigureScaled(sprite_types_enum::user_6,
                             13,
                             320 + (static_cast<s32>(sprite_w) / 2),
                             static_cast<u32>(mYpos),
                             kAngZero_706610,
                             palette_types_enum::sprites_2,
                             0,
                             0,
                             0);
        }

        DrawStringScaled(mCarName, ((640 - mTextWidth) / 2), (mYpos - GetLineSpacingFromFontType(sCarNameFont) / 2), sCarNameFont);
    }
}

MATCH_FUNC(0x5d5190)
void Hud::CalcCarNameTextWidth()
{
    if (mCarName.mDisplayTime)
    {
        mCarName.mTextWidth = Frontend::GetMaxTextWidth(mCarName.mCarName, sCarNameFont);
    }
}

MATCH_FUNC(0x5d5240)
void Hud::ShowCarName(const wchar_t* pName)
{
    mCarName.mDisplayTime = 120;
    wcscpy(mCarName.mCarName, pName);
    gText_0x14->StrToUpper(mCarName.mCarName);
    Hud::CalcCarNameTextWidth();
    mCarName.mYpos = -17;
}

// ----------------------------------------------------

MATCH_FUNC(0x5d5350)
void Hud::UpdateCarName()
{
    Hud_CarName* pCarName = &mCarName;
    if (pCarName->mDisplayTime)
    {
        pCarName->mDisplayTime--;
        if (pCarName->mDisplayTime > 80u)
        {
            ++pCarName->mYpos;
        }
        else if (pCarName->mDisplayTime < 40u)
        {
            --pCarName->mYpos;
        }
    }
}

MATCH_FUNC(0x5d53b0)
Hud_CarName::Hud_CarName()
{
    mDisplayTime = 0;
}

MATCH_FUNC(0x5d53e0)
void Hud_PickupText::CalcTextWidth()
{
    if (mTimer)
    {
        mTextWidth = Frontend::GetMaxTextWidth(mStr, sPickupTextFont);
    }
}

MATCH_FUNC(0x5d5420)
void Hud_PickupText::Draw()
{
    if (mTimer)
    {
        DrawStringScaledPalette(mStr, (640 - mTextWidth) / 2, 32, sPickupTextFont, palette_types_enum::font_remaps_8, 5, 0, 0);
    }
}

MATCH_FUNC(0x5d5600)
void Hud_PickupText::ShowPickupText(u8 pickup_idx)
{
    sprintf(gTmpGxtKey, "c%02d", pickup_idx);
    mTimer = 90;
    wchar_t* pStr = gText_0x14->Find(gTmpGxtKey);
    wcscpy(mStr, pStr);
    CalcTextWidth();
}

MATCH_FUNC(0x5d5690)
void Hud_PickupText::DecrementTimer()
{
    if (mTimer)
    {
        mTimer--;
    }
}

MATCH_FUNC(0x5d56a0)
Hud_PickupText::Hud_PickupText()
{
    mTimer = 0;
}

MATCH_FUNC(0x5d56b0)
void Hud_MpMessage::CalcTextWidth()
{
    if (this->mTimer)
    {
        this->mTextWidth = Frontend::GetMaxTextWidth(mStr, word_7064D8);
    }
}

//https://decomp.me/scratch/QUvWb
MATCH_FUNC(0x5d56d0)
void Hud_MpMessage::Draw()
{
    if (this->mTimer)
    {
        DrawStringScaledPalette(this->mStr,
                                static_cast<u32>((640 - this->mTextWidth) / 2),
                                static_cast<u32>(16),
                                word_7064D8,
                                palette_types_enum::font_remaps_8,
                                5,
                                0,
                                0);
    }
}

MATCH_FUNC(0x5d5730)
void Hud_MpMessage::ShowText(const wchar_t* pStr)
{
    this->mTimer = 120;
    wcscpy(this->mStr, pStr);
    CalcTextWidth();
}

MATCH_FUNC(0x5d5760)
void Hud_MpMessage::DecrementTimer()
{
    if (mTimer)
    {
        mTimer--;
    }
}

//https://decomp.me/scratch/3hVt8
MATCH_FUNC(0x5d5770)
void Hud_MpMessage::AnnounceKill(Player* killer, Player* victim)
{
    if (killer->IsUser())
    {
        if (victim->IsUser())
        {
            swprintf(tmpBuff, gText_0x14->Find("mpkill1"));
            this->ShowText(tmpBuff);
            return;
        }

        swprintf(tmpBuff, L"%s %s", gText_0x14->Find("mpkill2"), victim->mPlayerName);

        gRoot_sound.PlayVoice(voice_line::random_laugh_32);
    }
    else if (victim->IsUser())
    {
        swprintf(tmpBuff, L"%s %s", killer->mPlayerName, gText_0x14->Find("mpkill3"));
    }
    else if (victim == killer)
    {
        swprintf(tmpBuff, L"%s %s", victim->mPlayerName, gText_0x14->Find("mpkill5"));
    }
    else
    {
        swprintf(tmpBuff,
                 L"%s %s %s",
                 killer->mPlayerName,
                 gText_0x14->Find("mpkill4"),
                 &victim->mPlayerName);

        gRoot_sound.PlayVoice(voice_line::random_laugh_31);
    }

    this->ShowText(tmpBuff);
}

MATCH_FUNC(0x5d58f0)
Hud_MpMessage::Hud_MpMessage()
{
    mTimer = 0;
}

MATCH_FUNC(0x5d5900)
void Hud_MapZone::DrawZoneName()
{
    if (mTimer)
    {
        s32 width = get_user_sprite_width(159);

        DrawFigureScaled(sprite_types_enum::user_6,
                         159,
                         static_cast<u32>(320 - (width / 2) - width),
                         static_cast<u32>(27),
                         kAngZero_706610,
                         palette_types_enum::sprites_2,
                         0,
                         mAlphaFlag,
                         mTransparency);

        DrawFigureScaled(sprite_types_enum::user_6,
                         160,
                         320 - (width / 2),
                         static_cast<u32>(27),
                         kAngZero_706610,
                         palette_types_enum::sprites_2,
                         0,
                         mAlphaFlag,
                         mTransparency);

        DrawFigureScaled(sprite_types_enum::user_6,
                         161,
                         (width / 2) + 320,
                         static_cast<u32>(27),
                         kAngZero_706610,
                         palette_types_enum::sprites_2,
                         0,
                         mAlphaFlag,
                         mTransparency);

        DrawFigureScaled(sprite_types_enum::user_6,
                         162,
                         static_cast<u32>((width / 2) + width + 320),
                         static_cast<u32>(27),
                         kAngZero_706610,
                         palette_types_enum::sprites_2,
                         0,
                         mAlphaFlag,
                         mTransparency);

        DrawStringScaledPalette(mWstr,
                                (640 - mTextWidth) / 2,
                                27 - (GetLineSpacingFromFontType(sZoneNameFont) / 2),
                                sZoneNameFont,
                                palette_types_enum::sprites_2,
                                0,
                                mAlphaFlag,
                                mTransparency);
    }
}

MATCH_FUNC(0x5d5ad0)
void Hud_MapZone::CalcTextWidth()
{
    if (mTimer)
    {
        mTextWidth = Frontend::GetMaxTextWidth(mWstr, sZoneNameFont);
    }
}

// ----------------------------------------------------

MATCH_FUNC(0x5d5af0)
void Hud_MapZone::ShowZoneName(gmp_map_zone* pZone1, gmp_map_zone* pZone2)
{
    gmp_map_zone* pNameZone = pZone2;
    if (!pZone2)
    {
        pNameZone = pZone1;
    }

    wchar_t* pStr = pNameZone->get_zone_str();
    if (pStr)
    {
        const wchar_t* pName = gText_0x14->StrToUpper(pStr);
        wcscpy(this->mWstr, pName);
        this->mNavZone = pZone1;
        this->mLocalNavZone = pZone2;
        this->mTimer = 90;
        CalcTextWidth();
        this->mAlphaFlag = 1;
        this->mTransparency = 0;
    }
}

MATCH_FUNC(0x5d5b60)
void Hud_MapZone::UpdateZoneName()
{
    u8 x;
    u8 y;
    u8 z;

    gGame_0x40->mLocalPlayer->GetPosU8(x, y, z);
    gmp_map_zone* navigation_zone = gMap_0x370->GetZoneAtPosOfType(x, y, Navigation_1);
    gmp_map_zone* local_navigation_zone = gMap_0x370->GetZoneAtPosOfType(x, y, Local_Navigation_15);

    if (navigation_zone || local_navigation_zone)
    {
        if (local_navigation_zone == mLocalNavZone && (local_navigation_zone || navigation_zone == mNavZone))
        {
            if (mTimer)
            {
                mTimer--;
                if (mTimer > 0x39u)
                {
                    mTransparency++;
                    if (mTransparency > 0x1Fu)
                    {
                        mAlphaFlag = 0;
                        mTransparency = 31;
                    }
                }
                else
                {
                    if (mTimer < 0x1Fu)
                    {
                        mAlphaFlag = 1;
                        if (mTransparency > 0)
                        {
                            mTransparency--;
                        }
                    }
                }
            }
        }
        else
        {
            Hud_MapZone::ShowZoneName(navigation_zone, local_navigation_zone);
        }
    }
    else
    {
        mTimer = 0;
        mNavZone = NULL;
    }
}

MATCH_FUNC(0x5d5c50)
void Hud_MapZone::ResetTransparency()
{
    mAlphaFlag = 0;
    mTransparency = 0;
}

MATCH_FUNC(0x5d5c60)
Hud_MapZone::Hud_MapZone()
{
    mTimer = 0;
    mNavZone = NULL;
    mLocalNavZone = NULL;
}

MATCH_FUNC(0x5d5c80)
void Hud_PlayerStats::DrawPlayerStats()
{
    Player* pPlayer = gGame_0x40->mLocalPlayer;

    s16 ammo_idx = pPlayer->mCurrWeaponIdx;
    if (ammo_idx == -1)
    {
        DrawAmmo(-1, 0);
    }
    else
    {
        u8 ammo = pPlayer->mWeapons[ammo_idx]->get_ammo_clips();
        DrawAmmo(ammo_idx, ammo);
    }

    s32 powerup_xpos = 639;
    for (s32 powerup_idx = 0; powerup_idx < 17; powerup_idx++)
    {
        u16 powerup_timer = pPlayer->mPowerUpTimers[powerup_idx];
        if (powerup_timer)
        {
            powerup_xpos = DrawPlayerStatsHelper(powerup_idx, powerup_xpos, powerup_timer);
        }
    }

    RollingDigitCounter* pScoreDigits = pPlayer->mScores.GetScoreDigits();
    s32 dollar_sign_xpos = pScoreDigits->DrawDigitsRightAligned(639, 4);

    // Now draw $ symbol

    if (bStartNetworkGame)
    {
        DrawFigureScaled(sprite_types_enum::user_6,
                         16,
                         dollar_sign_xpos - 8,
                         14,
                         kAngZero_706610,
                         pPlayer->mHudPaletteType,
                         pPlayer->mHudPalette,
                         0,
                         0);
    }
    else
    {
        if (pPlayer->mFinishScoreReached == 0)
        {
            DrawFigureScaled(sprite_types_enum::user_6,
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
            DrawFigureScaled(sprite_types_enum::user_6,
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

    if (bStartNetworkGame)
    {
        if (IsTagGame())
        {
            swprintf(buffer,
                     L"%2d:%02d",
                     gTagGame.GetPlayerTime(pPlayer) / 60,
                     gTagGame.GetPlayerTime(pPlayer) % 60);

            const s32 text_palette_type = (pPlayer->mHudPaletteType != palette_types_enum::user_remaps_7) ? palette_types_enum::sprites_2 : palette_types_enum::font_remaps_8;
            DrawStringScaledPalette(buffer, 420, 4, gStyleTextFont, text_palette_type, pPlayer->mHudPalette - 1, 0, 0);
        }
        else
        {
            RollingDigitCounter* pFragDigits = pPlayer->mScores.GetMultiplayerFragDigits();
            pFragDigits->DrawDigitsRightAligned(490, 4);
        }

        s32 ypos = 8;
        for (Player* pMultiPlayer = gGame_0x40->IterateFirstPlayer(); pMultiPlayer != NULL;
             pMultiPlayer = gGame_0x40->IterateNextPlayer())
        {
            if (!pMultiPlayer->IsUser())
            {
                RollingDigitCounter* pMultiScoreDigits = pMultiPlayer->mScores.GetScoreDigits();
                s32 score_end_xpos = pMultiScoreDigits->DrawDigitsLeftAligned(16, ypos);

                DrawFigureScaled(sprite_types_enum::user_6,
                                 16,
                                 8,
                                 ypos + 10,
                                 kAngZero_706610,
                                 pMultiPlayer->mHudPaletteType,
                                 pMultiPlayer->mHudPalette,
                                 0,
                                 0);

                if (IsTagGame())
                {
                    swprintf(buffer,
                             L"%2d:%02d",
                             gTagGame.GetPlayerTime(pMultiPlayer) / 60,
                             gTagGame.GetPlayerTime(pMultiPlayer) % 60);

                    const s32 text_palette_type = (pMultiPlayer->mHudPaletteType != palette_types_enum::user_remaps_7) ? palette_types_enum::sprites_2 : palette_types_enum::font_remaps_8;
                    DrawStringScaledPalette(buffer, score_end_xpos + 20, static_cast<u32>(ypos), gStyleTextFont, text_palette_type, pMultiPlayer->mHudPalette - 1, 0, 0);
                }
                else
                {
                    RollingDigitCounter* pFragDigits = pMultiPlayer->mScores.GetMultiplayerFragDigits();
                    pFragDigits->DrawDigitsLeftAligned(score_end_xpos + 20, ypos);
                }
                ypos += 27;
            }
        }
    }
    else
    {
        s32 lives_xpos = pPlayer->mLives.DrawDigitsRightAligned(523, 28);
        DrawFigureScaled(sprite_types_enum::user_6, 17, lives_xpos - 7, 32, kAngZero_706610, palette_types_enum::sprites_2, 0, 0, 0);

        s32 multiplier_xpos = pPlayer->mMultipliers.DrawDigitsRightAligned(523, 11);
        DrawFigureScaled(sprite_types_enum::user_6,
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
void __stdcall DrawAmmo(s16 ammo_idx, u8 ammo_count)
{
    if (ammo_idx != -1)
    {
        s32 width = get_user_sprite_width(ammo_idx + 85);
        s32 height = get_user_sprite_height(ammo_idx + 85);

        DrawFigureScaled(sprite_types_enum::user_6,
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
                swprintf(tmpBuff, L"K.F.");
            }
            else
            {
                swprintf(tmpBuff, L"%d", ammo_count);
            }
            DrawStringScaledPalette(tmpBuff,
                                    static_cast<u32>(638 - Frontend::GetMaxTextWidth(tmpBuff, sPlayerStatsFont)),
                                    82,
                                    sPlayerStatsFont,
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
static inline u16 GetUserSpriteIndex(gtx_0x106C* pGtx, s32 sprite_pal)
{
    return pGtx->GetSpriteTrueIndex(sprite_types_enum::user_6, sprite_pal);
}

MATCH_FUNC(0x5D61A0)
s32 __stdcall DrawPlayerStatsHelper(s32 powerup_idx, s32 base_xpos, u16 optional_number)
{
    u16 sprite_idx = GetUserSpriteIndex(gGtx_0x106C, powerup_idx + 141);
    s32 width = gGtx_0x106C->get_sprite_width(sprite_idx);
    DrawFigureScaled(sprite_types_enum::user_6,
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
        swprintf(tmpBuff, L"%d", optional_number);
        // The x goes through the Fix16(u32) constructor (out-of-line copy 0x4926F0). The offset is
        // computed before the y constructor call, so it is a local
        s32 x_offset = optional_number < 10 ? 18 : 22;
        DrawStringScaledPalette(tmpBuff,
                                static_cast<u32>(base_xpos - x_offset),
                                127,
                                sPlayerStatsFont,
                                palette_types_enum::font_remaps_8,
                                6,
                                0,
                                0);
    }
    return base_xpos - width;
}

// ----------------------------------------------------

MATCH_FUNC(0x5d6290)
void Hud_PlayerStats::UpdateRollingDigits()
{
    Player* pPlayerIter = gGame_0x40->IterateFirstPlayer();
    while (pPlayerIter)
    {
        RollingDigitCounter* pScoreDigits = pPlayerIter->mScores.GetScoreDigits();
        pScoreDigits->UpdateRollingDigits();
        RollingDigitCounter* pFragDigits = pPlayerIter->mScores.GetMultiplayerFragDigits();
        pFragDigits->UpdateRollingDigits();
        pPlayerIter = gGame_0x40->IterateNextPlayer();
    }
    Player* pPlayer = gGame_0x40->mLocalPlayer;
    pPlayer->mLives.UpdateRollingDigits();
    pPlayer->mMultipliers.UpdateRollingDigits();
}

// ----------------------------------------------------

// Not in IDA's function list: only reached through the UpdatePauseSection thunk below.
MATCH_FUNC(0x5D6300)
void Hud_PauseScreen::UpdatePauseSection()
{
    if (!gGameSession.IsBonusStage())
    {
        mTimer--;
        if (mTimer == 0)
        {
            mTimer = 45;
            mCurrentPauseSection++;

            if (mCurrentPauseSection > 6)
            {
                mCurrentPauseSection = 0;
            }

            if (mCurrentPauseSection == HudPauseSection::gang_1_missions_done_1)
            {
                if (!gGangPool_CA8->FirstGang())
                {
                    mCurrentPauseSection = HudPauseSection::all_missions_done_4;
                }
                return;
            }

            if (mCurrentPauseSection == HudPauseSection::gang_2_missions_done_2)
            {
                if (gGangPool_CA8->FirstGang())
                {
                    if (!gGangPool_CA8->NextGang())
                    {
                        mCurrentPauseSection = HudPauseSection::all_missions_done_4;
                    }
                    return;
                }
                mCurrentPauseSection = HudPauseSection::all_missions_done_4;
                return;
            }

            if (mCurrentPauseSection == HudPauseSection::gang_3_missions_done_3 &&
                (!gGangPool_CA8->FirstGang() || !gGangPool_CA8->NextGang() || !gGangPool_CA8->NextGang()))
            {
                mCurrentPauseSection = HudPauseSection::all_missions_done_4;
                return;
            }
        }
    }
}

// https://decomp.me/scratch/Uq97l
MATCH_FUNC(0x5d63b0)
void Hud_PauseScreen::DrawPause()
{

    u32 sprite_type;
    u16 sprite_pal = 0;
    if (gGame_0x40->IsPaused() && !gGame_0x40->mLocalPlayer->mShowQuitMessage)
    {
        DrawFigureScaled(sprite_types_enum::user_6, 134, 227, 180, kAngZero_706610, palette_types_enum::sprites_2, 0, 0, 0);
        DrawFigureScaled(sprite_types_enum::user_6, 136, 320, 180, kAngZero_706610, palette_types_enum::sprites_2, 0, 0, 0);
        DrawFigureScaled(sprite_types_enum::user_6, 135, 413, 180, kAngZero_706610, palette_types_enum::sprites_2, 0, 0, 0);

        wchar_t* pPauseText = gText_0x14->Find("pause");
        s32 max_width = Frontend::GetMaxTextWidth(pPauseText, sPauseFont);

        // u32: converts with the Fix16(u32) constructor, whose out-of-line copy is 0x4926F0
        u32 y_offset = !gText_0x14->LangIsJapanese() ? 158 : 164;

        DrawStringScaledPalette(pPauseText, (640 - max_width) / 2, y_offset, sPauseFont, palette_types_enum::sprites_2, 0, 0, 0);

        if (!gGameSession.IsBonusStage())
        {
            s32 passed_count;
            Gang* pGang;
            switch (mCurrentPauseSection)
            {
                case HudPauseSection::target_score_0:
                    swprintf(tmpBuff, gText_0x14->Find("pscore"), gScriptManager->mFinishScore);
                    break;

                case HudPauseSection::gang_1_missions_done_1:
                    pGang = gGangPool_CA8->FirstGang();
                    if (gScriptManager->m1PassedFlag)
                    {
                        passed_count = *gScriptManager->m1PassedFlag;
                    }
                    else
                    {
                        passed_count = 0;
                    }
                    swprintf(tmpBuff,
                             gText_0x14->Find("pgmiss"),
                             pGang->GetArrowColourText(),
                             passed_count,
                             gScriptManager->mGang1MissionsTotal);
                    sprite_type = sprite_types_enum::user_6;
                    sprite_pal = pGang->mArrowColour + 63;
                    break;

                case HudPauseSection::gang_2_missions_done_2:
                    gGangPool_CA8->FirstGang();
                    pGang = gGangPool_CA8->NextGang();
                    if (gScriptManager->m2PassedFlag)
                    {
                        passed_count = *gScriptManager->m2PassedFlag;
                    }
                    else
                    {
                        passed_count = 0;
                    }
                    swprintf(tmpBuff,
                             gText_0x14->Find("pgmiss"),
                             pGang->GetArrowColourText(),
                             passed_count,
                             gScriptManager->mGang2MissionsTotal);
                    sprite_type = sprite_types_enum::user_6;
                    sprite_pal = pGang->mArrowColour + 63;
                    break;

                case HudPauseSection::gang_3_missions_done_3:
                    gGangPool_CA8->FirstGang();
                    gGangPool_CA8->NextGang();
                    pGang = gGangPool_CA8->NextGang();
                    if (gScriptManager->m3PassedFlag)
                    {
                        passed_count = *gScriptManager->m3PassedFlag;
                    }
                    else
                    {
                        passed_count = 0;
                    }
                    swprintf(tmpBuff,
                             gText_0x14->Find("pgmiss"),
                             pGang->GetArrowColourText(),
                             passed_count,
                             gScriptManager->mGang3MissionsTotal);
                    sprite_type = sprite_types_enum::user_6;
                    sprite_pal = pGang->mArrowColour + 63;
                    break;

                case HudPauseSection::all_missions_done_4:
                    if (gScriptManager->mPassedFlag)
                    {
                        passed_count = *gScriptManager->mPassedFlag;
                    }
                    else
                    {
                        passed_count = 0;
                    }
                    swprintf(tmpBuff,
                             gText_0x14->Find("pmiss"),
                             passed_count,
                             gScriptManager->mTotalMissions);
                    break;

                case HudPauseSection::kill_frenzies_completed_5:
                    if (gScriptManager->mSecretsPassed)
                    {
                        passed_count = *gScriptManager->mSecretsPassed;
                    }
                    else
                    {
                        passed_count = 0;
                    }
                    swprintf(tmpBuff,
                             gText_0x14->Find("psec"),
                             passed_count,
                             gScriptManager->mTotalSecrets);
                    sprite_type = sprite_types_enum::code_obj1_4;
                    sprite_pal = gObjectDefinitions->GetObjectBaseSpriteIdx(objects::kill_frenzy_icon_286);
                    break;

                case HudPauseSection::tokens_collected_6:
                    swprintf(tmpBuff,
                             gText_0x14->Find("pbon"),
                             gGameSession.get_secret_tokens_collected(),
                             50);
                    sprite_type = sprite_types_enum::code_obj1_4;
                    sprite_pal = gObjectDefinitions->GetObjectBaseSpriteIdx(objects::secret_token_266);
                    break;
                default:
                    break;
            }

            s32 max_width_2 = Frontend::GetMaxTextWidth(tmpBuff, word_7064D8);
            s32 text_xpos;
            if (sprite_pal != 0)
            {
                u16 sprite_idx = gGtx_0x106C->GetSpriteTrueIndex(sprite_type, sprite_pal);
                s32 icon_width = gGtx_0x106C->get_sprite_width(sprite_idx) + 10;
                text_xpos = (640 - icon_width - max_width_2) / 2;
                DrawFigureScaled(sprite_type,
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
            DrawStringScaledPalette(tmpBuff, static_cast<u32>(text_xpos), 220, word_7064D8, palette_types_enum::font_remaps_8, 6, 0, 0);
        }
    }
}

MATCH_FUNC(0x5d6860)
void Hud::DrawGui()
{
    if (!bSkip_user)
    {
        SetFullAmbient();
        mPlayerStats.DrawPlayerStats();
        mUnderRoofArrowMarker.Draw();
        mPlayerNames.DrawPlayerNames();
        mWantedLevel.DrawWantedLevel();
        mGangRespectBars.DrawGangRespectBars();
        mHealth.DrawHealth();
        mZoneName.DrawZoneName();
        mCarName.DrawCarName();
        mPickupText.Draw();
        mBrief.DrawBrief();
        mPagers.DrawPagers();
        mTexts.Service();
        mArrows.DrawArrows();
        mMpMessage.Draw();
        mMessage.DrawMessage();
        mPauseScreen.DrawPause();
        mChatInput.DrawChatMessages();
        mQuitMessage.DrawQuitMessage();
    }
}

MATCH_FUNC(0x5d69c0)
void Hud::UpdatePauseSection()
{
    mPauseScreen.UpdatePauseSection();
}

MATCH_FUNC(0x5d69d0)
void Hud::UpdateHUD()
{
    mPlayerStats.UpdateRollingDigits();
    mUnderRoofArrowMarker.Update();
    mShowCoords.ShowPlayerCoords();
    mWantedLevel.UpdateWantedLevel();
    UpdateCarName();
    mPickupText.DecrementTimer();
    mZoneName.UpdateZoneName();
    mBrief.UpdateBrief();
    mPagers.UpdatePagers();
    mTexts.RemoveExpired();
    mArrows.UpdateArrows();
    mGangRespectBars.UpdateGangRespectBars();
    mMessage.DecrementTimeToShow();
    mMpMessage.DecrementTimer();
}

MATCH_FUNC(0x5d6a70)
void Hud::SetFullAmbient()
{
    if (gLighting)
    {
        pgbh_SetAmbient(1.0);
    }
}

MATCH_FUNC(0x5d6a90)
void Hud::GetTextSpeed()
{
    mTextSpeed = gRegistry.Get_Option_Int("text_speed", 3);
}

MATCH_FUNC(0x5d6ab0)
void Hud::RecalcTextLayout()
{
    SetFontTypes();
    mBrief.FormatCurrentBrief();
    CalcCarNameTextWidth();
    mZoneName.CalcTextWidth();
    mMessage.LayoutMessage();
    mMpMessage.CalcTextWidth();
    mPickupText.CalcTextWidth();
}

MATCH_FUNC(0x5d6b00)
void Hud::SetFontTypes()
{
    if (gText_0x14->LangIsJapanese())
    {
        sPickupTextFont = 103;
        sCarNameFont = 104;
        sZoneNameFont = 105;
        gBriefFont = 107;
        sPlayerStatsFont = gStyleTextFont;
        gMessageFont_7062F0 = 203;
        gDebugFont = 103;
        sPauseFont = 201;
        word_7064D8 = 103;
        sPlayerNameFont = 103;
        sChatDisplayFont = 103;
    }
    else
    {
        sCarNameFont = gStyleCarNameFont;
        sPickupTextFont = gStyleTextFont;
        sZoneNameFont = gStyleZoneNameFont;
        gBriefFont = gStyleTextFont;
        sPlayerStatsFont = gStyleTextFont;
        gMessageFont_7062F0 = gStyleMessageFont;
        gDebugFont = gStyleTextFont;
        word_7064D8 = gStyleTextFont;
        sPauseFont = gStylePauseFont;
        sPlayerNameFont = gStyleTextFont;
        sChatDisplayFont = gStyleTextFont;
    }
}

MATCH_FUNC(0x5d6be0)
void Hud::Init()
{
    GetTextSpeed();
    SetFontTypes();
    mZoneName.ResetTransparency();
    mGangRespectBars.InitGangRespectBars();
    mWantedLevel.Init();
    mArrows.CreatePlayerArrows();
}

MATCH_FUNC(0x5d6c20)
bool Hud::HandleInputKey(s32 action, Player* pPlayer)
{
    return mQuitMessage.HandleQuitMessageKey(action, pPlayer) || mChatInput.HandleChatInputKey(action, pPlayer);
}

MATCH_FUNC(0x5d6c70)
bool Hud::IsInputKeyConsumed(s32 action)
{
    return mQuitMessage.IsQuitMessageKey(action) || mChatInput.IsChatInputKey(action);
}

MATCH_FUNC(0x5d6cb0)
bool Hud::IsQuitMessageInputKey(s32 action)
{
    return mQuitMessage.IsQuitMessageKey(action);
}

// Defined before the member ctors it calls: when VC6 has already compiled them in this TU (and seen
// that they can't throw) it drops the EH frame and the state for the pager array that the original has.
MATCH_FUNC(0x5d6cd0)
Hud::Hud()
{
    mTextSpeed = 0;
}

MATCH_FUNC(0x5d7510)
Hud_CopHead::Hud_CopHead()
{
    mFrame = 0;
    mFrameDelay = 0;
    mFrameTimer = 0;
    mHeight = 0;
    mVelocity = -1;
}

MATCH_FUNC(0x5d7600)
Hud_Arrow::Hud_Arrow()
{
    mTargets.mGangInfo.mGang = NULL;
    mTargets.mGangInfo.mMinRespect = 0;
    mTargets.mGangInfo.mIsVisible = false;

    mTargets.mPrimaryTarget.init();
    mTargets.mSecondaryTarget.init();

    mTargets.mCurrTarget = &mTargets.mPrimaryTarget;
    mTargets.mTargetSwapTimer = 0;
    mTargets.mGangInfo.mInUse = false;
}

MATCH_FUNC(0x5d7650)
Hud_Pager::Hud_Pager()
{
    mTimer = -1;
    mPtrCounter = NULL;
    mSound = NULL;
}

