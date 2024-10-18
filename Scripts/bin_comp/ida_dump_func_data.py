from idautils import *
from idaapi import *
from idc import *
import os

#
# Dumps a csv that contains a record for each function containing:
# function name, function virtual address, function file (exe) offset and function size in bytes
# This should be executed from within IDA.
#  

CURRENT_DIRECTORY = os.path.dirname(os.path.abspath(__file__))

def dump_function_data():
    mapping_csv_path = os.path.join(CURRENT_DIRECTORY, "og_function_data.csv")
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
               

dump_function_data()
print("done!")
