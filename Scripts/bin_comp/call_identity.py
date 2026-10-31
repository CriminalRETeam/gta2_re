"""Original address of a build function: its marker (new_data.json), or for the COMDAT copy of an inline,
its row in inline_aliases.csv. Used by compare_function.pin_known_calls."""
import csv, os, re
import check_match_refs

HERE = os.path.dirname(os.path.abspath(__file__))


def map_symbols(path):
    sym = {}
    for line in open(path, errors='replace'):
        p = line.split()
        if len(p) >= 3 and re.match(r'^[0-9a-f]{4}:[0-9a-f]{8}$', p[0]) and re.match(r'^[0-9a-f]{8}$', p[2]):
            sym.setdefault(p[1], set()).add(int(p[2], 16))
    return sym


class CallIdentity:
    def __init__(self, new_data, map_path, og_exe):
        self.og = check_match_refs.Pe(og_exe)
        self.csv = {}  # original address -> (name, file offset, size)
        for r in csv.reader(open(os.path.join(HERE, 'og_function_data_v105.csv'))):
            if len(r) >= 4 and r[1].startswith('0x'):
                self.csv[int(r[1], 16)] = (r[0], int(r[2], 16), int(r[3], 16))
        self.sizes = {a: (n, size) for a, (n, _, size) in self.csv.items()}
        self.known = {}
        for f in new_data['functions']:
            if f.get('og_addr', '?').startswith('0x'):
                self.known[int(f['func_va'], 16)] = int(f['og_addr'], 16)
        sym = map_symbols(map_path)
        self.pinned = set()  # original addresses whose calls are compared by address
        self.aliases = []  # (decorated name, build address, original address)
        for row in csv.reader(open(os.path.join(HERE, 'inline_aliases.csv'))):
            if not row or row[0].startswith('#'):
                continue
            name, og = row[0].strip(), int(row[1], 16)
            if og not in self.csv:
                raise ValueError('alias %s: %s is not in og_function_data_v105.csv' % (name, row[1]))
            self.pinned.add(og)
            addresses = sym.get(name, set())
            if len(addresses) > 1:
                raise ValueError('alias %s has several addresses' % name)
            for va in addresses:
                if self.known.get(va, og) != og:
                    raise ValueError('alias %s contradicts its marker' % name)
                self.known[va] = og
                self.aliases.append((name, va, og))

    def original_of(self, build_va):
        return self.known.get(build_va)

    def same_original_code(self, a, b):
        return check_match_refs.same_code(self.og, self.sizes, a, b)
