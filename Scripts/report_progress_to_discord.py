import os
import re
import sys
import csv
import difflib
import json
import glob
import urllib.request

DISCORD_WEBHOOK_URL = os.environ.get("DISCORD_WEBHOOK_URL")
COMMIT_MESSAGE = os.environ.get("COMMIT_MESSAGE")

NEW_EXE = "../build_vc6/decomp_main.exe"
NEW_MAP = "../build_vc6/output.map"
OG_EXE = "./bin_comp/10.5.exe"

# CRT / iostream library code (stubs for fopen, malloc ...) starts here, it's not ours to match
LIBRARY_START = 0x5ED000

# Keys of progress.json whose change makes us post an update
PROGRESS_KEYS = ["matched", "wip", "wip_scores", "named_fields", "named_funcs"]


def fmt_delta(old, new, always=False, with_pct=True):
    """' | **+5** (0.43%)': the change since the previous run and it as a percentage of the old
    value. Nothing when unchanged (or ' | **No change**' with always), or without a previous run."""
    if old is None:
        return ""
    diff = new - old
    if diff == 0:
        return " | **No change**" if always else ""
    if not with_pct:
        return f" | **{diff:+d}**"
    diff_pct = diff / old * 100 if old else 0
    return f" | **{diff:+d}** ({diff_pct:.2f}%)"


def pct(part, whole):
    return part / whole * 100 if whole else 0.0


def demangle(mangled):
    """'?Name@Class@@QAEXXZ' -> 'Class::Name' (enough for display)."""
    m = re.match(r"^\?([^@]+)@([^@]*)@", mangled)
    if not m:
        return mangled.lstrip("?_")
    return f"{m.group(2)}::{m.group(1)}" if m.group(2) else m.group(1)


def code_symbol_addresses(map_file):
    """Sorted addresses of every symbol in the code section of the build's map file (publics and
    statics), to get the size of a function in the build: the distance to the next symbol."""
    sym_re = re.compile(r"^\s*0001:[0-9a-fA-F]{8}\s+\S+\s+([0-9a-fA-F]{8})\b")
    addrs = set()
    with open(map_file, errors="replace") as f:
        for line in f:
            m = sym_re.match(line)
            if m:
                addrs.add(int(m.group(1), 16))
    return sorted(addrs)


def normalize_for_score(line):
    """One instruction without what WIP_IMPLEMENTED moves around: it adds a logging prologue and an
    ebp frame, so registers, stack offsets, jump targets and the stable_name numbering all shift."""
    line = re.sub(r"stable_name_\w+", "S", line)
    line = re.sub(r"%[re]?(ax|bx|cx|dx|si|di|bp|sp)\b|%[a-d][lh]\b", "R", line)
    line = re.sub(r"-?(0x[0-9A-Fa-f]+|\d+)\(", "N(", line)
    return re.sub(r"^(j\w+|call) 0x[0-9A-Fa-f]+", r"\1 T", line)


def wip_score(func, og_file_offset, og_size, new_size):
    """(% of the original's instructions the build reproduces, instructions it doesn't) for a WIP
    function, from the verifier's post processed asm with registers, stack offsets, jump targets
    and symbol numbering ignored (normalize_for_score). The build's copy is read at its own size:
    WIP_IMPLEMENTED makes it longer than the original, and its extra lines don't count."""
    import compare_function
    import post_process_asm

    def asm(exe, offset, size):
        lines = post_process_asm.post_process_asm(compare_function.dism_func(compare_function.get_bytes_from_file(exe, offset, size))).split("\n")
        # alignment padding up to the next function
        while lines and lines[-1].strip() in ("int3", "nop", ""):
            lines.pop()
        return [normalize_for_score(line) for line in lines]

    og = asm(OG_EXE, og_file_offset, og_size)
    new = asm(NEW_EXE, int(func["func_fo"], 16), new_size)
    matcher = difflib.SequenceMatcher(None, og, new, autojunk=False)
    same = sum(block.size for block in matcher.get_matching_blocks())
    return pct(same, len(og)), len(og) - same


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


def main():
    # The embed text has non-ASCII characters; the Windows runner's console encoding (cp1252) can't print them
    sys.stdout.reconfigure(encoding="utf-8", errors="replace")
    if DISCORD_WEBHOOK_URL is None:
        print("DISCORD_WEBHOOK_URL env variable not set")
        sys.exit(1)

    if COMMIT_MESSAGE is None:
        print("COMMIT_MESSAGE env variable not set")
        sys.exit(1)

    for needed in ["./bin_comp/new_data.json", "./bin_comp/og_function_data_v105.csv"]:
        if not os.path.exists(needed):
            print(f"couldn't find {needed}")
            sys.exit(1)

    sys.path.insert(0, os.path.abspath("./bin_comp"))

    with open("./bin_comp/new_data.json", "rt") as file:
        new_data = json.load(file)

    # file offset and size of each original function, to disassemble the WIP ones
    og_funcs = {}
    with open("./bin_comp/og_function_data_v105.csv") as file:
        for row in csv.reader(file):
            og_funcs[int(row[1], 16)] = (row[0], int(row[2], 16), int(row[3], 16))

    # Only game functions with a MATCH_FUNC / WIP_FUNC / STUB_FUNC marker count
    status_by_addr = {}
    for func in new_data["functions"]:
        if func["og_addr"].startswith("0x") and func["func_status"] in ("0x0", "0x1", "0x2") and int(func["og_addr"], 16) < LIBRARY_START:
            status_by_addr[int(func["og_addr"], 16)] = func

    matched = sum(1 for f in status_by_addr.values() if f["func_status"] == "0x1")
    stub = sum(1 for f in status_by_addr.values() if f["func_status"] == "0x0")
    wip_funcs = [f for f in status_by_addr.values() if f["func_status"] == "0x2"]
    wip = len(wip_funcs)
    total = matched + stub + wip

    # how close each WIP is: % of the original's instructions reproduced and how many aren't, by original address
    wip_scores = {}
    names = {}
    can_score = os.path.exists(NEW_EXE) and os.path.exists(OG_EXE) and os.path.exists(NEW_MAP)
    symbol_addrs = code_symbol_addresses(NEW_MAP) if can_score else []
    for func in wip_funcs:
        key = func["og_addr"].lower()
        names[key] = demangle(func["mangled_name"])
        if not can_score:
            continue
        _, og_offset, og_size = og_funcs[int(func["og_addr"], 16)]
        va = int(func["func_va"], 16)
        next_addr = next((a for a in symbol_addrs if a > va), va + og_size)
        try:
            similarity, differing = wip_score(func, og_offset, og_size, next_addr - va)
        except Exception as e:
            print(f"couldn't score {func['mangled_name']}: {e}")
            continue
        wip_scores[key] = [round(similarity, 2), differing]

    named_fields, unnamed_fields, named_funcs, unnamed_funcs = count_fields_and_methods("../Source")

    new_progress_json = {
        "total_matches": matched,
        "matched": matched,
        "total": total,
        "wip": wip,
        "wip_scores": wip_scores,
        "named_fields": named_fields,
        "unnamed_fields": unnamed_fields,
        "named_funcs": named_funcs,
        "unnamed_funcs": unnamed_funcs,
    }

    previous_progress_json = {}
    prev_json_available = True
    try:
        with open("progress.json", "r") as file:
            previous_progress_json = json.load(file)
    except OSError:
        prev_json_available = False
    prev = previous_progress_json.get
    prev_scores = prev("wip_scores") or {}

    # Discord drops leading spaces, so indent with em spaces
    ind = "\u2003\u2003"

    out = [COMMIT_MESSAGE, ""]
    out.append(f"Remaining WIP functions: {wip}{fmt_delta(prev('wip'), wip, with_pct=False)}")
    if wip_scores:
        out.append(f"{ind}(scored on instructions, registers and stack offsets ignored)")
    if wip_scores:
        avg = sum(v[0] for v in wip_scores.values()) / len(wip_scores)
        prev_avg = None
        if prev_scores:
            prev_avg = sum(v[0] for v in prev_scores.values()) / len(prev_scores)
        avg_delta = f" | **{avg - prev_avg:+.2f}%**" if prev_avg is not None and round(avg - prev_avg, 2) else ""
        out.append(f"{ind}Average: {avg:.2f}% of the original instructions reproduced{avg_delta}")
        # closest first: by missing instructions, then %
        for key, (similarity, differing) in sorted(wip_scores.items(), key=lambda kv: (kv[1][1], -kv[1][0])):
            line = f"{ind}{names[key]}: {similarity:.2f}% ({differing} missing)"
            if key in prev_scores and prev_scores[key][1] != differing:
                line += f" | **{differing - prev_scores[key][1]:+d}**"
            elif key not in prev_scores and prev_scores:
                line += " | **new**"
            out.append(line)
    elif wip:
        out.append(f"{ind}(not scored: the original exe or the build is missing)")
    # WIPs from last time that are gone now (matched, or demoted to stubs)
    gone = [k for k in prev_scores if k not in names]
    if gone:
        out.append(f"{ind}Done since last time: {len(gone)}")
    out.append("")
    out.append("Renames:")
    out.append(f"{ind}Fields named [{named_fields}/{named_fields + unnamed_fields}] {pct(named_fields, named_fields + unnamed_fields):.2f}%"
               f"{fmt_delta(prev('named_fields'), named_fields)}")
    out.append(f"{ind}Functions named [{named_funcs}/{named_funcs + unnamed_funcs}] {pct(named_funcs, named_funcs + unnamed_funcs):.2f}%"
               f"{fmt_delta(prev('named_funcs'), named_funcs)}")
    out.append("")
    out.append(f"Matched: [{matched}/{total}] {pct(matched, total):.2f}%{fmt_delta(prev('matched'), matched, always=True)}")

    webhook_message = {
        "content": None,
        "embeds": [{"title": "Status", "description": "\n".join(out)}],
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

    print(webhook_message["embeds"][0]["description"])

    with open("progress.json", "w") as file:
        json.dump(new_progress_json, file, indent=4)


if __name__ == "__main__":
    main()
