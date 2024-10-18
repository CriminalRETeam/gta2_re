import os

CURRENT_DIRECTORY = os.path.dirname(os.path.abspath(__file__))

def load_file(filename):
    with open(filename) as file:
        lines = [line.rstrip() for line in file]
        return lines
    return []

def parse_map(filename, csv_file):
    lines = load_file(filename)
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
                    file.write(f"{parts[1]},{hex(func_va)},{hex(func_fo)},{hex(func_section_offset - last_func_offset)}\n")
                    print(f"{parts[1]},{hex(func_va)},{hex(func_fo)},{hex(func_section_offset - last_func_offset)}")
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

mapping_csv_path = os.path.join(CURRENT_DIRECTORY, "og_function_data.csv")
with open(mapping_csv_path, "w") as file:
    parse_map("../../build_vc6/output.map", file)
