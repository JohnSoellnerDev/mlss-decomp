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
    u8 filler94[0xAC - 0x94];
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

struct HeroNode
{
    struct HeroNode *next;
    u8 filler4[0x08 - 0x04];
    struct HeroSprite *sprite;
};

struct World
{
    u8 filler0[0x0A];
    u8 modeByte;
    u8 filler0B;
    u8 flagsC;
    u8 filler0D;
    u8 byte0E;
    u8 filler0F[0x70 - 0x0F];
    struct Hero *heroA;
    struct Hero *heroB;
    struct HeroNode *nodes;
    u8 filler7C[0x2BF - 0x7C];
    u8 phaseByte;
};

/* Body mode bits 1-2: 2 or 4 means the body is active on the field. */
#define HERO_MODE_ACTIVE(mode) (((mode) & 6) == 2 || ((mode) & 6) == 4)

/* HeroBody.flags bit set by state_stack_advance_progress once progress passes 0x3FFF. */
#define HERO_FLAG_PROGRESS_DONE 0x20

/* HeroSprite.flags bit set when the current animation has finished. */
#define HERO_SPRITE_ANIM_DONE 8

/* sprite_set_action ids for a hero standing still, without and with a pose. */
#define HERO_ACTION_STAND 0x2002
#define HERO_ACTION_STAND_POSED 0x204F

/* HeroBody.timer bits 0-11 count how often the current move has repeated; the upper bits are flags. */
#define HERO_TIMER_COUNT_MASK 0xFFF

extern struct World *gWorld;
/* Most jitter hits hero A repeats (0: no limit), indexed by World.phaseByte bits 6-7. */
extern const u8 gPhaseTable[];
extern s8 gJitterTable1[];
extern s8 gJitterRange1[];
extern s8 gJitterTable2[];
extern s8 gJitterRange2[];

void step_motion(struct HeroBody *body);
void set_slot_value(u32 id, s32 value);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 z, struct HeroBody *owner);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void set_motion_start(struct HeroBody *body, u32 speed);
void set_motion_offsets(struct HeroBody *body, u32 offsets);
void enter_finish_rebuild(struct HeroBody *body);
void state_stack_advance_progress(struct HeroBody *body);
void hero_a_clear_partner_flag_anim(struct HeroBody *body);
void hero_a_place_near_partner_if_b_active(struct HeroBody *body);
void resolve_contact_action(struct HeroBody *a, struct HeroBody *b, u32 c, u32 d, u32 e);
u32 mt19937_next(void);
u32 rand_below(u32 range, u32 rnd);

void hero_a_sprite_active_place_target(struct HeroBody *body);
void hero_a_active_step_fx_anim(struct HeroBody *body);
void jitter_place_a_hero_pair_place_both_finish(struct HeroBody *body);
void hero_a_phase_place_or_next(struct HeroBody *body);
void hero_pair_timer_step_080c7104(struct HeroBody *body);
void hero_pair_b_advance_place_finish(struct HeroBody *body);
void hero_a_jitter_fx_b_step_start(struct HeroBody *body);

ASM_FUNC("asm/code_080C6344/hero_pair_timer_pose_6344.s", void hero_pair_timer_pose_6344(void))

/* Hero pair state: once hero A's sprite animation has finished, hero A (while active on the field)
 * plays action 5 / 0x2034 with sprite mode 0 and jumps (speed 0x400) to 9 px right of and 16 px
 * below its data's target, its arc peaking 32 px above the target height; then goes on to
 * hero_a_active_step_fx_anim. */
void hero_a_sprite_active_place_target(struct HeroBody *body)
{
    struct Hero *hero_a = gWorld->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct HeroData *data;
    s32 fixedX;
    s32 fixedY;
    s32 fixedH;
    s32 x;
    s32 y;
    s32 h;

    if ((body_a->sprite->flags & HERO_SPRITE_ANIM_DONE) != 0)
    {
        if (HERO_MODE_ACTIVE(hero_a->body.mode))
        {
            sprite_set_action(body_a, 5, 0x2034, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        if (HERO_MODE_ACTIVE(body_a->mode))
        {
            /* the target is 8.8 fixed point */
            data = body_a->data;
            fixedX = data->targetX;
            if (fixedX < 0)
                fixedX += 0xFF;
            x = (fixedX >> 8) + 9;
            fixedY = data->targetY;
            if (fixedY < 0)
                fixedY += 0xFF;
            y = (fixedY >> 8) + 16;
            fixedH = data->targetH;
            if (fixedH < 0)
                fixedH += 0xFF;
            h = fixedH >> 8;
            place_targets(body_a, x, y, h, -1);
            fixedH = body_a->targetZ;
            if (fixedH < 0)
                fixedH += 0xFF;
            body_a->height = (fixedH >> 8) + 32;
            set_motion_offsets(body_a, 0x400);
        }
        body->state = hero_a_active_step_fx_anim;
    }
}

/* Hero pair state: moves hero A along the jump hero_a_sprite_active_place_target started; when it
 * lands, plays sound 0x2E, spawns effect 0x1E where A stands, gives A (while active on the field)
 * action 7 / 0x2034 with sprite mode 1, and goes on to jitter_place_a_hero_pair_place_both_finish. */
void hero_a_active_step_fx_anim(struct HeroBody *body)
{
    struct Hero *hero_a = gWorld->heroA;
    struct HeroBody *body_a = &hero_a->body;
    s32 fixedX;
    s32 fixedY;
    s32 fixedZ;
    s32 x;
    s32 y;
    s32 z;

    step_motion(body_a);
    if ((hero_a->body.flags & HERO_FLAG_PROGRESS_DONE) != 0)
    {
        set_slot_value(0x2E, -1);
        /* the position is 8.8 fixed point */
        fixedX = body_a->posX;
        if (fixedX < 0)
            fixedX += 0xFF;
        x = fixedX >> 8;
        fixedY = body_a->posY;
        if (fixedY < 0)
            fixedY += 0xFF;
        y = fixedY >> 8;
        fixedZ = body_a->posZ;
        if (fixedZ < 0)
            fixedZ += 0xFF;
        z = fixedZ >> 8;
        effect_spawn_at(0x1E, x, y, z, body_a);
        if (HERO_MODE_ACTIVE(body_a->mode))
        {
            sprite_set_action(body_a, 7, 0x2034, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
        body->state = jitter_place_a_hero_pair_place_both_finish;
    }
}




/*
 * Hero pair state: once hero A's sprite animation has finished, for hero A and then hero B (each
 * only while active on the field) sends the hero to its data's target position (8.8 fixed point,
 * speed 0x400), sets the standing action (posed when it has a pose), faces it along its span,
 * clears its sprite mode and hands it to enter_finish_rebuild.
 */
void jitter_place_a_hero_pair_place_both_finish(struct HeroBody *body)
{
    struct Hero *hero_a = gWorld->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct HeroBody *body_b = &gWorld->heroB->body;
    struct HeroData *data;
    struct HeroSprite *sprite;
    struct HeroSpan *span;
    s32 fixedX;
    s32 fixedY;
    s32 fixedH;
    s32 x;
    s32 y;
    s32 h;

    if ((body_a->sprite->flags & HERO_SPRITE_ANIM_DONE) != 0)
    {
        /* hero A */
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

        /* hero B */
        if (HERO_MODE_ACTIVE(body_b->mode))
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

NONMATCH("asm/code_080C6344/hero_a_jitter_link_partner.s", void hero_a_jitter_link_partner(struct HeroBody *body))
{
    struct Hero *hero_a = gWorld->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct HeroBody *partner = body->partner;
    struct HeroData *data;
    s32 mask;
    s32 raw;
    s32 index;
    s32 jx;
    s32 jy;
    s32 x;
    s32 y;
    s32 tx;
    s32 ty;

    data = partner->data;
    data->flags111 |= 2;
    mask = 0xFFF;
    raw = hero_a->body.timer + 1;
    hero_a->body.timer = raw;
    index = mask & raw;
    if (index <= 4)
    {
        hero_a->body.jitterX = gJitterTable2[index * 2];
        hero_a->body.jitterY = gJitterTable2[(mask & hero_a->body.timer) * 2 + 1];
    }
    else
    {
        s8 *range = gJitterRange2;

        if (range[0] < range[2])
            jx = range[0] + rand_below(range[2] - range[0], mt19937_next());
        else
            jx = range[2] + rand_below(range[0] - range[2], mt19937_next());
        hero_a->body.jitterX = jx;

        if (range[1] < range[3])
            jy = range[1] + rand_below(range[3] - range[1], mt19937_next());
        else
            jy = range[3] + rand_below(range[1] - range[3], mt19937_next());
        body_a->jitterY = jy;
    }
    ((struct HeroCcBits *)&body_a->cc)->low = 0;
    body_a->scale = 0x100;
    resolve_contact_action(body_a, partner, 0, 0, 0x0403058B);
    body_a->count++;
    data = partner->data;
    if (data->unkF6 > 0 || (data->flags111 & 2) != 0)
    {
        tx = partner->targetX;
        if (tx < 0)
            tx += 0xFF;
        x = (tx >> 8) + partner->ofsX;
        ty = partner->targetY;
        if (ty < 0)
            ty += 0xFF;
        y = (ty >> 8) + partner->ofsY;
        effect_spawn_at(0x2E93, x, y, 0, partner);
    }
    if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
    {
        sprite_set_action(body_a, 6, 0x2034, 0);
        ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
    }
    body->state = hero_a_phase_place_or_next;
}
END_NONMATCH

/*
 * Hero pair state after a jitter hit: once hero A's animation has finished, ends the attack
 * (hero_a_clear_partner_flag_anim) when hero A has repeated the hit as often as gPhaseTable allows
 * for this level; otherwise sends hero A back up beside its data's target spot (32 above the ground,
 * speed 0x400) with action 5, keeps the repeat count and goes on to hero_pair_timer_step_080c7104.
 */
void hero_a_phase_place_or_next(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct Hero *hero_a = world->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct HeroData *data;
    s32 limit;
    s32 fixedX;
    s32 fixedY;
    s32 fixedZ;
    s32 x;
    s32 y;
    s32 z;

    if ((body_a->sprite->flags & HERO_SPRITE_ANIM_DONE) != 0)
    {
        limit = gPhaseTable[world->phaseByte >> 6];
        if (limit != 0 && limit <= (s32)(hero_a->body.timer & HERO_TIMER_COUNT_MASK))
        {
            if (HERO_MODE_ACTIVE(body->mode))
                body->state = hero_a_clear_partner_flag_anim;
        }
        else
        {
            if (HERO_MODE_ACTIVE(body_a->mode))
            {
                data = body_a->data;
                fixedX = data->targetX;
                if (fixedX < 0)
                    fixedX += 0xFF;
                x = (fixedX >> 8) + 9;
                fixedY = data->targetY;
                if (fixedY < 0)
                    fixedY += 0xFF;
                y = (fixedY >> 8) + 0x10;
                fixedZ = data->targetH;
                if (fixedZ < 0)
                    fixedZ += 0xFF;
                z = fixedZ >> 8;
                place_targets(body_a, x, y, z, -1);
                fixedZ = body_a->targetZ;
                if (fixedZ < 0)
                    fixedZ += 0xFF;
                body_a->height = (fixedZ >> 8) + 0x20;
                set_motion_offsets(body_a, 0x400);
            }
            if (HERO_MODE_ACTIVE(body_a->mode))
            {
                sprite_set_action(body_a, 5, 0x2034, 0);
                ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
            }
            body_a->timer = body_a->timer & HERO_TIMER_COUNT_MASK;
            body->state = hero_pair_timer_step_080c7104;
        }
    }
}

ASM_FUNC("asm/code_080C6344/hero_pair_timer_step_080c7104.s", void hero_pair_timer_step_080c7104(struct HeroBody *body))

/* Advances hero B's progress; once it is done, sends each active hero of the pair back to its
 * home spot, gives it its idle pose facing along its span, and hands it to enter_finish_rebuild. */
void hero_pair_b_advance_place_finish(struct HeroBody *body)
{
    struct Hero *hero_a = gWorld->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct Hero *hero_b = gWorld->heroB;
    struct HeroBody *body_b = &hero_b->body;
    struct HeroData *data;
    struct HeroSprite *sprite;
    struct HeroSpan *span;
    s32 home_x_fixed;
    s32 home_y_fixed;
    s32 home_h_fixed;
    s32 home_x;
    s32 home_y;
    s32 home_h;

    state_stack_advance_progress(body_b);
    if ((body_b->flags & HERO_FLAG_PROGRESS_DONE) != 0)
    {
        if (HERO_MODE_ACTIVE(hero_a->body.mode))
        {
            /* home position, 24.8 fixed point to whole units (rounding toward zero) */
            data = body_a->data;
            home_x_fixed = data->targetX;
            if (home_x_fixed < 0)
                home_x_fixed += 0xFF;
            home_x = home_x_fixed >> 8;
            home_y_fixed = data->targetY;
            if (home_y_fixed < 0)
                home_y_fixed += 0xFF;
            home_y = home_y_fixed >> 8;
            home_h_fixed = data->targetH;
            if (home_h_fixed < 0)
                home_h_fixed += 0xFF;
            home_h = home_h_fixed >> 8;
            place_targets(body_a, home_x, home_y, home_h, -1);
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
            body_a->state = NULL;
            enter_finish_rebuild(body_a);
        }
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            data = body_b->data;
            home_x_fixed = data->targetX;
            if (home_x_fixed < 0)
                home_x_fixed += 0xFF;
            home_x = home_x_fixed >> 8;
            home_y_fixed = data->targetY;
            if (home_y_fixed < 0)
                home_y_fixed += 0xFF;
            home_y = home_y_fixed >> 8;
            home_h_fixed = data->targetH;
            if (home_h_fixed < 0)
                home_h_fixed += 0xFF;
            home_h = home_h_fixed >> 8;
            place_targets(body_b, home_x, home_y, home_h, -1);
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
            body_b->state = NULL;
            enter_finish_rebuild(body_b);
        }
    }
}

/*
 * State: hero A shakes: for frames 0-3 of its timer the offset comes from gJitterTable1 (x, y
 * pairs), after that it is random inside the box gJitterRange1 (x0, y0, x1, y1). Spawns effect
 * 0x2FE6 just left of A, gives B action 4 and starts a 4-step motion for B, then goes on to
 * hero_a_place_near_partner_if_b_active.
 */
void hero_a_jitter_fx_b_step_start(struct HeroBody *body)
{
    struct Hero *hero_a = gWorld->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct Hero *hero_b = gWorld->heroB;
    struct HeroBody *body_b = &hero_b->body;
    s32 frame;
    s32 jitterX;
    s32 jitterY;

    /* timer bits 0-11: frames in this state */
    frame = body_a->timer & 0xFFF;
    if (frame <= 3)
    {
        hero_a->body.jitterX = gJitterTable1[frame * 2];
        hero_a->body.jitterY = gJitterTable1[(body_a->timer & 0xFFF) * 2 + 1];
    }
    else
    {
        s8 *box = gJitterRange1;

        if (box[0] < box[2])
            jitterX = box[0] + rand_below(box[2] - box[0], mt19937_next());
        else
            jitterX = box[2] + rand_below(box[0] - box[2], mt19937_next());
        hero_a->body.jitterX = jitterX;

        if (box[1] < box[3])
            jitterY = box[1] + rand_below(box[3] - box[1], mt19937_next());
        else
            jitterY = box[3] + rand_below(box[1] - box[3], mt19937_next());
        body_a->jitterY = jitterY;
    }
    effect_spawn_at(0x2FE6, body_a->posX / 0x100 - 4, body_a->posY / 0x100, body_a->posZ / 0x100, body_a);
    if (HERO_MODE_ACTIVE(body_b->mode))
    {
        sprite_set_action(body_b, 4, 0x2063, 0);
        ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
    }
    if (HERO_MODE_ACTIVE(body_b->mode))
    {
        /* progress runs to 0x4000 in 4 steps */
        body_b->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 4);
        body_b->progress = 0;
        /* clears HERO_FLAG_PROGRESS_DONE */
        ((struct HeroFlagBits *)&body_b->flags)->active = 0;
    }
    body_b->timer = body_b->timer & 0xFFF;
    body->state = hero_a_place_near_partner_if_b_active;
}

