// OpenGL 3.3 core entry points, all loaded at runtime through a platform supplied loader.
#pragma once

#define GL_GLEXT_PROTOTYPES 0
#include <GL/glcorearb.h>

#define GTA2GL_FUNCS(X)                                            \
    X(PFNGLGETERRORPROC, glGetError)                               \
    X(PFNGLGETSTRINGPROC, glGetString)                             \
    X(PFNGLGETINTEGERVPROC, glGetIntegerv)                         \
    X(PFNGLENABLEPROC, glEnable)                                   \
    X(PFNGLDISABLEPROC, glDisable)                                 \
    X(PFNGLBLENDFUNCPROC, glBlendFunc)                             \
    X(PFNGLBLENDFUNCSEPARATEPROC, glBlendFuncSeparate)             \
    X(PFNGLVIEWPORTPROC, glViewport)                               \
    X(PFNGLSCISSORPROC, glScissor)                                 \
    X(PFNGLCLEARCOLORPROC, glClearColor)                           \
    X(PFNGLCLEARPROC, glClear)                                     \
    X(PFNGLPIXELSTOREIPROC, glPixelStorei)                         \
    X(PFNGLREADPIXELSPROC, glReadPixels)                           \
    X(PFNGLREADBUFFERPROC, glReadBuffer)                           \
    X(PFNGLGENTEXTURESPROC, glGenTextures)                         \
    X(PFNGLDELETETEXTURESPROC, glDeleteTextures)                   \
    X(PFNGLBINDTEXTUREPROC, glBindTexture)                         \
    X(PFNGLTEXIMAGE2DPROC, glTexImage2D)                           \
    X(PFNGLTEXSUBIMAGE2DPROC, glTexSubImage2D)                     \
    X(PFNGLTEXPARAMETERIPROC, glTexParameteri)                     \
    X(PFNGLACTIVETEXTUREPROC, glActiveTexture)                     \
    X(PFNGLDRAWARRAYSPROC, glDrawArrays)                           \
    X(PFNGLGENBUFFERSPROC, glGenBuffers)                           \
    X(PFNGLDELETEBUFFERSPROC, glDeleteBuffers)                     \
    X(PFNGLBINDBUFFERPROC, glBindBuffer)                           \
    X(PFNGLBUFFERDATAPROC, glBufferData)                           \
    X(PFNGLBUFFERSUBDATAPROC, glBufferSubData)                     \
    X(PFNGLGENVERTEXARRAYSPROC, glGenVertexArrays)                 \
    X(PFNGLDELETEVERTEXARRAYSPROC, glDeleteVertexArrays)           \
    X(PFNGLBINDVERTEXARRAYPROC, glBindVertexArray)                 \
    X(PFNGLENABLEVERTEXATTRIBARRAYPROC, glEnableVertexAttribArray) \
    X(PFNGLVERTEXATTRIBPOINTERPROC, glVertexAttribPointer)         \
    X(PFNGLCREATESHADERPROC, glCreateShader)                       \
    X(PFNGLDELETESHADERPROC, glDeleteShader)                       \
    X(PFNGLSHADERSOURCEPROC, glShaderSource)                       \
    X(PFNGLCOMPILESHADERPROC, glCompileShader)                     \
    X(PFNGLGETSHADERIVPROC, glGetShaderiv)                         \
    X(PFNGLGETSHADERINFOLOGPROC, glGetShaderInfoLog)               \
    X(PFNGLCREATEPROGRAMPROC, glCreateProgram)                     \
    X(PFNGLDELETEPROGRAMPROC, glDeleteProgram)                     \
    X(PFNGLATTACHSHADERPROC, glAttachShader)                       \
    X(PFNGLBINDATTRIBLOCATIONPROC, glBindAttribLocation)           \
    X(PFNGLLINKPROGRAMPROC, glLinkProgram)                         \
    X(PFNGLGETPROGRAMIVPROC, glGetProgramiv)                       \
    X(PFNGLGETPROGRAMINFOLOGPROC, glGetProgramInfoLog)             \
    X(PFNGLUSEPROGRAMPROC, glUseProgram)                           \
    X(PFNGLGETUNIFORMLOCATIONPROC, glGetUniformLocation)           \
    X(PFNGLUNIFORM1IPROC, glUniform1i)                             \
    X(PFNGLUNIFORM1FPROC, glUniform1f)                             \
    X(PFNGLUNIFORM2FPROC, glUniform2f)                             \
    X(PFNGLUNIFORM3FPROC, glUniform3f)                             \
    X(PFNGLUNIFORM4FPROC, glUniform4f)                             \
    X(PFNGLGENFRAMEBUFFERSPROC, glGenFramebuffers)                 \
    X(PFNGLDELETEFRAMEBUFFERSPROC, glDeleteFramebuffers)           \
    X(PFNGLBINDFRAMEBUFFERPROC, glBindFramebuffer)                 \
    X(PFNGLFRAMEBUFFERTEXTURE2DPROC, glFramebufferTexture2D)       \
    X(PFNGLCHECKFRAMEBUFFERSTATUSPROC, glCheckFramebufferStatus)   \
    X(PFNGLGENSAMPLERSPROC, glGenSamplers)                         \
    X(PFNGLDELETESAMPLERSPROC, glDeleteSamplers)                   \
    X(PFNGLBINDSAMPLERPROC, glBindSampler)                         \
    X(PFNGLSAMPLERPARAMETERIPROC, glSamplerParameteri)             \
    X(PFNGLDRAWBUFFERSPROC, glDrawBuffers)                         \
    X(PFNGLCLEARBUFFERFVPROC, glClearBufferfv)                     \
    X(PFNGLBINDBUFFERBASEPROC, glBindBufferBase)                   \
    X(PFNGLGETUNIFORMBLOCKINDEXPROC, glGetUniformBlockIndex)       \
    X(PFNGLUNIFORMBLOCKBINDINGPROC, glUniformBlockBinding)         \
    X(PFNGLFENCESYNCPROC, glFenceSync)                             \
    X(PFNGLCLIENTWAITSYNCPROC, glClientWaitSync)                   \
    X(PFNGLDELETESYNCPROC, glDeleteSync)

struct GLFuncs
{
#define GTA2GL_DECLARE(type, name) type name;
    GTA2GL_FUNCS(GTA2GL_DECLARE)
#undef GTA2GL_DECLARE

    typedef void* (*Loader)(const char* name);
    // Returns the name of the first missing function, or NULL when all were found.
    const char* Load(Loader loader);
};
