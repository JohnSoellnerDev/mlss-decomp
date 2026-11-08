#include "global.h"
#include "ram_routines.h"

struct HeroSprite
{
    u8 filler0[0x06];
    u16 scale;
    u8 filler08[0x11 - 0x08];
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

struct HeroSpriteFlag4Bits
{
    u8 unk0 : 4;
    u8 bit4 : 1;
    u8 unk5 : 3;
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
    u8 filler0E4[0xEC - 0xE4];
    s16 poseIndex;
    u8 filler0EE[0xF6 - 0xEE];
    s16 flagsF6;
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
    s32 posX;
    s32 posY;
    s32 posZ;
    u8 filler1C[0x24 - 0x1C];
    u16 count;
    u8 filler26[0x28 - 0x26];
    struct HeroData *data;
    struct HeroBody *partner;
    struct HeroBody *chain;
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
    u16 rate;
    u16 frame;
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
    u8 flags0C;
    u8 filler0D;
    u8 enableByte;
    u8 filler0F[0x70 - 0x0F];
    struct Hero *heroA;
    struct Hero *heroB;
    struct HeroBody *layers;
    u8 filler7C[0x24C - 0x7C];
    void *trackedObj;
};

extern struct World *gWorld;
extern u8 *gParty;

void state_stack_advance_progress(struct HeroBody *body);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void set_slot_value(u32 id, s32 value);
void step_motion(struct HeroBody *body);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void set_motion_start(struct HeroBody *body, u32 speed);
void enter_finish_rebuild(struct HeroBody *body);
void schedule_pose_task(struct HeroBody *obj);
void set_motion_speed_aux(struct HeroBody *body, u32 speed, u32 aux);
void set_motion_offsets(struct HeroBody *body, u32 offsets);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 h, struct HeroBody *owner);
u32 find_frame_entry(struct HeroBody *obj, u32 index);
s32 resolve_contact_action(struct HeroBody *body, struct HeroBody *partner, u32 a, u32 b, u32 cb);
void obj_slot_update(struct HeroBody *obj);

void hero_a_active_copy_pos_fx(struct HeroBody *body);
void hero_a_step_16_b_idle_alt(struct HeroBody *body);
void hero_b_active_place_at_a_pose(struct HeroBody *body);

void hero_a_link_to_partner_fx(struct HeroBody *body);
void hero_a_active_set_targets(struct HeroBody *body);
void hero_a_active_fx_mirror(struct HeroBody *body);
void hero_pair_place_finish_sprite(struct HeroBody *body);
void hero_a_on_partner(struct HeroBody *body);
void hero_a_active_step_12(struct HeroBody *body);
void hero_a_active_follow_b_aux(struct HeroBody *body);
void hero_a_step_threshold_c16c(struct HeroBody *body);
void hero_a_sprite_set_targets(struct HeroBody *body);
void hero_a_step_stage_fx(struct HeroBody *body);
void active_targets_hero_pair_place_finish_b(struct HeroBody *body);

void hero_b_pose_anim(struct HeroBody *body);
void noop_state_080acec0(struct HeroBody *body);
void hero_b_on_object(struct HeroBody *body);

struct SysKeys3E
{
    u8 filler0[0x3E];
    u16 keys;
};

extern u8 gSys[];

struct StepWorldMode
{
    u8 low2 : 2;
    u8 mid2 : 2;
    u8 mode : 2;
    u8 high2 : 2;
} __attribute__((packed));

struct StepFlagBits
{
    u8 unk0 : 5;
    u8 active : 1;
    u8 unk6 : 2;
} __attribute__((packed));

struct StepSpriteBits
{
    u8 unk0 : 1;
    u8 mode : 2;
    u8 unk3 : 5;
} __attribute__((packed));

struct StepCcBits
{
    u8 low : 7;
    u8 high : 1;
} __attribute__((packed));

void set_body_part(struct HeroBody *body, u32 art, s32 x, s32 y, u32 extra);
void body_set_pose(u32 art, struct HeroBody *body, s32 x, s32 y);
void obj_free_parts(struct HeroBody *body);
void display_flush(void);
void sprite_objects_draw_obj_tiles(u32 a, u32 b);
u32 step_motion_scaled(struct HeroBody *body);

/* Step has not passed the mark x (clamped to 0..0x7FFF) and the progress has reached it. */
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

/* Progress at or past lo and step not past hi (both clamped, lo at most 0x4000 and hi). */
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
        p = p->chain;
    }
    p = b;
    while (p != 0)
    {
        p->frame = 0x33;
        p = p->chain;
    }
    p = partner;
    while (p != 0)
    {
        p->frame = 0x33;
        p = p->chain;
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
        p->frame = 0x100;
        p = p->chain;
    }
    p = b;
    while (p != 0)
    {
        p->frame = 0x100;
        p = p->chain;
    }
    p = partner;
    while (p != 0)
    {
        p->frame = 0x100;
        p = p->chain;
    }
    p = gWorld->layers;
    while (p != 0)
    {
        if (p->sprite != 0)
            p->sprite->frame = 0x10;
        p = p->next;
    }
}

static inline void world_mode_or1(void)
{
    ((struct StepWorldMode *)&gWorld->modeByte)->mode |= 1;
}

/* Reset the contact bits and scale, then start the contact action on the partner. */
static inline s32 link_partner(struct HeroBody *a, struct HeroBody *partner, u32 cb)
{
    ((struct StepCcBits *)&a->cc)->low = 0;
    a->scale = 0x100;
    return resolve_contact_action(a, partner, 0, 0, cb);
}

/* Hero A step marks: frame values before the end of the step, a pose on the A key in the window,
   timer mode 1/2 at the end or on A/B, then the contact action with the partner and the next state. */
void hero_a_step_threshold_c16c(struct HeroBody *body)
{
    struct HeroBody *a;
    struct HeroBody *b;
    struct HeroBody *partner;
    s32 end;
    s32 full;
    s32 rate;
    s32 lead6;
    s32 lead4;
    u32 cb;
    s32 link;

    a = &gWorld->heroA->body;
    b = &gWorld->heroB->body;
    partner = body->partner;
    end = 0x4000;
    full = 0x4000;
    step_motion_scaled(a);
    rate = (s16)a->rate;
    lead6 = end - (((rate << 1) + rate) << 1);
    lead4 = end - (rate << 2);
    end += rate << 1;

    if (a->progress < end)
        ((struct StepFlagBits *)&a->flags)->active = 0;

    if ((a->timer >> 12) != 0)
        goto after_zero;

    if (step_reached(a, lead6))
    {
        if ((gWorld->flags0C & 8) != 0)
            frames_win(a, b, partner);
        else
            frames_full(a, b, partner);
        sprite_objects_draw_obj_tiles(0x4C, 0xC);
    }

    if (step_in_window(a, lead4, end))
    {
        set_body_part(a, 1, 0x18, 0, 0);
        if ((((struct SysKeys3E *)gSys)->keys & 1) != 0)
        {
            body_set_pose(2, a, 0x2E, 0);
            obj_free_parts(a);
            world_mode_or1();
            if ((((struct StepWorldMode *)&gWorld->modeByte)->mid2 & ~1) == 0)
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

    if ((((struct SysKeys3E *)gSys)->keys & 1) == 0)
    {
        if ((((struct SysKeys3E *)gSys)->keys & 3) == 0)
            goto after_zero;
    }

    world_mode_or1();
    if ((((struct StepWorldMode *)&gWorld->modeByte)->mid2 & ~1) != 0)
        goto after_zero;

    a->timer = (a->timer & 0xFFF) | 0x1000;
    if ((gWorld->flags0C & 8) != 0)
        frames_full(a, b, partner);
    else
        frames_full(a, b, partner);
    display_flush();
    obj_free_parts(a);
    body_set_pose(6, a, 0x2E, 0);

after_zero:
    {
        s32 tmode = a->timer >> 12;

        if (tmode != 0 && step_in_window(a, full, end))
        {
            if (tmode > 1)
            {
                cb = 0x040B1888;
                link = link_partner(a, partner, cb);
            }
            else
            {
                cb = 0x040B1788;
                link = link_partner(a, partner, cb);
            }
            a->count++;
            if (!(cb & 0x10000) && link == 1)
            {
                obj_slot_update(a);
                return;
            }
            if (tmode > 1)
                effect_spawn_at(0x2E47, a->posX / 0x100, a->posY / 0x100, a->posZ / 0x100, partner);
            else
                effect_spawn_at(0x70B, a->posX / 0x100, a->posY / 0x100, a->posZ / 0x100, partner);
            a->timer &= 0xFFF;
            ((struct StepFlagBits *)&a->flags)->active = 1;
        }
    }

    if (((struct StepFlagBits *)&a->flags)->active == 0)
        return;
    effect_spawn_at(0x2FF9, a->posX / 0x100, a->posY / 0x100, a->posZ / 0x100, a);
    if ((a->mode & 6) == 2 || (a->mode & 6) == 4)
    {
        sprite_set_action(a, 0xB, 0x2030, 0);
        ((struct StepSpriteBits *)&a->sprite->flags)->mode = 0;
    }
    if ((b->mode & 6) == 2 || (b->mode & 6) == 4)
    {
        sprite_set_action(b, 0, 0x204D, 0);
        ((struct StepSpriteBits *)&b->sprite->flags)->mode = 0;
    }
    body->state = hero_a_sprite_set_targets;
}

void hero_a_sprite_set_targets(struct HeroBody *body)
{
    struct Hero *hero_a = gWorld->heroA;
    struct HeroBody *body_a = &hero_a->body;

    if ((body_a->sprite->flags & 8) != 0)
    {
        set_slot_value(0x2B, -1);
        if ((hero_a->body.mode & 6) == 2 || (hero_a->body.mode & 6) == 4)
        {
            place_targets(body_a,
                          (body_a->targetX / 0x100) + body_a->ofsX - 0x3C,
                          (body_a->targetY / 0x100) + body_a->ofsY, 0, -1);
            body_a->height = (body_a->targetZ / 0x100) + 0x28;
            set_motion_offsets(body_a, 0x500);
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            sprite_set_action(body_a, 0xA, 0x2030, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        body->state = hero_a_step_stage_fx;
    }
}

/* Hero A sprite actions at a third and two thirds of the step, then the effect and the next state
   once the body is active. */
void hero_a_step_stage_fx(struct HeroBody *body)
{
    struct HeroBody *a;

    a = &gWorld->heroA->body;
    step_motion(a);
    if (step_reached(a, 0x1555))
    {
        if ((a->mode & 6) == 2 || (a->mode & 6) == 4)
        {
            sprite_set_action(a, 9, 0x2030, 0);
            ((struct HeroSpriteBits *)&a->sprite->flags)->mode = 0;
        }
    }
    if (step_reached(a, 0x2AAA))
    {
        if ((a->mode & 6) == 2 || (a->mode & 6) == 4)
        {
            sprite_set_action(a, 3, 0x2030, 0);
            ((struct HeroSpriteBits *)&a->sprite->flags)->mode = 0;
        }
    }
    if (a->flags & 0x20)
    {
        set_slot_value(0x2E, -1);
        effect_spawn_at(0x1E, a->posX / 0x100, a->posY / 0x100, a->posZ / 0x100, a);
        if ((a->mode & 6) == 2 || (a->mode & 6) == 4)
        {
            sprite_set_action(a, 8, 0x2034, 0);
            ((struct HeroSpriteBits *)&a->sprite->flags)->mode = 1;
        }
        body->state = active_targets_hero_pair_place_finish_b;
    }
}
