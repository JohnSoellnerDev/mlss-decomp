#include "global.h"
#include "ram_routines.h"

struct HeroSprite
{
    u16 unk0;
    u16 unk2;
    u8 filler4[0x0E - 0x04];
    u16 unkE;
    u8 filler10[0x11 - 0x10];
    u8 mirrorFlags;
    u8 flags;
    u8 filler13[0x20 - 0x13];
    u8 frame;
    u8 kind;
};

struct HeroSpriteBits
{
    u8 unk0 : 1;
    u8 mode : 2;
    u8 unk3 : 5;
};

struct HeroData
{
    u8 filler0[0xD8];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 fillerE4[0xEC - 0xE4];
    s16 pose;
    u8 fillerEE[0x114 - 0xEE];
    u32 poseValue;
    u8 filler118[0x11E - 0x118];
    u16 flags;
};

struct HeroBody
{
    u8 filler0[0x08];
    struct HeroSprite *sprite;
    u8 filler0C[0x10 - 0x0C];
    s32 posX;
    s32 posY;
    s32 posZ;
    u8 filler1C[0x24 - 0x1C];
    u16 count;
    u8 filler26[0x28 - 0x26];
    struct HeroData *data;
    struct HeroBody *partner;
    struct HeroBody *next;
    u8 filler34[0x38 - 0x34];
    s32 targetX;
    s32 targetY;
    s32 targetZ;
    u8 filler44[0x47 - 0x44];
    s8 unk47;
    u8 filler48[0x4C - 0x48];
    void (*state)(struct HeroBody *);
    u32 unk50;
    u8 filler54[0x75 - 0x54];
    u8 order;
    u8 mode;
    u8 filler77[0x79 - 0x77];
    u8 flags;
    u16 rate;
    u16 frame;
    s16 progress;
    u8 filler80[0x84 - 0x80];
    s16 spanA;
    s16 spanB;
    u8 filler88[0x92 - 0x88];
    s16 height;
    s32 ratioLo;
    s32 ratioHi;
    u8 filler9C[0xAC - 0x9C];
    s16 step;
    u16 timer;
    u8 filler_B0[0xC3 - 0xB0];
    s8 ofsX;
    s8 ofsY;
    s8 ofsZ;
    u8 filler_C6[0xCA - 0xC6];
    u16 scale;
    u8 cc;
};

struct Hero
{
    u8 filler0[0x08];
    struct HeroBody body;
};

struct HeroMirrorBits
{
    u8 unk0 : 6;
    u8 mirror : 1;
    u8 unk7 : 1;
};

struct World
{
    u8 filler0[0x70];
    struct Hero *heroA;
    struct Hero *heroB;
};

extern struct World *gWorld;

void set_slot_value(u32 id, s32 value);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void set_motion_offsets(struct HeroBody *body, u32 offsets);
void set_motion_start(struct HeroBody *body, u32 speed);
void enter_finish_rebuild(struct HeroBody *body);
void step_motion(struct HeroBody *body);
void effect_spawn_at(u32 type, s32 x, s32 y, s32 h, struct HeroBody *owner);

void hero_b_step_threshold_b05dc(struct HeroBody *body);
void hero_b_wait_ratio_then_fx(struct HeroBody *body);
void pair_end_place_both_a(struct HeroBody *body);
void hero_b_step_reach_13a4(struct HeroBody *body);

void hero_b_to_partner_fast(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct Hero *hero_a = world->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct Hero *hero_b = world->heroB;
    struct HeroBody *body_b = &hero_b->body;
    struct HeroBody *partner = body->partner;
    s32 x;
    s32 y;
    s32 z;

    if ((body_a->sprite->flags & 8) != 0)
    {
        set_slot_value(0xA7, -1);
        if (body_a->next->sprite->kind != 3)
            sprite_set_action(body_a->next, 3, 0, 0);
        body_b->order = body_a->order;
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            sprite_set_action(body_a, 5, 0x2032, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            sprite_set_action(body_b, 5, 0x2061, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
        }
        body_b->posX = body_a->targetX + 0xA00;
        body_b->targetX = body_b->posX;
        body_b->posY = body_a->targetY;
        body_b->targetY = body_b->posY;
        body_b->posZ = body_a->targetZ + 0x600;
        body_b->targetZ = body_b->posZ;
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            x = partner->targetX / 0x100 + partner->ofsX - 0x18;
            y = partner->targetY / 0x100 + partner->ofsY;
            z = partner->targetZ / 0x100 + partner->ofsZ - 0xC;
            place_targets(body_b, x, y, z, -1);
            set_motion_start(body_b, 0x600);
        }
        body_b->timer = body_b->timer & 0xFFF;
        body->state = hero_b_step_threshold_b05dc;
    }
}

ASM_FUNC("asm/code_080B0460/hero_b_step_threshold_b05dc.s", void hero_b_step_threshold_b05dc(struct HeroBody *body))

void hero_b_take_target_pose(struct HeroBody *body)
{
    struct Hero *hero_b = gWorld->heroB;
    struct HeroBody *body_b = &hero_b->body;
    s32 x;
    s32 y;

    if ((body_b->sprite->flags & 8) != 0)
    {
        set_slot_value(0x2B, -1);
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            x = body_b->targetX / 0x100 + body_b->ofsX - 0x3C;
            y = body_b->targetY / 0x100 + body_b->ofsY;
            place_targets(body_b, x, y, 0, -1);
            body_b->height = body_b->targetZ / 0x100 + 0x10;
            set_motion_offsets(body_b, 0x600);
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            if (body_b->data->pose == -1)
                sprite_set_action(body_b, 5, 0x2000, 0);
            else
                sprite_set_action(body_b, 5, 0x204D, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
        }
        body->state = hero_b_wait_ratio_then_fx;
    }
}

NONMATCH("asm/code_080B0460/hero_b_wait_ratio_then_fx.s", void hero_b_wait_ratio_then_fx(struct HeroBody *body))
{
    struct HeroBody *body_b;
    struct Hero *hero_b;
    s32 ratioLo;
    s32 ratioHi;
    s32 ratio;
    s32 inRange;

    hero_b = gWorld->heroB;
    body_b = &hero_b->body;
    step_motion(body_b);
    ratioLo = hero_b->body.ratioLo;
    ratioHi = hero_b->body.ratioHi;
    ratio = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(ratioHi << 14, ratioLo);
    if (hero_b->body.step > ratio)
        inRange = 0;
    else
        inRange = hero_b->body.progress >= ratio;
    if (inRange != 0)
    {
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            if (body_b->data->pose == -1)
                sprite_set_action(body_b, 6, 0x2000, 0);
            else
                sprite_set_action(body_b, 6, 0x204D, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
        }
    }
    if ((body_b->flags & 0x20) != 0)
    {
        set_slot_value(0x2E, -1);
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            if (body_b->data->pose == -1)
                sprite_set_action(body_b, 8, 0x2000, 0);
            else
                sprite_set_action(body_b, 8, 0x204D, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
        }
        effect_spawn_at(0x1E, body_b->posX / 0x100, body_b->posY / 0x100, body_b->posZ / 0x100, body_b);
        body->state = pair_end_place_both_a;
    }
}
END_NONMATCH

void pair_end_place_both_a(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct Hero *hero_a = world->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct Hero *hero_b = world->heroB;
    struct HeroBody *body_b = &hero_b->body;
    struct HeroData *data;
    struct HeroSprite *sprite;
    s16 *span;
    s32 mirror;

    if ((body_b->sprite->flags & 8) != 0)
    {
        set_slot_value(0x53, -1);
        if ((hero_b->body.mode & 6) == 2 || (hero_b->body.mode & 6) == 4)
        {
            data = body_b->data;
            place_targets(body_b, data->targetX / 0x100, data->targetY / 0x100, data->targetH / 0x100, -1);
            set_motion_start(body_b, 0x400);
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            if (body_b->data->pose == -1)
                sprite_set_action(body_b, 0, 0x2002, 0);
            else
                sprite_set_action(body_b, 0, 0x204F, 0);
            sprite = body_b->sprite;
            mirror = 0;
            span = &body_b->spanA;
            if (!(span[0] < span[1]))
                mirror = 1;
            ((struct HeroMirrorBits *)&sprite->mirrorFlags)->mirror = mirror;
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 0;
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            body_b->state = 0;
            enter_finish_rebuild(body_b);
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            data = body_a->data;
            place_targets(body_a, data->targetX / 0x100, data->targetY / 0x100, data->targetH / 0x100, -1);
            set_motion_start(body_a, 0x400);
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            if (body_a->data->pose == -1)
                sprite_set_action(body_a, 0, 0x2002, 0);
            else
                sprite_set_action(body_a, 0, 0x204F, 0);
            sprite = body_a->sprite;
            mirror = 0;
            span = &body_a->spanA;
            if (!(span[0] < span[1]))
                mirror = 1;
            ((struct HeroMirrorBits *)&sprite->mirrorFlags)->mirror = mirror;
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            body_a->state = 0;
            enter_finish_rebuild(body_a);
        }
    }
}

void hero_pair_place_b_beside_a(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct Hero *hero_a = world->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct Hero *hero_b = world->heroB;
    struct HeroBody *body_b = &hero_b->body;

    if ((body_b->sprite->flags & 8) != 0)
    {
        set_slot_value(0x2B, -1);
        if ((hero_b->body.mode & 6) == 2 || (hero_b->body.mode & 6) == 4)
        {
            place_targets(body_b, body_a->targetX / 0x100 + hero_a->body.ofsX + 0x20,
                          body_a->targetY / 0x100 + hero_a->body.ofsY, 0, -1);
            hero_b->body.height = body_b->targetZ / 0x100 + 0x20;
            set_motion_offsets(body_b, 0x399);
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            u16 v;

            place_targets(body_a, body_b->targetX / 0x100 + body_b->ofsX,
                          body_b->targetY / 0x100 + body_b->ofsY, body_b->targetZ / 0x100 + 0x18, -1);
            v = 0x20;
            body_a->height = v;
            set_motion_offsets(body_a, 0x600);
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            sprite_set_action(body_b, 1, 0x2061, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
        }
        body_b->timer = body_b->timer & 0xFFF;
        body->state = hero_b_step_reach_13a4;
    }
}

ASM_FUNC("asm/code_080B0460/hero_b_step_reach_13a4.s", void hero_b_step_reach_13a4(struct HeroBody *body))
