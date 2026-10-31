#include "Gang.hpp"
#include "Function.hpp"
#include "Globals.hpp"
#include "error.hpp"
#include "map_0x370.hpp"
#include "TextTable.hpp"
#include <string.h>

DEFINE_GLOBAL(GangPool*, gGangPool_CA8_67E274, 0x67E274);
DEFINE_STATIC_MEMBER_INIT(s32, GangPool, sGangIdx, -1, 0x6206B8);

EXTERN_GLOBAL(s32, bStartNetworkGame_7081F0);

MATCH_FUNC(0x4BE4E0);
Gang::Gang()
{
    Init_4BED70();
}

MATCH_FUNC(0x4BE4F0);
Gang::~Gang()
{
}

MATCH_FUNC(0x4be500)
GangPool::~GangPool()
{
}

MATCH_FUNC(0x4beca0)
Gang* GangPool::FirstGang_4BECA0()
{
    for (sGangIdx = 0; sGangIdx < GTA2_COUNTOF_S(mGangList); sGangIdx++)
    {
        if (mGangList[sGangIdx].mUsed && mGangList[sGangIdx].HasKillRespectChange_45DD50())
        {
            return &mGangList[sGangIdx];
        }
    }
    return NULL;
}

MATCH_FUNC(0x4bece0)
Gang* GangPool::NextGang_4BECE0()
{
    while (++sGangIdx < GTA2_COUNTOF_S(mGangList))
    {
        if (mGangList[sGangIdx].mUsed && mGangList[sGangIdx].HasKillRespectChange_45DD50())
        {
            return &mGangList[sGangIdx];
        }
    }
    return NULL;
}

MATCH_FUNC(0x4BED30)
wchar_t* Gang::GetNameWide_4BED30() const
{
    LOCAL_STATIC_ARRAY(wchar_t, sGangNameWide, 4, 0x67E030);
    sGangNameWide[0] = mName[0];
    sGangNameWide[1] = mName[1];
    sGangNameWide[2] = mName[2];
    sGangNameWide[3] = 0;
    return sGangNameWide;
}

MATCH_FUNC(0x4BED70);
void Gang::Init_4BED70()
{
    mGangIdx = 0;
    mUsed = false;
    mRemap = 1;
    mHasHighRespect = false;
    mKillAffectsRespect = false;
    mBasicWeapon = 0;
    mAngryWeapon = 0;
    mHateWeapon = 0;
    mGangCarModel = 1;
    mGangCarRemap = -1;
    mKillRespectChange = 0;

    for (u8 i = 0; i < 10; i++)
    {
        mHostileToGang[i] = true;
        mGangKillReaction[i] = 0;
    }

    for (u8 j = 0; j < MAX_PLAYERS; j++)
    {
        mRespect[j] = 0;
    }
    mAlwaysBasicWeapon = false;
}

MATCH_FUNC(0x4BEDF0)
bool Gang::IsHostileToGang_4BEDF0(u8 gang_idx) const
{
    return mHostileToGang[gang_idx];
}

MATCH_FUNC(0x4BEE30)
void Gang::SetRespect_4BEE30(u8 player_idx, char_type respect)
{
    mRespect[player_idx] = respect;
}

MATCH_FUNC(0x4BEE50)
void Gang::IncrementRespect_4BEE50(u8 player_idx, char_type respect)
{
    const s8 new_attr = mRespect[player_idx] + respect;
    if (new_attr < mRespect[player_idx])
    {
        mRespect[player_idx] = 100;
    }
    else
    {
        mRespect[player_idx] = new_attr;
        if (new_attr > 100)
        {
            mRespect[player_idx] = 100;
        }
    }

    if (mRespect[player_idx] >= 80)
    {
        mHasHighRespect = true;
    }
    else
    {
        mHasHighRespect = false;
    }
}

MATCH_FUNC(0x4BEEA0)
void Gang::DecrementRespect_4BEEA0(u8 player_idx, char_type respect)
{
    s8 diff = mRespect[player_idx] - respect;
    if (diff > mRespect[player_idx] || (mRespect[player_idx] = diff) < -100)
    {
        mRespect[player_idx] = -100;
    }

    if (mRespect[player_idx] >= 80)
    {
        mHasHighRespect = true;
    }
    else
    {
        mHasHighRespect = false;
    }
}

MATCH_FUNC(0x4BEEF0)
char_type Gang::GetRespectForPlayer_4BEEF0(u8 player_idx) const
{
    return mRespect[player_idx];
}

MATCH_FUNC(0x4BEF10)
bool Gang::IsRespectNegativeForPlayer_4BEF10(u8 player_idx) const
{
    if (mRespect[player_idx] < -19)
    {
        return true;
    }
    return false;
}

MATCH_FUNC(0x4BEF50)
void Gang::SetGangKillReaction_4BEF50(u8 gang_idx, char_type kill_reaction_value)
{
    mGangKillReaction[gang_idx] = kill_reaction_value;
}

// https://decomp.me/scratch/lPPny
MATCH_FUNC(0x4BEF70)
void Gang::ApplyKillRespectChange_4BEF70(u8 player_idx, u8 respect)
{
    Gang::DecrementRespect_4BEEA0(player_idx, respect * mKillRespectChange);

    for (u8 i = 0; i < 10; ++i)
    {
        Gang* pGang = gGangPool_CA8_67E274->GangByIdx_4BF1C0(i);
        if (mGangIdx != pGang->mGangIdx)
        {
            // The sum is written out twice in the checks and stored with +=, not kept in a local
            s8 change = respect * mGangKillReaction[i];
            if (static_cast<s8>(pGang->mRespect[player_idx] + change) < pGang->mRespect[player_idx] ||
                static_cast<s8>(pGang->mRespect[player_idx] + change) > 100)
            {
                pGang->mRespect[player_idx] = 100;
            }
            else
            {
                pGang->mRespect[player_idx] += change;
            }
        }
    }
}

MATCH_FUNC(0x4BF000)
void Gang::ChangeRespectAndUpdate_4BF000(u8 player_idx, char_type respect)
{
    if (respect > 0)
    {
        IncrementRespect_4BEE50(player_idx, respect);
    }
    else
    {
        DecrementRespect_4BEEA0(player_idx, abs(respect));
    }

    for (u8 i = 0; i < 10; ++i)
    {
        Gang* pGang = gGangPool_CA8_67E274->GangByIdx_4BF1C0(i);
        if (mGangIdx != pGang->mGangIdx)
        {
            if (pGang->mGangKillReaction[mGangIdx])
            {
                if (respect > 0)
                {
                    pGang->DecrementRespect_4BEEA0(player_idx, respect);
                }
                else
                {
                    pGang->IncrementRespect_4BEE50(player_idx, abs(respect));
                }
            }
        }
    }
}

MATCH_FUNC(0x4BF090);
void Gang::SetName_4BF090(const char_type* pName, u8 nameLen)
{
    strncpy(mName, pName, nameLen);
    mName[nameLen] = 0; // NULL terminate the string
}

MATCH_FUNC(0x4BF0C0);
s32 Gang::GetCurrentWeapon_4BF0C0() const
{
    if (bStartNetworkGame_7081F0 || mAlwaysBasicWeapon)
    {
        return mBasicWeapon;
    }

    if (mRespect[0] == -100)
    {
        return mHateWeapon;
    }

    if (mRespect[0] > -60)
    {
        return mBasicWeapon;
    }
    else
    {
        return mAngryWeapon;
    }
}

MATCH_FUNC(0x4BF100);
Gang* GangPool::FindGangByName_4BF100(const char_type* pGangName)
{
    for (u8 i = 0; i < 10; i++)
    {
        if (_strnicmp(pGangName, mGangList[i].mName, 4u) == 0)
        {
            return &mGangList[i];
        }
    }
    return NULL;
}

MATCH_FUNC(0x4BF170);
Gang* GangPool::AllocGang_4BF170()
{
    for (u8 i = 0; i < GTA2_COUNTOF(mGangList); i++)
    {
        if (!mGangList[i].mUsed)
        {
            Gang* result = &mGangList[i];
            result->mGangIdx = i;
            result->mUsed = true;
            return result;
        }
    }
    return NULL;
}

MATCH_FUNC(0x4BF1C0);
Gang* GangPool::GangByIdx_4BF1C0(u8 gang_idx)
{
    return &mGangList[gang_idx];
}

MATCH_FUNC(0x4BF1E0);
void GangPool::AddGangForMapZone_4BF1E0(gmp_map_zone* pMapZone)
{
    if (!FindGangByName_4BF100(pMapZone->mName))
    {
        Gang* pNewGang = AllocGang_4BF170();
        pNewGang->SetName_4BF090(pMapZone->mName, pMapZone->mNameLength);
    }
}

MATCH_FUNC(0x4BF210);
s8 GangPool::GetGangIdxByName_4BF210(const char_type* pGangName)
{
    Gang* pGang = FindGangByName_4BF100(pGangName);
    if (pGang)
    {
        return pGang->mGangIdx;
    }
    else
    {
        return -1;
    }
}

MATCH_FUNC(0x4BF230);
void GangPool::SwapGangSlots_4BF230(Gang* pGang, u8 gang_idx)
{
    // The original only calls Init_4BED70 on this local, no Gang ctor/dtor
    u8 gang_buf[sizeof(Gang)];
    Gang& tmp_gang = *reinterpret_cast<Gang*>(gang_buf);
    tmp_gang.Init_4BED70();
    if (pGang->mGangIdx != gang_idx)
    {
        Gang* pOtherGang = GangByIdx_4BF1C0(gang_idx);
        if (pOtherGang->mUsed)
        {
            if (pOtherGang->mArrowColour != pGang->mArrowColour)
            {
                memcpy(&tmp_gang, pOtherGang, sizeof(tmp_gang));
                memcpy(&gGangPool_CA8_67E274->mGangList[pOtherGang->mGangIdx],
                       pGang,
                       sizeof(gGangPool_CA8_67E274->mGangList[pOtherGang->mGangIdx]));
                memcpy(&gGangPool_CA8_67E274->mGangList[pGang->mGangIdx],
                       &tmp_gang,
                       sizeof(gGangPool_CA8_67E274->mGangList[pGang->mGangIdx]));
                pGang->mGangIdx = pOtherGang->mGangIdx;
                gGangPool_CA8_67E274->mGangList[gang_idx].mGangIdx = gang_idx;
            }
        }
    }
}

MATCH_FUNC(0x4BF2F0);
s8 GangPool::FindGangByCarModel_4BF2F0(s32 car_model)
{
    for (u8 i = 0; i < 10; i++)
    {
        if (GangByIdx_4BF1C0(i)->mGangCarModel == static_cast<u32>(car_model))
        {
            return i;
        }
    }
    return -1;
}

MATCH_FUNC(0x4BF340)
wchar_t* Gang::GetArrowColourText_4BF340() const
{
    sprintf(gTmpBuffer_67C598, "ganga%d", mArrowColour);
    return gText_0x14_704DFC->Find_5B5F90(gTmpBuffer_67C598);
}