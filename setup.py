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

    if os.getenv("APPVEYOR") is None and os.getenv("CI") is None:
        # set up VC6_ROOT env variable which will be used by set_vc6.bat 
        vc6_root_dir = CURRENT_DIRECTORY + "\\3rdParty" + "\\gta2_re_compile_tools"
        if os.getenv("VC6_ROOT") is None or os.getenv("VC6_ROOT") != vc6_root_dir:
            subprocess.run(f'setx VC6_ROOT "{vc6_root_dir}"', shell=True)
            print(f"set VC6_ROOT env variable to: {vc6_root_dir}")

if __name__ == "__main__":
    main()
