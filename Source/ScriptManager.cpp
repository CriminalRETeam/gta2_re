#include "ScriptManager.hpp"
#include "car_thread_trigger.hpp"
#include "Car.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "Generators.hpp"
#include "Globals.hpp"
#include "MissionCleanupList.hpp"
#include "Object_5C.hpp"
#include "Player.hpp"
#include "debug.hpp"
#include "enums.hpp"
#include "error.hpp"
#include "file.hpp"
#include "GameSession.hpp"
#include "map_0x370.hpp"
#include "memory.hpp"
#include "ScriptThread.hpp"
#include "Ped.hpp"
#include "PedGroup.hpp"
#include "ped_death_cause.hpp"
#include "PublicTransport.hpp"

DEFINE_GLOBAL(ScriptManager*, gScriptManager_6F8060, 0x6F8060);
DEFINE_GLOBAL(SaveData, gGameSave_6F78C8, 0x6F78C8);
DEFINE_GLOBAL(s32, gStoredCarId_6F78B4, 0x6F78B4);

EXTERN_GLOBAL(SCR_POINTER*, gBasePtr_6F8070);

// Internal linkage as in the original, where all users were in one file: VC6 allocates registers differently for it.
DEFINE_STATIC_GLOBAL(Car*, gStoredCar_6F7560, 0x6F7560);

MATCH_FUNC(0x502dc0)
void MissionCleanupList::MissionCleanUp_502DC0()
{
    MissionCleanupEntry* pIter = &mEntries[0];
    PedGroup* pGroup = gGame_0x40_67E008->mLocalPlayer->mPlayerPed->mPedGroup;
    if (pGroup)
    {
        pGroup->DestroyGroup_4C93A0();
    }

    if (gStoredCar_6F7560)
    {
        if (gStoredCar_6F7560->mCarId == gStoredCarId_6F78B4)
        {
            gGame_0x40_67E008->mLocalPlayer->RemoveCarFromHistory_564680(gStoredCar_6F7560);
            gStoredCar_6F7560->SetDriverPedType_421560(ped_type::dummy_3);
            if (!gStoredCar_6F7560->IsDeactivated_4214D0())
            {
                gStoredCar_6F7560->MarkForDespawn_421470();
            }
            gStoredCar_6F7560 = NULL;
            gStoredCarId_6F78B4 = 0;
        }
    }

    for (s32 i = 0; i < GTA2_COUNTOF(mEntries); i++)
    {
        if (reinterpret_cast<u32*>(pIter->mCar) != NULL)
        {
            switch (pIter->mType)
            {
                case cleanup_entry_type::object_2:
                    if (pIter->mObj->mId == pIter->mEntityId)
                    {
                        pIter->mObj->Dealloc_5291B0();
                        pIter->mObj->RequestRemovalWhenOffScreen_5290B0();
                    }
                    break;
                case cleanup_entry_type::ped_3:
                    if (pIter->mPed->mId == static_cast<u32>(pIter->mEntityId))
                    {
                        PedGroup* pPedGroup = pIter->mPed->mPedGroup;
                        if (pPedGroup)
                        {
                            pPedGroup->DisbandGroup_4C92A0();
                        }
                        pIter->mPed->ForceDoNothing_462590();
                        pIter->mPed->mBf21C.bScheduledForRemoval = true;
                    }
                    break;
                case cleanup_entry_type::car_1:
                    if (pIter->mCar->mCarId == pIter->mEntityId)
                    {
                        gGame_0x40_67E008->mLocalPlayer->RemoveCarFromHistory_564680(pIter->mCar);
                        pIter->mCar->SetDriverPedType_421560(ped_type::dummy_3);
                        if (!pIter->mCar->IsDeactivated_4214D0())
                        {
                            pIter->mCar->MarkForDespawn_421470();
                        }
                    }
                    break;
                default:
                    break;
            }
            pIter->mCar = NULL;
            pIter->mEntityId = 0;
            pIter->mType = cleanup_entry_type::empty_0;
            --mCount;
        }
        pIter++;
    }
    gScriptManager_6F8060->mWantedCarModel = car_model_enum::none;
}


MATCH_FUNC(0x503080)
str_table_entry* ScriptManager::FindStringById_503080(s16 stringId) const
{
    u16 idx = 0;
    str_table_entry* result = mpStringTbl->mEntries[idx];
    while (result)
    {
        if (result->mStrId == stringId)
        {
            return result;
        }
        idx++;
        result = mpStringTbl->mEntries[idx];
    }
    return NULL;
}

MATCH_FUNC(0x5030b0)
str_table_entry* ScriptManager::StrEntryByString_5030B0(const char_type* strToFind) const
{
    u16 idx = 0;
    str_table_entry* pEntry = mpStringTbl->mEntries[idx];
    while (pEntry)
    {
        if (_strnicmp(strToFind, pEntry->get_name(), pEntry->mLength) == 0)
        {
            return pEntry;
        }
        idx++;
        pEntry = mpStringTbl->mEntries[idx];
    }
    return NULL;
}

EXTERN_GLOBAL_ARRAY(u8, kDecidePowerupGenTypes_6212F0, 19);

// Sets the generator type for `car_model` from the script's car list, or type 0x41 when it isn't
// in the list. Reads the global rather than `this`.
MATCH_FUNC(0x509180)
void ScriptThread::SCRCMD_STORE_CAR_INFO_509180()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr_6F8070);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->mIndex));
    SCR_POINTER* pParam2 = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->mUnsigned2));
    Ped* pChar = pPointer->mChar;

    if (pChar != NULL)
    {
        if (pChar->has_car_403B80())
        {
            pParam2->mCar = pChar->get_car_416B60();

            if (gStoredCar_6F7560 != NULL)
            {
                if (gStoredCar_6F7560 != pParam2->mCar && gStoredCar_6F7560->mCarId != pParam2->mCar->mCarId)
                {
                    if (gStoredCar_6F7560->mCarId == gStoredCarId_6F78B4)
                    {
                        if (gCarManager_6C_677930->CanAllocateCarOfKind_446930(car_kind::recycled_1))
                        {
                            gStoredCar_6F7560->ReassignAllocatedCarType_443EE0(car_kind::recycled_1);
                            gGame_0x40_67E008->mLocalPlayer->RemoveCarFromHistory_564680(gStoredCar_6F7560);
                            gStoredCar_6F7560->SetDriverPedType_421560(ped_type::dummy_3);

                            if (!gStoredCar_6F7560->IsDeactivated_4214D0())
                            {
                                gStoredCar_6F7560->MarkForDespawn_421470();
                            }
                        }
                        else if (gCarManager_6C_677930->CanAllocateCarOfKindWithSpare_446870(car_kind::mission_car_8))
                        {
                            gGame_0x40_67E008->mLocalPlayer->RemoveCarFromHistory_564680(gStoredCar_6F7560);
                            gStoredCar_6F7560->SetDriverPedType_421560(ped_type::dummy_3);

                            if (!gStoredCar_6F7560->IsDeactivated_4214D0())
                            {
                                gStoredCar_6F7560->MarkForDespawn_421470();
                            }
                        }
                        else
                        {
                            gGame_0x40_67E008->mLocalPlayer->RemoveCarFromHistory_564680(gStoredCar_6F7560);
                            gStoredCar_6F7560->SetDriverPedType_421560(ped_type::dummy_3);

                            gStoredCar_6F7560->DespawnSoon_421490();
                        }
                    }

                    gStoredCar_6F7560 = pParam2->mCar;
                    gStoredCarId_6F78B4 = pParam2->mCar->mCarId;

                    gStoredCar_6F7560->ReassignAllocatedCarType_443EE0(car_kind::mission_car_8);

                    if (!gPublicTransport_181C_6FF1D4->is_bus_579AA0(pParam2->mCar))
                    {
                        pParam2->mCar->LockoutThiefUnlessPermanent_475C40();
                    }
                }
            }
            else
            {
                gStoredCar_6F7560 = pParam2->mCar;
                gStoredCarId_6F78B4 = pParam2->mCar->mCarId;
                gStoredCar_6F7560->ReassignAllocatedCarType_443EE0(car_kind::mission_car_8);
            }
        }
    }

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x511A70)
void ScriptManager::SetGeneratorTypeForCar_511A70(s32 car_model, Generator* pGen)
{
    u8* pList = reinterpret_cast<u8*>(gScriptManager_6F8060->mCarList);
    u8 i;
    for (i = 0; i < 19; pList++, i++)
    {
        if (car_model == *pList)
        {
            pGen->mGenType = kDecidePowerupGenTypes_6212F0[i];
            if (kDecidePowerupGenTypes_6212F0[i] < 0x5B)
            {
                pGen->Activate_4C1A70();
                pGen->mSpawnsLeft = 3;
            }
            else
            {
                pGen->Activate_4C1A70();
                pGen->mSpawnsLeft = 1;
            }
            break;
        }
    }

    if (i == 19)
    {
        pGen->Activate_4C1A70();
        pGen->mSpawnsLeft = 3;
        pGen->mGenType = 0x41;
    }
}

MATCH_FUNC(0x511b10)
char_type ScriptManager::AddCranePowerupTrigger_511B10(s16 idx)
{
    SCR_DECLARE_CRANE_POWERUP* pCmd = static_cast<SCR_DECLARE_CRANE_POWERUP*>(GetBasePointer_512770(idx));
    SCR_POINTER* pGenerator = static_cast<SCR_POINTER*>(GetBasePointer_512770(pCmd->mGenerator));
    return gCarThreadTriggers_214_705F20->AddThreadTrigger_5C86C0(car_thread_trigger_type::any_car_1,
                                       car_thread_trigger_action::set_generator_car_type_3,
                                       reinterpret_cast<SCR_THREAD*>(pGenerator->mGenerator),
                                       Fix16(pCmd->mPos.mX) + kFpHalf_6F75F0,
                                       Fix16(pCmd->mPos.mY) + kFpHalf_6F75F0,
                                       Fix16(pCmd->mPos.mZ),
                                       kFpHalf_6F75F0,
                                       kFpHalf_6F75F0);
}

MATCH_FUNC(0x511b90)
void ScriptManager::SaveScriptCounters_511B90()
{
    u16 saved_counter_count = 0;
    for (u16 ptr = 0; ptr < GTA2_COUNTOF(mBasePointers); ptr++)
    {
        if (mBasePointers[ptr])
        {
            SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(GetBasePointer_512770(ptr));
            if (pPointer->mType == SCRCMD_COUNTER_SAVE || pPointer->mType == SCRCMD_COUNTER_SET_SAVE)
            {
                gGameSave_6F78C8.mCarAndScriptData.mScriptCounter[saved_counter_count].mPointer =
                    pPointer->mCmdThis;
                gGameSave_6F78C8.mCarAndScriptData.mScriptCounter[saved_counter_count].mSavedValue =
                    pPointer->mCounter;
                saved_counter_count++;
            }
        }
    }

    gGameSave_6F78C8.mCarAndScriptData.mScriptPtrCount = saved_counter_count;

    if (saved_counter_count < 300)
    {
        memset(&gGameSave_6F78C8.mCarAndScriptData.mScriptCounter[saved_counter_count],
               0,
               (300 - saved_counter_count) * sizeof(SavedScriptCounter));
    }
}

MATCH_FUNC(0x511c30)
void ScriptManager::LoadScriptCounters_511C30()
{
    for (u16 saved_counter_idx = 0; saved_counter_idx < 300; saved_counter_idx++)
    {
        SavedScriptCounter* pSaved = &gGameSave_6F78C8.mCarAndScriptData.mScriptCounter[saved_counter_idx];
        if (pSaved->mPointer > 0)
        {
            SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(GetBasePointer_512770(pSaved->mPointer));
            pPointer->mCounter = pSaved->mSavedValue;
        }
    }
}

MATCH_FUNC(0x511C60)
void ScriptManager::RestoreAnsweredPhones_511C60()
{
    for (char_type bit = 0; bit < 32; bit++)
    {
        if (((1 << bit) & gGameSave_6F78C8.mCarAndScriptData.mFlags) != 0)
        {
            SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(GetBasePointer_512770(mPhoneIds[bit]));
            pPointer->mObj->PoolGiveAndQueueObjectType_5291E0(174);
            gScriptManager_6F8060->RemovePhoneThreadByObjId_512AA0(pPointer->mObj->mId);
        }
    }
}

MATCH_FUNC(0x511d40)
void ScriptManager::SaveMapInfo_511D40()
{
    strcpy(gGameSave_6F78C8.mMapName, gGameSession_67E8E0.GetMapName_4C5940());
    strcpy(gGameSave_6F78C8.mStyleName, gGameSession_67E8E0.GetStyleName_4C5950());
    strcpy(gGameSave_6F78C8.mScriptName, gGameSession_67E8E0.GetScriptName_4C5960());
    gGameSave_6F78C8.mMapName[24] = '\n';
    gGameSave_6F78C8.mStyleName[24] = '\n';
    gGameSave_6F78C8.mScriptName[24] = '\n';
    gGameSave_6F78C8.mMainStage = gGameSession_67E8E0.GetMainStageIdx_4C5980();
    gGameSave_6F78C8.mLhv = gGameSession_67E8E0.GetStage_4C5990();
    gGameSave_6F78C8.mBonusStage = gGameSession_67E8E0.IsBonusStage_4C59A0();
}

MATCH_FUNC(0x511e10)
void ScriptManager::SaveGame_511E10(const char_type* pFileName)
{
    if (!pFileName || !*pFileName)
    {
        sprintf(gTmpBuffer_67C598, "player\\plyslot%d.svg", gGameSession_67E8E0.GetPlySlotIdx_4C59B0());
        pFileName = gTmpBuffer_67C598;
    }

    ScriptManager::SaveScriptCounters_511B90();

    gGame_0x40_67E008->mLocalPlayer->CopyPlayerDataToSave_56A1A0(&gGameSave_6F78C8.mPlayerAndWorldStats);

    gObject_5C_6F8F84->SaveObjects_52A500(&gGameSave_6F78C8.mObjectData);

    memcpy(gGameSave_6F78C8.mObjectData.mObj5CBUnCollectedTokens,
           &gObject_5C_6F8F84->mUncollectedTokens,
           50u);

    gGameSave_6F78C8.mObjectData.mSecretTokensCollected = gGameSession_67E8E0.get_secret_tokens_collected_453A80();

    u16** pColData;
    u32 colBytes;
    gmp_block_info* pBlockInfo;
    u32 blockInfoBytes;
    DmapInfoList* pMapSub;
    s32 mapSubBytes;
    gMap_0x370_6F6268->GetModifiedMapData_4E8CF0(&pColData, &colBytes, &pBlockInfo, &blockInfoBytes, &pMapSub, &mapSubBytes);

    ScriptManager::SaveMapInfo_511D40();

    {
        size_t writeLen = sizeof(SaveData);
        File::WriteBufferToFile_4A6E80(pFileName, &gGameSave_6F78C8, &writeLen);
    }

    {
        size_t writeLen = 4;
        File::AppendBufferToFile_4A6F50(pFileName, &colBytes, &writeLen);
        if (colBytes)
        {
            File::AppendBufferToFile_4A6F50(pFileName, pColData, &colBytes);
        }
    }

    {
        size_t writeLen = 4;
        File::AppendBufferToFile_4A6F50(pFileName, &blockInfoBytes, &writeLen);
        if (blockInfoBytes)
        {
            File::AppendBufferToFile_4A6F50(pFileName, pBlockInfo, &blockInfoBytes);
        }
    }

    {
        size_t writeLen = 4;
        File::AppendBufferToFile_4A6F50(pFileName, &mapSubBytes, &writeLen);
        if (mapSubBytes)
        {
            File::AppendBufferToFile_4A6F50(pFileName, pMapSub, reinterpret_cast<u32*>(&mapSubBytes));
        }
    }
}

MATCH_FUNC(0x511f80)
void ScriptManager::LoadSave_511F80(const char_type* pFileName)
{
    size_t mapSubLen;
    size_t blockInfoLen;
    size_t colDataLen;
    u16** pColData;
    gmp_block_info* pBlockInfo;
    DmapInfoList* pMapSub;

    gMap_0x370_6F6268->GetModifiedMapData_4E8CF0(&pColData, &colDataLen, &pBlockInfo, &blockInfoLen, &pMapSub, reinterpret_cast<s32*>(&mapSubLen));
    File::Global_Open_4A7060(pFileName);

    File::Global_Read_4A71C0(&gGameSave_6F78C8, sizeof(gGameSave_6F78C8));

    File::Global_Read_4A71C0(&colDataLen, 4);
    if (colDataLen > 0)
    {
        File::Global_Read_4A71C0(pColData, colDataLen);
    }

    File::Global_Read_4A71C0(&blockInfoLen, 4);
    if (blockInfoLen > 0)
    {
        File::Global_Read_4A71C0(pBlockInfo, blockInfoLen);
    }

    File::Global_Read_4A71C0(&mapSubLen, 4);
    if (mapSubLen > 0)
    {
        File::Global_Read_4A71C0(pMapSub, mapSubLen);
    }
    gMap_0x370_6F6268->OnModifiedMapDataLoaded_4E8C00(colDataLen, blockInfoLen, mapSubLen);
    File::Global_Close_4A70C0();

    ScriptManager::LoadScriptCounters_511C30();

    gObject_5C_6F8F84->RestoreObjects_52A590(&gGameSave_6F78C8.mObjectData);

    memcpy(&gObject_5C_6F8F84->mUncollectedTokens,
           gGameSave_6F78C8.mObjectData.mObj5CBUnCollectedTokens,
           sizeof(gObject_5C_6F8F84->mUncollectedTokens));

    gGameSession_67E8E0.set_secret_tokens_collected_476B10(gGameSave_6F78C8.mObjectData.mSecretTokensCollected);
    mbLoadedFromSave = true;
}

MATCH_FUNC(0x5120C0)
ScriptThread* ScriptManager::SpawnThread_5120C0(s16 start_cmd, char_type flags)
{
    ScriptThread* pThread = miss2_0x11C_Pool_6F8064->Allocate_4767A0();
    pThread->InitThread_511930(flags, start_cmd);
    return pThread;
}

MATCH_FUNC(0x512100)
SCR_CMD_HEADER* ScriptManager::FindCommandByType_512100(u16 toFind, u16 startOff)
{
    if (bSkip_mission_67D4E5)
    {
        return NULL;
    }
    for (u16 idx = 0; idx < GTA2_COUNTOF_S(mBasePointers) - startOff; ++idx)
    {
        SCR_CMD_HEADER* pHeader = ScriptManager::GetBasePointer_512770(idx + startOff);
        if (pHeader)
        {
            if (pHeader->mType == toFind)
            {
                return pHeader;
            }
        }
    }
    return NULL;
}

MATCH_FUNC(0x512160)
void ScriptManager::StartLevelThread_512160()
{
    if (!bSkip_mission_67D4E5)
    {
        ScriptThread* pThread = miss2_0x11C_Pool_6F8064->Allocate_4767A0();
        SCR_CMD_HEADER* pLevelStart = FindCommandByType_512100(59, 0);
        if (!pLevelStart)
        {
            FatalError_4A38C0(Gta2Error::ScriptfileNo_LEVELSTART_Declaration,
                              "C:\\Splitting\\Gta2\\Source\\miss2.cpp",
                              12774); // No LEVELSTART declaration in scriptfile
            return;
        }
        pThread->ExecOpCode_5061C0();
        pThread->InitThread_511930(0, pLevelStart->mCmdThis);
        gGame_0x40_67E008->IterateFirstPlayer_4B9CD0();
    }
}

// https://decomp.me/scratch/W4gXh
// See docs/match_attempts.md
MATCH_FUNC(0x5121E0)
void ScriptManager::LoadStringTbl_5121E0(u16 tableSize)
{
    u32 total_str_length = 0;
    u8* pStringDataIter1 = reinterpret_cast<u8*>(mStrings);
    while (total_str_length < tableSize)
    {
        s32 str_length = pStringDataIter1[8] + 9;
        // Both arms are the same: the original keeps the even length check's (len & ~1) as a dead value in a
        // register, which only an if/else whose arms VC6 merges reproduces (9.6f has it too).
        if ((str_length & ~1) == str_length)
        {
            total_str_length += str_length;
            pStringDataIter1 += str_length;
        }
        else
        {
            total_str_length += str_length;
            pStringDataIter1 += str_length;
        }
    }

    mpStringTbl = reinterpret_cast<str_table_normalized*>(Memory::malloc_4FE4D0(sizeof(str_table_normalized)));
    memset(mpStringTbl, 0, sizeof(str_table_normalized));

    str_table_entry* pStringDataIter2 = mStrings;
    s32 str_count = 0;
    u32 total_str_length_ = 0;
    while (total_str_length_ < tableSize)
    {
        pStringDataIter2->mZoneIdx = gMap_0x370_6F6268->FindZoneIdxByName_4DF050(pStringDataIter2->get_name(),
                                                                                        static_cast<u8>(strlen(pStringDataIter2->get_name())));
        mpStringTbl->mEntries[str_count] = pStringDataIter2;
        s32 str_length_ = pStringDataIter2->mLength + 9;
        total_str_length_ += str_length_;
        pStringDataIter2 = reinterpret_cast<str_table_entry*>(reinterpret_cast<char_type*>(pStringDataIter2) + str_length_);
        ++str_count;
    }
    mpStringTbl->mStringCount = str_count;
}

MATCH_FUNC(0x5122D0)
void ScriptManager::GetScrFileName_5122D0()
{
    const char_type* pSlashPos = strrchr(mFullScrFileName, '\\');
    if (pSlashPos)
    {
        pSlashPos++;

        memset(mScrFileName, 0, sizeof(mScrFileName));

        u32 lenAfterSlash = strlen(pSlashPos);
        while (lenAfterSlash)
        {
            if (pSlashPos[lenAfterSlash] == '.')
            {
                strncpy(mScrFileName, pSlashPos, lenAfterSlash);
                return;
            }
            lenAfterSlash--;
        }
    }
}

MATCH_FUNC(0x512330)
void ScriptManager::Load_512330(const char_type* pScrName)
{
    if (!bSkip_mission_67D4E5)
    {
        strcpy(mFullScrFileName, pScrName);
        Error_SetName_4A0770(mFullScrFileName);
        File::Global_Open_4A7060(mFullScrFileName);
        u32 len = sizeof(mBasePointers);
        File::Global_Read_4A71C0(mBasePointers, len);
        len = sizeof(mScriptData);
        File::Global_Read_4A71C0(mScriptData, len);
        u16 tableSize;
        len = sizeof(tableSize);
        File::Global_Read_4A71C0(&tableSize, len);
        len = 5118;
        File::Global_ReadRemainder_4A7250(mStrings, &len);
        File::Global_Close_4A70C0();
        LoadStringTbl_5121E0(tableSize);
        GetScrFileName_5122D0();
        LoadSubScripts_5125F0();
    }
}

MATCH_FUNC(0x512400)
u16 ScriptManager::LoadMissionScript_512400(const char_type* pScriptName, u16* pStartCmd)
{
    u16 base_ptr_idx = 0;
    if (gScriptManager_6F8060->mbPreloadMissionScripts == 0)
    {
        u32 remainder;
        strcpy(gTmpBuffer_67C598, "data\\");
        strcat(gTmpBuffer_67C598, pScriptName);
        Error_SetName_4A0770(gTmpBuffer_67C598);
        File::Global_Open_4A7060(gTmpBuffer_67C598);
        File::Global_Read_4A71C0(&base_ptr_idx, 2);
        File::Global_Read_4A71C0(pStartCmd, 2);
        File::Global_Read_4A71C0(&remainder, 4);
        File::Global_Read_4A71C0(&mBasePointers[base_ptr_idx], 0xC00);
        File::Global_ReadRemainder_4A7250(&mScriptData[mBasePointers[base_ptr_idx]], &remainder);
        File::Global_Close_4A70C0();
    }
    else
    {
        str_table_entry* pStrEntry = gScriptManager_6F8060->StrEntryByString_5030B0(pScriptName);
        u16 mission_idx = pStrEntry->mZoneIdx;
        base_ptr_idx = gScriptManager_6F8060->mMissionBasePtrIdx[mission_idx];
        *pStartCmd = gScriptManager_6F8060->mMissionStartCmd[mission_idx];
        memcpy(&gScriptManager_6F8060->mBasePointers[base_ptr_idx],
               &gScriptManager_6F8060->mMissionBasePointers[3072 * mission_idx],
               0xC00u);
        memcpy(&gScriptManager_6F8060->mScriptData[gScriptManager_6F8060->mBasePointers[base_ptr_idx]],
               &gScriptManager_6F8060->mMissionScriptData[20000 * pStrEntry->mZoneIdx],
               gScriptManager_6F8060->mMissionScriptDataLen[pStrEntry->mZoneIdx]);
    }
    return base_ptr_idx;
}

MATCH_FUNC(0x5125F0)
void ScriptManager::LoadSubScripts_5125F0()
{
    u32 data_len;
    u16 value;
    u16 mission_idx = 0;
    for (u16 i = 0; i < mpStringTbl->mStringCount; i++)
    {
        str_table_entry* pEntry = mpStringTbl->mEntries[i];
        if (static_cast<u8>(pEntry->mType) == 21)
        {
            size_t length = strlen(pEntry->get_name());

            reinterpret_cast<char_type*>(&pEntry->mPadding6)[length] = 'S';
            reinterpret_cast<char_type*>(&pEntry->mPadding6)[length + 1] = 'C';
            reinterpret_cast<char_type*>(&pEntry->mPadding6)[length + 2] = 'R';

            if (mbPreloadMissionScripts == 1)
            {
                sprintf(gTmpBuffer_67C598, "data\\%s\\", mScrFileName);
                strncat(gTmpBuffer_67C598, pEntry->get_name(), pEntry->mLength);
                File::Global_Open_4A7060(gTmpBuffer_67C598);

                File::Global_Read_4A71C0(&value, 2);
                mMissionBasePtrIdx[mission_idx] = value;
                File::Global_Read_4A71C0(&value, 2);
                mMissionStartCmd[mission_idx] = value;
                File::Global_Read_4A71C0(&data_len, 4);
                mMissionScriptDataLen[mission_idx] = data_len;

                File::Global_Read_4A71C0(&mMissionBasePointers[3072 * mission_idx], 3072);
                File::Global_ReadRemainder_4A7250(&mMissionScriptData[20000 * mission_idx], &mMissionScriptDataLen[mission_idx]);

                File::Global_Close_4A70C0();

                pEntry->mZoneIdx = mission_idx;
                mission_idx++;
            }
        }
    }
}

MATCH_FUNC(0x512770)
SCR_CMD_HEADER* ScriptManager::GetBasePointer_512770(u16 idx)
{
    if (!mBasePointers[idx])
    {
        return NULL;
    }
    return reinterpret_cast<SCR_CMD_HEADER*>(&mScriptData[mBasePointers[idx]]);
}

MATCH_FUNC(0x5127a0)
void ScriptManager::ExecuteScriptThreads_5127A0()
{
    LOCAL_STATIC_INIT(u8, sbAllowMiss2Logging, 1, 0x6212EC);
    if (!bSkip_mission_67D4E5)
    {
        if (sbAllowMiss2Logging)
        {
            if (gDo_miss_logging_67D6BC)
            {
                miss2_0x11C_Pool_6F8064->LogAll_47F4D0();
                gMiss2Log_6F7698.Write_Log_4D9650("\n");
            }
        }
        miss2_0x11C_Pool_6F8064->mPool.UpdatePoolNoDeallocate();
    }
    gCarManager_6C_677930->mModelCheckDestroyed = false;
}

MATCH_FUNC(0x5128a0)
ThreadTrigger* ScriptManager::FindCarThread_5128A0(s32 ped_id, s32 car_id)
{
    ThreadTrigger* pTrigger = &mCarTriggers[0];
    for (u16 thread_idx = 0; thread_idx < GTA2_COUNTOF(mCarTriggers); thread_idx++)
    {
        if (pTrigger->mPedId == ped_id && pTrigger->mTargetId == car_id)
        {
            return pTrigger;
        }
        pTrigger++;
    }
    return NULL;
}

MATCH_FUNC(0x5128d0)
void ScriptManager::AddCarThread_5128D0(s32 ped_id, s32 car_id, u16 cmd_line)
{
    ThreadTrigger* pTrigger = &mCarTriggers[0];
    for (u16 i = 0; i < GTA2_COUNTOF(mCarTriggers); i++)
    {
        if (pTrigger->mCmdLine == 0)
        {
            break;
        }
        pTrigger++;
    }

    pTrigger->mPedId = ped_id;
    pTrigger->mTargetId = car_id;
    pTrigger->mCmdLine = cmd_line;
    ++mCarTriggerCount;
}

MATCH_FUNC(0x512910)
bool ScriptManager::TriggerCarThread_512910(s32 ped_id, s32 car_id)
{
    ThreadTrigger* pTrigger = ScriptManager::FindCarThread_5128A0(ped_id, car_id);
    if (pTrigger)
    {
        SCR_THREAD* pThreadCmd = static_cast<SCR_THREAD*>(GetBasePointer_512770(pTrigger->mCmdLine));
        pThreadCmd->mScriptThread = SpawnThread_5120C0(pThreadCmd->mThreadStartCmd, 0);

        if (pThreadCmd->mScriptThread)
        {
            pThreadCmd->mThreadId = pThreadCmd->mScriptThread->mThreadId;
            pTrigger->mPedId = 0;
            pTrigger->mTargetId = 0;
            pTrigger->mCmdLine = 0;
            --mCarTriggerCount;
            return true;
        }
    }
    return false;
}

MATCH_FUNC(0x512980)
ThreadTrigger* ScriptManager::FindPhoneThread_512980(s32 ped_id, s32 obj_id)
{
    ThreadTrigger* pTrigger = &mPhoneTriggers[0];
    for (u16 thread_idx = 0; thread_idx < GTA2_COUNTOF(mPhoneTriggers); thread_idx++)
    {
        if (pTrigger->mPedId == ped_id && pTrigger->mTargetId == obj_id)
        {
            return pTrigger;
        }
        pTrigger++;
    }
    return NULL;
}

MATCH_FUNC(0x5129b0)
void ScriptManager::AddPhoneThread_5129B0(s32 ped_id, s32 obj_id, u16 cmd_line)
{
    ThreadTrigger* pTrigger = &mPhoneTriggers[0];
    for (u16 i = 0; i < GTA2_COUNTOF(mPhoneTriggers) && pTrigger->mCmdLine != 0; i++)
    {
        pTrigger++;
    }
    pTrigger->mPedId = ped_id;
    pTrigger->mTargetId = obj_id;
    pTrigger->mCmdLine = cmd_line;
    mPhoneTriggerCount++;
}

MATCH_FUNC(0x5129f0)
bool ScriptManager::AnswerPhone_5129F0(s32 ped_id, s32 obj_id)
{
    ThreadTrigger* pTrigger = ScriptManager::FindPhoneThread_512980(ped_id, obj_id);
    if (pTrigger)
    {
        SCR_THREAD* pThreadCmd = static_cast<SCR_THREAD*>(GetBasePointer_512770(pTrigger->mCmdLine));
        SCR_ANSWER_PHONE* pTriggerCmd;

        switch (pThreadCmd->mType)
        {
            case SCRCMD_ANSWER_PHONE:
                pTriggerCmd = reinterpret_cast<SCR_ANSWER_PHONE*>(pThreadCmd);
                pTriggerCmd->mAnswered = 1;
                return true;
            case SCRCMD_THREAD_DECLARE4:
                pThreadCmd->mScriptThread = SpawnThread_5120C0(pThreadCmd->mThreadStartCmd, 0);
                if (pThreadCmd->mScriptThread)
                {
                    pThreadCmd->mThreadId = pThreadCmd->mScriptThread->mThreadId;
                }
                return true;
            default:
                return false;
        }
    }
    return false;
}

MATCH_FUNC(0x512a70)
ThreadTrigger* ScriptManager::RemovePhoneThread_512A70(s32 ped_id, s32 obj_id)
{
    ThreadTrigger* pTrigger = ScriptManager::FindPhoneThread_512980(ped_id, obj_id);
    if (pTrigger)
    {
        pTrigger->mPedId = 0;
        pTrigger->mTargetId = 0;
        pTrigger->mCmdLine = 0;
        --mPhoneTriggerCount;
    }
    return pTrigger;
}

MATCH_FUNC(0x512aa0)
ThreadTrigger* ScriptManager::RemovePhoneThreadByObjId_512AA0(s32 obj_id)
{
    ThreadTrigger* pTrigger = ScriptManager::FindPhoneThreadByObjId_512AD0(obj_id);
    if (pTrigger)
    {
        pTrigger = ScriptManager::RemovePhoneThread_512A70(pTrigger->mPedId, obj_id);
    }
    return pTrigger;
}

MATCH_FUNC(0x512ad0)
ThreadTrigger* ScriptManager::FindPhoneThreadByObjId_512AD0(s32 obj_id)
{
    ThreadTrigger* pTrigger = &mPhoneTriggers[0];
    for (u16 thread_idx = 0; thread_idx < GTA2_COUNTOF(mPhoneTriggers); thread_idx++)
    {
        if (pTrigger->mTargetId == obj_id)
        {
            return pTrigger;
        }
        pTrigger++;
    }
    return NULL;
}

MATCH_FUNC(0x512af0)
bool ScriptManager::TestAndRecordWeaponHit_512AF0(s32 id, char_type weapon_idx, bool bIsPed)
{
    WeaponCheckTable* pFree = NULL;
    WeaponCheckTable* pTable = &mWeaponCheckTable[0];

    for (u8 i = 0; i < 15; pTable++, i++)
    {
        if (pTable->mEntityId == 0 && pTable->mWeaponIdx == 0 && !pFree)
        {
            pFree = pTable;
        }

        if (pTable->mEntityId == id)
        {
            if (pTable->mWeaponIdx == weapon_idx || pTable->mWeaponIdx == ped_death_cause::any_23)
            {
                if (bIsPed)
                {
                    if (IsPedCheck_4759C0(pTable))
                    {
                        return true;
                    }
                }
                else
                {
                    if (IsCarCheck_4759A0(pTable))
                    {
                        return true;
                    }
                }
            }
        }
    }

    if (pFree)
    {
        pFree->mEntityId = id;
        pFree->mWeaponIdx = weapon_idx;
        if (bIsPed)
        {
            pFree->mFlags = weapon_check_flag::ped_4;
        }
        else
        {
            pFree->mFlags = weapon_check_flag::car_2;
        }
        mWeaponCheckCount++;
    }
    return false;
}

MATCH_FUNC(0x512ba0)
void ScriptManager::ClearWeaponHitFlag_512BA0(s32 id, bool bIsPed)
{
    WeaponCheckTable* pTable = &mWeaponCheckTable[0];

    for (u8 i = 0; i < 15; pTable++, i++)
    {
        if (pTable->mEntityId == id)
        {
            if (bIsPed)
            {
                if (IsPedCheck_4759C0(pTable))
                {
                    // je clear; jmp next in the original: the shared tail needs a goto (duplicated
                    // bodies, early continue/return and a combined condition all change the code)
                    goto clear;
                }
                continue;
            }
            if (!IsCarCheck_4759A0(pTable))
            {
                continue;
            }
        clear:
            pTable->mEntityId = 0;
            pTable->mWeaponIdx = 0;
            pTable->mHitProjectileType = 0;
            pTable->mFlags = 0;
            mWeaponCheckCount--;
        }
    }
}

// https://decomp.me/scratch/r6LhX
MATCH_FUNC(0x512c00)
void ScriptManager::RecordWeaponHit_512C00(s32 entity_id, s32 projectile_model, bool bIsPed)
{
    WeaponCheckTable* pTable = &mWeaponCheckTable[0];

    s8 projectile_type = GetDeathCauseForObjectModel_48E780(projectile_model);

    for (u8 i = 0; i < 15; pTable++, i++)
    {
        if (pTable->mEntityId == entity_id)
        {
            if (pTable->mWeaponIdx == ped_death_cause::any_23 || pTable->mWeaponIdx == projectile_type)
            {
                if (bIsPed)
                {
                    if (IsPedCheck_4759C0(pTable))
                    {
                        // je set; jmp next in the original: the shared tail needs a goto (duplicated
                        // bodies, early continue/return and a combined condition all change the code)
                        goto set;
                    }
                    return;
                }
                if (!IsCarCheck_4759A0(pTable))
                {
                    return;
                }
            set:
                pTable->mFlags |= weapon_check_flag::hit_1;
                pTable->mHitProjectileType = projectile_type;
                return;
            }
        }
    }
}

MATCH_FUNC(0x512c70)
bool ScriptManager::IsWeaponHitRecorded_512C70(s32 id, char_type weapon_idx, bool bIsPed) const
{
    const WeaponCheckTable* pTable = &mWeaponCheckTable[0];

    for (u8 i = 0; i < 15; pTable++, i++)
    {
        if (pTable->mEntityId == id)
        {
            if (pTable->mWeaponIdx == weapon_idx || pTable->mWeaponIdx == ped_death_cause::any_23)
            {
                if (bIsPed)
                {
                    if (IsPedCheck_4759C0(pTable))
                    {
                        return IsCheckHit_475980(pTable);
                    }
                }
                else
                {
                    if (IsCarCheck_4759A0(pTable))
                    {
                        return IsCheckHit_475980(pTable);
                    }
                }
            }
        }
    }
    return false;
}

// https://decomp.me/scratch/qh4EW
MATCH_FUNC(0x512ce0)
ScriptManager::ScriptManager()
{
    mpStringTbl = NULL;
    if (!bSkip_mission_67D4E5)
    {
        memset(mScriptData, 0, sizeof(mScriptData));
        if (!miss2_0x11C_Pool_6F8064)
        {
            miss2_0x11C_Pool_6F8064 = new ScriptThreadPool();
        }
    }
    mStrings = static_cast<str_table_entry*>(Memory::malloc_4FE4D0(0x1400u));
    memset(mFullScrFileName, 0, sizeof(mFullScrFileName));
    memset(mScriptData, 0, sizeof(mScriptData));
    memset(mBasePointers, 0, sizeof(mBasePointers));
    memset(mStrings, 0, 0x1400u);
    mCarTriggerCount = 0;
    memset(mCarTriggers, 0, sizeof(mCarTriggers));
    mPhoneTriggerCount = 0;
    memset(mPhoneTriggers, 0, sizeof(mPhoneTriggers));
    mWeaponCheckCount = 0;
    memset(mWeaponCheckTable, 0, sizeof(mWeaponCheckTable));
    memset(&gGameSave_6F78C8, 0, sizeof(gGameSave_6F78C8));

    MissionCleanupList* pCleanupList = new MissionCleanupList();
    gMiss2_25C_6F805C = pCleanupList;
    if (!pCleanupList)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\miss2.cpp", 13630);
    }

    memset(mMissionScriptData, 0, sizeof(mMissionScriptData));
    memset(mMissionBasePointers, 0, sizeof(mMissionBasePointers));
    memset(mMissionStartCmd, 0, sizeof(mMissionStartCmd));
    memset(mMissionBasePtrIdx, 0, sizeof(mMissionBasePtrIdx));
    memset(mMissionScriptDataLen, 0, sizeof(mMissionScriptDataLen));
    mMissionFlag = NULL;
    mbPreloadMissionScripts = 1;
    mbLoadedFromSave = false;
    mFinishScore = 0;
    mTotalMissions = 0;
    mTotalSecrets = 0;
    mGang1MissionsTotal = 0;
    mGang2MissionsTotal = 0;
    mGang3MissionsTotal = 0;
    mPassedFlag = NULL;
    m1PassedFlag = NULL;
    m2PassedFlag = NULL;
    m3PassedFlag = NULL;
    mSecretsPassed = NULL;
    mSecretsFailed = NULL;
    mCarList = NULL;
    mGang1MissionFlag = NULL;
    mGang2MissionFlag = NULL;
    mGang3MissionFlag = NULL;
    mNextGangIdx = 0;
    mWantedCarModel = car_model_enum::none;
    mGang1DeathBase = 0;
    mGang2DeathBase = 0;
    mGang3DeathBase = 0;
    mDeathArrState = 0;
    mDeathArrestFlag = false;
    mbKillFrenzyActive = false;
    mUnused468 = 0;

    if (gDo_miss_logging_67D6BC)
    {
        gMiss2Log_6F7698.Open_4D9470("test\\MISS_LOG.TXT", 1);
    }

    memset(mScrFileName, 0, sizeof(mScrFileName));
    memset(mPhoneIds, 0, sizeof(mPhoneIds));
    memset(mSavedGangRespect, 0, sizeof(mSavedGangRespect));
    memset(mBasicKf, 0, sizeof(mBasicKf));

    gStoredCar_6F7560 = NULL;
    gStoredCarId_6F78B4 = 0;
}

MATCH_FUNC(0x5130e0)
ScriptManager::~ScriptManager()
{
    if (miss2_0x11C_Pool_6F8064)
    {
        GTA2_DELETE_AND_NULL(miss2_0x11C_Pool_6F8064);
    }

    if (mpStringTbl)
    {
        memset(mpStringTbl->mEntries, 0, sizeof(mpStringTbl->mEntries));
        free(mpStringTbl);
        mpStringTbl = NULL;
    }

    if (mStrings)
    {
        free(mStrings);
        mStrings = NULL;
    }

    memset(&gGameSave_6F78C8, 0, sizeof(gGameSave_6F78C8));

    if (gMiss2_25C_6F805C)
    {
        GTA2_DELETE_AND_NULL(gMiss2_25C_6F805C);
    }

    mPassedFlag = NULL;
    m1PassedFlag = NULL;
    m2PassedFlag = NULL;
    m3PassedFlag = NULL;
    mCarList = NULL;
    mSecretsPassed = NULL;
    mSecretsFailed = NULL;
    mGang1MissionFlag = NULL;
    mGang2MissionFlag = NULL;
    mGang3MissionFlag = NULL;
    gStoredCarId_6F78B4 = 0;
    gStoredCar_6F7560 = NULL;
}