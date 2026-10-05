#include "Frontend.hpp"
#include "Bink.hpp"
#include "BurgerKing_67F8B0.hpp"
#include "Draw.hpp"
#include "Fix16_Point.hpp"
#include "Function.hpp"
#include "Globals.hpp"
#include "ang16.hpp"
#include "cSampleManager.hpp"
#include "crt_stubs.hpp"
#include "debug.hpp"
#include "dma_video.hpp"
#include "enums.hpp"
#include "error.hpp"
#include "file.hpp"
#include "fix16.hpp"
#include "gbh_graphics.hpp"
#include <stdlib.h>
#include "gtx_0x106C.hpp"
#include "infallible_turing.hpp"
#include "input.hpp"
#include "jolly_poitras_0x2BC0.hpp"
#include "keybrd_0x204.hpp"
#include "lucid_hamilton.hpp"
#include "magical_germain_0x8EC.hpp"
#include "registry.hpp"
#include "root_sound.hpp"
#include "sharp_pare_0x15D8.hpp"
#include "text_0x14.hpp"
#include "winmain.hpp"
#include "youthful_einstein.hpp"
#include <io.h>
#include <stdio.h>
#include <wchar.h>

#pragma comment(lib, "dxguid.lib")

void Start_GTA2Manager_5E4DE0();

DEFINE_GLOBAL(Frontend*, gFrontend_67DC84, 0x67DC84);
DEFINE_GLOBAL_INIT(u32, counter_706C4C, 0, 0x706C4C);
DEFINE_GLOBAL_INIT(s32, dword_67D930, 0, 0x67D930);
u16 gTableSize_61FF20 = 25; // Note is constant but can't be marked const
DEFINE_GLOBAL_ARRAY(wchar_t, gEmptyWStr_67DC8C, 32, 0x67DC8C); // 67DCCC
DEFINE_GLOBAL_INIT(Fix16, kFpOne_67D9FC, Fix16(1), 0x67D9FC);
DEFINE_GLOBAL(short, font_type_703C14, 0x703C14);
DEFINE_GLOBAL(s16, word_703C3C, 0x703C3C);
DEFINE_GLOBAL(s16, word_703D0C, 0x703D0C);
DEFINE_GLOBAL(s16, word_703C16, 0x703C16);
DEFINE_GLOBAL(s16, word_703C8C, 0x703C8C);
DEFINE_GLOBAL(s16, word_703C8A, 0x703C8A);
DEFINE_GLOBAL(s16, word_703BE2, 0x703BE2);
DEFINE_GLOBAL(s16, word_703B88, 0x703B88);
DEFINE_GLOBAL(s16, word_703DAC, 0x703DAC);
DEFINE_GLOBAL(s16, word_703B9C, 0x703B9C);
DEFINE_GLOBAL_INIT(Ang16, kAngZero_67DA70, Ang16(0), 0x67DA70);
DEFINE_GLOBAL_INIT(Fix16, kFpOne_67D934, Fix16(1), 0x67D934);
DEFINE_GLOBAL_ARRAY(wchar_t, tmpBuff_67BD9C, 640, 0x67BD9C);
DEFINE_GLOBAL(BYTE, bIsLeftRightLoopEnabled_67DA80, 0x67DA80);
DEFINE_GLOBAL_ARRAY(wchar_t, gTmpWideStr_67C7D8, 640, 0x67C7D8);
DEFINE_GLOBAL(bool, gCheatOnlyMuggerPeds_67D5A4, 0x67D5A4);
DEFINE_GLOBAL(bool, gCheatUnlimitedElectroGun_67D4F7, 0x67D4F7);
DEFINE_GLOBAL(bool, gCheatAllGangMaxRespect_67D587, 0x67D587);
DEFINE_GLOBAL(bool, gCheatOnlyElvisPeds_67D4ED, 0x67D4ED);
DEFINE_GLOBAL(bool, gCheatNakedPeds_67D5E8, 0x67D5E8);
DEFINE_GLOBAL(bool, gCheatGetBasicWeaponsMaxAmmo_67D545, 0x67D545);
DEFINE_GLOBAL(bool, gCheatGet99Lives_67D4F1, 0x67D4F1);
DEFINE_GLOBAL(bool, gCheatGetPlayerPoints_67D4C8, 0x67D4C8);
DEFINE_GLOBAL(bool, gCheatUnlimitedFlameThrower_67D6CC, 0x67D6CC);
DEFINE_GLOBAL(bool, gCheatUnknown_67D4F6, 0x67D4F6);
DEFINE_GLOBAL(bool, gCheatGet10MillionMoney_67D6CE, 0x67D6CE);
DEFINE_GLOBAL(bool, gCheat10xMultiplier_67D589, 0x67D589);
DEFINE_GLOBAL(bool, gCheatUnlockThreeLevels_67D6CB, 0x67D6CB);
DEFINE_GLOBAL(bool, gCheatUnlockLevelsOneAndTwo_67D584, 0x67D584);
DEFINE_GLOBAL(bool, gCheatUnlockAllLevels_67D538, 0x67D538);
DEFINE_GLOBAL(bool, gCheatUnlimitedDoubleDamage_67D57C, 0x67D57C);
DEFINE_GLOBAL(bool, gCheatInvisibility_67D539, 0x67D539);
DEFINE_GLOBAL(bool, gCheatMiniCars_67D6C8, 0x67D6C8);

int sCheatHashSecret_61F0A8[8] = {829, 761, 23, 641, 43, 809, 677, 191};

MATCH_FUNC(0x4AE010)
LPCSTR __stdcall FreeLoader::GetRegDword_4AE010(HKEY hKey, LPCSTR lpValueName, LPCSTR a3)
{
    DWORD Type = 4;
    if (!RegQueryValueExA(hKey, lpValueName, 0, &Type, (LPBYTE)&lpValueName, &Type) == 0)
    {
        return a3;
    }
    else
    {
        return lpValueName;
    }
}

MATCH_FUNC(0x4AE0F0)
s32 __stdcall FreeLoader::GetCityInstalled_4AE0F0()
{
    HKEY phkResult;
    RegOpenKeyA(HKEY_LOCAL_MACHINE, "Software\\freeloader.com\\GTA2", &phkResult);
    s32 v0 = (s32)FreeLoader::GetRegDword_4AE010(phkResult, "CityInstalled", reinterpret_cast<LPCSTR>(-1));
    RegCloseKey(phkResult);
    return v0;
}

MATCH_FUNC(0x4AE1F0)
EXPORT char_type __stdcall FreeLoader::CheckCityInstalled_4AE1F0(u8 a1)
{
    if (a1 > FreeLoader::GetCityInstalled_4AE0F0())
    {
        ShowWindow(gHwnd_707F04, SW_SHOWMINNOACTIVE);
        PostMessageA(gHwnd_707F04, WM_ACTIVATE, 0, 0);
        tagMSG Msg;

        while (PeekMessageA(&Msg, 0, 0, 0, 1u))
        {
            TranslateMessage(&Msg);
            DispatchMessageA(&Msg);
            Sleep(0xAu);
        }
        HANDLE v1 = OpenMutexA(0x1F0001u, 0, "WEBL_COOP_MUTEX");
        if (!v1)
        {
            ShellExecuteA(0, 0, "WebLaunch.exe", 0, gWorkingDir_707F64, 1);
        }
        else
        {
            CloseHandle(v1);
        }

        return 0;
    }
    return 1;
}

DIOBJECTDATAFORMAT gKeyboardObjectDataFormats_5E9110[256] = {
    {&GUID_Key, 0u, 2147483660u, 0u},   {&GUID_Key, 1u, 2147483916u, 0u},   {&GUID_Key, 2u, 2147484172u, 0u},
    {&GUID_Key, 3u, 2147484428u, 0u},   {&GUID_Key, 4u, 2147484684u, 0u},   {&GUID_Key, 5u, 2147484940u, 0u},
    {&GUID_Key, 6u, 2147485196u, 0u},   {&GUID_Key, 7u, 2147485452u, 0u},   {&GUID_Key, 8u, 2147485708u, 0u},
    {&GUID_Key, 9u, 2147485964u, 0u},   {&GUID_Key, 10u, 2147486220u, 0u},  {&GUID_Key, 11u, 2147486476u, 0u},
    {&GUID_Key, 12u, 2147486732u, 0u},  {&GUID_Key, 13u, 2147486988u, 0u},  {&GUID_Key, 14u, 2147487244u, 0u},
    {&GUID_Key, 15u, 2147487500u, 0u},  {&GUID_Key, 16u, 2147487756u, 0u},  {&GUID_Key, 17u, 2147488012u, 0u},
    {&GUID_Key, 18u, 2147488268u, 0u},  {&GUID_Key, 19u, 2147488524u, 0u},  {&GUID_Key, 20u, 2147488780u, 0u},
    {&GUID_Key, 21u, 2147489036u, 0u},  {&GUID_Key, 22u, 2147489292u, 0u},  {&GUID_Key, 23u, 2147489548u, 0u},
    {&GUID_Key, 24u, 2147489804u, 0u},  {&GUID_Key, 25u, 2147490060u, 0u},  {&GUID_Key, 26u, 2147490316u, 0u},
    {&GUID_Key, 27u, 2147490572u, 0u},  {&GUID_Key, 28u, 2147490828u, 0u},  {&GUID_Key, 29u, 2147491084u, 0u},
    {&GUID_Key, 30u, 2147491340u, 0u},  {&GUID_Key, 31u, 2147491596u, 0u},  {&GUID_Key, 32u, 2147491852u, 0u},
    {&GUID_Key, 33u, 2147492108u, 0u},  {&GUID_Key, 34u, 2147492364u, 0u},  {&GUID_Key, 35u, 2147492620u, 0u},
    {&GUID_Key, 36u, 2147492876u, 0u},  {&GUID_Key, 37u, 2147493132u, 0u},  {&GUID_Key, 38u, 2147493388u, 0u},
    {&GUID_Key, 39u, 2147493644u, 0u},  {&GUID_Key, 40u, 2147493900u, 0u},  {&GUID_Key, 41u, 2147494156u, 0u},
    {&GUID_Key, 42u, 2147494412u, 0u},  {&GUID_Key, 43u, 2147494668u, 0u},  {&GUID_Key, 44u, 2147494924u, 0u},
    {&GUID_Key, 45u, 2147495180u, 0u},  {&GUID_Key, 46u, 2147495436u, 0u},  {&GUID_Key, 47u, 2147495692u, 0u},
    {&GUID_Key, 48u, 2147495948u, 0u},  {&GUID_Key, 49u, 2147496204u, 0u},  {&GUID_Key, 50u, 2147496460u, 0u},
    {&GUID_Key, 51u, 2147496716u, 0u},  {&GUID_Key, 52u, 2147496972u, 0u},  {&GUID_Key, 53u, 2147497228u, 0u},
    {&GUID_Key, 54u, 2147497484u, 0u},  {&GUID_Key, 55u, 2147497740u, 0u},  {&GUID_Key, 56u, 2147497996u, 0u},
    {&GUID_Key, 57u, 2147498252u, 0u},  {&GUID_Key, 58u, 2147498508u, 0u},  {&GUID_Key, 59u, 2147498764u, 0u},
    {&GUID_Key, 60u, 2147499020u, 0u},  {&GUID_Key, 61u, 2147499276u, 0u},  {&GUID_Key, 62u, 2147499532u, 0u},
    {&GUID_Key, 63u, 2147499788u, 0u},  {&GUID_Key, 64u, 2147500044u, 0u},  {&GUID_Key, 65u, 2147500300u, 0u},
    {&GUID_Key, 66u, 2147500556u, 0u},  {&GUID_Key, 67u, 2147500812u, 0u},  {&GUID_Key, 68u, 2147501068u, 0u},
    {&GUID_Key, 69u, 2147501324u, 0u},  {&GUID_Key, 70u, 2147501580u, 0u},  {&GUID_Key, 71u, 2147501836u, 0u},
    {&GUID_Key, 72u, 2147502092u, 0u},  {&GUID_Key, 73u, 2147502348u, 0u},  {&GUID_Key, 74u, 2147502604u, 0u},
    {&GUID_Key, 75u, 2147502860u, 0u},  {&GUID_Key, 76u, 2147503116u, 0u},  {&GUID_Key, 77u, 2147503372u, 0u},
    {&GUID_Key, 78u, 2147503628u, 0u},  {&GUID_Key, 79u, 2147503884u, 0u},  {&GUID_Key, 80u, 2147504140u, 0u},
    {&GUID_Key, 81u, 2147504396u, 0u},  {&GUID_Key, 82u, 2147504652u, 0u},  {&GUID_Key, 83u, 2147504908u, 0u},
    {&GUID_Key, 84u, 2147505164u, 0u},  {&GUID_Key, 85u, 2147505420u, 0u},  {&GUID_Key, 86u, 2147505676u, 0u},
    {&GUID_Key, 87u, 2147505932u, 0u},  {&GUID_Key, 88u, 2147506188u, 0u},  {&GUID_Key, 89u, 2147506444u, 0u},
    {&GUID_Key, 90u, 2147506700u, 0u},  {&GUID_Key, 91u, 2147506956u, 0u},  {&GUID_Key, 92u, 2147507212u, 0u},
    {&GUID_Key, 93u, 2147507468u, 0u},  {&GUID_Key, 94u, 2147507724u, 0u},  {&GUID_Key, 95u, 2147507980u, 0u},
    {&GUID_Key, 96u, 2147508236u, 0u},  {&GUID_Key, 97u, 2147508492u, 0u},  {&GUID_Key, 98u, 2147508748u, 0u},
    {&GUID_Key, 99u, 2147509004u, 0u},  {&GUID_Key, 100u, 2147509260u, 0u}, {&GUID_Key, 101u, 2147509516u, 0u},
    {&GUID_Key, 102u, 2147509772u, 0u}, {&GUID_Key, 103u, 2147510028u, 0u}, {&GUID_Key, 104u, 2147510284u, 0u},
    {&GUID_Key, 105u, 2147510540u, 0u}, {&GUID_Key, 106u, 2147510796u, 0u}, {&GUID_Key, 107u, 2147511052u, 0u},
    {&GUID_Key, 108u, 2147511308u, 0u}, {&GUID_Key, 109u, 2147511564u, 0u}, {&GUID_Key, 110u, 2147511820u, 0u},
    {&GUID_Key, 111u, 2147512076u, 0u}, {&GUID_Key, 112u, 2147512332u, 0u}, {&GUID_Key, 113u, 2147512588u, 0u},
    {&GUID_Key, 114u, 2147512844u, 0u}, {&GUID_Key, 115u, 2147513100u, 0u}, {&GUID_Key, 116u, 2147513356u, 0u},
    {&GUID_Key, 117u, 2147513612u, 0u}, {&GUID_Key, 118u, 2147513868u, 0u}, {&GUID_Key, 119u, 2147514124u, 0u},
    {&GUID_Key, 120u, 2147514380u, 0u}, {&GUID_Key, 121u, 2147514636u, 0u}, {&GUID_Key, 122u, 2147514892u, 0u},
    {&GUID_Key, 123u, 2147515148u, 0u}, {&GUID_Key, 124u, 2147515404u, 0u}, {&GUID_Key, 125u, 2147515660u, 0u},
    {&GUID_Key, 126u, 2147515916u, 0u}, {&GUID_Key, 127u, 2147516172u, 0u}, {&GUID_Key, 128u, 2147516428u, 0u},
    {&GUID_Key, 129u, 2147516684u, 0u}, {&GUID_Key, 130u, 2147516940u, 0u}, {&GUID_Key, 131u, 2147517196u, 0u},
    {&GUID_Key, 132u, 2147517452u, 0u}, {&GUID_Key, 133u, 2147517708u, 0u}, {&GUID_Key, 134u, 2147517964u, 0u},
    {&GUID_Key, 135u, 2147518220u, 0u}, {&GUID_Key, 136u, 2147518476u, 0u}, {&GUID_Key, 137u, 2147518732u, 0u},
    {&GUID_Key, 138u, 2147518988u, 0u}, {&GUID_Key, 139u, 2147519244u, 0u}, {&GUID_Key, 140u, 2147519500u, 0u},
    {&GUID_Key, 141u, 2147519756u, 0u}, {&GUID_Key, 142u, 2147520012u, 0u}, {&GUID_Key, 143u, 2147520268u, 0u},
    {&GUID_Key, 144u, 2147520524u, 0u}, {&GUID_Key, 145u, 2147520780u, 0u}, {&GUID_Key, 146u, 2147521036u, 0u},
    {&GUID_Key, 147u, 2147521292u, 0u}, {&GUID_Key, 148u, 2147521548u, 0u}, {&GUID_Key, 149u, 2147521804u, 0u},
    {&GUID_Key, 150u, 2147522060u, 0u}, {&GUID_Key, 151u, 2147522316u, 0u}, {&GUID_Key, 152u, 2147522572u, 0u},
    {&GUID_Key, 153u, 2147522828u, 0u}, {&GUID_Key, 154u, 2147523084u, 0u}, {&GUID_Key, 155u, 2147523340u, 0u},
    {&GUID_Key, 156u, 2147523596u, 0u}, {&GUID_Key, 157u, 2147523852u, 0u}, {&GUID_Key, 158u, 2147524108u, 0u},
    {&GUID_Key, 159u, 2147524364u, 0u}, {&GUID_Key, 160u, 2147524620u, 0u}, {&GUID_Key, 161u, 2147524876u, 0u},
    {&GUID_Key, 162u, 2147525132u, 0u}, {&GUID_Key, 163u, 2147525388u, 0u}, {&GUID_Key, 164u, 2147525644u, 0u},
    {&GUID_Key, 165u, 2147525900u, 0u}, {&GUID_Key, 166u, 2147526156u, 0u}, {&GUID_Key, 167u, 2147526412u, 0u},
    {&GUID_Key, 168u, 2147526668u, 0u}, {&GUID_Key, 169u, 2147526924u, 0u}, {&GUID_Key, 170u, 2147527180u, 0u},
    {&GUID_Key, 171u, 2147527436u, 0u}, {&GUID_Key, 172u, 2147527692u, 0u}, {&GUID_Key, 173u, 2147527948u, 0u},
    {&GUID_Key, 174u, 2147528204u, 0u}, {&GUID_Key, 175u, 2147528460u, 0u}, {&GUID_Key, 176u, 2147528716u, 0u},
    {&GUID_Key, 177u, 2147528972u, 0u}, {&GUID_Key, 178u, 2147529228u, 0u}, {&GUID_Key, 179u, 2147529484u, 0u},
    {&GUID_Key, 180u, 2147529740u, 0u}, {&GUID_Key, 181u, 2147529996u, 0u}, {&GUID_Key, 182u, 2147530252u, 0u},
    {&GUID_Key, 183u, 2147530508u, 0u}, {&GUID_Key, 184u, 2147530764u, 0u}, {&GUID_Key, 185u, 2147531020u, 0u},
    {&GUID_Key, 186u, 2147531276u, 0u}, {&GUID_Key, 187u, 2147531532u, 0u}, {&GUID_Key, 188u, 2147531788u, 0u},
    {&GUID_Key, 189u, 2147532044u, 0u}, {&GUID_Key, 190u, 2147532300u, 0u}, {&GUID_Key, 191u, 2147532556u, 0u},
    {&GUID_Key, 192u, 2147532812u, 0u}, {&GUID_Key, 193u, 2147533068u, 0u}, {&GUID_Key, 194u, 2147533324u, 0u},
    {&GUID_Key, 195u, 2147533580u, 0u}, {&GUID_Key, 196u, 2147533836u, 0u}, {&GUID_Key, 197u, 2147534092u, 0u},
    {&GUID_Key, 198u, 2147534348u, 0u}, {&GUID_Key, 199u, 2147534604u, 0u}, {&GUID_Key, 200u, 2147534860u, 0u},
    {&GUID_Key, 201u, 2147535116u, 0u}, {&GUID_Key, 202u, 2147535372u, 0u}, {&GUID_Key, 203u, 2147535628u, 0u},
    {&GUID_Key, 204u, 2147535884u, 0u}, {&GUID_Key, 205u, 2147536140u, 0u}, {&GUID_Key, 206u, 2147536396u, 0u},
    {&GUID_Key, 207u, 2147536652u, 0u}, {&GUID_Key, 208u, 2147536908u, 0u}, {&GUID_Key, 209u, 2147537164u, 0u},
    {&GUID_Key, 210u, 2147537420u, 0u}, {&GUID_Key, 211u, 2147537676u, 0u}, {&GUID_Key, 212u, 2147537932u, 0u},
    {&GUID_Key, 213u, 2147538188u, 0u}, {&GUID_Key, 214u, 2147538444u, 0u}, {&GUID_Key, 215u, 2147538700u, 0u},
    {&GUID_Key, 216u, 2147538956u, 0u}, {&GUID_Key, 217u, 2147539212u, 0u}, {&GUID_Key, 218u, 2147539468u, 0u},
    {&GUID_Key, 219u, 2147539724u, 0u}, {&GUID_Key, 220u, 2147539980u, 0u}, {&GUID_Key, 221u, 2147540236u, 0u},
    {&GUID_Key, 222u, 2147540492u, 0u}, {&GUID_Key, 223u, 2147540748u, 0u}, {&GUID_Key, 224u, 2147541004u, 0u},
    {&GUID_Key, 225u, 2147541260u, 0u}, {&GUID_Key, 226u, 2147541516u, 0u}, {&GUID_Key, 227u, 2147541772u, 0u},
    {&GUID_Key, 228u, 2147542028u, 0u}, {&GUID_Key, 229u, 2147542284u, 0u}, {&GUID_Key, 230u, 2147542540u, 0u},
    {&GUID_Key, 231u, 2147542796u, 0u}, {&GUID_Key, 232u, 2147543052u, 0u}, {&GUID_Key, 233u, 2147543308u, 0u},
    {&GUID_Key, 234u, 2147543564u, 0u}, {&GUID_Key, 235u, 2147543820u, 0u}, {&GUID_Key, 236u, 2147544076u, 0u},
    {&GUID_Key, 237u, 2147544332u, 0u}, {&GUID_Key, 238u, 2147544588u, 0u}, {&GUID_Key, 239u, 2147544844u, 0u},
    {&GUID_Key, 240u, 2147545100u, 0u}, {&GUID_Key, 241u, 2147545356u, 0u}, {&GUID_Key, 242u, 2147545612u, 0u},
    {&GUID_Key, 243u, 2147545868u, 0u}, {&GUID_Key, 244u, 2147546124u, 0u}, {&GUID_Key, 245u, 2147546380u, 0u},
    {&GUID_Key, 246u, 2147546636u, 0u}, {&GUID_Key, 247u, 2147546892u, 0u}, {&GUID_Key, 248u, 2147547148u, 0u},
    {&GUID_Key, 249u, 2147547404u, 0u}, {&GUID_Key, 250u, 2147547660u, 0u}, {&GUID_Key, 251u, 2147547916u, 0u},
    {&GUID_Key, 252u, 2147548172u, 0u}, {&GUID_Key, 253u, 2147548428u, 0u}, {&GUID_Key, 254u, 2147548684u, 0u},
    {&GUID_Key, 255u, 2147548940u, 0u}};

DIOBJECTDATAFORMAT gInputDeviceObjectDataFormats_5EA110[44] = {{&GUID_XAxis, 0u, 2164260611u, 256u},   {&GUID_YAxis, 4u, 2164260611u, 256u},
                                      {&GUID_ZAxis, 8u, 2164260611u, 256u},   {&GUID_RxAxis, 12u, 2164260611u, 256u},
                                      {&GUID_RyAxis, 16u, 2164260611u, 256u}, {&GUID_RzAxis, 20u, 2164260611u, 256u},
                                      {&GUID_Slider, 24u, 2164260611u, 256u}, {&GUID_Slider, 28u, 2164260611u, 256u},
                                      {&GUID_POV, 32u, 2164260624u, 0u},      {&GUID_POV, 36u, 2164260624u, 0u},
                                      {&GUID_POV, 40u, 2164260624u, 0u},      {&GUID_POV, 44u, 2164260624u, 0u},
                                      {NULL, 48u, 2164260620u, 0u},           {NULL, 49u, 2164260620u, 0u},
                                      {NULL, 50u, 2164260620u, 0u},           {NULL, 51u, 2164260620u, 0u},
                                      {NULL, 52u, 2164260620u, 0u},           {NULL, 53u, 2164260620u, 0u},
                                      {NULL, 54u, 2164260620u, 0u},           {NULL, 55u, 2164260620u, 0u},
                                      {NULL, 56u, 2164260620u, 0u},           {NULL, 57u, 2164260620u, 0u},
                                      {NULL, 58u, 2164260620u, 0u},           {NULL, 59u, 2164260620u, 0u},
                                      {NULL, 60u, 2164260620u, 0u},           {NULL, 61u, 2164260620u, 0u},
                                      {NULL, 62u, 2164260620u, 0u},           {NULL, 63u, 2164260620u, 0u},
                                      {NULL, 64u, 2164260620u, 0u},           {NULL, 65u, 2164260620u, 0u},
                                      {NULL, 66u, 2164260620u, 0u},           {NULL, 67u, 2164260620u, 0u},
                                      {NULL, 68u, 2164260620u, 0u},           {NULL, 69u, 2164260620u, 0u},
                                      {NULL, 70u, 2164260620u, 0u},           {NULL, 71u, 2164260620u, 0u},
                                      {NULL, 72u, 2164260620u, 0u},           {NULL, 73u, 2164260620u, 0u},
                                      {NULL, 74u, 2164260620u, 0u},           {NULL, 75u, 2164260620u, 0u},
                                      {NULL, 76u, 2164260620u, 0u},           {NULL, 77u, 2164260620u, 0u},
                                      {NULL, 78u, 2164260620u, 0u},           {NULL, 79u, 2164260620u, 0u}};

DIOBJECTDATAFORMAT gMouseObjectDataFormats_5EA3D0[7] = {{&GUID_XAxis, 0u, 16776963u, 0u},
                                     {&GUID_YAxis, 4u, 16776963u, 0u},
                                     {&GUID_ZAxis, 8u, 2164260611u, 0u},
                                     {NULL, 12u, 16776972u, 0u},
                                     {NULL, 13u, 16776972u, 0u},
                                     {NULL, 14u, 2164260620u, 0u},
                                     {NULL, 15u, 2164260620u, 0u}};

DIDATAFORMAT gKeyboardDataFormat_601A54 = {24u, 16u, DIDF_RELAXIS, 256u, 256u, gKeyboardObjectDataFormats_5E9110};
DIDATAFORMAT gInputDeviceFormat_601A6C = {24u, 16u, DIDF_ABSAXIS, 80u, 44u, gInputDeviceObjectDataFormats_5EA110};
DIDATAFORMAT stru_601A84 = {24u, 16u, DIDF_RELAXIS, 16u, 7u, gMouseObjectDataFormats_5EA3D0};

struct TgaInfo
{
    char_type field_0_tga_name[128];
    s32 field_80_len;
    s32 field_84_img;
};

/*
TgaInfo tgaArray_61F0C8[25] = {{"data\\frontend\\1.tga", 347564, 0}, {"data\\frontend\\1_Options.tga", 266924, 0},
        {"data\\frontend\\1_Play.tga", 266924, 0}, {"data\\frontend\\1_Quit.tga", 266924, 0}, {"data\\frontend\\2.tga", 347564, 0},
        {"data\\frontend\\2_Bonus1.tga", 266924, 0}, {"data\\frontend\\2_Bonus2.tga", 266924, 0},
        {"data\\frontend\\2_Bonus3.tga", 266924, 0}, {"data\\frontend\\2_League.tga", 266924, 0},
        {"data\\frontend\\2_Level1.tga", 266924, 0}, {"data\\frontend\\2_Level2.tga", 266924, 0},
        {"data\\frontend\\2_Level3.tga", 266924, 0}, {"data\\frontend\\2_Name.tga", 266924, 0},
        {"data\\frontend\\2_Restart.tga", 266924, 0}, {"data\\frontend\\3.tga", 347564, 0}, {"data\\frontend\\3_Tables.tga", 614444, 0},
        {"data\\frontend\\GameComplete.tga", 614444, 0}, {"data\\frontend\\LevelComplete.tga", 614444, 0},
        {"data\\frontend\\MPLose.tga", 614444, 0}, {"data\\frontend\\PlayerDead.tga", 614444, 0}, {"data\\frontend\\Mask.tga", 104300, 0},
        {"data\\frontend\\Mask2.tga ", 53594, 0}, {"data\\frontend\\Credits.tga", 614444, 0}, {"data\\frontend\\Mask3.tga", 130427, 0},
        {"data\\frontend\\DemoInfo.tga ", 614939, 0}};
*/

DEFINE_GLOBAL_ARRAY_INIT(
    TgaInfo,
    tgaArray_61F0C8,
    25,
    0x61F0C8,
    {"data\\frontend\\1.tga" COMMA 347564 COMMA 0} COMMA {"data\\frontend\\1_Options.tga" COMMA 266924 COMMA 0} COMMA {
        "data\\frontend\\1_Play.tga" COMMA 266924 COMMA 0} COMMA {"data\\frontend\\1_Quit.tga" COMMA 266924 COMMA 0} COMMA {
        "data\\frontend\\2.tga" COMMA 347564 COMMA 0} COMMA {"data\\frontend\\2_Bonus1.tga" COMMA 266924 COMMA 0} COMMA {
        "data\\frontend\\2_Bonus2.tga" COMMA 266924 COMMA 0} COMMA {"data\\frontend\\2_Bonus3.tga" COMMA 266924 COMMA 0} COMMA {
        "data\\frontend\\2_League.tga" COMMA 266924 COMMA 0} COMMA {"data\\frontend\\2_Level1.tga" COMMA 266924 COMMA 0} COMMA {
        "data\\frontend\\2_Level2.tga" COMMA 266924 COMMA 0} COMMA {"data\\frontend\\2_Level3.tga" COMMA 266924 COMMA 0} COMMA {
        "data\\frontend\\2_Name.tga" COMMA 266924 COMMA 0} COMMA {"data\\frontend\\2_Restart.tga" COMMA 266924 COMMA 0} COMMA {
        "data\\frontend\\3.tga" COMMA 347564 COMMA 0} COMMA {"data\\frontend\\3_Tables.tga" COMMA 614444 COMMA 0} COMMA {
        "data\\frontend\\GameComplete.tga" COMMA 614444 COMMA 0} COMMA {"data\\frontend\\LevelComplete.tga" COMMA 614444 COMMA 0} COMMA {
        "data\\frontend\\MPLose.tga" COMMA 614444 COMMA 0} COMMA {"data\\frontend\\PlayerDead.tga" COMMA 614444 COMMA 0} COMMA {
        "data\\frontend\\Mask.tga" COMMA 104300 COMMA 0} COMMA {"data\\frontend\\Mask2.tga" COMMA 53594 COMMA 0} COMMA {
        "data\\frontend\\Credits.tga" COMMA 614444 COMMA 0} COMMA {"data\\frontend\\Mask3.tga" COMMA 130427 COMMA 0} COMMA {
        "data\\frontend\\DemoInfo.tga" COMMA 614939 COMMA 0});

// This function matches but Write_4D9620 from ErrorLog class is crashing standalone on exe boot
WIP_FUNC(0x5D9910)
EXPORT s32 __stdcall SetGamma_5D9910(s32 gamma)
{
    f32 gamma_f = gamma * 0.1;
    if (gVidSys_7071D0)
    {
        s32 result = pVid_SetGamma(gVidSys_7071D0, gamma_f, gamma_f, gamma_f);
        // TODO: format string at 0x626B34 not checked against the original
        sprintf(gTmpBuffer_67C598, "SetGamma %d = %d", gamma, result);
        //gErrorLog_67C530.Write_4D9620(gTmpBuffer_67C598);  // crashing standalone
        return result;
    }
    return gamma;
}

DEFINE_GLOBAL(infallible_turing, snd1_67D818, 0x67D818);
DEFINE_GLOBAL(infallible_turing, snd2_67D6F8, 0x67D6F8);

MATCH_FUNC(0x4B4C60)
void Frontend::LoadStringsFromStage_4B4C60(u16 mainBlockIdx, u16 bounusBlockIdx, char* pDebugStr, char* pMapName, char* pStyName)
{
    strcpy(pDebugStr, field_C9E8_blocks[mainBlockIdx][bounusBlockIdx].field_0_debug_str);
    strcpy(pMapName, field_C9E8_blocks[mainBlockIdx][bounusBlockIdx].field_100_map_name);
    strcpy(pStyName, field_C9E8_blocks[mainBlockIdx][bounusBlockIdx].field_200_sty_name);
}

MATCH_FUNC(0x4B4BC0)
void Frontend::StoreStringsForStage_4B4BC0(u16 mainBlockIdx, u16 bounusBlockIdx, const char* pDebugStr, const char* pMapName, const char* pStyName)
{
    strcpy(field_C9E8_blocks[mainBlockIdx][bounusBlockIdx].field_0_debug_str, pDebugStr);
    strcpy(field_C9E8_blocks[mainBlockIdx][bounusBlockIdx].field_100_map_name, pMapName);
    strcpy(field_C9E8_blocks[mainBlockIdx][bounusBlockIdx].field_200_sty_name, pStyName);
}

MATCH_FUNC(0x4ACFA0)
void __stdcall Frontend::create_4ACFA0()
{
    if (!gFrontend_67DC84)
    {
        gFrontend_67DC84 = new Frontend();
    }

    if (!bSkip_audio_67D6BE)
    {
        snd1_67D818.field_0_object_type = 0;
        snd1_67D818.field_4_bStatus = 0;
        snd2_67D6F8.field_0_object_type = SoundObjectTypeEnum::infallible_turing_2;
        snd2_67D6F8.field_C_pAny.pInfallible_turing = &snd1_67D818;
        snd2_67D6F8.field_4_bStatus = 0;
        snd2_67D6F8.field_8_sound_entry = gRoot_sound_66B038.AddSoundObject_40EFB0(&snd2_67D6F8);
        gRoot_sound_66B038.LoadStyle_40EFF0("data\\fstyle.sty");
        gRoot_sound_66B038.Set3DSound_40F160(0);
    }

    Bink::Reset_513210();
}

MATCH_FUNC(0x4AD070)
void __stdcall Frontend::destroy_4AD070()
{
    if (!bSkip_audio_67D6BE && snd2_67D6F8.field_8_sound_entry)
    {
        gRoot_sound_66B038.FreeSoundEntry_40EFD0(snd2_67D6F8.field_8_sound_entry);
        snd2_67D6F8.field_8_sound_entry = 0;
    }

    if (gFrontend_67DC84)
    {
        GTA2_DELETE_AND_NULL(gFrontend_67DC84);
    }

    Bink::CloseSlot1_513340();
    Bink::CloseSlot2_513390();
}

// 9.6f 0x453AB0: index of the tag game player with the longest time (that hasn't quit)
inline s32 youthful_einstein::GetLeaderIdx_453AB0()
{
    s32 leader_idx = -1;
    s32 leader_time = -1;
    if (IsTagGame_434B20())
    {
        for (s32 i = 0; i < 6; i++)
        {
            if (field_4_time[i] > leader_time && !field_20[i])
            {
                leader_time = field_4_time[i];
                leader_idx = i;
            }
        }
        return leader_idx;
    }
    return gLucid_hamilton_67E8E0.GetWinnerIdx_4C5C20();
}

WIP_FUNC(0x4B3170)
void Frontend::ChangeMenuPage_4B3170(u16 menu_page_idx)
{
    WIP_IMPLEMENTED;
    u8 bonus_count;
    u8 stage_idx;
    u8 i;
    u8 saved_main_stage;
    u8 saved_is_bonus;
    u8 main_stage_idx;
    u8 bonus_stage_idx;
    u8 game_mode;
    u8 opponent_idx;
    u8 player;
    s32 user_idx;
    wchar_t player_name[50];
    wchar_t quit_name[64];

    player_stats_0xA4* pStats = GetCurrPlayerStats_4B43E0();
    field_132_f136_idx = menu_page_idx;

    if (menu_page_idx == MENUPAGE_PARENTAL_CONTROL)
    {
        field_110_state = 5;
        field_C9CA_password_length = 0;
        field_C9CB_wrong_password_shown = 0;
        StripPasswordToCurrLength_4B8530();
        field_C9B3_key_held = 1;
        field_C9B4_last_key = DIK_RETURN;
        field_C9B6_key_repeat_timer = 5;
    }
    else if (menu_page_idx == MENUPAGE_CREDITS)
    {
        field_1EB34_credits_ypos = 0x668000;
        field_1EB30_credits_scroll_timer = 0;
        field_1EB38_credits_line_idx = 0;
        field_C9B3_key_held = 1;
    }
    else if (menu_page_idx == MENUPAGE_AREA_COMPLETE)
    {
        stage_idx = gLucid_hamilton_67E8E0.GetMainStageIdx_4C5980();
        bonus_count = gLucid_hamilton_67E8E0.GetLevelFinishBonusType_4C59C0();
        if (gLucid_hamilton_67E8E0.get_secret_tokens_collected_453A80() == 50)
        {
            bonus_count = 3;
        }

        for (i = 1; i <= bonus_count; i++)
        {
            if (i < field_1EB51_num_bonus_stages[stage_idx])
            {
                gJolly_poitras_0x2BC0_6FEAC0->UnlockStage_56BBD0(stage_idx, i);
            }
        }

        if (stage_idx == (u8)field_1EB50_num_main_stages - 1)
        {
            field_136_menu_pages_array[3].field_4_options_array[0].field_1_is_unlocked = 0;
            field_136_menu_pages_array[3].field_B8A[0].field_4_is_option_unlocked = 0;
        }
        else
        {
            gJolly_poitras_0x2BC0_6FEAC0->UnlockStage_56BBD0(stage_idx + 1, 0);
            field_136_menu_pages_array[3].field_4_options_array[0].field_1_is_unlocked = 1;
            field_136_menu_pages_array[3].field_B8A[0].field_4_is_option_unlocked = 1;
        }

        field_136_menu_pages_array[3].field_4_options_array[3].field_1_is_unlocked = 0;
        field_136_menu_pages_array[3].field_B8A[3].field_4_is_option_unlocked = 0;
        for (i = 1; i < 4; i++)
        {
            if (pStats->field_0_plyr_stage_stats[stage_idx][i].field_0_is_stage_unlocked && i < field_1EB51_num_bonus_stages[stage_idx])
            {
                field_136_menu_pages_array[3].field_4_options_array[3].field_1_is_unlocked = 1;
                field_136_menu_pages_array[3].field_B8A[3].field_4_is_option_unlocked = 1;
            }
        }
    }
    else if (menu_page_idx == MENUPAGE_BONUS_AREA)
    {
        gLucid_hamilton_67E8E0.DecodeStage_453A60(gLucid_hamilton_67E8E0.GetStage_4C5990(), &stage_idx, &bonus_count);
        swprintf(tmpBuff_67BD9C, L"%d", pStats->field_0_plyr_stage_stats[stage_idx][bonus_count].field_8_stage_latest_score);
        wcsncpy(field_136_menu_pages_array[6].field_518_elements_array[2].field_6_element_name_str, tmpBuff_67BD9C, 50);
        if (gLucid_hamilton_67E8E0.IsStartedFromPlayBonusMenu_4C5AE0() || stage_idx >= (u8)field_1EB50_num_main_stages - 1 ||
            !pStats->field_0_plyr_stage_stats[stage_idx + 1][0].field_0_is_stage_unlocked)
        {
            field_136_menu_pages_array[6].field_4_options_array[1].field_1_is_unlocked = 0;
            field_136_menu_pages_array[6].field_B8A[1].field_4_is_option_unlocked = 0;
        }
        else
        {
            field_136_menu_pages_array[6].field_4_options_array[1].field_1_is_unlocked = 1;
            field_136_menu_pages_array[6].field_B8A[1].field_4_is_option_unlocked = 1;
        }
    }

    if (menu_page_idx == MENUPAGE_AREA_COMPLETE || menu_page_idx == MENUPAGE_DEAD || menu_page_idx == MENUPAGE_RESULTS_PLAYER_QUIT)
    {
        // the option to check is "continue" (1) on the area complete page, and 0 on the dead/quit pages
        stage_idx = (menu_page_idx == MENUPAGE_DEAD || menu_page_idx == MENUPAGE_RESULTS_PLAYER_QUIT) ? 0 : 1;
        game_mode = gLucid_hamilton_67E8E0.GetPlySlotIdx_4C59B0();
        saved_main_stage = field_EDE8_plySlots[game_mode].field_1_last_saved_stage;
        bonus_count = field_EDE8_plySlots[game_mode].field_2_last_saved_bonus_stage_code;
        saved_is_bonus = field_EDE8_plySlots[game_mode].field_3_last_saved_is_bonus;
        if (!gLucid_hamilton_67E8E0.IsBonusStage_4C59A0())
        {
            main_stage_idx = gLucid_hamilton_67E8E0.GetMainStageIdx_4C5980();
            bonus_stage_idx = 0;
        }
        else
        {
            // 9.6f: lucid_hamilton::DecodeStage_453A60 here and below (inlined, using it changes the code)
            u8 stage = gLucid_hamilton_67E8E0.GetStage_4C5990();
            main_stage_idx = stage >> 4;
            bonus_stage_idx = stage & 0xF;
        }

        u8 saved_main;
        if (!saved_is_bonus)
        {
            saved_main = saved_main_stage;
            bonus_count = 0;
        }
        else
        {
            saved_main = bonus_count >> 4;
            bonus_count = bonus_count & 0xF;
        }

        MenuPage_0xBCA* pPage = &field_136_menu_pages_array[menu_page_idx];
        if (main_stage_idx == saved_main && bonus_stage_idx == bonus_count)
        {
            pPage->field_4_options_array[stage_idx].field_1_is_unlocked = 1;
            pPage->field_B8A[stage_idx].field_4_is_option_unlocked = 1;
        }
        else
        {
            pPage->field_4_options_array[stage_idx].field_1_is_unlocked = 0;
            pPage->field_B8A[stage_idx].field_4_is_option_unlocked = 0;
        }
    }

    if (menu_page_idx == MENUPAGE_PLAY)
    {
        s16 playerSlotSetting = gRegistry_6FF968.Create_Player_Setting_587810("plyrslot");
        field_136_menu_pages_array[1].field_4_options_array[0].field_6E_horizontal_selected_idx = playerSlotSetting;
        field_136_menu_pages_array[1].field_4_options_array[0].field_70 = playerSlotSetting;
        gLucid_hamilton_67E8E0.SetPlySlotIdx_4C5920(playerSlotSetting);
        UpdateMenuForCurrPlayer_4B42E0();
    }
    else if (menu_page_idx == MENUPAGE_MULTIPLAYER_RESULTS)
    {
        stage_idx = gLucid_hamilton_67E8E0.GetMaxPlayers_4C5BF0();
        gYouthful_einstein_6F8450.GetLeaderIdx_453AB0();
        user_idx = (u8)gLucid_hamilton_67E8E0.GetUserPlayerIdx_4C5BE0();
        game_mode = gLucid_hamilton_67E8E0.GetMultiplayerGamemode_4C5BC0();

        switch (game_mode)
        {
            case FRAG_GAME_1:
                wcsncpy(field_136_menu_pages_array[7].field_518_elements_array[13].field_6_element_name_str,
                        gText_0x14_704DFC->Find_5B5F90("frags_h"),
                        50);
                field_136_menu_pages_array[7].field_518_elements_array[13].field_2_xpos =
                    GetCenteredXPos_4B0190(field_136_menu_pages_array[7].field_518_elements_array[13].field_6_element_name_str,
                                              field_136_menu_pages_array[7].field_518_elements_array[13].field_6A_font_type,
                                              320);
                break;

            case POINTS_GAME_2:
                wcsncpy(field_136_menu_pages_array[7].field_518_elements_array[13].field_6_element_name_str,
                        gText_0x14_704DFC->Find_5B5F90("pnts_h"),
                        50);
                field_136_menu_pages_array[7].field_518_elements_array[13].field_2_xpos =
                    GetCenteredXPos_4B0190(field_136_menu_pages_array[7].field_518_elements_array[13].field_6_element_name_str,
                                              field_136_menu_pages_array[7].field_518_elements_array[13].field_6A_font_type,
                                              320);
                break;

            case TAG_GAME_3:
                wcsncpy(field_136_menu_pages_array[7].field_518_elements_array[13].field_6_element_name_str,
                        gText_0x14_704DFC->Find_5B5F90("times_h"),
                        50);
                field_136_menu_pages_array[7].field_518_elements_array[13].field_2_xpos =
                    GetCenteredXPos_4B0190(field_136_menu_pages_array[7].field_518_elements_array[13].field_6_element_name_str,
                                              field_136_menu_pages_array[7].field_518_elements_array[13].field_6A_font_type,
                                              320);
                break;

            default:
                FatalError_4A38C0(Gta2Error::InvalidMultiplayerGameType,
                                  "C:\\Splitting\\GTA2\\Source\\frontend2.cpp",
                                  4079); // Multiplayer game type should be frag, tag or score (but isn't)
                break;
        }

        for (player = 0; player < 6; player++)
        {
            if (player < stage_idx)
            {
                field_136_menu_pages_array[7].field_518_elements_array[player + 1].field_1_is_it_displayed = 1;
                field_136_menu_pages_array[7].field_518_elements_array[player + 7].field_1_is_it_displayed = !IsTagGame_434B20();
            }
            else
            {
                field_136_menu_pages_array[7].field_518_elements_array[player + 1].field_1_is_it_displayed = 0;
                field_136_menu_pages_array[7].field_518_elements_array[player + 7].field_1_is_it_displayed = 0;
            }
        }

        opponent_idx = 0;
        for (player = 0; player < stage_idx; player++)
        {
            if (gYouthful_einstein_6F8450.HasQuit_453A90(player))
            {
                swprintf(quit_name,
                         L"%s (%s)",
                         gLucid_hamilton_67E8E0.GetPlayerName_4C5C60(player)->field_0_str,
                         gText_0x14_704DFC->Find_5B5F90("mult_q"));
                wcscpy(player_name, quit_name);
                gText_0x14_704DFC->StrToUpper_5B5B80(player_name);
            }
            else
            {
                wcsncpy(player_name, gLucid_hamilton_67E8E0.GetPlayerName_4C5C60(player)->field_0_str, 50);
                gText_0x14_704DFC->StrToUpper_5B5B80(player_name);
            }
            wcsncpy(field_136_menu_pages_array[7].field_518_elements_array[player + 1].field_6_element_name_str, player_name, 50);
            if (player != user_idx)
            {
                wcsncpy(field_136_menu_pages_array[7].field_518_elements_array[opponent_idx + 8].field_6_element_name_str, player_name, 50);
                opponent_idx++;
            }
        }

        s32 best_opponent = -1;
        s32 user_value;
        if (gYouthful_einstein_6F8450.HasQuit_453A90(user_idx))
        {
            goto lose;
        }

        if (game_mode == FRAG_GAME_1)
        {
            for (i = 0; i < stage_idx; i++)
            {
                if (i != user_idx && !gYouthful_einstein_6F8450.HasQuit_453A90(i) &&
                    (s16)gLucid_hamilton_67E8E0.GetFragsForPlayerIdx_4C5D60(i) > best_opponent)
                {
                    best_opponent = (s16)gLucid_hamilton_67E8E0.GetFragsForPlayerIdx_4C5D60(i);
                }
            }
            user_value = (s16)gLucid_hamilton_67E8E0.GetFragsForPlayerIdx_4C5D60(user_idx);
        }
        else if (game_mode == POINTS_GAME_2)
        {
            for (i = 0; i < stage_idx; i++)
            {
                if (i != user_idx && !gYouthful_einstein_6F8450.HasQuit_453A90(i) &&
                    gLucid_hamilton_67E8E0.GetPointsForPlayerIdx_4C5CB0(i) > best_opponent)
                {
                    best_opponent = gLucid_hamilton_67E8E0.GetPointsForPlayerIdx_4C5CB0(i);
                }
            }
            user_value = gLucid_hamilton_67E8E0.GetPointsForPlayerIdx_4C5CB0(user_idx);
        }
        else if (game_mode == TAG_GAME_3)
        {
            for (i = 0; i < stage_idx; i++)
            {
                if (i != user_idx && !gYouthful_einstein_6F8450.HasQuit_453A90(i))
                {
                    s32 time = gYouthful_einstein_6F8450.GetTime_453AA0(i);
                    best_opponent = time > best_opponent ? time : best_opponent;
                }
            }
            user_value = gYouthful_einstein_6F8450.GetTime_453AA0(user_idx);
        }
        else
        {
            goto draw;
        }

        user_value -= best_opponent;
        if (user_value > 0)
        {
            wcsncpy(field_136_menu_pages_array[7].field_518_elements_array[0].field_6_element_name_str,
                    gText_0x14_704DFC->Find_5B5F90("mult_w"),
                    50);
        }
        else if (user_value < 0)
        {
        lose:
            wcsncpy(field_136_menu_pages_array[7].field_518_elements_array[0].field_6_element_name_str,
                    gText_0x14_704DFC->Find_5B5F90("mult_l"),
                    50);
        }
        else
        {
        draw:
            wcsncpy(field_136_menu_pages_array[7].field_518_elements_array[0].field_6_element_name_str,
                    gText_0x14_704DFC->Find_5B5F90("mult_d"),
                    50);
        }
    }
    else if (menu_page_idx == MENUPAGE_PLAY_INTRO)
    {
        if (bIsFrench_67D53C)
        {
            FreeSound_4B8650();
        }

        if (pre_intro_bik_exists_4B6030())
        {
            Bink::OpenSlot1_513560(gFrontend_67DC84->pre_intro_bik_4B5F20(), gSampManager_6FFF00.field_0_hDriver);
        }
        else
        {
            Bink::OpenSlot2_5133E0(gFrontend_67DC84->intro_bik_4B5E50(), gSampManager_6FFF00.field_0_hDriver);
        }
    }
    else if (menu_page_idx == MENUPAGE_START_MENU)
    {
        field_C9E4_last_input_time = timeGetTime();
    }

    field_132_f136_idx = menu_page_idx;
    field_136_menu_pages_array[menu_page_idx].field_BC6_current_option_idx = field_136_menu_pages_array[menu_page_idx].field_BC8_default_option_idx;
    MenuPage_0xBCA* pCurPage = &field_136_menu_pages_array[field_132_f136_idx];
    if (!pCurPage->field_4_options_array[pCurPage->field_BC6_current_option_idx].field_1_is_unlocked)
    {
        pCurPage->SelectNextOption_4B6200();
        if (!pCurPage->field_4_options_array[pCurPage->field_BC6_current_option_idx].field_1_is_unlocked)
        {
            FatalError_4A38C0(Gta2Error::FreeloaderEpisodeUnknown,
                              "C:\\Splitting\\GTA2\\Source\\frontend2.cpp",
                              4269); // the menu contains no valid options
        }
    }

    UpdateMenuScreen_4B6780();
}

MATCH_FUNC(0x4B3AF0)
void Frontend::GetOptionText_4B3AF0(u16 menu_page_idx, u16 option_idx, wchar_t** w_buffer)
{
    MenuPage_0xBCA* pPage = &field_136_menu_pages_array[menu_page_idx];
    menu_option_0x82* pOption = &pPage->field_4_options_array[option_idx];

    if (menu_page_idx == MENUPAGE_PLAY && option_idx == 0) // option 0 = change player/name
    {
        u16 plyr_idx = pOption->field_6E_horizontal_selected_idx;
        wchar_t* p_wName = (wchar_t*)&gJolly_poitras_0x2BC0_6FEAC0->field_26A0_plyr_stats[plyr_idx].field_90_strPlayerName;
        if (field_110_state == FrontendState::User_Typing_New_Player_Name_3)
        {
            // player typing a name
            wcscpy(gTmpWideStr_67C7D8, field_C9A0_curr_plyr_name);
        }
        else if (!*p_wName)
        {
            // player 1, 2, 3 etc.
            swprintf(tmpBuff_67BD9C, L"%d", plyr_idx);
            swprintf(gTmpWideStr_67C7D8, L"%s %s", pOption->field_6_option_name_str, tmpBuff_67BD9C);
        }
        else
        {
            // get saved player name
            swprintf(gTmpWideStr_67C7D8, L"%s", gJolly_poitras_0x2BC0_6FEAC0->field_26A0_plyr_stats[plyr_idx].field_90_strPlayerName);
        }
    }
    else if (menu_page_idx == MENUPAGE_VIEW_HIGH_SCORE && option_idx == 0)
    {
        swprintf(gTmpWideStr_67C7D8, L"%s", gText_0x14_704DFC->Find_5B5F90("hi_for"));
    }
    else
    {
        swprintf(tmpBuff_67BD9C, L"%d", pOption->field_6E_horizontal_selected_idx);
        swprintf(gTmpWideStr_67C7D8, L"%s %s", pOption->field_6_option_name_str, tmpBuff_67BD9C);
    }
    *w_buffer = (wchar_t*)&gTmpWideStr_67C7D8;
}

MATCH_FUNC(0x4B8680)
void Frontend::InitSound_4B8680()
{
    if (!bSkip_audio_67D6BE)
    {
        snd1_67D818.field_0_object_type = 0;
        snd1_67D818.field_4_bStatus = 0;
        snd2_67D6F8.field_0_object_type = SoundObjectTypeEnum::infallible_turing_2;
        snd2_67D6F8.field_C_pAny.pInfallible_turing = &snd1_67D818;
        snd2_67D6F8.field_4_bStatus = 0;
        snd2_67D6F8.field_8_sound_entry = gRoot_sound_66B038.AddSoundObject_40EFB0(&snd2_67D6F8);
    }
}

MATCH_FUNC(0x4AEDB0)
s32 Frontend::Run_4AEDB0()
{
    u32 Time; // eax
    u16 local_field_132_f136_idx; // cx
    //const char_type* v5; // eax
    char_type* local_field_8_keys; // edi
    s32 v7; // ebx
    s32 result; // eax
    char_type* v9; // ecx
    s32 v10; // edx
    char_type v12; // al
    HDIGDRIVER local_field_0_hDriver; // [esp-4h] [ebp-10h]

    Time = timeGetTime();
    local_field_132_f136_idx = field_132_f136_idx;
    if (local_field_132_f136_idx == MENUPAGE_PLAY_INTRO)
    {
        if (Bink::TickFrame_513240())
        {
            if (Bink::GetActiveSlot_513790() == 1)
            {
                Bink::SetActiveSlot_5137A0(2);
                local_field_0_hDriver = gSampManager_6FFF00.field_0_hDriver;
                Bink::OpenSlot2_5133E0(gFrontend_67DC84->intro_bik_4B5E50(), local_field_0_hDriver);
                Bink::CloseSlot1_513340();
                Bink::SetActiveSlot_5137A0(2);
                Bink::SetDDState_5137B0(2);
            }
            else
            {
                Bink::CloseSlot1_513340();
                Bink::CloseSlot2_513390();
                if (bIsFrench_67D53C)
                {
                    InitSound_4B8680();
                }

                ChangeMenuPage_4B3170(MENUPAGE_START_MENU);
            }
        }

        read_menu_input_4AFEB0();

        local_field_8_keys = field_8_keys;
        v7 = 256;
        do
        {
            if ((*local_field_8_keys & 0x80u) != 0)
            {
                Bink::CloseSlot1_513340();
                Bink::CloseSlot2_513390();
                if (bIsFrench_67D53C)
                {
                    InitSound_4B8680();
                }
                ChangeMenuPage_4B3170(MENUPAGE_START_MENU);
            }
            ++local_field_8_keys;
            --v7;
        } while (v7);

        return field_108_winmain_next_state;
    }
    else
    {
        if (local_field_132_f136_idx == MENUPAGE_START_MENU)
        {
            v9 = field_8_keys;
            v10 = 256;
            do
            {
                if ((*v9 & 0x80u) != 0)
                {
                    field_C9E4_last_input_time = Time;
                }
                ++v9;
                --v10;
            } while (v10);
            if (Time - field_C9E4_last_input_time > 60000)
            {
                return 4;
            }
        }
        else
        {
            field_C9E4_last_input_time = Time;
        }

        if (Time >= field_C9DC_next_frame_time || (field_C9E0_updates_since_render == 3))
        {
            Update_4AEC00();
            v12 = field_C9E0_updates_since_render + 1;
            field_C9DC_next_frame_time += 33;
            field_C9E0_updates_since_render = v12;
        }
        else if (field_C9E0_updates_since_render)
        {
            Render_4ADFB0(); // bQuit ??
            result = field_108_winmain_next_state;
            field_C9E0_updates_since_render = 0;
            return result;
        }
        return field_108_winmain_next_state;
    }
}

// https://decomp.me/scratch/ci11a
MATCH_FUNC(0x4B5430)
void Frontend::DrawScoreTable_4B5430(score_table_line* pStrings,
                          u16 text_xpos,
                          u16 text_ypos,
                          u16 num_entries,
                          u16 arg_fontType,
                          u16 palette,
                          u8 spacing_type)
{
    u16 new_xpos;

    for (u16 i = 0; i < num_entries; i++)
    {
        score_table_line* pIter = &pStrings[i];
        u16 text_ypos_to_use = text_ypos + 40 * i;
        if (spacing_type)
        {
            text_ypos_to_use = text_ypos + 20 * i;
        }
        if (!wcscmp(pIter->field_0_player_name, (wchar_t*)&gEmptyWStr_67DC8C))
        {
            swprintf(tmpBuff_67BD9C, gText_0x14_704DFC->Find_5B5F90("hi_empt"));
        }
        else
        {
            swprintf(tmpBuff_67BD9C, L"%s", pIter->field_0_player_name);
        }
        if ((u16)palette == 0xFFFFu)
        {
            DrawText_4B87A0(tmpBuff_67BD9C, text_xpos, text_ypos_to_use, arg_fontType, 1);
        }
        else
        {
            DrawText_5D8A10(tmpBuff_67BD9C, text_xpos, text_ypos_to_use, arg_fontType, 1, 8, palette, false, 0);
        }
        if (spacing_type == 0)
        {
            new_xpos = text_xpos + 175;
            text_ypos_to_use = text_ypos + 40 * i + 20;
        }
        else
        {
            new_xpos = spacing_type == 1 ? text_xpos + 600 : text_xpos + 300;
        }
        swprintf(tmpBuff_67BD9C, L"%d", pIter->field_14_score);

        if (gText_0x14_704DFC->field_10_lang_code == 'j')
        {
            Frontend::DrawTextFixedWidth_4B78B0(tmpBuff_67BD9C, new_xpos, text_ypos_to_use, arg_fontType, palette, 1, 16, true);
        }
        else
        {
            Frontend::DrawTextFixedWidth_4B78B0(tmpBuff_67BD9C, new_xpos, text_ypos_to_use, arg_fontType, palette, 1, 13, true);
        }

    }
}

// todo: add to header
EXTERN_GLOBAL(s32, gGTA2VersionMajor_708280);

EXTERN_GLOBAL(s32, gGTA2VersionMajor_708284);

// sub_457920 in 9.6f
// https://decomp.me/scratch/jchxT
MATCH_FUNC(0x4AD140)
void Frontend::DrawMenu_4AD140()
{
    const s32 v98 = gText_0x14_704DFC->field_10_lang_code != 'j' ? 14 : 16;

    MenuPage_0xBCA* pMenuPage = &field_136_menu_pages_array[field_132_f136_idx];

    u16 selected_option_idx;
    u16 last_xpos;

    if (field_132_f136_idx == MENUPAGE_START_MENU)
    {
        swprintf(tmpBuff_67BD9C, L"GTA2 V%d.%d", gGTA2VersionMajor_708280, gGTA2VersionMajor_708284);
        DrawText_4B87A0(tmpBuff_67BD9C, 300, 460, font_type_703C14, 1);
    }

    if (field_132_f136_idx == MENUPAGE_PLAY)
    {
        if (field_110_state == FrontendState::User_Typing_New_Player_Name_3)
        {
            pMenuPage->field_518_elements_array[8].field_1_is_it_displayed = false;
            pMenuPage->field_518_elements_array[9].field_1_is_it_displayed = false;

            // NOTE: field_124_font_type is u16

            last_xpos = sub_4B7E10(2, 0x12Cu, 0x1B8u, field_124_font_type, 0xFFFF); // text: ENTER
            last_xpos = sub_4B7E10(11, last_xpos + 300, 0x1B8u, field_124_font_type, 0xFFFF); // text: : ENTER NAME

            last_xpos = sub_4B7E10(3, 0x12Cu, 0x1CCu, field_124_font_type, 0xFFFF); // text: BACKSPACE
            sub_4B7E10(10, last_xpos + 300, 0x1CCu, field_124_font_type, 0xFFFF); // text: : DELETE LETTER
        }
        else
        {
            u16 idx = pMenuPage->field_4_options_array[0].field_6E_horizontal_selected_idx;
            selected_option_idx = idx;
            u16 unk_xpos =
                Frontend::GetMaxTextWidth_5D8990(gJolly_poitras_0x2BC0_6FEAC0->field_26A0_plyr_stats[idx].field_90_strPlayerName, field_11C_normal_font) + 10;

            if (unk_xpos == 10)
            {
                unk_xpos = Frontend::GetMaxTextWidth_5D8990(pMenuPage->field_4_options_array[0].field_6_option_name_str, field_11C_normal_font) + 40;
            }
            pMenuPage->field_518_elements_array[9].field_2_xpos = unk_xpos + pMenuPage->field_4_options_array[0].field_2_x_pos;
        }
    }

    high_score_table_0xF0* pHighScoreTable;

    if (field_132_f136_idx == MENUPAGE_VIEW_HIGH_SCORE)
    {
        if (field_EE0D_hiscore_table_idx < 3) //  line 1b8
        {
            pHighScoreTable = &gJolly_poitras_0x2BC0_6FEAC0->field_1890_stage_scores[field_EE0D_hiscore_table_idx][0]; // main district score
            Frontend::DrawScoreTable_4B5430((score_table_line*)&pHighScoreTable->field_0_score_table_line, 300, 250, 5, field_12A_score_font, 0xFFFF, 2);
        }
        else if (field_EE0D_hiscore_table_idx < 6)
        {
            pHighScoreTable = &gJolly_poitras_0x2BC0_6FEAC0->field_1890_stage_scores[0][field_EE0D_hiscore_table_idx - 2];
            Frontend::DrawScoreTable_4B5430((score_table_line*)&pHighScoreTable->field_0_score_table_line, 300, 250, 5, field_12A_score_font, 0xFFFF, 2);
        }
        else
        {
            if (field_EE0D_hiscore_table_idx < 9)
            {
                pHighScoreTable = &gJolly_poitras_0x2BC0_6FEAC0->field_1890_stage_scores[1][field_EE0D_hiscore_table_idx - 5];
                Frontend::DrawScoreTable_4B5430((score_table_line*)&pHighScoreTable->field_0_score_table_line, 300, 250, 5, field_12A_score_font, 0xFFFF, 2);
            }
            else
            {
                pHighScoreTable = &gJolly_poitras_0x2BC0_6FEAC0->field_1890_stage_scores[2][field_EE0D_hiscore_table_idx - 8];
                Frontend::DrawScoreTable_4B5430((score_table_line*)&pHighScoreTable->field_0_score_table_line, 300, 250, 5, field_12A_score_font, 0xFFFF, 2);
            }
        }

        if (!bIsLeftRightLoopEnabled_67DA80)
        {
            // left triangle
            if (field_EE0D_hiscore_table_idx == 0) // first option
            {
                pMenuPage->field_518_elements_array[2].field_1_is_it_displayed = false;
            }
            else
            {
                pMenuPage->field_518_elements_array[2].field_1_is_it_displayed = true;
            }
            // right triangle
            if (field_EE0D_hiscore_table_idx == 11) // last option
            {
                pMenuPage->field_518_elements_array[3].field_1_is_it_displayed = false;
            }
            else
            {
                pMenuPage->field_518_elements_array[3].field_1_is_it_displayed = true;
            }
        }
    }

    if (field_132_f136_idx == MENUPAGE_DEAD || field_132_f136_idx == MENUPAGE_AREA_COMPLETE || field_132_f136_idx == MENUPAGE_BONUS_AREA ||
        field_132_f136_idx == MENUPAGE_RESULTS_PLAYER_QUIT)
    {
        u8 main_level_idx;
        u8 bonus_stage_idx;

        if (!gLucid_hamilton_67E8E0.IsBonusStage_4C59A0())
        {
            main_level_idx = gLucid_hamilton_67E8E0.GetMainStageIdx_4C5980();
            bonus_stage_idx = 0;
        }
        else
        {
            gLucid_hamilton_67E8E0.DecodeStage_453A60(gLucid_hamilton_67E8E0.GetStage_4C5990(), &main_level_idx, &bonus_stage_idx);
        }
        if (field_132_f136_idx == MENUPAGE_BONUS_AREA)
        {
            s32 unk_offset = (3 * main_level_idx) + bonus_stage_idx + 64;
            swprintf(tmpBuff_67BD9C, L"%s %c", gText_0x14_704DFC->Find_5B5F90("bonslev"), unk_offset);
            wcsncpy(pMenuPage->field_518_elements_array[0].field_6_element_name_str, tmpBuff_67BD9C, 0x32u);
            DrawBonusRating_4B7D60();
        }

        Frontend::DrawScoreTable_4B5430((score_table_line*)&gJolly_poitras_0x2BC0_6FEAC0->field_1890_stage_scores[main_level_idx][bonus_stage_idx]
                                 .field_0_score_table_line,
                             0xAAu,
                             155,
                             3,
                             field_12A_score_font,
                             0xFFFF,
                             2);

        if (field_132_f136_idx == MENUPAGE_DEAD || field_132_f136_idx == MENUPAGE_AREA_COMPLETE ||
            field_132_f136_idx == MENUPAGE_RESULTS_PLAYER_QUIT)
        {
            Frontend::DrawLastAndBestStats_4B57B0(10, 0xE1);
        }
    }

    if (field_132_f136_idx == MENUPAGE_MULTIPLAYER_RESULTS)
    {
        Frontend::DrawMultiplayerScores_4B55F0(); // line 41d
    }

    u32 chosen_option_idx = -1;
    u16 option_idx = 0;

    wchar_t* wstr_array;

    u16 x_pos;
    u16 y_pos;

    for (option_idx = 0; option_idx < pMenuPage->field_0_number_of_options; option_idx++)
    {
        menu_option_0x82* pMenuOption = &pMenuPage->field_4_options_array[option_idx];

        if (pMenuOption->field_1_is_unlocked)
        {
            if (pMenuOption->field_0_option_type == STRING_TEXT_2)
            {
                Frontend::GetOptionText_4B3AF0(field_132_f136_idx, option_idx, &wstr_array);
            }
            else
            {
                wstr_array = (wchar_t*)&pMenuOption->field_6_option_name_str;
            }

            x_pos = pMenuOption->field_2_x_pos;
            y_pos = pMenuOption->field_4_y_pos;

            if (option_idx == pMenuPage->field_BC6_current_option_idx)
            {
                DrawText_4B87A0(wstr_array, x_pos, y_pos, field_120_selected_font, 1);

                if (field_132_f136_idx == MENUPAGE_PLAY)
                {
                    pMenuPage->field_518_elements_array[4].field_1_is_it_displayed = false;
                    pMenuPage->field_518_elements_array[5].field_1_is_it_displayed = false;
                    pMenuPage->field_518_elements_array[6].field_1_is_it_displayed = false;
                    pMenuPage->field_518_elements_array[7].field_1_is_it_displayed = false;
                    pMenuPage->field_518_elements_array[8].field_1_is_it_displayed = false;
                    pMenuPage->field_518_elements_array[9].field_1_is_it_displayed = false;
                    if (option_idx == 3) //  START PLAY IN AREA
                    {
                        chosen_option_idx = 3;
                        pMenuPage->field_518_elements_array[4].field_6_geometric_shape_type = 1;
                        pMenuPage->field_518_elements_array[5].field_6_geometric_shape_type = 2;
                        pMenuPage->field_518_elements_array[4].field_1_is_it_displayed = field_1EB4C_has_prev_main_stage != 0;
                        pMenuPage->field_518_elements_array[5].field_1_is_it_displayed = field_1EB4D_has_next_main_stage != 0;
                    }
                    else if (option_idx == 4) // BONUS STAGE
                    {
                        chosen_option_idx = 4;
                        pMenuPage->field_518_elements_array[6].field_6_geometric_shape_type = 1;
                        pMenuPage->field_518_elements_array[7].field_6_geometric_shape_type = 2;
                        pMenuPage->field_518_elements_array[6].field_1_is_it_displayed = field_1EB4E_has_prev_bonus_stage != 0;
                        pMenuPage->field_518_elements_array[7].field_1_is_it_displayed = field_1EB4F_has_next_bonus_stage != 0;
                    }
                    else if (option_idx == 0)
                    {
                        pMenuPage->field_518_elements_array[8].field_6_geometric_shape_type = 1;
                        pMenuPage->field_518_elements_array[9].field_6_geometric_shape_type = 2;
                        if (field_110_state != 3)
                        {
                            pMenuPage->field_518_elements_array[8].field_1_is_it_displayed = true;
                            pMenuPage->field_518_elements_array[9].field_1_is_it_displayed = true;
                            field_1EB4A_has_prev_player_slot = 1;
                            field_1EB4B_has_next_player_slot = 1;
                            if (!bIsLeftRightLoopEnabled_67DA80)
                            {
                                if (selected_option_idx == 0)
                                {
                                    pMenuPage->field_518_elements_array[8].field_1_is_it_displayed = false;
                                    field_1EB4A_has_prev_player_slot = 0;
                                }
                                else if (selected_option_idx == pMenuPage->field_4_options_array[0].field_7E_horizontal_max_idx)
                                {
                                    pMenuPage->field_518_elements_array[9].field_1_is_it_displayed = false;
                                    field_1EB4B_has_next_player_slot = 0;
                                }
                            }

                            last_xpos = sub_4B7E10(2, 0x12Cu, 0x1B8u, field_124_font_type, 0xFFFF);
                            sub_4B7E10(8, last_xpos + 300, 0x1B8u, field_124_font_type, 0xFFFF);
                            last_xpos = sub_4B7E10(1, 0x12Cu, 0x1CCu, field_124_font_type, 0xFFFF);
                            sub_4B7E10(9, last_xpos + 300, 0x1CCu, field_124_font_type, 0xFFFF);
                        }
                    }
                }
                else if (field_132_f136_idx == MENUPAGE_VIEW_HIGH_SCORE)
                {
                    pMenuPage->field_518_elements_array[2].field_6_geometric_shape_type = 3;
                    pMenuPage->field_518_elements_array[3].field_6_geometric_shape_type = 4;
                    if (option_idx == 0)
                    {
                        pMenuPage->field_518_elements_array[2].field_6_geometric_shape_type = 1;
                        pMenuPage->field_518_elements_array[3].field_6_geometric_shape_type = 2;
                    }
                }
                // ....
            }
            else
            {
                if (pMenuOption->field_6A_font_type != 0xFFFF)
                {
                    if (pMenuOption->field_6C_palette == 0xFFFF)
                    {
                        DrawText_4B87A0(wstr_array, x_pos, y_pos, pMenuOption->field_6A_font_type, 1);
                    }
                    else
                    {
                        DrawText_5D8A10(wstr_array, x_pos, y_pos, pMenuOption->field_6A_font_type, (s32)1, 8, pMenuOption->field_6C_palette, false, 0);
                    }
                }
                else
                {
                    if (pMenuOption->field_6C_palette == 0xFFFF)
                    {
                        DrawText_4B87A0(wstr_array, x_pos, y_pos, field_11C_normal_font, 1);
                    }
                    else
                    {
                        DrawText_5D8A10(wstr_array, x_pos, y_pos, field_11C_normal_font, (s32)1, 8, pMenuOption->field_6C_palette, false, 0);
                    }
                }
            }
        }
        else
        {
            bool v45 = false;
            if ((field_132_f136_idx == MENUPAGE_PLAY // 0x1
                 || field_132_f136_idx == MENUPAGE_AREA_COMPLETE) // 0x3
                && option_idx == 1)
            {
                v45 = true;
            }

            if (((field_132_f136_idx == MENUPAGE_RESULTS_PLAYER_QUIT || field_132_f136_idx == MENUPAGE_DEAD) && option_idx == 0) || v45)
            {
                if (pMenuOption->field_0_option_type == STRING_TEXT_2)
                {
                    Frontend::GetOptionText_4B3AF0(field_132_f136_idx, option_idx, &wstr_array);
                }
                else
                {
                    wstr_array = (wchar_t*)&pMenuOption->field_6_option_name_str;
                }

                x_pos = pMenuOption->field_2_x_pos;
                y_pos = pMenuOption->field_4_y_pos;

                if (pMenuOption->field_6A_font_type != 0xFFFF)
                {
                    DrawText_5D8A10(wstr_array, x_pos, y_pos, pMenuOption->field_6A_font_type, 1, 8, 8, false, 0);
                }
                else
                {
                    DrawText_5D8A10(wstr_array, x_pos, y_pos, field_11C_normal_font, 1, 8, 8, false, 0);
                }
            }

        } // end else
    } //  end FOR

    if (chosen_option_idx == 3 || chosen_option_idx == 4)
    {
        u8 main_level_idx;
        u8 bonus_level_idx;

        if (chosen_option_idx == 3) //  START PLAY IN AREA
        {
            main_level_idx = gLucid_hamilton_67E8E0.GetMainStageIdx_4C5980();
            bonus_level_idx = 0;
        }
        else if (chosen_option_idx == 4) // BONUS STAGE
        {
            gLucid_hamilton_67E8E0.DecodeStage_453A60(gLucid_hamilton_67E8E0.GetStage_4C5990(), &main_level_idx, &bonus_level_idx);
        }
        Frontend::DrawScoreTable_4B5430((score_table_line*)&gJolly_poitras_0x2BC0_6FEAC0->field_1890_stage_scores[main_level_idx][bonus_level_idx]
                                 .field_0_score_table_line,
                             0x12Cu,
                             v98,
                             1,
                             field_128,
                             0xFFFF,
                             2);
    }

    for (option_idx = 0; option_idx < pMenuPage->field_2_number_of_elements; option_idx++)
    {
        menu_element_0x6E* pMenuElement = &pMenuPage->field_518_elements_array[option_idx];

        if (pMenuElement->field_1_is_it_displayed)
        {
            u16 font_type;
            s32 shape_type;

            switch (pMenuElement->field_0_element_type)
            {
                case GEOMETRIC_SHAPE_3:
                    x_pos = pMenuElement->field_2_xpos;
                    y_pos = pMenuElement->field_4_ypos;

                    switch (pMenuElement->field_6_geometric_shape_type)
                    {
                        case 0u:
                            shape_type = 2;
                            break;
                        case 1u:
                            shape_type = 37;
                            break;
                        case 2u:
                            shape_type = 38;
                            break;
                        case 3u:
                            shape_type = 40;
                            break;
                        case 4u:
                            shape_type = 41;
                            break;
                        default:
                            break;
                    }

                    DrawFigure_5D7EC0(6, shape_type, x_pos, y_pos, kAngZero_67DA70, kFpOne_67D934, 2, 0, 0, false, 0);
                    break;

                case STRING_TEXT_1:
                    font_type = pMenuElement->field_6A_font_type;

                    x_pos = pMenuElement->field_2_xpos;
                    y_pos = pMenuElement->field_4_ypos;

                    if (font_type == 0xFFFF)
                    {
                        font_type = field_11C_normal_font;
                    }
                    Frontend::GetElementText_4B3CC0(field_132_f136_idx, option_idx, &wstr_array);

                    if (field_132_f136_idx == MENUPAGE_PLAY && (option_idx == 2 || option_idx == 3))
                    {
                        Frontend::DrawTextFixedWidth_4B78B0(wstr_array, x_pos, y_pos, font_type, pMenuElement->field_6C_font_palette, 1, 0x15u, 1);
                    }
                    else if (field_132_f136_idx == MENUPAGE_VIEW_HIGH_SCORE && option_idx == 1)
                    {
                        Frontend::DrawTextFixedWidth_4B78B0(wstr_array, x_pos, y_pos, font_type, pMenuElement->field_6C_font_palette, 1, 0x15u, 1);
                    }
                    else
                    {
                        if (pMenuElement->field_6C_font_palette == 0xFFFF)
                        {
                            DrawText_4B87A0(wstr_array, x_pos, y_pos, font_type, 1);
                        }
                        else
                        {
                            DrawText_5D8A10(wstr_array, x_pos, y_pos, font_type, 1, 8, pMenuElement->field_6C_font_palette, false, 0);
                        }
                    }
                    break;
                default:
                    break;
            }
        }
    }

    if (field_110_state == FrontendState::User_Typing_New_Player_Name_3) //  enter new player name
    {
        if (field_114_cursor_blink)
        {
            x_pos = pMenuPage->field_4_options_array[0].field_2_x_pos + Frontend::GetMaxTextWidth_5D8990(field_C9A0_curr_plyr_name, field_11C_normal_font);
            y_pos = pMenuPage->field_4_options_array[0].field_4_y_pos;
            swprintf(tmpBuff_67BD9C, L"_");
            DrawText_4B87A0(tmpBuff_67BD9C, x_pos, y_pos, field_11C_normal_font, 1);
        }
        wcscpy(tmpBuff_67BD9C, gText_0x14_704DFC->Find_5B5F90("entrnam"));
        x_pos = 0x15Eu;
        y_pos = gText_0x14_704DFC->field_10_lang_code != 'j' ? 12 : 16;
        DrawText_4B87A0(tmpBuff_67BD9C, x_pos, y_pos, field_126, 1);
    }
    if (field_110_state == 5 && field_114_cursor_blink) //  change current player name
    {
        x_pos = pMenuPage->field_518_elements_array[4].field_2_xpos + Frontend::GetMaxTextWidth_5D8990(field_C9B8_password, field_11C_normal_font);
        y_pos = pMenuPage->field_518_elements_array[4].field_4_ypos;
        swprintf(tmpBuff_67BD9C, L"_");
        DrawText_4B87A0(tmpBuff_67BD9C, x_pos, y_pos, field_11C_normal_font, 1);
    }
}

// https://decomp.me/scratch/qV1ie switch "goto" issue
WIP_FUNC(0x4B7AE0)
void Frontend::DrawCredits_4B7AE0()
{
    WIP_IMPLEMENTED;
    u16 font_type;
    s32 palette;
    s32 draw_kind;

    u16 credit_idx = field_1EB38_credits_line_idx;
    for (Fix16 y = field_1EB34_credits_ypos; y < 480 && credit_idx < 600; credit_idx++, y += field_EE0E_unk.field_2_lines[credit_idx].field_4_y_gap)
    {
        sleepy_stonebraker_0x6C* sleepy = &field_EE0E_unk.field_2_lines[credit_idx];
        switch (sleepy->field_6_string_category)
        {
            case 0: // normal string: white
                font_type = field_11E;
                draw_kind = 2;
                palette = 0;
                break;
            case 1: // ???
                font_type = field_120_selected_font;
                draw_kind = 2;
                palette = 0;
                break;
            case 2: // dev names: blue
                font_type = field_120_selected_font;
                draw_kind = 8;
                palette = 13;
                break;
            case 3: // department (DMA, T2 etc) : green
                font_type = field_120_selected_font;
                draw_kind = 8;
                palette = 14;
                break;
            case 4: // game name "GTA2" : yellow
                font_type = field_120_selected_font;
                draw_kind = 8;
                palette = 15;
                break;
            default:
                FatalError_4A38C0(Gta2Error::InvalidCreditTextColor, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 7966);
        }
        wchar_t* pStrBuf = sleepy->field_8_strBuf;
        if (wcscmp(pStrBuf, gEmptyWStr_67DC8C))
        {

            if (!wcscmp(pStrBuf, L"BINKLOGO"))
            {
                DrawFigure_5D7EC0(6, 1, (u16)320, y, kAngZero_67DA70, kFpOne_67D934, 2, 0, 0, 0, 0);
            }
            else if (!wcscmp(pStrBuf, L"MILESLOGO"))
            {
                DrawFigure_5D7EC0(6, 25, (u16)320, y, kAngZero_67DA70, kFpOne_67D934, 2, 0, 0, 0, 0);
            }
            else
            {
                s32 v7 = Frontend::GetMaxTextWidth_5D8990(pStrBuf, font_type);
                u16 draw_x = (640 - v7) / 2;
                DrawText_5D8A10(pStrBuf, draw_x, y, font_type, 1, draw_kind, palette, 0, 0);
            }
        }
    }

    if (pgbh_BlitImage(tgaArray_61F0C8[23].field_84_img, 0, 0, 451, 144, 85, 0) == -10)
    {
        Load_tga_4B6520(23u);
        pgbh_BlitImage(tgaArray_61F0C8[23].field_84_img, 0, 0, 451, 144, 85, 0);
    }
}

MATCH_FUNC(0x4B8650)
void Frontend::FreeSound_4B8650()
{
    if (!bSkip_audio_67D6BE)
    {
        if (snd2_67D6F8.field_8_sound_entry)
        {
            gRoot_sound_66B038.FreeSoundEntry_40EFD0(snd2_67D6F8.field_8_sound_entry);
            snd2_67D6F8.field_8_sound_entry = 0;
        }
    }
}

MATCH_FUNC(0x4B6030)
bool Frontend::pre_intro_bik_exists_4B6030()
{
    _finddata_t v3;
    long v1 = _findfirst(gFrontend_67DC84->pre_intro_bik_4B5F20(), &v3);
    if (v1 == -1)
    {
        return false;
    }
    else
    {
        _findclose(v1);
        return true;
    }
}

// TODO: the contents of these strings aren't known, only their addresses
DEFINE_GLOBAL_ARRAY_INIT(char_type, gBikDataDir_620454, 8, 0x620454, "data\\");
DEFINE_GLOBAL_ARRAY_INIT(char_type, gBikDriveDataDir_62045C, 8, 0x62045C, ":\\data\\");
DEFINE_GLOBAL_ARRAY_INIT(char_type, gIntroBikName_5FE76C, 16, 0x5FE76C, "movie\\intro.bik");
DEFINE_GLOBAL_ARRAY_INIT(char_type, gPreIntroBikName_5FE77C, 20, 0x5FE77C, "movie\\preintro.bik");
DEFINE_GLOBAL_ARRAY(char_type, gIntroBikPath_67DA84, 256, 0x67DA84);
DEFINE_GLOBAL_ARRAY(char_type, gPreIntroBikPath_67DB84, 256, 0x67DB84);

MATCH_FUNC(0x4B5F20)
char_type* Frontend::pre_intro_bik_4B5F20()
{
    char_type drive[32];
    drive[0] = gRoot_sound_66B038.GetAudioDriveLetter_40F150();
    if (drive[0])
    {
        drive[1] = 0;
        strcpy(gPreIntroBikPath_67DB84, drive);
        strcat(gPreIntroBikPath_67DB84, gBikDriveDataDir_62045C);
    }
    else
    {
        strcpy(gPreIntroBikPath_67DB84, gBikDataDir_620454);
    }
    strcat(gPreIntroBikPath_67DB84, gPreIntroBikName_5FE77C);
    return gPreIntroBikPath_67DB84;
}

MATCH_FUNC(0x4B5E50)
const char_type* Frontend::intro_bik_4B5E50()
{
    char_type drive[32];
    drive[0] = gRoot_sound_66B038.GetAudioDriveLetter_40F150();
    if (drive[0])
    {
        drive[1] = 0;
        strcpy(gIntroBikPath_67DA84, drive);
        strcat(gIntroBikPath_67DA84, gBikDriveDataDir_62045C);
    }
    else
    {
        strcpy(gIntroBikPath_67DA84, gBikDataDir_620454);
    }
    strcat(gIntroBikPath_67DA84, gIntroBikName_5FE76C);
    return gIntroBikPath_67DA84;
}

MATCH_FUNC(0x4B5FF0)
bool Frontend::intro_bik_exists_4B5FF0()
{
    // note: ecx wasn't first due to global being an object instead of a pointer
    _finddata_t findData;

    // note: put call in argument rather than local to change inst ordering
    const long hFind = _findfirst(gFrontend_67DC84->intro_bik_4B5E50(), &findData);

    if (hFind == -1)
    {
        return 0;
    }

    _findclose(hFind);
    return 1;
}

void sub_SetGamma()
{
    const s32 gammaVal = gRegistry_6FF968.Get_Screen_Setting_5870D0("gamma", 10);
    if (counter_706C4C)
    {
        if (SetGamma_5D9910(gammaVal))
        {
            --counter_706C4C;
        }
        else
        {
            counter_706C4C = 0;
        }
    }
}

MATCH_FUNC(0x4AEC00)
void Frontend::Update_4AEC00()
{
    read_menu_input_4AFEB0();
    UpdateMenuScreen_4B6780();

    snd1_67D818.field_0_object_type = 0;

    switch (field_110_state)
    {
        case FrontendState::User_Typing_New_Player_Name_3:
            HandlePlayerNameTyping_4B2F60();
            break;

        case FrontendState::Unknown_5:
            HandlePasswordTyping_4B8280();
            break;

        case FrontendState::Unknown_1:
            if (field_132_f136_idx == MENUPAGE_CREDITS)
            {
                snd1_67D818.field_4_bStatus = 1;
                ManageCredits_4B7A10();
            }
            else
            {
                snd1_67D818.field_4_bStatus = 0;
                UpdatePageFromUserInput_4AE2D0();
            }
            break;

        case FrontendState::Booting_Map_2:
            SetWinMainStateToBootMap_4AE990();
            break;

        case FrontendState::Unknown_4:
            HandleDeletePlayerDialog_4AE9A0();
            break;

        default:
            FatalError_4A38C0(Gta2Error::InvalidCase, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 2059, field_110_state);
    }

    if (!bSkip_audio_67D6BE)
    {
        gRoot_sound_66B038.Service_40EFA0();
    }

    if (counter_706C4C > 0)
    {
        sub_SetGamma();
    }
}

MATCH_FUNC(0x4AFEB0)
void Frontend::read_menu_input_4AFEB0()
{
    if (field_10D_bInputEnabled && KeyBoard_GetKeyStates_4AFDD0())
    {
        const u8 up = field_8_keys[DIK_UP] & 0x80;
        field_C9CE_up_pressed = up && !field_C9D5_up_key_down;
        field_C9D5_up_key_down = up;

        const u8 down = field_8_keys[DIK_DOWN] & 0x80;
        field_C9CF_down_pressed = down && !field_C9D6_down_key_down;
        field_C9D6_down_key_down = down;

        const u8 left = field_8_keys[DIK_LEFT] & 0x80;
        field_C9CC_left_pressed = left && !field_C9D3_left_key_down;
        field_C9D3_left_key_down = left;

        const u8 right = field_8_keys[DIK_RIGHT] & 0x80;
        field_C9CD_right_pressed = right && !field_C9D4_right_key_down;
        field_C9D4_right_key_down = right;

        const u8 returnKey = field_8_keys[DIK_RETURN] & 0x80;
        field_C9D0_return_pressed = returnKey && !field_C9D7_return_key_down;
        field_C9D7_return_key_down = returnKey;

        const u8 escape = field_8_keys[DIK_ESCAPE] & 0x80;
        field_C9D1_escape_pressed = escape && !field_C9D8_escape_key_down;
        field_C9D8_escape_key_down = escape;

        const u8 deleteKey = field_8_keys[DIK_DELETE] & 0x80;
        field_C9D2_delete_pressed = deleteKey && !field_C9D9_delete_key_down;
        field_C9D9_delete_key_down = deleteKey;
    }
    else
    {
        field_C9CE_up_pressed = 0;
        field_C9CF_down_pressed = 0;
        field_C9CC_left_pressed = 0;
        field_C9CD_right_pressed = 0;
        field_C9D0_return_pressed = 0;
        field_C9D1_escape_pressed = 0;
        field_C9D2_delete_pressed = 0;
    }
}

MATCH_FUNC(0x4B6780)
void Frontend::UpdateMenuScreen_4B6780()
{
    MenuPage_0xBCA* pBorg = &field_136_menu_pages_array[field_132_f136_idx];
    if (field_110_state != 2)
    {
        if (field_132_f136_idx == MENUPAGE_START_MENU)
        {
            switch (pBorg->field_BC6_current_option_idx)
            {
                case 0:
                    field_EE08_menu_screen = Play_1;
                    break;

                case 1:
                    field_EE08_menu_screen = Options_0;
                    break;

                case 2:
                    field_EE08_menu_screen = Quit_2;
                    break;
            }
        }
        else if (field_132_f136_idx == MENUPAGE_PLAY)
        {
            switch (pBorg->field_BC6_current_option_idx)
            {
                case 0:
                    field_EE08_menu_screen = EnterPlayerName_10;
                    break;
                case 1:
                    field_EE08_menu_screen = ResumeLoadSave_11;
                    break;
                case 2:
                    field_EE08_menu_screen = ViewHiScore_6;
                    break;
                case 3:
                    field_EE08_menu_screen = 7 + gLucid_hamilton_67E8E0.GetMainStageIdx_4C5980();
                    break;
                case 4:
                {
                    u8 main_stage_idx;
                    u8 bonus_stage_idx;
                    gLucid_hamilton_67E8E0.DecodeStage_453A60(gLucid_hamilton_67E8E0.GetStage_4C5990(), &main_stage_idx, &bonus_stage_idx);
                    field_EE08_menu_screen = main_stage_idx + 3;
                    break;
                }
                default:
                    break;
            }
        }
        else if (field_132_f136_idx == MENUPAGE_VIEW_HIGH_SCORE)
        {
            field_EE08_menu_screen = HiScoresDisplay_12;
        }
        else if (field_132_f136_idx == MENUPAGE_DEAD)
        {
            field_EE08_menu_screen = GameOver_13;
        }
        else if (field_132_f136_idx == MENUPAGE_AREA_COMPLETE || field_132_f136_idx == MENUPAGE_BONUS_AREA ||
                 field_132_f136_idx == MENUPAGE_MULTIPLAYER_RESULTS || field_132_f136_idx == MENUPAGE_RESULTS_PLAYER_QUIT ||
                 field_132_f136_idx == MENUPAGE_PARENTAL_CONTROL)
        {
            field_EE08_menu_screen = RedBar_16;
        }
        else if (field_132_f136_idx == MENUPAGE_GAME_COMPLETE || field_132_f136_idx == MENUPAGE_NICE_TRY)
        {
            field_EE08_menu_screen = Loading_15;
        }
        else if (field_132_f136_idx == MENUPAGE_CREDITS)
        {
            field_EE08_menu_screen = Credits_17;
        }
        else
        {
            field_EE08_menu_screen = PlayArea1_7;
        }
    }
}

// https://decomp.me/scratch/3NE2J
MATCH_FUNC(0x4B7A10)
void Frontend::ManageCredits_4B7A10()
{
    timeGetTime();
    Frontend::read_menu_input_4AFEB0();
    bool bKeyPressed = false;
    char_type* pKeyIter = &field_8_keys[0];

    for (s32 i = 256; i; i--)
    {
        if ((*pKeyIter & 0x80u) != 0)
        {
            bKeyPressed = true;
        }
        ++pKeyIter;
    }

    if (bKeyPressed)
    {
        if (!field_C9B3_key_held)
        {
            field_108_winmain_next_state = Quit_1;
            return;
        }
    }
    else
    {
        field_C9B3_key_held = 0;
    }

    if (++field_1EB30_credits_scroll_timer > 0)
    {
        field_1EB30_credits_scroll_timer = 0;

        if (field_1EB34_credits_ypos <= 262124)
        {
            while (1)
            {
                if (++field_1EB38_credits_line_idx == 600)
                {
                    field_108_winmain_next_state = Quit_1;
                    return;
                }
                if ((field_1EB34_credits_ypos += Fix16(field_EE0E_unk.field_2_lines[field_1EB38_credits_line_idx].field_4_y_gap)) > 262124)
                {
                    break;
                }
            }
        }
        field_1EB34_credits_ypos -= kFpOne_67D9FC;
    }
}

MATCH_FUNC(0x4AE2D0)
void Frontend::UpdatePageFromUserInput_4AE2D0()
{
    MenuPage_0xBCA* pBorg; // ebx
    player_stats_0xA4* v3; // ebp
    u16 target_page_idx; // ax
    u8 v5; // bl
    u8 main_stage_idx; // al
    u8 v7;
    u16 field_BC6_nifty_idx; // cx
    menu_option_0x82* v11; // edi
    bool v12; // bl
    s32 v13; // eax
    u16 v14; // cx
    menu_option_0x82* v15; // edi
    bool v16; // bl
    MenuPage_0xBCA* v18; // [esp+10h] [ebp-Ch]
    u8 stage_main_idx;
    u8 stage_bonus_idx;

    pBorg = &field_136_menu_pages_array[field_132_f136_idx];
    v18 = pBorg;
    v3 = GetCurrPlayerStats_4B43E0();
    if (field_C9D0_return_pressed)
    {
        menu_option_0x82* pOption = &pBorg->field_4_options_array[pBorg->field_BC6_current_option_idx];
        if (pOption->field_0_option_type == STRING_TEXT_1)
        {
            target_page_idx = pOption->field_80_menu_page_target;
            switch (target_page_idx)
            {
                case 263u:
                    stage_main_idx = gLucid_hamilton_67E8E0.GetMainStageIdx_4C5980();
                    v5 = 3;
                    for (stage_bonus_idx = 3; !v3->field_0_plyr_stage_stats[stage_main_idx][stage_bonus_idx].field_0_is_stage_unlocked || v5 >= field_1EB51_num_bonus_stages[stage_main_idx]; stage_bonus_idx = v5)
                    {
                        --v5;
                    }
                    LoadMapFilenames_4B4D00(stage_main_idx, stage_bonus_idx);
                    gLucid_hamilton_67E8E0.SetStartedFromPlayBonusMenu_4C5AD0(0);
                    field_EE08_menu_screen = RedBar_16;
                    field_110_state = FrontendState::Booting_Map_2;
                    break;
                case MENUPAGE_PLAY_NEXT_AREA: // 261
                    if (!gLucid_hamilton_67E8E0.IsBonusStage_4C59A0())
                    {
                        main_stage_idx = gLucid_hamilton_67E8E0.GetMainStageIdx_4C5980();
                    }
                    else
                    {
                        main_stage_idx = (u8)gLucid_hamilton_67E8E0.GetStage_4C5990() >> 4;
                    }

                    if (FreeLoader::CheckCityInstalled_4AE1F0(main_stage_idx + 1))
                    {
                        stage_main_idx = main_stage_idx + 1;
                        if (stage_main_idx >= field_1EB50_num_main_stages)
                        {
                            FatalError_4A38C0(Gta2Error::InvalidLevelAdvancement, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 1543);
                        }

                        if (!v3->field_0_plyr_stage_stats[stage_main_idx][0].field_0_is_stage_unlocked)
                        {
                            FatalError_4A38C0(Gta2Error::LevelNotOpened, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 1548);
                        }
                        LoadMapFilenames_4B4D00(stage_main_idx, 0);
                        field_EE08_menu_screen = RedBar_16;
                        field_110_state = FrontendState::Booting_Map_2;
                    }
                    break;
                case MENUPAGE_REPLAY_PREVIOUS_AREA: // 259
                    gLucid_hamilton_67E8E0.DebugStr_4C58D0("");
                    field_EE08_menu_screen = RedBar_16;
                    field_110_state = FrontendState::Booting_Map_2;
                    break;
                case MENUPAGE_LOADING_SAVE: // 260
                    stage_bonus_idx = gLucid_hamilton_67E8E0.GetPlySlotIdx_4C59B0();
                    if (PlySlotSvgExists_4B5370(stage_bonus_idx))
                    {
                        sub_4B4EC0();
                    }
                    else
                    {
                        gLucid_hamilton_67E8E0.DebugStr_4C58D0("");
                    }
                    field_EE08_menu_screen = RedBar_16;
                    field_110_state = FrontendState::Booting_Map_2;
                    break;
                case MENUPAGE_GTA2MANAGER: // 257
                    Start_GTA2Manager_5E4DE0();
                    break;
                case MENUPAGE_QUIT: // 258
                    field_108_winmain_next_state = Quit_1;
                    break;
                case MENUPAGE_GET_READY_TO_PLAY: // 264
                    stage_main_idx = gLucid_hamilton_67E8E0.GetMainStageIdx_4C5980();
                    if (FreeLoader::CheckCityInstalled_4AE1F0(stage_main_idx))
                    {
                        LoadMapFilenames_4B4D00(stage_main_idx, 0);
                        field_EE08_menu_screen = RedBar_16;
                        field_110_state = FrontendState::Booting_Map_2;
                    }
                    break;
                case MENUPAGE_GET_READY_TO_PLAY_BONUS: // 265
                    gLucid_hamilton_67E8E0.DecodeStage_453A60(gLucid_hamilton_67E8E0.GetStage_4C5990(), &stage_main_idx, &stage_bonus_idx);
                    if (FreeLoader::CheckCityInstalled_4AE1F0(stage_main_idx))
                    {
                        LoadMapFilenames_4B4D00(stage_main_idx, stage_bonus_idx);
                        gLucid_hamilton_67E8E0.SetStartedFromPlayBonusMenu_4C5AD0(1);
                        field_EE08_menu_screen = RedBar_16;
                        field_110_state = FrontendState::Booting_Map_2;
                    }
                    break;
                case MENUPAGE_CONTINUE_NEXT_STAGE: // 266
                    ContinueToNextStage_4B8020();
                    break;
                case 268u:
                    break;
                default:
                    ChangeMenuPage_4B3170(target_page_idx);
                    break;
            }
            snd1_67D818.field_0_object_type = 5;
        }
        else if (field_132_f136_idx == MENUPAGE_PLAY && pBorg->field_BC6_current_option_idx == 0) // player
        {
            field_110_state = FrontendState::User_Typing_New_Player_Name_3;
            LoadCurrPlayerName_4B4280();
            field_C9B2_curr_plyr_name_length = wcslen(field_C9A0_curr_plyr_name);
            StripPlayerNameToCurrLength_4B42B0();
            field_C9B3_key_held = 1;
            field_C9B4_last_key = DIK_RETURN;
            field_C9B6_key_repeat_timer = 5;
            snd1_67D818.field_0_object_type = 5;
        }
    }

    if (field_C9D1_escape_pressed)
    {
        switch (field_132_f136_idx)
        {
            case MENUPAGE_START_MENU:
            case MENUPAGE_PARENTAL_CONTROL:
                ChangeMenuPage_4B3170(MENUPAGE_CREDITS);
                break;
            case MENUPAGE_PLAY:
            case MENUPAGE_DEAD:
            case MENUPAGE_AREA_COMPLETE:
            case MENUPAGE_GAME_COMPLETE:
            case MENUPAGE_BONUS_AREA:
            case MENUPAGE_NICE_TRY:
            case MENUPAGE_RESULTS_PLAYER_QUIT:
                ChangeMenuPage_4B3170(MENUPAGE_START_MENU);
                break;
            case MENUPAGE_VIEW_HIGH_SCORE:
                ChangeMenuPage_4B3170(MENUPAGE_PLAY);
                break;
            default:
                field_108_winmain_next_state = Quit_1;
                break;
        }
        snd1_67D818.field_0_object_type = 6;
    }

    if (field_C9CE_up_pressed && pBorg->SelectPrevOption_4B61B0())
    {
        snd1_67D818.field_0_object_type = 1;
    }

    if (field_C9CF_down_pressed && pBorg->SelectNextOption_4B6200())
    {
        snd1_67D818.field_0_object_type = 2;
    }

    if (field_C9CC_left_pressed)
    {
        field_BC6_nifty_idx = pBorg->field_BC6_current_option_idx;
        v11 = &pBorg->field_4_options_array[field_BC6_nifty_idx];
        if (v11->field_0_option_type == STRING_TEXT_2)
        {
            v12 = v11->SelectPrevHorizontalIdx_4B6390();
            if (field_132_f136_idx == MENUPAGE_PLAY && !v18->field_BC6_current_option_idx)
            {
                gLucid_hamilton_67E8E0.SetPlySlotIdx_4C5920(v11->field_6E_horizontal_selected_idx);
                UpdateMenuForCurrPlayer_4B42E0();
                gRegistry_6FF968.Set_Player_Setting_5878C0("plyrslot", v11->field_6E_horizontal_selected_idx);
                if (v12)
                {
                    snd1_67D818.field_0_object_type = 3;
                }
            }

            if (field_132_f136_idx == MENUPAGE_VIEW_HIGH_SCORE && !v18->field_BC6_current_option_idx)
            {
                field_EE0D_hiscore_table_idx = v11->field_6E_horizontal_selected_idx;
                if (v12)
                {
                    snd1_67D818.field_0_object_type = 3;
                }
            }
            pBorg = v18;
        }
        else if (field_132_f136_idx == MENUPAGE_PLAY)
        {
            if (field_BC6_nifty_idx == 3)
            {
                if (ChangeMainStageToPrevious_4B6FF0())
                {
                    snd1_67D818.field_0_object_type = 3;
                }
            }
            else if (field_BC6_nifty_idx == 4 && ChangeBonusStageToPrevious_4B70B0())
            {
                snd1_67D818.field_0_object_type = 3;
            }
        }
    }
    else if (field_C9CD_right_pressed)
    {
        v14 = pBorg->field_BC6_current_option_idx;
        v15 = &pBorg->field_4_options_array[v14];
        if (v15->field_0_option_type == STRING_TEXT_2)
        {
            v16 = v15->SelectNextHorizontalIdx_4B6330();
            if (field_132_f136_idx == MENUPAGE_PLAY && !v18->field_BC6_current_option_idx)
            {
                gLucid_hamilton_67E8E0.SetPlySlotIdx_4C5920(v15->field_6E_horizontal_selected_idx);
                UpdateMenuForCurrPlayer_4B42E0();
                gRegistry_6FF968.Set_Player_Setting_5878C0("plyrslot", v15->field_6E_horizontal_selected_idx);
                if (v16)
                {
                    snd1_67D818.field_0_object_type = 4;
                }
            }
            if (field_132_f136_idx == MENUPAGE_VIEW_HIGH_SCORE && !v18->field_BC6_current_option_idx)
            {
                field_EE0D_hiscore_table_idx = v15->field_6E_horizontal_selected_idx;
                if (v16)
                {
                    snd1_67D818.field_0_object_type = 4;
                }
            }
            pBorg = v18;
        }
        else if (field_132_f136_idx == MENUPAGE_PLAY)
        {
            if (v14 == 3)
            {
                if (ChangeMainStageToNext_4B7200())
                {
                    snd1_67D818.field_0_object_type = 4;
                }
            }
            else if (v14 == 4 && ChangeBonusStageToNext_4B72F0())
            {
                snd1_67D818.field_0_object_type = 4;
            }
        }
    }
    if (field_C9D2_delete_pressed && field_132_f136_idx == MENUPAGE_PLAY && !pBorg->field_BC6_current_option_idx)
    {
        field_110_state = 4;
        field_EE0A_dialog_cursor_ypos = 190;
        field_EE0C_dialog_type = 1;
        snd1_67D818.field_0_object_type = 8;
    }

    if (--field_118_cursor_blink_timer <= 0)
    {
        field_114_cursor_blink = field_114_cursor_blink == 0;
        field_118_cursor_blink_timer = 2;
    }
}

MATCH_FUNC(0x4AE990)
void Frontend::SetWinMainStateToBootMap_4AE990()
{
    field_108_winmain_next_state = Start_Game_3;
}

// It matches, but we need to get rid of goto's
// https://decomp.me/scratch/LYZij
MATCH_FUNC(0x4B2F60)
void Frontend::HandlePlayerNameTyping_4B2F60()
{
    //NOT_IMPLEMENTED;
    s16 v1;
    s16 input;
    u8* pKeys;
    wchar_t Key_4D5F40;
    u16 v7;
    s16 v9;

    v1 = 0;
    input = 256;
    pKeys = (u8*)field_8_keys;
    do
    {
        if ((*pKeys & 0x80u) != 0 && v1 != 54 && v1 != 42)
        {
            input = v1;
        }
        ++v1;
        ++pKeys;
    } while ((u16)v1 < 0x100u);

    if (field_C9B4_last_key != input)
    {
        field_C9B4_last_key = input;
        field_C9B6_key_repeat_timer = 5;

        if (input == DIK_RETURN)
        {
            field_110_state = 1;
            Frontend::StripPlayerNameToCurrLength_4B42B0();
            Frontend::SaveAndUpdatePlayerName_4B4230();
            field_136_menu_pages_array[1].field_BC6_current_option_idx = 0;
            if (snd1_67D818.field_0_object_type != 9)
            {
                snd1_67D818.field_0_object_type = 5;
            }
        }
        else if (input == DIK_BACKSPACE)
        {
            //v8 = field_C9B2_curr_plyr_name_length;
            if (field_C9B2_curr_plyr_name_length > 0)
            {
                field_C9B2_curr_plyr_name_length--;
                Frontend::StripPlayerNameToCurrLength_4B42B0();
                snd1_67D818.field_0_object_type = 8;
            }
        }
        else if (input == DIK_ESCAPE)
        {
            field_110_state = 1;
            Frontend::StripPlayerNameToCurrLength_4B42B0();
            field_136_menu_pages_array[1].field_BC6_current_option_idx = 0;
            snd1_67D818.field_0_object_type = 6;
        }
        else if (input == 256)
        {
        LABEL_27:
            field_C9B3_key_held = 0;
            goto LABEL_29;
        }
        else if (input == DIK_SPACE)
        {
            v7 = 32;
        LABEL_13:
            if (field_C9B2_curr_plyr_name_length != 8)
            {
                field_C9A0_curr_plyr_name[field_C9B2_curr_plyr_name_length++] = v7;
                snd1_67D818.field_0_object_type = 7;
            }
            goto LABEL_26;
        }
        keybrd_0x204::RecreateIfLayoutChanged_4D5FD0();
        Key_4D5F40 = gKeybrd_0x204_6F52F4->GetKey_4D5F40(input);
        v7 = gText_0x14_704DFC->RemapExtendedChar_5B58D0(Key_4D5F40);
        if ((u16)GetCharWidth_4539D0(field_11C_normal_font, v7) >= 3u && v7)
        {
            goto LABEL_13;
        }
    }
    else
    {
        v9 = field_C9B6_key_repeat_timer;
        if (v9 == 0)
        {
            field_C9B4_last_key = 256;
            field_C9B6_key_repeat_timer = 5;
        }
        else
        {
            field_C9B6_key_repeat_timer = v9 - 1;
        }
    }
LABEL_26:
    if (input == 256)
    {
        goto LABEL_27;
    }
    field_C9B3_key_held = 1;
LABEL_29:
    field_118_cursor_blink_timer--;
    if (field_118_cursor_blink_timer <= 0)
    {
        field_114_cursor_blink = field_114_cursor_blink == 0;
        field_118_cursor_blink_timer = 2;
    }
}

MATCH_FUNC(0x4AE9A0)
void Frontend::HandleDeletePlayerDialog_4AE9A0()
{
    u16 v2;

    if (field_C9D0_return_pressed)
    {
        v2 = field_EE0A_dialog_cursor_ypos;
        if (v2 == 210)
        {
            switch (field_EE0C_dialog_type)
            {
                case 1:
                    Frontend::DeleteCurrentPlayer_4B4410();
                    field_110_state = FrontendState::Unknown_1;
                    break;
                default:
                    FatalError_4A38C0(Gta2Error::InvalidCase, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 1934);
            }
        }
        if (v2 == 230)
        {
            field_110_state = FrontendState::Unknown_1;
        }
        snd1_67D818.field_0_object_type = 5;
    }

    if (field_C9D1_escape_pressed)
    {
        field_110_state = 1;
        snd1_67D818.field_0_object_type = 6;
    }
    if (field_C9CE_up_pressed)
    {
        switch (field_EE0A_dialog_cursor_ypos)
        {
            case 190:
            case 210:
                field_EE0A_dialog_cursor_ypos = 230;
                break;
            case 230:
                field_EE0A_dialog_cursor_ypos = 210;
                break;
            default:
                FatalError_4A38C0(Gta2Error::InvalidCase, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 1968);
                break;
        }
        snd1_67D818.field_0_object_type = 1;
    }
    if (field_C9CF_down_pressed)
    {
        switch (field_EE0A_dialog_cursor_ypos)
        {
            case 190:
                field_EE0A_dialog_cursor_ypos = 210;
                break;
            case 210:
                field_EE0A_dialog_cursor_ypos = 230;
                snd1_67D818.field_0_object_type = 2;
                break;
            case 230:
                field_EE0A_dialog_cursor_ypos = 210;
                break;
            default:
                FatalError_4A38C0(Gta2Error::InvalidCase, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 1991);
                break;
        }
        snd1_67D818.field_0_object_type = 2;
    }
    field_118_cursor_blink_timer--;
    if (field_118_cursor_blink_timer <= 0)
    {
        field_114_cursor_blink = field_114_cursor_blink == 0;
        field_118_cursor_blink_timer = 2;
    }
}

// https://decomp.me/scratch/ySQ2h
MATCH_FUNC(0x4B8280)
void Frontend::HandlePasswordTyping_4B8280()
{
    wchar_t v6;
    s16 v3 = 256;
    char_type* pKeyIter = &field_8_keys[0];

    for (u16 i = 0; i < 256; i++, ++pKeyIter)
    {
        if ((*pKeyIter & 0x80u) != 0 && i != 54 && i != 42)
        {
            v3 = i;
        }
    }

    if (field_C9B4_last_key != v3)
    {
        field_C9B4_last_key = v3;
        field_C9B6_key_repeat_timer = 5;

        if (v3 == DIK_RETURN)
        {
            field_110_state = 1;
            Frontend::StripPasswordToCurrLength_4B8530();
            Frontend::CheckPassword_4B8560();
            snd1_67D818.field_0_object_type = 5;
        }
        else if (v3 == DIK_BACK)
        {
            if (field_C9CA_password_length > 0)
            {
                field_C9CA_password_length--;
                Frontend::StripPasswordToCurrLength_4B8530();
                snd1_67D818.field_0_object_type = 8;
            }
        }
        else if (v3 == DIK_ESCAPE)
        {
            field_110_state = 1;
            Frontend::StripPasswordToCurrLength_4B8530();
            Frontend::ChangeMenuPage_4B3170(9);
            snd1_67D818.field_0_object_type = 6;
        }
        else if (v3 == 256)
        {
            goto LABEL_27;
        }
        else if (v3 == DIK_SPACE)
        {
            v6 = 32;
        LABEL_12:
            if (field_C9CA_password_length != 8)
            {
                if (field_C9CB_wrong_password_shown)
                {
                    field_C9CB_wrong_password_shown = 0;
                    wcsncpy(field_136_menu_pages_array[14].field_518_elements_array[0].field_6_element_name_str,
                            gText_0x14_704DFC->Find_5B5F90("fr_ent1"),
                            0x32u);
                    wcsncpy(field_136_menu_pages_array[14].field_518_elements_array[1].field_6_element_name_str,
                            gText_0x14_704DFC->Find_5B5F90("fr_ent2"),
                            0x32u);
                    wcsncpy(field_136_menu_pages_array[14].field_518_elements_array[2].field_6_element_name_str, gEmptyWStr_67DC8C, 0x32u);
                }
                field_C9B8_password[field_C9CA_password_length++] = v6;
                snd1_67D818.field_0_object_type = 7;
            }
            goto LABEL_27;
        }
        u16 v5 = gKeybrd_0x204_6F52F4->GetKey_4D5F40(v3);
        v6 = v5;
        if (GetCharWidth_4539D0(field_11C_normal_font, v6) >= 3 && v6)
        {
            goto LABEL_12;
        }
    }
    else
    {
        if (field_C9B6_key_repeat_timer == 0)
        {
            field_C9B4_last_key = 256;
            field_C9B6_key_repeat_timer = 5;
        }
        else
        {
            field_C9B6_key_repeat_timer--;
        }
    }

LABEL_27:
    if (v3 == 256)
    {
        field_C9B3_key_held = 0;
    }
    else
    {
        field_C9B3_key_held = 1;
    }
    field_118_cursor_blink_timer--;
    if (field_118_cursor_blink_timer <= 0)
    {
        field_114_cursor_blink = (field_114_cursor_blink == 0);
        field_118_cursor_blink_timer = 2;
    }
}

MATCH_FUNC(0x4B4410)
void Frontend::DeleteCurrentPlayer_4B4410()
{
    GetCurrPlayerStats_4B43E0()->ResetPlayerSlot_56B630();
    gJolly_poitras_0x2BC0_6FEAC0->SavePlySlotDat_56BA60(field_136_menu_pages_array[1].field_4_options_array[0].field_6E_horizontal_selected_idx);
    UpdateMenuForCurrPlayer_4B42E0();
}

MATCH_FUNC(0x4B43E0)
player_stats_0xA4* Frontend::GetCurrPlayerStats_4B43E0()
{
    // note: movsx vs movzx due to signedness
    u16 idx = gLucid_hamilton_67E8E0.GetPlySlotIdx_4C59B0();
    return &gJolly_poitras_0x2BC0_6FEAC0->field_26A0_plyr_stats[idx];
}

MATCH_FUNC(0x4B42E0)
void Frontend::UpdateMenuForCurrPlayer_4B42E0()
{
    player_stats_0xA4* pPlayerStats = Frontend::GetCurrPlayerStats_4B43E0();
    u8 PlySlotIdx_4C59B0 = gLucid_hamilton_67E8E0.GetPlySlotIdx_4C59B0();
    MenuPage_0xBCA* pMenuPage = &field_136_menu_pages_array[field_132_f136_idx];

    u8 v4 = Frontend::GetPrevUnlockedStageIndex_4B77B0(pPlayerStats);
    u8 v8 = Frontend::GetPrevUnlockedStageBonusCode_4B7800(pPlayerStats);

    if (v4 < field_1EB3A_selected_main_stage[PlySlotIdx_4C59B0])
    {
        field_1EB3A_selected_main_stage[PlySlotIdx_4C59B0] = v4;
        gLucid_hamilton_67E8E0.SetMainStageIdx_4C58F0(v4);
    }
    else
    {
        gLucid_hamilton_67E8E0.SetMainStageIdx_4C58F0(field_1EB3A_selected_main_stage[PlySlotIdx_4C59B0]);
    }

    if (v8 < field_1EB42_selected_bonus_stage[PlySlotIdx_4C59B0] || v8 == 0xFF)
    {
        field_1EB42_selected_bonus_stage[PlySlotIdx_4C59B0] = v8;
        gLucid_hamilton_67E8E0.SetStage_4C5900(v8);
    }
    else
    {
        gLucid_hamilton_67E8E0.SetStage_4C5900(field_1EB42_selected_bonus_stage[PlySlotIdx_4C59B0]);
    }

    Frontend::UpdateBonusStageArrows_4B7610();
    Frontend::UpdateMainStageArrows_4B7550();
    if (Frontend::PlySlotSvgExists_4B5370(PlySlotIdx_4C59B0))
    {
        pMenuPage->field_4_options_array[1].field_1_is_unlocked = 1;
        pMenuPage->field_B8A[1].field_4_is_option_unlocked = 1;
    }
    else
    {
        pMenuPage->field_4_options_array[1].field_1_is_unlocked = 0;
        pMenuPage->field_B8A[1].field_4_is_option_unlocked = 0;
    }
}

MATCH_FUNC(0x4B4230)
void Frontend::SaveAndUpdatePlayerName_4B4230()
{
    u16 player_slot_idx = field_136_menu_pages_array[1].field_4_options_array[0].field_6E_horizontal_selected_idx;
    wchar_t* pPlayerName = gJolly_poitras_0x2BC0_6FEAC0->field_26A0_plyr_stats[player_slot_idx].field_90_strPlayerName;
    wcsncpy(pPlayerName, field_C9A0_curr_plyr_name, 9u);
    HandleCheatCode_4B3DD0(pPlayerName);
    gJolly_poitras_0x2BC0_6FEAC0->SavePlySlotDat_56BA60(player_slot_idx);
}

MATCH_FUNC(0x4B3CC0)
void Frontend::GetElementText_4B3CC0(u16 a2, u16 a3, wchar_t** a4)
{
    menu_element_0x6E* temp = &field_136_menu_pages_array[a2].field_518_elements_array[a3];

    if (a2 == 14 && a3 == 4)
    {
        wcscpy(gTmpWideStr_67C7D8, field_C9B8_password);
    }
    else if ((a2 == 14 && a3 != 4) || a2 != 5 || a3 != 1)
    {
        swprintf(gTmpWideStr_67C7D8, L"%s", temp->field_6_element_name_str);
    }
    else if (field_EE0D_hiscore_table_idx < 3)
    {
        swprintf(gTmpWideStr_67C7D8, L"%d", field_EE0D_hiscore_table_idx + 1);
    }
    else
    {
        swprintf(gTmpWideStr_67C7D8, L"%c", field_EE0D_hiscore_table_idx + 62);
    }
    *a4 = (wchar_t*)&gTmpWideStr_67C7D8;
}

MATCH_FUNC(0x4B3DD0)
void Frontend::HandleCheatCode_4B3DD0(const wchar_t* cheat_str_wide)
{
    const char* ascii_cheat_str = text_0x14::Wide2PesudoAscii_5B5D10(cheat_str_wide);
    const size_t cheat_str_len = wcslen(cheat_str_wide);

    if (cheat_str_len > 16) // OG bug - should be checking for 8?
    {
        return;
    }

    s32 cheat_str_hash = 0;
    u32 str_idx = 0;
    if (cheat_str_len > 0)
    {
        do
        {
            cheat_str_hash += sCheatHashSecret_61F0A8[str_idx] * ascii_cheat_str[str_idx];
        } while (++str_idx < cheat_str_len);

        if (cheat_str_hash == 0x49362) // GOURANGA
        {
            this->field_C9E1_bCheatsEnabled = 1;
            snd1_67D818.field_0_object_type = 9;
            return;
        }
    }

    if (!field_C9E1_bCheatsEnabled)
    {
        return;
    }

    if (cheat_str_hash == 0x484DF)
    { // GOREFEST
        bDo_blood_67D5C5 = bDo_blood_67D5C5 == 0;
        snd1_67D818.field_0_object_type = 9;
    }
    else if (cheat_str_hash == 0x4878D)
    { // BUCKFAST Only mugger peds spawn
        gCheatOnlyMuggerPeds_67D5A4 = gCheatOnlyMuggerPeds_67D5A4 == 0;
        snd1_67D818.field_0_object_type = 9;
    }
    else if (cheat_str_hash == 0x4DA77)
    { // VOLTFEST Electro Gun with infinite ammo
        gCheatUnlimitedElectroGun_67D4F7 = gCheatUnlimitedElectroGun_67D4F7 == 0;
        snd1_67D818.field_0_object_type = 9;
    }
    else if (cheat_str_hash == 0x41611)
    { // MADEMAN Max respect from all gangs
        gCheatAllGangMaxRespect_67D587 = gCheatAllGangMaxRespect_67D587 == 0;
        snd1_67D818.field_0_object_type = 9;
    }
    else if (cheat_str_hash == 0x46BE8)
    { // LASVEGAS Only Elvis peds spawn
        gCheatOnlyElvisPeds_67D4ED = gCheatOnlyElvisPeds_67D4ED == 0;
        snd1_67D818.field_0_object_type = 9;
    }
    else if (cheat_str_hash == 0x36F62)
    { // NEKKID All peds are naked
        gCheatNakedPeds_67D5E8 = gCheatNakedPeds_67D5E8 == 0;
        snd1_67D818.field_0_object_type = 9;
    }
    else if (cheat_str_hash == 0x4657B)
    { // EATSOUP Free shopping
        bDo_free_shopping_67D6CD = bDo_free_shopping_67D6CD == 0;
        snd1_67D818.field_0_object_type = 9;
    }
    else if (cheat_str_hash == 0x4639F)
    { // DAVEMOON Basic set of weapons and max ammo
        gCheatGetBasicWeaponsMaxAmmo_67D545 = gCheatGetBasicWeaponsMaxAmmo_67D545 == 0;
        snd1_67D818.field_0_object_type = 9;
    }
    else if (cheat_str_hash == 0x33A69)
    { // CUTIE1 99 lives
        gCheatGet99Lives_67D4F1 = gCheatGet99Lives_67D4F1 == 0;
        snd1_67D818.field_0_object_type = 9;
    }
    else if (cheat_str_hash == 0x47AF1)
    { // ARSESTAR Keep weapons after death
        bKeep_weapons_after_death_67D54D = bKeep_weapons_after_death_67D54D == 0;
        snd1_67D818.field_0_object_type = 9;
    }
    else if (cheat_str_hash == 0x49771)
    { // GODOFGTA All weapons
        bGet_all_weapons_67D684 = bGet_all_weapons_67D684 == 0;
        snd1_67D818.field_0_object_type = 9;
    }
    else if (cheat_str_hash == 0x478FB)
    { // RSJABBER Invincibility
        bDo_invulnerable_67D4CB = bDo_invulnerable_67D4CB == 0;
        snd1_67D818.field_0_object_type = 9;
    }
    else if (cheat_str_hash == 0x44D2F)
    { // DANISGOD Player get points
        gCheatGetPlayerPoints_67D4C8 = gCheatGetPlayerPoints_67D4C8 == 0;
        snd1_67D818.field_0_object_type = 9;
    }
    else if (cheat_str_hash == 0x478A9)
    { // COCKTART Skip exploding scores
        bExplodingScoresOff_67D4FB = bExplodingScoresOff_67D4FB == 0;
        snd1_67D818.field_0_object_type = 9;
    }
    else if (cheat_str_hash == 0x45EC2)
    { // FLAMEON Flame Thrower with infinite ammo
        gCheatUnlimitedFlameThrower_67D6CC = gCheatUnlimitedFlameThrower_67D6CC == 0;
        snd1_67D818.field_0_object_type = 9;
    }
    else if (cheat_str_hash == 0x45118)
    { // ??
        gCheatUnknown_67D4F6 = gCheatUnknown_67D4F6 == 0;
        snd1_67D818.field_0_object_type = 9;
    }
    else if (cheat_str_hash == 0x4672D)
    { // IAMDAVEJ Get $10,000,000
        gCheatGet10MillionMoney_67D6CE = gCheatGet10MillionMoney_67D6CE == 0;
        snd1_67D818.field_0_object_type = 9;
    }
    else if (cheat_str_hash == 0x4A98B)
    { // SEGARULZ 10x multiplier
        gCheat10xMultiplier_67D589 = gCheat10xMultiplier_67D589 == 0;
        snd1_67D818.field_0_object_type = 9;
    }
    else if (cheat_str_hash == 0x45B2C)
    { // UKGAMER Unlock three main levels
        gCheatUnlockThreeLevels_67D6CB = gCheatUnlockThreeLevels_67D6CB == 0;
        gJolly_poitras_0x2BC0_6FEAC0->UnlockAllStages_56BC40();
        UpdateMenuForCurrPlayer_4B42E0();
        snd1_67D818.field_0_object_type = 9;
    }
    else if (cheat_str_hash == 0x49C76)
    { // GINGERRR Unlock levels one and two
        gCheatUnlockLevelsOneAndTwo_67D584 = gCheatUnlockLevelsOneAndTwo_67D584 == 0;
        gJolly_poitras_0x2BC0_6FEAC0->UnlockStage_56BBD0(1u, 0);
        UpdateMenuForCurrPlayer_4B42E0();
        snd1_67D818.field_0_object_type = 9;
    }
    else if (cheat_str_hash == 0x5073D)
    { // TUMYFROG unlock all levels
        gCheatUnlockAllLevels_67D538 = gCheatUnlockAllLevels_67D538 == 0;
        gJolly_poitras_0x2BC0_6FEAC0->UnlockAllStages_56BC40();
        gJolly_poitras_0x2BC0_6FEAC0->UnlockStage_56BBD0(2u, 2u);
        UpdateMenuForCurrPlayer_4B42E0();
        snd1_67D818.field_0_object_type = 9;
    }
    else if (cheat_str_hash == 0x4D5C4)
    { // SCHURULZ Unlimited double damage
        gCheatUnlimitedDoubleDamage_67D57C = gCheatUnlimitedDoubleDamage_67D57C == 0;
        snd1_67D818.field_0_object_type = 9;
    }
    else if (cheat_str_hash == 0x4B28C)
    { // HUNSRUS Invisibility
        gCheatInvisibility_67D539 = gCheatInvisibility_67D539 == 0;
        snd1_67D818.field_0_object_type = 9;
    }
    else if (cheat_str_hash == 0x45AEF)
    { // FISHFLAP All cars are mini cars
        gCheatMiniCars_67D6C8 = gCheatMiniCars_67D6C8 == 0;
        snd1_67D818.field_0_object_type = 9;
    }
}

MATCH_FUNC(0x4B4280)
void Frontend::LoadCurrPlayerName_4B4280()
{
    wcsncpy(field_C9A0_curr_plyr_name,
            gJolly_poitras_0x2BC0_6FEAC0
                ->field_26A0_plyr_stats[field_136_menu_pages_array[1].field_4_options_array[0].field_6E_horizontal_selected_idx]
                .field_90_strPlayerName,
            9u);
}

MATCH_FUNC(0x4B8530)
void Frontend::StripPasswordToCurrLength_4B8530()
{
    u16 total = field_C9CA_password_length;
    for (u16 i = total; i < 9; i++)
    {
        field_C9B8_password[i] = 0;
    }
}

MATCH_FUNC(0x4B8560)
void Frontend::CheckPassword_4B8560()
{
    if (!wcscmp(field_C9B8_password, L"WFUSDFCF")) // french bonus mission unlocks?
    {
        if (intro_bik_exists_4B5FF0() && gRegistry_6FF968.Get_Screen_Setting_5870D0("do_play_movie", 1) == 1)
        {
            ChangeMenuPage_4B3170(MENUPAGE_PLAY_INTRO);
        }
        else
        {
            ChangeMenuPage_4B3170(MENUPAGE_START_MENU);
        }
    }
    else
    {
        field_110_state = 5;
        field_C9CA_password_length = 0;

        StripPasswordToCurrLength_4B8530();

        field_C9B3_key_held = 1;
        field_C9B4_last_key = DIK_RETURN;
        field_C9B6_key_repeat_timer = 5;

        wcsncpy(field_136_menu_pages_array[14].field_518_elements_array[0].field_6_element_name_str,
                gText_0x14_704DFC->Find_5B5F90("fr_rnt1"),
                0x32u);
        wcsncpy(field_136_menu_pages_array[14].field_518_elements_array[1].field_6_element_name_str,
                gText_0x14_704DFC->Find_5B5F90("fr_rnt2"),
                0x32u);
        wcsncpy(field_136_menu_pages_array[14].field_518_elements_array[2].field_6_element_name_str,
                gText_0x14_704DFC->Find_5B5F90("fr_rnt3"),
                0x32u);

        field_C9CB_wrong_password_shown = 1;
    }
}

MATCH_FUNC(0x4B8020)
void Frontend::ContinueToNextStage_4B8020()
{
    player_stats_0xA4* pClarke = GetCurrPlayerStats_4B43E0();
    u8 main_stage_idx = gLucid_hamilton_67E8E0.GetMainStageIdx_4C5980();

    if (AreAllStagesUnlocked_4B7FB0()) // Everything unlocked, including all bonus stages
    {
        ChangeMenuPage_4B3170(MENUPAGE_GAME_COMPLETE);
    }
    else if (main_stage_idx == field_1EB50_num_main_stages - 1) // Not all Bonus stages unlocked but finished last main stage
    {
        ChangeMenuPage_4B3170(MENUPAGE_NICE_TRY);
    }
    else
    {
        // Load next stage. Can be a Bonus stage or a Main stage
        // note: reg swap + push swap due to redundant local
        u8 substage_idx = 3;
        while (!pClarke->field_0_plyr_stage_stats[main_stage_idx][substage_idx].field_0_is_stage_unlocked || substage_idx >= field_1EB51_num_bonus_stages[main_stage_idx])
        {
            substage_idx--;
        }

        LoadMapFilenames_4B4D00(main_stage_idx, substage_idx);
        gLucid_hamilton_67E8E0.SetStartedFromPlayBonusMenu_4C5AD0(0);
        field_EE08_menu_screen = RedBar_16;
        field_110_state = FrontendState::Booting_Map_2;
    }
}

MATCH_FUNC(0x4B7D60)
void Frontend::DrawBonusRating_4B7D60()
{
    u16 font_type = field_11E;
    char_type text_id[12];
    wchar_t text[256];
    GetLineSpacingFromFontType_5D7700_inlined(font_type);
    u16 rating_idx = gLucid_hamilton_67E8E0.GetBonusRatingTextIdx_4C5AC0();
    _itoa(rating_idx, text_id, 10);
    if (rating_idx)
    {
        wchar_t* pRatingStr = gText_0x14_704DFC->Find_5B5F90(text_id);
        text_0x14::InsertLineBreaksAndGetNumLines_5B5BC0(text, pRatingStr, 560, font_type);
        DrawText_4B87A0(text, 40, 270, font_type, 1);
    }
}

// TODO: the text keys are guesses, only code is compared
MATCH_FUNC(0x4B7E10)
EXPORT int Frontend::sub_4B7E10(u8 str_id_idx, u16 text_xpos, u16 text_ypos, u16 fontType, s32 palette)
{
    switch (str_id_idx)
    {
        case 0:
            swprintf(tmpBuff_67BD9C, gText_0x14_704DFC->Find_5B5F90("fekey0"));
            break;
        case 1:
            swprintf(tmpBuff_67BD9C, gText_0x14_704DFC->Find_5B5F90("fekey1"));
            break;
        case 2:
            swprintf(tmpBuff_67BD9C, gText_0x14_704DFC->Find_5B5F90("fekey2"));
            break;
        case 3:
            swprintf(tmpBuff_67BD9C, gText_0x14_704DFC->Find_5B5F90("fekey3"));
            break;
        case 4:
            swprintf(tmpBuff_67BD9C, gText_0x14_704DFC->Find_5B5F90("fekey4"));
            break;
        case 5:
            swprintf(tmpBuff_67BD9C, gText_0x14_704DFC->Find_5B5F90("fekey5"));
            break;
        case 6:
            swprintf(tmpBuff_67BD9C, gText_0x14_704DFC->Find_5B5F90("fekey6"));
            break;
        case 7:
            swprintf(tmpBuff_67BD9C, gText_0x14_704DFC->Find_5B5F90("fekey7"));
            break;
        case 8:
            swprintf(gTmpWideStr_67C7D8, gText_0x14_704DFC->Find_5B5F90("fekey8"));
            swprintf(tmpBuff_67BD9C, L": %s", gTmpWideStr_67C7D8);
            break;
        case 9:
            swprintf(gTmpWideStr_67C7D8, gText_0x14_704DFC->Find_5B5F90("fekey9"));
            swprintf(tmpBuff_67BD9C, L": %s", gTmpWideStr_67C7D8);
            break;
        case 10:
            swprintf(gTmpWideStr_67C7D8, gText_0x14_704DFC->Find_5B5F90("fekey10"));
            swprintf(tmpBuff_67BD9C, L": %s", gTmpWideStr_67C7D8);
            break;
        case 11:
            swprintf(gTmpWideStr_67C7D8, gText_0x14_704DFC->Find_5B5F90("fekey11"));
            swprintf(tmpBuff_67BD9C, L": %s", gTmpWideStr_67C7D8);
            break;
        default:
            FatalError_4A38C0(Gta2Error::InvalidCase, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 8148);
            break;
    }

    if ((u16)palette == 0xFFFF)
    {
        DrawText_4B87A0(tmpBuff_67BD9C, text_xpos, text_ypos, fontType, 1);
    }
    else
    {
        DrawText_5D8A10(tmpBuff_67BD9C, text_xpos, text_ypos, fontType, 1, 8, palette, 0, 0);
    }
    return GetMaxTextWidth_5D8990(tmpBuff_67BD9C, fontType);
}

MATCH_FUNC(0x4B7FB0)
char_type Frontend::AreAllStagesUnlocked_4B7FB0()
{
    player_stats_0xA4* pPlayerSlot = GetCurrPlayerStats_4B43E0();
    u16 main_stage_idx = 0;
    // note: two separated while's interlaced by a backwards goto may be actually two nested while's
    while (main_stage_idx < field_1EB50_num_main_stages)
    {
        u16 bonus_stage_idx = 0;
        while (bonus_stage_idx < field_1EB51_num_bonus_stages[main_stage_idx])
        {
            if (!pPlayerSlot->field_0_plyr_stage_stats[main_stage_idx][bonus_stage_idx].field_0_is_stage_unlocked)
            {
                return false;
            }
            bonus_stage_idx++;
        }
        main_stage_idx++;
    }
    return true;
}

MATCH_FUNC(0x4B4D00)
void Frontend::LoadMapFilenames_4B4D00(u8 mainBlockIdx, u8 bonusBlockIdx)
{
    char fullPath[256]; // [esp+10h] [ebp-400h] BYREF
    char debugStr[256]; // [esp+110h] [ebp-300h] BYREF
    char mapName[256]; // [esp+210h] [ebp-200h] BYREF
    char styName[256]; // [esp+310h] [ebp-100h] BYREF

    LoadStringsFromStage_4B4C60(mainBlockIdx, bonusBlockIdx, debugStr, mapName, styName);
    gLucid_hamilton_67E8E0.DebugStr_4C58D0("");
    strcpy(fullPath, "data\\");
    strcat(fullPath, debugStr);
    gLucid_hamilton_67E8E0.SetMapName_4C5870(fullPath);
    strcpy(fullPath, "data\\");
    strcat(fullPath, mapName);
    gLucid_hamilton_67E8E0.SetStyleName_4C5890(fullPath);
    strcpy(fullPath, "data\\");
    strcat(fullPath, styName);
    gLucid_hamilton_67E8E0.SetScriptName_4C58B0(fullPath);
    if (!bonusBlockIdx)
    {
        gLucid_hamilton_67E8E0.SetMainStageIdx_4C58F0(mainBlockIdx);
        gLucid_hamilton_67E8E0.SetBonusStage_4C5910(0);
    }
    else
    {
        gLucid_hamilton_67E8E0.SetStage_4C5900(gLucid_hamilton_67E8E0.EncodeStage_453A40(mainBlockIdx, bonusBlockIdx));
        gLucid_hamilton_67E8E0.SetBonusStage_4C5910(1);
    }
}

MATCH_FUNC(0x4AD0D0)
void Frontend::DrawLoading_4AD0D0()
{
    const u16 x = GetCenteredXPos_4B0190(gText_0x14_704DFC->Find_5B5F90("loading"), -1, 320);

    DrawText_4B87A0(gText_0x14_704DFC->Find_5B5F90("loading"), x, 260, field_11C_normal_font, 1);
}

MATCH_FUNC(0x4ADDE0)
void Frontend::DrawDeletePlayerDialog_4ADDE0()
{
    // A one-case switch: the original zero-extends the type and tests it with `dec`.
    switch (field_EE0C_dialog_type)
    {
        case 1:
        {
            u16 ypos = gText_0x14_704DFC->field_10_lang_code != 'j' ? 12 : 16;
            DrawText_4B87A0(gText_0x14_704DFC->Find_5B5F90("clrchar"), (u16)275, ypos, field_126, 1);
            break;
        }
    }

    if (field_EE0A_dialog_cursor_ypos == 190)
    {
        DrawText_4B87A0(gText_0x14_704DFC->Find_5B5F90("sure"), 300, 190, field_120_selected_font, 1);
    }
    else
    {
        DrawText_4B87A0(gText_0x14_704DFC->Find_5B5F90("sure"), 300, 190, field_11C_normal_font, 1);
    }

    if (field_EE0A_dialog_cursor_ypos == 210)
    {
        DrawText_4B87A0(gText_0x14_704DFC->Find_5B5F90("yes"), 300, 210, field_120_selected_font, 1);
    }
    else
    {
        DrawText_4B87A0(gText_0x14_704DFC->Find_5B5F90("yes"), 300, 210, field_11C_normal_font, 1);
    }

    if (field_EE0A_dialog_cursor_ypos == 230)
    {
        DrawText_4B87A0(gText_0x14_704DFC->Find_5B5F90("no"), 300, 230, field_120_selected_font, 1);
    }
    else
    {
        DrawText_4B87A0(gText_0x14_704DFC->Find_5B5F90("no"), 300, 230, field_11C_normal_font, 1);
    }
}

MATCH_FUNC(0x4ADF50)
void Frontend::DrawCurrentState_4ADF50()
{
    switch (field_110_state)
    {
        case 4:
            DrawDeletePlayerDialog_4ADDE0();
            break;

        case 1:
        case 3:
        case 5:
            if (field_132_f136_idx == MENUPAGE_CREDITS)
            {
                DrawCredits_4B7AE0();
            }
            else
            {
                DrawMenu_4AD140();
            }
            break;

        case 2:
            DrawLoading_4AD0D0();
            break;

        default:
            FatalError_4A38C0(Gta2Error::InvalidCase, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 1217, field_110_state);
            break;
    }
}

MATCH_FUNC(0x5D7DC0)
EXPORT void __cdecl FreeSurface_5D7DC0()
{
    pVid_FreeSurface(gVidSys_7071D0);
}

// 16.16 step along a line, 0 for a point
static inline s32 StepFor_5D7DD0(s32 delta, s32 count)
{
    if (count == 0)
    {
        return count;
    }
    return (delta << 16) / count;
}

// Debug line, one gbh_Plot per pixel along the longer axis. x2 and y2 become the deltas, y2 then the y step
MATCH_FUNC(0x5D7DD0)
EXPORT void __stdcall DrawDebugLine_5D7DD0(s32 x1, s32 y1, s32 x2, s32 y2, u16 colour)
{
    x2 -= x1;
    y2 -= y1;
    s32 step_x;
    s32 count;
    s32 abs_dx = abs(x2);
    s32 abs_dy = abs(y2);
    if (abs_dx > abs_dy)
    {
        step_x = x2 > 0 ? 0x10000 : -0x10000;
        y2 = StepFor_5D7DD0(y2, abs_dx);
        count = abs_dx;
    }
    else
    {
        y2 = y2 > 0 ? 0x10000 : -0x10000;
        step_x = StepFor_5D7DD0(x2, abs_dy);
        count = abs_dy;
    }

    x1 <<= 16;
    y1 <<= 16;
    do
    {
        pgbh_Plot((f32)(x1 >> 16), (f32)(y1 >> 16), 0, colour);
        x1 += step_x;
        y1 += y2;
        count--;
    } while (count > 0);
}

MATCH_FUNC(0x4ADFB0)
void Frontend::Render_4ADFB0()
{
    MakeScreenTableAndSetWindow_5D7D30();

    pgbh_BeginScene();
    DrawBackground_4B6E10();
    DrawCurrentState_4ADF50();
    pgbh_EndScene();

    FreeSurface_5D7DC0();

    pVid_FlipBuffers(gVidSys_7071D0);

    pVid_ClearScreen(gVidSys_7071D0, 0, 0, 0, 0, 0, gVidSys_7071D0->field_48_rect_right, gVidSys_7071D0->field_4C_rect_bottom);
}

// https://decomp.me/scratch/IOmk7
// TODO: stop the tail merge... somehow
WIP_FUNC(0x4B6E10)
void Frontend::DrawBackground_4B6E10()
{
    WIP_IMPLEMENTED;
    // todo
    BYTE tga_idx; // [esp+50h] [ebp-8h] BYREF
    BYTE not_used; // [esp+54h] [ebp-4h] BYREF

    if (field_EE08_menu_screen == GameOver_13 || field_EE08_menu_screen == RedBar_16 || field_EE08_menu_screen == BlueBar_14 || field_EE08_menu_screen == Loading_15 ||
        field_EE08_menu_screen == HiScoresDisplay_12 || field_EE08_menu_screen == Credits_17)
    {
        GetTgaIdxsForMenuScreen_4B6B00(field_EE08_menu_screen, &tga_idx, &not_used);
        s32 blitRet = pgbh_BlitImage(tgaArray_61F0C8[tga_idx].field_84_img, 0, 0, 640, 480, 0, 0);
        if (blitRet == -10)
        {
            // need to reload image
            Load_tga_4B6520(tga_idx);
            pgbh_BlitImage(tgaArray_61F0C8[tga_idx].field_84_img, 0, 0, 640, 480, 0, 0);
        }
    }
    else
    {
        GetTgaIdxsForMenuScreen_4B6B00(field_EE08_menu_screen, &tga_idx, &not_used);

        // Left side
        s32 blitRet = pgbh_BlitImage(tgaArray_61F0C8[tga_idx].field_84_img, 0, 0, 278, 480, 0, 0);
        if (blitRet == -10)
        {
            Load_tga_4B6520(tga_idx);
            blitRet = pgbh_BlitImage(tgaArray_61F0C8[tga_idx].field_84_img, 0, 0, 278, 480, 0, 0);
        }

        // Right side
        if (blitRet == 0)
        {
            blitRet = pgbh_BlitImage(tgaArray_61F0C8[not_used].field_84_img, 0, 0, 362, 480, 278, 0);
            if (blitRet == -10)
            {
                Load_tga_4B6520(not_used);
                pgbh_BlitImage(tgaArray_61F0C8[not_used].field_84_img, 0, 0, 362, 480, 278, 0);
            }
        }
    }
}

MATCH_FUNC(0x4B6B00)
void Frontend::GetTgaIdxsForMenuScreen_4B6B00(u8 a1, BYTE* pTgaIdx, BYTE* a3)
{
    switch (a1)
    {
        case Options_0:
            *pTgaIdx = 1;
            *a3 = 0;
            break;

        case Play_1:
            *pTgaIdx = 2;
            *a3 = 0;
            break;

        case Quit_2:
            *pTgaIdx = 3;
            *a3 = 0;
            break;

        case BonusAC_3:
            *pTgaIdx = 5;
            *a3 = 4;
            break;

        case BonusDF_4:
            *pTgaIdx = 6;
            *a3 = 4;
            break;

        case BonusGI_5:
            *pTgaIdx = 7;
            *a3 = 4;
            break;

        case ViewHiScore_6:
            *pTgaIdx = 8;
            *a3 = 4;
            break;

        case PlayArea1_7:
            *pTgaIdx = 9;
            *a3 = 4;
            break;

        case PlayArea2_8:
            *pTgaIdx = 10;
            *a3 = 4;
            break;

        case PlayArea3_9:
            *pTgaIdx = 11;
            *a3 = 4;
            break;

        case EnterPlayerName_10:
            *pTgaIdx = 12;
            *a3 = 4;
            break;

        case ResumeLoadSave_11:
            *pTgaIdx = 13;
            *a3 = 4;
            break;

        case HiScoresDisplay_12:
            *pTgaIdx = 15;
            *a3 = 0;
            break;

        case GameOver_13:
            *pTgaIdx = 19;
            *a3 = 0;
            break;

        case RedBar_16:
            *pTgaIdx = 17;
            *a3 = 0;
            break;

        case BlueBar_14:
            *pTgaIdx = 18;
            *a3 = 0;
            break;

        case Loading_15:
            *pTgaIdx = 16;
            *a3 = 0;
            break;

        case Credits_17:
            *pTgaIdx = 22;
            *a3 = 0;
            break;

        default:
            return;
    }
}

MATCH_FUNC(0x4B6520)
void Frontend::Load_tga_4B6520(u16 idx)
{
    Error_SetName_4A0770(tgaArray_61F0C8[idx].field_0_tga_name);

    FILE* hFile = crt::fopen(tgaArray_61F0C8[idx].field_0_tga_name, "rb");
    if (!hFile)
    {
        FatalError_4A38C0(Gta2Error::FreeloaderEpisodeUnknown, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 6516);
    }

    void* pAlloc = malloc(tgaArray_61F0C8[idx].field_80_len);
    if (!pAlloc)
    {
        FatalError_4A38C0(Gta2Error::TargaMemoryAllocationError, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 6523);
    }

    if (crt::fread(pAlloc, 1u, tgaArray_61F0C8[idx].field_80_len, hFile) != tgaArray_61F0C8[idx].field_80_len)
    {
        FatalError_4A38C0(Gta2Error::InvalidBackgroundImageSize, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 6529);
    }

    tgaArray_61F0C8[idx].field_84_img = pgbh_LoadImage((SImage*)pAlloc);

    crt::fclose(hFile);
    free(pAlloc);
}

// https://decomp.me/scratch/MuqZh
WIP_FUNC(0x4AF2A0)
Frontend::Frontend()
{
    WIP_IMPLEMENTED;
    SetField10D_453A30(1);

    gText_0x14_704DFC = new text_0x14();
    if (!gText_0x14_704DFC)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 2288);
    }

    gGtx_0x106C_703DD4 = new gtx_0x106C();
    if (!gGtx_0x106C_703DD4)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 2290);
    }

    gSharp_pare_0x15D8_705064 = new sharp_pare_0x15D8();
    if (!gSharp_pare_0x15D8_705064)
    {
        FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 2292);
    }

    if (gText_0x14_704DFC->field_10_lang_code == 'j')
    {
        // pmagical_germain_0x8EC = pmagical_germain_0x8EC_mem ? magical_germain_0x8EC::ctor_4D2C80(pmagical_germain_0x8EC_mem) : 0;
        gMagical_germain_0x8EC_6F5168 = new magical_germain_0x8EC();
        if (!gMagical_germain_0x8EC_6F5168)
        {
            FatalError_4A38C0(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 2297);
        }
    }
    InitKeyBoardDevice_4AFBE0();

    gText_0x14_704DFC->Load_5B5E90();
    gGtx_0x106C_703DD4->LoadSty_5AB750("data\\fstyle.sty");

    gSharp_pare_0x15D8_705064->LoadStyleTextures_5B9350();

    ConvertColourBanks_5D7CB0();

    pgbh_SetAmbient(1.0);

    if (gMagical_germain_0x8EC_6F5168)
    {
        gMagical_germain_0x8EC_6F5168->InitGlyphCaches_4D2B40();
    }

    field_110_state = 1;
    field_114_cursor_blink = 0;
    field_118_cursor_blink_timer = 0;
    field_C9D5_up_key_down = 0;
    field_C9D6_down_key_down = 0;
    field_C9D3_left_key_down = 0;
    field_C9D4_right_key_down = 0;
    field_C9D7_return_key_down = 0;
    field_C9D8_escape_key_down = 0;
    field_C9D9_delete_key_down = 0;
    field_10C_bKeyboardAcquired = 0;
    field_108_winmain_next_state = Run_Frontend_2;
    field_C9E1_bCheatsEnabled = 0;

    SetFontTypes_4AF0E0();

    field_C9DC_next_frame_time = timeGetTime();
    field_C9E0_updates_since_render = 0;
    field_132_f136_idx = 0;
    field_C9E4_last_input_time = 0;

    SetupMenuStringsOptionsElements_4B0220();

    field_C9B2_curr_plyr_name_length = 0;
    field_C9B3_key_held = 1;
    field_C9B4_last_key = 256;
    field_C9B6_key_repeat_timer = 5;

    //memset(&field_C9A0_curr_plyr_name, 0, sizeof(field_C9A0_curr_plyr_name));

    *(u32*)field_C9A0_curr_plyr_name = 0;
    *(u32*)&field_C9A0_curr_plyr_name[2] = 0;
    *(u32*)&field_C9A0_curr_plyr_name[4] = 0;
    *(u32*)&field_C9A0_curr_plyr_name[6] = 0;
    field_C9A0_curr_plyr_name[8] = 0;

    memset(&field_C9B8_password, 0, sizeof(field_C9B8_password));
    /*
    *(_DWORD *)field_C9B8_password = 0;
    *(_DWORD *)&field_C9B8_password[2] = 0;
    *(_DWORD *)&field_C9B8_password[4] = 0;
    *(_DWORD *)&field_C9B8_password[6] = 0;
    *(_WORD *)&field_C9C8 = 0;
    */
    field_C9CA_password_length = 0;
    field_C9CB_wrong_password_shown = 0;
    field_1EB50_num_main_stages = 0;

    field_1EB51_num_bonus_stages[0] = 0; //  lobyte of u16?
    field_1EB51_num_bonus_stages[1] = 0; //  hibyte of u16?
    field_1EB51_num_bonus_stages[2] = 0;

    GetMainAndBonusStagesFromSeqFile_4B4440();
    LoadPlySlotSvgs_4B53C0();

    field_EE08_menu_screen = Play_1;

    Load_tgas_4B66B0();

    field_EE0D_hiscore_table_idx = 0;
    field_EE0A_dialog_cursor_ypos = 190;
    field_EE0C_dialog_type = 0;
    field_1EB30_credits_scroll_timer = 0;
    field_1EB34_credits_ypos = dword_67D930;
    field_1EB38_credits_line_idx = 0;
    field_1EB4A_has_prev_player_slot = 0;
    field_1EB4B_has_next_player_slot = 0;
    field_1EB4C_has_prev_main_stage = 0;
    field_1EB4D_has_next_main_stage = 0;
    field_1EB4E_has_prev_bonus_stage = 0;
    field_1EB4F_has_next_bonus_stage = 0;

    for (u8 i = 0; i < 8; i++)
    {
        field_1EB3A_selected_main_stage[i] = -1;
        field_1EB42_selected_bonus_stage[i] = -1;
    }
}

MATCH_FUNC(0x4AF970)
Frontend::~Frontend()
{
    FreeKeyBoardDevice_4AFD00();

    if (gSharp_pare_0x15D8_705064)
    {
        GTA2_DELETE_AND_NULL(gSharp_pare_0x15D8_705064);
    }

    if (gGtx_0x106C_703DD4)
    {
        GTA2_DELETE_AND_NULL(gGtx_0x106C_703DD4);
    }

    if (gText_0x14_704DFC)
    {
        GTA2_DELETE_AND_NULL(gText_0x14_704DFC);
    }

    if (gMagical_germain_0x8EC_6F5168)
    {
        GTA2_DELETE_AND_NULL(gMagical_germain_0x8EC_6F5168);
    }

    FreeImageTable_4B6750();
}

MATCH_FUNC(0x4AFD70)
void Frontend::AcquireKeyBoard_4AFD70()
{
    if (!field_4_pKeyboardDevice || field_4_pKeyboardDevice->Acquire() < 0)
    {
        field_10C_bKeyboardAcquired = 1;
    }
}

MATCH_FUNC(0x4AFD00)
void Frontend::FreeKeyBoardDevice_4AFD00()
{
    if (field_4_pKeyboardDevice)
    {
        field_4_pKeyboardDevice->Unacquire();
        field_4_pKeyboardDevice->Release();
        field_4_pKeyboardDevice = 0;
    }
}

MATCH_FUNC(0x4B6750)
void Frontend::FreeImageTable_4B6750()
{
    pgbh_FreeImageTable();
}

MATCH_FUNC(0x4AFDD0)
char_type Frontend::KeyBoard_GetKeyStates_4AFDD0()
{
    HRESULT hr = field_4_pKeyboardDevice->GetDeviceState(256, field_8_keys);
    if (FAILED(hr))
    {
        if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED)
        {
            if (FAILED(field_4_pKeyboardDevice->Acquire()))
            {
                return 0;
            }

            field_4_pKeyboardDevice->GetDeviceState(256, field_8_keys);
        }
        return 0;
    }

    return 1;
}

MATCH_FUNC(0x4AFBE0)
void Frontend::InitKeyBoardDevice_4AFBE0()
{
    field_0_pDInput = gpDInput_67B804;
    field_4_pKeyboardDevice = 0;

    if (field_0_pDInput->CreateDevice(GUID_SysKeyboard, &field_4_pKeyboardDevice, 0) < 0)
    {
        FatalError_4A38C0(Gta2Error::DirectInputCreateFail, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 2469);
    }

    if (field_4_pKeyboardDevice->SetDataFormat(&gKeyboardDataFormat_601A54) < 0)
    {
        FatalError_4A38C0(Gta2Error::DirectInputSetDataFormatFail, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 2480);
    }

    if (field_4_pKeyboardDevice->SetCooperativeLevel(gHwnd_707F04, 6) < 0)
    {
        FatalError_4A38C0(Gta2Error::DirectInputSetCooperativeLevelFail, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 2487);
    }

    AcquireKeyBoard_4AFD70();
}

MATCH_FUNC(0x4AF0E0)
void Frontend::SetFontTypes_4AF0E0()
{
    if (gText_0x14_704DFC->field_10_lang_code == 'j')
    {
        this->field_11C_normal_font = 101;
        this->field_11E = 101;
        this->field_120_selected_font = 102;
        this->field_122 = 101;
        this->field_124_font_type = 101;
        this->field_126 = 101;
        this->field_128 = 101;
        this->field_12A_score_font = 101;
        this->field_12C = 106;
        this->field_12E = 102;
        this->field_130 = 201;
    }
    else
    {
        this->field_11C_normal_font = word_703D0C;
        this->field_11E = word_703D0C;
        this->field_120_selected_font = word_703C16;
        this->field_122 = word_703C8C;
        this->field_124_font_type = font_type_703C14;
        this->field_126 = word_703C3C;
        this->field_128 = word_703C8A;
        this->field_12A_score_font = word_703BE2;
        this->field_12C = word_703B88;
        this->field_12E = word_703DAC;
        this->field_130 = word_703B9C;
    }
}

WIP_FUNC(0x4B0220)
void Frontend::SetupMenuStringsOptionsElements_4B0220()
{
    WIP_IMPLEMENTED;
    s16 v30; // ax

    // local_4 = (-(ushort)(cVar1 != 'j') & 0xfffc) + 0x10;

    s32 v2 = gText_0x14_704DFC->field_10_lang_code != 'j' ? 12 : 16;
    field_134 = 16;

    field_136_menu_pages_array[0].field_0_number_of_options = 3;
    field_136_menu_pages_array[0].field_4_options_array[0].field_0_option_type = STRING_TEXT_1; // ebx
    field_136_menu_pages_array[0].field_4_options_array[0].field_2_x_pos = 300; // edi
    field_136_menu_pages_array[0].field_4_options_array[0].field_4_y_pos = 250;
    wcsncpy(field_136_menu_pages_array[0].field_4_options_array[0].field_6_option_name_str, gText_0x14_704DFC->Find_5B5F90("play"), 0x32u);
    field_136_menu_pages_array[0].field_4_options_array[0].field_80_menu_page_target = 1;
    field_136_menu_pages_array[0].field_4_options_array[1].field_0_option_type = STRING_TEXT_1;
    field_136_menu_pages_array[0].field_4_options_array[1].field_2_x_pos = 300;
    field_136_menu_pages_array[0].field_4_options_array[1].field_4_y_pos = 270;
    wcsncpy(field_136_menu_pages_array[0].field_4_options_array[1].field_6_option_name_str,
            gText_0x14_704DFC->Find_5B5F90("options"),
            0x32u);
    field_136_menu_pages_array[0].field_4_options_array[1].field_80_menu_page_target = 257;
    field_136_menu_pages_array[0].field_4_options_array[2].field_0_option_type = STRING_TEXT_1;
    field_136_menu_pages_array[0].field_4_options_array[2].field_2_x_pos = 300;
    field_136_menu_pages_array[0].field_4_options_array[2].field_4_y_pos = 290;
    wcsncpy(field_136_menu_pages_array[0].field_4_options_array[2].field_6_option_name_str, gText_0x14_704DFC->Find_5B5F90("quit"), 0x32u);
    field_136_menu_pages_array[0].field_4_options_array[2].field_80_menu_page_target = 9;
    field_136_menu_pages_array[0].field_B8A[0].field_0 = 280;
    field_136_menu_pages_array[0].field_B8A[0].field_2 = 258;
    field_136_menu_pages_array[0].field_B8A[1].field_0 = 280;
    field_136_menu_pages_array[0].field_B8A[1].field_2 = 278;
    field_136_menu_pages_array[0].field_B8A[2].field_0 = 280;
    field_136_menu_pages_array[0].field_B8A[2].field_2 = 298;
    field_136_menu_pages_array[0].field_BC6_current_option_idx = 0;
    field_136_menu_pages_array[0].field_BC8_default_option_idx = 0;
    field_136_menu_pages_array[1].field_0_number_of_options = 5;
    field_136_menu_pages_array[1].field_4_options_array[0].field_0_option_type = STRING_TEXT_2;
    field_136_menu_pages_array[1].field_4_options_array[0].field_2_x_pos = 300;
    field_136_menu_pages_array[1].field_4_options_array[0].field_4_y_pos = 210;
    wcsncpy(field_136_menu_pages_array[1].field_4_options_array[0].field_6_option_name_str,
            gText_0x14_704DFC->Find_5B5F90("charctr"),
            0x32u);
    field_136_menu_pages_array[1].field_4_options_array[0].field_6E_horizontal_selected_idx = 0;
    field_136_menu_pages_array[1].field_4_options_array[0].field_70 = 0;
    field_136_menu_pages_array[1].field_4_options_array[0].field_7E_horizontal_max_idx = 7;

    u16 v77 = 0;
    do
    {
        field_136_menu_pages_array[1].field_4_options_array[0].field_72_horizontal_idx_enabled[v77++] = 1;
    } while (v77 <= field_136_menu_pages_array[1].field_4_options_array[0].field_7E_horizontal_max_idx);

    field_136_menu_pages_array[1].field_4_options_array[1].field_0_option_type = STRING_TEXT_1;
    field_136_menu_pages_array[1].field_4_options_array[1].field_2_x_pos = 300;
    field_136_menu_pages_array[1].field_4_options_array[1].field_4_y_pos = 230;
    wcsncpy(field_136_menu_pages_array[1].field_4_options_array[1].field_6_option_name_str,
            gText_0x14_704DFC->Find_5B5F90("savepos"),
            0x32u);
    field_136_menu_pages_array[1].field_4_options_array[1].field_80_menu_page_target = 260;
    field_136_menu_pages_array[1].field_4_options_array[2].field_0_option_type = STRING_TEXT_1;
    field_136_menu_pages_array[1].field_4_options_array[2].field_2_x_pos = 300;
    field_136_menu_pages_array[1].field_4_options_array[2].field_4_y_pos = 250;
    wcsncpy(field_136_menu_pages_array[1].field_4_options_array[2].field_6_option_name_str,
            gText_0x14_704DFC->Find_5B5F90("hi_scre"),
            0x32u);
    field_136_menu_pages_array[1].field_4_options_array[2].field_80_menu_page_target = 5;
    field_136_menu_pages_array[1].field_4_options_array[3].field_0_option_type = STRING_TEXT_1;
    field_136_menu_pages_array[1].field_4_options_array[3].field_2_x_pos = 300;
    field_136_menu_pages_array[1].field_4_options_array[3].field_4_y_pos = 270;
    wcsncpy(field_136_menu_pages_array[1].field_4_options_array[3].field_6_option_name_str,
            gText_0x14_704DFC->Find_5B5F90("strtlev"),
            0x32u);
    field_136_menu_pages_array[1].field_4_options_array[3].field_80_menu_page_target = 264;
    field_136_menu_pages_array[1].field_4_options_array[4].field_0_option_type = STRING_TEXT_1;
    field_136_menu_pages_array[1].field_4_options_array[4].field_2_x_pos = 300;
    field_136_menu_pages_array[1].field_4_options_array[4].field_4_y_pos = 350;
    wcsncpy(field_136_menu_pages_array[1].field_4_options_array[4].field_6_option_name_str,
            gText_0x14_704DFC->Find_5B5F90("bonslev"),
            0x32u);
    field_136_menu_pages_array[1].field_4_options_array[4].field_80_menu_page_target = 265;
    field_136_menu_pages_array[1].field_B8A[0].field_0 = 280;
    field_136_menu_pages_array[1].field_B8A[0].field_2 = 218;
    field_136_menu_pages_array[1].field_B8A[1].field_0 = 280;
    field_136_menu_pages_array[1].field_B8A[1].field_2 = 238;
    field_136_menu_pages_array[1].field_B8A[2].field_0 = 280;
    field_136_menu_pages_array[1].field_B8A[2].field_2 = 258;
    field_136_menu_pages_array[1].field_B8A[3].field_0 = 280;
    field_136_menu_pages_array[1].field_B8A[3].field_2 = 278;
    field_136_menu_pages_array[1].field_B8A[4].field_0 = 280;
    field_136_menu_pages_array[1].field_B8A[4].field_2 = 358;
    field_136_menu_pages_array[1].field_BC6_current_option_idx = 3;
    field_136_menu_pages_array[1].field_BC8_default_option_idx = 3;
    field_136_menu_pages_array[1].field_2_number_of_elements = 10;
    field_136_menu_pages_array[1].field_518_elements_array[0].field_0_element_type = GEOMETRIC_SHAPE_3;
    field_136_menu_pages_array[1].field_518_elements_array[0].field_2_xpos = 420;
    field_136_menu_pages_array[1].field_518_elements_array[0].field_4_ypos = 310;
    field_136_menu_pages_array[1].field_518_elements_array[0].field_6_geometric_shape_type = 0;
    field_136_menu_pages_array[1].field_518_elements_array[1].field_0_element_type = GEOMETRIC_SHAPE_3;
    field_136_menu_pages_array[1].field_518_elements_array[1].field_2_xpos = 420;
    field_136_menu_pages_array[1].field_518_elements_array[1].field_4_ypos = 390;
    field_136_menu_pages_array[1].field_518_elements_array[1].field_6_geometric_shape_type = 0;
    field_136_menu_pages_array[1].field_518_elements_array[2].field_0_element_type = STRING_TEXT_1;
    field_136_menu_pages_array[1].field_518_elements_array[2].field_2_xpos = 410;
    field_136_menu_pages_array[1].field_518_elements_array[2].field_4_ypos = 298;
    wcsncpy(field_136_menu_pages_array[1].field_518_elements_array[2].field_6_element_name_str,
            gText_0x14_704DFC->Find_5B5F90("car_dam"),
            0x32u);
    field_136_menu_pages_array[1].field_518_elements_array[2].field_6A_font_type = word_703C3C;
    field_136_menu_pages_array[1].field_518_elements_array[3].field_0_element_type = STRING_TEXT_1;
    field_136_menu_pages_array[1].field_518_elements_array[3].field_2_xpos = 410;
    field_136_menu_pages_array[1].field_518_elements_array[3].field_4_ypos = 378;
    wcsncpy(field_136_menu_pages_array[1].field_518_elements_array[3].field_6_element_name_str,
            gText_0x14_704DFC->Find_5B5F90("car_dam"),
            0x32u);
    field_136_menu_pages_array[1].field_518_elements_array[3].field_6A_font_type = word_703C3C;
    field_136_menu_pages_array[1].field_518_elements_array[4].field_2_xpos = 380;
    field_136_menu_pages_array[1].field_518_elements_array[4].field_4_ypos = 310;
    field_136_menu_pages_array[1].field_518_elements_array[5].field_2_xpos = 460;
    field_136_menu_pages_array[1].field_518_elements_array[5].field_4_ypos = 310;
    field_136_menu_pages_array[1].field_518_elements_array[6].field_2_xpos = 380;
    field_136_menu_pages_array[1].field_518_elements_array[7].field_2_xpos = 460;
    field_136_menu_pages_array[1].field_518_elements_array[4].field_0_element_type = GEOMETRIC_SHAPE_3;
    field_136_menu_pages_array[1].field_518_elements_array[4].field_6_geometric_shape_type = 3;
    field_136_menu_pages_array[1].field_518_elements_array[5].field_0_element_type = GEOMETRIC_SHAPE_3;
    field_136_menu_pages_array[1].field_518_elements_array[5].field_6_geometric_shape_type = 4;
    field_136_menu_pages_array[1].field_518_elements_array[6].field_0_element_type = GEOMETRIC_SHAPE_3;
    field_136_menu_pages_array[1].field_518_elements_array[6].field_4_ypos = 390;
    field_136_menu_pages_array[1].field_518_elements_array[6].field_6_geometric_shape_type = 3;
    field_136_menu_pages_array[1].field_518_elements_array[7].field_0_element_type = GEOMETRIC_SHAPE_3;
    field_136_menu_pages_array[1].field_518_elements_array[7].field_4_ypos = 390;
    field_136_menu_pages_array[1].field_518_elements_array[7].field_6_geometric_shape_type = 4;
    field_136_menu_pages_array[1].field_518_elements_array[8].field_0_element_type = GEOMETRIC_SHAPE_3;
    field_136_menu_pages_array[1].field_518_elements_array[8].field_2_xpos = 290;
    field_136_menu_pages_array[1].field_518_elements_array[8].field_4_ypos = 222;
    field_136_menu_pages_array[1].field_518_elements_array[8].field_6_geometric_shape_type = 3;
    field_136_menu_pages_array[1].field_518_elements_array[9].field_0_element_type = GEOMETRIC_SHAPE_3;
    field_136_menu_pages_array[1].field_518_elements_array[9].field_2_xpos = 580;
    field_136_menu_pages_array[1].field_518_elements_array[9].field_4_ypos = 222;
    field_136_menu_pages_array[1].field_518_elements_array[9].field_6_geometric_shape_type = 4;
    field_136_menu_pages_array[11].field_0_number_of_options = 3;
    field_136_menu_pages_array[11].field_2_number_of_elements = 1;
    field_136_menu_pages_array[11].field_518_elements_array[0].field_0_element_type = STRING_TEXT_1;
    field_136_menu_pages_array[11].field_518_elements_array[0].field_2_xpos = 35;
    field_136_menu_pages_array[11].field_518_elements_array[0].field_4_ypos = 11;
    wcscpy(field_136_menu_pages_array[11].field_518_elements_array[0].field_6_element_name_str, gText_0x14_704DFC->Find_5B5F90("plr_qut"));
    field_136_menu_pages_array[11].field_518_elements_array[0].field_6A_font_type = field_130;
    field_136_menu_pages_array[11].field_518_elements_array[0].field_6C_font_palette = 5;
    field_136_menu_pages_array[11].field_4_options_array[0].field_0_option_type = STRING_TEXT_1;
    field_136_menu_pages_array[11].field_4_options_array[0].field_4_y_pos = 392;
    wcsncpy(field_136_menu_pages_array[11].field_4_options_array[0].field_6_option_name_str,
            gText_0x14_704DFC->Find_5B5F90("savepos"),
            0x32u);
    field_136_menu_pages_array[11].field_4_options_array[0].field_2_x_pos =
        Frontend::GetCenteredXPos_4B0190(field_136_menu_pages_array[11].field_4_options_array[0].field_6_option_name_str,
                             field_136_menu_pages_array[11].field_4_options_array[0].field_6A_font_type,
                             320);
    field_136_menu_pages_array[11].field_4_options_array[0].field_80_menu_page_target = 260;
    field_136_menu_pages_array[11].field_4_options_array[1].field_0_option_type = STRING_TEXT_1;
    field_136_menu_pages_array[11].field_4_options_array[1].field_4_y_pos = 412;
    wcsncpy(field_136_menu_pages_array[11].field_4_options_array[1].field_6_option_name_str,
            gText_0x14_704DFC->Find_5B5F90("replay"),
            0x32u);
    field_136_menu_pages_array[11].field_4_options_array[1].field_2_x_pos =
        Frontend::GetCenteredXPos_4B0190(field_136_menu_pages_array[11].field_4_options_array[1].field_6_option_name_str,
                             field_136_menu_pages_array[11].field_4_options_array[1].field_6A_font_type,
                             320);
    field_136_menu_pages_array[11].field_4_options_array[1].field_80_menu_page_target = 259;
    field_136_menu_pages_array[11].field_4_options_array[2].field_0_option_type = STRING_TEXT_1;
    field_136_menu_pages_array[11].field_4_options_array[2].field_4_y_pos = 432;
    wcsncpy(field_136_menu_pages_array[11].field_4_options_array[2].field_6_option_name_str,
            gText_0x14_704DFC->Find_5B5F90("mainmen"),
            0x32u);
    field_136_menu_pages_array[11].field_4_options_array[2].field_2_x_pos =
        Frontend::GetCenteredXPos_4B0190(field_136_menu_pages_array[11].field_4_options_array[2].field_6_option_name_str,
                             field_136_menu_pages_array[11].field_4_options_array[2].field_6A_font_type,
                             320);
    field_136_menu_pages_array[11].field_4_options_array[2].field_80_menu_page_target = 0;
    field_136_menu_pages_array[11].field_B8A[0].field_0 = 150;
    field_136_menu_pages_array[11].field_B8A[0].field_2 = 400;
    field_136_menu_pages_array[11].field_B8A[1].field_0 = 150;
    field_136_menu_pages_array[11].field_B8A[1].field_2 = 420;
    field_136_menu_pages_array[11].field_B8A[2].field_0 = 150;
    field_136_menu_pages_array[11].field_B8A[2].field_2 = 440;
    field_136_menu_pages_array[11].field_BC6_current_option_idx = 0;
    field_136_menu_pages_array[11].field_BC8_default_option_idx = 0;
    field_136_menu_pages_array[2].field_0_number_of_options = 3;
    field_136_menu_pages_array[2].field_2_number_of_elements = 1;
    field_136_menu_pages_array[2].field_518_elements_array[0].field_0_element_type = STRING_TEXT_1;
    field_136_menu_pages_array[2].field_518_elements_array[0].field_2_xpos = 35;
    field_136_menu_pages_array[2].field_518_elements_array[0].field_4_ypos = 11;
    wcsncpy(field_136_menu_pages_array[2].field_518_elements_array[0].field_6_element_name_str,
            gText_0x14_704DFC->Find_5B5F90("plr_ded"),
            0x32u);
    field_136_menu_pages_array[2].field_518_elements_array[0].field_6A_font_type = field_130;
    field_136_menu_pages_array[2].field_518_elements_array[0].field_6C_font_palette = 0;
    field_136_menu_pages_array[2].field_4_options_array[0].field_0_option_type = STRING_TEXT_1;
    field_136_menu_pages_array[2].field_4_options_array[0].field_4_y_pos = 392;
    wcsncpy(field_136_menu_pages_array[2].field_4_options_array[0].field_6_option_name_str,
            gText_0x14_704DFC->Find_5B5F90("savepos"),
            0x32u);
    field_136_menu_pages_array[2].field_4_options_array[0].field_2_x_pos =
        Frontend::GetCenteredXPos_4B0190(field_136_menu_pages_array[2].field_4_options_array[0].field_6_option_name_str,
                             field_136_menu_pages_array[2].field_4_options_array[0].field_6A_font_type,
                             320);
    field_136_menu_pages_array[2].field_4_options_array[0].field_80_menu_page_target = 260;
    field_136_menu_pages_array[2].field_4_options_array[1].field_0_option_type = STRING_TEXT_1;
    field_136_menu_pages_array[2].field_4_options_array[1].field_4_y_pos = 412;
    wcsncpy(field_136_menu_pages_array[2].field_4_options_array[1].field_6_option_name_str,
            gText_0x14_704DFC->Find_5B5F90("replay"),
            0x32u);
    field_136_menu_pages_array[2].field_4_options_array[1].field_2_x_pos =
        Frontend::GetCenteredXPos_4B0190(field_136_menu_pages_array[2].field_4_options_array[1].field_6_option_name_str,
                             field_136_menu_pages_array[2].field_4_options_array[1].field_6A_font_type,
                             320);
    field_136_menu_pages_array[2].field_4_options_array[1].field_80_menu_page_target = 259;
    field_136_menu_pages_array[2].field_4_options_array[2].field_0_option_type = STRING_TEXT_1;
    field_136_menu_pages_array[2].field_4_options_array[2].field_4_y_pos = 432;
    wcsncpy(field_136_menu_pages_array[2].field_4_options_array[2].field_6_option_name_str,
            gText_0x14_704DFC->Find_5B5F90("mainmen"),
            0x32u);
    field_136_menu_pages_array[2].field_4_options_array[2].field_2_x_pos =
        Frontend::GetCenteredXPos_4B0190(field_136_menu_pages_array[2].field_4_options_array[2].field_6_option_name_str,
                             field_136_menu_pages_array[2].field_4_options_array[2].field_6A_font_type,
                             320);
    field_136_menu_pages_array[2].field_4_options_array[2].field_80_menu_page_target = 0;
    field_136_menu_pages_array[2].field_B8A[0].field_0 = 150;
    field_136_menu_pages_array[2].field_B8A[0].field_2 = 400;
    field_136_menu_pages_array[2].field_B8A[1].field_0 = 150;
    field_136_menu_pages_array[2].field_B8A[1].field_2 = 420;
    field_136_menu_pages_array[2].field_B8A[2].field_0 = 150;
    field_136_menu_pages_array[2].field_B8A[2].field_2 = 440;
    field_136_menu_pages_array[2].field_BC6_current_option_idx = 0;
    field_136_menu_pages_array[2].field_BC8_default_option_idx = 0;
    field_136_menu_pages_array[3].field_0_number_of_options = 5;
    field_136_menu_pages_array[3].field_2_number_of_elements = 1;
    field_136_menu_pages_array[3].field_518_elements_array[0].field_0_element_type = STRING_TEXT_1;
    field_136_menu_pages_array[3].field_518_elements_array[0].field_2_xpos = 35;
    field_136_menu_pages_array[3].field_518_elements_array[0].field_4_ypos = 11;
    wcsncpy(field_136_menu_pages_array[3].field_518_elements_array[0].field_6_element_name_str,
            gText_0x14_704DFC->Find_5B5F90("cmpltd"),
            0x32u);
    field_136_menu_pages_array[3].field_518_elements_array[0].field_6A_font_type = field_12C;
    field_136_menu_pages_array[3].field_4_options_array[0].field_0_option_type = STRING_TEXT_1;
    field_136_menu_pages_array[3].field_4_options_array[0].field_4_y_pos = 365;
    wcsncpy(field_136_menu_pages_array[3].field_4_options_array[0].field_6_option_name_str,
            gText_0x14_704DFC->Find_5B5F90("nxt_lvl"),
            0x32u);
    field_136_menu_pages_array[3].field_4_options_array[0].field_2_x_pos =
        Frontend::GetCenteredXPos_4B0190(field_136_menu_pages_array[3].field_4_options_array[0].field_6_option_name_str,
                             field_136_menu_pages_array[3].field_4_options_array[0].field_6A_font_type,
                             320);
    field_136_menu_pages_array[3].field_4_options_array[0].field_80_menu_page_target = 261;
    field_136_menu_pages_array[3].field_4_options_array[1].field_0_option_type = STRING_TEXT_1;
    field_136_menu_pages_array[3].field_4_options_array[1].field_4_y_pos = 385;
    wcsncpy(field_136_menu_pages_array[3].field_4_options_array[1].field_6_option_name_str,
            gText_0x14_704DFC->Find_5B5F90("savepos"),
            0x32u);
    field_136_menu_pages_array[3].field_4_options_array[1].field_2_x_pos =
        Frontend::GetCenteredXPos_4B0190(field_136_menu_pages_array[3].field_4_options_array[1].field_6_option_name_str,
                             field_136_menu_pages_array[3].field_4_options_array[1].field_6A_font_type,
                             320);
    field_136_menu_pages_array[3].field_4_options_array[1].field_80_menu_page_target = 260;
    field_136_menu_pages_array[3].field_4_options_array[2].field_0_option_type = STRING_TEXT_1;
    field_136_menu_pages_array[3].field_4_options_array[2].field_4_y_pos = 405;
    wcsncpy(field_136_menu_pages_array[3].field_4_options_array[2].field_6_option_name_str,
            gText_0x14_704DFC->Find_5B5F90("replay"),
            0x32u);
    field_136_menu_pages_array[3].field_4_options_array[2].field_2_x_pos =
        Frontend::GetCenteredXPos_4B0190(field_136_menu_pages_array[3].field_4_options_array[2].field_6_option_name_str,
                             field_136_menu_pages_array[3].field_4_options_array[2].field_6A_font_type,
                             320);
    field_136_menu_pages_array[3].field_4_options_array[2].field_80_menu_page_target = 259;
    field_136_menu_pages_array[3].field_4_options_array[3].field_0_option_type = STRING_TEXT_1;
    field_136_menu_pages_array[3].field_4_options_array[3].field_4_y_pos = 425;
    wcsncpy(field_136_menu_pages_array[3].field_4_options_array[3].field_6_option_name_str,
            gText_0x14_704DFC->Find_5B5F90("contnue"),
            0x32u);
    field_136_menu_pages_array[3].field_4_options_array[3].field_2_x_pos =
        Frontend::GetCenteredXPos_4B0190(field_136_menu_pages_array[3].field_4_options_array[3].field_6_option_name_str,
                             field_136_menu_pages_array[3].field_4_options_array[3].field_6A_font_type,
                             320);
    field_136_menu_pages_array[3].field_4_options_array[3].field_80_menu_page_target = 266;
    field_136_menu_pages_array[3].field_4_options_array[4].field_0_option_type = STRING_TEXT_1;
    field_136_menu_pages_array[3].field_4_options_array[4].field_4_y_pos = 445;
    wcsncpy(field_136_menu_pages_array[3].field_4_options_array[4].field_6_option_name_str,
            gText_0x14_704DFC->Find_5B5F90("mainmen"),
            0x32u);
    field_136_menu_pages_array[3].field_4_options_array[4].field_2_x_pos =
        Frontend::GetCenteredXPos_4B0190(field_136_menu_pages_array[3].field_4_options_array[4].field_6_option_name_str,
                             field_136_menu_pages_array[3].field_4_options_array[4].field_6A_font_type,
                             320);
    field_136_menu_pages_array[3].field_4_options_array[4].field_80_menu_page_target = 0;
    field_136_menu_pages_array[3].field_B8A[0].field_0 = 150;
    field_136_menu_pages_array[3].field_B8A[0].field_2 = 373;
    field_136_menu_pages_array[3].field_B8A[1].field_0 = 150;
    field_136_menu_pages_array[3].field_B8A[1].field_2 = 393;
    field_136_menu_pages_array[3].field_B8A[2].field_0 = 150;
    field_136_menu_pages_array[3].field_B8A[2].field_2 = 413;
    field_136_menu_pages_array[3].field_B8A[3].field_0 = 150;
    field_136_menu_pages_array[3].field_B8A[3].field_2 = 433;
    field_136_menu_pages_array[3].field_B8A[4].field_0 = 150;
    field_136_menu_pages_array[3].field_B8A[4].field_2 = 453;
    field_136_menu_pages_array[3].field_BC6_current_option_idx = 0;
    field_136_menu_pages_array[3].field_BC8_default_option_idx = 0;
    field_136_menu_pages_array[4].field_0_number_of_options = 1;
    field_136_menu_pages_array[4].field_2_number_of_elements = 1;
    field_136_menu_pages_array[4].field_518_elements_array[0].field_0_element_type = STRING_TEXT_1;
    field_136_menu_pages_array[4].field_518_elements_array[0].field_4_ypos = 230;
    wcsncpy(field_136_menu_pages_array[4].field_518_elements_array[0].field_6_element_name_str,
            gText_0x14_704DFC->Find_5B5F90("gam_cmp"),
            0x32u);
    v30 = field_130;
    field_136_menu_pages_array[4].field_518_elements_array[0].field_6A_font_type = v30;
    field_136_menu_pages_array[4].field_518_elements_array[0].field_2_xpos =
        Frontend::GetCenteredXPos_4B0190(field_136_menu_pages_array[4].field_518_elements_array[0].field_6_element_name_str, v30, 320);
    field_136_menu_pages_array[4].field_518_elements_array[0].field_6C_font_palette = 4;
    field_136_menu_pages_array[4].field_4_options_array[0].field_0_option_type = STRING_TEXT_1;
    field_136_menu_pages_array[4].field_4_options_array[0].field_2_x_pos = 180;
    field_136_menu_pages_array[4].field_4_options_array[0].field_4_y_pos = 410;
    wcsncpy(field_136_menu_pages_array[4].field_4_options_array[0].field_6_option_name_str,
            gText_0x14_704DFC->Find_5B5F90("mainmen"),
            0x32u);
    field_136_menu_pages_array[4].field_4_options_array[0].field_80_menu_page_target = 0;
    field_136_menu_pages_array[4].field_B8A[0].field_0 = 160;
    field_136_menu_pages_array[4].field_B8A[0].field_2 = 418;
    field_136_menu_pages_array[4].field_BC6_current_option_idx = 0;
    field_136_menu_pages_array[4].field_BC8_default_option_idx = 0;
    field_136_menu_pages_array[5].field_0_number_of_options = 1;
    field_136_menu_pages_array[5].field_2_number_of_elements = 5;
    field_136_menu_pages_array[5].field_4_options_array[0].field_0_option_type = STRING_TEXT_2;
    field_136_menu_pages_array[5].field_4_options_array[0].field_2_x_pos = 300;
    field_136_menu_pages_array[5].field_4_options_array[0].field_4_y_pos = 155;
    field_136_menu_pages_array[5].field_4_options_array[0].field_6E_horizontal_selected_idx = 0;
    field_136_menu_pages_array[5].field_4_options_array[0].field_70 = 0;
    field_136_menu_pages_array[5].field_4_options_array[0].field_7E_horizontal_max_idx = 11;

    u16 v323 = 0;
    do
    {
        field_136_menu_pages_array[5].field_4_options_array[0].field_72_horizontal_idx_enabled[v323++] = 1;
    } while (v323 <= field_136_menu_pages_array[5].field_4_options_array[0].field_7E_horizontal_max_idx);

    field_136_menu_pages_array[5].field_B8A[0].field_0 = 280;
    field_136_menu_pages_array[5].field_B8A[0].field_2 = 163;
    field_136_menu_pages_array[5].field_BC6_current_option_idx = 0;
    field_136_menu_pages_array[5].field_BC8_default_option_idx = 0;
    field_136_menu_pages_array[5].field_518_elements_array[0].field_0_element_type = GEOMETRIC_SHAPE_3;
    field_136_menu_pages_array[5].field_518_elements_array[0].field_2_xpos = 450;
    field_136_menu_pages_array[5].field_518_elements_array[0].field_4_ypos = 197;
    field_136_menu_pages_array[5].field_518_elements_array[0].field_6_geometric_shape_type = 0;
    field_136_menu_pages_array[5].field_518_elements_array[1].field_0_element_type = STRING_TEXT_1;
    field_136_menu_pages_array[5].field_518_elements_array[1].field_2_xpos = 440;
    field_136_menu_pages_array[5].field_518_elements_array[1].field_4_ypos = 185;
    //    v34 = ;
    field_136_menu_pages_array[5].field_518_elements_array[2].field_4_ypos = 197;
    field_136_menu_pages_array[5].field_518_elements_array[3].field_4_ypos = 197;
    field_136_menu_pages_array[5].field_518_elements_array[1].field_6A_font_type = word_703C3C; // v34
    field_136_menu_pages_array[5].field_518_elements_array[2].field_0_element_type = GEOMETRIC_SHAPE_3;
    field_136_menu_pages_array[5].field_518_elements_array[2].field_2_xpos = 410;
    field_136_menu_pages_array[5].field_518_elements_array[2].field_6_geometric_shape_type = 3;
    field_136_menu_pages_array[5].field_518_elements_array[3].field_0_element_type = GEOMETRIC_SHAPE_3;
    field_136_menu_pages_array[5].field_518_elements_array[3].field_2_xpos = 490;
    field_136_menu_pages_array[5].field_518_elements_array[3].field_6_geometric_shape_type = 4;
    field_136_menu_pages_array[5].field_518_elements_array[4].field_0_element_type = STRING_TEXT_1;
    field_136_menu_pages_array[5].field_518_elements_array[4].field_2_xpos = 340;
    field_136_menu_pages_array[5].field_518_elements_array[4].field_4_ypos = v2;
    wcsncpy(field_136_menu_pages_array[5].field_518_elements_array[4].field_6_element_name_str,
            gText_0x14_704DFC->Find_5B5F90("hi_scre"),
            0x32u);
    field_136_menu_pages_array[5].field_518_elements_array[4].field_6A_font_type = field_126;
    field_136_menu_pages_array[6].field_0_number_of_options = 3;
    field_136_menu_pages_array[6].field_2_number_of_elements = 3;
    field_136_menu_pages_array[6].field_518_elements_array[0].field_0_element_type = STRING_TEXT_1;
    field_136_menu_pages_array[6].field_518_elements_array[0].field_2_xpos = 35;
    field_136_menu_pages_array[6].field_518_elements_array[0].field_4_ypos = 11;
    wcsncpy(field_136_menu_pages_array[6].field_518_elements_array[0].field_6_element_name_str,
            gText_0x14_704DFC->Find_5B5F90("bonslev"),
            0x32u);
    field_136_menu_pages_array[6].field_518_elements_array[0].field_6A_font_type = field_130;
    field_136_menu_pages_array[6].field_518_elements_array[0].field_6C_font_palette = 5;
    field_136_menu_pages_array[6].field_518_elements_array[1].field_0_element_type = STRING_TEXT_1;
    field_136_menu_pages_array[6].field_518_elements_array[1].field_2_xpos = 170;
    field_136_menu_pages_array[6].field_518_elements_array[1].field_4_ypos = 250;
    wcsncpy(field_136_menu_pages_array[6].field_518_elements_array[1].field_6_element_name_str,
            gText_0x14_704DFC->Find_5B5F90("score"),
            0x32u);
    field_136_menu_pages_array[6].field_518_elements_array[2].field_0_element_type = STRING_TEXT_1;
    field_136_menu_pages_array[6].field_518_elements_array[2].field_2_xpos = 400;
    field_136_menu_pages_array[6].field_518_elements_array[2].field_4_ypos = 250;
    s16 v38 = field_120_selected_font;
    field_136_menu_pages_array[6].field_4_options_array[0].field_0_option_type = STRING_TEXT_1;
    field_136_menu_pages_array[6].field_518_elements_array[2].field_6A_font_type = v38;
    field_136_menu_pages_array[6].field_4_options_array[0].field_4_y_pos = 340;
    wcsncpy(field_136_menu_pages_array[6].field_4_options_array[0].field_6_option_name_str,
            gText_0x14_704DFC->Find_5B5F90("repbons"),
            0x32u);
    field_136_menu_pages_array[6].field_4_options_array[0].field_2_x_pos =
        Frontend::GetCenteredXPos_4B0190(field_136_menu_pages_array[6].field_4_options_array[0].field_6_option_name_str, v38, 320);
    field_136_menu_pages_array[6].field_4_options_array[0].field_80_menu_page_target = 259;
    field_136_menu_pages_array[6].field_4_options_array[1].field_0_option_type = STRING_TEXT_1;
    field_136_menu_pages_array[6].field_4_options_array[1].field_4_y_pos = 360;
    wcsncpy(field_136_menu_pages_array[6].field_4_options_array[1].field_6_option_name_str,
            gText_0x14_704DFC->Find_5B5F90("nxt_lvl"),
            0x32u);
    field_136_menu_pages_array[6].field_4_options_array[1].field_2_x_pos =
        Frontend::GetCenteredXPos_4B0190(field_136_menu_pages_array[6].field_4_options_array[1].field_6_option_name_str,
                             field_136_menu_pages_array[6].field_4_options_array[1].field_6A_font_type,
                             320);
    field_136_menu_pages_array[6].field_4_options_array[1].field_80_menu_page_target = 261;
    field_136_menu_pages_array[6].field_4_options_array[2].field_0_option_type = STRING_TEXT_1;
    field_136_menu_pages_array[6].field_4_options_array[2].field_4_y_pos = 380;
    wcsncpy(field_136_menu_pages_array[6].field_4_options_array[2].field_6_option_name_str,
            gText_0x14_704DFC->Find_5B5F90("mainmen"),
            0x32u);
    field_136_menu_pages_array[6].field_4_options_array[2].field_2_x_pos =
        Frontend::GetCenteredXPos_4B0190(field_136_menu_pages_array[6].field_4_options_array[2].field_6_option_name_str,
                             field_136_menu_pages_array[6].field_4_options_array[2].field_6A_font_type,
                             320);
    field_136_menu_pages_array[6].field_4_options_array[2].field_80_menu_page_target = 0;
    field_136_menu_pages_array[6].field_B8A[0].field_0 = 150;
    field_136_menu_pages_array[6].field_B8A[0].field_2 = 348;
    field_136_menu_pages_array[6].field_B8A[1].field_0 = 150;
    field_136_menu_pages_array[6].field_B8A[1].field_2 = 368;
    field_136_menu_pages_array[6].field_B8A[2].field_0 = 150;
    field_136_menu_pages_array[6].field_B8A[2].field_2 = 388;
    field_136_menu_pages_array[6].field_BC6_current_option_idx = 0;
    field_136_menu_pages_array[6].field_BC8_default_option_idx = 0;
    field_136_menu_pages_array[7].field_0_number_of_options = 1;
    field_136_menu_pages_array[7].field_2_number_of_elements = 14;
    field_136_menu_pages_array[7].field_518_elements_array[0].field_0_element_type = STRING_TEXT_1;
    field_136_menu_pages_array[7].field_518_elements_array[0].field_2_xpos = 35;
    field_136_menu_pages_array[7].field_518_elements_array[0].field_4_ypos = 11;
    field_136_menu_pages_array[7].field_518_elements_array[0].field_6A_font_type = field_130;
    field_136_menu_pages_array[7].field_518_elements_array[0].field_6C_font_palette = 5;
    field_136_menu_pages_array[7].field_518_elements_array[1].field_0_element_type = STRING_TEXT_1;
    field_136_menu_pages_array[7].field_518_elements_array[1].field_2_xpos = 100;
    field_136_menu_pages_array[7].field_518_elements_array[1].field_4_ypos = 170;
    wcsncpy(field_136_menu_pages_array[7].field_518_elements_array[1].field_6_element_name_str, gEmptyWStr_67DC8C, 50u);
    field_136_menu_pages_array[7].field_518_elements_array[2].field_0_element_type = STRING_TEXT_1;
    field_136_menu_pages_array[7].field_518_elements_array[2].field_2_xpos = 100;
    field_136_menu_pages_array[7].field_518_elements_array[2].field_4_ypos = 190;
    wcsncpy(field_136_menu_pages_array[7].field_518_elements_array[2].field_6_element_name_str, gEmptyWStr_67DC8C, 0x32u);
    field_136_menu_pages_array[7].field_518_elements_array[3].field_0_element_type = STRING_TEXT_1;
    field_136_menu_pages_array[7].field_518_elements_array[3].field_2_xpos = 100;
    field_136_menu_pages_array[7].field_518_elements_array[3].field_4_ypos = 210;
    wcsncpy(field_136_menu_pages_array[7].field_518_elements_array[3].field_6_element_name_str, gEmptyWStr_67DC8C, 0x32u);
    field_136_menu_pages_array[7].field_518_elements_array[4].field_0_element_type = STRING_TEXT_1;
    field_136_menu_pages_array[7].field_518_elements_array[4].field_2_xpos = 100;
    field_136_menu_pages_array[7].field_518_elements_array[4].field_4_ypos = 230;
    wcsncpy(field_136_menu_pages_array[7].field_518_elements_array[4].field_6_element_name_str, gEmptyWStr_67DC8C, 0x32u);
    field_136_menu_pages_array[7].field_518_elements_array[5].field_0_element_type = STRING_TEXT_1;
    field_136_menu_pages_array[7].field_518_elements_array[5].field_2_xpos = 100;
    field_136_menu_pages_array[7].field_518_elements_array[5].field_4_ypos = 250;
    wcsncpy(field_136_menu_pages_array[7].field_518_elements_array[5].field_6_element_name_str, gEmptyWStr_67DC8C, 0x32u);
    field_136_menu_pages_array[7].field_518_elements_array[6].field_0_element_type = STRING_TEXT_1;
    field_136_menu_pages_array[7].field_518_elements_array[6].field_2_xpos = 100;
    field_136_menu_pages_array[7].field_518_elements_array[6].field_4_ypos = 270;
    wcsncpy(field_136_menu_pages_array[7].field_518_elements_array[6].field_6_element_name_str, gEmptyWStr_67DC8C, 0x32u);
    field_136_menu_pages_array[7].field_518_elements_array[7].field_0_element_type = STRING_TEXT_1;
    field_136_menu_pages_array[7].field_518_elements_array[7].field_4_ypos = 300;
    wcsncpy(field_136_menu_pages_array[7].field_518_elements_array[7].field_6_element_name_str,
            gText_0x14_704DFC->Find_5B5F90("kills_h"),
            0x32u);
    field_136_menu_pages_array[7].field_518_elements_array[7].field_2_xpos =
        GetCenteredXPos_4B0190(field_136_menu_pages_array[7].field_518_elements_array[7].field_6_element_name_str,
                   field_136_menu_pages_array[7].field_518_elements_array[7].field_6A_font_type,
                   320);
    field_136_menu_pages_array[7].field_518_elements_array[8].field_0_element_type = STRING_TEXT_1;
    field_136_menu_pages_array[7].field_518_elements_array[8].field_2_xpos = 100;
    field_136_menu_pages_array[7].field_518_elements_array[8].field_4_ypos = 320;
    field_136_menu_pages_array[7].field_518_elements_array[9].field_0_element_type = STRING_TEXT_1;
    field_136_menu_pages_array[7].field_518_elements_array[9].field_2_xpos = 100;
    field_136_menu_pages_array[7].field_518_elements_array[9].field_4_ypos = 340;
    field_136_menu_pages_array[7].field_518_elements_array[10].field_0_element_type = STRING_TEXT_1;
    field_136_menu_pages_array[7].field_518_elements_array[10].field_2_xpos = 100;
    field_136_menu_pages_array[7].field_518_elements_array[10].field_4_ypos = 360;
    field_136_menu_pages_array[7].field_518_elements_array[11].field_0_element_type = STRING_TEXT_1;
    field_136_menu_pages_array[7].field_518_elements_array[11].field_2_xpos = 100;
    field_136_menu_pages_array[7].field_518_elements_array[11].field_4_ypos = 380;
    field_136_menu_pages_array[7].field_518_elements_array[12].field_0_element_type = STRING_TEXT_1;
    field_136_menu_pages_array[7].field_518_elements_array[12].field_2_xpos = 100;
    field_136_menu_pages_array[7].field_518_elements_array[12].field_4_ypos = 400;
    field_136_menu_pages_array[7].field_518_elements_array[13].field_0_element_type = STRING_TEXT_1;
    field_136_menu_pages_array[7].field_518_elements_array[13].field_2_xpos = 30;
    field_136_menu_pages_array[7].field_518_elements_array[13].field_4_ypos = 150;
    field_136_menu_pages_array[7].field_4_options_array[0].field_0_option_type = STRING_TEXT_1;
    field_136_menu_pages_array[7].field_4_options_array[0].field_4_y_pos = 430;
    wcsncpy(field_136_menu_pages_array[7].field_4_options_array[0].field_6_option_name_str, gText_0x14_704DFC->Find_5B5F90("quit"), 0x32u);
    field_136_menu_pages_array[7].field_4_options_array[0].field_2_x_pos =
        GetCenteredXPos_4B0190(field_136_menu_pages_array[7].field_4_options_array[0].field_6_option_name_str,
                   field_136_menu_pages_array[7].field_4_options_array[0].field_6A_font_type,
                   320);
    field_136_menu_pages_array[7].field_4_options_array[0].field_80_menu_page_target = 258;
    field_136_menu_pages_array[7].field_B8A[0].field_0 = 180;
    field_136_menu_pages_array[7].field_B8A[0].field_2 = 438;
    field_136_menu_pages_array[7].field_BC6_current_option_idx = 0;
    field_136_menu_pages_array[7].field_BC8_default_option_idx = 0;
    field_136_menu_pages_array[8].field_0_number_of_options = 1;
    field_136_menu_pages_array[8].field_2_number_of_elements = 0;
    field_136_menu_pages_array[8].field_4_options_array[0].field_0_option_type = STRING_TEXT_1;
    field_136_menu_pages_array[8].field_4_options_array[0].field_2_x_pos = 200;
    field_136_menu_pages_array[8].field_4_options_array[0].field_4_y_pos = 280;
    wcsncpy(field_136_menu_pages_array[8].field_4_options_array[0].field_6_option_name_str,
            gText_0x14_704DFC->Find_5B5F90("mainmen"),
            0x32u);
    field_136_menu_pages_array[8].field_4_options_array[0].field_80_menu_page_target = 0;
    field_136_menu_pages_array[8].field_B8A[0].field_0 = 180;
    field_136_menu_pages_array[8].field_B8A[0].field_2 = 288;
    field_136_menu_pages_array[8].field_BC6_current_option_idx = 0;
    field_136_menu_pages_array[8].field_BC8_default_option_idx = 0;
    field_136_menu_pages_array[10].field_0_number_of_options = 1;
    field_136_menu_pages_array[10].field_2_number_of_elements = 1;
    field_136_menu_pages_array[10].field_518_elements_array[0].field_0_element_type = STRING_TEXT_1;
    field_136_menu_pages_array[10].field_518_elements_array[0].field_4_ypos = 230;
    wcsncpy(field_136_menu_pages_array[10].field_518_elements_array[0].field_6_element_name_str,
            gText_0x14_704DFC->Find_5B5F90("nicetry"),
            0x32u);
    //    v46 = field_130;
    field_136_menu_pages_array[10].field_518_elements_array[0].field_6A_font_type = field_130; // v46;
    field_136_menu_pages_array[10].field_518_elements_array[0].field_2_xpos =
        GetCenteredXPos_4B0190(field_136_menu_pages_array[10].field_518_elements_array[0].field_6_element_name_str,
                   field_130, //v46,
                   320);
    field_136_menu_pages_array[10].field_518_elements_array[0].field_6C_font_palette = 4;
    field_136_menu_pages_array[10].field_4_options_array[0].field_0_option_type = STRING_TEXT_1;
    field_136_menu_pages_array[10].field_4_options_array[0].field_2_x_pos = 180;
    field_136_menu_pages_array[10].field_4_options_array[0].field_4_y_pos = 410;
    wcsncpy(field_136_menu_pages_array[10].field_4_options_array[0].field_6_option_name_str,
            gText_0x14_704DFC->Find_5B5F90("mainmen"),
            0x32u);
    field_136_menu_pages_array[10].field_4_options_array[0].field_80_menu_page_target = 0;
    field_136_menu_pages_array[10].field_B8A[0].field_0 = 160;
    field_136_menu_pages_array[10].field_B8A[0].field_2 = 418;
    field_136_menu_pages_array[10].field_BC6_current_option_idx = 0;
    field_136_menu_pages_array[10].field_BC8_default_option_idx = 0;
    field_136_menu_pages_array[14].field_0_number_of_options = 1;
    field_136_menu_pages_array[14].field_2_number_of_elements = 5;
    field_136_menu_pages_array[14].field_4_options_array[0].field_0_option_type = STRING_TEXT_1;
    field_136_menu_pages_array[14].field_4_options_array[0].field_2_x_pos = 170;
    field_136_menu_pages_array[14].field_4_options_array[0].field_4_y_pos = 340;
    field_136_menu_pages_array[14].field_4_options_array[0].field_80_menu_page_target = 268;
    field_136_menu_pages_array[14].field_518_elements_array[0].field_0_element_type = STRING_TEXT_1;
    field_136_menu_pages_array[14].field_518_elements_array[0].field_2_xpos = 20;
    field_136_menu_pages_array[14].field_518_elements_array[0].field_4_ypos = 160;
    wcsncpy(field_136_menu_pages_array[14].field_518_elements_array[0].field_6_element_name_str,
            gText_0x14_704DFC->Find_5B5F90("fr_ent1"),
            0x32u);
    field_136_menu_pages_array[14].field_518_elements_array[1].field_0_element_type = STRING_TEXT_1;
    field_136_menu_pages_array[14].field_518_elements_array[1].field_2_xpos = 20;
    field_136_menu_pages_array[14].field_518_elements_array[1].field_4_ypos = 180;
    wcsncpy(field_136_menu_pages_array[14].field_518_elements_array[1].field_6_element_name_str,
            gText_0x14_704DFC->Find_5B5F90("fr_ent2"),
            0x32u);
    field_136_menu_pages_array[14].field_518_elements_array[2].field_0_element_type = STRING_TEXT_1;
    field_136_menu_pages_array[14].field_518_elements_array[2].field_2_xpos = 20;
    field_136_menu_pages_array[14].field_518_elements_array[2].field_4_ypos = 200;
    wcsncpy(field_136_menu_pages_array[14].field_518_elements_array[2].field_6_element_name_str, gEmptyWStr_67DC8C, 0x32u);
    field_136_menu_pages_array[14].field_518_elements_array[3].field_0_element_type = STRING_TEXT_1;
    field_136_menu_pages_array[14].field_518_elements_array[3].field_2_xpos = 20;
    field_136_menu_pages_array[14].field_518_elements_array[3].field_4_ypos = 300;
    wcsncpy(field_136_menu_pages_array[14].field_518_elements_array[3].field_6_element_name_str,
            gText_0x14_704DFC->Find_5B5F90("fr_pmpt"),
            0x32u);
    field_136_menu_pages_array[14].field_518_elements_array[4].field_0_element_type = STRING_TEXT_1;
    field_136_menu_pages_array[14].field_518_elements_array[4].field_2_xpos = 20;
    field_136_menu_pages_array[14].field_518_elements_array[4].field_4_ypos = 320;
    wcsncpy(field_136_menu_pages_array[14].field_518_elements_array[4].field_6_element_name_str,
            gText_0x14_704DFC->Find_5B5F90("score"),
            0x32u);
    field_136_menu_pages_array[14].field_B8A[0].field_0 = 150;
    field_136_menu_pages_array[14].field_B8A[0].field_2 = 348; //  TODO: check for wrong var
    field_136_menu_pages_array[14].field_BC6_current_option_idx = 0;
    field_136_menu_pages_array[14].field_BC8_default_option_idx = 0;
    field_EE0E_unk.LoadCredits_483F20();
}

WIP_FUNC(0x4B4440)
void Frontend::GetMainAndBonusStagesFromSeqFile_4B4440()
{
    WIP_IMPLEMENTED;

    u8* pBlock; // esi
    char mainOrBonus[256]; // [esp+14h] [ebp-718h] BYREF
    char seqFileName[256]; // [esp+414h] [ebp-318h] BYREF
    _finddata_t findInfo; // [esp+514h] [ebp-218h] BYREF

    long hFind = _findfirst("data\\*.seq", &findInfo);
    if (hFind == -1)
    {
        FatalError_4A38C0(Gta2Error::SeqFileNotFound, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 4876);
    }
    else
    {
        if (!_findnext(hFind, &findInfo))
        {
            FatalError_4A38C0(Gta2Error::MultipleSeqFilesFound, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 4883);
        }

        strcpy(seqFileName, "data\\");
        strcat(seqFileName, findInfo.name);
        _findclose(hFind);
    }

    this->field_1EB50_num_main_stages = 0;
    bool mainBlockFound = false;
    for (s32 i = 0; i < 3; i++)
    {
        this->field_1EB51_num_bonus_stages[i] = 0;
    }
    u16 main_block_counter = 0;

    FILE* hSeqFile = crt::fopen(seqFileName, "rt");
    if (!hSeqFile)
    {
        FatalError_4A38C0(Gta2Error::SeqFileOpenError, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 4906);
        return;
    }

    GetSeqItem_4B48D0(0, mainOrBonus, hSeqFile);

    while (strcmp(mainOrBonus, "") != 0)
    {
        char debugStr[256];
        char styName[256];
        char mapName[256];
        char description[256];
        if (strcmp(mainOrBonus, "MAIN") == 0)
        {
            if (mainBlockFound)
            {
                if (++main_block_counter > 2u)
                {
                    FatalError_4A38C0(Gta2Error::TooManyMainBlocks, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 4922);
                }
            }
            mainBlockFound = true;
            pBlock = &this->field_1EB51_num_bonus_stages[main_block_counter];
            *pBlock = 0;

            GetSeqItem_4B48D0(1, debugStr, hSeqFile);
            GetSeqItem_4B48D0(2, styName, hSeqFile);
            GetSeqItem_4B48D0(3, mapName, hSeqFile);
            GetSeqItem_4B48D0(4, description, hSeqFile);
            StoreStringsForStage_4B4BC0(main_block_counter, *pBlock, debugStr, styName, mapName);
            ++*pBlock;
        }
        else if (strcmp(mainOrBonus, "BONUS") == 0)
        {
            if (!mainBlockFound)
            {
                FatalError_4A38C0(Gta2Error::MainBlockMustPrecedeBonus, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 4940);
            }
            pBlock = &field_1EB51_num_bonus_stages[main_block_counter];
            if (*pBlock > 3u)
            {
                FatalError_4A38C0(Gta2Error::TooManyBonusBlocks, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 4945);
            }

            GetSeqItem_4B48D0(1, debugStr, hSeqFile);
            GetSeqItem_4B48D0(2, styName, hSeqFile);
            GetSeqItem_4B48D0(3, mapName, hSeqFile);
            GetSeqItem_4B48D0(4, description, hSeqFile);
            StoreStringsForStage_4B4BC0(main_block_counter, *pBlock, debugStr, styName, mapName);
            ++*pBlock;
        }
        else
        {
            FatalError_4A38C0(Gta2Error::InvalidBlockType, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 4959);
        }

        GetSeqItem_4B48D0(0, mainOrBonus, hSeqFile);
    }

    field_1EB50_num_main_stages = main_block_counter + 1;
    crt::fclose(hSeqFile);
}

MATCH_FUNC(0x4B48D0)
void Frontend::GetSeqItem_4B48D0(s32 type, char_type* ppRet, FILE* hSeqFile)
{
    char_type type_buf[52];
    char_type output_buf[256];

    u16 pos = 0;
    char_type letter = File::SkipWhitespace_4A7340(hSeqFile);
    while (!letter)
    {
        if (feof(hSeqFile))
        {
            strcpy(ppRet, "");
            return;
        }
        letter = File::SkipWhitespace_4A7340(hSeqFile);
    }

    if (letter)
    {
        if ((letter < 'a' || letter > 'z') && (letter < 'A' || letter > 'Z'))
        {
            FatalError_4A38C0(Gta2Error::InvalidFirstLineCharacter, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 5014);
        }

        while (letter != '=')
        {
            output_buf[pos++] = letter;
            letter = File::SkipWhitespace_4A7340(hSeqFile);
            if (!letter)
            {
                FatalError_4A38C0(Gta2Error::LineInterruptedByNewline, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 5026);
            }
            if (pos > 0xFFu)
            {
                FatalError_4A38C0(Gta2Error::LabelTooLong, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 5031); // LabelTooLong
            }
        } // 0x3d
        output_buf[pos] = 0;

        switch (type & 0xff) // TODO: Wrong type ??
        {
            case 0:
                strcpy(type_buf, "MainOrBonus");
                break;
            case 1:
                strcpy(type_buf, "GMPFile");
                break;
            case 2:
                strcpy(type_buf, "STYFile");
                break;
            case 3:
                strcpy(type_buf, "SCRFile");
                break;
            case 4:
                strcpy(type_buf, "Description");
                break;
            default:
                FatalError_4A38C0(Gta2Error::UndefinedLabel, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 5056);
                break;
        }

        if (strcmp(type_buf, output_buf) == 0)
        {
            pos = 0;
            letter = File::SkipWhitespace_4A7340(hSeqFile);
            if (letter)
            {
                do
                {
                    output_buf[pos++] = letter;
                    letter = File::SkipWhitespace_4A7340(hSeqFile);
                    if (pos > 255u)
                    {
                        FatalError_4A38C0(Gta2Error::LineDataTooLong, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 5074);
                    }
                } while (letter);
            }
            output_buf[pos] = 0;
            strcpy(ppRet, output_buf);
        }
        else
        {
            FatalError_4A38C0(Gta2Error::UnexpectedLabel, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 5086);
        }
    }
}

MATCH_FUNC(0x4B53C0)
void Frontend::LoadPlySlotSvgs_4B53C0()
{
    char_type FileName[256];
    for (u8 i = 0; i < GTA2_COUNTOF(field_EDE8_plySlots); i++)
    {
        if (PlySlotSvgExists_4B5370(i))
        {
            GetPlySlotSvgName_4B51D0(i, FileName);
            field_EDE8_plySlots[i].LoadPlySlotSvg_4B6480(FileName);
        }
        else
        {
            field_EDE8_plySlots[i].field_0_save_exists = false;
            field_EDE8_plySlots[i].field_1_last_saved_stage = 3;
            field_EDE8_plySlots[i].field_2_last_saved_bonus_stage_code = 4;
            field_EDE8_plySlots[i].field_3_last_saved_is_bonus = 0;
        }
    }
}

MATCH_FUNC(0x4B66B0)
void Frontend::Load_tgas_4B66B0()
{
    if (pgbh_InitImageTable(gTableSize_61FF20) != -1)
    {
        for (u16 i = 0; i < gTableSize_61FF20; ++i)
        {
            Load_tga_4B6520(i);
        }
    }
}

MATCH_FUNC(0x4B51D0)
void Frontend::GetPlySlotSvgName_4B51D0(u8 idx, char_type* pStr)
{
    char_type Buffer[8];
    _itoa(idx, Buffer, 10);
    strcpy(pStr, "player\\plyslot");
    strcat(pStr, Buffer);
    strcat(pStr, ".svg");
}

MATCH_FUNC(0x4B5270)
void Frontend::DrawSavedStage_4B5270()
{
    u8 plySlotIdx = gLucid_hamilton_67E8E0.GetPlySlotIdx_4C59B0();
    u8 codified_stages = field_EDE8_plySlots[plySlotIdx].field_2_last_saved_bonus_stage_code;

    u8 main_stage;
    u8 bonus_stage;

    if (!field_EDE8_plySlots[plySlotIdx].field_3_last_saved_is_bonus)
    {
        bonus_stage = 0;
        main_stage = field_EDE8_plySlots[plySlotIdx].field_1_last_saved_stage;
    }
    else
    {
        gLucid_hamilton_67E8E0.DecodeStage_453A60(codified_stages, &main_stage, &bonus_stage);
    }
    swprintf(tmpBuff_67BD9C, L"%d", main_stage);
    DrawText_4B87A0(tmpBuff_67BD9C, (s16)450, (s16)90, field_11C_normal_font, 1);

    swprintf(tmpBuff_67BD9C, L"%d", bonus_stage);
    DrawText_4B87A0(tmpBuff_67BD9C, (s16)450, (s16)110, field_11C_normal_font, 1);
}

MATCH_FUNC(0x4B5370)
char_type Frontend::PlySlotSvgExists_4B5370(u8 idx)
{
    char_type FileName[256];
    GetPlySlotSvgName_4B51D0(idx, FileName);

    _finddata_t findData;
    long hFind = _findfirst(FileName, &findData);
    if (hFind == -1)
    {
        return 0;
    }

    _findclose(hFind);
    return 1;
}

MATCH_FUNC(0x4B77B0)
u8 Frontend::GetPrevUnlockedStageIndex_4B77B0(player_stats_0xA4* a2)
{
    u8 result;

    for (result = field_1EB50_num_main_stages - 1; !a2->field_0_plyr_stage_stats[result][0].field_0_is_stage_unlocked; --result)
    {
        if (result <= 0)
        {
            break;
        }
    }
    return result;
}

MATCH_FUNC(0x4B7800)
u8 Frontend::GetPrevUnlockedStageBonusCode_4B7800(player_stats_0xA4* pStats)
{
    u8 stage = Frontend::GetPrevUnlockedStageIndex_4B77B0(pStats);
    u8 bonus = field_1EB51_num_bonus_stages[stage] - 1;
    while (1)
    {
        if (bonus == 0)
        {
            if (stage == 0)
            {
                return -1;
            }
            stage--;
            bonus = field_1EB51_num_bonus_stages[stage] - 1;
        }
        else
        {
            while (!pStats->field_0_plyr_stage_stats[stage][bonus].field_0_is_stage_unlocked && bonus > 0)
            {
                bonus--;
            }
            if (pStats->field_0_plyr_stage_stats[stage][bonus].field_0_is_stage_unlocked == 1 && bonus > 0)
            {
                break;
            }
        }
    }
    return gLucid_hamilton_67E8E0.EncodeStage_453A40(stage, bonus);
}

EXTERN_GLOBAL(bool, bDoFrontEnd_626B68);

MATCH_FUNC(0x5E53C0)
void __stdcall Frontend::SetInputEnabled_5E53C0(BYTE* a1)
{
    if (bDoFrontEnd_626B68)
    {
        if (gFrontend_67DC84)
        {
            gFrontend_67DC84->field_10D_bInputEnabled = *a1;
        }
    }
    else
    {
        gBurgerKing_67F8B0.field_75344_bInputEnabled = *a1;
    }
}

MATCH_FUNC(0x5D8990)
s32 __stdcall Frontend::GetMaxTextWidth_5D8990(wchar_t* pStr, u16 font_type)
{
    /*If pStr has 1 line, this will return num of characters of pStr. 
    If pStr has multiple lines, this will return the num of characters of the line which have more 
    characters of all lines.*/ 

    wchar_t* pStrIter = pStr;
    s32 current = 0;
    s32 spaceSize = gGtx_0x106C_703DD4->GetSpaceCharWidth_5AA7B0(&font_type);
    s32 biggestLine = 0;
    if (*pStr)
    {
        do
        {
            s16 str_char_type = *pStrIter;
            if (*pStrIter == ' ')
            {
                current += spaceSize;
            }
            else if (str_char_type == '\n')
            {
                if (current > biggestLine)
                {
                    biggestLine = current;
                }
                current = 0; // reset for next line
            }
            else if (str_char_type != '#')
            {
                current += gGtx_0x106C_703DD4->GetFontWidth_5AA760(&font_type, pStrIter);
            }
            ++pStrIter;
        } while (*pStrIter);

        if (current > biggestLine)
        {
            biggestLine = current;
        }
    }
    return biggestLine;
}

MATCH_FUNC(0x4B78B0)
void Frontend::DrawTextFixedWidth_4B78B0(wchar_t* pString, u16 text_xpos, u16 text_ypos, u16 font_type, u16 palette, u16 scale, u16 a7, u8 pStr)
{
    u16 text_xbase;

    if (pStr)
    {
        u16 v9 = 0;
        for (; pString[v9]; ++v9)
        {
            ;
        }
        text_xbase = text_xpos - a7 * (v9 - 1);
    }
    else
    {
        text_xbase = text_xpos;
    }

    wchar_t chr[2] = {0};
    u16 text_xposa = 0;

    for (chr[0] = pString[0]; chr[0]; chr[0] = pString[++text_xposa])
    {
        u16 biggestLine = Frontend::GetMaxTextWidth_5D8990(chr, font_type);
        u16 v16 = (a7 - biggestLine) / 2;
        if ((u16)palette == 0xFFFF)
        {
            DrawText_4B87A0(chr, text_xbase + v16, text_ypos, font_type, scale);
        }
        else
        {
            DrawText_5D8A10(chr, text_xbase + v16, text_ypos, font_type, scale, 8, palette, false, false);
        }
        text_xbase += a7;
    }
}

MATCH_FUNC(0x4B55F0)
void Frontend::DrawMultiplayerScores_4B55F0()
{
    s8 game_mode = gLucid_hamilton_67E8E0.GetMultiplayerGamemode_4C5BC0();
    u8 max_players = gLucid_hamilton_67E8E0.GetMaxPlayers_4C5BF0();
    u8 user_idx = gLucid_hamilton_67E8E0.GetUserPlayerIdx_4C5BE0();

    u8 idx_2 = 0;

    for (u8 curr_plyr_idx = 0; curr_plyr_idx < max_players; ++curr_plyr_idx)
    {
        u16 x_pos;
        u16 y_pos;
        wchar_t Buffer[26];

        if (game_mode == FRAG_GAME_1) //  frags
        {
            s32 frags = (s16)gLucid_hamilton_67E8E0.GetFragsForPlayerIdx_4C5D60(curr_plyr_idx);
            _itow(frags, Buffer, 10);
            x_pos = 550;
            y_pos = 20 * curr_plyr_idx + 170;
        }
        else if (game_mode == POINTS_GAME_2) //  points game
        {
            s32 points = gLucid_hamilton_67E8E0.GetPointsForPlayerIdx_4C5CB0(curr_plyr_idx);
            _itow(points, Buffer, 10);
            x_pos = 550;
            y_pos = 20 * curr_plyr_idx + 170;
        }
        else // tag game
        {
            s32 player_time = gYouthful_einstein_6F8450.GetTime_453AA0(curr_plyr_idx);
            swprintf(Buffer, L"%2d:%02d", player_time / 60, player_time % 60);
            x_pos = 500;
            y_pos = 20 * curr_plyr_idx + 170;
        }

        DrawText_4B87A0(Buffer, x_pos, y_pos, field_11C_normal_font, 1);

        s32 v11 = gLucid_hamilton_67E8E0.GetFragsOnPlayer_4C5D80(user_idx, curr_plyr_idx);
        _itow(v11, Buffer, 10);

        if (game_mode != TAG_GAME_3 && curr_plyr_idx != user_idx)
        {
            x_pos = 550;
            y_pos = 20 * idx_2 + 320;
            DrawText_4B87A0(Buffer, x_pos, y_pos, field_11C_normal_font, 1);
            ++idx_2;
        }
    }
}

MATCH_FUNC(0x4B57B0)
void Frontend::DrawLastAndBestStats_4B57B0(u16 a3, u16 a5)
{
    u16 font_type = field_12A_score_font;
    s32 v4 = gText_0x14_704DFC->field_10_lang_code != 106 ? 14 : 16;
    u8 v39 = gLucid_hamilton_67E8E0.GetMainStageIdx_4C5980();

    if (gText_0x14_704DFC->field_10_lang_code == 106)
    {
        a5 += 5;
    }
    wchar_t* _5B5F90 = gText_0x14_704DFC->Find_5B5F90("last");
    swprintf(tmpBuff_67BD9C, _5B5F90);

    s32 x = Frontend::GetMaxTextWidth_5D8990(tmpBuff_67BD9C, font_type);

    u16 x_pos;
    u16 y_pos;
    DrawText_4B87A0(tmpBuff_67BD9C, (u16)(a3 - x + 494), (u16)(a5 - 15), font_type, 1);

    swprintf(tmpBuff_67BD9C, gText_0x14_704DFC->Find_5B5F90("best"));

    x = Frontend::GetMaxTextWidth_5D8990(tmpBuff_67BD9C, font_type);

    y_pos = a5 - 15;
    DrawText_4B87A0(tmpBuff_67BD9C, (u16)(a3 - x + 624), y_pos, font_type, 1);

    if (gText_0x14_704DFC->field_10_lang_code == 106)
    {
        a5 += 5;
    }

    //  vehicles hijacked

    x_pos = a3;
    y_pos = a5;
    DrawText_4B87A0(gText_0x14_704DFC->Find_5B5F90("carjack"), x_pos, y_pos, font_type, 1);

    swprintf(tmpBuff_67BD9C, L"%d", gLucid_hamilton_67E8E0.GetStatistic_4C59F0(5));
    u16 x_pos_last = a3 + 480;
    Frontend::DrawTextFixedWidth_4B78B0(tmpBuff_67BD9C, x_pos_last, a5, font_type, 10, 1, v4, 1);

    swprintf(tmpBuff_67BD9C, L"%d", *(u32*)&gJolly_poitras_0x2BC0_6FEAC0->field_1800_best_stats[v39].field_0[20]);
    u16 x_pos_best = a3 + 610;
    Frontend::DrawTextFixedWidth_4B78B0(tmpBuff_67BD9C, x_pos_best, a5, font_type, 10, 1, v4, 1);

    //  auto damage cost

    x_pos = a3;
    y_pos = a5 + 20;
    DrawText_4B87A0(gText_0x14_704DFC->Find_5B5F90("car_cst"), x_pos, y_pos, font_type, 1);

    swprintf(tmpBuff_67BD9C, L"$%d", gLucid_hamilton_67E8E0.GetCarDamageCost_4C5A80());
    Frontend::DrawTextFixedWidth_4B78B0(tmpBuff_67BD9C, x_pos_last, y_pos, font_type, 10, 1, v4, 1);

    swprintf(tmpBuff_67BD9C, L"$%d", gJolly_poitras_0x2BC0_6FEAC0->field_1878_best_car_damage_cost[v39]);
    Frontend::DrawTextFixedWidth_4B78B0(tmpBuff_67BD9C, x_pos_best, y_pos, font_type, 10, 1, v4, 1);

    //  civilians run down

    x_pos = a3;
    y_pos = a5 + 40;
    DrawText_4B87A0(gText_0x14_704DFC->Find_5B5F90("run_ovr"), x_pos, y_pos, font_type, 1);

    swprintf(tmpBuff_67BD9C, L"%d", gLucid_hamilton_67E8E0.GetStatistic_4C59F0(6u));
    Frontend::DrawTextFixedWidth_4B78B0(tmpBuff_67BD9C, x_pos_last, y_pos, font_type, 10, 1, v4, 1);

    swprintf(tmpBuff_67BD9C, L"%d", *(u32*)&gJolly_poitras_0x2BC0_6FEAC0->field_1800_best_stats[v39].field_0[24]);
    Frontend::DrawTextFixedWidth_4B78B0(tmpBuff_67BD9C, x_pos_best, y_pos, font_type, 10, 1, v4, 1);

    //  civilians murdered

    x_pos = a3;
    y_pos = a5 + 60;
    DrawText_4B87A0(gText_0x14_704DFC->Find_5B5F90("murder"), x_pos, y_pos, font_type, 1);

    swprintf(tmpBuff_67BD9C, L"%d", gLucid_hamilton_67E8E0.GetStatistic_4C59F0(7u));
    Frontend::DrawTextFixedWidth_4B78B0(tmpBuff_67BD9C, x_pos_last, y_pos, font_type, 10, 1, v4, 1);

    swprintf(tmpBuff_67BD9C, L"%d", *(u32*)&gJolly_poitras_0x2BC0_6FEAC0->field_1800_best_stats[v39].field_0[28]);
    Frontend::DrawTextFixedWidth_4B78B0(tmpBuff_67BD9C, x_pos_best, y_pos, font_type, 10, 1, v4, 1);

    //  lawmen killed

    if (!bIsFrench_67D53C)
    {
        x_pos = a3;
        y_pos = a5 + 80;
        DrawText_4B87A0(gText_0x14_704DFC->Find_5B5F90("cop_kl"), x_pos, y_pos, font_type, 1);

        swprintf(tmpBuff_67BD9C, L"%d", gLucid_hamilton_67E8E0.GetStatistic_4C59F0(8u));
        Frontend::DrawTextFixedWidth_4B78B0(tmpBuff_67BD9C, x_pos_last, y_pos, font_type, 10, 1, v4, 1);

        swprintf(tmpBuff_67BD9C, L"%d", *(u32*)&gJolly_poitras_0x2BC0_6FEAC0->field_1800_best_stats[v39].field_0[32]);
        Frontend::DrawTextFixedWidth_4B78B0(tmpBuff_67BD9C, x_pos_best, y_pos, font_type, 10, 1, v4, 1);
    }

    //  gang members killed

    x_pos = a3;
    y_pos = a5 + 100;
    DrawText_4B87A0(gText_0x14_704DFC->Find_5B5F90("gng_kl"), x_pos, y_pos, font_type, 1);

    swprintf(tmpBuff_67BD9C, L"%d", gLucid_hamilton_67E8E0.GetStatistic_4C59F0(9u));
    Frontend::DrawTextFixedWidth_4B78B0(tmpBuff_67BD9C, x_pos_last, y_pos, font_type, 10, 1, v4, 1);

    swprintf(tmpBuff_67BD9C, L"%d", *(u32*)&gJolly_poitras_0x2BC0_6FEAC0->field_1800_best_stats[v39].field_0[36]);
    Frontend::DrawTextFixedWidth_4B78B0(tmpBuff_67BD9C, x_pos_best, y_pos, font_type, 10, 1, v4, 1);

    //  fugitive factor

    x_pos = a3;
    y_pos = a5 + 120;
    DrawText_4B87A0(gText_0x14_704DFC->Find_5B5F90("evsnrtg"), x_pos, y_pos, font_type, 1);

    swprintf(tmpBuff_67BD9C, L"%d", gLucid_hamilton_67E8E0.GetEvasionRating_4C5AA0());
    Frontend::DrawTextFixedWidth_4B78B0(tmpBuff_67BD9C, x_pos_last, y_pos, font_type, 10, 1, v4, 1);

    swprintf(tmpBuff_67BD9C, L"%d", gJolly_poitras_0x2BC0_6FEAC0->field_1884_best_evasion_rating[v39]);
    Frontend::DrawTextFixedWidth_4B78B0(tmpBuff_67BD9C, x_pos_best, y_pos, font_type, 10, 1, v4, 1);
}

MATCH_FUNC(0x4B0190)
u16 Frontend::GetCenteredXPos_4B0190(wchar_t* pText, u16 fontType, s32 width)
{
    u16 v4;
    if (fontType != 0xFFFF)
    {
        v4 = ((u16)GetMaxTextWidth_5D8990(pText, fontType)) / 2;
    }
    else
    {
        v4 = ((u16)GetMaxTextWidth_5D8990(pText, field_11C_normal_font)) / 2;
    }
    return width - v4;
}

MATCH_FUNC(0x4B7060)
u8 Frontend::GetPreviousUnlockedMainStage_4B7060(u8 a2)
{
    player_stats_0xA4* v2 = GetCurrPlayerStats_4B43E0();
    u8 result = a2;
    if (a2 == 0)
    {
        if (bIsLeftRightLoopEnabled_67DA80)
        {
            result = 2;
            while (!v2->field_0_plyr_stage_stats[result][0].field_0_is_stage_unlocked)
            {
                --result;
            }
        }
        return result;
    }
    else
    {
        a2--;
        return a2;
    }
}

WIP_FUNC(0x4B7270)
u8 Frontend::GetNextUnlockedMainStage_4B7270(char_type main_stage_idx)
{
    WIP_IMPLEMENTED;

    player_stats_0xA4* pStats = GetCurrPlayerStats_4B43E0();
    u8 result = main_stage_idx;
    if (main_stage_idx == 2)
    {
        if (bIsLeftRightLoopEnabled_67DA80)
        {
            return 0;
        }
    }
    else
    {
        result = main_stage_idx + 1;
        while (!pStats->field_0_plyr_stage_stats[result][0].field_0_is_stage_unlocked)
        {
            if (result == 2)
            {
                result = bIsLeftRightLoopEnabled_67DA80 != 0 ? 0 : main_stage_idx;
            }
            else
            {
                ++result;
            }
        }
    }
    return result;
}

MATCH_FUNC(0x4B7490)
bool Frontend::ExistsPreviousMainStage_4B7490()
{
    u8 v2 = gLucid_hamilton_67E8E0.GetMainStageIdx_4C5980();
    bool result = GetPreviousUnlockedMainStage_4B7060(v2) != v2;
    return result;
}

MATCH_FUNC(0x4B74C0)
bool Frontend::ExistsNextMainStage_4B74C0()
{
    char_type curr_main_stage = gLucid_hamilton_67E8E0.GetMainStageIdx_4C5980();
    bool result = (char_type)GetNextUnlockedMainStage_4B7270(curr_main_stage) != curr_main_stage;
    return result;
}

MATCH_FUNC(0x4B7550)
void Frontend::UpdateMainStageArrows_4B7550()
{
    MenuPage_0xBCA* pBorg = &field_136_menu_pages_array[field_132_f136_idx];
    u8 main_stage_idx = gLucid_hamilton_67E8E0.GetMainStageIdx_4C5980();
    swprintf(tmpBuff_67BD9C, L"%d", main_stage_idx + 1);
    wcsncpy(pBorg->field_518_elements_array[2].field_6_element_name_str, tmpBuff_67BD9C, 0x32u);

    if (ExistsPreviousMainStage_4B7490())
    {
        pBorg->field_518_elements_array[4].field_1_is_it_displayed = 1;
        field_1EB4C_has_prev_main_stage = 1;
    }
    else
    {
        pBorg->field_518_elements_array[4].field_1_is_it_displayed = 0;
        field_1EB4C_has_prev_main_stage = 0;
    }

    if (ExistsNextMainStage_4B74C0())
    {
        pBorg->field_518_elements_array[5].field_1_is_it_displayed = 1;
        field_1EB4D_has_next_main_stage = 1;
    }
    else
    {
        pBorg->field_518_elements_array[5].field_1_is_it_displayed = 0;
        field_1EB4D_has_next_main_stage = 0;
    }
}

MATCH_FUNC(0x4B6FF0)
bool Frontend::ChangeMainStageToPrevious_4B6FF0()
{
    u8 main_stage_idx = gLucid_hamilton_67E8E0.GetMainStageIdx_4C5980();
    u8 old_main_stage_idx = main_stage_idx;
    main_stage_idx = GetPreviousUnlockedMainStage_4B7060(main_stage_idx);
    gLucid_hamilton_67E8E0.SetMainStageIdx_4C58F0(main_stage_idx);
    field_1EB3A_selected_main_stage[gLucid_hamilton_67E8E0.GetPlySlotIdx_4C59B0()] = main_stage_idx;
    UpdateMainStageArrows_4B7550();
    bool result = (old_main_stage_idx != main_stage_idx);
    return result;
}

MATCH_FUNC(0x4B42B0)
void Frontend::StripPlayerNameToCurrLength_4B42B0()
{
    u16 name_length = field_C9B2_curr_plyr_name_length;
    for (u16 i = name_length; i < 9; i++)
    {
        field_C9A0_curr_plyr_name[i] = 0;
    }
}

// https://decomp.me/scratch/2DKTF
MATCH_FUNC(0x4B7120)
char_type Frontend::GetPreviousUnlockedBonusStage_4B7120(u8 a2)
{
    player_stats_0xA4* player_stats = Frontend::GetCurrPlayerStats_4B43E0();

    u8 main_stage_idx;
    u8 bonus_stage_idx;
    gLucid_hamilton_67E8E0.DecodeStage_453A60(a2, &main_stage_idx, &bonus_stage_idx);

    u8 main_og = main_stage_idx;
    u8 bonus_og = bonus_stage_idx;
    u8 bFirstIteration = true;

    while (!player_stats->field_0_plyr_stage_stats[main_stage_idx][bonus_stage_idx].field_0_is_stage_unlocked || bFirstIteration)
    {
        bFirstIteration = false;
        if (bonus_stage_idx == 1)
        {
            do
            {
                if (main_stage_idx == 0)
                {
                    if (bIsLeftRightLoopEnabled_67DA80)
                    {
                        main_stage_idx = field_1EB50_num_main_stages - 1;
                        bonus_stage_idx = field_1EB51_num_bonus_stages[main_stage_idx] - 1;
                    }
                    else
                    {
                        main_stage_idx = main_og;
                        bonus_stage_idx = bonus_og;
                    }
                }
                else
                {
                    --main_stage_idx;
                    bonus_stage_idx = field_1EB51_num_bonus_stages[main_stage_idx] - 1;
                }
            } while (bonus_stage_idx == 0);
        }
        else
        {
            --bonus_stage_idx;
        }
    }
    return gLucid_hamilton_67E8E0.EncodeStage_453A40(main_stage_idx, bonus_stage_idx);
}

MATCH_FUNC(0x4B7610)
void Frontend::UpdateBonusStageArrows_4B7610()
{
    MenuPage_0xBCA* pPage = &field_136_menu_pages_array[field_132_f136_idx];
    u8 v3 = gLucid_hamilton_67E8E0.GetStage_4C5990();
    u8 v4;
    u8 v5;
    gLucid_hamilton_67E8E0.DecodeStage_453A60(v3, &v4, &v5);
    if (v3 == 0xFF)
    {
        pPage->field_4_options_array[4].field_1_is_unlocked = 0;
        pPage->field_B8A[4].field_4_is_option_unlocked = 0;
        pPage->field_518_elements_array[3].field_1_is_it_displayed = 0;
        pPage->field_518_elements_array[1].field_1_is_it_displayed = 0;
        pPage->field_518_elements_array[6].field_1_is_it_displayed = 0;
        pPage->field_518_elements_array[7].field_1_is_it_displayed = 0;
    }
    else
    {
        pPage->field_4_options_array[4].field_1_is_unlocked = 1;
        pPage->field_B8A[4].field_4_is_option_unlocked = 1;
        pPage->field_518_elements_array[3].field_1_is_it_displayed = 1;
        pPage->field_518_elements_array[1].field_1_is_it_displayed = 1;
        pPage->field_518_elements_array[6].field_1_is_it_displayed = 1;
        pPage->field_518_elements_array[7].field_1_is_it_displayed = 1;
        if (ExistsPreviousBonusStage_4B74F0())
        {
            pPage->field_518_elements_array[6].field_1_is_it_displayed = 1;
            field_1EB4E_has_prev_bonus_stage = 1;
        }
        else
        {
            pPage->field_518_elements_array[6].field_1_is_it_displayed = 0;
            field_1EB4E_has_prev_bonus_stage = 0;
        }

        if (ExistsNextBonusStage_4B7520())
        {
            pPage->field_518_elements_array[7].field_1_is_it_displayed = 1;
            field_1EB4F_has_next_bonus_stage = 1;
        }
        else
        {
            pPage->field_518_elements_array[7].field_1_is_it_displayed = 0;
            field_1EB4F_has_next_bonus_stage = 0;
        }

        swprintf(gTmpWideStr_67C7D8, L"%c", 3 * v4 + v5 + 64);
        wcsncpy(pPage->field_518_elements_array[3].field_6_element_name_str, gTmpWideStr_67C7D8, 0x32u);
    }
}

MATCH_FUNC(0x4B70B0)
bool Frontend::ChangeBonusStageToPrevious_4B70B0()
{
    s8 v3 = gLucid_hamilton_67E8E0.GetStage_4C5990();
    s8 v4 = v3;
    v3 = GetPreviousUnlockedBonusStage_4B7120(v3);
    gLucid_hamilton_67E8E0.SetStage_4C5900(v3);
    field_1EB42_selected_bonus_stage[gLucid_hamilton_67E8E0.GetPlySlotIdx_4C59B0()] = v3;
    UpdateBonusStageArrows_4B7610();
    bool result = v4 != v3;
    return result;
}

MATCH_FUNC(0x4B74F0)
bool Frontend::ExistsPreviousBonusStage_4B74F0()
{
    char_type v2 = gLucid_hamilton_67E8E0.GetStage_4C5990();
    bool result = GetPreviousUnlockedBonusStage_4B7120(v2) != v2;
    return result;
}

// https://decomp.me/scratch/kyxJQ
MATCH_FUNC(0x4B7360)
char_type Frontend::GetNextUnlockedBonusStage_4B7360(u8 a2)
{
    player_stats_0xA4* player_stats = GetCurrPlayerStats_4B43E0();

    u8 main_stage_idx;
    u8 bonus_stage_idx;
    gLucid_hamilton_67E8E0.DecodeStage_453A60(a2, &main_stage_idx, &bonus_stage_idx);

    u8 og_main_stage_idx = main_stage_idx;
    u8 og_bonus_stage_idx = bonus_stage_idx;

    u8 bFirstIteration = true;

    while (!player_stats->field_0_plyr_stage_stats[main_stage_idx][bonus_stage_idx].field_0_is_stage_unlocked || bFirstIteration)
    {
        bFirstIteration = false;
        if (bonus_stage_idx == field_1EB51_num_bonus_stages[main_stage_idx] - 1)
        {
            bonus_stage_idx = 1;
            if (main_stage_idx == field_1EB50_num_main_stages - 1)
            {
                if (bIsLeftRightLoopEnabled_67DA80)
                {
                    main_stage_idx = 0;
                }
                else
                {
                    main_stage_idx = og_main_stage_idx;
                    bonus_stage_idx = og_bonus_stage_idx;
                }
            }
            else
            {
                main_stage_idx++;
            }

            while (field_1EB51_num_bonus_stages[main_stage_idx] == 1)
            {
                if (main_stage_idx == field_1EB50_num_main_stages - 1)
                {
                    if (bIsLeftRightLoopEnabled_67DA80)
                    {
                        main_stage_idx = 0;
                    }
                    else
                    {
                        main_stage_idx = og_main_stage_idx;
                        bonus_stage_idx = og_bonus_stage_idx;
                    }
                }
                else
                {
                    main_stage_idx++;
                }
            }
        }
        else
        {
            bonus_stage_idx++;
        }
    }

    return gLucid_hamilton_67E8E0.EncodeStage_453A40(main_stage_idx, bonus_stage_idx);
}

MATCH_FUNC(0x4B7520)
bool Frontend::ExistsNextBonusStage_4B7520()
{
    char_type v2 = gLucid_hamilton_67E8E0.GetStage_4C5990();
    bool result = GetNextUnlockedBonusStage_4B7360(v2) != v2;
    return result;
}

MATCH_FUNC(0x4B72F0)
bool Frontend::ChangeBonusStageToNext_4B72F0()
{
    char_type v3 = gLucid_hamilton_67E8E0.GetStage_4C5990();
    char_type v4 = v3;
    v3 = GetNextUnlockedBonusStage_4B7360(v3);
    gLucid_hamilton_67E8E0.SetStage_4C5900(v3);
    field_1EB42_selected_bonus_stage[gLucid_hamilton_67E8E0.GetPlySlotIdx_4C59B0()] = v3;
    UpdateBonusStageArrows_4B7610();
    bool result = v4 != v3;
    return result;
}

MATCH_FUNC(0x4B7200)
bool Frontend::ChangeMainStageToNext_4B7200()
{
    char_type main_stage_idx = gLucid_hamilton_67E8E0.GetMainStageIdx_4C5980();
    char_type old_main_stage_idx = main_stage_idx;
    main_stage_idx = GetNextUnlockedMainStage_4B7270(main_stage_idx);
    gLucid_hamilton_67E8E0.SetMainStageIdx_4C58F0(main_stage_idx);
    field_1EB3A_selected_main_stage[gLucid_hamilton_67E8E0.GetPlySlotIdx_4C59B0()] = main_stage_idx;
    UpdateMainStageArrows_4B7550();
    bool result = old_main_stage_idx != main_stage_idx;
    return result;
}

MATCH_FUNC(0x4B4EC0)
void Frontend::sub_4B4EC0()
{
    char_type FileName[256];
    {
        u8 plySlotIdx = gLucid_hamilton_67E8E0.GetPlySlotIdx_4C59B0();
        GetPlySlotSvgName_4B51D0(plySlotIdx, FileName);
    }
    File::Global_Open_4A7060(FileName);

    svg_stru svg;
    {
        u32 len = sizeof(svg_stru);
        File::Global_Read_4A71C0(&svg, len);
    }

    File::Global_Close_4A70C0();

    u8 main_stage = svg.field_4B_last_saved_stage;
    u8 codified_stages = svg.field_4C_last_saved_bonus_stage_code;
    u8 bCodified = svg.field_4D_last_saved_is_bonus;
    u8 bonus_stage;

    if (!bCodified)
    {
        bonus_stage = 0;
    }
    else
    {
        gLucid_hamilton_67E8E0.DecodeStage_453A60(codified_stages, &main_stage, &bonus_stage);
    }

    char_type path[256];
    strcpy(path, "data\\");
    strcat(path, field_C9E8_blocks[main_stage][bonus_stage].field_0_debug_str);
    if (strcmp(svg.field_0_map_name, path))
    {
        FatalError_4A38C0(Gta2Error::GmpFilenameMismatch, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 5254);
    }

    strcpy(path, "data\\");
    strcat(path, field_C9E8_blocks[main_stage][bonus_stage].field_100_map_name);
    if (strcmp(svg.field_19_style_name, path))
    {
        FatalError_4A38C0(Gta2Error::StyFilenameMismatch, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 5262);
    }

    strcpy(path, "data\\");
    strcat(path, field_C9E8_blocks[main_stage][bonus_stage].field_200_sty_name);
    if (strcmp(svg.field_32_script_name, path))
    {
        FatalError_4A38C0(Gta2Error::ScrFilenameMismatch, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 5270);
    }

    gLucid_hamilton_67E8E0.DebugStr_4C58D0(FileName);
    gLucid_hamilton_67E8E0.SetMapName_4C5870(svg.field_0_map_name);
    gLucid_hamilton_67E8E0.SetStyleName_4C5890(svg.field_19_style_name);
    gLucid_hamilton_67E8E0.SetScriptName_4C58B0(svg.field_32_script_name);
    gLucid_hamilton_67E8E0.SetMainStageIdx_4C58F0(main_stage);
    gLucid_hamilton_67E8E0.SetStage_4C5900(codified_stages);
    gLucid_hamilton_67E8E0.SetBonusStage_4C5910(bCodified);
}

MATCH_FUNC(0x4B6070)
MenuPage_0xBCA::MenuPage_0xBCA()
{
    field_0_number_of_options = 0;
    field_2_number_of_elements = 0;
    field_BC6_current_option_idx = 0;
    field_BC8_default_option_idx = 0;
}

MATCH_FUNC(0x4B6110)
MenuPage_0xBCA::~MenuPage_0xBCA()
{
    field_0_number_of_options = 0;
    field_2_number_of_elements = 0;
    field_BC6_current_option_idx = 0;
    field_BC8_default_option_idx = 0;
}

MATCH_FUNC(0x4B61B0)
bool MenuPage_0xBCA::SelectPrevOption_4B61B0()
{
    u16 oldIdx = field_BC6_current_option_idx;
    do
    {
        if (!field_BC6_current_option_idx)
        {
            field_BC6_current_option_idx = field_0_number_of_options - 1;
        }
        else
        {
            field_BC6_current_option_idx--;
        }
    } while (!field_B8A[field_BC6_current_option_idx].field_4_is_option_unlocked);
    return oldIdx != field_BC6_current_option_idx ? true : false;
}

MATCH_FUNC(0x4B6200)
bool MenuPage_0xBCA::SelectNextOption_4B6200()
{
    u16 oldIdx = field_BC6_current_option_idx;
    do
    {
        if (field_BC6_current_option_idx == field_0_number_of_options - 1)
        {
            field_BC6_current_option_idx = 0;
        }
        else
        {
            field_BC6_current_option_idx++;
        }
    } while (!field_B8A[field_BC6_current_option_idx].field_4_is_option_unlocked);
    return oldIdx != field_BC6_current_option_idx ? true : false;
}

MATCH_FUNC(0x4B63E0)
menu_element_0x6E::menu_element_0x6E()
{
    field_0_element_type = NULL_TYPE_0;
    field_2_xpos = 0;
    field_4_ypos = 0;
    field_1_is_it_displayed = 1;
    wcscpy(field_6_element_name_str, gEmptyWStr_67DC8C);
    field_6A_font_type = -1;
    field_6C_font_palette = -1;
}

MATCH_FUNC(0x4B6420)
menu_element_0x6E::~menu_element_0x6E()
{
    field_1_is_it_displayed = 1;
    field_0_element_type = NULL_TYPE_0;
    field_2_xpos = 0;
    field_4_ypos = 0;
    field_6A_font_type = -1;
    field_6C_font_palette = -1;
}

MATCH_FUNC(0x4B6290)
menu_option_0x82::menu_option_0x82()
{
    field_6A_font_type = -1;
    field_6C_palette = -1;
    field_0_option_type = NULL_TYPE_0;
    field_1_is_unlocked = 1;
    field_2_x_pos = 0;
    field_4_y_pos = 0;
    field_6E_horizontal_selected_idx = 0;
    field_70 = 0;

    for (s32 i = 0; i < GTA2_COUNTOF(field_72_horizontal_idx_enabled); i++) // or is it a u32 ??
    {
        field_72_horizontal_idx_enabled[i] = 0;
    }

    wcscpy(field_6_option_name_str, gEmptyWStr_67DC8C);
    field_7E_horizontal_max_idx = 0;
    field_80_menu_page_target = 0;
}

MATCH_FUNC(0x4B62F0)
menu_option_0x82::~menu_option_0x82()
{
    field_0_option_type = NULL_TYPE_0;
    field_1_is_unlocked = 1;
    field_2_x_pos = 0;
    field_4_y_pos = 0;
    field_6A_font_type = -1;
    field_6C_palette = -1;
    field_6E_horizontal_selected_idx = 0;
    field_70 = 0;
    field_7E_horizontal_max_idx = 0;
    field_80_menu_page_target = 0;
}

MATCH_FUNC(0x4B6330)
bool menu_option_0x82::SelectNextHorizontalIdx_4B6330()
{
    BYTE tmp = bIsLeftRightLoopEnabled_67DA80;
    u16 old_count = field_6E_horizontal_selected_idx;
    u16 new_count = old_count;
    char_type bFound = 0;
    // Reading field_6E through a reference stops VC6 from reusing old_count's register for it in the
    // loop condition; with no register left to hoist it into, it is reloaded each pass like the original.
    u16& selected_idx = field_6E_horizontal_selected_idx;
    do
    {
        new_count++;
        if (new_count > field_7E_horizontal_max_idx)
        {
            if (tmp)
            {
                new_count = 0;
            }
            else
            {
                new_count += 0xFFFF;
            }
        }

        if (field_72_horizontal_idx_enabled[new_count])
        {
            bFound = 1;
        }

    } while (new_count != selected_idx && !bFound);

    field_6E_horizontal_selected_idx = new_count;

    return old_count != new_count ? true : false;
}

WIP_FUNC(0x4B6390)
bool menu_option_0x82::SelectPrevHorizontalIdx_4B6390()
{
    WIP_IMPLEMENTED;

    u16 oldCount = field_6E_horizontal_selected_idx;
    u16 new_count = oldCount;
    char_type bFound = 0;
    do
    {

        if (new_count == 0)
        {
            if (bIsLeftRightLoopEnabled_67DA80)
            {
                new_count = field_7E_horizontal_max_idx;
            }
        }

        else
        {
            --new_count; // add     eax, 0FFFFh
        }

        if (field_72_horizontal_idx_enabled[new_count])
        {
            bFound = 1;
        }

    } while (new_count != field_6E_horizontal_selected_idx && !bFound); // 6E not reloaded

    field_6E_horizontal_selected_idx = new_count;

    return oldCount != new_count ? true : false;
}

MATCH_FUNC(0x4B6260)
kind_beaver_6::kind_beaver_6()
{
    field_0 = 0;
    field_2 = 0;
    field_4_is_option_unlocked = 1;
}

MATCH_FUNC(0x4B6280)
kind_beaver_6::~kind_beaver_6()
{
    field_0 = 0;
    field_2 = 0;
    field_4_is_option_unlocked = 1;
}

MATCH_FUNC(0x4B6440)
admiring_euler_4::admiring_euler_4()
{
    field_0_save_exists = false;
    field_1_last_saved_stage = 0;
    field_2_last_saved_bonus_stage_code = 0;
    field_3_last_saved_is_bonus = 0;
}

MATCH_FUNC(0x4B6450)
admiring_euler_4::~admiring_euler_4()
{
    field_0_save_exists = false;
    field_1_last_saved_stage = 0;
    field_2_last_saved_bonus_stage_code = 0;
    field_3_last_saved_is_bonus = 0;
}

MATCH_FUNC(0x4B6480)
void admiring_euler_4::LoadPlySlotSvg_4B6480(const char_type* FileName)
{
    File::Global_Open_4A7060(FileName);

    svg_stru svg;
    u32 len = sizeof(svg_stru);
    File::Global_Read_4A71C0(&svg, len);

    File::Global_Close_4A70C0();

    field_0_save_exists = true;
    field_1_last_saved_stage = svg.field_4B_last_saved_stage;
    field_2_last_saved_bonus_stage_code = svg.field_4C_last_saved_bonus_stage_code;
    field_3_last_saved_is_bonus = svg.field_4D_last_saved_is_bonus;
}
