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

void set_dir_and_offsets(struct FollowObj *a, u8 dir)
{
    s16 *tab;
    s32 x;
    s32 y;
    u32 shift;

    a->partA->dir2 = dir;
    tab = &gDirVectors[((dir >> 5) & 7) << 1];
    x = (a->partA->speed * tab[0]) / 0x100;
    shift = ((u32)a->partA->flags353 << 26) >> 30;
    x >>= shift;
    y = (a->partA->speed * tab[1]) / 0x100;
    y >>= shift;
    if (x > 0)
        x = (x + 0x3F) & -0x40;
    else if (x < 0)
    {
        x = -x;
        x = (x + 0x3F) & -0x40;
        x = -x;
    }
    if (y > 0)
        y = (y + 0x3F) & -0x40;
    else if (y < 0)
    {
        y = -y;
        y = (y + 0x3F) & -0x40;
        y = -y;
    }
    a->partA->offX = x;
    a->partA->offY = y;
}

NONMATCH("asm/code_0802BD40/follow_obj_move_step.s", void follow_obj_move_step(struct FollowObj *obj, u16 *p))
{
    struct FollowObj *b;
    u32 bit6;
    u32 idx;
    u8 t;

    if ((obj->flags2B8 & 0x10) != 0)
        return;
    b = obj->partA;
    if ((b->flags2B5 & 7) != 0)
        return;
    if (obj->unk12A != 2)
        goto clear;
    bit6 = b->flags351 & 0x40;
    if (bit6 != 0)
        goto clear;
    if (p == 0)
        goto clear;
    idx = (*p & 0xF0) >> 4;
    t = gNibbleDirTable[idx];
    if (t != 0xFF)
    {
        set_dir_and_offsets(obj, t);
        return;
    }
    if (b->rate4 == 4)
    {
        b->offX = b->unk362;
    obj->partA->offY = obj->partA->unk364;
    {
        u16 u242 = obj->partA->offX;
        s32 x2 = obj->partA->offX;

        if (x2 > 0)
        {
            obj->partA->offX = u242 - 8;
            if (obj->partA->offX < 0)
                obj->partA->offX = bit6;
        }
        else
        {
            if (x2 < 0)
            {
                obj->partA->offX = u242 + 8;
                if (obj->partA->offX > 0)
                    obj->partA->offX = bit6;
            }
        }
    }
    {
        u16 u244 = obj->partA->offY;
        s32 y = obj->partA->offY;

        if (y > 0)
        {
            obj->partA->offY = u244 - 8;
            if (obj->partA->offY < 0)
                obj->partA->offY = 0;
        }
        else
        {
            if (y < 0)
            {
                obj->partA->offY = u244 + 8;
                if (obj->partA->offY > 0)
                    obj->partA->offY = 0;
            }
        }
    }
    obj->partA->speed = (u16)obj->partA->speed - 8;
    if (obj->partA->speed < 0)
        obj->partA->speed = 0;
    obj->partB->speed = obj->partA->speed;
        return;
    }
    b->offX = bit6;
    obj->partA->offY = bit6;
    return;
clear:
    obj->partA->offX = 0;
    obj->partA->offY = 0;
}
END_NONMATCH

ASM_FUNC("asm/code_0802BD40/anchor_frame_update.s", void anchor_frame_update(void))

void step_offset_244(struct FollowObj *obj)
{
    u32 k;
    u32 cur;
    s32 v;
    u32 cur2;
    s32 v2;

    k = obj->unk20A & 0x180;
    if (k == 0x100 || (k == 0 && (((struct SysKeys78 *)gSys)->keyEnable & 0x33) == 0))
    {
        cur = (u16)obj->offY;
        v = obj->offY;
        if (v > -0x38)
            obj->offY = cur - 8;
    }
    else
    {
        k = obj->unk20A & 0x180;
        if (k == 0x80 || (k == 0 && (((struct SysKeys78 *)gSys)->keyEnable & 0x33) != 0))
        {
            cur2 = (u16)obj->offY;
            v2 = obj->offY;
            if (v2 < 0)
                obj->offY = cur2 + 8;
        }
    }
}

void obj_anim_tick_queue_tiles(struct FollowObj *p, u32 idx, s32 v)
{
    struct FollowObj *obj = p->subs[idx];
    struct FollowFrame *frame = obj->frame;
    const u8 *tableA;
    u32 kind;
    u32 base;
    struct FollowSrc *src;

    if ((obj->flags352 & 0x80) != 0)
    {
        if (v > 0)
            v--;
    }
    else
    {
        if ((p->unk208 & 0x10) == 0 && v > 0)
            v--;
        else if (frame->first == 0x0A)
            v--;
    }
    frame->slot10[((obj->flags352 >> 7) ^ 1)] = v;
    src = p->anim->src10;
    tableA = gRomTileOfsA;
    kind = obj->frame->kind;
    base = tableA[kind];
    dma_copy_or_queue(src->tiles + (base << 5), (void *)(0x06010DC0 + (obj->bank54 << 10)),
        gRomTileOfsB[kind] << 5, 0x20, 0);
}

NONMATCH("asm/code_0802BD40/update_pair_facing.s", void update_pair_facing(struct FollowObj *obj, u8 reset, u8 enable))
{
    struct FollowObj *me;
    struct FollowObj *other;
    s32 dx;
    s32 dy;
    u32 ang;
    u8 t;
    u8 tb;
    u32 off;
    u8 zero;

    other = obj->partB;
    me = obj->partA;
    zero = 0;
    if (reset != 0)
    {
        off = 0x242;
        *(s16 *)((u8 *)me + off) = zero;
        off += 2;
        *(s16 *)((u8 *)me + off) = zero;
    }
    if (other != 0 && enable != 0)
    {
        off = 0x242;
        *(s16 *)((u8 *)other + off) = zero;
        off += 2;
        *(s16 *)((u8 *)other + off) = zero;
        if ((obj->unk208 & 0x10) != 0)
        {
            dx = me->posX - other->posX;
            dy = me->posY - other->posY;
            if (dx != 0 || dy != 0)
            {
                ang = arc_tan2((s16)(-dy), (s16)dx);
                tb = gRomFacingTable[(ang << 16) >> 28];
                t = tb & 7;
                other->dir2 = t << 5;
                if ((other->unk214 & 7) > 2)
                {
                    s32 low = other->unk24;
                    other->unk24 = (low & -8) | t;
                }
            }
        }
    }
}
END_NONMATCH

ASM_FUNC("asm/code_0802BD40/obj_state_tick.s", void obj_state_tick(struct FollowObj *obj, struct FollowObj *sub))

ASM_FUNC("asm/code_0802BD40/update_sub_state.s", void update_sub_state(void))

void reset_sub_object(struct FollowObj *obj, struct FollowObj *sub)
{
    u16 h;

    unlink_d54(sub->aux36C);
    ((u8 *)obj)[0x20B] &= 0x7F;
    ((u8 *)obj)[0x208] |= 1;
    ((struct Bits246 *)obj)->mid3 = 0;
    ((u8 *)sub)[0x351] &= 0x7F;
    sub->frame->first = 0;
    sub->rate4 = 0;
    ((struct Bits351 *)sub)->bit6 = 0;
    if ((u8)(sub->ptr388->frame->first - 1) <= 1)
        sub->ptr388->frame->unk1 = 0;
    else
        sub->ptr388->frame->first = 0;
    ((struct Bits35B *)sub)->bit6 = 0;
    h = obj->unk100;
    sub->speed = h;
    sub->unk260 = h;
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
        struct FollowObj *p;

        sub->ptr388->rate4 = 0;
        advance_anim_frame(obj, sub->ptr388, sub->flags352 >> 7, 0);
        p = sub->ptr388;
        p->speed = obj->unk100;
        p->unk260 = obj->unk100;
        pair_turn_and_sample(obj, 0);
    }
}

void obj_step_if_ready(struct FollowObj *obj, u32 flag)
{
    struct FollowObj *a;
    struct FollowObj *b;
    struct FollowObj *a2;
    struct FollowFrame *t;
    u32 test;

    if ((obj->flags2B8 & 0x10) != 0)
        goto finish;
    if ((obj->flags291 & 1) != 0)
        goto finish;
    a = obj->partA;
    if ((a->flags2B5 & 8) != 0)
        goto finish;
    if (flag == 0)
        goto finish;
    b = obj->partB;
    if (b->rate4 > 4)
        goto finish;
    if ((obj->flagsFB & 4) == 0 || a->frame->kind != 0 || a->rate4 > 4)
    {
        a2 = obj->partA;
        if (a2->rate4 > 1)
            goto finish;
    }
    test = obj_test_mode(obj, 0);
    if (((u32)test << 24) == 0)
        goto finish;
    a = obj->partA;
    t = a->frame;
    if ((t->state16 & 0x80) == 0)
    {
        obj_state_tick(obj, a);
        goto finish;
    }
    reset_slot_50(obj);
finish:
    advance_anim_frame(obj, obj->partA, 0, flag);
}
