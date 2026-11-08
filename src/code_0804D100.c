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
    u8 unk24;
    u8 unk25;
    u8 filler26[0x28 - 0x26];
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
    u8 unk4C;
    u8 filler4D[0x54 - 0x4D];
    u8 unk54;
    u8 filler55[0x20C - 0x55];
    u8 unk20C;
    u8 unk20D;
    u8 unk20E;
    u8 unk20F;
    u8 unk210;
    u8 filler211[0x216 - 0x211];
    u8 unk216;
    u8 filler217[0x242 - 0x217];
    s16 unk242;
    s16 unk244;
    u8 filler246[0x270 - 0x246];
    s16 unk270;
    u16 unk272;
    u16 unk274;
    u16 unk276;
    u16 unk278;
    u16 unk27A;
    u16 unk27C;
    u8 filler27E[0x290 - 0x27E];
    u8 *layer290;
    u8 *unk294;
    u8 filler298[0x33C - 0x298];
    u8 unk33C;
    u8 filler33D;
    u8 unk33E;
    u8 filler33F[0x342 - 0x33F];
    u8 unk342;
    u8 filler343;
    struct TileObj *unk344;
    s32 unk348;
    s32 unk34C;
    u8 unk350;
    u8 filler351;
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

typedef union __attribute__((packed)) { u8 value; } ByteArg __attribute__((transparent_union));
u8 *tile_query(u8 *world, s32 x, s32 y, s32 *outA, s32 *outB, s32 *outC, ByteArg adjust, ByteArg flag);
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

NONMATCH("asm/code_0804D100/obj_tile_corners_check.s", u32 obj_tile_corners_check(struct TileObj *obj))
{
    s32 outA;
    s32 outB;
    s32 outC;
    s32 x0;
    s32 x1;
    s32 yLo;
    s32 y;
    s32 h;
    s32 zTop;
    u32 w;
    s32 t;

    w = obj->unk278;
    x0 = obj->unkC - (w >> 1);
    t = x0 - 0x100;
    x1 = t + w;
    y = obj->unk10;
    t = y + 0x100;
    yLo = t - obj->unk27C;
    h = obj->unk14 + obj->unk18;
    if (h < 0)
        h = 0;
    zTop = obj->unk27A + h;

    tile_query(obj->layer290, x0, yLo, &outA, &outB, &outC, (u8)1, (u8)1);
    if (h < outA)
        return 1;
    if (h < outC && zTop >= outB)
        return 1;

    tile_query(obj->layer290, x1, yLo, &outA, &outB, &outC, (u8)1, (u8)1);
    if (h < outA)
        return 1;
    if (h < outC && zTop >= outB)
        return 1;

    tile_query(obj->layer290, x0, y, &outA, &outB, &outC, (u8)1, (u8)1);
    if (h < outA)
        return 1;
    if (h < outC && zTop >= outB)
        return 1;

    tile_query(obj->layer290, x1, y, &outA, &outB, &outC, (u8)1, (u8)1);
    if (h < outA)
        return 1;
    if (h >= outC || zTop < outB)
        return 0;
    return 1;
}
END_NONMATCH

NONMATCH("asm/code_0804D100/snap_to_tile_edge.s", s32 snap_to_tile_edge(struct TileObj *obj))
{
    s32 outA;
    s32 outB;
    s32 outC;
    s32 hits;
    s32 zTop;
    s32 top;
    s32 probeX;
    s32 probeY;
    s32 snapY;
    s32 snapX;
    s32 edgeY;
    s32 posX;
    s32 w;
    u32 move;
    u32 nibble;
    u32 bit;
    u32 left;
    u8 dirs;

    move = obj->unk25;
    if ((move & 0x78) == 0)
        return -1;
    if ((obj->unk4C & 0x78) != 0)
        return -1;
    if (obj->unk242 != 0 && obj->unk244 != 0)
        return -1;

    nibble = (move << 25) >> 28;
    dirs = nibble & 1;
    bit = 1;
    left = 4;
    while (1)
    {
        bit = (bit << 25) >> 24;
        left--;
        if (left == 0)
            break;
        if ((nibble & bit) == 0)
            continue;
        dirs++;
        if (dirs == 2)
            break;
    }
    if (dirs != 1)
        return -1;

    hits = 0;
    zTop = obj->unk14 + obj->unk18;
    top = obj->unk27A + zTop;

    if (obj->unk242 != 0)
    {
        if (obj->unk34C != obj->unk10)
            return -1;
        if (obj->unk242 < 0)
            probeX = obj->unk348 - (obj->unk278 >> 1);
        else
            probeX = (obj->unk348 - 0x100 + (obj->unk278 >> 1) + 0xFF) & 0xFFFFFF00;

        probeY = obj->unk34C + 0x100 - obj->unk27C;
        snapY = ((obj->unk34C + 0x100 + 0xFF) & 0xFFFFFF00) - 0x100;

        tile_query(obj->layer290, probeX, probeY, &outA, &outB, &outC, (u8)1, (u8)((obj->unk20D >> 3) & 1));
        if (zTop < outA)
            hits = 1;
        else if (outB >= 0 && top > outB && zTop < outC)
            hits = 1;
        else if ((obj->unk20D & 0x20) != 0 && zTop > outA && zTop != outC)
            hits = 1;

        tile_query(obj->layer290, probeX, snapY, &outA, &outB, &outC, (u8)1, (u8)((obj->unk20D >> 3) & 1));
        if (zTop < outA)
            hits |= 8;
        else if (outB >= 0 && top > outB && zTop < outC)
            hits |= 8;
        else if ((obj->unk20D & 0x20) != 0 && zTop > outA && zTop != outC)
            hits |= 8;

        if (hits == 1)
        {
            obj->unk10 = (obj->unk10 + 0x100) & 0xFFFFFF00;
            return 4;
        }
        if (hits != 8)
            return -1;
        obj->unk10 = ((obj->unk10 + 0xFF) & 0xFFFFFF00) - 0x100;
        return 0;
    }

    if (obj->unk244 == 0)
        return -1;
    posX = obj->unkC;
    if (obj->unk348 != posX)
        return -1;
    if (obj->unk244 > 0)
        probeY = obj->unk10 + 0x100;
    else
        probeY = obj->unk10 - obj->unk27C;
    w = obj->unk278;
    snapX = (posX - (w >> 1) + w + 0xFF) & 0xFFFFFF00;

    tile_query(obj->layer290, posX - (w >> 1), probeY, &outA, &outB, &outC, (u8)1, (u8)((obj->unk20D >> 3) & 1));
    if (zTop < outA)
        hits = 1;
    else if (outB >= 0 && top > outB && zTop < outC)
        hits = 1;
    else if ((obj->unk20D & 0x20) != 0 && zTop > outA && zTop != outC)
        hits = 1;

    tile_query(obj->layer290, posX - 0x100, probeY, &outA, &outB, &outC, (u8)1, (u8)((obj->unk20D >> 3) & 1));
    if (zTop < outA)
        hits |= 2;
    else if (outB >= 0 && top > outB && zTop < outC)
        hits |= 2;
    else if ((obj->unk20D & 0x20) != 0 && zTop > outA && zTop != outC)
        hits |= 2;

    tile_query(obj->layer290, posX, probeY, &outA, &outB, &outC, (u8)1, (u8)((obj->unk20D >> 3) & 1));
    if (zTop < outA)
        hits |= 4;
    else if (outB >= 0 && top > outB && zTop < outC)
        hits |= 4;
    else if ((obj->unk20D & 0x20) != 0 && zTop > outA && zTop != outC)
        hits |= 4;

    tile_query(obj->layer290, snapX, probeY, &outA, &outB, &outC, (u8)1, (u8)((obj->unk20D >> 3) & 1));
    if (zTop < outA)
        hits |= 8;
    else if (outB >= 0 && top > outB && zTop < outC)
        hits |= 8;
    else if ((obj->unk20D & 0x20) != 0 && zTop > outA && zTop != outC)
        hits |= 8;

    if (hits == 1)
    {
        obj->unkC = (obj->unkC + 0x200) & 0xFFFFFE00;
        return 2;
    }
    if (hits != 8)
        return -1;
    obj->unkC = (obj->unkC - 0x100) & 0xFFFFFE00;
    return 6;
}
END_NONMATCH

extern const s16 gBoxOffsetTable[];
extern const u8 gDirBits23b8[];
extern const u8 gDirBits23c0[];

NONMATCH("asm/code_0804D100/find_obj_contact.s", u32 find_obj_contact(struct TileObj *obj, u32 mode, struct TileObj **list, u32 count))
{
    s32 zLo;
    s32 zHi;
    s32 boxXLo;
    s32 boxX2Lo;
    s32 boxXHi;
    s32 boxX2Hi;
    s32 boxYLo;
    s32 boxY2Lo;
    s32 boxYHi;
    s32 boxY2Hi;
    s32 rest;
    s32 depth;
    s32 posX;
    s32 posY;
    s32 zTop;
    s32 zCur;
    s32 dirShift;
    s32 w;
    s32 left;
    s32 top;
    s32 bottom;
    s32 boxOk;
    s32 kindOk;
    s32 field;
    u32 off;
    u32 sel;
    const u8 *bt;
    struct TileObj **lp;
    struct TileObj *cur;
    struct TileObj *child;

    if (mode != 1)
    {
        kindOk = 0;
        if ((obj->flags35B & 8) != 0)
        {
            if (obj->unk294[0x23] > 2)
            {
                if (obj->unk294[0x23] <= 4)
                    kindOk = 1;
            }
        }
        if (kindOk == 0)
        {
            if ((u8)(obj->unk294[0x23] - 3) > 1)
                return 0;
        }
    }

    if (mode == 0)
    {
        if ((obj->flags35B & 8) != 0)
        {
            if (obj->unk294[0x23] == 3)
            {
                zLo = 0x1800;
                zHi = 0x100;
            }
            else
            {
                zLo = 0x1000;
                zHi = 0;
            }
        }
        else
        {
            if (obj->unk294[0x23] == 3)
            {
                zLo = 0x2800;
                zHi = 0x1000;
            }
            else
            {
                zLo = 0x1000;
                zHi = 0;
            }
        }
    }
    else
    {
        zHi = 0x100;
        if ((obj->flags35B & 8) != 0)
            zLo = 0x1000;
        else
            zLo = 0x2000;
    }

    posX = obj->unkC;
    posY = obj->unk10;
    dirShift = (obj->unk24 & 7) << 3;
    bt = (const u8 *)gBoxOffsetTable;
    off = dirShift + (mode << 7);
    boxXLo = posX + *(const s16 *)(bt + off);
    boxXHi = posX + *(const s16 *)(bt + off + 2);
    boxYLo = posY + *(const s16 *)(bt + off + 4);
    boxYHi = posY + *(const s16 *)(bt + off + 6);
    boxX2Lo = posX + *(const s16 *)(bt + off + 64);
    boxX2Hi = posX + *(const s16 *)(bt + off + 66);
    boxY2Lo = posY + *(const s16 *)(bt + off + 68);
    boxY2Hi = posY + *(const s16 *)(bt + off + 70);

    zTop = obj->unk14 + obj->unk18;
    zLo += zTop;
    zHi += zTop;

    rest = count;
    if (rest == 0)
        return 0;

    lp = list;
    for (;;)
    {
        cur = *lp;
        depth = 0;
        for (;;)
        {
            if (cur->unk54 != obj->unk54 && (cur->unk20C & 1) != 0 && (cur->unk216 & 0x20) == 0
                && (cur->unk20E & 0x38) != 0x30 && (cur->unk20C & 0xA0) != 0)
            {
                sel = cur->unk20E & 0x38;
                boxOk = 0;
                w = cur->unk272;
                left = cur->unkC - (w >> 1);
                top = cur->unk10 + cur->unk270;
                bottom = top - cur->unk276;
                if (sel == 0x18)
                {
                    if (boxXLo < left + w && boxXHi > left && boxYLo > bottom && boxYHi < top)
                        boxOk = 1;
                }
                else
                {
                    if (boxX2Lo < left + w && boxX2Hi > left && boxY2Lo > bottom && boxY2Hi < top)
                        boxOk = 1;
                }

                if (boxOk)
                {
                    zCur = cur->unk14 + cur->unk18;
                    if (zHi < zCur + cur->unk274 && zLo > zCur)
                    {
                        s32 dirWrite;

                        if (mode == 0)
                            dirWrite = (obj->unk20D & 1) && (cur->unk210 & 0x10);
                        else
                            dirWrite = (obj->unk20D & 1) && (cur->unk20C & 0x20);
                        if (dirWrite)
                            obj->unk24 = (obj->unk24 & 0x87)
                                | ((((obj->unk24 >> 3) | gDirBits23b8[obj->unk24 & 7]) & 0xF) << 3);

                        if (((cur->unk20C >> 6) & 1) && (obj->unk20D & 2))
                        {
                            struct TileObj *ent = *lp;
                            u8 e = ent->unk24;
                            ent->unk24 = (e & 0x87)
                                | ((((e >> 3) | gDirBits23c0[obj->unk24 & 7]) & 0xF) << 3);
                        }

                        if ((obj->unk20F & 1) != 0 && (obj->unk20D & 1) && (cur->unk20C & 0x80) != 0)
                        {
                            if ((obj->unk350 & 0xF) == 0)
                            {
                                field = (obj->unk33C & 0xFFFFFF80) | (cur->unk54 & 0x7F);
                                field = (field & 0x7F) | (depth << 7);
                                obj->unk33C = field;
                                obj->unk350 = (obj->unk350 & 0xF0) | (gDirBits23b8[obj->unk24 & 7] & 0xF);
                                obj->unk342 = mode;
                            }
                        }

                        if (mode == 0)
                        {
                            if ((cur->unk210 & 0x10) != 0)
                                return 1;
                        }
                        else if ((cur->unk20C & 0x20) != 0)
                        {
                            return 1;
                        }
                    }
                }
            }

            if ((cur->unk20E & 0x38) != 0x18)
                break;
            child = cur->unk344;
            if (child == 0)
                break;
            if ((child->unk20C & 1) == 0)
                break;
            cur = child;
            depth++;
            if (depth > 1)
                break;
        }

        rest--;
        lp++;
        if (rest == 0)
            return 0;
    }
}
END_NONMATCH


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
