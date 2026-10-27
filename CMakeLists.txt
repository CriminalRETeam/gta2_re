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

add_library (binkw32 SHARED
    Source/3rdParty/bink.h
    Source/3rdParty/bink.cpp
    )
target_compile_definitions(binkw32 PRIVATE BUILD_BINK)

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
  Source/crt_stubs.cpp
  Source/crt_stubs.hpp
  Source/Globals.cpp
  Source/Globals.hpp
  Source/3rdParty/mss.h
  Source/types.hpp
  Source/winmain.cpp
  Source/ScriptManager.hpp
  Source/ScriptManager.cpp
  Source/map_0x370.hpp
  Source/map_0x370.cpp
  Source/Light.hpp
  Source/Light.cpp
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
  Source/KeyboardLayout.hpp
  Source/KeyboardLayout.cpp
  Source/debug.hpp
  Source/debug.cpp
  Source/cSampleManager.hpp
  Source/cSampleManager.cpp
  Source/sound_obj.hpp
  Source/sound_obj.cpp
  Source/dma_video.hpp
  Source/dma_video.cpp
  Source/KanjiFont.hpp
  Source/KanjiFont.cpp
  Source/TextureCache.hpp
  Source/TextureCache.cpp
  Source/Draw.hpp
  Source/Draw.cpp
  Source/Bink.hpp
  Source/Bink.cpp
  Source/gbh_graphics.hpp
  Source/gbh_graphics.cpp
  Source/TextTable.hpp
  Source/TextTable.cpp
  Source/root_sound.hpp
  Source/root_sound.cpp
  Source/Player.hpp
  Source/Player.cpp
  Source/Frontend.hpp
  Source/Frontend.cpp
  Source/Game.hpp
  Source/Game.cpp
  Source/PlyDat.hpp
  Source/PlyDat.cpp
  Source/NetPlay.hpp
  Source/NetPlay.cpp
  Source/network_data.hpp
  Source/input.hpp
  Source/input.cpp
  Source/GameSession.hpp
  Source/GameSession.cpp
  Source/registry.hpp
  Source/registry.cpp
  Source/file.hpp
  Source/file.cpp
  Source/FpsCounter.hpp
  Source/FpsCounter.cpp
  Source/ProfilerTimer.hpp
  Source/ProfilerTimer.cpp
  Source/gtx_0x106C.hpp
  Source/gtx_0x106C.cpp
  Source/Function.hpp
  Source/Function.cpp
  Source/Camera.hpp
  Source/Camera.cpp
  Source/Ped.hpp
  Source/Ped.cpp
  Source/Car.hpp
  Source/Car.cpp
  Source/CarAI.hpp
  Source/CarAI.cpp
  Source/Car_10.hpp
  Source/Car_10.cpp
  Source/Fix16_Rect.hpp
  Source/Fix16_Rect.cpp
  Source/Fix16_Point.hpp
  Source/Fix16_Point.cpp
  Source/Fix16_Vec.hpp
  Source/Ped_List.hpp
  Source/Ped_List.cpp
  Source/PlayerCrimeStats.hpp
  Source/PlayerCrimeStats.cpp
  Source/SoundObject.hpp
  Source/SoundObject.cpp
  Source/BonusTracker.hpp
  Source/BonusTracker.cpp
  Source/Gang.hpp
  Source/Gang.cpp
  Source/TagGame.hpp
  Source/TagGame.cpp
  Source/CreditsText.hpp
  Source/CreditsText.cpp
  Source/CreditsLine.hpp
  Source/CreditsLine.cpp
  Source/ScriptThread.hpp
  Source/ScriptThread.cpp
  Source/ScriptGosubStack.hpp
  Source/ScriptGosubStack.cpp
  Source/miss2_xyz.hpp
  Source/InputRecorder_67F8B0.hpp
  Source/InputRecorder_67F8B0.cpp
  Source/Hud.hpp
  Source/Hud.cpp
  Source/MapRenderer.hpp
  Source/MapRenderer.cpp
  Source/SpriteRenderer.hpp
  Source/SpriteRenderer.cpp
  Source/FrameProfiler.hpp
  Source/FrameProfiler.cpp
  Source/ScriptStackFramePool.hpp
  Source/ScriptStackFramePool.cpp
  Source/sprite.hpp
  Source/sprite.cpp
  Source/char.hpp
  Source/char.cpp
  Source/Char_Pool.hpp
  Source/Char_Pool.cpp
  Source/collide.hpp
  Source/collide.cpp
  Source/ObjectDefinitions.hpp
  Source/ObjectDefinitions.cpp
  Source/Object_5C.hpp
  Source/Object_5C.cpp
  Source/PedRefTable.hpp
  Source/PedRefTable.cpp
  Source/TileAnimManager.hpp
  Source/TileAnimManager.cpp
  Source/WeaponManager.hpp
  Source/WeaponManager.cpp
  Source/DoorPool.hpp
  Source/DoorPool.cpp
  Source/AmbulanceService.hpp
  Source/AmbulanceService.cpp
  Source/PublicTransport.hpp
  Source/PublicTransport.cpp
  Source/TaxiList.hpp
  Source/TaxiList.cpp
  Source/TrafficLights.hpp
  Source/TrafficLights.cpp
  Source/RouteFinder.hpp
  Source/RouteFinder.cpp
  Source/PatrolRoutePool.hpp
  Source/PatrolRoutePool.cpp
  Source/PathFinder.hpp
  Source/PathFinder.cpp
  Source/CarInfoTable.hpp
  Source/CarInfoTable.cpp
  Source/ParticleManager.hpp
  Source/ParticleManager.cpp
  Source/Explosion.hpp
  Source/Explosion.cpp
  Source/Cranes.hpp
  Source/Cranes.cpp
  Source/Crushers.hpp
  Source/Crushers.cpp
  Source/Generators.hpp
  Source/Generators.cpp
  Source/EmergencyCrewPool.hpp
  Source/EmergencyCrewPool.cpp
  Source/Police.hpp
  Source/Police.cpp
  Source/LightPool.hpp
  Source/LightPool.cpp
  Source/Garage.hpp
  Source/Garage.cpp
  Source/CarChaseTaskTable.hpp
  Source/CarChaseTaskTable.cpp
  Source/ExplodingScore_100.hpp
  Source/ExplodingScore_100.cpp
  Source/CrimeReportQueue.hpp
  Source/CrimeReportQueue.cpp
  Source/Firefighters.hpp
  Source/Firefighters.cpp
  Source/CollisionSoundQueue.hpp
  Source/CollisionSoundQueue.cpp
  Source/SpriteGrid.hpp
  Source/SpriteGrid.cpp
  Source/NetUi.hpp
  Source/NetUi.cpp
  Source/PedGroup.hpp
  Source/PedGroup.cpp
  Source/rng.hpp
  Source/rng.cpp
  Source/PlayerScoreTracker.hpp
  Source/PlayerScoreTracker.cpp
  Source/RollingDigitCounter.hpp
  Source/RollingDigitCounter.cpp
  Source/enums.hpp
  Source/CarPhysics.hpp
  Source/CarPhysics.cpp
  Source/MissionCleanupList.hpp
  Source/MissionCleanupList.cpp
  Source/Object_8.hpp
  Source/Object_8.cpp
  Source/Object_8_Pool.hpp
  Source/Object_8_Pool.cpp
  Source/Object_3C_Pool.hpp
  Source/Object_3C_Pool.cpp
  Source/Object_3C.hpp
  Source/Object_3C.cpp
  Source/ObjectPool.hpp
  Source/ObjectPool.cpp
  Source/Particle_4C.hpp
  Source/Particle_4C.cpp
  Source/Police_38.hpp
  Source/Police_38.cpp
  Source/Door.hpp
  Source/Door.cpp
  Source/Weapon.hpp
  Source/Weapon.cpp
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
target_link_libraries(gta2_dll_exports mss32 binkw32 Winmm ${dplay_libs} dear_imgui)

add_library (gta2_dll_imports SHARED ${gta2_lib_src} Source/WinMainProxy.cpp)
target_compile_definitions(gta2_dll_imports PRIVATE IMPORT_VARS EXPORT_FUNCS _CRT_SECURE_NO_WARNINGS _CRT_NON_CONFORMING_SWPRINTFS IMGUI_DLL)
target_link_libraries(gta2_dll_imports gta2_dll_exports mss32 binkw32 Winmm ${dplay_libs} dear_imgui)

add_library(HookLoader SHARED 
    Source/HookLoader.cpp
    Source/HookLoader.hpp
    Source/3rdParty/Detours/src/detours.cpp
    Source/3rdParty/Detours/src/disasm.cpp
    "Source/3rdParty/Manual-DLL-Loader/Source/Manual Loader/Loader.cpp"
)
target_compile_definitions(HookLoader PRIVATE DETOURS_X86 DETOURS_32BIT IMGUI_DLL)

# The bot (docs/BOT_DESIGN.md): non original code in its own library, linked into decomp_main only.
# It reaches the game through globals, EXPORT methods and Detours; no MATCH_FUNC changes.
set(gta2_bot_src
  Source/Bot/BotCompat.hpp
  Source/Bot/Bot.hpp
  Source/Bot/Bot.cpp
  Source/Bot/BotHeap.hpp
  Source/Bot/BotHeap.cpp
  Source/Bot/BotLog.hpp
  Source/Bot/BotLog.cpp
  Source/Bot/BotOptions.hpp
  Source/Bot/BotOptions.cpp
  Source/Bot/BotRng.hpp
  Source/Bot/HeapTracker.hpp
  Source/Bot/HeapTracker.cpp
  Source/Bot/Hooks.hpp
  Source/Bot/Hooks.cpp
  Source/Bot/Snapshot.hpp
  Source/Bot/NavFoot.hpp
  Source/Bot/NavFoot.cpp
  Source/Bot/BotAction.hpp
  Source/Bot/BotAction.cpp
  Source/Bot/WalkAction.hpp
  Source/Bot/WalkAction.cpp
  Source/Bot/CarActions.hpp
  Source/Bot/CarActions.cpp
  Source/Bot/Goap.hpp
  Source/Bot/Goap.cpp
  Source/Bot/CollectActions.hpp
  Source/Bot/CollectActions.cpp
  Source/Bot/MissionActions.hpp
  Source/Bot/ChaseActions.hpp
  Source/Bot/ChaseActions.cpp
  Source/Bot/Goals.hpp
  Source/Bot/Goals.cpp
  Source/Bot/MissionActions.cpp
  Source/Bot/NavCar.hpp
  Source/Bot/NavCar.cpp
  Source/Bot/Snapshot.cpp
  Source/Bot/WorldModel.hpp
  Source/Bot/WorldModel.cpp
  Source/3rdParty/Detours/src/detours.cpp
  Source/3rdParty/Detours/src/disasm.cpp
)
add_library(gta2_bot STATIC ${gta2_bot_src})
target_compile_definitions(gta2_bot PRIVATE DETOURS_X86 DETOURS_32BIT _CRT_SECURE_NO_WARNINGS _CRT_NON_CONFORMING_SWPRINTFS IMGUI_DLL)

add_executable (ExePatcher Source/ExePatcher.cpp)
if (${MSVC_VERSION} EQUAL 1200)
    target_link_libraries(ExePatcher libcmt.lib libcpmt.lib)
    set_target_properties(ExePatcher PROPERTIES COMPILE_FLAGS "/ML")
    set_target_properties(ExePatcher PROPERTIES LINK_FLAGS "/NODEFAULTLIB:libc.lib /OPT:NOICF,NOREF")
endif()

add_executable (decomp_main WIN32
    test.cpp
    StdAfx.cpp
    resources.rc)
target_link_libraries(decomp_main mss32 binkw32 gta2_lib gta2_bot ${dplay_libs} dear_imgui)

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

