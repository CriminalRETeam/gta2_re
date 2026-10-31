#pragma once

#include "Function.hpp"
#include <windows.h>

#if defined(__clang__) || (_MSC_VER <= 1200)
#include <FSTREAM.H>
#define ostream_type ostream
#else
#include <fstream>
#define ostream_type std::ostream
#endif

class ErrorLog
{
  public:
    EXPORT void Open(const char_type* path, s32 truncate_mode);
    EXPORT ErrorLog(const char_type* FileName, s32 truncate_mode);
    EXPORT void Write(const char_type* pMsg);
    EXPORT void Write_Log(const char_type* buffer);

    EXPORT void log_timestamp();

    typedef void(__cdecl* TLogLineCallback)(void*);
    EXPORT void* log_on_line_written(TLogLineCallback pCallBack);

    EXPORT void log_intro();

    ErrorLog()
    {
    }

    EXPORT ~ErrorLog();

    // Note: Has to use the old/classic ofstream, not the one from the std namespace
    // although clang-cl preprocessing, bad build options or something is causing field 0x3C to
    // end up at the wrong offset

#if !defined(__clang__) && (_MSC_VER <= 1200)
    // VC6: the real classic ofstream (0x3C bytes, checked with sizeof), so the constructor emits
    // the original's ??0ofstream@@QAE@XZ call and EH frame (ErrorLog::ErrorLog 0x4D94E0)
    ofstream mOfstr; // 0x0
#else
    // Only keeps the 0x3C bytes (the globals registry checks sizes); never used as a stream
    struct fake_ofstream
    {
      char buffer[0x3C];
    };
    fake_ofstream mOfstr; // 0x0
#endif
    u8* mpLen; // 0x3C
};

EXPORT void __stdcall Error_SetName(const char_type* pFileName);

EXPORT void FatalError(s32 Code, const char_type* pSourceFile, s32 lineNo, ...);
EXPORT void __stdcall FatalDXError(HRESULT hr, const char_type* pSourceFile, s32 lineNo);
EXPORT void __stdcall ShutdownGame();

EXPORT void __stdcall GBH_Graphis_DMA_Video_Free();

EXTERN_GLOBAL(char_type, gTmpBuffer[256]);
EXTERN_GLOBAL(HWND, gHwnd);
EXTERN_GLOBAL(char_type, bDestroyed);
EXTERN_GLOBAL(char_type, gErrStr[256]);
EXTERN_GLOBAL(char_type, gErrStr2[256]);

EXTERN_GLOBAL(ErrorLog, gWarningLog);
EXTERN_GLOBAL(ErrorLog, gMiss2Log);
