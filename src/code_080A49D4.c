#include "global.h"
#include "ram_routines.h"

struct HeroSprite
{
    u8 filler0[0x11];
    u8 mirror;
    u8 flags;
    u8 filler13[0x20 - 0x13];
    u8 frame;
};

struct HeroMirrorBits
{
    u8 unk0 : 6;
    u8 flip : 1;
    u8 unk7 : 1;
};

struct HeroFlagBits
{
    u8 unk0 : 5;
    u8 active : 1;
    u8 unk6 : 2;
};

struct HeroSpriteBits
{
    u8 unk0 : 1;
    u8 mode : 2;
    u8 unk3 : 5;
};

struct HeroCcBits
{
    u8 low : 7;
    u8 high : 1;
};

struct HeroSpan
{
    s16 a;
    s16 b;
};

struct HeroData
{
    u8 filler0[0xD8];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler0E4[0xEC - 0xE4];
    s16 poseIndex;
    u8 filler0EE[0x114 - 0xEE];
    u32 poseValue;
    u8 filler118[0x11E - 0x118];
    u16 flags11E;
};

struct HeroBody
{
    u8 filler0[0x08];
    struct HeroSprite *sprite;
    u8 filler0C[0x24 - 0x0C];
    u16 count;
    u8 filler26[0x28 - 0x26];
    struct HeroData *data;
    struct HeroBody *partner;
    u8 filler30[0x38 - 0x30];
    s32 targetX;
    s32 targetY;
    u8 filler40[0x4C - 0x40];
    void (*state)(struct HeroBody *);
    u32 unk50;
    u8 filler54[0x6C - 0x54];
    void *anim;
    u8 filler70[0x76 - 0x70];
    u8 mode;
    u8 filler77[0x79 - 0x77];
    u8 flags;
    u16 rate;
    u16 frame;
    s16 progress;
    u8 filler80[0x84 - 0x80];
    struct HeroSpan span;
    u8 filler88[0xAC - 0x88];
    s16 step;
    u16 timer;
    u8 filler0B0[0xC3 - 0xB0];
    s8 ofsX;
    s8 ofsY;
    u8 filler0C5[0xCA - 0xC5];
    u16 scale;
    u8 cc;
};

struct Hero
{
    u8 filler0[0x08];
    struct HeroBody body;
};

struct World
{
    u8 filler0[0x0A];
    u8 modeByte;
    u8 filler0B[0x70 - 0x0B];
    struct Hero *heroA;
    struct Hero *heroB;
};

extern struct World *gWorld;
extern u8 *gParty;
extern u8 gSys[];

void state_stack_advance_progress(struct HeroBody *body);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void set_slot_value(u32 id, s32 value);
void step_motion(struct HeroBody *body);
void set_cur_flags_both(u32 value);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void enter_finish_rebuild(struct HeroBody *body);
void set_motion_start(struct HeroBody *body, u32 speed);
void schedule_pose_task(struct HeroBody *obj);
void resolve_contact_action(struct HeroBody *a, struct HeroBody *b, u32 c, u32 d, u32 e);

void hero_a_sprite_step_2(struct HeroBody *body);
void hero_a_begin_slow_step(struct HeroBody *body);
void hero_a_link_to_partner(struct HeroBody *body);
void hero_a_step_once(struct HeroBody *body);
void hero_b_begin_step(struct HeroBody *body);
void hero_b_step_with_input(struct HeroBody *body);
void hero_a_step_with_input(struct HeroBody *body);
void hero_pair_place_a_then_b(struct HeroBody *body);
void hero_pair_place_finish(struct HeroBody *body);
void pair_meet_hero_pair_place_finish_b(struct HeroBody *body);
void hero_b_show_anim_5(struct HeroBody *body);
void hero_a_show_anim_5(struct HeroBody *body);
void hero_b_link_to_partner_b(struct HeroBody *body);
void hero_a_link_to_partner_b(struct HeroBody *body);
void hero_b_go_to_partner(struct HeroBody *body);
void hero_a_show_anim_0(struct HeroBody *body);
void hero_b_show_anim_0(struct HeroBody *body);
void hero_b_begin_step_sprite_15(struct HeroBody *body);
void hero_b_sprite_step_20(struct HeroBody *body);
void hero_b_prepare_step(struct HeroBody *body);
void hero_b_begin_slow_step(struct HeroBody *body);
void hero_a_step_sprite_18(struct HeroBody *body);
void hero_a_prepare_step(struct HeroBody *body);
void hero_a_begin_step_sprite_15(struct HeroBody *body);

void hero_b_link_to_partner(struct HeroBody *body)
{
    struct Hero *hero = gWorld->heroB;
    struct HeroBody *body_a = &hero->body;
    struct HeroBody *partner = body->partner;

    state_stack_advance_progress(body_a);
    if ((hero->body.flags & 0x20) != 0)
    {
        set_slot_value(0x7E, -1);
        ((struct HeroCcBits *)&hero->body.cc)->low = 0;
        body_a->scale = 0x100;
        resolve_contact_action(body_a, partner, 0, 0, 0x08052003);
        body_a->count = body_a->count + 1;
        body->state = hero_b_begin_step_sprite_15;
    }
}

void hero_b_begin_step_sprite_15(struct HeroBody *body)
{
    struct Hero *hero = gWorld->heroB;

    if ((hero->body.sprite->flags & 8) != 0)
    {
        if ((hero->body.mode & 6) == 2 || (hero->body.mode & 6) == 4)
        {
            hero->body.rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0xF);
            hero->body.progress = 0;
            ((struct HeroFlagBits *)&hero->body.flags)->active = 0;
        }
        body->state = hero_b_prepare_step;
    }
}

void hero_b_prepare_step(struct HeroBody *body)
{
    struct Hero *hero = gWorld->heroB;
    struct HeroBody *body_a = &hero->body;

    state_stack_advance_progress(body_a);
    if ((hero->body.flags & 0x20) != 0)
    {
        if ((hero->body.mode & 6) == 2 || (hero->body.mode & 6) == 4)
        {
            sprite_set_action(body_a, 0, -1, 0);
            ((struct HeroMirrorBits *)&body_a->sprite->mirror)->flip = 0;
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            body_a->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 2);
            body_a->progress = 0;
            ((struct HeroFlagBits *)&body_a->flags)->active = 0;
        }
        body->state = hero_pair_place_a_then_b;
    }
}

void hero_pair_place_a_then_b(struct HeroBody *body)
{
    struct Hero *hero_a = gWorld->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct Hero *hero_b = gWorld->heroB;
    struct HeroBody *body_b = &hero_b->body;
    struct HeroData *data;
    struct HeroSprite *sprite;
    struct HeroSpan *span;
    s32 tx;
    s32 ty;
    s32 th;
    s32 x;
    s32 y;
    s32 h;

    state_stack_advance_progress(body_b);
    if ((hero_b->body.flags & 0x20) != 0)
    {
        if ((hero_a->body.mode & 6) == 2 || (hero_a->body.mode & 6) == 4)
        {
            data = body_a->data;
            tx = data->targetX;
            if (tx < 0)
                tx += 0xFF;
            x = tx >> 8;
            ty = data->targetY;
            if (ty < 0)
                ty += 0xFF;
            y = ty >> 8;
            th = data->targetH;
            if (th < 0)
                th += 0xFF;
            h = th >> 8;
            place_targets(body_a, x, y, h, -1);
            set_motion_start(body_a, 0x400);
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            if (body_a->data->poseIndex == -1)
                sprite_set_action(body_a, 0, 0x2002, 0);
            else
                sprite_set_action(body_a, 0, 0x204F, 0);
            sprite = body_a->sprite;
            {
                s32 flip = 0;
                span = &body_a->span;
                if (span->a >= span->b)
                    flip = 1;
                ((struct HeroMirrorBits *)&sprite->mirror)->flip = flip;
            }
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            body_a->state = 0;
            enter_finish_rebuild(body_a);
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            data = body_b->data;
            tx = data->targetX;
            if (tx < 0)
                tx += 0xFF;
            x = tx >> 8;
            ty = data->targetY;
            if (ty < 0)
                ty += 0xFF;
            y = ty >> 8;
            th = data->targetH;
            if (th < 0)
                th += 0xFF;
            h = th >> 8;
            place_targets(body_b, x, y, h, -1);
            set_motion_start(body_b, 0x400);
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            if (body_b->data->poseIndex == -1)
                sprite_set_action(body_b, 0, 0x2002, 0);
            else
                sprite_set_action(body_b, 0, 0x204F, 0);
            sprite = body_b->sprite;
            {
                s32 flip = 0;
                span = &body_b->span;
                if (span->a >= span->b)
                    flip = 1;
                ((struct HeroMirrorBits *)&sprite->mirror)->flip = flip;
            }
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 0;
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            body_b->state = 0;
            enter_finish_rebuild(body_b);
        }
    }
}


void hero_b_step_once(struct HeroBody *body)
{
    struct Hero *hero = gWorld->heroB;
    struct HeroBody *body_a = &hero->body;

    if ((hero->body.mode & 6) == 2 || (hero->body.mode & 6) == 4)
    {
        sprite_set_action(body_a, 2, 0x2053, 0);
        ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
    }
    if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
    {
        body_a->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 1);
        body_a->progress = 0;
        ((struct HeroFlagBits *)&body_a->flags)->active = 0;
    }
    body->state = hero_b_link_to_partner_b;
}
void hero_b_link_to_partner_b(struct HeroBody *body)
{
    struct Hero *hero = gWorld->heroB;
    struct HeroBody *body_a = &hero->body;
    struct HeroBody *partner = body->partner;

    state_stack_advance_progress(body_a);
    if ((hero->body.flags & 0x20) != 0)
    {
        set_slot_value(0x7D, -1);
        ((struct HeroCcBits *)&hero->body.cc)->low = 0;
        body_a->scale = 0x100;
        resolve_contact_action(body_a, partner, 0, 0, 0x08052103);
        body_a->count = body_a->count + 1;
        body->state = hero_b_sprite_step_20;
    }
}

void hero_b_sprite_step_20(struct HeroBody *body)
{
    struct Hero *hero = gWorld->heroB;
    struct HeroBody *body_a = &hero->body;

    if ((body_a->sprite->flags & 8) != 0)
    {
        if ((hero->body.mode & 6) == 2 || (hero->body.mode & 6) == 4)
        {
            sprite_set_action(body_a, 4, 0x2053, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            body_a->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0x14);
            body_a->progress = 0;
            ((struct HeroFlagBits *)&body_a->flags)->active = 0;
        }
        body->state = hero_b_show_anim_5;
    }
}
