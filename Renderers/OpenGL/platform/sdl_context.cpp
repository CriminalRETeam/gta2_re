#include "gl_context.hpp"
#include <SDL.h>
#include <stdio.h>

namespace
{
// The game owns the message loop; events SDL sees through its window hook are dropped.
int SDLCALL DropAllEvents(void*, SDL_Event*)
{
    return 0;
}

class SDLGLContext : public IGLContext
{
  public:
    SDLGLContext(SDL_Window* pWindow, SDL_GLContext ctx, void* pNative, bool owned) :
        mWindow(pWindow), mContext(ctx), mNative(pNative), mOwned(owned)
    {
    }

    ~SDLGLContext()
    {
        SDL_GL_MakeCurrent(mWindow, NULL);
        SDL_GL_DeleteContext(mContext);
        SDL_DestroyWindow(mWindow);
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
    }

    void* GetProcAddress(const char* name)
    {
        return SDL_GL_GetProcAddress(name);
    }
    void GetDrawableSize(int* pW, int* pH)
    {
        SDL_GL_GetDrawableSize(mWindow, pW, pH);
    }
    void SwapBuffers()
    {
        SDL_GL_SwapWindow(mWindow);
    }
    void MakeCurrent()
    {
        SDL_GL_MakeCurrent(mWindow, mContext);
    }
    void* NativeWindow()
    {
        return mNative;
    }

  private:
    SDL_Window* mWindow;
    SDL_GLContext mContext;
    void* mNative;
    bool mOwned;
};

bool InitVideo(const char** ppError)
{
    SDL_SetHint(SDL_HINT_VIDEO_FOREIGN_WINDOW_OPENGL, "1");
    SDL_SetHint(SDL_HINT_WINDOWS_DISABLE_THREAD_NAMING, "1");
    // The game owns process termination; SDL must not install SIGINT/SIGTERM handlers.
    SDL_SetHint(SDL_HINT_NO_SIGNAL_HANDLERS, "1");
    if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0)
    {
        *ppError = SDL_GetError();
        return false;
    }
    SDL_SetEventFilter(DropAllEvents, NULL);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);
    return true;
}

IGLContext* Finish(SDL_Window* pWindow, void* pNative, bool owned, const char** ppError)
{
    if (!pWindow)
    {
        *ppError = SDL_GetError();
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
        return NULL;
    }
    SDL_GLContext ctx = SDL_GL_CreateContext(pWindow);
    if (!ctx)
    {
        *ppError = SDL_GetError();
        SDL_DestroyWindow(pWindow);
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
        return NULL;
    }
    SDL_GL_MakeCurrent(pWindow, ctx);
    // The game paces itself.
    SDL_GL_SetSwapInterval(0);
    return new SDLGLContext(pWindow, ctx, pNative, owned);
}
} // namespace

IGLContext* CreateGLContextFromNativeWindow(void* pNativeWindow, const char** ppError)
{
    if (!InitVideo(ppError))
    {
        return NULL;
    }
    return Finish(SDL_CreateWindowFrom(pNativeWindow), pNativeWindow, false, ppError);
}

IGLContext* CreateGLContextWindow(const char* title, int w, int h, const char** ppError)
{
    if (!InitVideo(ppError))
    {
        return NULL;
    }
    SDL_Window* pWindow =
        SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    return Finish(pWindow, NULL, true, ppError);
}
