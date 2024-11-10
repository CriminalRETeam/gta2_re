#pragma once

#include "types.hpp"

class Frismo_C;

class miss2_8
{
public:
    miss2_8(); // 503120
    ~miss2_8(); // 503130

    Frismo_C* sub_503160(Frismo_C* a2);
    Frismo_C* sub_503180();
    void sub_5031E0(u8 a2);

    Frismo_C* field_0;
    char_type field_4;
    char_type field_5;
    char_type field_6;
    char_type field_7;
};
