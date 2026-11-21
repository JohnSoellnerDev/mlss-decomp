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

/* Body mode bits 1-2: 2 or 4 means the body is active on the field. */
#define HERO_MODE_ACTIVE(mode) (((mode) & 6) == 2 || ((mode) & 6) == 4)

/* HeroBody.flags bit set by state_stack_advance_progress once progress passes 0x3FFF. */
#define HERO_FLAG_PROGRESS_DONE 0x20

/* HeroSprite.flags: the current animation has finished. */
#define HERO_SPRITE_ANIM_DONE 8

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

void hero_a_begin_step_sprite(struct HeroBody *body);

void hero_a_link_to_partner(struct HeroBody *body)
{
    struct Hero *hero = gWorld->heroA;
    struct HeroBody *body_a = &hero->body;
    struct HeroBody *partner = body->partner;

    state_stack_advance_progress(body_a);
    if ((hero->body.flags & 0x20) != 0)
    {
        set_slot_value(0x7E, -1);
        ((struct HeroCcBits *)&hero->body.cc)->low = 0;
        body_a->scale = 0x100;
        resolve_contact_action(body_a, partner, 0, 0, 0x08052002);
        body_a->count = body_a->count + 1;
        body->state = hero_a_begin_step_sprite_15;
    }
}

void hero_a_begin_step_sprite_15(struct HeroBody *body)
{
    struct Hero *hero = gWorld->heroA;

    if ((hero->body.sprite->flags & 8) != 0)
    {
        if ((hero->body.mode & 6) == 2 || (hero->body.mode & 6) == 4)
        {
            hero->body.rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0xF);
            hero->body.progress = 0;
            ((struct HeroFlagBits *)&hero->body.flags)->active = 0;
        }
        body->state = hero_a_prepare_step;
    }
}

/*
 * Hero pair state: steps hero A's progress and, once it has finished, puts an active hero A back to
 * its default action (unmirrored, sprite mode 0), restarts its progress to run out in 2 frames
 * (rate 0x4000 / 2) and goes on with hero_pair_place_finish.
 */
void hero_a_prepare_step(struct HeroBody *body)
{
    struct Hero *hero = gWorld->heroA;
    struct HeroBody *body_a = &hero->body;

    state_stack_advance_progress(body_a);
    if ((hero->body.flags & HERO_FLAG_PROGRESS_DONE) != 0)
    {
        if (HERO_MODE_ACTIVE(hero->body.mode))
        {
            sprite_set_action(body_a, 0, -1, 0);
            ((struct HeroMirrorBits *)&body_a->sprite->mirror)->flip = 0;
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        if (HERO_MODE_ACTIVE(body_a->mode))
        {
            body_a->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 2);
            body_a->progress = 0;
            ((struct HeroFlagBits *)&body_a->flags)->active = 0;
        }
        body->state = hero_pair_place_finish;
    }
}

/*
 * Hero pair state: steps hero A's motion and, once it has finished (flag 0x20), for hero A and
 * then hero B (each only in mode 2 or 4), sends the hero to its data's target position (8.8 fixed
 * point, speed 0x400), sets the standing action (0x2002, or 0x204F when it has a pose), faces it
 * along its span, clears its sprite mode and hands it to enter_finish_rebuild.
 */
void hero_pair_place_finish(struct HeroBody *body)
{
    struct Hero *hero_a = gWorld->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct HeroBody *body_b = &gWorld->heroB->body;
    struct HeroData *data;
    struct HeroSprite *sprite;
    struct HeroSpan *span;
    s32 fixedX;
    s32 fixedY;
    s32 fixedH;
    s32 x;
    s32 y;
    s32 h;

    state_stack_advance_progress(body_a);
    if ((hero_a->body.flags & 0x20) != 0)
    {
        /* hero A */
        if ((hero_a->body.mode & 6) == 2 || (hero_a->body.mode & 6) == 4)
        {
            data = body_a->data;
            fixedX = data->targetX;
            if (fixedX < 0)
                fixedX += 0xFF;
            x = fixedX >> 8;
            fixedY = data->targetY;
            if (fixedY < 0)
                fixedY += 0xFF;
            y = fixedY >> 8;
            fixedH = data->targetH;
            if (fixedH < 0)
                fixedH += 0xFF;
            h = fixedH >> 8;
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

        /* hero B */
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            data = body_b->data;
            fixedX = data->targetX;
            if (fixedX < 0)
                fixedX += 0xFF;
            x = fixedX >> 8;
            fixedY = data->targetY;
            if (fixedY < 0)
                fixedY += 0xFF;
            y = fixedY >> 8;
            fixedH = data->targetH;
            if (fixedH < 0)
                fixedH += 0xFF;
            h = fixedH >> 8;
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

/*
 * Hero pair state: an active hero A starts action 2 (0x2024, sprite mode 1) and restarts its
 * progress to run out in a single frame (rate 0x4000 / 1); then hero_a_link_to_partner_b waits
 * for it.
 */
void hero_a_step_once(struct HeroBody *body)
{
    struct Hero *hero = gWorld->heroA;
    struct HeroBody *body_a = &hero->body;

    if (HERO_MODE_ACTIVE(hero->body.mode))
    {
        sprite_set_action(body_a, 2, 0x2024, 0);
        ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
    }
    if (HERO_MODE_ACTIVE(body_a->mode))
    {
        body_a->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 1);
        body_a->progress = 0;
        ((struct HeroFlagBits *)&body_a->flags)->active = 0;
    }
    body->state = hero_a_link_to_partner_b;
}
void hero_a_link_to_partner_b(struct HeroBody *body)
{
    struct Hero *hero = gWorld->heroA;
    struct HeroBody *body_a = &hero->body;
    struct HeroBody *partner = body->partner;

    state_stack_advance_progress(body_a);
    if ((hero->body.flags & 0x20) != 0)
    {
        set_slot_value(0x7D, -1);
        ((struct HeroCcBits *)&hero->body.cc)->low = 0;
        body_a->scale = 0x100;
        resolve_contact_action(body_a, partner, 0, 0, 0x08052102);
        body_a->count = body_a->count + 1;
        body->state = hero_a_step_sprite_18;
    }
}

/*
 * Hero pair state: once hero A's animation has finished, an active hero A plays action 4
 * (0x2024) with sprite mode 0 and restarts its progress to run out in 18 frames
 * (rate 0x4000 / 0x12), then goes on with hero_a_show_anim_5.
 */
void hero_a_step_sprite_18(struct HeroBody *body)
{
    struct Hero *hero = gWorld->heroA;
    struct HeroBody *body_a = &hero->body;

    if ((body_a->sprite->flags & HERO_SPRITE_ANIM_DONE) != 0)
    {
        if (HERO_MODE_ACTIVE(hero->body.mode))
        {
            sprite_set_action(body_a, 4, 0x2024, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        if (HERO_MODE_ACTIVE(body_a->mode))
        {
            body_a->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0x12);
            body_a->progress = 0;
            ((struct HeroFlagBits *)&body_a->flags)->active = 0;
        }
        body->state = hero_a_show_anim_5;
    }
}

/* Waits for hero A's current animation to finish, then puts it in its default pose (action 0,
 * unmirrored), lets its progress run for 2 frames and moves on to
 * pair_meet_hero_pair_place_finish_b. */
void hero_a_sprite_step_2(struct HeroBody *body)
{
    struct Hero *hero = gWorld->heroA;
    struct HeroBody *body_a = &hero->body;

    if ((body_a->sprite->flags & HERO_SPRITE_ANIM_DONE) != 0)
    {
        if (HERO_MODE_ACTIVE(hero->body.mode))
        {
            sprite_set_action(body_a, 0, -1, 0);
            ((struct HeroMirrorBits *)&body_a->sprite->mirror)->flip = 0;
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        if (HERO_MODE_ACTIVE(body_a->mode))
        {
            /* progress counts up to 0x4000 at rate per frame */
            body_a->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 2);
            body_a->progress = 0;
            ((struct HeroFlagBits *)&body_a->flags)->active = 0;
        }
        body->state = pair_meet_hero_pair_place_finish_b;
    }
}

/* Advances hero A's motion; once it is done, sends both heroes back to their
   home targets, puts them in their idle pose facing along their span and
   hands each one over to the finish-rebuild state. */
void pair_meet_hero_pair_place_finish_b(struct HeroBody *body)
{
    struct Hero *hero_a = gWorld->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct HeroBody *body_b = &gWorld->heroB->body;
    struct HeroData *data;
    struct HeroSprite *sprite;
    struct HeroSpan *span;
    s32 home_x_fixed;
    s32 home_y_fixed;
    s32 home_h_fixed;
    s32 home_x;
    s32 home_y;
    s32 home_h;

    state_stack_advance_progress(body_a);
    if ((hero_a->body.flags & HERO_FLAG_PROGRESS_DONE) != 0)
    {
        if (HERO_MODE_ACTIVE(hero_a->body.mode))
        {
            /* home position, 24.8 fixed point to whole units (rounding toward zero) */
            data = body_a->data;
            home_x_fixed = data->targetX;
            if (home_x_fixed < 0)
                home_x_fixed += 0xFF;
            home_x = home_x_fixed >> 8;
            home_y_fixed = data->targetY;
            if (home_y_fixed < 0)
                home_y_fixed += 0xFF;
            home_y = home_y_fixed >> 8;
            home_h_fixed = data->targetH;
            if (home_h_fixed < 0)
                home_h_fixed += 0xFF;
            home_h = home_h_fixed >> 8;
            place_targets(body_a, home_x, home_y, home_h, -1);
            set_motion_start(body_a, 0x400);
        }
        if (HERO_MODE_ACTIVE(body_a->mode))
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
        if (HERO_MODE_ACTIVE(body_a->mode))
        {
            body_a->state = NULL;
            enter_finish_rebuild(body_a);
        }
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            data = body_b->data;
            home_x_fixed = data->targetX;
            if (home_x_fixed < 0)
                home_x_fixed += 0xFF;
            home_x = home_x_fixed >> 8;
            home_y_fixed = data->targetY;
            if (home_y_fixed < 0)
                home_y_fixed += 0xFF;
            home_y = home_y_fixed >> 8;
            home_h_fixed = data->targetH;
            if (home_h_fixed < 0)
                home_h_fixed += 0xFF;
            home_h = home_h_fixed >> 8;
            place_targets(body_b, home_x, home_y, home_h, -1);
            set_motion_start(body_b, 0x400);
        }
        if (HERO_MODE_ACTIVE(body_b->mode))
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
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            body_b->state = NULL;
            enter_finish_rebuild(body_b);
        }
    }
}


void hero_a_show_anim_5(struct HeroBody *body)
{
    struct Hero *hero = gWorld->heroA;
    struct HeroBody *body_a = &hero->body;

    state_stack_advance_progress(body_a);
    if ((hero->body.flags & 0x20) != 0)
    {
        if ((hero->body.mode & 6) == 2 || (hero->body.mode & 6) == 4)
        {
            sprite_set_action(body_a, 5, 0x2024, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
        body->state = hero_a_sprite_step_2;
    }
}

void hero_a_show_anim_0(struct HeroBody *body)
{
    struct Hero *hero = gWorld->heroA;
    struct HeroBody *body_a = &hero->body;

    state_stack_advance_progress(body_a);
    if ((hero->body.flags & 0x20) != 0)
    {
        if ((hero->body.mode & 6) == 2 || (hero->body.mode & 6) == 4)
        {
            sprite_set_action(body_a, 0, 0x2024, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
        body->state = hero_a_begin_step_sprite;
    }
}

u32 pair_meet_hero_pair_start_b(struct HeroBody *body)
{
    struct HeroData *data;
    u8 *record;
    struct HeroBody *body_a;
    struct HeroBody *body_b;
    struct HeroBody *partner;
    void *anim;

    record = gParty - ((((body->data->poseIndex << 4) - body->data->poseIndex) << 2) + 0x3C);
    data = body->data;
    data->poseValue = *(u16 *)(record + 0x2A);
    body_a = &gWorld->heroA->body;
    body_b = &gWorld->heroB->body;
    partner = body->partner;
    body_b->partner = partner;
    body_a->partner = partner;
    anim = body->anim;
    body_b->anim = anim;
    body_a->anim = anim;
    partner->unk50 = 0;
    body_b->unk50 = 0;
    body_a->unk50 = 0;
    partner->frame = 0x100;
    body_b->frame = 0x100;
    body_a->frame = 0x100;
    body->state = hero_b_go_to_partner;
    body_b->count = 0;
    body_a->count = 0;
    schedule_pose_task(partner);
    return 1;
}

// Pair-pose state: sets the pose's speed (body->frame) and sprite frame (data flag 0x400: slow,
// 0x800: fast), plays sound 0x3F, and when hero B is in mode 2 or 4 sends it to 0x28 px left of
// the partner's target (plus the partner's x/y offsets, speed 0x300) and starts its run animation,
// facing along the span. Next state: hero_b_begin_step.
void hero_b_go_to_partner(struct HeroBody *body)
{
    struct Hero *hero_b = gWorld->heroB;
    struct HeroBody *body_b = &hero_b->body;
    struct HeroBody *partner = body->partner;
    s32 tx;
    s32 ty;
    s32 x;
    s32 y;
    struct HeroSprite *sprite;
    struct HeroSpan *span;

    if ((body->data->flags11E & 0x400) != 0)
    {
        body->frame = 0xCC;
        body->sprite->frame = 0xC;
    }
    if ((body->data->flags11E & 0x800) != 0)
    {
        body->frame = 0x199;
        body->sprite->frame = 0x19;
    }
    set_slot_value(0x3F, -1);
    if ((hero_b->body.mode & 6) == 2 || (hero_b->body.mode & 6) == 4)
    {
        /* 24.8 target to whole pixels */
        tx = partner->targetX;
        if (tx < 0)
            tx += 0xFF;
        x = (tx >> 8) + partner->ofsX - 0x28;
        ty = partner->targetY;
        if (ty < 0)
            ty += 0xFF;
        y = (ty >> 8) + partner->ofsY;
        place_targets(body_b, x, y, 0, -1);
        set_motion_start(body_b, 0x300);
    }
    if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
    {
        if (body_b->data->poseIndex == -1)
            sprite_set_action(body_b, 0, 0x2002, 0);
        else
            sprite_set_action(body_b, 0, 0x204F, 0);
        sprite = body_b->sprite;
        {
            /* mirrored when span.a >= span.b */
            s32 flip = 0;
            span = &body_b->span;
            if (span->a >= span->b)
                flip = 1;
            ((struct HeroMirrorBits *)&sprite->mirror)->flip = flip;
        }
        ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 0;
    }
    body->state = hero_b_begin_step;
}

void hero_b_begin_step(struct HeroBody *body)
{
    struct Hero *hero = gWorld->heroB;

    step_motion(&hero->body);
    if ((hero->body.flags & 0x20) != 0)
    {
        if ((hero->body.mode & 6) == 2 || (hero->body.mode & 6) == 4)
        {
            hero->body.rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0xB);
            hero->body.progress = 0;
            ((struct HeroFlagBits *)&hero->body.flags)->active = 0;
        }
        body->state = hero_b_show_anim_0;
    }
}

void hero_b_begin_step_sprite(struct HeroBody *body)
{
    struct Hero *hero = gWorld->heroB;

    if ((hero->body.sprite->flags & 8) != 0)
    {
        if ((hero->body.mode & 6) == 2 || (hero->body.mode & 6) == 4)
        {
            hero->body.rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 4);
            hero->body.progress = 0;
            ((struct HeroFlagBits *)&hero->body.flags)->active = 0;
        }
        body->state = hero_b_begin_slow_step;
    }
}

/* Advances hero B's progress; once it's done, calls set_cur_flags_both(0x40000000), plays sound 0x7C,
 * puts B in action 1 (0x2053), starts a slow step (rate 0x4000 / 20), keeps the low 12 bits of B's
 * timer and goes on in hero_b_step_with_input. */
void hero_b_begin_slow_step(struct HeroBody *body)
{
    struct Hero *hero_b = gWorld->heroB;
    struct HeroBody *body_b = &hero_b->body;

    state_stack_advance_progress(body_b);
    if ((hero_b->body.flags & HERO_FLAG_PROGRESS_DONE) != 0)
    {
        set_cur_flags_both(0x40000000);
        set_slot_value(0x7C, -1);
        if (HERO_MODE_ACTIVE(hero_b->body.mode))
        {
            sprite_set_action(body_b, 1, 0x2053, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 0;
        }
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            body_b->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 20);
            body_b->progress = 0;
            ((struct HeroFlagBits *)&body_b->flags)->active = 0;
        }
        body_b->timer = body_b->timer & 0xFFF;
        body->state = hero_b_step_with_input;
    }
}
