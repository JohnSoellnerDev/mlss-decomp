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

// The BIOS's interrupt check flags: IRQ handlers set the IRQ_* bit of the interrupt they served,
// and VBlankIntrWait / IntrWait wait for it.
#define INTR_CHECK (*(u16 *)(IWRAM_END - 8))

#define SCREEN_WIDTH  240
#define SCREEN_HEIGHT 160

// EWRAM begins with a copy of the I/O registers (same offsets as REG_BASE) that the game
// writes to the hardware once a frame; code changes the copy, not the registers.
#define REG_BUF_DISPCNT  (*(u16 *)(EWRAM_START + 0x000))
#define REG_BUF_BG2CNT   (*(u16 *)(EWRAM_START + 0x00C))
#define REG_BUF_BG3CNT   (*(u16 *)(EWRAM_START + 0x00E))
#define REG_BUF_WININ    (*(u16 *)(EWRAM_START + 0x048))
#define REG_BUF_WINOUT   (*(u16 *)(EWRAM_START + 0x04A))
#define REG_BUF_BLDCNT   (*(u16 *)(EWRAM_START + 0x050))
#define REG_BUF_BLDALPHA (*(u16 *)(EWRAM_START + 0x052))

#endif // GUARD_GBA_GBA_H
