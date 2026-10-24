// Windows DLL build: a WGL context on the game's own window, which keeps its own message handling.
#include "gl_context.hpp"
#include <windows.h>

namespace
{
typedef HGLRC(WINAPI* TCreateContextAttribs)(HDC, HGLRC, const int*);
typedef BOOL(WINAPI* TSwapInterval)(int);

const int kContextMajor = 0x2091;
const int kContextMinor = 0x2092;
const int kProfileMask = 0x9126;
const int kCoreProfile = 0x1;

class WGLContext : public IGLContext
{
  public:
    WGLContext(HWND hWnd, HDC hDc, HGLRC hRc) : mWnd(hWnd), mDc(hDc), mRc(hRc), mGL(GetModuleHandleA("opengl32.dll"))
    {
    }

    ~WGLContext()
    {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(mRc);
        ReleaseDC(mWnd, mDc);
    }

    void* GetProcAddress(const char* name)
    {
        // wglGetProcAddress only returns extensions and post 1.1 functions; it may return 1..3 or -1 as failure.
        void* p = (void*)wglGetProcAddress(name);
        const INT_PTR v = (INT_PTR)p;
        if (v >= -1 && v <= 3)
        {
            p = (void*)::GetProcAddress(mGL, name);
        }
        return p;
    }

    void GetDrawableSize(int* pW, int* pH)
    {
        RECT rc;
        GetClientRect(mWnd, &rc);
        *pW = rc.right - rc.left;
        *pH = rc.bottom - rc.top;
    }

    void SwapBuffers()
    {
        ::SwapBuffers(mDc);
    }
    void MakeCurrent()
    {
        wglMakeCurrent(mDc, mRc);
    }
    void* NativeWindow()
    {
        return mWnd;
    }

  private:
    HWND mWnd;
    HDC mDc;
    HGLRC mRc;
    HMODULE mGL;
};
} // namespace

IGLContext* CreateGLContextFromNativeWindow(void* pNativeWindow, const char** ppError)
{
    HWND hWnd = (HWND)pNativeWindow;
    HDC hDc = GetDC(hWnd);
    if (!hDc)
    {
        *ppError = "GetDC failed";
        return NULL;
    }
    PIXELFORMATDESCRIPTOR pfd;
    ZeroMemory(&pfd, sizeof(pfd));
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.iLayerType = PFD_MAIN_PLANE;
    // A window keeps its pixel format for life: on a mode change the existing one is reused.
    int format = GetPixelFormat(hDc);
    if (!format)
    {
        format = ChoosePixelFormat(hDc, &pfd);
        if (!format || !SetPixelFormat(hDc, format, &pfd))
        {
            ReleaseDC(hWnd, hDc);
            *ppError = "no OpenGL pixel format for the game window";
            return NULL;
        }
    }

    HGLRC legacy = wglCreateContext(hDc);
    if (!legacy || !wglMakeCurrent(hDc, legacy))
    {
        ReleaseDC(hWnd, hDc);
        *ppError = "wglCreateContext failed";
        return NULL;
    }
    TCreateContextAttribs pCreate = (TCreateContextAttribs)wglGetProcAddress("wglCreateContextAttribsARB");
    HGLRC hRc = NULL;
    if (pCreate)
    {
        const int attribs[] = {kContextMajor, 3, kContextMinor, 3, kProfileMask, kCoreProfile, 0};
        hRc = pCreate(hDc, NULL, attribs);
    }
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(legacy);
    if (!hRc || !wglMakeCurrent(hDc, hRc))
    {
        if (hRc)
        {
            wglDeleteContext(hRc);
        }
        ReleaseDC(hWnd, hDc);
        *ppError = "no OpenGL 3.3 core context";
        return NULL;
    }
    // The game paces itself.
    TSwapInterval pSwapInterval = (TSwapInterval)wglGetProcAddress("wglSwapIntervalEXT");
    if (pSwapInterval)
    {
        pSwapInterval(0);
    }
    return new WGLContext(hWnd, hDc, hRc);
}

IGLContext* CreateGLContextWindow(const char*, int, int, const char** ppError)
{
    *ppError = "own windows need the SDL platform layer";
    return NULL;
}
