import idc
import idaapi
import os
import re

# replace the path with your own
SOURCE_PATH = "C:\\GitHub\\gta2_re\\Source"

MARKER = re.compile(r"\b(?:MATCH|WIP|STUB)_FUNC\(0x([0-9A-Fa-f]+)\)\s*(?:template\s*<[^>]*>\s*)?[^{;(]*?([\w:~]+)\s*\(")
GLOBAL = re.compile(r"\bDEFINE_\w*GLOBAL\w*\(\s*[^,]+?,\s*(\w+)\s*,[^;]*?\b0x([0-9A-Fa-f]{6})\)")
SUFFIX = re.compile(r"\b(\w+_([A-Fa-f0-9]{6}))\b")


def parse_gta2_files() -> dict:
    """address -> name: functions by their marker (Class::Name), globals by DEFINE_GLOBAL, and anything else
    still named after its address (Name_ADDRESS)."""
    names = {}
    for file in os.listdir(SOURCE_PATH):
        if file.endswith(".hpp") or file.endswith(".cpp"):
            with open(os.path.join(SOURCE_PATH, file), "r", errors="replace") as f:
                text = f.read()
            for name, addr in SUFFIX.findall(text):
                names.setdefault(int(addr, 16), name)
            for name, addr in GLOBAL.findall(text):
                names[int(addr, 16)] = name
            for addr, name in MARKER.findall(text):
                names[int(addr, 16)] = name
    return names

def is_unnamed(name: str) -> bool:
    name = name.split("::")[-1]
    return name.startswith("sub_") or name.startswith("byte_") or name.startswith("word_") or name.startswith("dword_") or name.startswith("stru_") or name.startswith("off_") or name.startswith("asc_")

def rename_ida_functions(repo_names: dict) -> None:
    rename_count = 0
    for ea, name in sorted(repo_names.items()):
        if is_unnamed(name):
            continue

        ida_name = idc.get_name(ea)

        if ida_name == "" or name == ida_name:
            continue

        if idaapi.set_name(ea, name):
            print(f"renamed {ida_name} to {name}")
            rename_count = rename_count + 1
        else:
            print(f"failed to rename {ida_name} to {name}")


    print(f"renamed {rename_count} items")


def main():
    rename_ida_functions(parse_gta2_files())
    print("done!")

if __name__ == "__main__":
    main()
