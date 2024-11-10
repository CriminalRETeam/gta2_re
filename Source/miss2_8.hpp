#pragma once

#include "types.hpp"

// TODO: move
class Frismo_C
{
public:
    s32 field_0;
    s16 field_4;
    s16 field_6;
    Frismo_C* field_8_next;
};
GTA2_ASSERT_SIZEOF_ALWAYS(Frismo_C, 0xC)

struct Frismo_25C
{
    Frismo_C * field_0;
    Frismo_C field_4[50];
};
GTA2_ASSERT_SIZEOF_ALWAYS(Frismo_25C, 0x25C)

extern Frismo_25C* gFrismo_25C_6F8068;

class miss2_8
{
public:
    miss2_8(); // 503120
    ~miss2_8(); // 503130

    void add_503160(Frismo_C* a2);
    Frismo_C* remove_503180();
    void remove_5031E0(u8 count);

    Frismo_C* field_0_current;
    u8 field_4_count;
    u8 field_5_pad;
    u8 field_6_pad;
    u8 field_7_pad;
};
GTA2_ASSERT_SIZEOF_ALWAYS(miss2_8, 0x8)