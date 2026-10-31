#include "debug.hpp"
#include "Function.hpp"
#include "registry.hpp"
#include "Globals.hpp"
#include <stdio.h>

DEFINE_GLOBAL(bool, gDo_infinite_lives, 0x67D4C9);
DEFINE_GLOBAL(bool, bLimit_recycling, 0x67D4CA);
DEFINE_GLOBAL(bool, bDo_invulnerable, 0x67D4CB);
DEFINE_GLOBAL(bool, bLog_input, 0x67D4CC);
DEFINE_GLOBAL(bool, bSkip_right, 0x67D4E4);
DEFINE_GLOBAL(bool, bSkip_mission, 0x67D4E5);
DEFINE_GLOBAL(bool, bLog_collisions, 0x67D4E6);
DEFINE_GLOBAL(bool, bSkip_bottom, 0x67D4E7);
DEFINE_GLOBAL(bool, bSkip_quit_confirm, 0x67D4E8);
DEFINE_GLOBAL(bool, bDo_iain_test, 0x67D4E9);
DEFINE_GLOBAL(bool, bSkip_draw, 0x67D4EA);
DEFINE_GLOBAL(bool, bDo_release_replay, 0x67D4EB);
DEFINE_GLOBAL(bool, bSkip_traffic_lights, 0x67D4EC);
DEFINE_GLOBAL(bool, bDo_corner_window, 0x67D4EE);
DEFINE_GLOBAL(bool, bSkip_dummies, 0x67D4EF);
DEFINE_GLOBAL(bool, bDo_load_savegame, 0x67D4F0);
DEFINE_GLOBAL(bool, bDo_show_horn, 0x67D4F2);
DEFINE_GLOBAL(bool, bIgnore_replay_header, 0x67D4F3);
DEFINE_GLOBAL(bool, bPlay_replay, 0x67D4F4);
DEFINE_GLOBAL(bool, bDont_get_car_back, 0x67D4F5);
DEFINE_GLOBAL(bool, bDo_test, 0x67D4F8);
DEFINE_GLOBAL(bool, bSkip_police, 0x67D4F9);
DEFINE_GLOBAL(bool, bDo_show_traffic_lights_info, 0x67D4FA);
DEFINE_GLOBAL(bool, bExplodingScoresOff, 0x67D4FB);
DEFINE_GLOBAL(bool, bShow_brief_number, 0x67D504);
DEFINE_GLOBAL(bool, bSkip_slopes, 0x67D505);
DEFINE_GLOBAL(bool, bSkip_user, 0x67D506);
DEFINE_GLOBAL(bool, bSkip_fire_engines, 0x67D53A);
DEFINE_GLOBAL(bool, bSkip_frontend, 0x67D53B);
DEFINE_GLOBAL(bool, bIsFrench, 0x67D53C);
DEFINE_GLOBAL(bool, bDo_brian_test, 0x67D544);
DEFINE_GLOBAL(bool, bSkip_lid, 0x67D546);
DEFINE_GLOBAL(bool, bShow_player_names, 0x67D54C);
DEFINE_GLOBAL(bool, bKeep_weapons_after_death, 0x67D54D);
DEFINE_GLOBAL(bool, bSkip_window_check, 0x67D54E);
DEFINE_GLOBAL(bool, bDo_show_physics, 0x67D54F);
DEFINE_GLOBAL(bool, bSkip_trains, 0x67D550);
DEFINE_GLOBAL(bool, bSkip_buses, 0x67D558);
DEFINE_GLOBAL(bool, bDo_show_ids, 0x67D559);
DEFINE_GLOBAL(bool, bDo_police_1, 0x67D568);
DEFINE_GLOBAL(bool, bDo_police_2, 0x67D569);
DEFINE_GLOBAL(bool, bDo_police_3, 0x67D56A);
DEFINE_GLOBAL(bool, bSkip_top, 0x67D574);
DEFINE_GLOBAL(bool, bSkip_recycling, 0x67D575);
DEFINE_GLOBAL(bool, gDo_show_input, 0x67D576);
DEFINE_GLOBAL(bool, bSkip_skidmarks, 0x67D585);
DEFINE_GLOBAL(bool, bNo_annoying_chars, 0x67D586);
DEFINE_GLOBAL(bool, bDo_show_imaginary, 0x67D588);
DEFINE_GLOBAL(bool, bDo_show_camera, 0x67D58A);
DEFINE_GLOBAL(bool, bSkip_replay_sync_check, 0x67D594);
DEFINE_GLOBAL(bool, bDo_show_junc_ids, 0x67D5B0);
DEFINE_GLOBAL(bool, bLog_random_extra, 0x67D5BC);
DEFINE_GLOBAL(bool, bConstant_replay_save, 0x67D5C4);
DEFINE_GLOBAL(bool, bDo_blood, 0x67D5C5);
DEFINE_GLOBAL(bool, bDo_mike, 0x67D5CC);
DEFINE_GLOBAL(bool, bShow_hidden_faces, 0x67D5CD);
DEFINE_GLOBAL(bool, bLog_random, 0x67D5FC);
DEFINE_GLOBAL(bool, bDo_show_instruments, 0x67D64C);
DEFINE_GLOBAL(bool, bSkip_particles, 0x67D64D);
DEFINE_GLOBAL(bool, bSkip_tiles, 0x67D655);
DEFINE_GLOBAL(bool, bGet_all_weapons, 0x67D684);
DEFINE_GLOBAL(bool, gDo_miss_logging, 0x67D6BC);
DEFINE_GLOBAL(bool, gShow_cycle, 0x67D6BD);
DEFINE_GLOBAL(bool, bSkip_audio, 0x67D6BE);
DEFINE_GLOBAL(bool, bSkip_left, 0x67D6BF);
DEFINE_GLOBAL(bool, bLog_directinput, 0x67D6C0);
DEFINE_GLOBAL(bool, bDo_sync_check, 0x67D6C1);
DEFINE_GLOBAL(bool, bDo_3d_sound, 0x67D6C2);
DEFINE_GLOBAL(bool, bSkip_ambulance, 0x67D6C9);
DEFINE_GLOBAL(bool, bDo_show_object_ids, 0x67D6CA);
DEFINE_GLOBAL(bool, bDo_free_shopping, 0x67D6CD);
DEFINE_GLOBAL(bool, bDo_debug_keys, 0x67D6CF);
DEFINE_GLOBAL(bool, gDo_text_id_test, 0x67D6D0);
DEFINE_GLOBAL(bool, bLog_routefinder, 0x67D6D1);
DEFINE_GLOBAL(bool, bDo_show_timing, 0x67D6DC);
DEFINE_GLOBAL(bool, bDo_exit_after_replay, 0x67D6E4);
DEFINE_GLOBAL(bool, bDo_show_collision_box, 0x67D6E5);
DEFINE_GLOBAL(bool, gDo_show_counter, 0x67D6E6);
DEFINE_GLOBAL(bool, bShow_all_arrows, 0x67D6E7);
DEFINE_GLOBAL(bool, bDo_kill_phones_on_answer, 0x67D6E8);
DEFINE_GLOBAL(bool, gDebugFlag_Unused, 0x67D654);
DEFINE_GLOBAL(bool, gCheatJailCardSmgRespect, 0x67D56B);

MATCH_FUNC(0x4ABBD0)
void Debug::Init()
{
    bPlay_replay = gRegistry.Debug_Setting_Exists("play_replay");
    bConstant_replay_save = gRegistry.Debug_Setting_Exists("constant_replay_save");
    bSkip_dummies = gRegistry.Debug_Setting_Exists("skip_dummies");
    bDo_test = gRegistry.Debug_Setting_Exists("do_test");
    bSkip_mission = gRegistry.Debug_Setting_Exists("skip_mission");
    gShow_cycle = gRegistry.Debug_Setting_Exists("show_cycle");
    bDo_brian_test = gRegistry.Debug_Setting_Exists("do_brian_test");
    bDo_iain_test = gRegistry.Debug_Setting_Exists("do_iain_test");
    bSkip_tiles = gRegistry.Debug_Setting_Exists("skip_tiles");
    gDo_show_counter = gRegistry.Debug_Setting_Exists("do_show_counters");
    bDo_show_camera = gRegistry.Debug_Setting_Exists("do_show_camera");
    gDo_show_input = gRegistry.Debug_Setting_Exists("do_show_input");
    bDo_show_timing = gRegistry.Debug_Setting_Exists("do_show_timing");
    bDo_show_collision_box = gRegistry.Debug_Setting_Exists("do_show_collision_box");
    bDo_show_physics = gRegistry.Debug_Setting_Exists("do_show_physics");
    bDo_show_imaginary = gRegistry.Debug_Setting_Exists("do_show_imaginary");
    bSkip_user = gRegistry.Debug_Setting_Exists("skip_user");
    bSkip_traffic_lights = gRegistry.Debug_Setting_Exists("skip_traffic_lights");
    bSkip_recycling = gRegistry.Debug_Setting_Exists("skip_recycling");
    bLog_collisions = gRegistry.Debug_Setting_Exists("log_collisions");
    bDo_show_traffic_lights_info = gRegistry.Debug_Setting_Exists("do_show_traffic_lights_info");
    bDo_show_ids = gRegistry.Debug_Setting_Exists("do_show_ids");
    bLimit_recycling = gRegistry.Debug_Setting_Exists("limit_recycling");
    bNo_annoying_chars = gRegistry.Debug_Setting_Exists("no_annoying_chars");
    bSkip_slopes = gRegistry.Debug_Setting_Exists("skip_slopes");
    bSkip_left = gRegistry.Debug_Setting_Exists("skip_left");
    bSkip_right = gRegistry.Debug_Setting_Exists("skip_right");
    bSkip_top = gRegistry.Debug_Setting_Exists("skip_top");
    bSkip_bottom = gRegistry.Debug_Setting_Exists("skip_bottom");
    bSkip_lid = gRegistry.Debug_Setting_Exists("skip_lid");
    bLog_routefinder = gRegistry.Debug_Setting_Exists("log_routefinder");
    bDo_mike = gRegistry.Debug_Setting_Exists("do_mike");
    bSkip_particles = gRegistry.Debug_Setting_Exists("skip_particles");
    bShow_hidden_faces = gRegistry.Debug_Setting_Exists("show_hidden_faces");
    bGet_all_weapons = gRegistry.Debug_Setting_Exists("get_all_weapons");
    bDo_exit_after_replay = gRegistry.Debug_Setting_Exists("do_exit_after_replay");
    bDont_get_car_back = gRegistry.Debug_Setting_Exists("dont_get_car_back");
    bDo_show_instruments = gRegistry.Debug_Setting_Exists("do_show_instruments");
    bSkip_ambulance = gRegistry.Debug_Setting_Exists("skip_ambulance");
    bSkip_police = gRegistry.Debug_Setting_Exists("skip_police");
    bSkip_frontend = gRegistry.Debug_Setting_Exists("skip_frontend");
    bDo_invulnerable = gRegistry.Debug_Setting_Exists("do_invulnerable");
    bShow_all_arrows = gRegistry.Debug_Setting_Exists("show_all_arrows");
    bDo_show_horn = gRegistry.Debug_Setting_Exists("do_show_horn");
    bKeep_weapons_after_death = gRegistry.Debug_Setting_Exists("keep_weapons_after_death");
    bSkip_skidmarks = gRegistry.Debug_Setting_Exists("skip_skidmarks");
    bDo_show_junc_ids = gRegistry.Debug_Setting_Exists("do_show_junc_ids");
    bDo_corner_window = gRegistry.Debug_Setting_Exists("do_corner_window");
    gDo_infinite_lives = gRegistry.Debug_Setting_Exists("do_infinite_lives");
    bDo_load_savegame = gRegistry.Debug_Setting_Exists("do_load_savegame");
    bSkip_audio = gRegistry.Debug_Setting_Exists("skip_audio");
    bDo_debug_keys = gRegistry.Debug_Setting_Exists("do_debug_keys");
    bLog_random = gRegistry.Debug_Setting_Exists("log_random");
    bLog_random_extra = gRegistry.Debug_Setting_Exists("log_random_extra");
    bLog_input = gRegistry.Debug_Setting_Exists("log_input");
    bLog_directinput = gRegistry.Debug_Setting_Exists("log_directinput");
    bIgnore_replay_header = gRegistry.Debug_Setting_Exists("ignore_replay_header");
    bSkip_trains = gRegistry.Debug_Setting_Exists("skip_trains");
    bSkip_buses = gRegistry.Debug_Setting_Exists("skip_buses");
    bSkip_quit_confirm = gRegistry.Debug_Setting_Exists("skip_quit_confirm");
    bDo_sync_check = gRegistry.Debug_Setting_Exists("do_sync_check");
    bSkip_fire_engines = gRegistry.Debug_Setting_Exists("skip_fire_engines");
    bShow_brief_number = gRegistry.Debug_Setting_Exists("show_brief_number");
    bSkip_window_check = gRegistry.Debug_Setting_Exists("skip_window_check");
    bSkip_replay_sync_check = gRegistry.Debug_Setting_Exists("skip_replay_sync_check");
    bDo_show_object_ids = gRegistry.Debug_Setting_Exists("do_show_object_ids");
    bDo_kill_phones_on_answer = gRegistry.Debug_Setting_Exists("do_kill_phones_on_answer");
    gDo_miss_logging = gRegistry.Debug_Setting_Exists("do_miss_logging");
    gDo_text_id_test = gRegistry.Debug_Setting_Exists("do_text_id_test");
    bDo_police_1 = gRegistry.Debug_Setting_Exists("do_police_1");
    bDo_police_2 = gRegistry.Debug_Setting_Exists("do_police_2");
    bDo_police_3 = gRegistry.Debug_Setting_Exists("do_police_3");
    bSkip_draw = gRegistry.Debug_Setting_Exists("skip_draw");
    bDo_free_shopping = gRegistry.Debug_Setting_Exists("do_free_shopping");
    bDo_release_replay = gRegistry.Debug_Setting_Exists("do_release_replay");
    bDo_blood = gRegistry.Debug_Setting_Exists("do_blood");
    bDo_3d_sound = gRegistry.Sound_Setting_Exists("do_3d_sound");
    bIsFrench = HaveOnlyFrenchGxt();
    bShow_player_names = gRegistry.Get_Network_Setting("show_player_names", 1) != 0;
}

MATCH_FUNC(0x4AC200)
bool Debug::HaveOnlyFrenchGxt()
{
    bool bEGxtExists = false;
    bool bFGxtExists = false;
    bool bGGxtExists = false;
    bool bIGxtExists = false;
    bool bJGxtExists = false;
    bool bSGxtExists = false;

    FILE* fEnglish = fopen("data\\e.gxt", "rb");
    if (fEnglish)
    {
        fclose(fEnglish);
        bEGxtExists = true;
    }

    FILE* fFrench = fopen("data\\f.gxt", "rb");
    if (fFrench)
    {
        fclose(fFrench);
        bFGxtExists = true;
    }

    FILE* fGerman = fopen("data\\g.gxt", "rb");
    if (fGerman)
    {
        fclose(fGerman);
        bGGxtExists = true;
    }

    FILE* fItalian = fopen("data\\i.gxt", "rb");
    if (fItalian)
    {
        fclose(fItalian);
        bIGxtExists = true;
    }

    FILE* fJapanese = fopen("data\\j.gxt", "rb");
    if (fJapanese)
    {
        fclose(fJapanese);
        bJGxtExists = true;
    }

    FILE* fSpanish = fopen("data\\s.gxt", "rb");
    if (fSpanish)
    {
        fclose(fSpanish);
        bSGxtExists = true;
    }

    bool r = (!bEGxtExists && bFGxtExists == true && !bGGxtExists && !bIGxtExists && !bJGxtExists && !bSGxtExists);

    return r;
}

DEFINE_GLOBAL(Debug, gDebug, 0x67D52C);
