#include "global.h"
#include "ram_routines.h"

struct HeroSprite
{
    u8 filler0[0x11];
    u8 mirror;
    u8 flags;
    u8 filler13[0x20 - 0x13];
    u8 layer;
};

struct HeroLayer
{
    struct HeroLayer *next;
    u8 filler4[0x08 - 0x04];
    struct HeroSprite *sprite;
};

struct KeyState
{
    u8 filler0[0x3E];
    u16 pressed;
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

struct HeroData
{
    u8 filler0[0xD8];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler0E4[0xEC - 0xE4];
    s16 poseIndex;
    u8 filler0EE[0xF6 - 0xEE];
    s16 displayTimer;
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
    struct HeroBody *child;
    u8 filler34[0x38 - 0x34];
    s32 targetX;
    s32 targetY;
    s32 targetZ;
    u8 filler44[0x4C - 0x44];
    void (*state)(struct HeroBody *);
    u32 unk50;
    u8 filler54[0x6C - 0x54];
    void *anim;
    u8 filler70[0x76 - 0x70];
    u8 mode;
    u8 filler77[0x79 - 0x77];
    u8 flags;
    s16 rate;
    u16 frame;
    s16 progress;
    u8 filler80[0x84 - 0x80];
    struct HeroSpan span;
    u8 filler88[0x92 - 0x88];
    s16 height;
    u8 filler94[0xAC - 0x94];
    s16 prevProgress;
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
    u8 filler0B[0x0C - 0x0B];
    u8 view;
    u8 filler0D[0x58 - 0x0D];
    struct Hero *list[6];
    struct Hero *heroA;
    struct Hero *heroB;
    struct HeroLayer *layers;
};

extern struct World *gUnk_03000FD8;
extern u8 *gUnk_03000FF4;
extern struct KeyState gUnk_0300034C;
extern const u8 gRoom149Layer1[];

void hero_b_start_step_14th(struct HeroBody *body);
void hero_b_active_partner_anim_place(struct HeroBody *body);
void hero_b_step_window_d72e4(struct HeroBody *body);
void hero_b_link_list_objs(struct HeroBody *body);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void set_slot_value(u32 id, s32 value);
void obj_release(struct HeroBody *obj);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 z, struct HeroBody *owner);
void state_stack_advance_progress(struct HeroBody *body);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void set_motion_start(struct HeroBody *body, u32 speed);
void set_motion_offsets(struct HeroBody *body, u32 value);
void hero_b_sprite_idle_start_step10(struct HeroBody *body);
void hero_b_link_partner_child_fx(struct HeroBody *body);
void hero_pair_active_place_both(struct HeroBody *body);
void hero_b_step_d8164(struct HeroBody *body);
void hero_b_copy_pos(struct HeroBody *body);
void hero_b_active_anim_place(struct HeroBody *body);
void hero_pair_spawn_fx_anim_next(struct HeroBody *body);
void hero_b_active_place_at_partner(struct HeroBody *body);
void set_body_part(struct HeroBody *body, u32 art, s32 x, s32 y, u32 extra);
void body_set_pose(u32 art, struct HeroBody *body, s32 x, s32 y);
void obj_free_parts(struct HeroBody *body);
void display_flush(void);
void sprite_objects_draw_obj_tiles(u32 a, u32 b);
void hero_b_sprite_active_start_step16(struct HeroBody *body);
void step_motion(struct HeroBody *body);
void enter_finish_rebuild(struct HeroBody *body);
void resolve_contact_action(struct HeroBody *body, struct HeroBody *partner, u32 a, u32 b, const u8 *cb);
void hero_pair_active_place_a_finish(struct HeroBody *body);

void hero_b_remove_child_fx(struct HeroBody *body)
{
    struct HeroBody *body_b = &gUnk_03000FD8->heroB->body;

    set_slot_value(0x15E, -1);
    if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
    {
        sprite_set_action(body_b, 8, 0x2066, 0);
        ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
    }
    if (body_b->child != 0)
    {
        sprite_set_action(body_b->child, 8, 0x2095, 0);
        ((struct HeroSpriteBits *)&body_b->child->sprite->flags)->mode = 0;
    }
    obj_release(body_b->child);
    effect_spawn_at(0x69F, body_b->posX / 0x100, body_b->posY / 0x100, body_b->posZ / 0x100, body_b);
    body->state = hero_b_start_step_14th;
}

void hero_b_start_step_14th(struct HeroBody *body)
{
    struct Hero *hero = gUnk_03000FD8->heroB;

    if ((hero->body.sprite->flags & 8) != 0)
    {
        if ((hero->body.mode & 6) == 2 || (hero->body.mode & 6) == 4)
        {
            hero->body.rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0xE);
            hero->body.progress = 0;
            ((struct HeroFlagBits *)&hero->body.flags)->active = 0;
        }
        body->state = hero_b_link_list_objs;
    }
}

void hero_b_link_list_objs(struct HeroBody *body)
{
    struct HeroBody *body_b = &gUnk_03000FD8->heroB->body;
    struct HeroBody *list_body;
    s32 i;

    state_stack_advance_progress(body_b);
    if ((body_b->flags & 0x20) != 0)
    {
        for (i = 0; i < 6; i++)
        {
            list_body = &gUnk_03000FD8->list[i]->body;
            if ((list_body->mode & 6) == 4)
            {
                if ((list_body->data->flags110 & 0x20) == 0)
                {
                    ((struct HeroCcBits *)&body_b->cc)->low = 0x55;
                    body_b->scale = 0xC0;
                    resolve_contact_action(body_b, list_body, 0, 0, gRoom149Layer1 + 0x30D);
                    body_b->count++;
                    if (list_body->data->displayTimer > 0 || (list_body->data->flags111 & 2) != 0)
                        effect_spawn_at(0, list_body->targetX / 0x100 + list_body->ofsX,
                                        list_body->targetY / 0x100 + list_body->ofsY, 0, list_body);
                }
            }
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            sprite_set_action(body_b, 9, 0x2066, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 0;
        }
        if (body_b->child != 0)
        {
            sprite_set_action(body_b->child, 9, 0x2095, 0);
            ((struct HeroSpriteBits *)&body_b->child->sprite->flags)->mode = 0;
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            body_b->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0x20);
            body_b->progress = 0;
            ((struct HeroFlagBits *)&body_b->flags)->active = 0;
        }
        body->state = hero_b_active_anim_place;
    }
}

void hero_b_active_anim_place(struct HeroBody *body)
{
    struct Hero *hero = gUnk_03000FD8->heroB;
    struct HeroBody *body_b = &hero->body;

    state_stack_advance_progress(body_b);
    if ((hero->body.flags & 0x20) != 0)
    {
        if ((hero->body.mode & 6) == 2 || (hero->body.mode & 6) == 4)
        {
            if (body_b->data->poseIndex == -1)
                sprite_set_action(body_b, 5, 0x2000, 0);
            else
                sprite_set_action(body_b, 5, 0x204D, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            place_targets(body_b, body_b->targetX / 0x100 + body_b->ofsX - 0x28,
                          body_b->targetY / 0x100 + body_b->ofsY, 0, -1);
            body_b->height = body_b->targetZ / 0x100 + 2;
            set_motion_offsets(body_b, 0x600);
        }
        body->state = hero_pair_active_place_both;
    }
}

void hero_pair_active_place_both(struct HeroBody *body)
{
    struct Hero *hero_a = gUnk_03000FD8->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct Hero *hero_b = gUnk_03000FD8->heroB;
    struct HeroBody *body_b = &hero_b->body;
    struct HeroData *data;

    step_motion(body_b);
    if ((body_b->flags & 0x20) != 0)
    {
        set_slot_value(0x2E, -1);
        set_slot_value(0x53, -1);
        if ((hero_a->body.mode & 6) == 2 || (hero_a->body.mode & 6) == 4)
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
            {
                struct HeroSprite *sprite;
                s16 *span;
                s32 mirror;

                sprite = body_a->sprite;
                mirror = 0;
                span = &body_a->span.a;
                if (!(span[0] < span[1]))
                    mirror = 1;
                ((struct HeroMirrorBits *)&sprite->mirror)->flip = mirror;
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
            place_targets(body_b, data->targetX / 0x100, data->targetY / 0x100, data->targetH / 0x100, -1);
            set_motion_start(body_b, 0x400);
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            if (body_b->data->poseIndex == -1)
                sprite_set_action(body_b, 0, 0x2002, 0);
            else
                sprite_set_action(body_b, 0, 0x204F, 0);
            {
                struct HeroSprite *sprite;
                s16 *span;
                s32 mirror;

                sprite = body_b->sprite;
                mirror = 0;
                span = &body_b->span.a;
                if (!(span[0] < span[1]))
                    mirror = 1;
                ((struct HeroMirrorBits *)&sprite->mirror)->flip = mirror;
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

void hero_a_mode_place_finish(struct HeroBody *body)
{
    struct HeroBody *body_a;
    s32 mode;

    mode = gUnk_03000FD8->modeByte;
    mode |= 0xC0;
    gUnk_03000FD8->modeByte = mode;
    body_a = &gUnk_03000FD8->heroA->body;
    if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
    {
        place_targets(body_a, body_a->targetX / 0x100 + body_a->ofsX - 0x1E,
                      body_a->targetY / 0x100 + body_a->ofsY, 0, -1);
        set_motion_start(body_a, 0x300);
    }
    if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
    {
        if (body_a->data->poseIndex == -1)
            sprite_set_action(body_a, 0, 0x2002, 0);
        else
            sprite_set_action(body_a, 0, 0x204F, 0);
        {
            struct HeroSprite *sprite;
            s16 *span;
            s32 mirror;

            sprite = body_a->sprite;
            mirror = 0;
            span = &body_a->span.a;
            if (!(span[0] < span[1]))
                mirror = 1;
            ((struct HeroMirrorBits *)&sprite->mirror)->flip = mirror;
        }
        ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
    }
    ((struct HeroMirrorBits *)&body_a->sprite->mirror)->flip = 0;
    if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
    {
        body_a->state = 0;
        enter_finish_rebuild(body_a);
    }
    body->state = hero_b_copy_pos;
}

void hero_b_start_step32_then_next(struct HeroBody *body)
{
    struct World *world = gUnk_03000FD8;
    struct HeroBody *body_a = &world->heroA->body;
    struct HeroBody *body_b = &world->heroB->body;
    u32 zero = (u32)body_a->state;

    if (zero == 0)
    {
        set_slot_value(0x2E, -1);
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            body_b->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0x20);
            body_b->progress = zero;
            ((struct HeroFlagBits *)&body_b->flags)->active = 0;
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            sprite_set_action(body_a, 0, 0x2024, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
        body_b->timer = body_b->timer & 0xFFF;
        body->state = hero_b_step_window_d72e4;
    }
}

ASM_FUNC("asm/code_080D6B10/hero_b_step_window_d72e4.s", void hero_b_step_window_d72e4(struct HeroBody *body))

void hero_b_active_place_at_partner(struct HeroBody *body)
{
    struct HeroBody *body_b = &gUnk_03000FD8->heroB->body;
    struct HeroBody *partner = body->partner;

    state_stack_advance_progress(body_b);
    if ((body_b->flags & 0x20) != 0)
    {
        set_slot_value(0x11F, -1);
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            sprite_set_action(body_b, 0xE, 0x2066, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 0;
        }
        if (body_b->child != 0)
        {
            sprite_set_action(body_b->child, 0xE, 0x2095, 0);
            ((struct HeroSpriteBits *)&body_b->child->sprite->flags)->mode = 0;
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            place_targets(body_b, partner->targetX / 0x100 + partner->ofsX - 10,
                          partner->targetY / 0x100 + partner->ofsY, 0, -1);
            set_motion_start(body_b, 0x300);
        }
        body->state = hero_b_link_partner_child_fx;
    }
}

void hero_b_link_partner_child_fx(struct HeroBody *body)
{
    struct HeroBody *body_b = &gUnk_03000FD8->heroB->body;
    struct HeroBody *partner = body->partner;
    s32 x;
    s32 y;

    step_motion(body_b);
    body_b->child->posX = body_b->posX;
    body_b->child->posY = body_b->posY;
    body_b->child->posZ = body_b->posZ;
    if ((body_b->flags & 0x20) != 0)
    {
        set_slot_value(0x55, -1);
        obj_release(body_b->child);
        ((struct HeroCcBits *)&body_b->cc)->low = 0;
        body_b->scale = 0xE6;
        resolve_contact_action(body_b, partner, 0, 0, gRoom149Layer1 + 0x40D);
        body_b->count++;
        if (partner->data->displayTimer > 0 || (partner->data->flags111 & 2) != 0)
        {
            x = partner->targetX / 0x100 + partner->ofsX;
            y = partner->targetY / 0x100 + partner->ofsY;
            effect_spawn_at(0x6C4, x, y, 0, partner);
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            sprite_set_action(body_b, 0xF, 0x2066, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
        }
        if (body_b->child != 0)
        {
            sprite_set_action(body_b->child, 0xF, 0x2095, 0);
            ((struct HeroSpriteBits *)&body_b->child->sprite->flags)->mode = 0;
        }
        body->state = hero_b_sprite_active_start_step16;
    }
}

void hero_b_sprite_active_start_step16(struct HeroBody *body)
{
    struct Hero *hero = gUnk_03000FD8->heroB;

    if ((hero->body.sprite->flags & 8) != 0)
    {
        if ((hero->body.mode & 6) == 2 || (hero->body.mode & 6) == 4)
        {
            hero->body.rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0x10);
            hero->body.progress = 0;
            ((struct HeroFlagBits *)&hero->body.flags)->active = 0;
        }
        body->state = hero_pair_active_place_a_finish;
    }
}

void hero_pair_active_place_a_finish(struct HeroBody *body)
{
    struct Hero *hero_a = gUnk_03000FD8->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct Hero *hero_b = gUnk_03000FD8->heroB;
    struct HeroBody *body_b = &hero_b->body;
    struct HeroData *data;

    state_stack_advance_progress(body_b);
    if ((body_b->flags & 0x20) != 0)
    {
        set_slot_value(0x53, -1);
        if ((hero_a->body.mode & 6) == 2 || (hero_a->body.mode & 6) == 4)
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
            {
                struct HeroSprite *sprite;
                s16 *span;
                s32 mirror;

                sprite = body_a->sprite;
                mirror = 0;
                span = &body_a->span.a;
                if (!(span[0] < span[1]))
                    mirror = 1;
                ((struct HeroMirrorBits *)&sprite->mirror)->flip = mirror;
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
            place_targets(body_b, data->targetX / 0x100, data->targetY / 0x100, data->targetH / 0x100, -1);
            set_motion_start(body_b, 0x400);
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            if (body_b->data->poseIndex == -1)
                sprite_set_action(body_b, 0, 0x2002, 0);
            else
                sprite_set_action(body_b, 0, 0x204F, 0);
            {
                struct HeroSprite *sprite;
                s16 *span;
                s32 mirror;

                sprite = body_b->sprite;
                mirror = 0;
                span = &body_b->span.a;
                if (!(span[0] < span[1]))
                    mirror = 1;
                ((struct HeroMirrorBits *)&sprite->mirror)->flip = mirror;
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


void hero_pair_spawn_fx_anim_next(struct HeroBody *body)
{
    struct World *world = gUnk_03000FD8;
    struct HeroBody *body_a = &world->heroA->body;
    struct HeroBody *body_b = &world->heroB->body;

    effect_spawn_at(0x2FE6, body->posX / 0x100, body->posY / 0x100, body->posZ / 0x100, body);
    if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
    {
        sprite_set_action(body_a, 2, 0x2024, 0);
        ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
    }
    if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
    {
        sprite_set_action(body_b, 0xC, 0x2066, 0);
        ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
    }
    if (body_b->child != 0)
    {
        sprite_set_action(body_b->child, 0xC, 0x2095, 0);
        ((struct HeroSpriteBits *)&body_b->child->sprite->flags)->mode = 0;
    }
    ((struct HeroSpriteBits *)&body_b->child->sprite->flags)->mode = 1;
    body->state = hero_b_sprite_idle_start_step10;
}

void hero_b_sprite_idle_start_step10(struct HeroBody *body)
{
    struct World *world = gUnk_03000FD8;
    struct HeroBody *body_a = &world->heroA->body;
    struct HeroBody *body_b = &world->heroB->body;

    if ((body_a->sprite->flags & 8) != 0)
    {
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            sprite_set_action(body_a, 0, 0xFFFFFFFF, 0);
            ((struct HeroMirrorBits *)&body_a->sprite->mirror)->flip = 0;
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            body_b->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0xA);
            body_b->progress = 0;
            ((struct HeroFlagBits *)&body_b->flags)->active = 0;
        }
        body->state = hero_b_active_partner_anim_place;
    }
}

void hero_b_active_partner_anim_place(struct HeroBody *body)
{
    struct HeroBody *body_b = &gUnk_03000FD8->heroB->body;
    struct HeroBody *partner = body->partner;
    s32 x;
    s32 y;

    state_stack_advance_progress(body_b);
    if ((body_b->flags & 0x20) != 0)
    {
        set_slot_value(0x41, -1);
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            sprite_set_action(body_b, 0xD, 0x2066, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 0;
        }
        if (body_b->child != 0)
        {
            sprite_set_action(body_b->child, 0xD, 0x2095, 0);
            ((struct HeroSpriteBits *)&body_b->child->sprite->flags)->mode = 0;
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            x = partner->targetX;
            if (x < 0)
                x += 0xFF;
            x = x >> 8;
            x += partner->ofsX;
            y = partner->targetY;
            if (y < 0)
                y += 0xFF;
            y = y >> 8;
            y += partner->ofsY;
            place_targets(body_b, x, y, 0, -1);
            set_motion_start(body_b, 0x300);
        }
        body_b->timer = body_b->timer & 0xFFF;
        body->state = hero_b_step_d8164;
    }
}

ASM_FUNC("asm/code_080D6B10/hero_b_step_d8164.s", void hero_b_step_d8164(struct HeroBody *body))
