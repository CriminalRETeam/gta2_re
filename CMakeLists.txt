cmake_minimum_required (VERSION 3.5)
project (decomp)

ENABLE_LANGUAGE(ASM_MASM)

SET(CMAKE_WARN_VS6 CACHE BOOL OFF)

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
  Source/3rdParty/mss.h
  Source/types.hpp
  Source/winmain.cpp
  Source/frosty_pasteur_0xC1EA8.hpp
  Source/frosty_pasteur_0xC1EA8.cpp
  Source/map_0x370.hpp
  Source/map_0x370.cpp
  Source/nostalgic_ellis_0x28.hpp
  Source/nostalgic_ellis_0x28.cpp
  Source/fix16.hpp
  Source/fix16.cpp
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
  Source/Bink.hpp
  Source/Bink.cpp
  Source/gbh_graphics.hpp
  Source/gbh_graphics.cpp
  Source/text_0x14.hpp
  Source/text_0x14.cpp
  Source/root_sound.hpp
  Source/root_sound.cpp
  Source/angry_lewin_0x85C.hpp
  Source/angry_lewin_0x85C.cpp
  Source/laughing_blackwell_0x1EB54.hpp
  Source/laughing_blackwell_0x1EB54.cpp
  Source/Game_0x40.hpp
  Source/Game_0x40.cpp
  Source/jolly_poitras_0x2BC0.hpp
  Source/jolly_poitras_0x2BC0.cpp
  Source/goofy_thompson.hpp
  Source/goofy_thompson.cpp
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
  Source/DrawUnk_0xBC.hpp
  Source/DrawUnk_0xBC.cpp
  Source/cool_nash_0x294.hpp
  Source/cool_nash_0x294.cpp
  Source/Car_BC.hpp
  Source/Car_BC.cpp
  Source/Car_10.hpp
  Source/Car_10.cpp
  Source/Ped_Unknown_4.hpp
  Source/Ped_Unknown_4.cpp
  Source/zealous_borg.hpp
  Source/zealous_borg.cpp
  Source/infallible_turing.hpp
  Source/infallible_turing.cpp
  Source/sad_mirzakhani.hpp
  Source/sad_mirzakhani.cpp
  Source/Zones_CA8.hpp
  Source/Zones_CA8.cpp
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
  Source/BurgerKing_67F8B0.hpp
  Source/BurgerKing_67F8B0.cpp
  Source/Garox_2B00.hpp
  Source/Garox_2B00.cpp
  Source/Nanobotz.hpp
  Source/Nanobotz.cpp
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
  Source/jawwie_110.hpp
  Source/jawwie_110.cpp
  Source/Sero_181C.hpp
  Source/Sero_181C.cpp
  Source/Taxi_4.hpp
  Source/Taxi_4.cpp
  Source/TrafficLights_194.hpp
  Source/TrafficLights_194.cpp
  Source/Soula_CC68.hpp
  Source/Soula_CC68.cpp
  Source/Marz_1D7E.hpp
  Source/Marz_1D7E.cpp
  Source/Orca_2FD4.hpp
  Source/Orca_2FD4.cpp
  Source/Monster_808.hpp
  Source/Monster_808.cpp
  Source/Particle_8.hpp
  Source/Particle_8.cpp
  Source/Wolfy_3D4.hpp
  Source/Wolfy_3D4.cpp
  Source/Zheal_D9C.hpp
  Source/Zheal_D9C.cpp
  Source/Snooky_94.hpp
  Source/Snooky_94.cpp
  Source/Maccies_14AC.hpp
  Source/Maccies_14AC.cpp
  Source/Kfc_1E0.hpp
  Source/Kfc_1E0.cpp
  Source/Police_7B8.hpp
  Source/Police_7B8.cpp
  Source/Light_1D4CC.hpp
  Source/Light_1D4CC.cpp
  Source/ChickenLegend_48.hpp
  Source/ChickenLegend_48.cpp
  Source/Hamburger_500.hpp
  Source/Hamburger_500.cpp
  Source/CokeZero_100.hpp
  Source/CokeZero_100.cpp
  Source/Shooey_CC.hpp
  Source/Shooey_CC.cpp
  Source/Tango_28.hpp
  Source/Tango_28.cpp
  Source/Rozza_C88.hpp
  Source/Rozza_C88.cpp
  Source/PurpleDoom.hpp
  Source/PurpleDoom.cpp
  Source/Network_20324.hpp
  Source/Network_20324.cpp
  Source/Mouze_44.hpp
  Source/Mouze_44.cpp
  Source/rng.hpp
  Source/rng.cpp
)

# turn off intrinsic functions so we actually get a call to strcpy, enable stdcall calling convention
# else static methods don't match (and I doubt they manually annotated them with __stdcall)
set_source_files_properties(Source/Network_20324.cpp PROPERTIES COMPILE_FLAGS "/Oi- /Gz")

# global include dirs
include_directories(Source)
include_directories(${decomp_SOURCE_DIR})

add_library (gta2_lib ${gta2_lib_src})
target_compile_definitions(gta2_lib PRIVATE _CRT_SECURE_NO_WARNINGS _CRT_NON_CONFORMING_SWPRINTFS)

add_library (gta2_dll_exports SHARED ${gta2_lib_src})
target_compile_definitions(gta2_dll_exports PRIVATE EXPORT_VARS _CRT_SECURE_NO_WARNINGS _CRT_NON_CONFORMING_SWPRINTFS)
target_link_libraries(gta2_dll_exports mss32 Winmm)

add_library (gta2_dll_imports SHARED ${gta2_lib_src})
target_compile_definitions(gta2_dll_imports PRIVATE IMPORT_VARS EXPORT_FUNCS _CRT_SECURE_NO_WARNINGS _CRT_NON_CONFORMING_SWPRINTFS)
target_link_libraries(gta2_dll_imports gta2_dll_exports mss32 Winmm)

add_executable (decomp_main WIN32
    test.cpp
    StdAfx.cpp
    resources.rc)
target_link_libraries(decomp_main mss32 gta2_lib)

#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")

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

message(STATUS "${MSVC_VERSION}=${${MSVC_VERSION}}")

if (${MSVC_VERSION} GREATER 1200)
   SET ( CMAKE_SHARED_LINKER_FLAGS /MANIFEST:NO )

   if (NOT DEFINED ENV{CI})
       add_custom_command(TARGET decomp_main POST_BUILD
           COMMAND "./compare_matches.bat"
           WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
           COMMENT "Running compare_matches.bat")
   endif()
endif()

#install (TARGETS decomp_main DESTINATION bin)