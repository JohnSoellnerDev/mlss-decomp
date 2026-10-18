# Contributing: decompiling a function

Goal: C source that agbcc (the game's own compiler, GCC 2.95 for Thumb, `-O2
-mthumb-interwork`) turns into exactly the original machine code. Every function starts as
the original assembly in `asm/UNIT/FUNC.s`; you replace it with C in `src/UNIT.c` until the
compiled function is identical.

## Ground rules

1. **Nothing of the game in the repository.** Never commit the ROM, extracted
   graphics/music/text, binary blobs, hex dumps of data or the original assembly. `asm/` is
   generated from your ROM by `configure.py` and ignored by git (only `asm/macros.inc` is
   tracked); the server rejects pushes that contain any of it. C, headers, tools and
   configuration only.
2. **The ROM stays identical.** After every change `tools/bin/ninja` has to print
   `mlss.gba: OK (fa2314c2…)`. If you changed anything in `config/`, `tools/bin/ninja expected`
   has to be OK as well.
3. Keep changes focused: your unit's `src/` file, headers you need (add to them,
   do not reorganise), and the `config/symbols.txt` entries your functions need.

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

- `asm/UNIT/FUNC.s` (generated, see above): Thumb in unified syntax. Labels `.L0805085C` carry
  the address. Literal pool words (`.4byte`) are written as symbols where the value is a
  function, ROM data (`gUnk_08…` or a named table) or RAM (`gUnk_02…`/`gUnk_03…`), otherwise
  as numbers.
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
- `bl __floatsidf` / `__adddf3` / `__muldf3` / `__fixdfsi` … are `double` arithmetic in C
  (`(s32)(x * 1.5)`), `__lshrdi3` / `__muldi3` are 64-bit (`long long`) shifts and multiplies.
  agbcc keeps doubles high word first, like the original.
- Routines copied to IWRAM are called as `base + (routine - block start)` with three literal
  pool words (`gMathRoutines`, `iwram_div`, `iwram_sqrt`): use `MATH_ROUTINE(type, iwram_div)` /
  `MEM_ROUTINE(type, iwram_copy)` from `include/ram_routines.h`. The sine and cosine tables are
  `gSineTable` and `gCosineTable` (two symbols, two literal pool words).
- `bl __divsi3` / `__modsi3` / `__udivsi3` / `__umodsi3` are plain `/` and `%` in C (signed or
  unsigned operands); division by a power of two is inlined (`asrs` with a rounding fix-up).
- `switch` becomes `cmp rX, #N; bhi default; lsls; ldr table; adds; ldr; mov pc` followed by a
  `.4byte .Lcase…` table.
- Return a value only when callers use `r0` after the call.
- A loop counter that only counts down (`add rX, #-1` / `subs`, `cmp rX, #0`, `bge`) and is not
  used in the body is usually a counting-up `for (i = 0; i < N; i++)` that GCC reversed. Its
  start value N-1 can even be copied from another register that happens to hold the same
  constant (`mov r9, r10`). When the start value is loaded from a stack slot that also holds a
  flag (`movs r0, #0; str r0, [sp, #0x1c]`, a call, then `ldr r0, [sp, #0x1c]; mov sl, r0`
  before the loop), the flag is a plain local that did not get a register (`u32 saved;`, not a
  struct field) and the loop is `for (count = 0; count < 1; count++)`: the reversed loop takes
  its start value from the flag (example `obj_bounce_point_3fb0` in src/code_08182EB8.c).
- A signed division by 256 (`cmp; bge; adds #0xFF; asrs #8`): try both `x / 0x100` in the
  expression and `t = x; if (t < 0) t += 0xFF; t >>= 8;`. They differ in the temporaries GCC
  creates, and each form is the right one in some functions.
- The original computes a field address into r0 and copies it into a callee-saved register
  only after the first load (`adds r0, r4, #0; adds r0, #0xAC; ldrsh …; adds r5, r0, #0`), but
  your C puts it straight into r5: write `obj->field` at every use instead of keeping a
  pointer variable; GCC then shares the address itself. `ldrsh`/`ldrsb` mean the field is
  signed (`s16`/`s8`).
- Reading a bit field through a struct pointer compiles to a word `ldr`; when the original
  uses `ldrb` + `lsls`/`lsrs`, read the byte and shift (`((u32)x << 25) >> 31`).
- Clearing or setting bits of a byte keeps the full int constant in the original
  (`ldrb; movs r0, #9; rsbs r0, r0, #0; ands r0, r2; strb` = and with -9). A plain
  `p->flags &= ~8` on a `u8` gets the constant narrowed and does not match. Write a bit-field
  assignment: `((struct FlagBits *)&p->flags)->bit3 = 0;`, and for `(x & -7) | 2` (bits 1-2 set to
  01) `((struct ModeBits *)&p->flags)->mode = 1;`. This is the first thing to try: an int
  temporary (`int b = p->flags; b = -7 & b; b |= 2; p->flags = b;`) usually loads the byte into
  the other register. Two masks in a row on one byte are usually two bit-field assignments as
  well (`bits->b23 = 0; bits->lo2 = 0;` for `& -13; & -4`, example
  `script_init_obj_setup_sprite` in src/code_080502DC.c); only sometimes do they need the int
  temporary (`b = -0x41 & b; b = -0x21 & b;`). Halfwords work the same with `u16` bit fields
  (`obj_clear_mask_if_timer`). Examples: src/code_080A048C.c, src/code_08092784.c.
- Several stores through one pointer (`*p = a; p++; *p = b;`) become `stmia`; when the original
  uses `str` and `adds`, compute each address in its own variable (`figure_pair_step` in
  src/code_080FD8D8.c).
- A teardown that sets a class pointer, calls a child, then sets the base class pointer again
  and frees the object: the second half is an inlined helper. Written directly, GCC keeps the
  field address in a register across the call (`obj_teardown` in src/code_080502DC.c).
- A halfword/byte field is tested sign-extended (`ldrsh rX` + `cmp`) and later re-read with
  `ldrh`/`ldrb` for an update, but your C loads the `ldrh` first: read the field into an `s32`
  local (`s32 t = obj->timer; if (t != 0) obj->timer--;`). An `s16` local or a direct
  `if (obj->timer)` keeps the narrow value and reuses it.
- A multiply by a constant that sits in the destination register (`movs r0, #0xD0; muls r0, r1`)
  is usually indexing an array of structs (`&gWork->entries[i]` with a 0xD0-byte struct), not
  `base + i * 0xD0` written by hand.
- A constant stored to a field with a large offset: the original loads the constant first
  (`movs r1, #0x28` before `adds r2, r0, #0; adds r2, #0x92; strh r1, [r2]`) when the value went
  through a local (`u16 v = 0x28; body->height = v;`); a direct `body->height = 0x28` computes
  the address first.
- ROM tables that the original reads again after a call: declare them without `const`
  (`extern s8 gJitterRange[];`); with `const` GCC keeps the first loaded value across the call.
- Fixed-point conversions inside call arguments: write `f(x / 0x100, …)` directly; separate
  `if (x < 0) x += 0xFF; x >>= 8;` statements change the order of the stack-argument stores.
- `movs r0, #0x41; rsbs r0, r0, #0` is the constant `~0x40` (clears bit 6 only).
- Register or instruction order differs? Try: reordering independent statements, adding or
  removing a temporary variable, signed vs. unsigned types, `if`/`else` arrangement,
  `for` vs. `while` vs. `do {} while`, splitting or merging expressions, `!x` vs. `x == 0`.
- Flag tests on one byte (`ldrb r1; movs r0, #0x38; ands r0, r1; …; movs r0, #6; ands r0, r1;
  cmp r0, #2`) where the constant register receives the result are bit-field compares:
  `bits->kind == 0`, `bits->mode == 1` through a bit-field struct over the byte
  (`u8 unk0 : 1; u8 mode : 2; u8 kind : 3;`). A `u8 f` local with `(f & 6) == 2` reuses f's
  register instead (`ands r1, r0`).
- Call arguments are normally loaded in order, except that GCC computes an "expensive" constant
  such as `-1` (`movs r2, #1; rsbs r2, r2, #0`) first. When the original loads `-1` in its
  argument position, the call sits in an inlined helper: a `static inline` function that the
  callers use (example `actor_end_action` in src/code_0808C6F0.c). Repeated identical blocks in
  several functions are a hint for such helpers.
- A one-bit field set from a value (`movs r0, #1; ands r1, r0; lsls r1, r1, #6`, then
  `movs r0, #0x41; rsbs r0, r0, #0` for the mask) where your C gets `subs r0, #0x42` instead
  (GCC reuses the 1 still in r0): the original sets the bit in an inlined helper,
  `static inline void set_bit6(u8 *spr, u32 v) { ((struct Bits *)(spr + 0x11))->b6 = v; }`
  (example in src/code_080FA598.c).
- Copying a bit from one object to another (`ldrb` of the source byte, `lsls #25; lsrs #31`,
  then the destination address, `ands` with 1, `lsls #6`, merge): the bit fields have to sit at
  their real offset in a struct over the whole object, not in a struct cast onto the byte, or
  GCC computes the destination address first. Write
  `((struct ObjFlags77 *)child)->bit6 = ((struct ObjFlags77 *)obj)->bit6;` with
  `struct ObjFlags77 { u8 filler0[0x77]; u8 lo5 : 5; u8 bit5 : 1; u8 bit6 : 1; u8 bit7 : 1; };`
  (example `obj_child_flags` in src/code_08065718.c).
- A bit set from a comparison (`ldr r3, [body, #8]; movs r2, #0; adds r0, body, #0x84;
  ldrsh r1, [r0, r6=0]; ldrsh r0, [r0, r6=2]; cmp; blt; movs r2, #1; lsls r2, r2, #6; …merge`):
  assign the comparison straight to the bit field and take the address of the compared pair
  inside the comparison, so it is computed after the `movs r2, #0`:
  `bits->mirror = (span = &body->spanA)[0] >= span[1];` (example `hero_a_go_to_partner` in
  src/code_080A1DC8.c). A `mirror` variable or a separate pointer statement moves code around.
- A stack frame with an area nothing in the function touches (`sub sp, #0x10` and no access to
  those bytes) is an unused local array or struct in the original: GCC 2.95 reserves space for
  arrays and structs even when they are never used. Declare it (`s32 unused[4];`).
- `ldr r0, =gSys; ldrh r1, [r0, #0x2a]` (symbol in the pool, offset in the load) is a struct
  member: `((struct SysKeys2A *)gSys)->keys`. Pointer arithmetic on the array
  (`*(u16 *)(gSys + 0x2A)`) folds the offset into the pool word (`.4byte gSys+0x2A`, load at
  `#0`). Example `textbox_update` in src/code_0805335C.c.
- The game's source files were compiled with `-funroll-loops` (tools/agbcc.py adds it for every
  `code_*` unit; the sound driver and the libraries were built without it). Loops therefore show
  up unrolled: four copies of the body with a jump into the middle (`movs r0, #3; bics r0, r1`,
  then `cmp r0, #3; bge`, `cmp r0, #2; bge`), or a constant count split into rounds (twelve
  iterations as four rounds of three bodies, fifteen as three rounds of five). Write the plain
  loop the programmer wrote, one body with step 1: `i = n; while (--i >= 0) p[i] = 0;`
  (`clear_map_rect` in src/code_08084DB4.c), `for (i = 0; i < 15; i++) sum += e[i].v;`
  (`avg_fields_sdiv15_a` in src/code_08167E7C.c). Never unroll by hand (copies of the body,
  `i += 2`, Duff's device, `goto` into the copies). A loop the original keeps as one plain body
  has a counter GCC cannot analyse: a `u8` or `u16` counter is never unrolled (`for (i = 0; i < 6;
  i++)` with `s32 i` comes out as six copies, with `u8 i` as one loop). A unit made from a file
  built without the option can switch it off with the first line `/* agbcc: -fno-unroll-loops */`.
- A constant the original loads from the pool (`ldr r1, =0x5000; subs`) while yours folds it into
  a neighbouring subtraction: compute it in its own statement (`startX = centerX - 0x5000;`).
  A mask the original keeps in a high register across a loop (`mov r8, rX` before the loop,
  then `ands` with r8) is an `s32` local set before the loop (`mask = ~0x40;`); `&= ~0x40` on a
  `u8` narrows to `0xBF` instead (examples in src/code_08098B18.c).
- Table reads the original adds as `lsls r0, r0, #1; adds r0, r0, rBase` (index first) are
  array indexing, `((u16 *)(ai + 0xD0))[idx]`; hand-written `*(u16 *)(ai + (idx << 1) + 0xD0)`
  swaps the operands of the add (example `enemy_wait_sense` in src/code_08145D54.c).
- Stack slots follow the declaration scopes: a local array declared in an inner `{ }` block is
  placed after the outer locals (moves a byte local from `sp+0x14` to `sp+4`; example
  `enemy_contact_check_state` in src/code_08142A38.c).
- A signed halfword the original keeps as `ldrsh` while yours becomes `ldrh` + shifts: assign it to
  an `s32` temporary first (`v = obj->unk1D4;` with `s32 v`).
- A range test as two compares against the outer bounds (`cmp r0, #0xe; bgt else; cmp r0, #0xd;
  blt else`) with the case block first is a `switch` with `case 0xD: case 0xE:` and a `default`;
  an `if` chain gives `cmp #0xc; bgt` instead (example `partner_step_wait` in
  src/code_0809583C.c).
- No inline assembly, `register` variables, `asm("")` tricks, `volatile` or dummy variables that
  only exist to force a register choice or instruction order.
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
- The asm is the reference objdiff compares against, and it is written from
  `config/symbols.txt`: edits in `asm/` are overwritten. When your C refers to data through a
  different symbol than the asm (asm `.4byte gUnk_0300240C`, C `&gScene.unk200` → `gScene+0x200`),
  change the symbols instead: give the struct its size (`03002200 gScene ram size=…`) and
  remove the symbol in between; pool words inside a sized symbol are written as `NAME+0xOFF`.
  Then `python3 configure.py` (rewrites `asm/`) and check `tools/bin/ninja expected` is OK.
- Shared structs and prototypes go into headers in `include/` (one per subsystem, e.g.
  `include/scene.h`); things only one unit uses can stay in its `.c` file.

## Units

`config/units.txt` cuts the code into units of about 6 KB named after their start address,
because the original source files are not known yet. When it becomes clear where an original
file starts and ends (functions sharing static helpers and data, a subsystem's beginning), the
unit gets split and renamed. Until then, a long run of finished functions inside a unit is cut
out as its own unit (`tools/split_unit.py UNIT --auto 0x400`), so that it counts as complete.
Do not change `config/units.txt` or move functions between units unless that is your task.

## Before you finish

1. `python3 tools/asmdiff.py FUNC` prints `MATCH` for every function you converted (or it is
   a documented `NONMATCH`).
2. `tools/bin/ninja` prints `mlss.gba: OK`.
3. Optional: `tools/bin/ninja report && python3 tools/progress.py`.

## Tools

| Tool | Purpose |
|---|---|
| `tools/setup.sh` | Installs agbcc, binutils, ninja, objdiff-cli and capstone (for the disassembler) into `tools/` |
| `configure.py` | Generates `asm/` from `baserom.gba` and writes `build.ninja`, linker scripts, `objdiff.json` (runs again by itself when `config/` changes) |
| `tools/new_c_unit.py UNIT` | Creates `src/UNIT.c` with every function as `ASM_FUNC` |
| `tools/asmdiff.py FUNC` | Your C against the original, instruction by instruction |
| `tools/rename.py OLD NEW` | Renames a symbol everywhere |
| `tools/split_unit.py UNIT ADDR…` | Splits a unit (and its C file) at function addresses; `--auto MIN` cuts out runs of finished functions |
| `tools/check_rom.py ROM` | SHA-1 check; on a mismatch names the first misplaced function and the first differing byte |
| `tools/progress.py` | Summary of `build/report.json` |
| `tools/progress_svg.py` | Draws the README progress map and badges from `build/report.json` (into `build/progress/`) |
| `tools/disasm.py` | The disassembler behind `asm/` (`configure.py` runs it); `tools/disasm.py FUNC` prints one function |
