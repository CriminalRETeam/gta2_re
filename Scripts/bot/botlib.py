"""Shared helpers for the bot drivers: the GTA2 run directory, output.map symbols, running the
game under wine (docs/BOT_DESIGN.md, "Integration")."""
import os
import re
import shutil
import subprocess
import sys
import time
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[2]
BUILD_DIR = REPO_ROOT / "build_vc6"
MAP_FILE = BUILD_DIR / "output.map"
EXE_NAME = "decomp_main.exe"


def gta2_root(arg=None):
    root = arg or os.environ.get("GTA2_ROOT")
    if not root:
        sys.exit("GTA2_ROOT is not set (the GTA2 install dir with data\\ and test\\); pass --root")
    root = Path(root)
    if not (root / "data").is_dir():
        sys.exit(f"{root} has no data/ directory")
    (root / "test").mkdir(exist_ok=True)
    return root


def out_paths(root, out):
    """An out dir given as 'bot_out/run' or 'bot_out\\run': (Linux path, the game's argument)."""
    out = str(out).replace("\\", "/").strip("/")
    linux = root / out
    linux.mkdir(parents=True, exist_ok=True)
    return linux, out.replace("/", "\\")


def install_exe(root):
    """Copies a newer build_vc6/decomp_main.exe (and the GTA2Hax dlls) into the run dir."""
    src = BUILD_DIR / EXE_NAME
    if not src.exists():
        sys.exit(f"{src} not built; run python3 build.py first")
    dst = root / EXE_NAME
    if not dst.exists() or src.stat().st_mtime > dst.stat().st_mtime:
        shutil.copy2(src, dst)
    for dll in ("dear_imgui.dll", "d3ddll.dll", "DmaVideo.dll"):
        s = BUILD_DIR / "3rdParty" / "GTA2Hax" / dll
        d = root / dll
        if s.exists() and (not d.exists() or s.stat().st_mtime > d.stat().st_mtime):
            shutil.copy2(s, d)


class Symbol:
    __slots__ = ("va", "decorated", "name", "size", "obj")

    def __init__(self, va, decorated, obj):
        self.va = va
        self.decorated = decorated
        self.name = undecorate(decorated)
        self.size = 0
        self.obj = obj

    def __repr__(self):
        return f"{self.name}@{self.va:08x}+{self.size:x}"


def undecorate(sym):
    """'?gVidSys_7071D0@@3PAUSVideo@@A' -> 'gVidSys_7071D0', '__crtheap' -> '_crtheap',
    '?field@Class@@...' -> 'Class::field'."""
    if sym.startswith("?"):
        parts = sym[1:].split("@@", 1)[0].split("@")
        parts = [p for p in parts if p]
        return "::".join(reversed(parts))
    if sym.startswith("_"):
        return sym[1:].split("@", 1)[0]
    return sym


class MapFile:
    """The VC6 linker map: publics sorted by address, in section 0003 (.data) and others."""

    LINE = re.compile(r"^\s*(\d{4}):([0-9a-f]{8})\s+(\S+)\s+([0-9a-f]{8})(?:\s+f)?(?:\s+i)?\s*(\S*)")

    def __init__(self, path=MAP_FILE):
        self.path = Path(path)
        self.symbols = []
        self.image_base = 0x400000
        text = self.path.read_text(errors="replace").splitlines()
        in_publics = False
        for line in text:
            m = re.match(r"\s*Preferred load address is ([0-9a-f]+)", line)
            if m:
                self.image_base = int(m.group(1), 16)
                continue
            if line.strip().startswith("Address") and "Publics by Value" in line:
                in_publics = True
                continue
            if line.strip().startswith("Static symbols"):
                in_publics = True  # statics follow the same format
                continue
            if not in_publics:
                continue
            m = self.LINE.match(line)
            if not m:
                continue
            section, offset, decorated, va, obj = m.groups()
            self.symbols.append(Symbol(int(va, 16), decorated, obj))
        self.symbols.sort(key=lambda s: s.va)
        for a, b in zip(self.symbols, self.symbols[1:]):
            a.size = b.va - a.va
        self.vas = [s.va for s in self.symbols]

    def lookup(self, addr):
        """Returns (symbol, offset) for the symbol at or before addr, or (None, 0)."""
        import bisect

        i = bisect.bisect_right(self.vas, addr) - 1
        if i < 0:
            return None, 0
        s = self.symbols[i]
        return s, addr - s.va

    def describe(self, addr):
        s, off = self.lookup(addr)
        if not s:
            return f"{addr:08x}"
        return f"{s.name}+{off:x}" if off else s.name

    def find(self, name):
        """Symbols named name; a trailing '*' matches a prefix."""
        if name.endswith("*"):
            return [s for s in self.symbols if s.name.startswith(name[:-1])]
        return [s for s in self.symbols if s.name == name]


# Memory that must survive a Snapshot restore, by symbol: (name, reason). The CRT allocator's
# bookkeeping is excluded because the heap's own metadata is not snapshotted (only the live
# blocks are), so restoring the pointers into it would make the two disagree.
DEFAULT_EXCLUSIONS = [
    ("_crtheap", "CRT heap handle; heap metadata is not snapshotted"),
    ("__sbh_pHeaderList", "CRT small block heap bookkeeping"),
    ("__sbh_pHeaderScan", "CRT small block heap bookkeeping"),
    ("__sbh_pHeaderDefer", "CRT small block heap bookkeeping"),
    ("__sbh_indGroupDefer", "CRT small block heap bookkeeping"),
    ("__sbh_cntHeaderList", "CRT small block heap bookkeeping"),
    ("__sbh_sizeHeaderList", "CRT small block heap bookkeeping"),
    ("__sbh_threshold", "CRT small block heap bookkeeping"),
    ("_amblksiz", "CRT heap growth"),
    ("_iob", "stdio FILE table (error.log): file positions are not rewound"),
    # OS handles and DLL state: valid only in the process that created them, so a .snap loaded
    # into another process must not overwrite them (a stale gHwnd made DestroyWindow fail and
    # the exit hang)
    ("gHwnd_707F04", "window handle"),
    ("gHInstance_708220", "module handle"),
    ("gMutex_707078", "mutex handle"),
    ("gpDInput_67B804", "DirectInput object"),
    ("gKeyboardDevice_67B5C0", "DirectInput device"),
    ("gGamePadDevice_67B6C0", "DirectInput device"),
    ("gMouseDevice_706C60", "DirectInput device"),
    ("gVidSys_7071D0", "video system object owned by d3ddll"),
    ("gVidFuncs_708600", "d3ddll function pointers"),
    ("hDmaVideoDll_7085E8", "d3ddll module handle"),
    ("gGbhDllHandle_7085D8", "gbh dll module handle"),
    ("pgbh_*", "gbh graphics dll function pointers"),
]


def write_exclusions(mapfile, out_dir, extra=()):
    """Writes out_dir/bot_exclude.txt ('rva size name') for MemoryMap::LoadExclusionFile."""
    lines = ["# rva size name: memory a Snapshot restore leaves alone (Scripts/bot/botlib.py)"]
    missing = []
    for name, reason in list(DEFAULT_EXCLUSIONS) + list(extra):
        syms = mapfile.find(name)
        if not syms:
            missing.append(name)
            continue
        for s in syms:
            size = min(s.size, 0x10000) if s.size > 0 else 4
            lines.append(f"{s.va - mapfile.image_base:08x} {size} {s.name}  # {reason}")
    out_dir.mkdir(parents=True, exist_ok=True)
    (out_dir / "bot_exclude.txt").write_text("\n".join(lines) + "\n")
    return missing


# The VC6 CRT mallocs its copies of the command line and the environment at start up, before
# the bot's allocator hooks, so their sizes decide where every later heap block lands. A
# snapshot only loads into a process with the same heap layout, hence a fixed command line
# length and a fixed environment for every run.
CMDLINE_LENGTH = 1024
ENV_KEYS = ("HOME", "PATH", "USER", "LOGNAME", "WINEPREFIX", "WINEARCH", "WINEDEBUG", "WINEDLLOVERRIDES", "LANG", "LC_ALL", "TMPDIR")


XVFB_DISPLAY = ":99"  # fixed: the display name is part of the environment, whose size shifts the heap layout


def screen_unlocked():
    """True when the user's own X display can be drawn on: DISPLAY is set and answers, and the login
    session is not locked (a locked session blocks window creation, which hangs the game)."""
    if not os.environ.get("DISPLAY"):
        return False
    try:
        sid = os.environ.get("XDG_SESSION_ID", "")
        cmd = ["loginctl", "show-session"] + ([sid] if sid else []) + ["-p", "LockedHint", "--value"]
        out = subprocess.run(cmd, capture_output=True, text=True, timeout=5).stdout.strip()
        if out == "yes":
            return False
        subprocess.run(["xset", "q"], capture_output=True, timeout=5, check=True)
    except (OSError, subprocess.SubprocessError):
        return False
    return True


def find_xvfb():
    for cand in (os.environ.get("BOT_XVFB"), shutil.which("Xvfb"), str(Path.home() / ".local/bin/Xvfb")):
        if cand and Path(cand).exists():
            return cand
    return None


class VirtualDisplay:
    """Runs the game on a private Xvfb (the session's display may be locked and then blocks window
    creation). BOT_USE_DISPLAY=1 uses $DISPLAY instead."""

    def __init__(self):
        self.proc = None
        self.display = None

    def __enter__(self):
        if os.environ.get("BOT_USE_DISPLAY"):
            self.display = os.environ.get("DISPLAY")
            return self
        xvfb = find_xvfb()
        if not xvfb:
            sys.exit("no Xvfb found: install xvfb, put the binary in ~/.local/bin or set BOT_XVFB (or BOT_USE_DISPLAY=1)")
        if not Path("/tmp/.X11-unix/X" + XVFB_DISPLAY[1:]).exists():
            self.proc = subprocess.Popen([xvfb, XVFB_DISPLAY, "-screen", "0", "800x600x24", "-nolisten", "tcp"], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
            for _ in range(50):
                if Path("/tmp/.X11-unix/X" + XVFB_DISPLAY[1:]).exists():
                    break
                time.sleep(0.1)
        self.display = XVFB_DISPLAY
        return self

    def __exit__(self, *exc):
        if self.proc:
            self.proc.terminate()
            self.proc.wait()



def pad_args(bot_args):
    args = list(bot_args)
    used = sum(len(a) + 1 for a in args)
    pad = CMDLINE_LENGTH - used - len("-bot_pad=") - 1
    if pad < 0:
        print(f"warning: command line longer than {CMDLINE_LENGTH}, snapshots of this run will not load elsewhere")
    else:
        args.append("-bot_pad=" + "x" * pad)
    return args


def run_game(root, bot_args, timeout=600, desktop=None, log=None):
    """Runs decomp_main.exe under wine from the GTA2 dir. bot_args is a list like
    ['-bot=walk_test', '-bot_out=bot_out']. Returns the exit code."""
    env = {k: os.environ[k] for k in ENV_KEYS if k in os.environ}
    env.setdefault("WINEDEBUG", "-all")
    with VirtualDisplay() as vd:
        if vd.display:
            env["DISPLAY"] = vd.display
        return _run_game(root, bot_args, env, timeout, desktop, log)


def _run_game(root, bot_args, env, timeout, desktop, log):
    cmd = ["wine"]
    if desktop:
        cmd += ["explorer", f"/desktop={desktop}"]
    cmd += [EXE_NAME] + pad_args(bot_args)
    print("+", " ".join(a if not a.startswith("-bot_pad=") else "-bot_pad=..." for a in cmd), "(cwd", str(root) + ")", flush=True)
    t0 = time.time()
    with open(log, "w") if log else open(os.devnull, "w") as out:
        try:
            proc = subprocess.run(cmd, cwd=root, env=env, stdout=out, stderr=subprocess.STDOUT, timeout=timeout)
            code = proc.returncode
        except subprocess.TimeoutExpired:
            print(f"timeout after {timeout}s, killing", flush=True)
            subprocess.run(["wineserver", "-k"], env=env)
            code = -1
    print(f"exit code {code} after {time.time() - t0:.1f}s", flush=True)
    return code


def read_blocks(out_dir):
    """bot_blocks.txt: [(addr, size, [referencing rvas])], sorted by address."""
    path = out_dir / "bot_blocks.txt"
    blocks = []
    if path.exists():
        for line in path.read_text().splitlines():
            if line.startswith("#"):
                continue
            parts = line.split()
            if len(parts) >= 2:
                blocks.append((int(parts[0], 16), int(parts[1]), [int(p, 16) for p in parts[2:]]))
    blocks.sort()
    return blocks


def find_block(blocks, addr):
    import bisect

    i = bisect.bisect_right([b[0] for b in blocks], addr) - 1
    if i >= 0 and blocks[i][0] <= addr < blocks[i][0] + blocks[i][1]:
        return blocks[i]
    return None


def read_report(out_dir):
    path = out_dir / "bot_report.txt"
    if not path.exists():
        return {}
    rep = {}
    for line in path.read_text().splitlines():
        if " " in line:
            k, v = line.split(" ", 1)
            rep[k] = v
    return rep


def read_trace(out_dir):
    """bot_trace.txt: frame rng x y z hp rec_idx per logic frame."""
    path = out_dir / "bot_trace.txt"
    rows = []
    if path.exists():
        for line in path.read_text().splitlines():
            parts = line.split()
            if len(parts) >= 7 and parts[0].lstrip('-').isdigit():
                rows.append(tuple(int(p) for p in parts[:7]))
    return rows


def tail(path, n=20):
    path = Path(path)
    if not path.exists():
        return ""
    lines = path.read_text(errors="replace").splitlines()
    return "\n".join(lines[-n:])
