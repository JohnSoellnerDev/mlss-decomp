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
    u8 filler15;
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
    u8 filler14[0x1C - 0x14];
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
    u8 filler102[0x200 - 0x102];
    struct FollowObj *partA;
    struct FollowObj *partB;
    u8 unk208;
    u8 filler209;
    u16 unk20A;
    u8 filler20C[0x214 - 0x20C];
    u8 unk214;
    u8 filler215[0x23E - 0x215];
    s16 speed;
    u8 filler240[2];
    s16 offX;
    s16 offY;
    u8 flags246;
    u8 filler247[0x260 - 0x247];
    u16 unk260;
    u8 filler262[0x291 - 0x262];
    u8 flags291;
    u8 filler292[0x2B5 - 0x292];
    u8 flags2B5;
    u8 filler2B6[0x2B8 - 0x2B6];
    u8 flags2B8;
    u8 filler2B9[0x351 - 0x2B9];
    u8 flags351;
    u8 flags352;
    u8 flags353;
    u8 filler354[0x35B - 0x354];
    u8 flags35B;
    u8 filler35C[0x368 - 0x35C];
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

extern u8 gSys[];
extern u8 gRomTileOfsA[];
extern u8 gRomTileOfsB[];
void dma_copy_or_queue(void *src, void *dst, u32 len, u32 unit, u32 flags);
u32 obj_test_mode(struct FollowObj *obj, u32 mode);
void reset_slot_50(struct FollowObj *obj);
void advance_anim_frame(void *obj, void *sub, u32 a, u32 b);
void obj_state_tick(struct FollowObj *obj, struct FollowObj *sub);

ASM_FUNC("asm/code_0802BD40/obj_follow_step.s", void obj_follow_step(void))

ASM_FUNC("asm/code_0802BD40/set_dir_and_offsets.s", void set_dir_and_offsets(void))

ASM_FUNC("asm/code_0802BD40/follow_obj_move_step.s", void follow_obj_move_step(void))

ASM_FUNC("asm/code_0802BD40/anchor_frame_update.s", void anchor_frame_update(void))

NONMATCH("asm/code_0802BD40/step_offset_244.s", void step_offset_244(struct FollowObj *obj))
{
    u32 k;
    u32 cur;
    s32 v;

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
            cur = (u16)obj->offY;
            v = obj->offY;
            if (v < 0)
                obj->offY = cur + 8;
        }
    }
}
END_NONMATCH

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

ASM_FUNC("asm/code_0802BD40/update_pair_facing.s", void update_pair_facing(void))

ASM_FUNC("asm/code_0802BD40/obj_state_tick.s", void obj_state_tick(struct FollowObj *obj, struct FollowObj *sub))

ASM_FUNC("asm/code_0802BD40/update_sub_state.s", void update_sub_state(void))

ASM_FUNC("asm/code_0802BD40/reset_sub_object.s", void reset_sub_object(void))

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
