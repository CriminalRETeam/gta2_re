import os
import re
import sys
import csv
import json
import glob
import urllib.request

DISCORD_WEBHOOK_URL = os.environ.get("DISCORD_WEBHOOK_URL")
COMMIT_MESSAGE = os.environ.get("COMMIT_MESSAGE")
# Optional: the captured output of build.py, used to count compiler warnings
BUILD_LOG = os.environ.get("BUILD_LOG", "../build.log")

# Everything at or above this address is CRT / iostream library code, not game code
LIBRARY_START = 0x5ED000

NEW_EXE = "../build_vc6/decomp_main.exe"
OG_EXE = "./bin_comp/10.5.exe"

# Keys of progress.json whose change makes us post an update
PROGRESS_KEYS = ["matched", "total", "stub", "not_started", "wip", "wip_reg_swap", "wip_reg_choice",
                 "wip_instr_order", "wip_structural", "wip_unclassified", "wip_ready", "matched_boot_to_map_funcs"]


def load_coverage_file(filename):
    with open(filename) as file:
        return [line.rstrip() for line in file]


def fmt_delta(old, new, as_percent=False):
    """' (+8)' / ' (-2)', or nothing when unchanged or there's no previous value."""
    if old is None or old == new:
        return ""
    diff = new - old
    return f" ({diff:+.2f}%)" if as_percent else f" ({diff:+d})"


def delta_with_pct(old, new, old_pct, new_pct):
    """' (+8, +0.20%)' for a count and its percentage."""
    if old is None or old == new:
        return ""
    return f" ({new - old:+d}, {new_pct - old_pct:+.2f}%)"


def pct(part, whole):
    return part / whole * 100 if whole else 0.0


def classify_wip(func, og_file_offset, og_size):
    """What kind of mismatch a WIP function has, from its asm compared with the original's."""
    import compare_function
    import post_process_asm
    import regonly

    def asm(exe, offset):
        return post_process_asm.post_process_asm(compare_function.dism_func(compare_function.get_bytes_from_file(exe, offset, og_size)))

    og = asm(OG_EXE, og_file_offset)
    new = asm(NEW_EXE, int(func["func_fo"], 16))
    if new == og:
        return "ready"  # already identical: can be promoted to MATCH_FUNC
    if regonly.canon(new) == regonly.canon(og):
        return "wip_reg_swap"
    if regonly.shape(new) == regonly.shape(og):
        return "wip_reg_choice"
    if sorted(regonly.shape(new).split("\n")) == sorted(regonly.shape(og).split("\n")):
        return "wip_instr_order"
    return "wip_structural"


def count_fields_and_methods(source_dir):
    """Named vs unnamed (field_XX / sub_XXXXXX) class fields and functions in Source/."""
    field_re = re.compile(r"^\s+[\w:<>\*&\s,]+?\bfield_([0-9A-Fa-f]+)(_\w+)?\s*(\[[^\]]*\])*\s*(:\s*\d+\s*)?;", re.M)
    named_fields = unnamed_fields = 0
    for path in glob.glob(os.path.join(source_dir, "*.hpp")):
        with open(path, errors="replace") as f:
            for m in field_re.finditer(f.read()):
                if m.group(2):
                    named_fields += 1
                else:
                    unnamed_fields += 1

    marker_re = re.compile(r"^(?:MATCH|WIP|STUB)_FUNC\(0x[0-9a-fA-F]+\)\s*$")
    name_re = re.compile(r"([A-Za-z_~][\w~]*)\s*\(")
    named_funcs = unnamed_funcs = 0
    for path in glob.glob(os.path.join(source_dir, "*.cpp")):
        with open(path, errors="replace") as f:
            lines = f.read().split("\n")
        for i, line in enumerate(lines):
            if not marker_re.match(line.strip()):
                continue
            # the signature is on the next non-blank line
            for sig in lines[i + 1:i + 4]:
                if sig.strip():
                    # skip the return type / class qualifier: take the last identifier before '('
                    head = sig.split("(")[0]
                    name = re.split(r"::|\s|\*|&", head.strip())[-1]
                    if re.match(r"^(sub|nullsub|j)_[0-9A-Fa-f]+$", name):
                        unnamed_funcs += 1
                    else:
                        named_funcs += 1
                    break
    return named_fields, unnamed_fields, named_funcs, unnamed_funcs


def count_warnings(build_log):
    """Unique compiler warnings in Source/ (the same TU is compiled for several targets)."""
    if not os.path.exists(build_log):
        return None
    warn_re = re.compile(r"Source[\\/](?!3rdParty).*\(\d+\) : warning C\d+")
    seen = set()
    with open(build_log, errors="replace") as f:
        for line in f:
            if warn_re.search(line):
                seen.add(line.strip())
    return len(seen)


def main():
    if DISCORD_WEBHOOK_URL is None:
        print("DISCORD_WEBHOOK_URL env variable not set")
        sys.exit(1)

    if COMMIT_MESSAGE is None:
        print("COMMIT_MESSAGE env variable not set")
        sys.exit(1)

    for needed in ["./bin_comp/new_data.json", "./bin_comp/coverage_trace_funcs.txt", "./bin_comp/og_function_data_v105.csv"]:
        if not os.path.exists(needed):
            print(f"couldn't find {needed}")
            sys.exit(1)

    sys.path.insert(0, os.path.abspath("./bin_comp"))
    sys.path.insert(0, os.path.abspath("./regalloc"))

    with open("./bin_comp/new_data.json", "rt") as file:
        new_data = json.load(file)
    coverage_data = {line.lower() for line in load_coverage_file("./bin_comp/coverage_trace_funcs.txt")}

    # Every game function of the original (CRT / library code is not ours to match)
    og_funcs = {}
    with open("./bin_comp/og_function_data_v105.csv") as file:
        for row in csv.reader(file):
            addr = int(row[1], 16)
            if addr < LIBRARY_START:
                og_funcs[addr] = (row[0], int(row[2], 16), int(row[3], 16))

    status_by_addr = {}
    for func in new_data["functions"]:
        if func["og_addr"].startswith("0x"):
            status_by_addr[int(func["og_addr"], 16)] = func

    total = len(og_funcs)
    matched = sum(1 for a, f in status_by_addr.items() if a in og_funcs and f["func_status"] == "0x1")
    stub = sum(1 for a, f in status_by_addr.items() if a in og_funcs and f["func_status"] == "0x0")
    wip_funcs = [f for a, f in status_by_addr.items() if a in og_funcs and f["func_status"] == "0x2"]
    wip = len(wip_funcs)
    not_started = total - matched - stub - wip

    # boot to map coverage
    matched_coverage_funcs = sum(1 for f in status_by_addr.values() if f["func_status"] == "0x1" and f["og_addr"].lower() in coverage_data)
    total_coverage_funcs = len(coverage_data)

    # what kind of mismatch each WIP has
    wip_buckets = {"wip_reg_swap": 0, "wip_reg_choice": 0, "wip_instr_order": 0, "wip_structural": 0, "wip_unclassified": 0}
    wip_ready = 0
    can_classify = os.path.exists(NEW_EXE) and os.path.exists(OG_EXE)
    for func in wip_funcs:
        if not can_classify:
            wip_buckets["wip_unclassified"] += 1
            continue
        _, og_offset, og_size = og_funcs[int(func["og_addr"], 16)]
        try:
            kind = classify_wip(func, og_offset, og_size)
        except Exception as e:
            print(f"couldn't classify {func['mangled_name']}: {e}")
            kind = "wip_unclassified"
        if kind == "ready":
            print(f"{func['mangled_name']} matches, promote it to MATCH_FUNC")
            wip_ready += 1
        else:
            wip_buckets[kind] += 1

    named_fields, unnamed_fields, named_funcs, unnamed_funcs = count_fields_and_methods("../Source")
    warnings = count_warnings(BUILD_LOG)

    new_progress_json = {
        "matched_boot_to_map_funcs": matched_coverage_funcs,
        "total_matches": matched,
        "matched": matched,
        "total": total,
        "stub": stub,
        "not_started": not_started,
        "wip": wip,
        "wip_ready": wip_ready,
        **wip_buckets,
        "named_fields": named_fields,
        "unnamed_fields": unnamed_fields,
        "named_funcs": named_funcs,
        "unnamed_funcs": unnamed_funcs,
        "warnings": warnings,
    }

    previous_progress_json = {}
    prev_json_available = True
    try:
        with open("progress.json", "r") as file:
            previous_progress_json = json.load(file)
    except OSError:
        prev_json_available = False
    prev = previous_progress_json.get

    def d(key, as_percent=False, value=None):
        new = new_progress_json[key] if value is None else value
        return fmt_delta(prev(key), new, as_percent)

    match_pct = pct(matched, total)
    prev_match_pct = pct(prev("matched"), prev("total")) if prev("matched") is not None and prev("total") else None
    boot_pct = pct(matched_coverage_funcs, total_coverage_funcs)
    prev_boot_pct = pct(prev("matched_boot_to_map_funcs"), total_coverage_funcs) if prev("matched_boot_to_map_funcs") is not None else None

    unmatched = total - matched

    def line(label, key):
        return f"{label}: **{new_progress_json[key]}**{d(key)}"

    overall = f"**{matched} / {total}** matched, **{match_pct:.2f}%**{delta_with_pct(prev('matched'), matched, prev_match_pct, match_pct)}"

    unmatched_lines = [
        line("Not started", "not_started"),
        line("Stubs", "stub"),
        f"WIP: **{wip}**{d('wip')}",
    ]
    if wip_ready:
        unmatched_lines.append(f"  - identical, ready to promote: **{wip_ready}**")
    wip_labels = [
        ("wip_reg_swap", "register swap only"),
        ("wip_reg_choice", "different registers"),
        ("wip_instr_order", "instruction order"),
        ("wip_structural", "control flow / other"),
        ("wip_unclassified", "unclassified"),
    ]
    for key, label in wip_labels:
        if key != "wip_unclassified" or wip_buckets[key] or prev(key):
            unmatched_lines.append(f"  - {label}: **{wip_buckets[key]}**{d(key)}")

    health_lines = [
        f"Fields named: **{named_fields} / {named_fields + unnamed_fields}** ({pct(named_fields, named_fields + unnamed_fields):.1f}%)"
        f"{fmt_delta(prev('named_fields'), named_fields)}",
        f"Functions named: **{named_funcs} / {named_funcs + unnamed_funcs}** ({pct(named_funcs, named_funcs + unnamed_funcs):.1f}%)"
        f"{fmt_delta(prev('named_funcs'), named_funcs)}",
    ]
    if warnings is not None:
        health_lines.append(f"Build warnings: **{warnings}**{fmt_delta(prev('warnings'), warnings)}")

    webhook_message = {
        "content": None,
        "embeds": [
            {
                "title": "Status",
                "description": COMMIT_MESSAGE,
                "fields": [
                    {"name": "All functions", "value": overall},
                    {"name": "Boot to map", "value":
                        f"**{matched_coverage_funcs} / {total_coverage_funcs}** matched, **{boot_pct:.2f}%**"
                        f"{delta_with_pct(prev('matched_boot_to_map_funcs'), matched_coverage_funcs, prev_boot_pct, boot_pct)}"},
                    {"name": f"Not matched ({unmatched})", "value": "\n".join(unmatched_lines)},
                    {"name": "Code health", "value": "\n".join(health_lines)},
                ],
            }
        ],
        "attachments": []
    }

    print(f"prev_json_available: {prev_json_available}")
    print(f"previous_progress_json: {previous_progress_json}")
    print(f"new_progress_json: {new_progress_json}")

    changed = any(previous_progress_json.get(k) != new_progress_json[k] for k in PROGRESS_KEYS)
    if not prev_json_available or changed:
        print("Posting update...")
        if DISCORD_WEBHOOK_URL != "dry-run":
            req = urllib.request.Request(DISCORD_WEBHOOK_URL, json.dumps(webhook_message).encode())
            req.add_header("Content-Type", "application/json")
            req.add_header("User-Agent", "gta2_re webhook/1.0")
            urllib.request.urlopen(req)
    else:
        print("Not posting update")

    for f in webhook_message["embeds"][0]["fields"]:
        print(f"{f['name']}\n{f['value']}\n")

    with open("progress.json", "w") as file:
        json.dump(new_progress_json, file, indent=4)


if __name__ == "__main__":
    main()
