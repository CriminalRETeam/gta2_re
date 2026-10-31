from iced_x86 import *
import post_process_asm
from pathlib import Path

def get_bytes_from_file(fileName, offset, length):
    if offset < 0:
        raise ValueError(f"Negative function offset {offset} in {fileName}")
    if length <= 0:
        raise ValueError(f"Function size must be positive in {fileName}: {length}")
    with open(fileName, mode='rb') as file: # b is important -> binary
        file.seek(offset)
        data = file.read(length)
    if len(data) != length:
        raise ValueError(f"Short function read in {fileName} at {hex(offset)}: "
                         f"expected {length} bytes, got {len(data)}")
    return data

def dism_func(func_bytes):
    decoder = Decoder(32, func_bytes)
    formatter = Formatter(FormatterSyntax.GAS)

    asm = list()
    for instruction in decoder:
        if instruction.is_invalid:
            raise ValueError(f"Invalid x86 instruction at function offset {hex(instruction.ip)}")
        asm.append(formatter.format(instruction))

    asm_str = "\n".join(asm)
    return asm_str

# so windows doesn't like :: being in a file name, and probably other stuff
# put em in the bin and replacee with something its more happy with
def func_name_as_safe_file_name(func_name):
    func_name = func_name.replace("::", "_")
    return func_name

def call_targets(func_bytes, va):
    """Absolute target of each instruction that is a direct call, None for the others."""
    return [i.near_branch_target if i.flow_control == FlowControl.CALL and i.op0_kind == OpKind.NEAR_BRANCH32 else None
            for i in Decoder(32, func_bytes, ip=va)]


def pin_known_calls(og_asm_str, new_asm_str, og_targets, new_targets, call_identity):
    """Calls to an original function that has an inline alias (inline_aliases.csv) are compared by address, not by
    the order of first use: the build may call the alias or the marked function, the original must call that address."""
    og_lines, new_lines = og_asm_str.split("\n"), new_asm_str.split("\n")
    if len(og_lines) != len(new_lines) or len(og_targets) != len(og_lines) or len(new_targets) != len(new_lines):
        return og_asm_str, new_asm_str
    for k, (x, y) in enumerate(zip(og_targets, new_targets)):
        if x is None or y is None:
            continue
        want = call_identity.original_of(y)
        if x not in call_identity.pinned and want not in call_identity.pinned:
            continue
        if want is not None and want != x and call_identity.same_original_code(x, want):
            want = x
        og_lines[k] = "call og_%x" % x
        new_lines[k] = "call og_%x" % want if want is not None else "call unknown_%x" % y
    return "\n".join(og_lines), "\n".join(new_lines)


def compare_function(newExeFile, ogExeFile, func_name, new_func_offset, og_func_offset, og_func_size,
                     new_va=None, og_va=None, call_identity=None):
    strStatus = "Checking " + func_name + " " + "N:" + hex(new_func_offset) + " OG:" + hex(og_func_offset) + " LEN:" + hex(og_func_size)
    og_func_bytes = get_bytes_from_file(ogExeFile, og_func_offset, og_func_size)
    og_asm_str = dism_func(og_func_bytes)

    new_func_bytes = get_bytes_from_file(newExeFile, new_func_offset, og_func_size)
    new_asm_str = dism_func(new_func_bytes)
    if call_identity is not None:
        og_asm_str, new_asm_str = pin_known_calls(og_asm_str, new_asm_str, call_targets(og_func_bytes, og_va),
                                                  call_targets(new_func_bytes, new_va), call_identity)
    og_asm_str_post_processed = post_process_asm.post_process_asm(og_asm_str)
    new_asm_str_post_processed = post_process_asm.post_process_asm(new_asm_str)

    if new_asm_str_post_processed == og_asm_str_post_processed:
        return True
    else:
        print(strStatus)
        Path("diff").mkdir(parents=True, exist_ok=True)

        with open("diff/" + func_name_as_safe_file_name(func_name) + "_og_asm_post_processed.txt", "w") as og_asm_file:
            og_asm_file.write(og_asm_str_post_processed)
        with open("diff/" + func_name_as_safe_file_name(func_name) + "_og_asm.txt", "w") as og_asm_file:
            og_asm_file.write(og_asm_str)
        with open("diff/" + func_name_as_safe_file_name(func_name) + "_new_asm_post_processed.txt", "w") as new_asm_file:
            new_asm_file.write(new_asm_str_post_processed)

        return False

if __name__ == "__main__":
    compare_function(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5], sys.argv[6])
