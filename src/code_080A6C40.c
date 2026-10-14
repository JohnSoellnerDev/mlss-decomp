#include "global.h"
#include "ram_routines.h"

struct HeroSprite
{
    u16 unk0;
    u16 unk2;
    u8 filler4[0x0E - 0x04];
    u16 unkE;
    u8 filler10[0x11 - 0x10];
    u8 mirrorFlags;
    u8 flags;
    u8 filler13[0x20 - 0x13];
    u8 frame;
};

struct HeroNode
{
    u16 x;
    u16 y;
    u8 filler4[0x0E - 0x04];
    u16 z;
};

struct HeroMirrorBits
{
    u8 unk0 : 6;
    u8 mirror : 1;
    u8 unk7 : 1;
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
    u8 bit7 : 1;
};

struct HeroFlagBits
{
    u8 unk0 : 5;
    u8 active : 1;
    u8 unk6 : 2;
};

struct HeroData
{
    u8 filler0[0xD8];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 fillerE4[0xEC - 0xE4];
    s16 pose;
    u8 fillerEE[0x114 - 0xEE];
    u32 poseValue;
    u8 filler118[0x11E - 0x118];
    u16 flags;
};

struct HeroBody
{
    u8 filler0[0x08];
    struct HeroSprite *sprite;
    u8 filler0C[0x24 - 0x0C];
    u16 count;
    u8 filler26[0x28 - 0x26];
    struct HeroData *data;
    struct HeroBody *partner;
    u8 filler30[0x34 - 0x30];
    struct HeroNode *node;
    s32 targetX;
    s32 targetY;
    s32 targetZ;
    u8 filler44[0x4C - 0x44];
    void (*state)(struct HeroBody *);
    u32 unk50;
    u8 filler54[0x6C - 0x54];
    const u8 *anim;
    u8 filler70[0x76 - 0x70];
    u8 mode;
    u8 filler77[0x79 - 0x77];
    u8 flags;
    u16 rate;
    u16 frame;
    s16 progress;
    u8 filler80[0x84 - 0x80];
    s16 spanA;
    s16 spanB;
    u8 filler88[0x92 - 0x88];
    u16 height;
    u8 filler94[0x9C - 0x94];
    u32 unk9C;
    u8 filler_A0[0xAC - 0xA0];
    s16 step;
    u16 timer;
    u8 filler_B0[0xC3 - 0xB0];
    s8 ofsX;
    s8 ofsY;
    s8 ofsZ;
    u8 filler_C6[0xCA - 0xC6];
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
    u8 filler0B[0x0E - 0x0B];
    u8 unkE;
    u8 filler0F[0x70 - 0x0F];
    struct Hero *heroA;
    struct Hero *heroB;
};

extern struct World *gUnk_03000FD8;
extern u8 *gUnk_03000FF4;

void state_stack_advance_progress(struct HeroBody *body);
void resolve_contact_action(struct HeroBody *body, struct HeroBody *partner, u32 a, u32 b, const u8 *cb);
void hero_b_sprite_step_15_6d00(struct HeroBody *body);
void hero_b_sprite_step_18_70e8(struct HeroBody *body);
void hero_b_ready_step_6d78(struct HeroBody *body);
void hero_b_ready_step_7160(struct HeroBody *body);
void hero_a_active_anims_pair(struct HeroBody *body);
void hero_b_gate_place_pair_6e34(struct HeroBody *body);
void hero_pair_place_finish_on_b_active(struct HeroBody *body);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void sound_stop_song(u32 song);
void set_slot_value(u32 id, s32 value);
void obj_release_node(void *node);
void schedule_pose_task(struct HeroBody *partner);
void hero_pair_place_then_a_clear(struct HeroBody *body);
void hero_b_active_link_to_partner(struct HeroBody *body);
void hero_b_active_link_to_partner_alt(struct HeroBody *body);
void hero_a_sprite_place_at_b(struct HeroBody *body);
void hero_a_step_wait_press(struct HeroBody *body);
void hero_a_active_clear_anim(struct HeroBody *body);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void set_motion_offsets(struct HeroBody *body, u32 offsets);
void set_motion_start(struct HeroBody *body, u32 speed);
void enter_finish_rebuild(struct HeroBody *body);
void sprite_entry_update(struct HeroNode *node);
void hero_b_drop_node_702c(struct HeroBody *body);
void hero_b_drop_node_6c40(struct HeroBody *body);

void hero_b_drop_node_6c40(struct HeroBody *body)
{
    struct Hero *hero = gUnk_03000FD8->heroB;
    struct HeroBody *b = &hero->body;

    sound_stop_song(0x87);
    sound_stop_song(0x156);
    set_slot_value(0x9B, -1);
    obj_release_node(b->node);
    b->node = 0;
    if ((hero->body.mode & 6) == 2 || (hero->body.mode & 6) == 4)
    {
        sprite_set_action(b, 2, 0x2055, 0);
        ((struct HeroSpriteBits *)&b->sprite->flags)->mode = 1;
    }
    if ((b->mode & 6) == 2 || (b->mode & 6) == 4)
    {
        b->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 8);
        b->progress = 0;
        ((struct HeroFlagBits *)&b->flags)->active = 0;
    }
    body->state = hero_b_active_link_to_partner_alt;
}

void hero_b_sprite_step_15_6d00(struct HeroBody *body)
{
    struct Hero *hero = gUnk_03000FD8->heroB;

    if ((hero->body.sprite->flags & 8) != 0)
    {
        if ((hero->body.mode & 6) == 2 || (hero->body.mode & 6) == 4)
        {
            hero->body.rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0xF);
            hero->body.progress = 0;
            ((struct HeroFlagBits *)&hero->body.flags)->active = 0;
        }
        body->state = hero_b_ready_step_6d78;
    }
}

void hero_b_ready_step_6d78(struct HeroBody *body)
{
    struct Hero *hero = gUnk_03000FD8->heroB;
    struct HeroBody *b = &hero->body;

    state_stack_advance_progress(b);
    if ((b->flags & 0x20) != 0)
    {
        if ((hero->body.mode & 6) == 2 || (hero->body.mode & 6) == 4)
        {
            sprite_set_action(b, 0, 0xFFFFFFFF, 0);
            ((struct HeroMirrorBits *)&b->sprite->mirrorFlags)->mirror = 0;
            ((struct HeroSpriteBits *)&b->sprite->flags)->mode = 0;
        }
        if ((b->mode & 6) == 2 || (b->mode & 6) == 4)
        {
            b->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 2);
            b->progress = 0;
            ((struct HeroFlagBits *)&b->flags)->active = 0;
        }
        body->state = hero_b_gate_place_pair_6e34;
    }
}

void hero_b_gate_place_pair_6e34(struct HeroBody *body)
{
    struct World *world = gUnk_03000FD8;
    struct Hero *hero = world->heroA;
    struct HeroBody *a = &hero->body;
    struct HeroBody *b = &world->heroB->body;
    struct HeroData *data;
    u8 *mode;
    struct HeroSprite *sprite;
    s16 *span;
    s32 mirror;
    s32 x;
    s32 y;
    s32 z;
    s32 t;

    state_stack_advance_progress(b);
    if ((b->flags & 0x20) != 0)
    {
        if ((hero->body.mode & 6) == 2 || (hero->body.mode & 6) == 4)
        {
            data = a->data;
            t = data->targetX;
            if (t < 0)
                t += 0xFF;
            x = t >> 8;
            t = data->targetY;
            if (t < 0)
                t += 0xFF;
            y = t >> 8;
            t = data->targetH;
            if (t < 0)
                t += 0xFF;
            z = t >> 8;
            place_targets(a, x, y, z, -1);
            set_motion_start(a, 0x400);
        }
        if ((a->mode & 6) == 2 || (a->mode & 6) == 4)
        {
            if (a->data->pose == -1)
                sprite_set_action(a, 0, 0x2002, 0);
            else
                sprite_set_action(a, 0, 0x204F, 0);
            sprite = a->sprite;
            mirror = 0;
            span = &a->spanA;
            if (!(span[0] < span[1]))
                mirror = 1;
            ((struct HeroMirrorBits *)&sprite->mirrorFlags)->mirror = mirror;
            ((struct HeroSpriteBits *)&a->sprite->flags)->mode = 0;
        }
        mode = &a->mode;
        if ((*mode & 6) == 2 || (*mode & 6) == 4)
        {
            a->state = 0;
            enter_finish_rebuild(a);
        }
        if ((b->mode & 6) == 2 || (b->mode & 6) == 4)
        {
            data = b->data;
            t = data->targetX;
            if (t < 0)
                t += 0xFF;
            x = t >> 8;
            t = data->targetY;
            if (t < 0)
                t += 0xFF;
            y = t >> 8;
            t = data->targetH;
            if (t < 0)
                t += 0xFF;
            z = t >> 8;
            place_targets(b, x, y, z, -1);
            set_motion_start(b, 0x400);
        }
        mode = &b->mode;
        if ((*mode & 6) == 2 || (*mode & 6) == 4)
        {
            if (b->data->pose == -1)
                sprite_set_action(b, 0, 0x2002, 0);
            else
                sprite_set_action(b, 0, 0x204F, 0);
            sprite = b->sprite;
            mirror = 0;
            span = &b->spanA;
            if (!(span[0] < span[1]))
                mirror = 1;
            ((struct HeroMirrorBits *)&sprite->mirrorFlags)->mirror = mirror;
            ((struct HeroSpriteBits *)&b->sprite->flags)->mode = 0;
        }
        if ((*mode & 6) == 2 || (*mode & 6) == 4)
        {
            b->state = 0;
            enter_finish_rebuild(b);
        }
    }
}

void hero_b_drop_node_702c(struct HeroBody *body)
{
    struct Hero *hero = gUnk_03000FD8->heroB;
    struct HeroBody *b = &hero->body;

    sound_stop_song(0x156);
    set_slot_value(0x9A, -1);
    obj_release_node(b->node);
    b->node = 0;
    if ((hero->body.mode & 6) == 2 || (hero->body.mode & 6) == 4)
    {
        sprite_set_action(b, 1, 0x2055, 0);
        ((struct HeroSpriteBits *)&b->sprite->flags)->mode = 1;
    }
    if ((b->mode & 6) == 2 || (b->mode & 6) == 4)
    {
        b->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 1);
        b->progress = 0;
        ((struct HeroFlagBits *)&b->flags)->active = 0;
    }
    body->state = hero_b_active_link_to_partner;
}

void hero_b_sprite_step_18_70e8(struct HeroBody *body)
{
    struct Hero *hero = gUnk_03000FD8->heroB;

    if ((hero->body.sprite->flags & 8) != 0)
    {
        if ((hero->body.mode & 6) == 2 || (hero->body.mode & 6) == 4)
        {
            hero->body.rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0x12);
            hero->body.progress = 0;
            ((struct HeroFlagBits *)&hero->body.flags)->active = 0;
        }
        body->state = hero_b_ready_step_7160;
    }
}

void hero_b_ready_step_7160(struct HeroBody *body)
{
    struct Hero *hero = gUnk_03000FD8->heroB;
    struct HeroBody *b = &hero->body;

    state_stack_advance_progress(b);
    if ((b->flags & 0x20) != 0)
    {
        if ((hero->body.mode & 6) == 2 || (hero->body.mode & 6) == 4)
        {
            sprite_set_action(b, 0, 0xFFFFFFFF, 0);
            ((struct HeroMirrorBits *)&b->sprite->mirrorFlags)->mirror = 0;
            ((struct HeroSpriteBits *)&b->sprite->flags)->mode = 0;
        }
        if ((b->mode & 6) == 2 || (b->mode & 6) == 4)
        {
            b->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 2);
            b->progress = 0;
            ((struct HeroFlagBits *)&b->flags)->active = 0;
        }
        body->state = hero_pair_place_finish_on_b_active;
    }
}

void hero_pair_place_finish_on_b_active(struct HeroBody *body)
{
    struct World *world = gUnk_03000FD8;
    struct Hero *hero = world->heroA;
    struct HeroBody *a = &hero->body;
    struct HeroBody *b = &world->heroB->body;
    struct HeroData *data;
    u8 *mode;
    struct HeroSprite *sprite;
    s16 *span;
    s32 mirror;
    s32 x;
    s32 y;
    s32 z;
    s32 t;

    state_stack_advance_progress(b);
    if ((b->flags & 0x20) != 0)
    {
        if ((hero->body.mode & 6) == 2 || (hero->body.mode & 6) == 4)
        {
            data = a->data;
            t = data->targetX;
            if (t < 0)
                t += 0xFF;
            x = t >> 8;
            t = data->targetY;
            if (t < 0)
                t += 0xFF;
            y = t >> 8;
            t = data->targetH;
            if (t < 0)
                t += 0xFF;
            z = t >> 8;
            place_targets(a, x, y, z, -1);
            set_motion_start(a, 0x400);
        }
        if ((a->mode & 6) == 2 || (a->mode & 6) == 4)
        {
            if (a->data->pose == -1)
                sprite_set_action(a, 0, 0x2002, 0);
            else
                sprite_set_action(a, 0, 0x204F, 0);
            sprite = a->sprite;
            mirror = 0;
            span = &a->spanA;
            if (!(span[0] < span[1]))
                mirror = 1;
            ((struct HeroMirrorBits *)&sprite->mirrorFlags)->mirror = mirror;
            ((struct HeroSpriteBits *)&a->sprite->flags)->mode = 0;
        }
        mode = &a->mode;
        if ((*mode & 6) == 2 || (*mode & 6) == 4)
        {
            a->state = 0;
            enter_finish_rebuild(a);
        }
        if ((b->mode & 6) == 2 || (b->mode & 6) == 4)
        {
            data = b->data;
            t = data->targetX;
            if (t < 0)
                t += 0xFF;
            x = t >> 8;
            t = data->targetY;
            if (t < 0)
                t += 0xFF;
            y = t >> 8;
            t = data->targetH;
            if (t < 0)
                t += 0xFF;
            z = t >> 8;
            place_targets(b, x, y, z, -1);
            set_motion_start(b, 0x400);
        }
        mode = &b->mode;
        if ((*mode & 6) == 2 || (*mode & 6) == 4)
        {
            if (b->data->pose == -1)
                sprite_set_action(b, 0, 0x2002, 0);
            else
                sprite_set_action(b, 0, 0x204F, 0);
            sprite = b->sprite;
            mirror = 0;
            span = &b->spanA;
            if (!(span[0] < span[1]))
                mirror = 1;
            ((struct HeroMirrorBits *)&sprite->mirrorFlags)->mirror = mirror;
            ((struct HeroSpriteBits *)&b->sprite->flags)->mode = 0;
        }
        if ((*mode & 6) == 2 || (*mode & 6) == 4)
        {
            b->state = 0;
            enter_finish_rebuild(b);
        }
    }
}

void hero_b_active_link_to_partner(struct HeroBody *body)
{
    struct HeroBody *b = &gUnk_03000FD8->heroB->body;
    struct HeroBody *partner;

    partner = body->partner;
    state_stack_advance_progress(b);
    if ((b->flags & 0x20) != 0)
    {
        ((struct HeroCcBits *)&b->cc)->low = 0;
        b->scale = 0x100;
        resolve_contact_action(b, partner, 0, 0, (const u8 *)0x08018105);
        b->count++;
        body->state = hero_b_sprite_step_18_70e8;
    }
}

void hero_b_active_link_to_partner_alt(struct HeroBody *body)
{
    struct HeroBody *b = &gUnk_03000FD8->heroB->body;
    struct HeroBody *partner;

    partner = body->partner;
    state_stack_advance_progress(b);
    if ((b->flags & 0x20) != 0)
    {
        ((struct HeroCcBits *)&b->cc)->low = 0;
        b->scale = 0x100;
        resolve_contact_action(b, partner, 0, 0, (const u8 *)0x08018005);
        b->count++;
        body->state = hero_b_sprite_step_15_6d00;
    }
}

s32 pair_start_pose_then_place(struct HeroBody *body)
{
    struct HeroData *data;
    struct HeroBody *body_a;
    struct HeroBody *body_b;
    struct HeroBody *partner;
    const u8 *anim;
    s32 pose;
    u8 *record;
    struct World *world;
    u8 **party = &gUnk_03000FF4;

    data = body->data;
    pose = data->pose;
    record = *party - (pose * 60 + 0x3C);
    data->poseValue = *(u16 *)(record + 0x2A);
    world = gUnk_03000FD8;
    body_a = &world->heroA->body;
    body_b = &world->heroB->body;
    partner = body->partner;
    body_b->partner = partner;
    body_a->partner = partner;
    anim = body->anim;
    body_b->anim = anim;
    body_a->anim = anim;
    partner->unk50 = 0;
    body_b->unk50 = 0;
    body_a->unk50 = 0;
    partner->frame = 0x100;
    body_b->frame = 0x100;
    body_a->frame = 0x100;
    body->state = hero_pair_place_then_a_clear;
    body_b->count = 0;
    body_a->count = 0;
    schedule_pose_task(partner);
    return 1;
}
