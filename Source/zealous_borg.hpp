#pragma once

#include "Function.hpp"

class zealous_borg
{
  public:
    EXPORT zealous_borg(); // 0x484ED0
    EXPORT ~zealous_borg(); // 0x484EE0
    EXPORT void sub_484EF0();
    EXPORT void sub_484F20();
    EXPORT void sub_484F30();
    EXPORT void report_crime(int crime_type);
    EXPORT void sub_484FA0(int a2);
    EXPORT void sub_484FB0(int amount);

  public:
    u32 field_0[10];
    u8 field_28[10];
    s16 field_32;
    s32 field_34;
    s32 field_38;
};
