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
    u8 layer;
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
    struct HeroNode *node;
    s32 targetX;
    s32 targetY;
    s32 targetZ;
    u8 filler44[0x47 - 0x44];
    s8 unk47;
    u8 filler48[0x4C - 0x48];
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
    s16 height;
    u8 filler94[0xAC - 0x94];
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
    u8 filler_D8[0x110 - 0xD8];
    u8 flags110;
};

struct World
{
    u8 filler0[0x0A];
    u8 mode;
    u8 filler0B[0x0C - 0x0B];
    u8 flags;
    u8 filler0D[0x0E - 0x0D];
    u8 unk0E;
    u8 filler0F[0x58 - 0x0F];
    struct Hero *list[6];
    struct Hero *heroA;
    struct Hero *heroB;
    struct HeroBody *layers;
};

struct KeyState
{
    u8 filler0[0x3E];
    u16 held;
};

extern struct World *gWorld;
extern u8 *gParty;
extern struct KeyState gSys;

void state_stack_advance_progress(struct HeroBody *body);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void step_motion(struct HeroBody *body);
void set_slot_value(u32 id, s32 value);
void set_motion_offsets(struct HeroBody *body, u32 offsets);
void set_motion_start(struct HeroBody *body, u32 speed);
void step_motion_checked(struct HeroBody *body);
void enter_finish_rebuild(struct HeroBody *body);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void schedule_pose_task(struct HeroBody *partner);
void hero_a_stop_anim_if_active(struct HeroBody *body);
void hero_b_stop_anim_if_active(struct HeroBody *body);
void hero_b_anim_on_sprite(struct HeroBody *body);
void hero_b_begin_step_16(struct HeroBody *body);
void hero_a_go_to_b_target(struct HeroBody *body);
void hero_a_active_anim_24(struct HeroBody *body);
void hero_pair_place_mirror(struct HeroBody *body);
void hero_a_move_ba294(struct HeroBody *body);
void hero_a_place_at_target(struct HeroBody *body);
void hero_a_pair_finish(void);
void hero_a_approach_91e0(struct HeroBody *body);
void hero_a_step_slot_anim(struct HeroBody *body);
void hero_b_step_slot_anim(struct HeroBody *body);
u32 mt19937_next(void);
s32 rand_below(s32 range, u32 seed);
u32 find_frame_entry(struct HeroBody *obj, u32 index);
void hero_a_active_anim_none(struct HeroBody *body);
void set_body_part(struct HeroBody *body, u32 art, s32 x, s32 y, u32 extra);
void body_set_pose(u32 art, struct HeroBody *body, s32 x, s32 y);
void obj_free_parts(struct HeroBody *body);
void display_flush(void);
void sprite_objects_draw_obj_tiles(u32 a, u32 b);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 z, struct HeroBody *owner);
void hero_pair_anim_start(struct HeroBody *body);
void hero_pair_fx_anim_a_b(struct HeroBody *body);

struct PartyByteA
{
    u8 low2 : 2;
    u8 high6 : 6;
} __attribute__((packed));

struct HeroWorldModeBits
{
    u8 lo2 : 2;
    u8 m2 : 2;
    u8 mode : 2;
    u8 hi2 : 2;
} __attribute__((packed));

/* World mode bits 4-5: set bit 4. */
static inline void world_mode_or1(void)
{
    ((struct HeroWorldModeBits *)&gWorld->mode)->mode |= 1;
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

/* Window frame values for the three body chains, layer 3 for every listed sprite. */
static inline void frames_win(struct HeroBody *a, struct HeroBody *b, struct HeroBody *partner)
{
    struct HeroBody *p;

    p = a;
    while (p != 0)
    {
        p->frame = 0x33;
        p = p->child;
    }
    p = b;
    while (p != 0)
    {
        p->frame = 0x33;
        p = p->child;
    }
    p = partner;
    while (p != 0)
    {
        p->frame = 0x33;
        p = p->child;
    }
    p = gWorld->layers;
    while (p != 0)
    {
        if (p->sprite != 0)
            p->sprite->layer = 3;
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

/* Both heroes back to their data targets, standing pose and mirror bit from their span. */
void hero_pair_place_mirror(struct HeroBody *body)
{
    struct HeroBody *a;
    struct HeroBody *b;
    struct HeroData *data;
    s16 *span;

    gWorld->unk0E = 2;
    /* world mode bits 6-7 from a party flag bit at 0xA and one at 0xC */
    ((struct HeroWorldModeBits *)&gWorld->mode)->hi2 =
        (((struct PartyByteA *)&gParty[0xA])->high6 & 1)
        + ((((u32)*(u16 *)(gParty + 0xC) << 22) >> 26) & 1) * 2;
    a = &gWorld->heroA->body;
    b = &gWorld->heroB->body;
    set_slot_value(0x53, -1);
    if ((b->mode & 6) == 2 || (b->mode & 6) == 4)
    {
        data = b->data;
        place_targets(b, data->targetX / 0x100 + 0x14, data->targetY / 0x100 - 0x10, data->targetH / 0x100, -1);
        set_motion_start(b, 0x200);
    }
    if ((b->mode & 6) == 2 || (b->mode & 6) == 4)
    {
        if (b->data->pose == -1)
        {
            sprite_set_action(b, 0, 0x2002, 0);
        }
        else
        {
            sprite_set_action(b, 0, 0x204F, 0);
        }
        ((struct HeroMirrorBits *)&b->sprite->mirrorFlags)->mirror = (span = &b->spanA)[0] >= span[1];
        ((struct HeroSpriteBits *)&b->sprite->flags)->mode = 0;
    }
    if ((b->mode & 6) == 2 || (b->mode & 6) == 4)
    {
        b->state = 0;
        enter_finish_rebuild(b);
    }
    if ((a->mode & 6) == 2 || (a->mode & 6) == 4)
    {
        data = a->data;
        place_targets(a, data->targetX / 0x100 - 0x14, data->targetY / 0x100 + 0x10, data->targetH / 0x100, -1);
        set_motion_start(a, 0x200);
    }
    if ((a->mode & 6) == 2 || (a->mode & 6) == 4)
    {
        if (a->data->pose == -1)
        {
            sprite_set_action(a, 0, 0x2002, 0);
        }
        else
        {
            sprite_set_action(a, 0, 0x204F, 0);
        }
        ((struct HeroMirrorBits *)&a->sprite->mirrorFlags)->mirror = (span = &a->spanA)[0] >= span[1];
        ((struct HeroSpriteBits *)&a->sprite->flags)->mode = 0;
    }
    body->state = hero_a_active_anim_none;
}


void hero_a_rate_wait(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct HeroBody *a = &world->heroA->body;
    struct HeroBody *b = &world->heroB->body;
    u32 next = (u32)b->state;

    if (next == 0)
    {
        if ((a->mode & 6) == 2 || (a->mode & 6) == 4)
        {
            a->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0x10);
            a->progress = next;
            ((struct HeroFlagBits *)&a->flags)->active = 0;
        }
        body->state = hero_a_active_anim_24;
    }
}

void hero_a_rate_anim(struct HeroBody *body)
{
    struct Hero *hero = gWorld->heroA;
    struct HeroBody *a = &hero->body;

    if ((a->sprite->flags & 8) != 0)
    {
        if ((hero->body.mode & 6) == 2 || (hero->body.mode & 6) == 4)
        {
            sprite_set_action(a, 1, 0x2024, 0);
            ((struct HeroSpriteBits *)&a->sprite->flags)->mode = 0;
        }
        if ((a->mode & 6) == 2 || (a->mode & 6) == 4)
        {
            a->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0x14);
            a->progress = 0;
            ((struct HeroFlagBits *)&a->flags)->active = 0;
        }
        a->timer &= 0xFFF;
        body->state = hero_a_move_ba294;
    }
}

/* Hero A moves: frame and layer values when the step starts, a body part and pose for hero A on
   the A key inside the step window, the timer mode at the end of the step, then the link state or
   the animations for both heroes and an effect at hero A. */
void hero_a_move_ba294(struct HeroBody *body)
{
    struct HeroBody *a;
    struct HeroBody *b;
    struct HeroBody *partner;
    s32 start;
    s32 full;
    void (*fn)(struct HeroBody *);

    a = &gWorld->heroA->body;
    b = &gWorld->heroB->body;
    partner = body->partner;
    start = 0;
    full = 0x4000;
    state_stack_advance_progress(a);

    if (a->progress < full)
    {
        ((struct HeroFlagBits *)&a->flags)->active = 0;
    }

    if ((a->timer >> 12) != 0)
        goto after_zero;

    if (step_reached(a, start))
    {
        if ((gWorld->flags & 8) != 0)
            frames_win(a, b, partner);
        else
            frames_full(a, b, partner);
        sprite_objects_draw_obj_tiles(0x4C, 0xC);
    }

    if (step_in_window(a, start, full))
    {
        set_body_part(a, 1, 0, 0, 0);
        if ((gSys.held & 1) != 0)
        {
            body_set_pose(3, a, 0x16, 0);
            obj_free_parts(a);
            world_mode_or1();
            if ((((struct HeroWorldModeBits *)&gWorld->mode)->m2 & ~1) == 0)
            {
                if ((gWorld->flags & 8) != 0)
                    frames_full(a, b, partner);
                else
                    frames_full(a, b, partner);
                display_flush();
            }
            a->timer = (a->timer & 0xFFF) | 0x2000;
        }
    }

    if ((a->timer >> 12) != 0)
        goto after_zero;

    if (step_reached(a, full))
    {
        world_mode_or1();
        a->timer = (a->timer & 0xFFF) | 0x1000;
        if ((gWorld->flags & 8) != 0)
            frames_full(a, b, partner);
        else
            frames_full(a, b, partner);
        display_flush();
        obj_free_parts(a);
        goto after_zero;
    }

    if (a->progress > full)
        goto after_zero;

    if ((gSys.held & 1) == 0)
        if ((gSys.held & 3) == 0)
            goto after_zero;

    world_mode_or1();
    if ((((struct HeroWorldModeBits *)&gWorld->mode)->m2 & ~1) != 0)
        goto after_zero;

    a->timer = (a->timer & 0xFFF) | 0x1000;
    if ((gWorld->flags & 8) != 0)
        frames_full(a, b, partner);
    else
        frames_full(a, b, partner);
    display_flush();
    obj_free_parts(a);
    body_set_pose(6, a, 0x16, 0);

after_zero:
    {
        s32 tmode = a->timer >> 12;
        if (tmode != 0 && step_in_window(a, start, full) && tmode > 1)
        {
            a->timer &= 0xFFF;
            fn = hero_pair_fx_anim_a_b;
            body->state = fn;
            fn(body);
            return;
        }
    }

    if ((a->flags & 0x20) == 0)
        return;

    set_slot_value(0x7E, -1);
    if ((a->mode & 6) == 2 || (a->mode & 6) == 4)
    {
        sprite_set_action(a, 9, 0x2024, 0);
        ((struct HeroSpriteBits *)&a->sprite->flags)->mode = 1;
    }
    if ((b->mode & 6) == 2 || (b->mode & 6) == 4)
    {
        sprite_set_action(b, 6, 0x2062, 0);
        ((struct HeroSpriteBits *)&b->sprite->flags)->mode = 1;
    }
    effect_spawn_at(0, a->x / 0x100, a->y / 0x100, a->z / 0x100, b);
    body->state = hero_pair_anim_start;
}
