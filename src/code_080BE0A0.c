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

struct HeroSpan
{
    s16 a;
    s16 b;
};

struct HeroDataFlagBits
{
    u8 unk0 : 1;
    u8 bit1 : 1;
    u8 unk2 : 6;
};

struct HeroSlot
{
    u8 filler0[0x1C];
    u8 bits1C;
    u8 bits1D;
};

struct Hero;

struct HeroData
{
    struct Hero *first;
    u8 filler4[0xD8 - 0x04];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler0E4[0xEC - 0xE4];
    s16 poseIndex;
    u8 filler0EE[0xF0 - 0xEE];
    struct HeroSlot *slot;
    u8 filler0F4[0xF6 - 0xF4];
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
    u8 filler44[0x4C - 0x44];
    void (*state)(struct HeroBody *);
    u32 unk50;
    u8 filler54[0x6C - 0x54];
    void *anim;
    u8 filler70[0x74 - 0x70];
    u8 unk74;
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
    u16 unk92;
    u32 wideLo;
    u32 wideHi;
    u8 filler9C[0xA8 - 0x9C];
    u32 link;
    s16 step;
    u16 timer;
    s16 randX;
    s16 randY;
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
    struct Hero *next;
    u8 filler4[0x08 - 0x04];
    struct HeroBody body;
    u8 filler0D8[0xF0 - 0xD8];
    struct HeroSlot *slot;
};

struct World
{
    u8 filler0[0x70];
    struct Hero *heroA;
    struct Hero *heroB;
    u8 filler78[0x24C - 0x78];
    void *trackedObj;
};

extern struct World *gUnk_03000FD8;
extern u8 gUnk_0300034C[];

void state_stack_advance_progress(struct HeroBody *body);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void set_slot_value(u32 id, s32 value);
void step_motion(struct HeroBody *body);
void step_motion_scaled(struct HeroBody *body);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void set_motion_start(struct HeroBody *body, u32 speed);
void set_motion_speed_aux(struct HeroBody *body, u32 speed, u32 aux);
void set_motion_offsets(struct HeroBody *body, u32 offsets);
void enter_finish_rebuild(struct HeroBody *body);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 h, struct HeroBody *owner);
s32 obj_last_x(void *obj);
s32 obj_last_y(void *obj);
void body_set_pose(u32 art, struct HeroBody *body, u32 x, u32 y);

void hero_a_hand_over(struct HeroBody *body);
void hero_a_place_pose_finish_2(struct HeroBody *body);
void hero_a_active_slot_pose_anim_2(struct HeroBody *body);
void hero_a_place_pose_finish_3(struct HeroBody *body);
void hero_a_join_partner_next(struct HeroBody *body);
void hero_a_height_decay_next(struct HeroBody *body);
void hero_a_random_jitter_rate_next(struct HeroBody *body);
void hero_wait_then_hand_over(struct HeroBody *body);
void hero_a_timer_step_080be698(struct HeroBody *body);
void hero_a_step_wait_f5c8(struct HeroBody *body);
void hero_b_follow_partner_offset(struct HeroBody *body);
void hero_b_ratio_release_a(struct HeroBody *body);

void hero_a_active_slot_pose_anim(struct HeroBody *body)
{
    struct HeroBody *body_a = &gUnk_03000FD8->heroA->body;

    step_motion_scaled(body_a);
    if ((body_a->flags & 0x20) != 0)
    {
        set_slot_value(0x2E, -1);
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            if (body_a->data->poseIndex == -1)
                sprite_set_action(body_a, 8, 0x2000, 0);
            else
                sprite_set_action(body_a, 8, 0x204D, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
        body->state = hero_a_place_pose_finish_2;
    }
}

void hero_a_place_pose_finish_2(struct HeroBody *body)
{
    struct HeroBody *body_a = &gUnk_03000FD8->heroA->body;
    struct HeroData *data;
    struct HeroSprite *sprite;
    struct HeroSpan *span;
    s32 tx;
    s32 ty;
    s32 th;
    s32 x;
    s32 y;
    s32 h;

    if ((body_a->sprite->flags & 8) != 0)
    {
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            data = body_a->data;
            tx = data->targetX;
            if (tx < 0)
                tx += 0xFF;
            x = tx >> 8;
            ty = data->targetY;
            if (ty < 0)
                ty += 0xFF;
            y = ty >> 8;
            th = data->targetH;
            if (th < 0)
                th += 0xFF;
            h = th >> 8;
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
            {
                s32 flip = 0;
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

void hero_b_link_partner_fx_rate(struct HeroBody *body)
{
    struct HeroBody *body_b = &gUnk_03000FD8->heroB->body;
    struct HeroBody *partner = body->partner;

    ((struct HeroDataFlagBits *)&partner->data->flags111)->bit1 = 1;
    partner->link = 1;
    if (partner->targetZ == 0)
    {
        if ((partner->mode & 6) == 2 || (partner->mode & 6) == 4)
            partner->state = hero_a_join_partner_next;
    }
    effect_spawn_at(0x5DF, body_b->posX / 0x100, body_b->posY / 0x100, body_b->posZ / 0x100, body_b);
    if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
    {
        sprite_set_action(body_b, 4, 0x2062, 0);
        ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
    }
    body_b->unk74 = 0;
    body_b->unk75 = 0;
    if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
    {
        body_b->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 8);
        body_b->progress = 0;
        ((struct HeroFlagBits *)&body_b->flags)->active = 0;
    }
    body->state = hero_b_follow_partner_offset;
}

void hero_b_follow_partner_offset(struct HeroBody *body)
{
    struct Hero *hero_b = gUnk_03000FD8->heroB;
    struct HeroBody *body_b = &hero_b->body;
    struct HeroBody *partner = body->partner;

    state_stack_advance_progress(body_b);
    if (partner->link != 0)
    {
        if ((partner->targetZ << 6) < hero_b->body.ofsZ * hero_b->body.progress)
        {
            if ((partner->mode & 6) == 2 || (partner->mode & 6) == 4)
                partner->state = hero_a_join_partner_next;
        }
    }
    if ((body_b->flags & 0x20) != 0)
    {
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            place_targets(body_b, partner->targetX / 0x100 + partner->ofsX, partner->targetY / 0x100 + partner->ofsY, 0, -1);
            body_b->unk92 = 0x20;
            set_motion_offsets(body_b, 0x600);
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            if (body_b->data->poseIndex == -1)
                sprite_set_action(body_b, 5, 0x2000, 0);
            else
                sprite_set_action(body_b, 5, 0x204D, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
        }
        body->state = hero_b_ratio_release_a;
    }
}

ASM_FUNC("asm/code_080BE0A0/hero_b_ratio_release_a.s", void hero_b_ratio_release_a(struct HeroBody *body))

ASM_FUNC("asm/code_080BE0A0/hero_a_timer_step_080be698.s", void hero_a_timer_step_080be698(struct HeroBody *body))

void hero_a_place_offset_speed(struct HeroBody *body)
{
    struct Hero *hero_a = gUnk_03000FD8->heroA;
    struct HeroBody *body_a = &hero_a->body;

    if ((body_a->sprite->flags & 8) != 0)
    {
        if ((hero_a->body.mode & 6) == 2 || (hero_a->body.mode & 6) == 4)
        {
            place_targets(body_a, body_a->targetX / 0x100 + hero_a->body.ofsX, body_a->targetY / 0x100 + hero_a->body.ofsY, 0, -1);
            set_motion_speed_aux(body_a, 0, 0x66);
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            if (body_a->data->poseIndex == -1)
                sprite_set_action(body_a, 6, 0x2000, 0);
            else
                sprite_set_action(body_a, 6, 0x204D, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
        body->state = hero_a_active_slot_pose_anim_2;
    }
}

void hero_a_active_slot_pose_anim_2(struct HeroBody *body)
{
    struct HeroBody *body_a = &gUnk_03000FD8->heroA->body;

    step_motion_scaled(body_a);
    if ((body_a->flags & 0x20) != 0)
    {
        set_slot_value(0x2E, -1);
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            if (body_a->data->poseIndex == -1)
                sprite_set_action(body_a, 8, 0x2000, 0);
            else
                sprite_set_action(body_a, 8, 0x204D, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
        body->state = hero_a_place_pose_finish_3;
    }
}

void hero_a_place_pose_finish_3(struct HeroBody *body)
{
    struct HeroBody *body_a = &gUnk_03000FD8->heroA->body;
    struct HeroData *data;
    struct HeroSprite *sprite;
    struct HeroSpan *span;
    s32 tx;
    s32 ty;
    s32 th;
    s32 x;
    s32 y;
    s32 h;

    if ((body_a->sprite->flags & 8) != 0)
    {
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            data = body_a->data;
            tx = data->targetX;
            if (tx < 0)
                tx += 0xFF;
            x = tx >> 8;
            ty = data->targetY;
            if (ty < 0)
                ty += 0xFF;
            y = ty >> 8;
            th = data->targetH;
            if (th < 0)
                th += 0xFF;
            h = th >> 8;
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
            {
                s32 flip = 0;
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

ASM_FUNC("asm/code_080BE0A0/hero_a_join_partner_next.s", void hero_a_join_partner_next(struct HeroBody *body))

ASM_FUNC("asm/code_080BE0A0/hero_b_shift_place_pose_next.s", void hero_b_shift_place_pose_next(void))

void hero_a_effect_link_rate_next(struct HeroBody *body)
{
    struct HeroBody *body_a = &gUnk_03000FD8->heroA->body;

    body->partner->link = 1;
    effect_spawn_at(0x2FE6, body_a->posX / 0x100 + 0x1C, body_a->posY / 0x100, body_a->posZ / 0x100 + 0x10, body_a);
    if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
    {
        sprite_set_action(body_a, 2, 0x2025, 0);
        ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
    }
    if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
    {
        body_a->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 5);
        body_a->progress = 0;
        ((struct HeroFlagBits *)&body_a->flags)->active = 0;
    }
    body->state = hero_a_height_decay_next;
}

ASM_FUNC("asm/code_080BE0A0/hero_a_height_decay_next.s", void hero_a_height_decay_next(struct HeroBody *body))

ASM_FUNC("asm/code_080BE0A0/hero_a_random_jitter_rate_next.s", void hero_a_random_jitter_rate_next(struct HeroBody *body))

void hero_wait_then_hand_over(struct HeroBody *body)
{
    struct HeroBody *body_a = &gUnk_03000FD8->heroA->body;
    struct HeroBody *partner = body->partner;
    u8 *input;
    s32 rnd;
    s32 z;

    input = gUnk_0300034C;
    if ((*(u16 *)(input + 0x3E) & 3) != 0)
    {
        if ((body->mode & 6) == 2 || (body->mode & 6) == 4)
            body->state = hero_a_hand_over;
    }
    if (partner->state != 0)
    {
        if (partner->targetZ < partner->posZ)
            return;
        rnd = body_a->randY;
        z = partner->posZ;
        if (z < 0)
            z += 0xFF;
        z = z >> 8;
        if (rnd < z)
            return;
    }
    body_a->timer &= 0xFFF;
    body->state = hero_a_step_wait_f5c8;
}

ASM_FUNC("asm/code_080BE0A0/hero_a_step_wait_f5c8.s", void hero_a_step_wait_f5c8(struct HeroBody *body))
