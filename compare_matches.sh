# checks functions marked as matching actually match for a linux build
WINEDEBUG=-all wine build.bat || exit 1
cd Scripts/bin_comp
python msvc_dump_func_data.py || exit 1
./venv/bin/python compare_all_functions.py || exit 1
cd ../..
