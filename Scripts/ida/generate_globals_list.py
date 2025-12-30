import os
import sys
import idautils
import idc
from dataclasses import dataclass
from pathlib import Path

REPO_ROOT_FOLDER = Path(__file__).parent.parent.parent
SOURCE_FOLDER = REPO_ROOT_FOLDER / "Source"
FILENAME = "globals.txt"

WANTED_GLOBAL_TYPES = ["Fix16", "Ang16"]

FOUND = "found"

@dataclass(slots=True)
class globals():
    var_type : str
    name : str
    address : str
    init_value : str

def upper_hex(hex_str : str):
    return "0x" + hex_str[2:].upper() if hex_str.startswith("0x") else hex_str.upper()

if False:

    def get_crt_init_list():
        func_list = list()
        for func_ea in idautils.Functions():
            func_name = idc.get_func_name(func_ea)
            if "crt_init_" in func_name:
                func_list.append(func_ea)
        return func_list


    def write_global_vars_list(func_list):
        script_path = os.path.dirname(os.path.realpath(__file__))
        output_path = os.path.join(
            os.path.dirname(script_path), "bin_comp", FILENAME
            )  
        
        #print(output_path)
        #return
        with open(output_path, "w") as file:
            for func_ea in func_list:
                func_name = idc.get_func_name(func_ea)
                file.write(func_name + "\n")




    def parse_globals(func_list):

        globals_list = list()
        #globals_offset_set = set()

        for func_ea in func_list[:5]:
            print(idc.GetDisasm(func_ea))
            idc.get
            #inst = idautils.DecodeInstruction(func_ea)
            #print()
            #iterator = idautils.FuncItems(func_ea)
            #for item in iterator.data_items(): #iterator.addresses():
            #    print(hex(item))

def get_uninit_global(line : str):
    # DEFINE_GLOBAL(Car_214*, gCar_214_705F20, 0x705F20);
    line = line.strip()
    parenthesis_span = slice(line.find('(') + 1, line.rfind(')'))
    arguments = line[parenthesis_span].split(",")

    if (len(arguments) != 3):
        raise Exception(f"More than 3 arguments in DEFINE_GLOBAL: {arguments}")

    global_type = arguments[0].strip()
    global_name = arguments[1].strip()
    global_address = upper_hex(arguments[2].strip())

    return globals(global_type, global_name, global_address, None)
    
def get_inited_global(line : str):
    # DEFINE_GLOBAL_INIT(Fix16, dword_6777A0, Fix16(0x333, 0), 0x6777A0);
    line = line.strip()
    parenthesis_span = slice(line.find('(') + 1, line.rfind(')'))
    arguments = line[parenthesis_span].split(",")

    assert len(arguments) == 4

    global_type = arguments[0].strip()
    global_name = arguments[1].strip()
    global_value = arguments[2].strip()
    global_address = arguments[3].strip()

    return globals(global_type, global_name, global_address, global_value)



def get_repo_global_list() -> list[globals]:

    globals_list = list()

    if SOURCE_FOLDER.exists():
        for src_file in SOURCE_FOLDER.iterdir():
            path = str(src_file)

            # exclude folders and other unwanted files
            if path.endswith(".cpp") or path.endswith(".hpp"):
                
                with open(path, "r") as file:
                    for line in file:
                        
                        if "//" in line:    # remove comments
                            line = line[: line.find("//")]
                        
                        if "DEFINE_GLOBAL_ARRAY" in line.upper():
                            # TODO
                            continue
                        if "DEFINE_GLOBAL_INIT" in line.upper():
                            #globals_list.append(get_inited_global(line))
                            pass
                        elif "DEFINE_GLOBAL" in line.upper():
                            globals_list.append(get_uninit_global(line))
                        
                            

    return globals_list


remove_hex_prefix = lambda string : string.upper().replace("0X", "")

def try_get_value_from_func(reference_ea, global_var : globals):
    offset = reference_ea

    mnem = idc.print_insn_mnem(offset)
    if mnem == "mov" or mnem == "movl" or mnem == "movw" or mnem == "movsx":
        destination = idc.print_operand(offset, 0)

        global_address = remove_hex_prefix(global_var.address)
        
        # exclude unwanted destinations
        if global_address in destination:

            source = idc.print_operand(offset, 1)

            crt_init_func_address = idc.get_prev_fchunk(reference_ea)

            if source[0].isdigit():
                # Found literal value

                if source.endswith('h'):
                    source = source.replace('h', '')
                    return (crt_init_func_address, "0x" + source.upper())
                else:
                    return (crt_init_func_address, source)


            else:
                # multiplication or something else...

                return (crt_init_func_address, FOUND)

        else:
            return (None, None)
    else:
        return (None, None)



def check_uninited_globals(repo_globals : list[globals]):
    for global_var in repo_globals:
        
        if global_var.var_type not in WANTED_GLOBAL_TYPES:
            continue

        if global_var.init_value == None:
            
            for reference in idautils.XrefsTo(int(global_var.address, 16)):
                ref_address_str = hex(reference.frm)
                ref_address_int = int(ref_address_str, 16)
                #print(ref_address)
                #func_address = idc.get_prev_fchunk(ref_address_int)
                #print(hex(func_address.frm))
                func_name = idc.get_func_name(ref_address_int)
                if "crt_init_" in func_name:
                    #print(f"{ref_address_str} at {func_name}")
                    crt_address, value = try_get_value_from_func(ref_address_int, global_var)
                    if value is not None:
                        pass
                        break
                


def main():
    repo_globals = get_repo_global_list()

    report_1 = check_uninited_globals(repo_globals)
    #report_2 = check_inited_globals(repo_globals)

    #func_list = get_crt_init_list()

    #parse_globals(func_list)
    
    #write_global_vars_list(func_list)

    #for func_ea in func_list[:5]:
    #    print()

    pass

if __name__ == "__main__":
    main()