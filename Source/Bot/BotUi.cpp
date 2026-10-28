// The bot's overlay: what it is doing and why, drawn with the ImGui that the game's video dll already carries (the debug
// window of ImGuiDebug.cpp uses the same hook). Plus the WTF button, see Bot::SaveWtf.
#include "Bot.hpp"
#include "Snapshot.hpp"
#include "WorldModel.hpp"
#include "BotAction.hpp"
#include "NavCar.hpp"
#include "BotLog.hpp"
#include "BotDebugDraw.hpp"
#include "Camera.hpp"
#include "3rdParty/GTA2Hax/d3ddll/d3ddll.hpp" // CC
#include "gbh_graphics.hpp"
#include "3rdParty/GTA2Hax/3rdParty/imgui/imgui.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <direct.h>
#include <math.h>
#include "BotCompat.hpp"

Bot* Bot::sInstance = NULL;

void Bot_RoutePlanned(const char* why, NavCar* road, float sx, float sy, float tx, float ty)
{
    Bot::RoutePlannedStatic(why, road, sx, sy, tx, ty);
}

void Bot::RoutePlannedStatic(const char* why, NavCar* road, float sx, float sy, float tx, float ty)
{
    if (sInstance)
    {
        sInstance->OnRoutePlanned(why, road, sx, sy, tx, ty);
    }
}

// A drive route was planned: a picture of it (bot_out/<run>/route_N.bmp and route_latest.bmp) and, with the checkbox on, a pause
void Bot::OnRoutePlanned(const char* why, NavCar* road, float sx, float sy, float tx, float ty)
{
    if (!mUiOn || !road)
    {
        return;
    }
    mRouteCount++;
    char path[MAX_PATH];
    _snprintf(path, sizeof(path), "%s\\route_%d.bmp", mOpts.out_dir, mRouteCount);
    const bool ok = road->WriteRouteBmp(path, sx, sy, tx, ty);
    char latest[MAX_PATH];
    _snprintf(latest, sizeof(latest), "%s\\route_latest.bmp", mOpts.out_dir);
    road->WriteRouteBmp(latest, sx, sy, tx, ty);
    _snprintf(mRouteLast, sizeof(mRouteLast), "route %d (%s) (%.0f,%.0f) to (%.0f,%.0f), %u waypoints: %s%s", mRouteCount, why, sx, sy, tx, ty, road->Count(), path, ok ? "" : " (NOT WRITTEN)");
    BotDecision("f=%d %s", mFrame, mRouteLast);
    if (mPauseOnRoute)
    {
        mPaused = true;
    }
}

void __stdcall Bot::UiCallback()
{
    if (sInstance)
    {
        sInstance->DrawUi();
    }
}

// Hooks the overlay into d3ddll's BeginScene callback (present when the Hax video dll is in use)
void Bot::RegisterUi()
{
    sInstance = this;
    HMODULE dll = GetModuleHandleA("d3ddll.dll");
    T_gbh_SetBeginSceneCB set_cb = dll ? reinterpret_cast<T_gbh_SetBeginSceneCB>(GetProcAddress(dll, "gbh_SetBeginSceneCB")) : NULL;
    if (!set_cb)
    {
        BotLog("bot: overlay not available (d3ddll.dll has no gbh_SetBeginSceneCB: the original video dll)");
        return;
    }
    set_cb(UiCallback);
    mUiOn = true;
    BotLog("bot: overlay on");
}

// The game's world to the overlay's pixels: through the game's own camera, so it follows the zoom and the pan
static bool Ui_Project(float x, float y, float z, ImVec2* out)
{
    Camera* cam = gViewCamera_676978;
    if (!cam || cam->field_68_screen_px_width <= 0)
    {
        return false;
    }
    Fix16 sx;
    Fix16 sy;
    cam->ProjectWorldToScreen_4B90E0(Fix16(x), Fix16(y), Fix16(z), &sx, &sy);
    const ImVec2 disp = ImGui::GetIO().DisplaySize;
    out->x = sx.ToFloat() * disp.x / static_cast<float>(cam->field_68_screen_px_width);
    out->y = sy.ToFloat() * disp.y / static_cast<float>(cam->field_6C_screen_px_height);
    return true;
}

// Lines and rings over the whole game view, from what the actions put in BotDebugDraw this frame
static void Ui_DrawWorld(float z)
{
    const ImVec2 disp = ImGui::GetIO().DisplaySize;
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::Begin("##botdraw", NULL, disp, 0.0f,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs |
                     ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus);
    ImDrawList* dl = ImGui::GetWindowDrawList();
    for (int i = 0; i < BotDebugDraw::LineCount(); i++)
    {
        const BotDebugDraw::DebugLine& l = BotDebugDraw::LineAt(i);
        ImVec2 a;
        ImVec2 b;
        if (Ui_Project(l.x0, l.y0, z, &a) && Ui_Project(l.x1, l.y1, z, &b))
        {
            dl->AddLine(a, b, l.col, 2.0f);
        }
    }
    for (int i = 0; i < BotDebugDraw::MarkCount(); i++)
    {
        const BotDebugDraw::DebugMark& m = BotDebugDraw::MarkAt(i);
        ImVec2 c;
        ImVec2 e;
        if (Ui_Project(m.x, m.y, z, &c) && Ui_Project(m.x + m.radius, m.y, z, &e))
        {
            float r = e.x - c.x;
            r = r < 4.0f ? 4.0f : r;
            dl->AddCircle(c, r, m.col, 16, 2.0f);
            if (m.label[0])
            {
                dl->AddText(ImVec2(c.x + r + 2.0f, c.y - 6.0f), m.col, m.label);
            }
        }
    }
    ImGui::End();
}

void Bot::DrawUi()
{
    if (!mWorld)
    {
        return;
    }
    const WorldModel::PlayerInfo& pl = mWorld->player;
    if (mDrawPath)
    {
        Ui_DrawWorld(pl.z);
    }
    ImGui::Begin("Bot", NULL, ImVec2(460, 640), 0.85f);

    if (mPaused)
    {
        ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.2f, 1.0f), "PAUSED: look at the route picture, then");
        if (ImGui::Button("Resume"))
        {
            mPaused = false;
        }
        ImGui::Separator();
    }
    ImGui::Checkbox("draw the path on the game view (cyan route, yellow next waypoint, magenta aim, green steering, red cars)", &mDrawPath);
    ImGui::Checkbox("pause when a drive route is planned", &mPauseOnRoute);
    if (mRouteLast[0])
    {
        ImGui::TextWrapped("%s", mRouteLast);
    }
    ImGui::Separator();
    if (ImGui::Button("WTF!"))
    {
        mWtfRequested = true;
    }
    ImGui::SameLine();
    ImGui::Text("saves the state from several seconds ago");
    if (mWtfLast[0])
    {
        ImGui::TextWrapped("last: %s", mWtfLast);
    }
    ImGui::Separator();

    ImGui::Text("frame %d   profile %s   seed %u", mFrame, mOpts.profile, mOpts.seed);
    ImGui::Text("goal chain:");
    int depth = 0;
    for (Action* a = (mActionIdx >= 0 && mActionIdx < mActionCount) ? mActions[mActionIdx] : NULL; a && depth < 12; a = a->Active(), depth++)
    {
        char indent[40];
        int n = depth * 2;
        if (n > 38)
        {
            n = 38;
        }
        memset(indent, ' ', n);
        indent[n] = 0;
        ImGui::Text("%s%s", indent, a->Name());
    }
    ImGui::Separator();

    ImGui::Text("at (%.1f, %.1f)  hp %d  armour %d  wanted %d  cash %d", pl.x, pl.y, pl.hp, pl.armour, pl.wanted_stars, pl.cash);
    ImGui::Text("weapon %d  ammo %d   %s%s", pl.weapon, pl.ammo, pl.in_car ? "in car " : "on foot", pl.dead ? "  DEAD/BUSTED" : "");
    if (pl.in_car)
    {
        ImGui::Text("car #%d  damage %d", pl.car_id, pl.car_damage);
    }
    ImGui::Text("mission %s  passed %d  phones ringing %d", mWorld->on_mission ? "ON" : "off", mWorld->missions_passed, mWorld->phones_ringing);
    if (mWorld->arrow.valid)
    {
        const float dx = mWorld->arrow.x - pl.x;
        const float dy = mWorld->arrow.y - pl.y;
        ImGui::Text("arrow type %d at (%.0f, %.0f), %.0f tiles", mWorld->arrow.type, mWorld->arrow.x, mWorld->arrow.y, sqrtf(dx * dx + dy * dy));
    }
    if (mWorld->brief_text[0])
    {
        ImGui::TextWrapped("brief: %s", mWorld->brief_text);
    }
    char resp[120];
    char* rp = resp;
    for (int g = 0; g < mWorld->gang_count && g < 5; g++)
    {
        rp += _snprintf(rp, resp + sizeof(resp) - rp, "%s %d  ", mWorld->gang_names[g], mWorld->respect[g]);
    }
    *rp = 0;
    ImGui::Text("respect: %s", resp);
    ImGui::Text("input bits %x   peds %d  cars %d  cops near: %s", mInputBits, mWorld->ped_count, mWorld->car_count, mWorld->CopsNear(14.0f) ? "YES" : "no");
    ImGui::Separator();

    ImGui::Text("decisions (newest last):");
    ImGui::BeginChild("decisions", ImVec2(0, 0), true);
    const u32 total = BotDecision_Count();
    const u32 first = total > 40 ? total - 40 : 0;
    for (u32 i = first; i < total; i++)
    {
        ImGui::TextWrapped("%s", BotDecision_At(i));
    }
    ImGui::SetScrollHere(1.0f);
    ImGui::EndChild();
    ImGui::End();
}

// Called every logic frame while the overlay is on: a snapshot every two seconds of real time into a ring of five, so
// that the oldest is eight to ten seconds old; and a WTF waiting to be saved
void Bot::UiFrame()
{
    if (!mUiOn)
    {
        return;
    }
    const u32 now = GetTickCount();
    if (now - mRingTick > 2000 && mFrame > 100)
    {
        mRingTick = now;
        EnsureMap();
        if (!mRing[mRingNext])
        {
            mRing[mRingNext] = new Snapshot(mMap);
        }
        mRing[mRingNext]->Capture();
        mRingFrame[mRingNext] = mFrame;
        mRingNext = (mRingNext + 1) % kRing;
    }
    if (mOpts.wtf_at_frame >= 0 && mRunFrame == mOpts.wtf_at_frame)
    {
        mWtfRequested = true;
    }
    if (mWtfRequested)
    {
        mWtfRequested = false;
        SaveWtf();
    }
}

// The WTF button: the oldest ring snapshot (before whatever went wrong) into bot_out/<run>/wtf_N/state.snap, with notes.txt
// holding the goal chain, the state and the decisions up to now, and the command to run it again:
//   python3 Scripts/bot/wtf_replay.py <wtf dir> [--show]
void Bot::SaveWtf()
{
    mWtfCount++;
    char dir[MAX_PATH];
    _snprintf(dir, sizeof(dir), "%s\\wtf_%d", mOpts.out_dir, mWtfCount);
    _mkdir(dir);
    // the oldest snapshot taken
    int best = -1;
    for (int i = 0; i < kRing; i++)
    {
        if (mRing[i] && mRingFrame[i] > 0 && (best < 0 || mRingFrame[i] < mRingFrame[best]))
        {
            best = i;
        }
    }
    char snap_path[MAX_PATH];
    _snprintf(snap_path, sizeof(snap_path), "%s\\state.snap", dir);
    bool saved = false;
    if (best >= 0)
    {
        saved = mRing[best]->SaveToFile(snap_path);
    }
    char notes_path[MAX_PATH];
    _snprintf(notes_path, sizeof(notes_path), "%s\\notes.txt", dir);
    FILE* f = fopen(notes_path, "w");
    if (f)
    {
        fprintf(f, "WTF #%d pressed at frame %d; state saved from frame %d (%s)\n", mWtfCount, mFrame, best >= 0 ? mRingFrame[best] : -1, saved ? "ok" : "NOT SAVED");
        fprintf(f, "profile %s map %s seed %u\n", mOpts.profile, mOpts.map, mOpts.seed);
        fprintf(f, "command line: %s\n", mOpts.cmdline);
        fprintf(f, "goal chain:");
        for (Action* a = (mActionIdx >= 0 && mActionIdx < mActionCount) ? mActions[mActionIdx] : NULL; a; a = a->Active())
        {
            fprintf(f, " > %s", a->Name());
        }
        fprintf(f, "\n\n");
        char text[4000];
        mWorld->Text(text, sizeof(text), "state at the WTF");
        fprintf(f, "%s\n", text);
        fprintf(f, "decisions:\n");
        const u32 total = BotDecision_Count();
        const u32 first = total > 150 ? total - 150 : 0;
        for (u32 i = first; i < total; i++)
        {
            fprintf(f, "%s\n", BotDecision_At(i));
        }
        fclose(f);
    }
    _snprintf(mWtfLast, sizeof(mWtfLast), "wtf_%d at frame %d, state from frame %d: %s", mWtfCount, mFrame, best >= 0 ? mRingFrame[best] : -1, dir);
    BotLog("bot: WTF %s", mWtfLast);
    BotDecision("f=%d WTF #%d saved in %s (state from frame %d)", mFrame, mWtfCount, dir, best >= 0 ? mRingFrame[best] : -1);
}
