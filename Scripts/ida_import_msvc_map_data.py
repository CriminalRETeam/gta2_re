import idc
import idaapi
import os
import re

# this script will name everything in your IDB for the new/recompiled executable, IDA won't load the PDB
# because the PDB version produced by MSVC6 is too old :)

# run msvc_dump_func_data.py 1st to extract data from the msvc6 map file into new_function_data.csv 1st

def load_csv_file(filename):
    ret = []
    with open(filename) as file:
        lines = [line.rstrip() for line in file]
        for line in lines:
            ret.append(line.split(","))        
    return ret

def is_unnamed(name: str) -> bool:
    return name.startswith("sub_") or name.startswith("byte_") or name.startswith("word_") or name.startswith("dword_") or name.startswith("stru_") or name.startswith("off_") or name.startswith("asc_")

def rename_ida_functions(repo_func_and_var_names) -> None:
    rename_count = 0
    for csv_data in repo_func_and_var_names:
        name = csv_data[0]
        #if is_unnamed(name):
        #    continue
        
        ea = int(csv_data[1], 16)
        ida_name = idc.get_name(ea)
        
        if ida_name == "" or name == ida_name:
            continue
        
        if idaapi.set_name(ea, name):
            print(f"renamed {ida_name} to {name}")
            rename_count = rename_count + 1
        else:
            print(f"failed to rename {ida_name} to {name}")
        

    print(f"renamed {rename_count} items")
        

def main():
    script_path = os.path.dirname(os.path.realpath(__file__))
    map_data = load_csv_file(script_path + "/bin_comp/new_function_data.csv")
    rename_ida_functions(map_data)
    print("done!")

if __name__ == "__main__":
    main()
