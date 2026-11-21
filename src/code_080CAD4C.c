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

struct HeroRange
{
    u8 filler0[0x86];
    s16 lo;
    u8 filler88[0x9C - 0x88];
    u32 pairLo;
    u32 pairHi;
    u8 filler0A4[0xB4 - 0xA4];
    s16 hi;
};

struct HeroExt
{
    u8 filler0[0x04];
    s8 ofsA;
    s8 ofsB;
};

struct HeroData
{
    u8 filler0[0xD8];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    struct HeroExt *ext;
    u8 filler0E8[0xEC - 0xE8];
    s16 poseIndex;
    u8 filler0EE[0xF6 - 0xEE];
    s16 flagsF6;
    u8 filler0F8[0x110 - 0xF8];
    u8 flags110;
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
    u32 unk94;
    u32 unk98;
    s32 unk9C;      /* hero A in the pair slides: frames left before it ends */
    u32 link;       /* ... and the cursor art shown over hero B while it aims (0: none) */
    u8 filler0A4[0xAC - 0xA4];
    s16 step;
    u16 timer;      /* hero A in the pair slides: X velocity (s16, 1/256 px a frame) */
    s16 jitterY;    /* ... and Y velocity */
    u8 filler0B2[0xC3 - 0xB2];
    s8 ofsX;
    s8 ofsY;
    s8 ofsZ;
    u8 filler0C6[0xCA - 0xC6];
    u16 scale;
    u8 cc;
    u8 filler0CD[0xD0 - 0xCD];
};

struct Hero
{
    u8 filler0[0x08];
    struct HeroBody body;
    u8 filler0D8[0xE4 - 0xD8];
    struct HeroExt *ext;
    u8 filler0E8[0x110 - 0xE8];
    u8 flags110;
};

struct World
{
    u8 filler0[0x58];
    struct Hero *objList[6];
    struct Hero *heroA;
    struct Hero *heroB;
};

struct KeyState
{
    u8 filler0[0x3C];
    u16 held;
};

extern struct World *gWorld;
extern struct KeyState gSys;

void set_slot_value(u32 id, s32 value);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void set_motion_speed_aux(struct HeroBody *body, u32 speed, u32 aux);
void hero_pair_follow_ae14(struct HeroBody *body);
void state_stack_advance_progress(struct HeroBody *body);
void set_motion_offsets(struct HeroBody *body, u32 offsets);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void hero_pair_anim_a_wait(struct HeroBody *body);
void hero_pair_find_partner(struct HeroBody *body);
void hero_b_step_window_cb508(struct HeroBody *body);
void hero_b_sprite_active_place_finish(struct HeroBody *body);
u32 mt19937_next(void);
u32 rand_below(u32 range, u32 rnd);
void step_motion(struct HeroBody *body);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 h, struct HeroBody *owner);
void enter_finish_rebuild(struct HeroBody *body);
void set_motion_start(struct HeroBody *body, u32 speed);
void hero_pair_z_step_place_partner(struct HeroBody *body);
void set_body_part(struct HeroBody *body, u32 art, s32 x, s32 y, u32 extra);
void obj_free_parts(struct HeroBody *body);
void display_flush(void);

/*
 * State: hero A's sprite grows back to full size (scale 0x100) by 0x10 a frame while hero B rises
 * a pixel a frame; once A is full size, B is back at Z 0. Then, in body modes 2 and 4, A starts
 * moving (from rest, accelerating by 0x33) to the partner's target plus its offsets, sound 0x8C
 * plays (sound_play: set_slot_value queues the song) and the state goes on to hero_pair_follow_ae14.
 */
void hero_pair_z_step_place_partner(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct HeroBody *heroA = &world->heroA->body;
    struct HeroBody *heroB = &world->heroB->body;
    struct HeroBody *partner = body->partner;
    u16 scale = heroA->sprite->scale;

    if ((s16)heroA->sprite->scale <= 0xFF)
    {
        heroA->sprite->scale = scale + 0x10;
        heroB->posZ += 0x100; /* 1 pixel */
        if ((s16)heroA->sprite->scale > 0x100)
        {
            heroA->sprite->scale = 0x100;
            heroB->posZ = 0;
        }
    }
    else
    {
        if ((heroA->mode & 6) == 2 || (heroA->mode & 6) == 4)
        {
            s32 destX = partner->targetX / 0x100 + partner->ofsX;
            s32 destY = partner->targetY / 0x100 + partner->ofsY;
            place_targets(heroA, destX, destY, 0, -1);
            set_motion_speed_aux(heroA, 0, 0x33);
        }
        set_slot_value(0x8C, -1); /* play sound 0x8C at the current volume */
        body->state = hero_pair_follow_ae14;
    }
}

ASM_FUNC("asm/code_080CAD4C/hero_pair_follow_ae14.s", void hero_pair_follow_ae14(struct HeroBody *body))

/*
 * State: hero A slides with hero B on top of it, its velocity decaying to 90% a frame. While a
 * cursor is shown over B, B aims (hero_pair_find_partner); a press of A, B or the D-pad hides the
 * cursor and makes B's target A's. When the slide's frames run out the cursor goes and, in body
 * modes 2 and 4, the state goes on to hero_pair_z_step_place_partner.
 */
void hero_a_jitter_apply_partner(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct HeroBody *body_a = &world->heroA->body;
    struct HeroBody *body_b = &world->heroB->body;

    if (body_a->link != 0)
    {
        set_body_part(body_b, body_a->link, 0, -0x30, 0);
    }
    /* timer and jitterY are the slide's X and Y velocity */
    body_a->timer = (s16)body_a->timer * 0xE6 / 0x100;
    body_a->jitterY = body_a->jitterY * 0xE6 / 0x100;
    body_a->posX += (s16)body_a->timer;
    body_b->posX = body_a->posX;
    body_a->posY += body_a->jitterY;
    body_b->posY = body_a->posY;
    if (body_a->link != 0)
    {
        hero_pair_find_partner(body_b);
        if ((gSys.held & 0xF3) != 0) /* A, B or the D-pad */
        {
            body_a->link = 0;
            if (body_a->partner != body_b->partner)
            {
                body_a->partner = body_b->partner;
            }
            obj_free_parts(body_b);
            display_flush();
        }
    }
    if (body_a->unk9C > 0)
    {
        body_a->unk9C--;
    }
    else
    {
        obj_free_parts(body_b);
        display_flush();
        if ((body->mode & 6) == 2 || (body->mode & 6) == 4)
        {
            body->state = hero_pair_z_step_place_partner;
        }
    }
}


// Slides hero A, with hero B on top of it, along X: the velocity decays to 90% a frame, but
// while 10% of the distance from A's target + 0x3C pixels to the partner's target is further
// left, it moves half way there. Once A is down to under a pixel a frame it plays action 4
// (0x2035) on A and action 8 (0x2053) on B, restarts B's motion (rate 0x4000 / 14), keeps B's
// timer to 12 bits and goes on to hero_b_step_window_cb508. Actions only play in body modes 2
// and 4.
void hero_a_jitter_dx_step_anim(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct HeroBody *body_a = &world->heroA->body;
    struct HeroBody *body_b = &world->heroB->body;
    struct HeroBody *partner = body->partner;
    s32 stopX = body_a->targetX + 0x3C00;
    s32 pull = (partner->targetX - stopX) * 25 / 0x100;

    body_a->timer = (s16)body_a->timer * 0xE6 / 0x100;
    if (pull < (s16)body_a->timer)
    {
        body_a->timer = ((s16)body_a->timer + pull) / 2;
    }
    body_a->posX += (s16)body_a->timer;
    body_b->posX = body_a->posX;
    if ((s16)body_a->timer >= -256)
    {
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            sprite_set_action(body_a, 4, 0x2035, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            sprite_set_action(body_b, 8, 0x2053, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 0;
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            body_b->rate = MATH_ROUTINE(u32 (*)(s32, s32), iwram_div)(0x4000, 14);
            body_b->progress = 0;
            ((struct HeroFlagBits *)&body_b->flags)->active = 0;
        }
        body_b->timer = body_b->timer & 0xFFF;
        body->state = hero_b_step_window_cb508;
    }
}


ASM_FUNC("asm/code_080CAD4C/hero_b_step_window_cb508.s", void hero_b_step_window_cb508(struct HeroBody *body))

// Moves hero B on along its motion. Once the motion's progress reaches the point
// (unk98 << 14) / unk94 (with step not past it) it plays action 6; when the motion ends it
// plays sound 0x2E and action 8, spawns effect 0x33 at hero B and goes on to
// hero_b_sprite_active_place_finish. The actions are only played in body modes 2 and 4.
void hero_b_step_in_range_pose_fx(struct HeroBody *body)
{
    register struct Hero *heroB asm("r5");
    struct HeroBody *bodyB;
    s32 reached;
    register s32 point asm("r2");
    s32 progress;
    s32 whole;
    s32 part;

    heroB = gWorld->heroB;
    bodyB = &heroB->body;
    step_motion(bodyB);
    whole = heroB->body.unk94;
    part = heroB->body.unk98;
    point = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(part << 14, whole);
    if (heroB->body.step <= point)
    {
        register s16 *progressp asm("r0") = &heroB->body.progress;

        progress = *progressp;
        reached = 1;
        if (progress >= point)
            goto reached_done;
    }
    reached = 0;
reached_done:
    if (reached)
    {
        if ((bodyB->mode & 6) == 2 || (bodyB->mode & 6) == 4)
        {
            if (bodyB->data->poseIndex == -1)
            {
                sprite_set_action(bodyB, 6, 0x2000, 0);
            }
            else
            {
                sprite_set_action(bodyB, 6, 0x204D, 0);
            }
            ((struct HeroSpriteBits *)&bodyB->sprite->flags)->mode = 1;
        }
    }
    if (bodyB->flags & 0x20)
    {
        set_slot_value(0x2E, -1);
        if ((bodyB->mode & 6) == 2 || (bodyB->mode & 6) == 4)
        {
            if (bodyB->data->poseIndex == -1)
            {
                sprite_set_action(bodyB, 8, 0x2000, 0);
            }
            else
            {
                sprite_set_action(bodyB, 8, 0x204D, 0);
            }
            ((struct HeroSpriteBits *)&bodyB->sprite->flags)->mode = 1;
        }
        effect_spawn_at(0x33, bodyB->posX / 0x100, bodyB->posY / 0x100, bodyB->posZ / 0x100, bodyB);
        body->state = hero_b_sprite_active_place_finish;
    }
}
