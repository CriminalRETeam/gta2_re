#include "Function.hpp"
#include "debug.hpp"
#include "registry.hpp"
#include <stdio.h>

bool gDo_infinite_lives_67D4C9;
bool bLimit_recycling_67D4CA;
bool bDo_invulnerable_67D4CB;
bool bLog_input_67D4CC;
bool bSkip_right_67D4E4;
bool bSkip_mission_67D4E5;
bool bLog_collisions_67D4E6;
bool bSkip_bottom_67D4E7;
bool bSkip_quit_confirm_67D4E8;
bool bDo_iain_test_67D4E9;
bool bSkip_draw_67D4EA;
bool bDo_release_replay_67D4EB;
bool bSkip_traffic_lights_67D4EC;
bool byte_67D4ED;
bool bDo_corner_window_67D4EE;
bool bSkip_dummies_67D4EF;
bool bDo_load_savegame_67D4F0;
bool byte_67D4F1;
bool bDo_show_horn_67D4F2;
bool bIgnore_replay_header_67D4F3;
bool bPlay_replay_67D4F4;
bool bDont_get_car_back_67D4F5;
bool byte_67D4F6;
bool byte_67D4F7;
bool bDo_test_67D4F8;
bool bSkip_police_67D4F9;
bool bDo_show_traffic_lights_info_67D4FA;
bool bExplodingOff_67D4FB;
bool bShow_brief_number_67D504;
bool bSkip_slopes_67D505;
bool bSkip_user_67D506;
bool bSkip_fire_engines_67D53A;
bool bSkip_frontend_67D53B;
bool bIsFrench_67D53C;
bool bDo_brian_test_67D544;
bool bSkip_lid_67D546;
bool bShow_player_names_67D54C;
bool bKeep_weapons_after_death_67D54D;
bool bSkip_window_check_67D54E;
bool bDo_show_physics_67D54F;
bool bSkip_trains_67D550;
bool bSkip_buses_67D558;
bool bDo_show_ids_67D559;
bool bDo_police_1_67D568;
bool bDo_police_2_67D569;
bool bDo_police_3_67D56A;
bool bSkip_top_67D574;
bool bSkip_recycling_67D575;
bool gDo_show_input_67D576;
bool bSkip_skidmarks_67D585;
bool bNo_annoying_chars_67D586;
bool bDo_show_imaginary_67D588;
bool bDo_show_camera_67D58A;
bool bSkip_replay_sync_check_67D594;
bool bDo_show_junc_ids_67D5B0;
bool bLog_random_extra_67D5BC;
bool bConstant_replay_save_67D5C4;
bool bDo_blood_67D5C5;
bool bDo_mike_67D5CC;
bool bShow_hidden_faces_67D5CD;
bool bLog_random_67D5FC;
bool bDo_show_instruments_67D64C;
bool bSkip_particles_67D64D;
bool bSkip_tiles_67D655;
bool bGet_all_weapons_67D684;
bool gDo_miss_logging_67D6BC;
bool gShow_cycle_67D6BD;
bool bSkip_audio_67D6BE;
bool bSkip_left_67D6BF;
bool bLog_directinput_67D6C0;
bool bDo_sync_check_67D6C1;
bool bDo_3d_sound_67D6C2;
bool bSkip_ambulance_67D6C9;
bool bDo_show_object_ids_67D6CA;
bool bDo_free_shopping_67D6CD;
bool bDo_debug_keys_67D6CF;
bool gDo_text_id_test_67D6D0;
bool bLog_routefinder_67D6D1;
bool bDo_show_timing_67D6DC;
bool bDo_exit_after_replay_67D6E4;
bool bDo_show_collision_box_67D6E5;
bool gDo_show_counter_67D6E6;
bool bShow_all_arrows_67D6E7;
bool bDo_kill_phones_on_answer_67D6E8;

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

    FILE*v5 = fopen("data\\i.gxt", "rb");
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

Debug gDebug_67D52C;
