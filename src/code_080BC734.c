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
    s16 frame;
    s16 progress;
    u8 filler80[0x84 - 0x80];
    struct HeroSpan span;
    u8 filler88[0x92 - 0x88];
    u16 height;
    u8 filler94[0xA4 - 0x94];
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
    u8 filler0D[0x70 - 0x0D];
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

void hero_pair_b_follow_partner(struct HeroBody *body);
void hero_b_active_place_action(struct HeroBody *body);
void hero_a_place_pose_finish(struct HeroBody *body);
void hero_b_fx_and_end(struct HeroBody *body);
void hero_b_step_window_bcd28(struct HeroBody *body);
void hero_a_sprite_place_pose6(struct HeroBody *body);
void hero_a_slot_pose_anim(struct HeroBody *body);
void hero_b_step_place_pose_finish(struct HeroBody *body);
void hero_a_press_link_080bdbb0(struct HeroBody *body);
void hero_a_place_at_partner_rate(struct HeroBody *owner);
void hero_a_wait_visible_anim_33(struct HeroBody *body);
void hero_a_active_slot_pose_anim(struct HeroBody *body);
void body_set_pose(u32 art, struct HeroBody *body, s32 x, s32 y);
void obj_free_parts(struct HeroBody *body);
void display_flush(void);
void obj_stop(struct HeroBody *obj);
void set_motion_speed_aux(struct HeroBody *body, u32 speed, u32 aux);
void step_motion_scaled(struct HeroBody *body);
void set_body_part(struct HeroBody *body, u32 art, s32 x, s32 y, u32 extra);
s32 obj_last_x(void *obj);
s32 obj_last_y(void *obj);

ASM_FUNC("asm/code_080BC734/hero_a_z_cross_link_fx.s", void hero_a_z_cross_link_fx(void))

void hero_pair_place_finish_a_sprite(void)
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
        if ((hero_b->body.mode & 6) == 2 || (hero_b->body.mode & 6) == 4)
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

ASM_FUNC("asm/code_080BC734/hero_b_fx_anim_reset.s", void hero_b_fx_anim_reset(void))

void hero_b_active_place_action(struct HeroBody *body)
{
    struct Hero *hero_b;
    struct HeroBody *body_b;
    s32 tx;
    s32 ty;
    s32 x;
    s32 y;
    u16 height;

    hero_b = gUnk_03000FD8->heroB;
    body_b = &hero_b->body;
    if ((body_b->sprite->flags & 8) != 0)
    {
        set_slot_value(0x53, -1);
        if ((hero_b->body.mode & 6) == 2 || (hero_b->body.mode & 6) == 4)
        {
            tx = body_b->targetX;
            if (tx < 0)
                tx += 0xFF;
            x = (tx >> 8) + hero_b->body.ofsX - 0x20;
            ty = body_b->targetY;
            if (ty < 0)
                ty += 0xFF;
            y = (ty >> 8) + hero_b->body.ofsY;
            place_targets(body_b, x, y, 0, -1);
            height = 0x18;
            hero_b->body.height = height;
            set_motion_offsets(body_b, 0x400);
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            body_b->state = hero_b_fx_and_end;
            start_action_5(body_b);
        }
    }
}

void hero_a_ready_place_mode(struct HeroBody *body)
{
    struct Hero *hero;
    struct HeroBody *body_a;
    u8 mode;
    s32 tx;
    s32 ty;
    s32 x;
    s32 y;
    u16 height;

    mode = gUnk_03000FD8->modeByte;
    gUnk_03000FD8->modeByte = mode | 0xC0;
    hero = gUnk_03000FD8->heroA;
    body_a = &hero->body;
    if ((hero->body.mode & 6) == 2 || (hero->body.mode & 6) == 4)
    {
        tx = body_a->targetX;
        if (tx < 0)
            tx += 0xFF;
        x = (tx >> 8) + body_a->ofsX;
        ty = body_a->targetY;
        if (ty < 0)
            ty += 0xFF;
        y = (ty >> 8) + body_a->ofsY;
        place_targets(body_a, x, y, 0x17, -1);
        height = 0x18;
        hero->body.height = height;
        set_motion_offsets(body_a, 0x300);
    }
    body->state = hero_pair_b_follow_partner;
}

void hero_pair_b_follow_partner(struct HeroBody *body)
{
    struct Hero *hero_a;
    struct HeroBody *body_a;
    struct HeroBody *body_b;
    struct HeroBody *partner;
    s32 tx;
    s32 ty;
    s32 x;
    s32 y;

    hero_a = gUnk_03000FD8->heroA;
    body_a = &hero_a->body;
    body_b = &gUnk_03000FD8->heroB->body;
    partner = body->partner;
    step_motion(body_a);
    if ((hero_a->body.flags & 0x20) != 0)
    {
        if ((hero_a->body.mode & 6) == 2 || (hero_a->body.mode & 6) == 4)
        {
            sprite_set_action(body_a, 1, 0x2024, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            tx = partner->targetX;
            if (tx < 0)
                tx += 0xFF;
            x = (tx >> 8) + partner->ofsX + 0x18;
            ty = partner->targetY;
            if (ty < 0)
                ty += 0xFF;
            y = (ty >> 8) + partner->ofsY;
            place_targets(body_b, x, y, 0, -1);
            set_motion_start(body_b, 0x166);
        }
        body_b->timer = body_b->timer & 0xFFF;
        body->state = hero_b_step_window_bcd28;
    }
}

ASM_FUNC("asm/code_080BC734/hero_b_step_window_bcd28.s", void hero_b_step_window_bcd28(struct HeroBody *body))

void hero_a_link_partner_fx(struct HeroBody *body)
{
    struct Hero *hero_a;
    struct HeroBody *body_a;
    struct HeroBody *partner;
    s32 tx;
    s32 ty;
    s32 x;
    s32 y;

    hero_a = gUnk_03000FD8->heroA;
    body_a = &hero_a->body;
    partner = body->partner;
    state_stack_advance_progress(body_a);
    if ((hero_a->body.flags & 0x20) != 0)
    {
        set_slot_value(0x7E, -1);
        ((struct HeroCcBits *)&hero_a->body.cc)->low = 0;
        body_a->scale = 0x100;
        resolve_contact_action(body_a, partner, 0, 0, 0x0403238A);
        body_a->count = body_a->count + 1;
        if (partner->data->unkF6 > 0 || (partner->data->flags111 & 2) != 0)
        {
            tx = partner->targetX;
            if (tx < 0)
                tx += 0xFF;
            x = (tx >> 8) + partner->ofsX;
            ty = partner->targetY;
            if (ty < 0)
                ty += 0xFF;
            y = (ty >> 8) + partner->ofsY;
            effect_spawn_at(0, x, y, 0, partner);
        }
        body->state = hero_a_sprite_place_pose6;
    }
}

void hero_a_sprite_place_pose6(struct HeroBody *body)
{
    struct Hero *hero_a;
    struct HeroBody *body_a;
    s32 tx;
    s32 ty;
    s32 tz;
    s32 x;
    s32 y;

    hero_a = gUnk_03000FD8->heroA;
    body_a = &hero_a->body;
    if ((body_a->sprite->flags & 8) != 0)
    {
        if ((hero_a->body.mode & 6) == 2 || (hero_a->body.mode & 6) == 4)
        {
            tx = body_a->targetX;
            if (tx < 0)
                tx += 0xFF;
            x = (tx >> 8) + hero_a->body.ofsX - 0x20;
            ty = body_a->targetY;
            if (ty < 0)
                ty += 0xFF;
            y = (ty >> 8) + hero_a->body.ofsY;
            place_targets(body_a, x, y, 0, -1);
            tz = body_a->targetZ;
            if (tz < 0)
                tz += 0xFF;
            hero_a->body.height = (tz >> 8) + 2;
            set_motion_offsets(body_a, 0x300);
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            if (body_a->data->poseIndex == -1)
                sprite_set_action(body_a, 6, 0x2000, 0);
            else
                sprite_set_action(body_a, 6, 0x204D, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
        body->state = hero_a_slot_pose_anim;
    }
}

void hero_a_slot_pose_anim(struct HeroBody *body)
{
    struct Hero *hero_a;
    struct HeroBody *body_a;

    hero_a = gUnk_03000FD8->heroA;
    body_a = &hero_a->body;
    step_motion(body_a);
    if ((hero_a->body.flags & 0x20) != 0)
    {
        set_slot_value(0x2E, -1);
        if ((hero_a->body.mode & 6) == 2 || (hero_a->body.mode & 6) == 4)
        {
            if (body_a->data->poseIndex == -1)
                sprite_set_action(body_a, 8, 0x2000, 0);
            else
                sprite_set_action(body_a, 8, 0x204D, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
        body->state = hero_a_place_pose_finish;
    }
}

void hero_a_place_pose_finish(struct HeroBody *body)
{
    struct Hero *hero_a;
    struct HeroBody *body_a;
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

ASM_FUNC("asm/code_080BC734/hero_b_place_last_pose.s", void hero_b_place_last_pose(void))

void hero_b_step_place_pose_finish(struct HeroBody *body)
{
    struct Hero *hero_b;
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

    hero_b = gUnk_03000FD8->heroB;
    body_b = &hero_b->body;
    step_motion(body_b);
    if ((hero_b->body.flags & 0x20) != 0)
    {
        body_b->x = body_b->x + 0xFFFED000;
        body_b->targetX = body_b->x;
        if ((hero_b->body.mode & 6) == 2 || (hero_b->body.mode & 6) == 4)
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

void hero_a_link_partner_anim(struct HeroBody *owner)
{
    struct Hero *hero_a;
    struct HeroBody *body_a;
    struct HeroBody *partner;
    s32 tx;
    s32 ty;
    s32 x;
    s32 y;

    hero_a = gUnk_03000FD8->heroA;
    body_a = &hero_a->body;
    partner = owner->partner;
    ((struct HeroFlag111Bits *)&partner->data->flags111)->bit1 = 1;
    ((struct HeroCcBits *)&hero_a->body.cc)->low = 0;
    body_a->scale = 0x100;
    resolve_contact_action(body_a, partner, 0, 0, 0x0403248A);
    body_a->count = body_a->count + 1;
    if (partner->data->unkF6 > 0 || (partner->data->flags111 & 2) != 0)
    {
        tx = partner->targetX;
        if (tx < 0)
            tx += 0xFF;
        x = (tx >> 8) + partner->ofsX;
        ty = partner->targetY;
        if (ty < 0)
            ty += 0xFF;
        y = (ty >> 8) + partner->ofsY;
        effect_spawn_at(0x733, x, y, 0x10, partner);
    }
    body_a->unkA8 = body_a->progress + 0x28000;
    body_a->unkA4 = 0;
    if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
    {
        sprite_set_action(body_a, 1, 0x2033, 0);
        ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
    }
    set_body_part(body_a, 1, 0x18, 4, 0);
    owner->state = hero_a_place_at_partner_rate;
}

void hero_a_place_at_partner_rate(struct HeroBody *owner)
{
    struct Hero *hero_a;
    struct HeroBody *body_a;
    struct HeroBody *partner;
    s32 tx;
    s32 ty;
    s32 tz;
    s32 x;
    s32 y;
    s32 z;

    hero_a = gUnk_03000FD8->heroA;
    body_a = &hero_a->body;
    partner = owner->partner;
    if ((hero_a->body.mode & 6) == 2 || (hero_a->body.mode & 6) == 4)
    {
        tx = body_a->targetX;
        if (tx < 0)
            tx += 0xFF;
        x = tx >> 8;
        ty = body_a->targetY;
        if (ty < 0)
            ty += 0xFF;
        y = ty >> 8;
        tz = partner->targetZ;
        if (tz < 0)
            tz += 0xFF;
        z = tz >> 8;
        place_targets(body_a, x, y, z, -1);
        set_motion_start(body_a, 0x400);
    }
    if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
    {
        body_a->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0x32);
        body_a->progress = 0;
        ((struct HeroFlagBits *)&body_a->flags)->active = 0;
    }
    set_body_part(body_a, 1, 0x18, 4, 0);
    owner->state = hero_a_press_link_080bdbb0;
}

ASM_FUNC("asm/code_080BC734/hero_a_press_link_080bdbb0.s", void hero_a_press_link_080bdbb0(struct HeroBody *body))
