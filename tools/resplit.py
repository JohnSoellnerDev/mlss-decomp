#!/usr/bin/env python3
"""After editing config/units.txt: move every asm/UNIT/FUNC.s into the directory of the unit
that now contains the function and fix the asm paths in src/. C files are not split or merged
(move decompiled functions by hand); the script refuses when a src/ file no longer matches a
unit.

    tools/resplit.py
"""
import glob
import os
import re
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools"))
from symbols import load_symbols, load_units  # noqa: E402


def main():
    units = load_units(os.path.join(ROOT, "config/units.txt"))
    syms = load_symbols(os.path.join(ROOT, "config/symbols.txt"))
    names = {u[2] for u in units}
    for path in glob.glob(os.path.join(ROOT, "src/**/*.c"), recursive=True):
        unit = os.path.relpath(path, os.path.join(ROOT, "src"))[:-2]
        if unit not in names:
            sys.exit(f"{os.path.relpath(path, ROOT)} has no unit in config/units.txt any more: move its C first")
    where = {}
    for path in glob.glob(os.path.join(ROOT, "asm/**/*.s"), recursive=True):
        rel = os.path.relpath(path, os.path.join(ROOT, "asm"))
        if "/" in rel:
            where[os.path.basename(rel)[:-2]] = rel
    moved = {}
    for a, s in sorted(syms.items()):
        if not s.is_func:
            continue
        unit = next((u[2] for u in units if u[0] <= a < u[1]), None)
        if unit is None or s.name not in where:
            continue
        old, new = where[s.name], f"{unit}/{s.name}.s"
        if old != new:
            os.makedirs(os.path.join(ROOT, "asm", unit), exist_ok=True)
            os.rename(os.path.join(ROOT, "asm", old), os.path.join(ROOT, "asm", new))
            moved[old] = new
    for d in sorted(glob.glob(os.path.join(ROOT, "asm/**/"), recursive=True), reverse=True):
        if d.rstrip("/") != os.path.join(ROOT, "asm") and not os.listdir(d):
            os.rmdir(d)
    pat = re.compile(r'asm/([A-Za-z0-9_/]+\.s)')
    for path in glob.glob(os.path.join(ROOT, "src/**/*.c"), recursive=True):
        text = open(path).read()
        text2 = pat.sub(lambda m: "asm/" + moved.get(m.group(1), m.group(1)), text)
        if text2 != text:
            open(path, "w").write(text2)
    subprocess.run([sys.executable, os.path.join(ROOT, "configure.py")], check=True)
    print(f"moved {len(moved)} asm files")


if __name__ == "__main__":
    main()
