"""
Runs an exe's C and C++ static initialisers (the CRT's __xi_a..__xi_z and __xc_a..__xc_z tables) under
unicorn and gives the memory they leave, for check_global_values.py.

The image is mapped at its base, every import slot points at a stub that returns a plausible value and
pops the stdcall arguments, and HeapAlloc/VirtualAlloc hand out zeroed blocks from a fake heap at HEAP.
The x87 control word starts at 0x27F, the precision Windows gives a new thread (53-bit mantissa), so float
tables computed at startup round as they do in the game.
"""
import re
import struct

import pefile
from unicorn import Uc, UcError, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE, UC_HOOK_MEM_WRITE
from unicorn.x86_const import UC_X86_REG_EAX, UC_X86_REG_EIP, UC_X86_REG_ESP, UC_X86_REG_FPCW, UC_X86_REG_FS, \
    UC_X86_REG_GDTR, UC_X86_REG_DS, UC_X86_REG_ES, UC_X86_REG_SS

STACK, STACK_SIZE = 0x10000000, 0x100000
STUBS = 0x20000000
HEAP, HEAP_SIZE = 0x30000000, 0x8000000
TEB, GDT = 0x7FFD0000, 0x7FFE0000
RETURN = STACK + STACK_SIZE - 0x1000  # a return address that ends the emulation
FPU_CONTROL = 0x27F
MAX_INSNS = 5000000

# stdcall argument counts of the undecorated imports the initialisers can reach
NARGS = dict(
    HeapAlloc=3, HeapReAlloc=4, HeapFree=3, HeapSize=3, HeapCreate=3, HeapDestroy=1, VirtualAlloc=4, VirtualFree=3,
    GetProcessHeap=0, WriteFile=5, GetStdHandle=1, GetLastError=0, SetLastError=1, OutputDebugStringA=1,
    GetModuleHandleA=1, GetProcAddress=2, LoadLibraryA=1, FreeLibrary=1, GetTickCount=0, timeGetTime=0,
    CreateFileA=7, ReadFile=5, CloseHandle=1, SetFilePointer=4, GetFileAttributesA=1, MultiByteToWideChar=6,
    WideCharToMultiByte=8, GetACP=0, GetCPInfo=2, GetOEMCP=0, LCMapStringA=6, LCMapStringW=6, GetStringTypeA=5,
    GetStringTypeW=4, CompareStringA=6, CompareStringW=6, GetVersion=0, GetVersionExA=1, lstrlenA=1, lstrcatA=2,
    lstrcpyA=2, IsBadReadPtr=2, IsBadWritePtr=2, IsBadCodePtr=1, GetModuleFileNameA=3, GetEnvironmentVariableA=3,
    RaiseException=4, FindFirstFileA=2, FindNextFileA=2, FindClose=1, GetPrivateProfileStringA=6, GetDriveTypeA=1,
    GetCurrentDirectoryA=2, GetFullPathNameA=4, Sleep=1, CreateMutexA=3, OpenMutexA=3, ReleaseMutex=1,
    CreateEventA=4, GetComputerNameA=2, wvsprintfA=3, MessageBoxA=4, RegOpenKeyExA=5, RegQueryValueExA=6,
    RegCloseKey=1, RegCreateKeyExA=9, RegSetValueExA=6, RegOpenKeyA=3, GetSystemMetrics=1, CoInitialize=1,
    FlushFileBuffers=1, GetFileType=1, SetHandleCount=1, GetStartupInfoA=1, SetUnhandledExceptionFilter=1,
    ShowCursor=1, InitializeCriticalSection=1, EnterCriticalSection=1, LeaveCriticalSection=1,
    DeleteCriticalSection=1, TlsAlloc=0, TlsGetValue=1, TlsSetValue=2, GetCurrentThreadId=0, QueryPerformanceCounter=1,
    QueryPerformanceFrequency=1, GetCommandLineA=0, GetEnvironmentStrings=0, GetEnvironmentStringsW=0,
    FreeEnvironmentStringsA=1, FreeEnvironmentStringsW=1, wsprintfA=0, GetLocalTime=1, GetSystemTime=1,
    GetTimeZoneInformation=1, FileTimeToLocalFileTime=2, FileTimeToSystemTime=2, ExitProcess=1, DestroyWindow=1)
SYSTEM_TIME = struct.pack('<8H', 2000, 1, 6, 1, 12, 0, 0, 0)  # what the time functions report


def segment(base, limit, access, flags):
    """A GDT descriptor."""
    return (limit & 0xFFFF) | (base & 0xFFFFFF) << 16 | access << 40 | (limit >> 16 & 0xF) << 48 | flags << 52 | \
        (base >> 24 & 0xFF) << 56


class Emu:
    def __init__(self, exe):
        pe = pefile.PE(exe)
        self.base = pe.OPTIONAL_HEADER.ImageBase
        self.size = (pe.OPTIONAL_HEADER.SizeOfImage + 0xFFFF) & ~0xFFFF
        self.uc = uc = Uc(UC_ARCH_X86, UC_MODE_32)
        uc.mem_map(self.base, self.size)
        for sec in pe.sections:
            data = sec.get_data()
            uc.mem_write(self.base + sec.VirtualAddress, data[:min(len(data), sec.Misc_VirtualSize)])
        uc.mem_map(STACK, STACK_SIZE)
        uc.mem_map(STUBS, 0x10000)
        uc.mem_map(HEAP, HEAP_SIZE)
        # fs:[0] is the SEH chain that functions with an EH frame link into: a TEB with an empty chain
        uc.mem_map(TEB, 0x1000)
        uc.mem_map(GDT, 0x1000)
        uc.mem_write(TEB, struct.pack('<7I', 0xFFFFFFFF, STACK + STACK_SIZE, STACK, 0, 0, 0, TEB))
        uc.mem_write(GDT + 8, struct.pack('<3Q', segment(0, 0xFFFFF, 0x9A, 0xC), segment(0, 0xFFFFF, 0x92, 0xC),
                                          segment(TEB, 0xFFF, 0x92, 0x4)))
        uc.reg_write(UC_X86_REG_GDTR, (0, GDT, 0x1F, 0))
        for reg in (UC_X86_REG_DS, UC_X86_REG_ES, UC_X86_REG_SS):
            uc.reg_write(reg, 0x10)
        uc.reg_write(UC_X86_REG_FS, 0x18)
        self.heap = HEAP
        self.blocks = {}
        self.stubs = {}
        self.unknown = {}
        self.exited = False
        n = 0
        for dll in getattr(pe, 'DIRECTORY_ENTRY_IMPORT', []):
            for imp in dll.imports:
                stub = STUBS + 16 * n
                n += 1
                uc.mem_write(imp.address, struct.pack('<I', stub))
                uc.mem_write(stub, b'\xc3')
                self.stubs[stub] = imp.name.decode() if imp.name else '#%d' % imp.ordinal
        uc.hook_add(UC_HOOK_CODE, self._import, begin=STUBS, end=STUBS + 0xFFFF)
        self.write_starts = {}  # the lowest image address each initialiser writes (an object start): initialiser
        self.lowest = None
        uc.hook_add(UC_HOOK_MEM_WRITE, self._write, begin=self.base, end=self.base + self.size - 1)

    def _write(self, uc, access, address, size, value, user):
        if self.lowest is None or address < self.lowest:
            self.lowest = address

    def alloc(self, n):
        n = max(n, 1)
        a = self.heap
        self.heap += (n + 15) & ~15
        self.blocks[a] = n
        return a

    def _import(self, uc, address, size, user):
        name = self.stubs.get(address)
        if name is None:
            return
        esp = uc.reg_read(UC_X86_REG_ESP)
        ret = struct.unpack('<I', uc.mem_read(esp, 4))[0]
        args = struct.unpack('<8I', uc.mem_read(esp + 4, 32))
        m = re.search(r'@(\d+)$', name)
        nargs = int(m.group(1)) // 4 if m else NARGS.get(re.sub(r'^_', '', name))
        if nargs is None:
            self.unknown[name] = self.unknown.get(name, 0) + 1
            nargs = 0
        r = 0
        if name == 'HeapAlloc':
            r = self.alloc(args[2])
        elif name == 'HeapReAlloc':
            r = self.alloc(args[3])
            if args[2] in self.blocks:
                uc.mem_write(r, bytes(uc.mem_read(args[2], min(self.blocks[args[2]], args[3]))))
        elif name == 'HeapSize':
            r = self.blocks.get(args[2], 0)
        elif name == 'VirtualAlloc':
            r = self.alloc(args[1]) if args[0] == 0 else args[0]
        elif name in ('HeapCreate', 'GetProcessHeap', 'GetModuleHandleA', 'HeapFree', 'VirtualFree', 'CloseHandle',
                      'FindClose', 'FreeLibrary'):
            r = 1
        elif name == 'WriteFile':
            if args[3]:
                uc.mem_write(args[3], struct.pack('<I', args[2]))
            r = 1
        elif name == 'GetStartupInfoA':
            uc.mem_write(args[0], struct.pack('<I', 68) + b'\0' * 64)
        elif name == 'GetFileType':
            r = 2  # FILE_TYPE_CHAR
        elif name == 'GetStdHandle':
            r = 7
        elif name == 'GetVersion':
            r = 0x0A280105
        elif name == 'GetACP':
            r = 1252
        elif name == 'CreateFileA':
            r = 0xFFFFFFFF
        elif name == 'FindFirstFileA':
            uc.mem_write(args[1], b'\0' * 0x140)  # the attract replays InputRecorder_67F8B0 checks for exist
            r = 0x100
        elif name in ('GetLocalTime', 'GetSystemTime'):
            uc.mem_write(args[0], SYSTEM_TIME)
        elif name in ('FileTimeToLocalFileTime', 'FileTimeToSystemTime'):
            uc.mem_write(args[1], SYSTEM_TIME if name == 'FileTimeToSystemTime' else b'\0' * 8)
            r = 1
        elif name == 'GetTimeZoneInformation':
            uc.mem_write(args[0], b'\0' * 172)
        elif name == 'ExitProcess':
            self.exited = True
            uc.emu_stop()
        elif name in ('timeGetTime', 'GetTickCount'):
            r = 1000
        uc.reg_write(UC_X86_REG_EAX, r)
        uc.reg_write(UC_X86_REG_ESP, esp + 4 + 4 * nargs)
        uc.reg_write(UC_X86_REG_EIP, ret)

    def call(self, address):
        uc = self.uc
        esp = RETURN - 0x100
        uc.mem_write(esp, struct.pack('<I', RETURN))
        uc.reg_write(UC_X86_REG_ESP, esp)
        uc.reg_write(UC_X86_REG_FPCW, FPU_CONTROL)
        self.exited = False
        uc.emu_start(address, RETURN, count=MAX_INSNS)
        return uc.reg_read(UC_X86_REG_EIP) == RETURN and not self.exited

    def run_table(self, start, end):
        """Calls each non-null pointer in [start, end); returns [(slot, function, error)] for those that fail."""
        fails = []
        for slot in range(start, end, 4):
            f = struct.unpack('<I', self.uc.mem_read(slot, 4))[0]
            if not f:
                continue
            self.lowest = None
            try:
                if not self.call(f):
                    fails.append((slot, f, 'called ExitProcess' if self.exited else 'did not return'))
            except UcError as e:
                fails.append((slot, f, '%s at %x' % (e, self.uc.reg_read(UC_X86_REG_EIP))))
            if self.lowest is not None:
                self.write_starts.setdefault(self.lowest, f)
        return fails

    def read(self, va, n):
        return bytes(self.uc.mem_read(va, n))


def run_static_init(exe, xc_a, xc_z, xi_a, xi_z, ioinit):
    """Emulates the CRT startup up to WinMain as far as the globals go: _ioinit (the C initialisers' stdio
    setup reads its handle table), the C initialisers, then the C++ ones. Returns the emulator and the
    initialisers that failed."""
    emu = Emu(exe)
    fails = []
    try:
        if not emu.call(ioinit):
            fails.append((0, ioinit, 'did not return'))
    except UcError as e:
        fails.append((0, ioinit, str(e)))
    fails += emu.run_table(xi_a + 4, xi_z) + emu.run_table(xc_a + 4, xc_z)
    return emu, fails
