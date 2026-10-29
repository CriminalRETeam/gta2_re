#include "vid_system.hpp"
#include "../platform/gl_context.hpp"
#include <algorithm>
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const u16 VidSystem::kSurfaceKey;
const s32 VidSystem::kWindowedModeId;

namespace
{
double Now()
{
    return std::chrono::duration<double>(std::chrono::steady_clock::now().time_since_epoch()).count();
}

const s32 kModeSizes[][2] = {{640, 480}, {800, 600}, {1024, 768}, {1280, 960}, {1280, 1024}, {1600, 1200}, {1440, 1080}, {1920, 1080}};

IGLContext* gLoaderContext;
void* LoadGL(const char* name)
{
    return gLoaderContext->GetProcAddress(name);
}
} // namespace

VidSystem::VidSystem() :
    mContext(0), mEnumIdx(0), mEnumBpp(0), mSurfaceW(0), mSurfaceH(0), mSurfaceGrabbed(false), mFrame(0), mStatsInterval(0), mStatsStart(0),
    mPresentSeconds(0), mWaitSeconds(0), mStatsDraws(0), mStatsUploads(0), mStatsVerts(0)
{
    memset(&mHeader, 0, sizeof(mHeader));
    mHeader.mMagic = kGLVideoMagic;
    mHeader.mRenderer = &mRenderer;
    mHeader.mFlipCounter = &mFrame;
    memset(&mDevice, 0, sizeof(mDevice));
    mDevice.field_0_id = 1;
    mDevice.field_4_flags = 1;
    mDevice.field_8_driver_name = (char*)"opengl";
    mDevice.field_C_driver_desc = (char*)"OpenGL 3.3";

    for (u32 i = 0; i < sizeof(kModeSizes) / sizeof(kModeSizes[0]); i++)
    {
        SDisplayMode m;
        memset(&m, 0, sizeof(m));
        m.field_0_display_mode_idx = i + 1;
        m.field_4_deviceId = 1;
        m.field_8_width = kModeSizes[i][0];
        m.field_C_height = kModeSizes[i][1];
        m.field_10_pitch = kModeSizes[i][0] * 2;
        m.field_14_rgb_bit_count = 16;
        mModes.push_back(m);
    }
    for (size_t i = 0; i + 1 < mModes.size(); i++)
    {
        mModes[i].field_38_pnext = &mModes[i + 1];
    }
}

VidSystem::~VidSystem()
{
    if (mContext)
    {
        mContext->MakeCurrent();
        mRenderer.Shutdown();
        delete mContext;
    }
}

VidSystem* VidSystem::FromVideo(SVideo* pVideo)
{
    GLVideoHeader* pHeader = reinterpret_cast<GLVideoHeader*>(pVideo);
    if (!pHeader || pHeader->mMagic != kGLVideoMagic)
    {
        return 0;
    }
    return reinterpret_cast<VidSystem*>(pHeader);
}

void VidSystem::Init(void* hInstance, u16 flags, void* pVidFuncs, const char* iniPath)
{
    SVideo& v = mHeader.mVideo;
    v.field_0 = 1;
    v.field_4_flags = (flags & 0x40) | 0x200;
    v.field_7C_self_dll_handle = hInstance;
    v.field_84_from_initDLL = pVidFuncs;
    v.field_14_display_mode_count_2_q = 1;
    v.field_18_num_guids = 1;
    v.field_1C_num_display_modes = (u32)mModes.size();
    v.field_24_head_ptr = &mModes[0];
    v.field_28_display_mode_array = &mModes[0];
    v.field_2C_device_info_head_ptr = &mDevice;
    v.field_30_device_info_last = &mDevice;
    v.field_34_active_device_id = 1;
    // RGB565: bit counts and shifts as the Bink setup reads them.
    v.field_5C = 5;
    v.field_60_g = 5;
    v.field_64_r = 6;
    v.field_68_b = 11;
    v.field_6C = 5;
    v.field_70 = 0;
    v.field_10_found_rgb_bit_count = 16;
    mCapture.Init("gl");
    const char* pStats = getenv("GTA2GL_STATS");
    mStatsInterval = pStats ? (u32)atoi(pStats) : 0;
    mOptions.Init(iniPath ? iniPath : "gta2gl.ini");
    mRenderer.SetOptions(mOptions.Effective());
    if (const char* pToggle = getenv("GTA2GL_TOGGLE_AT"))
    {
        for (const char* p = pToggle; p && *p; p = strchr(p, ','), p = p ? p + 1 : 0)
        {
            mToggleFrames.push_back((u32)strtoul(p, 0, 10));
        }
    }
}

s32 VidSystem::CheckMode(s32 width, s32 height, s32 bpp)
{
    for (size_t i = 0; i < mModes.size(); i++)
    {
        if ((s32)mModes[i].field_8_width == width && (s32)mModes[i].field_C_height == height &&
            (bpp == 0 || (s32)mModes[i].field_14_rgb_bit_count == bpp))
        {
            return mModes[i].field_0_display_mode_idx;
        }
    }
    return 0;
}

SDisplayMode* VidSystem::FindMode(s32 modeId)
{
    for (size_t i = 0; i < mModes.size(); i++)
    {
        if ((s32)mModes[i].field_0_display_mode_idx == modeId)
        {
            return &mModes[i];
        }
    }
    return 0;
}

s32 VidSystem::FindFirstMode(s32 bppFilter)
{
    mEnumIdx = 0;
    mEnumBpp = bppFilter;
    return FindNextMode();
}

s32 VidSystem::FindNextMode()
{
    while (mEnumIdx < (s32)mModes.size())
    {
        SDisplayMode& m = mModes[mEnumIdx++];
        if (mEnumBpp == 0 || (s32)m.field_14_rgb_bit_count == mEnumBpp)
        {
            mHeader.mVideo.field_3C_current_enum_ptr = &m;
            mHeader.mVideo.field_8_found_width = m.field_8_width;
            mHeader.mVideo.field_C_found_height = m.field_C_height;
            mHeader.mVideo.field_10_found_rgb_bit_count = m.field_14_rgb_bit_count;
            return m.field_0_display_mode_idx;
        }
    }
    return 0;
}

SDevice* VidSystem::FindDevice(s32)
{
    return &mDevice;
}

void VidSystem::ApplyOptions()
{
    if (mOptions.Effective().mGeneration != mRenderer.Options().mGeneration)
    {
        mRenderer.SetOptions(mOptions.Effective());
    }
}

void VidSystem::ToggleFaithful()
{
    mOptions.ToggleFaithful();
    ApplyOptions();
}

bool VidSystem::AttachContext(IGLContext* pContext, const char** ppError)
{
    mContext = pContext;
    gLoaderContext = pContext;
    if (!mRenderer.Init(LoadGL, ppError))
    {
        return false;
    }
    mRenderer.SetOptions(mOptions.Effective());
    return true;
}

void VidSystem::SetMode(s32 modeId, s32 width, s32 height)
{
    SVideo& v = mHeader.mVideo;
    v.field_40_full_screen = modeId;
    v.field_80_active_mode_q = modeId;
    v.field_48_rect_right = width;
    v.field_4C_rect_bottom = height;
    mRenderer.SetLogicalSize(width, height);
    ResizeSurface(width, height);
}

void VidSystem::ResizeSurface(s32 width, s32 height)
{
    mSurfaceW = width;
    mSurfaceH = height;
    mSurface.assign((size_t)width * height, kSurfaceKey);
}

void VidSystem::GetSurface()
{
    SVideo& v = mHeader.mVideo;
    if (mSurface.empty())
    {
        return;
    }
    if (!mSurfaceGrabbed)
    {
        std::fill(mSurface.begin(), mSurface.end(), kSurfaceKey);
        mSurfaceGrabbed = true;
    }
    v.field_50_surface_pixels_ptr = &mSurface[0];
    v.field_54_surface_pixels_pitch = mSurfaceW * 2;
    v.field_4_flags |= 2;
}

void VidSystem::FreeSurface()
{
    mHeader.mVideo.field_4_flags &= ~2u;
}

void VidSystem::Flip()
{
    if (!mContext)
    {
        return;
    }
    mFrame++;
    if (mSurfaceGrabbed)
    {
        bool dirty = false;
        const u32 key2 = kSurfaceKey | (kSurfaceKey << 16);
        const u32* p = (const u32*)&mSurface[0];
        const size_t n = mSurface.size() / 2;
        for (size_t i = 0; i < n; i++)
        {
            if (p[i] != key2)
            {
                dirty = true;
                break;
            }
        }
        if (dirty)
        {
            mRenderer.CompositeSurface(&mSurface[0], mSurfaceW * 2, mSurfaceW, mSurfaceH, kSurfaceKey);
        }
        mSurfaceGrabbed = false;
    }

    const double t0 = mStatsInterval ? Now() : 0.0;
    mRenderer.FinishFrame();
    if (mCapture.WantFrame(mFrame))
    {
        std::vector<u8> rgba;
        mRenderer.ReadLogical(rgba);
        FrameCapture::WriteBmp(mCapture.PathFor(mFrame), &rgba[0], mRenderer.LogicalWidth(), mRenderer.LogicalHeight(), false);
    }

    int w = 0, h = 0;
    mContext->GetDrawableSize(&w, &h);
    mRenderer.Present(w, h);
    mContext->SwapBuffers();
    const double tw = mStatsInterval ? Now() : 0.0;
    mRenderer.ThrottleFrames();
    const double wait = mStatsInterval ? Now() - tw : 0.0;
    for (size_t i = 0; i < mToggleFrames.size(); i++)
    {
        if (mToggleFrames[i] == mFrame)
        {
            ToggleFaithful();
        }
    }
    mOptions.Poll(Now());
    ApplyOptions();
    if (mStatsInterval)
    {
        const double t1 = Now();
        mPresentSeconds += t1 - t0 - wait; // post effects + present + swap
        mWaitSeconds += wait;
        if (mFrame % mStatsInterval == 0)
        {
            if (mStatsStart > 0.0)
            {
                const double n = mStatsInterval;
                fprintf(stderr,
                        "glstats frame %u: %.1f fps, %.1f draw calls, %.0f vertices, %.1f uploads, post+present+swap %.3f ms, gpu wait "
                        "%.3f ms per frame\n",
                        mFrame,
                        n / (t1 - mStatsStart),
                        (mRenderer.GetDrawCallCount() - mStatsDraws) / n,
                        (mRenderer.GetVertexCount() - mStatsVerts) / n,
                        (mRenderer.GetUploadCount() - mStatsUploads) / n,
                        mPresentSeconds * 1000.0 / n,
                        mWaitSeconds * 1000.0 / n);
            }
            mStatsStart = t1;
            mPresentSeconds = 0.0;
            mWaitSeconds = 0.0;
            mStatsDraws = mRenderer.GetDrawCallCount();
            mStatsUploads = mRenderer.GetUploadCount();
            mStatsVerts = mRenderer.GetVertexCount();
        }
    }
}

void VidSystem::Clear(u8 r, u8 g, u8 b, s32 left, s32 top, s32 right, s32 bottom)
{
    mRenderer.Clear(r, g, b, left, top, right, bottom);
}

s32 VidSystem::SetGamma(f32 r, f32 g, f32 b)
{
    mRenderer.SetGamma(r, g, b);
    return 0;
}
