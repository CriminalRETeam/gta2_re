import json
import idaapi
import os
import enum
import sys

class GameVersion(enum.Enum):
    unknown = 0
    v96f = 1
    v105 = 2

def rename_function(hex_addr, name):
    if hex_addr is not None and "sub_" not in name:
        final_name = f"{name}_{hex_addr[2:].upper()}"
        idaapi.set_name(int(hex_addr, 16), final_name)
        print(f"rename function at address {hex_addr} to {final_name}")

def main():
    idb_name = idaapi.get_root_filename()
    current_version = GameVersion.unknown
    functions = list()

    if idb_name == "9.6f.exe":
        current_version = GameVersion.v96f
    elif idb_name == "10.5.exe":
        current_version = GameVersion.v105
    else:
        print(f"unknown game version: {idb_name}")
        sys.exit(1)

    if not os.path.exists("functions_data.json"):
        print("functions_data.json doesn't exist!")
        sys.exit(1)
    
    with open("functions_data.json", "rt") as file:
        functions = json.load(file)

    for func in functions:
        if current_version == GameVersion.v96f:
            rename_function(func["v96f_address"], func["name"])
        elif current_version == GameVersion.v105:
            rename_function(func["v105_address"], func["name"])

if __name__ == "__main__":
    main()
    print("done!")