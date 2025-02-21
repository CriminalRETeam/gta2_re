#pragma once

#include "Function.hpp"
#include <fstream>
#include <windows.h>

class ErrorLog : public std::fstream
{
public:
    ErrorLog& Write_4D9620(const char_type* pMsg);
};

EXPORT void __stdcall Error_SetName_4A0770(const char_type* pFileName);

EXPORT void FatalError_4A38C0(s32 Code, const char_type* pSourceFile, s32 lineNo, ...);

EXPORT void sub_4DA740();
EXPORT void __stdcall GBH_Graphis_DMA_Video_Free_5D9830();

EXPORT_VAR extern char_type gTmpBuffer_67C598[256]; 
EXPORT_VAR extern HWND gHwnd_707F04;
EXPORT_VAR extern char_type bDestroyed_6F5B70;
EXPORT_VAR extern char_type gErrStr_67C29C[256];
EXPORT_VAR extern char_type byte_67C3A8[256];

EXPORT_VAR extern ErrorLog gErrorLog_67C530;
