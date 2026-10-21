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
    u8 group54;
    u8 filler55[0x20D - 0x55];
    u8 flags20D;
    u8 filler20E[0x210 - 0x20E];
    u8 flags210;
    u8 filler211[0x214 - 0x211];
    u8 state214;
    u8 filler215[0x230 - 0x215];
    s32 field230;
    s32 field234;
    u8 filler238[0x23E - 0x238];
    u16 speed23E;
    u8 filler240[0x242 - 0x240];
    u16 field242;
    u16 field244;
    u8 filler246[0x260 - 0x246];
    u16 field260;
    u8 filler262[0x33C - 0x262];
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

ASM_FUNC("asm/code_08039A14/sub_08039A14.s", void sub_08039A14(void))

ASM_FUNC("asm/code_08039A14/sub_08039B70.s", void sub_08039B70(void))

ASM_FUNC("asm/code_08039A14/pair_b_state_90_9f_step.s", void pair_b_state_90_9f_step(void))

ASM_FUNC("asm/code_08039A14/init_sub_pair.s", void init_sub_pair(void))

ASM_FUNC("asm/code_08039A14/pair_b_state_4d_64_step.s", void pair_b_state_4d_64_step(void))

ASM_FUNC("asm/code_08039A14/reset_sub_pair.s", void reset_sub_pair(void))

ASM_FUNC("asm/code_08039A14/shift_sub_pair_frames.s", void shift_sub_pair_frames(void))

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

ASM_FUNC("asm/code_08039A14/clear_sub_pair.s", void clear_sub_pair(void))

ASM_FUNC("asm/code_08039A14/pair_state_a1_b2_step.s", void pair_state_a1_b2_step(void))
