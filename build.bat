@echo off
reg import vscommondir.reg
call set_vc6
mkdir build_vc6
cd build_vc6
rem echo %PATH%
cmake.exe -DCMAKE_MAKE_PROGRAM=jom.exe -DCMAKE_VERBOSE_MAKEFILE:BOOL=OFF -DCMAKE_BUILD_TYPE=Release .. -G"NMake Makefiles JOM"
rem cmake.exe -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON .. -G"Visual Studio 6"
rem msdev decomp.dsw /MAKE "decomp_main - Win32 Release"
rem msdev decomp.dsw /MAKE "gta2_dll_exports - Win32 Release"
rem msdev decomp.dsw /MAKE "gta2_dll_imports - Win32 Release"
cmake --build . --target all
