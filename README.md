# Mario & Luigi: Superstar Saga (GBA) – decompilation

A matching decompilation of *Mario & Luigi: Superstar Saga* for the Game Boy Advance, European
release (`A88P`, English/French/German/Spanish/Italian). The C code here compiles, with the
same compiler the game was built with (agbcc, from Nintendo's GBA SDK), to a ROM that is
byte-for-byte identical to the original.

This repository contains **nothing of the game**: no ROM, no graphics, music or text, and none of
its code. The original assembly that the C is compared against is generated from your own copy
of the game when you configure the build.

| ROM | SHA-1 |
|---|---|
| `baserom.gba` (Mario & Luigi - Superstar Saga (Europe) (En,Fr,De,Es,It)) | `fa2314c2fbe0db1ab17175f8be7cceb0ab084efc` |

## Building

Linux (x86-64) with a C compiler, `make`, `curl`, `git`, `xz`, `unzip` and Python 3.

```sh
tools/setup.sh                 # agbcc, binutils, ninja, objdiff, capstone into tools/ (a few minutes, once)
cp /path/to/your/rom.gba baserom.gba
python3 configure.py           # the first time also disassembles the ROM into asm/ (about a minute)
tools/bin/ninja                # builds mlss.gba and checks it: "mlss.gba: OK (fa2314c2…)"
```

Other targets: `tools/bin/ninja expected` links the original assembly alone (also has to be
OK), `tools/bin/ninja report` writes the progress report `build/report.json`,
`tools/progress.py` summarises it.

## How it is put together

| Path | Contents |
|---|---|
| `src/UNIT.c` | Decompiled C. Functions that are not decompiled yet are pulled in from `asm/` with `ASM_FUNC`. |
| `asm/UNIT/FUNC.s` | The original assembly of every function, one file each: the target the C is compared against. Generated from `baserom.gba` by `configure.py` and not part of the repository. |
| `include/` | Headers (`global.h`, GBA hardware in `gba/`). |
| `config/units.txt` | The code units (one object file / source file each) with their ROM ranges. Everything between them is data and still comes from `baserom.gba`. |
| `config/symbols.txt` | Names of functions, data and RAM variables with their addresses. |
| `configure.py` | Writes `build.ninja`, the linker script and `objdiff.json`, and `asm/` when `config/` changed. |
| `tools/` | Toolchain setup, compiler wrapper, ROM check, `asmdiff.py`, `rename.py`, `new_c_unit.py`, the disassembler that generates `asm/`. |

The units are named after their start address (`code_08017A00`) for now: the game's real source
files are not known yet, so the code is cut into pieces of about 6 KB. When the original file
boundaries are found, units get split and renamed.

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for the workflow:
pick a function in `asm/`, write C in `src/` until `tools/asmdiff.py FUNC` says `MATCH`, then
check that the ROM is still identical.

Progress is reported with [objdiff](https://github.com/encounter/objdiff) to
[decomp.dev](https://decomp.dev).

## Thanks

[pret](https://github.com/pret) for agbcc and the conventions this project follows, encounter
for objdiff and decomp.dev.
