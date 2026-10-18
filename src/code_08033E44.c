#include "global.h"

struct PairSprite
{
    u8 filler0[0x12];
    u8 flags12;
    u8 filler13[0x20 - 0x13];
    u8 unk20;
};

struct PairFrames
{
    u8 byte0;
};

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
    u8 filler6[0xC - 6];
    s32 posX;
    s32 posY;
    s32 posZ;
    u8 filler18[0x20D - 0x18];
    u8 flags20D;
    u8 filler20E[0x20F - 0x20E];
    u8 flags20F;
    u8 flags210;
    u8 filler211;
    u8 flags212;
    u8 filler213;
    u8 state214;
    u8 filler215[0x23B - 0x215];
    u8 unk23B;
    u8 filler23C[0x294 - 0x23C];
    struct PairSprite *sprite294;
    u8 filler298[0x33E - 0x298];
    s8 index33E;
    u8 filler33F[0x351 - 0x33F];
    u8 flags351;
    u8 mode352;
    u8 filler353[0x355 - 0x353];
    u8 flags355;
    u8 filler356[0x358 - 0x356];
    u8 flags358;
    u8 filler359[0x35B - 0x359];
    u8 flags35B;
    u8 filler35C[0x368 - 0x35C];
    struct PairFrames *frames368;
    struct PairSprite *attached36C;
    u8 filler370[0x388 - 0x370];
    struct PairPart *partner388;
};

struct PairObj
{
    u8 filler0[0x28];
    struct PairEntry *entries[12];
    u8 filler58[0xFB - 0x58];
    u8 flagsFB;
    u8 filler0FC[0x100 - 0xFC];
    s16 move100;
    u8 filler102[0x200 - 0x102];
    struct PairPart *subA;
    struct PairPart *subB;
    u32 flags208;
    u8 filler20C[0x246 - 0x20C];
    u8 state246;
    u8 filler247[0x248 - 0x247];
    u16 pixX;
    u16 pixY;
    s16 relX;
    s16 relY;
    u8 filler250[0x26A - 0x250];
    u16 flags26A;
};

void motion_obj_set_move_target(void *obj, s32 target, s32 dir, s32 speed);
void contacts_obj_start_move(void *obj, s32 a, s32 b, s32 x, s32 y, s32 zero, s32 mode);
void set_slot_value(u32 id, s32 value);
void update_pair_facing(void *obj, u32 reset, u32 enable);
void obj_set_id(void *sub, u32 id);
void obj_setup_aux_1045(void *sub);
void obj_shift_state_slots(void *sub);
void advance_anim_frame(void *obj, void *sub, u32 a, u32 b);
void obj_set_active(void *entry, u32 zero);
void obj_set_sprite_link(void *entry, u32 zero);
void pair_turn_and_sample(void *obj, u32 zero);

ASM_FUNC("asm/code_08033E44/sub_08033E44.s", void sub_08033E44(void))

ASM_FUNC("asm/code_08033E44/pair_state_step.s", void pair_state_step(void))

ASM_FUNC("asm/code_08033E44/sub_08034710.s", void sub_08034710(void))

ASM_FUNC("asm/code_08033E44/sub_08034884.s", void sub_08034884(void))

ASM_FUNC("asm/code_08033E44/set_sub_pair_state_49.s", void set_sub_pair_state_49(void))

ASM_FUNC("asm/code_08033E44/sub_08034A70.s", void sub_08034A70(void))


ASM_FUNC("asm/code_08033E44/pair_state_a_49_4c.s", void pair_state_a_49_4c(void))

void sub_08034FFC(struct PairObj *obj)
{
    struct PairPart **slotA;
    struct PairPart **slotB;
    struct PairPart *b = obj->subB;
    u32 flags;
    struct PairPart *pa;
    struct PairPart *pb;
    u32 dir = 0xEE >> (((u32)obj->flagsFB << 29) >> 31);
    motion_obj_set_move_target(b, 4, dir, obj->move100);
    contacts_obj_start_move(obj->subB, 2, 1, obj->relX, obj->relY, 0,
        (obj->flagsFB & 4) ? 0x10 : 0xA);
    slotA = &obj->subA;
    (*slotA)->unk23B = 0;
    slotB = &obj->subB;
    flags = (*slotB)->flags20F;
    flags |= 4;
    (*slotB)->flags20F = flags;
    pb = *slotB;
    pa = *slotA;
    pa->state4 = 0x56;
    pb->state4 = 0x56;
    obj->flags208 &= 0xFFFE7FFF;
    ((u8 *)obj)[0x20B] |= 0x80;
    set_slot_value(0x2B, -1);
}

ASM_FUNC("asm/code_08033E44/pair_b_state_step.s", void pair_b_state_step(void))
