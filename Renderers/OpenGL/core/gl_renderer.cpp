#include "gl_renderer.hpp"
#include <math.h>
#include <stdlib.h>
#include <string.h>

namespace
{
// Faithful path: kept exactly as validated against the reference (any change of the varyings can
// move a rasterisation tie).
const char* kFaithfulVS = "#version 330 core\n"
                          "layout(location = 0) in vec3 aPos;\n"
                          "layout(location = 1) in vec4 aCol;\n"
                          "layout(location = 2) in vec2 aUV;\n"
                          "uniform vec2 uScale;\n"
                          "uniform vec2 uOffset;\n"
                          "out vec4 vCol;\n"
                          "out vec2 vUV;\n"
                          "void main()\n"
                          "{\n"
                          "    float w = 1.0 / max(aPos.z, 1e-7);\n"
                          "    vec2 p = (aPos.xy + uOffset) * uScale;\n"
                          "    gl_Position = vec4((p.x - 1.0) * w, (p.y - 1.0) * w, 0.0, w);\n"
                          "    vCol = aCol;\n"
                          "    vUV = aUV;\n"
                          "}\n";

const char* kFaithfulFS = "#version 330 core\n"
                          "uniform sampler2D uTex;\n"
                          "uniform int uUseTex;\n"
                          "uniform float uSampleShift;\n"
                          "in vec4 vCol;\n"
                          "in vec2 vUV;\n"
                          "out vec4 oColor;\n"
                          "void main()\n"
                          "{\n"
                          "    vec2 uv = vUV - (dFdx(vUV) + dFdy(vUV)) * uSampleShift;\n"
                          "    vec4 t = uUseTex != 0 ? texture(uTex, uv) : vec4(1.0);\n"
                          "    oColor = t * vCol;\n"
                          "}\n";

// Fullscreen triangle; also composites the CPU surface (alpha 0 where the game did not write).
const char* kPresentVS = "#version 330 core\n"
                         "out vec2 vUV;\n"
                         "void main()\n"
                         "{\n"
                         "    vec2 p = vec2((gl_VertexID << 1) & 2, gl_VertexID & 2);\n"
                         "    vUV = p;\n"
                         "    gl_Position = vec4(p * 2.0 - 1.0, 0.0, 1.0);\n"
                         "}\n";

const char* kPresentFS = "#version 330 core\n"
                         "uniform sampler2D uTex;\n"
                         "uniform vec3 uGamma;\n"
                         "uniform int uFlipY;\n"
                         "in vec2 vUV;\n"
                         "out vec4 oColor;\n"
                         "void main()\n"
                         "{\n"
                         "    vec2 uv = uFlipY != 0 ? vec2(vUV.x, 1.0 - vUV.y) : vUV;\n"
                         "    vec4 c = texture(uTex, uv);\n"
                         "    oColor = vec4(pow(c.rgb, 1.0 / uGamma), c.a);\n"
                         "}\n";

// D3D puts pixel centres on integers, GL on halves. The extra 1/2048 resolves exact ties like the
// reference without catching the original's "w - 0.001" quad edges.
const f32 kPixelOffset = 0.5f - 1.0f / 2048.0f;
// Texture sampling moved this many pixels towards the top left of the pixel centre (D3D tie behaviour).
const f32 kSampleShift = 1.0f / 1024.0f;
const s32 kMaxBatchVerts = 6 * 4096;
} // namespace

GLRenderer::GLRenderer() :
    mReady(false), mLogicalW(640), mLogicalH(480), mFbo(0), mFboTex(0), mBloomSrcTex(0), mPostFbo(0), mPostTex(0), mBlurW(0), mBlurH(0),
    mTargetsBloom(false), mPostValid(false), mFrameFence(0), mPresentProgram(0), mDownProgram(0), mBlurProgram(0), mComposeProgram(0),
    mVao(0), mVbo(0), mLightUbo(0), mSamplerNearest(0), mSamplerLinear(0), mWhiteTex(0), mSurfaceTex(0), mSurfaceW(0), mSurfaceH(0),
    mBatchTex(0), mBatchState(0), mAppliedState(0), mStateValid(false), mNumLights(0), mAmbient(255.0f), mDrawCalls(0), mUploads(0),
    mVertices(0)
{
    memset(mScene, 0, sizeof(mScene));
    mActiveScene = 0;
    memset(&gl, 0, sizeof(gl));
    mGamma[0] = mGamma[1] = mGamma[2] = 1.0f;
    mBlurFbo[0] = mBlurFbo[1] = 0;
    mBlurTex[0] = mBlurTex[1] = 0;
}

GLRenderer::~GLRenderer()
{
    Shutdown();
}

GLuint GLRenderer::CompileProgram(const char* vs, const char* fs, const char** ppError)
{
    static char log[1024];
    GLuint shaders[2];
    const char* srcs[2] = {vs, fs};
    const GLenum types[2] = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER};
    for (int i = 0; i < 2; i++)
    {
        shaders[i] = gl.glCreateShader(types[i]);
        gl.glShaderSource(shaders[i], 1, &srcs[i], NULL);
        gl.glCompileShader(shaders[i]);
        GLint ok = 0;
        gl.glGetShaderiv(shaders[i], GL_COMPILE_STATUS, &ok);
        if (!ok)
        {
            gl.glGetShaderInfoLog(shaders[i], sizeof(log), NULL, log);
            *ppError = log;
            return 0;
        }
    }
    GLuint prog = gl.glCreateProgram();
    gl.glAttachShader(prog, shaders[0]);
    gl.glAttachShader(prog, shaders[1]);
    gl.glLinkProgram(prog);
    gl.glDeleteShader(shaders[0]);
    gl.glDeleteShader(shaders[1]);
    GLint ok = 0;
    gl.glGetProgramiv(prog, GL_LINK_STATUS, &ok);
    if (!ok)
    {
        gl.glGetProgramInfoLog(prog, sizeof(log), NULL, log);
        *ppError = log;
        return 0;
    }
    return prog;
}

bool GLRenderer::Init(GLFuncs::Loader loader, const char** ppError)
{
    const char* missing = gl.Load(loader);
    if (missing)
    {
        *ppError = missing;
        return false;
    }

    mScene[0].mId = CompileProgram(kFaithfulVS, kFaithfulFS, ppError);
    mPresentProgram = mScene[0].mId ? CompileProgram(kPresentVS, kPresentFS, ppError) : 0;
    if (!mPresentProgram || !InitEnhanced(kPresentVS, ppError))
    {
        return false;
    }
    for (int i = 0; i < 2; i++)
    {
        SceneProgram& sp = mScene[i];
        sp.mScale = gl.glGetUniformLocation(sp.mId, "uScale");
        sp.mOffset = gl.glGetUniformLocation(sp.mId, "uOffset");
        sp.mUseTex = gl.glGetUniformLocation(sp.mId, "uUseTex");
        sp.mSampleShift = gl.glGetUniformLocation(sp.mId, "uSampleShift");
        sp.mAdditive = gl.glGetUniformLocation(sp.mId, "uAdditive");
        sp.mPerPixel = gl.glGetUniformLocation(sp.mId, "uPerPixel");
        sp.mNumLights = gl.glGetUniformLocation(sp.mId, "uNumLights");
        sp.mAmbient = gl.glGetUniformLocation(sp.mId, "uAmbient");
        gl.glUseProgram(sp.mId);
        gl.glUniform1i(gl.glGetUniformLocation(sp.mId, "uTex"), 0);
    }
    mUniGamma = gl.glGetUniformLocation(mPresentProgram, "uGamma");
    mUniFlipY = gl.glGetUniformLocation(mPresentProgram, "uFlipY");
    gl.glUseProgram(mPresentProgram);
    gl.glUniform1i(gl.glGetUniformLocation(mPresentProgram, "uTex"), 0);

    gl.glGenVertexArrays(1, &mVao);
    gl.glBindVertexArray(mVao);
    gl.glGenBuffers(1, &mVbo);
    gl.glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    gl.glBufferData(GL_ARRAY_BUFFER, kMaxBatchVerts * sizeof(RVertex), NULL, GL_STREAM_DRAW);
    gl.glEnableVertexAttribArray(0);
    gl.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(RVertex), (void*)offsetof(RVertex, x));
    gl.glEnableVertexAttribArray(1);
    gl.glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(RVertex), (void*)offsetof(RVertex, rgba));
    gl.glEnableVertexAttribArray(2);
    gl.glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(RVertex), (void*)offsetof(RVertex, u));
    gl.glEnableVertexAttribArray(3);
    gl.glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(RVertex), (void*)offsetof(RVertex, wx));
    gl.glEnableVertexAttribArray(4);
    gl.glVertexAttribPointer(4, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(RVertex), (void*)offsetof(RVertex, unlit));
    gl.glEnableVertexAttribArray(5);
    gl.glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(RVertex), (void*)offsetof(RVertex, shade));
    gl.glEnableVertexAttribArray(6);
    gl.glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, sizeof(RVertex), (void*)offsetof(RVertex, lit));

    gl.glGenSamplers(1, &mSamplerNearest);
    gl.glGenSamplers(1, &mSamplerLinear);
    GLuint samplers[2] = {mSamplerNearest, mSamplerLinear};
    for (int i = 0; i < 2; i++)
    {
        const GLint f = i ? GL_LINEAR : GL_NEAREST;
        gl.glSamplerParameteri(samplers[i], GL_TEXTURE_MIN_FILTER, f);
        gl.glSamplerParameteri(samplers[i], GL_TEXTURE_MAG_FILTER, f);
        gl.glSamplerParameteri(samplers[i], GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        gl.glSamplerParameteri(samplers[i], GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    const u32 white = 0xFFFFFFFFu;
    gl.glGenTextures(1, &mWhiteTex);
    gl.glBindTexture(GL_TEXTURE_2D, mWhiteTex);
    gl.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &white);

    gl.glGenTextures(1, &mSurfaceTex);
    gl.glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    gl.glPixelStorei(GL_PACK_ALIGNMENT, 1);

    gl.glGenFramebuffers(1, &mFbo);
    gl.glGenTextures(1, &mFboTex);
    gl.glGenTextures(1, &mBloomSrcTex);
    gl.glGenFramebuffers(1, &mPostFbo);
    gl.glGenTextures(1, &mPostTex);
    gl.glGenFramebuffers(2, mBlurFbo);
    gl.glGenTextures(2, mBlurTex);

    mBatch.reserve(kMaxBatchVerts);
    mReady = true;
    CreateTargets();
    Clear(0, 0, 0, 0, 0, 0, 0);
    return true;
}

void GLRenderer::ThrottleFrames()
{
    if (mFrameFence)
    {
        gl.glClientWaitSync(mFrameFence, GL_SYNC_FLUSH_COMMANDS_BIT, 1000000000ull);
        gl.glDeleteSync(mFrameFence);
    }
    mFrameFence = gl.glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
}

void GLRenderer::Shutdown()
{
    if (!mReady)
    {
        return;
    }
    if (mFrameFence)
    {
        gl.glDeleteSync(mFrameFence);
        mFrameFence = 0;
    }
    gl.glDeleteFramebuffers(1, &mFbo);
    gl.glDeleteFramebuffers(1, &mPostFbo);
    gl.glDeleteFramebuffers(2, mBlurFbo);
    gl.glDeleteTextures(1, &mFboTex);
    gl.glDeleteTextures(1, &mBloomSrcTex);
    gl.glDeleteTextures(1, &mPostTex);
    gl.glDeleteTextures(2, mBlurTex);
    gl.glDeleteTextures(1, &mWhiteTex);
    gl.glDeleteTextures(1, &mSurfaceTex);
    gl.glDeleteSamplers(1, &mSamplerNearest);
    gl.glDeleteSamplers(1, &mSamplerLinear);
    gl.glDeleteBuffers(1, &mVbo);
    gl.glDeleteBuffers(1, &mLightUbo);
    gl.glDeleteVertexArrays(1, &mVao);
    gl.glDeleteProgram(mScene[0].mId);
    gl.glDeleteProgram(mScene[1].mId);
    gl.glDeleteProgram(mPresentProgram);
    gl.glDeleteProgram(mDownProgram);
    gl.glDeleteProgram(mBlurProgram);
    gl.glDeleteProgram(mComposeProgram);
    mReady = false;
}

void GLRenderer::SetOptions(const RenderOptions& options)
{
    const bool targetsChanged = options.mTrueColour != mOptions.mTrueColour || (options.mBloom > 0.0f) != (mOptions.mBloom > 0.0f);
    Flush();
    mOptions = options;
    if (targetsChanged && mReady)
    {
        CreateTargets();
        Clear(0, 0, 0, 0, 0, 0, 0);
    }
    mStateValid = false;
}

void GLRenderer::SetLogicalSize(s32 width, s32 height)
{
    if (width <= 0 || height <= 0)
    {
        return;
    }
    Flush();
    const bool changed = width != mLogicalW || height != mLogicalH;
    mLogicalW = width;
    mLogicalH = height;
    if (mReady && changed)
    {
        CreateTargets();
    }
}

void GLRenderer::CreateTargets()
{
    const s32 width = mLogicalW, height = mLogicalH;
    gl.glBindTexture(GL_TEXTURE_2D, mFboTex);
    gl.glBindFramebuffer(GL_FRAMEBUFFER, mFbo);
    bool complete = false;
    if (!mOptions.mTrueColour)
    {
        // The original's 16-bit back buffer.
        gl.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB565, width, height, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL);
        gl.glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mFboTex, 0);
        complete = gl.glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
    }
    if (!complete)
    {
        gl.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        gl.glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mFboTex, 0);
    }
    mTargetsBloom = mOptions.mBloom > 0.0f;
    const GLenum one[1] = {GL_COLOR_ATTACHMENT0};
    if (mTargetsBloom)
    {
        CreateBloomTargets(width, height);
    }
    else
    {
        gl.glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, 0, 0);
        gl.glDrawBuffers(1, one);
    }
    mPostValid = false;
    mStateValid = false;
}

void GLRenderer::BindTarget()
{
    gl.glBindFramebuffer(GL_FRAMEBUFFER, mFbo);
    gl.glViewport(0, 0, mLogicalW, mLogicalH);
}

void GLRenderer::Clear(u8 r, u8 g, u8 b, s32 left, s32 top, s32 right, s32 bottom)
{
    if (!mReady)
    {
        return;
    }
    Flush();
    BindTarget();
    const bool full =
        (left == 0 && top == 0 && right == 0 && bottom == 0) || (left <= 0 && top <= 0 && right >= mLogicalW && bottom >= mLogicalH);
    if (!full)
    {
        gl.glEnable(GL_SCISSOR_TEST);
        gl.glScissor(left, top, right - left, bottom - top);
    }
    const GLfloat colour[4] = {r / 255.0f, g / 255.0f, b / 255.0f, 1.0f};
    gl.glClearBufferfv(GL_COLOR, 0, colour);
    if (mTargetsBloom)
    {
        const GLfloat zero[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        gl.glClearBufferfv(GL_COLOR, 1, zero);
    }
    gl.glDisable(GL_SCISSOR_TEST);
}

RTexture* GLRenderer::CreateTexture()
{
    RTexture* pTex = new RTexture;
    pTex->mId = 0;
    pTex->mWidth = 0;
    pTex->mHeight = 0;
    return pTex;
}

void GLRenderer::UploadTexture(RTexture* pTex, s32 width, s32 height, const u32* pRgba)
{
    if (!mReady || !pTex || width <= 0 || height <= 0)
    {
        return;
    }
    if (pTex == mBatchTex)
    {
        Flush();
    }
    if (!pTex->mId)
    {
        gl.glGenTextures(1, &pTex->mId);
    }
    mUploads++;
    gl.glBindTexture(GL_TEXTURE_2D, pTex->mId);
    if (pTex->mWidth != width || pTex->mHeight != height)
    {
        gl.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pRgba);
        pTex->mWidth = width;
        pTex->mHeight = height;
    }
    else
    {
        gl.glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pRgba);
    }
}

void GLRenderer::FreeTexture(RTexture* pTex)
{
    if (!pTex)
    {
        return;
    }
    if (pTex == mBatchTex)
    {
        Flush();
        mBatchTex = 0;
    }
    if (mReady && pTex->mId)
    {
        gl.glDeleteTextures(1, &pTex->mId);
    }
    delete pTex;
}

void GLRenderer::BeginScene()
{
    mStateValid = false;
}

void GLRenderer::EndScene()
{
    Flush();
}

void GLRenderer::ApplyState(u32 state)
{
    if (mStateValid && state == mAppliedState)
    {
        return;
    }
    if (state & RS_BLEND_ADD)
    {
        gl.glEnable(GL_BLEND);
        gl.glBlendFunc(GL_ONE, GL_ONE);
    }
    else if (state & RS_BLEND_ALPHA)
    {
        gl.glEnable(GL_BLEND);
        gl.glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    else
    {
        gl.glDisable(GL_BLEND);
    }
    const SceneProgram& sp = mScene[mActiveScene];
    gl.glUniform1i(sp.mUseTex, (state & RS_NO_TEXTURE) ? 0 : 1);
    if (mActiveScene == 1)
    {
        gl.glUniform1i(sp.mAdditive, (state & RS_BLEND_ADD) ? 1 : 0);
        gl.glUniform1i(sp.mPerPixel, mOptions.mPerPixelLighting ? 1 : 0);
        gl.glUniform1i(sp.mNumLights, mNumLights);
        gl.glUniform1f(sp.mAmbient, mAmbient);
    }
    gl.glBindSampler(0, (state & RS_FILTER_LINEAR) ? mSamplerLinear : mSamplerNearest);
    if (state & RS_PIXEL_EXACT)
    {
        gl.glUniform2f(sp.mOffset, 0.0f, 0.0f);
        gl.glUniform1f(sp.mSampleShift, 0.0f);
    }
    else
    {
        gl.glUniform2f(sp.mOffset, kPixelOffset, kPixelOffset);
        gl.glUniform1f(sp.mSampleShift, kSampleShift);
    }
    mAppliedState = state;
    mStateValid = true;
}

void GLRenderer::DrawTriangles(const RVertex* pVerts, s32 count, RTexture* pTex, u32 state)
{
    if (!mReady || count <= 0)
    {
        return;
    }
    if (!pTex || !pTex->mId)
    {
        state |= RS_NO_TEXTURE;
        pTex = 0;
    }
    if (!mBatch.empty() && (pTex != mBatchTex || state != mBatchState || (s32)mBatch.size() + count > kMaxBatchVerts))
    {
        Flush();
    }
    mBatchTex = pTex;
    mBatchState = state;
    mBatch.insert(mBatch.end(), pVerts, pVerts + count);
}

void GLRenderer::Flush()
{
    if (mBatch.empty() || !mReady)
    {
        mBatch.clear();
        return;
    }
    BindTarget();
    const int scene = (mOptions.mBloom > 0.0f || mOptions.mPerPixelLighting) ? 1 : 0;
    if (scene != mActiveScene)
    {
        mActiveScene = scene;
        mStateValid = false;
    }
    gl.glUseProgram(mScene[scene].mId);
    gl.glUniform2f(mScene[scene].mScale, 2.0f / mLogicalW, 2.0f / mLogicalH);
    gl.glBindVertexArray(mVao);
    gl.glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    gl.glBufferData(GL_ARRAY_BUFFER, kMaxBatchVerts * sizeof(RVertex), NULL, GL_STREAM_DRAW);
    gl.glBufferSubData(GL_ARRAY_BUFFER, 0, mBatch.size() * sizeof(RVertex), &mBatch[0]);
    gl.glActiveTexture(GL_TEXTURE0);
    gl.glBindTexture(GL_TEXTURE_2D, mBatchTex ? mBatchTex->mId : mWhiteTex);
    ApplyState(mBatchState);
    gl.glDrawArrays(GL_TRIANGLES, 0, (GLsizei)mBatch.size());
    mDrawCalls++;
    mVertices += (u32)mBatch.size();
    mBatch.clear();
}

void GLRenderer::DrawFullscreen()
{
    gl.glBindVertexArray(mVao);
    gl.glDrawArrays(GL_TRIANGLES, 0, 3);
}

void GLRenderer::CompositeSurface(const u16* pPixels, s32 pitchBytes, s32 width, s32 height, u16 key)
{
    if (!mReady || !pPixels)
    {
        return;
    }
    Flush();
    mSurfaceRgba.resize((size_t)width * height);
    for (s32 y = 0; y < height; y++)
    {
        const u16* row = (const u16*)((const u8*)pPixels + (size_t)y * pitchBytes);
        u32* out = &mSurfaceRgba[(size_t)y * width];
        for (s32 x = 0; x < width; x++)
        {
            const u16 p = row[x];
            if (p == key)
            {
                out[x] = 0;
                continue;
            }
            const u32 r = (p >> 11) & 31, g = (p >> 5) & 63, b = p & 31;
            out[x] = ((r << 3) | (r >> 2)) | (((g << 2) | (g >> 4)) << 8) | (((b << 3) | (b >> 2)) << 16) | 0xFF000000u;
        }
    }
    gl.glBindTexture(GL_TEXTURE_2D, mSurfaceTex);
    if (mSurfaceW != width || mSurfaceH != height)
    {
        gl.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &mSurfaceRgba[0]);
        mSurfaceW = width;
        mSurfaceH = height;
    }
    else
    {
        gl.glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, &mSurfaceRgba[0]);
    }

    BindTarget();
    const GLenum one[1] = {GL_COLOR_ATTACHMENT0};
    gl.glDrawBuffers(1, one);
    gl.glViewport(0, 0, width, height);
    gl.glUseProgram(mPresentProgram);
    gl.glUniform3f(mUniGamma, 1.0f, 1.0f, 1.0f);
    gl.glUniform1i(mUniFlipY, 0);
    gl.glActiveTexture(GL_TEXTURE0);
    gl.glBindSampler(0, mSamplerNearest);
    gl.glEnable(GL_BLEND);
    gl.glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    DrawFullscreen();
    if (mTargetsBloom)
    {
        const GLenum two[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
        gl.glDrawBuffers(2, two);
    }
    mStateValid = false;
}

void GLRenderer::FinishFrame()
{
    if (!mReady)
    {
        return;
    }
    Flush();
    mPostValid = false;
    if (mTargetsBloom && mOptions.mBloom > 0.0f)
    {
        ApplyBloom();
    }
}

void GLRenderer::SetGamma(f32 r, f32 g, f32 b)
{
    mGamma[0] = r > 0.05f ? r : 0.05f;
    mGamma[1] = g > 0.05f ? g : 0.05f;
    mGamma[2] = b > 0.05f ? b : 0.05f;
}

void GLRenderer::Present(s32 drawableW, s32 drawableH)
{
    if (!mReady)
    {
        return;
    }
    Flush();
    gl.glBindFramebuffer(GL_FRAMEBUFFER, 0);
    gl.glViewport(0, 0, drawableW, drawableH);
    gl.glClearColor(0, 0, 0, 1);
    gl.glClear(GL_COLOR_BUFFER_BIT);

    // Keep the aspect ratio of the logical target.
    s32 w = drawableW;
    s32 h = (s32)((long long)drawableW * mLogicalH / mLogicalW);
    if (h > drawableH)
    {
        h = drawableH;
        w = (s32)((long long)drawableH * mLogicalW / mLogicalH);
    }
    gl.glViewport((drawableW - w) / 2, (drawableH - h) / 2, w, h);
    gl.glUseProgram(mPresentProgram);
    gl.glUniform3f(mUniGamma, mGamma[0], mGamma[1], mGamma[2]);
    gl.glUniform1i(mUniFlipY, 1);
    gl.glActiveTexture(GL_TEXTURE0);
    gl.glBindTexture(GL_TEXTURE_2D, mPostValid ? mPostTex : mFboTex);
    const bool exact = (w == mLogicalW && h == mLogicalH) || (w % mLogicalW == 0 && h % mLogicalH == 0);
    gl.glBindSampler(0, exact ? mSamplerNearest : mSamplerLinear);
    gl.glDisable(GL_BLEND);
    DrawFullscreen();
    mStateValid = false;
}

void GLRenderer::ReadLogical(std::vector<u8>& out)
{
    out.resize((size_t)mLogicalW * mLogicalH * 4);
    if (!mReady)
    {
        return;
    }
    Flush();
    gl.glBindFramebuffer(GL_FRAMEBUFFER, mPostValid ? mPostFbo : mFbo);
    gl.glReadBuffer(GL_COLOR_ATTACHMENT0);
    gl.glReadPixels(0, 0, mLogicalW, mLogicalH, GL_RGBA, GL_UNSIGNED_BYTE, &out[0]);
}
