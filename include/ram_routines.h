#ifndef GUARD_RAM_ROUTINES_H
#define GUARD_RAM_ROUTINES_H

// Routines that are copied from ROM into IWRAM at start-up and called through a pointer to
// the copy. The offset of a routine inside its block is taken from the ROM addresses.

// copy of the block starting with iwram_fill: +0 fill(value, dst, bytes), +0x7C copy(src, dst, bytes)
extern u8 *gMemRoutines;
extern u8 iwram_fill[];
extern u8 iwram_copy[];

// copy of the block starting with iwram_sqrt: +0 square root, +0x10C division
extern u8 *gMathRoutines;
extern u8 iwram_sqrt[];
extern u8 iwram_div[];
extern u8 iwram_math_08199424[];

#define MEM_ROUTINE(type, name) ((type)(gMemRoutines + ((u32)name - (u32)iwram_fill)))
#define MATH_ROUTINE(type, name) ((type)(gMathRoutines + ((u32)name - (u32)iwram_sqrt)))

// 320 entries: sine over 256 steps, the last 64 repeat the start so that cos(x) = gSineTable[x + 64]
extern const s16 gSineTable[];

#endif // GUARD_RAM_ROUTINES_H
