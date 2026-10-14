#!/usr/bin/env python3
"""Compare a decompiled function with the original: the function from your C (src/UNIT.c,
compiled without the ASM_FUNC functions) against the original assembly, line by line.

    tools/asmdiff.py FUNC

Prints MATCH when code and relocations are identical, otherwise a unified diff (- original,
+ yours). Offsets are relative to the function start. Builds in a private temporary
directory, so it can run while others build.
"""
import difflib
import os
import re
import subprocess
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools"))
from symbols import binutil  # noqa: E402
from unitbuild import Project  # noqa: E402


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
    p = Project()
    sym = p.func(func)
    unit = p.unit_of(sym.addr)
    with tempfile.TemporaryDirectory(prefix="asmdiff-") as tmp:
        exp = p.build_expected(unit, tmp)
        mine = p.build_c(unit, tmp, report=True)
        a, b = disasm(exp, func), disasm(mine, func)
    if not b:
        sys.exit(f"{func} is not in your C yet (still ASM_FUNC in src/{unit[2]}.c?)")
    if a == b:
        print(f"MATCH {func} ({len(a)} lines)")
        return 0
    sys.stdout.writelines(l + "\n" for l in difflib.unified_diff(a, b, "original", "yours", lineterm="", n=3))
    return 1


if __name__ == "__main__":
    if len(sys.argv) != 2:
        sys.exit(__doc__)
    sys.exit(main(sys.argv[1]))
