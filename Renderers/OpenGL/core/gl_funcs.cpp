#include "gl_funcs.hpp"

const char* GLFuncs::Load(Loader loader)
{
#define GTA2GL_LOAD(type, name) \
    name = (type)loader(#name); \
    if (!name)                  \
    {                           \
        return #name;           \
    }
    GTA2GL_FUNCS(GTA2GL_LOAD)
#undef GTA2GL_LOAD
    return 0;
}
