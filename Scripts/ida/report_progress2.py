import idc
import idautils
from enum import IntEnum

# in BGR format
class FuncColor(IntEnum):
    Matched = 0x0
    Stubbed = 0x3e4a4a
    CoverageTrace = 0x065e03

covered_funcs_left = 0
class_funcs = dict()
misc_funcs = list()
for segment in idautils.Segments():
    for ea in idautils.Functions(segment, idc.get_segm_end(segment)):
        ida_func_name = idc.get_func_name(ea)
        if "nullsub" in ida_func_name or "crt_j_init__" in ida_func_name or "crt_init_" in ida_func_name:
            continue

        if idc.get_color(ea, idc.CIC_FUNC) != FuncColor.CoverageTrace:
            continue

        print("TODO: " + ida_func_name)

        if "::" in ida_func_name:
            class_name, func_name = ida_func_name.split("::")
            if class_name in class_funcs:
                class_funcs[class_name] = class_funcs[class_name] + 1
            else:
                class_funcs[class_name] = 1
        else:
            misc_funcs.append(ida_func_name)

        covered_funcs_left = covered_funcs_left + 1


sorted_funcs = dict(sorted(class_funcs.items(), key=lambda item: item[1], reverse=True))

#with open("C:\\Users\\mouzedrift\\Desktop\\progress.txt", "w") as file:
#    file.write(f"total: {covered_funcs_left} funcs left\n")
#    file.write(f"misc funcs: {len(misc_funcs)}\n\n")
#    for key, val in sorted_funcs.items():
#        file.write(f"{key} {val}\n")

print(f"total: {covered_funcs_left} funcs left")
print(f"misc funcs: {len(misc_funcs)}\n")

for key, val in sorted_funcs.items():
    print(f"{key} {val}")
