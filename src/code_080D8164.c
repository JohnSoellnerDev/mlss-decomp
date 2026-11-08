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
    u8 filler0C[0x10 - 0x0C];
    s32 posX;
    s32 posY;
    s32 posZ;
    u8 filler1C[0x2C - 0x1C];
    struct HeroBody *partner;
    struct HeroBody *child;
    u8 filler34[0x38 - 0x34];
    s32 targetX;
    s32 targetY;
    s32 targetZ;
    u8 filler44[0x4C - 0x44];
    void (*state)(struct HeroBody *);
    u8 filler50[0x74 - 0x50];
    u8 unk74;
    u8 unk75;
    u8 mode;
    u8 filler77[0x79 - 0x77];
    u8 flags;
    s16 rate;
    u16 frame;
    s16 progress;
    u8 filler80[0xA8 - 0x80];
    u32 pending;
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
    u8 filler0D[0x58 - 0x0D];
    struct Hero *list[6];
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
void set_slot_value(u32 id, s32 value);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void set_body_part(struct HeroBody *body, u32 art, s32 x, s32 y, u32 extra);
void body_set_pose(u32 art, struct HeroBody *body, s32 x, s32 y);
void obj_free_parts(struct HeroBody *body);
void display_flush(void);
void sprite_objects_draw_obj_tiles(u32 x, u32 y);
void hero_b_link_a(struct HeroBody *body);
void hero_b_unlink_a(struct HeroBody *body);
void hero_b_partner_place_height(struct HeroBody *body);

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

/* Slow frame values for the three body chains, layer 4 for every listed sprite. */
static inline void frames_slow(struct HeroBody *a, struct HeroBody *b, struct HeroBody *partner)
{
    struct HeroBody *p;

    p = a;
    while (p != 0)
    {
        p->frame = 0x40;
        p = p->child;
    }
    p = b;
    while (p != 0)
    {
        p->frame = 0x40;
        p = p->child;
    }
    p = partner;
    while (p != 0)
    {
        p->frame = 0x40;
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

/* Hero B steps: its chain follows its position, frame and layer values at the marks around the
   end of the step, a body part and pose on the B key, then the link states for both heroes. */
void hero_b_step_d8164(struct HeroBody *body)
{
    struct HeroBody *a;
    struct HeroBody *b;
    struct HeroBody *partner;
    struct HeroBody *child;
    s32 end;
    s32 full;
    s32 rate;
    s32 lead;
    void (*fn)(struct HeroBody *);

    a = &gWorld->heroA->body;
    b = &gWorld->heroB->body;
    partner = body->partner;
    end = 0x4000;
    full = 0x4000;
    step_motion(b);
    child = b->child;
    child->posX = b->posX;
    child->posY = b->posY;
    child->posZ = b->posZ;
    rate = b->rate;
    lead = end - (rate << 2);
    end += ((rate << 2) + rate) << 1;

    if (b->progress < end)
    {
        ((struct HeroActiveBits *)&b->flags)->active = 0;
    }

    if ((b->timer >> 12) != 0)
        goto after_zero;

    if (step_reached(b, lead))
    {
        if ((gWorld->flags & 8) != 0)
            frames_slow(a, b, partner);
        else
            frames_full(a, b, partner);
        sprite_objects_draw_obj_tiles(0x4C, 0xC);
    }

    if (step_in_window(b, full, end))
    {
        set_body_part(b, 2, 0x18, 0, 0);
        if ((((struct HeroSysKeys *)gSys)->keys & 2) != 0)
        {
            body_set_pose(2, b, 0x2E, 0);
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
    body_set_pose(6, b, 0x2E, 0);

after_zero:
    {
        s32 tmode = b->timer >> 12;
        if (tmode != 0 && step_in_window(b, full, end) && tmode > 1)
        {
            b->timer &= 0xFFF;
            fn = hero_b_unlink_a;
            body->state = fn;
            fn(body);
            return;
        }
    }

    if ((b->flags & 0x20) == 0)
        return;

    set_slot_value(0x9B, -1);
    if (partner->targetZ == 0)
    {
        if ((partner->mode & 6) == 2 || (partner->mode & 6) == 4)
            partner->state = hero_b_link_a;
    }
    if ((b->mode & 6) == 2 || (b->mode & 6) == 4)
    {
        sprite_set_action(b, 0x12, 0x2066, 0);
        ((struct HeroSpriteBits *)&b->sprite->flags)->mode = 1;
    }
    if (b->child != 0)
    {
        sprite_set_action(b->child, 0x12, 0x2095, 0);
        ((struct HeroSpriteBits *)&b->child->sprite->flags)->mode = 0;
    }
    b->unk74 = 0;
    b->unk75 = 0;
    if ((b->mode & 6) == 2 || (b->mode & 6) == 4)
    {
        b->rate = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(0x4000, 8);
        b->progress = 0;
        ((struct HeroActiveBits *)&b->flags)->active = 0;
    }
    partner->pending = 1;
    body->state = hero_b_partner_place_height;
}
