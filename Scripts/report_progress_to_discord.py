import os
import sys
import json
import urllib.request

DISCORD_WEBHOOK_URL = os.environ.get("DISCORD_WEBHOOK_URL")
COMMIT_MESSAGE = os.environ.get("COMMIT_MESSAGE")

def load_coverage_file(filename):
    with open(filename) as file:
        lines = [line.rstrip() for line in file]
        return lines

def calc_difference(old_count, new_count, as_string = False):
    diff_count = new_count - old_count
    diff_percentage = (diff_count / old_count) * 100 if old_count != 0 else 0

    if as_string:
        return get_difference_str(diff_count, diff_percentage)
    else:
        return (diff_count, diff_percentage)

def get_difference_str(diff_count, diff_percentage):
    if diff_count > 0:
        return f"**+{diff_count}** ({diff_percentage:.2f}%)"
    elif diff_count < 0:
        return f"**-{abs(diff_count)}** ({diff_percentage:.2f}%)"
    else:
        return "**No change**"

def main():
    if DISCORD_WEBHOOK_URL is None:
        print("DISCORD_WEBHOOK_URL env variable not set")
        sys.exit(1)

    if COMMIT_MESSAGE is None:
        print("COMMIT_MESSAGE env variable not set")
        sys.exit(1)

    if not os.path.exists("./bin_comp/new_data.json"):
        print("couldn't find new_data.json")
        sys.exit(1)

    if not os.path.exists("./bin_comp/coverage_trace_funcs.txt"):
        print("couldn't find coverage_trace_funcs.txt")
        sys.exit(1)

    with open("./bin_comp/new_data.json", "rt") as file:
        new_data = json.load(file)
    coverage_data = load_coverage_file("./bin_comp/coverage_trace_funcs.txt")

    total_coverage_funcs = len(coverage_data)

    matched_coverage_funcs = 0
    total_matched_funcs = 0
    unmatched_funcs = 0
    wip_funcs = 0

    for func in new_data["functions"]:
        status = func["func_status"]
        if status == "0x1":
            total_matched_funcs = total_matched_funcs + 1
        elif status == "0x0" or status == "0x2":
            unmatched_funcs = unmatched_funcs + 1 # wip counts as stub total too, wip is a sub count of it
        if status == "0x2":
            wip_funcs = wip_funcs + 1


    for func in new_data["functions"]:
        og_address = func["og_addr"]
        status = func["func_status"]
        for coverage_addr in coverage_data:
            if status == "0x1" and coverage_addr.lower() == og_address.lower():
                matched_coverage_funcs = matched_coverage_funcs + 1
                break

    coverage_funcs_percentage = matched_coverage_funcs / total_coverage_funcs * 100

    prev_json_available = True
    previous_progress_json = {}
    try:
        with open("progress.json", "r") as file:
            previous_progress_json = json.load(file)
            # rename the old key name 'unknown_status_funcs' to 'unmatched_funcs'
            if "unknown_status_funcs" in previous_progress_json:
                previous_progress_json["unmatched_funcs"] = previous_progress_json.pop("unknown_status_funcs")

    except OSError as e:
        # we don't have a progress.json from a previous build
        prev_json_available = False

    if not prev_json_available:
        boot_to_map_str = f"Boot to map progress: [{matched_coverage_funcs}/{total_coverage_funcs}] {coverage_funcs_percentage:.2f}%"
        total_matches_str = f"Total matches: {total_matched_funcs}"
        unmatched_funcs_str = f"Unmatched funcs in repo: {unmatched_funcs} (wip funcs: {wip_funcs})"
    else:
        coverage_diff_str = calc_difference(previous_progress_json["matched_boot_to_map_funcs"], matched_coverage_funcs, as_string=True)
        total_diff_str = calc_difference(previous_progress_json["total_matches"], total_matched_funcs, as_string=True)
        unmatched_funcs_diff_str = calc_difference(previous_progress_json["unmatched_funcs"], unmatched_funcs, as_string=True)

        boot_to_map_str = f"Boot to map progress: [{matched_coverage_funcs}/{total_coverage_funcs}] {coverage_funcs_percentage:.2f}% | {coverage_diff_str}"
        total_matches_str = f"Total matches: {total_matched_funcs} | {total_diff_str}"
        unmatched_funcs_str = f"Unmatched funcs in repo: {unmatched_funcs} | {unmatched_funcs_diff_str}"

    wip_funcs_percentage = (wip_funcs / unmatched_funcs) * 100
    wip_funcs_str = f"wip funcs: {wip_funcs} | ({wip_funcs_percentage:.2f}%)"

    webhook_message = {
        "content": None,
        "embeds": [
            {
            "title": "Status",
            "description": f"{COMMIT_MESSAGE}\n{boot_to_map_str}\n{total_matches_str}\n{unmatched_funcs_str}\n{wip_funcs_str}"
            }
        ],
        "attachments": []
    }

    new_progress_json = {
        "matched_boot_to_map_funcs": matched_coverage_funcs,
        "total_matches": total_matched_funcs,
        "unmatched_funcs": unmatched_funcs
    }

    print(f"prev_json_available: {prev_json_available}")
    print(f"previous_progress_json: {previous_progress_json}")
    print(f"new_progress_json: {new_progress_json}")

    if not prev_json_available or (prev_json_available and previous_progress_json != new_progress_json):
        print("Posting update...")
        req = urllib.request.Request(DISCORD_WEBHOOK_URL, json.dumps(webhook_message).encode())
        req.add_header("Content-Type", "application/json")
        req.add_header("User-Agent", "gta2_re webhook/1.0")
        response = urllib.request.urlopen(req)
    else:
        print("Not posting update")

    print(boot_to_map_str)
    print(total_matches_str)
    print(unmatched_funcs_str)
    print(wip_funcs_str)

    with open("progress.json", "w") as file:
        json.dump(new_progress_json, file, indent=4)

if __name__ == "__main__":
    main()
