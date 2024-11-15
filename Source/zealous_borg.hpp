#pragma once

#include "Function.hpp"

class zealous_borg
{
public:
    zealous_borg();  // 0x484ED0
    ~zealous_borg(); // 0x484EE0
    void sub_484EF0();
    void sub_484F20();
    void sub_484F30();
    void sub_484F50(int a2);
    void sub_484FA0(int a2);
    void sub_484FB0(int amount);

private:
    u32 field_0[10];
    u8 field_28[10];
    s16 field_32;
    s32 field_34;
    s32 field_38;
};
