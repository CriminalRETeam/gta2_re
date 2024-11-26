import idc
import idaapi
import os
import re

# replace the path with your own
SOURCE_PATH = "C:\\GitHub\\gta2_re\\Source"

def parse_gta2_files() -> set:
    repo_func_and_var_names = set()
    for file in os.listdir(SOURCE_PATH):
        if file.endswith(".hpp") or file.endswith(".cpp"):
            file_path = os.path.join(SOURCE_PATH, file)
            with open(file_path, "r") as f:
                for line in f:
                    matches = re.findall(r"(\w+_[A-Fa-f0-9]{6})\b", line)
                    repo_func_and_var_names.update(matches)
                
    return repo_func_and_var_names

def is_unnamed(name: str) -> bool:
    return name.startswith("sub_") or name.startswith("byte_") or name.startswith("word_") or name.startswith("dword_") or name.startswith("stru_") or name.startswith("off_") or name.startswith("asc_")

def rename_ida_functions(repo_func_and_var_names: set) -> None:
    rename_count = 0
    for name in repo_func_and_var_names:
        if is_unnamed(name):
            continue
        
        ea = int(name[-6:], 16)
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
