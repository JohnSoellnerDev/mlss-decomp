#include "global.h"
#include "ram_routines.h"
#include "tile_window.h"

#define DISPCNT_SHADOW 0x02000000

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

struct MoveObj
{
    u8 filler0[0x10];
    s32 posX;
    s32 posY;
    s32 posH;
    u8 filler1C[0x7C - 0x1C];
    u16 rate;
    u16 timer;
    u8 filler80[0x84 - 0x80];
    s32 baseX;
    s32 baseY;
    s32 baseH;
    u8 filler90[0x94 - 0x90];
    s32 phase;
    s32 unk98;
    u8 filler9C[0xAE - 0x9C];
    s16 velX;
    s16 velY;
    s16 velH;
};


NONMATCH("asm/code_08084DB4/set_tile_slot.s", void set_tile_slot(u8 *obj, u8 idx, u32 flag, u32 value, u32 p5, u32 p6, u32 p7))
{
    struct TileSlot *slot = (struct TileSlot *)(obj + idx * 0x14);
    u32 tiles;
    u32 bit6;
    s32 sm = ~0x3FFF;

    if (slot->maxTiles != 0)
    {
        release_tile_slot(obj, idx);
    }
    ((struct TileSlotFlags *)slot)->bit6 = flag & 1;
    ((struct TileSlotFlags *)slot)->bit7 = 0;
    tiles = (p6 * p7) & 0x3FFF;
    slot->size = (slot->size & sm) | tiles;
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
    s32 rowsNext;
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
        rows--;
        if (rows >= 0)
        {
            do
            {
                rowsNext = rows - 1;
                next = cell + 0x20;
                if (width != 0)
                {
                    p = cell;
                    left = width;
                    do
                    {
                        *p++ = attr++;
                    } while (--left != 0);
                }
                cell = next;
                rows = rowsNext;
            } while (rows >= 0);
        }
        insert_span(obj, slot->pos, slot->end);
    }
}
END_NONMATCH

// Writes count tiles in a row, starting at dst; count is at least 1.
static inline void fill_run(u16 *dst, s32 count, u32 value)
{
    do
    {
        *dst++ = value;
    } while (--count != 0);
}

// Frame with a left column and a filled middle: top row, middle rows, bottom row.
static inline void frame_box(u16 *cell, s32 w, s32 h, u32 tl, u32 tf, u32 ml, u32 mf, u32 bl, u32 bf)
{
    u16 *next;
    s32 i;

    cell[0] = 0x8000 | tl;
    next = cell + 0x20;
    if (w > 1)
    {
        fill_run(cell + 1, w - 1, 0x8000 | tf);
    }
    i = h - 3;
    while (i >= 0)
    {
        cell = next;
        cell[0] = 0x8000 | ml;
        i--;
        if (w > 1)
        {
            fill_run(cell + 1, w - 1, 0x8000 | mf);
        }
        next = cell + 0x20;
    }
    cell = next;
    cell[0] = 0x8000 | bl;
    if (w > 1)
    {
        fill_run(cell + 1, w - 1, 0x8000 | bf);
    }
}

NONMATCH("asm/code_08084DB4/draw_slot_frame.s", void draw_slot_frame(u8 *obj, u8 slot, u8 rect, u32 kind))
{
    u8 *rec;
    s32 x;
    s32 y;
    s32 w;
    s32 h;
    u16 start;
    u16 *cell;
    u16 *next;
    s32 i;
    s32 idx;
    s32 last;

    rec = obj + rect * 0x14;
    switch (kind)
    {
    case 0:
        x = rec[0];
        y = rec[1];
        w = rec[2];
        h = rec[3];
        break;
    case 1:
        x = rec[0];
        y = rec[1];
        w = rec[2] - 1;
        h = rec[3];
        break;
    case 2:
    case 3:
        x = rec[0] - 1;
        y = rec[1] - 1;
        w = rec[2] + 2;
        h = rec[3];
        break;
    case 4:
        x = rec[0];
        y = rec[1];
        w = rec[2];
        h = rec[3] + 1;
        break;
    default:
        break;
    }

    set_tile_slot(obj, slot, 1, x, y, w, h);
    start = (y << 5) + 0x280 + x;
    cell = *(u16 **)(obj + 0x288) + start;
    insert_span(obj, start, ((h - 1) << 5) + w);

    switch (kind)
    {
    case 0:
        frame_box(cell, w, h, 1, 2, 3, 4, 5, 6);
        break;
    case 1:
        last = 0;
        if (w - 1 > 0)
        {
            fill_run(cell, w - 1, 0x8000 | 2);
            last = w - 1;
        }
        cell[last] = 0x8000 | 0x401;
        next = cell + 0x20;
        i = h - 3;
        while (i >= 0)
        {
            cell = next;
            i--;
            last = 0;
            if (w - 1 > 0)
            {
                fill_run(cell, w - 1, 0x8000 | 4);
                last = w - 1;
            }
            cell[last] = 0x8000 | 0x403;
            next = cell + 0x20;
        }
        cell = next;
        last = 0;
        if (w - 1 > 0)
        {
            fill_run(cell, w - 1, 0x8000 | 6);
            last = w - 1;
        }
        cell[last] = 0x8000 | 0x405;
        break;
    case 2:
    case 3:
        cell[0] = 0x8000 | 0x12;
        cell[1] = 0x8000 | 0xD;
        idx = 2;
        if (2 < w - 2)
        {
            fill_run(cell + 2, w - 4, 0x8000 | 0xE);
            idx = w - 2;
        }
        cell[idx] = 0x8000 | 0x40D;
        cell[idx + 1] = 0x8000 | 0x412;
        cell += 0x20;
        cell[0] = 0x8000 | 0x13;
        idx = 1;
        if (1 < w - 1)
        {
            fill_run(cell + 1, w - 2, 0x8000 | 4);
            idx = w - 1;
        }
        cell[idx] = 0x8000 | 0x413;
        i = h - 5;
        while (i >= 0)
        {
            cell += 0x20;
            cell[0] = 0x8000 | 0xF;
            i--;
            idx = 1;
            if (1 < w - 1)
            {
                fill_run(cell + 1, w - 2, 0x8000 | 4);
                idx = w - 1;
            }
            cell[idx] = 0x8000 | 0x40F;
        }
        cell += 0x20;
        cell[0] = 0x8000 | 0x14;
        idx = 1;
        if (1 < w - 1)
        {
            fill_run(cell + 1, w - 2, 0x8000 | 4);
            idx = w - 1;
        }
        cell[idx] = 0x8000 | 0x414;
        cell += 0x20;
        cell[0] = 0x8000 | 0x15;
        cell[1] = 0x8000 | 0x10;
        idx = 2;
        if (2 < w - 2)
        {
            fill_run(cell + 2, w - 4, 0x8000 | 0x11);
            idx = w - 2;
        }
        cell[idx] = 0x8000 | 0x410;
        cell[idx + 1] = 0x8000 | 0x415;
        break;
    case 4:
        frame_box(cell, w, h, 7, 8, 9, 0xA, 0xB, 0xC);
        break;
    default:
        break;
    }
}
END_NONMATCH

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

// Copies a w x h block of cells from src (its rows TILE_WINDOW_MAP_WIDTH cells apart) to cell
// (x, y) of tilemap `map`, adding `add` to each cell (e.g. palette bits), and queues them for
// VRAM. Does nothing without src.
void copy_map_rect_add(struct TileWindow *win, u32 map, u32 x, u32 y, s32 w, s32 h, u16 *src, u32 add)
{
    if (src != NULL)
    {
        u16 start = map * TILE_WINDOW_MAP_CELLS + y * TILE_WINDOW_MAP_WIDTH + x;
        u16 *row = win->tilemap + start;
        s32 i;

        insert_span((u8 *)win, start, ((h - 1) << 5) + w);
        while (--h >= 0)
        {
            i = w;
            while (--i >= 0)
                row[i] = src[i] + add;
            row += TILE_WINDOW_MAP_WIDTH;
            src += TILE_WINDOW_MAP_WIDTH;
        }
    }
}

// Sets the first `rows` rows of tilemap `map`: copies them from src adding `value` to each cell,
// or, without src, fills them with `value`. The rows are queued for VRAM.
void fill_map_rect(struct TileWindow *win, u32 map, u16 *src, u32 rows, u32 value)
{
    u16 *dst;
    s32 cells;
    s32 i;
    u16 *cell;

    // map * TILE_WINDOW_MAP_CELLS cells, in bytes
    dst = (u16 *)((u8 *)win->tilemap + ((map * 5) << 8));
    if (src != NULL)
    {
        MEM_ROUTINE(void (*)(u8 *, u8 *, u32), iwram_copy)((u8 *)src, (u8 *)dst, rows << 6);
    }
    else
    {
        MEM_ROUTINE(void (*)(u32, u8 *, u32), iwram_fill)((value << 16) | value, (u8 *)dst, rows << 6);
    }
    insert_span((u8 *)win, (map * 5) << 7, rows << 5);
    if (value != 0 && src != NULL)
    {
        cells = rows << 5;
        if (cells > 0)
        {
            cell = dst;
            i = cells;
            do
            {
                *cell = *cell + value;
                cell++;
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

/* Frees window slot idx's tiles: gives its tile block back to its tilemap's free list (the first
 * time it only moves the block to spare), clears its rectangle of tilemap cells and queues them
 * for the copy to VRAM. A slot with no width is left alone. */
void release_tile_slot(u8 *obj, u8 idx)
{
    struct TileSlot *slot = (struct TileSlot *)(obj + idx * 0x14);
    u8 *block;
    void **heaps;
    u32 off;
    u16 *row;
    s32 rows;
    s32 col;
    u32 map;

    if (slot->maxTiles != 0)
    {
        if (slot->spare != 0)
        {
            block = slot->block;
            if (block == NULL)
            {
                return;
            }
            map = ((struct TileSlotFlags *)slot)->bit6;
            off = map * sizeof(void *);
            heaps = ((struct TileWindow *)obj)->tileHeaps;
            free_list_move(*(void **)((u8 *)heaps + off), block);
        }
        else
        {
            slot->spare = slot->block;
        }
        slot->block = NULL;
        rows = slot->unk3;
        row = ((struct TileWindow *)obj)->tilemap + slot->pos;
        while (--rows >= 0)
        {
            col = slot->maxTiles;
            while (--col >= 0)
                row[col] = 0;
            row += TILE_WINDOW_MAP_WIDTH;
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
/*
 * Saves DISPCNT (for restore_display) and sets the display up in the register buffer: every layer
 * and both windows on, BG2 and BG3 unscrolled, both windows covering the whole screen with every
 * layer shown, an even alpha blend; then clears BG3's tiles (char block 2) and tilemap (screen block 31).
 */
void reset_display(void)
{
    vu16 *reg;

    gSavedDispcnt = REG_BUF_DISPCNT;
    REG_BUF_DISPCNT = DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON
                    | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON | DISPCNT_WIN1_ON;
    REG_BUF_BLDCNT = BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG0 | BLDCNT_TGT2_BG1 | BLDCNT_TGT2_BG2
                   | BLDCNT_TGT2_OBJ | BLDCNT_TGT2_BD;
    REG_BUF_BLDALPHA = BLDALPHA_BLEND(8, 8);
    REG_BUF_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHARBASE(1) | BGCNT_SCREENBASE(30);
    REG_BUF_BG3CNT = BGCNT_PRIORITY(0) | BGCNT_CHARBASE(2) | BGCNT_SCREENBASE(31);
    reg = (vu16 *)&gUnk_02000018;
    *reg = 0; // BG2HOFS
    reg++;
    *reg = 0; // BG2VOFS
    reg++;
    *reg = 0; // BG3HOFS
    reg++;
    *reg = 0; // BG3VOFS
    reg += 0x15;
    *reg = WIN_ALL_LAYERS | (WIN_ALL_LAYERS << 8); // WININ
    reg++;
    *reg = WIN_ALL_LAYERS | (WIN_ALL_LAYERS << 8); // WINOUT
    reg -= 5;
    *reg = WIN_RANGE(0, SCREEN_WIDTH); // WIN0H
    reg += 2;
    *reg = WIN_RANGE(0, SCREEN_HEIGHT); // WIN0V
    reg--;
    *reg = WIN_RANGE(0, SCREEN_WIDTH); // WIN1H
    reg += 2;
    *reg = WIN_RANGE(0, SCREEN_HEIGHT); // WIN1V
    dma_fill_or_queue(0, (void *)(VRAM + 0x8000), 0x5000, 0x20, 0);
    dma_fill_or_queue(0, (void *)(VRAM + 0xF800), 0x800, 0x20, 0);
}

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
/* Plans a jump (like jump_velocity, along one axis): it covers `distance` at `speed` per frame,
   rises over `peak` and lands `height` above the start. Returns the gravity per frame, with the
   starting vertical speed in *outVelH and the frame it reaches `height` again (distance_step)
   in *outLandFrame. */
s32 sub_080861A4(s32 *outLandFrame, s32 *outVelH, s32 distance, s32 height, s32 peak, s32 speed)
{
    s32 rise;
    s32 path;
    s32 frames;
    s32 gravity;
    s32 frameProduct;
    s32 apex;

    rise = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)((peak * (peak / 2)) / 2 - (peak * (height / 2)) / 2);
    apex = peak + rise * 2;
    path = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)((apex * (apex / 2)) / 2 + (distance * (distance / 4)) / 4);
    *outVelH = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(speed * apex * 0x10 + path / 2, path) << 3;
    frames = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(path * 4 + speed / 2, speed);
    frameProduct = frames * (frames + 1);
    gravity = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(((height << 8) - *outVelH * frames) * 2 + frameProduct / 2, frameProduct);
    *outLandFrame = distance_step(*outVelH, gravity, height);
    return gravity;
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
    s16 motionProgress; /* how much of index one frame of motion_step covers */
    s32 index;          /* motion left to the target: 0 = arrived */
    s32 tgtX;           /* where the motion ends */
    s32 tgtY;
    s32 tgtH;
};

/* One frame of a straight-line motion: moves x, y and h the fraction step / index of the way to the
   target (step = motionProgress, at most what's left) and takes step off index. Returns 1 once the
   target is reached, else 0. */
u32 motion_step(struct MotionObj *m)
{
    s32 left = m->index;
    s32 dx = m->tgtX - m->x;
    s32 dy = m->tgtY - m->y;
    s32 dh = m->tgtH - m->h;
    s32 step = m->motionProgress;

    if (left < step)
    {
        step = left;
    }
    if (dx != 0)
    {
        m->x += MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(dx * step, left);
    }
    if (dy != 0)
    {
        m->y += MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(dy * step, left);
    }
    if (dh != 0)
    {
        m->h += MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(dh * step, left);
    }
    m->index -= step;
    if (m->index != 0)
    {
        return 0;
    }
    return 1;
}

ASM_FUNC("asm/code_08084DB4/start_motion.s", void start_motion(void))

static inline s32 move_obj_height(struct MoveObj *o, s32 tm)
{
    s32 h;

    if (o->phase != 0)
    {
        s32 inv = 0x4000 - tm;
        s32 u = o->unk98;
        s32 bh = o->baseH;

        if (u < 0)
        {
            s32 a = (o->phase * (0x4000 - inv)) / 0x4000 - u;
            s32 b = o->phase - u;

            h = bh - (u * u) / 0x100 - (b * b) / 0x100 + (a * a) / 0x100;
        }
        else
        {
            s32 x = (o->phase * inv) / 0x4000 - u;

            h = bh - (x * x) / 0x100;
        }
    }
    else
    {
        h = o->baseH - ((s16)o->velH * tm) / 0x1000;
    }
    return h;
}

// Steps an object's glide onto its base point: timer counts down by rate (from 0x4000), and the
// position is base - vel * timer / 0x1000, the height on an arc if phase is set (move_obj_height).
// Returns 1 once the timer runs out and the object sits on the base point, 0 while still moving.
u32 unknown_code_0808645c(u8 *obj)
{
    struct MoveObj *mover = (struct MoveObj *)obj;
    s32 remaining;

    mover->timer = mover->timer - mover->rate;
    if ((s16)mover->timer > 0)
    {
        remaining = (s16)mover->timer;
        mover->posX = mover->baseX - ((s16)mover->velX * remaining) / 0x1000;
        mover->posY = mover->baseY - ((s16)mover->velY * remaining) / 0x1000;
        mover->posH = move_obj_height(mover, remaining);
        return 0;
    }
    mover->posX = mover->baseX;
    mover->posY = mover->baseY;
    mover->posH = move_obj_height(mover, 0);
    mover->timer = 0;
    return 1;
}
