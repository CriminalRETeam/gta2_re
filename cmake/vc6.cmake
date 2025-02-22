

# turn off intrinsic functions so we actually get a call to strcpy, enable stdcall calling convention
# else static methods don't match (and I doubt they manually annotated them with __stdcall)
set_source_files_properties(Source/Network_20324.cpp PROPERTIES COMPILE_FLAGS "/Oi- /Gz")

set_source_files_properties(Source/miss2_0x11C.cpp PROPERTIES COMPILE_FLAGS "/GX-")
set_source_files_properties(Source/sharp_bose_0x54.cpp PROPERTIES COMPILE_FLAGS "/GX-")

# NOICF is required because skip_ovly_5AAE20 and skip_psxt_5AAE30 have identical bodies and the linker will make them one and the same
# however the real binary does have 2 copies. NOREF keeps functions that are not yet called so we can diff the content against the original.
if (${MSVC_VERSION} GREATER 1200)
    set (CMAKE_EXE_LINKER_FLAGS "/MAP:output.map /OPT:NOICF,NOREF /SAFESEH:NO" )
else()
    set (CMAKE_EXE_LINKER_FLAGS "/MAP:output.map /OPT:NOICF,NOREF" )
endif()
set (CMAKE_SHARED_LINKER_FLAGS "/OPT:NOICF,NOREF")

set(CompilerFlags
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
)

if (${MSVC_VERSION} EQUAL 1200)
    foreach(CompilerFlag ${CompilerFlags})
      string(REPLACE "/MD" "/ML /W3 /GX" ${CompilerFlag} "${${CompilerFlag}}")
    endforeach()

    foreach(CompilerFlag ${CompilerFlags})
      string(REPLACE "/Zi" "/Z7" ${CompilerFlag} "${${CompilerFlag}}")
    endforeach()

    foreach(CompilerFlag ${CompilerFlags})
      string(REPLACE "/Ob2" "" ${CompilerFlag} "${${CompilerFlag}}")
    endforeach()

    foreach(CompilerFlag ${CompilerFlags})
      string(REPLACE "/GR" "/EHsc" ${CompilerFlag} "${${CompilerFlag}}")
    endforeach()
endif()

if (${MSVC_VERSION} GREATER 1200)
   SET ( CMAKE_SHARED_LINKER_FLAGS /MANIFEST:NO )

   if (NOT DEFINED ENV{CI})
       add_custom_command(TARGET decomp_main POST_BUILD
               COMMAND python build.py
               WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
               COMMENT "Running build.py (VC6 build)")
   endif()
endif()

message(STATUS "${MSVC_VERSION}=${${MSVC_VERSION}}")
