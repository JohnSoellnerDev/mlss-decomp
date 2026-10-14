#!/usr/bin/env python3
"""Start decompiling a unit: write src/UNIT.c with every function as ASM_FUNC (its original
assembly), then run configure.py. The ROM stays identical; replace ASM_FUNC lines with C one
by one.

    tools/new_c_unit.py UNIT
"""
import os
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools"))
from symbols import load_symbols, load_units  # noqa: E402


def main(unit):
    units = {u[2]: u for u in load_units(os.path.join(ROOT, "config/units.txt"))}
    if unit not in units:
        sys.exit(f"unknown unit {unit} (see config/units.txt)")
    path = os.path.join(ROOT, "src", unit + ".c")
    if os.path.exists(path):
        sys.exit(f"{path} exists already")
    start, end, _ = units[unit]
    syms = load_symbols(os.path.join(ROOT, "config/symbols.txt"))
    lines = ['#include "global.h"', ""]
    for a, s in sorted(syms.items()):
        if not (start <= a < end and s.is_func):
            continue
        inc = f"asm/{unit}/{s.name}.s"
        if s.kind == "arm" or a % 4:
            # agbcc only writes 4-byte aligned Thumb functions: keep these as plain assembly
            macro = "arm_func_start" if s.kind == "arm" else "non_word_aligned_thumb_func_start"
            lines.append(f'asm(".include \\"asm/macros.inc\\"\\n"\n'
                         f'    ".syntax unified\\n"\n'
                         f'    "\\t{macro} {s.name}\\n"\n'
                         f'    ".include \\"{inc}\\"\\n"\n'
                         f'    "\\tfunc_end {s.name}\\n"\n'
                         f'    ".syntax divided\\n\\t.thumb\\n");')
        else:
            lines.append(f'ASM_FUNC("{inc}", void {s.name}(void))')
        lines.append("")
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "w") as f:
        f.write("\n".join(lines))
    subprocess.run([sys.executable, os.path.join(ROOT, "configure.py")], check=True)
    print(f"wrote {os.path.relpath(path, ROOT)}")


if __name__ == "__main__":
    if len(sys.argv) != 2:
        sys.exit(__doc__)
    main(sys.argv[1])
