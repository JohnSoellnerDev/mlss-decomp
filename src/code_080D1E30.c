#include "global.h"
#include "ram_routines.h"

struct BodyData
{
    u8 filler0[0xD8];
    s32 targetX;
    s32 targetY;
    s32 targetZ;
    u8 filler0E4[0xEC - 0xE4];
    s16 pose;
    u8 filler0EE[0xF6 - 0xEE];
    s16 displayTimer;
    u8 filler0F8[0x111 - 0xF8];
    u8 flags111;
    u8 filler112[0x114 - 0x112];
    u32 poseVal;
    u8 filler118[0x11E - 0x118];
    u16 flags;
};

struct DataConnectBits
{
    u8 lo1 : 1;
    u8 connect : 1;
    u8 hi : 6;
};

struct CcBits
{
    u8 lo7 : 7;
    u8 top : 1;
};

struct SpriteEntry
{
    u8 filler0[6];
    u16 scale;
    u8 filler8[0x11 - 0x8];
    u8 unk11;
    u8 flags;
    u8 filler13[0x20 - 0x13];
    u8 layer;
    u8 anim;
};

struct SysKeys3E
{
    u8 filler0[0x3E];
    u16 keys;
};

struct ActiveBit
{
    u8 lo5 : 5;
    u8 active : 1;
    u8 hi : 2;
};

struct SpriteMirrorByte
{
    u8 lo6 : 6;
    u8 mirror : 1;
    u8 hi : 1;
};

struct SpriteModeByte
{
    u8 lo1 : 1;
    u8 mode : 2;
    u8 active : 1;
    u8 hi : 4;
};

struct Body
{
    struct Body *next;
    u8 filler4[4];
    struct SpriteEntry *sprite;
    u8 filler0C[4];
    s32 x;
    s32 y;
    s32 z;
    u8 filler1C[0x24 - 0x1C];
    u16 count;
    u8 filler26[0x28 - 0x26];
    struct BodyData *data;
    struct Body *partner;
    struct Body *child;
    u8 filler34[0x38 - 0x34];
    s32 targetX;
    s32 targetY;
    s32 targetZ;
    u8 filler44[0x47 - 0x44];
    s8 unk47;
    u8 filler48[0x4C - 0x48];
    u32 state;
    u32 slot50;
    u8 filler54[0x6C - 0x54];
    u32 anim;
    u8 filler70[0x74 - 0x70];
    u8 unk74;
    u8 unk75;
    u8 mode;
    u8 unk77;
    u8 unk78;
    u8 flags;
    u16 rate;
    u16 speed;
    s16 progress;
    u8 filler80[0x84 - 0x80];
    s16 spanA; /* motion start X in pixels (place_targets packs the current X here) */
    s16 spanB; /* motion target X in pixels */
    u8 filler88[0x92 - 0x88];
    u16 arc;
    s32 motionX;
    s32 motionY;
    u8 filler9C[0xAC - 0x9C];
    s16 stepLen;
    u16 timer;
    u8 fillerB0[0xB2 - 0xB0];
    u16 speedZ;
    u8 fillerB4[0xC3 - 0xB4];
    s8 ofsX;
    s8 ofsY;
    s8 ofsZ;
    u8 fillerC6[0xCA - 0xC6];
    u16 scale;
    u8 cc;
};

struct Hero
{
    u8 filler0[8];
    struct Body body;
};

struct World
{
    u8 filler0[0x0A];
    u8 mode;
    u8 filler0B;
    u8 view;
    u8 filler0D[0x70 - 0x0D];
    struct Hero *heroA;
    struct Hero *heroB;
    struct Body *layers;
};

/* Body mode bits 1-2: 2 or 4 means the body is active on the field. */
#define HERO_MODE_ACTIVE(mode) (((mode) & 6) == 2 || ((mode) & 6) == 4)

/* Sprite flags bit: the current animation has finished. */
#define SPRITE_FLAG_ANIM_DONE 0x08

/* Body flags bit set by step_motion / state_stack_advance_progress once the motion has finished. */
#define BODY_FLAG_MOTION_DONE 0x20

extern struct World *gWorld;
extern u8 gSys[];

void obj_release(struct Body *obj);
void sprite_set_action(struct Body *body, u32 action, s32 arg, u32 extra);
void state_stack_advance_progress(struct Body *body);
void step_motion(struct Body *body);
void set_slot_value(u32 id, s32 value);
void hero_body_blend_to_a(struct Body *body);
void step_motion_scaled(struct Body *body);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 z, struct Body *owner);
void place_targets(struct Body *body, s32 x, s32 y, s32 z, s32 flag);
void set_motion_offsets(struct Body *body, u32 a);
void hero_pair_place_both_rebuild(void);
void body_place_at_midpoint_arc(struct Body *body);
void body_fx_then_place_on_partner(struct Body *body);
void hero_a_place_own_target_anim10(struct Body *body);
void hero_a_step_scaled_fx(struct Body *body);
void hero_a_step_scaled_fx_pose(struct Body *body);
void hero_pair_place_both_rebuild_alt(void);
void enter_finish_rebuild(struct Body *body);
void set_motion_start(struct Body *body, u32 speed);
void set_motion_speed_aux(struct Body *obj, u32 value, s32 aux);
s32 find_frame_entry(struct Body *obj, s32 index);
void hero_a_place_at_child_target(struct Body *body);
void hero_a_step_window_20e8(struct Body *body);
void body_link_partner_fx_slot55(struct Body *body);
void obj_free_parts(struct Body *body);
void set_body_part(struct Body *body, s32 kind, s32 x, s32 y, s32 step);
void body_set_pose(s32 art, struct Body *body, s32 x, s32 y);
void sprite_objects_draw_obj_tiles(u32 col, u32 row);
void display_flush(void);
void body_place_at_midpoint(struct Body *body);
void hero_a_place_own_target_anim11(struct Body *body);
void hero_a_child_next_state_anim(struct Body *body);
void body_link_partner_fx_slot9d(struct Body *body);
void body_finish_object(struct Body *body);
void hero_a_step_window_20e8(struct Body *body);
s32 resolve_contact_action(struct Body *a, struct Body *b, u32 c, u32 d, u32 e);

/* Hero A jumps onto hero B's head: plays sound 0x2B (set_slot_value queues a sound
 * id/volume), sets A's jump animation, aims its motion at B's target position (0x14 below
 * the offset top) with an arc as high as B plus a little, then continues with
 * hero_a_place_at_child_target, which squashes B as A lands. */
void hero_a_place_at_b_target(struct Body *body)
{
    struct Hero *heroA = gWorld->heroA;
    struct Body *bodyA = &heroA->body;
    struct Body *bodyB = &gWorld->heroB->body;
    s32 height;

    set_slot_value(0x2B, -1);
    if (HERO_MODE_ACTIVE(heroA->body.mode))
    {
        if (bodyA->data->pose == -1)
        {
            sprite_set_action(bodyA, 5, 0x2000, 0);
        }
        else
        {
            sprite_set_action(bodyA, 5, 0x204D, 0);
        }
        ((struct SpriteModeByte *)&bodyA->sprite->flags)->mode = 1;
    }
    ((struct SpriteMirrorByte *)&bodyA->sprite->unk11)->mirror = 1;
    if (HERO_MODE_ACTIVE(bodyA->mode))
    {
        place_targets(bodyA, bodyB->targetX / 0x100 + bodyB->ofsX, bodyB->targetY / 0x100 + bodyB->ofsY,
                      bodyB->targetZ / 0x100 + bodyB->ofsZ - 0x14, -1);
        /* B's height: its own Z offset, or the current animation frame's. */
        if (bodyB == (struct Body *)((u8 *)bodyB->data + 8))
        {
            height = bodyB->ofsZ;
        }
        else
        {
            find_frame_entry(bodyB, 0);
            height = bodyB->unk47;
        }
        height += bodyB->targetZ / 0x100;
        height += MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(4, height * height / 0x40 + 2);
        bodyA->arc = height;
        set_motion_offsets(bodyA, 0x500);
    }
    body->state = (u32)hero_a_place_at_child_target;
}

/* Hero A falls onto hero B: while A is below its target height and inside B's top, B's sprite is squashed
 * by A's height. When A's motion ends (flag 0x20) it plays sound 0x2B, unmirrors, aims a small arc at its
 * child's target (0x14 left of it), sets action 0x2036 and goes on to hero_a_step_window_20e8. */
void hero_a_place_at_child_target(struct Body *body)
{
    struct Hero *heroB;
    struct Body *bodyA;
    struct Body *bodyB;
    s32 squashHeight;
    s32 zA;
    s32 x;
    s32 y;
    s32 z;
    s32 targetZ;
    s32 t;

    bodyA = &gWorld->heroA->body;
    heroB = gWorld->heroB;
    bodyB = &heroB->body;
    step_motion(bodyA);
    squashHeight = (s8)heroB->body.ofsZ - 4;
    zA = bodyA->z;
    if (zA < bodyA->targetZ)
    {
        if (zA < bodyB->z + (squashHeight << 8))
        {
            bodyB->sprite->scale = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(zA, squashHeight);
        }
    }
    if (bodyA->flags & 0x20)
    {
        set_slot_value(0x2B, -1);
        ((struct SpriteMirrorByte *)&bodyA->sprite->unk11)->mirror = 0;
        if (HERO_MODE_ACTIVE(bodyA->mode))
        {
            t = bodyA->child->targetX;
            if (t < 0)
                t += 0xff;
            x = (t >> 8) + bodyA->child->ofsX - 0x14;
            t = bodyA->child->targetY;
            if (t < 0)
                t += 0xff;
            y = (t >> 8) + bodyA->child->ofsY;
            targetZ = bodyA->child->targetZ;
            if (targetZ < 0)
                targetZ += 0xff;
            z = targetZ >> 8;
            place_targets(bodyA, x, y, z, -1);
            t = bodyA->child->targetZ;
            if (t < 0)
                t += 0xff;
            bodyA->arc = (t >> 8) + 2;
            set_motion_offsets(bodyA, 0x500);
        }
        if (HERO_MODE_ACTIVE(bodyA->mode))
        {
            sprite_set_action(bodyA, 7, 0x2036, 0);
            ((struct SpriteModeByte *)&bodyA->sprite->flags)->mode = 1;
        }
        bodyA->timer &= 0xFFF;
        body->state = (u32)hero_a_step_window_20e8;
    }
}

struct __attribute__((packed)) WorldModeBits
{
    u8 lo2 : 2;
    u8 m2 : 2;
    u8 f45 : 2;
    u8 hi : 2;
};

static inline void world_mode_or1(void)
{
    ((struct WorldModeBits *)&gWorld->mode)->f45 |= 1;
}

/* Previous progress within x and progress at least x (x clamped to 0..0x7FFF). */
static inline u32 step_reached(struct Body *a, s32 x)
{
    u32 r;
    s32 t;

    if (x < 0)
        x = 0;
    if (x > 0x7FFF)
        x = 0x7FFF;
    if (a->stepLen <= x)
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

/* Progress at least lo and previous progress within hi (both clamped, lo at most hi). */
static inline u32 step_in_window(struct Body *a, s32 lo, s32 hi)
{
    u32 r;
    s32 t;

    if (lo < 0)
        lo = 0;
    if (hi < 0)
        hi = 0;
    if (lo > 0x4000)
        lo = 0x4000;
    if (hi > 0x7FFF)
        hi = 0x7FFF;
    if (hi < lo)
        lo = hi;
    if (a->progress >= lo)
    {
        t = a->stepLen;
        r = 1;
        if (t <= hi)
            goto done;
    }
    r = 0;
done:
    return r;
}

/* Slow speed for the three body chains, layer 6 for every listed sprite. */
static inline void speeds_slow(struct Body *a, struct Body *b, struct Body *partner)
{
    struct Body *p;

    p = a;
    while (p != 0)
    {
        p->speed = 0x66;
        p = p->child;
    }
    p = b;
    while (p != 0)
    {
        p->speed = 0x66;
        p = p->child;
    }
    p = partner;
    while (p != 0)
    {
        p->speed = 0x66;
        p = p->child;
    }
    p = gWorld->layers;
    while (p != 0)
    {
        if (p->sprite != 0)
            p->sprite->layer = 6;
        p = p->next;
    }
}

/* Normal speed for the three body chains, layer 0x10 for every listed sprite. */
static inline void speeds_normal(struct Body *a, struct Body *b, struct Body *partner)
{
    struct Body *p;

    p = a;
    while (p != 0)
    {
        p->speed = 0x100;
        p = p->child;
    }
    p = b;
    while (p != 0)
    {
        p->speed = 0x100;
        p = p->child;
    }
    p = partner;
    while (p != 0)
    {
        p->speed = 0x100;
        p = p->child;
    }
    p = gWorld->layers;
    while (p != 0)
    {
        if (p->sprite != 0)
            p->sprite->layer = 0x10;
        p = p->next;
    }
}

/* Hero A steps through the timing window: hero B's sprite grows to full size, speeds and layers
   change at the window marks, the A key in the window sets a pose, then the next state. */
NONMATCH("asm/code_080D1E30/hero_a_step_window_20e8.s", void hero_a_step_window_20e8(struct Body *body))
{
    struct Body *a;
    struct Body *b;
    struct Body *partner;
    struct Body *child;
    s32 full;
    s32 rate;
    s32 start;
    s32 end;
    u32 m;
    void (*fn)(struct Body *);

    a = &gWorld->heroA->body;
    b = &gWorld->heroB->body;
    partner = body->partner;
    full = 0x4000;
    step_motion(a);

    if ((s16)b->sprite->scale <= 0xFF)
    {
        b->sprite->scale = b->sprite->scale + 0x10;
        if ((s16)b->sprite->scale > 0x100)
            b->sprite->scale = 0x100;
    }

    rate = (s16)a->rate;
    start = -(rate << 2) + 0x4000;
    end = full + (rate << 3);

    if (a->progress < end)
        ((struct ActiveBit *)&a->flags)->active = 0;

    if ((a->timer >> 12) != 0)
        goto after_zero;

    if (step_reached(a, start))
    {
        if ((gWorld->view & 8) != 0)
            speeds_slow(a, b, partner);
        else
            speeds_normal(a, b, partner);
        sprite_objects_draw_obj_tiles(0x4C, 0xC);
    }

    if (step_in_window(a, full, end))
    {
        set_body_part(a, 1, 0x20, 0, 0);
        if ((((struct SysKeys3E *)gSys)->keys & 1) != 0)
        {
            body_set_pose(2, a, 0x2E, 0);
            obj_free_parts(a);
            world_mode_or1();
            if ((((struct WorldModeBits *)&gWorld->mode)->m2 & ~1) == 0)
            {
                if ((gWorld->view & 8) != 0)
                    speeds_normal(a, b, partner);
                else
                    speeds_normal(a, b, partner);
                display_flush();
            }
            a->timer = (a->timer & 0xFFF) | 0x2000;
        }
    }

    if ((a->timer >> 12) != 0)
        goto after_zero;

    if (step_reached(a, end))
    {
        world_mode_or1();
        a->timer = (a->timer & 0xFFF) | 0x1000;
        if ((gWorld->view & 8) != 0)
            speeds_normal(a, b, partner);
        else
            speeds_normal(a, b, partner);
        display_flush();
        obj_free_parts(a);
        goto after_zero;
    }

    if (a->progress > end)
        goto after_zero;

    if ((((struct SysKeys3E *)gSys)->keys & 1) == 0)
    {
        if ((((struct SysKeys3E *)gSys)->keys & 3) == 0)
            goto after_zero;
    }

    world_mode_or1();
    if ((((struct WorldModeBits *)&gWorld->mode)->m2 & ~1) != 0)
        goto after_zero;

    a->timer = (a->timer & 0xFFF) | 0x1000;
    if ((gWorld->view & 8) != 0)
        speeds_normal(a, b, partner);
    else
        speeds_normal(a, b, partner);
    display_flush();
    obj_free_parts(a);
    body_set_pose(6, a, 0x2E, 0);

after_zero:
    {
        s32 tmode = a->timer >> 12;
        if (tmode != 0 && step_in_window(a, full, end) && tmode > 1)
        {
            a->timer &= 0xFFF;
            fn = hero_a_child_next_state_anim;
            body->state = (u32)fn;
            fn(body);
            return;
        }
    }

    if (step_reached(a, full))
    {
        m = a->mode & 6;
        if (m == 2 || m == 4)
        {
            sprite_set_action(a, 8, 0x2036, 0);
            ((struct SpriteModeByte *)&a->sprite->flags)->mode = 0;
        }
    }

    if ((a->flags & 0x20) == 0)
        return;

    set_slot_value(0x88, -1);
    child = a->child;
    m = child->mode & 6;
    if (m == 2 || m == 4)
        child->state = (u32)body_place_at_midpoint;
    m = a->mode & 6;
    if (m == 2 || m == 4)
    {
        sprite_set_action(a, 9, 0x2036, 0);
        ((struct SpriteModeByte *)&a->sprite->flags)->mode = 0;
    }
    body->state = (u32)hero_a_place_own_target_anim11;
}
END_NONMATCH

/* State handler: once hero A's current animation has finished, aim its motion back down to
 * its own target position on the ground (z 0) with speed 0 and acceleration 0x66, play
 * action 0xB, and continue with hero_a_step_scaled_fx, which steps that motion. */
void hero_a_place_own_target_anim11(struct Body *body)
{
    struct Body *heroBody;
    s32 x;
    s32 y;

    heroBody = &gWorld->heroA->body;
    if (heroBody->sprite->flags & SPRITE_FLAG_ANIM_DONE)
    {
        if (HERO_MODE_ACTIVE(heroBody->mode))
        {
            x = heroBody->targetX / 0x100 + heroBody->ofsX;
            y = heroBody->targetY / 0x100 + heroBody->ofsY;
            place_targets(heroBody, x, y, 0, -1);
            set_motion_speed_aux(heroBody, 0, 0x66);
        }
        if (HERO_MODE_ACTIVE(heroBody->mode))
        {
            sprite_set_action(heroBody, 0xB, 0x2036, 0);
            ((struct SpriteModeByte *)&heroBody->sprite->flags)->mode = 1;
        }
        body->state = (u32)hero_a_step_scaled_fx;
    }
}

/* State handler: moves hero A on its accelerating fall back to its own target (set up by
 * hero_a_place_own_target_anim11); once it has landed it plays sound 0x2E, spawns landing dust
 * (effect 0x1E) where it stands, plays action 0xC (art 0x2036) if it is on the field and hands
 * over to hero_pair_place_both_rebuild. */
void hero_a_step_scaled_fx(struct Body *body)
{
    struct Body *heroA;
    s32 x;
    s32 y;
    s32 z;

    heroA = &gWorld->heroA->body;
    step_motion_scaled(heroA);
    if (heroA->flags & BODY_FLAG_MOTION_DONE)
    {
        set_slot_value(0x2E, -1);
        /* positions are 8.8 fixed point, rounded toward 0 */
        x = heroA->x;
        if (x < 0)
            x += 0xff;
        x >>= 8;
        y = heroA->y;
        if (y < 0)
            y += 0xff;
        y >>= 8;
        z = heroA->z;
        if (z < 0)
            z += 0xff;
        z >>= 8;
        effect_spawn_at(0x1E, x, y, z, heroA);
        if (HERO_MODE_ACTIVE(heroA->mode))
        {
            sprite_set_action(heroA, 0xC, 0x2036, 0);
            ((struct SpriteModeByte *)&heroA->sprite->flags)->mode = 0;
        }
        body->state = (u32)hero_pair_place_both_rebuild;
    }
}

/*
 * State handler: once hero A's sprite animation has finished, play sound 0x53
 * (set_slot_value queues the song) and send both heroes (B first, then A) walking
 * back to their home positions stored in their BodyData. Each hero on the field
 * gets the walk action (0x204F if it has a pose, else 0x2002), faces the way it
 * walks, and then hands over to the finish-rebuild state.
 */
void hero_pair_place_both_rebuild(void)
{
    struct Body *hero_a;
    struct Body *hero_b;
    struct BodyData *home;
    s16 *move_x;
    s32 t;
    s32 home_x;
    s32 home_y;
    s32 home_z;

    hero_a = &gWorld->heroA->body;
    hero_b = &gWorld->heroB->body;
    if (hero_a->sprite->flags & 8)
    {
        set_slot_value(0x53, -1);
        if ((hero_b->mode & 6) == 2 || (hero_b->mode & 6) == 4)
        {
            home = hero_b->data;
            t = home->targetX;
            if (t < 0)
                t += 0xff;
            home_x = t >> 8;
            t = home->targetY;
            if (t < 0)
                t += 0xff;
            home_y = t >> 8;
            t = home->targetZ;
            if (t < 0)
                t += 0xff;
            home_z = t >> 8;
            place_targets(hero_b, home_x, home_y, home_z, -1);
            set_motion_start(hero_b, 0x400);
        }
        if ((hero_b->mode & 6) == 2 || (hero_b->mode & 6) == 4)
        {
            if (hero_b->data->pose == -1)
            {
                sprite_set_action(hero_b, 0, 0x2002, 0);
            }
            else
            {
                sprite_set_action(hero_b, 0, 0x204F, 0);
            }
            /* face left when the walk goes towards smaller X */
            ((struct SpriteMirrorByte *)&hero_b->sprite->unk11)->mirror = (move_x = &hero_b->spanA)[0] >= move_x[1];
            ((struct SpriteModeByte *)&hero_b->sprite->flags)->mode = 0;
        }
        if ((hero_b->mode & 6) == 2 || (hero_b->mode & 6) == 4)
        {
            hero_b->state = 0;
            enter_finish_rebuild(hero_b);
        }
        if ((hero_a->mode & 6) == 2 || (hero_a->mode & 6) == 4)
        {
            home = hero_a->data;
            t = home->targetX;
            if (t < 0)
                t += 0xff;
            home_x = t >> 8;
            t = home->targetY;
            if (t < 0)
                t += 0xff;
            home_y = t >> 8;
            t = home->targetZ;
            if (t < 0)
                t += 0xff;
            home_z = t >> 8;
            place_targets(hero_a, home_x, home_y, home_z, -1);
            set_motion_start(hero_a, 0x400);
        }
        if ((hero_a->mode & 6) == 2 || (hero_a->mode & 6) == 4)
        {
            if (hero_a->data->pose == -1)
            {
                sprite_set_action(hero_a, 0, 0x2002, 0);
            }
            else
            {
                sprite_set_action(hero_a, 0, 0x204F, 0);
            }
            ((struct SpriteMirrorByte *)&hero_a->sprite->unk11)->mirror = (move_x = &hero_a->spanA)[0] >= move_x[1];
            ((struct SpriteModeByte *)&hero_a->sprite->flags)->mode = 0;
        }
        if ((hero_a->mode & 6) == 2 || (hero_a->mode & 6) == 4)
        {
            hero_a->state = 0;
            enter_finish_rebuild(hero_a);
        }
    }
}

void body_place_at_midpoint(struct Body *body)
{
    s32 mx;
    s32 my;
    s32 t;
    s32 sy;
    s32 tz;
    struct Body *partner;

    partner = body->partner;
    if ((body->mode & 6) == 2 || (body->mode & 6) == 4)
    {
        t = partner->targetX + body->targetX;
        if (t < 0)
            t += 0xff;
        mx = ((t >> 8) + partner->ofsX) / 2;
        sy = partner->targetY + body->targetY;
        if (sy < 0)
            sy += 0xff;
        my = ((sy >> 8) + partner->ofsY) / 2;
        place_targets(body, mx, my, 0, -1);
        tz = body->targetZ;
        if (tz < 0)
            tz += 0xff;
        body->arc = (tz >> 8) + 2;
        set_motion_offsets(body, 0x300);
    }
    body->state = (u32)body_place_at_midpoint_arc;
}

/* Second hop towards the partner: once the body's motion is done (sound 0x9C), it aims at the midpoint
 * between itself and the partner (x, y with the partner's offsets) with an arc over the partner's head:
 * the partner's height (its Z offset, or the current animation frame's) plus its target Z, plus
 * (0x50 - partner top)/2 squared over (that + height^2/64). Then body_link_partner_fx_slot55 hits it. */
void body_place_at_midpoint_arc(struct Body *body)
{
    s32 height;
    struct Body *partner;
    s32 sumX;
    s32 sumY;
    s32 partnerZ;
    s32 partnerTargetZ;
    s32 ofsZ;
    s32 midX;
    s32 midY;
    s32 clearance;

    partner = body->partner;
    step_motion(body);
    if (body->flags & BODY_FLAG_MOTION_DONE)
    {
        set_slot_value(0x9C, -1);
        if (HERO_MODE_ACTIVE(body->mode))
        {
            sumX = partner->targetX + body->targetX;
            if (sumX < 0)
                sumX += 0xff;
            midX = ((sumX >> 8) + partner->ofsX) / 2;
            sumY = partner->targetY + body->targetY;
            if (sumY < 0)
                sumY += 0xff;
            midY = ((sumY >> 8) + partner->ofsY) / 2;
            place_targets(body, midX, midY, 0, -1);
            partnerZ = partner->z;
            if (partnerZ < 0)
                partnerZ += 0xff;
            partnerZ >>= 8;
            ofsZ = partner->ofsZ;
            clearance = (0x50 - (partnerZ + ofsZ)) / 2;
            if (partner == (struct Body *)((u8 *)partner->data + 8))
            {
                height = ofsZ;
            }
            else
            {
                find_frame_entry(partner, 0);
                height = partner->unk47;
            }
            partnerTargetZ = partner->targetZ;
            if (partnerTargetZ < 0)
                partnerTargetZ += 0xff;
            height += partnerTargetZ >> 8;
            height += MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(clearance * clearance, clearance + height * height / 0x40);
            body->arc = height;
            set_motion_offsets(body, 0x500);
        }
        body->state = (u32)body_link_partner_fx_slot55;
    }
}

/* Moves the body until its motion is done (flags 0x20), then hits its partner: releases
 * the partner's connect bit, plays sound 0x55 (set_slot_value queues a sound), resolves the
 * contact and counts the hit. If the partner is still showing damage, spawns a hit effect
 * at it (0x7D for sprite anims 3, 5, 7, 9 and 11, else effect 0); an active body then
 * finishes with body_finish_object. */
void body_link_partner_fx_slot55(struct Body *body)
{
    struct Body *target;
    s32 hitY;
    s32 hitX;

    target = body->partner;
    step_motion(body);
    if (body->flags & 0x20)
    {
        ((struct DataConnectBits *)&target->data->flags111)->connect = 0;
        set_slot_value(0x55, -1);
        ((struct CcBits *)&body->cc)->lo7 = 0x19;
        body->scale = 0x100;
        resolve_contact_action(body, target, 0, 0, 0x0423448C);
        body->count++;
        if (target->data->displayTimer > 0 || (target->data->flags111 & 2))
        {
            switch (body->sprite->anim - 3)
            {
            case 0:
            case 2:
            case 4:
            case 6:
            case 8:
                hitX = target->targetX;
                if (hitX < 0)
                    hitX += 0xff;
                hitX >>= 8;
                hitX += target->ofsX;
                hitY = target->targetY;
                if (hitY < 0)
                    hitY += 0xff;
                hitY >>= 8;
                hitY += target->ofsY;
                effect_spawn_at(0x7D, hitX, hitY, 0, target);
                break;
            default:
                hitX = target->targetX;
                if (hitX < 0)
                    hitX += 0xff;
                hitX >>= 8;
                hitX += target->ofsX;
                hitY = target->targetY;
                if (hitY < 0)
                    hitY += 0xff;
                hitY >>= 8;
                hitY += target->ofsY;
                effect_spawn_at(0, hitX, hitY, 0, target);
                break;
            }
        }
        if (HERO_MODE_ACTIVE(body->mode))
        {
            body->state = (u32)body_finish_object;
        }
    }
}

void hero_a_child_next_state_anim(struct Body *body)
{
    struct Body *hbody;
    struct Body *child;

    hbody = &gWorld->heroA->body;
    set_slot_value(0x85, -1);
    child = hbody->child;
    if ((child->mode & 6) == 2 || (child->mode & 6) == 4)
    {
        child->state = (u32)body_fx_then_place_on_partner;
    }
    if ((hbody->mode & 6) == 2 || (hbody->mode & 6) == 4)
    {
        sprite_set_action(hbody, 9, 0x2036, 0);
        ((struct SpriteModeByte *)&hbody->sprite->flags)->mode = 0;
    }
    body->state = (u32)hero_a_place_own_target_anim10;
}

/* Once hero A's animation has ended and if it can still act: drops it back onto its own place
 * (its target position plus its offsets, on the ground) starting at speed 0 with gravity 0x66, and
 * starts its action 0xA (art 0x2036). hero_a_step_scaled_fx_pose waits for the landing. */
void hero_a_place_own_target_anim10(struct Body *body)
{
    struct Body *heroBody;
    s32 homeX;
    s32 homeY;

    heroBody = &gWorld->heroA->body;
    if (heroBody->sprite->flags & 8)
    {
        if (HERO_MODE_ACTIVE(heroBody->mode))
        {
            homeX = heroBody->targetX / 0x100 + heroBody->ofsX;
            homeY = heroBody->targetY / 0x100 + heroBody->ofsY;
            place_targets(heroBody, homeX, homeY, 0, -1);
            set_motion_speed_aux(heroBody, 0, 0x66);
        }
        if (HERO_MODE_ACTIVE(heroBody->mode))
        {
            sprite_set_action(heroBody, 0xA, 0x2036, 0);
            ((struct SpriteModeByte *)&heroBody->sprite->flags)->mode = 1;
        }
        body->state = (u32)hero_a_step_scaled_fx_pose;
    }
}

/* State handler: moves hero A on its accelerating fall back to its own target (set up with
 * set_motion_speed_aux); once it has landed it plays sound 0x65, spawns effect 0x1E where it
 * stands, takes its landing pose (action 8: 0x204D if it has a pose, else 0x2000) and hands over
 * to hero_pair_place_both_rebuild_alt. */
void hero_a_step_scaled_fx_pose(struct Body *body)
{
    struct Body *heroA;
    s32 x;
    s32 y;
    s32 z;

    heroA = &gWorld->heroA->body;
    step_motion_scaled(heroA);
    if (heroA->flags & BODY_FLAG_MOTION_DONE)
    {
        set_slot_value(0x65, -1);
        /* positions are 8.8 fixed point, rounded toward 0 */
        x = heroA->x;
        if (x < 0)
            x += 0xff;
        x >>= 8;
        y = heroA->y;
        if (y < 0)
            y += 0xff;
        y >>= 8;
        z = heroA->z;
        if (z < 0)
            z += 0xff;
        z >>= 8;
        effect_spawn_at(0x1E, x, y, z, heroA);
        if (HERO_MODE_ACTIVE(heroA->mode))
        {
            if (heroA->data->pose == -1)
            {
                sprite_set_action(heroA, 8, 0x2000, 0);
            }
            else
            {
                sprite_set_action(heroA, 8, 0x204D, 0);
            }
            ((struct SpriteModeByte *)&heroA->sprite->flags)->mode = 1;
        }
        body->state = (u32)hero_pair_place_both_rebuild_alt;
    }
}

/* Waits for hero A's animation to end, then sends hero B and then hero A back
   to their home targets, puts them in their idle pose facing along their span
   and hands each one over to the finish-rebuild state. */
void hero_pair_place_both_rebuild_alt(void)
{
    struct Body *body_a;
    struct Body *body_b;
    struct BodyData *data;
    s16 *span;
    s32 fixed;
    s32 home_x;
    s32 home_y;
    s32 home_z;

    body_a = &gWorld->heroA->body;
    body_b = &gWorld->heroB->body;
    if (body_a->sprite->flags & SPRITE_FLAG_ANIM_DONE)
    {
        set_slot_value(0x53, -1);
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            /* home position, 24.8 fixed point to whole units (rounding toward zero) */
            data = body_b->data;
            fixed = data->targetX;
            if (fixed < 0)
                fixed += 0xff;
            home_x = fixed >> 8;
            fixed = data->targetY;
            if (fixed < 0)
                fixed += 0xff;
            home_y = fixed >> 8;
            fixed = data->targetZ;
            if (fixed < 0)
                fixed += 0xff;
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
            /* face along the span: mirrored when its start is not before its end */
            ((struct SpriteMirrorByte *)&body_b->sprite->unk11)->mirror = (span = &body_b->spanA)[0] >= span[1];
            ((struct SpriteModeByte *)&body_b->sprite->flags)->mode = 0;
        }
        if (HERO_MODE_ACTIVE(body_b->mode))
        {
            body_b->state = 0;
            enter_finish_rebuild(body_b);
        }
        if (HERO_MODE_ACTIVE(body_a->mode))
        {
            data = body_a->data;
            fixed = data->targetX;
            if (fixed < 0)
                fixed += 0xff;
            home_x = fixed >> 8;
            fixed = data->targetY;
            if (fixed < 0)
                fixed += 0xff;
            home_y = fixed >> 8;
            fixed = data->targetZ;
            if (fixed < 0)
                fixed += 0xff;
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
            ((struct SpriteMirrorByte *)&body_a->sprite->unk11)->mirror = (span = &body_a->spanA)[0] >= span[1];
            ((struct SpriteModeByte *)&body_a->sprite->flags)->mode = 0;
        }
        if (HERO_MODE_ACTIVE(body_a->mode))
        {
            body_a->state = 0;
            enter_finish_rebuild(body_a);
        }
    }
}

/* Spawns an effect at the body's position (effect 0x3002 for sprite animations
 * 3, 5, 7, 9 and 11, effect 0 otherwise). If the body is on the field, aims its
 * motion at the partner's target point (minus 8 in X and Z), then continues
 * with body_link_partner_fx_slot9d, which moves it there. */
void body_fx_then_place_on_partner(struct Body *body)
{
    struct Body *partner = body->partner;

    switch (body->sprite->anim)
    {
    case 3:
        effect_spawn_at(0x3002, body->x / 256, body->y / 256, body->z / 256, body);
        break;
    case 5:
        effect_spawn_at(0x3002, body->x / 256, body->y / 256, body->z / 256, body);
        break;
    case 7:
        effect_spawn_at(0x3002, body->x / 256, body->y / 256, body->z / 256, body);
        break;
    case 9:
        effect_spawn_at(0x3002, body->x / 256, body->y / 256, body->z / 256, body);
        break;
    case 11:
        effect_spawn_at(0x3002, body->x / 256, body->y / 256, body->z / 256, body);
        break;
    default:
        effect_spawn_at(0, body->x / 256, body->y / 256, body->z / 256, body);
        break;
    }
    if (HERO_MODE_ACTIVE(body->mode))
    {
        place_targets(body, partner->targetX / 256 + partner->ofsX - 8, partner->targetY / 256 + partner->ofsY,
                      partner->targetZ / 256 + partner->ofsZ - 8, -1);
        set_motion_speed_aux(body, 0x500, 0x33);
    }
    body->state = (u32)body_link_partner_fx_slot9d;
}

/*
 * Moves the body (scaled motion) towards its partner. On arrival (flag 0x20) it
 * unlinks the partner, sets slot 0x9D, resolves the contact and, if the partner
 * is still shown, spawns an effect at the partner's target point: one effect id
 * per sprite animation 3, 5, 7, 9 and 11. When the animation ends, the body
 * finishes.
 */
void body_link_partner_fx_slot9d(struct Body *body)
{
    struct Body *partner;
    s32 fxY;
    s32 fxX;

    partner = body->partner;
    step_motion_scaled(body);
    if (body->flags & 0x20)
    {
        ((struct DataConnectBits *)&partner->data->flags111)->connect = 0;
        set_slot_value(0x9D, -1);
        ((struct CcBits *)&body->cc)->lo7 = 0x5F;
        body->scale = 0x100;
        resolve_contact_action(body, partner, 0, 0, 0x0423458C);
        body->count++;
        if (partner->data->displayTimer > 0 || (partner->data->flags111 & 2))
        {
            /* effect position: the partner's target (8.8 fixed point) plus its offset */
            switch (body->sprite->anim)
            {
            case 3:
                fxX = partner->targetX;
                if (fxX < 0)
                    fxX += 0xff;
                fxX >>= 8;
                fxX += partner->ofsX;
                fxY = partner->targetY;
                if (fxY < 0)
                    fxY += 0xff;
                fxY >>= 8;
                fxY += partner->ofsY;
                effect_spawn_at(0x2EAC, fxX, fxY, 0, partner);
                break;
            case 5:
                fxX = partner->targetX;
                if (fxX < 0)
                    fxX += 0xff;
                fxX >>= 8;
                fxX += partner->ofsX;
                fxY = partner->targetY;
                if (fxY < 0)
                    fxY += 0xff;
                fxY >>= 8;
                fxY += partner->ofsY;
                effect_spawn_at(0x2EC5, fxX, fxY, 0, partner);
                break;
            case 7:
                fxX = partner->targetX;
                if (fxX < 0)
                    fxX += 0xff;
                fxX >>= 8;
                fxX += partner->ofsX;
                fxY = partner->targetY;
                if (fxY < 0)
                    fxY += 0xff;
                fxY >>= 8;
                fxY += partner->ofsY;
                effect_spawn_at(0x2EE3, fxX, fxY, 0, partner);
                break;
            case 9:
                fxX = partner->targetX;
                if (fxX < 0)
                    fxX += 0xff;
                fxX >>= 8;
                fxX += partner->ofsX;
                fxY = partner->targetY;
                if (fxY < 0)
                    fxY += 0xff;
                fxY >>= 8;
                fxY += partner->ofsY;
                effect_spawn_at(0x2F01, fxX, fxY, 0, partner);
                break;
            case 11:
                fxX = partner->targetX;
                if (fxX < 0)
                    fxX += 0xff;
                fxX >>= 8;
                fxX += partner->ofsX;
                fxY = partner->targetY;
                if (fxY < 0)
                    fxY += 0xff;
                fxY >>= 8;
                fxY += partner->ofsY;
                effect_spawn_at(0x2F1F, fxX, fxY, 0, partner);
                break;
            default:
                fxX = partner->targetX;
                if (fxX < 0)
                    fxX += 0xff;
                fxX >>= 8;
                fxX += partner->ofsX;
                fxY = partner->targetY;
                if (fxY < 0)
                    fxY += 0xff;
                fxY >>= 8;
                fxY += partner->ofsY;
                effect_spawn_at(0, fxX, fxY, 0, partner);
                break;
            }
        }
        if ((body->mode & 6) == 2 || (body->mode & 6) == 4)
        {
            body->state = (u32)body_finish_object;
        }
    }
}

void body_finish_object(struct Body *body)
{
    obj_release(body);
}

void hero_a_child_remove(void)
{
    obj_release(gWorld->heroA->body.child);
}

void body_advance_end_if_active(struct Body *body)
{
    state_stack_advance_progress(body);
    if (body->flags & 0x20)
    {
        body->state = 0;
    }
}

void body_anim_next_then_end(struct Body *body)
{
    struct SpriteEntry *sprite;

    sprite = body->sprite;
    if (sprite->flags & 8)
    {
        sprite_set_action(body, sprite->anim + 1, 0, 0);
        ((struct SpriteModeByte *)&body->sprite->flags)->mode = 0;
        body->state = 0;
    }
}

void body_step_if_near_hero_a(struct Body *body)
{
    struct Body *hbody;
    s32 ax;

    hbody = &gWorld->heroA->body;
    ax = hbody->x;
    if (body->x < ax - 0x800
        || (hbody->z + (hbody->ofsZ << 8) < body->z && ax + 0x800 < body->x))
    {
        step_motion(body);
        return;
    }
    set_slot_value(0x9C, -1);
    body->state = (u32)hero_body_blend_to_a;
}
