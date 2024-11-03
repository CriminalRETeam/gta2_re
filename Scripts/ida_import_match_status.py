import idc
import idaapi
import os
import re

# this script will set the colours of functions if they are matched/partial impl or stubbed

# run msvc_dump_func_data.py 1st to extract data from the msvc6 map file/og executable into new_function_data.csv 1st

def load_csv_file(filename):
    ret = []
    with open(filename) as file:
        lines = [line.rstrip() for line in file]
        for line in lines:
            ret.append(line.split(","))        
    return ret

def ida_set_function_colour(function_address, colour):
    idc.set_color(function_address, CIC_FUNC, colour)

def rename_ida_functions(repo_func_and_var_names) -> None:
    rename_count = 0
    #   csv_file.write(f"{parts[1]},{hex(func_va)},{hex(func_fo)},{hex(ogAddr)},{hex(funcStatus)}\n")
    for csv_data in repo_func_and_var_names:
        name = csv_data[0]
        
        #ida_name = idc.get_name(ea)
        
        #if ida_name == "" or name == ida_name:
        #    continue
        
        #if idaapi.set_name(ea, name):
        #    print(f"renamed {ida_name} to {name}")
        #    rename_count = rename_count + 1
        #else:
        #    print(f"failed to rename {ida_name} to {name}")
        status = csv_data[4]
        if status != "?":
            ea = int(csv_data[3], 16)
            print(hex(ea) + " status is " + status)
            if status == "0x1":
                # set to matched colour
                ida_set_function_colour(ea, 0x0)

    print(f"renamed {rename_count} items")
        

def main():
    script_path = os.path.dirname(os.path.realpath(__file__))
    map_data = load_csv_file(script_path + "/bin_comp/new_function_data.csv")
    rename_ida_functions(map_data)
    print("done!")

if __name__ == "__main__":
    main()
