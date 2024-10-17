reg import vscommondir.reg
call set_vc6
mkdir build_vc6
cd build_vc6
echo %PATH%
dir Z:\home\appveyor\projects\gta2-re\3rdParty\gta2_re_compile_tools\cmake-3.5.0-win32-x86\bin
cmake.exe .. -G"Visual Studio 6"
Z:\home\appveyor\projects\gta2-re\3rdParty\gta2_re_compile_tools\cmake-3.5.0-win32-x86\bin\cmake.exe .. -G"Visual Studio 6"
msdev decomp.dsw /MAKE "decomp_main - Win32 Release"
