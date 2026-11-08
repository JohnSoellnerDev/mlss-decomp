#include "global.h"
#include "ram_routines.h"

struct HeroSprite
{
    u8 filler0[0x11];
    u8 mirror;
    u8 flags;
    u8 filler13[0x20 - 0x13];
    u8 layer;
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
    u8 view;
    u8 filler0D[0x70 - 0x0D];
    struct Hero *heroA;
    struct Hero *heroB;
    struct HeroBody *layers;
};

struct SysKeys3E
{
    u8 filler0[0x3E];
    u16 keys;
};

struct WorldModeBits
{
    u8 lo4 : 4;
    u8 f45 : 2;
    u8 hi2 : 2;
} __attribute__((packed));

struct WorldMode2Bits
{
    u8 lo2 : 2;
    u8 g23 : 2;
    u8 hi4 : 4;
} __attribute__((packed));

extern struct World *gWorld;
extern u8 gSys[];

void state_stack_advance_progress(struct HeroBody *body);
void sprite_objects_draw_obj_tiles(u32 a, u32 b);
void set_body_part(struct HeroBody *body, u32 art, s32 x, s32 y, s32 extra);
void body_set_pose(u32 art, struct HeroBody *body, s32 x, s32 y);
void obj_free_parts(struct HeroBody *body);
void display_flush(void);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void set_slot_value(u32 id, s32 value);
void hero_pair_spawn_fx_anim_next(struct HeroBody *body);
void hero_b_active_place_at_partner(struct HeroBody *body);

static inline void world_mode_set_bit4(void)
{
    struct WorldModeBits *bits = (struct WorldModeBits *)&gWorld->modeByte;
    bits->f45 = bits->f45 | 1;
}

static inline u32 world_mode_bit3(void)
{
    return (((struct WorldMode2Bits *)&gWorld->modeByte)->g23 & ~1) != 0;
}

/* Previous step within x and progress at least x (x clamped to 0..0x7FFF). */
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

/* Progress at least lo and previous step within hi (both clamped, lo at most hi). */
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

/* Slow frame value for the three body chains, layer 4 for every listed sprite. */
static inline void frames_slow(struct HeroBody *a, struct HeroBody *b, struct HeroBody *partner)
{
    struct HeroBody *p;

    p = a;
    while (p != 0)
    {
        p->frame = 0x4C;
        p = p->child;
    }
    p = b;
    while (p != 0)
    {
        p->frame = 0x4C;
        p = p->child;
    }
    p = partner;
    while (p != 0)
    {
        p->frame = 0x4C;
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

/* Full frame value for the three body chains, layer 0x10 for every listed sprite. */
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

/* Hero B steps through the window: frame and layer values when the progress passes the first mark,
   a body part and pose for hero A on the A key, the timer mode at the end of the step, then the
   next state. */
void hero_b_step_window_d72e4(struct HeroBody *body)
{
    struct HeroBody *a;
    struct HeroBody *b;
    struct HeroBody *partner;
    s32 mark;
    s32 full;
    s32 rate;
    s32 lead14;
    s32 lead12;
    s32 tmode;
    u32 m;
    void (*fn)(struct HeroBody *);

    a = &gWorld->heroA->body;
    b = &gWorld->heroB->body;
    partner = body->partner;
    mark = 0x4000;
    full = 0x4000;
    state_stack_advance_progress(b);
    rate = b->rate;
    lead14 = mark - rate * 14;
    mark -= rate * 12;
    lead12 = mark;

    if (b->progress < full)
    {
        ((struct HeroFlagBits *)&b->flags)->active = 0;
    }

    /* The marks before the end of the step only count while no timer mode is set. */
    do
    {
        if ((b->timer >> 12) != 0)
            break;

        if (step_reached(b, lead14))
        {
            if ((gWorld->view & 8) != 0)
                frames_slow(a, b, partner);
            else
                frames_full(a, b, partner);
            sprite_objects_draw_obj_tiles(0x4C, 0xC);
        }

        if (step_in_window(b, mark, full))
        {
            set_body_part(a, 1, -8, -0x24, 0);
            if ((((struct SysKeys3E *)gSys)->keys & 1) != 0)
            {
                body_set_pose(1, a, 0x2E, -8);
                obj_free_parts(a);
                world_mode_set_bit4();
                if (!world_mode_bit3())
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
    } while (0);

    if ((b->timer >> 12) != 0)
        goto finish;

    if (step_reached(b, full))
    {
        world_mode_set_bit4();
        b->timer = (b->timer & 0xFFF) | 0x1000;
        if ((gWorld->view & 8) != 0)
            frames_full(a, b, partner);
        else
            frames_full(a, b, partner);
        display_flush();
        obj_free_parts(a);
        goto finish;
    }

    if (b->progress > full)
        goto finish;

    if ((((struct SysKeys3E *)gSys)->keys & 1) == 0)
    {
        if ((((struct SysKeys3E *)gSys)->keys & 3) == 0)
            goto finish;
    }

    world_mode_set_bit4();
    if (world_mode_bit3())
        goto finish;

    b->timer = (b->timer & 0xFFF) | 0x1000;
    if ((gWorld->view & 8) != 0)
        frames_full(a, b, partner);
    else
        frames_full(a, b, partner);
    display_flush();
    obj_free_parts(a);
    body_set_pose(6, a, 0x2E, -8);

finish:
    tmode = b->timer >> 12;
    if (tmode != 0 && step_in_window(b, lead12, full) && tmode > 1)
    {
        b->timer &= 0xFFF;
        fn = hero_pair_spawn_fx_anim_next;
        body->state = fn;
        fn(body);
        return;
    }

    if (step_reached(b, mark))
    {
        m = a->mode & 6;
        if (m == 2 || m == 4)
        {
            sprite_set_action(a, 1, 0x2024, 0);
            ((struct HeroSpriteBits *)&a->sprite->flags)->mode = 0;
        }
    }

    if ((b->flags & 0x20) == 0)
        return;

    set_slot_value(0xAF, -1);
    m = a->mode & 6;
    if (m == 2 || m == 4)
    {
        sprite_set_action(a, 3, 0x2024, 0);
        ((struct HeroSpriteBits *)&a->sprite->flags)->mode = 1;
    }
    m = b->mode & 6;
    if (m == 2 || m == 4)
    {
        b->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 6);
        b->progress = 0;
        ((struct HeroFlagBits *)&b->flags)->active = 0;
    }
    body->state = hero_b_active_place_at_partner;
}
