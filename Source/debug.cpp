#include "debug.hpp"
#include "Function.hpp"
#include "registry.hpp"
#include "Globals.hpp"
#include "file.hpp"
#include <stdio.h>

// GLOBAL: 105 0x67D4C9
DEFINE_GLOBAL(bool, gDo_infinite_lives_67D4C9);
// GLOBAL: 105 0x67D4CA
DEFINE_GLOBAL(bool, bLimit_recycling_67D4CA);
// GLOBAL: 105 0x67D4CB
DEFINE_GLOBAL(bool, bDo_invulnerable_67D4CB);
// GLOBAL: 105 0x67D4CC
DEFINE_GLOBAL(bool, bLog_input_67D4CC);
// GLOBAL: 105 0x67D4E4
DEFINE_GLOBAL(bool, bSkip_right_67D4E4);
// GLOBAL: 105 0x67D4E5
DEFINE_GLOBAL(bool, bSkip_mission_67D4E5);
// GLOBAL: 105 0x67D4E6
DEFINE_GLOBAL(bool, bLog_collisions_67D4E6);
// GLOBAL: 105 0x67D4E7
DEFINE_GLOBAL(bool, bSkip_bottom_67D4E7);
// GLOBAL: 105 0x67D4E8
DEFINE_GLOBAL(bool, bSkip_quit_confirm_67D4E8);
// GLOBAL: 105 0x67D4E9
DEFINE_GLOBAL(bool, bDo_iain_test_67D4E9);
// GLOBAL: 105 0x67D4EA
DEFINE_GLOBAL(bool, bSkip_draw_67D4EA);
// GLOBAL: 105 0x67D4EB
DEFINE_GLOBAL(bool, bDo_release_replay_67D4EB);
// GLOBAL: 105 0x67D4EC
DEFINE_GLOBAL(bool, bSkip_traffic_lights_67D4EC);
// GLOBAL: 105 0x67D4EE
DEFINE_GLOBAL(bool, bDo_corner_window_67D4EE);
// GLOBAL: 105 0x67D4EF
DEFINE_GLOBAL(bool, bSkip_dummies_67D4EF);
// GLOBAL: 105 0x67D4F0
DEFINE_GLOBAL(bool, bDo_load_savegame_67D4F0);
// GLOBAL: 105 0x67D4F2
DEFINE_GLOBAL(bool, bDo_show_horn_67D4F2);
// GLOBAL: 105 0x67D4F3
DEFINE_GLOBAL(bool, bIgnore_replay_header_67D4F3);
// GLOBAL: 105 0x67D4F4
DEFINE_GLOBAL(bool, bPlay_replay_67D4F4);
// GLOBAL: 105 0x67D4F5
DEFINE_GLOBAL(bool, bDont_get_car_back_67D4F5);
// GLOBAL: 105 0x67D4F8
DEFINE_GLOBAL(bool, bDo_test_67D4F8);
// GLOBAL: 105 0x67D4F9
DEFINE_GLOBAL(bool, bSkip_police_67D4F9);
// GLOBAL: 105 0x67D4FA
DEFINE_GLOBAL(bool, bDo_show_traffic_lights_info_67D4FA);
// GLOBAL: 105 0x67D4FB
DEFINE_GLOBAL(bool, bExplodingOff_67D4FB);
// GLOBAL: 105 0x67D504
DEFINE_GLOBAL(bool, bShow_brief_number_67D504);
// GLOBAL: 105 0x67D505
DEFINE_GLOBAL(bool, bSkip_slopes_67D505);
// GLOBAL: 105 0x67D506
DEFINE_GLOBAL(bool, bSkip_user_67D506);
// GLOBAL: 105 0x67D53A
DEFINE_GLOBAL(bool, bSkip_fire_engines_67D53A);
// GLOBAL: 105 0x67D53B
DEFINE_GLOBAL(bool, bSkip_frontend_67D53B);
// GLOBAL: 105 0x67D53C
DEFINE_GLOBAL(bool, bIsFrench_67D53C);
// GLOBAL: 105 0x67D544
DEFINE_GLOBAL(bool, bDo_brian_test_67D544);
// GLOBAL: 105 0x67D546
DEFINE_GLOBAL(bool, bSkip_lid_67D546);
// GLOBAL: 105 0x67D54C
DEFINE_GLOBAL(bool, bShow_player_names_67D54C);
// GLOBAL: 105 0x67D54D
DEFINE_GLOBAL(bool, bKeep_weapons_after_death_67D54D);
// GLOBAL: 105 0x67D54E
DEFINE_GLOBAL(bool, bSkip_window_check_67D54E);
// GLOBAL: 105 0x67D54F
DEFINE_GLOBAL(bool, bDo_show_physics_67D54F);
// GLOBAL: 105 0x67D550
DEFINE_GLOBAL(bool, bSkip_trains_67D550);
// GLOBAL: 105 0x67D558
DEFINE_GLOBAL(bool, bSkip_buses_67D558);
// GLOBAL: 105 0x67D559
DEFINE_GLOBAL(bool, bDo_show_ids_67D559);
// GLOBAL: 105 0x67D568
DEFINE_GLOBAL(bool, bDo_police_1_67D568);
// GLOBAL: 105 0x67D569
DEFINE_GLOBAL(bool, bDo_police_2_67D569);
// GLOBAL: 105 0x67D56A
DEFINE_GLOBAL(bool, bDo_police_3_67D56A);
// GLOBAL: 105 0x67D574
DEFINE_GLOBAL(bool, bSkip_top_67D574);
// GLOBAL: 105 0x67D575
DEFINE_GLOBAL(bool, bSkip_recycling_67D575);
// GLOBAL: 105 0x67D576
DEFINE_GLOBAL(bool, gDo_show_input_67D576);
// GLOBAL: 105 0x67D585
DEFINE_GLOBAL(bool, bSkip_skidmarks_67D585);
// GLOBAL: 105 0x67D586
DEFINE_GLOBAL(bool, bNo_annoying_chars_67D586);
// GLOBAL: 105 0x67D588
DEFINE_GLOBAL(bool, bDo_show_imaginary_67D588);
// GLOBAL: 105 0x67D58A
DEFINE_GLOBAL(bool, bDo_show_camera_67D58A);
// GLOBAL: 105 0x67D594
DEFINE_GLOBAL(bool, bSkip_replay_sync_check_67D594);
// GLOBAL: 105 0x67D5B0
DEFINE_GLOBAL(bool, bDo_show_junc_ids_67D5B0);
// GLOBAL: 105 0x67D5BC
DEFINE_GLOBAL(bool, bLog_random_extra_67D5BC);
// GLOBAL: 105 0x67D5C4
DEFINE_GLOBAL(bool, bConstant_replay_save_67D5C4);
// GLOBAL: 105 0x67D5C5
DEFINE_GLOBAL(bool, bDo_blood_67D5C5);
// GLOBAL: 105 0x67D5CC
DEFINE_GLOBAL(bool, bDo_mike_67D5CC);
// GLOBAL: 105 0x67D5CD
DEFINE_GLOBAL(bool, bShow_hidden_faces_67D5CD);
// GLOBAL: 105 0x67D5FC
DEFINE_GLOBAL(bool, bLog_random_67D5FC);
// GLOBAL: 105 0x67D64C
DEFINE_GLOBAL(bool, bDo_show_instruments_67D64C);
// GLOBAL: 105 0x67D64D
DEFINE_GLOBAL(bool, bSkip_particles_67D64D);
// GLOBAL: 105 0x67D655
DEFINE_GLOBAL(bool, bSkip_tiles_67D655);
// GLOBAL: 105 0x67D684
DEFINE_GLOBAL(bool, bGet_all_weapons_67D684);
// GLOBAL: 105 0x67D6BC
DEFINE_GLOBAL(bool, gDo_miss_logging_67D6BC);
// GLOBAL: 105 0x67D6BD
DEFINE_GLOBAL(bool, gShow_cycle_67D6BD);
// GLOBAL: 105 0x67D6BE
DEFINE_GLOBAL(bool, bSkip_audio_67D6BE);
// GLOBAL: 105 0x67D6BF
DEFINE_GLOBAL(bool, bSkip_left_67D6BF);
// GLOBAL: 105 0x67D6C0
DEFINE_GLOBAL(bool, bLog_directinput_67D6C0);
// GLOBAL: 105 0x67D6C1
DEFINE_GLOBAL(bool, bDo_sync_check_67D6C1);
// GLOBAL: 105 0x67D6C2
DEFINE_GLOBAL(bool, bDo_3d_sound_67D6C2);
// GLOBAL: 105 0x67D6C9
DEFINE_GLOBAL(bool, bSkip_ambulance_67D6C9);
// GLOBAL: 105 0x67D6CA
DEFINE_GLOBAL(bool, bDo_show_object_ids_67D6CA);
// GLOBAL: 105 0x67D6CD
DEFINE_GLOBAL(bool, bDo_free_shopping_67D6CD);
// GLOBAL: 105 0x67D6CF
DEFINE_GLOBAL(bool, bDo_debug_keys_67D6CF);
// GLOBAL: 105 0x67D6D0
DEFINE_GLOBAL(bool, gDo_text_id_test_67D6D0);
// GLOBAL: 105 0x67D6D1
DEFINE_GLOBAL(bool, bLog_routefinder_67D6D1);
// GLOBAL: 105 0x67D6DC
DEFINE_GLOBAL(bool, bDo_show_timing_67D6DC);
// GLOBAL: 105 0x67D6E4
DEFINE_GLOBAL(bool, bDo_exit_after_replay_67D6E4);
// GLOBAL: 105 0x67D6E5
DEFINE_GLOBAL(bool, bDo_show_collision_box_67D6E5);
// GLOBAL: 105 0x67D6E6
DEFINE_GLOBAL(bool, gDo_show_counter_67D6E6);
// GLOBAL: 105 0x67D6E7
DEFINE_GLOBAL(bool, bShow_all_arrows_67D6E7);
// GLOBAL: 105 0x67D6E8
DEFINE_GLOBAL(bool, bDo_kill_phones_on_answer_67D6E8);

// FUNCTION: 105 0x4ABBD0
void Debug::Init_4ABBD0()
{
    bPlay_replay_67D4F4 = gRegistry_6FF968.Get_Debug_Setting_586E20("play_replay");
    bConstant_replay_save_67D5C4 = gRegistry_6FF968.Get_Debug_Setting_586E20("constant_replay_save");
    bSkip_dummies_67D4EF = gRegistry_6FF968.Get_Debug_Setting_586E20("skip_dummies");
    bDo_test_67D4F8 = gRegistry_6FF968.Get_Debug_Setting_586E20("do_test");
    bSkip_mission_67D4E5 = gRegistry_6FF968.Get_Debug_Setting_586E20("skip_mission");
    gShow_cycle_67D6BD = gRegistry_6FF968.Get_Debug_Setting_586E20("show_cycle");
    bDo_brian_test_67D544 = gRegistry_6FF968.Get_Debug_Setting_586E20("do_brian_test");
    bDo_iain_test_67D4E9 = gRegistry_6FF968.Get_Debug_Setting_586E20("do_iain_test");
    bSkip_tiles_67D655 = gRegistry_6FF968.Get_Debug_Setting_586E20("skip_tiles");
    gDo_show_counter_67D6E6 = gRegistry_6FF968.Get_Debug_Setting_586E20("do_show_counters");
    bDo_show_camera_67D58A = gRegistry_6FF968.Get_Debug_Setting_586E20("do_show_camera");
    gDo_show_input_67D576 = gRegistry_6FF968.Get_Debug_Setting_586E20("do_show_input");
    bDo_show_timing_67D6DC = gRegistry_6FF968.Get_Debug_Setting_586E20("do_show_timing");
    bDo_show_collision_box_67D6E5 = gRegistry_6FF968.Get_Debug_Setting_586E20("do_show_collision_box");
    bDo_show_physics_67D54F = gRegistry_6FF968.Get_Debug_Setting_586E20("do_show_physics");
    bDo_show_imaginary_67D588 = gRegistry_6FF968.Get_Debug_Setting_586E20("do_show_imaginary");
    bSkip_user_67D506 = gRegistry_6FF968.Get_Debug_Setting_586E20("skip_user");
    bSkip_traffic_lights_67D4EC = gRegistry_6FF968.Get_Debug_Setting_586E20("skip_traffic_lights");
    bSkip_recycling_67D575 = gRegistry_6FF968.Get_Debug_Setting_586E20("skip_recycling");
    bLog_collisions_67D4E6 = gRegistry_6FF968.Get_Debug_Setting_586E20("log_collisions");
    bDo_show_traffic_lights_info_67D4FA = gRegistry_6FF968.Get_Debug_Setting_586E20("do_show_traffic_lights_info");
    bDo_show_ids_67D559 = gRegistry_6FF968.Get_Debug_Setting_586E20("do_show_ids");
    bLimit_recycling_67D4CA = gRegistry_6FF968.Get_Debug_Setting_586E20("limit_recycling");
    bNo_annoying_chars_67D586 = gRegistry_6FF968.Get_Debug_Setting_586E20("no_annoying_chars");
    bSkip_slopes_67D505 = gRegistry_6FF968.Get_Debug_Setting_586E20("skip_slopes");
    bSkip_left_67D6BF = gRegistry_6FF968.Get_Debug_Setting_586E20("skip_left");
    bSkip_right_67D4E4 = gRegistry_6FF968.Get_Debug_Setting_586E20("skip_right");
    bSkip_top_67D574 = gRegistry_6FF968.Get_Debug_Setting_586E20("skip_top");
    bSkip_bottom_67D4E7 = gRegistry_6FF968.Get_Debug_Setting_586E20("skip_bottom");
    bSkip_lid_67D546 = gRegistry_6FF968.Get_Debug_Setting_586E20("skip_lid");
    bLog_routefinder_67D6D1 = gRegistry_6FF968.Get_Debug_Setting_586E20("log_routefinder");
    bDo_mike_67D5CC = gRegistry_6FF968.Get_Debug_Setting_586E20("do_mike");
    bSkip_particles_67D64D = gRegistry_6FF968.Get_Debug_Setting_586E20("skip_particles");
    bShow_hidden_faces_67D5CD = gRegistry_6FF968.Get_Debug_Setting_586E20("show_hidden_faces");
    bGet_all_weapons_67D684 = gRegistry_6FF968.Get_Debug_Setting_586E20("get_all_weapons");
    bDo_exit_after_replay_67D6E4 = gRegistry_6FF968.Get_Debug_Setting_586E20("do_exit_after_replay");
    bDont_get_car_back_67D4F5 = gRegistry_6FF968.Get_Debug_Setting_586E20("dont_get_car_back");
    bDo_show_instruments_67D64C = gRegistry_6FF968.Get_Debug_Setting_586E20("do_show_instruments");
    bSkip_ambulance_67D6C9 = gRegistry_6FF968.Get_Debug_Setting_586E20("skip_ambulance");
    bSkip_police_67D4F9 = gRegistry_6FF968.Get_Debug_Setting_586E20("skip_police");
    bSkip_frontend_67D53B = gRegistry_6FF968.Get_Debug_Setting_586E20("skip_frontend");
    bDo_invulnerable_67D4CB = gRegistry_6FF968.Get_Debug_Setting_586E20("do_invulnerable");
    bShow_all_arrows_67D6E7 = gRegistry_6FF968.Get_Debug_Setting_586E20("show_all_arrows");
    bDo_show_horn_67D4F2 = gRegistry_6FF968.Get_Debug_Setting_586E20("do_show_horn");
    bKeep_weapons_after_death_67D54D = gRegistry_6FF968.Get_Debug_Setting_586E20("keep_weapons_after_death");
    bSkip_skidmarks_67D585 = gRegistry_6FF968.Get_Debug_Setting_586E20("skip_skidmarks");
    bDo_show_junc_ids_67D5B0 = gRegistry_6FF968.Get_Debug_Setting_586E20("do_show_junc_ids");
    bDo_corner_window_67D4EE = gRegistry_6FF968.Get_Debug_Setting_586E20("do_corner_window");
    gDo_infinite_lives_67D4C9 = gRegistry_6FF968.Get_Debug_Setting_586E20("do_infinite_lives");
    bDo_load_savegame_67D4F0 = gRegistry_6FF968.Get_Debug_Setting_586E20("do_load_savegame");
    bSkip_audio_67D6BE = gRegistry_6FF968.Get_Debug_Setting_586E20("skip_audio");
    bDo_debug_keys_67D6CF = gRegistry_6FF968.Get_Debug_Setting_586E20("do_debug_keys");
    bLog_random_67D5FC = gRegistry_6FF968.Get_Debug_Setting_586E20("log_random");
    bLog_random_extra_67D5BC = gRegistry_6FF968.Get_Debug_Setting_586E20("log_random_extra");
    bLog_input_67D4CC = gRegistry_6FF968.Get_Debug_Setting_586E20("log_input");
    bLog_directinput_67D6C0 = gRegistry_6FF968.Get_Debug_Setting_586E20("log_directinput");
    bIgnore_replay_header_67D4F3 = gRegistry_6FF968.Get_Debug_Setting_586E20("ignore_replay_header");
    bSkip_trains_67D550 = gRegistry_6FF968.Get_Debug_Setting_586E20("skip_trains");
    bSkip_buses_67D558 = gRegistry_6FF968.Get_Debug_Setting_586E20("skip_buses");
    bSkip_quit_confirm_67D4E8 = gRegistry_6FF968.Get_Debug_Setting_586E20("skip_quit_confirm");
    bDo_sync_check_67D6C1 = gRegistry_6FF968.Get_Debug_Setting_586E20("do_sync_check");
    bSkip_fire_engines_67D53A = gRegistry_6FF968.Get_Debug_Setting_586E20("skip_fire_engines");
    bShow_brief_number_67D504 = gRegistry_6FF968.Get_Debug_Setting_586E20("show_brief_number");
    bSkip_window_check_67D54E = gRegistry_6FF968.Get_Debug_Setting_586E20("skip_window_check");
    bSkip_replay_sync_check_67D594 = gRegistry_6FF968.Get_Debug_Setting_586E20("skip_replay_sync_check");
    bDo_show_object_ids_67D6CA = gRegistry_6FF968.Get_Debug_Setting_586E20("do_show_object_ids");
    bDo_kill_phones_on_answer_67D6E8 = gRegistry_6FF968.Get_Debug_Setting_586E20("do_kill_phones_on_answer");
    gDo_miss_logging_67D6BC = gRegistry_6FF968.Get_Debug_Setting_586E20("do_miss_logging");
    gDo_text_id_test_67D6D0 = gRegistry_6FF968.Get_Debug_Setting_586E20("do_text_id_test");
    bDo_police_1_67D568 = gRegistry_6FF968.Get_Debug_Setting_586E20("do_police_1");
    bDo_police_2_67D569 = gRegistry_6FF968.Get_Debug_Setting_586E20("do_police_2");
    bDo_police_3_67D56A = gRegistry_6FF968.Get_Debug_Setting_586E20("do_police_3");
    bSkip_draw_67D4EA = gRegistry_6FF968.Get_Debug_Setting_586E20("skip_draw");
    bDo_free_shopping_67D6CD = gRegistry_6FF968.Get_Debug_Setting_586E20("do_free_shopping");
    bDo_release_replay_67D4EB = gRegistry_6FF968.Get_Debug_Setting_586E20("do_release_replay");
    bDo_blood_67D5C5 = gRegistry_6FF968.Get_Debug_Setting_586E20("do_blood");
    bDo_3d_sound_67D6C2 = gRegistry_6FF968.Get_Sound_Settting_586A70("do_3d_sound");
    bIsFrench_67D53C = HaveOnlyFrenchGtx_4AC200();
    bShow_player_names_67D54C = gRegistry_6FF968.Set_Network_Setting_587690("show_player_names", 1) != 0;
}

// STRING: 105 0x61f09c
#define E_GTX_STRING "data\\e.gxt"
// STRING: 105 0x61f090
#define F_GTX_STRING "data\\f.gxt"
// STRING: 105 0x61f084
#define G_GTX_STRING "data\\g.gxt"
// STRING: 105 0x61f078
#define I_GTX_STRING "data\\i.gxt"
// STRING: 105 0x61f06c
#define J_GTX_STRING "data\\j.gxt"
// STRING: 105 0x61f060
#define S_GTX_STRING "data\\s.gxt" 

// FUNCTION: 105 0x4AC200
bool Debug::HaveOnlyFrenchGtx_4AC200()
{
    bool bEGtxExists = false;
    bool bFgxtExists = false;
    bool bGGtxExists = false;
    bool bIGtxExists = false;
    bool bJGtxExists = false;
    bool bSGtxExists = false;

    FILE* v2 = fopen(E_GTX_STRING, READ_BINARY_STRING);
    if (v2)
    {
        fclose(v2);
        bEGtxExists = true;
    }

    FILE* v3 = fopen(F_GTX_STRING, READ_BINARY_STRING);
    if (v3)
    {
        fclose(v3);
        bFgxtExists = true;
    }

    FILE* v4 = fopen(G_GTX_STRING, READ_BINARY_STRING);
    if (v4)
    {
        fclose(v4);
        bGGtxExists = true;
    }

    FILE* v5 = fopen(I_GTX_STRING, READ_BINARY_STRING);
    if (v5)
    {
        fclose(v5);
        bIGtxExists = true;
    }

    FILE* v6 = fopen(J_GTX_STRING, READ_BINARY_STRING);
    if (v6)
    {
        fclose(v6);
        bJGtxExists = true;
    }

    FILE* v7 = fopen(S_GTX_STRING, READ_BINARY_STRING);
    if (v7)
    {
        fclose(v7);
        bSGtxExists = true;
    }

    bool r = (!bEGtxExists && bFgxtExists == true && !bGGtxExists && !bIGtxExists && !bJGtxExists && !bSGtxExists);

    return r;
}

// GLOBAL: 105 0x67D52C
DEFINE_GLOBAL(Debug, gDebug_67D52C);
