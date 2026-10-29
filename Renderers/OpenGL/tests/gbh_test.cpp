// Native (Linux) test of core/gbh_api.cpp texture invalidation with a recording backend.
// Mirrors how the game updates textures (TextureCache in Source/): rolling score digits,
// palette changes, car damage size changes, the shared texture and never drawn tiles.
#include "../core/gbh_api.hpp"
#include "../core/renderer.hpp"
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <vector>

extern "C" u32 gbh_InitDLL(SVideo*);
extern "C" u32 gbh_RegisterPalette(s32, u32*);
extern "C" STexture* gbh_RegisterTexture(s32, s32, u8*, s32, char);
extern "C" STexture* gbh_LockTexture(STexture*);
extern "C" STexture* gbh_UnlockTexture(STexture*);
extern "C" void gbh_AssignPalette(STexture*, s32);
extern "C" void gbh_DrawQuad(s32, STexture*, Vert*, s32);
extern "C" void gbh_FreeTexture(STexture*);
extern "C" void gbh_SetWindow(f32, f32, f32, f32);

struct RTexture
{
    int mId;
};

class RecordingRenderer : public IRenderer
{
  public:
    struct Upload
    {
        RTexture* mTex;
        s32 mW, mH;
        std::vector<u32> mPixels;
    };
    std::vector<Upload> mUploads;
    int mNextId = 1;

    RTexture* CreateTexture()
    {
        return new RTexture {mNextId++};
    }
    void UploadTexture(RTexture* t, s32 w, s32 h, const u32* p)
    {
        mUploads.push_back(Upload {t, w, h, std::vector<u32>(p, p + w * h)});
    }
    void FreeTexture(RTexture* t)
    {
        delete t;
    }
    void BeginScene()
    {
    }
    void EndScene()
    {
    }
    void DrawTriangles(const RVertex*, s32, RTexture*, u32)
    {
    }
    void SetClipRect(f32, f32, f32, f32)
    {
    }
    u32 GetDrawCallCount() const
    {
        return 0;
    }
    const RenderOptions& Options() const
    {
        return mOptions;
    }
    void SetLights(const RLight*, s32, f32)
    {
    }
    RenderOptions mOptions;
};

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

static RecordingRenderer gRec;

static void Draw(STexture* t)
{
    Vert v[8];
    memset(v, 0, sizeof(v));
    for (int i = 0; i < 4; i++)
    {
        v[i].x = 10.0f + (i == 1 || i == 2) * 10.0f;
        v[i].y = 10.0f + (i >= 2) * 10.0f;
        v[i].z = 1.0f;
    }
    gbh_DrawQuad(0x10000 | 0x20000 | 0x80, t, v, 255);
}

// Expected RGBA of palette `pal` entry `idx`: the test draws with the alpha flag, ARGB4444 texels.
static u32 Expect(const u32* palPage, int pal, int idx)
{
    const u32 c = palPage[idx * 64 + pal];
    const u32 r = ((c >> 16) & 0xFF) >> 4, g = ((c >> 8) & 0xFF) >> 4, b = (c & 0xFF) >> 4;
    return r * 17 | (g * 17 << 8) | (b * 17 << 16) | (idx ? 0xFF000000u : 0);
}

// Uploads are square cache slots (see SlotSize in gbh_api.cpp); the texture sits at the top left.
static bool Matches(const RecordingRenderer::Upload& up, const u8* pixels, const u32* palPage, int pal, int w, int h)
{
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            if (up.mPixels[y * up.mW + x] != Expect(palPage, pal, pixels[y * 256 + x]))
            {
                return false;
            }
        }
    }
    return true;
}

int main()
{
    GLVideoHeader header;
    memset(&header, 0, sizeof(header));
    header.mMagic = kGLVideoMagic;
    header.mRenderer = &gRec;
    CHECK(gbh_InitDLL(&header.mVideo) == 1);
    gbh_SetWindow(0, 0, 639, 479);

    // Two palettes in one page (columns of 64 dwords), distinct colours.
    static u32 palPage[64 * 256];
    for (int i = 0; i < 256; i++)
    {
        palPage[i * 64 + 0] = (i << 16) | ((255 - i) << 8) | 0x10;
        palPage[i * 64 + 1] = (0x20 << 16) | (i << 8) | (255 - i);
    }
    gbh_RegisterPalette(0, &palPage[0]);
    gbh_RegisterPalette(1, &palPage[1]);

    // A 256 wide page: rows of digits like the score font (each row a different pattern).
    static u8 page[256 * 256];
    for (int y = 0; y < 256; y++)
    {
        for (int x = 0; x < 256; x++)
        {
            page[y * 256 + x] = (u8)((x * 7 + y * 13) % 251 + 1);
        }
    }

    // 1. First draw uploads the texture, a second draw does not.
    STexture* digit = gbh_RegisterTexture(11, 20, page, 0, 0);
    Draw(digit);
    CHECK(gRec.mUploads.size() == 1);
    CHECK(gRec.mUploads.back().mW == 32 && gRec.mUploads.back().mH == 32);
    CHECK(Matches(gRec.mUploads.back(), page, palPage, 0, 11, 20));
    Draw(digit);
    CHECK(gRec.mUploads.size() == 1);

    // 2. Rolling digit (GetDigitTexture_5B95F0): pixel pointer and height change between lock and unlock.
    gbh_LockTexture(digit);
    digit->field_14_original_pixel_data_ptr = &page[256 * 37];
    digit->field_10_height = 9;
    gbh_UnlockTexture(digit);
    Draw(digit);
    CHECK(gRec.mUploads.size() == 2);
    CHECK(gRec.mUploads.back().mW == 16 && gRec.mUploads.back().mH == 16);
    CHECK(Matches(gRec.mUploads.back(), &page[256 * 37], palPage, 0, 11, 9));

    // 3. Palette swap (SetPal_5B9660): AssignPalette between lock and unlock.
    gbh_LockTexture(digit);
    gbh_AssignPalette(digit, 1);
    gbh_UnlockTexture(digit);
    Draw(digit);
    CHECK(gRec.mUploads.size() == 3);
    CHECK(Matches(gRec.mUploads.back(), &page[256 * 37], palPage, 1, 11, 9));

    // 4. AssignPalette alone locks and unlocks by itself.
    gbh_AssignPalette(digit, 0);
    Draw(digit);
    CHECK(gRec.mUploads.size() == 4);
    CHECK(Matches(gRec.mUploads.back(), &page[256 * 37], palPage, 0, 11, 9));

    // 5. Car damage (SetCarDamageTextureSizeAndPalette_5B96B0): size and palette change.
    STexture* damage = gbh_RegisterTexture(64, 64, page, 0, 0);
    Draw(damage);
    gbh_LockTexture(damage);
    damage->field_E_width = 40;
    damage->field_10_height = 30;
    gbh_AssignPalette(damage, 1);
    gbh_UnlockTexture(damage);
    Draw(damage);
    CHECK(gRec.mUploads.size() == 6);
    CHECK(gRec.mUploads.back().mW == 64 && gRec.mUploads.back().mH == 64);
    CHECK(Matches(gRec.mUploads.back(), page, palPage, 1, 40, 30));

    // 6. Shared texture registered without pixels: drawing it before it gets data uploads nothing.
    STexture* shared = gbh_RegisterTexture(128, 128, 0, 0, 0);
    const size_t before = gRec.mUploads.size();
    Draw(shared);
    CHECK(gRec.mUploads.size() == before);
    gbh_LockTexture(shared);
    shared->field_14_original_pixel_data_ptr = &page[257 * 3];
    shared->field_E_width = 20;
    shared->field_10_height = 18;
    gbh_AssignPalette(shared, 1);
    gbh_UnlockTexture(shared);
    Draw(shared);
    CHECK(gRec.mUploads.size() == before + 1);
    CHECK(Matches(gRec.mUploads.back(), &page[257 * 3], palPage, 1, 20, 18));

    // 7. Tiles registered past the end of the data are never read unless drawn.
    u8* guard = (u8*)mmap(0, 4096, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    STexture* bogus = gbh_RegisterTexture(64, 64, guard, 0, 0);
    gbh_FreeTexture(bogus);

    gbh_FreeTexture(digit);
    gbh_FreeTexture(damage);
    gbh_FreeTexture(shared);
    printf("%s (%d failures, %zu uploads)\n", gFailures ? "FAILED" : "OK", gFailures, gRec.mUploads.size());
    return gFailures ? 1 : 0;
}
