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
};

struct HeroNode
{
    u16 x;
    u16 y;
    u8 filler4[0x0E - 0x04];
    u16 z;
};

struct HeroMirrorBits
{
    u8 unk0 : 6;
    u8 mirror : 1;
    u8 unk7 : 1;
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
    u8 bit7 : 1;
};

struct HeroFlagBits
{
    u8 unk0 : 5;
    u8 active : 1;
    u8 unk6 : 2;
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
    u8 filler0C[0x24 - 0x0C];
    u16 count;
    u8 filler26[0x28 - 0x26];
    struct HeroData *data;
    struct HeroBody *partner;
    u8 filler30[0x34 - 0x30];
    struct HeroNode *node;
    s32 targetX;
    s32 targetY;
    s32 targetZ;
    u8 filler44[0x4C - 0x44];
    void (*state)(struct HeroBody *);
    u32 unk50;
    u8 filler54[0x6C - 0x54];
    const u8 *anim;
    u8 filler70[0x76 - 0x70];
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
    u16 height;
    u8 filler94[0x9C - 0x94];
    u32 unk9C;
    u8 filler_A0[0xAC - 0xA0];
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

struct World
{
    u8 filler0[0x0A];
    u8 mode;
    u8 filler0B[0x0E - 0x0B];
    u8 unkE;
    u8 filler0F[0x70 - 0x0F];
    struct Hero *heroA;
    struct Hero *heroB;
};

/* Body mode bits 1-2: 2 or 4 means the body is active on the field. */
#define HERO_MODE_ACTIVE(mode) (((mode) & 6) == 2 || ((mode) & 6) == 4)

/* HeroBody.flags bit set by state_stack_advance_progress once progress passes 0x3FFF. */
#define HERO_FLAG_PROGRESS_DONE 0x20

/* sprite_set_action arguments for a hero standing still, without and with a pose. */
#define HERO_ACTION_STAND 0x2002
#define HERO_ACTION_STAND_POSED 0x204F

/* HeroBody.rate that takes progress from 0 to 0x4000 in `frames` frames. */
#define HERO_RATE_FRAMES(frames) MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, frames)

extern struct World *gWorld;
extern u8 *gParty;

void state_stack_advance_progress(struct HeroBody *body);
void resolve_contact_action(struct HeroBody *body, struct HeroBody *partner, u32 a, u32 b, const u8 *cb);
void hero_b_sprite_step_15_6d00(struct HeroBody *body);
void hero_b_sprite_step_18_70e8(struct HeroBody *body);
void hero_b_ready_step_6d78(struct HeroBody *body);
void hero_b_ready_step_7160(struct HeroBody *body);
void hero_a_active_anims_pair(struct HeroBody *body);
void hero_b_gate_place_pair_6e34(struct HeroBody *body);
void hero_pair_place_finish_on_b_active(struct HeroBody *body);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void sound_stop_song(u32 song);
void set_slot_value(u32 id, s32 value);
void obj_release_node(void *node);
void schedule_pose_task(struct HeroBody *partner);
void hero_pair_place_then_a_clear(struct HeroBody *body);
void hero_b_active_link_to_partner(struct HeroBody *body);
void hero_b_active_link_to_partner_alt(struct HeroBody *body);
void hero_a_sprite_place_at_b(struct HeroBody *body);
void hero_a_step_wait_press(struct HeroBody *body);
void hero_a_active_clear_anim(struct HeroBody *body);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void set_motion_offsets(struct HeroBody *body, u32 offsets);
void set_motion_start(struct HeroBody *body, u32 speed);
void enter_finish_rebuild(struct HeroBody *body);
void sprite_entry_update(struct HeroNode *node);
void hero_b_drop_node_702c(struct HeroBody *body);
void hero_b_drop_node_6c40(struct HeroBody *body);

/* Hero pair state: stops songs 0x87 and 0x156, plays sound 0x9B (set_slot_value) and releases the
   node hero B carries. While B is active it gets action 2 of 0x2055 with sprite mode 1, and its
   progress restarts to run over 8 frames (rate 0x4000 / 8); then waits in
   hero_b_active_link_to_partner_alt. */
void hero_b_drop_node_6c40(struct HeroBody *body)
{
    struct Hero *hero = gWorld->heroB;
    struct HeroBody *heroB = &hero->body;

    sound_stop_song(0x87);
    sound_stop_song(0x156);
    set_slot_value(0x9B, -1);
    obj_release_node(heroB->node);
    heroB->node = 0;
    if (HERO_MODE_ACTIVE(hero->body.mode))
    {
        sprite_set_action(heroB, 2, 0x2055, 0);
        ((struct HeroSpriteBits *)&heroB->sprite->flags)->mode = 1;
    }
    if (HERO_MODE_ACTIVE(heroB->mode))
    {
        heroB->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 8);
        heroB->progress = 0;
        ((struct HeroFlagBits *)&heroB->flags)->active = 0;
    }
    body->state = hero_b_active_link_to_partner_alt;
}

void hero_b_sprite_step_15_6d00(struct HeroBody *body)
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
        body->state = hero_b_ready_step_6d78;
    }
}

/* Advances hero B's state-stack progress. When it is done, hero B (while active) takes action 0
   unflipped and starts a new progress over 2 frames (rate 0x4000 / 2); the state goes on with
   hero_b_gate_place_pair_6e34. */
void hero_b_ready_step_6d78(struct HeroBody *body)
{
    struct Hero *hero = gWorld->heroB;
    struct HeroBody *body_b = &hero->body;

    state_stack_advance_progress(body_b);
    if ((body_b->flags & HERO_FLAG_PROGRESS_DONE) != 0)
    {
        if (HERO_MODE_ACTIVE(hero->body.mode))
        {
            sprite_set_action(body_b, 0, 0xFFFFFFFF, 0);
            ((struct HeroMirrorBits *)&body_b->sprite->mirrorFlags)->mirror = 0;
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 0;
        }
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            body_b->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 2);
            body_b->progress = 0;
            ((struct HeroFlagBits *)&body_b->flags)->active = 0;
        }
        body->state = hero_b_gate_place_pair_6e34;
    }
}

/*
 * Hero pair state: steps hero B's motion and, once it has finished, sends hero A and then hero B
 * (each only while active) to the rest position in its data (8.8 fixed point, speed 0x400), gives it
 * the standing action, faces it along its move, clears its sprite mode and hands it to
 * enter_finish_rebuild.
 */
void hero_b_gate_place_pair_6e34(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct Hero *heroA = world->heroA;
    struct HeroBody *bodyA = &heroA->body;
    struct HeroBody *bodyB = &world->heroB->body;
    struct HeroData *data;
    u8 *mode;
    struct HeroSprite *sprite;
    s16 *span;
    s32 mirror;
    s32 x;
    s32 y;
    s32 h;
    s32 fixed;

    state_stack_advance_progress(bodyB);
    if ((bodyB->flags & HERO_FLAG_PROGRESS_DONE) != 0)
    {
        /* hero A */
        if (HERO_MODE_ACTIVE(heroA->body.mode))
        {
            data = bodyA->data;
            fixed = data->targetX;
            if (fixed < 0)
                fixed += 0xFF;
            x = fixed >> 8;
            fixed = data->targetY;
            if (fixed < 0)
                fixed += 0xFF;
            y = fixed >> 8;
            fixed = data->targetH;
            if (fixed < 0)
                fixed += 0xFF;
            h = fixed >> 8;
            place_targets(bodyA, x, y, h, -1);
            set_motion_start(bodyA, 0x400);
        }
        if (HERO_MODE_ACTIVE(bodyA->mode))
        {
            if (bodyA->data->pose == -1)
                sprite_set_action(bodyA, 0, HERO_ACTION_STAND, 0);
            else
                sprite_set_action(bodyA, 0, HERO_ACTION_STAND_POSED, 0);
            /* face left unless the move goes right (spanA = start x, spanB = target x) */
            sprite = bodyA->sprite;
            mirror = 0;
            span = &bodyA->spanA;
            if (!(span[0] < span[1]))
                mirror = 1;
            ((struct HeroMirrorBits *)&sprite->mirrorFlags)->mirror = mirror;
            ((struct HeroSpriteBits *)&bodyA->sprite->flags)->mode = 0;
        }
        mode = &bodyA->mode;
        if (HERO_MODE_ACTIVE(*mode))
        {
            bodyA->state = 0;
            enter_finish_rebuild(bodyA);
        }

        /* hero B */
        if (HERO_MODE_ACTIVE(bodyB->mode))
        {
            data = bodyB->data;
            fixed = data->targetX;
            if (fixed < 0)
                fixed += 0xFF;
            x = fixed >> 8;
            fixed = data->targetY;
            if (fixed < 0)
                fixed += 0xFF;
            y = fixed >> 8;
            fixed = data->targetH;
            if (fixed < 0)
                fixed += 0xFF;
            h = fixed >> 8;
            place_targets(bodyB, x, y, h, -1);
            set_motion_start(bodyB, 0x400);
        }
        mode = &bodyB->mode;
        if (HERO_MODE_ACTIVE(*mode))
        {
            if (bodyB->data->pose == -1)
                sprite_set_action(bodyB, 0, HERO_ACTION_STAND, 0);
            else
                sprite_set_action(bodyB, 0, HERO_ACTION_STAND_POSED, 0);
            sprite = bodyB->sprite;
            mirror = 0;
            span = &bodyB->spanA;
            if (!(span[0] < span[1]))
                mirror = 1;
            ((struct HeroMirrorBits *)&sprite->mirrorFlags)->mirror = mirror;
            ((struct HeroSpriteBits *)&bodyB->sprite->flags)->mode = 0;
        }
        if (HERO_MODE_ACTIVE(*mode))
        {
            bodyB->state = 0;
            enter_finish_rebuild(bodyB);
        }
    }
}

/* Hero pair state: stops song 0x156, plays sound 0x9A and releases the node hero B carries. While
   B is active it gets action 1 of 0x2055 with sprite mode 1, and its progress restarts to run out in
   one frame (rate 0x4000 / 1); then waits in hero_b_active_link_to_partner. */
void hero_b_drop_node_702c(struct HeroBody *body)
{
    struct Hero *hero = gWorld->heroB;
    struct HeroBody *heroB = &hero->body;

    sound_stop_song(0x156);
    set_slot_value(0x9A, -1);
    obj_release_node(heroB->node);
    heroB->node = NULL;
    if (HERO_MODE_ACTIVE(hero->body.mode))
    {
        sprite_set_action(heroB, 1, 0x2055, 0);
        ((struct HeroSpriteBits *)&heroB->sprite->flags)->mode = 1;
    }
    if (HERO_MODE_ACTIVE(heroB->mode))
    {
        heroB->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 1);
        heroB->progress = 0;
        ((struct HeroFlagBits *)&heroB->flags)->active = 0;
    }
    body->state = hero_b_active_link_to_partner;
}

void hero_b_sprite_step_18_70e8(struct HeroBody *body)
{
    struct Hero *hero = gWorld->heroB;

    if ((hero->body.sprite->flags & 8) != 0)
    {
        if ((hero->body.mode & 6) == 2 || (hero->body.mode & 6) == 4)
        {
            hero->body.rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0x12);
            hero->body.progress = 0;
            ((struct HeroFlagBits *)&hero->body.flags)->active = 0;
        }
        body->state = hero_b_ready_step_7160;
    }
}

/* Waits for hero B's progress timer; once it has run out, puts hero B back in its default action
   (unmirrored, sprite mode 0), starts a 2-frame progress timer and goes on with
   hero_pair_place_finish_on_b_active, which sends both heroes home. */
void hero_b_ready_step_7160(struct HeroBody *body)
{
    struct Hero *heroB = gWorld->heroB;
    struct HeroBody *bodyB = &heroB->body;

    state_stack_advance_progress(bodyB);
    if ((bodyB->flags & HERO_FLAG_PROGRESS_DONE) != 0)
    {
        if (HERO_MODE_ACTIVE(heroB->body.mode))
        {
            sprite_set_action(bodyB, 0, 0xFFFFFFFF, 0);
            ((struct HeroMirrorBits *)&bodyB->sprite->mirrorFlags)->mirror = 0;
            ((struct HeroSpriteBits *)&bodyB->sprite->flags)->mode = 0;
        }
        if (HERO_MODE_ACTIVE(bodyB->mode))
        {
            bodyB->rate = HERO_RATE_FRAMES(2);
            bodyB->progress = 0;
            ((struct HeroFlagBits *)&bodyB->flags)->active = 0;
        }
        body->state = hero_pair_place_finish_on_b_active;
    }
}

/*
 * Hero pair state: steps hero B's motion and, once it has finished (flag 0x20), for hero A and
 * then hero B (each only in mode 2 or 4), sends the hero to its data's target position (8.8 fixed
 * point, speed 0x400), sets the standing action (0x2002, or 0x204F when it has a pose), faces it
 * along its span, clears its sprite mode and hands it to enter_finish_rebuild.
 */
void hero_pair_place_finish_on_b_active(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct Hero *heroA = world->heroA;
    struct HeroBody *bodyA = &heroA->body;
    struct HeroBody *bodyB = &world->heroB->body;
    struct HeroData *data;
    u8 *mode;
    struct HeroSprite *sprite;
    s16 *span;
    s32 mirror;
    s32 x;
    s32 y;
    s32 h;
    s32 fixed;

    state_stack_advance_progress(bodyB);
    if ((bodyB->flags & 0x20) != 0)
    {
        /* hero A */
        if ((heroA->body.mode & 6) == 2 || (heroA->body.mode & 6) == 4)
        {
            data = bodyA->data;
            fixed = data->targetX;
            if (fixed < 0)
                fixed += 0xFF;
            x = fixed >> 8;
            fixed = data->targetY;
            if (fixed < 0)
                fixed += 0xFF;
            y = fixed >> 8;
            fixed = data->targetH;
            if (fixed < 0)
                fixed += 0xFF;
            h = fixed >> 8;
            place_targets(bodyA, x, y, h, -1);
            set_motion_start(bodyA, 0x400);
        }
        if ((bodyA->mode & 6) == 2 || (bodyA->mode & 6) == 4)
        {
            if (bodyA->data->pose == -1)
                sprite_set_action(bodyA, 0, 0x2002, 0);
            else
                sprite_set_action(bodyA, 0, 0x204F, 0);
            sprite = bodyA->sprite;
            mirror = 0;
            span = &bodyA->spanA;
            if (!(span[0] < span[1]))
                mirror = 1;
            ((struct HeroMirrorBits *)&sprite->mirrorFlags)->mirror = mirror;
            ((struct HeroSpriteBits *)&bodyA->sprite->flags)->mode = 0;
        }
        mode = &bodyA->mode;
        if ((*mode & 6) == 2 || (*mode & 6) == 4)
        {
            bodyA->state = 0;
            enter_finish_rebuild(bodyA);
        }

        /* hero B */
        if ((bodyB->mode & 6) == 2 || (bodyB->mode & 6) == 4)
        {
            data = bodyB->data;
            fixed = data->targetX;
            if (fixed < 0)
                fixed += 0xFF;
            x = fixed >> 8;
            fixed = data->targetY;
            if (fixed < 0)
                fixed += 0xFF;
            y = fixed >> 8;
            fixed = data->targetH;
            if (fixed < 0)
                fixed += 0xFF;
            h = fixed >> 8;
            place_targets(bodyB, x, y, h, -1);
            set_motion_start(bodyB, 0x400);
        }
        mode = &bodyB->mode;
        if ((*mode & 6) == 2 || (*mode & 6) == 4)
        {
            if (bodyB->data->pose == -1)
                sprite_set_action(bodyB, 0, 0x2002, 0);
            else
                sprite_set_action(bodyB, 0, 0x204F, 0);
            sprite = bodyB->sprite;
            mirror = 0;
            span = &bodyB->spanA;
            if (!(span[0] < span[1]))
                mirror = 1;
            ((struct HeroMirrorBits *)&sprite->mirrorFlags)->mirror = mirror;
            ((struct HeroSpriteBits *)&bodyB->sprite->flags)->mode = 0;
        }
        if ((*mode & 6) == 2 || (*mode & 6) == 4)
        {
            bodyB->state = 0;
            enter_finish_rebuild(bodyB);
        }
    }
}

void hero_b_active_link_to_partner(struct HeroBody *body)
{
    struct HeroBody *b = &gWorld->heroB->body;
    struct HeroBody *partner;

    partner = body->partner;
    state_stack_advance_progress(b);
    if ((b->flags & 0x20) != 0)
    {
        ((struct HeroCcBits *)&b->cc)->low = 0;
        b->scale = 0x100;
        resolve_contact_action(b, partner, 0, 0, (const u8 *)0x08018105);
        b->count++;
        body->state = hero_b_sprite_step_18_70e8;
    }
}

void hero_b_active_link_to_partner_alt(struct HeroBody *body)
{
    struct HeroBody *b = &gWorld->heroB->body;
    struct HeroBody *partner;

    partner = body->partner;
    state_stack_advance_progress(b);
    if ((b->flags & 0x20) != 0)
    {
        ((struct HeroCcBits *)&b->cc)->low = 0;
        b->scale = 0x100;
        resolve_contact_action(b, partner, 0, 0, (const u8 *)0x08018005);
        b->count++;
        body->state = hero_b_sprite_step_15_6d00;
    }
}

s32 pair_start_pose_then_place(struct HeroBody *body)
{
    struct HeroData *data;
    struct HeroBody *body_a;
    struct HeroBody *body_b;
    struct HeroBody *partner;
    const u8 *anim;
    s32 pose;
    u8 *record;
    struct World *world;
    u8 **party = &gParty;

    data = body->data;
    pose = data->pose;
    record = *party - (pose * 60 + 0x3C);
    data->poseValue = *(u16 *)(record + 0x2A);
    world = gWorld;
    body_a = &world->heroA->body;
    body_b = &world->heroB->body;
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
    body->state = hero_pair_place_then_a_clear;
    body_b->count = 0;
    body_a->count = 0;
    schedule_pose_task(partner);
    return 1;
}
