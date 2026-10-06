"""
Scores one function in a compiled .obj against target_asm.json, for cpp_permuter's --score-cmd.

The target asm was disassembled from the original exe and post processed (absolute addresses
became stable_name_N in order of first use). An .obj has relocations instead of addresses, so
every relocated operand is patched to a fake address that is unique per symbol before the
same disassembly + post processing runs. Two operands then compare equal exactly when they
reference the same symbol, the same as in the full exe build.

Usage:
    python permuter_score.py <obj> <og_addr> [symbol_substring]
    python permuter_score.py --96f <obj> <96f_addr> <symbol_substring>
    python permuter_score.py --structure [--96f] <obj> <addr> <symbol_substring>

--structure ignores register allocation: register names, stack offsets, jump targets (byte offsets
that move with any size change) and register-to-register moves are masked or dropped, so what is left
is the instruction kinds, block layout, call order and the symbols touched. 0 there means the control
flow and calls match and only register allocation / stack layout differ.

--skeleton goes further and keeps only the control flow skeleton: jumps (kind only), calls (with their
targets, in order) and returns. Instruction scheduling inside a block doesn't count, so 0 means the
branches, block order and call order match.

Unless --no-callees is given, each call to a function named after an address (Name_ADDRESS) that the
original doesn't call adds 2. The asm comparison only checks that the call targets map one to one,
so a call to the wrong named function would otherwise score 0 when that function is called once.
A call to an inline's out-of-line COMDAT copy (??HFix16@@...) is not counted: the original's linker
folded those copies to one address (0x408660), and the build's verifier accepts them as well. Only
for targets from target_asm.json (the original's asm); see callee_penalty.

--96f scores against the 9.6f build instead (target_96f.json, built with VC7.0 and no inlining;
compile the candidate with Scripts/compile_vc7.sh). Its absolute addresses are first rewritten to
the start-relative form of target_asm.json.

Prints a unified diff of the post processed asm, then the score as the last number: 0 is a
match (see score_lines for how the rest is counted).
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
FAKE_BASE = 0x7E000000  # not a power of two, so a bit mask immediate (1 << 28) is never taken for a symbol


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
            fake[name] = FAKE_BASE + len(fake) * 0x10000  # room for symbol+offset operands
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


STABLE = re.compile(r"stable_name_\d+")


def score_lines(tl, ml):
    """0 for identical asm. Lines are aligned with the symbol names masked, so a symbol that
    only differs in its stable_name number (they are numbered by first use, so one early
    difference renumbers everything after it) doesn't cascade. Each unaligned line costs 2,
    and each aligned line whose symbols break a consistent target<->candidate mapping costs 1."""
    tmask = [STABLE.sub("SYM", l) for l in tl]
    mmask = [STABLE.sub("SYM", l) for l in ml]
    score = 0
    fwd, rev = {}, {}
    for op, i1, i2, j1, j2 in difflib.SequenceMatcher(None, tmask, mmask, autojunk=False).get_opcodes():
        if op != "equal":
            score += 2 * max(i2 - i1, j2 - j1)
            continue
        for k in range(i2 - i1):
            bad = False
            for a, b in zip(STABLE.findall(tl[i1 + k]), STABLE.findall(ml[j1 + k])):
                if fwd.setdefault(a, b) != b or rev.setdefault(b, a) != a:
                    bad = True
            score += bad
    return score


ADDR_KEY = re.compile(r"_([0-9A-Fa-f]{6})(?:@|$)")
# Functions the source names after an address: an original callee counts only when the source has
# a function for it, so a callee nobody has named yet doesn't cost anything.
SOURCE_ADDRS = None


def source_addrs():
    global SOURCE_ADDRS
    if SOURCE_ADDRS is None:
        src = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "Source")
        found = set()
        for root, _, files in os.walk(src):
            for f in files:
                if f.endswith((".cpp", ".hpp", ".h")):
                    text = open(os.path.join(root, f), errors="replace").read()
                    found.update(m.lower() for m in re.findall(r"\w_([0-9A-Fa-f]{6})\b", text))
        SOURCE_ADDRS = found
    return SOURCE_ADDRS


def callee_penalty(coff, symidx, target, addr):
    """2 for each call of the candidate to a function named after an address (the REL32
    relocations whose symbol ends in _ADDRESS) beyond the original's calls to that address
    (its absolute call targets). Returns (penalty, the original's calls the candidate doesn't
    make to a named function, the candidate's extra calls)."""
    from collections import Counter

    orig = Counter()
    for l in target["asm"].split("\n"):
        m = re.match(r"call 0x([0-9A-Fa-f]+)$", l.strip())
        if m:
            orig["%06x" % ((addr + int(m.group(1), 16)) & 0xFFFFFFFF)] += 1
    orig["%06x" % addr] += 1000  # self-recursion: the target has "call 0", not the address
    sym = coff.symbols[symidx]
    sec = sym["sec"] - 1
    start, end = sym["value"], coff.sections[sec]["size"]
    for other in coff.symbols:
        if other and other["sec"] == sym["sec"] and (other["type"] & 0x20) and start < other["value"] < end:
            end = other["value"]
    ours = Counter()
    for va, ridx, typ in coff.relocs(sec):
        if start <= va < end and typ == IMAGE_REL_I386_REL32:
            m = ADDR_KEY.search(coff.symbols[ridx]["name"])
            if m and m.group(1).lower() in source_addrs():
                ours[m.group(1).lower()] += 1
    missing = Counter({a: n for a, n in (orig - ours).items() if a in source_addrs() and a != "%06x" % addr})
    extra = ours - orig
    return 2 * sum(extra.values()), missing, extra


def target_lines(target):
    # A self-recursive call is dumped as "call 0" (relative to the function start), and
    # post-processing then names the bare "0" and rewrites every 0 in the stored "pp".
    # Re-process the raw asm with that call pointed at a fake address instead.
    asm = target["asm"].split("\n")
    if "call 0" not in asm:
        return target["pp"].split("\n")
    asm = ["call 0x%X" % (FAKE_BASE - 0x10) if l == "call 0" else l for l in asm]
    return post_process_asm.post_process_asm("\n".join(asm)).split("\n")


def target_from_96f(t, addr):
    # 9.6f lines are "addr: insn" with absolute jump/call targets. target_asm.json has them
    # relative to the function start, so rewrite them the same way and post process.
    lines = []
    for l in t["asm"].split("\n"):
        l = re.sub(r"^[0-9a-f]+:\s*", "", l).strip()
        if not l:
            continue
        m = re.match(r"^(call|j\w+) 0x([0-9A-Fa-f]+)$", l)
        if m:
            l = "%s 0x%08X" % (m.group(1), (int(m.group(2), 16) - addr) & 0xFFFFFFFF)
        lines.append(l)
    asm = "\n".join(lines)
    return {"name": t["name"], "size": t["size"], "asm": asm, "pp": post_process_asm.post_process_asm(asm)}


REG = re.compile(r"%(e?[abcd]x|[abcd][lh]|e?si|e?di|e?bp)\b")
STACK = re.compile(r"(-?0x[0-9A-Fa-f]+|-?\d+)?\(%esp(,[^)]*)?\)")


def structure_lines(lines):
    """Masks register allocation: see --structure in the module docstring."""
    out = []
    for l in lines:
        l = re.sub(r"^(j\w+) 0x[0-9A-Fa-f]+$", r"\1 L", l)
        l = STACK.sub("S", l)
        l = REG.sub("%r", l)
        if re.match(r"^(mov|xchg) %r,%r$", l):
            continue
        out.append(l)
    return out


def skeleton_lines(lines):
    return [l for l in structure_lines(lines) if re.match(r"^(j\w+|call\w*|ret)\b", l)]  # call\w*: also calll *ptr


def main():
    skeleton = "--skeleton" in sys.argv
    if skeleton:
        sys.argv.remove("--skeleton")
    structure = "--structure" in sys.argv
    if structure:
        sys.argv.remove("--structure")
    callees = "--no-callees" not in sys.argv
    if not callees:
        sys.argv.remove("--no-callees")
    v96 = "--96f" in sys.argv
    if v96:
        sys.argv.remove("--96f")
    obj, addr = sys.argv[1], int(sys.argv[2], 16)
    here = os.path.dirname(os.path.abspath(__file__))
    if v96:
        target = target_from_96f(json.load(open(os.path.join(here, "target_96f.json")))[hex(addr)], addr)
    else:
        target = json.load(open(os.path.join(here, "target_asm.json"))).get(hex(addr))
    if target is None:
        # Its call offsets are relative to our build's layout, not the original's.
        callees = False
        # A MATCH_FUNC: its asm in a verified build is the original's (dump_matched_asm.py).
        target = json.load(open(os.path.join(here, "matched_asm.json")))[hex(addr)]
    needle = sys.argv[3] if len(sys.argv) > 3 else target["name"].split("::")[-1]
    coff = Coff(open(obj, "rb").read())
    symidx = find_function(coff, needle)
    ml = function_lines(coff, symidx)
    tl = target_lines(target)
    if skeleton:
        tl, ml = skeleton_lines(tl), skeleton_lines(ml)
    elif structure:
        tl, ml = structure_lines(tl), structure_lines(ml)
    score = score_lines(tl, ml)
    if callees and not v96:
        penalty, missing, extra = callee_penalty(coff, symidx, target, addr)
        if penalty:
            print("callees: candidate calls %s, the original doesn't (+%d); original calls %s"
                  % (sorted(extra.elements()), penalty, sorted(missing.elements())))
        score += penalty
    # cpp_permuter keeps this output as score_output.txt next to each improvement.
    print("\n".join(difflib.unified_diff(tl, ml, "target", "candidate", lineterm="", n=2)))
    print(score)


if __name__ == "__main__":
    main()
