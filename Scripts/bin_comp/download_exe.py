import requests
import sys
import os

CURRENT_DIRECTORY = os.path.dirname(os.path.abspath(__file__))

if os.path.exists(os.path.join(CURRENT_DIRECTORY, "10.5.exe")):
    print("10.5.exe already exists, skipping")
    sys.exit(0)

r = requests.get("https://mouzedrift.s-ul.eu/KTb8N0bc")

if r.status_code != 200:
    print(f"Failed to download 10.5.exe error code: {r.status_code}")
    sys.exit(1)

with open("10.5.exe", "wb") as exe:
    exe.write(r.content)

print("Successfully downloaded 10.5.exe")
