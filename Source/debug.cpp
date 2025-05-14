#include "debug.hpp"
#include "Function.hpp"
#include "registry.hpp"
#include "Globals.hpp"
#include <stdio.h>

EXPORT_VAR bool gDo_infinite_lives_67D4C9;
GLOBAL(gDo_infinite_lives_67D4C9, 0x67D4C9);

EXPORT_VAR bool bLimit_recycling_67D4CA;
GLOBAL(bLimit_recycling_67D4CA, 0x67D4CA);


EXPORT_VAR bool bDo_invulnerable_67D4CB;
GLOBAL(bDo_invulnerable_67D4CB, 0x67D4CB);

EXPORT_VAR bool bLog_input_67D4CC;
GLOBAL(bLog_input_67D4CC, 0x67D4CC);

EXPORT_VAR bool bSkip_right_67D4E4;
GLOBAL(bSkip_right_67D4E4, 0x67D4E4);


EXPORT_VAR bool bSkip_mission_67D4E5;
GLOBAL(bSkip_mission_67D4E5, 0x67D4E5);

EXPORT_VAR bool bLog_collisions_67D4E6;
GLOBAL(bLog_collisions_67D4E6, 0x67D4E6);

EXPORT_VAR bool bSkip_bottom_67D4E7;
GLOBAL(bSkip_bottom_67D4E7, 0x67D4E7);

EXPORT_VAR bool bSkip_quit_confirm_67D4E8;
GLOBAL(bSkip_quit_confirm_67D4E8, 0x67D4E8);

EXPORT_VAR bool bDo_iain_test_67D4E9;
GLOBAL(bDo_iain_test_67D4E9, 0x67D4E9);

EXPORT_VAR bool bSkip_draw_67D4EA;
GLOBAL(bSkip_draw_67D4EA, 0x67D4EA);

EXPORT_VAR bool bDo_release_replay_67D4EB;
GLOBAL(bDo_release_replay_67D4EB, 0x67D4EB);

EXPORT_VAR bool bSkip_traffic_lights_67D4EC;
GLOBAL(bSkip_traffic_lights_67D4EC, 0x67D4EC);

EXPORT_VAR bool bDo_corner_window_67D4EE;
GLOBAL(bDo_corner_window_67D4EE, 0x67D4EE);

EXPORT_VAR bool bSkip_dummies_67D4EF;
GLOBAL(bSkip_dummies_67D4EF, 0x67D4EF);

EXPORT_VAR bool bDo_load_savegame_67D4F0;
GLOBAL(bDo_load_savegame_67D4F0, 0x67D4F0);

EXPORT_VAR bool bDo_show_horn_67D4F2;
GLOBAL(bDo_show_horn_67D4F2, 0x67D4F2);

EXPORT_VAR bool bIgnore_replay_header_67D4F3;
GLOBAL(bIgnore_replay_header_67D4F3, 0x67D4F3);

EXPORT_VAR bool bPlay_replay_67D4F4;
GLOBAL(bPlay_replay_67D4F4, 0x67D4F4);

EXPORT_VAR bool bDont_get_car_back_67D4F5;
GLOBAL(bDont_get_car_back_67D4F5, 0x67D4F5);

EXPORT_VAR bool bDo_test_67D4F8;
GLOBAL(bDo_test_67D4F8, 0x67D4F8);

EXPORT_VAR bool bSkip_police_67D4F9;
GLOBAL(bSkip_police_67D4F9, 0x67D4F9);

EXPORT_VAR bool bDo_show_traffic_lights_info_67D4FA;
GLOBAL(bDo_show_traffic_lights_info_67D4FA, 0x67D4FA);

EXPORT_VAR bool bExplodingOff_67D4FB;
GLOBAL(bExplodingOff_67D4FB, 0x67D4FB);

EXPORT_VAR bool bShow_brief_number_67D504;
GLOBAL(bShow_brief_number_67D504, 0x67D504);

EXPORT_VAR bool bSkip_slopes_67D505;
GLOBAL(bSkip_slopes_67D505, 0x67D505);

EXPORT_VAR bool bSkip_user_67D506;
GLOBAL(bSkip_user_67D506, 0x67D506);

EXPORT_VAR bool bSkip_fire_engines_67D53A;
GLOBAL(bSkip_fire_engines_67D53A, 0x67D53A);

EXPORT_VAR bool bSkip_frontend_67D53B;
GLOBAL(bSkip_frontend_67D53B, 0x67D53B);

EXPORT_VAR bool bIsFrench_67D53C;
GLOBAL(bIsFrench_67D53C, 0x67D53C);

EXPORT_VAR bool bDo_brian_test_67D544;
GLOBAL(bDo_brian_test_67D544, 0x67D544);

EXPORT_VAR bool bSkip_lid_67D546;
GLOBAL(bSkip_lid_67D546, 0x67D546);

EXPORT_VAR bool bShow_player_names_67D54C;
GLOBAL(bShow_player_names_67D54C, 0x67D54C);

EXPORT_VAR bool bKeep_weapons_after_death_67D54D;
GLOBAL(bKeep_weapons_after_death_67D54D, 0x67D54D);

EXPORT_VAR bool bSkip_window_check_67D54E;
GLOBAL(bSkip_window_check_67D54E, 0x67D54E);

EXPORT_VAR bool bDo_show_physics_67D54F;
GLOBAL(bDo_show_physics_67D54F, 0x67D54F);

EXPORT_VAR bool bSkip_trains_67D550;
GLOBAL(bSkip_trains_67D550, 0x67D550);

EXPORT_VAR bool bSkip_buses_67D558;
GLOBAL(bSkip_buses_67D558, 0x67D558);

EXPORT_VAR bool bDo_show_ids_67D559;
GLOBAL(bDo_show_ids_67D559, 0x67D559);

EXPORT_VAR bool bDo_police_1_67D568;
GLOBAL(bDo_police_1_67D568, 0x67D568);

EXPORT_VAR bool bDo_police_2_67D569;
GLOBAL(bDo_police_2_67D569, 0x67D569);

EXPORT_VAR bool bDo_police_3_67D56A;
GLOBAL(bDo_police_3_67D56A, 0x67D56A);

EXPORT_VAR bool bSkip_top_67D574;
GLOBAL(bSkip_top_67D574, 0x67D574);

EXPORT_VAR bool bSkip_recycling_67D575;
GLOBAL(bSkip_recycling_67D575, 0x67D575);

EXPORT_VAR bool gDo_show_input_67D576;
GLOBAL(gDo_show_input_67D576, 0x67D576);

EXPORT_VAR bool bSkip_skidmarks_67D585;
GLOBAL(bSkip_skidmarks_67D585, 0x67D585);

EXPORT_VAR bool bNo_annoying_chars_67D586;
GLOBAL(bNo_annoying_chars_67D586, 0x67D586);

EXPORT_VAR bool bDo_show_imaginary_67D588;
GLOBAL(bDo_show_imaginary_67D588, 0x67D588);

EXPORT_VAR bool bDo_show_camera_67D58A;
GLOBAL(bDo_show_camera_67D58A, 0x67D58A);

EXPORT_VAR bool bSkip_replay_sync_check_67D594;
GLOBAL(bSkip_replay_sync_check_67D594, 0x67D594);

EXPORT_VAR bool bDo_show_junc_ids_67D5B0;
GLOBAL(bDo_show_junc_ids_67D5B0, 0x67D5B0);

EXPORT_VAR bool bLog_random_extra_67D5BC;
GLOBAL(bLog_random_extra_67D5BC, 0x67D5BC);

EXPORT_VAR bool bConstant_replay_save_67D5C4;
GLOBAL(bConstant_replay_save_67D5C4, 0x67D5C4);

EXPORT_VAR bool bDo_blood_67D5C5;
GLOBAL(bDo_blood_67D5C5, 0x67D5C5);

EXPORT_VAR bool bDo_mike_67D5CC;
GLOBAL(bDo_mike_67D5CC, 0x67D5CC);

EXPORT_VAR bool bShow_hidden_faces_67D5CD;
GLOBAL(bShow_hidden_faces_67D5CD, 0x67D5CD);

EXPORT_VAR bool bLog_random_67D5FC;
GLOBAL(bLog_random_67D5FC, 0x67D5FC);

EXPORT_VAR bool bDo_show_instruments_67D64C;
GLOBAL(bDo_show_instruments_67D64C, 0x67D64C);

EXPORT_VAR bool bSkip_particles_67D64D;
GLOBAL(bSkip_particles_67D64D, 0x67D64D);

EXPORT_VAR bool bSkip_tiles_67D655;
GLOBAL(bSkip_tiles_67D655, 0x67D655);

EXPORT_VAR bool bGet_all_weapons_67D684;
GLOBAL(bGet_all_weapons_67D684, 0x67D684);

EXPORT_VAR bool gDo_miss_logging_67D6BC;
GLOBAL(gDo_miss_logging_67D6BC, 0x67D6BC);

EXPORT_VAR bool gShow_cycle_67D6BD;
GLOBAL(gShow_cycle_67D6BD, 0x67D6BD);

EXPORT_VAR bool bSkip_audio_67D6BE;
GLOBAL(bSkip_audio_67D6BE, 0x67D6BE);

EXPORT_VAR bool bSkip_left_67D6BF;
GLOBAL(bSkip_left_67D6BF, 0x67D6BF);

EXPORT_VAR bool bLog_directinput_67D6C0;
GLOBAL(bLog_directinput_67D6C0, 0x67D6C0);

EXPORT_VAR bool bDo_sync_check_67D6C1;
GLOBAL(bDo_sync_check_67D6C1, 0x67D6C1);

EXPORT_VAR bool bDo_3d_sound_67D6C2;
GLOBAL(bDo_3d_sound_67D6C2, 0x67D6C2);

EXPORT_VAR bool bSkip_ambulance_67D6C9;
GLOBAL(bSkip_ambulance_67D6C9, 0x67D6C9);

EXPORT_VAR bool bDo_show_object_ids_67D6CA;
GLOBAL(bDo_show_object_ids_67D6CA, 0x67D6CA);

EXPORT_VAR bool bDo_free_shopping_67D6CD;
GLOBAL(bDo_free_shopping_67D6CD, 0x67D6CD);

EXPORT_VAR bool bDo_debug_keys_67D6CF;
GLOBAL(bDo_debug_keys_67D6CF, 0x67D6CF);

EXPORT_VAR bool gDo_text_id_test_67D6D0;
GLOBAL(gDo_text_id_test_67D6D0, 0x67D6D0);

EXPORT_VAR bool bLog_routefinder_67D6D1;
GLOBAL(bLog_routefinder_67D6D1, 0x67D6D1);

EXPORT_VAR bool bDo_show_timing_67D6DC;
GLOBAL(bDo_show_timing_67D6DC, 0x67D6DC);

EXPORT_VAR bool bDo_exit_after_replay_67D6E4;
GLOBAL(bDo_exit_after_replay_67D6E4, 0x67D6E4);

EXPORT_VAR bool bDo_show_collision_box_67D6E5;
GLOBAL(bDo_show_collision_box_67D6E5, 0x67D6E5);

EXPORT_VAR bool gDo_show_counter_67D6E6;
GLOBAL(gDo_show_counter_67D6E6, 0x67D6E6);

EXPORT_VAR bool bShow_all_arrows_67D6E7;
GLOBAL(bShow_all_arrows_67D6E7, 0x67D6E7);

EXPORT_VAR bool bDo_kill_phones_on_answer_67D6E8;
GLOBAL(bDo_kill_phones_on_answer_67D6E8, 0x67D6E8);

MATCH_FUNC(0x4ABBD0)
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

MATCH_FUNC(0x4AC200)
bool Debug::HaveOnlyFrenchGtx_4AC200()
{
    bool bEGtxExists = false;
    bool bFgxtExists = false;
    bool bGGtxExists = false;
    bool bIGtxExists = false;
    bool bJGtxExists = false;
    bool bSGtxExists = false;

    FILE* v2 = fopen("data\\e.gxt", "rb");
    if (v2)
    {
        fclose(v2);
        bEGtxExists = true;
    }

    FILE* v3 = fopen("data\\f.gxt", "rb");
    if (v3)
    {
        fclose(v3);
        bFgxtExists = true;
    }

    FILE* v4 = fopen("data\\g.gxt", "rb");
    if (v4)
    {
        fclose(v4);
        bGGtxExists = true;
    }

    FILE* v5 = fopen("data\\i.gxt", "rb");
    if (v5)
    {
        fclose(v5);
        bIGtxExists = true;
    }

    FILE* v6 = fopen("data\\j.gxt", "rb");
    if (v6)
    {
        fclose(v6);
        bJGtxExists = true;
    }

    FILE* v7 = fopen("data\\s.gxt", "rb");
    if (v7)
    {
        fclose(v7);
        bSGtxExists = true;
    }

    bool r = (!bEGtxExists && bFgxtExists == true && !bGGtxExists && !bIGtxExists && !bJGtxExists && !bSGtxExists);

    return r;
}

EXPORT_VAR Debug gDebug_67D52C;
GLOBAL(gDebug_67D52C, 0x67D52C);
