import requests
import sys

r = requests.get("https://mouzedrift.s-ul.eu/KTb8N0bc")

if r.status_code != 200:
    print(f"Failed to download 10.5.exe error code: {r.status_code}")
    sys.exit(1)

with open("10.5.exe", "wb") as exe:
    exe.write(r.content)

print("Successfully downloaded 10.5.exe")
