#include "global.h"
#include "ram_routines.h"

struct HeroSprite
{
    u8 filler0[0x06];
    s16 scale;
    u8 filler8[0x11 - 0x08];
    u8 mirror;
    u8 flags;
    u8 filler13[0x20 - 0x13];
    u8 frame;
};

struct HeroSpriteBits
{
    u8 unk0 : 1;
    u8 mode : 2;
    u8 unk3 : 5;
};

struct HeroData
{
    u8 filler0[0xEC];
    s16 pose;
};

struct HeroBody
{
    u8 filler0[0x08];
    struct HeroSprite *sprite;
    u8 filler0C[0x10 - 0x0C];
    s32 posX;
    s32 posY;
    s32 posZ;
    u8 filler1C[0x28 - 0x1C];
    struct HeroData *data;
    struct HeroBody *partner;
    u8 filler30[0x38 - 0x30];
    s32 targetX;
    s32 targetY;
    s32 targetZ;
    u8 filler44[0x47 - 0x44];
    s8 unk47;
    u8 filler48[0x4C - 0x48];
    void (*state)(struct HeroBody *);
    u8 filler50[0x76 - 0x50];
    u8 mode;
    u8 filler77[0x79 - 0x77];
    u8 flags;
    u16 rate;
    u16 frame;
    s16 progress;
    u8 filler80[0x92 - 0x80];
    s16 height;
    u8 filler94[0xAC - 0x94];
    s16 step;
    u16 timer;
    u8 filler0B0[0xC3 - 0xB0];
    s8 ofsX;
    s8 ofsY;
    s8 ofsZ;
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

extern struct World *gWorld;

/* HeroBody.mode bits 1-2 are 2 or 4 while the body is on the field */
#define BODY_ON_FIELD(body) (((body)->mode & 6) == 2 || ((body)->mode & 6) == 4)

/* HeroBody.flags: the motion started with place_targets has arrived */
#define BODY_MOTION_DONE 0x20

void step_motion_scaled(struct HeroBody *body);
void set_slot_value(u32 id, s32 value);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void set_motion_speed_aux(struct HeroBody *body, u32 speed, u32 aux);
void set_motion_offsets(struct HeroBody *body, u32 offsets);
void set_cur_flags_both(u32 value);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 h, struct HeroBody *owner);
u32 find_frame_entry(struct HeroBody *obj, u32 index);

void hero_b_go_to_a_speed(struct HeroBody *body);
void hero_b_step_scaled_flags(struct HeroBody *body);
void hero_b_go_to_partner_height_2(struct HeroBody *body);
void hero_b_arrive_59a8(struct HeroBody *body);
void hero_b_step_done_6274(struct HeroBody *body);
void hero_a_go_to_b_target(struct HeroBody *body);
void hero_pair_anim_set(struct HeroBody *body);
void hero_b_go_to_partner_aux(struct HeroBody *body);
void hero_b_step_to_partner_6da0(struct HeroBody *body);

/*
 * Sends hero B, from standstill and speeding up (accel 0xB3), to hero A's target point plus
 * hero A's offsets, 0x10 below hero A's height, in action 6. Then hands over to
 * hero_b_step_scaled_flags.
 */
void hero_b_go_to_a_speed(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct HeroBody *body_a = &world->heroA->body;
    struct HeroBody *body_b = &world->heroB->body;
    s32 x;
    s32 y;
    s32 z;

    /* mode bits 1-2 are 2 or 4 while the body is on the field */
    if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
    {
        x = body_a->targetX / 0x100 + body_a->ofsX;
        y = body_a->targetY / 0x100 + body_a->ofsY;
        z = body_a->targetZ / 0x100 + body_a->ofsZ - 0x10;
        place_targets(body_b, x, y, z, -1);
        set_motion_speed_aux(body_b, 0, 0xB3);
    }
    if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
    {
        /* pose -1 and -2 pick each hero's own sprite set */
        if (body_b->data->pose == -1)
            sprite_set_action(body_b, 6, 0x2000, 0);
        else
            sprite_set_action(body_b, 6, 0x204D, 0);
        ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
    }
    body_b->timer = body_b->timer & 0xFFF;
    body->state = hero_b_step_scaled_flags;
}

NONMATCH("asm/code_080B55B8/hero_b_step_scaled_flags.s", void hero_b_step_scaled_flags(struct HeroBody *body))
{
    struct World *world = gWorld;
    struct Hero *hero_a = world->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct Hero *hero_b = world->heroB;
    struct HeroBody *body_b = &hero_b->body;
    s32 z;
    s32 ofs_z;
    s32 limit_a = 0x4000;
    s32 limit_b = 0x4000;
    s32 reached_a;
    s32 reached_b;

    step_motion_scaled(body_b);
    z = body_b->posZ / 0x100;
    ofs_z = hero_a->body.ofsZ;
    if (z < ofs_z)
    {
        if (body_a->sprite->scale == 0x100)
        {
            set_slot_value(0x7C, -1);
            if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
            {
                sprite_set_action(body_b, 1, 0x2060, 0);
                ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
            }
        }
        body_a->sprite->scale = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(z << 8, ofs_z);
    }
    if (limit_a > 0x7FFF)
        limit_a = 0x7FFF;
    reached_a = 0;
    if (body_b->step <= limit_a)
    {
        if (body_b->progress >= limit_a)
            reached_a = 1;
    }
    if (reached_a)
        set_cur_flags_both(0x40000000);
    if (limit_b > 0x7FFF)
        limit_b = 0x7FFF;
    reached_b = 0;
    if (body_b->step <= limit_b)
    {
        if (body_b->progress >= limit_b)
            reached_b = 1;
    }
    if (reached_b)
        set_cur_flags_both(0x40000000);
    if ((body_b->flags & 0x20) != 0)
    {
        effect_spawn_at(0x2F7D, body_b->posX / 0x100, body_b->posY / 0x100, body_b->posZ / 0x100, body_b);
        body->state = hero_b_go_to_partner_height_2;
    }
}
END_NONMATCH

/*
 * Once hero B's animation has finished: plays sound 0x2B and sends hero B on a
 * high arc onto its target (higher the higher the target stands), in its jump
 * pose, then hands over to hero_b_arrive_59a8.
 */
void hero_b_go_to_partner_height_2(struct HeroBody *body)
{
    struct Hero *hero_b = gWorld->heroB;
    struct HeroBody *body_b = &hero_b->body;
    struct HeroBody *target = body->partner;
    s32 term;
    s32 x;
    s32 y;
    s32 z;
    s32 height;

    /* sprite flags bit 3: the animation has finished */
    if ((body_b->sprite->flags & 8) != 0)
    {
        set_slot_value(0x2B, -1);
        /* mode bits 1-2 are 2 or 4 while the body is on the field */
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            x = target->targetX / 0x100 + target->ofsX;
            y = target->targetY / 0x100 + target->ofsY;
            z = target->targetZ / 0x100 + target->ofsZ;
            place_targets(body_b, x, y, z, -1);
            /* the target's own body uses its offset, a part its current frame */
            if (target == (struct HeroBody *)((u8 *)target->data + 8))
            {
                height = target->ofsZ;
            }
            else
            {
                find_frame_entry(target, 0);
                height = target->unk47;
            }
            /* + targetZ / 0x100 */
            term = target->targetZ;
            if (term < 0)
                term += 0xFF;
            height += term >> 8;
            /* the jump's peak: the target's height plus 10000 / (height^2 / 0x40 + 100) */
            term = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x2710, (height * height) / 64 + 0x64);
            height += term;
            body_b->height = height;
            set_motion_offsets(body_b, 0x600);
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            if (body_b->data->pose == -1)
                sprite_set_action(body_b, 5, 0x2000, 0);
            else
                sprite_set_action(body_b, 5, 0x204D, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
        }
        body_b->timer = body_b->timer & 0xFFF;
        body->state = hero_b_arrive_59a8;
    }
}

ASM_FUNC("asm/code_080B55B8/hero_b_arrive_59a8.s", void hero_b_arrive_59a8(struct HeroBody *body))

ASM_FUNC("asm/code_080B55B8/hero_b_step_done_6274.s", void hero_b_step_done_6274(struct HeroBody *body))

/*
 * Once hero A's animation has finished: spawns effect 0x2FAC at hero A and sends it (speed 0x600)
 * to hero B's target point plus hero B's x/y offsets, at ground height, in action 5. Then hands
 * over to hero_pair_anim_set.
 */
void hero_a_go_to_b_target(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct HeroBody *body_a = &world->heroA->body;
    struct HeroBody *body_b = &world->heroB->body;
    s32 x;
    s32 y;
    s32 z;

    /* sprite flags bit 3: the animation has finished */
    if ((body_a->sprite->flags & 8) != 0)
    {
        effect_spawn_at(0x2FAC, body_a->posX / 0x100, body_a->posY / 0x100, body_a->posZ / 0x100, body_a);
        /* mode bits 1-2 are 2 or 4 while the body is on the field */
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            x = body_b->targetX / 0x100 + body_b->ofsX;
            y = body_b->targetY / 0x100 + body_b->ofsY;
            z = body_b->targetZ / 0x100;
            place_targets(body_a, x, y, z, -1);
            set_motion_speed_aux(body_a, 0x600, 0);
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            /* pose -1 and -2 pick each hero's own sprite set */
            if (body_a->data->pose == -1)
                sprite_set_action(body_a, 5, 0x2000, 0);
            else
                sprite_set_action(body_a, 5, 0x204D, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
        body->state = hero_pair_anim_set;
    }
}

/*
 * Moves hero A on towards hero B's target. Once it arrives: plays sound 0xAE, puts both heroes
 * (those on the field) in action 2 (hero A anim 0x2031, hero B 0x2060), then hands over to
 * hero_b_go_to_partner_aux.
 */
void hero_pair_anim_set(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct HeroBody *body_a = &world->heroA->body;
    struct HeroBody *body_b = &world->heroB->body;

    step_motion_scaled(body_a);
    if ((body_a->flags & BODY_MOTION_DONE) != 0)
    {
        set_slot_value(0xAE, -1);
        if (BODY_ON_FIELD(body_a))
        {
            sprite_set_action(body_a, 2, 0x2031, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
        if (BODY_ON_FIELD(body_b))
        {
            sprite_set_action(body_b, 2, 0x2060, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
        }
        body->state = hero_b_go_to_partner_aux;
    }
}

/*
 * Once hero B's animation has finished: plays sound 0xA7 and sends hero B, from standstill and
 * speeding up (accel 0x200), to the target's point plus its offsets. Then hands over to
 * hero_b_step_to_partner_6da0.
 */
void hero_b_go_to_partner_aux(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct HeroBody *body_b = &world->heroB->body;
    struct HeroBody *target = body->partner;
    s32 x;
    s32 y;
    s32 z;

    /* sprite flags bit 3: the animation has finished */
    if ((body_b->sprite->flags & 8) != 0)
    {
        set_slot_value(0xA7, -1);
        /* mode bits 1-2 are 2 or 4 while the body is on the field */
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            x = target->targetX / 0x100 + target->ofsX;
            y = target->targetY / 0x100 + target->ofsY;
            z = target->targetZ / 0x100 + target->ofsZ;
            place_targets(body_b, x, y, z, -1);
            set_motion_speed_aux(body_b, 0, 0x200);
        }
        body_b->timer = body_b->timer & 0xFFF;
        body->state = hero_b_step_to_partner_6da0;
    }
}

ASM_FUNC("asm/code_080B55B8/hero_b_step_to_partner_6da0.s", void hero_b_step_to_partner_6da0(struct HeroBody *body))
