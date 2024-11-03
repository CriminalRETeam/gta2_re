@echo off
reg import vscommondir.reg
call set_vc6
mkdir build_vc6
cd build_vc6
rem echo %PATH%
cmake.exe .. -G"NMake Makefiles JOM"
rem msdev decomp.dsw /MAKE "decomp_main - Win32 Release"
rem msdev decomp.dsw /MAKE "gta2_dll_exports - Win32 Release"
rem msdev decomp.dsw /MAKE "gta2_dll_imports - Win32 Release"
cmake.exe --build . --config Release
