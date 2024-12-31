import os
import subprocess
import platform
import sys
import requests

CURRENT_DIRECTORY = os.path.dirname(os.path.abspath(__file__))

BUILD_FOLDER_NAME = "build_vc6"

BUILD_DIRECTORY = os.path.join(CURRENT_DIRECTORY, BUILD_FOLDER_NAME)
BIN_COMP_DIRECTORY = os.path.join(CURRENT_DIRECTORY, "Scripts", "bin_comp")

CMAKE_GENERATE_JOM_CMD = "cmake -DCMAKE_VERBOSE_MAKEFILE:BOOL=OFF -DCMAKE_BUILD_TYPE=Release .. -G\"NMake Makefiles JOM\""
CMAKE_BUILD_CMD = "cmake --build . --target all"

BUILD_CMDS = [CMAKE_GENERATE_JOM_CMD,
              CMAKE_BUILD_CMD]

def main():
    print("Starting build")
    print(f"Build platform: {platform.system()}")

    returncode = build()
    if returncode != 0:
        print(f"build failed with return code {returncode}")
        sys.exit(1)
    
    ok = verify()
    if not ok:
        print(f"function verification failed!")
        sys.exit(1)

    print("Build finished and verified successfully!")

def get_vc6_env_cmds():
    vc6_root = os.path.join(CURRENT_DIRECTORY, "3rdParty", "gta2_re_compile_tools")
    lib = os.path.join(vc6_root, "VC98", "Lib")
    include = f'{os.path.join(vc6_root, "VC98", "ATL", "Include")};{os.path.join(vc6_root, "VC98", "Include")};{os.path.join(vc6_root, "VC98", "MFC", "Include")}'
    path = f'{os.path.join(vc6_root, "cmake-3.5.0-win32-x86", "bin")};{os.path.join(vc6_root, "VC98", "bin")};{os.path.join(vc6_root, "Common", "MSDev98", "Bin")}'

    vc6_cmds = list()
    vc6_cmds.append(f"set LIB={lib}")
    vc6_cmds.append(f"set INCLUDE={include}")
    vc6_cmds.append(f"set PATH={path};%PATH%")

    return vc6_cmds

def build():
    os.makedirs(BUILD_FOLDER_NAME, exist_ok=True)

    all_cmds = get_vc6_env_cmds()
    all_cmds.extend(BUILD_CMDS)
    build_cmds = "\n".join(all_cmds) + "\n"

    if platform.system() == "Linux":
        p1 = subprocess.Popen("wine cmd", cwd=BUILD_DIRECTORY, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    elif platform.system() == "Windows":
        p1 = subprocess.Popen("cmd", cwd=BUILD_DIRECTORY, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)

    p1.stdin.write(build_cmds)
    p1.stdin.close()
    
    while True:
        output = p1.stdout.readline()
        if output == "" and p1.poll() is not None:
            break

        if output:
            print(output.strip())

    return p1.poll()

def verify():
    download_exe()

    if platform.system() == "Windows":
        python = "python"
    elif platform.system() == "Linux":
        python = "python3"

    dump_result = subprocess.run(f"{python} msvc_dump_func_data.py", cwd=BIN_COMP_DIRECTORY)
    compare_result = subprocess.run(f"{python} compare_all_functions.py", cwd=BIN_COMP_DIRECTORY)

    return dump_result.returncode == 0 and compare_result.returncode == 0

def download_exe():
    exe_path = os.path.join(BIN_COMP_DIRECTORY, "10.5.exe")
    if os.path.exists(exe_path):
        print("10.5.exe already exists, skipping")
        return

    r = requests.get("https://mouzedrift.s-ul.eu/KTb8N0bc")

    if r.status_code != 200:
        print(f"Failed to download 10.5.exe error code: {r.status_code}")
        sys.exit(1)

    with open(exe_path, "wb") as exe:
        exe.write(r.content)

    print(f"Successfully downloaded 10.5.exe to: {exe_path}")

if __name__ == "__main__":
    main()
