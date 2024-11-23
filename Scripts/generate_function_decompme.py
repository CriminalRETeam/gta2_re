import json
from pathlib import Path
import sys
import urllib.error
import urllib.request
import webbrowser
from iced_x86 import *
import argparse

REPO_DIR = Path(__file__).parent.parent
SCRIPTS_DIR = REPO_DIR / "Scripts"
CONFIG_DIR = REPO_DIR / "config"
RESOURCES_DIR = REPO_DIR / "resources"
INCLUDE_PATHS = [REPO_DIR / "Source"]


class OgFunctionData:
    def __init__(self, name: str, address: str, offset: str, size: str, mangled_name: str | None = None):
        self.name = name
        self.address = int(address, 16)
        self.offset = int(offset, 16)
        self.size = int(size, 16)
        self.mangled_name = mangled_name

    def get_function_bytes(self) -> bytes:
        with open(str(SCRIPTS_DIR) + "/bin_comp/10.5.exe", mode='rb') as file: # b is important -> binary
            file.seek(self.offset)
            return file.read(self.size)

class FunctionCollection:
    __functions: set[OgFunctionData] = set()

    def __init__(self):
        og_func_data = self.load_csv_file(str(SCRIPTS_DIR) + "/bin_comp/og_function_data.csv")
        new_func_data = self.load_csv_file(str(SCRIPTS_DIR) + "/bin_comp/new_function_data.csv")
        for og_func in og_func_data:
            og_name, og_address, og_offset, og_size = og_func
            mangled_name_found = False
            for new_func in new_func_data:
                mangled_name = new_func[0]
                new_og_address = new_func[3]
                if og_address.lower() == new_og_address.lower():
                    mangled_name_found = True
                    break

            if mangled_name_found:
                data = OgFunctionData(og_name, og_address, og_offset, og_size, mangled_name)
            else:
                data = OgFunctionData(og_name, og_address, og_offset, og_size)

            self.__functions.add(data)

    def load_csv_file(self, filename) -> list[list]:
        ret = []
        with open(filename) as file:
            lines = [line.rstrip() for line in file]
            for line in lines:
                ret.append(line.split(","))
        return ret

    def get_data_by_name(self, func_name: str) -> OgFunctionData | None:
        for data in self.__functions:
            if data.name == func_name:
                return data
        return None

    def get_data_by_address(self, address: int) -> OgFunctionData | None:
        for data in self.__functions:
            if data.address == address:
                return data
        return None
    
    def get_data(self) -> set[OgFunctionData]:
        return self.__functions.copy()

FUNC_COLLECTION = FunctionCollection()

DATA_OFFSET = 0x00401000

def toSigned32(n):
    n = n & 0xffffffff
    return (n ^ 0x80000000) - 0x80000000

def resolve_func_name(resolve_func_address: int, current_func: OgFunctionData, as_mangled_name: bool = True) -> str | None:
    resolve_func_address = toSigned32(resolve_func_address)

    near_func_address_offset = DATA_OFFSET + resolve_func_address
    far_func_address_offset = current_func.address + resolve_func_address 
    for func_data in FUNC_COLLECTION.get_data():
        if (near_func_address_offset == func_data.address or
            far_func_address_offset == func_data.address):
            ret_func_name = ""
            if as_mangled_name and func_data.mangled_name is not None:
                ret_func_name = func_data.mangled_name
            elif "::" in func_data.name: # don't include the class name
                ret_func_name = func_data.name.split("::")[1]
            else:
                ret_func_name = func_data.name

            if any(char in ret_func_name for char in {"?", "@", "$"}):
                # the mangled name needs to be in double quotes or the assembler will complain about special characters
                return f'"{ret_func_name}"'
            else:
                return ret_func_name

    print(f"resolve_func_name: near func address {hex(near_func_address_offset)} nor far func address {hex(far_func_address_offset)} found!")
    return None

def is_jump(mnemonic: Mnemonic) -> bool:
    return mnemonic in {Mnemonic.JL, Mnemonic.JLE, Mnemonic.JG, Mnemonic.JGE, Mnemonic.JE, Mnemonic.JNE, Mnemonic.JBE, Mnemonic.JMP, Mnemonic.JB}

def dism_func(target_func: OgFunctionData, objdiff_scratch: bool):
    decoder = Decoder(32, target_func.get_function_bytes())
    formatter = Formatter(FormatterSyntax.GAS)

    unk_funcs = dict()
    labels = dict()
    asm = list()
    asm.append(".att_syntax")

    if objdiff_scratch:
        asm.append(f'"{target_func.mangled_name}":')
        asm.append(f'.global "{target_func.mangled_name}"')

    # gather labels first
    for instruction in decoder:
        if is_jump(instruction.mnemonic):
            if instruction.near_branch_target not in labels:
                # when labels start with .L they aren't emitted by the assembler
                labels[instruction.near_branch_target] = f".L{len(labels)}"
                print(f"add label at ip: {instruction.near_branch_target}")

    # reset decoder for the second run
    decoder = Decoder(32, target_func.get_function_bytes())

    for instruction in decoder:
        # add label
        if instruction.ip in labels:
            asm.append(f"{labels[instruction.ip]}:")
        
        if instruction.mnemonic == Mnemonic.CALL:
            resolved_name = resolve_func_name(instruction.near_branch_target, target_func)
            if resolved_name is not None:
                asm.append(f"call {resolved_name}")
            else:
                if instruction.near_branch_target not in unk_funcs:
                    unk_funcs[instruction.near_branch_target] = f"unknown_func{len(unk_funcs)}"
                asm.append(f"call {unk_funcs[instruction.near_branch_target]}")
        elif is_jump(instruction.mnemonic):
            # add label to jump instruction
            if instruction.near_branch_target in labels:
                jump_mnemonic = formatter.format_mnemonic(instruction);
                asm.append(f"{jump_mnemonic} {labels[instruction.near_branch_target]}")
        else:
            asm.append(formatter.format(instruction))

    asm_str = "\n".join(asm)
    return asm_str

def main():
    parser = argparse.ArgumentParser("generate_function_decompme")
    parser.add_argument("ida_function_name")
    parser.add_argument("-o", "--objdiff", help="generate a scratch for the objdiff tool", action="store_true")
    args = parser.parse_args()

    if not (SCRIPTS_DIR / "bin_comp" / "10.5.exe").exists():
        print("gta2 executable '10.5.exe' not found! Move '10.5.exe' to " + str(SCRIPTS_DIR + "/bin_comp/10.5.exe") + " and try again!")
        sys.exit(1)

    if not (SCRIPTS_DIR / "bin_comp" / "og_function_data.csv").exists():
        print(f"og_function_data.csv not found! run ida_dump_func_data.py and try again!")
        sys.exit(1)

    target_func = FUNC_COLLECTION.get_data_by_name(args.ida_function_name)
    if target_func == None:
        print(f"could not find a function with the name: {args.ida_function_name}")
        sys.exit(1)

    asm = dism_func(target_func, args.objdiff)
    print("\n" + asm)

    if args.objdiff:
        diff_label = target_func.mangled_name
    else:
        diff_label = args.ida_function_name

    req = urllib.request.Request(
        "https://decomp.me/api/scratch",
        headers={
            "Content-Type": "application/json",
        },
        data=json.dumps(
            {
                "compiler": "msvc6.4",
                "compiler_flags": "/TP /O2 /GX /EHsc",
                "context": "",
                "diff_flags": [],
                "diff_label": diff_label,
                "libraries": [],
                "platform": "win32",
                "preset": 152,
                "target_asm": asm,
            }
        ).encode("utf8"),
    )

    try:
        with urllib.request.urlopen(req) as res:
            out_data = json.load(res)
    except urllib.error.HTTPError as err:
        print(json.load(err.fp))
        raise

    scartch_url = "https://decomp.me/scratch/" + out_data["slug"] + "/claim?token=" + out_data["claim_token"]
    print(scartch_url)
    webbrowser.open_new_tab(scartch_url)

if __name__ == "__main__":
    main()
