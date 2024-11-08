#pragma once

#include "types.hpp"

class infallible_turing
{
 public:
    s32 field_0_object_type;
    char_type field_4;
    char_type field_5;
    char_type field_6;
    char_type field_7;
    s32 field_8;
    
    // Type depends on what field_0_object_type is
    void *field_C_pAny;

    void release_40EF20();
};
