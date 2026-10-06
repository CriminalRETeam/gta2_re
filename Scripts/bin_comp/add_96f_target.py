"""
Adds 9.6f functions that match_96f.py left unpaired to the local target_96f.json (and the pair
to match_96f.json), so permuter_score.py --96f, permute.sh --96f and show_96f.py work for them.
Both json files are git-ignored local copies; rerun after fetching them again.

    python add_96f_target.py <105_addr>=<96f_addr> [...]

Find candidate counterparts with find_96f_counterparts.py.
"""
import csv
import json
import sys

from iced_x86 import Decoder, Formatter, FormatterSyntax

rows = {int(r[1], 16): r for r in csv.reader(open("og_function_data_v96f.csv")) if r[1].startswith("0x")}
targets = json.load(open("target_96f.json"))
match = json.load(open("match_96f.json"))
data = open("9.6f.exe", "rb").read()
fmt = Formatter(FormatterSyntax.GAS)
for arg in sys.argv[1:]:
    a105, a96 = (int(x, 16) for x in arg.split("="))
    name, _, off, size = rows[a96][0], None, int(rows[a96][2], 16), int(rows[a96][3], 16)
    dec = Decoder(32, data[off : off + size], ip=a96)
    asm = "\n".join("%x: %s" % (i.ip, fmt.format(i)) for i in dec)
    targets[hex(a96)] = {"name": name, "size": size, "asm": asm}
    match["pairs"][hex(a105)] = hex(a96)
    print(hex(a105), "->", hex(a96), name, size)
json.dump(targets, open("target_96f.json", "w"))
json.dump(match, open("match_96f.json", "w"))
