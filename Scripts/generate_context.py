import os
import re
from contextlib import suppress
from pathlib import Path
import pyclip

CURRENT_DIRECTORY = os.path.dirname(os.path.abspath(__file__))

REPO_DIR = Path(__file__).parent.parent
INCLUDE_PATHS = [REPO_DIR / "Source"]

def cpp_expand(file, path, matched_files):
    out = ""
    matched_files.add(path)
    for line in file:
        val = re.match(r'#include "(.*)"', line)
        if val:
            include_path = val.group(1)
            full_include_path = find_include_path(include_path)
            if full_include_path and full_include_path not in matched_files:
                with suppress(FileNotFoundError):
                    with open(full_include_path, 'r') as include_file:
                        out += cpp_expand(include_file, full_include_path, matched_files)
        else:
            out += line
    return out

def find_include_path(include):
    for include_path in INCLUDE_PATHS:
        full_path = include_path / include
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

def main():
    ctx = cpp_expand_path("decompme_context.hpp", set())

    ctx = ctx.replace("#pragma once", "")

    pyclip.copy(ctx)
    print("Context copied to clipboard")

if __name__ == "__main__":
    main()
