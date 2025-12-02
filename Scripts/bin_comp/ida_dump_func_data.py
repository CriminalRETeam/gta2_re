from idautils import *
from idaapi import *
from idc import *
import os
import enum
import json
import sys

#
# Dumps a csv that contains a record for each function containing:
# function name, function virtual address, function file (exe) offset and function size in bytes
# This should be executed from within IDA.
#  

CURRENT_DIRECTORY = os.path.dirname(os.path.abspath(__file__))

class GameVersion(enum.Enum):
    unknown = 0
    v96f = 1
    v105 = 2

def dump_function_data(game_version: GameVersion):
    mapping_csv_path = os.path.join(CURRENT_DIRECTORY, "og_function_data_v105.csv" if game_version == GameVersion.v105 else "og_function_data_v96f.csv")
    with open(mapping_csv_path, "w") as file:
        for segment in Segments():
            for ea in Functions(segment, get_segm_end(segment)):
                func_name = get_func_name(ea)
                if any(substring in func_name for substring in {"crt_init_", "crt_j_init__"}):
                    print(f"skip {func_name}")
                    continue

                func_size = get_func(ea).size()
                
                func_fo = idaapi.get_fileregion_offset(ea)


                file.write(f"{func_name},{hex(ea)},{hex(func_fo)},{hex(func_size)}\n")
                print(f"{func_name},{hex(ea)},{hex(func_fo)},{hex(func_size)}")

def main():
    idb_name = idaapi.get_root_filename()
    print("Save dir is " + CURRENT_DIRECTORY + " and IDB name is " + idb_name)
    if idb_name == "9.6f.exe":
        dump_function_data(GameVersion.v96f)
    elif idb_name == "10.5.exe":
        dump_function_data(GameVersion.v105)
    else:
        print(f"unknown game version: {idb_name}")
        sys.exit(1)

    print("done!")

if __name__ == "__main__":
    main()
