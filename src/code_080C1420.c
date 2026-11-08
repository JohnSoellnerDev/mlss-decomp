#include "global.h"
#include "ram_routines.h"

struct HeroSprite
{
    u8 filler0[0x11];
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

struct HeroFlag111Bits
{
    u8 unk0 : 1;
    u8 bit1 : 1;
    u8 unk2 : 6;
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
    u8 filler0EE[0xF6 - 0xEE];
    s16 unkF6;
    u8 filler0F8[0x111 - 0xF8];
    u8 flags111;
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
    u8 filler70[0x74 - 0x70];
    u8 unk74;
    u8 unk75;
    u8 mode;
    u8 filler77[0x79 - 0x77];
    u8 flags;
    u16 rate;
    u16 unk7C;
    s16 progress;
    u8 filler80[0x84 - 0x80];
    struct HeroSpan span;
    u8 filler88[0x92 - 0x88];
    u16 height;
    u32 wideLo;
    u32 wideHi;
    u8 filler9C[0xA4 - 0x9C];
    s32 unkA4;
    s32 unkA8;
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
    u8 flags0C;
    u8 filler0D;
    u8 unk0E;
    u8 filler0F[0x70 - 0x0F];
    struct Hero *heroA;
    struct Hero *heroB;
    struct HeroBody *layers;
    u8 filler7C[0x24C - 0x7C];
    void *focus24C;
};

struct KeyState
{
    u8 filler0[0x3E];
    u16 pressed;
};

struct HeroActiveBits
{
    u8 unk0 : 1;
    u8 unk1 : 4;
    u8 active : 1;
    u8 unk6 : 2;
} __attribute__((packed));

struct HeroWorldModeBits
{
    u8 lo2 : 2;
    u8 m2 : 2;
    u8 mode : 2;
    u8 hi2 : 2;
} __attribute__((packed));

struct PartyByteA
{
    u8 low2 : 2;
    u8 high6 : 6;
} __attribute__((packed));

extern struct World *gWorld;
extern u8 *gParty;
extern struct KeyState gSys;

void state_stack_advance_progress(struct HeroBody *body);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void set_slot_value(u32 id, s32 value);
void step_motion(struct HeroBody *body);
void set_cur_flags_both(u32 value);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void enter_finish_rebuild(struct HeroBody *body);
void set_motion_start(struct HeroBody *body, u32 speed);
void set_motion_offsets(struct HeroBody *body, u32 offsets);
void start_action_5(struct HeroBody *body);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 h, struct HeroBody *owner);
void schedule_pose_task(struct HeroBody *obj);
void resolve_contact_action(struct HeroBody *a, struct HeroBody *b, u32 c, u32 d, u32 e);
void body_set_pose(u32 art, struct HeroBody *body, s32 x, s32 y);
void obj_free_parts(struct HeroBody *body);
void display_flush(void);
void obj_stop(struct HeroBody *obj);
void set_motion_speed_aux(struct HeroBody *body, u32 speed, u32 aux);
void step_motion_scaled(struct HeroBody *body);
void set_body_part(struct HeroBody *body, u32 art, s32 x, s32 y, u32 extra);
s32 obj_last_x(void *obj);
s32 obj_last_y(void *obj);

void hero_a_rate_wait(struct HeroBody *body);
void hero_a_rate_anim(struct HeroBody *body);
void hero_b_active_anim_pair(struct HeroBody *body);
void hero_pair_place_finish_ab(struct HeroBody *body);
void hero_b_slot_anim_rate_next(struct HeroBody *body);
void hero_b_step_active_to_176c(struct HeroBody *body);
void enter_finish_flag(struct HeroBody *body);
void hero_b_anim_a_and_rate(struct HeroBody *body);
void hero_b_pair_step_1948(struct HeroBody *body);
void hero_a_follow_partner_target(struct HeroBody *body);
void hero_b_active_set_a_step_rate(struct HeroBody *body);
void hero_a_step_c2850(struct HeroBody *body);
void hero_pair_place_and_finish(struct HeroBody *body);
void hero_a_wide_step_finish(struct HeroBody *body);
void visible_place_hero_a_active_link_partner(struct HeroBody *body);
void hero_a_visible_place_on_target(struct HeroBody *body);
void hero_a_active_fx_anim_next(struct HeroBody *body);
void hero_b_set_step_rate_and_release(struct HeroBody *body);
void sprite_objects_draw_obj_tiles(u32 a, u32 b);
void hero_a_sprite_active_anim8_to_2520(struct HeroBody *body);

void hero_a_active_anim_24(struct HeroBody *body)
{
    struct Hero *hero_a;
    struct HeroBody *body_a;

    hero_a = gWorld->heroA;
    body_a = &hero_a->body;
    state_stack_advance_progress(body_a);
    if ((hero_a->body.flags & 0x20) != 0)
    {
        set_slot_value(0xAF, -1);
        if ((hero_a->body.mode & 6) == 2 || (hero_a->body.mode & 6) == 4)
        {
            sprite_set_action(body_a, 0, 0x2024, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
        body->state = hero_a_rate_anim;
    }
}

void hero_a_active_anim_none(struct HeroBody *body)
{
    struct Hero *hero_a;
    struct HeroBody *body_a;

    hero_a = gWorld->heroA;
    body_a = &hero_a->body;
    step_motion(body_a);
    if ((hero_a->body.flags & 0x20) != 0)
    {
        if ((hero_a->body.mode & 6) == 2 || (hero_a->body.mode & 6) == 4)
        {
            sprite_set_action(body_a, 0, -1, 0);
            ((struct HeroMirrorBits *)&body_a->sprite->mirror)->flip = 0;
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        body->state = hero_a_rate_wait;
    }
}

void owner_take_link_state(struct HeroBody *body)
{
    s32 link;

    link = body->unkA8;
    if (link == 0)
    {
        ((struct HeroFlag111Bits *)&body->data->flags111)->bit1 = 0;
        if (body->data->unkF6 == 0)
            obj_stop(body);
        else
            body->state = (void (*)(struct HeroBody *))link;
    }
}

u32 hero_pair_setup(struct HeroBody *body)
{
    struct HeroData *data;
    struct Hero *hero_a;
    struct Hero *hero_b;
    struct HeroBody *body_a;
    struct HeroBody *body_b;
    struct HeroBody *partner;
    void *anim;
    u8 **party;
    u8 *entry;
    s32 pose;

    party =&gParty;
    data = body->data;
    pose = data->poseIndex;
    entry = *party - ((((pose << 4) - pose) << 2) + 0x3C);
    data->poseValue = *(u16 *)(entry + 0x2A);
    hero_a = gWorld->heroA;
    body_a = &hero_a->body;
    hero_b = gWorld->heroB;
    body_b = &hero_b->body;
    partner = body->partner;
    body_b->partner = partner;
    body_a->partner = partner;
    anim = body->anim;
    body_b->anim = anim;
    body_a->anim = anim;
    partner->unk50 = 0;
    body_b->unk50 = 0;
    body_a->unk50 = 0;
    partner->unk7C = 0x100;
    body_b->unk7C = 0x100;
    body_a->unk7C = 0x100;
    body->state = hero_pair_place_finish_ab;
    body_b->count = 0;
    body_a->count = 0;
    schedule_pose_task(partner);
    return 1;
}

void hero_pair_place_finish_ab(struct HeroBody *body)
{
    struct Hero *hero_a;
    struct Hero *hero_b;
    struct HeroBody *body_a;
    struct HeroBody *body_b;
    struct HeroData *data;
    s32 none;
    s32 x;
    s32 y;
    s32 z;

    gWorld->unk0E = 5;
    /* world mode bits 6-7 from a party flag bit at 0x46 and one at 0x48 */
    ((struct HeroWorldModeBits *)&gWorld->modeByte)->hi2 =
        (((struct PartyByteA *)&gParty[0x46])->high6 & 1)
        + ((((u32)*(u16 *)(gParty + 0x48) << 22) >> 26) & 1) * 2;
    hero_a = gWorld->heroA;
    body_a = &hero_a->body;
    hero_b = gWorld->heroB;
    body_b = &hero_b->body;
    none = -1;
    set_slot_value(0x88, none);
    if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
    {
        data = body_a->data;
        x = data->targetX / 0x100 + 8;
        y = data->targetY / 0x100 + 0x10;
        z = data->targetH / 0x100;
        place_targets(body_a, x, y, z, none);
        set_motion_start(body_a, 0x200);
    }
    if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
    {
        sprite_set_action(body_a, 0, 0x2034, 0);
        ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
    }
    if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
    {
        body_a->state = 0;
        enter_finish_flag(body_a);
    }
    if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
    {
        data = body_b->data;
        x = data->targetX / 0x100 - 0xA;
        y = data->targetY / 0x100 - 0x10;
        z = data->targetH / 0x100;
        place_targets(body_b, x, y, z, -1);
        set_motion_start(body_b, 0x200);
    }
    if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
    {
        sprite_set_action(body_b, 0, 0x2063, 0);
        ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 0;
    }
    body->state = hero_b_step_active_to_176c;
}

void hero_b_set_rate_if_idle(struct HeroBody *body)
{
    struct Hero *hero_a;
    struct Hero *hero_b;
    struct HeroBody *body_a;
    struct HeroBody *body_b;
    s32 state;

    hero_a = gWorld->heroA;
    body_a = &hero_a->body;
    hero_b = gWorld->heroB;
    body_b = &hero_b->body;
    state = (s32)body_a->state;
    if (state == 0)
    {
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            body_b->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0x10);
            body_b->progress = state;
            ((struct HeroFlagBits *)&body_b->flags)->active = 0;
        }
        body->state = hero_b_active_anim_pair;
    }
}

void hero_b_active_anim_pair(struct HeroBody *body)
{
    struct Hero *hero_a;
    struct Hero *hero_b;
    struct HeroBody *body_a;
    struct HeroBody *body_b;

    hero_a = gWorld->heroA;
    body_a = &hero_a->body;
    hero_b = gWorld->heroB;
    body_b = &hero_b->body;
    state_stack_advance_progress(body_b);
    if ((hero_b->body.flags & 0x20) != 0)
    {
        set_slot_value(0xAF, -1);
        if ((hero_b->body.mode & 6) == 2 || (hero_b->body.mode & 6) == 4)
        {
            sprite_set_action(body_b, 1, 0x2063, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            sprite_set_action(body_a, 0, -1, 0);
            ((struct HeroMirrorBits *)&body_a->sprite->mirror)->flip = 0;
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        body->state = hero_b_anim_a_and_rate;
    }
}

void hero_b_anim_a_and_rate(struct HeroBody *body)
{
    struct Hero *hero_b;
    struct HeroBody *body_b;

    hero_b = gWorld->heroB;
    body_b = &hero_b->body;
    if ((body_b->sprite->flags & 8) != 0)
    {
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            sprite_set_action(body_b, 0xA, 0x2063, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 0;
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            body_b->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0x14);
            body_b->progress = 0;
            ((struct HeroFlagBits *)&body_b->flags)->active = 0;
        }
        body_b->timer = body_b->timer & 0xFFF;
        body->state = hero_b_pair_step_1948;
    }
}

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
        p->unk7C = 0x33;
        p = p->child;
    }
    p = b;
    while (p != 0)
    {
        p->unk7C = 0x33;
        p = p->child;
    }
    p = partner;
    while (p != 0)
    {
        p->unk7C = 0x33;
        p = p->child;
    }
    p = gWorld->layers;
    while (p != 0)
    {
        if (p->sprite != 0)
            p->sprite->frame = 3;
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
        p->unk7C = 0x100;
        p = p->child;
    }
    p = b;
    while (p != 0)
    {
        p->unk7C = 0x100;
        p = p->child;
    }
    p = partner;
    while (p != 0)
    {
        p->unk7C = 0x100;
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

/* Hero B steps in the pair: frame and layer values when the step starts, a body part and pose on
   the B key inside the window, the reached step or the A/B keys end it; then the rate for the
   next state. */
void hero_b_pair_step_1948(struct HeroBody *body)
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
        if ((gWorld->flags0C & 8) != 0)
            frames_win(a, b, partner);
        else
            frames_full(a, b, partner);
        sprite_objects_draw_obj_tiles(0x4C, 0xC);
    }

    if (step_in_window(b, start, end))
    {
        set_body_part(b, 2, 0, 0x14, 0);
        if ((gSys.pressed & 2) != 0)
        {
            body_set_pose(3, b, 0, -0x28);
            obj_free_parts(b);
            world_mode_or2();
            if ((((struct HeroWorldModeBits *)&gWorld->modeByte)->m2 & ~2) == 0)
            {
                if ((gWorld->flags0C & 8) != 0)
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
        if ((gWorld->flags0C & 8) != 0)
            frames_full(a, b, partner);
        else
            frames_full(a, b, partner);
        display_flush();
        obj_free_parts(b);
        goto after_zero;
    }

    if (b->progress > end)
        goto after_zero;

    if ((gSys.pressed & 3) == 0)
        goto after_zero;

    world_mode_or2();
    if ((((struct HeroWorldModeBits *)&gWorld->modeByte)->m2 & ~2) != 0)
        goto after_zero;

    b->timer = (b->timer & 0xFFF) | 0x1000;
    if ((gWorld->flags0C & 8) != 0)
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
            fn = hero_b_set_step_rate_and_release;
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
        b->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 7);
        b->progress = 0;
        ((struct HeroActiveBits *)&b->flags)->active = 0;
    }
    body->state = hero_a_active_fx_anim_next;
}
