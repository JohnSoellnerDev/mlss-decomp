#include "global.h"
#include "ram_routines.h"

struct HeroSprite
{
    u8 filler0[0x06];
    u16 y;
    u8 filler8[0x11 - 0x08];
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
    struct HeroBody *link;
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
    struct HeroBody *next;
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
void hero_a_sprite_active_anim8_to_2520(struct HeroBody *body);

void hero_pair_mode_and_ready_anims(struct HeroBody *body);
void hero_b_sprite_active_anim3_to_2f20(struct HeroBody *body);
void sprite_objects_draw_obj_tiles(u32 a, u32 b);

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

/* Window frame values for the three body chains, layer 3 for every listed sprite. */
static inline void frames_win(struct HeroBody *a, struct HeroBody *b, struct HeroBody *partner)
{
    struct HeroBody *p;

    p = a;
    while (p != 0)
    {
        p->unk7C = 0x33;
        p = p->next;
    }
    p = b;
    while (p != 0)
    {
        p->unk7C = 0x33;
        p = p->next;
    }
    p = partner;
    while (p != 0)
    {
        p->unk7C = 0x33;
        p = p->next;
    }
    p = gWorld->layers;
    while (p != 0)
    {
        if (p->sprite != 0)
            p->sprite->frame = 3;
        p = p->link;
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
        p = p->next;
    }
    p = b;
    while (p != 0)
    {
        p->unk7C = 0x100;
        p = p->next;
    }
    p = partner;
    while (p != 0)
    {
        p->unk7C = 0x100;
        p = p->next;
    }
    p = gWorld->layers;
    while (p != 0)
    {
        if (p->sprite != 0)
            p->sprite->frame = 0x10;
        p = p->link;
    }
}

NONMATCH("asm/code_080C23F4/hero_a_wide_step_finish.s", void hero_a_wide_step_finish(struct HeroBody *body))
{
    struct Hero *hero_a;
    struct HeroBody *body_a;
    struct HeroData *data;
    s32 stepDone;
    s32 quotient;
    u32 lo;
    u32 hi;

    hero_a = gWorld->heroA;
    body_a = &hero_a->body;
    step_motion(body_a);
    lo = body_a->wideLo;
    hi = body_a->wideHi;
    quotient = MATH_ROUTINE(s32 (*)(u32, u32), iwram_div)(hi << 14, lo);
    if (body_a->step > quotient)
        stepDone = 0;
    else
    {
        stepDone = 1;
        if (body_a->progress < quotient)
            stepDone = 0;
    }
    if (stepDone)
    {
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            data = body_a->data;
            if (data->poseIndex == -1)
                sprite_set_action(body_a, 6, 0x2000, 0);
            else
                sprite_set_action(body_a, 6, 0x204D, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
    }
    if ((body_a->flags & 0x20) != 0)
    {
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            sprite_set_action(body_a, 8, 0x2000, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
        effect_spawn_at(0x33, body_a->x / 0x100, body_a->y / 0x100, body_a->z / 0x100, body_a);
        body->state = hero_a_sprite_active_anim8_to_2520;
    }
}
END_NONMATCH

void hero_pair_place_and_finish(struct HeroBody *body)
{
    struct Hero *hero_a;
    struct Hero *hero_b;
    struct HeroBody *body_a;
    struct HeroBody *body_b;
    struct HeroData *data;
    struct HeroSprite *sprite;
    struct HeroSpan *span;
    s32 tx;
    s32 ty;
    s32 th;
    s32 x;
    s32 y;
    s32 h;

    hero_a = gWorld->heroA;
    body_a = &hero_a->body;
    hero_b = gWorld->heroB;
    body_b = &hero_b->body;
    if ((body_a->sprite->flags & 8) != 0)
    {
        if ((hero_a->body.mode & 6) == 2 || (hero_a->body.mode & 6) == 4)
        {
            data = body_a->data;
            tx = data->targetX;
            if (tx < 0)
                tx += 0xFF;
            x = tx >> 8;
            ty = data->targetY;
            if (ty < 0)
                ty += 0xFF;
            y = ty >> 8;
            th = data->targetH;
            if (th < 0)
                th += 0xFF;
            h = th >> 8;
            place_targets(body_a, x, y, h, -1);
            set_motion_start(body_a, 0x400);
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            if (body_a->data->poseIndex == -1)
                sprite_set_action(body_a, 0, 0x2002, 0);
            else
                sprite_set_action(body_a, 0, 0x204F, 0);
            sprite = body_a->sprite;
            {
                s32 flip = 0;
                span = &body_a->span;
                if (span->a >= span->b)
                    flip = 1;
                ((struct HeroMirrorBits *)&sprite->mirror)->flip = flip;
            }
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            body_a->state = 0;
            enter_finish_rebuild(body_a);
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            data = body_b->data;
            tx = data->targetX;
            if (tx < 0)
                tx += 0xFF;
            x = tx >> 8;
            ty = data->targetY;
            if (ty < 0)
                ty += 0xFF;
            y = ty >> 8;
            th = data->targetH;
            if (th < 0)
                th += 0xFF;
            h = th >> 8;
            place_targets(body_b, x, y, h, -1);
            set_motion_start(body_b, 0x400);
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            if (body_b->data->poseIndex == -1)
                sprite_set_action(body_b, 0, 0x2002, 0);
            else
                sprite_set_action(body_b, 0, 0x204F, 0);
            sprite = body_b->sprite;
            {
                s32 flip = 0;
                span = &body_b->span;
                if (span->a >= span->b)
                    flip = 1;
                ((struct HeroMirrorBits *)&sprite->mirror)->flip = flip;
            }
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 0;
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            body_b->state = 0;
            enter_finish_rebuild(body_b);
        }
    }
}

void hero_b_set_step_rate_and_release(struct HeroBody *body)
{
    struct Hero *hero_b;
    struct HeroBody *body_b;

    hero_b = gWorld->heroB;
    body_b = &hero_b->body;
    if ((hero_b->body.mode & 6) == 2 || (hero_b->body.mode & 6) == 4)
    {
        sprite_set_action(body_b, 2, 0x2063, 0);
        ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
    }
    if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
    {
        body_b->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 2);
        body_b->progress = 0;
        ((struct HeroFlagBits *)&body_b->flags)->active = 0;
    }
    body->state = hero_b_active_set_a_step_rate;
}

void hero_b_active_set_a_step_rate(struct HeroBody *body)
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
        set_slot_value(0x7C, -1);
        if ((hero_a->body.mode & 6) == 2 || (hero_a->body.mode & 6) == 4)
        {
            body_a->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0x18);
            body_a->progress = 0;
            ((struct HeroFlagBits *)&body_a->flags)->active = 0;
        }
        body_a->timer = body_a->timer & 0xFFF;
        body->state = hero_a_step_c2850;
    }
}

/* Hero A steps while the world mode bits 6-7 are set: sprite height from the progress, frame and
   layer values at the start of the step, a body part and pose on the A key, then the next state. */
void hero_a_step_c2850(struct HeroBody *body)
{
    struct HeroBody *a;
    struct HeroBody *b;
    struct HeroBody *partner;
    s32 end;
    s32 zero;
    s32 full;
    u32 hi;
    void (*fn)(struct HeroBody *);

    a = &gWorld->heroA->body;
    b = &gWorld->heroB->body;
    partner = body->partner;
    end = 0x4000;
    zero = 0;
    full = 0x4000;
    state_stack_advance_progress(a);
    a->sprite->y = 0x110 - a->progress / 0x40;

    if ((gWorld->modeByte & 0xC0) == 0)
        goto flags;

    if (a->progress < end)
    {
        ((struct HeroFlagBits *)&a->flags)->active = 0;
    }

    if ((a->timer >> 12) != 0)
        goto after_zero;

    if ((gWorld->modeByte & 0xC0) != 0 && step_reached(a, 0))
    {
        if ((gWorld->flags0C & 8) != 0)
            frames_win(a, b, partner);
        else
            frames_full(a, b, partner);
        sprite_objects_draw_obj_tiles(0x4C, 0xC);
    }

    if (step_in_window(a, zero, full))
    {
        hi = gWorld->modeByte & 0xC0;
        if (hi == 0x80)
            set_body_part(a, 1, 0x18, 0, 0);
        else if (hi == 0x40)
            set_body_part(a, 0xFF000000, 0x18, 0, 0);
        if ((gSys.pressed & 1) != 0)
        {
            body_set_pose(7, a, 0x16, 0);
            obj_free_parts(a);
            world_mode_or1();
            if ((((struct HeroWorldModeBits *)&gWorld->modeByte)->m2 & ~1) == 0)
            {
                if ((gWorld->flags0C & 8) != 0)
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
        world_mode_or1();
        a->timer = (a->timer & 0xFFF) | 0x1000;
        if ((gWorld->flags0C & 8) != 0)
            frames_full(a, b, partner);
        else
            frames_full(a, b, partner);
        display_flush();
        obj_free_parts(a);
        goto after_zero;
    }

    if (a->progress > end)
        goto after_zero;

    if (!((gSys.pressed & 1) || (gSys.pressed & 3)))
        goto after_zero;

    world_mode_or1();
    if ((((struct HeroWorldModeBits *)&gWorld->modeByte)->m2 & ~1) != 0)
        goto after_zero;

    a->timer = (a->timer & 0xFFF) | 0x1000;
    if ((gWorld->flags0C & 8) != 0)
        frames_full(a, b, partner);
    else
        frames_full(a, b, partner);
    display_flush();
    obj_free_parts(a);
    body_set_pose(6, a, 0x16, 0);

after_zero:
    {
        s32 tmode = a->timer >> 12;
        if (tmode != 0 && step_in_window(a, zero, full) && tmode > 1)
        {
            a->timer &= 0xFFF;
            fn = hero_pair_mode_and_ready_anims;
            body->state = fn;
            fn(body);
            return;
        }
    }

flags:
    if ((a->flags & 0x20) == 0)
        return;
    a->sprite->y = 0x100;
    if ((a->mode & 6) == 2 || (a->mode & 6) == 4)
    {
        sprite_set_action(a, 2, 0x2034, 0);
        ((struct HeroSpriteBits *)&a->sprite->flags)->mode = 1;
    }
    if ((b->mode & 6) == 2 || (b->mode & 6) == 4)
    {
        sprite_set_action(b, 0xC, 0x2063, 0);
        ((struct HeroSpriteBits *)&b->sprite->flags)->mode = 1;
    }
    body->state = hero_b_sprite_active_anim3_to_2f20;
}
