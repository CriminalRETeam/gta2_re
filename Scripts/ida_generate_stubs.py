from idautils import *
from idaapi import *
from idc import *
import os

# replace with the class you want to stub
CLASS_NAME = "Ped_Unknown_4"

# from highest to lowest priority
rename_table = {
    "unsigned __int8": "u8",
    "char": "char_type",
    "CHAR": "char_type",
    "unsigned __int16": "u16",
    "__int16": "s16",
    "unsigned __int32": "u32",
    "__int32": "s32",
    "float": "f32",
    "double": "f64",
    "_BYTE": "u8",
    "BYTE": "u8",
    "LONG": "s32",
    "_DWORD": "u32",
    "DWORD": "u32",
    "_WORD": "u16",
    "WORD": "u16",
    "BOOL": "s32",
    "unsigned int": "u32",
    "int": "s32"
}

def rename_types(string: str) -> str:
    new_string = string
    if "wchar_t" in new_string:
        print("wchar_t found")
        return new_string
        
    for key, value in rename_table.items():
        if key in new_string:
            new_string = new_string.replace(key, value)
    return new_string
        

def get_args(cfunc) -> str:
    num_args = cfunc.type.get_nargs()
    if (num_args != -1):
        args_str = ""
        for i in range(num_args):
            arg_name = cfunc.get_lvars()[i].name
            if arg_name == "this":
                continue
                
            args_str = args_str + str(cfunc.type.get_nth_arg(i)) + " " + arg_name
            if i < num_args - 1:
                args_str = args_str + ", "
                
    return args_str

def get_func_declaration(cfunc, func_name) -> str:
    rettype = str(cfunc.type.get_rettype())
    rettype = rename_types(rettype)

    args = get_args(cfunc)
    args = rename_types(args)
    return f"EXPORT {rettype} {func_name}({args});"

def get_func_definition(cfunc, func_name) -> str:
    rettype = str(cfunc.type.get_rettype())
    rettype = rename_types(rettype)

    args = get_args(cfunc)
    args = rename_types(args)

    func_definition = f"STUB_FUNC({hex(cfunc.entry_ea)})\n"
    func_definition = func_definition + f"{rettype} {CLASS_NAME}::{func_name}({args})\n"
    func_definition = func_definition + "{\n"
    if rettype != "void":
        func_definition = func_definition + "    return 0;"
    func_definition = func_definition + "\n"
    func_definition = func_definition + "}\n"
    return func_definition

def generate_stubs() -> None:
    with open(f"{CLASS_NAME}.hpp", "w") as header, open(f"{CLASS_NAME}.cpp", "w") as source:
        for segment in Segments():
            for ea in Functions(segment, get_segm_end(segment)):
                ida_func_name = get_func_name(ea)
                if CLASS_NAME + "::" not in ida_func_name:
                    #print(f"skip {ida_func_name}")
                    continue
                
                class_name, func_name = ida_func_name.split("::")
                cfunc = ida_hexrays.decompile(ea)
                
                func_decl = get_func_declaration(cfunc, func_name)
                func_def = get_func_definition(cfunc, func_name)
                
                header.write(func_decl)
                header.write("\n")
                source.write(func_def)
                source.write("\n")

                #print(rename_types(get_func_declaration(cfunc, func_name)))

 
generate_stubs()
print("done!")
