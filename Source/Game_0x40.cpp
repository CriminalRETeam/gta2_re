#include "Game_0x40.hpp"
#include "Function.hpp"
#include "debug.hpp"
#include "root_sound.hpp"
#include "lucid_hamilton.hpp"
#include "angry_lewin_0x85C.hpp"
#include "Garox_2B00.hpp"
#include "DrawUnk_0xBC.hpp"
#include "winmain.hpp"

// === start wip hook code ===
// TODO: This will get moved later
#include <vector>

class GlobalRef;

class GlobalsRegistry
{
public:
    void Add(GlobalRef *pRef)
    {
        mGlobals.push_back(pRef);
    }

    std::vector<GlobalRef *> mGlobals;
};

GlobalsRegistry &GetGlobalsRegistry()
{
    static GlobalsRegistry reg;
    return reg;
}

class GlobalRef
{
public:
    GlobalRef(void *pVar, u32 addr)
        : mVar(pVar), mOgAddr(addr)
    {
        GetGlobalsRegistry().Add(this);
    }
    const void *mVar;
    const u32 mOgAddr;
};

#define GLOBAL(var, addr) const GlobalRef gRef_##var(&var, addr);
// === end wip hook code ===

EXPORT_VAR Game_0x40 *gGame_0x40_67E008;
GLOBAL(gGame_0x40_67E008, 0x67E008)

STUB_FUNC(0x4B8BB0)
s32 Game_0x40::sub_4B8BB0()
{
    return 0;
}

MATCH_FUNC(0x4B8BD0)
void Game_0x40::sub_4B8BD0(s32 new_timer, s32 main_state, s8 a2)
{
    int timer = this->field_28_timer;
    if (timer == -1 || timer > new_timer)
    {
        this->field_28_timer = new_timer;
        this->field_2C_main_state = main_state;
        gLucid_hamilton_67E8E0.sub_4C5930(a2);
    }
}

MATCH_FUNC(0x4B8C00)
void Game_0x40::sub_4B8C00(s32 a1, s32 a2)
{
    sub_4B8BD0(a1, a2, 0);
}

MATCH_FUNC(0x4B8C20)
s8 Game_0x40::sub_4B8C20()
{
    if (field_28_timer == 0)
    {
        return 1;
    }

    if (field_28_timer > 0)
    {
        field_28_timer--;
    }

    return 0;
}

STUB_FUNC(0x4B8C40)
void Game_0x40::sub_4B8C40()
{
}

MATCH_FUNC(0x4B8E00)
void Game_0x40::sub_4B8E00(u32 a1, u32 a2)
{
    field_38_orf1->field_90_game_camera.sub_4361B0(a1, a2);
    field_38_orf1->field_208_aux_game_camera.sub_4361B0(a1, a2);
    field_38_orf1->field_14C_view_camera.sub_4361B0(a1, a2);
    gGarox_2B00_706620->sub_5D6AB0();
}

STUB_FUNC(0x4B8E50)
void Game_0x40::sub_4B8E50()
{
}

STUB_FUNC(0x4B8EB0)
void Game_0x40::sub_4B8EB0()
{
}

STUB_FUNC(0x4B8FF0)
void Game_0x40::ShowCounters_4B8FF0()
{
}

STUB_FUNC(0x4B9270)
s8 Game_0x40::sub_4B9270(u16 *a2) // TODO: user call 2nd param
{
    return 0;
}

STUB_FUNC(0x4B92D0)
void Game_0x40::Draw_4B92D0()
{
    // TODO
}

MATCH_FUNC(0x4B9380)
void Game_0x40::sub_4B9380()
{
    angry_lewin_0x85C **ppPlayersIterator = field_4_players;
    u32 idx = 0;
    while (idx < field_23_max_idx)
    {
        if ((*ppPlayersIterator)->field_8E_bInUse)
        {
            (*ppPlayersIterator)->sub_566820();
        }
        ++idx;
        ++ppPlayersIterator;
    }
}

STUB_FUNC(0x4B93C0)
s32 Game_0x40::sub_4B93C0()
{
    return 0;
}

STUB_FUNC(0x4B9410)
void Game_0x40::sub_4B9410()
{
}

STUB_FUNC(0x4B9640)
s8 Game_0x40::sub_4B9640() // TODO: 1 instruction swapped
{
    sub_4B9380();

    switch (field_0_game_state)
    {
    case 0:
        sub_4B9410();

        if (!bSkip_audio_67D6BE)
        {
            gRoot_sound_66B038.Service_40EFA0();
        }
        field_0_game_state = 2;
        return sub_4B8C20();

    case 1:
        sub_4B9410();
        if (!bSkip_audio_67D6BE)
        {
            gRoot_sound_66B038.Service_40EFA0();
        }
        return sub_4B8C20();

    case 2:
        sub_4B93C0();
        if (!bSkip_audio_67D6BE)
        {
            gRoot_sound_66B038.Service_40EFA0();
        }
        return sub_4B8C20();

    default:
        return sub_4B8C20();
    }
}

STUB_FUNC(0x4B9700)
void Game_0x40::sub_4B9700()
{
}

MATCH_FUNC(0x4B9710)
void Game_0x40::sub_4B9710()
{
    if (field_23_max_idx == 1)
    {
        field_0_game_state = 0;
    }
}

MATCH_FUNC(0x4B9720)
void Game_0x40::sub_4B9720()
{
    if (field_38_orf1)
    {
        field_38_orf1->sub_565460();
    }

    sub_4DA830();

    if (gGarox_2B00_706620)
    {
        gGarox_2B00_706620->sub_5D6A90();
    }
}

MATCH_FUNC(0x4B9750)
angry_lewin_0x85C *Game_0x40::sub_4B9750()
{
    for (s32 i = 0; i < field_23_max_idx; i++)
    {
        if (field_4_players[i]->field_8E_bInUse && !field_4_players[i]->field_2C4_player_ped)
        {
            return field_4_players[i];
        }
    }
    return NULL;
}

MATCH_FUNC(0x4B9790)
void Game_0x40::sub_4B9790(s32 a2, s32 a3, s32 a4)
{
    DrawUnk_0xBC *pCam = IteratePlayerCamera_4B9BC0();
    while (pCam)
    {
        if (a3 >= pCam->field_78 && a3 <= pCam->field_7C && a4 >= pCam->field_80 && a4 <= pCam->field_84)
        {
            pCam->sub_436120(a2);
        }
        pCam = sub_4B9C50();
    }
}

MATCH_FUNC(0x4B97E0)
s8 Game_0x40::sub_4B97E0(Car_3C *a2, s32 a3)
{
    for (u8 i = 0; i < field_23_max_idx; i++)
    {
        if (sub_4B9950(a2, i, a3))
        {
            return 1;
        }
    }
    return 0;
}

MATCH_FUNC(0x4B9830)
s8 Game_0x40::sub_4B9830(Car_3C *pCarSprite, s32 a3)
{
    for (u8 i = 0; i < field_23_max_idx; i++)
    {
        if (sub_4B9950(pCarSprite, i, a3) || sub_4B9A10(pCarSprite, i))
        {
            return 1;
        }
    }
    return 0;
}

STUB_FUNC(0x4B9890)
s8 Game_0x40::sub_4B9890(s32 a2, s32 a3)
{
    return 0;
}

STUB_FUNC(0x4B98E0)
bool Game_0x40::sub_4B98E0(s16 *a2, u8 playerIdx, s32 a4)
{
    return false;
}

STUB_FUNC(0x4B9950)
s8 Game_0x40::sub_4B9950(Car_3C *pCarSprite, u8 idx, s32 a4)
{
    return 0;
}

STUB_FUNC(0x4B9A10)
bool Game_0x40::sub_4B9A10(Car_3C *a2, u8 playerIdx)
{
    return false;
}

STUB_FUNC(0x4B9A80)
s8 Game_0x40::is_point_on_screen_4B9A80(s32 a2_fp, s32 a3_fp)
{
    return 0;
}

STUB_FUNC(0x4B9B10)
s8 Game_0x40::sub_4B9B10(s32 *pBounds)
{
    return 0;
}

STUB_FUNC(0x4B9BC0)
DrawUnk_0xBC *Game_0x40::IteratePlayerCamera_4B9BC0()
{
    return 0;
}

STUB_FUNC(0x4B9C10)
s8 Game_0x40::sub_4B9C10(Car_BC *a2)
{
    return 0;
}

STUB_FUNC(0x4B9C50)
DrawUnk_0xBC *Game_0x40::sub_4B9C50()
{
    return 0;
}

STUB_FUNC(0x4B9CD0)
angry_lewin_0x85C *Game_0x40::sub_4B9CD0()
{
    return 0;
}

STUB_FUNC(0x4B9D10)
angry_lewin_0x85C *Game_0x40::IterateNextPlayer_4B9D10()
{
    return 0;
}

STUB_FUNC(0x4B9D60)
void Game_0x40::sub_4B9D60(angry_lewin_0x85C *a2)
{
}

STUB_FUNC(0x4B9DE0)
Game_0x40::Game_0x40(u8 max_players, s8 player_idx) // 4B9DE0
{
}

STUB_FUNC(0x4BAE30)
Game_0x40::~Game_0x40() // 4BAE30
{
}
