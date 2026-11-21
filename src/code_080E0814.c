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

/*
 * Turns the iris (circle window) effect on, unless it already is (saved260 holds the replaced IRQ
 * callback while it's on): saves the BLDCNT (gUnk_02000050) and WINOUT (gUnk_0200004A) shadows
 * in tile26C/tile26E and the type-2 callback (gSys + 0x8C4) in saved260, sets BLDCNT 0x3744,
 * WINOUT 0x3F3B and WIN0H (gUnk_02000040) 0, installs reset_display_step, clears the start of the
 * window table (gWorkBuf + 0x3EF0), fills BG tile 0x3FF at 0x06007FE0 with colour 0xE, stores
 * `mode` in gUnk_0200025C, sets bit 0x4000 of gSys's halfword 2 and selects table half 0
 * (flags0B bit 5). leave_special_mode undoes it.
 */
void enter_special_mode(u32 mode)
{
    u16 irisMode = mode;
    struct World *world = gWorld;
    u32 savedIrq = world->saved260;
    u8 *sys;

    if (savedIrq == 0)
    {
        world->tile26C = gUnk_02000050;
        world->tile26E = gUnk_0200004A;
        sys = gSys;
        world->saved260 = *(u32 *)(sys + 0x8C4);
        gUnk_02000050 = 0x3744;
        gUnk_0200004A = 0x3F3B;
        gUnk_02000040 = savedIrq; /* 0 */
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
        gUnk_0200025C = irisMode;
        ((struct SysHead *)sys)->unk2 |= 0x4000;
        ((struct WorldBit5 *)&gWorld->flags0B)->bit5 = 0;
    }
}

void leave_special_mode(void)
{
 struct World *world = gWorld;
 u32 saved = world->saved260;
 register u16 *ram asm("r1");
 struct DmaCtl0 *dma;
 if (saved != 0) {
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

void offset_three_words(u32 item, s32 *a, s32 *b, s32 *c, u32 d);

/*
 * Fills the window table of the iris (circle) effect for one frame. Only while the effect is on
 * (saved260 holds the IRQ callback enter_special_mode replaced): the world point (x, y, z) goes
 * through the camera to the screen, and for each of the 160 scanlines a u32 gets the circle's
 * span around it, as a window register value (left << 8 | right) in both halves; 0 hides the
 * line. The table is the half of gWorkBuf + 0x3EF0 that flags0B bit 5 selects (double buffered,
 * 160 * 4 bytes each).
 */
void build_iris_rows(s32 x, s32 y, s32 z, u32 radius)
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
            s32 half;
            s32 right;
            u32 span;

            /* half width of the circle on this row: sqrt(r^2 - dy^2) */
            half = y - row;
            half = half * half;
            half = radiusSq - half;
            if (half <= 0)
            {
                *out++ = 0;
                continue;
            }
            half = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(half);
            right = x + half;
            if (right < 0)
            {
                *out++ = 0;
                continue;
            }
            if (right > 240)
            {
                right = 240;
            }
            half = x - half; /* the left edge */
            if (half > 239)
            {
                *out++ = 0;
                continue;
            }
            if (half < 0)
            {
                *out++ = right | (right << 16);
            }
            else
            {
                span = (half << 8) | right;
                *out++ = span | (span << 16);
            }
        }
    }
}

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
/* Shows picture `index` of the picture archive gUnk_089F46B0 (u32 offsets to its entries, which
   gTileHalfTable numbers): entry [index] is the tile graphics, queued to BG VRAM at 0x06004000;
   [index+2] and [index+3] are two palettes of the same size (the second one when gSys[0x888] & 6),
   loaded into BG palette banks 8 and up (gUnk_02000180) after the old colours are saved to
   gWorkBuf + 0x3DF0, with those banks marked for upload; copy_bg_block_flipped then writes the
   tilemap [index+1]. */
void show_frame_pair(u32 index)
{
    u32 *palEndOfs;
    u32 *palOfs;
    u32 *srcPalOfs;
    s32 palSize;
    u32 palEntry;
    u32 altPalEntry;
    u32 srcEntry;
    s32 roundedSize;
    u8 *tiles;
    void (*saveCopy)(u8 *, u8 *, u32);
    void (*loadCopy)(u8 *, u8 *, u32);

    gWorld->tile26C = index;
    tiles = (u8 *)((u32)gUnk_089F46B0 + *(u32 *)((u32)gUnk_089F46B0 + gTileHalfTable[index] * 4));
    queue_display_entry(2, tiles, (void *)0x06004000, -1, 0, 0);
    /* byte offsets into gTileHalfTable of entries index+3 and index+2 */
    altPalEntry = (index + 3) << 1;
    palEndOfs = (u32 *)((u32)gUnk_089F46B0 + *(u16 *)((u32)gTileHalfTable + altPalEntry) * 4);
    palEntry = (index + 2) << 1;
    palOfs = (u32 *)((u32)gUnk_089F46B0 + *(u16 *)((u32)gTileHalfTable + palEntry) * 4);
    palSize = *palEndOfs - *palOfs;
    saveCopy = MEM_ROUTINE(void (*)(u8 *, u8 *, u32), iwram_copy);
    saveCopy(gUnk_02000180, gWorkBuf + 0x3DF0, palSize);
    loadCopy = MEM_ROUTINE(void (*)(u8 *, u8 *, u32), iwram_copy);
    srcEntry = palEntry;
    if ((gSys[0x888] & 6) != 0)
    {
        srcEntry = altPalEntry;
    }
    srcPalOfs = (u32 *)((u32)gUnk_089F46B0 + *(u16 *)((u32)gTileHalfTable + srcEntry) * 4);
    loadCopy((u8 *)((u32)gUnk_089F46B0 + *srcPalOfs), gUnk_02000180, palSize);
    /* one dirty bit per 32-byte palette bank, from bank 8 up */
    roundedSize = palSize;
    if (roundedSize < 0)
    {
        roundedSize += 0x1F;
    }
    ((struct SysHead *)gSys)->unk2 |= (0xFF00 >> (8 - (roundedSize >> 5))) & 0xFF00;
    task_push_list_50(copy_bg_block_flipped, 0);
}

/* Takes down the picture show_frame_pair put up (gWorld->tile26C): copies the colours it saved at
   gWorkBuf + 0x3DF0 back into BG palette banks 8 and up (gUnk_02000180), marks those banks for
   upload, stops the scroll task and zeroes the scroll offsets (gUnk_02000018/1A), and clears the
   picture's tilemap at 0x0600F000. */
void frame_copy_fade_clear(void)
{
    s32 palSize;
    u32 index;
    u32 *palEndOfs;
    u32 *palOfs;
    u8 *bgPal;
    struct ScrollBlock *scroll;
    u16 *scrollOfs;
    struct SysHead *sys;

    index = gWorld->tile26C;
    palEndOfs = (u32 *)((u32)gUnk_089F46B0 + gTileHalfTable[index + 3] * 4);
    palOfs = (u32 *)((u32)gUnk_089F46B0 + gTileHalfTable[index + 2] * 4);
    palSize = *palEndOfs;
    palSize -= *palOfs;
    bgPal = gUnk_02000180;
    MEM_ROUTINE(void (*)(u8 *, u8 *, u32), iwram_copy)(gWorkBuf + 0x3DF0, bgPal, palSize);
    sys = (struct SysHead *)gSys;
    /* one dirty bit per 32-byte palette bank, from bank 8 up */
    if (palSize < 0)
    {
        palSize += 0x1F;
    }
    sys->unk2 |= (0xFF00 >> (8 - (palSize >> 5))) & ~0xFF;
    scroll = gWorld->scrollBlock;
    if (scroll != 0)
    {
        scroll->unk4 = 0;
        gWorld->scrollBlock = 0;
        scrollOfs = &gUnk_02000018;
        *scrollOfs = 0;
        scrollOfs += 1;
        *scrollOfs = 0;
    }
    dma_fill_or_queue(0, (void *)0x0600F000, 0x800, 0x20, 0);
}

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

void copy_bg_block_flipped(struct StreamObj *obj)
{
 u32 tile = gWorld->tile26C;
 register u32 *src asm("r0");
 u32 *dst;
 register u32 remaining asm("r4");
 register u32 flip asm("r3");
 u32 offset = *(u32 *)(gUnk_089F46B0 + (gTileHalfTable[tile + 1] << 2));
 flip = 0x80008000;
 src = (u32 *)(gUnk_089F46B0 + offset);
 dst = (u32 *)0x0600F000;
 remaining = 0x200;
loop:
 dst[0] = src[0] + flip;
 dst[1] = src[1] + flip;
 dst[2] = src[2] + flip;
 dst[3] = src[3] + flip;
 src += 4;
 dst += 4;
 remaining -= 4;
 if (remaining != 0) goto loop;
 obj->state = (void (*)(void))remaining;
}
