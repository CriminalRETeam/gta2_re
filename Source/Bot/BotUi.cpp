// The bot's overlay: what it is doing and why, drawn with the ImGui that the game's video dll already carries (the debug
// window of ImGuiDebug.cpp uses the same hook). Plus the WTF button, see Bot::SaveWtf.
#include "Bot.hpp"
#include "Snapshot.hpp"
#include "WorldModel.hpp"
#include "BotAction.hpp"
#include "BotLog.hpp"
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

void Bot::DrawUi()
{
    if (!mWorld)
    {
        return;
    }
    const WorldModel::PlayerInfo& pl = mWorld->player;
    ImGui::Begin("Bot", NULL, ImVec2(460, 640), 0.85f);

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
