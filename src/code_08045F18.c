#include "global.h"
#include "ram_routines.h"

struct FieldSub
{
    u8 filler0[4];
    u16 h04;
    u16 h06;
    u16 h08;
    u16 h0A;
    u16 h0C;
    u8 filler0E[0x11 - 0x0E];
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
    u8 filler25[0x34 - 0x25];
    s32 tgtX;
    s32 tgtY;
    s32 unk3C;
    u8 filler40[0x4C - 0x40];
    u8 unk4C;
    u8 filler4D[0x4E - 0x4D];
    u8 flags4E;
    u8 filler4F[0x1EC - 0x4F];
    s32 vec1EC[3];
    u8 filler1F8[0x20C - 0x1F8];
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
          u16 unk330;
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

ASM_FUNC("asm/code_08045F18/cell_motion_obj_move_toward_target.s", void cell_motion_obj_move_toward_target(void))

struct ShapeRef
{
    u8 filler0[0x0C];
    s32 a0C;
    s32 a10;
    s32 a14;
    s32 a18;
};

void motion_obj_step_motion(struct FieldObj *obj);
void rotate_vec(void *vec, u32 angle, s32 *out0, s32 *out1, s32 *out2);
u32 arc_tan2(s32 x, s32 y);

NONMATCH("asm/code_08045F18/advance_obj_motion.s", void advance_obj_motion(struct FieldObj *obj, struct ShapeRef **shapes))
{
    s32 one = 1;
    u32 seven = 7;
    s32 vx;
    s32 vy;
    s32 vz;
    s32 mode;
    s32 v;
    s32 t;
    s32 idx;
    s32 ang;
    s32 delta;

    if (obj->flags20C & 1)
    {
        if (seven & obj->moveFlags2B5)
        {
            obj->unk2DE++;
            mode = ((struct MoveBits *)&obj->moveFlags2B5)->lo3;
            if (mode == 2)
            {
                v = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)((obj->unk2CC - obj->unk2BC) * obj->unk2DE, obj->v2E0);
                obj->posX = obj->unk2BC + v;
                v = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)((obj->unk2D0 - obj->unk2C0) * obj->unk2DE, obj->v2E0);
                obj->posY = obj->unk2C0 + v;
                if (!(obj->flags20D & 0x40) && obj->counter25C == -1)
                {
                    v = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)((obj->unk2D4 - obj->unk2C4) * obj->unk2DE, obj->v2E0);
                    obj->posZB = obj->unk2C4 + v - obj->unk3C;
                }
            }
            else if (mode > 2)
            {
                if (mode <= 5)
                {
                    if (mode == 4)
                    {
                        obj->unk2F4 = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(obj->unk2F0 * obj->unk2DE, obj->v2E0);
                    }
                    else
                    {
                        obj->unk2F4 += obj->unk2F8;
                    }
                    if (obj->unk2F0 > 0)
                    {
                        if (obj->unk2F4 > obj->unk2F0)
                            obj->unk2F4 = obj->unk2F0;
                    }
                    else if (obj->unk2F0 < 0)
                    {
                        if (obj->unk2F4 < obj->unk2F0)
                            obj->unk2F4 = obj->unk2F0;
                    }
                    t = obj->unk2F4;
                    if (t < 0)
                        t += 0xFF;
                    rotate_vec(obj->vec1EC, (u8)(t >> 8), &vx, &vy, &vz);
                    idx = (s8)(((obj->flags2B8 & 7) << 5) | (obj->flags2B7 >> 3));
                    if (idx >= 0)
                    {
                        obj->unk2E4 = shapes[idx]->a0C;
                        obj->unk2E8 = shapes[idx]->a10;
                        obj->unk2EC = shapes[idx]->a14 + shapes[idx]->a18;
                    }
                    obj->posX = obj->unk2E4 + vx;
                    obj->posY = obj->unk2E8 + vy;
                    if (!(obj->flags20D & 0x40) && obj->counter25C == -1)
                    {
                        obj->posZ = 0;
                        obj->posZB = obj->unk2EC + vz;
                    }
                    if (obj->tgtX != obj->posX || obj->tgtY != obj->posY)
                    {
                        obj->v242 = obj->tgtX - obj->posX;
                        obj->v244 = obj->tgtY - obj->posY;
                    }
                    if (obj->posY == obj->tgtY && obj->posX == obj->tgtX)
                        ang = -1;
                    else
                        ang = (u16)arc_tan2((s16)(obj->tgtY - obj->posY), (s16)(obj->posX - obj->tgtX));
                    if (ang != -1)
                    {
                        if (ang < 0)
                            ang += 0xFF;
                        obj->unk02 = ang >> 8;
                    }
                }
            }
            else if (mode == 1)
            {
                if ((((((struct MoveBits *)&obj->flags2B7)->lo3) ^ one) & one) != 0 && obj->unk2CC != obj->unk2BC)
                    obj->posX += obj->unk2D8;
                if (!(((struct MoveBits *)&obj->flags2B7)->lo3 & 2) && obj->unk2D0 != obj->unk2C0)
                    obj->posY += obj->unk2DA;
                if (!(((struct MoveBits *)&obj->flags2B7)->lo3 & 4) && obj->unk2D4 != obj->unk2C4 && obj->counter25C == -1)
                    obj->posZB += obj->unk2DC;
            }
            delta = 0;
            switch (((struct ModeBits23 *)&obj->flags4E)->mode)
            {
            case 1:
                delta = obj->posX - obj->tgtX;
                break;
            case 2:
                delta = obj->tgtX - obj->posX;
                break;
            case 3:
                delta = obj->tgtY - obj->posY;
                break;
            }
            if (delta != 0)
            {
                obj->posZ += delta;
                obj->posZB -= delta;
                if (obj->posZB < 0)
                    obj->posZB = 0;
            }
        }
        if (obj->moveFlags2B5 & 8)
            motion_obj_step_motion(obj);
    }
}
END_NONMATCH

ASM_FUNC("asm/code_08045F18/obj_advance_motion.s", void obj_advance_motion(void))


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
            step = -spd;
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
    s32 keepMask = 0x3F;
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
    *flags = (*flags & keepMask) | bits;
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
            step = -spd;
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
            step = -spd;
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

NONMATCH("asm/code_08045F18/obj_advance_axes.s", void obj_advance_axes(struct FieldObj *obj))
{
    s32 v;
    s32 lim;

    if (obj->flags20F & 2)
    {
        if ((obj->moveFlags2B5 & 0x30) == 0x10)
        {
            obj->axisACount++;
            obj->sub->h04 += obj->axisAStep;
            if ((s16)obj->axisAStep > 0)
            {
                if ((s16)obj->sub->h04 > (s16)obj->axisATarget)
                    obj->sub->h04 = obj->axisATarget;
            }
            else
            {
                if ((s16)obj->sub->h04 < (s16)obj->axisATarget)
                    obj->sub->h04 = obj->axisATarget;
            }
        }
        else if ((obj->moveFlags2B5 & 0x30) == 0x20)
        {
            u16 *cnt = &obj->axisACount;
            s16 *tgt = (s16 *)&obj->axisATarget;
            s16 *cur = (s16 *)&obj->axisACur;
            (*cnt)++;
            v = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)((*tgt - *cur) * *cnt, obj->axisASpeed);
            obj->sub->h04 = obj->axisACur + v;
        }
        if ((obj->moveFlags2B5 & 0xC0) == 0x40)
        {
            obj->axisBCount++;
            obj->sub->h06 += obj->axisBStep;
            if ((s16)obj->axisBStep > 0)
            {
                if ((s16)obj->sub->h06 > (s16)obj->axisBTarget)
                    obj->sub->h06 = obj->axisBTarget;
            }
            else
            {
                if ((s16)obj->sub->h06 < (s16)obj->axisBTarget)
                    obj->sub->h06 = obj->axisBTarget;
            }
        }
        else if ((obj->moveFlags2B5 & 0xC0) == 0x80)
        {
            obj->axisBCount++;
            v = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)((s16)(obj->axisBTarget - (s16)obj->axisBCur) * obj->axisBCount, obj->axisBSpeed);
            obj->sub->h06 = obj->axisBCur + v;
        }
        if ((obj->flags2B6 & 3) == 1)
        {
            obj->unk316++;
            obj->sub->h08 += obj->unk30E;
            if ((s16)obj->unk30E > 0)
            {
                if ((s16)obj->sub->h08 > (s16)obj->unk306)
                    obj->sub->h08 = obj->unk306;
            }
            else
            {
                if ((s16)obj->sub->h08 < (s16)obj->unk306)
                    obj->sub->h08 = obj->unk306;
            }
        }
        else if ((obj->flags2B6 & 3) == 2)
        {
            obj->unk316++;
            v = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)((s16)(obj->unk306 - (s16)obj->unk2FE) * obj->unk316, obj->unk31E);
            obj->sub->h08 = obj->unk2FE + v;
        }
        if ((obj->flags2B6 & 0xC) == 4)
        {
            obj->unk318++;
            obj->sub->h0A += obj->unk310;
            if ((s16)obj->unk310 > 0)
            {
                if ((s16)obj->sub->h0A > (s16)obj->unk308)
                    obj->sub->h0A = obj->unk308;
            }
            else
            {
                if ((s16)obj->sub->h0A < (s16)obj->unk308)
                    obj->sub->h0A = obj->unk308;
            }
        }
        else if ((obj->flags2B6 & 0xC) == 8)
        {
            obj->unk318++;
            v = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)((s16)(obj->unk308 - (s16)obj->unk300) * obj->unk318, obj->unk320);
            obj->sub->h0A = obj->unk300 + v;
        }
        if ((obj->flags2B6 & 0x30) == 0x10)
        {
            obj->unk330++;
            obj->unk32C += obj->unk324;
            lim = obj->unk328;
            if (lim > 0)
            {
                if (obj->unk32C > lim)
                    obj->unk32C = lim;
            }
            else if (lim < 0)
            {
                if (obj->unk32C < lim)
                    obj->unk32C = lim;
            }
            obj->sub->h0C = (u16)obj->unk322 + obj->unk32C;
        }
        else if ((obj->flags2B6 & 0x30) == 0x20)
        {
            obj->unk330++;
            v = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)((obj->unk328 - (s16)obj->unk322) * obj->unk330, obj->unk332);
            obj->unk32C = v;
            obj->sub->h0C = (u16)obj->unk322 + v;
        }
    }
}
END_NONMATCH

struct Flags11
{
    u8 lo6 : 6;
    u8 bit6 : 1;
    u8 hi1 : 1;
};

NONMATCH("asm/code_08045F18/obj_set_cell_coord.s", void obj_set_cell_coord(struct FieldObj *obj, s32 pos, u32 extra))
{
    u32 cell;
    u32 high;
    u8 low;
    u8 entry;

    cell = 0;
    high = 0;
    if ((obj->flags216 & 0xC) != 0)
    {
        if ((obj->flags216 & 0x10) == 0)
        {
            if ((obj->flags216 & 0xC) == 4)
            {
                s32 t = pos / 4;
                cell = (t << 1) + t;
                pos -= t << 2;
                pos <<= 1;
            }
            else
            {
                s32 t = pos / 8;
                cell = (t << 2) + t;
                pos -= t << 3;
            }
        }
        else
        {
            if ((obj->flags216 & 0xC) == 4)
            {
                s32 t = pos / 4;
                cell = t << 2;
                pos -= cell;
                pos <<= 1;
            }
            else
            {
                s32 t = pos / 8;
                cell = t << 3;
                pos -= cell;
            }
        }
        low = pos & 7;
        ((struct MoveBits *)&obj->unk24)->lo3 = low;
        ((struct MoveBits *)&obj->unk4C)->lo3 = low;
        entry = gFrameAttrTable[((((u32)obj->flags216 << 28) >> 30) << 4)
            + ((((u32)obj->flags216 << 27) >> 31) << 3) + pos];
        pos = entry & 0x7F;
        high = entry >> 7;
    }
    obj_load_record(obj->sub, cell + pos, extra, 0, 0);
    ((struct Flags11 *)&obj->sub->flags11)->bit6 = high;
}
END_NONMATCH

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

NONMATCH("asm/code_08045F18/obj_step_index.s", void obj_step_index(struct FieldObj *obj, struct FieldObj *src, u32 advance))
{
    u32 *word;
    u32 cur;
    u32 idx;
    u32 m15;

    if (src != 0)
    {
        m15 = 0xF;
        cur = *(u32 *)&src->state214 & 0x1E000;
        word = (u32 *)&obj->state214;
        cur = (*word & 0xFFFE1FFF) | cur;
        *word = cur;
        if (advance)
        {
            idx = (cur << 15) >> 28;
            if (idx <= 14)
            {
                idx = (idx + 1) & m15;
                *word = (cur & 0xFFFE1FFF) | (idx << 13);
            }
        }
        else
        {
            if ((cur & 0x1E000) != 0)
            {
                idx = (((cur << 15) >> 28) - 1) & m15;
                *word = (cur & 0xFFFE1FFF) | (idx << 13);
            }
        }
    }
    else
    {
        word = (u32 *)&obj->state214;
        if (advance)
            *word = *word | 0x1E000;
        else
            *word = *word & 0xFFFE1FFF;
    }
}
END_NONMATCH

static inline void obj_set_state_bit6(struct FieldObj *o)
{
    o->state214 |= 0x40;
}

NONMATCH("asm/code_08045F18/obj_set_state.s", void obj_set_state(struct FieldObj *obj, u8 state))
{
    u8 *p;
    u32 old;
    u32 v;
    u32 m;

    m = 7;
    p = &obj->state214;
    old = *p;
    v = ((old & -0x39) | ((old << 29) >> 26)) & -8;
    v = v | (state & m);
    *p = v;
    if ((((u32)v << 29) >> 29) > 2)
        obj->unk02 = obj->unk24 << 5;
    if ((*p & 7) == 3)
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
