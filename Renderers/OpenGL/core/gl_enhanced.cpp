// Optional enhancements (gta2gl.ini, all off by default): bloom and per pixel lighting.
// Nothing here runs for the faithful renderer, which only uses gl_renderer.cpp.
#include "gl_renderer.hpp"
#include <string.h>

namespace
{
// Enhanced path (bloom output, per pixel lighting).
const char* kSceneVS = "#version 330 core\n"
                       "layout(location = 0) in vec3 aPos;\n"
                       "layout(location = 1) in vec4 aCol;\n"
                       "layout(location = 2) in vec2 aUV;\n"
                       "layout(location = 3) in vec3 aWorld;\n"
                       "layout(location = 4) in vec4 aUnlit;\n"
                       "layout(location = 5) in float aShade;\n"
                       "layout(location = 6) in float aLit;\n"
                       "uniform vec2 uScale;\n"
                       "uniform vec2 uOffset;\n"
                       "out vec4 vCol;\n"
                       "out vec2 vUV;\n"
                       "out vec3 vWorld;\n"
                       "out vec4 vUnlit;\n"
                       "out float vShade;\n"
                       "out float vLit;\n"
                       "void main()\n"
                       "{\n"
                       "    float w = 1.0 / max(aPos.z, 1e-7);\n"
                       "    vec2 p = (aPos.xy + uOffset) * uScale;\n"
                       "    gl_Position = vec4((p.x - 1.0) * w, (p.y - 1.0) * w, 0.0, w);\n"
                       "    vCol = aCol;\n"
                       "    vUV = aUV;\n"
                       "    vWorld = aWorld;\n"
                       "    vUnlit = aUnlit;\n"
                       "    vShade = aShade;\n"
                       "    vLit = aLit;\n"
                       "}\n";

// Per pixel lighting is the original's LightVertex formula with a real square root.
const char* kSceneFS = "#version 330 core\n"
                       "uniform sampler2D uTex;\n"
                       "uniform int uUseTex;\n"
                       "uniform float uSampleShift;\n"
                       "uniform int uAdditive;\n"
                       "uniform int uPerPixel;\n"
                       "uniform int uNumLights;\n"
                       "uniform float uAmbient;\n"
                       "layout(std140) uniform Lights\n"
                       "{\n"
                       "    vec4 uLightPos[256];\n" // xyz, radius
                       "    vec4 uLightCol[256];\n" // rgb * brightness, 1 / radius
                       "};\n"
                       "in vec4 vCol;\n"
                       "in vec2 vUV;\n"
                       "in vec3 vWorld;\n"
                       "in vec4 vUnlit;\n"
                       "in float vShade;\n"
                       "in float vLit;\n"
                       "layout(location = 0) out vec4 oColor;\n"
                       "layout(location = 1) out vec4 oBloom;\n"
                       "void main()\n"
                       "{\n"
                       "    vec2 uv = vUV - (dFdx(vUV) + dFdy(vUV)) * uSampleShift;\n"
                       "    vec4 t = uUseTex != 0 ? texture(uTex, uv) : vec4(1.0);\n"
                       "    vec4 c = vCol;\n"
                       "    if (uPerPixel != 0 && vLit > 0.5)\n"
                       "    {\n"
                       "        vec3 acc = vec3(0.0);\n"
                       "        for (int i = 0; i < uNumLights; i++)\n"
                       "        {\n"
                       "            vec3 d = vWorld - uLightPos[i].xyz;\n"
                       "            float d2 = dot(d, d);\n"
                       "            float r = uLightPos[i].w;\n"
                       "            if (d2 <= r * r)\n"
                       "            {\n"
                       "                float att = (r - sqrt(d2)) * uLightCol[i].w;\n"
                       "                if (att > 0.0) acc += uLightCol[i].rgb * att;\n"
                       "            }\n"
                       "        }\n"
                       "        c = vec4(min((vShade * vUnlit.rgb * 255.0 * acc + uAmbient) / 255.0, vec3(1.0)), vUnlit.a);\n"
                       "    }\n"
                       "    oColor = t * c;\n"
                       "    oBloom = uAdditive != 0 ? oColor : vec4(0.0, 0.0, 0.0, oColor.a);\n"
                       "}\n";

// Bloom: 4x4 box downsample (four bilinear taps), separable 9 tap gaussian, additive compose.
const char* kDownFS =
    "#version 330 core\n"
    "uniform sampler2D uTex;\n"
    "uniform vec2 uTexel;\n"
    "in vec2 vUV;\n"
    "out vec4 oColor;\n"
    "void main()\n"
    "{\n"
    "    oColor = 0.25 * (texture(uTex, vUV + uTexel * vec2(-1.0, -1.0)) + texture(uTex, vUV + uTexel * vec2(1.0, -1.0)) +\n"
    "                     texture(uTex, vUV + uTexel * vec2(-1.0, 1.0)) + texture(uTex, vUV + uTexel * vec2(1.0, 1.0)));\n"
    "}\n";

const char* kBlurFS = "#version 330 core\n"
                      "uniform sampler2D uTex;\n"
                      "uniform vec2 uStep;\n"
                      "in vec2 vUV;\n"
                      "out vec4 oColor;\n"
                      "void main()\n"
                      "{\n"
                      "    const float w[5] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);\n"
                      "    vec4 c = texture(uTex, vUV) * w[0];\n"
                      "    for (int i = 1; i < 5; i++)\n"
                      "    {\n"
                      "        c += (texture(uTex, vUV + uStep * float(i)) + texture(uTex, vUV - uStep * float(i))) * w[i];\n"
                      "    }\n"
                      "    oColor = c;\n"
                      "}\n";

const char* kComposeFS = "#version 330 core\n"
                         "uniform sampler2D uTex;\n"
                         "uniform sampler2D uBloom;\n"
                         "uniform float uIntensity;\n"
                         "in vec2 vUV;\n"
                         "out vec4 oColor;\n"
                         "void main()\n"
                         "{\n"
                         "    vec3 c = texture(uTex, vUV).rgb + texture(uBloom, vUV).rgb * (uIntensity * 2.0);\n"
                         "    oColor = vec4(min(c, vec3(1.0)), 1.0);\n"
                         "}\n";

const s32 kMaxLights = 256;
} // namespace

// Scene program with the bloom output and per pixel lighting, plus the bloom post passes.
bool GLRenderer::InitEnhanced(const char* pFullscreenVS, const char** ppError)
{
    mScene[1].mId = CompileProgram(kSceneVS, kSceneFS, ppError);
    mDownProgram = mScene[1].mId ? CompileProgram(pFullscreenVS, kDownFS, ppError) : 0;
    mBlurProgram = mDownProgram ? CompileProgram(pFullscreenVS, kBlurFS, ppError) : 0;
    mComposeProgram = mBlurProgram ? CompileProgram(pFullscreenVS, kComposeFS, ppError) : 0;
    if (!mComposeProgram)
    {
        return false;
    }
    mUniDownTexel = gl.glGetUniformLocation(mDownProgram, "uTexel");
    mUniBlurStep = gl.glGetUniformLocation(mBlurProgram, "uStep");
    mUniBloomIntensity = gl.glGetUniformLocation(mComposeProgram, "uIntensity");
    gl.glUseProgram(mScene[1].mId);
    gl.glUniformBlockBinding(mScene[1].mId, gl.glGetUniformBlockIndex(mScene[1].mId, "Lights"), 0);
    const GLuint posts[3] = {mDownProgram, mBlurProgram, mComposeProgram};
    for (int i = 0; i < 3; i++)
    {
        gl.glUseProgram(posts[i]);
        gl.glUniform1i(gl.glGetUniformLocation(posts[i], "uTex"), 0);
    }
    gl.glUniform1i(gl.glGetUniformLocation(mComposeProgram, "uBloom"), 1);

    gl.glGenBuffers(1, &mLightUbo);
    gl.glBindBuffer(GL_UNIFORM_BUFFER, mLightUbo);
    gl.glBufferData(GL_UNIFORM_BUFFER, kMaxLights * 2 * 16, NULL, GL_DYNAMIC_DRAW);
    gl.glBindBufferBase(GL_UNIFORM_BUFFER, 0, mLightUbo);
    return true;
}

// Second colour attachment for the additive draws, the composed frame and two quarter size blur targets.
void GLRenderer::CreateBloomTargets(s32 width, s32 height)
{
    gl.glBindTexture(GL_TEXTURE_2D, mBloomSrcTex);
    gl.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    gl.glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, mBloomSrcTex, 0);
    const GLenum two[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    gl.glDrawBuffers(2, two);

    gl.glBindTexture(GL_TEXTURE_2D, mPostTex);
    gl.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    gl.glBindFramebuffer(GL_FRAMEBUFFER, mPostFbo);
    gl.glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mPostTex, 0);
    mBlurW = (width + 3) / 4;
    mBlurH = (height + 3) / 4;
    for (int i = 0; i < 2; i++)
    {
        gl.glBindTexture(GL_TEXTURE_2D, mBlurTex[i]);
        gl.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mBlurW, mBlurH, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        gl.glBindFramebuffer(GL_FRAMEBUFFER, mBlurFbo[i]);
        gl.glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mBlurTex[i], 0);
    }
}

void GLRenderer::ApplyBloom()
{
    gl.glDisable(GL_BLEND);
    gl.glActiveTexture(GL_TEXTURE0);
    gl.glBindSampler(0, mSamplerLinear);

    // Additive contributions, quarter size.
    gl.glBindFramebuffer(GL_FRAMEBUFFER, mBlurFbo[0]);
    gl.glViewport(0, 0, mBlurW, mBlurH);
    gl.glUseProgram(mDownProgram);
    gl.glUniform2f(mUniDownTexel, 1.0f / mLogicalW, 1.0f / mLogicalH);
    gl.glBindTexture(GL_TEXTURE_2D, mBloomSrcTex);
    DrawFullscreen();

    gl.glUseProgram(mBlurProgram);
    for (int pass = 0; pass < 2; pass++)
    {
        gl.glBindFramebuffer(GL_FRAMEBUFFER, mBlurFbo[1 - pass]);
        gl.glUniform2f(mUniBlurStep, pass == 0 ? 1.0f / mBlurW : 0.0f, pass == 0 ? 0.0f : 1.0f / mBlurH);
        gl.glBindTexture(GL_TEXTURE_2D, mBlurTex[pass]);
        DrawFullscreen();
    }

    gl.glBindFramebuffer(GL_FRAMEBUFFER, mPostFbo);
    gl.glViewport(0, 0, mLogicalW, mLogicalH);
    gl.glUseProgram(mComposeProgram);
    gl.glUniform1f(mUniBloomIntensity, mOptions.mBloom);
    gl.glBindTexture(GL_TEXTURE_2D, mFboTex);
    gl.glBindSampler(0, mSamplerNearest);
    gl.glActiveTexture(GL_TEXTURE1);
    gl.glBindTexture(GL_TEXTURE_2D, mBlurTex[0]);
    gl.glBindSampler(1, mSamplerLinear);
    DrawFullscreen();
    gl.glBindSampler(1, 0);
    gl.glActiveTexture(GL_TEXTURE0);
    mPostValid = true;
    mStateValid = false;
}

void GLRenderer::SetLights(const RLight* pLights, s32 count, f32 ambient)
{
    if (!mReady)
    {
        return;
    }
    Flush();
    if (count > kMaxLights)
    {
        count = kMaxLights;
    }
    static f32 data[kMaxLights * 8];
    memset(data, 0, sizeof(data));
    for (s32 i = 0; i < count; i++)
    {
        const RLight& l = pLights[i];
        f32* pos = &data[i * 4];
        f32* col = &data[kMaxLights * 4 + i * 4];
        pos[0] = l.x, pos[1] = l.y, pos[2] = l.z, pos[3] = l.radius;
        col[0] = l.r, col[1] = l.g, col[2] = l.b, col[3] = l.invRadius;
    }
    gl.glBindBuffer(GL_UNIFORM_BUFFER, mLightUbo);
    gl.glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(data), data);
    mNumLights = count;
    mAmbient = ambient;
    mStateValid = false;
}
