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
void motion_obj_set_move_target(void *obj, u32 a, s32 b, s32 c);
void obj_setup_aux_1045(void *sub);
void obj_release_oam(void *block);
extern const u16 gUnitStatusIdTable[];

NONMATCH("asm/code_0803DC9C/set_unit_table_flags.s", void set_unit_table_flags(struct PairObj *obj, struct PairSub *unit))
{
    s32 mask = 0x1F;
    const u16 *table = gUnitStatusIdTable;
    u16 *half = &unit->half218;
    u8 *status = &unit->status359;
    s32 extra;

    if (table[0] == *half)
    {
        *status = mask & *status;
    }
    else
    {
        if (table[1] == *half)
            extra = 0x20;
        else if (table[2] == *half)
            extra = 0x40;
        else if (table[3] == *half)
            extra = 0x60;
        else if (table[4] == *half)
            extra = 0x80;
        else
            goto done;
        *status = (mask & *status) | extra;
    }

done:
    update_pair_facing(obj, 1, 1);
    ((struct StateLo3 *)&unit->stateWord214)->lo3 = 0;
    unit->state = 0x6B;
    ((struct Flag351Bits *)&unit->flags351)->b6 = 1;
    unit->field23B = 0;
    if (unit->ringFlags358 & 8)
    {
        unit->part388->state = 0x6B;
        ((struct Flag351Bits *)&unit->part388->flags351)->b6 = 1;
        unit->part388->field23B = 0;
    }
    unit->field355 = 0;
    set_slot_value(0x4A, -1);
}
END_NONMATCH

void setup_unit_move(struct PairObj *obj, struct PairSub *unit)
{
    struct SubTable338 *table;
    void *arg0;
    void (*func)(void *, u32);

    update_pair_facing(obj, 1, 1);
    unit->state = 0x48;
    ((struct Flag351Bits *)&unit->flags351)->b6 = 1;
    unit->field23B = 0;
    if (unit->ringFlags358 & 8)
    {
        unit->part388->state = 0x48;
        ((struct Flag351Bits *)&unit->part388->flags351)->b6 = 1;
        unit->part388->field23B = 0;
    }
    obj_set_id(unit, 0x1002);
    motion_obj_set_move_target(unit, 0x18, 0x75, obj->speed100);
    table = unit->table338;
    arg0 = (u8 *)unit + table->off38;
    func = table->func3C;
    func(arg0, 1);
    unit->field23B = 0;
    obj_setup_aux_1045(unit);
    unit->ctl36C->frame20 = obj->fieldFB >> 3;
    set_slot_value(0x46, -1);
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

void init_sub_unit_table(struct PairObj *obj, struct PairSub *unit)
{
    struct PairSprite *sprite;
    u8 v;

    update_pair_facing(obj, 1, 1);
    unit->state = 0x3A;
    ((struct Flag351Bits *)&unit->flags351)->b6 = 1;
    unit->field23B = 0;
    ((struct StateLo3 *)&unit->stateWord214)->lo3 = 4;
    obj_set_id(unit, 0x1030);
    sprite = unit->sprite;
    v = obj->fieldFB >> 3;
    sprite->frame20 = v;
    unit->field239 = v;
    obj_init_from_table(obj->oamBlocks[0], 4, obj->subA, 0);
    unit->field355 = 0x46;
    set_slot_value(0x56, -1);
}

NONMATCH("asm/code_0803DC9C/release_unit.s", void release_unit(struct PairObj *obj, struct PairSub *unit))
{
    u8 *group;
    s32 none;
    s32 id;

    unit->state = 0xF;
    if (unit->ringFlags358 & 8)
        unit->part388->state = 0xF;
    group = &unit->group54;
    obj_release_oam(obj->oamBlocks[0]);
    obj_release_oam(obj->oamBlocks[1]);
    obj_release_oam(obj->oamBlocks[2]);
    obj_release_oam(obj->oamBlocks[3]);
    obj_release_oam(obj->oamBlocks[4]);
    obj_release_oam(obj->oamBlocks[5]);
    obj_release_oam(obj->oamBlocks[6]);
    obj_release_oam(obj->oamBlocks[7]);
    none = -1;
    if (*group == 0)
        id = 0x1001;
    else
        id = 0x1002;
    obj_set_id(unit, id);
    unit->sprite->half6 = 0x100;
    unit->sprite->half4 = 0x100;
    unit->field23B = 0;
    {
        struct SubTable80 *t = (struct SubTable80 *)((u8 *)unit->table338 + 0x80);
        u8 *arg0 = (u8 *)unit + t->off0;
        t->func4(arg0, 1);
    }
    set_slot_value(0x49, none);
}
END_NONMATCH

ASM_FUNC("asm/code_0803DC9C/update_obj_gfx_state.s", void update_obj_gfx_state(void))

u32 obj_flag_291_bit0(u8 *obj)
{
    return obj[0x291] & 1;
}

ASM_FUNC("asm/code_0803DC9C/obj_update_entry.s", void obj_update_entry(void))
