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

struct HeroFlag111Bits
{
    u8 unk0 : 1;
    u8 bit1 : 1;
    u8 unk2 : 6;
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
    s32 x;
    s32 y;
    s32 z;
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
    u16 unk7C;
    s16 progress;
    u8 filler80[0x84 - 0x80];
    struct HeroSpan span;
    u8 filler88[0x92 - 0x88];
    u16 height;
    u32 wideLo;
    u32 wideHi;
    u8 filler9C[0xA4 - 0x9C];
    s32 unkA4;
    s32 unkA8;
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

struct WorldNode
{
    struct WorldNode *next;
    u8 filler4[0x08 - 0x04];
    void *obj;
};

struct World
{
    u8 filler0[0x0A];
    u8 modeByte;
    u8 filler0B;
    u8 flags0C;
    u8 filler0D;
    u8 unk0E;
    u8 filler0F[0x70 - 0x0F];
    struct Hero *heroA;
    struct Hero *heroB;
    struct WorldNode *nodes78;
    u8 filler7C[0x24C - 0x7C];
    void *focus24C;
};

struct KeyState
{
    u8 filler0[0x3E];
    u16 pressed;
};

extern struct World *gUnk_03000FD8;
extern u8 *gUnk_03000FF4;
extern struct KeyState gUnk_0300034C;

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
void body_set_pose(u32 art, struct HeroBody *body, s32 x, s32 y);
void obj_free_parts(struct HeroBody *body);
void display_flush(void);
void obj_stop(struct HeroBody *obj);
void set_motion_speed_aux(struct HeroBody *body, u32 speed, u32 aux);
void step_motion_scaled(struct HeroBody *body);
void set_body_part(struct HeroBody *body, u32 art, s32 x, s32 y, u32 extra);
s32 obj_last_x(void *obj);
s32 obj_last_y(void *obj);

void hero_a_rate_wait(struct HeroBody *body);
void hero_a_rate_anim(struct HeroBody *body);
void hero_b_active_anim_pair(struct HeroBody *body);
void hero_pair_place_finish_ab(struct HeroBody *body);
void hero_b_slot_anim_rate_next(struct HeroBody *body);
void hero_b_step_active_to_176c(struct HeroBody *body);
void enter_finish_flag(struct HeroBody *body);
void hero_b_anim_a_and_rate(struct HeroBody *body);
void hero_b_pair_step_1948(struct HeroBody *body);
void hero_a_follow_partner_target(struct HeroBody *body);
void hero_b_active_set_a_step_rate(struct HeroBody *body);
void hero_a_step_c2850(struct HeroBody *body);
void hero_pair_place_and_finish(struct HeroBody *body);
void hero_a_wide_step_finish(struct HeroBody *body);
void visible_place_hero_a_active_link_partner(struct HeroBody *body);
void hero_a_visible_place_on_target(struct HeroBody *body);
void hero_a_sprite_active_anim8_to_2520(struct HeroBody *body);

void hero_a_active_anim_24(struct HeroBody *body)
{
    struct Hero *hero_a;
    struct HeroBody *body_a;

    hero_a = gUnk_03000FD8->heroA;
    body_a = &hero_a->body;
    state_stack_advance_progress(body_a);
    if ((hero_a->body.flags & 0x20) != 0)
    {
        set_slot_value(0xAF, -1);
        if ((hero_a->body.mode & 6) == 2 || (hero_a->body.mode & 6) == 4)
        {
            sprite_set_action(body_a, 0, 0x2024, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
        body->state = hero_a_rate_anim;
    }
}

void hero_a_active_anim_none(struct HeroBody *body)
{
    struct Hero *hero_a;
    struct HeroBody *body_a;

    hero_a = gUnk_03000FD8->heroA;
    body_a = &hero_a->body;
    step_motion(body_a);
    if ((hero_a->body.flags & 0x20) != 0)
    {
        if ((hero_a->body.mode & 6) == 2 || (hero_a->body.mode & 6) == 4)
        {
            sprite_set_action(body_a, 0, -1, 0);
            ((struct HeroMirrorBits *)&body_a->sprite->mirror)->flip = 0;
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        body->state = hero_a_rate_wait;
    }
}

void owner_take_link_state(struct HeroBody *body)
{
    s32 link;

    link = body->unkA8;
    if (link == 0)
    {
        ((struct HeroFlag111Bits *)&body->data->flags111)->bit1 = 0;
        if (body->data->unkF6 == 0)
            obj_stop(body);
        else
            body->state = (void (*)(struct HeroBody *))link;
    }
}

u32 hero_pair_setup(struct HeroBody *body)
{
    struct HeroData *data;
    struct Hero *hero_a;
    struct Hero *hero_b;
    struct HeroBody *body_a;
    struct HeroBody *body_b;
    struct HeroBody *partner;
    void *anim;
    u8 **party;
    u8 *entry;
    s32 pose;

    party =&gUnk_03000FF4;
    data = body->data;
    pose = data->poseIndex;
    entry = *party - ((((pose << 4) - pose) << 2) + 0x3C);
    data->poseValue = *(u16 *)(entry + 0x2A);
    hero_a = gUnk_03000FD8->heroA;
    body_a = &hero_a->body;
    hero_b = gUnk_03000FD8->heroB;
    body_b = &hero_b->body;
    partner = body->partner;
    body_b->partner = partner;
    body_a->partner = partner;
    anim = body->anim;
    body_b->anim = anim;
    body_a->anim = anim;
    partner->unk50 = 0;
    body_b->unk50 = 0;
    body_a->unk50 = 0;
    partner->unk7C = 0x100;
    body_b->unk7C = 0x100;
    body_a->unk7C = 0x100;
    body->state = hero_pair_place_finish_ab;
    body_b->count = 0;
    body_a->count = 0;
    schedule_pose_task(partner);
    return 1;
}

ASM_FUNC("asm/code_080C1420/hero_pair_place_finish_ab.s", void hero_pair_place_finish_ab(struct HeroBody *body))

void hero_b_set_rate_if_idle(struct HeroBody *body)
{
    struct Hero *hero_a;
    struct Hero *hero_b;
    struct HeroBody *body_a;
    struct HeroBody *body_b;
    s32 state;

    hero_a = gUnk_03000FD8->heroA;
    body_a = &hero_a->body;
    hero_b = gUnk_03000FD8->heroB;
    body_b = &hero_b->body;
    state = (s32)body_a->state;
    if (state == 0)
    {
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            body_b->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0x10);
            body_b->progress = state;
            ((struct HeroFlagBits *)&body_b->flags)->active = 0;
        }
        body->state = hero_b_active_anim_pair;
    }
}

void hero_b_active_anim_pair(struct HeroBody *body)
{
    struct Hero *hero_a;
    struct Hero *hero_b;
    struct HeroBody *body_a;
    struct HeroBody *body_b;

    hero_a = gUnk_03000FD8->heroA;
    body_a = &hero_a->body;
    hero_b = gUnk_03000FD8->heroB;
    body_b = &hero_b->body;
    state_stack_advance_progress(body_b);
    if ((hero_b->body.flags & 0x20) != 0)
    {
        set_slot_value(0xAF, -1);
        if ((hero_b->body.mode & 6) == 2 || (hero_b->body.mode & 6) == 4)
        {
            sprite_set_action(body_b, 1, 0x2063, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            sprite_set_action(body_a, 0, -1, 0);
            ((struct HeroMirrorBits *)&body_a->sprite->mirror)->flip = 0;
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        body->state = hero_b_anim_a_and_rate;
    }
}

void hero_b_anim_a_and_rate(struct HeroBody *body)
{
    struct Hero *hero_b;
    struct HeroBody *body_b;

    hero_b = gUnk_03000FD8->heroB;
    body_b = &hero_b->body;
    if ((body_b->sprite->flags & 8) != 0)
    {
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            sprite_set_action(body_b, 0xA, 0x2063, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 0;
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            body_b->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0x14);
            body_b->progress = 0;
            ((struct HeroFlagBits *)&body_b->flags)->active = 0;
        }
        body_b->timer = body_b->timer & 0xFFF;
        body->state = hero_b_pair_step_1948;
    }
}

ASM_FUNC("asm/code_080C1420/hero_b_pair_step_1948.s", void hero_b_pair_step_1948(struct HeroBody *body))

void hero_a_active_fx_anim_next(struct HeroBody *body)
{
    struct Hero *hero_a;
    struct Hero *hero_b;
    struct HeroBody *body_a;
    struct HeroBody *body_b;

    hero_a = gUnk_03000FD8->heroA;
    body_a = &hero_a->body;
    hero_b = gUnk_03000FD8->heroB;
    body_b = &hero_b->body;
    state_stack_advance_progress(body_b);
    if ((hero_b->body.flags & 0x20) != 0)
    {
        set_slot_value(0x7E, -1);
        if ((hero_a->body.mode & 6) == 2 || (hero_a->body.mode & 6) == 4)
        {
            sprite_set_action(body_a, 9, 0x2034, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
        effect_spawn_at(0, body_a->x / 0x100, body_a->y / 0x100, body_a->z / 0x100, body_a);
        body->state = hero_b_slot_anim_rate_next;
    }
}

void hero_b_slot_anim_rate_next(struct HeroBody *body)
{
    struct Hero *hero_b;
    struct HeroBody *body_b;

    hero_b = gUnk_03000FD8->heroB;
    body_b = &hero_b->body;
    if ((body_b->sprite->flags & 8) != 0)
    {
        set_slot_value(0xAF, -1);
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            sprite_set_action(body_b, 7, 0x2063, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            body_b->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0x23);
            body_b->progress = 0;
            ((struct HeroFlagBits *)&body_b->flags)->active = 0;
        }
        body->state = hero_a_follow_partner_target;
    }
}

void hero_a_follow_partner_target(struct HeroBody *body)
{
    struct Hero *hero_a;
    struct Hero *hero_b;
    struct HeroBody *body_a;
    struct HeroBody *body_b;
    struct HeroBody *partner;

    hero_a = gUnk_03000FD8->heroA;
    body_a = &hero_a->body;
    hero_b = gUnk_03000FD8->heroB;
    body_b = &hero_b->body;
    partner = body->partner;
    state_stack_advance_progress(body_b);
    if ((hero_b->body.flags & 0x20) != 0)
    {
        set_slot_value(0x7E, -1);
        effect_spawn_at(0, body_a->x / 0x100, body_a->y / 0x100, body_a->z / 0x100, body_a);
        if ((hero_a->body.mode & 6) == 2 || (hero_a->body.mode & 6) == 4)
        {
            place_targets(body_a, partner->targetX / 0x100 + partner->ofsX - 0xA,
                          partner->targetY / 0x100 + partner->ofsY, 0, -1);
            set_motion_start(body_a, 0x400);
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            sprite_set_action(body_a, 0xA, 0x2034, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        body->state = visible_place_hero_a_active_link_partner;
    }
}

void visible_place_hero_a_active_link_partner(struct HeroBody *body)
{
    struct Hero *hero_a;
    struct HeroBody *body_a;
    struct HeroBody *partner;
    s32 x;
    s32 y;

    hero_a = gUnk_03000FD8->heroA;
    body_a = &hero_a->body;
    partner = body->partner;
    step_motion(body_a);
    if ((hero_a->body.flags & 0x20) != 0)
    {
        set_slot_value(0x63, -1);
        ((struct HeroCcBits *)&body_a->cc)->low = 0;
        body_a->scale = 0x100;
        resolve_contact_action(body_a, partner, 0, 0, 0x0803008B);
        body_a->count = body_a->count + 1;
        if (partner->data->unkF6 > 0 || (partner->data->flags111 & 2) != 0)
        {
            x = partner->targetX / 0x100 + partner->ofsX + 4;
            y = partner->targetY / 0x100 + partner->ofsY;
            effect_spawn_at(0x6E3, x, y, 0x10, partner);
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            sprite_set_action(body_a, 0xB, 0x2034, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
        body->state = hero_a_visible_place_on_target;
    }
}

void hero_a_visible_place_on_target(struct HeroBody *body)
{
    struct Hero *hero_a;
    struct HeroBody *body_a;

    hero_a = gUnk_03000FD8->heroA;
    body_a = &hero_a->body;
    if ((body_a->sprite->flags & 8) != 0)
    {
        if (body_a->targetZ == 0)
        {
            if ((hero_a->body.mode & 6) == 2 || (hero_a->body.mode & 6) == 4)
                body_a->state = hero_pair_place_and_finish;
        }
        else
        {
            if ((hero_a->body.mode & 6) == 2 || (hero_a->body.mode & 6) == 4)
            {
                place_targets(body_a, body_a->targetX / 0x100 + body_a->ofsX - 0x3A,
                              body_a->targetY / 0x100 + body_a->ofsY, 0, -1);
                body_a->height = body_a->targetZ / 0x100 + 8;
                set_motion_offsets(body_a, 0x500);
            }
            body_a->timer = body_a->timer & 0xFFF;
            body->state = hero_a_wide_step_finish;
        }
    }
}

ASM_FUNC("asm/code_080C1420/hero_a_wide_step_finish.s", void hero_a_wide_step_finish(struct HeroBody *body))

void hero_pair_place_and_finish(struct HeroBody *body)
{
    struct Hero *hero_a;
    struct Hero *hero_b;
    struct HeroBody *body_a;
    struct HeroBody *body_b;
    struct HeroData *data;
    struct HeroSprite *sprite;
    struct HeroSpan *span;
    s32 tx;
    s32 ty;
    s32 th;
    s32 x;
    s32 y;
    s32 h;

    hero_a = gUnk_03000FD8->heroA;
    body_a = &hero_a->body;
    hero_b = gUnk_03000FD8->heroB;
    body_b = &hero_b->body;
    if ((body_a->sprite->flags & 8) != 0)
    {
        if ((hero_a->body.mode & 6) == 2 || (hero_a->body.mode & 6) == 4)
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
    }
}

void hero_b_set_step_rate_and_release(struct HeroBody *body)
{
    struct Hero *hero_b;
    struct HeroBody *body_b;

    hero_b = gUnk_03000FD8->heroB;
    body_b = &hero_b->body;
    if ((hero_b->body.mode & 6) == 2 || (hero_b->body.mode & 6) == 4)
    {
        sprite_set_action(body_b, 2, 0x2063, 0);
        ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
    }
    if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
    {
        body_b->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 2);
        body_b->progress = 0;
        ((struct HeroFlagBits *)&body_b->flags)->active = 0;
    }
    body->state = hero_b_active_set_a_step_rate;
}

void hero_b_active_set_a_step_rate(struct HeroBody *body)
{
    struct Hero *hero_a;
    struct Hero *hero_b;
    struct HeroBody *body_a;
    struct HeroBody *body_b;

    hero_a = gUnk_03000FD8->heroA;
    body_a = &hero_a->body;
    hero_b = gUnk_03000FD8->heroB;
    body_b = &hero_b->body;
    state_stack_advance_progress(body_b);
    if ((hero_b->body.flags & 0x20) != 0)
    {
        set_slot_value(0x7C, -1);
        if ((hero_a->body.mode & 6) == 2 || (hero_a->body.mode & 6) == 4)
        {
            body_a->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0x18);
            body_a->progress = 0;
            ((struct HeroFlagBits *)&body_a->flags)->active = 0;
        }
        body_a->timer = body_a->timer & 0xFFF;
        body->state = hero_a_step_c2850;
    }
}

ASM_FUNC("asm/code_080C1420/hero_a_step_c2850.s", void hero_a_step_c2850(struct HeroBody *body))
