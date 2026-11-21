#include "global.h"
#include "ram_routines.h"

/* the sprite drawn for a FieldObj; scales are 8.8 fixed point */
struct FieldSub
{
    u8 filler0[4];
    u16 scaleX;         /* sprite scale, applied before the rotation */
    u16 scaleY;
    u16 postScaleX;     /* screen scale, applied after the rotation */
    u16 postScaleY;
    u16 rotation;       /* high byte: angle 0-255 */
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
    u8 unk24;           /* bits 0-2: facing direction 0-7 (unk02 = direction * 32) */
    u8 filler25[0x34 - 0x25];
    s32 tgtX;
    s32 tgtY;
    s32 unk3C;
    u8 filler40[0x4C - 0x40];
    u8 unk4C;           /* bits 0-2: facing direction too */
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
    u8 state214;        /* struct StateBits214; bit 7: id218 was overridden, the old id is in nextId21A */
    u8 flags215;        /* struct SlotBits; bit 0: id218 was overridden, the old id is in prevId21C */
    u8 flags216;
    u8 filler217;
    u16 id218;
    u16 nextId21A;      /* id218 to restore when state214 bit 7 is undone */
    u16 prevId21C;      /* id218 to restore when flags215 bit 0 is undone */
    u16 h21E;
    u8 anim220;         /* sprite animation saved in slot 0 (obj_sync_sub_slot) */
    u8 anim221;         /* sprite animation saved in slot 1 */
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
    /* the current move (moveFlags2B5 bits 0-2): 1 steps by moveStep, 2 goes from moveStart to
     * moveDest in moveDuration frames, 3-5 swing around a pivot (4 over moveDuration frames,
     * 3/5 by arcStep a frame) */
    s32 moveStartX;
    s32 moveStartY;
    s32 moveStartZ;
    u8 filler2C8[0x2CC - 0x2C8];
    s32 moveDestX;
    s32 moveDestY;
    s32 moveDestZ;
    s16 moveStepX;
    s16 moveStepY;
    s16 moveStepZ;
    u16 moveTime;
    u16 moveDuration;
    u8 filler2E2[0x2E4 - 0x2E2];
    s32 pivotX;
    s32 pivotY;
    s32 pivotZ;
    s32 arcAngle; /* how far to swing, 8.8 angle units */
    s32 arcPos;   /* how far it has swung */
    s16 arcStep;
    u16 axisACur;
    u16 axisBCur;
    u16 axisCCur;
    u16 axisDCur;
    u16 axisATarget;
    u16 axisBTarget;
    u16 axisCTarget;
    u16 axisDTarget;
    u16 axisAStep;
    u16 axisBStep;
    u16 axisCStep;
    u16 axisDStep;
    u16 axisACount;
    u16 axisBCount;
    u16 axisCCount;
    u16 axisDCount;
    u16 axisASpeed;
    u16 axisBSpeed;
    u16 axisCSpeed;
    u16 axisDSpeed;
    s16 axisEBase;
    s16 axisEStep;
    u8 filler326[0x328 - 0x326];
    s32 axisETarget;
    s32 axisEOffset;
    u16 axisECount;
    u16 axisESpeed;
    void *unk334;
    struct ObjClass *klass338;
};

/*
 * Sprite tweens: axis A moves sub->scaleX, B scaleY, C postScaleX, D postScaleY, E rotation
 * (as axisEBase + axisEOffset). Each axis has a 2-bit enum TweenMode, at moveFlags2B5 bits 4-5 (A)
 * and 6-7 (B), flags2B6 bits 0-1 (C), 2-3 (D) and 4-5 (E).
 */
enum TweenMode
{
    TWEEN_STEP = 1,     /* add axisStep every frame until the target */
    TWEEN_TIMED = 2,    /* reach the target after axisSpeed frames */
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
    u8 mode : 2; /* axis A's enum TweenMode */
    u8 hi2 : 2;  /* axis B's enum TweenMode */
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

struct MoveBitsP
{
    u8 lo3 : 3;
    u8 bit3 : 1;
    u8 hi4 : 4;
} __attribute__((packed));

struct ModeBits23P
{
    u8 lo2 : 2;
    u8 mode : 2;
    u8 hi4 : 4;
} __attribute__((packed));

/* motion mode, pending-axis pairs and axis lock bits at +0x2B5..+0x2B7 */
struct MotionBits
{
    u8 filler0[0x2B5];
    u32 mode : 3;
    u32 bit3 : 1;
    u8 axisA : 2;
    u8 axisB : 2;
    u8 axisC : 2;
    u8 axisD : 2;
    u8 axisE : 2;
    u8 hi6 : 2;
    u32 lock : 3;
    u32 hi3 : 5;
};

/* axis lock bits and an 8-bit shape index spanning bytes +0x2B7/+0x2B8 */
struct ObjShape2B7
{
    u8 filler0[0x2B7];
    u8 lock : 3;
    s8 shape : 8;
    u8 hi5 : 5;
} __attribute__((packed));

NONMATCH("asm/code_08045F18/advance_obj_motion.s", void advance_obj_motion(struct FieldObj *obj, struct ShapeRef **shapes))
{
    s32 vx;
    s32 vy;
    s32 vz;
    s32 ang;
    s32 delta;

    if (obj->flags20C & 1)
    {
        if (obj->moveFlags2B5 & 7)
        {
            obj->moveTime++;
            switch ((u32)((struct MoveBitsP *)&obj->moveFlags2B5)->lo3)
            {
            case 1:
                if (((((struct ObjShape2B7 *)obj)->lock ^ 1) & 1) != 0 && obj->moveDestX != obj->moveStartX)
                    obj->posX += obj->moveStepX;
                if (!(((struct ObjShape2B7 *)obj)->lock & 2) && obj->moveDestY != obj->moveStartY)
                    obj->posY += obj->moveStepY;
                if (!(((struct ObjShape2B7 *)obj)->lock & 4) && obj->moveDestZ != obj->moveStartZ && obj->counter25C == -1)
                    obj->posZB += obj->moveStepZ;
                break;
            case 2:
                obj->posX = obj->moveStartX + MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)((obj->moveDestX - obj->moveStartX) * obj->moveTime, obj->moveDuration);
                obj->posY = obj->moveStartY + MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)((obj->moveDestY - obj->moveStartY) * obj->moveTime, obj->moveDuration);
                if (!(obj->flags20D & 0x40) && obj->counter25C == -1)
                {
                    obj->posZB = obj->moveStartZ + MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)((obj->moveDestZ - obj->moveStartZ) * obj->moveTime, obj->moveDuration) - obj->unk3C;
                }
                break;
            case 3:
            case 4:
            case 5:
                if (((struct MoveBitsP *)&obj->moveFlags2B5)->lo3 == 4)
                {
                    obj->arcPos = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(obj->arcAngle * obj->moveTime, obj->moveDuration);
                }
                else
                {
                    obj->arcPos += obj->arcStep;
                }
                if (obj->arcAngle > 0)
                {
                    if (obj->arcPos > obj->arcAngle)
                        obj->arcPos = obj->arcAngle;
                }
                else if (obj->arcAngle < 0)
                {
                    if (obj->arcPos < obj->arcAngle)
                        obj->arcPos = obj->arcAngle;
                }
                rotate_vec(obj->vec1EC, (u8)(obj->arcPos / 0x100), &vx, &vy, &vz);
                if (((struct ObjShape2B7 *)obj)->shape >= 0)
                {
                    obj->pivotX = shapes[((struct ObjShape2B7 *)obj)->shape]->a0C;
                    obj->pivotY = shapes[((struct ObjShape2B7 *)obj)->shape]->a10;
                    obj->pivotZ = shapes[((struct ObjShape2B7 *)obj)->shape]->a14 + shapes[((struct ObjShape2B7 *)obj)->shape]->a18;
                }
                obj->posX = obj->pivotX + vx;
                obj->posY = obj->pivotY + vy;
                if (!(obj->flags20D & 0x40) && obj->counter25C == -1)
                {
                    obj->posZ = 0;
                    obj->posZB = obj->pivotZ + vz;
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
                    obj->unk02 = ang / 0x100;
                }
                break;
            }
            delta = 0;
            switch ((u32)((struct ModeBits23P *)&obj->flags4E)->mode)
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

void obj_finish_move_step(struct FieldObj *obj);
void obj_stop_motion(struct FieldObj *obj);
/*
 * Checks whether the object's current move is over: a stepped move (mode 1) locks each axis once
 * it has reached or passed moveDest and ends when its main (fastest) axis has arrived, a swing
 * (3/5) when arcPos reaches arcAngle, a timed move (2/4) after moveDuration frames. Then ends the
 * height motion (bit 3) once its counter has run out, and clears each running scale animation
 * that has reached its target.
 */
void obj_advance_motion(struct FieldObj *obj)
{
    s32 speedX;
    s32 speedY;
    s32 speedZ;
    u32 mainAxis;

    if (obj->flags20C & 1)
    {
        switch (((struct MotionBits *)obj)->mode)
        {
        case 1:
            if ((((u8)((struct MotionBits *)obj)->lock ^ 1) & 1) != 0)
            {
                if ((obj->moveDestX >= obj->moveStartX && obj->moveDestX <= obj->posX)
                    || (obj->moveDestX <= obj->moveStartX && obj->moveDestX >= obj->posX))
                {
                    obj->posX = obj->moveDestX;
                    ((struct MotionBits *)obj)->lock |= 1;
                }
            }
            if (!(((struct MotionBits *)obj)->lock & 2))
            {
                if ((obj->moveDestY >= obj->moveStartY && obj->moveDestY <= obj->posY)
                    || (obj->moveDestY <= obj->moveStartY && obj->moveDestY >= obj->posY))
                {
                    obj->posY = obj->moveDestY;
                    ((struct MotionBits *)obj)->lock |= 2;
                }
            }
            if (!(((struct MotionBits *)obj)->lock & 4) && !(obj->flags20D & 0x40) && obj->counter25C == -1)
            {
                if ((obj->moveDestZ >= obj->moveStartZ && obj->moveDestZ <= obj->posZ + obj->posZB)
                    || (obj->moveDestZ <= obj->moveStartZ && obj->moveDestZ >= obj->posZ + obj->posZB))
                {
                    obj->posZB = obj->moveDestZ;
                    obj->posZ = 0;
                    ((struct MotionBits *)obj)->lock |= 4;
                }
            }
            speedX = obj->moveStepX;
            if (speedX < 0)
                speedX = -speedX;
            speedY = obj->moveStepY;
            if (speedY < 0)
                speedY = -speedY;
            speedZ = obj->moveStepZ;
            if (speedZ < 0)
                speedZ = -speedZ;
            /* lock bit of the axis that moves fastest */
            if (speedX >= speedY)
                mainAxis = speedX >= speedZ ? 1 : 4;
            else
                mainAxis = speedY >= speedZ ? 2 : 4;
            if (((struct MotionBits *)obj)->lock & mainAxis)
            {
                obj->posX = obj->moveDestX;
                obj->posY = obj->moveDestY;
                if (!(obj->flags20D & 0x40) && obj->counter25C == -1)
                    obj->posZB = obj->moveDestZ - obj->posZ;
                obj_finish_move_step(obj);
            }
            break;
        case 3:
        case 5:
            if (obj->arcAngle > 0)
            {
                if (obj->arcPos >= obj->arcAngle)
                    obj_finish_move_step(obj);
            }
            else if (obj->arcAngle < 0)
            {
                if (obj->arcPos <= obj->arcAngle)
                    obj_finish_move_step(obj);
            }
            break;
        case 2:
        case 4:
            if (obj->moveTime >= obj->moveDuration)
                obj_finish_move_step(obj);
            break;
        }
        if ((obj->moveFlags2B5 & 8) && obj->counter25C == -1)
            obj_stop_motion(obj);
    }
    if (obj->flags20F & 2)
    {
        if (((struct MotionBits *)obj)->axisA && (s16)obj->sub->scaleX == (s16)obj->axisATarget)
            ((struct MotionBits *)obj)->axisA = 0;
        if (((struct MotionBits *)obj)->axisB && (s16)obj->sub->scaleY == (s16)obj->axisBTarget)
            ((struct MotionBits *)obj)->axisB = 0;
        if (((struct MotionBits *)obj)->axisC && (s16)obj->sub->postScaleX == (s16)obj->axisATarget)
            ((struct MotionBits *)obj)->axisC = 0;
        if (((struct MotionBits *)obj)->axisD && (s16)obj->sub->postScaleY == (s16)obj->axisBTarget)
            ((struct MotionBits *)obj)->axisD = 0;
        if (((struct MotionBits *)obj)->axisE && obj->axisEBase + obj->axisEOffset == obj->axisETarget)
            ((struct MotionBits *)obj)->axisE = 0;
    }
}


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
        obj->moveDuration = 0;
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

/* Starts the axis A tween (sub->scaleX) toward value (added to the current scale when relative is
 * 1): TWEEN_STEP moves by speed each frame, TWEEN_TIMED takes speed frames. Returns 0 (and starts
 * nothing) if the scale is already there, else 1. */
u32 obj_set_motion_a_08046AA4(struct FieldObj *obj, u32 mode, u32 relative, u16 value, u16 speed)
{
    s32 now;
    s32 goal;
    s32 step;
    struct FieldSub *sub;
    s32 offset;

    if (relative == 1)
    {
        sub = obj->sub;
        offset = (s16)value;
        value = (u16)(offset + sub->scaleX);
    }
    obj->axisACur = obj->sub->scaleX;
    obj->axisATarget = value;
    now = (s16)obj->axisACur;
    goal = (s16)value;
    if (now == goal)
        return 0;
    if (mode == TWEEN_STEP)
    {
        step = speed;
        if (now >= goal)
            step = -speed;
        obj->axisAStep = step;
    }
    else
    {
        obj->axisASpeed = speed;
    }
    obj->axisACount = 0;
    ((struct ModeBits45 *)&obj->moveFlags2B5)->mode = mode; /* axis A's TweenMode */
    return 1;
}

/* Starts the axis B tween (sub->scaleY) toward value (added to the current scale when relative is
 * 1): TWEEN_STEP moves by speed each frame, TWEEN_TIMED takes speed frames. Returns 0 (and starts
 * nothing) if the scale is already there, else 1. */
u32 obj_set_motion_b(struct FieldObj *obj, u32 mode, u32 relative, u16 value, u16 speed)
{
    s32 now;
    s32 goal;
    s32 step;
    struct FieldSub *sub;
    s32 offset;

    if (relative == 1)
    {
        sub = obj->sub;
        offset = (s16)value;
        value = (u16)(offset + sub->scaleY);
    }
    obj->axisBCur = obj->sub->scaleY;
    obj->axisBTarget = value;
    now = (s16)obj->axisBCur;
    goal = (s16)value;
    if (now == goal)
        return 0;
    if (mode == TWEEN_STEP)
    {
        step = speed;
        if (now >= goal)
            step = -speed;
        obj->axisBStep = step;
    }
    else
    {
        obj->axisBSpeed = speed;
    }
    obj->axisBCount = 0;
    ((struct ModeBits45 *)&obj->moveFlags2B5)->hi2 = mode; /* axis B's TweenMode */
    return 1;
}

/* Starts the axis C tween (sub->postScaleX) toward value (added to the current scale when relative
 * is 1): TWEEN_STEP moves by speed each frame, TWEEN_TIMED takes speed frames. Returns 0 if there
 * is nothing to do. The game stores the start and goal in axis A's fields here (a bug), so the
 * check reads axis C's old start and goal. */
u32 obj_set_motion_a_08046BDC(struct FieldObj *obj, u32 mode, u32 relative, u16 value, u16 speed)
{
    s32 now;
    s32 goal;
    s32 step;
    struct FieldSub *sub;
    s32 offset;
    s16 *cur;
    s16 *target;

    if (relative == 1)
    {
        sub = obj->sub;
        offset = (s16)value;
        value = (u16)(offset + sub->postScaleX);
    }
    obj->axisACur = obj->sub->postScaleX;
    obj->axisATarget = value;
    cur = (s16 *)&obj->axisCCur;
    target = (s16 *)&obj->axisCTarget;
    now = *cur;
    goal = *target;
    if (now == goal)
        return 0;
    if (mode == TWEEN_STEP)
    {
        step = speed;
        if (now >= goal)
            step = -speed;
        obj->axisCStep = step;
    }
    else
    {
        obj->axisCSpeed = speed;
    }
    obj->axisCCount = 0;
    ((struct ModeBits01 *)&obj->flags2B6)->mode = mode;
    return 1;
}

/* Starts the axis D tween (sub->postScaleY) toward value (added to the current scale when relative
 * is 1): TWEEN_STEP moves by speed each frame, TWEEN_TIMED takes speed frames. Returns 0 if the
 * scale is already there, else 1. */
u32 obj_set_motion_a_compare(struct FieldObj *obj, u32 mode, u32 relative, u16 value, u16 speed)
{
    s32 now;
    s32 goal;
    s32 step;
    struct FieldSub *sub;
    s32 offset;

    if (relative == 1)
    {
        sub = obj->sub;
        offset = (s16)value;
        value = (u16)(offset + sub->postScaleY);
    }
    obj->axisDCur = obj->sub->postScaleY;
    obj->axisDTarget = value;
    now = (s16)obj->axisDCur;
    goal = (s16)value;
    if (now == goal)
        return 0;
    if (mode == TWEEN_STEP)
    {
        step = speed;
        if (now >= goal)
            step = -speed;
        obj->axisDStep = step;
    }
    else
    {
        obj->axisDSpeed = speed;
    }
    obj->axisDCount = 0;
    ((struct ModeBits23 *)&obj->flags2B6)->mode = mode;
    return 1;
}

/* Advances each running sprite tween (scale, screen scale, rotation) of obj by one frame. */
void obj_advance_axes(struct FieldObj *obj)
{
    s32 offset;
    s32 target;

    if (obj->flags20F & 2)
    {
        if ((obj->moveFlags2B5 & 0x30) == TWEEN_STEP << 4)
        {
            obj->axisACount++;
            obj->sub->scaleX += obj->axisAStep;
            if ((s16)obj->axisAStep > 0)
            {
                if ((s16)obj->sub->scaleX > (s16)obj->axisATarget)
                    obj->sub->scaleX = obj->axisATarget;
            }
            else
            {
                if ((s16)obj->sub->scaleX < (s16)obj->axisATarget)
                    obj->sub->scaleX = obj->axisATarget;
            }
        }
        else if ((obj->moveFlags2B5 & 0x30) == TWEEN_TIMED << 4)
        {
            obj->axisACount++;
            obj->sub->scaleX = obj->axisACur + MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(((s16)obj->axisATarget - (s16)obj->axisACur) * obj->axisACount, obj->axisASpeed);
        }
        if ((obj->moveFlags2B5 & 0xC0) == TWEEN_STEP << 6)
        {
            obj->axisBCount++;
            obj->sub->scaleY += obj->axisBStep;
            if ((s16)obj->axisBStep > 0)
            {
                if ((s16)obj->sub->scaleY > (s16)obj->axisBTarget)
                    obj->sub->scaleY = obj->axisBTarget;
            }
            else
            {
                if ((s16)obj->sub->scaleY < (s16)obj->axisBTarget)
                    obj->sub->scaleY = obj->axisBTarget;
            }
        }
        else if ((obj->moveFlags2B5 & 0xC0) == TWEEN_TIMED << 6)
        {
            obj->axisBCount++;
            obj->sub->scaleY = obj->axisBCur + MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(((s16)obj->axisBTarget - (s16)obj->axisBCur) * obj->axisBCount, obj->axisBSpeed);
        }
        if ((obj->flags2B6 & 3) == TWEEN_STEP)
        {
            obj->axisCCount++;
            obj->sub->postScaleX += obj->axisCStep;
            if ((s16)obj->axisCStep > 0)
            {
                if ((s16)obj->sub->postScaleX > (s16)obj->axisCTarget)
                    obj->sub->postScaleX = obj->axisCTarget;
            }
            else
            {
                if ((s16)obj->sub->postScaleX < (s16)obj->axisCTarget)
                    obj->sub->postScaleX = obj->axisCTarget;
            }
        }
        else if ((obj->flags2B6 & 3) == TWEEN_TIMED)
        {
            obj->axisCCount++;
            obj->sub->postScaleX = obj->axisCCur + MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(((s16)obj->axisCTarget - (s16)obj->axisCCur) * obj->axisCCount, obj->axisCSpeed);
        }
        if ((obj->flags2B6 & 0xC) == TWEEN_STEP << 2)
        {
            obj->axisDCount++;
            obj->sub->postScaleY += obj->axisDStep;
            if ((s16)obj->axisDStep > 0)
            {
                if ((s16)obj->sub->postScaleY > (s16)obj->axisDTarget)
                    obj->sub->postScaleY = obj->axisDTarget;
            }
            else
            {
                if ((s16)obj->sub->postScaleY < (s16)obj->axisDTarget)
                    obj->sub->postScaleY = obj->axisDTarget;
            }
        }
        else if ((obj->flags2B6 & 0xC) == TWEEN_TIMED << 2)
        {
            obj->axisDCount++;
            obj->sub->postScaleY = obj->axisDCur + MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(((s16)obj->axisDTarget - (s16)obj->axisDCur) * obj->axisDCount, obj->axisDSpeed);
        }
        if ((obj->flags2B6 & 0x30) == TWEEN_STEP << 4)
        {
            obj->axisECount++;
            obj->axisEOffset += obj->axisEStep;
            target = obj->axisETarget;
            if (target > 0)
            {
                if (obj->axisEOffset > target)
                    obj->axisEOffset = target;
            }
            else if (target < 0)
            {
                if (obj->axisEOffset < target)
                    obj->axisEOffset = target;
            }
            obj->sub->rotation = obj->axisEBase + obj->axisEOffset;
        }
        else if ((obj->flags2B6 & 0x30) == TWEEN_TIMED << 4)
        {
            obj->axisECount++;
            offset = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)((obj->axisETarget - (s16)obj->axisEBase) * obj->axisECount, obj->axisESpeed);
            obj->axisEOffset = offset;
            obj->sub->rotation = obj->axisEBase + offset;
        }
    }
}

struct Flags11
{
    u8 lo6 : 6;
    u8 bit6 : 1;        /* drawn flipped horizontally */
    u8 hi1 : 1;
};

/* FieldObj.flags216 bits 2-3: how many directions the object's frames face (0: one, 1: four,
   2/3: eight). Bit 4 clear: mirrored directions share frames, so a group has 3 (or 5) frames;
   set: every direction has its own. gFrameAttrTable[bits 2-3][bit 4][direction] gives the
   frame within the group (bits 0-6) and whether it is drawn flipped (bit 7). */
#define FIELD_OBJ_DIRS_MASK 0x0C
#define FIELD_OBJ_DIRS_4 0x04
#define FIELD_OBJ_ALL_DIRS_DRAWN 0x10

/*
 * Shows frame `index` of an object whose frames face several directions (index = group *
 * directions + direction, see FIELD_OBJ_DIRS_MASK): stores the direction (four-direction indices
 * are doubled to 0, 2, 4, 6) as the object's facing, looks up the group's frame for it in
 * gFrameAttrTable, loads that record into the sprite and flips the sprite when the table says so.
 * An object with one direction just loads record `index`.
 */
void obj_set_cell_coord(struct FieldObj *obj, s32 index, u32 loadFlags)
{
    u32 groupStart;
    u32 flipped;
    u8 attr;

    groupStart = 0;
    flipped = 0;
    if ((obj->flags216 & FIELD_OBJ_DIRS_MASK) != 0)
    {
        if ((obj->flags216 & FIELD_OBJ_ALL_DIRS_DRAWN) == 0)
        {
            if ((obj->flags216 & FIELD_OBJ_DIRS_MASK) == FIELD_OBJ_DIRS_4)
            {
                /* 3 frames a group */
                s32 group = index / 4;
                groupStart = (group << 1) + group;
                index -= group << 2;
                index <<= 1;
            }
            else
            {
                /* 5 frames a group */
                s32 group = index / 8;
                groupStart = (group << 2) + group;
                index -= group << 3;
            }
        }
        else
        {
            if ((obj->flags216 & FIELD_OBJ_DIRS_MASK) == FIELD_OBJ_DIRS_4)
            {
                s32 group = index / 4;
                groupStart = group << 2;
                index -= groupStart;
                index <<= 1;
            }
            else
            {
                s32 group = index / 8;
                groupStart = group << 3;
                index -= groupStart;
            }
        }
        ((struct MoveBits *)&obj->unk24)->lo3 = index;
        ((struct MoveBits *)&obj->unk4C)->lo3 = index;
        /* [flags216 bits 2-3][flags216 bit 4][direction] */
        attr = gFrameAttrTable[((((u32)obj->flags216 << 28) >> 30) << 4)
            + ((((u32)obj->flags216 << 27) >> 31) << 3) + index];
        index = attr & 0x7F;
        flipped = attr >> 7;
    }
    obj_load_record(obj->sub, groupStart + index, loadFlags, 0, 0);
    ((struct Flags11 *)&obj->sub->flags11)->bit6 = flipped;
}

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

/* 4-bit step index in bits 13-16 of the word at +0x214 */
struct IdxWord214
{
    u8 filler0[0x214];
    u32 lo13 : 13;
    u32 idx : 4;
    u32 hi15 : 15;
};

void obj_step_index(struct FieldObj *obj, struct FieldObj *src, u32 advance)
{
    struct IdxWord214 *o = (struct IdxWord214 *)obj;

    if (src != 0)
    {
        o->idx = ((struct IdxWord214 *)src)->idx;
        if (advance)
        {
            if ((u32)o->idx < 15)
                o->idx++;
        }
        else
        {
            /* index bits tested on the whole word */
            if (*(u32 *)&obj->state214 & 0x1E000)
                o->idx--;
        }
    }
    else
    {
        if (advance)
            o->idx = 15;
        else
            o->idx = 0;
    }
}

/* state byte: current state in bits 0-2, previous state in bits 3-5 */
struct StateBits214
{
    u8 cur : 3;
    u8 prev : 3;
    u8 bit6 : 1;
    u8 bit7 : 1;
} __attribute__((packed));

void obj_set_state(struct FieldObj *obj, u8 state)
{
    struct StateBits214 *st = (struct StateBits214 *)&obj->state214;

    st->prev = st->cur;
    st->cur = state;
    if (st->cur > 2)
    {
        u32 v;
        obj->unk02 = obj->unk24 << 5;
        v = obj->state214;
        {
            register u32 mask asm("r0") = 7;
            mask &= v;
            if (mask == 3)
            {
                struct FieldSub *sub = obj->sub;
                obj_load_record(sub, sub->anim21, 0, 0, 0);
            }
        }
    }
    ((struct StateBits214 *)&obj->state214)->bit6 = 1;
}

/*
 * Undoes a temporary override: a pending state change goes back to the previous state (facing
 * reset for states above 2), an id set by obj_set_table_id_flag7 goes back to the saved one, and
 * the sprite animation is refreshed with the one saved in slot 0 (or the default). Then the
 * override flags are cleared and the class callback runs with 1.
 */
void obj_apply_state_a(struct FieldObj *obj)
{
    u8 *state;
    u32 bits;
    u32 restored;
    u32 prevState;
    u32 slotFlags;
    s32 anim;
    s32 flip;
    u32 idOverridden;

    state = &obj->state214;
    bits = *state;
    if (bits & 0x40)
    {
        /* state changed: cur = prev */
        prevState = ((u32)(bits << 26)) >> 29;
        restored = (bits & -8) | prevState;
        *state = restored;
        if (((u32)(restored << 29) >> 29) > 2)
            obj->unk02 = obj->unk24 << 5;
        *(s8 *)state &= -0x41;
    }
    if (obj->state214 & 0x80)
    {
        if (obj->nextId21A != obj->id218)
        {
            obj->id218 = obj->nextId21A;
            obj_update_special_flag(obj);
        }
    }
    idOverridden = obj->state214 >> 7;
    if (obj->flags215 & 2)
        anim = obj->anim220;
    else
        anim = -1;
    slotFlags = obj->flags215;
    if (slotFlags & 2)
        flip = (slotFlags << 28) >> 31;
    else
        flip = -1;
    obj_refresh_anim_state(obj, idOverridden, anim, flip);
    obj->state214 &= 0x7F;
    *(s8 *)&obj->flags215 &= -3;
    obj->klass338->fn3C((u8 *)obj + obj->klass338->thisOffset38, 1);
}

/*
 * The slot-1 twin of obj_apply_state_a: undoes a temporary override. A pending state change goes
 * back to the previous state (facing reset for states above 2), an id overridden under flags215
 * bit 0 goes back to prevId21C, and the sprite animation is refreshed with the one saved in slot 1
 * (or the default). Then flags215 bit 0 and slot 1 are cleared; no class callback runs.
 */
void obj_apply_state_b(struct FieldObj *obj)
{
    u8 *state;
    u32 bits;
    u32 restored;
    u32 prevState;
    u32 idOverridden;
    u32 slotFlags;
    s32 anim;
    s32 flip;

    state = &obj->state214;
    bits = *state;
    if (bits & 0x40)
    {
        /* state changed: cur = prev */
        prevState = ((u32)(bits << 26)) >> 29;
        restored = (bits & -8) | prevState;
        *state = restored;
        if (((u32)(restored << 29) >> 29) > 2)
            obj->unk02 = obj->unk24 << 5;
        *(s8 *)state &= -0x41;
    }
    if (obj->flags215 & 1)
    {
        if (obj->prevId21C != obj->id218)
        {
            obj->id218 = obj->prevId21C;
            obj_update_special_flag(obj);
        }
    }
    idOverridden = obj->flags215 & 1;
    if (obj->flags215 & 4)
        anim = obj->anim221;
    else
        anim = -1;
    slotFlags = obj->flags215;
    if (slotFlags & 4)
        flip = ((u32)(slotFlags << 27)) >> 31;
    else
        flip = -1;
    obj_refresh_anim_state(obj, idOverridden, anim, flip);
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

void obj_queue_update(void *sprite);
void menu_entry_mark_1(void *table, u32 idx);
void menu_entry_mark_2(void *table, u32 idx);
void unlink_d54(void *ctl);
u32 sprite_alloc(void *ctx, u32 a, u32 b, u32 c, u32 d, u32 e);

struct LinkBits20F
{
    u8 bit0 : 1;
    u8 linked : 1;
    u8 hi6 : 6;
} __attribute__((packed));

struct SubBits12
{
    u8 bit0 : 1;
    u8 mode : 2;
    u8 hi5 : 5;
} __attribute__((packed));

static inline void obj_alloc_entry_inl(struct FieldObj *obj, u8 mode)
{
    if (obj->unk26C != 0xFFFF)
    {
        s32 shift = 0;
        s16 mask = obj->sub->mask14;

        while (((((u8)(mask >> shift)) ^ 1) & 1) != 0)
            shift++;
        obj->unk26E = sprite_alloc(obj->unk334, mode, obj->unk26C, (u8)(shift + 0x10), 0xFFFF, 0xFFFF);
        obj->flags216 |= 0x40;
    }
}

NONMATCH("asm/code_08045F18/obj_set_sprite_link.s", void obj_set_sprite_link(struct FieldObj *obj, u32 link))
{
    if (obj->unk222 == 0xFF)
        return;
    if (((struct LinkBits20F *)&obj->flags20F)->linked == link)
        return;
    ((struct LinkBits20F *)&obj->flags20F)->linked = link;
    if (link != 0)
    {
        u32 mode = ((struct SubBits12 *)&obj->sub->flags12)->mode;

        obj_queue_update(obj->sub);
        ((struct SubBits12 *)&obj->sub->flags12)->mode = mode;
        if (obj->flags20E & 0x40)
        {
            if (obj->unk26E != 0xFF)
                menu_entry_mark_1(obj->unk334, obj->unk26E);
            else
                obj_alloc_entry_inl(obj, 0xFF);
        }
    }
    else
    {
        if (obj->unk26E != 0xFF)
            menu_entry_mark_2(obj->unk334, obj->unk26E);
        obj->unk23D = link;
        unlink_d54(obj->sub);
    }
}
END_NONMATCH
