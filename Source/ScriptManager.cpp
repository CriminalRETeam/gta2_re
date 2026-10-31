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

DEFINE_GLOBAL(ScriptManager*, gScriptManager, 0x6F8060);
DEFINE_GLOBAL(SaveData, gGameSave, 0x6F78C8);
DEFINE_GLOBAL(s32, gStoredCarId, 0x6F78B4);

EXTERN_GLOBAL(SCR_POINTER*, gBasePtr);

// Internal linkage as in the original, where all users were in one file: VC6 allocates registers differently for it.
DEFINE_STATIC_GLOBAL(Car*, gStoredCar, 0x6F7560);

MATCH_FUNC(0x502dc0)
void MissionCleanupList::MissionCleanUp()
{
    MissionCleanupEntry* pIter = &mEntries[0];
    PedGroup* pGroup = gGame_0x40->mLocalPlayer->mPlayerPed->mPedGroup;
    if (pGroup)
    {
        pGroup->DestroyGroup();
    }

    if (gStoredCar)
    {
        if (gStoredCar->mCarId == gStoredCarId)
        {
            gGame_0x40->mLocalPlayer->RemoveCarFromHistory(gStoredCar);
            gStoredCar->SetDriverPedType(ped_type::dummy_3);
            if (!gStoredCar->IsDeactivated())
            {
                gStoredCar->MarkForDespawn();
            }
            gStoredCar = NULL;
            gStoredCarId = 0;
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
                        pIter->mObj->Dealloc();
                        pIter->mObj->RequestRemovalWhenOffScreen();
                    }
                    break;
                case cleanup_entry_type::ped_3:
                    if (pIter->mPed->mId == static_cast<u32>(pIter->mEntityId))
                    {
                        PedGroup* pPedGroup = pIter->mPed->mPedGroup;
                        if (pPedGroup)
                        {
                            pPedGroup->DisbandGroup();
                        }
                        pIter->mPed->ForceDoNothing();
                        pIter->mPed->mBf21C.bScheduledForRemoval = true;
                    }
                    break;
                case cleanup_entry_type::car_1:
                    if (pIter->mCar->mCarId == pIter->mEntityId)
                    {
                        gGame_0x40->mLocalPlayer->RemoveCarFromHistory(pIter->mCar);
                        pIter->mCar->SetDriverPedType(ped_type::dummy_3);
                        if (!pIter->mCar->IsDeactivated())
                        {
                            pIter->mCar->MarkForDespawn();
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
    gScriptManager->mWantedCarModel = car_model_enum::none;
}


MATCH_FUNC(0x503080)
str_table_entry* ScriptManager::FindStringById(s16 stringId) const
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
str_table_entry* ScriptManager::StrEntryByString(const char_type* strToFind) const
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

EXTERN_GLOBAL_ARRAY(u8, kDecidePowerupGenTypes, 19);

// Sets the generator type for `car_model` from the script's car list, or type 0x41 when it isn't
// in the list. Reads the global rather than `this`.
MATCH_FUNC(0x509180)
void ScriptThread::SCRCMD_STORE_CAR_INFO_509180()
{
    SCR_TWO_PARAMS* pCmd = reinterpret_cast<SCR_TWO_PARAMS*>(gBasePtr);
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(gBasePtr->mIndex));
    SCR_POINTER* pParam2 = static_cast<SCR_POINTER*>(gScriptManager->GetBasePointer(pCmd->mUnsigned2));
    Ped* pChar = pPointer->mChar;

    if (pChar != NULL)
    {
        if (pChar->has_car())
        {
            pParam2->mCar = pChar->get_car();

            if (gStoredCar != NULL)
            {
                if (gStoredCar != pParam2->mCar && gStoredCar->mCarId != pParam2->mCar->mCarId)
                {
                    if (gStoredCar->mCarId == gStoredCarId)
                    {
                        if (gCarManager_6C->CanAllocateCarOfKind(car_kind::recycled_1))
                        {
                            gStoredCar->ReassignAllocatedCarType(car_kind::recycled_1);
                            gGame_0x40->mLocalPlayer->RemoveCarFromHistory(gStoredCar);
                            gStoredCar->SetDriverPedType(ped_type::dummy_3);

                            if (!gStoredCar->IsDeactivated())
                            {
                                gStoredCar->MarkForDespawn();
                            }
                        }
                        else if (gCarManager_6C->CanAllocateCarOfKindWithSpare(car_kind::mission_car_8))
                        {
                            gGame_0x40->mLocalPlayer->RemoveCarFromHistory(gStoredCar);
                            gStoredCar->SetDriverPedType(ped_type::dummy_3);

                            if (!gStoredCar->IsDeactivated())
                            {
                                gStoredCar->MarkForDespawn();
                            }
                        }
                        else
                        {
                            gGame_0x40->mLocalPlayer->RemoveCarFromHistory(gStoredCar);
                            gStoredCar->SetDriverPedType(ped_type::dummy_3);

                            gStoredCar->DespawnSoon_421490();
                        }
                    }

                    gStoredCar = pParam2->mCar;
                    gStoredCarId = pParam2->mCar->mCarId;

                    gStoredCar->ReassignAllocatedCarType(car_kind::mission_car_8);

                    if (!gPublicTransport_181C->is_bus(pParam2->mCar))
                    {
                        pParam2->mCar->LockoutThiefUnlessPermanent();
                    }
                }
            }
            else
            {
                gStoredCar = pParam2->mCar;
                gStoredCarId = pParam2->mCar->mCarId;
                gStoredCar->ReassignAllocatedCarType(car_kind::mission_car_8);
            }
        }
    }

    ScriptThread::Next(gBasePtr);
}

MATCH_FUNC(0x511A70)
void ScriptManager::SetGeneratorTypeForCar(s32 car_model, Generator* pGen)
{
    u8* pList = reinterpret_cast<u8*>(gScriptManager->mCarList);
    u8 i;
    for (i = 0; i < 19; pList++, i++)
    {
        if (car_model == *pList)
        {
            pGen->mGenType = kDecidePowerupGenTypes[i];
            if (kDecidePowerupGenTypes[i] < 0x5B)
            {
                pGen->Activate();
                pGen->mSpawnsLeft = 3;
            }
            else
            {
                pGen->Activate();
                pGen->mSpawnsLeft = 1;
            }
            break;
        }
    }

    if (i == 19)
    {
        pGen->Activate();
        pGen->mSpawnsLeft = 3;
        pGen->mGenType = 0x41;
    }
}

MATCH_FUNC(0x511b10)
char_type ScriptManager::AddCranePowerupTrigger(s16 idx)
{
    SCR_DECLARE_CRANE_POWERUP* pCmd = static_cast<SCR_DECLARE_CRANE_POWERUP*>(GetBasePointer(idx));
    SCR_POINTER* pGenerator = static_cast<SCR_POINTER*>(GetBasePointer(pCmd->mGenerator));
    return gCarThreadTriggers_214->AddThreadTrigger(car_thread_trigger_type::any_car_1,
                                       car_thread_trigger_action::set_generator_car_type_3,
                                       reinterpret_cast<SCR_THREAD*>(pGenerator->mGenerator),
                                       Fix16(pCmd->mPos.mX) + kFpHalf_6F75F0,
                                       Fix16(pCmd->mPos.mY) + kFpHalf_6F75F0,
                                       Fix16(pCmd->mPos.mZ),
                                       kFpHalf_6F75F0,
                                       kFpHalf_6F75F0);
}

MATCH_FUNC(0x511b90)
void ScriptManager::SaveScriptCounters()
{
    u16 saved_counter_count = 0;
    for (u16 ptr = 0; ptr < GTA2_COUNTOF(mBasePointers); ptr++)
    {
        if (mBasePointers[ptr])
        {
            SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(GetBasePointer(ptr));
            if (pPointer->mType == SCRCMD_COUNTER_SAVE || pPointer->mType == SCRCMD_COUNTER_SET_SAVE)
            {
                gGameSave.mCarAndScriptData.mScriptCounter[saved_counter_count].mPointer =
                    pPointer->mCmdThis;
                gGameSave.mCarAndScriptData.mScriptCounter[saved_counter_count].mSavedValue =
                    pPointer->mCounter;
                saved_counter_count++;
            }
        }
    }

    gGameSave.mCarAndScriptData.mScriptPtrCount = saved_counter_count;

    if (saved_counter_count < 300)
    {
        memset(&gGameSave.mCarAndScriptData.mScriptCounter[saved_counter_count],
               0,
               (300 - saved_counter_count) * sizeof(SavedScriptCounter));
    }
}

MATCH_FUNC(0x511c30)
void ScriptManager::LoadScriptCounters()
{
    for (u16 saved_counter_idx = 0; saved_counter_idx < 300; saved_counter_idx++)
    {
        SavedScriptCounter* pSaved = &gGameSave.mCarAndScriptData.mScriptCounter[saved_counter_idx];
        if (pSaved->mPointer > 0)
        {
            SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(GetBasePointer(pSaved->mPointer));
            pPointer->mCounter = pSaved->mSavedValue;
        }
    }
}

MATCH_FUNC(0x511C60)
void ScriptManager::RestoreAnsweredPhones()
{
    for (char_type bit = 0; bit < 32; bit++)
    {
        if (((1 << bit) & gGameSave.mCarAndScriptData.mFlags) != 0)
        {
            SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(GetBasePointer(mPhoneIds[bit]));
            pPointer->mObj->PoolGiveAndQueueObjectType(174);
            gScriptManager->RemovePhoneThreadByObjId(pPointer->mObj->mId);
        }
    }
}

MATCH_FUNC(0x511d40)
void ScriptManager::SaveMapInfo()
{
    strcpy(gGameSave.mMapName, gGameSession.GetMapName());
    strcpy(gGameSave.mStyleName, gGameSession.GetStyleName());
    strcpy(gGameSave.mScriptName, gGameSession.GetScriptName());
    gGameSave.mMapName[24] = '\n';
    gGameSave.mStyleName[24] = '\n';
    gGameSave.mScriptName[24] = '\n';
    gGameSave.mMainStage = gGameSession.GetMainStageIdx();
    gGameSave.mLhv = gGameSession.GetStage();
    gGameSave.mBonusStage = gGameSession.IsBonusStage();
}

MATCH_FUNC(0x511e10)
void ScriptManager::SaveGame(const char_type* pFileName)
{
    if (!pFileName || !*pFileName)
    {
        sprintf(gTmpBuffer, "player\\plyslot%d.svg", gGameSession.GetPlySlotIdx());
        pFileName = gTmpBuffer;
    }

    ScriptManager::SaveScriptCounters();

    gGame_0x40->mLocalPlayer->CopyPlayerDataToSave(&gGameSave.mPlayerAndWorldStats);

    gObject_5C->SaveObjects(&gGameSave.mObjectData);

    memcpy(gGameSave.mObjectData.mObj5CBUnCollectedTokens,
           &gObject_5C->mUncollectedTokens,
           50u);

    gGameSave.mObjectData.mSecretTokensCollected = gGameSession.get_secret_tokens_collected();

    u16** pColData;
    u32 colBytes;
    gmp_block_info* pBlockInfo;
    u32 blockInfoBytes;
    DmapInfoList* pMapSub;
    s32 mapSubBytes;
    gMap_0x370->GetModifiedMapData(&pColData, &colBytes, &pBlockInfo, &blockInfoBytes, &pMapSub, &mapSubBytes);

    ScriptManager::SaveMapInfo();

    {
        size_t writeLen = sizeof(SaveData);
        File::WriteBufferToFile(pFileName, &gGameSave, &writeLen);
    }

    {
        size_t writeLen = 4;
        File::AppendBufferToFile(pFileName, &colBytes, &writeLen);
        if (colBytes)
        {
            File::AppendBufferToFile(pFileName, pColData, &colBytes);
        }
    }

    {
        size_t writeLen = 4;
        File::AppendBufferToFile(pFileName, &blockInfoBytes, &writeLen);
        if (blockInfoBytes)
        {
            File::AppendBufferToFile(pFileName, pBlockInfo, &blockInfoBytes);
        }
    }

    {
        size_t writeLen = 4;
        File::AppendBufferToFile(pFileName, &mapSubBytes, &writeLen);
        if (mapSubBytes)
        {
            File::AppendBufferToFile(pFileName, pMapSub, reinterpret_cast<u32*>(&mapSubBytes));
        }
    }
}

MATCH_FUNC(0x511f80)
void ScriptManager::LoadSave(const char_type* pFileName)
{
    size_t mapSubLen;
    size_t blockInfoLen;
    size_t colDataLen;
    u16** pColData;
    gmp_block_info* pBlockInfo;
    DmapInfoList* pMapSub;

    gMap_0x370->GetModifiedMapData(&pColData, &colDataLen, &pBlockInfo, &blockInfoLen, &pMapSub, reinterpret_cast<s32*>(&mapSubLen));
    File::Global_Open(pFileName);

    File::Global_Read_4A71C0(&gGameSave, sizeof(gGameSave));

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
    gMap_0x370->OnModifiedMapDataLoaded(colDataLen, blockInfoLen, mapSubLen);
    File::Global_Close();

    ScriptManager::LoadScriptCounters();

    gObject_5C->RestoreObjects(&gGameSave.mObjectData);

    memcpy(&gObject_5C->mUncollectedTokens,
           gGameSave.mObjectData.mObj5CBUnCollectedTokens,
           sizeof(gObject_5C->mUncollectedTokens));

    gGameSession.set_secret_tokens_collected(gGameSave.mObjectData.mSecretTokensCollected);
    mbLoadedFromSave = true;
}

MATCH_FUNC(0x5120C0)
ScriptThread* ScriptManager::SpawnThread(s16 start_cmd, char_type flags)
{
    ScriptThread* pThread = miss2_0x11C_Pool->Allocate_4767A0();
    pThread->InitThread(flags, start_cmd);
    return pThread;
}

MATCH_FUNC(0x512100)
SCR_CMD_HEADER* ScriptManager::FindCommandByType(u16 toFind, u16 startOff)
{
    if (bSkip_mission)
    {
        return NULL;
    }
    for (u16 idx = 0; idx < GTA2_COUNTOF_S(mBasePointers) - startOff; ++idx)
    {
        SCR_CMD_HEADER* pHeader = ScriptManager::GetBasePointer(idx + startOff);
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
void ScriptManager::StartLevelThread()
{
    if (!bSkip_mission)
    {
        ScriptThread* pThread = miss2_0x11C_Pool->Allocate_4767A0();
        SCR_CMD_HEADER* pLevelStart = FindCommandByType(59, 0);
        if (!pLevelStart)
        {
            FatalError_4A38C0(Gta2Error::ScriptfileNo_LEVELSTART_Declaration,
                              "C:\\Splitting\\Gta2\\Source\\miss2.cpp",
                              12774); // No LEVELSTART declaration in scriptfile
            return;
        }
        pThread->ExecOpCode();
        pThread->InitThread(0, pLevelStart->mCmdThis);
        gGame_0x40->IterateFirstPlayer();
    }
}

// https://decomp.me/scratch/W4gXh
// See docs/match_attempts.md
MATCH_FUNC(0x5121E0)
void ScriptManager::LoadStringTbl(u16 tableSize)
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
        pStringDataIter2->mZoneIdx = gMap_0x370->FindZoneIdxByName(pStringDataIter2->get_name(),
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
void ScriptManager::GetScrFileName()
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
void ScriptManager::Load(const char_type* pScrName)
{
    if (!bSkip_mission)
    {
        strcpy(mFullScrFileName, pScrName);
        Error_SetName(mFullScrFileName);
        File::Global_Open(mFullScrFileName);
        u32 len = sizeof(mBasePointers);
        File::Global_Read_4A71C0(mBasePointers, len);
        len = sizeof(mScriptData);
        File::Global_Read_4A71C0(mScriptData, len);
        u16 tableSize;
        len = sizeof(tableSize);
        File::Global_Read_4A71C0(&tableSize, len);
        len = 5118;
        File::Global_ReadRemainder(mStrings, &len);
        File::Global_Close();
        LoadStringTbl(tableSize);
        GetScrFileName();
        LoadSubScripts();
    }
}

MATCH_FUNC(0x512400)
u16 ScriptManager::LoadMissionScript(const char_type* pScriptName, u16* pStartCmd)
{
    u16 base_ptr_idx = 0;
    if (gScriptManager->mbPreloadMissionScripts == 0)
    {
        u32 remainder;
        strcpy(gTmpBuffer, "data\\");
        strcat(gTmpBuffer, pScriptName);
        Error_SetName(gTmpBuffer);
        File::Global_Open(gTmpBuffer);
        File::Global_Read_4A71C0(&base_ptr_idx, 2);
        File::Global_Read_4A71C0(pStartCmd, 2);
        File::Global_Read_4A71C0(&remainder, 4);
        File::Global_Read_4A71C0(&mBasePointers[base_ptr_idx], 0xC00);
        File::Global_ReadRemainder(&mScriptData[mBasePointers[base_ptr_idx]], &remainder);
        File::Global_Close();
    }
    else
    {
        str_table_entry* pStrEntry = gScriptManager->StrEntryByString(pScriptName);
        u16 mission_idx = pStrEntry->mZoneIdx;
        base_ptr_idx = gScriptManager->mMissionBasePtrIdx[mission_idx];
        *pStartCmd = gScriptManager->mMissionStartCmd[mission_idx];
        memcpy(&gScriptManager->mBasePointers[base_ptr_idx],
               &gScriptManager->mMissionBasePointers[3072 * mission_idx],
               0xC00u);
        memcpy(&gScriptManager->mScriptData[gScriptManager->mBasePointers[base_ptr_idx]],
               &gScriptManager->mMissionScriptData[20000 * pStrEntry->mZoneIdx],
               gScriptManager->mMissionScriptDataLen[pStrEntry->mZoneIdx]);
    }
    return base_ptr_idx;
}

MATCH_FUNC(0x5125F0)
void ScriptManager::LoadSubScripts()
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
                sprintf(gTmpBuffer, "data\\%s\\", mScrFileName);
                strncat(gTmpBuffer, pEntry->get_name(), pEntry->mLength);
                File::Global_Open(gTmpBuffer);

                File::Global_Read_4A71C0(&value, 2);
                mMissionBasePtrIdx[mission_idx] = value;
                File::Global_Read_4A71C0(&value, 2);
                mMissionStartCmd[mission_idx] = value;
                File::Global_Read_4A71C0(&data_len, 4);
                mMissionScriptDataLen[mission_idx] = data_len;

                File::Global_Read_4A71C0(&mMissionBasePointers[3072 * mission_idx], 3072);
                File::Global_ReadRemainder(&mMissionScriptData[20000 * mission_idx], &mMissionScriptDataLen[mission_idx]);

                File::Global_Close();

                pEntry->mZoneIdx = mission_idx;
                mission_idx++;
            }
        }
    }
}

MATCH_FUNC(0x512770)
SCR_CMD_HEADER* ScriptManager::GetBasePointer(u16 idx)
{
    if (!mBasePointers[idx])
    {
        return NULL;
    }
    return reinterpret_cast<SCR_CMD_HEADER*>(&mScriptData[mBasePointers[idx]]);
}

MATCH_FUNC(0x5127a0)
void ScriptManager::ExecuteScriptThreads()
{
    LOCAL_STATIC_INIT(u8, sbAllowMiss2Logging, 1, 0x6212EC);
    if (!bSkip_mission)
    {
        if (sbAllowMiss2Logging)
        {
            if (gDo_miss_logging)
            {
                miss2_0x11C_Pool->LogAll_47F4D0();
                gMiss2Log.Write_Log("\n");
            }
        }
        miss2_0x11C_Pool->mPool.UpdatePoolNoDeallocate();
    }
    gCarManager_6C->mModelCheckDestroyed = false;
}

MATCH_FUNC(0x5128a0)
ThreadTrigger* ScriptManager::FindCarThread(s32 ped_id, s32 car_id)
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
void ScriptManager::AddCarThread(s32 ped_id, s32 car_id, u16 cmd_line)
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
bool ScriptManager::TriggerCarThread(s32 ped_id, s32 car_id)
{
    ThreadTrigger* pTrigger = ScriptManager::FindCarThread(ped_id, car_id);
    if (pTrigger)
    {
        SCR_THREAD* pThreadCmd = static_cast<SCR_THREAD*>(GetBasePointer(pTrigger->mCmdLine));
        pThreadCmd->mScriptThread = SpawnThread(pThreadCmd->mThreadStartCmd, 0);

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
ThreadTrigger* ScriptManager::FindPhoneThread(s32 ped_id, s32 obj_id)
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
void ScriptManager::AddPhoneThread(s32 ped_id, s32 obj_id, u16 cmd_line)
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
bool ScriptManager::AnswerPhone(s32 ped_id, s32 obj_id)
{
    ThreadTrigger* pTrigger = ScriptManager::FindPhoneThread(ped_id, obj_id);
    if (pTrigger)
    {
        SCR_THREAD* pThreadCmd = static_cast<SCR_THREAD*>(GetBasePointer(pTrigger->mCmdLine));
        SCR_ANSWER_PHONE* pTriggerCmd;

        switch (pThreadCmd->mType)
        {
            case SCRCMD_ANSWER_PHONE:
                pTriggerCmd = reinterpret_cast<SCR_ANSWER_PHONE*>(pThreadCmd);
                pTriggerCmd->mAnswered = 1;
                return true;
            case SCRCMD_THREAD_DECLARE4:
                pThreadCmd->mScriptThread = SpawnThread(pThreadCmd->mThreadStartCmd, 0);
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
ThreadTrigger* ScriptManager::RemovePhoneThread(s32 ped_id, s32 obj_id)
{
    ThreadTrigger* pTrigger = ScriptManager::FindPhoneThread(ped_id, obj_id);
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
ThreadTrigger* ScriptManager::RemovePhoneThreadByObjId(s32 obj_id)
{
    ThreadTrigger* pTrigger = ScriptManager::FindPhoneThreadByObjId(obj_id);
    if (pTrigger)
    {
        pTrigger = ScriptManager::RemovePhoneThread(pTrigger->mPedId, obj_id);
    }
    return pTrigger;
}

MATCH_FUNC(0x512ad0)
ThreadTrigger* ScriptManager::FindPhoneThreadByObjId(s32 obj_id)
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
bool ScriptManager::TestAndRecordWeaponHit(s32 id, char_type weapon_idx, bool bIsPed)
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
                    if (IsPedCheck(pTable))
                    {
                        return true;
                    }
                }
                else
                {
                    if (IsCarCheck(pTable))
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
void ScriptManager::ClearWeaponHitFlag(s32 id, bool bIsPed)
{
    WeaponCheckTable* pTable = &mWeaponCheckTable[0];

    for (u8 i = 0; i < 15; pTable++, i++)
    {
        if (pTable->mEntityId == id)
        {
            if (bIsPed)
            {
                if (IsPedCheck(pTable))
                {
                    // je clear; jmp next in the original: the shared tail needs a goto (duplicated
                    // bodies, early continue/return and a combined condition all change the code)
                    goto clear;
                }
                continue;
            }
            if (!IsCarCheck(pTable))
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
void ScriptManager::RecordWeaponHit(s32 entity_id, s32 projectile_model, bool bIsPed)
{
    WeaponCheckTable* pTable = &mWeaponCheckTable[0];

    s8 projectile_type = GetDeathCauseForObjectModel(projectile_model);

    for (u8 i = 0; i < 15; pTable++, i++)
    {
        if (pTable->mEntityId == entity_id)
        {
            if (pTable->mWeaponIdx == ped_death_cause::any_23 || pTable->mWeaponIdx == projectile_type)
            {
                if (bIsPed)
                {
                    if (IsPedCheck(pTable))
                    {
                        // je set; jmp next in the original: the shared tail needs a goto (duplicated
                        // bodies, early continue/return and a combined condition all change the code)
                        goto set;
                    }
                    return;
                }
                if (!IsCarCheck(pTable))
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
bool ScriptManager::IsWeaponHitRecorded(s32 id, char_type weapon_idx, bool bIsPed) const
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
                    if (IsPedCheck(pTable))
                    {
                        return IsCheckHit(pTable);
                    }
                }
                else
                {
                    if (IsCarCheck(pTable))
                    {
                        return IsCheckHit(pTable);
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
    if (!bSkip_mission)
    {
        memset(mScriptData, 0, sizeof(mScriptData));
        if (!miss2_0x11C_Pool)
        {
            miss2_0x11C_Pool = new ScriptThreadPool();
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
    memset(&gGameSave, 0, sizeof(gGameSave));

    MissionCleanupList* pCleanupList = new MissionCleanupList();
    gMiss2_25C = pCleanupList;
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

    if (gDo_miss_logging)
    {
        gMiss2Log.Open("test\\MISS_LOG.TXT", 1);
    }

    memset(mScrFileName, 0, sizeof(mScrFileName));
    memset(mPhoneIds, 0, sizeof(mPhoneIds));
    memset(mSavedGangRespect, 0, sizeof(mSavedGangRespect));
    memset(mBasicKf, 0, sizeof(mBasicKf));

    gStoredCar = NULL;
    gStoredCarId = 0;
}

MATCH_FUNC(0x5130e0)
ScriptManager::~ScriptManager()
{
    if (miss2_0x11C_Pool)
    {
        GTA2_DELETE_AND_NULL(miss2_0x11C_Pool);
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

    memset(&gGameSave, 0, sizeof(gGameSave));

    if (gMiss2_25C)
    {
        GTA2_DELETE_AND_NULL(gMiss2_25C);
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
    gStoredCarId = 0;
    gStoredCar = NULL;
}