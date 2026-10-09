#!/usr/bin/env python3
"""
Stack slot packing simulator, modelling the C2 frame pass (0x10723F8C) documented in
Scripts/regalloc/README.md ("Stack slots"), without needing a compile.

The real packing rule:
  1. Walk every local/temp referenced via a stack operand, build one list sorted by
     size ascending, then reference count descending, then first-reference order.
  2. Walk that list. A local takes the first already-created slot (in creation order)
     whose current members don't overlap its live range, if its size is at most twice
     the slot's current size (the slot then grows to that size). Otherwise it gets a
     new slot.
  3. New slots go from the bottom of the frame up, in creation order.

This lets a hypothesis ("what if the original also had a local X, referenced N times,
live from line A to line B") be checked for its resulting slot layout and total frame
size in milliseconds, instead of editing source and running a full VC6 build.

It is an approximation: it only models named locals/params you describe, not the
compiler's own IL temps ($T) from multi-use subexpressions, and "reference count" here
means textual occurrences, not IL node count (the two usually track closely for plain
reads/writes of a scalar or struct local, but calls like a.Method(x) still count as one
reference to `a`). Calibrate against a real slotlog.py dump of a function you already
understand before trusting it on a new hypothesis (see --calibrate).

Usage:
  Write a small Python list of locals and call simulate() -- see __main__ below for the
  worked example, or import this module from a one-off script.
"""
from __future__ import annotations
from dataclasses import dataclass, field


@dataclass
class Local:
    name: str
    size: int          # bytes
    refs: int           # reference count (textual occurrences is a decent proxy)
    lo: float            # live range start (e.g. first line the scope/local exists from)
    hi: float            # live range end (e.g. last line of its enclosing block, or +inf for function scope)
    first_ref: float = None  # tie-break; defaults to lo

    def __post_init__(self):
        if self.first_ref is None:
            self.first_ref = self.lo


@dataclass
class Slot:
    size: int
    members: list = field(default_factory=list)  # list[Local]
    creation_index: int = 0

    def overlaps(self, loc: Local) -> bool:
        return any(not (loc.hi <= m.lo or loc.lo >= m.hi) for m in self.members)


def simulate(locals_: list[Local]):
    """Returns (slots, total_frame_size). slots is a list of Slot in creation order;
    each slot's final byte offset from the bottom of the frame is the running sum of
    the sizes of the slots created before it."""
    order = sorted(locals_, key=lambda l: (l.size, -l.refs, l.first_ref))
    slots: list[Slot] = []
    for loc in order:
        placed = False
        for slot in slots:
            if not slot.overlaps(loc) and loc.size <= 2 * slot.size:
                slot.size = max(slot.size, loc.size)
                slot.members.append(loc)
                placed = True
                break
        if not placed:
            s = Slot(size=loc.size, members=[loc], creation_index=len(slots))
            slots.append(s)
    total = sum(s.size for s in slots)
    return slots, total


def render(slots, total, base=0):
    """base: offset of the bottom-most slot (0 = report relative offsets)."""
    lines = []
    offset = base
    for s in slots:
        names = ", ".join(f"{m.name}(sz={m.size},refs={m.refs},live={m.lo}..{m.hi})" for m in s.members)
        lines.append(f"  [{offset:>4}..{offset+s.size:>4}) size={s.size:<3} {names}")
        offset += s.size
    lines.append(f"total frame (named locals only): {total} bytes")
    return "\n".join(lines)


def _probe_tests():
    """Checks against the worked probes in Scripts/regalloc/README.md's slot table."""
    # int a, b, c; ext(&c); ext(&a); ext(&b);  -> c, a, b (decl order ignored, ref order doesn't matter, all 1 ref, function scope)
    locs = [Local("a", 4, 1, 0, 1e9, first_ref=2), Local("b", 4, 1, 0, 1e9, first_ref=3), Local("c", 4, 1, 0, 1e9, first_ref=1)]
    slots, total = simulate(locs)
    order = [s.members[0].name for s in slots]
    assert order == ["c", "a", "b"], order

    # ext(&b); ext(&a); ext(&a); ext(&a); ext(&a);  -> a (4 refs), b
    locs = [Local("a", 4, 4, 0, 1e9, first_ref=2), Local("b", 4, 1, 0, 1e9, first_ref=1)]
    slots, total = simulate(locs)
    order = [s.members[0].name for s in slots]
    assert order == ["a", "b"], order

    # { char c; } { int i; }  -> two slots (4 > 2x1), disjoint sibling blocks
    locs = [Local("c", 1, 1, 0, 1, first_ref=0), Local("i", 4, 1, 2, 3, first_ref=2)]
    slots, total = simulate(locs)
    assert len(slots) == 2 and total == 5, (slots, total)

    # { char c; } { short s; } { int i; }  -> one slot (1 -> 2 -> 4), all sibling/disjoint
    locs = [Local("c", 1, 1, 0, 1, first_ref=0), Local("s", 2, 1, 2, 3, first_ref=2), Local("i", 4, 1, 4, 5, first_ref=4)]
    slots, total = simulate(locs)
    assert len(slots) == 1 and total == 4, (slots, total)

    print("all probe checks passed")


if __name__ == "__main__":
    _probe_tests()
