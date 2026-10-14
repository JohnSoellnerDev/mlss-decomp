#!/usr/bin/env python3
"""Disassemble functions of the MLSS ROM into GNU as source, one file per function.

    tools/disasm.py [--rom baserom.gba] [--symbols config/symbols.txt] FUNC...   (names or addresses)
    tools/disasm.py --all [--start ADDR --end ADDR] --out DIR

Every function runs from its entry to the next symbol in config/symbols.txt. Inside that
range the code is followed recursively (branches, jump tables); words loaded with
`ldr rX, [pc, #n]` become literal pools. Pool words are written as symbols where the value
is a known function or data address (see `symbolize`). Bytes behind the last reached
instruction/pool word that are not alignment padding are reported as data ("tail"), so the
caller can give them their own data symbol.

The output is checked by tools/verify_asm.py (assembles everything and compares with the
ROM); instructions that GNU as encodes differently are written as raw `.2byte`/`.4byte`.
Needs capstone (`.venv`, see README).
"""
import argparse
import os
import re
import struct
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import capstone
from capstone import arm as csarm

ROM_BASE = 0x08000000
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

EWRAM = (0x02000000, 0x02040000)
IWRAM = (0x03000000, 0x03007E00)  # without the stack and the BIOS area at the end


# ---------------------------------------------------------------------------- symbols

from symbols import Sym, binutil, load_symbols, load_units, write_symbols  # noqa: E402


# ---------------------------------------------------------------------------- analysis

class Func:
    def __init__(self, sym, limit):
        self.sym, self.addr, self.limit = sym, sym.addr, limit
        self.thumb = sym.kind == "thumb"
        self.insns = {}       # addr -> capstone insn
        self.pool = set()     # addresses of 4-byte pool words
        self.jt = {}          # addr of jump table word -> target
        self.labels = set()   # branch / pool / table targets inside the function
        self.calls = set()
        self.inner_bl = set()  # bl targets inside the function
        self.unreached = set()
        self.end = sym.addr
        self.warnings = []


class Disassembler:
    def __init__(self, rom, syms):
        self.rom, self.syms = rom, syms
        self.thumb_cs = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_THUMB)
        self.arm_cs = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_ARM)
        for c in (self.thumb_cs, self.arm_cs):
            c.detail = True
            c.syntax = capstone.CS_OPT_SYNTAX_NOREGNAME  # r9-r12 instead of sb/sl/fp/ip
        self.order = sorted(syms)

    def u16(self, a):
        return struct.unpack_from("<H", self.rom, a - ROM_BASE)[0]

    def u32(self, a):
        return struct.unpack_from("<I", self.rom, a - ROM_BASE)[0]

    def next_symbol(self, addr):
        import bisect
        i = bisect.bisect_right(self.order, addr)
        while i < len(self.order) and self.syms[self.order[i]].kind == "ram":
            i += 1
        return self.order[i] if i < len(self.order) else ROM_BASE + len(self.rom)

    def decode(self, f, a):
        c = self.thumb_cs if f.thumb else self.arm_cs
        n = 4 if not f.thumb else (4 if (self.u16(a) & 0xF800) == 0xF000 else 2)
        if a + n > f.limit:
            return None
        insn = next(c.disasm(self.rom[a - ROM_BASE:a - ROM_BASE + n], a, 1), None)
        if insn is None or insn.size != n:
            return None
        return insn

    def analyze(self, sym, limit=None):
        f = Func(sym, limit or self.next_symbol(sym.addr))
        if sym.size:
            f.limit = min(f.limit, sym.addr + sym.size)
        for _ in range(2):  # second pass: pool words found in pass 1 stop linear decoding
            f.insns.clear(); f.jt.clear(); f.labels.clear(); f.calls.clear(); f.inner_bl.clear()
            pool_before = set(f.pool)
            self._walk(f, [f.addr])
            if f.pool == pool_before:
                break
        covered = [a + i.size for a, i in f.insns.items()] + [a + 4 for a in f.pool] + [a + 4 for a in f.jt]
        f.end = max(covered) if covered else f.addr
        if sym.size:
            f.end = sym.addr + sym.size
        elif f.end < f.limit:
            # bytes up to the next symbol belong to this function, except the zero padding
            # that aligns the next function to 4 bytes
            gap = self.rom[f.end - ROM_BASE:f.limit - ROM_BASE]
            if not (len(gap) == 2 and f.limit % 4 == 0 and gap == bytes(2)):
                f.end = f.limit
                f.warnings.append(f"{len(gap)} bytes behind the last instruction up to {f.limit:08X}")
        self._fill_unreached(f)
        return f

    def _walk(self, f, work):
        step = 2 if f.thumb else 4
        seen = set()
        while work:
            a = work.pop()
            while True:
                if a in seen or a in f.insns or a in f.pool or a in f.jt or not (f.addr <= a < f.limit):
                    break
                if any(p <= a < p + 4 for p in (a - 2,) if p in f.pool):
                    break
                seen.add(a)
                insn = self.decode(f, a)
                if insn is None:
                    f.warnings.append(f"undecodable at {a:08X}")
                    break
                f.insns[a] = insn
                stop = self._effects(f, insn, work)
                if stop:
                    break
                a += insn.size

    def _pc_rel(self, f, insn):
        """Address loaded by `ldr rX, [pc, #n]`, or None."""
        if insn.id != csarm.ARM_INS_LDR or len(insn.operands) < 2:
            return None
        mem = insn.operands[1]
        if mem.type != csarm.ARM_OP_MEM or mem.mem.base != csarm.ARM_REG_PC or mem.mem.index:
            return None
        if f.thumb:
            return ((insn.address + 4) & ~3) + mem.mem.disp
        return insn.address + 8 + mem.mem.disp

    def _effects(self, f, insn, work):
        """Record targets of insn; True when control does not continue behind it."""
        mn = insn.mnemonic
        ops = insn.operands
        pool = self._pc_rel(f, insn)
        if pool is not None:
            if f.addr <= pool < f.limit:
                f.pool.add(pool)
                f.labels.add(pool)
            else:
                f.warnings.append(f"pool word {pool:08X} outside the function ({insn.address:08X})")
            if ops[0].reg == csarm.ARM_REG_PC:
                return insn.cc in (csarm.ARM_CC_AL, csarm.ARM_CC_INVALID)
            return False
        if insn.id in (csarm.ARM_INS_B, csarm.ARM_INS_BL) and ops and ops[0].type == csarm.ARM_OP_IMM:
            t = ops[0].imm
            uncond = insn.cc in (csarm.ARM_CC_AL, csarm.ARM_CC_INVALID)
            if insn.id == csarm.ARM_INS_BL:
                if f.addr < t < f.limit and t not in self.syms:
                    # far jump inside the function (agbcc uses bl when b cannot reach), or a call
                    # of a function that has no symbol yet: follow both ways
                    f.labels.add(t); work.append(t)
                    f.inner_bl.add(t)
                    return False
                f.calls.add(t)
                return False
            if f.addr <= t < f.limit:
                f.labels.add(t); work.append(t)
            else:
                f.warnings.append(f"branch out of the function to {t:08X} ({insn.address:08X})")
            return uncond
        if insn.id == csarm.ARM_INS_BX:
            return insn.cc in (csarm.ARM_CC_AL, csarm.ARM_CC_INVALID)
        if insn.id == csarm.ARM_INS_POP or (insn.id in (csarm.ARM_INS_LDM,) and not f.thumb):
            regs = [o.reg for o in ops if o.type == csarm.ARM_OP_REG]
            if csarm.ARM_REG_PC in regs:
                return insn.cc in (csarm.ARM_CC_AL, csarm.ARM_CC_INVALID)
            return False
        if insn.id in (csarm.ARM_INS_MOV, csarm.ARM_INS_ADD) and ops and ops[0].type == csarm.ARM_OP_REG \
                and ops[0].reg == csarm.ARM_REG_PC:
            self._jump_table(f, insn, work)
            return insn.cc in (csarm.ARM_CC_AL, csarm.ARM_CC_INVALID)
        if insn.id == csarm.ARM_INS_ADR or (insn.id == csarm.ARM_INS_ADD and len(ops) == 3
                                            and ops[1].type == csarm.ARM_OP_REG and ops[1].reg == csarm.ARM_REG_PC
                                            and ops[2].type == csarm.ARM_OP_IMM):
            base = ((insn.address + 4) & ~3) if f.thumb else insn.address + 8
            t = base + (ops[-1].imm if insn.id == csarm.ARM_INS_ADD else 0)
            if insn.id == csarm.ARM_INS_ADR:
                t = ops[1].imm if ops[1].type == csarm.ARM_OP_IMM else t
            if f.addr <= t < f.limit:
                f.labels.add(t)
        return False

    def _jump_table(self, f, insn, work):
        """agbcc switch: ldr rT, =table; add rA, rI, rT; ldr rA, [rA]; mov pc, rA.

        The table register is followed back through the instructions before the jump (other
        literal loads can sit in between)."""
        table = None
        src = self._reg_source(f, insn.address, insn.operands[-1].reg)
        if src and src[0] == "load":
            add = self._reg_source(f, src[2], src[1])
            if add and add[0] == "add":
                for r in add[1:3]:
                    lit = self._reg_source(f, add[3], r)
                    if lit and lit[0] == "lit" and insn.address < lit[1] < f.limit + 0x10000:
                        table = lit[1]
        if table is None:
            f.warnings.append(f"computed jump without table at {insn.address:08X}")
            return
        if table >= f.limit:
            f.warnings.append(f"jump table {table:08X} outside the function ({insn.address:08X})")
            return
        f.labels.add(table)
        a, first_target = table, f.limit + 0x10000
        while a + 4 <= f.limit and a < first_target and a not in f.insns:
            w = self.u32(a)
            t = w & ~1
            if not (f.addr <= t < f.limit + 0x10000) or table <= t < a + 4:
                break
            if t >= f.limit:
                f.warnings.append(f"jump table target {t:08X} outside the function ({insn.address:08X})")
                first_target = min(first_target, t)
                a += 4
                continue
            f.jt[a] = w
            f.labels.add(t)
            work.append(t)
            if t > table:
                first_target = min(first_target, t)
            a += 4

    def _reg_source(self, f, before, reg, window=40):
        """How reg got its value before address `before` (same block, at most `window` bytes back):
        ("lit", value) | ("load", base_reg, addr) | ("add", reg_a, reg_b, addr) | None."""
        for a in range(before - (2 if f.thumb else 4), before - window, -(2 if f.thumb else 4)):
            i = f.insns.get(a)
            if i is None or not i.operands or i.operands[0].type != csarm.ARM_OP_REG:
                continue
            if i.operands[0].reg != reg or i.id in (csarm.ARM_INS_CMP, csarm.ARM_INS_TST, csarm.ARM_INS_STR,
                                                    csarm.ARM_INS_STRB, csarm.ARM_INS_STRH, csarm.ARM_INS_CMN):
                continue
            p = self._pc_rel(f, i)
            if p is not None:
                return ("lit", self.u32(p))
            ops = i.operands
            if i.id == csarm.ARM_INS_LDR and ops[1].type == csarm.ARM_OP_MEM and not ops[1].mem.index \
                    and ops[1].mem.disp == 0:
                return ("load", ops[1].mem.base, a)
            if i.id == csarm.ARM_INS_ADD and len(ops) == 3 and ops[2].type == csarm.ARM_OP_REG:
                return ("add", ops[1].reg, ops[2].reg, a)
            if i.id == csarm.ARM_INS_ADD and len(ops) == 2 and ops[1].type == csarm.ARM_OP_REG:
                return ("add", ops[0].reg, ops[1].reg, a)
            if i.id in (csarm.ARM_INS_MOV, csarm.ARM_INS_ADD) and ops[1].type == csarm.ARM_OP_REG and \
                    (len(ops) == 2 or (ops[2].type == csarm.ARM_OP_IMM and ops[2].imm == 0)):
                return self._reg_source(f, a, ops[1].reg, window)
            return None
        return None

    def _fill_unreached(self, f):
        """Code between reached parts that nothing branches to (dead code after noreturn calls)."""
        covered = set()
        for a, i in f.insns.items():
            covered.update(range(a, a + i.size))
        for a in list(f.pool) + list(f.jt):
            covered.update(range(a, a + 4))
        if not covered:
            return
        top = f.end
        step = 2 if f.thumb else 4
        a = f.addr
        while a < top:
            if a in covered:
                a += 1
                continue
            # alignment padding before a pool / table
            if f.thumb and a % 4 == 2 and self.u16(a) == 0 and (a + 2) in covered and \
                    ((a + 2) in f.pool or (a + 2) in f.jt):
                a += 2
                continue
            insn = self.decode(f, a) if a % step == 0 else None
            if insn is None or any(x in covered for x in range(a, a + insn.size)):
                a += 1
                continue
            f.insns[a] = insn
            f.unreached.add(a)
            covered.update(range(a, a + insn.size))
            a += insn.size


# ---------------------------------------------------------------------------- output

def lab(a):
    return f".L{a:08X}"


class Writer:
    def __init__(self, dis, code_ranges):
        self.dis, self.syms = dis, dis.syms
        self.code_ranges = code_ranges
        self.data_starts = sorted(a for a, s in self.syms.items() if s.kind in ("data", "ram") and s.size)
        self.raw_texts = set()  # (thumb, text, encoding) that GNU as encodes differently

    def in_code(self, a):
        return any(s <= a < e for s, e in self.code_ranges)

    def symbolize(self, w, f=None):
        """Text for a pool word with value w: label inside the function, function, data symbol
        (+offset) or number."""
        if f is not None and f.addr <= w < f.end and w in f.labels:
            return lab(w)
        s = self.syms.get(w & ~1)
        if s is not None and s.is_func and self.in_code(s.addr) and (s.kind == "thumb") == bool(w & 1):
            return s.name
        s = self.syms.get(w)
        if s is not None and s.kind in ("data", "ram"):
            return s.name
        import bisect
        i = bisect.bisect_right(self.data_starts, w) - 1
        if i >= 0:
            s = self.syms[self.data_starts[i]]
            if s.addr < w < s.addr + s.size:
                return f"{s.name}+0x{w - s.addr:X}"
        return f"0x{w:08X}"

    def operand_text(self, f, insn):
        """Instruction text, or None when it has to be written as raw bytes."""
        mn, op = insn.mnemonic, insn.op_str
        ops = insn.operands
        pool = self.dis._pc_rel(f, insn)
        if pool is not None and pool in f.pool:
            return f"{mn} {op.split(',')[0]}, {lab(pool)}"
        if insn.id in (csarm.ARM_INS_B, csarm.ARM_INS_BL) and ops and ops[0].type == csarm.ARM_OP_IMM:
            t = ops[0].imm
            if t in f.labels and (insn.id == csarm.ARM_INS_B or t not in self.syms):
                return f"{mn} {lab(t)}"
            s = self.syms.get(t)
            if s is not None and s.is_func and self.in_code(t) and (s.kind == "thumb") == f.thumb:
                return f"{mn} {s.name}"  # (a branch into the other mode would get a linker veneer)
            return None  # target without a symbol in the code units
        if insn.id == csarm.ARM_INS_ADR:
            base = ((insn.address + 4) & ~3) if f.thumb else insn.address + 8
            t = ops[1].imm
            if f.addr <= t < f.end:
                return f"add {op.split(',')[0]}, pc, #0x{t - base:X}"
            return None
        if "pc" in op and insn.id not in (csarm.ARM_INS_MOV, csarm.ARM_INS_ADD, csarm.ARM_INS_BX,
                                          csarm.ARM_INS_POP, csarm.ARM_INS_PUSH, csarm.ARM_INS_LDM,
                                          csarm.ARM_INS_STM, csarm.ARM_INS_STMDB):
            return None  # other pc-relative forms
        text = f"{mn} {op}" if op else mn
        m = re.fullmatch(r"add (r\d), sp, \1", text) if f.thumb else None
        if m:
            text = f"add {m.group(1)}, sp"  # GNU as does not take the three-operand form
        enc = bytes(insn.bytes)
        if (f.thumb, text, enc) in self.raw_texts:
            return None
        return text

    def raw_insn(self, f, insn):
        if insn.size == 2:
            return f".2byte 0x{self.dis.u16(insn.address):04X}"
        if f.thumb:
            return f".2byte 0x{self.dis.u16(insn.address):04X}, 0x{self.dis.u16(insn.address + 2):04X}"
        return f".4byte 0x{self.dis.u32(insn.address):08X}"

    def plain_texts(self, f):
        """(thumb, text, encoding) of the instructions whose text has no label or symbol."""
        out = set()
        for a, insn in f.insns.items():
            if self.dis._pc_rel(f, insn) is not None or insn.id in (csarm.ARM_INS_B, csarm.ARM_INS_BL, csarm.ARM_INS_ADR):
                continue
            text = f"{insn.mnemonic} {insn.op_str}" if insn.op_str else insn.mnemonic
            out.add((f.thumb, text, bytes(insn.bytes)))
        return out

    def check_encodings(self, items, tmpdir):
        """Assemble every distinct instruction text once; remember those GNU as encodes differently."""
        import subprocess
        for thumb in (True, False):
            group = sorted({(t, e) for th, t, e in items if th == thumb})
            if not group:
                continue
            src = os.path.join(tmpdir, "enc.s")
            with open(src, "w") as fh:
                fh.write("\t.syntax unified\n\t" + (".thumb" if thumb else ".arm") + "\n")
                for text, _ in group:
                    fh.write(f"\t.balign 8, 0xEE\n\t{text}\n")
                fh.write("\t.balign 8, 0xEE\n")
            obj, binf = src[:-2] + ".o", src[:-2] + ".bin"
            r = subprocess.run([binutil("as"), "-mcpu=arm7tdmi", "-o", obj, src], capture_output=True, text=True)
            if r.returncode != 0:
                # texts GNU as rejects: find them one by one
                bad = set()
                for text, enc in group:
                    with open(src, "w") as fh:
                        fh.write("\t.syntax unified\n\t" + (".thumb" if thumb else ".arm") + f"\n\t{text}\n")
                    if subprocess.run([binutil("as"), "-mcpu=arm7tdmi", "-o", obj, src],
                                      capture_output=True).returncode != 0:
                        bad.add(text)
                for text, enc in group:
                    if text in bad:
                        self.raw_texts.add((thumb, text, enc))
                group = [(t, e) for t, e in group if t not in bad]
                with open(src, "w") as fh:
                    fh.write("\t.syntax unified\n\t" + (".thumb" if thumb else ".arm") + "\n")
                    for text, _ in group:
                        fh.write(f"\t.balign 8, 0xEE\n\t{text}\n")
                    fh.write("\t.balign 8, 0xEE\n")
                subprocess.run([binutil("as"), "-mcpu=arm7tdmi", "-o", obj, src], check=True)
            subprocess.run([binutil("objcopy"), "-O", "binary", "-j", ".text", obj, binf], check=True)
            out = open(binf, "rb").read()
            for i, (text, enc) in enumerate(group):
                slot = out[8 * i:8 * i + 8]
                if slot[:len(enc)] != enc or (len(enc) < 8 and slot[len(enc)] != 0xEE):
                    self.raw_texts.add((thumb, text, enc))

    def body(self, f):
        """Lines of the function body (without the function header/footer macros)."""
        dis = self.dis
        out = []
        a = f.addr
        while a < f.end:
            if a in f.labels and a != f.addr:
                out.append(f"{lab(a)}:")
            if a in f.insns:
                insn = f.insns[a]
                text = self.operand_text(f, insn)
                if text is None:
                    comment = f"{insn.mnemonic} {insn.op_str}".strip()
                    out.append(f"\t{self.raw_insn(f, insn)} @ {comment}")
                else:
                    out.append(f"\t{text}")
                a += insn.size
            elif a in f.pool:
                out.append(f"\t.4byte {self.symbolize(dis.u32(a), f)}")
                a += 4
            elif a in f.jt:
                w = f.jt[a]
                out.append(f"\t.4byte {lab(w & ~1)}" + ("+1" if w & 1 else ""))
                a += 4
            elif f.thumb and a % 4 == 2 and dis.u16(a) == 0 and (a + 2 in f.pool or a + 2 in f.jt):
                out.append("\t.align 2, 0")
                a += 2
            else:
                # bytes the walk did not reach: raw up to the next known item
                b = a + 1
                while b < f.end and b not in f.insns and b not in f.pool and b not in f.jt and b not in f.labels:
                    b += 1
                chunk = dis.rom[a - ROM_BASE:b - ROM_BASE]
                for i in range(0, len(chunk), 16):
                    out.append("\t.byte " + ", ".join(f"0x{x:02X}" for x in chunk[i:i + 16]))
                a = b
        return out


def unit_functions(syms, unit):
    start, end, _ = unit
    return [syms[a] for a in sorted(syms) if start <= a < end and syms[a].is_func]


def write_units(dis, units, which, force, outroot):
    import tempfile
    w = Writer(dis, [(s, e) for s, e, _ in units])
    todo = [u for u in units if not which or u[2] in which]
    funcs = []
    for u in todo:
        prev_end = u[0]
        for s in unit_functions(dis.syms, u):
            f = dis.analyze(s, limit=min(dis.next_symbol(s.addr), u[1]))
            funcs.append((u, f))
    items = set()
    for _, f in funcs:
        items |= w.plain_texts(f)
    with tempfile.TemporaryDirectory() as tmp:
        w.check_encodings(items, tmp)
    print(f"{len(funcs)} functions, {len(w.raw_texts)} instruction texts kept as raw bytes", file=sys.stderr)
    written = 0
    for u, f in funcs:
        d = os.path.join(outroot, u[2])
        os.makedirs(d, exist_ok=True)
        path = os.path.join(d, f.sym.name + ".s")
        if os.path.exists(path) and not force:
            continue
        with open(path, "w") as fh:
            fh.write("\n".join(w.body(f)) + "\n")
        written += 1
    print(f"wrote {written} files", file=sys.stderr)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--rom", default=os.path.join(ROOT, "baserom.gba"))
    ap.add_argument("--symbols", default=os.path.join(ROOT, "config/symbols.txt"))
    ap.add_argument("--units", default=os.path.join(ROOT, "config/units.txt"))
    ap.add_argument("--write", nargs="*", metavar="UNIT",
                    help="write asm/UNIT/FUNC.s for these units (all without names)")
    ap.add_argument("--force", action="store_true", help="overwrite existing asm files")
    ap.add_argument("--out", default=os.path.join(ROOT, "asm"))
    ap.add_argument("funcs", nargs="*", help="print these functions (names or addresses)")
    args = ap.parse_args()
    rom = open(args.rom, "rb").read()
    syms = load_symbols(args.symbols)
    dis = Disassembler(rom, syms)
    units = load_units(args.units) if os.path.exists(args.units) else []
    if args.write is not None:
        write_units(dis, units, set(args.write), args.force, args.out)
        return
    byname = {s.name: s for s in syms.values()}
    w = Writer(dis, [(s, e) for s, e, _ in units])
    for name in args.funcs:
        s = byname.get(name) or syms.get(int(name, 16))
        f = dis.analyze(s)
        print(f"@ {s.name} {s.addr:08X}..{f.end:08X} (limit {f.limit:08X})")
        for warn in f.warnings:
            print(f"@ warning: {warn}")
        print("\n".join(w.body(f)))


if __name__ == "__main__":
    main()
