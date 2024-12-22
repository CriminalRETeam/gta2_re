import os
import sys
import json
import urllib.request

WEBHOOK_URL = sys.argv[1]

if WEBHOOK_URL is None:
    print("the first arg needs to be the discord webhook url")
    sys.exit(1)

def load_csv_file(filename):
    ret = []
    with open(filename) as file:
        lines = [line.rstrip() for line in file]
        for line in lines:
            ret.append(line.split(","))
    return ret

def load_coverage_file(filename):
    with open(filename) as file:
        lines = [line.rstrip() for line in file]
        return lines

if not os.path.exists("./bin_comp/new_function_data.csv"):
    print("couldn't find new_function_data.csv")
    sys.exit(1)

if not os.path.exists("./bin_comp/coverage_trace_funcs.txt"):
    print("couldn't find coverage_trace_funcs.txt")
    sys.exit(1)

new_func_data = load_csv_file("./bin_comp/new_function_data.csv")
coverage_data = load_coverage_file("./bin_comp/coverage_trace_funcs.txt")

total_coverage_funcs = len(coverage_data)

matched_coverage_funcs = 0
total_matched_funcs = 0
unknown_status_funcs = 0

for func in new_func_data:
    status = func[4]
    if status == "0x1":
        total_matched_funcs = total_matched_funcs + 1
    elif status == "0x0":
        unknown_status_funcs = unknown_status_funcs + 1

for func in new_func_data:
    og_address = func[3]
    status = func[4]
    for coverage_addr in coverage_data:
        if status == "0x1" and coverage_addr.lower() == og_address.lower():
            matched_coverage_funcs = matched_coverage_funcs + 1
            break

coverage_funcs_percentage = matched_coverage_funcs / total_coverage_funcs * 100

boot_to_map_str = f"Boot to map progress: [{matched_coverage_funcs}/{total_coverage_funcs}] {coverage_funcs_percentage:.2f}%"
total_matches_str = f"Total matches: {total_matched_funcs}"
unkown_status_str = f"Unknown status funcs: {unknown_status_funcs}"

print(boot_to_map_str)
print(total_matches_str)
print(unkown_status_str)

webhook_message = {
  "content": None,
  "embeds": [
    {
      "title": "Status",
      "description": f"{boot_to_map_str}\n{total_matches_str}\n{unkown_status_str}"
    }
  ],
  "attachments": []
}

req = urllib.request.Request(WEBHOOK_URL, json.dumps(webhook_message).encode())
req.add_header("Content-Type", "application/json")
req.add_header("User-Agent", "gta2_rev webhook/1.0")
response = urllib.request.urlopen(req)
