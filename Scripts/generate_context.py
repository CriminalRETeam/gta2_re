import os
import re
from contextlib import suppress
from pathlib import Path
import pyclip

CURRENT_DIRECTORY = os.path.dirname(os.path.abspath(__file__))

REPO_DIR = Path(__file__).parent.parent
INCLUDE_PATHS = [REPO_DIR / "Source", REPO_DIR]
IGNORE_PATHS = ["Function.hpp"]

CTORS_TO_FIX = ["UnknownList", "TrailerPool", "Hud_CopHead_C_Array", "Hud_Arrow_7C_Array", "Police_7B8"]

MACROS = r"""
#define EXPORT 

#define DEFINE_GLOBAL(type, name, addr) extern type name
#define DEFINE_GLOBAL_ARRAY(type, name, size, addr) extern type name[size]

#define DEFINE_GLOBAL_INIT(type, name, value, addr) extern type name

#define EXTERN_GLOBAL(type, name) extern type name
#define EXTERN_GLOBAL_ARRAY(type, name, size) extern type name[size]

#define GTA2_ASSERT_SIZEOF_ALWAYS(structureName, expectedSize)

#define GTA2_COUNTOF(x) (sizeof(x) / sizeof(*(x)))
#define GTA2_COUNTOF_S(x) ((s32)GTA2_COUNTOF(x))

#define GTA2_DELETE_AND_NULL(x) \
    delete x;                   \
    x = 0;

"""

def cpp_expand(file, curr_path : Path, matched_files):
    out = ""
    matched_files.add(curr_path)
    for line in file:
        val = re.match(r'#include "(.*)"', line)
        if val:
            include_path = val.group(1)
            if include_path not in IGNORE_PATHS:
                full_include_path = find_include_path(include_path, curr_path)
                if full_include_path and full_include_path not in matched_files:
                    with suppress(FileNotFoundError):
                        with open(full_include_path, 'r') as include_file:
                            out += cpp_expand(include_file, full_include_path, matched_files)
        else:
            out += line
    return out

def find_include_path(include : str, curr_path : Path):
    for include_path in INCLUDE_PATHS:
        full_path = include_path / include
        if full_path.exists():
            return full_path
        else:
            # treat more compliated paths
            # TODO: may not work for multiple /../ usages
            if "..\\" in include:
                new_path = curr_path.parent.parent
                full_path = new_path / include.replace("..\\", "")
            elif "../" in include:
                new_path = curr_path.parent.parent
                full_path = new_path / include.replace("../", "")

            if full_path.exists():
                return full_path            
    return None

def cpp_expand_path(path, matched_files):
    for include_path in INCLUDE_PATHS:
        include_file_path = include_path / path
        if include_file_path.exists():
            with open(include_file_path, 'r') as f:
                return cpp_expand(f, str(include_file_path), matched_files)
    
    raise Exception("No include found for " + path + " in " + str(INCLUDE_PATHS))

def remove_annoying_inlined_ctors(ctx_body : str):
    """Remove ctors from context, so decompme won't display junk asm.
    
    This won't work for complex ctors, such as involving IFs and FORs"""
    new_body = ctx_body
    for ctor_class in CTORS_TO_FIX:
        str_index = new_body.find(f"{ctor_class}()\n")

        if str_index == -1:
            continue    # not found

        body_stripped = new_body[str_index : ]

        left_brace_idx = body_stripped.find(r"{")
        right_brace_idx = body_stripped.find(r"}")
        
        block_to_remove = body_stripped[left_brace_idx : right_brace_idx+1]

        new_body = new_body.replace(f"{ctor_class}()\n    " + block_to_remove, f"{ctor_class}();\n")
    
    return new_body

def main():
    matched_files = set()
    for ignore_path in IGNORE_PATHS:
        matched_files.add(Path(ignore_path))

    ctx_body = cpp_expand_path("decompme_context.hpp", matched_files)

    ctx_body = ctx_body.replace(";#include", ";\n#include")  # fix missing \n somewhere

    ctx_body = ctx_body.replace("<VC98/Include/DPLAY.H>", "<DPLAY.H>")
    ctx_body = ctx_body.replace("<VC98/Include/DPLOBBY.H>", "<DPLOBBY.H>")

    ctx_body = ctx_body.replace("#pragma once", "")

    ctx_body = remove_annoying_inlined_ctors(ctx_body)

    ctx = MACROS + ctx_body

    pyclip.copy(ctx)
    print("Context copied to clipboard")

if __name__ == "__main__":
    main()
