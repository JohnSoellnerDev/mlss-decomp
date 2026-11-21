#include "global.h"

struct PairEntry
{
    u8 filler0[0xC];
    s32 posX;
    s32 posY;
};

struct PairPart
{
    u8 filler0[4];
    u16 state4;
    u8 filler6[0x24 - 6];
    u8 cell24;
    u8 filler25[0x54 - 0x25];
    u8 broIndex; /* which bro this part is (0 or 1) */
    u8 filler55[0x20D - 0x55];
    u8 flags20D;
    u8 filler20E[0x210 - 0x20E];
    u8 flags210;
    u8 filler211[0x214 - 0x211];
    u8 state214;
    u8 filler215[0x230 - 0x215];
    s32 field230;
    s32 field234;
    u8 filler238[0x239 - 0x238];
    u8 field239;
    u8 filler23A[0x23E - 0x23A];
    u16 speed23E;
    u8 filler240[0x242 - 0x240];
    u16 field242;
    u16 field244;
    u8 filler246[0x260 - 0x246];
    u16 field260;
    u8 filler262[0x294 - 0x262];
    u8 *sprite;
    u8 filler298[0x33C - 0x298];
    u8 entryIdx;
    u8 filler33D[0x351 - 0x33D];
    u8 flags351;
    u8 filler352[0x358 - 0x352];
    u8 flags358;
    u8 filler359[0x35A - 0x359];
    u8 flags35A;
    u8 filler35B[0x368 - 0x35B];
    u8 *frames;
};
struct Bit1
{
    u8 bit0 : 1;
    u8 bit1 : 1;
    u8 rest : 6;
};

struct Lo3Bits
{
    u8 lo3 : 3;
    u8 rest : 5;
};

struct Bits8
{
    u8 b0 : 1;
    u8 b1 : 1;
    u8 b2 : 1;
    u8 b3 : 1;
    u8 b4 : 1;
    u8 b5 : 1;
    u8 b6 : 1;
    u8 b7 : 1;
};

struct Bits246
{
    u8 lo2 : 2;
    u8 mid3 : 3;
    u8 hi3 : 3;
};

struct PairObj
{
    u8 filler0[0x100];
    u16 field100;
    u8 filler102[0x200 - 0x102];
    struct PairPart *subs[2];
    u8 flags208;
    u8 filler209;
    u8 flags20A;
    u8 flags20B;
    u8 filler20C[0x246 - 0x20C];
    u8 flags246;
    u8 filler247[0x261 - 0x247];
    u8 field261;
    u8 filler262[0x264 - 0x262];
    u32 field264;
    u16 field268;
    u8 flags26A;
    u8 filler26B[0x26E - 0x26B];
    u16 field26E;
};

void contacts_obj_start_move(void *obj, s32 a, s32 b, s32 x, s32 y, s32 zero, s32 mode);
void update_pair_facing(void *obj, u32 reset, u32 enable);
void obj_shift_state_slots(void *sub);
void advance_anim_frame(void *obj, void *sub, u32 a, u32 b);
void obj_setup_aux_sprite(void *sub, u32 n);
void follow_anchor(void *obj, u32 n);
void obj_refresh_anim_state(void *sub, u32 a, u32 b, s32 c);
void obj_set_id(void *sub, u32 id);
void set_slot_value(u32 id, s32 value);

static inline void set_bit1_26A(struct PairObj *obj, u32 v)
{
    ((struct Bit1 *)&obj->flags26A)->bit1 = v;
}

/* Facing, slot shift and animation step for A at the start of a pair state. */
static inline void pair_start_anim(struct PairObj *obj)
{
    update_pair_facing(obj, 1, 1);
    obj_shift_state_slots(obj->subs[0]);
    advance_anim_frame(obj, obj->subs[0], 0, 0);
}

/* Steps the state slots and the animation of A (bit 0) and/or B (bit 1). */
static inline void pair_shift_advance(struct PairObj *obj, u32 which)
{
    if ((which & 1) != 0)
    {
        obj_shift_state_slots(obj->subs[0]);
        advance_anim_frame(obj, obj->subs[0], 0, 0);
    }
    if ((which & 2) != 0)
    {
        obj_shift_state_slots(obj->subs[1]);
        advance_anim_frame(obj, obj->subs[1], (((u32)obj->flags208 << 27) >> 31), 0);
    }
}

NONMATCH("asm/code_08039A14/sub_08039A14.s", void sub_08039A14(struct PairObj *obj))
{
    s32 idB = 0x1026;
    struct PairPart **slot;
    struct PairPart **pa;
    struct PairPart **pb;
    u8 *fa;
    u8 *fb;
    s32 v;
    u16 zero;

    update_pair_facing(obj, 1, 1);
    slot = &obj->subs[0];
    obj_shift_state_slots(*slot);
    advance_anim_frame(obj, *slot, 0, 0);

    pb = &obj->subs[1];
    fb = (*pb)->frames;
    pa = &obj->subs[0];
    fa = (*pa)->frames;
    zero = 0;
    fa[0] = 0x10;
    fb[0] = 0x10;
    (*pa)->state4 = 0x90;
    (*pb)->state4 = 0x90;
    (*pa)->flags351 |= 0x40;
    (*pb)->flags351 |= 0x40;
    (*pa)->flags358 |= 0x08;
    (*pb)->flags358 |= 0x08;
    (*pb)->flags20D |= 0x20;
    (*pb)->flags35A |= 0x02;
    obj_setup_aux_sprite(*pb, 2);

    obj->field26E = zero;
    ((struct Bit1 *)&obj->flags208)->bit0 = 0;
    obj->flags20B |= 0x80;
    v = (*pa)->state214;
    v &= -8;
    v |= 4;
    (*pa)->state214 = v;
    obj_set_id(*pa, 0x104D);
    v = (*pb)->state214;
    v &= -8;
    v |= 4;
    (*pb)->state214 = v;
    obj_set_id(*pb, idB);

    (*pa)->sprite[0x20] = 8;
    (*pa)->field239 = 8;
    set_slot_value(0x157, -1);
}
END_NONMATCH

NONMATCH("asm/code_08039A14/sub_08039B70.s", void sub_08039B70(struct PairObj *obj))
{
    struct PairPart **slot;
    struct PairPart **pa;
    struct PairPart **pb;
    u8 *fa;
    u8 *fb;
    s32 v;
    u16 zero;

    update_pair_facing(obj, 1, 1);
    slot = &obj->subs[0];
    obj_shift_state_slots(*slot);
    advance_anim_frame(obj, *slot, 0, 0);

    pb = &obj->subs[1];
    fb = (*pb)->frames;
    pa = &obj->subs[0];
    fa = (*pa)->frames;
    zero = 0;
    fa[0] = 0x11;
    fb[0] = 0x11;
    (*pa)->state4 = 0x98;
    (*pb)->state4 = 0x98;
    (*pa)->flags351 |= 0x40;
    (*pb)->flags351 |= 0x40;
    (*pa)->flags358 |= 0x08;
    (*pb)->flags358 |= 0x08;
    (*pb)->flags20D |= 0x20;
    (*pb)->flags35A |= 0x02;
    obj_setup_aux_sprite(*pb, 2);

    obj->field26E = zero;
    ((struct Bit1 *)&obj->flags208)->bit0 = 0;
    obj->flags20B |= 0x80;
    v = (*pa)->state214 & -8;
    (*pa)->state214 = v | 4;
    obj_set_id(*pa, 0x1034);
    v = (*pb)->state214 & -8;
    (*pb)->state214 = v | 4;
    obj_set_id(*pb, 0x1040);
    set_slot_value(0x87, -1);
}
END_NONMATCH

ASM_FUNC("asm/code_08039A14/pair_b_state_90_9f_step.s", void pair_b_state_90_9f_step(void))

/* Starts the pair's state 0x61 (bro 0 leads) or 0x4D (bro 1 leads) for both bros: steps A's
   animation, marks both parts busy, attaches B's aux sprite, copies B's low cell bits to A, then
   gives B its animation id and plays the leading bro's sound (0x157 or 0x87). */
void init_sub_pair(struct PairObj *obj)
{
    struct PairPart **partA;
    struct PairPart **partB;

    update_pair_facing(obj, 1, 1);
    pair_shift_advance(obj, 1);

    if (obj->subs[0]->broIndex == 0)
    {
        struct PairPart *b = obj->subs[1];
        obj->subs[0]->state4 = 0x61;
        b->state4 = 0x61;
    }
    else
    {
        struct PairPart *b = obj->subs[1];
        obj->subs[0]->state4 = 0x4D;
        b->state4 = 0x4D;
    }

    partB = &obj->subs[1];
    {
        struct PairPart *b = *partB;
        partA = &obj->subs[0];
        (*partA)->flags351 |= 0x40;
        b->flags351 |= 0x40;
    }
    {
        struct PairPart *b = *partB;
        (*partA)->flags358 |= 0x08;
        b->flags358 |= 0x08;
    }
    ((struct Bits8 *)&(*partB)->flags20D)->b5 = 1;
    ((struct Bits8 *)&(*partB)->flags35A)->b1 = 1;
    obj_setup_aux_sprite(*partB, 2);
    {
        register u16 zero asm("r0") = 0;
        obj->field26E = zero;
    }
    ((struct Bits8 *)&obj->flags20B)->b7 = 1;
    ((struct Bits8 *)&obj->flags20A)->b3 = 1;

    ((struct Lo3Bits *)&(*partA)->cell24)->lo3 = ((u32)(*partB)->cell24 << 29) >> 29;
    ((struct Lo3Bits *)&(*partB)->state214)->lo3 = 4;

    obj_set_id(*partB, (*partA)->broIndex == 0 ? 0x1040 : 0x1026);
    set_slot_value((*partB)->broIndex == 0 ? 0x157 : 0x87, -1); /* plays a sound */
}

ASM_FUNC("asm/code_08039A14/pair_b_state_4d_64_step.s", void pair_b_state_4d_64_step(void))

ASM_FUNC("asm/code_08039A14/reset_sub_pair.s", void reset_sub_pair(void))

NONMATCH("asm/code_08039A14/shift_sub_pair_frames.s", void shift_sub_pair_frames(struct PairObj *obj))
{
    struct PairPart **slot;
    struct PairPart **pa;
    struct PairPart **pb;
    u8 *flags;
    u8 *fa;
    u8 *fb;
    u8 zero;

    flags = &obj->flags208;
    *flags |= 0x10;
    update_pair_facing(obj, 1, 1);
    slot = &obj->subs[0];
    obj_shift_state_slots(*slot);
    advance_anim_frame(obj, *slot, 0, 0);
    slot = &obj->subs[1];
    obj_shift_state_slots(*slot);
    advance_anim_frame(obj, *slot, ((u32)obj->flags208 << 27) >> 31, 0);
    pb = &obj->subs[1];
    fb = (*pb)->frames;
    pa = &obj->subs[0];
    fa = (*pa)->frames;
    fa[0] = 0x12;
    fb[0] = 0x12;
    zero = 0;
    (*pa)->state4 = 0xA0;
    (*pb)->state4 = 0xA0;
    (*pa)->flags358 |= 0x08;
    (*pb)->flags358 |= 0x08;

    (*pa)->frames[0x12] = (*pa)->frames[0x10];
    (*pa)->frames[0x10] = zero;
    (*pb)->frames[0x15] = (*pb)->frames[0x11];
    (*pb)->frames[0x11] = zero;

    ((struct Bit1 *)flags)->bit0 = 0;
    *(u32 *)flags = (*(u32 *)flags & 0xFFFE7FFF) | 0x10000;
    obj->flags246 |= 0x1C;
}
END_NONMATCH

void probe_sub_pair_target(struct PairObj *obj, u8 flag)
{
    struct PairPart *a;
    struct PairPart *b;
    struct PairEntry *e;
    u8 *base;
    u32 flags20B;
    s32 idx;
    s32 x;
    s32 y;

    b = obj->subs[1];
    obj->subs[0]->state4 = 0xA8;
    b->state4 = 0xA8;
    a = obj->subs[0];
    idx = ((s32)(a->entryIdx << 25)) >> 23;
    base = (u8 *)obj + 0x28;
    e = *(struct PairEntry **)(base + idx);
    x = e->posX / 0x100;
    y = e->posY / 0x100;
    contacts_obj_start_move(a, 1, 0, x, y, 0, 0x200);
    set_bit1_26A(obj, flag);
    flags20B = obj->flags20B;
    obj->flags20B = flags20B | 0x80;
}

NONMATCH("asm/code_08039A14/clear_sub_pair.s", void clear_sub_pair(struct PairObj *obj))
{
    s32 idB = 0x1001;
    struct PairPart **pa;
    struct PairPart **pb;
    u8 *fa;
    u8 *fb;
    s32 v;
    s32 zero;

    ((struct Bits8 *)&obj->flags208)->b4 = 0;
    pb = &obj->subs[1];
    fb = (*pb)->frames;
    pa = &obj->subs[0];
    fa = (*pa)->frames;
    zero = 0;
    fa[0] = zero;
    fb[0] = zero;
    (*pa)->state4 = zero;
    (*pb)->state4 = zero;
    ((struct Bits8 *)&(*pa)->flags351)->b6 = 0;
    ((struct Bits8 *)&(*pb)->flags351)->b6 = 0;
    ((struct Bits8 *)&(*pa)->flags358)->b3 = 0;
    ((struct Bits8 *)&(*pb)->flags358)->b3 = 0;
    (*pa)->flags210 |= 0x08;
    (*pb)->flags210 |= 0x08;
    ((struct Bits8 *)&(*pa)->flags20D)->b5 = 0;
    ((struct Bits8 *)&(*pb)->flags20D)->b5 = 0;
    (*pa)->speed23E = obj->field100;
    (*pa)->field260 = obj->field100;
    obj->flags208 |= 1;
    v = (*pb)->state214;
    v &= -8;
    v |= 3;
    (*pb)->state214 = v;
    v = (*pa)->state214;
    v &= -8;
    v |= 3;
    (*pa)->state214 = v;
    *(u32 *)&obj->flags208 = *(u32 *)&obj->flags208 & 0xFFFE7FFF;
    ((struct Bits246 *)&obj->flags246)->mid3 = 0;

    (*pa)->frames[0x10] = (*pa)->frames[0x12];
    (*pa)->frames[0x12] = 0xFF;
    (*pb)->frames[0x11] = (*pb)->frames[0x15];
    (*pb)->frames[0x15] = -1;
    obj_set_id(*pb, idB);
    obj_set_id(*pa, 0x1002);
    follow_anchor(obj, 1);
}
END_NONMATCH

ASM_FUNC("asm/code_08039A14/pair_state_a1_b2_step.s", void pair_state_a1_b2_step(void))
