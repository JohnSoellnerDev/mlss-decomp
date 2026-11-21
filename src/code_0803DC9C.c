#include "global.h"

struct PairSprite
{
    u8 filler0[4];
    u16 half4;
    u16 half6;
    u8 filler8[0x20 - 0x8];
    u8 frame20;
};

struct PairCtl
{
    u8 filler0[0x20];
    u8 frame20;
};

struct StateLo3
{
    u8 lo3 : 3;
    u8 rest : 5;
};

struct Flag351Bits
{
    u8 lo6 : 6;
    u8 b6 : 1;
    u8 rest : 1;
};

struct SubTable338
{
    u8 filler0[0x38];
    s16 off38;
    u8 filler3A[0x3C - 0x3A];
    void (*func3C)(void *, u32);
};

struct SubTable80
{
    s16 off0;
    u8 filler2[2];
    void (*func4)(void *, u32);
};

struct PairSub
{
    u8 filler0[4];
    u16 state;
    u8 filler6[0x54 - 0x6];
    u8 group54;
    u8 filler55[0x214 - 0x55];
    u8 stateWord214;
    u8 filler215[0x218 - 0x215];
    u16 half218;
    u8 filler21A[0x239 - 0x21A];
    u8 field239;
    u8 filler23A;
    u8 field23B;
    u8 filler23C[0x294 - 0x23C];
    struct PairSprite *sprite;
    u8 filler298[0x338 - 0x298];
    struct SubTable338 *table338;
    u8 filler33C[0x351 - 0x33C];
    u8 flags351;
    u8 filler352[0x355 - 0x352];
    u8 field355;
    u8 filler356[0x358 - 0x356];
    u8 ringFlags358;
    u8 status359;
    u8 filler35A[0x36C - 0x35A];
    struct PairCtl *ctl36C;
    u8 filler370[0x388 - 0x370];
    struct PairSub *part388;
};

struct PairObj
{
    u8 filler0[0x28];
    void *entries[32];
    void *oamBlocks[8];
    u8 filler0C8[0xFA - 0xC8];
    u8 maskFA;
    u8 fieldFB;
    u8 filler0FC[0x100 - 0xFC];
    s16 speed100;
    u8 filler102[0x200 - 0x102];
    struct PairSub *subA;
    struct PairSub *subB;
    u32 word208;
};

void update_pair_facing(void *obj, u32 a, u32 b);
void obj_set_id(void *sub, u32 id);
void obj_init_from_table(void *table, u32 a, void *sub, u32 b);
void set_slot_value(u32 id, s32 value);
/* Starts a jump: height in pixels (-1: the object's default), gravity (8.8 px per frame per frame,
 * -1: default) and speed in the air (-1: default). */
void motion_obj_set_move_target(void *obj, s32 height, s32 gravity, s32 airSpeed);
void obj_setup_aux_1045(void *sub);
void obj_release_oam(void *block);
extern const u16 gUnitStatusIdTable[]; /* 5 ids; a unit's position in it goes in status359 bits 5-7 */

/* PairSub.ringFlags358: the unit has a second part (part388) that takes the same state. */
#define UNIT_HAS_LINKED_PART 0x08
/* PairSub.status359: bits 0-4 are kept, bits 5-7 hold the gUnitStatusIdTable index. */
#define UNIT_STATUS_LOW_MASK 0x1F
#define UNIT_STATUS_INDEX_SHIFT 5

/*
 * Puts the unit into state 0x6B: stores where its id (half218) sits in gUnitStatusIdTable in
 * status359 bits 5-7 (unchanged if it isn't there), updates the pair's facing, clears the low
 * state bits, sets state 0x6B with flags351 bit 6 on the unit and its linked part, clears
 * field355 and plays sound 0x4A.
 */
void set_unit_table_flags(struct PairObj *obj, struct PairSub *unit)
{
    s32 i;

    for (i = 0; i < 5; i++)
    {
        if (gUnitStatusIdTable[i] == unit->half218)
        {
            unit->status359 = (i << UNIT_STATUS_INDEX_SHIFT) | (unit->status359 & UNIT_STATUS_LOW_MASK);
            break;
        }
    }
    update_pair_facing(obj, 1, 1);

    ((struct StateLo3 *)&unit->stateWord214)->lo3 = 0;
    unit->state = 0x6B;
    ((struct Flag351Bits *)&unit->flags351)->b6 = 1;
    unit->field23B = 0;
    if (unit->ringFlags358 & UNIT_HAS_LINKED_PART)
    {
        unit->part388->state = 0x6B;
        ((struct Flag351Bits *)&unit->part388->flags351)->b6 = 1;
        unit->part388->field23B = 0;
    }
    unit->field355 = 0;
    set_slot_value(0x4A, -1); /* plays sound 0x4A */
}

/*
 * Makes the unit jump: updates the pair's facing, puts the unit (and its linked part) into state
 * 0x48 with flags351 bit 6, sets its id to 0x1002, starts a 24-pixel jump (gravity 0x75) at the
 * pair's speed, calls the unit's method at table338 + 0x38 with 1, copies the pair's frame
 * (fieldFB / 8) to the unit's control block and plays sound 0x46.
 */
void setup_unit_move(struct PairObj *obj, struct PairSub *unit)
{
    struct SubTable338 *table;
    void *self;
    void (*method)(void *, u32);

    update_pair_facing(obj, 1, 1);
    unit->state = 0x48;
    ((struct Flag351Bits *)&unit->flags351)->b6 = 1;
    unit->field23B = 0;
    if (unit->ringFlags358 & UNIT_HAS_LINKED_PART)
    {
        unit->part388->state = 0x48;
        ((struct Flag351Bits *)&unit->part388->flags351)->b6 = 1;
        unit->part388->field23B = 0;
    }
    obj_set_id(unit, 0x1002);
    motion_obj_set_move_target(unit, 0x18, 0x75, obj->speed100);
    table = unit->table338;
    self = (u8 *)unit + table->off38;
    method = table->func3C;
    method(self, 1);
    unit->field23B = 0;
    obj_setup_aux_1045(unit);
    unit->ctl36C->frame20 = obj->fieldFB >> 3;
    set_slot_value(0x46, -1); /* plays sound 0x46 */
}

NONMATCH("asm/code_0803DC9C/setup_pair_move_58.s", void setup_pair_move_58(struct PairObj *obj))
{
    struct PairSub *a;
    struct PairSub *b;
    u8 zero;

    update_pair_facing(obj, 1, 1);
    b = obj->subB;
    a = obj->subA;
    zero = 0;
    a->state = 0x58;
    b->state = 0x58;
    obj->subA->flags351 |= 0x40;
    obj->subB->flags351 |= 0x40;
    obj->subA->field23B = zero;
    obj->subB->field23B = zero;
    obj_set_id(obj->subA, 0x1002);
    motion_obj_set_move_target(obj->subA, 0x18, 0x75, obj->speed100);
    {
        struct SubTable338 *table = obj->subA->table338;
        u8 *arg0 = (u8 *)obj->subA + table->off38;
        void (*func)(void *, u32) = table->func3C;
        func(arg0, 1);
    }
    obj->word208 &= 0xFFFE7FFF;
    obj_setup_aux_1045(obj->subA);
    obj->subA->ctl36C->frame20 = obj->fieldFB >> 3;
    set_slot_value(0x46, -1);
}
END_NONMATCH

/*
 * Puts the unit into state 0x3A: updates the pair's facing, sets flags351 bit 6, clears field23B,
 * sets the low state bits to 4 and the unit's id to 0x1030, copies the pair's frame (fieldFB / 8)
 * to the unit's sprite and field239, sets up the pair's first OAM block (4) for unit A, sets
 * field355 to 0x46 and plays sound 0x56.
 */
void init_sub_unit_table(struct PairObj *obj, struct PairSub *unit)
{
    struct PairSprite *sprite;
    u8 frame;

    update_pair_facing(obj, 1, 1);
    unit->state = 0x3A;
    ((struct Flag351Bits *)&unit->flags351)->b6 = 1;
    unit->field23B = 0;
    ((struct StateLo3 *)&unit->stateWord214)->lo3 = 4;
    obj_set_id(unit, 0x1030);
    sprite = unit->sprite;
    frame = obj->fieldFB >> 3;
    sprite->frame20 = frame;
    unit->field239 = frame;
    obj_init_from_table(obj->oamBlocks[0], 4, obj->subA, 0);
    unit->field355 = 0x46;
    set_slot_value(0x56, -1); /* plays sound 0x56 */
}

/*
 * Puts the unit (and its linked part) into state 0xF and releases the pair's 8 OAM blocks, then
 * sets the unit's id to 0x1001 (group54 == 0) or 0x1002, resets its sprite scale to 1.0 (0x100),
 * clears field23B, calls the unit's method at table338 + 0x80 with 1 and plays sound 0x49.
 */
void release_unit(struct PairObj *obj, struct PairSub *unit)
{
    u8 *group;
    s32 id;
    struct PairSprite *sprite;
    void **blocks;

    unit->state = 0xF;
    if (unit->ringFlags358 & UNIT_HAS_LINKED_PART)
        unit->part388->state = 0xF;
    blocks = obj->oamBlocks;
    group = &unit->group54;
    obj_release_oam(blocks[0]);
    obj_release_oam(obj->oamBlocks[1]);
    obj_release_oam(obj->oamBlocks[2]);
    obj_release_oam(obj->oamBlocks[3]);
    obj_release_oam(obj->oamBlocks[4]);
    obj_release_oam(obj->oamBlocks[5]);
    obj_release_oam(obj->oamBlocks[6]);
    obj_release_oam(obj->oamBlocks[7]);
    {
        register s32 noValue asm("r5") = -1;

        do
        {
            if (*group == 0)
                id = 0x1001;
            else
                id = 0x1002;
            obj_set_id(unit, id);
        } while (0);
        sprite = unit->sprite;
        sprite->half6 = 0x100; /* scale 1.0 */
        sprite->half4 = 0x100;
        unit->field23B = 0;
        {
            struct SubTable80 *method = (struct SubTable80 *)((u8 *)unit->table338 + 0x80);
            u8 *self = (u8 *)unit + method->off0;
            method->func4(self, 1);
        }
        set_slot_value(0x49, noValue); /* plays sound 0x49 */
    }
}

ASM_FUNC("asm/code_0803DC9C/update_obj_gfx_state.s", void update_obj_gfx_state(void))

u32 obj_flag_291_bit0(u8 *obj)
{
    return obj[0x291] & 1;
}

ASM_FUNC("asm/code_0803DC9C/obj_update_entry.s", void obj_update_entry(void))
