import os
import subprocess
import platform
import sys
import requests
import winreg

CURRENT_DIRECTORY = os.path.dirname(os.path.abspath(__file__))

BUILD_FOLDER_NAME = "build_vc6"

BUILD_DIRECTORY = os.path.join(CURRENT_DIRECTORY, BUILD_FOLDER_NAME)
BIN_COMP_DIRECTORY = os.path.join(CURRENT_DIRECTORY, "Scripts", "bin_comp")

CMAKE_GENERATE_JOM_CMD = "cmake.exe -DCMAKE_VERBOSE_MAKEFILE:BOOL=OFF -DCMAKE_BUILD_TYPE=Release .. -G\"NMake Makefiles JOM\""
CMAKE_BUILD_CMD = "cmake --build . --target all"

BUILD_CMDS = [CMAKE_GENERATE_JOM_CMD,
              CMAKE_BUILD_CMD]

def main():
    if not is_vc6_available():
        print("The required vc6 tools to build the project are not available!")
        print("Run vc6_setup.py first and then try again.")
        sys.exit(1)

    print("Starting build")
    print(f"Build platform: {platform.system()}")
    print(f"Wine build: {is_wine_build()}\n")

    returncode = build()
    if returncode != 0:
        print(f"build failed with return code {returncode}")
        sys.exit(1)
    
    ok = verify()
    if not ok:
        print(f"function verification failed!")
        sys.exit(1)

    print("Build finished and verified successfully!")

def is_wine_build():
    try:
        key = winreg.OpenKeyEx(winreg.HKEY_LOCAL_MACHINE, "SOFTWARE\\Wine", winreg.KEY_READ)
        winreg.CloseKey(key)
        return True
    except OSError as e:
        return False

def is_vc6_available():
    try:
        key = winreg.OpenKeyEx(winreg.HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Microsoft\\VisualStudio\\6.0\\Setup", winreg.KEY_READ)
        winreg.CloseKey(key)
        return True
    except OSError as e:
        return False


def get_build_env():
    new_env = os.environ.copy()

    vc6_root = os.path.join(CURRENT_DIRECTORY, "3rdParty", "gta2_re_compile_tools")
    lib = os.path.join(vc6_root, "VC98", "Lib")
    include = f'{os.path.join(vc6_root, "VC98", "ATL", "INCLUDE")};{os.path.join(vc6_root, "VC98", "INCLUDE")};{os.path.join(vc6_root, "VC98", "MFC", "INCLUDE")}'
    path = f'{os.path.join(vc6_root, "cmake-3.5.0-win32-x86", "bin")};{os.path.join(vc6_root, "VC98", "bin")};{os.path.join(vc6_root, "Common", "MSDev98", "Bin")};{new_env["PATH"]}'

    new_env["LIB"] = lib
    new_env["INCLUDE"] = include
    new_env["PATH"] = path

    if is_wine_build():
        new_env["WINEDEBUG"] = "-all"

    return new_env

def build():
    os.makedirs(BUILD_FOLDER_NAME, exist_ok=True)

    build_cmds = " && ".join(BUILD_CMDS)
    result = subprocess.run(build_cmds, env=get_build_env(), cwd=BUILD_DIRECTORY, shell=True)

    return result.returncode

def verify():
    download_exe()

    dump_result = subprocess.run("python msvc_dump_func_data.py", env= get_build_env(), cwd=BIN_COMP_DIRECTORY)
    compare_result = subprocess.run("python compare_all_functions.py", env=get_build_env(), cwd=BIN_COMP_DIRECTORY)

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
