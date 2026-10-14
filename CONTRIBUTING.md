# Contributing: decompiling a function

Goal: C source that agbcc (the game's own compiler, GCC 2.95 for Thumb, `-O2
-mthumb-interwork`) turns into exactly the original machine code. Every function starts as
the original assembly in `asm/UNIT/FUNC.s`; you replace it with C in `src/UNIT.c` until the
compiled function is identical.

## Ground rules

1. **No game data in the repository.** Never commit the ROM, extracted graphics/music/text,
   binary blobs or hex dumps of data. Code (C, the original assembly in `asm/`), headers, tools
   and configuration only.
2. **The ROM stays identical.** After every change `tools/bin/ninja` has to print
   `mlss.gba: OK (fa2314c2…)`. If you touched anything in `asm/`, `tools/bin/ninja expected`
   has to be OK as well.
3. Keep changes focused: your unit's `src/` file, headers you need (add to them,
   do not reorganise), and symbol references in the `asm/` files of the functions you work on.

## One function, start to finish

```sh
python3 tools/new_c_unit.py code_080502DC      # once per unit: src/code_080502DC.c with all ASM_FUNC lines
# replace the function's ASM_FUNC line in src/code_080502DC.c with C
python3 tools/asmdiff.py obj_update_child     # repeat until it prints MATCH
tools/bin/ninja                               # mlss.gba: OK
```

`asmdiff.py` compiles your C (without the ASM_FUNC functions) and the original, and prints a
diff of the function's instructions (`-` original, `+` yours); offsets are relative to the
function start, calls show the relocation (the called symbol). A worked example is
`obj_update_child` in `src/code_080502DC.c`.

## Reading the original

- `asm/UNIT/FUNC.s`: Thumb in unified syntax. Labels `.L0805085C` carry the address. Literal
  pool words (`.4byte`) are written as symbols where the value is a function, ROM data
  (`gUnk_08…` or a named table) or RAM (`gUnk_02…`/`gUnk_03…`), otherwise as numbers.
- `config/symbols.txt` maps names to addresses; `grep -rn "bl FUNC$" asm/` finds the callers.

## Writing C that matches (agbcc / GCC 2.95 habits)

- Types: `u8 s8 u16 s16 u32 s32` (`include/gba/types.h`). A narrow parameter shows up as
  `lsls rX, rX, #0x18` + `lsrs`/`asrs rX, rX, #0x18` (or `#0x10`) at the start, a narrow return
  value the same way before the return. `lsrs` = unsigned, `asrs` = signed.
- `adds rX, rY, #0` is a plain register copy.
- Large constants and struct offsets are built as `movs r1, #0xE3; lsls r1, r1, #2` (= 0x38C)
  or loaded from the literal pool. Model memory as structs with filler arrays at exact offsets
  (`u8 filler0[0x22C];`), field names `unkXXX` until their meaning is clear.
- Bit fields: `lsls r2, r2, #10; lsrs r2, r2, #20` reads 12 bits starting at bit 10 of a u32
  (`u32 a : 10; u32 b : 12;` – GCC lays them out from bit 0 upwards).
- Calls through a function pointer go through `bl _call_via_rN`.
- Routines copied to IWRAM are called as `base + (routine - block start)` with three literal
  pool words (`gMathRoutines`, `iwram_div`, `iwram_sqrt`): use `MATH_ROUTINE(type, iwram_div)` /
  `MEM_ROUTINE(type, iwram_copy)` from `include/ram_routines.h`. The sine table is `gSineTable`
  (cosine = `gSineTable[x + 64]`).
- `bl __divsi3` / `__modsi3` / `__udivsi3` / `__umodsi3` are plain `/` and `%` in C (signed or
  unsigned operands); division by a power of two is inlined (`asrs` with a rounding fix-up).
- `switch` becomes `cmp rX, #N; bhi default; lsls; ldr table; adds; ldr; mov pc` followed by a
  `.4byte .Lcase…` table.
- Return a value only when callers use `r0` after the call.
- A loop counter that only counts down (`add rX, #-1` / `subs`, `cmp rX, #0`, `bge`) and is not
  used in the body is usually a counting-up `for (i = 0; i < N; i++)` that GCC reversed. Its
  start value N-1 can even be copied from another register that happens to hold the same
  constant (`mov r9, r10`).
- Register or instruction order differs? Try: reordering independent statements, adding or
  removing a temporary variable, signed vs. unsigned types, `if`/`else` arrangement,
  `for` vs. `while` vs. `do {} while`, splitting or merging expressions, `!x` vs. `x == 0`.
- No inline assembly, `register` variables, or `asm("")` tricks to force a match.
- If the C does the same thing but does not match after real effort, keep it as
  `NONMATCH("asm/UNIT/FUNC.s", decl) { … } END_NONMATCH` (the ROM keeps the original assembly,
  the progress report counts the C as partly matching) and say so in your pull request.
- `-Werror -Wimplicit`: every called function needs a prototype. If a prototype in a header
  differs from the `void f(void)` of an `ASM_FUNC` line, change the `ASM_FUNC` declaration.

## Symbols, names, headers

- Rename with `python3 tools/rename.py OLD NEW` (updates `config/symbols.txt`, `asm/`, `src/`,
  `include/`). Function names: snake_case. Global variables: `gCamelCase`. Unknown things keep
  `sub_ADDR` / `gUnk_ADDR`. Types: `CamelCase`; struct fields `unkXX` until known.
- RAM variables are not defined in C yet: declare them `extern`; the linker puts every symbol
  from `config/symbols.txt` that no object defines at its address. ROM data is still taken from
  the ROM: declare it `extern const` (e.g. `extern const u8 gUnk_0812345C[];`).
- The asm is the reference objdiff compares against. When your C refers to data through a
  different symbol than the asm (asm `.4byte gUnk_0300240C`, C `&gScene.unk200` → `gScene+0x200`),
  change the symbol text in the asm (never the bytes) so both agree, then check
  `tools/bin/ninja expected` is still OK.
- Shared structs and prototypes go into headers in `include/` (one per subsystem, e.g.
  `include/scene.h`); things only one unit uses can stay in its `.c` file.

## Units

`config/units.txt` cuts the code into units of about 6 KB named after their start address,
because the original source files are not known yet. When it becomes clear where an original
file starts and ends (functions sharing static helpers and data, a subsystem's beginning), the
unit gets split and renamed. Do not change `config/units.txt` or move functions between units
unless that is your task.

## Before you finish

1. `python3 tools/asmdiff.py FUNC` prints `MATCH` for every function you converted (or it is
   a documented `NONMATCH`).
2. `tools/bin/ninja` prints `mlss.gba: OK`.
3. Optional: `tools/bin/ninja report && python3 tools/progress.py`.

## Tools

| Tool | Purpose |
|---|---|
| `tools/setup.sh` | Installs agbcc, binutils, ninja and objdiff-cli into `tools/` |
| `configure.py` | Writes `build.ninja`, linker scripts, `objdiff.json` (runs again by itself when `config/` changes) |
| `tools/new_c_unit.py UNIT` | Creates `src/UNIT.c` with every function as `ASM_FUNC` |
| `tools/asmdiff.py FUNC` | Your C against the original, instruction by instruction |
| `tools/rename.py OLD NEW` | Renames a symbol everywhere |
| `tools/check_rom.py ROM` | SHA-1 check; on a mismatch names the first misplaced function and the first differing byte |
| `tools/progress.py` | Summary of `build/report.json` |
| `tools/disasm.py` | Produced `asm/` from the ROM (needs `.venv` with capstone). Only for re-splitting functions; it does not overwrite edited files without `--force` |
