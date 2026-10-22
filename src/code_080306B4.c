#include "global.h"

struct PairSpr
{
    u8 filler0[0x12];
    u8 flags12;
    u8 filler13[0x20 - 0x13];
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
    u8 filler18[0x20D - 0x18];
    u8 flags20D;
    u8 filler20E[0x212 - 0x20E];
    u8 flags212;
    u8 filler213;
    u8 stateBits214;
    u8 filler215[0x239 - 0x215];
    u8 field239;
    u8 filler23A;
    u8 field23B;
    u8 filler23C[0x294 - 0x23C];
    struct PairSpr *sprite;
    u8 filler298[0x351 - 0x298];
    u8 flags351;
    u8 filler352[0x355 - 0x352];
    u8 field355;
    u8 filler356[0x358 - 0x356];
    u8 flags358;
    u8 filler359;
    u8 flags35A;
};

struct PairObj
{
    u8 filler0[0xA8];
    void *slots[8];
    u8 filler0C8[0xFB - 0xC8];
    u8 flagsFB;
    u8 filler0FC[0x100 - 0xFC];
    s16 move100;
    u8 filler102[0x200 - 0x102];
    struct PairSub *subA;
    struct PairSub *subB;
    u8 filler208[0x20B - 0x208];
    u8 select20B;
    u8 filler20C[0x246 - 0x20C];
    u8 state246;
    u8 filler247[0x248 - 0x247];
    s16 pixX;
    s16 pixY;
    u8 filler24C[0x26E - 0x24C];
    u16 field26E;
};

struct StateLo3
{
    u8 lo3 : 3;
    u8 rest : 5;
};

struct Bits352
{
    u8 lo6 : 6;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

struct Bit7
{
    u8 lo7 : 7;
    u8 bit7 : 1;
};

struct Bit5
{
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 rest : 2;
};

struct Bit1Clear
{
    u8 b0 : 1;
    u8 b1 : 1;
    u8 rest : 6;
};

struct ModeBits12
{
    u8 b0 : 1;
    u8 mode : 2;
    u8 rest : 5;
};

void update_pair_facing(void *obj, u32 a, u32 b);
void obj_set_id(void *sub, u32 id);
void obj_init_from_table(void *table, u32 a, void *sub, u32 b);
void set_slot_value(u32 id, s32 value);
void obj_shift_state_slots(void *sub);
void advance_anim_frame(void *obj, void *sub, u32 a, u32 b);
void contacts_obj_start_move(void *obj, s32 a, s32 b, s32 x, s32 y, s32 zero, s32 mode);
void motion_obj_set_move_target(void *obj, u32 a, s32 b, s32 c);

ASM_FUNC("asm/code_080306B4/pair_b_state_10_1c.s", void pair_b_state_10_1c(void))

ASM_FUNC("asm/code_080306B4/pair_start_state_2e.s", void pair_start_state_2e(void))

ASM_FUNC("asm/code_080306B4/pair_start_state_37.s", void pair_start_state_37(void))

ASM_FUNC("asm/code_080306B4/pair_start_state_3a.s", void pair_start_state_3a(void))

void pair_sub_state_3a(struct PairObj *obj, struct PairSub *sub)
{
    struct PairSpr *sprite;
    u8 value;

    update_pair_facing(obj, 1, 1);
    sub->state4 = 0x3A;
    ((struct Bits352 *)&sub->flags351)->bit6 = 1;
    sub->field23B = 0;
    ((struct StateLo3 *)&sub->stateBits214)->lo3 = 4;
    obj_set_id(sub, 0x1030);
    sprite = sub->sprite;
    value = obj->flagsFB >> 3;
    sprite->value20 = value;
    sub->field239 = value;
    obj_init_from_table(obj->slots[0], 4, obj->subA, 0);
    sub->field355 = 0x46;
    ((struct Bit7 *)&obj->select20B)->bit7 = 1;
    set_slot_value(0x56, -1);
}

NONMATCH("asm/code_080306B4/pair_slot_kind.s", u32 pair_slot_kind(struct PairObj *obj))
{
    s16 i;
    u8 f;
    s32 state;

    i = 0;
    do
    {
        f = *(u8 *)obj->slots[i];
        if ((f & 0x40) == 0)
        {
            f = *(u8 *)obj->slots[i + 1];
            if ((f & 0x40) == 0)
            {
                i += 2;
                continue;
            }
        }
        if ((f & 0x38) == 0x10)
            return 2;
        if ((f & 0x38) == 0x18)
            return 3;
        return 4;
    } while (i <= 7);
    state = 0;
    if ((obj->state246 & 0x1C) == 4)
        state = 1;
    return state;
}
END_NONMATCH

ASM_FUNC("asm/code_080306B4/pair_state_2e_3a.s", void pair_state_2e_3a(void))
