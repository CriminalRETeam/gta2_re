python setup.py
call set_vc6
mkdir build_vc6
cd build_vc6
cmake .. -G"Visual Studio 6"
msdev decomp.dsw /MAKE "decomp_main - Win32 Release"
pause
