// Native (Linux) test of core/options.cpp: ini parsing, env overrides, reload on change, A/B toggle.
#include "../core/options.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int gFailures;
#define CHECK(cond)                                                \
    do                                                             \
    {                                                              \
        if (!(cond))                                               \
        {                                                          \
            printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
            gFailures++;                                           \
        }                                                          \
    } while (0)

static void Write(const char* path, const char* text)
{
    FILE* f = fopen(path, "w");
    fputs(text, f);
    fclose(f);
}

int main()
{
    const char* path = "options_test.ini";
    unsetenv("GTA2GL_TRUECOLOR");
    unsetenv("GTA2GL_BLOOM");
    unsetenv("GTA2GL_PPL");

    // Missing file: faithful.
    remove(path);
    OptionsFile o;
    o.Init(path);
    CHECK(!o.Effective().mTrueColour && o.Effective().mBloom == 0.0f && !o.Effective().mPerPixelLighting);

    Write(path,
          "; comment\n[other]\nbloom = 1\n[graphics]\ntruecolor = 1   ; trailing comment\n bloom=0.25\nper_pixel_lighting = 1\n"
          "unknown = 3\n");
    OptionsFile p;
    p.Init(path);
    CHECK(p.Effective().mTrueColour);
    CHECK(p.Effective().mBloom == 0.25f);
    CHECK(p.Effective().mPerPixelLighting);
    const u32 gen = p.Effective().mGeneration;

    // A/B toggle: faithful, then back.
    p.ToggleFaithful();
    CHECK(!p.Effective().mTrueColour && p.Effective().mBloom == 0.0f && !p.Effective().mPerPixelLighting);
    CHECK(p.Effective().mGeneration != gen);
    p.ToggleFaithful();
    CHECK(p.Effective().mTrueColour && p.Effective().mBloom == 0.25f);

    // Out of range bloom is clamped; the file is re-read when it changes (mtime has 1 s resolution).
    sleep(1);
    Write(path, "[graphics]\nbloom = 7\n");
    p.Poll(1000.0);
    CHECK(!p.Effective().mTrueColour && p.Effective().mBloom == 1.0f && !p.Effective().mPerPixelLighting);

    // Environment variables win.
    setenv("GTA2GL_BLOOM", "0.5", 1);
    setenv("GTA2GL_PPL", "1", 1);
    OptionsFile e;
    e.Init(path);
    CHECK(e.Effective().mBloom == 0.5f && e.Effective().mPerPixelLighting);

    remove(path);
    printf("%s (%d failures)\n", gFailures ? "FAILED" : "OK", gFailures);
    return gFailures ? 1 : 0;
}
