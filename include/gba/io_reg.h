#ifndef GUARD_GBA_IO_REG_H
#define GUARD_GBA_IO_REG_H

// GBA I/O registers (GBATEK names). REG_ADDR_X is the address, REG_X the register.

#define REG_BASE 0x4000000

#define REG_ADDR_DISPCNT      (REG_BASE + 0x000)
#define REG_ADDR_DISPSTAT     (REG_BASE + 0x004)
#define REG_ADDR_VCOUNT       (REG_BASE + 0x006)
#define REG_ADDR_BG0CNT       (REG_BASE + 0x008)
#define REG_ADDR_BG1CNT       (REG_BASE + 0x00A)
#define REG_ADDR_BG2CNT       (REG_BASE + 0x00C)
#define REG_ADDR_BG3CNT       (REG_BASE + 0x00E)
#define REG_ADDR_BG0HOFS      (REG_BASE + 0x010)
#define REG_ADDR_BG0VOFS      (REG_BASE + 0x012)
#define REG_ADDR_BG1HOFS      (REG_BASE + 0x014)
#define REG_ADDR_BG1VOFS      (REG_BASE + 0x016)
#define REG_ADDR_BG2HOFS      (REG_BASE + 0x018)
#define REG_ADDR_BG2VOFS      (REG_BASE + 0x01A)
#define REG_ADDR_BG3HOFS      (REG_BASE + 0x01C)
#define REG_ADDR_BG3VOFS      (REG_BASE + 0x01E)
#define REG_ADDR_BG2PA        (REG_BASE + 0x020)
#define REG_ADDR_BG2PB        (REG_BASE + 0x022)
#define REG_ADDR_BG2PC        (REG_BASE + 0x024)
#define REG_ADDR_BG2PD        (REG_BASE + 0x026)
#define REG_ADDR_BG2X         (REG_BASE + 0x028)
#define REG_ADDR_BG2Y         (REG_BASE + 0x02C)
#define REG_ADDR_BG3PA        (REG_BASE + 0x030)
#define REG_ADDR_BG3PB        (REG_BASE + 0x032)
#define REG_ADDR_BG3PC        (REG_BASE + 0x034)
#define REG_ADDR_BG3PD        (REG_BASE + 0x036)
#define REG_ADDR_BG3X         (REG_BASE + 0x038)
#define REG_ADDR_BG3Y         (REG_BASE + 0x03C)
#define REG_ADDR_WIN0H        (REG_BASE + 0x040)
#define REG_ADDR_WIN1H        (REG_BASE + 0x042)
#define REG_ADDR_WIN0V        (REG_BASE + 0x044)
#define REG_ADDR_WIN1V        (REG_BASE + 0x046)
#define REG_ADDR_WININ        (REG_BASE + 0x048)
#define REG_ADDR_WINOUT       (REG_BASE + 0x04A)
#define REG_ADDR_MOSAIC       (REG_BASE + 0x04C)
#define REG_ADDR_BLDCNT       (REG_BASE + 0x050)
#define REG_ADDR_BLDALPHA     (REG_BASE + 0x052)
#define REG_ADDR_BLDY         (REG_BASE + 0x054)
#define REG_ADDR_SOUND1CNT_L  (REG_BASE + 0x060)
#define REG_ADDR_SOUND1CNT_H  (REG_BASE + 0x062)
#define REG_ADDR_SOUND1CNT_X  (REG_BASE + 0x064)
#define REG_ADDR_SOUND2CNT_L  (REG_BASE + 0x068)
#define REG_ADDR_SOUND2CNT_H  (REG_BASE + 0x06C)
#define REG_ADDR_SOUND3CNT_L  (REG_BASE + 0x070)
#define REG_ADDR_SOUND3CNT_H  (REG_BASE + 0x072)
#define REG_ADDR_SOUND3CNT_X  (REG_BASE + 0x074)
#define REG_ADDR_SOUND4CNT_L  (REG_BASE + 0x078)
#define REG_ADDR_SOUND4CNT_H  (REG_BASE + 0x07C)
#define REG_ADDR_SOUNDCNT_L   (REG_BASE + 0x080)
#define REG_ADDR_SOUNDCNT_H   (REG_BASE + 0x082)
#define REG_ADDR_SOUNDCNT_X   (REG_BASE + 0x084)
#define REG_ADDR_SOUNDBIAS    (REG_BASE + 0x088)
#define REG_ADDR_WAVE_RAM     (REG_BASE + 0x090)
#define REG_ADDR_FIFO_A       (REG_BASE + 0x0A0)
#define REG_ADDR_FIFO_B       (REG_BASE + 0x0A4)
#define REG_ADDR_DMA0SAD      (REG_BASE + 0x0B0)
#define REG_ADDR_DMA0DAD      (REG_BASE + 0x0B4)
#define REG_ADDR_DMA0CNT      (REG_BASE + 0x0B8)
#define REG_ADDR_DMA0CNT_L    (REG_BASE + 0x0B8)
#define REG_ADDR_DMA0CNT_H    (REG_BASE + 0x0BA)
#define REG_ADDR_DMA1SAD      (REG_BASE + 0x0BC)
#define REG_ADDR_DMA1DAD      (REG_BASE + 0x0C0)
#define REG_ADDR_DMA1CNT      (REG_BASE + 0x0C4)
#define REG_ADDR_DMA1CNT_L    (REG_BASE + 0x0C4)
#define REG_ADDR_DMA1CNT_H    (REG_BASE + 0x0C6)
#define REG_ADDR_DMA2SAD      (REG_BASE + 0x0C8)
#define REG_ADDR_DMA2DAD      (REG_BASE + 0x0CC)
#define REG_ADDR_DMA2CNT      (REG_BASE + 0x0D0)
#define REG_ADDR_DMA2CNT_L    (REG_BASE + 0x0D0)
#define REG_ADDR_DMA2CNT_H    (REG_BASE + 0x0D2)
#define REG_ADDR_DMA3SAD      (REG_BASE + 0x0D4)
#define REG_ADDR_DMA3DAD      (REG_BASE + 0x0D8)
#define REG_ADDR_DMA3CNT      (REG_BASE + 0x0DC)
#define REG_ADDR_DMA3CNT_L    (REG_BASE + 0x0DC)
#define REG_ADDR_DMA3CNT_H    (REG_BASE + 0x0DE)
#define REG_ADDR_TM0CNT       (REG_BASE + 0x100)
#define REG_ADDR_TM0CNT_L     (REG_BASE + 0x100)
#define REG_ADDR_TM0CNT_H     (REG_BASE + 0x102)
#define REG_ADDR_TM1CNT       (REG_BASE + 0x104)
#define REG_ADDR_TM1CNT_L     (REG_BASE + 0x104)
#define REG_ADDR_TM1CNT_H     (REG_BASE + 0x106)
#define REG_ADDR_TM2CNT       (REG_BASE + 0x108)
#define REG_ADDR_TM2CNT_L     (REG_BASE + 0x108)
#define REG_ADDR_TM2CNT_H     (REG_BASE + 0x10A)
#define REG_ADDR_TM3CNT       (REG_BASE + 0x10C)
#define REG_ADDR_TM3CNT_L     (REG_BASE + 0x10C)
#define REG_ADDR_TM3CNT_H     (REG_BASE + 0x10E)
#define REG_ADDR_SIOCNT       (REG_BASE + 0x128)
#define REG_ADDR_SIODATA8     (REG_BASE + 0x12A)
#define REG_ADDR_SIODATA32    (REG_BASE + 0x120)
#define REG_ADDR_SIOMULTI0    (REG_BASE + 0x120)
#define REG_ADDR_SIOMULTI1    (REG_BASE + 0x122)
#define REG_ADDR_SIOMULTI2    (REG_BASE + 0x124)
#define REG_ADDR_SIOMULTI3    (REG_BASE + 0x126)
#define REG_ADDR_SIOMLT_SEND  (REG_BASE + 0x12A)
#define REG_ADDR_KEYINPUT     (REG_BASE + 0x130)
#define REG_ADDR_KEYCNT       (REG_BASE + 0x132)
#define REG_ADDR_RCNT         (REG_BASE + 0x134)
#define REG_ADDR_JOYCNT       (REG_BASE + 0x140)
#define REG_ADDR_JOY_RECV     (REG_BASE + 0x150)
#define REG_ADDR_JOY_TRANS    (REG_BASE + 0x154)
#define REG_ADDR_JOYSTAT      (REG_BASE + 0x158)
#define REG_ADDR_IE           (REG_BASE + 0x200)
#define REG_ADDR_IF           (REG_BASE + 0x202)
#define REG_ADDR_WAITCNT      (REG_BASE + 0x204)
#define REG_ADDR_IME          (REG_BASE + 0x208)

#define REG_DISPCNT      (*(vu16 *)REG_ADDR_DISPCNT)
#define REG_DISPSTAT     (*(vu16 *)REG_ADDR_DISPSTAT)
#define REG_VCOUNT       (*(vu16 *)REG_ADDR_VCOUNT)
#define REG_BG0CNT       (*(vu16 *)REG_ADDR_BG0CNT)
#define REG_BG1CNT       (*(vu16 *)REG_ADDR_BG1CNT)
#define REG_BG2CNT       (*(vu16 *)REG_ADDR_BG2CNT)
#define REG_BG3CNT       (*(vu16 *)REG_ADDR_BG3CNT)
#define REG_BG0HOFS      (*(vu16 *)REG_ADDR_BG0HOFS)
#define REG_BG0VOFS      (*(vu16 *)REG_ADDR_BG0VOFS)
#define REG_BG1HOFS      (*(vu16 *)REG_ADDR_BG1HOFS)
#define REG_BG1VOFS      (*(vu16 *)REG_ADDR_BG1VOFS)
#define REG_BG2HOFS      (*(vu16 *)REG_ADDR_BG2HOFS)
#define REG_BG2VOFS      (*(vu16 *)REG_ADDR_BG2VOFS)
#define REG_BG3HOFS      (*(vu16 *)REG_ADDR_BG3HOFS)
#define REG_BG3VOFS      (*(vu16 *)REG_ADDR_BG3VOFS)
#define REG_BG2PA        (*(vu16 *)REG_ADDR_BG2PA)
#define REG_BG2PB        (*(vu16 *)REG_ADDR_BG2PB)
#define REG_BG2PC        (*(vu16 *)REG_ADDR_BG2PC)
#define REG_BG2PD        (*(vu16 *)REG_ADDR_BG2PD)
#define REG_BG2X         (*(vu32 *)REG_ADDR_BG2X)
#define REG_BG2Y         (*(vu32 *)REG_ADDR_BG2Y)
#define REG_BG3PA        (*(vu16 *)REG_ADDR_BG3PA)
#define REG_BG3PB        (*(vu16 *)REG_ADDR_BG3PB)
#define REG_BG3PC        (*(vu16 *)REG_ADDR_BG3PC)
#define REG_BG3PD        (*(vu16 *)REG_ADDR_BG3PD)
#define REG_BG3X         (*(vu32 *)REG_ADDR_BG3X)
#define REG_BG3Y         (*(vu32 *)REG_ADDR_BG3Y)
#define REG_WIN0H        (*(vu16 *)REG_ADDR_WIN0H)
#define REG_WIN1H        (*(vu16 *)REG_ADDR_WIN1H)
#define REG_WIN0V        (*(vu16 *)REG_ADDR_WIN0V)
#define REG_WIN1V        (*(vu16 *)REG_ADDR_WIN1V)
#define REG_WININ        (*(vu16 *)REG_ADDR_WININ)
#define REG_WINOUT       (*(vu16 *)REG_ADDR_WINOUT)
#define REG_MOSAIC       (*(vu16 *)REG_ADDR_MOSAIC)
#define REG_BLDCNT       (*(vu16 *)REG_ADDR_BLDCNT)
#define REG_BLDALPHA     (*(vu16 *)REG_ADDR_BLDALPHA)
#define REG_BLDY         (*(vu16 *)REG_ADDR_BLDY)
#define REG_SOUND1CNT_L  (*(vu16 *)REG_ADDR_SOUND1CNT_L)
#define REG_SOUND1CNT_H  (*(vu16 *)REG_ADDR_SOUND1CNT_H)
#define REG_SOUND1CNT_X  (*(vu16 *)REG_ADDR_SOUND1CNT_X)
#define REG_SOUND2CNT_L  (*(vu16 *)REG_ADDR_SOUND2CNT_L)
#define REG_SOUND2CNT_H  (*(vu16 *)REG_ADDR_SOUND2CNT_H)
#define REG_SOUND3CNT_L  (*(vu16 *)REG_ADDR_SOUND3CNT_L)
#define REG_SOUND3CNT_H  (*(vu16 *)REG_ADDR_SOUND3CNT_H)
#define REG_SOUND3CNT_X  (*(vu16 *)REG_ADDR_SOUND3CNT_X)
#define REG_SOUND4CNT_L  (*(vu16 *)REG_ADDR_SOUND4CNT_L)
#define REG_SOUND4CNT_H  (*(vu16 *)REG_ADDR_SOUND4CNT_H)
#define REG_SOUNDCNT_L   (*(vu16 *)REG_ADDR_SOUNDCNT_L)
#define REG_SOUNDCNT_H   (*(vu16 *)REG_ADDR_SOUNDCNT_H)
#define REG_SOUNDCNT_X   (*(vu16 *)REG_ADDR_SOUNDCNT_X)
#define REG_SOUNDBIAS    (*(vu16 *)REG_ADDR_SOUNDBIAS)
#define REG_WAVE_RAM     (*(vu32 *)REG_ADDR_WAVE_RAM)
#define REG_FIFO_A       (*(vu32 *)REG_ADDR_FIFO_A)
#define REG_FIFO_B       (*(vu32 *)REG_ADDR_FIFO_B)
#define REG_DMA0SAD      (*(vu32 *)REG_ADDR_DMA0SAD)
#define REG_DMA0DAD      (*(vu32 *)REG_ADDR_DMA0DAD)
#define REG_DMA0CNT      (*(vu32 *)REG_ADDR_DMA0CNT)
#define REG_DMA0CNT_L    (*(vu16 *)REG_ADDR_DMA0CNT_L)
#define REG_DMA0CNT_H    (*(vu16 *)REG_ADDR_DMA0CNT_H)
#define REG_DMA1SAD      (*(vu32 *)REG_ADDR_DMA1SAD)
#define REG_DMA1DAD      (*(vu32 *)REG_ADDR_DMA1DAD)
#define REG_DMA1CNT      (*(vu32 *)REG_ADDR_DMA1CNT)
#define REG_DMA1CNT_L    (*(vu16 *)REG_ADDR_DMA1CNT_L)
#define REG_DMA1CNT_H    (*(vu16 *)REG_ADDR_DMA1CNT_H)
#define REG_DMA2SAD      (*(vu32 *)REG_ADDR_DMA2SAD)
#define REG_DMA2DAD      (*(vu32 *)REG_ADDR_DMA2DAD)
#define REG_DMA2CNT      (*(vu32 *)REG_ADDR_DMA2CNT)
#define REG_DMA2CNT_L    (*(vu16 *)REG_ADDR_DMA2CNT_L)
#define REG_DMA2CNT_H    (*(vu16 *)REG_ADDR_DMA2CNT_H)
#define REG_DMA3SAD      (*(vu32 *)REG_ADDR_DMA3SAD)
#define REG_DMA3DAD      (*(vu32 *)REG_ADDR_DMA3DAD)
#define REG_DMA3CNT      (*(vu32 *)REG_ADDR_DMA3CNT)
#define REG_DMA3CNT_L    (*(vu16 *)REG_ADDR_DMA3CNT_L)
#define REG_DMA3CNT_H    (*(vu16 *)REG_ADDR_DMA3CNT_H)
#define REG_TM0CNT       (*(vu32 *)REG_ADDR_TM0CNT)
#define REG_TM0CNT_L     (*(vu16 *)REG_ADDR_TM0CNT_L)
#define REG_TM0CNT_H     (*(vu16 *)REG_ADDR_TM0CNT_H)
#define REG_TM1CNT       (*(vu32 *)REG_ADDR_TM1CNT)
#define REG_TM1CNT_L     (*(vu16 *)REG_ADDR_TM1CNT_L)
#define REG_TM1CNT_H     (*(vu16 *)REG_ADDR_TM1CNT_H)
#define REG_TM2CNT       (*(vu32 *)REG_ADDR_TM2CNT)
#define REG_TM2CNT_L     (*(vu16 *)REG_ADDR_TM2CNT_L)
#define REG_TM2CNT_H     (*(vu16 *)REG_ADDR_TM2CNT_H)
#define REG_TM3CNT       (*(vu32 *)REG_ADDR_TM3CNT)
#define REG_TM3CNT_L     (*(vu16 *)REG_ADDR_TM3CNT_L)
#define REG_TM3CNT_H     (*(vu16 *)REG_ADDR_TM3CNT_H)
#define REG_SIOCNT       (*(vu16 *)REG_ADDR_SIOCNT)
#define REG_SIODATA8     (*(vu16 *)REG_ADDR_SIODATA8)
#define REG_SIODATA32    (*(vu32 *)REG_ADDR_SIODATA32)
#define REG_SIOMULTI0    (*(vu16 *)REG_ADDR_SIOMULTI0)
#define REG_SIOMULTI1    (*(vu16 *)REG_ADDR_SIOMULTI1)
#define REG_SIOMULTI2    (*(vu16 *)REG_ADDR_SIOMULTI2)
#define REG_SIOMULTI3    (*(vu16 *)REG_ADDR_SIOMULTI3)
#define REG_SIOMLT_SEND  (*(vu16 *)REG_ADDR_SIOMLT_SEND)
#define REG_KEYINPUT     (*(vu16 *)REG_ADDR_KEYINPUT)
#define REG_KEYCNT       (*(vu16 *)REG_ADDR_KEYCNT)
#define REG_RCNT         (*(vu16 *)REG_ADDR_RCNT)
#define REG_JOYCNT       (*(vu16 *)REG_ADDR_JOYCNT)
#define REG_JOY_RECV     (*(vu32 *)REG_ADDR_JOY_RECV)
#define REG_JOY_TRANS    (*(vu32 *)REG_ADDR_JOY_TRANS)
#define REG_JOYSTAT      (*(vu16 *)REG_ADDR_JOYSTAT)
#define REG_IE           (*(vu16 *)REG_ADDR_IE)
#define REG_IF           (*(vu16 *)REG_ADDR_IF)
#define REG_WAITCNT      (*(vu16 *)REG_ADDR_WAITCNT)
#define REG_IME          (*(vu16 *)REG_ADDR_IME)

// REG_DISPCNT bits
#define DISPCNT_OBJ_1D_MAP 0x0040
#define DISPCNT_BG0_ON    0x0100
#define DISPCNT_BG1_ON    0x0200
#define DISPCNT_BG2_ON    0x0400
#define DISPCNT_BG3_ON    0x0800
#define DISPCNT_OBJ_ON    0x1000
#define DISPCNT_WIN0_ON   0x2000
#define DISPCNT_WIN1_ON   0x4000
#define DISPCNT_OBJWIN_ON 0x8000

// REG_BGxCNT bits
#define BGCNT_PRIORITY_MASK 0x0003
#define BGCNT_PRIORITY(n)   (n)        // 0-3, 0 drawn on top
#define BGCNT_CHARBASE(n)   ((n) << 2) // tiles at VRAM + n * 0x4000
#define BGCNT_SCREENBASE(n) ((n) << 8) // tilemap at VRAM + n * 0x800
#define BGCNT_256COLOR      0x0080
// Text BG sizes (bits 14-15)
#define BGCNT_TXT256x256    0x0000
#define BGCNT_TXT512x256    0x4000
#define BGCNT_TXT256x512    0x8000
#define BGCNT_TXT512x512    0xC000
// A text tilemap entry: tile number, flips and the 16-colour palette bank.
#define TILEMAP_HFLIP       0x0400
#define TILEMAP_VFLIP       0x0800
#define TILEMAP_PALETTE(n)  ((n) << 12)

// REG_BLDCNT bits: first targets (bits 0-5), effect (bits 6-7), second targets (bits 8-13)
#define BLDCNT_TGT1_BG0     0x0001
#define BLDCNT_TGT1_BG1     0x0002
#define BLDCNT_TGT1_BG2     0x0004
#define BLDCNT_TGT1_BG3     0x0008
#define BLDCNT_TGT1_OBJ     0x0010
#define BLDCNT_TGT1_BD      0x0020
#define BLDCNT_EFFECT_BLEND   0x0040 // alpha blend with the second targets (REG_BLDALPHA)
#define BLDCNT_EFFECT_LIGHTEN 0x0080 // towards white by REG_BLDY
#define BLDCNT_EFFECT_DARKEN  0x00C0 // towards black by REG_BLDY
#define BLDCNT_TGT2_BG0     0x0100
#define BLDCNT_TGT2_BG1     0x0200
#define BLDCNT_TGT2_BG2     0x0400
#define BLDCNT_TGT2_BG3     0x0800
#define BLDCNT_TGT2_OBJ     0x1000
#define BLDCNT_TGT2_BD      0x2000
#define BLDCNT_TGT2_ALL     0x3F00 // BG0-3, sprites and the backdrop

// REG_BLDALPHA: weights of the first and second targets, 0-16 (16 = full)
#define BLDALPHA_BLEND(a, b) ((a) | ((b) << 8))

// REG_WINxH / REG_WINxV: first pixel/line in the high byte, the one after the last in the low byte
#define WIN_RANGE(start, end) (((start) << 8) | (end))
// REG_WININ / REG_WINOUT: what shows in a window (bits 0-5 one window, bits 8-13 the other)
#define WIN_ALL_LAYERS      0x003F // BG0-3, sprites and colour effects

// REG_DISPSTAT bits
#define DISPSTAT_HBLANK_IRQ   0x10
#define DISPSTAT_VCOUNT_IRQ   0x20 // interrupt when VCOUNT reaches the line in bits 8-15
#define DISPSTAT_VCOUNT_SHIFT 8

// REG_IE / REG_IF bits
#define IRQ_VBLANK 0x01
#define IRQ_HBLANK 0x02
#define IRQ_VCOUNT 0x04
#define IRQ_TIMER3 0x40
#define IRQ_SERIAL 0x80

// REG_SIOCNT bits (normal mode)
#define SIO_INTERNAL_CLOCK 0x0001 // shift clock: 0 = external (the other GBA drives it)
#define SIO_SO_HIGH        0x0008 // SO line high while idle
#define SIO_START          0x0080
#define SIO_32BIT_MODE     0x1000
#define SIO_IRQ_ENABLE     0x4000

// REG_SIOCNT bits (multiplayer mode)
#define SIO_115200_BPS     0x0003
#define SIO_MULTI_MODE     0x2000

// REG_TMxCNT_H bits
#define TIMER_64CLK        0x0001 // prescaler: count every 64 cycles
#define TIMER_IRQ_ENABLE   0x0040
#define TIMER_ENABLE       0x0080

// REG_DMAxCNT_H bits
#define DMA_DEST_RELOAD  0x0060 // destination: increment, back to the start on each repeat
#define DMA_REPEAT       0x0200
#define DMA_32BIT        0x0400
#define DMA_START_HBLANK 0x2000
#define DMA_DREQ_ON      0x0800 // game pak DRQ (DMA3 only)
#define DMA_START_MASK   0x3000 // start timing: now, VBlank, HBlank or special
#define DMA_ENABLE       0x8000
// Clearing these (then DMA_ENABLE) stops a repeating DMA, as DmaStop does.
#define DMA_STOP_MASK    (u16)~(DMA_START_MASK | DMA_DREQ_ON | DMA_REPEAT)

// Key bits, as in REG_KEYINPUT (there 0 means pressed).
#define A_BUTTON      0x0001
#define B_BUTTON      0x0002
#define SELECT_BUTTON 0x0004
#define START_BUTTON  0x0008
#define DPAD_RIGHT    0x0010
#define DPAD_LEFT     0x0020
#define DPAD_UP       0x0040
#define DPAD_DOWN     0x0080
#define R_BUTTON      0x0100
#define L_BUTTON      0x0200
#define DPAD_ANY      (DPAD_RIGHT | DPAD_LEFT | DPAD_UP | DPAD_DOWN)

#endif // GUARD_GBA_IO_REG_H
