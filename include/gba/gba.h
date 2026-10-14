#ifndef GUARD_GBA_GBA_H
#define GUARD_GBA_GBA_H

#include "gba/types.h"
#include "gba/io_reg.h"

#define EWRAM_START 0x02000000
#define EWRAM_END   (EWRAM_START + 0x40000)
#define IWRAM_START 0x03000000
#define IWRAM_END   (IWRAM_START + 0x8000)
#define PLTT        0x05000000
#define BG_PLTT     PLTT
#define OBJ_PLTT    (PLTT + 0x200)
#define VRAM        0x06000000
#define OAM         0x07000000
#define ROM_START   0x08000000

#endif // GUARD_GBA_GBA_H
