#include "Phi_8CA8.hpp"
#include "enums.hpp"
#include "Globals.hpp"
#include "debug.hpp"
#include "gtx_0x106C.hpp"
#include "sprite.hpp"
#include <memory.h>

DEFINE_GLOBAL(Phi_8CA8*, gPhi_8CA8_6FCF00, 0x6FCF00);

DEFINE_GLOBAL_INIT(Fix16, DAT_006f8fac, Fix16(4096, 0), 0x6f8fac);
DEFINE_GLOBAL_INIT(Fix16, DAT_006f8fd8, Fix16(1638, 0), 0x6F8FD8);
DEFINE_GLOBAL_INIT(Fix16, DAT_006fc578, Fix16(3276, 0), 0x6fc578);
DEFINE_GLOBAL_INIT(Fix16, DAT_006fc584, Fix16(8192, 0), 0x6fc584);
DEFINE_GLOBAL_INIT(Fix16, DAT_006fce08, Fix16(0, 0), 0x6fce08);
DEFINE_GLOBAL_INIT(Fix16, DAT_006fceb0, Fix16(256, 0), 0x6fceb0);
DEFINE_GLOBAL_INIT(Fix16, DAT_006fcde0, Fix16(327, 0), 0x6fcde0);
DEFINE_GLOBAL_INIT(Fix16, DAT_006fcdd8, Fix16(163, 0), 0x6FCDD8);
DEFINE_GLOBAL_INIT(Fix16, DAT_006fce10, Fix16(32768, 0), 0x6FCE10);
DEFINE_GLOBAL_INIT(Fix16, DAT_006F8FF0, Fix16(9830, 0), 0x6F8FF0);
DEFINE_GLOBAL_INIT(Fix16, DAT_006FCED0, Fix16(4096, 0), 0x6FCED0);

DEFINE_GLOBAL_INIT(Fix16, dword_6FCE0C, Fix16(4096, 0), 0x6FCE0C);
DEFINE_GLOBAL_INIT(Fix16, dword_6FCE14, Fix16(49152, 0), 0x6FCE14);
DEFINE_GLOBAL_INIT(Fix16, dword_6F8FE0, Fix16(3276, 0), 0x6F8FE0);
DEFINE_GLOBAL_INIT(Fix16, dword_6FCE1C, Fix16(81920, 0), 0x6FCE1C);
DEFINE_GLOBAL_INIT(Fix16, dword_6F8FDC, Fix16(4092, 0), 0x6F8FDC);
DEFINE_GLOBAL_INIT(Fix16, dword_6FCE28, Fix16(0x20000, 0), 0x6FCE28);
DEFINE_GLOBAL_INIT(Fix16, dword_6FCE60, Fix16(4096, 0), 0x6FCE60);
DEFINE_GLOBAL_INIT(Fix16, dword_6F8FEC, Fix16(0x2000, 0), 0x6F8FEC);
DEFINE_GLOBAL_INIT(Fix16, dword_6FC5A4, Fix16(23170, 0), 0x6FC5A4);
DEFINE_GLOBAL_INIT(Fix16, dword_6F8FE8, Fix16(6553, 0), 0x6F8FE8);

Fix16 dword_6FCE08 = 0;

DEFINE_GLOBAL_INIT(s32, Phi_54_array_lenght_00623EEC, 24, 0x623EEC);

// This array is initialized by FUN_005323b0 (likely a static constructor).
// Must remain a fixed array — pointer would break Phi_8CA8::sub_533360.
DEFINE_GLOBAL_ARRAY(Phi_54, Phi_54_array_006FC5F8, 24, 0x6FC5F8);

/*
DEFINE_GLOBAL_ARRAY_INIT(Phi_54,
Phi_54_array_006FC5F8,
24,
0x6FC5F8,
Phi_54(22 ,0 ,45 ,0 ,2 ,0 ,4 ,dword_6FCE08 ,dword_6FCE08 ,0 ,0 ,1 ,0 ,1 ,dword_6FCE0C ,10 ,0 ,0 ,0 ,1 ,1),
Phi_54(7 ,0 ,157 ,0 ,2 ,0 ,4 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,0 ,1 ,dword_6FCE14 ,10 ,0 ,0 ,0 ,1 ,1),
Phi_54(1 ,0 ,158 ,0 ,2 ,0 ,4 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,0 ,1 ,dword_6F8FE0 ,11 ,0 ,0 ,0 ,1 ,1),
Phi_54(5 ,0 ,156 ,0 ,2 ,0 ,4 ,dword_6FCE08 ,dword_6FCE08 ,0 ,0 ,1 ,0 ,1 ,DAT_006f8fd8 ,10 ,0 ,1 ,0 ,1 ,1),
Phi_54(2 ,0 ,0 ,0 ,0 ,0 ,4 ,dword_6FCE08 ,-DAT_006fcdd8 ,0 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,10 ,0 ,0 ,0 ,1 ,1),
Phi_54(4 ,0 ,52 ,0 ,1 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,2 ,0 ,0 ,0 ,1 ,1),
Phi_54(11 ,0 ,123 ,0 ,2 ,0 ,4 ,dword_6FCE08 ,dword_6FCE08 ,0 ,0 ,1 ,0 ,1 ,DAT_006f8fd8 ,10 ,0 ,0 ,0 ,1 ,1),
Phi_54(3 ,0 ,155 ,0 ,2 ,0 ,4 ,dword_6FCE08 ,dword_6FCE08 ,0 ,0 ,1 ,0 ,1 ,DAT_006f8fd8 ,10 ,0 ,0 ,0 ,1 ,1),
Phi_54(12 ,0 ,50 ,0 ,1 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,2 ,0 ,0 ,0 ,1 ,1),
Phi_54(13 ,0 ,56 ,0 ,2 ,0 ,4 ,dword_6FCE08 ,dword_6FCE08 ,0 ,0 ,1 ,0 ,1 ,DAT_006f8fd8 ,10 ,0 ,0 ,0 ,1 ,1),
Phi_54(14 ,0 ,57 ,0 ,2 ,0 ,4 ,dword_6FCE08 ,dword_6FCE08 ,0 ,0 ,1 ,0 ,1 ,DAT_006f8fd8 ,10 ,0 ,0 ,0 ,1 ,1),
Phi_54(18 ,0 ,49 ,0 ,2 ,0 ,4 ,dword_6FCE08 ,dword_6FCE08 ,0 ,0 ,1 ,0 ,1 ,DAT_006f8fd8 ,10 ,0 ,0 ,0 ,1 ,1),
Phi_54(19 ,0 ,0 ,0 ,0 ,0 ,4 ,dword_6FCE08 ,-DAT_006fcdd8 ,0 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,10 ,0 ,0 ,0 ,1 ,1),
Phi_54(20 ,0 ,0 ,0 ,0 ,0 ,4 ,dword_6FCE08 ,-DAT_006fcdd8 ,0 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,10 ,0 ,0 ,0 ,1 ,1),
Phi_54(10 ,0 ,0 ,0 ,7 ,0 ,3 ,dword_6FCE08 ,-DAT_006fcdd8 ,-1 ,1 ,1 ,4 ,0 ,dword_6FCE08 ,7 ,0 ,0 ,0 ,3 ,0),
Phi_54(15 ,0 ,59 ,0 ,2 ,0 ,4 ,dword_6FCE08 ,dword_6FCE08 ,0 ,0 ,1 ,0 ,1 ,DAT_006f8fd8 ,10 ,0 ,0 ,0 ,1 ,1),
Phi_54(8 ,2 ,9 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,1 ,1 ,1 ,1 ,0 ,dword_6FCE08 ,3 ,0 ,0 ,0 ,1 ,1),
Phi_54(9 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,3 ,0 ,0 ,0 ,3 ,0),
Phi_54(16 ,0 ,61 ,0 ,2 ,0 ,4 ,dword_6FCE08 ,dword_6FCE08 ,0 ,0 ,1 ,0 ,1 ,DAT_006f8fd8 ,10 ,0 ,0 ,0 ,1 ,1),
Phi_54(23 ,0 ,43 ,0 ,2 ,0 ,4 ,dword_6FCE08 ,dword_6FCE08 ,0 ,0 ,1 ,0 ,1 ,DAT_006f8fd8 ,10 ,0 ,0 ,0 ,1 ,1),
Phi_54(21 ,0 ,47 ,0 ,2 ,0 ,4 ,dword_6FCE08 ,dword_6FCE08 ,0 ,0 ,1 ,0 ,1 ,dword_6FCE1C ,10 ,0 ,0 ,0 ,1 ,1),
Phi_54(6 ,0 ,151 ,0 ,1 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,2 ,0 ,0 ,0 ,1 ,1),
Phi_54(25 ,0 ,0 ,0 ,0 ,12 ,4 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,10 ,0 ,0 ,0 ,1 ,1),
Phi_54(17 ,0 ,63 ,0 ,2 ,0 ,4 ,dword_6FCE08 ,dword_6FCE08 ,0 ,0 ,1 ,0 ,1 ,DAT_006f8fd8 ,10 ,0 ,0 ,0 ,1 ,1));
*/

DEFINE_GLOBAL_INIT(s32, Phi_6C_array_lenght_623EF0, 126, 0x623EF0);

// This array is initialized by FUN_0052cde0 (likely a static constructor).
// Must remain a fixed array — pointer would break Phi_8CA8::sub_533B30.
DEFINE_GLOBAL_ARRAY(Phi_6C, Phi_6C_array_6F9038, 126, 0x6F9038);

/* TODO: fix pass constants by ref, solve Fix16 operations and Phi_6C id 225 variables
DEFINE_GLOBAL_ARRAY_INIT(Phi_6C,
Phi_6C_array_6F9038,
126,
0x6F9038,
{149 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE0C ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,28 ,0 ,1 ,1 ,0 ,0 ,0 ,1},
{109 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,2 ,0 ,1 ,1 ,0 ,0 ,0 ,1},
{257 ,1 ,0 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,2 ,0 ,1 ,1 ,0 ,0 ,0 ,1},
{111 ,5 ,2 ,0 ,0 ,1 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,1 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,2 ,0 ,1 ,2 ,0 ,0 ,0 ,1},
{247 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,5 ,0 ,1 ,1 ,0 ,0 ,0 ,1},
{266 ,1 ,1 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,0 ,1 ,0 ,0 ,0 ,1},
{175 ,5 ,2 ,0 ,0 ,1 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,3 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,5 ,0 ,1 ,1 ,0 ,0 ,0 ,1},
{254 ,3 ,3 ,0 ,0 ,1 ,7 ,0 ,1 ,dword_6F8FE0 ,dword_6FCE08 ,25 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,28 ,4 ,1 ,1 ,1 ,0 ,0 ,1},
{129 ,0 ,0 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,DAT_006F8FF0 ,DAT_006F8FF0 ,dword_6F8FDC ,0 ,0 ,0 ,1 ,0 ,0 ,0 ,1},
{132 ,0 ,2 ,42 ,42 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,120 ,1 ,1 ,1 ,1 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,0 ,0 ,1 ,1 ,0 ,0 ,0 ,1},
{122 ,0 ,0 ,0 ,0 ,0 ,9 ,0 ,4 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,dword_6FCE0C ,2 * DAT_006fceb0 ,dword_6F8FDC ,0 ,0 ,0 ,1 ,0 ,0 ,0 ,1},
{130 ,0 ,10 ,131 ,131 ,0 ,6 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,dword_6FCE0C ,dword_6FCE0C ,dword_6FCE0C ,7 ,0 ,0 ,1 ,0 ,0 ,0 ,0},
{131 ,0 ,10 ,130 ,130 ,1 ,3 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,dword_6FCE0C ,dword_6FCE0C ,dword_6FCE0C ,7 ,0 ,0 ,1 ,0 ,0 ,0 ,0},
{139 ,0 ,1 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,dword_6FCE0C ,dword_6FCE0C ,dword_6FCE08 ,0 ,0 ,0 ,1 ,0 ,1 ,0 ,0},
{200 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,4 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1},
{201 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,4 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1},
{202 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,4 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1},
{203 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,4 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1},
{204 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,4 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1},
{205 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,4 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1},
{206 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,4 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1},
{207 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,4 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1},
{208 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,4 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1},
{209 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,4 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1},
{210 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,4 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1},
{211 ,1 ,6 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1},
{212 ,1 ,6 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1},
{213 ,1 ,6 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1},
{214 ,1 ,6 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1},
{215 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,4 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1},
{216 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,4 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1},
{217 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,4 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1},
{218 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,4 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1},
{219 ,1 ,6 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1},
{220 ,1 ,6 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1},
{221 ,1 ,6 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1},
{222 ,1 ,6 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1},
{223 ,1 ,6 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1},
{224 ,1 ,6 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1},
{225 ,1 ,6 ,0 ,0 ,0 ,9 ,0 ,3 ,v16 ,v17 ,0 ,1 ,1 ,4 ,0 ,0 ,v18 ,v19 ,v20 ,v21 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1},
{226 ,1 ,6 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1},
{227 ,1 ,6 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1},
{228 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,3 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1},
{229 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,3 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1},
{230 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,3 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1},
{231 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,3 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1},
{232 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,3 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1},
{233 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,3 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1},
{234 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,3 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1},
{235 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,3 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1},
{236 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,3 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1},
{237 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,3 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1},
{238 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,3 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1},
{239 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,3 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1},
{240 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,3 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1},
{241 ,1 ,6 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1},
{242 ,1 ,6 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1},
{243 ,1 ,6 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1},
{244 ,1 ,6 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1},
{134 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,30 ,0 ,0 ,1 ,0 ,0 ,0 ,1},
{252 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,32 ,0 ,0 ,1 ,0 ,0 ,0 ,1},
{260 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,32 ,0 ,0 ,1 ,0 ,0 ,0 ,1},
{261 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,32 ,0 ,0 ,1 ,0 ,0 ,0 ,1},
{262 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,32 ,0 ,0 ,1 ,0 ,0 ,0 ,1},
{135 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,4 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,10 ,0 ,0 ,1 ,0 ,0 ,0 ,1},
{136 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,29 ,0 ,0 ,1 ,0 ,0 ,0 ,1},
{140 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,30 ,0 ,0 ,1 ,0 ,0 ,0 ,1},
{263 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,31 ,0 ,0 ,1 ,0 ,0 ,0 ,1},
{137 ,0 ,1 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,dword_6FCE28 ,dword_6FCE28 ,DAT_006fce10 ,0 ,0 ,0 ,1 ,0 ,0 ,0 ,0},
{258 ,0 ,0 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,DAT_006F8FF0 ,DAT_006F8FF0 ,dword_6F8FDC ,0 ,0 ,0 ,1 ,0 ,0 ,0 ,0},
{142 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,15 ,0 ,0 ,1 ,0 ,0 ,0 ,1},
{150 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,17 ,0 ,0 ,1 ,0 ,0 ,0 ,1},
{143 ,0 ,1 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,dword_6FCE0C ,dword_6FCE0C ,dword_6FCE08 ,0 ,0 ,0 ,1 ,0 ,0 ,0 ,0},
{141 ,0 ,1 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,dword_6FCE0C ,dword_6FCE0C ,dword_6FCE08 ,0 ,0 ,0 ,1 ,0 ,0 ,0 ,0},
{167 ,0 ,1 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,dword_6FCE0C ,dword_6FCE0C ,dword_6FCE0C ,0 ,0 ,0 ,1 ,0 ,0 ,0 ,0},
{168 ,0 ,1 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,DAT_006fceb0 ,dword_6FCE0C ,dword_6FCE0C ,0 ,0 ,0 ,1 ,0 ,0 ,0 ,0},
{169 ,0 ,1 ,0 ,0 ,0 ,0 ,12 ,4 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,dword_6FCE0C ,dword_6FCE0C ,dword_6FCE0C ,0 ,0 ,0 ,1 ,0 ,0 ,0 ,1},
{197 ,8 ,2 ,0 ,0 ,1 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,1 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,29 ,133 ,1 ,1 ,0 ,1 ,0 ,1},
{114 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE0C ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,28 ,0 ,1 ,1 ,0 ,0 ,0 ,1},
{194 ,0 ,3 ,194 ,194 ,0 ,8 ,0 ,2 ,dword_6FCE0C ,dword_6FCE08 ,1 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,dword_6FCE60 ,dword_6FCE60 ,dword_6FCE60 ,0 ,0 ,1 ,1 ,1 ,0 ,0 ,1},
{284 ,2 ,2 ,0 ,0 ,1 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,9 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,11 ,4 ,1 ,1 ,0 ,0 ,0 ,1},
{183 ,4 ,4 ,182 ,40 ,1 ,0 ,0 ,4 ,DAT_006f8fd8 ,-DAT_006fcdd8 ,12 ,3 ,3 ,4 ,2 ,1 ,DAT_006f8fd8 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,10 ,0 ,1 ,1 ,1 ,0 ,0 ,1},
{195 ,0 ,3 ,0 ,0 ,1 ,7 ,0 ,2 ,dword_6FCE0C ,dword_6FCE08 ,1 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,dword_6FCE60 ,dword_6FCE60 ,dword_6FCE60 ,0 ,0 ,1 ,1 ,1 ,0 ,0 ,0},
{199 ,0 ,3 ,0 ,0 ,1 ,7 ,0 ,2 ,dword_6FCE14 ,dword_6FCE08 ,1 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,dword_6FCE60 ,dword_6FCE60 ,dword_6FCE60 ,0 ,0 ,1 ,1 ,1 ,0 ,0 ,0},
{285 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,5 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,0 ,0 ,0 ,1 ,0 ,0 ,0 ,1},
{283 ,0 ,0 ,285 ,0 ,0 ,11 ,13 ,4 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,dword_6F8FEC ,dword_6F8FEC ,dword_6F8FEC ,0 ,0 ,0 ,1 ,0 ,0 ,0 ,1},
{166 ,0 ,1 ,0 ,0 ,0 ,0 ,12 ,2 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,DAT_006fceb0 ,dword_6FC5A4 ,DAT_006fce10 ,0 ,0 ,0 ,1 ,0 ,0 ,0 ,1},
{280 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,5 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,0 ,0 ,0 ,1 ,0 ,0 ,0 ,1},
{248 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE0C ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,28 ,0 ,1 ,1 ,0 ,0 ,0 ,1},
{286 ,8 ,2 ,0 ,0 ,1 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,3 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,5 ,0 ,1 ,1 ,0 ,0 ,1 ,1},
{165 ,0 ,11 ,0 ,0 ,0 ,0 ,0 ,5 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE0C ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,0 ,0 ,1 ,1 ,0 ,0 ,0 ,1},
{171 ,0 ,11 ,0 ,0 ,0 ,0 ,0 ,5 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE0C ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,0 ,0 ,1 ,1 ,0 ,0 ,0 ,1},
{172 ,0 ,11 ,0 ,0 ,0 ,0 ,0 ,5 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE0C ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,0 ,0 ,1 ,1 ,0 ,0 ,0 ,1},
{173 ,0 ,11 ,0 ,0 ,0 ,0 ,0 ,5 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE0C ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,0 ,0 ,1 ,1 ,0 ,0 ,0 ,1},
{287 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE0C ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,28 ,0 ,1 ,1 ,0 ,0 ,0 ,1},
{288 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE0C ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,28 ,0 ,1 ,1 ,0 ,0 ,0 ,1},
{289 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE0C ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,28 ,0 ,1 ,1 ,0 ,0 ,0 ,1},
{290 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE0C ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,28 ,0 ,1 ,1 ,0 ,0 ,0 ,1},
{291 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE0C ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,28 ,0 ,1 ,1 ,0 ,0 ,0 ,1},
{292 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE0C ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,28 ,0 ,1 ,1 ,0 ,0 ,0 ,1},
{293 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE0C ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,28 ,0 ,1 ,1 ,0 ,0 ,0 ,1},
{161 ,0 ,1 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,dword_6FCE0C ,dword_6FCE0C ,dword_6FCE08 ,0 ,0 ,0 ,1 ,0 ,0 ,0 ,0},
{138 ,4 ,4 ,39 ,39 ,1 ,7 ,0 ,1 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,4 ,4 ,4 ,3 ,1 ,DAT_006f8fd8 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,10 ,0 ,1 ,1 ,1 ,0 ,0 ,1},
{112 ,205 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,0 ,0 ,0 ,1 ,0 ,0 ,0 ,1},
{163 ,1 ,0 ,164 ,164 ,0 ,1 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,1 ,0 ,0 ,1 ,0 ,1 ,0 ,1},
{164 ,2 ,2 ,185 ,185 ,1 ,6 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,10 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,1 ,0 ,0 ,1 ,0 ,1 ,0 ,1},
{281 ,1 ,0 ,282 ,0 ,1 ,11 ,13 ,4 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,Fix16::Multiply_408680(&DAT_006fceb0, &v27, &v2->mValue) ,*Fix16::Multiply_408680(&DAT_006fceb0, &v25, &v1->mValue) ,dword_6FCE08 ,10 ,0 ,0 ,1 ,0 ,1 ,0 ,1},
{282 ,1 ,0 ,0 ,0 ,0 ,0 ,12 ,4 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,Fix16::Multiply_408680(&DAT_006fceb0, &v24, &v5->mValue) ,*Fix16::Multiply_408680(&DAT_006fceb0, &v26, &v4->mValue) ,dword_6FCE08 ,10 ,0 ,0 ,1 ,0 ,0 ,0 ,1},
{256 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,2 ,0 ,0 ,1 ,0 ,0 ,0 ,1},
{253 ,4 ,2 ,0 ,0 ,1 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,45 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,2 ,0 ,0 ,2 ,0 ,0 ,0 ,1},
{144 ,4 ,2 ,0 ,0 ,1 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,45 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,2 ,0 ,0 ,2 ,0 ,0 ,0 ,1},
{121 ,4 ,2 ,0 ,0 ,1 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,45 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,2 ,0 ,0 ,2 ,0 ,0 ,0 ,1},
{118 ,4 ,2 ,0 ,0 ,1 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,45 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,2 ,0 ,0 ,2 ,0 ,0 ,0 ,1},
{295 ,8 ,2 ,0 ,0 ,1 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,3 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,5 ,0 ,1 ,1 ,0 ,0 ,1 ,1},
{128 ,1 ,3 ,0 ,0 ,1 ,7 ,0 ,1 ,dword_6F8FE8 ,dword_6FCE08 ,50 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,28 ,4 ,1 ,1 ,1 ,1 ,0 ,1},
{192 ,0 ,3 ,259 ,259 ,1 ,8 ,0 ,2 ,dword_6F8FE8 ,dword_6FCE08 ,8 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,dword_6FCE60 ,dword_6FCE60 ,dword_6FCE60 ,0 ,0 ,1 ,1 ,1 ,0 ,0 ,1},
{259 ,5 ,2 ,0 ,0 ,1 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,1 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,2 ,0 ,1 ,2 ,0 ,0 ,0 ,1},
{148 ,2 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE0C ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,28 ,0 ,1 ,1 ,0 ,0 ,0 ,1},
{170 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,29 ,0 ,0 ,1 ,0 ,0 ,0 ,1},
{255 ,3 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,32 ,4 ,0 ,1 ,0 ,0 ,0 ,1},
{294 ,0 ,0 ,0 ,0 ,0 ,0 ,12 ,4 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,dword_6FCE0C ,dword_6FCE0C ,dword_6FCE0C ,0 ,0 ,0 ,1 ,0 ,0 ,0 ,1},
{113 ,0 ,5 ,0 ,0 ,1 ,0 ,0 ,2 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,0 ,0 ,0 ,1 ,0 ,1 ,0 ,1},
{278 ,0 ,12 ,0 ,0 ,0 ,0 ,0 ,5 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,0 ,0 ,1 ,1 ,0 ,1 ,0 ,1},
{279 ,0 ,12 ,0 ,0 ,1 ,0 ,0 ,5 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,0 ,0 ,1 ,1 ,0 ,1 ,0 ,1},
{198 ,0 ,3 ,198 ,198 ,0 ,8 ,0 ,2 ,dword_6FCE0C ,dword_6FCE08 ,1 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,dword_6FCE60 ,dword_6FCE60 ,dword_6FCE60 ,0 ,0 ,1 ,1 ,1 ,0 ,0 ,1},
{251 ,0 ,3 ,0 ,0 ,1 ,0 ,0 ,5 ,dword_6FCE08 ,dword_6FCE08 ,1 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,dword_6FCE60 ,dword_6FCE60 ,dword_6FCE60 ,0 ,0 ,1 ,1 ,1 ,0 ,0 ,1});
*/

// Temporary workaround: create overload without pulling params by ref
Phi_54::Phi_54(s32 param_1,
               s32 param_2,
               s32 param_3,
               s8 param_4,
               s32 param_5,
               s32 param_6,
               s32 param_7,
               Fix16 param_8,
               Fix16 param_9,
               s8 param_10,
               s32 param_11,
               s32 param_12,
               s8 param_13,
               s32 param_14,
               Fix16 param_15,
               s32 param_16,
               s32 param_17,
               s32 param_18,
               s8 param_19,
               s32 param_20,
               s8 param_21)
{
    field_0 = param_1;
    field_4 = param_2;
    field_8 = param_3;
    field_C = param_4;
    field_10 = param_5;
    field_14 = param_6;
    field_18 = param_7;
    field_1C = param_8;
    field_20 = param_9;
    field_24 = param_10;
    field_28 = param_11;
    field_2C = param_12;
    field_30 = param_13;
    field_34 = param_14;
    field_38 = param_15;
    field_3C = param_16;
    field_40 = param_17;
    field_44 = param_18;
    field_48 = param_19;
    field_4C = param_20;
    field_50 = param_21;
}

// Temporary workaround: create overload without pulling params by ref
Phi_6C::Phi_6C(u32 param_1,
               u8 param_2,
               u32 param_3,
               u32 param_4,
               u32 param_5,
               u8 param_6,
               u32 param_7,
               u32 param_8,
               u32 param_9,
               Fix16 param_10,
               Fix16 param_11,
               u8 param_12,
               u32 param_13,
               u32 param_14,
               u32 param_15,
               u32 param_16,
               u32 param_17,
               Fix16 param_18,
               Fix16 param_19,
               Fix16 param_20,
               Fix16 param_21,
               u32 param_22,
               u32 param_23,
               u8 param_24,
               u32 param_25,
               u32 param_26,
               u32 param_27,
               u8 param_28,
               u8 param_29)
{
    field_0 = param_1;
    field_4 = param_2;
    field_8 = param_3;
    field_C = param_4;
    field_10 = param_5;
    field_14 = param_6;
    field_18 = param_7;
    field_1C = param_8;
    field_20 = param_9;
    field_24 = param_10;
    field_28 = param_11;
    field_2C = param_12;
    field_30 = param_13;
    field_34 = param_14;
    field_38 = param_15;
    field_3C = param_16;
    field_40 = param_17;
    field_44 = param_18;
    field_48 = param_19;
    field_4C = param_20;
    field_50 = param_21;
    field_54 = param_22;
    field_58 = param_23;
    field_59 = param_24;
    field_5C = param_25;
    field_60 = param_26;
    field_64 = param_27;
    field_68 = param_28;
    field_69 = param_29;
}

void Init_Phi_54_array()
{
    Phi_54_array_006FC5F8[0] = Phi_54(22 ,0 ,45 ,0 ,2 ,0 ,4 ,dword_6FCE08 ,dword_6FCE08 ,0 ,0 ,1 ,0 ,1 ,dword_6FCE0C ,10 ,0 ,0 ,0 ,1 ,1);
    Phi_54_array_006FC5F8[1] = Phi_54(7 ,0 ,157 ,0 ,2 ,0 ,4 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,0 ,1 ,dword_6FCE14 ,10 ,0 ,0 ,0 ,1 ,1);
    Phi_54_array_006FC5F8[2] = Phi_54(1 ,0 ,158 ,0 ,2 ,0 ,4 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,0 ,1 ,dword_6F8FE0 ,11 ,0 ,0 ,0 ,1 ,1);
    Phi_54_array_006FC5F8[3] = Phi_54(5 ,0 ,156 ,0 ,2 ,0 ,4 ,dword_6FCE08 ,dword_6FCE08 ,0 ,0 ,1 ,0 ,1 ,DAT_006f8fd8 ,10 ,0 ,1 ,0 ,1 ,1);
    Phi_54_array_006FC5F8[4] = Phi_54(2 ,0 ,0 ,0 ,0 ,0 ,4 ,dword_6FCE08 ,-DAT_006fcdd8 ,0 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,10 ,0 ,0 ,0 ,1 ,1);
    Phi_54_array_006FC5F8[5] = Phi_54(4 ,0 ,52 ,0 ,1 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,2 ,0 ,0 ,0 ,1 ,1);
    Phi_54_array_006FC5F8[6] = Phi_54(11 ,0 ,123 ,0 ,2 ,0 ,4 ,dword_6FCE08 ,dword_6FCE08 ,0 ,0 ,1 ,0 ,1 ,DAT_006f8fd8 ,10 ,0 ,0 ,0 ,1 ,1);
    Phi_54_array_006FC5F8[7] = Phi_54(3 ,0 ,155 ,0 ,2 ,0 ,4 ,dword_6FCE08 ,dword_6FCE08 ,0 ,0 ,1 ,0 ,1 ,DAT_006f8fd8 ,10 ,0 ,0 ,0 ,1 ,1);
    Phi_54_array_006FC5F8[8] = Phi_54(12 ,0 ,50 ,0 ,1 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,2 ,0 ,0 ,0 ,1 ,1);
    Phi_54_array_006FC5F8[9] = Phi_54(13 ,0 ,56 ,0 ,2 ,0 ,4 ,dword_6FCE08 ,dword_6FCE08 ,0 ,0 ,1 ,0 ,1 ,DAT_006f8fd8 ,10 ,0 ,0 ,0 ,1 ,1);
    Phi_54_array_006FC5F8[10] = Phi_54(14 ,0 ,57 ,0 ,2 ,0 ,4 ,dword_6FCE08 ,dword_6FCE08 ,0 ,0 ,1 ,0 ,1 ,DAT_006f8fd8 ,10 ,0 ,0 ,0 ,1 ,1);
    Phi_54_array_006FC5F8[11] = Phi_54(18 ,0 ,49 ,0 ,2 ,0 ,4 ,dword_6FCE08 ,dword_6FCE08 ,0 ,0 ,1 ,0 ,1 ,DAT_006f8fd8 ,10 ,0 ,0 ,0 ,1 ,1);
    Phi_54_array_006FC5F8[12] = Phi_54(19 ,0 ,0 ,0 ,0 ,0 ,4 ,dword_6FCE08 ,-DAT_006fcdd8 ,0 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,10 ,0 ,0 ,0 ,1 ,1);
    Phi_54_array_006FC5F8[13] = Phi_54(20 ,0 ,0 ,0 ,0 ,0 ,4 ,dword_6FCE08 ,-DAT_006fcdd8 ,0 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,10 ,0 ,0 ,0 ,1 ,1);
    Phi_54_array_006FC5F8[14] = Phi_54(10 ,0 ,0 ,0 ,7 ,0 ,3 ,dword_6FCE08 ,-DAT_006fcdd8 ,-1 ,1 ,1 ,4 ,0 ,dword_6FCE08 ,7 ,0 ,0 ,0 ,3 ,0);
    Phi_54_array_006FC5F8[15] = Phi_54(15 ,0 ,59 ,0 ,2 ,0 ,4 ,dword_6FCE08 ,dword_6FCE08 ,0 ,0 ,1 ,0 ,1 ,DAT_006f8fd8 ,10 ,0 ,0 ,0 ,1 ,1);
    Phi_54_array_006FC5F8[16] = Phi_54(8 ,2 ,9 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,1 ,1 ,1 ,1 ,0 ,dword_6FCE08 ,3 ,0 ,0 ,0 ,1 ,1);
    Phi_54_array_006FC5F8[17] = Phi_54(9 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,3 ,0 ,0 ,0 ,3 ,0);
    Phi_54_array_006FC5F8[18] = Phi_54(16 ,0 ,61 ,0 ,2 ,0 ,4 ,dword_6FCE08 ,dword_6FCE08 ,0 ,0 ,1 ,0 ,1 ,DAT_006f8fd8 ,10 ,0 ,0 ,0 ,1 ,1);
    Phi_54_array_006FC5F8[19] = Phi_54(23 ,0 ,43 ,0 ,2 ,0 ,4 ,dword_6FCE08 ,dword_6FCE08 ,0 ,0 ,1 ,0 ,1 ,DAT_006f8fd8 ,10 ,0 ,0 ,0 ,1 ,1);
    Phi_54_array_006FC5F8[20] = Phi_54(21 ,0 ,47 ,0 ,2 ,0 ,4 ,dword_6FCE08 ,dword_6FCE08 ,0 ,0 ,1 ,0 ,1 ,dword_6FCE1C ,10 ,0 ,0 ,0 ,1 ,1);
    Phi_54_array_006FC5F8[21] = Phi_54(6 ,0 ,151 ,0 ,1 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,2 ,0 ,0 ,0 ,1 ,1);
    Phi_54_array_006FC5F8[22] = Phi_54(25 ,0 ,0 ,0 ,0 ,12 ,4 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,10 ,0 ,0 ,0 ,1 ,1);
    Phi_54_array_006FC5F8[23] = Phi_54(17 ,0 ,63 ,0 ,2 ,0 ,4 ,dword_6FCE08 ,dword_6FCE08 ,0 ,0 ,1 ,0 ,1 ,DAT_006f8fd8 ,10 ,0 ,0 ,0 ,1 ,1);
    printf("Phi_54 array loaded!");
}

void Init_Phi_6C_array()
{
    Phi_6C_array_6F9038[0] = Phi_6C(149 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE0C ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,28 ,0 ,1 ,1 ,0 ,0 ,0 ,1);
    Phi_6C_array_6F9038[1] = Phi_6C(109 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,2 ,0 ,1 ,1 ,0 ,0 ,0 ,1);
    Phi_6C_array_6F9038[2] = Phi_6C(257 ,1 ,0 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,2 ,0 ,1 ,1 ,0 ,0 ,0 ,1);
    Phi_6C_array_6F9038[3] = Phi_6C(111 ,5 ,2 ,0 ,0 ,1 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,1 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,2 ,0 ,1 ,2 ,0 ,0 ,0 ,1);
    Phi_6C_array_6F9038[4] = Phi_6C(247 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,5 ,0 ,1 ,1 ,0 ,0 ,0 ,1);
    Phi_6C_array_6F9038[5] = Phi_6C(266 ,1 ,1 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,0 ,1 ,0 ,0 ,0 ,1);
    Phi_6C_array_6F9038[6] = Phi_6C(175 ,5 ,2 ,0 ,0 ,1 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,3 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,5 ,0 ,1 ,1 ,0 ,0 ,0 ,1);
    Phi_6C_array_6F9038[7] = Phi_6C(254 ,3 ,3 ,0 ,0 ,1 ,7 ,0 ,1 ,dword_6F8FE0 ,dword_6FCE08 ,25 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,28 ,4 ,1 ,1 ,1 ,0 ,0 ,1);
    Phi_6C_array_6F9038[8] = Phi_6C(129 ,0 ,0 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,DAT_006F8FF0 ,DAT_006F8FF0 ,dword_6F8FDC ,0 ,0 ,0 ,1 ,0 ,0 ,0 ,1);
    Phi_6C_array_6F9038[9] = Phi_6C(132 ,0 ,2 ,42 ,42 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,120 ,1 ,1 ,1 ,1 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,0 ,0 ,1 ,1 ,0 ,0 ,0 ,1);
    Phi_6C_array_6F9038[10] = Phi_6C(122 ,0 ,0 ,0 ,0 ,0 ,9 ,0 ,4 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,dword_6FCE0C , DAT_006fceb0 * 2 ,dword_6F8FDC ,0 ,0 ,0 ,1 ,0 ,0 ,0 ,1);
    Phi_6C_array_6F9038[11] = Phi_6C(130 ,0 ,10 ,131 ,131 ,0 ,6 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,dword_6FCE0C ,dword_6FCE0C ,dword_6FCE0C ,7 ,0 ,0 ,1 ,0 ,0 ,0 ,0);
    Phi_6C_array_6F9038[12] = Phi_6C(131 ,0 ,10 ,130 ,130 ,1 ,3 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,dword_6FCE0C ,dword_6FCE0C ,dword_6FCE0C ,7 ,0 ,0 ,1 ,0 ,0 ,0 ,0);
    Phi_6C_array_6F9038[13] = Phi_6C(139 ,0 ,1 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,dword_6FCE0C ,dword_6FCE0C ,dword_6FCE08 ,0 ,0 ,0 ,1 ,0 ,1 ,0 ,0);
    Phi_6C_array_6F9038[14] = Phi_6C(200 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,4 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1);
    Phi_6C_array_6F9038[15] = Phi_6C(201 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,4 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1);
    Phi_6C_array_6F9038[16] = Phi_6C(202 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,4 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1);
    Phi_6C_array_6F9038[17] = Phi_6C(203 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,4 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1);
    Phi_6C_array_6F9038[18] = Phi_6C(204 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,4 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1);
    Phi_6C_array_6F9038[19] = Phi_6C(205 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,4 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1);
    Phi_6C_array_6F9038[20] = Phi_6C(206 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,4 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1);
    Phi_6C_array_6F9038[21] = Phi_6C(207 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,4 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1);
    Phi_6C_array_6F9038[22] = Phi_6C(208 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,4 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1);
    Phi_6C_array_6F9038[23] = Phi_6C(209 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,4 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1);
    Phi_6C_array_6F9038[24] = Phi_6C(210 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,4 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1);
    Phi_6C_array_6F9038[25] = Phi_6C(211 ,1 ,6 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1);
    Phi_6C_array_6F9038[26] = Phi_6C(212 ,1 ,6 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1);
    Phi_6C_array_6F9038[27] = Phi_6C(213 ,1 ,6 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1);
    Phi_6C_array_6F9038[28] = Phi_6C(214 ,1 ,6 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1);
    Phi_6C_array_6F9038[29] = Phi_6C(215 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,4 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1);
    Phi_6C_array_6F9038[30] = Phi_6C(216 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,4 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1);
    Phi_6C_array_6F9038[31] = Phi_6C(217 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,4 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1);
    Phi_6C_array_6F9038[32] = Phi_6C(218 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,4 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1);
    Phi_6C_array_6F9038[33] = Phi_6C(219 ,1 ,6 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1);
    Phi_6C_array_6F9038[34] = Phi_6C(220 ,1 ,6 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1);
    Phi_6C_array_6F9038[35] = Phi_6C(221 ,1 ,6 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1);
    Phi_6C_array_6F9038[36] = Phi_6C(222 ,1 ,6 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1);
    Phi_6C_array_6F9038[37] = Phi_6C(223 ,1 ,6 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1);
    Phi_6C_array_6F9038[38] = Phi_6C(224 ,1 ,6 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1);
    Phi_6C_array_6F9038[39] = Phi_6C(225 ,1 ,6 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1);
    Phi_6C_array_6F9038[40] = Phi_6C(226 ,1 ,6 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1);
    Phi_6C_array_6F9038[41] = Phi_6C(227 ,1 ,6 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1);
    Phi_6C_array_6F9038[42] = Phi_6C(228 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,3 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1);
    Phi_6C_array_6F9038[43] = Phi_6C(229 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,3 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1);
    Phi_6C_array_6F9038[44] = Phi_6C(230 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,3 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1);
    Phi_6C_array_6F9038[45] = Phi_6C(231 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,3 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1);
    Phi_6C_array_6F9038[46] = Phi_6C(232 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,3 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1);
    Phi_6C_array_6F9038[47] = Phi_6C(233 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,3 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1);
    Phi_6C_array_6F9038[48] = Phi_6C(234 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,3 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1);
    Phi_6C_array_6F9038[49] = Phi_6C(235 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,3 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1);
    Phi_6C_array_6F9038[50] = Phi_6C(236 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,3 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1);
    Phi_6C_array_6F9038[51] = Phi_6C(237 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,3 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1);
    Phi_6C_array_6F9038[52] = Phi_6C(238 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,3 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1);
    Phi_6C_array_6F9038[53] = Phi_6C(239 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,3 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1);
    Phi_6C_array_6F9038[54] = Phi_6C(240 ,8 ,8 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,3 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1);
    Phi_6C_array_6F9038[55] = Phi_6C(241 ,1 ,6 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1);
    Phi_6C_array_6F9038[56] = Phi_6C(242 ,1 ,6 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1);
    Phi_6C_array_6F9038[57] = Phi_6C(243 ,1 ,6 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1);
    Phi_6C_array_6F9038[58] = Phi_6C(244 ,1 ,6 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,7 ,0 ,1 ,1 ,0 ,0 ,1 ,1);
    Phi_6C_array_6F9038[59] = Phi_6C(134 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,30 ,0 ,0 ,1 ,0 ,0 ,0 ,1);
    Phi_6C_array_6F9038[60] = Phi_6C(252 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,32 ,0 ,0 ,1 ,0 ,0 ,0 ,1);
    Phi_6C_array_6F9038[61] = Phi_6C(260 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,32 ,0 ,0 ,1 ,0 ,0 ,0 ,1);
    Phi_6C_array_6F9038[62] = Phi_6C(261 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,32 ,0 ,0 ,1 ,0 ,0 ,0 ,1);
    Phi_6C_array_6F9038[63] = Phi_6C(262 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,32 ,0 ,0 ,1 ,0 ,0 ,0 ,1);
    Phi_6C_array_6F9038[64] = Phi_6C(135 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,4 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,10 ,0 ,0 ,1 ,0 ,0 ,0 ,1);
    Phi_6C_array_6F9038[65] = Phi_6C(136 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,29 ,0 ,0 ,1 ,0 ,0 ,0 ,1);
    Phi_6C_array_6F9038[66] = Phi_6C(140 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,30 ,0 ,0 ,1 ,0 ,0 ,0 ,1);
    Phi_6C_array_6F9038[67] = Phi_6C(263 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,31 ,0 ,0 ,1 ,0 ,0 ,0 ,1);
    Phi_6C_array_6F9038[68] = Phi_6C(137 ,0 ,1 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,dword_6FCE28 ,dword_6FCE28 ,DAT_006fce10 ,0 ,0 ,0 ,1 ,0 ,0 ,0 ,0);
    Phi_6C_array_6F9038[69] = Phi_6C(258 ,0 ,0 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,DAT_006F8FF0 ,DAT_006F8FF0 ,dword_6F8FDC ,0 ,0 ,0 ,1 ,0 ,0 ,0 ,0);
    Phi_6C_array_6F9038[70] = Phi_6C(142 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,15 ,0 ,0 ,1 ,0 ,0 ,0 ,1);
    Phi_6C_array_6F9038[71] = Phi_6C(150 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,17 ,0 ,0 ,1 ,0 ,0 ,0 ,1);
    Phi_6C_array_6F9038[72] = Phi_6C(143 ,0 ,1 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,dword_6FCE0C ,dword_6FCE0C ,dword_6FCE08 ,0 ,0 ,0 ,1 ,0 ,0 ,0 ,0);
    Phi_6C_array_6F9038[73] = Phi_6C(141 ,0 ,1 ,0 ,0 ,1 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,dword_6FCE0C ,dword_6FCE0C ,dword_6FCE08 ,0 ,0 ,0 ,1 ,0 ,0 ,0 ,0);
    Phi_6C_array_6F9038[74] = Phi_6C(167 ,0 ,1 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,dword_6FCE0C ,dword_6FCE0C ,dword_6FCE0C ,0 ,0 ,0 ,1 ,0 ,0 ,0 ,0);
    Phi_6C_array_6F9038[75] = Phi_6C(168 ,0 ,1 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,DAT_006fceb0 ,dword_6FCE0C ,dword_6FCE0C ,0 ,0 ,0 ,1 ,0 ,0 ,0 ,0);
    Phi_6C_array_6F9038[76] = Phi_6C(169 ,0 ,1 ,0 ,0 ,0 ,0 ,12 ,4 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,dword_6FCE0C ,dword_6FCE0C ,dword_6FCE0C ,0 ,0 ,0 ,1 ,0 ,0 ,0 ,1);
    Phi_6C_array_6F9038[77] = Phi_6C(197 ,8 ,2 ,0 ,0 ,1 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,1 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,29 ,133 ,1 ,1 ,0 ,1 ,0 ,1);
    Phi_6C_array_6F9038[78] = Phi_6C(114 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE0C ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,28 ,0 ,1 ,1 ,0 ,0 ,0 ,1);
    Phi_6C_array_6F9038[79] = Phi_6C(194 ,0 ,3 ,194 ,194 ,0 ,8 ,0 ,2 ,dword_6FCE0C ,dword_6FCE08 ,1 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,dword_6FCE60 ,dword_6FCE60 ,dword_6FCE60 ,0 ,0 ,1 ,1 ,1 ,0 ,0 ,1);
    Phi_6C_array_6F9038[80] = Phi_6C(284 ,2 ,2 ,0 ,0 ,1 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,9 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,11 ,4 ,1 ,1 ,0 ,0 ,0 ,1);
    Phi_6C_array_6F9038[81] = Phi_6C(183 ,4 ,4 ,182 ,40 ,1 ,0 ,0 ,4 ,DAT_006f8fd8 ,-DAT_006fcdd8 ,12 ,3 ,3 ,4 ,2 ,1 ,DAT_006f8fd8 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,10 ,0 ,1 ,1 ,1 ,0 ,0 ,1);
    Phi_6C_array_6F9038[82] = Phi_6C(195 ,0 ,3 ,0 ,0 ,1 ,7 ,0 ,2 ,dword_6FCE0C ,dword_6FCE08 ,1 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,dword_6FCE60 ,dword_6FCE60 ,dword_6FCE60 ,0 ,0 ,1 ,1 ,1 ,0 ,0 ,0);
    Phi_6C_array_6F9038[83] = Phi_6C(199 ,0 ,3 ,0 ,0 ,1 ,7 ,0 ,2 ,dword_6FCE14 ,dword_6FCE08 ,1 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,dword_6FCE60 ,dword_6FCE60 ,dword_6FCE60 ,0 ,0 ,1 ,1 ,1 ,0 ,0 ,0);
    Phi_6C_array_6F9038[84] = Phi_6C(285 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,5 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,0 ,0 ,0 ,1 ,0 ,0 ,0 ,1);
    Phi_6C_array_6F9038[85] = Phi_6C(283 ,0 ,0 ,285 ,0 ,0 ,11 ,13 ,4 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,dword_6F8FEC ,dword_6F8FEC ,dword_6F8FEC ,0 ,0 ,0 ,1 ,0 ,0 ,0 ,1);
    Phi_6C_array_6F9038[86] = Phi_6C(166 ,0 ,1 ,0 ,0 ,0 ,0 ,12 ,2 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,DAT_006fceb0 ,dword_6FC5A4 ,DAT_006fce10 ,0 ,0 ,0 ,1 ,0 ,0 ,0 ,1);
    Phi_6C_array_6F9038[87] = Phi_6C(280 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,5 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,0 ,0 ,0 ,1 ,0 ,0 ,0 ,1);
    Phi_6C_array_6F9038[88] = Phi_6C(248 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE0C ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,28 ,0 ,1 ,1 ,0 ,0 ,0 ,1);
    Phi_6C_array_6F9038[89] = Phi_6C(286 ,8 ,2 ,0 ,0 ,1 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,3 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,5 ,0 ,1 ,1 ,0 ,0 ,1 ,1);
    Phi_6C_array_6F9038[90] = Phi_6C(165 ,0 ,11 ,0 ,0 ,0 ,0 ,0 ,5 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE0C ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,0 ,0 ,1 ,1 ,0 ,0 ,0 ,1);
    Phi_6C_array_6F9038[91] = Phi_6C(171 ,0 ,11 ,0 ,0 ,0 ,0 ,0 ,5 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE0C ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,0 ,0 ,1 ,1 ,0 ,0 ,0 ,1);
    Phi_6C_array_6F9038[92] = Phi_6C(172 ,0 ,11 ,0 ,0 ,0 ,0 ,0 ,5 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE0C ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,0 ,0 ,1 ,1 ,0 ,0 ,0 ,1);
    Phi_6C_array_6F9038[93] = Phi_6C(173 ,0 ,11 ,0 ,0 ,0 ,0 ,0 ,5 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE0C ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,0 ,0 ,1 ,1 ,0 ,0 ,0 ,1);
    Phi_6C_array_6F9038[94] = Phi_6C(287 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE0C ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,28 ,0 ,1 ,1 ,0 ,0 ,0 ,1);
    Phi_6C_array_6F9038[95] = Phi_6C(288 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE0C ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,28 ,0 ,1 ,1 ,0 ,0 ,0 ,1);
    Phi_6C_array_6F9038[96] = Phi_6C(289 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE0C ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,28 ,0 ,1 ,1 ,0 ,0 ,0 ,1);
    Phi_6C_array_6F9038[97] = Phi_6C(290 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE0C ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,28 ,0 ,1 ,1 ,0 ,0 ,0 ,1);
    Phi_6C_array_6F9038[98] = Phi_6C(291 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE0C ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,28 ,0 ,1 ,1 ,0 ,0 ,0 ,1);
    Phi_6C_array_6F9038[99] = Phi_6C(292 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE0C ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,28 ,0 ,1 ,1 ,0 ,0 ,0 ,1);
    Phi_6C_array_6F9038[100] = Phi_6C(293 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE0C ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,28 ,0 ,1 ,1 ,0 ,0 ,0 ,1);
    Phi_6C_array_6F9038[101] = Phi_6C(161 ,0 ,1 ,0 ,0 ,0 ,9 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,dword_6FCE0C ,dword_6FCE0C ,dword_6FCE08 ,0 ,0 ,0 ,1 ,0 ,0 ,0 ,0);
    Phi_6C_array_6F9038[102] = Phi_6C(138 ,4 ,4 ,39 ,39 ,1 ,7 ,0 ,1 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,4 ,4 ,4 ,3 ,1 ,DAT_006f8fd8 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,10 ,0 ,1 ,1 ,1 ,0 ,0 ,1);
    Phi_6C_array_6F9038[103] = Phi_6C(112 ,205 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,0 ,0 ,0 ,1 ,0 ,0 ,0 ,1);
    Phi_6C_array_6F9038[104] = Phi_6C(163 ,1 ,0 ,164 ,164 ,0 ,1 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,1 ,0 ,0 ,1 ,0 ,1 ,0 ,1);
    Phi_6C_array_6F9038[105] = Phi_6C(164 ,2 ,2 ,185 ,185 ,1 ,6 ,0 ,3 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,10 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,1 ,0 ,0 ,1 ,0 ,1 ,0 ,1);
    Phi_6C_array_6F9038[106] = Phi_6C(281 ,1 ,0 ,282 ,0 ,1 ,11 ,13 ,4 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 , DAT_006fceb0 * Fix16(48) , DAT_006fceb0 * Fix16(52) ,dword_6FCE08 ,10 ,0 ,0 ,1 ,0 ,1 ,0 ,1);
    Phi_6C_array_6F9038[107] = Phi_6C(282 ,1 ,0 ,0 ,0 ,0 ,0 ,12 ,4 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 , DAT_006fceb0 * Fix16(48) ,DAT_006fceb0 * Fix16(52) ,dword_6FCE08 ,10 ,0 ,0 ,1 ,0 ,0 ,0 ,1);
    Phi_6C_array_6F9038[108] = Phi_6C(256 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,2 ,0 ,0 ,1 ,0 ,0 ,0 ,1);
    Phi_6C_array_6F9038[109] = Phi_6C(253 ,4 ,2 ,0 ,0 ,1 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,45 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,2 ,0 ,0 ,2 ,0 ,0 ,0 ,1);
    Phi_6C_array_6F9038[110] = Phi_6C(144 ,4 ,2 ,0 ,0 ,1 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,45 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,2 ,0 ,0 ,2 ,0 ,0 ,0 ,1);
    Phi_6C_array_6F9038[111] = Phi_6C(121 ,4 ,2 ,0 ,0 ,1 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,45 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,2 ,0 ,0 ,2 ,0 ,0 ,0 ,1);
    Phi_6C_array_6F9038[112] = Phi_6C(118 ,4 ,2 ,0 ,0 ,1 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,45 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,2 ,0 ,0 ,2 ,0 ,0 ,0 ,1);
    Phi_6C_array_6F9038[113] = Phi_6C(295 ,8 ,2 ,0 ,0 ,1 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,-1 ,1 ,1 ,4 ,3 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,5 ,0 ,1 ,1 ,0 ,0 ,1 ,1);
    Phi_6C_array_6F9038[114] = Phi_6C(128 ,1 ,3 ,0 ,0 ,1 ,7 ,0 ,1 ,dword_6F8FE8 ,dword_6FCE08 ,50 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,28 ,4 ,1 ,1 ,1 ,1 ,0 ,1);
    Phi_6C_array_6F9038[115] = Phi_6C(192 ,0 ,3 ,259 ,259 ,1 ,8 ,0 ,2 ,dword_6F8FE8 ,dword_6FCE08 ,8 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,dword_6FCE60 ,dword_6FCE60 ,dword_6FCE60 ,0 ,0 ,1 ,1 ,1 ,0 ,0 ,1);
    Phi_6C_array_6F9038[116] = Phi_6C(259 ,5 ,2 ,0 ,0 ,1 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,1 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,2 ,0 ,1 ,2 ,0 ,0 ,0 ,1);
    Phi_6C_array_6F9038[117] = Phi_6C(148 ,2 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE0C ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,28 ,0 ,1 ,1 ,0 ,0 ,0 ,1);
    Phi_6C_array_6F9038[118] = Phi_6C(170 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,29 ,0 ,0 ,1 ,0 ,0 ,0 ,1);
    Phi_6C_array_6F9038[119] = Phi_6C(255 ,3 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,4 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,32 ,4 ,0 ,1 ,0 ,0 ,0 ,1);
    Phi_6C_array_6F9038[120] = Phi_6C(294 ,0 ,0 ,0 ,0 ,0 ,0 ,12 ,4 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,dword_6FCE0C ,dword_6FCE0C ,dword_6FCE0C ,0 ,0 ,0 ,1 ,0 ,0 ,0 ,1);
    Phi_6C_array_6F9038[121] = Phi_6C(113 ,0 ,5 ,0 ,0 ,1 ,0 ,0 ,2 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,0 ,0 ,0 ,1 ,0 ,1 ,0 ,1);
    Phi_6C_array_6F9038[122] = Phi_6C(278 ,0 ,12 ,0 ,0 ,0 ,0 ,0 ,5 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,0 ,0 ,1 ,1 ,0 ,1 ,0 ,1);
    Phi_6C_array_6F9038[123] = Phi_6C(279 ,0 ,12 ,0 ,0 ,1 ,0 ,0 ,5 ,dword_6FCE08 ,dword_6FCE08 ,0 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,dword_6FCE08 ,0 ,0 ,1 ,1 ,0 ,1 ,0 ,1);
    Phi_6C_array_6F9038[124] = Phi_6C(198 ,0 ,3 ,198 ,198 ,0 ,8 ,0 ,2 ,dword_6FCE0C ,dword_6FCE08 ,1 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,dword_6FCE60 ,dword_6FCE60 ,dword_6FCE60 ,0 ,0 ,1 ,1 ,1 ,0 ,0 ,1);
    Phi_6C_array_6F9038[125] = Phi_6C(251 ,0 ,3 ,0 ,0 ,1 ,0 ,0 ,5 ,dword_6FCE08 ,dword_6FCE08 ,1 ,1 ,1 ,1 ,0 ,0 ,dword_6FCE08 ,dword_6FCE60 ,dword_6FCE60 ,dword_6FCE60 ,0 ,0 ,1 ,1 ,1 ,0 ,0 ,1);
    printf("Phi_6C array loaded!");
}

MATCH_FUNC(0x532fb0)
Phi_54::Phi_54(s32 param_1,
               s32& param_2,
               s32 param_3,
               s8 param_4,
               s32& param_5,
               s32& param_6,
               s32& param_7,
               Fix16 param_8,
               Fix16 param_9,
               s8 param_10,
               s32& param_11,
               s32& param_12,
               s8 param_13,
               s32& param_14,
               Fix16 param_15,
               s32 param_16,
               s32 param_17,
               s32 param_18,
               s8 param_19,
               s32 param_20,
               s8 param_21)
{
    field_0 = param_1;
    field_4 = param_2;
    field_8 = param_3;
    field_C = param_4;
    field_10 = param_5;
    field_14 = param_6;
    field_18 = param_7;
    field_1C = param_8;
    field_20 = param_9;
    field_24 = param_10;
    field_28 = param_11;
    field_2C = param_12;
    field_30 = param_13;
    field_34 = param_14;
    field_38 = param_15;
    field_3C = param_16;
    field_40 = param_17;
    field_44 = param_18;
    field_48 = param_19;
    field_4C = param_20;
    field_50 = param_21;
}

MATCH_FUNC(0x531860)
Phi_6C::Phi_6C(u32 param_1,
               u8 param_2,
               u32& param_3,
               u32 param_4,
               u32 param_5,
               u8 param_6,
               u32& param_7,
               u32& param_8,
               u32& param_9,
               Fix16 param_10,
               Fix16 param_11,
               u8 param_12,
               u32& param_13,
               u32& param_14,
               u32 param_15,
               u32 param_16,
               u32& param_17,
               Fix16 param_18,
               Fix16 param_19,
               Fix16 param_20,
               Fix16 param_21,
               u32 param_22,
               u32 param_23,
               u8 param_24,
               u32& param_25,
               u32 param_26,
               u32 param_27,
               u8 param_28,
               u8 param_29)
{
    field_0 = param_1;
    field_4 = param_2;
    field_8 = param_3;
    field_C = param_4;
    field_10 = param_5;
    field_14 = param_6;
    field_18 = param_7;
    field_1C = param_8;
    field_20 = param_9;
    field_24 = param_10;
    field_28 = param_11;
    field_2C = param_12;
    field_30 = param_13;
    field_34 = param_14;
    field_38 = param_15;
    field_3C = param_16;
    field_40 = param_17;
    field_44 = param_18;
    field_48 = param_19;
    field_4C = param_20;
    field_50 = param_21;
    field_54 = param_22;
    field_58 = param_23;
    field_59 = param_24;
    field_5C = param_25;
    field_60 = param_26;
    field_64 = param_27;
    field_68 = param_28;
    field_69 = param_29;
}

MATCH_FUNC(0x4bdf60)
Phi_74::~Phi_74()
{
}

MATCH_FUNC(0x533060)
void Phi_74::sub_533060(Fix16 a2, Fix16 a3, Fix16 a4)
{
    field_0 = a2;
    field_4 = a3;
    field_8 = a4;
    if (field_0 < field_4)
    {
        field_C = a2;
    }
    else
    {
        field_C = a3;
    }
}

MATCH_FUNC(0x533090)
void Phi_74::sub_533090()
{
    if (this->field_28 != sprite_types_enum::unknown_1)
    {
        u16 sprite_id = gGtx_0x106C_703DD4->convert_sprite_pal_5AA460(field_28, field_1E);
        sprite_index * sprite = gGtx_0x106C_703DD4->get_sprite_index_5AA440(sprite_id);
        field_0 = Fix16(sprite->field_4_width) / 0x40;
        field_4 = Fix16(sprite->field_5_height) / 0x40;
        field_8 = DAT_006FCED0;
    }
    else
    {
        field_0 = DAT_006F8FF0;
        field_4 = DAT_006F8FF0;
        field_8 = DAT_006FCED0;
    }

    if (field_0 < field_4)
    {
        field_C = field_0;
    }
    else
    {
        field_C = field_4;
    }
}

MATCH_FUNC(0x533110)
void Phi_74::sub_533110(s16 remap)
{
    switch (field_28)
    {
        case 4:
            field_30 = 5;
            break;
        case 5:
            field_30 = 6;
            break;
    }
    field_1C_remap = remap;
}

MATCH_FUNC(0x533150)
void Phi_74::sub_533150(s16 a2, s16 a3)
{
    field_1E += a2;
    field_6C_sprite_anim_speed = a3;
}

MATCH_FUNC(0x533170)
Sprite* Phi_74::sub_533170()
{
    Sprite* pFreeSprite = gSprite_Pool_703818->get_new_sprite();
    sub_5331A0(pFreeSprite);
    return pFreeSprite;
}

MATCH_FUNC(0x5331a0)
void Phi_74::sub_5331A0(Sprite* pSprite)
{
    s16 f1E; // ax
    s32 f40; // eax

    pSprite->field_30_sprite_type_enum = this->field_28;
    pSprite->sub_59E960();
    f1E = this->field_1E;

    if (pSprite->field_22_sprite_id != f1E)
    {
        pSprite->field_22_sprite_id = f1E;
        pSprite->sub_59FA40();
    }

    pSprite->field_28_num = this->field_2C;
    pSprite->field_2C = this->field_20;

    if (this->field_30 != 2)
    {
        pSprite->SetRemap(this->field_1C_remap);
    }

    f40 = this->field_40;
    if (f40 > 0 && (f40 <= 3 || f40 == 4))
    {
        pSprite->AllocInternal_59F950(this->field_0, this->field_4, this->field_8);
    }

    if (this->field_70)
    {
        pSprite->CreateSoundObj_5A29D0();
    }
}

// fix16?
s32 dword_6F8FA4 = 0x3FC000;

MATCH_FUNC(0x533220)
Phi_74::Phi_74()
{
    field_0 = dword_6F8FA4;
    field_4 = dword_6F8FA4;
    field_8 = dword_6F8FA4;
    field_24_idx = 0;
    field_28 = 0;
    field_2C = 0;
    field_30 = 0;
    field_34 = 0;
    field_38 = 0;
    field_3C = 0;
    field_40 = 0;
    field_44 = 0;
    field_48 = 0;
    field_4C = 1;
    field_50 = 1;
    field_58 = 0;
    field_54_react_to_collisions_with = CollisionReaction::Always_0;
    field_C = dword_6F8FA4;
    field_10 = dword_6F8FA4;
    field_14_friction = dword_6F8FA4;
    field_1C_remap = 9999;
    field_61 = 0;
    field_64_next_frame_max = 99;
    field_65 = 99;
    field_1E = 99;
    field_6C_sprite_anim_speed = 99;
    Fix16 v1 = dword_6FCE08;
    field_68 = 0;
    field_18 = v1;
    field_20 = 0;
    field_60 = 0;
    field_5C = 1;
    field_70 = 0;
}

MATCH_FUNC(0x4bdf40)
Phi_8CA8::~Phi_8CA8()
{
}

MATCH_FUNC(0x5332d0)
Phi_74* Phi_8CA8::sub_5332D0(s32 idx, s32 a3, s16 a4, u8 a5)
{
    Phi_74* result = sub_5343C0(idx);
    result->field_28 = a3;
    result->field_1E = a4;
    result->field_6C_sprite_anim_speed = a5;
    result->field_30 = 2;
    return result;
}

MATCH_FUNC(0x533300)
void Phi_8CA8::sub_533300()
{
    s16 sprites = 0;
    u16 i = 0;
    object_info* pIter = gGtx_0x106C_703DD4->get_map_object_info_5AA910(i++);
    while (pIter)
    {
        Phi_74* pPhi = sub_5332D0(pIter->field_0_model, 5, sprites, pIter->field_1_sprites);
        sprites += pIter->field_1_sprites;
        pPhi->sub_533090();
        pIter = gGtx_0x106C_703DD4->get_map_object_info_5AA910(i++);
    }
}

MATCH_FUNC(0x533360)
void Phi_8CA8::sub_533360()
{
    int iVar4 = Phi_54_array_lenght_00623EEC;

    if (0 < iVar4)
    {
        Phi_54* puVar3 = Phi_54_array_006FC5F8;
        do
        {
            Phi_74* pPVar2 = sub_534360(puVar3->field_0);
            pPVar2->field_34 = puVar3->field_4;
            pPVar2->field_38 = puVar3->field_8;
            pPVar2->field_3C = puVar3->field_8;
            pPVar2->field_61 = puVar3->field_C;
            pPVar2->field_40 = puVar3->field_18;
            pPVar2->field_44 = puVar3->field_10;
            pPVar2->field_48 = puVar3->field_14;
            pPVar2->field_10 = puVar3->field_1C;
            pPVar2->field_65 = puVar3->field_24;
            pPVar2->field_14_friction = puVar3->field_20;
            pPVar2->field_4C = puVar3->field_28;
            pPVar2->field_64_next_frame_max = puVar3->field_30;
            pPVar2->field_58 = puVar3->field_34;
            pPVar2->field_18 = puVar3->field_38;
            pPVar2->field_20 = puVar3->field_40;
            pPVar2->field_2C = puVar3->field_3C;
            pPVar2->field_60 = 1;
            pPVar2->field_5C = puVar3->field_4C;
            pPVar2->field_68 = 0;
            pPVar2->field_70 = puVar3->field_44;
            pPVar2->field_62_has_shadows = puVar3->field_48;
            pPVar2->field_63 = puVar3->field_50;
            pPVar2->field_54_react_to_collisions_with = CollisionReaction::Always_0;
            puVar3++;
        } while (--iVar4 != 0);
    }
}

MATCH_FUNC(0x533420)
void Phi_8CA8::sub_533420()
{
    Phi_74* pAVar1;

    sub_534370(0x97, 6);
    pAVar1 = sub_534360(0x97);
    pAVar1->sub_533150(1, pAVar1->field_6C_sprite_anim_speed + -1);
    pAVar1->field_34 = 2;
    pAVar1->field_64_next_frame_max = '\x05';
    pAVar1->field_38 = 0x98;
    pAVar1->field_3C = 0x98;
    pAVar1->field_40 = 0;
    pAVar1->field_44 = 0;
    pAVar1->field_65 = '\x01';
    pAVar1->field_61 = '\x01';

    pAVar1 = sub_534370(0x98, 6);
    pAVar1->sub_533150(pAVar1->field_6C_sprite_anim_speed - 1, 1);
    pAVar1->field_34 = 0;
    pAVar1->field_38 = 0;
    pAVar1->field_3C = 0;
    pAVar1->field_40 = 0;
    pAVar1->field_44 = 0;
    pAVar1->field_61 = '\0';

    pAVar1 = sub_534370(0x34, 4);
    pAVar1->sub_533150(pAVar1->field_6C_sprite_anim_speed - 1, 1);
    pAVar1->field_34 = 0;
    pAVar1->field_38 = 0;
    pAVar1->field_3C = 0;
    pAVar1->field_40 = 0;
    pAVar1->field_44 = 0;
    pAVar1->field_61 = '\0';

    sub_534370(0x32, 0xc);
    pAVar1 = sub_534360(0x32);
    pAVar1->sub_533150(1, pAVar1->field_6C_sprite_anim_speed + -1);
    pAVar1->field_34 = 2;
    pAVar1->field_64_next_frame_max = '\x03';
    pAVar1->field_38 = 0x33;
    pAVar1->field_3C = 0x33;
    pAVar1->field_40 = 0;
    pAVar1->field_44 = 0;
    pAVar1->field_65 = '\x01';
    pAVar1->field_61 = '\x01';

    pAVar1 = sub_534370(0x33, 0xc);
    pAVar1->sub_533150(pAVar1->field_6C_sprite_anim_speed - 1, 1);
    pAVar1->field_34 = 0;
    pAVar1->field_38 = 0;
    pAVar1->field_3C = 0;
    pAVar1->field_40 = 0;
    pAVar1->field_44 = 0;
    pAVar1->field_61 = '\0';

    pAVar1 = sub_534370(0x9b, 3);
    pAVar1->sub_533150(1, pAVar1->field_6C_sprite_anim_speed + -1);
    pAVar1->field_34 = 4;
    pAVar1->field_64_next_frame_max = 1;
    pAVar1->field_38 = 0x35;
    pAVar1->field_3C = 0x35;
    pAVar1->field_40 = 4;
    pAVar1->field_44 = 0;
    pAVar1->field_10 = DAT_006f8fd8;
    pAVar1->field_14_friction = -DAT_006fcdd8;
    pAVar1->field_61 = 1;
    pAVar1->field_4C = 3;
    pAVar1->field_50 = 3;
    pAVar1->field_65 = -1;
    pAVar1->field_58 = 1;

    pAVar1 = sub_534370(0x35, 3);
    pAVar1->sub_533150(pAVar1->field_6C_sprite_anim_speed - 1, 1);

    pAVar1 = sub_534370(0x7b, 0xb);
    pAVar1->sub_533150(1, pAVar1->field_6C_sprite_anim_speed + -1);
    pAVar1->field_34 = 4;
    pAVar1->field_64_next_frame_max = '\x01';
    pAVar1->field_38 = 0x37;
    pAVar1->field_3C = 0x37;
    pAVar1->field_40 = 4;
    pAVar1->field_44 = 0;
    pAVar1->field_10 = DAT_006f8fd8;
    pAVar1->field_14_friction = -DAT_006fcdd8;
    pAVar1->field_61 = '\x01';
    pAVar1->field_4C = 3;
    pAVar1->field_50 = 3;
    pAVar1->field_65 = -1;
    pAVar1->field_58 = 1;

    pAVar1 = sub_534370(0x37, 0xb);
    pAVar1->sub_533150(pAVar1->field_6C_sprite_anim_speed - 1, 1);

    pAVar1 = sub_534370(0x9c, 5);
    pAVar1->sub_533150(1, pAVar1->field_6C_sprite_anim_speed + -1);
    pAVar1->field_34 = 4;
    pAVar1->field_64_next_frame_max = '\x01';
    pAVar1->field_38 = 0x36;
    pAVar1->field_3C = 0x36;
    pAVar1->field_40 = 4;
    pAVar1->field_44 = 0;
    pAVar1->field_10 = DAT_006f8fd8;
    pAVar1->field_14_friction = -DAT_006fcdd8;
    pAVar1->field_61 = '\x01';
    pAVar1->field_4C = 3;
    pAVar1->field_50 = 3;
    pAVar1->field_65 = -1;
    pAVar1->field_58 = 1;

    pAVar1 = sub_534370(0x36, 5);
    pAVar1->sub_533150(pAVar1->field_6C_sprite_anim_speed - 1, 1);

    pAVar1 = sub_534370(0x38, 0xd);
    pAVar1->sub_533150(1, pAVar1->field_6C_sprite_anim_speed + -1);
    pAVar1->field_34 = 4;
    pAVar1->field_64_next_frame_max = '\x01';
    pAVar1->field_38 = 0xd;
    pAVar1->field_3C = 0xd;
    pAVar1->field_40 = 4;
    pAVar1->field_44 = 0;
    pAVar1->field_10 = DAT_006f8fd8;
    pAVar1->field_14_friction = -DAT_006fcdd8;
    pAVar1->field_61 = '\x01';
    pAVar1->field_4C = 3;
    pAVar1->field_50 = 3;
    pAVar1->field_65 = -1;
    pAVar1->field_58 = 1;

    pAVar1 = sub_534370(0x39, 0xe);
    pAVar1->sub_533150(1, pAVar1->field_6C_sprite_anim_speed + -1);
    pAVar1->field_34 = 4;
    pAVar1->field_64_next_frame_max = '\x01';
    pAVar1->field_38 = 0x3a;
    pAVar1->field_3C = 0x3a;
    pAVar1->field_40 = 4;
    pAVar1->field_44 = 0;
    pAVar1->field_10 = DAT_006f8fd8;
    pAVar1->field_14_friction = -DAT_006fcdd8;
    pAVar1->field_61 = '\x01';
    pAVar1->field_4C = 3;
    pAVar1->field_50 = 3;
    pAVar1->field_65 = -1;
    pAVar1->field_58 = 1;

    pAVar1 = sub_534370(0x3a, 0xe);
    pAVar1->sub_533150(pAVar1->field_6C_sprite_anim_speed - 1, 1);

    pAVar1 = sub_534370(0x3b, 0xf);
    pAVar1->sub_533150(1, pAVar1->field_6C_sprite_anim_speed + -1);
    pAVar1->field_34 = 4;
    pAVar1->field_64_next_frame_max = '\x01';
    pAVar1->field_38 = 0x3c;
    pAVar1->field_3C = 0x3c;
    pAVar1->field_40 = 4;
    pAVar1->field_44 = 0;
    pAVar1->field_10 = DAT_006f8fd8;
    pAVar1->field_14_friction = -DAT_006fcdd8;
    pAVar1->field_61 = '\x01';
    pAVar1->field_4C = 3;
    pAVar1->field_50 = 3;
    pAVar1->field_65 = -1;
    pAVar1->field_58 = 1;

    pAVar1 = sub_534370(0x3c, 0xf);
    pAVar1->sub_533150(pAVar1->field_6C_sprite_anim_speed - 1, 1);

    pAVar1 = sub_534370(0x3d, 0x10);
    pAVar1->sub_533150(1, pAVar1->field_6C_sprite_anim_speed + -1);
    pAVar1->field_34 = 4;
    pAVar1->field_64_next_frame_max = '\x01';
    pAVar1->field_38 = 0x3e;
    pAVar1->field_3C = 0x3e;
    pAVar1->field_40 = 4;
    pAVar1->field_44 = 0;
    pAVar1->field_10 = DAT_006f8fd8;
    pAVar1->field_14_friction = -DAT_006fcdd8;
    pAVar1->field_61 = '\x01';
    pAVar1->field_4C = 3;
    pAVar1->field_50 = 3;
    pAVar1->field_65 = -1;
    pAVar1->field_58 = 1;

    pAVar1 = sub_534370(0x3e, 0x10);
    pAVar1->sub_533150(pAVar1->field_6C_sprite_anim_speed - 1, 1);

    pAVar1 = sub_534370(0x31, 0x12);
    pAVar1->sub_533150(1, pAVar1->field_6C_sprite_anim_speed + -1);
    pAVar1->field_34 = 4;
    pAVar1->field_64_next_frame_max = '\x01';
    pAVar1->field_38 = 0x12;
    pAVar1->field_3C = 0x12;
    pAVar1->field_40 = 4;
    pAVar1->field_44 = 0;
    pAVar1->field_10 = DAT_006f8fd8;
    pAVar1->field_14_friction = -DAT_006fcdd8;
    pAVar1->field_61 = '\x01';
    pAVar1->field_4C = 3;
    pAVar1->field_50 = 3;
    pAVar1->field_65 = -1;
    pAVar1->field_58 = 1;

    pAVar1 = sub_534370(0x2d, 0x16);
    pAVar1->sub_533150(1, pAVar1->field_6C_sprite_anim_speed + -1);
    pAVar1->field_34 = 4;
    pAVar1->field_64_next_frame_max = '\x01';
    pAVar1->field_38 = 0x2e;
    pAVar1->field_3C = 0x2e;
    pAVar1->field_40 = 4;
    pAVar1->field_44 = 0;
    pAVar1->field_10 = DAT_006f8fd8;
    pAVar1->field_14_friction = -DAT_006fcdd8;
    pAVar1->field_61 = '\x01';
    pAVar1->field_4C = 3;
    pAVar1->field_50 = 3;
    pAVar1->field_65 = -1;
    pAVar1->field_58 = 1;

    pAVar1 = sub_534370(0x2e, 0x16);
    pAVar1->sub_533150(pAVar1->field_6C_sprite_anim_speed - 1, 1);

    pAVar1 = sub_534370(0x2f, 0x15);
    pAVar1->sub_533150(1, pAVar1->field_6C_sprite_anim_speed + -1);
    pAVar1->field_34 = 4;
    pAVar1->field_64_next_frame_max = '\x01';
    pAVar1->field_38 = 0x30;
    pAVar1->field_3C = 0x30;
    pAVar1->field_40 = 4;
    pAVar1->field_44 = 0;
    pAVar1->field_10 = DAT_006f8fd8;
    pAVar1->field_14_friction = -DAT_006fcdd8;
    pAVar1->field_61 = '\x01';
    pAVar1->field_4C = 3;
    pAVar1->field_50 = 3;
    pAVar1->field_65 = -1;
    pAVar1->field_58 = 1;

    pAVar1 = sub_534370(0x30, 0x15);
    pAVar1->sub_533150(pAVar1->field_6C_sprite_anim_speed - 1, 1);

    pAVar1 = sub_534370(0x3f, 0x11);
    pAVar1->field_38 = 0x11;
    pAVar1->field_34 = 3;
    pAVar1->field_3C = 0x11;
    pAVar1->field_40 = 4;
    pAVar1->field_44 = 0;
    pAVar1->field_10 = DAT_006f8fd8;
    pAVar1->field_14_friction = -DAT_006fcdd8;
    pAVar1->field_4C = 3;
    pAVar1->field_50 = 3;
    pAVar1->field_61 = '\x01';
    pAVar1->field_58 = 1;

    pAVar1 = sub_534370(0x2b, 0x17);
    pAVar1->sub_533150(1, pAVar1->field_6C_sprite_anim_speed + -1);
    pAVar1->field_34 = 4;
    pAVar1->field_64_next_frame_max = '\x01';
    pAVar1->field_38 = 0x2c;
    pAVar1->field_3C = 0x2c;
    pAVar1->field_40 = 4;
    pAVar1->field_44 = 0;
    pAVar1->field_10 = DAT_006f8fd8;
    pAVar1->field_14_friction = -DAT_006fcdd8;
    pAVar1->field_61 = '\x01';
    pAVar1->field_4C = 3;
    pAVar1->field_50 = 3;
    pAVar1->field_65 = -1;
    pAVar1->field_58 = 1;

    pAVar1 = sub_534370(0x2c, 0x17);
    pAVar1->sub_533150(pAVar1->field_6C_sprite_anim_speed - 1, 1);

    pAVar1 = sub_534370(0x9d, 7);
    pAVar1->field_4C = 2;
    pAVar1->field_34 = 3;
    pAVar1->field_14_friction = -DAT_006fcdd8;
    pAVar1->field_38 = 7;
    pAVar1->field_3C = 7;
    pAVar1->field_40 = 4;
    pAVar1->field_44 = 0;
    pAVar1->field_61 = '\x01';
    pAVar1->field_58 = 1;

    pAVar1 = sub_534370(0x9e, 1);
    pAVar1->field_34 = 3;
    pAVar1->field_4C = 2;
    pAVar1->field_14_friction = -DAT_006fcdd8;
    pAVar1->field_38 = 1;
    pAVar1->field_3C = 1;
    pAVar1->field_44 = 0;
    pAVar1->field_61 = '\x01';
    pAVar1->field_58 = 1;
    pAVar1 = sub_534360(0x19);
    pAVar1->field_28 = 1;
    pAVar1->field_8 = DAT_006fce10;
    return;
}

MATCH_FUNC(0x533b30)
void Phi_8CA8::sub_533B30()
{
    u16 sprite_base = 0;
    if (0 < Phi_6C_array_lenght_623EF0)
    {
        Phi_6C* puVar2 = &Phi_6C_array_6F9038[0];
        for(s32 local_8 = Phi_6C_array_lenght_623EF0; local_8 != 0; local_8--)
        {
            Phi_74* this_00 = sub_5332D0(puVar2->field_0, 4, sprite_base, puVar2->field_4);
            sprite_base += puVar2->field_4;
            this_00->field_28 = puVar2->field_38;
            if (puVar2->field_48 == DAT_006fce08 && puVar2->field_4C == DAT_006fce08 && puVar2->field_50 == DAT_006fce08)
            {
                this_00->sub_533090();
            }
            else
            {
                this_00->sub_533060(puVar2->field_48, puVar2->field_4C, puVar2->field_50);
            }

            this_00->field_34 = puVar2->field_8;
            this_00->field_38 = puVar2->field_C;
            this_00->field_3C = puVar2->field_10;
            this_00->field_61 = puVar2->field_14;
            this_00->field_40 = puVar2->field_20;
            this_00->field_44 = puVar2->field_18;
            this_00->field_48 = puVar2->field_1C;
            this_00->field_10 = puVar2->field_24;
            this_00->field_65 = puVar2->field_2C;
            this_00->field_14_friction = puVar2->field_28;
            this_00->field_4C = puVar2->field_30;
            this_00->field_50 = puVar2->field_34;
            this_00->field_64_next_frame_max = puVar2->field_3C;
            this_00->field_54_react_to_collisions_with = CollisionReaction::Always_0;
            this_00->field_58 = puVar2->field_40;
            this_00->field_18 = puVar2->field_44;
            this_00->field_2C = puVar2->field_54;
            this_00->field_20 = puVar2->field_58;
            this_00->field_60 = puVar2->field_59;
            this_00->field_5C = puVar2->field_5C;
            this_00->field_68 = puVar2->field_60;
            this_00->field_70 = puVar2->field_64;
            this_00->field_62_has_shadows = puVar2->field_68;
            this_00->field_63 = puVar2->field_69;
            puVar2++;
        }
    }
}

MATCH_FUNC(0x533c90)
void Phi_8CA8::sub_533C90()
{
    Phi_74* tmp;
    tmp = sub_534370(0x93, 0x90);
    tmp->sub_533110(4);
    tmp = sub_534370(0xfa, 0xfd);
    tmp->sub_533110(4);
    tmp = sub_534370(0x78, 0x79);
    tmp->sub_533110(4);
    tmp = sub_534370(0x75, 0x76);
    tmp->sub_533110(4);
    tmp = sub_534370(0x92, 0x90);
    tmp->sub_533110(3);
    tmp = sub_534370(0xf9, 0xfd);
    tmp->sub_533110(3);
    tmp = sub_534370(0x77, 0x79);
    tmp->sub_533110(3);
    tmp = sub_534370(0x74, 0x76);
    tmp->sub_533110(3);
    tmp = sub_534370(0x91, 0x90);
    tmp->sub_533110(5);
    tmp->field_64_next_frame_max = 3;

    tmp = sub_534370(0x7c, 0xfd);
    tmp->sub_533110(5);
    tmp->field_64_next_frame_max = 3;
    tmp = sub_534370(0x7d, 0x79);
    tmp->sub_533110(5);
    tmp->field_64_next_frame_max = 3;
    tmp = sub_534370(0x7e, 0x76);
    tmp->sub_533110(5);
    tmp->field_64_next_frame_max = 3;

    tmp = sub_534370(0x9a, 0xfe);
    if (bDo_show_imaginary_67D588 == false)
    {
        tmp->field_40 = 2;
    }
    sub_534370(0x109, 0xfe);
    tmp = sub_534370(0x9f, 0x80);
    if (bDo_show_imaginary_67D588 == false)
    {
        tmp->field_40 = 2;
    }

    sub_534370(0x115, 0xfe);
    tmp = sub_534370(0x6e, 0x9b);
    tmp->field_38 = 0x6e;
    tmp->field_3C = 0x6e;
    tmp->field_54_react_to_collisions_with = CollisionReaction::OnlyPeds_2;
    tmp->sub_533060(DAT_006fc578, DAT_006fc578, DAT_006f8fac);
    tmp->field_18 = DAT_006fc584;
    if (bDo_show_imaginary_67D588 == false)
    {
        tmp->field_40 = 2;
    }
    else
    {
        tmp->field_40 = 1;
    }
    tmp->field_44 = 2;
    tmp->field_58 = 1;
    tmp->field_50 = 1;

    tmp = sub_534370(0x7f, 0x9b);
    tmp->field_38 = 0;
    tmp->field_3C = 0;
    tmp->field_54_react_to_collisions_with = CollisionReaction::OnlyPeds_2;
    tmp->sub_533060((DAT_006fceb0 * 8), (DAT_006fceb0 * 8), DAT_006f8fac);
    tmp->field_18 = DAT_006fc584;
    if (bDo_show_imaginary_67D588 == '\0')
    {
        tmp->field_40 = 2;
    }
    else
    {
        tmp->field_40 = 1;
    }
    tmp->field_44 = 2;
    tmp->field_58 = 1;
    tmp->field_50 = 1;

    tmp = sub_534370(0xc1, 0xc0);
    tmp->field_38 = 0;
    tmp->field_3C = 0;
    if (bDo_show_imaginary_67D588 == '\0')
    {
        tmp->field_40 = 2;
    }

    tmp = sub_534370(0xb9, 0xa3);
    tmp->field_38 = 0xb8;
    tmp->field_3C = 0xb8;
    tmp->field_44 = 4;
    tmp->field_40 = 3;
    tmp->field_61 = '\x01';

    tmp = sub_534370(0xb8, 0xa3);
    tmp->field_38 = 0xb9;
    tmp->field_3C = 0xa4;
    tmp->field_65 = 'd';
    tmp->field_61 = '\x01';
    tmp->field_34 = 2;
    tmp->field_6C_sprite_anim_speed = '\x01';
    tmp->field_64_next_frame_max = '\x01';
    tmp->field_40 = 3;
    tmp->field_44 = 5;

    tmp = sub_534370(0xb0, 0xa3);
    tmp->sub_533110(1);
    tmp->field_38 = 0xb1;
    tmp->field_3C = 0xb1;
    if (bDo_kill_phones_on_answer_67D6E8 != '\0')
    {
        tmp = sub_534360(0xa4);
        tmp->field_38 = 0xae;
        tmp->field_3C = 0xae;
    }
    tmp = sub_534370(0xb1, 0xa4);
    tmp->sub_533110(1);
    if (bDo_kill_phones_on_answer_67D6E8 == '\0')
    {
        tmp->field_38 = 0xbb;
        tmp->field_3C = 0xbb;
    }

    tmp = sub_534370(0xbb, 0xb9);
    tmp->sub_533110(1);
    tmp->field_38 = 0xba;
    tmp->field_3C = 0xba;

    tmp = sub_534370(0xba, 0xb8);
    tmp->sub_533110(1);
    tmp->field_38 = 0xbb;
    tmp->field_3C = 0xb1;

    tmp = sub_534370(0xb2, 0xa3);
    tmp->sub_533110(2);
    tmp->field_38 = 0xb3;
    tmp->field_3C = 0xb3;

    tmp = sub_534370(0xb3, 0xa4);
    tmp->sub_533110(2);
    if (bDo_kill_phones_on_answer_67D6E8 == '\0')
    {
        tmp->field_38 = 0xbd;
        tmp->field_3C = 0xbd;
    }
    tmp = sub_534370(0xbd, 0xb9);
    tmp->sub_533110(2);
    tmp->field_38 = 0xbc;
    tmp->field_3C = 0xbc;

    tmp = sub_534370(0xbc, 0xb8);
    tmp->sub_533110(2);
    tmp->field_38 = 0xbd;
    tmp->field_3C = 0xb3;

    tmp = sub_534370(0xb4, 0xa3);
    tmp->sub_533110(0);
    tmp->field_38 = 0xb5;
    tmp->field_3C = 0xb5;

    tmp = sub_534370(0xb5, 0xa4);
    tmp->sub_533110(0);
    if (bDo_kill_phones_on_answer_67D6E8 == '\0')
    {
        tmp->field_38 = 0xbf;
        tmp->field_3C = 0xbf;
    }

    tmp = sub_534370(0xbf, 0xb9);
    tmp->sub_533110(0);
    tmp->field_38 = 0xbe;
    tmp->field_3C = 0xbe;

    tmp = sub_534370(0xbe, 0xb8);
    tmp->sub_533110(0);
    tmp->field_38 = 0xbf;
    tmp->field_3C = 0xb5;

    tmp = sub_534370(0xae, 0xa3);
    tmp->field_44 = 0;

    tmp = sub_534370(0xb6, 0xb7);
    tmp->field_38 = 0xb7;
    tmp->field_34 = 2;
    tmp->field_44 = 2;

    tmp->field_14_friction = DAT_006fce08;
    tmp->field_10 = DAT_006fce08;
    tmp->field_4C = 0;
    tmp->field_50 = 0;
    tmp->field_64_next_frame_max = '\b';
    tmp->sub_533150(tmp->field_6C_sprite_anim_speed - 1, 1);

    for(s32 iVar3 = 200; iVar3 - 200 <= 44; iVar3++)
    {
        tmp = sub_534370(iVar3 - 0x88, iVar3);
        tmp->field_38 = iVar3;
        tmp->field_34 = tmp->field_34 == 6 ? 7 : 9;
        tmp->field_14_friction = -DAT_006fcdd8;
        tmp->field_4C = 1;
        tmp->field_50 = 1;
        tmp->field_61 = '\x01';
        tmp->field_58 = 1;
        tmp->field_40 = 3;
        tmp->field_18 = DAT_006f8fd8;
        tmp->field_10 = DAT_006fcde0;
    }

    sub_534270();
}

MATCH_FUNC(0x534270)
void Phi_8CA8::sub_534270()
{
    sub_5342F0(287);
    sub_5342F0(288);
    sub_5342F0(289);
    sub_5342F0(290);
    sub_5342F0(291);
    sub_5342F0(292);
    sub_5342F0(293);
}

MATCH_FUNC(0x5342d0)
void Phi_8CA8::sub_5342D0()
{
    field_8CA4 = sub_534360(112)->field_1E;
}

MATCH_FUNC(0x5342f0)
void Phi_8CA8::sub_5342F0(s32 idx)
{
    Phi_74* v2 = sub_534360(idx);
    u16 v3 = gGtx_0x106C_703DD4->convert_sprite_pal_5AA460(4, v2->field_1E);
    sprite_index* psprite_index = gGtx_0x106C_703DD4->get_sprite_index_5AA440(v3);
    psprite_index->sub_5ABAA0(1);
}

MATCH_FUNC(0x534330)
void Phi_8CA8::sub_534330()
{
    sub_533300();
    sub_533360();
    sub_533420();
    sub_533B30();
    sub_533C90();
    sub_5342D0();
}

MATCH_FUNC(0x534360)
Phi_74* Phi_8CA8::sub_534360(s32 idx)
{
    return field_87F4[idx];
}

MATCH_FUNC(0x534370)
Phi_74* Phi_8CA8::sub_534370(s32 idx1, s32 idx2)
{
    Phi_74* result = &field_4[field_0_next_idx];
    field_87F4[idx1] = result;
    ++field_0_next_idx;
    memcpy(result, field_87F4[idx2], sizeof(Phi_74));
    result->field_24_idx = idx1;
    return result;
}

MATCH_FUNC(0x5343c0)
Phi_74* Phi_8CA8::sub_5343C0(s32 idx)
{
    Phi_74* result = &field_4[field_0_next_idx];
    field_87F4[idx] = result;
    ++field_0_next_idx;
    result->field_24_idx = idx;
    return result;
}

MATCH_FUNC(0x5343f0)
Phi_8CA8::Phi_8CA8()
{
    for (s32 i = 0; i < GTA2_COUNTOF(field_87F4); i++)
    {
        field_87F4[i] = 0;
    }
    field_0_next_idx = 0;
    field_8CA4 = 99;
}