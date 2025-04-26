#include "Game_0x40.hpp"
#include "Ambulance_110.hpp"
#include "CarInfo_808.hpp"
#include "Car_BC.hpp"
#include "Door_4D4.hpp"
#include "DrawUnk_0xBC.hpp"
#include "ExplodingScore_100.hpp"
#include "Frismo_25C.hpp"
#include "Frontend.hpp"
#include "Function.hpp"
#include "Garage_48.hpp"
#include "Garox_2B00.hpp"
#include "Globals.hpp"
#include "Hamburger_500.hpp"
#include "Kfc_1E0.hpp"
#include "Light_1D4CC.hpp"
#include "Maccies_14AC.hpp"
#include "MapRenderer.hpp"
#include "Mike_A80.hpp"
#include "Montana.hpp"
#include "Object_5C.hpp"
#include "Orca_2FD4.hpp"
#include "Particle_8.hpp"
#include "Ped.hpp"
#include "PedGroup.hpp"
#include "Phi_8CA8.hpp"
#include "Player.hpp"
#include "Police_7B8.hpp"
#include "PurpleDoom.hpp"
#include "RouteFinder.hpp"
#include "Rozza_C88.hpp"
#include "Sero_181C.hpp"
#include "Shooey_CC.hpp"
#include "Snooky_94.hpp"
#include "Tango_28.hpp"
#include "Taxi_4.hpp"
#include "TileAnim_2.hpp"
#include "TrafficLights_194.hpp"
#include "Varrok_7F8.hpp"
#include "Weapon_8.hpp"
#include "Wolfy_3D4.hpp"
#include "Zheal_D9C.hpp"
#include "Zones_CA8.hpp"
#include "char.hpp"
#include "collide.hpp"
#include "debug.hpp"
#include "error.hpp"
#include "frosty_pasteur_0xC1EA8.hpp"
#include "gbh_graphics.hpp"
#include "gtx_0x106C.hpp"
#include "jolly_poitras_0x2BC0.hpp"
#include "lucid_hamilton.hpp"
#include "magical_germain_0x8EC.hpp"
#include "map_0x370.hpp"
#include "registry.hpp"
#include "rng.hpp"
#include "root_sound.hpp"
#include "sharp_bose_0x54.hpp"
#include "sharp_pare_0x15D8.hpp"
#include "sprite.hpp"
#include "text_0x14.hpp"
#include "winmain.hpp"

EXPORT_VAR s32 dword_67DFB4; // TODO
GLOBAL(dword_67DFB4, 0x67DFB4);

EXPORT_VAR s32 dword_7071A0;
GLOBAL(dword_7071A0, 0x7071A0);

EXPORT_VAR s32 dword_7071B0;
GLOBAL(dword_7071B0, 0x7071B0);

// TODO
EXPORT_VAR extern u32 counter_706C4C;
EXPORT_VAR extern char_type gLighting_626A09;

EXPORT_VAR Game_0x40* gGame_0x40_67E008;
GLOBAL(gGame_0x40_67E008, 0x67E008)

EXPORT_VAR Rozza_C88* gRozza_C88_66AFE0;
GLOBAL(gRozza_C88_66AFE0, 0x66AFE0);

EXPORT_VAR Tango_54* gTango_54_67D4C0;
GLOBAL(gTango_54_67D4C0, 0x67D4C0);

EXPORT_VAR Shooey_CC* gShooey_CC_67A4B8;
GLOBAL(gShooey_CC_67A4B8, 0x67A4B8);

EXPORT_VAR Hamburger_500* gHamburger_500_678E30;
GLOBAL(gHamburger_500_678E30, 0x678E30);

EXPORT_VAR Police_7B8* gPolice_7B8_6FEE40;
GLOBAL(gPolice_7B8_6FEE40, 0x6FEE40);

EXPORT_VAR Kfc_1E0* gKfc_1E0_706280;
GLOBAL(gKfc_1E0_706280, 0x706280);

EXPORT_VAR Wolfy_7A8* gWolfy_7A8_6FD5F0;
GLOBAL(gWolfy_7A8_6FD5F0, 0x6FD5F0);

EXPORT_VAR Wolfy_3D4* gWolfy_3D4_6FD5EC;
GLOBAL(gWolfy_3D4_6FD5EC, 0x6FD5EC);

EXPORT_VAR Particle_8* gParticle_8_6FD5E8;
GLOBAL(gParticle_8_6FD5E8, 0x6FD5E8);

EXPORT_VAR Orca_2FD4* gOrca_2FD4_6FDEF0;
GLOBAL(gOrca_2FD4_6FDEF0, 0x6FDEF0);

EXPORT_VAR Marz_1D7E* gMarz_1D7E_6FD784;
GLOBAL(gMarz_1D7E_6FD784, 0x6FD784);

EXPORT_VAR Taxi_4* gTaxi_4_704130;
GLOBAL(gTaxi_4_704130, 0x704130);

EXPORT_VAR Ambulance_110* gAmbulance_110_6F70A8;
GLOBAL(gAmbulance_110_6F70A8, 0x6F70A8);

EXPORT_VAR Varrok_7F8* gVarrok_7F8_703398;
GLOBAL(gVarrok_7F8_703398, 0x703398);

EXPORT_VAR Phi_8CA8* gPhi_8CA8_6FCF00;
GLOBAL(gPhi_8CA8_6FCF00, 0x6FCF00);

EXPORT_VAR Collide_C* gCollide_C_6791FC;
GLOBAL(gCollide_C_6791FC, 0x6791FC);

EXPORT_VAR sharp_bose_0x54* gsharp_bose_0x54_7055D4;
GLOBAL(gsharp_bose_0x54_7055D4, 0x7055D4);

EXPORT_VAR Light_1D4CC* gLight_1D4CC_6F5520;
GLOBAL(gLight_1D4CC_6F5520, 0x6F5520);

// TODO: move
void sub_SetGamma();

MATCH_FUNC(0x4B8BB0)
s32 Game_0x40::sub_4B8BB0()
{
    if (bStartNetworkGame_7081F0 == false)
    {
        return field_30 == false ? 33 : 11;
    }
    return field_34;
}

MATCH_FUNC(0x4B8BD0)
void Game_0x40::sub_4B8BD0(s32 new_timer, s32 main_state, s8 a2)
{
    int timer = field_28_timer;
    if (timer == -1 || timer > new_timer)
    {
        field_28_timer = new_timer;
        field_2C_main_state = main_state;
        gLucid_hamilton_67E8E0.sub_4C5930(a2);
    }
}

MATCH_FUNC(0x4B8C00)
void Game_0x40::sub_4B8C00(s32 a1, s32 a2)
{
    sub_4B8BD0(a1, a2, 0);
}

MATCH_FUNC(0x4B8C20)
s8 Game_0x40::sub_4B8C20()
{
    if (field_28_timer == 0)
    {
        return 1;
    }

    if (field_28_timer > 0)
    {
        field_28_timer--;
    }

    return 0;
}

MATCH_FUNC(0x4B8C40)
void Game_0x40::sub_4B8C40()
{
    char_type tmp_array[256];

    gText_0x14_704DFC->Load_5B5E90();
    if (bSkip_audio_67D6BE == false)
    {
        char_type* style_name = gLucid_hamilton_67E8E0.GetStyleName_4C5950();
        gRoot_sound_66B038.LoadStyle_40EFF0(style_name);
    }

    char_type *style_name = gLucid_hamilton_67E8E0.GetStyleName_4C5950();
    gGtx_0x106C_703DD4->LoadSty_5AB750(style_name);

    char_type *map_name = gLucid_hamilton_67E8E0.GetMapName_4C5940();
    gMap_0x370_6F6268->LoadMap_4E95B0(map_name);

    gPhi_8CA8_6FCF00->sub_534330();

    gSharp_pare_0x15D8_705064->sub_5B9350();

    gLucid_hamilton_67E8E0.reset_field_574();

    char_type *script_name = gLucid_hamilton_67E8E0.GetScriptName_4C5960();
    gfrosty_pasteur_6F8060->Load_512330(script_name);

    if (strlen(gLucid_hamilton_67E8E0.GetDebugStr_4C5970()) != 0)
    {
        char_type *debug_str = gLucid_hamilton_67E8E0.GetDebugStr_4C5970();
        gfrosty_pasteur_6F8060->sub_511F80(debug_str);
    }

    strcpy(tmp_array, "data\\");
    memset(gTmpBuffer_67C598, 0, sizeof(gTmpBuffer_67C598));
    gRegistry_6FF968.Get_Debug_Setting_586E90("carname", (LPBYTE)gTmpBuffer_67C598, 0x100);

    strcat(tmp_array, gTmpBuffer_67C598);

    if (strcmp(tmp_array, "data\\"))
    {
        gCarInfo_808_678098->sub_454A00(tmp_array);
    }
    else
    {
        gCarInfo_808_678098->sub_454A00("data\\nyc.gci");
    }

    gCar_6C_677930->sub_444980();

    if (gMagical_germain_0x8EC_6F5168 != NULL)
    {
        gMagical_germain_0x8EC_6F5168->sub_4D2B40();
    }
}

MATCH_FUNC(0x4B8E00)
void Game_0x40::sub_4B8E00(u32 a1, u32 a2)
{
    field_38_orf1->field_90_game_camera.sub_4361B0(a1, a2);
    field_38_orf1->field_208_aux_game_camera.sub_4361B0(a1, a2);
    field_38_orf1->field_14C_view_camera.sub_4361B0(a1, a2);
    gGarox_2B00_706620->sub_5D6AB0();
}

MATCH_FUNC(0x4B8E50)
void Game_0x40::sub_4B8E50()
{
    switch (dword_7071A0)
    {
        case 0:
            field_34 = 50;
            break;

        case 1:
            field_34 = 40;
            break;

        case 2:
            field_34 = 33;
            break;
    }

    bSkip_police_67D4F9 = dword_7071B0 == 0;
}

// TODO: move
EXPORT_VAR extern s32 bStartNetworkGame_7081F0;

// TODO: move
STUB_FUNC(0x46E370)
EXPORT s32 IanTest_46E370()
{
    return 0;
}

// TODO: move
STUB_FUNC(0x5D8DF0)
EXPORT void sub_5D8DF0()
{
}

// TODO: move
STUB_FUNC(0x5D8E00)
EXPORT void sub_5D8E00()
{
}

MATCH_FUNC(0x4B8EB0)
void Game_0x40::sub_4B8EB0()
{

    gLucid_hamilton_67E8E0.sub_4C5AB0(0);
    if (bStartNetworkGame_7081F0)
    {
        sub_4B8E50();
    }

    sub_5D8DF0();
    sub_5D7CB0();
    sub_5D8E00();
    gSprite_8_703820->sub_5A5870();
    gTileAnim_2_7052C4->Empty_5BC300();
    gSero_181C_6FF1D4->sub_5794B0();
    gObject_5C_6F8F84->sub_5297F0();
    PedGroup::sub_4CB080();
    if (bDo_mike_67D5CC)
    {
        gMike_A80_6F7328->sub_4FF1B0();
    }
    gCar_214_705F20->sub_5C8750();
    gMap_0x370_6F6268->alloc_zones_4DFCA0();
    gGarox_2B00_706620->sub_5D6BE0();
    gfrosty_pasteur_6F8060->Update_512160(); // script
    gGame_0x40_67E008->field_38_orf1->sub_56A490();
    if (bDo_iain_test_67D4E9)
    {
        IanTest_46E370();
    }
    gRouteFinder_6FFDC8->Reset_588C60();
    gGarox_2B00_706620->sub_5D6BE0();
    gMap_0x370_6F6268->sub_4DFB90(); // map objects
    gMap_0x370_6F6268->update_lights_4DFCD0(); // lights
    if (!bSkip_traffic_lights_67D4EC)
    {
        gTrafficLights_194_705958->sub_5C2AC0();
    }
    gSero_181C_6FF1D4->sub_578860(); // trains?

    for (s32 i = 0; i < field_23_max_idx; i++)
    {
        field_4_players[i]->sub_569CB0(); // respawn dead players?
    }

    gJolly_poitras_0x2BC0_6FEAC0->sub_56C250();
}

MATCH_FUNC(0x4B8FF0)
void Game_0x40::ShowCounters_4B8FF0()
{
    swprintf(tmpBuff_67BD9C, L"recycled cars : %d", gCar_6C_677930->field_28_recycled_cars);
    gGarox_2B00_706620->field_650.sub_5D1F50(tmpBuff_67BD9C, 0, 32, word_706600, 1);

    swprintf(tmpBuff_67BD9C, L"prot. recycled cars : %d", gCar_6C_677930->field_40_proto_recycled_cars);
    gGarox_2B00_706620->field_650.sub_5D1F50(tmpBuff_67BD9C, 0, 48, word_706600, 1);

    swprintf(tmpBuff_67BD9C, L"mission cars : %d", gCar_6C_677930->field_3C_mission_cars);
    gGarox_2B00_706620->field_650.sub_5D1F50(tmpBuff_67BD9C, 0, 64, word_706600, 1);

    swprintf(tmpBuff_67BD9C, L"unit cars : %d", gCar_6C_677930->field_34_unit_cars);
    gGarox_2B00_706620->field_650.sub_5D1F50(tmpBuff_67BD9C, 0, 80, word_706600, 1);

    swprintf(tmpBuff_67BD9C, L"cars:%d", gCar_E0C4_67792C->field_E0C0_cars_count);
    gGarox_2B00_706620->field_650.sub_5D1F50(tmpBuff_67BD9C, 0, 96, word_706600, 1);

    swprintf(tmpBuff_67BD9C, L"dummy_chars : %d", (unsigned __int8)gChar_C_6787BC->field_2);
    gGarox_2B00_706620->field_650.sub_5D1F50(tmpBuff_67BD9C, 0, 112, word_706600, 1);

    Player* field_38_orf1 = gGame_0x40_67E008->field_38_orf1;
    if (field_38_orf1)
    {
        swprintf(tmpBuff_67BD9C, L"accuracy_count : %d", (unsigned __int8)field_38_orf1->field_2D4_unk.field_198_accuracy_count);
        gGarox_2B00_706620->field_650.sub_5D1F50(tmpBuff_67BD9C, 0, 128, word_706600, 1);

        swprintf(tmpBuff_67BD9C, L"reverse_count : %d", field_38_orf1->field_2D4_unk.field_19C_reverse_count);
        gGarox_2B00_706620->field_650.sub_5D1F50(tmpBuff_67BD9C, 0, 144, word_706600, 1);

        // TODO: Seems strange, converted to a local integer point or something ??
        const u32 x = field_38_orf1->field_2C4_player_ped->field_1AC_cam.x.ToInt();
        const u32 y = field_38_orf1->field_2C4_player_ped->field_1AC_cam.y.ToInt();

        gmp_zone_unknown* pNavZone = gMap_0x370_6F6268->get_nav_zone_unknown_4DF890(x, y);

        swprintf(tmpBuff_67BD9C, L"density:%d", pNavZone->field_0_density);
        gGarox_2B00_706620->field_650.sub_5D1F50(tmpBuff_67BD9C, 0, 160, word_706600, 1);
    }
}

MATCH_FUNC(0x4B9270)
void Game_0x40::sub_4B9270()
{
    if (gDo_show_counter_67D6E6)
    {
        ShowCounters_4B8FF0();
    }

    if (field_30)
    {
        Garox_C4* pC4 = gGarox_2B00_706620->field_650.sub_5D1F50(L"FF", 0, 440, word_706600, 1);
        pC4->field_B0 = 8;
        pC4->field_B4 = 6;
    }
}

MATCH_FUNC(0x4B92D0)
void Game_0x40::Draw_4B92D0()
{
    gViewCamera_676978 = &field_1C_unk->field_14C_view_camera;

    gpMapRenderer_6F66E4->ClearDrawnTileCount_4F6A10();
    gSprite_8_703820->sub_5A5860();
    gMontana_67B580->ResetAll_4954F0();

    gPurpleDoom_3_679210->sub_477A40();
    gPurpleDoom_2_67920C->sub_477A40();
    gPurpleDoom_1_679208->sub_477A40();

    sub_5D7D30();

    pgbh_BeginScene();

    gpMapRenderer_6F66E4->Draw_4F6A20();

    if (!bExplodingOff_67D4FB)
    {
        gExplodingScore_100_702F34->DrawExploding_5969E0();
    }

    gGarox_2B00_706620->DrawGui_5D6860(); // user

    if (bDo_mike_67D5CC)
    {
        gMike_A80_6F7328->DebugDrawProfiling_4FF250();
    }

    pgbh_EndScene();
    FreeSurface_5D7DC0();
}

MATCH_FUNC(0x4B9380)
void Game_0x40::sub_4B9380()
{
    Player** ppPlayersIterator = field_4_players;
    u32 idx = 0;
    while (idx < field_23_max_idx)
    {
        if ((*ppPlayersIterator)->field_8E_bInUse)
        {
            (*ppPlayersIterator)->sub_566820();
        }
        ++idx;
        ++ppPlayersIterator;
    }
}

//MATCH_FUNC(0x5D9970)
static void SetGamma_5D9970() // TODO Function chunk of 0x4AEC00, 0x4B93C0 and 0x4B9410 - probably move elsewhere
{
    const s32 gamma = gRegistry_6FF968.Get_Screen_Setting_5870D0("gamma", 10u);
    if (counter_706C4C)
    {
        if (SetGamma_5D9910(gamma))
        {
            --counter_706C4C;
        }
        else
        {
            counter_706C4C = 0;
        }
    }
}

MATCH_FUNC(0x4B93C0)
void Game_0x40::sub_4B93C0()
{
    Player** pIter = field_4_players;
    u32 i = 0;
    while (i < field_23_max_idx)
    {
        if ((*pIter)->field_8E_bInUse)
        {
            (*pIter)->sub_569410();
        }
        ++i;
        ++pIter;
    }

    gGarox_2B00_706620->sub_5D69C0();

    if (counter_706C4C > 0)
    {
        SetGamma_5D9970();
    }
}

// https://decomp.me/scratch/vQmiC - asm differ bug, needs objdiff
STUB_FUNC(0x4B9410)
void Game_0x40::sub_4B9410()
{
    for (s32 i = 0; i < field_23_max_idx; i++)
    {
        if (field_4_players[i]->field_8E_bInUse)
        {
            field_4_players[i]->sub_5687F0();
        }
    }

    gRozza_C88_66AFE0->Reset_40BB90();
    gCollide_C_6791FC->sub_478A20();

    if (gLighting_626A09)
    {
        Light_1D4CC* pgLight_1D4CC = gLight_1D4CC_6F5520;
        nostalgic_ellis_0x28* pCurLight = gLight_1D4CC_6F5520->field_4;
        gLight_1D4CC_6F5520->field_1D4C8 = 0;
        if (pCurLight)
        {
            nostalgic_ellis_0x28* pNext;
            do
            {
                ++pgLight_1D4CC->field_1D4C8;
                pNext = pCurLight->field_1C;
                char new_off_time = pCurLight->field_17_off_time - 1;
                pCurLight->field_17_off_time = new_off_time;
                if (!new_off_time)
                {
                    // if (LOBYTE(pCurLight->field_0))
                    if (pCurLight->field_0)
                    {
                        int v8 = pCurLight->field_0;
                        u8* p_field_16_shape = &pCurLight->field_16_shape;
                        pCurLight->field_17_off_time = pCurLight->field_15_off_time;
                        //LOBYTE(v8) = 0;
                        u8 local_field_16_shape = pCurLight->field_16_shape;
                        pCurLight->field_0 = v8;
                        if (local_field_16_shape)
                        {
                            // goto LABEL_12;
                            pCurLight->field_17_off_time += stru_6F6784.get_uint8_4F7B70(p_field_16_shape);
                        }
                    }
                    else
                    {
                        int v11 = pCurLight->field_0;
                        // LOBYTE(v11) = 0;
                        int local_field_18_intensity = pCurLight->field_18_intensity;
                        pCurLight->field_17_off_time = pCurLight->field_14_on_time;
                        u8 shape = pCurLight->field_16_shape;
                        int v14 = v11 | local_field_18_intensity;
                        u8* p_field_16_shape = &pCurLight->field_16_shape;
                        pCurLight->field_0 = v14;
                        if (shape)
                        {
                            //LABEL_12:
                            pCurLight->field_17_off_time += stru_6F6784.get_uint8_4F7B70(p_field_16_shape);
                        }
                    }
                }
                pCurLight = pNext;
            } while (pNext);
        }
    }

    gZheal_D9C_679FD4->sub_480E50();
    gSnooky_94_67A830->sub_4887F0();
    gMaccies_14AC_67E5D0->sub_4C1D70();
    gChar_C_6787BC->sub_4703F0(); // ped stuff? has arg??
    gSero_181C_6FF1D4->sub_57A7A0(); // trains
    gGarage_48_6FD26C->sub_5349D0();
    gCar_6C_677930->sub_446790();

    if (bDo_mike_67D5CC)
    {
        gMike_A80_6F7328->sub_4FF980();
    }

    if (!bSkip_traffic_lights_67D4EC)
    {
        gTrafficLights_194_705958->sub_5C2950(); // traffic lights
    }

    gObject_5C_6F8F84->sub_5293A0();

    if (!bSkip_mission_67D4E5)
    {
        gfrosty_pasteur_6F8060->sub_5127A0(); // missions
    }

    gKfc_1E0_706280->sub_5CBBD0();

    if (!bSkip_ambulance_67D6C9)
    {
        gAmbulance_110_6F70A8->sub_4FA790(); // ambulance
    }

    if (!bSkip_police_67D4F9)
    {
        gPolice_7B8_6FEE40->sub_570270();
    }

    if (!bSkip_particles_67D64D)
    {
        gParticle_8_6FD5E8->sub_53E320();
    }

    gTileAnim_2_7052C4->sub_5BC310();

    if (bDo_show_timing_67D6DC)
    {
        gsharp_bose_0x54_7055D4->ShowFps_5BEC30();
    }

    gGarox_2B00_706620->sub_5D69D0();
    rng_dword_67AB34->sub_48B900(); // rng
    gDoor_4D4_67BD2C->sub_49D460();
    gTango_54_67D4C0->sub_4A85F0(); // fire engines

    if (!bExplodingOff_67D4FB)
    {
        gExplodingScore_100_702F34->sub_596940();
    }

    if (bDo_show_junc_ids_67D5B0)
    {
        gRouteFinder_6FFDC8->ShowJunctionIds_588620();
    }

    if (counter_706C4C)
    {
        sub_SetGamma();
    }
}

MATCH_FUNC(0x4B9640)
s8 Game_0x40::sub_4B9640()
{
    sub_4B9380();

    switch (field_0_game_state)
    {
        case 0:
            sub_4B9410();

            if (!bSkip_audio_67D6BE)
            {
                gRoot_sound_66B038.Service_40EFA0();
            }
            field_0_game_state = 2;
            break;

        case 1:
            sub_4B9410();
            if (!bSkip_audio_67D6BE)
            {
                gRoot_sound_66B038.Service_40EFA0();
            }
            break;

        case 2:
            sub_4B93C0();
            if (!bSkip_audio_67D6BE)
            {
                gRoot_sound_66B038.Service_40EFA0();
            }
            break;

        default:
            break;
    }
    return sub_4B8C20();
}

STUB_FUNC(0x4B9700)
void Game_0x40::sub_4B9700()
{
}

MATCH_FUNC(0x4B9710)
void Game_0x40::sub_4B9710()
{
    if (field_23_max_idx == 1)
    {
        field_0_game_state = 0;
    }
}

MATCH_FUNC(0x4B9720)
void Game_0x40::sub_4B9720()
{
    if (field_38_orf1)
    {
        field_38_orf1->sub_565460();
    }

    sub_4DA830();

    if (gGarox_2B00_706620)
    {
        gGarox_2B00_706620->sub_5D6A90();
    }
}

MATCH_FUNC(0x4B9750)
Player* Game_0x40::sub_4B9750()
{
    for (s32 i = 0; i < field_23_max_idx; i++)
    {
        if (field_4_players[i]->field_8E_bInUse && !field_4_players[i]->field_2C4_player_ped)
        {
            return field_4_players[i];
        }
    }
    return NULL;
}

MATCH_FUNC(0x4B9790)
void Game_0x40::sub_4B9790(Fix16 a2, Fix16 a3, Fix16 a4)
{
    DrawUnk_0xBC* pCam = IteratePlayerCamera_4B9BC0();
    while (pCam)
    {
        if (a3 >= pCam->field_78_win_left && a3 <= pCam->field_7C_win_right && a4 >= pCam->field_80_win_top && a4 <= pCam->field_84_win_bottom)
        {
            pCam->sub_436120(a2);
        }
        pCam = sub_4B9C50();
    }
}

MATCH_FUNC(0x4B97E0)
s8 Game_0x40::sub_4B97E0(Sprite* a2, s32 a3)
{
    for (u8 i = 0; i < field_23_max_idx; i++)
    {
        if (sub_4B9950(a2, i, a3))
        {
            return 1;
        }
    }
    return 0;
}

MATCH_FUNC(0x4B9830)
s8 Game_0x40::sub_4B9830(Sprite* pCarSprite, s32 a3)
{
    for (u8 i = 0; i < field_23_max_idx; i++)
    {
        if (sub_4B9950(pCarSprite, i, a3) || sub_4B9A10(pCarSprite, i))
        {
            return 1;
        }
    }
    return 0;
}

MATCH_FUNC(0x4B9890)
s8 Game_0x40::sub_4B9890(s16* a2, s32 a3)
{
    for (u8 i = 0; i < field_23_max_idx; i++)
    {
        if (sub_4B98E0(a2, i, a3))
        {
            return 1;
        }
    }
    return 0;
}

MATCH_FUNC(0x4B98E0)
bool Game_0x40::sub_4B98E0(s16* a2, u8 playerIdx, s32 a4)
{
    if (field_4_players[playerIdx]->field_8E_bInUse)
    {
        if (field_4_players[playerIdx]->field_90_game_camera.sub_435630(a2, a4))
        {
            return true;
        }
        return field_4_players[playerIdx]->field_2D0 && field_4_players[playerIdx]->field_208_aux_game_camera.sub_435630(a2, a4) ? true :
                                                                                                                                   false;
    }
    return false;
}

STUB_FUNC(0x4B9950)
s8 Game_0x40::sub_4B9950(Sprite* pCarSprite, u8 idx, s32 a4)
{
    // TODO: Requires Sprite stubs
    return 0;
}

MATCH_FUNC(0x4B9A10)
bool Game_0x40::sub_4B9A10(Sprite* a2, u8 playerIdx)
{
    if (field_4_players[playerIdx]->field_8E_bInUse)
    {
        if (field_4_players[playerIdx]->field_90_game_camera.sub_4355D0(a2))
        {
            return true;
        }
        return field_4_players[playerIdx]->field_2D0 && field_4_players[playerIdx]->field_208_aux_game_camera.sub_4355D0(a2) ? true : false;
    }
    return false;
}

// https://decomp.me/scratch/PO0RU wip - prob needs Fix16 operators ??
STUB_FUNC(0x4B9A80)
bool Game_0x40::is_point_on_screen_4B9A80(Fix16 a2_fp, Fix16 a3_fp)
{
    for (s32 i = 0; i < field_23_max_idx; i++)
    {
        Player* pPlayer = field_4_players[i];
        if (pPlayer->field_8E_bInUse)
        {
            if (pPlayer->field_90_game_camera.check_camera(a2_fp, a3_fp))
            {
                return true;
            }
            if (pPlayer->field_2D0 && pPlayer->field_208_aux_game_camera.check_camera(a2_fp, a3_fp))
            {
                return true;
            }
        }
    }
    return false;
}

STUB_FUNC(0x4B9B10)
s8 Game_0x40::sub_4B9B10(Fix16_Rect* pBounds)
{
    // wip
    for (u8 i = 0; i < field_23_max_idx; i++)
    {
        Player* pCurPlayer = field_4_players[i];
        if (pCurPlayer->field_8E_bInUse)
        {
            if (pBounds->field_8_top <= pCurPlayer->field_90_game_camera.field_2C_top &&
                pBounds->field_C_bottom >= pCurPlayer->field_90_game_camera.field_28_bottom &&
                pBounds->field_0_left <= pCurPlayer->field_90_game_camera.field_24_left &&
                pBounds->field_4_right >= pCurPlayer->field_90_game_camera.field_20_right)
            {
                return 1;
            }
            if (pCurPlayer->field_2D0 && pBounds->field_8_top <= pCurPlayer->field_208_aux_game_camera.field_2C_top &&
                pBounds->field_C_bottom >= pCurPlayer->field_208_aux_game_camera.field_28_bottom &&
                pBounds->field_0_left <= pCurPlayer->field_208_aux_game_camera.field_24_left &&
                pBounds->field_4_right >= pCurPlayer->field_208_aux_game_camera.field_20_right)
            {
                return 1;
            }
        }
    }
    return 0;
}

MATCH_FUNC(0x4B9BC0)
DrawUnk_0xBC* Game_0x40::IteratePlayerCamera_4B9BC0()
{
    for (field_21_player_camera_idx = 0; field_21_player_camera_idx < field_23_max_idx; field_21_player_camera_idx++)
    {
        if (field_4_players[field_21_player_camera_idx]->field_8E_bInUse)
        {
            field_22 = 0;
            return &field_4_players[field_21_player_camera_idx]->field_90_game_camera;
        }
    }
    return NULL;
}

MATCH_FUNC(0x4B9C10)
s8 Game_0x40::sub_4B9C10(Car_BC* a2)
{
    Player* pPlayer = sub_4B9CD0();
    while (pPlayer)
    {
        if (pPlayer->sub_564610(a2, 1))
        {
            return 1;
        }
        pPlayer = IterateNextPlayer_4B9D10();
    }
    return 0;
}

MATCH_FUNC(0x4B9C50)
DrawUnk_0xBC* Game_0x40::sub_4B9C50()
{
    if (!field_4_players[field_21_player_camera_idx]->field_2D0 || field_22)
    {
        while (++field_21_player_camera_idx < field_23_max_idx)
        {
            if (field_4_players[field_21_player_camera_idx]->field_8E_bInUse)
            {
                field_22 = 0;
                return &field_4_players[field_21_player_camera_idx]->field_90_game_camera;
            }
        }
        return 0;
    }
    else
    {
        field_22 = 1;
        return &field_4_players[field_21_player_camera_idx]->field_208_aux_game_camera;
    }
}

MATCH_FUNC(0x4B9CD0)
Player* Game_0x40::sub_4B9CD0()
{
    for (field_20_idx = 0; field_20_idx < field_23_max_idx; field_20_idx++)
    {
        if (field_4_players[field_20_idx]->field_8E_bInUse)
        {
            return field_4_players[field_20_idx];
        }
    }
    return 0;
}

MATCH_FUNC(0x4B9D10)
Player* Game_0x40::IterateNextPlayer_4B9D10()
{
    while (++field_20_idx < field_23_max_idx)
    {
        if (field_4_players[field_20_idx]->field_8E_bInUse)
        {
            return field_4_players[field_20_idx];
        }
    }
    return 0;
}

MATCH_FUNC(0x4B9D60)
void Game_0x40::sub_4B9D60(Sprite* a2, Player* pExclude)
{
    for (u8 i = 0; i < GTA2_COUNTOF(field_4_players); i++)
    {
        Player* p = gGame_0x40_67E008->field_4_players[i];
        if (p && p != pExclude)
        {
            if (gGame_0x40_67E008->sub_4B9950(a2, i, dword_67DFB4))
            {
                if (p->field_680 > p->field_682)
                {
                    p->field_680 -= p->field_682;
                }
                else
                {
                    p->field_680 = 0;
                }
            }
        }
    }
}

EXPORT_VAR Fix16 dword_67DCCC; //  not sure if it's fix16
GLOBAL(dword_67DCCC, 0x67DCCC);

MATCH_FUNC(0x4B9DE0)
Game_0x40::Game_0x40(u8 max_players, s8 player_idx) // 4B9DE0
{
    stru_6F6784.srand_4F7A40(); // srand?

    field_3C_bSkipPolice = bSkip_police_67D4F9;

    for (s32 ii = 0; ii < GTA2_COUNTOF(field_4_players); ii++)
    {
        field_4_players[ii] = 0;
    }

    field_23_max_idx = max_players;
    field_24_cur_idx = player_idx;
    for (u32 i = 0; i < field_23_max_idx; i++)
    {
        field_4_players[i] = new Player(i); // ctor call
        if (!field_4_players[i])
        {
            FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1784);
        }
    }
    Player* pPlayer = field_4_players[field_24_cur_idx];
    field_38_orf1 = pPlayer;
    pPlayer->field_0 = 1;
    field_1C_unk = field_38_orf1;

    rng_dword_67AB34 = new rng(); // inline
    if (!rng_dword_67AB34)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1793);
    }

    gText_0x14_704DFC = new text_0x14(); // ctor call
    if (!gText_0x14_704DFC)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1795);
    }

    gGtx_0x106C_703DD4 = new gtx_0x106C(); // ctor call
    if (!gGtx_0x106C_703DD4)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1803);
    }

    gMap_0x370_6F6268 = new Map_0x370(); // ctor call
    if (!gMap_0x370_6F6268)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1806);
    }

    gpMapRenderer_6F66E4 = new MapRenderer(); // inlined ok?
    if (!gpMapRenderer_6F66E4)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1808);
    }

    gMontana_67B580 = new Montana(); // ctor call
    if (!gMontana_67B580)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1810);
    }

    gMike_A80_6F7328 = new Mike_A80(); // inlined
    if (!gMike_A80_6F7328)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1824);
    }

    gCar_6C_677930 = new Car_6C(); // ctor call
    if (!gCar_6C_677930)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1827);
    }

    gCar_214_705F20 = new Car_214(); // no ctor
    if (!gCar_214_705F20)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1829);
    }

    gfrosty_pasteur_6F8060 = new frosty_pasteur_0xC1EA8(); // ctor call
    if (!gfrosty_pasteur_6F8060)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1831);
    }

    gFrismo_25C_6F8068 = new Frismo_25C(); // multi level inlines
    if (!gFrismo_25C_6F8068)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1833);
    }

    gsharp_bose_0x54_7055D4 = new sharp_bose_0x54(); // ctor call
    if (!gsharp_bose_0x54_7055D4)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1836);
    }

    gSprite_8_703820 = new Sprite_8(); // ctor call
    if (!gSprite_8_703820)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1839);
    }

    gChar_C_6787BC = new Char_C(); // ctor call
    if (!gChar_C_6787BC)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1841);
    }

    gCollide_C_6791FC = new Collide_C(); // ctor call
    if (!gCollide_C_6791FC)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1843);
    }

    gPhi_8CA8_6FCF00 = new Phi_8CA8(); // ctor call
    if (!gPhi_8CA8_6FCF00)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1845);
    }

    gObject_5C_6F8F84 = new Object_5C(); // ctor call
    if (!gObject_5C_6F8F84)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1847);
    }

    gVarrok_7F8_703398 = new Varrok_7F8(); // ctor call
    if (!gVarrok_7F8_703398)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1849);
    }

    gTileAnim_2_7052C4 = new TileAnim_2(); // ctor call
    if (!gTileAnim_2_7052C4)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1851);
    }

    gWeapon_8_707018 = new Weapon_8(); // ctor call
    if (!gWeapon_8_707018)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1853);
    }

    gDoor_4D4_67BD2C = new Door_4D4(); // ctor call ok
    if (!gDoor_4D4_67BD2C)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1855);
    }

    gAmbulance_110_6F70A8 = new Ambulance_110(); // multi level inline
    if (!gAmbulance_110_6F70A8)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1857);
    }

    gSero_181C_6FF1D4 = new Sero_181C(); // ctor call
    if (!gSero_181C_6FF1D4)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1860);
    }

    gTaxi_4_704130 = new Taxi_4(); // ctor call
    if (!gTaxi_4_704130)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1863);
    }

    gGarox_2B00_706620 = new Hud_2B00(); // ctor call
    if (!gGarox_2B00_706620)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1865);
    }

    gSharp_pare_0x15D8_705064 = new sharp_pare_0x15D8(); // ctor call
    if (!gSharp_pare_0x15D8_705064)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1867);
    }

    gTrafficLights_194_705958 = new TrafficLights_194(); // ctor call
    if (!gTrafficLights_194_705958)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1869);
    }

    gRouteFinder_6FFDC8 = new RouteFinder(); // ctor call
    if (!gRouteFinder_6FFDC8)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1871);
    }

    gMarz_1D7E_6FD784 = new Marz_1D7E(); // ctor call
    if (!gMarz_1D7E_6FD784)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1873);
    }

    gOrca_2FD4_6FDEF0 = new Orca_2FD4(); // inline
    if (!gOrca_2FD4_6FDEF0)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1875);
    }

    gCarInfo_808_678098 = new CarInfo_808(); // ctor call
    if (!gCarInfo_808_678098)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1877);
    }

    gParticle_8_6FD5E8 = new Particle_8(); // ctor call
    if (!gParticle_8_6FD5E8)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1879);
    }

    gWolfy_3D4_6FD5EC = new Wolfy_3D4(); // ctor call
    if (!gWolfy_3D4_6FD5EC)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1881);
    }

    gWolfy_7A8_6FD5F0 = new Wolfy_7A8(); // ctor call
    if (!gWolfy_7A8_6FD5F0)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1883);
    }

    gZheal_D9C_679FD4 = new Zheal_D9C(); // ctor call
    if (!gZheal_D9C_679FD4)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1885);
    }

    gSnooky_94_67A830 = new Snooky_94(); // ctor call
    if (!gSnooky_94_67A830)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1887);
    }

    gMaccies_14AC_67E5D0 = new Maccies_14AC(); // ctor call
    if (!gMaccies_14AC_67E5D0)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1889);
    }

    gKfc_1E0_706280 = new Kfc_1E0(); // multi level inline
    if (!gKfc_1E0_706280)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1891);
    }

    gPolice_7B8_6FEE40 = new Police_7B8(); // multi level inline
    if (!gPolice_7B8_6FEE40)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1893);
    }

    gLight_1D4CC_6F5520 = new Light_1D4CC(); // multi level inline
    if (!gLight_1D4CC_6F5520)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1896);
    }

    gZones_CA8_67E274 = new Zones_CA8(); // multi level inline
    if (!gZones_CA8_67E274)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1899);
    }

    gGarage_48_6FD26C = new Garage_48(); // ctor call
    if (!gGarage_48_6FD26C)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1901);
    }

    gHamburger_500_678E30 = new Hamburger_500(); // ctor call
    if (!gHamburger_500_678E30)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1903);
    }

    if (!bExplodingOff_67D4FB)
    {
        gExplodingScore_100_702F34 = new ExplodingScore_100(); // ctor call
        if (!gExplodingScore_100_702F34)
        {
            FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1909);
        }
    }

    gShooey_CC_67A4B8 = new Shooey_CC(); // ctor call
    if (!gShooey_CC_67A4B8)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1913);
    }

    gTango_54_67D4C0 = new Tango_54(); // multi level inline 4A88D0 func only
    if (!gTango_54_67D4C0)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1915);
    }

    gRozza_C88_66AFE0 = new Rozza_C88(); // ctor call
    if (!gRozza_C88_66AFE0)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1918);
    }

    if (gText_0x14_704DFC->field_10_lang_code == 'j')
    {
        gMagical_germain_0x8EC_6F5168 = new magical_germain_0x8EC(); // ctor call
        if (!gMagical_germain_0x8EC_6F5168)
        {
            FatalError_4A38C0(32, "C:\\Splitting\\Gta2\\Source\\game.cpp", 1923);
        }
    }

    field_21_player_camera_idx = 0;
    field_22 = 0;
    field_0_game_state = 1;
    byte_679C0A = 0;
    field_20_idx = 0;
    field_28_timer = -1;
    field_30 = 0;
    field_2C_main_state = 0;
    field_34 = 0;
    if (!bSkip_audio_67D6BE)
    {
        gRoot_sound_66B038.sub_40F010();
    }
}

MATCH_FUNC(0x4BAE30)
Game_0x40::~Game_0x40()
{
    bSkip_police_67D4F9 = field_3C_bSkipPolice;

    if (!bSkip_audio_67D6BE)
    {
        gRoot_sound_66B038.sub_40F020();
    }

    for (u32 i = 0; i < field_23_max_idx; i++)
    {
        GTA2_DELETE_AND_NULL(field_4_players[i]);
    }

    GTA2_DELETE_AND_NULL(rng_dword_67AB34);
    GTA2_DELETE_AND_NULL(gText_0x14_704DFC);
    GTA2_DELETE_AND_NULL(gGtx_0x106C_703DD4);
    GTA2_DELETE_AND_NULL(gMap_0x370_6F6268);
    GTA2_DELETE_AND_NULL(gpMapRenderer_6F66E4);
    GTA2_DELETE_AND_NULL(gMontana_67B580);
    GTA2_DELETE_AND_NULL(gChar_203AC_6787B8);

    GTA2_DELETE_AND_NULL(gCar_6C_677930);
    GTA2_DELETE_AND_NULL(gCar_214_705F20);
    GTA2_DELETE_AND_NULL(gfrosty_pasteur_6F8060);
    GTA2_DELETE_AND_NULL(gFrismo_25C_6F8068);

    GTA2_DELETE_AND_NULL(gPhi_8CA8_6FCF00);
    GTA2_DELETE_AND_NULL(gObject_5C_6F8F84);

    GTA2_DELETE_AND_NULL(gChar_C_6787BC);
    GTA2_DELETE_AND_NULL(gMike_A80_6F7328);

    GTA2_DELETE_AND_NULL(gsharp_bose_0x54_7055D4);

    GTA2_DELETE_AND_NULL(gSprite_8_703820);
    GTA2_DELETE_AND_NULL(gCollide_C_6791FC);
    GTA2_DELETE_AND_NULL(gVarrok_7F8_703398);
    GTA2_DELETE_AND_NULL(gSero_181C_6FF1D4);
    GTA2_DELETE_AND_NULL(gTaxi_4_704130);
    GTA2_DELETE_AND_NULL(gTileAnim_2_7052C4);
    GTA2_DELETE_AND_NULL(gWeapon_8_707018);
    GTA2_DELETE_AND_NULL(gDoor_4D4_67BD2C);
    GTA2_DELETE_AND_NULL(gAmbulance_110_6F70A8);
    GTA2_DELETE_AND_NULL(gGarox_2B00_706620);
    GTA2_DELETE_AND_NULL(gSharp_pare_0x15D8_705064);

    GTA2_DELETE_AND_NULL(gTrafficLights_194_705958);
    GTA2_DELETE_AND_NULL(gRouteFinder_6FFDC8);

    GTA2_DELETE_AND_NULL(gMarz_1D7E_6FD784);

    GTA2_DELETE_AND_NULL(gOrca_2FD4_6FDEF0);

    GTA2_DELETE_AND_NULL(gCarInfo_808_678098);

    GTA2_DELETE_AND_NULL(gParticle_8_6FD5E8);
    GTA2_DELETE_AND_NULL(gWolfy_3D4_6FD5EC);
    GTA2_DELETE_AND_NULL(gWolfy_7A8_6FD5F0);
    GTA2_DELETE_AND_NULL(gZheal_D9C_679FD4);
    GTA2_DELETE_AND_NULL(gSnooky_94_67A830);
    GTA2_DELETE_AND_NULL(gMaccies_14AC_67E5D0);

    GTA2_DELETE_AND_NULL(gKfc_1E0_706280);
    GTA2_DELETE_AND_NULL(gPolice_7B8_6FEE40);
    GTA2_DELETE_AND_NULL(gLight_1D4CC_6F5520);
    GTA2_DELETE_AND_NULL(gZones_CA8_67E274);
    GTA2_DELETE_AND_NULL(gGarage_48_6FD26C);
    GTA2_DELETE_AND_NULL(gHamburger_500_678E30);

    if (!bExplodingOff_67D4FB)
    {
        GTA2_DELETE_AND_NULL(gExplodingScore_100_702F34);
    }

    GTA2_DELETE_AND_NULL(gShooey_CC_67A4B8);
    GTA2_DELETE_AND_NULL(gTango_54_67D4C0);
    GTA2_DELETE_AND_NULL(gRozza_C88_66AFE0);

    if (gMagical_germain_0x8EC_6F5168)
    {
        GTA2_DELETE_AND_NULL(gMagical_germain_0x8EC_6F5168);
    }

    field_1C_unk = 0;
    field_38_orf1 = 0;
}
