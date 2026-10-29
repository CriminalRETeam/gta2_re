// Frame capture to BMP, driven by GTA2GL_CAPTURE=<dir>,<frame>,<first>-<last>,...
#pragma once

#include "gta2_api.hpp"
#include <string>
#include <vector>

class FrameCapture
{
  public:
    // Reads the env var once. prefix names the files: <dir>/<prefix>_<frame>.bmp
    void Init(const char* prefix)
    {
        InitFromEnv("GTA2GL_CAPTURE", prefix);
    }
    void InitFromEnv(const char* envName, const char* prefix);
    bool Enabled() const
    {
        return !mFrames.empty();
    }
    bool WantFrame(u32 frame) const;
    std::string PathFor(u32 frame) const;

    // rgba: width*height pixels, top row first.
    static bool WriteBmp(const std::string& path, const u8* rgba, s32 width, s32 height, bool bottomUp);

  private:
    std::string mDir;
    std::string mPrefix;
    std::vector<u32> mFrames;
};
