from iced_x86 import *
import post_process_asm

def get_bytes_from_file(fileName, offset, len):
    with open(fileName, mode='rb') as file: # b is important -> binary
        file.seek(offset)
        return file.read(len)

def dism_func(func_bytes):
    decoder = Decoder(32, func_bytes)
    formatter = Formatter(FormatterSyntax.GAS)

    asm = list()
    for instruction in decoder:
        asm.append(formatter.format(instruction))

    asm_str = "\n".join(asm)
    return asm_str

# so windows doesn't like :: being in a file name, and probably other stuff
# put em in the bin and something its more happy with
def func_name_as_safe_file_name(func_name):
    func_name = func_name.replace("::", "_")
    return func_name

def compare_function(newExeFile, ogExeFile, func_name, new_func_offset, og_func_offset, og_func_size):
    #strStatus = "Checking " + func_name + " " + hex(new_func_offset) + " " + hex(og_func_offset) + " " + hex(og_func_size)
    og_func_bytes = get_bytes_from_file(ogExeFile, og_func_offset, og_func_size)
    og_asm_str = dism_func(og_func_bytes)
    og_asm_str_post_processed = post_process_asm.post_process_asm(og_asm_str)

    new_func_bytes = get_bytes_from_file(newExeFile, new_func_offset, og_func_size)
    new_asm_str = dism_func(new_func_bytes)
    new_asm_str_post_processed = post_process_asm.post_process_asm(new_asm_str)

    if new_asm_str_post_processed == og_asm_str_post_processed:
        return True
    else:
        with open(func_name_as_safe_file_name(func_name) + "_og_asm_post_processed.txt", "w") as og_asm_file:
            og_asm_file.write(og_asm_str_post_processed)
        with open(func_name_as_safe_file_name(func_name) + "_og_asm.txt", "w") as og_asm_file:
            og_asm_file.write(og_asm_str)
        with open(func_name_as_safe_file_name(func_name) + "_new_asm_post_processed.txt", "w") as new_asm_file:
            new_asm_file.write(new_asm_str_post_processed)

        return False

if __name__ == "__main__":
    compare_function(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5], sys.argv[6])
