#include "Frontend.hpp"
#include "Bink.hpp"
#include "Function.hpp"
#include "Globals.hpp"
#include "cSampleManager.hpp"
#include "crt_stubs.hpp"
#include "debug.hpp"
#include "dma_video.hpp"
#include "error.hpp"
#include "file.hpp"
#include "fix16.hpp"
#include "gbh_graphics.hpp"
#include "gtx_0x106C.hpp"
#include "infallible_turing.hpp"
#include "input.hpp"
#include "jolly_poitras_0x2BC0.hpp"
#include "lucid_hamilton.hpp"
#include "magical_germain_0x8EC.hpp"
#include "registry.hpp"
#include "root_sound.hpp"
#include "sharp_pare_0x15D8.hpp"
#include "text_0x14.hpp"
#include "youthful_einstein.hpp"
#include <io.h>
#include <stdio.h>
#include <wchar.h>

#pragma comment(lib, "dxguid.lib")

EXPORT void __stdcall DrawText_4B87A0(const wchar_t* pBuffer, Fix16 xpos_fp, Fix16 ypos_fp, s16 spaceWidth, Fix16 fp4);

void Start_GTA2Manager_5E4DE0();

EXPORT_VAR Frontend* gFrontend_67DC84;
GLOBAL(gFrontend_67DC84, 0x67DC84);

EXPORT_VAR u32 counter_706C4C;
GLOBAL(counter_706C4C, 0x706C4C);

EXPORT_VAR s32 dword_67D930;
GLOBAL(dword_67D930, 0x67D930);

u16 gTableSize_61FF20 = 25; // Note is constant but can't be marked const

EXPORT_VAR extern wchar_t word_67DC8C[50];

EXPORT_VAR DWORD dword_67D9FC;
GLOBAL(dword_67D9FC, 0x67D9FC);

EXPORT_VAR short font_type_703C14;
GLOBAL(font_type_703C14, 0x703C14);

EXPORT_VAR s16 word_703C3C;
GLOBAL(word_703C3C, 0x703C3C);

EXPORT_VAR s16 word_703D0C;
GLOBAL(word_703D0C, 0x703D0C);

EXPORT_VAR s16 word_703C16;
GLOBAL(word_703C16, 0x703C16);

EXPORT_VAR s16 word_703C8C;
GLOBAL(word_703C8C, 0x703C8C);

EXPORT_VAR s16 word_703C8A;
GLOBAL(word_703C8A, 0x703C8A);

EXPORT_VAR s16 word_703BE2;
GLOBAL(word_703BE2, 0x703BE2);

EXPORT_VAR s16 word_703B88;
GLOBAL(word_703B88, 0x703B88);

EXPORT_VAR s16 word_703DAC;
GLOBAL(word_703DAC, 0x703DAC);

EXPORT_VAR s16 word_703B9C;
GLOBAL(word_703B9C, 0x703B9C);

EXPORT_VAR wchar_t tmpBuff_67BD9C[640];
GLOBAL(tmpBuff_67BD9C, 0x67BD9C);

EXPORT_VAR BYTE byte_67DA80;
GLOBAL(byte_67DA80, 0x67DA80);

EXPORT_VAR char_type byte_67DC88[4]; // todo: prob bigger
GLOBAL(byte_67DC88, 0x67DC88);

EXPORT_VAR wchar_t word_67C7D8[640];
GLOBAL(word_67C7D8, 0x67C7D8);

EXPORT_VAR wchar_t* dword_67EE54;
GLOBAL(dword_67EE54, 0x67EE54);

EXPORT_VAR bool gCheatOnlyMuggerPeds_67D5A4;
GLOBAL(gCheatOnlyMuggerPeds_67D5A4, 0x67D5A4);

EXPORT_VAR bool gCheatUnlimitedElectroGun_67D4F7;
GLOBAL(gCheatUnlimitedElectroGun_67D4F7, 0x67D4F7);

EXPORT_VAR bool gCheatAllGangMaxRespect_67D587;
GLOBAL(gCheatAllGangMaxRespect_67D587, 0x67D587);

EXPORT_VAR bool gCheatOnlyElvisPeds_67D4ED;
GLOBAL(gCheatOnlyElvisPeds_67D4ED, 0x67D4ED);

EXPORT_VAR bool gCheatNakedPeds_67D5E8;
GLOBAL(gCheatNakedPeds_67D5E8, 0x67D5E8);

EXPORT_VAR bool gCheatGetBasicWeaponsMaxAmmo_67D545;
GLOBAL(gCheatGetBasicWeaponsMaxAmmo_67D545, 0x67D545);

EXPORT_VAR bool gCheatGet99Lives_67D4F1;
GLOBAL(gCheatGet99Lives_67D4F1, 0x67D4F1);

EXPORT_VAR bool gCheatGetPlayerPoints_67D4C8;
GLOBAL(gCheatGetPlayerPoints_67D4C8, 0x67D4C8);

EXPORT_VAR bool gCheatUnlimitedFlameThrower_67D6CC;
GLOBAL(gCheatUnlimitedFlameThrower_67D6CC, 0x67D6CC);

EXPORT_VAR bool gCheatUnknown_67D4F6;
GLOBAL(gCheatUnknown_67D4F6, 0x67D4F6);

EXPORT_VAR bool gCheatGet10MillionMoney_67D6CE;
GLOBAL(gCheatGet10MillionMoney_67D6CE, 0x67D6CE);

EXPORT_VAR bool gCheat10xMultiplier_67D589;
GLOBAL(gCheat10xMultiplier_67D589, 0x67D589);

EXPORT_VAR bool gCheatUnlockThreeLevels_67D6CB;
GLOBAL(gCheatUnlockThreeLevels_67D6CB, 0x67D6CB);

EXPORT_VAR bool gCheatUnlockLevelsOneAndTwo_67D584;
GLOBAL(gCheatUnlockLevelsOneAndTwo_67D584, 0x67D584);

EXPORT_VAR bool gCheatUnlockAllLevels_67D538;
GLOBAL(gCheatUnlockAllLevels_67D538, 0x67D538);

EXPORT_VAR bool gCheatUnlimitedDoubleDamage_67D57C;
GLOBAL(gCheatUnlimitedDoubleDamage_67D57C, 0x67D57C);

EXPORT_VAR bool gCheatInvisibility_67D539;
GLOBAL(gCheatInvisibility_67D539, 0x67D539);

EXPORT_VAR bool gCheatMiniCars_67D6C8;
GLOBAL(gCheatMiniCars_67D6C8, 0x67D6C8);

int sCheatHashSecret_61F0A8[8] = {829, 761, 23, 641, 43, 809, 677, 191};

class FreeLoader
{
  public:
    EXPORT static char_type sub_4AE1F0(u8 a1);
};

STUB_FUNC(0x4AE1F0)
EXPORT char_type FreeLoader::sub_4AE1F0(u8 a1)
{
    // todo
    return 0;
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

EXPORT_VAR DWORD dword_70675C;
GLOBAL(dword_70675C, 0x70675C);

EXPORT_VAR DWORD dword_70679C;
GLOBAL(dword_70679C, 0x70679C);

EXPORT_VAR extern s32 window_width_706630;
EXPORT_VAR extern s32 window_height_706B50;

struct TgaInfo
{
    char_type field_0_tga_name[128];
    s32 field_80_len;
    s32 field_84_img;
};

#if defined(EXPORT_VARS) || defined(IMPORT_VARS)
EXPORT_VAR TgaInfo tgaArray_61F0C8[25];
GLOBAL(tgaArray_61F0C8, 0x61F0C8);
#else
TgaInfo tgaArray_61F0C8[25] = {{"data\\frontend\\1.tga", 347564, 0},
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
                               {"data\\frontend\\DemoInfo.tga", 614939, 0}};
#endif

STUB_FUNC(0x5D9910)
EXPORT s32 __stdcall SetGamma_5D9910(s32 gamma)
{
    // todo
    return 0;
}

EXPORT_VAR infallible_turing snd1_67D818;
GLOBAL(snd1_67D818, 0x67D818);

EXPORT_VAR infallible_turing snd2_67D6F8;
GLOBAL(snd2_67D6F8, 0x67D6F8);

STUB_FUNC(0x4B4C60)
void Frontend::sub_4B4C60(u16 mainBlockIdx, u16 bounusBlockIdx, const char* pDebugStr, const char* pMapName, const char* pStyName)
{

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
        delete gFrontend_67DC84;
        gFrontend_67DC84 = 0;
    }

    Bink::Close1_513340();
    Bink::Close2_513390();
}

STUB_FUNC(0x4B3170)
void Frontend::sub_4B3170(u16 arg0)
{
    u16 v3; // bp
    u8 v4; // bl
    u8 v5; // al
    gifted_joliot* v6; // ecx
    u8 v7; // al
    s32 v8; // edi
    LPDIRECTINPUTA* v9; // eax
    char_type v10; // bl
    u8 v11; // al
    char_type v12; // al
    loving_borg_0xBCA* v13; // ecx
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
    loving_borg_0xBCA* v47; // edi
    wchar_t* v48; // [esp-4h] [ebp-11Ch]
    HDIGDRIVER field_0_hDriver; // [esp-4h] [ebp-11Ch]
    HDIGDRIVER v50; // [esp-4h] [ebp-11Ch]
    u8 v51; // [esp+13h] [ebp-105h]
    u8 v52; // [esp+13h] [ebp-105h]
    char_type v53; // [esp+13h] [ebp-105h]
    u8 a2; // [esp+14h] [ebp-104h]
    u8 a2a; // [esp+14h] [ebp-104h]
    u8 a2b; // [esp+14h] [ebp-104h]
    dreamy_clarke_0xA4* v57; // [esp+18h] [ebp-100h]
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

    v3 = arg0;
    v57 = sub_4B43E0();
    field_132_f136_idx = arg0;
    switch (arg0)
    {
        case 0xEu:
            field_110_state = 5;
            field_C9CA = 0;
            field_C9CB = 0;
            sub_4B8530();
            field_C9B3 = 1;
            field_C9B4 = 28;
            field_C9B6 = 5;
            goto LABEL_116;

        case 9u:
            field_1EB34 = 0x668000;
            field_1EB30 = 0;
            field_1EB38 = 0;
            field_C9B3 = 1;
            goto LABEL_116;

        case 3u:
            a2 = gLucid_hamilton_67E8E0.sub_4C5980();
            v51 = gLucid_hamilton_67E8E0.sub_4C59C0();
            if (dword_67EE54 == (wchar_t*)'2')
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
                field_136[3].field_4[0].field_1 = 0;
                field_136[3].field_B8A[0].field_4 = 0;
            }
            else
            {
                gJolly_poitras_0x2BC0_6FEAC0->sub_56BBD0(a2 + 1, 0);
                field_136[3].field_4[0].field_1 = 1;
                field_136[3].field_B8A[0].field_4 = 1;
            }
            field_136[3].field_4[3].field_1 = 0;
            field_136[3].field_B8A[3].field_4 = 0;
            v5 = 1;
            v6 = &v57->field_0[a2][1];
            do
            {
                if (v6->field_0 && v5 < field_1EB51_blocks[a2])
                {
                    field_136[3].field_4[3].field_1 = 1;
                    field_136[3].field_B8A[3].field_4 = 1;
                }
                ++v5;
                ++v6;
            } while (v5 < 4u);
            a2a = 1;
            goto LABEL_30;

        case 6u:
            v7 = gLucid_hamilton_67E8E0.sub_4C5990();
            v8 = v7 >> 4;
            swprintf(tmpBuff_67BD9C, L"%d", v57->field_0[v8][v7 & 0xF].field_8);
            wcsncpy(field_136[6].field_518[2].field_6_wstr_buf, tmpBuff_67BD9C, 0x32u);
            if (gLucid_hamilton_67E8E0.sub_4C5AE0() || v8 >= (unsigned __int8)field_1EB50_idx - 1 || !v57->field_0[v8 + 1][0].field_0)
            {
                field_136[6].field_4[1].field_1 = 0;
                field_136[6].field_B8A[1].field_4 = 0;
            }
            else
            {
                field_136[6].field_4[1].field_1 = 1;
                field_136[6].field_B8A[1].field_4 = 1;
            }
            goto LABEL_116;
        case 2u:
        case 0xBu:
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
            v13 = &field_136[arg0];
            if (v10 == v12 && v61 == v53)
            {
                v13->field_4[a2a].field_1 = 1;
                v13->field_B8A[a2a].field_4 = 1;
            }
            else
            {
                v13->field_4[a2a].field_1 = 0;
                v13->field_B8A[a2a].field_4 = 0;
            }
            break;
    }

    switch (arg0)
    {
        case 1u:
            playerSlotSetting = gRegistry_6FF968.Create_Player_Setting_587810("plyrslot");
            field_136[1].field_4[0].field_6E_count = playerSlotSetting;
            field_136[1].field_4[0].field_70 = playerSlotSetting;
            gLucid_hamilton_67E8E0.sub_4C5920(playerSlotSetting);
            sub_4B42E0();
            break;

        case 7u:
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
                case 1:
                    _5B5F90 = gText_0x14_704DFC->Find_5B5F90("frags_h");
                    wcsncpy(field_136[7].field_518[13].field_6_wstr_buf, _5B5F90, 0x32u);
                    v19 = sub_4B0190(field_136[7].field_518[13].field_6_wstr_buf, field_136[7].field_518[13].field_6A, 320);
                    break;

                case 2:
                    v20 = gText_0x14_704DFC->Find_5B5F90("pnts_h");
                    wcsncpy(field_136[7].field_518[13].field_6_wstr_buf, v20, 50u);
                    v19 = sub_4B0190(field_136[7].field_518[13].field_6_wstr_buf, field_136[7].field_518[13].field_6A, 320);
                    break;

                case 3:
                    v18 = gText_0x14_704DFC->Find_5B5F90("times_h");
                    wcsncpy(field_136[7].field_518[13].field_6_wstr_buf, v18, 0x32u);
                    v19 = sub_4B0190(field_136[7].field_518[13].field_6_wstr_buf, field_136[7].field_518[13].field_6A, 320);
                    break;
                default:
                    FatalError_4A38C0(189,
                                      "C:\\Splitting\\GTA2\\Source\\frontend2.cpp",
                                      4079); // Multiplayer game type should be frag, tag or score (but isn't)
            }

            field_136[7].field_518[13].field_2_xpos = v19;
            v22 = 0;
            v23 = &field_136[7].field_518[7].field_1;
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
                field_6_wstr_buf = field_136[7].field_518[1].field_6_wstr_buf;
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
                        wcsncpy(field_136[7].field_518[v69++ + 8].field_6_wstr_buf, Destination, 0x32u);
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
                v3 = arg0;
                goto LABEL_105;
            }

            switch (v68)
            {
                case 1:
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
                case 2:
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
                case 3:
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
                    v3 = arg0;
                    goto LABEL_107;
            }

            v3 = arg0;
            v40 = v33 - v30; //  v33 = your frag/score/time, v30 = highest opponent frag/score/time

            if (v40 > 0)
            {
                v41 = gText_0x14_704DFC->Find_5B5F90("mult_w"); //  win
                wcsncpy(field_136[7].field_518[0].field_6_wstr_buf, v41, 0x32u);
                break;
            }

            if (v40 < 0)
            {
            LABEL_105:
                v42 = gText_0x14_704DFC->Find_5B5F90("mult_l"); //  lose
                wcsncpy(field_136[7].field_518[0].field_6_wstr_buf, v42, 0x32u);
                break;
            }

        LABEL_107:
            v43 = gText_0x14_704DFC->Find_5B5F90("mult_d"); //  draw
            wcsncpy(field_136[7].field_518[0].field_6_wstr_buf, v43, 0x32u);
            break;

        case 8u:
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

        case 0u:
            field_C9E4 = timeGetTime();
            break;
    }

LABEL_116:
    field_132_f136_idx = v3;
    field_136[v3].field_BC6_nifty_idx = field_136[v3].field_BC8;
    local_field_132_f136_idx = field_132_f136_idx;
    v47 = &field_136[local_field_132_f136_idx];
    if (!v47->field_4[field_136[local_field_132_f136_idx].field_BC6_nifty_idx].field_1)
    {
        v47->sub_4B6200();
        if (!v47->field_4[v47->field_BC6_nifty_idx].field_1)
        {
            FatalError_4A38C0(165, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 4269); // the menu contains no valid options
        }
    }

    sub_4B6780();
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
    if (local_field_132_f136_idx == 8)
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

        return field_108;
    }
    else
    {
        if (!local_field_132_f136_idx)
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
            result = field_108;
            field_C9E0 = 0;
            return result;
        }
        return field_108;
    }
}

// todo: add to header
EXPORT_VAR extern s32 gGTA2VersionMajor_708280;
EXPORT_VAR extern s32 gGTA2VersionMajor_708284;

// sub_457920 in 9.6f
STUB_FUNC(0x4AD140)
void Frontend::sub_4AD140()
{
    __int32 v1; // edx
    int v2; // ebx
    __int16 v12; // ax
    int v13; // edx
    int v14; // eax
    __int16 v15; // ax
    int v16; // ecx
    u16 v18;
    small_string* v20; // ecx
    u16 v21; // ax
    u8 v23; // al
    u8 v24; // bl
    u8 v25; // bp
    wchar_t* _5B5F90; // eax
    u16 v27; // ax
    __int32 v30; // eax
    int field_6_wstr_buf; // edx
    u16 v35; // eax
    __int16 v37; // ax
    int v38; // ecx
    int v39; // edx
    __int16 v40; // ax
    u16 v44; // ax
    Fix16* v46; // eax
    small_string* v47; // ecx
    int v48; // ecx
    int v49; // edx
    small_string* v50; // ecx
    int v51; // ecx
    u16 v54; // ax
    competent_noyce_0x6E* v56; // ebp
    u16 field_2_xpos; // bx
    u16 v58; // dx
    int* v59; // eax
    u16* v60; // ecx
    u16 field_4_ypos; // ax
    u16 v63; // ax
    small_string* v66; // ecx
    agitated_keldysh_0xF0* v85;
    int a5;
    int a6;

    const u16 local_field_132_f136_idx = field_132_f136_idx;
    const s32 v98 = gText_0x14_704DFC->field_10_lang_code != 'j' ? 14 : 16;
    const s32 v5 = field_132_f136_idx;
    s32 v6 = 1509 * field_132_f136_idx;
    loving_borg_0xBCA* v7 = &field_136[local_field_132_f136_idx];

    u16 temp_unk1;

    /*
for (s32 i=0; i<3; i++)
{
    wprintf(L"%S\n", field_136[i].field_4[0].field_6_wstr_buf);
}*/
    printf("field_132_f136_idx %d\n", field_132_f136_idx);

    if (field_132_f136_idx == 0)
    {
        swprintf(tmpBuff_67BD9C, L"GTA2 V%d.%d", gGTA2VersionMajor_708280, gGTA2VersionMajor_708284);
        DrawText_4B87A0(tmpBuff_67BD9C, 300, 460, font_type_703C14, 1);

        // Testing...
        //  buffer, x, y, font_type, palette, scale, lang={14,16,21}, count line
        //Frontend::sub_4B78B0(L"ABCDE", 300, 400, 4, 10, 1, 16, true);
        //DrawText_4B87A0(L"ABCDEFGHIJKLM\nNOPQRSTUVW\nXYZ 0123456789", (s32)300, (s32)370, 4, 1);
        //s32 local = 2;
        //DrawText_5D8A10(L"ABCDEFGHIJKLM\nNOPQRSTUVW\nXYZ 0123456789", (s32)300, (s32)370, 4, 1, &local, 0, 0, 0);
    }

    if (field_132_f136_idx == 1)
    {
        if (field_110_state == 3)
        {
            v7->field_518[8].field_1 = 0;
            v7->field_518[9].field_1 = 0;

            // NOTE: field_124_font_type is u16
            // NOTE: sub_4B7E10 is not static

            v12 = sub_4B7E10(2, 0x12Cu, 0x1B8u, field_124_font_type, 0xFFFF); // text: ENTER
            v14 = sub_4B7E10(11, v12 + 300, 0x1B8u, field_124_font_type, 0xFFFF); // text: : ENTER NAME

            v15 = sub_4B7E10(3, 0x12Cu, 0x1CCu, field_124_font_type, 0xFFFF); // text: BACKSPACE
            sub_4B7E10(10, v15 + 300, 0x1CCu, field_124_font_type, 0xFFFF); // text: : DELETE LETTER
        }
        else
        {
            u16 idx = v7->field_4[0].field_6E_count;
            temp_unk1 = idx;
            v18 = Frontend::sub_5D8990(gJolly_poitras_0x2BC0_6FEAC0->field_26A0[idx].field_90_strPlayerName, field_11C) + 10;

            if (v18 == 10)
            {
                v18 = Frontend::sub_5D8990(v7->field_4[0].field_6_wstr_buf, field_11C) + 40;
            }
            v7->field_518[9].field_2_xpos = v18 + v7->field_4[0].field_2;
        }
    }

    if (field_132_f136_idx == 5)
    {
        if (field_EE0D < 3)
        {
            v85 = gJolly_poitras_0x2BC0_6FEAC0->field_1890[field_EE0D];
            // TODO: STUB
            //Frontend::sub_4B5430((small_string**)&v85->field_0, 300, 250, 5, field_12A, 0xFFFF, 2);
        }
        else if (field_EE0D < 6u)
        {
            //v85 = gJolly_poitras_0x2BC0_6FEAC0->field_16B0[field_EE0D].field_0; // TODO: struct at field_16B0
            // TODO: STUB
            //Frontend::sub_4B5430((small_string**)&v85->field_0, 300, 250, 5, field_12A, 0xFFFF, 2);
        }
        else
        {
            if (field_EE0D < 9u)
            {
                // TODO: struct at field_17A0
                //v85 = gJolly_poitras_0x2BC0_6FEAC0->field_17A0[field_EE0D].field_0; // (agitated_keldysh_0xF0*)(& + 60 * v19);
                // TODO: STUB
                //Frontend::sub_4B5430((small_string**)&v85->field_0, 300, 250, 5, field_12A, 0xFFFF, 2);
            }
            else
            {
                v85 = gJolly_poitras_0x2BC0_6FEAC0->field_1890[field_EE0D];
                // TODO: STUB
                //Frontend::sub_4B5430((small_string**)&v85->field_0, 300, 250, 5, field_12A, 0xFFFF, 2);
            }
        }

        if (!byte_67DA80)
        {
            if (!field_EE0D)
            {
                v7->field_518[2].field_1 = false;
            }
            else
            {
                v7->field_518[2].field_1 = true;
            }
            if (field_EE0D == 11)
            {
                v7->field_518[3].field_1 = false;
            }
            else
            {
                v7->field_518[3].field_1 = true;
            }
        }
    }

    v21 = field_132_f136_idx;
    u8 temp1;
    u8 temp2;
    if (v21 == 2 || v21 == 3 || v21 == 6 || v21 == 11)
    {
        if (!gLucid_hamilton_67E8E0.sub_4C59A0())
        {
            temp1 = gLucid_hamilton_67E8E0.sub_4C5980();
            temp2 = 0;
        }
        else
        {
            v23 = gLucid_hamilton_67E8E0.sub_4C5990();
            temp1 = v23 >> 4;
            temp2 = v23 & 0xF;
        }
        v24 = temp1;
        v25 = temp2;
        if (field_132_f136_idx == 6)
        {
            // TODO: remove volatile
            volatile s32 scale_unk = 3 * temp1 + temp2 + 64;
            swprintf(tmpBuff_67BD9C, L"%s %c", gText_0x14_704DFC->Find_5B5F90("bonslev"));
            wcsncpy(v7->field_518[0].field_6_wstr_buf, tmpBuff_67BD9C, 0x32u);

            // TODO: STUB
            //Frontend::sub_4B7D60();
        }
        /*
        LOWORD(v22) = field_12A;
        v2 = v25 + 4 * v24;
        */
        //Frontend::sub_4B5430(gJolly_poitras_0x2BC0_6FEAC0->field_1890[0][v2].field_0, 0xAAu, 155, 3, v22, 0xFFFF, 2);

        // TODO: STUB
        /*
        Frontend::sub_4B5430((small_string**)&gJolly_poitras_0x2BC0_6FEAC0->field_1890[v24][v25].field_0,
                             0xAAu,
                             155,
                             3,
                             field_12A,
                             0xFFFF,
                             2);
        */

        v27 = field_132_f136_idx;
        if (v27 == 2 || v27 == 3 || v27 == 11)
        {
            Frontend::sub_4B57B0(10, 0xE1);
        }
    }

    if (field_132_f136_idx == 7)
    {
        Frontend::sub_4B55F0();
    }

    u32 temp3 = -1;
    u16 temp4 = 0;

    nifty_maxwell_0x82* v31;

    u16 v53;
    u8 v53_u8;

    u8 unk_idx;

    wchar_t* a4_unk;

    s32 v29;

    u16 v2_u16;
    u16 v93_a;

    //  note: probably 'for' structure
    if (v7->field_0 > 0)
    {
        for (temp4 = 0; (u16)(temp4) < v7->field_0; temp4++)
        {
            v29 = temp4;
            v30 = temp4;
            v31 = &v7->field_4[(u16)temp4];

            if (v7->field_4[temp4].field_1)
            {
                if (v31->field_0 == 2)
                {
                    //v30 = (__int32)Frontend::sub_4B3AF0(field_132_f136_idx, a3[0], a4_unk);
                    // TODO: STUB
                    //Frontend::sub_4B3AF0(field_132_f136_idx, temp4, &a4_unk);
                }
                else
                {
                    a4_unk = (wchar_t*)&v7->field_4[temp4].field_6_wstr_buf;
                }
                u16 v29_u16 = temp4;
                v2_u16 = v31->field_2;
                v93_a = v31->field_4;
                if (temp4 == v7->field_BC6_nifty_idx)
                {
                    DrawText_4B87A0(a4_unk, v2_u16, v93_a, field_120, 1);

                    if (field_132_f136_idx == 1)
                    {
                        v35 = temp4;
                        v7->field_518[4].field_1 = 0;
                        v7->field_518[5].field_1 = 0;
                        v7->field_518[6].field_1 = 0;
                        v7->field_518[7].field_1 = 0;
                        v7->field_518[8].field_1 = 0;
                        v7->field_518[9].field_1 = 0;
                        if ((u16)temp4 == 3)
                        {
                            temp3 = 3;
                            v7->field_518[4].field_6_wstr_buf[0] = 1;
                            v7->field_518[5].field_6_wstr_buf[0] = 2;
                            v7->field_518[4].field_1 = field_1EB4C != 0;
                            v7->field_518[5].field_1 = field_1EB4D != 0;
                        }
                        else if (v35 == 4)
                        {
                            temp3 = 4;
                            v7->field_518[6].field_6_wstr_buf[0] = 1;
                            v7->field_518[7].field_6_wstr_buf[0] = 2;
                            v7->field_518[6].field_1 = field_1EB4E != 0;
                            v7->field_518[7].field_1 = field_1EB4F != 0;
                        }
                        else if (v35 == 0)
                        {
                            v7->field_518[8].field_6_wstr_buf[0] = 1;
                            v7->field_518[9].field_6_wstr_buf[0] = 2;
                            if (field_110_state != 3)
                            {
                                v7->field_518[8].field_1 = 1;
                                v7->field_518[9].field_1 = 1;
                                field_1EB4A = 1;
                                field_1EB4B = 1;
                                if (!byte_67DA80)
                                {
                                    v35 = temp_unk1;
                                    if (!temp_unk1)
                                    {
                                        v7->field_518[8].field_1 = temp_unk1;
                                        field_1EB4A = v35;
                                    }
                                    else
                                    {
                                        if (temp_unk1 == v7->field_4[0].field_7E)
                                        {
                                            v7->field_518[9].field_1 = 0;
                                            field_1EB4B = 0;
                                        }
                                    }
                                }

                                v37 = sub_4B7E10(2, 0x12Cu, 0x1B8u, field_124_font_type, 0xFFFF);
                                sub_4B7E10(8, v37 + 300, 0x1B8u, field_124_font_type, 0xFFFF);
                                v40 = sub_4B7E10(1, 0x12Cu, 0x1CCu, field_124_font_type, 0xFFFF);
                                sub_4B7E10(9, v40 + 300, 0x1CCu, field_124_font_type, 0xFFFF);
                            }
                        }
                    }
                    else if (field_132_f136_idx == 5)
                    {
                        v7->field_518[2].field_6_wstr_buf[0] = 3;
                        v7->field_518[3].field_6_wstr_buf[0] = 4;
                        if ((u16)temp4 == 0)
                        {
                            v7->field_518[2].field_6_wstr_buf[0] = 1;
                            v7->field_518[3].field_6_wstr_buf[0] = 2;
                        }
                    }
                }
                else
                {
                    if (v31->field_6A != 0xFFFF)
                    {
                        if (v31->field_6C == 0xFFFF)
                        {
                            DrawText_4B87A0(a4_unk, v2_u16, v93_a, v31->field_6A, 1);
                        }
                        else
                        {
                            s32 eight_6 = 8;
                            DrawText_5D8A10(a4_unk, v2_u16, v93_a, v31->field_6A, (s32)1, &eight_6, v31->field_6C, false, 0);
                        }
                    }
                    else
                    {
                        if (v31->field_6C == 0xFFFF)
                        {
                            DrawText_4B87A0(a4_unk, v2_u16, v93_a, field_11C, 1);
                        }
                        else
                        {
                            s32 eight_unk_8 = 8;
                            DrawText_5D8A10(a4_unk, v2_u16, v93_a, field_11C, (s32)1, &eight_unk_8, v31->field_6C, false, 0);
                        }
                    }
                }
            }
            else
            {
                v44 = field_132_f136_idx;
                bool v45 = false;
                if ((v44 == 1 || v44 == 3) && temp4 == 1)
                {
                    v45 = true;
                }
                if (v44 != 11 && v44 != 2 || temp4 != 0)
                {
                    if (!v45)
                    {
                        continue;
                    }
                    v29 = temp4;
                }
                if (v31->field_0 == 2)
                {
                    //Frontend::sub_4B3AF0(v44, v29, &a4_unk);  TODO: STUB
                }
                else
                {
                    a4_unk = (wchar_t*)&v7->field_4[temp4].field_6_wstr_buf;
                }
                v29 = v31->field_4;
                v2_u16 = v31->field_2;
                u16 v93_a = v29;

                if (v31->field_6A != 0xFFFF)
                {
                    s32 eight_unk = 8;
                    DrawText_5D8A10(a4_unk, v2_u16, v93_a, v31->field_6A, (s32)1, &eight_unk, 8, false, 0);
                }
                else
                {
                    s32 eight_unk_2 = 8;
                    DrawText_5D8A10(a4_unk, v2_u16, v93_a, field_11C, (s32)1, &eight_unk_2, 8, false, 0);
                }
            }
        } //  end FOR

        if (temp3 != 3)
        {
            if (temp3 == 4)
            {
                v53_u8 = gLucid_hamilton_67E8E0.sub_4C5990();
                unk_idx = v53_u8 >> 4;
                u8 a5_idx = v53_u8 & 0xF;
                /*
                Frontend::sub_4B5430((small_string**)gJolly_poitras_0x2BC0_6FEAC0->field_1890[unk_idx][0].field_0,
                                     0x12Cu,
                                     v98,
                                     1,
                                     field_128,
                                     0xFFFF,
                                     2);
                */
            }
        }
        else
        {
            unk_idx = gLucid_hamilton_67E8E0.sub_4C5980();
            /*
            Frontend::sub_4B5430((small_string**)gJolly_poitras_0x2BC0_6FEAC0->field_1890[unk_idx][0].field_0,
                                 0x12Cu,
                                 v98,
                                 1,
                                 field_128,
                                 0xFFFF,
                                 2);
            */
        }
    }

    v54 = 0;
    temp4 = 0;

    s32 v55;

    if (v7->field_2 > 0)
    {
        for (temp4 = 0; (u16)(temp4) < (u16)v7->field_2; temp4++)
        {
            v56 = &v7->field_518[(u16)temp4];

            if (v7->field_518[(u16)temp4].field_1)
            {
                if (v56->field_0 == 3)
                {
                    field_2_xpos = v7->field_518[(u16)temp4].field_2_xpos;
                    v58 = v7->field_518[(u16)temp4].field_4_ypos;

                    switch (v7->field_518[v55].field_6_wstr_buf[0])
                    {
                        case 0u:
                            a5 = 2;
                            break;
                        case 1u:
                            a5 = 37;
                            break;
                        case 2u:
                            a5 = 38;
                            break;
                        case 3u:
                            a5 = 40;
                            break;
                        case 4u:
                            a5 = 41;
                            break;
                        default:
                            break;
                    }

                    /*
                    scale = 0;
                    font_type = 0;
                    ypos.mValue = 0;
                    v59 = &v98;
                    v98 = 2;
                    xpos.mValue = 0;
                    v87.mValue = (__int32)&v98;
                    LOWORD(v59) = HIWORD(dword_67DA6E);
                    v86 = dword_67D934;
                    v85 = (agitated_keldysh_0xF0*)v59;
                    v84 = dword_67D934;
                    Fix16::FromU16_4AE970(&v84, v58);
                    v83 = v60;
                    Fix16::FromU16_4AE970(&v83, field_2_xpos);
                    sub_5D7EC0(6, a5, (int)v83, v84, (int)v85, v86, (int*)v87.mValue, xpos.mValue, ypos.mValue, font_type, scale);
                    */
                    //  11 params
                    s32 two = 2;
                    //sub_5D7EC0(6, a5, field_2_xpos, dword_67D934, word_67DA70, dword_67D934, &two, 0, 0, false, 0);
                }
                else if (v56->field_0 == 1)
                {
                    v2_u16 = v7->field_518[(u16)temp4].field_6A;
                    field_4_ypos = v7->field_518[(u16)temp4].field_4_ypos;
                    u16 v96_u16 = v56->field_2_xpos;
                    u16 v93_u16 = field_4_ypos;
                    if (v2_u16 == 0xFFFF)
                    {
                        v2_u16 = field_11C;
                    }
                    Frontend::sub_4B3CC0(field_132_f136_idx, temp4, &a4_unk);
                    v63 = field_132_f136_idx;
                    u16 v64_u16;
                    if (v63 == 1 && ((u16)temp4 == 2 || (u16)temp4 == 3))
                    {
                        v64_u16 = v56->field_6C;
                        Frontend::sub_4B78B0(a4_unk, v96_u16, v93_u16, v2_u16, v64_u16, 1u, 0x15u, 1);
                    }
                    else if (v63 == 5 && (u16)temp4 == 1)
                    {
                        u16 v62_u16 = v56->field_6C;
                        Frontend::sub_4B78B0(a4_unk, v96_u16, v93_u16, v2_u16, v62_u16, 1u, 0x15u, 1);
                    }
                    else
                    {
                        u16 v56_u16 = v56->field_6C;
                        if (v56_u16 == 0xFFFF)
                        {
                            DrawText_4B87A0(a4_unk, v96_u16, v93_u16, v2_u16, 1);
                        }
                        else
                        {
                            s32 eight_again = 8;
                            DrawText_5D8A10(a4_unk, v96_u16, v93_u16, v2_u16, (s32)1, &eight_again, v56_u16, false, 0);
                        }
                    }
                }
            }
        }
    }

    u16 x_pos;
    u16 y_pos;

    if (field_110_state == 3) //  enter new player name?
    {
        if (field_114)
        {
            x_pos = v7->field_4[0].field_2 + Frontend::sub_5D8990(field_C9A0, field_11C);
            y_pos = v7->field_4[0].field_4;
            swprintf(tmpBuff_67BD9C, L"_");
            DrawText_4B87A0(tmpBuff_67BD9C, x_pos, y_pos, field_11C, 1);
        }
        wcscpy(tmpBuff_67BD9C, gText_0x14_704DFC->Find_5B5F90("entrnam"));
        u16 v74_u8 = gText_0x14_704DFC->field_10_lang_code != 106 ? 22 : 11;
        x_pos = 0x15Eu;
        y_pos = v74_u8 + 16;
        DrawText_4B87A0(tmpBuff_67BD9C, x_pos, y_pos, field_126, 1);
    }
    if (field_110_state == 5) //  change current player name?
    {
        if (field_114)
        {
            x_pos = v7->field_518[4].field_2_xpos + Frontend::sub_5D8990(field_C9B8, field_11C);
            y_pos = v7->field_518[4].field_4_ypos;
            swprintf(tmpBuff_67BD9C, L"_");
            DrawText_4B87A0(tmpBuff_67BD9C, x_pos, y_pos, field_11C, 1);
        }
    }
}

STUB_FUNC(0x4B7AE0)
void Frontend::sub_4B7AE0()
{
    // todo

    // TEST

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

STUB_FUNC(0x4B6030)
char_type Frontend::pre_intro_bik_exists_4B6030()
{
    char_type* v0; // eax
    long v1; // eax
    _finddata_t v3; // [esp+0h] [ebp-118h] BYREF

    v0 = gFrontend_67DC84->pre_intro_bik_4B5F20();
    v1 = _findfirst(v0, &v3);
    if (v1 == -1)
        return 0;
    _findclose(v1);
    return 1;
}

STUB_FUNC(0x4B5F20)
char_type* Frontend::pre_intro_bik_4B5F20()
{
    // todo
    return "";
}

STUB_FUNC(0x4B5E50)
const char_type* Frontend::intro_bik_4B5E50()
{
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
        case 3:
            sub_4B2F60();
            break;

        case 5:
            sub_4B8280();
            break;

        case 1:
            if (field_132_f136_idx == 9)
            {
                snd1_67D818.field_4_bStatus = 1;
                sub_4B7A10();
            }
            else
            {
                snd1_67D818.field_4_bStatus = 0;
                sub_4AE2D0();
            }
            break;

        case 2:
            sub_4AE990();
            break;

        case 4:
            sub_4AE9A0();
            break;

        default:
            FatalError_4A38C0(1006, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 2059, field_110_state);
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
    loving_borg_0xBCA* pBorg = &field_136[field_132_f136_idx];
    if (field_110_state != 2)
    {
        if (field_132_f136_idx == 0)
        {
            switch (pBorg->field_BC6_nifty_idx)
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
        else if (field_132_f136_idx == 1)
        {
            switch (pBorg->field_BC6_nifty_idx)
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
        else if (field_132_f136_idx == 5)
        {
            field_EE08 = HiScoresDisplay_12;
        }
        else if (field_132_f136_idx == 2)
        {
            field_EE08 = GameOver_13;
        }
        else if (field_132_f136_idx == 3u || field_132_f136_idx == 6 || field_132_f136_idx == 7 || field_132_f136_idx == 0xB ||
                 field_132_f136_idx == 0xE)
        {
            field_EE08 = RedBar_16;
        }
        else if (field_132_f136_idx == 4 || field_132_f136_idx == 0xA)
        {
            field_EE08 = Loading_15;
        }
        else if (field_132_f136_idx == 9)
        {
            field_EE08 = Credits_17;
        }
        else
        {
            field_EE08 = PlayArea1_7;
        }
    }
}

STUB_FUNC(0x4B7A10)
void Frontend::sub_4B7A10()
{
    char_type v2; // cl
    char_type* local_field_8_keys; // eax
    s32 v4; // edx
    s32 v5; // eax
    s32 v6; // eax

    timeGetTime();
    read_menu_input_4AFEB0();
    v2 = 0;
    local_field_8_keys = field_8_keys;
    v4 = 256;
    do
    {
        if (*local_field_8_keys < 0)
            v2 = 1;
        ++local_field_8_keys;
        --v4;
    } while (v4);
    if (v2)
    {
        if (!field_C9B3)
        {
        LABEL_7:
            field_108 = 1;
            return;
        }
    }
    else
    {
        field_C9B3 = 0;
    }
    if (++field_1EB30)
    {
        v5 = field_1EB34;
        field_1EB30 = 0;
        if (v5 <= -327680)
        {
            while (++field_1EB38 != 600)
            {
                v6 = ((u16)field_EE0E_unk.field_2[(u16)field_1EB38].field_4 << 14) + field_1EB34;
                field_1EB34 = v6;
                if (v6 > -327680)
                    goto LABEL_13;
            }
            goto LABEL_7;
        }
    LABEL_13:
        field_1EB34 -= dword_67D9FC;
    }
}

STUB_FUNC(0x4AE2D0)
void Frontend::sub_4AE2D0()
{
    loving_borg_0xBCA* pBorg; // ebx
    dreamy_clarke_0xA4* v3; // ebp
    u16 v4; // ax
    u8 v5; // bl
    char_type v6; // al
    u8 v7; // bl
    u8 v8; // di
    u8 v9; // bl
    u16 field_BC6_nifty_idx; // cx
    nifty_maxwell_0x82* v11; // edi
    bool v12; // bl
    s32 v13; // eax
    u16 v14; // cx
    nifty_maxwell_0x82* v15; // edi
    bool v16; // bl
    bool v17; // al
    loving_borg_0xBCA* v18; // [esp+10h] [ebp-Ch]
    u8 v19; // [esp+14h] [ebp-8h]
    s32 v20; // [esp+18h] [ebp-4h]
    u8 i; // [esp+18h] [ebp-4h]

    pBorg = &field_136[field_132_f136_idx];
    v18 = pBorg;
    v3 = sub_4B43E0();
    if (field_C9D0_return_pressed)
    {
        if (pBorg->field_4[pBorg->field_BC6_nifty_idx].field_0 == 1)
        {
            v4 = pBorg->field_4[pBorg->field_BC6_nifty_idx].field_80;
            switch (v4)
            {
                case 257u:
                    Start_GTA2Manager_5E4DE0();
                    snd1_67D818.field_0_object_type = 5;
                    break;
                case 258u:
                    field_108 = 1;
                    snd1_67D818.field_0_object_type = 5;
                    break;
                case 259u:
                    goto LABEL_21;
                case 260u:
                    v20 = gLucid_hamilton_67E8E0.GetPlySlotIdx_4C59B0(); //  LOBYTE(v20) =
                    if (PlySlotSvgExists_4B5370(v20))
                    {
                        sub_4B4EC0();
                    }
                    else
                    {
                    }
                LABEL_21:
                    gLucid_hamilton_67E8E0.DebugStr_4C58D0(byte_67DC88);
                    goto LABEL_28;
                case 261u:
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
                        FatalError_4A38C0(185, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 1543);
                    }

                    if (!v3->field_0[v7][0].field_0)
                    {
                        FatalError_4A38C0(186, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 1548);
                    }
                    sub_4B4D00(v7, 0);
                    goto LABEL_9;
                case 263u:
                    v19 = gLucid_hamilton_67E8E0.sub_4C5980();
                    v5 = 3;
                    for (i = 3; !v3->field_0[v19][i].field_0 || v5 >= field_1EB51_blocks[v19]; i = v5)
                    {
                        --v5;
                    }
                    sub_4B4D00(v19, i);
                    gLucid_hamilton_67E8E0.sub_4C5AD0(0);
                    goto LABEL_9;
                case 264u:
                    v8 = gLucid_hamilton_67E8E0.sub_4C5980();
                    if (!FreeLoader::sub_4AE1F0(v8))
                    {
                        goto LABEL_11;
                    }
                    sub_4B4D00(v8, 0);
                LABEL_28:
                    field_EE08 = RedBar_16;
                    field_110_state = 2;
                    snd1_67D818.field_0_object_type = 5;
                    break;
                case 265u:
                    v9 = gLucid_hamilton_67E8E0.sub_4C5990();
                    if (!FreeLoader::sub_4AE1F0(v9 >> 4))
                    {
                        goto LABEL_10;
                    }
                    sub_4B4D00(v9 >> 4, v9 & 0xF);
                    gLucid_hamilton_67E8E0.sub_4C5AD0(1);
                LABEL_9:
                    field_EE08 = RedBar_16;
                    field_110_state = 2;
                LABEL_10:
                    pBorg = v18;
                LABEL_11:
                    snd1_67D818.field_0_object_type = 5;
                    break;
                case 266u:
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
        else if (field_132_f136_idx == 1 && !pBorg->field_BC6_nifty_idx)
        {
            field_110_state = 3;
            sub_4B4280();
            field_C9B2 = wcslen(field_C9A0);
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
            case 0u:
            case 0xEu:
                sub_4B3170(9u);
                break;
            case 1u:
            case 2u:
            case 3u:
            case 4u:
            case 6u:
            case 0xAu:
            case 0xBu:
                sub_4B3170(0);
                break;
            case 5u:
                sub_4B3170(1u);
                break;
            default:
                field_108 = 1;
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
        field_BC6_nifty_idx = pBorg->field_BC6_nifty_idx;
        v11 = &pBorg->field_4[field_BC6_nifty_idx];
        if (v11->field_0 == 2)
        {
            v12 = pBorg->field_4[pBorg->field_BC6_nifty_idx].sub_4B6390();
            if (field_132_f136_idx == 1 && !v18->field_BC6_nifty_idx)
            {
                gLucid_hamilton_67E8E0.sub_4C5920(v11->field_6E_count);
                sub_4B42E0(); // this
                gRegistry_6FF968.Set_Player_Setting_5878C0("plyrslot", v11->field_6E_count);
                if (v12)
                {
                    snd1_67D818.field_0_object_type = 3;
                }
            }

            if (field_132_f136_idx == 5 && !v18->field_BC6_nifty_idx)
            {
                field_EE0D = v11->field_6E_count;
                if (v12)
                {
                    snd1_67D818.field_0_object_type = 3;
                }
            }

        LABEL_59:
            pBorg = v18;
            goto LABEL_60;
        }

        if (field_132_f136_idx == 1)
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

        v14 = pBorg->field_BC6_nifty_idx;
        v15 = &pBorg->field_4[v14];
        if (v15->field_0 == 2)
        {
            v16 = pBorg->field_4[pBorg->field_BC6_nifty_idx].sub_4B6330(); // this
            if (field_132_f136_idx == 1 && !v18->field_BC6_nifty_idx)
            {
                gLucid_hamilton_67E8E0.sub_4C5920(v15->field_6E_count);
                sub_4B42E0();
                gRegistry_6FF968.Set_Player_Setting_5878C0("plyrslot", v15->field_6E_count);
                if (v16)
                {
                    snd1_67D818.field_0_object_type = 4;
                }
            }
            if (field_132_f136_idx == 5 && !v18->field_BC6_nifty_idx)
            {
                field_EE0D = v15->field_6E_count;
                if (v16)
                {
                    snd1_67D818.field_0_object_type = 4;
                }
            }
            goto LABEL_59;
        }

        if (field_132_f136_idx == 1)
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
    if (field_C9D2_delete_pressed && field_132_f136_idx == 1 && !pBorg->field_BC6_nifty_idx)
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
void Frontend::sub_4AE990()
{
    field_108 = 3;
}

STUB_FUNC(0x4B2F60)
void Frontend::sub_4B2F60()
{
    // todo
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
                    FatalError_4A38C0(0x3EE, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 1934);
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
                FatalError_4A38C0(0x3EE, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 1968);
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
                FatalError_4A38C0(0x3EE, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 1991);
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
    // todo
}

MATCH_FUNC(0x4B4410)
void Frontend::sub_4B4410()
{
    sub_4B43E0()->sub_56B630();
    gJolly_poitras_0x2BC0_6FEAC0->sub_56BA60(field_136[1].field_4[0].field_6E_count);
    sub_4B42E0();
}

MATCH_FUNC(0x4B43E0)
dreamy_clarke_0xA4* Frontend::sub_4B43E0()
{
    // note: movsx vs movzx due to signedness
    u16 idx = gLucid_hamilton_67E8E0.GetPlySlotIdx_4C59B0();
    return &gJolly_poitras_0x2BC0_6FEAC0->field_26A0[idx];
}

STUB_FUNC(0x4B42E0)
void Frontend::sub_4B42E0()
{
    // todo
}

MATCH_FUNC(0x4B4230)
void Frontend::sub_4B4230()
{
    u16 count = field_136[1].field_4[0].field_6E_count;
    wchar_t* pStr = gJolly_poitras_0x2BC0_6FEAC0->field_26A0[count].field_90_strPlayerName;
    wcsncpy(pStr, field_C9A0, 9u);
    HandleCheatCode_4B3DD0(pStr);
    gJolly_poitras_0x2BC0_6FEAC0->sub_56BA60(count);
}

MATCH_FUNC(0x4B3CC0)
void Frontend::sub_4B3CC0(u16 a2, u16 a3, wchar_t** a4)
{
    competent_noyce_0x6E* temp = &field_136[a2].field_518[a3];

    if (a2 == 14 && a3 == 4)
    {
        wcscpy(word_67C7D8, field_C9B8);
    }
    else if ((a2 == 14 && a3 != 4) || a2 != 5 || a3 != 1)
    {
        swprintf(word_67C7D8, L"%s", temp->field_6_wstr_buf);
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
        bExplodingOff_67D4FB = bExplodingOff_67D4FB == 0;
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
    wcsncpy(field_C9A0, gJolly_poitras_0x2BC0_6FEAC0->field_26A0[field_136[1].field_4[0].field_6E_count].field_90_strPlayerName, 9u);
}

STUB_FUNC(0x4B8530)
void Frontend::sub_4B8530()
{
    short total = field_C9CA;
    if (total < 9)
    {
        s32 remainder = 9 - total;
        //remainder &= 0xFFFF;
        wchar_t* pStart = &field_C9B8[total];
        while (remainder)
        {
            *pStart = 0;
            pStart++;
            remainder--;
        }
        //        wmemset(&field_C9B8[total], 0, remainder);
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

        wcsncpy(field_136[14].field_518[0].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("fr_rnt1"), 0x32u);
        wcsncpy(field_136[14].field_518[1].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("fr_rnt2"), 0x32u);
        wcsncpy(field_136[14].field_518[2].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("fr_rnt3"), 0x32u);

        field_C9CB = 1;
    }
}

MATCH_FUNC(0x4B8020)
void Frontend::sub_4B8020()
{
    dreamy_clarke_0xA4* pClarke = sub_4B43E0();
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
        while (!pClarke->field_0[idx][i].field_0 || i >= field_1EB51_blocks[idx])
        {
            i--;
        }

        sub_4B4D00(idx, i);
        gLucid_hamilton_67E8E0.sub_4C5AD0(0);
        field_EE08 = RedBar_16;
        field_110_state = 2;
    }
}

STUB_FUNC(0x4B7E10)
EXPORT int __stdcall Frontend::sub_4B7E10(s32 str_id_idx, u16 text_xpos, u16 text_ypos, s32 fontType, s32 draw_kind)
{
    return 0;
}

MATCH_FUNC(0x4B7FB0)
char_type Frontend::sub_4B7FB0()
{
    dreamy_clarke_0xA4* v2 = sub_4B43E0();
    u16 v3 = 0;
    // note: two separated while's interlaced by a backwards goto may be actually two nested while's
    while (v3 < field_1EB50_idx)
    {
        u16 v4 = 0;
        while (v4 < field_1EB51_blocks[v3])
        {
            if (!v2->field_0[v3][v4].field_0)
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
void Frontend::sub_4B4D00(u8 mainBlockIdx, u8 bonusBlockIdx)
{
    char fullPath[256]; // [esp+10h] [ebp-400h] BYREF
    char debugStr[256]; // [esp+110h] [ebp-300h] BYREF
    char mapName[256]; // [esp+210h] [ebp-200h] BYREF
    char styName[256]; // [esp+310h] [ebp-100h] BYREF

    sub_4B4C60( mainBlockIdx, bonusBlockIdx, debugStr, mapName, styName);
    gLucid_hamilton_67E8E0.DebugStr_4C58D0(byte_67DC88);
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
    switch (field_110_state)
    {
        case 1:
        case 3:
        case 5:
            if (field_132_f136_idx == 9)
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
            const s32 x = sub_4B0190(gText_0x14_704DFC->Find_5B5F90("loading"), -1, 320);

            Fix16 scale;
            scale.mValue = 0x4000;

            Fix16 ypos;
            ypos.FromInt_4369F0(260);

            Fix16 xpos;
            xpos.FromU16_4AE970(x);

            DrawText_4B87A0(gText_0x14_704DFC->Find_5B5F90("loading"), xpos, ypos, field_11C, scale);
            break;
        }

        case 4:
        {
            if (field_EE0C == 1)
            {
                const s32 v12 = gText_0x14_704DFC->field_10_lang_code - 'j';

                Fix16 scale;
                scale.mValue = 0x4000;

                // TODO: Fix this part
                u8 v13 = -(v12 != 0);
                v13 = v13 & 0xFC;

                Fix16 ypos;
                ypos.FromU16_4AE970(v13 + 16);

                Fix16 xpos;
                xpos.FromU16_4AE970(275);

                DrawText_4B87A0(gText_0x14_704DFC->Find_5B5F90("clrchar"), xpos, ypos, field_126, scale);
            }

            {
                Fix16 scale;
                scale.mValue = 0x4000;

                u16 fontType;
                if (field_EE0A == 190)
                {
                    fontType = field_120;
                }
                else
                {
                    fontType = field_11C;
                }

                Fix16 ypos;
                ypos.FromInt_4369F0(190);

                Fix16 xpos;
                xpos.FromInt_4369F0(300);
                DrawText_4B87A0(gText_0x14_704DFC->Find_5B5F90("sure"), xpos, ypos, fontType, scale);
            }

            {
                Fix16 scale;
                scale.mValue = 0x4000;

                u16 fontType;
                if (field_EE0A == 210)
                {
                    fontType = field_120;
                }
                else
                {
                    fontType = field_11C;
                }

                Fix16 ypos;
                ypos.FromInt_4369F0(210);

                Fix16 xpos;
                xpos.FromInt_4369F0(300);

                DrawText_4B87A0(gText_0x14_704DFC->Find_5B5F90("yes"), xpos, ypos, fontType, scale);
            }

            {
                Fix16 scale;
                scale.mValue = 0x4000;

                u16 fontType;
                if (field_EE0A == 230)
                {
                    fontType = field_120;
                }
                else
                {
                    fontType = field_11C;
                }

                Fix16 ypos;
                ypos.FromInt_4369F0(230);

                Fix16 xpos;
                xpos.FromInt_4369F0(300);

                DrawText_4B87A0(gText_0x14_704DFC->Find_5B5F90("no"), xpos, ypos, fontType, scale);
            }
            break;
        }

        default:
            FatalError_4A38C0(0x3EE, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 1217, field_110_state);
            break;
    }
}

STUB_FUNC(0x5D7D30)
EXPORT void __stdcall sub_5D7D30()
{
    pVid_GetSurface(gVidSys_7071D0);
    pMakeScreenTable((int)gVidSys_7071D0->field_50_surface_pixels_ptr,
                     gVidSys_7071D0->field_54_surface_pixels_pitch,
                     gVidSys_7071D0->field_4C_rect_bottom);

    if (gVidSys_7071D0->field_40_full_screen == -2)
    {
        dword_70679C = window_height_706B50 - 1;
        dword_70675C = window_width_706630 - 1;
    }
    else
    {
        dword_70675C = gVidSys_7071D0->field_48_rect_right - 1;
        dword_70679C = gVidSys_7071D0->field_4C_rect_bottom - 1;
    }

    pgbh_SetWindow(0, 0, dword_70675C, dword_70679C);
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
        FatalError_4A38C0(16, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 6516);
    }

    void* pAlloc = malloc(tgaArray_61F0C8[idx].field_80_len);
    if (!pAlloc)
    {
        FatalError_4A38C0(174, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 6523);
    }

    if (crt::fread(pAlloc, 1u, tgaArray_61F0C8[idx].field_80_len, hFile) != tgaArray_61F0C8[idx].field_80_len)
    {
        FatalError_4A38C0(187, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 6529);
    }

    tgaArray_61F0C8[idx].field_84_img = pgbh_LoadImage((SImage*)pAlloc);

    crt::fclose(hFile);
    free(pAlloc);
}

STUB_FUNC(0x5D7CB0)
EXPORT void __stdcall sub_5D7CB0()
{
    // todo: move me
}

// https://decomp.me/scratch/MuqZh
STUB_FUNC(0x4AF2A0)
Frontend::Frontend()
{
    field_10D = 1;

    gText_0x14_704DFC = new text_0x14();
    if (!gText_0x14_704DFC)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 2288);
    }

    gGtx_0x106C_703DD4 = new gtx_0x106C();
    if (!gGtx_0x106C_703DD4)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 2290);
    }

    gSharp_pare_0x15D8_705064 = new sharp_pare_0x15D8();
    if (!gSharp_pare_0x15D8_705064)
    {
        FatalError_4A38C0(32, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 2292);
    }

    if (gText_0x14_704DFC->field_10_lang_code == 'j')
    {
        // pmagical_germain_0x8EC = pmagical_germain_0x8EC_mem ? magical_germain_0x8EC::ctor_4D2C80(pmagical_germain_0x8EC_mem) : 0;
        gMagical_germain_0x8EC_6F5168 = new magical_germain_0x8EC();
        if (!gMagical_germain_0x8EC_6F5168)
        {
            FatalError_4A38C0(32, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 2297);
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
    field_108 = 2;
    field_C9E1_bCheatsEnabled = 0;

    sub_4AF0E0();

    field_C9DC = timeGetTime();
    field_C9E0 = 0;
    field_132_f136_idx = 0;
    field_C9E4 = 0;

    sub_4B0220();

    field_C9B2 = 0;
    field_C9B3 = 1;
    field_C9B4 = 256;
    field_C9B6 = 5;

    //memset(&field_C9A0, 0, sizeof(field_C9A0));

    *(u32*)field_C9A0 = 0;
    *(u32*)&field_C9A0[2] = 0;
    *(u32*)&field_C9A0[4] = 0;
    *(u32*)&field_C9A0[6] = 0;
    field_C9A0[8] = 0;

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
        FatalError_4A38C0(31, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 2469);
    }

    if (field_4_pKeyboardDevice->SetDataFormat(&gKeyboardDataFormat_601A54) < 0)
    {
        FatalError_4A38C0(9, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 2480);
    }

    if (field_4_pKeyboardDevice->SetCooperativeLevel(gHwnd_707F04, 6) < 0)
    {
        FatalError_4A38C0(10, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 2487);
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
    s16 v30; // ax

    // local_4 = (-(ushort)(cVar1 != 'j') & 0xfffc) + 0x10;

    s32 v2 = gText_0x14_704DFC->field_10_lang_code != 'j' ? 12 : 16;
    field_134 = 16;

    field_136[0].field_0 = 3;
    field_136[0].field_4[0].field_0 = 1; // ebx
    field_136[0].field_4[0].field_2 = 300; // edi
    field_136[0].field_4[0].field_4 = 250;
    wcsncpy(field_136[0].field_4[0].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("play"), 0x32u);
    field_136[0].field_4[0].field_80 = 1;
    field_136[0].field_4[1].field_0 = 1;
    field_136[0].field_4[1].field_2 = 300;
    field_136[0].field_4[1].field_4 = 270;
    wcsncpy(field_136[0].field_4[1].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("options"), 0x32u);
    field_136[0].field_4[1].field_80 = 257;
    field_136[0].field_4[2].field_0 = 1;
    field_136[0].field_4[2].field_2 = 300;
    field_136[0].field_4[2].field_4 = 290;
    wcsncpy(field_136[0].field_4[2].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("quit"), 0x32u);
    field_136[0].field_4[2].field_80 = 9;
    field_136[0].field_B8A[0].field_0 = 280;
    field_136[0].field_B8A[0].field_2 = 258;
    field_136[0].field_B8A[1].field_0 = 280;
    field_136[0].field_B8A[1].field_2 = 278;
    field_136[0].field_B8A[2].field_0 = 280;
    field_136[0].field_B8A[2].field_2 = 298;
    field_136[0].field_BC6_nifty_idx = 0;
    field_136[0].field_BC8 = 0;
    field_136[1].field_0 = 5;
    field_136[1].field_4[0].field_0 = 2;
    field_136[1].field_4[0].field_2 = 300;
    field_136[1].field_4[0].field_4 = 210;
    wcsncpy(field_136[1].field_4[0].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("charctr"), 0x32u);
    field_136[1].field_4[0].field_6E_count = 0;
    field_136[1].field_4[0].field_70 = 0;
    field_136[1].field_4[0].field_7E = 7;

    u16 v77 = 0;
    do
    {
        field_136[1].field_4[0].field_72[v77++] = 1;
    } while (v77 <= field_136[1].field_4[0].field_7E);

    field_136[1].field_4[1].field_0 = 1;
    field_136[1].field_4[1].field_2 = 300;
    field_136[1].field_4[1].field_4 = 230;
    wcsncpy(field_136[1].field_4[1].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("savepos"), 0x32u);
    field_136[1].field_4[1].field_80 = 260;
    field_136[1].field_4[2].field_0 = 1;
    field_136[1].field_4[2].field_2 = 300;
    field_136[1].field_4[2].field_4 = 250;
    wcsncpy(field_136[1].field_4[2].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("hi_scre"), 0x32u);
    field_136[1].field_4[2].field_80 = 5;
    field_136[1].field_4[3].field_0 = 1;
    field_136[1].field_4[3].field_2 = 300;
    field_136[1].field_4[3].field_4 = 270;
    wcsncpy(field_136[1].field_4[3].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("strtlev"), 0x32u);
    field_136[1].field_4[3].field_80 = 264;
    field_136[1].field_4[4].field_0 = 1;
    field_136[1].field_4[4].field_2 = 300;
    field_136[1].field_4[4].field_4 = 350;
    wcsncpy(field_136[1].field_4[4].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("bonslev"), 0x32u);
    field_136[1].field_4[4].field_80 = 265;
    field_136[1].field_B8A[0].field_0 = 280;
    field_136[1].field_B8A[0].field_2 = 218;
    field_136[1].field_B8A[1].field_0 = 280;
    field_136[1].field_B8A[1].field_2 = 238;
    field_136[1].field_B8A[2].field_0 = 280;
    field_136[1].field_B8A[2].field_2 = 258;
    field_136[1].field_B8A[3].field_0 = 280;
    field_136[1].field_B8A[3].field_2 = 278;
    field_136[1].field_B8A[4].field_0 = 280;
    field_136[1].field_B8A[4].field_2 = 358;
    field_136[1].field_BC6_nifty_idx = 3;
    field_136[1].field_BC8 = 3;
    field_136[1].field_2 = 10;
    field_136[1].field_518[0].field_0 = 3;
    field_136[1].field_518[0].field_2_xpos = 420;
    field_136[1].field_518[0].field_4_ypos = 310;
    field_136[1].field_518[0].field_6_wstr_buf[0] = 0;
    field_136[1].field_518[1].field_0 = 3;
    field_136[1].field_518[1].field_2_xpos = 420;
    field_136[1].field_518[1].field_4_ypos = 390;
    field_136[1].field_518[1].field_6_wstr_buf[0] = 0;
    field_136[1].field_518[2].field_0 = 1;
    field_136[1].field_518[2].field_2_xpos = 410;
    field_136[1].field_518[2].field_4_ypos = 298;
    wcsncpy(field_136[1].field_518[2].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("car_dam"), 0x32u);
    field_136[1].field_518[2].field_6A = word_703C3C;
    field_136[1].field_518[3].field_0 = 1;
    field_136[1].field_518[3].field_2_xpos = 410;
    field_136[1].field_518[3].field_4_ypos = 378;
    wcsncpy(field_136[1].field_518[3].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("car_dam"), 0x32u);
    field_136[1].field_518[3].field_6A = word_703C3C;
    field_136[1].field_518[4].field_2_xpos = 380;
    field_136[1].field_518[4].field_4_ypos = 310;
    field_136[1].field_518[5].field_2_xpos = 460;
    field_136[1].field_518[5].field_4_ypos = 310;
    field_136[1].field_518[6].field_2_xpos = 380;
    field_136[1].field_518[7].field_2_xpos = 460;
    field_136[1].field_518[4].field_0 = 3;
    field_136[1].field_518[4].field_6_wstr_buf[0] = 3;
    field_136[1].field_518[5].field_0 = 3;
    field_136[1].field_518[5].field_6_wstr_buf[0] = 4;
    field_136[1].field_518[6].field_0 = 3;
    field_136[1].field_518[6].field_4_ypos = 390;
    field_136[1].field_518[6].field_6_wstr_buf[0] = 3;
    field_136[1].field_518[7].field_0 = 3;
    field_136[1].field_518[7].field_4_ypos = 390;
    field_136[1].field_518[7].field_6_wstr_buf[0] = 4;
    field_136[1].field_518[8].field_0 = 3;
    field_136[1].field_518[8].field_2_xpos = 290;
    field_136[1].field_518[8].field_4_ypos = 222;
    field_136[1].field_518[8].field_6_wstr_buf[0] = 3;
    field_136[1].field_518[9].field_0 = 3;
    field_136[1].field_518[9].field_2_xpos = 580;
    field_136[1].field_518[9].field_4_ypos = 222;
    field_136[1].field_518[9].field_6_wstr_buf[0] = 4;
    field_136[11].field_0 = 3;
    field_136[11].field_2 = 1;
    field_136[11].field_518[0].field_0 = 1;
    field_136[11].field_518[0].field_2_xpos = 35;
    field_136[11].field_518[0].field_4_ypos = 11;
    wcscpy(field_136[11].field_518[0].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("plr_qut"));
    field_136[11].field_518[0].field_6A = field_130;
    field_136[11].field_518[0].field_6C = 5;
    field_136[11].field_4[0].field_0 = 1;
    field_136[11].field_4[0].field_4 = 392;
    wcsncpy(field_136[11].field_4[0].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("savepos"), 0x32u);
    field_136[11].field_4[0].field_2 =
        Frontend::sub_4B0190(field_136[11].field_4[0].field_6_wstr_buf, field_136[11].field_4[0].field_6A, 320);
    field_136[11].field_4[0].field_80 = 260;
    field_136[11].field_4[1].field_0 = 1;
    field_136[11].field_4[1].field_4 = 412;
    wcsncpy(field_136[11].field_4[1].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("replay"), 0x32u);
    field_136[11].field_4[1].field_2 =
        Frontend::sub_4B0190(field_136[11].field_4[1].field_6_wstr_buf, field_136[11].field_4[1].field_6A, 320);
    field_136[11].field_4[1].field_80 = 259;
    field_136[11].field_4[2].field_0 = 1;
    field_136[11].field_4[2].field_4 = 432;
    wcsncpy(field_136[11].field_4[2].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("mainmen"), 0x32u);
    field_136[11].field_4[2].field_2 =
        Frontend::sub_4B0190(field_136[11].field_4[2].field_6_wstr_buf, field_136[11].field_4[2].field_6A, 320);
    field_136[11].field_4[2].field_80 = 0;
    field_136[11].field_B8A[0].field_0 = 150;
    field_136[11].field_B8A[0].field_2 = 400;
    field_136[11].field_B8A[1].field_0 = 150;
    field_136[11].field_B8A[1].field_2 = 420;
    field_136[11].field_B8A[2].field_0 = 150;
    field_136[11].field_B8A[2].field_2 = 440;
    field_136[11].field_BC6_nifty_idx = 0;
    field_136[11].field_BC8 = 0;
    field_136[2].field_0 = 3;
    field_136[2].field_2 = 1;
    field_136[2].field_518[0].field_0 = 1;
    field_136[2].field_518[0].field_2_xpos = 35;
    field_136[2].field_518[0].field_4_ypos = 11;
    wcsncpy(field_136[2].field_518[0].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("plr_ded"), 0x32u);
    field_136[2].field_518[0].field_6A = field_130;
    field_136[2].field_518[0].field_6C = 0;
    field_136[2].field_4[0].field_0 = 1;
    field_136[2].field_4[0].field_4 = 392;
    wcsncpy(field_136[2].field_4[0].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("savepos"), 0x32u);
    field_136[2].field_4[0].field_2 = Frontend::sub_4B0190(field_136[2].field_4[0].field_6_wstr_buf, field_136[2].field_4[0].field_6A, 320);
    field_136[2].field_4[0].field_80 = 260;
    field_136[2].field_4[1].field_0 = 1;
    field_136[2].field_4[1].field_4 = 412;
    wcsncpy(field_136[2].field_4[1].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("replay"), 0x32u);
    field_136[2].field_4[1].field_2 = Frontend::sub_4B0190(field_136[2].field_4[1].field_6_wstr_buf, field_136[2].field_4[1].field_6A, 320);
    field_136[2].field_4[1].field_80 = 259;
    field_136[2].field_4[2].field_0 = 1;
    field_136[2].field_4[2].field_4 = 432;
    wcsncpy(field_136[2].field_4[2].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("mainmen"), 0x32u);
    field_136[2].field_4[2].field_2 = Frontend::sub_4B0190(field_136[2].field_4[2].field_6_wstr_buf, field_136[2].field_4[2].field_6A, 320);
    field_136[2].field_4[2].field_80 = 0;
    field_136[2].field_B8A[0].field_0 = 150;
    field_136[2].field_B8A[0].field_2 = 400;
    field_136[2].field_B8A[1].field_0 = 150;
    field_136[2].field_B8A[1].field_2 = 420;
    field_136[2].field_B8A[2].field_0 = 150;
    field_136[2].field_B8A[2].field_2 = 440;
    field_136[2].field_BC6_nifty_idx = 0;
    field_136[2].field_BC8 = 0;
    field_136[3].field_0 = 5;
    field_136[3].field_2 = 1;
    field_136[3].field_518[0].field_0 = 1;
    field_136[3].field_518[0].field_2_xpos = 35;
    field_136[3].field_518[0].field_4_ypos = 11;
    wcsncpy(field_136[3].field_518[0].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("cmpltd"), 0x32u);
    field_136[3].field_518[0].field_6A = field_12C;
    field_136[3].field_4[0].field_0 = 1;
    field_136[3].field_4[0].field_4 = 365;
    wcsncpy(field_136[3].field_4[0].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("nxt_lvl"), 0x32u);
    field_136[3].field_4[0].field_2 = Frontend::sub_4B0190(field_136[3].field_4[0].field_6_wstr_buf, field_136[3].field_4[0].field_6A, 320);
    field_136[3].field_4[0].field_80 = 261;
    field_136[3].field_4[1].field_0 = 1;
    field_136[3].field_4[1].field_4 = 385;
    wcsncpy(field_136[3].field_4[1].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("savepos"), 0x32u);
    field_136[3].field_4[1].field_2 = Frontend::sub_4B0190(field_136[3].field_4[1].field_6_wstr_buf, field_136[3].field_4[1].field_6A, 320);
    field_136[3].field_4[1].field_80 = 260;
    field_136[3].field_4[2].field_0 = 1;
    field_136[3].field_4[2].field_4 = 405;
    wcsncpy(field_136[3].field_4[2].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("replay"), 0x32u);
    field_136[3].field_4[2].field_2 = Frontend::sub_4B0190(field_136[3].field_4[2].field_6_wstr_buf, field_136[3].field_4[2].field_6A, 320);
    field_136[3].field_4[2].field_80 = 259;
    field_136[3].field_4[3].field_0 = 1;
    field_136[3].field_4[3].field_4 = 425;
    wcsncpy(field_136[3].field_4[3].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("contnue"), 0x32u);
    field_136[3].field_4[3].field_2 = Frontend::sub_4B0190(field_136[3].field_4[3].field_6_wstr_buf, field_136[3].field_4[3].field_6A, 320);
    field_136[3].field_4[3].field_80 = 266;
    field_136[3].field_4[4].field_0 = 1;
    field_136[3].field_4[4].field_4 = 445;
    wcsncpy(field_136[3].field_4[4].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("mainmen"), 0x32u);
    field_136[3].field_4[4].field_2 = Frontend::sub_4B0190(field_136[3].field_4[4].field_6_wstr_buf, field_136[3].field_4[4].field_6A, 320);
    field_136[3].field_4[4].field_80 = 0;
    field_136[3].field_B8A[0].field_0 = 150;
    field_136[3].field_B8A[0].field_2 = 373;
    field_136[3].field_B8A[1].field_0 = 150;
    field_136[3].field_B8A[1].field_2 = 393;
    field_136[3].field_B8A[2].field_0 = 150;
    field_136[3].field_B8A[2].field_2 = 413;
    field_136[3].field_B8A[3].field_0 = 150;
    field_136[3].field_B8A[3].field_2 = 433;
    field_136[3].field_B8A[4].field_0 = 150;
    field_136[3].field_B8A[4].field_2 = 453;
    field_136[3].field_BC6_nifty_idx = 0;
    field_136[3].field_BC8 = 0;
    field_136[4].field_0 = 1;
    field_136[4].field_2 = 1;
    field_136[4].field_518[0].field_0 = 1;
    field_136[4].field_518[0].field_4_ypos = 230;
    wcsncpy(field_136[4].field_518[0].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("gam_cmp"), 0x32u);
    v30 = field_130;
    field_136[4].field_518[0].field_6A = v30;
    field_136[4].field_518[0].field_2_xpos = Frontend::sub_4B0190(field_136[4].field_518[0].field_6_wstr_buf, v30, 320);
    field_136[4].field_518[0].field_6C = 4;
    field_136[4].field_4[0].field_0 = 1;
    field_136[4].field_4[0].field_2 = 180;
    field_136[4].field_4[0].field_4 = 410;
    wcsncpy(field_136[4].field_4[0].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("mainmen"), 0x32u);
    field_136[4].field_4[0].field_80 = 0;
    field_136[4].field_B8A[0].field_0 = 160;
    field_136[4].field_B8A[0].field_2 = 418;
    field_136[4].field_BC6_nifty_idx = 0;
    field_136[4].field_BC8 = 0;
    field_136[5].field_0 = 1;
    field_136[5].field_2 = 5;
    field_136[5].field_4[0].field_0 = 2;
    field_136[5].field_4[0].field_2 = 300;
    field_136[5].field_4[0].field_4 = 155;
    field_136[5].field_4[0].field_6E_count = 0;
    field_136[5].field_4[0].field_70 = 0;
    field_136[5].field_4[0].field_7E = 11;

    u16 v323 = 0;
    do
    {
        field_136[5].field_4[0].field_72[v323++] = 1;
    } while (v323 <= field_136[5].field_4[0].field_7E);

    field_136[5].field_B8A[0].field_0 = 280;
    field_136[5].field_B8A[0].field_2 = 163;
    field_136[5].field_BC6_nifty_idx = 0;
    field_136[5].field_BC8 = 0;
    field_136[5].field_518[0].field_0 = 3;
    field_136[5].field_518[0].field_2_xpos = 450;
    field_136[5].field_518[0].field_4_ypos = 197;
    field_136[5].field_518[0].field_6_wstr_buf[0] = 0;
    field_136[5].field_518[1].field_0 = 1;
    field_136[5].field_518[1].field_2_xpos = 440;
    field_136[5].field_518[1].field_4_ypos = 185;
    //    v34 = ;
    field_136[5].field_518[2].field_4_ypos = 197;
    field_136[5].field_518[3].field_4_ypos = 197;
    field_136[5].field_518[1].field_6A = word_703C3C; // v34
    field_136[5].field_518[2].field_0 = 3;
    field_136[5].field_518[2].field_2_xpos = 410;
    field_136[5].field_518[2].field_6_wstr_buf[0] = 3;
    field_136[5].field_518[3].field_0 = 3;
    field_136[5].field_518[3].field_2_xpos = 490;
    field_136[5].field_518[3].field_6_wstr_buf[0] = 4;
    field_136[5].field_518[4].field_0 = 1;
    field_136[5].field_518[4].field_2_xpos = 340;
    field_136[5].field_518[4].field_4_ypos = v2;
    wcsncpy(field_136[5].field_518[4].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("hi_scre"), 0x32u);
    field_136[5].field_518[4].field_6A = field_126;
    field_136[6].field_0 = 3;
    field_136[6].field_2 = 3;
    field_136[6].field_518[0].field_0 = 1;
    field_136[6].field_518[0].field_2_xpos = 35;
    field_136[6].field_518[0].field_4_ypos = 11;
    wcsncpy(field_136[6].field_518[0].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("bonslev"), 0x32u);
    field_136[6].field_518[0].field_6A = field_130;
    field_136[6].field_518[0].field_6C = 5;
    field_136[6].field_518[1].field_0 = 1;
    field_136[6].field_518[1].field_2_xpos = 170;
    field_136[6].field_518[1].field_4_ypos = 250;
    wcsncpy(field_136[6].field_518[1].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("score"), 0x32u);
    field_136[6].field_518[2].field_0 = 1;
    field_136[6].field_518[2].field_2_xpos = 400;
    field_136[6].field_518[2].field_4_ypos = 250;
    s16 v38 = field_120;
    field_136[6].field_4[0].field_0 = 1;
    field_136[6].field_518[2].field_6A = v38;
    field_136[6].field_4[0].field_4 = 340;
    wcsncpy(field_136[6].field_4[0].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("repbons"), 0x32u);
    field_136[6].field_4[0].field_2 = Frontend::sub_4B0190(field_136[6].field_4[0].field_6_wstr_buf, v38, 320);
    field_136[6].field_4[0].field_80 = 259;
    field_136[6].field_4[1].field_0 = 1;
    field_136[6].field_4[1].field_4 = 360;
    wcsncpy(field_136[6].field_4[1].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("nxt_lvl"), 0x32u);
    field_136[6].field_4[1].field_2 = Frontend::sub_4B0190(field_136[6].field_4[1].field_6_wstr_buf, field_136[6].field_4[1].field_6A, 320);
    field_136[6].field_4[1].field_80 = 261;
    field_136[6].field_4[2].field_0 = 1;
    field_136[6].field_4[2].field_4 = 380;
    wcsncpy(field_136[6].field_4[2].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("mainmen"), 0x32u);
    field_136[6].field_4[2].field_2 = Frontend::sub_4B0190(field_136[6].field_4[2].field_6_wstr_buf, field_136[6].field_4[2].field_6A, 320);
    field_136[6].field_4[2].field_80 = 0;
    field_136[6].field_B8A[0].field_0 = 150;
    field_136[6].field_B8A[0].field_2 = 348;
    field_136[6].field_B8A[1].field_0 = 150;
    field_136[6].field_B8A[1].field_2 = 368;
    field_136[6].field_B8A[2].field_0 = 150;
    field_136[6].field_B8A[2].field_2 = 388;
    field_136[6].field_BC6_nifty_idx = 0;
    field_136[6].field_BC8 = 0;
    field_136[7].field_0 = 1;
    field_136[7].field_2 = 14;
    field_136[7].field_518[0].field_0 = 1;
    field_136[7].field_518[0].field_2_xpos = 35;
    field_136[7].field_518[0].field_4_ypos = 11;
    field_136[7].field_518[0].field_6A = field_130;
    field_136[7].field_518[0].field_6C = 5;
    field_136[7].field_518[1].field_0 = 1;
    field_136[7].field_518[1].field_2_xpos = 100;
    field_136[7].field_518[1].field_4_ypos = 170;
    wcsncpy(field_136[7].field_518[1].field_6_wstr_buf, word_67DC8C, 50u);
    field_136[7].field_518[2].field_0 = 1;
    field_136[7].field_518[2].field_2_xpos = 100;
    field_136[7].field_518[2].field_4_ypos = 190;
    wcsncpy(field_136[7].field_518[2].field_6_wstr_buf, word_67DC8C, 0x32u);
    field_136[7].field_518[3].field_0 = 1;
    field_136[7].field_518[3].field_2_xpos = 100;
    field_136[7].field_518[3].field_4_ypos = 210;
    wcsncpy(field_136[7].field_518[3].field_6_wstr_buf, word_67DC8C, 0x32u);
    field_136[7].field_518[4].field_0 = 1;
    field_136[7].field_518[4].field_2_xpos = 100;
    field_136[7].field_518[4].field_4_ypos = 230;
    wcsncpy(field_136[7].field_518[4].field_6_wstr_buf, word_67DC8C, 0x32u);
    field_136[7].field_518[5].field_0 = 1;
    field_136[7].field_518[5].field_2_xpos = 100;
    field_136[7].field_518[5].field_4_ypos = 250;
    wcsncpy(field_136[7].field_518[5].field_6_wstr_buf, word_67DC8C, 0x32u);
    field_136[7].field_518[6].field_0 = 1;
    field_136[7].field_518[6].field_2_xpos = 100;
    field_136[7].field_518[6].field_4_ypos = 270;
    wcsncpy(field_136[7].field_518[6].field_6_wstr_buf, word_67DC8C, 0x32u);
    field_136[7].field_518[7].field_0 = 1;
    field_136[7].field_518[7].field_4_ypos = 300;
    wcsncpy(field_136[7].field_518[7].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("kills_h"), 0x32u);
    field_136[7].field_518[7].field_2_xpos =
        sub_4B0190(field_136[7].field_518[7].field_6_wstr_buf, field_136[7].field_518[7].field_6A, 320);
    field_136[7].field_518[8].field_0 = 1;
    field_136[7].field_518[8].field_2_xpos = 100;
    field_136[7].field_518[8].field_4_ypos = 320;
    field_136[7].field_518[9].field_0 = 1;
    field_136[7].field_518[9].field_2_xpos = 100;
    field_136[7].field_518[9].field_4_ypos = 340;
    field_136[7].field_518[10].field_0 = 1;
    field_136[7].field_518[10].field_2_xpos = 100;
    field_136[7].field_518[10].field_4_ypos = 360;
    field_136[7].field_518[11].field_0 = 1;
    field_136[7].field_518[11].field_2_xpos = 100;
    field_136[7].field_518[11].field_4_ypos = 380;
    field_136[7].field_518[12].field_0 = 1;
    field_136[7].field_518[12].field_2_xpos = 100;
    field_136[7].field_518[12].field_4_ypos = 400;
    field_136[7].field_518[13].field_0 = 1;
    field_136[7].field_518[13].field_2_xpos = 30;
    field_136[7].field_518[13].field_4_ypos = 150;
    field_136[7].field_4[0].field_0 = 1;
    field_136[7].field_4[0].field_4 = 430;
    wcsncpy(field_136[7].field_4[0].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("quit"), 0x32u);
    field_136[7].field_4[0].field_2 = sub_4B0190(field_136[7].field_4[0].field_6_wstr_buf, field_136[7].field_4[0].field_6A, 320);
    field_136[7].field_4[0].field_80 = 258;
    field_136[7].field_B8A[0].field_0 = 180;
    field_136[7].field_B8A[0].field_2 = 438;
    field_136[7].field_BC6_nifty_idx = 0;
    field_136[7].field_BC8 = 0;
    field_136[8].field_0 = 1;
    field_136[8].field_2 = 0;
    field_136[8].field_4[0].field_0 = 1;
    field_136[8].field_4[0].field_2 = 200;
    field_136[8].field_4[0].field_4 = 280;
    wcsncpy(field_136[8].field_4[0].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("mainmen"), 0x32u);
    field_136[8].field_4[0].field_80 = 0;
    field_136[8].field_B8A[0].field_0 = 180;
    field_136[8].field_B8A[0].field_2 = 288;
    field_136[8].field_BC6_nifty_idx = 0;
    field_136[8].field_BC8 = 0;
    field_136[10].field_0 = 1;
    field_136[10].field_2 = 1;
    field_136[10].field_518[0].field_0 = 1;
    field_136[10].field_518[0].field_4_ypos = 230;
    wcsncpy(field_136[10].field_518[0].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("nicetry"), 0x32u);
    //    v46 = field_130;
    field_136[10].field_518[0].field_6A = field_130; // v46;
    field_136[10].field_518[0].field_2_xpos = sub_4B0190(field_136[10].field_518[0].field_6_wstr_buf,
                                                         field_130, //v46,
                                                         320);
    field_136[10].field_518[0].field_6C = 4;
    field_136[10].field_4[0].field_0 = 1;
    field_136[10].field_4[0].field_2 = 180;
    field_136[10].field_4[0].field_4 = 410;
    wcsncpy(field_136[10].field_4[0].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("mainmen"), 0x32u);
    field_136[10].field_4[0].field_80 = 0;
    field_136[10].field_B8A[0].field_0 = 160;
    field_136[10].field_B8A[0].field_2 = 418;
    field_136[10].field_BC6_nifty_idx = 0;
    field_136[10].field_BC8 = 0;
    field_136[14].field_0 = 1;
    field_136[14].field_2 = 5;
    field_136[14].field_4[0].field_0 = 1;
    field_136[14].field_4[0].field_2 = 170;
    field_136[14].field_4[0].field_4 = 340;
    field_136[14].field_4[0].field_80 = 268;
    field_136[14].field_518[0].field_0 = 1;
    field_136[14].field_518[0].field_2_xpos = 20;
    field_136[14].field_518[0].field_4_ypos = 160;
    wcsncpy(field_136[14].field_518[0].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("fr_ent1"), 0x32u);
    field_136[14].field_518[1].field_0 = 1;
    field_136[14].field_518[1].field_2_xpos = 20;
    field_136[14].field_518[1].field_4_ypos = 180;
    wcsncpy(field_136[14].field_518[1].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("fr_ent2"), 0x32u);
    field_136[14].field_518[2].field_0 = 1;
    field_136[14].field_518[2].field_2_xpos = 20;
    field_136[14].field_518[2].field_4_ypos = 200;
    wcsncpy(field_136[14].field_518[2].field_6_wstr_buf, word_67DC8C, 0x32u);
    field_136[14].field_518[3].field_0 = 1;
    field_136[14].field_518[3].field_2_xpos = 20;
    field_136[14].field_518[3].field_4_ypos = 300;
    wcsncpy(field_136[14].field_518[3].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("fr_pmpt"), 0x32u);
    field_136[14].field_518[4].field_0 = 1;
    field_136[14].field_518[4].field_2_xpos = 20;
    field_136[14].field_518[4].field_4_ypos = 320;
    wcsncpy(field_136[14].field_518[4].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("score"), 0x32u);
    field_136[14].field_B8A[0].field_0 = 150;
    field_136[14].field_B8A[0].field_4 = 348;
    field_136[14].field_BC6_nifty_idx = 0;
    field_136[14].field_BC8 = 0;
    field_EE0E_unk.sub_483F20();
}

STUB_FUNC(0x4B4440)
void Frontend::sub_4B4440()
{
    // todo
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
            field_EDE8_plySlots[i].field_0 = 0;
            field_EDE8_plySlots[i].field_1 = 3;
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
u8 Frontend::sub_4B77B0(dreamy_clarke_0xA4* a2)
{
    u8 result;

    for (result = this->field_1EB50_idx - 1; !a2->field_0[result][0].field_0; --result)
    {
        if (result <= 0)
        {
            break;
        }
    }
    return result;
}

EXPORT_VAR extern BYTE byte_6F4BF4;
EXPORT_VAR extern bool bDoFrontEnd_626B68;

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
        byte_6F4BF4 = *a1;
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
            s32 eight = 8;
            DrawText_5D8A10(chr, text_xbase + v16, text_ypos, arg_C, a6, &eight, a2, false, false);
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

    swprintf(tmpBuff_67BD9C, L"%d", *(u32*)&gJolly_poitras_0x2BC0_6FEAC0->field_1800[v39].field_0[20]);
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

    swprintf(tmpBuff_67BD9C, L"%d", *(u32*)&gJolly_poitras_0x2BC0_6FEAC0->field_1800[v39].field_0[24]);
    Frontend::sub_4B78B0(tmpBuff_67BD9C, x_pos_best, y_pos, font_type, 10, 1, v4, 1);

    //  civilians murdered

    x_pos = a3;
    y_pos = a5 + 60;
    DrawText_4B87A0(gText_0x14_704DFC->Find_5B5F90("murder"), x_pos, y_pos, font_type, 1);

    swprintf(tmpBuff_67BD9C, L"%d", gLucid_hamilton_67E8E0.sub_4C59F0(7u));
    Frontend::sub_4B78B0(tmpBuff_67BD9C, x_pos_last, y_pos, font_type, 10, 1, v4, 1);

    swprintf(tmpBuff_67BD9C, L"%d", *(u32*)&gJolly_poitras_0x2BC0_6FEAC0->field_1800[v39].field_0[28]);
    Frontend::sub_4B78B0(tmpBuff_67BD9C, x_pos_best, y_pos, font_type, 10, 1, v4, 1);

    //  lawmen killed

    if (!bIsFrench_67D53C)
    {
        x_pos = a3;
        y_pos = a5 + 80;
        DrawText_4B87A0(gText_0x14_704DFC->Find_5B5F90("cop_kl"), x_pos, y_pos, font_type, 1);

        swprintf(tmpBuff_67BD9C, L"%d", gLucid_hamilton_67E8E0.sub_4C59F0(8u));
        Frontend::sub_4B78B0(tmpBuff_67BD9C, x_pos_last, y_pos, font_type, 10, 1, v4, 1);

        swprintf(tmpBuff_67BD9C, L"%d", *(u32*)&gJolly_poitras_0x2BC0_6FEAC0->field_1800[v39].field_0[32]);
        Frontend::sub_4B78B0(tmpBuff_67BD9C, x_pos_best, y_pos, font_type, 10, 1, v4, 1);
    }

    //  gang members killed

    x_pos = a3;
    y_pos = a5 + 100;
    DrawText_4B87A0(gText_0x14_704DFC->Find_5B5F90("gng_kl"), x_pos, y_pos, font_type, 1);

    swprintf(tmpBuff_67BD9C, L"%d", gLucid_hamilton_67E8E0.sub_4C59F0(9u));
    Frontend::sub_4B78B0(tmpBuff_67BD9C, x_pos_last, y_pos, font_type, 10, 1, v4, 1);

    swprintf(tmpBuff_67BD9C, L"%d", *(u32*)&gJolly_poitras_0x2BC0_6FEAC0->field_1800[v39].field_0[36]);
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

STUB_FUNC(0x4B7060)
u8 Frontend::sub_4B7060(u8 a2)
{
    dreamy_clarke_0xA4* v2; // edx
    u8 result; // al

    v2 = sub_4B43E0();
    result = a2;
    if (a2)
    {
        return a2 - 1;
    }

    if (byte_67DA80)
    {
        result = 2;
        if (!v2->field_0[2][0].field_0)
        {
            do
            {
                --result;
            } while (!v2->field_0[result][0].field_0);
        }
    }
    return result;
}

STUB_FUNC(0x4B7270)
u8 Frontend::sub_4B7270(char_type a2)
{
    dreamy_clarke_0xA4* v2; // esi
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
        while (!v2->field_0[result][0].field_0)
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

STUB_FUNC(0x4B7550)
void Frontend::sub_4B7550()
{
    loving_borg_0xBCA* pBorg; // edi
    u8 v3; // [esp+Ch] [ebp-4h]

    pBorg = &field_136[field_132_f136_idx];
    v3 = gLucid_hamilton_67E8E0.sub_4C5980();
    swprintf(tmpBuff_67BD9C, L"%d", v3 + 1);
    wcsncpy(pBorg->field_518[2].field_6_wstr_buf, tmpBuff_67BD9C, 0x32u);

    if (sub_4B7490())
    {
        pBorg->field_518[4].field_1 = 1;
        field_1EB4C = 1;
    }
    else
    {
        pBorg->field_518[4].field_1 = 0;
        field_1EB4C = 0;
    }

    if (sub_4B74C0())
    {
        pBorg->field_518[5].field_1 = 1;
        field_1EB4D = 1;
    }
    else
    {
        pBorg->field_518[5].field_1 = 0;
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

STUB_FUNC(0x4B42B0)
void Frontend::sub_4B42B0()
{
    s32 v1; // eax
    wchar_t* v2; // edi
    u32 v3; // ecx
    wchar_t* v4; // edi
    s32 i; // ecx

    if ((unsigned __int8)field_C9B2 < 9u)
    {
        v1 = (unsigned __int8)field_C9B2;
        v2 = &field_C9A0[v1];
        v3 = (u32)(9 - v1) >> 1;
        memset(v2, 0, 4 * v3);
        v4 = &v2[2 * v3];
        for (i = (9 - (BYTE)v1) & 1; i; --i)
        {
            *v4++ = 0;
        }
    }
}

STUB_FUNC(0x4B7120)
char_type Frontend::sub_4B7120(char_type a2)
{
    // todo
    return 0;
}

STUB_FUNC(0x4B7610)
void Frontend::sub_4B7610()
{
    loving_borg_0xBCA* pItem; // esi
    u8 v3; // al
    u8 v4; // [esp+8h] [ebp-8h]
    u8 v5; // [esp+Ch] [ebp-4h]

    pItem = &field_136[field_132_f136_idx];
    v3 = gLucid_hamilton_67E8E0.sub_4C5990();
    v4 = v3 >> 4;
    v5 = v3 & 0xF;
    if (v3 == 0xFF)
    {
        pItem->field_4[4].field_1 = 0;
        pItem->field_B8A[4].field_4 = 0;
        pItem->field_518[3].field_1 = 0;
        pItem->field_518[1].field_1 = 0;
        pItem->field_518[6].field_1 = 0;
        pItem->field_518[7].field_1 = 0;
    }
    else
    {
        pItem->field_4[4].field_1 = 1;
        pItem->field_B8A[4].field_4 = 1;
        pItem->field_518[3].field_1 = 1;
        pItem->field_518[1].field_1 = 1;
        pItem->field_518[6].field_1 = 1;
        pItem->field_518[7].field_1 = 1;
        if (sub_4B74F0())
        {
            pItem->field_518[6].field_1 = 1;
            field_1EB4E = 1;
        }
        else
        {
            pItem->field_518[6].field_1 = 0;
            field_1EB4E = 0;
        }

        if (sub_4B7520())
        {
            pItem->field_518[7].field_1 = 1;
            field_1EB4F = 1;
        }
        else
        {
            pItem->field_518[7].field_1 = 0;
            field_1EB4F = 0;
        }

        swprintf(word_67C7D8, L"%c", 3 * v4 + v5 + 64);
        wcsncpy(pItem->field_518[3].field_6_wstr_buf, word_67C7D8, 0x32u);
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
    // todo
}

MATCH_FUNC(0x4B6070)
loving_borg_0xBCA::loving_borg_0xBCA()
{
    field_0 = 0;
    field_2 = 0;
    field_BC6_nifty_idx = 0;
    field_BC8 = 0;
}

MATCH_FUNC(0x4B6110)
loving_borg_0xBCA::~loving_borg_0xBCA()
{
    field_0 = 0;
    field_2 = 0;
    field_BC6_nifty_idx = 0;
    field_BC8 = 0;
}

MATCH_FUNC(0x4B61B0)
bool loving_borg_0xBCA::sub_4B61B0()
{
    u16 oldIdx = field_BC6_nifty_idx;
    do
    {
        if (!field_BC6_nifty_idx)
        {
            field_BC6_nifty_idx = field_0 - 1;
        }
        else
        {
            field_BC6_nifty_idx--;
        }
    } while (!field_B8A[field_BC6_nifty_idx].field_4);
    return oldIdx != field_BC6_nifty_idx ? true : false;
}

MATCH_FUNC(0x4B6200)
bool loving_borg_0xBCA::sub_4B6200()
{
    u16 oldIdx = field_BC6_nifty_idx;
    do
    {
        if (field_BC6_nifty_idx == field_0 - 1)
        {
            field_BC6_nifty_idx = 0;
        }
        else
        {
            field_BC6_nifty_idx++;
        }
    } while (!field_B8A[field_BC6_nifty_idx].field_4);
    return oldIdx != field_BC6_nifty_idx ? true : false;
}

MATCH_FUNC(0x4B63E0)
competent_noyce_0x6E::competent_noyce_0x6E()
{
    field_0 = 0;
    field_2_xpos = 0;
    field_4_ypos = 0;
    field_1 = 1;
    wcscpy(field_6_wstr_buf, word_67DC8C);
    field_6A = -1;
    field_6C = -1;
}

MATCH_FUNC(0x4B6420)
competent_noyce_0x6E::~competent_noyce_0x6E()
{
    field_1 = 1;
    field_0 = 0;
    field_2_xpos = 0;
    field_4_ypos = 0;
    field_6A = -1;
    field_6C = -1;
}

MATCH_FUNC(0x4B6290)
nifty_maxwell_0x82::nifty_maxwell_0x82()
{
    field_6A = -1;
    field_6C = -1;
    field_0 = 0;
    field_1 = 1;
    field_2 = 0;
    field_4 = 0;
    field_6E_count = 0;
    field_70 = 0;

    for (s32 i = 0; i < GTA2_COUNTOF(field_72); i++) // or is it a u32 ??
    {
        field_72[i] = 0;
    }

    wcscpy(field_6_wstr_buf, word_67DC8C);
    field_7E = 0;
    field_80 = 0;
}

MATCH_FUNC(0x4B62F0)
nifty_maxwell_0x82::~nifty_maxwell_0x82()
{
    field_0 = 0;
    field_1 = 1;
    field_2 = 0;
    field_4 = 0;
    field_6A = -1;
    field_6C = -1;
    field_6E_count = 0;
    field_70 = 0;
    field_7E = 0;
    field_80 = 0;
}

STUB_FUNC(0x4B6330)
bool nifty_maxwell_0x82::sub_4B6330()
{
    BYTE tmp = byte_67DA80;
    u16 old_count = field_6E_count;
    u16 new_count = old_count;
    char_type bFound = 0;
    do
    {
        new_count++;
        if (new_count > field_7E)
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

    field_6E_count = new_count;

    return old_count != new_count ? true : false;
}

STUB_FUNC(0x4B6390)
bool nifty_maxwell_0x82::sub_4B6390()
{
    u16 oldCount = field_6E_count;
    u16 new_count = oldCount;
    char_type bFound = 0;
    do
    {

        if (new_count == 0)
        {
            if (byte_67DA80)
            {
                new_count = field_7E;
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

    } while (new_count != field_6E_count && !bFound); // 6E not reloaded

    field_6E_count = new_count;

    return oldCount != new_count ? true : false;
}

MATCH_FUNC(0x4B6260)
kind_beaver_6::kind_beaver_6()
{
    field_0 = 0;
    field_2 = 0;
    field_4 = 1;
}

MATCH_FUNC(0x4B6280)
kind_beaver_6::~kind_beaver_6()
{
    field_0 = 0;
    field_2 = 0;
    field_4 = 1;
}

MATCH_FUNC(0x4B6440)
admiring_euler_4::admiring_euler_4()
{
    field_0 = 0;
    field_1 = 0;
    field_2 = 0;
    field_3 = 0;
}

MATCH_FUNC(0x4B6450)
admiring_euler_4::~admiring_euler_4()
{
    field_0 = 0;
    field_1 = 0;
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

    field_0 = 1;
    field_1 = svg.field_4B;
    field_2 = svg.field_4C;
    field_3 = svg.field_4D;
}

struct QuadVerts // TODO: Same as Verts in gbh header (d3ddll)
{
    Vert field_0_verts[4];
};
EXPORT_VAR QuadVerts gQuadVerts_706B88;
GLOBAL(gQuadVerts_706B88, 0x706B88);

EXPORT_VAR extern u32 gLightingDrawFlag_7068F4;

MATCH_FUNC(0x5D83E0);
EXPORT s32 __stdcall CalcQuadFlags_5D83E0(s32 mode, u8 a2)
{
    switch (mode)
    {
        case 0:
            return gLightingDrawFlag_7068F4 | 0x80;
        case 1:
            gQuadVerts_706B88.field_0_verts[0].diff = (a2 << 27) | 0xFFFFFF;
            gQuadVerts_706B88.field_0_verts[1].diff = (a2 << 27) | 0xFFFFFF;
            gQuadVerts_706B88.field_0_verts[2].diff = (a2 << 27) | 0xFFFFFF;
            gQuadVerts_706B88.field_0_verts[3].diff = (a2 << 27) | 0xFFFFFF;
            return gLightingDrawFlag_7068F4 | 0x2180;
        case 2:
            gQuadVerts_706B88.field_0_verts[0].diff = (a2 << 27) | 0xFFFFFF;
            gQuadVerts_706B88.field_0_verts[1].diff = (a2 << 27) | 0xFFFFFF;
            gQuadVerts_706B88.field_0_verts[2].diff = (a2 << 27) | 0xFFFFFF;
            gQuadVerts_706B88.field_0_verts[3].diff = (a2 << 27) | 0xFFFFFF;
            return gLightingDrawFlag_7068F4 | 0x2280;
        default:
            return 0;
    }
}

EXPORT_VAR Fix16 dword_706A6C;
GLOBAL(dword_706A6C, 0x706A6C);

STUB_FUNC(0x5D8A10)
void __stdcall DrawText_5D8A10(const wchar_t* pText,
                               Fix16 xpos_fp,
                               Fix16 ypos_fp,
                               u16 font_type,
                               Fix16 scale_fp,
                               s32* pUnknown,
                               s32 unknown1, // seems to be related with palette
                               s32 unknown2, // alpha_value
                               s32 flags) // bool use_alpha
{

    s32 new_Flags = CalcQuadFlags_5D83E0(unknown2, flags) | 0x20000;

    Fix16 cur_xpos = xpos_fp; // note: new var

    Fix16 spaceWidth = scale_fp * gGtx_0x106C_703DD4->space_width_5AA7B0(&font_type);
    Fix16 lineHeight = scale_fp * gGtx_0x106C_703DD4->sub_5AA800(&font_type);

    unknown2 = unknown1;

    u32 kind = *pUnknown;
    if (scale_fp == dword_706A6C)
    {
        new_Flags = new_Flags | 0x10000;
    }

    if (font_type >= 101u)
    {
        if (kind == 8)
        {
            gMagical_germain_0x8EC_6F5168->sub_4D29D0(unknown1);
        }
        else
        {
            gMagical_germain_0x8EC_6F5168->sub_4D28A0(font_type);
        }
    }

    while (*pText != 0)
    {
        wchar_t text_char = *pText;

        // HIWORD(v16) =
        //HIWORD(v16) = HIWORD(pTextIter);

        // = LOWORD(v16)

        if (text_char == L'\n')
        {
            // reset xpos back to the start
            cur_xpos = xpos_fp;

            // move to the next line down
            ypos_fp += lineHeight;
        }
        else if (text_char == L' ')
        {
            // advance by size of space char
            cur_xpos += spaceWidth;
        }
        else if (text_char == L'#')
        {
            if (kind == *pUnknown && (WORD)unknown2 == (WORD)unknown1)
            {
                kind = 8;
                /*
                v17 = -((u16)font_type < 0x65u);
                // LOBYTE(v17) =
                v17 = v17 & 0xFB;
                v18 = v17 + 5;
                */
                unknown2 = font_type < 0x65u ? 10 : 5;
            }
            else
            {
                kind = *pUnknown;
                unknown2 = unknown1;
            }

            if ((u16)font_type >= 101u)
            {
                if (kind == 8)
                {
                    gMagical_germain_0x8EC_6F5168->sub_4D29D0(unknown2);
                }
                else
                {
                    gMagical_germain_0x8EC_6F5168->sub_4D28A0(font_type);
                }
            }
        }
        else
        {
            sprite_index* pSprIdx;
            STexture* pTextureToUse;
            if ((u16)font_type < 0x65u || (u16)font_type > 107u)
            {
                if ((u16)font_type < 0xC9u || (u16)font_type > 203u)
                {
                    // LOWORD(sprite_pal) =
                    u16 sprite_pal = gGtx_0x106C_703DD4->sub_5AA710(font_type, text_char - 33);
                    u16 sprt_idx = gGtx_0x106C_703DD4->convert_sprite_pal_5AA460(7, sprite_pal);
                    pSprIdx = gGtx_0x106C_703DD4->get_sprite_index_5AA440(sprt_idx);
                    pTextureToUse = gSharp_pare_0x15D8_705064->sub_5B94F0(7, sprite_pal, kind, unknown2);
                }
                else
                {
                    pSprIdx = gMagical_germain_0x8EC_6F5168->field_8E0_sprite_index;
                    pTextureToUse = gMagical_germain_0x8EC_6F5168->sub_4D27D0(text_char);
                }
            }
            else
            {
                pSprIdx = gMagical_germain_0x8EC_6F5168->field_8D4_sprite_index;
                pTextureToUse = gMagical_germain_0x8EC_6F5168->sub_4D2710(text_char);
            }

            STexture* pTexture = pTextureToUse;

            //v25 = v11.mValue;
            //v26 = v11.mValue >> 31;

            Fix16 sprite_xoff = Fix16(pSprIdx->field_4_width) * scale_fp;

            Fix16 sprite_yoff = Fix16(pSprIdx->field_5_height) * scale_fp;

            gQuadVerts_706B88.field_0_verts[0].x = cur_xpos.ToFloat();
            gQuadVerts_706B88.field_0_verts[0].y = ypos_fp.ToFloat();
            gQuadVerts_706B88.field_0_verts[0].z = 0.0001; // line 214

            f32 v_1_2_x = (sprite_xoff + cur_xpos).ToFloat();
            gQuadVerts_706B88.field_0_verts[1].x = v_1_2_x;
            gQuadVerts_706B88.field_0_verts[1].y = gQuadVerts_706B88.field_0_verts[0].y;
            gQuadVerts_706B88.field_0_verts[1].z = 0.0001;

            // f32 v1_u = (((f64)pSprIdx->field_4_width - 0.000099999997) * 16384.0);

            gQuadVerts_706B88.field_0_verts[2].x = v_1_2_x;
            gQuadVerts_706B88.field_0_verts[2].y = (ypos_fp + sprite_yoff).ToFloat();

            s32 v28 = sprite_xoff.mValue + cur_xpos.mValue;
            gQuadVerts_706B88.field_0_verts[2].z = 0.0001;

            gQuadVerts_706B88.field_0_verts[3].x = gQuadVerts_706B88.field_0_verts[0].x;
            gQuadVerts_706B88.field_0_verts[3].y = (ypos_fp + sprite_yoff).ToFloat();
            gQuadVerts_706B88.field_0_verts[3].z = 0.0001;

            Fix16 letterW((float)(pSprIdx->field_4_width - 0.0001));
            cur_xpos += letterW;
            Fix16 spriteH((float)(pSprIdx->field_5_height - 0.0001));

            gQuadVerts_706B88.field_0_verts[0].u = 0.0;
            gQuadVerts_706B88.field_0_verts[0].v = 0.0;

            gQuadVerts_706B88.field_0_verts[1].u = spriteH.ToFloat();
            gQuadVerts_706B88.field_0_verts[1].v = 0.0;

            gQuadVerts_706B88.field_0_verts[2].u = spriteH.ToFloat();
            gQuadVerts_706B88.field_0_verts[2].v = letterW.ToFloat();

            gQuadVerts_706B88.field_0_verts[3].u = 0.0;
            gQuadVerts_706B88.field_0_verts[3].v = letterW.ToFloat();

            pgbh_DrawQuad(new_Flags, pTexture, &gQuadVerts_706B88.field_0_verts[0], 255);
        }
        pText++;
    }
}

MATCH_FUNC(0x4B87A0)
EXPORT void __stdcall DrawText_4B87A0(const wchar_t* pBuffer, Fix16 xpos_fp, Fix16 ypos_fp, s16 fontType, Fix16 scale)
{
    s32 local; // [esp+0h] [ebp-4h] BYREF

    local = 2;
    DrawText_5D8A10(pBuffer, xpos_fp, ypos_fp, fontType, scale, &local, 0, 0, 0);
}