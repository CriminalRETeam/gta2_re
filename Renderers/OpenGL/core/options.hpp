// Optional enhancements, read from gta2gl.ini ([graphics]) next to the DLLs, overridden by env vars.
// With nothing set the renderer is the faithful one.
#pragma once

#include "renderer.hpp"
#include <string>

class OptionsFile
{
  public:
    // path: the ini file. Reads it at once.
    void Init(const std::string& path);
    // Re-reads the file when its modification time changed (at most once per second).
    void Poll(double nowSeconds);
    // Live A/B switch between the faithful renderer and the configured options.
    void ToggleFaithful();
    const RenderOptions& Effective() const
    {
        return mEffective;
    }

  private:
    void Load();
    void Update();

    std::string mPath;
    long long mMtime = -1;
    double mLastPoll = 0.0;
    bool mFaithful = false;
    RenderOptions mConfigured;
    RenderOptions mEffective;
};
