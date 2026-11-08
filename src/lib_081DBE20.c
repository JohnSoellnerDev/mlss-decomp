/* agbcc: --old */
/* Double-precision soft float: libgcc's dp-bit.c, i.e. GCC's fp-bit.c built with these
   defines, as in the SDK's libgcc (the static helper gets a unique name here). */
#define FLOAT_BIT_ORDER_MISMATCH
#define FLOAT_WORD_ORDER_MISMATCH
#define _fpadd_parts _fpadd_parts_d
#include "libgcc/fp-bit.c"
