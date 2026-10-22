#include "cmdlog.hpp"
#include <string.h>

void CmdLog::Init(const char* prefix)
{
    strncpy(mPrefix, prefix, sizeof(mPrefix) - 1);
    mPrefix[sizeof(mPrefix) - 1] = 0;
    mFrames.InitFromEnv("GTA2GL_CMDLOG", prefix);
}

void CmdLog::Update(u32 flipsDone)
{
    const u32 frame = flipsDone + 1;
    if (mFile && mOpenFrame != frame)
    {
        fclose(mFile);
        mFile = 0;
    }
    if (!mFile && mFrames.WantFrame(frame))
    {
        std::string path = mFrames.PathFor(frame);
        path.replace(path.size() - 4, 4, ".txt");
        mFile = fopen(path.c_str(), "w");
        mOpenFrame = frame;
    }
}

void CmdLog::Call(const char* func, u32 flags, u32 texId, s32 texW, s32 texH, u32 colour)
{
    if (mFile)
    {
        fprintf(mFile, "G %s f=%x tex=%u %dx%d c=%u\n", func, flags, texId, texW, texH, colour);
    }
}

void CmdLog::Prim(const char* type, s32 count, const u32 state[5], u32 texKey, s32 texW, s32 texH)
{
    if (mFile)
    {
        fprintf(mFile,
                "P %s %d blend=%u,%u,%u filter=%u,%u tex=%x %dx%d\n",
                type,
                count,
                state[0],
                state[1],
                state[2],
                state[3],
                state[4],
                texKey,
                texW,
                texH);
    }
}

void CmdLog::Vertex(f32 x, f32 y, f32 z, f32 rhw, u32 diffuse, u32 specular, f32 u, f32 v)
{
    if (mFile)
    {
        fprintf(mFile, "V %.9g %.9g %.9g %.9g %08x %08x %.9g %.9g\n", x, y, z, rhw, diffuse, specular, u, v);
    }
}

void CmdLog::Note(const char* text)
{
    if (mFile)
    {
        fprintf(mFile, "N %s\n", text);
    }
}
