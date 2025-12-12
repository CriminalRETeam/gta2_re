#include "Frontend.hpp"
#include "ang16.hpp"
#include "Bink.hpp"
#include "BurgerKing_67F8B0.hpp"
#include "Draw.hpp"
#include "Function.hpp"
#include "Globals.hpp"
#include "cSampleManager.hpp"
#include "crt_stubs.hpp"
#include "debug.hpp"
#include "dma_video.hpp"
#include "error.hpp"
#include "file.hpp"
#include "fix16.hpp"
#include "Fix16_Point.hpp"
#include "gbh_graphics.hpp"
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
#include "enums.hpp"

#pragma comment(lib, "dxguid.lib")

void Start_GTA2Manager_5E4DE0();

DEFINE_GLOBAL(Frontend*, gFrontend_67DC84, 0x67DC84);
DEFINE_GLOBAL_INIT(u32, counter_706C4C, 0, 0x706C4C);
DEFINE_GLOBAL_INIT(s32, dword_67D930, 0, 0x67D930);
u16 gTableSize_61FF20 = 25; // Note is constant but can't be marked const
DEFINE_GLOBAL_ARRAY(wchar_t, word_67DC8C, 32, 0x67DC8C); // 67DCCC
DEFINE_GLOBAL_INIT(Fix16, dword_67D9FC, Fix16(1), 0x67D9FC);
DEFINE_GLOBAL_INIT(short, font_type_703C14, 7, 0x703C14);
DEFINE_GLOBAL(s16, word_703C3C, 0x703C3C);
DEFINE_GLOBAL(s16, word_703D0C, 0x703D0C);
DEFINE_GLOBAL(s16, word_703C16, 0x703C16);
DEFINE_GLOBAL(s16, word_703C8C, 0x703C8C);
DEFINE_GLOBAL(s16, word_703C8A, 0x703C8A);
DEFINE_GLOBAL(s16, word_703BE2, 0x703BE2);
DEFINE_GLOBAL(s16, word_703B88, 0x703B88);
DEFINE_GLOBAL(s16, word_703DAC, 0x703DAC);
DEFINE_GLOBAL(s16, word_703B9C, 0x703B9C);
DEFINE_GLOBAL_INIT(Ang16, word_67DA70, Ang16(0), 0x67DA70);
DEFINE_GLOBAL_INIT(Fix16, dword_67D934, Fix16(1), 0x67D934);
DEFINE_GLOBAL_ARRAY(wchar_t, tmpBuff_67BD9C, 640, 0x67BD9C);
DEFINE_GLOBAL(BYTE, byte_67DA80, 0x67DA80);
DEFINE_GLOBAL_ARRAY(wchar_t, word_67C7D8, 640, 0x67C7D8);
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
LPCSTR __stdcall FreeLoader::sub_4AE010(HKEY hKey, LPCSTR lpValueName, LPCSTR a3)
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
s32 __stdcall FreeLoader::sub_4AE0F0()
{
    HKEY phkResult;
    RegOpenKeyA(HKEY_LOCAL_MACHINE, "Software\\freeloader.com\\GTA2", &phkResult);
    s32 v0 = (s32)FreeLoader::sub_4AE010(phkResult, "CityInstalled", reinterpret_cast<LPCSTR>(-1));
    RegCloseKey(phkResult);
    return v0;
}

MATCH_FUNC(0x4AE1F0)
EXPORT char_type __stdcall FreeLoader::sub_4AE1F0(u8 a1)
{
    if (a1 > FreeLoader::sub_4AE0F0())
    {
        ShowWindow(gHwnd_707F04, 7);
        PostMessageA(gHwnd_707F04, 6u, 0, 0);
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

DIOBJECTDATAFORMAT stru_5E9110[256] = {
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

DIOBJECTDATAFORMAT stru_5EA110[44] = {{&GUID_XAxis, 0u, 2164260611u, 256u},   {&GUID_YAxis, 4u, 2164260611u, 256u},
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

DIOBJECTDATAFORMAT stru_5EA3D0[7] = {{&GUID_XAxis, 0u, 16776963u, 0u},
                                     {&GUID_YAxis, 4u, 16776963u, 0u},
                                     {&GUID_ZAxis, 8u, 2164260611u, 0u},
                                     {NULL, 12u, 16776972u, 0u},
                                     {NULL, 13u, 16776972u, 0u},
                                     {NULL, 14u, 2164260620u, 0u},
                                     {NULL, 15u, 2164260620u, 0u}};

DIDATAFORMAT gKeyboardDataFormat_601A54 = {24u, 16u, DIDF_RELAXIS, 256u, 256u, stru_5E9110};
DIDATAFORMAT gInputDeviceFormat_601A6C = {24u, 16u, DIDF_ABSAXIS, 80u, 44u, stru_5EA110};
DIDATAFORMAT stru_601A84 = {24u, 16u, DIDF_RELAXIS, 16u, 7u, stru_5EA3D0};

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



DEFINE_GLOBAL_ARRAY_INIT(TgaInfo,
                         tgaArray_61F0C8,
                         25,
                         0x61F0C8,
                         {"data\\frontend\\1.tga", 347564, 0},
                         {"data\\frontend\\1_Options.tga", 266924, 0},
                         {"data\\frontend\\1_Play.tga", 266924, 0},
                         {"data\\frontend\\1_Quit.tga", 266924, 0},
                         {"data\\frontend\\2.tga", 347564, 0},
                         {"data\\frontend\\2_Bonus1.tga", 266924, 0},
                         {"data\\frontend\\2_Bonus2.tga", 266924, 0},
                         {"data\\frontend\\2_Bonus3.tga", 266924, 0},
                         {"data\\frontend\\2_League.tga", 266924, 0},
                         {"data\\frontend\\2_Level1.tga", 266924, 0},
                         {"data\\frontend\\2_Level2.tga", 266924, 0},
                         {"data\\frontend\\2_Level3.tga", 266924, 0},
                         {"data\\frontend\\2_Name.tga", 266924, 0},
                         {"data\\frontend\\2_Restart.tga", 266924, 0},
                         {"data\\frontend\\3.tga", 347564, 0},
                         {"data\\frontend\\3_Tables.tga", 614444, 0},
                         {"data\\frontend\\GameComplete.tga", 614444, 0},
                         {"data\\frontend\\LevelComplete.tga", 614444, 0},
                         {"data\\frontend\\MPLose.tga", 614444, 0},
                         {"data\\frontend\\PlayerDead.tga", 614444, 0},
                         {"data\\frontend\\Mask.tga", 104300, 0},
                         {"data\\frontend\\Mask2.tga", 53594, 0},
                         {"data\\frontend\\Credits.tga", 614444, 0},
                         {"data\\frontend\\Mask3.tga", 130427, 0},
                         {"data\\frontend\\DemoInfo.tga", 614939, 0});


STUB_FUNC(0x5D9910)
EXPORT s32 __stdcall SetGamma_5D9910(s32 gamma)
{
    NOT_IMPLEMENTED;
    // todo

    return 0;
}

DEFINE_GLOBAL(infallible_turing, snd1_67D818, 0x67D818);
DEFINE_GLOBAL(infallible_turing, snd2_67D6F8, 0x67D6F8);

MATCH_FUNC(0x4B4C60)
void Frontend::LoadStringsFromStage_4B4C60(u16 mainBlockIdx, u16 bounusBlockIdx, char* pDebugStr, char* pMapName, char* pStyName)
{
    strcpy(pDebugStr, field_C9E8_blocks[mainBlockIdx][bounusBlockIdx].field_0);
    strcpy(pMapName, field_C9E8_blocks[mainBlockIdx][bounusBlockIdx].field_100);
    strcpy(pStyName, field_C9E8_blocks[mainBlockIdx][bounusBlockIdx].field_200);
}

MATCH_FUNC(0x4B4BC0)
void Frontend::sub_4B4BC0(u16 mainBlockIdx, u16 bounusBlockIdx, const char* pDebugStr, const char* pMapName, const char* pStyName)
{
    strcpy(field_C9E8_blocks[mainBlockIdx][bounusBlockIdx].field_0, pDebugStr);
    strcpy(field_C9E8_blocks[mainBlockIdx][bounusBlockIdx].field_100, pMapName);
    strcpy(field_C9E8_blocks[mainBlockIdx][bounusBlockIdx].field_200, pStyName);
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
        snd2_67D6F8.field_0_object_type = 2;
        snd2_67D6F8.field_C_pAny = &snd1_67D818;
        snd2_67D6F8.field_4_bStatus = 0;
        snd2_67D6F8.field_8 = gRoot_sound_66B038.AddSoundObject_40EFB0(&snd2_67D6F8);
        gRoot_sound_66B038.LoadStyle_40EFF0("data\\fstyle.sty");
        gRoot_sound_66B038.Set3DSound_40F160(0);
    }

    Bink::Reset_513210();
}

MATCH_FUNC(0x4AD070)
void __stdcall Frontend::destroy_4AD070()
{
    if (!bSkip_audio_67D6BE && snd2_67D6F8.field_8)
    {
        gRoot_sound_66B038.FreeSoundEntry_40EFD0(snd2_67D6F8.field_8);
        snd2_67D6F8.field_8 = 0;
    }

    if (gFrontend_67DC84)
    {
        GTA2_DELETE_AND_NULL(gFrontend_67DC84);
    }

    Bink::Close1_513340();
    Bink::Close2_513390();
}

STUB_FUNC(0x4B3170)
void Frontend::sub_4B3170(u16 menu_page_idx)
{
    NOT_IMPLEMENTED;
    u16 v3; // bp
    u8 v4; // bl
    u8 v5; // al
    stage_stats* v6; // ecx
    u8 v7; // al
    s32 v8; // edi
    LPDIRECTINPUTA* v9; // eax
    char_type v10; // bl
    u8 v11; // al
    char_type v12; // al
    MenuPage_0xBCA* v13; // ecx
    s16 playerSlotSetting; // ax
    s32 v15; // edi
    //s32 v16; // edx
    s32 i; // eax
    wchar_t* v18; // eax
    s16 v19; // ax
    wchar_t* v20; // eax
    wchar_t* _5B5F90; // eax
    u8 v22; // bl
    char_type* v23; // edi
    u8 v24; // al
    u8 v25; // bl
    s32 v26; // edi
    wchar_t* field_6_wstr_buf; // ebp
    blissful_ganguly_0x20* v28; // eax
    blissful_ganguly_0x20* v29; // eax
    s32 v30; // edi
    s32 v31; // ebp
    bool v32; // cf
    s32 v33; // eax
    s32 v34; // ebp
    s32 v35; // ebx
    s32* v36; // ebp
    s32* v37; // edx
    s32 v38; // eax
    s32 v39; // ecx
    s32 v40; // eax
    wchar_t* v41; // eax
    wchar_t* v42; // eax
    wchar_t* v43; // eax
    char_type* v44; // eax
    const char_type* v45; // eax
    u16 local_field_132_f136_idx; // cx
    MenuPage_0xBCA* v47; // edi
    wchar_t* v48; // [esp-4h] [ebp-11Ch]
    HDIGDRIVER field_0_hDriver; // [esp-4h] [ebp-11Ch]
    HDIGDRIVER v50; // [esp-4h] [ebp-11Ch]
    u8 v51; // [esp+13h] [ebp-105h]
    u8 v52; // [esp+13h] [ebp-105h]
    char_type v53; // [esp+13h] [ebp-105h]
    u8 a2; // [esp+14h] [ebp-104h]
    u8 a2a; // [esp+14h] [ebp-104h]
    u8 a2b; // [esp+14h] [ebp-104h]
    player_stats_0xA4* v57; // [esp+18h] [ebp-100h]
    s32* v58; // [esp+18h] [ebp-100h]
    s32* v59; // [esp+18h] [ebp-100h]
    s32* v60; // [esp+18h] [ebp-100h]
    char_type v61; // [esp+1Fh] [ebp-F9h]
    u8 a3; // [esp+20h] [ebp-F8h]
    u8 a3a; // [esp+20h] [ebp-F8h]
    u8 a3b; // [esp+20h] [ebp-F8h]
    s32 v65; // [esp+24h] [ebp-F4h]
    char_type v66; // [esp+2Ah] [ebp-EEh]
    char_type v67; // [esp+2Bh] [ebp-EDh]
    char_type v68; // [esp+2Ch] [ebp-ECh]
    u8 v69; // [esp+30h] [ebp-E8h]
    wchar_t Destination[50]; // [esp+34h] [ebp-E4h] BYREF
    wchar_t Buffer[64]; // [esp+98h] [ebp-80h] BYREF

    v3 = menu_page_idx;
    v57 = sub_4B43E0();
    field_132_f136_idx = menu_page_idx;
    switch (menu_page_idx)
    {
        case MENUPAGE_PARENTAL_CONTROL:
            field_110_state = 5;
            field_C9CA = 0;
            field_C9CB = 0;
            sub_4B8530();
            field_C9B3 = 1;
            field_C9B4 = 28;
            field_C9B6 = 5;
            goto LABEL_116;

        case MENUPAGE_CREDITS:
            field_1EB34 = 0x668000;
            field_1EB30 = 0;
            field_1EB38 = 0;
            field_C9B3 = 1;
            goto LABEL_116;

        case MENUPAGE_AREA_COMPLETE:
            a2 = gLucid_hamilton_67E8E0.sub_4C5980();
            v51 = gLucid_hamilton_67E8E0.sub_4C59C0();
            if (gLucid_hamilton_67E8E0.field_574 == 50)
            {
                v51 = 3;
            }

            v4 = 1;
            for (a3 = 1; v4 <= v51; a3 = v4)
            {
                if (v4 < field_1EB51_blocks[a2])
                {
                    gJolly_poitras_0x2BC0_6FEAC0->sub_56BBD0(a2, a3);
                }
                ++v4;
            }
            if (a2 == (unsigned __int8)field_1EB50_idx - 1)
            {
                field_136_menu_pages_array[3].field_4_options_array[0].field_1_is_unlocked = 0;
                field_136_menu_pages_array[3].field_B8A[0].field_4_is_option_unlocked = 0;
            }
            else
            {
                gJolly_poitras_0x2BC0_6FEAC0->sub_56BBD0(a2 + 1, 0);
                field_136_menu_pages_array[3].field_4_options_array[0].field_1_is_unlocked = 1;
                field_136_menu_pages_array[3].field_B8A[0].field_4_is_option_unlocked = 1;
            }
            field_136_menu_pages_array[3].field_4_options_array[3].field_1_is_unlocked = 0;
            field_136_menu_pages_array[3].field_B8A[3].field_4_is_option_unlocked = 0;
            v5 = 1;
            v6 = &v57->field_0_plyr_stage_stats[a2][1];
            do
            {
                if (v6->field_0_is_stage_unlocked && v5 < field_1EB51_blocks[a2])
                {
                    field_136_menu_pages_array[3].field_4_options_array[3].field_1_is_unlocked = 1;
                    field_136_menu_pages_array[3].field_B8A[3].field_4_is_option_unlocked = 1;
                }
                ++v5;
                ++v6;
            } while (v5 < 4u);
            a2a = 1;
            goto LABEL_30;

        case MENUPAGE_BONUS_AREA:
            v7 = gLucid_hamilton_67E8E0.sub_4C5990();
            v8 = v7 >> 4;
            swprintf(tmpBuff_67BD9C, L"%d", v57->field_0_plyr_stage_stats[v8][v7 & 0xF].field_8_stage_latest_score);
            wcsncpy(field_136_menu_pages_array[6].field_518_elements_array[2].field_6_element_name_str, tmpBuff_67BD9C, 0x32u);
            if (gLucid_hamilton_67E8E0.sub_4C5AE0() || v8 >= (unsigned __int8)field_1EB50_idx - 1 ||
                !v57->field_0_plyr_stage_stats[v8 + 1][0].field_0_is_stage_unlocked)
            {
                field_136_menu_pages_array[6].field_4_options_array[1].field_1_is_unlocked = 0;
                field_136_menu_pages_array[6].field_B8A[1].field_4_is_option_unlocked = 0;
            }
            else
            {
                field_136_menu_pages_array[6].field_4_options_array[1].field_1_is_unlocked = 1;
                field_136_menu_pages_array[6].field_B8A[1].field_4_is_option_unlocked = 1;
            }
            goto LABEL_116;
        case MENUPAGE_DEAD:
        case MENUPAGE_RESULTS_PLAYER_QUIT:
            a2a = 0;
        LABEL_30:
            v9 = &(&field_0_pDInput)[gLucid_hamilton_67E8E0.GetPlySlotIdx_4C59B0()];
            v66 = *((BYTE*)v9 + 60905);
            v52 = *((BYTE*)v9 + 60906);
            v67 = *((BYTE*)v9 + 60907);
            if (gLucid_hamilton_67E8E0.sub_4C59A0())
            {
                v11 = gLucid_hamilton_67E8E0.sub_4C5990();
                v10 = v11 >> 4;
                v61 = v11 & 0xF;
            }
            else
            {
                v10 = gLucid_hamilton_67E8E0.sub_4C5980();
                v61 = 0;
            }
            if (v67)
            {
                v12 = v52 >> 4;
                v53 = v52 & 0xF;
            }
            else
            {
                v12 = v66;
                v53 = 0;
            }
            v13 = &field_136_menu_pages_array[menu_page_idx];
            if (v10 == v12 && v61 == v53)
            {
                v13->field_4_options_array[a2a].field_1_is_unlocked = 1;
                v13->field_B8A[a2a].field_4_is_option_unlocked = 1;
            }
            else
            {
                v13->field_4_options_array[a2a].field_1_is_unlocked = 0;
                v13->field_B8A[a2a].field_4_is_option_unlocked = 0;
            }
            break;
    }

    switch (menu_page_idx)
    {
        case MENUPAGE_PLAY:
            playerSlotSetting = gRegistry_6FF968.Create_Player_Setting_587810("plyrslot");
            field_136_menu_pages_array[1].field_4_options_array[0].field_6E_horizontal_selected_idx = playerSlotSetting;
            field_136_menu_pages_array[1].field_4_options_array[0].field_70 = playerSlotSetting;
            gLucid_hamilton_67E8E0.sub_4C5920(playerSlotSetting);
            sub_4B42E0();
            break;

        case MENUPAGE_MULTIPLAYER_RESULTS:
            a2b = gLucid_hamilton_67E8E0.sub_4C5BF0();
            v15 = -1;
            if (gLucid_hamilton_67E8E0.sub_4C5BC0() == 3)
            {
                for (u8 v16 = 0; v16 < 6; v16++)
                {
                    if (gYouthful_einstein_6F8450.field_4_time[v16] > v15 && gYouthful_einstein_6F8450.field_20[v16] == 0)
                    {
                        v15 = gYouthful_einstein_6F8450.field_4_time[v16];
                    }
                }
            }
            else
            {
                gLucid_hamilton_67E8E0.sub_4C5C20();
            }

            v65 = (unsigned __int8)gLucid_hamilton_67E8E0.sub_4C5BE0();
            v68 = gLucid_hamilton_67E8E0.sub_4C5BC0();
            switch (v68)
            {
                case FRAG_GAME_1:
                    _5B5F90 = gText_0x14_704DFC->Find_5B5F90("frags_h");
                    wcsncpy(field_136_menu_pages_array[7].field_518_elements_array[13].field_6_element_name_str, _5B5F90, 0x32u);
                    v19 = sub_4B0190(field_136_menu_pages_array[7].field_518_elements_array[13].field_6_element_name_str,
                                     field_136_menu_pages_array[7].field_518_elements_array[13].field_6A_font_type,
                                     320);
                    break;

                case POINTS_GAME_2:
                    v20 = gText_0x14_704DFC->Find_5B5F90("pnts_h");
                    wcsncpy(field_136_menu_pages_array[7].field_518_elements_array[13].field_6_element_name_str, v20, 50u);
                    v19 = sub_4B0190(field_136_menu_pages_array[7].field_518_elements_array[13].field_6_element_name_str,
                                     field_136_menu_pages_array[7].field_518_elements_array[13].field_6A_font_type,
                                     320);
                    break;

                case TAG_GAME_3:
                    v18 = gText_0x14_704DFC->Find_5B5F90("times_h");
                    wcsncpy(field_136_menu_pages_array[7].field_518_elements_array[13].field_6_element_name_str, v18, 0x32u);
                    v19 = sub_4B0190(field_136_menu_pages_array[7].field_518_elements_array[13].field_6_element_name_str,
                                     field_136_menu_pages_array[7].field_518_elements_array[13].field_6A_font_type,
                                     320);
                    break;
                default:
                    FatalError_4A38C0(Gta2Error::InvalidMultiplayerGameType,
                                      "C:\\Splitting\\GTA2\\Source\\frontend2.cpp",
                                      4079); // Multiplayer game type should be frag, tag or score (but isn't)
            }

            field_136_menu_pages_array[7].field_518_elements_array[13].field_2_xpos = v19;
            v22 = 0;
            v23 = &field_136_menu_pages_array[7].field_518_elements_array[7].field_1_is_it_displayed;
            do
            {
                if (v22 >= a2b)
                {
                    *(v23 - 660) = 0;
                    *v23 = 0;
                }
                else
                {
                    *(v23 - 660) = 1;
                    *v23 = gLucid_hamilton_67E8E0.sub_4C5BC0() != 3;
                }
                ++v22;
                v23 += 110;
            } while (v22 < 6u);

            v24 = a2b;
            v25 = 0;
            v69 = 0;

            if (a2b)
            {
                v26 = 0;
                field_6_wstr_buf = field_136_menu_pages_array[7].field_518_elements_array[1].field_6_element_name_str;
                do
                {
                    if (gYouthful_einstein_6F8450.field_20[v26] != 0)
                    {
                        v48 = gText_0x14_704DFC->Find_5B5F90("mult_q"); //  quit
                        v28 = gLucid_hamilton_67E8E0.sub_4C5C60(v25);
                        swprintf(Buffer, L"%s (%s)", v28->field_0_str, v48);
                        wcscpy(Destination, Buffer);
                    }
                    else
                    {
                        v29 = gLucid_hamilton_67E8E0.sub_4C5C60(v25);
                        wcsncpy(Destination, v29->field_0_str, 0x32u);
                    }
                    gText_0x14_704DFC->sub_5B5B80(Destination);
                    wcsncpy(field_6_wstr_buf, Destination, 0x32u);
                    if (v26 != v65)
                    {
                        wcsncpy(field_136_menu_pages_array[7].field_518_elements_array[v69++ + 8].field_6_element_name_str,
                                Destination,
                                0x32u);
                    }
                    ++v25;
                    ++v26;
                    v24 = a2b;
                    field_6_wstr_buf += 55;
                } while (v25 < a2b);
            }

            v30 = -1;

            if (*((BYTE*)&gYouthful_einstein_6F8450.field_20 + v65))
            {
                v3 = menu_page_idx;
                goto LABEL_105;
            }

            switch (v68)
            {
                case FRAG_GAME_1:
                    a3a = 0;
                    if (v24)
                    {
                        v31 = 0;
                        do
                        {
                            if (v31 != v65 //  not this player (i.e. an opponent)
                                && gYouthful_einstein_6F8450.field_20[v31] == 0 //  not quit?
                                && (s16)(&gLucid_hamilton_67E8E0)->sub_4C5D60(a3a) > v30) // v30 = highest frag from opponents
                            {
                                v30 = (s16)(&gLucid_hamilton_67E8E0)->sub_4C5D60(a3a); // update highest frag
                            }
                            ++v31;
                            ++a3a;
                        } while (a3a < a2b);
                    }
                    v33 = (s16)gLucid_hamilton_67E8E0.sub_4C5D60(v65);
                    break;
                case POINTS_GAME_2:
                    a3b = 0;
                    if (v24)
                    {
                        v34 = 0;
                        do
                        {
                            if (v34 != v65 //  not you
                                && gYouthful_einstein_6F8450.field_20[v34] == 0 //  not quit?
                                && (s16)(&gLucid_hamilton_67E8E0)->sub_4C5CB0(a3b) > v30) // v30 = highest score from opponents
                            {
                                v30 = (s16)(&gLucid_hamilton_67E8E0)->sub_4C5CB0(a3b); // update highest opponent score
                            }
                            ++v34;
                            ++a3b;
                        } while (a3b < a2b);
                    }
                    v33 = gLucid_hamilton_67E8E0.sub_4C5CB0(v65);
                    break;
                case TAG_GAME_3:
                    if (v24)
                    {
                        v35 = 0;
                        v38 = a2b;
                        do
                        {
                            if (v35 != v65 //  not you
                                && gYouthful_einstein_6F8450.field_20[v35] == 0) //  not quit?
                            {
                                v39 = gYouthful_einstein_6F8450.field_4_time[v35]; //  get opponent time
                                if (v39 > v30) //  v30 = highest opponent tag time
                                {
                                    v30 = v39; //  update highest opponent tag time
                                }
                            }
                            ++v35;
                            --v38;
                        } while (v38 < a2b);
                    }
                    v33 = gYouthful_einstein_6F8450.field_4_time[v65]; //  get your time
                    break;
                default:
                    v3 = menu_page_idx;
                    goto LABEL_107;
            }

            v3 = menu_page_idx;
            v40 = v33 - v30; //  v33 = your frag/score/time, v30 = highest opponent frag/score/time

            if (v40 > 0)
            {
                v41 = gText_0x14_704DFC->Find_5B5F90("mult_w"); //  win
                wcsncpy(field_136_menu_pages_array[7].field_518_elements_array[0].field_6_element_name_str, v41, 0x32u);
                break;
            }

            if (v40 < 0)
            {
            LABEL_105:
                v42 = gText_0x14_704DFC->Find_5B5F90("mult_l"); //  lose
                wcsncpy(field_136_menu_pages_array[7].field_518_elements_array[0].field_6_element_name_str, v42, 0x32u);
                break;
            }

        LABEL_107:
            v43 = gText_0x14_704DFC->Find_5B5F90("mult_d"); //  draw
            wcsncpy(field_136_menu_pages_array[7].field_518_elements_array[0].field_6_element_name_str, v43, 0x32u);
            break;

        case MENUPAGE_PLAY_INTRO:
            if (bIsFrench_67D53C)
            {
                sub_4B8650();
            }

            if (pre_intro_bik_exists_4B6030())
            {
                field_0_hDriver = gSampManager_6FFF00.field_0_hDriver;
                v44 = gFrontend_67DC84->pre_intro_bik_4B5F20();
                Bink::sub_513560(v44, field_0_hDriver);
            }
            else
            {
                v50 = gSampManager_6FFF00.field_0_hDriver;
                v45 = gFrontend_67DC84->intro_bik_4B5E50();
                Bink::sub_5133E0(v45, v50);
            }
            break;

        case MENUPAGE_START_MENU:
            field_C9E4 = timeGetTime();
            break;
    }

LABEL_116:
    field_132_f136_idx = v3;
    field_136_menu_pages_array[v3].field_BC6_current_option_idx = field_136_menu_pages_array[v3].field_BC8;
    local_field_132_f136_idx = field_132_f136_idx;
    v47 = &field_136_menu_pages_array[local_field_132_f136_idx];
    if (!v47->field_4_options_array[field_136_menu_pages_array[local_field_132_f136_idx].field_BC6_current_option_idx].field_1_is_unlocked)
    {
        v47->sub_4B6200();
        if (!v47->field_4_options_array[v47->field_BC6_current_option_idx].field_1_is_unlocked)
        {
            FatalError_4A38C0(Gta2Error::FreeloaderEpisodeUnknown, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 4269); // the menu contains no valid options
        }
    }

    sub_4B6780();
}

MATCH_FUNC(0x4B3AF0)
void Frontend::sub_4B3AF0(u16 menu_page_idx, u16 option_idx, wchar_t** w_buffer)
{
    MenuPage_0xBCA* pPage = &field_136_menu_pages_array[menu_page_idx];
    menu_option_0x82* pOption = &pPage->field_4_options_array[option_idx];

    if (menu_page_idx == MENUPAGE_PLAY && option_idx == 0) // option 0 = change player/name
    {
        u16 plyr_idx = pOption->field_6E_horizontal_selected_idx;
        wchar_t* p_wName = (wchar_t*)&gJolly_poitras_0x2BC0_6FEAC0->field_26A0_plyr_stats[plyr_idx].field_90_strPlayerName;
        if (field_110_state == User_Typing_New_Player_Name_3)
        {
            // player typing a name
            wcscpy(word_67C7D8, field_C9A0_curr_plyr_name);
        }
        else if (!*p_wName)
        {
            // player 1, 2, 3 etc.
            swprintf(tmpBuff_67BD9C, L"%d", plyr_idx);
            swprintf(word_67C7D8, L"%s %s", pOption->field_6_option_name_str, tmpBuff_67BD9C);
        }
        else
        {
            // get saved player name
            swprintf(word_67C7D8, L"%s", gJolly_poitras_0x2BC0_6FEAC0->field_26A0_plyr_stats[plyr_idx].field_90_strPlayerName);
        }
    }
    else if (menu_page_idx == MENUPAGE_VIEW_HIGH_SCORE && option_idx == 0)
    {
        swprintf(word_67C7D8, L"%s", gText_0x14_704DFC->Find_5B5F90("hi_for"));
    }
    else
    {
        swprintf(tmpBuff_67BD9C, L"%d", pOption->field_6E_horizontal_selected_idx);
        swprintf(word_67C7D8, L"%s %s", pOption->field_6_option_name_str, tmpBuff_67BD9C);
    }
    *w_buffer = (wchar_t*)&word_67C7D8;
}

MATCH_FUNC(0x4B8680)
void Frontend::sub_4B8680()
{
    if (!bSkip_audio_67D6BE)
    {
        snd1_67D818.field_0_object_type = 0;
        snd1_67D818.field_4_bStatus = 0;
        snd2_67D6F8.field_0_object_type = 2;
        snd2_67D6F8.field_C_pAny = &snd1_67D818;
        snd2_67D6F8.field_4_bStatus = 0;
        snd2_67D6F8.field_8 = gRoot_sound_66B038.AddSoundObject_40EFB0(&snd2_67D6F8);
    }
}

MATCH_FUNC(0x4AEDB0)
s32 Frontend::sub_4AEDB0()
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
        if (Bink::sub_513240())
        {
            if (Bink::sub_513790() == 1)
            {
                Bink::sub_5137A0(2);
                local_field_0_hDriver = gSampManager_6FFF00.field_0_hDriver;
                Bink::sub_5133E0(gFrontend_67DC84->intro_bik_4B5E50(), local_field_0_hDriver);
                Bink::Close1_513340();
                Bink::sub_5137A0(2);
                Bink::sub_5137B0(2);
            }
            else
            {
                Bink::Close1_513340();
                Bink::Close2_513390();
                if (bIsFrench_67D53C)
                {
                    sub_4B8680();
                }

                sub_4B3170(0);
            }
        }

        read_menu_input_4AFEB0();

        local_field_8_keys = field_8_keys;
        v7 = 256;
        do
        {
            if ((*local_field_8_keys & 0x80u) != 0)
            {
                Bink::Close1_513340();
                Bink::Close2_513390();
                if (bIsFrench_67D53C)
                {
                    sub_4B8680();
                }
                sub_4B3170(0);
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
                    field_C9E4 = Time;
                }
                ++v9;
                --v10;
            } while (v10);
            if (Time - field_C9E4 > 60000)
            {
                return 4;
            }
        }
        else
        {
            field_C9E4 = Time;
        }

        if (Time >= field_C9DC || (field_C9E0 == 3))
        {
            sub_4AEC00();
            v12 = field_C9E0 + 1;
            field_C9DC += 33;
            field_C9E0 = v12;
        }
        else if (field_C9E0)
        {
            sub_4ADFB0(); // bQuit ??
            result = field_108_winmain_next_state;
            field_C9E0 = 0;
            return result;
        }
        return field_108_winmain_next_state;
    }
}

// https://decomp.me/scratch/ci11a
STUB_FUNC(0x4B5430)
void Frontend::sub_4B5430(score_table_line* pStrings,
                          u16 text_xpos,
                          u16 text_ypos,
                          u16 num_entries,
                          u16 arg_fontType,
                          u16 draw_kind,
                          u8 spacing_type)
{
    NOT_IMPLEMENTED;
    if (num_entries > 0)
    {
        u16 new_xpos;
        score_table_line* pSmallStringIter = pStrings;
        u16 og_y_pos = text_ypos;

        for (u16 i = 0; i < num_entries; i++)
        {
            //pSmallStringIter = pStrings[i];
            u16 text_ypos_to_use = text_ypos;
            if (spacing_type)
            {
                text_ypos_to_use = og_y_pos;
            }
            if (!wcscmp(pSmallStringIter->field_0_player_name, (wchar_t*)&word_67DC8C))
            {
                swprintf(tmpBuff_67BD9C, gText_0x14_704DFC->Find_5B5F90("hi_empt"));
            }
            else
            {
                swprintf(tmpBuff_67BD9C, L"%s", pSmallStringIter->field_0_player_name);
            }
            if (draw_kind == 0xFFFFu)
            {
                DrawText_4B87A0(tmpBuff_67BD9C, text_xpos, text_ypos_to_use, arg_fontType, 1);
            }
            else
            {
                DrawText_5D8A10(tmpBuff_67BD9C, text_xpos, text_ypos_to_use, arg_fontType, 1, DrawKind(8), draw_kind, false, 0);
            }
            if (spacing_type == 0)
            {
                new_xpos = text_xpos + 175;
                text_ypos_to_use = text_ypos + 20;
            }
            else
            {
                new_xpos = spacing_type == 1 ? text_xpos + 600 : text_xpos + 300;
            }
            swprintf(tmpBuff_67BD9C, L"%d", pSmallStringIter->field_14_score);

            if (gText_0x14_704DFC->field_10_lang_code == 106)
            {
                Frontend::sub_4B78B0(tmpBuff_67BD9C, new_xpos, text_ypos_to_use, arg_fontType, draw_kind, 1, 16, true);
            }
            else
            {
                Frontend::sub_4B78B0(tmpBuff_67BD9C, new_xpos, text_ypos_to_use, arg_fontType, draw_kind, 1, 13, true);
            }

            ++pSmallStringIter;
            og_y_pos += 20;
            text_ypos += 40;
        }
    }
}

// todo: add to header
EXTERN_GLOBAL(s32, gGTA2VersionMajor_708280);

EXTERN_GLOBAL(s32, gGTA2VersionMajor_708284);

// sub_457920 in 9.6f
// https://decomp.me/scratch/jchxT
STUB_FUNC(0x4AD140)
void Frontend::sub_4AD140()
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
        if (field_110_state == User_Typing_New_Player_Name_3)
        {
            pMenuPage->field_518_elements_array[8].field_1_is_it_displayed = false;
            pMenuPage->field_518_elements_array[9].field_1_is_it_displayed = false;
            
            // NOTE: field_124_font_type is u16
            // NOTE: sub_4B7E10 is not static

            last_xpos = sub_4B7E10(2, 0x12Cu, 0x1B8u, field_124_font_type, 0xFFFF); // text: ENTER
            last_xpos = sub_4B7E10(11, last_xpos + 300, 0x1B8u, field_124_font_type, 0xFFFF); // text: : ENTER NAME

            last_xpos = sub_4B7E10(3, 0x12Cu, 0x1CCu, field_124_font_type, 0xFFFF); // text: BACKSPACE
            sub_4B7E10(10, last_xpos + 300, 0x1CCu, field_124_font_type, 0xFFFF); // text: : DELETE LETTER
        }
        else
        {
            u16 idx = pMenuPage->field_4_options_array[0].field_6E_horizontal_selected_idx;
            selected_option_idx = idx;
            u16 unk_xpos = Frontend::sub_5D8990(gJolly_poitras_0x2BC0_6FEAC0->field_26A0_plyr_stats[idx].field_90_strPlayerName, field_11C) + 10;
            
            if (unk_xpos == 10)
            {
                unk_xpos = Frontend::sub_5D8990(pMenuPage->field_4_options_array[0].field_6_option_name_str, field_11C) + 40;
            }
            pMenuPage->field_518_elements_array[9].field_2_xpos = unk_xpos + pMenuPage->field_4_options_array[0].field_2_x_pos;
        }
    }

    high_score_table_0xF0* pHighScoreTable;
    
    if (field_132_f136_idx == MENUPAGE_VIEW_HIGH_SCORE)
    {
        if (field_EE0D < 3)    //  line 1b8
        {
            pHighScoreTable = &gJolly_poitras_0x2BC0_6FEAC0->field_1890_stage_scores[field_EE0D][0];    // main district score
            Frontend::sub_4B5430((score_table_line *)&pHighScoreTable->field_0_score_table_line, 300, 250, 5, field_12A, 0xFFFF, 2);
        }
        else if (field_EE0D < 6)
        {
            pHighScoreTable = &gJolly_poitras_0x2BC0_6FEAC0->field_1890_stage_scores[0][field_EE0D-2];
            Frontend::sub_4B5430((score_table_line *)&pHighScoreTable->field_0_score_table_line, 300, 250, 5, field_12A, 0xFFFF, 2);
        }
        else
        {
            if (field_EE0D < 9)
            {
                pHighScoreTable = &gJolly_poitras_0x2BC0_6FEAC0->field_1890_stage_scores[1][field_EE0D-5];
                Frontend::sub_4B5430((score_table_line *)&pHighScoreTable->field_0_score_table_line, 300, 250, 5, field_12A, 0xFFFF, 2);
            }
            else
            {
                pHighScoreTable = &gJolly_poitras_0x2BC0_6FEAC0->field_1890_stage_scores[2][field_EE0D-8];
                Frontend::sub_4B5430((score_table_line *)&pHighScoreTable->field_0_score_table_line, 300, 250, 5, field_12A, 0xFFFF, 2);
            }
        }
        
        if (!byte_67DA80)
        {
            // left triangle
            if (field_EE0D == 0) // first option
            {
                pMenuPage->field_518_elements_array[2].field_1_is_it_displayed = false;
            }
            else
            {
                pMenuPage->field_518_elements_array[2].field_1_is_it_displayed = true;
            }
            // right triangle
            if (field_EE0D == 11) // last option
            {
                pMenuPage->field_518_elements_array[3].field_1_is_it_displayed = false;
            }
            else
            {
                pMenuPage->field_518_elements_array[3].field_1_is_it_displayed = true;
            }
        }
    }
    
    u8 main_level_idx;
    u8 bonus_stage_idx;
    u8 codified_stages;
    
    if (field_132_f136_idx == MENUPAGE_DEAD 
        || field_132_f136_idx == MENUPAGE_AREA_COMPLETE 
        || field_132_f136_idx == MENUPAGE_BONUS_AREA 
        || field_132_f136_idx == MENUPAGE_RESULTS_PLAYER_QUIT)
    {
        if (!gLucid_hamilton_67E8E0.sub_4C59A0())
        {
            main_level_idx = gLucid_hamilton_67E8E0.sub_4C5980();
            bonus_stage_idx = 0;
        }
        else
        {
            codified_stages = gLucid_hamilton_67E8E0.sub_4C5990();
            main_level_idx = codified_stages >> 4;
            bonus_stage_idx = codified_stages & 0xF;
        }
        if (field_132_f136_idx == MENUPAGE_BONUS_AREA)
        {
            s32 unk_offset = (3 * main_level_idx) + bonus_stage_idx + 64;
            swprintf(tmpBuff_67BD9C, L"%s %c", gText_0x14_704DFC->Find_5B5F90("bonslev"), unk_offset);
            wcsncpy(pMenuPage->field_518_elements_array[0].field_6_element_name_str, tmpBuff_67BD9C, 0x32u);
            //Frontend::sub_4B7D60();
        }
        
        Frontend::sub_4B5430((score_table_line*)&gJolly_poitras_0x2BC0_6FEAC0->field_1890_stage_scores[main_level_idx][bonus_stage_idx].field_0_score_table_line, 0xAAu, 155, 3, field_12A, 0xFFFF, 2);
        
        if (field_132_f136_idx == MENUPAGE_DEAD 
            || field_132_f136_idx == MENUPAGE_AREA_COMPLETE 
            || field_132_f136_idx == MENUPAGE_RESULTS_PLAYER_QUIT)
        {
            Frontend::sub_4B57B0(10, 0xE1);
        }
    }
    
    if (field_132_f136_idx == MENUPAGE_MULTIPLAYER_RESULTS)
    {
        Frontend::sub_4B55F0();    // line 41d
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
                Frontend::sub_4B3AF0(field_132_f136_idx, option_idx, &wstr_array);
            }
            else
            {
                wstr_array = (wchar_t*)&pMenuOption->field_6_option_name_str;
            }

            x_pos = pMenuOption->field_2_x_pos;
            y_pos = pMenuOption->field_4_y_pos;
            
            if (option_idx == pMenuPage->field_BC6_current_option_idx)
            {
                DrawText_4B87A0(wstr_array, x_pos, y_pos, field_120, 1);
                
                if (field_132_f136_idx == MENUPAGE_PLAY)
                {
                    pMenuPage->field_518_elements_array[4].field_1_is_it_displayed = false;
                    pMenuPage->field_518_elements_array[5].field_1_is_it_displayed = false;
                    pMenuPage->field_518_elements_array[6].field_1_is_it_displayed = false;
                    pMenuPage->field_518_elements_array[7].field_1_is_it_displayed = false;
                    pMenuPage->field_518_elements_array[8].field_1_is_it_displayed = false;
                    pMenuPage->field_518_elements_array[9].field_1_is_it_displayed = false;
                    if (option_idx == 3)    //  START PLAY IN AREA
                    {
                        chosen_option_idx = 3;
                        pMenuPage->field_518_elements_array[4].field_6_geometric_shape_type = 1;
                        pMenuPage->field_518_elements_array[5].field_6_geometric_shape_type = 2;
                        pMenuPage->field_518_elements_array[4].field_1_is_it_displayed = field_1EB4C != 0;
                        pMenuPage->field_518_elements_array[5].field_1_is_it_displayed = field_1EB4D != 0;
                    }
                    else if (option_idx == 4) // BONUS STAGE
                    {
                        chosen_option_idx = 4;
                        pMenuPage->field_518_elements_array[6].field_6_geometric_shape_type = 1;
                        pMenuPage->field_518_elements_array[7].field_6_geometric_shape_type = 2;
                        pMenuPage->field_518_elements_array[6].field_1_is_it_displayed = field_1EB4E != 0;
                        pMenuPage->field_518_elements_array[7].field_1_is_it_displayed = field_1EB4F != 0;
                    }
                    else if (option_idx == 0)
                    {
                        pMenuPage->field_518_elements_array[8].field_6_geometric_shape_type = 1;
                        pMenuPage->field_518_elements_array[9].field_6_geometric_shape_type = 2;
                        if (field_110_state != 3)
                        {
                            pMenuPage->field_518_elements_array[8].field_1_is_it_displayed = true;
                            pMenuPage->field_518_elements_array[9].field_1_is_it_displayed = true;
                            field_1EB4A = 1;
                            field_1EB4B = 1;
                            if (!byte_67DA80)
                            {
                                if (selected_option_idx == 0)
                                {
                                    pMenuPage->field_518_elements_array[8].field_1_is_it_displayed = false;
                                    field_1EB4A = 0;
                                }
                                else if (selected_option_idx == pMenuPage->field_4_options_array[0].field_7E_horizontal_max_idx)
                                {
                                    pMenuPage->field_518_elements_array[9].field_1_is_it_displayed = false;
                                    field_1EB4B = 0;
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
                if (pMenuOption->field_6A != 0xFFFF)
                {
                    if (pMenuOption->field_6C == 0xFFFF)
                    {
                        DrawText_4B87A0(wstr_array, x_pos, y_pos, pMenuOption->field_6A, 1);
                    }
                    else
                    {
                        DrawText_5D8A10(wstr_array, x_pos, y_pos, pMenuOption->field_6A, (s32)1, DrawKind(8), pMenuOption->field_6C, false, 0);
                    }
                }
                else
                {
                    if (pMenuOption->field_6C == 0xFFFF)
                    {
                        DrawText_4B87A0(wstr_array, x_pos, y_pos, field_11C, 1);
                    }
                    else
                    {
                        DrawText_5D8A10(wstr_array, x_pos, y_pos, field_11C, (s32)1, DrawKind(8), pMenuOption->field_6C, false, 0);
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
            
            if (((field_132_f136_idx == MENUPAGE_RESULTS_PLAYER_QUIT
                || field_132_f136_idx == MENUPAGE_DEAD)
                && option_idx == 0) || v45)
            {
                if (pMenuOption->field_0_option_type == STRING_TEXT_2)
                {
                    Frontend::sub_4B3AF0(field_132_f136_idx, option_idx, &wstr_array);
                }
                else
                {
                    wstr_array = (wchar_t*)&pMenuOption->field_6_option_name_str;
                }
                
                x_pos = pMenuOption->field_2_x_pos;
                y_pos = pMenuOption->field_4_y_pos;
                
                if (pMenuOption->field_6A != 0xFFFF)
                {
                    DrawText_5D8A10(wstr_array, x_pos, y_pos, pMenuOption->field_6A, 1, DrawKind(8), 8, false, 0);
                }
                else
                {
                    DrawText_5D8A10(wstr_array, x_pos, y_pos, field_11C, 1, DrawKind(8), 8, false, 0);
                }
            }
            
        } // end else
    }    //  end FOR

    u8 bonus_level_idx;

    if (chosen_option_idx == 3 || chosen_option_idx == 4)
    {
        if (chosen_option_idx == 3)  //  START PLAY IN AREA
        {
            main_level_idx = gLucid_hamilton_67E8E0.sub_4C5980();
            bonus_level_idx = 0;
        }
        else if (chosen_option_idx == 4)  // BONUS STAGE
        {
            u8 codified_stages = gLucid_hamilton_67E8E0.sub_4C5990();
            main_level_idx = codified_stages >> 4;
            bonus_level_idx = codified_stages & 0xF;
        }
        Frontend::sub_4B5430((score_table_line*)&gJolly_poitras_0x2BC0_6FEAC0->field_1890_stage_scores[main_level_idx][bonus_level_idx].field_0_score_table_line,
                             0x12Cu,
                             v98,
                             1,
                             field_128,
                             0xFFFF,
                             2);  
    }

    for (option_idx = 0; option_idx < pMenuPage->field_2; option_idx++)
    {
        menu_element_0x6E* pMenuElement = &pMenuPage->field_518_elements_array[option_idx];

        if (pMenuElement->field_1_is_it_displayed)
        {
            s32 two;
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
                    
                    DrawFigure_5D7EC0(6, shape_type, x_pos, y_pos, word_67DA70, dword_67D934, DrawKind(2), 0, 0, false, 0);
                    break;
                
                case STRING_TEXT_1:
                    font_type = pMenuElement->field_6A_font_type;
                
                    x_pos = pMenuElement->field_2_xpos;
                    y_pos = pMenuElement->field_4_ypos;
                    
                    if (font_type == 0xFFFF)
                    {
                        font_type = field_11C;
                    }
                    Frontend::sub_4B3CC0(field_132_f136_idx, option_idx, &wstr_array);
                    
                    if (field_132_f136_idx == MENUPAGE_PLAY 
                        && (option_idx == 2 || option_idx == 3)) 
                    {
                        Frontend::sub_4B78B0(wstr_array, x_pos, y_pos, font_type, pMenuElement->field_6C_font_variant, 1, 0x15u, 1);
                    }
                    else if (field_132_f136_idx == MENUPAGE_VIEW_HIGH_SCORE 
                             && option_idx == 1)
                    {
                        Frontend::sub_4B78B0(wstr_array, x_pos, y_pos, font_type, pMenuElement->field_6C_font_variant, 1, 0x15u, 1);
                    }
                    else
                    {
                        if (pMenuElement->field_6C_font_variant == 0xFFFF)
                        {
                            DrawText_4B87A0(wstr_array, x_pos, y_pos, font_type, 1);
                        }
                        else
                        {
                            DrawText_5D8A10(wstr_array, x_pos, y_pos, font_type, 1, DrawKind(8), pMenuElement->field_6C_font_variant, false, 0);
                        }
                    }
                    break;
                default:
                    break;
            }
        }
    }

    if (field_110_state == User_Typing_New_Player_Name_3)    //  enter new player name
    {
        if (field_114)
        {
            x_pos = pMenuPage->field_4_options_array[0].field_2_x_pos + Frontend::sub_5D8990(field_C9A0_curr_plyr_name, field_11C);
            y_pos = pMenuPage->field_4_options_array[0].field_4_y_pos;
            swprintf(tmpBuff_67BD9C, L"_");
            DrawText_4B87A0(tmpBuff_67BD9C, 
                x_pos, 
                y_pos, 
                field_11C, 
                1);
        }
        wcscpy(tmpBuff_67BD9C, gText_0x14_704DFC->Find_5B5F90("entrnam"));
        x_pos = 0x15Eu;
        y_pos = gText_0x14_704DFC->field_10_lang_code != 'j' ? 12 : 16;
        DrawText_4B87A0(tmpBuff_67BD9C, x_pos, y_pos, field_126, 1);
    }
    if (field_110_state == 5 && field_114)    //  change current player name
    {
        x_pos = pMenuPage->field_518_elements_array[4].field_2_xpos + Frontend::sub_5D8990(field_C9B8, field_11C);
        y_pos = pMenuPage->field_518_elements_array[4].field_4_ypos;
        swprintf(tmpBuff_67BD9C, L"_");
        DrawText_4B87A0(tmpBuff_67BD9C, 
            x_pos, 
            y_pos, 
            field_11C, 
            1);
    }
}

// https://decomp.me/scratch/qV1ie switch "goto" issue
STUB_FUNC(0x4B7AE0)
void Frontend::sub_4B7AE0()
{
    NOT_IMPLEMENTED;
    u16 font_type;
    s32 palette;
    s32 draw_kind;

    u16 credit_idx = field_1EB38;
    for (Fix16 y = field_1EB34; y < 480 && credit_idx < 600; credit_idx++, y += field_EE0E_unk.field_2[credit_idx].field_4)
    {
        sleepy_stonebraker_0x6C* sleepy = &field_EE0E_unk.field_2[credit_idx];
        switch (sleepy->field_6)
        {
            case 0:
                font_type = field_11E;
                draw_kind = 2;
                palette = 0;
                break;
            case 1:
                font_type = field_120;
                draw_kind = 2;
                palette = 0;
                break;
            case 2:
                font_type = field_120;
                draw_kind = 8;
                palette = 13;
                break;
            case 3:
                font_type = field_120;
                draw_kind = 8;
                palette = 14;
                break;
            case 4:
                font_type = field_120;
                draw_kind = 8;
                palette = 15;
                break;
            default:
                FatalError_4A38C0(Gta2Error::InvalidCreditTextColor, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 7966);
        }
        wchar_t* pStrBuf = sleepy->field_8_strBuf;
        if (wcscmp(pStrBuf, word_67DC8C))
        {

            if (!wcscmp(pStrBuf, L"BINKLOGO"))
            {
                DrawFigure_5D7EC0(6, 1, (u16)320, y, word_67DA70, dword_67D934, DrawKind(2), 0, 0, 0, 0);
            }
            else if (!wcscmp(pStrBuf, L"MILESLOGO"))
            {
                DrawFigure_5D7EC0(6, 25, (u16)320, y, word_67DA70, dword_67D934, DrawKind(2), 0, 0, 0, 0);
            }
            else
            {
                s32 v7 = Frontend::sub_5D8990(pStrBuf, font_type);
                u32 draw_x = (640 - v7) / 2;
                DrawText_5D8A10(pStrBuf, draw_x, y, font_type, 1, DrawKind(draw_kind), palette, 0, 0);
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
void Frontend::sub_4B8650()
{
    if (!bSkip_audio_67D6BE)
    {
        if (snd2_67D6F8.field_8)
        {
            gRoot_sound_66B038.FreeSoundEntry_40EFD0(snd2_67D6F8.field_8);
            snd2_67D6F8.field_8 = 0;
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

STUB_FUNC(0x4B5F20)
char_type* Frontend::pre_intro_bik_4B5F20()
{
    NOT_IMPLEMENTED;
    // todo
    return "";
}

STUB_FUNC(0x4B5E50)
const char_type* Frontend::intro_bik_4B5E50()
{
    NOT_IMPLEMENTED;
    // todo
    return "meh.dat";
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
void Frontend::sub_4AEC00()
{
    read_menu_input_4AFEB0();
    sub_4B6780();

    snd1_67D818.field_0_object_type = 0;

    switch (field_110_state)
    {
        case User_Typing_New_Player_Name_3:
            sub_4B2F60();
            break;

        case Unknown_5:
            sub_4B8280();
            break;

        case Unknown_1:
            if (field_132_f136_idx == MENUPAGE_CREDITS)
            {
                snd1_67D818.field_4_bStatus = 1;
                sub_4B7A10();
            }
            else
            {
                snd1_67D818.field_4_bStatus = 0;
                UpdatePageFromUserInput_4AE2D0();
            }
            break;

        case Booting_Map_2:
            SetWinMainStateToBootMap_4AE990();
            break;

        case Unknown_4:
            sub_4AE9A0();
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
    if (field_10D && KeyBoard_GetKeyStates_4AFDD0())
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
void Frontend::sub_4B6780()
{
    MenuPage_0xBCA* pBorg = &field_136_menu_pages_array[field_132_f136_idx];
    if (field_110_state != 2)
    {
        if (field_132_f136_idx == MENUPAGE_START_MENU)
        {
            switch (pBorg->field_BC6_current_option_idx)
            {
                case 0:
                    field_EE08 = Play_1;
                    break;

                case 1:
                    field_EE08 = Options_0;
                    break;

                case 2:
                    field_EE08 = Quit_2;
                    break;
            }
        }
        else if (field_132_f136_idx == MENUPAGE_PLAY)
        {
            switch (pBorg->field_BC6_current_option_idx)
            {
                case 0u:
                    field_EE08 = EnterPlayerName_10;
                    break;
                case 1u:
                    field_EE08 = ResumeLoadSave_11;
                    break;
                case 2u:
                    field_EE08 = ViewHiScore_6;
                    break;
                case 3u:
                    field_EE08 = gLucid_hamilton_67E8E0.sub_4C5980() + 7;
                    break;
                case 4u:
                    field_EE08 = ((unsigned __int8)gLucid_hamilton_67E8E0.sub_4C5990() >> 4) + 3;
                    break;
                default:
                    break;
            }
        }
        else if (field_132_f136_idx == MENUPAGE_VIEW_HIGH_SCORE)
        {
            field_EE08 = HiScoresDisplay_12;
        }
        else if (field_132_f136_idx == MENUPAGE_DEAD)
        {
            field_EE08 = GameOver_13;
        }
        else if (field_132_f136_idx == MENUPAGE_AREA_COMPLETE || field_132_f136_idx == MENUPAGE_BONUS_AREA ||
                 field_132_f136_idx == MENUPAGE_MULTIPLAYER_RESULTS || field_132_f136_idx == MENUPAGE_RESULTS_PLAYER_QUIT ||
                 field_132_f136_idx == MENUPAGE_PARENTAL_CONTROL)
        {
            field_EE08 = RedBar_16;
        }
        else if (field_132_f136_idx == MENUPAGE_GAME_COMPLETE || field_132_f136_idx == MENUPAGE_NICE_TRY)
        {
            field_EE08 = Loading_15;
        }
        else if (field_132_f136_idx == MENUPAGE_CREDITS)
        {
            field_EE08 = Credits_17;
        }
        else
        {
            field_EE08 = PlayArea1_7;
        }
    }
}

// https://decomp.me/scratch/3NE2J
STUB_FUNC(0x4B7A10)
void Frontend::sub_4B7A10()
{
    NOT_IMPLEMENTED;
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
        if (!field_C9B3)
        {
            field_108_winmain_next_state = Quit_1;
            return;
        }
    }
    else
    {
        field_C9B3 = 0;
    }

    if (++field_1EB30 > 0)
    {
        field_1EB30 = 0;

        if (field_1EB34 <= 262124)
        {
            while (++field_1EB38 != 600)
            {
                field_1EB34 = Fix16(field_EE0E_unk.field_2[field_1EB38].field_4) + field_1EB34;

                if (field_1EB34 > 262124)
                {
                    field_1EB34 -= dword_67D9FC;
                    return;
                }
            }
            field_108_winmain_next_state = Quit_1;
        }
        else
        {
            field_1EB34 -= dword_67D9FC;
        }
    }
}

STUB_FUNC(0x4AE2D0)
void Frontend::UpdatePageFromUserInput_4AE2D0()
{
    NOT_IMPLEMENTED;
    MenuPage_0xBCA* pBorg; // ebx
    player_stats_0xA4* v3; // ebp
    u16 v4; // ax
    u8 v5; // bl
    char_type v6; // al
    u8 v7; // bl
    u8 v8; // di
    u8 v9; // bl
    u16 field_BC6_nifty_idx; // cx
    menu_option_0x82* v11; // edi
    bool v12; // bl
    s32 v13; // eax
    u16 v14; // cx
    menu_option_0x82* v15; // edi
    bool v16; // bl
    bool v17; // al
    MenuPage_0xBCA* v18; // [esp+10h] [ebp-Ch]
    u8 v19; // [esp+14h] [ebp-8h]
    s32 v20; // [esp+18h] [ebp-4h]
    u8 i; // [esp+18h] [ebp-4h]

    pBorg = &field_136_menu_pages_array[field_132_f136_idx];
    v18 = pBorg;
    v3 = sub_4B43E0();
    if (field_C9D0_return_pressed)
    {
        if (pBorg->field_4_options_array[pBorg->field_BC6_current_option_idx].field_0_option_type == STRING_TEXT_1)
        {
            v4 = pBorg->field_4_options_array[pBorg->field_BC6_current_option_idx].field_80_menu_page_target;
            switch (v4)
            {
                case MENUPAGE_GTA2MANAGER: // 257
                    Start_GTA2Manager_5E4DE0();
                    snd1_67D818.field_0_object_type = 5;
                    break;
                case MENUPAGE_QUIT: // 258
                    field_108_winmain_next_state = Quit_1;
                    snd1_67D818.field_0_object_type = 5;
                    break;
                case MENUPAGE_REPLAY_PREVIOUS_AREA: // 259
                    goto LABEL_21;
                case MENUPAGE_LOADING_SAVE: // 260
                    v20 = gLucid_hamilton_67E8E0.GetPlySlotIdx_4C59B0(); //  LOBYTE(v20) =
                    if (PlySlotSvgExists_4B5370(v20))
                    {
                        sub_4B4EC0();
                    }
                    else
                    {
                    }
                LABEL_21:
                    gLucid_hamilton_67E8E0.DebugStr_4C58D0("");
                    goto LABEL_28;
                case MENUPAGE_PLAY_NEXT_AREA: // 261
                    if (gLucid_hamilton_67E8E0.sub_4C59A0())
                    {
                        v6 = (unsigned __int8)gLucid_hamilton_67E8E0.sub_4C5990() >> 4;
                    }
                    else
                    {
                        v6 = gLucid_hamilton_67E8E0.sub_4C5980();
                    }

                    v7 = v6 + 1;
                    if (!FreeLoader::sub_4AE1F0(v6 + 1))
                    {
                        goto LABEL_10;
                    }

                    if (v7 >= (u32)field_1EB50_idx)
                    {
                        FatalError_4A38C0(Gta2Error::InvalidLevelAdvancement, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 1543);
                    }

                    if (!v3->field_0_plyr_stage_stats[v7][0].field_0_is_stage_unlocked)
                    {
                        FatalError_4A38C0(Gta2Error::LevelNotOpened, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 1548);
                    }
                    LoadMapFilenames_4B4D00(v7, 0);
                    goto LABEL_9;
                case 263u:
                    v19 = gLucid_hamilton_67E8E0.sub_4C5980();
                    v5 = 3;
                    for (i = 3; !v3->field_0_plyr_stage_stats[v19][i].field_0_is_stage_unlocked || v5 >= field_1EB51_blocks[v19]; i = v5)
                    {
                        --v5;
                    }
                    LoadMapFilenames_4B4D00(v19, i);
                    gLucid_hamilton_67E8E0.sub_4C5AD0(0);
                    goto LABEL_9;
                case MENUPAGE_GET_READY_TO_PLAY: // 264
                    v8 = gLucid_hamilton_67E8E0.sub_4C5980();
                    if (!FreeLoader::sub_4AE1F0(v8))
                    {
                        goto LABEL_11;
                    }
                    LoadMapFilenames_4B4D00(v8, 0);
                LABEL_28:
                    field_EE08 = RedBar_16;
                    field_110_state = Booting_Map_2;
                    snd1_67D818.field_0_object_type = 5;
                    break;
                case MENUPAGE_GET_READY_TO_PLAY_BONUS: // 265
                    v9 = gLucid_hamilton_67E8E0.sub_4C5990();
                    if (!FreeLoader::sub_4AE1F0(v9 >> 4))
                    {
                        goto LABEL_10;
                    }
                    LoadMapFilenames_4B4D00(v9 >> 4, v9 & 0xF);
                    gLucid_hamilton_67E8E0.sub_4C5AD0(1);
                LABEL_9:
                    field_EE08 = RedBar_16;
                    field_110_state = Booting_Map_2;
                LABEL_10:
                    pBorg = v18;
                LABEL_11:
                    snd1_67D818.field_0_object_type = 5;
                    break;
                case MENUPAGE_CONTINUE_NEXT_STAGE: // 266
                    sub_4B8020();
                    snd1_67D818.field_0_object_type = 5;
                    break;
                case 268u:
                    goto LABEL_11;
                default:
                    sub_4B3170(v4);
                    snd1_67D818.field_0_object_type = 5;
                    break;
            }
        }
        else if (field_132_f136_idx == MENUPAGE_PLAY && pBorg->field_BC6_current_option_idx == 0) // player
        {
            field_110_state = User_Typing_New_Player_Name_3;
            sub_4B4280();
            field_C9B2_curr_plyr_name_length = wcslen(field_C9A0_curr_plyr_name);
            sub_4B42B0();
            field_C9B3 = 1;
            field_C9B4 = 28;
            field_C9B6 = 5;
            snd1_67D818.field_0_object_type = 5;
        }
    }

    if (field_C9D1_escape_pressed)
    {
        switch (field_132_f136_idx)
        {
            case MENUPAGE_START_MENU:
            case MENUPAGE_PARENTAL_CONTROL:
                sub_4B3170(9u);
                break;
            case MENUPAGE_PLAY:
            case MENUPAGE_DEAD:
            case MENUPAGE_AREA_COMPLETE:
            case MENUPAGE_GAME_COMPLETE:
            case MENUPAGE_BONUS_AREA:
            case MENUPAGE_NICE_TRY:
            case MENUPAGE_RESULTS_PLAYER_QUIT:
                sub_4B3170(0);
                break;
            case MENUPAGE_VIEW_HIGH_SCORE:
                sub_4B3170(1u);
                break;
            default:
                field_108_winmain_next_state = Quit_1;
                break;
        }
        snd1_67D818.field_0_object_type = 6;
    }

    if (field_C9CE_up_pressed && pBorg->sub_4B61B0())
    {
        snd1_67D818.field_0_object_type = 1;
    }

    if (field_C9CF_down_pressed && pBorg->sub_4B6200())
    {
        snd1_67D818.field_0_object_type = 2;
    }

    if (field_C9CC_left_pressed)
    {
        field_BC6_nifty_idx = pBorg->field_BC6_current_option_idx;
        v11 = &pBorg->field_4_options_array[field_BC6_nifty_idx];
        if (v11->field_0_option_type == STRING_TEXT_2)
        {
            v12 = pBorg->field_4_options_array[pBorg->field_BC6_current_option_idx].sub_4B6390();
            if (field_132_f136_idx == MENUPAGE_PLAY && !v18->field_BC6_current_option_idx)
            {
                gLucid_hamilton_67E8E0.sub_4C5920(v11->field_6E_horizontal_selected_idx);
                sub_4B42E0(); // this
                gRegistry_6FF968.Set_Player_Setting_5878C0("plyrslot", v11->field_6E_horizontal_selected_idx);
                if (v12)
                {
                    snd1_67D818.field_0_object_type = 3;
                }
            }

            if (field_132_f136_idx == MENUPAGE_VIEW_HIGH_SCORE && !v18->field_BC6_current_option_idx)
            {
                field_EE0D = v11->field_6E_horizontal_selected_idx;
                if (v12)
                {
                    snd1_67D818.field_0_object_type = 3;
                }
            }

        LABEL_59:
            pBorg = v18;
            goto LABEL_60;
        }

        if (field_132_f136_idx == MENUPAGE_PLAY)
        {
            if (field_BC6_nifty_idx == 3)
            {
                if (sub_4B6FF0())
                {
                    snd1_67D818.field_0_object_type = 3;
                }
            }
            else if (field_BC6_nifty_idx == 4 && sub_4B70B0())
            {
                snd1_67D818.field_0_object_type = 3;
            }
        }
    }
    else
    {
        if (!field_C9CD_right_pressed)
        {
            goto LABEL_60;
        }

        v14 = pBorg->field_BC6_current_option_idx;
        v15 = &pBorg->field_4_options_array[v14];
        if (v15->field_0_option_type == STRING_TEXT_2)
        {
            v16 = pBorg->field_4_options_array[pBorg->field_BC6_current_option_idx].sub_4B6330(); // this
            if (field_132_f136_idx == MENUPAGE_PLAY && !v18->field_BC6_current_option_idx)
            {
                gLucid_hamilton_67E8E0.sub_4C5920(v15->field_6E_horizontal_selected_idx);
                sub_4B42E0();
                gRegistry_6FF968.Set_Player_Setting_5878C0("plyrslot", v15->field_6E_horizontal_selected_idx);
                if (v16)
                {
                    snd1_67D818.field_0_object_type = 4;
                }
            }
            if (field_132_f136_idx == MENUPAGE_VIEW_HIGH_SCORE && !v18->field_BC6_current_option_idx)
            {
                field_EE0D = v15->field_6E_horizontal_selected_idx;
                if (v16)
                {
                    snd1_67D818.field_0_object_type = 4;
                }
            }
            goto LABEL_59;
        }

        if (field_132_f136_idx == MENUPAGE_PLAY)
        {
            if (v14 == 3)
            {
                v17 = sub_4B7200();
            }
            else
            {
                if (v14 != 4)
                {
                    goto LABEL_60;
                }
                v17 = sub_4B72F0();
            }

            if (v17)
            {
                snd1_67D818.field_0_object_type = 4;
            }
        }
    }

LABEL_60:
    if (field_C9D2_delete_pressed && field_132_f136_idx == MENUPAGE_PLAY && !pBorg->field_BC6_current_option_idx)
    {
        field_110_state = 4;
        field_EE0A = 190;
        field_EE0C = 1;
        snd1_67D818.field_0_object_type = 8;
    }

    v13 = field_118 - 1;
    field_118 = v13;
    if (v13 <= 0)
    {
        field_114 = field_114 == 0;
        field_118 = 2;
    }
}

MATCH_FUNC(0x4AE990)
void Frontend::SetWinMainStateToBootMap_4AE990()
{
    field_108_winmain_next_state = Start_Game_3;
}

// It matches, but we need to get rid of goto's
// https://decomp.me/scratch/LYZij
STUB_FUNC(0x4B2F60)
void Frontend::sub_4B2F60()
{
    //NOT_IMPLEMENTED;
    s16 v1;
    s16 v3;
    u8* field_8_keys;
    wchar_t Key_4D5F40;
    u16 v7;
    u8 v8;
    s16 v9;
    s32 v10;
    u16 v11;
    s16 v12;

    v1 = 0;
    v3 = 256;
    field_8_keys = (u8*)&field_8_keys;
    do
    {
        if ((*field_8_keys & 0x80u) != 0 && v1 != 54 && v1 != 42)
        {
            v3 = v1;
        }
        ++v1;
        ++field_8_keys;
    } while ((u16)v1 < 0x100u);

    if (field_C9B4 != v3)
    {
        field_C9B4 = v3;
        field_C9B6 = 5;

        if (v3 == 28)
        {
            field_110_state = 1;
            Frontend::sub_4B42B0();
            Frontend::sub_4B4230();
            field_136_menu_pages_array[1].field_BC6_current_option_idx = 0;
            if (snd1_67D818.field_0_object_type != 9)
            {
                snd1_67D818.field_0_object_type = 5;
            }
        }
        else if (v3 == 14)
        {
            v8 = field_C9B2_curr_plyr_name_length;
            if (v8 > 0)
            {
                field_C9B2_curr_plyr_name_length = v8 - 1;
                Frontend::sub_4B42B0();
                snd1_67D818.field_0_object_type = 8;
            }
        }
        else if (v3 == 1)
        {
            field_110_state = 1;
            Frontend::sub_4B42B0();
            field_136_menu_pages_array[1].field_BC6_current_option_idx = 0;
            snd1_67D818.field_0_object_type = 6;
        }
        else if (v3 == 256)
        {
        LABEL_27:
            field_C9B3 = 0;
            goto LABEL_29;
        }
        else if (v3 == 57)
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
        Key_4D5F40 = gKeybrd_0x204_6F52F4->GetKey_4D5F40(v3);
        v7 = gText_0x14_704DFC->sub_5B58D0(Key_4D5F40);
        v11 = v7;
        v12 = field_11C;
        if ((u16)gGtx_0x106C_703DD4->sub_5AA760((u16*)&v12, (wchar_t*)&v11) >= 3u && v7)
        {
            goto LABEL_13;
        }
    }
    else
    {
        v9 = field_C9B6;
        if (v9 == 0)
        {
            field_C9B4 = 256;
            field_C9B6 = 5;
        }
        else
        {
            field_C9B6 = v9 - 1;
        }
    }
LABEL_26:
    if (v3 == 256)
    {
        goto LABEL_27;
    }
    field_C9B3 = 1;
LABEL_29:
    field_118--;
    if (field_118 <= 0)
    {
        field_114 = field_114 == 0;
        field_118 = 2;
    }
}

MATCH_FUNC(0x4AE9A0)
void Frontend::sub_4AE9A0()
{
    u16 v2;

    if (field_C9D0_return_pressed)
    {
        v2 = field_EE0A;
        if (v2 == 210)
        {
            switch (field_EE0C)
            {
                case 1:
                    Frontend::sub_4B4410();
                    field_110_state = 1;
                    break;
                default:
                    FatalError_4A38C0(Gta2Error::InvalidCase, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 1934);
            }
        }
        if (v2 == 230)
        {
            field_110_state = 1;
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
        switch (field_EE0A)
        {
            case 190:
            case 210:
                field_EE0A = 230;
                break;
            case 230:
                field_EE0A = 210;
                break;
            default:
                FatalError_4A38C0(Gta2Error::InvalidCase, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 1968);
                break;
        }
        snd1_67D818.field_0_object_type = 1;
    }
    if (field_C9CF_down_pressed)
    {
        switch (field_EE0A)
        {
            case 190:
                field_EE0A = 210;
                break;
            case 210:
                field_EE0A = 230;
                snd1_67D818.field_0_object_type = 2;
                break;
            case 230:
                field_EE0A = 210;
                break;
            default:
                FatalError_4A38C0(Gta2Error::InvalidCase, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 1991);
                break;
        }
        snd1_67D818.field_0_object_type = 2;
    }
    field_118--;
    if (field_118 <= 0)
    {
        field_114 = field_114 == 0;
        field_118 = 2;
    }
}

STUB_FUNC(0x4B8280)
void Frontend::sub_4B8280()
{
    NOT_IMPLEMENTED;
    // todo
}

MATCH_FUNC(0x4B4410)
void Frontend::sub_4B4410()
{
    sub_4B43E0()->sub_56B630();
    gJolly_poitras_0x2BC0_6FEAC0->sub_56BA60(field_136_menu_pages_array[1].field_4_options_array[0].field_6E_horizontal_selected_idx);
    sub_4B42E0();
}

MATCH_FUNC(0x4B43E0)
player_stats_0xA4* Frontend::sub_4B43E0()
{
    // note: movsx vs movzx due to signedness
    u16 idx = gLucid_hamilton_67E8E0.GetPlySlotIdx_4C59B0();
    return &gJolly_poitras_0x2BC0_6FEAC0->field_26A0_plyr_stats[idx];
}

MATCH_FUNC(0x4B42E0)
void Frontend::sub_4B42E0()
{
    player_stats_0xA4* pPlayerStats = Frontend::sub_4B43E0();
    u8 PlySlotIdx_4C59B0 = gLucid_hamilton_67E8E0.GetPlySlotIdx_4C59B0();
    MenuPage_0xBCA* pMenuPage = &field_136_menu_pages_array[field_132_f136_idx];

    u8 v4 = Frontend::sub_4B77B0(pPlayerStats);
    u8 v8 = Frontend::sub_4B7800(pPlayerStats);

    if (v4 < field_1EB3A[PlySlotIdx_4C59B0])
    {
        field_1EB3A[PlySlotIdx_4C59B0] = v4;
        gLucid_hamilton_67E8E0.sub_4C58F0(v4);
    }
    else
    {
        gLucid_hamilton_67E8E0.sub_4C58F0(field_1EB3A[PlySlotIdx_4C59B0]);
    }

    if (v8 < field_1EB42[PlySlotIdx_4C59B0] || v8 == 0xFF)
    {
        field_1EB42[PlySlotIdx_4C59B0] = v8;
        gLucid_hamilton_67E8E0.sub_4C5900(v8);
    }
    else
    {
        gLucid_hamilton_67E8E0.sub_4C5900(field_1EB42[PlySlotIdx_4C59B0]);
    }

    Frontend::sub_4B7610();
    Frontend::sub_4B7550();
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
void Frontend::sub_4B4230()
{
    u16 count = field_136_menu_pages_array[1].field_4_options_array[0].field_6E_horizontal_selected_idx;
    wchar_t* pStr = gJolly_poitras_0x2BC0_6FEAC0->field_26A0_plyr_stats[count].field_90_strPlayerName;
    wcsncpy(pStr, field_C9A0_curr_plyr_name, 9u);
    HandleCheatCode_4B3DD0(pStr);
    gJolly_poitras_0x2BC0_6FEAC0->sub_56BA60(count);
}

MATCH_FUNC(0x4B3CC0)
void Frontend::sub_4B3CC0(u16 a2, u16 a3, wchar_t** a4)
{
    menu_element_0x6E* temp = &field_136_menu_pages_array[a2].field_518_elements_array[a3];

    if (a2 == 14 && a3 == 4)
    {
        wcscpy(word_67C7D8, field_C9B8);
    }
    else if ((a2 == 14 && a3 != 4) || a2 != 5 || a3 != 1)
    {
        swprintf(word_67C7D8, L"%s", temp->field_6_element_name_str);
    }
    else if (field_EE0D < 3)
    {
        swprintf(word_67C7D8, L"%d", field_EE0D + 1);
    }
    else
    {
        swprintf(word_67C7D8, L"%c", field_EE0D + 62);
    }
    *a4 = (wchar_t*)&word_67C7D8;
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
        gJolly_poitras_0x2BC0_6FEAC0->sub_56BC40();
        sub_4B42E0();
        snd1_67D818.field_0_object_type = 9;
    }
    else if (cheat_str_hash == 0x49C76)
    { // GINGERRR Unlock levels one and two
        gCheatUnlockLevelsOneAndTwo_67D584 = gCheatUnlockLevelsOneAndTwo_67D584 == 0;
        gJolly_poitras_0x2BC0_6FEAC0->sub_56BBD0(1u, 0);
        sub_4B42E0();
        snd1_67D818.field_0_object_type = 9;
    }
    else if (cheat_str_hash == 0x5073D)
    { // TUMYFROG unlock all levels
        gCheatUnlockAllLevels_67D538 = gCheatUnlockAllLevels_67D538 == 0;
        gJolly_poitras_0x2BC0_6FEAC0->sub_56BC40();
        gJolly_poitras_0x2BC0_6FEAC0->sub_56BBD0(2u, 2u);
        sub_4B42E0();
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
void Frontend::sub_4B4280()
{
    wcsncpy(field_C9A0_curr_plyr_name,
            gJolly_poitras_0x2BC0_6FEAC0
                ->field_26A0_plyr_stats[field_136_menu_pages_array[1].field_4_options_array[0].field_6E_horizontal_selected_idx]
                .field_90_strPlayerName,
            9u);
}

MATCH_FUNC(0x4B8530)
void Frontend::sub_4B8530()
{
    u16 total = field_C9CA;
    for (u16 i = total; i < 9; i++)
    {
        field_C9B8[i] = 0;
    }
}

MATCH_FUNC(0x4B8560)
void Frontend::sub_4B8560()
{
    if (!wcscmp(field_C9B8, L"WFUSDFCF")) // french bonus mission unlocks?
    {
        if (intro_bik_exists_4B5FF0() && gRegistry_6FF968.Get_Screen_Setting_5870D0("do_play_movie", 1) == 1)
        {
            sub_4B3170(8u);
        }
        else
        {
            sub_4B3170(0);
        }
    }
    else
    {
        field_110_state = 5;
        field_C9CA = 0;

        sub_4B8530();

        field_C9B3 = 1;
        field_C9B4 = 28;
        field_C9B6 = 5;

        wcsncpy(field_136_menu_pages_array[14].field_518_elements_array[0].field_6_element_name_str,
                gText_0x14_704DFC->Find_5B5F90("fr_rnt1"),
                0x32u);
        wcsncpy(field_136_menu_pages_array[14].field_518_elements_array[1].field_6_element_name_str,
                gText_0x14_704DFC->Find_5B5F90("fr_rnt2"),
                0x32u);
        wcsncpy(field_136_menu_pages_array[14].field_518_elements_array[2].field_6_element_name_str,
                gText_0x14_704DFC->Find_5B5F90("fr_rnt3"),
                0x32u);

        field_C9CB = 1;
    }
}

MATCH_FUNC(0x4B8020)
void Frontend::sub_4B8020()
{
    player_stats_0xA4* pClarke = sub_4B43E0();
    u8 idx = gLucid_hamilton_67E8E0.sub_4C5980();

    if (sub_4B7FB0())
    {
        sub_4B3170(4);
    }
    else if (idx == field_1EB50_idx - 1)
    {
        sub_4B3170(10);
    }
    else
    {
        // note: reg swap + push swap due to redundant local
        u8 i = 3;
        while (!pClarke->field_0_plyr_stage_stats[idx][i].field_0_is_stage_unlocked || i >= field_1EB51_blocks[idx])
        {
            i--;
        }

        LoadMapFilenames_4B4D00(idx, i);
        gLucid_hamilton_67E8E0.sub_4C5AD0(0);
        field_EE08 = RedBar_16;
        field_110_state = Booting_Map_2;
    }
}

STUB_FUNC(0x4B7E10)
EXPORT int __stdcall Frontend::sub_4B7E10(s32 str_id_idx, u16 text_xpos, u16 text_ypos, s32 fontType, s32 draw_kind)
{
    NOT_IMPLEMENTED;
    return 0;
}

MATCH_FUNC(0x4B7FB0)
char_type Frontend::sub_4B7FB0()
{
    player_stats_0xA4* v2 = sub_4B43E0();
    u16 v3 = 0;
    // note: two separated while's interlaced by a backwards goto may be actually two nested while's
    while (v3 < field_1EB50_idx)
    {
        u16 v4 = 0;
        while (v4 < field_1EB51_blocks[v3])
        {
            if (!v2->field_0_plyr_stage_stats[v3][v4].field_0_is_stage_unlocked)
            {
                return false;
            }
            v4++;
        }
        v3++;
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
        gLucid_hamilton_67E8E0.sub_4C58F0(mainBlockIdx);
        gLucid_hamilton_67E8E0.sub_4C5910(0);
    }
    else
    {
        gLucid_hamilton_67E8E0.sub_4C5900(bonusBlockIdx | (0x10 * mainBlockIdx));
        gLucid_hamilton_67E8E0.sub_4C5910(1);
    }
}

STUB_FUNC(0x4ADF50)
void Frontend::sub_4ADF50()
{
    NOT_IMPLEMENTED;
    switch (field_110_state)
    {
        case 1:
        case 3:
        case 5:
            if (field_132_f136_idx == MENUPAGE_CREDITS)
            {
                sub_4B7AE0();
            }
            else
            {
                sub_4AD140();
            }
            break;

        case 2:
        {
            const u16 x = sub_4B0190(gText_0x14_704DFC->Find_5B5F90("loading"), -1, 320);

            DrawText_4B87A0(gText_0x14_704DFC->Find_5B5F90("loading"), x, 260, field_11C, 1);
            break;
        }

        case 4:
        {
            if (field_EE0C == 1)
            {
                const s32 v12 = gText_0x14_704DFC->field_10_lang_code - 'j';

                // TODO: Fix this part
                u8 v13 = -(v12 != 0);
                v13 = v13 & 0xFC;

                Fix16 ypos;
                ypos.FromU16_4AE970(v13 + 16);

                DrawText_4B87A0(gText_0x14_704DFC->Find_5B5F90("clrchar"), (u16)275, ypos, field_126, 1);
            }

            {

                u16 fontType;
                if (field_EE0A == 190)
                {
                    fontType = field_120;
                }
                else
                {
                    fontType = field_11C;
                }

                DrawText_4B87A0(gText_0x14_704DFC->Find_5B5F90("sure"), 300, 190, fontType, 1);
            }

            {
                u16 fontType;
                if (field_EE0A == 210)
                {
                    fontType = field_120;
                }
                else
                {
                    fontType = field_11C;
                }

                DrawText_4B87A0(gText_0x14_704DFC->Find_5B5F90("yes"), 300, 210, fontType, 1);
            }

            {
                u16 fontType;
                if (field_EE0A == 230)
                {
                    fontType = field_120;
                }
                else
                {
                    fontType = field_11C;
                }

                DrawText_4B87A0(gText_0x14_704DFC->Find_5B5F90("no"), 300, 230, fontType, 1);
            }
            break;
        }

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

MATCH_FUNC(0x4ADFB0)
void Frontend::sub_4ADFB0()
{
    sub_5D7D30();

    pgbh_BeginScene();
    DrawBackground_4B6E10();
    sub_4ADF50();
    pgbh_EndScene();

    FreeSurface_5D7DC0();

    pVid_FlipBuffers(gVidSys_7071D0);

    pVid_ClearScreen(gVidSys_7071D0, 0, 0, 0, 0, 0, gVidSys_7071D0->field_48_rect_right, gVidSys_7071D0->field_4C_rect_bottom);
}

// https://decomp.me/scratch/IOmk7
// TODO: stop the tail merge... somehow
STUB_FUNC(0x4B6E10)
void Frontend::DrawBackground_4B6E10()
{
    NOT_IMPLEMENTED;
    // todo
    BYTE tga_idx; // [esp+50h] [ebp-8h] BYREF
    BYTE not_used; // [esp+54h] [ebp-4h] BYREF

    if (field_EE08 == GameOver_13 || field_EE08 == RedBar_16 || field_EE08 == BlueBar_14 || field_EE08 == Loading_15 ||
        field_EE08 == HiScoresDisplay_12 || field_EE08 == Credits_17)
    {
        sub_4B6B00(field_EE08, &tga_idx, &not_used);
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
        sub_4B6B00(field_EE08, &tga_idx, &not_used);

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
void Frontend::sub_4B6B00(u8 a1, BYTE* pTgaIdx, BYTE* a3)
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
STUB_FUNC(0x4AF2A0)
Frontend::Frontend()
{
    NOT_IMPLEMENTED;
    field_10D = 1;

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

    gSharp_pare_0x15D8_705064->sub_5B9350();

    sub_5D7CB0();

    pgbh_SetAmbient(1.0);

    if (gMagical_germain_0x8EC_6F5168)
    {
        gMagical_germain_0x8EC_6F5168->sub_4D2B40();
    }

    field_110_state = 1;
    field_114 = 0;
    field_118 = 0;
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

    sub_4AF0E0();

    field_C9DC = timeGetTime();
    field_C9E0 = 0;
    field_132_f136_idx = 0;
    field_C9E4 = 0;

    sub_4B0220();

    field_C9B2_curr_plyr_name_length = 0;
    field_C9B3 = 1;
    field_C9B4 = 256;
    field_C9B6 = 5;

    //memset(&field_C9A0_curr_plyr_name, 0, sizeof(field_C9A0_curr_plyr_name));

    *(u32*)field_C9A0_curr_plyr_name = 0;
    *(u32*)&field_C9A0_curr_plyr_name[2] = 0;
    *(u32*)&field_C9A0_curr_plyr_name[4] = 0;
    *(u32*)&field_C9A0_curr_plyr_name[6] = 0;
    field_C9A0_curr_plyr_name[8] = 0;

    memset(&field_C9B8, 0, sizeof(field_C9B8));
    /*
    *(_DWORD *)field_C9B8 = 0;
    *(_DWORD *)&field_C9B8[2] = 0;
    *(_DWORD *)&field_C9B8[4] = 0;
    *(_DWORD *)&field_C9B8[6] = 0;
    *(_WORD *)&field_C9C8 = 0;
    */
    field_C9CA = 0;
    field_C9CB = 0;
    field_1EB50_idx = 0;

    field_1EB51_blocks[0] = 0; //  lobyte of u16?
    field_1EB51_blocks[1] = 0; //  hibyte of u16?
    field_1EB51_blocks[2] = 0;

    sub_4B4440();
    LoadPlySlotSvgs_4B53C0();

    field_EE08 = Play_1;

    Load_tgas_4B66B0();

    field_EE0D = 0;
    field_EE0A = 190;
    field_EE0C = 0;
    field_1EB30 = 0;
    field_1EB34 = dword_67D930;
    field_1EB38 = 0;
    field_1EB4A = 0;
    field_1EB4B = 0;
    field_1EB4C = 0;
    field_1EB4D = 0;
    field_1EB4E = 0;
    field_1EB4F = 0;

    for (u8 i = 0; i < 8; i++)
    {
        field_1EB3A[i] = -1;
        field_1EB42[i] = -1;
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
void Frontend::sub_4AF0E0()
{
    if (gText_0x14_704DFC->field_10_lang_code == 'j')
    {
        this->field_11C = 101;
        this->field_11E = 101;
        this->field_120 = 102;
        this->field_122 = 101;
        this->field_124_font_type = 101;
        this->field_126 = 101;
        this->field_128 = 101;
        this->field_12A = 101;
        this->field_12C = 106;
        this->field_12E = 102;
        this->field_130 = 201;
    }
    else
    {
        this->field_11C = word_703D0C;
        this->field_11E = word_703D0C;
        this->field_120 = word_703C16;
        this->field_122 = word_703C8C;
        this->field_124_font_type = font_type_703C14;
        this->field_126 = word_703C3C;
        this->field_128 = word_703C8A;
        this->field_12A = word_703BE2;
        this->field_12C = word_703B88;
        this->field_12E = word_703DAC;
        this->field_130 = word_703B9C;
    }
}

STUB_FUNC(0x4B0220)
void Frontend::sub_4B0220()
{
    NOT_IMPLEMENTED;
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
    field_136_menu_pages_array[0].field_BC8 = 0;
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
        field_136_menu_pages_array[1].field_4_options_array[0].field_72[v77++] = 1;
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
    field_136_menu_pages_array[1].field_BC8 = 3;
    field_136_menu_pages_array[1].field_2 = 10;
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
    field_136_menu_pages_array[11].field_2 = 1;
    field_136_menu_pages_array[11].field_518_elements_array[0].field_0_element_type = STRING_TEXT_1;
    field_136_menu_pages_array[11].field_518_elements_array[0].field_2_xpos = 35;
    field_136_menu_pages_array[11].field_518_elements_array[0].field_4_ypos = 11;
    wcscpy(field_136_menu_pages_array[11].field_518_elements_array[0].field_6_element_name_str, gText_0x14_704DFC->Find_5B5F90("plr_qut"));
    field_136_menu_pages_array[11].field_518_elements_array[0].field_6A_font_type = field_130;
    field_136_menu_pages_array[11].field_518_elements_array[0].field_6C_font_variant = 5;
    field_136_menu_pages_array[11].field_4_options_array[0].field_0_option_type = STRING_TEXT_1;
    field_136_menu_pages_array[11].field_4_options_array[0].field_4_y_pos = 392;
    wcsncpy(field_136_menu_pages_array[11].field_4_options_array[0].field_6_option_name_str,
            gText_0x14_704DFC->Find_5B5F90("savepos"),
            0x32u);
    field_136_menu_pages_array[11].field_4_options_array[0].field_2_x_pos =
        Frontend::sub_4B0190(field_136_menu_pages_array[11].field_4_options_array[0].field_6_option_name_str,
                             field_136_menu_pages_array[11].field_4_options_array[0].field_6A,
                             320);
    field_136_menu_pages_array[11].field_4_options_array[0].field_80_menu_page_target = 260;
    field_136_menu_pages_array[11].field_4_options_array[1].field_0_option_type = STRING_TEXT_1;
    field_136_menu_pages_array[11].field_4_options_array[1].field_4_y_pos = 412;
    wcsncpy(field_136_menu_pages_array[11].field_4_options_array[1].field_6_option_name_str,
            gText_0x14_704DFC->Find_5B5F90("replay"),
            0x32u);
    field_136_menu_pages_array[11].field_4_options_array[1].field_2_x_pos =
        Frontend::sub_4B0190(field_136_menu_pages_array[11].field_4_options_array[1].field_6_option_name_str,
                             field_136_menu_pages_array[11].field_4_options_array[1].field_6A,
                             320);
    field_136_menu_pages_array[11].field_4_options_array[1].field_80_menu_page_target = 259;
    field_136_menu_pages_array[11].field_4_options_array[2].field_0_option_type = STRING_TEXT_1;
    field_136_menu_pages_array[11].field_4_options_array[2].field_4_y_pos = 432;
    wcsncpy(field_136_menu_pages_array[11].field_4_options_array[2].field_6_option_name_str,
            gText_0x14_704DFC->Find_5B5F90("mainmen"),
            0x32u);
    field_136_menu_pages_array[11].field_4_options_array[2].field_2_x_pos =
        Frontend::sub_4B0190(field_136_menu_pages_array[11].field_4_options_array[2].field_6_option_name_str,
                             field_136_menu_pages_array[11].field_4_options_array[2].field_6A,
                             320);
    field_136_menu_pages_array[11].field_4_options_array[2].field_80_menu_page_target = 0;
    field_136_menu_pages_array[11].field_B8A[0].field_0 = 150;
    field_136_menu_pages_array[11].field_B8A[0].field_2 = 400;
    field_136_menu_pages_array[11].field_B8A[1].field_0 = 150;
    field_136_menu_pages_array[11].field_B8A[1].field_2 = 420;
    field_136_menu_pages_array[11].field_B8A[2].field_0 = 150;
    field_136_menu_pages_array[11].field_B8A[2].field_2 = 440;
    field_136_menu_pages_array[11].field_BC6_current_option_idx = 0;
    field_136_menu_pages_array[11].field_BC8 = 0;
    field_136_menu_pages_array[2].field_0_number_of_options = 3;
    field_136_menu_pages_array[2].field_2 = 1;
    field_136_menu_pages_array[2].field_518_elements_array[0].field_0_element_type = STRING_TEXT_1;
    field_136_menu_pages_array[2].field_518_elements_array[0].field_2_xpos = 35;
    field_136_menu_pages_array[2].field_518_elements_array[0].field_4_ypos = 11;
    wcsncpy(field_136_menu_pages_array[2].field_518_elements_array[0].field_6_element_name_str,
            gText_0x14_704DFC->Find_5B5F90("plr_ded"),
            0x32u);
    field_136_menu_pages_array[2].field_518_elements_array[0].field_6A_font_type = field_130;
    field_136_menu_pages_array[2].field_518_elements_array[0].field_6C_font_variant = 0;
    field_136_menu_pages_array[2].field_4_options_array[0].field_0_option_type = STRING_TEXT_1;
    field_136_menu_pages_array[2].field_4_options_array[0].field_4_y_pos = 392;
    wcsncpy(field_136_menu_pages_array[2].field_4_options_array[0].field_6_option_name_str,
            gText_0x14_704DFC->Find_5B5F90("savepos"),
            0x32u);
    field_136_menu_pages_array[2].field_4_options_array[0].field_2_x_pos =
        Frontend::sub_4B0190(field_136_menu_pages_array[2].field_4_options_array[0].field_6_option_name_str,
                             field_136_menu_pages_array[2].field_4_options_array[0].field_6A,
                             320);
    field_136_menu_pages_array[2].field_4_options_array[0].field_80_menu_page_target = 260;
    field_136_menu_pages_array[2].field_4_options_array[1].field_0_option_type = STRING_TEXT_1;
    field_136_menu_pages_array[2].field_4_options_array[1].field_4_y_pos = 412;
    wcsncpy(field_136_menu_pages_array[2].field_4_options_array[1].field_6_option_name_str,
            gText_0x14_704DFC->Find_5B5F90("replay"),
            0x32u);
    field_136_menu_pages_array[2].field_4_options_array[1].field_2_x_pos =
        Frontend::sub_4B0190(field_136_menu_pages_array[2].field_4_options_array[1].field_6_option_name_str,
                             field_136_menu_pages_array[2].field_4_options_array[1].field_6A,
                             320);
    field_136_menu_pages_array[2].field_4_options_array[1].field_80_menu_page_target = 259;
    field_136_menu_pages_array[2].field_4_options_array[2].field_0_option_type = STRING_TEXT_1;
    field_136_menu_pages_array[2].field_4_options_array[2].field_4_y_pos = 432;
    wcsncpy(field_136_menu_pages_array[2].field_4_options_array[2].field_6_option_name_str,
            gText_0x14_704DFC->Find_5B5F90("mainmen"),
            0x32u);
    field_136_menu_pages_array[2].field_4_options_array[2].field_2_x_pos =
        Frontend::sub_4B0190(field_136_menu_pages_array[2].field_4_options_array[2].field_6_option_name_str,
                             field_136_menu_pages_array[2].field_4_options_array[2].field_6A,
                             320);
    field_136_menu_pages_array[2].field_4_options_array[2].field_80_menu_page_target = 0;
    field_136_menu_pages_array[2].field_B8A[0].field_0 = 150;
    field_136_menu_pages_array[2].field_B8A[0].field_2 = 400;
    field_136_menu_pages_array[2].field_B8A[1].field_0 = 150;
    field_136_menu_pages_array[2].field_B8A[1].field_2 = 420;
    field_136_menu_pages_array[2].field_B8A[2].field_0 = 150;
    field_136_menu_pages_array[2].field_B8A[2].field_2 = 440;
    field_136_menu_pages_array[2].field_BC6_current_option_idx = 0;
    field_136_menu_pages_array[2].field_BC8 = 0;
    field_136_menu_pages_array[3].field_0_number_of_options = 5;
    field_136_menu_pages_array[3].field_2 = 1;
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
        Frontend::sub_4B0190(field_136_menu_pages_array[3].field_4_options_array[0].field_6_option_name_str,
                             field_136_menu_pages_array[3].field_4_options_array[0].field_6A,
                             320);
    field_136_menu_pages_array[3].field_4_options_array[0].field_80_menu_page_target = 261;
    field_136_menu_pages_array[3].field_4_options_array[1].field_0_option_type = STRING_TEXT_1;
    field_136_menu_pages_array[3].field_4_options_array[1].field_4_y_pos = 385;
    wcsncpy(field_136_menu_pages_array[3].field_4_options_array[1].field_6_option_name_str,
            gText_0x14_704DFC->Find_5B5F90("savepos"),
            0x32u);
    field_136_menu_pages_array[3].field_4_options_array[1].field_2_x_pos =
        Frontend::sub_4B0190(field_136_menu_pages_array[3].field_4_options_array[1].field_6_option_name_str,
                             field_136_menu_pages_array[3].field_4_options_array[1].field_6A,
                             320);
    field_136_menu_pages_array[3].field_4_options_array[1].field_80_menu_page_target = 260;
    field_136_menu_pages_array[3].field_4_options_array[2].field_0_option_type = STRING_TEXT_1;
    field_136_menu_pages_array[3].field_4_options_array[2].field_4_y_pos = 405;
    wcsncpy(field_136_menu_pages_array[3].field_4_options_array[2].field_6_option_name_str,
            gText_0x14_704DFC->Find_5B5F90("replay"),
            0x32u);
    field_136_menu_pages_array[3].field_4_options_array[2].field_2_x_pos =
        Frontend::sub_4B0190(field_136_menu_pages_array[3].field_4_options_array[2].field_6_option_name_str,
                             field_136_menu_pages_array[3].field_4_options_array[2].field_6A,
                             320);
    field_136_menu_pages_array[3].field_4_options_array[2].field_80_menu_page_target = 259;
    field_136_menu_pages_array[3].field_4_options_array[3].field_0_option_type = STRING_TEXT_1;
    field_136_menu_pages_array[3].field_4_options_array[3].field_4_y_pos = 425;
    wcsncpy(field_136_menu_pages_array[3].field_4_options_array[3].field_6_option_name_str,
            gText_0x14_704DFC->Find_5B5F90("contnue"),
            0x32u);
    field_136_menu_pages_array[3].field_4_options_array[3].field_2_x_pos =
        Frontend::sub_4B0190(field_136_menu_pages_array[3].field_4_options_array[3].field_6_option_name_str,
                             field_136_menu_pages_array[3].field_4_options_array[3].field_6A,
                             320);
    field_136_menu_pages_array[3].field_4_options_array[3].field_80_menu_page_target = 266;
    field_136_menu_pages_array[3].field_4_options_array[4].field_0_option_type = STRING_TEXT_1;
    field_136_menu_pages_array[3].field_4_options_array[4].field_4_y_pos = 445;
    wcsncpy(field_136_menu_pages_array[3].field_4_options_array[4].field_6_option_name_str,
            gText_0x14_704DFC->Find_5B5F90("mainmen"),
            0x32u);
    field_136_menu_pages_array[3].field_4_options_array[4].field_2_x_pos =
        Frontend::sub_4B0190(field_136_menu_pages_array[3].field_4_options_array[4].field_6_option_name_str,
                             field_136_menu_pages_array[3].field_4_options_array[4].field_6A,
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
    field_136_menu_pages_array[3].field_BC8 = 0;
    field_136_menu_pages_array[4].field_0_number_of_options = 1;
    field_136_menu_pages_array[4].field_2 = 1;
    field_136_menu_pages_array[4].field_518_elements_array[0].field_0_element_type = STRING_TEXT_1;
    field_136_menu_pages_array[4].field_518_elements_array[0].field_4_ypos = 230;
    wcsncpy(field_136_menu_pages_array[4].field_518_elements_array[0].field_6_element_name_str,
            gText_0x14_704DFC->Find_5B5F90("gam_cmp"),
            0x32u);
    v30 = field_130;
    field_136_menu_pages_array[4].field_518_elements_array[0].field_6A_font_type = v30;
    field_136_menu_pages_array[4].field_518_elements_array[0].field_2_xpos =
        Frontend::sub_4B0190(field_136_menu_pages_array[4].field_518_elements_array[0].field_6_element_name_str, v30, 320);
    field_136_menu_pages_array[4].field_518_elements_array[0].field_6C_font_variant = 4;
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
    field_136_menu_pages_array[4].field_BC8 = 0;
    field_136_menu_pages_array[5].field_0_number_of_options = 1;
    field_136_menu_pages_array[5].field_2 = 5;
    field_136_menu_pages_array[5].field_4_options_array[0].field_0_option_type = STRING_TEXT_2;
    field_136_menu_pages_array[5].field_4_options_array[0].field_2_x_pos = 300;
    field_136_menu_pages_array[5].field_4_options_array[0].field_4_y_pos = 155;
    field_136_menu_pages_array[5].field_4_options_array[0].field_6E_horizontal_selected_idx = 0;
    field_136_menu_pages_array[5].field_4_options_array[0].field_70 = 0;
    field_136_menu_pages_array[5].field_4_options_array[0].field_7E_horizontal_max_idx = 11;

    u16 v323 = 0;
    do
    {
        field_136_menu_pages_array[5].field_4_options_array[0].field_72[v323++] = 1;
    } while (v323 <= field_136_menu_pages_array[5].field_4_options_array[0].field_7E_horizontal_max_idx);

    field_136_menu_pages_array[5].field_B8A[0].field_0 = 280;
    field_136_menu_pages_array[5].field_B8A[0].field_2 = 163;
    field_136_menu_pages_array[5].field_BC6_current_option_idx = 0;
    field_136_menu_pages_array[5].field_BC8 = 0;
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
    field_136_menu_pages_array[6].field_2 = 3;
    field_136_menu_pages_array[6].field_518_elements_array[0].field_0_element_type = STRING_TEXT_1;
    field_136_menu_pages_array[6].field_518_elements_array[0].field_2_xpos = 35;
    field_136_menu_pages_array[6].field_518_elements_array[0].field_4_ypos = 11;
    wcsncpy(field_136_menu_pages_array[6].field_518_elements_array[0].field_6_element_name_str,
            gText_0x14_704DFC->Find_5B5F90("bonslev"),
            0x32u);
    field_136_menu_pages_array[6].field_518_elements_array[0].field_6A_font_type = field_130;
    field_136_menu_pages_array[6].field_518_elements_array[0].field_6C_font_variant = 5;
    field_136_menu_pages_array[6].field_518_elements_array[1].field_0_element_type = STRING_TEXT_1;
    field_136_menu_pages_array[6].field_518_elements_array[1].field_2_xpos = 170;
    field_136_menu_pages_array[6].field_518_elements_array[1].field_4_ypos = 250;
    wcsncpy(field_136_menu_pages_array[6].field_518_elements_array[1].field_6_element_name_str,
            gText_0x14_704DFC->Find_5B5F90("score"),
            0x32u);
    field_136_menu_pages_array[6].field_518_elements_array[2].field_0_element_type = STRING_TEXT_1;
    field_136_menu_pages_array[6].field_518_elements_array[2].field_2_xpos = 400;
    field_136_menu_pages_array[6].field_518_elements_array[2].field_4_ypos = 250;
    s16 v38 = field_120;
    field_136_menu_pages_array[6].field_4_options_array[0].field_0_option_type = STRING_TEXT_1;
    field_136_menu_pages_array[6].field_518_elements_array[2].field_6A_font_type = v38;
    field_136_menu_pages_array[6].field_4_options_array[0].field_4_y_pos = 340;
    wcsncpy(field_136_menu_pages_array[6].field_4_options_array[0].field_6_option_name_str,
            gText_0x14_704DFC->Find_5B5F90("repbons"),
            0x32u);
    field_136_menu_pages_array[6].field_4_options_array[0].field_2_x_pos =
        Frontend::sub_4B0190(field_136_menu_pages_array[6].field_4_options_array[0].field_6_option_name_str, v38, 320);
    field_136_menu_pages_array[6].field_4_options_array[0].field_80_menu_page_target = 259;
    field_136_menu_pages_array[6].field_4_options_array[1].field_0_option_type = STRING_TEXT_1;
    field_136_menu_pages_array[6].field_4_options_array[1].field_4_y_pos = 360;
    wcsncpy(field_136_menu_pages_array[6].field_4_options_array[1].field_6_option_name_str,
            gText_0x14_704DFC->Find_5B5F90("nxt_lvl"),
            0x32u);
    field_136_menu_pages_array[6].field_4_options_array[1].field_2_x_pos =
        Frontend::sub_4B0190(field_136_menu_pages_array[6].field_4_options_array[1].field_6_option_name_str,
                             field_136_menu_pages_array[6].field_4_options_array[1].field_6A,
                             320);
    field_136_menu_pages_array[6].field_4_options_array[1].field_80_menu_page_target = 261;
    field_136_menu_pages_array[6].field_4_options_array[2].field_0_option_type = STRING_TEXT_1;
    field_136_menu_pages_array[6].field_4_options_array[2].field_4_y_pos = 380;
    wcsncpy(field_136_menu_pages_array[6].field_4_options_array[2].field_6_option_name_str,
            gText_0x14_704DFC->Find_5B5F90("mainmen"),
            0x32u);
    field_136_menu_pages_array[6].field_4_options_array[2].field_2_x_pos =
        Frontend::sub_4B0190(field_136_menu_pages_array[6].field_4_options_array[2].field_6_option_name_str,
                             field_136_menu_pages_array[6].field_4_options_array[2].field_6A,
                             320);
    field_136_menu_pages_array[6].field_4_options_array[2].field_80_menu_page_target = 0;
    field_136_menu_pages_array[6].field_B8A[0].field_0 = 150;
    field_136_menu_pages_array[6].field_B8A[0].field_2 = 348;
    field_136_menu_pages_array[6].field_B8A[1].field_0 = 150;
    field_136_menu_pages_array[6].field_B8A[1].field_2 = 368;
    field_136_menu_pages_array[6].field_B8A[2].field_0 = 150;
    field_136_menu_pages_array[6].field_B8A[2].field_2 = 388;
    field_136_menu_pages_array[6].field_BC6_current_option_idx = 0;
    field_136_menu_pages_array[6].field_BC8 = 0;
    field_136_menu_pages_array[7].field_0_number_of_options = 1;
    field_136_menu_pages_array[7].field_2 = 14;
    field_136_menu_pages_array[7].field_518_elements_array[0].field_0_element_type = STRING_TEXT_1;
    field_136_menu_pages_array[7].field_518_elements_array[0].field_2_xpos = 35;
    field_136_menu_pages_array[7].field_518_elements_array[0].field_4_ypos = 11;
    field_136_menu_pages_array[7].field_518_elements_array[0].field_6A_font_type = field_130;
    field_136_menu_pages_array[7].field_518_elements_array[0].field_6C_font_variant = 5;
    field_136_menu_pages_array[7].field_518_elements_array[1].field_0_element_type = STRING_TEXT_1;
    field_136_menu_pages_array[7].field_518_elements_array[1].field_2_xpos = 100;
    field_136_menu_pages_array[7].field_518_elements_array[1].field_4_ypos = 170;
    wcsncpy(field_136_menu_pages_array[7].field_518_elements_array[1].field_6_element_name_str, word_67DC8C, 50u);
    field_136_menu_pages_array[7].field_518_elements_array[2].field_0_element_type = STRING_TEXT_1;
    field_136_menu_pages_array[7].field_518_elements_array[2].field_2_xpos = 100;
    field_136_menu_pages_array[7].field_518_elements_array[2].field_4_ypos = 190;
    wcsncpy(field_136_menu_pages_array[7].field_518_elements_array[2].field_6_element_name_str, word_67DC8C, 0x32u);
    field_136_menu_pages_array[7].field_518_elements_array[3].field_0_element_type = STRING_TEXT_1;
    field_136_menu_pages_array[7].field_518_elements_array[3].field_2_xpos = 100;
    field_136_menu_pages_array[7].field_518_elements_array[3].field_4_ypos = 210;
    wcsncpy(field_136_menu_pages_array[7].field_518_elements_array[3].field_6_element_name_str, word_67DC8C, 0x32u);
    field_136_menu_pages_array[7].field_518_elements_array[4].field_0_element_type = STRING_TEXT_1;
    field_136_menu_pages_array[7].field_518_elements_array[4].field_2_xpos = 100;
    field_136_menu_pages_array[7].field_518_elements_array[4].field_4_ypos = 230;
    wcsncpy(field_136_menu_pages_array[7].field_518_elements_array[4].field_6_element_name_str, word_67DC8C, 0x32u);
    field_136_menu_pages_array[7].field_518_elements_array[5].field_0_element_type = STRING_TEXT_1;
    field_136_menu_pages_array[7].field_518_elements_array[5].field_2_xpos = 100;
    field_136_menu_pages_array[7].field_518_elements_array[5].field_4_ypos = 250;
    wcsncpy(field_136_menu_pages_array[7].field_518_elements_array[5].field_6_element_name_str, word_67DC8C, 0x32u);
    field_136_menu_pages_array[7].field_518_elements_array[6].field_0_element_type = STRING_TEXT_1;
    field_136_menu_pages_array[7].field_518_elements_array[6].field_2_xpos = 100;
    field_136_menu_pages_array[7].field_518_elements_array[6].field_4_ypos = 270;
    wcsncpy(field_136_menu_pages_array[7].field_518_elements_array[6].field_6_element_name_str, word_67DC8C, 0x32u);
    field_136_menu_pages_array[7].field_518_elements_array[7].field_0_element_type = STRING_TEXT_1;
    field_136_menu_pages_array[7].field_518_elements_array[7].field_4_ypos = 300;
    wcsncpy(field_136_menu_pages_array[7].field_518_elements_array[7].field_6_element_name_str,
            gText_0x14_704DFC->Find_5B5F90("kills_h"),
            0x32u);
    field_136_menu_pages_array[7].field_518_elements_array[7].field_2_xpos =
        sub_4B0190(field_136_menu_pages_array[7].field_518_elements_array[7].field_6_element_name_str,
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
        sub_4B0190(field_136_menu_pages_array[7].field_4_options_array[0].field_6_option_name_str,
                   field_136_menu_pages_array[7].field_4_options_array[0].field_6A,
                   320);
    field_136_menu_pages_array[7].field_4_options_array[0].field_80_menu_page_target = 258;
    field_136_menu_pages_array[7].field_B8A[0].field_0 = 180;
    field_136_menu_pages_array[7].field_B8A[0].field_2 = 438;
    field_136_menu_pages_array[7].field_BC6_current_option_idx = 0;
    field_136_menu_pages_array[7].field_BC8 = 0;
    field_136_menu_pages_array[8].field_0_number_of_options = 1;
    field_136_menu_pages_array[8].field_2 = 0;
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
    field_136_menu_pages_array[8].field_BC8 = 0;
    field_136_menu_pages_array[10].field_0_number_of_options = 1;
    field_136_menu_pages_array[10].field_2 = 1;
    field_136_menu_pages_array[10].field_518_elements_array[0].field_0_element_type = STRING_TEXT_1;
    field_136_menu_pages_array[10].field_518_elements_array[0].field_4_ypos = 230;
    wcsncpy(field_136_menu_pages_array[10].field_518_elements_array[0].field_6_element_name_str,
            gText_0x14_704DFC->Find_5B5F90("nicetry"),
            0x32u);
    //    v46 = field_130;
    field_136_menu_pages_array[10].field_518_elements_array[0].field_6A_font_type = field_130; // v46;
    field_136_menu_pages_array[10].field_518_elements_array[0].field_2_xpos =
        sub_4B0190(field_136_menu_pages_array[10].field_518_elements_array[0].field_6_element_name_str,
                   field_130, //v46,
                   320);
    field_136_menu_pages_array[10].field_518_elements_array[0].field_6C_font_variant = 4;
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
    field_136_menu_pages_array[10].field_BC8 = 0;
    field_136_menu_pages_array[14].field_0_number_of_options = 1;
    field_136_menu_pages_array[14].field_2 = 5;
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
    wcsncpy(field_136_menu_pages_array[14].field_518_elements_array[2].field_6_element_name_str, word_67DC8C, 0x32u);
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
    field_136_menu_pages_array[14].field_BC8 = 0;
    field_EE0E_unk.sub_483F20();
}

STUB_FUNC(0x4B4440)
void Frontend::sub_4B4440()
{
    NOT_IMPLEMENTED;

    u8* pBlock; // esi
    char mainOrBonus[256]; // [esp+14h] [ebp-718h] BYREF
    char styName[256]; // [esp+114h] [ebp-618h] BYREF
    char debugStr[256]; // [esp+214h] [ebp-518h] BYREF
    char mapName[256]; // [esp+314h] [ebp-418h] BYREF
    char seqFileName[256]; // [esp+414h] [ebp-318h] BYREF
    _finddata_t findInfo; // [esp+514h] [ebp-218h] BYREF
    char description[256]; // [esp+62Ch] [ebp-100h] BYREF

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

    this->field_1EB50_idx = 0;
    *(u16*)this->field_1EB51_blocks = 0;
    this->field_1EB51_blocks[2] = 0;

    u16 block_idx = 0;
    bool mainBlockFound = false;

    FILE* hSeqFile = crt::fopen(seqFileName, "rt");
    if (!hSeqFile)
    {
        FatalError_4A38C0(Gta2Error::SeqFileOpenError, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 4906);
        return;
    }

    GetSeqItem_4B48D0(0, mainOrBonus, hSeqFile);

    while (strcmp(mainOrBonus, "") != 0)
    {
        if (strcmp(mainOrBonus, "MAIN") == 0)
        {
            if (mainBlockFound)
            {
                if (++block_idx > 2u)
                {
                    FatalError_4A38C0(Gta2Error::TooManyMainBlocks, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 4922);
                }
            }
            mainBlockFound = true;
            pBlock = &this->field_1EB51_blocks[block_idx];
            *pBlock = 0;

            GetSeqItem_4B48D0(1, debugStr, hSeqFile);
            GetSeqItem_4B48D0(2, styName, hSeqFile);
            GetSeqItem_4B48D0(3, mapName, hSeqFile);
            GetSeqItem_4B48D0(4, description, hSeqFile);
            sub_4B4BC0(block_idx, *pBlock, debugStr, styName, mapName);
            ++*pBlock;
        }
        else if (strcmp(mainOrBonus, "BONUS") == 0)
        {
            if (!mainBlockFound)
            {
                FatalError_4A38C0(Gta2Error::MainBlockMustPrecedeBonus, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 4940);
            }
            pBlock = &this->field_1EB51_blocks[block_idx];
            if (*pBlock > 3u)
            {
                FatalError_4A38C0(Gta2Error::TooManyBonusBlocks, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 4945);
            }

            GetSeqItem_4B48D0(1, debugStr, hSeqFile);
            GetSeqItem_4B48D0(2, styName, hSeqFile);
            GetSeqItem_4B48D0(3, mapName, hSeqFile);
            GetSeqItem_4B48D0(4, description, hSeqFile);
            sub_4B4BC0(block_idx, *pBlock, debugStr, styName, mapName);
            ++*pBlock;
        }
        else
        {
            FatalError_4A38C0(Gta2Error::InvalidBlockType, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 4959);
        }

        GetSeqItem_4B48D0(0, mainOrBonus, hSeqFile);
    }

    this->field_1EB50_idx = block_idx + 1;
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
            FatalError_4A38C0(Gta2Error::InvalidFirstLineCharacter,
                              "C:\\Splitting\\GTA2\\Source\\frontend2.cpp",
                              5014);
        }

        while (letter != '=')
        {
            output_buf[pos++] = letter;
            letter = File::SkipWhitespace_4A7340(hSeqFile);
            if (!letter)
            {
                FatalError_4A38C0(Gta2Error::LineInterruptedByNewline,
                                  "C:\\Splitting\\GTA2\\Source\\frontend2.cpp",
                                  5026);
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
                        FatalError_4A38C0(Gta2Error::LineDataTooLong,
                                          "C:\\Splitting\\GTA2\\Source\\frontend2.cpp",
                                          5074);
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
            field_EDE8_plySlots[i].field_2 = 4;
            field_EDE8_plySlots[i].field_3 = 0;
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
void Frontend::sub_4B5270()
{
    u8 plySlotIdx = gLucid_hamilton_67E8E0.GetPlySlotIdx_4C59B0();
    u8 codified_stages = field_EDE8_plySlots[plySlotIdx].field_2;

    u8 main_stage;
    u8 bonus_stage;

    if (!field_EDE8_plySlots[plySlotIdx].field_3)
    {
        bonus_stage = 0;
        main_stage = field_EDE8_plySlots[plySlotIdx].field_1_last_saved_stage;
    }
    else
    {
        main_stage = codified_stages >> 4;
        bonus_stage = codified_stages & 0xF;
    }
    swprintf(tmpBuff_67BD9C, L"%d", main_stage);
    DrawText_4B87A0(tmpBuff_67BD9C, (s16)450, (s16)90, field_11C, 1);

    swprintf(tmpBuff_67BD9C, L"%d", bonus_stage);
    DrawText_4B87A0(tmpBuff_67BD9C, (s16)450, (s16)110, field_11C, 1);
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
u8 Frontend::sub_4B77B0(player_stats_0xA4* a2)
{
    u8 result;

    for (result = this->field_1EB50_idx - 1; !a2->field_0_plyr_stage_stats[result][0].field_0_is_stage_unlocked; --result)
    {
        if (result <= 0)
        {
            break;
        }
    }
    return result;
}

STUB_FUNC(0x4B7800)
u8 Frontend::sub_4B7800(player_stats_0xA4* pPlayerStats)
{
    NOT_IMPLEMENTED;
    return 0;
}

EXTERN_GLOBAL(bool, bDoFrontEnd_626B68);

MATCH_FUNC(0x5E53C0)
void __stdcall Frontend::sub_5E53C0(BYTE* a1)
{
    if (bDoFrontEnd_626B68)
    {
        if (gFrontend_67DC84)
        {
            gFrontend_67DC84->field_10D = *a1;
        }
    }
    else
    {
        gBurgerKing_67F8B0.field_75344_bSomething = *a1;
    }
}

MATCH_FUNC(0x5D8990)
s32 __stdcall Frontend::sub_5D8990(wchar_t* pStr, u16 a2)
{
    wchar_t* pStrIter = pStr;
    s32 current = 0;
    s32 spaceSize = gGtx_0x106C_703DD4->space_width_5AA7B0(&a2);
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
                current = 0;
            }
            else if (str_char_type != '#')
            {
                current += gGtx_0x106C_703DD4->sub_5AA760(&a2, pStrIter);
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
void Frontend::sub_4B78B0(wchar_t* pString, u16 text_xpos, u16 text_ypos, u16 arg_C, s32 a2, u16 a6, u16 a7, u8 pStr)
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
        u16 biggestLine = Frontend::sub_5D8990(chr, arg_C);
        u16 v16 = (a7 - biggestLine) / 2;
        if ((u16)a2 == 0xFFFF)
        {
            DrawText_4B87A0(chr, text_xbase + v16, text_ypos, arg_C, a6);
        }
        else
        {
            DrawText_5D8A10(chr, text_xbase + v16, text_ypos, arg_C, a6, DrawKind(8), a2, false, false);
        }
        text_xbase += a7;
    }
}

MATCH_FUNC(0x4B55F0)
void Frontend::sub_4B55F0()
{
    s8 game_mode = gLucid_hamilton_67E8E0.sub_4C5BC0();
    u8 v20 = gLucid_hamilton_67E8E0.sub_4C5BF0();
    u8 v22 = gLucid_hamilton_67E8E0.sub_4C5BE0();

    u8 v18 = 0;

    for (u8 player_idx = 0; player_idx < v20; ++player_idx)
    {
        u16 x_pos;
        u16 y_pos;
        wchar_t Buffer[26];

        if (game_mode == 1) //  frags
        {
            s32 frags = (s16)gLucid_hamilton_67E8E0.sub_4C5D60(player_idx);
            _itow(frags, Buffer, 10);
            x_pos = 550;
            y_pos = 20 * player_idx + 170;
        }
        else if (game_mode == 2) //  points game
        {
            s32 points = gLucid_hamilton_67E8E0.sub_4C5CB0(player_idx);
            _itow(points, Buffer, 10);
            x_pos = 550;
            y_pos = 20 * player_idx + 170;
        }
        else // tag game
        {
            s32 player_time = gYouthful_einstein_6F8450.field_4_time[player_idx];
            swprintf(Buffer, L"%2d:%02d", player_time / 60, player_time % 60);
            x_pos = 500;
            y_pos = 20 * player_idx + 170;
        }

        DrawText_4B87A0(Buffer, x_pos, y_pos, field_11C, 1);

        s32 v11 = gLucid_hamilton_67E8E0.sub_4C5D80(v22, player_idx);
        _itow(v11, Buffer, 10);

        if (game_mode != 3 && player_idx != v22)
        {
            x_pos = 550;
            y_pos = 20 * v18 + 320;
            DrawText_4B87A0(Buffer, x_pos, y_pos, field_11C, 1);
            ++v18;
        }
    }
}

MATCH_FUNC(0x4B57B0)
void Frontend::sub_4B57B0(u16 a3, u16 a5)
{
    u16 font_type = field_12A;
    s32 v4 = gText_0x14_704DFC->field_10_lang_code != 106 ? 14 : 16;
    u8 v39 = gLucid_hamilton_67E8E0.sub_4C5980();

    if (gText_0x14_704DFC->field_10_lang_code == 106)
    {
        a5 += 5;
    }
    wchar_t* _5B5F90 = gText_0x14_704DFC->Find_5B5F90("last");
    swprintf(tmpBuff_67BD9C, _5B5F90);

    s32 x = Frontend::sub_5D8990(tmpBuff_67BD9C, font_type);

    u16 x_pos;
    u16 y_pos;
    DrawText_4B87A0(tmpBuff_67BD9C, (u16)(a3 - x + 494), (u16)(a5 - 15), font_type, 1);

    swprintf(tmpBuff_67BD9C, gText_0x14_704DFC->Find_5B5F90("best"));

    x = Frontend::sub_5D8990(tmpBuff_67BD9C, font_type);

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

    swprintf(tmpBuff_67BD9C, L"%d", gLucid_hamilton_67E8E0.sub_4C59F0(5));
    u16 x_pos_last = a3 + 480;
    Frontend::sub_4B78B0(tmpBuff_67BD9C, x_pos_last, a5, font_type, 10, 1, v4, 1);

    swprintf(tmpBuff_67BD9C, L"%d", *(u32*)&gJolly_poitras_0x2BC0_6FEAC0->field_1800_best_stats[v39].field_0[20]);
    u16 x_pos_best = a3 + 610;
    Frontend::sub_4B78B0(tmpBuff_67BD9C, x_pos_best, a5, font_type, 10, 1, v4, 1);

    //  auto damage cost

    x_pos = a3;
    y_pos = a5 + 20;
    DrawText_4B87A0(gText_0x14_704DFC->Find_5B5F90("car_cst"), x_pos, y_pos, font_type, 1);

    swprintf(tmpBuff_67BD9C, L"$%d", gLucid_hamilton_67E8E0.sub_4C5A80());
    Frontend::sub_4B78B0(tmpBuff_67BD9C, x_pos_last, y_pos, font_type, 10, 1, v4, 1);

    swprintf(tmpBuff_67BD9C, L"$%d", gJolly_poitras_0x2BC0_6FEAC0->field_1878[v39]);
    Frontend::sub_4B78B0(tmpBuff_67BD9C, x_pos_best, y_pos, font_type, 10, 1, v4, 1);

    //  civilians run down

    x_pos = a3;
    y_pos = a5 + 40;
    DrawText_4B87A0(gText_0x14_704DFC->Find_5B5F90("run_ovr"), x_pos, y_pos, font_type, 1);

    swprintf(tmpBuff_67BD9C, L"%d", gLucid_hamilton_67E8E0.sub_4C59F0(6u));
    Frontend::sub_4B78B0(tmpBuff_67BD9C, x_pos_last, y_pos, font_type, 10, 1, v4, 1);

    swprintf(tmpBuff_67BD9C, L"%d", *(u32*)&gJolly_poitras_0x2BC0_6FEAC0->field_1800_best_stats[v39].field_0[24]);
    Frontend::sub_4B78B0(tmpBuff_67BD9C, x_pos_best, y_pos, font_type, 10, 1, v4, 1);

    //  civilians murdered

    x_pos = a3;
    y_pos = a5 + 60;
    DrawText_4B87A0(gText_0x14_704DFC->Find_5B5F90("murder"), x_pos, y_pos, font_type, 1);

    swprintf(tmpBuff_67BD9C, L"%d", gLucid_hamilton_67E8E0.sub_4C59F0(7u));
    Frontend::sub_4B78B0(tmpBuff_67BD9C, x_pos_last, y_pos, font_type, 10, 1, v4, 1);

    swprintf(tmpBuff_67BD9C, L"%d", *(u32*)&gJolly_poitras_0x2BC0_6FEAC0->field_1800_best_stats[v39].field_0[28]);
    Frontend::sub_4B78B0(tmpBuff_67BD9C, x_pos_best, y_pos, font_type, 10, 1, v4, 1);

    //  lawmen killed

    if (!bIsFrench_67D53C)
    {
        x_pos = a3;
        y_pos = a5 + 80;
        DrawText_4B87A0(gText_0x14_704DFC->Find_5B5F90("cop_kl"), x_pos, y_pos, font_type, 1);

        swprintf(tmpBuff_67BD9C, L"%d", gLucid_hamilton_67E8E0.sub_4C59F0(8u));
        Frontend::sub_4B78B0(tmpBuff_67BD9C, x_pos_last, y_pos, font_type, 10, 1, v4, 1);

        swprintf(tmpBuff_67BD9C, L"%d", *(u32*)&gJolly_poitras_0x2BC0_6FEAC0->field_1800_best_stats[v39].field_0[32]);
        Frontend::sub_4B78B0(tmpBuff_67BD9C, x_pos_best, y_pos, font_type, 10, 1, v4, 1);
    }

    //  gang members killed

    x_pos = a3;
    y_pos = a5 + 100;
    DrawText_4B87A0(gText_0x14_704DFC->Find_5B5F90("gng_kl"), x_pos, y_pos, font_type, 1);

    swprintf(tmpBuff_67BD9C, L"%d", gLucid_hamilton_67E8E0.sub_4C59F0(9u));
    Frontend::sub_4B78B0(tmpBuff_67BD9C, x_pos_last, y_pos, font_type, 10, 1, v4, 1);

    swprintf(tmpBuff_67BD9C, L"%d", *(u32*)&gJolly_poitras_0x2BC0_6FEAC0->field_1800_best_stats[v39].field_0[36]);
    Frontend::sub_4B78B0(tmpBuff_67BD9C, x_pos_best, y_pos, font_type, 10, 1, v4, 1);

    //  fugitive factor

    x_pos = a3;
    y_pos = a5 + 120;
    DrawText_4B87A0(gText_0x14_704DFC->Find_5B5F90("evsnrtg"), x_pos, y_pos, font_type, 1);

    swprintf(tmpBuff_67BD9C, L"%d", gLucid_hamilton_67E8E0.sub_4C5AA0());
    Frontend::sub_4B78B0(tmpBuff_67BD9C, x_pos_last, y_pos, font_type, 10, 1, v4, 1);

    swprintf(tmpBuff_67BD9C, L"%d", gJolly_poitras_0x2BC0_6FEAC0->field_1884[v39]);
    Frontend::sub_4B78B0(tmpBuff_67BD9C, x_pos_best, y_pos, font_type, 10, 1, v4, 1);
}

MATCH_FUNC(0x4B0190)
u16 Frontend::sub_4B0190(wchar_t* pText, s16 fontType, s32 width)
{
    u16 v4;
    if (fontType != -1)
    {
        v4 = ((u16)sub_5D8990(pText, fontType)) / 2;
    }
    else
    {
        v4 = ((u16)sub_5D8990(pText, field_11C)) / 2;
    }
    return width - v4;
}

MATCH_FUNC(0x4B7060)
u8 Frontend::sub_4B7060(u8 a2)
{
    player_stats_0xA4* v2 = sub_4B43E0();
    u8 result = a2;
    if (a2 == 0)
    {
        if (byte_67DA80)
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

STUB_FUNC(0x4B7270)
u8 Frontend::sub_4B7270(char_type a2)
{
    NOT_IMPLEMENTED;
    player_stats_0xA4* v2; // esi
    u8 result; // al

    v2 = sub_4B43E0();
    if (a2 == 2)
    {
        if (byte_67DA80)
        {
            return 0;
        }
        else
        {
            return 2;
        }
    }
    else
    {
        result = a2 + 1;
        while (!v2->field_0_plyr_stage_stats[result][0].field_0_is_stage_unlocked)
        {
            if (result == 2)
            {
                result = byte_67DA80 != 0 ? 0 : a2;
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
bool Frontend::sub_4B7490()
{
    u8 v2 = gLucid_hamilton_67E8E0.sub_4C5980();
    bool result = sub_4B7060(v2) != v2;
    return result;
}

MATCH_FUNC(0x4B74C0)
bool Frontend::sub_4B74C0()
{
    char_type v2 = gLucid_hamilton_67E8E0.sub_4C5980();
    bool result = (char_type)sub_4B7270(v2) != v2;
    return result;
}

MATCH_FUNC(0x4B7550)
void Frontend::sub_4B7550()
{
    MenuPage_0xBCA* pBorg = &field_136_menu_pages_array[field_132_f136_idx];
    u8 v3 = gLucid_hamilton_67E8E0.sub_4C5980();
    swprintf(tmpBuff_67BD9C, L"%d", v3 + 1);
    wcsncpy(pBorg->field_518_elements_array[2].field_6_element_name_str, tmpBuff_67BD9C, 0x32u);

    if (sub_4B7490())
    {
        pBorg->field_518_elements_array[4].field_1_is_it_displayed = 1;
        field_1EB4C = 1;
    }
    else
    {
        pBorg->field_518_elements_array[4].field_1_is_it_displayed = 0;
        field_1EB4C = 0;
    }

    if (sub_4B74C0())
    {
        pBorg->field_518_elements_array[5].field_1_is_it_displayed = 1;
        field_1EB4D = 1;
    }
    else
    {
        pBorg->field_518_elements_array[5].field_1_is_it_displayed = 0;
        field_1EB4D = 0;
    }
}

MATCH_FUNC(0x4B6FF0)
bool Frontend::sub_4B6FF0()
{
    u8 v3 = gLucid_hamilton_67E8E0.sub_4C5980();
    u8 v4 = v3;
    v3 = sub_4B7060(v3);
    gLucid_hamilton_67E8E0.sub_4C58F0(v3);
    field_1EB3A[gLucid_hamilton_67E8E0.GetPlySlotIdx_4C59B0()] = v3;
    sub_4B7550();
    bool result = v4 != v3;
    return result;
}

MATCH_FUNC(0x4B42B0)
void Frontend::sub_4B42B0()
{
    u16 name_length = field_C9B2_curr_plyr_name_length;
    for (u16 i = name_length; i < 9; i++)
    {
        field_C9A0_curr_plyr_name[i] = 0;
    }
}

STUB_FUNC(0x4B7120)
char_type Frontend::sub_4B7120(char_type a2)
{
    NOT_IMPLEMENTED;
    // todo
    return 0;
}

MATCH_FUNC(0x4B7610)
void Frontend::sub_4B7610()
{
    MenuPage_0xBCA* pItem = &field_136_menu_pages_array[field_132_f136_idx];
    u8 v3 = gLucid_hamilton_67E8E0.sub_4C5990();
    u8 v4 = v3 >> 4;
    u8 v5 = v3 & 0xF;
    if (v3 == 0xFF)
    {
        pItem->field_4_options_array[4].field_1_is_unlocked = 0;
        pItem->field_B8A[4].field_4_is_option_unlocked = 0;
        pItem->field_518_elements_array[3].field_1_is_it_displayed = 0;
        pItem->field_518_elements_array[1].field_1_is_it_displayed = 0;
        pItem->field_518_elements_array[6].field_1_is_it_displayed = 0;
        pItem->field_518_elements_array[7].field_1_is_it_displayed = 0;
    }
    else
    {
        pItem->field_4_options_array[4].field_1_is_unlocked = 1;
        pItem->field_B8A[4].field_4_is_option_unlocked = 1;
        pItem->field_518_elements_array[3].field_1_is_it_displayed = 1;
        pItem->field_518_elements_array[1].field_1_is_it_displayed = 1;
        pItem->field_518_elements_array[6].field_1_is_it_displayed = 1;
        pItem->field_518_elements_array[7].field_1_is_it_displayed = 1;
        if (sub_4B74F0())
        {
            pItem->field_518_elements_array[6].field_1_is_it_displayed = 1;
            field_1EB4E = 1;
        }
        else
        {
            pItem->field_518_elements_array[6].field_1_is_it_displayed = 0;
            field_1EB4E = 0;
        }

        if (sub_4B7520())
        {
            pItem->field_518_elements_array[7].field_1_is_it_displayed = 1;
            field_1EB4F = 1;
        }
        else
        {
            pItem->field_518_elements_array[7].field_1_is_it_displayed = 0;
            field_1EB4F = 0;
        }

        swprintf(word_67C7D8, L"%c", 3 * v4 + v5 + 64);
        wcsncpy(pItem->field_518_elements_array[3].field_6_element_name_str, word_67C7D8, 0x32u);
    }
}

MATCH_FUNC(0x4B70B0)
bool Frontend::sub_4B70B0()
{
    s8 v3 = gLucid_hamilton_67E8E0.sub_4C5990();
    s8 v4 = v3;
    v3 = sub_4B7120(v3);
    gLucid_hamilton_67E8E0.sub_4C5900(v3);
    field_1EB42[gLucid_hamilton_67E8E0.GetPlySlotIdx_4C59B0()] = v3;
    sub_4B7610();
    bool result = v4 != v3;
    return result;
}

MATCH_FUNC(0x4B74F0)
bool Frontend::sub_4B74F0()
{
    char_type v2 = gLucid_hamilton_67E8E0.sub_4C5990();
    bool result = sub_4B7120(v2) != v2;
    return result;
}

STUB_FUNC(0x4B7360)
char_type Frontend::sub_4B7360(char_type a2)
{
    NOT_IMPLEMENTED;
    // todo
    return 0;
}

MATCH_FUNC(0x4B7520)
bool Frontend::sub_4B7520()
{
    char_type v2 = gLucid_hamilton_67E8E0.sub_4C5990();
    bool result = sub_4B7360(v2) != v2;
    return result;
}

MATCH_FUNC(0x4B72F0)
bool Frontend::sub_4B72F0()
{
    char_type v3 = gLucid_hamilton_67E8E0.sub_4C5990();
    char_type v4 = v3;
    v3 = sub_4B7360(v3);
    gLucid_hamilton_67E8E0.sub_4C5900(v3);
    field_1EB42[gLucid_hamilton_67E8E0.GetPlySlotIdx_4C59B0()] = v3;
    sub_4B7610();
    bool result = v4 != v3;
    return result;
}

MATCH_FUNC(0x4B7200)
bool Frontend::sub_4B7200()
{
    char_type v3 = gLucid_hamilton_67E8E0.sub_4C5980();
    u8 v4 = v3;
    v3 = sub_4B7270(v3);
    gLucid_hamilton_67E8E0.sub_4C58F0(v3);
    field_1EB3A[gLucid_hamilton_67E8E0.GetPlySlotIdx_4C59B0()] = v3;
    sub_4B7550();
    bool result = (char)v4 != v3;
    return result;
}

STUB_FUNC(0x4B4EC0)
void Frontend::sub_4B4EC0()
{
    NOT_IMPLEMENTED;
    // todo
}

MATCH_FUNC(0x4B6070)
MenuPage_0xBCA::MenuPage_0xBCA()
{
    field_0_number_of_options = 0;
    field_2 = 0;
    field_BC6_current_option_idx = 0;
    field_BC8 = 0;
}

MATCH_FUNC(0x4B6110)
MenuPage_0xBCA::~MenuPage_0xBCA()
{
    field_0_number_of_options = 0;
    field_2 = 0;
    field_BC6_current_option_idx = 0;
    field_BC8 = 0;
}

MATCH_FUNC(0x4B61B0)
bool MenuPage_0xBCA::sub_4B61B0()
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
bool MenuPage_0xBCA::sub_4B6200()
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
    wcscpy(field_6_element_name_str, word_67DC8C);
    field_6A_font_type = -1;
    field_6C_font_variant = -1;
}

MATCH_FUNC(0x4B6420)
menu_element_0x6E::~menu_element_0x6E()
{
    field_1_is_it_displayed = 1;
    field_0_element_type = NULL_TYPE_0;
    field_2_xpos = 0;
    field_4_ypos = 0;
    field_6A_font_type = -1;
    field_6C_font_variant = -1;
}

MATCH_FUNC(0x4B6290)
menu_option_0x82::menu_option_0x82()
{
    field_6A = -1;
    field_6C = -1;
    field_0_option_type = NULL_TYPE_0;
    field_1_is_unlocked = 1;
    field_2_x_pos = 0;
    field_4_y_pos = 0;
    field_6E_horizontal_selected_idx = 0;
    field_70 = 0;

    for (s32 i = 0; i < GTA2_COUNTOF(field_72); i++) // or is it a u32 ??
    {
        field_72[i] = 0;
    }

    wcscpy(field_6_option_name_str, word_67DC8C);
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
    field_6A = -1;
    field_6C = -1;
    field_6E_horizontal_selected_idx = 0;
    field_70 = 0;
    field_7E_horizontal_max_idx = 0;
    field_80_menu_page_target = 0;
}

STUB_FUNC(0x4B6330)
bool menu_option_0x82::sub_4B6330()
{
    NOT_IMPLEMENTED;
    BYTE tmp = byte_67DA80;
    u16 old_count = field_6E_horizontal_selected_idx;
    u16 new_count = old_count;
    char_type bFound = 0;
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

        if (field_72[new_count])
        {
            bFound = 1;
        }

    } while (new_count != old_count && !bFound); // note: field_6E_count being reg cached instead of re-read from this

    field_6E_horizontal_selected_idx = new_count;

    return old_count != new_count ? true : false;
}

STUB_FUNC(0x4B6390)
bool menu_option_0x82::sub_4B6390()
{
    NOT_IMPLEMENTED;
    u16 oldCount = field_6E_horizontal_selected_idx;
    u16 new_count = oldCount;
    char_type bFound = 0;
    do
    {

        if (new_count == 0)
        {
            if (byte_67DA80)
            {
                new_count = field_7E_horizontal_max_idx;
            }
        }

        else
        {
            --new_count; // add     eax, 0FFFFh
        }

        if (field_72[new_count])
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
    field_2 = 0;
    field_3 = 0;
}

MATCH_FUNC(0x4B6450)
admiring_euler_4::~admiring_euler_4()
{
    field_0_save_exists = false;
    field_1_last_saved_stage = 0;
    field_2 = 0;
    field_3 = 0;
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
    field_1_last_saved_stage = svg.field_4B;
    field_2 = svg.field_4C;
    field_3 = svg.field_4D;
}
