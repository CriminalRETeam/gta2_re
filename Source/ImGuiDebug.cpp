#include "ImGuiDebug.hpp"
#include "3rdParty/GTA2Hax/3rdParty/imgui/imgui.h"
#include "Ambulance_110.hpp"
#include "Car_BC.hpp"
#include "Game_0x40.hpp"
#include "Hud.hpp"
#include "Hamburger_500.hpp"
#include "Object_5C.hpp"
#include "Orca_2FD4.hpp"
#include "Player.hpp"
#include "Shooey_CC.hpp"
#include "Firefighters.hpp"
#include "Weapon_8.hpp"
#include "collide.hpp"
#include "debug.hpp"
#include "gbh_graphics.hpp"
#include "Frontend.hpp"
#include "jolly_poitras_0x2BC0.hpp"
#include "Phi_8CA8.hpp"
#include "MapRenderer.hpp"
#include "map_0x370.hpp"
#include "gtx_0x106C.hpp"
#include "sprite.hpp"
#include "registry.hpp"
#include "lucid_hamilton.hpp"
#include <stdarg.h>

EXTERN_GLOBAL(Ambulance_110*, gAmbulance_110_6F70A8);
EXTERN_GLOBAL(Collide_C*, gCollide_C_6791FC);
EXTERN_GLOBAL(FirefighterPool_54*, gFirefighterPool_54_67D4C0);
EXTERN_GLOBAL(Orca_2FD4*, gOrca_2FD4_6FDEF0);

Object_2C* spawned_obj = NULL;
Car_BC* pNewCar = NULL;

void wchar_to_char(wchar_t* wchar, char* out, u8 size)
{
    u16 i = 0;
    for (; wchar[i]; i++)
    {
        out[(s8)i] = wchar[i];
    }
    // clear remaining slots
    for (s8 j = i; j < size; j++)
    {
        out[j] = 0;
    }
}

void get_car_name(Car_BC* pCar, char* out)
{
    wchar_t* car_wname = pCar->GetCarStr_439F80();
    wchar_to_char(car_wname, out, 33);
}

namespace ImGui
{
bool SliderS16(const char* label, s16* v, s16 v_min, s16 v_max)
{
    // Cast to int for ImGui::SliderInt
    int value = static_cast<int>(*v);
    bool result = ImGui::SliderInt(label, &value, static_cast<int>(v_min), static_cast<int>(v_max));
    if (result)
    {
        *v = static_cast<s16>(value); // Convert back to s16
    }
    return result;
}

bool SliderS8(const char* label, char_type* v, char_type v_min, char_type v_max)
{
    // Cast to int for ImGui::SliderInt
    int value = static_cast<int>(*v);
    bool result = ImGui::SliderInt(label, &value, static_cast<int>(v_min), static_cast<int>(v_max));
    if (result)
    {
        *v = static_cast<char_type>(value); // Convert back to s8
    }
    return result;
}

bool SliderU8(const char* label, u8* v, u8 v_min, u8 v_max)
{
    // Cast to int for ImGui::SliderInt
    int value = static_cast<int>(*v);
    bool result = ImGui::SliderInt(label, &value, static_cast<int>(v_min), static_cast<int>(v_max));
    if (result)
    {
        *v = static_cast<u8>(value); // Convert back to u8
    }
    return result;
}

bool Input_char_type(const char* label, char_type* v, int step, int step_fast, ImGuiInputTextFlags extra_flags = 0)
{
    int tmp = *v;
    bool ret = ImGui::InputInt(label, &tmp, step, step_fast, extra_flags);
    *v = static_cast<char_type>(tmp);
    return ret;
}

bool InputU8(const char* label, u8* v, int step, int step_fast, ImGuiInputTextFlags extra_flags = 0)
{
    int tmp = *v;
    bool ret = ImGui::InputInt(label, &tmp, step, step_fast, extra_flags);
    *v = static_cast<u8>(tmp);
    return ret;
}

bool InputS16(const char* label, s16* v, int step, int step_fast, ImGuiInputTextFlags extra_flags = 0)
{
    int tmp = *v;
    bool ret = ImGui::InputInt(label, &tmp, step, step_fast, extra_flags);
    *v = static_cast<s16>(tmp);
    return ret;
}

bool InputU16(const char* label, u16* v, int step, int step_fast, ImGuiInputTextFlags extra_flags = 0)
{
    int tmp = *v;
    bool ret = ImGui::InputInt(label, &tmp, step, step_fast, extra_flags);
    *v = static_cast<u16>(tmp);
    return ret;
}

} // namespace ImGui

EXTERN_GLOBAL(Shooey_CC*, gShooey_CC_67A4B8);


void CC ImGuiDebugDraw()
{
    ImGui::Begin("Debugger");

    if (ImGui::TreeNode("gTango_54_67D4C0"))
    {
        if (gFirefighterPool_54_67D4C0)
        {
            if (ImGui::Button("sub_4A8820"))
            {
                gFirefighterPool_54_67D4C0->sub_4A8820(0);
            }
        }
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("gCollide_C_6791FC"))
    {
        if (gCollide_C_6791FC)
        {
            ImGui::Text("field_0_count %d", gCollide_C_6791FC->field_0_count);
            ImGui::Text("field_4_count %d", gCollide_C_6791FC->field_4_count);
            ImGui::Text("field_8_bUnknown %d", gCollide_C_6791FC->field_8_bUnknown);
        }
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Camera"))
    {
        if (ImGui::TreeNode("gViewCamera_676978"))
        {
            if (gViewCamera_676978)
            {
                ImGui::Text("field_78_win_left %f", gViewCamera_676978->field_78_boundaries_non_neg.field_0_left.ToFloat());
                ImGui::Text("field_7C_right %f", gViewCamera_676978->field_78_boundaries_non_neg.field_4_right.ToFloat());
                ImGui::Text("field_80_win_top %f", gViewCamera_676978->field_78_boundaries_non_neg.field_8_top.ToFloat());
                ImGui::Text("field_84_win_bottom %f", gViewCamera_676978->field_78_boundaries_non_neg.field_C_bottom.ToFloat());
                ImGui::SliderInt("field_A4", &gViewCamera_676978->field_98_cam_pos2.field_C_zoom.mValue, 0, 30000);
                ImGui::SliderInt("screen_px_center_x", &gViewCamera_676978->field_70_screen_px_center_x, 0, 2000);
                ImGui::SliderInt("screen_px_center_y", &gViewCamera_676978->field_74_screen_px_center_y, 0, 1200);

                ImGui::SliderInt("gXCoord_6F63AC", &gXCoord_6F63AC.mValue, 0, 2000);
                ImGui::SliderInt("gYCoord_6F63B8", &gYCoord_6F63B8.mValue, 0, 2000);
                ImGui::SliderInt("gZCoord_6F63E0", &gZCoord_6F63E0, 0, 2000);
            }

            if (ImGui::Button("Orca_2FD4::sub_5552B0"))
            {
                u8 xpos = gViewCamera_676978->field_78_boundaries_non_neg.field_0_left.ToInt() + 5;
                u8 ypos = gViewCamera_676978->field_78_boundaries_non_neg.field_8_top.ToInt() + 5;
                u8 zpos = 2;
                if (gOrca_2FD4_6FDEF0->sub_5552B0(0, &xpos, &ypos, &zpos, 1))
                {
                }
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Game Camera"))
        {
            if (gGame_0x40_67E008)
            {
                Player* pPlayer = gGame_0x40_67E008->field_4_players[0];
                if (pPlayer)
                {
                    Camera_0xBC* game_camera = &pPlayer->field_90_game_camera;
                    //Camera_0xBC* view_camera = &pPlayer->field_14C_view_camera;
                    //Camera_0xBC* aux_camera = &pPlayer->field_208_aux_game_camera;
                    if (game_camera)
                    {
                        ImGui::SliderInt("field_A4", &game_camera->field_98_cam_pos2.field_C_zoom.mValue, 0, 25000);
                    }
                }
            }
            ImGui::TreePop();
        }
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("gCar_6C_677930"))
    {
        Player* pPlayer = gGame_0x40_67E008->field_4_players[0];
        if (gCar_6C_677930 && pPlayer)
        {
            Ped* pPlayerPed = pPlayer->field_2C4_player_ped;

            Char_B4* pPlayerChar = pPlayerPed->field_168_game_object;
            Sprite* pPlayerSprite = pPlayerChar->field_80_sprite_ptr;

            Fix16 scale;
            scale.mValue = 0x4000;

            static int currentCarModelIndex = 0;
            const char* carModelNames[] = {
                "Alfa",     "Allard",   "Amdb4",    "Apc",      "Bank Van",  "BMW",       "Boxcar",    "Boxtruck", "Bug",      "CAR9",
                "BUICK",    "BUS",      "COPCAR",   "DART",     "EDSEL",     "CAR15",     "FIAT",      "FIRETRUK", "GRAHAM",   "GT24640",
                "CAR20",    "GTRUCK",   "GUNJEEP",  "HOTDOG",   "HOTDOG_D1", "HOTDOG_D2", "HOTDOG_D3", "ICECREAM", "ISETLIMO", "ISETTA",
                "JEEP",     "JEFFREY",  "LIMO",     "LIMO2",    "MEDICAR",   "MERC",      "MESSER",    "MIURA",    "MONSTER",  "MORGAN",
                "MORRIS",   "PICKUP",   "RTYPE",    "CAR43",    "SPIDER",    "SPRITE",    "STINGRAY",  "STRATOS",  "STRATOSB", "STRIPETB",
                "STYPE",    "STYPECAB", "SWATVAN",  "T2000GT",  "TANK",      "TANKER",    "TAXI",      "TBIRD",    "TOWTRUCK", "TRAIN",
                "TRAINCAB", "TRAINFB",  "TRANCEAM", "TRUKCAB1", "TRUKCAB2",  "TRUKCONT",  "TRUKTRNS",  "TVVAN",    "VAN",      "VESPA",
                "VTYPE",    "WBTWIN",   "WRECK0",   "WRECK1",   "WRECK2",    "WRECK3",    "WRECK4",    "WRECK5",   "WRECK6",   "WRECK7",
                "WRECK8",   "WRECK9",   "XK120",    "ZCX5",     "EDSELFBI",  "HOTDOG_D4", "KRSNABUS",  "None"};

            // Calculate the size of the carModelNames array
            const int carModelCount = sizeof(carModelNames) / sizeof(carModelNames[0]);

            Fix16 xOff;
            xOff.FromInt(1);

            // Combo box for car model selection
            if (ImGui::Combo("Car Model", &currentCarModelIndex, carModelNames, carModelCount))
            {
                // Car model selection changed
            }

            if (ImGui::Button("Spawn car"))
            {
                pNewCar = gCar_6C_677930->SpawnCarAt_446230(pPlayerSprite->field_14_xpos.x + xOff,
                                                             pPlayerSprite->field_14_xpos.y,
                                                             pPlayerSprite->field_1C_zpos,
                                                             0,
                                                             currentCarModelIndex,
                                                             scale);
                pNewCar->sub_43BCA0();
                if (!pNewCar->field_5C)
                {
                    printf("Alloc physics\n");
                    Car_78* v18 = gCar_8F74_677CF8->field_0;
                    gCar_8F74_677CF8->field_0 = gCar_8F74_677CF8->field_0->field_C;
                    v18->sub_453D80();
                    pNewCar->field_5C = v18;
                }
                pNewCar->field_5C->sub_453BF0(pNewCar);

                //pPlayerPed->GiveWeapon_46F650(weapon_type::flamethrower);
                /*
                Ped* pNewPed = gChar_C_6787BC->SpawnPedAt(pPlayerSprite->field_14_xpos.x,
                                                          pPlayerSprite->field_14_xpos.y,
                                                          pPlayerSprite->field_1C_zpos,
                                                          pPlayerChar->field_5_remap,
                                                          pPlayerPed->field_134);
*/
                //gChar_C_6787BC->sub_470E30();

                gFirefighterPool_54_67D4C0->sub_4A8820(pNewCar);

                //pNewPed->SpawnDriverRunAway_45C650(pNewCar, 0);
            }

            if (pNewCar)
            {
                // test things on spawned car
            }
        }
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("gObject_5C_6F8F84"))
    {
        Player* pPlayer = gGame_0x40_67E008->field_4_players[0];
        if (gObject_5C_6F8F84 && pPlayer)
        {
            Ped* pPlayerPed = pPlayer->field_2C4_player_ped;

            if (ImGui::Button("gShooey_CC_67A4B8->ReportCrimeForPed"))
            {
                // 0 = ?
                // 1 = 10 24
                // 2 = 10 34
                // 3 = 10 90
                // 7 = 10 71 crime - shooting?
                gShooey_CC_67A4B8->ReportCrimeForPed(0u, pPlayer->field_2C4_player_ped);
            }

            static int currentObjectIndex = 0;
            const char* objectNames[] = {"object_0",
                                         "bin_lid_1",
                                         "bollard_2",
                                         "cone_3",
                                         "boxes_4",
                                         "blaster_5",
                                         "rubbish_6",
                                         "bin_7",
                                         "animating_oil_8",
                                         "oil_9",
                                         "mine_10",
                                         "bush_11",
                                         "crate_12",
                                         "footy_13",
                                         "hardbox_14",
                                         "newsdis_15",
                                         "oildrum_16",
                                         "tyre_17",
                                         "hydrant_lid_18",
                                         "hydrant_19",
                                         "hydrant_unlid_20",
                                         "roadblock_21",
                                         "bench_22",
                                         "package_23",
                                         "object_24",
                                         "tower_25",
                                         "object_26",
                                         "object_27",
                                         "object_28",
                                         "object_29",
                                         "object_30",
                                         "object_31",
                                         "object_32",
                                         "object_33",
                                         "object_34",
                                         "object_35",
                                         "object_36",
                                         "object_37",
                                         "object_38",
                                         "object_39",
                                         "object_40",
                                         "explode_medium_41",
                                         "object_42",
                                         "object_43",
                                         "object_44",
                                         "object_45",
                                         "object_46",
                                         "moving_collect_00_96",
                                         "moving_collect_01_97",
                                         "moving_collect_02_98",
                                         "moving_collect_03_99",
                                         "moving_collect_04_100",
                                         "moving_collect_05_101",
                                         "moving_collect_06_102",
                                         "moving_collect_07_103",
                                         "moving_collect_08_104",
                                         "moving_collect_09_105",
                                         "moving_collect_10_106",
                                         "moving_collect_11_107",
                                         "moving_collect_12_108",
                                         "moving_collect_13_109",
                                         "moving_collect_14_110",
                                         "moving_collect_15_111",
                                         "moving_collect_16_112",
                                         "moving_collect_17_113",
                                         "moving_collect_18_114",
                                         "moving_collect_19_115",
                                         "moving_collect_20_116",
                                         "moving_collect_21_117",
                                         "moving_collect_22_118",
                                         "moving_collect_23_119",
                                         "moving_collect_24_120",
                                         "moving_collect_25_121",
                                         "moving_collect_26_122",
                                         "moving_collect_27_123",
                                         "moving_collect_28_124",
                                         "moving_collect_29_125",
                                         "moving_collect_30_126",
                                         "moving_collect_31_127",
                                         "moving_collect_32_128",
                                         "moving_collect_33_129",
                                         "moving_collect_34_130",
                                         "moving_collect_35_131",
                                         "moving_collect_36_132",
                                         "moving_collect_37_133",
                                         "moving_collect_38_134",
                                         "moving_collect_39_135",
                                         "moving_collect_40_136",
                                         "moving_collect_41_137",
                                         "moving_collect_42_138",
                                         "moving_collect_43_139",
                                         "moving_collect_44_140",
                                         "small_arrow_141",
                                         "object_142",
                                         "blood_spark_143",
                                         "object_144",
                                         "particle_system_145",
                                         "firejet_146",
                                         "object_147",
                                         "small_brown_skid_148",
                                         "small_grey_skid_149",
                                         "small_red_skid_150",
                                         "medium_brown_skid_151",
                                         "medium_grey_skid_152",
                                         "medium_red_skid_153",
                                         "car_crossing_154",
                                         "car_stop_155",
                                         "big_white_skid_156",
                                         "medium_white_skid_157",
                                         "small_white_skid_158",
                                         "object_159",
                                         "rocket_160",
                                         "bus_stop_marker_161",
                                         "car_shop_162",
                                         "busy_car_shop_163",
                                         "car_bomb_164",
                                         "object_165",
                                         "object_166",
                                         "molotov_moving_167",
                                         "huge_red_skid_189",
                                         "huge_white_skid_190",
                                         "huge_brown_skid_191",
                                         "huge_grey_skid_192",
                                         "tanktop_193",
                                         "antenna_194",
                                         "object_195",
                                         "animating_rubbish_196",
                                         "dead_rubbish_197",
                                         "moving_cone_198",
                                         "object_199",
                                         "remote_200"};
            const int objectCount = sizeof(objectNames) / sizeof(objectNames[0]);

            // Combo box for object selection
            if (ImGui::Combo("Object", &currentObjectIndex, objectNames, objectCount))
            {
                // Object selection changed
            }

            if (ImGui::Button("Obj spawn"))
            {
                Char_B4* pPlayerChar = pPlayerPed->field_168_game_object;
                Sprite* pPlayerSprite = pPlayerChar->field_80_sprite_ptr;
                spawned_obj = gObject_5C_6F8F84->sub_5299B0(currentObjectIndex,
                                              pPlayerSprite->field_14_xpos.x,
                                              pPlayerSprite->field_14_xpos.y,
                                              pPlayerSprite->field_1C_zpos,
                                              0);
            }

            if (ImGui::TreeNode("Object_5C debug"))
            {
                ImGui::Value("field_1C", gObject_5C_6F8F84->field_1C);
                ImGui::Value("field_20", gObject_5C_6F8F84->field_20);
                ImGui::Value("field_24", gObject_5C_6F8F84->field_24);
                ImGui::Value("field_28", gObject_5C_6F8F84->field_28);
                ImGui::Value("field_2C", gObject_5C_6F8F84->field_2C);
                ImGui::Value("field_30", gObject_5C_6F8F84->field_30);
                ImGui::Value("field_34", gObject_5C_6F8F84->field_34);
                ImGui::Value("field_38", gObject_5C_6F8F84->field_38);
                ImGui::Value("field_3C", gObject_5C_6F8F84->field_3C);
                ImGui::Value("field_40", gObject_5C_6F8F84->field_40);
                ImGui::Value("field_44", gObject_5C_6F8F84->field_44);
                ImGui::Value("field_48", gObject_5C_6F8F84->field_48);
                ImGui::Value("field_4C", gObject_5C_6F8F84->field_4C);
                ImGui::Value("field_50", gObject_5C_6F8F84->field_50);
                ImGui::Value("field_54", gObject_5C_6F8F84->field_54);
                ImGui::TreePop();
            }
        }
        if (spawned_obj != NULL)
        {
            ImGui::Text("Object spawned attributes:");
            ImGui::SliderU8("field_26_varrok_idx", &spawned_obj->field_26_varrok_idx, 0, 25);
            Object_3C* obj_3c = spawned_obj->field_10;
            if (ImGui::TreeNode("Object_3C"))
            {
                if (obj_3c)
                {
                    ImGui::SliderS16("Obj3C angle", &obj_3c->field_4.rValue, 0, 1439);
                    ImGui::InputS16("Obj3C field_6", &obj_3c->field_6, 0, 1439);
                    ImGui::Value("Obj3C field_C", obj_3c->field_C.mValue);
                    ImGui::Value("Obj3C field_10", obj_3c->field_10.mValue);
                    ImGui::Value("Obj3C field_14", obj_3c->field_14);
                    ImGui::Value("Obj3C field_18", obj_3c->field_18.mValue);
                    ImGui::Value("Obj3C field_1C", obj_3c->field_1C.mValue);

                    ImGui::Value("Obj3C field_2E", obj_3c->field_2E);
                    ImGui::Value("Obj3C field_2F", obj_3c->field_2F);

                    ImGui::Value("Obj3C field_38", obj_3c->field_38);
                    ImGui::Value("Obj3C field_39", obj_3c->field_39);
                    ImGui::Value("Obj3C field_3A", obj_3c->field_3A);
                    ImGui::Value("Obj3C field_3B", obj_3c->field_3B);
                }
                ImGui::TreePop();
            }
        }
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Player"))
    {
        if (gGame_0x40_67E008)
        {
            Player* pPlayer = gGame_0x40_67E008->field_4_players[0];
            if (pPlayer)
            {
                ImGui::SliderInt("Lives", &pPlayer->field_684_lives.field_0, 0, 99);
                ImGui::SliderInt("Multiplier", &pPlayer->field_6BC_multpliers.field_0, 0, 99);
                ImGui::SliderInt("Cash", &pPlayer->field_2D4_unk.field_0_unk.field_0, 0, 999999999 - 50);
                ImGui::SliderInt("??", &pPlayer->field_2D4_unk.field_38_unk.field_0, 0, 99);

                Ped* pPlayerPed = pPlayer->field_2C4_player_ped;
                ImGui::SliderS16("wanted points", &pPlayerPed->field_20A_wanted_points, 0, 12000);
                ImGui::SliderS16("health", &pPlayerPed->field_216_health, 0, 999999);

                ImGui::SliderU8("accuracy_count", &pPlayer->field_2D4_unk.field_198_accuracy_count, 0, 255);

                Car_BC* pPlayerCar = pPlayerPed->field_16C_car;
                ImGui::Text("Car 0x%X", pPlayerCar);
                if (pPlayerCar)
                {
                    ImGui::Text("trailer? 0x%X", pPlayerCar->field_64_pTrailer);

                    if (ImGui::Button("ResprayOrCleanPlates"))
                    {
                        pPlayerCar->ResprayOrCleanPlates(2); // 0xFD - clean plates
                    }

                    /* Crash, for some reason
                    if (ImGui::Button("Add TV Aerial"))
                    {
                        pPlayerCar->sub_440BB0(); 
                    }
                    */
                }

                static int currentWeaponIndex = 0;
                const char* weaponNames[] = {"Pistol",         "SMG",
                                            "Rocket",         "Shocker",
                                            "Molotov",        "Grenade",
                                            "Shotgun",        "Electro Batton",
                                            "Flamethrower",   "Silence SMG",
                                            "Dual Pistol",    "Weapon 11",
                                            "Weapon 12",      "Weapon 13",
                                            "Weapon 14",      "Car Bomb",
                                            "Oil Stain",      "Car Mines",
                                            "Car SMG",        "Tank Main Gun",
                                            "Fire Truck Gun", "Fire Truck Flamethrower",
                                            "Army Gun Jeep",  "Weapon 0x17"};
                const s32 weaponCount = sizeof(weaponNames) / sizeof(weaponNames[0]);
                if (ImGui::Combo("Weapon Type", &currentWeaponIndex, weaponNames, weaponCount))
                {
                    // Weapon selection changed
                }
                if (ImGui::Button("Add car weapon"))
                {
                    if (pPlayerCar)
                    {
                        gWeapon_8_707018->allocate_5E3D50(currentWeaponIndex, 20, pPlayerCar);
                    }
                }
                if (ImGui::Button("Add player weapon"))
                {
                    //gWeapon_8_707018->allocate_5E3C10(currentWeaponIndex, pPlayerPed, 20);
                    // pPlayerPed->AddWeaponWithAmmo_45DD30(currentWeaponIndex, 20);

                    pPlayerPed->ForceWeapon_46F600(currentWeaponIndex);

                    /*
                    Char_B4* pB4 = pPlayerPed->field_168_game_object;
                    if ( pB4 )
                    {
                    //pB4->field_80_sprite_ptr->field_2C |= 4u;
                    // Object_2C* p2C = gObject_5C_6F8F84->sub_5299B0(198, 0, 0, 0, 0); // dead_rubbish_197 ?? but its actually fire
                    // pB4->field_80_sprite_ptr->sub_5A3100(p2C->field_4, 0, 0, 0);
                    pB4->field_b0 = 100;
                    }*/
                }

                //ImGui::SliderInt("field_220", &pPlayerPed->field_220, 0, 999999);

                Car_BC** field_54_car_array = pPlayer->field_54_unk;
                if (field_54_car_array)
                {
                    for(u32 car_idx = 0; car_idx < 3; car_idx++)
                    {
                        Car_BC* pCar = field_54_car_array[car_idx];
                        if (pCar)
                        {
                            static char car_name[33];
                            get_car_name(pCar, car_name);
                            ImGui::Text("field_54_unk car %d name: %s", car_idx, car_name);
                        }
                    }
                }
            }
        }
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("gGame_0x40_67E008"))
    {
        if (gGame_0x40_67E008)
        {
            if (ImGui::Button("sub_569E70"))
            {
                gGame_0x40_67E008->field_38_orf1->sub_569E70();
            }
        }
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("HUD"))
    {
        if (gHud_2B00_706620)
        {
            if (ImGui::TreeNode("Timer"))
            {
                if (ImGui::Button("CreateTimer_5D31F0"))
                {
                    gHud_2B00_706620->field_620.CreateTimer_5D31F0(40);
                }

                static s32 v = 0;
                ImGui::InputInt("Timer num", (s32*)&v, 1, 1);
                if (ImGui::Button("Stop timer") && v <= 3)
                {
                    gHud_2B00_706620->field_620.sub_5D32D0(v);
                }

                if (ImGui::Button("sub_5D3280 (stop sound?)") && v <= 3)
                {
                    gHud_2B00_706620->field_620.sub_5D3280(v);
                }

                if (ImGui::Button("sub_5D3220 (start sound?)") && v <= 3)
                {
                    gHud_2B00_706620->field_620.sub_5D3220(v);
                }

                ImGui::InputInt("timer f4", &gHud_2B00_706620->field_620.field_620[1].field_4, 1, 100);
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Garox_1E34_L"))
            {
                Garox_1E34_L* pGarox_1E34_L = &gHud_2B00_706620->field_DC;
                
                if (pGarox_1E34_L)
                {
                    ImGui::Value("field_500", pGarox_1E34_L->field_500);
                    ImGui::InputS16("field_502_face_idx", &pGarox_1E34_L->field_502_face_idx, 1, 1);
                    ImGui::Value("field_504_tick_timer", pGarox_1E34_L->field_504_tick_timer);
                    ImGui::Value("field_506", pGarox_1E34_L->field_506);
                    ImGui::Value("field_508_num_lines", pGarox_1E34_L->field_508_num_lines);
                    ImGui::Value("field_50C", pGarox_1E34_L->field_50C);
                    ImGui::Value("field_510_time_to_show", pGarox_1E34_L->field_510_time_to_show);
                    ImGui::Value("field_514_upward_timer", pGarox_1E34_L->field_514_upward_timer);
                    ImGui::Value("field_51C", pGarox_1E34_L->field_51C);
                    ImGui::Value("field_6EC", pGarox_1E34_L->field_6EC);
                    ImGui::Value("field_6F0", pGarox_1E34_L->field_6F0);
                    ImGui::Value("field_6F4", pGarox_1E34_L->field_6F4);
                }
                
                ImGui::TreePop();
            }
        }
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("gChar_C_6787BC"))
    {
        if (gChar_C_6787BC)
        {
            if (ImGui::Button("Player invisible Ian Test 8"))
            {
                gChar_C_6787BC->DoIanTest_471060(8);
            }
            if (ImGui::Button("Player visible Ian Test 9"))
            {
                gChar_C_6787BC->DoIanTest_471060(9);
            }
            if (ImGui::Button("Set player on fire PIan Test 10"))
            {
                gChar_C_6787BC->DoIanTest_471060(10);
            }

            ImGui::Text("field_0 %d", gChar_C_6787BC->field_0); // spawn rate ?
            ImGui::Input_char_type("field_2", &gChar_C_6787BC->field_2, 1, 1); // total spawned ?
            ImGui::Input_char_type("field_3", &gChar_C_6787BC->field_3, 1, 1); // something to do with total ped count also
            ImGui::Input_char_type("field_4", &gChar_C_6787BC->field_4, 1, 1); // ??
            ImGui::InputU8("field_5", &gChar_C_6787BC->field_5, 1, 1); // ??
            ImGui::Text("Num peds on screen %d", gChar_C_6787BC->field_6_num_peds_on_screen);
            ImGui::Input_char_type("field_7_make_all_muggers", &gChar_C_6787BC->field_7_make_all_muggers, 1, 1);
        }
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("gChar_203AC_6787B8"))
    {
        if (gChar_203AC_6787B8)
        {
            Player* pPlayer = gGame_0x40_67E008->field_4_players[0];
            Ped* pPlayerPed = pPlayer->field_2C4_player_ped;

            if (ImGui::Button("Spawn 100"))
            {
                s32 c;
                for (c = 0; c < 100; c++)
                {
                    Char_B4* pPlayerChar = pPlayerPed->field_168_game_object;
                    Sprite* pPlayerSprite = pPlayerChar->field_80_sprite_ptr;
                    gChar_C_6787BC->SpawnPedAt(pPlayerSprite->field_14_xpos.x,
                                               pPlayerSprite->field_14_xpos.y,
                                               pPlayerSprite->field_1C_zpos,
                                               pPlayerChar->field_5_remap,
                                               pPlayerPed->field_134);
                }
            }

            Ped* pPedIter = gChar_203AC_6787B8->field_4_pNext;
            while (pPedIter)
            {
                char buffer[128];
                sprintf(buffer, "Ped %d", pPedIter->field_200_id);
                if (ImGui::TreeNode(buffer))
                {
                    ImGui::SliderS16("wanted points", &pPedIter->field_20A_wanted_points, 0, 12000);
                    ImGui::SliderS16("health", &pPedIter->field_216_health, 0, 999999);
                    ImGui::SliderInt("occupation", &pPedIter->field_240_occupation, 0, 500);
                    ImGui::TreePop();
                }
                pPedIter = pPedIter->field_160_next_ped;
            }
        }
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("gAmbulance_110_6F70A8"))
    {
        if (gAmbulance_110_6F70A8)
        {
            s32 i;

            for (i = 0; i < 25; i++)
            {
                jawwie_8& j8 = gAmbulance_110_6F70A8->field_8[i];
                ImGui::Value("field_0", j8.field_0);
                ImGui::Value("field_4", j8.field_4);
            }

            for (i = 0; i < 2; i++)
            {
                Ambulance_20& a20 = gAmbulance_110_6F70A8->field_D0[i];
            }

            /*
            // TODO:
                char_type field_0;
                u8 field_1_f8_idx;
                char_type field_2;
                char_type field_3;
                Ped_Unknown_4 field_4;
            */
        }
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("gHamburger_500_678E30"))
    {
        if (gHamburger_500_678E30)
        {
            for (s32 i = 0; i < 20; i++)
            {
                Hamburger_40& hb = gHamburger_500_678E30->field_0[i];
                ImGui::Value("field_0", hb.field_0);
            }
        }
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("gPhi_8CA8_6FCF00"))
    {
        if (ImGui::Button("Init Phi arrays"))
        {
            Init_Phi_54_array();
            Init_Phi_6C_array();
        }
        if (ImGui::TreeNode("Global Vars"))
        {
            ImGui::Value("DAT_006f8fac", DAT_006f8fac.mValue);
            ImGui::Value("DAT_006f8fd8", DAT_006f8fd8.mValue);
            ImGui::Value("DAT_006fc578", DAT_006fc578.mValue);
            ImGui::Value("DAT_006fc584", DAT_006fc584.mValue);
            ImGui::Value("Phi_54_array_lenght_00623EEC", Phi_54_array_lenght_00623EEC);
            ImGui::Value("Phi_6C_array_lenght_623EF0", Phi_6C_array_lenght_623EF0);

            if (ImGui::TreeNode("Phi_54_array_006FC5F8"))
            {
                static s32 Phi_54_idx = 0;
                ImGui::SliderInt("Phi_54 idx", &Phi_54_idx, 0, 23);
                
                if (Phi_54_array_006FC5F8)
                {
                    Phi_54* phi_54 = &Phi_54_array_006FC5F8[Phi_54_idx];
                    if (phi_54)
                    {
                        ImGui::Value("field_0", phi_54->field_0);
                        ImGui::Value("field_4", phi_54->field_4);
                        ImGui::Value("field_8", phi_54->field_8);
                        ImGui::Value("field_C", phi_54->field_C);
                        ImGui::Value("field_10", phi_54->field_10);
                        ImGui::Value("field_14", phi_54->field_14);
                        ImGui::Value("field_18", phi_54->field_18);
                        ImGui::Value("field_1C", phi_54->field_1C.mValue);
                        ImGui::Value("field_20", phi_54->field_20.mValue);
                        ImGui::Value("field_24", phi_54->field_24);
                        ImGui::Value("field_28", phi_54->field_28);
                        ImGui::Value("field_2C", phi_54->field_2C);
                        ImGui::Value("field_30", phi_54->field_30);
                        ImGui::Value("field_34", phi_54->field_34);
                        ImGui::Value("field_38", phi_54->field_38.mValue);
                        ImGui::Value("field_3C", phi_54->field_3C);
                        ImGui::Value("field_40", phi_54->field_40);
                        ImGui::Value("field_44", phi_54->field_44);
                        ImGui::Value("field_48", phi_54->field_48);
                        ImGui::Value("field_4C", phi_54->field_4C);
                        ImGui::Value("field_50", phi_54->field_50);
                    }
                }
                
                ImGui::TreePop();
            }

            ImGui::TreePop();
        }
        if (gPhi_8CA8_6FCF00)
        {
            ImGui::Value("field_0_next_idx", gPhi_8CA8_6FCF00->field_0_next_idx);
            ImGui::Value("field_2", gPhi_8CA8_6FCF00->field_2);
            ImGui::Value("field_8CA4", gPhi_8CA8_6FCF00->field_8CA4);
            ImGui::Value("field_8CA6", gPhi_8CA8_6FCF00->field_8CA6);

            //static s32 phi_74_id = 0;
            //ImGui::SliderInt("Phi_74 id", &phi_74_id, 0, 299);
            //Phi_74* phi = gPhi_8CA8_6FCF00->field_87F4[phi_74_id];
            if (ImGui::TreeNode("Spawned object"))
            {
                if (spawned_obj)
                {
                    Phi_74* phi = spawned_obj->field_8;
                
                    ImGui::InputInt("field_0", &phi->field_0.mValue, 1, 1);
                    ImGui::InputInt("field_4", &phi->field_4.mValue, 1, 1);
                    ImGui::InputInt("field_8", &phi->field_8.mValue, 1, 1);
                    ImGui::InputInt("field_C", &phi->field_C.mValue, 1, 1);
                    ImGui::InputInt("field_10", &phi->field_10.mValue, 1, 1);
                    ImGui::InputInt("field_14", &phi->field_14.mValue, 1, 1);
                    ImGui::InputInt("field_18", &phi->field_18.mValue, 1, 1);
                    ImGui::SliderS16("field_1C_remap", &phi->field_1C_remap, 0, 50);
                    ImGui::Input_char_type("field_20", &phi->field_20, 1, 1);
                    ImGui::Input_char_type("field_21", &phi->field_21, 1, 1);
                    ImGui::Input_char_type("field_22", &phi->field_22, 1, 1);
                    ImGui::Input_char_type("field_23", &phi->field_23, 1, 1);
                    ImGui::InputInt("field_24_idx", &phi->field_24_idx, 1, 1);
                    ImGui::InputInt("field_28", &phi->field_28, 1, 1);

                    if (ImGui::TreeNode("another vars"))
                    {
                        ImGui::InputInt("field_2C", &phi->field_2C, 1, 1);
                        ImGui::InputInt("field_30", &phi->field_30, 1, 1);
                        ImGui::InputInt("field_34", &phi->field_34, 1, 1);
                        ImGui::InputInt("field_38", &phi->field_38, 1, 1);
                        ImGui::InputInt("field_3C", &phi->field_3C, 1, 1);
                        ImGui::InputInt("field_40", &phi->field_40, 1, 1);
                        ImGui::InputInt("field_44", &phi->field_44, 1, 1);
                        ImGui::InputInt("field_48", &phi->field_48, 1, 1);
                        ImGui::InputInt("field_4C", &phi->field_4C, 1, 1);
                        ImGui::InputInt("field_50", &phi->field_50, 1, 1);
                        ImGui::InputInt("field_54", &phi->field_54, 1, 1);
                        ImGui::InputInt("field_58", &phi->field_58, 1, 1);
                        ImGui::InputInt("field_5C", &phi->field_5C, 1, 1);

                        ImGui::Input_char_type("field_60", &phi->field_60, 1, 1);
                        ImGui::Input_char_type("field_61", &phi->field_61, 1, 1);
                        ImGui::Input_char_type("field_62", &phi->field_62, 1, 1);
                        ImGui::Input_char_type("field_63", &phi->field_63, 1, 1);
                        ImGui::Input_char_type("field_64", &phi->field_64, 1, 1);
                        ImGui::Input_char_type("field_65", &phi->field_65, 1, 1);
                        ImGui::Input_char_type("field_66", &phi->field_66, 1, 1);
                        ImGui::Input_char_type("field_67", &phi->field_67, 1, 1);

                        ImGui::InputInt("field_68", &phi->field_68, 1, 1);
                        

                        ImGui::TreePop();
                    }
                    

                    ImGui::SliderU8("field_6C", &phi->field_6C, 0, 32);
                    ImGui::Input_char_type("field_6D", &phi->field_6D, 1, 1);
                    ImGui::Input_char_type("field_6E", &phi->field_6E, 1, 1);
                    ImGui::Input_char_type("field_6F", &phi->field_6F, 1, 1);
                    ImGui::InputInt("field_70", &phi->field_70, 1, 1);
                }
                ImGui::TreePop();
            }
        }
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Debug options"))
    {
        ImGui::Checkbox("Debug Keys", &bDo_debug_keys_67D6CF);
        ImGui::Checkbox("Iain Test", &bDo_iain_test_67D4E9);
        ImGui::Checkbox("Brian Test", &bDo_brian_test_67D544);
        ImGui::Checkbox("Do Test", &bDo_test_67D4F8);
        ImGui::Checkbox("Do Mike", &bDo_mike_67D5CC);
        ImGui::Checkbox("Do Text ID Test", &gDo_text_id_test_67D6D0); // press '<' or '>' to navigate through gxt text id's  (Garox_4 stuff)
        ImGui::Checkbox("3D Sound", &bDo_3d_sound_67D6C2); // seems to not work

        if (ImGui::TreeNode("Skip stuff"))
        {
            ImGui::Checkbox("Skip Missions", &bSkip_mission_67D4E5);
            ImGui::Checkbox("Skip Particles", &bSkip_particles_67D64D);
            ImGui::Checkbox("Skip Ambulance", &bSkip_ambulance_67D6C9);
            ImGui::Checkbox("Skip Explosions", &bExplodingOff_67D4FB);
            ImGui::Checkbox("Skip Police", &bSkip_police_67D4F9);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Show things"))
        {
            ImGui::Checkbox("Show Ped IDs", &bDo_show_ids_67D559);
            ImGui::Checkbox("Show Objects IDs", &bDo_show_object_ids_67D6CA);
            ImGui::Checkbox("Show Junctions IDs", &bDo_show_junc_ids_67D5B0);
            ImGui::Checkbox("Show Traffic lights info", &bDo_show_traffic_lights_info_67D4FA);
            ImGui::Checkbox("Show Imaginary", &bDo_show_imaginary_67D588); // some cones appears when the player is hit by rocket launcher
            ImGui::Checkbox("Show Camera", &bDo_show_camera_67D58A);
            //ImGui::Checkbox("Show Collision Box", &bDo_show_collision_box_67D6E5);    // crashes the game
            ImGui::Checkbox("Show Physics", &bDo_show_physics_67D54F);
            ImGui::Checkbox("Show Instruments", &bDo_show_instruments_67D64C);
            ImGui::Checkbox("Show Cycle", &gShow_cycle_67D6BD);
            ImGui::Checkbox("Show Timing", &bDo_show_timing_67D6DC);
            ImGui::Checkbox("Show Counter", &gDo_show_counter_67D6E6);
            ImGui::Checkbox("Show Brief number", &bShow_brief_number_67D504);
            ImGui::Checkbox("Show Horn", &bDo_show_horn_67D4F2);
            ImGui::TreePop();
        } 

        if (ImGui::TreeNode("Cheats"))
        {
            ImGui::Checkbox("Invulnerable", &bDo_invulnerable_67D4CB);
            ImGui::Checkbox("Get all weapons", &bGet_all_weapons_67D684);
            ImGui::Checkbox("Keep weapons after death", &bKeep_weapons_after_death_67D54D);
            ImGui::Checkbox("Do Blood", &bDo_blood_67D5C5);
            ImGui::TreePop();
        }

        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Frontend"))
    {
        if (ImGui::TreeNode("MenuPage_0xBCA"))
        {
            if (gFrontend_67DC84)
            {
                static s32 loving_id = 0;
                ImGui::SliderInt("Loving ID", &loving_id, 0, 16);

                MenuPage_0xBCA* loving_borg = &gFrontend_67DC84->field_136_menu_pages_array[loving_id];
                ImGui::Value("field_0_number_of_options", loving_borg->field_0_number_of_options);
                ImGui::Value("field_2", loving_borg->field_2);
                ImGui::Value("field_BC6_current_option_idx", loving_borg->field_BC6_current_option_idx);
                ImGui::Value("field_BC8", loving_borg->field_BC8);

                if (ImGui::TreeNode("menu_option_0x82"))
                {
                    static s32 nifty_id = 0;
                    ImGui::SliderInt("Nifty Maxwell ID", &nifty_id, 0, 9);

                    menu_option_0x82* nifty_maxwell = &loving_borg->field_4_options_array[nifty_id];

                    ImGui::Value("field_0", nifty_maxwell->field_0_option_type);
                    ImGui::Value("field_1", nifty_maxwell->field_1_is_unlocked);

                    ImGui::SliderS16("field_2", &nifty_maxwell->field_2_x_pos, 0, 1000);
                    ImGui::SliderS16("field_4", &nifty_maxwell->field_4_y_pos, 0, 700);

                    static char str_buf[50];
                    wchar_to_char(nifty_maxwell->field_6_option_name_str, str_buf, 50);
                    ImGui::Text(str_buf);

                    ImGui::Value("field_6A", nifty_maxwell->field_6A);
                    ImGui::Value("field_6C", nifty_maxwell->field_6C);
                    ImGui::Value("field_6E_count", nifty_maxwell->field_6E_horizontal_selected_idx);
                    ImGui::Value("field_70", nifty_maxwell->field_70);

                    ImGui::Text(nifty_maxwell->field_72);

                    ImGui::Value("field_7E", nifty_maxwell->field_7E_horizontal_max_idx);
                    ImGui::Value("field_80", nifty_maxwell->field_80_menu_page_target);

                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("menu_element_0x6E"))
                {
                    static s32 noyce_id = 0;
                    ImGui::SliderInt("Competent Noyce ID", &noyce_id, 0, 14);

                    menu_element_0x6E* competent_noyce = &loving_borg->field_518_elements_array[noyce_id];

                    ImGui::Value("field_0", competent_noyce->field_0_element_type);
                    ImGui::Value("field_1", competent_noyce->field_1_is_it_displayed);

                    ImGui::SliderS16("field_2_xpos", &competent_noyce->field_2_xpos, 0, 1000);
                    ImGui::SliderS16("field_4_ypos", &competent_noyce->field_4_ypos, 0, 700);

                    static char str_buf_2[50];
                    wchar_to_char(competent_noyce->field_6_element_name_str, str_buf_2, 50);
                    ImGui::Text(str_buf_2);

                    ImGui::InputU16("field_6A", &competent_noyce->field_6A_font_type, 1, 1);
                    ImGui::InputU16("field_6C", &competent_noyce->field_6C_font_variant, 1, 1);

                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("kind_beaver_6"))
                {
                    static s32 k_beaver_id = 0;
                    ImGui::SliderInt("Kind Beaver ID", &k_beaver_id, 0, 9);

                    kind_beaver_6* kind_beaver = &loving_borg->field_B8A[k_beaver_id];

                    ImGui::SliderS16("field_0", &kind_beaver->field_0, 0, 1000);
                    ImGui::SliderS16("field_2", &kind_beaver->field_2, 0, 700);

                    ImGui::Input_char_type("field_4_is_option_unlocked", &kind_beaver->field_4_is_option_unlocked, 1, 1);
                    ImGui::Input_char_type("field_5", &kind_beaver->field_5, 1, 1);

                    ImGui::TreePop();
                }
                

            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("admiring_euler_4"))
        {
            if (gFrontend_67DC84)
            {
                static s32 euler_id = 0;
                ImGui::SliderInt("Euler ID", &euler_id, 0, 7);

                admiring_euler_4* admiring_euler = &gFrontend_67DC84->field_EDE8_plySlots[euler_id];

                ImGui::Value("field_0", admiring_euler->field_0_save_exists);
                ImGui::Value("field_1", admiring_euler->field_1_last_saved_stage);
                ImGui::Value("field_2", admiring_euler->field_2);
                ImGui::Value("field_3", admiring_euler->field_3);
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Debug Frontend"))
        {
            if (gFrontend_67DC84)
            {
                if (ImGui::TreeNode("Player settings"))
                {
                    static s32 player_idx;
                    ImGui::SliderInt("Player idx", &player_idx, 0, 7);

                    // fix softlock crashes after trying to change player name in standalone
                    if (ImGui::Button("Set Player"))
                    {
                        gLucid_hamilton_67E8E0.sub_4C5920(player_idx);
                        gFrontend_67DC84->sub_4B42E0();
                        gRegistry_6FF968.Set_Player_Setting_5878C0("plyrslot", player_idx);
                    }
                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("Credits"))
                {

                    xenodochial_morse* pCreditsStruct = &gFrontend_67DC84->field_EE0E_unk;
                    if (pCreditsStruct)
                    {
                        ImGui::InputS16("Xeno field_0", &pCreditsStruct->field_0, 1, 1);

                        static s32 credit_line;
                        ImGui::InputInt("Credit line", &credit_line, 1, 1);

                        sleepy_stonebraker_0x6C* pCreditLine = &pCreditsStruct->field_2[credit_line];
                        if (pCreditLine)
                        {
                            ImGui::SliderS8("Sleepy field_0", &pCreditLine->field_0, -127, 127);
                            ImGui::SliderS8("Sleepy field_1", &pCreditLine->field_1, -127, 127);
                            ImGui::InputS16("Sleepy field_2", &pCreditLine->field_2, 1, 1);
                            ImGui::InputU16("Sleepy field_4", &pCreditLine->field_4, 1, 1);
                            ImGui::InputU16("Sleepy field_6", &pCreditLine->field_6, 1, 1);
                        }
                    }
                    
                }
                
            }
            ImGui::TreePop();
        }
        
        ImGui::TreePop();
    }
    
    if (ImGui::TreeNode("gJolly_poitras_0x2BC0_6FEAC0"))
    {
        if (gJolly_poitras_0x2BC0_6FEAC0)
        {
            if (ImGui::TreeNode("struc_221"))
            {
                static s32 struc_221_id = 0;
                ImGui::SliderInt("struc_221 ID", &struc_221_id, 0, 2);
                struc_221* struc = &gJolly_poitras_0x2BC0_6FEAC0->field_1800_best_stats[struc_221_id];

                static s32 byte_id = 0;
                ImGui::SliderInt("Byte ID", &byte_id, 0, 39);
                ImGui::Value("field_0 at id", struc->field_0[byte_id]);
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("high_score_table_0xF0"))
            {
                static s32 agitated_type = 0;

                const char* agitated_fields[] = {"field_1890", "field_23D0",
                                                "field_24C0", "field_25B0"};
                ImGui::Combo("agitated_field", &agitated_type, agitated_fields, 4);

                static s32 id_1 = 0;
                static s32 id_2 = 0;
                ImGui::SliderInt("field_1890 ID 1", &id_1, 0, 2);
                ImGui::SliderInt("field_1890 ID 2", &id_2, 0, 3);
                
                high_score_table_0xF0* agitated_keldysh;

                switch(agitated_type)
                {
                    case 0:
                        agitated_keldysh = &gJolly_poitras_0x2BC0_6FEAC0->field_1890_stage_scores[id_1][id_2];
                        break;
                    case 1:
                        agitated_keldysh = &gJolly_poitras_0x2BC0_6FEAC0->field_23D0;
                        break;
                    case 2:
                        agitated_keldysh = &gJolly_poitras_0x2BC0_6FEAC0->field_24C0;
                        break;
                    case 3:
                        agitated_keldysh = &gJolly_poitras_0x2BC0_6FEAC0->field_25B0;
                        break;
                    default:
                        break;
                }

                static s32 string_id = 0;
                ImGui::SliderInt("string_id", &string_id, 0, 9);
                score_table_line* s_string = &agitated_keldysh->field_0_score_table_line[string_id];

                static char str_buf_3[10];
                wchar_to_char(s_string->field_0_player_name, str_buf_3, 10);
                ImGui::Text(str_buf_3);
                ImGui::Value("field_14_score", s_string->field_14_score);

                ImGui::TreePop();
            }

            if (ImGui::TreeNode("player_stats_0xA4"))
            {
                static s32 dreamy_id = 0;
                ImGui::SliderInt("dreamy_id", &dreamy_id, 0, 7);
                player_stats_0xA4* dreamy = &gJolly_poitras_0x2BC0_6FEAC0->field_26A0_plyr_stats[dreamy_id];

                static char str_buf_4[9];
                wchar_to_char(dreamy->field_90_strPlayerName, str_buf_4, 9);
                ImGui::Text(str_buf_4);
                ImGui::Value("field_A2", dreamy->field_A2);

                static s32 gifted_joliot_id_1 = 0;
                static s32 gifted_joliot_id_2 = 0;
                ImGui::SliderInt("stage_stats ID 1", &gifted_joliot_id_1, 0, 2);
                ImGui::SliderInt("stage_stats ID 2", &gifted_joliot_id_2, 0, 3);
                stage_stats* g_joliot = &dreamy->field_0_plyr_stage_stats[gifted_joliot_id_1][gifted_joliot_id_2];

                ImGui::Value("Joliot field_0", g_joliot->field_0_is_stage_unlocked);
                ImGui::Value("Joliot field_1", g_joliot->field_1);
                ImGui::Value("Joliot field_2", g_joliot->field_2);
                ImGui::Value("Joliot field_3", g_joliot->field_3);
                ImGui::Value("Joliot field_4", g_joliot->field_4_stage_best_score);
                ImGui::Value("Joliot field_8", g_joliot->field_8_stage_latest_score);

                ImGui::TreePop();
            }
        }
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("gGtx_0x106C_703DD4"))
    {
        if (gGtx_0x106C_703DD4)
        {
            static s32 tile_idx = 0;
            ImGui::InputInt("Tile idx", &tile_idx, 1, 1);
            s32 spec = gGtx_0x106C_703DD4->field_6C_spec[tile_idx];
            ImGui::Value("Spec", spec);
        }
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Slope struct 0x6F5BA8"))
    {
        static u8 slope_idx = 0;
        ImGui::SliderU8("slope_idx", &slope_idx, 0, 63);
        gmp_map_slope* slope_struct = &byte_6F5BA8[slope_idx];
        if (slope_struct)
        {
            ImGui::Value("field_0", slope_struct->field_0_gradient_direction);
            ImGui::Value("field_1", slope_struct->field_1_gradient_size);
            ImGui::Value("field_2", slope_struct->field_2_gradient_level);
            ImGui::Value("field_3", slope_struct->field_3_padding);
            ImGui::Value("field_4 float", slope_struct->field_4_zpos_lower.ToFloat());
            ImGui::Value("field_8 float", slope_struct->field_8_zpos_higher.ToFloat());
        }
        ImGui::TreePop();
    }

    ImGui::End(); // ImGui::Begin("Debugger");
}