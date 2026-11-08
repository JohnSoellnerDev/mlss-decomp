#include "global.h"

ASM_FUNC("asm/code_0802A4FC/sync_mode_353.s", void sync_mode_353(void))

ASM_FUNC("asm/code_0802A4FC/update_sub_objects.s", void update_sub_objects(void))

ASM_FUNC("asm/code_0802A4FC/pair_frame_update.s", void pair_frame_update(void))

ASM_FUNC("asm/code_0802A4FC/update_sub_frame.s", void update_sub_frame(void))

extern const u8 gDirMask[];
u8 obj_push_out(void *obj, void *entry);
void obj_set_slot_value(void *obj, s32 value);
void obj_shift_state_slots(void *sub);
void advance_anim_frame(void *obj, void *sub, u32 a, u32 b);

struct CtLink
{
    s8 idx : 7;
    u8 contact : 1;
} __attribute__((packed));

struct CtKind
{
    u8 b0 : 1;
    u8 kind : 3;
    u8 hi : 4;
} __attribute__((packed));

struct CtLo4
{
    u8 lo4 : 4;
    u8 hi4 : 4;
} __attribute__((packed));

struct CtDir
{
    u8 lo5 : 5;
    u8 dir : 3;
} __attribute__((packed));

struct CtMid
{
    u8 lo2 : 2;
    u8 mid : 3;
    u8 hi : 3;
} __attribute__((packed));

struct CtBit4
{
    u8 lo4 : 4;
    u8 bit4 : 1;
    u8 hi : 3;
} __attribute__((packed));

struct CtObj
{
    u8 filler0[2];
    u8 unk2;
    u8 filler3[0xC - 3];
    s32 x;
    s32 y;
    s32 z;
    s32 zStep;
    u8 filler1C[0x40 - 0x1C];
    s32 zVel;
    u8 filler44[0x54 - 0x44];
    u8 group;
    u8 filler55[0x20C - 0x55];
    u8 flags20C;
    u8 flags20D;
    u8 type20E;
    u8 flags20F;
    u8 filler210[0x216 - 0x210];
    u8 flags216;
    u8 filler217[0x270 - 0x217];
    s16 offsetY;
    u16 width;
    u8 filler274[2];
    u16 height;
    u8 filler278[0x284 - 0x278];
    s32 boxX;
    s32 boxY;
    s32 boxZ;
    u8 filler290[0x33C - 0x290];
    u8 link;
    u8 filler33D[3];
    u8 state;
    u8 prevState;
    u8 filler342[2];
    struct CtObj *partner;
    u8 filler348[0x350 - 0x348];
    u8 mask350;
    u8 filler351;
    u8 mode352;
    u8 filler353[0x358 - 0x353];
    u8 flags358;
    u8 filler359[0x35B - 0x359];
    u8 flags35B;
};

struct CtPair
{
    u8 filler0[0x28];
    struct CtObj *entries[0x39];
    u8 filler10C[2];
    u8 pStart;
    u8 pCount;
    u8 aStart;
    u8 aCount;
    u8 cStart;
    u8 cCount;
    u8 filler114[0x11A - 0x114];
    u8 dStart;
    u8 dCount;
    u8 filler11C[0x12A - 0x11C];
    u8 action;
    u8 filler12B[0x208 - 0x12B];
    u8 flags208;
    u8 flags209;
    u8 filler20A[0x246 - 0x20A];
    u8 state246;
};

void obj_update_contacts(struct CtPair *ctx, struct CtObj *obj, u8 flag)
{
    struct CtObj **slot;
    struct CtObj *entry;
    struct CtObj *partner;
    struct CtObj *other;
    u32 left;
    s32 start;
    s32 objX, objY, otherX, otherY, half;
    u32 type, mode, state;
    s32 hit;

    if (obj == NULL)
        goto part2;
    if (((struct CtLink *)&obj->link)->idx != -1)
        goto part2;
    if (!(obj->flags20C & 1))
        goto part2;
    if (obj->flags216 & 0x20)
        goto part2;
    if (!(obj->flags20F & 1))
        goto part2;

    obj->boxX = obj->x - (obj->width >> 1);
    obj->boxY = obj->y + obj->offsetY;
    obj->boxZ = (obj->zStep > 0 || (obj->zStep < 0 && obj->zVel < 0)) ? obj->z + obj->zStep : obj->z;

    if (!(ctx->flags208 & 0x10) && (obj->flags20C & 0x40))
        obj_push_out(obj, ctx->entries[obj->group ^ 1]);

    if (!(obj->flags20D & 1))
        goto part2;
    if (obj->flags35B & 0x10)
        goto part2;

    // list C: an entry or the partner of an entry pushes the object
    if (((struct CtLink *)&obj->link)->idx == -1 && ctx->cStart != 0)
    {
        slot = &ctx->entries[ctx->cStart];
        for (left = ctx->cCount; left != 0; left--, slot++)
        {
            entry = *slot;
            if (obj_push_out(obj, entry))
            {
                ((struct CtLink *)&obj->link)->contact = 0;
                break;
            }
            partner = entry->partner;
            if (partner != NULL && (partner->flags20C & 1) && obj_push_out(obj, partner))
            {
                ((struct CtLink *)&obj->link)->contact = 1;
                break;
            }
        }
    }

    if (flag != 0)
        goto part2;
    if (((struct CtLink *)&obj->link)->idx != -1)
        goto part2;

    // the entity list the scan starts with
    if ((!(ctx->flags208 & 0x10) || (obj->mode352 & 0x80)
         || ((struct CtMid *)&ctx->state246)->mid == 3 || ((struct CtMid *)&ctx->state246)->mid == 6)
        && ctx->pStart != 0)
        start = ctx->pStart;
    else if (ctx->aStart != 0)
        start = ctx->aStart;
    else
        start = 0;

    if (start > 0)
    {
        slot = &ctx->entries[start];
        for (left = ctx->pCount + ctx->aCount - start + 2; left != 0; left--, slot++)
        {
            if (obj_push_out(obj, *slot) && !(obj->flags358 & 8))
            {
                other = ctx->entries[((struct CtLink *)&obj->link)->idx];
                type = other->type20E & 0x38;
                if ((type == 8 && (obj->mode352 & 1)) || (type == 0x10 && (obj->mode352 & 2)))
                {
                    if ((other->type20E & 0x38) == 8 && ((struct CtKind *)&other->flags20C)->kind > 1
                        && obj->zStep == 0)
                    {
                        objX = obj->x;
                        objY = obj->y + obj->offsetY;
                        otherX = other->x;
                        otherY = other->y + other->offsetY;
                        half = (obj->width + other->width) >> 1;
                        hit = 0;
                        if (objX - half < otherX && objX + half > otherX
                            && objY - obj->height < otherY && objY + other->height > otherY)
                        {
                            hit = 1;
                        }
                        else
                        {
                            if (gDirMask[((struct CtDir *)&obj->unk2)->dir] & (mode = ((struct CtLo4 *)&obj->mask350)->lo4))
                            {
                                switch (mode)
                                {
                                case 1:
                                case 4:
                                    if (objX + (obj->width >> 1) > otherX - (other->width >> 1)
                                        && objX - (obj->width >> 1) < otherX + (other->width >> 1))
                                        hit = 1;
                                    break;
                                case 2:
                                case 8:
                                    if (objY > otherY - other->height && objY - obj->height < otherY)
                                        hit = 1;
                                    break;
                                }
                            }
                        }
                        if (hit)
                            obj->state = ((struct CtKind *)&other->flags20C)->kind;
                    }
                    break;
                }
            }
        }
    }

    // list D: pushed out of the entries until one reports a push
    if (((struct CtLink *)&obj->link)->idx == -1 && ctx->dStart != 0)
    {
        struct CtObj **dSlot = &ctx->entries[ctx->dStart];
        u32 dLeft;

        for (dLeft = ctx->dCount; dLeft != 0; dLeft--, dSlot++)
        {
            if (obj_push_out(obj, *dSlot))
                break;
        }
    }

part2:
    if (!(ctx->flags209 & 1) && ctx->action != 0 && ctx->action <= 2
        && ((ctx->flags208 & 0x10) || (obj->mode352 & 0x80)))
    {
        state = obj->state;
        if (state == 2)
        {
            obj_set_slot_value(obj, 1);
            advance_anim_frame(ctx, obj, (obj->mode352 & 0x80) ? 0 : ((struct CtBit4 *)&ctx->flags208)->bit4, 0);
        }
        else if ((obj->mode352 & 0x80) && state == 3)
        {
            obj_set_slot_value(obj, 2);
            advance_anim_frame(ctx, obj, (obj->mode352 & 0x80) ? 0 : ((struct CtBit4 *)&ctx->flags208)->bit4, 0);
        }
        else
        {
            if (obj->prevState == obj->state)
                return;
            obj_shift_state_slots(obj);
            advance_anim_frame(ctx, obj, (obj->mode352 & 0x80) ? 0 : ((struct CtBit4 *)&ctx->flags208)->bit4, 0);
        }
        return;
    }
    obj_shift_state_slots(obj);
}
