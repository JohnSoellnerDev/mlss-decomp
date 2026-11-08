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

struct WorldModeBits
{
    u8 low2 : 2;
    u8 f23 : 2;
    u8 f45 : 2;
    u8 top : 2;
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
    u8 filler70[0x74 - 0x70];
    u8 unk74;
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
    s16 height;
    u8 filler94[0xA8 - 0x94];
    u32 pending;
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
void hero_b_link_a(struct HeroBody *body);
void hero_b_unlink_a(struct HeroBody *body);
void hero_b_partner_place_height(struct HeroBody *body);

static inline void chain_slow(struct HeroBody *link)
{
    if (link != 0) {
        do {
            link->speed = 0x40;
            link = link->child;
        } while (link != 0);
    }
}

static inline void chain_fast(struct HeroBody *link)
{
    if (link != 0) {
        do {
            link->speed = 0x100;
            link = link->child;
        } while (link != 0);
    }
}

static inline void layers_4(void)
{
    struct HeroLayer *node = gWorld->layers;
    if (node != 0) {
        do {
            if (node->sprite != 0) {
                node->sprite->layer = 4;
            }
            node = node->next;
        } while (node != 0);
    }
}

static inline void layers_10(void)
{
    struct HeroLayer *node = gWorld->layers;
    if (node != 0) {
        do {
            if (node->sprite != 0) {
                node->sprite->layer = 0x10;
            }
            node = node->next;
        } while (node != 0);
    }
}

static inline s32 clamp_limit(s32 v)
{
    if (v < 0) {
        v = 0;
    }
    if (v > 0x7FFF) {
        v = 0x7FFF;
    }
    return v;
}

static inline u32 window_hit(struct HeroBody *b, s32 limit)
{
    if (b->prevProgress <= limit) {
        if (b->progress >= limit) {
            return 1;
        }
    }
    return 0;
}

static inline u32 limit_hit(struct HeroBody *b, s32 cap, s32 far)
{
    if (b->progress >= cap) {
        if (b->prevProgress <= far) {
            return 1;
        }
    }
    return 0;
}

static inline void timer_set_nibble(u16 *timer, u32 nibble)
{
    *timer = (*timer & 0xFFF) | (nibble << 12);
}

NONMATCH("asm/code_080D8164/hero_b_step_d8164.s", void hero_b_step_d8164(struct HeroBody *body))
{
    struct HeroBody *bodyA;
    struct HeroBody *bodyB;
    struct HeroBody *partner;
    struct HeroBody *next;
    u16 *timer;
    s32 rate;
    s32 t;
    s32 nearLimit;
    s32 farLimit;
    s32 farC;
    s32 cap;
    s32 far2;
    s32 cap2;
    u32 nibble;
    void (*unlink)(struct HeroBody *);

    bodyA = &gWorld->heroA->body;
    bodyB = &gWorld->heroB->body;
    partner = body->partner;
    farLimit = 0x4000;
    cap = 0x4000;
    step_motion(bodyB);

    next = bodyB->child;
    next->posX = bodyB->posX;
    next->posY = bodyB->posY;
    next->posZ = bodyB->posZ;

    rate = bodyB->rate;
    t = rate * 4;
    nearLimit = farLimit - t;
    t = (t + rate) * 2;
    farLimit += t;

    if (bodyB->progress < farLimit) {
        ((struct HeroFlagBits *)&bodyB->flags)->active = 0;
    }

    timer = &bodyB->timer;
    if ((*timer >> 12) != 0) {
        goto part2;
    }

    nearLimit = clamp_limit(nearLimit);
    if (window_hit(bodyB, nearLimit)) {
        if ((gWorld->view & 8) != 0) {
            chain_slow(bodyA);
            chain_slow(bodyB);
            chain_slow(partner);
            layers_4();
        } else {
            chain_fast(bodyA);
            chain_fast(bodyB);
            chain_fast(partner);
            layers_10();
        }
        sprite_objects_draw_obj_tiles(0x4C, 0xC);
    }

    farC = farLimit;
    if (farC < 0) {
        farC = 0;
    }
    if (cap > 0x4000) {
        cap = 0x4000;
    }
    if (farC > 0x7FFF) {
        farC = 0x7FFF;
    }
    if (farC < cap) {
        cap = farC;
    }

    if (limit_hit(bodyB, cap, farC)) {
        set_body_part(bodyB, 2, 0x18, 0, 0);
        if ((gSys.pressed & 2) != 0) {
            body_set_pose(2, bodyB, 0x2E, 0);
            obj_free_parts(bodyB);
            ((struct WorldModeBits *)&gWorld->modeByte)->f45 |= 2;
            if ((((struct WorldModeBits *)&gWorld->modeByte)->f23 & -3) == 0) {
                if ((gWorld->view & 8) != 0) {
                    chain_fast(bodyA);
                    chain_fast(bodyB);
                    chain_fast(partner);
                    layers_10();
                } else {
                    chain_fast(bodyA);
                    chain_fast(bodyB);
                    chain_fast(partner);
                    layers_10();
                }
                display_flush();
            }
            *timer = (*timer & 0xFFF) | 0x2000;
        }
    }

    if ((*timer >> 12) != 0) {
        goto part2;
    }

    far2 = clamp_limit(farLimit);
    if (window_hit(bodyB, far2)) {
        ((struct WorldModeBits *)&gWorld->modeByte)->f45 |= 2;
        *timer = (*timer & 0xFFF) | 0x1000;
        if ((gWorld->view & 8) != 0) {
            chain_fast(bodyA);
            chain_fast(bodyB);
            chain_fast(partner);
            layers_10();
        } else {
            chain_fast(bodyA);
            chain_fast(bodyB);
            chain_fast(partner);
            layers_10();
        }
        display_flush();
        obj_free_parts(bodyB);
        goto part2;
    }
    if (bodyB->progress > farLimit) {
        goto part2;
    }
    if ((gSys.pressed & 3) == 0) {
        goto part2;
    }
    ((struct WorldModeBits *)&gWorld->modeByte)->f45 |= 2;
    if ((((struct WorldModeBits *)&gWorld->modeByte)->f23 & -3) != 0) {
        goto part2;
    }
    *timer = (*timer & 0xFFF) | 0x1000;
    if ((gWorld->view & 8) != 0) {
        chain_fast(bodyA);
        chain_fast(bodyB);
        chain_fast(partner);
        layers_10();
    } else {
        chain_fast(bodyA);
        chain_fast(bodyB);
        chain_fast(partner);
        layers_10();
    }
    display_flush();
    obj_free_parts(bodyB);
    body_set_pose(6, bodyB, 0x2E, 0);

part2:
    nibble = *timer >> 12;
    if (nibble != 0) {
        cap2 = 0x4000;
        far2 = farLimit;
        if (far2 < 0) {
            far2 = 0;
        }
        if (cap2 > 0x4000) {
            cap2 = 0x4000;
        }
        if (far2 > 0x7FFF) {
            far2 = 0x7FFF;
        }
        if (far2 < cap2) {
            cap2 = far2;
        }
        if (limit_hit(bodyB, cap2, far2)) {
            if (nibble > 1) {
                *timer = *timer & 0xFFF;
                unlink = hero_b_unlink_a;
                body->state = unlink;
                unlink(body);
                return;
            }
        }
    }

    if ((bodyB->flags & 0x20) == 0) {
        return;
    }
    set_slot_value(0x9B, -1);

    if (partner->targetZ == 0) {
        if ((partner->mode & 6) == 2 || (partner->mode & 6) == 4) {
            partner->state = hero_b_link_a;
        }
    }

    if ((bodyB->mode & 6) == 2 || (bodyB->mode & 6) == 4) {
        sprite_set_action(bodyB, 0x12, 0x2066, 0);
        ((struct HeroSpriteBits *)&bodyB->sprite->flags)->mode = 2;
    }

    next = bodyB->child;
    if (next != 0) {
        sprite_set_action(next, 0x12, 0x2095, 0);
        ((struct HeroSpriteBits *)&next->sprite->flags)->mode = 0;
    }

    bodyB->unk74 = 0;
    bodyB->unk75 = 0;
    if ((bodyB->mode & 6) == 2 || (bodyB->mode & 6) == 4) {
        rate = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(0x4000, 8);
        bodyB->rate = rate;
        bodyB->progress = 0;
        ((struct HeroFlagBits *)&bodyB->flags)->active = 0;
    }
    partner->pending = 1;
    body->state = hero_b_partner_place_height;
}
END_NONMATCH
