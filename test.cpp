#include "stdafx.h"
#include "resource.h"
#include "Source/gtx_0x106C.hpp"
#include "Source/registry.hpp"
#include "Source/file.hpp"
#include "Source/sharp_bose_0x54.hpp"
#include "source/lucid_hamilton.hpp"
#include "Source/winmain.hpp"
#include "Source/text_0x14.hpp"
#include "Source/gbh_graphics.hpp"
#include "Source/distracted_einstein_0xC.hpp"
#include "Source/sharp_bose_0x54.hpp"
#include "Source/cSampleManager.hpp"
#include "Source/map_0x370.hpp"
#include "Source/frosty_pasteur_0xC1EA8.hpp"

#pragma comment(lib, "Winmm.lib")

void test_file()
{
    File::GetFileSize_4A6B10(0);
    File::IsCdRomDrive_4A6BB0(0);
    File::ReadFileToBuffer_4A6C80(0, 0);
    File::Read_4A6D90(0, 0, 0, 0);
    File::WriteBufferToFile_4A6E80(0, 0, 0);
    File::Write_4A6F30(0, 0, 0, 0);
    File::AppendBufferToFile_4A6F50(0, 0, 0);
    File::CreateFile_4A7000(0);
    File::Global_Open_4A7060("lol.txt");
    File::Global_Close_4A70C0();
    File::Global_Close_UnChecked_4A7110();
    File::Global_Seek_4A7140(0);
    File::File_Error_4A7190(0, 0, 0);
    File::Global_Read_4A71C0(0, 0);
    File::GetRemainderSize_4A7250(0, 0);
    File::SkipWhitespace_4A7340(0);
}

void test_gtx_0x106C()
{
    gtx_0x106C c;
    c.get_car_info_5AA3B0(0);
    c.get_car_remap_5AA3D0(0);
    c.get_sprite_index_5AA440(0);
    c.convert_sprite_pal_5AA460(0, 0);
    c.sub_5AA4F0(0);
    c.sub_5AA560(0);
    c.convert_pal_type_5AA5F0(0, 0);
    c.GetPalData_5AA6A0(0);
    c.get_phys_pal_5AA6F0(0);
    c.sub_5AA710(0, 0);
    c.sub_5AA760(0, 0);
    c.space_width_5AA7B0(0);
    c.sub_5AA800(0);
    c.sub_5AA850(0);
    c.sub_5AA870(0);
    c.sub_5AA890();
    c.get_physical_palettes_len_5AA900();
    c.get_map_object_info_5AA910(0);
    c.sub_5AA930(0, 0);
    c.create_tile_num_array_5AA950();
    c.sub_5AA9A0(0);
    c.sub_5AAB30(0);
    c.sub_5AABF0();
    c.SetSpriteIndexDataPtrs_5AAC40();
    c.sub_5AAC70();
    c.load_car_info_5AAD50(0);
    c.load_delta_index_5AAD80(0);
    c.load_delta_store_5AADD0(0);
    c.load_tiles_5AADF0(0);
    c.skip_ovly_5AAE20(0);
    c.skip_psxt_5AAE30(0);
    c.load_sprite_graphics_5AAE40(0);
    c.load_physical_palettes_5AAE70(0);
    c.load_palette_index_5AAEA0(0);
    c.load_map_object_info_5AAF00(0);
    c.load_sprite_index_5AAF80(0);
    c.sub_5AAFE0(0);
    c.load_font_base_5AB0F0(0);
    c.ConvertToVirtualOffsets_5AB1A0(0, 0);
    c.ConvertToVirtualOffsets_5AB1C0(0, 0);
    c.load_sprite_base_5AB210(0);
    c.load_palete_base_5AB2C0(0);
    c.sub_5AB380(0);
    c.load_car_recycling_info_5AB3C0(0);
    c.read_spec_5AB3F0(0);
    c.load_spec_5AB450();
    c.LoadChunk_5AB4B0(0, 0);
    c.sub_5AB720();
    c.LoadSty_5AB750(0);
}

void test_registry()
{
    gRegistry_6FF968.Open_Sound_Root_586A00(NULL);
    gRegistry_6FF968.Get_Sound_Settting_586A70("lol");
    gRegistry_6FF968.Set_Sound_Setting_586AE0("lol", 5);
}

void test_lucid_hamilton()
{
    lucid_hamilton c;
    c.sub_4C5920(0);
}

void test_winmain()
{
    Start_GTA2Manager_5E4DE0();
    ErrorMsgBox_5E4EC0("meh");
}

void test_text_0x14()
{
    text_0x14 t;
}

void test_gbh_graphics()
{
    GBH_GraphicsLoadDll_5EA680("meh.dll");
}

void test_distracted_einstein_0xC()
{
    distracted_einstein_0xC t;
    t.sub_5BEBF0();
}

void test_sharp_bose_0x54()
{
    sharp_bose_0x54 t;
    t.sub_5BECF0(0, 0);
}

void test_wizardly_margulis()
{
    gSampManager_6FFF00.SoundInit_58D6C0(0);
    gSampManager_6FFF00.AllocSamples_58D9F0(0);
}

void test_Map_0x370()
{
    Map_0x370 t;
    t.get_block_452980(0, 0, 0);
}

void test_frosty_pasteur_0xC1EA8()
{
    frosty_pasteur_0xC1EA8 t;
    t.Load_512330("lol.scr");
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
        test_distracted_einstein_0xC();
        test_sharp_bose_0x54();
        test_registry();
        test_file();
        test_gtx_0x106C();
        test_lucid_hamilton();
        test_winmain();
        test_wizardly_margulis();
        test_Map_0x370();
        test_frosty_pasteur_0xC1EA8();

        force_link();
    }

    WinMain_5E53F0(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

    return 0;
}
