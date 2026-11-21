#include "global.h"
#include "ram_routines.h"

extern u8 gSys[];
extern u32 gUnk_03000014[];
extern u8 gUnk_02000480[];
extern u8 gUnk_03000374[];
extern u8 gUnk_02000C80[];
extern u8 gUnk_02000C8C[];
extern u8 end[];
extern const u16 gKeyRepeatMaskPairs[];
extern const u8 gHeapTags[];
extern const u32 gRomBootWords[];
void cpu_set(const void *src, void *dst, u32 ctl);
void cpu_fast_set(const void *src, void *dst, u32 ctl);
extern u8 gUnk_02000080[];
extern vu16 gUnk_02000004;
void sub_081DB79C(u32 arg);
void sound_driver_init(void);
void m4a_sound_init(u8 *p);
void set_vcount_mode(u32 mode, u32 line);
void heap_free(void *p);
void serial_link_update(void);
void flag_88c_handler_a(void);
void flag_88c_handler_b(void);
void dma_queue_drain(void);
void copy_shadow_to_io(void);
void flush_palette_blocks(void);
void *heap_alloc_raw(u32 pool, u32 size, const u8 *tag);
extern u8 gUnk_0203FFF8[];
extern u8 gUnk_0300004C[];
extern u8 irq_handler[];
void register_ram_reset(u32 flags);
void sub_081DE840(void);
void sub_081DE848(void);
void mt19937_seed(u32 state, u32 seed);
void mem_routines_init(u8 *buf);
void math_routines_init(u8 *buf);
u32 save_data_verify(void);
u32 check_warm_reset_counter(void);
void reset_state_0c28(void);
void load_rom_table(void);
void run_tasks(void);
void frame_update(void);
void init_sys_defaults(void);
void init_heaps(void);
void vblank_irq_callback(void);
void irq_nop(void);
void start_scene_or_textbox(void);
void *heap_alloc(u32 size, u32 pool, const u8 *tag, u8 clear);
void *heap_alloc2(u32 size, u32 pool, const u8 *tag, u8 clear);
void textbox_create(void *p, u32 kind, const u8 *tag);
void scene_start_task(void *node, u32 kind, const void *tag, u32 arg);
void obj_init_gfx(void *p, u32 kind, const u8 *tag, void (*cb)(void));
void update_key_state(u32 keys, u32 slot);
void queue_player_volume(u32 idx, u32 level);
void queue_player_word(u32 idx, u32 level);
void slot_reset(u32 idx);

/* One step of a volume ramp: TRUE while it is still running. */
static inline u8 ramp_step(u16 *value, u8 *count, s16 speed, u16 target, u8 period)
{
    s32 v;

    if (++*count == period)
    {
        *value = target;
        return FALSE;
    }
    v = *value;
    v += speed;
    if (speed > 0)
    {
        if (v > target)
            v = target;
    }
    else if (speed < 0)
    {
        if (v < target)
            v = target;
    }
    *value = v;
    return TRUE;
}

struct DmaQueueEntry
{
    u32 unk0;
    u32 dst;
    u32 unk8;
    u32 unkC;
};

struct SysArea
{
    /* Palettes changed in the shadow (gUnk_02000080) since the last flush: bit n is palette n. */
    u16 objPalDirty;
    u16 bgPalDirty;
    struct DmaQueueEntry *volatile dmaQueue;
    vu8 dmaCount;
    u8 filler9[0x20 - 9];
    u32 frameCount;
    u32 playFrames;
    u8 filler28[0x884 - 0x28];
    u32 palMask;               /* palettes flush_palette_blocks may copy: BG in bits 0-15, OBJ in 16-31 */
    u8 sysFlags;
    u8 tickFlags;
    u8 filler88A[0x88C - 0x88A];
    u8 busy;
    /* Fades in progress, stepped once a frame by update_meter_ramps. */
    u8 rampUnk0 : 2;
    u8 songVolRamping : 1;     /* player 0 volume */
    u8 slot0VolRamping : 1;    /* player 1 volume */
    u8 slot1VolRamping : 1;    /* player 2 volume */
    u8 slot0WordRamping : 1;   /* player 1 word (sound command 8) */
    u8 slot1WordRamping : 1;   /* player 2 word */
    u8 rampUnk7 : 1;
    u8 filler88E[0x890 - 0x88E];
    /* Levels are 8.8 fixed point: the high byte goes to the sound driver. */
    u16 songVol;
    u8 songVolPeriod;          /* frames the fade takes */
    u8 songVolCount;           /* frames done */
    u16 songVolTarget;
    s16 songVolStep;           /* added each frame */
    u16 songSlot[2];           /* song playing on players 1 and 2 */
    u16 slotVol[2];
    u16 slotSavedVol[2];       /* volume to restore when the slot restarts */
    u16 slotWord[2];           /* default 0x4B00 */
    u8 slotVolPeriod[2];
    u8 slotVolCount[2];
    u16 slotVolTarget[2];
    s16 slotVolStep[2];
    u8 slotWordPeriod[2];
    u8 slotWordCount[2];
    u16 slotWordTarget[2];
    s16 slotWordStep[2];
    u8 filler8C0;
    u8 vblankCount;
    u8 filler8C2[0x8C4 - 0x8C2];
    u32 irqCallback;
};

#define SYS_AREA ((struct SysArea *)gSys)

/* sysFlags bits (struct SysFlags888 gives the same byte as bitfields) */
#define SYS_FLAG_OAM_BUF  0x08 /* the OAM buffer being filled */
#define SYS_FLAG_OAM_SWAP 0x20 /* set: the buffers swap every frame (the other one is shown) */
#define SYS_FLAG_CPU_COPY 0x40 /* copy OAM and palettes with the CPU instead of DMA3 */

struct DmaChannel
{
    vu32 src;
    vu32 dst;
    vu16 cntL;
    vu16 cntH;
};

struct DmaRegs
{
    vu32 src;
    vu32 dst;
    vu32 ctl;
};

/* Key state of one input slot (an array of them at gUnk_03000374), updated by update_key_state.
 * Key bits are the GBA's 10 KEYINPUT bits. */
struct KeyRecord
{
    u16 held;           /* keys down this frame */
    u16 pressed;        /* went down this frame */
    u16 released;       /* went up this frame */
    u16 fired;          /* auto-repeat: pressed, or held long enough to repeat this frame */
    u8 delay;           /* frames before the first repeat */
    u8 interval;        /* frames between later repeats */
    u8 counters[8];     /* frames left to the next repeat, per gKeyRepeatMaskPairs group */
    u8 filler12[2];
};

/* Header in front of every heap block; the blocks of a pool form a doubly linked list in address
   order. size counts the header; bit 0 (HEAP_BLOCK_FREE) is set while the block is free. tag names
   the block's owner (4 chars, not terminated). */
struct HeapBlock
{
    struct HeapBlock *prev;
    struct HeapBlock *next;
    u32 size;
    u8 tag[4];
};

#define HEAP_BLOCK_FREE 1
#define HEAP_FREE_TAG (gHeapTags + 0x40) /* tag of a free block */

struct SysBusyBits
{
    u8 unk0 : 1;
    u8 mode : 2;
    u8 unk3 : 5;
} __attribute__((packed));

struct SysBusyLine
{
    u16 unk0 : 3;
    u16 line : 7;
    u16 unk10 : 6;
} __attribute__((packed));

struct SysFlags888
{
    u32 unk0 : 3;
    u32 oamBuf : 1;
    u32 oamHold : 1;
    u32 oamSwap : 1;
    u32 unk6 : 2;
    u32 tickOn : 1;
    u32 keyTimer : 4;
    u32 keyPeriod : 4;
    u32 unk17 : 15;
};

struct OamSlot
{
    u32 attr01;
    u16 attr2;
    u16 attr3;
};

// The bitfield view of the system area that frame_update uses.
struct SysCtl
{
    u8 filler0[0x1C];
    u32 unk1C; /* counts every frame */
    u8 filler20[0x24 - 0x20];
    u32 playFrames; /* counts the frames tickOn is set, stops at 0xFFFFFFFF */
    u16 keysHeld;
    u16 keysNew;
    u8 filler2C[0x78 - 0x2C];
    u8 unk78;
    u8 unk79;
    u8 filler7A[0x80 - 0x7A];
    struct OamSlot oam[2][0x80];
    struct OamSlot *oamCur;
    u32 palMask;
    u8 unk888_0 : 3;
    u32 oamBuf : 1;
    u8 oamHold : 1;
    u32 oamSwap : 1;
    u8 unk888_6 : 1;
    u8 waitIntr : 1;
    u32 tickOn : 1;
    u32 keyTimer : 4;
    u32 keyPeriod : 4;
    u8 unk888_17 : 2;
    u8 resetCombo : 1;
    u8 sleepMode : 2;
    u8 sleepArmed : 1;
    u8 asleep : 1;
    u8 unk888_24 : 8;
    u8 filler88C[0x8C1 - 0x88C];
    u8 vblankCount;
};

#define SYSCTL ((struct SysCtl *)gSys)

/* A+B+Select+Start resets the game; L+R+Select puts it to sleep. */
#define SOFT_RESET_KEYS (A_BUTTON | B_BUTTON | SELECT_BUTTON | START_BUTTON)
#define SLEEP_KEYS (L_BUTTON | R_BUTTON | SELECT_BUTTON)

static inline void flags_and(u8 *p, u32 m)
{
    u32 t = *p;
    t &= m;
    *p = t;
}

NONMATCH("asm/code_08017A00/agb_main.s", void agb_main(void))
{
    u32 slot;
    u32 zero;
    void *handler;
    u8 *rf;
    u8 *sys;
    u32 *q;
    u32 i;
    u32 k;
    void *p;
    u32 keys;
    u32 b6;
    u32 f2;
    u32 flags;
    u32 n;
    u32 v200;
    u8 *base;

    REG_WAITCNT = 0x4014;
    rf = gUnk_0203FFF8;
    if ((rf[7] & 3) == 1)
        sub_081DE840();
    REG_SOUNDCNT_L = 0;
    REG_SOUNDCNT_H &= 0xCCFF;
    register_ram_reset(0xFC);
    REG_WAITCNT = 0x4014;

    zero = 0;
    slot = zero;
    cpu_set(&slot, (void *)0x03000000, ((*(const u32 *)((u32)gRomBootWords + 4) + 0xFCFFFE00) << 9 >> 11) | 0x05000000);
    slot = zero;
    cpu_set(&slot, (void *)0x02000000, 0x0500FFFE);

    sys = gSys;
    b6 = ((rf[7] >> 7) & 3) << 1;
    flags_and(sys + 0x888, -7);
    sys[0x888] |= b6;
    flags_and(sys + 0x88A, -0x21);
    REG_IME = 0;
    REG_IE = 0x2000;
    REG_IME = 1;
    REG_DISPCNT = 0x80;
    *(vu16 *)0x02000000 = 0x80;

    handler = irq_nop;
    slot = (u32)handler;
    cpu_set(&slot, gUnk_03000014, 0x0500000E);
    cpu_set(irq_handler, gUnk_0300004C, 0x180);
    ((u32 *)gUnk_03000014)[2] = (u32)vblank_irq_callback;
    *(u32 *)(sys + 0x8C4) = (u32)handler;
    *(u32 *)0x03007FFC = (u32)gUnk_0300004C;
    init_heaps();

    p = heap_alloc(0x800, 1, gHeapTags + 8, 0);
    *(u32 *)(sys + 4) = (u32)p;
    slot = zero;
    cpu_set(&slot, p, 0x05000200);

    v200 = 0x200;
    base = sys + 0x80;
    for (n = 0x100; n != 0; n -= 0x10)
    {
        for (k = 0; k < 0x80; k += 8)
        {
            *(u32 *)(base + k) = v200;
            *(u32 *)(base + k + 4) = zero;
        }
        base += 0x80;
    }

    flags_and(sys + 0x888, -9);
    *(u32 *)(sys + 0x880) = (u32)(sys + 0x80);
    *(u16 *)(sys + 0x78) |= 0x3FF;
    for (i = 0; i < 4; i++)
    {
        u8 *e = sys + 0x30 + 0x14 * i;
        e[0] = 0xA;
        e[1] = 2;
    }
    flags = (*(u32 *)(sys + 0x888) & 0xFFFE1FFF) | 0x2000;
    *(u32 *)(sys + 0x888) = flags;
    flags_and(sys + 0x889, -0x1F);

    if (*(u32 *)(sys + 0x8C8) == 0)
    {
        p = heap_alloc((u32)gSineTable - (u32)iwram_sqrt, 0, gHeapTags + 0x10, 0);
        *(u32 *)(sys + 0x8C8) = (u32)p;
    }
    math_routines_init(*(u8 **)(sys + 0x8C8));

    if (*(u32 *)(sys + 0x8CC) == 0)
    {
        p = heap_alloc((u32)iwram_sqrt - (u32)iwram_fill, 0, gHeapTags + 0x18, 0);
        *(u32 *)(sys + 0x8CC) = (u32)p;
    }
    mem_routines_init(*(u8 **)(sys + 0x8CC));

    if (*(u32 *)(sys + 0x8D0) == 0)
    {
        p = heap_alloc(0x9C8, 1, gHeapTags + 0x20, 0);
        *(u32 *)(sys + 0x8D0) = (u32)p;
    }
    mt19937_seed(*(u32 *)(sys + 0x8D0), 0x49895963);

    f2 = (sys[0x88A] & -5) | ((save_data_verify() & 1) << 2);
    sys[0x88A] = f2;
    rf[6] &= 0x7F;
    if (f2 & 4)
    {
        if ((rf[7] & 3) == 2)
        {
            b6 = check_warm_reset_counter() << 7;
            rf[6] = (rf[6] & 0x7F) | b6;
        }
    }

    init_sys_defaults();
    reset_state_0c28();
    load_rom_table();

    REG_IME = 0;
    gUnk_02000004 |= 8;
    REG_IE |= 1;
    REG_IF |= 1;
    REG_IME = 1;
    *(vu16 *)gUnk_02000080 = 0x7FFF;
    *(u32 *)(sys + 0x884) = 0xFFFFFFFF;
    ((struct SysArea *)sys)->objPalDirty = 0xFFFF;
    ((struct SysArea *)sys)->bgPalDirty = 0xFFFF;
    flush_palette_blocks();
    copy_shadow_to_io();
    frame_update();
    sys[0x88A] |= 8;

    if ((rf[7] & 3) == 2)
    {
        keys = (~REG_KEYINPUT) & 0xFFFF;
        update_key_state(keys, 0);
        if (sys[0x8C0] == 7)
            sys[0x8C0] = (rf[7] >> 4) & 7;
        rf[7] &= -4;
        b6 = rf[6];
        if ((b6 & 0x80) == 0)
        {
            rf[6] = b6 & 0x7F;
            p = heap_alloc2(0x40, 0, gHeapTags + 0x28, 0);
            textbox_create(p, 8, gHeapTags + 0x30);
        }
        else
        {
            p = heap_alloc2(0xE4, 0, gHeapTags + 0x30, 0);
            scene_start_task(p, 8, gHeapTags + 0x30, 2);
        }
    }
    else
    {
        p = heap_alloc2(0x30, 0, gHeapTags + 0x38, 0);
        obj_init_gfx(p, 1, gHeapTags + 0x38, start_scene_or_textbox);
    }

    for (;;)
    {
        run_tasks();
        sub_081DE848();
        frame_update();
    }
}
END_NONMATCH

extern u8 gUnk_03000C28;
extern void *gUnk_03000D0C;
void serial_timer_tick(void);
void update_meter_ramps(void);
void dma_stop_and_reset(void);
void sleep_until_key(void);
void vblank_intr_wait(void);
void intr_wait(u32 a, u32 b);
void serial_frame(void);
void poll_keys(void);
/* End-of-frame work: handles the soft reset and sleep key combos, waits for vblank, counts frames,
   reads the keys every keyPeriod frames, flips the double-buffered OAM shadow (clearing the new
   one) and polls the keys while no serial link (gUnk_03000D0C) is open. */
void frame_update(void)
{
    u32 keys;
    struct OamSlot *dst;
    struct OamSlot *src;
    u32 i;

    serial_timer_tick();
    update_meter_ramps();
    if (SYSCTL->resetCombo && (SYSCTL->keysHeld & SOFT_RESET_KEYS) == SOFT_RESET_KEYS && (SYSCTL->keysNew & SOFT_RESET_KEYS) != 0)
        dma_stop_and_reset();

    /* sleep: hold L+R+Select (no other key), then release them; after waking, asleep stays set
       until they are released, so the wake combo doesn't arm it again */
    if (SYSCTL->sleepMode == 3)
    {
        if (SYSCTL->asleep)
        {
            if ((SYSCTL->keysHeld & SLEEP_KEYS) == 0)
                SYSCTL->asleep = 0;
        }
        else if (SYSCTL->sleepArmed)
        {
            keys = SYSCTL->keysHeld;
            if ((keys & ~SLEEP_KEYS) != 0)
                SYSCTL->sleepArmed = 0;
            else if ((keys & SLEEP_KEYS) == 0)
            {
                SYSCTL->sleepArmed = 0;
                sleep_until_key();
                SYSCTL->asleep = 1;
            }
        }
        else
        {
            if ((SYSCTL->keysHeld & SLEEP_KEYS) == SLEEP_KEYS)
                SYSCTL->sleepArmed = 1;
        }
    }

    if (!SYSCTL->waitIntr)
    {
        if (gUnk_03000C28 == 0)
            vblank_intr_wait();
    }
    else
    {
        intr_wait(1, 0x80);
    }

    SYSCTL->vblankCount = 0;
    serial_frame();
    SYSCTL->unk1C++;
    if (SYSCTL->playFrames < 0xFFFFFFFF)
        SYSCTL->playFrames += SYSCTL->tickOn;
    if (++SYSCTL->keyTimer >= SYSCTL->keyPeriod)
    {
        update_key_state((u16)~REG_KEYINPUT, 0);
        SYSCTL->keyTimer = 0;
    }
    if (!SYSCTL->oamHold)
    {
        /* swap the OAM buffers; the new one is hidden, keeping the old one's attr3 (affine params) */
        SYSCTL->oamBuf ^= SYSCTL->oamSwap;
        SYSCTL->oamCur = SYSCTL->oam[SYSCTL->oamBuf];
        dst = SYSCTL->oamCur;
        src = SYSCTL->oam[SYSCTL->oamBuf ^ SYSCTL->oamSwap];
        for (i = 0x80; i != 0; i--, dst++, src++)
        {
            dst->attr01 = 0x200;
            dst->attr2 = 0;
            dst->attr3 = src->attr3;
        }
    }
    if (gUnk_03000D0C == 0 && (SYSCTL->unk79 & 0x40) == 0)
        poll_keys();
}

/* The VBlank interrupt handler: updates the serial link, writes the I/O register shadows, copies
 * the OAM buffer being shown (gSys + 0x80, buffer oamBuf ^ oamSwap) to OAM by DMA3 or by the CPU,
 * flushes changed palette blocks and the DMA queue, runs the scene's irqCallback, counts the frame
 * and sets the BIOS's VBlank flag so VBlankIntrWait returns. */
void vblank_irq_callback(void)
{
    u8 flags;

    serial_link_update();
    if ((((struct SysArea *)gSys)->busy & 6) == 0)
        flag_88c_handler_b();
    copy_shadow_to_io();
    flags = ((struct SysArea *)gSys)->sysFlags;
    if ((flags & SYS_FLAG_CPU_COPY) == 0)
    {
        struct DmaRegs *dma3 = (struct DmaRegs *)REG_ADDR_DMA3SAD;
        dma3->src = (u32)gSys + 0x80 + (((((u32)flags << 28) >> 31) ^ (((u32)flags << 26) >> 31)) << 10);
        dma3->dst = OAM;
        dma3->ctl = 0x84000100; /* enable, 32-bit, 0x100 words */
        (void)dma3->ctl;
    }
    else
    {
        MEM_ROUTINE(void (*)(u8 *, u8 *, u32), iwram_copy)((u8 *)gSys + 0x80 + (((((u32)flags << 28) >> 31) ^ (((u32)flags << 26) >> 31)) << 10), (u8 *)OAM, 0x400);
    }
    flush_palette_blocks();
    dma_queue_drain();
    ((void (*)(void))((struct SysArea *)gSys)->irqCallback)();
    flag_88c_handler_a();
    ((struct SysArea *)gSys)->vblankCount++;
    ((struct SysArea *)gSys)->frameCount++;
    REG_IME = 0;
    INTR_CHECK |= IRQ_VBLANK;
    REG_IME = 1;
}


/* Copies every 16-colour palette marked dirty (and allowed by palMask) from the palette shadow
 * gUnk_02000080 to palette RAM, by DMA3 or (SYS_FLAG_CPU_COPY) with CpuFastSet, then clears the
 * dirty masks. Bits 0-15 are the BG palettes, bits 16-31 the OBJ palettes. */
void flush_palette_blocks(void)
{
    struct DmaRegs *dma3;
    u32 dirty;
    u8 *src;
    u8 *dst;

    if (SYS_AREA->palMask == 0)
        return;
    dirty = ((u32)SYS_AREA->objPalDirty << 16) | SYS_AREA->bgPalDirty;
    dirty &= SYS_AREA->palMask;
    if ((SYS_AREA->sysFlags & SYS_FLAG_CPU_COPY) == 0)
    {
        src = gUnk_02000080;
        dst = (u8 *)PLTT;
        if (dirty != 0)
        {
            do
            {
                if ((dirty & 1) != 0)
                {
                    dma3 = (struct DmaRegs *)REG_ADDR_DMA3SAD;
                    dma3->src = (u32)src;
                    dma3->dst = (u32)dst;
                    dma3->ctl = 0x84000008; /* enable, 32-bit, 8 words: one palette */
                    (void)dma3->ctl;
                }
                src += 0x20;
                dst += 0x20;
                dirty >>= 1;
            } while (dirty != 0);
        }
    }
    else
    {
        src = gUnk_02000080;
        dst = (u8 *)PLTT;
        if (dirty != 0)
        {
            do
            {
                if ((dirty & 1) != 0)
                    cpu_fast_set(src, dst, 8);
                src += 0x20;
                dst += 0x20;
                dirty >>= 1;
            } while (dirty != 0);
        }
    }
    SYS_AREA->bgPalDirty = 0;
    SYS_AREA->objPalDirty = 0;
}

ASM_FUNC("asm/code_08017A00/dma_copy_or_queue.s", void dma_copy_or_queue(void))

ASM_FUNC("asm/code_08017A00/dma_fill_or_queue.s", void dma_fill_or_queue(void))

ASM_FUNC("asm/code_08017A00/dma_cancel_request.s", void dma_cancel_request(void))
ASM_FUNC("asm/code_08017A00/dma_queue_drain.s", void dma_queue_drain(void))

void copy_shadow_to_io(void)
{
    u32 *src = (u32 *)EWRAM_START;
    u32 *dst = (u32 *)REG_BASE;

    *(u16 *)dst = *(u16 *)src;
    src++;
    dst++;
    *(u16 *)dst = *(u16 *)src;
    src++;
    dst++;
    *dst++ = *src++;
    *dst++ = *src++;
    *dst++ = *src++;
    *dst++ = *src++;
    *dst++ = *src++;
    *dst++ = *src++;
    *dst++ = *src++;
    *dst++ = *src++;
    *dst++ = *src++;
    *dst++ = *src++;
    *dst++ = *src++;
    *dst++ = *src++;
    *dst++ = *src++;
    *dst++ = *src++;
    *dst++ = *src++;
    *dst++ = *src++;
    *dst++ = *src++;
    *(u16 *)dst = *(u16 *)src;
    src++;
    dst++;
    *dst++ = *src++;
    *(u16 *)dst = *(u16 *)src;
}


ASM_FUNC("asm/code_08017A00/poll_keys.s", void poll_keys(void))

/* Updates input slot `slot`'s KeyRecord from this frame's keys: the pressed and released edges,
 * and the auto-repeat keys (fired), counted per key group of gKeyRepeatMaskPairs: a group fires
 * when it goes down, then after `delay` frames and every `interval` frames while held. */
void update_key_state(u32 keys, u32 slot)
{
    struct KeyRecord *rec;
    const u16 *groups;
    u32 now;
    u32 prev;
    u32 held;
    u32 fired;
    u32 i;
    u32 down;
    u32 left;

    rec = (struct KeyRecord *)(gUnk_03000374 + slot * sizeof(struct KeyRecord));
    now = (keys << 22) >> 22; /* the 10 key bits */
    prev = rec->held;
    rec->pressed = now & ~prev;
    rec->released = prev & ~now;
    fired = 0;
    held = rec->held;
    groups = gKeyRepeatMaskPairs;
    for (i = 0; i < 8; i++)
    {
        down = groups[i] & now;
        if (down != 0)
        {
            if ((held & down) == 0)
            {
                /* just pressed */
                fired |= down;
                rec->counters[i] = rec->delay;
            }
            else
            {
                left = rec->counters[i] - 1;
                rec->counters[i] = left;
                if ((left << 24) == 0)
                {
                    fired |= down;
                    rec->counters[i] = rec->interval;
                }
            }
        }
        else
            rec->counters[i] = down;
    }
    rec->fired = fired;
    rec->held = now;
}

void dma_stop_and_reset(void)
{
    register vu16 *dma asm("r1");
    vu16 *dma3;
    u8 f;

    f = ((struct SysArea *)gSys)->sysFlags;
    f |= 1;
    ((struct SysArea *)gSys)->sysFlags = f;
    dma = (vu16 *)REG_ADDR_DMA0SAD;
    dma[5] = dma[5] & 0xC5FF;
    dma[5] = dma[5] & 0x7FFF;
    (void)dma[5];
    dma = (vu16 *)REG_ADDR_DMA1SAD;
    dma[5] = dma[5] & 0xC5FF;
    dma[5] = dma[5] & 0x7FFF;
    (void)dma[5];
    dma = (vu16 *)REG_ADDR_DMA2SAD;
    dma[5] = dma[5] & 0xC5FF;
    dma[5] = dma[5] & 0x7FFF;
    (void)dma[5];
    dma3 = (vu16 *)REG_ADDR_DMA3SAD;
    dma3[5] = dma3[5] & 0xC5FF;
    dma3[5] = dma3[5] & 0x7FFF;
    (void)dma3[5];
    sub_081DB79C(0xFC);
}

void stub_nop_08018A5C(void)
{
}

void set_sys24_request(u32 value)
{
    u8 f;
    ((struct SysArea *)gSys)->playFrames = value;
    f = ((struct SysArea *)gSys)->tickFlags;
    f |= 1;
    ((struct SysArea *)gSys)->tickFlags = f;
}

ASM_FUNC("asm/code_08017A00/sleep_until_key.s", void sleep_until_key(void))

void oam_affine_identity_init(void)
{
    u16 *q;
    s32 i;

    q = (u16 *)((u8 *)gSys + 0x80 + ((((u32)((struct SysArea *)gSys)->sysFlags << 28) >> 31) << 10));
    for (i = 32; i != 0; i--, q += 16)
    {
        q[3] = 0x100;
        q[7] = 0;
        q[11] = 0;
        q[15] = 0x100;
    }
}

void irq_nop(void)
{
}

void set_irq_callback(u32 kind, void *handler)
{
    u32 h = (u32)handler;
    u32 a;
    u32 b;
    if (h == 0)
        h = (u32)irq_nop;
    REG_IME = 0;
    if (kind == 2)
    {
        a = (u32)gSys;
        b = 0x8C4;
    }
    else
    {
        b = (u32)gUnk_03000014;
        a = kind << 2;
    }
    *(u32 *)(a + b) = h;
    REG_IME = 1;
}

ASM_FUNC("asm/code_08017A00/init_heaps.s", void init_heaps(void))

void heap_free(void *p)
{
    struct HeapBlock *blk;
    struct HeapBlock *prev;
    struct HeapBlock *next;
    struct HeapBlock *link;
    s32 k;

    if (p == 0)
        return;
    blk = (struct HeapBlock *)p - 1;
    prev = blk->prev;
    next = blk->next;
    blk->size = blk->size | 1;
    for (k = 0; k < 4; k++)
        blk->tag[k] = (gHeapTags + 0x40)[k];
    if (next != 0 && (next->size & 1) != 0)
    {
        blk->size = blk->size + (next->size & ~1);
        link = next->next;
        blk->next = link;
        if (link != 0)
            link->prev = blk;
    }
    if (prev != 0 && (prev->size & 1) != 0)
    {
        prev->size = prev->size + (blk->size & ~1);
        link = blk->next;
        prev->next = link;
        if (link != 0)
            link->prev = prev;
    }
}

/* Allocates `size` bytes (rounded up to 4) from heap pool `pool` (0: the one at `end`, otherwise
   the one at gUnk_02000C80), first fit: the first free block that is big enough is taken, and split
   when more than a header and 0x10 bytes would be left over. The block gets `tag`; returns the
   memory after its header, or NULL when size is 0 or nothing fits. */
void *heap_alloc_raw(u32 pool, u32 size, const u8 *tag)
{
    struct HeapBlock *block;
    struct HeapBlock *rest;
    struct HeapBlock *next;
    u8 *dst;
    void *mem;
    register u32 needed asm("r12");
    u32 total;
    u32 blockSize;
    u32 left;
    s32 k;

    if (size == 0)
        return NULL;
    block = (struct HeapBlock *)gUnk_02000C80;
    if (pool == 0)
        block = (struct HeapBlock *)end;
    size = (size + 3) & ~3;
    do
    {
        blockSize = block->size;
        if ((blockSize & HEAP_BLOCK_FREE) != 0)
        {
            needed = total = size + sizeof(struct HeapBlock);
            if (blockSize >= total)
            {
                blockSize &= ~HEAP_BLOCK_FREE;
                if (blockSize > size + 2 * sizeof(struct HeapBlock))
                {
                    /* split: the rest becomes a free block right after this one */
                    rest = (struct HeapBlock *)((u8 *)block + total);
                    left = blockSize - sizeof(struct HeapBlock);
                    rest->size = (left - size) | HEAP_BLOCK_FREE;
                    rest->prev = block;
                    next = block->next;
                    rest->next = next;
                    dst = block->tag;
                    mem = block + 1;
                    for (k = 0; k < 4; k++)
                        rest->tag[k] = HEAP_FREE_TAG[k];
                    if (next != NULL)
                        next->prev = rest;
                    block->size = needed;
                    block->next = rest;
                }
                else
                {
                    block->size = blockSize;
                    dst = block->tag;
                    mem = block + 1;
                }
                for (k = 0; k < 4; k++)
                    dst[k] = tag[k];
                return mem;
            }
        }
        block = block->next;
    } while (block != NULL);
    return NULL;
}


void heap_free_a(void *p)
{
    heap_free(p);
}

void heap_free_b(void *p)
{
    heap_free(p);
}

void *heap_alloc(u32 size, u32 pool, const u8 *tag, u8 clear)
{
    void *p;
    u32 zero;
    p = heap_alloc_raw(pool, size, tag);
    if (clear != 0)
    {
        zero = 0;
        cpu_set(&zero, p, 0x05000000 | ((size / 4) & 0x1FFFFF));
    }
    return p;
}

void *heap_alloc2(u32 size, u32 pool, const u8 *tag, u8 clear)
{
    void *p;
    u32 zero;
    p = heap_alloc_raw(pool, size, tag);
    if (clear != 0)
    {
        zero = 0;
        cpu_set(&zero, p, 0x05000000 | ((size / 4) & 0x1FFFFF));
    }
    return p;
}

void init_sys_defaults(void)
{
    u8 f;
    ((struct SysArea *)gSys)->songVol = 0xFF00;
    ((struct SysArea *)gSys)->slotVol[0] = 0xFF00;
    ((struct SysArea *)gSys)->slotVol[1] = 0xFF00;
    ((struct SysArea *)gSys)->slotWord[0] = 0x4B00;
    ((struct SysArea *)gSys)->slotWord[1] = 0x4B00;
    sound_driver_init();
    m4a_sound_init(gUnk_02000480);
    f = ((struct SysArea *)gSys)->busy;
    f |= 6;
    ((struct SysArea *)gSys)->busy = f;
    set_vcount_mode(0, 0x64);
}

static inline void dma_clear_enable(struct DmaRegs *dma)
{
    ((vu16 *)&dma->ctl)[1] = ((vu16 *)&dma->ctl)[1] & 0xC5FF;
    ((vu16 *)&dma->ctl)[1] = ((vu16 *)&dma->ctl)[1] & 0x7FFF;
    (void)((vu16 *)&dma->ctl)[1];
}

NONMATCH("asm/code_08017A00/set_vcount_mode.s", void set_vcount_mode(u32 mode, u32 line))
{
    u8 *sys;
    u8 *flags;
    u32 old;
    u32 v;

    sys = gSys;
    flags = &sys[0x88C];
    old = *flags;
    if (mode != ((struct SysBusyBits *)flags)->mode || (mode == 1 && (old & 6) == 2 && line != ((((u32)*(u16 *)flags) << 22) >> 25)))
    {
        dma_clear_enable((struct DmaRegs *)0x040000BC);
        dma_clear_enable((struct DmaRegs *)0x040000C8);
        if ((old & 6) == 2)
        {
            REG_IME = 0;
            gUnk_02000004 &= 0xFFDF;
            REG_IE &= 0xFFFB;
            REG_IF |= 4;
            REG_IME = 1;
        }
        v = ((mode & 3) << 1);
        v |= old & -7;
        *flags = v;
        if (mode == 1)
        {
            set_irq_callback(3, flag_88c_handler_b);
            REG_IME = 0;
            gUnk_02000004 &= 0xFF;
            gUnk_02000004 = (line << 8) | 0x20 | gUnk_02000004;
            REG_IE |= 4;
            REG_IF |= 4;
            REG_IME = mode;
            *(u16 *)flags = ((*(u16 *)flags) & -1017) | ((line & 0x7F) << 3);
        }
    }
    ((struct SysArea *)gSys)->busy |= 1;
}
END_NONMATCH

/* Once a frame: steps every running fade one frame (the song player's volume, the
 * two slot players' volume and word) and sends the new level to the sound driver.
 * A slot whose volume has faded out to 0 is stopped. */
void update_meter_ramps(void)
{
    if (SYS_AREA->songVolRamping)
    {
        SYS_AREA->songVolRamping = ramp_step(&SYS_AREA->songVol, &SYS_AREA->songVolCount,
            SYS_AREA->songVolStep, SYS_AREA->songVolTarget, SYS_AREA->songVolPeriod);
        queue_player_volume(0, SYS_AREA->songVol >> 8);
    }
    if (SYS_AREA->slot0VolRamping)
    {
        SYS_AREA->slot0VolRamping = ramp_step(&SYS_AREA->slotVol[0], &SYS_AREA->slotVolCount[0],
            SYS_AREA->slotVolStep[0], SYS_AREA->slotVolTarget[0], SYS_AREA->slotVolPeriod[0]);
        queue_player_volume(1, SYS_AREA->slotVol[0] >> 8);
        SYS_AREA->slotSavedVol[0] = SYS_AREA->slotVol[0];
        if (!SYS_AREA->slot0VolRamping && SYS_AREA->slotSavedVol[0] == 0)
            slot_reset(0);
    }
    if (SYS_AREA->slot1VolRamping)
    {
        SYS_AREA->slot1VolRamping = ramp_step(&SYS_AREA->slotVol[1], &SYS_AREA->slotVolCount[1],
            SYS_AREA->slotVolStep[1], SYS_AREA->slotVolTarget[1], SYS_AREA->slotVolPeriod[1]);
        queue_player_volume(2, SYS_AREA->slotVol[1] >> 8);
        SYS_AREA->slotSavedVol[1] = SYS_AREA->slotVol[1];
        if (!SYS_AREA->slot1VolRamping && SYS_AREA->slotSavedVol[1] == 0)
            slot_reset(1);
    }
    if (SYS_AREA->slot0WordRamping)
    {
        SYS_AREA->slot0WordRamping = ramp_step(&SYS_AREA->slotWord[0], &SYS_AREA->slotWordCount[0],
            SYS_AREA->slotWordStep[0], SYS_AREA->slotWordTarget[0], SYS_AREA->slotWordPeriod[0]);
        queue_player_word(1, SYS_AREA->slotWord[0] >> 8);
    }
    if (SYS_AREA->slot1WordRamping)
    {
        SYS_AREA->slot1WordRamping = ramp_step(&SYS_AREA->slotWord[1], &SYS_AREA->slotWordCount[1],
            SYS_AREA->slotWordStep[1], SYS_AREA->slotWordTarget[1], SYS_AREA->slotWordPeriod[1]);
        queue_player_word(2, SYS_AREA->slotWord[1] >> 8);
    }
}
