#include "global.h"
#include "ram_routines.h"

struct ScrollSlot
{
    u8 filler0[0x10];
    u16 unk10;
    s16 unk12;
    u8 filler14[2];
    u16 unk16;
};

struct SlotSource
{
    u8 filler0[0x10];
    u16 unk10;
};

struct StripObj
{
    u8 filler0[0x4C];
    u8 *dst;
};

struct ObjParams
{
    u8 filler0[8];
    u32 unk8;
    u8 filler0C[4];
    u16 unk10;
    u16 unk12;
    u16 unk14;
    u16 unk16;
    u16 unk18;
};

struct ObjParamSrc
{
    s16 a;
    s16 b;
    s16 c;
};

struct ObjStripRec
{
    u8 filler0[4];
    u32 unk4;
    struct StripRec *rec;
};

struct StripRec
{
    s16 count;
    s16 first;
    s16 second;
};

struct StreamObj
{
    u8 filler0[4];
    void (*state)(void);
    u8 filler8[4];
    u16 *stream;
    u16 unk10;
    u16 unk12;
    u16 unk14;
};

struct World
{
    u8 filler0[0xB];
    u8 flags0B;
    u8 filler0C;
    u8 flags;
    u8 filler0E[0x24C - 0xE];
    u32 item24C;
    u8 filler250[0x260 - 0x250];
    u32 saved260;
    u8 filler264[0x26C - 0x264];
    u16 tile26C;
    u16 tile26E;
    u8 count270;
    u8 filler271[0x274 - 0x271];
    struct ScrollBlock *scrollBlock;
};

struct SysHead
{
    u8 filler0[2];
    u16 unk2;
};

struct RecXYZ
{
    s32 x;
    s32 y;
    s32 z;
};

struct ScrollBlock
{
    u8 filler0[4];
    u32 unk4;
    u8 filler8[8];
    u16 unk10;
    u16 unk12;
    u16 unk14;
    u8 filler16[2];
    u16 unk18;
};

extern struct World *gWorld;
extern const u16 gTileHalfTable[];
extern const u8 gUnk_089F46B0[];
extern u16 gUnk_02000018;
extern u16 gUnk_0200001A;
extern u16 gUnk_02000050;
extern u16 gUnk_0200004A;
extern u16 gUnk_02000040;
extern u8 *gWorkBuf;
extern u16 gUnk_0200025C;
extern u8 gSys[];
extern u8 gUnk_02000180[];
void dma_fill_or_queue(u32 value, void *dst, u32 len, u32 unit, u32 flags);

void reset_display_step(void);
void add_obj_target_offset(u32 item, s32 a, s32 b);
void update_scroll_shadows(void *task);
void *task_create_list_40(void (*entry)(void *));
void set_irq_callback(u32 kind, void *cb);
u32 mt19937_next(void);
void dma_win0_from_buffer(void);
void *heap_alloc_raw(u32 pool, u32 size, const u8 *tag);
extern const u8 gTagYbtb[];

ASM_FUNC("asm/code_080E0814/unknown_thumb_080e0814.s", void unknown_thumb_080e0814(void))

struct DmaCtl0
{
    u8 filler0[0xA];
    u16 cnt_h;
};

struct WorldBit1
{
    u8 bit0 : 1;
    u8 bit1 : 1;
    u8 hi6 : 6;
};

struct WorldBit5
{
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 hi2 : 2;
};

void enter_special_mode(u32 mode)
{
    u16 m = mode;
    struct World *world = gWorld;
    u32 saved = world->saved260;
    u8 *base;

    if (saved == 0)
    {
        world->tile26C = gUnk_02000050;
        world->tile26E = gUnk_0200004A;
        base = gSys;
        world->saved260 = *(u32 *)(base + 0x8C4);
        gUnk_02000050 = 0x3744;
        gUnk_0200004A = 0x3F3B;
        gUnk_02000040 = saved;
        set_irq_callback(2, reset_display_step);
        ((void (*)(u32, u8 *, u32))gMemRoutines)(0, gWorkBuf + 0x3EF0, 4);
        *(u32 *)0x06007FFC = 0xEEEEEEEE;
        *(u32 *)0x06007FF8 = 0xEEEEEEEE;
        *(u32 *)0x06007FF4 = 0xEEEEEEEE;
        *(u32 *)0x06007FF0 = 0xEEEEEEEE;
        *(u32 *)0x06007FEC = 0xEEEEEEEE;
        *(u32 *)0x06007FE8 = 0xEEEEEEEE;
        *(u32 *)0x06007FE4 = 0xEEEEEEEE;
        *(u32 *)0x06007FE0 = 0xEEEEEEEE;
        gUnk_0200025C = m;
        ((struct SysHead *)base)->unk2 |= 0x4000;
        ((struct WorldBit5 *)&gWorld->flags0B)->bit5 = 0;
    }
}

NONMATCH("asm/code_080E0814/leave_special_mode.s", void leave_special_mode(void))
{
    struct World *world = gWorld;
    u32 saved = world->saved260;
    u16 *ram;
    struct DmaCtl0 *dma;

    if (saved != 0)
    {
        ram = &gUnk_02000050;
        *ram = world->tile26C;
        ram -= 3;
        *ram = world->tile26E;
        ram -= 5;
        *ram = 0xF0;
        set_irq_callback(2, (void *)saved);
        gWorld->tile26C = 0;
        gWorld->tile26E = 0;
        gWorld->saved260 = 0;
        ((void (*)(u32, u8 *, u32))gMemRoutines)(0, (u8 *)0x0600F000, 0x500);
        dma = (struct DmaCtl0 *)0x040000B0;
        dma->cnt_h = *(vu16 *)&dma->cnt_h & 0xC5FF;
        dma->cnt_h = *(vu16 *)&dma->cnt_h & 0x7FFF;
        *(vu16 *)&dma->cnt_h;
    }
}
END_NONMATCH

ASM_FUNC("asm/code_080E0814/build_iris_rows.s", void build_iris_rows(void))

ASM_FUNC("asm/code_080E0814/scale_point_list.s", void scale_point_list(void))

ASM_FUNC("asm/code_080E0814/rotate_point_list_shifted.s", void rotate_point_list_shifted(void))

ASM_FUNC("asm/code_080E0814/rotate_point_list_y.s", void rotate_point_list_y(void))

ASM_FUNC("asm/code_080E0814/rotate_point_list.s", void rotate_point_list(void))

ASM_FUNC("asm/code_080E0814/rasterize_outline_rows.s", void rasterize_outline_rows(void))

NONMATCH("asm/code_080E0814/advance_path.s", void advance_path(struct StreamObj *obj))
{
    u16 *stream;
    u16 delay;
    u16 timer;

    if ((s16)obj->unk10 == 0)
    {
        do
        {
            stream = obj->stream;
            add_obj_target_offset(gWorld->item24C, (s16)stream[0] - (s16)obj->unk12, (s16)stream[1] - (s16)obj->unk14);
            obj->unk12 = stream[0];
            obj->unk14 = stream[1];
            delay = stream[2];
            obj->unk10 = delay;
            obj->stream = stream + 3;
        } while (delay == 0);
    }
    timer = obj->unk10;
    if ((s16)obj->unk10 < 0)
    {
        add_obj_target_offset(gWorld->item24C, -(s16)obj->unk12, -(s16)obj->unk14);
        ((struct WorldBit1 *)&gWorld->flags)->bit1 = 0;
        obj->state = 0;
    }
    else
    {
        obj->unk10 = timer - 1;
    }
}
END_NONMATCH

void copy_bg_block_flipped(struct StreamObj *obj);
ASM_FUNC("asm/code_080E0814/show_frame_pair.s", void show_frame_pair(void))

NONMATCH("asm/code_080E0814/frame_copy_fade_clear.s", void frame_copy_fade_clear(void))
{
    s32 span;
    s32 ctrl;
    u32 idx;
    u32 *entryA;
    u32 *entryB;
    u8 *dst;
    struct ScrollBlock *block;
    u16 *ram;

    idx = gWorld->tile26C;
    entryA = (u32 *)((u32)gUnk_089F46B0 + gTileHalfTable[idx + 3] * 4);
    entryB = (u32 *)((u32)gUnk_089F46B0 + gTileHalfTable[idx + 2] * 4);
    span = *entryA;
    span -= *entryB;
    dst = gUnk_02000180;
    MEM_ROUTINE(void (*)(u8 *, u8 *, u32), iwram_copy)(gWorkBuf + 0x3DF0, dst, span);
    if (span < 0)
    {
        span += 0x1F;
    }
    ctrl = 0xFF00;
    ctrl >>= 8 - (span >> 5);
    ctrl &= 0xFFFFFF00;
    ((struct SysHead *)gSys)->unk2 = ctrl | ((struct SysHead *)gSys)->unk2;
    block = gWorld->scrollBlock;
    if (block != 0)
    {
        block->unk4 = 0;
        gWorld->scrollBlock = 0;
        ram = &gUnk_02000018;
        *ram = 0;
        ram += 1;
        *ram = 0;
    }
    dma_fill_or_queue(0, (void *)0x0600F000, 0x800, 0x20, 0);
}
END_NONMATCH

ASM_FUNC("asm/code_080E0814/unknown_thumb_080e16b8.s", void unknown_thumb_080e16b8(void))

void upload_strip_from_rec(struct StripObj *obj, struct StripRec *rec)
{
    u8 *dst;
    s32 count;
    s32 first;
    s32 second;

    if (obj == 0)
    {
        return;
    }
    dst = obj->dst;
    if (dst == 0)
    {
        return;
    }
    count = rec->count << 5;
    first = rec->first << 5;
    second = rec->second << 5;
    if (first < 0)
    {
        MEM_ROUTINE(void (*)(u32, u8 *, u32), iwram_fill)(0, dst + second, count);
    }
    else
    {
        MEM_ROUTINE(void (*)(u8 *, u8 *, u32), iwram_copy)(dst + first, dst + second, count);
    }
}

void upload_obj_strip(struct ObjStripRec *obj)
{
    struct StripRec *rec = obj->rec;
    s32 count;
    s32 first;
    s32 second;

    count = rec->count << 5;
    first = rec->first << 5;
    second = rec->second << 5;
    if (first < 0)
    {
        MEM_ROUTINE(void (*)(u32, u8 *, u32), iwram_fill)(0, (u8 *)(second + 0x06004000), count);
    }
    else
    {
        MEM_ROUTINE(void (*)(u8 *, u8 *, u32), iwram_copy)((u8 *)(first + 0x06004000), (u8 *)(second + 0x06004000), count);
    }
    obj->unk4 = 0;
}

void set_scroll_slot(s32 x, s32 y)
{
    struct ScrollBlock *block = gWorld->scrollBlock;
    if (block == 0)
    {
        block = task_create_list_40(update_scroll_shadows);
    }
    block->unk12 = x;
    block->unk14 = y;
    block->unk10 = gUnk_02000018 << 8;
    block->unk18 = gUnk_0200001A << 8;
    gWorld->scrollBlock = block;
}

ASM_FUNC("asm/code_080E0814/copy_bg_block_flipped.s", void copy_bg_block_flipped(struct StreamObj *obj))
