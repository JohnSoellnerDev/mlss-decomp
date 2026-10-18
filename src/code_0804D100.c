#include "global.h"

struct TileObj
{
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 filler3[0x8 - 0x3];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    u16 unk28;
    s16 unk2A;
    s16 unk2C;
    u16 unk2E;
    u16 unk30;
    u16 unk32;
    s32 unk34;
    s32 unk38;
    u8 filler3C[0x3E - 0x3C];
    s8 unk3E;
    u8 filler3F[0x44 - 0x3F];
    u8 *unk44;
    void *unk48;
    u8 filler4C[0x54 - 0x4C];
    u8 unk54;
    u8 filler55[0x20C - 0x55];
    u8 unk20C;
    u8 unk20D;
    u8 unk20E;
    u8 filler20F[0x270 - 0x20F];
    s16 unk270;
    u16 unk272;
    u8 filler274[0x276 - 0x274];
    u16 unk276;
    u16 unk278;
    u16 unk27A;
    u16 unk27C;
    u8 filler27E[0x290 - 0x27E];
    u8 *layer290;
    u8 filler294[0x33C - 0x294];
    u8 unk33C;
    u8 filler33D;
    u8 unk33E;
    u8 filler33F[0x344 - 0x33F];
    struct TileObj *unk344;
    u8 filler348[0x352 - 0x348];
    u8 unk352;
    u8 filler353[0x35B - 0x353];
    u8 flags35B;
    u8 filler35C[0x368 - 0x35C];
    u8 *unk368;
};

struct SpriteFlags11
{
    u8 filler0[0x11];
    u8 lo6 : 6;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

struct SpriteFlags12
{
    u8 filler0[0x12];
    u8 lo1 : 1;
    u8 mode : 2;
    u8 hi5 : 5;
};

struct ObjFlags0
{
    u8 lo6 : 6;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

u32 tile_ptr_at(u8 *obj, s16 x, s16 y);
void *create_obj_de4(u32 a, u32 b, u32 c, s32 d, s32 e, s32 f, s32 g);
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
void find_or_alloc_obj_slots(u32 a, u32 b, u32 c, u32 d, u32 e, void *f, s32 g);
void obj_queue_update(void *sprite);
extern const u16 gAttachedSpriteProtoTable[];
extern const u8 gFrameAttrTable[];
void resolve_obj_overlap(struct TileObj *obj, void *list, u32 count);
u32 obj_snap_to_tile_walls(struct TileObj *obj);
void sweep_obj_motion(struct TileObj *obj);
void obj_release_oam(void *obj);

s32 obj_on_tile_18(struct TileObj *obj)
{
    u8 **slot;
    u8 *layer;
    u8 *tile;

    if (obj->unk54 != 0)
        return 0;
    if ((obj->flags35B & 8) == 0)
        return 0;
    slot = &obj->layer290;
    layer = *slot;
    tile = (u8 *)tile_ptr_at(layer, (s16)(obj->unk34 / 0x1000), (s16)(obj->unk38 / 0x1000));
    if ((*tile & 0x78) == 0x18)
        return 1;
    tile = (u8 *)tile_ptr_at(*slot, (s16)(obj->unk34 / 0x1000), (s16)((obj->unk38 - obj->unk27C) / 0x1000));
    if ((*tile & 0x78) != 0x18)
        return 0;
    return 1;
}

ASM_FUNC("asm/code_0804D100/obj_tile_corners_check.s", void obj_tile_corners_check(void))

ASM_FUNC("asm/code_0804D100/snap_to_tile_edge.s", void snap_to_tile_edge(void))

ASM_FUNC("asm/code_0804D100/find_obj_contact.s", void find_obj_contact(void))

ASM_FUNC("asm/code_0804D100/probe_obj_cells.s", void probe_obj_cells(void))

ASM_FUNC("asm/code_0804D100/obj_scan_contacts.s", void obj_scan_contacts(void))

void attached_sprite_obj_setup_sprite(struct TileObj *obj)
{
    u32 slot;
    u8 *sprite;
    u32 kind;
    u32 anim;
    s32 rest;
    u32 flip;
    u8 entry;
    u32 b0;

    b0 = obj->unk0;
    if ((b0 & 0x40) == 0)
    {
        obj->unk44 = create_obj_de4(0, -1, 1, -1, -1, -1, -1);
        return;
    }
    kind = gAttachedSpriteProtoTable[((b0 << 26) >> 29) - 2];
    find_or_alloc_obj_slots(0, 0x1004, 0, 0, 0, &slot, 0xc);
    sprite = create_obj_de4(0, kind, 1, -1, -1, -1, -1);
    obj->unk44 = sprite;
    entry = gFrameAttrTable[0x20 + ((((u32)obj->unk1) << 26) >> 29)];
    anim = entry & 0x7F;
    flip = entry >> 7;
    obj_load_record(sprite, anim, -1, 0, 0);
    obj_queue_update(obj->unk44);
    if (obj->unk3E != 0)
    {
        rest = obj->unk3E;
        while (rest >= obj->unk44[0x25])
        {
            rest -= obj->unk44[0x25];
        }
        obj_load_record(obj->unk44, anim, rest, 0, 0);
    }
    ((struct SpriteFlags11 *)obj->unk44)->bit6 = flip;
}

ASM_FUNC("asm/code_0804D100/obj_init_from_table.s", void obj_init_from_table(void))

void obj_advance_step(struct TileObj *obj, void *list, u32 count)
{
    u8 *sprite;
    s32 rest;
    u32 cnt;

    if ((obj->unk0 & 0x40) != 0)
    {
        sprite = obj->unk44;
        if (sprite[0x21] > 4)
        {
            if ((sprite[0x12] & 0x20) != 0)
                ((struct ObjFlags0 *)obj)->bit6 = 0;
        }
        else if ((obj->unk0 & 0x38) == 0x20)
        {
            obj_load_record(sprite, sprite[0x21] + 5, 0, 0, 0);
            resolve_obj_overlap(obj, list, count);
        }
        else
        {
            obj->unk8 += obj->unk2A;
            obj->unkC += obj->unk2C;
            cnt = obj->unk28;
            cnt += obj->unk2E;
            obj->unk2E = cnt;
            if ((u16)cnt > obj->unk30)
            {
                obj_release_oam(obj);
            }
            else
            {
                resolve_obj_overlap(obj, list, count);
                if ((u8)obj_snap_to_tile_walls(obj) != 0 || (obj->unk2 & 0xF) != 0)
                {
                    obj_load_record(obj->unk44, obj->unk44[0x21] + 5, 0, 0, 0);
                    if (obj->unk3E != 0)
                    {
                        rest = obj->unk3E;
                        while (rest >= obj->unk44[0x25])
                        {
                            rest -= obj->unk44[0x25];
                        }
                        obj_load_record(obj->unk44, obj->unk44[0x21], rest, 0, 0);
                    }
                    ((struct SpriteFlags12 *)obj->unk44)->mode = 2;
                }
                sweep_obj_motion(obj);
            }
        }
    }
}

ASM_FUNC("asm/code_0804D100/update_footprint_level.s", void update_footprint_level(void))

ASM_FUNC("asm/code_0804D100/find_entry_containing.s", void find_entry_containing(void))

ASM_FUNC("asm/code_0804D100/resolve_obj_overlap.s", void resolve_obj_overlap(struct TileObj *obj, void *list, u32 count))
