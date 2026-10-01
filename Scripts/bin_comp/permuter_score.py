"""
Scores one function in a compiled .obj against target_asm.json, for cpp_permuter's --score-cmd.

The target asm was disassembled from the original exe and post processed (absolute addresses
became stable_name_N in order of first use). An .obj has relocations instead of addresses, so
every relocated operand is patched to a fake address that is unique per symbol before the
same disassembly + post processing runs. Two operands then compare equal exactly when they
reference the same symbol, the same as in the full exe build.

Usage:
    python permuter_score.py <obj> <og_addr> [symbol_substring]

Prints the score as the last number: 0 is a match, otherwise the number of target and
candidate lines that aren't part of a common run (difflib opcodes).
"""

import difflib
import json
import os
import re
import struct
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import compare_function  # noqa: E402
import post_process_asm  # noqa: E402

IMAGE_REL_I386_DIR32 = 0x06
IMAGE_REL_I386_REL32 = 0x14
FAKE_BASE = 0x10000000


class Coff:
    def __init__(self, data):
        self.data = data
        (self.machine, nsec, _, self.symtab, self.nsyms, opt, _) = struct.unpack_from("<HHIIIHH", data, 0)
        self.sections = []
        off = 20 + opt
        for i in range(nsec):
            name, vsize, vaddr, rawsize, rawptr, relptr, _, nrel, _, chars = struct.unpack_from(
                "<8sIIIIIIHHI", data, off + i * 40
            )
            self.sections.append(dict(name=name, size=rawsize, ptr=rawptr, relptr=relptr, nrel=nrel))
        strtab_off = self.symtab + self.nsyms * 18
        self.symbols = []
        i = 0
        while i < self.nsyms:
            raw = data[self.symtab + i * 18 : self.symtab + i * 18 + 18]
            name_raw, value, secnum, typ, cls, naux = struct.unpack("<8sIhHBB", raw)
            if name_raw[:4] == b"\0\0\0\0":
                soff = struct.unpack_from("<I", name_raw, 4)[0]
                end = data.index(b"\0", strtab_off + soff)
                name = data[strtab_off + soff : end].decode("latin-1")
            else:
                name = name_raw.rstrip(b"\0").decode("latin-1")
            self.symbols.append(dict(name=name, value=value, sec=secnum, type=typ, cls=cls))
            for _ in range(naux):
                self.symbols.append(None)
            i += 1 + naux

    def relocs(self, sec):
        s = self.sections[sec]
        for i in range(s["nrel"]):
            va, symidx, typ = struct.unpack_from("<IIH", self.data, s["relptr"] + i * 10)
            yield va, symidx, typ


def find_function(coff, needle):
    cands = []
    for idx, sym in enumerate(coff.symbols):
        if sym and sym["sec"] > 0 and (sym["type"] & 0x20) and needle in sym["name"]:
            cands.append(idx)
    if not cands:
        raise SystemExit(f"no function symbol containing {needle!r}")
    # Prefer the shortest name: the plain function, not a static local or a lambda inside it.
    return min(cands, key=lambda i: len(coff.symbols[i]["name"]))


def function_asm(coff, symidx):
    sym = coff.symbols[symidx]
    sec = sym["sec"] - 1
    s = coff.sections[sec]
    start = sym["value"]
    # The function ends at the next function symbol in its section, or the section end.
    end = s["size"]
    for other in coff.symbols:
        if other and other["sec"] == sym["sec"] and (other["type"] & 0x20) and start < other["value"] < end:
            end = other["value"]
    # Switch jump tables follow the code in the same section. A table's start is the target of
    # a DIR32 reloc against this section that is itself a reloc location (its first entry).
    relocs = [r for r in coff.relocs(sec) if start <= r[0] < end]
    locations = set(r[0] for r in relocs)
    for va, ridx, typ in relocs:
        rsym = coff.symbols[ridx]
        if typ == IMAGE_REL_I386_DIR32 and rsym["sec"] == sym["sec"]:
            addend = struct.unpack_from("<I", coff.data, s["ptr"] + va)[0] + rsym["value"]
            if addend in locations and start < addend < end:
                end = addend
    code = bytearray(coff.data[s["ptr"] + start : s["ptr"] + end])

    fake = {}
    for va, ridx, typ in coff.relocs(sec):
        if not start <= va < end:
            continue
        rsym = coff.symbols[ridx]
        name = rsym["name"]
        off = va - start
        if rsym["sec"] == sym["sec"]:
            # Jump table / case label inside this function: keep it, relative to the function
            # start, like the target's absolute addresses relative to the exe.
            name = "%s+%x" % (name, struct.unpack_from("<I", code, off)[0])
        if rsym["sec"] == -1 or "except_list" in name:
            # Absolute symbol (__except_list for fs:0): the exe has its real value.
            continue
        if name not in fake:
            fake[name] = FAKE_BASE + len(fake) * 0x10
        addend = 0 if rsym["sec"] == sym["sec"] else struct.unpack_from("<i", code, off)[0]
        if typ == IMAGE_REL_I386_DIR32:
            struct.pack_into("<I", code, off, (fake[name] + addend) & 0xFFFFFFFF)
        elif typ == IMAGE_REL_I386_REL32:
            # The decoder runs at ip 0, so the operand shown is next_ip + disp.
            struct.pack_into("<I", code, off, (fake[name] - (off + 4)) & 0xFFFFFFFF)
    return compare_function.dism_func(bytes(code))


# Alignment padding VC6 puts after the code (before a jump table or the next function).
PADDING = re.compile(r"^(nop|int3|mov %(\w+),%\2|lea (0x)?\w*\(%(\w+)\),%\4)$")


def function_lines(coff, symidx):
    lines = post_process_asm.post_process_asm(function_asm(coff, symidx)).split("\n")
    while lines and PADDING.match(lines[-1]):
        lines.pop()
    return lines


def main():
    obj, addr = sys.argv[1], int(sys.argv[2], 16)
    here = os.path.dirname(os.path.abspath(__file__))
    target = json.load(open(os.path.join(here, "target_asm.json")))[hex(addr)]
    needle = sys.argv[3] if len(sys.argv) > 3 else target["name"].split("::")[-1]
    coff = Coff(open(obj, "rb").read())
    ml = function_lines(coff, find_function(coff, needle))
    tl = target["pp"].split("\n")
    score = 0
    for op, i1, i2, j1, j2 in difflib.SequenceMatcher(None, tl, ml, autojunk=False).get_opcodes():
        if op != "equal":
            score += max(i2 - i1, j2 - j1)
    if "-v" in sys.argv:
        print("\n".join(difflib.unified_diff(tl, ml, "target", "candidate", lineterm="", n=2)))
    print(score)


if __name__ == "__main__":
    main()
