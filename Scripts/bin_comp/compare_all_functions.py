import sys

# ?first_zone_by_type_4DF1D0@Map_0x370@@QAEPAUgmp_map_zone@@E@Z,0x4030d0,0x30d0,0x4df1d0,0x1
# mangled_name, v addr, file offset, og addr, match status
# new_function_data.csv

# Map_0x370::first_zone_by_type_4DF1D0,0x4df1d0,0xdf1d0,0x69
# func name, v addr, len
# og_function_data.csv

import compare_function

def load_csv_file(filename):
    ret = []
    with open(filename) as file:
        lines = [line.rstrip() for line in file]
        for line in lines:
            ret.append(line.split(","))        
    return ret

def calc_funcs_to_check_match(new_data, old_data):
    ret = []
    for new_rec in new_data:
        if new_rec[4] == "0x1":
            # found a func record that is supposed to be impl'd as matching, find the og data
            # to get the func size
            for og_rec in old_data:
                if int(og_rec[1], 16) == int(new_rec[3], 16):
                    # mangled name, new func offset, og func offfset, og func size
                    ret.append([og_rec[0], new_rec[2], og_rec[2], og_rec[3]])
                    break
    return ret

def check_funcs_match():
    new_data = load_csv_file("new_function_data.csv")
    old_data = load_csv_file("og_function_data.csv")
    funcs_to_check = calc_funcs_to_check_match(new_data, old_data)
    ok_funcs = []
    fail_funcs = []
    for func in funcs_to_check:
        if compare_function.compare_function("../../build_vc6/Release/decomp_main.exe", "10.5.exe", func[0], int(func[1], 16), int(func[2], 16), int(func[3], 16)):
            ok_funcs.append(func[0] + " OK!")
        else:
            fail_funcs.append(func[0] + " FAIL!")
    
    for ok_func in ok_funcs:
        print(ok_func)
    
    for fail_func in fail_funcs:
        print(fail_func)

    print("[" + str(len(ok_funcs)) + "/" + str(len(funcs_to_check)) + "] funcs OK")
    return len(fail_funcs) == 0

if check_funcs_match():
    sys.exit(0)
else:
    sys.exit(1)
