"""
Prints a function's asm with esp relative operands rewritten as fixed frame offsets, so the stack
slots of the original and of a build line up even where pushes have moved esp.

    python frame_slots.py target <addr>            # original, from target_asm.json
    python frame_slots.py obj <file.obj> <needle>  # a compiled function (as permuter_score.py)

{F0x38} is the offset from esp right after the prologue (after sub esp and the register pushes).
Calls are assumed to pop the arguments pushed since the previous call or branch (thiscall/stdcall) unless
an `add $n,%esp` follows before the next call or jump (cdecl). Diff the two outputs to see which
local sits in which slot.
"""

import json
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import permuter_score  # noqa: E402

ESP = re.compile(r"(-?0x[0-9A-Fa-f]+|-?\d+)?\(%esp\)")
SAVED = re.compile(r"^push %e(bx|bp|si|di)$")


def num(s):
    return int(s, 16) if "0x" in s else int(s)


def frame_lines(lines):
    lines = [re.sub(r"^\s*[0-9a-f]+:\s*", "", l).strip() for l in lines if l.strip()]
    out = []
    depth = 0  # bytes pushed since the end of the prologue
    pending = 0  # argument bytes pushed since the last call
    in_prologue = True
    for i, l in enumerate(lines):
        m = ESP.search(l)
        if m and not in_prologue:
            off = num(m.group(1)) if m.group(1) else 0
            l = l.replace(m.group(0), "{F%s}" % hex(off - depth))
        out.append(l)
        if in_prologue:
            nxt = lines[i + 1] if i + 1 < len(lines) else ""
            started = l.startswith("sub $") or SAVED.match(l)
            if started and not SAVED.match(nxt):
                in_prologue = False
            continue
        if l.endswith(",%esp") and (l.startswith("sub $") or l.startswith("add $")):
            n = num(l[5 : l.index(",")])
            depth += n if l.startswith("sub") else -n
            if l.startswith("add"):
                pending = 0
        elif l.startswith("push"):
            depth += 4
            pending += 4
        elif l.startswith("pop"):
            depth -= 4
        elif l.startswith("j"):
            # Arguments are pushed right before their call: pushes before a branch are saved
            # registers of a split prologue.
            pending = 0
        elif l.startswith("call"):
            cdecl = False
            for l2 in lines[i + 1 : i + 12]:
                if l2.startswith(("call", "j", "ret")):
                    break
                if l2.startswith("add $") and l2.endswith(",%esp"):
                    cdecl = True
                    break
            if not cdecl:
                depth -= pending
                pending = 0
    return out


def main():
    if sys.argv[1] == "target":
        here = os.path.dirname(os.path.abspath(__file__))
        lines = json.load(open(os.path.join(here, "target_asm.json")))["0x" + sys.argv[2].lower()]["asm"].split("\n")
    else:
        coff = permuter_score.Coff(open(sys.argv[2], "rb").read())
        lines = permuter_score.function_asm(coff, permuter_score.find_function(coff, sys.argv[3])).split("\n")
    print("\n".join(frame_lines(lines)))


if __name__ == "__main__":
    main()
