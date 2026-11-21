#include "global.h"
#include "ram_routines.h"

struct HeroSprite
{
    u8 filler0[0x11];
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

struct HeroFlag111Bits
{
    u8 unk0 : 1;
    u8 bit1 : 1;
    u8 unk2 : 6;
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
    s16 unkF6;    /* hp */
    u8 filler0F8[0x111 - 0xF8];
    u8 flags111;  /* bit 1: damage that would defeat it doesn't */
    u8 filler112[0x114 - 0x112];
    u32 poseValue;
    u8 filler118[0x11E - 0x118];
    u16 flags11E;
};

struct HeroBody
{
    struct HeroBody *link;
    u8 filler04[0x08 - 0x04];
    struct HeroSprite *sprite;
    u8 filler0C[0x10 - 0x0C];
    s32 x;
    s32 y;
    s32 z;
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
    u8 filler70[0x74 - 0x70];
    u8 unk74;
    u8 unk75;
    u8 mode;
    u8 filler77[0x79 - 0x77];
    u8 flags;
    u16 rate;
    s16 frame;
    s16 progress;
    u8 filler80[0x84 - 0x80];
    struct HeroSpan span;
    u8 filler88[0x92 - 0x88];
    u16 height;
    u8 filler94[0xA4 - 0x94];
    s32 unkA4;
    s32 unkA8;
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
    u8 filler0B;
    u8 flags0C;
    u8 filler0D[0x70 - 0x0D];
    struct Hero *heroA;
    struct Hero *heroB;
    struct HeroBody *layers;
    u8 filler7C[0x24C - 0x7C];
    void *focus24C;
};

struct KeyState
{
    u8 filler0[0x3E];
    u16 pressed;
};

/* Body mode bits 1-2: 2 or 4 means the body is active on the field. */
#define HERO_MODE_ACTIVE(mode) (((mode) & 6) == 2 || ((mode) & 6) == 4)

/* HeroBody.flags bit step_motion sets when the body has reached its motion target. */
#define HERO_FLAG_MOTION_DONE 0x20

/* HeroData.flags111 bit: damage that would defeat it doesn't. */
#define HERO_DATA_FLAG_SURVIVES 0x02

/* HeroSprite.flags bit set when the current animation has finished. */
#define HERO_SPRITE_ANIM_DONE 8

/* sprite_set_action ids for a hero standing still, without and with a pose. */
#define HERO_ACTION_STAND 0x2002
#define HERO_ACTION_STAND_POSED 0x204F

extern struct World *gWorld;
extern u8 *gParty;
extern struct KeyState gSys;

void state_stack_advance_progress(struct HeroBody *body);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void set_slot_value(u32 id, s32 value);
void step_motion(struct HeroBody *body);
void set_cur_flags_both(u32 value);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void enter_finish_rebuild(struct HeroBody *body);
void set_motion_start(struct HeroBody *body, u32 speed);
void set_motion_offsets(struct HeroBody *body, u32 offsets);
void start_action_5(struct HeroBody *body);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 h, struct HeroBody *owner);
void schedule_pose_task(struct HeroBody *obj);
void resolve_contact_action(struct HeroBody *a, struct HeroBody *b, u32 c, u32 d, u32 e);

void hero_pair_b_follow_partner(struct HeroBody *body);
void hero_b_active_place_action(struct HeroBody *body);
void hero_a_place_pose_finish(struct HeroBody *body);
void hero_b_fx_and_end(struct HeroBody *body);
void hero_b_step_window_bcd28(struct HeroBody *body);
void hero_a_sprite_place_pose6(struct HeroBody *body);
void hero_a_slot_pose_anim(struct HeroBody *body);
void hero_b_step_place_pose_finish(struct HeroBody *body);
void hero_a_press_link_080bdbb0(struct HeroBody *body);
void hero_a_place_at_partner_rate(struct HeroBody *owner);
void hero_a_wait_visible_anim_33(struct HeroBody *body);
void hero_a_active_slot_pose_anim(struct HeroBody *body);
void body_set_pose(u32 art, struct HeroBody *body, s32 x, s32 y);
void obj_free_parts(struct HeroBody *body);
void display_flush(void);
void obj_stop(struct HeroBody *obj);
void set_motion_speed_aux(struct HeroBody *body, u32 speed, u32 aux);
void step_motion_scaled(struct HeroBody *body);
void set_body_part(struct HeroBody *body, u32 art, s32 x, s32 y, u32 extra);
s32 obj_last_x(void *obj);
s32 obj_last_y(void *obj);

/* Hero A drops onto its target (body->partner). The frame its height crosses 4 pixels below the
   target's top, the attack hits (resolve_contact_action) and a target that survives gets effect
   0x6E3. When the drop ends: sound 0x2E, landing dust (effect 0x1E), action 0x2033, then
   hero_a_wait_visible_anim_33. */
void hero_a_z_cross_link_fx(struct HeroBody *body)
{
    struct Hero *heroA = gWorld->heroA;
    struct HeroBody *a = &heroA->body;
    struct HeroBody *target = body->partner;
    s32 hitZ;
    s32 x;
    s32 y;
    s32 mode;

    step_motion_scaled(a);
    hitZ = target->targetZ + ((s32)target->ofsZ << 8) - 0x400;
    if ((a->z - hitZ) * (a->targetZ - hitZ) < 0)
    {
        ((struct HeroCcBits *)&heroA->body.cc)->low = 0;
        heroA->body.scale = 0x100;
        resolve_contact_action(a, target, 0, 0, 0x0403228A);
        a->count++;
        if (target->data->unkF6 > 0 || (target->data->flags111 & HERO_DATA_FLAG_SURVIVES))
        {
            x = target->targetX / 0x100 + target->ofsX;
            y = target->targetY / 0x100 + target->ofsY;
            effect_spawn_at(0x6E3, x, y, 0x14, target);
        }
    }
    if (a->flags & HERO_FLAG_MOTION_DONE)
    {
        set_slot_value(0x2E, -1);
        effect_spawn_at(0x1E, a->x / 0x100, a->y / 0x100 + 2, a->z / 0x100, a);
        mode = a->mode & 6;
        if (mode == 2 || mode == 4)
        {
            sprite_set_action(a, 5, 0x2033, 0);
            ((struct HeroSpriteBits *)&a->sprite->flags)->mode = 0;
        }
        body->state = hero_a_wait_visible_anim_33;
    }
}

// Once hero A's sprite has flag 8 set, both heroes (B first, each only in body mode 2 or 4) are
// put at their data's target (8.8 fixed point), start moving at speed 0x400, take the standing
// action (0x2002 unposed, 0x204F posed) facing along their span, and hand over to
// finish_rebuild_checked.
void hero_pair_place_finish_a_sprite(void)
{
    struct Hero *hero_a;
    struct Hero *hero_b;
    struct HeroBody *body_a;
    struct HeroBody *body_b;
    struct HeroData *data;
    struct HeroSprite *sprite;
    struct HeroSpan *span;
    s32 tx;
    s32 ty;
    s32 th;
    s32 x;
    s32 y;
    s32 h;

    hero_a = gWorld->heroA;
    body_a = &hero_a->body;
    hero_b = gWorld->heroB;
    body_b = &hero_b->body;
    if ((body_a->sprite->flags & 8) != 0)
    {
        // Hero B: place at the target and start moving.
        if ((hero_b->body.mode & 6) == 2 || (hero_b->body.mode & 6) == 4)
        {
            data = body_b->data;
            tx = data->targetX;
            if (tx < 0)
                tx += 0xFF;
            x = tx >> 8;
            ty = data->targetY;
            if (ty < 0)
                ty += 0xFF;
            y = ty >> 8;
            th = data->targetH;
            if (th < 0)
                th += 0xFF;
            h = th >> 8;
            place_targets(body_b, x, y, h, -1);
            set_motion_start(body_b, 0x400);
        }
        // Standing action, mirrored when span.a >= span.b.
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            if (body_b->data->poseIndex == -1)
                sprite_set_action(body_b, 0, 0x2002, 0);
            else
                sprite_set_action(body_b, 0, 0x204F, 0);
            sprite = body_b->sprite;
            {
                s32 flip = 0;
                span = &body_b->span;
                if (span->a >= span->b)
                    flip = 1;
                ((struct HeroMirrorBits *)&sprite->mirror)->flip = flip;
            }
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 0;
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            body_b->state = 0;
            enter_finish_rebuild(body_b);
        }

        // Hero A: the same.
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            data = body_a->data;
            tx = data->targetX;
            if (tx < 0)
                tx += 0xFF;
            x = tx >> 8;
            ty = data->targetY;
            if (ty < 0)
                ty += 0xFF;
            y = ty >> 8;
            th = data->targetH;
            if (th < 0)
                th += 0xFF;
            h = th >> 8;
            place_targets(body_a, x, y, h, -1);
            set_motion_start(body_a, 0x400);
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            if (body_a->data->poseIndex == -1)
                sprite_set_action(body_a, 0, 0x2002, 0);
            else
                sprite_set_action(body_a, 0, 0x204F, 0);
            sprite = body_a->sprite;
            {
                s32 flip = 0;
                span = &body_a->span;
                if (span->a >= span->b)
                    flip = 1;
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

/* Hero B's part of hero A's jump onto the partner (set by hero_a_place_jump_arc): sound 0x62,
   effect 0x5DF at hero B, and an active hero B plays action 3 (images 0x2062) in sprite mode 1.
   Clears its unk74/unk75, then waits in hero_b_active_place_action. */
void hero_b_fx_anim_reset(struct HeroBody *body)
{
    struct HeroBody *body_b = &gWorld->heroB->body;
    u32 mode;

    set_slot_value(0x62, -1);
    effect_spawn_at(0x5DF, body_b->x / 0x100, body_b->y / 0x100, body_b->z / 0x100, body_b);
    mode = body_b->mode & 6;
    if (mode == 2 || mode == 4)
    {
        sprite_set_action(body_b, 3, 0x2062, 0);
        ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
    }
    body_b->unk74 = 0;
    body_b->unk75 = 0;
    body->state = hero_b_active_place_action;
}

/* Once hero B's animation has finished: sound 0x53, and an active hero B hops (arc 0x18 high, speed
   0x400) back to 0x20 pixels left of its own target plus its offsets, then plays action 5 and goes
   on with hero_b_fx_and_end. */
void hero_b_active_place_action(struct HeroBody *body)
{
    struct Hero *hero_b;
    struct HeroBody *body_b;
    s32 tx;
    s32 ty;
    s32 x;
    s32 y;
    u16 arcHeight;

    hero_b = gWorld->heroB;
    body_b = &hero_b->body;
    if ((body_b->sprite->flags & HERO_SPRITE_ANIM_DONE) != 0)
    {
        set_slot_value(0x53, -1);
        if (HERO_MODE_ACTIVE(hero_b->body.mode))
        {
            // Own target, 8.8 fixed point rounded toward 0.
            tx = body_b->targetX;
            if (tx < 0)
                tx += 0xFF;
            x = (tx >> 8) + hero_b->body.ofsX - 0x20;
            ty = body_b->targetY;
            if (ty < 0)
                ty += 0xFF;
            y = (ty >> 8) + hero_b->body.ofsY;
            place_targets(body_b, x, y, 0, -1);
            arcHeight = 0x18;
            hero_b->body.height = arcHeight;
            set_motion_offsets(body_b, 0x400);
        }
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            body_b->state = hero_b_fx_and_end;
            start_action_5(body_b);
        }
    }
}

void hero_a_ready_place_mode(struct HeroBody *body)
{
    struct Hero *hero;
    struct HeroBody *body_a;
    u8 mode;
    s32 tx;
    s32 ty;
    s32 x;
    s32 y;
    u16 height;

    mode = gWorld->modeByte;
    gWorld->modeByte = mode | 0xC0;
    hero = gWorld->heroA;
    body_a = &hero->body;
    if ((hero->body.mode & 6) == 2 || (hero->body.mode & 6) == 4)
    {
        tx = body_a->targetX;
        if (tx < 0)
            tx += 0xFF;
        x = (tx >> 8) + body_a->ofsX;
        ty = body_a->targetY;
        if (ty < 0)
            ty += 0xFF;
        y = (ty >> 8) + body_a->ofsY;
        place_targets(body_a, x, y, 0x17, -1);
        height = 0x18;
        hero->body.height = height;
        set_motion_offsets(body_a, 0x300);
    }
    body->state = hero_pair_b_follow_partner;
}

/* Moves hero A on to its jump target. Once it gets there: hero A loops action 1 of sprite 0x2024,
   hero B (if active) runs to 0x18 px right of the target (body->partner) at speed 0x166, hero B's
   timer loses its top 4 bits, and hero_b_step_window_bcd28 takes over. */
void hero_pair_b_follow_partner(struct HeroBody *body)
{
    struct Hero *hero_a;
    struct HeroBody *body_a;
    struct HeroBody *body_b;
    struct HeroBody *target;
    s32 tx;
    s32 ty;
    s32 x;
    s32 y;

    hero_a = gWorld->heroA;
    body_a = &hero_a->body;
    body_b = &gWorld->heroB->body;
    target = body->partner;
    step_motion(body_a);
    if ((hero_a->body.flags & HERO_FLAG_MOTION_DONE) != 0)
    {
        if (HERO_MODE_ACTIVE(hero_a->body.mode))
        {
            sprite_set_action(body_a, 1, 0x2024, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            /* 8.8 fixed point to pixels, rounding towards zero */
            tx = target->targetX;
            if (tx < 0)
                tx += 0xFF;
            x = (tx >> 8) + target->ofsX + 0x18;
            ty = target->targetY;
            if (ty < 0)
                ty += 0xFF;
            y = (ty >> 8) + target->ofsY;
            place_targets(body_b, x, y, 0, -1);
            set_motion_start(body_b, 0x166);
        }
        body_b->timer = body_b->timer & 0xFFF;
        body->state = hero_b_step_window_bcd28;
    }
}

ASM_FUNC("asm/code_080BC734/hero_b_step_window_bcd28.s", void hero_b_step_window_bcd28(struct HeroBody *body))

/* Hero A moves on (state_stack_advance_progress) until its motion is done; then the hit lands on its
   target (body->partner): sound 0x7E, attack 0x0403238A resolved and counted, and a target that
   survives gets effect 0 at its position. Next state: hero_a_sprite_place_pose6. */
void hero_a_link_partner_fx(struct HeroBody *body)
{
    struct Hero *heroA;
    struct HeroBody *a;
    struct HeroBody *target;
    s32 tx;
    s32 ty;
    s32 x;
    s32 y;

    heroA = gWorld->heroA;
    a = &heroA->body;
    target = body->partner;
    state_stack_advance_progress(a);
    if ((heroA->body.flags & HERO_FLAG_MOTION_DONE) != 0)
    {
        set_slot_value(0x7E, -1);
        ((struct HeroCcBits *)&heroA->body.cc)->low = 0;
        a->scale = 0x100;
        resolve_contact_action(a, target, 0, 0, 0x0403238A);
        a->count = a->count + 1;
        if (target->data->unkF6 > 0 || (target->data->flags111 & HERO_DATA_FLAG_SURVIVES) != 0)
        {
            // Target position, 8.8 fixed point rounded toward 0.
            tx = target->targetX;
            if (tx < 0)
                tx += 0xFF;
            x = (tx >> 8) + target->ofsX;
            ty = target->targetY;
            if (ty < 0)
                ty += 0xFF;
            y = (ty >> 8) + target->ofsY;
            effect_spawn_at(0, x, y, 0, target);
        }
        body->state = hero_a_sprite_place_pose6;
    }
}

/* Once hero A's animation has finished: an active hero A hops (arc 2 above its target, speed 0x300)
   back to 0x20 pixels left of its own target plus its offsets, playing action 6 of its pose
   (0x2000, or 0x204D when it has a pose index). Then hero_a_slot_pose_anim lands it. */
void hero_a_sprite_place_pose6(struct HeroBody *body)
{
    struct Hero *hero_a;
    struct HeroBody *body_a;
    s32 tx;
    s32 ty;
    s32 tz;
    s32 x;
    s32 y;

    hero_a = gWorld->heroA;
    body_a = &hero_a->body;
    if ((body_a->sprite->flags & 8) != 0) /* animation done */
    {
        if (HERO_MODE_ACTIVE(hero_a->body.mode))
        {
            // Own target, 8.8 fixed point rounded toward 0.
            tx = body_a->targetX;
            if (tx < 0)
                tx += 0xFF;
            x = (tx >> 8) + hero_a->body.ofsX - 0x20;
            ty = body_a->targetY;
            if (ty < 0)
                ty += 0xFF;
            y = (ty >> 8) + hero_a->body.ofsY;
            place_targets(body_a, x, y, 0, -1);
            tz = body_a->targetZ;
            if (tz < 0)
                tz += 0xFF;
            hero_a->body.height = (tz >> 8) + 2;
            set_motion_offsets(body_a, 0x300);
        }
        if (HERO_MODE_ACTIVE(body_a->mode))
        {
            if (body_a->data->poseIndex == -1)
                sprite_set_action(body_a, 6, 0x2000, 0);
            else
                sprite_set_action(body_a, 6, 0x204D, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
        body->state = hero_a_slot_pose_anim;
    }
}

void hero_a_slot_pose_anim(struct HeroBody *body)
{
    struct Hero *hero_a;
    struct HeroBody *body_a;

    hero_a = gWorld->heroA;
    body_a = &hero_a->body;
    step_motion(body_a);
    if ((hero_a->body.flags & 0x20) != 0)
    {
        set_slot_value(0x2E, -1);
        if ((hero_a->body.mode & 6) == 2 || (hero_a->body.mode & 6) == 4)
        {
            if (body_a->data->poseIndex == -1)
                sprite_set_action(body_a, 8, 0x2000, 0);
            else
                sprite_set_action(body_a, 8, 0x204D, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
        body->state = hero_a_place_pose_finish;
    }
}

/*
 * Follows hero_a_slot_pose_anim: once hero A's animation has finished, an active hero A walks back
 * to its data's home spot (8.8 fixed point, speed 0x400), stands (posed when it has a pose) facing
 * along its span, and is handed to enter_finish_rebuild.
 */
void hero_a_place_pose_finish(struct HeroBody *body)
{
    struct Hero *hero_a;
    struct HeroBody *body_a;
    struct HeroData *data;
    struct HeroSprite *sprite;
    struct HeroSpan *span;
    s32 fixedX;
    s32 fixedY;
    s32 fixedH;
    s32 x;
    s32 y;
    s32 h;

    hero_a = gWorld->heroA;
    body_a = &hero_a->body;
    if ((body_a->sprite->flags & HERO_SPRITE_ANIM_DONE) != 0)
    {
        if (HERO_MODE_ACTIVE(body_a->mode))
        {
            data = body_a->data;
            fixedX = data->targetX;
            if (fixedX < 0)
                fixedX += 0xFF;
            x = fixedX >> 8;
            fixedY = data->targetY;
            if (fixedY < 0)
                fixedY += 0xFF;
            y = fixedY >> 8;
            fixedH = data->targetH;
            if (fixedH < 0)
                fixedH += 0xFF;
            h = fixedH >> 8;
            place_targets(body_a, x, y, h, -1);
            set_motion_start(body_a, 0x400);
        }
        if (HERO_MODE_ACTIVE(body_a->mode))
        {
            if (body_a->data->poseIndex == -1)
                sprite_set_action(body_a, 0, HERO_ACTION_STAND, 0);
            else
                sprite_set_action(body_a, 0, HERO_ACTION_STAND_POSED, 0);
            sprite = body_a->sprite;
            {
                s32 flip = 0;
                span = &body_a->span;
                if (span->a >= span->b)
                    flip = 1;
                ((struct HeroMirrorBits *)&sprite->mirror)->flip = flip;
            }
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        if (HERO_MODE_ACTIVE(body_a->mode))
        {
            body_a->state = 0;
            enter_finish_rebuild(body_a);
        }
    }
}

static inline void *focus_obj(void)
{
    return gWorld->focus24C;
}

/*
 * Waits for hero B's animation to finish. Then an active hero B is sent off to the right of
 * the focus object (0x110 pixels past its last X, at its own Y offset), stands (posed when it
 * has a pose) facing along its span, and the owner moves on to hero_b_step_place_pose_finish.
 */
void hero_b_place_last_pose(struct HeroBody *owner)
{
    struct World *world;
    struct Hero *hero_b;
    struct HeroBody *body_b;
    struct HeroSprite *sprite;
    struct HeroSpan *span;
    s32 offsetY;
    s32 x;
    s32 y;

    world = gWorld;
    hero_b = world->heroB;
    body_b = &hero_b->body;
    if ((body_b->sprite->flags & HERO_SPRITE_ANIM_DONE) != 0)
    {
        if (HERO_MODE_ACTIVE(hero_b->body.mode))
        {
            offsetY = body_b->targetY;
            if (offsetY < 0)
                offsetY += 0xFF;
            offsetY >>= 8;
            x = (s16)obj_last_x(focus_obj()) + 0x110;
            y = (s16)obj_last_y(focus_obj()) + offsetY;
            place_targets(body_b, x, y, 0, -1);
            set_motion_start(body_b, 0x400);
        }
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            if (body_b->data->poseIndex == -1)
                sprite_set_action(body_b, 0, HERO_ACTION_STAND, 0);
            else
                sprite_set_action(body_b, 0, HERO_ACTION_STAND_POSED, 0);
            sprite = body_b->sprite;
            {
                s32 flip = 0;
                span = &body_b->span;
                if (span->a >= span->b)
                    flip = 1;
                ((struct HeroMirrorBits *)&sprite->mirror)->flip = flip;
            }
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 0;
        }
        owner->state = hero_b_step_place_pose_finish;
    }
}

/*
 * Follows hero_b_place_last_pose: steps hero B's motion off to the right. Once it arrives, hero B
 * jumps 0x130 pixels back to the left and, while active on the field, walks back to its data's
 * home spot (speed 0x400), stands (posed when it has a pose) facing along its span, and is handed
 * to enter_finish_rebuild.
 */
void hero_b_step_place_pose_finish(struct HeroBody *body)
{
    struct Hero *hero_b;
    struct HeroBody *body_b;
    struct HeroData *data;
    struct HeroSprite *sprite;
    struct HeroSpan *span;
    s32 fixedX;
    s32 fixedY;
    s32 fixedH;
    s32 x;
    s32 y;
    s32 h;

    hero_b = gWorld->heroB;
    body_b = &hero_b->body;
    step_motion(body_b);
    if ((hero_b->body.flags & HERO_FLAG_MOTION_DONE) != 0)
    {
        body_b->x = body_b->x - 0x13000;
        body_b->targetX = body_b->x;
        if (HERO_MODE_ACTIVE(hero_b->body.mode))
        {
            data = body_b->data;
            fixedX = data->targetX;
            if (fixedX < 0)
                fixedX += 0xFF;
            x = fixedX >> 8;
            fixedY = data->targetY;
            if (fixedY < 0)
                fixedY += 0xFF;
            y = fixedY >> 8;
            fixedH = data->targetH;
            if (fixedH < 0)
                fixedH += 0xFF;
            h = fixedH >> 8;
            place_targets(body_b, x, y, h, -1);
            set_motion_start(body_b, 0x400);
        }
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            if (body_b->data->poseIndex == -1)
                sprite_set_action(body_b, 0, HERO_ACTION_STAND, 0);
            else
                sprite_set_action(body_b, 0, HERO_ACTION_STAND_POSED, 0);
            sprite = body_b->sprite;
            {
                s32 flip = 0;
                span = &body_b->span;
                if (span->a >= span->b)
                    flip = 1;
                ((struct HeroMirrorBits *)&sprite->mirror)->flip = flip;
            }
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 0;
        }
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            body_b->state = 0;
            enter_finish_rebuild(body_b);
        }
    }
}

/* Hero A's first hit on its target (owner->partner): the target can't be defeated by it (flags111
   bit 1), the attack 0x0403248A is resolved and counted, and a surviving target gets effect 0x733.
   Then the A-press count (unkA4) and its deadline (unkA8) are reset, an active hero A plays action
   0x2033, and the next state is hero_a_place_at_partner_rate. */
void hero_a_link_partner_anim(struct HeroBody *owner)
{
    struct Hero *heroA;
    struct HeroBody *a;
    struct HeroBody *target;
    s32 tx;
    s32 ty;
    s32 x;
    s32 y;

    heroA = gWorld->heroA;
    a = &heroA->body;
    target = owner->partner;
    ((struct HeroFlag111Bits *)&target->data->flags111)->bit1 = 1;
    ((struct HeroCcBits *)&heroA->body.cc)->low = 0;
    a->scale = 0x100;
    resolve_contact_action(a, target, 0, 0, 0x0403248A);
    a->count = a->count + 1;
    if (target->data->unkF6 > 0 || (target->data->flags111 & HERO_DATA_FLAG_SURVIVES) != 0)
    {
        tx = target->targetX;
        if (tx < 0)
            tx += 0xFF;
        x = (tx >> 8) + target->ofsX;
        ty = target->targetY;
        if (ty < 0)
            ty += 0xFF;
        y = (ty >> 8) + target->ofsY;
        effect_spawn_at(0x733, x, y, 0x10, target);
    }
    a->unkA8 = a->progress + 0x28000;
    a->unkA4 = 0;
    if (HERO_MODE_ACTIVE(a->mode))
    {
        sprite_set_action(a, 1, 0x2033, 0);
        ((struct HeroSpriteBits *)&a->sprite->flags)->mode = 0;
    }
    set_body_part(a, 1, 0x18, 4, 0);
    owner->state = hero_a_place_at_partner_rate;
}

/* State: if hero A is on the field, starts its move (speed 0x400) to its own target x/y at the
   partner's height; restarts its progress at a rate of 0x4000 / 50 a frame with the done flag
   (HERO_FLAG_MOTION_DONE) clear, sets its body part, and continues in hero_a_press_link_080bdbb0. */
void hero_a_place_at_partner_rate(struct HeroBody *owner)
{
    struct Hero *heroA;
    struct HeroBody *a;
    struct HeroBody *partner;
    s32 tx;
    s32 ty;
    s32 tz;
    s32 x;
    s32 y;
    s32 z;

    heroA = gWorld->heroA;
    a = &heroA->body;
    partner = owner->partner;
    if (HERO_MODE_ACTIVE(heroA->body.mode))
    {
        tx = a->targetX;
        if (tx < 0)
            tx += 0xFF;
        x = tx >> 8;
        ty = a->targetY;
        if (ty < 0)
            ty += 0xFF;
        y = ty >> 8;
        tz = partner->targetZ;
        if (tz < 0)
            tz += 0xFF;
        z = tz >> 8;
        place_targets(a, x, y, z, -1);
        set_motion_start(a, 0x400);
    }
    if (HERO_MODE_ACTIVE(a->mode))
    {
        a->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 50);
        a->progress = 0;
        ((struct HeroFlagBits *)&a->flags)->active = 0;
    }
    set_body_part(a, 1, 0x18, 4, 0);
    owner->state = hero_a_press_link_080bdbb0;
}

/* Full frame values for the three body chains, layer 0x10 for every listed sprite. */
static inline void frames_full(struct HeroBody *a, struct HeroBody *b, struct HeroBody *partner)
{
    struct HeroBody *p;

    p = a;
    while (p != 0)
    {
        p->frame = 0x100;
        p = p->next;
    }
    p = b;
    while (p != 0)
    {
        p->frame = 0x100;
        p = p->next;
    }
    p = partner;
    while (p != 0)
    {
        p->frame = 0x100;
        p = p->next;
    }
    p = gWorld->layers;
    while (p != 0)
    {
        if (p->sprite != 0)
            p->sprite->frame = 0x10;
        p = p->link;
    }
}

/* Hero A counts A presses; enough presses link it to the partner. Once active, both heroes are
   placed back at their targets (or hero A gets a target next to its own and the next state). */
NONMATCH("asm/code_080BC734/hero_a_press_link_080bdbb0.s", void hero_a_press_link_080bdbb0(struct HeroBody *body))
{
    struct Hero *hero_a;
    struct HeroBody *a;
    struct HeroBody *b;
    struct HeroBody *partner;
    struct HeroData *data;
    struct HeroSprite *sprite;
    struct HeroSpan *span;
    s32 tx;
    s32 ty;
    s32 th;
    s32 x;
    s32 y;
    s32 h;

    hero_a = gWorld->heroA;
    a = &hero_a->body;
    b = &gWorld->heroB->body;
    partner = body->partner;
    step_motion(a);
    set_body_part(a, 1, 0x18, 4, 0);
    if (gSys.pressed & 1)
        a->unkA4++;
    if ((a->frame == 0x100 && a->unkA4 > 0) || a->unkA4 > 2)
    {
        body_set_pose(2, a, -8, -0x24);
        ((struct HeroCcBits *)&a->cc)->low = 0;
        a->scale = 0x100;
        resolve_contact_action(a, partner, 0, 0, 0x0403248A);
        a->count++;
        if (partner->data->unkF6 > 0 || (partner->data->flags111 & 2) != 0)
        {
            tx = partner->targetX;
            if (tx < 0)
                tx += 0xFF;
            x = (tx >> 8) + partner->ofsX;
            ty = partner->targetY;
            if (ty < 0)
                ty += 0xFF;
            y = (ty >> 8) + partner->ofsY;
            effect_spawn_at(0x733, x, y, 0x10, partner);
        }
        a->unkA8 = a->progress + 0x28000;
        a->unkA4 = 0;
    }
    if (a->unkA8 < a->progress)
        ((struct HeroFlagBits *)&a->flags)->active = 1;
    if (!(a->flags & 0x20))
        return;

    set_slot_value(0x88, -1);
    if (gWorld->flags0C & 8)
        frames_full(a, b, partner);
    else
        frames_full(a, b, partner);
    display_flush();
    obj_free_parts(a);
    ((struct HeroFlag111Bits *)&partner->data->flags111)->bit1 = 0;
    if (partner->data->unkF6 == 0)
        obj_stop(partner);

    if ((b->mode & 6) == 2 || (b->mode & 6) == 4)
    {
        data = b->data;
        tx = data->targetX;
        if (tx < 0)
            tx += 0xFF;
        x = tx >> 8;
        ty = data->targetY;
        if (ty < 0)
            ty += 0xFF;
        y = ty >> 8;
        th = data->targetH;
        if (th < 0)
            th += 0xFF;
        h = th >> 8;
        place_targets(b, x, y, h, -1);
        set_motion_start(b, 0x400);
    }
    if ((b->mode & 6) == 2 || (b->mode & 6) == 4)
    {
        if (b->data->poseIndex == -1)
            sprite_set_action(b, 0, 0x2002, 0);
        else
            sprite_set_action(b, 0, 0x204F, 0);
        sprite = b->sprite;
        {
            s32 flip = 0;
            span = &b->span;
            if (span->a >= span->b)
                flip = 1;
            ((struct HeroMirrorBits *)&sprite->mirror)->flip = flip;
        }
        ((struct HeroSpriteBits *)&b->sprite->flags)->mode = 0;
    }
    if ((b->mode & 6) == 2 || (b->mode & 6) == 4)
    {
        b->state = 0;
        enter_finish_rebuild(b);
    }

    if (a->z == 0)
    {
        if ((a->mode & 6) == 2 || (a->mode & 6) == 4)
        {
            data = a->data;
            tx = data->targetX;
            if (tx < 0)
                tx += 0xFF;
            x = tx >> 8;
            ty = data->targetY;
            if (ty < 0)
                ty += 0xFF;
            y = ty >> 8;
            th = data->targetH;
            if (th < 0)
                th += 0xFF;
            h = th >> 8;
            place_targets(a, x, y, h, -1);
            set_motion_start(a, 0x400);
        }
        if ((a->mode & 6) == 2 || (a->mode & 6) == 4)
        {
            if (a->data->poseIndex == -1)
                sprite_set_action(a, 0, 0x2002, 0);
            else
                sprite_set_action(a, 0, 0x204F, 0);
            sprite = a->sprite;
            {
                s32 flip = 0;
                span = &a->span;
                if (span->a >= span->b)
                    flip = 1;
                ((struct HeroMirrorBits *)&sprite->mirror)->flip = flip;
            }
            ((struct HeroSpriteBits *)&a->sprite->flags)->mode = 0;
        }
        if ((a->mode & 6) == 2 || (a->mode & 6) == 4)
        {
            a->state = 0;
            enter_finish_rebuild(a);
        }
    }
    else
    {
        if ((a->mode & 6) == 2 || (a->mode & 6) == 4)
        {
            tx = a->targetX;
            if (tx < 0)
                tx += 0xFF;
            x = (tx >> 8) + a->ofsX;
            ty = a->targetY;
            if (ty < 0)
                ty += 0xFF;
            y = (ty >> 8) + a->ofsY;
            place_targets(a, x, y, 0, -1);
            set_motion_speed_aux(a, 0, 0x66);
        }
        if ((a->mode & 6) == 2 || (a->mode & 6) == 4)
        {
            if (a->data->poseIndex == -1)
                sprite_set_action(a, 6, 0x2000, 0);
            else
                sprite_set_action(a, 6, 0x204D, 0);
            ((struct HeroSpriteBits *)&a->sprite->flags)->mode = 1;
        }
        body->state = hero_a_active_slot_pose_anim;
    }
}
END_NONMATCH
