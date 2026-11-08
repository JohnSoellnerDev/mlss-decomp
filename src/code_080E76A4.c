#include "global.h"

void dma_copy_or_queue(void *src, void *dst, u32 len, u32 unit, u32 flags);

// One rectangle of a state block: tile column and row (1/8 units, scaled down by 3 later),
// width and height in tiles. The slots are 0x4C bytes apart.
struct SlotRect
{
    s16 x;
    s16 y;
    u8 cols;
    u8 rows;
    u8 filler4[0x4C - 6];
};

struct ObjState
{
    u8 filler0[0x34];
    struct SlotRect slots[4];
    u8 filler164[0x184 - 0x164];
    u8 *map;
    u16 *header;
    u16 *hofs;
    u16 *vofs;
    u32 unk194;
    u16 prevX;
    u16 prevY;
    u16 wallX;
    u16 wallY;
    u16 velX;
    u16 velY;
    u16 posX;
    u16 posY;
    u16 boxX0;
    u16 boxY0;
    u16 boxX1;
    u16 boxY1;
    u16 fillValue;
    u8 slotMask;
    u8 unk1B3;
    u8 unk1B4;
    u8 filler1B5[0x1B8 - 0x1B5];
    u8 flags;
};

struct ObjWithState
{
    void *head;
    struct ObjState *state;
};

struct ObjFields
{
    u8 filler0[0x42];
    u8 unk42;
    u8 unk43;
    s32 unk44;
    s32 unk48;
};

struct Bits43
{
    u8 b0 : 1;
    u8 b1 : 1;
    u8 b2 : 1;
    u8 hi5 : 5;
};

struct SlotRectU
{
    u16 x;
    u16 y;
    u8 cols;
    u8 rows;
    u8 filler4[0x4C - 6];
};

// Slot record (0x4C-byte stride): the slot rectangle sits at +0x34, as in the state's slots.
struct SlotRec
{
    u8 filler0[8];
    u32 unk8;
    u8 filler0C[0x2C - 0xC];
    u32 unk2C;
    u8 filler30[0x34 - 0x30];
    struct SlotRectU rect;
};

struct FlagBits8
{
    u8 lo2 : 2;
    u8 bit2 : 1;
    u8 hi5 : 5;
};

// Sprite record of an object (the 0x4C-byte slot records are separate): position, flags and
// the two signed offsets added to the position when the entry is emitted.
struct SpriteRec
{
    u8 filler0[0x34];
    u16 x;
    u16 y;
    u8 filler38[0x3C - 0x38];
    u16 flags;
    u8 filler3E[0x42 - 0x3E];
    u8 b42;
    u8 b43;
    s32 ofsX;
    s32 ofsY;
};

extern u8 gSys[];

void free_list_move(void *head, void *block);

ASM_FUNC("asm/code_080E76A4/clear_active_slot_rects.s", void clear_active_slot_rects(struct ObjWithState *obj))

NONMATCH("asm/code_080E76A4/clear_slot_rect.s", void clear_slot_rect(struct ObjWithState *obj, u16 slot))
{
    struct ObjState *st;
    struct SlotRec *rec;
    struct ObjState *cells;
    u8 inactive;
    u16 col;
    u8 colEnd;
    u8 rowEnd;
    u16 x;
    u16 y;
    u32 one;

    st = obj->state;
    inactive = ((st->slotMask >> slot) ^ 1) & 1;
    if (inactive == 0)
    {
        rec = (struct SlotRec *)((u8 *)st + slot * 0x4C);
        col = (s16)rec->rect.x >> 3;
        y = (s16)rec->rect.y >> 3;
        colEnd = rec->rect.cols + col + 1;
        rowEnd = rec->rect.rows + y + 1;
        free_list_move(obj->head, (void *)(rec->unk2C - st->unk194));
        rec->unk2C = 0;
        if (y < rowEnd)
        {
            cells = obj->state;
            for (; y < rowEnd; y++)
            {
                for (x = col; x < colEnd; x++)
                {
                    ((u16 *)cells->map)[(y << 5) + x] = cells->fillValue;
                }
            }
        }
        one = 1;
        obj->state->slotMask &= ~(one << slot);
        obj->state->unk1B4 &= obj->state->slotMask;
        rec->unk8 = 0;
        if (obj->state->slotMask == 0)
        {
            if (((((u32)obj->state->flags) << 29) >> 31) & one)
            {
                *(u16 *)0x02000000 ^= 0x2000;
                *obj->state->hofs = 0;
                *obj->state->vofs = 0;
                obj->state->prevY = 0;
                obj->state->prevX = 0;
                ((struct FlagBits8 *)&obj->state->flags)->bit2 = 0;
            }
        }
    }
}
END_NONMATCH

NONMATCH("asm/code_080E76A4/copy_map_rects.s", void copy_map_rects(struct ObjWithState *obj, u8 mask))
{
    struct ObjState *st;
    struct SlotRect *slot;
    u8 *vram;
    s16 i;

    if (mask != 0)
    {
        st = obj->state;
        vram = (u8 *)((((u32)*st->header & 0x1F00) << 3) + 0x06000000);
        for (i = 0, slot = st->slots; i < 5; i++, slot++)
        {
            u8 skip = ((mask >> i) ^ 1) & 1;
            if (skip == 0)
            {
                u16 x = slot->x >> 3;
                u16 y = slot->y >> 3;
                u32 cell = (y << 5) + x;
                u32 index = cell << 1;
                u8 **pmap = &obj->state->map;
                u8 *dst = *pmap + index;
                u8 *end = *pmap + ((slot->rows + y + 1) << 6) + ((slot->cols + x) << 1) + 4;
                u8 *src = vram + index;
                dma_copy_or_queue(dst, src, end - dst, 0x20, skip);
            }
        }
    }
}
END_NONMATCH

ASM_FUNC("asm/code_080E76A4/set_box_and_flags.s", void set_box_and_flags(void))

ASM_FUNC("asm/code_080E76A4/update_box_and_scroll.s", void update_box_and_scroll(void))

ASM_FUNC("asm/code_080E76A4/emit_box_oam.s", void emit_box_oam(void))

NONMATCH("asm/code_080E76A4/emit_oam_entry.s", void emit_oam_entry(struct ObjWithState *obj, struct SpriteRec *ent))
{
    struct ObjState *st;
    u8 *base;
    u16 **oamPtr;
    u16 *oam;
    s32 xSum;
    s32 t;
    s32 x;
    u16 y;

    if ((ent->flags & 0x38) != 0)
    {
        s32 w44 = ent->ofsX;
        xSum = ent->x + w44;
        st = obj->state;
        xSum += st->prevX;
        y = ent->y + ent->ofsY + st->prevY;
        t = xSum << 16;
        x = t >> 16;
        if ((u32)(t + 0xC0000) <= 0x1080000)
        {
            if ((s16)y >= -12)
            {
                if ((s16)y <= 0xAC)
                {
                    base = gSys;
                    oamPtr = (u16 **)(base + 0x880);
                    oam = *oamPtr;
                    *oam = (((u32)ent->b43 << 29) >> 31 << 15) | ((s16)y & 0xFF);
                    oam++;
                    *oam = ((((u32)ent->b43 << 30) >> 31) << 14) | (x & 0x1FF) | ((ent->b42 >> 7) << 13) | (((u32)ent->b43 << 31) >> 31 << 12);
                    oam++;
                    *oam = (ent->b43 >> 3) | 0xF000;
                    oam += 2;
                    *oamPtr = oam;
                }
            }
        }
    }
}
END_NONMATCH

ASM_FUNC("asm/code_080E76A4/draw_obj_dma_tiles.s", void draw_obj_dma_tiles(void))

extern u8 *gUnk_03000FB4;
extern u8 tile_shift_copy[];
extern u8 tile_shift_upload[];
extern u8 tile_shift_merge[];
extern u8 tile_shift_copy_wide[];
extern const u8 gHeapTagTiles[];
void *heap_alloc_raw(u32 pool, u32 size, const u8 *tag);
void heap_free(void *p);

// One DMA3 transfer of 0x20 words (8x32 units): source, destination, control, then read the control back.
static inline void box_dma_copy(vu32 *dma, u8 *src, u8 *dst)
{
    dma[0] = (u32)src;
    dma[1] = (u32)dst;
    dma[2] = 0x84000008;
    (void)dma[2];
}

NONMATCH("asm/code_080E76A4/upload_box_frame.s", void upload_box_frame(struct ObjWithState *obj, u32 target, u16 x, u16 y, u32 wArg, u32 hArg))
{
    vu32 *dma;
    u8 *src;
    u8 *buf;
    u8 *s1c0;
    u8 *s220;
    u8 *s200;
    u8 *s260;
    s32 w8;
    u32 h;
    u32 wp1;
    u32 off;
    u32 offWide;
    u32 xHi;
    u32 yHi;
    s32 xs;
    s32 ys;
    u32 yRow;
    u32 yRow2;
    u32 yRow3;
    u32 y8;
    u32 left;
    u32 col2;
    u32 x18;
    u32 hm3;
    u16 third;

    dma = (vu32 *)REG_ADDR_DMA3SAD;
    src = *(u8 **)((u8 *)obj->state + 0x180);
    w8 = (u8)wArg;
    h = (u8)hArg;
    wp1 = (u8)(w8 + 1);
    buf = heap_alloc_raw(1, 0x80, gHeapTagTiles);

    box_dma_copy(dma, src + 0x320, buf);
    box_dma_copy(dma, src + 0x340, (buf + 0x20));
    s1c0 = src + 0x1C0;
    box_dma_copy(dma, s1c0, (buf + 0x40));

    off = (u32)tile_shift_upload - (u32)tile_shift_copy;
    xHi = (u32)x << 16;
    yHi = (u32)y << 16;
    xs = (s32)xHi >> 16;
    ys = (s32)yHi >> 16;
    ((void (*)())(gUnk_03000FB4 + off))(target, buf, xHi >> 16, yHi >> 16, wp1, 3);

    box_dma_copy(dma, src + 0x300, buf);
    y8 = (yHi + 0x00080000) >> 16;
    ((void (*)())gUnk_03000FB4)(target, buf, xHi >> 16, y8, wp1);

    box_dma_copy(dma, src + 0x3E0, buf);
    box_dma_copy(dma, src + 0x400, (buf + 0x20));
    s220 = src + 0x220;
    box_dma_copy(dma, s220, (buf + 0x40));
    yRow = (u16)((h - 1) * 8 + ys);
    ((void (*)())(gUnk_03000FB4 + off))(target, buf, xHi >> 16, yRow, wp1, 3);

    box_dma_copy(dma, src + 0x3C0, buf);
    yRow2 = (u16)((h - 2) * 8 + ys);
    ((void (*)())gUnk_03000FB4)(target, buf, xHi >> 16, yRow2, wp1);

    s200 = src + 0x200;
    box_dma_copy(dma, s200, buf);
    box_dma_copy(dma, src + 0x360, (buf + 0x20));
    box_dma_copy(dma, src + 0x380, (buf + 0x40));
    left = (u16)(xs + (w8 - 3) * 8);
    ((void (*)())(gUnk_03000FB4 + off))(target, buf, left, yHi >> 16, wp1, 3);

    box_dma_copy(dma, src + 0x3A0, buf);
    col2 = (u16)(xs + (w8 - 1) * 8);
    ((void (*)())gUnk_03000FB4)(target, buf, col2, y8, wp1);

    s260 = src + 0x260;
    box_dma_copy(dma, s260, buf);
    box_dma_copy(dma, src + 0x420, (buf + 0x20));
    box_dma_copy(dma, src + 0x440, (buf + 0x40));
    ((void (*)())(gUnk_03000FB4 + off))(target, buf, left, yRow, wp1, 3);

    box_dma_copy(dma, src + 0x460, buf);
    ((void (*)())gUnk_03000FB4)(target, buf, col2, yRow2, wp1);

    box_dma_copy(dma, src + 0x280, buf);
    box_dma_copy(dma, src + 0x2A0, (buf + 0x20));
    yRow3 = (u16)(ys + 0x10);
    offWide = (u32)tile_shift_copy_wide - (u32)tile_shift_copy;
    hm3 = ((h - 3) << 15) >> 16;
    ((void (*)())(gUnk_03000FB4 + offWide))(target, buf, xHi >> 16, yRow3, wp1, 2, hm3);

    box_dma_copy(dma, s1c0, buf);
    box_dma_copy(dma, src + 0x1E0, (buf + 0x20));
    box_dma_copy(dma, s200, (buf + 0x40));
    x18 = (u16)(xs + 0x18);
    third = (w8 - 4) / 3;
    ((void (*)())(gUnk_03000FB4 + (u32)tile_shift_merge - (u32)tile_shift_copy))(target, buf, x18, yHi >> 16, wp1, 3, (u16)third);

    box_dma_copy(dma, s220, buf);
    box_dma_copy(dma, src + 0x240, (buf + 0x20));
    box_dma_copy(dma, s260, (buf + 0x40));
    ((void (*)())(gUnk_03000FB4 + (u32)tile_shift_merge - (u32)tile_shift_copy))(target, buf, x18, yRow, wp1, 3, (u16)third);

    box_dma_copy(dma, src + 0x2C0, buf);
    box_dma_copy(dma, src + 0x2E0, (buf + 0x20));
    ((void (*)())(gUnk_03000FB4 + offWide))(target, buf, col2, yRow3, wp1, 2, hm3);

    heap_free(buf);
}
END_NONMATCH

ASM_FUNC("asm/code_080E76A4/dma_tile_upload.s", void dma_tile_upload(void))

void set_obj_fields(u32 unused0, struct ObjFields *obj, u32 unused2, u32 unused3, u8 val48, u16 val44, u16 flags)
{
    s32 mode;
    s32 idx;
    u8 b42;
    u32 bit7;

    if ((flags & 0x38) != 0)
    {
        mode = (flags & 0x700) >> 8;
        if (mode <= 1)
        {
            if (mode >= 0)
            {
                obj->unk44 = val44;
                bit7 = (flags & 0x80) >> 7;
                ((struct Bits43 *)&obj->unk43)->b0 = bit7;
                if (mode == 0)
                {
                    b42 = obj->unk42;
                    b42 |= 0x80;
                    obj->unk42 = b42;
                }
                else
                {
                    obj->unk48 = (u32)val48 << 3;
                }
                obj->unk48 -= 8;
                idx = ((flags & 0x78) >> 3) - 1;
                switch (idx)
                {
                case 0:
                case 2:
                    ((struct Bits43 *)&obj->unk43)->hi5 = 5;
                    ((struct Bits43 *)&obj->unk43)->b2 = 1;
                    obj->unk44 -= 4;
                    break;
                case 8:
                case 10:
                    ((struct Bits43 *)&obj->unk43)->hi5 = 9;
                    ((struct Bits43 *)&obj->unk43)->b1 = 1;
                    obj->unk44 -= 8;
                    break;
                case 1:
                    ((struct Bits43 *)&obj->unk43)->hi5 = 7;
                    ((struct Bits43 *)&obj->unk43)->b2 = 1;
                    obj->unk44 -= 4;
                    break;
                case 9:
                    ((struct Bits43 *)&obj->unk43)->hi5 = 13;
                    ((struct Bits43 *)&obj->unk43)->b1 = 1;
                    obj->unk44 -= 8;
                    break;
                default:
                    break;
                }
            }
        }
    }
}

ASM_FUNC("asm/code_080E76A4/slot_tile_upload_draw_obj_tiles.s", void slot_tile_upload_draw_obj_tiles(void))
