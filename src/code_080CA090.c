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
    u8 filler0[0x70];
    struct Hero *heroA;
    struct Hero *heroB;
};

extern struct World *gUnk_03000FD8;

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

void hero_pair_finish_place_both(struct HeroBody *body)
{
    struct Hero *hero_a = gUnk_03000FD8->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct Hero *hero_b = gUnk_03000FD8->heroB;
    struct HeroBody *body_b = &hero_b->body;

    if ((body_a->sprite->flags & 8) != 0)
    {
        set_slot_value(0x53, -1);
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            place_targets(body_b, body_b->data->targetX / 0x100, body_b->data->targetY / 0x100,
                          body_b->data->targetH / 0x100, -1);
            set_motion_start(body_b, 0x400);
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            if (body_b->data->poseIndex == -1)
            {
                sprite_set_action(body_b, 0, 0x2002, 0);
            }
            else
            {
                sprite_set_action(body_b, 0, 0x204F, 0);
            }
            {
                struct HeroSprite *sprite = body_b->sprite;
                s32 flip = 0;
                struct HeroSpan *span = &body_b->span;
                if (span->a >= span->b)
                {
                    flip = 1;
                }
                ((struct HeroMirrorBits *)&sprite->mirror)->flip = flip;
            }
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 0;
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            body_b->state = 0;
            enter_finish_rebuild(body_b);
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            place_targets(body_a, body_a->data->targetX / 0x100, body_a->data->targetY / 0x100,
                          body_a->data->targetH / 0x100, -1);
            set_motion_start(body_a, 0x400);
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            if (body_a->data->poseIndex == -1)
            {
                sprite_set_action(body_a, 0, 0x2002, 0);
            }
            else
            {
                sprite_set_action(body_a, 0, 0x204F, 0);
            }
            {
                struct HeroSprite *sprite = body_a->sprite;
                s32 flip = 0;
                struct HeroSpan *span = &body_a->span;
                if (span->a >= span->b)
                {
                    flip = 1;
                }
                ((struct HeroMirrorBits *)&sprite->mirror)->flip = flip;
            }
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            body_a->state = 0;
            enter_finish_rebuild(body_a);
        }
    }
}

void hero_b_anim_start_step2(struct HeroBody *body)
{
    struct Hero *hero_b = gUnk_03000FD8->heroB;
    struct HeroBody *body_b = &hero_b->body;

    if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
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
    body->state = hero_b_active_fx_a_step_rate;
}

void hero_b_active_fx_a_step_rate(struct HeroBody *body)
{
    struct Hero *hero_a = gUnk_03000FD8->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct Hero *hero_b = gUnk_03000FD8->heroB;
    struct HeroBody *body_b = &hero_b->body;

    state_stack_advance_progress(body_b);
    if ((body_b->flags & 0x20) != 0)
    {
        effect_spawn_at(0x2FE6, body_a->posX / 0x100, body_a->posY / 0x100, body_a->posZ / 0x100, body_a);
        set_slot_value(0x7C, -1);
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            body_a->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0x18);
            body_a->progress = 0;
            ((struct HeroFlagBits *)&body_a->flags)->active = 0;
        }
        body->state = hero_a_active_place_b_anim_fx;
    }
}

void hero_a_active_place_b_anim_fx(struct HeroBody *body)
{
    struct Hero *hero_a = gUnk_03000FD8->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct Hero *hero_b = gUnk_03000FD8->heroB;
    struct HeroBody *body_b = &hero_b->body;

    state_stack_advance_progress(body_a);
    body_a->sprite->unk6 = 0x110 - body_a->progress / 0x40;
    if ((body_a->flags & 0x20) != 0)
    {
        set_slot_value(0x2B, -1);
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            place_targets(body_b, body_a->targetX / 0x100 + body_a->ofsX, body_a->targetY / 0x100 + body_a->ofsY, 6, -1);
            {
                u16 h = 0x3C;
                body_b->height = h;
            }
            set_motion_offsets(body_b, 0x399);
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            sprite_set_action(body_b, 6, 0x2053, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 0;
        }
        body_a->sprite->unk6 = 0x100;
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            sprite_set_action(body_a, 0, 0x2035, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        effect_spawn_at(0x2FF9, body_a->posX / 0x100, body_a->posY / 0x100, body_a->posZ / 0x100, body_a);
        body_a->unk9C = 0;
        body_a->unkA4 = 1;
        body_b->timer = body_b->timer & 0xFFF;
        body->state = hero_b_step_window_a568;
    }
}
