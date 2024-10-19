from iced_x86 import *

# get_nav_zone_unknown_4DF890
# Map_0x370::get_nav_zone_unknown_4DF890,0x4df890,0xdf890,0x2d
# 

newExeFile = "/home/raiden/git/gta2_re/build_vc6/Release/decomp_main.exe"
new_func_offset = 0x8550

ogExeFile = "10.5.exe"
og_func_offset = 0xdf890
og_func_size = 0x2d

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

og_func_bytes = get_bytes_from_file(ogExeFile, og_func_offset, og_func_size)
og_asm_str = dism_func(og_func_bytes)
with open("og_asm.txt", "w") as og_asm_file:
    og_asm_file.write(og_asm_str)

new_func_bytes = get_bytes_from_file(newExeFile, new_func_offset, og_func_size)
new_asm_str = dism_func(new_func_bytes)
with open("new_asm.txt", "w") as new_asm_file:
    new_asm_file.write(new_asm_str)

if new_asm_str == og_asm_str:
    print("Match!")
else:
    # todo prob only save asm files on failure else rip disk i/o
    print("Over!")