import os

CURRENT_DIRECTORY = os.path.dirname(os.path.abspath(__file__))

def load_file(filename):
    with open(filename) as file:
        lines = [line.rstrip() for line in file]
        return lines
    return []

# Looking backwards for the pattern:
# 90
# 90
# B8 XX XX XX XX
# B8 XX XX XX XX
# 90
# 90
def get_func_meta(bytes, offset):
    # find double nop
    if bytes[offset] == 0x90 and bytes[offset-1] == 0x90:
        # find double mov eax, xyz, mov eax xyz
        if bytes[offset-6] == 0xB8 and bytes[offset-11] == 0xB8:
            # find final double nop
            if bytes[offset-12] == 0x90 and bytes[offset-13] == 0x90:
                ogFuncAddr = int.from_bytes([bytes[offset - 10],bytes[offset - 9],bytes[offset - 8],bytes[offset - 7]], "little")
                funcStatus = int.from_bytes([bytes[offset - 5],bytes[offset - 4],bytes[offset - 3],bytes[offset - 2]], "little")
                #print(hex(ogFuncAddr))
                return [ogFuncAddr, funcStatus]
    return []

def find_func_meta_data(exeBytes, funcFileOffset):
    pos = funcFileOffset

    # look backwards up to 10 bytes for a double nop and mov eax, XXXXXXXX
    lookBackPos = 1
    while True:
        meta = get_func_meta(exeBytes, pos - lookBackPos)
        if len(meta) > 0:
            return meta

        lookBackPos = lookBackPos + 1
        if lookBackPos > 10:
            break

    return []

def parse_map(mapFilename, exeBytes, csv_file):
    lines = load_file(mapFilename)
    load_address = 0x0 # executable base load address

    processing_sections = False
    text_section_id = "" # id of the .text section, don't process other section entries

    processing_text_section_entries = False
    first_text_section_entry = True # to subtract the load address from the 1st entry VA to get the base file offset
    base_file_offset = 0 # to add onto the func offset to get the correct file offset
    last_func_offset = 0 # to calculate current func size

    for line in lines:
        if processing_text_section_entries:
            parts = line.split(" ")
            parts = list(filter(None, parts))
            if (len(parts) >= 1):
                if parts[0].split(":")[0] == text_section_id and parts[3] == "f": # seems to indicate its a function ?? whatever, it works
                    if first_text_section_entry:
                        first_text_section_entry = False
                        base_file_offset = int("0x" + parts[2], 16) - load_address
                        print("base_file_offset is: " + hex(base_file_offset))

                    func_section_offset = int("0x" + parts[0].split(":")[1].strip(), 16)
                    func_fo = func_section_offset + base_file_offset
                    func_size = 0
                    func_va = int("0x" + parts[2], 16)
                    # mangled_name, virtual address, file offset
                    meta = find_func_meta_data(exeBytes, func_fo)
                    if len(meta) > 0:
                        ogAddr = meta[0]
                        funcStatus = meta[1]
                        csv_file.write(f"{parts[1]},{hex(func_va)},{hex(func_fo)},{hex(ogAddr)},{hex(funcStatus)}\n")
                    else:
                        csv_file.write(f"{parts[1]},{hex(func_va)},{hex(func_fo)},?,?\n")
                else:
                    processing_text_section_entries = False

                # todo: func size calc is probably wrong ?? need to sub the size of the NEXT func, not the last one? then whole section size for the last entry
                last_func_offset = func_section_offset
        elif processing_sections:
            parts = line.split(" ")
            parts = list(filter(None, parts))
            if parts[2] == ".text":
                text_section_id = parts[0].strip().split(":")[0]
                print("text_section_id is " + text_section_id)
                processing_sections = False
        elif line.startswith(" Preferred load address is "):
            load_address_str = "0x" + line[27:].strip()
            print("Load address is " + load_address_str)
            load_address = int(load_address_str,16)
        elif line.startswith(" Start         Length     Name                   Class"):
            processing_sections = True
            print("Process sections...")
        elif line.startswith("  Address         Publics by Value              Rva+Base     Lib:Object"):
            processing_text_section_entries = True
            print("Processing text section entries...")
        else:
            pass

mapping_csv_path = os.path.join(CURRENT_DIRECTORY, "new_function_data.csv")
exeBytes = open("../../build_vc6/Release/decomp_main.exe", "rb").read()

with open(mapping_csv_path, "w") as file:
    parse_map("../../build_vc6/output.map", exeBytes, file)
