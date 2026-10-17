#include "global.h"
#include "ram_routines.h"

struct FieldSub
{
    u8 filler0[4];
    u16 h04;
    u16 h06;
    u16 h08;
    u16 h0A;
    u8 filler0C[0x11 - 0x0C];
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

struct ObjClass
{
    u8 filler0[0x38];
    s16 thisOffset38;
    u16 filler3A;
    void (*fn3C)(void *self, u32 mode);
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
    u8 filler4D[0x20C - 0x4D];
    u8 flags20C;
    u8 flags20D;
    u8 filler20E[0x20F - 0x20E];
    u8 flags20F;
    u8 filler210[0x213 - 0x210];
    u8 change213;
    u8 state214;
    u8 flags215;
    u8 flags216;
    u8 filler217;
    u16 id218;
    u16 nextId21A;
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
    u8 flags2B6;
    u8 flags2B7;
    u8 flags2B8;
    u8 filler2B9[0x2BC - 0x2B9];
    s32 unk2BC;
    s32 unk2C0;
    s32 unk2C4;
    u8 filler2C8[0x2CC - 0x2C8];
    s32 unk2CC;
    s32 unk2D0;
    s32 unk2D4;
    s16 unk2D8;
    s16 unk2DA;
    s16 unk2DC;
    u16 unk2DE;
    u16 v2E0;
    u8 filler2E2[0x2E4 - 0x2E2];
    s32 unk2E4;
    s32 unk2E8;
    s32 unk2EC;
    s32 unk2F0;
    s32 unk2F4;
    s16 unk2F8;
    u16 axisACur;
    u16 axisBCur;
    u16 unk2FE;
    u16 unk300;
    u16 axisATarget;
    u16 axisBTarget;
    u16 unk306;
    u16 unk308;
    u16 axisAStep;
    u16 axisBStep;
    u16 unk30E;
    u16 unk310;
    u16 axisACount;
    u16 axisBCount;
    u16 unk316;
    u16 unk318;
    u16 axisASpeed;
    u16 axisBSpeed;
    u16 unk31E;
    u16 unk320;
    s16 unk322;
          s16 unk324;
      u8 filler326[0x328 - 0x326];
    s32 unk328;
    s32 unk32C;
          u8 filler330[2];
      u16 unk332;
      u8 filler334[4];
    struct ObjClass *klass338;
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

struct SlotBits
{
    u8 bit0 : 1;
    u8 has0 : 1;
    u8 has1 : 1;
    u8 flip0 : 1;
    u8 flip1 : 1;
    u8 hi3 : 3;
};

void obj_sync_sub_slot(struct FieldObj *obj, s32 slot)
{
    u32 flip;

    if (slot == 0)
    {
        obj->anim220 = obj->sub->anim21;
        flip = ((u32)obj->sub->flags11 << 25) >> 31;
        ((struct SlotBits *)&obj->flags215)->flip0 = flip;
        ((struct SlotBits *)&obj->flags215)->has0 = 1;
    }
    else
    {
        obj->anim221 = obj->sub->anim21;
        flip = ((u32)obj->sub->flags11 << 25) >> 31;
        ((struct SlotBits *)&obj->flags215)->flip1 = flip;
        ((struct SlotBits *)&obj->flags215)->has1 = 1;
    }
}

ASM_FUNC("asm/code_08045F18/obj_step_index.s", void obj_step_index(void))

ASM_FUNC("asm/code_08045F18/obj_set_state.s", void obj_set_state(void))

void obj_apply_state_a(struct FieldObj *obj)
{
    u8 *p;
    u32 v;
    u32 w;
    u32 idx;
    u32 f;
    s32 anim;
    s32 sub;
    u32 top;

    p = &obj->state214;
    v = *p;
    if (v & 0x40)
    {
        idx = ((u32)(v << 26)) >> 29;
        w = (v & -8) | idx;
        *p = w;
        if (((u32)(w << 29) >> 29) > 2)
            obj->unk02 = obj->unk24 << 5;
        *(s8 *)p &= -0x41;
    }
    if (obj->state214 & 0x80)
    {
        if (obj->nextId21A != obj->id218)
        {
            obj->id218 = obj->nextId21A;
            obj_update_special_flag(obj);
        }
    }
    top = obj->state214 >> 7;
    if (obj->flags215 & 2)
        anim = obj->anim220;
    else
        anim = -1;
    f = obj->flags215;
    if (f & 2)
        sub = (f << 28) >> 31;
    else
        sub = -1;
    obj_refresh_anim_state(obj, top, anim, sub);
    obj->state214 &= 0x7F;
    *(s8 *)&obj->flags215 &= -3;
    obj->klass338->fn3C((u8 *)obj + obj->klass338->thisOffset38, 1);
}

ASM_FUNC("asm/code_08045F18/obj_apply_state_b.s", void obj_apply_state_b(void))

void obj_set_sprite_slot(struct FieldObj *obj, u32 slot)
{
    u16 v;
    u32 hi;
    u16 lo;
    u32 i;

    v = *(slot + gUnk_0203FFC4);
    hi = (v >> 10) & 0xf;
    lo = v & 0x3ff;
    obj->h21E = ((hi + 1) << 12) | lo;
    ((struct MoveBits *)&obj->sub->b1E)->hi4 = hi;
    obj->sub->index1C = lo;
    obj->sub->mask14 = 0;
    for (i = 0; i < (obj->sub->count13 >> 4); i++, slot++)
    {
        obj->sub->mask14 |= 1 << slot;
    }
}

ASM_FUNC("asm/code_08045F18/obj_set_sprite_link.s", void obj_set_sprite_link(void))
