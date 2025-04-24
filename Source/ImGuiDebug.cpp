#include "ImGuiDebug.hpp"
#include "3rdParty/GTA2Hax/3rdParty/imgui/imgui.h"
#include "Ambulance_110.hpp"
#include "Car_BC.hpp"
#include "Game_0x40.hpp"
#include "Garox_2B00.hpp"
#include "Hamburger_500.hpp"
#include "Object_5C.hpp"
#include "Player.hpp"
#include "Shooey_CC.hpp"
#include "Weapon_8.hpp"
#include "gbh_graphics.hpp"
#include <stdarg.h>

extern EXPORT_VAR Ambulance_110* gAmbulance_110_6F70A8;

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

} // namespace ImGui

EXPORT_VAR extern Shooey_CC* gShooey_CC_67A4B8;

void CC ImGuiDebugDraw()
{
    ImGui::Begin("Debugger");

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
                Car_BC* pNewCar = gCar_6C_677930->sub_446230(pPlayerSprite->field_14_xpos + xOff,
                                           pPlayerSprite->field_18_ypos,
                                           pPlayerSprite->field_1C_zpos,
                                           0,
                                           currentCarModelIndex,
                                           scale);

                pNewCar->sub_43A9A0(pPlayerPed);
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
                gObject_5C_6F8F84->sub_5299B0(currentObjectIndex,
                                              pPlayerSprite->field_14_xpos,
                                              pPlayerSprite->field_18_ypos,
                                              pPlayerSprite->field_1C_zpos,
                                              0);
            }
        }
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Player"))
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

            ImGui::SliderS8("accuracy_count", &pPlayer->field_2D4_unk.field_198_accuracy_count, 0, 255);

            Car_BC* pPlayerCar = pPlayerPed->field_16C_car;
            ImGui::Text("Car 0x%X", pPlayerCar);
            if (pPlayerCar)
            {
                ImGui::Text("trailer? 0x%X", pPlayerCar->field_64);

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
                pPlayerPed->sub_45DD30(currentWeaponIndex, 20);

                pPlayerPed->RestoreCarOrPedHealth();

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
        if (gGarox_2B00_706620)
        {
            if (ImGui::Button("CreateTimer_5D31F0"))
            {
                gGarox_2B00_706620->field_620.CreateTimer_5D31F0(40);
            }

            static int v = 0;
            ImGui::InputInt("Timer num", &v, 1, 1);
            if (ImGui::Button("Stop timer") && v <= 3)
            {
                gGarox_2B00_706620->field_620.sub_5D32D0(v);
            }

            if (ImGui::Button("sub_5D3280 (stop sound?)") && v <= 3)
            {
                gGarox_2B00_706620->field_620.sub_5D3280(v);
            }

            if (ImGui::Button("sub_5D3220 (start sound?)") && v <= 3)
            {
                gGarox_2B00_706620->field_620.sub_5D3220(v);
            }

            ImGui::InputInt("timer f4", &gGarox_2B00_706620->field_620.field_620[1].field_4, 1, 100);
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
            ImGui::Input_char_type("field_5", &gChar_C_6787BC->field_5, 1, 1); // ??
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
                    gChar_C_6787BC->SpawnPedAt(pPlayerSprite->field_14_xpos,
                                               pPlayerSprite->field_18_ypos,
                                               pPlayerSprite->field_1C_zpos,
                                               pPlayerChar->field_5_remap,
                                               pPlayerPed->field_134);
                }
            }

            Ped* pPedIter = gChar_203AC_6787B8->field_4;
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

    ImGui::End(); // ImGui::Begin("Debugger");
}
