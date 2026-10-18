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

ASM_FUNC("asm/code_080E76A4/clear_active_slot_rects.s", void clear_active_slot_rects(void))

ASM_FUNC("asm/code_080E76A4/clear_slot_rect.s", void clear_slot_rect(void))

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
            s32 skip = mask >> i;
            skip ^= 1;
            skip &= 1;
            if (skip == 0)
            {
                u16 x = slot->x >> 3;
                u16 y = slot->y >> 3;
                u32 cell = (y << 5) + x;
                u8 **pmap = &obj->state->map;
                u32 index = cell << 1;
                u8 *dst = *pmap + index;
                u8 *src = vram + index;
                u8 *end = *pmap + ((slot->rows + y + 1) << 6) + ((slot->cols + x) << 1) + 4;
                dma_copy_or_queue(dst, src, end - dst, 0x20, skip);
            }
        }
    }
}
END_NONMATCH

ASM_FUNC("asm/code_080E76A4/set_box_and_flags.s", void set_box_and_flags(void))

ASM_FUNC("asm/code_080E76A4/update_box_and_scroll.s", void update_box_and_scroll(void))

ASM_FUNC("asm/code_080E76A4/emit_box_oam.s", void emit_box_oam(void))

ASM_FUNC("asm/code_080E76A4/emit_oam_entry.s", void emit_oam_entry(void))

ASM_FUNC("asm/code_080E76A4/draw_obj_dma_tiles.s", void draw_obj_dma_tiles(void))

ASM_FUNC("asm/code_080E76A4/upload_box_frame.s", void upload_box_frame(void))

ASM_FUNC("asm/code_080E76A4/dma_tile_upload.s", void dma_tile_upload(void))

ASM_FUNC("asm/code_080E76A4/set_obj_fields.s", void set_obj_fields(void))

ASM_FUNC("asm/code_080E76A4/slot_tile_upload_draw_obj_tiles.s", void slot_tile_upload_draw_obj_tiles(void))
