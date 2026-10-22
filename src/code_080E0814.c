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
void queue_display_entry(u32 kind, void *text, void *dst, u32 size, u32 a, u32 b);
void *task_push_list_50(void *next, void *obj);

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

void offset_three_words(u32 item, s32 *a, s32 *b, s32 *c, u32 d);

NONMATCH("asm/code_080E0814/build_iris_rows.s", void build_iris_rows(s32 x, s32 y, s32 z, u32 radius))
{
    u32 *out;
    u32 radiusSq;
    s32 row;

    if (gWorld->saved260 != 0)
    {
        offset_three_words(gWorld->item24C, &x, &y, &z, 0);
        out = (u32 *)(gWorkBuf + ((((((u32)gWorld->flags0B << 26) >> 31) * 5) << 7) + 0x3EF0));
        radiusSq = radius * radius;
        for (row = 0; row < 160; row++)
        {
            s32 dy = y - row;
            s32 half = radiusSq - dy * dy;
            s32 h;
            s32 left;
            s32 right;
            u32 v;

            if (half <= 0)
            {
                *out++ = 0;
                continue;
            }
            h = ((s32 (*)(s32))gMathRoutines)(half);
            right = x + h;
            if (right < 0)
            {
                *out++ = 0;
                continue;
            }
            if (right > 240)
            {
                right = 240;
            }
            left = x - h;
            if (left > 239)
            {
                *out++ = 0;
                continue;
            }
            if (left < 0)
            {
                *out++ = right | (right << 16);
            }
            else
            {
                v = (left << 8) | right;
                *out++ = v | (v << 16);
            }
        }
    }
}
END_NONMATCH

ASM_FUNC("asm/code_080E0814/scale_point_list.s", void scale_point_list(void))

ASM_FUNC("asm/code_080E0814/rotate_point_list_shifted.s", void rotate_point_list_shifted(void))

ASM_FUNC("asm/code_080E0814/rotate_point_list_y.s", void rotate_point_list_y(void))

ASM_FUNC("asm/code_080E0814/rotate_point_list.s", void rotate_point_list(void))

ASM_FUNC("asm/code_080E0814/rasterize_outline_rows.s", void rasterize_outline_rows(void))

void advance_path(struct StreamObj *obj)
{
    u16 *stream;
    u16 timer;

    if ((s16)obj->unk10 == 0)
    {
        do
        {
            stream = obj->stream;
            add_obj_target_offset(gWorld->item24C, (s16)stream[0] - (s16)obj->unk12, (s16)stream[1] - (s16)obj->unk14);
            obj->unk12 = stream[0];
            obj->unk14 = stream[1];
            obj->unk10 = stream[2];
            obj->stream = stream + 3;
        } while (obj->unk10 == 0);
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

void copy_bg_block_flipped(struct StreamObj *obj);
void show_frame_pair(u32 index)
{
    u32 *entryA;
    u32 *entryB;
    u32 *entry;
    s32 diff;
    u32 i2;
    u32 i3;
    u32 sel;
    s32 h;
    u8 *frame;
    void (*copy)(u8 *, u8 *, u32);
    void (*copy2)(u8 *, u8 *, u32);

    gWorld->tile26C = index;
    frame = (u8 *)((u32)gUnk_089F46B0 + *(u32 *)((u32)gUnk_089F46B0 + gTileHalfTable[index] * 4));
    queue_display_entry(2, frame, (void *)0x06004000, -1, 0, 0);
    i3 = (index + 3) << 1;
    entryA = (u32 *)((u32)gUnk_089F46B0 + *(u16 *)((u32)gTileHalfTable + i3) * 4);
    i2 = (index + 2) << 1;
    entryB = (u32 *)((u32)gUnk_089F46B0 + *(u16 *)((u32)gTileHalfTable + i2) * 4);
    diff = *entryA - *entryB;
    copy = MEM_ROUTINE(void (*)(u8 *, u8 *, u32), iwram_copy);
    copy(gUnk_02000180, gWorkBuf + 0x3DF0, diff);
    copy2 = MEM_ROUTINE(void (*)(u8 *, u8 *, u32), iwram_copy);
    sel = i2;
    if ((gSys[0x888] & 6) != 0)
    {
        sel = i3;
    }
    entry = (u32 *)((u32)gUnk_089F46B0 + *(u16 *)((u32)gTileHalfTable + sel) * 4);
    copy2((u8 *)((u32)gUnk_089F46B0 + *entry), gUnk_02000180, diff);
    h = diff;
    if (h < 0)
    {
        h += 0x1F;
    }
    ((struct SysHead *)gSys)->unk2 |= (0xFF00 >> (8 - (h >> 5))) & 0xFF00;
    task_push_list_50(copy_bg_block_flipped, 0);
}

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
    struct SysHead *sys;
    s32 amount;

    idx = gWorld->tile26C;
    entryA = (u32 *)((u32)gUnk_089F46B0 + gTileHalfTable[idx + 3] * 4);
    entryB = (u32 *)((u32)gUnk_089F46B0 + gTileHalfTable[idx + 2] * 4);
    span = *entryA;
    span -= *entryB;
    dst = gUnk_02000180;
    MEM_ROUTINE(void (*)(u8 *, u8 *, u32), iwram_copy)(gWorkBuf + 0x3DF0, dst, span);
    sys = (struct SysHead *)gSys;
    if (span < 0)
    {
        span += 0x1F;
    }
    amount = 8 - (span >> 5);
    ctrl = 0xFF00 >> amount;
    ctrl &= 0xFFFFFF00;
    sys->unk2 = ctrl | sys->unk2;
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

NONMATCH("asm/code_080E0814/copy_bg_block_flipped.s", void copy_bg_block_flipped(struct StreamObj *obj))
{
    struct World *world = gWorld;
    u32 *src;
    u32 *dst;
    u32 remaining;
    u32 flip;

    src = (u32 *)(gUnk_089F46B0 + *(u32 *)(gUnk_089F46B0 + (gTileHalfTable[world->tile26C + 1] << 2)));
    flip = 0x80008000;
    dst = (u32 *)0x0600F000;
    remaining = 0x200;
    do
    {
        dst[0] = src[0] + flip;
        dst[1] = src[1] + flip;
        dst[2] = src[2] + flip;
        dst[3] = src[3] + flip;
        src += 4;
        dst += 4;
        remaining -= 4;
    } while ((u16)remaining != 0);
    obj->state = (void (*)(void))(u32)remaining;
}
END_NONMATCH
