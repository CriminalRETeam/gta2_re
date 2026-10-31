#include "Game.hpp"
#include "ped_group_type.hpp"
#include "ped_class.hpp"
#include "AmbulanceService.hpp"
#include "InputRecorder_67F8B0.hpp"
#include "Camera.hpp"
#include "CarInfoTable.hpp"
#include "Car.hpp"
#include "Char_Pool.hpp"
#include "Cranes.hpp"
#include "Crushers.hpp"
#include "DoorPool.hpp"
#include "ExplodingScore_100.hpp"
#include "Firefighters.hpp"
#include "Fix16_Rect.hpp"
#include "ScriptStackFramePool.hpp"
#include "Frontend.hpp"
#include "Function.hpp"
#include "Gang.hpp"
#include "Garage.hpp"
#include "Generators.hpp"
#include "Globals.hpp"
#include "CarChaseTaskTable.hpp"
#include "Hud.hpp"
#include "EmergencyCrewPool.hpp"
#include "LightPool.hpp"
#include "MapRenderer.hpp"
#include "FrameProfiler.hpp"
#include "SpriteRenderer.hpp"
#include "Object_5C.hpp"
#include "PathFinder.hpp"
#include "ParticleManager.hpp"
#include "Ped.hpp"
#include "PedGroup.hpp"
#include "ObjectDefinitions.hpp"
#include "Player.hpp"
#include "Police.hpp"
#include "PublicTransport.hpp"
#include "SpriteGrid.hpp"
#include "RouteFinder.hpp"
#include "CollisionSoundQueue.hpp"
#include "CrimeReportQueue.hpp"
#include "TaxiList.hpp"
#include "TileAnimManager.hpp"
#include "TrafficLights.hpp"
#include "PedRefTable.hpp"
#include "WeaponManager.hpp"
#include "Explosion.hpp"
#include "char.hpp"
#include "collide.hpp"
#include "debug.hpp"
#include "error.hpp"
#include "NetUi.hpp"
#include "ScriptManager.hpp"
#include "gbh_graphics.hpp"
#include "gtx_0x106C.hpp"
#include "PlyDat.hpp"
#include "GameSession.hpp"
#include "KanjiFont.hpp"
#include "map_0x370.hpp"
#include "registry.hpp"
#include "rng.hpp"
#include "root_sound.hpp"
#include "FpsCounter.hpp"
#include "TextureCache.hpp"
#include "sprite.hpp"
#include "TextTable.hpp"
#include "winmain.hpp"

DEFINE_GLOBAL_INIT(Fix16, kFpFive_67DECC, Fix16(5), 0x67DECC);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpFive, kFpFive_67DECC, 0x67DFB4);
EXTERN_GLOBAL(NetworkGameSettings, gNetworkGameSettings_707098);
// Network game speed setting; selects the frame duration in ApplyNetworkGameSettings
namespace network_game_speed
{
enum
{
    slow_0 = 0, // 50 ms per frame
    normal_1 = 1, // 40 ms per frame
    fast_2 = 2, // 33 ms per frame
};
} // namespace network_game_speed

DEFINE_GLOBAL(s32, gCachedScreenDimension_706C58, 0x706C58);

EXTERN_GLOBAL(u32, counter_706C4C);
EXTERN_GLOBAL(char_type, gLighting_626A09);

DEFINE_GLOBAL(Game*, gGame_0x40_67E008, 0x67E008);

EXPORT void __stdcall ReapplySavedGamma();

MATCH_FUNC(0x4B8BB0)
s32 Game::GetFrameDurationMs() const
{
    if (bStartNetworkGame_7081F0 == false)
    {
        return !mFastForward ? 33 : 11;
    }
    return mNetworkFrameMs;
}

MATCH_FUNC(0x4B8BD0)
void Game::ExitGame(s32 new_timer, s32 exit_type, s8 bonus_type)
{
    s32 timer = mExitTimer;
    if (timer == -1 || timer > new_timer)
    {
        mExitTimer = new_timer;
        mGameExitType = exit_type;
        gGameSession_67E8E0.SetLevelFinishBonusType(bonus_type);
    }
}

MATCH_FUNC(0x4B8C00)
void Game::ExitGameNoBonus(s32 new_timer, s32 exit_type)
{
    ExitGame(new_timer, exit_type, 0);
}

MATCH_FUNC(0x4B8C20)
bool Game::UpdateExitTimer()
{
    if (mExitTimer == 0)
    {
        return true;
    }

    if (mExitTimer > 0)
    {
        mExitTimer--;
    }

    return false;
}

MATCH_FUNC(0x4B8C40)
void Game::LoadGameFiles()
{
    char_type gci_path[256];

    gText_0x14_704DFC->Load();
    if (bSkip_audio_67D6BE == false)
    {
        char_type* style_name = gGameSession_67E8E0.GetStyleName();
        gRoot_sound_66B038.LoadStyle(style_name);
    }

    char_type* style_name = gGameSession_67E8E0.GetStyleName();
    gGtx_0x106C_703DD4->LoadSty(style_name);

    char_type* map_name = gGameSession_67E8E0.GetMapName();
    gMap_0x370_6F6268->LoadMap(map_name);

    gObjectDefinitions_6FCF00->InitDefinitions();

    gTextureCache_15D8_705064->LoadStyleTextures();

    gGameSession_67E8E0.clear_secret_tokens_collected();

    char_type* script_name = gGameSession_67E8E0.GetScriptName();
    gScriptManager_6F8060->Load(script_name);

    if (strlen(gGameSession_67E8E0.GetSaveFileName_4C5970()) != 0)
    {
        char_type* debug_str = gGameSession_67E8E0.GetSaveFileName_4C5970();
        gScriptManager_6F8060->LoadSave(debug_str);
    }

    strcpy(gci_path, "data\\");
    memset(gTmpBuffer_67C598, 0, sizeof(gTmpBuffer_67C598));
    gRegistry_6FF968.Get_Debug_Setting("carname", reinterpret_cast<LPBYTE>(gTmpBuffer_67C598), 0x100);

    strcat(gci_path, gTmpBuffer_67C598);

    if (strcmp(gci_path, "data\\"))
    {
        gCarInfo_808_678098->LoadFromGciFile(gci_path);
    }
    else
    {
        gCarInfo_808_678098->LoadFromGciFile("data\\nyc.gci");
    }

    gCarManager_6C_677930->DistributeCarsByRating();

    if (gKanjiFont_6F5168 != NULL)
    {
        gKanjiFont_6F5168->InitGlyphCaches();
    }
}

MATCH_FUNC(0x4B8E00)
void Game::SetScreenSize(u32 width, u32 height)
{
    mLocalPlayer->mGameCamera.SetScreenSize(width, height);
    mLocalPlayer->mAuxGameCamera.SetScreenSize(width, height);
    mLocalPlayer->mViewCamera.SetScreenSize(width, height);
    gHud_2B00_706620->RecalcTextLayout();
}

MATCH_FUNC(0x4B8E50)
void Game::ApplyNetworkGameSettings()
{
    switch (gNetworkGameSettings_707098.mGameSpeed)
    {
        case network_game_speed::slow_0:
            mNetworkFrameMs = 50;
            break;

        case network_game_speed::normal_1:
            mNetworkFrameMs = 40;
            break;

        case network_game_speed::fast_2:
            mNetworkFrameMs = 33;
            break;
    }

    bSkip_police_67D4F9 = gNetworkGameSettings_707098.mPoliceOn == 0;
}

// Gives the player a group of count peds with the given remap
MATCH_FUNC(0x46DD70)
EXPORT void __stdcall SpawnTestGroup(char_type remap, u8 count)
{
    LOCAL_STATIC(Ped*, sLastTestPed, 0x6787E8);
    Ped* pPlayerPed = gGame_0x40_67E008->mLocalPlayer->GetPlayerPed_41D020();
    PedGroup* pGroup = PedGroup::New();
    pGroup->add_ped_leader(pPlayerPed);
    pGroup->SetCounts_433360(0);
    pGroup->mGroupType = ped_group_type::normal_2;

    sLastTestPed = gPedPool_6787B8->Allocate();
    sLastTestPed->set_occupation_403970(ped_ocupation_enum::bank_robber);
    sLastTestPed->set_remap_433B90(remap);
    sLastTestPed->mGraphicType = pPlayerPed->mGraphicType;
    sLastTestPed->SetPedType_403920(ped_type::script_created_5);
    if (!sLastTestPed->AllocCharB4(pPlayerPed->mCam.x, pPlayerPed->mCam.y, pPlayerPed->mCam.z))
    // 9.6f: Ped::get_cam_x/get_cam_y/get_cam_z (inlined, using them changes the code)
    {
        sLastTestPed->mGameObject->SetRemap(sLastTestPed->get_remap_433BA0());
    }
    sLastTestPed->set_health_4039A0(100);
    sLastTestPed->SetPedClass_433BC0(ped_class::gang_member_1);
    pGroup->add_ped_to_end_of_list(sLastTestPed);
    sLastTestPed->ForceWeapon(weapon_type::pistol);

    for (u8 i = 1; i < count; i++)
    {
        Ped* pPed = gPedPool_6787B8->Allocate();
        pPed->set_occupation_403970(ped_ocupation_enum::bank_robber);
        pPed->set_remap_433B90(remap);
        pPed->mGraphicType = pPlayerPed->mGraphicType;
        pPed->SetPedType_403920(ped_type::script_created_5);
        pPed->AllocCharB4(pPlayerPed->mCam.x, pPlayerPed->mCam.y, pPlayerPed->mCam.z);
        pPed->SetRemap_433C10(pPed->get_remap_433BA0());
        pPed->set_health_4039A0(100);
        pPed->SetPedClass_433BC0(ped_class::gang_member_1);
        pGroup->add_ped_to_end_of_list(pPed);
        pPed->ForceWeapon(weapon_type::pistol);
    }
}

MATCH_FUNC(0x46E370)
EXPORT void SpawnTestGroupDefault()
{
    SpawnTestGroup(4, 4);
}

MATCH_FUNC(0x5D8DF0)
EXPORT void InvalidateCachedScreenDimension()
{
    gCachedScreenDimension_706C58 = 0;
}

MATCH_FUNC(0x4B8EB0)
void Game::BootGame()
{

    gGameSession_67E8E0.SetBonusRatingTextIdx(0);
    if (bStartNetworkGame_7081F0)
    {
        ApplyNetworkGameSettings();
    }

    InvalidateCachedScreenDimension();
    ConvertColourBanks_5D7CB0();
    UpdateGameScreenSize();
    gSpriteManager_703820->AllocScratchSprite();
    gTileAnim_2_7052C4->Empty();
    gPublicTransport_181C_6FF1D4->SetupTrainAndBusStops();
    gObject_5C_6F8F84->CreateDiagonalWallObjects();
    PedGroup::ResetAllGroups();
    if (bDo_mike_67D5CC)
    {
        gMike_A80_6F7328->Init();
    }
    gCarThreadTriggers_214_705F20->Reset();
    gMap_0x370_6F6268->AllocZones();
    gHud_2B00_706620->Init();
    gScriptManager_6F8060->StartLevelThread(); // script
    gGame_0x40_67E008->mLocalPlayer->ApplyCheats();
    if (bDo_iain_test_67D4E9)
    {
        SpawnTestGroupDefault();
    }
    gRouteFinder_6FFDC8->Reset();
    gHud_2B00_706620->Init();
    gMap_0x370_6F6268->SpawnMapObjects(); // map objects
    gMap_0x370_6F6268->UpdateLights(); // lights
    if (!bSkip_traffic_lights_67D4EC)
    {
        gTrafficLights_194_705958->CreateFromMapZones();
    }
    gPublicTransport_181C_6FF1D4->SpawnTrainsFromStations(); // trains?

    for (s32 i = 0; i < mNumPlayers; i++)
    {
        mPlayers[i]->InitializePlayerState();
    }

    gPlyDat_6FEAC0->DoMuchCashCheat();
}

MATCH_FUNC(0x4B8FF0)
void Game::ShowCounters()
{
    swprintf(tmpBuff_67BD9C, L"recycled cars : %d", gCarManager_6C_677930->mRecycledCars);
    gHud_2B00_706620->mTexts.DisplayText(tmpBuff_67BD9C, 0, 32, gDebugFont_706600, 1);

    swprintf(tmpBuff_67BD9C, L"prot. recycled cars : %d", gCarManager_6C_677930->mProtoRecycledCars);
    gHud_2B00_706620->mTexts.DisplayText(tmpBuff_67BD9C, 0, 48, gDebugFont_706600, 1);

    swprintf(tmpBuff_67BD9C, L"mission cars : %d", gCarManager_6C_677930->mMissionCars);
    gHud_2B00_706620->mTexts.DisplayText(tmpBuff_67BD9C, 0, 64, gDebugFont_706600, 1);

    swprintf(tmpBuff_67BD9C, L"unit cars : %d", gCarManager_6C_677930->mUnitCars);
    gHud_2B00_706620->mTexts.DisplayText(tmpBuff_67BD9C, 0, 80, gDebugFont_706600, 1);

    swprintf(tmpBuff_67BD9C, L"cars:%d", gCar_BC_Pool_67792C->get_cars_count_45AD30());
    gHud_2B00_706620->mTexts.DisplayText(tmpBuff_67BD9C, 0, 96, gDebugFont_706600, 1);

    swprintf(tmpBuff_67BD9C, L"dummy_chars : %d", static_cast<u8>(gPedManager_6787BC->mNumDummyChars));
    gHud_2B00_706620->mTexts.DisplayText(tmpBuff_67BD9C, 0, 112, gDebugFont_706600, 1);

    Player* pLocalPlayer = gGame_0x40_67E008->mLocalPlayer;
    if (pLocalPlayer)
    {
        swprintf(tmpBuff_67BD9C, L"accuracy_count : %d", static_cast<u8>(pLocalPlayer->mScores.get_accuracy_count_45B0A0()));
        gHud_2B00_706620->mTexts.DisplayText(tmpBuff_67BD9C, 0, 128, gDebugFont_706600, 1);

        swprintf(tmpBuff_67BD9C, L"reverse_count : %d", pLocalPlayer->mScores.get_reverse_count_45B0B0());
        gHud_2B00_706620->mTexts.DisplayText(tmpBuff_67BD9C, 0, 144, gDebugFont_706600, 1);

        const u32 x = pLocalPlayer->mPlayerPed->get_cam_x().ToInt();
        const u32 y = pLocalPlayer->mPlayerPed->get_cam_y().ToInt();

        gmp_zone_info* pNavZone = gMap_0x370_6F6268->GetZoneInfoAt(x, y);

        swprintf(tmpBuff_67BD9C, L"density:%d", pNavZone->mCarDensity);
        gHud_2B00_706620->mTexts.DisplayText(tmpBuff_67BD9C, 0, 160, gDebugFont_706600, 1);
    }
}

MATCH_FUNC(0x4B9270)
void Game::ShowDebugOverlay()
{
    if (gDo_show_counter_67D6E6)
    {
        ShowCounters();
    }

    if (mFastForward)
    {
        Hud_TextEntry* pTextEntry = gHud_2B00_706620->mTexts.DisplayText(L"FF", 0, 440, gDebugFont_706600, 1);
        pTextEntry->SetFontRemap_45AFD0(6);
    }
}

MATCH_FUNC(0x4B92D0)
void Game::Draw()
{
    gViewCamera_676978 = &mDrawPlayer->mViewCamera;

    gpMapRenderer_6F66E4->ClearDrawnTileCount();
    gSpriteManager_703820->ResetDrawnSpriteCount();
    gSpriteRenderer_67B580->ResetAll();

    gSpriteGrid_3_679210->DrawSpritesClipped();
    gSpriteGrid_2_67920C->DrawSpritesClipped();
    gSpriteGrid_1_679208->DrawSpritesClipped();

    MakeScreenTableAndSetWindow();

    pgbh_BeginScene();

    gpMapRenderer_6F66E4->Draw(); // Note: Also seems to draw sprites

    if (!bExplodingScoresOff_67D4FB)
    {
        gExplodingScorePool->DrawExplodingScores();
    }

    gHud_2B00_706620->DrawGui(); // user

    if (bDo_mike_67D5CC)
    {
        gMike_A80_6F7328->DebugDrawProfiling();
    }

    pgbh_EndScene();
    FreeSurface();
}

MATCH_FUNC(0x4B9380)
void Game::UpdateAllPlayersControls()
{
    Player** ppPlayersIterator = mPlayers;
    u32 idx = 0;
    while (idx < mNumPlayers)
    {
        if ((*ppPlayersIterator)->mInUse)
        {
            (*ppPlayersIterator)->ControlInputs();
        }
        ++idx;
        ++ppPlayersIterator;
    }
}

MATCH_FUNC(0x4B93C0)
void Game::UpdatePaused()
{
    Player** pIter = mPlayers;
    u32 i = 0;
    while (i < mNumPlayers)
    {
        if ((*pIter)->mInUse)
        {
            (*pIter)->UpdatePaused();
        }
        ++i;
        ++pIter;
    }

    gHud_2B00_706620->UpdatePauseSection();

    if (counter_706C4C > 0)
    {
        ReapplySavedGamma();
    }
}

MATCH_FUNC(0x4B9410)
void Game::UpdateGame()
{
    Player** ppPlayerIter = mPlayers;
    u32 i = 0;
    while (i < mNumPlayers)
    {
        if ((*ppPlayerIter)->mInUse)
        {
            (*ppPlayerIter)->Service();
        }
        i++;
        ppPlayerIter++;
    }

    gCollisionSoundQueue_66AFE0->Reset();
    gCollisionCounters_6791FC->ResetCount();

    if (gLighting_626A09)
    {
        gLight_1D4CC_6F5520->Service_45C1E0();
    }

    gCranePool_D9C_679FD4->CranesService();
    gCrusherPool_94_67A830->CrushersService();
    gGeneratorPool_14AC_67E5D0->GeneratorsService();
    gPedManager_6787BC->PedsService();
    gPublicTransport_181C_6FF1D4->PublicTransportService(); // trains & buses
    gGarage_48_6FD26C->GaragesService();
    gCarManager_6C_677930->CarsService();

    if (bDo_mike_67D5CC)
    {
        gMike_A80_6F7328->Service();
    }

    if (!bSkip_traffic_lights_67D4EC)
    {
        gTrafficLights_194_705958->TrafficLightsService(); // traffic lights
    }

    gObject_5C_6F8F84->ObjectsService();

    if (!bSkip_mission_67D4E5)
    {
        gScriptManager_6F8060->ExecuteScriptThreads(); // missions
    }

    gEmergencyCrewPool_706280->ServiceAll();

    if (!bSkip_ambulance_67D6C9)
    {
        gAmbulance_110_6F70A8->AmbulancesService(); // ambulance
    }

    if (!bSkip_police_67D4F9)
    {
        gPolice_7B8_6FEE40->Service();
    }

    if (!bSkip_particles_67D64D)
    {
        gParticle_8_6FD5E8->ParticlesService();
    }

    gTileAnim_2_7052C4->UpdateTileAnimations();

    if (bDo_show_timing_67D6DC)
    {
        gFpsCounter_7055D4->ShowFps();
    }

    gHud_2B00_706620->UpdateHUD();
    gpRng_67AB34->AdvanceCycle(); // rng
    gDoor_4D4_67BD2C->DoorsService();
    gFirefighterPool_54_67D4C0->FireEnginesService(); // fire engines

    if (!bExplodingScoresOff_67D4FB)
    {
        gExplodingScorePool->Update();
    }

    if (bDo_show_junc_ids_67D5B0)
    {
        gRouteFinder_6FFDC8->ShowJunctionIds();
    }

    if (counter_706C4C > 0)
    {
        ReapplySavedGamma();
    }
}

MATCH_FUNC(0x4B9640)
bool Game::ExecuteGame()
{
    UpdateAllPlayersControls();

    switch (mGameState)
    {
        case GameState::ExecuteFrameAndPause_0:
            UpdateGame();

            if (!bSkip_audio_67D6BE)
            {
                gRoot_sound_66B038.Service();
            }
            mGameState = GameState::Paused_2;
            break;

        case GameState::Running_1:
            UpdateGame();
            if (!bSkip_audio_67D6BE)
            {
                gRoot_sound_66B038.Service();
            }
            break;

        case GameState::Paused_2:
            UpdatePaused();
            if (!bSkip_audio_67D6BE)
            {
                gRoot_sound_66B038.Service();
            }
            break;

        default:
            break;
    }
    return UpdateExitTimer();
}

MATCH_FUNC(0x4B96B0)
void Game::Pause()
{
    // Singleplayer or playing alone on multiplayer?
    if (mNumPlayers == 1)
    {
        mGameState = GameState::Paused_2;
    }
}

MATCH_FUNC(0x4B96C0)
void Game::Unpause()
{
    if (mNumPlayers == 1)
    {
        mGameState = GameState::Running_1;

        if (gInputDevices_67B990)
        {
            gInputDevices_67B990->flush_keyboard_and_gamepad();
        }

        Player* pPlayer = this->mLocalPlayer;
        if (pPlayer)
        {
            pPlayer->DisableInputs();
            mLocalPlayer->ClearInputs();
            gInputRecorder_67F8B0.mInputBits &= ~0xFFFu;
        }
    }
}

MATCH_FUNC(0x4B9700)
void Game::TogglePause()
{
    if (mGameState == GameState::Running_1)
    {
        Pause();
    }
    else
    {
        Unpause();
    }
}

MATCH_FUNC(0x4B9710)
void Game::SinglePlayerStepFrame()
{
    if (mNumPlayers == 1)
    {
        mGameState = GameState::ExecuteFrameAndPause_0;
    }
}

MATCH_FUNC(0x4B9720)
void Game::ReinitAfterVideoModeChange()
{
    if (mLocalPlayer)
    {
        mLocalPlayer->OnVideoModeChange();
    }

    ResetFrameTimer();

    if (gHud_2B00_706620)
    {
        gHud_2B00_706620->GetTextSpeed();
    }
}

MATCH_FUNC(0x4B9750)
Player* Game::GetFirstPlayerWithoutPed()
{
    for (s32 i = 0; i < mNumPlayers; i++)
    {
        if (mPlayers[i]->mInUse && !mPlayers[i]->has_player_ped_45B0C0())
        {
            return mPlayers[i];
        }
    }
    return NULL;
}

MATCH_FUNC(0x4B9790)
void Game::ShakeCamerasAtPos(Fix16 amount, Fix16 x, Fix16 y)
{
    Camera* pCam = IteratePlayerCamera();
    while (pCam)
    {
        if (pCam->IsPointInViewBounds_40CF60(x, y))
        {
            pCam->SetShake(amount);
        }
        pCam = IterateNextPlayerCamera();
    }
}

MATCH_FUNC(0x4B97E0)
bool Game::IsSpriteInAnyPlayerBounds(Sprite* pSprite, Fix16 margin) const
{
    for (u8 i = 0; i < mNumPlayers; i++)
    {
        if (IsSpriteInPlayerBounds(pSprite, i, margin))
        {
            return true;
        }
    }
    return false;
}

MATCH_FUNC(0x4B9830)
bool Game::IsSpriteInBoundsOrFollowedByAnyPlayer(Sprite* pSprite, Fix16 margin) const
{
    for (u8 i = 0; i < mNumPlayers; i++)
    {
        if (IsSpriteInPlayerBounds(pSprite, i, margin) || IsSpriteCameraSubjectForPlayer(pSprite, i))
        {
            return true;
        }
    }
    return false;
}

MATCH_FUNC(0x4B9890)
bool Game::IsSpriteInAnyPlayerView(Sprite* pSprite, s32 wide_view) const
{
    for (u8 i = 0; i < mNumPlayers; i++)
    {
        if (IsSpriteInPlayerView(pSprite, i, wide_view))
        {
            return true;
        }
    }
    return false;
}

MATCH_FUNC(0x4B98E0)
bool Game::IsSpriteInPlayerView(Sprite* pSprite, u8 player_idx, s32 wide_view) const
{
    if (mPlayers[player_idx]->mInUse)
    {
        if (mPlayers[player_idx]->mGameCamera.IsSpriteInView(pSprite, wide_view))
        {
            return true;
        }
        return mPlayers[player_idx]->mAuxCamActive && mPlayers[player_idx]->mAuxGameCamera.IsSpriteInView(pSprite, wide_view) ? true :
                                                                                                                                   false;
    }
    return false;
}

MATCH_FUNC(0x4B9950)
bool Game::IsSpriteInPlayerBounds(Sprite* pSprite, u8 player_idx, Fix16 margin) const
{
    Player* const& pPlayer = this->mPlayers[player_idx]; // a reference: 9.6f keeps the slot address too
    if (!pPlayer->mInUse)
    {
        return false;
    }

    if (pPlayer->mGameCamera.IsSpriteInBounds_45AEA0(pSprite, margin))
    {
        return true;
    }

    if (pPlayer->mAuxCamActive && pPlayer->mAuxGameCamera.IsSpriteInBounds_45AEA0(pSprite, margin))
    {
        return true;
    }

    return false;
}

MATCH_FUNC(0x4B9A10)
bool Game::IsSpriteCameraSubjectForPlayer(Sprite* pSprite, u8 player_idx) const
{
    if (mPlayers[player_idx]->mInUse)
    {
        if (mPlayers[player_idx]->mGameCamera.IsSpriteTheCameraSubject(pSprite))
        {
            return true;
        }
        return mPlayers[player_idx]->mAuxCamActive && mPlayers[player_idx]->mAuxGameCamera.IsSpriteTheCameraSubject(pSprite) ? true : false;
    }
    return false;
}

MATCH_FUNC(0x4B9A80)
bool Game::IsPointOnScreen(Fix16 x, Fix16 y)
{
    s32 i;
    Player** ppPlayer = mPlayers;
    for (i = 0; i < mNumPlayers; i++, ppPlayer++)
    {
        Player* pPlayer = *ppPlayer;
        if (pPlayer->mInUse)
        {
            if (pPlayer->mGameCamera.IsPointInViewBounds_40CF60(x, y))
            {
                return true;
            }
            if (pPlayer->mAuxCamActive && pPlayer->mAuxGameCamera.IsPointInViewBounds_40CF60(x, y))
            {
                return true;
            }
        }
    }
    return false;
}

MATCH_FUNC(0x4B9B10)
bool Game::IsRectVisibleToAnyPlayer(Fix16_Rect* pBounds)
{
    s32 i;
    Player** ppPlayer = mPlayers;
    for (i = 0; i < mNumPlayers; i++, ppPlayer++)
    {
        Player* pCurPlayer = *ppPlayer;
        if (pCurPlayer->mInUse)
        {
            if (pCurPlayer->mGameCamera.IsRectInBounds_45AF40(pBounds))
            {
                return true;
            }
            if (pCurPlayer->mAuxCamActive && pCurPlayer->mAuxGameCamera.IsRectInBounds_45AF40(pBounds))
            {
                return true;
            }
        }
    }
    return false;
}

MATCH_FUNC(0x4B9BC0)
Camera* Game::IteratePlayerCamera()
{
    for (mCameraIterIdx = 0; mCameraIterIdx < mNumPlayers; mCameraIterIdx++)
    {
        if (mPlayers[mCameraIterIdx]->mInUse)
        {
            mAuxCamIterated = false;
            return &mPlayers[mCameraIterIdx]->mGameCamera;
        }
    }
    return NULL;
}

MATCH_FUNC(0x4B9C10)
bool Game::IsCarInAnyPlayerHistory(Car* pCar)
{
    Player* pPlayer = IterateFirstPlayer();
    while (pPlayer)
    {
        if (pPlayer->PromoteCarInHistory(pCar, true))
        {
            return true;
        }
        pPlayer = IterateNextPlayer();
    }
    return false;
}

MATCH_FUNC(0x4B9C50)
Camera* Game::IterateNextPlayerCamera()
{
    if (!mPlayers[mCameraIterIdx]->mAuxCamActive || mAuxCamIterated)
    {
        while (++mCameraIterIdx < mNumPlayers)
        {
            if (mPlayers[mCameraIterIdx]->mInUse)
            {
                mAuxCamIterated = false;
                return &mPlayers[mCameraIterIdx]->mGameCamera;
            }
        }
        return NULL;
    }
    else
    {
        mAuxCamIterated = true;
        return &mPlayers[mCameraIterIdx]->mAuxGameCamera;
    }
}

MATCH_FUNC(0x4B9CD0)
Player* Game::IterateFirstPlayer()
{
    for (mPlayerIterIdx = 0; mPlayerIterIdx < mNumPlayers; mPlayerIterIdx++)
    {
        if (mPlayers[mPlayerIterIdx]->mInUse)
        {
            return mPlayers[mPlayerIterIdx];
        }
    }
    return NULL;
}

MATCH_FUNC(0x4B9D10)
Player* Game::IterateNextPlayer()
{
    while (++mPlayerIterIdx < mNumPlayers)
    {
        if (mPlayers[mPlayerIterIdx]->mInUse)
        {
            return mPlayers[mPlayerIterIdx];
        }
    }
    return NULL;
}

MATCH_FUNC(0x4B9D60)
void Game::ReduceTrafficSpawnCounterOfOtherViewers(Sprite* pSprite, Player* pExclude)
{
    for (u8 i = 0; i < GTA2_COUNTOF(mPlayers); i++)
    {
        Player* p = gGame_0x40_67E008->GetPlayer_4219E0(i);
        if (p && p != pExclude)
        {
            if (gGame_0x40_67E008->IsSpriteInPlayerBounds(pSprite, i, kFpFive))
            {
                if (p->mTrafficSpawnCounter > p->mTrafficSpawnThreshold)
                {
                    p->mTrafficSpawnCounter -= p->mTrafficSpawnThreshold;
                }
                else
                {
                    p->mTrafficSpawnCounter = 0;
                }
            }
        }
    }
}

DEFINE_GLOBAL_INIT(Fix16, kInitialAmbient_67DCCC, Fix16(0x666, 0), 0x67DCCC);

MATCH_FUNC(0x4B9DE0)
Game::Game(u8 max_players, s8 player_idx)
{
    gRng_6F6784.srand_4F7A40();

    mSkipPolice = bSkip_police_67D4F9;

    for (s32 ii = 0; ii < GTA2_COUNTOF(mPlayers); ii++)
    {
        mPlayers[ii] = NULL;
    }

    mNumPlayers = max_players;
    mLocalPlayerIdx = player_idx;
    for (u32 i = 0; i < mNumPlayers; i++)
    {
        mPlayers[i] = new Player(i); // ctor call
        if (!mPlayers[i])
        {
            FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1784);
        }
    }
    Player* pPlayer = mPlayers[mLocalPlayerIdx];
    mLocalPlayer = pPlayer;
    pPlayer->mIsUser = true;
    mDrawPlayer = mLocalPlayer;

    gpRng_67AB34 = new rng(); // inline
    if (!gpRng_67AB34)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1793);
    }

    gText_0x14_704DFC = new TextTable(); // ctor call
    if (!gText_0x14_704DFC)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1795);
    }

    gGtx_0x106C_703DD4 = new gtx_0x106C(); // ctor call
    if (!gGtx_0x106C_703DD4)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1803);
    }

    gMap_0x370_6F6268 = new Map_0x370(); // ctor call
    if (!gMap_0x370_6F6268)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1806);
    }

    gpMapRenderer_6F66E4 = new MapRenderer(); // inlined ok?
    if (!gpMapRenderer_6F66E4)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1808);
    }

    gSpriteRenderer_67B580 = new SpriteRenderer(); // ctor call
    if (!gSpriteRenderer_67B580)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1810);
    }

    gMike_A80_6F7328 = new FrameProfiler(); // inlined
    if (!gMike_A80_6F7328)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1824);
    }

    gCarManager_6C_677930 = new CarManager(); // ctor call
    if (!gCarManager_6C_677930)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1827);
    }

    gCarThreadTriggers_214_705F20 = new CarThreadTriggers(); // no ctor
    if (!gCarThreadTriggers_214_705F20)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1829);
    }

    gScriptManager_6F8060 = new ScriptManager(); // ctor call
    if (!gScriptManager_6F8060)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1831);
    }

    gScriptStackFramePool_6F8068 = new ScriptStackFramePool(); // multi level inlines
    if (!gScriptStackFramePool_6F8068)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1833);
    }

    gFpsCounter_7055D4 = new FpsCounter(); // ctor call
    if (!gFpsCounter_7055D4)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1836);
    }

    gSpriteManager_703820 = new SpriteManager_8(); // ctor call
    if (!gSpriteManager_703820)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1839);
    }

    gPedManager_6787BC = new PedManager(); // ctor call
    if (!gPedManager_6787BC)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1841);
    }

    gCollisionCounters_6791FC = new CollisionCounters(); // ctor call
    if (!gCollisionCounters_6791FC)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1843);
    }

    gObjectDefinitions_6FCF00 = new ObjectDefinitions(); // ctor call
    if (!gObjectDefinitions_6FCF00)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1845);
    }

    gObject_5C_6F8F84 = new Object_5C(); // ctor call
    if (!gObject_5C_6F8F84)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1847);
    }

    gPedRefTable_7F8_703398 = new PedRefTable(); // ctor call
    if (!gPedRefTable_7F8_703398)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1849);
    }

    gTileAnim_2_7052C4 = new TileAnimManager(); // ctor call
    if (!gTileAnim_2_7052C4)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1851);
    }

    gWeapon_8_707018 = new WeaponManager(); // ctor call
    if (!gWeapon_8_707018)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1853);
    }

    gDoor_4D4_67BD2C = new DoorPool(); // ctor call ok
    if (!gDoor_4D4_67BD2C)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1855);
    }

    gAmbulance_110_6F70A8 = new AmbulanceService(); // multi level inline
    if (!gAmbulance_110_6F70A8)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1857);
    }

    gPublicTransport_181C_6FF1D4 = new PublicTransport(); // ctor call
    if (!gPublicTransport_181C_6FF1D4)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1860);
    }

    gTaxi_4_704130 = new TaxiList(); // ctor call
    if (!gTaxi_4_704130)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1863);
    }

    gHud_2B00_706620 = new Hud(); // ctor call
    if (!gHud_2B00_706620)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1865);
    }

    gTextureCache_15D8_705064 = new TextureCache(); // ctor call
    if (!gTextureCache_15D8_705064)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1867);
    }

    gTrafficLights_194_705958 = new TrafficLights(); // ctor call
    if (!gTrafficLights_194_705958)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1869);
    }

    gRouteFinder_6FFDC8 = new RouteFinder(); // ctor call
    if (!gRouteFinder_6FFDC8)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1871);
    }

    gPatrolRoutePool_6FD784 = new PatrolRoutePool(); // ctor call
    if (!gPatrolRoutePool_6FD784)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1873);
    }

    gPathFinder_6FDEF0 = new PathFinder(); // inline
    if (!gPathFinder_6FDEF0)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1875);
    }

    gCarInfo_808_678098 = new CarInfoTable(); // ctor call
    if (!gCarInfo_808_678098)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1877);
    }

    gParticle_8_6FD5E8 = new ParticleManager(); // ctor call
    if (!gParticle_8_6FD5E8)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1879);
    }

    gExplosionPool_3D4_6FD5EC = new ExplosionPool_3D4(); // ctor call
    if (!gExplosionPool_3D4_6FD5EC)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1881);
    }

    gExplosionPool_7A8_6FD5F0 = new ExplosionPool(); // ctor call
    if (!gExplosionPool_7A8_6FD5F0)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1883);
    }

    gCranePool_D9C_679FD4 = new CranePool(); // ctor call
    if (!gCranePool_D9C_679FD4)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1885);
    }

    gCrusherPool_94_67A830 = new CrusherPool(); // ctor call
    if (!gCrusherPool_94_67A830)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1887);
    }

    gGeneratorPool_14AC_67E5D0 = new GeneratorPool(); // ctor call
    if (!gGeneratorPool_14AC_67E5D0)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1889);
    }

    gEmergencyCrewPool_706280 = new EmergencyCrewPool(); // multi level inline
    if (!gEmergencyCrewPool_706280)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1891);
    }

    gPolice_7B8_6FEE40 = new Police(); // multi level inline
    if (!gPolice_7B8_6FEE40)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1893);
    }

    gLight_1D4CC_6F5520 = new LightPool(); // multi level inline
    if (!gLight_1D4CC_6F5520)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1896);
    }

    gGangPool_CA8_67E274 = new GangPool(); // multi level inline
    if (!gGangPool_CA8_67E274)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1899);
    }

    gGarage_48_6FD26C = new Garage(); // ctor call
    if (!gGarage_48_6FD26C)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1901);
    }

    gCarChaseTaskTable_678E30 = new CarChaseTaskTable(); // ctor call
    if (!gCarChaseTaskTable_678E30)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1903);
    }

    if (!bExplodingScoresOff_67D4FB)
    {
        gExplodingScorePool = new ExplodingScorePool(); // ctor call
        if (!gExplodingScorePool)
        {
            FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1909);
        }
    }

    gCrimeReportQueue_67A4B8 = new CrimeReportQueue_CC_Sub(); // ctor call
    if (!gCrimeReportQueue_67A4B8)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1913);
    }

    gFirefighterPool_54_67D4C0 = new FirefighterPool(); // multi level inline 4A88D0 func only
    if (!gFirefighterPool_54_67D4C0)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1915);
    }

    gCollisionSoundQueue_66AFE0 = new CollisionSoundQueue(); // ctor call
    if (!gCollisionSoundQueue_66AFE0)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1918);
    }

    if (gText_0x14_704DFC->LangIsJapanese_452E60())
    {
        gKanjiFont_6F5168 = new KanjiFont(); // ctor call
        if (!gKanjiFont_6F5168)
        {
            FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1923);
        }
    }

    mCameraIterIdx = 0;
    mAuxCamIterated = false;
    mGameState = GameState::Running_1;
    gbRngRemapTableDone_679C0A = false;
    mPlayerIterIdx = 0;
    mExitTimer = -1;
    mFastForward = false;
    mGameExitType = GameExitType::None;
    mNetworkFrameMs = 0;
    if (!bSkip_audio_67D6BE)
    {
        gRoot_sound_66B038.InitMusicAndCopRadio();
    }
}

MATCH_FUNC(0x4BAE30)
Game::~Game()
{
    bSkip_police_67D4F9 = mSkipPolice;

    if (!bSkip_audio_67D6BE)
    {
        gRoot_sound_66B038.DeInitVocals();
    }

    for (u32 i = 0; i < mNumPlayers; i++)
    {
        GTA2_DELETE_AND_NULL(mPlayers[i]);
    }

    GTA2_DELETE_AND_NULL(gpRng_67AB34);
    GTA2_DELETE_AND_NULL(gText_0x14_704DFC);
    GTA2_DELETE_AND_NULL(gGtx_0x106C_703DD4);
    GTA2_DELETE_AND_NULL(gMap_0x370_6F6268);
    GTA2_DELETE_AND_NULL(gpMapRenderer_6F66E4);
    GTA2_DELETE_AND_NULL(gSpriteRenderer_67B580);
    GTA2_DELETE_AND_NULL(gPedPool_6787B8);

    GTA2_DELETE_AND_NULL(gCarManager_6C_677930);
    GTA2_DELETE_AND_NULL(gCarThreadTriggers_214_705F20);
    GTA2_DELETE_AND_NULL(gScriptManager_6F8060);
    GTA2_DELETE_AND_NULL(gScriptStackFramePool_6F8068);

    GTA2_DELETE_AND_NULL(gObjectDefinitions_6FCF00);
    GTA2_DELETE_AND_NULL(gObject_5C_6F8F84);

    GTA2_DELETE_AND_NULL(gPedManager_6787BC);
    GTA2_DELETE_AND_NULL(gMike_A80_6F7328);

    GTA2_DELETE_AND_NULL(gFpsCounter_7055D4);

    GTA2_DELETE_AND_NULL(gSpriteManager_703820);
    GTA2_DELETE_AND_NULL(gCollisionCounters_6791FC);
    GTA2_DELETE_AND_NULL(gPedRefTable_7F8_703398);
    GTA2_DELETE_AND_NULL(gPublicTransport_181C_6FF1D4);
    GTA2_DELETE_AND_NULL(gTaxi_4_704130);
    GTA2_DELETE_AND_NULL(gTileAnim_2_7052C4);
    GTA2_DELETE_AND_NULL(gWeapon_8_707018);
    GTA2_DELETE_AND_NULL(gDoor_4D4_67BD2C);
    GTA2_DELETE_AND_NULL(gAmbulance_110_6F70A8);
    GTA2_DELETE_AND_NULL(gHud_2B00_706620);
    GTA2_DELETE_AND_NULL(gTextureCache_15D8_705064);

    GTA2_DELETE_AND_NULL(gTrafficLights_194_705958);
    GTA2_DELETE_AND_NULL(gRouteFinder_6FFDC8);

    GTA2_DELETE_AND_NULL(gPatrolRoutePool_6FD784);

    GTA2_DELETE_AND_NULL(gPathFinder_6FDEF0);

    GTA2_DELETE_AND_NULL(gCarInfo_808_678098);

    GTA2_DELETE_AND_NULL(gParticle_8_6FD5E8);
    GTA2_DELETE_AND_NULL(gExplosionPool_3D4_6FD5EC);
    GTA2_DELETE_AND_NULL(gExplosionPool_7A8_6FD5F0);
    GTA2_DELETE_AND_NULL(gCranePool_D9C_679FD4);
    GTA2_DELETE_AND_NULL(gCrusherPool_94_67A830);
    GTA2_DELETE_AND_NULL(gGeneratorPool_14AC_67E5D0);

    GTA2_DELETE_AND_NULL(gEmergencyCrewPool_706280);
    GTA2_DELETE_AND_NULL(gPolice_7B8_6FEE40);
    GTA2_DELETE_AND_NULL(gLight_1D4CC_6F5520);
    GTA2_DELETE_AND_NULL(gGangPool_CA8_67E274);
    GTA2_DELETE_AND_NULL(gGarage_48_6FD26C);
    GTA2_DELETE_AND_NULL(gCarChaseTaskTable_678E30);

    if (!bExplodingScoresOff_67D4FB)
    {
        GTA2_DELETE_AND_NULL(gExplodingScorePool);
    }

    GTA2_DELETE_AND_NULL(gCrimeReportQueue_67A4B8);
    GTA2_DELETE_AND_NULL(gFirefighterPool_54_67D4C0);
    GTA2_DELETE_AND_NULL(gCollisionSoundQueue_66AFE0);

    if (gKanjiFont_6F5168)
    {
        GTA2_DELETE_AND_NULL(gKanjiFont_6F5168);
    }

    mDrawPlayer = NULL;
    mLocalPlayer = NULL;
}