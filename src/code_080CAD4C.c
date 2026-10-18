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
    s32 unk9C;
    u32 link;
    u8 filler0A4[0xAC - 0xA4];
    s16 step;
    u16 timer;
    s16 jitterY;
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

void hero_pair_z_step_place_partner(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct HeroBody *body_a = &world->heroA->body;
    struct HeroBody *body_b = &world->heroB->body;
    struct HeroBody *partner = body->partner;
    u16 scale = body_a->sprite->scale;

    if ((s16)body_a->sprite->scale <= 0xFF)
    {
        body_a->sprite->scale = scale + 0x10;
        body_b->posZ += 0x100;
        if ((s16)body_a->sprite->scale > 0x100)
        {
            body_a->sprite->scale = 0x100;
            body_b->posZ = 0;
        }
    }
    else
    {
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            s32 x = partner->targetX / 0x100 + partner->ofsX;
            s32 y = partner->targetY / 0x100 + partner->ofsY;
            place_targets(body_a, x, y, 0, -1);
            set_motion_speed_aux(body_a, 0, 0x33);
        }
        set_slot_value(0x8C, -1);
        body->state = hero_pair_follow_ae14;
    }
}

ASM_FUNC("asm/code_080CAD4C/hero_pair_follow_ae14.s", void hero_pair_follow_ae14(struct HeroBody *body))

void hero_a_jitter_apply_partner(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct HeroBody *body_a = &world->heroA->body;
    struct HeroBody *body_b = &world->heroB->body;

    if (body_a->link != 0)
    {
        set_body_part(body_b, body_a->link, 0, -0x30, 0);
    }
    body_a->timer = (s16)body_a->timer * 0xE6 / 0x100;
    body_a->jitterY = body_a->jitterY * 0xE6 / 0x100;
    body_a->posX += (s16)body_a->timer;
    body_b->posX = body_a->posX;
    body_a->posY += body_a->jitterY;
    body_b->posY = body_a->posY;
    if (body_a->link != 0)
    {
        hero_pair_find_partner(body_b);
        if ((gSys.held & 0xF3) != 0)
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


void hero_a_jitter_dx_step_anim(struct HeroBody *body)
{
    struct World *world = gWorld;
    struct HeroBody *body_a = &world->heroA->body;
    struct HeroBody *body_b = &world->heroB->body;
    struct HeroBody *partner = body->partner;
    s32 target_a = body_a->targetX + 0x3C00;
    s32 leftover = (partner->targetX - target_a) * 25 / 0x100;

    body_a->timer = (s16)body_a->timer * 0xE6 / 0x100;
    if (leftover < (s16)body_a->timer)
    {
        body_a->timer = ((s16)body_a->timer + leftover) / 2;
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
            body_b->rate = MATH_ROUTINE(u32 (*)(s32, s32), iwram_div)(0x4000, 0xE);
            body_b->progress = 0;
            ((struct HeroFlagBits *)&body_b->flags)->active = 0;
        }
        body_b->timer = body_b->timer & 0xFFF;
        body->state = hero_b_step_window_cb508;
    }
}


ASM_FUNC("asm/code_080CAD4C/hero_b_step_window_cb508.s", void hero_b_step_window_cb508(struct HeroBody *body))

ASM_FUNC("asm/code_080CAD4C/hero_b_step_in_range_pose_fx.s", void hero_b_step_in_range_pose_fx(void))
