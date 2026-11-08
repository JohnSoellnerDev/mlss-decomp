#include "global.h"
#include "ram_routines.h"

struct HeroSprite
{
    u8 filler0[0x06];
    u16 unk6;
    u8 filler08[0x11 - 0x08];
    u8 mirror;
    u8 flags;
    u8 filler13[0x20 - 0x13];
    u8 layer;
};

struct HeroSpriteBits
{
    u8 unk0 : 1;
    u8 mode : 2;
    u8 unk3 : 5;
};

struct HeroActiveBits
{
    u8 unk0 : 1;
    u8 unk1 : 4;
    u8 active : 1;
    u8 unk6 : 2;
} __attribute__((packed));

struct HeroBody
{
    struct HeroBody *next;
    u8 filler04[0x08 - 0x04];
    struct HeroSprite *sprite;
    u8 filler0C[0x2C - 0x0C];
    struct HeroBody *partner;
    struct HeroBody *child;
    u8 filler34[0x4C - 0x34];
    void (*state)(struct HeroBody *);
    u8 filler50[0x76 - 0x50];
    u8 mode;
    u8 filler77[0x79 - 0x77];
    u8 flags;
    s16 rate;
    u16 frame;
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
    u8 flags;
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

struct HeroSysKeys
{
    u8 filler0[0x3E];
    u16 keys;
};

extern struct World *gWorld;
extern u8 gSys[];

void state_stack_advance_progress(struct HeroBody *body);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void set_body_part(struct HeroBody *body, u32 art, s32 x, s32 y, u32 extra);
void body_set_pose(u32 art, struct HeroBody *body, s32 x, s32 y);
void obj_free_parts(struct HeroBody *body);
void display_flush(void);
void sprite_objects_draw_obj_tiles(u32 x, u32 y);
void hero_b_anim_start_step2(struct HeroBody *body);
void hero_b_active_fx_a_anim(struct HeroBody *body);

static inline void world_mode_or2(void)
{
    ((struct HeroWorldModeBits *)&gWorld->modeByte)->mode |= 2;
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

/* Near frame values for the three body chains, layer 6 for every listed sprite. */
static inline void frames_near(struct HeroBody *a, struct HeroBody *b, struct HeroBody *partner)
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
            p->sprite->layer = 6;
        p = p->next;
    }
}

/* Full frame values for the three body chains, layer 0x10 for every listed sprite. */
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
            p->sprite->layer = 0x10;
        p = p->next;
    }
}

/* Hero B steps: frame and layer values when the step starts, a body part and pose on the B key
   inside the window, the reached step or the A/B keys end it; then the rate for the next state. */
void hero_b_step_c94a0(struct HeroBody *body)
{
    struct HeroBody *a;
    struct HeroBody *b;
    struct HeroBody *partner;
    s32 end;
    s32 start;
    u32 m;
    void (*fn)(struct HeroBody *);

    a = &gWorld->heroA->body;
    b = &gWorld->heroB->body;
    partner = body->partner;
    end = 0x4000;
    start = 0;
    state_stack_advance_progress(b);

    if (b->progress < end)
    {
        ((struct HeroActiveBits *)&b->flags)->active = 0;
    }

    if ((b->timer >> 12) != 0)
        goto after_zero;

    if (step_reached(b, start))
    {
        if ((gWorld->flags & 8) != 0)
            frames_near(a, b, partner);
        else
            frames_full(a, b, partner);
        sprite_objects_draw_obj_tiles(0x4C, 0xC);
    }

    if (step_in_window(b, start, end))
    {
        set_body_part(b, 2, 0, 0x14, 0);
        if ((((struct HeroSysKeys *)gSys)->keys & 2) != 0)
        {
            body_set_pose(3, b, 0, -0x28);
            obj_free_parts(b);
            world_mode_or2();
            if ((((struct HeroWorldModeBits *)&gWorld->modeByte)->m2 & ~2) == 0)
            {
                if ((gWorld->flags & 8) != 0)
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
        world_mode_or2();
        b->timer = (b->timer & 0xFFF) | 0x1000;
        if ((gWorld->flags & 8) != 0)
            frames_full(a, b, partner);
        else
            frames_full(a, b, partner);
        display_flush();
        obj_free_parts(b);
        goto after_zero;
    }

    if (b->progress > end)
        goto after_zero;

    if ((((struct HeroSysKeys *)gSys)->keys & 3) == 0)
        goto after_zero;

    world_mode_or2();
    if ((((struct HeroWorldModeBits *)&gWorld->modeByte)->m2 & ~2) != 0)
        goto after_zero;

    b->timer = (b->timer & 0xFFF) | 0x1000;
    if ((gWorld->flags & 8) != 0)
        frames_full(a, b, partner);
    else
        frames_full(a, b, partner);
    display_flush();
    obj_free_parts(b);
    body_set_pose(6, b, 0, -0x28);

after_zero:
    {
        s32 tmode = b->timer >> 12;
        if (tmode != 0 && step_in_window(b, start, end) && tmode > 1)
        {
            b->timer &= 0xFFF;
            fn = hero_b_anim_start_step2;
            body->state = fn;
            fn(body);
            return;
        }
    }

    if ((b->flags & 0x20) == 0)
        return;

    m = b->mode & 6;
    if (m == 2 || m == 4)
    {
        sprite_set_action(b, 6, 0x2063, 0);
        ((struct HeroSpriteBits *)&b->sprite->flags)->mode = 1;
    }
    m = b->mode & 6;
    if (m == 2 || m == 4)
    {
        b->rate = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(0x4000, 7);
        b->progress = 0;
        ((struct HeroActiveBits *)&b->flags)->active = 0;
    }
    body->state = hero_b_active_fx_a_anim;
}

