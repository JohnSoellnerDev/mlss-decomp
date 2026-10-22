#include "global.h"
#include "ram_routines.h"

extern u8 gSys[];
extern u32 gUnk_03000014[];
extern u8 gUnk_02000480[];
extern u8 gUnk_03000374[];
extern u8 gUnk_02000C80[];
extern u8 gUnk_02000C8C[];
extern u8 gUnk_03001BD8[];
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

struct DmaQueueEntry
{
    u32 unk0;
    u32 dst;
    u32 unk8;
    u32 unkC;
};

struct SysArea
{
    u16 unk0;
    u16 unk2;
    struct DmaQueueEntry *volatile dmaQueue;
    vu8 dmaCount;
    u8 filler9[0x20 - 9];
    u32 frameCount;
    u32 playFrames;
    u8 filler28[0x884 - 0x28];
    u32 palMask;
    u8 sysFlags;
    u8 tickFlags;
    u8 filler88A[0x88C - 0x88A];
    u8 busy;
    u8 rampFlags;
    u8 filler88E[0x890 - 0x88E];
    u16 unk890;
    u8 filler892[0x89C - 0x892];
    u16 unk89C;
    u16 unk89E;
    u8 filler8A0[0x8A4 - 0x8A0];
    u16 unk8A4;
    u16 unk8A6;
    u8 filler8A8[0x8C1 - 0x8A8];
    u8 vblankCount;
    u8 filler8C2[0x8C4 - 0x8C2];
    u32 irqCallback;
};

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

struct KeyRecord
{
    u16 held;
    u16 pressed;
    u16 released;
    u16 fired;
    u8 delay;
    u8 interval;
    u8 counters[8];
};

struct HeapBlock
{
    struct HeapBlock *prev;
    struct HeapBlock *next;
    u32 size;
    u8 tag[4];
};

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

ASM_FUNC("asm/code_08017A00/agb_main.s", void agb_main(void))

ASM_FUNC("asm/code_08017A00/frame_update.s", void frame_update(void))

NONMATCH("asm/code_08017A00/vblank_irq_callback.s", void vblank_irq_callback(void))
{
    struct DmaRegs *dma3;
    u8 flags;
    u32 off;

    serial_link_update();
    if ((((struct SysArea *)gSys)->busy & 6) == 0)
        flag_88c_handler_b();
    copy_shadow_to_io();
    flags = ((struct SysArea *)gSys)->sysFlags;
    if ((flags & 0x40) == 0)
    {
        off = (((((u32)flags << 28) >> 31) ^ (((u32)flags << 26) >> 31)) << 10);
        dma3 = (struct DmaRegs *)0x040000D4;
        dma3->src = (u32)gSys + 0x80 + off;
        dma3->dst = 0x07000000;
        dma3->ctl = 0x84000100;
        (void)dma3->ctl;
    }
    else
    {
        off = (((((u32)flags << 28) >> 31) ^ (((u32)flags << 26) >> 31)) << 10);
        MEM_ROUTINE(void (*)(u8 *, u8 *, u32), iwram_copy)((u8 *)gSys + 0x80 + off, (u8 *)0x07000000, 0x400);
    }
    flush_palette_blocks();
    dma_queue_drain();
    ((void (*)(void))((struct SysArea *)gSys)->irqCallback)();
    flag_88c_handler_a();
    ((struct SysArea *)gSys)->vblankCount++;
    ((struct SysArea *)gSys)->frameCount++;
    REG_IME = 0;
    *(u16 *)0x03007FF8 |= 1;
    REG_IME = 1;
}
END_NONMATCH


void flush_palette_blocks(void)
{
    struct DmaRegs *dma3;
    u32 bits;
    u8 *src;
    u8 *dst;

    if (((struct SysArea *)gSys)->palMask == 0)
        return;
    bits = ((u32)((struct SysArea *)gSys)->unk0 << 16) | ((struct SysArea *)gSys)->unk2;
    bits &= ((struct SysArea *)gSys)->palMask;
    if ((((struct SysArea *)gSys)->sysFlags & 0x40) == 0)
    {
        src = gUnk_02000080;
        dst = (u8 *)0x05000000;
        if (bits != 0)
        {
            do
            {
                if ((bits & 1) != 0)
                {
                    dma3 = (struct DmaRegs *)0x040000D4;
                    dma3->src = (u32)src;
                    dma3->dst = (u32)dst;
                    dma3->ctl = 0x84000008;
                    (void)dma3->ctl;
                }
                src += 0x20;
                dst += 0x20;
                bits >>= 1;
            } while (bits != 0);
        }
    }
    else
    {
        src = gUnk_02000080;
        dst = (u8 *)0x05000000;
        if (bits != 0)
        {
            do
            {
                if ((bits & 1) != 0)
                    cpu_fast_set(src, dst, 8);
                src += 0x20;
                dst += 0x20;
                bits >>= 1;
            } while (bits != 0);
        }
    }
    ((struct SysArea *)gSys)->unk2 = 0;
    ((struct SysArea *)gSys)->unk0 = 0;
}

ASM_FUNC("asm/code_08017A00/dma_copy_or_queue.s", void dma_copy_or_queue(void))

ASM_FUNC("asm/code_08017A00/dma_fill_or_queue.s", void dma_fill_or_queue(void))

ASM_FUNC("asm/code_08017A00/dma_cancel_request.s", void dma_cancel_request(void))
ASM_FUNC("asm/code_08017A00/dma_queue_drain.s", void dma_queue_drain(void))

ASM_FUNC("asm/code_08017A00/copy_shadow_to_io.s", void copy_shadow_to_io(void))


ASM_FUNC("asm/code_08017A00/poll_keys.s", void poll_keys(void))

void update_key_state(u32 keys, u32 slot)
{
    struct KeyRecord *rec;
    const u16 *tbl;
    u32 k;
    u32 old;
    u32 held;
    u32 fired;
    u32 i;
    u32 hit;
    u32 remaining;

    rec = (struct KeyRecord *)(gUnk_03000374 + slot * 20);
    k = (keys << 22) >> 22;
    old = rec->held;
    rec->pressed = k & ~old;
    rec->released = old & ~k;
    fired = 0;
    held = rec->held;
    tbl = gKeyRepeatMaskPairs;
    for (i = 0; i < 8; i++)
    {
        hit = tbl[i] & k;
        if (hit != 0)
        {
            if ((held & hit) == 0)
            {
                fired |= hit;
                rec->counters[i] = rec->delay;
            }
            else
            {
                remaining = rec->counters[i] - 1;
                rec->counters[i] = remaining;
                if ((remaining << 24) == 0)
                {
                    fired |= hit;
                    rec->counters[i] = rec->interval;
                }
            }
        }
        else
            rec->counters[i] = hit;
    }
    rec->fired = fired;
    rec->held = k;
}

NONMATCH("asm/code_08017A00/dma_stop_and_reset.s", void dma_stop_and_reset(void))
{
    struct DmaRegs *dma;
    s32 i;
    u8 f;

    f = ((struct SysArea *)gSys)->sysFlags;
    f |= 1;
    ((struct SysArea *)gSys)->sysFlags = f;
    dma = (struct DmaRegs *)0x040000B0;
    for (i = 0; i < 4; i++)
    {
        ((vu16 *)&dma->ctl)[1] = ((vu16 *)&dma->ctl)[1] & 0xC5FF;
        ((vu16 *)&dma->ctl)[1] = ((vu16 *)&dma->ctl)[1] & 0x7FFF;
        (void)((vu16 *)&dma->ctl)[1];
        dma++;
    }
    sub_081DB79C(0xFC);
}
END_NONMATCH

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

NONMATCH("asm/code_08017A00/oam_affine_identity_init.s", void oam_affine_identity_init(void))
{
    u16 *base;
    u16 *q;
    s32 n;
    u32 k;
    u32 off;
    u16 v;

    off = (((u32)((struct SysArea *)gSys)->sysFlags << 28) >> 31) << 10;
    base = (u16 *)((u8 *)gSys + 0x80 + off);
    v = 0x100;
    for (n = 0x20; n != 0; n -= 8)
    {
        q = base;
        for (k = 0; k < 8; k++)
        {
            q[3] = v;
            q[7] = 0;
            q[11] = 0;
            q[15] = v;
            q += 16;
        }
        base += 0x80;
    }
}
END_NONMATCH

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

NONMATCH("asm/code_08017A00/heap_free.s", void heap_free(void *p))
{
    const u8 *src;
    struct HeapBlock *blk = (struct HeapBlock *)p - 1;
    struct HeapBlock *prev;
    struct HeapBlock *next;
    struct HeapBlock *link;

    if (p == 0)
        return;
    prev = blk->prev;
    next = blk->next;
    blk->size = blk->size | 1;
    src = gHeapTags + 0x40;
    ((u8 *)p)[-4] = src[0];
    ((u8 *)p)[-3] = src[1];
    ((u8 *)p)[-2] = src[2];
    ((u8 *)p)[-1] = src[3];
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
END_NONMATCH

NONMATCH("asm/code_08017A00/heap_alloc_raw.s", void *heap_alloc_raw(u32 pool, u32 size, const u8 *tag))
{
    struct HeapBlock *cur;
    struct HeapBlock *rest;
    struct HeapBlock *next;
    u32 needed;
    u32 avail;
    u32 flags;
    u32 k;

    if (size == 0)
        return 0;
    if (pool != 0)
        cur = (struct HeapBlock *)gUnk_02000C80;
    else
        cur = (struct HeapBlock *)gUnk_03001BD8;
    size = (size + 3) & ~3;
    needed = size + 0x10;
    do
    {
        flags = cur->size;
        if ((flags & 1) != 0)
        {
            if (flags >= needed)
            {
                avail = flags & ~1;
                if (avail > size + 0x20)
                {
                    rest = (struct HeapBlock *)((u8 *)cur + needed);
                    rest->size = (avail - 0x10 - size) | 1;
                    rest->prev = cur;
                    next = cur->next;
                    rest->next = next;
                    for (k = 0; k < 4; k++)
                        rest->tag[k] = gHeapTags[0x40 + k];
                    if (next != 0)
                        next->prev = rest;
                    cur->size = needed;
                    cur->next = rest;
                }
                else
                {
                    cur->size = avail;
                }
                for (k = 0; k < 4; k++)
                    cur->tag[k] = tag[k];
                return (u8 *)cur + 0x10;
            }
        }
        cur = cur->next;
    } while (cur != 0);
    return 0;
}
END_NONMATCH


void heap_free_a(void *p)
{
    heap_free(p);
}

void heap_free_b(void *p)
{
    heap_free(p);
}

NONMATCH("asm/code_08017A00/heap_alloc.s", void *heap_alloc(u32 size, u32 pool, const u8 *tag, u8 clear))
{
    u32 zero;
    void *p;
    p = heap_alloc_raw(pool, size, tag);
    if (clear != 0)
    {
        zero = 0;
        cpu_set(&zero, p, ((size << 9) >> 11) | 0x05000000);
    }
    return p;
}
END_NONMATCH

NONMATCH("asm/code_08017A00/heap_alloc2.s", void *heap_alloc2(u32 size, u32 pool, const u8 *tag, u8 clear))
{
    void *p = heap_alloc_raw(pool, size, tag);
    if (clear != 0)
    {
        u32 zero = 0;
        cpu_set(&zero, p, ((size << 9) >> 11) | 0x05000000);
    }
    return p;
}
END_NONMATCH

void init_sys_defaults(void)
{
    u8 f;
    ((struct SysArea *)gSys)->unk890 = 0xFF00;
    ((struct SysArea *)gSys)->unk89C = 0xFF00;
    ((struct SysArea *)gSys)->unk89E = 0xFF00;
    ((struct SysArea *)gSys)->unk8A4 = 0x4B00;
    ((struct SysArea *)gSys)->unk8A6 = 0x4B00;
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

ASM_FUNC("asm/code_08017A00/update_meter_ramps.s", void update_meter_ramps(void))
