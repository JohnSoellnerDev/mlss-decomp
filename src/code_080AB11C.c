#include "global.h"
#include "ram_routines.h"

struct HeroSprite
{
    u8 filler0[0x06];
    s16 scale;
    u8 filler8[0x11 - 0x08];
    u8 mirror;
    u8 flags;
    u8 filler13[0x16 - 0x13];
    u16 unk16;
    u8 filler18[0x1E - 0x18];
    u8 unk1E;
    u8 filler1F[0x20 - 0x1F];
    u8 layer;
    u8 unk21;
    u8 filler22;
    u8 unk23;
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
    u8 filler0E4[0xEC - 0xE4];
    s16 poseIndex;
    u8 filler0EE[0x114 - 0xEE];
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
    u8 filler70[0x76 - 0x70];
    u8 mode;
    u8 filler77[0x79 - 0x77];
    u8 flags;
    s16 rate;
    s16 frame;
    s16 progress;
    u8 filler80[0x84 - 0x80];
    struct HeroSpan span;
    u8 filler88[0x92 - 0x88];
    u16 height;
    u8 filler94[0xAC - 0x94];
    s16 step;
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
    u8 filler0B;
    u8 layerFlags;
    u8 filler0D[0x70 - 0x0D];
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
extern struct KeyState gSys;

void state_stack_advance_progress(struct HeroBody *body);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void set_slot_value(u32 id, s32 value);
void step_motion(struct HeroBody *body);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void set_motion_start(struct HeroBody *body, u32 speed);
void set_motion_speed_aux(struct HeroBody *body, u32 speed, u32 aux);
void set_motion_offsets(struct HeroBody *body, u32 offsets);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 z, struct HeroBody *owner);
void enter_finish_rebuild(struct HeroBody *body);
void start_action_4(struct HeroBody *body);
void hero_b_progress_end(struct HeroBody *body);
void hero_b_active_hand_over(struct HeroBody *body);
void set_body_part(struct HeroBody *body, u32 art, s32 x, s32 y, u32 extra);
void body_set_pose(u32 art, struct HeroBody *body, s32 x, s32 y);
void obj_free_parts(struct HeroBody *body);
void display_flush(void);
void sprite_objects_draw_obj_tiles(u32 a, u32 b);
u32 find_frame_entry(struct HeroBody *obj, u32 index);
void hero_a_on_partner(struct HeroBody *body);
void hero_a_link_to_partner_fx(struct HeroBody *body);

void hero_pair_finish_b_place_a_action_copy(struct HeroBody *body);
void hero_b_move_to_partner_pose(struct HeroBody *body);
void hero_a_active_copy_pos_fx(struct HeroBody *body);
void hero_a_active_place_partner_anims(struct HeroBody *body);
void hero_a_walk_to_partner_then_pair(struct HeroBody *body);
void hero_pair_place_action_both(void);
void hero_b_active_place_at_a_pose(struct HeroBody *body);
void hero_b_active_start_step(struct HeroBody *body);
void hero_a_snap_to_b_target(struct HeroBody *body);
void hero_a_active_follow_b(struct HeroBody *body);
void hero_pair_follow_step(struct HeroBody *body);

struct HeroWorldModeBits
{
    u8 lo2 : 2;
    u8 m2 : 2;
    u8 mode : 2;
    u8 hi2 : 2;
} __attribute__((packed));

/* World mode bits 4-5: set bit 5. */
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

/* Hero A follows the partner: hero B's sprite scale from hero A's height, frame and layer values
   at the marks of the step, hero B's pose on the keys, then hero A is placed on the partner. */
void hero_pair_follow_step(struct HeroBody *body)
{
    struct HeroBody *a;
    struct HeroBody *b;
    struct HeroBody *partner;
    s32 zLimit;
    s32 rate;
    s32 lead8;
    s32 lead6;
    s32 near;
    s32 end;
    s32 h;
    u32 m;
    u32 mb;

    a = &gWorld->heroA->body;
    b = &gWorld->heroB->body;
    partner = body->partner;
    step_motion(a);

    zLimit = b->ofsZ - 0xC;
    if (a->posZ / 0x100 < zLimit)
    {
        if (b->sprite->scale == 0x100)
            set_slot_value(0x7C, -1);
        b->sprite->scale = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(a->posZ, zLimit);
    }

    rate = a->rate;
    lead8 = -(rate << 3) + 0x4000;
    lead6 = 0x4000 - rate * 6;
    near = lead6;
    end = rate * 4 + 0x4000;
    if (a->progress < end)
    {
        ((struct HeroFlagBits *)&a->flags)->active = 0;
    }

    if ((a->timer >> 12) != 0)
        goto after_zero;

    if (step_reached(a, lead8))
    {
        if ((gWorld->layerFlags & 8) != 0)
            frames_win(a, b, partner);
        else
            frames_full(a, b, partner);
        sprite_objects_draw_obj_tiles(0x4C, 0xC);
    }

    if (step_in_window(a, near, end))
    {
        set_body_part(b, 2, 0x18, 0, 0);
        if ((gSys.held & 2) != 0)
        {
            body_set_pose(1, b, 0x2E, 0);
            obj_free_parts(b);
            world_mode_or2();
            if ((((struct HeroWorldModeBits *)&gWorld->modeByte)->m2 & ~2) == 0)
            {
                if ((gWorld->layerFlags & 8) != 0)
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

    if (step_reached(a, end))
    {
        world_mode_or2();
        a->timer = (a->timer & 0xFFF) | 0x1000;
        if ((gWorld->layerFlags & 8) != 0)
            frames_full(a, b, partner);
        else
            frames_full(a, b, partner);
        display_flush();
        obj_free_parts(b);
        goto after_zero;
    }

    if (a->progress > end)
        goto after_zero;

    if ((gSys.held & 3) == 0)
        goto after_zero;

    world_mode_or2();
    if ((((struct HeroWorldModeBits *)&gWorld->modeByte)->m2 & ~2) != 0)
        goto after_zero;

    a->timer = (a->timer & 0xFFF) | 0x1000;
    if ((gWorld->layerFlags & 8) != 0)
        frames_full(a, b, partner);
    else
        frames_full(a, b, partner);
    display_flush();
    obj_free_parts(b);
    body_set_pose(6, b, 0x2E, 0);

after_zero:
    {
        s32 tmode = a->timer >> 12;
        if (tmode != 0 && step_in_window(a, lead6, end) && tmode > 1)
        {
            void (*fn)(struct HeroBody *);

            a->timer &= 0xFFF;
            fn = hero_a_on_partner;
            body->state = fn;
            fn(body);
            return;
        }
    }

    if ((a->flags & 0x20) == 0)
        return;

    set_slot_value(0x30, -1);
    m = a->mode & 6;
    if (m == 2 || m == 4)
    {
        place_targets(a, partner->targetX / 0x100 + partner->ofsX - 8, partner->targetY / 0x100 + partner->ofsY,
                      partner->targetZ / 0x100 + partner->ofsZ - 8, -1);
        if (partner == &((struct Hero *)partner->data)->body)
            h = partner->ofsZ;
        else
        {
            find_frame_entry(partner, 0);
            h = partner->unk47;
        }
        h += partner->targetZ / 0x100;
        h += MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(0x1000, h * h / 0x40 + 0x40);
        a->height = h;
        set_motion_offsets(a, 0x600);
    }
    m = a->mode & 6;
    if (m == 2 || m == 4)
    {
        sprite_set_action(a, 0xC, 0x2000, 0);
        ((struct HeroSpriteBits *)&a->sprite->flags)->mode = 0;
    }
    mb = b->mode & 6;
    if (mb == 2 || mb == 4)
    {
        sprite_set_action(b, 7, 0x205F, 0);
        ((struct HeroSpriteBits *)&b->sprite->flags)->mode = 1;
    }
    body->state = hero_a_link_to_partner_fx;
}
