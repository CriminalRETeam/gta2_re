#include "ObjectDefinitions.hpp"
#include "enums.hpp"
#include "objects_more.hpp"
#include "instance_limit_kind.hpp"
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
DEFINE_STATIC_GLOBAL_INIT(Fix16, kFpPoint02, Fix16(327, 0), 0x6fcde0);
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

// Must remain a fixed array, a pointer would break ObjectDefinitions::CreateCodeObjectDefinitions
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

// Must remain a fixed array, a pointer would break ObjectDefinitions::ApplyMapObjectOverrides
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
               u32 impact_definition_idx,
               u32 next_definition_idx,
               u8 in_pool_list,
               const u32& touch_behavior,
               const u32& projectile_hit_type,
               const u32& collision_bucket_category,
               Fix16 speed,
               Fix16 friction,
               s8 next_definition_timer,
               const u32& collision_response,
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
    mDefinitionIdx = definition_idx;
    mNumSprites = num_sprites;
    mBehaviorType = behavior_type;
    mImpactDefinitionIdx = impact_definition_idx;
    mNextDefinitionIdx = next_definition_idx;
    mInPoolList = in_pool_list;
    mTouchBehavior = touch_behavior;
    mProjectileHitType = projectile_hit_type;
    mCollisionBucketCategory = collision_bucket_category;
    mSpeed = speed;
    mFriction = friction;
    mNextDefinitionTimer = next_definition_timer;
    mCollisionResponse = collision_response;
    mZCollisionResponse = z_collision_response;
    mSpriteType = sprite_type;
    mNextFrameMax = next_frame_max;
    mUses3dMovement = uses_3d_movement;
    mMass = mass;
    mWidth = width;
    mHeight = height;
    mDepth = depth;
    mDrawLayer = draw_layer;
    mSpriteFlags = sprite_flags;
    mDestroyedByDestructor = destroyed_by_destructor;
    mInstanceLimitKind = instance_limit_kind;
    mUsesBulletWalls = uses_bullet_walls;
    mHasSound = has_sound;
    mHasShadows = has_shadows;
    mMakesCollisionSound = makes_collision_sound;
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
               const s32& collision_response,
               const s32& z_collision_response,
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
    mDefinitionIdx = definition_idx;
    mBehaviorType = behavior_type;
    mNextDefinitionIdx = next_definition_idx;
    mInPoolList = in_pool_list;
    mTouchBehavior = touch_behavior;
    mProjectileHitType = projectile_hit_type;
    mCollisionBucketCategory = collision_bucket_category;
    mSpeed = speed;
    mFriction = friction;
    mNextDefinitionTimer = next_definition_timer;
    mCollisionResponse = collision_response;
    mZCollisionResponse = z_collision_response;
    mNextFrameMax = next_frame_max;
    mUses3dMovement = uses_3d_movement;
    mMass = mass;
    mDrawLayer = draw_layer;
    mSpriteFlags = sprite_flags;
    mHasSound = has_sound;
    mHasShadows = has_shadows;
    mInstanceLimitKind = instance_limit_kind;
    mMakesCollisionSound = makes_collision_sound;
}

MATCH_FUNC(0x533060)
void ObjectDefinition::SetDimensions(Fix16 width, Fix16 height, Fix16 depth)
{
    mWidth = width;
    mHeight = height;
    mDepth = depth;
    if (mWidth < mHeight)
    {
        mMinSize = width;
    }
    else
    {
        mMinSize = height;
    }
}

MATCH_FUNC(0x533090)
void ObjectDefinition::SetDimensionsFromSprite()
{
    if (mSpriteType != sprite_types_enum::object_1)
    {
        u16 sprite_id = gGtx_0x106C_703DD4->GetSpriteTrueIndex(mSpriteType, mBaseSpriteIdx);
        sprite_index * sprite = gGtx_0x106C_703DD4->get_sprite_index(sprite_id);
        mWidth = Fix16(sprite->mWidth) / 0x40;
        mHeight = Fix16(sprite->mHeight) / 0x40;
        mDepth = kFpQuarter_6FCED0;
    }
    else
    {
        mWidth = kFpPoint6_6F8FF0;
        mHeight = kFpPoint6_6F8FF0;
        mDepth = kFpQuarter_6FCED0;
    }

    if (mWidth < mHeight)
    {
        mMinSize = mWidth;
    }
    else
    {
        mMinSize = mHeight;
    }
}

MATCH_FUNC(0x533110)
void ObjectDefinition::SetRemap(s16 remap)
{
    switch (mSpriteType)
    {
        case sprite_types_enum::code_obj1_4:
            mPaletteType = palette_types_enum::code_obj_remaps_5;
            break;
        case sprite_types_enum::map_obj_5:
            mPaletteType = palette_types_enum::map_obj_remaps_6;
            break;
    }
    mRemap = remap;
}

MATCH_FUNC(0x533150)
void ObjectDefinition::OffsetBaseSpriteIdxAndSetNumFrames(s16 sprite_idx_offset, s16 num_frames)
{
    mBaseSpriteIdx += sprite_idx_offset;
    mNumFrames = static_cast<u8>(num_frames);
}

MATCH_FUNC(0x533170)
Sprite* ObjectDefinition::CreateSpriteFromDefinition() const
{
    Sprite* pFreeSprite = gSpritePool_703818->get_new_sprite();
    ApplyDefinitionToSprite(pFreeSprite);
    return pFreeSprite;
}

// 255 as a raw Fix16
s32 kFp255_6F8FA4 = 0x3FC000;

MATCH_FUNC(0x5331a0)
void ObjectDefinition::ApplyDefinitionToSprite(Sprite* pSprite) const
{
    pSprite->SetType_4206F0(this->mSpriteType);
    pSprite->set_id_lazy_4206C0(this->mBaseSpriteIdx);

    pSprite->SetDrawLayer_40F7B0(this->mDrawLayer);
    pSprite->mFlags = this->mSpriteFlags;

    if (this->mPaletteType != palette_types_enum::sprites_2)
    {
        pSprite->SetRemap(this->mRemap);
    }

    const s32 bucket_category = this->mCollisionBucketCategory;
    if (bucket_category > collision_bucket_category::sprite_grid_3_single_cell_0 &&
        (bucket_category <= collision_bucket_category::sprite_grid_2_region_3 || bucket_category == collision_bucket_category::sprite_grid_1_region_4))
    {
        pSprite->AllocCollisionShape(this->mWidth, this->mHeight, this->mDepth);
    }

    if (this->mHasSound)
    {
        pSprite->CreateSoundObj();
    }
}

MATCH_FUNC(0x533220)
ObjectDefinition::ObjectDefinition()
{
    mWidth = kFp255_6F8FA4;
    mHeight = kFp255_6F8FA4;
    mDepth = kFp255_6F8FA4;
    mObjectIdx = 0;
    mSpriteType = sprite_types_enum::none_0;
    mDrawLayer = 0;
    mPaletteType = 0;
    mBehaviorType = object_behavior_type::static_object_0;
    mImpactDefinitionIdx = 0;
    mNextDefinitionIdx = 0;
    mCollisionBucketCategory = collision_bucket_category::sprite_grid_3_single_cell_0;
    mTouchBehavior = 0;
    mProjectileHitType = 0;
    mCollisionResponse = 1;
    mZCollisionResponse = 1;
    mUses3dMovement = 0;
    mReactToCollisionsWith = CollisionReaction::Always_0;
    mMinSize = kFp255_6F8FA4;
    mSpeed = kFp255_6F8FA4;
    mFriction = kFp255_6F8FA4;
    mRemap = 9999;
    mInPoolList = 0;
    mNextFrameMax = 99;
    mNextDefinitionTimer = 99;
    mBaseSpriteIdx = 99;
    mNumFrames = 99;
    Fix16 zero_mass = kFpZeroForStaticInit_6FCE08;
    mUsesBulletWalls = 0;
    mMass = zero_mass;
    mSpriteFlags = 0;
    mDestroyedByDestructor = 0;
    mInstanceLimitKind = instance_limit_kind::unlimited_1;
    mHasSound = 0;
}

MATCH_FUNC(0x5332d0)
ObjectDefinition* ObjectDefinitions::AllocDefinitionWithSprite(s32 idx, s32 sprite_type, s16 base_sprite_idx, u8 num_frames)
{
    ObjectDefinition* result = AllocDefinition(idx);
    result->mSpriteType = sprite_type;
    result->mBaseSpriteIdx = base_sprite_idx;
    result->mNumFrames = num_frames;
    result->mPaletteType = palette_types_enum::sprites_2;
    return result;
}

MATCH_FUNC(0x533300)
void ObjectDefinitions::CreateMapObjectDefinitions()
{
    s16 sprites = 0;
    u16 i = 0;
    object_info* pIter = gGtx_0x106C_703DD4->get_map_object_info(i++);
    while (pIter)
    {
        ObjectDefinition* pDefinition = AllocDefinitionWithSprite(pIter->mModel, sprite_types_enum::map_obj_5, sprites, pIter->mSprites);
        sprites += pIter->mSprites;
        pDefinition->SetDimensionsFromSprite();
        pIter = gGtx_0x106C_703DD4->get_map_object_info(i++);
    }
}

MATCH_FUNC(0x533360)
void ObjectDefinitions::ApplyMapObjectOverrides()
{
    s32 override_count = gMapObjectOverrides_length_623EEC;

    if (0 < override_count)
    {
        MapObjectOverride* pOverride = gMapObjectOverrides_6FC5F8;
        do
        {
            ObjectDefinition* pDef = GetObjectDefinition(pOverride->mDefinitionIdx);
            pDef->mBehaviorType = pOverride->mBehaviorType;
            pDef->mImpactDefinitionIdx = pOverride->mNextDefinitionIdx;
            pDef->mNextDefinitionIdx = pOverride->mNextDefinitionIdx;
            pDef->mInPoolList = pOverride->mInPoolList;
            pDef->mCollisionBucketCategory = pOverride->mCollisionBucketCategory;
            pDef->mTouchBehavior = pOverride->mTouchBehavior;
            pDef->mProjectileHitType = pOverride->mProjectileHitType;
            pDef->mSpeed = pOverride->mSpeed;
            pDef->mNextDefinitionTimer = pOverride->mNextDefinitionTimer;
            pDef->mFriction = pOverride->mFriction;
            pDef->mCollisionResponse = pOverride->mCollisionResponse;
            pDef->mNextFrameMax = pOverride->mNextFrameMax;
            pDef->mUses3dMovement = pOverride->mUses3dMovement;
            pDef->mMass = pOverride->mMass;
            pDef->mSpriteFlags = pOverride->mSpriteFlags;
            pDef->mDrawLayer = pOverride->mDrawLayer;
            pDef->mDestroyedByDestructor = 1;
            pDef->mInstanceLimitKind = pOverride->mInstanceLimitKind;
            pDef->mUsesBulletWalls = 0;
            pDef->mHasSound = pOverride->mHasSound;
            pDef->mHasShadows = pOverride->mHasShadows;
            pDef->mMakesCollisionSound = pOverride->mMakesCollisionSound;
            pDef->mReactToCollisionsWith = CollisionReaction::Always_0;
            pOverride++;
        } while (--override_count != 0);
    }
}

MATCH_FUNC(0x533420)
void ObjectDefinitions::CloneAnimatedDefinitions()
{
    ObjectDefinition* pDef;

    CloneDefinition(0x97, 6);
    pDef = GetObjectDefinition(0x97);
    pDef->OffsetBaseSpriteIdxAndSetNumFrames(1, pDef->mNumFrames + -1);
    pDef->mBehaviorType = object_behavior_type::animated_2;
    pDef->mNextFrameMax = 5;
    pDef->mImpactDefinitionIdx = 0x98;
    pDef->mNextDefinitionIdx = 0x98;
    pDef->mCollisionBucketCategory = collision_bucket_category::sprite_grid_3_single_cell_0;
    pDef->mTouchBehavior = 0;
    pDef->mNextDefinitionTimer = 1;
    pDef->mInPoolList = 1;

    pDef = CloneDefinition(0x98, 6);
    pDef->OffsetBaseSpriteIdxAndSetNumFrames(pDef->mNumFrames - 1, 1);
    pDef->mBehaviorType = object_behavior_type::static_object_0;
    pDef->mImpactDefinitionIdx = 0;
    pDef->mNextDefinitionIdx = 0;
    pDef->mCollisionBucketCategory = collision_bucket_category::sprite_grid_3_single_cell_0;
    pDef->mTouchBehavior = 0;
    pDef->mInPoolList = 0;

    pDef = CloneDefinition(0x34, 4);
    pDef->OffsetBaseSpriteIdxAndSetNumFrames(pDef->mNumFrames - 1, 1);
    pDef->mBehaviorType = object_behavior_type::static_object_0;
    pDef->mImpactDefinitionIdx = 0;
    pDef->mNextDefinitionIdx = 0;
    pDef->mCollisionBucketCategory = collision_bucket_category::sprite_grid_3_single_cell_0;
    pDef->mTouchBehavior = 0;
    pDef->mInPoolList = 0;

    CloneDefinition(0x32, 0xc);
    pDef = GetObjectDefinition(0x32);
    pDef->OffsetBaseSpriteIdxAndSetNumFrames(1, pDef->mNumFrames + -1);
    pDef->mBehaviorType = object_behavior_type::animated_2;
    pDef->mNextFrameMax = 3;
    pDef->mImpactDefinitionIdx = 0x33;
    pDef->mNextDefinitionIdx = 0x33;
    pDef->mCollisionBucketCategory = collision_bucket_category::sprite_grid_3_single_cell_0;
    pDef->mTouchBehavior = 0;
    pDef->mNextDefinitionTimer = 1;
    pDef->mInPoolList = 1;

    pDef = CloneDefinition(0x33, 0xc);
    pDef->OffsetBaseSpriteIdxAndSetNumFrames(pDef->mNumFrames - 1, 1);
    pDef->mBehaviorType = object_behavior_type::static_object_0;
    pDef->mImpactDefinitionIdx = 0;
    pDef->mNextDefinitionIdx = 0;
    pDef->mCollisionBucketCategory = collision_bucket_category::sprite_grid_3_single_cell_0;
    pDef->mTouchBehavior = 0;
    pDef->mInPoolList = 0;

    pDef = CloneDefinition(0x9b, 3);
    pDef->OffsetBaseSpriteIdxAndSetNumFrames(1, pDef->mNumFrames + -1);
    pDef->mBehaviorType = object_behavior_type::physics_animated_4;
    pDef->mNextFrameMax = 1;
    pDef->mImpactDefinitionIdx = 0x35;
    pDef->mNextDefinitionIdx = 0x35;
    pDef->mCollisionBucketCategory = collision_bucket_category::sprite_grid_1_region_4;
    pDef->mTouchBehavior = 0;
    pDef->mSpeed = kFpPoint1_6F8FD8;
    pDef->mFriction = -kFpPoint01_6FCDD8;
    pDef->mInPoolList = 1;
    pDef->mCollisionResponse = 3;
    pDef->mZCollisionResponse = 3;
    pDef->mNextDefinitionTimer = -1;
    pDef->mUses3dMovement = 1;

    pDef = CloneDefinition(0x35, 3);
    pDef->OffsetBaseSpriteIdxAndSetNumFrames(pDef->mNumFrames - 1, 1);

    pDef = CloneDefinition(0x7b, 0xb);
    pDef->OffsetBaseSpriteIdxAndSetNumFrames(1, pDef->mNumFrames + -1);
    pDef->mBehaviorType = object_behavior_type::physics_animated_4;
    pDef->mNextFrameMax = 1;
    pDef->mImpactDefinitionIdx = 0x37;
    pDef->mNextDefinitionIdx = 0x37;
    pDef->mCollisionBucketCategory = collision_bucket_category::sprite_grid_1_region_4;
    pDef->mTouchBehavior = 0;
    pDef->mSpeed = kFpPoint1_6F8FD8;
    pDef->mFriction = -kFpPoint01_6FCDD8;
    pDef->mInPoolList = 1;
    pDef->mCollisionResponse = 3;
    pDef->mZCollisionResponse = 3;
    pDef->mNextDefinitionTimer = -1;
    pDef->mUses3dMovement = 1;

    pDef = CloneDefinition(0x37, 0xb);
    pDef->OffsetBaseSpriteIdxAndSetNumFrames(pDef->mNumFrames - 1, 1);

    pDef = CloneDefinition(0x9c, 5);
    pDef->OffsetBaseSpriteIdxAndSetNumFrames(1, pDef->mNumFrames + -1);
    pDef->mBehaviorType = object_behavior_type::physics_animated_4;
    pDef->mNextFrameMax = 1;
    pDef->mImpactDefinitionIdx = 0x36;
    pDef->mNextDefinitionIdx = 0x36;
    pDef->mCollisionBucketCategory = collision_bucket_category::sprite_grid_1_region_4;
    pDef->mTouchBehavior = 0;
    pDef->mSpeed = kFpPoint1_6F8FD8;
    pDef->mFriction = -kFpPoint01_6FCDD8;
    pDef->mInPoolList = 1;
    pDef->mCollisionResponse = 3;
    pDef->mZCollisionResponse = 3;
    pDef->mNextDefinitionTimer = -1;
    pDef->mUses3dMovement = 1;

    pDef = CloneDefinition(0x36, 5);
    pDef->OffsetBaseSpriteIdxAndSetNumFrames(pDef->mNumFrames - 1, 1);

    pDef = CloneDefinition(0x38, 0xd);
    pDef->OffsetBaseSpriteIdxAndSetNumFrames(1, pDef->mNumFrames + -1);
    pDef->mBehaviorType = object_behavior_type::physics_animated_4;
    pDef->mNextFrameMax = 1;
    pDef->mImpactDefinitionIdx = 0xd;
    pDef->mNextDefinitionIdx = 0xd;
    pDef->mCollisionBucketCategory = collision_bucket_category::sprite_grid_1_region_4;
    pDef->mTouchBehavior = 0;
    pDef->mSpeed = kFpPoint1_6F8FD8;
    pDef->mFriction = -kFpPoint01_6FCDD8;
    pDef->mInPoolList = 1;
    pDef->mCollisionResponse = 3;
    pDef->mZCollisionResponse = 3;
    pDef->mNextDefinitionTimer = -1;
    pDef->mUses3dMovement = 1;

    pDef = CloneDefinition(0x39, 0xe);
    pDef->OffsetBaseSpriteIdxAndSetNumFrames(1, pDef->mNumFrames + -1);
    pDef->mBehaviorType = object_behavior_type::physics_animated_4;
    pDef->mNextFrameMax = 1;
    pDef->mImpactDefinitionIdx = 0x3a;
    pDef->mNextDefinitionIdx = 0x3a;
    pDef->mCollisionBucketCategory = collision_bucket_category::sprite_grid_1_region_4;
    pDef->mTouchBehavior = 0;
    pDef->mSpeed = kFpPoint1_6F8FD8;
    pDef->mFriction = -kFpPoint01_6FCDD8;
    pDef->mInPoolList = 1;
    pDef->mCollisionResponse = 3;
    pDef->mZCollisionResponse = 3;
    pDef->mNextDefinitionTimer = -1;
    pDef->mUses3dMovement = 1;

    pDef = CloneDefinition(0x3a, 0xe);
    pDef->OffsetBaseSpriteIdxAndSetNumFrames(pDef->mNumFrames - 1, 1);

    pDef = CloneDefinition(0x3b, 0xf);
    pDef->OffsetBaseSpriteIdxAndSetNumFrames(1, pDef->mNumFrames + -1);
    pDef->mBehaviorType = object_behavior_type::physics_animated_4;
    pDef->mNextFrameMax = 1;
    pDef->mImpactDefinitionIdx = 0x3c;
    pDef->mNextDefinitionIdx = 0x3c;
    pDef->mCollisionBucketCategory = collision_bucket_category::sprite_grid_1_region_4;
    pDef->mTouchBehavior = 0;
    pDef->mSpeed = kFpPoint1_6F8FD8;
    pDef->mFriction = -kFpPoint01_6FCDD8;
    pDef->mInPoolList = 1;
    pDef->mCollisionResponse = 3;
    pDef->mZCollisionResponse = 3;
    pDef->mNextDefinitionTimer = -1;
    pDef->mUses3dMovement = 1;

    pDef = CloneDefinition(0x3c, 0xf);
    pDef->OffsetBaseSpriteIdxAndSetNumFrames(pDef->mNumFrames - 1, 1);

    pDef = CloneDefinition(0x3d, 0x10);
    pDef->OffsetBaseSpriteIdxAndSetNumFrames(1, pDef->mNumFrames + -1);
    pDef->mBehaviorType = object_behavior_type::physics_animated_4;
    pDef->mNextFrameMax = 1;
    pDef->mImpactDefinitionIdx = 0x3e;
    pDef->mNextDefinitionIdx = 0x3e;
    pDef->mCollisionBucketCategory = collision_bucket_category::sprite_grid_1_region_4;
    pDef->mTouchBehavior = 0;
    pDef->mSpeed = kFpPoint1_6F8FD8;
    pDef->mFriction = -kFpPoint01_6FCDD8;
    pDef->mInPoolList = 1;
    pDef->mCollisionResponse = 3;
    pDef->mZCollisionResponse = 3;
    pDef->mNextDefinitionTimer = -1;
    pDef->mUses3dMovement = 1;

    pDef = CloneDefinition(0x3e, 0x10);
    pDef->OffsetBaseSpriteIdxAndSetNumFrames(pDef->mNumFrames - 1, 1);

    pDef = CloneDefinition(0x31, 0x12);
    pDef->OffsetBaseSpriteIdxAndSetNumFrames(1, pDef->mNumFrames + -1);
    pDef->mBehaviorType = object_behavior_type::physics_animated_4;
    pDef->mNextFrameMax = 1;
    pDef->mImpactDefinitionIdx = 0x12;
    pDef->mNextDefinitionIdx = 0x12;
    pDef->mCollisionBucketCategory = collision_bucket_category::sprite_grid_1_region_4;
    pDef->mTouchBehavior = 0;
    pDef->mSpeed = kFpPoint1_6F8FD8;
    pDef->mFriction = -kFpPoint01_6FCDD8;
    pDef->mInPoolList = 1;
    pDef->mCollisionResponse = 3;
    pDef->mZCollisionResponse = 3;
    pDef->mNextDefinitionTimer = -1;
    pDef->mUses3dMovement = 1;

    pDef = CloneDefinition(0x2d, 0x16);
    pDef->OffsetBaseSpriteIdxAndSetNumFrames(1, pDef->mNumFrames + -1);
    pDef->mBehaviorType = object_behavior_type::physics_animated_4;
    pDef->mNextFrameMax = 1;
    pDef->mImpactDefinitionIdx = 0x2e;
    pDef->mNextDefinitionIdx = 0x2e;
    pDef->mCollisionBucketCategory = collision_bucket_category::sprite_grid_1_region_4;
    pDef->mTouchBehavior = 0;
    pDef->mSpeed = kFpPoint1_6F8FD8;
    pDef->mFriction = -kFpPoint01_6FCDD8;
    pDef->mInPoolList = 1;
    pDef->mCollisionResponse = 3;
    pDef->mZCollisionResponse = 3;
    pDef->mNextDefinitionTimer = -1;
    pDef->mUses3dMovement = 1;

    pDef = CloneDefinition(0x2e, 0x16);
    pDef->OffsetBaseSpriteIdxAndSetNumFrames(pDef->mNumFrames - 1, 1);

    pDef = CloneDefinition(0x2f, 0x15);
    pDef->OffsetBaseSpriteIdxAndSetNumFrames(1, pDef->mNumFrames + -1);
    pDef->mBehaviorType = object_behavior_type::physics_animated_4;
    pDef->mNextFrameMax = 1;
    pDef->mImpactDefinitionIdx = 0x30;
    pDef->mNextDefinitionIdx = 0x30;
    pDef->mCollisionBucketCategory = collision_bucket_category::sprite_grid_1_region_4;
    pDef->mTouchBehavior = 0;
    pDef->mSpeed = kFpPoint1_6F8FD8;
    pDef->mFriction = -kFpPoint01_6FCDD8;
    pDef->mInPoolList = 1;
    pDef->mCollisionResponse = 3;
    pDef->mZCollisionResponse = 3;
    pDef->mNextDefinitionTimer = -1;
    pDef->mUses3dMovement = 1;

    pDef = CloneDefinition(0x30, 0x15);
    pDef->OffsetBaseSpriteIdxAndSetNumFrames(pDef->mNumFrames - 1, 1);

    pDef = CloneDefinition(0x3f, 0x11);
    pDef->mImpactDefinitionIdx = 0x11;
    pDef->mBehaviorType = object_behavior_type::physics_3;
    pDef->mNextDefinitionIdx = 0x11;
    pDef->mCollisionBucketCategory = collision_bucket_category::sprite_grid_1_region_4;
    pDef->mTouchBehavior = 0;
    pDef->mSpeed = kFpPoint1_6F8FD8;
    pDef->mFriction = -kFpPoint01_6FCDD8;
    pDef->mCollisionResponse = 3;
    pDef->mZCollisionResponse = 3;
    pDef->mInPoolList = 1;
    pDef->mUses3dMovement = 1;

    pDef = CloneDefinition(0x2b, 0x17);
    pDef->OffsetBaseSpriteIdxAndSetNumFrames(1, pDef->mNumFrames + -1);
    pDef->mBehaviorType = object_behavior_type::physics_animated_4;
    pDef->mNextFrameMax = 1;
    pDef->mImpactDefinitionIdx = 0x2c;
    pDef->mNextDefinitionIdx = 0x2c;
    pDef->mCollisionBucketCategory = collision_bucket_category::sprite_grid_1_region_4;
    pDef->mTouchBehavior = 0;
    pDef->mSpeed = kFpPoint1_6F8FD8;
    pDef->mFriction = -kFpPoint01_6FCDD8;
    pDef->mInPoolList = 1;
    pDef->mCollisionResponse = 3;
    pDef->mZCollisionResponse = 3;
    pDef->mNextDefinitionTimer = -1;
    pDef->mUses3dMovement = 1;

    pDef = CloneDefinition(0x2c, 0x17);
    pDef->OffsetBaseSpriteIdxAndSetNumFrames(pDef->mNumFrames - 1, 1);

    pDef = CloneDefinition(0x9d, 7);
    pDef->mCollisionResponse = 2;
    pDef->mBehaviorType = object_behavior_type::physics_3;
    pDef->mFriction = -kFpPoint01_6FCDD8;
    pDef->mImpactDefinitionIdx = 7;
    pDef->mNextDefinitionIdx = 7;
    pDef->mCollisionBucketCategory = collision_bucket_category::sprite_grid_1_region_4;
    pDef->mTouchBehavior = 0;
    pDef->mInPoolList = 1;
    pDef->mUses3dMovement = 1;

    pDef = CloneDefinition(0x9e, 1);
    pDef->mBehaviorType = object_behavior_type::physics_3;
    pDef->mCollisionResponse = 2;
    pDef->mFriction = -kFpPoint01_6FCDD8;
    pDef->mImpactDefinitionIdx = 1;
    pDef->mNextDefinitionIdx = 1;
    pDef->mTouchBehavior = 0;
    pDef->mInPoolList = 1;
    pDef->mUses3dMovement = 1;
    pDef = GetObjectDefinition(0x19);
    pDef->mSpriteType = sprite_types_enum::object_1;
    pDef->mDepth = kFpTwo_6FCE10;
}

MATCH_FUNC(0x533b30)
void ObjectDefinitions::CreateCodeObjectDefinitions()
{
    u16 sprite_base = 0;
    if (0 < gCodeObjectTemplates_length_623EF0)
    {
        CodeObjectTemplate* pTemplate = &gCodeObjectTemplates_6F9038[0];
        for(s32 remaining = gCodeObjectTemplates_length_623EF0; remaining != 0; remaining--)
        {
            ObjectDefinition* pDef = AllocDefinitionWithSprite(pTemplate->mDefinitionIdx, sprite_types_enum::code_obj1_4, sprite_base, pTemplate->mNumSprites);
            sprite_base += pTemplate->mNumSprites;
            pDef->mSpriteType = pTemplate->mSpriteType;
            if (pTemplate->mWidth == kFpZero_6FCE08 && pTemplate->mHeight == kFpZero_6FCE08 && pTemplate->mDepth == kFpZero_6FCE08)
            {
                pDef->SetDimensionsFromSprite();
            }
            else
            {
                pDef->SetDimensions(pTemplate->mWidth, pTemplate->mHeight, pTemplate->mDepth);
            }

            pDef->mBehaviorType = pTemplate->mBehaviorType;
            pDef->mImpactDefinitionIdx = pTemplate->mImpactDefinitionIdx;
            pDef->mNextDefinitionIdx = pTemplate->mNextDefinitionIdx;
            pDef->mInPoolList = pTemplate->mInPoolList;
            pDef->mCollisionBucketCategory = pTemplate->mCollisionBucketCategory;
            pDef->mTouchBehavior = pTemplate->mTouchBehavior;
            pDef->mProjectileHitType = pTemplate->mProjectileHitType;
            pDef->mSpeed = pTemplate->mSpeed;
            pDef->mNextDefinitionTimer = pTemplate->mNextDefinitionTimer;
            pDef->mFriction = pTemplate->mFriction;
            pDef->mCollisionResponse = pTemplate->mCollisionResponse;
            pDef->mZCollisionResponse = pTemplate->mZCollisionResponse;
            pDef->mNextFrameMax = pTemplate->mNextFrameMax;
            pDef->mReactToCollisionsWith = CollisionReaction::Always_0;
            pDef->mUses3dMovement = pTemplate->mUses3dMovement;
            pDef->mMass = pTemplate->mMass;
            pDef->mDrawLayer = pTemplate->mDrawLayer;
            pDef->mSpriteFlags = pTemplate->mSpriteFlags;
            pDef->mDestroyedByDestructor = pTemplate->mDestroyedByDestructor;
            pDef->mInstanceLimitKind = pTemplate->mInstanceLimitKind;
            pDef->mUsesBulletWalls = pTemplate->mUsesBulletWalls;
            pDef->mHasSound = pTemplate->mHasSound;
            pDef->mHasShadows = pTemplate->mHasShadows;
            pDef->mMakesCollisionSound = pTemplate->mMakesCollisionSound;
            pTemplate++;
        }
    }
}

MATCH_FUNC(0x533c90)
void ObjectDefinitions::CloneRemappedDefinitions()
{
    ObjectDefinition* pDef;
    pDef = CloneDefinition(0x93, 0x90);
    pDef->SetRemap(4);
    pDef = CloneDefinition(0xfa, 0xfd);
    pDef->SetRemap(4);
    pDef = CloneDefinition(0x78, 0x79);
    pDef->SetRemap(4);
    pDef = CloneDefinition(0x75, 0x76);
    pDef->SetRemap(4);
    pDef = CloneDefinition(0x92, 0x90);
    pDef->SetRemap(3);
    pDef = CloneDefinition(0xf9, 0xfd);
    pDef->SetRemap(3);
    pDef = CloneDefinition(0x77, 0x79);
    pDef->SetRemap(3);
    pDef = CloneDefinition(0x74, 0x76);
    pDef->SetRemap(3);
    pDef = CloneDefinition(0x91, 0x90);
    pDef->SetRemap(5);
    pDef->mNextFrameMax = 3;

    pDef = CloneDefinition(0x7c, 0xfd);
    pDef->SetRemap(5);
    pDef->mNextFrameMax = 3;
    pDef = CloneDefinition(0x7d, 0x79);
    pDef->SetRemap(5);
    pDef->mNextFrameMax = 3;
    pDef = CloneDefinition(0x7e, 0x76);
    pDef->SetRemap(5);
    pDef->mNextFrameMax = 3;

    pDef = CloneDefinition(0x9a, 0xfe);
    if (!bDo_show_imaginary_67D588)
    {
        pDef->mCollisionBucketCategory = collision_bucket_category::none_2;
    }
    CloneDefinition(0x109, 0xfe);
    pDef = CloneDefinition(0x9f, 0x80);
    if (!bDo_show_imaginary_67D588)
    {
        pDef->mCollisionBucketCategory = collision_bucket_category::none_2;
    }

    CloneDefinition(0x115, 0xfe);
    pDef = CloneDefinition(0x6e, 0x9b);
    pDef->mImpactDefinitionIdx = 0x6e;
    pDef->mNextDefinitionIdx = 0x6e;
    pDef->mReactToCollisionsWith = CollisionReaction::OnlyPeds_2;
    pDef->SetDimensions(kFpPoint2_6FC578, kFpPoint2_6FC578, kFpQuarter_6F8FAC);
    pDef->mMass = kFpHalf_6FC584;
    if (!bDo_show_imaginary_67D588)
    {
        pDef->mCollisionBucketCategory = collision_bucket_category::none_2;
    }
    else
    {
        pDef->mCollisionBucketCategory = collision_bucket_category::sprite_grid_3_single_cell_1;
    }
    pDef->mTouchBehavior = 2;
    pDef->mUses3dMovement = 1;
    pDef->mZCollisionResponse = 1;

    pDef = CloneDefinition(0x7f, 0x9b);
    pDef->mImpactDefinitionIdx = 0;
    pDef->mNextDefinitionIdx = 0;
    pDef->mReactToCollisionsWith = CollisionReaction::OnlyPeds_2;
    pDef->SetDimensions((kFpOne64th_6FCEB0 * 8), (kFpOne64th_6FCEB0 * 8), kFpQuarter_6F8FAC);
    pDef->mMass = kFpHalf_6FC584;
    if (!bDo_show_imaginary_67D588)
    {
        pDef->mCollisionBucketCategory = collision_bucket_category::none_2;
    }
    else
    {
        pDef->mCollisionBucketCategory = collision_bucket_category::sprite_grid_3_single_cell_1;
    }
    pDef->mTouchBehavior = 2;
    pDef->mUses3dMovement = 1;
    pDef->mZCollisionResponse = 1;

    pDef = CloneDefinition(0xc1, 0xc0);
    pDef->mImpactDefinitionIdx = 0;
    pDef->mNextDefinitionIdx = 0;
    if (!bDo_show_imaginary_67D588)
    {
        pDef->mCollisionBucketCategory = collision_bucket_category::none_2;
    }

    pDef = CloneDefinition(objects::blue_phone_answered_185, objects::blue_phone_not_ringing_163);
    pDef->mImpactDefinitionIdx = objects::blue_phone_after_answer_184;
    pDef->mNextDefinitionIdx = objects::blue_phone_after_answer_184;
    pDef->mTouchBehavior = 4;
    pDef->mCollisionBucketCategory = collision_bucket_category::sprite_grid_2_region_3;
    pDef->mInPoolList = 1;

    pDef = CloneDefinition(objects::blue_phone_after_answer_184, objects::blue_phone_not_ringing_163);
    pDef->mImpactDefinitionIdx = objects::blue_phone_answered_185;
    pDef->mNextDefinitionIdx = objects::blue_phone_164;
    pDef->mNextDefinitionTimer = 100;
    pDef->mInPoolList = 1;
    pDef->mBehaviorType = object_behavior_type::animated_2;
    pDef->mNumFrames = 1;
    pDef->mNextFrameMax = 1;
    pDef->mCollisionBucketCategory = collision_bucket_category::sprite_grid_2_region_3;
    pDef->mTouchBehavior = 5;

    pDef = CloneDefinition(objects::red_phone_not_ringing_176, objects::blue_phone_not_ringing_163);
    pDef->SetRemap(1);
    pDef->mImpactDefinitionIdx = objects::red_phone_177;
    pDef->mNextDefinitionIdx = objects::red_phone_177;
    if (bDo_kill_phones_on_answer_67D6E8)
    {
        pDef = GetObjectDefinition(objects::blue_phone_164);
        pDef->mImpactDefinitionIdx = objects::dead_phone_174;
        pDef->mNextDefinitionIdx = objects::dead_phone_174;
    }
    pDef = CloneDefinition(objects::red_phone_177, objects::blue_phone_164);
    pDef->SetRemap(1);
    if (!bDo_kill_phones_on_answer_67D6E8)
    {
        pDef->mImpactDefinitionIdx = objects::red_phone_answered_187;
        pDef->mNextDefinitionIdx = objects::red_phone_answered_187;
    }

    pDef = CloneDefinition(objects::red_phone_answered_187, objects::blue_phone_answered_185);
    pDef->SetRemap(1);
    pDef->mImpactDefinitionIdx = objects::red_phone_after_answer_186;
    pDef->mNextDefinitionIdx = objects::red_phone_after_answer_186;

    pDef = CloneDefinition(objects::red_phone_after_answer_186, objects::blue_phone_after_answer_184);
    pDef->SetRemap(1);
    pDef->mImpactDefinitionIdx = objects::red_phone_answered_187;
    pDef->mNextDefinitionIdx = objects::red_phone_177;

    pDef = CloneDefinition(objects::yellow_phone_not_ringing_178, objects::blue_phone_not_ringing_163);
    pDef->SetRemap(2);
    pDef->mImpactDefinitionIdx = objects::yellow_phone_179;
    pDef->mNextDefinitionIdx = objects::yellow_phone_179;

    pDef = CloneDefinition(objects::yellow_phone_179, objects::blue_phone_164);
    pDef->SetRemap(2);
    if (!bDo_kill_phones_on_answer_67D6E8)
    {
        pDef->mImpactDefinitionIdx = objects::yellow_phone_answered_189;
        pDef->mNextDefinitionIdx = objects::yellow_phone_answered_189;
    }
    pDef = CloneDefinition(objects::yellow_phone_answered_189, objects::blue_phone_answered_185);
    pDef->SetRemap(2);
    pDef->mImpactDefinitionIdx = objects::yellow_phone_after_answer_188;
    pDef->mNextDefinitionIdx = objects::yellow_phone_after_answer_188;

    pDef = CloneDefinition(objects::yellow_phone_after_answer_188, objects::blue_phone_after_answer_184);
    pDef->SetRemap(2);
    pDef->mImpactDefinitionIdx = objects::yellow_phone_answered_189;
    pDef->mNextDefinitionIdx = objects::yellow_phone_179;

    pDef = CloneDefinition(objects::green_phone_not_ringing_180, objects::blue_phone_not_ringing_163);
    pDef->SetRemap(0);
    pDef->mImpactDefinitionIdx = objects::green_phone_181;
    pDef->mNextDefinitionIdx = objects::green_phone_181;

    pDef = CloneDefinition(objects::green_phone_181, objects::blue_phone_164);
    pDef->SetRemap(0);
    if (!bDo_kill_phones_on_answer_67D6E8)
    {
        pDef->mImpactDefinitionIdx = objects::green_phone_answered_191;
        pDef->mNextDefinitionIdx = objects::green_phone_answered_191;
    }

    pDef = CloneDefinition(objects::green_phone_answered_191, objects::blue_phone_answered_185);
    pDef->SetRemap(0);
    pDef->mImpactDefinitionIdx = objects::green_phone_after_answer_190;
    pDef->mNextDefinitionIdx = objects::green_phone_after_answer_190;

    pDef = CloneDefinition(objects::green_phone_after_answer_190, objects::blue_phone_after_answer_184);
    pDef->SetRemap(0);
    pDef->mImpactDefinitionIdx = objects::green_phone_answered_191;
    pDef->mNextDefinitionIdx = objects::green_phone_181;

    pDef = CloneDefinition(objects::dead_phone_174, objects::blue_phone_not_ringing_163);
    pDef->mTouchBehavior = 0;

    pDef = CloneDefinition(objects::grenade_lying_still_182, objects::grenade_obj_183);
    pDef->mImpactDefinitionIdx = objects::grenade_obj_183;
    pDef->mBehaviorType = object_behavior_type::animated_2;
    pDef->mTouchBehavior = 2;

    pDef->mFriction = kFpZero_6FCE08;
    pDef->mSpeed = kFpZero_6FCE08;
    pDef->mCollisionResponse = 0;
    pDef->mZCollisionResponse = 0;
    pDef->mNextFrameMax = 8;
    pDef->OffsetBaseSpriteIdxAndSetNumFrames(pDef->mNumFrames - 1, 1);

    for(s32 def_idx = 200; def_idx - 200 <= 44; def_idx++)
    {
        pDef = CloneDefinition(def_idx - 0x88, def_idx);
        pDef->mImpactDefinitionIdx = def_idx;
        pDef->mBehaviorType = pDef->mBehaviorType == object_behavior_type::pickup_6 ? object_behavior_type::pickup_physics_7 : object_behavior_type::pickup_physics_animated_9;
        pDef->mFriction = -kFpPoint01_6FCDD8;
        pDef->mCollisionResponse = 1;
        pDef->mZCollisionResponse = 1;
        pDef->mInPoolList = 1;
        pDef->mUses3dMovement = 1;
        pDef->mCollisionBucketCategory = collision_bucket_category::sprite_grid_2_region_3;
        pDef->mMass = kFpPoint1_6F8FD8;
        pDef->mSpeed = kFpPoint02;
    }

    ClearColour1PixelsOfDefinitions287To293();
}

MATCH_FUNC(0x534270)
void ObjectDefinitions::ClearColour1PixelsOfDefinitions287To293()
{
    ClearColour1PixelsOfDefinitionSprite(objects::loonies_icon_287);
    ClearColour1PixelsOfDefinitionSprite(objects::yakuza_icon_288);
    ClearColour1PixelsOfDefinitionSprite(objects::zaibatsu_icon_289);
    ClearColour1PixelsOfDefinitionSprite(objects::redneck_icon_290);
    ClearColour1PixelsOfDefinitionSprite(objects::scientists_icon_291);
    ClearColour1PixelsOfDefinitionSprite(objects::krishna_icon_292);
    ClearColour1PixelsOfDefinitionSprite(objects::russian_mafia_icon_293);
}

MATCH_FUNC(0x5342d0)
void ObjectDefinitions::CacheDef112BaseSpriteIdx()
{
    mDef112BaseSpriteIdx = GetObjectDefinition(112)->mBaseSpriteIdx;
}

MATCH_FUNC(0x5342f0)
void ObjectDefinitions::ClearColour1PixelsOfDefinitionSprite(s32 idx)
{
    ObjectDefinition* pDef = GetObjectDefinition(idx);
    u16 sprite_idx = gGtx_0x106C_703DD4->GetSpriteTrueIndex(sprite_types_enum::code_obj1_4, pDef->mBaseSpriteIdx);
    sprite_index* psprite_index = gGtx_0x106C_703DD4->get_sprite_index(sprite_idx);
    psprite_index->ClearPixelsOfColour(1);
}

MATCH_FUNC(0x534330)
void ObjectDefinitions::InitDefinitions()
{
    CreateMapObjectDefinitions();
    ApplyMapObjectOverrides();
    CloneAnimatedDefinitions();
    CreateCodeObjectDefinitions();
    CloneRemappedDefinitions();
    CacheDef112BaseSpriteIdx();
}

MATCH_FUNC(0x534360)
ObjectDefinition* ObjectDefinitions::GetObjectDefinition(s32 idx) const
{
    return mDefinitionByIdx[idx];
}

MATCH_FUNC(0x534370)
ObjectDefinition* ObjectDefinitions::CloneDefinition(s32 dst_idx, s32 src_idx)
{
    ObjectDefinition* result = &mDefinitions[mNextIdx];
    mDefinitionByIdx[dst_idx] = result;
    ++mNextIdx;
    memcpy(result, mDefinitionByIdx[src_idx], sizeof(ObjectDefinition));
    result->mObjectIdx = dst_idx;
    return result;
}

MATCH_FUNC(0x5343c0)
ObjectDefinition* ObjectDefinitions::AllocDefinition(s32 idx)
{
    ObjectDefinition* result = &mDefinitions[mNextIdx];
    mDefinitionByIdx[idx] = result;
    ++mNextIdx;
    result->mObjectIdx = idx;
    return result;
}

MATCH_FUNC(0x5343f0)
ObjectDefinitions::ObjectDefinitions()
{
    for (s32 i = 0; i < GTA2_COUNTOF(mDefinitionByIdx); i++)
    {
        mDefinitionByIdx[i] = NULL;
    }
    mNextIdx = 0;
    mDef112BaseSpriteIdx = 99;
}