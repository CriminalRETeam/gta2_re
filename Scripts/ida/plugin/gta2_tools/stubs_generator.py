import sys
sys.path.append("..")
import idautils 
import idc
import ida_hexrays
import ida_typeinf
import ida_idaapi
from enum import Enum

# ---------- START ----------
# helper functions to get the type of struct members
def get_struc(struct_tid):
    tif = ida_typeinf.tinfo_t()
    if tif.get_type_by_tid(struct_tid):
        if tif.is_struct():
            return tif
    return ida_idaapi.BADADDR

def get_member(tif, offset):
    if not tif.is_struct():
        return None
    
    udm = ida_typeinf.udm_t()
    udm.offset = offset * 8
    idx = tif.find_udm(udm, ida_typeinf.STRMEM_OFFSET)
    if idx != -1:
        return udm
    
    return None

def get_member_tinfo(tif, udm):
    if tif and udm:
        ida_typeinf.copy_tinfo_t(tif, udm.type)
        return True
    return False

def get_member_type(struc_id, member_offset) -> ida_typeinf.udm_t | None:
    tif = get_struc(struc_id)
    udm = get_member(tif, member_offset)
    if get_member_tinfo(tif, udm):
        return udm
    return None

# ---------- END ----------

class StubsGenerator:
    class StubsType(Enum):
        Source = 0
        Header = 1

    def __init__(self, wrap_header_in_class: bool = True, include_funcs: bool = True, include_fields: bool = True):
        self.wrap_header_in_class = wrap_header_in_class
        self.include_funcs = include_funcs
        self.include_fields = include_fields

    def __rename_types(self, string: str) -> str:
        new_string = string
        if "wchar_t" in new_string:
            print("wchar_t found")
            return new_string
            
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

        for key, value in rename_table.items():
            if key in new_string:
                new_string = new_string.replace(key, value)
        return new_string
            
    def __get_args(self, cfunc) -> str:
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

    def __get_func_declaration(self, cfunc, func_name) -> str:
        rettype = str(cfunc.type.get_rettype())
        rettype = self.__rename_types(rettype)

        args = self.__get_args(cfunc)
        args = self.__rename_types(args)
        return f"EXPORT {rettype} {func_name}({args});"

    def __get_func_definition(self, cfunc, func_name, class_name) -> str:
        rettype = str(cfunc.type.get_rettype())
        rettype = self.__rename_types(rettype)

        args = self.__get_args(cfunc)
        args = self.__rename_types(args)

        func_definition = f"STUB_FUNC({hex(cfunc.entry_ea)})\n"
        func_definition = func_definition + f"{rettype} {class_name}::{func_name}({args})\n"
        func_definition = func_definition + "{\n"
        if rettype != "void":
            func_definition = func_definition + "    return 0;"
        func_definition = func_definition + "\n"
        func_definition = func_definition + "}\n"
        return func_definition

    def generate_stubs(self, class_name: str, stubs_type: StubsType) -> str | None:
        if class_name == "":
            return None

        struc_id = idc.get_struc_id(class_name)
        if struc_id == -1:
            return None

        source = list()
        header = list()
        fields = list()

        if self.wrap_header_in_class and stubs_type == self.StubsType.Header:
            header.append(f"class {class_name}")
            header.append("{")
            header.append("public:")

        if self.include_funcs:
            for segment in idautils.Segments():
                for ea in idautils.Functions(segment, idc.get_segm_end(segment)):
                    ida_func_name = idc.get_func_name(ea)
                    if class_name + "::" not in ida_func_name:
                        continue
                    
                    class_name, func_name = ida_func_name.split("::")
                    cfunc = ida_hexrays.decompile(ea)
                    
                    if stubs_type == self.StubsType.Header:
                        func_decl = self.__get_func_declaration(cfunc, func_name)
                        header.append(func_decl)
                    elif stubs_type == self.StubsType.Source:
                        func_def = self.__get_func_definition(cfunc, func_name, class_name)
                        source.append(func_def)

        if self.include_fields and stubs_type == self.StubsType.Header:
            for member_data in idautils.StructMembers(struc_id):
                field_offset, field_name, field_size = member_data
                udm = get_member_type(struc_id, field_offset)
                if udm:
                    type_str = str(udm.type)
                    if udm.type.is_array():
                        # if it's an array it looks like this: Garox_7C[17]
                        # Garox_7C is the struct
                        array_pos = type_str.find("[")
                        type_name = type_str[:array_pos]
                        type_array = type_str[array_pos:]

                        type_name = self.__rename_types(type_name)

                        fields.append(f"{type_name} {field_name}{type_array};")
                    else:
                        type_name = self.__rename_types(type_str)
                        fields.append(f"{type_name} {field_name};")
            fields_str = "\n".join(fields)
            header.append(fields_str)

        if self.wrap_header_in_class and stubs_type == self.StubsType.Header:
            header.append("};")

        if stubs_type == self.StubsType.Header and len(header) > 0:
            return "\n".join(header)
        elif stubs_type == self.StubsType.Source and len(source) > 0:
            return "\n".join(source)
        else:
            return None
