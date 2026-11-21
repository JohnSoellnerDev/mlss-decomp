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

/* A walk's X range in pixels, as place_targets packs it into the body at 0x84. */
struct HeroSpan
{
    s16 a; /* start X (where the body was) */
    s16 b; /* target X */
};

struct PoseFlagBits
{
    u16 unk0 : 5;
    u16 field5 : 5;
    u16 unk10 : 6;
};

struct PoseRecord
{
    u8 filler0[0x46];
    u8 byte46;
    u8 filler47;
    struct PoseFlagBits word48;
};

struct HeroDataFlagBits
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
};

/* Body mode bits 1-2: 2 or 4 means the body is active on the field. */
#define HERO_MODE_ACTIVE(mode) (((mode) & 6) == 2 || ((mode) & 6) == 4)

/* HeroBody.timer bits 0-11 count how often the current move has repeated; the upper bits are flags. */
#define HERO_TIMER_COUNT_MASK 0xFFF

extern struct World *gWorld;
extern u8 *gParty;
extern u8 gSys[];
/* (x, y) jitter of hero A for the first repeats, indexed by the repeat count. */
extern s8 gJitterTable1[];
/* Box for the later repeats' random jitter: two corners (x0, y0, x1, y1). */
extern s8 gJitterRange1[];

u32 mt19937_next(void);
u32 rand_below(u32 range, u32 rnd);
void hero_a_step_window_c491c(struct HeroBody *body);
void hero_a_jitter_step_5ca4(struct HeroBody *body);
void hero_pair_place_and_finish_alt3(struct HeroBody *body);
void step_motion(struct HeroBody *body);
void set_slot_value(u32 id, s32 value);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 z, struct HeroBody *owner);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void set_motion_offsets(struct HeroBody *body, u32 offsets);
void set_motion_start(struct HeroBody *body, u32 speed);
void enter_finish_rebuild(struct HeroBody *body);
u32 find_frame_entry(struct HeroBody *obj, u32 index);

/* Spawns effect 0x2FE6 at hero A and sends hero A jumping (speed 0x600, pose 0x2034) to just beside and
   below the partner's top; the jump's height is the partner's top height plus 0x90 / (h * h / 0x40 + 12).
   Clears hero A's flag bits above the repeat count, then goes on to hero_a_step_window_c491c. */
void hero_a_place_on_partner_offset(struct HeroBody *body)
{
    struct Hero *hero_a = gWorld->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct HeroBody *partner = body->partner;
    s32 height;

    effect_spawn_at(0x2FE6, body_a->posX / 0x100, body_a->posY / 0x100, body_a->posZ / 0x100 + 8, body_a);
    if (HERO_MODE_ACTIVE(body_a->mode))
    {
        place_targets(body_a,
                      (partner->targetX / 0x100) + partner->ofsX - 10,
                      (partner->targetY / 0x100) + partner->ofsY + 3,
                      (partner->targetZ / 0x100) + partner->ofsZ - 0x15,
                      -1);
        /* the partner's own top: its offset when it is its data's main body, else its frame's */
        if (partner == (struct HeroBody *)((u8 *)partner->data + 8))
        {
            height = partner->ofsZ;
        }
        else
        {
            find_frame_entry(partner, 0);
            height = partner->unk47;
        }
        height += partner->targetZ / 0x100;
        height += MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(0x90, (height * height) / 0x40 + 0xc);
        body_a->height = height;
        set_motion_offsets(body_a, 0x600);
    }
    if (HERO_MODE_ACTIVE(body_a->mode))
    {
        sprite_set_action(body_a, 4, 0x2034, 0);
        ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
    }
    body_a->timer &= HERO_TIMER_COUNT_MASK;
    body->state = hero_a_step_window_c491c;
}

ASM_FUNC("asm/code_080C47AC/hero_a_step_window_c491c.s", void hero_a_step_window_c491c(struct HeroBody *body))

ASM_FUNC("asm/code_080C47AC/hero_a_step_pose_c5054.s", void hero_a_step_pose_c5054(void))

/* Attack step: runs hero A's motion; once it has arrived, plays sound 0x2E, spawns effect 0x1E at
 * hero A, starts its action 7 (art 0x2034) if it can still act, and moves on to
 * hero_pair_place_and_finish_alt3. */
void hero_a_active_fx_anim_ready_b(struct HeroBody *body)
{
    struct Hero *hero = gWorld->heroA;
    struct HeroBody *body_a = &hero->body;
    s32 x;
    s32 y;
    s32 z;

    step_motion(body_a);
    if ((hero->body.flags & 0x20) != 0)
    {
        set_slot_value(0x2E, -1);
        x = body_a->posX / 0x100;
        y = body_a->posY / 0x100;
        z = body_a->posZ / 0x100;
        effect_spawn_at(0x1E, x, y, z, body_a);
        if (HERO_MODE_ACTIVE(body_a->mode))
        {
            sprite_set_action(body_a, 7, 0x2034, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
        body->state = hero_pair_place_and_finish_alt3;
    }
}

/*
 * State handler: once hero A's sprite animation has finished, send both heroes (A first,
 * then B) walking back to the home positions stored in their HeroData at speed 0x400.
 * Each hero on the field gets the walk action (0x204F if it has a pose, else 0x2002),
 * faces the way it walks, and then hands over to the finish-rebuild state.
 */
void hero_pair_place_and_finish_alt3(struct HeroBody *body)
{
    struct Hero *hero_a = gWorld->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct Hero *hero_b = gWorld->heroB;
    struct HeroBody *body_b = &hero_b->body;
    struct HeroData *home;
    struct HeroSprite *sprite;
    struct HeroSpan *walk;
    s32 face_left;
    s32 x;
    s32 y;
    s32 h;

    if ((body_a->sprite->flags & 8) != 0)
    {
        if ((hero_a->body.mode & 6) == 2 || (hero_a->body.mode & 6) == 4)
        {
            home = body_a->data;
            x = home->targetX / 0x100;
            y = home->targetY / 0x100;
            h = home->targetH / 0x100;
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
            face_left = 0;
            walk = &body_a->span;
            if (walk->a >= walk->b)
                face_left = 1;
            ((struct HeroMirrorBits *)&sprite->mirror)->flip = face_left;
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            body_a->state = 0;
            enter_finish_rebuild(body_a);
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            home = body_b->data;
            x = home->targetX / 0x100;
            y = home->targetY / 0x100;
            h = home->targetH / 0x100;
            place_targets(body_b, x, y, h, -1);
            set_motion_start(body_b, 0x400);
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            if (body_b->data->poseIndex == -1)
                sprite_set_action(body_b, 0, 0x2002, 0);
            else
                sprite_set_action(body_b, 0, 0x204F, 0);
            sprite = body_b->sprite;
            face_left = 0;
            walk = &body_b->span;
            if (walk->a >= walk->b)
                face_left = 1;
            ((struct HeroMirrorBits *)&sprite->mirror)->flip = face_left;
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 0;
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            body_b->state = 0;
            enter_finish_rebuild(body_b);
        }
    }
}

/* Gives hero A its jitter for this repeat (from gJitterTable1 for the first four, random within
   gJitterRange1 after that), spawns effect 0x2FE6 at hero A, moves hero A to just beside and below
   the partner's top and gives it pose 0x2034, then goes on to hero_a_jitter_step_5ca4. */
void hero_a_jitter_place_near_partner(struct HeroBody *body)
{
    struct Hero *hero_a = gWorld->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct HeroBody *partner = body->partner;
    s32 repeat;
    s32 jitter_x;
    s32 jitter_y;

    repeat = body_a->timer & HERO_TIMER_COUNT_MASK;
    if (repeat <= 3)
    {
        hero_a->body.jitterX = gJitterTable1[repeat * 2];
        hero_a->body.jitterY = gJitterTable1[(body_a->timer & HERO_TIMER_COUNT_MASK) * 2 + 1];
    }
    else
    {
        s8 *box = gJitterRange1;

        if (box[0] < box[2])
            jitter_x = box[0] + rand_below(box[2] - box[0], mt19937_next());
        else
            jitter_x = box[2] + rand_below(box[0] - box[2], mt19937_next());
        hero_a->body.jitterX = jitter_x;

        if (box[1] < box[3])
            jitter_y = box[1] + rand_below(box[3] - box[1], mt19937_next());
        else
            jitter_y = box[3] + rand_below(box[1] - box[3], mt19937_next());
        body_a->jitterY = jitter_y;
    }
    effect_spawn_at(0x2FE6, body_a->posX / 0x100, body_a->posY / 0x100, body_a->posZ / 0x100 + 8, body_a);
    if (HERO_MODE_ACTIVE(body_a->mode))
    {
        place_targets(body_a,
                      (partner->targetX / 0x100) + partner->ofsX - 10,
                      (partner->targetY / 0x100) + partner->ofsY + 3,
                      (partner->targetZ / 0x100) + partner->ofsZ - 0x15,
                      -1);
        set_motion_start(body_a, 0x600);
    }
    if (HERO_MODE_ACTIVE(body_a->mode))
    {
        sprite_set_action(body_a, 4, 0x2034, 0);
        ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
    }
    body_a->timer = body_a->timer & HERO_TIMER_COUNT_MASK;
    body->state = hero_a_jitter_step_5ca4;
}

ASM_FUNC("asm/code_080C47AC/hero_a_jitter_step_5ca4.s", void hero_a_jitter_step_5ca4(struct HeroBody *body))
