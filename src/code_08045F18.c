#include "global.h"

struct FieldSub
{
    u8 filler0[4];
    u16 h04;
    u16 h06;
    u8 filler08[0x11 - 0x08];
    u8 flags11;
    u8 filler12;
    u8 count13;
    u16 mask14;
    u8 filler16[0x1C - 0x16];
    u16 index1C;
    u8 b1E;
    u8 filler1F[0x21 - 0x1F];
    u8 anim21;
};

struct FieldObj
{
    u8 filler0[2];
    u8 unk02;
    u8 filler3[0x0C - 0x03];
    s32 posX;
    s32 posY;
    s32 posZ;
    s32 posZB;
    u8 filler1C[0x24 - 0x1C];
    u8 unk24;
    u8 filler25[0x4C - 0x25];
    u8 unk4C;
    u8 filler4D[0x20D - 0x4D];
    u8 flags20D;
    u8 filler20E[0x213 - 0x20E];
    u8 change213;
    u8 state214;
    u8 flags215;
    u8 flags216;
    u8 filler217;
    u16 id218;
    u8 filler21A[0x21C - 0x21A];
    u16 prevId21C;
    u16 h21E;
    u8 anim220;
    u8 anim221;
    u8 filler222[0x23E - 0x222];
    u16 v23E;
    u16 v240;
    u16 v242;
    u16 v244;
    u8 filler246[0x24C - 0x246];
    s32 v24C;
    s32 v250;
    u8 filler254[0x25C - 0x254];
    s32 counter25C;
    u8 filler260[0x266 - 0x260];
    u16 v266;
    u16 pending268;
    u8 filler26A[0x294 - 0x26A];
    struct FieldSub *sub;
    u8 filler298[0x2B5 - 0x298];
    u8 moveFlags2B5;
    u8 filler2B6[0x2E0 - 0x2B6];
    u16 v2E0;
    u8 filler2E2[0x2FA - 0x2E2];
    u16 axisACur;
    u16 axisBCur;
    u8 filler300[0x302 - 0x300];
    u16 axisATarget;
    u16 axisBTarget;
    u8 filler306[0x30A - 0x306];
    u16 axisAStep;
    u16 axisBStep;
    u8 filler30E[0x312 - 0x30E];
    u16 axisACount;
    u16 axisBCount;
    u8 filler316[0x31A - 0x316];
    u16 axisASpeed;
    u16 axisBSpeed;
};

static inline u32 round_to_pixel(u32 v)
{
    return (v + ((v & 0x80) << 1)) & 0xFFFFFF00;
}

extern u16 gUnk_0203FFC4[];
extern const u8 gFrameAttrTable[];
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
void obj_update_special_flag(struct FieldObj *obj);
void obj_refresh_anim_state(struct FieldObj *obj, u32 a, s32 b, s32 c);

ASM_FUNC("asm/code_08045F18/cell_motion_obj_move_toward_target.s", void cell_motion_obj_move_toward_target(void))

ASM_FUNC("asm/code_08045F18/advance_obj_motion.s", void advance_obj_motion(void))

ASM_FUNC("asm/code_08045F18/obj_advance_motion.s", void obj_advance_motion(void))

struct MoveBits
{
    u8 lo3 : 3;
    u8 bit3 : 1;
    u8 hi4 : 4;
};

void obj_finish_move_step(struct FieldObj *obj)
{
    if (obj->moveFlags2B5 & 7)
    {
        obj->posX = round_to_pixel(obj->posX);
        obj->posY = round_to_pixel(obj->posY);
        if (!(obj->flags20D & 0x40))
        {
            obj->posZ = round_to_pixel(obj->posZ);
            obj->posZB = round_to_pixel(obj->posZB);
        }
        obj->v2E0 = 0;
        obj->v242 = 0;
        obj->v244 = 0;
        ((struct MoveBits *)&obj->moveFlags2B5)->lo3 = 0;
    }
}

struct ChangeBits
{
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 hi2 : 2;
};

void obj_stop_motion(struct FieldObj *obj)
{
    u16 step;
    s32 *p24C;

    if (obj->moveFlags2B5 & 8)
    {
        obj->counter25C = -1;
        ((struct ChangeBits *)&obj->change213)->bit5 = 0;
        p24C = &obj->v24C;
        obj->v250 = -1;
        *p24C = -1;
        obj->v23E = obj->v240;
        step = obj->pending268;
        if ((s16)obj->pending268 != -1)
        {
            obj->v266 = step;
            obj->pending268 = 0xFFFF;
        }
        ((struct MoveBits *)&obj->moveFlags2B5)->bit3 = 0;
    }
}

ASM_FUNC("asm/code_08045F18/obj_set_motion_a_08046AA4.s", void obj_set_motion_a_08046AA4(void))

ASM_FUNC("asm/code_08045F18/obj_set_motion_b.s", void obj_set_motion_b(void))

ASM_FUNC("asm/code_08045F18/obj_set_motion_a_08046BDC.s", void obj_set_motion_a_08046BDC(void))

ASM_FUNC("asm/code_08045F18/obj_set_motion_a_compare.s", void obj_set_motion_a_compare(void))

ASM_FUNC("asm/code_08045F18/obj_advance_axes.s", void obj_advance_axes(void))

ASM_FUNC("asm/code_08045F18/obj_set_cell_coord.s", void obj_set_cell_coord(void))

NONMATCH("asm/code_08045F18/obj_sync_sub_slot.s", void obj_sync_sub_slot(struct FieldObj *obj, s32 slot))
{
    u32 bit;
    u32 value;
    u32 mark;
    u8 *flags;

    if (slot == 0)
    {
        obj->anim220 = obj->sub->anim21;
        flags = &obj->flags215;
        bit = ((u32)obj->sub->flags11 << 25) >> 31;
        value = (*flags & -9) | (bit << 3);
        mark = 2;
    }
    else
    {
        obj->anim221 = obj->sub->anim21;
        flags = &obj->flags215;
        bit = ((u32)obj->sub->flags11 << 25) >> 31;
        value = (*flags & -0x11) | (bit << 4);
        mark = 4;
    }
    *flags = value | mark;
}
END_NONMATCH

ASM_FUNC("asm/code_08045F18/obj_step_index.s", void obj_step_index(void))

ASM_FUNC("asm/code_08045F18/obj_set_state.s", void obj_set_state(void))

ASM_FUNC("asm/code_08045F18/obj_apply_state_a.s", void obj_apply_state_a(void))

ASM_FUNC("asm/code_08045F18/obj_apply_state_b.s", void obj_apply_state_b(void))

ASM_FUNC("asm/code_08045F18/obj_set_sprite_slot.s", void obj_set_sprite_slot(void))

ASM_FUNC("asm/code_08045F18/obj_set_sprite_link.s", void obj_set_sprite_link(void))
