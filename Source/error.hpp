#pragma once

#include "Function.hpp"
#include <fstream>
#include <windows.h>

// TODO: Base type is too big
class ErrorLog /*: public std::ofstream*/
{
public:
    ErrorLog& Write_4D9620(const char_type* pMsg);
    void Write_Log_4D9650(const char_type* buffer);
};

EXPORT void __stdcall Error_SetName_4A0770(const char_type* pFileName);

EXPORT void FatalError_4A38C0(s32 Code, const char_type* pSourceFile, s32 lineNo, ...);
EXPORT void __stdcall sub_4DA740();

EXPORT void __stdcall GBH_Graphis_DMA_Video_Free_5D9830();

EXTERN_GLOBAL(char_type, gTmpBuffer_67C598[256]);
EXTERN_GLOBAL(HWND, gHwnd_707F04);
EXTERN_GLOBAL(char_type, bDestroyed_6F5B70);
EXTERN_GLOBAL(char_type, gErrStr_67C29C[256]);
EXTERN_GLOBAL(char_type, byte_67C3A8[256]);

EXTERN_GLOBAL(ErrorLog, gErrorLog_67C530);
EXTERN_GLOBAL(ErrorLog, gMiss2Log_6F7698);
extern ErrorLog gFile_67C530; //EXTERN_GLOBAL(ErrorLog, gFile_67C530);
