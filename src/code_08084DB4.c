#include "global.h"
#include "ram_routines.h"

#define DISPCNT_SHADOW 0x02000000

// One tile slot. The slots are 0x14 bytes each, slot idx starts at obj + idx * 0x14.
struct TileSlot
{
    u8 value;
    u8 unk1;
    u8 maxTiles;
    u8 unk3;
    u16 size;
    u16 end;
    u16 pos;
    u8 filler0A[2];
    u8 *block;
    u8 filler10[4];
};

extern u32 gUnk_03000F70;

void dma_copy_or_queue(void *src, void *dst, u32 len, u32 unit, u32 flags);
void heap_free(void *p);
void heap_free_b(void *p);
void alloc_tile_slot(u8 *obj, u8 idx, u8 palette);
void release_tile_slot(u8 *obj, u8 idx);
void cpu_set(const void *src, void *dst, u32 ctl);
void free_list_move(void *head, void *block);
void draw_text(u8 *base, u8 maxTiles, u16 x, u16 y, u8 *text, u8 palette);
void vram_slots_draw_number(u8 *base, u8 maxTiles, u16 x, u16 y, u32 value, u8 palette, u32 align, u32 font);

static inline struct TileSlot *slot_at(u8 *obj, u32 idx)
{
    return (struct TileSlot *)(obj + idx * 0x14);
}

ASM_FUNC("asm/code_08084DB4/set_tile_slot.s", void set_tile_slot(void))

struct SpanNode
{
    struct SpanNode *next;
    s16 start;
    s16 len;
};

void insert_span(u8 *obj, s32 start, s32 len)
{
    s32 end = start + len;
    struct SpanNode *prev = 0;
    struct SpanNode *node = *(struct SpanNode **)(obj + 0x1F4);
    struct SpanNode *fresh;

    if (node == 0 || end < node->start)
    {
        goto alloc;
    }
    do
    {
        if (node->start + node->len >= start)
        {
            goto merge;
        }
        prev = node;
        node = node->next;
        if (node == 0)
        {
            goto alloc;
        }
    } while (end >= node->start);
    goto alloc;
merge:
    if (start < node->start)
    {
        node->len = node->start - start + node->len;
        node->start = start;
    }
    if (node->start + node->len < end)
    {
        node->len = end - node->start;
    }
    return;
alloc:
    fresh = *(struct SpanNode **)(obj + 0x1F0);
    *(struct SpanNode **)(obj + 0x1F0) = fresh->next;
    if (prev != 0)
    {
        prev->next = fresh;
    }
    else
    {
        *(struct SpanNode **)(obj + 0x1F4) = fresh;
    }
    fresh->next = node;
    fresh->start = start;
    fresh->len = len;
}

ASM_FUNC("asm/code_08084DB4/alloc_tile_slot.s", void alloc_tile_slot(u8 *obj, u8 idx, u8 palette))

ASM_FUNC("asm/code_08084DB4/draw_slot_frame.s", void draw_slot_frame(void))

u8 *slot_block_ptr(u8 *obj, u8 idx)
{
    u32 off = idx * 0x14;
    u8 *base = obj + 0xC;

    return *(u8 **)(base + off);
}

ASM_FUNC("asm/code_08084DB4/clear_map_rect.s", void clear_map_rect(void))

ASM_FUNC("asm/code_08084DB4/copy_map_rect_add.s", void copy_map_rect_add(void))

ASM_FUNC("asm/code_08084DB4/unknown_code_08085850.s", void unknown_code_08085850(void))

void copy_slot_tiles(u8 *obj, u8 idx, void *dst, u32 off, u32 count)
{
    struct TileSlot *slot = slot_at(obj, idx);
    u32 skip = off << 5;

    dma_copy_or_queue(dst, slot->block + skip, count << 5, 0x20, 0);
}

void unknown_code_08085964(u8 *obj, u8 idx, u8 *src, u8 palette)
{
    struct TileSlot *slot = slot_at(obj, idx);

    alloc_tile_slot(obj, idx, palette);
    MEM_ROUTINE(void (*)(u8 *, u8 *, u32), iwram_copy)(src, slot->block, ((u32)slot->size << 18) >> 13);
}

void fill_slot(u8 *obj, u8 idx, u32 arg)
{
    struct TileSlot *slot = slot_at(obj, idx);
    u32 nibble;
    u32 pattern;

    alloc_tile_slot(obj, idx, (u8)(arg >> 4));
    nibble = arg & 0xF;
    pattern = (nibble << 4) + nibble;
    pattern += pattern << 8;
    pattern += pattern << 16;
    ((void (*)(u32, u8 *, u32))gMemRoutines)(pattern, slot->block, ((u32)slot->size << 18) >> 13);
}

void unknown_code_080859f0(u8 *obj, u8 idx, u32 a, u32 b, u32 p5, u8 p6, u32 p7)
{
    struct TileSlot *slot = slot_at(obj, idx);

    alloc_tile_slot(obj, idx, p6 >> 4);
    vram_slots_draw_number(slot->block, slot->maxTiles, (u16)a, (u16)b, p5, p6, p7, 0);
}

void draw_slot_text(u8 *obj, u8 idx, u32 x, u32 y, u8 *text, u8 color)
{
    struct TileSlot *slot;

    if (text != 0)
    {
        slot = slot_at(obj, idx);
        alloc_tile_slot(obj, idx, color >> 4);
        draw_text(slot->block, slot->maxTiles, (u16)x, (u16)y, text, color);
    }
}

ASM_FUNC("asm/code_08084DB4/release_tile_slot.s", void release_tile_slot(u8 *obj, u8 idx))

ASM_FUNC("asm/code_08084DB4/flush_copy_queue.s", void flush_copy_queue(void))

ASM_FUNC("asm/code_08084DB4/free_slot_blocks.s", void free_slot_blocks(void))
ASM_FUNC("asm/code_08084DB4/reset_display.s", void reset_display(void))

void restore_display(u8 *obj, u32 flags)
{
    *(u16 *)DISPCNT_SHADOW = gUnk_03000F70;
    heap_free(*(void **)(obj + 0x288));
    if (flags & 1)
    {
        heap_free_b(obj);
    }
}

ASM_FUNC("asm/code_08084DB4/jump_velocity.s", void jump_velocity(void))

ASM_FUNC("asm/code_08084DB4/unknown_code_08085f6c.s", void unknown_code_08085f6c(void))

ASM_FUNC("asm/code_08084DB4/sub_080861A4.s", void sub_080861A4(void))

struct MotionObj
{
    u8 filler0[0x10];
    s32 x;
    s32 y;
    s32 h;
    u8 filler1C[0x79 - 0x1C];
    u8 flags;
    s16 motionStart;
    s16 motionSpeed;
    s16 motionProgress;
    s32 index;
    s32 tgtX;
    s32 tgtY;
    s32 tgtH;
};

u32 motion_step(struct MotionObj *m)
{
    s32 index = m->index;
    s32 dx = m->tgtX - m->x;
    s32 dy = m->tgtY - m->y;
    s32 dh = m->tgtH - m->h;
    s32 step = m->motionProgress;

    if (index < step)
    {
        step = index;
    }
    if (dx != 0)
    {
        m->x += MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(dx * step, index);
    }
    if (dy != 0)
    {
        m->y += MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(dy * step, index);
    }
    if (dh != 0)
    {
        m->h += MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(dh * step, index);
    }
    m->index -= step;
    if (m->index != 0)
    {
        return 0;
    }
    return 1;
}

ASM_FUNC("asm/code_08084DB4/start_motion.s", void start_motion(void))

ASM_FUNC("asm/code_08084DB4/unknown_code_0808645c.s", void unknown_code_0808645c(void))
