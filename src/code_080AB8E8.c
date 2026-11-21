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

struct HeroSpriteFlag4Bits
{
    u8 unk0 : 4;
    u8 bit4 : 1;
    u8 unk5 : 3;
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
    u8 filler30[0x38 - 0x30];
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
    u16 rate;
    u16 frame;
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

struct World
{
    u8 filler0[0x0A];
    u8 modeByte;
    u8 filler0B[0x0E - 0x0B];
    u8 enableByte;
    u8 filler0F[0x70 - 0x0F];
    struct Hero *heroA;
    struct Hero *heroB;
    u8 filler78[0x24C - 0x78];
    void *trackedObj;
};

/* Body mode bits 1-2: 2 or 4 means the body is active on the field. */
#define HERO_MODE_ACTIVE(mode) (((mode) & 6) == 2 || ((mode) & 6) == 4)

/* HeroSprite.flags bit: the current animation has finished. */
#define SPRITE_FLAG_ANIM_DONE 0x08

/* HeroBody.flags bit: the body's motion/progress step has finished. */
#define HERO_FLAG_PROGRESS_DONE 0x20

extern struct World *gWorld;
extern u8 *gParty;

void state_stack_advance_progress(struct HeroBody *body);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void set_slot_value(u32 id, s32 value);
void step_motion(struct HeroBody *body);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void set_motion_start(struct HeroBody *body, u32 speed);
void enter_finish_rebuild(struct HeroBody *body);
void schedule_pose_task(struct HeroBody *obj);
void set_motion_speed_aux(struct HeroBody *body, u32 speed, u32 aux);
void set_motion_offsets(struct HeroBody *body, u32 offsets);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 h, struct HeroBody *owner);
u32 find_frame_entry(struct HeroBody *obj, u32 index);
s32 resolve_contact_action(struct HeroBody *body, struct HeroBody *partner, u32 a, u32 b, u32 cb);
void obj_slot_update(struct HeroBody *obj);

void hero_a_active_copy_pos_fx(struct HeroBody *body);
void hero_a_step_16_b_idle_alt(struct HeroBody *body);
void hero_b_active_place_at_a_pose(struct HeroBody *body);

void hero_a_link_to_partner_fx(struct HeroBody *body);
void hero_a_active_set_targets(struct HeroBody *body);
void hero_a_active_fx_mirror(struct HeroBody *body);
void hero_pair_place_finish_sprite(struct HeroBody *body);
void hero_a_on_partner(struct HeroBody *body);
void hero_a_active_step_12(struct HeroBody *body);
void hero_a_active_follow_b_aux(struct HeroBody *body);
void hero_a_step_threshold_c16c(struct HeroBody *body);
void hero_a_sprite_set_targets(struct HeroBody *body);
void hero_a_step_stage_fx(struct HeroBody *body);
void active_targets_hero_pair_place_finish_b(struct HeroBody *body);

void hero_b_pose_anim(struct HeroBody *body);
void noop_state_080acec0(struct HeroBody *body);
void hero_b_on_object(struct HeroBody *body);

/* Steps hero A's motion while hero B's sprite grows back to full size (0x10 a frame); once A's motion is
   done, sets both heroes' poses and resolves A's contact with this body's partner. If that hands A over
   (result 1) A's slot is updated and this state ends; otherwise an effect plays on the partner when it
   needs one, A gets its next rate and the sequence goes on in hero_a_active_set_targets. */
void hero_a_link_to_partner_fx(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct HeroBody *body_a = &world->heroA->body;
    struct HeroBody *body_b = &world->heroB->body;
    struct HeroBody *partner = body->partner;
    s32 scale;
    s32 result;

    step_motion(body_a);
    scale = body_b->sprite->scale;
    if ((s16)body_b->sprite->scale <= 0xFF)
    {
        body_b->sprite->scale = scale + 0x10;
        if ((s16)body_b->sprite->scale > 0x100)
            body_b->sprite->scale = 0x100;
    }
    if ((body_a->flags & HERO_FLAG_PROGRESS_DONE) != 0)
    {
        set_slot_value(0x63, -1);
        if (HERO_MODE_ACTIVE(body_a->mode))
        {
            sprite_set_action(body_a, 0xC, 0x2030, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            sprite_set_action(body_b, 0, 0x204D, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 0;
        }
        ((struct HeroCcBits *)&body_a->cc)->low = 0;
        body_a->scale = 0x100;
        result = resolve_contact_action(body_a, partner, 0, 0, 0x040A1688);
        body_a->count++;
        if (result == 1)
        {
            obj_slot_update(body_a);
            return;
        }
        if (partner->data->flagsF6 > 0 || (partner->data->flags111 & 2) != 0)
        {
            s32 x;
            s32 y;
            s32 z;
            u32 effectId;

            x = (partner->targetX / 0x100) + partner->ofsX;
            y = (partner->targetY / 0x100) + partner->ofsY;
            z = (partner->targetZ / 0x100) + partner->ofsZ;
            effectId = 0x70B;
            effect_spawn_at(effectId, x, y, z, partner);
        }
        if (HERO_MODE_ACTIVE(body_a->mode))
        {
            body_a->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 8);
            body_a->progress = 0;
            ((struct HeroFlagBits *)&body_a->flags)->active = 0;
        }
        body->state = hero_a_active_set_targets;
    }
}

/* Steps hero A's progress; once it is done, plays sound slot 0x30 and, while A is active on the field,
   sends A to 0x3C pixels left of its own target point, 0x28 above its target height, with motion
   offsets 0x500, and puts it in pose 0xB (animation 0x2000). The sequence goes on in
   hero_a_active_fx_mirror. */
void hero_a_active_set_targets(struct HeroBody *body)
{
    struct Hero *hero_a = gWorld->heroA;
    struct HeroBody *body_a = &hero_a->body;

    state_stack_advance_progress(body_a);
    if ((hero_a->body.flags & HERO_FLAG_PROGRESS_DONE) != 0)
    {
        set_slot_value(0x30, -1);
        if (HERO_MODE_ACTIVE(hero_a->body.mode))
        {
            place_targets(body_a,
                          (body_a->targetX / 0x100) + body_a->ofsX - 0x3C,
                          (body_a->targetY / 0x100) + body_a->ofsY, 0, -1);
            body_a->height = (body_a->targetZ / 0x100) + 0x28;
            set_motion_offsets(body_a, 0x500);
        }
        if (HERO_MODE_ACTIVE(body_a->mode))
        {
            sprite_set_action(body_a, 0xB, 0x2000, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        body->state = hero_a_active_fx_mirror;
    }
}

/* Steps hero A's motion; once it is done, plays sound slot 0x2E and effect 0x1E at A, puts A (while it
   is active on the field) in pose 8 (animation 0x2034), mirrors A's sprite and goes on in
   hero_pair_place_finish_sprite. */
void hero_a_active_fx_mirror(struct HeroBody *body)
{
    struct Hero *hero_a = gWorld->heroA;
    struct HeroBody *body_a = &hero_a->body;

    step_motion(body_a);
    if ((hero_a->body.flags & HERO_FLAG_PROGRESS_DONE) != 0)
    {
        set_slot_value(0x2E, -1);
        effect_spawn_at(0x1E, body_a->posX / 0x100, body_a->posY / 0x100, body_a->posZ / 0x100, body_a);
        if (HERO_MODE_ACTIVE(hero_a->body.mode))
        {
            sprite_set_action(body_a, 8, 0x2034, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
        ((struct HeroMirrorBits *)&body_a->sprite->mirror)->flip = 1;
        body->state = hero_pair_place_finish_sprite;
    }
}

/* Waits for hero A's animation to end, then sends hero B and then hero A back
   to their home targets, puts them in their idle pose facing along their span
   and hands each one over to the finish-rebuild state. */
void hero_pair_place_finish_sprite(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct Hero *hero_a = world->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct Hero *hero_b = world->heroB;
    struct HeroBody *body_b = &hero_b->body;
    struct HeroData *data;
    struct HeroSprite *sprite;
    struct HeroSpan *span;
    s32 flip;

    if ((body_a->sprite->flags & SPRITE_FLAG_ANIM_DONE) != 0)
    {
        set_slot_value(0x53, -1);
        if (HERO_MODE_ACTIVE(hero_b->body.mode))
        {
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

/* Starts hero A's jump onto this body's partner: plays sound 0x62 and effect 0x2F9E at A, sends A
   in an arc to just above the partner's top (the higher the target, the higher the arc), and puts A
   in its jump pose and B in its watching pose. Goes on in hero_a_active_step_12. */
void hero_a_on_partner(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct Hero *hero_a = world->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct HeroBody *body_b = &world->heroB->body;
    struct HeroBody *partner = body->partner;
    s32 arcHeight;

    set_slot_value(0x62, -1);
    effect_spawn_at(0x2F9E, body_a->posX / 0x100, body_a->posY / 0x100, body_a->posZ / 0x100, body_a);
    if (HERO_MODE_ACTIVE(hero_a->body.mode))
    {
        place_targets(body_a,
                      (partner->targetX / 0x100) + partner->ofsX,
                      (partner->targetY / 0x100) + partner->ofsY,
                      (partner->targetZ / 0x100) + partner->ofsZ + 0x3C, -1);
        // The partner's top: its own z offset if it is its object's main body, else its frame's.
        if (partner == (struct HeroBody *)((u8 *)partner->data + 8))
        {
            arcHeight = partner->ofsZ;
        }
        else
        {
            find_frame_entry(partner, 0);
            arcHeight = partner->unk47;
        }
        arcHeight += partner->targetZ / 0x100;
        arcHeight += MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x1000, (arcHeight * arcHeight) / 0x40 + 0x40);
        body_a->height = arcHeight;
        set_motion_offsets(body_a, 0x600);
    }
    if (HERO_MODE_ACTIVE(body_a->mode))
    {
        sprite_set_action(body_a, 9, 0x2030, 0);
        ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
    }
    if (HERO_MODE_ACTIVE(body_b->mode))
    {
        sprite_set_action(body_b, 7, 0x205F, 0);
        ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
    }
    body->state = hero_a_active_step_12;
}

/* Steps hero A's jump arc onto the partner (started by hero_a_on_partner) while hero B's sprite grows back to
   full size (0x10 a frame). Once A lands, puts it in its landing pose, starts its next progress at rate
   0x4000 / 12 and goes on in hero_a_active_follow_b_aux. */
void hero_a_active_step_12(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct HeroBody *body_a = &world->heroA->body;
    struct HeroBody *body_b = &world->heroB->body;
    s32 scale;

    step_motion(body_a);
    scale = body_b->sprite->scale;
    if ((s16)body_b->sprite->scale <= 0xFF)
    {
        body_b->sprite->scale = scale + 0x10;
        if ((s16)body_b->sprite->scale > 0x100)
            body_b->sprite->scale = 0x100;
    }
    if ((body_a->flags & HERO_FLAG_PROGRESS_DONE) != 0)
    {
        if (HERO_MODE_ACTIVE(body_a->mode))
        {
            sprite_set_action(body_a, 3, 0x2030, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        if (HERO_MODE_ACTIVE(body_a->mode))
        {
            body_a->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0xC);
            body_a->progress = 0;
            ((struct HeroFlagBits *)&body_a->flags)->active = 0;
        }
        body->state = hero_a_active_follow_b_aux;
    }
}

/* Once hero A's progress is done (12 frames, started by hero_a_active_step_12), A (if active) moves
   from rest, speeding up by 0xCC a frame, to 4 px below this body's partner's target position plus
   its offsets, and this body goes on in hero_a_step_threshold_c16c. */
void hero_a_active_follow_b_aux(struct HeroBody *body)
{
    struct Hero *hero_a = gWorld->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct HeroBody *partner = body->partner;

    state_stack_advance_progress(body_a);
    if ((hero_a->body.flags & HERO_FLAG_PROGRESS_DONE) != 0)
    {
        if (HERO_MODE_ACTIVE(hero_a->body.mode))
        {
            place_targets(body_a,
                          (partner->targetX / 0x100) + partner->ofsX,
                          (partner->targetY / 0x100) + partner->ofsY,
                          (partner->targetZ / 0x100) + partner->ofsZ - 4, -1);
            set_motion_speed_aux(body_a, 0, 0xCC); /* start speed 0, acceleration 0xCC */
        }
        body_a->timer &= 0xFFF;
        body->state = hero_a_step_threshold_c16c;
    }
}
