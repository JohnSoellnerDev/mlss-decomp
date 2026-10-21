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
    u8 *spare;
};

extern u32 gSavedDispcnt;
extern u16 gUnk_02000018;
s32 distance_step(s32 x, s32 y, s32 z);

void dma_copy_or_queue(void *src, void *dst, u32 len, u32 unit, u32 flags);
void dma_fill_or_queue(u32 value, void *dst, u32 len, u32 unit, u32 flags);
void heap_free(void *p);
void heap_free_b(void *p);
void alloc_tile_slot(u8 *obj, u8 idx, u8 palette);
void release_tile_slot(u8 *obj, u8 idx);
void cpu_set(const void *src, void *dst, u32 ctl);
void free_list_move(void *head, void *block);
u32 free_list_alloc(void *list, u32 size);
void draw_text(u8 *base, u8 maxTiles, u16 x, u16 y, u8 *text, u8 palette);
void vram_slots_draw_number(u8 *base, u8 maxTiles, u16 x, u16 y, u32 value, u8 palette, u32 align, u32 font);

static inline struct TileSlot *slot_at(u8 *obj, u32 idx)
{
    return (struct TileSlot *)(obj + idx * 0x14);
}

struct TileSlotFlags
{
    u8 filler0[5];
    u8 low6 : 6;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

NONMATCH("asm/code_08084DB4/set_tile_slot.s", void set_tile_slot(u8 *obj, u8 idx, u32 flag, u32 value, u32 p5, u32 p6, u32 p7))
{
    struct TileSlot *slot = (struct TileSlot *)(obj + idx * 0x14);
    u32 tiles;
    u32 bit6;

    if (slot->maxTiles != 0)
    {
        release_tile_slot(obj, idx);
    }
    ((struct TileSlotFlags *)slot)->bit6 = flag & 1;
    ((struct TileSlotFlags *)slot)->bit7 = 0;
    tiles = (p6 * p7) & 0x3FFF;
    slot->size = (slot->size & -0x4000) | tiles;
    slot->value = value;
    slot->unk1 = p5;
    slot->maxTiles = p6;
    slot->unk3 = p7;
    bit6 = ((struct TileSlotFlags *)slot)->bit6;
    slot->pos = (bit6 * 5 << 7) + (p5 << 5) + value;
    slot->end = ((p7 - 1) << 5) + p6;
    slot->block = 0;
}
END_NONMATCH

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

extern u8 *gMemRoutines;
void insert_span(u8 *obj, s32 start, s32 len);
NONMATCH("asm/code_08084DB4/alloc_tile_slot.s", void alloc_tile_slot(u8 *obj, u8 idx, u8 palette))
{
    struct TileSlot *slot = (struct TileSlot *)(obj + idx * 0x14);
    void *list;
    void **head;
    u8 flags;
    u32 sel;
    u32 off;
    u32 size;
    u32 block;
    u32 tile;
    u32 attr;
    u32 width;
    s32 rows;
    u32 left;
    u16 *p;
    u16 *cell;
    u16 *next;

    flags = ((u8 *)slot)[5];
    if ((flags & 0x80) == 0)
    {
        flags |= 0x80;
        ((u8 *)slot)[5] = flags;
        sel = ((u32)flags << 25) >> 31;
        size = ((u32)slot->size << 18) >> 18;
        if (slot->spare == 0)
        {
            slot->spare = slot->block;
        }
        off = sel * 4;
        head = (void **)(obj + 0x280);
        list = *(void **)((u8 *)head + off);
        size <<= 5;
        block = free_list_alloc(list, size);
        slot->block = (u8 *)block;
        tile = ((block - 0x06004000) << 11) >> 16;
        ((void (*)(u32, u8 *, u32))gMemRoutines)(0, (u8 *)block, size);
        width = slot->maxTiles;
        rows = slot->unk3;
        cell = *(u16 **)(obj + 0x288) + slot->pos;
        attr = ((palette & 0xF) << 12) | tile;
        while (--rows >= 0)
        {
            next = cell + 0x20;
            if (width != 0)
            {
                left = width;
                p = cell;
                do
                {
                    *p++ = attr++;
                } while (--left != 0);
            }
            cell = next;
        }
        insert_span(obj, slot->pos, slot->end);
    }
}
END_NONMATCH

ASM_FUNC("asm/code_08084DB4/draw_slot_frame.s", void draw_slot_frame(void))

u8 *slot_block_ptr(u8 *obj, u8 idx)
{
    u32 off = idx * 0x14;
    u8 *base = obj + 0xC;

    return *(u8 **)(base + off);
}

void clear_map_rect(u8 *obj, u32 y, u32 x, u32 layer, s32 w, s32 h)
{
    u16 off = y * 0x280 + layer * 32 + x;
    u16 *row = *(u16 **)(obj + 0x288) + off;
    s32 i;

    insert_span(obj, off, ((h - 1) << 5) + w);
    while (--h >= 0)
    {
        i = w;
        while (--i >= 0)
            row[i] = 0;
        row += 0x20;
    }
}

void copy_map_rect_add(u8 *obj, u32 y, u32 x, u32 layer, s32 w, s32 h, u16 *src, u32 add)
{
    if (src != NULL)
    {
        u16 off = y * 0x280 + layer * 32 + x;
        u16 *row = *(u16 **)(obj + 0x288) + off;
        s32 i;

        insert_span(obj, off, ((h - 1) << 5) + w);
        while (--h >= 0)
        {
            i = w;
            while (--i >= 0)
                row[i] = src[i] + add;
            row += 0x20;
            src += 0x20;
        }
    }
}

void fill_map_rect(u8 *obj, u32 y, u16 *src, u32 h, u32 value)
{
    u16 *dst;
    s32 n;
    s32 i;
    u16 *p;

    dst = (u16 *)(*(u8 **)(obj + 0x288) + ((y * 5) << 8));
    if (src != 0)
    {
        MEM_ROUTINE(void (*)(u8 *, u8 *, u32), iwram_copy)((u8 *)src, (u8 *)dst, h << 6);
    }
    else
    {
        ((void (*)(u32, u8 *, u32))gMemRoutines)((value << 16) | value, (u8 *)dst, h << 6);
    }
    insert_span(obj, (y * 5) << 7, h << 5);
    if (value != 0 && src != 0)
    {
        n = h << 5;
        if (n > 0)
        {
            p = dst;
            i = n;
            do
            {
                *p = *p + value;
                p++;
            } while (--i != 0);
        }
    }
}

void copy_slot_tiles(u8 *obj, u8 idx, void *dst, u32 off, u32 count)
{
    struct TileSlot *slot = slot_at(obj, idx);
    u32 skip = off << 5;

    dma_copy_or_queue(dst, slot->block + skip, count << 5, 0x20, 0);
}

void copy_into_slot(u8 *obj, u8 idx, u8 *src, u8 palette)
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

void draw_slot_number(u8 *obj, u8 idx, u32 a, u32 b, u32 p5, u8 p6, u32 p7)
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

void release_tile_slot(u8 *obj, u8 idx)
{
    struct TileSlot *slot = (struct TileSlot *)(obj + idx * 0x14);
    u8 *block;
    void **list;
    u32 off;
    u16 *row;
    s32 h;
    s32 i;
    u32 bit6;

    if (slot->maxTiles != 0)
    {
        if (slot->spare != 0)
        {
            block = slot->block;
            if (block == 0)
            {
                return;
            }
            bit6 = ((u32)((u8 *)slot)[5] << 25) >> 31;
            off = bit6 * 4;
            list = (void **)(obj + 0x280);
            free_list_move(*(void **)((u8 *)list + off), block);
        }
        else
        {
            slot->spare = slot->block;
        }
        slot->block = 0;
        h = slot->unk3;
        row = *(u16 **)(obj + 0x288) + slot->pos;
        while (--h >= 0)
        {
            i = slot->maxTiles;
            while (--i >= 0)
                row[i] = 0;
            row += 0x20;
        }
        insert_span(obj, slot->pos, slot->end);
    }
}

void flush_copy_queue(u8 *obj)
{
    struct SpanNode *node;
    u32 vram;

    while ((node = *(struct SpanNode **)(obj + 0x1F4)) != 0)
    {
        if (node->start <= 0x27F)
        {
            vram = 0x0600F000;
        }
        else
        {
            vram = 0x0600F300;
        }
        cpu_set(*(u8 **)(obj + 0x288) + node->start * 2, (void *)(vram + node->start * 2), node->len & 0x1FFFFF);
        *(struct SpanNode **)(obj + 0x1F4) = node->next;
        node->next = *(struct SpanNode **)(obj + 0x1F0);
        *(struct SpanNode **)(obj + 0x1F0) = node;
    }
}

ASM_FUNC("asm/code_08084DB4/free_slot_blocks.s", void free_slot_blocks(void))
NONMATCH("asm/code_08084DB4/reset_display.s", void reset_display(void))
{
    u16 *p;

    gSavedDispcnt = *(u16 *)DISPCNT_SHADOW;
    *(u16 *)DISPCNT_SHADOW = 0x7F40;
    *(u16 *)(DISPCNT_SHADOW + 0x50) = 0x3740;
    *(u16 *)(DISPCNT_SHADOW + 0x52) = 0x808;
    *(u16 *)(DISPCNT_SHADOW + 0x0C) = 0x1E04;
    *(u16 *)(DISPCNT_SHADOW + 0x0E) = 0x1F08;
    p = (u16 *)&gUnk_02000018;
    *p = 0;
    p++;
    *p = 0;
    p++;
    *p = 0;
    p++;
    *p = 0;
    p += 0x15;
    *p = 0x3F3F;
    p++;
    *p = 0x3F3F;
    p -= 5;
    *p = 0xF0;
    p += 2;
    *p = 0xA0;
    p--;
    *p = 0xF0;
    p += 2;
    *p = 0xA0;
    dma_fill_or_queue(0, (void *)0x06008000, 0x5000, 0x20, 0);
    dma_fill_or_queue(0, (void *)0x0600F800, 0x800, 0x20, 0);
}
END_NONMATCH

void restore_display(u8 *obj, u32 flags)
{
    *(u16 *)DISPCNT_SHADOW = gSavedDispcnt;
    heap_free(*(void **)(obj + 0x288));
    if (flags & 1)
    {
        heap_free_b(obj);
    }
}

NONMATCH("asm/code_08084DB4/jump_velocity.s", s32 jump_velocity(u16 *outDx, u16 *outDy, u16 *outDh, s32 tx, s32 ty, s32 h0, s32 t, s32 g, s32 damp, s32 *extra))
{
    s32 dist;
    s32 disc;
    s32 ft;
    s32 halfC;
    s32 arg;
    s32 q2x4;
    s32 n;
    s32 sum;
    s32 k;
    s32 j;
    s32 v;
    s32 res;

    dist = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(tx * tx + ty * ty);
    disc = (t * (t / 2)) / 2 - (t * (h0 / 2)) / 2;
    ft = t + MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(disc) * 2;
    halfC = (ft * (ft / 2)) / 2;
    arg = halfC + (dist * (dist / 4)) / 4;
    q2x4 = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(arg) * 4;
    *outDh = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(((g * ft) << 1) + (q2x4 >> 1), q2x4);
    n = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(q2x4 + g / 2, g);

    sum = 0;
    k = n;
    do
    {
        v = 0x4000;
        j = k;
        if (j > 0)
        {
            do
            {
                v = (v * damp) / 0x100;
                j--;
            } while (j != 0);
        }
        sum += v;
        k--;
    } while (k != 0);

    *outDx = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)((tx << 14) + sum / 2, sum);
    *outDy = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)((ty << 14) + sum / 2, sum);

    res = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(2 * (h0 - (s16)*outDh * n) + (n * (n + 1)) / 2, n * (n + 1));
    if (extra != NULL)
    {
        *extra = distance_step((s16)*outDh, res, h0);
    }
    return res;
}
END_NONMATCH

NONMATCH("asm/code_08084DB4/unknown_code_08085f6c.s", s32 unknown_code_08085f6c(u16 *outDx, u16 *outDy, u16 *outDh, s32 tx, s32 ty, s32 h0, s32 t, s32 g, s32 damp, s32 *extra))
{
    s32 dist;
    s32 tt;
    s32 disc;
    s32 ft;
    s32 halfC;
    s32 arg;
    s32 q2x4;
    s32 n;
    s32 sum;
    s32 k;
    s32 j;
    s32 v;
    s32 res;

    tt = t;
    dist = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(tx * tx + ty * ty);
    if (h0 > 0)
    {
        tt += h0;
    }
    tt = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(tt * ((tt * (tt / 0x100)) / 0x100));
    tt = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(tt << 14);
    disc = (tt * (tt / 2)) / 2 - (tt * (t / 2)) / 2;
    ft = tt + MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(disc) * 2;
    halfC = (ft * (ft / 2)) / 2;
    arg = halfC + (dist * (dist / 4)) / 4;
    q2x4 = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(arg) * 4;
    *outDh = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(((g * ft) << 1) + (q2x4 >> 1), q2x4);
    n = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(q2x4 + g / 2, g);

    sum = 0;
    k = n;
    do
    {
        v = 0x4000;
        for (j = k; j > 0; j--)
        {
            v = (v * damp) / 0x100;
        }
        sum += v;
        k--;
    } while (k != 0);

    *outDx = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)((tx << 14) + sum / 2, sum);
    *outDy = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)((ty << 14) + sum / 2, sum);

    res = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(2 * (h0 - (s16)*outDh * n) + (n * (n + 1)) / 2, n * (n + 1));
    if (extra != NULL)
    {
        *extra = distance_step((s16)*outDh, res, h0);
    }
    return res;
}
END_NONMATCH

s32 distance_step(s32 x, s32 y, s32 z);
s32 sub_080861A4(s32 *dist, s32 *outX, s32 x, s32 y, s32 h, s32 n)
{
    s32 s1;
    s32 s2;
    s32 s3;
    s32 s4;
    s32 s5;
    s32 h2;

    s1 = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)((h * (h / 2)) / 2 - (h * (y / 2)) / 2);
    h2 = h + s1 * 2;
    s2 = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)((h2 * (h2 / 2)) / 2 + (x * (x / 4)) / 4);
    *outX = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(n * h2 * 0x10 + s2 / 2, s2) << 3;
    s3 = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(s2 * 4 + n / 2, n);
    s5 = s3 * (s3 + 1);
    s4 = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(((y << 8) - *outX * s3) * 2 + s5 / 2, s5);
    *dist = distance_step(*outX, s4, y);
    return s4;
}

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
