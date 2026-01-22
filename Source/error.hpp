#pragma once

#include "Function.hpp"
#include <windows.h>
#include <FSTREAM.H>

class ErrorLog
{
  public:
    EXPORT void sub_4D9470(const char_type* path, s32 a3);
    EXPORT ErrorLog(const char* FileName, int a3);
    EXPORT void Write_4D9620(const char_type* pMsg);
    EXPORT void Write_Log_4D9650(const char_type* buffer);

    EXPORT void log_timestamp_4D9540();

    typedef void(__cdecl* TLogLineCallback)(void*);
    EXPORT void log_on_line_written_4D9670(TLogLineCallback pCallBack);

    EXPORT void log_intro_4D95A0();

    ErrorLog()
    {
    }

    // Note: Has to use the old/classic ofstream, not the one from the std namespace
    // although clang-cl preprocessing, bad build options or something is causing field 0x3C to
    // end up at the wrong offset

    // HACK: Isn't a pointer but just done so we can use the var in Write and some other funcs for now
    struct fake_ofstream
    {
      char buffer[0x3C];
    };
    fake_ofstream field_0_ofstr; // Crashes standalone
    u8* field_3C_pLen;
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
