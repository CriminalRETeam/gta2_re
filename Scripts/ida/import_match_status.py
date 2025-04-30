import idc
import idaapi
import os
import re
from enum import IntEnum
import json

# this script will set the colours of functions if they are matched/partial impl or stubbed

# run msvc_dump_new_data.py 1st to extract data from the msvc6 map file/og executable into new_data.json 1st

# in BGR format
class FuncColor(IntEnum):
    Matched = 0x0
    Stubbed = 0x3e4a4a
    CoverageTrace = 0x14106b
    CoverageTrace2 = 0x065e03

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
    elif status == "0x3": # coverage 1
        idc.set_color(function_address, idc.CIC_FUNC, FuncColor.CoverageTrace)
    elif status == "0x4": # coverage 2
        idc.set_color(function_address, idc.CIC_FUNC, FuncColor.CoverageTrace2)

def color_functions(new_data, coverage_funcs_addrs, coverage_funcs_addrs2) -> None:
    #   csv_file.write(f"{parts[1]},{hex(func_va)},{hex(func_fo)},{hex(ogAddr)},{hex(funcStatus)}\n")
    for func in new_data["functions"]:
        name = func["mangled_name"]
        status = func["func_status"]
        if status != "?":
            ea = int(func["og_addr"], 16)
            #print(hex(ea) + " status is " + status)
            if status == "0x0" and ea in coverage_funcs_addrs2: # coverage2 wins over 1
                # stubbed and covered, let coverage win
                status = "0x4" # fake status :')
            elif status == "0x0" and ea in coverage_funcs_addrs:
                # stubbed and covered, let coverage win
                status = "0x3" # fake status :')
            
            ida_set_function_colour(ea, status)
        

def main():
    script_path = os.path.dirname(os.path.realpath(__file__))
    with open(script_path + "/../bin_comp/new_data.json", "rt") as file:
        new_data = json.load(file)

    coverage_funcs = load_file(script_path + "/../bin_comp/coverage_trace_funcs.txt")
    coverage_funcs_addrs = []
    for c in coverage_funcs:
        coverage_funcs_addrs.append(int(c, 16))

    # ugly and nasty but unlikely to have more than 2 coverage files so meh :P
    coverage_funcs2 = load_file(script_path + "/../bin_comp/coverage_frontend.txt")
    coverage_funcs_addrs2 = []
    for c in coverage_funcs2:
        coverage_funcs_addrs2.append(int(c, 16))

    color_functions(new_data, coverage_funcs_addrs, coverage_funcs_addrs2)
    #recolor_all_functions(0x42f5e3, 0x14106b) # bright yellow to a more readable red
    print("done!")

if __name__ == "__main__":
    main()
