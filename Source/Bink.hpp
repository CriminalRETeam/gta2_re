#pragma once

#include "3rdParty/mss.h"
#include "Function.hpp"
#include <windows.h>

class Bink
{
  public:
    EXPORT static void __stdcall Reset_513210();

    EXPORT static void __stdcall Close1_513340();

    EXPORT static void __stdcall Close2_513390();

    EXPORT static void __stdcall sub_5136D0(s32* pNewWindowX, s32* pNewWindowY);

    EXPORT static BOOL sub_513770();

    EXPORT static BOOL sub_513760();

    EXPORT static void sub_513720();

    EXPORT static char_type sub_513240();

    EXPORT static char_type sub_513790();

    EXPORT static void __stdcall sub_5137A0(char_type a1);

    EXPORT static void __stdcall sub_5133E0(const char_type* a1, HDIGDRIVER a2);

    EXPORT static void __stdcall sub_5137B0(char_type a1);

    EXPORT static void __stdcall sub_513560(const char_type* pFileName, HDIGDRIVER a2);
};