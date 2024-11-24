#include "Game_0x40.hpp"
#include "CokeZero_100.hpp"
#include "DrawUnk_0xBC.hpp"
#include "Function.hpp"
#include "Garox_2B00.hpp"
#include "Mike_A80.hpp"
#include "Montana.hpp"
#include "Nanobotz.hpp"
#include "PurpleDoom.hpp"
#include "angry_lewin_0x85C.hpp"
#include "debug.hpp"
#include "laughing_blackwell_0x1EB54.hpp"
#include "lucid_hamilton.hpp"
#include "root_sound.hpp"
#include "sprite.hpp"
#include "winmain.hpp"

DrawUnk_0xBC* gViewCamera_676978;

// TODO: Move to d3ddll.h or something
int (*pgbh_BeginScene_626CC0)();
void (*pgbh_EndScene_626CC4)();

short word_706600;

// === start wip hook code ===
// TODO: This will get moved later
#include <vector>

class GlobalRef;

class GlobalsRegistry
{
  public:
    void Add(GlobalRef* pRef)
    {
        mGlobals.push_back(pRef);
    }

    std::vector<GlobalRef*> mGlobals;
};

GlobalsRegistry& GetGlobalsRegistry()
{
    static GlobalsRegistry reg;
    return reg;
}

class GlobalRef
{
  public:
    GlobalRef(void* pVar, u32 addr) : mVar(pVar), mOgAddr(addr)
    {
        GetGlobalsRegistry().Add(this);
    }
    const void* mVar;
    const u32 mOgAddr;
};

#define GLOBAL(var, addr) const GlobalRef gRef_##var(&var, addr);
// === end wip hook code ===

EXPORT_VAR Game_0x40* gGame_0x40_67E008;
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

MATCH_FUNC(0x4B9270)
void Game_0x40::sub_4B9270()
{
    if (gDo_show_counter_67D6E6)
    {
        ShowCounters_4B8FF0();
    }

    if (field_30)
    {
        Garox_C4* pC4 = gGarox_2B00_706620->field_DC.field_650.sub_5D1F50(L"FF", 0, 440, word_706600, 1);
        pC4->field_B0 = 8;
        pC4->field_B4 = 6;
    }
}

MATCH_FUNC(0x4B92D0)
void Game_0x40::Draw_4B92D0()
{
    gViewCamera_676978 = &field_1C_unk->field_14C_view_camera;

    gpNanobotz_6F66E4->ClearDrawnTileCount_4F6A10();
    gSprite_8_703820->sub_5A5860();
    gMontana_67B580->ResetAll_4954F0();

    gPurpleDoom_3_679210->sub_477A40();
    gPurpleDoom_2_67920C->sub_477A40();
    gPurpleDoom_1_679208->sub_477A40();

    sub_5D7D30();

    pgbh_BeginScene_626CC0();

    gpNanobotz_6F66E4->Draw_4F6A20();

    if (!bExplodingOff_67D4FB)
    {
        gCokeZero_100_702F34->DrawExploding_5969E0();
    }

    gGarox_2B00_706620->DrawGui_5D6860(); // user

    if (bDo_mike_67D5CC)
    {
        gMike_A80_6F7328->DebugDrawProfiling_4FF250();
    }

    pgbh_EndScene_626CC4();
    FreeSurface_5D7DC0();
}

MATCH_FUNC(0x4B9380)
void Game_0x40::sub_4B9380()
{
    angry_lewin_0x85C** ppPlayersIterator = field_4_players;
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
angry_lewin_0x85C* Game_0x40::sub_4B9750()
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
    DrawUnk_0xBC* pCam = IteratePlayerCamera_4B9BC0();
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
s8 Game_0x40::sub_4B97E0(Car_3C* a2, s32 a3)
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
s8 Game_0x40::sub_4B9830(Car_3C* pCarSprite, s32 a3)
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

MATCH_FUNC(0x4B9890)
s8 Game_0x40::sub_4B9890(s16* a2, s32 a3)
{
    for (u8 i = 0; i < field_23_max_idx; i++)
    {
        if (sub_4B98E0(a2, i, a3))
        {
            return 1;
        }
    }
    return 0;
}

MATCH_FUNC(0x4B98E0)
bool Game_0x40::sub_4B98E0(s16* a2, u8 playerIdx, s32 a4)
{
    if (field_4_players[playerIdx]->field_8E_bInUse)
    {
        if (field_4_players[playerIdx]->field_90_game_camera.sub_435630(a2, a4))
        {
            return true;
        }
        return field_4_players[playerIdx]->field_2D0 && field_4_players[playerIdx]->field_208_aux_game_camera.sub_435630(a2, a4) ? true :
                                                                                                                                   false;
    }
    return false;
}

STUB_FUNC(0x4B9950)
s8 Game_0x40::sub_4B9950(Car_3C* pCarSprite, u8 idx, s32 a4)
{
    // TODO: Requires Car_3C stubs
    return 0;
}

MATCH_FUNC(0x4B9A10)
bool Game_0x40::sub_4B9A10(Car_3C* a2, u8 playerIdx)
{
    if (field_4_players[playerIdx]->field_8E_bInUse)
    {
        if (field_4_players[playerIdx]->field_90_game_camera.sub_4355D0(a2))
        {
            return true;
        }
        return field_4_players[playerIdx]->field_2D0 && field_4_players[playerIdx]->field_208_aux_game_camera.sub_4355D0(a2) ? true : false;
    }
    return false;
}

// wip - prob needs Fix16 operators ??
STUB_FUNC(0x4B9A80)
s8 Game_0x40::is_point_on_screen_4B9A80(s32 a2_fp, s32 a3_fp)
{
    for (s32 i = 0; i < field_23_max_idx; i++)
    {
        if (field_4_players[i]->field_8E_bInUse)
        {
            if ((a2_fp >= field_4_players[i]->field_90_game_camera.field_78 && a2_fp <= field_4_players[i]->field_90_game_camera.field_7C &&
                 a3_fp >= field_4_players[i]->field_90_game_camera.field_80 &&
                 a3_fp <= field_4_players[i]->field_90_game_camera.field_84) ||
                (field_4_players[i]->field_2D0 && a2_fp >= field_4_players[i]->field_208_aux_game_camera.field_78 &&
                 a2_fp <= field_4_players[i]->field_208_aux_game_camera.field_7C &&
                 a3_fp >= field_4_players[i]->field_208_aux_game_camera.field_80 &&
                 a3_fp <= field_4_players[i]->field_208_aux_game_camera.field_84))
            {
                return 1;
            }
        }
    }
    return 0;
}

STUB_FUNC(0x4B9B10)
s8 Game_0x40::sub_4B9B10(Fix16_Rect* pBounds)
{
    // wip
    for (u8 i = 0; i < field_23_max_idx; i++)
    {
        angry_lewin_0x85C* pCurPlayer = field_4_players[i];
        if (pCurPlayer->field_8E_bInUse)
        {
            if (pBounds->field_8_top <= pCurPlayer->field_90_game_camera.field_2C_top &&
                pBounds->field_C_bottom >= pCurPlayer->field_90_game_camera.field_28_bottom &&
                pBounds->field_0_left <= pCurPlayer->field_90_game_camera.field_24_left &&
                pBounds->field_4_right >= pCurPlayer->field_90_game_camera.field_20_right)
            {
                return 1;
            }
            if (pCurPlayer->field_2D0 && pBounds->field_8_top <= pCurPlayer->field_208_aux_game_camera.field_2C_top &&
                pBounds->field_C_bottom >= pCurPlayer->field_208_aux_game_camera.field_28_bottom &&
                pBounds->field_0_left <= pCurPlayer->field_208_aux_game_camera.field_24_left &&
                pBounds->field_4_right >= pCurPlayer->field_208_aux_game_camera.field_20_right)
            {
                return 1;
            }
        }
    }
    return 0;
}

MATCH_FUNC(0x4B9BC0)
DrawUnk_0xBC* Game_0x40::IteratePlayerCamera_4B9BC0()
{
    for (field_21_player_camera_idx = 0; field_21_player_camera_idx < field_23_max_idx; field_21_player_camera_idx++)
    {
        if (field_4_players[field_21_player_camera_idx]->field_8E_bInUse)
        {
            field_22 = 0;
            return &field_4_players[field_21_player_camera_idx]->field_90_game_camera;
        }
    }
    return NULL;
}

MATCH_FUNC(0x4B9C10)
s8 Game_0x40::sub_4B9C10(Car_BC* a2)
{
    angry_lewin_0x85C* pPlayer = sub_4B9CD0();
    while (pPlayer)
    {
        if (pPlayer->sub_564610(a2, 1))
        {
            return 1;
        }
        pPlayer = IterateNextPlayer_4B9D10();
    }
    return 0;
}

STUB_FUNC(0x4B9C50)
DrawUnk_0xBC* Game_0x40::sub_4B9C50()
{
    return 0;
}

MATCH_FUNC(0x4B9CD0)
angry_lewin_0x85C* Game_0x40::sub_4B9CD0()
{
    for (field_20_idx = 0; field_20_idx < field_23_max_idx; field_20_idx++)
    {
        if (field_4_players[field_20_idx]->field_8E_bInUse)
        {
            return field_4_players[field_20_idx];
        }
    }
    return 0;
}

MATCH_FUNC(0x4B9D10)
angry_lewin_0x85C* Game_0x40::IterateNextPlayer_4B9D10()
{
    while (++field_20_idx < field_23_max_idx)
    {
        if (field_4_players[field_20_idx]->field_8E_bInUse)
        {
            return field_4_players[field_20_idx];
        }
    }
    return 0;
}

STUB_FUNC(0x4B9D60)
void Game_0x40::sub_4B9D60(angry_lewin_0x85C* a2)
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
