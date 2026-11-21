#include "global.h"
#include "ram_routines.h"

void heap_free(void *p);
void *heap_alloc_raw(u32 pool, u32 size, const u8 *tag);
void set_irq_callback(u32 kind, void *cb);
void start_dma0_line_copy(void);
void start_dma_window(void);
void cpu_set(const void *src, void *dst, u32 ctl);
void driver_tables_init(u8 *block, u32 *params);
u8 *set_text_buffer(u8 *buf);
extern const u8 gTagYwav[];
extern u8 *gUnk_03000F90;
extern u8 *gUnk_03000FA4;
extern void *gUnk_03000F9C;     /* the VBlank callback start_dma_window replaced (and calls first) */
extern void *gUnk_03000F94;     /* window line table: 2 banks of 160 lines x WIN0H/WIN1H (0x280 bytes each) */
extern s32 gUnk_03000FA0;       /* DMA channel (0-3) that streams the line table to the window registers */
extern u8 *gUnk_03000FB4;
extern u8 *gUnk_03000FB8;
extern u8 *gUnk_03000FBC;
extern const u8 gTextObjTag[];
extern const u8 gTextTableTag[];
extern const u8 gTextBlockTag[];
extern u8 gTextFrameTiles[];
void *heap_alloc2(u32 size, u32 a, const void *table, u32 c);
void *build_record_chain(void *table, u32 a, u32 b, u32 c);
void free_text_pair(void *pair, u32 mode);
void heap_free_b(void *p);
u32 sound_is_playing(u32 kind);
void sound_stop_song(u32 song);
extern const u16 gTextModeTable[];
extern u8 gSys[];
extern u8 gUnk_02000008[];
extern u8 gUnk_02000012[];
extern u8 gUnk_02000080[];
extern const u8 gRomPaletteStatus[];
void cpu_fast_set(const void *src, void *dst, u32 control);
extern u8 gUnk_02000280[];
extern const u8 gWindowBufTag[];
extern u16 gUnk_02000048;       /* shadow WININ, then WINOUT (copied to 0x04000048) */
extern u8 gUnk_02000044[];      /* shadow WIN0V, then WIN1V (copied to 0x04000044) */
extern u32 gUnk_03000F98;       /* bank of gUnk_03000F94 being drawn into (0 or 1); the other one is shown */
extern u8 menu_obj_routine[];
extern u8 tile_shift_copy[];
extern u8 iwram_overlay[];
extern u8 var_block_routine[];
extern u8 rect_fill_routine[];
extern const u8 gEngineBlockTag[];
extern u32 gDriverParams[];
extern s32 gUnk_03000FAC;
extern s32 gUnk_03000FB0;
extern u32 gUnk_03000FA8;

struct ListNode
{
    u8 unk0;
    u8 filler1[3];
    struct ListNode *unk4;
    void *unk8;
};

void sub_080E57D4(struct ListNode *head, void *key);

ASM_FUNC("asm/code_080E5590/sub_080E5590.s", void sub_080E5590(void))

void sub_080E5728(void)
{
    heap_free(gUnk_03000F90);
    gUnk_03000F90 = 0;
}

void sub_080E5740(void)
{
    ((void (*)(u32, void *, u32))gMemRoutines)(0, gUnk_03000F90 + 0x10, 0x40);
}

struct ListNode *sub_080E5760(struct ListNode *head, void *key)
{
    struct ListNode *cur;
    struct ListNode *node;
    struct ListNode *next;
    struct ListNode *prev;

    if (key == 0)
    {
        struct ListNode *all;

        all = head;
        if (all != 0)
        {
            do
            {
                next = all->unk4;
                heap_free(all);
                all = next;
            } while (all != 0);
        }
        return 0;
    }
    for (cur = head; cur != 0; cur = cur->unk4)
    {
        if (key == cur->unk8)
        {
            sub_080E57D4(head, cur);
        }
        else if (key == cur)
        {
            cur->unk0 = 0xFF;
        }
    }
    prev = 0;
    for (node = head; node != 0; node = next)
    {
        next = node->unk4;
        if ((s8)node->unk0 < 0)
        {
            if (prev != 0)
            {
                prev->unk4 = next;
            }
            else
            {
                head = next;
            }
            heap_free(node);
        }
    }
    return head;
}

void sub_080E57D4(struct ListNode *head, void *key)
{
    struct ListNode *node;

    for (node = head; node != 0; node = node->unk4)
    {
        if (key == node->unk8)
        {
            sub_080E57D4(head, node);
        }
        else if (key == node)
        {
            node->unk0 = 0xFF;
        }
    }
}

static inline void dma_ctrl_stop(u8 *base)
{
    *(vu16 *)(base + 0xA) &= 0xC5FF;
    *(vu16 *)(base + 0xA) &= 0x7FFF;
    *(vu16 *)(base + 0xA);
}

void stop_dma_channel(void)
{
    s32 mode;

    heap_free(gUnk_03000F94);
    gUnk_03000F94 = 0;
    mode = gUnk_03000FA0;
    switch (mode)
    {
        case 0:
            dma_ctrl_stop((u8 *)0x040000B0);
            break;
        case 1:
            dma_ctrl_stop((u8 *)0x040000BC);
            break;
        case 2:
            dma_ctrl_stop((u8 *)0x040000C8);
            break;
        case 3:
            dma_ctrl_stop((u8 *)0x040000D4);
            break;
    }
    set_irq_callback(2, gUnk_03000F9C);
    *(vu16 *)0x02000000 &= 0x9FFF;
}

static inline void dma_window_start(u8 *base)
{
    *(vu16 *)(base + 0xA) &= 0xC5FF;
    *(vu16 *)(base + 0xA) &= 0x7FFF;
    *(vu16 *)(base + 0xA);
    *(vu32 *)base = (u32)gUnk_03000F94 + ((gUnk_03000F98 * 5) << 7);
    *(vu32 *)(base + 4) = 0x04000040;
    *(vu32 *)(base + 8) = 0xA6600001;
    *(vu32 *)(base + 8);
}

/* VBlank callback of the scanline window effect: runs the callback it replaced, restarts the
   HBlank DMA on the chosen channel to stream the finished bank of the line table into
   WIN0H/WIN1H, writes line 0's entry itself, and flips to the other bank for the next frame. */
void start_dma_window(void)
{
    vu32 *winRegs;
    u32 bank;
    u32 lineTable;

    ((void (*)(void))gUnk_03000F9C)();
    switch (gUnk_03000FA0)
    {
        case 0:
            dma_window_start((u8 *)REG_ADDR_DMA0SAD);
            break;
        case 1:
            dma_window_start((u8 *)REG_ADDR_DMA1SAD);
            break;
        case 2:
            dma_window_start((u8 *)REG_ADDR_DMA2SAD);
            break;
        case 3:
            dma_window_start((u8 *)REG_ADDR_DMA3SAD);
            break;
    }
    winRegs = (vu32 *)REG_ADDR_WIN0H;
    bank = gUnk_03000F98;
    lineTable = (u32)gUnk_03000F94;
    *winRegs = *(u32 *)(((bank * 5) << 7) + lineTable);
    gUnk_03000F98 = bank ^ 1;
}

ASM_FUNC("asm/code_080E5590/clip_edges_to_lines.s", void clip_edges_to_lines(void))

NONMATCH("asm/code_080E5590/fill_iris_spans.s", void fill_iris_spans(s32 cx, s32 cy, s32 scale, s32 radius))
{
    s32 y;
    s32 radSq;
    s32 rcp;
    s32 dy;
    s32 dySq;
    s32 t;
    s32 frac;
    s32 h;
    s32 w;
    s32 half;
    s32 left;
    s32 right;
    u8 *line;

    line = (u8 *)gUnk_03000F94 + ((gUnk_03000F98 * 5) << 7);
    MEM_ROUTINE(void (*)(u32, void *, u32), iwram_fill)(0xFFFFFFFF, line, 0x280);
    radSq = radius * radius;
    rcp = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(0x40000000, radSq);
    for (y = 0; y < 160; y++)
    {
        dy = y - cy;
        dySq = dy * dy;
        if (dySq < radSq)
        {
            t = dySq * rcp;
            if (t < 0)
            {
                t += 0xFFFF;
            }
            frac = t >> 16;
            h = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(0x4000 - frac);
            w = scale * h;
            if (w < 0)
            {
                w += 0x7F;
            }
            half = w >> 7;
            left = cx - half;
            if (left <= 239)
            {
                if (left < 0)
                {
                    line[1] = 0;
                }
                else
                {
                    line[1] = left;
                }
                right = cx + half;
                if (right < 0)
                {
                    line[0] = 0;
                }
                if (right <= 239)
                {
                    line[0] = right;
                }
            }
        }
        line += 4;
    }
}
END_NONMATCH

/* Starts the scanline window effect unless it already runs (returns 0 then, 1 when started):
   turns on windows 0 and 1 in the shadow DISPCNT, shows `layers` inside both windows and the other
   layers outside, makes both windows full height, allocates the two-bank line table and hooks
   start_dma_window in as the VBlank callback (saving the old one). dmaChannel (clamped to 0-3) is
   the DMA channel that streams the table into WIN0H/WIN1H. */
u32 start_window_buffer(s32 dmaChannel, u32 layers)
{
    vu16 *winIn;
    vu16 *winV;
    u32 started;
    void *lineTable;
    u32 vblankCbOffset;

    lineTable = gUnk_03000F94;
    if (lineTable != 0)
    {
        started = 0;
    }
    else
    {
        *(vu16 *)0x02000000 |= DISPCNT_WIN0_ON | DISPCNT_WIN1_ON;
        winIn = (vu16 *)&gUnk_02000048;
        *winIn = (layers << 8) | layers;
        winIn++;
        *winIn = layers ^ 0x3F;
        winV = (vu16 *)gUnk_02000044;
        *winV = 0xA0;
        winV++;
        *winV = 0xA0;
        gUnk_03000F94 = heap_alloc_raw(1, 0xA0 << 3, gWindowBufTag);
        gUnk_03000F98 = (u32)lineTable;
        vblankCbOffset = 0x8C4;
        gUnk_03000F9C = *(void **)(gSys + vblankCbOffset);
        set_irq_callback(2, start_dma_window);
        if (dmaChannel > 3)
        {
            dmaChannel = 3;
        }
        if (dmaChannel < 0)
        {
            dmaChannel = 0;
        }
        gUnk_03000FA0 = dmaChannel;
        started = 1;
    }
    return started;
}

ASM_FUNC("asm/code_080E5590/unknown_thumb_080e5cc8.s", void unknown_thumb_080e5cc8(void))

void sub_080E5F18(void)
{
    heap_free(gUnk_03000FA4);
    set_irq_callback(2, 0);
}

void sub_080E5F34(s32 first, s32 last)
{
    if (first < 0)
    {
        first = 0;
    }
    else if (first > 3)
    {
        first = 3;
    }
    if (last < 0)
    {
        last = 0;
    }
    else if (last > 3)
    {
        last = 3;
    }
    if (last < first)
    {
        last = first;
    }
    gUnk_03000FAC = first;
    gUnk_03000FB0 = last - first + 1;
    gUnk_03000FA4 = heap_alloc_raw(1, (gUnk_03000FB0 * 5) << 8, gTagYwav);
    ((void (*)(u32, void *, u32))gMemRoutines)(0, gUnk_03000FA4, (gUnk_03000FB0 * 5) << 8);
    gUnk_03000FA8 = 0;
    set_irq_callback(2, start_dma0_line_copy);
}

struct DmaChannel
{
    vu32 sad;
    vu32 dad;
    vu32 cnt;
};

void start_dma0_line_copy(void)
{
    struct DmaChannel *dma = (struct DmaChannel *)0x040000B0;
    s32 rows4;
    u32 sad;
    u32 dad;

    *(vu16 *)((u8 *)dma + 0xA) &= 0xC5FF;
    *(vu16 *)((u8 *)dma + 0xA) &= 0x7FFF;
    *(vu16 *)((u8 *)dma + 0xA);
    sad = (u32)gUnk_03000FA4 + ((((rows4 = gUnk_03000FB0 << 2) + gUnk_03000FB0) << 7) * gUnk_03000FA8);
    dad = (gUnk_03000FAC << 2) + REG_ADDR_BG0HOFS;
    dma->sad = sad;
    dma->dad = dad;
    if (rows4 < 0)
    {
        rows4 += 3;
    }
    dma->cnt = ((rows4 >> 2) | 0xA6600000);
    dma->cnt;
    gUnk_03000FA8 ^= 1;
}

void free_engine_block(void)
{
    heap_free(gUnk_03000FB4);
}

void load_engine_block(void)
{
    u32 a;
    u32 b;
    u32 size;
    u8 **block;

    block = &gUnk_03000FB4;
    a = (u32)menu_obj_routine - (u32)tile_shift_copy;
    b = a + (0x0819A4F0 - (u32)iwram_overlay);
    size = b + ((u32)var_block_routine - (u32)rect_fill_routine);
    *block = heap_alloc_raw(0, size, gEngineBlockTag);
    cpu_set(tile_shift_copy, *block, ((a << 9) >> 11) | 0x04000000);
    driver_tables_init(*block + a, gDriverParams);
    set_text_buffer(*block + b);
}

struct TextScratch
{
    u8 filler0[0x28];
    u16 unk28;
    u16 unk2A;
    u16 unk2C;
    u16 unk2E;
    u8 filler30[2];
    u8 unk32;
};

struct TextObj
{
    u8 filler0[0x17C];
    u8 *textBlock;
    void *frameTiles;
    u8 filler184[0x194 - 0x184];
    u32 bufferOffset;   /* added to an address in the record region to get its copy in textBlock */
    u8 filler198[0x1B2 - 0x198];
    u8 flags1B2;
    u8 filler1B3[0x1BC - 0x1B3];
};

struct TextHolder
{
    void *records;
    struct TextObj *obj;
};

/* Sets up a text object: clears both 0x34-byte text states (gUnk_03000FB8 gets its defaults),
   allocates and clears the TextObj, builds a chain of recordCount records over the region
   [regionBase, regionBase + regionSize), and allocates textBlock, a 32-byte aligned buffer of
   regionSize bytes that mirrors the region (bufferOffset converts region addresses to it). */
struct TextHolder *init_text_obj(struct TextHolder *holder, u32 regionBase, u32 regionSize, u32 recordCount)
{
    struct TextScratch *state;
    void *table;
    u32 block;
    u32 aligned;

    ((void (*)(u32, void *, u32))gMemRoutines)(0, gUnk_03000FB8, 0x34);
    ((void (*)(u32, void *, u32))gMemRoutines)(0, gUnk_03000FBC, 0x34);
    state = (struct TextScratch *)gUnk_03000FB8;
    state->unk28 = 1;
    state->unk2A = 1;
    state->unk2C = 2;
    state->unk2E = 1;
    state->unk32 = 1;
    holder->obj = heap_alloc_raw(1, 0x1BC, gTextObjTag);
    ((void (*)(u32, void *, u32))gMemRoutines)(0, holder->obj, 0x1BC);
    table = heap_alloc2(0x10, 1, gTextTableTag, 0);
    holder->records = build_record_chain(table, regionBase, regionSize, recordCount);
    block = (u32)heap_alloc_raw(1, regionSize + 0x20, gTextBlockTag);
    holder->obj->textBlock = (u8 *)block;
    aligned = (block + 0x1F) & 0xFFFFFFE0;
    holder->obj->bufferOffset = aligned - regionBase;
    holder->obj->frameTiles = gTextFrameTiles;
    return holder;
}

static inline void release_part_sound(u8 *part)
{
    if ((u8)sound_is_playing(gTextModeTable[*part >> 5]))
    {
        sound_stop_song(gTextModeTable[*part >> 5]);
    }
}

NONMATCH("asm/code_080E5590/release_obj_parts.s", void release_obj_parts(struct TextHolder *holder, u32 keep))
{
    struct TextObj *obj;
    u8 *base;
    s32 index;

    if (holder->records != 0)
    {
        free_text_pair(holder->records, 3);
    }
    holder->records = 0;
    obj = holder->obj;
    if (obj != 0)
    {
        index = 0;
        base = (u8 *)obj;
        for (; index < 5; index++, base += 0x4C)
        {
            if (((((u8)(holder->obj->flags1B2 >> index)) ^ 1) & 1) == 0)
            {
                release_part_sound(base + 0x41);
            }
        }
        if (holder->obj->textBlock != 0)
        {
            heap_free(holder->obj->textBlock);
        }
        heap_free(holder->obj);
        holder->obj = 0;
    }
    if (keep & 1)
    {
        heap_free_b(holder);
    }
}
END_NONMATCH

ASM_FUNC("asm/code_080E5590/flags_vars_setup_bg_layer.s", void flags_vars_setup_bg_layer(void))

ASM_FUNC("asm/code_080E5590/draw_text_prim.s", void draw_text_prim(void))

ASM_FUNC("asm/code_080E5590/draw_text_box.s", void draw_text_box(void))

ASM_FUNC("asm/code_080E5590/text_layer_step.s", void text_layer_step(void))
