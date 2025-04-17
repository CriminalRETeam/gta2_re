#include "ImGuiDebug.hpp"
#include "gbh_graphics.hpp"
#include "3rdParty/GTA2Hax/3rdParty/imgui/imgui.h"

void CC ImGuiDebugDraw()
{
    ImGui::Begin("lol");
    ImGui::Text("blah blah");
    static char buf[255];
    ImGui::InputText("meh", buf, 255);
    if (ImGui::Button("omfg"))
    {
        printf("Clicked\n");
    }
    ImGui::End();
}
