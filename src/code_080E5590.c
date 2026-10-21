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
extern void *gUnk_03000F9C;
extern void *gUnk_03000F94;
extern s32 gUnk_03000FA0;
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
extern u16 gUnk_02000048;
extern u8 gUnk_02000044[];
extern u32 gUnk_03000F98;
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

ASM_FUNC("asm/code_080E5590/sub_080E5760.s", void sub_080E5760(void))

ASM_FUNC("asm/code_080E5590/sub_080E57D4.s", void sub_080E57D4(void))

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

NONMATCH("asm/code_080E5590/start_dma_window.s", void start_dma_window(void))
{
    u32 bank;

    ((void (*)(void))gUnk_03000F9C)();
    switch (gUnk_03000FA0)
    {
        case 0:
            dma_window_start((u8 *)0x040000B0);
            break;
        case 1:
            dma_window_start((u8 *)0x040000BC);
            break;
        case 2:
            dma_window_start((u8 *)0x040000C8);
            break;
        case 3:
            dma_window_start((u8 *)0x040000D4);
            break;
    }
    *(vu32 *)0x04000040 = *(u32 *)((u32)gUnk_03000F94 + (((bank = gUnk_03000F98) * 5) << 7));
    gUnk_03000F98 = bank ^ 1;
}
END_NONMATCH

ASM_FUNC("asm/code_080E5590/clip_edges_to_lines.s", void clip_edges_to_lines(void))

ASM_FUNC("asm/code_080E5590/fill_iris_spans.s", void fill_iris_spans(void))

u32 start_window_buffer(s32 mode, u32 fill)
{
    vu16 *win;
    vu16 *win2;
    u32 result;
    void *buffer;
    u32 off;

    buffer = gUnk_03000F94;
    if (buffer != 0)
    {
        result = 0;
    }
    else
    {
        *(vu16 *)0x02000000 |= 0x6000;
        win = (vu16 *)&gUnk_02000048;
        *win = (fill << 8) | fill;
        win++;
        *win = fill ^ 0x3F;
        win2 = (vu16 *)gUnk_02000044;
        *win2 = 0xA0;
        win2++;
        *win2 = 0xA0;
        gUnk_03000F94 = heap_alloc_raw(1, 0xA0 << 3, gWindowBufTag);
        gUnk_03000F98 = (u32)buffer;
        off = 0x8C4;
        gUnk_03000F9C = *(void **)(gSys + off);
        set_irq_callback(2, start_dma_window);
        if (mode > 3)
        {
            mode = 3;
        }
        if (mode < 0)
        {
            mode = 0;
        }
        gUnk_03000FA0 = mode;
        result = 1;
    }
    return result;
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

NONMATCH("asm/code_080E5590/start_dma0_line_copy.s", void start_dma0_line_copy(void))
{
    struct DmaChannel *dma = (struct DmaChannel *)0x040000B0;
    s32 rows4;

    *(vu16 *)((u8 *)dma + 0xA) &= 0xC5FF;
    *(vu16 *)((u8 *)dma + 0xA) &= 0x7FFF;
    *(vu16 *)((u8 *)dma + 0xA);
    dma->sad = (u32)gUnk_03000FA4 + ((((rows4 = gUnk_03000FB0 << 2) + gUnk_03000FB0) << 7) * gUnk_03000FA8);
    dma->dad = (gUnk_03000FAC << 2) + REG_ADDR_BG0HOFS;
    if (rows4 < 0)
    {
        rows4 += 3;
    }
    dma->cnt = ((rows4 >> 2) | 0xA6600000);
    dma->cnt;
    gUnk_03000FA8 ^= 1;
}
END_NONMATCH

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
    u32 unk194;
    u8 filler198[0x1B2 - 0x198];
    u8 flags1B2;
    u8 filler1B3[0x1BC - 0x1B3];
};

struct TextHolder
{
    void *records;
    struct TextObj *obj;
};

struct TextHolder *init_text_obj(struct TextHolder *holder, u32 arg1, u32 size, u32 arg3)
{
    struct TextScratch *first;
    void *table;
    u32 textBlock;
    u32 aligned;

    ((void (*)(u32, void *, u32))gMemRoutines)(0, gUnk_03000FB8, 0x34);
    ((void (*)(u32, void *, u32))gMemRoutines)(0, gUnk_03000FBC, 0x34);
    first = (struct TextScratch *)gUnk_03000FB8;
    first->unk28 = 1;
    first->unk2A = 1;
    first->unk2C = 2;
    first->unk2E = 1;
    first->unk32 = 1;
    holder->obj = heap_alloc_raw(1, 0x1BC, gTextObjTag);
    ((void (*)(u32, void *, u32))gMemRoutines)(0, holder->obj, 0x1BC);
    table = heap_alloc2(0x10, 1, gTextTableTag, 0);
    holder->records = build_record_chain(table, arg1, size, arg3);
    textBlock = (u32)heap_alloc_raw(1, size + 0x20, gTextBlockTag);
    holder->obj->textBlock = (u8 *)textBlock;
    aligned = (textBlock + 0x1F) & 0xFFFFFFE0;
    holder->obj->unk194 = aligned - arg1;
    holder->obj->frameTiles = gTextFrameTiles;
    return holder;
}

ASM_FUNC("asm/code_080E5590/release_obj_parts.s", void release_obj_parts(void))

ASM_FUNC("asm/code_080E5590/flags_vars_setup_bg_layer.s", void flags_vars_setup_bg_layer(void))

ASM_FUNC("asm/code_080E5590/draw_text_prim.s", void draw_text_prim(void))

ASM_FUNC("asm/code_080E5590/draw_text_box.s", void draw_text_box(void))

ASM_FUNC("asm/code_080E5590/text_layer_step.s", void text_layer_step(void))
