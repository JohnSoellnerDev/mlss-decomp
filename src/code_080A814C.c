#include "global.h"
#include "ram_routines.h"

struct HeroSprite
{
    u8 filler0[0x06];
    s16 scale;
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

struct HeroFlagBits
{
    u8 unk0 : 5;
    u8 active : 1;
    u8 unk6 : 2;
};

struct HeroSpriteFlagBits
{
    u8 unk0 : 4;
    u8 bit4 : 1;
    u8 unk5 : 3;
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
    u8 filler0EE[0x114 - 0xEE];
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
    u8 filler30[0x38 - 0x30];
    s32 targetX;
    s32 targetY;
    s32 targetZ;
    u8 filler44[0x47 - 0x44];
    s8 ofsAlt;
    u8 filler48[0x4C - 0x48];
    void (*state)(struct HeroBody *);
    u32 unk50;
    u8 filler54[0x6C - 0x54];
    void *anim;
    u8 filler70[0x76 - 0x70];
    u8 mode;
    u8 filler77[0x79 - 0x77];
    u8 flags;
    u16 rate;
    u16 frame;
    s16 progress;
    u8 filler80[0x84 - 0x80];
    struct HeroSpan span;
    u8 filler88[0x92 - 0x88];
    u16 height;
    u32 arcSpeed; /* how fast the jump arc is crossed (set by set_motion_offsets) */
    u32 arcPeak;  /* the arc peaks at progress arcPeak * 0x4000 / arcSpeed */
    u32 unk9C;
    u8 filler0A0[0xAC - 0xA0];
    s16 step; /* progress before the last step_motion */
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
    u8 filler0B[0x70 - 0x0B];
    struct Hero *heroA;
    struct Hero *heroB;
};

extern struct World *gWorld;

/* Body mode bits 1-2: 2 or 4 means the body is active on the field. */
#define HERO_MODE_ACTIVE(mode) (((mode) & 6) == 2 || ((mode) & 6) == 4)
/* HeroBody.flags: set by state_stack_advance_progress once progress passes 0x3FFF */
#define HERO_MOTION_DONE 0x20

void state_stack_advance_progress(struct HeroBody *body);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void set_slot_value(u32 id, s32 value);
void step_motion(struct HeroBody *body);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void set_motion_start(struct HeroBody *body, u32 speed);
void set_motion_offsets(struct HeroBody *body, u32 offsets);
void set_motion_speed_aux(struct HeroBody *body, u32 speed, u32 aux);
void enter_finish_rebuild(struct HeroBody *body);
void start_action_4(struct HeroBody *body);
u32 find_frame_entry(struct HeroBody *obj, u32 index);

void hero_a_step_end_82ec(struct HeroBody *body);
void hero_b_ready_a_place_anim(struct HeroBody *body);
void hero_a_wait_reach_rate(struct HeroBody *body);
void hero_a_active_place_partner_z(struct HeroBody *body);
void hero_a_step_reach_input(struct HeroBody *body);
void hero_a_timing_window_9804(struct HeroBody *body);
void hero_b_move_to_partner_pose(struct HeroBody *body);

/* Grows hero B's sprite back to full size (0x10 a frame). Once it is full: if A's unk9C is 1, A goes
   on in hero_b_ready_a_place_anim; otherwise A takes jump frame 8, plays sound 0x2B and jumps in an
   arc onto this body's partner (the higher the partner's top, the higher the arc), and this body goes
   on in hero_a_step_end_82ec. */
void hero_b_sprite_grow_a_place(struct HeroBody *body)
{
    struct Hero *hero_a = gWorld->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct Hero *hero_b = gWorld->heroB;
    struct HeroBody *body_b = &hero_b->body;
    struct HeroBody *partner = body->partner;
    u16 scale;
    s32 arcHeight;
    s32 sq;
    s32 tx;
    s32 ty;
    s32 tz;
    s32 x;
    s32 y;
    s32 z;

    scale = body_b->sprite->scale;
    if (body_b->sprite->scale <= 255)
    {
        body_b->sprite->scale = scale + 0x10;
        if (body_b->sprite->scale <= 255)
            return;
        body_b->sprite->scale = 0x100;
    }
    body_b->sprite->scale = 0x100;
    if (body_a->unk9C == 1)
    {
        if (HERO_MODE_ACTIVE(body_a->mode))
            body_a->state = hero_b_ready_a_place_anim;
        return;
    }
    ((struct HeroSpriteFlagBits *)&body_a->sprite->flags)->bit4 = 0;
    body_a->sprite->frame = 8;
    ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
    set_slot_value(0x2B, -1);
    if (HERO_MODE_ACTIVE(body_a->mode))
    {
        // Land on the partner's target position (8.8 fixed point, rounded toward 0).
        tx = partner->targetX;
        if (tx < 0)
            tx += 0xFF;
        x = (tx >> 8) + partner->ofsX;
        ty = partner->targetY;
        if (ty < 0)
            ty += 0xFF;
        y = (ty >> 8) + partner->ofsY;
        z = partner->targetZ;
        if (z < 0)
            z += 0xFF;
        z = z >> 8;
        z += partner->ofsZ;
        place_targets(body_a, x, y, z, -1);
        // The partner's top: its own z offset if it is its object's main body, else its frame's.
        if (partner == (struct HeroBody *)((u8 *)partner->data + 8))
        {
            arcHeight = partner->ofsZ;
        }
        else
        {
            find_frame_entry(partner, 0);
            arcHeight = partner->ofsAlt;
        }
        tz = partner->targetZ;
        if (tz < 0)
            tz += 0xFF;
        arcHeight += tz >> 8;
        sq = arcHeight * arcHeight;
        arcHeight += MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0xE10, sq / 0x40 + 0x3C);
        body_a->height = arcHeight;
        set_motion_offsets(body_a, 0x566);
    }
    body_a->timer = body_a->timer & 0xFFF;
    body->state = hero_a_step_end_82ec;
}

ASM_FUNC("asm/code_080A814C/hero_a_step_end_82ec.s", void hero_a_step_end_82ec(struct HeroBody *body))

/* Once hero A's sprite sets flag bit 3: hero B (if active) walks back to its data's target position
   (speed 0x300), with its move animation (0x2002 for slot -1, else 0x204F), facing along its span,
   and goes on in enter_finish_rebuild. Sound 0x2B plays, and hero A (if active) jumps back to its
   data's target position in an arc 0x28 above its target height and starts action 4. */
void hero_pair_finish_b_place_a_action(void)
{
    struct Hero *hero_a = gWorld->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct Hero *hero_b = gWorld->heroB;
    struct HeroBody *body_b = &hero_b->body;
    struct HeroData *data;
    struct HeroSprite *sprite;
    struct HeroSpan *span;
    s32 tx;
    s32 ty;
    s32 th;
    s32 x;
    s32 y;
    s32 h;

    if ((body_a->sprite->flags & 8) != 0)
    {
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            // B's target position (8.8 fixed point, rounded toward 0).
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
            set_motion_start(body_b, 0x300);
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
            body_b->state = 0;
            enter_finish_rebuild(body_b);
        }
        set_slot_value(0x2B, -1);
        if (HERO_MODE_ACTIVE(body_a->mode))
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
            // The arc peaks 0x28 above the target height.
            th = body_a->targetZ;
            if (th < 0)
                th += 0xFF;
            body_a->height = (th >> 8) + 0x28;
            set_motion_offsets(body_a, 0x7CC);
        }
        if (HERO_MODE_ACTIVE(body_a->mode))
        {
            body_a->state = 0;
            start_action_4(body_a);
        }
    }
}

/* Hero B (if active) goes on in hero_b_move_to_partner_pose. Sound 0x2B plays, and hero A (if active)
   jumps from where it is to its own target X/Y plus its offsets, at height 0, in an arc 0x32 above
   its target height, with action 3 (0x2030); this body goes on in hero_a_wait_reach_rate. */
void hero_b_ready_a_place_anim(struct HeroBody *body)
{
    struct Hero *hero_a = gWorld->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct Hero *hero_b = gWorld->heroB;
    struct HeroBody *body_b = &hero_b->body;
    s32 tx;
    s32 ty;
    s32 tz;
    s32 x;
    s32 y;

    if (HERO_MODE_ACTIVE(hero_b->body.mode))
    {
        body_b->state = hero_b_move_to_partner_pose;
    }
    set_slot_value(0x2B, -1);
    if (HERO_MODE_ACTIVE(body_a->mode))
    {
        // A's own target position (8.8 fixed point, rounded toward 0) plus its offsets.
        tx = body_a->targetX;
        if (tx < 0)
            tx += 0xFF;
        x = (tx >> 8) + body_a->ofsX;
        ty = body_a->targetY;
        if (ty < 0)
            ty += 0xFF;
        y = (ty >> 8) + body_a->ofsY;
        place_targets(body_a, x, y, 0, -1);
        // The arc peaks 0x32 above the target height.
        tz = body_a->targetZ;
        if (tz < 0)
            tz += 0xFF;
        body_a->height = (tz >> 8) + 0x32;
        set_motion_offsets(body_a, 0x800);
    }
    if (HERO_MODE_ACTIVE(body_a->mode))
    {
        sprite_set_action(body_a, 3, 0x2030, 0);
        ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
    }
    body->state = hero_a_wait_reach_rate;
}

/* Step length within x and progress at least x. */
static inline u32 step_reached(struct HeroBody *a, s32 x)
{
    u32 r;
    s32 t;

    if (a->step <= x)
    {
        t = a->progress;
        r = 1;
        if (t >= x)
            goto done;
    }
    r = 0;
done:
    return r;
}

/* Hero A's jump: steps A along its arc and waits for the frame it passes the arc's peak. Then
   sound 0x62 plays, A (if active) starts a new progress over 12 frames, and this body goes on in
   hero_a_active_place_partner_z. */
void hero_a_wait_reach_rate(struct HeroBody *body)
{
    struct Hero *hero_a = gWorld->heroA;
    struct HeroBody *body_a = &hero_a->body;
    u16 rate;
    u32 arcSpeed;
    u32 arcPeak;
    register s32 peak asm("r2");
    u32 passedPeak;
    s32 progress;

    step_motion(body_a);
    arcSpeed = hero_a->body.arcSpeed;
    arcPeak = hero_a->body.arcPeak;
    peak = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(arcPeak << 14, arcSpeed);
    // The peak lies between the progress before this step and the progress after it.
    if (body_a->step <= peak)
    {
        progress = body_a->progress;
        passedPeak = 1;
        if (progress >= peak)
            goto done;
    }
    passedPeak = 0;
done:
    if (passedPeak != 0)
    {
        set_slot_value(0x62, -1); /* plays sound 0x62 */
        if (HERO_MODE_ACTIVE(body_a->mode))
        {
            rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 12);
            body_a->rate = rate;
            body_a->progress = 0;
            ((struct HeroFlagBits *)&body_a->flags)->active = 0;
        }
        body->state = hero_a_active_place_partner_z;
    }
}

/* Advances hero A's current motion. Once it is done: if A is active on the field, A jumps onto this
   body's partner (the partner's target X/Y plus its offsets, keeping A's own height, in pixels rounded
   toward 0) in an arc 0x28 high; then this body goes on in hero_a_step_reach_input. */
void hero_a_active_place_partner_z(struct HeroBody *body)
{
    struct Hero *hero_a = gWorld->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct HeroBody *partner = body->partner;
    s32 tx;
    s32 ty;
    s32 tz;
    s32 x;
    s32 y;
    s32 z;

    state_stack_advance_progress(body_a);
    if ((hero_a->body.flags & HERO_MOTION_DONE) != 0)
    {
        if (HERO_MODE_ACTIVE(hero_a->body.mode))
        {
            tz = body_a->posZ;
            if (tz < 0)
                tz += 0xFF;
            z = tz >> 8;
            tx = partner->targetX;
            if (tx < 0)
                tx += 0xFF;
            x = (tx >> 8) + partner->ofsX;
            ty = partner->targetY;
            if (ty < 0)
                ty += 0xFF;
            y = (ty >> 8) + partner->ofsY;
            place_targets(body_a, x, y, z, -1);
            {
                u16 arcHeight = 0x28;
                body_a->height = arcHeight;
            }
            set_motion_offsets(body_a, 0x533);
        }
        body_a->timer = body_a->timer & 0xFFF;
        body->state = hero_a_step_reach_input;
    }
}

ASM_FUNC("asm/code_080A814C/hero_a_step_reach_input.s", void hero_a_step_reach_input(struct HeroBody *body))

/* Advances hero A's current motion. Once it is done: if A is active on the field, A starts a
 * move onto this body's partner (its target plus its x/y/z offsets, in pixels rounded toward 0)
 * with aux speed 0xC0; then this body goes on in hero_a_timing_window_9804. */
void hero_a_active_place_partner_aux(struct HeroBody *body)
{
    struct Hero *hero_a = gWorld->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct HeroBody *partner = body->partner;
    s32 tx;
    s32 ty;
    s32 tz;
    s32 x;
    s32 y;
    s32 z;

    state_stack_advance_progress(body_a);
    if ((hero_a->body.flags & HERO_MOTION_DONE) != 0)
    {
        if (HERO_MODE_ACTIVE(hero_a->body.mode))
        {
            tx = partner->targetX;
            if (tx < 0)
                tx += 0xFF;
            x = (tx >> 8) + partner->ofsX;
            ty = partner->targetY;
            if (ty < 0)
                ty += 0xFF;
            y = (ty >> 8) + partner->ofsY;
            tz = partner->targetZ;
            if (tz < 0)
                tz += 0xFF;
            z = (tz >> 8) + partner->ofsZ;
            place_targets(body_a, x, y, z, -1);
            set_motion_speed_aux(body_a, 0, 0xC0);
        }
        body_a->timer = body_a->timer & 0xFFF;
        body->state = hero_a_timing_window_9804;
    }
}

ASM_FUNC("asm/code_080A814C/hero_a_timing_window_9804.s", void hero_a_timing_window_9804(struct HeroBody *body))
