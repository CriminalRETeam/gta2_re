python build.py
cp build_vc6/gta2_dll_imports.dll GTA2_data/gta2_dll_imports.dll
cp build_vc6/gta2_dll_exports.dll GTA2_data/gta2_dll_exports.dll
cp build_vc6/HookLoader.dll GTA2_data/HookLoader.dll
cp build_vc6/decomp_main.exe GTA2_data/decomp_main.exe
cd GTA2_data
wine 10.5.new.exe
#wine decomp_main.exe

