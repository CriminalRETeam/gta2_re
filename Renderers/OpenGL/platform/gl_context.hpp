// The only platform specific piece: a window with an OpenGL 3.3 core context.
#pragma once

class IGLContext
{
  public:
    virtual ~IGLContext()
    {
    }
    virtual void* GetProcAddress(const char* name) = 0;
    virtual void GetDrawableSize(int* pW, int* pH) = 0;
    virtual void SwapBuffers() = 0;
    virtual void MakeCurrent() = 0;
    // Native handle of the window (HWND on Windows), NULL if the context owns an SDL window.
    virtual void* NativeWindow() = 0;
};

// Adopts an existing native window (the game's HWND) and creates the GL context on it.
IGLContext* CreateGLContextFromNativeWindow(void* pNativeWindow, const char** ppError);

// Creates its own window (native port). Not used by the DLLs.
IGLContext* CreateGLContextWindow(const char* title, int w, int h, const char** ppError);
