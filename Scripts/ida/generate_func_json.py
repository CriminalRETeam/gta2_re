import idc
import idautils
import json
import enum
import idaapi
import sys
import os

class GameVersion(enum.Enum):
    unknown = 0
    v96f = 1
    v105 = 2

# the function name might be File::Read_4A6D90 so lets try to grab the File::Read part
def extract_name(func_name: str):
    if "sub_" not in func_name and "_" in func_name and len(func_name) >= 10:
        return func_name[:-7]
    return func_name

# TODO: handle updating function names. the function name should be based on version 10.5
def update_function(game_version: GameVersion, ea, ida_func_name: str, functions: list):
    hex_addr = hex(ea)

    for func in functions:
        if func["name"] != extract_name(ida_func_name):
            continue

        print(f"updating data to {hex_addr}")
        if game_version == GameVersion.v96f:
            func["v96f_address"] = hex_addr
        elif game_version == GameVersion.v105:
            func["v105_address"] = hex_addr

    return functions

def add_function(game_version: GameVersion, ea, ida_func_name: str, functions):
    hex_addr = hex(ea)

    function = {
        "name": extract_name(ida_func_name),
        "v96f_address": hex_addr if game_version == GameVersion.v96f else None,
        "v105_address": hex_addr if game_version == GameVersion.v105 else None,
    }
    functions.append(function)
    return functions

def main():
    idb_name = idaapi.get_root_filename()
    current_version = GameVersion.unknown
    data_exists = os.path.exists("functions_data.json")
    functions = list()

    if data_exists:
        print("data exists")
        with open("functions_data.json", "rt") as file:
            functions = json.load(file)

    if idb_name == "9.6f.exe":
        current_version = GameVersion.v96f
    elif idb_name == "10.5.exe":
        current_version = GameVersion.v105
    else:
        print(f"unknown game version: {idb_name}")
        sys.exit(1)

    for segment in idautils.Segments():
        for ea in idautils.Functions(segment, idc.get_segm_end(segment)):
            ida_func_name = idc.get_func_name(ea)
            if "nullsub" in ida_func_name or "crt_j_init__" in ida_func_name or "crt_init_" in ida_func_name:
                continue

            if data_exists:
                functions = update_function(current_version, ea, ida_func_name, functions)
            else:
                functions = add_function(current_version, ea, ida_func_name, functions)

    with open("functions_data.json", "wt") as file:
        file.write(json.dumps(functions, indent=4))

if __name__ == "__main__":
    main()