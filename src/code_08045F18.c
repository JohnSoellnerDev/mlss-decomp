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
    u8 flags12;
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
    u8 flags20E;
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
    u8 unk222;
    u8 filler223[0x23D - 0x223];
    u8 unk23D;
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
    u8 filler26A[2];
    u16 unk26C;
    u8 unk26E;
    u8 filler26F[0x294 - 0x26F];
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
      void *unk334;
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

struct ModeBits45
{
    u8 lo4 : 4;
    u8 mode : 2;
    u8 hi2 : 2;
};

struct ModeBits23
{
    u8 lo2 : 2;
    u8 mode : 2;
    u8 hi4 : 4;
};

struct ModeBits01
{
    u8 mode : 2;
    u8 hi6 : 6;
};

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

NONMATCH("asm/code_08045F18/obj_set_motion_a_08046AA4.s", u32 obj_set_motion_a_08046AA4(struct FieldObj *obj, u32 mode, u32 relative, u16 value, u16 speed))
{
    u16 spd;
    u32 target;
    s32 now;
    s32 goal;
    s32 step;
    struct FieldSub *sub;
    s32 t;
    u8 *flags;
    u32 bits;

    spd = speed;
    if (relative == 1)
    {
        sub = obj->sub;
        t = (s16)value;
        target = (u16)(t + sub->h04);
    }
    else
    {
        target = value;
    }
    obj->axisACur = obj->sub->h04;
    obj->axisATarget = target;
    now = (s16)obj->axisACur;
    goal = (s16)target;
    if (now == goal)
        return 0;
    if (mode == 1)
    {
        step = speed;
        if (now >= goal)
            step = -speed;
        obj->axisAStep = step;
    }
    else
    {
        obj->axisASpeed = spd;
    }
    obj->axisACount = 0;
    ((struct ModeBits45 *)&obj->moveFlags2B5)->mode = mode;
    return 1;
}
END_NONMATCH

NONMATCH("asm/code_08045F18/obj_set_motion_b.s", u32 obj_set_motion_b(struct FieldObj *obj, u32 mode, u32 relative, u16 value, u16 speed))
{
    u32 target;
    u16 spd;
    s32 now;
    s32 goal;
    s32 step;
    struct FieldSub *sub;
    s32 t;
    u8 *flags;
    u32 bits;

    spd = speed;
    if (relative == 1)
    {
        sub = obj->sub;
        t = (s16)value;
        target = (u16)(t + sub->h06);
    }
    else
    {
        target = value;
    }
    obj->axisBCur = obj->sub->h06;
    obj->axisBTarget = target;
    now = (s16)obj->axisBCur;
    goal = (s16)target;
    if (now == goal)
        return 0;
    if (mode == 1)
    {
        step = speed;
        if (now >= goal)
            step = -speed;
        obj->axisBStep = step;
    }
    else
    {
        obj->axisBSpeed = spd;
    }
    obj->axisBCount = 0;
    flags = &obj->moveFlags2B5;
    bits = mode << 6;
    *flags = (*flags & 0x3F) | bits;
    return 1;
}
END_NONMATCH

NONMATCH("asm/code_08045F18/obj_set_motion_a_08046BDC.s", u32 obj_set_motion_a_08046BDC(struct FieldObj *obj, u32 mode, u32 relative, u16 value, u16 speed))
{
    u32 target;
    u16 spd;
    s32 now;
    s32 goal;
    s32 step;
    struct FieldSub *sub;
    s32 t;
    s16 *pNow;
    s16 *pGoal;

    spd = speed;
    if (relative == 1)
    {
        sub = obj->sub;
        t = (s16)value;
        target = (u16)(t + sub->h08);
    }
    else
    {
        target = value;
    }
    obj->axisACur = obj->sub->h08;
    obj->axisATarget = target;
    pNow = (s16 *)&obj->unk2FE;
    pGoal = (s16 *)&obj->unk306;
    now = *pNow;
    goal = *pGoal;
    if (now == goal)
        return 0;
    if (mode == 1)
    {
        step = speed;
        if (now >= goal)
            step = -speed;
        obj->unk30E = step;
    }
    else
    {
        obj->unk31E = spd;
    }
    obj->unk316 = 0;
    ((struct ModeBits01 *)&obj->flags2B6)->mode = mode;
    return 1;
}
END_NONMATCH

NONMATCH("asm/code_08045F18/obj_set_motion_a_compare.s", u32 obj_set_motion_a_compare(struct FieldObj *obj, u32 mode, u32 relative, u16 value, u16 speed))
{
    u32 target;
    u16 spd;
    s32 now;
    s32 goal;
    s32 step;
    struct FieldSub *sub;
    s32 t;

    spd = speed;
    if (relative == 1)
    {
        sub = obj->sub;
        t = (s16)value;
        target = (u16)(t + sub->h0A);
    }
    else
    {
        target = value;
    }
    obj->unk300 = obj->sub->h0A;
    obj->unk308 = target;
    now = (s16)obj->unk300;
    goal = (s16)target;
    if (now == goal)
        return 0;
    if (mode == 1)
    {
        step = speed;
        if (now >= goal)
            step = -speed;
        obj->unk310 = step;
    }
    else
    {
        obj->unk320 = spd;
    }
    obj->unk318 = 0;
    ((struct ModeBits23 *)&obj->flags2B6)->mode = mode;
    return 1;
}
END_NONMATCH

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

static inline void obj_set_state_bit6(struct FieldObj *o)
{
    o->state214 |= 0x40;
}

NONMATCH("asm/code_08045F18/obj_set_state.s", void obj_set_state(struct FieldObj *obj, u8 state))
{
    u32 m;
    u32 old;
    u32 prev;
    u32 v;

    old = obj->state214;
    m = 7;
    prev = (old << 29) >> 26;
    v = (old & -0x39) | prev;
    v = (v & -8) | (state & m);
    obj->state214 = v;
    if ((((u32)v << 29) >> 29) > 2)
    {
        obj->unk02 = obj->unk24 << 5;
    }
    if ((obj->state214 & 7) == 3)
    {
        struct FieldSub *sub = obj->sub;
        obj_load_record(sub, sub->anim21, 0, 0, 0);
    }
    obj_set_state_bit6(obj);
}
END_NONMATCH

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

void obj_apply_state_b(struct FieldObj *obj)
{
    u8 *p;
    u32 v;
    u32 w;
    u32 idx;
    u32 mode;
    u32 f;
    s32 a;
    s32 b;

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
    if (obj->flags215 & 1)
    {
        if (obj->prevId21C != obj->id218)
        {
            obj->id218 = obj->prevId21C;
            obj_update_special_flag(obj);
        }
    }
    mode = obj->flags215 & 1;
    if (obj->flags215 & 4)
        a = obj->anim221;
    else
        a = -1;
    f = obj->flags215;
    if (f & 4)
        b = ((u32)(f << 27)) >> 31;
    else
        b = -1;
    obj_refresh_anim_state(obj, mode, a, b);
    ((struct SlotBits *)&obj->flags215)->bit0 = 0;
    ((struct SlotBits *)&obj->flags215)->has1 = 0;
}

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
