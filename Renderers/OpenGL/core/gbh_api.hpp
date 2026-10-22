// gbh_* entry points. In the Windows render DLL they are exported undecorated (see Makefile).
#pragma once

#include "gta2_api.hpp"

#if defined(_WIN32) && defined(GTA2GL_BUILD_DLL)
    #define GBH_EXPORT extern "C" __declspec(dllexport)
#else
    #define GBH_EXPORT extern "C"
#endif
