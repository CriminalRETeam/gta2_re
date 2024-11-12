from contextlib import suppress
import json
from pathlib import Path
import re
import sys
import urllib.error
import urllib.request
import webbrowser
from iced_x86 import *

REPO_DIR = Path(__file__).parent.parent
SCRIPTS_DIR = REPO_DIR / "Scripts"
CONFIG_DIR = REPO_DIR / "config"
RESOURCES_DIR = REPO_DIR / "resources"
INCLUDE_PATHS = [REPO_DIR / "Source"]

def cpp_expand(file, path, matched_files):
    out = ""
    for line in file:
        val = re.match(r'#include "(.*)"', line)
        if val:
            out += cpp_expand_path(val.group(1), matched_files)
        elif re.match(r"#pragma once", line):
            matched_files.add(path)
        else:
            out += line
    return out


def cpp_expand_path(path, matched_files):
    if path in matched_files:
        return ""

    for include_path in INCLUDE_PATHS:
        with suppress(FileNotFoundError):
            with open(include_path / path) as f:
                return cpp_expand(f, path, matched_files)

    raise Exception("No include found for " + path + " in " + str(INCLUDE_PATHS))

def load_csv_file(filename):
    ret = []
    with open(filename) as file:
        lines = [line.rstrip() for line in file]
        for line in lines:
            ret.append(line.split(","))        
    return ret

def get_bytes_from_function(func_name_to_find) -> bytes:
    funcs = load_csv_file("bin_comp/og_function_data.csv")
    for func in funcs:
        func_name, og_func_address, og_func_offset, og_func_size = func
        if func_name == func_name_to_find:
            with open("bin_comp/10.5.exe", mode='rb') as file: # b is important -> binary
                file.seek(int(og_func_offset, 16))
                return file.read(int(og_func_size, 16))
            
    print(f"couldn't find function: {func_name_to_find}")
    return None

DATA_OFFSET = 0xFFB59380

def resolve_func_name(func_address: int) -> str | None:
    func_address_offset = func_address - DATA_OFFSET
    funcs = load_csv_file("bin_comp/og_function_data.csv")
    for func in funcs:
        func_name, og_func_address, og_func_offset, og_func_size = func
        if func_address_offset == int(og_func_address, 16):
            # don't include the class name
            if "::" in func_name:
                return func_name.split("::")[1]
            else:
                return func_name
            
    return None

def to_str(mnemonic: Mnemonic) -> str:
    match mnemonic:
        case Mnemonic.JL:
            return "jl"
        case Mnemonic.JLE:
            return "jle"
        case Mnemonic.JG:
            return "jg"
        case Mnemonic.JGE:
            return "jge"
        case Mnemonic.JE:
            return "je"
        case Mnemonic.JNE:
            return "jne"
        case Mnemonic.JBE:
            return "jbe"
        case _:
            print(f"error: Mnemonic {mnemonic} has no to_str case!")
            sys.exit(1)


def is_jump(mnemonic: Mnemonic) -> bool:
    return mnemonic in {Mnemonic.JL, Mnemonic.JLE, Mnemonic.JG, Mnemonic.JGE, Mnemonic.JE, Mnemonic.JNE, Mnemonic.JBE}

def dism_func(func_bytes):
    decoder = Decoder(32, func_bytes)
    formatter = Formatter(FormatterSyntax.GAS)

    unk_funcs = dict()
    labels = dict()
    asm = list()
    asm.append(".att_syntax")

    # gather labels first
    for instruction in decoder:
        if is_jump(instruction.mnemonic):
            if instruction.near_branch_target not in labels:
                labels[instruction.near_branch_target] = f"label_{len(labels)}"
                print(f"add label at ip: {instruction.near_branch_target}")

    # reset decoder for the second run
    decoder = Decoder(32, func_bytes)

    for instruction in decoder:
        # add label
        if instruction.ip in labels:
            asm.append(f"{labels[instruction.ip]}:")
        
        if instruction.mnemonic == Mnemonic.CALL:
            resolved_name = resolve_func_name(instruction.near_branch_target)
            if resolved_name is not None:
                asm.append(f"call {resolved_name}")
            else:
                if instruction.near_branch_target not in unk_funcs:
                    unk_funcs[instruction.near_branch_target] = f"unknown_func{len(unk_funcs)}"
                asm.append(f"call {unk_funcs[instruction.near_branch_target]}")
        elif is_jump(instruction.mnemonic):
            # add label to jump instruction
            if instruction.near_branch_target in labels:
                asm.append(f"{to_str(instruction.mnemonic)} {labels[instruction.near_branch_target]}")
        else:
            asm.append(formatter.format(instruction))

    asm_str = "\n".join(asm)
    return asm_str

def main():
    if not (SCRIPTS_DIR / "bin_comp" / "10.5.exe").exists():
        print(f"gta2 executable '10.5.exe' not found! Move '10.5.exe' to {str(SCRIPTS_DIR / "bin_comp" / "10.5.exe")} and try again!")
        sys.exit(1)

    if not (SCRIPTS_DIR / "bin_comp" / "og_function_data.csv").exists():
        print(f"og_function_data.csv not found! run ida_dump_func_data.py and try again!")
        sys.exit(1)

    if len(sys.argv) < 2:
        print(f"syntax: {sys.argv[0]} <function_name>\n")
        print("function names are from bin_comp/og_function_data.csv")
        sys.exit(1)

    function_name = sys.argv[1]
    func_bytes = get_bytes_from_function(function_name)
    if func_bytes == None:
        sys.exit(1)

    asm = dism_func(func_bytes)
    print(asm)

    #ctx = cpp_expand_path("Game_0x40.hpp", set())
    #print(ctx)

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
                "diff_label": function_name,
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
