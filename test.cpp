#include "stdafx.h"
#include "resource.h"
#include "Source/gtx_0x106C.hpp"
#include "Source/registry.hpp"
#include "Source/file.hpp"
#include "Source/FpsCounter.hpp"
#include "source/GameSession.hpp"
#include "Source/winmain.hpp"
#include "Source/TextTable.hpp"
#include "Source/GlobalsRegistry.hpp"
#include "Source/gbh_graphics.hpp"
#include "Source/ProfilerTimer.hpp"
#include "Source/FpsCounter.hpp"
#include "Source/cSampleManager.hpp"
#include "Source/map_0x370.hpp"
#include "Source/ScriptManager.hpp"
#include "Globals.hpp"

#pragma comment(lib, "Winmm.lib")

extern void EnableBoot2MapDebugOptions();

void test_file()
{
    File::GetFileSize(0);
    File::IsCdRomDrive(0);
    File::ReadFileToBuffer(0, 0);
    File::Read(0, 0, 0, 0);
    File::WriteBufferToFile(0, 0, 0);
    File::Write(0, 0, 0, 0);
    File::AppendBufferToFile(0, 0, 0);
    File::CreateFile_4A7000(0);
    File::Global_Open("lol.txt");
    File::Global_Close();
    File::Global_Close_UnChecked();
    File::Global_Seek(0);
    File::File_Error(0, 0, 0);
    File::Global_Read(0, 0);
    File::Global_ReadRemainder(0, 0);
    File::SkipWhitespace(0);
}

void test_gtx_0x106C()
{
    gtx_0x106C c;
    c.get_car_info_5AA3B0(0);
    c.get_car_door_data(0);
    c.get_sprite_index(0);
    c.GetSpriteTrueIndex(0, 0);
    c.GetSpriteBaseOfType(0);
    c.GetPaletteBaseOfType(0);
    c.GetTruePalette(0, 0);
    c.GetPalData(0);
    c.get_phys_pal(0);
    c.GetSpriteIdxFromFont(0, 0);
    c.GetFontWidth(0, 0);
    c.GetSpaceCharWidth(0);
    c.GetLineSpacing(0);
    c.IsTileRemapped(0);
    c.GetTile(0);
    c.GetFirstFreeReservedTileIdx();
    c.get_physical_palettes_len();
    c.get_map_object_info(0);
    c.SetTileRemap(0, 0);
    c.InitTileMapping();
    c.BuildCarInfoContainer(0);
    c.load_delx(0);
    c.SetDeltaDataPtrs();
    c.SetSpriteIndexDataPtrs();
    c.build_delta_container();
    c.load_car_info(0);
    c.load_delta_index(0);
    c.load_delta_store(0);
    c.load_tiles(0);
    c.skip_ovly(0);
    c.skip_psxt(0);
    c.load_sprite_graphics(0);
    c.load_physical_palettes(0);
    c.load_palette_index(0);
    c.load_map_object_info(0);
    c.load_sprite_index(0);
    c.InitFontTypes(0);
    c.load_font_base(0);
    c.SumCounts(0, 0);
    c.ConvertToVirtualOffsets(0, 0);
    c.load_sprite_base(0);
    c.load_palete_base(0);
    c.IsCarModelInRecycleList(0);
    c.load_car_recycling_info(0);
    c.read_spec(0);
    c.load_spec();
    c.LoadChunk(0, 0);
    c.SetDataPtrs();
    c.LoadSty(0);
}

void test_registry()
{
    gRegistry.Open_Sound_Root(NULL);
    gRegistry.Sound_Setting_Exists("lol");
    gRegistry.Get_Sound_Setting("lol", 5);
}

void test_GameSession_578()
{
    GameSession c;
    c.SetPlySlotIdx(0);
}

void test_winmain()
{
    Start_GTA2Manager();
    ErrorMsgBox("meh");
}

void test_text_0x14()
{
    TextTable t;
}

void test_gbh_graphics()
{
    GBH_GraphicsLoadDll("meh.dll");
}

void test_ProfilerTimer_C()
{
    ProfilerTimer t;
    t.AccumulateElapsed();
}

void test_FpsCounter_54()
{
    FpsCounter t;
    t.UpdateFpsCounters(0, 0);
}

void test_wizardly_margulis()
{
    gSampManager.SoundInit(0);
    gSampManager.AllocSamples(0);
}

void test_Map_0x370()
{
    Map_0x370 t;
    t.GetBlockOrNull(0, 0, 0);
}

void test_ScriptManager_C1EA8()
{
    ScriptManager t;
    t.Load("lol.scr");
}

void force_link();

s32 APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     s32       nCmdShow)
 {
    if (lpCmdLine && strstr(lpCmdLine, "billybobbins"))
    {
        test_text_0x14();
        test_gbh_graphics();
        test_ProfilerTimer_C();
        test_FpsCounter_54();
        test_registry();
        test_file();
        test_gtx_0x106C();
        test_GameSession_578();
        test_winmain();
        test_wizardly_margulis();
        test_Map_0x370();
        test_ScriptManager_C1EA8();

        force_link();
    }

    if (lpCmdLine && strstr(lpCmdLine, "--check_vars_only"))
    {
        GetGlobalsRegistry()->CheckVars();
        return 0;
    }

    GetGlobalsRegistry()->CheckVars();

    LoadBeginSceneCBPtr();
    EnableBoot2MapDebugOptions();

    WinMain_5E53F0(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

    return 0;
}
