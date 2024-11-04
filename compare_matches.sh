# checks functions marked as matching actually match for a linux build
WINEDEBUG=-all wine build.bat
cd Scripts/bin_comp
python msvc_dump_func_data.py
./venv/bin/python compare_all_functions.py
cd ../..
