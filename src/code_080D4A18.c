#include "global.h"
#include "ram_routines.h"

struct HeroSprite
{
    u8 filler0[0x20];
    u8 layer;
};

struct HeroBody
{
    struct HeroBody *next;
    u8 filler04[0x08 - 0x04];
    struct HeroSprite *sprite;
    u8 filler0C[0x10 - 0x0C];
    s32 posX;
    s32 posY;
    s32 posZ;
    u8 filler1C[0x2C - 0x1C];
    struct HeroBody *partner;
    struct HeroBody *child;
    u8 filler34[0x4C - 0x34];
    void (*state)(struct HeroBody *);
    u8 filler50[0x79 - 0x50];
    u8 flags;
    s16 rate;
    u16 speed;
    s16 progress;
    u8 filler80[0xAC - 0x80];
    s16 step;
    u16 timer;
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
    u8 view;
    u8 filler0D[0x70 - 0x0D];
    struct Hero *heroA;
    struct Hero *heroB;
    struct HeroBody *layers;
};

struct HeroWorldModeBits
{
    u8 lo2 : 2;
    u8 m2 : 2;
    u8 mode : 2;
    u8 hi2 : 2;
} __attribute__((packed));

struct HeroActiveBits
{
    u8 unk0 : 1;
    u8 unk1 : 4;
    u8 active : 1;
    u8 unk6 : 2;
} __attribute__((packed));

struct HeroSysKeys
{
    u8 filler0[0x3E];
    u16 keys;
};

extern struct World *gWorld;
extern u8 gSys[];

void step_motion(struct HeroBody *body);
void set_body_part(struct HeroBody *body, u32 art, s32 x, s32 y, u32 extra);
void body_set_pose(u32 art, struct HeroBody *body, s32 x, s32 y);
void obj_free_parts(struct HeroBody *body);
void display_flush(void);
void sprite_objects_draw_obj_tiles(u32 x, u32 y);
void hero_a_mode_place_finish(struct HeroBody *body);
void hero_b_window_d5114(struct HeroBody *body);
void hero_b_step_keys_d4a18(struct HeroBody *body);

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

/* Slow speed for the three body chains, layer 4 for every listed sprite. */
static inline void frames_slow(struct HeroBody *a, struct HeroBody *b, struct HeroBody *partner)
{
    struct HeroBody *p;

    p = a;
    while (p != 0)
    {
        p->speed = 0x4C;
        p = p->child;
    }
    p = b;
    while (p != 0)
    {
        p->speed = 0x4C;
        p = p->child;
    }
    p = partner;
    while (p != 0)
    {
        p->speed = 0x4C;
        p = p->child;
    }
    p = gWorld->layers;
    while (p != 0)
    {
        if (p->sprite != 0)
            p->sprite->layer = 4;
        p = p->next;
    }
}

/* Full speed for the three body chains, layer 0x10 for every listed sprite. */
static inline void frames_full(struct HeroBody *a, struct HeroBody *b, struct HeroBody *partner)
{
    struct HeroBody *p;

    p = a;
    while (p != 0)
    {
        p->speed = 0x100;
        p = p->child;
    }
    p = b;
    while (p != 0)
    {
        p->speed = 0x100;
        p = p->child;
    }
    p = partner;
    while (p != 0)
    {
        p->speed = 0x100;
        p = p->child;
    }
    p = gWorld->layers;
    while (p != 0)
    {
        if (p->sprite != 0)
            p->sprite->layer = 0x10;
        p = p->next;
    }
}

/* Hero B steps and hands its position to the next body; with world mode bits 6-7 set the step is
   checked against two marks: speeds and layers, a body part for hero A and poses on the A key,
   then the timer mode and the next state. */
void hero_b_step_keys_d4a18(struct HeroBody *body)
{
    struct HeroBody *a;
    struct HeroBody *b;
    struct HeroBody *partner;
    struct HeroBody *next;
    s32 end;
    s32 rate;
    s32 leadPart;
    s32 lead;
    u32 m;
    void (*fn)(struct HeroBody *);

    a = &gWorld->heroA->body;
    b = &gWorld->heroB->body;
    partner = body->partner;
    end = 0x4000;
    step_motion(b);
    next = b->child;
    next->posX = b->posX;
    next->posY = b->posY;
    next->posZ = b->posZ;

    if ((gWorld->modeByte & 0xC0) == 0)
        goto finish;

    rate = b->rate;
    /* both marks lie 26 steps before the end of the step */
    lead = 0x4000 + rate * -26;
    leadPart = 0x4000 + rate * -26;
    end -= rate * 14;

    if (b->progress < end)
    {
        ((struct HeroActiveBits *)&b->flags)->active = 0;
    }

    if ((b->timer >> 12) != 0)
        goto after_zero;

    if ((gWorld->modeByte & 0xC0) != 0 && step_reached(b, lead))
    {
        if ((gWorld->view & 8) != 0)
            frames_slow(a, b, partner);
        else
            frames_full(a, b, partner);
        sprite_objects_draw_obj_tiles(0x4C, 0xC);
    }

    if (step_in_window(b, leadPart, end))
    {
        m = gWorld->modeByte & 0xC0;
        if (m == 0x80)
            set_body_part(a, 1, -22, 0, 0);
        else if (m == 0x40)
            set_body_part(a, 0xFF000000, -22, 0, 0);
        if ((((struct HeroSysKeys *)gSys)->keys & 1) != 0)
        {
            body_set_pose(7, a, 8, -0x28);
            obj_free_parts(a);
            world_mode_or1();
            if ((((struct HeroWorldModeBits *)&gWorld->modeByte)->m2 & ~1) == 0)
            {
                if ((gWorld->view & 8) != 0)
                    frames_full(a, b, partner);
                else
                    frames_full(a, b, partner);
                display_flush();
            }
            b->timer = (b->timer & 0xFFF) | 0x2000;
        }
    }

    if ((b->timer >> 12) != 0)
        goto after_zero;

    if (step_reached(b, end))
    {
        world_mode_or1();
        b->timer = (b->timer & 0xFFF) | 0x1000;
        if ((gWorld->view & 8) != 0)
            frames_full(a, b, partner);
        else
            frames_full(a, b, partner);
        display_flush();
        obj_free_parts(a);
        goto after_zero;
    }

    if (b->progress > end)
        goto after_zero;

    if ((((struct HeroSysKeys *)gSys)->keys & 1) == 0)
        if ((((struct HeroSysKeys *)gSys)->keys & 3) == 0)
            goto after_zero;

    world_mode_or1();
    if ((((struct HeroWorldModeBits *)&gWorld->modeByte)->m2 & ~1) != 0)
        goto after_zero;

    b->timer = (b->timer & 0xFFF) | 0x1000;
    if ((gWorld->view & 8) != 0)
        frames_full(a, b, partner);
    else
        frames_full(a, b, partner);
    display_flush();
    obj_free_parts(a);
    body_set_pose(6, a, 8, -0x28);

after_zero:
    {
        s32 tmode = b->timer >> 12;
        if (tmode != 0 && step_in_window(b, lead, end) && tmode > 1)
        {
            b->timer &= 0xFFF;
            fn = hero_a_mode_place_finish;
            body->state = fn;
            fn(body);
            return;
        }
    }

    if (!step_reached(b, end))
        return;

finish:
    b->timer &= 0xFFF;
    body->state = hero_b_window_d5114;
}
