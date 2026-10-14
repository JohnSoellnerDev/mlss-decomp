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
    u8 view;
    u8 filler0D[0x58 - 0x0D];
    struct Hero *list[6];
    struct Hero *heroA;
    struct Hero *heroB;
    struct HeroLayer *layers;
};

extern struct World *gUnk_03000FD8;

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

void hero_b_visible_anim_b_rate_a(struct HeroBody *body)
{
    struct HeroBody *body_a = &gUnk_03000FD8->heroA->body;
    struct HeroBody *body_b = &gUnk_03000FD8->heroB->body;

    if ((body_b->sprite->flags & 8) != 0)
    {
        set_slot_value(0xAF, -1);
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            sprite_set_action(body_b, 0xB, 0x2063, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            body_a->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0xA);
            body_a->progress = 0;
            ((struct HeroFlagBits *)&body_a->flags)->active = 0;
        }
        body_a->timer = body_a->timer & 0xFFF;
        body->state = hero_a_step_window_2fec;
    }
}

ASM_FUNC("asm/code_080C2F20/hero_a_step_window_2fec.s", void hero_a_step_window_2fec(struct HeroBody *body))

void hero_a_active_link_place_finish(struct HeroBody *body)
{
    struct HeroBody *body_a = &gUnk_03000FD8->heroA->body;
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
            s32 x = partner->targetX / 0x100 + partner->ofsX + 4;
            s32 y = partner->targetY / 0x100 + partner->ofsY;

            effect_spawn_at(0x6E3, x, y, 0x10, partner);
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
        body->state = hero_a_active_fx_anim_ready;
    }
}

void hero_a_active_fx_anim_ready(struct HeroBody *body)
{
    struct HeroBody *body_a = &gUnk_03000FD8->heroA->body;

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
        body->state = hero_pair_place_and_finish_alt;
    }
}

void hero_pair_place_and_finish_alt(struct HeroBody *body)
{
    struct HeroBody *body_a = &gUnk_03000FD8->heroA->body;
    struct HeroBody *body_b = &gUnk_03000FD8->heroB->body;

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

void hero_pair_mode_and_ready_anims(struct HeroBody *body)
{
    struct HeroBody *body_a;
    struct HeroBody *body_b;
    s32 mode;

    mode = gUnk_03000FD8->modeByte;
    mode |= 0xC0;
    gUnk_03000FD8->modeByte = mode;
    body_a = &gUnk_03000FD8->heroA->body;
    body_b = &gUnk_03000FD8->heroB->body;
    body_a->sprite->unk06 = 0x100;
    if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
    {
        sprite_set_action(body_a, 3, 0x2034, 0);
        ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
    }
    if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
    {
        sprite_set_action(body_b, 0xC, 0x2063, 0);
        ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
    }
    body->state = hero_b_sprite_active_anim3_to_3bec;
}

void hero_b_visible_anim_b_rate_a_alt(struct HeroBody *body)
{
    struct HeroBody *body_a = &gUnk_03000FD8->heroA->body;
    struct HeroBody *body_b = &gUnk_03000FD8->heroB->body;

    if ((body_b->sprite->flags & 8) != 0)
    {
        set_slot_value(0xAF, -1);
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            sprite_set_action(body_b, 0xB, 0x2063, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            body_a->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0xA);
            body_a->progress = 0;
            ((struct HeroFlagBits *)&body_a->flags)->active = 0;
        }
        body_a->timer = body_a->timer & 0xFFF;
        body->state = hero_a_walk_to_partner_3cb8;
    }
}

ASM_FUNC("asm/code_080C2F20/hero_a_walk_to_partner_3cb8.s", void hero_a_walk_to_partner_3cb8(struct HeroBody *body))

void hero_a_active_link_place_finish_alt(struct HeroBody *body)
{
    struct HeroBody *body_a = &gUnk_03000FD8->heroA->body;
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
    struct HeroBody *body_a = &gUnk_03000FD8->heroA->body;

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
    struct HeroBody *body_a = &gUnk_03000FD8->heroA->body;
    struct HeroBody *body_b = &gUnk_03000FD8->heroB->body;

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
