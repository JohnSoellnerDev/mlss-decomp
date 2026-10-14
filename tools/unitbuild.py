"""Building single units in a private directory (used by asmdiff.py and check_unit.py, so that
several people or agents can check their work at the same time without touching build/)."""
import os
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools"))
from symbols import binutil, load_symbols, load_units  # noqa: E402


def expected_source(name, funcs):
    """Assembly source of a unit's original code: every function from asm/UNIT/FUNC.s."""
    lines = ['\t.include "asm/macros.inc"', "\t.syntax unified", "\t.text"]
    for s in funcs:
        if s.kind == "arm":
            start = "arm_func_start"
        else:
            start = "thumb_func_start" if s.addr % 4 == 0 else "non_word_aligned_thumb_func_start"
        lines += [f"\t{start} {s.name}", f'\t.include "asm/{name}/{s.name}.s"', f"\tfunc_end {s.name}", ""]
    lines.append("\t.align 2, 0")
    return "\n".join(lines) + "\n"


class Project:
    def __init__(self):
        self.syms = load_symbols(os.path.join(ROOT, "config/symbols.txt"))
        self.units = load_units(os.path.join(ROOT, "config/units.txt"))

    def unit(self, name):
        for u in self.units:
            if u[2] == name:
                return u
        sys.exit(f"unknown unit {name} (see config/units.txt)")

    def unit_of(self, addr):
        return next((u for u in self.units if u[0] <= addr < u[1]), None)

    def func(self, name):
        s = next((s for s in self.syms.values() if s.name == name), None)
        if s is None or not s.is_func:
            sys.exit(f"{name} is not a function in config/symbols.txt")
        return s

    def funcs(self, unit):
        start, end, _ = unit
        return [self.syms[a] for a in sorted(self.syms) if start <= a < end and self.syms[a].is_func]

    def build_expected(self, unit, tmp):
        src = os.path.join(tmp, unit[2].replace("/", "_") + ".expected.s")
        obj = src[:-2] + ".o"
        with open(src, "w") as f:
            f.write(expected_source(unit[2], self.funcs(unit)))
        run([binutil("as"), "-mcpu=arm7tdmi", "-I", ".", "-o", obj, src])
        return obj

    def build_c(self, unit, tmp, report):
        src = os.path.join("src", unit[2] + ".c")
        if not os.path.exists(os.path.join(ROOT, src)):
            sys.exit(f"{src} does not exist yet (tools/new_c_unit.py {unit[2]})")
        obj = os.path.join(tmp, unit[2].replace("/", "_") + (".report.o" if report else ".o"))
        run([sys.executable, os.path.join(ROOT, "tools/agbcc.py")] + (["--report"] if report else []) +
            ["-o", obj, src])
        return obj


def run(cmd):
    r = subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True)
    out = "\n".join(l for l in (r.stdout + r.stderr).split("\n")
                    if l.strip() and "use of r13 is deprecated" not in l and l.strip() != "Assembler messages:"
                    and not l.endswith("Assembler messages:"))
    if r.returncode != 0:
        print(out)
        sys.exit(r.returncode)
    return r
