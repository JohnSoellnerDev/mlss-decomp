#include "global.h"
#include "ram_routines.h"

struct HeroSprite
{
    u16 unk0;
    u16 unk2;
    u8 filler4[0x0E - 0x04];
    u16 unkE;
    u8 filler10[0x11 - 0x10];
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

struct HeroCcBits
{
    u8 low : 7;
    u8 bit7 : 1;
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
    u8 filler118[0x11E - 0x118];
    u16 flags;
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
    struct HeroBody *next;
    struct HeroNode *node;
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
    s16 spanA; /* the current jump's start and end X in pixels (step_motion_checked) */
    s16 spanB;
    u8 filler88[0x92 - 0x88];
    s16 height;
    u8 filler94[0xAC - 0x94];
    s16 step;
    u16 timer;
    u8 filler_B0[0xC3 - 0xB0];
    s8 ofsX;
    s8 ofsY;
    s8 ofsZ;
    u8 filler_C6[0xCA - 0xC6];
    u16 scale;
    u8 cc;
};

/* HeroSprite.flags: its animation has finished. */
#define HERO_SPRITE_ANIM_DONE 8

/* HeroBody.mode bits 1-2 say where it is: 2 or 4 = on the field. */
#define HERO_ON_FIELD(mode) (((mode) & 6) == 2 || ((mode) & 6) == 4)

struct Hero
{
    u8 filler0[0x08];
    struct HeroBody body;
    u8 filler_D8[0x110 - 0xD8];
    u8 flags110;
};

struct World
{
    u8 filler0[0x0A];
    u8 mode;
    u8 filler0B[0x0C - 0x0B];
    u8 flags;
    u8 filler0D[0x0E - 0x0D];
    u8 unk0E;
    u8 filler0F[0x58 - 0x0F];
    struct Hero *list[6];
    struct Hero *heroA;
    struct Hero *heroB;
};

extern struct World *gWorld;
extern u8 *gParty;

void state_stack_advance_progress(struct HeroBody *body);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void step_motion(struct HeroBody *body);
void set_slot_value(u32 id, s32 value);
void set_motion_offsets(struct HeroBody *body, u32 offsets);
void set_motion_start(struct HeroBody *body, u32 speed);
void step_motion_checked(struct HeroBody *body);
void enter_finish_rebuild(struct HeroBody *body);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void schedule_pose_task(struct HeroBody *partner);
void hero_a_stop_anim_if_active(struct HeroBody *body);
void hero_b_stop_anim_if_active(struct HeroBody *body);
void hero_b_anim_on_sprite(struct HeroBody *body);
void hero_b_begin_step_16(struct HeroBody *body);
void hero_a_go_to_b_target(struct HeroBody *body);
void hero_a_active_anim_24(struct HeroBody *body);
void hero_pair_place_mirror(struct HeroBody *body);
void hero_a_move_ba294(struct HeroBody *body);
void hero_a_place_at_target(struct HeroBody *body);
void hero_a_pair_finish(void);
void hero_a_approach_91e0(struct HeroBody *body);
void hero_a_step_slot_anim(struct HeroBody *body);
void hero_b_step_slot_anim(struct HeroBody *body);
u32 mt19937_next(void);
s32 rand_below(s32 range, u32 seed);
u32 find_frame_entry(struct HeroBody *obj, u32 index);

/* State: once hero B's animation has finished, and if it is on the field, gives it action 0xB with
   animation 0x204D and sets up its jump home: targets its data's position (in pixels), with an arc
   32 pixels above the target's height (motion offsets 0x700). Continues in hero_b_step_slot_anim. */
void hero_b_place_at_target(struct HeroBody *body)
{
    struct Hero *hero = gWorld->heroB;
    struct HeroBody *heroB = &hero->body;
    struct HeroData *data;
    s32 homeX;
    s32 homeY;
    s32 homeZ;
    s32 t;

    if ((heroB->sprite->flags & HERO_SPRITE_ANIM_DONE) != 0)
    {
        if (HERO_ON_FIELD(hero->body.mode))
        {
            sprite_set_action(heroB, 0xB, 0x204D, 0);
            ((struct HeroSpriteBits *)&heroB->sprite->flags)->mode = 1;
        }
        if (HERO_ON_FIELD(heroB->mode))
        {
            data = heroB->data;
            t = data->targetX;
            if (t < 0)
                t += 0xFF;
            homeX = t >> 8;
            t = data->targetY;
            if (t < 0)
                t += 0xFF;
            homeY = t >> 8;
            t = data->targetH;
            if (t < 0)
                t += 0xFF;
            homeZ = t >> 8;
            place_targets(heroB, homeX, homeY, homeZ, -1);
            t = heroB->targetZ;
            if (t < 0)
                t += 0xFF;
            heroB->height = (t >> 8) + 0x20;
            set_motion_offsets(heroB, 0x700);
        }
        body->state = hero_b_step_slot_anim;
    }
}

/*
 * Steps hero B's jump. When it lands (flag 0x20, set by step_motion_checked) it plays sound 0x53
 * and, if hero B is on the field (mode 2 or 4), jumps back to its home position (its data's target)
 * at speed 0x400, with action 0 and animation 0x2002 (0x204F when its data has a pose), facing the
 * way it jumps; then it clears its state and rebuilds.
 */
void hero_b_pair_finish(void)
{
    struct Hero *hero = gWorld->heroB;
    struct HeroBody *body = &hero->body;
    struct HeroData *data;
    struct HeroSprite *sprite;
    u8 *mode;
    s16 *jumpX;
    s32 mirror;
    s32 homeX;
    s32 homeY;
    s32 homeZ;
    s32 t;

    step_motion_checked(body);
    if ((hero->body.flags & 0x20) != 0)
    {
        set_slot_value(0x53, -1);
        if ((hero->body.mode & 6) == 2 || (hero->body.mode & 6) == 4)
        {
            data = body->data;
            t = data->targetX;
            if (t < 0)
                t += 0xFF;
            homeX = t >> 8;
            t = data->targetY;
            if (t < 0)
                t += 0xFF;
            homeY = t >> 8;
            t = data->targetH;
            if (t < 0)
                t += 0xFF;
            homeZ = t >> 8;
            place_targets(body, homeX, homeY, homeZ, -1);
            set_motion_start(body, 0x400);
        }
        if ((body->mode & 6) == 2 || (body->mode & 6) == 4)
        {
            if (body->data->pose == -1)
                sprite_set_action(body, 0, 0x2002, 0);
            else
                sprite_set_action(body, 0, 0x204F, 0);
            sprite = body->sprite;
            /* face left unless the jump goes right */
            mirror = 0;
            jumpX = &body->spanA;
            if (!(jumpX[0] < jumpX[1]))
                mirror = 1;
            ((struct HeroMirrorBits *)&sprite->mirrorFlags)->mirror = mirror;
            ((struct HeroSpriteBits *)&body->sprite->flags)->mode = 0;
        }
        mode = &body->mode;
        if ((*mode & 6) == 2 || (*mode & 6) == 4)
        {
            body->state = 0;
            enter_finish_rebuild(body);
        }
    }
}

/*
 * Picks a new target for hero A: one of the six enemy slots, chosen uniformly at random
 * (reservoir sampling) among those on the field (mode 4), not flagged 0x20 and not the
 * current target's owner. Plays sound 0x2B (set_slot_value queues the song), then starts
 * hero A's jump onto the target: lands on its position plus its offsets, and the arc's
 * height grows with the target's height. Continues in hero_a_approach_91e0.
 */
void hero_a_pick_partner(struct HeroBody *body)
{
    struct HeroBody *hero_a = &gWorld->heroA->body;
    struct HeroBody *target = body->partner;
    struct World *world;
    struct Hero *enemy;
    u8 *mode;
    s8 *ofs_z;
    u32 candidates = 0;
    s32 index;
    s32 next;
    s32 x;
    s32 y;
    s32 z;
    s32 height;
    s32 t;

    for (index = 0; index < 6; index += 2)
    {
        enemy = gWorld->list[index];
        if ((enemy->body.mode & 6) == 4 && (enemy->flags110 & 0x20) == 0
            && enemy != (struct Hero *)body->partner->data)
        {
            candidates++;
            if (rand_below(candidates, mt19937_next()) == 0)
                target = &enemy->body;
        }
        next = index + 1;
        enemy = gWorld->list[next];
        if ((enemy->body.mode & 6) == 4 && (enemy->flags110 & 0x20) == 0
            && enemy != (struct Hero *)body->partner->data)
        {
            candidates++;
            if (rand_below(candidates, mt19937_next()) == 0)
                target = &enemy->body;
        }
    }
    body->partner = target;
    set_slot_value(0x2B, -1);
    if ((hero_a->mode & 6) == 2 || (hero_a->mode & 6) == 4)
    {
        sprite_set_action(hero_a, 0xC, 0x2000, 0);
        ((struct HeroSpriteBits *)&hero_a->sprite->flags)->mode = 0;
    }
    mode = &hero_a->mode;
    if ((*mode & 6) == 2 || (*mode & 6) == 4)
    {
        {
            s32 tx = target->targetX;
            if (tx < 0)
                tx += 0xFF;
            x = (tx >> 8) + target->ofsX;
        }
        t = target->targetY;
        if (t < 0)
            t += 0xFF;
        y = (t >> 8) + target->ofsY;
        z = target->targetZ;
        if (z < 0)
            z += 0xFF;
        z >>= 8;
        ofs_z = &target->ofsZ;
        z += *ofs_z;
        place_targets(hero_a, x, y, z, -1);
        /* the target's own height: its Z offset if it is its owner's main body, else its frame's */
        if (target == (struct HeroBody *)((u8 *)target->data + 8))
        {
            height = *ofs_z;
        }
        else
        {
            find_frame_entry(target, 0);
            height = target->unk47;
        }
        t = target->targetZ;
        if (t < 0)
            t += 0xFF;
        height += t >> 8;
        t = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x6E4, (height * height) / 64 + 0x2A);
        height += t;
        hero_a->height = height;
        set_motion_offsets(hero_a, 0x500);
    }
    hero_a->timer &= 0xFFF;
    body->state = hero_a_approach_91e0;
}

ASM_FUNC("asm/code_080B8E10/hero_a_approach_91e0.s", void hero_a_approach_91e0(struct HeroBody *body))
