import idc
import idaapi
import os
import re
from enum import IntEnum

# this script will set the colours of functions if they are matched/partial impl or stubbed

# run msvc_dump_func_data.py 1st to extract data from the msvc6 map file/og executable into new_function_data.csv 1st

# in BGR format
class FuncColor(IntEnum):
    Matched = 0x0
    Stubbed = 0x3e4a4a
    CoverageTrace = 0x14106b

def load_csv_file(filename):
    ret = []
    with open(filename) as file:
        lines = [line.rstrip() for line in file]
        for line in lines:
            ret.append(line.split(","))        
    return ret

def load_file(filename):
    lines = []
    with open(filename) as file:
        lines = [line.rstrip() for line in file]
    return lines

def recolor_all_functions(from_rgb: int, to_bgr: int):
    """recolors all functions that match from_rgb to to_bgr"""
    for segment in Segments():
        for ea in Functions(segment, get_segm_end(segment)):
            func_color = hex(idc.get_color(ea, idc.CIC_FUNC))
            if func_color == hex(from_rgb):
                idc.set_color(ea, idc.CIC_FUNC, to_bgr)

def ida_set_function_colour(function_address, status):
    # NOTE: IDA's set_color is in BGR and get_color is RGB
    if status == "0x1": # matched
        idc.set_color(function_address, idc.CIC_FUNC, FuncColor.Matched)
    elif status == "0x0": # stubbed
        idc.set_color(function_address, idc.CIC_FUNC, FuncColor.Stubbed)
    elif status == "0x3": # coverage
        idc.set_color(function_address, idc.CIC_FUNC, FuncColor.CoverageTrace)

def color_functions(repo_func_and_var_names, coverage_funcs_addrs) -> None:
    #   csv_file.write(f"{parts[1]},{hex(func_va)},{hex(func_fo)},{hex(ogAddr)},{hex(funcStatus)}\n")
    for csv_data in repo_func_and_var_names:
        name = csv_data[0]
        status = csv_data[4]
        if status != "?":
            ea = int(csv_data[3], 16)
            #print(hex(ea) + " status is " + status)
            if status == "0x0" and ea in coverage_funcs_addrs:
                # stubbed and covered, let coverage win
                status = "0x3" # fake status :')
            ida_set_function_colour(ea, status)
        

def main():
    script_path = os.path.dirname(os.path.realpath(__file__))
    map_data = load_csv_file(script_path + "/../bin_comp/new_function_data.csv")
    coverage_funcs = load_file(script_path + "/../bin_comp/coverage_trace_funcs.txt")
    coverage_funcs_addrs = []
    for c in coverage_funcs:
        coverage_funcs_addrs.append(int(c, 16))
    color_functions(map_data, coverage_funcs_addrs)
    #recolor_all_functions(0x42f5e3, 0x14106b) # bright yellow to a more readable red
    print("done!")

if __name__ == "__main__":
    main()
