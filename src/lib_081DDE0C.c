/* agbcc: --old -fno-builtin */
/* newlib's memcpy (string/memcpy.c), built like the SDK's libc. */
#include "libc/config.h"
/* the game calls it by this name */
#define memcpy mem_copy_fast
#include "libc/string/memcpy.c"
