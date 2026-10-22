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
    void *unk260;
    u8 filler264[0x2D8 - 0x264];
    struct ObjA *obj2D8;
};

struct PendingCtx
{
    u8 filler0[0xC];
    u16 pending[16];
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

static inline u32 scroll_bank_offset(void)
{
    u32 bank = ((u32)gWorld->flagsB << 26) >> 31;
    return ((bank << 2) + bank) << 9;
}

void copy_scroll_and_start_dma(void)
{
    void (*copy)(u8 *, u8 *, u32);
    u8 **bufp;
    u8 *dmaBase;
    if (gWorld->unk260 != 0)
        ((void (*)(void))gWorld->unk260)();
    copy = MEM_ROUTINE(void (*)(u8 *, u8 *, u32), iwram_copy);
    bufp = &gWorkBuf;
    copy(*bufp + (scroll_bank_offset() + 0x3EF0), (u8 *)REG_ADDR_BG0HOFS, 0x10);
    dmaBase = (u8 *)REG_ADDR_DMA0SAD;
    *(vu16 *)(dmaBase + 0xA) = *(vu16 *)(dmaBase + 0xA) & 0xC5FF;
    *(vu16 *)(dmaBase + 0xA) = *(vu16 *)(dmaBase + 0xA) & 0x7FFF;
    (void)*(vu16 *)(dmaBase + 0xA);
    *(vu32 *)dmaBase = (u32)(gWorkBuf + (scroll_bank_offset() + 0x3EF0));
    *(vu32 *)(dmaBase + 4) = REG_ADDR_BG0HOFS;
    *(vu32 *)(dmaBase + 8) = 0xA6600004;
    (void)*(vu32 *)(dmaBase + 8);
}

void apply_bg_scroll_shadows(void)
{
    u8 *dmaBase = (u8 *)REG_ADDR_DMA0SAD;
    *(vu16 *)(dmaBase + 0xA) = *(vu16 *)(dmaBase + 0xA) & 0xC5FF;
    *(vu16 *)(dmaBase + 0xA) = *(vu16 *)(dmaBase + 0xA) & 0x7FFF;
    (void)*(vu16 *)(dmaBase + 0xA);
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

NONMATCH("asm/code_080E3870/refresh_slot_flags.s", void refresh_slot_flags(void))
{
    s32 i = 0;
    s32 key1 = 0xFFFF;
    s32 key2 = 0x1FFF;
    for (; i < 16; i++)
    {
        u8 *slot = gUnk_03000F78 + i;
        u16 e = ((struct PendingCtx *)gModelCtx)->pending[i];
        s32 v;
        if (e == key1 || e == key2)
            v = -1;
        else
            v = -2;
        *slot = v;
    }
}
END_NONMATCH

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

NONMATCH("asm/code_080E3870/unknown_thumb_080e4860.s", u8 *unknown_thumb_080e4860(u8 *buf))
{
    u8 *aligned;
    u8 *src;
    u32 size;
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
    gUnk_03000F8C = aligned + ((u32)rect_fill_routine - (u32)src);
    return aligned + size;
}
END_NONMATCH

ASM_FUNC("asm/code_080E3870/sub_080E48C0.s", void sub_080E48C0(void))

ASM_FUNC("asm/code_080E3870/sub_080E4B44.s", void sub_080E4B44(void))

ASM_FUNC("asm/code_080E3870/sub_080E4D98.s", void sub_080E4D98(void))
