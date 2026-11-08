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
    u16 state4;
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

struct PairObj
{
    u8 filler0[0x28];
    struct PairSub *entries[32];
    u8 filler0A8[0xFB - 0xA8];
    u8 flagFB;
    u8 filler0FC[0x100 - 0xFC];
    s16 move100;
    u8 filler102[0x134 - 0x102];
    struct HitEntry hits[16];
    u8 filler1F4[0x200 - 0x1F4];
    struct PairSub *subA;
    struct PairSub *subB;
    u8 flags208;
    u8 filler209;
    u8 flags20A;
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

struct Bit0Bits
{
    u8 bit0 : 1;
    u8 rest : 7;
};

struct Bit1Bits
{
    u8 bit0 : 1;
    u8 bit1 : 1;
    u8 rest : 6;
};

struct Bit2Bits
{
    u8 lo2 : 2;
    u8 bit2 : 1;
    u8 rest : 5;
};

struct Bit3Bits
{
    u8 lo3 : 3;
    u8 bit3 : 1;
    u8 rest : 4;
};

struct Bit4Bits
{
    u8 lo4 : 4;
    u8 bit4 : 1;
    u8 rest : 3;
};

struct Bit5Bits
{
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 rest : 2;
};

struct Bit6Bits
{
    u8 lo6 : 6;
    u8 bit6 : 1;
    u8 hi1 : 1;
};

struct Bit7Bits
{
    u8 lo7 : 7;
    u8 bit7 : 1;
};

struct Hi3Bits
{
    u8 lo5 : 5;
    u8 hi3 : 3;
};

/* bits 0-2 of the sub object byte +0x24 */
struct Cell24Bits
{
    u8 filler0[0x24];
    u8 lo3 : 3;
    u8 rest : 5;
};

/* bits 1-2 */
struct ModeBits
{
    u8 bit0 : 1;
    u8 mode : 2;
    u8 rest : 5;
};

/* bits 2-4 */
struct Mid3Bits
{
    u8 lo2 : 2;
    u8 mid3 : 3;
    u8 rest : 3;
};

/* bits 0-1 and 2-3 of the sprite byte +0x1F */
struct SprLayerBits
{
    u8 lo2 : 2;
    u8 b23 : 2;
    u8 rest : 4;
};

/* the same fields, read as a byte */
struct SprLayerP
{
    u8 lo2 : 2;
    u8 b23 : 2;
    u8 rest : 4;
} __attribute__((packed));

struct Half20A
{
    u16 lo7 : 7;
    u16 b78 : 2;
    u16 rest : 7;
};

typedef union __attribute__((packed)) { u8 value; } ByteArg __attribute__((transparent_union));

void update_pair_facing(void *obj, u32 a, u32 b);
void obj_shift_state_slots(void *sub);
void advance_anim_frame(void *obj, void *sub, u32 a, u32 b);
void motion_obj_set_move_target(void *obj, u32 a, s32 b, s32 c);
void contacts_obj_start_move(void *obj, s32 a, s32 b, s32 x, s32 y, s32 zero, s32 mode);
void obj_set_id(void *sub, u32 id);
void sound_stop_song(u32 song);
void set_slot_value(u32 id, s32 value);
u32 obj_find_hit_entry(void *obj, void *sub);
u16 drop_roll(u32 item, u32 kind, u32 group);
void obj_draw_desc_entries(void *obj, s32 x, s32 y, s32 z, s32 w, u32 amount, ByteArg flag, ByteArg one);
void obj_build_message(void *obj, u32 a, u32 amount, u32 item, u32 wide);
void set_flag(u8 *flags, u32 a, u32 id, u32 value);
u32 *tile_ptr_at(void *map, u32 x, u32 y);
void layer3_entries_by_group(void *map, u32 group);
void obj_reset_movement(void *ent);
void attach_sprite_setup(void *sub);
extern u8 *gUnk_03000FC0;

/* Steps the state slots and the animation of A (bit 0) and/or B (bit 1). */
static inline void pair_shift_advance(struct PairObj *obj, u32 which)
{
    if ((which & 1) != 0)
    {
        obj_shift_state_slots(obj->subA);
        advance_anim_frame(obj, obj->subA, 0, 0);
    }
    if ((which & 2) != 0)
    {
        obj_shift_state_slots(obj->subB);
        advance_anim_frame(obj, obj->subB, (((u32)obj->flags208 << 27) >> 31), 0);
    }
}

/* The entry that the slot byte of a sub object selects. */
static inline struct PairSub *pair_slot_entry(struct PairObj *obj, struct PairSub *sub)
{
    return obj->entries[((s32)(sub->slot33C << 25)) >> 25];
}

/* A and B start state 0x1D: B moves to A, its jump height follows the height difference. */
void pair_start_state_1d(struct PairObj *obj)
{
    s32 maxDist;
    s32 dist;

    update_pair_facing(obj, 1, 1);
    pair_shift_advance(obj, 1);
    ((struct Bit1Bits *)&obj->state246)->bit1 = 0;
    obj->pixX = obj->subB->posX / 0x100;
    obj->pixY = obj->subB->posY / 0x100;
    {
        u8 *bf = obj->subB->frames;
        *obj->subA->frames = 4;
        *bf = 4;
    }
    {
        struct PairSub *b = obj->subB;
        obj->subA->state4 = 0x1D;
        b->state4 = 0x1D;
    }
    {
        struct PairSub *b = obj->subB;
        ((struct Bit6Bits *)&obj->subA->flags351)->bit6 = 1;
        ((struct Bit6Bits *)&b->flags351)->bit6 = 1;
    }
    {
        struct PairSub *b = obj->subB;
        ((struct Bit3Bits *)&obj->subA->flags358)->bit3 = 1;
        ((struct Bit3Bits *)&b->flags358)->bit3 = 1;
    }
    obj->field258 = 0;
    ((struct Hi3Bits *)&obj->state246)->hi3 = ((struct Cell24Bits *)obj->subA)->lo3;
    maxDist = 0x24;
    if (obj->subA->posZ + obj->subA->posZB > obj->subB->posZ + obj->subB->posZB)
    {
        u8 layerA = obj->subA->type26;
        u8 layerB = obj->subB->type26;
        if ((((u32)(layerA & layerB)) << 26) >> 30)
            maxDist = obj->subA->posZ - obj->subB->posZ + 0x24;
    }
    dist = ((obj->subA->posZ + obj->subA->posZB) - (obj->subB->posZ + obj->subB->posZB)) / 0x100 + 0x24;
    if (dist > maxDist)
        dist = maxDist;
    motion_obj_set_move_target(obj->subB, dist, 0xEE >> (((u32)obj->flagFB << 29) >> 31), obj->move100);
    contacts_obj_start_move(obj->subB, 2, 0, obj->subA->posX / 0x100, obj->subA->posY / 0x100, 0,
        (obj->flagFB & 4) ? 0xE : 8);
    ((struct Bit7Bits *)&obj->select20B)->bit7 = 1;
    set_slot_value(0x2B, -1);
}

/* A in state 0x23 or 0x29 near the height of its slot entry: A and B start state 0x27. */
void pair_start_state_27(struct PairObj *obj)
{
    struct PairSub *e;
    s32 aPix;
    s32 ePix;
    s32 top;

    if ((s16)obj->subA->state4 != 0x23 && (s16)obj->subA->state4 != 0x29)
        return;
    aPix = (obj->subA->posZ + obj->subA->posZB) / 0x100;
    e = pair_slot_entry(obj, obj->subA);
    ePix = e->posZ + e->posZB;
    if (ePix < 0)
        ePix += 0xFF;
    ePix >>= 8;
    top = aPix + 8;
    if (top < ePix)
        return;
    if (top - 0x10 > ePix)
        return;
    {
        struct PairSub *b = obj->subB;
        obj->subA->state4 = 0x27;
        b->state4 = 0x27;
    }
    {
        struct PairSub *b = obj->subB;
        ((struct Bit6Bits *)&obj->subA->flags351)->bit6 = 1;
        ((struct Bit6Bits *)&b->flags351)->bit6 = 1;
    }
    e = pair_slot_entry(obj, obj->subA);
    contacts_obj_start_move(obj->subA, 1, 0, e->posX / 0x100, e->posY / 0x100,
        (obj->subA->posZ + obj->subA->posZB) / 0x100, 0);
    ((struct Half20A *)&obj->flags20A)->b78 = 1;
    ((struct Bit0Bits *)&obj->subA->flags20D)->bit0 = 1;
    ((struct Bit4Bits *)&obj->subA->flags20D)->bit4 = 0;
    {
        struct PairSpr *spr = obj->subA->sprite;
        ((struct SprLayerBits *)&spr->flags1F)->lo2 = ((struct SprLayerBits *)&spr->flags1F)->b23
            = ((struct SprLayerP *)&pair_slot_entry(obj, obj->subA)->sprite->flags1F)->b23;
    }
    ((struct Bit4Bits *)&obj->subB->flags20D)->bit4 = 0;
    {
        struct PairSpr *spr = obj->subB->sprite;
        ((struct SprLayerBits *)&spr->flags1F)->lo2 = ((struct SprLayerBits *)&spr->flags1F)->b23
            = ((struct SprLayerP *)&pair_slot_entry(obj, obj->subA)->sprite->flags1F)->lo2;
    }
    sound_stop_song(0x5D);
    set_slot_value(0x8E, -1);
}

/* A in state 0x21-0x24 or 0x2C: A and B start state 0x2A, A moves onto its slot entry. */
void pair_start_state_2a(struct PairObj *obj)
{
    struct PairSub *e;
    s32 slot;
    u16 st;

    st = obj->subA->state4;
    if ((u16)(st - 0x21) > 3 && st != 0x2C)
        return;
    slot = ((s32)(obj->subA->slot33C << 25)) >> 25;
    e = obj->entries[slot];
    obj->field25C = slot;
    obj->field254 = e->posZ + e->posZB + 0x6000;
    {
        struct PairSub *b = obj->subB;
        obj->subA->state4 = 0x2A;
        b->state4 = 0x2A;
    }
    {
        struct PairSub *b = obj->subB;
        ((struct Bit6Bits *)&obj->subA->flags351)->bit6 = 1;
        ((struct Bit6Bits *)&b->flags351)->bit6 = 1;
    }
    contacts_obj_start_move(obj->subA, 1, 0, e->posX / 0x100, e->posY / 0x100,
        (obj->subA->posZ + obj->subA->posZB) / 0x100, 0);
    ((struct Half20A *)&obj->flags20A)->b78 = 1;
    obj->field252 = 2;
    ((struct Bit0Bits *)&obj->subA->flags20D)->bit0 = 1;
    ((struct Bit4Bits *)&obj->subA->flags20D)->bit4 = 0;
    {
        struct PairSpr *spr = obj->subA->sprite;
        ((struct SprLayerBits *)&spr->flags1F)->lo2 = ((struct SprLayerBits *)&spr->flags1F)->b23
            = ((struct SprLayerP *)&e->sprite->flags1F)->b23;
    }
    ((struct Bit4Bits *)&obj->subB->flags20D)->bit4 = 0;
    {
        struct PairSpr *spr = obj->subB->sprite;
        ((struct SprLayerBits *)&spr->flags1F)->lo2 = ((struct SprLayerBits *)&spr->flags1F)->b23
            = ((struct SprLayerP *)&e->sprite->flags1F)->b23;
    }
    ((struct SprLayerBits *)&e->sprite->flags1F)->lo2 = ((struct SprLayerP *)&e->sprite->flags1F)->b23 - 1;
    ((struct Bit4Bits *)&e->flags20D)->bit4 = 0;
    ((struct Bit6Bits *)&obj->subA->flags20D)->bit6 = 0;
    {
        struct PairSub *b = obj->subB;
        ((struct StateLo3 *)&obj->subA->stateBits214)->lo3 = 2;
        ((struct StateLo3 *)&b->stateBits214)->lo3 = 2;
    }
}

ASM_FUNC("asm/code_08032528/pair_frame4_step.s", void pair_frame4_step(void))

/* A and B start state 0x3B. */
void pair_start_state_3b(struct PairObj *obj)
{
    update_pair_facing(obj, 1, 1);
    pair_shift_advance(obj, 1);
    {
        u8 *bf = obj->subB->frames;
        *obj->subA->frames = 6;
        *bf = 6;
    }
    {
        struct PairSub *b = obj->subB;
        obj->subA->state4 = 0x3B;
        b->state4 = 0x3B;
    }
    {
        struct PairSub *b = obj->subB;
        ((struct Bit6Bits *)&obj->subA->flags351)->bit6 = 1;
        ((struct Bit6Bits *)&b->flags351)->bit6 = 1;
    }
    {
        struct PairSub *b = obj->subB;
        ((struct Bit3Bits *)&obj->subA->flags358)->bit3 = 1;
        ((struct Bit3Bits *)&b->flags358)->bit3 = 1;
    }
    ((struct Bit4Bits *)&obj->subA->flags35B)->bit4 = 1;
    ((struct Bit5Bits *)&obj->subA->flags20D)->bit5 = 1;
    ((struct Bit1Bits *)&obj->subA->flags210)->bit1 = 0;
    ((struct Bit2Bits *)&obj->subA->flags210)->bit2 = 0;
    ((struct Bit6Bits *)&obj->subB->seqFlags212)->bit6 = 1;
    ((struct Bit5Bits *)&obj->subB->flags20D)->bit5 = 1;
    ((struct Bit1Bits *)&obj->subB->flags35A)->bit1 = 1;
    ((struct Bit0Bits *)&obj->flags208)->bit0 = 0;
    ((struct Bit7Bits *)&obj->select20B)->bit7 = 1;
    ((struct StateLo3 *)&obj->subB->stateBits214)->lo3 = 4;
    obj_set_id(obj->subB, 0x1025);
    motion_obj_set_move_target(obj->subB, 0x10, 0xEE >> (((u32)obj->flagFB << 29) >> 31), obj->move100);
    ((struct ModeBits *)&obj->subB->sprite->flags12)->mode = 1;
    obj->subA->frames[0x12] = obj->subA->frames[0x10];
    obj->subA->frames[0x10] = 0;
    ((struct Mid3Bits *)&obj->state246)->mid3 = 3;
    set_slot_value(0x3C, -1);
    obj->field272 = (obj->flagFB & 4) == 0 ? 0xA : 0x14;
}

/* The sub object hits: state 0x3E; a hit entry is marked, its drop rolled and shown. */
void sub_08033BAC(struct PairObj *obj, struct PairSub *sub)
{
    u32 hit;
    struct HitEntry *entry;
    u32 roll;
    u16 amount;
    u8 flag;
    u32 itemId;
    u32 hi;

    update_pair_facing(obj, 1, 1);
    ((struct Bit4Bits *)&sub->flags35B)->bit4 = 0;
    ((struct Bit1Bits *)&sub->flags210)->bit1 = 1;
    ((struct Bit2Bits *)&sub->flags210)->bit2 = 1;
    sub->state4 = 0x3E;
    ((struct Bit6Bits *)&sub->flags351)->bit6 = 1;
    if (obj->flags208 & 0x10)
    {
        if (sub->mode352 & 0x80)
            pair_shift_advance(obj, 2);
        ((struct Bit6Bits *)&sub->partner->flags351)->bit6 = 1;
    }
    ((struct Bit6Bits *)&sub->mode352)->bit6 = 0;
    ((struct StateLo3 *)&sub->stateBits214)->lo3 = 4;
    obj_set_id(sub, 0x1044);
    ((struct ModeBits *)&sub->sprite->flags12)->mode = 1;
    sub->sprite->value20 = (obj->flagFB >> 3) << (((u32)obj->flags20A << 30) >> 31);
    ((struct Bit7Bits *)&obj->select20B)->bit7 = 1;
    set_slot_value(0x38, -1);
    if (obj->flags20A & 2)
        return;
    hit = obj_find_hit_entry(obj, sub);
    if (hit != -1 && !(hit & 0x80))
    {
        entry = &obj->hits[hit];
        ((struct Bit1Bits *)&entry->flags)->bit1 = 1;
        hi = entry->data[7] << 8;
        roll = drop_roll(entry->data[6] | hi, entry->data[0], sub->group54);
        amount = roll & 0xF;
        flag = (roll >> 4) & 1;
        itemId = (roll >> 5) & 0x7F;
        obj_draw_desc_entries(obj, (entry->tileX << 12) + 0x800, (entry->tileY << 12) - 0x800,
            entry->tileZ << 11, 0xC00, amount, flag, (u8)1);
        obj_build_message(obj, 1, amount, itemId, sub->sprite->half2 <= 0x3F);
        if (flag)
        {
            set_flag(gUnk_03000FC0 + 0x38C, 0, (u16)(entry->half4 + 0x1360), 1);
            if (!(entry->flags & 1))
            {
                u32 *tile = tile_ptr_at(obj->map304, entry->tileX, entry->tileY);
                layer3_entries_by_group(obj->map304, (*tile << 1) >> 30);
            }
        }
    }
    if (sub->entryIndex33E != -1)
    {
        struct PairSub *ent = obj->entries[sub->entryIndex33E];
        if ((ent->state20E & 0x38) == 0x18)
        {
            obj_reset_movement(ent);
            attach_sprite_setup(sub);
        }
        else
            motion_obj_set_move_target(ent, 0x30, -1, obj->move100);
    }
}
