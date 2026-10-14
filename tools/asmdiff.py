#!/usr/bin/env python3
"""Compare a decompiled function with the original: disassembly of build/report/UNIT.o (your
C) against build/expected/UNIT.o (the original), line by line.

    tools/asmdiff.py FUNC            # builds both objects with ninja first

Prints MATCH when code and relocations are identical, otherwise a unified diff (- original,
+ yours). Offsets are relative to the function start.
"""
import difflib
import os
import re
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools"))
from symbols import binutil, load_symbols, load_units  # noqa: E402


def disasm(obj, func):
    out = subprocess.run([binutil("objdump"), "-d", "-r", "--disassemble=" + func, obj],
                         cwd=ROOT, capture_output=True, text=True).stdout
    lines, start = [], None
    for line in out.split("\n"):
        m = re.match(r"^\s*([0-9a-f]+):\s+(.*)$", line)
        if not m:
            continue
        off = int(m.group(1), 16)
        start = off if start is None else start
        body = re.sub(r"\s+", " ", m.group(2)).strip()
        body = re.sub(r"<[^>]*>", "", body)  # objdump's symbol+offset hints differ between objects
        body = re.sub(r"\s*@ \(.*$", "", body)  # literal address comments
        # branch targets relative to the function start
        body = re.sub(r"^(\S+ (?:\S+ )?b[a-z.]*) ([0-9a-f]+)\s*$",
                      lambda b: f"{b.group(1)} +0x{int(b.group(2), 16) - start:x}", body)
        lines.append(f"{off - start:5x}: {body}")
    # a call with a relocation: the encoded offset is a placeholder, the relocation says where
    for i in range(len(lines) - 1):
        if "R_ARM_" in lines[i + 1] and lines[i + 1].split(":")[0] == lines[i].split(":")[0]:
            lines[i] = re.sub(r" \+0x-?[0-9a-f]+$", "", lines[i])
    return lines


def main(func):
    syms = load_symbols(os.path.join(ROOT, "config/symbols.txt"))
    sym = next((s for s in syms.values() if s.name == func), None)
    if sym is None:
        sys.exit(f"{func} is not in config/symbols.txt")
    unit = next(u[2] for u in load_units(os.path.join(ROOT, "config/units.txt")) if u[0] <= sym.addr < u[1])
    if not os.path.exists(os.path.join(ROOT, "src", unit + ".c")):
        sys.exit(f"src/{unit}.c does not exist yet (tools/new_c_unit.py {unit})")
    exp, mine = f"build/expected/{unit}.o", f"build/report/{unit}.o"
    ninja = os.path.join(ROOT, "tools/bin/ninja")
    r = subprocess.run([ninja if os.path.exists(ninja) else "ninja", exp, mine], cwd=ROOT)
    if r.returncode:
        sys.exit(r.returncode)
    a, b = disasm(exp, func), disasm(mine, func)
    if not b:
        sys.exit(f"{func} is not in {mine} (still ASM_FUNC?)")
    if a == b:
        print(f"MATCH {func} ({len(a)} lines)")
        return 0
    sys.stdout.writelines(l + "\n" for l in difflib.unified_diff(a, b, "original", "yours", lineterm="", n=3))
    return 1


if __name__ == "__main__":
    if len(sys.argv) != 2:
        sys.exit(__doc__)
    sys.exit(main(sys.argv[1]))
