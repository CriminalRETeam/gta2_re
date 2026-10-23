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


def local_branch_target_mismatch(og_func_bytes, new_func_bytes):
    """Bind local direct branches to decoded instructions after asm equality.

    Both-external targets remain relocation-normalized. Calls, indirect branches
    and 16-bit near branches are outside this check; it is not CFG equivalence.
    """
    original = list(Decoder(32, og_func_bytes))
    compiled = list(Decoder(32, new_func_bytes))
    if len(original) != len(compiled):
        return "Decoded instruction counts differ"
    original_indices = {ins.ip: index for index, ins in enumerate(original)}
    compiled_indices = {ins.ip: index for index, ins in enumerate(compiled)}
    branch_flows = (FlowControl.UNCONDITIONAL_BRANCH, FlowControl.CONDITIONAL_BRANCH)
    for index, (before, after) in enumerate(zip(original, compiled)):
        before_direct = before.flow_control in branch_flows and before.op0_kind == OpKind.NEAR_BRANCH32
        after_direct = after.flow_control in branch_flows and after.op0_kind == OpKind.NEAR_BRANCH32
        if not before_direct and not after_direct:
            continue
        if not before_direct or not after_direct or before.flow_control != after.flow_control:
            return f"Direct branch shape differs at instruction {index}"
        before_target, after_target = before.near_branch_target, after.near_branch_target
        before_local = 0 <= before_target < len(og_func_bytes)
        after_local = 0 <= after_target < len(new_func_bytes)
        prefix = (f"instruction {index}, original offset {hex(before.ip)} target {hex(before_target)}, "
                  f"compiled offset {hex(after.ip)} target {hex(after_target)}")
        if before_local != after_local:
            return "Local/external branch target shape differs at " + prefix
        if not before_local:
            continue
        if before_target not in original_indices or after_target not in compiled_indices:
            return "Local branch target is not a decoded instruction boundary at " + prefix
        before_index, after_index = original_indices[before_target], compiled_indices[after_target]
        if before_index != after_index:
            return ("Local branch target instruction differs at " + prefix
                    + f" (target instructions {before_index} and {after_index})")
    return None

# so windows doesn't like :: being in a file name, and probably other stuff
# put em in the bin and replacee with something its more happy with
def func_name_as_safe_file_name(func_name):
    func_name = func_name.replace("::", "_")
    return func_name

def compare_function(newExeFile, ogExeFile, func_name, new_func_offset, og_func_offset, og_func_size):
    strStatus = "Checking " + func_name + " " + "N:" + hex(new_func_offset) + " OG:" + hex(og_func_offset) + " LEN:" + hex(og_func_size)
    og_func_bytes = get_bytes_from_file(ogExeFile, og_func_offset, og_func_size)
    og_asm_str = dism_func(og_func_bytes)
    og_asm_str_post_processed = post_process_asm.post_process_asm(og_asm_str)

    new_func_bytes = get_bytes_from_file(newExeFile, new_func_offset, og_func_size)
    new_asm_str = dism_func(new_func_bytes)
    new_asm_str_post_processed = post_process_asm.post_process_asm(new_asm_str)

    asm_matches = new_asm_str_post_processed == og_asm_str_post_processed
    branch_mismatch = local_branch_target_mismatch(og_func_bytes, new_func_bytes) if asm_matches else None
    if asm_matches and branch_mismatch is None:
        return True
    else:
        print(strStatus)
        if branch_mismatch is not None:
            print(branch_mismatch)
        Path("diff").mkdir(parents=True, exist_ok=True)

        with open("diff/" + func_name_as_safe_file_name(func_name) + "_og_asm_post_processed.txt", "w") as og_asm_file:
            og_asm_file.write(og_asm_str_post_processed)
        with open("diff/" + func_name_as_safe_file_name(func_name) + "_og_asm.txt", "w") as og_asm_file:
            og_asm_file.write(og_asm_str)
        with open("diff/" + func_name_as_safe_file_name(func_name) + "_new_asm_post_processed.txt", "w") as new_asm_file:
            new_asm_file.write(new_asm_str_post_processed)
        if branch_mismatch is not None:
            with open("diff/" + func_name_as_safe_file_name(func_name) + "_branch_targets.txt", "w") as branch_file:
                branch_file.write(branch_mismatch + "\n")

        return False

if __name__ == "__main__":
    compare_function(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5], sys.argv[6])
