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

/* Bits 1-2 of HeroBody.mode are 1 or 2: the body is drawn, so its sprite can be animated. */
#define HERO_MODE_ACTIVE(mode) (((mode) & 6) == 2 || ((mode) & 6) == 4)

/* HeroBody.flags bit 5: the body's motion has reached its target. */
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

/* Hero B state (after hero_b_show_anim_5): once its animation has finished (sprite flags bit 3),
 * switches it back to action 0, unmirrored, in sprite mode 0, starts a 2-frame progress step
 * and hands over to hero_pair_place_b_then_a. */
void hero_b_sprite_step_2(struct HeroBody *body)
{
    struct Hero *hero = gWorld->heroB;
    struct HeroBody *heroBody = &hero->body;

    if ((heroBody->sprite->flags & 8) != 0)
    {
        if (HERO_MODE_ACTIVE(hero->body.mode))
        {
            sprite_set_action(heroBody, 0, 0xFFFFFFFF, 0);
            ((struct HeroMirrorBits *)&heroBody->sprite->mirrorFlags)->mirror = 0;
            ((struct HeroSpriteBits *)&heroBody->sprite->flags)->mode = 0;
        }
        if (HERO_MODE_ACTIVE(heroBody->mode))
        {
            heroBody->rate = HERO_RATE_FRAMES(2);
            heroBody->progress = 0;
            ((struct HeroFlagBits *)&heroBody->flags)->active = 0;
        }
        body->state = hero_pair_place_b_then_a;
    }
}


/*
 * Hero pair state: steps hero B's motion and, once it has finished (flag 0x20), for hero B and
 * then hero A (each only in mode 2 or 4), sends the hero to its data's target position (8.8 fixed
 * point, speed 0x400), sets the standing action (0x2002, or 0x204F when it has a pose), faces it
 * along its span, clears its sprite mode and hands it to enter_finish_rebuild.
 */
void hero_pair_place_b_then_a(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct HeroBody *bodyA = &world->heroA->body;
    struct Hero *heroB = world->heroB;
    struct HeroBody *bodyB = &heroB->body;
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
        /* hero B */
        if ((heroB->body.mode & 6) == 2 || (heroB->body.mode & 6) == 4)
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
        if ((bodyB->mode & 6) == 2 || (bodyB->mode & 6) == 4)
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
        mode = &bodyB->mode;
        if ((*mode & 6) == 2 || (*mode & 6) == 4)
        {
            bodyB->state = 0;
            enter_finish_rebuild(bodyB);
        }

        /* hero A */
        if ((bodyA->mode & 6) == 2 || (bodyA->mode & 6) == 4)
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
        mode = &bodyA->mode;
        if ((*mode & 6) == 2 || (*mode & 6) == 4)
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
        if ((*mode & 6) == 2 || (*mode & 6) == 4)
        {
            bodyA->state = 0;
            enter_finish_rebuild(bodyA);
        }
    }
}

void hero_b_show_anim_5(struct HeroBody *body)
{
    struct Hero *hero = gWorld->heroB;
    struct HeroBody *b = &hero->body;

    state_stack_advance_progress(b);
    if ((b->flags & 0x20) != 0)
    {
        if ((b->mode & 6) == 2 || (b->mode & 6) == 4)
        {
            sprite_set_action(b, 5, 0x2053, 0);
            ((struct HeroSpriteBits *)&b->sprite->flags)->mode = 1;
        }
        body->state = hero_b_sprite_step_2;
    }
}

void hero_b_show_anim_0(struct HeroBody *body)
{
    struct Hero *hero = gWorld->heroB;
    struct HeroBody *b = &hero->body;

    state_stack_advance_progress(b);
    if ((b->flags & 0x20) != 0)
    {
        if ((b->mode & 6) == 2 || (b->mode & 6) == 4)
        {
            sprite_set_action(b, 0, 0x2053, 0);
            ((struct HeroSpriteBits *)&b->sprite->flags)->mode = 1;
        }
        body->state = hero_b_begin_step_sprite;
    }
}

s32 hero_pair_start_pose_done(struct HeroBody *body)
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
    body->state = hero_a_go_to_partner_random;
    body_b->count = 0;
    body_a->count = 0;
    schedule_pose_task(partner);
    return 1;
}

/* State step: with data flag 0x400 the body's animation speed is randomised to slow (0.4-0.7x),
   with flag 0x800 to fast (1.5-1.8x), in the body's 8.8 rate and its sprite's 4.4 rate. Plays
   sound 0x3F, sends hero A walking to 32 px left of the partner's target (speed 0x300) with its
   walk animation (0x2002, or 0x204F when a pose is set), facing the way it goes, then hands over
   to hero_a_spawn_node. */
void hero_a_go_to_partner_random(struct HeroBody *body)
{
    struct HeroBody *heroA = &gWorld->heroA->body;
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
    if ((heroA->mode & 6) == 2 || (heroA->mode & 6) == 4)
    {
        tx = partner->targetX;
        if (tx < 0)
            tx += 0xFF;
        x = (tx >> 8) + partner->ofsX - 0x20;
        ty = partner->targetY;
        if (ty < 0)
            ty += 0xFF;
        y = (ty >> 8) + partner->ofsY;
        place_targets(heroA, x, y, 0, -1);
        set_motion_start(heroA, 0x300);
    }
    if ((heroA->mode & 6) == 2 || (heroA->mode & 6) == 4)
    {
        if (heroA->data->pose == -1)
            sprite_set_action(heroA, 0, 0x2002, 0);
        else
            sprite_set_action(heroA, 0, 0x204F, 0);
        sprite = heroA->sprite;
        mirror = 0;
        span = &heroA->spanA;
        if (!(span[0] < span[1]))
            mirror = 1;
        ((struct HeroMirrorBits *)&sprite->mirrorFlags)->mirror = mirror;
        ((struct HeroSpriteBits *)&heroA->sprite->flags)->mode = 0;
    }
    body->state = hero_a_spawn_node;
}

/* Walks hero A (sent off by hero_a_go_to_partner_random) until it arrives; then plays sound 0x157,
 * takes pose 0x2026, spawns object 0x208A as its node and starts a 70-frame window (progress 0 to
 * 0x4000) that hero_a_node_step_with_input runs. */
void hero_a_spawn_node(struct HeroBody *body)
{
    struct Hero *hero = gWorld->heroA;
    struct HeroBody *heroA = &hero->body;

    step_motion(heroA);
    if ((heroA->flags & HERO_FLAG_MOTION_DONE) != 0)
    {
        set_slot_value(0x157, -1);
        if (HERO_MODE_ACTIVE(hero->body.mode))
        {
            sprite_set_action(heroA, 0, 0x2026, 0);
            ((struct HeroSpriteBits *)&heroA->sprite->flags)->mode = 1;
        }
        heroA->node = spawn_obj_by_id(0x208A, 0, 2, 1);
        if (HERO_MODE_ACTIVE(heroA->mode))
        {
            heroA->rate = HERO_RATE_FRAMES(70);
            heroA->progress = 0;
            ((struct HeroFlagBits *)&heroA->flags)->active = 0;
        }
        heroA->timer &= 0xFFF;
        body->state = hero_a_node_step_with_input;
    }
}
