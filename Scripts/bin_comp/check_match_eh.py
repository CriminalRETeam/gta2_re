"""
For every MATCH function with C++ exception handling (/GX), compares its EH data with 10.5.exe's.

Functions that destroy locals on unwind start with `push -1; push __ehhandler`. The handler is
`mov eax, __ehfuncinfo; jmp ___CxxFrameHandler`, and the FuncInfo it loads lists, per EH state, the
state to go back to and an unwind funclet placed after all the code (`lea ecx, [ebp-N]; jmp ~T`).
compare_all_functions.py only reads the function's own bytes and renames the handler address, so a
local destroyed from the wrong frame slot, with the wrong destructor, or from another state, still
matches: Object::IntegrateMovementAndCollisions_523BF0 destroys [ebp-0x14] and [ebp-0x1C] where
10.5.exe destroys [ebp-0x44] and [ebp-0x14].

This checks, per function: that both have an EH frame or neither, that the handler goes to
__CxxFrameHandler, the FuncInfo magic and maxState, each unwind entry (toState, and the funclet's
instructions with frame offsets compared exactly and calls mapped to 10.5.exe through ref_values.py),
and the try block map with its catch handlers (types by name, handler code normalised).

Differences reviewed as equivalent are in REVIEWED and left out; check a new one against 10.5.exe
before adding it there.

Usage (from Scripts/bin_comp, after build.py): python3 check_match_eh.py [-v] [address ...]
-v prints the unwind map of both sides for every function checked.
Exit code 1 when something differs.
"""
import struct
import sys

from iced_x86 import Decoder, FlowControl, Formatter, FormatterSyntax, MemorySizeExt, Mnemonic, OpKind, Register

import ref_values

# Differences that do the same as 10.5.exe (reviewed by reading both)
REVIEWED = {
}
OG_CXX_FRAME_HANDLER = 0x5ED587  # ___CxxFrameHandler in 10.5.exe (no reccmp/library.csv entry)
EH_MAGIC = 0x19930520
FRAME_REGS = (Register.EBP, Register.ESP)
FORMATTER = Formatter(FormatterSyntax.MASM)


def eh_handler(pe, va, size):
    """The handler address pushed by the function's EH prologue (push -1; push handler), or None."""
    insns = list(Decoder(32, pe.read(va, min(size, 0x40)), ip=va))[:8]
    for i, j in zip(insns, insns[1:]):
        if i.mnemonic == Mnemonic.PUSH and i.op0_kind == OpKind.IMMEDIATE8TO32 and \
                i.immediate(0) & 0xFFFFFFFF == 0xFFFFFFFF and \
                j.mnemonic == Mnemonic.PUSH and j.op0_kind == OpKind.IMMEDIATE32:
            return j.immediate(0)
    return None


def func_info(pe, handler):
    """(FuncInfo address, frame handler jumped to) from `mov eax, FuncInfo; jmp handler`."""
    insns = list(Decoder(32, pe.read(handler, 16), ip=handler))
    if len(insns) < 2 or insns[0].mnemonic != Mnemonic.MOV or insns[0].op1_kind != OpKind.IMMEDIATE32 or \
            insns[1].mnemonic != Mnemonic.JMP or insns[1].op0_kind != OpKind.NEAR_BRANCH32:
        return None, None
    return insns[0].immediate(1), insns[1].near_branch_target


def read_func_info(pe, fi):
    magic, max_state, p_unwind, n_try, p_try = struct.unpack('<IiIiI', pe.read(fi, 20))
    unwind = [struct.unpack('<iI', pe.read(p_unwind + 8 * s, 8)) for s in range(max(0, min(max_state, 4096)))]
    tries = []
    for t in range(max(0, min(n_try, 256))):
        low, high, catch_high, n_catch, p_handlers = struct.unpack('<iiiiI', pe.read(p_try + 20 * t, 20))
        catches = [struct.unpack('<IIiI', pe.read(p_handlers + 16 * c, 16)) for c in range(max(0, min(n_catch, 64)))]
        tries.append((low, high, catch_high, catches))
    return magic, max_state, unwind, tries


def funclet(pe, va, limit=40):
    """The funclet's instructions up to its ret or jmp out."""
    out = []
    for i in Decoder(32, pe.read(va, 0x100), ip=va):
        if i.is_invalid:
            break
        out.append(i)
        if i.flow_control in (FlowControl.RETURN, FlowControl.UNCONDITIONAL_BRANCH, FlowControl.INDIRECT_BRANCH) \
                or len(out) >= limit:
            break
    return out


def describe(refs, insns, og):
    out = []
    for i in insns:
        text = FORMATTER.format(i)
        if i.op_count and i.op0_kind == OpKind.NEAR_BRANCH32:
            t = i.near_branch_target
            text = '%s %s' % (FORMATTER.format_mnemonic(i), refs.og_name(t) if og else refs.new_name(t))
        out.append(text)
    return '; '.join(out)


def insn_diff(refs, x, y, xstart, ystart):
    """None when the two funclet instructions do the same, frame offsets compared exactly."""
    if x.code != y.code or x.op_count != y.op_count:
        return 'instruction'
    for o in range(x.op_count):
        k = x.op_kind(o)
        if k != y.op_kind(o):
            return 'operand kind'
        if k == OpKind.REGISTER and x.op_register(o) != y.op_register(o):
            return 'register'
        if k == OpKind.MEMORY:
            if (x.memory_base, x.memory_index, x.memory_index_scale, x.memory_segment) != \
                    (y.memory_base, y.memory_index, y.memory_index_scale, y.memory_segment):
                return 'address registers'
            xd, yd = x.memory_displacement, y.memory_displacement
            if x.memory_base in FRAME_REGS or not (refs.og.inside(xd) and refs.new.inside(yd)):
                if xd != yd:
                    return 'frame offset' if x.memory_base in FRAME_REGS else 'displacement'
            else:
                e = refs.diff(xd, yd, ref_size(x))
                if e:
                    return e
        if k in (OpKind.IMMEDIATE8, OpKind.IMMEDIATE16, OpKind.IMMEDIATE32, OpKind.IMMEDIATE8TO32):
            xi, yi = x.immediate(o), y.immediate(o)
            if xi != yi:
                if not (refs.og.inside(xi) and refs.new.inside(yi)):
                    return 'immediate'
                e = refs.diff(xi, yi)
                if e:
                    return e
        if k == OpKind.NEAR_BRANCH32:
            xt, yt = x.near_branch_target, y.near_branch_target
            if x.flow_control == FlowControl.CONDITIONAL_BRANCH:
                if xt - xstart != yt - ystart:
                    return 'branch'
            else:
                e = refs.diff(xt, yt)
                if e:
                    return e
    return None


def ref_size(insn):
    n = MemorySizeExt.size(insn.memory_size)
    return n if n and insn.memory_index == Register.NONE else None


def funclet_diff(refs, xa, ya):
    if not xa or not ya:
        return None if xa == ya else 'only one side has a funclet'
    a, b = funclet(refs.og, xa), funclet(refs.new, ya)
    if len(a) != len(b):
        return 'different length'
    for x, y in zip(a, b):
        e = insn_diff(refs, x, y, xa, ya)
        if e:
            return e
    return None


def permuted(refs, ux, uy):
    """Each of 10.5.exe's funclets is one of the build's, on another state."""
    left = [a for _, a in uy]
    for _, a in ux:
        k = next((k for k, b in enumerate(left) if funclet_diff(refs, a, b) is None), None)
        if k is None:
            return False
        del left[k]
    return True


def catch_code_diff(refs, xa, ya):
    a, b = ref_values.code_until_return(refs.og, xa, 0x1000), ref_values.code_until_return(refs.new, ya, 0x1000)
    if a is None or b is None or not ref_values.same_shape(refs.og, refs.new, a, b, xa, ya):
        return 'catch block code'
    return None


def type_name(pe, td):
    return ref_values.Refs.c_string(pe, td + 8, 1) if td else '...'


def compare_eh(refs, frame_handler, oa, na, size):
    """The list of differences between the EH data of 10.5.exe's function at oa and the build's at na."""
    og, new = refs.og, refs.new
    hx, hy = eh_handler(og, oa, size), eh_handler(new, na, size)
    if hx is None or hy is None:
        return [] if hx == hy else ['EH frame only in %s' % ('10.5.exe' if hx else 'the build')], False
    fx, jx = func_info(og, hx)
    fy, jy = func_info(new, hy)
    if fx is None or fy is None:
        return ['EH handler not understood (10.5 %x, the build %s)' % (hx, refs.new_name(hy))], True
    problems = []
    if jx != OG_CXX_FRAME_HANDLER or jy != frame_handler:
        problems.append('frame handler: 10.5 %s, the build %s' % (refs.og_name(jx), refs.new_name(jy)))
    mx, sx, ux, tx = read_func_info(og, fx)
    my, sy, uy, ty = read_func_info(new, fy)
    if mx != my or mx != EH_MAGIC:
        problems.append('FuncInfo magic 10.5 %x, build %x' % (mx, my))
    if sx != sy:
        problems.append('maxState: 10.5 %d, the build %d' % (sx, sy))
    for s, ((tox, ax), (toy, ay)) in enumerate(zip(ux, uy)):
        e = 'toState 10.5 %d, the build %d' % (tox, toy) if tox != toy else funclet_diff(refs, ax, ay)
        if e:
            problems.append('state %d: %s: 10.5 [%s], the build [%s]' % (
                s, e, describe(refs, funclet(og, ax), True) if ax else '-',
                describe(refs, funclet(new, ay), False) if ay else '-'))
    if problems and sx == sy and [t for t, _ in ux] == [t for t, _ in uy] and permuted(refs, ux, uy):
        problems.insert(0, 'the same funclets on other states: the objects are constructed in another order')
    if len(tx) != len(ty):
        problems.append('try blocks: 10.5 %d, the build %d' % (len(tx), len(ty)))
    for t, (a, b) in enumerate(zip(tx, ty)):
        if a[:3] != b[:3] or len(a[3]) != len(b[3]):
            problems.append('try block %d: 10.5 %s, the build %s' % (t, a[:3] + (len(a[3]),), b[:3] + (len(b[3]),)))
            continue
        for c, ((adjx, tdx, dispx, hx2), (adjy, tdy, dispy, hy2)) in enumerate(zip(a[3], b[3])):
            if adjx != adjy or dispx != dispy or type_name(og, tdx) != type_name(new, tdy):
                problems.append('try block %d catch %d: 10.5 (%x, %s, %d), the build (%x, %s, %d)' % (
                    t, c, adjx, type_name(og, tdx), dispx, adjy, type_name(new, tdy), dispy))
            e = catch_code_diff(refs, hx2, hy2)
            if e:
                problems.append('try block %d catch %d: %s' % (t, c, e))
    return problems, True


def unwind_map_text(refs, pe, va, size, og):
    h = eh_handler(pe, va, size)
    if h is None:
        return 'no EH frame'
    fi, _ = func_info(pe, h)
    if fi is None:
        return 'EH handler not understood'
    _, max_state, unwind, tries = read_func_info(pe, fi)
    lines = ['maxState %d, %d try blocks' % (max_state, len(tries))]
    for s, (to, act) in enumerate(unwind):
        lines.append('state %d -> %d: %s' % (s, to, describe(refs, funclet(pe, act), og) if act else '-'))
    return '\n      '.join(lines)


def print_difference(d):
    print('%s (0x%x):' % (d['name'], d['address']))
    for p in d['details'][:16]:
        print('   ' + p)


def find_differences(refs, only=(), verbose=False):
    """(summary line, [{'address', 'name', 'details'}] of each MATCH function, or each one in only, that differs).
    verbose prints the unwind map of every function checked."""
    frame_handler = refs.by_name.get('___CxxFrameHandler')
    checked = with_eh = 0
    found = []
    for f in refs.nd['functions']:
        if f.get('func_status') != '0x1' or not f.get('og_addr', '?').startswith('0x'):
            continue
        oa, na = int(f['og_addr'], 16), int(f['func_va'], 16)
        if only and oa not in only or oa not in refs.sizes or (oa in REVIEWED and not only):
            continue
        name, size = refs.sizes[oa]
        checked += 1
        problems, has_eh = compare_eh(refs, frame_handler, oa, na, size)
        with_eh += has_eh
        if problems:
            found.append({'address': oa, 'name': name, 'details': problems})
            if verbose:
                print_difference(found[-1])
        if verbose and has_eh:
            print('%s (0x%x)\n   10.5:  %s\n   build: %s' % (name, oa, unwind_map_text(refs, refs.og, oa, size, True),
                                                       unwind_map_text(refs, refs.new, na, size, False)))
    return '%d MATCH functions checked, %d with an EH frame, %d differ' % (checked, with_eh, len(found)), found


def main():
    args = sys.argv[1:]
    verbose = '-v' in args
    summary, found = find_differences(ref_values.Refs(), {int(a, 16) for a in args if a != '-v'}, verbose)
    if not verbose:
        for d in found:
            print_difference(d)
    print(summary)
    sys.exit(1 if found else 0)


if __name__ == '__main__':
    main()
