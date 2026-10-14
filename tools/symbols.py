"""Reading and writing config/symbols.txt and config/units.txt (standard library only)."""
import sys


class Sym:
    __slots__ = ("addr", "name", "kind", "size", "flags")

    def __init__(self, addr, name, kind, size=None, flags=()):
        self.addr, self.name, self.kind, self.size, self.flags = addr, name, kind, size, set(flags)

    @property
    def is_func(self):
        return self.kind in ("thumb", "arm")


def load_symbols(path):
    """config/symbols.txt: `ADDR NAME KIND [size=N] [flag...]`; KIND thumb, arm, data, ram."""
    syms = {}
    for n, line in enumerate(open(path), 1):
        line = line.split("#")[0].split()
        if not line:
            continue
        addr, name, kind = int(line[0], 16), line[1], line[2]
        size, flags = None, []
        for extra in line[3:]:
            if extra.startswith("size="):
                size = int(extra[5:], 16)
            else:
                flags.append(extra)
        if addr in syms:
            sys.exit(f"{path}:{n}: address {addr:08X} defined twice")
        syms[addr] = Sym(addr, name, kind, size, flags)
    return syms


def write_symbols(path, syms):
    with open(path, "w") as f:
        f.write("# Symbols of Mario & Luigi: Superstar Saga (EU, A88P): ADDR NAME KIND [size=N]\n"
                "# KIND: thumb/arm = function, data = ROM data, ram = EWRAM/IWRAM variable\n")
        for s in sorted(syms.values(), key=lambda s: s.addr):
            extra = (f" size={s.size:X}" if s.size else "") + "".join(" " + x for x in sorted(s.flags))
            f.write(f"{s.addr:08X} {s.name} {s.kind}{extra}\n")


def load_units(path):
    """config/units.txt: `START END NAME` per code unit, in ROM order."""
    units = []
    for line in open(path):
        line = line.split("#")[0].split()
        if line:
            units.append((int(line[0], 16), int(line[1], 16), line[2]))
    return units


def binutil(name, root=None):
    """Path of arm-none-eabi-NAME: tools/binutils (tools/setup.sh) or the one on PATH."""
    import os
    root = root or os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    local = os.path.join(root, "tools/binutils/bin", "arm-none-eabi-" + name)
    return local if os.path.exists(local) else "arm-none-eabi-" + name
