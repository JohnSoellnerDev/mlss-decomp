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

struct HeroSpriteFlag4Bits
{
    u8 unk0 : 4;
    u8 bit4 : 1;
    u8 unk5 : 3;
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

struct HeroData
{
    u8 filler0[0xD8];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler0E4[0xEC - 0xE4];
    s16 poseIndex;
    u8 filler0EE[0xF6 - 0xEE];
    s16 flagsF6;
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
    u8 filler94[0xAC - 0x94];
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
    u8 enableByte;
    u8 filler0F[0x70 - 0x0F];
    struct Hero *heroA;
    struct Hero *heroB;
    u8 filler78[0x24C - 0x78];
    void *trackedObj;
};

extern struct World *gUnk_03000FD8;
extern u8 *gUnk_03000FF4;

void state_stack_advance_progress(struct HeroBody *body);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void set_slot_value(u32 id, s32 value);
void step_motion(struct HeroBody *body);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void set_motion_start(struct HeroBody *body, u32 speed);
void enter_finish_rebuild(struct HeroBody *body);
void schedule_pose_task(struct HeroBody *obj);
void set_motion_speed_aux(struct HeroBody *body, u32 speed, u32 aux);
void set_motion_offsets(struct HeroBody *body, u32 offsets);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 h, struct HeroBody *owner);
u32 find_frame_entry(struct HeroBody *obj, u32 index);

void hero_a_active_copy_pos_fx(struct HeroBody *body);
void hero_a_step_16_b_idle_alt(struct HeroBody *body);
void hero_b_active_place_at_a_pose(struct HeroBody *body);

void hero_a_link_to_partner_fx(struct HeroBody *body);
void hero_a_active_set_targets(struct HeroBody *body);
void hero_a_active_fx_mirror(struct HeroBody *body);
void hero_pair_place_finish_sprite(struct HeroBody *body);
void hero_a_on_partner(struct HeroBody *body);
void hero_a_active_step_12(struct HeroBody *body);
void hero_a_active_follow_b_aux(struct HeroBody *body);
void hero_a_step_threshold_c16c(struct HeroBody *body);
void hero_a_sprite_set_targets(struct HeroBody *body);
void hero_a_step_stage_fx(struct HeroBody *body);
void active_targets_hero_pair_place_finish_b(struct HeroBody *body);

void hero_b_pose_anim(struct HeroBody *body);
void noop_state_080acec0(struct HeroBody *body);
void hero_b_on_object(struct HeroBody *body);

ASM_FUNC("asm/code_080AB8E8/hero_a_link_to_partner_fx.s", void hero_a_link_to_partner_fx(struct HeroBody *body))

void hero_a_active_set_targets(struct HeroBody *body)
{
    struct Hero *hero_a = gUnk_03000FD8->heroA;
    struct HeroBody *body_a = &hero_a->body;

    state_stack_advance_progress(body_a);
    if ((hero_a->body.flags & 0x20) != 0)
    {
        set_slot_value(0x30, -1);
        if ((hero_a->body.mode & 6) == 2 || (hero_a->body.mode & 6) == 4)
        {
            place_targets(body_a,
                          (body_a->targetX / 0x100) + body_a->ofsX - 0x3C,
                          (body_a->targetY / 0x100) + body_a->ofsY, 0, -1);
            body_a->height = (body_a->targetZ / 0x100) + 0x28;
            set_motion_offsets(body_a, 0x500);
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            sprite_set_action(body_a, 0xB, 0x2000, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        body->state = hero_a_active_fx_mirror;
    }
}

void hero_a_active_fx_mirror(struct HeroBody *body)
{
    struct Hero *hero_a = gUnk_03000FD8->heroA;
    struct HeroBody *body_a = &hero_a->body;

    step_motion(body_a);
    if ((hero_a->body.flags & 0x20) != 0)
    {
        set_slot_value(0x2E, -1);
        effect_spawn_at(0x1E, body_a->posX / 0x100, body_a->posY / 0x100, body_a->posZ / 0x100, body_a);
        if ((hero_a->body.mode & 6) == 2 || (hero_a->body.mode & 6) == 4)
        {
            sprite_set_action(body_a, 8, 0x2034, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
        ((struct HeroMirrorBits *)&body_a->sprite->mirror)->flip = 1;
        body->state = hero_pair_place_finish_sprite;
    }
}

void hero_pair_place_finish_sprite(struct HeroBody *body)
{
    struct World *world = gUnk_03000FD8;
    struct Hero *hero_a = world->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct Hero *hero_b = world->heroB;
    struct HeroBody *body_b = &hero_b->body;
    struct HeroData *data;
    struct HeroSprite *sprite;
    struct HeroSpan *span;
    s32 flip;

    if ((body_a->sprite->flags & 8) != 0)
    {
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
            sprite = body_b->sprite;
            {
                flip = 0;
                span = &body_b->span;
                if (span->a >= span->b)
                    flip = 1;
                ((struct HeroMirrorBits *)&sprite->mirror)->flip = flip;
            }
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 0;
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            body_b->state = 0;
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
            sprite = body_a->sprite;
            {
                flip = 0;
                span = &body_a->span;
                if (span->a >= span->b)
                    flip = 1;
                ((struct HeroMirrorBits *)&sprite->mirror)->flip = flip;
            }
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            body_a->state = 0;
            enter_finish_rebuild(body_a);
        }
    }
}

void hero_a_on_partner(struct HeroBody *body)
{
    struct World *world = gUnk_03000FD8;
    struct Hero *hero_a = world->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct HeroBody *body_b = &world->heroB->body;
    struct HeroBody *partner = body->partner;
    s32 height;

    set_slot_value(0x62, -1);
    effect_spawn_at(0x2F9E, body_a->posX / 0x100, body_a->posY / 0x100, body_a->posZ / 0x100, body_a);
    if ((hero_a->body.mode & 6) == 2 || (hero_a->body.mode & 6) == 4)
    {
        place_targets(body_a,
                      (partner->targetX / 0x100) + partner->ofsX,
                      (partner->targetY / 0x100) + partner->ofsY,
                      (partner->targetZ / 0x100) + partner->ofsZ + 0x3C, -1);
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
        height += MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x1000, (height * height) / 0x40 + 0x40);
        body_a->height = height;
        set_motion_offsets(body_a, 0x600);
    }
    if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
    {
        sprite_set_action(body_a, 9, 0x2030, 0);
        ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
    }
    if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
    {
        sprite_set_action(body_b, 7, 0x205F, 0);
        ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
    }
    body->state = hero_a_active_step_12;
}

void hero_a_active_step_12(struct HeroBody *body)
{
    struct World *world = gUnk_03000FD8;
    struct HeroBody *body_a = &world->heroA->body;
    struct HeroBody *body_b = &world->heroB->body;
    s32 scale;

    step_motion(body_a);
    scale = body_b->sprite->scale;
    if ((s16)body_b->sprite->scale <= 0xFF)
    {
        body_b->sprite->scale = scale + 0x10;
        if ((s16)body_b->sprite->scale > 0x100)
            body_b->sprite->scale = 0x100;
    }
    if ((body_a->flags & 0x20) != 0)
    {
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            sprite_set_action(body_a, 3, 0x2030, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            body_a->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0xC);
            body_a->progress = 0;
            ((struct HeroFlagBits *)&body_a->flags)->active = 0;
        }
        body->state = hero_a_active_follow_b_aux;
    }
}

void hero_a_active_follow_b_aux(struct HeroBody *body)
{
    struct Hero *hero_a = gUnk_03000FD8->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct HeroBody *partner = body->partner;

    state_stack_advance_progress(body_a);
    if ((hero_a->body.flags & 0x20) != 0)
    {
        if ((hero_a->body.mode & 6) == 2 || (hero_a->body.mode & 6) == 4)
        {
            place_targets(body_a,
                          (partner->targetX / 0x100) + partner->ofsX,
                          (partner->targetY / 0x100) + partner->ofsY,
                          (partner->targetZ / 0x100) + partner->ofsZ - 4, -1);
            set_motion_speed_aux(body_a, 0, 0xCC);
        }
        body_a->timer &= 0xFFF;
        body->state = hero_a_step_threshold_c16c;
    }
}

ASM_FUNC("asm/code_080AB8E8/hero_a_step_threshold_c16c.s", void hero_a_step_threshold_c16c(struct HeroBody *body))

void hero_a_sprite_set_targets(struct HeroBody *body)
{
    struct Hero *hero_a = gUnk_03000FD8->heroA;
    struct HeroBody *body_a = &hero_a->body;

    if ((body_a->sprite->flags & 8) != 0)
    {
        set_slot_value(0x2B, -1);
        if ((hero_a->body.mode & 6) == 2 || (hero_a->body.mode & 6) == 4)
        {
            place_targets(body_a,
                          (body_a->targetX / 0x100) + body_a->ofsX - 0x3C,
                          (body_a->targetY / 0x100) + body_a->ofsY, 0, -1);
            body_a->height = (body_a->targetZ / 0x100) + 0x28;
            set_motion_offsets(body_a, 0x500);
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            sprite_set_action(body_a, 0xA, 0x2030, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        body->state = hero_a_step_stage_fx;
    }
}

ASM_FUNC("asm/code_080AB8E8/hero_a_step_stage_fx.s", void hero_a_step_stage_fx(struct HeroBody *body))

void active_targets_hero_pair_place_finish_b(struct HeroBody *body)
{
    struct World *world = gUnk_03000FD8;
    struct Hero *hero_a = world->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct Hero *hero_b = world->heroB;
    struct HeroBody *body_b = &hero_b->body;
    struct HeroData *data;
    struct HeroSprite *sprite;
    struct HeroSpan *span;
    s32 flip;

    if ((body_a->sprite->flags & 8) != 0)
    {
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
            sprite = body_b->sprite;
            {
                flip = 0;
                span = &body_b->span;
                if (span->a >= span->b)
                    flip = 1;
                ((struct HeroMirrorBits *)&sprite->mirror)->flip = flip;
            }
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 0;
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            body_b->state = 0;
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
            sprite = body_a->sprite;
            {
                flip = 0;
                span = &body_a->span;
                if (span->a >= span->b)
                    flip = 1;
                ((struct HeroMirrorBits *)&sprite->mirror)->flip = flip;
            }
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            body_a->state = 0;
            enter_finish_rebuild(body_a);
        }
    }
}

void hero_b_active_hand_over(void)
{
    struct Hero *hero_a = gUnk_03000FD8->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct Hero *hero_b = gUnk_03000FD8->heroB;
    struct HeroBody *body_b = &hero_b->body;

    state_stack_advance_progress(body_b);
    if ((hero_b->body.flags & 0x20) != 0)
    {
        if ((hero_a->body.mode & 6) == 2 || (hero_a->body.mode & 6) == 4)
            body_a->state = hero_a_active_copy_pos_fx;
        body_b->state = 0;
    }
}

void hero_b_ready_chain(struct HeroBody *body)
{
    struct Hero *hero_b = gUnk_03000FD8->heroB;
    struct HeroBody *body_b = &hero_b->body;

    if ((hero_b->body.mode & 6) == 2 || (hero_b->body.mode & 6) == 4)
        body_b->state = hero_b_pose_anim;
    body->state = noop_state_080acec0;
}

void hero_b_progress_end(void)
{
    struct Hero *hero_b = gUnk_03000FD8->heroB;
    struct HeroBody *body_b = &hero_b->body;

    step_motion(body_b);
    if (hero_b->body.progress > 0x1FFF)
    {
        set_slot_value(0x53, -1);
        if ((hero_b->body.mode & 6) == 2 || (hero_b->body.mode & 6) == 4)
        {
            sprite_set_action(body_b, 1, 0x205F, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 0;
        }
        ((struct HeroSpriteFlag4Bits *)&body_b->sprite->flags)->bit4 = 1;
        body_b->state = 0;
    }
}

void hero_a_active_clear_anim(struct HeroBody *body)
{
    struct Hero *hero_a = gUnk_03000FD8->heroA;
    struct HeroBody *body_a = &hero_a->body;

    step_motion(body_a);
    if ((hero_a->body.flags & 0x20) != 0)
    {
        if ((hero_a->body.mode & 6) == 2 || (hero_a->body.mode & 6) == 4)
        {
            sprite_set_action(body_a, 0, -1, 0);
            ((struct HeroMirrorBits *)&body_a->sprite->mirror)->flip = 0;
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        body->state = hero_a_step_16_b_idle_alt;
    }
}

void hero_b_pose_anim(struct HeroBody *body)
{
    struct Hero *hero_b = gUnk_03000FD8->heroB;
    struct HeroBody *body_b = &hero_b->body;

    if ((hero_b->body.mode & 6) == 2 || (hero_b->body.mode & 6) == 4)
    {
        if (body_b->data->poseIndex == -1)
            sprite_set_action(body_b, 4, 0x2000, 0);
        else
            sprite_set_action(body_b, 4, 0x204D, 0);
        ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
    }
    body->state = hero_b_active_place_at_a_pose;
}

void noop_state_080acec0(struct HeroBody *body)
{
}

u32 pair_start_pose(struct HeroBody *body)
{
    struct HeroData *data;
    struct HeroBody *body_a;
    struct HeroBody *body_b;
    struct HeroBody *partner;
    void *anim;
    s32 pose;
    u8 *record;
    struct World *world;
    u8 **party = &gUnk_03000FF4;

    data = body->data;
    pose = data->poseIndex;
    record = *party - (pose * 60 + 0x3C);
    data->poseValue = *(u16 *)(record + 0x2A);
    world = gUnk_03000FD8;
    body_a = &world->heroA->body;
    body_b = &world->heroB->body;
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
    body->state = hero_b_on_object;
    body_b->count = 0;
    body_a->count = 0;
    schedule_pose_task(partner);
    return 1;
}

ASM_FUNC("asm/code_080AB8E8/hero_b_on_object.s", void hero_b_on_object(struct HeroBody *body))
