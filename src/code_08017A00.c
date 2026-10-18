/* agbcc: -funroll-loops */
#include "global.h"

extern u8 gSys[];
extern u32 gUnk_03000014[];
extern u8 gUnk_02000480[];
extern u8 gUnk_03000374[];
extern u8 gUnk_02000C80[];
extern u8 gUnk_03001BD8[];
extern const u16 gKeyRepeatMaskPairs[];
extern const u8 gHeapTags[];
extern const u32 gRomBootWords[];
void cpu_set(const void *src, void *dst, u32 ctl);
void cpu_fast_set(const void *src, void *dst, u32 ctl);
extern u8 gUnk_02000080[];
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

struct SysArea
{
    u8 filler0[0x24];
    u32 playFrames;
    u8 filler28[0x888 - 0x28];
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

ASM_FUNC("asm/code_08017A00/agb_main.s", void agb_main(void))

ASM_FUNC("asm/code_08017A00/frame_update.s", void frame_update(void))

ASM_FUNC("asm/code_08017A00/vblank_irq_callback.s", void vblank_irq_callback(void))

ASM_FUNC("asm/code_08017A00/flush_palette_blocks.s", void flush_palette_blocks(void))

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

ASM_FUNC("asm/code_08017A00/dma_stop_and_reset.s", void dma_stop_and_reset(void))

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

ASM_FUNC("asm/code_08017A00/oam_affine_identity_init.s", void oam_affine_identity_init(void))

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

ASM_FUNC("asm/code_08017A00/heap_free.s", void heap_free(void *p))

ASM_FUNC("asm/code_08017A00/heap_alloc_raw.s", void *heap_alloc_raw(u32 pool, u32 size, const u8 *tag))

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
    void *p = heap_alloc_raw(pool, size, tag);
    if (clear != 0)
    {
        u32 zero = 0;
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

ASM_FUNC("asm/code_08017A00/set_vcount_mode.s", void set_vcount_mode(u32 mode, u32 line))

ASM_FUNC("asm/code_08017A00/update_meter_ramps.s", void update_meter_ramps(void))
