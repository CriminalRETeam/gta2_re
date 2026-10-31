#pragma once

#include "Draw.hpp"
#include "Function.hpp"
#include "ang16.hpp"
#include "fix16.hpp"
#include "gtx_0x106C.hpp"
#include "map_0x370.hpp"
#include <wchar.h>

class Ped;
class Player;
class Gang;
class SoundObject;
class Object;
class Car;

class Hud_PlayerNames
{
  public:
    EXPORT void DrawPlayerNames();
    s32 mUnused0; // 0x0
};
GTA2_ASSERT_SIZEOF_ALWAYS(Hud_PlayerNames, 0x4);

class Hud_MpMessage
{
  public:
    EXPORT void CalcTextWidth();
    EXPORT void Draw();
    EXPORT void ShowText(const wchar_t* pStr);
    EXPORT void DecrementTimer();
    EXPORT void AnnounceKill(Player* killer, Player* victim);
    EXPORT Hud_MpMessage();
    char_type mTimer; // 0x0
    wchar_t mStr[101]; // 0x2
    s32 mTextWidth; // 0xCC
};
GTA2_ASSERT_SIZEOF_ALWAYS(Hud_MpMessage, 0xD0);

class Hud_ChatInput
{
  public:
    EXPORT bool HandleChatInputKey(s32 action, Player* pPlayer);
    EXPORT void DrawChatMessages();
    EXPORT bool IsChatInputKey(s32 key_idx);
    EXPORT void StartChatting(Player* pPlayer);
};
GTA2_ASSERT_SIZEOF_ALWAYS(Hud_ChatInput, 0x1);

class Hud_QuitMessage
{
  public:
    EXPORT bool HandleQuitMessageKey(s32 action, Player* pPlayer);
    EXPORT void DrawQuitMessage();
    EXPORT bool IsQuitMessageKey(s32 action);
    EXPORT void ShowQuitMessage(Player* pPlayer);
    char_type mQuitMessage; // 0x12EC
};
GTA2_ASSERT_SIZEOF_ALWAYS(Hud_QuitMessage, 0x1);

class Hud_BriefSelector
{
  public:
    // inline 0x4C6AC0
    Hud_BriefSelector()
    {
        mValue = 0;
    }
    EXPORT void ShowNextNumberedBrief();
    EXPORT void ShowPrevNumberedBrief();
    s32 mValue; // 0x0
};
GTA2_ASSERT_SIZEOF_ALWAYS(Hud_BriefSelector, 0x4);

namespace HudPauseSection
{
enum
{
    target_score_0 = 0,
    gang_1_missions_done_1 = 1,
    gang_2_missions_done_2 = 2,
    gang_3_missions_done_3 = 3,
    all_missions_done_4 = 4,
    kill_frenzies_completed_5 = 5,
    tokens_collected_6 = 6,
};
} // namespace HudPauseSection

class Hud_PauseScreen
{
  public:
    // inline 0x4C71A0
    Hud_PauseScreen()
    {
        mCurrentPauseSection = 0;
        mTimer = 45;
    }
    EXPORT void DrawPause();
    EXPORT void UpdatePauseSection();
    u8 mCurrentPauseSection; // 0x0
    char_type mTimer; // 0x1
};
GTA2_ASSERT_SIZEOF_ALWAYS(Hud_PauseScreen, 0x2);

class Hud_Message
{
  public:
    EXPORT void ClearTimeToShow();
    EXPORT void FormatMessage_5D1860();
    EXPORT void DrawMessage();
    EXPORT void ShowMessage(const wchar_t* pStr, s32 priority);
    EXPORT void DecrementTimeToShow();
    EXPORT Hud_Message();
    u8 mTimeToShow; // 0x0
    wchar_t mStr[221]; // 0x2
    s32 mXpos; // 0x1BC
    s32 mYpos; // 0x1C0
    s32 mPriority; // 0x1C4: hud_message_priority (hud_message_priority.hpp)
};
GTA2_ASSERT_SIZEOF_ALWAYS(Hud_Message, 0x1C8);

class Hud_PlayerStats
{
  public:
    EXPORT void DrawPlayerStats();
    EXPORT void UpdateRollingDigits();
    s32 mUnused0; // 0x0
};
GTA2_ASSERT_SIZEOF_ALWAYS(Hud_PlayerStats, 0x4);

class Hud_UnderRoofArrowMarker
{
  public:
    // inline 0x4C6E50
    Hud_UnderRoofArrowMarker()
    {
        mRotation = 0;
        mPedUnderSolid = false;
    }
    EXPORT void Update();
    EXPORT void Draw();
    Fix16 mScreenX; // 0x0
    Fix16 mScreenY; // 0x4
    Ang16 mRotation; // 0x8
    bool mPedUnderSolid; // 0xA
    char_type mUnusedB; // 0xB
};
GTA2_ASSERT_SIZEOF_ALWAYS(Hud_UnderRoofArrowMarker, 0xC);

class Hud_Health
{
  public:
    EXPORT void DrawHealth();
    s32 mUnused0; // 0x0
};
GTA2_ASSERT_SIZEOF_ALWAYS(Hud_Health, 0x4);

class Hud_PickupText
{
  public:
    EXPORT void CalcTextWidth();
    EXPORT void Draw();
    EXPORT void ShowPickupText(u8 pickup_idx);
    EXPORT void DecrementTimer();
    EXPORT Hud_PickupText();
    char_type mTimer; // 0x0
    wchar_t mStr[65]; // 0x2
    s32 mTextWidth; // 0x84
};
GTA2_ASSERT_SIZEOF_ALWAYS(Hud_PickupText, 0x88);

class Hud_ShowCoords
{
  public:
    // inline 0x4C6E70
    Hud_ShowCoords()
    {
        mShowCoords = false;
    }
    EXPORT void ShowPlayerCoords();

    // 9.6f 0x4A4760
    inline void ToggleShowCoords_4A4760()
    {
        mShowCoords = !mShowCoords;
    }

    bool mShowCoords; // 0x0
};
GTA2_ASSERT_SIZEOF_ALWAYS(Hud_ShowCoords, 0x1);

class Hud_GangRespectBars
{
  public:
    EXPORT void DrawGangRespectBars();
    EXPORT void Empty_5CFE20();
    EXPORT void Empty_5CFE30();
    char_type mGangRespectBars; // 0x107C
};
GTA2_ASSERT_SIZEOF_ALWAYS(Hud_GangRespectBars, 0x1);

class Hud_CopHead
{
  public:
    EXPORT void UpdateHead(bool bShakeHead);
    EXPORT Hud_CopHead();
    u8 mFrame; // 0x0
    char_type mFrameTimer; // 0x1
    char_type mFrameDelay; // 0x2
    s32 mHeight; // 0x4
    s32 mVelocity; // 0x8
};
GTA2_ASSERT_SIZEOF_ALWAYS(Hud_CopHead, 0xC);

class Hud_CopHead_C_Array
{
  public:
    // inline 0x4C6EE0
    Hud_CopHead_C_Array()
    {
        mCopLevel = 0;
    }
    EXPORT void UpdateWantedLevel();
    EXPORT void DrawWantedLevel();
    EXPORT void Init();
    Hud_CopHead mCopHeads[6]; // 0x1028
    s32 mCopLevel; // 0x48
    Fix16 mWFp; // 0x4C
    Fix16 mHFp; // 0x50
};

class Hud_TextEntry
{
  public:
    EXPORT void FormatAndSetupText(const wchar_t* pStr, s16 xpos, s16 ypos, s16 fontType, s32 displayTime);
    EXPORT void Draw();
    EXPORT bool DecrementDisplayTime();
    EXPORT bool operator_equals(const Hud_TextEntry* pOther) const;

    // 9.6f 0x4C70F0
    inline void Expire_4C70F0()
    {
        mDisplayTime = 0;
    }

    // 9.6f 0x4C70E0
    inline void ClearAlpha_4C70E0()
    {
        mAlpha = 0;
        *reinterpret_cast<u8*>(&mAlphaFlag) = 0; // byte store in both 9.6f and 10.5, the field is read as s32 elsewhere
    }

    // 9.6f 0x45AFD0
    void SetFontRemap_45AFD0(s16 palette)
    {
        mDrawKind = 8;
        mPalette = palette;
    }
    wchar_t mStrBuf[82]; // 0x0
    s32 mDisplayTime; // 0xA4
    u16 mX; // 0xA8
    u16 mY; // 0xAA
    s16 mFontType; // 0xAC
    s32 mDrawKind; // 0xB0
    s16 mPalette; // 0xB4
    s32 mAlpha; // 0xB8
    s32 mAlphaFlag; // 0xBC
    Hud_TextEntry* mpNext; // 0xC0
};
GTA2_ASSERT_SIZEOF_ALWAYS(Hud_TextEntry, 0xC4);

class Hud_TextList
{
  public:
    EXPORT void ExpireDuplicates(const Hud_TextEntry* pEntry);
    EXPORT Hud_TextEntry* DisplayText(const wchar_t* pStr, s16 xpos, s16 ypos, s16 font_type, s32 display_time);
    EXPORT void Service();
    EXPORT void RemoveExpired();
    EXPORT Hud_TextList();

    // A free list written out by hand: the entries come before the list heads, unlike Pool<>
    Hud_TextEntry m29Ary[30]; // 0x0
    Hud_TextEntry* mpFirst; // 0x960
    Hud_TextEntry* mpFreeList; // 0x964
};
GTA2_ASSERT_SIZEOF_ALWAYS(Hud_TextList, 0x1700);

class Hud_Pager
{
  public:
    // 9.6f 0x4C7160
    inline bool no_ptr_counter_4C7160() const
    {
        return mPtrCounter == NULL;
    }

    // 9.6f 0x4C7170
    inline bool IsTimerOff_4C7170() const
    {
        return mTimer < 0;
    }

    // 9.6f 0x4C7120
    inline void SetTimer_4C7120(s32 timer)
    {
        mTimer = timer;
    }

    // 9.6f 0x4C7130, defined in Hud.cpp
    inline void SetCounter_4C7130(s32* pCounter);

    // 9.6f 0x411A40
    inline s32 get_timer_411A40() const
    {
        return mTimer;
    }

    EXPORT ~Hud_Pager();
    EXPORT void Service();
    EXPORT void DrawCounterDigits(s32 xpos, s32 ypos);
    EXPORT void DrawDigits(s32 xpos, s32 ypos);
    EXPORT void DrawPager(u32 xpos, s32 ypos);

    EXPORT Hud_Pager();
    s32 mTimer; // 0x0
    s32* mPtrCounter; // 0x4
    SoundObject* mSound; // 0x8
};
GTA2_ASSERT_SIZEOF_ALWAYS(Hud_Pager, 0xC);

class Hud_Pager_C_Array
{
  public:
    // inline 0x4CA660
    Hud_Pager_C_Array()
    {
    }

    EXPORT s32 AddOnScreenCounter(s32* pCounter);
    EXPORT void ClearPager(s32 idx);

    EXPORT void DrawPagers();
    EXPORT void UpdatePagers();
    EXPORT s32 CreateTimer(s32 seconds);
    EXPORT void ClearClockOnly(s32 pager_idx);
    EXPORT void AddTime(s32 pager_idx, s32 time_to_add);
    EXPORT void ClearCounterOnly(s32 pager_idx);

    Hud_Pager mPagersArray[4]; // 0x0
};

inline u8 __stdcall get_sprite_width_4C7220(s16 user_sprite_idx)
{
    s16 sprite_idx = gGtx_0x106C->GetSpriteTrueIndex(sprite_types_enum::user_6, user_sprite_idx);
    return gGtx_0x106C->get_sprite_width_420220(sprite_idx);
}

inline u8 __stdcall get_sprite_height_4C7250(s16 user_sprite_idx)
{
    s16 sprite_idx = gGtx_0x106C->GetSpriteTrueIndex(sprite_types_enum::user_6, user_sprite_idx);
    return gGtx_0x106C->get_sprite_height_4C6C90(sprite_idx);
}

class Hud_BriefEntry
{
  public:
    char_type mBriefIdStr[8]; // 0x0: gxt key, e.g. "1_3"
    s32 mBriefPriority; // 0x8
    Hud_BriefEntry* mpNext; // 0xC
    u8 mWasDisplayed; // 0x10
    s32 mCostParam; // 0x14
};
GTA2_ASSERT_SIZEOF_ALWAYS(Hud_BriefEntry, 0x18);

namespace ArrowTargetType
{
enum
{
    Nothing_0 = 0,
    XYZ_Coord_1 = 1,
    Ped_2 = 2,
    Car_3 = 3,
    Object_4 = 4,
    InfoPhone_5 = 5,
    Player_6 = 6 // Multiplayer arrows
};
} // namespace ArrowTargetType

class Hud_ArrowGangInfo
{
  public:
    Gang* mGang; // 0x30
    char_type mMinRespect; // 0x34
    char_type mIsVisible; // 0x5: not sure
    char_type mInUse; // 0x6
};
GTA2_ASSERT_SIZEOF_ALWAYS(Hud_ArrowGangInfo, 0x8);

class ArrowTrace
{
  public:
    EXPORT void PointToInfoPhone(Gang* pZone);
    EXPORT void UpdateAimCoordinates();

    void SetTargetCar(Car* pCar)
    {
        mCar = pCar;
        mTargetType = ArrowTargetType::Car_3;
    }

    // 9.6f 0x4767E0
    inline void SetTargetPed_4767E0(Ped* pPed)
    {
        mPed = pPed;
        mTargetType = ArrowTargetType::Ped_2;
    }

    // 9.6f 0x476810
    inline void SetTargetObject_476810(Object* pObj)
    {
        mObj = pObj;
        mTargetType = ArrowTargetType::Object_4;
    }

    // 9.6f 0x476830
    inline void ClearTarget_476830()
    {
        mTargetType = ArrowTargetType::Nothing_0;
    }

    // inline 0x4C6F00
    void init()
    {
        mPed = NULL;
        mCar = NULL;
        mObj = NULL;
        mPlayer = NULL;
        mTargetType = ArrowTargetType::Nothing_0;
        mbIsTargetVisible = false;
    }

    // 9.6f 0x4767C0
    inline void set_arrow_aim_from_pos_4767C0(Fix16 x, Fix16 y, Fix16 z)
    {
        mAimX = x;
        mAimY = y;
        mAimZ = z;
    }

    // 9.6f inline 0x4C6F20
    inline bool HasNoTarget_4C6F20() const
    {
        return mTargetType == ArrowTargetType::Nothing_0;
    }
    
    // 9.6f 0x4C6F30
    inline Player* GetTargetPlayer_4C6F30() const
    {
        return mPlayer;
    }

    // 9.6f inline 0x4820A0. The player is stored first (VC7 gives 9.6f's body either way): with the
    // type store first, SetNewFugitive schedules and allocates registers differently
    inline void SetTargetPlayer_4820A0(Player* player)
    {
        mPlayer = player;
        mTargetType = ArrowTargetType::Player_6;
    }

    Ped* mPed; // 0x0
    Car* mCar; // 0x4
    Object* mObj; // 0x8
    Player* mPlayer; // 0xC
    s32 mTargetType; // 0x10
    Fix16 mAimX; // 0x14
    Fix16 mAimY; // 0x18
    Fix16 mAimZ; // 0x1C
    char_type mbIsTargetVisible; // 0x20
};
GTA2_ASSERT_SIZEOF_ALWAYS(ArrowTrace, 0x24);

class Hud_ArrowTargets
{
  public:

    // 9.6f 0x4C6FB0
    inline bool HasBothTargets_4C6FB0() const
    {
        return !mPrimaryTarget.HasNoTarget_4C6F20() && !mSecondaryTarget.HasNoTarget_4C6F20();
    }

    s32 mUnused20; // 0x20
    s16 mUnused24; // 0x24
    s32 mArrowColour; // 0x28
    s16 mArrowSprtIdx; // 0x2C
    u8 mTargetSwapTimer; // 0x2E
    Hud_ArrowGangInfo mGangInfo; // 0x10
    ArrowTrace mPrimaryTarget; // 0x18
    ArrowTrace mSecondaryTarget; // 0x3C
    ArrowTrace* mCurrTarget; // 0x60
};
GTA2_ASSERT_SIZEOF_ALWAYS(Hud_ArrowTargets, 0x64);

class Hud_Arrow
{
  public:
    // 9.6f 0x4CA610, defined in Hud.cpp
    inline void Reset_4CA610();

    // 9.6f 0x4C6FF0, defined in Hud.cpp
    inline void SetMinRadiusPos_4C6FF0(s32 steps);

    // 9.6f 0x4C6F20
    inline bool Is_radius_pos_0_4C6F20() const
    {
        return (mRadiusPos == 0) != 0;
    }

    EXPORT void SetArrowColour(s32 arrow_colour);
    EXPORT bool CheckVisibility();
    EXPORT bool UpdateTargets();
    EXPORT void UpdateScreenPos();
    EXPORT void Service();
    EXPORT void DrawArrow();
    EXPORT void SetPlayerArrowColour(Ped* pPed);

    // 9.6f inline 0x4C6F80
    inline bool HasNeitherTarget_4C6F80() const
    {
        if (mTargets.mPrimaryTarget.HasNoTarget_4C6F20() && mTargets.mSecondaryTarget.HasNoTarget_4C6F20())
        {
            return true;
        }
        return false;
    }

    // 9.6f inline 0x4C7050
    inline bool IsVisible_4C7050() const
    {
        if (mTargets.mGangInfo.mIsVisible)
        {
            return true;
        }
        return false;
    }

    // 9.6f 0x4C7060
    inline void swap_arrows_4C7060()
    {
        if (mTargets.mCurrTarget == &mTargets.mPrimaryTarget)
        {
            mTargets.mCurrTarget = &mTargets.mSecondaryTarget;
        }
        else
        {
            mTargets.mCurrTarget = &mTargets.mPrimaryTarget;
        }
    }

    // 9.6f 0x476850
    inline void SetArrowTargetPed_476850(Ped* pPed)
    {
        mTargets.mPrimaryTarget.SetTargetPed_4767E0(pPed);
    }

    // 9.6f 0x476860
    inline void SetArrowTargetCar_476860(Car* pCar)
    {
        mTargets.mPrimaryTarget.SetTargetCar(pCar);
    }

    // 9.6f 0x476870
    inline void SetArrowTargetObject_476870(Object* pObj)
    {
        mTargets.mPrimaryTarget.SetTargetObject_476810(pObj);
    }

    // 9.6f 0x476880
    inline void ClearTargets_476880()
    {
        mTargets.mPrimaryTarget.ClearTarget_476830();
        mTargets.mSecondaryTarget.ClearTarget_476830();
    }

    // 9.6f 0x476840
    inline void SetArrowAim_476840(Fix16 xpos, Fix16 ypos, Fix16 zpos)
    {
        ArrowTrace* pTarget = &mTargets.mPrimaryTarget;
        pTarget->mAimX = xpos;
        pTarget->mAimY = ypos;
        pTarget->mAimZ = zpos;
        pTarget->mTargetType = ArrowTargetType::XYZ_Coord_1;
    }

    EXPORT Hud_Arrow();
    Fix16 mScreenPosX; // 0x0: x and y are not independent from mRadiusPos
    Fix16 mScreenPosY; // 0x4
    Ang16 mRotation; // 0x8
    Fix16 mMinRadiusPos; // 0xC: minimum radial distance from the player
    Fix16 mRadiusPos; // 0x10: radial distance from the player
    Fix16 mRepositionSpeed; // 0x14: how slower/faster the arrow goes to the aim target, or "get back" to the player
    Hud_ArrowTargets mTargets; // 0x18
};
GTA2_ASSERT_SIZEOF_ALWAYS(Hud_Arrow, 0x7C);

class Hud_Arrow_7C_Array
{
  public:
    // 9.6f 0x4C70B0
    inline bool ShowGangArrows_4C70B0() const
    {
        return mShowGangArrows;
    }

    // inline 0x4C7080
    Hud_Arrow_7C_Array()
    {
        mShowGangArrows = true;
        mVisibleGangArrow = NULL;
        mCheckInfoPhones = false;
    }

    EXPORT void CreatePlayerArrows();
    EXPORT bool IsThereAnyOtherArrowsInSameGang(Hud_Arrow* pArgArrow);
    EXPORT void DrawArrows();
    EXPORT void FindVisibleGangArrow();
    EXPORT bool IsThereAnyMissionPhoneArrowForGang(Gang* pArgGang);
    EXPORT void ClearOrphanInfoPhoneArrows();
    EXPORT void UpdateArrows();
    EXPORT Hud_Arrow* FindFreeArrow(s32* pOutIdx);
    EXPORT Hud_Arrow* AllocArrow();
    EXPORT void ReleaseAllArrows();
    EXPORT Hud_Arrow* FindGangPhoneArrow(Gang* pZone, s32 phone_type);
    EXPORT void place_gang_phone(Object* pPhoneInfo);
    EXPORT void SetNewGangArrow(Gang* pZone);
    Hud_Arrow mArray[17]; // 0x0
    bool mShowGangArrows; // 0x83C
    Hud_Arrow* mVisibleGangArrow; // 0x840
    bool mCheckInfoPhones; // 0x844
};

EXPORT char_type __stdcall GetBriefFaceIdx(u16 face_char);

class Hud_Brief
{
  public:
    EXPORT void MovePrevBriefToCurrent();
    EXPORT void FreeCurrentBrief();
    EXPORT void MoveCurrentBriefToPrev();
    EXPORT void AppendCurrentBriefToPrev();
    EXPORT Hud_BriefEntry* AllocBrief();
    EXPORT size_t FormatCurrentBrief();
    EXPORT void StartCurrentBrief();
    EXPORT void DrawBrief();
    EXPORT void SetHudBrief_5D3F10(s32 priority, const char_type* pText, s32 cost_param);
    EXPORT void SetHudBrief_5D4400(s32 priority, const char_type* pTextIdStr);
    EXPORT void UpdateBrief();
    EXPORT void ShowBrief();
    EXPORT void ClearAllBriefsWithPriority(s32 priority);
    EXPORT Hud_Brief();

    wchar_t mStr[640]; // 0x0
    s16 mUnused500; // 0x500
    u8 mFaceIdx; // 0x502
    u16 mTickTimer; // 0x504
    s32 mNumLines; // 0x508
    s32 mFaceVariant; // 0x50C
    s32 mNumChars; // 0x510
    s32 mFaceAnimCharIdx; // 0x514
    // Pool of 20 entries linked through mpNext as the free list (mFreeBriefs is its head)
    Hud_BriefEntry mBriefs[20]; // 0x518
    // Linked lists through mpNext: the shown brief and those queued behind it by priority, the briefs pushed back
    // by a more urgent one, and the free pool entries
    Hud_BriefEntry* mCurrBriefs; // 0x6F8
    Hud_BriefEntry* mFreeBriefs; // 0x6FC
    Hud_BriefEntry* mPrevBriefs; // 0x700
};
GTA2_ASSERT_SIZEOF_ALWAYS(Hud_Brief, 0x704);

class gmp_map_zone;

class Hud_MapZone
{
  public:
    // 9.6f 0x4A4770
    void clear_zones()
    {
        mNavZone = NULL;
        mLocalNavZone = NULL;
    }

    EXPORT void DrawZoneName();
    EXPORT void CalcTextWidth();
    EXPORT void ShowZoneName(gmp_map_zone* pZone1, gmp_map_zone* pZone2);
    EXPORT void UpdateZoneName();
    EXPORT void ResetTransparency();
    EXPORT Hud_MapZone();
    u8 mTimer; // 0x0
    wchar_t mWstr[65]; // 0x2
    s32 mTextWidth; // 0x84
    gmp_map_zone* mNavZone; // 0x88
    gmp_map_zone* mLocalNavZone; // 0x8C
    s32 mAlphaFlag; // 0x90
    u8 mTransparency; // 0x94: range from 0 to 31
};
GTA2_ASSERT_SIZEOF_ALWAYS(Hud_MapZone, 0x98);

class Hud_CarName
{
  public:
    EXPORT Hud_CarName();
    EXPORT void DrawCarName();
    char_type mDisplayTime; // 0x0
    wchar_t mCarName[33]; // 0x2
    s32 mTextWidth; // 0x44
    s32 mYpos; // 0x48
};
GTA2_ASSERT_SIZEOF_ALWAYS(Hud_CarName, 0x4C);

class Hud
{
  public:
    EXPORT ~Hud();
    EXPORT void CalcCarNameTextWidth();
    EXPORT void ShowCarName(const wchar_t* pName);
    EXPORT void UpdateCarName();

    // 9.6f 0x4219F0
    void HideCarName_4219F0()
    {
        mCarName.mDisplayTime = 0;
    }

    EXPORT void DrawGui();
    EXPORT void UpdatePauseSection();
    EXPORT void UpdateHUD();
    EXPORT void SetFullAmbient();
    EXPORT void GetTextSpeed();
    EXPORT void RecalcTextLayout();
    EXPORT void SetFontTypes();
    EXPORT void Init();
    EXPORT bool HandleInputKey(s32 action, Player* pPlayer);
    EXPORT bool IsInputKeyConsumed(s32 action);
    EXPORT bool IsQuitMessageInputKey(s32 action);
    EXPORT Hud();

    Hud_CarName mCarName; // 0x0
    Hud_MapZone mZoneName; // 0x4C
    Hud_Brief mBrief; // 0xDC
    Hud_Pager_C_Array mPagers; // 0x620
    Hud_TextList mTexts; // 0x650
    Hud_Arrow_7C_Array mArrows; // 0x1F18
    Hud_CopHead_C_Array mWantedLevel; // 0x1028
    Hud_GangRespectBars mGangRespectBars; // 0x107C
    Hud_ShowCoords mShowCoords; // 0x27B5
    Hud_PickupText mPickupText; // 0x1080
    Hud_Health mHealth; // 0x1108
    Hud_UnderRoofArrowMarker mUnderRoofArrowMarker; // 0x110C
    Hud_PlayerStats mPlayerStats; // 0x1118
    Hud_Message mMessage; // 0x111C
    Hud_PauseScreen mPauseScreen; // 0x12E4
    Hud_BriefSelector mBriefSelector; // 0x12E8
    Hud_QuitMessage mQuitMessage; // 0x12EC
    Hud_ChatInput mChatInput; // 0x2A25
    Hud_MpMessage mMpMessage; // 0x12F0
    Hud_PlayerNames mPlayerNames; // 0x13C0
    u32 mTextSpeed; // 0x13C4
};
GTA2_ASSERT_SIZEOF_ALWAYS(Hud, 0x2B00);

EXTERN_GLOBAL(Hud*, gHud_2B00);

EXTERN_GLOBAL(s16, gDebugFont);

EXTERN_GLOBAL_ARRAY(char_type, gTmpGxtKey, 256);

EXPORT s32 __stdcall GetPhoneTypeFromObjModel(s32 phone_model_idx);
EXPORT char_type* __stdcall get_phone_colour(s32 phone_type);
EXPORT u8 __stdcall GetMinRespectForPhoneType(s32 phone_type);

EXPORT void __stdcall DrawAmmo(s16 ammo_idx, u8 ammo_count);
EXPORT s32 __stdcall DrawPlayerStatsHelper(s32 powerup_idx, s32 base_xpos, u16 optional_number);
