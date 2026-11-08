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
    u8 layer;
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

struct HeroChildFlagBits
{
    u8 bit0 : 1;
    u8 unk1 : 1;
    u8 bit2 : 1;
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

struct HeroDataFlagBits
{
    u8 unk0 : 1;
    u8 partnerLinked : 1;
    u8 unk2 : 6;
};

struct HeroData
{
    u8 filler0[0xD8];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler0E4[0xEC - 0xE4];
    s16 poseIndex;
    u8 filler0EE[0xF6 - 0xEE];
    s16 unkF6;
    u8 filler0F8[0x111 - 0xF8];
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
    s32 posX;
    s32 posY;
    s32 posZ;
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
    u8 filler44[0x47 - 0x44];
    s8 unk47;
    u8 filler48[0x4C - 0x48];
    void (*state)(struct HeroBody *);
    u32 unk50;
    u8 filler54[0x6C - 0x54];
    void *anim;
    u8 filler70[0x75 - 0x70];
    u8 unk75;
    u8 mode;
    u8 filler77[0x79 - 0x77];
    u8 flags;
    s16 rate;
    u16 speed;
    s16 progress;
    u8 filler80[0x84 - 0x80];
    struct HeroSpan span;
    u8 filler88[0x92 - 0x88];
    u16 height;
    u8 filler94[0xA0 - 0x94];
    u32 unkA0;
    u32 poseCount;
    u32 unkA8;
    s16 prevProgress;
    u16 timer;
    u8 filler0B0[0xC3 - 0xB0];
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
    u8 view;
    u8 filler0D[0x0E - 0x0D];
    u8 unk0E;
    u8 filler0F[0x70 - 0x0F];
    struct Hero *heroA;
    struct Hero *heroB;
    struct HeroBody *nodes;
    u8 filler7C[0x24C - 0x7C];
    void *trackedObj;
};

extern struct World *gWorld;
extern u8 *gParty;

void step_motion(struct HeroBody *body);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void set_slot_value(u32 id, s32 value);
void hero_pair_place_partner_height(struct HeroBody *body);
void hero_a_child_fade_b_step30(struct HeroBody *body);
void hero_b_follow_tracked(struct HeroBody *body);
void state_stack_advance_progress(struct HeroBody *body);
s32 obj_last_x(void *obj);
s32 obj_last_y(void *obj);
void set_motion_start(struct HeroBody *body, u32 speed);
void hero_b_child_follow_partner(struct HeroBody *body);
void hero_b_match_partner(struct HeroBody *body);
void hero_b_partner_link_fx(struct HeroBody *body);
void hero_b_finish_child_anim9(struct HeroBody *body);
void hero_a_release_body_cf9f4(struct HeroBody *body);
void hero_a_child_window_eb9c(struct HeroBody *body);
void enter_finish_rebuild(struct HeroBody *body);
struct HeroBody *obj_spawn_linked(struct HeroBody *parent, u32 kind, u32 value);
u32 find_frame_entry(struct HeroBody *obj, u32 index);
void hero_b_active_link_partner_anchor(struct HeroBody *body);
void hero_b_finish_child_anim9_alt(struct HeroBody *body);
void resolve_contact_action(struct HeroBody *a, struct HeroBody *b, u32 c, u32 d, const u8 *e);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 h, struct HeroBody *owner);
extern const u8 gUnk_0800E210[];
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void set_motion_offsets(struct HeroBody *body, u32 offsets);
void set_body_part(struct HeroBody *body, u32 art, s32 x, s32 y, u32 extra);
void body_set_pose(u32 art, struct HeroBody *body, s32 x, s32 y);
void obj_free_parts(struct HeroBody *body);
void display_flush(void);
void sprite_objects_draw_obj_tiles(u32 col, u32 row);
void hero_b_anim_and_step_rate8(struct HeroBody *body);
void hero_b_follow_tracked_place(struct HeroBody *body);

struct KeyState
{
    u8 filler0[0x3E];
    u16 pressed;
};

extern struct KeyState gSys;

static inline void *tracked_obj(void)
{
    return gWorld->trackedObj;
}

struct HeroWorldModeBits
{
    u8 lo2 : 2;
    u8 m2 : 2;
    u8 mode : 2;
    u8 hi2 : 2;
} __attribute__((packed));

struct HeroBit0
{
    u8 bit0 : 1;
    u8 unk1 : 7;
} __attribute__((packed));

struct HeroActiveBits
{
    u8 unk0 : 1;
    u8 unk1 : 4;
    u8 active : 1;
    u8 unk6 : 2;
} __attribute__((packed));

static inline void world_mode_or2(void)
{
    ((struct HeroWorldModeBits *)&gWorld->modeByte)->mode |= 2;
}

/* Previous progress within x and progress at least x (x clamped to 0..0x7FFF). */
static inline u32 step_reached(struct HeroBody *a, s32 x)
{
    u32 r;
    s32 t;

    if (x < 0)
        x = 0;
    if (x > 0x7FFF)
        x = 0x7FFF;
    if (a->prevProgress <= x)
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

/* Progress at least lo and previous progress within hi (both clamped, lo at most hi). */
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
        t = a->prevProgress;
        r = 1;
        if (t <= hi)
            goto done;
    }
    r = 0;
done:
    return r;
}

/* Slow speed for the three body chains, layer 8 for every listed sprite. */
static inline void frames_slow(struct HeroBody *a, struct HeroBody *b, struct HeroBody *partner)
{
    struct HeroBody *p;

    p = a;
    while (p != 0)
    {
        p->speed = 0x80;
        p = p->child;
    }
    p = b;
    while (p != 0)
    {
        p->speed = 0x80;
        p = p->child;
    }
    p = partner;
    while (p != 0)
    {
        p->speed = 0x80;
        p = p->child;
    }
    p = gWorld->nodes;
    while (p != 0)
    {
        if (p->sprite != 0)
            p->sprite->layer = 8;
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
    p = gWorld->nodes;
    while (p != 0)
    {
        if (p->sprite != 0)
            p->sprite->layer = 0x10;
        p = p->next;
    }
}

/* Hero A's child steps through a window: speed and layer changes at the first mark, a body part
   and the B key for hero B inside the window, the end mark, then hero B's pose or the next state. */
void hero_a_child_window_eb9c(struct HeroBody *body)
{
    struct HeroBody *a;
    struct HeroBody *b;
    struct HeroBody *partner;
    s32 end;
    s32 rate;
    s32 lead24;
    s32 lead22;
    s32 mid;
    u32 m;

    a = &gWorld->heroA->body;
    b = &gWorld->heroB->body;
    partner = body->partner;
    end = 0x4000;
    step_motion(a->child);
    rate = a->child->rate;
    lead24 = end - rate * 24;
    lead22 = end - (rate * 12 - rate) * 2;
    mid = lead22;
    end -= rate * 9;

    if (a->child->progress < end)
    {
        ((struct HeroActiveBits *)&a->child->flags)->active = 0;
    }

    if ((a->child->timer >> 12) != 0)
        goto after_zero;

    if (step_reached(a->child, lead24))
    {
        if ((gWorld->view & 8) != 0)
            frames_slow(a, b, partner);
        else
            frames_full(a, b, partner);
        sprite_objects_draw_obj_tiles(0x4C, 0xC);
    }

    if (step_in_window(a->child, mid, end))
    {
        set_body_part(b, 2, 0, -0x28, 0);
        if ((gSys.pressed & 2) != 0)
        {
            obj_free_parts(b);
            world_mode_or2();
            if ((((struct HeroWorldModeBits *)&gWorld->modeByte)->m2 & ~2) == 0)
            {
                if ((gWorld->view & 8) != 0)
                    frames_full(a, b, partner);
                else
                    frames_full(a, b, partner);
                display_flush();
            }
            a->child->timer &= 0xFFF;
            a->child->timer |= 0x2000;
        }
    }

    if ((a->child->timer >> 12) != 0)
        goto after_zero;

    if (step_reached(a->child, end))
    {
        world_mode_or2();
        a->child->timer &= 0xFFF;
        a->child->timer |= 0x1000;
        if ((gWorld->view & 8) != 0)
            frames_full(a, b, partner);
        else
            frames_full(a, b, partner);
        display_flush();
        obj_free_parts(b);
        goto after_zero;
    }

    if (a->child->progress > end)
        goto after_zero;

    if ((gSys.pressed & 2) == 0)
        goto after_zero;

    world_mode_or2();
    if ((((struct HeroWorldModeBits *)&gWorld->modeByte)->m2 & ~2) != 0)
        goto after_zero;

    a->child->timer &= 0xFFF;
    a->child->timer |= 0x1000;
    if ((gWorld->view & 8) != 0)
        frames_full(a, b, partner);
    else
        frames_full(a, b, partner);
    display_flush();
    obj_free_parts(b);

after_zero:
    {
        s32 tmode = a->child->timer >> 12;

        if (tmode != 0 && step_in_window(a->child, lead22, end) && tmode == 2)
        {
            if ((s32)b->poseCount <= 1)
            {
                b->poseCount++;
                body_set_pose(1, b, 0, -0x28);
            }
            else
            {
                body_set_pose(2, b, 0, -0x28);
            }
            a->state = 0;
            if ((a->child->mode & 6) == 2 || (a->child->mode & 6) == 4)
                a->child->state = hero_b_anim_and_step_rate8;
            return;
        }
    }

    if ((a->child->flags & 0x20) == 0)
        return;

    sprite_set_action(a->child, 0, 0, 0);
    ((struct HeroBit0 *)&a->child->flags)->bit0 = 1;
    ((struct HeroSpriteBits *)&a->child->sprite->flags)->mode = 0;
    a->child->unk75 = b->unk75 - 1;
    set_slot_value(0x104, -1);
    m = b->mode & 6;
    if (m == 2 || m == 4)
    {
        sprite_set_action(b, 0, 0x2065, 0);
        ((struct HeroSpriteBits *)&b->sprite->flags)->mode = 0;
    }
    m = b->mode & 6;
    if (m == 2 || m == 4)
    {
        b->rate = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(0x4000, 0x1E);
        b->progress = 0;
        ((struct HeroActiveBits *)&b->flags)->active = 0;
    }
    body->state = hero_b_follow_tracked_place;
}

void hero_b_follow_tracked_place(struct HeroBody *body)
{
    struct Hero *hero_b = gWorld->heroB;
    struct HeroBody *body_b = &hero_b->body;
    s32 x;
    s32 y;

    state_stack_advance_progress(body_b);
    if ((body_b->flags & 0x20) != 0)
    {
        set_slot_value(0xA7, -1);
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            sprite_set_action(body_b, 1, 0x2065, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 0;
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            s32 offY = body_b->posY / 0x100;
            x = (s16)obj_last_x(tracked_obj()) - 0x10;
            y = (s16)obj_last_y(tracked_obj()) + offY;
            place_targets(body_b, x, y, 0, -1);
            set_motion_start(body_b, 0x380);
        }
        body->state = hero_b_match_partner;
    }
}
