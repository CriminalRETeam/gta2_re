"""
Write fingerprints.json: per function of 10.5.exe and 9.6f.exe (from the og csvs) the call
targets, referenced globals, strings, float constants and the mnemonic sequence. No code bytes,
just enough to pair 10.5 functions with their 9.6f versions offline (see match_96f.py).
Usage: dump_fingerprints.py [exe csv key]...   (default: both original exes)
"""
import json
import struct
import sys
from iced_x86 import Decoder, Formatter, FormatterSyntax, OpKind, Register, FlowControl


class Pe:
    def __init__(self, path):
        self.b = open(path, "rb").read()
        b = self.b
        pe = struct.unpack_from("<I", b, 0x3C)[0]
        nsec = struct.unpack_from("<H", b, pe + 6)[0]
        opt = struct.unpack_from("<H", b, pe + 20)[0]
        self.base = struct.unpack_from("<I", b, pe + 24 + 28)[0]
        self.secs = []
        for i in range(nsec):
            o = pe + 24 + opt + i * 40
            name = b[o:o + 8].rstrip(b"\0").decode("latin1")
            vsz, va, rsz, rptr = struct.unpack_from("<IIII", b, o + 8)
            self.secs.append((name, self.base + va, max(vsz, rsz), rptr, rsz))
        self.end = max(s[1] + s[2] for s in self.secs)
        self.imports = {}
        imp_rva = struct.unpack_from("<I", b, pe + 24 + 104)[0]
        if imp_rva:
            d = self.base + imp_rva
            while True:
                desc = self.read(d, 20)
                if desc is None:
                    break
                ilt, _, _, name_rva, iat = struct.unpack("<IIIII", desc)
                if not name_rva:
                    break
                dll = self.cstr(self.base + name_rva) or "?"
                thunk = ilt or iat
                k = 0
                while True:
                    t = self.read(self.base + thunk + 4 * k, 4)
                    if t is None:
                        break
                    v = struct.unpack("<I", t)[0]
                    if not v:
                        break
                    if v & 0x80000000:
                        nm = f"{dll}#{v & 0xFFFF}"
                    else:
                        nm = self.cstr(self.base + v + 2) or "?"
                    self.imports[self.base + iat + 4 * k] = nm
                    k += 1
                d += 20

    def sec(self, va):
        for s in self.secs:
            if s[1] <= va < s[1] + s[2]:
                return s
        return None

    def read(self, va, n):
        s = self.sec(va)
        if s is None or va - s[1] + n > s[4]:
            return None
        o = s[3] + va - s[1]
        return self.b[o:o + n]

    def cstr(self, va, maxlen=256):
        r = self.read(va, 1)
        if r is None:
            return None
        s = self.sec(va)
        o = s[3] + va - s[1]
        z = self.b.find(b"\0", o, o + maxlen)
        if z < 0:
            return None
        return self.b[o:z].decode("latin1")


def text_like(s):
    return s is not None and len(s) >= 3 and all(32 <= ord(c) < 127 or c in "\t\r\n" for c in s)


def fingerprint(pe, va, fo, size):
    code = pe.b[fo:fo + size]
    fmt = Formatter(FormatterSyntax.GAS)
    calls, globs, strs, flts, mn = [], [], [], [], []
    lo, hi = va, va + size
    for ins in Decoder(32, code, ip=va):
        m = fmt.format_mnemonic(ins)
        mn.append(m)
        fc = ins.flow_control
        for i in range(ins.op_count):
            k = ins.op_kind(i)
            if k == OpKind.NEAR_BRANCH32:
                t = ins.near_branch_target
                if fc == FlowControl.CALL:
                    calls.append(t)
                elif not (lo <= t < hi):
                    calls.append(t)  # tail call
            elif k == OpKind.MEMORY:
                if ins.memory_base == Register.NONE or ins.memory_index != Register.NONE:
                    a = ins.memory_displacement & 0xFFFFFFFF
                    if a in pe.imports:
                        calls.append(pe.imports[a])
                    elif pe.base <= a < pe.end:
                        sec = pe.sec(a)
                        if m.startswith("f") and ins.memory_index == Register.NONE and sec and sec[0] != ".data":
                            raw = pe.read(a, 8)
                            if raw is not None:
                                flts.append(m + ":" + raw.hex())
                                continue
                        globs.append(a)
            elif k in (OpKind.IMMEDIATE32, OpKind.IMMEDIATE8TO32):
                a = ins.immediate(i) & 0xFFFFFFFF
                if pe.base <= a < pe.end:
                    if lo <= a < hi:
                        continue
                    s = pe.cstr(a)
                    if text_like(s):
                        strs.append(s)
                    else:
                        globs.append(a)
    return {"size": size, "c": calls, "g": globs, "s": strs, "f": flts, "m": " ".join(mn)}


def dump(exe, csv):
    pe = Pe(exe)
    out = {}
    for line in open(csv):
        rec = line.rstrip().split(",")
        if len(rec) < 4:
            continue
        va, fo, size = int(rec[1], 16), int(rec[2], 16), int(rec[3], 16)
        fp = fingerprint(pe, va, fo, size)
        fp["name"] = rec[0]
        out[hex(va)] = fp
    return out


if __name__ == "__main__":
    args = sys.argv[1:] or ["10.5.exe", "og_function_data_v105.csv", "v105",
                            "9.6f.exe", "og_function_data_v96f.csv", "v96f"]
    res = {}
    for i in range(0, len(args), 3):
        res[args[i + 2]] = dump(args[i], args[i + 1])
        print(f"{args[i + 2]}: {len(res[args[i + 2]])} functions")
    with open("fingerprints.json", "w") as f:
        json.dump(res, f, separators=(",", ":"))
