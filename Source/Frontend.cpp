#include "Frontend.hpp"
#include "credits_line_category.hpp"
#include "Bink.hpp"
#include "InputRecorder_67F8B0.hpp"
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
#include "SoundObject.hpp"
#include "input.hpp"
#include "PlyDat.hpp"
#include "KeyboardLayout.hpp"
#include "GameSession.hpp"
#include "KanjiFont.hpp"
#include "registry.hpp"
#include "root_sound.hpp"
#include "TextureCache.hpp"
#include "TextTable.hpp"
#include "winmain.hpp"
#include "TagGame.hpp"
#include <io.h>
#include <stdio.h>
#include <wchar.h>

#pragma comment(lib, "dxguid.lib")

void Start_GTA2Manager();

DEFINE_GLOBAL(Frontend*, gFrontend, 0x67DC84);
DEFINE_GLOBAL_INIT(u32, counter_706C4C, 0, 0x706C4C);
DEFINE_GLOBAL_INIT(Fix16, kFpZero_67D930, Fix16(0, 0), 0x67D930);
u16 gTableSize_61FF20 = 25; // Note is constant but can't be marked const
DEFINE_GLOBAL_ARRAY(wchar_t, gEmptyWStr, 2, 0x67DC8C);
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpOne, Fix16(1), 0x67D9FC);
DEFINE_GLOBAL(s16, font_type_703C14, 0x703C14);
DEFINE_GLOBAL(s16, gFrontendHeadingFont, 0x703C3C);
DEFINE_GLOBAL(s16, gFrontendNormalFont, 0x703D0C);
DEFINE_GLOBAL(s16, gFrontendSelectedFont, 0x703C16);
DEFINE_GLOBAL(s16, gFrontendFont6, 0x703C8C);
DEFINE_GLOBAL(s16, gFrontendPlayMenuScoreFont, 0x703C8A);
DEFINE_GLOBAL(s16, gFrontendScoreFont, 0x703BE2);
DEFINE_GLOBAL(s16, gFrontendAreaCompleteFont, 0x703B88);
DEFINE_GLOBAL(s16, gFrontendFont12, 0x703DAC);
DEFINE_GLOBAL(s16, gFrontendTitleFont, 0x703B9C);
DEFINE_GLOBAL_INIT(Ang16, kAngZero_67DA70, Ang16(0), 0x67DA70);
DEFINE_GLOBAL_INIT(Fix16, kFpOne_67D934, Fix16(1), 0x67D934);
DEFINE_GLOBAL_ARRAY(wchar_t, tmpBuff, 640, 0x67BD9C);
DEFINE_GLOBAL(u8, bIsLeftRightLoopEnabled, 0x67DA80);
DEFINE_GLOBAL_ARRAY(wchar_t, gTmpWideStr, 640, 0x67C7D8);
DEFINE_GLOBAL(bool, gCheatOnlyMuggerPeds, 0x67D5A4);
DEFINE_GLOBAL(bool, gCheatUnlimitedElectroGun, 0x67D4F7);
DEFINE_GLOBAL(bool, gCheatAllGangMaxRespect, 0x67D587);
DEFINE_GLOBAL(bool, gCheatOnlyElvisPeds, 0x67D4ED);
DEFINE_GLOBAL(bool, gCheatNakedPeds, 0x67D5E8);
DEFINE_GLOBAL(bool, gCheatGetBasicWeaponsMaxAmmo, 0x67D545);
DEFINE_GLOBAL(bool, gCheatGet99Lives, 0x67D4F1);
DEFINE_GLOBAL(bool, gCheatGetPlayerPoints, 0x67D4C8);
DEFINE_GLOBAL(bool, gCheatUnlimitedFlameThrower, 0x67D6CC);
DEFINE_GLOBAL(bool, gCheatUnlimitedJailCard, 0x67D4F6);
DEFINE_GLOBAL(bool, gCheatGet10MillionMoney, 0x67D6CE);
DEFINE_GLOBAL(bool, gCheat10xMultiplier, 0x67D589);
DEFINE_GLOBAL(bool, gCheatUnlockThreeLevels, 0x67D6CB);
DEFINE_GLOBAL(bool, gCheatUnlockLevelsOneAndTwo, 0x67D584);
DEFINE_GLOBAL(bool, gCheatUnlockAllLevels, 0x67D538);
DEFINE_GLOBAL(bool, gCheatUnlimitedDoubleDamage, 0x67D57C);
DEFINE_GLOBAL(bool, gCheatInvisibility, 0x67D539);
DEFINE_GLOBAL(bool, gCheatMiniCars, 0x67D6C8);

s32 sCheatHashSecret_61F0A8[8] = {829, 761, 23, 641, 43, 809, 677, 191};

MATCH_FUNC(0x4AE010)
LPCSTR __stdcall FreeLoader::GetRegDword(HKEY hKey, LPCSTR lpValueName, LPCSTR default_value)
{
    DWORD Type = 4;
    if (!RegQueryValueExA(hKey, lpValueName, NULL, &Type, reinterpret_cast<LPBYTE>(&lpValueName), &Type) == 0)
    {
        return default_value;
    }
    else
    {
        return lpValueName;
    }
}

MATCH_FUNC(0x4AE0F0)
s32 __stdcall FreeLoader::GetCityInstalled()
{
    HKEY phkResult;
    RegOpenKeyA(HKEY_LOCAL_MACHINE, "Software\\freeloader.com\\GTA2", &phkResult);
    s32 city_installed = reinterpret_cast<s32>(FreeLoader::GetRegDword(phkResult, "CityInstalled", reinterpret_cast<LPCSTR>(-1)));
    RegCloseKey(phkResult);
    return city_installed;
}

MATCH_FUNC(0x4AE170)
void __cdecl FreeLoader::MessageBoxPrintf(UINT uType, LPCSTR lpCaption, LPCSTR lpFormat, ...)
{
    char_type Buffer[1024];
    va_list args;
    va_start(args, lpFormat);
    wvsprintfA(Buffer, lpFormat, args);
    lstrcatA(Buffer, "\r\n");
    OutputDebugStringA(Buffer);
    MessageBoxA(NULL, Buffer, lpCaption, uType);
}

MATCH_FUNC(0x4AE1F0)
EXPORT bool __stdcall FreeLoader::CheckCityInstalled(u8 city_idx)
{
    if (city_idx > FreeLoader::GetCityInstalled())
    {
        ShowWindow(gHwnd, SW_SHOWMINNOACTIVE);
        PostMessageA(gHwnd, WM_ACTIVATE, 0, 0);
        tagMSG Msg;

        while (PeekMessageA(&Msg, NULL, 0, 0, 1u))
        {
            TranslateMessage(&Msg);
            DispatchMessageA(&Msg);
            Sleep(0xAu);
        }
        HANDLE hMutex = OpenMutexA(0x1F0001u, 0, "WEBL_COOP_MUTEX");
        if (!hMutex)
        {
            ShellExecuteA(NULL, NULL, "WebLaunch.exe", NULL, gWorkingDir, 1);
        }
        else
        {
            CloseHandle(hMutex);
        }

        return false;
    }
    return true;
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

DIDATAFORMAT gKeyboardDataFormat = {24u, 16u, DIDF_RELAXIS, 256u, 256u, gKeyboardObjectDataFormats_5E9110};
DIDATAFORMAT gInputDeviceFormat_601A6C = {24u, 16u, DIDF_ABSAXIS, 80u, 44u, gInputDeviceObjectDataFormats_5EA110};
DIDATAFORMAT stru_601A84 = {24u, 16u, DIDF_RELAXIS, 16u, 7u, gMouseObjectDataFormats_5EA3D0};

struct TgaInfo
{
    char_type mTgaName[128]; // 0x0
    s32 mLen; // 0x80
    s32 mImg; // 0x84
};

DEFINE_GLOBAL_ARRAY_INIT(
    TgaInfo,
    tgaArray,
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

MATCH_FUNC(0x5D98D0)
EXPORT s32 IsVideoFullScreen()
{
    return gVidSys->field_40_full_screen;
}

MATCH_FUNC(0x5D9910)
EXPORT s32 __stdcall SetGamma(s32 gamma)
{
    f32 gamma_f = static_cast<f32>(gamma * 0.1);
    if (gVidSys)
    {
        s32 result = pVid_SetGamma(gVidSys, gamma_f, gamma_f, gamma_f);
        sprintf(gTmpBuffer, "gammma(%d) : %d", gamma, result);
        gErrorLog_67C530.Write(gTmpBuffer);
        return result;
    }
    return gamma;
}

MATCH_FUNC(0x5D9970)
EXPORT void __stdcall ReapplySavedGamma()
{
    const s32 gamma = gRegistry.Get_Screen_Setting("gamma", 10);
    if (counter_706C4C > 0)
    {
        if (!SetGamma(gamma))
        {
            counter_706C4C = 0;
        }
        else
        {
            --counter_706C4C;
        }
    }
}

DEFINE_GLOBAL(SoundObject, snd1, 0x67D818);
DEFINE_GLOBAL(SoundObject, snd2, 0x67D6F8);

MATCH_FUNC(0x4B4C60)
void Frontend::LoadStringsFromStage(u16 mainBlockIdx, u16 bounusBlockIdx, char* pDebugStr, char* pMapName, char* pStyName)
{
    strcpy(pDebugStr, mBlocks[mainBlockIdx][bounusBlockIdx].mGmpName);
    strcpy(pMapName, mBlocks[mainBlockIdx][bounusBlockIdx].mStyName);
    strcpy(pStyName, mBlocks[mainBlockIdx][bounusBlockIdx].mScrName);
}

MATCH_FUNC(0x4B4BC0)
void Frontend::StoreStringsForStage(u16 mainBlockIdx, u16 bounusBlockIdx, const char* pDebugStr, const char* pMapName, const char* pStyName)
{
    strcpy(mBlocks[mainBlockIdx][bounusBlockIdx].mGmpName, pDebugStr);
    strcpy(mBlocks[mainBlockIdx][bounusBlockIdx].mStyName, pMapName);
    strcpy(mBlocks[mainBlockIdx][bounusBlockIdx].mScrName, pStyName);
}

MATCH_FUNC(0x4ACFA0)
void __stdcall Frontend::create()
{
    if (!gFrontend)
    {
        gFrontend = new Frontend();
    }

    if (!bSkip_audio)
    {
        snd1.mObjectType = 0;
        snd1.mbStatus = false;
        snd2.mObjectType = SoundObjectTypeEnum::Frontend_2;
        snd2.mpAny.pNextFree = &snd1;
        snd2.mbStatus = false;
        snd2.mSoundEntry = gRoot_sound.AddSoundObject(&snd2);
        gRoot_sound.LoadStyle("data\\fstyle.sty");
        gRoot_sound.Set3DSound(0);
    }

    Bink::Reset();
}

MATCH_FUNC(0x4AD070)
void __stdcall Frontend::destroy()
{
    if (!bSkip_audio && snd2.mSoundEntry)
    {
        gRoot_sound.FreeSoundEntry(snd2.mSoundEntry);
        snd2.mSoundEntry = 0;
    }

    if (gFrontend)
    {
        GTA2_DELETE_AND_NULL(gFrontend);
    }

    Bink::CloseSlot1();
    Bink::CloseSlot2();
}

// 9.6f 0x453AB0: index of the tag game player with the longest time (that hasn't quit)
inline s32 TagGame::GetLeaderIdx()
{
    s32 leader_idx = -1;
    s32 leader_time = -1;
    if (IsTagGame())
    {
        for (s32 i = 0; i < MAX_PLAYERS; i++)
        {
            if (mItTimeSecs[i] > leader_time && !mbHasQuit[i])
            {
                leader_time = mItTimeSecs[i];
                leader_idx = i;
            }
        }
        return leader_idx;
    }
    return gGameSession.GetWinnerIdx();
}

MATCH_FUNC(0x4B3170)
void Frontend::ChangeMenuPage(u16 menu_page_idx)
{
    u8 bonus_count;
    u8 option_idx;
    u8 i;
    u8 saved_main_stage;
    u8 saved_bonus_code;
    u8 saved_is_bonus;
    u8 main_stage_idx;
    u8 bonus_stage_idx;
    u8 game_mode;
    u8 opponent_idx;
    u8 player;
    u8 num_players;
    s32 user_idx;
    wchar_t player_name[50];
    wchar_t quit_name[64];

    PlySlot* pStats = GetCurrPlayerStats();
    mCurrentPageIdx = menu_page_idx;

    if (menu_page_idx == MENUPAGE_PARENTAL_CONTROL)
    {
        mState = FrontendState::User_Typing_Password_5;
        mPasswordLength = 0;
        mWrongPasswordShown = false;
        StripPasswordToCurrLength();
        mKeyHeld = true;
        mLastKey = DIK_RETURN;
        mKeyRepeatTimer = 5;
    }
    else if (menu_page_idx == MENUPAGE_CREDITS)
    {
        mCreditsScrollTimer = 0;
        mCreditsYpos = Fix16(410);
        mCreditsLineIdx = 0;
        mKeyHeld = true;
    }
    else if (menu_page_idx == MENUPAGE_AREA_COMPLETE)
    {
        main_stage_idx = gGameSession.GetMainStageIdx();
        bonus_stage_idx = 0;
        bonus_count = gGameSession.GetLevelFinishBonusType();
        if (gGameSession.get_secret_tokens_collected() == 50)
        {
            bonus_count = 3;
        }

        for (i = 1; i <= bonus_count; i++)
        {
            if (i < mNumBonusStages[main_stage_idx])
            {
                gPlyDat->UnlockStage(main_stage_idx, i);
            }
        }

        if (main_stage_idx == mNumMainStages - 1)
        {
            mMenuPagesArray[3].mOptionsArray[0].mIsUnlocked = 0;
            mMenuPagesArray[3].mOptionStates[0].mIsOptionUnlocked = 0;
        }
        else
        {
            gPlyDat->UnlockStage(main_stage_idx + 1, 0);
            mMenuPagesArray[3].mOptionsArray[0].mIsUnlocked = 1;
            mMenuPagesArray[3].mOptionStates[0].mIsOptionUnlocked = 1;
        }

        mMenuPagesArray[3].mOptionsArray[3].mIsUnlocked = 0;
        mMenuPagesArray[3].mOptionStates[3].mIsOptionUnlocked = 0;
        for (i = 1; i < 4; i++)
        {
            if (pStats->mPlyrStageStats[main_stage_idx][i].mIsStageUnlocked && i < mNumBonusStages[main_stage_idx])
            {
                mMenuPagesArray[3].mOptionsArray[3].mIsUnlocked = 1;
                mMenuPagesArray[3].mOptionStates[3].mIsOptionUnlocked = 1;
            }
        }
    }
    else if (menu_page_idx == MENUPAGE_BONUS_AREA)
    {
        gGameSession.DecodeStage(gGameSession.GetStage(), &main_stage_idx, &bonus_stage_idx);
        swprintf(tmpBuff, L"%d", pStats->mPlyrStageStats[main_stage_idx][bonus_stage_idx].mStageLatestScore);
        wcsncpy(mMenuPagesArray[6].mElementsArray[2].mElementNameStr, tmpBuff, 50);
        if (gGameSession.IsStartedFromPlayBonusMenu() || main_stage_idx >= mNumMainStages - 1 ||
            !pStats->mPlyrStageStats[main_stage_idx + 1][0].mIsStageUnlocked)
        {
            mMenuPagesArray[6].mOptionsArray[1].mIsUnlocked = 0;
            mMenuPagesArray[6].mOptionStates[1].mIsOptionUnlocked = 0;
        }
        else
        {
            mMenuPagesArray[6].mOptionsArray[1].mIsUnlocked = 1;
            mMenuPagesArray[6].mOptionStates[1].mIsOptionUnlocked = 1;
        }
    }

    if (menu_page_idx == MENUPAGE_AREA_COMPLETE || menu_page_idx == MENUPAGE_DEAD || menu_page_idx == MENUPAGE_RESULTS_PLAYER_QUIT)
    {
        // the option to check is "continue" (1) on the area complete page, and 0 on the dead/quit pages
        option_idx = (menu_page_idx == MENUPAGE_DEAD || menu_page_idx == MENUPAGE_RESULTS_PLAYER_QUIT) ? 0 : 1;
        game_mode = gGameSession.GetPlySlotIdx();
        saved_main_stage = mPlySlots[game_mode].mLastSavedStage;
        saved_bonus_code = mPlySlots[game_mode].mLastSavedBonusStageCode;
        saved_is_bonus = mPlySlots[game_mode].mLastSavedIsBonus;
        if (!gGameSession.IsBonusStage())
        {
            main_stage_idx = gGameSession.GetMainStageIdx();
            bonus_stage_idx = 0;
        }
        else
        {
            gGameSession.DecodeStage(gGameSession.GetStage(), &main_stage_idx, &bonus_stage_idx);
        }

        if (!saved_is_bonus)
        {
            saved_bonus_code = 0;
        }
        else
        {
            gGameSession.DecodeStage(saved_bonus_code, &saved_main_stage, &saved_bonus_code);
        }

        MenuPage* pPage = &mMenuPagesArray[menu_page_idx];
        if (main_stage_idx == saved_main_stage && bonus_stage_idx == saved_bonus_code)
        {
            pPage->mOptionsArray[option_idx].mIsUnlocked = 1;
            pPage->mOptionStates[option_idx].mIsOptionUnlocked = 1;
        }
        else
        {
            pPage->mOptionsArray[option_idx].mIsUnlocked = 0;
            pPage->mOptionStates[option_idx].mIsOptionUnlocked = 0;
        }
    }

    if (menu_page_idx == MENUPAGE_PLAY)
    {
        // One chained assignment: the slot index pushed to SetPlySlotIdx is a byte temp of the call result
        // (`mov %al,%cl; push %ecx`), which an s16 local gives as `push %eax`
        gGameSession.SetPlySlotIdx(static_cast<char_type>(mMenuPagesArray[1].mOptionsArray[0].mUnused70 =
                                                     mMenuPagesArray[1].mOptionsArray[0].mHorizontalSelectedIdx =
                                                         static_cast<u16>(gRegistry.Get_Player_Setting("plyrslot"))));
        UpdateMenuForCurrPlayer();
    }
    else if (menu_page_idx == MENUPAGE_MULTIPLAYER_RESULTS)
    {
        num_players = gGameSession.GetMaxPlayers();
        gTagGame.GetLeaderIdx();
        user_idx = static_cast<u8>(gGameSession.GetUserPlayerIdx());
        u8 game_mode_2 = gGameSession.GetMultiplayerGamemode();

        switch (game_mode_2)
        {
            case FRAG_GAME_1:
                wcsncpy(mMenuPagesArray[7].mElementsArray[13].mElementNameStr,
                        gText_0x14->Find("frags_h"),
                        50);
                mMenuPagesArray[7].mElementsArray[13].mXpos =
                    GetCenteredXPos(mMenuPagesArray[7].mElementsArray[13].mElementNameStr,
                                              mMenuPagesArray[7].mElementsArray[13].mFontType,
                                              320);
                break;

            case POINTS_GAME_2:
                wcsncpy(mMenuPagesArray[7].mElementsArray[13].mElementNameStr,
                        gText_0x14->Find("pnts_h"),
                        50);
                mMenuPagesArray[7].mElementsArray[13].mXpos =
                    GetCenteredXPos(mMenuPagesArray[7].mElementsArray[13].mElementNameStr,
                                              mMenuPagesArray[7].mElementsArray[13].mFontType,
                                              320);
                break;

            case TAG_GAME_3:
                wcsncpy(mMenuPagesArray[7].mElementsArray[13].mElementNameStr,
                        gText_0x14->Find("times_h"),
                        50);
                mMenuPagesArray[7].mElementsArray[13].mXpos =
                    GetCenteredXPos(mMenuPagesArray[7].mElementsArray[13].mElementNameStr,
                                              mMenuPagesArray[7].mElementsArray[13].mFontType,
                                              320);
                break;

            default:
                FatalError(Gta2Error::InvalidMultiplayerGameType,
                                  "C:\\Splitting\\GTA2\\Source\\frontend2.cpp",
                                  4079); // Multiplayer game type should be frag, tag or score (but isn't)
                break;
        }

        for (player = 0; player < 6; player++)
        {
            if (player < num_players)
            {
                mMenuPagesArray[7].mElementsArray[player + 1].mIsItDisplayed = true;
                mMenuPagesArray[7].mElementsArray[player + 7].mIsItDisplayed = !IsTagGame();
            }
            else
            {
                mMenuPagesArray[7].mElementsArray[player + 1].mIsItDisplayed = false;
                mMenuPagesArray[7].mElementsArray[player + 7].mIsItDisplayed = false;
            }
        }

        opponent_idx = 0;
        for (player = 0; player < num_players; player++)
        {
            if (gTagGame.HasQuit(player))
            {
                swprintf(quit_name,
                         L"%s (%s)",
                         gGameSession.GetPlayerName(player)->mName,
                         gText_0x14->Find("mult_q"));
                wcscpy(player_name, quit_name);
            }
            else
            {
                wcsncpy(player_name, gGameSession.GetPlayerName(player)->mName, 50);
            }
            gText_0x14->StrToUpper(player_name);
            wcsncpy(mMenuPagesArray[7].mElementsArray[player + 1].mElementNameStr, player_name, 50);
            if (player != user_idx)
            {
                wcsncpy(mMenuPagesArray[7].mElementsArray[opponent_idx + 8].mElementNameStr, player_name, 50);
                opponent_idx++;
            }
        }

        // A quitter loses and a game without a known mode draws: both go through user_value (the tests
        // below are threaded away), which also keeps best_opponent in edi across the three loops
        s32 best_opponent = -1;
        s32 user_value = 0;
        if (gTagGame.HasQuit(user_idx))
        {
            user_value = -1;
        }
        else if (game_mode_2 == FRAG_GAME_1)
        {
            for (i = 0; i < num_players; i++)
            {
                if (i != user_idx && !gTagGame.HasQuit(i) &&
                    static_cast<s16>(gGameSession.GetFragsForPlayerIdx(i)) > best_opponent)
                {
                    best_opponent = static_cast<s16>(gGameSession.GetFragsForPlayerIdx(i));
                }
            }
            user_value = static_cast<s16>(gGameSession.GetFragsForPlayerIdx(user_idx)) - best_opponent;
        }
        else if (game_mode_2 == POINTS_GAME_2)
        {
            for (i = 0; i < num_players; i++)
            {
                if (i != user_idx && !gTagGame.HasQuit(i) &&
                    gGameSession.GetPointsForPlayerIdx(i) > best_opponent)
                {
                    best_opponent = gGameSession.GetPointsForPlayerIdx(i);
                }
            }
            user_value = gGameSession.GetPointsForPlayerIdx(user_idx) - best_opponent;
        }
        else if (game_mode_2 == TAG_GAME_3)
        {
            for (i = 0; i < num_players; i++)
            {
                if (i != user_idx && !gTagGame.HasQuit(i))
                {
                    // A select (`cmp; jg; mov; mov`), not `best = time > best ? time : best` (spills best)
                    s32 time = gTagGame.GetTime(i);
                    if (time <= best_opponent)
                    {
                        time = best_opponent;
                    }
                    best_opponent = time;
                }
            }
            user_value = gTagGame.GetTime(user_idx) - best_opponent;
        }

        if (user_value > 0)
        {
            wcsncpy(mMenuPagesArray[7].mElementsArray[0].mElementNameStr,
                    gText_0x14->Find("mult_w"),
                    50);
        }
        else if (user_value < 0)
        {
            wcsncpy(mMenuPagesArray[7].mElementsArray[0].mElementNameStr,
                    gText_0x14->Find("mult_l"),
                    50);
        }
        else
        {
            wcsncpy(mMenuPagesArray[7].mElementsArray[0].mElementNameStr,
                    gText_0x14->Find("mult_d"),
                    50);
        }
    }
    else if (menu_page_idx == MENUPAGE_PLAY_INTRO)
    {
        if (bIsFrench)
        {
            FreeSound();
        }

        if (pre_intro_bik_exists())
        {
            Bink::OpenSlot1(gFrontend->pre_intro_bik(), gSampManager.mhDriver);
        }
        else
        {
            Bink::OpenSlot2(gFrontend->intro_bik(), gSampManager.mhDriver);
        }
    }
    else if (menu_page_idx == MENUPAGE_START_MENU)
    {
        mLastInputTime = timeGetTime();
    }

    mCurrentPageIdx = menu_page_idx;
    mMenuPagesArray[menu_page_idx].mCurrentOptionIdx = mMenuPagesArray[menu_page_idx].mDefaultOptionIdx;
    MenuPage* pCurPage = &mMenuPagesArray[mCurrentPageIdx];
    if (!pCurPage->mOptionsArray[pCurPage->mCurrentOptionIdx].mIsUnlocked)
    {
        pCurPage->SelectNextOption();
        if (!pCurPage->mOptionsArray[pCurPage->mCurrentOptionIdx].mIsUnlocked)
        {
            FatalError(Gta2Error::NoValidMenuOptions,
                              "C:\\Splitting\\GTA2\\Source\\frontend2.cpp",
                              4269); // the menu contains no valid options
        }
    }

    UpdateMenuScreen();
}

MATCH_FUNC(0x4B3AF0)
void Frontend::GetOptionText(u16 menu_page_idx, u16 option_idx, wchar_t** w_buffer)
{
    MenuPage* pPage = &mMenuPagesArray[menu_page_idx];
    MenuItem* pOption = &pPage->mOptionsArray[option_idx];

    if (menu_page_idx == MENUPAGE_PLAY && option_idx == 0) // option 0 = change player/name
    {
        u16 plyr_idx = pOption->mHorizontalSelectedIdx;
        wchar_t* p_wName = reinterpret_cast<wchar_t*>(&gPlyDat->mPlyrStats[plyr_idx].mstrPlayerName);
        if (mState == FrontendState::User_Typing_New_Player_Name_3)
        {
            // player typing a name
            wcscpy(gTmpWideStr, mCurrPlyrName);
        }
        else if (!*p_wName)
        {
            // player 1, 2, 3 etc.
            swprintf(tmpBuff, L"%d", plyr_idx);
            swprintf(gTmpWideStr, L"%s %s", pOption->mOptionNameStr, tmpBuff);
        }
        else
        {
            // get saved player name
            swprintf(gTmpWideStr, L"%s", gPlyDat->mPlyrStats[plyr_idx].mstrPlayerName);
        }
    }
    else if (menu_page_idx == MENUPAGE_VIEW_HIGH_SCORE && option_idx == 0)
    {
        swprintf(gTmpWideStr, L"%s", gText_0x14->Find("hi_for"));
    }
    else
    {
        swprintf(tmpBuff, L"%d", pOption->mHorizontalSelectedIdx);
        swprintf(gTmpWideStr, L"%s %s", pOption->mOptionNameStr, tmpBuff);
    }
    *w_buffer = reinterpret_cast<wchar_t*>(&gTmpWideStr);
}

MATCH_FUNC(0x4B8680)
void Frontend::InitSound()
{
    if (!bSkip_audio)
    {
        snd1.mObjectType = 0;
        snd1.mbStatus = false;
        snd2.mObjectType = SoundObjectTypeEnum::Frontend_2;
        snd2.mpAny.pNextFree = &snd1;
        snd2.mbStatus = false;
        snd2.mSoundEntry = gRoot_sound.AddSoundObject(&snd2);
    }
}

MATCH_FUNC(0x4AEDB0)
s32 Frontend::Run()
{
    u32 Time;
    u16 local_current_page_idx;
    char_type* local_field_8_keys;
    s32 key_count;
    s32 result;
    char_type* pKey;
    s32 remaining;
    char_type frames;
    HDIGDRIVER local_field_0_hDriver;

    Time = timeGetTime();
    local_current_page_idx = mCurrentPageIdx;
    if (local_current_page_idx == MENUPAGE_PLAY_INTRO)
    {
        if (Bink::TickFrame())
        {
            if (Bink::GetActiveSlot() == 1)
            {
                Bink::SetActiveSlot(2);
                local_field_0_hDriver = gSampManager.mhDriver;
                Bink::OpenSlot2(gFrontend->intro_bik(), local_field_0_hDriver);
                Bink::CloseSlot1();
                Bink::SetActiveSlot(2);
                Bink::SetDDState(2);
            }
            else
            {
                Bink::CloseSlot1();
                Bink::CloseSlot2();
                if (bIsFrench)
                {
                    InitSound();
                }

                ChangeMenuPage(MENUPAGE_START_MENU);
            }
        }

        read_menu_input();

        local_field_8_keys = mKeys;
        key_count = 256;
        do
        {
            if ((*local_field_8_keys & 0x80u) != 0)
            {
                Bink::CloseSlot1();
                Bink::CloseSlot2();
                if (bIsFrench)
                {
                    InitSound();
                }
                ChangeMenuPage(MENUPAGE_START_MENU);
            }
            ++local_field_8_keys;
            --key_count;
        } while (key_count);

        return mWinmainNextState;
    }
    else
    {
        if (local_current_page_idx == MENUPAGE_START_MENU)
        {
            pKey = mKeys;
            remaining = 256;
            do
            {
                if ((*pKey & 0x80u) != 0)
                {
                    mLastInputTime = Time;
                }
                ++pKey;
                --remaining;
            } while (remaining);
            if (Time - mLastInputTime > 60000)
            {
                return 4;
            }
        }
        else
        {
            mLastInputTime = Time;
        }

        if (Time >= static_cast<u32>(mNextFrameTime) || (mUpdatesSinceRender == 3))
        {
            Update();
            frames = mUpdatesSinceRender + 1;
            mNextFrameTime += 33;
            mUpdatesSinceRender = frames;
        }
        else if (mUpdatesSinceRender)
        {
            Render();
            result = mWinmainNextState;
            mUpdatesSinceRender = 0;
            return result;
        }
        return mWinmainNextState;
    }
}

// https://decomp.me/scratch/ci11a
MATCH_FUNC(0x4B5430)
void Frontend::DrawScoreTable(ScoreTableLine* pStrings,
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
        ScoreTableLine* pIter = &pStrings[i];
        u16 text_ypos_to_use = text_ypos + 40 * i;
        if (spacing_type)
        {
            text_ypos_to_use = text_ypos + 20 * i;
        }
        if (!wcscmp(pIter->mPlayerName, reinterpret_cast<wchar_t*>(&gEmptyWStr)))
        {
            swprintf(tmpBuff, gText_0x14->Find("hi_empt"));
        }
        else
        {
            swprintf(tmpBuff, L"%s", pIter->mPlayerName);
        }
        if (static_cast<u16>(palette) == 0xFFFFu)
        {
            DrawStringDefaultPalette(tmpBuff, text_xpos, text_ypos_to_use, arg_fontType, 1);
        }
        else
        {
            DrawString(tmpBuff, text_xpos, text_ypos_to_use, arg_fontType, 1, palette_types_enum::font_remaps_8, palette, 0, 0);
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
        swprintf(tmpBuff, L"%d", pIter->mScore);

        if (gText_0x14->LangIsJapanese())
        {
            Frontend::DrawTextFixedWidth(tmpBuff, new_xpos, text_ypos_to_use, arg_fontType, palette, 1, 16, true);
        }
        else
        {
            Frontend::DrawTextFixedWidth(tmpBuff, new_xpos, text_ypos_to_use, arg_fontType, palette, 1, 13, true);
        }

    }
}

// 9.6f 0x457920
// https://decomp.me/scratch/jchxT
MATCH_FUNC(0x4AD140)
void Frontend::DrawMenu()
{
    const s32 line_height = !gText_0x14->LangIsJapanese() ? 14 : 16;

    MenuPage* pMenuPage = &mMenuPagesArray[mCurrentPageIdx];

    u16 selected_option_idx;
    u16 last_xpos;

    if (mCurrentPageIdx == MENUPAGE_START_MENU)
    {
        swprintf(tmpBuff, L"GTA2 V%d.%d", gGTA2VersionMajor, gGTA2VersionMinor);
        DrawStringDefaultPalette(tmpBuff, 300, 460, font_type_703C14, 1);
    }

    if (mCurrentPageIdx == MENUPAGE_PLAY)
    {
        if (mState == FrontendState::User_Typing_New_Player_Name_3)
        {
            pMenuPage->mElementsArray[8].mIsItDisplayed = false;
            pMenuPage->mElementsArray[9].mIsItDisplayed = false;

            // NOTE: mFontType is u16

            last_xpos = DrawKeyHint(2, 0x12Cu, 0x1B8u, mFontType, 0xFFFF); // text: ENTER
            last_xpos = DrawKeyHint(11, last_xpos + 300, 0x1B8u, mFontType, 0xFFFF); // text: : ENTER NAME

            last_xpos = DrawKeyHint(3, 0x12Cu, 0x1CCu, mFontType, 0xFFFF); // text: BACKSPACE
            DrawKeyHint(10, last_xpos + 300, 0x1CCu, mFontType, 0xFFFF); // text: : DELETE LETTER
        }
        else
        {
            u16 idx = pMenuPage->mOptionsArray[0].mHorizontalSelectedIdx;
            selected_option_idx = idx;
            u16 name_x_offset =
                Frontend::GetMaxTextWidth(gPlyDat->mPlyrStats[idx].mstrPlayerName, mNormalFont) + 10;

            if (name_x_offset == 10)
            {
                name_x_offset = Frontend::GetMaxTextWidth(pMenuPage->mOptionsArray[0].mOptionNameStr, mNormalFont) + 40;
            }
            pMenuPage->mElementsArray[9].mXpos = name_x_offset + pMenuPage->mOptionsArray[0].mXPos;
        }
    }

    HighScoreTable* pHighScoreTable;

    if (mCurrentPageIdx == MENUPAGE_VIEW_HIGH_SCORE)
    {
        if (mHiscoreTableIdx < 3)
        {
            pHighScoreTable = &gPlyDat->mStageScores[mHiscoreTableIdx][0]; // main district score
            Frontend::DrawScoreTable(reinterpret_cast<ScoreTableLine*>(&pHighScoreTable->mScoreTableLine), 300, 250, 5, mScoreFont, 0xFFFF, 2);
        }
        else if (mHiscoreTableIdx < 6)
        {
            pHighScoreTable = &gPlyDat->mStageScores[0][mHiscoreTableIdx - 2];
            Frontend::DrawScoreTable(reinterpret_cast<ScoreTableLine*>(&pHighScoreTable->mScoreTableLine), 300, 250, 5, mScoreFont, 0xFFFF, 2);
        }
        else
        {
            if (mHiscoreTableIdx < 9)
            {
                pHighScoreTable = &gPlyDat->mStageScores[1][mHiscoreTableIdx - 5];
                Frontend::DrawScoreTable(reinterpret_cast<ScoreTableLine*>(&pHighScoreTable->mScoreTableLine), 300, 250, 5, mScoreFont, 0xFFFF, 2);
            }
            else
            {
                pHighScoreTable = &gPlyDat->mStageScores[2][mHiscoreTableIdx - 8];
                Frontend::DrawScoreTable(reinterpret_cast<ScoreTableLine*>(&pHighScoreTable->mScoreTableLine), 300, 250, 5, mScoreFont, 0xFFFF, 2);
            }
        }

        if (!bIsLeftRightLoopEnabled)
        {
            // left triangle
            if (mHiscoreTableIdx == 0) // first option
            {
                pMenuPage->mElementsArray[2].mIsItDisplayed = false;
            }
            else
            {
                pMenuPage->mElementsArray[2].mIsItDisplayed = true;
            }
            // right triangle
            if (mHiscoreTableIdx == 11) // last option
            {
                pMenuPage->mElementsArray[3].mIsItDisplayed = false;
            }
            else
            {
                pMenuPage->mElementsArray[3].mIsItDisplayed = true;
            }
        }
    }

    if (mCurrentPageIdx == MENUPAGE_DEAD || mCurrentPageIdx == MENUPAGE_AREA_COMPLETE || mCurrentPageIdx == MENUPAGE_BONUS_AREA ||
        mCurrentPageIdx == MENUPAGE_RESULTS_PLAYER_QUIT)
    {
        u8 main_level_idx;
        u8 bonus_stage_idx;

        if (!gGameSession.IsBonusStage())
        {
            main_level_idx = gGameSession.GetMainStageIdx();
            bonus_stage_idx = 0;
        }
        else
        {
            gGameSession.DecodeStage(gGameSession.GetStage(), &main_level_idx, &bonus_stage_idx);
        }
        if (mCurrentPageIdx == MENUPAGE_BONUS_AREA)
        {
            s32 bonus_letter = (3 * main_level_idx) + bonus_stage_idx + 64;
            swprintf(tmpBuff, L"%s %c", gText_0x14->Find("bonslev"), bonus_letter);
            wcsncpy(pMenuPage->mElementsArray[0].mElementNameStr, tmpBuff, 0x32u);
            DrawBonusRating();
        }

        Frontend::DrawScoreTable(reinterpret_cast<ScoreTableLine*>(&gPlyDat->mStageScores[main_level_idx][bonus_stage_idx]
                                 .mScoreTableLine),
                             0xAAu,
                             155,
                             3,
                             mScoreFont,
                             0xFFFF,
                             2);

        if (mCurrentPageIdx == MENUPAGE_DEAD || mCurrentPageIdx == MENUPAGE_AREA_COMPLETE ||
            mCurrentPageIdx == MENUPAGE_RESULTS_PLAYER_QUIT)
        {
            Frontend::DrawLastAndBestStats(10, 0xE1);
        }
    }

    if (mCurrentPageIdx == MENUPAGE_MULTIPLAYER_RESULTS)
    {
        Frontend::DrawMultiplayerScores();
    }

    u32 chosen_option_idx = -1;
    u16 option_idx = 0;

    wchar_t* wstr_array;

    u16 x_pos;
    u16 y_pos;

    for (option_idx = 0; option_idx < pMenuPage->mNumberOfOptions; option_idx++)
    {
        MenuItem* pMenuOption = &pMenuPage->mOptionsArray[option_idx];

        if (pMenuOption->mIsUnlocked)
        {
            if (pMenuOption->mOptionType == HORIZONTAL_SELECTION_2)
            {
                Frontend::GetOptionText(mCurrentPageIdx, option_idx, &wstr_array);
            }
            else
            {
                wstr_array = reinterpret_cast<wchar_t*>(&pMenuOption->mOptionNameStr);
            }

            x_pos = pMenuOption->mXPos;
            y_pos = pMenuOption->mYPos;

            if (option_idx == pMenuPage->mCurrentOptionIdx)
            {
                DrawStringDefaultPalette(wstr_array, x_pos, y_pos, mSelectedFont, 1);

                if (mCurrentPageIdx == MENUPAGE_PLAY)
                {
                    pMenuPage->mElementsArray[4].mIsItDisplayed = false;
                    pMenuPage->mElementsArray[5].mIsItDisplayed = false;
                    pMenuPage->mElementsArray[6].mIsItDisplayed = false;
                    pMenuPage->mElementsArray[7].mIsItDisplayed = false;
                    pMenuPage->mElementsArray[8].mIsItDisplayed = false;
                    pMenuPage->mElementsArray[9].mIsItDisplayed = false;
                    if (option_idx == 3) //  START PLAY IN AREA
                    {
                        chosen_option_idx = 3;
                        pMenuPage->mElementsArray[4].mGeometricShapeType = 1;
                        pMenuPage->mElementsArray[5].mGeometricShapeType = 2;
                        pMenuPage->mElementsArray[4].mIsItDisplayed = mHasPrevMainStage != 0;
                        pMenuPage->mElementsArray[5].mIsItDisplayed = mHasNextMainStage != 0;
                    }
                    else if (option_idx == 4) // BONUS STAGE
                    {
                        chosen_option_idx = 4;
                        pMenuPage->mElementsArray[6].mGeometricShapeType = 1;
                        pMenuPage->mElementsArray[7].mGeometricShapeType = 2;
                        pMenuPage->mElementsArray[6].mIsItDisplayed = mHasPrevBonusStage != 0;
                        pMenuPage->mElementsArray[7].mIsItDisplayed = mHasNextBonusStage != 0;
                    }
                    else if (option_idx == 0)
                    {
                        pMenuPage->mElementsArray[8].mGeometricShapeType = 1;
                        pMenuPage->mElementsArray[9].mGeometricShapeType = 2;
                        if (mState != FrontendState::User_Typing_New_Player_Name_3)
                        {
                            pMenuPage->mElementsArray[8].mIsItDisplayed = true;
                            pMenuPage->mElementsArray[9].mIsItDisplayed = true;
                            mHasPrevPlayerSlot = true;
                            mHasNextPlayerSlot = true;
                            if (!bIsLeftRightLoopEnabled)
                            {
                                if (selected_option_idx == 0)
                                {
                                    pMenuPage->mElementsArray[8].mIsItDisplayed = false;
                                    mHasPrevPlayerSlot = false;
                                }
                                else if (selected_option_idx == pMenuPage->mOptionsArray[0].mHorizontalMaxIdx)
                                {
                                    pMenuPage->mElementsArray[9].mIsItDisplayed = false;
                                    mHasNextPlayerSlot = false;
                                }
                            }

                            last_xpos = DrawKeyHint(2, 0x12Cu, 0x1B8u, mFontType, 0xFFFF);
                            DrawKeyHint(8, last_xpos + 300, 0x1B8u, mFontType, 0xFFFF);
                            last_xpos = DrawKeyHint(1, 0x12Cu, 0x1CCu, mFontType, 0xFFFF);
                            DrawKeyHint(9, last_xpos + 300, 0x1CCu, mFontType, 0xFFFF);
                        }
                    }
                }
                else if (mCurrentPageIdx == MENUPAGE_VIEW_HIGH_SCORE)
                {
                    pMenuPage->mElementsArray[2].mGeometricShapeType = 3;
                    pMenuPage->mElementsArray[3].mGeometricShapeType = 4;
                    if (option_idx == 0)
                    {
                        pMenuPage->mElementsArray[2].mGeometricShapeType = 1;
                        pMenuPage->mElementsArray[3].mGeometricShapeType = 2;
                    }
                }
                // ....
            }
            else
            {
                if (pMenuOption->mFontType != 0xFFFF)
                {
                    if (pMenuOption->mPalette == 0xFFFF)
                    {
                        DrawStringDefaultPalette(wstr_array, x_pos, y_pos, pMenuOption->mFontType, 1);
                    }
                    else
                    {
                        DrawString(wstr_array, x_pos, y_pos, pMenuOption->mFontType, static_cast<s32>(1), palette_types_enum::font_remaps_8, pMenuOption->mPalette, 0, 0);
                    }
                }
                else
                {
                    if (pMenuOption->mPalette == 0xFFFF)
                    {
                        DrawStringDefaultPalette(wstr_array, x_pos, y_pos, mNormalFont, 1);
                    }
                    else
                    {
                        DrawString(wstr_array, x_pos, y_pos, mNormalFont, static_cast<s32>(1), palette_types_enum::font_remaps_8, pMenuOption->mPalette, 0, 0);
                    }
                }
            }
        }
        else
        {
            bool bForceSelect = false;
            if ((mCurrentPageIdx == MENUPAGE_PLAY // 0x1
                 || mCurrentPageIdx == MENUPAGE_AREA_COMPLETE) // 0x3
                && option_idx == 1)
            {
                bForceSelect = true;
            }

            if (((mCurrentPageIdx == MENUPAGE_RESULTS_PLAYER_QUIT || mCurrentPageIdx == MENUPAGE_DEAD) && option_idx == 0) || bForceSelect)
            {
                if (pMenuOption->mOptionType == HORIZONTAL_SELECTION_2)
                {
                    Frontend::GetOptionText(mCurrentPageIdx, option_idx, &wstr_array);
                }
                else
                {
                    wstr_array = reinterpret_cast<wchar_t*>(&pMenuOption->mOptionNameStr);
                }

                x_pos = pMenuOption->mXPos;
                y_pos = pMenuOption->mYPos;

                if (pMenuOption->mFontType != 0xFFFF)
                {
                    DrawString(wstr_array, x_pos, y_pos, pMenuOption->mFontType, 1, palette_types_enum::font_remaps_8, 8, 0, 0);
                }
                else
                {
                    DrawString(wstr_array, x_pos, y_pos, mNormalFont, 1, palette_types_enum::font_remaps_8, 8, 0, 0);
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
            main_level_idx = gGameSession.GetMainStageIdx();
            bonus_level_idx = 0;
        }
        else if (chosen_option_idx == 4) // BONUS STAGE
        {
            gGameSession.DecodeStage(gGameSession.GetStage(), &main_level_idx, &bonus_level_idx);
        }
        Frontend::DrawScoreTable(reinterpret_cast<ScoreTableLine*>(&gPlyDat->mStageScores[main_level_idx][bonus_level_idx]
                                 .mScoreTableLine),
                             0x12Cu,
                             line_height,
                             1,
                             mPlayMenuScoreFont,
                             0xFFFF,
                             2);
    }

    for (option_idx = 0; option_idx < pMenuPage->mNumberOfElements; option_idx++)
    {
        MenuElement* pMenuElement = &pMenuPage->mElementsArray[option_idx];

        if (pMenuElement->mIsItDisplayed)
        {
            u16 font_type;
            s32 shape_type;

            switch (pMenuElement->mElementType)
            {
                case GEOMETRIC_SHAPE_3:
                    x_pos = pMenuElement->mXpos;
                    y_pos = pMenuElement->mYpos;

                    switch (pMenuElement->mGeometricShapeType)
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

                    DrawFigure(sprite_types_enum::user_6, shape_type, x_pos, y_pos, kAngZero_67DA70, kFpOne_67D934, palette_types_enum::sprites_2, 0, 0, 0, false);
                    break;

                case STRING_TEXT_1:
                    font_type = pMenuElement->mFontType;

                    x_pos = pMenuElement->mXpos;
                    y_pos = pMenuElement->mYpos;

                    if (font_type == 0xFFFF)
                    {
                        font_type = mNormalFont;
                    }
                    Frontend::GetElementText(mCurrentPageIdx, option_idx, &wstr_array);

                    if (mCurrentPageIdx == MENUPAGE_PLAY && (option_idx == 2 || option_idx == 3))
                    {
                        Frontend::DrawTextFixedWidth(wstr_array, x_pos, y_pos, font_type, pMenuElement->mFontPalette, 1, 0x15u, true);
                    }
                    else if (mCurrentPageIdx == MENUPAGE_VIEW_HIGH_SCORE && option_idx == 1)
                    {
                        Frontend::DrawTextFixedWidth(wstr_array, x_pos, y_pos, font_type, pMenuElement->mFontPalette, 1, 0x15u, true);
                    }
                    else
                    {
                        if (pMenuElement->mFontPalette == 0xFFFF)
                        {
                            DrawStringDefaultPalette(wstr_array, x_pos, y_pos, font_type, 1);
                        }
                        else
                        {
                            DrawString(wstr_array, x_pos, y_pos, font_type, 1, palette_types_enum::font_remaps_8, pMenuElement->mFontPalette, 0, 0);
                        }
                    }
                    break;
                default:
                    break;
            }
        }
    }

    if (mState == FrontendState::User_Typing_New_Player_Name_3) //  enter new player name
    {
        if (mCursorBlink)
        {
            x_pos = pMenuPage->mOptionsArray[0].mXPos + Frontend::GetMaxTextWidth(mCurrPlyrName, mNormalFont);
            y_pos = pMenuPage->mOptionsArray[0].mYPos;
            swprintf(tmpBuff, L"_");
            DrawStringDefaultPalette(tmpBuff, x_pos, y_pos, mNormalFont, 1);
        }
        wcscpy(tmpBuff, gText_0x14->Find("entrnam"));
        x_pos = 0x15Eu;
        y_pos = !gText_0x14->LangIsJapanese() ? 12 : 16;
        DrawStringDefaultPalette(tmpBuff, x_pos, y_pos, mHeadingFont, 1);
    }
    if (mState == FrontendState::User_Typing_Password_5 && mCursorBlink)
    {
        x_pos = pMenuPage->mElementsArray[4].mXpos + Frontend::GetMaxTextWidth(mPassword, mNormalFont);
        y_pos = pMenuPage->mElementsArray[4].mYpos;
        swprintf(tmpBuff, L"_");
        DrawStringDefaultPalette(tmpBuff, x_pos, y_pos, mNormalFont, 1);
    }
}

// https://decomp.me/scratch/qV1ie switch "goto" issue
MATCH_FUNC(0x4B7AE0)
void Frontend::DrawCredits()
{
    u16 font_type;
    s32 palette;
    s32 palette_type;

    u16 credit_idx = mCreditsLineIdx;
    for (Fix16 y = mCreditsYpos; y < 480 && credit_idx < 600; credit_idx++, y += mCredits.mLines[credit_idx].mYGap)
    {
        CreditsLine* pLine = &mCredits.mLines[credit_idx];
        switch (pLine->mStringCategory)
        {
            case credits_line_category::normal_0: // normal string: white
                font_type = mTextFont;
                palette_type = palette_types_enum::sprites_2;
                palette = 0;
                break;
            case credits_line_category::other_tag_1:
                font_type = mSelectedFont;
                palette_type = palette_types_enum::sprites_2;
                palette = 0;
                break;
            case credits_line_category::dev_names_2: // dev names: blue
                font_type = mSelectedFont;
                palette_type = palette_types_enum::font_remaps_8;
                palette = 13;
                break;
            case credits_line_category::department_3: // department (DMA, T2 etc) : green
                font_type = mSelectedFont;
                palette_type = palette_types_enum::font_remaps_8;
                palette = 14;
                break;
            case credits_line_category::game_name_4: // game name "GTA2" : yellow
                font_type = mSelectedFont;
                palette_type = palette_types_enum::font_remaps_8;
                palette = 15;
                break;
            default:
                FatalError(Gta2Error::InvalidCreditTextColor, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 7966);
        }
        wchar_t* pStrBuf = pLine->mText;
        if (wcscmp(pStrBuf, gEmptyWStr))
        {

            if (!wcscmp(pStrBuf, L"BINKLOGO"))
            {
                DrawFigure(sprite_types_enum::user_6, 1, static_cast<u16>(320), y, kAngZero_67DA70, kFpOne_67D934, palette_types_enum::sprites_2, 0, 0, 0, false);
            }
            else if (!wcscmp(pStrBuf, L"MILESLOGO"))
            {
                DrawFigure(sprite_types_enum::user_6, 25, static_cast<u16>(320), y, kAngZero_67DA70, kFpOne_67D934, palette_types_enum::sprites_2, 0, 0, 0, false);
            }
            else
            {
                s32 text_width = Frontend::GetMaxTextWidth(pStrBuf, font_type);
                u16 draw_x = (640 - text_width) / 2;
                DrawString(pStrBuf, draw_x, y, font_type, 1, palette_type, palette, 0, 0);
            }
        }
    }

    if (pgbh_BlitImage(tgaArray[23].mImg, 0, 0, 451, 144, 85, 0) == -10)
    {
        Load_tga(23u);
        pgbh_BlitImage(tgaArray[23].mImg, 0, 0, 451, 144, 85, 0);
    }
}

MATCH_FUNC(0x4B8650)
void Frontend::FreeSound()
{
    if (!bSkip_audio)
    {
        if (snd2.mSoundEntry)
        {
            gRoot_sound.FreeSoundEntry(snd2.mSoundEntry);
            snd2.mSoundEntry = 0;
        }
    }
}

MATCH_FUNC(0x4B6030)
bool Frontend::pre_intro_bik_exists()
{
    _finddata_t find_data;
    long find_handle = _findfirst(gFrontend->pre_intro_bik(), &find_data);
    if (find_handle == -1)
    {
        return false;
    }
    else
    {
        _findclose(find_handle);
        return true;
    }
}

DEFINE_STATIC_MEMBER_ARRAY_INIT(char_type, Frontend, sBikDataDir, 8, 0x620454, "Data\\");
DEFINE_STATIC_GLOBAL_ARRAY_INIT(char_type, sBikDriveDataDir, 4, 0x62045C, ":\\");

MATCH_FUNC(0x4B5F20)
char_type* Frontend::pre_intro_bik()
{
    LOCAL_STATIC_ARRAY_INIT(char_type, sPreIntroBikName, 20, 0x5FE77C, "Movie\\preintro.bik");
    LOCAL_STATIC_ARRAY(char_type, sPreIntroBikPath, 256, 0x67DB84);
    char_type drive[32];
    drive[0] = gRoot_sound.GetAudioDriveLetter();
    if (drive[0])
    {
        drive[1] = 0;
        strcpy(sPreIntroBikPath, drive);
        strcat(sPreIntroBikPath, sBikDriveDataDir);
    }
    else
    {
        strcpy(sPreIntroBikPath, sBikDataDir);
    }
    strcat(sPreIntroBikPath, sPreIntroBikName);
    return sPreIntroBikPath;
}

MATCH_FUNC(0x4B5E50)
const char_type* Frontend::intro_bik()
{
    LOCAL_STATIC_ARRAY_INIT(char_type, sIntroBikName, 16, 0x5FE76C, "Movie\\intro.bik");
    LOCAL_STATIC_ARRAY(char_type, sIntroBikPath, 256, 0x67DA84);
    char_type drive[32];
    drive[0] = gRoot_sound.GetAudioDriveLetter();
    if (drive[0])
    {
        drive[1] = 0;
        strcpy(sIntroBikPath, drive);
        strcat(sIntroBikPath, sBikDriveDataDir);
    }
    else
    {
        strcpy(sIntroBikPath, sBikDataDir);
    }
    strcat(sIntroBikPath, sIntroBikName);
    return sIntroBikPath;
}

MATCH_FUNC(0x4B5FF0)
bool Frontend::intro_bik_exists()
{
    // note: ecx wasn't first due to global being an object instead of a pointer
    _finddata_t findData;

    // note: put call in argument rather than local to change inst ordering
    const long hFind = _findfirst(gFrontend->intro_bik(), &findData);

    if (hFind == -1)
    {
        return false;
    }

    _findclose(hFind);
    return true;
}

MATCH_FUNC(0x4AEC00)
void Frontend::Update()
{
    read_menu_input();
    UpdateMenuScreen();

    snd1.mObjectType = 0;

    switch (mState)
    {
        case FrontendState::User_Typing_New_Player_Name_3:
            HandlePlayerNameTyping();
            break;

        case FrontendState::User_Typing_Password_5:
            HandlePasswordTyping();
            break;

        case FrontendState::Menu_Navigation_1:
            if (mCurrentPageIdx == MENUPAGE_CREDITS)
            {
                snd1.mbStatus = true;
                ManageCredits();
            }
            else
            {
                snd1.mbStatus = false;
                UpdatePageFromUserInput();
            }
            break;

        case FrontendState::Booting_Map_2:
            SetWinMainStateToBootMap();
            break;

        case FrontendState::Delete_Player_Dialog_4:
            HandleDeletePlayerDialog();
            break;

        default:
            FatalError(Gta2Error::InvalidCase, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 2059, mState);
    }

    if (!bSkip_audio)
    {
        gRoot_sound.Service();
    }

    if (counter_706C4C > 0)
    {
        ReapplySavedGamma();
    }
}

MATCH_FUNC(0x4AFEB0)
void Frontend::read_menu_input()
{
    if (mbInputEnabled && KeyBoard_GetKeyStates())
    {
        const u8 up = mKeys[DIK_UP] & 0x80;
        mUpPressed = up && !mUpKeyDown;
        mUpKeyDown = up;

        const u8 down = mKeys[DIK_DOWN] & 0x80;
        mDownPressed = down && !mDownKeyDown;
        mDownKeyDown = down;

        const u8 left = mKeys[DIK_LEFT] & 0x80;
        mLeftPressed = left && !mLeftKeyDown;
        mLeftKeyDown = left;

        const u8 right = mKeys[DIK_RIGHT] & 0x80;
        mRightPressed = right && !mRightKeyDown;
        mRightKeyDown = right;

        const u8 returnKey = mKeys[DIK_RETURN] & 0x80;
        mReturnPressed = returnKey && !mReturnKeyDown;
        mReturnKeyDown = returnKey;

        const u8 escape = mKeys[DIK_ESCAPE] & 0x80;
        mEscapePressed = escape && !mEscapeKeyDown;
        mEscapeKeyDown = escape;

        const u8 deleteKey = mKeys[DIK_DELETE] & 0x80;
        mDeletePressed = deleteKey && !mDeleteKeyDown;
        mDeleteKeyDown = deleteKey;
    }
    else
    {
        mUpPressed = false;
        mDownPressed = false;
        mLeftPressed = false;
        mRightPressed = false;
        mReturnPressed = false;
        mEscapePressed = false;
        mDeletePressed = false;
    }
}

MATCH_FUNC(0x4B6780)
void Frontend::UpdateMenuScreen()
{
    // Change background image according to the current page or options
    MenuPage* pMenuPage = &mMenuPagesArray[mCurrentPageIdx];
    if (mState != FrontendState::Booting_Map_2)
    {
        if (mCurrentPageIdx == MENUPAGE_START_MENU)
        {
            switch (pMenuPage->mCurrentOptionIdx)
            {
                case 0:
                    mMenuScreen = Play_1;
                    break;

                case 1:
                    mMenuScreen = Options_0;
                    break;

                case 2:
                    mMenuScreen = Quit_2;
                    break;
            }
        }
        else if (mCurrentPageIdx == MENUPAGE_PLAY)
        {
            switch (pMenuPage->mCurrentOptionIdx)
            {
                case 0:
                    mMenuScreen = EnterPlayerName_10;
                    break;
                case 1:
                    mMenuScreen = ResumeLoadSave_11;
                    break;
                case 2:
                    mMenuScreen = ViewHiScore_6;
                    break;
                case 3:
                    mMenuScreen = 7 + gGameSession.GetMainStageIdx();
                    break;
                case 4:
                {
                    u8 main_stage_idx;
                    u8 bonus_stage_idx;
                    gGameSession.DecodeStage(gGameSession.GetStage(), &main_stage_idx, &bonus_stage_idx);
                    mMenuScreen = main_stage_idx + 3;
                    break;
                }
                default:
                    break;
            }
        }
        else if (mCurrentPageIdx == MENUPAGE_VIEW_HIGH_SCORE)
        {
            mMenuScreen = HiScoresDisplay_12;
        }
        else if (mCurrentPageIdx == MENUPAGE_DEAD)
        {
            mMenuScreen = GameOver_13;
        }
        else if (mCurrentPageIdx == MENUPAGE_AREA_COMPLETE || mCurrentPageIdx == MENUPAGE_BONUS_AREA ||
                 mCurrentPageIdx == MENUPAGE_MULTIPLAYER_RESULTS || mCurrentPageIdx == MENUPAGE_RESULTS_PLAYER_QUIT ||
                 mCurrentPageIdx == MENUPAGE_PARENTAL_CONTROL)
        {
            mMenuScreen = RedBar_16;
        }
        else if (mCurrentPageIdx == MENUPAGE_GAME_COMPLETE || mCurrentPageIdx == MENUPAGE_NICE_TRY)
        {
            mMenuScreen = Loading_15;
        }
        else if (mCurrentPageIdx == MENUPAGE_CREDITS)
        {
            mMenuScreen = Credits_17;
        }
        else
        {
            mMenuScreen = PlayArea1_7;
        }
    }
}

// https://decomp.me/scratch/3NE2J
MATCH_FUNC(0x4B7A10)
void Frontend::ManageCredits()
{
    timeGetTime();
    Frontend::read_menu_input();
    bool bKeyPressed = false;
    char_type* pKeyIter = &mKeys[0];

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
        if (!mKeyHeld)
        {
            mWinmainNextState = Quit_1;
            return;
        }
    }
    else
    {
        mKeyHeld = false;
    }

    if (++mCreditsScrollTimer > 0)
    {
        mCreditsScrollTimer = 0;

        if (mCreditsYpos <= 262124)
        {
            while (1)
            {
                if (++mCreditsLineIdx == 600)
                {
                    mWinmainNextState = Quit_1;
                    return;
                }
                if ((mCreditsYpos += Fix16(mCredits.mLines[mCreditsLineIdx].mYGap)) > 262124)
                {
                    break;
                }
            }
        }
        mCreditsYpos -= kFpOne;
    }
}

MATCH_FUNC(0x4AE2D0)
void Frontend::UpdatePageFromUserInput()
{
    MenuPage* pMenuPage;
    PlySlot* pStats;
    u16 target_page_idx;
    u8 bonus_scan_idx;
    u8 main_stage_idx;
    u16 current_option_idx;
    MenuItem* pOption_prev;
    bool bWrapped_prev;
    u16 option_idx;
    MenuItem* pOption_next;
    bool bWrapped_next;
    MenuPage* pMenuPageSaved;
    u8 stage_main_idx;
    u8 stage_bonus_idx;

    pMenuPage = &mMenuPagesArray[mCurrentPageIdx];
    pMenuPageSaved = pMenuPage;
    pStats = GetCurrPlayerStats();

    // Set what happens when the user presses ENTER
    if (mReturnPressed)
    {
        MenuItem* pOption = &pMenuPage->mOptionsArray[pMenuPage->mCurrentOptionIdx];
        if (pOption->mOptionType == ENTERABLE_1)
        {
            target_page_idx = pOption->mMenuPageTarget;
            switch (target_page_idx)
            {
                case 263u:
                    stage_main_idx = gGameSession.GetMainStageIdx();
                    bonus_scan_idx = 3;
                    for (stage_bonus_idx = 3; !pStats->mPlyrStageStats[stage_main_idx][stage_bonus_idx].mIsStageUnlocked || bonus_scan_idx >= mNumBonusStages[stage_main_idx]; stage_bonus_idx = bonus_scan_idx)
                    {
                        --bonus_scan_idx;
                    }
                    LoadMapFilenames(stage_main_idx, stage_bonus_idx);
                    gGameSession.SetStartedFromPlayBonusMenu(false);
                    mMenuScreen = RedBar_16;
                    mState = FrontendState::Booting_Map_2;
                    break;
                case MENUPAGE_PLAY_NEXT_AREA: // 261
                    if (!gGameSession.IsBonusStage())
                    {
                        main_stage_idx = gGameSession.GetMainStageIdx();
                    }
                    else
                    {
                        main_stage_idx = static_cast<u8>(gGameSession.GetStage()) >> 4;
                    }

                    if (FreeLoader::CheckCityInstalled(main_stage_idx + 1))
                    {
                        stage_main_idx = main_stage_idx + 1;
                        if (stage_main_idx >= mNumMainStages)
                        {
                            FatalError(Gta2Error::InvalidLevelAdvancement, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 1543);
                        }

                        if (!pStats->mPlyrStageStats[stage_main_idx][0].mIsStageUnlocked)
                        {
                            FatalError(Gta2Error::LevelNotOpened, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 1548);
                        }
                        LoadMapFilenames(stage_main_idx, 0);
                        mMenuScreen = RedBar_16;
                        mState = FrontendState::Booting_Map_2;
                    }
                    break;
                case MENUPAGE_REPLAY_PREVIOUS_AREA: // 259
                    gGameSession.SetSaveFileName("");
                    mMenuScreen = RedBar_16;
                    mState = FrontendState::Booting_Map_2;
                    break;
                case MENUPAGE_LOADING_SAVE: // 260
                    stage_bonus_idx = gGameSession.GetPlySlotIdx();
                    if (PlySlotSvgExists(stage_bonus_idx))
                    {
                        LoadMapFilenamesFromSvg();
                    }
                    else
                    {
                        gGameSession.SetSaveFileName("");
                    }
                    mMenuScreen = RedBar_16;
                    mState = FrontendState::Booting_Map_2;
                    break;
                case MENUPAGE_GTA2MANAGER: // 257
                    Start_GTA2Manager();
                    break;
                case MENUPAGE_QUIT: // 258
                    mWinmainNextState = Quit_1;
                    break;
                case MENUPAGE_GET_READY_TO_PLAY: // 264
                    stage_main_idx = gGameSession.GetMainStageIdx();
                    if (FreeLoader::CheckCityInstalled(stage_main_idx))
                    {
                        LoadMapFilenames(stage_main_idx, 0);
                        mMenuScreen = RedBar_16;
                        mState = FrontendState::Booting_Map_2;
                    }
                    break;
                case MENUPAGE_GET_READY_TO_PLAY_BONUS: // 265
                    gGameSession.DecodeStage(gGameSession.GetStage(), &stage_main_idx, &stage_bonus_idx);
                    if (FreeLoader::CheckCityInstalled(stage_main_idx))
                    {
                        LoadMapFilenames(stage_main_idx, stage_bonus_idx);
                        gGameSession.SetStartedFromPlayBonusMenu(true);
                        mMenuScreen = RedBar_16;
                        mState = FrontendState::Booting_Map_2;
                    }
                    break;
                case MENUPAGE_CONTINUE_NEXT_STAGE: // 266
                    ContinueToNextStage();
                    break;
                case 268u:
                    break;
                default:
                    ChangeMenuPage(target_page_idx);
                    break;
            }
            snd1.mObjectType = 5;
        }
        else if (mCurrentPageIdx == MENUPAGE_PLAY && pMenuPage->mCurrentOptionIdx == 0) // player
        {
            mState = FrontendState::User_Typing_New_Player_Name_3;
            LoadCurrPlayerName();
            mCurrPlyrNameLength = static_cast<u8>(wcslen(mCurrPlyrName));
            StripPlayerNameToCurrLength();
            mKeyHeld = true;
            mLastKey = DIK_RETURN;
            mKeyRepeatTimer = 5;
            snd1.mObjectType = 5;
        }
    }

    // Set what happens if the user presses ESC
    if (mEscapePressed)
    {
        switch (mCurrentPageIdx)
        {
            case MENUPAGE_START_MENU:
            case MENUPAGE_PARENTAL_CONTROL:
                ChangeMenuPage(MENUPAGE_CREDITS);
                break;
            case MENUPAGE_PLAY:
            case MENUPAGE_DEAD:
            case MENUPAGE_AREA_COMPLETE:
            case MENUPAGE_GAME_COMPLETE:
            case MENUPAGE_BONUS_AREA:
            case MENUPAGE_NICE_TRY:
            case MENUPAGE_RESULTS_PLAYER_QUIT:
                ChangeMenuPage(MENUPAGE_START_MENU);
                break;
            case MENUPAGE_VIEW_HIGH_SCORE:
                ChangeMenuPage(MENUPAGE_PLAY);
                break;
            default:
                mWinmainNextState = Quit_1;
                break;
        }
        snd1.mObjectType = 6;
    }

    // Move selection UP and emit sound
    if (mUpPressed && pMenuPage->SelectPrevOption())
    {
        snd1.mObjectType = 1;
    }

    // Move selection DOWN and emit sound
    if (mDownPressed && pMenuPage->SelectNextOption())
    {
        snd1.mObjectType = 2;
    }

    // Set what happens if the player press the left arrow.
    if (mLeftPressed)
    {
        current_option_idx = pMenuPage->mCurrentOptionIdx;
        pOption_prev = &pMenuPage->mOptionsArray[current_option_idx];
        if (pOption_prev->mOptionType == HORIZONTAL_SELECTION_2)
        {
            bWrapped_prev = pOption_prev->SelectPrevHorizontalIdx();
            if (mCurrentPageIdx == MENUPAGE_PLAY && !pMenuPageSaved->mCurrentOptionIdx)
            {
                gGameSession.SetPlySlotIdx(static_cast<char_type>(pOption_prev->mHorizontalSelectedIdx));
                UpdateMenuForCurrPlayer();
                gRegistry.Set_Player_Setting("plyrslot", pOption_prev->mHorizontalSelectedIdx);
                if (bWrapped_prev)
                {
                    snd1.mObjectType = 3;
                }
            }

            if (mCurrentPageIdx == MENUPAGE_VIEW_HIGH_SCORE && !pMenuPageSaved->mCurrentOptionIdx)
            {
                mHiscoreTableIdx = static_cast<u8>(pOption_prev->mHorizontalSelectedIdx);
                if (bWrapped_prev)
                {
                    snd1.mObjectType = 3;
                }
            }
            pMenuPage = pMenuPageSaved;
        }
        else if (mCurrentPageIdx == MENUPAGE_PLAY)
        {
            if (current_option_idx == 3)
            {
                if (ChangeMainStageToPrevious())
                {
                    snd1.mObjectType = 3;
                }
            }
            else if (current_option_idx == 4 && ChangeBonusStageToPrevious())
            {
                snd1.mObjectType = 3;
            }
        }
    }
    else if (mRightPressed) // Set what happens if the player press the right arrow.
    {
        option_idx = pMenuPage->mCurrentOptionIdx;
        pOption_next = &pMenuPage->mOptionsArray[option_idx];
        if (pOption_next->mOptionType == HORIZONTAL_SELECTION_2)
        {
            bWrapped_next = pOption_next->SelectNextHorizontalIdx();
            if (mCurrentPageIdx == MENUPAGE_PLAY && !pMenuPageSaved->mCurrentOptionIdx)
            {
                gGameSession.SetPlySlotIdx(static_cast<char_type>(pOption_next->mHorizontalSelectedIdx));
                UpdateMenuForCurrPlayer();
                gRegistry.Set_Player_Setting("plyrslot", pOption_next->mHorizontalSelectedIdx);
                if (bWrapped_next)
                {
                    snd1.mObjectType = 4;
                }
            }
            if (mCurrentPageIdx == MENUPAGE_VIEW_HIGH_SCORE && !pMenuPageSaved->mCurrentOptionIdx)
            {
                mHiscoreTableIdx = static_cast<u8>(pOption_next->mHorizontalSelectedIdx);
                if (bWrapped_next)
                {
                    snd1.mObjectType = 4;
                }
            }
            pMenuPage = pMenuPageSaved;
        }
        else if (mCurrentPageIdx == MENUPAGE_PLAY)
        {
            if (option_idx == 3)
            {
                if (ChangeMainStageToNext())
                {
                    snd1.mObjectType = 4;
                }
            }
            else if (option_idx == 4 && ChangeBonusStageToNext())
            {
                snd1.mObjectType = 4;
            }
        }
    }
    if (mDeletePressed && mCurrentPageIdx == MENUPAGE_PLAY && !pMenuPage->mCurrentOptionIdx)
    {
        mState = FrontendState::Delete_Player_Dialog_4;
        mDialogCursorYpos = 190;
        mDialogType = 1;
        snd1.mObjectType = 8;
    }

    if (--mCursorBlinkTimer <= 0)
    {
        mCursorBlink = mCursorBlink == 0;
        mCursorBlinkTimer = 2;
    }
}

MATCH_FUNC(0x4AE990)
void Frontend::SetWinMainStateToBootMap()
{
    mWinmainNextState = Start_Game_3;
}

MATCH_FUNC(0x4B2F60)
void Frontend::HandlePlayerNameTyping()
{
    s16 scan_code;
    s16 input;
    u8* pKeys;
    wchar_t Key_4D5F40;
    u16 typed_char;
    s16 repeat_timer;

    scan_code = 0;
    input = 256;
    pKeys = reinterpret_cast<u8*>(mKeys);
    do
    {
        if ((*pKeys & 0x80u) != 0 && scan_code != 54 && scan_code != 42)
        {
            input = scan_code;
        }
        ++scan_code;
        ++pKeys;
    } while (static_cast<u16>(scan_code) < 0x100u);

    if (mLastKey != input)
    {
        mLastKey = input;
        mKeyRepeatTimer = 5;

        if (input == DIK_RETURN)
        {
            mState = FrontendState::Menu_Navigation_1;
            Frontend::StripPlayerNameToCurrLength();
            Frontend::SaveAndUpdatePlayerName();
            mMenuPagesArray[1].mCurrentOptionIdx = 0;
            if (snd1.mObjectType != 9)
            {
                snd1.mObjectType = 5;
            }
        }
        else if (input == DIK_BACKSPACE)
        {
            if (mCurrPlyrNameLength > 0)
            {
                mCurrPlyrNameLength--;
                Frontend::StripPlayerNameToCurrLength();
                snd1.mObjectType = 8;
            }
        }
        else if (input == DIK_ESCAPE)
        {
            mState = FrontendState::Menu_Navigation_1;
            Frontend::StripPlayerNameToCurrLength();
            mMenuPagesArray[1].mCurrentOptionIdx = 0;
            snd1.mObjectType = 6;
        }

        if (input != 256)
        {
            if (input == DIK_SPACE)
            {
                typed_char = 32;
            }
            else
            {
                KeyboardLayout::RecreateIfLayoutChanged();
                Key_4D5F40 = gKeybrd_0x204->GetKey(input);
                typed_char = gText_0x14->RemapExtendedChar(Key_4D5F40);
                if (static_cast<u16>(GetCharWidthFromFontType(mNormalFont, typed_char)) < 3u || !typed_char)
                {
                    // Both builds jump past the append here; an append in each branch folds the space into a constant
                    goto update_key_held;
                }
            }

            if (mCurrPlyrNameLength != 8)
            {
                mCurrPlyrName[mCurrPlyrNameLength++] = typed_char;
                snd1.mObjectType = 7;
            }
        }
    }
    else
    {
        repeat_timer = mKeyRepeatTimer;
        if (repeat_timer == 0)
        {
            mLastKey = 256;
            mKeyRepeatTimer = 5;
        }
        else
        {
            mKeyRepeatTimer = repeat_timer - 1;
        }
    }

update_key_held:
    if (input == 256)
    {
        mKeyHeld = false;
    }
    else
    {
        mKeyHeld = true;
    }

    mCursorBlinkTimer--;
    if (mCursorBlinkTimer <= 0)
    {
        mCursorBlink = mCursorBlink == 0;
        mCursorBlinkTimer = 2;
    }
}

MATCH_FUNC(0x4AE9A0)
void Frontend::HandleDeletePlayerDialog()
{
    u16 cursor_y;

    if (mReturnPressed)
    {
        cursor_y = mDialogCursorYpos;
        if (cursor_y == 210)
        {
            switch (mDialogType)
            {
                case 1:
                    Frontend::DeleteCurrentPlayer();
                    mState = FrontendState::Menu_Navigation_1;
                    break;
                default:
                    FatalError(Gta2Error::InvalidCase, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 1934);
            }
        }
        if (cursor_y == 230)
        {
            mState = FrontendState::Menu_Navigation_1;
        }
        snd1.mObjectType = 5;
    }

    if (mEscapePressed)
    {
        mState = FrontendState::Menu_Navigation_1;
        snd1.mObjectType = 6;
    }
    if (mUpPressed)
    {
        switch (mDialogCursorYpos)
        {
            case 190:
            case 210:
                mDialogCursorYpos = 230;
                break;
            case 230:
                mDialogCursorYpos = 210;
                break;
            default:
                FatalError(Gta2Error::InvalidCase, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 1968);
                break;
        }
        snd1.mObjectType = 1;
    }
    if (mDownPressed)
    {
        switch (mDialogCursorYpos)
        {
            case 190:
                mDialogCursorYpos = 210;
                break;
            case 210:
                mDialogCursorYpos = 230;
                snd1.mObjectType = 2;
                break;
            case 230:
                mDialogCursorYpos = 210;
                break;
            default:
                FatalError(Gta2Error::InvalidCase, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 1991);
                break;
        }
        snd1.mObjectType = 2;
    }
    mCursorBlinkTimer--;
    if (mCursorBlinkTimer <= 0)
    {
        mCursorBlink = mCursorBlink == 0;
        mCursorBlinkTimer = 2;
    }
}

// Copy protection: picks a random file of the CD table and checks that it is on a CD-ROM drive, has the
// expected size and can't be opened for writing. The table's sizes must add up to a fixed sum, so it can't be
// edited. 10.5 has three identical copies (a static function in a header included by three files), so the body
// is a forced inline here. The count must be defined in another file: with its definition in view VC6 reads it
// with 32 bit loads instead of the original's 16 bit ones.
static __forceinline bool CheckCdFile()
{
    char_type path[255] = "\0";

    u32 sum = 0;
    for (u16 i = 0; i < gCdCheckFileCount; i++)
    {
        sum += gCdCheckFiles[i].mSize;
    }

    if (sum == 0x12209BDC)
    {
        s16 idx = static_cast<s16>(timeGetTime() % gCdCheckFileCount);
        char_type drive[32];
        drive[0] = gRoot_sound.GetAudioDriveLetter();
        if (drive[0])
        {
            drive[1] = 0;
            strcpy(path, drive);
            strcat(path, sBikDriveDataDir);
            strcat(path, gCdCheckFiles[idx].mName);
            if (File::IsCdRomDrive(drive[0]))
            {
                FILE* hFile = crt::fopen(path, "rb");
                if (hFile)
                {
                    if (static_cast<u32>(File::GetFileSize(hFile)) == gCdCheckFiles[idx].mSize)
                    {
                        if (!crt::fclose(hFile))
                        {
                            bool ok = crt::fopen(path, "wb") == NULL;
                            return ok;
                        }
                    }
                }
            }
        }
    }
    return false;
}

MATCH_FUNC(0x4B80F0)
EXPORT bool CheckCdFile_4B80F0()
{
    return CheckCdFile();
}

MATCH_FUNC(0x4BBCB0)
EXPORT bool CheckCdFile_4BBCB0()
{
    return CheckCdFile();
}

MATCH_FUNC(0x4BBFA0)
EXPORT bool CheckCdFile_4BBFA0()
{
    return CheckCdFile();
}

// https://decomp.me/scratch/ySQ2h
MATCH_FUNC(0x4B8280)
void Frontend::HandlePasswordTyping()
{
    wchar_t ch;
    s16 key_scan = 256;
    char_type* pKeyIter = &mKeys[0];

    for (u16 i = 0; i < 256; i++, ++pKeyIter)
    {
        if ((*pKeyIter & 0x80u) != 0 && i != 54 && i != 42)
        {
            key_scan = i;
        }
    }

    if (mLastKey != key_scan)
    {
        mLastKey = key_scan;
        mKeyRepeatTimer = 5;

        if (key_scan == DIK_RETURN)
        {
            mState = FrontendState::Menu_Navigation_1;
            Frontend::StripPasswordToCurrLength();
            Frontend::CheckPassword();
            snd1.mObjectType = 5;
        }
        else if (key_scan == DIK_BACK)
        {
            if (mPasswordLength > 0)
            {
                mPasswordLength--;
                Frontend::StripPasswordToCurrLength();
                snd1.mObjectType = 8;
            }
        }
        else if (key_scan == DIK_ESCAPE)
        {
            mState = FrontendState::Menu_Navigation_1;
            Frontend::StripPasswordToCurrLength();
            Frontend::ChangeMenuPage(MENUPAGE_CREDITS);
            snd1.mObjectType = 6;
        }
        else if (key_scan == 256)
        {
            goto update_key_held;
        }
        else if (key_scan == DIK_SPACE)
        {
            ch = 32;
        // shared with the typed-char path below: goto-free forms change the code
        append_char:
            if (mPasswordLength != 8)
            {
                if (mWrongPasswordShown)
                {
                    mWrongPasswordShown = false;
                    wcsncpy(mMenuPagesArray[14].mElementsArray[0].mElementNameStr,
                            gText_0x14->Find("fr_ent1"),
                            0x32u);
                    wcsncpy(mMenuPagesArray[14].mElementsArray[1].mElementNameStr,
                            gText_0x14->Find("fr_ent2"),
                            0x32u);
                    wcsncpy(mMenuPagesArray[14].mElementsArray[2].mElementNameStr, gEmptyWStr, 0x32u);
                }
                mPassword[mPasswordLength++] = ch;
                snd1.mObjectType = 7;
            }
            goto update_key_held;
        }
        u16 key = gKeybrd_0x204->GetKey(key_scan);
        ch = key;
        if (GetCharWidthFromFontType(mNormalFont, ch) >= 3 && ch)
        {
            goto append_char;
        }
    }
    else
    {
        if (mKeyRepeatTimer == 0)
        {
            mLastKey = 256;
            mKeyRepeatTimer = 5;
        }
        else
        {
            mKeyRepeatTimer--;
        }
    }

update_key_held:
    if (key_scan == 256)
    {
        mKeyHeld = false;
    }
    else
    {
        mKeyHeld = true;
    }
    mCursorBlinkTimer--;
    if (mCursorBlinkTimer <= 0)
    {
        mCursorBlink = (mCursorBlink == 0);
        mCursorBlinkTimer = 2;
    }
}

MATCH_FUNC(0x4B4410)
void Frontend::DeleteCurrentPlayer()
{
    GetCurrPlayerStats()->ResetPlayerSlot();
    gPlyDat->SavePlySlotDat(mMenuPagesArray[1].mOptionsArray[0].mHorizontalSelectedIdx);
    UpdateMenuForCurrPlayer();
}

MATCH_FUNC(0x4B43E0)
PlySlot* Frontend::GetCurrPlayerStats()
{
    // note: movsx vs movzx due to signedness
    u16 idx = gGameSession.GetPlySlotIdx();
    return &gPlyDat->mPlyrStats[idx];
}

MATCH_FUNC(0x4B42E0)
void Frontend::UpdateMenuForCurrPlayer()
{
    PlySlot* pPlayerStats = Frontend::GetCurrPlayerStats();
    u8 PlySlotIdx_4C59B0 = gGameSession.GetPlySlotIdx();
    MenuPage* pMenuPage = &mMenuPagesArray[mCurrentPageIdx];

    u8 prev_stage_idx = Frontend::GetLastUnlockedMainStage(pPlayerStats);
    u8 prev_bonus_code = Frontend::GetLastUnlockedBonusStageCode(pPlayerStats);

    if (prev_stage_idx < mSelectedMainStage[PlySlotIdx_4C59B0])
    {
        mSelectedMainStage[PlySlotIdx_4C59B0] = prev_stage_idx;
        gGameSession.SetMainStageIdx(prev_stage_idx);
    }
    else
    {
        gGameSession.SetMainStageIdx(mSelectedMainStage[PlySlotIdx_4C59B0]);
    }

    if (prev_bonus_code < mSelectedBonusStage[PlySlotIdx_4C59B0] || prev_bonus_code == 0xFF)
    {
        mSelectedBonusStage[PlySlotIdx_4C59B0] = prev_bonus_code;
        gGameSession.SetStage(prev_bonus_code);
    }
    else
    {
        gGameSession.SetStage(mSelectedBonusStage[PlySlotIdx_4C59B0]);
    }

    Frontend::UpdateBonusStageArrows();
    Frontend::UpdateMainStageArrows();
    if (Frontend::PlySlotSvgExists(PlySlotIdx_4C59B0))
    {
        pMenuPage->mOptionsArray[1].mIsUnlocked = 1;
        pMenuPage->mOptionStates[1].mIsOptionUnlocked = 1;
    }
    else
    {
        pMenuPage->mOptionsArray[1].mIsUnlocked = 0;
        pMenuPage->mOptionStates[1].mIsOptionUnlocked = 0;
    }
}

MATCH_FUNC(0x4B4230)
void Frontend::SaveAndUpdatePlayerName()
{
    u16 player_slot_idx = mMenuPagesArray[1].mOptionsArray[0].mHorizontalSelectedIdx;
    wchar_t* pPlayerName = gPlyDat->mPlyrStats[player_slot_idx].mstrPlayerName;
    wcsncpy(pPlayerName, mCurrPlyrName, 9u);
    HandleCheatCode(pPlayerName);
    gPlyDat->SavePlySlotDat(player_slot_idx);
}

MATCH_FUNC(0x4B3CC0)
void Frontend::GetElementText(u16 page_idx, u16 element_idx, wchar_t** ppText)
{
    MenuElement* pElement = &mMenuPagesArray[page_idx].mElementsArray[element_idx];

    if (page_idx == 14 && element_idx == 4)
    {
        wcscpy(gTmpWideStr, mPassword);
    }
    else if ((page_idx == 14 && element_idx != 4) || page_idx != 5 || element_idx != 1)
    {
        swprintf(gTmpWideStr, L"%s", pElement->mElementNameStr);
    }
    else if (mHiscoreTableIdx < 3)
    {
        swprintf(gTmpWideStr, L"%d", mHiscoreTableIdx + 1);
    }
    else
    {
        swprintf(gTmpWideStr, L"%c", mHiscoreTableIdx + 62);
    }
    *ppText = reinterpret_cast<wchar_t*>(&gTmpWideStr);
}

MATCH_FUNC(0x4B3DD0)
void Frontend::HandleCheatCode(const wchar_t* cheat_str_wide)
{
    const char* ascii_cheat_str = TextTable::Wide2PesudoAscii(cheat_str_wide);
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
            this->mbCheatsEnabled = true;
            snd1.mObjectType = 9;
            return;
        }
    }

    if (!mbCheatsEnabled)
    {
        return;
    }

    if (cheat_str_hash == 0x484DF)
    { // GOREFEST
        bDo_blood = !bDo_blood;
        snd1.mObjectType = 9;
    }
    else if (cheat_str_hash == 0x4878D)
    { // BUCKFAST Only mugger peds spawn
        gCheatOnlyMuggerPeds = !gCheatOnlyMuggerPeds;
        snd1.mObjectType = 9;
    }
    else if (cheat_str_hash == 0x4DA77)
    { // VOLTFEST Electro Gun with infinite ammo
        gCheatUnlimitedElectroGun = !gCheatUnlimitedElectroGun;
        snd1.mObjectType = 9;
    }
    else if (cheat_str_hash == 0x41611)
    { // MADEMAN Max respect from all gangs
        gCheatAllGangMaxRespect = !gCheatAllGangMaxRespect;
        snd1.mObjectType = 9;
    }
    else if (cheat_str_hash == 0x46BE8)
    { // LASVEGAS Only Elvis peds spawn
        gCheatOnlyElvisPeds = !gCheatOnlyElvisPeds;
        snd1.mObjectType = 9;
    }
    else if (cheat_str_hash == 0x36F62)
    { // NEKKID All peds are naked
        gCheatNakedPeds = !gCheatNakedPeds;
        snd1.mObjectType = 9;
    }
    else if (cheat_str_hash == 0x4657B)
    { // EATSOUP Free shopping
        bDo_free_shopping = !bDo_free_shopping;
        snd1.mObjectType = 9;
    }
    else if (cheat_str_hash == 0x4639F)
    { // DAVEMOON Basic set of weapons and max ammo
        gCheatGetBasicWeaponsMaxAmmo = !gCheatGetBasicWeaponsMaxAmmo;
        snd1.mObjectType = 9;
    }
    else if (cheat_str_hash == 0x33A69)
    { // CUTIE1 99 lives
        gCheatGet99Lives = !gCheatGet99Lives;
        snd1.mObjectType = 9;
    }
    else if (cheat_str_hash == 0x47AF1)
    { // ARSESTAR Keep weapons after death
        bKeep_weapons_after_death = !bKeep_weapons_after_death;
        snd1.mObjectType = 9;
    }
    else if (cheat_str_hash == 0x49771)
    { // GODOFGTA All weapons
        bGet_all_weapons = !bGet_all_weapons;
        snd1.mObjectType = 9;
    }
    else if (cheat_str_hash == 0x478FB)
    { // RSJABBER Invincibility
        bDo_invulnerable = !bDo_invulnerable;
        snd1.mObjectType = 9;
    }
    else if (cheat_str_hash == 0x44D2F)
    { // DANISGOD Player get points
        gCheatGetPlayerPoints = !gCheatGetPlayerPoints;
        snd1.mObjectType = 9;
    }
    else if (cheat_str_hash == 0x478A9)
    { // COCKTART Skip exploding scores
        bExplodingScoresOff = !bExplodingScoresOff;
        snd1.mObjectType = 9;
    }
    else if (cheat_str_hash == 0x45EC2)
    { // FLAMEON Flame Thrower with infinite ammo
        gCheatUnlimitedFlameThrower = !gCheatUnlimitedFlameThrower;
        snd1.mObjectType = 9;
    }
    else if (cheat_str_hash == 0x45118)
    {
        gCheatUnlimitedJailCard = !gCheatUnlimitedJailCard;
        snd1.mObjectType = 9;
    }
    else if (cheat_str_hash == 0x4672D)
    { // IAMDAVEJ Get $10,000,000
        gCheatGet10MillionMoney = !gCheatGet10MillionMoney;
        snd1.mObjectType = 9;
    }
    else if (cheat_str_hash == 0x4A98B)
    { // SEGARULZ 10x multiplier
        gCheat10xMultiplier = !gCheat10xMultiplier;
        snd1.mObjectType = 9;
    }
    else if (cheat_str_hash == 0x45B2C)
    { // UKGAMER Unlock three main levels
        gCheatUnlockThreeLevels = !gCheatUnlockThreeLevels;
        gPlyDat->UnlockAllStages();
        UpdateMenuForCurrPlayer();
        snd1.mObjectType = 9;
    }
    else if (cheat_str_hash == 0x49C76)
    { // GINGERRR Unlock levels one and two
        gCheatUnlockLevelsOneAndTwo = !gCheatUnlockLevelsOneAndTwo;
        gPlyDat->UnlockStage(1u, 0);
        UpdateMenuForCurrPlayer();
        snd1.mObjectType = 9;
    }
    else if (cheat_str_hash == 0x5073D)
    { // TUMYFROG unlock all levels
        gCheatUnlockAllLevels = !gCheatUnlockAllLevels;
        gPlyDat->UnlockAllStages();
        gPlyDat->UnlockStage(2u, 2u);
        UpdateMenuForCurrPlayer();
        snd1.mObjectType = 9;
    }
    else if (cheat_str_hash == 0x4D5C4)
    { // SCHURULZ Unlimited double damage
        gCheatUnlimitedDoubleDamage = !gCheatUnlimitedDoubleDamage;
        snd1.mObjectType = 9;
    }
    else if (cheat_str_hash == 0x4B28C)
    { // HUNSRUS Invisibility
        gCheatInvisibility = !gCheatInvisibility;
        snd1.mObjectType = 9;
    }
    else if (cheat_str_hash == 0x45AEF)
    { // FISHFLAP All cars are mini cars
        gCheatMiniCars = !gCheatMiniCars;
        snd1.mObjectType = 9;
    }
}

MATCH_FUNC(0x4B4280)
void Frontend::LoadCurrPlayerName()
{
    wcsncpy(mCurrPlyrName,
            gPlyDat
                ->mPlyrStats[mMenuPagesArray[1].mOptionsArray[0].mHorizontalSelectedIdx]
                .mstrPlayerName,
            9u);
}

MATCH_FUNC(0x4B8530)
void Frontend::StripPasswordToCurrLength()
{
    u16 total = mPasswordLength;
    for (u16 i = total; i < 9; i++)
    {
        mPassword[i] = 0;
    }
}

MATCH_FUNC(0x4B8560)
void Frontend::CheckPassword()
{
    if (!wcscmp(mPassword, L"WFUSDFCF")) // french bonus mission unlocks?
    {
        if (intro_bik_exists() && gRegistry.Get_Screen_Setting("do_play_movie", 1) == 1)
        {
            ChangeMenuPage(MENUPAGE_PLAY_INTRO);
        }
        else
        {
            ChangeMenuPage(MENUPAGE_START_MENU);
        }
    }
    else
    {
        mState = FrontendState::User_Typing_Password_5;
        mPasswordLength = 0;

        StripPasswordToCurrLength();

        mKeyHeld = true;
        mLastKey = DIK_RETURN;
        mKeyRepeatTimer = 5;

        wcsncpy(mMenuPagesArray[14].mElementsArray[0].mElementNameStr,
                gText_0x14->Find("fr_rnt1"),
                0x32u);
        wcsncpy(mMenuPagesArray[14].mElementsArray[1].mElementNameStr,
                gText_0x14->Find("fr_rnt2"),
                0x32u);
        wcsncpy(mMenuPagesArray[14].mElementsArray[2].mElementNameStr,
                gText_0x14->Find("fr_rnt3"),
                0x32u);

        mWrongPasswordShown = true;
    }
}

MATCH_FUNC(0x4B8020)
void Frontend::ContinueToNextStage()
{
    PlySlot* pClarke = GetCurrPlayerStats();
    u8 main_stage_idx = gGameSession.GetMainStageIdx();

    if (AreAllStagesUnlocked()) // Everything unlocked, including all bonus stages
    {
        ChangeMenuPage(MENUPAGE_GAME_COMPLETE);
    }
    else if (main_stage_idx == mNumMainStages - 1) // Not all Bonus stages unlocked but finished last main stage
    {
        ChangeMenuPage(MENUPAGE_NICE_TRY);
    }
    else
    {
        // Load next stage. Can be a Bonus stage or a Main stage
        // note: reg swap + push swap due to redundant local
        u8 substage_idx = 3;
        while (!pClarke->mPlyrStageStats[main_stage_idx][substage_idx].mIsStageUnlocked || substage_idx >= mNumBonusStages[main_stage_idx])
        {
            substage_idx--;
        }

        LoadMapFilenames(main_stage_idx, substage_idx);
        gGameSession.SetStartedFromPlayBonusMenu(false);
        mMenuScreen = RedBar_16;
        mState = FrontendState::Booting_Map_2;
    }
}

MATCH_FUNC(0x4B7D60)
void Frontend::DrawBonusRating()
{
    u16 font_type = mTextFont;
    char_type text_id[12];
    wchar_t text[256];
    GetLineSpacingFromFontType_5D7700_inlined(font_type);
    u16 rating_idx = gGameSession.GetBonusRatingTextIdx();
    _itoa(rating_idx, text_id, 10);
    if (rating_idx)
    {
        wchar_t* pRatingStr = gText_0x14->Find(text_id);
        TextTable::InsertLineBreaksAndGetNumLines(text, pRatingStr, 560, font_type);
        DrawStringDefaultPalette(text, static_cast<u16>(40), static_cast<u16>(270), font_type, 1);
    }
}

MATCH_FUNC(0x4B7E10)
s32 Frontend::DrawKeyHint(u8 str_id_idx, u16 text_xpos, u16 text_ypos, u16 fontType, s32 palette)
{
    switch (str_id_idx)
    {
        case 0:
            swprintf(tmpBuff, gText_0x14->Find("esc_i"));
            break;
        case 1:
            swprintf(tmpBuff, gText_0x14->Find("del_i"));
            break;
        case 2:
            swprintf(tmpBuff, gText_0x14->Find("ent_i"));
            break;
        case 3:
            swprintf(tmpBuff, gText_0x14->Find("bspc_i"));
            break;
        case 4:
            swprintf(tmpBuff, gText_0x14->Find("up_i"));
            break;
        case 5:
            swprintf(tmpBuff, gText_0x14->Find("down_i"));
            break;
        case 6:
            swprintf(tmpBuff, gText_0x14->Find("left_i"));
            break;
        case 7:
            swprintf(tmpBuff, gText_0x14->Find("right_i"));
            break;
        case 8:
            swprintf(gTmpWideStr, gText_0x14->Find("edtnm_i"));
            swprintf(tmpBuff, L";  %s", gTmpWideStr);
            break;
        case 9:
            swprintf(gTmpWideStr, gText_0x14->Find("clrsl_i"));
            swprintf(tmpBuff, L";  %s", gTmpWideStr);
            break;
        case 10:
            swprintf(gTmpWideStr, gText_0x14->Find("delch_i"));
            swprintf(tmpBuff, L";  %s", gTmpWideStr);
            break;
        case 11:
            swprintf(gTmpWideStr, gText_0x14->Find("name_i"));
            swprintf(tmpBuff, L";  %s", gTmpWideStr);
            break;
        default:
            FatalError(Gta2Error::InvalidCase, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 8148);
            break;
    }

    if (static_cast<u16>(palette) == 0xFFFF)
    {
        DrawStringDefaultPalette(tmpBuff, text_xpos, text_ypos, fontType, 1);
    }
    else
    {
        DrawString(tmpBuff, text_xpos, text_ypos, fontType, 1, palette_types_enum::font_remaps_8, palette, 0, 0);
    }
    return GetMaxTextWidth(tmpBuff, fontType);
}

MATCH_FUNC(0x4B7FB0)
bool Frontend::AreAllStagesUnlocked()
{
    PlySlot* pPlayerSlot = GetCurrPlayerStats();
    u16 main_stage_idx = 0;
    // note: two separated while's interlaced by a backwards goto may be actually two nested while's
    while (main_stage_idx < mNumMainStages)
    {
        u16 bonus_stage_idx = 0;
        while (bonus_stage_idx < mNumBonusStages[main_stage_idx])
        {
            if (!pPlayerSlot->mPlyrStageStats[main_stage_idx][bonus_stage_idx].mIsStageUnlocked)
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
void Frontend::LoadMapFilenames(u8 mainBlockIdx, u8 bonusBlockIdx)
{
    char fullPath[256];
    char debugStr[256];
    char mapName[256];
    char styName[256];

    LoadStringsFromStage(mainBlockIdx, bonusBlockIdx, debugStr, mapName, styName);
    gGameSession.SetSaveFileName("");
    strcpy(fullPath, "data\\");
    strcat(fullPath, debugStr);
    gGameSession.SetMapName(fullPath);
    strcpy(fullPath, "data\\");
    strcat(fullPath, mapName);
    gGameSession.SetStyleName(fullPath);
    strcpy(fullPath, "data\\");
    strcat(fullPath, styName);
    gGameSession.SetScriptName(fullPath);
    if (!bonusBlockIdx)
    {
        gGameSession.SetMainStageIdx(mainBlockIdx);
        gGameSession.SetBonusStage(0);
    }
    else
    {
        gGameSession.SetStage(gGameSession.EncodeStage(mainBlockIdx, bonusBlockIdx));
        gGameSession.SetBonusStage(1);
    }
}

MATCH_FUNC(0x4AD0D0)
void Frontend::DrawLoading()
{
    const u16 x = GetCenteredXPos(gText_0x14->Find("loading"), -1, 320);

    DrawStringDefaultPalette(gText_0x14->Find("loading"), x, 260, mNormalFont, 1);
}

MATCH_FUNC(0x4ADDE0)
void Frontend::DrawDeletePlayerDialog()
{
    // A one-case switch: the original zero-extends the type and tests it with `dec`.
    switch (mDialogType)
    {
        case 1:
        {
            u16 ypos = !gText_0x14->LangIsJapanese() ? 12 : 16;
            DrawStringDefaultPalette(gText_0x14->Find("clrchar"), static_cast<u16>(275), ypos, mHeadingFont, 1);
            break;
        }
    }

    if (mDialogCursorYpos == 190)
    {
        DrawStringDefaultPalette(gText_0x14->Find("sure"), 300, 190, mSelectedFont, 1);
    }
    else
    {
        DrawStringDefaultPalette(gText_0x14->Find("sure"), 300, 190, mNormalFont, 1);
    }

    if (mDialogCursorYpos == 210)
    {
        DrawStringDefaultPalette(gText_0x14->Find("yes"), 300, 210, mSelectedFont, 1);
    }
    else
    {
        DrawStringDefaultPalette(gText_0x14->Find("yes"), 300, 210, mNormalFont, 1);
    }

    if (mDialogCursorYpos == 230)
    {
        DrawStringDefaultPalette(gText_0x14->Find("no"), 300, 230, mSelectedFont, 1);
    }
    else
    {
        DrawStringDefaultPalette(gText_0x14->Find("no"), 300, 230, mNormalFont, 1);
    }
}

MATCH_FUNC(0x4ADF50)
void Frontend::DrawCurrentState()
{
    switch (mState)
    {
        case FrontendState::Delete_Player_Dialog_4:
            DrawDeletePlayerDialog();
            break;

        case FrontendState::Menu_Navigation_1:
        case FrontendState::User_Typing_New_Player_Name_3:
        case FrontendState::User_Typing_Password_5:
            if (mCurrentPageIdx == MENUPAGE_CREDITS)
            {
                DrawCredits();
            }
            else
            {
                DrawMenu();
            }
            break;

        case FrontendState::Booting_Map_2:
            DrawLoading();
            break;

        default:
            FatalError(Gta2Error::InvalidCase, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 1217, mState);
            break;
    }
}

MATCH_FUNC(0x5D7DC0)
EXPORT void __cdecl FreeSurface()
{
    pVid_FreeSurface(gVidSys);
}

// 16.16 step along a line, 0 for a point
static inline s32 GetLineStep(s32 delta, s32 count)
{
    if (count == 0)
    {
        return count;
    }
    return (delta << 16) / count;
}

// Debug line, one gbh_Plot per pixel along the longer axis. x2 and y2 become the deltas, y2 then the y step
MATCH_FUNC(0x5D7DD0)
EXPORT void __stdcall DrawDebugLine(s32 x1, s32 y1, s32 x2, s32 y2, u16 colour)
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
        y2 = GetLineStep(y2, abs_dx);
        count = abs_dx;
    }
    else
    {
        y2 = y2 > 0 ? 0x10000 : -0x10000;
        step_x = GetLineStep(x2, abs_dy);
        count = abs_dy;
    }

    x1 <<= 16;
    y1 <<= 16;
    do
    {
        pgbh_Plot(static_cast<f32>(x1 >> 16), static_cast<f32>(y1 >> 16), 0, colour);
        x1 += step_x;
        y1 += y2;
        count--;
    } while (count > 0);
}

MATCH_FUNC(0x4ADFB0)
void Frontend::Render()
{
    MakeScreenTableAndSetWindow();

    pgbh_BeginScene();
    DrawBackground();
    DrawCurrentState();
    pgbh_EndScene();

    FreeSurface();

    pVid_FlipBuffers(gVidSys);

    pVid_ClearScreen(gVidSys, 0, 0, 0, 0, 0, gVidSys->field_48_rect_right, gVidSys->field_4C_rect_bottom);
}

// https://decomp.me/scratch/IOmk7
// The second GetTgaIdxsForMenuScreen output lives in a dword local that also takes the
// result of the last right-hand blit, and that result is tested afterwards. Without the store
// (and the read that keeps it alive until after tail merging) VC6 merges the two final retry
// blits into one tail; the original keeps them apart. 9.6f (0x453020) has the same store.
MATCH_FUNC(0x4B6E10)
void Frontend::DrawBackground()
{
    u8 tga_idx;
    s32 ret; // second tga idx then the blit result

    if (mMenuScreen == GameOver_13 || mMenuScreen == RedBar_16 || mMenuScreen == BlueBar_14 || mMenuScreen == Loading_15 ||
        mMenuScreen == HiScoresDisplay_12 || mMenuScreen == Credits_17)
    {
        GetTgaIdxsForMenuScreen(mMenuScreen, &tga_idx, reinterpret_cast<u8*>(&ret));
        if (pgbh_BlitImage(tgaArray[tga_idx].mImg, 0, 0, 640, 480, 0, 0) == -10)
        {
            // need to reload image
            Load_tga(tga_idx);
            pgbh_BlitImage(tgaArray[tga_idx].mImg, 0, 0, 640, 480, 0, 0);
        }
    }
    else
    {
        GetTgaIdxsForMenuScreen(mMenuScreen, &tga_idx, reinterpret_cast<u8*>(&ret));

        // Left side
        s32 blitRet = pgbh_BlitImage(tgaArray[tga_idx].mImg, 0, 0, 278, 480, 0, 0);
        if (blitRet == -10)
        {
            Load_tga(tga_idx);
            blitRet = pgbh_BlitImage(tgaArray[tga_idx].mImg, 0, 0, 278, 480, 0, 0);
        }

        // Right side
        if (blitRet == 0)
        {
            if (pgbh_BlitImage(tgaArray[static_cast<u8>(ret)].mImg, 0, 0, 362, 480, 278, 0) == -10)
            {
                Load_tga(static_cast<u8>(ret));
                ret = pgbh_BlitImage(tgaArray[static_cast<u8>(ret)].mImg, 0, 0, 362, 480, 278, 0);
                if (ret == -10)
                {
                    // still failing, nothing more to do (the read is needed for the match, see above)
                }
            }
        }
    }
}

MATCH_FUNC(0x4B6B00)
void Frontend::GetTgaIdxsForMenuScreen(u8 screen, u8* pTgaIdx, u8* pTgaIdx2)
{
    switch (screen)
    {
        case Options_0:
            *pTgaIdx = 1;
            *pTgaIdx2 = 0;
            break;

        case Play_1:
            *pTgaIdx = 2;
            *pTgaIdx2 = 0;
            break;

        case Quit_2:
            *pTgaIdx = 3;
            *pTgaIdx2 = 0;
            break;

        case BonusAC_3:
            *pTgaIdx = 5;
            *pTgaIdx2 = 4;
            break;

        case BonusDF_4:
            *pTgaIdx = 6;
            *pTgaIdx2 = 4;
            break;

        case BonusGI_5:
            *pTgaIdx = 7;
            *pTgaIdx2 = 4;
            break;

        case ViewHiScore_6:
            *pTgaIdx = 8;
            *pTgaIdx2 = 4;
            break;

        case PlayArea1_7:
            *pTgaIdx = 9;
            *pTgaIdx2 = 4;
            break;

        case PlayArea2_8:
            *pTgaIdx = 10;
            *pTgaIdx2 = 4;
            break;

        case PlayArea3_9:
            *pTgaIdx = 11;
            *pTgaIdx2 = 4;
            break;

        case EnterPlayerName_10:
            *pTgaIdx = 12;
            *pTgaIdx2 = 4;
            break;

        case ResumeLoadSave_11:
            *pTgaIdx = 13;
            *pTgaIdx2 = 4;
            break;

        case HiScoresDisplay_12:
            *pTgaIdx = 15;
            *pTgaIdx2 = 0;
            break;

        case GameOver_13:
            *pTgaIdx = 19;
            *pTgaIdx2 = 0;
            break;

        case RedBar_16:
            *pTgaIdx = 17;
            *pTgaIdx2 = 0;
            break;

        case BlueBar_14:
            *pTgaIdx = 18;
            *pTgaIdx2 = 0;
            break;

        case Loading_15:
            *pTgaIdx = 16;
            *pTgaIdx2 = 0;
            break;

        case Credits_17:
            *pTgaIdx = 22;
            *pTgaIdx2 = 0;
            break;

        default:
            return;
    }
}

MATCH_FUNC(0x4B6520)
void Frontend::Load_tga(u16 idx)
{
    Error_SetName(tgaArray[idx].mTgaName);

    FILE* hFile = crt::fopen(tgaArray[idx].mTgaName, "rb");
    if (!hFile)
    {
        FatalError(Gta2Error::FileOpenError, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 6516);
    }

    void* pAlloc = malloc(tgaArray[idx].mLen);
    if (!pAlloc)
    {
        FatalError(Gta2Error::TargaMemoryAllocationError, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 6523);
    }

    if (crt::fread(pAlloc, 1u, tgaArray[idx].mLen, hFile) != static_cast<u32>(tgaArray[idx].mLen))
    {
        FatalError(Gta2Error::InvalidBackgroundImageSize, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 6529);
    }

    tgaArray[idx].mImg = pgbh_LoadImage(static_cast<SImage*>(pAlloc));

    crt::fclose(hFile);
    free(pAlloc);
}

// https://decomp.me/scratch/MuqZh
MATCH_FUNC(0x4AF2A0)
Frontend::Frontend()
{
    SetField10D(1);

    gText_0x14 = new TextTable();
    if (!gText_0x14)
    {
        FatalError(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 2288);
    }

    gGtx_0x106C = new gtx_0x106C();
    if (!gGtx_0x106C)
    {
        FatalError(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 2290);
    }

    gTextureCache_15D8 = new TextureCache();
    if (!gTextureCache_15D8)
    {
        FatalError(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 2292);
    }

    if (gText_0x14->LangIsJapanese())
    {
        gKanjiFont = new KanjiFont();
        if (!gKanjiFont)
        {
            FatalError(Gta2Error::OutOfMemoryNewOperator, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 2297);
        }
    }
    InitKeyBoardDevice();

    gText_0x14->Load();
    gGtx_0x106C->LoadSty("data\\fstyle.sty");

    gTextureCache_15D8->LoadStyleTextures();

    SetColourDepthAndConvertBanks();

    pgbh_SetAmbient(1.0);

    if (gKanjiFont)
    {
        gKanjiFont->InitGlyphCaches();
    }

    mState = FrontendState::Menu_Navigation_1;
    mCursorBlink = 0;
    mCursorBlinkTimer = 0;
    mUpKeyDown = 0;
    mDownKeyDown = 0;
    mLeftKeyDown = 0;
    mRightKeyDown = 0;
    mReturnKeyDown = 0;
    mEscapeKeyDown = 0;
    mDeleteKeyDown = 0;
    mbKeyboardAcquireFailed = false;
    mWinmainNextState = Run_Frontend_2;
    mbCheatsEnabled = false;

    SetFontTypes();

    mNextFrameTime = timeGetTime();
    mUpdatesSinceRender = 0;
    mCurrentPageIdx = MENUPAGE_START_MENU;
    mLastInputTime = 0;

    SetupMenuStringsOptionsElements();

    mCurrPlyrNameLength = 0;
    mKeyHeld = true;
    mLastKey = 256;
    mKeyRepeatTimer = 5;

    // Constant-count zeroing loops: VC6 turns them into dword/word stores at fixed offsets off this,
    // each with its own zero register, where memset takes the address into a register first
    for (s32 k = 0; k < 9; k++)
    {
        mCurrPlyrName[k] = 0;
    }
    for (s32 k2 = 0; k2 < 9; k2++)
    {
        mPassword[k2] = 0;
    }
    mPasswordLength = 0;
    mWrongPasswordShown = false;
    mNumMainStages = 0;

    for (s32 k3 = 0; k3 < 3; k3++)
    {
        mNumBonusStages[k3] = 0;
    }

    GetMainAndBonusStagesFromSeqFile();
    LoadPlySlotSvgs();

    mMenuScreen = Play_1;

    Load_tgas();

    mHiscoreTableIdx = 0;
    mDialogCursorYpos = 190;
    mDialogType = 0;
    mCreditsScrollTimer = 0;
    mCreditsYpos = kFpZero_67D930;
    mCreditsLineIdx = 0;
    mHasPrevPlayerSlot = false;
    mHasNextPlayerSlot = false;
    mHasPrevMainStage = false;
    mHasNextMainStage = false;
    mHasPrevBonusStage = false;
    mHasNextBonusStage = false;

    for (u8 i = 0; i < 8; i++)
    {
        mSelectedMainStage[i] = -1;
        mSelectedBonusStage[i] = -1;
    }
}

MATCH_FUNC(0x4AF970)
Frontend::~Frontend()
{
    FreeKeyBoardDevice();

    if (gTextureCache_15D8)
    {
        GTA2_DELETE_AND_NULL(gTextureCache_15D8);
    }

    if (gGtx_0x106C)
    {
        GTA2_DELETE_AND_NULL(gGtx_0x106C);
    }

    if (gText_0x14)
    {
        GTA2_DELETE_AND_NULL(gText_0x14);
    }

    if (gKanjiFont)
    {
        GTA2_DELETE_AND_NULL(gKanjiFont);
    }

    FreeImageTable();
}

MATCH_FUNC(0x4AFD70)
void Frontend::AcquireKeyBoard()
{
    if (!mpKeyboardDevice || mpKeyboardDevice->Acquire() < 0)
    {
        mbKeyboardAcquireFailed = true;
    }
}

MATCH_FUNC(0x4AFD00)
void Frontend::FreeKeyBoardDevice()
{
    if (mpKeyboardDevice)
    {
        mpKeyboardDevice->Unacquire();
        mpKeyboardDevice->Release();
        mpKeyboardDevice = NULL;
    }
}

MATCH_FUNC(0x4B6750)
void Frontend::FreeImageTable()
{
    pgbh_FreeImageTable();
}

MATCH_FUNC(0x4AFDD0)
bool Frontend::KeyBoard_GetKeyStates()
{
    HRESULT hr = mpKeyboardDevice->GetDeviceState(256, mKeys);
    if (FAILED(hr))
    {
        if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED)
        {
            if (FAILED(mpKeyboardDevice->Acquire()))
            {
                return false;
            }

            mpKeyboardDevice->GetDeviceState(256, mKeys);
        }
        return false;
    }

    return true;
}

MATCH_FUNC(0x4AFBE0)
void Frontend::InitKeyBoardDevice()
{
    mpDInput = gpDInput;
    mpKeyboardDevice = NULL;

    if (mpDInput->CreateDevice(GUID_SysKeyboard, &mpKeyboardDevice, NULL) < 0)
    {
        FatalError(Gta2Error::DirectInputCreateFail, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 2469);
    }

    if (mpKeyboardDevice->SetDataFormat(&gKeyboardDataFormat) < 0)
    {
        FatalError(Gta2Error::DirectInputSetDataFormatFail, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 2480);
    }

    if (mpKeyboardDevice->SetCooperativeLevel(gHwnd, 6) < 0)
    {
        FatalError(Gta2Error::DirectInputSetCooperativeLevelFail, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 2487);
    }

    AcquireKeyBoard();
}

MATCH_FUNC(0x4AF0E0)
void Frontend::SetFontTypes()
{
    if (gText_0x14->LangIsJapanese())
    {
        this->mNormalFont = 101;
        this->mTextFont = 101;
        this->mSelectedFont = 102;
        this->mUnused122 = 101;
        this->mFontType = 101;
        this->mHeadingFont = 101;
        this->mPlayMenuScoreFont = 101;
        this->mScoreFont = 101;
        this->mAreaCompleteFont = 106;
        this->mUnused12E = 102;
        this->mTitleFont = 201;
    }
    else
    {
        this->mNormalFont = gFrontendNormalFont;
        this->mTextFont = gFrontendNormalFont;
        this->mSelectedFont = gFrontendSelectedFont;
        this->mUnused122 = gFrontendFont6;
        this->mFontType = font_type_703C14;
        this->mHeadingFont = gFrontendHeadingFont;
        this->mPlayMenuScoreFont = gFrontendPlayMenuScoreFont;
        this->mScoreFont = gFrontendScoreFont;
        this->mAreaCompleteFont = gFrontendAreaCompleteFont;
        this->mUnused12E = gFrontendFont12;
        this->mTitleFont = gFrontendTitleFont;
    }
}

MATCH_FUNC(0x4B0220)
void Frontend::SetupMenuStringsOptionsElements()
{
    // Setup all menu pages, options and elements

    MenuPage* pPage;
    s32 line_height = !gText_0x14->LangIsJapanese() ? 12 : 16;
    mUnused134 = 16;

    pPage = &mMenuPagesArray[0];
    pPage->mNumberOfOptions = 3;
    pPage->mOptionsArray[0].mOptionType = ENTERABLE_1;
    pPage->mOptionsArray[0].mXPos = 300;
    pPage->mOptionsArray[0].mYPos = 250;
    wcsncpy(pPage->mOptionsArray[0].mOptionNameStr, gText_0x14->Find("play"), 0x32u);
    pPage->mOptionsArray[0].mMenuPageTarget = 1;
    pPage->mOptionsArray[1].mOptionType = ENTERABLE_1;
    pPage->mOptionsArray[1].mXPos = 300;
    pPage->mOptionsArray[1].mYPos = 270;
    wcsncpy(pPage->mOptionsArray[1].mOptionNameStr,
            gText_0x14->Find("options"),
            0x32u);
    pPage->mOptionsArray[1].mMenuPageTarget = 257;
    pPage->mOptionsArray[2].mOptionType = ENTERABLE_1;
    pPage->mOptionsArray[2].mXPos = 300;
    pPage->mOptionsArray[2].mYPos = 290;
    wcsncpy(pPage->mOptionsArray[2].mOptionNameStr, gText_0x14->Find("quit"), 0x32u);
    pPage->mOptionsArray[2].mMenuPageTarget = 9;
    pPage->mOptionStates[0].mUnused0 = 280;
    pPage->mOptionStates[0].mUnused2 = 258;
    pPage->mOptionStates[1].mUnused0 = 280;
    pPage->mOptionStates[1].mUnused2 = 278;
    pPage->mOptionStates[2].mUnused0 = 280;
    pPage->mOptionStates[2].mUnused2 = 298;
    pPage->mCurrentOptionIdx = 0;
    pPage->mDefaultOptionIdx = 0;
    pPage = &mMenuPagesArray[1];
    pPage->mNumberOfOptions = 5;
    pPage->mOptionsArray[0].mOptionType = HORIZONTAL_SELECTION_2;
    pPage->mOptionsArray[0].mXPos = 300;
    pPage->mOptionsArray[0].mYPos = 210;
    wcsncpy(pPage->mOptionsArray[0].mOptionNameStr,
            gText_0x14->Find("charctr"),
            0x32u);
    pPage->mOptionsArray[0].mHorizontalSelectedIdx = 0;
    pPage->mOptionsArray[0].mUnused70 = 0;
    pPage->mOptionsArray[0].mHorizontalMaxIdx = 7;

    u16 enabled_idx = 0;
    do
    {
        pPage->mOptionsArray[0].mHorizontalIdxEnabled[enabled_idx++] = 1;
    } while (enabled_idx <= pPage->mOptionsArray[0].mHorizontalMaxIdx);

    pPage->mOptionsArray[1].mOptionType = ENTERABLE_1;
    pPage->mOptionsArray[1].mXPos = 300;
    pPage->mOptionsArray[1].mYPos = 230;
    wcsncpy(pPage->mOptionsArray[1].mOptionNameStr,
            gText_0x14->Find("savepos"),
            0x32u);
    pPage->mOptionsArray[1].mMenuPageTarget = 260;
    pPage->mOptionsArray[2].mOptionType = ENTERABLE_1;
    pPage->mOptionsArray[2].mXPos = 300;
    pPage->mOptionsArray[2].mYPos = 250;
    wcsncpy(pPage->mOptionsArray[2].mOptionNameStr,
            gText_0x14->Find("hi_scre"),
            0x32u);
    pPage->mOptionsArray[2].mMenuPageTarget = 5;
    pPage->mOptionsArray[3].mOptionType = ENTERABLE_1;
    pPage->mOptionsArray[3].mXPos = 300;
    pPage->mOptionsArray[3].mYPos = 270;
    wcsncpy(pPage->mOptionsArray[3].mOptionNameStr,
            gText_0x14->Find("strtlev"),
            0x32u);
    pPage->mOptionsArray[3].mMenuPageTarget = 264;
    pPage->mOptionsArray[4].mOptionType = ENTERABLE_1;
    pPage->mOptionsArray[4].mXPos = 300;
    pPage->mOptionsArray[4].mYPos = 350;
    wcsncpy(pPage->mOptionsArray[4].mOptionNameStr,
            gText_0x14->Find("bonslev"),
            0x32u);
    pPage->mOptionsArray[4].mMenuPageTarget = 265;
    pPage->mOptionStates[0].mUnused0 = 280;
    pPage->mOptionStates[0].mUnused2 = 218;
    pPage->mOptionStates[1].mUnused0 = 280;
    pPage->mOptionStates[1].mUnused2 = 238;
    pPage->mOptionStates[2].mUnused0 = 280;
    pPage->mOptionStates[2].mUnused2 = 258;
    pPage->mOptionStates[3].mUnused0 = 280;
    pPage->mOptionStates[3].mUnused2 = 278;
    pPage->mOptionStates[4].mUnused0 = 280;
    pPage->mOptionStates[4].mUnused2 = 358;
    pPage->mCurrentOptionIdx = 3;
    pPage->mDefaultOptionIdx = 3;
    pPage->mNumberOfElements = 10;
    pPage->mElementsArray[0].mElementType = GEOMETRIC_SHAPE_3;
    pPage->mElementsArray[0].mXpos = 420;
    pPage->mElementsArray[0].mYpos = 310;
    pPage->mElementsArray[0].mGeometricShapeType = 0;
    pPage->mElementsArray[1].mElementType = GEOMETRIC_SHAPE_3;
    pPage->mElementsArray[1].mXpos = 420;
    pPage->mElementsArray[1].mYpos = 390;
    pPage->mElementsArray[1].mGeometricShapeType = 0;
    pPage->mElementsArray[2].mElementType = STRING_TEXT_1;
    pPage->mElementsArray[2].mXpos = 410;
    pPage->mElementsArray[2].mYpos = 298;
    wcsncpy(pPage->mElementsArray[2].mElementNameStr,
            gText_0x14->Find("car_dam"),
            0x32u);
    pPage->mElementsArray[2].mFontType = gFrontendHeadingFont;
    pPage->mElementsArray[3].mElementType = STRING_TEXT_1;
    pPage->mElementsArray[3].mXpos = 410;
    pPage->mElementsArray[3].mYpos = 378;
    wcsncpy(pPage->mElementsArray[3].mElementNameStr,
            gText_0x14->Find("car_dam"),
            0x32u);
    pPage->mElementsArray[3].mFontType = gFrontendHeadingFont;
    pPage->mElementsArray[4].mElementType = GEOMETRIC_SHAPE_3;
    pPage->mElementsArray[4].mXpos = 380;
    pPage->mElementsArray[4].mYpos = 310;
    pPage->mElementsArray[4].mGeometricShapeType = 3;
    pPage->mElementsArray[5].mElementType = GEOMETRIC_SHAPE_3;
    pPage->mElementsArray[5].mXpos = 460;
    pPage->mElementsArray[5].mYpos = 310;
    pPage->mElementsArray[5].mGeometricShapeType = 4;
    pPage->mElementsArray[6].mElementType = GEOMETRIC_SHAPE_3;
    pPage->mElementsArray[6].mXpos = 380;
    pPage->mElementsArray[6].mYpos = 390;
    pPage->mElementsArray[6].mGeometricShapeType = 3;
    pPage->mElementsArray[7].mElementType = GEOMETRIC_SHAPE_3;
    pPage->mElementsArray[7].mXpos = 460;
    pPage->mElementsArray[7].mYpos = 390;
    pPage->mElementsArray[7].mGeometricShapeType = 4;
    pPage->mElementsArray[8].mElementType = GEOMETRIC_SHAPE_3;
    pPage->mElementsArray[8].mXpos = 290;
    pPage->mElementsArray[8].mYpos = 222;
    pPage->mElementsArray[8].mGeometricShapeType = 3;
    pPage->mElementsArray[9].mElementType = GEOMETRIC_SHAPE_3;
    pPage->mElementsArray[9].mXpos = 580;
    pPage->mElementsArray[9].mYpos = 222;
    pPage->mElementsArray[9].mGeometricShapeType = 4;
    pPage = &mMenuPagesArray[11];
    pPage->mNumberOfOptions = 3;
    pPage->mNumberOfElements = 1;
    pPage->mElementsArray[0].mElementType = STRING_TEXT_1;
    pPage->mElementsArray[0].mXpos = 35;
    pPage->mElementsArray[0].mYpos = 11;
    wcscpy(pPage->mElementsArray[0].mElementNameStr, gText_0x14->Find("plr_qut"));
    pPage->mElementsArray[0].mFontType = mTitleFont;
    pPage->mElementsArray[0].mFontPalette = 5;
    pPage->mOptionsArray[0].mOptionType = ENTERABLE_1;
    pPage->mOptionsArray[0].mYPos = 392;
    wcsncpy(pPage->mOptionsArray[0].mOptionNameStr,
            gText_0x14->Find("savepos"),
            0x32u);
    pPage->mOptionsArray[0].mXPos =
        Frontend::GetCenteredXPos(pPage->mOptionsArray[0].mOptionNameStr,
                             pPage->mOptionsArray[0].mFontType,
                             320);
    pPage->mOptionsArray[0].mMenuPageTarget = 260;
    pPage->mOptionsArray[1].mOptionType = ENTERABLE_1;
    pPage->mOptionsArray[1].mYPos = 412;
    wcsncpy(pPage->mOptionsArray[1].mOptionNameStr,
            gText_0x14->Find("replay"),
            0x32u);
    pPage->mOptionsArray[1].mXPos =
        Frontend::GetCenteredXPos(pPage->mOptionsArray[1].mOptionNameStr,
                             pPage->mOptionsArray[1].mFontType,
                             320);
    pPage->mOptionsArray[1].mMenuPageTarget = 259;
    pPage->mOptionsArray[2].mOptionType = ENTERABLE_1;
    pPage->mOptionsArray[2].mYPos = 432;
    wcsncpy(pPage->mOptionsArray[2].mOptionNameStr,
            gText_0x14->Find("mainmen"),
            0x32u);
    pPage->mOptionsArray[2].mXPos =
        Frontend::GetCenteredXPos(pPage->mOptionsArray[2].mOptionNameStr,
                             pPage->mOptionsArray[2].mFontType,
                             320);
    pPage->mOptionsArray[2].mMenuPageTarget = 0;
    pPage->mOptionStates[0].mUnused0 = 150;
    pPage->mOptionStates[0].mUnused2 = 400;
    pPage->mOptionStates[1].mUnused0 = 150;
    pPage->mOptionStates[1].mUnused2 = 420;
    pPage->mOptionStates[2].mUnused0 = 150;
    pPage->mOptionStates[2].mUnused2 = 440;
    pPage->mCurrentOptionIdx = 0;
    pPage->mDefaultOptionIdx = 0;
    pPage = &mMenuPagesArray[2];
    pPage->mNumberOfOptions = 3;
    pPage->mNumberOfElements = 1;
    pPage->mElementsArray[0].mElementType = STRING_TEXT_1;
    pPage->mElementsArray[0].mXpos = 35;
    pPage->mElementsArray[0].mYpos = 11;
    wcsncpy(pPage->mElementsArray[0].mElementNameStr,
            gText_0x14->Find("plr_ded"),
            0x32u);
    pPage->mElementsArray[0].mFontType = mTitleFont;
    pPage->mElementsArray[0].mFontPalette = 0;
    pPage->mOptionsArray[0].mOptionType = ENTERABLE_1;
    pPage->mOptionsArray[0].mYPos = 392;
    wcsncpy(pPage->mOptionsArray[0].mOptionNameStr,
            gText_0x14->Find("savepos"),
            0x32u);
    pPage->mOptionsArray[0].mXPos =
        Frontend::GetCenteredXPos(pPage->mOptionsArray[0].mOptionNameStr,
                             pPage->mOptionsArray[0].mFontType,
                             320);
    pPage->mOptionsArray[0].mMenuPageTarget = 260;
    pPage->mOptionsArray[1].mOptionType = ENTERABLE_1;
    pPage->mOptionsArray[1].mYPos = 412;
    wcsncpy(pPage->mOptionsArray[1].mOptionNameStr,
            gText_0x14->Find("replay"),
            0x32u);
    pPage->mOptionsArray[1].mXPos =
        Frontend::GetCenteredXPos(pPage->mOptionsArray[1].mOptionNameStr,
                             pPage->mOptionsArray[1].mFontType,
                             320);
    pPage->mOptionsArray[1].mMenuPageTarget = 259;
    pPage->mOptionsArray[2].mOptionType = ENTERABLE_1;
    pPage->mOptionsArray[2].mYPos = 432;
    wcsncpy(pPage->mOptionsArray[2].mOptionNameStr,
            gText_0x14->Find("mainmen"),
            0x32u);
    pPage->mOptionsArray[2].mXPos =
        Frontend::GetCenteredXPos(pPage->mOptionsArray[2].mOptionNameStr,
                             pPage->mOptionsArray[2].mFontType,
                             320);
    pPage->mOptionsArray[2].mMenuPageTarget = 0;
    pPage->mOptionStates[0].mUnused0 = 150;
    pPage->mOptionStates[0].mUnused2 = 400;
    pPage->mOptionStates[1].mUnused0 = 150;
    pPage->mOptionStates[1].mUnused2 = 420;
    pPage->mOptionStates[2].mUnused0 = 150;
    pPage->mOptionStates[2].mUnused2 = 440;
    pPage->mCurrentOptionIdx = 0;
    pPage->mDefaultOptionIdx = 0;
    pPage = &mMenuPagesArray[3];
    pPage->mNumberOfOptions = 5;
    pPage->mNumberOfElements = 1;
    pPage->mElementsArray[0].mElementType = STRING_TEXT_1;
    pPage->mElementsArray[0].mXpos = 35;
    pPage->mElementsArray[0].mYpos = 11;
    wcsncpy(pPage->mElementsArray[0].mElementNameStr,
            gText_0x14->Find("cmpltd"),
            0x32u);
    pPage->mElementsArray[0].mFontType = mAreaCompleteFont;
    pPage->mOptionsArray[0].mOptionType = ENTERABLE_1;
    pPage->mOptionsArray[0].mYPos = 365;
    wcsncpy(pPage->mOptionsArray[0].mOptionNameStr,
            gText_0x14->Find("nxt_lvl"),
            0x32u);
    pPage->mOptionsArray[0].mXPos =
        Frontend::GetCenteredXPos(pPage->mOptionsArray[0].mOptionNameStr,
                             pPage->mOptionsArray[0].mFontType,
                             320);
    pPage->mOptionsArray[0].mMenuPageTarget = 261;
    pPage->mOptionsArray[1].mOptionType = ENTERABLE_1;
    pPage->mOptionsArray[1].mYPos = 385;
    wcsncpy(pPage->mOptionsArray[1].mOptionNameStr,
            gText_0x14->Find("savepos"),
            0x32u);
    pPage->mOptionsArray[1].mXPos =
        Frontend::GetCenteredXPos(pPage->mOptionsArray[1].mOptionNameStr,
                             pPage->mOptionsArray[1].mFontType,
                             320);
    pPage->mOptionsArray[1].mMenuPageTarget = 260;
    pPage->mOptionsArray[2].mOptionType = ENTERABLE_1;
    pPage->mOptionsArray[2].mYPos = 405;
    wcsncpy(pPage->mOptionsArray[2].mOptionNameStr,
            gText_0x14->Find("replay"),
            0x32u);
    pPage->mOptionsArray[2].mXPos =
        Frontend::GetCenteredXPos(pPage->mOptionsArray[2].mOptionNameStr,
                             pPage->mOptionsArray[2].mFontType,
                             320);
    pPage->mOptionsArray[2].mMenuPageTarget = 259;
    pPage->mOptionsArray[3].mOptionType = ENTERABLE_1;
    pPage->mOptionsArray[3].mYPos = 425;
    wcsncpy(pPage->mOptionsArray[3].mOptionNameStr,
            gText_0x14->Find("contnue"),
            0x32u);
    pPage->mOptionsArray[3].mXPos =
        Frontend::GetCenteredXPos(pPage->mOptionsArray[3].mOptionNameStr,
                             pPage->mOptionsArray[3].mFontType,
                             320);
    pPage->mOptionsArray[3].mMenuPageTarget = 266;
    pPage->mOptionsArray[4].mOptionType = ENTERABLE_1;
    pPage->mOptionsArray[4].mYPos = 445;
    wcsncpy(pPage->mOptionsArray[4].mOptionNameStr,
            gText_0x14->Find("mainmen"),
            0x32u);
    pPage->mOptionsArray[4].mXPos =
        Frontend::GetCenteredXPos(pPage->mOptionsArray[4].mOptionNameStr,
                             pPage->mOptionsArray[4].mFontType,
                             320);
    pPage->mOptionsArray[4].mMenuPageTarget = 0;
    pPage->mOptionStates[0].mUnused0 = 150;
    pPage->mOptionStates[0].mUnused2 = 373;
    pPage->mOptionStates[1].mUnused0 = 150;
    pPage->mOptionStates[1].mUnused2 = 393;
    pPage->mOptionStates[2].mUnused0 = 150;
    pPage->mOptionStates[2].mUnused2 = 413;
    pPage->mOptionStates[3].mUnused0 = 150;
    pPage->mOptionStates[3].mUnused2 = 433;
    pPage->mOptionStates[4].mUnused0 = 150;
    pPage->mOptionStates[4].mUnused2 = 453;
    pPage->mCurrentOptionIdx = 0;
    pPage->mDefaultOptionIdx = 0;
    pPage = &mMenuPagesArray[4];
    pPage->mNumberOfOptions = 1;
    pPage->mNumberOfElements = 1;
    pPage->mElementsArray[0].mElementType = STRING_TEXT_1;
    pPage->mElementsArray[0].mYpos = 230;
    wcsncpy(pPage->mElementsArray[0].mElementNameStr,
            gText_0x14->Find("gam_cmp"),
            0x32u);
    s16 title_font = mTitleFont;
    pPage->mElementsArray[0].mFontType = title_font;
    pPage->mElementsArray[0].mXpos =
        Frontend::GetCenteredXPos(pPage->mElementsArray[0].mElementNameStr, title_font, 320);
    pPage->mElementsArray[0].mFontPalette = 4;
    pPage->mOptionsArray[0].mOptionType = ENTERABLE_1;
    pPage->mOptionsArray[0].mXPos = 180;
    pPage->mOptionsArray[0].mYPos = 410;
    wcsncpy(pPage->mOptionsArray[0].mOptionNameStr,
            gText_0x14->Find("mainmen"),
            0x32u);
    pPage->mOptionsArray[0].mMenuPageTarget = 0;
    pPage->mOptionStates[0].mUnused0 = 160;
    pPage->mOptionStates[0].mUnused2 = 418;
    pPage->mCurrentOptionIdx = 0;
    pPage->mDefaultOptionIdx = 0;
    pPage = &mMenuPagesArray[5];
    pPage->mNumberOfOptions = 1;
    pPage->mNumberOfElements = 5;
    pPage->mOptionsArray[0].mOptionType = HORIZONTAL_SELECTION_2;
    pPage->mOptionsArray[0].mXPos = 300;
    pPage->mOptionsArray[0].mYPos = 155;
    pPage->mOptionsArray[0].mHorizontalSelectedIdx = 0;
    pPage->mOptionsArray[0].mUnused70 = 0;
    pPage->mOptionsArray[0].mHorizontalMaxIdx = 11;

    u16 enabled_idx2 = 0;
    do
    {
        pPage->mOptionsArray[0].mHorizontalIdxEnabled[enabled_idx2++] = 1;
    } while (enabled_idx2 <= pPage->mOptionsArray[0].mHorizontalMaxIdx);

    pPage->mOptionStates[0].mUnused0 = 280;
    pPage->mOptionStates[0].mUnused2 = 163;
    pPage->mCurrentOptionIdx = 0;
    pPage->mDefaultOptionIdx = 0;
    pPage->mElementsArray[0].mElementType = GEOMETRIC_SHAPE_3;
    pPage->mElementsArray[0].mXpos = 450;
    pPage->mElementsArray[0].mYpos = 197;
    pPage->mElementsArray[0].mGeometricShapeType = 0;
    pPage->mElementsArray[1].mElementType = STRING_TEXT_1;
    pPage->mElementsArray[1].mXpos = 440;
    pPage->mElementsArray[1].mYpos = 185;
    pPage->mElementsArray[1].mFontType = gFrontendHeadingFont;
    pPage->mElementsArray[2].mYpos = 197;
    pPage->mElementsArray[3].mYpos = 197;
    pPage->mElementsArray[2].mElementType = GEOMETRIC_SHAPE_3;
    pPage->mElementsArray[2].mXpos = 410;
    pPage->mElementsArray[2].mGeometricShapeType = 3;
    pPage->mElementsArray[3].mElementType = GEOMETRIC_SHAPE_3;
    pPage->mElementsArray[3].mXpos = 490;
    pPage->mElementsArray[3].mGeometricShapeType = 4;
    pPage->mElementsArray[4].mElementType = STRING_TEXT_1;
    pPage->mElementsArray[4].mXpos = 340;
    pPage->mElementsArray[4].mYpos = line_height;
    wcsncpy(pPage->mElementsArray[4].mElementNameStr,
            gText_0x14->Find("hi_scre"),
            0x32u);
    pPage->mElementsArray[4].mFontType = mHeadingFont;
    pPage = &mMenuPagesArray[6];
    pPage->mNumberOfOptions = 3;
    pPage->mNumberOfElements = 3;
    pPage->mElementsArray[0].mElementType = STRING_TEXT_1;
    pPage->mElementsArray[0].mXpos = 35;
    pPage->mElementsArray[0].mYpos = 11;
    wcsncpy(pPage->mElementsArray[0].mElementNameStr,
            gText_0x14->Find("bonslev"),
            0x32u);
    pPage->mElementsArray[0].mFontType = mTitleFont;
    pPage->mElementsArray[0].mFontPalette = 5;
    pPage->mElementsArray[1].mElementType = STRING_TEXT_1;
    pPage->mElementsArray[1].mXpos = 170;
    pPage->mElementsArray[1].mYpos = 250;
    wcsncpy(pPage->mElementsArray[1].mElementNameStr,
            gText_0x14->Find("score"),
            0x32u);
    pPage->mElementsArray[2].mElementType = STRING_TEXT_1;
    pPage->mElementsArray[2].mXpos = 400;
    pPage->mElementsArray[2].mYpos = 250;
    pPage->mElementsArray[2].mFontType = mSelectedFont;
    pPage->mOptionsArray[0].mOptionType = ENTERABLE_1;
    pPage->mOptionsArray[0].mYPos = 340;
    wcsncpy(pPage->mOptionsArray[0].mOptionNameStr,
            gText_0x14->Find("repbons"),
            0x32u);
    // The original reads the option's own (never written) font field here, not mSelectedFont
    pPage->mOptionsArray[0].mXPos =
        Frontend::GetCenteredXPos(pPage->mOptionsArray[0].mOptionNameStr,
                             pPage->mOptionsArray[0].mFontType,
                             320);
    pPage->mOptionsArray[0].mMenuPageTarget = 259;
    pPage->mOptionsArray[1].mOptionType = ENTERABLE_1;
    pPage->mOptionsArray[1].mYPos = 360;
    wcsncpy(pPage->mOptionsArray[1].mOptionNameStr,
            gText_0x14->Find("nxt_lvl"),
            0x32u);
    pPage->mOptionsArray[1].mXPos =
        Frontend::GetCenteredXPos(pPage->mOptionsArray[1].mOptionNameStr,
                             pPage->mOptionsArray[1].mFontType,
                             320);
    pPage->mOptionsArray[1].mMenuPageTarget = 261;
    pPage->mOptionsArray[2].mOptionType = ENTERABLE_1;
    pPage->mOptionsArray[2].mYPos = 380;
    wcsncpy(pPage->mOptionsArray[2].mOptionNameStr,
            gText_0x14->Find("mainmen"),
            0x32u);
    pPage->mOptionsArray[2].mXPos =
        Frontend::GetCenteredXPos(pPage->mOptionsArray[2].mOptionNameStr,
                             pPage->mOptionsArray[2].mFontType,
                             320);
    pPage->mOptionsArray[2].mMenuPageTarget = 0;
    pPage->mOptionStates[0].mUnused0 = 150;
    pPage->mOptionStates[0].mUnused2 = 348;
    pPage->mOptionStates[1].mUnused0 = 150;
    pPage->mOptionStates[1].mUnused2 = 368;
    pPage->mOptionStates[2].mUnused0 = 150;
    pPage->mOptionStates[2].mUnused2 = 388;
    pPage->mCurrentOptionIdx = 0;
    pPage->mDefaultOptionIdx = 0;
    pPage = &mMenuPagesArray[7];
    pPage->mNumberOfOptions = 1;
    pPage->mNumberOfElements = 14;
    pPage->mElementsArray[0].mElementType = STRING_TEXT_1;
    pPage->mElementsArray[0].mXpos = 35;
    pPage->mElementsArray[0].mYpos = 11;
    pPage->mElementsArray[0].mFontType = mTitleFont;
    pPage->mElementsArray[0].mFontPalette = 5;
    pPage->mElementsArray[1].mElementType = STRING_TEXT_1;
    pPage->mElementsArray[1].mXpos = 100;
    pPage->mElementsArray[1].mYpos = 170;
    wcsncpy(pPage->mElementsArray[1].mElementNameStr, gEmptyWStr, 50u);
    pPage->mElementsArray[2].mElementType = STRING_TEXT_1;
    pPage->mElementsArray[2].mXpos = 100;
    pPage->mElementsArray[2].mYpos = 190;
    wcsncpy(pPage->mElementsArray[2].mElementNameStr, gEmptyWStr, 0x32u);
    pPage->mElementsArray[3].mElementType = STRING_TEXT_1;
    pPage->mElementsArray[3].mXpos = 100;
    pPage->mElementsArray[3].mYpos = 210;
    wcsncpy(pPage->mElementsArray[3].mElementNameStr, gEmptyWStr, 0x32u);
    pPage->mElementsArray[4].mElementType = STRING_TEXT_1;
    pPage->mElementsArray[4].mXpos = 100;
    pPage->mElementsArray[4].mYpos = 230;
    wcsncpy(pPage->mElementsArray[4].mElementNameStr, gEmptyWStr, 0x32u);
    pPage->mElementsArray[5].mElementType = STRING_TEXT_1;
    pPage->mElementsArray[5].mXpos = 100;
    pPage->mElementsArray[5].mYpos = 250;
    wcsncpy(pPage->mElementsArray[5].mElementNameStr, gEmptyWStr, 0x32u);
    pPage->mElementsArray[6].mElementType = STRING_TEXT_1;
    pPage->mElementsArray[6].mXpos = 100;
    pPage->mElementsArray[6].mYpos = 270;
    wcsncpy(pPage->mElementsArray[6].mElementNameStr, gEmptyWStr, 0x32u);
    pPage->mElementsArray[7].mElementType = STRING_TEXT_1;
    pPage->mElementsArray[7].mYpos = 300;
    wcsncpy(pPage->mElementsArray[7].mElementNameStr,
            gText_0x14->Find("kills_h"),
            0x32u);
    pPage->mElementsArray[7].mXpos =
        GetCenteredXPos(pPage->mElementsArray[7].mElementNameStr,
                   pPage->mElementsArray[7].mFontType,
                   320);
    pPage->mElementsArray[8].mElementType = STRING_TEXT_1;
    pPage->mElementsArray[8].mXpos = 100;
    pPage->mElementsArray[8].mYpos = 320;
    pPage->mElementsArray[9].mElementType = STRING_TEXT_1;
    pPage->mElementsArray[9].mXpos = 100;
    pPage->mElementsArray[9].mYpos = 340;
    pPage->mElementsArray[10].mElementType = STRING_TEXT_1;
    pPage->mElementsArray[10].mXpos = 100;
    pPage->mElementsArray[10].mYpos = 360;
    pPage->mElementsArray[11].mElementType = STRING_TEXT_1;
    pPage->mElementsArray[11].mXpos = 100;
    pPage->mElementsArray[11].mYpos = 380;
    pPage->mElementsArray[12].mElementType = STRING_TEXT_1;
    pPage->mElementsArray[12].mXpos = 100;
    pPage->mElementsArray[12].mYpos = 400;
    pPage->mElementsArray[13].mElementType = STRING_TEXT_1;
    pPage->mElementsArray[13].mXpos = 30;
    pPage->mElementsArray[13].mYpos = 150;
    pPage->mOptionsArray[0].mOptionType = ENTERABLE_1;
    pPage->mOptionsArray[0].mYPos = 430;
    wcsncpy(pPage->mOptionsArray[0].mOptionNameStr, gText_0x14->Find("quit"), 0x32u);
    pPage->mOptionsArray[0].mXPos =
        GetCenteredXPos(pPage->mOptionsArray[0].mOptionNameStr,
                   pPage->mOptionsArray[0].mFontType,
                   320);
    pPage->mOptionsArray[0].mMenuPageTarget = 258;
    pPage->mOptionStates[0].mUnused0 = 180;
    pPage->mOptionStates[0].mUnused2 = 438;
    pPage->mCurrentOptionIdx = 0;
    pPage->mDefaultOptionIdx = 0;
    pPage = &mMenuPagesArray[8];
    pPage->mNumberOfOptions = 1;
    pPage->mNumberOfElements = 0;
    pPage->mOptionsArray[0].mOptionType = ENTERABLE_1;
    pPage->mOptionsArray[0].mXPos = 200;
    pPage->mOptionsArray[0].mYPos = 280;
    wcsncpy(pPage->mOptionsArray[0].mOptionNameStr,
            gText_0x14->Find("mainmen"),
            0x32u);
    pPage->mOptionsArray[0].mMenuPageTarget = 0;
    pPage->mOptionStates[0].mUnused0 = 180;
    pPage->mOptionStates[0].mUnused2 = 288;
    pPage->mCurrentOptionIdx = 0;
    pPage->mDefaultOptionIdx = 0;
    pPage = &mMenuPagesArray[10];
    pPage->mNumberOfOptions = 1;
    pPage->mNumberOfElements = 1;
    pPage->mElementsArray[0].mElementType = STRING_TEXT_1;
    pPage->mElementsArray[0].mYpos = 230;
    wcsncpy(pPage->mElementsArray[0].mElementNameStr,
            gText_0x14->Find("nicetry"),
            0x32u);
    s16 title_font2 = mTitleFont;
    pPage->mElementsArray[0].mFontType = title_font2;
    pPage->mElementsArray[0].mXpos =
        GetCenteredXPos(pPage->mElementsArray[0].mElementNameStr, title_font2, 320);
    pPage->mElementsArray[0].mFontPalette = 4;
    pPage->mOptionsArray[0].mOptionType = ENTERABLE_1;
    pPage->mOptionsArray[0].mXPos = 180;
    pPage->mOptionsArray[0].mYPos = 410;
    wcsncpy(pPage->mOptionsArray[0].mOptionNameStr,
            gText_0x14->Find("mainmen"),
            0x32u);
    pPage->mOptionsArray[0].mMenuPageTarget = 0;
    pPage->mOptionStates[0].mUnused0 = 160;
    pPage->mOptionStates[0].mUnused2 = 418;
    pPage->mCurrentOptionIdx = 0;
    pPage->mDefaultOptionIdx = 0;
    pPage = &mMenuPagesArray[14];
    pPage->mNumberOfOptions = 1;
    pPage->mNumberOfElements = 5;
    pPage->mOptionsArray[0].mOptionType = ENTERABLE_1;
    pPage->mOptionsArray[0].mXPos = 170;
    pPage->mOptionsArray[0].mYPos = 340;
    pPage->mOptionsArray[0].mMenuPageTarget = 268;
    pPage->mElementsArray[0].mElementType = STRING_TEXT_1;
    pPage->mElementsArray[0].mXpos = 20;
    pPage->mElementsArray[0].mYpos = 160;
    wcsncpy(pPage->mElementsArray[0].mElementNameStr,
            gText_0x14->Find("fr_ent1"),
            0x32u);
    pPage->mElementsArray[1].mElementType = STRING_TEXT_1;
    pPage->mElementsArray[1].mXpos = 20;
    pPage->mElementsArray[1].mYpos = 180;
    wcsncpy(pPage->mElementsArray[1].mElementNameStr,
            gText_0x14->Find("fr_ent2"),
            0x32u);
    pPage->mElementsArray[2].mElementType = STRING_TEXT_1;
    pPage->mElementsArray[2].mXpos = 20;
    pPage->mElementsArray[2].mYpos = 200;
    wcsncpy(pPage->mElementsArray[2].mElementNameStr, gEmptyWStr, 0x32u);
    pPage->mElementsArray[3].mElementType = STRING_TEXT_1;
    pPage->mElementsArray[3].mXpos = 20;
    pPage->mElementsArray[3].mYpos = 300;
    wcsncpy(pPage->mElementsArray[3].mElementNameStr,
            gText_0x14->Find("fr_pmpt"),
            0x32u);
    pPage->mElementsArray[4].mElementType = STRING_TEXT_1;
    pPage->mElementsArray[4].mXpos = 20;
    pPage->mElementsArray[4].mYpos = 320;
    wcsncpy(pPage->mElementsArray[4].mElementNameStr,
            gText_0x14->Find("score"),
            0x32u);
    pPage->mOptionStates[0].mUnused0 = 150;
    pPage->mOptionStates[0].mUnused2 = 348;
    pPage->mCurrentOptionIdx = 0;
    pPage->mDefaultOptionIdx = 0;
    mCredits.LoadCredits();
}

// The GMP/STY/SCR names read from a MAIN or BONUS block of the .seq file. A struct (not three
// arrays) because its field order fixes the stack layout: STY, GMP, SCR (same in 9.6f).
struct SeqStageFiles
{
    char mSty[256]; // 0x0
    char mGmp[256]; // 0x100
    char mScr[256]; // 0x200
};

MATCH_FUNC(0x4B4440)
void Frontend::GetMainAndBonusStagesFromSeqFile()
{
    u8* pBlock;
    char mainOrBonus[256];
    char seqFileName[256];
    _finddata_t findInfo;

    long hFind = _findfirst("data\\*.seq", &findInfo);
    if (hFind == -1)
    {
        FatalError(Gta2Error::SeqFileNotFound, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 4876);
    }
    else
    {
        if (!_findnext(hFind, &findInfo))
        {
            FatalError(Gta2Error::MultipleSeqFilesFound, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 4883);
        }

        strcpy(seqFileName, "data\\");
        strcat(seqFileName, findInfo.name);
        _findclose(hFind);
    }

    this->mNumMainStages = 0;
    bool mainBlockFound = false;
    for (s32 i = 0; i < 3; i++)
    {
        this->mNumBonusStages[i] = 0;
    }
    u16 main_block_counter = 0;

    FILE* hSeqFile = crt::fopen(seqFileName, "rt");
    if (!hSeqFile)
    {
        FatalError(Gta2Error::SeqFileOpenError, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 4906);
        return;
    }

    GetSeqItem(0, mainOrBonus, hSeqFile);

    while (strcmp(mainOrBonus, "") != 0)
    {
        SeqStageFiles files;
        char description[256];
        if (strcmp(mainOrBonus, "MAIN") == 0)
        {
            if (mainBlockFound)
            {
                if (++main_block_counter > 2u)
                {
                    FatalError(Gta2Error::TooManyMainBlocks, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 4922);
                }
            }
            mainBlockFound = true;
            pBlock = &this->mNumBonusStages[main_block_counter];
            *pBlock = 0;

            GetSeqItem(1, files.mGmp, hSeqFile);
            GetSeqItem(2, files.mSty, hSeqFile);
            GetSeqItem(3, files.mScr, hSeqFile);
            GetSeqItem(4, description, hSeqFile);
            StoreStringsForStage(main_block_counter, *pBlock, files.mGmp, files.mSty, files.mScr);
            ++*pBlock;
        }
        else if (strcmp(mainOrBonus, "BONUS") == 0)
        {
            if (!mainBlockFound)
            {
                FatalError(Gta2Error::MainBlockMustPrecedeBonus, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 4940);
            }
            pBlock = &mNumBonusStages[main_block_counter];
            if (*pBlock > 3u)
            {
                FatalError(Gta2Error::TooManyBonusBlocks, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 4945);
            }

            GetSeqItem(1, files.mGmp, hSeqFile);
            GetSeqItem(2, files.mSty, hSeqFile);
            GetSeqItem(3, files.mScr, hSeqFile);
            GetSeqItem(4, description, hSeqFile);
            StoreStringsForStage(main_block_counter, *pBlock, files.mGmp, files.mSty, files.mScr);
            ++*pBlock;
        }
        else
        {
            FatalError(Gta2Error::InvalidBlockType, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 4959);
        }

        GetSeqItem(0, mainOrBonus, hSeqFile);
    }

    mNumMainStages = main_block_counter + 1;
    crt::fclose(hSeqFile);
}

MATCH_FUNC(0x4B48D0)
void Frontend::GetSeqItem(u8 type, char_type* ppRet, FILE* hSeqFile)
{
    char_type type_buf[52];
    char_type output_buf[256];

    u16 pos = 0;
    char_type letter = File::SkipWhitespace(hSeqFile);
    while (!letter)
    {
        if (feof(hSeqFile))
        {
            strcpy(ppRet, "");
            return;
        }
        letter = File::SkipWhitespace(hSeqFile);
    }

    if (letter)
    {
        if ((letter < 'a' || letter > 'z') && (letter < 'A' || letter > 'Z'))
        {
            FatalError(Gta2Error::InvalidFirstLineCharacter, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 5014);
        }

        while (letter != '=')
        {
            output_buf[pos++] = letter;
            letter = File::SkipWhitespace(hSeqFile);
            if (!letter)
            {
                FatalError(Gta2Error::LineInterruptedByNewline, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 5026);
            }
            if (pos > 0xFFu)
            {
                FatalError(Gta2Error::LabelTooLong, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 5031);
            }
        } // 0x3d
        output_buf[pos] = 0;

        switch (type)
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
                FatalError(Gta2Error::UndefinedLabel, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 5056);
                break;
        }

        if (strcmp(type_buf, output_buf) == 0)
        {
            pos = 0;
            letter = File::SkipWhitespace(hSeqFile);
            if (letter)
            {
                do
                {
                    output_buf[pos++] = letter;
                    letter = File::SkipWhitespace(hSeqFile);
                    if (pos > 255u)
                    {
                        FatalError(Gta2Error::LineDataTooLong, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 5074);
                    }
                } while (letter);
            }
            output_buf[pos] = 0;
            strcpy(ppRet, output_buf);
        }
        else
        {
            FatalError(Gta2Error::UnexpectedLabel, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 5086);
        }
    }
}

MATCH_FUNC(0x4B53C0)
void Frontend::LoadPlySlotSvgs()
{
    char_type FileName[256];
    for (u8 i = 0; i < GTA2_COUNTOF(mPlySlots); i++)
    {
        if (PlySlotSvgExists(i))
        {
            GetPlySlotSvgName(i, FileName);
            mPlySlots[i].LoadPlySlotSvg(FileName);
        }
        else
        {
            mPlySlots[i].mSaveExists = false;
            mPlySlots[i].mLastSavedStage = 3;
            mPlySlots[i].mLastSavedBonusStageCode = 4;
            mPlySlots[i].mLastSavedIsBonus = 0;
        }
    }
}

MATCH_FUNC(0x4B66B0)
void Frontend::Load_tgas()
{
    if (pgbh_InitImageTable(gTableSize_61FF20) != -1)
    {
        for (u16 i = 0; i < gTableSize_61FF20; ++i)
        {
            Load_tga(i);
        }
    }
}

MATCH_FUNC(0x4B51D0)
void Frontend::GetPlySlotSvgName(u8 idx, char_type* pStr)
{
    char_type Buffer[8];
    _itoa(idx, Buffer, 10);
    strcpy(pStr, "player\\plyslot");
    strcat(pStr, Buffer);
    strcat(pStr, ".svg");
}

MATCH_FUNC(0x4B5270)
void Frontend::DrawSavedStage()
{
    u8 plySlotIdx = gGameSession.GetPlySlotIdx();
    u8 codified_stages = mPlySlots[plySlotIdx].mLastSavedBonusStageCode;

    u8 main_stage;
    u8 bonus_stage;

    if (!mPlySlots[plySlotIdx].mLastSavedIsBonus)
    {
        bonus_stage = 0;
        main_stage = mPlySlots[plySlotIdx].mLastSavedStage;
    }
    else
    {
        gGameSession.DecodeStage(codified_stages, &main_stage, &bonus_stage);
    }
    swprintf(tmpBuff, L"%d", main_stage);
    DrawStringDefaultPalette(tmpBuff, static_cast<u16>(450), static_cast<u16>(90), mNormalFont, 1);

    swprintf(tmpBuff, L"%d", bonus_stage);
    DrawStringDefaultPalette(tmpBuff, static_cast<u16>(450), static_cast<u16>(110), mNormalFont, 1);
}

MATCH_FUNC(0x4B5370)
bool Frontend::PlySlotSvgExists(u8 idx)
{
    char_type FileName[256];
    GetPlySlotSvgName(idx, FileName);

    _finddata_t findData;
    long hFind = _findfirst(FileName, &findData);
    if (hFind == -1)
    {
        return false;
    }

    _findclose(hFind);
    return true;
}

MATCH_FUNC(0x4B77B0)
u8 Frontend::GetLastUnlockedMainStage(PlySlot* pStats)
{
    u8 result;

    for (result = mNumMainStages - 1; !pStats->mPlyrStageStats[result][0].mIsStageUnlocked; --result)
    {
        if (result <= 0)
        {
            break;
        }
    }
    return result;
}

MATCH_FUNC(0x4B7800)
u8 Frontend::GetLastUnlockedBonusStageCode(PlySlot* pStats)
{
    u8 stage = Frontend::GetLastUnlockedMainStage(pStats);
    u8 bonus = mNumBonusStages[stage] - 1;
    while (1)
    {
        if (bonus == 0)
        {
            if (stage == 0)
            {
                return -1;
            }
            stage--;
            bonus = mNumBonusStages[stage] - 1;
        }
        else
        {
            while (!pStats->mPlyrStageStats[stage][bonus].mIsStageUnlocked && bonus > 0)
            {
                bonus--;
            }
            if (pStats->mPlyrStageStats[stage][bonus].mIsStageUnlocked == 1 && bonus > 0)
            {
                break;
            }
        }
    }
    return gGameSession.EncodeStage(stage, bonus);
}

EXTERN_GLOBAL(bool, bDoFrontEnd);

MATCH_FUNC(0x5E53C0)
void __stdcall Frontend::SetInputEnabled(u8* pEnabled)
{
    if (bDoFrontEnd)
    {
        if (gFrontend)
        {
            gFrontend->mbInputEnabled = *pEnabled;
        }
    }
    else
    {
        gInputRecorder.mbInputEnabled = *pEnabled;
    }
}

MATCH_FUNC(0x5D8990)
s32 __stdcall Frontend::GetMaxTextWidth(wchar_t* pStr, u16 font_type)
{
    /*If pStr has 1 line, this will return num of characters of pStr. 
    If pStr has multiple lines, this will return the num of characters of the line which have more 
    characters of all lines.*/ 

    wchar_t* pStrIter = pStr;
    s32 current = 0;
    s32 spaceSize = gGtx_0x106C->GetSpaceCharWidth(&font_type);
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
                current += gGtx_0x106C->GetFontWidth(&font_type, pStrIter);
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
void Frontend::DrawTextFixedWidth(wchar_t* pString, u16 text_xpos, u16 text_ypos, u16 font_type, u16 palette, u16 scale, u16 char_width, bool bRightAlign)
{
    u16 text_xbase;

    if (bRightAlign)
    {
        u16 char_count = 0;
        for (; pString[char_count]; ++char_count)
        {
            ;
        }
        text_xbase = text_xpos - char_width * (char_count - 1);
    }
    else
    {
        text_xbase = text_xpos;
    }

    wchar_t chr[2] = {0};
    u16 text_xposa = 0;

    for (chr[0] = pString[0]; chr[0]; chr[0] = pString[++text_xposa])
    {
        u16 biggestLine = Frontend::GetMaxTextWidth(chr, font_type);
        u16 char_pad = (char_width - biggestLine) / 2;
        if (static_cast<u16>(palette) == 0xFFFF)
        {
            DrawStringDefaultPalette(chr, text_xbase + char_pad, text_ypos, font_type, scale);
        }
        else
        {
            DrawString(chr, text_xbase + char_pad, text_ypos, font_type, scale, palette_types_enum::font_remaps_8, palette, 0, 0);
        }
        text_xbase += char_width;
    }
}

MATCH_FUNC(0x4B55F0)
void Frontend::DrawMultiplayerScores()
{
    // Draw page containing the match results according to the game mode
    s8 game_mode = gGameSession.GetMultiplayerGamemode();
    u8 max_players = gGameSession.GetMaxPlayers();
    u8 user_idx = gGameSession.GetUserPlayerIdx();

    u8 idx_2 = 0;

    for (u8 curr_plyr_idx = 0; curr_plyr_idx < max_players; ++curr_plyr_idx)
    {
        u16 x_pos;
        u16 y_pos;
        wchar_t Buffer[26];

        if (game_mode == FRAG_GAME_1) //  frags
        {
            s32 frags = static_cast<s16>(gGameSession.GetFragsForPlayerIdx(curr_plyr_idx));
            _itow(frags, Buffer, 10);
            x_pos = 550;
            y_pos = 20 * curr_plyr_idx + 170;
        }
        else if (game_mode == POINTS_GAME_2) //  points game
        {
            s32 points = gGameSession.GetPointsForPlayerIdx(curr_plyr_idx);
            _itow(points, Buffer, 10);
            x_pos = 550;
            y_pos = 20 * curr_plyr_idx + 170;
        }
        else // tag game
        {
            s32 player_time = gTagGame.GetTime(curr_plyr_idx);
            swprintf(Buffer, L"%2d:%02d", player_time / 60, player_time % 60);
            x_pos = 500;
            y_pos = 20 * curr_plyr_idx + 170;
        }

        DrawStringDefaultPalette(Buffer, x_pos, y_pos, mNormalFont, 1);

        s32 frags = gGameSession.GetFragsOnPlayer(user_idx, curr_plyr_idx);
        _itow(frags, Buffer, 10);

        if (game_mode != TAG_GAME_3 && curr_plyr_idx != user_idx)
        {
            x_pos = 550;
            y_pos = 20 * idx_2 + 320;
            DrawStringDefaultPalette(Buffer, x_pos, y_pos, mNormalFont, 1);
            ++idx_2;
        }
    }
}

MATCH_FUNC(0x4B57B0)
void Frontend::DrawLastAndBestStats(u16 text_xpos, u16 text_ypos)
{
    u16 font_type = mScoreFont;
    s32 line_height = gText_0x14->mLangCode != 106 ? 14 : 16;
    u8 main_stage_idx = gGameSession.GetMainStageIdx();

    if (gText_0x14->mLangCode == 106)
    {
        text_ypos += 5;
    }
    wchar_t* _5B5F90 = gText_0x14->Find("last");
    swprintf(tmpBuff, _5B5F90);

    s32 x = Frontend::GetMaxTextWidth(tmpBuff, font_type);

    u16 x_pos;
    u16 y_pos;
    DrawStringDefaultPalette(tmpBuff, static_cast<u16>(text_xpos - x + 494), static_cast<u16>(text_ypos - 15), font_type, 1);

    swprintf(tmpBuff, gText_0x14->Find("best"));

    x = Frontend::GetMaxTextWidth(tmpBuff, font_type);

    y_pos = text_ypos - 15;
    DrawStringDefaultPalette(tmpBuff, static_cast<u16>(text_xpos - x + 624), y_pos, font_type, 1);

    if (gText_0x14->mLangCode == 106)
    {
        text_ypos += 5;
    }

    //  vehicles hijacked

    x_pos = text_xpos;
    y_pos = text_ypos;
    DrawStringDefaultPalette(gText_0x14->Find("carjack"), x_pos, y_pos, font_type, 1);

    swprintf(tmpBuff, L"%d", gGameSession.GetStatistic(5));
    u16 x_pos_last = text_xpos + 480;
    Frontend::DrawTextFixedWidth(tmpBuff, x_pos_last, text_ypos, font_type, 10, 1, line_height, true);

    swprintf(tmpBuff, L"%d", *reinterpret_cast<u32*>(&gPlyDat->mBestStats[main_stage_idx].mBestStatistics[20]));
    u16 x_pos_best = text_xpos + 610;
    Frontend::DrawTextFixedWidth(tmpBuff, x_pos_best, text_ypos, font_type, 10, 1, line_height, true);

    //  auto damage cost

    x_pos = text_xpos;
    y_pos = text_ypos + 20;
    DrawStringDefaultPalette(gText_0x14->Find("car_cst"), x_pos, y_pos, font_type, 1);

    swprintf(tmpBuff, L"$%d", gGameSession.GetCarDamageCost());
    Frontend::DrawTextFixedWidth(tmpBuff, x_pos_last, y_pos, font_type, 10, 1, line_height, true);

    swprintf(tmpBuff, L"$%d", gPlyDat->mBestCarDamageCost[main_stage_idx]);
    Frontend::DrawTextFixedWidth(tmpBuff, x_pos_best, y_pos, font_type, 10, 1, line_height, true);

    //  civilians run down

    x_pos = text_xpos;
    y_pos = text_ypos + 40;
    DrawStringDefaultPalette(gText_0x14->Find("run_ovr"), x_pos, y_pos, font_type, 1);

    swprintf(tmpBuff, L"%d", gGameSession.GetStatistic(6u));
    Frontend::DrawTextFixedWidth(tmpBuff, x_pos_last, y_pos, font_type, 10, 1, line_height, true);

    swprintf(tmpBuff, L"%d", *reinterpret_cast<u32*>(&gPlyDat->mBestStats[main_stage_idx].mBestStatistics[24]));
    Frontend::DrawTextFixedWidth(tmpBuff, x_pos_best, y_pos, font_type, 10, 1, line_height, true);

    //  civilians murdered

    x_pos = text_xpos;
    y_pos = text_ypos + 60;
    DrawStringDefaultPalette(gText_0x14->Find("murder"), x_pos, y_pos, font_type, 1);

    swprintf(tmpBuff, L"%d", gGameSession.GetStatistic(7u));
    Frontend::DrawTextFixedWidth(tmpBuff, x_pos_last, y_pos, font_type, 10, 1, line_height, true);

    swprintf(tmpBuff, L"%d", *reinterpret_cast<u32*>(&gPlyDat->mBestStats[main_stage_idx].mBestStatistics[28]));
    Frontend::DrawTextFixedWidth(tmpBuff, x_pos_best, y_pos, font_type, 10, 1, line_height, true);

    //  lawmen killed

    if (!bIsFrench)
    {
        x_pos = text_xpos;
        y_pos = text_ypos + 80;
        DrawStringDefaultPalette(gText_0x14->Find("cop_kl"), x_pos, y_pos, font_type, 1);

        swprintf(tmpBuff, L"%d", gGameSession.GetStatistic(8u));
        Frontend::DrawTextFixedWidth(tmpBuff, x_pos_last, y_pos, font_type, 10, 1, line_height, true);

        swprintf(tmpBuff, L"%d", *reinterpret_cast<u32*>(&gPlyDat->mBestStats[main_stage_idx].mBestStatistics[32]));
        Frontend::DrawTextFixedWidth(tmpBuff, x_pos_best, y_pos, font_type, 10, 1, line_height, true);
    }

    //  gang members killed

    x_pos = text_xpos;
    y_pos = text_ypos + 100;
    DrawStringDefaultPalette(gText_0x14->Find("gng_kl"), x_pos, y_pos, font_type, 1);

    swprintf(tmpBuff, L"%d", gGameSession.GetStatistic(9u));
    Frontend::DrawTextFixedWidth(tmpBuff, x_pos_last, y_pos, font_type, 10, 1, line_height, true);

    swprintf(tmpBuff, L"%d", *reinterpret_cast<u32*>(&gPlyDat->mBestStats[main_stage_idx].mBestStatistics[36]));
    Frontend::DrawTextFixedWidth(tmpBuff, x_pos_best, y_pos, font_type, 10, 1, line_height, true);

    //  fugitive factor

    x_pos = text_xpos;
    y_pos = text_ypos + 120;
    DrawStringDefaultPalette(gText_0x14->Find("evsnrtg"), x_pos, y_pos, font_type, 1);

    swprintf(tmpBuff, L"%d", gGameSession.GetEvasionRating());
    Frontend::DrawTextFixedWidth(tmpBuff, x_pos_last, y_pos, font_type, 10, 1, line_height, true);

    swprintf(tmpBuff, L"%d", gPlyDat->mBestEvasionRating[main_stage_idx]);
    Frontend::DrawTextFixedWidth(tmpBuff, x_pos_best, y_pos, font_type, 10, 1, line_height, true);
}

MATCH_FUNC(0x4B0190)
u16 Frontend::GetCenteredXPos(wchar_t* pText, u16 fontType, s32 width)
{
    u16 half_width;
    if (fontType != 0xFFFF)
    {
        half_width = static_cast<u16>(GetMaxTextWidth(pText, fontType)) / 2;
    }
    else
    {
        half_width = static_cast<u16>(GetMaxTextWidth(pText, mNormalFont)) / 2;
    }
    return width - half_width;
}

MATCH_FUNC(0x4B7060)
u8 Frontend::GetPreviousUnlockedMainStage(u8 stage_idx)
{
    PlySlot* pStats = GetCurrPlayerStats();
    u8 result = stage_idx;
    if (stage_idx == 0)
    {
        if (bIsLeftRightLoopEnabled)
        {
            result = 2;
            while (!pStats->mPlyrStageStats[result][0].mIsStageUnlocked)
            {
                --result;
            }
        }
        return result;
    }
    else
    {
        stage_idx--;
        return stage_idx;
    }
}

MATCH_FUNC(0x4B7270)
u8 Frontend::GetNextUnlockedMainStage(char_type main_stage_idx)
{
    PlySlot* pStats = GetCurrPlayerStats();
    u8 result = main_stage_idx;
    if (main_stage_idx == 2)
    {
        if (bIsLeftRightLoopEnabled)
        {
            result = 0;
        }
    }
    else
    {
        result = main_stage_idx + 1;
        while (!pStats->mPlyrStageStats[result][0].mIsStageUnlocked)
        {
            if (result == 2)
            {
                result = bIsLeftRightLoopEnabled != 0 ? 0 : main_stage_idx;
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
bool Frontend::ExistsPreviousMainStage()
{
    u8 main_stage_idx = gGameSession.GetMainStageIdx();
    bool result = GetPreviousUnlockedMainStage(main_stage_idx) != main_stage_idx;
    return result;
}

MATCH_FUNC(0x4B74C0)
bool Frontend::ExistsNextMainStage()
{
    char_type curr_main_stage = gGameSession.GetMainStageIdx();
    bool result = static_cast<char_type>(GetNextUnlockedMainStage(curr_main_stage)) != curr_main_stage;
    return result;
}

MATCH_FUNC(0x4B7550)
void Frontend::UpdateMainStageArrows()
{
    MenuPage* pMenuPage = &mMenuPagesArray[mCurrentPageIdx];
    u8 main_stage_idx = gGameSession.GetMainStageIdx();
    swprintf(tmpBuff, L"%d", main_stage_idx + 1);
    wcsncpy(pMenuPage->mElementsArray[2].mElementNameStr, tmpBuff, 0x32u);

    if (ExistsPreviousMainStage())
    {
        pMenuPage->mElementsArray[4].mIsItDisplayed = true;
        mHasPrevMainStage = true;
    }
    else
    {
        pMenuPage->mElementsArray[4].mIsItDisplayed = false;
        mHasPrevMainStage = false;
    }

    if (ExistsNextMainStage())
    {
        pMenuPage->mElementsArray[5].mIsItDisplayed = true;
        mHasNextMainStage = true;
    }
    else
    {
        pMenuPage->mElementsArray[5].mIsItDisplayed = false;
        mHasNextMainStage = false;
    }
}

MATCH_FUNC(0x4B6FF0)
bool Frontend::ChangeMainStageToPrevious()
{
    u8 main_stage_idx = gGameSession.GetMainStageIdx();
    u8 old_main_stage_idx = main_stage_idx;
    main_stage_idx = GetPreviousUnlockedMainStage(main_stage_idx);
    gGameSession.SetMainStageIdx(main_stage_idx);
    mSelectedMainStage[gGameSession.GetPlySlotIdx()] = main_stage_idx;
    UpdateMainStageArrows();
    bool result = (old_main_stage_idx != main_stage_idx);
    return result;
}

MATCH_FUNC(0x4B42B0)
void Frontend::StripPlayerNameToCurrLength()
{
    u16 name_length = mCurrPlyrNameLength;
    for (u16 i = name_length; i < 9; i++)
    {
        mCurrPlyrName[i] = 0;
    }
}

// https://decomp.me/scratch/2DKTF
MATCH_FUNC(0x4B7120)
char_type Frontend::GetPreviousUnlockedBonusStage(u8 stage)
{
    PlySlot* player_stats = Frontend::GetCurrPlayerStats();

    u8 main_stage_idx;
    u8 bonus_stage_idx;
    gGameSession.DecodeStage(stage, &main_stage_idx, &bonus_stage_idx);

    u8 main_og = main_stage_idx;
    u8 bonus_og = bonus_stage_idx;
    bool bFirstIteration = true;

    while (!player_stats->mPlyrStageStats[main_stage_idx][bonus_stage_idx].mIsStageUnlocked || bFirstIteration)
    {
        bFirstIteration = false;
        if (bonus_stage_idx == 1)
        {
            do
            {
                if (main_stage_idx == 0)
                {
                    if (bIsLeftRightLoopEnabled)
                    {
                        main_stage_idx = mNumMainStages - 1;
                        bonus_stage_idx = mNumBonusStages[main_stage_idx] - 1;
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
                    bonus_stage_idx = mNumBonusStages[main_stage_idx] - 1;
                }
            } while (bonus_stage_idx == 0);
        }
        else
        {
            --bonus_stage_idx;
        }
    }
    return gGameSession.EncodeStage(main_stage_idx, bonus_stage_idx);
}

MATCH_FUNC(0x4B7610)
void Frontend::UpdateBonusStageArrows()
{
    MenuPage* pPage = &mMenuPagesArray[mCurrentPageIdx];
    u8 stage = gGameSession.GetStage();
    u8 main_idx;
    u8 bonus_idx;
    gGameSession.DecodeStage(stage, &main_idx, &bonus_idx);
    if (stage == 0xFF)
    {
        pPage->mOptionsArray[4].mIsUnlocked = 0;
        pPage->mOptionStates[4].mIsOptionUnlocked = 0;
        pPage->mElementsArray[3].mIsItDisplayed = false;
        pPage->mElementsArray[1].mIsItDisplayed = false;
        pPage->mElementsArray[6].mIsItDisplayed = false;
        pPage->mElementsArray[7].mIsItDisplayed = false;
    }
    else
    {
        pPage->mOptionsArray[4].mIsUnlocked = 1;
        pPage->mOptionStates[4].mIsOptionUnlocked = 1;
        pPage->mElementsArray[3].mIsItDisplayed = true;
        pPage->mElementsArray[1].mIsItDisplayed = true;
        pPage->mElementsArray[6].mIsItDisplayed = true;
        pPage->mElementsArray[7].mIsItDisplayed = true;
        if (ExistsPreviousBonusStage())
        {
            pPage->mElementsArray[6].mIsItDisplayed = true;
            mHasPrevBonusStage = true;
        }
        else
        {
            pPage->mElementsArray[6].mIsItDisplayed = false;
            mHasPrevBonusStage = false;
        }

        if (ExistsNextBonusStage())
        {
            pPage->mElementsArray[7].mIsItDisplayed = true;
            mHasNextBonusStage = true;
        }
        else
        {
            pPage->mElementsArray[7].mIsItDisplayed = false;
            mHasNextBonusStage = false;
        }

        swprintf(gTmpWideStr, L"%c", 3 * main_idx + bonus_idx + 64);
        wcsncpy(pPage->mElementsArray[3].mElementNameStr, gTmpWideStr, 0x32u);
    }
}

MATCH_FUNC(0x4B70B0)
bool Frontend::ChangeBonusStageToPrevious()
{
    s8 stage = gGameSession.GetStage();
    s8 start_stage = stage;
    stage = GetPreviousUnlockedBonusStage(stage);
    gGameSession.SetStage(stage);
    mSelectedBonusStage[gGameSession.GetPlySlotIdx()] = stage;
    UpdateBonusStageArrows();
    bool result = start_stage != stage;
    return result;
}

MATCH_FUNC(0x4B74F0)
bool Frontend::ExistsPreviousBonusStage()
{
    char_type stage = gGameSession.GetStage();
    bool result = GetPreviousUnlockedBonusStage(stage) != stage;
    return result;
}

// https://decomp.me/scratch/kyxJQ
MATCH_FUNC(0x4B7360)
char_type Frontend::GetNextUnlockedBonusStage(u8 stage)
{
    PlySlot* player_stats = GetCurrPlayerStats();

    u8 main_stage_idx;
    u8 bonus_stage_idx;
    gGameSession.DecodeStage(stage, &main_stage_idx, &bonus_stage_idx);

    u8 og_main_stage_idx = main_stage_idx;
    u8 og_bonus_stage_idx = bonus_stage_idx;

    bool bFirstIteration = true;

    while (!player_stats->mPlyrStageStats[main_stage_idx][bonus_stage_idx].mIsStageUnlocked || bFirstIteration)
    {
        bFirstIteration = false;
        if (bonus_stage_idx == mNumBonusStages[main_stage_idx] - 1)
        {
            bonus_stage_idx = 1;
            if (main_stage_idx == mNumMainStages - 1)
            {
                if (bIsLeftRightLoopEnabled)
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

            while (mNumBonusStages[main_stage_idx] == 1)
            {
                if (main_stage_idx == mNumMainStages - 1)
                {
                    if (bIsLeftRightLoopEnabled)
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

    return gGameSession.EncodeStage(main_stage_idx, bonus_stage_idx);
}

MATCH_FUNC(0x4B7520)
bool Frontend::ExistsNextBonusStage()
{
    char_type stage = gGameSession.GetStage();
    bool result = GetNextUnlockedBonusStage(stage) != stage;
    return result;
}

MATCH_FUNC(0x4B72F0)
bool Frontend::ChangeBonusStageToNext()
{
    char_type stage = gGameSession.GetStage();
    char_type start_stage = stage;
    stage = GetNextUnlockedBonusStage(stage);
    gGameSession.SetStage(stage);
    mSelectedBonusStage[gGameSession.GetPlySlotIdx()] = stage;
    UpdateBonusStageArrows();
    bool result = start_stage != stage;
    return result;
}

MATCH_FUNC(0x4B7200)
bool Frontend::ChangeMainStageToNext()
{
    char_type main_stage_idx = gGameSession.GetMainStageIdx();
    char_type old_main_stage_idx = main_stage_idx;
    main_stage_idx = GetNextUnlockedMainStage(main_stage_idx);
    gGameSession.SetMainStageIdx(main_stage_idx);
    mSelectedMainStage[gGameSession.GetPlySlotIdx()] = main_stage_idx;
    UpdateMainStageArrows();
    bool result = old_main_stage_idx != main_stage_idx;
    return result;
}

MATCH_FUNC(0x4B4EC0)
void Frontend::LoadMapFilenamesFromSvg()
{
    char_type FileName[256];
    {
        u8 plySlotIdx = gGameSession.GetPlySlotIdx();
        GetPlySlotSvgName(plySlotIdx, FileName);
    }
    File::Global_Open(FileName);

    SaveSlotInfo svg;
    {
        u32 len = sizeof(SaveSlotInfo);
        File::Global_Read(&svg, len);
    }

    File::Global_Close();

    u8 main_stage = svg.mLastSavedStage;
    u8 codified_stages = svg.mLastSavedBonusStageCode;
    u8 bCodified = svg.mLastSavedIsBonus;
    u8 bonus_stage;

    if (!bCodified)
    {
        bonus_stage = 0;
    }
    else
    {
        gGameSession.DecodeStage(codified_stages, &main_stage, &bonus_stage);
    }

    char_type path[256];
    strcpy(path, "data\\");
    strcat(path, mBlocks[main_stage][bonus_stage].mGmpName);
    if (strcmp(svg.mMapName, path))
    {
        FatalError(Gta2Error::GmpFilenameMismatch, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 5254);
    }

    strcpy(path, "data\\");
    strcat(path, mBlocks[main_stage][bonus_stage].mStyName);
    if (strcmp(svg.mStyleName, path))
    {
        FatalError(Gta2Error::StyFilenameMismatch, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 5262);
    }

    strcpy(path, "data\\");
    strcat(path, mBlocks[main_stage][bonus_stage].mScrName);
    if (strcmp(svg.mScriptName, path))
    {
        FatalError(Gta2Error::ScrFilenameMismatch, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 5270);
    }

    gGameSession.SetSaveFileName(FileName);
    gGameSession.SetMapName(svg.mMapName);
    gGameSession.SetStyleName(svg.mStyleName);
    gGameSession.SetScriptName(svg.mScriptName);
    gGameSession.SetMainStageIdx(main_stage);
    gGameSession.SetStage(codified_stages);
    gGameSession.SetBonusStage(bCodified);
}

MATCH_FUNC(0x4B6070)
MenuPage::MenuPage()
{
    mNumberOfOptions = 0;
    mNumberOfElements = 0;
    mCurrentOptionIdx = 0;
    mDefaultOptionIdx = 0;
}

MATCH_FUNC(0x4B6110)
MenuPage::~MenuPage()
{
    mNumberOfOptions = 0;
    mNumberOfElements = 0;
    mCurrentOptionIdx = 0;
    mDefaultOptionIdx = 0;
}

MATCH_FUNC(0x4B61B0)
bool MenuPage::SelectPrevOption()
{
    u16 oldIdx = mCurrentOptionIdx;
    do
    {
        if (!mCurrentOptionIdx)
        {
            mCurrentOptionIdx = mNumberOfOptions - 1;
        }
        else
        {
            mCurrentOptionIdx--;
        }
    } while (!mOptionStates[mCurrentOptionIdx].mIsOptionUnlocked);
    return oldIdx != mCurrentOptionIdx ? true : false;
}

MATCH_FUNC(0x4B6200)
bool MenuPage::SelectNextOption()
{
    u16 oldIdx = mCurrentOptionIdx;
    do
    {
        if (mCurrentOptionIdx == mNumberOfOptions - 1)
        {
            mCurrentOptionIdx = 0;
        }
        else
        {
            mCurrentOptionIdx++;
        }
    } while (!mOptionStates[mCurrentOptionIdx].mIsOptionUnlocked);
    return oldIdx != mCurrentOptionIdx ? true : false;
}

MATCH_FUNC(0x4B63E0)
MenuElement::MenuElement()
{
    mElementType = NULL_TYPE_0;
    mXpos = 0;
    mYpos = 0;
    mIsItDisplayed = true;
    wcscpy(mElementNameStr, gEmptyWStr);
    mFontType = -1;
    mFontPalette = -1;
}

MATCH_FUNC(0x4B6420)
MenuElement::~MenuElement()
{
    mIsItDisplayed = true;
    mElementType = NULL_TYPE_0;
    mXpos = 0;
    mYpos = 0;
    mFontType = -1;
    mFontPalette = -1;
}

MATCH_FUNC(0x4B6290)
MenuItem::MenuItem()
{
    mFontType = -1;
    mPalette = -1;
    mOptionType = NO_TYPE_0;
    mIsUnlocked = 1;
    mXPos = 0;
    mYPos = 0;
    mHorizontalSelectedIdx = 0;
    mUnused70 = 0;

    for (s32 i = 0; i < GTA2_COUNTOF(mHorizontalIdxEnabled); i++)
    {
        mHorizontalIdxEnabled[i] = 0;
    }

    wcscpy(mOptionNameStr, gEmptyWStr);
    mHorizontalMaxIdx = 0;
    mMenuPageTarget = 0;
}

MATCH_FUNC(0x4B62F0)
MenuItem::~MenuItem()
{
    mOptionType = NO_TYPE_0;
    mIsUnlocked = 1;
    mXPos = 0;
    mYPos = 0;
    mFontType = -1;
    mPalette = -1;
    mHorizontalSelectedIdx = 0;
    mUnused70 = 0;
    mHorizontalMaxIdx = 0;
    mMenuPageTarget = 0;
}

MATCH_FUNC(0x4B6330)
bool MenuItem::SelectNextHorizontalIdx()
{
    u8 bLoopEnabled = bIsLeftRightLoopEnabled;
    u16 old_count = mHorizontalSelectedIdx;
    u16 new_count = old_count;
    bool bFound = false;
    // Reading field_6E through a reference stops VC6 from reusing old_count's register for it in the
    // loop condition; with no register left to hoist it into, it is reloaded each pass like the original.
    u16& selected_idx = mHorizontalSelectedIdx;
    do
    {
        new_count++;
        if (new_count > mHorizontalMaxIdx)
        {
            if (bLoopEnabled)
            {
                new_count = 0;
            }
            else
            {
                new_count += 0xFFFF;
            }
        }

        if (mHorizontalIdxEnabled[new_count])
        {
            bFound = true;
        }

    } while (new_count != selected_idx && !bFound);

    mHorizontalSelectedIdx = new_count;

    return old_count != new_count ? true : false;
}

MATCH_FUNC(0x4B6390)
bool MenuItem::SelectPrevHorizontalIdx()
{
    u16 oldCount = mHorizontalSelectedIdx;
    u16 new_count = oldCount;
    bool bFound = false;
    do
    {

        if (new_count == 0)
        {
            if (bIsLeftRightLoopEnabled)
            {
                new_count = mHorizontalMaxIdx;
            }
        }

        else
        {
            --new_count;
        }

        if (mHorizontalIdxEnabled[new_count])
        {
            bFound = true;
        }

        // Last-resort workaround, probably not the original source: a redundant self-store. VC6 deletes it, but
        // only after it has stopped field_6E's load in the loop condition from being CSE'd with oldCount or
        // hoisted, so it is reloaded each pass like the original.
        mHorizontalSelectedIdx = mHorizontalSelectedIdx;
    } while (new_count != mHorizontalSelectedIdx && !bFound);

    mHorizontalSelectedIdx = new_count;

    return oldCount != new_count ? true : false;
}

MATCH_FUNC(0x4B6260)
MenuOption::MenuOption()
{
    mUnused0 = 0;
    mUnused2 = 0;
    mIsOptionUnlocked = 1;
}

MATCH_FUNC(0x4B6280)
MenuOption::~MenuOption()
{
    mUnused0 = 0;
    mUnused2 = 0;
    mIsOptionUnlocked = 1;
}

MATCH_FUNC(0x4B6440)
SaveSlotSummary::SaveSlotSummary()
{
    mSaveExists = false;
    mLastSavedStage = 0;
    mLastSavedBonusStageCode = 0;
    mLastSavedIsBonus = 0;
}

MATCH_FUNC(0x4B6450)
SaveSlotSummary::~SaveSlotSummary()
{
    mSaveExists = false;
    mLastSavedStage = 0;
    mLastSavedBonusStageCode = 0;
    mLastSavedIsBonus = 0;
}

MATCH_FUNC(0x4B6480)
void SaveSlotSummary::LoadPlySlotSvg(const char_type* FileName)
{
    File::Global_Open(FileName);

    SaveSlotInfo svg;
    u32 len = sizeof(SaveSlotInfo);
    File::Global_Read(&svg, len);

    File::Global_Close();

    mSaveExists = true;
    mLastSavedStage = svg.mLastSavedStage;
    mLastSavedBonusStageCode = svg.mLastSavedBonusStageCode;
    mLastSavedIsBonus = svg.mLastSavedIsBonus;
}
