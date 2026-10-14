#!/usr/bin/env python3
"""Rewrite `.4byte SYMBOL+0xOFF` words in asm/ after data symbols were split or added: each
word is turned back into its address and named again from config/symbols.txt (exact symbol,
or the enclosing sized symbol + offset).

    tools/resymbolize.py OLDNAME...      # OLDNAME may be gone from symbols.txt already
    tools/resymbolize.py --at OLDNAME=ADDR ...
"""
import bisect
import glob
import os
import re
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools"))
from symbols import load_symbols  # noqa: E402


def main(args):
    syms = load_symbols(os.path.join(ROOT, "config/symbols.txt"))
    base = {}
    for a in args:
        if "=" in a:
            n, v = a.split("=")
            base[n] = int(v, 16)
        else:
            s = next((s for s in syms.values() if s.name == a), None)
            if s is None:
                sys.exit(f"{a}: give its old address as {a}=ADDR")
            base[a] = s.addr
    starts = sorted(a for a, s in syms.items() if s.kind in ("data", "ram") and s.size)

    def name(v):
        s = syms.get(v)
        if s is not None and s.kind in ("data", "ram"):
            return s.name
        i = bisect.bisect_right(starts, v) - 1
        if i >= 0 and v < starts[i] + syms[starts[i]].size:
            return f"{syms[starts[i]].name}+0x{v - starts[i]:X}"
        return f"0x{v:08X}"

    pat = re.compile(r"\.4byte (" + "|".join(map(re.escape, base)) + r")(?:\+0x([0-9A-Fa-f]+))?\b")
    changed = 0
    for p in glob.glob(os.path.join(ROOT, "asm/**/*.s"), recursive=True):
        text = open(p).read()
        new = pat.sub(lambda m: ".4byte " + name(base[m.group(1)] + int(m.group(2) or "0", 16)), text)
        if new != text:
            open(p, "w").write(new)
            changed += 1
    print(f"{changed} files changed")


if __name__ == "__main__":
    main(sys.argv[1:])
