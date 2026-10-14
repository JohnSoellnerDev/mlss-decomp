#!/usr/bin/env python3
"""Rename a symbol everywhere: config/symbols.txt, asm/ (file name too), src/, include/.

    tools/rename.py OLD NEW
"""
import os
import re
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools"))
from symbols import load_symbols, load_units, write_symbols  # noqa: E402


def main(old, new):
    if not re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", new):
        sys.exit(f"not a C identifier: {new}")
    path = os.path.join(ROOT, "config/symbols.txt")
    syms = load_symbols(path)
    byname = {s.name: s for s in syms.values()}
    if old not in byname:
        sys.exit(f"{old} is not in config/symbols.txt")
    if new in byname:
        sys.exit(f"{new} exists already (0x{byname[new].addr:08X})")
    sym = byname[old]
    sym.name = new
    write_symbols(path, syms)
    if sym.is_func:
        unit = next((u[2] for u in load_units(os.path.join(ROOT, "config/units.txt")) if u[0] <= sym.addr < u[1]), None)
        if unit and os.path.exists(os.path.join(ROOT, "asm", unit, old + ".s")):
            os.rename(os.path.join(ROOT, "asm", unit, old + ".s"), os.path.join(ROOT, "asm", unit, new + ".s"))
    word = re.compile(r"(?<![A-Za-z0-9_])" + re.escape(old) + r"(?![A-Za-z0-9_])")
    changed = 0
    for top in ("asm", "src", "include"):
        for dp, _, files in os.walk(os.path.join(ROOT, top)):
            for fn in files:
                if not fn.endswith((".s", ".c", ".h", ".inc")):
                    continue
                p = os.path.join(dp, fn)
                text = open(p).read()
                if old not in text:
                    continue
                text2 = word.sub(new, text)
                if text2 != text:
                    open(p, "w").write(text2)
                    changed += 1
    subprocess.run([sys.executable, os.path.join(ROOT, "configure.py")], check=True)
    print(f"{old} -> {new} (0x{sym.addr:08X}), {changed} files changed")


if __name__ == "__main__":
    if len(sys.argv) != 3:
        sys.exit(__doc__)
    main(sys.argv[1], sys.argv[2])
