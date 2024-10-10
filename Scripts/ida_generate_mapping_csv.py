from idautils import *
from idaapi import *
from idc import *
import os

CURRENT_DIRECTORY = os.path.dirname(os.path.abspath(__file__))

def generate_mapping_csv():
    mapping_csv_path = os.path.join(CURRENT_DIRECTORY, "mapping.csv")
    with open(mapping_csv_path, "w") as file:
        for segment in Segments():
            for ea in Functions(segment, get_segm_end(segment)):
                func_name = get_func_name(ea)
                if any(substring in func_name for substring in {"crt_init_", "crt_j_init__"}):
                    print(f"skip {func_name}")
                    continue
                func_size = get_func(ea).size()
                file.write(f"{func_name},{hex(ea)},{hex(func_size)}\n")
                #print(f"{func_name},{hex(ea)},{hex(func_size)}")
               
        ''' 
        nimps = idaapi.get_import_module_qty()  
        for i in range(nimps):
            name = idaapi.get_import_module_name(i)
            def imp_cb(ea, name, ordinal):
                if name:
                    file.write(f"{name},{hex(ea)},0x0\n")
                # True -> Continue enumeration
                return True
            idaapi.enum_import_names(i, imp_cb)
        '''
 
generate_mapping_csv()
print("done!")
