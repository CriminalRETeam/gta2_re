#include "ObjectDefinitions.hpp"
#include "enums.hpp"
#include "Globals.hpp"
#include "debug.hpp"
#include "gtx_0x106C.hpp"
#include "sprite.hpp"
#include <memory.h>

DEFINE_GLOBAL(ObjectDefinitions*, gObjectDefinitions_6FCF00, 0x6FCF00);

DEFINE_GLOBAL_INIT(Fix16, kFpQuarter_6F8FAC, Fix16(4096, 0), 0x6f8fac);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint1_6F8FD8, Fix16(1638, 0), 0x6F8FD8);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint2_6FC578, Fix16(3276, 0), 0x6fc578);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_6FC584, Fix16(8192, 0), 0x6fc584);
DEFINE_GLOBAL_INIT(Fix16, kFpZero_6FCE08, Fix16(0, 0), 0x6fce08);
DEFINE_GLOBAL_INIT(Fix16, kFpOne64th_6FCEB0, Fix16(256, 0), 0x6fceb0);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint02_6FCDE0, Fix16(327, 0), 0x6fcde0);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint01_6FCDD8, Fix16(163, 0), 0x6FCDD8);
DEFINE_GLOBAL_INIT(Fix16, kFpTwo_6FCE10, Fix16(2), 0x6FCE10);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint6_6F8FF0, Fix16(9830, 0), 0x6F8FF0);
DEFINE_GLOBAL_INIT(Fix16, kFpQuarter_6FCED0, Fix16(4096, 0), 0x6FCED0);

DEFINE_GLOBAL_INIT(Fix16, kFpOne_6FCE0C, Fix16(1), 0x6FCE0C);
DEFINE_GLOBAL_INIT(Fix16, kFpThree_6FCE14, Fix16(3), 0x6FCE14);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint2_6F8FE0, Fix16(3276, 0), 0x6F8FE0);
DEFINE_GLOBAL_INIT(Fix16, kFpFive_6FCE1C, Fix16(5), 0x6FCE1C);
DEFINE_GLOBAL_INIT(Fix16, kFpQuarter_6F8FDC, Fix16(0x1000, 0), 0x6F8FDC);
DEFINE_GLOBAL_INIT(Fix16, kFpEight_6FCE28, Fix16(8), 0x6FCE28);
DEFINE_GLOBAL_INIT(Fix16, kFpQuarter_6FCE60, Fix16(4096, 0), 0x6FCE60);
DEFINE_GLOBAL_INIT(Fix16, kFpHalf_6F8FEC, Fix16(0x2000, 0), 0x6F8FEC);
DEFINE_GLOBAL_INIT(Fix16, kFpSqrt2_6FC5A4, Fix16(23170, 0), 0x6FC5A4);
DEFINE_GLOBAL_INIT(Fix16, kFpPoint4_6F8FE8, Fix16(6553, 0), 0x6F8FE8);

Fix16 kFpZeroForStaticInit_6FCE08 = 0;

DEFINE_GLOBAL_INIT(s32, gCodeObjectTemplates_length_623EF0, 126, 0x623EF0);

// Must remain a fixed array, a pointer would break ObjectDefinitions::CreateCodeObjectDefinitions_533B30
DEFINE_GLOBAL_ARRAY_INIT(CodeObjectTemplate,
                         gCodeObjectTemplates_6F9038,
                         126,
                         0x6F9038,
                         CodeObjectTemplate(149, 1, 0, 0, 0, 0, 0, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 0, 0, kFpOne_6FCE0C, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 28, 0, 1, 1, 0, 0, 0, 1) COMMA
                         CodeObjectTemplate(109, 1, 0, 0, 0, 0, 0, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 2, 0, 1, 1, 0, 0, 0, 1) COMMA
                         CodeObjectTemplate(257, 1, 0, 0, 0, 0, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 2, 0, 1, 1, 0, 0, 0, 1) COMMA
                         CodeObjectTemplate(111, 5, 2, 0, 0, 1, 0, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 1, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 2, 0, 1, 2, 0, 0, 0, 1) COMMA
                         CodeObjectTemplate(247, 1, 0, 0, 0, 0, 0, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, -1, 1, 1, 4, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 5, 0, 1, 1, 0, 0, 0, 1) COMMA
                         CodeObjectTemplate(266, 1, 1, 0, 0, 0, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, -1, 1, 1, 4, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 7, 0, 0, 1, 0, 0, 0, 1) COMMA
                         CodeObjectTemplate(175, 5, 2, 0, 0, 1, 0, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, -1, 1, 1, 4, 3, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 5, 0, 1, 1, 0, 0, 0, 1) COMMA
                         CodeObjectTemplate(254, 3, 3, 0, 0, 1, 7, 0, 1, kFpPoint2_6F8FE0, kFpZero_6FCE08, 25, 1, 1, 4, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 28, 4, 1, 1, 1, 0, 0, 1) COMMA
                         CodeObjectTemplate(129, 0, 0, 0, 0, 0, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 1, 0, 0, kFpZero_6FCE08, kFpPoint6_6F8FF0, kFpPoint6_6F8FF0, kFpQuarter_6F8FDC, 0, 0, 0, 1, 0, 0, 0, 1) COMMA
                         CodeObjectTemplate(132, 0, 2, 42, 42, 0, 0, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, 120, 1, 1, 1, 1, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 0, 0, 1, 1, 0, 0, 0, 1) COMMA
                         CodeObjectTemplate(122, 0, 0, 0, 0, 0, 9, 0, 4, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 1, 0, 0, kFpZero_6FCE08, kFpOne_6FCE0C, kFpOne64th_6FCEB0 * 2, kFpQuarter_6F8FDC, 0, 0, 0, 1, 0, 0, 0, 1) COMMA
                         CodeObjectTemplate(130, 0, 10, 131, 131, 0, 6, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 1, 0, 0, kFpZero_6FCE08, kFpOne_6FCE0C, kFpOne_6FCE0C, kFpOne_6FCE0C, 7, 0, 0, 1, 0, 0, 0, 0) COMMA
                         CodeObjectTemplate(131, 0, 10, 130, 130, 1, 3, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 1, 0, 0, kFpZero_6FCE08, kFpOne_6FCE0C, kFpOne_6FCE0C, kFpOne_6FCE0C, 7, 0, 0, 1, 0, 0, 0, 0) COMMA
                         CodeObjectTemplate(139, 0, 1, 0, 0, 1, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 1, 0, 0, kFpZero_6FCE08, kFpOne_6FCE0C, kFpOne_6FCE0C, kFpZero_6FCE08, 0, 0, 0, 1, 0, 1, 0, 0) COMMA
                         CodeObjectTemplate(200, 8, 8, 0, 0, 1, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, -1, 1, 1, 4, 4, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 7, 0, 1, 1, 0, 0, 1, 1) COMMA
                         CodeObjectTemplate(201, 8, 8, 0, 0, 1, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, -1, 1, 1, 4, 4, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 7, 0, 1, 1, 0, 0, 1, 1) COMMA
                         CodeObjectTemplate(202, 8, 8, 0, 0, 1, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, -1, 1, 1, 4, 4, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 7, 0, 1, 1, 0, 0, 1, 1) COMMA
                         CodeObjectTemplate(203, 8, 8, 0, 0, 1, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, -1, 1, 1, 4, 4, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 7, 0, 1, 1, 0, 0, 1, 1) COMMA
                         CodeObjectTemplate(204, 8, 8, 0, 0, 1, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, -1, 1, 1, 4, 4, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 7, 0, 1, 1, 0, 0, 1, 1) COMMA
                         CodeObjectTemplate(205, 8, 8, 0, 0, 1, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, -1, 1, 1, 4, 4, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 7, 0, 1, 1, 0, 0, 1, 1) COMMA
                         CodeObjectTemplate(206, 8, 8, 0, 0, 1, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, -1, 1, 1, 4, 4, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 7, 0, 1, 1, 0, 0, 1, 1) COMMA
                         CodeObjectTemplate(207, 8, 8, 0, 0, 1, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, -1, 1, 1, 4, 4, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 7, 0, 1, 1, 0, 0, 1, 1) COMMA
                         CodeObjectTemplate(208, 8, 8, 0, 0, 1, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, -1, 1, 1, 4, 4, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 7, 0, 1, 1, 0, 0, 1, 1) COMMA
                         CodeObjectTemplate(209, 8, 8, 0, 0, 1, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, -1, 1, 1, 4, 4, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 7, 0, 1, 1, 0, 0, 1, 1) COMMA
                         CodeObjectTemplate(210, 8, 8, 0, 0, 1, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, -1, 1, 1, 4, 4, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 7, 0, 1, 1, 0, 0, 1, 1) COMMA
                         CodeObjectTemplate(211, 1, 6, 0, 0, 0, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 7, 0, 1, 1, 0, 0, 1, 1) COMMA
                         CodeObjectTemplate(212, 1, 6, 0, 0, 0, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 7, 0, 1, 1, 0, 0, 1, 1) COMMA
                         CodeObjectTemplate(213, 1, 6, 0, 0, 0, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 7, 0, 1, 1, 0, 0, 1, 1) COMMA
                         CodeObjectTemplate(214, 1, 6, 0, 0, 0, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 7, 0, 1, 1, 0, 0, 1, 1) COMMA
                         CodeObjectTemplate(215, 8, 8, 0, 0, 1, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, -1, 1, 1, 4, 4, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 7, 0, 1, 1, 0, 0, 1, 1) COMMA
                         CodeObjectTemplate(216, 8, 8, 0, 0, 1, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, -1, 1, 1, 4, 4, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 7, 0, 1, 1, 0, 0, 1, 1) COMMA
                         CodeObjectTemplate(217, 8, 8, 0, 0, 1, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, -1, 1, 1, 4, 4, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 7, 0, 1, 1, 0, 0, 1, 1) COMMA
                         CodeObjectTemplate(218, 8, 8, 0, 0, 1, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, -1, 1, 1, 4, 4, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 7, 0, 1, 1, 0, 0, 1, 1) COMMA
                         CodeObjectTemplate(219, 1, 6, 0, 0, 0, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 7, 0, 1, 1, 0, 0, 1, 1) COMMA
                         CodeObjectTemplate(220, 1, 6, 0, 0, 0, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 7, 0, 1, 1, 0, 0, 1, 1) COMMA
                         CodeObjectTemplate(221, 1, 6, 0, 0, 0, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 7, 0, 1, 1, 0, 0, 1, 1) COMMA
                         CodeObjectTemplate(222, 1, 6, 0, 0, 0, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 7, 0, 1, 1, 0, 0, 1, 1) COMMA
                         CodeObjectTemplate(223, 1, 6, 0, 0, 0, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 7, 0, 1, 1, 0, 0, 1, 1) COMMA
                         CodeObjectTemplate(224, 1, 6, 0, 0, 0, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 7, 0, 1, 1, 0, 0, 1, 1) COMMA
                         CodeObjectTemplate(225, 1, 6, 0, 0, 0, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 7, 0, 1, 1, 0, 0, 1, 1) COMMA
                         CodeObjectTemplate(226, 1, 6, 0, 0, 0, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 7, 0, 1, 1, 0, 0, 1, 1) COMMA
                         CodeObjectTemplate(227, 1, 6, 0, 0, 0, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 7, 0, 1, 1, 0, 0, 1, 1) COMMA
                         CodeObjectTemplate(228, 8, 8, 0, 0, 1, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, -1, 1, 1, 4, 3, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 7, 0, 1, 1, 0, 0, 1, 1) COMMA
                         CodeObjectTemplate(229, 8, 8, 0, 0, 1, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, -1, 1, 1, 4, 3, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 7, 0, 1, 1, 0, 0, 1, 1) COMMA
                         CodeObjectTemplate(230, 8, 8, 0, 0, 1, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, -1, 1, 1, 4, 3, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 7, 0, 1, 1, 0, 0, 1, 1) COMMA
                         CodeObjectTemplate(231, 8, 8, 0, 0, 1, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, -1, 1, 1, 4, 3, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 7, 0, 1, 1, 0, 0, 1, 1) COMMA
                         CodeObjectTemplate(232, 8, 8, 0, 0, 1, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, -1, 1, 1, 4, 3, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 7, 0, 1, 1, 0, 0, 1, 1) COMMA
                         CodeObjectTemplate(233, 8, 8, 0, 0, 1, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, -1, 1, 1, 4, 3, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 7, 0, 1, 1, 0, 0, 1, 1) COMMA
                         CodeObjectTemplate(234, 8, 8, 0, 0, 1, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, -1, 1, 1, 4, 3, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 7, 0, 1, 1, 0, 0, 1, 1) COMMA
                         CodeObjectTemplate(235, 8, 8, 0, 0, 1, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, -1, 1, 1, 4, 3, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 7, 0, 1, 1, 0, 0, 1, 1) COMMA
                         CodeObjectTemplate(236, 8, 8, 0, 0, 1, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, -1, 1, 1, 4, 3, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 7, 0, 1, 1, 0, 0, 1, 1) COMMA
                         CodeObjectTemplate(237, 8, 8, 0, 0, 1, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, -1, 1, 1, 4, 3, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 7, 0, 1, 1, 0, 0, 1, 1) COMMA
                         CodeObjectTemplate(238, 8, 8, 0, 0, 1, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, -1, 1, 1, 4, 3, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 7, 0, 1, 1, 0, 0, 1, 1) COMMA
                         CodeObjectTemplate(239, 8, 8, 0, 0, 1, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, -1, 1, 1, 4, 3, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 7, 0, 1, 1, 0, 0, 1, 1) COMMA
                         CodeObjectTemplate(240, 8, 8, 0, 0, 1, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, -1, 1, 1, 4, 3, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 7, 0, 1, 1, 0, 0, 1, 1) COMMA
                         CodeObjectTemplate(241, 1, 6, 0, 0, 0, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 7, 0, 1, 1, 0, 0, 1, 1) COMMA
                         CodeObjectTemplate(242, 1, 6, 0, 0, 0, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 7, 0, 1, 1, 0, 0, 1, 1) COMMA
                         CodeObjectTemplate(243, 1, 6, 0, 0, 0, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 7, 0, 1, 1, 0, 0, 1, 1) COMMA
                         CodeObjectTemplate(244, 1, 6, 0, 0, 0, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 7, 0, 1, 1, 0, 0, 1, 1) COMMA
                         CodeObjectTemplate(134, 1, 0, 0, 0, 0, 0, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 30, 0, 0, 1, 0, 0, 0, 1) COMMA
                         CodeObjectTemplate(252, 1, 0, 0, 0, 0, 0, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 32, 0, 0, 1, 0, 0, 0, 1) COMMA
                         CodeObjectTemplate(260, 1, 0, 0, 0, 0, 0, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 32, 0, 0, 1, 0, 0, 0, 1) COMMA
                         CodeObjectTemplate(261, 1, 0, 0, 0, 0, 0, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 32, 0, 0, 1, 0, 0, 0, 1) COMMA
                         CodeObjectTemplate(262, 1, 0, 0, 0, 0, 0, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 32, 0, 0, 1, 0, 0, 0, 1) COMMA
                         CodeObjectTemplate(135, 1, 0, 0, 0, 0, 0, 0, 4, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 10, 0, 0, 1, 0, 0, 0, 1) COMMA
                         CodeObjectTemplate(136, 1, 0, 0, 0, 0, 0, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 29, 0, 0, 1, 0, 0, 0, 1) COMMA
                         CodeObjectTemplate(140, 1, 0, 0, 0, 0, 0, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 30, 0, 0, 1, 0, 0, 0, 1) COMMA
                         CodeObjectTemplate(263, 1, 0, 0, 0, 0, 0, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 31, 0, 0, 1, 0, 0, 0, 1) COMMA
                         CodeObjectTemplate(137, 0, 1, 0, 0, 0, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 1, 0, 0, kFpZero_6FCE08, kFpEight_6FCE28, kFpEight_6FCE28, kFpTwo_6FCE10, 0, 0, 0, 1, 0, 0, 0, 0) COMMA
                         CodeObjectTemplate(258, 0, 0, 0, 0, 0, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 1, 0, 0, kFpZero_6FCE08, kFpPoint6_6F8FF0, kFpPoint6_6F8FF0, kFpQuarter_6F8FDC, 0, 0, 0, 1, 0, 0, 0, 0) COMMA
                         CodeObjectTemplate(142, 1, 0, 0, 0, 0, 0, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 15, 0, 0, 1, 0, 0, 0, 1) COMMA
                         CodeObjectTemplate(150, 1, 0, 0, 0, 0, 0, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 17, 0, 0, 1, 0, 0, 0, 1) COMMA
                         CodeObjectTemplate(143, 0, 1, 0, 0, 0, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 1, 0, 0, kFpZero_6FCE08, kFpOne_6FCE0C, kFpOne_6FCE0C, kFpZero_6FCE08, 0, 0, 0, 1, 0, 0, 0, 0) COMMA
                         CodeObjectTemplate(141, 0, 1, 0, 0, 1, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 1, 0, 0, kFpZero_6FCE08, kFpOne_6FCE0C, kFpOne_6FCE0C, kFpZero_6FCE08, 0, 0, 0, 1, 0, 0, 0, 0) COMMA
                         CodeObjectTemplate(167, 0, 1, 0, 0, 0, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 1, 0, 0, kFpZero_6FCE08, kFpOne_6FCE0C, kFpOne_6FCE0C, kFpOne_6FCE0C, 0, 0, 0, 1, 0, 0, 0, 0) COMMA
                         CodeObjectTemplate(168, 0, 1, 0, 0, 0, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 1, 0, 0, kFpZero_6FCE08, kFpOne64th_6FCEB0, kFpOne_6FCE0C, kFpOne_6FCE0C, 0, 0, 0, 1, 0, 0, 0, 0) COMMA
                         CodeObjectTemplate(169, 0, 1, 0, 0, 0, 0, 12, 4, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 1, 0, 0, kFpZero_6FCE08, kFpOne_6FCE0C, kFpOne_6FCE0C, kFpOne_6FCE0C, 0, 0, 0, 1, 0, 0, 0, 1) COMMA
                         CodeObjectTemplate(197, 8, 2, 0, 0, 1, 0, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, -1, 1, 1, 4, 1, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 29, 133, 1, 1, 0, 1, 0, 1) COMMA
                         CodeObjectTemplate(114, 1, 0, 0, 0, 0, 0, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 0, 0, kFpOne_6FCE0C, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 28, 0, 1, 1, 0, 0, 0, 1) COMMA
                         CodeObjectTemplate(194, 0, 3, 194, 194, 0, 8, 0, 2, kFpOne_6FCE0C, kFpZero_6FCE08, 1, 1, 1, 1, 0, 0, kFpZero_6FCE08, kFpQuarter_6FCE60, kFpQuarter_6FCE60, kFpQuarter_6FCE60, 0, 0, 1, 1, 1, 0, 0, 1) COMMA
                         CodeObjectTemplate(284, 2, 2, 0, 0, 1, 0, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, -1, 1, 1, 4, 9, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 11, 4, 1, 1, 0, 0, 0, 1) COMMA
                         CodeObjectTemplate(183, 4, 4, 182, 40, 1, 0, 0, 4, kFpPoint1_6F8FD8, -kFpPoint01_6FCDD8, 12, 3, 3, 4, 2, 1, kFpPoint1_6F8FD8, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 10, 0, 1, 1, 1, 0, 0, 1) COMMA
                         CodeObjectTemplate(195, 0, 3, 0, 0, 1, 7, 0, 2, kFpOne_6FCE0C, kFpZero_6FCE08, 1, 1, 1, 1, 0, 0, kFpZero_6FCE08, kFpQuarter_6FCE60, kFpQuarter_6FCE60, kFpQuarter_6FCE60, 0, 0, 1, 1, 1, 0, 0, 0) COMMA
                         CodeObjectTemplate(199, 0, 3, 0, 0, 1, 7, 0, 2, kFpThree_6FCE14, kFpZero_6FCE08, 1, 1, 1, 1, 0, 0, kFpZero_6FCE08, kFpQuarter_6FCE60, kFpQuarter_6FCE60, kFpQuarter_6FCE60, 0, 0, 1, 1, 1, 0, 0, 0) COMMA
                         CodeObjectTemplate(285, 0, 0, 0, 0, 0, 0, 0, 5, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 1, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 0, 0, 0, 1, 0, 0, 0, 1) COMMA
                         CodeObjectTemplate(283, 0, 0, 285, 0, 0, 11, 13, 4, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 1, 0, 0, kFpZero_6FCE08, kFpHalf_6F8FEC, kFpHalf_6F8FEC, kFpHalf_6F8FEC, 0, 0, 0, 1, 0, 0, 0, 1) COMMA
                         CodeObjectTemplate(166, 0, 1, 0, 0, 0, 0, 12, 2, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 1, 0, 0, kFpZero_6FCE08, kFpOne64th_6FCEB0, kFpSqrt2_6FC5A4, kFpTwo_6FCE10, 0, 0, 0, 1, 0, 0, 0, 1) COMMA
                         CodeObjectTemplate(280, 0, 0, 0, 0, 0, 0, 0, 5, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 1, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 0, 0, 0, 1, 0, 0, 0, 1) COMMA
                         CodeObjectTemplate(248, 1, 0, 0, 0, 0, 0, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 0, 0, kFpOne_6FCE0C, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 28, 0, 1, 1, 0, 0, 0, 1) COMMA
                         CodeObjectTemplate(286, 8, 2, 0, 0, 1, 0, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, -1, 1, 1, 4, 3, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 5, 0, 1, 1, 0, 0, 1, 1) COMMA
                         CodeObjectTemplate(165, 0, 11, 0, 0, 0, 0, 0, 5, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 1, 0, 0, kFpOne_6FCE0C, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 0, 0, 1, 1, 0, 0, 0, 1) COMMA
                         CodeObjectTemplate(171, 0, 11, 0, 0, 0, 0, 0, 5, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 1, 0, 0, kFpOne_6FCE0C, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 0, 0, 1, 1, 0, 0, 0, 1) COMMA
                         CodeObjectTemplate(172, 0, 11, 0, 0, 0, 0, 0, 5, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 1, 0, 0, kFpOne_6FCE0C, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 0, 0, 1, 1, 0, 0, 0, 1) COMMA
                         CodeObjectTemplate(173, 0, 11, 0, 0, 0, 0, 0, 5, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 1, 0, 0, kFpOne_6FCE0C, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 0, 0, 1, 1, 0, 0, 0, 1) COMMA
                         CodeObjectTemplate(287, 1, 0, 0, 0, 0, 0, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 0, 0, kFpOne_6FCE0C, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 28, 0, 1, 1, 0, 0, 0, 1) COMMA
                         CodeObjectTemplate(288, 1, 0, 0, 0, 0, 0, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 0, 0, kFpOne_6FCE0C, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 28, 0, 1, 1, 0, 0, 0, 1) COMMA
                         CodeObjectTemplate(289, 1, 0, 0, 0, 0, 0, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 0, 0, kFpOne_6FCE0C, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 28, 0, 1, 1, 0, 0, 0, 1) COMMA
                         CodeObjectTemplate(290, 1, 0, 0, 0, 0, 0, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 0, 0, kFpOne_6FCE0C, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 28, 0, 1, 1, 0, 0, 0, 1) COMMA
                         CodeObjectTemplate(291, 1, 0, 0, 0, 0, 0, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 0, 0, kFpOne_6FCE0C, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 28, 0, 1, 1, 0, 0, 0, 1) COMMA
                         CodeObjectTemplate(292, 1, 0, 0, 0, 0, 0, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 0, 0, kFpOne_6FCE0C, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 28, 0, 1, 1, 0, 0, 0, 1) COMMA
                         CodeObjectTemplate(293, 1, 0, 0, 0, 0, 0, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 0, 0, kFpOne_6FCE0C, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 28, 0, 1, 1, 0, 0, 0, 1) COMMA
                         CodeObjectTemplate(161, 0, 1, 0, 0, 0, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 1, 0, 0, kFpZero_6FCE08, kFpOne_6FCE0C, kFpOne_6FCE0C, kFpZero_6FCE08, 0, 0, 0, 1, 0, 0, 0, 0) COMMA
                         CodeObjectTemplate(138, 4, 4, 39, 39, 1, 7, 0, 1, kFpZero_6FCE08, kFpZero_6FCE08, -1, 4, 4, 4, 3, 1, kFpPoint1_6F8FD8, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 10, 0, 1, 1, 1, 0, 0, 1) COMMA
                         CodeObjectTemplate(112, 205, 0, 0, 0, 0, 0, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 0, 0, 0, 1, 0, 0, 0, 1) COMMA
                         CodeObjectTemplate(163, 1, 0, 164, 164, 0, 1, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 1, 0, 0, 1, 0, 1, 0, 1) COMMA
                         CodeObjectTemplate(164, 2, 2, 185, 185, 1, 6, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, -1, 1, 1, 4, 10, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 1, 0, 0, 1, 0, 1, 0, 1) COMMA
                         CodeObjectTemplate(281, 1, 0, 282, 0, 1, 11, 13, 4, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 0, 0, kFpZero_6FCE08, kFpOne64th_6FCEB0 * Fix16(48), kFpOne64th_6FCEB0 * Fix16(52), kFpZero_6FCE08, 10, 0, 0, 1, 0, 1, 0, 1) COMMA
                         CodeObjectTemplate(282, 1, 0, 0, 0, 0, 0, 12, 4, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 0, 0, kFpZero_6FCE08, kFpOne64th_6FCEB0 * Fix16(48), kFpOne64th_6FCEB0 * Fix16(52), kFpZero_6FCE08, 10, 0, 0, 1, 0, 0, 0, 1) COMMA
                         CodeObjectTemplate(256, 1, 0, 0, 0, 0, 0, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 2, 0, 0, 1, 0, 0, 0, 1) COMMA
                         CodeObjectTemplate(253, 4, 2, 0, 0, 1, 0, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 45, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 2, 0, 0, 2, 0, 0, 0, 1) COMMA
                         CodeObjectTemplate(144, 4, 2, 0, 0, 1, 0, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 45, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 2, 0, 0, 2, 0, 0, 0, 1) COMMA
                         CodeObjectTemplate(121, 4, 2, 0, 0, 1, 0, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 45, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 2, 0, 0, 2, 0, 0, 0, 1) COMMA
                         CodeObjectTemplate(118, 4, 2, 0, 0, 1, 0, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 45, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 2, 0, 0, 2, 0, 0, 0, 1) COMMA
                         CodeObjectTemplate(295, 8, 2, 0, 0, 1, 0, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, -1, 1, 1, 4, 3, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 5, 0, 1, 1, 0, 0, 1, 1) COMMA
                         CodeObjectTemplate(128, 1, 3, 0, 0, 1, 7, 0, 1, kFpPoint4_6F8FE8, kFpZero_6FCE08, 50, 1, 1, 4, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 28, 4, 1, 1, 1, 1, 0, 1) COMMA
                         CodeObjectTemplate(192, 0, 3, 259, 259, 1, 8, 0, 2, kFpPoint4_6F8FE8, kFpZero_6FCE08, 8, 1, 1, 1, 0, 0, kFpZero_6FCE08, kFpQuarter_6FCE60, kFpQuarter_6FCE60, kFpQuarter_6FCE60, 0, 0, 1, 1, 1, 0, 0, 1) COMMA
                         CodeObjectTemplate(259, 5, 2, 0, 0, 1, 0, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 1, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 2, 0, 1, 2, 0, 0, 0, 1) COMMA
                         CodeObjectTemplate(148, 2, 0, 0, 0, 0, 0, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 0, 0, kFpOne_6FCE0C, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 28, 0, 1, 1, 0, 0, 0, 1) COMMA
                         CodeObjectTemplate(170, 1, 0, 0, 0, 0, 0, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 29, 0, 0, 1, 0, 0, 0, 1) COMMA
                         CodeObjectTemplate(255, 3, 0, 0, 0, 0, 0, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 4, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 32, 4, 0, 1, 0, 0, 0, 1) COMMA
                         CodeObjectTemplate(294, 0, 0, 0, 0, 0, 0, 12, 4, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 1, 0, 0, kFpZero_6FCE08, kFpOne_6FCE0C, kFpOne_6FCE0C, kFpOne_6FCE0C, 0, 0, 0, 1, 0, 0, 0, 1) COMMA
                         CodeObjectTemplate(113, 0, 5, 0, 0, 1, 0, 0, 2, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 1, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 0, 0, 0, 1, 0, 1, 0, 1) COMMA
                         CodeObjectTemplate(278, 0, 12, 0, 0, 0, 0, 0, 5, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 1, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 0, 0, 1, 1, 0, 1, 0, 1) COMMA
                         CodeObjectTemplate(279, 0, 12, 0, 0, 1, 0, 0, 5, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 1, 0, 0, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, kFpZero_6FCE08, 0, 0, 1, 1, 0, 1, 0, 1) COMMA
                         CodeObjectTemplate(198, 0, 3, 198, 198, 0, 8, 0, 2, kFpOne_6FCE0C, kFpZero_6FCE08, 1, 1, 1, 1, 0, 0, kFpZero_6FCE08, kFpQuarter_6FCE60, kFpQuarter_6FCE60, kFpQuarter_6FCE60, 0, 0, 1, 1, 1, 0, 0, 1) COMMA
                         CodeObjectTemplate(251, 0, 3, 0, 0, 1, 0, 0, 5, kFpZero_6FCE08, kFpZero_6FCE08, 1, 1, 1, 1, 0, 0, kFpZero_6FCE08, kFpQuarter_6FCE60, kFpQuarter_6FCE60, kFpQuarter_6FCE60, 0, 0, 1, 1, 1, 0, 0, 1));

DEFINE_GLOBAL_INIT(s32, gMapObjectOverrides_length_623EEC, 24, 0x623EEC);

// Must remain a fixed array, a pointer would break ObjectDefinitions::ApplyMapObjectOverrides_533360
DEFINE_GLOBAL_ARRAY_INIT(MapObjectOverride,
                         gMapObjectOverrides_6FC5F8,
                         24,
                         0x6FC5F8,
                         MapObjectOverride(22, 0, 45, 0, 2, 0, 4, kFpZero_6FCE08, kFpZero_6FCE08, 0, 0, 1, 0, 1, kFpOne_6FCE0C, 10, 0, 0, 0, 1, 1) COMMA
                         MapObjectOverride(7, 0, 157, 0, 2, 0, 4, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 0, 1, kFpThree_6FCE14, 10, 0, 0, 0, 1, 1) COMMA
                         MapObjectOverride(1, 0, 158, 0, 2, 0, 4, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 0, 1, kFpPoint2_6F8FE0, 11, 0, 0, 0, 1, 1) COMMA
                         MapObjectOverride(5, 0, 156, 0, 2, 0, 4, kFpZero_6FCE08, kFpZero_6FCE08, 0, 0, 1, 0, 1, kFpPoint1_6F8FD8, 10, 0, 1, 0, 1, 1) COMMA
                         MapObjectOverride(2, 0, 0, 0, 0, 0, 4, kFpZero_6FCE08, -kFpPoint01_6FCDD8, 0, 1, 1, 0, 0, kFpZero_6FCE08, 10, 0, 0, 0, 1, 1) COMMA
                         MapObjectOverride(4, 0, 52, 0, 1, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 0, 0, kFpZero_6FCE08, 2, 0, 0, 0, 1, 1) COMMA
                         MapObjectOverride(11, 0, 123, 0, 2, 0, 4, kFpZero_6FCE08, kFpZero_6FCE08, 0, 0, 1, 0, 1, kFpPoint1_6F8FD8, 10, 0, 0, 0, 1, 1) COMMA
                         MapObjectOverride(3, 0, 155, 0, 2, 0, 4, kFpZero_6FCE08, kFpZero_6FCE08, 0, 0, 1, 0, 1, kFpPoint1_6F8FD8, 10, 0, 0, 0, 1, 1) COMMA
                         MapObjectOverride(12, 0, 50, 0, 1, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 0, 0, kFpZero_6FCE08, 2, 0, 0, 0, 1, 1) COMMA
                         MapObjectOverride(13, 0, 56, 0, 2, 0, 4, kFpZero_6FCE08, kFpZero_6FCE08, 0, 0, 1, 0, 1, kFpPoint1_6F8FD8, 10, 0, 0, 0, 1, 1) COMMA
                         MapObjectOverride(14, 0, 57, 0, 2, 0, 4, kFpZero_6FCE08, kFpZero_6FCE08, 0, 0, 1, 0, 1, kFpPoint1_6F8FD8, 10, 0, 0, 0, 1, 1) COMMA
                         MapObjectOverride(18, 0, 49, 0, 2, 0, 4, kFpZero_6FCE08, kFpZero_6FCE08, 0, 0, 1, 0, 1, kFpPoint1_6F8FD8, 10, 0, 0, 0, 1, 1) COMMA
                         MapObjectOverride(19, 0, 0, 0, 0, 0, 4, kFpZero_6FCE08, -kFpPoint01_6FCDD8, 0, 1, 1, 0, 0, kFpZero_6FCE08, 10, 0, 0, 0, 1, 1) COMMA
                         MapObjectOverride(20, 0, 0, 0, 0, 0, 4, kFpZero_6FCE08, -kFpPoint01_6FCDD8, 0, 1, 1, 0, 0, kFpZero_6FCE08, 10, 0, 0, 0, 1, 1) COMMA
                         MapObjectOverride(10, 0, 0, 0, 7, 0, 3, kFpZero_6FCE08, -kFpPoint01_6FCDD8, -1, 1, 1, 4, 0, kFpZero_6FCE08, 7, 0, 0, 0, 3, 0) COMMA
                         MapObjectOverride(15, 0, 59, 0, 2, 0, 4, kFpZero_6FCE08, kFpZero_6FCE08, 0, 0, 1, 0, 1, kFpPoint1_6F8FD8, 10, 0, 0, 0, 1, 1) COMMA
                         MapObjectOverride(8, 2, 9, 1, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, 1, 1, 1, 1, 0, kFpZero_6FCE08, 3, 0, 0, 0, 1, 1) COMMA
                         MapObjectOverride(9, 0, 0, 0, 9, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 0, 0, kFpZero_6FCE08, 3, 0, 0, 0, 3, 0) COMMA
                         MapObjectOverride(16, 0, 61, 0, 2, 0, 4, kFpZero_6FCE08, kFpZero_6FCE08, 0, 0, 1, 0, 1, kFpPoint1_6F8FD8, 10, 0, 0, 0, 1, 1) COMMA
                         MapObjectOverride(23, 0, 43, 0, 2, 0, 4, kFpZero_6FCE08, kFpZero_6FCE08, 0, 0, 1, 0, 1, kFpPoint1_6F8FD8, 10, 0, 0, 0, 1, 1) COMMA
                         MapObjectOverride(21, 0, 47, 0, 2, 0, 4, kFpZero_6FCE08, kFpZero_6FCE08, 0, 0, 1, 0, 1, kFpFive_6FCE1C, 10, 0, 0, 0, 1, 1) COMMA
                         MapObjectOverride(6, 0, 151, 0, 1, 0, 3, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 0, 0, kFpZero_6FCE08, 2, 0, 0, 0, 1, 1) COMMA
                         MapObjectOverride(25, 0, 0, 0, 0, 12, 4, kFpZero_6FCE08, kFpZero_6FCE08, 0, 1, 1, 0, 0, kFpZero_6FCE08, 10, 0, 0, 0, 1, 1) COMMA
                         MapObjectOverride(17, 0, 63, 0, 2, 0, 4, kFpZero_6FCE08, kFpZero_6FCE08, 0, 0, 1, 0, 1, kFpPoint1_6F8FD8, 10, 0, 0, 0, 1, 1));

MATCH_FUNC(0x4bdf40)
ObjectDefinitions::~ObjectDefinitions()
{
}

MATCH_FUNC(0x4bdf60)
ObjectDefinition::~ObjectDefinition()
{
}

MATCH_FUNC(0x531860)
inline CodeObjectTemplate::CodeObjectTemplate(u32 definition_idx,
               u8 num_sprites,
               const u32& behavior_type,
               u32 param_4,
               u32 next_definition_idx,
               u8 in_pool_list,
               const u32& touch_behavior,
               const u32& projectile_hit_type,
               const u32& collision_bucket_category,
               Fix16 speed,
               Fix16 friction,
               s8 next_definition_timer,
               const u32& param_13,
               const u32& z_collision_response,
               u32 sprite_type,
               u32 next_frame_max,
               const u32& uses_3d_movement,
               Fix16 mass,
               Fix16 width,
               Fix16 height,
               Fix16 depth,
               u32 draw_layer,
               u32 sprite_flags,
               u8 destroyed_by_destructor,
               const u32& instance_limit_kind,
               u32 uses_bullet_walls,
               u32 has_sound,
               u8 has_shadows,
               u8 makes_collision_sound)
{
    field_0_definition_idx = definition_idx;
    field_4_num_sprites = num_sprites;
    field_8_behavior_type = behavior_type;
    field_C = param_4;
    field_10_next_definition_idx = next_definition_idx;
    field_14_in_pool_list = in_pool_list;
    field_18_touch_behavior = touch_behavior;
    field_1C_projectile_hit_type = projectile_hit_type;
    field_20_collision_bucket_category = collision_bucket_category;
    field_24_speed = speed;
    field_28_friction = friction;
    field_2C_next_definition_timer = next_definition_timer;
    field_30 = param_13;
    field_34_z_collision_response = z_collision_response;
    field_38_sprite_type = sprite_type;
    field_3C_next_frame_max = next_frame_max;
    field_40_uses_3d_movement = uses_3d_movement;
    field_44_mass = mass;
    field_48_width = width;
    field_4C_height = height;
    field_50_depth = depth;
    field_54_draw_layer = draw_layer;
    field_58_sprite_flags = sprite_flags;
    field_59_destroyed_by_destructor = destroyed_by_destructor;
    field_5C_instance_limit_kind = instance_limit_kind;
    field_60_uses_bullet_walls = uses_bullet_walls;
    field_64_has_sound = has_sound;
    field_68_has_shadows = has_shadows;
    field_69_makes_collision_sound = makes_collision_sound;
}

MATCH_FUNC(0x532fb0)
inline MapObjectOverride::MapObjectOverride(s32 definition_idx,
               const s32& behavior_type,
               s32 next_definition_idx,
               s8 in_pool_list,
               const s32& touch_behavior,
               const s32& projectile_hit_type,
               const s32& collision_bucket_category,
               Fix16 speed,
               Fix16 friction,
               s8 next_definition_timer,
               const s32& param_11,
               const s32& param_12,
               s8 next_frame_max,
               const s32& uses_3d_movement,
               Fix16 mass,
               s32 draw_layer,
               s32 sprite_flags,
               s32 has_sound,
               s8 has_shadows,
               s32 instance_limit_kind,
               s8 makes_collision_sound)
{
    field_0_definition_idx = definition_idx;
    field_4_behavior_type = behavior_type;
    field_8_next_definition_idx = next_definition_idx;
    field_C_in_pool_list = in_pool_list;
    field_10_touch_behavior = touch_behavior;
    field_14_projectile_hit_type = projectile_hit_type;
    field_18_collision_bucket_category = collision_bucket_category;
    field_1C_speed = speed;
    field_20_friction = friction;
    field_24_next_definition_timer = next_definition_timer;
    field_28 = param_11;
    field_2C = param_12;
    field_30_next_frame_max = next_frame_max;
    field_34_uses_3d_movement = uses_3d_movement;
    field_38_mass = mass;
    field_3C_draw_layer = draw_layer;
    field_40_sprite_flags = sprite_flags;
    field_44_has_sound = has_sound;
    field_48_has_shadows = has_shadows;
    field_4C_instance_limit_kind = instance_limit_kind;
    field_50_makes_collision_sound = makes_collision_sound;
}

MATCH_FUNC(0x533060)
void ObjectDefinition::SetDimensions_533060(Fix16 width, Fix16 height, Fix16 depth)
{
    field_0_width = width;
    field_4_height = height;
    field_8_depth = depth;
    if (field_0_width < field_4_height)
    {
        field_C_min_size = width;
    }
    else
    {
        field_C_min_size = height;
    }
}

MATCH_FUNC(0x533090)
void ObjectDefinition::SetDimensionsFromSprite_533090()
{
    if (field_28_sprite_type != sprite_types_enum::object_1)
    {
        u16 sprite_id = gGtx_0x106C_703DD4->GetSpriteTrueIndex_5AA460(field_28_sprite_type, field_1E_base_sprite_idx);
        sprite_index * sprite = gGtx_0x106C_703DD4->get_sprite_index_5AA440(sprite_id);
        field_0_width = Fix16(sprite->field_4_width) / 0x40;
        field_4_height = Fix16(sprite->field_5_height) / 0x40;
        field_8_depth = kFpQuarter_6FCED0;
    }
    else
    {
        field_0_width = kFpPoint6_6F8FF0;
        field_4_height = kFpPoint6_6F8FF0;
        field_8_depth = kFpQuarter_6FCED0;
    }

    if (field_0_width < field_4_height)
    {
        field_C_min_size = field_0_width;
    }
    else
    {
        field_C_min_size = field_4_height;
    }
}

MATCH_FUNC(0x533110)
void ObjectDefinition::SetRemap_533110(s16 remap)
{
    switch (field_28_sprite_type)
    {
        case sprite_types_enum::code_obj1_4:
            field_30_palette_type = 5;
            break;
        case sprite_types_enum::map_obj_5:
            field_30_palette_type = 6;
            break;
    }
    field_1C_remap = remap;
}

MATCH_FUNC(0x533150)
void ObjectDefinition::OffsetBaseSpriteIdxAndSetNumFrames_533150(s16 sprite_idx_offset, s16 anim_speed)
{
    field_1E_base_sprite_idx += sprite_idx_offset;
    field_6C_num_frames = static_cast<u8>(anim_speed);
}

MATCH_FUNC(0x533170)
Sprite* ObjectDefinition::CreateSpriteFromDefinition_533170()
{
    Sprite* pFreeSprite = gSpritePool_703818->get_new_sprite();
    ApplyDefinitionToSprite_5331A0(pFreeSprite);
    return pFreeSprite;
}

// fix16?
s32 kFp255_6F8FA4 = 0x3FC000;

MATCH_FUNC(0x5331a0)
void ObjectDefinition::ApplyDefinitionToSprite_5331A0(Sprite* pSprite)
{
    s32 bucket_category;

    pSprite->SetType_4206F0(this->field_28_sprite_type);
    pSprite->set_id_lazy_4206C0(this->field_1E_base_sprite_idx);

    pSprite->SetDrawLayer_40F7B0(this->field_2C_draw_layer);
    pSprite->field_2C_flags = this->field_20_sprite_flags;

    if (this->field_30_palette_type != 2)
    {
        pSprite->SetRemap(this->field_1C_remap);
    }

    bucket_category = this->field_40_collision_bucket_category;
    if (bucket_category > 0 && (bucket_category <= 3 || bucket_category == 4))
    {
        pSprite->AllocCollisionShape_59F950(this->field_0_width, this->field_4_height, this->field_8_depth);
    }

    if (this->field_70_has_sound)
    {
        pSprite->CreateSoundObj_5A29D0();
    }
}

MATCH_FUNC(0x533220)
ObjectDefinition::ObjectDefinition()
{
    field_0_width = kFp255_6F8FA4;
    field_4_height = kFp255_6F8FA4;
    field_8_depth = kFp255_6F8FA4;
    field_24_object_idx = 0;
    field_28_sprite_type = sprite_types_enum::none_0;
    field_2C_draw_layer = 0;
    field_30_palette_type = 0;
    field_34_behavior_type = object_behavior_type::static_object_0;
    field_38 = 0;
    field_3C_next_definition_idx = 0;
    field_40_collision_bucket_category = collision_bucket_category::sprite_grid_3_single_cell_0;
    field_44_touch_behavior = 0;
    field_48_projectile_hit_type = 0;
    field_4C = 1;
    field_50_z_collision_response = 1;
    field_58_uses_3d_movement = 0;
    field_54_react_to_collisions_with = CollisionReaction::Always_0;
    field_C_min_size = kFp255_6F8FA4;
    field_10_speed = kFp255_6F8FA4;
    field_14_friction = kFp255_6F8FA4;
    field_1C_remap = 9999;
    field_61_in_pool_list = 0;
    field_64_next_frame_max = 99;
    field_65_next_definition_timer = 99;
    field_1E_base_sprite_idx = 99;
    field_6C_num_frames = 99;
    Fix16 zero_mass = kFpZeroForStaticInit_6FCE08;
    field_68_uses_bullet_walls = 0;
    field_18_mass = zero_mass;
    field_20_sprite_flags = 0;
    field_60_destroyed_by_destructor = 0;
    field_5C_instance_limit_kind = 1;
    field_70_has_sound = 0;
}

MATCH_FUNC(0x5332d0)
ObjectDefinition* ObjectDefinitions::AllocDefinitionWithSprite_5332D0(s32 idx, s32 sprite_type, s16 sprite_palette, u8 anim_speed)
{
    ObjectDefinition* result = AllocDefinition_5343C0(idx);
    result->field_28_sprite_type = sprite_type;
    result->field_1E_base_sprite_idx = sprite_palette;
    result->field_6C_num_frames = anim_speed;
    result->field_30_palette_type = 2;
    return result;
}

MATCH_FUNC(0x533300)
void ObjectDefinitions::CreateMapObjectDefinitions_533300()
{
    s16 sprites = 0;
    u16 i = 0;
    object_info* pIter = gGtx_0x106C_703DD4->get_map_object_info_5AA910(i++);
    while (pIter)
    {
        ObjectDefinition* pDefinition = AllocDefinitionWithSprite_5332D0(pIter->field_0_model, sprite_types_enum::map_obj_5, sprites, pIter->field_1_sprites);
        sprites += pIter->field_1_sprites;
        pDefinition->SetDimensionsFromSprite_533090();
        pIter = gGtx_0x106C_703DD4->get_map_object_info_5AA910(i++);
    }
}

MATCH_FUNC(0x533360)
void ObjectDefinitions::ApplyMapObjectOverrides_533360()
{
    int override_count = gMapObjectOverrides_length_623EEC;

    if (0 < override_count)
    {
        MapObjectOverride* pOverride = gMapObjectOverrides_6FC5F8;
        do
        {
            ObjectDefinition* pDef = GetObjectDefinition_534360(pOverride->field_0_definition_idx);
            pDef->field_34_behavior_type = pOverride->field_4_behavior_type;
            pDef->field_38 = pOverride->field_8_next_definition_idx;
            pDef->field_3C_next_definition_idx = pOverride->field_8_next_definition_idx;
            pDef->field_61_in_pool_list = pOverride->field_C_in_pool_list;
            pDef->field_40_collision_bucket_category = pOverride->field_18_collision_bucket_category;
            pDef->field_44_touch_behavior = pOverride->field_10_touch_behavior;
            pDef->field_48_projectile_hit_type = pOverride->field_14_projectile_hit_type;
            pDef->field_10_speed = pOverride->field_1C_speed;
            pDef->field_65_next_definition_timer = pOverride->field_24_next_definition_timer;
            pDef->field_14_friction = pOverride->field_20_friction;
            pDef->field_4C = pOverride->field_28;
            pDef->field_64_next_frame_max = pOverride->field_30_next_frame_max;
            pDef->field_58_uses_3d_movement = pOverride->field_34_uses_3d_movement;
            pDef->field_18_mass = pOverride->field_38_mass;
            pDef->field_20_sprite_flags = pOverride->field_40_sprite_flags;
            pDef->field_2C_draw_layer = pOverride->field_3C_draw_layer;
            pDef->field_60_destroyed_by_destructor = 1;
            pDef->field_5C_instance_limit_kind = pOverride->field_4C_instance_limit_kind;
            pDef->field_68_uses_bullet_walls = 0;
            pDef->field_70_has_sound = pOverride->field_44_has_sound;
            pDef->field_62_has_shadows = pOverride->field_48_has_shadows;
            pDef->field_63_makes_collision_sound = pOverride->field_50_makes_collision_sound;
            pDef->field_54_react_to_collisions_with = CollisionReaction::Always_0;
            pOverride++;
        } while (--override_count != 0);
    }
}

MATCH_FUNC(0x533420)
void ObjectDefinitions::CloneAnimatedDefinitions_533420()
{
    ObjectDefinition* pDef;

    CloneDefinition_534370(0x97, 6);
    pDef = GetObjectDefinition_534360(0x97);
    pDef->OffsetBaseSpriteIdxAndSetNumFrames_533150(1, pDef->field_6C_num_frames + -1);
    pDef->field_34_behavior_type = object_behavior_type::animated_2;
    pDef->field_64_next_frame_max = '\x05';
    pDef->field_38 = 0x98;
    pDef->field_3C_next_definition_idx = 0x98;
    pDef->field_40_collision_bucket_category = collision_bucket_category::sprite_grid_3_single_cell_0;
    pDef->field_44_touch_behavior = 0;
    pDef->field_65_next_definition_timer = '\x01';
    pDef->field_61_in_pool_list = '\x01';

    pDef = CloneDefinition_534370(0x98, 6);
    pDef->OffsetBaseSpriteIdxAndSetNumFrames_533150(pDef->field_6C_num_frames - 1, 1);
    pDef->field_34_behavior_type = object_behavior_type::static_object_0;
    pDef->field_38 = 0;
    pDef->field_3C_next_definition_idx = 0;
    pDef->field_40_collision_bucket_category = collision_bucket_category::sprite_grid_3_single_cell_0;
    pDef->field_44_touch_behavior = 0;
    pDef->field_61_in_pool_list = '\0';

    pDef = CloneDefinition_534370(0x34, 4);
    pDef->OffsetBaseSpriteIdxAndSetNumFrames_533150(pDef->field_6C_num_frames - 1, 1);
    pDef->field_34_behavior_type = object_behavior_type::static_object_0;
    pDef->field_38 = 0;
    pDef->field_3C_next_definition_idx = 0;
    pDef->field_40_collision_bucket_category = collision_bucket_category::sprite_grid_3_single_cell_0;
    pDef->field_44_touch_behavior = 0;
    pDef->field_61_in_pool_list = '\0';

    CloneDefinition_534370(0x32, 0xc);
    pDef = GetObjectDefinition_534360(0x32);
    pDef->OffsetBaseSpriteIdxAndSetNumFrames_533150(1, pDef->field_6C_num_frames + -1);
    pDef->field_34_behavior_type = object_behavior_type::animated_2;
    pDef->field_64_next_frame_max = '\x03';
    pDef->field_38 = 0x33;
    pDef->field_3C_next_definition_idx = 0x33;
    pDef->field_40_collision_bucket_category = collision_bucket_category::sprite_grid_3_single_cell_0;
    pDef->field_44_touch_behavior = 0;
    pDef->field_65_next_definition_timer = '\x01';
    pDef->field_61_in_pool_list = '\x01';

    pDef = CloneDefinition_534370(0x33, 0xc);
    pDef->OffsetBaseSpriteIdxAndSetNumFrames_533150(pDef->field_6C_num_frames - 1, 1);
    pDef->field_34_behavior_type = object_behavior_type::static_object_0;
    pDef->field_38 = 0;
    pDef->field_3C_next_definition_idx = 0;
    pDef->field_40_collision_bucket_category = collision_bucket_category::sprite_grid_3_single_cell_0;
    pDef->field_44_touch_behavior = 0;
    pDef->field_61_in_pool_list = '\0';

    pDef = CloneDefinition_534370(0x9b, 3);
    pDef->OffsetBaseSpriteIdxAndSetNumFrames_533150(1, pDef->field_6C_num_frames + -1);
    pDef->field_34_behavior_type = object_behavior_type::physics_animated_4;
    pDef->field_64_next_frame_max = 1;
    pDef->field_38 = 0x35;
    pDef->field_3C_next_definition_idx = 0x35;
    pDef->field_40_collision_bucket_category = collision_bucket_category::sprite_grid_1_region_4;
    pDef->field_44_touch_behavior = 0;
    pDef->field_10_speed = kFpPoint1_6F8FD8;
    pDef->field_14_friction = -kFpPoint01_6FCDD8;
    pDef->field_61_in_pool_list = 1;
    pDef->field_4C = 3;
    pDef->field_50_z_collision_response = 3;
    pDef->field_65_next_definition_timer = -1;
    pDef->field_58_uses_3d_movement = 1;

    pDef = CloneDefinition_534370(0x35, 3);
    pDef->OffsetBaseSpriteIdxAndSetNumFrames_533150(pDef->field_6C_num_frames - 1, 1);

    pDef = CloneDefinition_534370(0x7b, 0xb);
    pDef->OffsetBaseSpriteIdxAndSetNumFrames_533150(1, pDef->field_6C_num_frames + -1);
    pDef->field_34_behavior_type = object_behavior_type::physics_animated_4;
    pDef->field_64_next_frame_max = '\x01';
    pDef->field_38 = 0x37;
    pDef->field_3C_next_definition_idx = 0x37;
    pDef->field_40_collision_bucket_category = collision_bucket_category::sprite_grid_1_region_4;
    pDef->field_44_touch_behavior = 0;
    pDef->field_10_speed = kFpPoint1_6F8FD8;
    pDef->field_14_friction = -kFpPoint01_6FCDD8;
    pDef->field_61_in_pool_list = '\x01';
    pDef->field_4C = 3;
    pDef->field_50_z_collision_response = 3;
    pDef->field_65_next_definition_timer = -1;
    pDef->field_58_uses_3d_movement = 1;

    pDef = CloneDefinition_534370(0x37, 0xb);
    pDef->OffsetBaseSpriteIdxAndSetNumFrames_533150(pDef->field_6C_num_frames - 1, 1);

    pDef = CloneDefinition_534370(0x9c, 5);
    pDef->OffsetBaseSpriteIdxAndSetNumFrames_533150(1, pDef->field_6C_num_frames + -1);
    pDef->field_34_behavior_type = object_behavior_type::physics_animated_4;
    pDef->field_64_next_frame_max = '\x01';
    pDef->field_38 = 0x36;
    pDef->field_3C_next_definition_idx = 0x36;
    pDef->field_40_collision_bucket_category = collision_bucket_category::sprite_grid_1_region_4;
    pDef->field_44_touch_behavior = 0;
    pDef->field_10_speed = kFpPoint1_6F8FD8;
    pDef->field_14_friction = -kFpPoint01_6FCDD8;
    pDef->field_61_in_pool_list = '\x01';
    pDef->field_4C = 3;
    pDef->field_50_z_collision_response = 3;
    pDef->field_65_next_definition_timer = -1;
    pDef->field_58_uses_3d_movement = 1;

    pDef = CloneDefinition_534370(0x36, 5);
    pDef->OffsetBaseSpriteIdxAndSetNumFrames_533150(pDef->field_6C_num_frames - 1, 1);

    pDef = CloneDefinition_534370(0x38, 0xd);
    pDef->OffsetBaseSpriteIdxAndSetNumFrames_533150(1, pDef->field_6C_num_frames + -1);
    pDef->field_34_behavior_type = object_behavior_type::physics_animated_4;
    pDef->field_64_next_frame_max = '\x01';
    pDef->field_38 = 0xd;
    pDef->field_3C_next_definition_idx = 0xd;
    pDef->field_40_collision_bucket_category = collision_bucket_category::sprite_grid_1_region_4;
    pDef->field_44_touch_behavior = 0;
    pDef->field_10_speed = kFpPoint1_6F8FD8;
    pDef->field_14_friction = -kFpPoint01_6FCDD8;
    pDef->field_61_in_pool_list = '\x01';
    pDef->field_4C = 3;
    pDef->field_50_z_collision_response = 3;
    pDef->field_65_next_definition_timer = -1;
    pDef->field_58_uses_3d_movement = 1;

    pDef = CloneDefinition_534370(0x39, 0xe);
    pDef->OffsetBaseSpriteIdxAndSetNumFrames_533150(1, pDef->field_6C_num_frames + -1);
    pDef->field_34_behavior_type = object_behavior_type::physics_animated_4;
    pDef->field_64_next_frame_max = '\x01';
    pDef->field_38 = 0x3a;
    pDef->field_3C_next_definition_idx = 0x3a;
    pDef->field_40_collision_bucket_category = collision_bucket_category::sprite_grid_1_region_4;
    pDef->field_44_touch_behavior = 0;
    pDef->field_10_speed = kFpPoint1_6F8FD8;
    pDef->field_14_friction = -kFpPoint01_6FCDD8;
    pDef->field_61_in_pool_list = '\x01';
    pDef->field_4C = 3;
    pDef->field_50_z_collision_response = 3;
    pDef->field_65_next_definition_timer = -1;
    pDef->field_58_uses_3d_movement = 1;

    pDef = CloneDefinition_534370(0x3a, 0xe);
    pDef->OffsetBaseSpriteIdxAndSetNumFrames_533150(pDef->field_6C_num_frames - 1, 1);

    pDef = CloneDefinition_534370(0x3b, 0xf);
    pDef->OffsetBaseSpriteIdxAndSetNumFrames_533150(1, pDef->field_6C_num_frames + -1);
    pDef->field_34_behavior_type = object_behavior_type::physics_animated_4;
    pDef->field_64_next_frame_max = '\x01';
    pDef->field_38 = 0x3c;
    pDef->field_3C_next_definition_idx = 0x3c;
    pDef->field_40_collision_bucket_category = collision_bucket_category::sprite_grid_1_region_4;
    pDef->field_44_touch_behavior = 0;
    pDef->field_10_speed = kFpPoint1_6F8FD8;
    pDef->field_14_friction = -kFpPoint01_6FCDD8;
    pDef->field_61_in_pool_list = '\x01';
    pDef->field_4C = 3;
    pDef->field_50_z_collision_response = 3;
    pDef->field_65_next_definition_timer = -1;
    pDef->field_58_uses_3d_movement = 1;

    pDef = CloneDefinition_534370(0x3c, 0xf);
    pDef->OffsetBaseSpriteIdxAndSetNumFrames_533150(pDef->field_6C_num_frames - 1, 1);

    pDef = CloneDefinition_534370(0x3d, 0x10);
    pDef->OffsetBaseSpriteIdxAndSetNumFrames_533150(1, pDef->field_6C_num_frames + -1);
    pDef->field_34_behavior_type = object_behavior_type::physics_animated_4;
    pDef->field_64_next_frame_max = '\x01';
    pDef->field_38 = 0x3e;
    pDef->field_3C_next_definition_idx = 0x3e;
    pDef->field_40_collision_bucket_category = collision_bucket_category::sprite_grid_1_region_4;
    pDef->field_44_touch_behavior = 0;
    pDef->field_10_speed = kFpPoint1_6F8FD8;
    pDef->field_14_friction = -kFpPoint01_6FCDD8;
    pDef->field_61_in_pool_list = '\x01';
    pDef->field_4C = 3;
    pDef->field_50_z_collision_response = 3;
    pDef->field_65_next_definition_timer = -1;
    pDef->field_58_uses_3d_movement = 1;

    pDef = CloneDefinition_534370(0x3e, 0x10);
    pDef->OffsetBaseSpriteIdxAndSetNumFrames_533150(pDef->field_6C_num_frames - 1, 1);

    pDef = CloneDefinition_534370(0x31, 0x12);
    pDef->OffsetBaseSpriteIdxAndSetNumFrames_533150(1, pDef->field_6C_num_frames + -1);
    pDef->field_34_behavior_type = object_behavior_type::physics_animated_4;
    pDef->field_64_next_frame_max = '\x01';
    pDef->field_38 = 0x12;
    pDef->field_3C_next_definition_idx = 0x12;
    pDef->field_40_collision_bucket_category = collision_bucket_category::sprite_grid_1_region_4;
    pDef->field_44_touch_behavior = 0;
    pDef->field_10_speed = kFpPoint1_6F8FD8;
    pDef->field_14_friction = -kFpPoint01_6FCDD8;
    pDef->field_61_in_pool_list = '\x01';
    pDef->field_4C = 3;
    pDef->field_50_z_collision_response = 3;
    pDef->field_65_next_definition_timer = -1;
    pDef->field_58_uses_3d_movement = 1;

    pDef = CloneDefinition_534370(0x2d, 0x16);
    pDef->OffsetBaseSpriteIdxAndSetNumFrames_533150(1, pDef->field_6C_num_frames + -1);
    pDef->field_34_behavior_type = object_behavior_type::physics_animated_4;
    pDef->field_64_next_frame_max = '\x01';
    pDef->field_38 = 0x2e;
    pDef->field_3C_next_definition_idx = 0x2e;
    pDef->field_40_collision_bucket_category = collision_bucket_category::sprite_grid_1_region_4;
    pDef->field_44_touch_behavior = 0;
    pDef->field_10_speed = kFpPoint1_6F8FD8;
    pDef->field_14_friction = -kFpPoint01_6FCDD8;
    pDef->field_61_in_pool_list = '\x01';
    pDef->field_4C = 3;
    pDef->field_50_z_collision_response = 3;
    pDef->field_65_next_definition_timer = -1;
    pDef->field_58_uses_3d_movement = 1;

    pDef = CloneDefinition_534370(0x2e, 0x16);
    pDef->OffsetBaseSpriteIdxAndSetNumFrames_533150(pDef->field_6C_num_frames - 1, 1);

    pDef = CloneDefinition_534370(0x2f, 0x15);
    pDef->OffsetBaseSpriteIdxAndSetNumFrames_533150(1, pDef->field_6C_num_frames + -1);
    pDef->field_34_behavior_type = object_behavior_type::physics_animated_4;
    pDef->field_64_next_frame_max = '\x01';
    pDef->field_38 = 0x30;
    pDef->field_3C_next_definition_idx = 0x30;
    pDef->field_40_collision_bucket_category = collision_bucket_category::sprite_grid_1_region_4;
    pDef->field_44_touch_behavior = 0;
    pDef->field_10_speed = kFpPoint1_6F8FD8;
    pDef->field_14_friction = -kFpPoint01_6FCDD8;
    pDef->field_61_in_pool_list = '\x01';
    pDef->field_4C = 3;
    pDef->field_50_z_collision_response = 3;
    pDef->field_65_next_definition_timer = -1;
    pDef->field_58_uses_3d_movement = 1;

    pDef = CloneDefinition_534370(0x30, 0x15);
    pDef->OffsetBaseSpriteIdxAndSetNumFrames_533150(pDef->field_6C_num_frames - 1, 1);

    pDef = CloneDefinition_534370(0x3f, 0x11);
    pDef->field_38 = 0x11;
    pDef->field_34_behavior_type = object_behavior_type::physics_3;
    pDef->field_3C_next_definition_idx = 0x11;
    pDef->field_40_collision_bucket_category = collision_bucket_category::sprite_grid_1_region_4;
    pDef->field_44_touch_behavior = 0;
    pDef->field_10_speed = kFpPoint1_6F8FD8;
    pDef->field_14_friction = -kFpPoint01_6FCDD8;
    pDef->field_4C = 3;
    pDef->field_50_z_collision_response = 3;
    pDef->field_61_in_pool_list = '\x01';
    pDef->field_58_uses_3d_movement = 1;

    pDef = CloneDefinition_534370(0x2b, 0x17);
    pDef->OffsetBaseSpriteIdxAndSetNumFrames_533150(1, pDef->field_6C_num_frames + -1);
    pDef->field_34_behavior_type = object_behavior_type::physics_animated_4;
    pDef->field_64_next_frame_max = '\x01';
    pDef->field_38 = 0x2c;
    pDef->field_3C_next_definition_idx = 0x2c;
    pDef->field_40_collision_bucket_category = collision_bucket_category::sprite_grid_1_region_4;
    pDef->field_44_touch_behavior = 0;
    pDef->field_10_speed = kFpPoint1_6F8FD8;
    pDef->field_14_friction = -kFpPoint01_6FCDD8;
    pDef->field_61_in_pool_list = '\x01';
    pDef->field_4C = 3;
    pDef->field_50_z_collision_response = 3;
    pDef->field_65_next_definition_timer = -1;
    pDef->field_58_uses_3d_movement = 1;

    pDef = CloneDefinition_534370(0x2c, 0x17);
    pDef->OffsetBaseSpriteIdxAndSetNumFrames_533150(pDef->field_6C_num_frames - 1, 1);

    pDef = CloneDefinition_534370(0x9d, 7);
    pDef->field_4C = 2;
    pDef->field_34_behavior_type = object_behavior_type::physics_3;
    pDef->field_14_friction = -kFpPoint01_6FCDD8;
    pDef->field_38 = 7;
    pDef->field_3C_next_definition_idx = 7;
    pDef->field_40_collision_bucket_category = collision_bucket_category::sprite_grid_1_region_4;
    pDef->field_44_touch_behavior = 0;
    pDef->field_61_in_pool_list = '\x01';
    pDef->field_58_uses_3d_movement = 1;

    pDef = CloneDefinition_534370(0x9e, 1);
    pDef->field_34_behavior_type = object_behavior_type::physics_3;
    pDef->field_4C = 2;
    pDef->field_14_friction = -kFpPoint01_6FCDD8;
    pDef->field_38 = 1;
    pDef->field_3C_next_definition_idx = 1;
    pDef->field_44_touch_behavior = 0;
    pDef->field_61_in_pool_list = '\x01';
    pDef->field_58_uses_3d_movement = 1;
    pDef = GetObjectDefinition_534360(0x19);
    pDef->field_28_sprite_type = sprite_types_enum::object_1;
    pDef->field_8_depth = kFpTwo_6FCE10;
    return;
}

MATCH_FUNC(0x533b30)
void ObjectDefinitions::CreateCodeObjectDefinitions_533B30()
{
    u16 sprite_base = 0;
    if (0 < gCodeObjectTemplates_length_623EF0)
    {
        CodeObjectTemplate* pTemplate = &gCodeObjectTemplates_6F9038[0];
        for(s32 remaining = gCodeObjectTemplates_length_623EF0; remaining != 0; remaining--)
        {
            ObjectDefinition* pDef = AllocDefinitionWithSprite_5332D0(pTemplate->field_0_definition_idx, sprite_types_enum::code_obj1_4, sprite_base, pTemplate->field_4_num_sprites);
            sprite_base += pTemplate->field_4_num_sprites;
            pDef->field_28_sprite_type = pTemplate->field_38_sprite_type;
            if (pTemplate->field_48_width == kFpZero_6FCE08 && pTemplate->field_4C_height == kFpZero_6FCE08 && pTemplate->field_50_depth == kFpZero_6FCE08)
            {
                pDef->SetDimensionsFromSprite_533090();
            }
            else
            {
                pDef->SetDimensions_533060(pTemplate->field_48_width, pTemplate->field_4C_height, pTemplate->field_50_depth);
            }

            pDef->field_34_behavior_type = pTemplate->field_8_behavior_type;
            pDef->field_38 = pTemplate->field_C;
            pDef->field_3C_next_definition_idx = pTemplate->field_10_next_definition_idx;
            pDef->field_61_in_pool_list = pTemplate->field_14_in_pool_list;
            pDef->field_40_collision_bucket_category = pTemplate->field_20_collision_bucket_category;
            pDef->field_44_touch_behavior = pTemplate->field_18_touch_behavior;
            pDef->field_48_projectile_hit_type = pTemplate->field_1C_projectile_hit_type;
            pDef->field_10_speed = pTemplate->field_24_speed;
            pDef->field_65_next_definition_timer = pTemplate->field_2C_next_definition_timer;
            pDef->field_14_friction = pTemplate->field_28_friction;
            pDef->field_4C = pTemplate->field_30;
            pDef->field_50_z_collision_response = pTemplate->field_34_z_collision_response;
            pDef->field_64_next_frame_max = pTemplate->field_3C_next_frame_max;
            pDef->field_54_react_to_collisions_with = CollisionReaction::Always_0;
            pDef->field_58_uses_3d_movement = pTemplate->field_40_uses_3d_movement;
            pDef->field_18_mass = pTemplate->field_44_mass;
            pDef->field_2C_draw_layer = pTemplate->field_54_draw_layer;
            pDef->field_20_sprite_flags = pTemplate->field_58_sprite_flags;
            pDef->field_60_destroyed_by_destructor = pTemplate->field_59_destroyed_by_destructor;
            pDef->field_5C_instance_limit_kind = pTemplate->field_5C_instance_limit_kind;
            pDef->field_68_uses_bullet_walls = pTemplate->field_60_uses_bullet_walls;
            pDef->field_70_has_sound = pTemplate->field_64_has_sound;
            pDef->field_62_has_shadows = pTemplate->field_68_has_shadows;
            pDef->field_63_makes_collision_sound = pTemplate->field_69_makes_collision_sound;
            pTemplate++;
        }
    }
}

MATCH_FUNC(0x533c90)
void ObjectDefinitions::CloneRemappedDefinitions_533C90()
{
    ObjectDefinition* pDef;
    pDef = CloneDefinition_534370(0x93, 0x90);
    pDef->SetRemap_533110(4);
    pDef = CloneDefinition_534370(0xfa, 0xfd);
    pDef->SetRemap_533110(4);
    pDef = CloneDefinition_534370(0x78, 0x79);
    pDef->SetRemap_533110(4);
    pDef = CloneDefinition_534370(0x75, 0x76);
    pDef->SetRemap_533110(4);
    pDef = CloneDefinition_534370(0x92, 0x90);
    pDef->SetRemap_533110(3);
    pDef = CloneDefinition_534370(0xf9, 0xfd);
    pDef->SetRemap_533110(3);
    pDef = CloneDefinition_534370(0x77, 0x79);
    pDef->SetRemap_533110(3);
    pDef = CloneDefinition_534370(0x74, 0x76);
    pDef->SetRemap_533110(3);
    pDef = CloneDefinition_534370(0x91, 0x90);
    pDef->SetRemap_533110(5);
    pDef->field_64_next_frame_max = 3;

    pDef = CloneDefinition_534370(0x7c, 0xfd);
    pDef->SetRemap_533110(5);
    pDef->field_64_next_frame_max = 3;
    pDef = CloneDefinition_534370(0x7d, 0x79);
    pDef->SetRemap_533110(5);
    pDef->field_64_next_frame_max = 3;
    pDef = CloneDefinition_534370(0x7e, 0x76);
    pDef->SetRemap_533110(5);
    pDef->field_64_next_frame_max = 3;

    pDef = CloneDefinition_534370(0x9a, 0xfe);
    if (bDo_show_imaginary_67D588 == false)
    {
        pDef->field_40_collision_bucket_category = collision_bucket_category::none_2;
    }
    CloneDefinition_534370(0x109, 0xfe);
    pDef = CloneDefinition_534370(0x9f, 0x80);
    if (bDo_show_imaginary_67D588 == false)
    {
        pDef->field_40_collision_bucket_category = collision_bucket_category::none_2;
    }

    CloneDefinition_534370(0x115, 0xfe);
    pDef = CloneDefinition_534370(0x6e, 0x9b);
    pDef->field_38 = 0x6e;
    pDef->field_3C_next_definition_idx = 0x6e;
    pDef->field_54_react_to_collisions_with = CollisionReaction::OnlyPeds_2;
    pDef->SetDimensions_533060(kFpPoint2_6FC578, kFpPoint2_6FC578, kFpQuarter_6F8FAC);
    pDef->field_18_mass = kFpHalf_6FC584;
    if (bDo_show_imaginary_67D588 == false)
    {
        pDef->field_40_collision_bucket_category = collision_bucket_category::none_2;
    }
    else
    {
        pDef->field_40_collision_bucket_category = collision_bucket_category::sprite_grid_3_single_cell_1;
    }
    pDef->field_44_touch_behavior = 2;
    pDef->field_58_uses_3d_movement = 1;
    pDef->field_50_z_collision_response = 1;

    pDef = CloneDefinition_534370(0x7f, 0x9b);
    pDef->field_38 = 0;
    pDef->field_3C_next_definition_idx = 0;
    pDef->field_54_react_to_collisions_with = CollisionReaction::OnlyPeds_2;
    pDef->SetDimensions_533060((kFpOne64th_6FCEB0 * 8), (kFpOne64th_6FCEB0 * 8), kFpQuarter_6F8FAC);
    pDef->field_18_mass = kFpHalf_6FC584;
    if (bDo_show_imaginary_67D588 == '\0')
    {
        pDef->field_40_collision_bucket_category = collision_bucket_category::none_2;
    }
    else
    {
        pDef->field_40_collision_bucket_category = collision_bucket_category::sprite_grid_3_single_cell_1;
    }
    pDef->field_44_touch_behavior = 2;
    pDef->field_58_uses_3d_movement = 1;
    pDef->field_50_z_collision_response = 1;

    pDef = CloneDefinition_534370(0xc1, 0xc0);
    pDef->field_38 = 0;
    pDef->field_3C_next_definition_idx = 0;
    if (bDo_show_imaginary_67D588 == '\0')
    {
        pDef->field_40_collision_bucket_category = collision_bucket_category::none_2;
    }

    pDef = CloneDefinition_534370(0xb9, 0xa3);
    pDef->field_38 = 0xb8;
    pDef->field_3C_next_definition_idx = 0xb8;
    pDef->field_44_touch_behavior = 4;
    pDef->field_40_collision_bucket_category = collision_bucket_category::sprite_grid_2_region_3;
    pDef->field_61_in_pool_list = '\x01';

    pDef = CloneDefinition_534370(0xb8, 0xa3);
    pDef->field_38 = 0xb9;
    pDef->field_3C_next_definition_idx = 0xa4;
    pDef->field_65_next_definition_timer = 'd';
    pDef->field_61_in_pool_list = '\x01';
    pDef->field_34_behavior_type = object_behavior_type::animated_2;
    pDef->field_6C_num_frames = '\x01';
    pDef->field_64_next_frame_max = '\x01';
    pDef->field_40_collision_bucket_category = collision_bucket_category::sprite_grid_2_region_3;
    pDef->field_44_touch_behavior = 5;

    pDef = CloneDefinition_534370(0xb0, 0xa3);
    pDef->SetRemap_533110(1);
    pDef->field_38 = 0xb1;
    pDef->field_3C_next_definition_idx = 0xb1;
    if (bDo_kill_phones_on_answer_67D6E8 != '\0')
    {
        pDef = GetObjectDefinition_534360(0xa4);
        pDef->field_38 = 0xae;
        pDef->field_3C_next_definition_idx = 0xae;
    }
    pDef = CloneDefinition_534370(0xb1, 0xa4);
    pDef->SetRemap_533110(1);
    if (bDo_kill_phones_on_answer_67D6E8 == '\0')
    {
        pDef->field_38 = 0xbb;
        pDef->field_3C_next_definition_idx = 0xbb;
    }

    pDef = CloneDefinition_534370(0xbb, 0xb9);
    pDef->SetRemap_533110(1);
    pDef->field_38 = 0xba;
    pDef->field_3C_next_definition_idx = 0xba;

    pDef = CloneDefinition_534370(0xba, 0xb8);
    pDef->SetRemap_533110(1);
    pDef->field_38 = 0xbb;
    pDef->field_3C_next_definition_idx = 0xb1;

    pDef = CloneDefinition_534370(0xb2, 0xa3);
    pDef->SetRemap_533110(2);
    pDef->field_38 = 0xb3;
    pDef->field_3C_next_definition_idx = 0xb3;

    pDef = CloneDefinition_534370(0xb3, 0xa4);
    pDef->SetRemap_533110(2);
    if (bDo_kill_phones_on_answer_67D6E8 == '\0')
    {
        pDef->field_38 = 0xbd;
        pDef->field_3C_next_definition_idx = 0xbd;
    }
    pDef = CloneDefinition_534370(0xbd, 0xb9);
    pDef->SetRemap_533110(2);
    pDef->field_38 = 0xbc;
    pDef->field_3C_next_definition_idx = 0xbc;

    pDef = CloneDefinition_534370(0xbc, 0xb8);
    pDef->SetRemap_533110(2);
    pDef->field_38 = 0xbd;
    pDef->field_3C_next_definition_idx = 0xb3;

    pDef = CloneDefinition_534370(0xb4, 0xa3);
    pDef->SetRemap_533110(0);
    pDef->field_38 = 0xb5;
    pDef->field_3C_next_definition_idx = 0xb5;

    pDef = CloneDefinition_534370(0xb5, 0xa4);
    pDef->SetRemap_533110(0);
    if (bDo_kill_phones_on_answer_67D6E8 == '\0')
    {
        pDef->field_38 = 0xbf;
        pDef->field_3C_next_definition_idx = 0xbf;
    }

    pDef = CloneDefinition_534370(0xbf, 0xb9);
    pDef->SetRemap_533110(0);
    pDef->field_38 = 0xbe;
    pDef->field_3C_next_definition_idx = 0xbe;

    pDef = CloneDefinition_534370(0xbe, 0xb8);
    pDef->SetRemap_533110(0);
    pDef->field_38 = 0xbf;
    pDef->field_3C_next_definition_idx = 0xb5;

    pDef = CloneDefinition_534370(0xae, 0xa3);
    pDef->field_44_touch_behavior = 0;

    pDef = CloneDefinition_534370(0xb6, 0xb7);
    pDef->field_38 = 0xb7;
    pDef->field_34_behavior_type = object_behavior_type::animated_2;
    pDef->field_44_touch_behavior = 2;

    pDef->field_14_friction = kFpZero_6FCE08;
    pDef->field_10_speed = kFpZero_6FCE08;
    pDef->field_4C = 0;
    pDef->field_50_z_collision_response = 0;
    pDef->field_64_next_frame_max = '\b';
    pDef->OffsetBaseSpriteIdxAndSetNumFrames_533150(pDef->field_6C_num_frames - 1, 1);

    for(s32 def_idx = 200; def_idx - 200 <= 44; def_idx++)
    {
        pDef = CloneDefinition_534370(def_idx - 0x88, def_idx);
        pDef->field_38 = def_idx;
        pDef->field_34_behavior_type = pDef->field_34_behavior_type == object_behavior_type::pickup_6 ? object_behavior_type::pickup_physics_7 : object_behavior_type::pickup_physics_animated_9;
        pDef->field_14_friction = -kFpPoint01_6FCDD8;
        pDef->field_4C = 1;
        pDef->field_50_z_collision_response = 1;
        pDef->field_61_in_pool_list = '\x01';
        pDef->field_58_uses_3d_movement = 1;
        pDef->field_40_collision_bucket_category = collision_bucket_category::sprite_grid_2_region_3;
        pDef->field_18_mass = kFpPoint1_6F8FD8;
        pDef->field_10_speed = kFpPoint02_6FCDE0;
    }

    ClearColour1PixelsOfDefinitions287To293_534270();
}

MATCH_FUNC(0x534270)
void ObjectDefinitions::ClearColour1PixelsOfDefinitions287To293_534270()
{
    ClearColour1PixelsOfDefinitionSprite_5342F0(287);
    ClearColour1PixelsOfDefinitionSprite_5342F0(288);
    ClearColour1PixelsOfDefinitionSprite_5342F0(289);
    ClearColour1PixelsOfDefinitionSprite_5342F0(290);
    ClearColour1PixelsOfDefinitionSprite_5342F0(291);
    ClearColour1PixelsOfDefinitionSprite_5342F0(292);
    ClearColour1PixelsOfDefinitionSprite_5342F0(293);
}

MATCH_FUNC(0x5342d0)
void ObjectDefinitions::CacheDef112BaseSpriteIdx_5342D0()
{
    field_8CA4_def112_base_sprite_idx = GetObjectDefinition_534360(112)->field_1E_base_sprite_idx;
}

MATCH_FUNC(0x5342f0)
void ObjectDefinitions::ClearColour1PixelsOfDefinitionSprite_5342F0(s32 idx)
{
    ObjectDefinition* pDef = GetObjectDefinition_534360(idx);
    u16 sprite_idx = gGtx_0x106C_703DD4->GetSpriteTrueIndex_5AA460(sprite_types_enum::code_obj1_4, pDef->field_1E_base_sprite_idx);
    sprite_index* psprite_index = gGtx_0x106C_703DD4->get_sprite_index_5AA440(sprite_idx);
    psprite_index->ClearPixelsOfColour_5ABAA0(1);
}

MATCH_FUNC(0x534330)
void ObjectDefinitions::InitDefinitions_534330()
{
    CreateMapObjectDefinitions_533300();
    ApplyMapObjectOverrides_533360();
    CloneAnimatedDefinitions_533420();
    CreateCodeObjectDefinitions_533B30();
    CloneRemappedDefinitions_533C90();
    CacheDef112BaseSpriteIdx_5342D0();
}

MATCH_FUNC(0x534360)
ObjectDefinition* ObjectDefinitions::GetObjectDefinition_534360(s32 idx)
{
    return field_87F4_definition_by_idx[idx];
}

MATCH_FUNC(0x534370)
ObjectDefinition* ObjectDefinitions::CloneDefinition_534370(s32 dst_idx, s32 src_idx)
{
    ObjectDefinition* result = &field_4_definitions[field_0_next_idx];
    field_87F4_definition_by_idx[dst_idx] = result;
    ++field_0_next_idx;
    memcpy(result, field_87F4_definition_by_idx[src_idx], sizeof(ObjectDefinition));
    result->field_24_object_idx = dst_idx;
    return result;
}

MATCH_FUNC(0x5343c0)
ObjectDefinition* ObjectDefinitions::AllocDefinition_5343C0(s32 idx)
{
    ObjectDefinition* result = &field_4_definitions[field_0_next_idx];
    field_87F4_definition_by_idx[idx] = result;
    ++field_0_next_idx;
    result->field_24_object_idx = idx;
    return result;
}

MATCH_FUNC(0x5343f0)
ObjectDefinitions::ObjectDefinitions()
{
    for (s32 i = 0; i < GTA2_COUNTOF(field_87F4_definition_by_idx); i++)
    {
        field_87F4_definition_by_idx[i] = 0;
    }
    field_0_next_idx = 0;
    field_8CA4_def112_base_sprite_idx = 99;
}