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

struct HeroSpriteBits
{
    u8 unk0 : 1;
    u8 mode : 2;
    u8 unk3 : 5;
};

struct HeroFlagBits
{
    u8 unk0 : 5;
    u8 active : 1;
    u8 unk6 : 2;
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
    u8 filler70[0x76 - 0x70];
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

struct SpriteListItem
{
    struct SpriteListItem *next;
    u8 filler4[0x08 - 0x04];
    struct HeroSprite *sprite;
};

struct World
{
    u8 filler0[0x0A];
    u8 modeByte;
    u8 filler0B;
    u8 layerFlags;
    u8 filler0D[0x70 - 0x0D];
    struct Hero *heroA;
    struct Hero *heroB;
    struct SpriteListItem *sprites;
};

struct WorldModeBits
{
    u8 unk0 : 4;
    u8 field4 : 2;
    u8 unk6 : 2;
};

struct KeyState
{
    u8 filler0[0x3E];
    u16 held;
};

/* Body mode bits 1-2: 2 or 4 means the body is active on the field. */
#define HERO_MODE_ACTIVE(mode) (((mode) & 6) == 2 || ((mode) & 6) == 4)

/* HeroSprite.flags bit set when the current animation has finished. */
#define HERO_SPRITE_ANIM_DONE 8

/* HeroBody.flags bit set once the body's current motion has finished. */
#define HERO_FLAG_MOTION_DONE 0x20

extern struct World *gWorld;
extern struct KeyState gSys;

void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void set_slot_value(u32 id, s32 value);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 h, struct HeroBody *owner);

void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void set_motion_offsets(struct HeroBody *body, u32 offsets);
void state_stack_advance_progress(struct HeroBody *body);
s32 resolve_contact_action(struct HeroBody *body, struct HeroBody *partner, u32 a, u32 b, u32 cb);

void hero_b_sprite_fx_partner_target(struct HeroBody *body);
void hero_a_wait_visible_anim_24(struct HeroBody *body);
void hero_b_active_fx(struct HeroBody *body);
void hero_b_active_link_partner(struct HeroBody *body);
void hero_b_sprite_place_height(struct HeroBody *body);
void step_motion(struct HeroBody *body);
void set_motion_start(struct HeroBody *body, u32 speed);
void hero_a_sprite_place_pose5(struct HeroBody *body);
void sprite_fx_hero_a_active_link_partner(struct HeroBody *body);
void hero_pair_a_active_place_finish(struct HeroBody *body);
void hero_b_anim_c_then_clear(struct HeroBody *body);
void hero_b_active_place_finish(struct HeroBody *body);
void hero_a_partner_action_b054(struct HeroBody *body);
void hero_b_fx_anim_reset(struct HeroBody *body);
void hero_a_jump_step_bc114(struct HeroBody *body);
void hero_a_z_cross_link_fx(struct HeroBody *body);
void hero_a_link_partner_anim(struct HeroBody *body);
void body_set_pose(u32 art, struct HeroBody *body, s32 x, s32 y);
void set_body_part(struct HeroBody *body, u32 art, s32 x, s32 y, u32 extra);
void obj_free_parts(struct HeroBody *body);
void display_flush(void);
void sprite_objects_draw_obj_tiles(u32 a, u32 b);
void set_motion_speed_aux(struct HeroBody *body, u32 speed, u32 aux);
u32 find_frame_entry(struct HeroBody *obj, u32 index);
void enter_finish_rebuild(struct HeroBody *body);

/* Hero pair state: once hero A's animation has finished, sets slot 0xAF (set_slot_value), starts
   A's action 2 of 0x2033 and B's action 7 of 0x2062 (each only while active, sprite mode 1) and
   goes on to hero_b_sprite_fx_partner_target. */
void hero_pair_anim_start(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct HeroBody *body_a = &world->heroA->body;
    struct HeroBody *body_b = &world->heroB->body;

    if ((body_a->sprite->flags & HERO_SPRITE_ANIM_DONE) != 0)
    {
        set_slot_value(0xAF, -1);
        if (HERO_MODE_ACTIVE(body_a->mode))
        {
            sprite_set_action(body_a, 2, 0x2033, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            sprite_set_action(body_b, 7, 0x2062, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
        }
        body->state = hero_b_sprite_fx_partner_target;
    }
}

/* State after hero_pair_anim_start: once hero B's animation has finished, plays sound 0x7E
   (set_slot_value), spawns effect 0 at B's feet, switches B to action 8 of 0x2062 and sends
   it towards the partner's spot (8 to its left, height 8) at speed 0x200, then goes on to
   hero_b_active_link_partner. */
void hero_b_sprite_fx_partner_target(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct HeroBody *body_b = &world->heroB->body;
    struct HeroBody *partner = body->partner;

    if ((body_b->sprite->flags & HERO_SPRITE_ANIM_DONE) != 0)
    {
        set_slot_value(0x7E, -1);
        /* positions are 24.8 fixed point */
        effect_spawn_at(0, body_b->posX / 0x100, body_b->posY / 0x100, body_b->posZ / 0x100, body_b);
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            sprite_set_action(body_b, 8, 0x2062, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 0;
        }
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            place_targets(body_b, partner->targetX / 0x100 + partner->ofsX - 8, partner->targetY / 0x100 + partner->ofsY, 8, -1);
            set_motion_start(body_b, 0x200);
        }
        body->state = hero_b_active_link_partner;
    }
}

/* Moves hero B towards the partner set up by hero_b_sprite_fx_partner_target. Once the motion
   is done: slot 0x63, resets B's scale, resolves B's contact with the partner and counts it,
   spawns effect 0x6E3 at the partner (height 20) if its data has flagsF6 > 0 or flags111 bit 1,
   plays B's action 9 of 0x2062 and goes on to hero_b_sprite_place_height. */
void hero_b_active_link_partner(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct HeroBody *body_b = &world->heroB->body;
    struct HeroBody *partner = body->partner;
    struct HeroData *partnerData;
    s32 fxX;
    s32 fxY;

    step_motion(body_b);
    if ((body_b->flags & HERO_FLAG_MOTION_DONE) != 0)
    {
        set_slot_value(0x63, -1);
        ((struct HeroCcBits *)&body_b->cc)->low = 0;
        body_b->scale = 0x100;
        resolve_contact_action(body_b, partner, 0, 0, 0x0403208A);
        body_b->count++;
        partnerData = partner->data;
        if (partnerData->flagsF6 > 0 || (partnerData->flags111 & 2) != 0)
        {
            /* partner's spot, 24.8 fixed point to whole units */
            fxX = partner->targetX / 0x100 + partner->ofsX;
            fxY = partner->targetY / 0x100 + partner->ofsY;
            effect_spawn_at(0x6E3, fxX, fxY, 20, partner);
        }
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            sprite_set_action(body_b, 9, 0x2062, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 0;
        }
        body->state = hero_b_sprite_place_height;
    }
}

/* State after hero_b_active_link_partner: once hero B's animation has finished, sends B on a hop to
   12 px left of its own spot (height its z + 4, motion 0x500), plays action 0 of 0x2005 and goes on
   to hero_b_active_fx, which waits for the landing. */
void hero_b_sprite_place_height(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct HeroBody *body_b = &world->heroB->body;

    if ((body_b->sprite->flags & HERO_SPRITE_ANIM_DONE) != 0)
    {
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            /* 24.8 fixed point to whole units */
            place_targets(body_b, body_b->targetX / 0x100 + body_b->ofsX - 0xC, body_b->targetY / 0x100 + body_b->ofsY, 0, -1);
            body_b->height = body_b->targetZ / 0x100 + 4;
            set_motion_offsets(body_b, 0x500);
        }
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            sprite_set_action(body_b, 0, 0x2005, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
        }
        body->state = hero_b_active_fx;
    }
}

/* Once hero B's animation has finished, sends each active hero (B first, then A) back to its home
   spot, gives it its idle pose facing along its span, and hands it to enter_finish_rebuild. */
void hero_pair_place_finish_b_sprite(void)
{
    struct World *world = gWorld;
    struct HeroBody *body_a = &world->heroA->body;
    struct HeroBody *body_b = &world->heroB->body;
    struct HeroData *data;
    struct HeroSprite *sprite;
    struct HeroSpan *span;
    s32 flip;

    if ((body_b->sprite->flags & HERO_SPRITE_ANIM_DONE) != 0)
    {
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            /* home position, 24.8 fixed point to whole units */
            data = body_b->data;
            place_targets(body_b, data->targetX / 0x100, data->targetY / 0x100, data->targetH / 0x100, -1);
            set_motion_start(body_b, 0x400);
        }
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            if (body_b->data->poseIndex == -1)
                sprite_set_action(body_b, 0, 0x2002, 0);
            else
                sprite_set_action(body_b, 0, 0x204F, 0);
            sprite = body_b->sprite;
            {
                flip = 0;
                span = &body_b->span;
                if (span->a >= span->b)
                    flip = 1;
                ((struct HeroMirrorBits *)&sprite->mirror)->flip = flip;
            }
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 0;
        }
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            body_b->state = NULL;
            enter_finish_rebuild(body_b);
        }
        if (HERO_MODE_ACTIVE(body_a->mode))
        {
            data = body_a->data;
            place_targets(body_a, data->targetX / 0x100, data->targetY / 0x100, data->targetH / 0x100, -1);
            set_motion_start(body_a, 0x400);
        }
        if (HERO_MODE_ACTIVE(body_a->mode))
        {
            if (body_a->data->poseIndex == -1)
                sprite_set_action(body_a, 0, 0x2002, 0);
            else
                sprite_set_action(body_a, 0, 0x204F, 0);
            sprite = body_a->sprite;
            {
                flip = 0;
                span = &body_a->span;
                if (span->a >= span->b)
                    flip = 1;
                ((struct HeroMirrorBits *)&sprite->mirror)->flip = flip;
            }
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        if (HERO_MODE_ACTIVE(body_a->mode))
        {
            body_a->state = NULL;
            enter_finish_rebuild(body_a);
        }
    }
}

/* Hero pair state: hero A (while active) plays action 2 with animation 0x2024 and hero B action 1
   with 0x2062, both in sprite mode 1; spawns effect 0x2FE6 owned by hero B at hero A's position
   moved 24 px in x and height, then goes on with hero_a_wait_visible_anim_24. */
void hero_pair_fx_anim_a_b(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct HeroBody *body_a = &world->heroA->body;
    struct HeroBody *body_b = &world->heroB->body;

    if (HERO_MODE_ACTIVE(body_a->mode))
    {
        sprite_set_action(body_a, 2, 0x2024, 0);
        ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
    }
    if (HERO_MODE_ACTIVE(body_b->mode))
    {
        sprite_set_action(body_b, 1, 0x2062, 0);
        ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
    }
    effect_spawn_at(0x2FE6, body_a->posX / 0x100 + 0x18, body_a->posY / 0x100, body_a->posZ / 0x100 + 0x18, body_b);
    body->state = hero_a_wait_visible_anim_24;
}

/*
 * Once hero A's animation has finished: plays sound 0x2B, puts hero B in action 2 (0x2062), and
 * sends hero A on a jump (peak height 0x28, speed 0x399) to 24 px left of its partner's target
 * point, in action 6 (0x2024). Then hands over to hero_a_partner_action_b054.
 */
void hero_a_sprite_place_partner(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct HeroBody *body_a = &world->heroA->body;
    struct HeroBody *body_b = &world->heroB->body;
    struct HeroBody *partner = body->partner;

    /* sprite flags bit 3: the animation has finished */
    if ((body_a->sprite->flags & 8) != 0)
    {
        set_slot_value(0x2B, -1);
        /* mode bits 1-2 are 2 or 4 while the body is on the field */
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            sprite_set_action(body_b, 2, 0x2062, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 0;
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            u16 peak;

            place_targets(body_a, partner->targetX / 0x100 + partner->ofsX - 0x18, partner->targetY / 0x100 + partner->ofsY, 0, -1);
            peak = 0x28;
            body_a->height = peak;
            set_motion_offsets(body_a, 0x399);
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            sprite_set_action(body_a, 6, 0x2024, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
        body_a->timer &= 0xFFF;
        body->state = hero_a_partner_action_b054;
    }
}

ASM_FUNC("asm/code_080BA92C/hero_a_partner_action_b054.s", void hero_a_partner_action_b054(struct HeroBody *body))

/* Moves hero A on; once its motion is done: sound 0xAF, hero A plays action 7 and hero B action 0xB
   (both one-shot), effect 0x6E3 at hero B (0x1C px ahead, 0x18 up), and hero A starts a 5-frame step
   before the pair goes on to sprite_fx_hero_a_active_link_partner. */
void hero_a_active_fx_start_step(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct HeroBody *body_a = &world->heroA->body;
    struct HeroBody *body_b = &world->heroB->body;

    step_motion(body_a);
    if ((body_a->flags & HERO_FLAG_MOTION_DONE) != 0)
    {
        set_slot_value(0xAF, -1);
        if (HERO_MODE_ACTIVE(body_a->mode))
        {
            sprite_set_action(body_a, 7, 0x2024, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            sprite_set_action(body_b, 0xB, 0x2062, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
        }
        effect_spawn_at(0x6E3, body_b->posX / 0x100 + 0x1C, body_b->posY / 0x100, body_b->posZ / 0x100 + 0x18, body_b);
        if (HERO_MODE_ACTIVE(body_a->mode))
        {
            body_a->rate = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(0x4000, 5);
            body_a->progress = 0;
            ((struct HeroFlagBits *)&body_a->flags)->active = 0;
        }
        body->state = sprite_fx_hero_a_active_link_partner;
    }
}

/* Advances hero A's progress towards the partner. Once it's done: plays sound 0x7D, resets A's scale,
   resolves A's contact with the partner and counts it, spawns effect 0x2FE6 at the partner (0x14 to
   its left, height 16) if its data has flagsF6 > 0 or flags111 bit 1, and goes on to
   hero_a_sprite_place_pose5. */
void sprite_fx_hero_a_active_link_partner(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct HeroBody *body_a = &world->heroA->body;
    struct HeroBody *partner = body->partner;
    struct HeroData *partnerData;

    state_stack_advance_progress(body_a);
    if ((body_a->flags & HERO_FLAG_MOTION_DONE) != 0)
    {
        set_slot_value(0x7D, -1);
        ((struct HeroCcBits *)&body_a->cc)->low = 0;
        body_a->scale = 0x100;
        resolve_contact_action(body_a, partner, 0, 0, 0x0403218A);
        body_a->count++;
        partnerData = partner->data;
        if (partnerData->flagsF6 > 0 || (partnerData->flags111 & 2) != 0)
        {
            /* partner's spot, 24.8 fixed point to whole units */
            s32 fxX = partner->targetX / 0x100 + partner->ofsX - 0x14;
            s32 fxY = partner->targetY / 0x100 + partner->ofsY;
            effect_spawn_at(0x2FE6, fxX, fxY, 0x10, partner);
        }
        body->state = hero_a_sprite_place_pose5;
    }
}

/* Once hero A's animation has finished, sends it on a jump (speed 0x500) to 32 px left of its own
   target point, landing 4 above it, plays action 5 one-shot (image 0x2000, or 0x204D when its data
   has a pose index) and goes on to hero_pair_a_active_place_finish. */
void hero_a_sprite_place_pose5(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct HeroBody *body_a = &world->heroA->body;

    if ((body_a->sprite->flags & HERO_SPRITE_ANIM_DONE) != 0)
    {
        if (HERO_MODE_ACTIVE(body_a->mode))
        {
            /* 24.8 fixed point to whole units */
            place_targets(body_a, body_a->targetX / 0x100 + body_a->ofsX - 0x20, body_a->targetY / 0x100 + body_a->ofsY, 0, -1);
            body_a->height = body_a->targetZ / 0x100 + 4;
            set_motion_offsets(body_a, 0x500);
        }
        if (HERO_MODE_ACTIVE(body_a->mode))
        {
            if (body_a->data->poseIndex == -1)
                sprite_set_action(body_a, 5, 0x2000, 0);
            else
                sprite_set_action(body_a, 5, 0x204D, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
        body->state = hero_pair_a_active_place_finish;
    }
}

/* Steps hero A's motion; once it has finished, hands hero B to hero_b_anim_c_then_clear, then sends
   hero A back to its home spot, gives it its idle pose facing along its span, and hands it to
   enter_finish_rebuild. */
void hero_pair_a_active_place_finish(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct HeroBody *body_a = &world->heroA->body;
    struct HeroBody *body_b = &world->heroB->body;
    struct HeroData *data;
    struct HeroSprite *sprite;
    struct HeroSpan *span;
    s32 flip;

    step_motion(body_a);
    if ((body_a->flags & HERO_FLAG_MOTION_DONE) != 0)
    {
        if (HERO_MODE_ACTIVE(body_b->mode))
            body_b->state = hero_b_anim_c_then_clear;
        if (HERO_MODE_ACTIVE(body_a->mode))
        {
            /* home position, 24.8 fixed point to whole units */
            data = body_a->data;
            place_targets(body_a, data->targetX / 0x100, data->targetY / 0x100, data->targetH / 0x100, -1);
            set_motion_start(body_a, 0x400);
        }
        if (HERO_MODE_ACTIVE(body_a->mode))
        {
            if (body_a->data->poseIndex == -1)
                sprite_set_action(body_a, 0, 0x2002, 0);
            else
                sprite_set_action(body_a, 0, 0x204F, 0);
            sprite = body_a->sprite;
            {
                flip = 0;
                span = &body_a->span;
                if (span->a >= span->b)
                    flip = 1;
                ((struct HeroMirrorBits *)&sprite->mirror)->flip = flip;
            }
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        if (HERO_MODE_ACTIVE(body_a->mode))
        {
            body_a->state = NULL;
            enter_finish_rebuild(body_a);
        }
    }
}

/* Once hero B's animation has finished: plays sound 0x2B, gives B action 5 (its pose variant)
   facing mirrored, sends it to 24 px left of its target, 32 px up, and moves on to
   hero_b_active_place_finish. */
void hero_b_sprite_place_mirror(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct HeroBody *body_b = &world->heroB->body;

    if ((body_b->sprite->flags & HERO_SPRITE_ANIM_DONE) != 0)
    {
        set_slot_value(0x2B, -1);
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            if (body_b->data->poseIndex == -1)
                sprite_set_action(body_b, 5, 0x2000, 0);
            else
                sprite_set_action(body_b, 5, 0x204D, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
        }
        ((struct HeroMirrorBits *)&body_b->sprite->mirror)->flip = 1;
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            /* 24.8 fixed point to whole units */
            place_targets(body_b, body_b->targetX / 0x100 + body_b->ofsX - 0x18, body_b->targetY / 0x100 + body_b->ofsY, 0, -1);
            body_b->height = body_b->targetZ / 0x100 + 0x20;
            set_motion_offsets(body_b, 0x600);
        }
        body->state = hero_b_active_place_finish;
    }
}

/* Steps hero B's motion; once it has finished, sends hero B back to its home spot, gives it its idle
   pose facing along its span, and hands it to enter_finish_rebuild. */
void hero_b_active_place_finish(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct HeroBody *body_b = &world->heroB->body;
    struct HeroData *data;
    struct HeroSprite *sprite;
    struct HeroSpan *span;
    s32 flip;

    step_motion(body_b);
    if ((body_b->flags & HERO_FLAG_MOTION_DONE) != 0)
    {
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            /* home position, 24.8 fixed point to whole units */
            data = body_b->data;
            place_targets(body_b, data->targetX / 0x100, data->targetY / 0x100, data->targetH / 0x100, -1);
            set_motion_start(body_b, 0x400);
        }
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            if (body_b->data->poseIndex == -1)
                sprite_set_action(body_b, 0, 0x2002, 0);
            else
                sprite_set_action(body_b, 0, 0x204F, 0);
            sprite = body_b->sprite;
            {
                flip = 0;
                span = &body_b->span;
                if (span->a >= span->b)
                    flip = 1;
                ((struct HeroMirrorBits *)&sprite->mirror)->flip = flip;
            }
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 0;
        }
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            body_b->state = NULL;
            enter_finish_rebuild(body_b);
        }
    }
}

/* Starts hero A's jump onto the partner body: resets hero B's effect animation, plays sound 0x62,
 * sets A's jump pose (action 9, image 0x2033), aims A at its own x/y on top of the partner (its top
 * height + 6), with a peak of top + 0x900 / (top^2 / 64 + 0x30), and moves on to
 * hero_a_jump_step_bc114. */
void hero_a_place_jump_arc(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct HeroBody *body_a = &world->heroA->body;
    struct HeroBody *body_b = &world->heroB->body;
    struct HeroBody *partner = body->partner;
    s32 top;
    s32 topSquared;
    s32 divisor;
    s32 (*div)(s32, s32);

    if (HERO_MODE_ACTIVE(body_b->mode))
        body_b->state = hero_b_fx_anim_reset;
    set_slot_value(0x62, -1);
    if (HERO_MODE_ACTIVE(body_a->mode))
    {
        sprite_set_action(body_a, 9, 0x2033, 0);
        ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
    }
    if (HERO_MODE_ACTIVE(body_a->mode))
    {
        place_targets(body_a, body_a->targetX / 0x100, body_a->targetY / 0x100,
                      partner->targetZ / 0x100 + partner->ofsZ + 6, -1);
        /* the partner's top: its own z offset if it is its data's main body, else its frame's */
        if (partner == (struct HeroBody *)((u8 *)partner->data + 8))
        {
            top = partner->ofsZ;
        }
        else
        {
            find_frame_entry(partner, 0);
            top = partner->unk47;
        }
        top += partner->targetZ / 0x100;
        div = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div);
        topSquared = top * top;
        if (topSquared < 0)
            topSquared += 0x3F;
        divisor = (topSquared >> 6) + 0x30;
        top += div(0x900, divisor);
        body_a->height = top;
        set_motion_offsets(body_a, 0x300);
    }
    body_a->timer &= 0xFFF;
    body->state = hero_a_jump_step_bc114;
}

ASM_FUNC("asm/code_080BA92C/hero_a_jump_step_bc114.s", void hero_a_jump_step_bc114(struct HeroBody *body))
