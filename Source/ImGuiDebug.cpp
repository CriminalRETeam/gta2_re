#include "ImGuiDebug.hpp"
#include "gbh_graphics.hpp"
#include "3rdParty/GTA2Hax/3rdParty/imgui/imgui.h"
#include "Game_0x40.hpp"
#include "Hamburger_500.hpp"
#include <stdarg.h>
#include "Ambulance_110.hpp"

extern EXPORT_VAR Ambulance_110* gAmbulance_110_6F70A8;


void CC ImGuiDebugDraw()
{
    ImGui::Begin("Debugger");

    if (gGame_0x40_67E008)
    {

    }

    ImGui::Begin("gAmbulance_110_6F70A8");
    if (gAmbulance_110_6F70A8)
    {
        s32 i;

        for (i = 0; i < 25; i++)
        {
            jawwie_8& j8 = gAmbulance_110_6F70A8->field_8[i];
            ImGui::Value("field_0", j8.field_0);
            ImGui::Value("field_4", j8.field_4);
        }

        for (i = 0; i <2; i++)
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
    ImGui::End();

    ImGui::Begin("gHamburger_500_678E30");
    if (gHamburger_500_678E30)
    {
        for (s32 i=0; i<20; i++)
        {
            Hamburger_40& hb = gHamburger_500_678E30->field_0[i];
            ImGui::Value("field_0", hb.field_0);
        }
    }
    ImGui::End();

    ImGui::End();
}
