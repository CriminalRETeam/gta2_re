import winreg
import os
import subprocess

CURRENT_DIRECTORY = os.path.dirname(os.path.abspath(__file__))

def main():
    # set up the registry key that is required by cmake to detect the compiler
    vs_common_dir = CURRENT_DIRECTORY + "\\3rdParty" + "\\gta2_re_compile_tools" + "\\Common"
    key = winreg.CreateKey(winreg.HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Microsoft\\VisualStudio\\6.0\\Setup")
    winreg.SetValueEx(key, "VsCommonDir", 0, winreg.REG_SZ, vs_common_dir)
    winreg.CloseKey(key)

    print(f"set vs common dir to: {vs_common_dir}")

    # set up VC6_ROOT env variable which will be used by set_vc6.bat 
    vc6_root_dir = os.getenv("VC6_ROOT")
    if vc6_root_dir is None or vc6_root_dir != CURRENT_DIRECTORY:
        subprocess.run(f'setx VC6_ROOT "{CURRENT_DIRECTORY}"', shell=True)
        print(f"set VC6_ROOT env variable to: {CURRENT_DIRECTORY}")
    
    subprocess.run("set_vc6.bat", shell=True)

if __name__ == "__main__":
    main()
