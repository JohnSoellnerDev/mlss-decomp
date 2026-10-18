#include "global.h"
#include "ram_routines.h"

struct HeroSprite
{
    u8 filler0[0x06];
    s16 scale;
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

struct HeroSpriteFlagBits
{
    u8 unk0 : 4;
    u8 bit4 : 1;
    u8 unk5 : 3;
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

struct HeroData
{
    u8 filler0[0xD8];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler0E4[0xEC - 0xE4];
    s16 poseIndex;
    u8 filler0EE[0x114 - 0xEE];
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
    s8 ofsAlt;
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
    u32 distLo;
    u32 distHi;
    u32 unk9C;
    u8 filler0A0[0xAC - 0xA0];
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
    u8 filler0B[0x70 - 0x0B];
    struct Hero *heroA;
    struct Hero *heroB;
};

extern struct World *gWorld;

void state_stack_advance_progress(struct HeroBody *body);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void set_slot_value(u32 id, s32 value);
void step_motion(struct HeroBody *body);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void set_motion_start(struct HeroBody *body, u32 speed);
void set_motion_offsets(struct HeroBody *body, u32 offsets);
void set_motion_speed_aux(struct HeroBody *body, u32 speed, u32 aux);
void enter_finish_rebuild(struct HeroBody *body);
void start_action_4(struct HeroBody *body);
u32 find_frame_entry(struct HeroBody *obj, u32 index);

void hero_a_step_end_82ec(struct HeroBody *body);
void hero_b_ready_a_place_anim(struct HeroBody *body);
void hero_a_wait_reach_rate(struct HeroBody *body);
void hero_a_active_place_partner_z(struct HeroBody *body);
void hero_a_step_reach_input(struct HeroBody *body);
void hero_a_timing_window_9804(struct HeroBody *body);
void hero_b_move_to_partner_pose(struct HeroBody *body);

void hero_b_sprite_grow_a_place(struct HeroBody *body)
{
    struct Hero *hero_a = gWorld->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct Hero *hero_b = gWorld->heroB;
    struct HeroBody *body_b = &hero_b->body;
    struct HeroBody *partner = body->partner;
    u16 scale;
    s32 height;
    s32 sq;
    s32 tx;
    s32 ty;
    s32 tz;
    s32 x;
    s32 y;
    s32 z;

    scale = body_b->sprite->scale;
    if (body_b->sprite->scale <= 255)
    {
        body_b->sprite->scale = scale + 0x10;
        if (body_b->sprite->scale <= 255)
            return;
        body_b->sprite->scale = 0x100;
    }
    body_b->sprite->scale = 0x100;
    if (body_a->unk9C == 1)
    {
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
            body_a->state = hero_b_ready_a_place_anim;
        return;
    }
    ((struct HeroSpriteFlagBits *)&body_a->sprite->flags)->bit4 = 0;
    body_a->sprite->frame = 8;
    ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
    set_slot_value(0x2B, -1);
    if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
    {
        tx = partner->targetX;
        if (tx < 0)
            tx += 0xFF;
        x = (tx >> 8) + partner->ofsX;
        ty = partner->targetY;
        if (ty < 0)
            ty += 0xFF;
        y = (ty >> 8) + partner->ofsY;
        z = partner->targetZ;
        if (z < 0)
            z += 0xFF;
        z = z >> 8;
        z += partner->ofsZ;
        place_targets(body_a, x, y, z, -1);
        if (partner == (struct HeroBody *)((u8 *)partner->data + 8))
        {
            height = partner->ofsZ;
        }
        else
        {
            find_frame_entry(partner, 0);
            height = partner->ofsAlt;
        }
        tz = partner->targetZ;
        if (tz < 0)
            tz += 0xFF;
        height += tz >> 8;
        sq = height * height;
        height += MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0xE10, sq / 0x40 + 0x3C);
        body_a->height = height;
        set_motion_offsets(body_a, 0x566);
    }
    body_a->timer = body_a->timer & 0xFFF;
    body->state = hero_a_step_end_82ec;
}

ASM_FUNC("asm/code_080A814C/hero_a_step_end_82ec.s", void hero_a_step_end_82ec(struct HeroBody *body))

void hero_pair_finish_b_place_a_action(void)
{
    struct Hero *hero_a = gWorld->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct Hero *hero_b = gWorld->heroB;
    struct HeroBody *body_b = &hero_b->body;
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
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            data = body_b->data;
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
            place_targets(body_b, x, y, h, -1);
            set_motion_start(body_b, 0x300);
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            if (body_b->data->poseIndex == -1)
                sprite_set_action(body_b, 0, 0x2002, 0);
            else
                sprite_set_action(body_b, 0, 0x204F, 0);
            sprite = body_b->sprite;
            {
                s32 flip = 0;
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
        set_slot_value(0x2B, -1);
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
            th = body_a->targetZ;
            if (th < 0)
                th += 0xFF;
            body_a->height = (th >> 8) + 0x28;
            set_motion_offsets(body_a, 0x7CC);
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            body_a->state = 0;
            start_action_4(body_a);
        }
    }
}

void hero_b_ready_a_place_anim(struct HeroBody *body)
{
    struct Hero *hero_a = gWorld->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct Hero *hero_b = gWorld->heroB;
    struct HeroBody *body_b = &hero_b->body;
    s32 tx;
    s32 ty;
    s32 tz;
    s32 x;
    s32 y;

    if ((hero_b->body.mode & 6) == 2 || (hero_b->body.mode & 6) == 4)
    {
        body_b->state = hero_b_move_to_partner_pose;
    }
    set_slot_value(0x2B, -1);
    if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
    {
        tx = body_a->targetX;
        if (tx < 0)
            tx += 0xFF;
        x = (tx >> 8) + body_a->ofsX;
        ty = body_a->targetY;
        if (ty < 0)
            ty += 0xFF;
        y = (ty >> 8) + body_a->ofsY;
        place_targets(body_a, x, y, 0, -1);
        tz = body_a->targetZ;
        if (tz < 0)
            tz += 0xFF;
        body_a->height = (tz >> 8) + 0x32;
        set_motion_offsets(body_a, 0x800);
    }
    if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
    {
        sprite_set_action(body_a, 3, 0x2030, 0);
        ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
    }
    body->state = hero_a_wait_reach_rate;
}

NONMATCH("asm/code_080A814C/hero_a_wait_reach_rate.s", void hero_a_wait_reach_rate(struct HeroBody *body))
{
    struct Hero *hero_a = gWorld->heroA;
    struct HeroBody *body_a = &hero_a->body;
    s32 reach;
    u16 rate;
    u32 distLo;
    u32 distHi;
    s32 reached;

    step_motion(body_a);
    distLo = hero_a->body.distLo;
    distHi = hero_a->body.distHi;
    reach = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(distHi << 14, distLo);
    reached = body_a->step <= reach && body_a->progress >= reach;
    if (reached != 0)
    {
        set_slot_value(0x62, -1);
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0xC);
            body_a->rate = rate;
            body_a->progress = 0;
            ((struct HeroFlagBits *)&body_a->flags)->active = 0;
        }
        body->state = hero_a_active_place_partner_z;
    }
}
END_NONMATCH

void hero_a_active_place_partner_z(struct HeroBody *body)
{
    struct Hero *hero_a = gWorld->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct HeroBody *partner = body->partner;
    s32 tx;
    s32 ty;
    s32 tz;
    s32 x;
    s32 y;
    s32 z;

    state_stack_advance_progress(body_a);
    if ((hero_a->body.flags & 0x20) != 0)
    {
        if ((hero_a->body.mode & 6) == 2 || (hero_a->body.mode & 6) == 4)
        {
            tz = body_a->posZ;
            if (tz < 0)
                tz += 0xFF;
            z = tz >> 8;
            tx = partner->targetX;
            if (tx < 0)
                tx += 0xFF;
            x = (tx >> 8) + partner->ofsX;
            ty = partner->targetY;
            if (ty < 0)
                ty += 0xFF;
            y = (ty >> 8) + partner->ofsY;
            place_targets(body_a, x, y, z, -1);
            {
                u16 h = 0x28;
                body_a->height = h;
            }
            set_motion_offsets(body_a, 0x533);
        }
        body_a->timer = body_a->timer & 0xFFF;
        body->state = hero_a_step_reach_input;
    }
}

ASM_FUNC("asm/code_080A814C/hero_a_step_reach_input.s", void hero_a_step_reach_input(struct HeroBody *body))

void hero_a_active_place_partner_aux(struct HeroBody *body)
{
    struct Hero *hero = gWorld->heroA;
    struct HeroBody *body_a = &hero->body;
    struct HeroBody *partner = body->partner;
    s32 tx;
    s32 ty;
    s32 tz;
    s32 x;
    s32 y;
    s32 z;

    state_stack_advance_progress(body_a);
    if ((hero->body.flags & 0x20) != 0)
    {
        if ((hero->body.mode & 6) == 2 || (hero->body.mode & 6) == 4)
        {
            tx = partner->targetX;
            if (tx < 0)
                tx += 0xFF;
            x = (tx >> 8) + partner->ofsX;
            ty = partner->targetY;
            if (ty < 0)
                ty += 0xFF;
            y = (ty >> 8) + partner->ofsY;
            tz = partner->targetZ;
            if (tz < 0)
                tz += 0xFF;
            z = (tz >> 8) + partner->ofsZ;
            place_targets(body_a, x, y, z, -1);
            set_motion_speed_aux(body_a, 0, 0xC0);
        }
        body_a->timer = body_a->timer & 0xFFF;
        body->state = hero_a_timing_window_9804;
    }
}

ASM_FUNC("asm/code_080A814C/hero_a_timing_window_9804.s", void hero_a_timing_window_9804(struct HeroBody *body))
