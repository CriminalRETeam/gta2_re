"""
Checks the DEFINE_GLOBAL* declarations in Source/ against 10.5's static initialisers.

Every global with a dynamic initialiser (a class type such as Fix16/Ang16/Fix16_Point, or a value that
isn't a compile time constant) gets a small init function, and the CRT calls them all through the
__xc_a..__xc_z pointer table (10.5: 0x607004..0x61A640, about 19,900 functions; most are the header
constants every TU has its own copy of). For each init function this records which globals it
stores to, with the store width and the value when it is an immediate, and reports:

- globals the original initialises dynamically that we declare as a plain POD (wrong type?);
- a store width that differs from our type (Ang16 2, Fix16 4, Fix16_Point 8);
- a constant that differs from our initialiser (only simple Fix16(n), Fix16(raw, 0), Ang16(n) forms);
- class-typed globals we initialise that no original initialiser writes (wrong address?).

Values computed at startup (`0xA00000 - other`, a copy of another global, a normalising loop) show
as 'reg' and aren't compared. The verifier never checks which global an instruction reads, so a
wrong DEFINE_GLOBAL address can sit in a matched function (kAngZero, CarPhysics_B0::PoolAllocate).

Usage (from Scripts/bin_comp): python3 check_global_inits.py
"""

import collections
import glob
import re
import struct

import pefile
from iced_x86 import Decoder, Mnemonic, OpKind, Register, MemorySize

XC_END = 0x61A640  # first 0 after the table (__xc_z)

SIZES = {"Fix16": 4, "Ang16": 2, "Fix16_Point": 8, "Fix16_Point_POD": 8, "s32": 4, "u32": 4, "s16": 2,
         "u16": 2, "u8": 1, "s8": 1, "char": 1, "f32": 4, "float": 4, "bool": 1, "char_type": 1}
CLASSES = {"Fix16", "Ang16", "Fix16_Point", "Fix16_Point_POD"}
WIDTH = {MemorySize.UINT8: 1, MemorySize.INT8: 1, MemorySize.UINT16: 2, MemorySize.INT16: 2,
         MemorySize.UINT32: 4, MemorySize.INT32: 4, MemorySize.FLOAT32: 4, MemorySize.FLOAT64: 8}


def original_inits(exe):
    pe = pefile.PE(exe)
    base = pe.OPTIONAL_HEADER.ImageBase
    mem = pe.get_memory_mapped_image()
    rd = lambda a: struct.unpack_from("<I", mem, a - base)[0]
    start = XC_END - 4
    while rd(start) != 0:
        start -= 4
    inits = collections.defaultdict(list)
    for slot in range(start + 4, XC_END, 4):
        fa = rd(slot)
        imms = {}
        for ins in Decoder(32, mem[fa - base:fa - base + 0x200], ip=fa):
            if ins.mnemonic == Mnemonic.RET:
                break
            if ins.mnemonic == Mnemonic.JMP and ins.op0_kind == OpKind.NEAR_BRANCH32:
                continue  # the incremental-link style thunk in front of each init body is followed below
            if ins.mnemonic == Mnemonic.MOV and ins.op0_kind == OpKind.REGISTER:
                imms[ins.op0_register] = ins.immediate(1) if ins.op1_kind in (OpKind.IMMEDIATE32, OpKind.IMMEDIATE16) else None
            elif ins.op0_kind == OpKind.REGISTER:
                imms[ins.op0_register] = None  # any other write makes the register value unknown
            if (ins.op_count == 2 and ins.mnemonic == Mnemonic.MOV and ins.op0_kind == OpKind.MEMORY
                    and ins.memory_base == Register.NONE and ins.memory_index == Register.NONE):
                if ins.op1_kind in (OpKind.IMMEDIATE32, OpKind.IMMEDIATE16, OpKind.IMMEDIATE8):
                    value = ins.immediate(1)
                elif ins.op1_kind == OpKind.REGISTER:
                    value = imms.get(ins.op1_register)
                else:
                    value = None
                inits[ins.memory_displacement].append((WIDTH.get(ins.memory_size), value, fa))
    return inits


def our_globals():
    ours = {}
    for path in glob.glob("../../Source/*.cpp"):
        text = open(path, errors="ignore").read()
        for m in re.finditer(r"\b(DEFINE_GLOBAL(?:_INIT|_ARRAY|_ARRAY_INIT)?)\((.*?)\)\s*;", text):
            addrs = re.findall(r"0x[0-9a-fA-F]{6}\b", m.group(2))
            if addrs:
                ours[int(addrs[-1], 16)] = (m.group(1), m.group(2).split(",")[0].strip(), m.group(2), path.split("/")[-1])
    return ours


def our_value(ty, args):
    parts = [p.strip() for p in re.split(r",(?![^()]*\))", args)]
    if len(parts) < 4:
        return None
    num = r"(-?0x[0-9a-fA-F]+|-?\d+)"
    e = parts[2]
    if m := re.fullmatch(r"Fix16\(\s*%s\s*,\s*0\s*\)" % num, e):
        return int(m.group(1), 0) & 0xFFFFFFFF
    if m := re.fullmatch(r"Fix16\(\s*%s\s*\)" % num, e):
        return (int(m.group(1), 0) << 14) & 0xFFFFFFFF
    if m := re.fullmatch(r"Ang16\(\s*%s\s*(,\s*0\s*)?\)" % num, e):
        return int(m.group(1), 0) & 0xFFFF
    return None


def main():
    inits = original_inits("10.5.exe")
    ours = our_globals()
    for addr, stores in sorted(inits.items()):
        if addr not in ours:
            continue
        kind, ty, args, path = ours[addr]
        widths = {w for w, _, _ in stores}
        values = {v for _, v, _ in stores if v is not None}
        where = "%x %s (%s, init 0x%x)" % (addr, path, args[:90], stores[0][2])
        if ty not in CLASSES and "ARRAY" not in kind and ty in SIZES:
            print("POD but dynamically initialised:", where)
        elif ty in SIZES and SIZES[ty] not in widths and not (SIZES[ty] == 8 and 4 in widths):
            print("store width %s, type %s:" % (sorted(w for w in widths if w), ty), where)
        else:
            want = our_value(ty, args)
            if want is not None and values and want not in {v & (0xFFFF if ty == "Ang16" else 0xFFFFFFFF) for v in values}:
                print("value %s, ours 0x%x:" % ([hex(v) for v in values], want), where)
    for addr, (kind, ty, args, path) in sorted(ours.items()):
        if ty in CLASSES and kind == "DEFINE_GLOBAL_INIT" and addr not in inits:
            print("no original initialiser: %x %s (%s)" % (addr, path, args[:90]))


if __name__ == "__main__":
    main()
