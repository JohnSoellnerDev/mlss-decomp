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

struct KeyState
{
    u8 filler0[0x3C];
    u16 held;
};

extern struct KeyState gSys;

// Soft-float constants as the helpers receive them: the high word in the first register.
#define F64(hi, lo) ((((u64)(lo)) << 32) | (hi))

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
    u8 filler40[0x4C - 0x40];
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
    u16 progress;
    u8 filler80[0x84 - 0x80];
    s16 spanA;
    s16 spanB;
    u8 filler88[0xAC - 0x88];
    s16 step;
    u16 timer;
    u8 filler_B0[0xC3 - 0xB0];
    s8 ofsX;
    s8 ofsY;
    u8 filler_C5[0xCA - 0xC5];
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
    u8 filler0B[0x70 - 0x0B];
    struct Hero *heroA;
    struct Hero *heroB;
};

/* Body mode bits 1-2: 2 or 4 means the body is active on the field. */
#define HERO_MODE_ACTIVE(mode) (((mode) & 6) == 2 || ((mode) & 6) == 4)

/* HeroBody.flags bit set by state_stack_advance_progress once progress passes 0x3FFF. */
#define HERO_FLAG_PROGRESS_DONE 0x20

/* HeroBody.flags bit step_motion sets once the body has arrived. */
#define HERO_FLAG_MOTION_DONE 0x20

/* HeroBody.rate that takes progress from 0 to 0x4000 in `frames` frames. */
#define HERO_RATE_FRAMES(frames) MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, frames)

extern struct World *gWorld;
extern u8 *gParty;
extern const u8 gPartShapeRow02[];

void state_stack_advance_progress(struct HeroBody *body);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void resolve_contact_action(struct HeroBody *body, struct HeroBody *partner, u32 a, u32 b, const u8 *cb);
void schedule_pose_task(struct HeroBody *partner);
void hero_b_begin_step_sprite(struct HeroBody *body);
void hero_b_sprite_step_2(struct HeroBody *body);
void hero_b_pose_to_partner_rand_6464(struct HeroBody *body);
void hero_a_sprite_step_18_5fcc(struct HeroBody *body);
void hero_a_begin_step_sprite_15_b(struct HeroBody *body);
void hero_a_prepare_step_b(struct HeroBody *body);
void hero_a_ready_step_6044(struct HeroBody *body);
void hero_a_go_to_partner_random(struct HeroBody *body);
void hero_a_spawn_node(struct HeroBody *body);
void hero_a_link_partner_6364(struct HeroBody *body);
void hero_a_link_partner_62f8(struct HeroBody *body);
void hero_a_advance_place_pair_5d20(struct HeroBody *body);
void hero_a_advance_place_pair_6100(struct HeroBody *body);
void hero_a_node_step_with_input(struct HeroBody *body);
void hero_a_drop_node_5f18(struct HeroBody *body);
void hero_a_release_node(struct HeroBody *body);
void sprite_entry_update(struct HeroNode *node);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
u32 mt19937_next(void);
s32 rand_below(s32 range, u32 seed);
u64 __floatsidf(s32 n);
u64 __adddf3(u64 a, u64 b);
u64 __muldf3(u64 a, u64 b);
s32 __fixdfsi(u64 a);
void set_motion_start(struct HeroBody *body, u32 speed);
void enter_finish_rebuild(struct HeroBody *body);
void hero_b_node_input_step_67a8(struct HeroBody *body);
void hero_b_spawn_node_66c8(struct HeroBody *body);
void step_motion(struct HeroBody *body);
void *spawn_obj_by_id(u32 id, u32 a, u32 b, u32 c);
void hero_pair_place_b_then_a(struct HeroBody *body);
void set_slot_value(u32 id, s32 value);
void obj_release_node(void *node);

/* Plays sound 0xAD and releases hero A's node object, then starts animation 2 (0x2026) on hero A,
 * lets its progress run for 2 frames and moves on to hero_a_link_partner_6364. */
void hero_a_release_node(struct HeroBody *body)
{
    struct Hero *hero = gWorld->heroA;
    struct HeroBody *body_a = &hero->body;

    set_slot_value(0xAD, -1); /* plays sound 0xAD */
    obj_release_node(body_a->node);
    body_a->node = 0;
    if (HERO_MODE_ACTIVE(hero->body.mode))
    {
        sprite_set_action(body_a, 2, 0x2026, 0);
        ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
    }
    if (HERO_MODE_ACTIVE(body_a->mode))
    {
        body_a->rate = HERO_RATE_FRAMES(2);
        body_a->progress = 0;
        ((struct HeroFlagBits *)&body_a->flags)->active = 0;
    }
    body->state = hero_a_link_partner_6364;
}

void hero_a_begin_step_sprite_15_b(struct HeroBody *body)
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
        body->state = hero_a_prepare_step_b;
    }
}

/*
 * Hero pair state: steps hero A's progress and, once it has finished, puts an active hero A back to
 * its default action (unmirrored, sprite mode 0), restarts its progress to run out in 2 frames and
 * goes on with hero_a_advance_place_pair_5d20.
 */
void hero_a_prepare_step_b(struct HeroBody *body)
{
    struct Hero *hero = gWorld->heroA;
    struct HeroBody *body_a = &hero->body;

    state_stack_advance_progress(body_a);
    if ((body_a->flags & HERO_FLAG_PROGRESS_DONE) != 0)
    {
        if (HERO_MODE_ACTIVE(hero->body.mode))
        {
            sprite_set_action(body_a, 0, -1, 0);
            ((struct HeroMirrorBits *)&body_a->sprite->mirrorFlags)->mirror = 0;
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        if (HERO_MODE_ACTIVE(body_a->mode))
        {
            body_a->rate = HERO_RATE_FRAMES(2);
            body_a->progress = 0;
            ((struct HeroFlagBits *)&body_a->flags)->active = 0;
        }
        body->state = hero_a_advance_place_pair_5d20;
    }
}

/*
 * Hero pair state: steps hero A's motion and, once it has finished (flag 0x20), for hero A and
 * then hero B (each only in mode 2 or 4), sends the hero to its data's target position (8.8 fixed
 * point, speed 0x400), sets the standing action (0x2002, or 0x204F when it has a pose), faces it
 * along its span, clears its sprite mode and hands it to enter_finish_rebuild.
 */
void hero_a_advance_place_pair_5d20(struct HeroBody *body)
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

    state_stack_advance_progress(bodyA);
    if ((bodyA->flags & 0x20) != 0)
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

/* Hero pair state: plays sound 0x9C, frees hero A's node object, poses an active hero A
   (pose 1, action 0x2026), restarts its progress to run out in 2 frames and goes on with
   hero_a_link_partner_62f8. */
void hero_a_drop_node_5f18(struct HeroBody *body)
{
    struct Hero *heroA = gWorld->heroA;
    struct HeroBody *bodyA = &heroA->body;

    set_slot_value(0x9C, -1);
    obj_release_node(bodyA->node);
    bodyA->node = NULL;
    if (HERO_MODE_ACTIVE(heroA->body.mode))
    {
        sprite_set_action(bodyA, 1, 0x2026, 0);
        ((struct HeroSpriteBits *)&bodyA->sprite->flags)->mode = 1;
    }
    if (HERO_MODE_ACTIVE(bodyA->mode))
    {
        bodyA->rate = HERO_RATE_FRAMES(2);
        bodyA->progress = 0;
        ((struct HeroFlagBits *)&bodyA->flags)->active = 0;
    }
    body->state = hero_a_link_partner_62f8;
}

void hero_a_sprite_step_18_5fcc(struct HeroBody *body)
{
    struct Hero *hero = gWorld->heroA;

    if ((hero->body.sprite->flags & 8) != 0)
    {
        if ((hero->body.mode & 6) == 2 || (hero->body.mode & 6) == 4)
        {
            hero->body.rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0x12);
            hero->body.progress = 0;
            ((struct HeroFlagBits *)&hero->body.flags)->active = 0;
        }
        body->state = hero_a_ready_step_6044;
    }
}

/* Waits for hero A's progress timer; once it has run out, puts hero A back in its default action
   (unmirrored, sprite mode 0), starts a 2-frame progress timer and goes on with
   hero_a_advance_place_pair_6100, which sends both heroes home. */
void hero_a_ready_step_6044(struct HeroBody *body)
{
    struct Hero *heroA = gWorld->heroA;
    struct HeroBody *bodyA = &heroA->body;

    state_stack_advance_progress(bodyA);
    if ((bodyA->flags & HERO_FLAG_PROGRESS_DONE) != 0)
    {
        if (HERO_MODE_ACTIVE(heroA->body.mode))
        {
            sprite_set_action(bodyA, 0, 0xFFFFFFFF, 0);
            ((struct HeroMirrorBits *)&bodyA->sprite->mirrorFlags)->mirror = 0;
            ((struct HeroSpriteBits *)&bodyA->sprite->flags)->mode = 0;
        }
        if (HERO_MODE_ACTIVE(bodyA->mode))
        {
            bodyA->rate = HERO_RATE_FRAMES(2);
            bodyA->progress = 0;
            ((struct HeroFlagBits *)&bodyA->flags)->active = 0;
        }
        body->state = hero_a_advance_place_pair_6100;
    }
}

/* Advances hero A's motion; once it is done, sends both heroes back to their
   home targets, puts them in their idle pose facing along their span and
   hands each one over to the finish-rebuild state. */
void hero_a_advance_place_pair_6100(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct Hero *hero_a = world->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct HeroBody *body_b = &world->heroB->body;
    struct HeroData *data;
    u8 *mode;
    struct HeroSprite *sprite;
    s16 *span;
    s32 mirror;
    s32 home_x;
    s32 home_y;
    s32 home_z;
    s32 fixed;

    state_stack_advance_progress(body_a);
    if ((body_a->flags & HERO_FLAG_PROGRESS_DONE) != 0)
    {
        if (HERO_MODE_ACTIVE(hero_a->body.mode))
        {
            /* home position, 24.8 fixed point to whole units (rounding toward zero) */
            data = body_a->data;
            fixed = data->targetX;
            if (fixed < 0)
                fixed += 0xFF;
            home_x = fixed >> 8;
            fixed = data->targetY;
            if (fixed < 0)
                fixed += 0xFF;
            home_y = fixed >> 8;
            fixed = data->targetH;
            if (fixed < 0)
                fixed += 0xFF;
            home_z = fixed >> 8;
            place_targets(body_a, home_x, home_y, home_z, -1);
            set_motion_start(body_a, 0x400);
        }
        if (HERO_MODE_ACTIVE(body_a->mode))
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
        mode = &body_a->mode;
        if (HERO_MODE_ACTIVE(*mode))
        {
            body_a->state = NULL;
            enter_finish_rebuild(body_a);
        }
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            data = body_b->data;
            fixed = data->targetX;
            if (fixed < 0)
                fixed += 0xFF;
            home_x = fixed >> 8;
            fixed = data->targetY;
            if (fixed < 0)
                fixed += 0xFF;
            home_y = fixed >> 8;
            fixed = data->targetH;
            if (fixed < 0)
                fixed += 0xFF;
            home_z = fixed >> 8;
            place_targets(body_b, home_x, home_y, home_z, -1);
            set_motion_start(body_b, 0x400);
        }
        mode = &body_b->mode;
        if (HERO_MODE_ACTIVE(*mode))
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
        if (HERO_MODE_ACTIVE(*mode))
        {
            body_b->state = NULL;
            enter_finish_rebuild(body_b);
        }
    }
}

void hero_a_link_partner_62f8(struct HeroBody *body)
{
    struct HeroBody *a = &gWorld->heroA->body;
    struct HeroBody *partner;

    partner = body->partner;
    state_stack_advance_progress(a);
    if ((a->flags & 0x20) != 0)
    {
        ((struct HeroCcBits *)&a->cc)->low = 0;
        a->scale = 0x100;
        resolve_contact_action(a, partner, 0, 0, gPartShapeRow02 + 0x7C8);
        a->count++;
        body->state = hero_a_sprite_step_18_5fcc;
    }
}

void hero_a_link_partner_6364(struct HeroBody *body)
{
    struct HeroBody *a = &gWorld->heroA->body;
    struct HeroBody *partner;

    partner = body->partner;
    state_stack_advance_progress(a);
    if ((a->flags & 0x20) != 0)
    {
        ((struct HeroCcBits *)&a->cc)->low = 0;
        a->scale = 0x100;
        resolve_contact_action(a, partner, 0, 0, gPartShapeRow02 + 0x6C8);
        a->count++;
        body->state = hero_a_begin_step_sprite_15_b;
    }
}

s32 hero_pair_pose_start_63d0(struct HeroBody *body)
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
    body->state = hero_b_pose_to_partner_rand_6464;
    body_b->count = 0;
    body_a->count = 0;
    schedule_pose_task(partner);
    return 1;
}

/* State step (hero B's twin of hero_a_go_to_partner_random): with data flag 0x400 the body's
   animation speed is randomised to slow (0.4-0.7x), with flag 0x800 to fast (1.5-1.8x), in the
   body's 8.8 rate and its sprite's 4.4 rate. Plays sound 0x3F, sends hero B walking to 32 px left
   of the partner's target (speed 0x300) with its walk animation (0x2002, or 0x204F when a pose is
   set), facing the way it goes, then hands over to hero_b_spawn_node_66c8. */
void hero_b_pose_to_partner_rand_6464(struct HeroBody *body)
{
    struct HeroBody *heroB = &gWorld->heroB->body;
    struct HeroBody *partner = body->partner;
    s32 tx;
    s32 ty;
    s32 x;
    s32 y;
    struct HeroSprite *sprite;
    s16 *span;
    s32 mirror;

    if ((body->data->flags & 0x400) != 0)
    {
        struct HeroSprite *sprite;
        s32 roll;
        double speed;

        roll = rand_below(4, mt19937_next());
        speed = roll;
        if (roll < 0)
            speed += 4294967296.0;
        speed = (speed * 0.1 + 0.4) * 256.0;
        body->frame = (s32)speed;
        roll = rand_below(4, mt19937_next());
        sprite = body->sprite;
        speed = roll;
        if (roll < 0)
            speed += 4294967296.0;
        speed = (speed * 0.1 + 0.4) * 16.0;
        sprite->frame = (s32)speed;
    }
    if ((body->data->flags & 0x800) != 0)
    {
        struct HeroSprite *sprite;
        s32 roll;
        double speed;

        roll = rand_below(4, mt19937_next());
        speed = roll;
        if (roll < 0)
            speed += 4294967296.0;
        speed = (speed * 0.1 + 1.5) * 256.0;
        body->frame = (s32)speed;
        roll = rand_below(4, mt19937_next());
        sprite = body->sprite;
        speed = roll;
        if (roll < 0)
            speed += 4294967296.0;
        speed = (speed * 0.1 + 1.5) * 16.0;
        sprite->frame = (s32)speed;
    }
    set_slot_value(0x3F, -1);
    if ((heroB->mode & 6) == 2 || (heroB->mode & 6) == 4)
    {
        tx = partner->targetX;
        if (tx < 0)
            tx += 0xFF;
        x = (tx >> 8) + partner->ofsX - 0x20;
        ty = partner->targetY;
        if (ty < 0)
            ty += 0xFF;
        y = (ty >> 8) + partner->ofsY;
        place_targets(heroB, x, y, 0, -1);
        set_motion_start(heroB, 0x300);
    }
    if ((heroB->mode & 6) == 2 || (heroB->mode & 6) == 4)
    {
        if (heroB->data->pose == -1)
            sprite_set_action(heroB, 0, 0x2002, 0);
        else
            sprite_set_action(heroB, 0, 0x204F, 0);
        sprite = heroB->sprite;
        mirror = 0;
        span = &heroB->spanA;
        if (!(span[0] < span[1]))
            mirror = 1;
        ((struct HeroMirrorBits *)&sprite->mirrorFlags)->mirror = mirror;
        ((struct HeroSpriteBits *)&heroB->sprite->flags)->mode = 0;
    }
    body->state = hero_b_spawn_node_66c8;
}

/* Hero B's twin of hero_a_spawn_node: walks hero B (sent off by hero_b_pose_to_partner_rand_6464)
 * until it arrives; then plays sound 0x87, takes pose 0x2055, spawns object 0x208B as its node and
 * starts an 82-frame window (progress 0 to 0x4000) that hero_b_node_input_step_67a8 runs. */
void hero_b_spawn_node_66c8(struct HeroBody *body)
{
    struct Hero *hero = gWorld->heroB;
    struct HeroBody *heroB = &hero->body;

    step_motion(heroB);
    if ((heroB->flags & HERO_FLAG_MOTION_DONE) != 0)
    {
        set_slot_value(0x87, -1);
        if (HERO_MODE_ACTIVE(hero->body.mode))
        {
            sprite_set_action(heroB, 0, 0x2055, 0);
            ((struct HeroSpriteBits *)&heroB->sprite->flags)->mode = 1;
        }
        heroB->node = spawn_obj_by_id(0x208B, 0, 2, 1);
        if (HERO_MODE_ACTIVE(heroB->mode))
        {
            heroB->rate = HERO_RATE_FRAMES(82);
            heroB->progress = 0;
            ((struct HeroFlagBits *)&heroB->flags)->active = 0;
        }
        heroB->timer &= 0xFFF;
        body->state = hero_b_node_input_step_67a8;
    }
}
