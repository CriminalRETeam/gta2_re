#include "PlyDat.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "Globals.hpp"
#include "Player.hpp"
#include "error.hpp"
#include "file.hpp"
#include "GameSession.hpp"
#include <io.h>

DEFINE_GLOBAL(PlyDat*, gPlyDat, 0x6FEAC0);
EXTERN_GLOBAL_ARRAY(wchar_t, gEmptyWStr, 2);
DEFINE_STATIC_GLOBAL_ARRAY_INIT(ScoreTableLine, sDefaultHiScores, 10, 0x6242B0,
    { L"ALISDAIR" COMMA 50000 } COMMA
    { L"BILLY"    COMMA 40000 } COMMA
    { L"BRIAN"    COMMA 30000 } COMMA
    { L"COLIN"    COMMA 20000 } COMMA
    { L"IAIN"     COMMA 10000 } COMMA
    { L"IAN"      COMMA  9000 } COMMA
    { L"KEITH"    COMMA  8000 } COMMA
    { L"MARTIN"   COMMA  7000 } COMMA
    { L"STEPHEN"  COMMA  6000 } COMMA
    { L"WILLIAM"  COMMA  5000 } COMMA
);
DEFINE_GLOBAL_ARRAY_INIT(ScoreTableLine, gDefaultStageHiScores, 120, 0x6243A0,   //, , 3][4][10, 0xUNKNOWN);
    {L"ALISDAIR" COMMA 50000 } COMMA
    {L"BILLY"    COMMA 40000 } COMMA
    {L"BRIAN"    COMMA 30000 } COMMA
    {L"COLIN"    COMMA 20000 } COMMA
    {L"IAIN"     COMMA 10000 } COMMA
    {L"IAN"      COMMA  9000 } COMMA
    {L"KEITH"    COMMA  8000 } COMMA
    {L"MARTIN"   COMMA  7000 } COMMA
    {L"STEPHEN"  COMMA  6000 } COMMA
    {L"WILLIAM"  COMMA  5000 } COMMA
    {L"ALISDAIR" COMMA 50000 } COMMA
    {L"BILLY"    COMMA 40000 } COMMA
    {L"BRIAN"    COMMA 30000 } COMMA
    {L"COLIN"    COMMA 20000 } COMMA
    {L"IAIN"     COMMA 10000 } COMMA
    {L"IAN"      COMMA  9000 } COMMA
    {L"KEITH"    COMMA  8000 } COMMA
    {L"MARTIN"   COMMA  7000 } COMMA
    {L"STEPHEN"  COMMA  6000 } COMMA
    {L"WILLIAM"  COMMA  5000 } COMMA
    {L"ALISDAIR" COMMA 50000 } COMMA
    {L"BILLY"    COMMA 40000 } COMMA
    {L"BRIAN"    COMMA 30000 } COMMA
    {L"COLIN"    COMMA 20000 } COMMA
    {L"IAIN"     COMMA 10000 } COMMA
    {L"IAN"      COMMA  9000 } COMMA
    {L"KEITH"    COMMA  8000 } COMMA
    {L"MARTIN"   COMMA  7000 } COMMA
    {L"STEPHEN"  COMMA  6000 } COMMA
    {L"WILLIAM"  COMMA  5000 } COMMA
    {L"ALISDAIR" COMMA 50000 } COMMA
    {L"BILLY"    COMMA 40000 } COMMA
    {L"BRIAN"    COMMA 30000 } COMMA
    {L"COLIN"    COMMA 20000 } COMMA
    {L"IAIN"     COMMA 10000 } COMMA
    {L"IAN"      COMMA  9000 } COMMA
    {L"KEITH"    COMMA  8000 } COMMA
    {L"MARTIN"   COMMA  7000 } COMMA
    {L"STEPHEN"  COMMA  6000 } COMMA
    {L"WILLIAM"  COMMA  5000 } COMMA
    {L"ALISDAIR" COMMA 50000 } COMMA
    {L"BILLY"    COMMA 40000 } COMMA
    {L"BRIAN"    COMMA 30000 } COMMA
    {L"COLIN"    COMMA 20000 } COMMA
    {L"IAIN"     COMMA 10000 } COMMA
    {L"IAN"      COMMA  9000 } COMMA
    {L"KEITH"    COMMA  8000 } COMMA
    {L"MARTIN"   COMMA  7000 } COMMA
    {L"STEPHEN"  COMMA  6000 } COMMA
    {L"WILLIAM"  COMMA  5000 } COMMA
    {L"ALISDAIR" COMMA 50000 } COMMA
    {L"BILLY"    COMMA 40000 } COMMA
    {L"BRIAN"    COMMA 30000 } COMMA
    {L"COLIN"    COMMA 20000 } COMMA
    {L"IAIN"     COMMA 10000 } COMMA
    {L"IAN"      COMMA  9000 } COMMA
    {L"KEITH"    COMMA  8000 } COMMA
    {L"MARTIN"   COMMA  7000 } COMMA
    {L"STEPHEN"  COMMA  6000 } COMMA
    {L"WILLIAM"  COMMA  5000 } COMMA
    {L"ALISDAIR" COMMA 50000 } COMMA
    {L"BILLY"    COMMA 40000 } COMMA
    {L"BRIAN"    COMMA 30000 } COMMA
    {L"COLIN"    COMMA 20000 } COMMA
    {L"IAIN"     COMMA 10000 } COMMA
    {L"IAN"      COMMA  9000 } COMMA
    {L"KEITH"    COMMA  8000 } COMMA
    {L"MARTIN"   COMMA  7000 } COMMA
    {L"STEPHEN"  COMMA  6000 } COMMA
    {L"WILLIAM"  COMMA  5000 } COMMA
    {L"ALISDAIR" COMMA 50000 } COMMA
    {L"BILLY"    COMMA 40000 } COMMA
    {L"BRIAN"    COMMA 30000 } COMMA
    {L"COLIN"    COMMA 20000 } COMMA
    {L"IAIN"     COMMA 10000 } COMMA
    {L"IAN"      COMMA  9000 } COMMA
    {L"KEITH"    COMMA  8000 } COMMA
    {L"MARTIN"   COMMA  7000 } COMMA
    {L"STEPHEN"  COMMA  6000 } COMMA
    {L"WILLIAM"  COMMA  5000 } COMMA
    {L"ALISDAIR" COMMA 50000 } COMMA
    {L"BILLY"    COMMA 40000 } COMMA
    {L"BRIAN"    COMMA 30000 } COMMA
    {L"COLIN"    COMMA 20000 } COMMA
    {L"IAIN"     COMMA 10000 } COMMA
    {L"IAN"      COMMA  9000 } COMMA
    {L"KEITH"    COMMA  8000 } COMMA
    {L"MARTIN"   COMMA  7000 } COMMA
    {L"STEPHEN"  COMMA  6000 } COMMA
    {L"WILLIAM"  COMMA  5000 } COMMA
    {L"ALISDAIR" COMMA 50000 } COMMA
    {L"BILLY"    COMMA 40000 } COMMA
    {L"BRIAN"    COMMA 30000 } COMMA
    {L"COLIN"    COMMA 20000 } COMMA
    {L"IAIN"     COMMA 10000 } COMMA
    {L"IAN"      COMMA  9000 } COMMA
    {L"KEITH"    COMMA  8000 } COMMA
    {L"MARTIN"   COMMA  7000 } COMMA
    {L"STEPHEN"  COMMA  6000 } COMMA
    {L"WILLIAM"  COMMA  5000 } COMMA
    {L"ALISDAIR" COMMA 50000 } COMMA
    {L"BILLY"    COMMA 40000 } COMMA
    {L"BRIAN"    COMMA 30000 } COMMA
    {L"COLIN"    COMMA 20000 } COMMA
    {L"IAIN"     COMMA 10000 } COMMA
    {L"IAN"      COMMA  9000 } COMMA
    {L"KEITH"    COMMA  8000 } COMMA
    {L"MARTIN"   COMMA  7000 } COMMA
    {L"STEPHEN"  COMMA  6000 } COMMA
    {L"WILLIAM"  COMMA  5000 } COMMA
    {L"ALISDAIR" COMMA 50000 } COMMA
    {L"BILLY"    COMMA 40000 } COMMA
    {L"BRIAN"    COMMA 30000 } COMMA
    {L"COLIN"    COMMA 20000 } COMMA
    {L"IAIN"     COMMA 10000 } COMMA
    {L"IAN"      COMMA  9000 } COMMA
    {L"KEITH"    COMMA  8000 } COMMA
    {L"MARTIN"   COMMA  7000 } COMMA
    {L"STEPHEN"  COMMA  6000 } COMMA
    {L"WILLIAM"  COMMA  5000 } COMMA
);

EXTERN_GLOBAL(s32, bStartNetworkGame);


MATCH_FUNC(0x56B500)
HighScoreTable::HighScoreTable()
{
    Init();
}

MATCH_FUNC(0x56B510)
HighScoreTable::~HighScoreTable()
{
}

MATCH_FUNC(0x56B520)
void HighScoreTable::Init()
{
    for (s32 i = 0; i < 10; i++)
    {
        wcscpy(mScoreTableLine[i].mPlayerName, gEmptyWStr);
        mScoreTableLine[i].mScore = 0;
    }
}

MATCH_FUNC(0x56B550)
bool HighScoreTable::InsertScore(const wchar_t* pFindStr, s32 findScore)
{
    u16 startIdx = 10;
    for (s16 i = 9; i != -1; --i)
    {
        if (static_cast<u32>(findScore) > mScoreTableLine[i].mScore)
        {
            startIdx = i;
        }

        if (static_cast<u32>(findScore) == mScoreTableLine[i].mScore &&
            wcscmp(pFindStr, mScoreTableLine[i].mPlayerName) == 0)
        {
            return false;
        }
    }

    if (startIdx != 10)
    {
        if (startIdx < 9u)
        {
            for (u16 k = 9; k > startIdx; --k)
            {
                wcsncpy(mScoreTableLine[k].mPlayerName, mScoreTableLine[k - 1].mPlayerName, 9u);
                mScoreTableLine[k].mScore = mScoreTableLine[k - 1].mScore;
            }
        }

        wcsncpy(mScoreTableLine[startIdx].mPlayerName, pFindStr, 9u);
        mScoreTableLine[startIdx].mScore = findScore;

        return true;
    }
    return false;
}

MATCH_FUNC(0x56B610)
PlySlot::PlySlot()
{
    ResetPlayerSlot();
}

MATCH_FUNC(0x56B620)
PlySlot::~PlySlot()
{
}

MATCH_FUNC(0x56B630)
void PlySlot::ResetPlayerSlot()
{
    for (u16 k = 0; k < 9; k++)
    {
        mstrPlayerName[k] = 0;
    }

    for (u32 i = 0; i < 3; i++)
    {
        for (u32 j = 0; j < 4; j++)
        {
            mPlyrStageStats[i][j].mIsStageUnlocked = false;
            mPlyrStageStats[i][j].mStageBestScore = 0;
            mPlyrStageStats[i][j].mStageLatestScore = 0;
        }
    }
    mPlyrStageStats[0][0].mIsStageUnlocked = true;
}

MATCH_FUNC(0x56B680)
s32 PlySlot::GetTotalLatestScore() const
{
    s32 result = 0;

    for (u32 i = 0; i < 3; i++)
    {
        for (u32 j = 0; j < 4; j++)
        {
            result += mPlyrStageStats[i][j].mStageLatestScore;
        }
    }
    return result;
}

MATCH_FUNC(0x56B6B0)
s32 PlySlot::GetTotalBestScore() const
{
    s32 result = 0;

    for (u32 i = 0; i < 3; i++)
    {
        for (u32 j = 0; j < 4; j++)
        {
            result += mPlyrStageStats[i][j].mStageBestScore;
        }
    }
    return result;
}

MATCH_FUNC(0x56B6E0)
PlyDat::PlyDat()
{
    for (s32 i = 0; i < 3; i++)
    {
        memset(&mBestStats[i], 0, sizeof(BestStageStats));
        mBestCarDamageCost[i] = 0;
        mBestEvasionRating[i] = 0;
    }

    for (u32 j = 0; static_cast<u16>(j) < 8; j++)
    {
        if (PlySlotDatExists(j))
        {
            LoadPlySlotDat(j);
        }
        else
        {
            SavePlySlotDat(j);
        }
    }

    if (HiScoreHscExists())
    {
        LoadHiScores();
    }
    else
    {
        InitDefaultHiScores();
        SaveHiScores();
    }

    InitAltHiScores();
}

// Defined after ~HighScoreTable so VC6 knows that destructor can't throw and
// drops the EH state updates between the calls to it.
MATCH_FUNC(0x56B810)
PlyDat::~PlyDat()
{
}

MATCH_FUNC(0x56B8A0)
void PlyDat::GetPlySlotDatName(u16 slot_idx, char_type* pName)
{
    char_type Buffer[8];
    _itoa(slot_idx, Buffer, 10);
    strcpy(pName, "player\\plyslot");
    strcat(pName, Buffer);
    strcat(pName, ".dat");
}

MATCH_FUNC(0x56B940)
bool PlyDat::PlySlotDatExists(s32 slot_idx)
{
    char_type FileName[356];
    GetPlySlotDatName(slot_idx, FileName);

    _finddata_t findData;
    long hFind = _findfirst(FileName, &findData);
    if (hFind == -1)
    {
        return false;
    }
    _findclose(hFind);
    return true;
}

MATCH_FUNC(0x56B990)
void PlyDat::LoadPlySlotDat(u16 slotIdx)
{
    char_type FileName[356];

    PlySlot* pSlot = &mPlyrStats[slotIdx];

    GetPlySlotDatName(slotIdx, FileName);
    File::Global_Open(FileName);

    for (s32 i = 0; i < 9; i++)
    {
        u32 len_read = 2;
        File::Global_Read(&pSlot->mstrPlayerName[i], len_read);
    }

    for (s32 k = 0; k < 3; k++)
    {
        for (s32 j = 0; j < 4; j++)
        {
            u32 len_read = 1;
            File::Global_Read(&mPlyrStats[slotIdx].mPlyrStageStats[k][j].mIsStageUnlocked, len_read);

            len_read = 4;
            File::Global_Read(&mPlyrStats[slotIdx].mPlyrStageStats[k][j].mStageBestScore, len_read);

            len_read = 4;
            File::Global_Read(&mPlyrStats[slotIdx].mPlyrStageStats[k][j].mStageLatestScore, len_read);
        }
    }
    File::Global_Close();
}

MATCH_FUNC(0x56BA60)
void PlyDat::SavePlySlotDat(s16 slotIdx)
{
    char_type FileName[356];
    size_t len;

    GetPlySlotDatName(slotIdx, FileName);

    // The start of this object is reused as the buffer for the player slot .dat file
    memcpy(this,
           mPlyrStats[static_cast<u16>(slotIdx)].mstrPlayerName,
           sizeof(mPlyrStats[0].mstrPlayerName));
    len = sizeof(mPlyrStats[0].mstrPlayerName);

    // len is accumulated like in SaveHiScores: VC6 folds its final value (126) into one store
    // that it emits after the loop counter init, which a plain "len = 126" never does.
    u8* pDst = reinterpret_cast<u8*>(this) + sizeof(mPlyrStats[0].mstrPlayerName);
    for (s32 k = 0; k < 3; k++)
    {
        for (s32 j = 0; j < 4; j++)
        {
            *pDst = mPlyrStats[static_cast<u16>(slotIdx)].mPlyrStageStats[k][j].mIsStageUnlocked;
            pDst++;
            len++;
            *reinterpret_cast<u32*>(pDst) = mPlyrStats[static_cast<u16>(slotIdx)].mPlyrStageStats[k][j].mStageBestScore;
            pDst += 4;
            len += 4;
            *reinterpret_cast<s32*>(pDst) = mPlyrStats[static_cast<u16>(slotIdx)].mPlyrStageStats[k][j].mStageLatestScore;
            pDst += 4;
            len += 4;
        }
    }

    File::WriteBufferToFile(FileName, this, &len);
}

// https://decomp.me/scratch/oIJET
MATCH_FUNC(0x56BB10)
void PlyDat::UpdateStageScore(Player* pPlayer)
{
    const u8 slot_idx = gGameSession.GetPlySlotIdx();
    PlySlot* pPlayerStats = &mPlyrStats[slot_idx];
    u8 map_num;
    u8 bonus_num;
    if (!gGameSession.IsBonusStage())
    {
        map_num = gGameSession.GetMainStageIdx();
        bonus_num = 0;
    }
    else
    {
        gGameSession.DecodeStage(gGameSession.GetStage(), &map_num, &bonus_num);
    }

    StageStats* pStageStats = &pPlayerStats->mPlyrStageStats[map_num][bonus_num];
    const u32 latest_score = pPlayer->mScores.GetScore();
    if (latest_score > pStageStats->mStageBestScore)
    {
        pStageStats->mStageBestScore = latest_score;
    }
    pStageStats->mStageLatestScore = latest_score;
    SavePlySlotDat(slot_idx);
}

MATCH_FUNC(0x56BBD0)
void PlyDat::UnlockStage(u8 map_num, u8 bonus_num)
{
    const u8 slot_idx = gGameSession.GetPlySlotIdx();
    this->mPlyrStats[slot_idx].mPlyrStageStats[map_num][bonus_num].mIsStageUnlocked = 1;
    if (!bStartNetworkGame)
    {
        SavePlySlotDat(slot_idx);
    }
}

MATCH_FUNC(0x56BC40)
void PlyDat::UnlockAllStages()
{
    const u8 slot_idx = gGameSession.GetPlySlotIdx();
    PlySlot* pStats = &this->mPlyrStats[slot_idx];
    for (s32 k3 = 0; k3 < 3; k3++)
    {
        for (s32 k4 = 0; k4 < 4; k4++)
        {
            pStats->mPlyrStageStats[k3][k4].mIsStageUnlocked = 1;
        }
    }

    if (!bStartNetworkGame)
    {
        SavePlySlotDat(slot_idx);
    }
}

// =====================================================================

MATCH_FUNC(0x56BCA0)
bool PlyDat::HiScoreHscExists()
{
    char_type FileName[356];
    GetHiScoreHscFileName(FileName);

    _finddata_t findData;
    long hFind = _findfirst(FileName, &findData);
    if (hFind == -1)
    {
        return false;
    }
    _findclose(hFind);
    return true;
}

MATCH_FUNC(0x56BCF0)
void PlyDat::GetHiScoreHscFileName(char_type* pName)
{
    strcpy(pName, "player\\hiscores.hsc");
}

MATCH_FUNC(0x56BD20)
void PlyDat::InitAltHiScores()
{
    wcsncpy(mAltScores.mScoreTableLine[0].mPlayerName, L"ALAN", 9u);
    mAltScores.mScoreTableLine[0].mScore = 1000000;
    wcsncpy(mAltScores.mScoreTableLine[1].mPlayerName, L"BRIAN", 9u);
    mAltScores.mScoreTableLine[1].mScore = 500000;
    wcsncpy(mAltScores.mScoreTableLine[2].mPlayerName, L"COLIN", 9u);
    mAltScores.mScoreTableLine[2].mScore = 400000;
    wcsncpy(mAltScores.mScoreTableLine[3].mPlayerName, L"DAVE", 9u);
    mAltScores.mScoreTableLine[3].mScore = 300000;
    wcsncpy(mAltScores.mScoreTableLine[4].mPlayerName, L"ERIC", 9u);
    mAltScores.mScoreTableLine[4].mScore = 250000;
    wcsncpy(mAltScores.mScoreTableLine[5].mPlayerName, L"FRANK", 9u);
    mAltScores.mScoreTableLine[5].mScore = 200000;
    wcsncpy(mAltScores.mScoreTableLine[6].mPlayerName, L"GRAEME", 9u);
    mAltScores.mScoreTableLine[6].mScore = 100000;
    wcsncpy(mAltScores.mScoreTableLine[7].mPlayerName, L"HECTOR", 9u);
    mAltScores.mScoreTableLine[7].mScore = 50000;
    wcsncpy(mAltScores.mScoreTableLine[8].mPlayerName, L"IMOGEN", 9u);
    mAltScores.mScoreTableLine[8].mScore = 25000;
    wcsncpy(mAltScores.mScoreTableLine[9].mPlayerName, L"JACKSON", 9u);
    mAltScores.mScoreTableLine[9].mScore = 10000;
}

MATCH_FUNC(0x56BE50)
void PlyDat::LoadHiScores()
{
    char_type FileName[256];
    GetHiScoreHscFileName(FileName);
    File::Global_Open(FileName);

    File::Global_Read(&mTotalScores.mScoreTableLine, 240);

    for (s32 k3Counter = 0; k3Counter < 3; k3Counter++)
    {
        for (s32 k4Counter = 0; k4Counter < 4; k4Counter++)
        {
            File::Global_Read(&mStageScores[k3Counter][k4Counter], 240);
        }

        File::Global_Read(&mBestStats[k3Counter], 40); // 3 40 byte objs

        File::Global_Read(&mBestCarDamageCost[k3Counter], 4);

        File::Global_Read(&mBestEvasionRating[k3Counter], 4);
    }

    File::Global_Close();
}

MATCH_FUNC(0x56BF20)
void PlyDat::SaveHiScores()
{
    char_type FileName[256];
    size_t len;

    GetHiScoreHscFileName(FileName);

    // The start of this object is reused as the buffer for the high score file
    u8* pDst = reinterpret_cast<u8*>(this);
    memcpy(pDst, &mTotalScores, sizeof(HighScoreTable));
    pDst += sizeof(HighScoreTable);
    len = sizeof(HighScoreTable);

    for (s32 k = 0; k < 3; k++)
    {
        for (s32 j = 0; j < 4; j++)
        {
            memcpy(pDst, &mStageScores[k][j], sizeof(HighScoreTable));
            pDst += sizeof(HighScoreTable);
            len += sizeof(HighScoreTable);
        }

        memcpy(pDst, &mBestStats[k], sizeof(BestStageStats));
        pDst += sizeof(BestStageStats);
        len += sizeof(BestStageStats);

        *reinterpret_cast<s32*>(pDst) = mBestCarDamageCost[k];
        pDst += 4;
        len += 4;

        *reinterpret_cast<s32*>(pDst) = mBestEvasionRating[k];
        pDst += 4;
        len += 4;
    }

    File::WriteBufferToFile(FileName, this, &len);
}

MATCH_FUNC(0x56C010)
void PlyDat::UpdateHiScores()
{
    u8 map_num;
    u8 bonus_num;
    bool bBestStatsChanged = false;

    if (!gGameSession.IsBonusStage())
    {
        map_num = gGameSession.GetMainStageIdx();
        bonus_num = 0;
    }
    else
    {
        gGameSession.DecodeStage(gGameSession.GetStage(), &map_num, &bonus_num);
    }

    PlySlot* pPlayerStats = &mPlyrStats[gGameSession.GetPlySlotIdx()];
    s32 latest = pPlayerStats->mPlyrStageStats[map_num][bonus_num].mStageLatestScore;
    const bool bNewStageScore =
        mStageScores[map_num][bonus_num].InsertScore(pPlayerStats->mstrPlayerName, latest);
    const bool bNewTotalScore = mTotalScores.InsertScore(pPlayerStats->mstrPlayerName, pPlayerStats->GetTotalBestScore());

    if (!bonus_num)
    {
        u32* pBestStats = reinterpret_cast<u32*>(&mBestStats[map_num]);
        for (u8 i = 0; i < 10; i++, pBestStats++)
        {
            const u32 value = gGameSession.GetStatistic(i);
            if (value > *pBestStats)
            {
                *pBestStats = value;
                bBestStatsChanged = true;
            }
        }

        const u32 value1 = gGameSession.GetCarDamageCost();
        if (value1 > static_cast<u32>(mBestCarDamageCost[map_num]))
        {
            mBestCarDamageCost[map_num] = value1;
            bBestStatsChanged = true;
        }

        const u32 value2 = gGameSession.GetEvasionRating();
        if (value2 > static_cast<u32>(mBestEvasionRating[map_num]))
        {
            mBestEvasionRating[map_num] = value2;
            bBestStatsChanged = true;
        }
    }

    if (bNewStageScore || bNewTotalScore || bBestStatsChanged)
    {
        SaveHiScores();
    }
}

MATCH_FUNC(0x56C170)
void PlyDat::BuildMergedHiScores()
{
    mMergedScores.Init();
    ScoreTableLine* pLine = mAltScores.mScoreTableLine;
    for (s32 k10 = 0; k10 < 10; k10++)
    {
        mMergedScores.InsertScore(pLine->mPlayerName, pLine->mScore);
        ++pLine;
    }

    for (s32 k8 = 0; k8 < 8; k8++)
    {
        PlySlot* pSlot = &mPlyrStats[k8];
        mMergedScores.InsertScore(pSlot->mstrPlayerName, pSlot->GetTotalLatestScore());
    }
}

MATCH_FUNC(0x56C1D0)
void PlyDat::InitDefaultHiScores()
{
    mTotalScores.Init();
    ScoreTableLine* p10StruIter = sDefaultHiScores;
    for (s32 k10 = 0; k10 < 10; k10++)
    {
        mTotalScores.InsertScore(p10StruIter->mPlayerName, p10StruIter->mScore);
        ++p10StruIter;
    }

    ScoreTableLine* pSruIter = gDefaultStageHiScores;
    for (s32 k3 = 0; k3 < 3; k3++)
    {
        for (s32 k4 = 0; k4 < 4; k4++)
        {
            mStageScores[k3][k4].Init();
            for (s32 k10 = 0; k10 < 10; k10++)
            {
                mStageScores[k3][k4].InsertScore(pSruIter->mPlayerName, pSruIter->mScore);
                ++pSruIter;
            }
        }
    }
}

MATCH_FUNC(0x56C250)
void PlyDat::DoMuchCashCheat()
{
    if (!bStartNetworkGame)
    {
        PlySlot* pStats = &this->mPlyrStats[gGameSession.GetPlySlotIdx()];
        if (wcscmp(pStats->mstrPlayerName, L"MUCHCASH") == 0)
        {
            Player* pPlayer = gGame_0x40->mLocalPlayer;
            pPlayer->mScores.AddCash(pPlayer->mMultipliers.mValue * 500000);
        }
    }
}

MATCH_FUNC(0x56C2C0)
void PlyDat::Create()
{
    if (!gPlyDat)
    {
        gPlyDat = new PlyDat();
        if (!gPlyDat)
        {
            FatalError(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\plydat.cpp", 1269);
        }
    }
}

MATCH_FUNC(0x56C340)
void PlyDat::Destroy()
{
    if (gPlyDat)
    {
        delete gPlyDat;
        gPlyDat = NULL;
    }
}