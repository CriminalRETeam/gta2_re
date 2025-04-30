import idc
import idaapi
import os
import re
import json

# this script will name everything in your IDB for the new/recompiled executable, IDA won't load the PDB
# because the PDB version produced by MSVC6 is too old :)

# run msvc_dump_new_data.py 1st to extract data from the msvc6 map file into new_data.json 1st

def is_unnamed(name: str) -> bool:
    return name.startswith("sub_") or name.startswith("byte_") or name.startswith("word_") or name.startswith("dword_") or name.startswith("stru_") or name.startswith("off_") or name.startswith("asc_")

def rename_internal(name, ea):
    ida_name = idc.get_name(ea)
    
    if ida_name == "" or name == ida_name:
        return 0
    
    if idaapi.set_name(ea, name):
        print(f"renamed {ida_name} to {name}")
        return 1

    print(f"failed to rename {ida_name} to {name}")
    return 0

def rename(new_data, rename_funcs = True, rename_vars = False) -> None:
    rename_count = 0

    if rename_vars:
        for var in new_data["variables"]:
            rename_count += rename_internal(var["name"], int(var["og_address"], 16))

    if rename_funcs:
        for func in new_data["functions"]:
            if func["og_addr"] == "?":
                continue
            rename_count += rename_internal(func["mangled_name"], int(func["og_addr"], 16))

    print(f"renamed {rename_count} items")
        

def main():
    script_path = os.path.dirname(os.path.realpath(__file__))
    with open(script_path + "/../bin_comp/new_data.json", "rt") as file:
        new_data = json.load(file)

    rename(new_data)
    print("done!")

if __name__ == "__main__":
    main()
