#!/usr/bin/env python3
"""Check that src/UNIT.c (with its remaining ASM_FUNC functions) still gives the original ROM:
compiles the unit in a private directory, links it with the original objects of every other
unit and compares the result with baserom.gba.

    tools/check_unit.py UNIT

Needs baserom.gba and the shared objects of the other units (tools/bin/ninja expected).
Prints OK, or the first function that moved or differs.
"""
import bisect
import os
import subprocess
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools"))
from symbols import binutil  # noqa: E402
from unitbuild import Project, run  # noqa: E402

ROM_START = 0x08000000


def main(name):
    p = Project()
    unit = p.unit(name)
    script = os.path.join(ROOT, "build/ld_script_expected.ld")
    if not os.path.exists(script) or not os.path.exists(os.path.join(ROOT, "baserom.gba")):
        sys.exit("needs baserom.gba and the shared objects: run tools/bin/ninja expected first")
    with tempfile.TemporaryDirectory(prefix="check-unit-") as tmp:
        obj = p.build_c(unit, tmp, report=False)
        text = open(script).read()
        key = f"build/expected/{name}.o("
        if key not in text:
            sys.exit(f"{name} is not in build/ld_script_expected.ld: run python3 configure.py")
        with open(os.path.join(tmp, "ld.ld"), "w") as f:
            f.write(text.replace(key, obj + "("))
        elf, rom = os.path.join(tmp, "unit.elf"), os.path.join(tmp, "unit.gba")
        run([binutil("ld"), "-T", os.path.join(tmp, "ld.ld"), "-o", elf])
        run([binutil("objcopy"), "-O", "binary", elf, rom])
        data = open(rom, "rb").read()
        nm = subprocess.run([binutil("nm"), obj], capture_output=True, text=True).stdout
    orig = open(os.path.join(ROOT, "baserom.gba"), "rb").read()
    if data == orig:
        print(f"OK {name}: the ROM is identical")
        return 0
    start, end = unit[0], unit[1]
    funcs = p.funcs(unit)
    n = min(len(data), len(orig))
    first = next((i for i in range(n) if data[i] != orig[i]), n)
    addr = ROM_START + first
    i = bisect.bisect_right([s.addr for s in funcs], addr) - 1
    where = f"{funcs[i].name}+0x{addr - funcs[i].addr:X}" if i >= 0 and start <= addr < end else "after the unit"
    print(f"MISMATCH {name}: first difference at 0x{addr:08X} ({where})")
    if len(data) != len(orig):
        print(f"  the ROM is 0x{len(data) - len(orig):+X} bytes longer: some function or data in the unit has a "
              f"different size (data defined in C? a function not matching?)")
    return 1


if __name__ == "__main__":
    if len(sys.argv) != 2:
        sys.exit(__doc__)
    sys.exit(main(sys.argv[1]))
