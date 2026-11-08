#include "global.h"
#include "ram_routines.h"

struct HeroSprite
{
    u8 filler0[0x06];
    u16 unk6;
    u8 filler08[0x11 - 0x08];
    u8 mirror;
    u8 flags;
    u8 filler13[0x20 - 0x13];
    u8 frame;
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

struct HeroMirrorBits
{
    u8 unk0 : 6;
    u8 flip : 1;
    u8 unk7 : 1;
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
    u8 filler70[0x75 - 0x70];
    u8 unk75;
    u8 mode;
    u8 filler77[0x79 - 0x77];
    u8 flags;
    s16 rate;
    u16 frame;
    s16 progress;
    u8 filler80[0x84 - 0x80];
    struct HeroSpan span;
    u8 filler88[0x92 - 0x88];
    u16 height;
    u32 unk94;
    u32 unk98;
    u32 unk9C;
    u8 filler0A0[0xA4 - 0xA0];
    u32 unkA4;
    u8 filler0A8[0xAC - 0xA8];
    s16 step;
    u16 timer;
    s16 jitterX;
    s16 jitterY;
    u8 filler0B4[0xC3 - 0xB4];
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
    u8 mode;
    u8 filler0B;
    u8 flags;
    u8 filler0D[0x70 - 0x0D];
    struct Hero *heroA;
    struct Hero *heroB;
    struct SpriteNode *sprites;
};

struct SpriteNode
{
    struct SpriteNode *next;
    u8 filler4[0x08 - 0x04];
    struct HeroSprite *sprite;
};

struct WorldModeBits
{
    u8 lo4 : 4;
    u8 mid : 2;
    u8 hi : 2;
};

struct SysKeys3E
{
    u8 filler0[0x3E];
    u16 keys;
};

extern struct World *gWorld;
extern u8 gSys[];

void state_stack_advance_progress(struct HeroBody *body);
void set_slot_value(u32 id, s32 value);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 z, struct HeroBody *owner);

void hero_b_sprite_active_start_step(struct HeroBody *body);
void hero_a_active_place_partner_fx(struct HeroBody *body);
void hero_b_active_fx_a_step_rate(struct HeroBody *body);
void hero_a_active_place_b_anim_fx(struct HeroBody *body);
void hero_a_step_range_pose_fx(struct HeroBody *body);
void hero_pair_finish_place_both(struct HeroBody *body);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void set_motion_offsets(struct HeroBody *body, u32 value);
void set_motion_start(struct HeroBody *body, u32 speed);
void step_motion(struct HeroBody *body);
void resolve_contact_action(struct HeroBody *a, struct HeroBody *b, u32 c, u32 d, u32 e);
void hero_b_step_link_partner_fx(struct HeroBody *body);
void hero_a_sprite_active_target_or_next(struct HeroBody *body);
void hero_pair_pose_a_anim(struct HeroBody *body);
void hero_b_step_window_a568(struct HeroBody *body);
void enter_finish_rebuild(struct HeroBody *body);
void set_body_part(struct HeroBody *body, u32 art, s32 x, s32 y, u32 extra);
void body_set_pose(u32 art, struct HeroBody *body, s32 x, s32 y);
void obj_free_parts(struct HeroBody *body);
void display_flush(void);
void sprite_objects_draw_obj_tiles(u32 a, u32 b);
void hero_b_anim_start_step2(struct HeroBody *body);
void hero_b_active_fx_a_anim(struct HeroBody *body);

static inline void world_mode_or2(void)
{
    ((struct WorldModeBits *)&gWorld->mode)->mid |= 2;
}

static inline void timer_set_mode(u16 *timer, u32 mode)
{
    *timer = (*timer & 0xFFF) | (mode << 12);
}

static inline void chain_frame_set(struct HeroBody *p, u16 frame)
{
    for (; p != 0; p = p->next)
        p->frame = frame;
}

static inline void sprite_layer_set(u8 layer)
{
    struct SpriteNode *node;
    for (node = gWorld->sprites; node != 0; node = node->next)
        if (node->sprite != 0)
            node->sprite->frame = layer;
}

static inline void frames_set(struct HeroBody *a, struct HeroBody *b, struct HeroBody *c, u16 chain, u8 layer)
{
    chain_frame_set(a, chain);
    chain_frame_set(b, chain);
    chain_frame_set(c, chain);
    sprite_layer_set(layer);
}

NONMATCH("asm/code_080C94A0/hero_b_step_c94a0.s", void hero_b_step_c94a0(struct HeroBody *body))
{
    struct HeroBody *bodyA;
    struct HeroBody *bodyB;
    struct HeroBody *partner;
    struct HeroBody *p;
    struct SpriteNode *node;
    u8 *heroB;
    u16 *timer;
    s32 zero;
    s32 stepMax;
    s32 lo;
    s32 hi;
    s32 limit;
    s32 inWin;
    s32 near;
    s32 reached;
    s32 tmode;
    s32 mode;
    s32 (*div)(s32, s32);
    void (*next)(struct HeroBody *);

    stepMax = 0x7FFF;
    bodyA = &gWorld->heroA->body;
    heroB = (u8 *)gWorld->heroB;
    bodyB = (struct HeroBody *)(heroB + 8);
    partner = body->partner;
    state_stack_advance_progress(bodyB);

    if (*(s16 *)(heroB + 0x86) < 0x4000)
        ((struct HeroFlagBits *)(heroB + 0x81))->active = 0;

    timer = (u16 *)(heroB + 0xB6);
    if ((*timer >> 12) != 0)
        goto tail;

    zero = 0;
    near = zero;
    if (*(s16 *)(heroB + 0xB4) > zero)
        near = zero;
    else if (*(s16 *)(heroB + 0x86) >= zero)
        near = 1;
    else
        near = zero;
    timer = (u16 *)((u8 *)bodyB + 0xAE);
    if (near != 0) {
        if (gWorld->flags & 8) {
            for (p = bodyA; p != 0; p = p->next)
                p->frame = 0x66;
            for (p = bodyB; p != 0; p = p->next)
                p->frame = 0x66;
            for (p = partner; p != 0; p = p->next)
                p->frame = 0x66;
            for (node = gWorld->sprites; node != 0; node = node->next)
                if (node->sprite != 0)
                    node->sprite->frame = 6;
        } else {
            for (p = bodyA; p != 0; p = p->next)
                p->frame = 0x100;
            for (p = bodyB; p != 0; p = p->next)
                p->frame = 0x100;
            for (p = partner; p != 0; p = p->next)
                p->frame = 0x100;
            for (node = gWorld->sprites; node != 0; node = node->next)
                if (node->sprite != 0)
                    node->sprite->frame = 0x10;
        }
        sprite_objects_draw_obj_tiles(0x4C, 0xC);
    }

    lo = zero;
    hi = 0x4000;
    if (lo > 0x4000)
        lo = 0x4000;
    if (hi > stepMax)
        hi = stepMax;
    if (hi < lo)
        lo = hi;

    inWin = zero;
    if (bodyB->progress >= lo && bodyB->step <= hi)
        inWin = 1;

    if (inWin != 0) {
        set_body_part(bodyB, 2, 0, 0x14, 0);
        if ((((struct SysKeys3E *)gSys)->keys & 2) != 0) {
            body_set_pose(3, bodyB, 0, -0x28);
            obj_free_parts(bodyB);
            ((struct WorldModeBits *)&gWorld->mode)->mid |= 2;
            if ((gWorld->mode & 4) == 0) {
                for (p = bodyA; p != 0; p = p->next)
                    p->frame = 0x100;
                for (p = bodyB; p != 0; p = p->next)
                    p->frame = 0x100;
                for (p = partner; p != 0; p = p->next)
                    p->frame = 0x100;
                for (node = gWorld->sprites; node != 0; node = node->next)
                    if (node->sprite != 0)
                        node->sprite->frame = 0x10;
                display_flush();
            }
            *timer = (*timer & 0xFFF) | 0x2000;
        }
    }

    if ((*timer >> 12) != 0)
        goto tail;

    limit = 0x4000;
    if (limit > stepMax)
        limit = stepMax;
    reached = zero;
    if (bodyB->step <= limit && bodyB->progress >= limit)
        reached = 1;

    if (reached != 0) {
        ((struct WorldModeBits *)&gWorld->mode)->mid |= 2;
        *timer = (*timer & 0xFFF) | 0x1000;
        for (p = bodyA; p != 0; p = p->next)
            p->frame = 0x100;
        for (p = bodyB; p != 0; p = p->next)
            p->frame = 0x100;
        for (p = partner; p != 0; p = p->next)
            p->frame = 0x100;
        for (node = gWorld->sprites; node != 0; node = node->next)
            if (node->sprite != 0)
                node->sprite->frame = 0x10;
        display_flush();
        obj_free_parts(bodyB);
        goto tail;
    }

    if (bodyB->progress > 0x4000)
        goto tail;
    if ((((struct SysKeys3E *)gSys)->keys & 3) == 0)
        goto tail;

    ((struct WorldModeBits *)&gWorld->mode)->mid |= 2;
    if (gWorld->mode & 4)
        goto tail;
    *timer = (*timer & 0xFFF) | 0x1000;
    for (p = bodyA; p != 0; p = p->next)
        p->frame = 0x100;
    for (p = bodyB; p != 0; p = p->next)
        p->frame = 0x100;
    for (p = partner; p != 0; p = p->next)
        p->frame = 0x100;
    for (node = gWorld->sprites; node != 0; node = node->next)
        if (node->sprite != 0)
            node->sprite->frame = 0x10;
    display_flush();
    obj_free_parts(bodyB);
    body_set_pose(6, bodyB, 0, -0x28);

tail:
    tmode = *timer >> 12;
    if (tmode != 0) {
        lo = zero;
        hi = 0x4000;
        if (lo > 0x4000)
            lo = 0x4000;
        if (hi > stepMax)
            hi = stepMax;
        if (hi < lo)
            lo = hi;

        inWin = zero;
        if (bodyB->progress >= lo && bodyB->step <= hi)
            inWin = 1;

        if (inWin != 0 && tmode > 1) {
            *timer = *timer & 0xFFF;
            next = hero_b_anim_start_step2;
            body->state = next;
            next(body);
            return;
        }
    }

    if ((bodyB->flags & 0x20) == 0)
        return;

    mode = bodyB->mode & 6;
    if (mode == 2 || mode == 4) {
        sprite_set_action(bodyB, 6, 0x2063, 0);
        ((struct HeroSpriteBits *)&bodyB->sprite->flags)->mode = 1;
    }

    mode = bodyB->mode & 6;
    if (mode == 2 || mode == 4) {
        div = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div);
        bodyB->rate = div(0x4000, 7);
        bodyB->progress = 0;
        ((struct HeroFlagBits *)&bodyB->flags)->active = 0;
    }
    body->state = hero_b_active_fx_a_anim;
}
END_NONMATCH
