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
    u8 filler30[0x38 - 0x30];
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

extern struct World *gUnk_03000FD8;
extern struct KeyState gUnk_0300034C;

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

ASM_FUNC("asm/code_080B1D44/hero_a_copy_partner_pos.s", void hero_a_copy_partner_pos(void))

ASM_FUNC("asm/code_080B1D44/hero_a_walk_toward_partner.s", void hero_a_walk_toward_partner(struct HeroBody *body))

void hero_a_drop_and_land_with_b(struct HeroBody *body)
{
    struct World *world = gUnk_03000FD8;
    struct HeroBody *body_a = &world->heroA->body;
    struct HeroBody *body_b = &world->heroB->body;
    s32 damped;

    if (body_a->link != 0)
    {
        if ((gUnk_0300034C.pressed & 3) != 0)
        {
            body_set_pose(6, body_b, 0x16, 0);
            body_a->link = 0;
        }
    }
    damped = body_a->vel * 217 - 0x4CCC;
    if (damped < 0)
        damped += 0xFF;
    body_a->vel = damped >> 8;
    body_a->posZ += body_a->vel;
    body_b->posZ = body_a->posZ;
    if (body_a->spanZ >= body_a->posZ)
    {
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            body_b->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0xB);
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

void hero_b_bind_partner_fx(struct HeroBody *body)
{
    struct World *world = gUnk_03000FD8;
    struct HeroBody *body_a = &world->heroA->body;
    struct Hero *hero_b = world->heroB;
    struct HeroBody *body_b = &hero_b->body;
    struct HeroBody *partner = body->partner;
    s32 x;
    s32 y;
    s32 z;

    if ((body_b->sprite->flags & 8) != 0)
    {
        set_slot_value(0x63, -1);
        ((struct HeroFlag111Bits *)&partner->data->flags111)->bit1 = 0;
        ((struct HeroCcBits *)&body_b->cc)->low = 0;
        body_b->scale = 0x100;
        resolve_contact_action(body_b, partner, 0, 0, 0x040308A8);
        body_b->count++;
        if (partner->data->flagsF6 > 0 || (partner->data->flags111 & 2) != 0)
        {
            x = partner->targetX / 0x100 + partner->ofsX;
            y = partner->targetY / 0x100 + partner->ofsY;
            z = partner->targetZ / 0x100 + partner->ofsZ;
            effect_spawn_at(0x70B, x, y, z, partner);
        }
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

ASM_FUNC("asm/code_080B1D44/hero_pair_scale_and_settle.s", void hero_pair_scale_and_settle(struct HeroBody *body))

void pair_end_place_both_b(struct HeroBody *body)
{
    struct World *world = gUnk_03000FD8;
    struct HeroBody *body_a = &world->heroA->body;
    struct Hero *hero_b = world->heroB;
    struct HeroBody *body_b = &hero_b->body;
    struct HeroData *data;
    struct HeroSprite *sprite;
    u8 *mode;
    s16 *span;
    s32 mirror;
    s32 x;
    s32 y;
    s32 z;
    s32 t;

    if ((body_b->sprite->flags & 8) != 0)
    {
        set_slot_value(0x53, -1);
        if ((hero_b->body.mode & 6) == 2 || (hero_b->body.mode & 6) == 4)
        {
            data = body_b->data;
            t = data->targetX;
            if (t < 0)
                t += 0xFF;
            x = t >> 8;
            t = data->targetY;
            if (t < 0)
                t += 0xFF;
            y = t >> 8;
            t = data->targetH;
            if (t < 0)
                t += 0xFF;
            z = t >> 8;
            place_targets(body_b, x, y, z, -1);
            set_motion_start(body_b, 0x400);
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
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
        if ((*mode & 6) == 2 || (*mode & 6) == 4)
        {
            body_b->state = 0;
            enter_finish_rebuild(body_b);
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            data = body_a->data;
            t = data->targetX;
            if (t < 0)
                t += 0xFF;
            x = t >> 8;
            t = data->targetY;
            if (t < 0)
                t += 0xFF;
            y = t >> 8;
            t = data->targetH;
            if (t < 0)
                t += 0xFF;
            z = t >> 8;
            place_targets(body_a, x, y, z, -1);
            set_motion_start(body_a, 0x400);
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
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
        if ((*mode & 6) == 2 || (*mode & 6) == 4)
        {
            body_a->state = 0;
            enter_finish_rebuild(body_a);
        }
    }
}

void hero_a_key_toggle_step(struct HeroBody *body)
{
    struct World *world = gUnk_03000FD8;
    struct Hero *hero_a = world->heroA;
    struct HeroBody *body_a = &hero_a->body;
    s32 rate;

    if ((gUnk_0300034C.pressed & body_a->pressMask) != 0)
    {
        body_a->pressCount++;
        body_a->pressMask ^= 3;
    }
    if ((body_a->sprite->flags & 8) != 0)
    {
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
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

ASM_FUNC("asm/code_080B1D44/hero_pair_arrive.s", void hero_pair_arrive(void))
