#pragma once

#include "types.hpp"

// Logging and file helpers on raw Win32 handles. The bot avoids CRT stdio on purpose: the FILE
// table and its buffers are game memory that a Snapshot restore rewinds.
void BotLog_Open(const char* out_dir); // creates out_dir and out_dir\bot_log.txt
void BotLog(const char* fmt, ...);
void BotLog_Close();

void BotPath_Join(char* out, u32 out_size, const char* dir, const char* name);
bool BotFile_WriteAll(const char* path, const void* data, u32 size);
bool BotFile_AppendText(const char* path, const char* text);
u8* BotFile_ReadAll(const char* path, u32* out_size); // bot heap, NUL terminated; NULL if missing

// A text file kept open for per frame writes (traces, hash logs)
class BotTextFile
{
  public:
    BotTextFile();
    ~BotTextFile();
    bool Open(const char* path);
    void Printf(const char* fmt, ...);
    void Close();

  private:
    void* mHandle;
};
