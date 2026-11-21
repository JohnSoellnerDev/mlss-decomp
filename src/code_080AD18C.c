#include "global.h"
#include "ram_routines.h"

struct HeroSprite
{
    u8 filler0[0x02];
    s16 ofsY;
    u8 filler4[0x0E - 0x04];
    u16 unkE;
    u8 filler10[0x11 - 0x10];
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
    s8 *offsets;
    u8 filler0E8[0xEC - 0xE8];
    s16 poseIndex;
    u8 filler0EE[0xF4 - 0xEE];
    u8 count;
    u8 filler0F5[0xF6 - 0xF5];
    s16 unkF6;
    u8 filler0F8[0x110 - 0xF8];
    u8 unk110;
    u8 unk111;
    u8 filler112[0x114 - 0x112];
    u32 poseValue;
    u8 filler118[0x11E - 0x118];
    u16 flags11E;
};

struct HeroBody
{
    struct HeroBody *next;
    u8 filler04[0x08 - 0x04];
    struct HeroSprite *sprite;
    u8 filler0C[0x10 - 0x0C];
    s32 x;
    s32 y;
    s32 z;
    u8 filler1C[0x24 - 0x1C];
    u16 count;
    u8 filler26[0x28 - 0x26];
    struct HeroData *data;
    struct HeroBody *partner;
    struct HeroBody *child;
    u8 filler34[0x38 - 0x34];
    s32 targetX;
    s32 targetY;
    s32 targetZ;
    u8 filler44[0x4C - 0x44];
    void (*state)(struct HeroBody *);
    u32 unk50;
    u8 filler54[0x6C - 0x54];
    void *anim;
    u8 filler70[0x75 - 0x70];
    u8 order;
    u8 mode;
    u8 filler77[0x79 - 0x77];
    u8 flags;
    s16 rate;
    u16 frame;
    s16 progress;
    u8 filler80[0x84 - 0x80];
    struct HeroSpan span;
    u8 filler88[0x92 - 0x88];
    u16 height;
    u8 filler94[0x9C - 0x94];
    u32 place;
    s32 counter;
    u32 unkA4;
    s32 unkA8;
    s16 step;
    u16 timer;
    u8 filler0B0[0xB2 - 0xB0];
    u16 vel;
    u8 filler0B4[0xC3 - 0xB4];
    s8 ofsX;
    s8 ofsY;
    s8 ofsZ;
    u8 filler0C6[0xCA - 0xC6];
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
    u8 filler0B[0x0C - 0x0B];
    u8 flags;
    u8 filler0D[0x58 - 0x0D];
    struct Hero *slots[6];
    struct Hero *heroA;
    struct Hero *heroB;
    struct HeroBody *layers;
    u8 filler7C[0x24C - 0x7C];
    void *trackedObj;
};

struct ObjFlags
{
    u8 filler0[0x20];
    u8 flags;
};

struct KeyState
{
    u8 filler0[0x3E];
    u16 held;
};

extern struct KeyState gSys;

#define KEY_A 1
#define KEY_B 2

extern struct World *gWorld;

// World.flags: slow everything down while a timed press is due.
#define WORLD_SLOW_TIMING 8

// HeroBody.flags: the hero's motion has finished.
#define HERO_MOTION_DONE 0x20

// The top four bits of HeroBody.timer hold the result of a timed button press.
#define TIMING_PENDING 0
#define TIMING_MISSED  1
#define TIMING_HIT     2
#define TIMING_RESULT(timer) (((timer) >> 12) & 0xF)
#define WITH_TIMING_RESULT(timer, result) (((timer) & 0xFFF) | ((result) << 12))

// HeroBody.progress at which a jump lands.
#define STEP_LANDING 0x4000

// HeroSprite.flags: the current animation has finished.
#define HERO_SPRITE_ANIM_DONE 8

// HeroBody.mode: the hero is in play (bits 1-2 are 1 or 2).
#define HERO_MODE_ACTIVE(mode) (((mode) & 6) == 2 || ((mode) & 6) == 4)

void state_stack_advance_progress(struct HeroBody *body);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void set_slot_value(u32 id, s32 value);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void set_motion_start(struct HeroBody *body, u32 speed);
void set_motion_offsets(struct HeroBody *body, u32 offsets);
s32 obj_last_x(void *obj);
s32 obj_last_y(void *obj);

static inline void *tracked_obj(void)
{
    return gWorld->trackedObj;
}

void hero_a_active_anims(struct HeroBody *body);
void hero_b_place_from_a(struct HeroBody *body);
void hero_b_step_end_d354(struct HeroBody *body);
void hero_b_follow_tracked_object(struct HeroBody *body);
void pair_regroup(struct HeroBody *body);
void set_body_part(struct HeroBody *body, u32 art, s32 x, s32 y, u32 extra);
void sprite_objects_draw_obj_tiles(u32 a, u32 b);
void hero_b_enter_side(struct HeroBody *body);
void hero_b_join_partner(struct HeroBody *body);
void hero_b_set_fall_timers(struct HeroBody *body);
void step_motion(struct HeroBody *body);
void enter_finish_rebuild(struct HeroBody *body);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 z, struct HeroBody *owner);
u32 resolve_contact_action(struct HeroBody *a, struct HeroBody *b, u32 c, u32 d, u32 e);
void obj_slot_update(struct HeroBody *obj);
void display_flush(void);
void obj_free_parts(struct HeroBody *body);
void body_set_pose(u32 art, struct HeroBody *body, s32 x, s32 y);
struct HeroSprite *find_lowest_order_pose(struct HeroBody *body);
void pair_action_b_follow_a(struct HeroBody *body);
void pair_action_b_follow_partner(struct HeroBody *body);
void pair_action_a_hold(struct HeroBody *body);
void pair_action_a_start(struct HeroBody *body);
struct HeroBody *obj_spawn_linked(struct HeroBody *parent, u32 kind, u32 value);
void hero_b_to_partner_slow(struct HeroBody *body);
void pair_action_pick_partner(struct HeroBody *body);
u32 mt19937_next(void);
s32 rand_below(s32 a, u32 b);
void obj_release(struct HeroBody *obj);
void hero_b_to_partner_fast(struct HeroBody *body);
void pair_finish_and_rebuild(struct HeroBody *body);

void hero_a_step_16_b_idle(struct HeroBody *body)
{
    struct Hero *hero_a = gWorld->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct HeroBody *body_b = &gWorld->heroB->body;

    if (body_b->state == 0)
    {
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            body_a->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0x10);
            body_a->progress = 0;
            ((struct HeroFlagBits *)&body_a->flags)->active = 0;
        }
        body->state = hero_a_active_anims;
    }
}

// Steps hero A's motion; once it has finished, plays sound slot 0x65 and starts each hero in play
// on action 0 (A: 0x2032, B: 0x2061) with sprite mode 1, then goes on to hero_b_place_from_a.
void hero_a_active_anims(struct HeroBody *body)
{
    struct Hero *hero_a = gWorld->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct HeroBody *body_b = &gWorld->heroB->body;

    state_stack_advance_progress(body_a);
    if ((hero_a->body.flags & HERO_MOTION_DONE) != 0)
    {
        set_slot_value(0x65, -1);
        if (HERO_MODE_ACTIVE(body_a->mode))
        {
            sprite_set_action(body_a, 0, 0x2032, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            sprite_set_action(body_b, 0, 0x2061, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
        }
        body->state = hero_b_place_from_a;
    }
}

// Once hero B's animation ends: if B is in play, it moves (speed 0x200) to 12 px right of hero A's
// target (A's sprite offsets added); then B's timed-press result is cleared and the body goes on
// to hero_b_step_end_d354.
void hero_b_place_from_a(struct HeroBody *body)
{
    struct Hero *hero_a = gWorld->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct Hero *hero_b = gWorld->heroB;
    struct HeroBody *body_b = &hero_b->body;
    s32 x;
    s32 y;
    s32 z;

    if ((body_b->sprite->flags & HERO_SPRITE_ANIM_DONE) != 0)
    {
        if (HERO_MODE_ACTIVE(hero_b->body.mode))
        {
            x = body_a->targetX / 0x100 + body_a->ofsX + 0xC;
            y = body_a->targetY / 0x100 + body_a->ofsY;
            z = body_a->targetZ / 0x100;
            place_targets(body_b, x, y, z, -1);
            set_motion_start(body_b, 0x200);
        }
        body_b->timer = WITH_TIMING_RESULT(body_b->timer, TIMING_PENDING);
        body->state = hero_b_step_end_d354;
    }
}

struct HeroWorldModeBits
{
    u8 lo2 : 2;
    u8 m2 : 2;
    u8 mode : 2;
    u8 hi2 : 2;
} __attribute__((packed));

static inline void world_mode_or1(void)
{
    ((struct HeroWorldModeBits *)&gWorld->modeByte)->mode |= 1;
}

/* Step length within x and progress at least x (x clamped to 0..0x7FFF). */
static inline u32 step_reached(struct HeroBody *a, s32 x)
{
    u32 r;
    s32 t;

    if (x < 0)
        x = 0;
    if (x > 0x7FFF)
        x = 0x7FFF;
    if (a->step <= x)
    {
        t = a->progress;
        r = 1;
        if (t >= x)
            goto done;
    }
    r = 0;
done:
    return r;
}

/* Progress at least lo and step length within hi (both clamped, lo at most hi). */
static inline u32 step_in_window(struct HeroBody *a, s32 lo, s32 hi)
{
    u32 r;
    s32 t;

    if (lo < 0)
        lo = 0;
    if (hi < 0)
        hi = 0;
    if (lo > 0x4000)
        lo = 0x4000;
    if (hi > 0x7FFF)
        hi = 0x7FFF;
    if (hi < lo)
        lo = hi;
    if (a->progress >= lo)
    {
        t = a->step;
        r = 1;
        if (t <= hi)
            goto done;
    }
    r = 0;
done:
    return r;
}

/* Slow frame values for the three body chains, 6 for every listed sprite. */
static inline void frames_slow(struct HeroBody *a, struct HeroBody *b, struct HeroBody *partner)
{
    struct HeroBody *p;

    p = a;
    while (p != 0)
    {
        p->frame = 0x66;
        p = p->child;
    }
    p = b;
    while (p != 0)
    {
        p->frame = 0x66;
        p = p->child;
    }
    p = partner;
    while (p != 0)
    {
        p->frame = 0x66;
        p = p->child;
    }
    p = gWorld->layers;
    while (p != 0)
    {
        if (p->sprite != 0)
            p->sprite->frame = 6;
        p = p->next;
    }
}

/* Full frame values for the three body chains, 0x10 for every listed sprite. */
static inline void frames_full(struct HeroBody *a, struct HeroBody *b, struct HeroBody *partner)
{
    struct HeroBody *p;

    p = a;
    while (p != 0)
    {
        p->frame = 0x100;
        p = p->child;
    }
    p = b;
    while (p != 0)
    {
        p->frame = 0x100;
        p = p->child;
    }
    p = partner;
    while (p != 0)
    {
        p->frame = 0x100;
        p = p->child;
    }
    p = gWorld->layers;
    while (p != 0)
    {
        if (p->sprite != 0)
            p->sprite->frame = 0x10;
        p = p->next;
    }
}

/* Hero B's jump with a timed press of A: seven frames before the landing the game slows down
   (with WORLD_SLOW_TIMING) and the prompt appears; from six frames before to four after it the
   A icon shows over hero A and A is a hit. Any press outside that window, B, or no press at all
   is a miss. A hit regroups the pair at the landing; after a miss hero B moves on to the
   tracked object. */
void hero_b_step_end_d354(struct HeroBody *body)
{
    struct HeroBody *heroA;
    struct HeroBody *heroB;
    struct HeroBody *partner;
    s32 windowEnd;
    s32 landing;
    s32 rate;
    s32 promptStart;
    s32 windowStart;
    void (*next)(struct HeroBody *);

    heroA = &gWorld->heroA->body;
    heroB = &gWorld->heroB->body;
    partner = body->partner;
    windowEnd = STEP_LANDING;
    landing = STEP_LANDING;
    step_motion(heroB);
    rate = heroB->rate;
    promptStart = windowEnd + rate * -7;
    windowStart = windowEnd - (((rate << 1) + rate) << 1);
    windowEnd += rate << 2;

    if (heroB->progress < windowEnd)
    {
        ((struct HeroFlagBits *)&heroB->flags)->active = 0;
    }

    if (TIMING_RESULT(heroB->timer) != TIMING_PENDING)
        goto judged;

    if (step_reached(heroB, promptStart))
    {
        if ((gWorld->flags & WORLD_SLOW_TIMING) != 0)
            frames_slow(heroA, heroB, partner);
        else
            frames_full(heroA, heroB, partner);
        sprite_objects_draw_obj_tiles(0x4C, 0xC);
    }

    if (step_in_window(heroB, windowStart, windowEnd))
    {
        set_body_part(heroA, 1, 0, -0x28, 2);
        if ((gSys.held & KEY_A) != 0)
        {
            body_set_pose(3, heroA, 0x18, -0x18);
            heroB->timer = WITH_TIMING_RESULT(heroB->timer, TIMING_HIT);
        }
    }

    if (TIMING_RESULT(heroB->timer) != TIMING_PENDING)
        goto judged;

    /* the window has passed without a press */
    if (step_reached(heroB, windowEnd))
    {
        world_mode_or1();
        heroB->timer = WITH_TIMING_RESULT(heroB->timer, TIMING_MISSED);
        if ((gWorld->flags & WORLD_SLOW_TIMING) != 0)
            frames_full(heroA, heroB, partner);
        else
            frames_full(heroA, heroB, partner);
        display_flush();
        obj_free_parts(heroA);
        goto judged;
    }

    if (heroB->progress > windowEnd)
        goto judged;

    /* a press too early, or B */
    if ((gSys.held & KEY_A) == 0)
        if ((gSys.held & (KEY_A | KEY_B)) == 0)
            goto judged;

    world_mode_or1();
    if ((((struct HeroWorldModeBits *)&gWorld->modeByte)->m2 & ~1) != 0)
        goto judged;

    heroB->timer = WITH_TIMING_RESULT(heroB->timer, TIMING_MISSED);
    if ((gWorld->flags & WORLD_SLOW_TIMING) != 0)
        frames_full(heroA, heroB, partner);
    else
        frames_full(heroA, heroB, partner);
    display_flush();
    obj_free_parts(heroA);
    body_set_pose(6, heroA, 0x18, -0x18);

judged:
    {
        s32 result = TIMING_RESULT(heroB->timer);
        if (result != TIMING_PENDING && step_in_window(heroB, landing, windowEnd) && result > TIMING_MISSED)
        {
            heroB->timer &= 0xFFF;
            next = pair_regroup;
            body->state = next;
            next(body);
            return;
        }
    }

    if ((heroB->flags & HERO_MOTION_DONE) == 0)
        return;

    if ((heroA->mode & 6) == 2 || (heroA->mode & 6) == 4)
    {
        sprite_set_action(heroA, 6, 0x2032, 0);
        ((struct HeroSpriteBits *)&heroA->sprite->flags)->mode = 1;
    }
    if ((heroB->mode & 6) == 2 || (heroB->mode & 6) == 4)
    {
        sprite_set_action(heroB, 7, 0x2061, 0);
        ((struct HeroSpriteBits *)&heroB->sprite->flags)->mode = 0;
    }
    body->state = hero_b_follow_tracked_object;
}

/* Waits for hero B's animation to finish, then moves B 16 px left, starts animation 0x2061 on it and sends
   it toward 24 px left of the tracked object's last position (at its sprite's y offset, 40 px up), then
   continues in hero_b_enter_side. */
void hero_b_follow_tracked_object(struct HeroBody *body)
{
    struct Hero *hero_b = gWorld->heroB;
    struct HeroBody *body_b = &hero_b->body;
    s32 x;
    s32 y;

    if ((body_b->sprite->flags & HERO_SPRITE_ANIM_DONE) != 0)
    {
        s32 moved = body_b->x - 0x1000;
        body_b->x = moved;
        body_b->targetX = moved;
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            sprite_set_action(body_b, 8, 0x2061, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 0;
        }
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            s32 spriteOfsY = body_b->sprite->ofsY;
            x = (s16)obj_last_x(tracked_obj()) - 0x18;
            y = (s16)obj_last_y(tracked_obj()) + spriteOfsY;
            place_targets(body_b, x, y, 0, -1);
            { u16 height = 0x28; body_b->height = height; }
            set_motion_offsets(body_b, 0x600);
        }
        body->state = hero_b_enter_side;
    }
}
