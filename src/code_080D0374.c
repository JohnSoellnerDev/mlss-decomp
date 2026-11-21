#include "global.h"
#include "ram_routines.h"

struct HeroSprite
{
    u8 filler0[0x0E];
    u16 unkE;
    u8 filler10[0x11 - 0x10];
    u8 mirror;
    u8 flags;
    u8 filler13[0x20 - 0x13];
    u8 frame;
    u8 anim;
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

struct HeroStepBits
{
    u8 low : 5;
    u8 step : 3;
};

struct HeroChildFlagBits
{
    u8 bit0 : 1;
    u8 unk1 : 1;
    u8 bit2 : 1;
    u8 unk3 : 5;
};

struct HeroCcBits
{
    u8 low : 7;
    u8 high : 1;
};

struct HeroEffectDesc
{
    u8 filler0[0x1C];
    u8 unk1C;
};

struct HeroDataFlagBits
{
    u8 unk0 : 1;
    u8 partnerLinked : 1;
    u8 unk2 : 6;
};

struct HeroData
{
    u8 filler0[0xD8];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler0E4[0xEC - 0xE4];
    s16 poseIndex;
    u8 filler0EE[0xF0 - 0xEE];
    struct HeroEffectDesc *effect;
    u8 filler0F4[0xF6 - 0xF4];
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
    u8 filler44[0x47 - 0x44];
    s8 unk47;
    u8 filler48[0x4C - 0x48];
    void (*state)(struct HeroBody *);
    u32 unk50;
    u8 filler54[0x6C - 0x54];
    void *anim;
    u8 filler70[0x75 - 0x70];
    u8 unk75;
    u8 mode;
    u8 filler77[0x78 - 0x77];
    u8 unk78;
    u8 flags;
    u16 rate;
    u16 frame;
    s16 progress;
    u8 filler80[0x84 - 0x80];
    s16 spanA;
    s16 spanB;
    u32 spanY;
    u8 filler8C[0x92 - 0x8C];
    u16 height;
    u8 filler94[0x9C - 0x94];
    u32 unk9C;
    u32 unkA0;
    u8 filler0A4[0xA8 - 0xA4];
    u32 unkA8;
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
    u8 unk0E;
    u8 filler0F[0x70 - 0x0F];
    struct Hero *heroA;
    struct Hero *heroB;
    u8 filler78[0x24C - 0x78];
    void *trackedObj;
};

extern struct World *gWorld;
extern u8 *gParty;

#define HERO_MODE_ACTIVE(mode) (((mode) & 6) == 2 || ((mode) & 6) == 4)

/* HeroSprite.flags: the current animation has finished. */
#define HERO_SPRITE_ANIM_DONE 0x08

/* HeroBody.flags: step_motion has brought the body to its motion target. */
#define HERO_FLAG_MOTION_DONE 0x20

void step_motion(struct HeroBody *body);
void step_motion_scaled(struct HeroBody *body);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void set_slot_value(u32 id, s32 value);
void body_advance_end_if_active(struct HeroBody *body);
void hero_a_spawn_child_counted(struct HeroBody *body);
void hero_a_child_wait_active(struct HeroBody *body);
void hero_start_step_eighth(struct HeroBody *body);
void body_anim_next_then_end(struct HeroBody *body);
void hero_pair_clear_anim_mirror(struct HeroBody *body);
void set_motion_offsets(struct HeroBody *body, u32 offsets);
void obj_release(void *obj);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void set_motion_speed_aux(struct HeroBody *body, u32 speed, u32 aux);
void resolve_contact_action(struct HeroBody *a, struct HeroBody *b, u32 c, u32 d, u32 e);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 h, struct HeroBody *owner);
void hero_a_child_remove(struct HeroBody *body);
void state_stack_advance_progress(struct HeroBody *body);
struct HeroBody *obj_spawn_linked(struct HeroBody *parent, u32 kind, u32 value);
void hero_a_place_at_b_target(struct HeroBody *body);
void hero_a_step_press_pose_d0878(struct HeroBody *body);
void enter_finish_rebuild(struct HeroBody *body);
void set_motion_start(struct HeroBody *body, u32 speed);
u32 find_frame_entry(struct HeroBody *obj, u32 index);
void body_step_window_d16c0(struct HeroBody *body);

/*
 * State of an object hero A threw: it glides towards hero A in x/y (by 26/256 every frame) and
 * sinks towards z 0x5000. Once it is down to 0x80 above its target z it plays sound 0x120 and
 * lands on the oldest object in A's child list (the last one): a later object is released and
 * counts as one more on the oldest (unk9C, which the oldest itself resets to 0). Every odd count
 * while the oldest's animation is at most 0xA, the pile grows: the oldest plays its next
 * animation (step bits = count / 3) and goes to body_anim_next_then_end; otherwise an active
 * oldest object goes to hero_start_step_eighth.
 */
void hero_body_blend_to_a(struct HeroBody *body)
{
    struct HeroBody *node = &gWorld->heroA->body;
    u32 count;
    void (*next)(struct HeroBody *);

    body->posX = (body->posX * 0xE6 + node->posX * 0x1A) / 256;
    body->posY = (body->posY * 0xE6 + node->posY * 0x1A) / 256;
    body->posZ = (body->posZ * 0xE6 + 0x82000) / 256; /* 0x82000 = 0x1A * 0x5000 */
    if (body->targetZ + 0x80 < body->posZ)
        return;
    set_slot_value(0x120, -1); /* plays sound 0x120 */
    body->posZ -= 0x300;
    /* the oldest child: the end of hero A's child list */
    for (;;)
    {
        node = node->child;
        if (node->child == 0)
            break;
    }
    if (body != node)
    {
        obj_release(body);
        body = node;
    }
    else
    {
        body->unk9C = 0;
    }
    count = body->unk9C + 1;
    body->unk9C = count;
    if ((((u8)count ^ 1) & 1) != 0 || body->sprite->anim > 0xA)
    {
        if (HERO_MODE_ACTIVE(body->mode))
            next = hero_start_step_eighth;
        else
            return;
    }
    else
    {
        ((struct HeroStepBits *)&body->unk78)->step = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(count, 3);
        sprite_set_action(body, body->sprite->anim + 1, 0, 0);
        ((struct HeroSpriteBits *)&body->sprite->flags)->mode = 1;
        next = body_anim_next_then_end;
    }
    body->state = next;
}

void hero_start_step_eighth(struct HeroBody *body)
{
    if ((body->mode & 6) == 2 || (body->mode & 6) == 4)
    {
        body->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 8);
        body->progress = 0;
        ((struct HeroFlagBits *)&body->flags)->active = 0;
    }
    body->state = body_advance_end_if_active;
}

/*
 * Sets bits 6-7 of gWorld->modeByte. If hero A is shown (HERO_MODE_ACTIVE), he hops 0x14 high
 * (set_motion_offsets, speed 0x800) from where he stands to that spot plus his ofsX/ofsY, then
 * plays action 5 (arg 0x2000 without a pose, 0x204D with one) with sprite mode 1. His sprite is
 * mirrored either way; next state: hero_pair_clear_anim_mirror.
 */
void hero_a_place_targets_anim5(struct HeroBody *body)
{
    struct HeroBody *heroA;
    u8 mode;

    mode = gWorld->modeByte;
    gWorld->modeByte = mode | 0xC0;
    heroA = &gWorld->heroA->body;
    if (HERO_MODE_ACTIVE(heroA->mode))
    {
        s32 x = (heroA->targetX / 256) + heroA->ofsX;
        s32 y = (heroA->targetY / 256) + heroA->ofsY;
        place_targets(heroA, x, y, 0, -1);
        {
            u16 arcHeight = 0x14;
            heroA->height = arcHeight;
        }
        set_motion_offsets(heroA, 0x800);
    }
    if (HERO_MODE_ACTIVE(heroA->mode))
    {
        if (heroA->data->poseIndex == -1)
        {
            sprite_set_action(heroA, 5, 0x2000, 0);
        }
        else
        {
            sprite_set_action(heroA, 5, 0x204D, 0);
        }
        ((struct HeroSpriteBits *)&heroA->sprite->flags)->mode = 1;
    }
    ((struct HeroMirrorBits *)&heroA->sprite->mirror)->flip = 1;
    body->state = hero_pair_clear_anim_mirror;
}

/* State: steps hero A's hop from hero_a_place_targets_anim5. When it lands, plays sound 0x65, puts
   both heroes that are shown back to action 0 unmirrored with sprite mode 0, then mirrors hero A
   again and continues in hero_a_spawn_child_counted. */
void hero_pair_clear_anim_mirror(struct HeroBody *body)
{
    struct Hero *heroA = gWorld->heroA;
    struct HeroBody *a = &heroA->body;
    struct Hero *heroB = gWorld->heroB;
    struct HeroBody *b = &heroB->body;

    step_motion(a);
    if ((a->flags & HERO_FLAG_MOTION_DONE) != 0)
    {
        set_slot_value(0x65, -1);
        if (HERO_MODE_ACTIVE(b->mode))
        {
            sprite_set_action(b, 0, -1, 0);
            ((struct HeroMirrorBits *)&b->sprite->mirror)->flip = 0;
            ((struct HeroSpriteBits *)&b->sprite->flags)->mode = 0;
        }
        if (HERO_MODE_ACTIVE(a->mode))
        {
            sprite_set_action(a, 0, -1, 0);
            ((struct HeroMirrorBits *)&a->sprite->mirror)->flip = 0;
            ((struct HeroSpriteBits *)&a->sprite->flags)->mode = 0;
        }
        ((struct HeroMirrorBits *)&a->sprite->mirror)->flip = 1;
        body->state = hero_a_spawn_child_counted;
    }
}

/*
 * Hero A's scripted sequence, one spawn per call while A's counter unkA0 lasts: each call spawns
 * object 0x208A as A's newest child (obj_spawn_linked puts it at the head of A's child list), 14
 * pixels ahead of A and 16 up, plays A's and B's press poses and sends the new object in a 0x64-high
 * jump to 6 pixels right of B's target; then waits in hero_a_step_press_pose_d0878. Once the counter
 * is used up and A's child has stopped, A goes to hero_a_place_at_b_target.
 * ((mode & 6) == 2 or 4: a body that is shown and can be animated.)
 */
void hero_a_spawn_child_counted(struct HeroBody *body)
{
    struct Hero *hero_a = gWorld->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct HeroBody *body_b = &gWorld->heroB->body;
    struct HeroBody *spawned;

    if ((s32)body_a->unkA0 <= 0)
    {
        if (body_a->child->state == 0)
        {
            if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
            {
                body_a->state = hero_a_place_at_b_target;
            }
        }
    }
    else
    {
        set_slot_value(0x9C, -1);
        body_a->unkA0 = body_a->unkA0 - 1;
        spawned = obj_spawn_linked(body_a, 0x208A, 0);
        spawned->posX += 0xE00;
        spawned->posZ += 0x1000;
        ((struct HeroChildFlagBits *)&spawned->flags)->bit0 = 0;
        ((struct HeroChildFlagBits *)&spawned->flags)->bit2 = 1;
        spawned->sprite->unkE = body_a->sprite->unkE - 3;
        if ((spawned->mode & 6) == 2 || (spawned->mode & 6) == 4)
        {
            sprite_set_action(spawned, 1, 0x2092, 0);
            ((struct HeroSpriteBits *)&spawned->sprite->flags)->mode = 0;
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            sprite_set_action(body_a, 2, 0x2036, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
        ((struct HeroMirrorBits *)&body->sprite->mirror)->flip = 0;
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            sprite_set_action(body_b, 0, 0x2054, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
        }
        {
            struct HeroBody *newest = body_a->child;

            /* it jumps to 6 pixels right of B's target, 12 up */
            if ((newest->mode & 6) == 2 || (newest->mode & 6) == 4)
            {
                s32 x = (body_b->targetX / 256) + body_b->ofsX + 6;
                s32 y = (body_b->targetY / 256) + body_b->ofsY;
                place_targets(newest, x, y, 0xC, -1);
                {
                    struct HeroBody *jumper = body_a->child;
                    u16 height = 0x64;
                    jumper->height = height;
                }
                set_motion_offsets(body_a->child, 0x400);
            }
        }
        body_a->child->timer = body_a->child->timer & 0xFFF;
        body->state = hero_a_step_press_pose_d0878;
    }
}

ASM_FUNC("asm/code_080D0374/hero_a_step_press_pose_d0878.s", void hero_a_step_press_pose_d0878(struct HeroBody *body))

/* Once hero B's animation is done: plays sound 0x11F, B (if shown) plays pose 0x2054's follow-up
 * 0x204D, A's child is released and, if shown, sent to A's target (plus A's offsets, 8 up) with
 * set_motion_speed_aux(0, 0x66). Then waits in hero_a_child_wait_active. */
void hero_a_child_finish_place(struct HeroBody *body)
{
    struct Hero *hero_a = gWorld->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct Hero *hero_b = gWorld->heroB;
    struct HeroBody *body_b = &hero_b->body;

    if ((body_b->sprite->flags & HERO_SPRITE_ANIM_DONE) != 0)
    {
        set_slot_value(0x11F, -1);
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            sprite_set_action(body_b, 0, 0x204D, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 0;
        }
        obj_release(body_a->child);
        if (HERO_MODE_ACTIVE(body_a->child->mode))
        {
            s32 x = (body_a->targetX / 256) + body_a->ofsX;
            s32 y = (body_a->targetY / 256) + body_a->ofsY;
            s32 z = (body_a->targetZ / 256) + 8;
            place_targets(body_a->child, x, y, z, -1);
            set_motion_speed_aux(body_a->child, 0, 0x66);
        }
        body->state = hero_a_child_wait_active;
    }
}

void hero_pair_place_both_finish_a_idle(struct HeroBody *body);
void hero_child_place_on_partner(struct HeroBody *body);

void hero_a_child_wait_active(struct HeroBody *body)
{
    struct Hero *hero_a = gWorld->heroA;
    struct HeroBody *body_a = &hero_a->body;

    step_motion_scaled(body_a->child);
    if ((body_a->child->flags & 0x20) != 0)
    {
        set_slot_value(0x9C, -1);
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            sprite_set_action(body_a, 5, 0x2036, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
        if ((body_a->child->mode & 6) == 2 || (body_a->child->mode & 6) == 4)
        {
            body_a->child->state = hero_child_place_on_partner;
        }
        body->state = hero_pair_place_both_finish_a_idle;
    }
}

/* Once hero A's sprite animation has ended and it carries no child: plays sound slot 0x53, and each
   hero in turn (B, then A) walks back to its home spot (its data's target, in 1/256 pixels), takes
   its idle action facing along its span, and clears its state to enter the finish rebuild. */
void hero_pair_place_both_finish_a_idle(struct HeroBody *body)
{
    struct Hero *hero_b;
    struct HeroBody *body_a;
    struct HeroBody *body_b;
    struct HeroData *data;
    s16 *span;

    body_a = &gWorld->heroA->body;
    hero_b = gWorld->heroB;
    body_b = &hero_b->body;
    if ((body_a->sprite->flags & 8) == 0)
        return;
    if (body_a->child != NULL)
        return;
    set_slot_value(0x53, -1);
    if ((hero_b->body.mode & 6) == 2 || (hero_b->body.mode & 6) == 4)
    {
        data = body_b->data;
        place_targets(body_b, data->targetX / 0x100, data->targetY / 0x100, data->targetH / 0x100, -1);
        set_motion_start(body_b, 0x400);
    }
    if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
    {
        if (body_b->data->poseIndex == -1)
            sprite_set_action(body_b, 0, 0x2002, 0);
        else
            sprite_set_action(body_b, 0, 0x204F, 0);
        ((struct HeroMirrorBits *)&body_b->sprite->mirror)->flip = (span = &body_b->spanA)[0] >= span[1];
        ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 0;
    }
    if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
    {
        body_b->state = NULL;
        enter_finish_rebuild(body_b);
    }
    if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
    {
        data = body_a->data;
        place_targets(body_a, data->targetX / 0x100, data->targetY / 0x100, data->targetH / 0x100, -1);
        set_motion_start(body_a, 0x400);
    }
    if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
    {
        if (body_a->data->poseIndex == -1)
            sprite_set_action(body_a, 0, 0x2002, 0);
        else
            sprite_set_action(body_a, 0, 0x204F, 0);
        ((struct HeroMirrorBits *)&body_a->sprite->mirror)->flip = (span = &body_a->spanA)[0] >= span[1];
        ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
    }
    if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
    {
        body_a->state = NULL;
        enter_finish_rebuild(body_a);
    }
}

void hero_a_child_link_partner_fx(struct HeroBody *body);

/* Child state set by hero_a_child_wait_active: if hero A's child is in play, it jumps (height 0x20,
 * speed 0x800) onto its partner's target plus the partner's offsets, 8 px lower. Then it goes on to
 * hero_a_child_link_partner_fx. */
void hero_child_place_on_partner(struct HeroBody *body)
{
    struct Hero *hero_a = gWorld->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct HeroBody *partner = body->partner;
    struct HeroBody *child = body_a->child;

    if (HERO_MODE_ACTIVE(child->mode))
    {
        s32 x = (partner->targetX / 256) + partner->ofsX;
        s32 y = (partner->targetY / 256) + partner->ofsY;
        s32 z = (partner->targetZ / 256) + partner->ofsZ - 8;
        place_targets(child, x, y, z, -1);
        {
            struct HeroBody *jumper = body_a->child;
            u16 height = 0x20;
            jumper->height = height;
        }
        set_motion_offsets(body_a->child, 0x800);
    }
    body->state = hero_a_child_link_partner_fx;
}

/* Body state: steps the motion of hero A's child. Once that motion ends (flag 0x20): clears the
 * partner's partnerLinked bit, plays sound 0x55, sets the child's cc to 0x46 and scale to 0x100,
 * resolves the child's contact with the partner (script 0x0413438C) and counts it, spawns effect
 * 0x7D at the partner's spot when the partner shows its timer or has flags111 bit 1, and goes on
 * to hero_a_child_remove. */
void hero_a_child_link_partner_fx(struct HeroBody *body)
{
    struct Hero *hero_a = gWorld->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct HeroBody *partner = body->partner;

    step_motion(body_a->child);
    if ((body_a->child->flags & 0x20) != 0)
    {
        ((struct HeroDataFlagBits *)&partner->data->flags111)->partnerLinked = 0;
        set_slot_value(0x55, -1);
        ((struct HeroCcBits *)&body_a->child->cc)->low = 0x46;
        body_a->child->scale = 0x100;
        resolve_contact_action(body_a->child, partner, 0, 0, 0x0413438C);
        body_a->child->count = body_a->child->count + 1;
        if (partner->data->displayTimer > 0 || (partner->data->flags111 & 2) != 0)
        {
            /* positions are 24.8 fixed point */
            s32 x = (partner->targetX / 256) + partner->ofsX;
            s32 y = (partner->targetY / 256) + partner->ofsY;

            effect_spawn_at(0x7D, x, y, 0, partner);
        }
        body->state = hero_a_child_remove;
    }
}

void body_place_at_partner_target(struct HeroBody *body);

/* Plays sound 0xAF; hero B (if active) takes action 2 (0x2054) in sprite mode 1 and starts a new
 * progress over 8 frames (rate 0x4000 / 8), then the state goes on with
 * body_place_at_partner_target. */
void hero_b_anim_and_step_rate8_alt(struct HeroBody *body)
{
    struct Hero *hero_b = gWorld->heroB;
    struct HeroBody *body_b = &hero_b->body;

    set_slot_value(0xAF, -1);
    if (HERO_MODE_ACTIVE(body_b->mode))
    {
        sprite_set_action(body_b, 2, 0x2054, 0);
        ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
    }
    if (HERO_MODE_ACTIVE(body_b->mode))
    {
        body_b->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 8);
        body_b->progress = 0;
        ((struct HeroFlagBits *)&body_b->flags)->active = 0;
    }
    body->state = body_place_at_partner_target;
}

/* Body state: advances hero B's state stack and steps this body's motion. Once hero B's motion sets
 * flag 0x20: sets slot 0x7D and (when active) sends the body in an arc onto its partner's target
 * position (10 below it), the arc as high as the partner's top plus a bit more for low tops, at
 * speed 0x800. Then flips the sprite, keeps only the low 12 bits of both timers and goes on with
 * body_step_window_d16c0. */
void body_place_at_partner_target(struct HeroBody *body)
{
    struct HeroBody *body_a = &gWorld->heroA->body;
    struct HeroBody *body_b = &gWorld->heroB->body;
    struct HeroBody *partner = body->partner;

    state_stack_advance_progress(body_b);
    step_motion(body);
    if ((body_b->flags & 0x20) != 0)
    {
        set_slot_value(0x7D, -1);
        if (HERO_MODE_ACTIVE(body->mode))
        {
            /* positions are 24.8 fixed point */
            s32 x = (partner->targetX / 256) + partner->ofsX;
            s32 y = (partner->targetY / 256) + partner->ofsY;
            s32 z = (partner->targetZ / 256) + partner->ofsZ - 10;
            s32 arcHeight;
            s32 sq;

            place_targets(body, x, y, z, -1);
            /* the partner's top: its own z offset if it is its object's main body, else its frame's */
            if (partner == (struct HeroBody *)((u8 *)partner->data + 8))
            {
                arcHeight = partner->ofsZ;
            }
            else
            {
                find_frame_entry(partner, 0);
                arcHeight = partner->unk47;
            }
            arcHeight += partner->targetZ / 256;
            sq = arcHeight * arcHeight;
            arcHeight = arcHeight + MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x400, (sq / 0x40) + 0x20);
            body->height = arcHeight;
            set_motion_offsets(body, 0x800);
        }
        ((struct HeroMirrorBits *)&body->sprite->mirror)->flip = 1;
        body_a->timer = body_a->timer & 0xFFF;
        body->unkA8 = 1;
        body->timer = body->timer & 0xFFF;
        body->state = body_step_window_d16c0;
    }
}

ASM_FUNC("asm/code_080D0374/body_step_window_d16c0.s", void body_step_window_d16c0(struct HeroBody *body))
