#include "ScriptManager.hpp"
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
#include "thread_trigger.hpp"
#include "Ped.hpp"
#include "PedGroup.hpp"
#include "PublicTransport.hpp"

DEFINE_GLOBAL(ScriptManager*, gScriptManager_6F8060, 0x6F8060);
DEFINE_GLOBAL(SaveData, gGameSave_6F78C8, 0x6F78C8);
DEFINE_GLOBAL(s32, gStoredCarId_6F78B4, 0x6F78B4);
DEFINE_GLOBAL_INIT(u8, gbAllowMiss2Logging_6212EC, 1, 0x6212EC);

EXTERN_GLOBAL(SCR_POINTER*, gBasePtr_6F8070);

// Internal linkage as in the original, where all users were in one file: VC6 allocates registers differently for it.
DEFINE_STATIC_GLOBAL(Car*, gStoredCar_6F7560, 0x6F7560);

MATCH_FUNC(0x502dc0)
void MissionCleanupList::MissionCleanUp_502DC0()
{
    MissionCleanupEntry* pIter = &field_0_entries[0];
    PedGroup* pGroup = gGame_0x40_67E008->field_38_local_player->field_2C4_player_ped->field_164_ped_group;
    if (pGroup)
    {
        pGroup->DestroyGroup_4C93A0();
    }

    if (gStoredCar_6F7560)
    {
        if (gStoredCar_6F7560->field_6C_car_id == gStoredCarId_6F78B4)
        {
            gGame_0x40_67E008->field_38_local_player->RemoveCarFromHistory_564680(gStoredCar_6F7560);
            gStoredCar_6F7560->SetDriverPedType_421560(ped_type::dummy_3);
            if (!gStoredCar_6F7560->IsDeactivated_4214D0())
            {
                gStoredCar_6F7560->MarkForDespawn_421470();
            }
            gStoredCar_6F7560 = NULL;
            gStoredCarId_6F78B4 = 0;
        }
    }

    for (s32 i = 0; i < GTA2_COUNTOF(field_0_entries); i++)
    {
        if (reinterpret_cast<u32*>(pIter->field_0_car) != NULL)
        {
            switch (pIter->field_4_type)
            {
                case cleanup_entry_type::object_2:
                    if (pIter->field_0_obj->field_14_id == pIter->field_8_entity_id)
                    {
                        pIter->field_0_obj->Dealloc_5291B0();
                        pIter->field_0_obj->RequestRemovalWhenOffScreen_5290B0();
                    }
                    break;
                case cleanup_entry_type::ped_3:
                    if (pIter->field_0_ped->field_200_id == static_cast<u32>(pIter->field_8_entity_id))
                    {
                        PedGroup* pPedGroup = pIter->field_0_ped->field_164_ped_group;
                        if (pPedGroup)
                        {
                            pPedGroup->DisbandGroup_4C92A0();
                        }
                        pIter->field_0_ped->ForceDoNothing_462590();
                        pIter->field_0_ped->field_21C_bf.bScheduledForRemoval = true;
                    }
                    break;
                case cleanup_entry_type::car_1:
                    if (pIter->field_0_car->field_6C_car_id == pIter->field_8_entity_id)
                    {
                        gGame_0x40_67E008->field_38_local_player->RemoveCarFromHistory_564680(pIter->field_0_car);
                        pIter->field_0_car->SetDriverPedType_421560(ped_type::dummy_3);
                        if (!pIter->field_0_car->IsDeactivated_4214D0())
                        {
                            pIter->field_0_car->MarkForDespawn_421470();
                        }
                    }
                    break;
                default:
                    break;
            }
            pIter->field_0_car = NULL;
            pIter->field_8_entity_id = 0;
            pIter->field_4_type = cleanup_entry_type::empty_0;
            --field_258_count;
        }
        pIter++;
    }
    gScriptManager_6F8060->field_C1E70_wanted_car_model = car_model_enum::none;
}


MATCH_FUNC(0x503080)
str_table_entry* ScriptManager::FindStringById_503080(s16 stringId) const
{
    u16 idx = 0;
    str_table_entry* result = field_13350_pStringTbl->field_4_entries[idx];
    while (result)
    {
        if (result->field_0_str_id == stringId)
        {
            return result;
        }
        idx++;
        result = field_13350_pStringTbl->field_4_entries[idx];
    }
    return NULL;
}

MATCH_FUNC(0x5030b0)
str_table_entry* ScriptManager::StrEntryByString_5030B0(const char_type* strToFind) const
{
    u16 idx = 0;
    str_table_entry* pEntry = field_13350_pStringTbl->field_4_entries[idx];
    while (pEntry)
    {
        if (_strnicmp(strToFind, pEntry->get_name(), pEntry->field_8_length) == 0)
        {
            return pEntry;
        }
        idx++;
        pEntry = field_13350_pStringTbl->field_4_entries[idx];
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
    SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(gBasePtr_6F8070->field_8_index));
    SCR_POINTER* pParam2 = static_cast<SCR_POINTER*>(gScriptManager_6F8060->GetBasePointer_512770(pCmd->field_A_unsigned_2));
    Ped* pChar = pPointer->field_8_char;

    if (pChar != NULL)
    {
        if (pChar->has_car_403B80())
        {
            pParam2->field_8_car = pChar->get_car_416B60();

            if (gStoredCar_6F7560 != NULL)
            {
                if (gStoredCar_6F7560 != pParam2->field_8_car && gStoredCar_6F7560->field_6C_car_id != pParam2->field_8_car->field_6C_car_id)
                {
                    if (gStoredCar_6F7560->field_6C_car_id == gStoredCarId_6F78B4)
                    {
                        if (gCarManager_6C_677930->CanAllocateCarOfKind_446930(car_kind::recycled_1))
                        {
                            gStoredCar_6F7560->ReassignAllocatedCarType_443EE0(car_kind::recycled_1);
                            gGame_0x40_67E008->field_38_local_player->RemoveCarFromHistory_564680(gStoredCar_6F7560);
                            gStoredCar_6F7560->SetDriverPedType_421560(ped_type::dummy_3);

                            if (!gStoredCar_6F7560->IsDeactivated_4214D0())
                            {
                                gStoredCar_6F7560->MarkForDespawn_421470();
                            }
                        }
                        else if (gCarManager_6C_677930->CanAllocateCarOfKindWithSpare_446870(car_kind::mission_car_8))
                        {
                            gGame_0x40_67E008->field_38_local_player->RemoveCarFromHistory_564680(gStoredCar_6F7560);
                            gStoredCar_6F7560->SetDriverPedType_421560(ped_type::dummy_3);

                            if (!gStoredCar_6F7560->IsDeactivated_4214D0())
                            {
                                gStoredCar_6F7560->MarkForDespawn_421470();
                            }
                        }
                        else
                        {
                            gGame_0x40_67E008->field_38_local_player->RemoveCarFromHistory_564680(gStoredCar_6F7560);
                            gStoredCar_6F7560->SetDriverPedType_421560(ped_type::dummy_3);

                            gStoredCar_6F7560->DespawnSoon_421490();
                        }
                    }

                    gStoredCar_6F7560 = pParam2->field_8_car;
                    gStoredCarId_6F78B4 = pParam2->field_8_car->field_6C_car_id;

                    gStoredCar_6F7560->ReassignAllocatedCarType_443EE0(car_kind::mission_car_8);

                    if (!gPublicTransport_181C_6FF1D4->is_bus_579AA0(pParam2->field_8_car))
                    {
                        pParam2->field_8_car->LockoutThiefUnlessPermanent_475C40();
                    }
                }
            }
            else
            {
                gStoredCar_6F7560 = pParam2->field_8_car;
                gStoredCarId_6F78B4 = pParam2->field_8_car->field_6C_car_id;
                gStoredCar_6F7560->ReassignAllocatedCarType_443EE0(car_kind::mission_car_8);
            }
        }
    }

    ScriptThread::Next_503620(gBasePtr_6F8070);
}

MATCH_FUNC(0x511A70)
void ScriptManager::SetGeneratorTypeForCar_511A70(s32 car_model, Generator* pGen)
{
    u8* pList = reinterpret_cast<u8*>(gScriptManager_6F8060->field_340_car_list);
    u8 i;
    for (i = 0; i < 19; pList++, i++)
    {
        if (car_model == *pList)
        {
            pGen->field_0_gen_type = kDecidePowerupGenTypes_6212F0[i];
            if (kDecidePowerupGenTypes_6212F0[i] < 0x5B)
            {
                pGen->Activate_4C1A70();
                pGen->field_1E_spawns_left = 3;
            }
            else
            {
                pGen->Activate_4C1A70();
                pGen->field_1E_spawns_left = 1;
            }
            break;
        }
    }

    if (i == 19)
    {
        pGen->Activate_4C1A70();
        pGen->field_1E_spawns_left = 3;
        pGen->field_0_gen_type = 0x41;
    }
}

MATCH_FUNC(0x511b10)
char_type ScriptManager::AddCranePowerupTrigger_511B10(s16 idx)
{
    SCR_DECLARE_CRANE_POWERUP* pCmd = static_cast<SCR_DECLARE_CRANE_POWERUP*>(GetBasePointer_512770(idx));
    SCR_POINTER* pGenerator = static_cast<SCR_POINTER*>(GetBasePointer_512770(pCmd->field_A_generator));
    return gCarThreadTriggers_214_705F20->AddThreadTrigger_5C86C0(thread_trigger_type::any_car_1,
                                       3,
                                       reinterpret_cast<SCR_THREAD*>(pGenerator->field_8_generator),
                                       Fix16(pCmd->field_C_pos.field_0_x) + kFpHalf_6F75F0,
                                       Fix16(pCmd->field_C_pos.field_1_y) + kFpHalf_6F75F0,
                                       Fix16(pCmd->field_C_pos.field_2_z),
                                       kFpHalf_6F75F0,
                                       kFpHalf_6F75F0);
}

MATCH_FUNC(0x511b90)
void ScriptManager::SaveScriptCounters_511B90()
{
    u16 saved_counter_count = 0;
    for (u16 ptr = 0; ptr < GTA2_COUNTOF(field_46C_base_pointers); ptr++)
    {
        if (field_46C_base_pointers[ptr])
        {
            SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(GetBasePointer_512770(ptr));
            if (pPointer->field_2_type == SCRCMD_COUNTER_SAVE || pPointer->field_2_type == SCRCMD_COUNTER_SET_SAVE)
            {
                gGameSave_6F78C8.field_E4_car_and_script_data.field_50_script_counter[saved_counter_count].field_0_pointer =
                    pPointer->field_0_cmd_this;
                gGameSave_6F78C8.field_E4_car_and_script_data.field_50_script_counter[saved_counter_count].field_2_saved_value =
                    pPointer->field_8_counter;
                saved_counter_count++;
            }
        }
    }

    gGameSave_6F78C8.field_E4_car_and_script_data.field_46_script_ptr_count = saved_counter_count;

    if (saved_counter_count < 300)
    {
        memset(&gGameSave_6F78C8.field_E4_car_and_script_data.field_50_script_counter[saved_counter_count],
               0,
               (300 - saved_counter_count) * sizeof(SavedScriptCounter));
    }
}

MATCH_FUNC(0x511c30)
void ScriptManager::LoadScriptCounters_511C30()
{
    for (u16 saved_counter_idx = 0; saved_counter_idx < 300; saved_counter_idx++)
    {
        SavedScriptCounter* pSaved = &gGameSave_6F78C8.field_E4_car_and_script_data.field_50_script_counter[saved_counter_idx];
        if (pSaved->field_0_pointer > 0)
        {
            SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(GetBasePointer_512770(pSaved->field_0_pointer));
            pPointer->field_8_counter = pSaved->field_2_saved_value;
        }
    }
}

MATCH_FUNC(0x511C60)
void ScriptManager::RestoreAnsweredPhones_511C60()
{
    for (char_type bit = 0; bit < 32; bit++)
    {
        if (((1 << bit) & gGameSave_6F78C8.field_E4_car_and_script_data.field_48_flags) != 0)
        {
            SCR_POINTER* pPointer = static_cast<SCR_POINTER*>(GetBasePointer_512770(field_C1E32_phone_ids[bit]));
            pPointer->field_8_obj->PoolGiveAndQueueObjectType_5291E0(174);
            gScriptManager_6F8060->RemovePhoneThreadByObjId_512AA0(pPointer->field_8_obj->field_14_id);
        }
    }
}

MATCH_FUNC(0x511d40)
void ScriptManager::SaveMapInfo_511D40()
{
    strcpy(gGameSave_6F78C8.field_0_map_name, gGameSession_67E8E0.GetMapName_4C5940());
    strcpy(gGameSave_6F78C8.field_19_style_name, gGameSession_67E8E0.GetStyleName_4C5950());
    strcpy(gGameSave_6F78C8.field_32_script_name, gGameSession_67E8E0.GetScriptName_4C5960());
    gGameSave_6F78C8.field_0_map_name[24] = '\n';
    gGameSave_6F78C8.field_19_style_name[24] = '\n';
    gGameSave_6F78C8.field_32_script_name[24] = '\n';
    gGameSave_6F78C8.field_4B_main_stage = gGameSession_67E8E0.GetMainStageIdx_4C5980();
    gGameSave_6F78C8.field_4C_lhv = gGameSession_67E8E0.GetStage_4C5990();
    gGameSave_6F78C8.field_4D_bonus_stage = gGameSession_67E8E0.IsBonusStage_4C59A0();
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

    gGame_0x40_67E008->field_38_local_player->CopyPlayerDataToSave_56A1A0(&gGameSave_6F78C8.field_54_player_and_world_stats);

    gObject_5C_6F8F84->SaveObjects_52A500(&gGameSave_6F78C8.field_5E4_object_data);

    memcpy(gGameSave_6F78C8.field_5E4_object_data.field_12C_obj_5C_bUnCollectedTokens,
           &gObject_5C_6F8F84->field_20_uncollected_tokens,
           50u);

    gGameSave_6F78C8.field_5E4_object_data.field_160_secret_tokens_collected = gGameSession_67E8E0.get_secret_tokens_collected_453A80();

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

    gObject_5C_6F8F84->RestoreObjects_52A590(&gGameSave_6F78C8.field_5E4_object_data);

    memcpy(&gObject_5C_6F8F84->field_20_uncollected_tokens,
           gGameSave_6F78C8.field_5E4_object_data.field_12C_obj_5C_bUnCollectedTokens,
           sizeof(gObject_5C_6F8F84->field_20_uncollected_tokens));

    gGameSession_67E8E0.set_secret_tokens_collected_476B10(gGameSave_6F78C8.field_5E4_object_data.field_160_secret_tokens_collected);
    field_C1E2C_bLoadedFromSave = true;
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
    for (u16 idx = 0; idx < GTA2_COUNTOF_S(field_46C_base_pointers) - startOff; ++idx)
    {
        SCR_CMD_HEADER* pHeader = ScriptManager::GetBasePointer_512770(idx + startOff);
        if (pHeader)
        {
            if (pHeader->field_2_type == toFind)
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
        pThread->InitThread_511930(0, pLevelStart->field_0_cmd_this);
        gGame_0x40_67E008->IterateFirstPlayer_4B9CD0();
    }
}

// https://decomp.me/scratch/W4gXh
// See docs/match_attempts.md
MATCH_FUNC(0x5121E0)
void ScriptManager::LoadStringTbl_5121E0(u16 tableSize)
{
    u32 total_str_length = 0;
    u8* pStringDataIter1 = reinterpret_cast<u8*>(field_1334C_strings);
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

    field_13350_pStringTbl = reinterpret_cast<str_table_normalized*>(Memory::malloc_4FE4D0(sizeof(str_table_normalized)));
    memset(field_13350_pStringTbl, 0, sizeof(str_table_normalized));

    str_table_entry* pStringDataIter2 = field_1334C_strings;
    s32 str_count = 0;
    u32 total_str_length_ = 0;
    while (total_str_length_ < tableSize)
    {
        pStringDataIter2->field_2_zone_idx = gMap_0x370_6F6268->FindZoneIdxByName_4DF050(pStringDataIter2->get_name(),
                                                                                        static_cast<u8>(strlen(pStringDataIter2->get_name())));
        field_13350_pStringTbl->field_4_entries[str_count] = pStringDataIter2;
        s32 str_length_ = pStringDataIter2->field_8_length + 9;
        total_str_length_ += str_length_;
        pStringDataIter2 = reinterpret_cast<str_table_entry*>(reinterpret_cast<char_type*>(pStringDataIter2) + str_length_);
        ++str_count;
    }
    field_13350_pStringTbl->field_0_string_count = str_count;
}

MATCH_FUNC(0x5122D0)
void ScriptManager::GetScrFileName_5122D0()
{
    const char_type* pSlashPos = strrchr(field_35C_full_scr_file_name, '\\');
    if (pSlashPos)
    {
        pSlashPos++;

        memset(field_45C_scr_file_name, 0, sizeof(field_45C_scr_file_name));

        u32 lenAfterSlash = strlen(pSlashPos);
        while (lenAfterSlash)
        {
            if (pSlashPos[lenAfterSlash] == '.')
            {
                strncpy(field_45C_scr_file_name, pSlashPos, lenAfterSlash);
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
        strcpy(field_35C_full_scr_file_name, pScrName);
        Error_SetName_4A0770(field_35C_full_scr_file_name);
        File::Global_Open_4A7060(field_35C_full_scr_file_name);
        u32 len = sizeof(field_46C_base_pointers);
        File::Global_Read_4A71C0(field_46C_base_pointers, len);
        len = sizeof(field_334C_script_data);
        File::Global_Read_4A71C0(field_334C_script_data, len);
        u16 tableSize;
        len = sizeof(tableSize);
        File::Global_Read_4A71C0(&tableSize, len);
        len = 5118;
        File::Global_ReadRemainder_4A7250(field_1334C_strings, &len);
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
    if (gScriptManager_6F8060->field_2F4_bPreloadMissionScripts == 0)
    {
        u32 remainder;
        strcpy(gTmpBuffer_67C598, "data\\");
        strcat(gTmpBuffer_67C598, pScriptName);
        Error_SetName_4A0770(gTmpBuffer_67C598);
        File::Global_Open_4A7060(gTmpBuffer_67C598);
        File::Global_Read_4A71C0(&base_ptr_idx, 2);
        File::Global_Read_4A71C0(pStartCmd, 2);
        File::Global_Read_4A71C0(&remainder, 4);
        File::Global_Read_4A71C0(&field_46C_base_pointers[base_ptr_idx], 0xC00);
        File::Global_ReadRemainder_4A7250(&field_334C_script_data[field_46C_base_pointers[base_ptr_idx]], &remainder);
        File::Global_Close_4A70C0();
    }
    else
    {
        str_table_entry* pStrEntry = gScriptManager_6F8060->StrEntryByString_5030B0(pScriptName);
        u16 mission_idx = pStrEntry->field_2_zone_idx;
        base_ptr_idx = gScriptManager_6F8060->field_C1D72_mission_base_ptr_idx[mission_idx];
        *pStartCmd = gScriptManager_6F8060->field_C1D34_mission_start_cmd[mission_idx];
        memcpy(&gScriptManager_6F8060->field_46C_base_pointers[base_ptr_idx],
               &gScriptManager_6F8060->field_AA934_mission_base_pointers[3072 * mission_idx],
               0xC00u);
        memcpy(&gScriptManager_6F8060->field_334C_script_data[gScriptManager_6F8060->field_46C_base_pointers[base_ptr_idx]],
               &gScriptManager_6F8060->field_13354_mission_script_data[20000 * pStrEntry->field_2_zone_idx],
               gScriptManager_6F8060->field_C1DB0_mission_script_data_len[pStrEntry->field_2_zone_idx]);
    }
    return base_ptr_idx;
}

MATCH_FUNC(0x5125F0)
void ScriptManager::LoadSubScripts_5125F0()
{
    u32 data_len;
    u16 value;
    u16 mission_idx = 0;
    for (u16 i = 0; i < field_13350_pStringTbl->field_0_string_count; i++)
    {
        str_table_entry* pEntry = field_13350_pStringTbl->field_4_entries[i];
        if (static_cast<u8>(pEntry->field_4_type) == 21)
        {
            size_t length = strlen(pEntry->get_name());

            reinterpret_cast<char_type*>(&pEntry->field_6_padding)[length] = 'S';
            reinterpret_cast<char_type*>(&pEntry->field_6_padding)[length + 1] = 'C';
            reinterpret_cast<char_type*>(&pEntry->field_6_padding)[length + 2] = 'R';

            if (field_2F4_bPreloadMissionScripts == 1)
            {
                sprintf(gTmpBuffer_67C598, "data\\%s\\", field_45C_scr_file_name);
                strncat(gTmpBuffer_67C598, pEntry->get_name(), pEntry->field_8_length);
                File::Global_Open_4A7060(gTmpBuffer_67C598);

                File::Global_Read_4A71C0(&value, 2);
                field_C1D72_mission_base_ptr_idx[mission_idx] = value;
                File::Global_Read_4A71C0(&value, 2);
                field_C1D34_mission_start_cmd[mission_idx] = value;
                File::Global_Read_4A71C0(&data_len, 4);
                field_C1DB0_mission_script_data_len[mission_idx] = data_len;

                File::Global_Read_4A71C0(&field_AA934_mission_base_pointers[3072 * mission_idx], 3072);
                File::Global_ReadRemainder_4A7250(&field_13354_mission_script_data[20000 * mission_idx], &field_C1DB0_mission_script_data_len[mission_idx]);

                File::Global_Close_4A70C0();

                pEntry->field_2_zone_idx = mission_idx;
                mission_idx++;
            }
        }
    }
}

MATCH_FUNC(0x512770)
SCR_CMD_HEADER* ScriptManager::GetBasePointer_512770(u16 idx)
{
    if (!field_46C_base_pointers[idx])
    {
        return NULL;
    }
    return reinterpret_cast<SCR_CMD_HEADER*>(&field_334C_script_data[field_46C_base_pointers[idx]]);
}

MATCH_FUNC(0x5127a0)
void ScriptManager::ExecuteScriptThreads_5127A0()
{
    if (!bSkip_mission_67D4E5)
    {
        if (gbAllowMiss2Logging_6212EC)
        {
            if (gDo_miss_logging_67D6BC)
            {
                miss2_0x11C_Pool_6F8064->LogAll_47F4D0();
                gMiss2Log_6F7698.Write_Log_4D9650("\n");
            }
        }
        miss2_0x11C_Pool_6F8064->field_0_pool.UpdatePoolNoDeallocate();
    }
    gCarManager_6C_677930->field_5C_model_check_destroyed = false;
}

MATCH_FUNC(0x5128a0)
ThreadTrigger* ScriptManager::FindCarThread_5128A0(s32 ped_id, s32 car_id)
{
    ThreadTrigger* pTrigger = &field_4_car_triggers[0];
    for (u16 thread_idx = 0; thread_idx < GTA2_COUNTOF(field_4_car_triggers); thread_idx++)
    {
        if (pTrigger->field_0_ped_id == ped_id && pTrigger->field_4_target_id == car_id)
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
    ThreadTrigger* pTrigger = &field_4_car_triggers[0];
    for (u16 i = 0; i < GTA2_COUNTOF(field_4_car_triggers); i++)
    {
        if (pTrigger->field_8_cmd_line == 0)
        {
            break;
        }
        pTrigger++;
    }

    pTrigger->field_0_ped_id = ped_id;
    pTrigger->field_4_target_id = car_id;
    pTrigger->field_8_cmd_line = cmd_line;
    ++field_0_car_trigger_count;
}

MATCH_FUNC(0x512910)
bool ScriptManager::TriggerCarThread_512910(s32 ped_id, s32 car_id)
{
    ThreadTrigger* pTrigger = ScriptManager::FindCarThread_5128A0(ped_id, car_id);
    if (pTrigger)
    {
        SCR_THREAD* pThreadCmd = static_cast<SCR_THREAD*>(GetBasePointer_512770(pTrigger->field_8_cmd_line));
        pThreadCmd->field_8_script_thread = SpawnThread_5120C0(pThreadCmd->field_E_thread_start_cmd, 0);

        if (pThreadCmd->field_8_script_thread)
        {
            pThreadCmd->field_C_thread_id = pThreadCmd->field_8_script_thread->field_11A_thread_id;
            pTrigger->field_0_ped_id = 0;
            pTrigger->field_4_target_id = 0;
            pTrigger->field_8_cmd_line = 0;
            --field_0_car_trigger_count;
            return true;
        }
    }
    return false;
}

MATCH_FUNC(0x512980)
ThreadTrigger* ScriptManager::FindPhoneThread_512980(s32 ped_id, s32 obj_id)
{
    ThreadTrigger* pTrigger = &field_188_phone_triggers[0];
    for (u16 thread_idx = 0; thread_idx < GTA2_COUNTOF(field_188_phone_triggers); thread_idx++)
    {
        if (pTrigger->field_0_ped_id == ped_id && pTrigger->field_4_target_id == obj_id)
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
    ThreadTrigger* pTrigger = &field_188_phone_triggers[0];
    for (u16 i = 0; i < GTA2_COUNTOF(field_188_phone_triggers) && pTrigger->field_8_cmd_line != 0; i++)
    {
        pTrigger++;
    }
    pTrigger->field_0_ped_id = ped_id;
    pTrigger->field_4_target_id = obj_id;
    pTrigger->field_8_cmd_line = cmd_line;
    field_184_phone_trigger_count++;
}

MATCH_FUNC(0x5129f0)
bool ScriptManager::AnswerPhone_5129F0(s32 ped_id, s32 obj_id)
{
    ThreadTrigger* pTrigger = ScriptManager::FindPhoneThread_512980(ped_id, obj_id);
    if (pTrigger)
    {
        SCR_THREAD* pThreadCmd = static_cast<SCR_THREAD*>(GetBasePointer_512770(pTrigger->field_8_cmd_line));
        SCR_ANSWER_PHONE* pTriggerCmd;

        switch (pThreadCmd->field_2_type)
        {
            case SCRCMD_ANSWER_PHONE:
                pTriggerCmd = reinterpret_cast<SCR_ANSWER_PHONE*>(pThreadCmd);
                pTriggerCmd->field_12_answered = 1;
                return true;
            case SCRCMD_THREAD_DECLARE4:
                pThreadCmd->field_8_script_thread = SpawnThread_5120C0(pThreadCmd->field_E_thread_start_cmd, 0);
                if (pThreadCmd->field_8_script_thread)
                {
                    pThreadCmd->field_C_thread_id = pThreadCmd->field_8_script_thread->field_11A_thread_id;
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
        pTrigger->field_0_ped_id = 0;
        pTrigger->field_4_target_id = 0;
        pTrigger->field_8_cmd_line = 0;
        --field_184_phone_trigger_count;
    }
    return pTrigger;
}

MATCH_FUNC(0x512aa0)
ThreadTrigger* ScriptManager::RemovePhoneThreadByObjId_512AA0(s32 obj_id)
{
    ThreadTrigger* pTrigger = ScriptManager::FindPhoneThreadByObjId_512AD0(obj_id);
    if (pTrigger)
    {
        pTrigger = ScriptManager::RemovePhoneThread_512A70(pTrigger->field_0_ped_id, obj_id);
    }
    return pTrigger;
}

MATCH_FUNC(0x512ad0)
ThreadTrigger* ScriptManager::FindPhoneThreadByObjId_512AD0(s32 obj_id)
{
    ThreadTrigger* pTrigger = &field_188_phone_triggers[0];
    for (u16 thread_idx = 0; thread_idx < GTA2_COUNTOF(field_188_phone_triggers); thread_idx++)
    {
        if (pTrigger->field_4_target_id == obj_id)
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
    WeaponCheckTable* pTable = &field_27C_weapon_check_table[0];

    for (u8 i = 0; i < 15; pTable++, i++)
    {
        if (pTable->field_0_entity_id == 0 && pTable->field_4_weapon_idx == 0 && !pFree)
        {
            pFree = pTable;
        }

        if (pTable->field_0_entity_id == id)
        {
            if (pTable->field_4_weapon_idx == weapon_idx || pTable->field_4_weapon_idx == weapon_type::weapon_0x17)
            {
                if (bIsPed)
                {
                    if (HasCheckFlag4_4759C0(pTable))
                    {
                        return true;
                    }
                }
                else
                {
                    if (HasCheckFlag2_4759A0(pTable))
                    {
                        return true;
                    }
                }
            }
        }
    }

    if (pFree)
    {
        pFree->field_0_entity_id = id;
        pFree->field_4_weapon_idx = weapon_idx;
        if (bIsPed)
        {
            pFree->field_6_flags = 4;
        }
        else
        {
            pFree->field_6_flags = 2;
        }
        field_278_weapon_check_count++;
    }
    return false;
}

MATCH_FUNC(0x512ba0)
void ScriptManager::ClearWeaponHitFlag_512BA0(s32 id, bool bIsPed)
{
    WeaponCheckTable* pTable = &field_27C_weapon_check_table[0];

    for (u8 i = 0; i < 15; pTable++, i++)
    {
        if (pTable->field_0_entity_id == id)
        {
            if (bIsPed)
            {
                if (HasCheckFlag4_4759C0(pTable))
                {
                    // je clear; jmp next in the original: the shared tail needs a goto (duplicated
                    // bodies, early continue/return and a combined condition all change the code)
                    goto clear;
                }
                continue;
            }
            if (!HasCheckFlag2_4759A0(pTable))
            {
                continue;
            }
        clear:
            pTable->field_0_entity_id = 0;
            pTable->field_4_weapon_idx = 0;
            pTable->field_5_hit_projectile_type = 0;
            pTable->field_6_flags = 0;
            field_278_weapon_check_count--;
        }
    }
}

// https://decomp.me/scratch/r6LhX
MATCH_FUNC(0x512c00)
void ScriptManager::RecordWeaponHit_512C00(s32 entity_id, s32 projectile_model, bool bIsPed)
{
    WeaponCheckTable* pTable = &field_27C_weapon_check_table[0];

    s8 projectile_type = GetDeathCauseForObjectModel_48E780(projectile_model);

    for (u8 i = 0; i < 15; pTable++, i++)
    {
        if (pTable->field_0_entity_id == entity_id)
        {
            if (pTable->field_4_weapon_idx == weapon_type::weapon_0x17 || pTable->field_4_weapon_idx == projectile_type)
            {
                if (bIsPed)
                {
                    if (HasCheckFlag4_4759C0(pTable))
                    {
                        // je set; jmp next in the original: the shared tail needs a goto (duplicated
                        // bodies, early continue/return and a combined condition all change the code)
                        goto set;
                    }
                    return;
                }
                if (!HasCheckFlag2_4759A0(pTable))
                {
                    return;
                }
            set:
                pTable->field_6_flags |= 1;
                pTable->field_5_hit_projectile_type = projectile_type;
                return;
            }
        }
    }
}

MATCH_FUNC(0x512c70)
bool ScriptManager::IsWeaponHitRecorded_512C70(s32 id, char_type weapon_idx, bool bIsPed) const
{
    const WeaponCheckTable* pTable = &field_27C_weapon_check_table[0];

    for (u8 i = 0; i < 15; pTable++, i++)
    {
        if (pTable->field_0_entity_id == id)
        {
            if (pTable->field_4_weapon_idx == weapon_idx || pTable->field_4_weapon_idx == weapon_type::weapon_0x17)
            {
                if (bIsPed)
                {
                    if (HasCheckFlag4_4759C0(pTable))
                    {
                        return HasCheckFlag1_475980(pTable);
                    }
                }
                else
                {
                    if (HasCheckFlag2_4759A0(pTable))
                    {
                        return HasCheckFlag1_475980(pTable);
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
    field_13350_pStringTbl = NULL;
    if (!bSkip_mission_67D4E5)
    {
        memset(field_334C_script_data, 0, sizeof(field_334C_script_data));
        if (!miss2_0x11C_Pool_6F8064)
        {
            miss2_0x11C_Pool_6F8064 = new ScriptThreadPool();
        }
    }
    field_1334C_strings = static_cast<str_table_entry*>(Memory::malloc_4FE4D0(0x1400u));
    memset(field_35C_full_scr_file_name, 0, sizeof(field_35C_full_scr_file_name));
    memset(field_334C_script_data, 0, sizeof(field_334C_script_data));
    memset(field_46C_base_pointers, 0, sizeof(field_46C_base_pointers));
    memset(field_1334C_strings, 0, 0x1400u);
    field_0_car_trigger_count = 0;
    memset(field_4_car_triggers, 0, sizeof(field_4_car_triggers));
    field_184_phone_trigger_count = 0;
    memset(field_188_phone_triggers, 0, sizeof(field_188_phone_triggers));
    field_278_weapon_check_count = 0;
    memset(field_27C_weapon_check_table, 0, sizeof(field_27C_weapon_check_table));
    memset(&gGameSave_6F78C8, 0, sizeof(gGameSave_6F78C8));

    MissionCleanupList* pCleanupList = new MissionCleanupList();
    gMiss2_25C_6F805C = pCleanupList;
    if (!pCleanupList)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\miss2.cpp", 13630);
    }

    memset(field_13354_mission_script_data, 0, sizeof(field_13354_mission_script_data));
    memset(field_AA934_mission_base_pointers, 0, sizeof(field_AA934_mission_base_pointers));
    memset(field_C1D34_mission_start_cmd, 0, sizeof(field_C1D34_mission_start_cmd));
    memset(field_C1D72_mission_base_ptr_idx, 0, sizeof(field_C1D72_mission_base_ptr_idx));
    memset(field_C1DB0_mission_script_data_len, 0, sizeof(field_C1DB0_mission_script_data_len));
    field_344_mission_flag = NULL;
    field_2F4_bPreloadMissionScripts = 1;
    field_C1E2C_bLoadedFromSave = false;
    field_310_finish_score = 0;
    field_314_total_missions = 0;
    field_318_total_secrets = 0;
    field_31C_gang_1_missions_total = 0;
    field_320_gang_2_missions_total = 0;
    field_324_gang_3_missions_total = 0;
    field_328_passed_flag = NULL;
    field_32C_1_passed_flag = NULL;
    field_330_2_passed_flag = NULL;
    field_334_3_passed_flag = NULL;
    field_338_secrets_passed = NULL;
    field_33C_secrets_failed = NULL;
    field_340_car_list = NULL;
    field_348_gang_1_mission_flag = NULL;
    field_34C_gang_2_mission_flag = NULL;
    field_350_gang_3_mission_flag = NULL;
    field_354_next_gang_idx = 0;
    field_C1E70_wanted_car_model = car_model_enum::none;
    field_356_gang_1_death_base = 0;
    field_358_gang_2_death_base = 0;
    field_35A_gang_3_death_base = 0;
    field_355_death_arr_state = 0;
    field_C1E2E_death_arrest_flag = false;
    field_C1E2D_bKillFrenzyActive = false;
    field_468_unused = 0;

    if (gDo_miss_logging_67D6BC)
    {
        gMiss2Log_6F7698.Open_4D9470("test\\MISS_LOG.TXT", 1);
    }

    memset(field_45C_scr_file_name, 0, sizeof(field_45C_scr_file_name));
    memset(field_C1E32_phone_ids, 0, sizeof(field_C1E32_phone_ids));
    memset(field_C1E2F_saved_gang_respect, 0, sizeof(field_C1E2F_saved_gang_respect));
    memset(field_C1E74_basic_kf, 0, sizeof(field_C1E74_basic_kf));

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

    if (field_13350_pStringTbl)
    {
        memset(field_13350_pStringTbl->field_4_entries, 0, sizeof(field_13350_pStringTbl->field_4_entries));
        free(field_13350_pStringTbl);
        field_13350_pStringTbl = NULL;
    }

    if (field_1334C_strings)
    {
        free(field_1334C_strings);
        field_1334C_strings = NULL;
    }

    memset(&gGameSave_6F78C8, 0, sizeof(gGameSave_6F78C8));

    if (gMiss2_25C_6F805C)
    {
        GTA2_DELETE_AND_NULL(gMiss2_25C_6F805C);
    }

    field_328_passed_flag = NULL;
    field_32C_1_passed_flag = NULL;
    field_330_2_passed_flag = NULL;
    field_334_3_passed_flag = NULL;
    field_340_car_list = NULL;
    field_338_secrets_passed = NULL;
    field_33C_secrets_failed = NULL;
    field_348_gang_1_mission_flag = NULL;
    field_34C_gang_2_mission_flag = NULL;
    field_350_gang_3_mission_flag = NULL;
    gStoredCarId_6F78B4 = 0;
    gStoredCar_6F7560 = NULL;
}