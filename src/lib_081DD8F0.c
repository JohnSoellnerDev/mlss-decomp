/* agbcc: --old -fno-builtin */
/* newlib's _malloc_r (stdlib/mallocr.c built with DEFINE_MALLOC), built like the SDK's libc. */
#include "libc/config.h"
#define DEFINE_MALLOC
#include "libc/stdlib/mallocr.c"
