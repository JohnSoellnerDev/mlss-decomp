#include "global.h"

struct PairSpr
{
    u8 filler0[2];
    s16 half2;
    u8 filler4[0x12 - 0x04];
    u8 flags12;
    u8 filler13[0x1F - 0x13];
    u8 flags1F;
    u8 value20;
};

struct PairSub
{
    u8 filler0[4];
    s16 state4;
    u8 filler6[0x0C - 0x06];
    s32 posX;
    s32 posY;
    s32 posZ;
    s32 posZB;
    u8 filler1C[0x24 - 0x1C];
    u8 cell24;
    u8 filler25;
    u8 type26;
    u8 filler27[0x54 - 0x27];
    u8 group54;
    u8 filler55[0x20D - 0x55];
    u8 flags20D;
    u8 filler20E;
    u8 state20E;
    u8 filler20F;
    u8 flags210;
    u8 filler211;
    u8 seqFlags212;
    u8 filler213;
    u8 stateBits214;
    u8 filler215[0x294 - 0x215];
    struct PairSpr *sprite;
    u8 filler298[0x33C - 0x298];
    u8 slot33C;
    u8 filler33D;
    s8 entryIndex33E;
    u8 filler33F[0x351 - 0x33F];
    u8 flags351;
    u8 mode352;
    u8 filler353[0x358 - 0x353];
    u8 flags358;
    u8 filler359;
    u8 flags35A;
    u8 flags35B;
    u8 filler35C[0x368 - 0x35C];
    u8 *frames;
    u8 filler36C[0x388 - 0x36C];
    struct PairSub *partner;
};

struct PairObj
{
    u8 filler0[0x28];
    u32 entries[32];
    u8 filler0A8[0xFB - 0xA8];
    u8 flagFB;
    u8 filler0FC[0x100 - 0xFC];
    s16 move100;
    u8 filler102[0x200 - 0x102];
    struct PairSub *subA;
    struct PairSub *subB;
    u8 flags208;
    u8 filler209[0x20B - 0x209];
    u8 select20B;
    u8 filler20C[0x246 - 0x20C];
    u8 state246;
    u8 filler247;
    s16 pixX;
    s16 pixY;
    u8 filler24C[0x252 - 0x24C];
    u8 field252;
    u8 filler253;
    s32 field254;
    u16 field258;
    u8 filler25A[0x25C - 0x25A];
    u8 field25C;
    u8 filler25D[0x272 - 0x25D];
    u16 field272;
    u8 filler274[0x304 - 0x274];
    void *map304;
};

struct StateLo3
{
    u8 lo3 : 3;
    u8 rest : 5;
};

void update_pair_facing(void *obj, u32 a, u32 b);
void obj_shift_state_slots(void *sub);
void advance_anim_frame(void *obj, void *sub, u32 a, u32 b);
void motion_obj_set_move_target(void *obj, u32 a, s32 b, s32 c);
void contacts_obj_start_move(void *obj, s32 a, s32 b, s32 x, s32 y, s32 zero, s32 mode);
void obj_set_id(void *sub, u32 id);
void sound_stop_song(u32 song);
void set_slot_value(u32 id, s32 value);
u32 obj_find_hit_entry(void *obj, void *sub);
u32 drop_roll(u32 item, u32 kind, u32 group);
void obj_draw_desc_entries(void *obj, s32 x, s32 y, s32 z, s32 w, s32 amount, u8 flag, u8 one);
void obj_build_message(void *obj, u32 a, u32 amount, u32 item, u32 wide);
void set_flag(void *g, u32 a, u32 id, u32 b);
void *tile_ptr_at(void *map, u32 x, u32 y);
void layer3_entries_by_group(void *map, u32 group);
void obj_reset_movement(void *ent);
void attach_sprite_setup(void *sub);
extern u8 *gUnk_03000FC0;

static inline struct PairSub *pair_entry(struct PairObj *obj, struct PairSub *sub)
{
    s32 ofs = ((s32)(sub->slot33C << 25)) >> 23;
    return *(struct PairSub **)((u8 *)obj->entries + ofs);
}

NONMATCH("asm/code_08032528/pair_start_state_1d.s", void pair_start_state_1d(struct PairObj *obj))
{
    struct PairSub **aSlot;
    struct PairSub **bSlot;
    s32 maxDist;
    s32 dist;
    s32 dz;
    s32 x;
    s32 y;
    u32 flagsFb;

    update_pair_facing(obj, 1, 1);
    aSlot = &obj->subA;
    obj_shift_state_slots(*aSlot);
    advance_anim_frame(obj, *aSlot, 0, 0);
    obj->state246 &= ~2;
    bSlot = &obj->subB;
    x = (*bSlot)->posX;
    if (x < 0)
        x += 0xFF;
    obj->pixX = x >> 8;
    y = (*bSlot)->posY;
    if (y < 0)
        y += 0xFF;
    obj->pixY = y >> 8;
    *(*aSlot)->frames = 4;
    *(*bSlot)->frames = 4;
    (*aSlot)->state4 = 0x1D;
    (*bSlot)->state4 = 0x1D;
    (*aSlot)->flags351 |= 0x40;
    (*bSlot)->flags351 |= 0x40;
    (*aSlot)->flags358 |= 8;
    (*bSlot)->flags358 |= 8;
    obj->field258 = 0;
    obj->state246 = (obj->state246 & 0x1F) | (((*aSlot)->cell24 & 7) << 5);
    maxDist = 0x24;
    if ((*aSlot)->posZ + (*aSlot)->posZB > (*bSlot)->posZ + (*bSlot)->posZB) {
        if (((((*aSlot)->type26 & (*bSlot)->type26) << 26) >> 30) != 0) {
            maxDist = (*aSlot)->posZ - (*bSlot)->posZ + 0x24;
        }
    }
    dz = ((*aSlot)->posZ + (*aSlot)->posZB) - ((*bSlot)->posZ + (*bSlot)->posZB);
    if (dz < 0)
        dz += 0xFF;
    dist = (dz >> 8) + 0x24;
    if (dist > maxDist)
        dist = maxDist;
    flagsFb = obj->flagFB;
    motion_obj_set_move_target(*bSlot, dist, 0xEE >> ((flagsFb >> 2) & 1), obj->move100);
    x = (*aSlot)->posX;
    if (x < 0)
        x += 0xFF;
    y = (*aSlot)->posY;
    if (y < 0)
        y += 0xFF;
    contacts_obj_start_move(*bSlot, 2, 0, x >> 8, y >> 8, 0, (flagsFb & 4) ? 0xE : 8);
    obj->select20B |= 0x80;
    set_slot_value(0x2B, -1);
}
END_NONMATCH

NONMATCH("asm/code_08032528/pair_start_state_27.s", void pair_start_state_27(struct PairObj *obj))
{
    struct PairSub **aSlot;
    struct PairSub **bSlot;
    struct PairSub *e;
    u32 *entries;
    s32 aPix;
    s32 ePix;
    s32 top;

    entries = obj->entries;
    aSlot = &obj->subA;
    if ((*aSlot)->state4 != 0x23 && (*aSlot)->state4 != 0x29)
        return;
    aPix = ((*aSlot)->posZ + (*aSlot)->posZB) / 0x100;
    e = *(struct PairSub **)((u8 *)entries + (((s32)((*aSlot)->slot33C << 25)) >> 23));
    ePix = (e->posZ + e->posZB) / 0x100;
    top = aPix + 8;
    if (top < ePix)
        return;
    if (top - 0x10 > ePix)
        return;
    bSlot = &obj->subB;
    (*aSlot)->state4 = 0x27;
    (*bSlot)->state4 = 0x27;
    (*aSlot)->flags351 |= 0x40;
    (*bSlot)->flags351 |= 0x40;
    e = *(struct PairSub **)((u8 *)entries + (((s32)((*aSlot)->slot33C << 25)) >> 23));
    contacts_obj_start_move(*aSlot, 1, 0, e->posX / 0x100, e->posY / 0x100,
        ((*aSlot)->posZ + (*aSlot)->posZB) / 0x100, 0);
    *(u16 *)((u8 *)obj + 0x20A) = (*(u16 *)((u8 *)obj + 0x20A) & 0xFE7F) | 0x80;
    (*aSlot)->flags20D |= 1;
    (*aSlot)->flags20D &= ~0x10;
    {
        u32 bits = (pair_entry(obj, *aSlot)->sprite->flags1F >> 2) & 3;
        struct PairSpr *aSprite = (*aSlot)->sprite;
        aSprite->flags1F = (((aSprite->flags1F & ~0xC) | (bits << 2)) & ~3) | bits;
    }
    (*bSlot)->flags20D &= ~0x10;
    {
        u32 bits3 = pair_entry(obj, *aSlot)->sprite->flags1F & 3;
        struct PairSpr *bSprite = (*bSlot)->sprite;
        bSprite->flags1F = (((bSprite->flags1F & ~0xC) | (bits3 << 2)) & ~3) | bits3;
    }
    sound_stop_song(0x5D);
    set_slot_value(0x8E, -1);
}
END_NONMATCH

NONMATCH("asm/code_08032528/pair_start_state_2a.s", void pair_start_state_2a(struct PairObj *obj))
{
    struct PairSub **aSlot;
    struct PairSub **bSlot;
    struct PairSub *e;
    s32 slot;
    s32 x;
    s32 y;
    s32 z;
    u32 bits;

    aSlot = &obj->subA;
    if ((u16)((u16)(*aSlot)->state4 - 0x21) > 3 && (*aSlot)->state4 != 0x2C)
        return;
    slot = ((s32)((*aSlot)->slot33C << 25)) >> 25;
    e = *(struct PairSub **)((u8 *)obj->entries + (slot << 2));
    obj->field25C = slot;
    obj->field254 = e->posZ + e->posZB + 0x6000;
    bSlot = &obj->subB;
    (*aSlot)->state4 = 0x2A;
    (*bSlot)->state4 = 0x2A;
    (*aSlot)->flags351 |= 0x40;
    (*bSlot)->flags351 |= 0x40;
    x = e->posX;
    if (x < 0)
        x += 0xFF;
    y = e->posY;
    if (y < 0)
        y += 0xFF;
    z = (*aSlot)->posZ + (*aSlot)->posZB;
    if (z < 0)
        z += 0xFF;
    contacts_obj_start_move(*aSlot, 1, 0, x >> 8, y >> 8, z >> 8, 0);
    *(u16 *)((u8 *)obj + 0x20A) = (*(u16 *)((u8 *)obj + 0x20A) & 0xFE7F) | 0x80;
    obj->field252 = 2;
    (*aSlot)->flags20D |= 1;
    (*aSlot)->flags20D &= ~0x10;
    bits = (e->sprite->flags1F >> 2) & 3;
    (*aSlot)->sprite->flags1F = ((((*aSlot)->sprite->flags1F & ~0xC) | (bits << 2)) & ~3) | bits;
    (*bSlot)->flags20D &= ~0x10;
    (*bSlot)->sprite->flags1F = ((((*bSlot)->sprite->flags1F & ~0xC) | (bits << 2)) & ~3) | bits;
    e->sprite->flags1F = (e->sprite->flags1F & ~3) | ((((e->sprite->flags1F >> 2) & 3) - 1) & 3);
    e->flags20D &= ~0x10;
    (*aSlot)->flags20D &= ~0x40;
    ((struct StateLo3 *)&(*aSlot)->stateBits214)->lo3 = 2;
    ((struct StateLo3 *)&(*bSlot)->stateBits214)->lo3 = 2;
}
END_NONMATCH


ASM_FUNC("asm/code_08032528/pair_frame4_step.s", void pair_frame4_step(void))

NONMATCH("asm/code_08032528/pair_start_state_3b.s", void pair_start_state_3b(struct PairObj *obj))
{
    struct PairSub **aSlot;
    struct PairSub **bSlot;
    u32 flagsFb;

    update_pair_facing(obj, 1, 1);
    obj_shift_state_slots(obj->subA);
    advance_anim_frame(obj, obj->subA, 0, 0);
    aSlot = &obj->subA;
    bSlot = &obj->subB;
    *(*aSlot)->frames = 6;
    *(*bSlot)->frames = 6;
    (*aSlot)->state4 = 0x3B;
    (*bSlot)->state4 = 0x3B;
    (*aSlot)->flags351 |= 0x40;
    (*bSlot)->flags351 |= 0x40;
    (*aSlot)->flags358 |= 8;
    (*bSlot)->flags358 |= 8;
    (*aSlot)->flags35B |= 0x10;
    (*aSlot)->flags20D |= 0x20;
    (*aSlot)->flags210 &= ~2;
    (*aSlot)->flags210 &= ~4;
    (*bSlot)->seqFlags212 |= 0x40;
    (*bSlot)->flags20D |= 0x20;
    (*bSlot)->flags35A |= 2;
    obj->flags208 &= ~1;
    obj->select20B |= 0x80;
    ((struct StateLo3 *)&(*bSlot)->stateBits214)->lo3 = 4;
    obj_set_id(*bSlot, 0x1025);
    flagsFb = obj->flagFB;
    motion_obj_set_move_target(*bSlot, 0x10, 0xEE >> ((flagsFb >> 2) & 1), obj->move100);
    ((struct StateLo3 *)&(*bSlot)->sprite->flags12)->lo3 = 2;
    (*aSlot)->frames[0x12] = (*aSlot)->frames[0x10];
    (*aSlot)->frames[0x10] = 0;
    obj->state246 = (obj->state246 & ~0x1C) | 0x0C;
    set_slot_value(0x3C, -1);
    obj->field272 = (flagsFb & 4) ? 0x14 : 0x0A;
}
END_NONMATCH


struct HitEntry
{
    u8 flags;
    u8 tileX;
    u8 tileY;
    u8 tileZ;
    u16 half4;
    u8 filler6[2];
    u8 *data;
};

NONMATCH("asm/code_08032528/sub_08033BAC.s", void sub_08033BAC(struct PairObj *obj, struct PairSub *sub))
{
    u32 hit;
    struct HitEntry *entry;
    s32 bit4;
    u32 roll;
    u32 amount;
    u32 flag;
    u32 itemId;
    s32 x1;
    s32 y1;
    s32 z1;
    u32 wide;
    s32 idx;
    struct PairSub *ent;

    update_pair_facing(obj, 1, 1);
    sub->flags35B &= ~0x10;
    sub->flags210 |= 2 | 4;
    sub->state4 = 0x3E;
    sub->flags351 |= 0x40;
    if (obj->flags208 & 0x10) {
        if (sub->mode352 & 0x80) {
            obj_shift_state_slots(obj->subB);
            bit4 = ((s32)(obj->flags208 << 27)) >> 31;
            advance_anim_frame(obj, obj->subB, bit4, 0);
        }
        sub->partner->flags351 |= 0x40;
    }
    sub->mode352 &= ~0x40;
    ((struct StateLo3 *)&sub->stateBits214)->lo3 = 4;
    obj_set_id(sub, 0x1044);
    sub->sprite->flags12 = (sub->sprite->flags12 & ~6) | 2;
    sub->sprite->value20 = (*(u8 *)((u8 *)obj + 0x20A) & 2) ? 0 : (obj->flagFB >> 3);
    obj->select20B |= 0x80;
    set_slot_value(0x38, -1);
    if (*(u8 *)((u8 *)obj + 0x20A) & 2)
        return;
    hit = obj_find_hit_entry(obj, sub);
    if (hit != 0xFFFFFFFF) {
        if (!(hit & 0x80)) {
            entry = (struct HitEntry *)((u8 *)obj + 0x134 + hit * 12);
            entry->flags |= 2;
            {
                u8 *data = entry->data;
                u32 item = data[6] | (data[7] << 8);
                roll = drop_roll(item, data[0], sub->group54);
            }
            amount = roll & 0xF;
            flag = (roll >> 4) & 1;
            itemId = (roll >> 5) & 0x7F;
            x1 = (entry->tileX << 12) + 0x800;
            y1 = (entry->tileY << 12) - 0x800;
            z1 = entry->tileZ << 11;
            obj_draw_desc_entries(obj, x1, y1, z1, 0xC00, amount, flag, 1);
            wide = 0;
            if (!((s16)sub->sprite->half2 > 63))
                wide = 1;
            obj_build_message(obj, 1, amount, itemId, wide);
            if (flag) {
                set_flag(gUnk_03000FC0 + 0x38C, 0, (entry->half4 + 0x1360) & 0xFFFF, 1);
                if (!(entry->flags & 1)) {
                    void *tile = tile_ptr_at(obj->map304, entry->tileX, entry->tileY);
                    u32 group3 = (*(u32 *)tile << 1) >> 30;
                    layer3_entries_by_group(obj->map304, group3);
                }
            }
        }
    }
    idx = sub->entryIndex33E;
    if (idx != -1) {
        ent = pair_entry(obj, sub);
        if ((ent->state20E & 0x38) != 0x18) {
            obj_reset_movement(ent);
            attach_sprite_setup(sub);
        } else {
            motion_obj_set_move_target(ent, 0x30, -1, obj->move100);
        }
    }
}
END_NONMATCH

