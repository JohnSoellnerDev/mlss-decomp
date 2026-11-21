#include "global.h"

struct FollowFrame
{
    u8 first;
    u8 unk1;
    u8 kind;
    u8 filler3[0x10 - 0x3];
    u8 slot10[2];
    u8 unk12;
    u8 filler13;
    u8 unk14;
    u8 unk15;
    u8 state16;
};

struct FollowSrc
{
    u8 filler0[8];
    u8 *tiles;
};

struct FollowAnim
{
    u8 filler0[0x10];
    struct FollowSrc *src10;
};

struct FollowObj
{
    u8 filler0[2];
    u8 dir2;
    s16 rate4;
    u8 filler6[0x0C - 0x6];
    s32 posX;
    s32 posY;
    s32 unk14;
    s32 unk18;
    struct FollowAnim *anim;
    u8 filler20[0x24 - 0x20];
    u8 unk24;
    u8 filler25[0x28 - 0x25];
    struct FollowObj *subs[4];
    u8 filler38[0x54 - 0x38];
    u8 bank54;
    u8 filler55[0xFB - 0x55];
    u8 flagsFB;
    u8 filler0FC[0x100 - 0xFC];
    u16 unk100;
    u8 filler102[0x12A - 0x102];
    u8 unk12A;
    u8 filler12B[0x200 - 0x12B];
    struct FollowObj *partA;
    struct FollowObj *partB;
    u8 unk208;
    u8 filler209;
    u16 unk20A;
    u8 filler20C[0x20E - 0x20C];
    u8 unk20E;
    u8 filler20F[0x214 - 0x20F];
    u8 unk214;
    u8 filler215[0x239 - 0x215];
    u8 unk239;
    u8 filler23A[0x23E - 0x23A];
    s16 speed;
    s16 unk240;
    s16 offX;
    s16 offY;
    u8 flags246;
    u8 filler247[0x260 - 0x247];
    u16 unk260;
    u8 filler262[0x264 - 0x262];
    s16 unk264;
    s16 unk266;
    u8 filler268[0x26C - 0x268];
    u8 unk26C;
    u8 filler26D[0x291 - 0x26D];
    u8 flags291;
    u8 filler292[0x294 - 0x292];
    struct FollowObj *ptr294;
    u8 filler298[0x2B5 - 0x298];
    u8 flags2B5;
    u8 filler2B6[0x2B8 - 0x2B6];
    u8 flags2B8;
    u8 filler2B9[0x33E - 0x2B9];
    s8 unk33E;
    u8 filler33F[0x351 - 0x33F];
    u8 flags351;
    u8 flags352;
    u8 flags353;
    u8 filler354[0x358 - 0x354];
    u16 unk358;
    u8 filler35A[0x35B - 0x35A];
    u8 flags35B;
    u8 filler35C[0x362 - 0x35C];
    u16 unk362;
    u16 unk364;
    u8 filler366[0x368 - 0x366];
    struct FollowFrame *frame;
    void *aux36C;
    u8 filler370[0x388 - 0x370];
    struct FollowObj *ptr388;
};

struct SysKeys78
{
    u8 filler0[0x78];
    u16 keyEnable : 10;
};

struct Bits246
{
    u8 filler0[0x246];
    u8 lo2 : 2;
    u8 mid3 : 3;
    u8 hi3 : 3;
} __attribute__((packed));

struct Low3Bits
{
    u8 low3 : 3;
    u8 rest : 5;
};

struct Bits351
{
    u8 filler0[0x351];
    u8 low6 : 6;
    u8 bit6 : 1;
    u8 bit7 : 1;
} __attribute__((packed));

struct Bits35B
{
    u8 filler0[0x35B];
    u8 low6 : 6;
    u8 bit6 : 1;
    u8 bit7 : 1;
} __attribute__((packed));

extern u8 gSys[];
extern u8 gRomTileOfsA[];
extern u8 gRomTileOfsB[];
extern u8 gRomFacingTable[];
extern s16 gDirVectors[];
extern u8 gNibbleDirTable[];
u32 arc_tan2(s32 x, s32 y);
void dma_copy_or_queue(void *src, void *dst, u32 len, u32 unit, u32 flags);
u32 obj_test_mode(struct FollowObj *obj, u32 mode);
void reset_slot_50(struct FollowObj *obj);
void advance_anim_frame(void *obj, void *sub, u32 a, u32 b);
void obj_state_tick(struct FollowObj *obj, struct FollowObj *sub);
void unlink_d54(void *ctl);
void pair_turn_and_sample(void *obj, u32 mode);
void update_pair_facing(struct FollowObj *obj, u8 reset, u8 enable);

ASM_FUNC("asm/code_0802BD40/obj_follow_step.s", void obj_follow_step(void))

/*
 * Points obj's lead part (partA) in direction dir (0-255, 8 sectors) and sets its move
 * offsets: the sector's unit vector from gDirVectors times its speed (8.8 fixed point),
 * halved per step of the slow-down level in bits 2-3 of flags353, rounded away from 0
 * to a whole multiple of 0x40.
 */
void set_dir_and_offsets(struct FollowObj *obj, u8 dir)
{
    s16 *vec;
    s32 dx;
    s32 dy;
    u32 slowShift;

    obj->partA->dir2 = dir;
    vec = &gDirVectors[((dir >> 5) & 7) << 1];
    dx = (obj->partA->speed * vec[0]) / 0x100;
    slowShift = ((u32)obj->partA->flags353 << 26) >> 30;
    dx >>= slowShift;
    dy = (obj->partA->speed * vec[1]) / 0x100;
    dy >>= slowShift;
    if (dx > 0)
        dx = (dx + 0x3F) & -0x40;
    else if (dx < 0)
    {
        dx = -dx;
        dx = (dx + 0x3F) & -0x40;
        dx = -dx;
    }
    if (dy > 0)
        dy = (dy + 0x3F) & -0x40;
    else if (dy < 0)
    {
        dy = -dy;
        dy = (dy + 0x3F) & -0x40;
        dy = -dy;
    }
    obj->partA->offX = dx;
    obj->partA->offY = dy;
}

/* Steers obj's lead part (partA) from the held keys: a d-pad direction sets its move offsets.
 * With no direction held, a lead in rate 4 slides on with its saved offsets (unk362/unk364),
 * which shrink by 8 towards 0 each frame along with its speed (copied to partB); otherwise
 * it stops. */
void follow_obj_move_step(struct FollowObj *obj, u16 *keys)
{
    struct FollowObj *lead;
    u32 zero;
    u32 dpad;
    s32 dir;
    u16 old;

    if ((obj->flags2B8 & 0x10) != 0)
        return;
    lead = obj->partA;
    if ((lead->flags2B5 & 7) != 0)
        return;
    /* zero: bit 6 of flags351, which is clear inside this block */
    if (obj->unk12A == 2 && (zero = lead->flags351 & 0x40) == 0 && keys != 0)
    {
        dpad = (*keys & 0xF0) >> 4;
        dir = gNibbleDirTable[dpad];
        if (dir != 0xFF)
        {
            set_dir_and_offsets(obj, dir);
            return;
        }
        if (lead->rate4 == 4)
        {
            lead->offX = lead->unk362;
            obj->partA->offY = obj->partA->unk364;
            {
                s32 offX;

                old = obj->partA->offX;
                offX = obj->partA->offX;
                if (offX > 0)
                {
                    obj->partA->offX = old - 8;
                    if (obj->partA->offX < 0)
                        obj->partA->offX = zero;
                }
                else if (offX < 0)
                {
                    obj->partA->offX = old + 8;
                    if (obj->partA->offX > 0)
                        obj->partA->offX = zero;
                }
            }
            old = obj->partA->offY;
            dir = obj->partA->offY;
            if (dir > 0)
            {
                obj->partA->offY = old - 8;
                if (obj->partA->offY < 0)
                    obj->partA->offY = 0;
            }
            else if (dir < 0)
            {
                obj->partA->offY = old + 8;
                if (obj->partA->offY > 0)
                    obj->partA->offY = 0;
            }
            obj->partA->speed = (u16)obj->partA->speed - 8;
            if (obj->partA->speed < 0)
                obj->partA->speed = 0;
            obj->partB->speed = obj->partA->speed;
        }
        else
        {
            lead->offX = zero;
            obj->partA->offY = zero;
        }
        return;
    }
    obj->partA->offX = 0;
    obj->partA->offY = 0;
}

ASM_FUNC("asm/code_0802BD40/anchor_frame_update.s", void anchor_frame_update(void))

/* Slides obj's Y offset by 8 per frame: towards -0x38 while A, B, Left and Right are all disabled
 * in gSys (or unk20A bits 7-8 force it with 0x100), back towards 0 while any of them is enabled
 * (or forced with 0x80). */
void step_offset_244(struct FollowObj *obj)
{
    u32 force;
    u32 raw;
    s32 offY;
    u32 raw2;
    s32 offY2;

    force = obj->unk20A & 0x180;
    if (force == 0x100
        || (force == 0 && (((struct SysKeys78 *)gSys)->keyEnable & (A_BUTTON | B_BUTTON | DPAD_RIGHT | DPAD_LEFT)) == 0))
    {
        raw = (u16)obj->offY;
        offY = obj->offY;
        if (offY > -0x38)
            obj->offY = raw - 8;
    }
    else
    {
        force = obj->unk20A & 0x180;
        if (force == 0x80
            || (force == 0 && (((struct SysKeys78 *)gSys)->keyEnable & (A_BUTTON | B_BUTTON | DPAD_RIGHT | DPAD_LEFT)) != 0))
        {
            raw2 = (u16)obj->offY;
            offY2 = obj->offY;
            if (offY2 < 0)
                obj->offY = raw2 + 8;
        }
    }
}

/* Stores a frame count for part `idx` of obj (obj->subs[idx]) in its frame's slot10 (the slot of
 * the other bro: index !(flags352 bit 7)), one less than `count` if it is above 0 and that part is
 * bro 1 or the pair doesn't move together (unk208 bit 4), and one less anyway in action 0x0A.
 * Then queues a DMA of that frame kind's tiles (gRomTileOfsB[kind] tiles from tile
 * gRomTileOfsA[kind] of obj's animation) to OBJ VRAM 0x06010DC0 + the part's bank54 * 0x400. */
void obj_anim_tick_queue_tiles(struct FollowObj *obj, u32 idx, s32 count)
{
    struct FollowObj *part = obj->subs[idx];
    struct FollowFrame *frame = part->frame;
    const u8 *tileStart;
    u32 kind;
    u32 firstTile;
    struct FollowSrc *src;

    if ((part->flags352 & 0x80) != 0)
    {
        if (count > 0)
            count--;
    }
    else
    {
        if ((obj->unk208 & 0x10) == 0 && count > 0)
            count--;
        else if (frame->first == 0x0A)
            count--;
    }
    frame->slot10[((part->flags352 >> 7) ^ 1)] = count;
    src = obj->anim->src10;
    tileStart = gRomTileOfsA;
    kind = part->frame->kind;
    firstTile = tileStart[kind];
    /* 0x20 bytes per 4bpp tile */
    dma_copy_or_queue(src->tiles + (firstTile << 5), (void *)(0x06010DC0 + (part->bank54 << 10)),
        gRomTileOfsB[kind] << 5, 0x20, 0);
}

/* Stops the bros pair: clears the lead's (partA) move offsets when stopLead is set, and the
 * follower's (partB) when stopFollower is set. When the pair moves together (unk208 bit 4)
 * the follower is then turned to face the lead (one of 8 directions, via gRomFacingTable);
 * a follower in a mode above 2 (unk214 bits 0-2) gets that direction in unk24 too. */
void update_pair_facing(struct FollowObj *obj, u8 stopLead, u8 stopFollower)
{
    struct FollowObj *follower = obj->partB;
    struct FollowObj *lead = obj->partA;
    s32 dx = 0; /* also the 0 the offsets are cleared with */
    s32 dy;
    u32 angle;
    u8 dir;

    if (stopLead != 0)
    {
        lead->offX = dx;
        lead->offY = dx;
    }
    if (follower != NULL && stopFollower != 0)
    {
        follower->offX = dx;
        follower->offY = dx;
        if (obj->unk208 & 0x10)
        {
            dx = lead->posX - follower->posX;
            dy = lead->posY - follower->posY;
            if (dx != 0 || dy != 0)
            {
                angle = arc_tan2((s16)-dy, (s16)dx);
                /* the angle's top 4 bits (16 sectors) map to a direction 0-7 */
                dir = gRomFacingTable[(angle << 16) >> 28];
                follower->dir2 = (dir & 7) << 5;
                if (((u32)(follower->unk214 << 29) >> 29) > 2)
                    ((struct Low3Bits *)&follower->unk24)->low3 = dir;
            }
        }
    }
}

ASM_FUNC("asm/code_0802BD40/obj_state_tick.s", void obj_state_tick(struct FollowObj *obj, struct FollowObj *sub))

ASM_FUNC("asm/code_0802BD40/update_sub_state.s", void update_sub_state(void))

/* Puts one part of the pair (sub) back to its idle state: unlinks its aux control (aux36C), clears
 * its animation state and flags, ends its partner's (ptr388) action, restores its speed from the
 * owner's base speed (obj->unk100) and restarts its animation. When the pair moves together
 * (obj->unk208 bit 4) the partner is reset the same way and pair_turn_and_sample runs. */
void reset_sub_object(struct FollowObj *obj, struct FollowObj *sub)
{
    u16 baseSpeed;

    unlink_d54(sub->aux36C);
    ((u8 *)obj)[0x20B] &= 0x7F; /* unk20A bit 15 */
    ((u8 *)obj)[0x208] |= 1;
    ((struct Bits246 *)obj)->mid3 = 0;
    ((u8 *)sub)[0x351] &= 0x7F;
    sub->frame->first = 0;
    sub->rate4 = 0;
    ((struct Bits351 *)sub)->bit6 = 0;
    /* partner in action 1 or 2: clear its sub-step, else its action */
    if ((u8)(sub->ptr388->frame->first - 1) <= 1)
        sub->ptr388->frame->unk1 = 0;
    else
        sub->ptr388->frame->first = 0;
    ((struct Bits35B *)sub)->bit6 = 0;
    baseSpeed = obj->unk100;
    sub->speed = baseSpeed;
    sub->unk260 = baseSpeed;
    sub->frame->slot10[0] = sub->frame->unk12;
    sub->frame->unk12 = 0xFF;
    advance_anim_frame(obj, sub, 0, 0);
    if ((obj->unk208 & 0x10) == 0 && (sub->flags352 & 0x80) == 0)
    {
        sub->frame->unk14 = sub->frame->slot10[0];
        sub->frame->slot10[0] = 0;
        advance_anim_frame(obj, sub, 0, 0);
    }
    ((struct Bits351 *)sub->ptr388)->bit6 = 0;
    if ((obj->unk208 & 0x10) != 0)
    {
        struct FollowObj *partner;

        sub->ptr388->rate4 = 0;
        advance_anim_frame(obj, sub->ptr388, sub->flags352 >> 7, 0);
        partner = sub->ptr388;
        partner->speed = obj->unk100;
        partner->unk260 = obj->unk100;
        pair_turn_and_sample(obj, 0);
    }
}

/* Per-frame step of the bros pair: unless obj or its lead bro (partA) is blocked or animFlag is 0,
 * when the partner (partB) is nearly still and the lead too (rate4 <= 1, or <= 4 when flagsFB bit 2
 * is set and its frame is of kind 0) and the lead's action button was pressed (obj_test_mode 0),
 * runs the lead's action (obj_state_tick), or clears slot 0x50 when its frame's state16 bit 7 is
 * set. Always ends by advancing the lead's animation frame. */
void obj_step_if_ready(struct FollowObj *obj, u32 animFlag)
{
    struct FollowObj *lead;
    struct FollowObj *partner;
    struct FollowObj *leadAgain;
    struct FollowFrame *frame;
    u32 pressed;

    if ((obj->flags2B8 & 0x10) != 0)
        goto advance;
    if ((obj->flags291 & 1) != 0)
        goto advance;
    lead = obj->partA;
    if ((lead->flags2B5 & 8) != 0)
        goto advance;
    if (animFlag == 0)
        goto advance;
    partner = obj->partB;
    if (partner->rate4 > 4)
        goto advance;
    if ((obj->flagsFB & 4) == 0 || lead->frame->kind != 0 || lead->rate4 > 4)
    {
        leadAgain = obj->partA;
        if (leadAgain->rate4 > 1)
            goto advance;
    }
    pressed = obj_test_mode(obj, 0);
    if (((u32)pressed << 24) == 0)
        goto advance;
    lead = obj->partA;
    frame = lead->frame;
    if ((frame->state16 & 0x80) == 0)
    {
        obj_state_tick(obj, lead);
        goto advance;
    }
    reset_slot_50(obj);
advance:
    advance_anim_frame(obj, obj->partA, 0, animFlag);
}
