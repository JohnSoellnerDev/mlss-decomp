/* agbcc: --old -fno-builtin */
/* newlib's _free_r and _malloc_trim_r (stdlib/mallocr.c built with DEFINE_FREE), built like the SDK's libc. */
#include "libc/config.h"
#define DEFINE_FREE
#include "libc/stdlib/mallocr.c"
