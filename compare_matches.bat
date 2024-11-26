@echo off
rem checks functions marked as matching actually match for a windows build
python vc6_setup.py
call build.bat
SET PATH=%ORIGINAL_PATH%
cd ../Scripts/bin_comp
python download_exe.py
python msvc_dump_func_data.py
python compare_all_functions.py
cd ../..
