#include "global.h"
#include "ram_routines.h"

struct HeroSprite
{
    u8 filler0[0x06];
    s16 scale;
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
};

struct HeroBody
{
    u8 filler0[0x08];
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
    const u8 *anim;
    u8 filler70[0x76 - 0x70];
    u8 mode;
    u8 filler77[0x79 - 0x77];
    u8 flags;
    u16 rate;
    u16 frame;
    u16 progress;
    u8 filler80[0x84 - 0x80];
    s16 spanA;
    s16 spanB;
    u8 filler88[0x92 - 0x88];
    s16 height;
    u8 filler94[0xAC - 0x94];
    s16 step;
    u16 timer;
    u8 filler_B0[0xB2 - 0xB0];
    u16 unkB2;
    u8 filler_B4[0xC3 - 0xB4];
    s8 ofsX;
    s8 ofsY;
    s8 ofsZ;
};

/* Bits 1-2 of HeroBody.mode are 1 or 2: the body is drawn, so its sprite can be animated. */
#define HERO_MODE_ACTIVE(mode) (((mode) & 6) == 2 || ((mode) & 6) == 4)

/* HeroSprite.flags bit 3: the current animation has finished. */
#define HERO_SPRITE_ANIM_DONE 8

/* HeroBody.flags bit 5: the body's motion has reached its target. */
#define HERO_FLAG_MOTION_DONE 0x20

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
    u8 unk0E;
    u8 filler0F[0x70 - 0x0F];
    struct Hero *heroA;
    struct Hero *heroB;
};

extern struct World *gWorld;
extern u8 *gParty;

void step_motion(struct HeroBody *body);
void step_motion_scaled(struct HeroBody *body);
void set_slot_value(u32 id, s32 value);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void set_motion_offsets(struct HeroBody *body, u32 offsets);
void set_motion_start(struct HeroBody *body, u32 speed);
void enter_finish_rebuild(struct HeroBody *body);
void obj_detach_active(struct HeroSprite *sprite);
void obj_release(struct HeroBody *obj);
void release_slot4_tiles(void);
u32 find_frame_entry(struct HeroBody *obj, u32 index);
void schedule_pose_task(struct HeroBody *partner);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 h, struct HeroBody *owner);

void hero_pair_finish(struct HeroBody *body);
void body_set_target_offset(struct HeroBody *body);
void body_step_reset_pose(struct HeroBody *body);
void hero_a_start_anim_2032(struct HeroBody *body);
void hero_b_steps(struct HeroBody *body);
void hero_b_set_take_target_state(struct HeroBody *body);
void hero_b_set_fall_timers(struct HeroBody *body);
void hero_b_enter_side(struct HeroBody *body);
void hero_a_stop_anim(struct HeroBody *body);
void hero_pair_end_when_idle(struct HeroBody *body);
void hero_a_anim_2032_sprite(struct HeroBody *body);
u32 pair_arrive_hero_pair_start_b(struct HeroBody *body);
void hero_pair_place_offset(struct HeroBody *body);
void hero_b_begin_step_16(struct HeroBody *body);
void hero_b_anim_on_sprite(struct HeroBody *body);
void hero_b_go_to_a_target(struct HeroBody *body);
void hero_b_step_window_4008(struct HeroBody *body);
void hero_b_step_scaled(struct HeroBody *body);
void hero_b_go_to_partner_height(struct HeroBody *body);
void hero_b_partner_height_4c48(struct HeroBody *body);

void hero_pair_place_b_beside_a(struct HeroBody *body);
void hero_b_take_target_pose(struct HeroBody *body);
void hero_b_fall_and_land(struct HeroBody *body);
void hero_a_step_16_b_idle(struct HeroBody *body);
void hero_a_copy_partner_pos(struct HeroBody *body);
void hero_b_advance_anim(struct HeroBody *body);
void hero_b_stop_anim(struct HeroBody *body);
void hero_b_place_beside_partner(struct HeroBody *body);

/*
 * Once hero B's animation has finished (sound 0x53), sends hero B and then hero A back to
 * their home positions (data->target, in pixels) at speed 0x400, each in its standing pose
 * (0x2002, or 0x204F when it has a pose) facing the way its span says, and leaves both
 * waiting in enter_finish_rebuild for the walk to end.
 */
void hero_pair_finish(struct HeroBody *body)
{
    struct HeroBody *bodyA;
    struct Hero *heroB;
    struct HeroBody *bodyB;
    struct HeroData *data;
    s32 t;
    s32 homeX;
    s32 homeY;
    s32 homeH;
    s16 *span;

    bodyA = &gWorld->heroA->body;
    heroB = gWorld->heroB;
    bodyB = &heroB->body;
    if (bodyB->sprite->flags & HERO_SPRITE_ANIM_DONE)
    {
        set_slot_value(0x53, -1);

        /* hero B */
        if (HERO_MODE_ACTIVE(heroB->body.mode))
        {
            data = bodyB->data;
            t = data->targetX;
            if (t < 0)
                t += 0xff;
            homeX = t >> 8;
            t = data->targetY;
            if (t < 0)
                t += 0xff;
            homeY = t >> 8;
            t = data->targetH;
            if (t < 0)
                t += 0xff;
            homeH = t >> 8;
            place_targets(bodyB, homeX, homeY, homeH, -1);
            set_motion_start(bodyB, 0x400);
        }
        if (HERO_MODE_ACTIVE(bodyB->mode))
        {
            if (bodyB->data->pose == -1)
            {
                sprite_set_action(bodyB, 0, 0x2002, 0);
            }
            else
            {
                sprite_set_action(bodyB, 0, 0x204F, 0);
            }
            ((struct HeroMirrorBits *)&bodyB->sprite->mirrorFlags)->mirror = (span = &bodyB->spanA)[0] >= span[1];
            ((struct HeroSpriteBits *)&bodyB->sprite->flags)->mode = 0;
        }
        if (HERO_MODE_ACTIVE(bodyB->mode))
        {
            bodyB->state = 0;
            enter_finish_rebuild(bodyB);
        }

        /* hero A */
        if (HERO_MODE_ACTIVE(bodyA->mode))
        {
            data = bodyA->data;
            t = data->targetX;
            if (t < 0)
                t += 0xff;
            homeX = t >> 8;
            t = data->targetY;
            if (t < 0)
                t += 0xff;
            homeY = t >> 8;
            t = data->targetH;
            if (t < 0)
                t += 0xff;
            homeH = t >> 8;
            place_targets(bodyA, homeX, homeY, homeH, -1);
            set_motion_start(bodyA, 0x400);
        }
        if (HERO_MODE_ACTIVE(bodyA->mode))
        {
            if (bodyA->data->pose == -1)
            {
                sprite_set_action(bodyA, 0, 0x2002, 0);
            }
            else
            {
                sprite_set_action(bodyA, 0, 0x204F, 0);
            }
            ((struct HeroMirrorBits *)&bodyA->sprite->mirrorFlags)->mirror = (span = &bodyA->spanA)[0] >= span[1];
            ((struct HeroSpriteBits *)&bodyA->sprite->flags)->mode = 0;
        }
        if (HERO_MODE_ACTIVE(bodyA->mode))
        {
            bodyA->state = 0;
            enter_finish_rebuild(bodyA);
        }
    }
}

void body_set_target_offset(struct HeroBody *body)
{
    s32 tx;
    s32 ty;
    s32 tz;
    s32 px;
    s32 py;

    if ((body->mode & 6) == 2 || (body->mode & 6) == 4)
    {
        tx = body->targetX;
        if (tx < 0)
        {
            tx += 0xff;
        }
        px = (tx >> 8) + body->ofsX - 0x1e;
        ty = body->targetY;
        if (ty < 0)
        {
            ty += 0xff;
        }
        py = (ty >> 8) + body->ofsY;
        place_targets(body, px, py, 0, -1);
        tz = body->targetZ;
        if (tz < 0)
        {
            tz += 0xff;
        }
        body->height = (tz >> 8) + 0x28;
        set_motion_offsets(body, 0x600);
    }
    body->state = body_step_reset_pose;
}

void body_step_reset_pose(struct HeroBody *body)
{
    step_motion(body);
    if ((body->flags & 0x20) != 0)
    {
        obj_detach_active(body->sprite);
        body->state = hero_pair_end_when_idle;
    }
}

void hero_a_start_anim_2032(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct HeroBody *body_a;

    {
        s32 m = world->mode;

        m |= 0xC0;
        world->mode = m;
    }
    body_a = &gWorld->heroA->body;
    if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
    {
        sprite_set_action(body_a, 8, 0x2032, 0);
        ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
    }
    body->state = hero_a_anim_2032_sprite;
}

void hero_b_steps(struct HeroBody *body)
{
    body->state = hero_pair_place_b_beside_a;
}

void hero_b_set_take_target_state(struct HeroBody *body)
{
    body->state = hero_b_take_target_pose;
}

void hero_b_set_fall_timers(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct HeroBody *body_b = &world->heroB->body;

    body_b->timer = 0x400;
    body_b->unkB2 = 0x400;
    body->state = hero_b_fall_and_land;
}

void hero_b_enter_side(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct HeroBody *body_b = &world->heroB->body;
    struct HeroBody *partner = body->partner;

    step_motion(body_b);
    if ((body_b->flags & 0x20) != 0)
    {
        effect_spawn_at(0x2FF9, body_b->x / 0x100, body_b->y / 0x100, body_b->z / 0x100, body_b);
        set_slot_value(0x30, -1);
        body_b->x += 0x12000;
        body_b->targetX = body_b->x;
        body_b->targetY = body_b->y = partner->y;
        body->state = hero_b_place_beside_partner;
    }
}

void hero_a_stop_anim(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct HeroBody *body_a = &world->heroA->body;

    step_motion(body_a);
    if ((body_a->flags & 0x20) != 0)
    {
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            sprite_set_action(body_a, 0, -1, 0);
            ((struct HeroMirrorBits *)&body_a->sprite->mirrorFlags)->mirror = 0;
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        body->state = hero_a_step_16_b_idle;
    }
}

void hero_pair_end_when_idle(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct HeroBody *body_a = &world->heroA->body;
    struct HeroBody *body_b = &world->heroB->body;

    if (body_a->state == 0 && body_b->state == 0)
    {
        release_slot4_tiles();
        obj_release(body);
    }
}

void hero_a_anim_2032_sprite(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct HeroBody *body_a = &world->heroA->body;
    struct HeroBody *body_b = &world->heroB->body;

    step_motion(body_b);
    if ((body_a->sprite->flags & 8) != 0)
    {
        set_slot_value(0x53, -1);
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            sprite_set_action(body_a, 9, 0x2032, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
        body->state = hero_a_copy_partner_pos;
    }
}

u32 pair_arrive_hero_pair_start_b(struct HeroBody *body)
{
    struct HeroData *data;
    u8 *record;
    struct HeroBody *body_a;
    struct HeroBody *body_b;
    struct HeroBody *partner;
    const u8 *anim;

    record = gParty - ((((body->data->pose << 4) - body->data->pose) << 2) + 0x3C);
    data = body->data;
    data->poseValue = *(u16 *)(record + 0x2A);
    body_a = &gWorld->heroA->body;
    body_b = &gWorld->heroB->body;
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
    body->state = hero_pair_place_offset;
    body_b->count = 0;
    body_a->count = 0;
    schedule_pose_task(partner);
    return 1;
}

NONMATCH("asm/code_080B3670/hero_pair_place_offset.s", void hero_pair_place_offset(struct HeroBody *body))
{
    struct World *world;
    struct HeroBody *bodyA;
    struct HeroBody *bodyB;
    u8 *party;
    s32 one;
    s32 bitA;
    u32 hi;
    u32 lo;
    u32 bits;

    gWorld->unk0E = 4;
    world = gWorld;
    party = gParty;
    one = 1;
    bitA = one & ((*(u32 *)(party + 0x44) << 14) >> 26);
    hi = party[0x47] >> 6;
    lo = party[0x48];
    bits = (bitA + (((((lo & 0xF) << 2) | hi) & one) << 1)) << 6;
    world->mode = (world->mode & 0x3F) | bits;
    bodyA = &gWorld->heroA->body;
    bodyB = &gWorld->heroB->body;
    set_slot_value(0x53, -1);
    if ((bodyA->mode & 6) == 2 || (bodyA->mode & 6) == 4)
    {
        s32 t;
        s32 px;
        s32 py;
        s32 pz;
        struct HeroData *data = bodyA->data;
        t = data->targetX;
        if (t < 0)
            t += 0xff;
        px = (t >> 8) + 8;
        t = data->targetY;
        if (t < 0)
            t += 0xff;
        py = (t >> 8) + 0x10;
        t = data->targetH;
        if (t < 0)
            t += 0xff;
        pz = t >> 8;
        place_targets(bodyA, px, py, pz, -1);
        set_motion_start(bodyA, 0x300);
    }
    if ((bodyA->mode & 6) == 2 || (bodyA->mode & 6) == 4)
    {
        s16 *span;
        if (bodyA->data->pose == -1)
        {
            sprite_set_action(bodyA, 0, 0x2002, 0);
        }
        else
        {
            sprite_set_action(bodyA, 0, 0x204F, 0);
        }
        ((struct HeroMirrorBits *)&bodyA->sprite->mirrorFlags)->mirror = (span = &bodyA->spanA)[0] >= span[1];
        ((struct HeroSpriteBits *)&bodyA->sprite->flags)->mode = 0;
    }
    if ((bodyA->mode & 6) == 2 || (bodyA->mode & 6) == 4)
    {
        bodyA->state = 0;
        enter_finish_rebuild(bodyA);
    }
    if ((bodyB->mode & 6) == 2 || (bodyB->mode & 6) == 4)
    {
        s32 t;
        s32 px;
        s32 py;
        s32 pz;
        struct HeroData *data = bodyB->data;
        t = data->targetX;
        if (t < 0)
            t += 0xff;
        px = (t >> 8) - 0x14;
        t = data->targetY;
        if (t < 0)
            t += 0xff;
        py = (t >> 8) - 0x10;
        t = data->targetH;
        if (t < 0)
            t += 0xff;
        pz = t >> 8;
        place_targets(bodyB, px, py, pz, -1);
        set_motion_start(bodyB, 0x400);
    }
    if ((bodyB->mode & 6) == 2 || (bodyB->mode & 6) == 4)
    {
        s16 *span;
        if (bodyB->data->pose == -1)
        {
            sprite_set_action(bodyB, 0, 0x2002, 0);
        }
        else
        {
            sprite_set_action(bodyB, 0, 0x204F, 0);
        }
        ((struct HeroMirrorBits *)&bodyB->sprite->mirrorFlags)->mirror = (span = &bodyB->spanA)[0] >= span[1];
        ((struct HeroSpriteBits *)&bodyB->sprite->flags)->mode = 0;
    }
    body->state = hero_b_stop_anim;
}
END_NONMATCH

void hero_b_begin_step_16(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct HeroBody *body_a = &world->heroA->body;
    struct HeroBody *body_b = &world->heroB->body;
    s32 zero = (s32)body_a->state;

    if (zero == 0)
    {
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            body_b->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0x10);
            body_b->progress = zero;
            ((struct HeroFlagBits *)&body_b->flags)->active = 0;
        }
        body->state = hero_b_advance_anim;
    }
}

void hero_b_anim_on_sprite(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct HeroBody *body_b = &world->heroB->body;

    if ((body_b->sprite->flags & 8) != 0)
    {
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            if (body_b->data->pose == -1)
                sprite_set_action(body_b, 4, 0x2000, 0);
            else
                sprite_set_action(body_b, 4, 0x204D, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
        }
        body->state = hero_b_go_to_a_target;
    }
}

/* Once hero B's animation is done: plays sound 0x2B and sends B in a jump (action 5) at height 0x40
 * to hero A's target plus A's offset, then waits in hero_b_step_window_4008. */
void hero_b_go_to_a_target(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct HeroBody *body_a = &world->heroA->body;
    struct HeroBody *body_b = &world->heroB->body;
    s32 x;
    s32 y;
    u16 jumpHeight;

    if ((body_b->sprite->flags & HERO_SPRITE_ANIM_DONE) != 0)
    {
        set_slot_value(0x2B, -1);
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            x = body_a->targetX / 0x100 + body_a->ofsX;
            y = body_a->targetY / 0x100 + body_a->ofsY;
            place_targets(body_b, x, y, 0x3C, -1);
            jumpHeight = 0x40;
            body_b->height = jumpHeight;
            set_motion_offsets(body_b, 0x400);
        }
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            if (body_b->data->pose == -1)
                sprite_set_action(body_b, 5, 0x2000, 0);
            else
                sprite_set_action(body_b, 5, 0x204D, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
        }
        body_b->timer = body_b->timer & 0xFFF;
        body->state = hero_b_step_window_4008;
    }
}

ASM_FUNC("asm/code_080B3670/hero_b_step_window_4008.s", void hero_b_step_window_4008(struct HeroBody *body))

/* Moves hero B down onto hero A's head: while B is below A's top (A's ofsZ), A's sprite is squashed
 * to B's height (scale 0x100 = full size, sound 0x7C when it starts). Once B's motion is done, B
 * takes pose 4 and goes on to hero_b_go_to_partner_height. */
void hero_b_step_scaled(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct HeroBody *body_a = &world->heroA->body;
    struct HeroBody *body_b = &world->heroB->body;
    s32 bZ;
    s32 aTop;

    step_motion_scaled(body_b);
    bZ = body_b->z / 0x100;
    aTop = body_a->ofsZ;
    if (bZ < aTop)
    {
        if (body_a->sprite->scale == 0x100)
            set_slot_value(0x7C, -1);
        body_a->sprite->scale = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(bZ << 8, aTop);
    }
    if ((body_b->flags & HERO_FLAG_MOTION_DONE) != 0)
    {
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            if (body_b->data->pose == -1)
                sprite_set_action(body_b, 4, 0x2000, 0);
            else
                sprite_set_action(body_b, 4, 0x204D, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
        }
        body->state = hero_b_go_to_partner_height;
    }
}

/* Once hero B's animation is done: plays sound 0x2B and sends B in a jump (action 5) to its partner's
 * target, arcing a bit above the partner's top (its ofsZ, or the frame height of a non-hero partner). */
void hero_b_go_to_partner_height(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct HeroBody *body_b = &world->heroB->body;
    struct HeroBody *partner = body->partner;
    s32 t;
    s32 x;
    s32 y;
    s32 z;
    s32 height;

    if (body_b->sprite->flags & HERO_SPRITE_ANIM_DONE)
    {
        set_slot_value(0x2B, -1);
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            x = partner->targetX / 0x100 + partner->ofsX;
            y = partner->targetY / 0x100 + partner->ofsY;
            z = partner->targetZ / 0x100 + partner->ofsZ;
            place_targets(body_b, x, y, z, -1);
            if (partner == (struct HeroBody *)((u8 *)partner->data + 8))
            {
                height = partner->ofsZ;
            }
            else
            {
                find_frame_entry(partner, 0);
                height = partner->unk47;
            }
            t = partner->targetZ;
            if (t < 0)
                t += 0xFF;
            height += t >> 8;
            /* the arc's extra height: 3600 / (height^2 / 64 + 60) */
            t = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0xE10, (height * height) / 64 + 0x3C);
            height += t;
            body_b->height = height;
            set_motion_offsets(body_b, 0x600);
        }
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            if (body_b->data->pose == -1)
                sprite_set_action(body_b, 5, 0x2000, 0);
            else
                sprite_set_action(body_b, 5, 0x204D, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
        }
        body_b->timer = body_b->timer & 0xFFF;
        body->state = hero_b_partner_height_4c48;
    }
}

ASM_FUNC("asm/code_080B3670/hero_b_partner_height_4c48.s", void hero_b_partner_height_4c48(struct HeroBody *body))
