cmake_minimum_required (VERSION 3.5)
project (decomp)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

ENABLE_LANGUAGE(ASM_MASM)

SET(CMAKE_WARN_VS6 CACHE BOOL OFF)

add_subdirectory(3rdParty/GTA2Hax)

add_library (mss32 SHARED
    Source/3rdParty/mss.h
    Source/3rdParty/mss.cpp
    )
target_compile_definitions(mss32 PRIVATE BUILD_MSS)

#add_custom_command (
#    TARGET mss32
#    POST_BUILD
#    COMMAND del $(TargetPath)
#)

# we build the code 3 times as:
# a static lib where by we link it to an exe that is supposed to match the OG as closely as possible
# a dll that contains the global vars as being exported and no functions exported
# a dll that contains the global vars as being imported and all functions exported
# this is so that for the hook dll we can redirect importing the vars from the 2nd copy of the dll
# to the actual executable data/vars
set(gta2_lib_src
  Source/Pool.hpp
  Source/angle.cpp
  Source/angle.hpp
  Source/crt_stubs.cpp
  Source/crt_stubs.hpp
  Source/Globals.cpp
  Source/Globals.hpp
  Source/3rdParty/mss.h
  Source/types.hpp
  Source/winmain.cpp
  Source/frosty_pasteur_0xC1EA8.hpp
  Source/frosty_pasteur_0xC1EA8.cpp
  Source/map_0x370.hpp
  Source/map_0x370.cpp
  Source/nostalgic_ellis_0x28.hpp
  Source/nostalgic_ellis_0x28.cpp
  Source/BitSet32.hpp
  Source/fix16.hpp
  Source/fix16.cpp
  Source/ang16.hpp
  Source/ang16.cpp
  Source/error.hpp
  Source/error.cpp
  Source/memory.hpp
  Source/memory.cpp
  Source/chunk.hpp
  Source/keybrd_0x204.hpp
  Source/keybrd_0x204.cpp
  Source/debug.hpp
  Source/debug.cpp
  Source/cSampleManager.hpp
  Source/cSampleManager.cpp
  Source/sound_obj.hpp
  Source/sound_obj.cpp
  Source/dma_video.hpp
  Source/dma_video.cpp
  Source/magical_germain_0x8EC.hpp
  Source/magical_germain_0x8EC.cpp
  Source/sharp_pare_0x15D8.hpp
  Source/sharp_pare_0x15D8.cpp
  Source/Draw.hpp
  Source/Draw.cpp
  Source/Bink.hpp
  Source/Bink.cpp
  Source/gbh_graphics.hpp
  Source/gbh_graphics.cpp
  Source/text_0x14.hpp
  Source/text_0x14.cpp
  Source/root_sound.hpp
  Source/root_sound.cpp
  Source/Player.hpp
  Source/Player.cpp
  Source/Frontend.hpp
  Source/Frontend.cpp
  Source/Game_0x40.hpp
  Source/Game_0x40.cpp
  Source/jolly_poitras_0x2BC0.hpp
  Source/jolly_poitras_0x2BC0.cpp
  Source/NetPlay.hpp
  Source/NetPlay.cpp
  Source/input.hpp
  Source/input.cpp
  Source/lucid_hamilton.hpp
  Source/lucid_hamilton.cpp
  Source/registry.hpp
  Source/registry.cpp
  Source/file.hpp
  Source/file.cpp
  Source/sharp_bose_0x54.hpp
  Source/sharp_bose_0x54.cpp
  Source/distracted_einstein_0xC.hpp
  Source/distracted_einstein_0xC.cpp
  Source/gtx_0x106C.hpp
  Source/gtx_0x106C.cpp
  Source/Function.hpp
  Source/Function.cpp
  Source/Camera.hpp
  Source/Camera.cpp
  Source/Ped.hpp
  Source/Ped.cpp
  Source/Car_BC.hpp
  Source/Car_BC.cpp
  Source/Car_10.hpp
  Source/Car_10.cpp
  Source/Fix16_Rect.hpp
  Source/Fix16_Rect.cpp
  Source/Fix16_Point.hpp
  Source/Fix16_Point.cpp
  Source/Ped_Unknown_4.hpp
  Source/Ped_Unknown_4.cpp
  Source/zealous_borg.hpp
  Source/zealous_borg.cpp
  Source/infallible_turing.hpp
  Source/infallible_turing.cpp
  Source/sad_mirzakhani.hpp
  Source/sad_mirzakhani.cpp
  Source/Gang.hpp
  Source/Gang.cpp
  Source/youthful_einstein.hpp
  Source/youthful_einstein.cpp
  Source/xenodochial_morse.hpp
  Source/xenodochial_morse.cpp
  Source/sleepy_stonebraker_0x6C.hpp
  Source/sleepy_stonebraker_0x6C.cpp
  Source/miss2_0x11C.hpp
  Source/miss2_0x11C.cpp
  Source/miss2_8.hpp
  Source/miss2_8.cpp
  Source/miss2_xyz.hpp
  Source/BurgerKing_67F8B0.hpp
  Source/BurgerKing_67F8B0.cpp
  Source/Hud.hpp
  Source/Hud.cpp
  Source/MapRenderer.hpp
  Source/MapRenderer.cpp
  Source/Montana.hpp
  Source/Montana.cpp
  Source/Mike_A80.hpp
  Source/Mike_A80.cpp
  Source/Frismo_25C.hpp
  Source/Frismo_25C.cpp
  Source/sprite.hpp
  Source/sprite.cpp
  Source/char.hpp
  Source/char.cpp
  Source/collide.hpp
  Source/collide.cpp
  Source/Phi_8CA8.hpp
  Source/Phi_8CA8.cpp
  Source/Object_5C.hpp
  Source/Object_5C.cpp
  Source/Varrok_7F8.hpp
  Source/Varrok_7F8.cpp
  Source/TileAnim_2.hpp
  Source/TileAnim_2.cpp
  Source/Weapon_8.hpp
  Source/Weapon_8.cpp
  Source/Door_4D4.hpp
  Source/Door_4D4.cpp
  Source/Ambulance_110.hpp
  Source/Ambulance_110.cpp
  Source/PublicTransport.hpp
  Source/PublicTransport.cpp
  Source/Taxi_4.hpp
  Source/Taxi_4.cpp
  Source/TrafficLights_194.hpp
  Source/TrafficLights_194.cpp
  Source/RouteFinder.hpp
  Source/RouteFinder.cpp
  Source/Marz_1D7E.hpp
  Source/Marz_1D7E.cpp
  Source/Orca_2FD4.hpp
  Source/Orca_2FD4.cpp
  Source/CarInfo_808.hpp
  Source/CarInfo_808.cpp
  Source/Particle_8.hpp
  Source/Particle_8.cpp
  Source/Wolfy_3D4.hpp
  Source/Wolfy_3D4.cpp
  Source/Cranes.hpp
  Source/Cranes.cpp
  Source/Crushers.hpp
  Source/Crushers.cpp
  Source/Generators.hpp
  Source/Generators.cpp
  Source/Kfc_1E0.hpp
  Source/Kfc_1E0.cpp
  Source/Police_7B8.hpp
  Source/Police_7B8.cpp
  Source/Light_1D4CC.hpp
  Source/Light_1D4CC.cpp
  Source/Garage_48.hpp
  Source/Garage_48.cpp
  Source/Hamburger_500.hpp
  Source/Hamburger_500.cpp
  Source/ExplodingScore_100.hpp
  Source/ExplodingScore_100.cpp
  Source/Shooey_CC.hpp
  Source/Shooey_CC.cpp
  Source/Firefighters.hpp
  Source/Firefighters.cpp
  Source/Rozza_C88.hpp
  Source/Rozza_C88.cpp
  Source/PurpleDoom.hpp
  Source/PurpleDoom.cpp
  Source/Network_20324.hpp
  Source/Network_20324.cpp
  Source/PedGroup.hpp
  Source/PedGroup.cpp
  Source/rng.hpp
  Source/rng.cpp
  Source/eager_benz.hpp
  Source/eager_benz.cpp
  Source/thirsty_lamarr.hpp
  Source/thirsty_lamarr.cpp
  Source/enums.hpp
  Source/Car_B0.hpp
  Source/Car_B0.cpp
  Source/Miss2_25C.hpp
  Source/Miss2_25C.cpp
  Source/Object_8.hpp
  Source/Object_8.cpp
  Source/Object_8_Pool.hpp
  Source/Object_8_Pool.cpp
  Source/Object_3C_Pool.hpp
  Source/Object_3C_Pool.cpp
  Source/Object_3C.hpp
  Source/Object_3C.cpp
  Source/Object_2C_Pool.hpp
  Source/Object_2C_Pool.cpp
  Source/Particle_4C.hpp
  Source/Particle_4C.cpp
  Source/Police_38.hpp
  Source/Police_38.cpp
  Source/Door_38.hpp
  Source/Door_38.cpp
  Source/Weapon_30.hpp
  Source/Weapon_30.cpp
  Source/ImGuiDebug.hpp
  Source/ImGuiDebug.cpp
)

# global include dirs
include_directories(Source)
include_directories(${decomp_SOURCE_DIR})
include_directories(3rdParty/gta2_re_compile_tools)

# global lib dirs
#link_directories(${CMAKE_SOURCE_DIR}/gta2_lib)

set(dplay_libs
  ${CMAKE_SOURCE_DIR}/3rdParty/gta2_re_compile_tools/VC98/Lib/DPLAYX.lib)

add_library (gta2_lib ${gta2_lib_src})
target_compile_definitions(gta2_lib PRIVATE _CRT_SECURE_NO_WARNINGS _CRT_NON_CONFORMING_SWPRINTFS IMGUI_DLL)
target_link_libraries(gta2_lib)

message(STATUS "${CMAKE_BINARY_DIR}=${${CMAKE_BINARY_DIR}}")

add_library (gta2_dll_exports SHARED ${gta2_lib_src})
target_compile_definitions(gta2_dll_exports PRIVATE EXPORT_VARS _CRT_SECURE_NO_WARNINGS _CRT_NON_CONFORMING_SWPRINTFS IMGUI_DLL)
target_link_libraries(gta2_dll_exports mss32 Winmm ${dplay_libs} dear_imgui)

add_library (gta2_dll_imports SHARED ${gta2_lib_src} Source/WinMainProxy.cpp)
target_compile_definitions(gta2_dll_imports PRIVATE IMPORT_VARS EXPORT_FUNCS _CRT_SECURE_NO_WARNINGS _CRT_NON_CONFORMING_SWPRINTFS IMGUI_DLL)
target_link_libraries(gta2_dll_imports gta2_dll_exports mss32 Winmm ${dplay_libs} dear_imgui)

add_library(HookLoader SHARED 
    Source/HookLoader.cpp
    Source/3rdParty/Detours/src/detours.cpp
    Source/3rdParty/Detours/src/disasm.cpp
    "Source/3rdParty/Manual-DLL-Loader/Source/Manual Loader/Loader.cpp"
)
target_compile_definitions(HookLoader PRIVATE DETOURS_X86 DETOURS_32BIT IMGUI_DLL)

add_executable (ExePatcher Source/ExePatcher.cpp)
if (${MSVC_VERSION} EQUAL 1200)
    target_link_libraries(ExePatcher libcmt.lib libcpmt.lib)
    set_target_properties(ExePatcher PROPERTIES COMPILE_FLAGS "/ML")
    set_target_properties(ExePatcher PROPERTIES LINK_FLAGS "/NODEFAULTLIB:libc.lib")
endif()

add_executable (decomp_main WIN32
    test.cpp
    StdAfx.cpp
    resources.rc)
target_link_libraries(decomp_main mss32 gta2_lib ${dplay_libs} dear_imgui)

if (${MSVC_VERSION} EQUAL 1200)
    target_link_libraries(decomp_main libc.lib libcp.lib)
    target_compile_definitions(decomp_main PRIVATE IMGUI_DLL)
    set_target_properties(decomp_main PROPERTIES LINK_FLAGS "/NODEFAULTLIB:libc.lib")
endif()

if (${MSVC_VERSION} GREATER 1200)
    set_target_properties(decomp_main PROPERTIES
        LINK_FLAGS "/MAP:output.map /OPT:NOICF,NOREF /SAFESEH:NO"
    )
else()
    set_target_properties(decomp_main PROPERTIES
        LINK_FLAGS "/MAP:output.map /OPT:NOICF,NOREF"
    )
endif()

if (${MSVC_VERSION} GREATER 1200)
    set_target_properties(gta2_dll_exports PROPERTIES COMPILE_FLAGS "/Gy")
    set_target_properties(gta2_dll_imports PROPERTIES COMPILE_FLAGS "/Gy")

    set_target_properties(gta2_dll_exports PROPERTIES LINK_FLAGS "/OPT:NOICF,NOREF /SAFESEH:NO /INCREMENTAL:NO" )
    set_target_properties(gta2_dll_imports PROPERTIES LINK_FLAGS "/OPT:NOICF,NOREF /SAFESEH:NO /INCREMENTAL:NO" )
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /OPT:NOICF,NOREF /SAFESEH:NO")
else()
    set_target_properties(gta2_dll_exports PROPERTIES LINK_FLAGS "/OPT:NOICF,NOREF" )
    set_target_properties(gta2_dll_imports PROPERTIES LINK_FLAGS "/OPT:NOICF,NOREF" )
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /OPT:NOICF,NOREF")
endif()


add_executable (cl_proxy Source/cl_proxy.cpp)
if (${MSVC_VERSION} EQUAL 1200)
    set_target_properties(cl_proxy PROPERTIES COMPILE_FLAGS "/ML")
    set_target_properties(cl_proxy PROPERTIES LINK_FLAGS "/NODEFAULTLIB:libc.lib")
    target_link_libraries(cl_proxy  libcmt.lib libcpmt.lib)
endif()

if(${MSVC})
    include(cmake/vc6.cmake)
endif ()

