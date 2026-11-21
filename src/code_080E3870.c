#include "global.h"
#include "ram_routines.h"

struct SpinTask
{
    u8 filler0[0x12];
    u16 unk12;
    s16 unk14;
    u16 unk16;
};

struct DmaRegs
{
    vu32 sad;
    vu32 dad;
    vu16 cntL;
    vu16 cntH;
};

struct FlagsB
{
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 hi2 : 2;
};

struct ObjA
{
    u8 filler0[0xA];
    u16 unkA;
};

struct WorldRef
{
    u8 filler0[0xB];
    u8 flagsB;
    u8 filler0C[0x248 - 0xC];
    void *entry248;
    u8 filler24C[4];
    void *unk250;
    u8 filler254[0x260 - 0x254];
    void *unk260; // VBlank callback saved while the HBlank scroll effect runs
    u8 filler264[0x2D8 - 0x264];
    struct ObjA *obj2D8;
};

struct PendingCtx
{
    u8 filler0[0xC];
    u16 pending[16]; /* owner of each OBJ palette: 0xFFFF free, 0x1FFF lent to place_layer_tile */
};

struct SysBlock
{
    s16 unk0;
    u16 unk2;
    u8 filler4[0x880];
    s32 unk884;
};

extern struct WorldRef *gWorld;
extern u8 gSys[];
extern u8 *gMemRoutines;
extern u8 gShadowShapes083a16c8[];
extern u8 *gWorkBuf;
// RAM copies of REG_BG0HOFS..REG_BG3VOFS (HOFS, VOFS per BG)
extern u16 gUnk_02000010;
extern u16 gUnk_02000012;
extern u16 gUnk_02000014;
extern u16 gUnk_02000016;
extern u16 gUnk_02000018;
extern u16 gUnk_0200001A;
extern u16 gUnk_0200001C;
extern u16 gUnk_0200001E;
void load_vram_tiles(void *p);
extern u8 rect_fill_routine[];
extern u8 var_block_routine[];
extern u8 gUnk_02000080[];
extern void *gUnk_03000F8C;
extern void *gUnk_03000F88;
extern void *gModelCtx;
extern u8 gUnk_03000F78[];
extern u8 gUnk_08000AF0[];
s32 is_state_busy(void);
u8 sub_obj_running(void *menu);
void queue_display_entry(u32 kind, void *text, void *dst, u32 size, u32 a, u32 b);
void cpu_set(const void *src, void *dst, u32 ctl);
void sub_08115F54(void *child, u32 a1, u32 a2, u32 a3, u32 a4, u32 a5, u32 a6, u32 a7);
void sub_080E377C(s32 n);
void set_irq_callback(u32 kind, void *cb);
void *task_create_list_4c(void (*fn)(struct SpinTask *), void *obj);
void copy_scroll_and_start_dma(void);
void zoom_spin_task(struct SpinTask *task);
struct SpinTask *sub_080E3870(s32 n);

struct SpinTask *sub_080E3870(s32 n)
{
    struct SpinTask *task;
    sub_080E377C(n);
    {
        void **slot = &gWorld->unk260;
        u8 *sys = gSys;
        *slot = *(void **)(sys + 0x8C4);
    }
    set_irq_callback(2, copy_scroll_and_start_dma);
    ((struct FlagsB *)&gWorld->flagsB)->bit5 = 0;
    task = task_create_list_4c(zoom_spin_task, 0);
    task->unk12 = (n > 0) ? 0 : 0x4000;
    task->unk14 = n;
    if (n < 0)
        task->unk16 = 0x43;
    else
        task->unk16 = 0x34;
    zoom_spin_task(task);
    return task;
}

// gWorkBuf + SCANLINE_SCROLL_TABLE: per-scanline BG0HOFS..BG3VOFS values (16 bytes a line, 160 lines),
// double buffered; flagsB bit 5 picks the half the HBlank DMA reads.
#define SCANLINE_SCROLL_TABLE 0x3EF0
#define SCROLL_LINE_SIZE 0x10

// Byte offset of the half of the scanline scroll table that flagsB bit 5 selects (0 or 0xA00).
static inline u32 scroll_bank_offset(void)
{
    u32 bank = ((u32)gWorld->flagsB << 26) >> 31;
    return ((bank << 2) + bank) << 9;
}

/* IRQ callback of the per-scanline scroll effect: runs the callback it replaced, copies line 0 of
 * the current scanline scroll table half into BG0HOFS..BG3VOFS and restarts DMA0 so that on every
 * HBlank it copies the next line's 16 bytes into the scroll registers. */
void copy_scroll_and_start_dma(void)
{
    void (*copy)(u8 *, u8 *, u32);
    u8 **workBuf;
    u8 *dma0;
    if (gWorld->unk260 != 0)
        ((void (*)(void))gWorld->unk260)();
    copy = MEM_ROUTINE(void (*)(u8 *, u8 *, u32), iwram_copy);
    workBuf = &gWorkBuf;
    copy(*workBuf + (scroll_bank_offset() + SCANLINE_SCROLL_TABLE), (u8 *)REG_ADDR_BG0HOFS, SCROLL_LINE_SIZE);
    dma0 = (u8 *)REG_ADDR_DMA0SAD;
    *(vu16 *)(dma0 + 0xA) = *(vu16 *)(dma0 + 0xA) & DMA_STOP_MASK;
    *(vu16 *)(dma0 + 0xA) = *(vu16 *)(dma0 + 0xA) & (u16)~DMA_ENABLE;
    (void)*(vu16 *)(dma0 + 0xA);
    *(vu32 *)dma0 = (u32)(gWorkBuf + (scroll_bank_offset() + SCANLINE_SCROLL_TABLE));
    *(vu32 *)(dma0 + 4) = REG_ADDR_BG0HOFS;
    *(vu32 *)(dma0 + 8) = ((u32)(DMA_ENABLE | DMA_START_HBLANK | DMA_32BIT | DMA_REPEAT | DMA_DEST_RELOAD) << 16)
                        | (SCROLL_LINE_SIZE / 4);
    (void)*(vu32 *)(dma0 + 8);
}

/* Ends the per-scanline scroll effect started by sub_080E3870: stops the HBlank
 * DMA0 into the scroll registers, restores the saved VBlank callback, reloads the
 * BG tiles and puts the RAM scroll copies (BG2 VOFS reset to 0) back in the registers. */
void apply_bg_scroll_shadows(void)
{
    u8 *dma0 = (u8 *)REG_ADDR_DMA0SAD;
    *(vu16 *)(dma0 + 0xA) = *(vu16 *)(dma0 + 0xA) & DMA_STOP_MASK;
    *(vu16 *)(dma0 + 0xA) = *(vu16 *)(dma0 + 0xA) & (u16)~DMA_ENABLE;
    (void)*(vu16 *)(dma0 + 0xA);
    set_irq_callback(2, gWorld->unk260);
    gUnk_0200001A = 0;
    load_vram_tiles(gWorld->unk250);
    gWorld->unk260 = 0;
    REG_BG0HOFS = gUnk_02000010;
    REG_BG1HOFS = gUnk_02000014;
    REG_BG2HOFS = gUnk_02000018;
    REG_BG3HOFS = gUnk_0200001C;
    REG_BG0VOFS = gUnk_02000012;
    REG_BG1VOFS = gUnk_02000016;
    REG_BG2VOFS = gUnk_0200001A;
    REG_BG3VOFS = gUnk_0200001E;
}

ASM_FUNC("asm/code_080E3870/zoom_spin_task.s", void zoom_spin_task(struct SpinTask *task))

struct SpinTask *unknown_thumb_080e40dc(s32 n)
{
    u32 obj = (u32)sub_080E3870(n);
    if (n < 0)
        ((struct SpinTask *)obj)->unk16 = 0x34;
    else
        ((struct SpinTask *)obj)->unk16 = 0x43;
    return (struct SpinTask *)obj;
}

void sub_080E40FC(void)
{
    queue_display_entry(2, gShadowShapes083a16c8, (void *)0x06004000, -1, 0, 0);
}

ASM_FUNC("asm/code_080E3870/place_layer_tile.s", void place_layer_tile(void))

/* Marks each of the 16 OBJ palettes in gUnk_03000F78: 0xFF (-1) if its owner is 0xFFFF (free) or
 * 0x1FFF (lent to place_layer_tile), 0xFE (-2) if something else uses it. */
void refresh_slot_flags(void)
{
    s32 palette = 0;
    u16 freeOwner = 0xFFFF;
    u16 lentOwner = 0x1FFF;

    for (; palette < 16; palette++)
    {
        u8 *mark = gUnk_03000F78 + palette;
        u16 owner = ((struct PendingCtx *)gModelCtx)->pending[palette];
        register s32 value asm("r1");
        register s32 out asm("r0");

        if (owner == freeOwner || owner == lentOwner)
            value = -1;
        else
            value = -2;
        out = value;
        *mark = (u8)out;
    }
}

u32 world_entry_ready(void)
{
    if ((u8)sub_obj_running(gWorld->entry248) != 0)
        return 1;
    return 0;
}

void run_obj_entry(void)
{
    struct ObjA *obj;
    u32 n;
    obj = gWorld->obj2D8;
    n = 0;
    if (obj != 0)
        n = obj->unkA;
    sub_08115F54(gWorld->entry248, 0, n, 0x10, 0xFFFF, 0xFFFF, 0, 0);
    {
        u8 *base = gSys;
        *(u32 *)(base + 0x884) = -1;
    }
}

void clear_obj_bg_vram(void)
{
    ((void (*)(u32, void *, u32))gMemRoutines)(0, (void *)0x06004000, 0x9000);
    ((void (*)(u32, void *, u32))gMemRoutines)(0, (void *)0x0600F000, 0x1000);
}

u32 poll_flag_bd0(void)
{
    u8 busy = is_state_busy();
    if (busy != 0)
        return 1;
    {
        u8 *base = gSys;
        *(u32 *)(base + 0x884) = busy;
    }
    return 0;
}

void clear_ewram_from_obj(void)
{
    struct ObjA *obj;
    u32 v;
    obj = gWorld->obj2D8;
    v = 0;
    if (obj != 0)
        v = obj->unkA;
    ((void (*)(u32, void *, u32))gMemRoutines)(v, gUnk_02000080, 0x200);
    v |= v << 16;
    ((void (*)(u32, void *, u32))gMemRoutines)(v, gUnk_02000080, 0x400);
    ((struct SysBlock *)gSys)->unk2 = 0xFFFF;
    ((struct SysBlock *)gSys)->unk0 = -1;
    ((struct SysBlock *)gSys)->unk884 = -1;
}

ASM_FUNC("asm/code_080E3870/unknown_thumb_080e467c.s", void unknown_thumb_080e467c(void))

ASM_FUNC("asm/code_080E3870/sub_080E4740.s", void sub_080E4740(void))

u8 *set_text_buffer(u8 *buf)
{
    u32 size;
    u8 *aligned;
    u8 *src;
    if (buf == 0)
    {
        gUnk_03000F8C = rect_fill_routine;
        return 0;
    }
    aligned = (u8 *)(((u32)buf + 3) & -4);
    src = rect_fill_routine;
    size = (u32)var_block_routine - (u32)src;
    cpu_set(src, aligned, (size << 10) >> 11);
    gUnk_03000F8C = aligned;
    return aligned + size;
}

u8 *unknown_thumb_080e4860(u8 *buf)
{
    u8 *aligned;
    u8 *src;
    u32 size;
    u32 off;

    if (buf == 0)
    {
        gUnk_03000F88 = gUnk_08000AF0 + 0x80;
        return 0;
    }
    aligned = (u8 *)(((u32)buf + 3) & -4);
    src = gUnk_08000AF0 + 0x80;
    size = (u32)var_block_routine - (u32)src;
    cpu_set(src, aligned, (size << 10) >> 11);
    gUnk_03000F88 = aligned;
    gUnk_03000F8C = aligned + (off = (u32)rect_fill_routine - (u32)src);
    return aligned + size;
}

ASM_FUNC("asm/code_080E3870/sub_080E48C0.s", void sub_080E48C0(void))

ASM_FUNC("asm/code_080E3870/sub_080E4B44.s", void sub_080E4B44(void))

ASM_FUNC("asm/code_080E3870/sub_080E4D98.s", void sub_080E4D98(void))
