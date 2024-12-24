#include "laughing_blackwell_0x1EB54.hpp"
#include "Bink.hpp"
#include "Function.hpp"
#include "cSampleManager.hpp"
#include "debug.hpp"
#include "dma_video.hpp"
#include "error.hpp"
#include "file.hpp"
#include "fix16.hpp"
#include "gbh_graphics.hpp"
#include "gtx_0x106C.hpp"
#include "infallible_turing.hpp"
#include "Globals.hpp"
#include "jolly_poitras_0x2BC0.hpp"
#include "lucid_hamilton.hpp"
#include "magical_germain_0x8EC.hpp"
#include "registry.hpp"
#include "root_sound.hpp"
#include "sharp_pare_0x15D8.hpp"
#include "crt_stubs.hpp"
#include "text_0x14.hpp"
#include <io.h>
#include <stdio.h>
#include <wchar.h>

#pragma comment(lib, "dxguid.lib")

EXPORT void __stdcall DrawText_4B87A0(const wchar_t* pBuffer, Fix16 xpos_fp, Fix16 ypos_fp, s16 spaceWidth, Fix16 fp4);

void Start_GTA2Manager_5E4DE0();

EXPORT_VAR youthful_einstein gYouthful_einstein_6F8450;
GLOBAL(gYouthful_einstein_6F8450, 0x6F8450);

EXPORT_VAR laughing_blackwell_0x1EB54* gLaughing_blackwell_0x1EB54_67DC84;
GLOBAL(gLaughing_blackwell_0x1EB54_67DC84, 0x67DC84);

EXPORT_VAR u32 counter_706C4C;
GLOBAL(counter_706C4C, 0x706C4C);

EXPORT_VAR s32 dword_67D930;
GLOBAL(dword_67D930, 0x67D930);

u16 gTableSize_61FF20 = 25; // Note is constant but can't be marked const

EXPORT_VAR extern wchar_t word_67DC8C[50];

EXPORT_VAR DWORD dword_67D9FC;
GLOBAL(dword_67D9FC, 0x67D9FC);

// todo: likely an array
EXPORT_VAR short word_703C14; // = 7; // TODO: VAR HACK NOT CONST! Standalone hack ?? move hack to ctor ??
GLOBAL(word_703C14, 0x703C14);

EXPORT_VAR s16 word_703C3C; // = 8; // TODO: Ditto
GLOBAL(word_703C3C, 0x703C3C);

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

EXPORT_VAR extern LPDIRECTINPUTA gpDInput_67B804;

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

MATCH_FUNC(0x4ACFA0)
void __stdcall laughing_blackwell_0x1EB54::create_4ACFA0()
{
    if (!gLaughing_blackwell_0x1EB54_67DC84)
    {
        gLaughing_blackwell_0x1EB54_67DC84 = new laughing_blackwell_0x1EB54();
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
void __stdcall laughing_blackwell_0x1EB54::destroy_4AD070()
{
    if (!bSkip_audio_67D6BE && snd2_67D6F8.field_8)
    {
        gRoot_sound_66B038.FreeSoundEntry_40EFD0(snd2_67D6F8.field_8);
        snd2_67D6F8.field_8 = 0;
    }

    if (gLaughing_blackwell_0x1EB54_67DC84)
    {
        delete gLaughing_blackwell_0x1EB54_67DC84;
        gLaughing_blackwell_0x1EB54_67DC84 = 0;
    }

    Bink::Close1_513340();
    Bink::Close2_513390();
}

STUB_FUNC(0x4B3170)
void laughing_blackwell_0x1EB54::sub_4B3170(u16 arg0)
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
    s32 v16; // edx
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
    u16 field_132_f136_idx; // cx
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
    this->field_132_f136_idx = arg0;
    switch (arg0)
    {
        case 0xEu:
            this->field_110_state = 5;
            this->field_C9CA = 0;
            this->field_C9CB = 0;
            sub_4B8530();
            this->field_C9B3 = 1;
            this->field_C9B4 = 28;
            this->field_C9B6 = 5;
            goto LABEL_116;

        case 9u:
            this->field_1EB34 = 0x668000;
            this->field_1EB30 = 0;
            this->field_1EB38 = 0;
            this->field_C9B3 = 1;
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
                if (v4 < this->field_1EB51_blocks[a2])
                {
                    gJolly_poitras_0x2BC0_6FEAC0->sub_56BBD0(a2, a3);
                }
                ++v4;
            }
            if (a2 == (unsigned __int8)this->field_1EB50_idx - 1)
            {
                this->field_136[3].field_4[0].field_1 = 0;
                this->field_136[3].field_B8A[0].field_4 = 0;
            }
            else
            {
                gJolly_poitras_0x2BC0_6FEAC0->sub_56BBD0(a2 + 1, 0);
                this->field_136[3].field_4[0].field_1 = 1;
                this->field_136[3].field_B8A[0].field_4 = 1;
            }
            this->field_136[3].field_4[3].field_1 = 0;
            this->field_136[3].field_B8A[3].field_4 = 0;
            v5 = 1;
            v6 = &v57->field_0[a2][1];
            do
            {
                if (v6->field_0 && v5 < this->field_1EB51_blocks[a2])
                {
                    this->field_136[3].field_4[3].field_1 = 1;
                    this->field_136[3].field_B8A[3].field_4 = 1;
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
            wcsncpy(this->field_136[6].field_518[2].field_6_wstr_buf, tmpBuff_67BD9C, 0x32u);
            if (gLucid_hamilton_67E8E0.sub_4C5AE0() || v8 >= (unsigned __int8)this->field_1EB50_idx - 1 || !v57->field_0[v8 + 1][0].field_0)
            {
                this->field_136[6].field_4[1].field_1 = 0;
                this->field_136[6].field_B8A[1].field_4 = 0;
            }
            else
            {
                this->field_136[6].field_4[1].field_1 = 1;
                this->field_136[6].field_B8A[1].field_4 = 1;
            }
            goto LABEL_116;
        case 2u:
        case 0xBu:
            a2a = 0;
        LABEL_30:
            v9 = &(&this->field_0_pDInput)[gLucid_hamilton_67E8E0.sub_4C59B0()];
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
            v13 = &this->field_136[arg0];
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
            this->field_136[1].field_4[0].field_6E_count = playerSlotSetting;
            this->field_136[1].field_4[0].field_70 = playerSlotSetting;
            gLucid_hamilton_67E8E0.sub_4C5920(playerSlotSetting);
            sub_4B42E0();
            break;

        case 7u:
            a2b = gLucid_hamilton_67E8E0.sub_4C5BF0();
            v15 = -1;
            if (gLucid_hamilton_67E8E0.sub_4C5BC0() == 3)
            {
                v16 = 0;
                for (i = 4; i < 28; i += 4)
                {
                    if (*(s32*)((char_type*)&gYouthful_einstein_6F8450.field_0 + i) > v15 &&
                        !*((BYTE*)&gYouthful_einstein_6F8450.field_20 + v16))
                    {
                        v15 = *(s32*)((char_type*)&gYouthful_einstein_6F8450.field_0 + i);
                    }
                    ++v16;
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
                    wcsncpy(this->field_136[7].field_518[13].field_6_wstr_buf, _5B5F90, 0x32u);
                    v19 = sub_4B0190(this->field_136[7].field_518[13].field_6_wstr_buf, this->field_136[7].field_518[13].field_6A, 320);
                    break;

                case 2:
                    v20 = gText_0x14_704DFC->Find_5B5F90("pnts_h");
                    wcsncpy(this->field_136[7].field_518[13].field_6_wstr_buf, v20, 50u);
                    v19 = sub_4B0190(this->field_136[7].field_518[13].field_6_wstr_buf, this->field_136[7].field_518[13].field_6A, 320);
                    break;

                case 3:
                    v18 = gText_0x14_704DFC->Find_5B5F90("times_h");
                    wcsncpy(this->field_136[7].field_518[13].field_6_wstr_buf, v18, 0x32u);
                    v19 = sub_4B0190(this->field_136[7].field_518[13].field_6_wstr_buf, this->field_136[7].field_518[13].field_6A, 320);
                    break;
                default:
                    FatalError_4A38C0(189,
                                      "C:\\Splitting\\GTA2\\Source\\frontend2.cpp",
                                      4079); // Multiplayer game type should be frag, tag or score (but isn't)
            }

            this->field_136[7].field_518[13].field_2 = v19;
            v22 = 0;
            v23 = &this->field_136[7].field_518[7].field_1;
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
                v58 = &gYouthful_einstein_6F8450.field_20;
                field_6_wstr_buf = this->field_136[7].field_518[1].field_6_wstr_buf;
                do
                {
                    if (*(BYTE*)v58)
                    {
                        v48 = gText_0x14_704DFC->Find_5B5F90("mult_q");
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
                        wcsncpy(this->field_136[7].field_518[v69++ + 8].field_6_wstr_buf, Destination, 0x32u);
                    }
                    ++v25;
                    ++v26;
                    v58 = (s32*)((char_type*)v58 + 1);
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
                        v59 = &gYouthful_einstein_6F8450.field_20;
                        do
                        {
                            if (v31 != v65 && !*(BYTE*)v59 && (s16)gLucid_hamilton_67E8E0.sub_4C5D60(a3a) > v30)
                            {
                                v30 = (s16)gLucid_hamilton_67E8E0.sub_4C5D60(a3a);
                            }
                            ++v31;
                            v32 = ++a3a < a2b;
                            v59 = (s32*)((char_type*)v59 + 1);
                        } while (v32);
                    }
                    v33 = (s16)gLucid_hamilton_67E8E0.sub_4C5D60(v65);
                    break;
                case 2:
                    a3b = 0;
                    if (v24)
                    {
                        v34 = 0;
                        v60 = &gYouthful_einstein_6F8450.field_20;
                        do
                        {
                            if (v34 != v65 && !*(BYTE*)v60 && gLucid_hamilton_67E8E0.sub_4C5CB0(a3b) > v30)
                            {
                                v30 = gLucid_hamilton_67E8E0.sub_4C5CB0(a3b);
                            }
                            ++v34;
                            v32 = ++a3b < a2b;
                            v60 = (s32*)((char_type*)v60 + 1);
                        } while (v32);
                    }
                    v33 = gLucid_hamilton_67E8E0.sub_4C5CB0(v65);
                    break;
                case 3:
                    if (v24)
                    {
                        v35 = 0;
                        v36 = &gYouthful_einstein_6F8450.field_20;
                        v37 = gYouthful_einstein_6F8450.field_4_time;
                        v38 = a2b;
                        do
                        {
                            if (v35 != v65 && !*(BYTE*)v36)
                            {
                                v39 = *v37;
                                if (*v37 <= v30)
                                    v39 = v30;
                                v30 = v39;
                            }
                            ++v35;
                            ++v37;
                            v36 = (s32*)((char_type*)v36 + 1);
                            --v38;
                        } while (v38);
                    }
                    v33 = gYouthful_einstein_6F8450.field_4_time[v65];
                    break;
                default:
                    v3 = arg0;
                    goto LABEL_107;
            }

            v3 = arg0;
            v40 = v33 - v30;

            if (v40 > 0)
            {
                v41 = gText_0x14_704DFC->Find_5B5F90("mult_w");
                wcsncpy(this->field_136[7].field_518[0].field_6_wstr_buf, v41, 0x32u);
                break;
            }

            if (v40 < 0)
            {
            LABEL_105:
                v42 = gText_0x14_704DFC->Find_5B5F90("mult_l");
                wcsncpy(this->field_136[7].field_518[0].field_6_wstr_buf, v42, 0x32u);
                break;
            }

        LABEL_107:
            v43 = gText_0x14_704DFC->Find_5B5F90("mult_d");
            wcsncpy(this->field_136[7].field_518[0].field_6_wstr_buf, v43, 0x32u);
            break;

        case 8u:
            if (bIsFrench_67D53C)
            {
                sub_4B8650();
            }

            if (pre_intro_bik_exists_4B6030())
            {
                field_0_hDriver = gSampManager_6FFF00.field_0_hDriver;
                v44 = gLaughing_blackwell_0x1EB54_67DC84->pre_intro_bik_4B5F20();
                Bink::sub_513560(v44, field_0_hDriver);
            }
            else
            {
                v50 = gSampManager_6FFF00.field_0_hDriver;
                v45 = gLaughing_blackwell_0x1EB54_67DC84->intro_bik_4B5E50();
                Bink::sub_5133E0(v45, v50);
            }
            break;

        case 0u:
            this->field_C9E4 = timeGetTime();
            break;
    }

LABEL_116:
    this->field_132_f136_idx = v3;
    this->field_136[v3].field_BC6_nifty_idx = this->field_136[v3].field_BC8;
    field_132_f136_idx = this->field_132_f136_idx;
    v47 = &this->field_136[field_132_f136_idx];
    if (!v47->field_4[this->field_136[field_132_f136_idx].field_BC6_nifty_idx].field_1)
    {
        v47->sub_4B6200();
        if (!v47->field_4[v47->field_BC6_nifty_idx].field_1)
        {
            FatalError_4A38C0(165, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 4269); // the menu contains no valid options
        }
    }

    sub_4B6780();
}

STUB_FUNC(0x4B8680)
void laughing_blackwell_0x1EB54::sub_4B8680()
{
    if (!bSkip_audio_67D6BE)
    {
        // todo
    }
}

STUB_FUNC(0x4AEDB0)
s32 laughing_blackwell_0x1EB54::sub_4AEDB0()
{
    u32 Time; // eax
    u16 field_132_f136_idx; // cx
    const char_type* v5; // eax
    char_type* field_8_keys; // edi
    s32 v7; // ebx
    s32 result; // eax
    char_type* v9; // ecx
    s32 v10; // edx
    char_type v12; // al
    HDIGDRIVER field_0_hDriver; // [esp-4h] [ebp-10h]

    Time = timeGetTime();
    field_132_f136_idx = this->field_132_f136_idx;
    if (field_132_f136_idx == 8)
    {
        if (Bink::sub_513240())
        {
            if (Bink::sub_513790() == 1)
            {
                Bink::sub_5137A0(2);
                field_0_hDriver = gSampManager_6FFF00.field_0_hDriver;
                Bink::sub_5133E0(gLaughing_blackwell_0x1EB54_67DC84->intro_bik_4B5E50(), field_0_hDriver);
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

        sub_4AFEB0();

        field_8_keys = this->field_8_keys;
        v7 = 256;
        do
        {
            if ((*field_8_keys & 0x80u) != 0)
            {
                Bink::Close1_513340();
                Bink::Close2_513390();
                if (bIsFrench_67D53C)
                {
                    sub_4B8680();
                }
                sub_4B3170(0);
            }
            ++field_8_keys;
            --v7;
        } while (v7);

        return this->field_108;
    }
    else
    {
        if (!field_132_f136_idx)
        {
            v9 = this->field_8_keys;
            v10 = 256;
            do
            {
                if ((*v9 & 0x80u) != 0)
                {
                    this->field_C9E4 = Time;
                }
                ++v9;
                --v10;
            } while (v10);
            if (Time - this->field_C9E4 > 60000)
            {
                return 4;
            }
        }
        else
        {
            this->field_C9E4 = Time;
        }

        if (Time >= this->field_C9DC || (this->field_C9E0 == 3))
        {
            sub_4AEC00();
            v12 = this->field_C9E0 + 1;
            this->field_C9DC += 33;
            this->field_C9E0 = v12;
        }
        else if (this->field_C9E0)
        {
            sub_4ADFB0(); // bQuit ??
            result = this->field_108;
            this->field_C9E0 = 0;
            return result;
        }
        return this->field_108;
    }
}

// todo: add to header
EXPORT_VAR extern s32 gGTA2VersionMajor_708280;
EXPORT_VAR extern s32 gGTA2VersionMajor_708284;

STUB_FUNC(0x4AD140)
void laughing_blackwell_0x1EB54::sub_4AD140()
{
    s32 v1; // edx
    s32 v2; // ebx
    u16 field_132_f136_idx; // dx
    s32 v5; // ecx
    s32 v6; // eax
    loving_borg_0xBCA* v7; // edi
    s16 v8; // esp^2
    s32 v9; // eax
    s32 v10; // ecx
    s32* v11; // ecx
    s16 v12; // ax
    void* v13; // edx
    void* v14; // eax
    s16 v15; // ax
    void* v16; // ecx
    s16 v17; // dx
    s16 v18; // ax
    u8 v19; // al
    wchar_t* v20; // ecx
    u16 v21; // ax
    s32 v22; // ecx
    u8 v23; // al
    u8 v24; // bl
    u8 v25; // bp
    wchar_t* _5B5F90; // eax
    u16 v27; // ax
    bool v28; // zf
    s32* v29; // ecx
    s32 v30; // eax
    s32 v31; // ebp
    wchar_t* v32; // edx
    s32* v33; // ecx
    u16 v34; // ax
    void* v35; // eax
    char_type v36; // cl
    s16 v37; // ax
    void* v38; // ecx
    void* v39; // edx
    s16 v40; // ax
    void* v41; // ecx
    s32* v42; // ecx
    s32* v43; // ecx
    u16 v44; // ax
    char_type v45; // dl
    u32* v46; // eax
    wchar_t* v47; // ecx
    s32 v48; // ecx
    s32 v49; // edx
    wchar_t* v50; // ecx
    s32 v51; // ecx
    bool v52; // cf
    s32 v53; // eax
    u16 v54; // ax
    s32 v55; // eax
    s32 v56; // ebp
    u16 v57; // bx
    u16 v58; // dx
    s32* v59; // eax
    wchar_t* v60; // ecx
    s16 v61; // ax
    s32 v62; // edx
    u16 v63; // ax
    s32 v64; // ecx
    s32* v65; // ecx
    wchar_t* v66; // ecx
    s32 v67; // ecx
    s16 v68; // ax
    u16 v69; // bp
    u16 v70; // bx
    s32* v71; // ecx
    s32* v72; // ecx
    wchar_t* v73; // eax
    s32 v74; // eax
    s16 v75; // esp^2
    s32* v76; // ecx
    s32* v77; // ecx
    s16 v78; // ax
    u16 v79; // bx
    u16 v80; // di
    s32* v81; // ecx
    s32* v82; // ecx
    wchar_t* v83; // [esp-24h] [ebp-50h] BYREF
    s32 v84; // [esp-20h] [ebp-4Ch] BYREF
    agitated_keldysh_0xF0* v85; // [esp-1Ch] [ebp-48h] BYREF
    s32 v86; // [esp-18h] [ebp-44h]
    s32 v87; // [esp-14h] [ebp-40h] BYREF
    s32 fp_300; // [esp-10h] [ebp-3Ch] BYREF
    s32* fp_460; // [esp-Ch] [ebp-38h] BYREF
    s32 v90; // [esp-8h] [ebp-34h]
    s32 v91; // [esp-4h] [ebp-30h]
    wchar_t* field_6_wstr_buf; // [esp+10h] [ebp-1Ch] BYREF
    s32 v93; // [esp+14h] [ebp-18h]
    s32* v94; // [esp+18h] [ebp-14h]
    s32 v95; // [esp+1Ch] [ebp-10h] BYREF
    s32 v96; // [esp+20h] [ebp-Ch]
    s32 v97; // [esp+24h] [ebp-8h] BYREF
    s32 v98; // [esp+28h] [ebp-4h] BYREF

    field_132_f136_idx = this->field_132_f136_idx;
    v98 = gText_0x14_704DFC->field_10_lang_code != 'j' ? 14 : 16;
    v5 = field_132_f136_idx;
    //v6 = 0x5E5 * field_132_f136_idx;
    v7 = &this->field_136[field_132_f136_idx];
    if (!field_132_f136_idx)
    {
        swprintf(tmpBuff_67BD9C, L"GTA2 V%d.%d", gGTA2VersionMajor_708280, gGTA2VersionMajor_708284);
        //HIWORD(v9) = v8;
        //LOWORD(v9) = word_703C14;
        //v90 = v9;
        //fp_460 = (s32 *)v10;
        Fix16 fp_460;
        fp_460.FromInt_4369F0(460);
        //fp_300 = (int)v11;
        Fix16 fp_300;
        fp_300.FromInt_4369F0(300);

        Fix16 tmp;
        tmp.mValue = 0x4000;

        /*v6 =*/DrawText_4B87A0(tmpBuff_67BD9C, fp_300, fp_460, word_703C14, tmp);

        tmp.mValue = 0x4000;

        Fix16 fp_0;
        fp_0.FromInt_4369F0(0);
        DrawText_4B87A0(L"1111", fp_0, fp_0, word_703C14, tmp);
    }

    if (this->field_132_f136_idx == 1)
    {
        if (this->field_110_state == 3)
        {
            /*
            v7->field_518[8].field_1 = 0;
            v7->field_518[9].field_1 = 0;
            LOWORD(v5) = this->field_124;
            v12 = sub_4B7E10(2, 0x12Cu, 0x1B8u, (void *)v5, 0xFFFF);
            LOWORD(v13) = this->field_124;
            v14 = (void *)sub_4B7E10(11, v12 + 300, 0x1B8u, v13, 0xFFFF);
            LOWORD(v14) = this->field_124;
            v15 = sub_4B7E10(3, 0x12Cu, 0x1CCu, v14, 0xFFFF);
            LOWORD(v16) = this->field_124;
            sub_4B7E10(10, v15 + 300, 0x1CCu, v16, 0xFFFF);
            */
        }
        else
        {
            /*
            LOWORD(v6) = v7->field_4[0].field_6E_count;
            v17 = this->field_11C;
            v97 = v6;
            v18 = laughing_blackwell_0x1EB54::sub_5D8990(
                (s16 *)gJolly_poitras_0x2BC0_6FEAC0->field_26A0[(u16)v6].field_90_str,
                v17)
                + 10;
            if (v18 == 10)
                v18 = laughing_blackwell_0x1EB54::sub_5D8990((s16 *)v7->field_4[0].field_6_wstr_buf, this->field_11C) + 40;
            v7->field_518[9].field_2 = v18 + v7->field_4[0].field_2;
            */
        }
    }
    if (this->field_132_f136_idx == 5)
    {
        /*
        v19 = this->field_EE0D;
        if (v19 < 3u)
        {
            LOWORD(v1) = this->field_12A;
            v91 = 2;
            v90 = 0xFFFF;
            fp_460 = (s32 *)v1;
            fp_300 = 5;
            v87 = 250;
            v86 = 300;
            v85 = gJolly_poitras_0x2BC0_6FEAC0->field_1890[v19];
            goto LABEL_18;
        }

        if (v19 >= 6u)
        {
            v91 = 2;
            v90 = 0xFFFF;
            if (v19 < 9u)
            {
                LOWORD(v1) = this->field_12A;
                fp_460 = (s32 *)v1;
                fp_300 = 5;
                v87 = 250;
                v86 = 300;
                v85 = (agitated_keldysh_0xF0 *)(&gJolly_poitras_0x2BC0_6FEAC0->field_17A0 + 60 * v19);
                goto LABEL_18;
            }
            LOWORD(v5) = this->field_12A;
            fp_460 = (s32 *)v5;
            fp_300 = 5;
            v87 = 250;
            v86 = 300;
            v20 = (wchar_t *)&gJolly_poitras_0x2BC0_6FEAC0->field_1890[0][v19];
        }
        else
        {
            LOWORD(v5) = this->field_12A;
            v91 = 2;
            v90 = 0xFFFF;
            fp_460 = (s32 *)v5;
            fp_300 = 5;
            v87 = 250;
            v86 = 300;
            v20 = (wchar_t *)(&gJolly_poitras_0x2BC0_6FEAC0->field_16B0 + 0x3C * v19);
        }
        */

        /*
        v85 = (agitated_keldysh_0xF0 *)v20;
    LABEL_18:
        sub_4B5430(this, (wchar_t *)v85, v86, v87, fp_300, (int)fp_460, v90, v91);
        if (!byte_67DA80)
        {
            if (this->field_EE0D)
                v7->field_518[2].field_1 = 1;
            else
                v7->field_518[2].field_1 = 0;
            v7->field_518[3].field_1 = this->field_EE0D != 11;
        }*/
    }

    v21 = this->field_132_f136_idx;
    if (v21 == 2 || v21 == 3 || v21 == 6 || v21 == 11)
    {
        /*
        if (lucid_hamilton::sub_4C59A0(&gLucid_hamilton_67E8E0))
        {
            v23 = lucid_hamilton::sub_4C5990(&gLucid_hamilton_67E8E0);
            LOBYTE(v96) = v23 >> 4;
            LOBYTE(v95) = v23 & 0xF;
        }
        else
        {
            LOBYTE(v96) = lucid_hamilton::sub_4C5980(&gLucid_hamilton_67E8E0);
            LOBYTE(v95) = 0;
        }
        v24 = v96;
        v25 = v95;
        if (this->field_132_f136_idx == 6)
        {
            v91 = 3 * (unsigned __int8)v96 + (unsigned __int8)v95 + 64;
            _5B5F90 = text_0x14::Find_5B5F90(gText_0x14_704DFC, "bonslev");
            swprintf(tmpBuff_67BD9C, L"%s %c", _5B5F90, v91);
            wcsncpy(v7->field_518[0].field_6_wstr_buf, tmpBuff_67BD9C, 0x32u);
            sub_4B7D60((int)this, this);
        }
        LOWORD(v22) = this->field_12A;
        v2 = v25 + 4 * v24;
        sub_4B5430(this, (wchar_t *)&gJolly_poitras_0x2BC0_6FEAC0->field_1890[0][v2], 170, 155, 3, v22, 0xFFFF, 2);
        v27 = this->field_132_f136_idx;
        if (v27 == 2 || v27 == 3 || v27 == 11)
            sub_4B57B0((int)this, this, 10, (text_0x14 *)0xE1);
            */
    }

    if (this->field_132_f136_idx == 7)
    {
        //laughing_blackwell_0x1EB54::sub_4B55F0(this);
    }

    /*
    v28 = v7->field_0 == 0;
    v96 = -1;
    v94 = 0;
    if (v28)
        goto LABEL_92;
    do
    {
        v29 = v94;
        v30 = (u16)v94;
        v31 = (int)&v7->field_4[(u16)v94];
        if (!v7->field_4[(u16)v94].field_1)
        {
            v44 = this->field_132_f136_idx;
            v45 = 0;
            if ((v44 == 1 || v44 == 3) && (_WORD)v94 == 1)
                v45 = 1;
            if (v44 != 11 && v44 != 2 || (_WORD)v94)
            {
                if (!v45)
                    goto LABEL_87;
                v29 = v94;
            }
            if (*(_BYTE *)v31 == 2)
                sub_4B3AF0((int)this, v44, (u16)v29, &field_6_wstr_buf);
            else
                field_6_wstr_buf = v7->field_4[(u16)v94].field_6_wstr_buf;
            LOWORD(v29) = *(_WORD *)(v31 + 4);
            v28 = *(_WORD *)(v31 + 106) == 0xFFFF;
            LOWORD(v2) = *(_WORD *)(v31 + 2);
            LOWORD(v93) = (_WORD)v29;
            if (!v28)
            {
                v91 = 0;
                v90 = 0;
                v95 = 8;
                fp_460 = (s32 *)8;
            LABEL_83:
                fp_300 = (int)&v95;
                v87 = (int)v29;
                v46 = FP::FromInt_4369F0(&v87, 1);
                LOWORD(v46) = *(_WORD *)(v31 + 106);
                v86 = (int)v46;
                v85 = (agitated_keldysh_0xF0 *)v47;
                FP::sub_4AE970(&v85, v93);
                v84 = v48;
                FP::sub_4AE970(&v84, v2);
                v83 = field_6_wstr_buf;
            LABEL_86:
                DrawText_5D8A10(v83, v84, (int)v85, (void *)v86, v87, (s32 *)fp_300, (int)fp_460, v90, v91);
                goto LABEL_87;
            }
            v91 = 0;
            v90 = 0;
            v95 = 8;
            fp_460 = (s32 *)8;
        LABEL_85:
            fp_300 = (int)&v95;
            v87 = (int)&v95;
            FP::FromInt_4369F0(&v87, 1);
            LOWORD(v49) = this->field_11C;
            v86 = v49;
            v85 = (agitated_keldysh_0xF0 *)v50;
            FP::sub_4AE970(&v85, v93);
            v84 = v51;
            FP::sub_4AE970(&v84, v2);
            v83 = field_6_wstr_buf;
            goto LABEL_86;
        }
        if (*(_BYTE *)v31 == 2)
        {
            v30 = (int)sub_4B3AF0((int)this, this->field_132_f136_idx, (u16)v94, &field_6_wstr_buf);
        }
        else
        {
            v32 = v7->field_4[(u16)v94].field_6_wstr_buf;
            field_6_wstr_buf = v32;
        }
        LOWORD(v29) = (_WORD)v94;
        v28 = (_WORD)v94 == v7->field_BC6_nifty_idx;
        LOWORD(v2) = *(_WORD *)(v31 + 2);
        LOWORD(v93) = *(_WORD *)(v31 + 4);
        if (!v28)
        {
            if (*(_WORD *)(v31 + 106) != 0xFFFF)
            {
                LOWORD(v30) = *(_WORD *)(v31 + 108);
                if ((_WORD)v30 == 0xFFFF)
                {
                    v91 = 0x4000;
                    LOWORD(v32) = *(_WORD *)(v31 + 106);
                    v90 = (int)v32;
                    fp_460 = v29;
                    FP::sub_4AE970(&fp_460, v93);
                    fp_300 = (int)v42;
                    FP::sub_4AE970(&fp_300, v2);
                    DrawText_4B87A0(field_6_wstr_buf, fp_300, (int)fp_460, (void *)v90, v91);
                    goto LABEL_87;
                }
                v91 = 0;
                v90 = 0;
                v95 = 8;
                fp_460 = (s32 *)v30;
                goto LABEL_83;
            }
            LOWORD(v31) = *(_WORD *)(v31 + 108);
            if ((_WORD)v31 == 0xFFFF)
            {
                v91 = 0x4000;
                LOWORD(v29) = this->field_11C;
                v90 = (int)v29;
                fp_460 = v29;
                FP::sub_4AE970(&fp_460, v93);
                fp_300 = (int)v43;
                FP::sub_4AE970(&fp_300, v2);
                DrawText_4B87A0(field_6_wstr_buf, fp_300, (int)fp_460, (void *)v90, v91);
                goto LABEL_87;
            }
            v91 = 0;
            v90 = 0;
            v95 = 8;
            fp_460 = (s32 *)v31;
            goto LABEL_85;
        }
        v91 = 0x4000;
        LOWORD(v32) = this->field_120;
        v90 = (int)v32;
        fp_460 = v29;
        FP::sub_4AE970(&fp_460, v93);
        fp_300 = (int)v33;
        FP::sub_4AE970(&fp_300, v2);
        DrawText_4B87A0(field_6_wstr_buf, fp_300, (int)fp_460, (void *)v90, v91);
        v34 = this->field_132_f136_idx;
        HIWORD(v2) = 0;
        if (v34 == 1)
        {
            v35 = v94;
            v28 = (_WORD)v94 == 3;
            v7->field_518[4].field_1 = 0;
            v7->field_518[5].field_1 = 0;
            v7->field_518[6].field_1 = 0;
            v7->field_518[7].field_1 = 0;
            v7->field_518[8].field_1 = 0;
            v7->field_518[9].field_1 = 0;
            if (v28)
            {
                v96 = 3;
                v7->field_518[4].field_6_wstr_buf[0] = 1;
                v7->field_518[5].field_6_wstr_buf[0] = 2;
                v7->field_518[4].field_1 = this->field_1EB4C != 0;
                v7->field_518[5].field_1 = this->field_1EB4D != 0;
            }
            else if ((_WORD)v35 == 4)
            {
                v7->field_518[6].field_6_wstr_buf[0] = 1;
                v7->field_518[7].field_6_wstr_buf[0] = 2;
                v36 = this->field_1EB4E;
                v96 = 4;
                v7->field_518[6].field_1 = v36 != 0;
                v7->field_518[7].field_1 = this->field_1EB4F != 0;
            }
            else if (!(_WORD)v35)
            {
                v7->field_518[8].field_6_wstr_buf[0] = 1;
                v7->field_518[9].field_6_wstr_buf[0] = 2;
                if (this->field_110_state != 3)
                {
                    v7->field_518[8].field_1 = 1;
                    v7->field_518[9].field_1 = 1;
                    this->field_1EB4A = 1;
                    this->field_1EB4B = 1;
                    if (!byte_67DA80)
                    {
                        v35 = (void *)v97;
                        if ((_WORD)v97)
                        {
                            if ((_WORD)v97 == v7->field_4[0].field_7E)
                            {
                                v7->field_518[9].field_1 = 0;
                                this->field_1EB4B = 0;
                            }
                        }
                        else
                        {
                            v7->field_518[8].field_1 = 0;
                            this->field_1EB4A = (char)v35;
                        }
                    }
                    LOWORD(v35) = this->field_124;
                    v37 = sub_4B7E10(2, 0x12Cu, 0x1B8u, v35, 0xFFFF);
                    LOWORD(v38) = this->field_124;
                    sub_4B7E10(8, v37 + 300, 0x1B8u, v38, 0xFFFF);
                    LOWORD(v39) = this->field_124;
                    v40 = sub_4B7E10(1, 0x12Cu, 0x1CCu, v39, 0xFFFF);
                    LOWORD(v41) = this->field_124;
                    sub_4B7E10(9, v40 + 300, 0x1CCu, v41, 0xFFFF);
                }
            }
        }
        else if (v34 == 5)
        {
            v28 = (_WORD)v94 == 0;
            v7->field_518[2].field_6_wstr_buf[0] = 3;
            v7->field_518[3].field_6_wstr_buf[0] = 4;
            if (v28)
            {
                v7->field_518[2].field_6_wstr_buf[0] = 1;
                v7->field_518[3].field_6_wstr_buf[0] = 2;
            }
        }
    LABEL_87:
        v52 = (u16)((_WORD)v94 + 1) < v7->field_0;
        v94 = (s32 *)((char_type *)v94 + 1);
    } while (v52);
    if (v96 == 3)
    {
        LOBYTE(v96) = lucid_hamilton::sub_4C5980(&gLucid_hamilton_67E8E0);
        LOBYTE(v95) = 0;
    LABEL_91:
        LOWORD(v53) = this->field_128;
        sub_4B5430(
            this,
            (wchar_t *)&gJolly_poitras_0x2BC0_6FEAC0->field_1890[(unsigned __int8)v96][(unsigned __int8)v95],
            300,
            v98,
            1,
            v53,
            0xFFFF,
            2);
    }
    else if (v96 == 4)
    {
        LOBYTE(v53) = lucid_hamilton::sub_4C5990(&gLucid_hamilton_67E8E0);
        LOBYTE(v96) = (unsigned __int8)v53 >> 4;
        LOBYTE(v95) = v53 & 0xF;
        goto LABEL_91;
    }

LABEL_92:
    v54 = 0;
    v28 = v7->field_2 == 0;
    v94 = 0;

    if (!v28)
    {
        do
        {
            v55 = v54;
            v56 = (int)&v7->field_518[v55];
            if (v7->field_518[v55].field_1)
            {
                if (*(_BYTE *)v56 == 1)
                {
                    LOWORD(v2) = v7->field_518[v55].field_6A;
                    v61 = v7->field_518[v55].field_4;
                    LOWORD(v96) = *(_WORD *)(v56 + 2);
                    LOWORD(v93) = v61;
                    if ((_WORD)v2 == 0xFFFF)
                        LOWORD(v2) = this->field_11C;
                    sub_4B3CC0((int)this, this->field_132_f136_idx, (u16)v94, (wchar_t *)&field_6_wstr_buf);
                    v63 = this->field_132_f136_idx;
                    HIWORD(v64) = HIWORD(v94);
                    if (v63 == 1 && ((_WORD)v94 == 2 || (_WORD)v94 == 3))
                    {
                        LOWORD(v64) = *(_WORD *)(v56 + 108);
                        sub_4B78B0(field_6_wstr_buf, v96, v93, v2, v64, 1u, 0x15u, 1);
                    }
                    else if (v63 == 5 && (_WORD)v94 == 1)
                    {
                        LOWORD(v62) = *(_WORD *)(v56 + 108);
                        sub_4B78B0(field_6_wstr_buf, v96, v93, v2, v62, 1u, 0x15u, 1);
                    }
                    else
                    {
                        LOWORD(v56) = *(_WORD *)(v56 + 108);
                        if ((_WORD)v56 == 0xFFFF)
                        {
                            v90 = v2;
                            fp_460 = v94;
                            v91 = 0x4000;
                            FP::sub_4AE970(&fp_460, v93);
                            fp_300 = (int)v65;
                            FP::sub_4AE970(&fp_300, v96);
                            DrawText_4B87A0(field_6_wstr_buf, fp_300, (int)fp_460, (void *)v90, v91);
                        }
                        else
                        {
                            v91 = 0;
                            v90 = 0;
                            fp_460 = (s32 *)v56;
                            fp_300 = (int)&v97;
                            v87 = (int)&v97;
                            v97 = 8;
                            FP::FromInt_4369F0(&v87, 1);
                            v86 = v2;
                            v85 = (agitated_keldysh_0xF0 *)v66;
                            FP::sub_4AE970(&v85, v93);
                            v84 = v67;
                            FP::sub_4AE970(&v84, v96);
                            DrawText_5D8A10(field_6_wstr_buf, v84, (int)v85, (void *)v86, v87, (s32 *)fp_300, (int)fp_460, v90, v91);
                        }
                    }
                }
                else if (*(BYTE *)v56 == 3)
                {
                    v57 = v7->field_518[v55].field_2;
                    v58 = v7->field_518[v55].field_4;
                    switch (v7->field_518[v55].field_6_wstr_buf[0])
                    {
                    case 0u:
                        v95 = 2;
                        break;
                    case 1u:
                        v95 = 37;
                        break;
                    case 2u:
                        v95 = 38;
                        break;
                    case 3u:
                        v95 = 40;
                        break;
                    case 4u:
                        v95 = 41;
                        break;
                    default:
                        break;
                    }
                    v91 = 0;
                    v90 = 0;
                    fp_460 = 0;
                    v59 = &v98;
                    v98 = 2;
                    fp_300 = 0;
                    v87 = (int)&v98;
                    LOWORD(v59) = HIWORD(dword_67DA6E);
                    v86 = dword_67D934;
                    v85 = (agitated_keldysh_0xF0 *)v59;
                    v84 = dword_67D934;
                    FP::sub_4AE970(&v84, v58);
                    v83 = v60;
                    FP::sub_4AE970(&v83, v57);
                    sub_5D7EC0(6, v95, (int)v83, v84, (int)v85, v86, (s32 *)v87, fp_300, (int)fp_460, v90, v91);
                }
            }
            v54 = (_WORD)v94 + 1;
            v52 = (u16)((_WORD)v94 + 1) < (u32)v7->field_2;
            v94 = (s32 *)((char_type *)v94 + 1);
        } while (v52);
    }
    */

    if (this->field_110_state == 3)
    {
        if (this->field_114)
        {
            /*
            v68 = laughing_blackwell_0x1EB54::sub_5D8990((s16 *)this->field_C9A0, this->field_11C);
            v69 = v7->field_4[0].field_4;
            v70 = v7->field_4[0].field_2 + v68;
            swprintf(tmpBuff_67BD9C, asc_62017C);
            v91 = 0x4000;
            LOWORD(v71) = this->field_11C;
            v90 = (int)v71;
            fp_460 = v71;
            FP::sub_4AE970(&fp_460, v69);
            fp_300 = (int)v72;
            FP::sub_4AE970(&fp_300, v70);
            DrawText_4B87A0(tmpBuff_67BD9C, fp_300, (int)fp_460, (void *)v90, v91);
            */
        }

        /*
        v73 = text_0x14::Find_5B5F90(gText_0x14_704DFC, aEntrnam);
        wcscpy(tmpBuff_67BD9C, v73);
        v74 = -(gText_0x14_704DFC->field_10_lang_code != 106);
        HIWORD(v76) = v75;
        LOBYTE(v74) = v74 & 0xFC;
        v91 = 0x4000;
        LOWORD(v76) = this->field_126;
        v90 = (int)v76;
        fp_460 = v76;
        FP::sub_4AE970(&fp_460, v74 + 16);
        fp_300 = (int)v77;
        FP::sub_4AE970(&fp_300, 0x15Eu);
        DrawText_4B87A0(tmpBuff_67BD9C, fp_300, (int)fp_460, (void *)v90, v91);
        */
    }

    if (this->field_110_state == 5)
    {
        if (this->field_114)
        {
            /*
            v78 = laughing_blackwell_0x1EB54::sub_5D8990((s16 *)this->field_C9B8, this->field_11C);
            v91 = (int)asc_62017C;
            v79 = v7->field_518[4].field_2 + v78;
            v80 = v7->field_518[4].field_4;
            swprintf(tmpBuff_67BD9C, asc_62017C);
            v91 = 0x4000;
            LOWORD(v81) = this->field_11C;
            v90 = (int)v81;
            fp_460 = v81;
            FP::sub_4AE970(&fp_460, v80);
            fp_300 = (int)v82;
            FP::sub_4AE970(&fp_300, v79);
            DrawText_4B87A0(tmpBuff_67BD9C, fp_300, (int)fp_460, (void *)v90, v91);
            */
        }
    }
}

STUB_FUNC(0x4B7AE0)
void laughing_blackwell_0x1EB54::sub_4B7AE0()
{
    // todo

    // TEST

    if (gbh_BlitImage(tgaArray_61F0C8[23].field_84_img, 0, 0, 451, 144, 85, 0) == -10)
    {
        Load_tga_4B6520(23u);
        gbh_BlitImage(tgaArray_61F0C8[23].field_84_img, 0, 0, 451, 144, 85, 0);
    }
}

STUB_FUNC(0x4B8650)
void laughing_blackwell_0x1EB54::sub_4B8650()
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
char_type laughing_blackwell_0x1EB54::pre_intro_bik_exists_4B6030()
{
    char_type* v0; // eax
    long v1; // eax
    _finddata_t v3; // [esp+0h] [ebp-118h] BYREF

    v0 = gLaughing_blackwell_0x1EB54_67DC84->pre_intro_bik_4B5F20();
    v1 = _findfirst(v0, &v3);
    if (v1 == -1)
        return 0;
    _findclose(v1);
    return 1;
}

STUB_FUNC(0x4B5F20)
char_type* laughing_blackwell_0x1EB54::pre_intro_bik_4B5F20()
{
    // todo
    return "";
}

STUB_FUNC(0x4B5E50)
const char_type* laughing_blackwell_0x1EB54::intro_bik_4B5E50()
{
    // todo
    return "meh.dat";
}

MATCH_FUNC(0x4B5FF0)
bool laughing_blackwell_0x1EB54::intro_bik_exists_4B5FF0()
{
    // note: ecx wasn't first due to global being an object instead of a pointer
    _finddata_t findData;

    // note: put call in argument rather than local to change inst ordering
    const long hFind = _findfirst(gLaughing_blackwell_0x1EB54_67DC84->intro_bik_4B5E50(), &findData);

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
void laughing_blackwell_0x1EB54::sub_4AEC00()
{
    sub_4AFEB0();
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

STUB_FUNC(0x4AFEB0)
void laughing_blackwell_0x1EB54::sub_4AFEB0()
{
    // todo
}

STUB_FUNC(0x4B6780)
void laughing_blackwell_0x1EB54::sub_4B6780()
{
    loving_borg_0xBCA* pBorg = &field_136[field_132_f136_idx];
    if (this->field_110_state != 2)
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
                    this->field_EE08 = EnterPlayerName_10;
                    break;
                case 1u:
                    this->field_EE08 = ResumeLoadSave_11;
                    break;
                case 2u:
                    this->field_EE08 = ViewHiScore_6;
                    break;
                case 3u:
                    this->field_EE08 = gLucid_hamilton_67E8E0.sub_4C5980() + 7;
                    break;
                case 4u:
                    this->field_EE08 = ((unsigned __int8)gLucid_hamilton_67E8E0.sub_4C5990() >> 4) + 3;
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
void laughing_blackwell_0x1EB54::sub_4B7A10()
{
    char_type v2; // cl
    char_type* field_8_keys; // eax
    s32 v4; // edx
    s32 v5; // eax
    s32 v6; // eax

    timeGetTime();
    sub_4AFEB0();
    v2 = 0;
    field_8_keys = this->field_8_keys;
    v4 = 256;
    do
    {
        if (*field_8_keys < 0)
            v2 = 1;
        ++field_8_keys;
        --v4;
    } while (v4);
    if (v2)
    {
        if (!this->field_C9B3)
        {
        LABEL_7:
            this->field_108 = 1;
            return;
        }
    }
    else
    {
        this->field_C9B3 = 0;
    }
    if (++this->field_1EB30)
    {
        v5 = this->field_1EB34;
        this->field_1EB30 = 0;
        if (v5 <= -327680)
        {
            while (++this->field_1EB38 != 600)
            {
                v6 = ((u16)this->field_EE0E_unk.field_2[(u16)this->field_1EB38].field_4 << 14) + this->field_1EB34;
                this->field_1EB34 = v6;
                if (v6 > -327680)
                    goto LABEL_13;
            }
            goto LABEL_7;
        }
    LABEL_13:
        this->field_1EB34 -= dword_67D9FC;
    }
}

STUB_FUNC(0x4AE2D0)
void laughing_blackwell_0x1EB54::sub_4AE2D0()
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

    pBorg = &this->field_136[this->field_132_f136_idx];
    v18 = pBorg;
    v3 = sub_4B43E0();
    if (this->field_C9D0)
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
                    this->field_108 = 1;
                    snd1_67D818.field_0_object_type = 5;
                    break;
                case 259u:
                    goto LABEL_21;
                case 260u:
                    v20 = gLucid_hamilton_67E8E0.sub_4C59B0(); //  LOBYTE(v20) =
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

                    if (v7 >= (u32)this->field_1EB50_idx)
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
                    for (i = 3; !v3->field_0[v19][i].field_0 || v5 >= this->field_1EB51_blocks[v19]; i = v5)
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
                    this->field_EE08 = RedBar_16;
                    this->field_110_state = 2;
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
                    this->field_EE08 = RedBar_16;
                    this->field_110_state = 2;
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
        else if (this->field_132_f136_idx == 1 && !pBorg->field_BC6_nifty_idx)
        {
            this->field_110_state = 3;
            sub_4B4280();
            this->field_C9B2 = wcslen(this->field_C9A0);
            sub_4B42B0();
            this->field_C9B3 = 1;
            this->field_C9B4 = 28;
            this->field_C9B6 = 5;
            snd1_67D818.field_0_object_type = 5;
        }
    }

    if (this->field_C9D1)
    {
        switch (this->field_132_f136_idx)
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
                this->field_108 = 1;
                break;
        }
        snd1_67D818.field_0_object_type = 6;
    }

    if (this->field_C9CE && pBorg->sub_4B61B0())
    {
        snd1_67D818.field_0_object_type = 1;
    }

    if (this->field_C9CF && pBorg->sub_4B6200())
    {
        snd1_67D818.field_0_object_type = 2;
    }

    if (this->field_C9CC)
    {
        field_BC6_nifty_idx = pBorg->field_BC6_nifty_idx;
        v11 = &pBorg->field_4[field_BC6_nifty_idx];
        if (v11->field_0 == 2)
        {
            v12 = pBorg->field_4[pBorg->field_BC6_nifty_idx].sub_4B6390();
            if (this->field_132_f136_idx == 1 && !v18->field_BC6_nifty_idx)
            {
                gLucid_hamilton_67E8E0.sub_4C5920(v11->field_6E_count);
                sub_4B42E0(); // this
                gRegistry_6FF968.Set_Player_Setting_5878C0("plyrslot", v11->field_6E_count);
                if (v12)
                {
                    snd1_67D818.field_0_object_type = 3;
                }
            }

            if (this->field_132_f136_idx == 5 && !v18->field_BC6_nifty_idx)
            {
                this->field_EE0D = v11->field_6E_count;
                if (v12)
                {
                    snd1_67D818.field_0_object_type = 3;
                }
            }

        LABEL_59:
            pBorg = v18;
            goto LABEL_60;
        }

        if (this->field_132_f136_idx == 1)
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
        if (!this->field_C9CD)
        {
            goto LABEL_60;
        }

        v14 = pBorg->field_BC6_nifty_idx;
        v15 = &pBorg->field_4[v14];
        if (v15->field_0 == 2)
        {
            v16 = pBorg->field_4[pBorg->field_BC6_nifty_idx].sub_4B6330(); // this
            if (this->field_132_f136_idx == 1 && !v18->field_BC6_nifty_idx)
            {
                gLucid_hamilton_67E8E0.sub_4C5920(v15->field_6E_count);
                sub_4B42E0();
                gRegistry_6FF968.Set_Player_Setting_5878C0("plyrslot", v15->field_6E_count);
                if (v16)
                {
                    snd1_67D818.field_0_object_type = 4;
                }
            }
            if (this->field_132_f136_idx == 5 && !v18->field_BC6_nifty_idx)
            {
                this->field_EE0D = v15->field_6E_count;
                if (v16)
                {
                    snd1_67D818.field_0_object_type = 4;
                }
            }
            goto LABEL_59;
        }

        if (this->field_132_f136_idx == 1)
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
    if (this->field_C9D2 && this->field_132_f136_idx == 1 && !pBorg->field_BC6_nifty_idx)
    {
        this->field_110_state = 4;
        this->field_EE0A = 190;
        this->field_EE0C = 1;
        snd1_67D818.field_0_object_type = 8;
    }

    v13 = this->field_118 - 1;
    this->field_118 = v13;
    if (v13 <= 0)
    {
        this->field_114 = this->field_114 == 0;
        this->field_118 = 2;
    }
}

MATCH_FUNC(0x4AE990)
void laughing_blackwell_0x1EB54::sub_4AE990()
{
    field_108 = 3;
}

STUB_FUNC(0x4B2F60)
void laughing_blackwell_0x1EB54::sub_4B2F60()
{
    // todo
}

STUB_FUNC(0x4AE9A0)
void laughing_blackwell_0x1EB54::sub_4AE9A0()
{
    s16 v2; // ax
    s32 v3; // eax

    if (this->field_C9D0)
    {
        v2 = this->field_EE0A;
        if (v2 == 210)
        {
            if (this->field_EE0C != 1)
            {
                FatalError_4A38C0(1006, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 1934);
            }

            sub_4B4410();
        }
        else if (v2 != 230)
        {
            snd1_67D818.field_0_object_type = 5;
        }
        else
        {
            this->field_110_state = 1;
            snd1_67D818.field_0_object_type = 5;
        }
    }

    if (this->field_C9D1)
    {
        this->field_110_state = 1;
        snd1_67D818.field_0_object_type = 6;
    }

    if (this->field_C9CE)
    {
        if (this->field_EE0A == 190 || this->field_EE0A == 210)
        {
            this->field_EE0A = 230;
        }
        else
        {
            if (this->field_EE0A != 230)
            {
                FatalError_4A38C0(1006, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 1968);
            }
            this->field_EE0A = 210;
        }
        snd1_67D818.field_0_object_type = 1;
    }

    if (this->field_C9CF)
    {
        if (this->field_EE0A != 190)
        {
            if (this->field_EE0A == 210)
            {
                this->field_EE0A = 230;
                snd1_67D818.field_0_object_type = 2;
            }
            else if (this->field_EE0A != 230)
            {
                FatalError_4A38C0(1006, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 1991);
            }
        }
        else
        {
            this->field_EE0A = 210;
            snd1_67D818.field_0_object_type = 2;
        }
    }

    v3 = this->field_118 - 1;
    this->field_118 = v3;
    if (v3 <= 0)
    {
        this->field_114 = this->field_114 == 0;
        this->field_118 = 2;
    }
}

STUB_FUNC(0x4B8280)
void laughing_blackwell_0x1EB54::sub_4B8280()
{
    // todo
}

MATCH_FUNC(0x4B4410)
void laughing_blackwell_0x1EB54::sub_4B4410()
{
    sub_4B43E0()->sub_56B630();
    gJolly_poitras_0x2BC0_6FEAC0->sub_56BA60(field_136[1].field_4[0].field_6E_count);
    sub_4B42E0();
}

MATCH_FUNC(0x4B43E0)
dreamy_clarke_0xA4* laughing_blackwell_0x1EB54::sub_4B43E0()
{
    // note: movsx vs movzx due to signedness
    u16 idx = gLucid_hamilton_67E8E0.sub_4C59B0();
    return &gJolly_poitras_0x2BC0_6FEAC0->field_26A0[idx];
}

STUB_FUNC(0x4B42E0)
void laughing_blackwell_0x1EB54::sub_4B42E0()
{
    // todo
}

MATCH_FUNC(0x4B4230)
void laughing_blackwell_0x1EB54::sub_4B4230()
{
    u16 count = field_136[1].field_4[0].field_6E_count;
    wchar_t* pStr = gJolly_poitras_0x2BC0_6FEAC0->field_26A0[count].field_90_str;
    wcsncpy(pStr, field_C9A0, 9u);
    HandleCheatCode_4B3DD0(pStr);
    gJolly_poitras_0x2BC0_6FEAC0->sub_56BA60(count);
}

STUB_FUNC(0x4B3DD0)
void laughing_blackwell_0x1EB54::HandleCheatCode_4B3DD0(const wchar_t* String)
{
    // todo
}

MATCH_FUNC(0x4B4280)
void laughing_blackwell_0x1EB54::sub_4B4280()
{
    wcsncpy(field_C9A0, gJolly_poitras_0x2BC0_6FEAC0->field_26A0[field_136[1].field_4[0].field_6E_count].field_90_str, 9u);
}

STUB_FUNC(0x4B8530)
void laughing_blackwell_0x1EB54::sub_4B8530()
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
void laughing_blackwell_0x1EB54::sub_4B8560()
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

STUB_FUNC(0x4B8020)
void laughing_blackwell_0x1EB54::sub_4B8020()
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

STUB_FUNC(0x4B7FB0)
char_type laughing_blackwell_0x1EB54::sub_4B7FB0()
{
    dreamy_clarke_0xA4* v2; // eax
    u16 v3; // bx
    u16 v4; // dx

    v2 = sub_4B43E0();
    v3 = 0;
    if (!this->field_1EB50_idx)
        return 1;
    while (1)
    {
        v4 = 0;
        if (this->field_1EB51_blocks[v3])
            break;
    LABEL_5:
        if (++v3 >= (unsigned __int8)this->field_1EB50_idx)
            return 1;
    }
    while (v2->field_0[v3][v4].field_0)
    {
        if (++v4 >= this->field_1EB51_blocks[v3])
            goto LABEL_5;
    }
    return 0;
}

STUB_FUNC(0x4B4D00)
void laughing_blackwell_0x1EB54::sub_4B4D00(u8 mainBlockIdx, u8 bonusBlockIdx)
{
    // todo
}

STUB_FUNC(0x4ADF50)
void laughing_blackwell_0x1EB54::sub_4ADF50() // s32 bQuit ??
{
    // todo

    /*
    wchar_t *pLoading; // eax
    u16 v4; // di
    s16 v5; // esp^2
    laughing_blackwell_0x1EB54 *v6; // eax
    s32 v7; // ecx
    s32 v8; // ecx
    wchar_t *pLoading2; // eax
    s16 v11; // esp^2
    laughing_blackwell_0x1EB54 *v12; // ecx
    char_type v13; // al
    s32 v14; // eax
    s32 v15; // ecx
    wchar_t *v16; // eax
    bool v17; // zf
    s32 *v18; // eax
    s32 v19; // ecx
    wchar_t *v20; // eax
    s32 v21; // edx
    laughing_blackwell_0x1EB54 *v22; // ecx
    s32 v23; // ecx
    wchar_t *v24; // eax
    laughing_blackwell_0x1EB54 *v25; // ecx
    s32 *v26; // eax
    s32 v27; // ecx
    wchar_t *pNo; // eax
    s32 v29; // esi
    s32 field_110_state; // edx
    s32 fp_v4; // [esp-18h] [ebp-18h] BYREF
    s32 fp_300; // [esp-14h] [ebp-14h] BYREF
    laughing_blackwell_0x1EB54 *fp_320; // [esp-10h] [ebp-10h] BYREF
    s32 flags; // [esp-Ch] [ebp-Ch]
    s32 v35; // [esp-8h] [ebp-8h] BYREF
    s32 v36; // [esp-4h] [ebp-4h]
    */

    field_110_state = this->field_110_state;

    switch (field_110_state)
    {
        case 1:
        case 3:
        case 5:
            if (this->field_132_f136_idx == 9)
            {
                sub_4B7AE0();
            }
            else
            {
                sub_4AD140();
            }
            break;

        case 2:
            /*
        v36 = v29;
        v4 = laughing_blackwell_0x1EB54::sub_4B0190(this, gText_0x14_704DFC->Find_5B5F90("loading"), -1, 320);
        HIWORD(v6) = v5;
        flags = 0x4000;
        LOWORD(v6) = this->field_11C;
        fp_320 = v6;
        fp_300 = v7;
        FP::FromInt_4369F0(&fp_300, 260);
        fp_v4 = v8;
        FP::sub_4AE970(&fp_v4, v4);
        sub_4B87A0(gText_0x14_704DFC->Find_5B5F90("loading"), fp_v4, fp_300, fp_320, flags);
        */
            break;

        case 4:
            /*
        v36 = v29;
        if (this->field_EE0C == 1)
        {
            v35 = (int)this;
            HIWORD(v12) = v11;
            v13 = gText_0x14_704DFC->field_10_lang_code - 106;
            v35 = 0x4000;
            LOWORD(v12) = this->field_126;
            v14 = -(v13 != 0);
            flags = (int)v12;
            LOBYTE(v14) = v14 & 0xFC;
            fp_320 = v12;
            FP::sub_4AE970(&fp_320, v14 + 16);
            fp_300 = v15;
            FP::sub_4AE970(&fp_300, 0x113u);
            v16 = text_0x14::Find_5B5F90(gText_0x14_704DFC, aClrchar);
            sub_4B87A0(v16, fp_300, (int)fp_320, (void *)flags, v35);
        }
        v17 = this->field_EE0A == 190;
        v18 = &v35;
        v35 = 0x4000;
        if (v17)
        {
            LOWORD(field_110_state) = this->field_120;
            flags = field_110_state;
        }
        else
        {
            LOWORD(v18) = this->field_11C;
            flags = (int)v18;
        }
        fp_320 = this;
        FP::FromInt_4369F0(&fp_320, 190);
        fp_300 = v19;
        FP::FromInt_4369F0(&fp_300, 300);
        v20 = text_0x14::Find_5B5F90(gText_0x14_704DFC, "sure");
        sub_4B87A0(v20, fp_300, (int)fp_320, (void *)flags, v35);
        v17 = this->field_EE0A == 210;
        v35 = 0x4000;
        if (v17)
        {
            LOWORD(v22) = this->field_120;
            flags = (int)v22;
        }
        else
        {
            LOWORD(v21) = this->field_11C;
            flags = v21;
        }
        fp_320 = v22;
        FP::FromInt_4369F0(&fp_320, 210);
        fp_300 = v23;
        FP::FromInt_4369F0(&fp_300, 300);
        v24 = text_0x14::Find_5B5F90(gText_0x14_704DFC, "yes");
        sub_4B87A0(v24, fp_300, (int)fp_320, (void *)flags, v35);
        v17 = this->field_EE0A == 230;
        v26 = &v35;
        v35 = 0x4000;
        if (v17)
        {
            LOWORD(v26) = this->field_120;
            flags = (int)v26;
        }
        else
        {
            LOWORD(v25) = this->field_11C;
            flags = (int)v25;
        }
        fp_320 = v25;
        FP::FromInt_4369F0(&fp_320, 230);
        fp_300 = v27;
        FP::FromInt_4369F0(&fp_300, 300);
        sub_4B87A0(gText_0x14_704DFC->Find_5B5F90("no"), fp_300, (int)fp_320, (void *)flags, v35);
        */
            break;

        default:
            FatalError_4A38C0(1006, "C:\\Splitting\\GTA2\\Source\\frontend2.cpp", 1217, this->field_110_state);
    }
}

STUB_FUNC(0x5D7D30)
EXPORT void __stdcall sub_5D7D30()
{
    Vid_GetSurface(gVidSys_7071D0);
    MakeScreenTable((int)gVidSys_7071D0->field_50_surface_pixels_ptr,
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

    gbh_SetWindow(0, 0, dword_70675C, dword_70679C);
}

MATCH_FUNC(0x5D7DC0)
EXPORT void __cdecl FreeSurface_5D7DC0()
{
    Vid_FreeSurface(gVidSys_7071D0);
}

MATCH_FUNC(0x4ADFB0)
void laughing_blackwell_0x1EB54::sub_4ADFB0()
{
    sub_5D7D30();

    gbh_BeginScene();
    DrawBackground_4B6E10();
    sub_4ADF50();
    gbh_EndScene();

    FreeSurface_5D7DC0();

    Vid_FlipBuffers(gVidSys_7071D0);

    Vid_ClearScreen(gVidSys_7071D0, 0, 0, 0, 0, 0, gVidSys_7071D0->field_48_rect_right, gVidSys_7071D0->field_4C_rect_bottom);
}

// https://decomp.me/scratch/IOmk7
// TODO: stop the tail merge... somehow
STUB_FUNC(0x4B6E10)
void laughing_blackwell_0x1EB54::DrawBackground_4B6E10()
{
    // todo
    BYTE tga_idx; // [esp+50h] [ebp-8h] BYREF
    BYTE not_used; // [esp+54h] [ebp-4h] BYREF

    if (field_EE08 == GameOver_13 || field_EE08 == RedBar_16 || field_EE08 == BlueBar_14 || field_EE08 == Loading_15 ||
        field_EE08 == HiScoresDisplay_12 || field_EE08 == Credits_17)
    {
        sub_4B6B00(field_EE08, &tga_idx, &not_used);
        s32 blitRet = gbh_BlitImage(tgaArray_61F0C8[tga_idx].field_84_img, 0, 0, 640, 480, 0, 0);
        if (blitRet == -10)
        {
            // need to reload image
            Load_tga_4B6520(tga_idx);
            gbh_BlitImage(tgaArray_61F0C8[tga_idx].field_84_img, 0, 0, 640, 480, 0, 0);
        }
    }
    else
    {
        sub_4B6B00(field_EE08, &tga_idx, &not_used);

        // Left side
        s32 blitRet = gbh_BlitImage(tgaArray_61F0C8[tga_idx].field_84_img, 0, 0, 278, 480, 0, 0);
        if (blitRet == -10)
        {
            Load_tga_4B6520(tga_idx);
            blitRet = gbh_BlitImage(tgaArray_61F0C8[tga_idx].field_84_img, 0, 0, 278, 480, 0, 0);
        }

        // Right side
        if (blitRet == 0)
        {
            blitRet = gbh_BlitImage(tgaArray_61F0C8[not_used].field_84_img, 0, 0, 362, 480, 278, 0);
            if (blitRet == -10)
            {
                Load_tga_4B6520(not_used);
                gbh_BlitImage(tgaArray_61F0C8[not_used].field_84_img, 0, 0, 362, 480, 278, 0);
            }
        }
    }
}

MATCH_FUNC(0x4B6B00)
void laughing_blackwell_0x1EB54::sub_4B6B00(u8 a1, BYTE* pTgaIdx, BYTE* a3)
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
void laughing_blackwell_0x1EB54::Load_tga_4B6520(u16 idx)
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

    tgaArray_61F0C8[idx].field_84_img = gbh_LoadImage((SImage*)pAlloc);

    crt::fclose(hFile);
    free(pAlloc);
}

STUB_FUNC(0x5D7CB0)
EXPORT void __stdcall sub_5D7CB0()
{
    // todo: move me
}

STUB_FUNC(0x4AF2A0)
laughing_blackwell_0x1EB54::laughing_blackwell_0x1EB54()
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

    gbh_SetAmbient(1.0);

    if (gMagical_germain_0x8EC_6F5168)
    {
        gMagical_germain_0x8EC_6F5168->sub_4D2B40();
    }

    field_110_state = 1;
    field_114 = 0;
    field_118 = 0;
    field_C9D5 = 0;
    field_C9D6 = 0;
    field_C9D3 = 0;
    field_C9D4 = 0;
    field_C9D7 = 0;
    field_C9D8 = 0;
    field_C9D9 = 0;
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

    memset(&field_C9A0, 0, sizeof(field_C9A0));
    /*
    *(_DWORD *)this->field_C9A0 = 0;
    *(_DWORD *)&this->field_C9A0[2] = 0;
    *(_DWORD *)&this->field_C9A0[4] = 0;
    *(_DWORD *)&this->field_C9A0[6] = 0;
    this->field_C9A0[8] = 0;
    */

    memset(&field_C9B8, 0, sizeof(field_C9B8));
    /*
    *(_DWORD *)this->field_C9B8 = 0;
    *(_DWORD *)&this->field_C9B8[2] = 0;
    *(_DWORD *)&this->field_C9B8[4] = 0;
    *(_DWORD *)&this->field_C9B8[6] = 0;
    *(_WORD *)&this->field_C9C8 = 0;
    */
    field_C9CA = 0;
    field_C9CB = 0;
    field_1EB50_idx = 0;

    field_1EB51_blocks[0] = 0;
    field_1EB51_blocks[1] = 0;
    field_1EB51_blocks[2] = 0;

    sub_4B4440();
    LoadPlySlotSvgs_4B53C0();

    field_EE08 = Play_1;

    Load_tgas_4B66B0();

    field_EE0D = 0;
    field_EE0A = 190;
    field_EE0C = 0;
    field_1EB30 = 0;
    field_1EB38 = 0;
    field_1EB34 = dword_67D930;
    field_1EB4A = 0;
    field_1EB4B = 0;
    field_1EB4C = 0;
    field_1EB4D = 0;
    field_1EB4E = 0;
    field_1EB4F = 0;

    // todo: fix me
    /*
    for (s32 i = 0; i < 8; i++)
    {

    }

    v11 = &this->field_1EB42;
    v12 = 8;
    do
    {
        *((_BYTE *)v11 - 8) = -1;
        *(_BYTE *)v11 = -1;
        v11 = (s16 *)((char_type *)v11 + 1);
        --v12;
    } while (v12);
    */
}

STUB_FUNC(0x4AF970)
laughing_blackwell_0x1EB54::~laughing_blackwell_0x1EB54()
{
    FreeKeyBoardDevice_4AFD00();

    if (gSharp_pare_0x15D8_705064)
    {
        delete gSharp_pare_0x15D8_705064;
        gSharp_pare_0x15D8_705064 = 0;
    }

    if (gGtx_0x106C_703DD4)
    {
        delete gGtx_0x106C_703DD4;
        gGtx_0x106C_703DD4 = 0;
    }

    if (gText_0x14_704DFC)
    {
        delete gText_0x14_704DFC;
        gText_0x14_704DFC = 0;
    }

    if (gMagical_germain_0x8EC_6F5168)
    {
        delete gMagical_germain_0x8EC_6F5168;
        gMagical_germain_0x8EC_6F5168 = 0;
    }

    FreeImageTable_4B6750();
}

MATCH_FUNC(0x4AFD70)
void laughing_blackwell_0x1EB54::AcquireKeyBoard_4AFD70()
{
    LPDIRECTINPUTDEVICEA field_4_pKeyboardDevice; // eax

    field_4_pKeyboardDevice = this->field_4_pKeyboardDevice;
    if (!field_4_pKeyboardDevice || field_4_pKeyboardDevice->Acquire() < 0)
    {
        this->field_10C_bKeyboardAcquired = 1;
    }
}

MATCH_FUNC(0x4AFD00)
void laughing_blackwell_0x1EB54::FreeKeyBoardDevice_4AFD00()
{
    if (field_4_pKeyboardDevice)
    {
        field_4_pKeyboardDevice->Unacquire();
        field_4_pKeyboardDevice->Release();
        field_4_pKeyboardDevice = 0;
    }
}

MATCH_FUNC(0x4B6750)
void laughing_blackwell_0x1EB54::FreeImageTable_4B6750()
{
    gbh_FreeImageTable();
}

MATCH_FUNC(0x4AFDD0)
char_type laughing_blackwell_0x1EB54::KeyBoard_GetKeyStates_4AFDD0()
{
    HRESULT hr = this->field_4_pKeyboardDevice->GetDeviceState(256, field_8_keys);
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
void laughing_blackwell_0x1EB54::InitKeyBoardDevice_4AFBE0()
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

STUB_FUNC(0x4AF0E0)
void laughing_blackwell_0x1EB54::sub_4AF0E0()
{
    // todo
}

STUB_FUNC(0x4B0220)
void laughing_blackwell_0x1EB54::sub_4B0220()
{
    s16 v30; // ax

    // local_4 = (-(ushort)(cVar1 != 'j') & 0xfffc) + 0x10;

    s32 v2 = gText_0x14_704DFC->field_10_lang_code != 'j' ? 12 : 16;
    this->field_134 = 16;

    this->field_136[0].field_0 = 3;
    this->field_136[0].field_4[0].field_0 = 1; // ebx
    this->field_136[0].field_4[0].field_2 = 300; // edi
    this->field_136[0].field_4[0].field_4 = 250;
    wcsncpy(this->field_136[0].field_4[0].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("play"), 0x32u);
    this->field_136[0].field_4[0].field_80 = 1;
    this->field_136[0].field_4[1].field_0 = 1;
    this->field_136[0].field_4[1].field_2 = 300;
    this->field_136[0].field_4[1].field_4 = 270;
    wcsncpy(this->field_136[0].field_4[1].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("options"), 0x32u);
    this->field_136[0].field_4[1].field_80 = 257;
    this->field_136[0].field_4[2].field_0 = 1;
    this->field_136[0].field_4[2].field_2 = 300;
    this->field_136[0].field_4[2].field_4 = 290;
    wcsncpy(this->field_136[0].field_4[2].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("quit"), 0x32u);
    this->field_136[0].field_4[2].field_80 = 9;
    this->field_136[0].field_B8A[0].field_0 = 280;
    this->field_136[0].field_B8A[0].field_2 = 258;
    this->field_136[0].field_B8A[1].field_0 = 280;
    this->field_136[0].field_B8A[1].field_2 = 278;
    this->field_136[0].field_B8A[2].field_0 = 280;
    this->field_136[0].field_B8A[2].field_2 = 298;
    this->field_136[0].field_BC6_nifty_idx = 0;
    this->field_136[0].field_BC8 = 0;
    this->field_136[1].field_0 = 5;
    this->field_136[1].field_4[0].field_0 = 2;
    this->field_136[1].field_4[0].field_2 = 300;
    this->field_136[1].field_4[0].field_4 = 210;
    wcsncpy(this->field_136[1].field_4[0].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("charctr"), 0x32u);
    this->field_136[1].field_4[0].field_6E_count = 0;
    this->field_136[1].field_4[0].field_70 = 0;
    this->field_136[1].field_4[0].field_7E = 7;

    u16 v77 = 0;
    do
    {
        this->field_136[1].field_4[0].field_72[v77++] = 1;
    } while (v77 <= this->field_136[1].field_4[0].field_7E);

    this->field_136[1].field_4[1].field_0 = 1;
    this->field_136[1].field_4[1].field_2 = 300;
    this->field_136[1].field_4[1].field_4 = 230;
    wcsncpy(this->field_136[1].field_4[1].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("savepos"), 0x32u);
    this->field_136[1].field_4[1].field_80 = 260;
    this->field_136[1].field_4[2].field_0 = 1;
    this->field_136[1].field_4[2].field_2 = 300;
    this->field_136[1].field_4[2].field_4 = 250;
    wcsncpy(this->field_136[1].field_4[2].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("hi_scre"), 0x32u);
    this->field_136[1].field_4[2].field_80 = 5;
    this->field_136[1].field_4[3].field_0 = 1;
    this->field_136[1].field_4[3].field_2 = 300;
    this->field_136[1].field_4[3].field_4 = 270;
    wcsncpy(this->field_136[1].field_4[3].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("strtlev"), 0x32u);
    this->field_136[1].field_4[3].field_80 = 264;
    this->field_136[1].field_4[4].field_0 = 1;
    this->field_136[1].field_4[4].field_2 = 300;
    this->field_136[1].field_4[4].field_4 = 350;
    wcsncpy(this->field_136[1].field_4[4].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("bonslev"), 0x32u);
    this->field_136[1].field_4[4].field_80 = 265;
    this->field_136[1].field_B8A[0].field_0 = 280;
    this->field_136[1].field_B8A[0].field_2 = 218;
    this->field_136[1].field_B8A[1].field_0 = 280;
    this->field_136[1].field_B8A[1].field_2 = 238;
    this->field_136[1].field_B8A[2].field_0 = 280;
    this->field_136[1].field_B8A[2].field_2 = 258;
    this->field_136[1].field_B8A[3].field_0 = 280;
    this->field_136[1].field_B8A[3].field_2 = 278;
    this->field_136[1].field_B8A[4].field_0 = 280;
    this->field_136[1].field_B8A[4].field_2 = 358;
    this->field_136[1].field_BC6_nifty_idx = 3;
    this->field_136[1].field_BC8 = 3;
    this->field_136[1].field_2 = 10;
    this->field_136[1].field_518[0].field_0 = 3;
    this->field_136[1].field_518[0].field_2 = 420;
    this->field_136[1].field_518[0].field_4 = 310;
    this->field_136[1].field_518[0].field_6_wstr_buf[0] = 0;
    this->field_136[1].field_518[1].field_0 = 3;
    this->field_136[1].field_518[1].field_2 = 420;
    this->field_136[1].field_518[1].field_4 = 390;
    this->field_136[1].field_518[1].field_6_wstr_buf[0] = 0;
    this->field_136[1].field_518[2].field_0 = 1;
    this->field_136[1].field_518[2].field_2 = 410;
    this->field_136[1].field_518[2].field_4 = 298;
    wcsncpy(this->field_136[1].field_518[2].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("car_dam"), 0x32u);
    this->field_136[1].field_518[2].field_6A = word_703C3C;
    this->field_136[1].field_518[3].field_0 = 1;
    this->field_136[1].field_518[3].field_2 = 410;
    this->field_136[1].field_518[3].field_4 = 378;
    wcsncpy(this->field_136[1].field_518[3].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("car_dam"), 0x32u);
    this->field_136[1].field_518[3].field_6A = word_703C3C;
    this->field_136[1].field_518[4].field_2 = 380;
    this->field_136[1].field_518[4].field_4 = 310;
    this->field_136[1].field_518[5].field_2 = 460;
    this->field_136[1].field_518[5].field_4 = 310;
    this->field_136[1].field_518[6].field_2 = 380;
    this->field_136[1].field_518[7].field_2 = 460;
    this->field_136[1].field_518[4].field_0 = 3;
    this->field_136[1].field_518[4].field_6_wstr_buf[0] = 3;
    this->field_136[1].field_518[5].field_0 = 3;
    this->field_136[1].field_518[5].field_6_wstr_buf[0] = 4;
    this->field_136[1].field_518[6].field_0 = 3;
    this->field_136[1].field_518[6].field_4 = 390;
    this->field_136[1].field_518[6].field_6_wstr_buf[0] = 3;
    this->field_136[1].field_518[7].field_0 = 3;
    this->field_136[1].field_518[7].field_4 = 390;
    this->field_136[1].field_518[7].field_6_wstr_buf[0] = 4;
    this->field_136[1].field_518[8].field_0 = 3;
    this->field_136[1].field_518[8].field_2 = 290;
    this->field_136[1].field_518[8].field_4 = 222;
    this->field_136[1].field_518[8].field_6_wstr_buf[0] = 3;
    this->field_136[1].field_518[9].field_0 = 3;
    this->field_136[1].field_518[9].field_2 = 580;
    this->field_136[1].field_518[9].field_4 = 222;
    this->field_136[1].field_518[9].field_6_wstr_buf[0] = 4;
    this->field_136[11].field_0 = 3;
    this->field_136[11].field_2 = 1;
    this->field_136[11].field_518[0].field_0 = 1;
    this->field_136[11].field_518[0].field_2 = 35;
    this->field_136[11].field_518[0].field_4 = 11;
    wcscpy(this->field_136[11].field_518[0].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("plr_qut"));
    this->field_136[11].field_518[0].field_6A = this->field_130;
    this->field_136[11].field_518[0].field_6C = 5;
    this->field_136[11].field_4[0].field_0 = 1;
    this->field_136[11].field_4[0].field_4 = 392;
    wcsncpy(this->field_136[11].field_4[0].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("savepos"), 0x32u);
    this->field_136[11].field_4[0].field_2 = laughing_blackwell_0x1EB54::sub_4B0190(this->field_136[11].field_4[0].field_6_wstr_buf,
                                                                                    this->field_136[11].field_4[0].field_6A,
                                                                                    320);
    this->field_136[11].field_4[0].field_80 = 260;
    this->field_136[11].field_4[1].field_0 = 1;
    this->field_136[11].field_4[1].field_4 = 412;
    wcsncpy(this->field_136[11].field_4[1].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("replay"), 0x32u);
    this->field_136[11].field_4[1].field_2 = laughing_blackwell_0x1EB54::sub_4B0190(this->field_136[11].field_4[1].field_6_wstr_buf,
                                                                                    this->field_136[11].field_4[1].field_6A,
                                                                                    320);
    this->field_136[11].field_4[1].field_80 = 259;
    this->field_136[11].field_4[2].field_0 = 1;
    this->field_136[11].field_4[2].field_4 = 432;
    wcsncpy(this->field_136[11].field_4[2].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("mainmen"), 0x32u);
    this->field_136[11].field_4[2].field_2 = laughing_blackwell_0x1EB54::sub_4B0190(this->field_136[11].field_4[2].field_6_wstr_buf,
                                                                                    this->field_136[11].field_4[2].field_6A,
                                                                                    320);
    this->field_136[11].field_4[2].field_80 = 0;
    this->field_136[11].field_B8A[0].field_0 = 150;
    this->field_136[11].field_B8A[0].field_2 = 400;
    this->field_136[11].field_B8A[1].field_0 = 150;
    this->field_136[11].field_B8A[1].field_2 = 420;
    this->field_136[11].field_B8A[2].field_0 = 150;
    this->field_136[11].field_B8A[2].field_2 = 440;
    this->field_136[11].field_BC6_nifty_idx = 0;
    this->field_136[11].field_BC8 = 0;
    this->field_136[2].field_0 = 3;
    this->field_136[2].field_2 = 1;
    this->field_136[2].field_518[0].field_0 = 1;
    this->field_136[2].field_518[0].field_2 = 35;
    this->field_136[2].field_518[0].field_4 = 11;
    wcsncpy(this->field_136[2].field_518[0].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("plr_ded"), 0x32u);
    this->field_136[2].field_518[0].field_6A = this->field_130;
    this->field_136[2].field_518[0].field_6C = 0;
    this->field_136[2].field_4[0].field_0 = 1;
    this->field_136[2].field_4[0].field_4 = 392;
    wcsncpy(this->field_136[2].field_4[0].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("savepos"), 0x32u);
    this->field_136[2].field_4[0].field_2 =
        laughing_blackwell_0x1EB54::sub_4B0190(this->field_136[2].field_4[0].field_6_wstr_buf, this->field_136[2].field_4[0].field_6A, 320);
    this->field_136[2].field_4[0].field_80 = 260;
    this->field_136[2].field_4[1].field_0 = 1;
    this->field_136[2].field_4[1].field_4 = 412;
    wcsncpy(this->field_136[2].field_4[1].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("replay"), 0x32u);
    this->field_136[2].field_4[1].field_2 =
        laughing_blackwell_0x1EB54::sub_4B0190(this->field_136[2].field_4[1].field_6_wstr_buf, this->field_136[2].field_4[1].field_6A, 320);
    this->field_136[2].field_4[1].field_80 = 259;
    this->field_136[2].field_4[2].field_0 = 1;
    this->field_136[2].field_4[2].field_4 = 432;
    wcsncpy(this->field_136[2].field_4[2].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("mainmen"), 0x32u);
    this->field_136[2].field_4[2].field_2 =
        laughing_blackwell_0x1EB54::sub_4B0190(this->field_136[2].field_4[2].field_6_wstr_buf, this->field_136[2].field_4[2].field_6A, 320);
    this->field_136[2].field_4[2].field_80 = 0;
    this->field_136[2].field_B8A[0].field_0 = 150;
    this->field_136[2].field_B8A[0].field_2 = 400;
    this->field_136[2].field_B8A[1].field_0 = 150;
    this->field_136[2].field_B8A[1].field_2 = 420;
    this->field_136[2].field_B8A[2].field_0 = 150;
    this->field_136[2].field_B8A[2].field_2 = 440;
    this->field_136[2].field_BC6_nifty_idx = 0;
    this->field_136[2].field_BC8 = 0;
    this->field_136[3].field_0 = 5;
    this->field_136[3].field_2 = 1;
    this->field_136[3].field_518[0].field_0 = 1;
    this->field_136[3].field_518[0].field_2 = 35;
    this->field_136[3].field_518[0].field_4 = 11;
    wcsncpy(this->field_136[3].field_518[0].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("cmpltd"), 0x32u);
    this->field_136[3].field_518[0].field_6A = this->field_12C;
    this->field_136[3].field_4[0].field_0 = 1;
    this->field_136[3].field_4[0].field_4 = 365;
    wcsncpy(this->field_136[3].field_4[0].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("nxt_lvl"), 0x32u);
    this->field_136[3].field_4[0].field_2 =
        laughing_blackwell_0x1EB54::sub_4B0190(this->field_136[3].field_4[0].field_6_wstr_buf, this->field_136[3].field_4[0].field_6A, 320);
    this->field_136[3].field_4[0].field_80 = 261;
    this->field_136[3].field_4[1].field_0 = 1;
    this->field_136[3].field_4[1].field_4 = 385;
    wcsncpy(this->field_136[3].field_4[1].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("savepos"), 0x32u);
    this->field_136[3].field_4[1].field_2 =
        laughing_blackwell_0x1EB54::sub_4B0190(this->field_136[3].field_4[1].field_6_wstr_buf, this->field_136[3].field_4[1].field_6A, 320);
    this->field_136[3].field_4[1].field_80 = 260;
    this->field_136[3].field_4[2].field_0 = 1;
    this->field_136[3].field_4[2].field_4 = 405;
    wcsncpy(this->field_136[3].field_4[2].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("replay"), 0x32u);
    this->field_136[3].field_4[2].field_2 =
        laughing_blackwell_0x1EB54::sub_4B0190(this->field_136[3].field_4[2].field_6_wstr_buf, this->field_136[3].field_4[2].field_6A, 320);
    this->field_136[3].field_4[2].field_80 = 259;
    this->field_136[3].field_4[3].field_0 = 1;
    this->field_136[3].field_4[3].field_4 = 425;
    wcsncpy(this->field_136[3].field_4[3].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("contnue"), 0x32u);
    this->field_136[3].field_4[3].field_2 =
        laughing_blackwell_0x1EB54::sub_4B0190(this->field_136[3].field_4[3].field_6_wstr_buf, this->field_136[3].field_4[3].field_6A, 320);
    this->field_136[3].field_4[3].field_80 = 266;
    this->field_136[3].field_4[4].field_0 = 1;
    this->field_136[3].field_4[4].field_4 = 445;
    wcsncpy(this->field_136[3].field_4[4].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("mainmen"), 0x32u);
    this->field_136[3].field_4[4].field_2 =
        laughing_blackwell_0x1EB54::sub_4B0190(this->field_136[3].field_4[4].field_6_wstr_buf, this->field_136[3].field_4[4].field_6A, 320);
    this->field_136[3].field_4[4].field_80 = 0;
    this->field_136[3].field_B8A[0].field_0 = 150;
    this->field_136[3].field_B8A[0].field_2 = 373;
    this->field_136[3].field_B8A[1].field_0 = 150;
    this->field_136[3].field_B8A[1].field_2 = 393;
    this->field_136[3].field_B8A[2].field_0 = 150;
    this->field_136[3].field_B8A[2].field_2 = 413;
    this->field_136[3].field_B8A[3].field_0 = 150;
    this->field_136[3].field_B8A[3].field_2 = 433;
    this->field_136[3].field_B8A[4].field_0 = 150;
    this->field_136[3].field_B8A[4].field_2 = 453;
    this->field_136[3].field_BC6_nifty_idx = 0;
    this->field_136[3].field_BC8 = 0;
    this->field_136[4].field_0 = 1;
    this->field_136[4].field_2 = 1;
    this->field_136[4].field_518[0].field_0 = 1;
    this->field_136[4].field_518[0].field_4 = 230;
    wcsncpy(this->field_136[4].field_518[0].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("gam_cmp"), 0x32u);
    v30 = this->field_130;
    this->field_136[4].field_518[0].field_6A = v30;
    this->field_136[4].field_518[0].field_2 =
        laughing_blackwell_0x1EB54::sub_4B0190(this->field_136[4].field_518[0].field_6_wstr_buf, v30, 320);
    this->field_136[4].field_518[0].field_6C = 4;
    this->field_136[4].field_4[0].field_0 = 1;
    this->field_136[4].field_4[0].field_2 = 180;
    this->field_136[4].field_4[0].field_4 = 410;
    wcsncpy(this->field_136[4].field_4[0].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("mainmen"), 0x32u);
    this->field_136[4].field_4[0].field_80 = 0;
    this->field_136[4].field_B8A[0].field_0 = 160;
    this->field_136[4].field_B8A[0].field_2 = 418;
    this->field_136[4].field_BC6_nifty_idx = 0;
    this->field_136[4].field_BC8 = 0;
    this->field_136[5].field_0 = 1;
    this->field_136[5].field_2 = 5;
    this->field_136[5].field_4[0].field_0 = 2;
    this->field_136[5].field_4[0].field_2 = 300;
    this->field_136[5].field_4[0].field_4 = 155;
    this->field_136[5].field_4[0].field_6E_count = 0;
    this->field_136[5].field_4[0].field_70 = 0;
    this->field_136[5].field_4[0].field_7E = 11;

    u16 v323 = 0;
    do
    {
        this->field_136[5].field_4[0].field_72[v323++] = 1;
    } while (v323 <= field_136[5].field_4[0].field_7E);

    this->field_136[5].field_B8A[0].field_0 = 280;
    this->field_136[5].field_B8A[0].field_2 = 163;
    this->field_136[5].field_BC6_nifty_idx = 0;
    this->field_136[5].field_BC8 = 0;
    this->field_136[5].field_518[0].field_0 = 3;
    this->field_136[5].field_518[0].field_2 = 450;
    this->field_136[5].field_518[0].field_4 = 197;
    this->field_136[5].field_518[0].field_6_wstr_buf[0] = 0;
    this->field_136[5].field_518[1].field_0 = 1;
    this->field_136[5].field_518[1].field_2 = 440;
    this->field_136[5].field_518[1].field_4 = 185;
    //    v34 = ;
    this->field_136[5].field_518[2].field_4 = 197;
    this->field_136[5].field_518[3].field_4 = 197;
    this->field_136[5].field_518[1].field_6A = word_703C3C; // v34
    this->field_136[5].field_518[2].field_0 = 3;
    this->field_136[5].field_518[2].field_2 = 410;
    this->field_136[5].field_518[2].field_6_wstr_buf[0] = 3;
    this->field_136[5].field_518[3].field_0 = 3;
    this->field_136[5].field_518[3].field_2 = 490;
    this->field_136[5].field_518[3].field_6_wstr_buf[0] = 4;
    this->field_136[5].field_518[4].field_0 = 1;
    this->field_136[5].field_518[4].field_2 = 340;
    this->field_136[5].field_518[4].field_4 = v2;
    wcsncpy(this->field_136[5].field_518[4].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("hi_scre"), 0x32u);
    this->field_136[5].field_518[4].field_6A = this->field_126;
    this->field_136[6].field_0 = 3;
    this->field_136[6].field_2 = 3;
    this->field_136[6].field_518[0].field_0 = 1;
    this->field_136[6].field_518[0].field_2 = 35;
    this->field_136[6].field_518[0].field_4 = 11;
    wcsncpy(this->field_136[6].field_518[0].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("bonslev"), 0x32u);
    this->field_136[6].field_518[0].field_6A = this->field_130;
    this->field_136[6].field_518[0].field_6C = 5;
    this->field_136[6].field_518[1].field_0 = 1;
    this->field_136[6].field_518[1].field_2 = 170;
    this->field_136[6].field_518[1].field_4 = 250;
    wcsncpy(this->field_136[6].field_518[1].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("score"), 0x32u);
    this->field_136[6].field_518[2].field_0 = 1;
    this->field_136[6].field_518[2].field_2 = 400;
    this->field_136[6].field_518[2].field_4 = 250;
    s16 v38 = this->field_120;
    this->field_136[6].field_4[0].field_0 = 1;
    this->field_136[6].field_518[2].field_6A = v38;
    this->field_136[6].field_4[0].field_4 = 340;
    wcsncpy(this->field_136[6].field_4[0].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("repbons"), 0x32u);
    this->field_136[6].field_4[0].field_2 =
        laughing_blackwell_0x1EB54::sub_4B0190(this->field_136[6].field_4[0].field_6_wstr_buf, v38, 320);
    this->field_136[6].field_4[0].field_80 = 259;
    this->field_136[6].field_4[1].field_0 = 1;
    this->field_136[6].field_4[1].field_4 = 360;
    wcsncpy(this->field_136[6].field_4[1].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("nxt_lvl"), 0x32u);
    this->field_136[6].field_4[1].field_2 =
        laughing_blackwell_0x1EB54::sub_4B0190(this->field_136[6].field_4[1].field_6_wstr_buf, this->field_136[6].field_4[1].field_6A, 320);
    this->field_136[6].field_4[1].field_80 = 261;
    this->field_136[6].field_4[2].field_0 = 1;
    this->field_136[6].field_4[2].field_4 = 380;
    wcsncpy(this->field_136[6].field_4[2].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("mainmen"), 0x32u);
    this->field_136[6].field_4[2].field_2 =
        laughing_blackwell_0x1EB54::sub_4B0190(this->field_136[6].field_4[2].field_6_wstr_buf, this->field_136[6].field_4[2].field_6A, 320);
    this->field_136[6].field_4[2].field_80 = 0;
    this->field_136[6].field_B8A[0].field_0 = 150;
    this->field_136[6].field_B8A[0].field_2 = 348;
    this->field_136[6].field_B8A[1].field_0 = 150;
    this->field_136[6].field_B8A[1].field_2 = 368;
    this->field_136[6].field_B8A[2].field_0 = 150;
    this->field_136[6].field_B8A[2].field_2 = 388;
    this->field_136[6].field_BC6_nifty_idx = 0;
    this->field_136[6].field_BC8 = 0;
    this->field_136[7].field_0 = 1;
    this->field_136[7].field_2 = 14;
    this->field_136[7].field_518[0].field_0 = 1;
    this->field_136[7].field_518[0].field_2 = 35;
    this->field_136[7].field_518[0].field_4 = 11;
    this->field_136[7].field_518[0].field_6A = this->field_130;
    this->field_136[7].field_518[0].field_6C = 5;
    this->field_136[7].field_518[1].field_0 = 1;
    this->field_136[7].field_518[1].field_2 = 100;
    this->field_136[7].field_518[1].field_4 = 170;
    wcsncpy(this->field_136[7].field_518[1].field_6_wstr_buf, word_67DC8C, 50u);
    this->field_136[7].field_518[2].field_0 = 1;
    this->field_136[7].field_518[2].field_2 = 100;
    this->field_136[7].field_518[2].field_4 = 190;
    wcsncpy(this->field_136[7].field_518[2].field_6_wstr_buf, word_67DC8C, 0x32u);
    this->field_136[7].field_518[3].field_0 = 1;
    this->field_136[7].field_518[3].field_2 = 100;
    this->field_136[7].field_518[3].field_4 = 210;
    wcsncpy(this->field_136[7].field_518[3].field_6_wstr_buf, word_67DC8C, 0x32u);
    this->field_136[7].field_518[4].field_0 = 1;
    this->field_136[7].field_518[4].field_2 = 100;
    this->field_136[7].field_518[4].field_4 = 230;
    wcsncpy(this->field_136[7].field_518[4].field_6_wstr_buf, word_67DC8C, 0x32u);
    this->field_136[7].field_518[5].field_0 = 1;
    this->field_136[7].field_518[5].field_2 = 100;
    this->field_136[7].field_518[5].field_4 = 250;
    wcsncpy(this->field_136[7].field_518[5].field_6_wstr_buf, word_67DC8C, 0x32u);
    this->field_136[7].field_518[6].field_0 = 1;
    this->field_136[7].field_518[6].field_2 = 100;
    this->field_136[7].field_518[6].field_4 = 270;
    wcsncpy(this->field_136[7].field_518[6].field_6_wstr_buf, word_67DC8C, 0x32u);
    this->field_136[7].field_518[7].field_0 = 1;
    this->field_136[7].field_518[7].field_4 = 300;
    wcsncpy(this->field_136[7].field_518[7].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("kills_h"), 0x32u);
    this->field_136[7].field_518[7].field_2 =
        sub_4B0190(this->field_136[7].field_518[7].field_6_wstr_buf, this->field_136[7].field_518[7].field_6A, 320);
    this->field_136[7].field_518[8].field_0 = 1;
    this->field_136[7].field_518[8].field_2 = 100;
    this->field_136[7].field_518[8].field_4 = 320;
    this->field_136[7].field_518[9].field_0 = 1;
    this->field_136[7].field_518[9].field_2 = 100;
    this->field_136[7].field_518[9].field_4 = 340;
    this->field_136[7].field_518[10].field_0 = 1;
    this->field_136[7].field_518[10].field_2 = 100;
    this->field_136[7].field_518[10].field_4 = 360;
    this->field_136[7].field_518[11].field_0 = 1;
    this->field_136[7].field_518[11].field_2 = 100;
    this->field_136[7].field_518[11].field_4 = 380;
    this->field_136[7].field_518[12].field_0 = 1;
    this->field_136[7].field_518[12].field_2 = 100;
    this->field_136[7].field_518[12].field_4 = 400;
    this->field_136[7].field_518[13].field_0 = 1;
    this->field_136[7].field_518[13].field_2 = 30;
    this->field_136[7].field_518[13].field_4 = 150;
    this->field_136[7].field_4[0].field_0 = 1;
    this->field_136[7].field_4[0].field_4 = 430;
    wcsncpy(this->field_136[7].field_4[0].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("quit"), 0x32u);
    this->field_136[7].field_4[0].field_2 =
        sub_4B0190(this->field_136[7].field_4[0].field_6_wstr_buf, this->field_136[7].field_4[0].field_6A, 320);
    this->field_136[7].field_4[0].field_80 = 258;
    this->field_136[7].field_B8A[0].field_0 = 180;
    this->field_136[7].field_B8A[0].field_2 = 438;
    this->field_136[7].field_BC6_nifty_idx = 0;
    this->field_136[7].field_BC8 = 0;
    this->field_136[8].field_0 = 1;
    this->field_136[8].field_2 = 0;
    this->field_136[8].field_4[0].field_0 = 1;
    this->field_136[8].field_4[0].field_2 = 200;
    this->field_136[8].field_4[0].field_4 = 280;
    wcsncpy(this->field_136[8].field_4[0].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("mainmen"), 0x32u);
    this->field_136[8].field_4[0].field_80 = 0;
    this->field_136[8].field_B8A[0].field_0 = 180;
    this->field_136[8].field_B8A[0].field_2 = 288;
    this->field_136[8].field_BC6_nifty_idx = 0;
    this->field_136[8].field_BC8 = 0;
    this->field_136[10].field_0 = 1;
    this->field_136[10].field_2 = 1;
    this->field_136[10].field_518[0].field_0 = 1;
    this->field_136[10].field_518[0].field_4 = 230;
    wcsncpy(this->field_136[10].field_518[0].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("nicetry"), 0x32u);
    //    v46 = this->field_130;
    this->field_136[10].field_518[0].field_6A = this->field_130; // v46;
    this->field_136[10].field_518[0].field_2 = sub_4B0190(this->field_136[10].field_518[0].field_6_wstr_buf,
                                                          this->field_130, //v46,
                                                          320);
    this->field_136[10].field_518[0].field_6C = 4;
    this->field_136[10].field_4[0].field_0 = 1;
    this->field_136[10].field_4[0].field_2 = 180;
    this->field_136[10].field_4[0].field_4 = 410;
    wcsncpy(this->field_136[10].field_4[0].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("mainmen"), 0x32u);
    this->field_136[10].field_4[0].field_80 = 0;
    this->field_136[10].field_B8A[0].field_0 = 160;
    this->field_136[10].field_B8A[0].field_2 = 418;
    this->field_136[10].field_BC6_nifty_idx = 0;
    this->field_136[10].field_BC8 = 0;
    this->field_136[14].field_0 = 1;
    this->field_136[14].field_2 = 5;
    this->field_136[14].field_4[0].field_0 = 1;
    this->field_136[14].field_4[0].field_2 = 170;
    this->field_136[14].field_4[0].field_4 = 340;
    this->field_136[14].field_4[0].field_80 = 268;
    this->field_136[14].field_518[0].field_0 = 1;
    this->field_136[14].field_518[0].field_2 = 20;
    this->field_136[14].field_518[0].field_4 = 160;
    wcsncpy(this->field_136[14].field_518[0].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("fr_ent1"), 0x32u);
    this->field_136[14].field_518[1].field_0 = 1;
    this->field_136[14].field_518[1].field_2 = 20;
    this->field_136[14].field_518[1].field_4 = 180;
    wcsncpy(this->field_136[14].field_518[1].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("fr_ent2"), 0x32u);
    this->field_136[14].field_518[2].field_0 = 1;
    this->field_136[14].field_518[2].field_2 = 20;
    this->field_136[14].field_518[2].field_4 = 200;
    wcsncpy(this->field_136[14].field_518[2].field_6_wstr_buf, word_67DC8C, 0x32u);
    this->field_136[14].field_518[3].field_0 = 1;
    this->field_136[14].field_518[3].field_2 = 20;
    this->field_136[14].field_518[3].field_4 = 300;
    wcsncpy(this->field_136[14].field_518[3].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("fr_pmpt"), 0x32u);
    this->field_136[14].field_518[4].field_0 = 1;
    this->field_136[14].field_518[4].field_2 = 20;
    this->field_136[14].field_518[4].field_4 = 320;
    wcsncpy(this->field_136[14].field_518[4].field_6_wstr_buf, gText_0x14_704DFC->Find_5B5F90("score"), 0x32u);
    this->field_136[14].field_B8A[0].field_0 = 150;
    this->field_136[14].field_B8A[0].field_2 = 348;
    this->field_136[14].field_BC6_nifty_idx = 0;
    this->field_136[14].field_BC8 = 0;
    field_EE0E_unk.sub_483F20();
}

STUB_FUNC(0x4B4440)
void laughing_blackwell_0x1EB54::sub_4B4440()
{
    // todo
}

STUB_FUNC(0x4B53C0)
void laughing_blackwell_0x1EB54::LoadPlySlotSvgs_4B53C0()
{
    char_type FileName[256];
    for (u32 i = 0; i < 8; i++)
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
void laughing_blackwell_0x1EB54::Load_tgas_4B66B0()
{
    if (gbh_InitImageTable(gTableSize_61FF20) != -1)
    {
        for (u16 i = 0; i < gTableSize_61FF20; ++i)
        {
            Load_tga_4B6520(i);
        }
    }
}

MATCH_FUNC(0x4B51D0)
void laughing_blackwell_0x1EB54::GetPlySlotSvgName_4B51D0(u8 idx, char_type* pStr)
{
    char_type Buffer[8];
    _itoa(idx, Buffer, 10);
    strcpy(pStr, "player\\plyslot");
    strcat(pStr, Buffer);
    strcat(pStr, ".svg");
}

MATCH_FUNC(0x4B5370)
char_type laughing_blackwell_0x1EB54::PlySlotSvgExists_4B5370(s32 idx)
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

EXPORT_VAR extern BYTE byte_6F4BF4;
EXPORT_VAR extern bool bDoFrontEnd_626B68;

STUB_FUNC(0x5E53C0)
void __stdcall laughing_blackwell_0x1EB54::sub_5E53C0(BYTE* a1)
{
    if (bDoFrontEnd_626B68)
    {
        if (gLaughing_blackwell_0x1EB54_67DC84)
        {
            gLaughing_blackwell_0x1EB54_67DC84->field_10D = *a1;
        }
    }
    else
    {
        byte_6F4BF4 = *a1;
    }
}

STUB_FUNC(0x5D8990)
s32 laughing_blackwell_0x1EB54::sub_5D8990(wchar_t* pStr, u16 a2)
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

STUB_FUNC(0x4B0190)
s32 laughing_blackwell_0x1EB54::sub_4B0190(wchar_t* a2, s16 a3, s32 a4)
{
    // todo
    /*
    s32 v4; // eax
    s32 v5; // ecx

    if (a3 == -1)
        v4 = sub_5D8990((s16 *)a2, this->field_11C);
    else
        v4 = sub_5D8990((s16 *)a2, a3);
    HIWORD(v5) = HIWORD(v4);
    LOWORD(v5) = (u16)v4 >> 1;
    return a4 - v5;
    */
    return 0;
}

STUB_FUNC(0x4B7060)
u8 laughing_blackwell_0x1EB54::sub_4B7060(u8 a2)
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
u8 laughing_blackwell_0x1EB54::sub_4B7270(char_type a2)
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

STUB_FUNC(0x4B7490)
bool laughing_blackwell_0x1EB54::sub_4B7490()
{
    u8 v2 = gLucid_hamilton_67E8E0.sub_4C5980();
    return sub_4B7060(v2) != v2;
}

STUB_FUNC(0x4B74C0)
bool laughing_blackwell_0x1EB54::sub_4B74C0()
{
    char_type v2 = gLucid_hamilton_67E8E0.sub_4C5980();
    return sub_4B7270(v2) != v2;
}

STUB_FUNC(0x4B7550)
void laughing_blackwell_0x1EB54::sub_4B7550()
{
    loving_borg_0xBCA* pBorg; // edi
    u8 v3; // [esp+Ch] [ebp-4h]

    pBorg = &this->field_136[this->field_132_f136_idx];
    v3 = gLucid_hamilton_67E8E0.sub_4C5980();
    swprintf(tmpBuff_67BD9C, L"%d", v3 + 1);
    wcsncpy(pBorg->field_518[2].field_6_wstr_buf, tmpBuff_67BD9C, 0x32u);

    if (sub_4B7490())
    {
        pBorg->field_518[4].field_1 = 1;
        this->field_1EB4C = 1;
    }
    else
    {
        pBorg->field_518[4].field_1 = 0;
        this->field_1EB4C = 0;
    }

    if (sub_4B74C0())
    {
        pBorg->field_518[5].field_1 = 1;
        this->field_1EB4D = 1;
    }
    else
    {
        pBorg->field_518[5].field_1 = 0;
        this->field_1EB4D = 0;
    }
}

STUB_FUNC(0x4B6FF0)
bool laughing_blackwell_0x1EB54::sub_4B6FF0()
{
    u8 v3 = gLucid_hamilton_67E8E0.sub_4C5980();
    u8 a2 = sub_4B7060(v3);
    gLucid_hamilton_67E8E0.sub_4C58F0(a2);
    field_1EB3A[gLucid_hamilton_67E8E0.sub_4C59B0()] = a2;
    sub_4B7550();
    return v3 != a2;
}

STUB_FUNC(0x4B42B0)
void laughing_blackwell_0x1EB54::sub_4B42B0()
{
    s32 v1; // eax
    wchar_t* v2; // edi
    u32 v3; // ecx
    wchar_t* v4; // edi
    s32 i; // ecx

    if ((unsigned __int8)this->field_C9B2 < 9u)
    {
        v1 = (unsigned __int8)this->field_C9B2;
        v2 = &this->field_C9A0[v1];
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
char_type laughing_blackwell_0x1EB54::sub_4B7120(char_type a2)
{
    // todo
    return 0;
}

STUB_FUNC(0x4B7610)
void laughing_blackwell_0x1EB54::sub_4B7610()
{
    loving_borg_0xBCA* pItem; // esi
    u8 v3; // al
    u8 v4; // [esp+8h] [ebp-8h]
    u8 v5; // [esp+Ch] [ebp-4h]

    pItem = &this->field_136[this->field_132_f136_idx];
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
            this->field_1EB4E = 1;
        }
        else
        {
            pItem->field_518[6].field_1 = 0;
            this->field_1EB4E = 0;
        }

        if (sub_4B7520())
        {
            pItem->field_518[7].field_1 = 1;
            this->field_1EB4F = 1;
        }
        else
        {
            pItem->field_518[7].field_1 = 0;
            this->field_1EB4F = 0;
        }

        swprintf(word_67C7D8, L"%c", 3 * v4 + v5 + 64);
        wcsncpy(pItem->field_518[3].field_6_wstr_buf, word_67C7D8, 0x32u);
    }
}

STUB_FUNC(0x4B70B0)
bool laughing_blackwell_0x1EB54::sub_4B70B0()
{
    char_type v3; // [esp+Bh] [ebp-5h]
    char_type a2; // [esp+Ch] [ebp-4h]

    v3 = gLucid_hamilton_67E8E0.sub_4C5990();
    a2 = sub_4B7120(v3);
    gLucid_hamilton_67E8E0.sub_4C5900(a2);
    this->field_1EB42[gLucid_hamilton_67E8E0.sub_4C59B0()] = a2;
    sub_4B7610();
    return v3 != a2;
}

STUB_FUNC(0x4B74F0)
bool laughing_blackwell_0x1EB54::sub_4B74F0()
{
    char_type v2; // bl

    v2 = gLucid_hamilton_67E8E0.sub_4C5990();
    return sub_4B7120(v2) != v2;
}

STUB_FUNC(0x4B7360)
char_type laughing_blackwell_0x1EB54::sub_4B7360(char_type a2)
{
    // todo
    return 0;
}

STUB_FUNC(0x4B7520)
bool laughing_blackwell_0x1EB54::sub_4B7520()
{
    char_type v2; // bl

    v2 = gLucid_hamilton_67E8E0.sub_4C5990();
    return sub_4B7360(v2) != v2;
}

STUB_FUNC(0x4B72F0)
bool laughing_blackwell_0x1EB54::sub_4B72F0()
{
    char_type v3; // [esp+Bh] [ebp-5h]
    char_type a2; // [esp+Ch] [ebp-4h]

    v3 = gLucid_hamilton_67E8E0.sub_4C5990();
    a2 = sub_4B7360(v3);
    gLucid_hamilton_67E8E0.sub_4C5900(a2);
    this->field_1EB42[gLucid_hamilton_67E8E0.sub_4C59B0()] = a2;
    sub_4B7610();
    return v3 != a2;
}

STUB_FUNC(0x4B7200)
bool laughing_blackwell_0x1EB54::sub_4B7200()
{
    char_type v3; // [esp+Bh] [ebp-5h]
    u8 a2; // [esp+Ch] [ebp-4h]

    v3 = gLucid_hamilton_67E8E0.sub_4C5980();
    a2 = sub_4B7270(v3);
    gLucid_hamilton_67E8E0.sub_4C58F0(a2);
    this->field_1EB3A[gLucid_hamilton_67E8E0.sub_4C59B0()] = a2;
    sub_4B7550();
    return v3 != (char)a2;
}

STUB_FUNC(0x4B4EC0)
void laughing_blackwell_0x1EB54::sub_4B4EC0()
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

STUB_FUNC(0x4B61B0)
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

STUB_FUNC(0x4B6200)
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
    field_2 = 0;
    field_4 = 0;
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
    field_2 = 0;
    field_4 = 0;
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
    
    for (s32 i=0; i< GTA2_COUNTOF(field_72); i++) // or is it a u32 ??
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
    u16 oldCount = this->field_6E_count;
    u16 new_count = oldCount;
    char_type bFound = 0;
    do
    {

        if (new_count == 0)
        {
            if (byte_67DA80)
            {
                new_count = this->field_7E;
            }
        }

        else
        {
            --new_count; // add     eax, 0FFFFh
        }

        if (this->field_72[new_count])
        {
            bFound = 1;
        }

    } while (new_count != this->field_6E_count && !bFound); // 6E not reloaded

    this->field_6E_count = new_count;

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
    File::Global_Read_4A71C0(&svg, &len);

    File::Global_Close_4A70C0();

    field_0 = 1;
    field_1 = svg.field_4B;
    field_2 = svg.field_4C;
    field_3 = svg.field_4D;
}

struct Vert
{
    f32 field_0_x;
    f32 field_4_y;
    f32 field_8_z;
    f32 field_C_w;
    s32 field_10_diff;
    s32 field_14_spec;
    f32 field_18_u;
    f32 field_1C_v;
};

struct QuadVerts
{
    Vert field_0_verts[4];
};
EXPORT_VAR QuadVerts gQuadVerts_706B88;
GLOBAL(gQuadVerts_706B88, 0x706B88);

EXPORT_VAR extern u32 gLightingDrawFlag_7068F4;

STUB_FUNC(0x5D83E0);
EXPORT s32 __stdcall CalcQuadFlags_5D83E0(s32 mode, u8 a2)
{
    s32 result; // eax

    if (mode)
    {
        if (mode == 1)
        {
            gQuadVerts_706B88.field_0_verts[0].field_10_diff = (a2 << 27) | 0xFFFFFF;
            gQuadVerts_706B88.field_0_verts[1].field_10_diff = (a2 << 27) | 0xFFFFFF;
            gQuadVerts_706B88.field_0_verts[2].field_10_diff = (a2 << 27) | 0xFFFFFF;
            gQuadVerts_706B88.field_0_verts[3].field_10_diff = (a2 << 27) | 0xFFFFFF;
            return gLightingDrawFlag_7068F4 | 0x2180;
        }
        else if (mode == 2)
        {
            gQuadVerts_706B88.field_0_verts[0].field_10_diff = (a2 << 27) | 0xFFFFFF;
            gQuadVerts_706B88.field_0_verts[1].field_10_diff = (a2 << 27) | 0xFFFFFF;
            gQuadVerts_706B88.field_0_verts[2].field_10_diff = (a2 << 27) | 0xFFFFFF;
            gQuadVerts_706B88.field_0_verts[3].field_10_diff = (a2 << 27) | 0xFFFFFF;
            return gLightingDrawFlag_7068F4 | 0x2280;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        //result = gLightingDrawFlag_7068F4;
        result = gLightingDrawFlag_7068F4 | 0x80;
    }
    return result;
}

EXPORT_VAR Fix16 dword_706A6C;
GLOBAL(dword_706A6C, 0x706A6C);

STUB_FUNC(0x5D8A10)
EXPORT void __stdcall DrawText_5D8A10(const wchar_t* pText,
                               Fix16 xpos_fp,
                               Fix16 ypos_fp,
                               u16 spaceWidth,
                               Fix16 fp4,
                               s32* pUnknown,
                               s32 a7,
                               s32 mode,
                               s32 a9)
{
    s32 new_Flags = CalcQuadFlags_5D83E0(mode, a9) | 0x20000;
    s32 drawFlags = new_Flags;
    const wchar_t* pTextIter = pText;

    Fix16 cur_xpos = xpos_fp; // note: new var
    u16 calcSpaceWidth = gGtx_0x106C_703DD4->space_width_5AA7B0(&spaceWidth);
    Fix16 v11 = fp4;

    Fix16 space_amount;
    space_amount.mValue = fp4.mValue * calcSpaceWidth;

    Fix16 lineHeight;
    lineHeight.mValue = v11.mValue * (u16)gGtx_0x106C_703DD4->sub_5AA800(&spaceWidth);
    //  pTmpIter = (WORD *)*pUnknown;
    mode = a7;

    s32 kind = *pUnknown;
    if (v11.mValue == dword_706A6C.mValue)
    {
        drawFlags = new_Flags | 0x10000;
    }

    s32 v15 = spaceWidth;
    if ((u16)spaceWidth >= 101u)
    {
        if (kind == 8)
        {
            kind = gMagical_germain_0x8EC_6F5168->sub_4D29D0(a7);
        }
        else
        {
            kind = gMagical_germain_0x8EC_6F5168->sub_4D28A0(spaceWidth);
        }
        v15 = spaceWidth;
    }

    //if (*pText)
    for (wchar_t text_char = *pTextIter; *pTextIter; pTextIter++)
    {

        // HIWORD(v16) =
        //HIWORD(v16) = HIWORD(pTextIter);

        // = LOWORD(v16)
        text_char = *pTextIter;
        if (*pTextIter == '\n')
        {
            cur_xpos = xpos_fp;
            ypos_fp.mValue += lineHeight.mValue;
        }
        else if ((WORD)text_char == ' ')
        {
            cur_xpos.mValue += space_amount.mValue;
        }
        else if ((WORD)text_char == '#')
        {
            s32 v18;
            if (kind == *pUnknown && (WORD)mode == (WORD)a7)
            {
                kind = 8;
                /*
                v17 = -((u16)v15 < 0x65u);
                // LOBYTE(v17) =
                v17 = v17 & 0xFB;
                v18 = v17 + 5;
                */
                v18 = v15 < 0x65u ? 10 : 5;
            }
            else
            {
                kind = *pUnknown;
                v18 = a7;
            }
            mode = v18;

            if ((u16)v15 >= 101u)
            {
                if (kind == 8)
                {
                    gMagical_germain_0x8EC_6F5168->sub_4D29D0(mode);
                }
                else
                {
                    gMagical_germain_0x8EC_6F5168->sub_4D28A0(v15);
                }
                v15 = spaceWidth;
            }
        }
        else
        {
            sprite_index* pSprIdx;
            STexture* pTextureToUse;
            if ((u16)v15 < 0x65u || (u16)v15 > 107u)
            {
                if ((u16)v15 < 0xC9u || (u16)v15 > 203u)
                {
                    // LOWORD(sprite_pal) =
                    u16 sprite_pal = gGtx_0x106C_703DD4->sub_5AA710(v15, (WORD)text_char - 33);
                    u16 sprt_idx = gGtx_0x106C_703DD4->convert_sprite_pal_5AA460(7, sprite_pal);
                    pSprIdx = gGtx_0x106C_703DD4->get_sprite_index_5AA440(sprt_idx);
                    pTextureToUse = gSharp_pare_0x15D8_705064->sub_5B94F0(7, sprite_pal, kind, mode);
                }
                else
                {
                    pSprIdx = gMagical_germain_0x8EC_6F5168->field_8E0_sprite_index;
                    pTextureToUse = gMagical_germain_0x8EC_6F5168->sub_4D27D0((u32*)text_char);
                }
            }
            else
            {
                pSprIdx = gMagical_germain_0x8EC_6F5168->field_8D4_sprite_index;
                pTextureToUse = gMagical_germain_0x8EC_6F5168->sub_4D2710(text_char);
            }

            __asm {nop}
            ;
            __asm {nop}
            ;
            __asm {nop}
            ;

            STexture* pTexture = pTextureToUse;

            //v25 = v11.mValue;
            //v26 = v11.mValue >> 31;

            Fix16 sprite_xoff;
            sprite_xoff.mValue = (__int64)(pSprIdx->field_4_width << 14) * (__int64)(v11.mValue >> 14);

            Fix16 sprite_yoff;
            sprite_yoff.mValue = (__int64)(pSprIdx->field_5_height << 14) * (__int64)(v11.mValue >> 14);

            gQuadVerts_706B88.field_0_verts[0].field_8_z = 0.000099999997;
            gQuadVerts_706B88.field_0_verts[1].field_8_z = 0.000099999997;
            s32 v28 = sprite_xoff.mValue + cur_xpos.mValue;
            gQuadVerts_706B88.field_0_verts[2].field_8_z = 0.000099999997;
            gQuadVerts_706B88.field_0_verts[3].field_8_z = 0.000099999997;

            gQuadVerts_706B88.field_0_verts[0].field_0_x = (f64)cur_xpos.mValue * 0.000061035156;
            gQuadVerts_706B88.field_0_verts[0].field_4_y = (f64)ypos_fp.mValue * 0.000061035156;

            f32 v_1_2_x = (f64)(sprite_xoff.mValue + cur_xpos.mValue) * 0.000061035156;
            gQuadVerts_706B88.field_0_verts[1].field_0_x = v_1_2_x;
            gQuadVerts_706B88.field_0_verts[1].field_4_y = gQuadVerts_706B88.field_0_verts[0].field_4_y;
            gQuadVerts_706B88.field_0_verts[2].field_0_x = v_1_2_x;

            f32 v2_3_y = (f64)(ypos_fp.mValue + sprite_yoff.mValue) * 0.000061035156;
            gQuadVerts_706B88.field_0_verts[2].field_4_y = v2_3_y;
            gQuadVerts_706B88.field_0_verts[3].field_0_x = gQuadVerts_706B88.field_0_verts[0].field_0_x;
            gQuadVerts_706B88.field_0_verts[3].field_4_y = v2_3_y;

            gQuadVerts_706B88.field_0_verts[0].field_18_u = 0.0;
            gQuadVerts_706B88.field_0_verts[0].field_1C_v = 0.0;
            gQuadVerts_706B88.field_0_verts[1].field_1C_v = 0.0;
            gQuadVerts_706B88.field_0_verts[3].field_18_u = 0.0;

            f32 v1_u = (((f64)pSprIdx->field_4_width - 0.000099999997) * 16384.0);
            gQuadVerts_706B88.field_0_verts[1].field_18_u = (f64)v1_u * 0.000061035156;
            gQuadVerts_706B88.field_0_verts[2].field_18_u = gQuadVerts_706B88.field_0_verts[1].field_18_u;

            f32 v2_v = (((f64)pSprIdx->field_5_height - 0.000099999997) * 16384.0);
            gQuadVerts_706B88.field_0_verts[2].field_1C_v = (f64)v2_v * 0.000061035156;
            gQuadVerts_706B88.field_0_verts[3].field_1C_v = gQuadVerts_706B88.field_0_verts[2].field_1C_v;

            gbh_DrawQuad(drawFlags, pTexture, &gQuadVerts_706B88.field_0_verts[0], 255);

            //v15 = (s16)retaddr;
            space_amount = kind;
            v11 = v2_v;
            fp4 = v28;
            //v13 = v29;
        }
    }
    // return (const wchar_t*)pTextIter;
}

MATCH_FUNC(0x4B87A0)
EXPORT void __stdcall DrawText_4B87A0(const wchar_t* pBuffer, Fix16 xpos_fp, Fix16 ypos_fp, s16 spaceWidth, Fix16 fp4)
{
    s32 local; // [esp+0h] [ebp-4h] BYREF

    local = 2;
    DrawText_5D8A10(pBuffer, xpos_fp, ypos_fp, spaceWidth, fp4, &local, 0, 0, 0);
}
