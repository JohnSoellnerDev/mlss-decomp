/* agbcc: --old */
/* Single-precision soft float: libgcc's fp-bit.c, i.e. GCC's fp-bit.c built with these
   defines, as in the SDK's libgcc (the static helper gets a unique name here). */
#define FLOAT
#define FLOAT_BIT_ORDER_MISMATCH
#define _fpadd_parts _fpadd_parts_f
#include "libgcc/fp-bit.c"
