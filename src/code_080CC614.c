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
    u8 filler40[0x4C - 0x40];
    void (*state)(struct HeroBody *);
    u32 unk50;
    u8 filler54[0x6C - 0x54];
    void *anim;
    u8 filler70[0x75 - 0x70];
    u8 unk75;
    u8 mode;
    u8 filler77[0x79 - 0x77];
    u8 flags;
    u16 rate;
    u16 frame;
    s16 progress;
    u8 filler80[0x84 - 0x80];
    s16 spanA;
    s16 spanB;
    u32 spanY;
    u8 filler8C[0x9C - 0x8C];
    u32 unk9C;
    u32 unkA0;
    u8 filler0A4[0xAC - 0xA4];
    s16 step;
    u16 timer;
    u8 filler0B0[0xC3 - 0xB0];
    s8 ofsX;
    s8 ofsY;
    u8 filler0C5[0xCA - 0xC5];
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

struct HeroPoseFlagsB
{
    u8 low6 : 6;
    u8 high2 : 2;
};

extern struct World *gUnk_03000FD8;
extern u8 *gUnk_03000FF4;

void step_motion(struct HeroBody *body);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void set_slot_value(u32 id, s32 value);
void schedule_pose_task(struct HeroBody *obj);
void hero_b_step_rate_16_when_a_idle(struct HeroBody *body);
void hero_pair_finish_place_both(struct HeroBody *body);
void hero_a_sprite_active_place_finish(struct HeroBody *body);
void hero_pair_pose_b_mode(struct HeroBody *body);
void hero_a_spawn_child(struct HeroBody *body);
void hero_pair_anim_b_rate10(struct HeroBody *body);
void hero_a_child_anim_then_done(struct HeroBody *body);
void hero_a_press_pose_cdbd4(struct HeroBody *body);
void hero_pair_b_step_cc7a0(struct HeroBody *body);
void hero_pair_link_b_fx_rate20_alt(struct HeroBody *body);
void pair_scan_hero_pair_place_both_finish(struct HeroBody *body);
void pair_scan_hero_pair_place_both_finish_alt(struct HeroBody *body);
void resolve_contact_action(struct HeroBody *a, struct HeroBody *b, u32 c, u32 d, u32 e);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 h, struct HeroBody *owner);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
s32 obj_last_x(void *obj);
s32 obj_last_y(void *obj);
void hero_a_anim_reset_mirror(struct HeroBody *body);
void set_motion_start(struct HeroBody *body, u32 speed);
void enter_finish_rebuild(struct HeroBody *body);
struct HeroBody *obj_spawn_linked(struct HeroBody *parent, u32 kind, u32 value);
void state_stack_advance_progress(struct HeroBody *body);

void hero_pair_prepare_anim(struct HeroBody *body)
{
    struct HeroBody *body_a;
    struct HeroBody *body_b;
    u8 mode;

    mode = gUnk_03000FD8->modeByte;
    gUnk_03000FD8->modeByte = mode | 0xC0;
    body_a = &gUnk_03000FD8->heroA->body;
    body_b = &gUnk_03000FD8->heroB->body;
    body_a->unk75 = body_b->unk75 - 8;
    if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
    {
        sprite_set_action(body_a, 5, 0x2035, 0);
        ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
    }
    if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
    {
        sprite_set_action(body_b, 5, 0x2064, 0);
        ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 0;
    }
    body->state = hero_pair_anim_b_rate10;
}

void hero_pair_anim_b_rate10(struct HeroBody *body)
{
    struct Hero *hero_a = gUnk_03000FD8->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct Hero *hero_b = gUnk_03000FD8->heroB;
    struct HeroBody *body_b = &hero_b->body;

    if ((body_b->sprite->flags & 8) != 0)
    {
        set_slot_value(0x88, -1);
        if ((hero_a->body.mode & 6) == 2 || (hero_a->body.mode & 6) == 4)
        {
            sprite_set_action(body_a, 6, 0x2035, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            sprite_set_action(body_b, 6, 0x2064, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 0;
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            body_b->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0xA);
            body_b->progress = 0;
            ((struct HeroFlagBits *)&body_b->flags)->active = 0;
        }
        body_b->timer = body_b->timer & 0xFFF;
        body->state = hero_pair_b_step_cc7a0;
    }
}

ASM_FUNC("asm/code_080CC614/hero_pair_b_step_cc7a0.s", void hero_pair_b_step_cc7a0(struct HeroBody *body))

void hero_pair_link_b_fx_rate20(struct HeroBody *body)
{
    struct Hero *hero_b = gUnk_03000FD8->heroB;
    struct HeroBody *body_b = &hero_b->body;
    struct HeroBody *partner = body->partner;

    state_stack_advance_progress(body_b);
    if ((hero_b->body.flags & 0x20) != 0)
    {
        set_slot_value(0x63, -1);
        ((struct HeroDataFlagBits *)&partner->data->flags111)->partnerLinked = 0;
        ((struct HeroCcBits *)&body_b->cc)->low = 0;
        body_b->scale = 0x100;
        resolve_contact_action(body_b, partner, 0, 0, 0x040322AB);
        body_b->count = body_b->count + 1;
        if (partner->data->displayTimer > 0 || (partner->data->flags111 & 2) != 0)
        {
            s32 x = (partner->targetX / 256) + partner->ofsX;
            s32 y = (partner->targetY / 256) + partner->ofsY;
            effect_spawn_at(0x71F, x, y, 0x10, partner);
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            body_b->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0x14);
            body_b->progress = 0;
            ((struct HeroFlagBits *)&body_b->flags)->active = 0;
        }
        body->state = pair_scan_hero_pair_place_both_finish;
    }
}

void pair_scan_hero_pair_place_both_finish(struct HeroBody *body)
{
    struct HeroBody *bodyA;
    struct Hero *heroB;
    struct HeroBody *bodyB;
    struct HeroData *data;
    s32 t;
    s32 px;
    s32 py;
    s32 pz;
    s16 *span;

    bodyA = &gUnk_03000FD8->heroA->body;
    heroB = gUnk_03000FD8->heroB;
    bodyB = &heroB->body;
    state_stack_advance_progress(bodyB);
    if (heroB->body.flags & 0x20)
    {
        set_slot_value(0x53, -1);
        if ((heroB->body.mode & 6) == 2 || (heroB->body.mode & 6) == 4)
        {
            data = bodyB->data;
            t = data->targetX;
            if (t < 0)
                t += 0xff;
            px = t >> 8;
            t = data->targetY;
            if (t < 0)
                t += 0xff;
            py = t >> 8;
            t = data->targetH;
            if (t < 0)
                t += 0xff;
            pz = t >> 8;
            place_targets(bodyB, px, py, pz, -1);
            set_motion_start(bodyB, 0x400);
        }
        if ((bodyB->mode & 6) == 2 || (bodyB->mode & 6) == 4)
        {
            if (bodyB->data->poseIndex == -1)
            {
                sprite_set_action(bodyB, 0, 0x2002, 0);
            }
            else
            {
                sprite_set_action(bodyB, 0, 0x204F, 0);
            }
            ((struct HeroMirrorBits *)&bodyB->sprite->mirror)->flip = (span = &bodyB->spanA)[0] >= span[1];
            ((struct HeroSpriteBits *)&bodyB->sprite->flags)->mode = 0;
        }
        if ((bodyB->mode & 6) == 2 || (bodyB->mode & 6) == 4)
        {
            bodyB->state = 0;
            enter_finish_rebuild(bodyB);
        }
        if ((bodyA->mode & 6) == 2 || (bodyA->mode & 6) == 4)
        {
            data = bodyA->data;
            t = data->targetX;
            if (t < 0)
                t += 0xff;
            px = t >> 8;
            t = data->targetY;
            if (t < 0)
                t += 0xff;
            py = t >> 8;
            t = data->targetH;
            if (t < 0)
                t += 0xff;
            pz = t >> 8;
            place_targets(bodyA, px, py, pz, -1);
            set_motion_start(bodyA, 0x400);
        }
        if ((bodyA->mode & 6) == 2 || (bodyA->mode & 6) == 4)
        {
            if (bodyA->data->poseIndex == -1)
            {
                sprite_set_action(bodyA, 0, 0x2002, 0);
            }
            else
            {
                sprite_set_action(bodyA, 0, 0x204F, 0);
            }
            ((struct HeroMirrorBits *)&bodyA->sprite->mirror)->flip = (span = &bodyA->spanA)[0] >= span[1];
            ((struct HeroSpriteBits *)&bodyA->sprite->flags)->mode = 0;
        }
        if ((bodyA->mode & 6) == 2 || (bodyA->mode & 6) == 4)
        {
            bodyA->state = 0;
            enter_finish_rebuild(bodyA);
        }
    }
}

void hero_pair_anim7_b_rate8(struct HeroBody *body)
{
    struct Hero *hero_a = gUnk_03000FD8->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct Hero *hero_b = gUnk_03000FD8->heroB;
    struct HeroBody *body_b = &hero_b->body;

    if ((hero_a->body.mode & 6) == 2 || (hero_a->body.mode & 6) == 4)
    {
        sprite_set_action(body_a, 7, 0x2035, 0);
        ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
    }
    if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
    {
        sprite_set_action(body_b, 7, 0x2064, 0);
        ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
    }
    if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
    {
        body_b->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 8);
        body_b->progress = 0;
        ((struct HeroFlagBits *)&body_b->flags)->active = 0;
    }
    body->state = hero_pair_link_b_fx_rate20_alt;
}

void hero_pair_link_b_fx_rate20_alt(struct HeroBody *body)
{
    struct Hero *hero_b = gUnk_03000FD8->heroB;
    struct HeroBody *body_b = &hero_b->body;
    struct HeroBody *partner = body->partner;
    u32 id;

    state_stack_advance_progress(body_b);
    if ((hero_b->body.flags & 0x20) != 0)
    {
        set_slot_value(0x55, -1);
        ((struct HeroDataFlagBits *)&partner->data->flags111)->partnerLinked = 0;
        ((struct HeroCcBits *)&body_b->cc)->low = 0x50;
        body_b->scale = 0x100;
        resolve_contact_action(body_b, partner, 0, 0, 0x050323AB);
        body_b->count = body_b->count + 1;
        if (partner->data->displayTimer > 0 || (partner->data->flags111 & 2) != 0)
        {
            s32 x = (partner->targetX / 256) + partner->ofsX;
            s32 y = (partner->targetY / 256) + partner->ofsY;
            u32 kind = partner->data->effect->unk1C >> 4;
            id = 0x2F6F;
            if (kind == 0)
            {
                id -= 0xDC;
            }
            effect_spawn_at(id, x, y, 0, partner);
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            body_b->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0x14);
            body_b->progress = 0;
            ((struct HeroFlagBits *)&body_b->flags)->active = 0;
        }
        body->state = pair_scan_hero_pair_place_both_finish_alt;
    }
}

void pair_scan_hero_pair_place_both_finish_alt(struct HeroBody *body)
{
    struct HeroBody *bodyA;
    struct Hero *heroB;
    struct HeroBody *bodyB;
    struct HeroData *data;
    s32 t;
    s32 px;
    s32 py;
    s32 pz;
    s16 *span;

    bodyA = &gUnk_03000FD8->heroA->body;
    heroB = gUnk_03000FD8->heroB;
    bodyB = &heroB->body;
    state_stack_advance_progress(bodyB);
    if (heroB->body.flags & 0x20)
    {
        set_slot_value(0x53, -1);
        if ((heroB->body.mode & 6) == 2 || (heroB->body.mode & 6) == 4)
        {
            data = bodyB->data;
            t = data->targetX;
            if (t < 0)
                t += 0xff;
            px = t >> 8;
            t = data->targetY;
            if (t < 0)
                t += 0xff;
            py = t >> 8;
            t = data->targetH;
            if (t < 0)
                t += 0xff;
            pz = t >> 8;
            place_targets(bodyB, px, py, pz, -1);
            set_motion_start(bodyB, 0x400);
        }
        if ((bodyB->mode & 6) == 2 || (bodyB->mode & 6) == 4)
        {
            if (bodyB->data->poseIndex == -1)
            {
                sprite_set_action(bodyB, 0, 0x2002, 0);
            }
            else
            {
                sprite_set_action(bodyB, 0, 0x204F, 0);
            }
            ((struct HeroMirrorBits *)&bodyB->sprite->mirror)->flip = (span = &bodyB->spanA)[0] >= span[1];
            ((struct HeroSpriteBits *)&bodyB->sprite->flags)->mode = 0;
        }
        if ((bodyB->mode & 6) == 2 || (bodyB->mode & 6) == 4)
        {
            bodyB->state = 0;
            enter_finish_rebuild(bodyB);
        }
        if ((bodyA->mode & 6) == 2 || (bodyA->mode & 6) == 4)
        {
            data = bodyA->data;
            t = data->targetX;
            if (t < 0)
                t += 0xff;
            px = t >> 8;
            t = data->targetY;
            if (t < 0)
                t += 0xff;
            py = t >> 8;
            t = data->targetH;
            if (t < 0)
                t += 0xff;
            pz = t >> 8;
            place_targets(bodyA, px, py, pz, -1);
            set_motion_start(bodyA, 0x400);
        }
        if ((bodyA->mode & 6) == 2 || (bodyA->mode & 6) == 4)
        {
            if (bodyA->data->poseIndex == -1)
            {
                sprite_set_action(bodyA, 0, 0x2002, 0);
            }
            else
            {
                sprite_set_action(bodyA, 0, 0x204F, 0);
            }
            ((struct HeroMirrorBits *)&bodyA->sprite->mirror)->flip = (span = &bodyA->spanA)[0] >= span[1];
            ((struct HeroSpriteBits *)&bodyA->sprite->flags)->mode = 0;
        }
        if ((bodyA->mode & 6) == 2 || (bodyA->mode & 6) == 4)
        {
            bodyA->state = 0;
            enter_finish_rebuild(bodyA);
        }
    }
}

void hero_pair_anim_a_wait(struct HeroBody *body)
{
    struct Hero *hero_a = gUnk_03000FD8->heroA;
    struct HeroBody *body_a = &hero_a->body;

    step_motion(body_a);
    if ((hero_a->body.flags & 0x20) != 0)
    {
        set_slot_value(0x2E, -1);
        if ((hero_a->body.mode & 6) == 2 || (hero_a->body.mode & 6) == 4)
        {
            sprite_set_action(body_a, 0xA, 0x2035, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        body->state = hero_a_sprite_active_place_finish;
    }
}

void hero_pair_pose_a_anim(struct HeroBody *body)
{
    struct Hero *hero_a = gUnk_03000FD8->heroA;
    struct HeroBody *body_a = &hero_a->body;

    if ((body_a->sprite->flags & 8) != 0)
    {
        if ((hero_a->body.mode & 6) == 2 || (hero_a->body.mode & 6) == 4)
        {
            sprite_set_action(body_a, 8, 0x2034, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
        body->state = hero_pair_finish_place_both;
    }
}

void hero_b_wait_idle(struct HeroBody *body)
{
    struct Hero *hero_b = gUnk_03000FD8->heroB;

    step_motion(&hero_b->body);
    if ((hero_b->body.flags & 0x20) != 0)
    {
        body->state = hero_b_step_rate_16_when_a_idle;
    }
}

u32 hero_pair_init_pose_partner(struct HeroBody *body)
{
    struct HeroData *data;
    u8 *record;
    struct HeroBody *body_a;
    struct HeroBody *body_b;
    struct HeroBody *partner;
    void *anim;

    record = gUnk_03000FF4 - ((((body->data->poseIndex << 4) - body->data->poseIndex) << 2) + 0x3C);
    data = body->data;
    data->poseValue = *(u16 *)(record + 0x2A);
    body_a = &gUnk_03000FD8->heroA->body;
    body_b = &gUnk_03000FD8->heroB->body;
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
    body->state = hero_pair_pose_b_mode;
    body_b->count = 0;
    body_a->count = 0;
    schedule_pose_task(partner);
    return 1;
}

ASM_FUNC("asm/code_080CC614/hero_pair_pose_b_mode.s", void hero_pair_pose_b_mode(struct HeroBody *body))

void hero_a_step16_when_b_idle(struct HeroBody *body)
{
    struct Hero *hero_a = gUnk_03000FD8->heroA;
    struct Hero *hero_b = gUnk_03000FD8->heroB;

    if (hero_b->body.state == 0)
    {
        if ((hero_a->body.mode & 6) == 2 || (hero_a->body.mode & 6) == 4)
        {
            hero_a->body.rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0x10);
            hero_a->body.progress = 0;
            ((struct HeroFlagBits *)&hero_a->body.flags)->active = 0;
        }
        body->state = hero_a_spawn_child;
    }
}

void hero_a_spawn_child(struct HeroBody *body)
{
    struct Hero *hero_a = gUnk_03000FD8->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct HeroBody *child;

    state_stack_advance_progress(body_a);
    if ((hero_a->body.flags & 0x20) != 0)
    {
        set_slot_value(0xAF, -1);
        if ((hero_a->body.mode & 6) == 2 || (hero_a->body.mode & 6) == 4)
        {
            sprite_set_action(body_a, 0, 0x2036, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
        child = obj_spawn_linked(body_a, 0x208A, 0);
        ((struct HeroChildFlagBits *)&child->flags)->bit0 = 1;
        ((struct HeroChildFlagBits *)&child->flags)->bit2 = 1;
        child->sprite->unkE = body_a->sprite->unkE + 3;
        child->posX += 0xE00;
        child->posZ += 0x1000;
        body->state = hero_a_child_anim_then_done;
    }
}

void hero_a_child_anim_then_done(struct HeroBody *body)
{
    struct Hero *hero_a = gUnk_03000FD8->heroA;
    struct HeroBody *body_a = &hero_a->body;

    if ((body_a->sprite->flags & 8) != 0)
    {
        set_slot_value(0x158, -1);
        if ((hero_a->body.mode & 6) == 2 || (hero_a->body.mode & 6) == 4)
        {
            sprite_set_action(body_a, 1, 0x2036, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            body_a->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0x50);
            body_a->progress = 0;
            ((struct HeroFlagBits *)&body_a->flags)->active = 0;
        }
        sprite_set_action(body_a->child, 1, 0, 0);
        ((struct HeroSpriteBits *)&body_a->child->sprite->flags)->mode = 0;
        body_a->unk9C = 0;
        body_a->unkA0 = 0;
        body_a->timer = body_a->timer & 0xFFF;
        body->state = hero_a_press_pose_cdbd4;
    }
}

ASM_FUNC("asm/code_080CC614/hero_a_press_pose_cdbd4.s", void hero_a_press_pose_cdbd4(struct HeroBody *body))
