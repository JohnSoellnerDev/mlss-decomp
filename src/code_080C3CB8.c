#include "global.h"
#include "ram_routines.h"

struct HeroSprite
{
    u8 filler0[0x06];
    u16 unk06;
    u8 filler08[0x11 - 0x08];
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
    u8 filler0B[0x0C - 0x0B];
    u8 flags;
    u8 filler0D[0x58 - 0x0D];
    struct Hero *list[6];
    struct Hero *heroA;
    struct Hero *heroB;
    struct HeroLayer *layers;
};

extern struct World *gWorld;

void step_motion(struct HeroBody *body);
void set_slot_value(u32 id, s32 value);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 z, struct HeroBody *owner);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void hero_pair_place_and_finish_alt(struct HeroBody *body);
void hero_pair_place_and_finish_alt2(struct HeroBody *body);
void hero_b_sprite_active_anim3_to_3bec(struct HeroBody *body);
void hero_a_step_window_2fec(struct HeroBody *body);
void hero_a_walk_to_partner_3cb8(struct HeroBody *body);
void hero_a_active_fx_anim_ready(struct HeroBody *body);
void hero_a_active_fx_anim_ready_alt(struct HeroBody *body);
void hero_a_active_link_place_finish_alt(struct HeroBody *body);
void set_motion_start(struct HeroBody *body, u32 speed);
void enter_finish_rebuild(struct HeroBody *body);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void set_motion_offsets(struct HeroBody *body, u32 value);
void resolve_contact_action(struct HeroBody *body, struct HeroBody *partner, u32 a, u32 b, const u8 *cb);

void state_stack_advance_progress(struct HeroBody *body);
void display_flush(void);
extern u8 gSys[];
void obj_free_parts(struct HeroBody *body);
void body_set_pose(u32 art, struct HeroBody *body, s32 x, s32 y);
void set_body_part(struct HeroBody *body, u32 art, s32 x, s32 y, u32 extra);
void sprite_objects_draw_obj_tiles(u32 x, u32 y);
void hero_a_jitter_place_near_partner(struct HeroBody *body);

struct HeroTimerBits
{
    u16 count : 12;
    u16 mode : 4;
};

struct HeroWorldModeBits
{
    u8 lo2 : 2;
    u8 m2 : 2;
    u8 mode : 2;
    u8 hi2 : 2;
} __attribute__((packed));

struct HeroModeBits
{
    u8 unk0 : 1;
    u8 mode : 2;
    u8 unk3 : 5;
};

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

static inline void set_timer_mode(struct HeroBody *b, u32 mode)
{
    b->timer = (b->timer & 0xFFF) | (mode << 12);
}

static inline void world_mode_or2(void)
{
    ((struct HeroWorldModeBits *)&gWorld->modeByte)->mode |= 2;
}

static inline void chain_win(struct HeroBody *b)
{
    while (b != 0)
    {
        b->frame = 0x33;
        b = b->child;
    }
}

static inline void chain_full(struct HeroBody *b)
{
    while (b != 0)
    {
        b->frame = 0x100;
        b = b->child;
    }
}

static inline void nodes_win(void)
{
    struct HeroLayer *l = gWorld->layers;
    while (l != 0)
    {
        if (l->sprite != 0)
            l->sprite->layer = 3;
        l = l->next;
    }
}

static inline void nodes_low(void)
{
    struct HeroLayer *l = gWorld->layers;
    while (l != 0)
    {
        if (l->sprite != 0)
            l->sprite->layer = 0x10;
        l = l->next;
    }
}

static inline void frames_full(struct HeroBody *a, struct HeroBody *b, struct HeroBody *p)
{
    chain_full(a);
    chain_full(b);
    chain_full(p);
    nodes_low();
}

static inline void frames_win(struct HeroBody *a, struct HeroBody *b, struct HeroBody *p)
{
    chain_win(a);
    chain_win(b);
    chain_win(p);
    nodes_win();
}

NONMATCH("asm/code_080C3CB8/hero_a_walk_to_partner_3cb8.s", void hero_a_walk_to_partner_3cb8(struct HeroBody *body))
{
    struct HeroBody *a;
    struct HeroBody *b;
    struct HeroBody *partner;
    s32 end;
    s32 rate;
    s32 lead10;
    s32 lead8;
    s32 window;
    s32 thresh;
    s32 endc;
    s32 limit;
    s32 arrive;
    s32 x;
    s32 y;
    s32 z;
    u32 inWin;
    u32 due;
    u32 done;
    u32 m;
    void (*fn)(struct HeroBody *);

    a = &gWorld->heroA->body;
    b = &gWorld->heroB->body;
    partner = body->partner;
    end = 0x4000;
    state_stack_advance_progress(a);
    rate = a->rate;
    lead10 = end - (((rate << 2) + rate) << 1);
    lead8 = end - (rate << 3);
    end += rate << 1;

    if (a->progress < end)
    {
        ((struct HeroActiveBits *)&a->flags)->active = 0;
    }

    if ((a->timer >> 12) != 0)
        goto after_zero;

    window = lead10;
    if (window < 0)
        window = 0;
    if (window > 0x7FFF)
        window = 0x7FFF;
    inWin = 0;
    if (a->step <= window)
    {
        inWin = 1;
        if (a->progress < window)
            inWin = 0;
    }

    if (inWin)
    {
        if ((gWorld->flags & 8) != 0)
            frames_win(a, b, partner);
        else
            frames_full(a, b, partner);
        sprite_objects_draw_obj_tiles(0x4C, 0xC);
    }

    thresh = lead8;
    endc = end;
    if (thresh < 0)
        thresh = 0;
    if (endc < 0)
        endc = 0;
    if (thresh > 0x4000)
        thresh = 0x4000;
    if (endc > 0x7FFF)
        endc = 0x7FFF;
    if (endc < thresh)
        thresh = endc;

    due = 0;
    if (a->progress >= thresh)
    {
        due = 1;
        if (a->step > endc)
            due = 0;
    }

    if (due)
    {
        set_body_part(b, 2, 0x18, 0, 0);
        if ((((struct HeroSysKeys *)gSys)->keys & 2) != 0)
        {
            body_set_pose(1, b, 0, -0x28);
            obj_free_parts(b);
            world_mode_or2();
            if (((struct HeroWorldModeBits *)&gWorld->modeByte)->m2 == 0)
            {
                if ((gWorld->flags & 8) != 0)
                    frames_full(a, b, partner);
                else
                    frames_full(a, b, partner);
                display_flush();
            }
            set_timer_mode(a, 2);
        }
    }

    if ((a->timer >> 12) != 0)
        goto after_zero;

    limit = end;
    if (limit < 0)
        limit = 0;
    if (limit > 0x7FFF)
        limit = 0x7FFF;
    done = 0;
    if (a->step <= limit)
    {
        done = 1;
        if (a->progress < limit)
            done = 0;
    }

    if (done)
    {
        world_mode_or2();
        set_timer_mode(a, 1);
        if ((gWorld->flags & 8) != 0)
            frames_full(a, b, partner);
        else
            frames_full(a, b, partner);
        display_flush();
        obj_free_parts(b);
        goto after_zero;
    }

    if (a->progress > end)
        goto after_zero;

    if ((((struct HeroSysKeys *)gSys)->keys & 3) == 0)
        goto after_zero;

    world_mode_or2();
    if ((((struct HeroWorldModeBits *)&gWorld->modeByte)->m2 & ~2) != 0)
        goto after_zero;

    set_timer_mode(a, 1);
    if ((gWorld->flags & 8) != 0)
        frames_full(a, b, partner);
    else
        frames_full(a, b, partner);
    display_flush();
    obj_free_parts(b);
    body_set_pose(6, b, 0, -0x28);

after_zero:
    {
        s32 tmode = a->timer >> 12;
        if (tmode != 0)
        {
            arrive = end;
            if (arrive < 0)
                arrive = 0;
            if (arrive > 0x7FFF)
                arrive = 0x7FFF;
            limit = 0x4000;
            if (arrive < limit)
                limit = arrive;
            due = 0;
            if (a->progress >= limit)
            {
                due = 1;
                if (a->step > arrive)
                    due = 0;
            }
            if (due && tmode > 1)
            {
                set_timer_mode(a, 0);
                fn = hero_a_jitter_place_near_partner;
                body->state = fn;
                fn(body);
                return;
            }
        }
    }

    if ((a->flags & 0x20) == 0)
        return;

    set_slot_value(0x7D, -1);
    m = a->mode & 6;
    if (m == 2 || m == 4)
    {
        x = partner->targetX / 0x100 + ((s8)partner->ofsX) - 10;
        y = partner->targetY / 0x100 + ((s8)partner->ofsY) + 3;
        z = partner->targetZ / 0x100 + ((s8)partner->ofsZ) - 0x15;
        place_targets(a, x, y, z, -1);
        set_motion_start(a, 0x600);
    }
    m = a->mode & 6;
    if (m == 2 || m == 4)
    {
        sprite_set_action(a, 5, 0x2034, 0);
        a->sprite->flags &= ~7;
    }
    body->state = hero_a_active_link_place_finish_alt;
}
END_NONMATCH

void hero_a_active_link_place_finish_alt(struct HeroBody *body)
{
    struct HeroBody *body_a = &gWorld->heroA->body;
    struct HeroBody *partner = body->partner;

    step_motion(body_a);
    if ((body_a->flags & 0x20) != 0)
    {
        set_slot_value(0x63, -1);
        ((struct HeroCcBits *)&body_a->cc)->low = 0;
        body_a->scale = 0x100;
        resolve_contact_action(body_a, partner, 0, 0, (const u8 *)0x0403018B);
        body_a->count++;
        if (partner->data->displayTimer > 0 || (partner->data->flags111 & 2) != 0)
        {
            s32 x = partner->targetX / 0x100 + partner->ofsX + 6;
            s32 y = partner->targetY / 0x100 + partner->ofsY;

            effect_spawn_at(0x6F7, x, y, 0x14, partner);
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            place_targets(body_a, body_a->data->targetX / 0x100 + 9, body_a->data->targetY / 0x100 + 0x10,
                          body_a->data->targetH / 0x100, -1);
            body_a->height = body_a->targetZ / 0x100 + 0x20;
            set_motion_offsets(body_a, 0x400);
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            sprite_set_action(body_a, 5, 0x2034, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        body->state = hero_a_active_fx_anim_ready_alt;
    }
}

void hero_a_active_fx_anim_ready_alt(struct HeroBody *body)
{
    struct HeroBody *body_a = &gWorld->heroA->body;

    step_motion(body_a);
    if ((body_a->flags & 0x20) != 0)
    {
        set_slot_value(0x2E, -1);
        effect_spawn_at(0x1E, body_a->posX / 0x100, body_a->posY / 0x100, body_a->posZ / 0x100, body_a);
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            sprite_set_action(body_a, 8, 0x2034, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
        body->state = hero_pair_place_and_finish_alt2;
    }
}

void hero_pair_place_and_finish_alt2(struct HeroBody *body)
{
    struct HeroBody *body_a = &gWorld->heroA->body;
    struct HeroBody *body_b = &gWorld->heroB->body;

    if ((body_a->sprite->flags & 8) != 0)
    {
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            place_targets(body_a, body_a->data->targetX / 0x100, body_a->data->targetY / 0x100,
                          body_a->data->targetH / 0x100, -1);
            set_motion_start(body_a, 0x400);
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            if (body_a->data->poseIndex == -1)
                sprite_set_action(body_a, 0, 0x2002, 0);
            else
                sprite_set_action(body_a, 0, 0x204F, 0);
            {
                struct HeroSprite *sprite;
                s16 *span;
                s32 mirror;

                sprite = body_a->sprite;
                mirror = 0;
                span = &body_a->span.a;
                if (!(span[0] < span[1]))
                    mirror = 1;
                ((struct HeroMirrorBits *)&sprite->mirror)->flip = mirror;
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
            place_targets(body_b, body_b->data->targetX / 0x100, body_b->data->targetY / 0x100,
                          body_b->data->targetH / 0x100, -1);
            set_motion_start(body_b, 0x400);
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            if (body_b->data->poseIndex == -1)
                sprite_set_action(body_b, 0, 0x2002, 0);
            else
                sprite_set_action(body_b, 0, 0x204F, 0);
            {
                struct HeroSprite *sprite;
                s16 *span;
                s32 mirror;

                sprite = body_b->sprite;
                mirror = 0;
                span = &body_b->span.a;
                if (!(span[0] < span[1]))
                    mirror = 1;
                ((struct HeroMirrorBits *)&sprite->mirror)->flip = mirror;
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
