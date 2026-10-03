"""Prints the Rich header (compiler/linker build numbers) of PE files.

    python rich_header.py 10.5.exe 9.6f.exe ../../build_vc6/decomp_main.exe
"""
import struct
import sys

PRODUCTS = {
    0: "Unknown", 1: "Import0", 2: "Linker510", 3: "Cvtomf510", 4: "Linker600", 5: "Cvtomf600",
    6: "Cvtres500", 7: "Utc11_Basic", 8: "Utc11_C", 9: "Utc12_Basic", 10: "Utc12_C", 11: "Utc12_CPP",
    12: "AliasObj60", 13: "VisualBasic60", 14: "Masm613", 15: "Masm710", 16: "Linker511",
    17: "Cvtomf511", 18: "Masm614", 19: "Linker512", 20: "Cvtomf512", 21: "Utc12_C_Std",
    22: "Utc12_CPP_Std", 23: "Utc12_C_Book", 24: "Utc12_CPP_Book", 25: "Implib700", 26: "Cvtomf700",
    27: "Utc13_Basic", 28: "Utc13_C", 29: "Utc13_CPP", 30: "Linker610", 31: "Cvtomf610",
    32: "Linker601", 33: "Cvtomf601", 34: "Utc12_1_Basic", 35: "Utc12_1_C", 36: "Utc12_1_CPP",
    37: "Linker620", 38: "Cvtomf620", 39: "AliasObj70", 40: "Linker621", 41: "Cvtomf621",
    42: "Masm615", 43: "Utc13_LTCG_C", 44: "Utc13_LTCG_CPP", 45: "Masm620", 46: "ILAsm100",
    47: "Utc12_2_Basic", 48: "Utc12_2_C", 49: "Utc12_2_CPP", 50: "Utc12_2_C_Std",
    51: "Utc12_2_CPP_Std", 52: "Utc12_2_C_Book", 53: "Utc12_2_CPP_Book", 54: "Implib622",
    55: "Cvtomf622", 56: "Cvtres501", 57: "Utc13_C_Std", 58: "Utc13_CPP_Std", 59: "Cvtpgd1300",
    60: "Linker622", 61: "Linker700", 62: "Export622", 63: "Export700", 64: "Masm700",
    65: "Utc13_POGO_I_C", 66: "Utc13_POGO_I_CPP", 67: "Utc13_POGO_O_C", 68: "Utc13_POGO_O_CPP",
    69: "Cvtres700", 70: "Cvtres710p", 71: "Linker710p", 72: "Cvtomf710p", 73: "Export710p",
    74: "Implib710p", 75: "Masm710p", 76: "Utc1310p_C", 77: "Utc1310p_CPP",
}


def dump(path):
    data = open(path, "rb").read()
    print("==", path)
    e_lfanew = struct.unpack_from("<I", data, 0x3C)[0]
    timestamp = struct.unpack_from("<I", data, e_lfanew + 8)[0]
    opt = e_lfanew + 24
    major, minor = data[opt + 2], data[opt + 3]
    print("PE timestamp 0x%08X, linker version %d.%02d" % (timestamp, major, minor))
    rich = data.find(b"Rich", 0x80, e_lfanew)
    if rich < 0:
        print("no Rich header")
        return
    key = struct.unpack_from("<I", data, rich + 4)[0]
    pos = rich - 4
    entries = []
    while pos >= 0x80:
        a = struct.unpack_from("<I", data, pos - 4)[0] ^ key
        b = struct.unpack_from("<I", data, pos)[0] ^ key
        if a == 0x536E6144:  # "DanS" (followed by 3 zero dwords, already passed)
            break
        entries.append((a, b))
        pos -= 8
    entries.reverse()
    print("key 0x%08X" % key)
    print("%-6s %-18s %-7s %s" % ("prodid", "product", "build", "count"))
    for comp_id, count in entries:
        if comp_id == 0 and count == 0:
            continue
        prod, build = comp_id >> 16, comp_id & 0xFFFF
        print("%-6d %-18s %-7d %d" % (prod, PRODUCTS.get(prod, "?"), build, count))


for p in sys.argv[1:]:
    try:
        dump(p)
    except Exception as e:  # keep going for the other files
        print("==", p, "error:", e)
    print()
