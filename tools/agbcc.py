#!/usr/bin/env python3
"""Compile a C file with agbcc the way the original was built (cpp -> agbcc -> as).

    tools/agbcc.py [--report] -o OUT.o IN.c

--report leaves out the functions that are still assembly (ASM_FUNC): that object is the
"base" objdiff compares, so only real C counts as decompiled. Writes OUT.o.d (headers and
included .s files) for ninja.
"""
import os
import re
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools"))
from symbols import binutil  # noqa: E402
AGBCC = os.path.join(ROOT, "tools/agbcc/bin/agbcc")
CFLAGS = ["-mthumb-interwork", "-Wimplicit", "-Wparentheses", "-Werror", "-O2", "-fhex-asm"]


def run(cmd, **kw):
    r = subprocess.run(cmd, **kw)
    if r.returncode != 0:
        sys.exit(r.returncode)
    return r


def game_flags(src):
    """The game's own source files were built with -funroll-loops; the sound driver and the
    libraries (sound_*, lib_* units) were not."""
    return [] if os.path.basename(src).startswith(("sound_", "lib_")) else ["-funroll-loops"]


def unit_flags(path):
    """Extra agbcc flags of a unit, from a first-line comment like `/* agbcc: -fno-unroll-loops */`
    (for original source files that were compiled with other options)."""
    with open(path) as f:
        m = re.match(r"/\* agbcc: ([-\w ]+) \*/", f.readline())
    return m.group(1).split() if m else []


def main():
    args = sys.argv[1:]
    report = "--report" in args
    args = [a for a in args if a != "--report"]
    out = args[args.index("-o") + 1]
    src = [a for i, a in enumerate(args) if a != "-o" and (i == 0 or args[i - 1] != "-o")][0]
    os.makedirs(os.path.dirname(out) or ".", exist_ok=True)
    base = out[:-2]
    defs = ["-DREPORT"] if report else []
    run(["cpp", "-nostdinc", "-undef", "-std=gnu89",
         "-I", "include", "-I", "tools/agbcc/include", "-iquote", "include", *defs,
         "-MMD", "-MF", base + ".cpp.d", "-MT", out, src, "-o", base + ".i"], cwd=ROOT)
    run([AGBCC, *CFLAGS, *game_flags(src), *unit_flags(os.path.join(ROOT, src)), base + ".i", "-o", base + ".agb.s"],
        cwd=ROOT)
    with open(os.path.join(ROOT, base + ".agb.s"), "a") as f:
        f.write("\t.text\n\t.align 2, 0\n")
    run([binutil("as"), "-mcpu=arm7tdmi", "-I", ".", "--MD", base + ".as.d", "-o", out, base + ".agb.s"],
        cwd=ROOT)
    deps = set()
    for d in (base + ".cpp.d", base + ".as.d"):
        text = open(os.path.join(ROOT, d)).read().replace("\\\n", " ")
        deps.update(text.split(":", 1)[1].split())
        os.remove(os.path.join(ROOT, d))
    with open(os.path.join(ROOT, out + ".d"), "w") as f:
        f.write(out + ": " + " ".join(sorted(x for x in deps if not x.endswith(".agb.s"))) + "\n")


if __name__ == "__main__":
    main()
