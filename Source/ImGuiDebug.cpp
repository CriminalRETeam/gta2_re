#include "ImGuiDebug.hpp"
#include "3rdParty/GTA2Hax/3rdParty/imgui/imgui.h"
#include "Ambulance_110.hpp"
#include "Game_0x40.hpp"
#include "Garox_2B00.hpp"
#include "Hamburger_500.hpp"
#include "Player.hpp"
#include "gbh_graphics.hpp"
#include "Object_5C.hpp"
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

void CC ImGuiDebugDraw()
{
    ImGui::Begin("Debugger");

    if (ImGui::TreeNode("gObject_5C_6F8F84"))
    {
        Player* pPlayer = gGame_0x40_67E008->field_4_players[0];
        if (gObject_5C_6F8F84 && pPlayer)
        {
            Ped* pPlayerPed = pPlayer->field_2C4_player_ped;
            if (ImGui::Button("Obj spawn"))
            {
                Char_B4* pPlayerChar = pPlayerPed->field_168_game_object;
                Sprite* pPlayerSprite = pPlayerChar->field_80_sprite_ptr;
                // 120 = tyre skid mark
                // 149 = tv aerial
                // 150 = ?
                // 151 = animated expanding rubbish pile?
                // 152 = rubbish pile?
                // 153 = crash
                // 154 = nothing?
                gObject_5C_6F8F84->sub_5299B0(120, pPlayerSprite->field_14_xpos, pPlayerSprite->field_18_ypos, pPlayerSprite->field_1C_zpos, 0);
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
                    gChar_C_6787BC->SpawnPedAt(pPlayerSprite->field_14_xpos, pPlayerSprite->field_18_ypos, pPlayerSprite->field_1C_zpos, pPlayerChar->field_5_remap, pPlayerPed->field_134);
                }
            }

            Ped* pPedIter = gChar_203AC_6787B8->field_4;
            while (pPedIter)
            {
                char buffer[128];
                sprintf(buffer, "Ped %d", pPedIter->field_200); // id
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
