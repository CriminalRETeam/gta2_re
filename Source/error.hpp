#pragma once

#include "types.hpp"

EXPORT void __stdcall Error_SetName_4A0770(const char_type *pFileName);

EXPORT void FatalError_4A38C0(s32 Code, const char_type *pSourceFile, s32 lineNo, ...);

EXPORT void sub_4DA740();
EXPORT void __stdcall GBH_Graphis_DMA_Video_Free_5D9830();
