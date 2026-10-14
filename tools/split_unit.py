#!/usr/bin/env python3
"""Split a unit into several units at function boundaries: config/units.txt gets the new
ranges, src/UNIT.c is cut into one file per new unit (each keeps the declarations at the top
of the old file and gets prototypes for the functions it calls in the other pieces), and the
asm/ files move along (tools/resplit.py).

    tools/split_unit.py UNIT ADDR...       # new units start at these function addresses
    tools/split_unit.py UNIT --auto MIN    # cut out every run of decompiled functions >= MIN bytes
    tools/split_unit.py ... --dry-run      # only print the pieces

New units are named code_ADDR. Check the result with tools/bin/ninja (and `ninja expected`).
"""
import os
import re
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools"))
from symbols import load_symbols, load_units  # noqa: E402


def find_blocks(text, unit, funcs):
    """Start offset of each function's block in the C file (address order), and whether it is C."""
    starts, is_c = [], []
    for s in funcs:
        path = re.escape(f"asm/{unit}/{s.name}.s")
        pats = [(rf'^(ASM_FUNC|NONMATCH)\("{path}"', False),
                (rf'^asm\((?:[^;]|;(?!\s*$))*?func_start {re.escape(s.name)}\\n', False),
                (rf'^[A-Za-z_][\w \t\*]*?\b{re.escape(s.name)}\s*\([^;{{}}]*\)\s*\{{', True)]
        for pat, c in pats:
            m = re.search(pat, text, re.M | re.S)
            if m:
                break
        else:
            sys.exit(f"{s.name}: not found in src/{unit}.c")
        pos = m.start()
        # comment lines directly above the function belong to it
        while True:
            prev = text.rfind("\n", 0, pos - 1) + 1
            line = text[prev:pos].strip()
            if pos > 0 and (line.startswith("//") or line.startswith("/*") or line.startswith("*")):
                pos = prev
            else:
                break
        starts.append(pos)
        is_c.append(c)
    if starts != sorted(starts):
        sys.exit(f"src/{unit}.c: functions are not in address order")
    return starts, is_c


def declaration(block, name):
    """Prototype of the function defined in a block (ASM_FUNC/NONMATCH argument or C header)."""
    m = re.search(r'(?:ASM_FUNC|NONMATCH)\("[^"]*",\s*(.*)\)\s*$', block.split("\n", 1)[0])
    if m:
        return m.group(1).strip() + ";"
    if block.lstrip().startswith("asm("):
        return f"void {name}(void);"
    body = re.sub(r"^\s*(//[^\n]*\n|/\*.*?\*/\s*)*", "", block, flags=re.S)
    return " ".join(body[:body.index("{")].split()) + ";"


def main(argv):
    dry = "--dry-run" in argv
    argv = [a for a in argv if a != "--dry-run"]
    if len(argv) < 2:
        sys.exit(__doc__)
    unit = argv[0]
    units = load_units(os.path.join(ROOT, "config/units.txt"))
    syms = load_symbols(os.path.join(ROOT, "config/symbols.txt"))
    try:
        ustart, uend, _ = next(u for u in units if u[2] == unit)
    except StopIteration:
        sys.exit(f"unknown unit {unit}")
    funcs = [s for a, s in sorted(syms.items()) if ustart <= a < uend and s.is_func]
    src = os.path.join(ROOT, "src", unit + ".c")
    text = open(src).read()
    starts, is_c = find_blocks(text, unit, funcs)
    ends = [f.addr for f in funcs[1:]] + [uend]

    if argv[1] == "--auto":
        minsize, cuts, i = int(argv[2], 0), set(), 0
        while i < len(funcs):
            if not is_c[i] or funcs[i].addr % 4:
                i += 1
                continue
            j = i
            while j < len(funcs) and is_c[j]:
                j += 1
            if ends[j - 1] - funcs[i].addr >= minsize:
                cuts.add(funcs[i].addr)
                if j < len(funcs):
                    cuts.add(funcs[j].addr)
            i = j
        cuts.discard(ustart)
        cuts = sorted(c for c in cuts if c % 4 == 0)
    else:
        cuts = sorted(int(a, 16) for a in argv[1:])
    by_addr = {f.addr: k for k, f in enumerate(funcs)}
    for c in cuts:
        if c not in by_addr or c == ustart:
            sys.exit(f"{c:08X} is not a function start inside {unit} (other than the first)")
        if c % 4:
            sys.exit(f"{c:08X} is not 4-byte aligned")
    if not cuts:
        print(f"{unit}: nothing to split")
        return

    bounds = [ustart] + cuts + [uend]
    pieces = []
    for a, b in zip(bounds, bounds[1:]):
        ks = [k for k, f in enumerate(funcs) if a <= f.addr < b]
        pieces.append((a, b, unit if a == ustart else f"code_{a:08X}", ks))

    pre = text[:starts[0]]
    blocks = [text[starts[k]:(starts[k + 1] if k + 1 < len(funcs) else len(text))] for k in range(len(funcs))]
    decl = {k: declaration(blocks[k], funcs[k].name) for k in range(len(funcs))}

    out = {}
    for a, b, name, ks in pieces:
        body = "".join(blocks[k] for k in ks).rstrip("\n") + "\n"
        protos = []
        for k in range(len(funcs)):
            if k in ks:
                continue
            n = funcs[k].name
            if re.search(rf"\b{re.escape(n)}\b", body) and not re.search(rf"\b{re.escape(n)}\s*\(", pre):
                protos.append(decl[k])
        head = pre.rstrip("\n") + "\n"
        if protos:
            head += "\n" + "\n".join(protos) + "\n"
        out[name] = head + "\n" + body
        done = sum((ends[k] - funcs[k].addr) for k in ks if is_c[k])
        state = "complete" if all(is_c[k] for k in ks) else f"{done:#x}/{b - a:#x} C"
        print(f"{a:08X} {b:08X} {name:16} {len(ks):3} funcs  {state}")
    if dry:
        return

    for name, t in out.items():
        open(os.path.join(ROOT, "src", name + ".c"), "w").write(t)
    lines = open(os.path.join(ROOT, "config/units.txt")).read().split("\n")
    new = []
    for line in lines:
        f = line.split("#")[0].split()
        if len(f) == 3 and f[2] == unit:
            new += [f"{a:08X} {b:08X} {name}" for a, b, name, _ in pieces]
        else:
            new.append(line)
    open(os.path.join(ROOT, "config/units.txt"), "w").write("\n".join(new))
    subprocess.run([sys.executable, os.path.join(ROOT, "tools/resplit.py")], check=True)


if __name__ == "__main__":
    main(sys.argv[1:])
