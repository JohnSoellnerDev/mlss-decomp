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
    u8 unk21; /* current animation (set by obj_load_record) */
};

struct HeroSpriteBits
{
    u8 unk0 : 1;
    u8 mode : 2;
    u8 unk3 : 5;
};

struct HeroData
{
    u8 filler0[0xD8];
    s32 unkD8;
    s32 unkDC;
    s32 unkE0;
    u8 filler0E4[0xEC - 0xE4];
    s16 pose;
    u8 filler0EE[0xF6 - 0xEE];
    s16 unkF6;
    u8 filler0F8[0x111 - 0xF8];
    u8 flags111;
};

struct SpriteMirrorByte
{
    u8 lo6 : 6;
    u8 mirror : 1;
    u8 hi : 1;
};

struct CcBits
{
    u8 lo7 : 7;
    u8 top : 1;
};

struct HeroSpan
{
    s16 a;
    s16 b;
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
    u8 filler44[0x47 - 0x44];
    s8 unk47;
    u8 filler48[0x4C - 0x48];
    void (*state)(struct HeroBody *);
    u32 unk50;
    u8 filler54[0x6C - 0x54];
    void *anim;
    u8 filler70[0x75 - 0x70];
    u8 order;
    u8 mode;
    u8 filler77[0x79 - 0x77];
    u8 flags;
    s16 rate;
    s16 frame;
    s16 progress;
    u8 filler80[0x84 - 0x80];
    struct HeroSpan span;
    u8 filler88[0x92 - 0x88];
    u16 height;
    s32 unk94;
    s32 unk98;
    u8 filler9C[0xAC - 0x9C];
    s16 step;
    u16 timer;
    u8 filler0B0[0xC3 - 0xB0];
    s8 ofsX;
    s8 ofsY;
    s8 ofsZ;
    u8 filler0C6[0xCA - 0xC6];
    u16 scale;
    u8 unkCC;
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

/* Bits 1-2 of HeroBody.mode are 1 or 2: the body is drawn, so its sprite can be animated. */
#define HERO_MODE_ACTIVE(mode) (((mode) & 6) == 2 || ((mode) & 6) == 4)

/* HeroSprite.flags bit 3: the current animation has finished. */
#define HERO_SPRITE_ANIM_DONE 8

/* HeroBody.flags bit step_motion sets when the body has reached its motion target. */
#define HERO_FLAG_MOTION_DONE 0x20

void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void set_slot_value(u32 id, s32 value);
void step_motion(struct HeroBody *body);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 z, struct HeroBody *owner);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void set_motion_start(struct HeroBody *body, u32 speed);
void set_motion_offsets(struct HeroBody *body, u32 offsets);

void pair_action_approach(struct HeroBody *body);
void pair_action_settle(struct HeroBody *body);
void hero_b_motion_end_edac(struct HeroBody *body);
void hero_b_walk_to_partner_then_pair_af2bc(struct HeroBody *body);
void hero_b_step_threshold_fc4c(struct HeroBody *body);
s32 resolve_contact_action(struct HeroBody *a, struct HeroBody *b, s32 x, s32 y, u32 code);
void obj_release(struct HeroBody *obj);
void obj_slot_update(struct HeroBody *body);
void enter_finish_rebuild(struct HeroBody *body);

/* Waits for hero A's current animation to finish, then starts the pair action: hero A's child
 * goes to action 2, hero A takes pose 5, hero B is placed 10 px right of and 6 px above hero A's
 * target, sound 0xA7 plays and hero B starts approaching its partner. */
void pair_action_b_follow_a(struct HeroBody *body)
{
    struct Hero *hero_a = gWorld->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct Hero *hero_b = gWorld->heroB;
    struct HeroBody *body_b = &hero_b->body;

    if ((body_a->sprite->flags & HERO_SPRITE_ANIM_DONE) != 0)
    {
        if (body_a->child != NULL && body_a->child->sprite->unk21 != 2)
            sprite_set_action(body_a->child, 2, 0, 0);
        body_b->order = body_a->order;
        if (HERO_MODE_ACTIVE(body_a->mode))
        {
            sprite_set_action(body_a, 5, 0x2032, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
        body_b->targetX = body_b->posX = body_a->targetX + 0xA00;
        body_b->targetY = body_b->posY = body_a->targetY;
        body_b->targetZ = body_b->posZ = body_a->targetZ + 0x600;
        set_slot_value(0xA7, -1);
        body->state = pair_action_approach;
    }
}

/* Hero B walks toward its partner: while it is more than 45 px away, it aims 30 px closer and
 * settles; once close enough it aims at the partner itself and the pair action ends. */
void pair_action_approach(struct HeroBody *body)
{
    struct Hero *hero_b = gWorld->heroB;
    struct HeroBody *body_b = &hero_b->body;
    struct HeroBody *partner = body->partner;
    s32 dx;
    s32 dy;
    s32 dist;
    s32 stepX;
    s32 stepY;
    s32 x;
    s32 y;

    if (HERO_MODE_ACTIVE(body_b->mode))
    {
        sprite_set_action(body_b, 0xC, 0x2061, 0);
        ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
    }
    /* distances in 1/16 px */
    dx = (partner->posX - body_b->posX) / 0x10;
    dy = (partner->posY - body_b->posY) / 0x10;
    dist = ((s32 (*)(s32))gMathRoutines)(dx * dx + dy * dy);
    if (dist > 0x2D0)
    {
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            stepX = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(dx * 30, dist);
            stepY = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(dy * 30, dist);
            x = body_b->targetX / 0x100 + body_b->ofsX + stepX;
            y = body_b->targetY / 0x100 + body_b->ofsY + stepY;
            place_targets(body_b, x, y, 0, -1);
            {
                u16 height = 0x28;
                body_b->height = height;
            }
            set_motion_offsets(body_b, 0x600);
        }
    }
    else
    {
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            place_targets(body_b, partner->targetX / 0x100 + partner->ofsX,
                partner->targetY / 0x100 + partner->ofsY, 0, -1);
            {
                u16 height = 0x28;
                body_b->height = height;
            }
            set_motion_offsets(body_b, 0x600);
        }
        if (HERO_MODE_ACTIVE(body->mode))
            body->state = hero_b_motion_end_edac;
        return;
    }
    body->state = pair_action_settle;
}

/* One hop of hero B toward its partner: moves it along its jump, switches to the falling pose
   (action 0xB) on the frame it passes the top of the arc (step (unk98 << 14) / unk94), and to the
   landing pose (0xA) once it falls below height 0x2600. When it lands it plays sound 0x30 and
   effect 0x2FF9 there, and hands back to pair_action_approach for the next hop. */
void pair_action_settle(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct Hero *hero_b = world->heroB;
    struct HeroBody *body_b = &hero_b->body;
    u32 atPeak;
    register s32 peakStep asm("r2");
    s32 mx;
    s32 my;
    s32 progress;

    step_motion(body_b);
    mx = body_b->unk94;
    my = body_b->unk98;
    peakStep = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(my << 14, mx);
    if (body_b->step <= peakStep)
    {
        progress = body_b->progress;
        atPeak = 1;
        if (progress >= peakStep)
            goto done;
    }
    atPeak = 0;
done:
    if (atPeak)
    {
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            sprite_set_action(body_b, 0xB, 0x2061, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
        }
    }
    if (body_b->sprite->unk21 == 0xB && body_b->posZ <= 0x25FF)
    {
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            sprite_set_action(body_b, 0xA, 0x2061, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
        }
    }
    if ((body_b->flags & HERO_FLAG_MOTION_DONE) != 0)
    {
        set_slot_value(0x30, -1);
        effect_spawn_at(0x2FF9, body_b->posX / 0x100, body_b->posY / 0x100,
            body_b->posZ / 0x100, body_b);
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            sprite_set_action(body_b, 0xB, 0x2061, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
        }
        if (HERO_MODE_ACTIVE(body->mode))
            body->state = pair_action_approach;
    }
}

NONMATCH("asm/code_080AE9C8/hero_b_motion_end_edac.s", void hero_b_motion_end_edac(struct HeroBody *body))
{
    struct World *world = gWorld;
    struct HeroBody *body_a = &world->heroA->body;
    struct Hero *hero_b = world->heroB;
    struct HeroBody *body_b = &hero_b->body;
    struct HeroBody *partner = body->partner;
    struct HeroData *motion;
    s32 ratio;
    s32 inRange;
    s32 mx;
    s32 my;
    s32 linked;
    s32 x;
    s32 y;
    s32 z;
    s16 *span;

    step_motion(body_b);
    mx = body_b->unk94;
    my = body_b->unk98;
    ratio = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(my << 14, mx);
    if (body_b->step > ratio)
        inRange = 0;
    else
    {
        inRange = 1;
        if (body_b->progress < ratio)
            inRange = 0;
    }
    if (inRange)
    {
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            sprite_set_action(body_b, 0xB, 0x2061, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
        }
    }
    if (body_b->sprite->unk21 == 0xB && body_b->posZ <= 0x25FF)
    {
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            sprite_set_action(body_b, 0xA, 0x2061, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
        }
    }
    if ((body_b->flags & 0x20) == 0)
        return;
    set_slot_value(0x55, -1);
    obj_release(body_a->child);
    ((struct CcBits *)&body_b->unkCC)->lo7 = 0;
    body_b->scale = 0x100;
    linked = resolve_contact_action(body_b, partner, 0, 0, 0x040211A8);
    body_b->count++;
    if (linked == 1)
    {
        obj_slot_update(body_b);
        return;
    }
    if (partner->data->unkF6 > 0 || (partner->data->flags111 & 2) != 0)
    {
        x = partner->targetX / 0x100 + partner->ofsX;
        y = partner->targetY / 0x100 + partner->ofsY;
        z = partner->targetZ / 0x100 + partner->ofsZ;
        effect_spawn_at(0x6E3, x, y, z, partner);
    }
    set_slot_value(0x53, -1);
    if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
    {
        motion = body_b->data;
        place_targets(body_b, motion->unkD8 / 0x100, motion->unkDC / 0x100,
            motion->unkE0 / 0x100, -1);
        set_motion_start(body_b, 0x400);
    }
    if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
    {
        if (body_b->data->pose == -1)
            sprite_set_action(body_b, 0, 0x2002, 0);
        else
            sprite_set_action(body_b, 0, 0x204F, 0);
        ((struct SpriteMirrorByte *)&body_b->sprite->mirror)->mirror =
            (span = &body_b->span.a)[0] >= span[1];
        ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 0;
    }
    if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
    {
        body_b->state = 0;
        enter_finish_rebuild(body_b);
    }
    if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
    {
        motion = body_a->data;
        place_targets(body_a, motion->unkD8 / 0x100, motion->unkDC / 0x100,
            motion->unkE0 / 0x100, -1);
        set_motion_start(body_a, 0x400);
    }
    if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
    {
        if (body_a->data->pose == -1)
            sprite_set_action(body_a, 0, 0x2002, 0);
        else
            sprite_set_action(body_a, 0, 0x204F, 0);
        ((struct SpriteMirrorByte *)&body_a->sprite->mirror)->mirror =
            (span = &body_a->span.a)[0] >= span[1];
        ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
    }
    if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
    {
        body_a->state = 0;
        enter_finish_rebuild(body_a);
    }
}
END_NONMATCH

/* Waits for hero A's sprite to finish (flags bit 3). Then plays slot 0xA7, gives A's child action 3,
 * puts hero B 10 px beside and 6 px above hero A (positions are 24.8), and walks it at speed 0x380
 * to 24 px left of / 12 px below the partner; the next state is hero_b_walk_to_partner_then_pair_af2bc. */
void pair_action_b_follow_partner(struct HeroBody *body)
{
    struct Hero *hero_a = gWorld->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct Hero *hero_b = gWorld->heroB;
    struct HeroBody *body_b = &hero_b->body;
    struct HeroBody *partner = body->partner;

    if ((body_a->sprite->flags & 8) != 0)
    {
        set_slot_value(0xA7, -1);
        if (body_a->child->sprite->unk21 != 3)
            sprite_set_action(body_a->child, 3, 0, 0);
        body_b->order = body_a->order;
        if (HERO_MODE_ACTIVE(body_a->mode))
        {
            sprite_set_action(body_a, 5, 0x2032, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            sprite_set_action(body_b, 5, 0x2061, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
        }
        body_b->targetX = body_b->posX = body_a->targetX + 0xA00;
        body_b->targetY = body_b->posY = body_a->targetY;
        body_b->targetZ = body_b->posZ = body_a->targetZ + 0x600;
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            place_targets(body_b, partner->targetX / 0x100 + partner->ofsX - 0x18,
                partner->targetY / 0x100 + partner->ofsY,
                partner->targetZ / 0x100 + partner->ofsZ - 0xC, -1);
            set_motion_start(body_b, 0x380);
        }
        body_b->timer = body_b->timer & 0xFFF;
        body->state = hero_b_walk_to_partner_then_pair_af2bc;
    }
}

ASM_FUNC("asm/code_080AE9C8/hero_b_walk_to_partner_then_pair_af2bc.s", void hero_b_walk_to_partner_then_pair_af2bc(struct HeroBody *body))

/* Like pair_action_b_follow_partner, at speed 0x480: once hero A's animation is done, plays sound 0xA7,
 * gives A's child action 3, puts hero B 10 px beside and 6 px above hero A (positions are 24.8), and
 * walks it to 24 px left of / 12 px below the partner; the next state is hero_b_step_threshold_fc4c. */
void hero_b_to_partner_slow(struct HeroBody *body)
{
    struct Hero *hero_a = gWorld->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct Hero *hero_b = gWorld->heroB;
    struct HeroBody *body_b = &hero_b->body;
    struct HeroBody *partner = body->partner;

    if (body_a->sprite->flags & HERO_SPRITE_ANIM_DONE)
    {
        set_slot_value(0xA7, -1);
        if (body_a->child->sprite->unk21 != 3)
            sprite_set_action(body_a->child, 3, 0, 0);
        body_b->order = body_a->order;
        if (HERO_MODE_ACTIVE(body_a->mode))
        {
            sprite_set_action(body_a, 5, 0x2032, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            sprite_set_action(body_b, 5, 0x2061, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
        }
        body_b->targetX = body_b->posX = body_a->targetX + (10 << 8);
        body_b->targetY = body_b->posY = body_a->targetY;
        body_b->targetZ = body_b->posZ = body_a->targetZ + (6 << 8);
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            place_targets(body_b, partner->targetX / 0x100 + partner->ofsX - 24,
                partner->targetY / 0x100 + partner->ofsY,
                partner->targetZ / 0x100 + partner->ofsZ - 12, -1);
            set_motion_start(body_b, 0x480);
        }
        body_b->timer = body_b->timer & 0xFFF;
        body->state = hero_b_step_threshold_fc4c;
    }
}

ASM_FUNC("asm/code_080AE9C8/hero_b_step_threshold_fc4c.s", void hero_b_step_threshold_fc4c(struct HeroBody *body))
