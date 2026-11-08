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

struct HeroLayer
{
    struct HeroLayer *next;
    u8 filler4[0x08 - 0x04];
    struct HeroSprite *sprite;
};

struct KeyState
{
    u8 filler0[0x3E];
    u16 pressed;
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
    s16 displayTimer;
    u8 filler0F8[0x110 - 0xF8];
    u8 flags110;
    u8 flags111;
    u8 filler112[0x114 - 0x112];
    u32 poseValue;
    u8 filler118[0x11E - 0x118];
    u16 flags11E;
};

struct HeroBody
{
    u8 filler0[0x08];
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
    u8 filler44[0x4C - 0x44];
    void (*state)(struct HeroBody *);
    u32 unk50;
    u8 filler54[0x6C - 0x54];
    void *anim;
    u8 filler70[0x76 - 0x70];
    u8 mode;
    u8 filler77[0x79 - 0x77];
    u8 flags;
    s16 rate;
    u16 frame;
    s16 progress;
    u8 filler80[0x84 - 0x80];
    struct HeroSpan span;
    u8 filler88[0x92 - 0x88];
    s16 height;
    u8 filler94[0xAC - 0x94];
    s16 prevProgress;
    u16 timer;
    u8 filler0B0[0xC3 - 0xB0];
    s8 ofsX;
    s8 ofsY;
    u8 filler0C5[0xCA - 0xC5];
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
    u8 filler0D[0x58 - 0x0D];
    struct Hero *list[6];
    struct Hero *heroA;
    struct Hero *heroB;
    struct HeroLayer *layers;
};

extern struct World *gWorld;
extern u8 *gParty;
extern struct KeyState gSys;
extern const u8 gRoom149Layer1[];

void hero_b_start_step_14th(struct HeroBody *body);
void hero_b_active_partner_anim_place(struct HeroBody *body);
void hero_b_step_window_d72e4(struct HeroBody *body);
void hero_b_link_list_objs(struct HeroBody *body);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void set_slot_value(u32 id, s32 value);
void obj_release(struct HeroBody *obj);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 z, struct HeroBody *owner);
void state_stack_advance_progress(struct HeroBody *body);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void set_motion_start(struct HeroBody *body, u32 speed);
void set_motion_offsets(struct HeroBody *body, u32 value);
void hero_b_sprite_idle_start_step10(struct HeroBody *body);
void hero_b_link_partner_child_fx(struct HeroBody *body);
void hero_pair_active_place_both(struct HeroBody *body);
void hero_b_step_d8164(struct HeroBody *body);
void hero_b_copy_pos(struct HeroBody *body);
void hero_b_active_anim_place(struct HeroBody *body);
void hero_pair_spawn_fx_anim_next(struct HeroBody *body);
void hero_b_active_place_at_partner(struct HeroBody *body);
void set_body_part(struct HeroBody *body, u32 art, s32 x, s32 y, u32 extra);
void body_set_pose(u32 art, struct HeroBody *body, s32 x, s32 y);
void obj_free_parts(struct HeroBody *body);
void display_flush(void);
void sprite_objects_draw_obj_tiles(u32 a, u32 b);
void hero_b_sprite_active_start_step16(struct HeroBody *body);
void step_motion(struct HeroBody *body);
void enter_finish_rebuild(struct HeroBody *body);
void resolve_contact_action(struct HeroBody *body, struct HeroBody *partner, u32 a, u32 b, const u8 *cb);
void hero_pair_active_place_a_finish(struct HeroBody *body);

static inline void chain_speed(struct HeroBody *first, u16 speed)
{
    struct HeroBody *link;

    if (first != 0) {
        link = first;
        do {
            link->frame = speed;
            link = link->child;
        } while (link != 0);
    }
}

static inline void layer_list(u8 layer)
{
    struct HeroLayer *node;

    node = gWorld->layers;
    if (node != 0) {
        do {
            if (node->sprite != 0) {
                node->sprite->layer = layer;
            }
            node = node->next;
        } while (node != 0);
    }
}

static inline void chains_and_layers(struct HeroBody *a, struct HeroBody *b, struct HeroBody *p, u16 speed, u8 layer)
{
    chain_speed(a, speed);
    chain_speed(b, speed);
    chain_speed(p, speed);
    layer_list(layer);
}

static inline void timer_set_mode(struct HeroBody *b, u32 nibble)
{
    b->timer = (b->timer & 0xFFF) | (nibble << 12);
}

static inline void timer_clear_mode(struct HeroBody *b)
{
    b->timer = b->timer & 0xFFF;
}

static inline void world_mode_bits45_set1(void)
{
    u32 m;

    m = gWorld->modeByte;
    gWorld->modeByte = (m & ~0x30) | ((((m << 26) >> 30) | 1) << 4);
}

static inline s32 clamp_0_7fff(s32 v)
{
    if (v < 0) {
        v = 0;
    }
    if (v > 0x7FFF) {
        v = 0x7FFF;
    }
    return v;
}

static inline s32 crossed_limit(struct HeroBody *b, s32 lim)
{
    if (b->prevProgress <= lim && b->progress >= lim) {
        return 1;
    }
    return 0;
}

static inline s32 in_window(struct HeroBody *b, s32 thr)
{
    s32 lo;
    s32 hi;

    lo = thr;
    if (lo < 0) {
        lo = 0;
    }
    if (lo > 0x4000) {
        lo = 0x4000;
    }
    hi = 0x4000;
    if (hi > 0x7FFF) {
        hi = 0x7FFF;
    }
    if (hi < lo) {
        lo = hi;
    }
    if (b->progress >= lo && b->prevProgress <= hi) {
        return 1;
    }
    return 0;
}

static inline s32 body_mode_ready(struct HeroBody *b)
{
    s32 m;

    m = 6;
    m &= b->mode;
    return m == 2 || m == 4;
}

static inline void sprite_mode_set(struct HeroBody *b, u32 mode)
{
    ((struct HeroSpriteBits *)&b->sprite->flags)->mode = mode;
}

NONMATCH("asm/code_080D72E4/hero_b_step_window_d72e4.s", void hero_b_step_window_d72e4(struct HeroBody *body))
{
    struct World *world;
    struct Hero *heroA;
    struct Hero *heroB;
    struct HeroBody *bodyA;
    struct HeroBody *bodyB;
    struct HeroBody *partner;
    s32 thrA;
    s32 thrB;
    s32 lim;
    s32 rate;

    s32 step;

    world = gWorld;
    heroA = world->heroA;
    heroB = world->heroB;
    bodyA = &heroA->body;
    bodyB = &heroB->body;
    partner = body->partner;
    step = 0x4000;
    state_stack_advance_progress(bodyB);

    thrA = step - bodyB->rate * 14;
    thrB = step - bodyB->rate * 12;
    if (bodyB->progress < 0x4000) {
        ((struct HeroFlagBits *)&bodyB->flags)->active = 0;
    }

    do {
        if ((bodyB->timer >> 12) != 0) {
            break;
        }
        lim = clamp_0_7fff(thrA);
        if (crossed_limit(bodyB, lim)) {
            if (gWorld->view & 8) {
                chains_and_layers(bodyA, bodyB, partner, 0x4C, 4);
            } else {
                chains_and_layers(bodyA, bodyB, partner, 0x100, 0x10);
            }
            sprite_objects_draw_obj_tiles(0x4C, 0xC);
        }

        if (in_window(bodyB, thrB)) {
            set_body_part(bodyA, 1, -8, -0x24, 0);
            if (gSys.pressed & 1) {
                body_set_pose(1, bodyA, 0x2E, -8);
                obj_free_parts(bodyA);
                world_mode_bits45_set1();
                if (!(gWorld->modeByte & 8)) {
                    if (gWorld->view & 8) {
                        chains_and_layers(bodyA, bodyB, partner, 0x100, 0x10);
                    } else {
                        chains_and_layers(bodyA, bodyB, partner, 0x100, 0x10);
                    }
                    display_flush();
                }
                timer_set_mode(bodyB, 2);
            }
        }

        if ((bodyB->timer >> 12) != 0) {
            break;
        }

        lim = 0x4000;
        if (lim > 0x7FFF) {
            lim = 0x7FFF;
        }
        if (crossed_limit(bodyB, lim)) {
            world_mode_bits45_set1();
            timer_set_mode(bodyB, 1);
            if (gWorld->view & 8) {
                chains_and_layers(bodyA, bodyB, partner, 0x100, 0x10);
            } else {
                chains_and_layers(bodyA, bodyB, partner, 0x100, 0x10);
            }
            display_flush();
            obj_free_parts(bodyA);
            break;
        }

        if (bodyB->progress > 0x4000) {
            break;
        }
        if (!(gSys.pressed & 1)) {
            if (!(gSys.pressed & 3)) {
                break;
            }
        }
        world_mode_bits45_set1();
        if (gWorld->modeByte & 8) {
            break;
        }
        timer_set_mode(bodyB, 1);
        if (gWorld->view & 8) {
            chains_and_layers(bodyA, bodyB, partner, 0x100, 0x10);
        } else {
            chains_and_layers(bodyA, bodyB, partner, 0x100, 0x10);
        }
        display_flush();
        obj_free_parts(bodyA);
        body_set_pose(6, bodyA, 0x2E, -8);
    } while (0);

    if ((bodyB->timer >> 12) != 0) {
        if (in_window(bodyB, thrB) && (bodyB->timer >> 12) > 1) {
            timer_clear_mode(bodyB);
            body->state = hero_pair_spawn_fx_anim_next;
            body->state(body);
            return;
        }
    }

    lim = clamp_0_7fff(thrB);
    if (crossed_limit(bodyB, lim)) {
        if (body_mode_ready(bodyA)) {
            sprite_set_action(bodyA, 1, 0x2024, 0);
            sprite_mode_set(bodyA, 0);
        }
    }

    if (!(((struct HeroFlagBits *)&bodyB->flags)->active)) {
        return;
    }
    set_slot_value(0xAF, -1);
    if (body_mode_ready(bodyA)) {
        sprite_set_action(bodyA, 3, 0x2024, 0);
        sprite_mode_set(bodyA, 2);
    }
    if (body_mode_ready(bodyB)) {
        rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 6);
        bodyB->rate = rate;
        bodyB->progress = 0;
        ((struct HeroFlagBits *)&bodyB->flags)->active = 0;
    }
    body->state = hero_b_active_place_at_partner;
}
END_NONMATCH
