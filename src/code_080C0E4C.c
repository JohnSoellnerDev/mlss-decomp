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

struct HeroData
{
    u8 filler0[0xD8];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler0E4[0xEC - 0xE4];
    s16 poseIndex;
    u8 filler0EE[0x111 - 0xEE];
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
    u16 rate;
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
    u8 filler78[0x2BF - 0x78];
    u8 byte2BF;
};

extern struct World *gUnk_03000FD8;
extern u8 gUnk_0300034C[];
extern const s8 gHandOverJitterStart[];
extern const s8 gHandOverDriftRange[];
extern const u8 gHandOverLimitTable[];

void state_stack_advance_progress(struct HeroBody *body);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void set_slot_value(u32 id, s32 value);
void step_motion(struct HeroBody *body);
void set_cur_flags_both(u32 value);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void enter_finish_rebuild(struct HeroBody *body);
void set_motion_start(struct HeroBody *body, u32 speed);
void set_motion_offsets(struct HeroBody *body, u32 offsets);
void start_action_5(struct HeroBody *body);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 h, struct HeroBody *owner);
void schedule_pose_task(struct HeroBody *obj);
void resolve_contact_action(struct HeroBody *a, struct HeroBody *b, u32 c, u32 d, u32 e);
u32 mt19937_next(void);
s32 sub_081DE850(s32 n, u32 seed);
void owner_take_link_state(struct HeroBody *body);
void body_set_pose(u32 art, struct HeroBody *body, u32 x, u32 y);
void hero_b_shift_place_pose_next(struct HeroBody *body);
void hero_b_place_last_pose(struct HeroBody *body);
void hero_a_join_partner_next(struct HeroBody *body);
void hero_a_sprite_place_partner(struct HeroBody *body);
void hero_pair_place_finish_a_sprite(struct HeroBody *body);
void hero_b_sprite_place_mirror(struct HeroBody *body);
void hero_pair_place_finish_b_sprite(struct HeroBody *body);

void hero_a_hand_over(struct HeroBody *body);
void hero_b_after_hand_over(struct HeroBody *body);
void hero_a_active_setup(struct HeroBody *body);
void hero_b_take_a_position(struct HeroBody *body);
void hero_b_visible_place(struct HeroBody *body);
void link_partner_and_hero_b(struct HeroBody *body);
void hero_b_follow_partner(struct HeroBody *body);
void hero_b_step_and_release(struct HeroBody *body);
void hero_pair_timer_step_06f0(struct HeroBody *body);
void hero_a_visible_place_finish(struct HeroBody *body);
void hero_b_active_place_and_finish(struct HeroBody *body);
void hero_b_start_anim_f(struct HeroBody *body);
void step_end_and_partner_next(struct HeroBody *body);
void hero_b_wait_visible_anim_e(struct HeroBody *body);
void hero_b_fx_and_end(struct HeroBody *body);
void hero_a_wait_visible_anim_33(struct HeroBody *body);
void hero_b_anim_c_then_clear(struct HeroBody *body);
void hero_a_wait_visible_anim_24(struct HeroBody *body);
void hero_b_active_fx(struct HeroBody *body);

void hero_a_visible_place_finish(struct HeroBody *body)
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

void hero_b_active_place_and_finish(struct HeroBody *body)
{
    struct HeroBody *body_a = &gUnk_03000FD8->heroA->body;
    struct Hero *hero_b = gUnk_03000FD8->heroB;
    struct HeroBody *body_b = &hero_b->body;
    struct HeroBody *partner = body->partner;
    struct HeroData *data;
    struct HeroSprite *sprite;
    struct HeroSpan *span;
    s32 tx;
    s32 ty;
    s32 th;
    s32 x;
    s32 y;
    s32 h;

    step_motion(body_b);
    if ((hero_b->body.flags & 0x20) != 0)
    {
        set_slot_value(0x2E, -1);
        partner->link = 0;
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

void hero_b_start_anim_f(struct HeroBody *body)
{
    struct HeroBody *body_b = &gUnk_03000FD8->heroB->body;

    if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
    {
        sprite_set_action(body_b, 0xF, 0x2062, 0);
        ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
    }
    body->state = hero_b_shift_place_pose_next;
}

void step_end_and_partner_next(struct HeroBody *body)
{
    struct HeroBody *partner = body->partner;

    step_motion(body);
    if ((body->flags & 0x20) != 0)
    {
        body->state = 0;
        partner->state = owner_take_link_state;
    }
}

void hero_b_wait_visible_anim_e(struct HeroBody *body)
{
    struct HeroBody *body_b = &gUnk_03000FD8->heroB->body;

    if ((body_b->sprite->flags & 8) != 0)
    {
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            sprite_set_action(body_b, 0xE, 0x2062, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 0;
        }
        body->state = hero_b_place_last_pose;
    }
}

void hero_b_fx_and_end(struct HeroBody *body)
{
    struct HeroBody *body_b = &gUnk_03000FD8->heroB->body;
    s32 x;
    s32 y;
    s32 z;

    set_slot_value(0x2E, -1);
    x = body_b->posX;
    if (x < 0)
        x += 0xFF;
    x = x >> 8;
    y = body_b->posY;
    if (y < 0)
        y += 0xFF;
    y = y >> 8;
    z = body_b->posZ;
    if (z < 0)
        z += 0xFF;
    z = z >> 8;
    effect_spawn_at(0x1E, x, y, z, body_b);
    body_b->state = 0;
}

void hero_a_wait_visible_anim_33(struct HeroBody *body)
{
    struct HeroBody *body_a = &gUnk_03000FD8->heroA->body;

    if ((body_a->sprite->flags & 8) != 0)
    {
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            sprite_set_action(body_a, 6, 0x2033, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        body->state = hero_pair_place_finish_a_sprite;
    }
}

void hero_b_anim_c_then_clear(struct HeroBody *body)
{
    struct HeroBody *body_b = &gUnk_03000FD8->heroB->body;

    set_slot_value(0x38, -1);
    if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
    {
        sprite_set_action(body_b, 0xC, 0x2062, 0);
        ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
    }
    body_b->unk74 = 0;
    body_b->unk75 = 0;
    body->state = hero_b_sprite_place_mirror;
}

void hero_a_wait_visible_anim_24(struct HeroBody *body)
{
    struct HeroBody *body_a = &gUnk_03000FD8->heroA->body;

    if ((body_a->sprite->flags & 8) != 0)
    {
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            sprite_set_action(body_a, 4, 0x2024, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
        body->state = hero_a_sprite_place_partner;
    }
}

void hero_b_active_fx(struct HeroBody *body)
{
    struct HeroBody *body_b = &gUnk_03000FD8->heroB->body;
    s32 x;
    s32 y;
    s32 z;

    step_motion(body_b);
    if ((body_b->flags & 0x20) != 0)
    {
        set_slot_value(0x2E, -1);
        x = body_b->posX;
        if (x < 0)
            x += 0xFF;
        x = x >> 8;
        y = body_b->posY;
        if (y < 0)
            y += 0xFF;
        y = y >> 8;
        z = body_b->posZ;
        if (z < 0)
            z += 0xFF;
        z = z >> 8;
        effect_spawn_at(0x1E, x, y, z, body_b);
        body->state = hero_pair_place_finish_b_sprite;
    }
}
