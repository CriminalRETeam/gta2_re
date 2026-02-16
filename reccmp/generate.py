"""Create reccmp CSV file for functions."""
import re
import json
from pathlib import Path


r_function          = re.compile(r"(MATCH|STUB|WIP)_FUNC\((?P<addr>0x[0-9A-Fa-f]{6})\);?[\w*&\s]*[\s](?P<func_name>\S+)\(")
r_global            = re.compile(r"DEFINE_GLOBAL\([^\s,]+,\s*(?P<var_name>[^\s,]+),\s*(?P<addr>0x[0-9A-Fa-f]{6})\);")
r_global_array      = re.compile(r"DEFINE_GLOBAL_ARRAY\([^\s,]+,\s*(?P<var_name>[^\s,]+),\s*\d+,\s*(?P<addr>0x[0-9A-Fa-f]{6})\);")
r_global_init       = re.compile(r"DEFINE_GLOBAL_INIT\([^\s,]+,\s*(?P<var_name>[^\s,]+),.+(?P<addr>0x[0-9A-Fa-f]{6})\);")
r_global_array_init = re.compile(r"DEFINE_GLOBAL_ARRAY_INIT\([^\s,]+,\s*(?P<var_name>[^\s,]+),\s*\d+,\s*(?P<addr>0x[0-9A-Fa-f]{6})")


reccmp_dir = Path(__file__).parent
root_dir = reccmp_dir.parent
assert root_dir.name == "gta2_re"
source_dir = root_dir / "Source"

# Read the IDA listing and use it to translate 10.5 addresses to 9.6f equivalents.
functions_json = root_dir / "Scripts" / "ida" / "functions_data.json"
with functions_json.open("r") as f:
    ida_list = json.load(f)
    ida_map = {
        obj["v105_address"].lower() : obj["v96f_address"].lower()
        for obj in ida_list
        if obj["v96f_address"] is not None
    }

functions = []
variables = []

for path in source_dir.glob("*.cpp"):
    # These functions are covered in "library.csv".
    if "crt_stubs" in path.name:
        continue

    with path.open("r") as f:
        text = f.read()
        for match_or_stub, addr, func_name in r_function.findall(text):
            is_stub = match_or_stub == "STUB"
            functions.append( (addr.lower(), "stub" if is_stub else "function", func_name ) )

        variables.extend((addr.lower(), name) for name, addr in r_global.findall(text))
        variables.extend((addr.lower(), name) for name, addr in r_global_array.findall(text))
        variables.extend((addr.lower(), name) for name, addr in r_global_init.findall(text))
        variables.extend((addr.lower(), name) for name, addr in r_global_array_init.findall(text))

functions.sort(key=lambda v: v[0])
variables.sort(key=lambda v: v[0])

with open(reccmp_dir / "functions.csv", "w+") as f:
    f.write("address,type,name\n")
    for addr, type_, name in functions:
        f.write(f"{addr},{type_},{name}\n")

with open(reccmp_dir / "variables.csv", "w+") as f:
    f.write("address,type,name\n")
    for addr, name in variables:
        f.write(f"{addr},global,{name}\n")

with open(reccmp_dir / "96f-functions.csv", "w+") as f:
    f.write("address,type,name\n")
    for addr, type_, name in functions:
        if addr in ida_map:
            f.write(f"{ida_map[addr]},{type_},{name}\n")

print(f"Read {len(functions)} functions.")
print(f"Read {len(variables)} variables.")
