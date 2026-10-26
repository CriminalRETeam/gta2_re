#include "BotLog.hpp"
#include "BotHeap.hpp"
#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static HANDLE gLogFile = INVALID_HANDLE_VALUE;

static void WriteHandle(HANDLE h, const char* text, u32 len)
{
    DWORD written = 0;
    if (h != INVALID_HANDLE_VALUE && h != NULL)
    {
        WriteFile(h, text, len, &written, NULL);
    }
}

void BotPath_Join(char* out, u32 out_size, const char* dir, const char* name)
{
    if (!dir || !dir[0])
    {
        _snprintf(out, out_size, "%s", name);
    }
    else
    {
        _snprintf(out, out_size, "%s\\%s", dir, name);
    }
    out[out_size - 1] = 0;
}

void BotLog_Open(const char* out_dir)
{
    char path[MAX_PATH];
    if (out_dir && out_dir[0])
    {
        CreateDirectoryA(out_dir, NULL);
    }
    BotPath_Join(path, sizeof(path), out_dir, "bot_log.txt");
    gLogFile = CreateFileA(path, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
}

void BotLog(const char* fmt, ...)
{
    char buf[2048];
    va_list args;
    va_start(args, fmt);
    int len = _vsnprintf(buf, sizeof(buf) - 2, fmt, args);
    va_end(args);
    if (len < 0)
    {
        len = sizeof(buf) - 2;
    }
    buf[len++] = '\n';
    buf[len] = 0;
    WriteHandle(gLogFile, buf, len);
    WriteHandle(GetStdHandle(STD_OUTPUT_HANDLE), buf, len);
    OutputDebugStringA(buf);
}

namespace
{
const u32 kDecisionRing = 50;
const u32 kDecisionLen = 200;
char gDecisions[kDecisionRing][kDecisionLen]; // static: never restored, the exe .data is the game's
u32 gDecisionCount = 0;
} // namespace

void BotDecision(const char* fmt, ...)
{
    char* slot = gDecisions[gDecisionCount % kDecisionRing];
    va_list args;
    va_start(args, fmt);
    int len = _vsnprintf(slot, kDecisionLen - 1, fmt, args);
    va_end(args);
    if (len < 0)
    {
        len = kDecisionLen - 1;
    }
    slot[len] = 0;
    gDecisionCount++;
    BotLog("decide: %s", slot);
}

u32 BotDecision_Count()
{
    return gDecisionCount;
}

const char* BotDecision_At(u32 i)
{
    if (i >= gDecisionCount || i + kDecisionRing < gDecisionCount)
    {
        return "";
    }
    return gDecisions[i % kDecisionRing];
}

void BotLog_Close()
{
    if (gLogFile != INVALID_HANDLE_VALUE)
    {
        CloseHandle(gLogFile);
        gLogFile = INVALID_HANDLE_VALUE;
    }
}

bool BotFile_WriteAll(const char* path, const void* data, u32 size)
{
    HANDLE h = CreateFileA(path, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE)
    {
        return false;
    }
    DWORD written = 0;
    BOOL ok = WriteFile(h, data, size, &written, NULL);
    CloseHandle(h);
    return ok && written == size;
}

bool BotFile_AppendText(const char* path, const char* text)
{
    HANDLE h = CreateFileA(path, FILE_APPEND_DATA, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE)
    {
        return false;
    }
    DWORD written = 0;
    BOOL ok = WriteFile(h, text, strlen(text), &written, NULL);
    CloseHandle(h);
    return ok == TRUE;
}

u8* BotFile_ReadAll(const char* path, u32* out_size)
{
    HANDLE h = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE)
    {
        return NULL;
    }
    DWORD size = GetFileSize(h, NULL);
    u8* data = static_cast<u8*>(BotHeap_Alloc(size + 1));
    DWORD read = 0;
    ReadFile(h, data, size, &read, NULL);
    CloseHandle(h);
    data[read] = 0;
    if (out_size)
    {
        *out_size = read;
    }
    return data;
}

BotTextFile::BotTextFile()
    : mHandle(INVALID_HANDLE_VALUE)
{
}

BotTextFile::~BotTextFile()
{
    Close();
}

bool BotTextFile::Open(const char* path)
{
    Close();
    mHandle = CreateFileA(path, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    return mHandle != INVALID_HANDLE_VALUE;
}

void BotTextFile::Printf(const char* fmt, ...)
{
    char buf[1024];
    va_list args;
    va_start(args, fmt);
    int len = _vsnprintf(buf, sizeof(buf) - 1, fmt, args);
    va_end(args);
    if (len < 0)
    {
        len = sizeof(buf) - 1;
    }
    WriteHandle(mHandle, buf, len);
}

void BotTextFile::Close()
{
    if (mHandle != INVALID_HANDLE_VALUE)
    {
        CloseHandle(mHandle);
        mHandle = INVALID_HANDLE_VALUE;
    }
}
