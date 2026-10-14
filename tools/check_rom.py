#!/usr/bin/env python3
"""Check a built ROM against config/rom.sha1; on a mismatch name the first differing unit
and function (needs baserom.gba for that).

    tools/check_rom.py mlss.gba
"""
import bisect
import hashlib
import os
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools"))
from symbols import binutil, load_symbols, load_units  # noqa: E402

ROM_START = 0x08000000


def main(path):
    data = open(path, "rb").read()
    want = open(os.path.join(ROOT, "config/rom.sha1")).read().split()[0]
    got = hashlib.sha1(data).hexdigest()
    if got == want:
        print(f"{path}: OK ({got})")
        return 0
    print(f"{path}: MISMATCH (sha1 {got}, expected {want})")
    syms = load_symbols(os.path.join(ROOT, "config/symbols.txt"))
    elf = path[:-4] + ".elf" if not path.startswith("mlss") else "build/mlss.elf"
    if os.path.exists(os.path.join(ROOT, elf)):
        # first function that is not at its original address
        out = subprocess.run([binutil("nm"), elf], cwd=ROOT, capture_output=True, text=True).stdout
        placed = {}
        for line in out.split("\n"):
            p = line.split()
            if len(p) == 3 and p[1] in "Tt":
                placed[p[2]] = int(p[0], 16) & ~1
        for a, s in sorted(syms.items()):
            if s.is_func and s.name in placed and placed[s.name] != a:
                print(f"  {s.name} is at 0x{placed[s.name]:08X}, original 0x{a:08X}: "
                      f"the code before it has a different size")
                break
    base = os.path.join(ROOT, "baserom.gba")
    if not os.path.exists(base):
        return 1
    orig = open(base, "rb").read()
    if len(data) != len(orig):
        print(f"  size 0x{len(data):X}, original 0x{len(orig):X}")
    n = min(len(data), len(orig))
    first = next((i for i in range(n) if data[i] != orig[i]), n)
    addr = ROM_START + first
    units = load_units(os.path.join(ROOT, "config/units.txt"))
    unit = next((u[2] for u in units if u[0] <= addr < u[1]), "data")
    funcs = sorted(a for a, s in syms.items() if s.is_func)
    i = bisect.bisect_right(funcs, addr) - 1
    func = f"{syms[funcs[i]].name}+0x{addr - funcs[i]:X}" if i >= 0 else "?"
    print(f"  first difference at 0x{addr:08X} (unit {unit}, {func}): "
          f"{data[first:first + 8].hex()} vs original {orig[first:first + 8].hex()}")
    return 1


if __name__ == "__main__":
    sys.exit(main(sys.argv[1]))
