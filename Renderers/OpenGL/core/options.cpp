#include "options.hpp"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

namespace
{
std::string Trim(const std::string& s)
{
    size_t a = 0, b = s.size();
    while (a < b && isspace((unsigned char)s[a]))
    {
        a++;
    }
    while (b > a && isspace((unsigned char)s[b - 1]))
    {
        b--;
    }
    return s.substr(a, b - a);
}

f32 Clamp01(f32 v)
{
    return v < 0.0f ? 0.0f : (v > 1.0f ? 1.0f : v);
}
} // namespace

void OptionsFile::Init(const std::string& path)
{
    mPath = path;
    Load();
}

void OptionsFile::Load()
{
    RenderOptions o;
    struct stat st;
    mMtime = stat(mPath.c_str(), &st) == 0 ? (long long)st.st_mtime : -1;
    FILE* f = mMtime >= 0 ? fopen(mPath.c_str(), "r") : 0;
    if (f)
    {
        char line[256];
        bool inGraphics = false;
        while (fgets(line, sizeof(line), f))
        {
            std::string s = Trim(line);
            if (s.empty() || s[0] == ';' || s[0] == '#')
            {
                continue;
            }
            if (s[0] == '[')
            {
                inGraphics = s == "[graphics]";
                continue;
            }
            const size_t eq = s.find('=');
            if (!inGraphics || eq == std::string::npos)
            {
                continue;
            }
            const std::string key = Trim(s.substr(0, eq));
            const f32 value = (f32)atof(Trim(s.substr(eq + 1)).c_str());
            if (key == "truecolor")
            {
                o.mTrueColour = value != 0.0f;
            }
            else if (key == "bloom")
            {
                o.mBloom = Clamp01(value);
            }
            else if (key == "per_pixel_lighting")
            {
                o.mPerPixelLighting = value != 0.0f;
            }
        }
        fclose(f);
    }
    // Environment variables win (automated runs).
    if (const char* e = getenv("GTA2GL_TRUECOLOR"))
    {
        o.mTrueColour = atoi(e) != 0;
    }
    if (const char* e = getenv("GTA2GL_BLOOM"))
    {
        o.mBloom = Clamp01((f32)atof(e));
    }
    if (const char* e = getenv("GTA2GL_PPL"))
    {
        o.mPerPixelLighting = atoi(e) != 0;
    }
    mConfigured = o;
    Update();
}

void OptionsFile::Poll(double nowSeconds)
{
    if (nowSeconds - mLastPoll < 1.0)
    {
        return;
    }
    mLastPoll = nowSeconds;
    struct stat st;
    const long long mtime = stat(mPath.c_str(), &st) == 0 ? (long long)st.st_mtime : -1;
    if (mtime != mMtime)
    {
        Load();
    }
}

void OptionsFile::ToggleFaithful()
{
    mFaithful = !mFaithful;
    Update();
}

void OptionsFile::Update()
{
    RenderOptions next = mFaithful ? RenderOptions() : mConfigured;
    next.mGeneration = mEffective.mGeneration;
    if (next.mTrueColour != mEffective.mTrueColour || next.mBloom != mEffective.mBloom ||
        next.mPerPixelLighting != mEffective.mPerPixelLighting)
    {
        next.mGeneration++;
    }
    mEffective = next;
}
