#ifndef GUARD_GLOBAL_H
#define GUARD_GLOBAL_H

#include "gba/gba.h"

#define NAKED __attribute__((naked))
#define ALIGNED(n) __attribute__((aligned(n)))
#define ARRAY_COUNT(a) (sizeof(a) / sizeof((a)[0]))

// A function that is not decompiled yet: its original assembly from asm/, unchanged.
// The objdiff report build (-DREPORT) leaves it out, so only real C counts as done.
#ifdef REPORT
#define ASM_FUNC(path, decl)
#else
#define ASM_FUNC(path, decl)                                                                \
    NAKED decl                                                                              \
    {                                                                                       \
        asm(".syntax unified\n"                                                             \
            ".include \"" path "\"\n"                                                       \
            ".syntax divided");                                                             \
    }
#endif

// C that does the same as the original but does not compile to identical code yet: the ROM
// gets the original assembly (the C sits behind `if (0)` and is dropped), NON_MATCHING and
// report builds compile the C.
//     NONMATCH("asm/UNIT/func.s", void func(int a))
//     {
//         ...
//     }
//     END_NONMATCH
#if defined(NON_MATCHING) || defined(REPORT)
#define NONMATCH(path, decl) decl
#define END_NONMATCH
#else
#define NONMATCH(path, decl)                                                                \
    NAKED decl                                                                              \
    {                                                                                       \
        asm(".syntax unified\n"                                                             \
            ".include \"" path "\"\n"                                                       \
            ".syntax divided");                                                             \
        if (0)
#define END_NONMATCH }
#endif

#endif // GUARD_GLOBAL_H
