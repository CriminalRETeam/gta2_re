import winreg
import os

CURRENT_DIRECTORY = os.path.dirname(os.path.abspath(__file__))

def main():
    # set up the registry key that is required by cmake to detect the compiler
    vs_common_dir = CURRENT_DIRECTORY + "\\3rdParty" + "\\gta2_re_compile_tools" + "\\Common"
    key = winreg.CreateKey(winreg.HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Microsoft\\VisualStudio\\6.0\\Setup")
    winreg.SetValueEx(key, "VsCommonDir", 0, winreg.REG_SZ, vs_common_dir)
    winreg.CloseKey(key)

    print(f"Setting VsCommonDir to: {vs_common_dir}")

if __name__ == "__main__":
    main()
