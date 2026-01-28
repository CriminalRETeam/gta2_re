import os
import re
import sys
import subprocess
from pathlib import Path

# Path to the directory containing THIS script
SCRIPT_DIR = Path(__file__).resolve().parent

# Path to your generate_function_decompme.py
DECOMP_SCRIPT = SCRIPT_DIR / "generate_function_decompme.py"

# Source directory (one level up, then Source/)
SOURCE_DIR = SCRIPT_DIR.parent / "Source"

# Directory where .asm files will be written
ASM_DIR = SCRIPT_DIR / "asm"
ASM_DIR.mkdir(exist_ok=True)

# Regex to match WIP_FUNC(0x123ABC) or STUB_FUNC(0x123ABC)
FUNC_REGEX = re.compile(r"(WIP_FUNC|STUB_FUNC)\s*\(\s*(0x[0-9A-Fa-f]{6})\s*\)")

def extract_addresses_from_cpp(cpp_path: Path):
    text = cpp_path.read_text(encoding="utf-8", errors="ignore")
    matches = FUNC_REGEX.findall(text)
    return [addr for (_, addr) in matches]

def run_decomp_script(addresses):
    out_file = SCRIPT_DIR / "output.asm"
    if out_file.exists():
        out_file.unlink()

    cmd = ["python3", str(DECOMP_SCRIPT), "--asm"] + addresses
    subprocess.run(cmd, cwd=SCRIPT_DIR)

    if out_file.exists():
        return out_file.read_text(encoding="utf-8", errors="ignore")
    return ""

def main():
    asm_files = []

    if len(sys.argv) > 1:
        target_name = sys.argv[1]
        target_path = SOURCE_DIR / target_name

        if not target_path.exists():
            print(f"ERROR: File '{target_name}' not found in {SOURCE_DIR}")
            sys.exit(1)

        cpp_files = [target_path]
    else:
        cpp_files = list(SOURCE_DIR.glob("*.cpp"))
    # ------------------------------

    for cpp_file in cpp_files:
        addrs = extract_addresses_from_cpp(cpp_file)
        if not addrs:
            continue

        print(f"Processing {cpp_file.name} with {len(addrs)} addresses")

        asm_output = run_decomp_script(addrs)

        out_path = ASM_DIR / f"{cpp_file.name}.asm"
        out_path.write_text(asm_output, encoding="utf-8")

        asm_files.append(out_path.name)

        print(f"  → wrote {out_path}")

    # Now generate make_objs.sh
    makefile_path = ASM_DIR / "make_objs.sh"
    with makefile_path.open("w", encoding="utf-8") as f:
        f.write("#!/bin/bash\n\n")
        for asm_name in asm_files:
            obj_name = asm_name.replace(".asm", ".obj")
            f.write(
                f"i686-w64-mingw32-as --32 -mmnemonic=intel -msyntax=intel -mnaked-reg "
                f"-o {obj_name} {asm_name}\n"
            )

    os.chmod(makefile_path, 0o755)
    print(f"\nGenerated {makefile_path}")

if __name__ == "__main__":
    main()
