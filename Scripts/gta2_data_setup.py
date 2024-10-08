import winreg
import platform
import tkinter
from tkinter import filedialog

def is_64_bit():
    return platform.machine() == "AMD64" or platform.machine() == "x86_64"

def main():
    print(f"architecture: {platform.machine()}")

    if is_64_bit():
        freeloader_key = winreg.CreateKey(winreg.HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\freeloader.com")
        gta2_key = winreg.CreateKey(winreg.HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\freeloader.com\\GTA2")
    else:
        freeloader_key = winreg.CreateKey(winreg.HKEY_LOCAL_MACHINE, "SOFTWARE\\freeloader.com")
        gta2_key = winreg.CreateKey(winreg.HKEY_LOCAL_MACHINE, "SOFTWARE\\freeloader.com\\GTA2")

    winreg.SetValueEx(freeloader_key, "WebRoot", 0, winreg.REG_SZ, "http://www.freeloader.com/?page_id=%d")
    winreg.CloseKey(freeloader_key)

    root = tkinter.Tk()
    root.withdraw()

    install_path = tkinter.filedialog.askdirectory()
    if install_path == "":
        print("no gta2 install path selected, aborting")
        return
    
    print(f"setting gta2 install path to: {install_path}")
    winreg.SetValueEx(gta2_key, "InstallPath", 0, winreg.REG_SZ, install_path)
    winreg.SetValueEx(gta2_key, "firstname", 0, winreg.REG_SZ, "")
    winreg.SetValueEx(gta2_key, "lastname", 0, winreg.REG_SZ, "")
    winreg.SetValueEx(gta2_key, "userid", 0, winreg.REG_DWORD, 0)
    winreg.SetValueEx(gta2_key, "page_id", 0, winreg.REG_DWORD, 0)
    winreg.SetValueEx(gta2_key, "lang", 0, winreg.REG_DWORD, 0)
    winreg.SetValueEx(gta2_key, "F_L_T", 0, winreg.REG_DWORD, 0)
    winreg.SetValueEx(gta2_key, "CityInstalled", 0, winreg.REG_DWORD, 2)
    winreg.CloseKey(gta2_key)

if __name__ == "__main__":
    main()
