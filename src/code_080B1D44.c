#include "global.h"
#include "ram_routines.h"

struct HeroSprite
{
    u8 filler0[0x06];
    u16 scale;
    u8 filler8[0x11 - 0x08];
    u8 mirrorFlags;
    u8 flags;
    u8 filler13[0x20 - 0x13];
    u8 frame;
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

struct HeroFlagBits
{
    u8 unk0 : 5;
    u8 active : 1;
    u8 unk6 : 2;
};

struct HeroCcBits
{
    u8 low : 7;
    u8 bit7 : 1;
};

struct HeroFlag111Bits
{
    u8 unk0 : 1;
    u8 bit1 : 1;
    u8 unk2 : 6;
};

struct HeroData
{
    u8 filler0[0xD8];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler0E4[0xEC - 0xE4];
    s16 pose;
    u8 filler0EE[0xF6 - 0xEE];
    s16 flagsF6;
    u8 filler0F8[0x111 - 0xF8];
    u8 flags111;
    u8 filler112[0x114 - 0x112];
    u32 poseValue;
    u8 filler118[0x11E - 0x118];
    u16 flags;
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
    u16 timer2;
    struct HeroData *data;
    struct HeroBody *partner;
    struct HeroBody *next;
    u8 filler34[0x38 - 0x34];
    s32 targetX;
    s32 targetY;
    s32 targetZ;
    u8 filler44[0x47 - 0x44];
    s8 unk47;
    u8 filler48[0x4C - 0x48];
    void (*state)(struct HeroBody *);
    u32 unk50;
    u8 filler54[0x76 - 0x54];
    u8 mode;
    u8 filler77[0x79 - 0x77];
    u8 flags;
    u16 rate;
    u16 frame;
    s16 progress;
    u8 filler80[0x84 - 0x80];
    s16 spanA;
    s16 spanB;
    u8 filler88[0x8C - 0x88];
    s32 spanZ;
    u8 filler90[0x92 - 0x90];
    s16 height;
    s32 ratioLo;
    s32 ratioHi;
    u32 pressCount;
    u32 pressMask;
    u8 filler_A4[0xA8 - 0xA4];
    u32 link;
    s16 step;
    u16 timer;
    s16 deltaY;
    s16 vel;
    u8 filler_B4[0xC3 - 0xB4];
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
    u8 filler0[0x70];
    struct Hero *heroA;
    struct Hero *heroB;
};

struct KeyState
{
    u8 filler0[0x3E];
    u16 pressed;
};

/* During the pair moves 0x84 and 0x88 hold the point hero A heads for (s32, 24.8 fixed
   point), and 0xAE / 0xB0 hold the body's last X / Y offset from its target. */
#define BODY_DEST_X(body) (*(s32 *)&(body)->spanA)
#define BODY_DEST_Y(body) (*(s32 *)&(body)->filler88)
#define BODY_DELTA_X(body) (*(s16 *)&(body)->timer)

/* Body mode bits 1-2: 2 or 4 means the body is active on the field. */
#define HERO_MODE_ACTIVE(mode) (((mode) & 6) == 2 || ((mode) & 6) == 4)

/* HeroSprite.flags bit: the current animation has finished. */
#define SPRITE_FLAG_ANIM_DONE 0x08

extern struct World *gWorld;
extern struct KeyState gSys;

void step_motion(struct HeroBody *body);
void set_slot_value(u32 id, s32 value);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void body_set_pose(u32 art, struct HeroBody *body, s32 x, s32 y);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 h, struct HeroBody *owner);
void hero_a_walk_toward_partner(struct HeroBody *body);
void hero_a_drop_and_land_with_b(struct HeroBody *body);
void hero_a_fall_settle_211c(struct HeroBody *body);
void hero_pair_attach_2dac(struct HeroBody *body);
void hero_pair_scale_and_settle(struct HeroBody *body);
void pair_end_place_both_b(struct HeroBody *body);
void resolve_contact_action(struct HeroBody *a, struct HeroBody *b, u32 c, u32 d, u32 e);
void set_motion_offsets(struct HeroBody *body, u32 offsets);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void set_motion_start(struct HeroBody *body, u32 speed);
void enter_finish_rebuild(struct HeroBody *body);
void hero_pair_finish(struct HeroBody *body);

/* Pair-move state: steps both heroes until hero A has caught up with hero B (A's X at or past
   B's). Then both heroes still on the field take their jump actions, hero A's destination is
   set 0x30 units above the partner (its position plus its offsets), the low 12 bits of timer2
   (the effect-pending count) are set to 1 and hero_a_walk_toward_partner takes over. */
void hero_a_copy_partner_pos(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct HeroBody *body_a = &world->heroA->body;
    struct HeroBody *body_b = &world->heroB->body;
    struct HeroBody *partner = body->partner;
    u32 high_bits;
    u32 timer2;

    step_motion(body_a);
    step_motion(body_b);
    if (body_a->posX >= body_b->posX)
    {
        set_slot_value(0xAE, -1);
        if (HERO_MODE_ACTIVE(body_a->mode))
        {
            sprite_set_action(body_a, 0xB, 0x2032, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            sprite_set_action(body_b, 0xF, 0x2061, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 0;
        }
        BODY_DEST_X(body_a) = partner->posX + (partner->ofsX << 8);
        BODY_DEST_Y(body_a) = partner->posY + (partner->ofsY << 8);
        body_a->spanZ = partner->posZ + (partner->ofsZ << 8) + 0x3000;
        timer2 = body_a->timer2;
        high_bits = 0xFFFFF000;
        high_bits &= timer2;
        body_a->timer2 = high_bits | 1;
        body->state = hero_a_walk_toward_partner;
    }
}

/* Pair-move state: eases hero A toward the destination hero_a_copy_partner_pos set
   (9/10 of its position plus 1/10 of the destination, averaged with its last offset),
   lifts it one unit a frame and carries hero B along. When the offset from its target
   gets short it spawns effect 0x2FBA once; when it is under 9 both heroes take their
   landing actions and hero A drops onto the partner (hero_a_drop_and_land_with_b). */
void hero_a_walk_toward_partner(struct HeroBody *body)
{
    struct Hero *hero_a = gWorld->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct HeroBody *body_b = &gWorld->heroB->body;
    struct HeroBody *partner = body->partner;
    s32 eased_x;
    s32 eased_y;
    s32 dist;

    eased_x = (body_a->posX * 0xE6 + BODY_DEST_X(body_a) * 0x1A) / 0x100;
    eased_y = (body_a->posY * 0xE6 + BODY_DEST_Y(body_a) * 0x1A) / 0x100;

    body_a->posX += (eased_x - body_a->targetX + BODY_DELTA_X(body_a)) / 2;
    body_a->posY += (eased_y - body_a->targetY + body_a->deltaY) / 2;
    body_a->posZ += 0x100;
    body_b->posX = body_a->posX;
    body_b->posY = body_a->posY;
    body_b->posZ = body_a->posZ;
    BODY_DELTA_X(body_a) = body_a->posX - body_a->targetX;
    body_a->deltaY = body_a->posY - body_a->targetY;

    dist = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(BODY_DELTA_X(body_a) * BODY_DELTA_X(body_a)
                                                  + body_a->deltaY * body_a->deltaY);
    if (dist < 0x10 && (body_a->timer2 & 0xFFF))
    {
        /* the low 12 bits are the "effect pending" count hero_a_copy_partner_pos set */
        u32 high_bits = 0xFFFFF000;

        body_a->timer2 &= high_bits;
        effect_spawn_at(0x2FBA, body_a->posX / 0x100, body_a->posY / 0x100, body_a->posZ / 0x100 - 14, body_a);
    }
    if (dist < 9)
    {
        set_slot_value(0x8E, -1);
        if (HERO_MODE_ACTIVE(body_a->mode))
        {
            sprite_set_action(body_a, 0xA, 0x2032, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            sprite_set_action(body_b, 0xE, 0x2061, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 0;
        }
        body_a->vel = body_a->posZ - body_a->targetZ;
        body_a->spanZ = partner->posZ + (partner->ofsZ << 8);
        body_a->link = 1;
        body->state = hero_a_drop_and_land_with_b;
    }
}

/* Pair-move state after hero_a_walk_toward_partner: hero A (carrying hero B along) falls onto
   the partner. Its vertical speed is damped to 217/256 and pulled down by gravity (0x4CCC/256)
   each frame. While the button window (link) is open, pressing A or B gives hero B pose 0x16
   once. When A reaches the height set on the partner it lands: hero B, if on the field, gets a
   new rate (0x4000 / 11) and its progress reset, A's speed is halved and hero_a_fall_settle_211c
   takes over. */
void hero_a_drop_and_land_with_b(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct HeroBody *body_a = &world->heroA->body;
    struct HeroBody *body_b = &world->heroB->body;
    s32 vel;

    if (body_a->link != 0)
    {
        if ((gSys.pressed & (A_BUTTON | B_BUTTON)) != 0)
        {
            body_set_pose(6, body_b, 0x16, 0);
            body_a->link = 0;
        }
    }
    vel = body_a->vel * 217 - 0x4CCC;
    if (vel < 0)
        vel += 0xFF;
    body_a->vel = vel >> 8;
    body_a->posZ += body_a->vel;
    body_b->posZ = body_a->posZ;
    if (body_a->spanZ >= body_a->posZ)
    {
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            body_b->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 11);
            body_b->progress = 0;
            ((struct HeroFlagBits *)&body_b->flags)->active = 0;
        }
        body_a->pressCount = 0;
        body_a->pressMask = 1;
        body_a->vel = body_a->vel / 2;
        body_b->timer = body_b->timer & 0xFFF;
        body->state = hero_a_fall_settle_211c;
    }
}

ASM_FUNC("asm/code_080B1D44/hero_a_fall_settle_211c.s", void hero_a_fall_settle_211c(struct HeroBody *body))

// Pair-action state: waits for hero B's sprite animation to finish (sprite flag 8), then
// ends the bind on the partner: runs contact script 0x040308A8 between B and the partner,
// spawns effect 0x70B on the partner if it is still showing or flagged, puts both heroes that
// stand on the field (mode 2 or 4) into action 5, mirrors B and sends it 0x28 units right and
// up, then hands over to hero_pair_scale_and_settle.
void hero_b_bind_partner_fx(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct HeroBody *body_a = &world->heroA->body;
    struct Hero *hero_b = world->heroB;
    struct HeroBody *body_b = &hero_b->body;
    struct HeroBody *partner = body->partner;
    s32 fx_x;
    s32 fx_y;
    s32 fx_z;

    if ((body_b->sprite->flags & 8) != 0)
    {
        set_slot_value(0x63, -1);
        ((struct HeroFlag111Bits *)&partner->data->flags111)->bit1 = 0;
        ((struct HeroCcBits *)&body_b->cc)->low = 0;
        body_b->scale = 0x100;
        resolve_contact_action(body_b, partner, 0, 0, 0x040308A8);
        body_b->count++;

        // Effect at the partner's position (8.8 fixed point plus its sprite offset).
        if (partner->data->flagsF6 > 0 || (partner->data->flags111 & 2) != 0)
        {
            fx_x = partner->targetX / 0x100 + partner->ofsX;
            fx_y = partner->targetY / 0x100 + partner->ofsY;
            fx_z = partner->targetZ / 0x100 + partner->ofsZ;
            effect_spawn_at(0x70B, fx_x, fx_y, fx_z, partner);
        }

        // Action 5 for each hero on the field: animation 0x2000 without a pose, else 0x204D.
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            if (body_a->data->pose == -1)
                sprite_set_action(body_a, 5, 0x2000, 0);
            else
                sprite_set_action(body_a, 5, 0x204D, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            if (body_b->data->pose == -1)
                sprite_set_action(body_b, 5, 0x2000, 0);
            else
                sprite_set_action(body_b, 5, 0x204D, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
        }
        ((struct HeroMirrorBits *)&body_b->sprite->mirrorFlags)->mirror = 1;

        // Hero B moves 0x28 units right of where it stands and 0x28 up.
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            place_targets(body_b, body_b->targetX / 0x100 + body_b->ofsX + 0x28,
                          body_b->targetY / 0x100 + body_b->ofsY, 0, -1);
            body_b->height = body_b->targetZ / 0x100 + 0x28;
            set_motion_offsets(body_b, 0x600);
        }
        *(s32 *)&body_a->spanA = body_a->posX;
        body->state = hero_pair_scale_and_settle;
    }
}

NONMATCH("asm/code_080B1D44/hero_pair_scale_and_settle.s", void hero_pair_scale_and_settle(struct HeroBody *body))
{
    struct World *world = gWorld;
    struct HeroBody *body_a = &world->heroA->body;
    struct Hero *hero_b = world->heroB;
    struct HeroBody *body_b = &hero_b->body;
    struct HeroBody *partner = body->partner;
    s32 ratio;
    s32 inRange;
    s32 ratioLo;
    u16 scale;

    step_motion(body_b);
    scale = partner->sprite->scale;
    if ((s16)partner->sprite->scale <= 0xFF)
    {
        partner->sprite->scale = scale + 8;
        if ((s16)partner->sprite->scale > 0x100)
            partner->sprite->scale = 0x100;
    }
    body_a->posX = *(s32 *)&body_a->spanA * 2 - body_b->posX;
    body_a->posY = body_b->posY;
    body_a->posZ = body_b->posZ;
    ratio = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(body_b->ratioHi << 14, body_b->ratioLo);

    inRange = 0;
    if (body_b->step <= ratio && body_b->progress >= ratio)
        inRange = 1;
    if (inRange)
    {
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            if (body_a->data->pose == -1)
                sprite_set_action(body_a, 6, 0x2000, 0);
            else
                sprite_set_action(body_a, 6, 0x204D, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            if (body_b->data->pose == -1)
                sprite_set_action(body_b, 6, 0x2000, 0);
            else
                sprite_set_action(body_b, 6, 0x204D, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
        }
    }
    if ((body_b->flags & 0x20) != 0)
    {
        s32 minus1 = -1;

        set_slot_value(0x2E, minus1);
        partner->sprite->scale = 0x100;
        effect_spawn_at(0x1E, body_b->posX / 0x100, body_b->posY / 0x100, body_b->posZ / 0x100, body_b);
        effect_spawn_at(0x1E, body_a->posX / 0x100, body_a->posY / 0x100, body_a->posZ / 0x100, body_a);
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            if (body_a->data->pose == minus1)
                sprite_set_action(body_a, 8, 0x2000, 0);
            else
                sprite_set_action(body_a, 8, 0x204D, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            if (body_b->data->pose == minus1)
                sprite_set_action(body_b, 8, 0x2000, 0);
            else
                sprite_set_action(body_b, 8, 0x204D, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
        }
        body->state = pair_end_place_both_b;
    }
}
END_NONMATCH

/* Waits for hero B's animation to end, then sends hero B and then hero A back
   to their home targets, puts them in their idle pose facing along their span
   and hands each one over to the finish-rebuild state. */
void pair_end_place_both_b(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct HeroBody *body_a = &world->heroA->body;
    struct Hero *hero_b = world->heroB;
    struct HeroBody *body_b = &hero_b->body;
    struct HeroData *data;
    struct HeroSprite *sprite;
    u8 *mode;
    s16 *span;
    s32 mirror;
    s32 home_x;
    s32 home_y;
    s32 home_z;
    s32 fixed;

    if ((body_b->sprite->flags & SPRITE_FLAG_ANIM_DONE) != 0)
    {
        set_slot_value(0x53, -1);
        if (HERO_MODE_ACTIVE(hero_b->body.mode))
        {
            /* home position, 24.8 fixed point to whole units (rounding toward zero) */
            data = body_b->data;
            fixed = data->targetX;
            if (fixed < 0)
                fixed += 0xFF;
            home_x = fixed >> 8;
            fixed = data->targetY;
            if (fixed < 0)
                fixed += 0xFF;
            home_y = fixed >> 8;
            fixed = data->targetH;
            if (fixed < 0)
                fixed += 0xFF;
            home_z = fixed >> 8;
            place_targets(body_b, home_x, home_y, home_z, -1);
            set_motion_start(body_b, 0x400);
        }
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            if (body_b->data->pose == -1)
                sprite_set_action(body_b, 0, 0x2002, 0);
            else
                sprite_set_action(body_b, 0, 0x204F, 0);
            sprite = body_b->sprite;
            mirror = 0;
            span = &body_b->spanA;
            if (!(span[0] < span[1]))
                mirror = 1;
            ((struct HeroMirrorBits *)&sprite->mirrorFlags)->mirror = mirror;
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 0;
        }
        mode = &body_b->mode;
        if (HERO_MODE_ACTIVE(*mode))
        {
            body_b->state = NULL;
            enter_finish_rebuild(body_b);
        }
        if (HERO_MODE_ACTIVE(body_a->mode))
        {
            data = body_a->data;
            fixed = data->targetX;
            if (fixed < 0)
                fixed += 0xFF;
            home_x = fixed >> 8;
            fixed = data->targetY;
            if (fixed < 0)
                fixed += 0xFF;
            home_y = fixed >> 8;
            fixed = data->targetH;
            if (fixed < 0)
                fixed += 0xFF;
            home_z = fixed >> 8;
            place_targets(body_a, home_x, home_y, home_z, -1);
            set_motion_start(body_a, 0x400);
        }
        if (HERO_MODE_ACTIVE(body_a->mode))
        {
            if (body_a->data->pose == -1)
                sprite_set_action(body_a, 0, 0x2002, 0);
            else
                sprite_set_action(body_a, 0, 0x204F, 0);
            sprite = body_a->sprite;
            mirror = 0;
            span = &body_a->spanA;
            if (!(span[0] < span[1]))
                mirror = 1;
            ((struct HeroMirrorBits *)&sprite->mirrorFlags)->mirror = mirror;
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        mode = &body_a->mode;
        if (HERO_MODE_ACTIVE(*mode))
        {
            body_a->state = NULL;
            enter_finish_rebuild(body_a);
        }
    }
}

/* Pair-move state: counts the button presses made in alternation (A, then B, then A...:
   pressMask flips between A_BUTTON and B_BUTTON) while hero A's animation plays. When the
   animation ends, hero A, if on the field, gets a new rate (0x4000 / 0x50) and its progress
   reset, and hero_pair_attach_2dac takes over. */
void hero_a_key_toggle_step(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct Hero *hero_a = world->heroA;
    struct HeroBody *body_a = &hero_a->body;
    s32 rate;

    if ((gSys.pressed & body_a->pressMask) != 0)
    {
        body_a->pressCount++;
        body_a->pressMask ^= A_BUTTON | B_BUTTON;
    }
    if ((body_a->sprite->flags & SPRITE_FLAG_ANIM_DONE) != 0)
    {
        if (HERO_MODE_ACTIVE(body_a->mode))
        {
            rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0x50);
            body_a->rate = rate;
            body_a->progress = 0;
            ((struct HeroFlagBits *)&body_a->flags)->active = 0;
        }
        body->state = hero_pair_attach_2dac;
    }
}

ASM_FUNC("asm/code_080B1D44/hero_pair_attach_2dac.s", void hero_pair_attach_2dac(struct HeroBody *body))

NONMATCH("asm/code_080B1D44/hero_pair_arrive.s", void hero_pair_arrive(struct HeroBody *body))
{
    struct World *world = gWorld;
    struct HeroBody *body_a = &world->heroA->body;
    struct HeroBody *body_b = &world->heroB->body;
    struct HeroBody *partner = body->partner;
    s32 ratio;
    s32 lo;
    s32 hi;
    s32 inRange;
    u16 scale;

    step_motion(body_b);
    scale = partner->sprite->scale;
    if ((s16)partner->sprite->scale <= 0xFF)
    {
        partner->sprite->scale = scale + 8;
        if ((s16)partner->sprite->scale > 0x100)
            partner->sprite->scale = 0x100;
    }
    body_a->posX = *(s32 *)&body_a->spanA * 2 - body_b->posX;
    body_a->posY = body_b->posY;
    body_a->posZ = body_b->posZ;
    lo = body_b->ratioLo;
    hi = body_b->ratioHi;
    ratio = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(hi << 14, lo);

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
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            if (body_a->data->pose == -1)
                sprite_set_action(body_a, 6, 0x2000, 0);
            else
                sprite_set_action(body_a, 6, 0x204D, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            if (body_b->data->pose == -1)
                sprite_set_action(body_b, 6, 0x2000, 0);
            else
                sprite_set_action(body_b, 6, 0x204D, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
        }
    }
    if ((body_b->flags & 0x20) != 0)
    {
        s32 minus1 = -1;

        set_slot_value(0x2E, minus1);
        partner->sprite->scale = 0x100;
        effect_spawn_at(0x1E, body_b->posX / 0x100, body_b->posY / 0x100, body_b->posZ / 0x100, body_b);
        effect_spawn_at(0x1E, body_a->posX / 0x100, body_a->posY / 0x100, body_a->posZ / 0x100, body_a);
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            if (body_a->data->pose == minus1)
                sprite_set_action(body_a, 8, 0x2000, 0);
            else
                sprite_set_action(body_a, 8, 0x204D, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            if (body_b->data->pose == -1)
                sprite_set_action(body_b, 8, 0x2000, 0);
            else
                sprite_set_action(body_b, 8, 0x204D, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
        }
        body->state = hero_pair_finish;
    }
}
END_NONMATCH
