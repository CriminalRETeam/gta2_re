import os
import subprocess
import platform
import sys
import requests
import shutil
import argparse
import enum
import json
import re

CURRENT_DIRECTORY = os.path.dirname(os.path.abspath(__file__))

BUILD_FOLDER_NAME = "build_vc6"

BUILD_DIRECTORY = os.path.join(CURRENT_DIRECTORY, BUILD_FOLDER_NAME)
BIN_COMP_DIRECTORY = os.path.join(CURRENT_DIRECTORY, "Scripts", "bin_comp")

CMAKE_GENERATE_JOM_CMD = f"cmake -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON -DCMAKE_BUILD_TYPE=Release {args.extra_cmake_flags} .. -G\"NMake Makefiles JOM\""
CMAKE_BUILD_CMD = "cmake --build . --target all" #  -- -j 1

BUILD_CMDS = [CMAKE_GENERATE_JOM_CMD,
              CMAKE_BUILD_CMD]

GTA2_ROOT = os.environ.get("GTA2_ROOT")

class ExeType(enum.IntEnum):
    standalone = 0,
    patched = 1

class CompileErrorEntry:
    def __init__(self, file_path: str, line_number: int, log_type: str, error_code: str, message: str):
        self.file_path = file_path
        self.line_number = line_number
        self.log_type = log_type
        self.error_code = error_code
        self.message = message

    def __repr__(self):
        return f"{self.file_path}({self.line_number}) : {self.log_type} {self.error_code}: {self.message}"

class CompileErrorEntryEncoder(json.JSONEncoder):
        def default(self, o):
            return o.__dict__

class CompileErrorCollection:
    def __init__(self):
        self.entries = list()

    def add(self, entry: CompileErrorEntry):
        self.entries.append(entry)

    def clear(self):
        self.entries.clear()

    def items(self):
        return self.entries

    def warnings(self):
        warnings = list()
        for entry in self.entries:
            if entry.log_type == "warning":
                warnings.append(entry)
        return warnings
    
    def errors(self):
        errors = list()
        for entry in self.entries:
            if entry.log_type == "error":
                errors.append(entry)
        return errors
    
    def save_json(self):
        if GTA2_ROOT is None:
            return

        with open(os.path.join(GTA2_ROOT, "build.json"), "w") as file:
            if len(self.entries) <= 0:
                json.dump([], fp=file)
            else:
                json.dump(self.entries, fp=file, indent=4, cls=CompileErrorEntryEncoder)


def main():
     parser = argparse.ArgumentParser()
    parser.add_argument('--extra-cmake-flags', default="", help="Additional flags to pass to CMake")
    parser.add_argument("--run_standalone", help="Run the standalone gta2 exe after the build successfully finishes", action="store_true")
    parser.add_argument("--run_patched", help="Run the patched gta2 exe after the build successfully finishes", action="store_true")
    parser.add_argument("--ignore_no_match", help="Ignore any errors related to non matching functions", action="store_true")

    args = parser.parse_args()

    print("Starting build")
    print(f"Build platform: {platform.system()}")

    returncode = build()
    if returncode != 0:
        print(f"Build failed with return code {returncode}")
        sys.exit(returncode)
    
    ok = verify()
    if not ok and not args.ignore_no_match:
        print(f"Function verification failed!")
        sys.exit(1)

    print("Build finished and verified successfully!")

    if GTA2_ROOT is None:
        if os.environ.get("CI") is None:
            print("Warning: GTA2_ROOT environment variable is not set. Some optional QoF features will not be available.")
        sys.exit(0)

    copy_files()

    if args.run_standalone:
        run_exe(ExeType.standalone)
    elif args.run_patched:
        run_exe(ExeType.patched)

def as_wine_path(unix_path):
    wine_path = unix_path.replace("/", "\\")
    wine_path = unix_path.replace("\\", "\\\\")
    wine_path = "Z:" + wine_path

    return wine_path

def get_vc6_env():
    vc6_root = os.path.join(CURRENT_DIRECTORY, "3rdParty", "gta2_re_compile_tools")

    lib = os.path.join(vc6_root, "VC98", "Lib")
    include1 = os.path.join(vc6_root, "VC98", "ATL", "Include")
    include2 = os.path.join(vc6_root, "VC98", "Include")
    include3 = os.path.join(vc6_root, "VC98", "MFC", "Include")

    path1 = os.path.join(vc6_root, "cmake-3.5.0-win32-x86", "bin")
    path2 = os.path.join(vc6_root, "VC98", "bin")
    path3 = os.path.join(vc6_root, "Common", "MSDev98", "Bin")

    if platform.system() == "Windows":
        path = f'{path1};{path2};{path3}'
        include = f'{include1};{include2};{include3};'
    elif platform.system() == "Linux" or platform.system() == "Darwin":
        lib = as_wine_path(lib)
        include1 = as_wine_path(include1)
        include2 = as_wine_path(include2)
        include3 = as_wine_path(include3)

        # escape ";"
        path = fr'{path1}\;{path2}\;{path3}'
        include = fr'{include1}\;{include2}\;{include3};'


    vc6_cmds = list()
    vc6_cmds.append(lib)
    vc6_cmds.append(include)
    vc6_cmds.append(path)

    return vc6_cmds

def convert_path(strPath):
    if os.name == "posix":
        if strPath.startswith("Z:\\"):
            strPath = strPath[2:]
            strPath = strPath.replace("\\", "/")
            return strPath
    return strPath

def build():
    os.makedirs(BUILD_FOLDER_NAME, exist_ok=True)

    vc6_env = get_vc6_env()
    lib = vc6_env[0]
    include = vc6_env[1]
    path = vc6_env[2]

    if platform.system() in ("Linux", "Darwin"):
        build_dir = as_wine_path(BUILD_DIRECTORY)
        command = (
            f"WINEDEBUG=-all "
            f"export WINEPATH={path} "
            f"export LIB={lib} "
            f"export INCLUDE={include} "
            f"wine cmd /c \"cd {build_dir} && {CMAKE_GENERATE_JOM_CMD} && {CMAKE_BUILD_CMD}\""
        )
        p1 = subprocess.Popen(
            command,
            cwd=BUILD_DIRECTORY,
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,  # ✅ Merge stderr into stdout
            text=True,
            shell=True
        )
    elif platform.system() == "Windows":
        p1 = subprocess.Popen(
            "cmd",
            cwd=BUILD_DIRECTORY,
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,  # ✅ Merge stderr into stdout
            text=True
        )

        p1.stdin.write(f"set LIB={lib}\n")
        p1.stdin.write(f"set INCLUDE={include}\n")
        p1.stdin.write(f"set PATH={path};%PATH%\n")

        for build_cmd in BUILD_CMDS:
            p1.stdin.write(f"{build_cmd}\n")

        p1.stdin.write("exit /b %errorlevel%\n")
        p1.stdin.close()

    # Regex for MSVC-style error/warning messages
    pattern = re.compile(
        r'(?P<file_path>[A-Z]:\\(?:[^\\/:*?"<>|\r\n]+\\)*[^\\/:*?"<>|\r\n]+)'
        r'\((?P<line_number>\d+)\)\s*:\s*(?:fatal\s+)?(?P<log_type>error|warning)\s+'
        r'(?P<code>[A-Z]\d+):\s*(?P<message>.+)'
    )

    error_collection = CompileErrorCollection()

    while True:
        output = p1.stdout.readline()
        if output == "" and p1.poll() is not None:
            break

        if output:
            print(output.strip())
            sys.stdout.flush()

            match = pattern.match(output)
            if match:
                strPath = convert_path(match.group("file_path"))
                entry = CompileErrorEntry(
                    strPath,
                    int(match.group("line_number")),
                    match.group("log_type"),
                    match.group("code"),
                    match.group("message")
                )
                error_collection.add(entry)

    error_collection.save_json()

    return p1.poll()


def verify():
    download_exe("10.5.exe")
    download_exe("9.6f.exe")

    python = sys.executable # should be the python venv

    dump_result = subprocess.run(f"{python} msvc_dump_new_data.py", cwd=BIN_COMP_DIRECTORY, shell=True)
    compare_result = subprocess.run(f"{python} compare_all_functions.py", cwd=BIN_COMP_DIRECTORY, shell=True, capture_output=True, text=True)

    error_collection = CompileErrorCollection()
    for line in compare_result.stdout.splitlines():
        print(line)
        if line.endswith(" FAIL!"):
            entry = CompileErrorEntry("", 0, "function_verification", "", line[:-6])
            error_collection.add(entry)

    error_collection.save_json()

    return dump_result.returncode == 0 and compare_result.returncode == 0

def download_exe(exe: str):
    if exe == "10.5.exe":
        url = "https://mouzedrift.s-ul.eu/KTb8N0bc"
    elif exe == "9.6f.exe":
        url = "https://mouzedrift.s-ul.eu/66JqLJv3"
    else:
        print(f"Tried downloading unknown exe: {exe}")
        return
    
    exe_path = os.path.join(BIN_COMP_DIRECTORY, exe)
    if os.path.exists(exe_path):
        print(f"{exe} already exists, skipping")
        return

    r = requests.get(url)

    if r.status_code != 200:
        print(f"Failed to download {exe} error code: {r.status_code}")
        sys.exit(1)

    with open(exe_path, "wb") as file:
        file.write(r.content)

    print(f"Successfully downloaded {exe} to: {exe_path}")

def copy_files():
    files = ["../Scripts/bin_comp/new_data.json", "gta2_dll_exports.dll", "gta2_dll_imports.dll", "HookLoader.dll", "decomp_main.exe", "3rdParty/GTA2Hax/dear_imgui.dll", "3rdParty/GTA2Hax/d3ddll.dll", "3rdParty/GTA2Hax/DmaVideo.dll"]
    for file in files:
        file_src = os.path.join(BUILD_DIRECTORY, file)
        file_dst = os.path.join(GTA2_ROOT, os.path.basename(file))
        if not os.path.exists(file_src):
            print(f"Could not find/copy {file_src}")
        
        print(f"Copy {file_src} to {file_dst}")
        shutil.copy2(file_src, file_dst)

def run_exe(exe: ExeType):
    exe_name = "10.5.new.exe" if exe == ExeType.patched else "decomp_main.exe"
    exe_path = os.path.join(GTA2_ROOT, exe_name)
    
    if not os.path.exists(exe_path):
        print(f"Could not find exe: {exe_path}")
        return

    arg_list = []
    if platform.system() == "Windows":
        arg_list.append(exe_path)
    else:
        arg_list.append("wine")
        arg_list.append(exe_path)

    print(f"Executing run command: {arg_list}")
    subprocess.run(args=arg_list, cwd=GTA2_ROOT)

if __name__ == "__main__":
    main()
