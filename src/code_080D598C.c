#include "global.h"
#include "ram_routines.h"

struct HeroSprite
{
    u8 filler0[0x06];
    s16 scale;
    u8 filler8[0x11 - 0x08];
    u8 mirror;
    u8 flags;
    u8 filler13[0x20 - 0x13];
    u8 layer;
    u8 unk21;
};

struct HeroSpriteBits
{
    u8 unk0 : 1;
    u8 mode : 2;
    u8 unk3 : 5;
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
    s16 displayTimer;
    u8 filler0F8[0x110 - 0xF8];
    u8 flags110;
    u8 flags111;
};

struct HeroMirrorBits
{
    u8 unk0 : 6;
    u8 flip : 1;
    u8 unk7 : 1;
};

struct HeroCcBits
{
    u8 low : 7;
    u8 high : 1;
};

struct HeroFlagBits
{
    u8 unk0 : 5;
    u8 active : 1;
    u8 unk6 : 2;
};

struct HeroSpan
{
    s16 a;
    s16 b;
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
    u8 filler44[0x47 - 0x44];
    s8 unk47;
    u8 filler48[0x4C - 0x48];
    void (*state)(struct HeroBody *);
    u32 unk50;
    u8 filler54[0x6C - 0x54];
    void *anim;
    u8 filler70[0x75 - 0x70];
    u8 order;
    u8 mode;
    u8 filler77[0x79 - 0x77];
    u8 flags;
    s16 rate;
    u16 speed;
    s16 progress;
    u8 filler80[0x84 - 0x80];
    struct HeroSpan span;
    u8 filler88[0x92 - 0x88];
    u16 height;
    s32 unk94;
    s32 unk98;
    u8 filler9C[0xAC - 0x9C];
    s16 prevProgress;
    u16 timer;
    u8 filler0B0[0xC3 - 0xB0];
    s8 ofsX;
    s8 ofsY;
    u8 filler0C5[0xCA - 0xC5];
    u16 scale;
    u8 cc;
};

struct ListNode
{
    struct ListNode *next;
    u8 filler4[0x08 - 0x04];
    struct HeroSprite *sprite;
};

struct KeyPad
{
    u8 filler0[0x3E];
    u16 keys;
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
    u8 filler0B;
    u8 viewByte;
    u8 filler0D[0x58 - 0x0D];
    struct Hero *objList[6];
    struct Hero *heroA;
    struct Hero *heroB;
    struct ListNode *layerList;
    u8 filler7C[0x24C - 0x7C];
    void *trackedObj;
};

extern struct World *gUnk_03000FD8;
extern const u8 gRoom149Layer1[];
extern struct KeyPad gUnk_0300034C;

void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void set_slot_value(u32 id, s32 value);
void step_motion(struct HeroBody *body);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 z, struct HeroBody *owner);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void set_motion_start(struct HeroBody *body, u32 speed);
void set_motion_offsets(struct HeroBody *body, u32 offsets);
s32 obj_last_x(void *obj);
s32 obj_last_y(void *obj);

void hero_pair_place_both_finish_step(struct HeroBody *body);
void hero_pair_b_window_5f5c(struct HeroBody *body);
void hero_pair_place_both_finish_adv_alt(struct HeroBody *body);
void enter_finish_rebuild(struct HeroBody *body);
void state_stack_advance_progress(struct HeroBody *body);
void step_motion_scaled(struct HeroBody *body);
void set_body_part(struct HeroBody *body, u32 art, s32 x, s32 y, u32 extra);
void body_set_pose(u32 art, struct HeroBody *body, s32 x, s32 y);
void obj_free_parts(struct HeroBody *body);
void display_flush(void);
void sprite_objects_draw_obj_tiles(u32 a, u32 b);
void set_motion_speed_aux(struct HeroBody *body, u32 speed, u32 aux);
void hero_b_remove_child_fx(struct HeroBody *body);
void hero_b_link_ready_objs(struct HeroBody *body);
void obj_release(void *obj);
void hero_b_place_at_target_h(struct HeroBody *owner);
void resolve_contact_action(struct HeroBody *a, struct HeroBody *b, u32 c, u32 d, const u8 *e);

void hero_b_link_partner_after_a(struct HeroBody *owner)
{
    struct Hero *hero_a = gUnk_03000FD8->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct Hero *hero_b = gUnk_03000FD8->heroB;
    struct HeroBody *body_b = &hero_b->body;
    struct HeroBody *partner = owner->partner;

    step_motion(body_b);
    body_b->child->posX = body_b->posX;
    body_b->child->posY = body_b->posY;
    body_b->child->posZ = body_b->posZ;
    {
        u16 scale = body_a->sprite->scale;

        if ((s16)body_a->sprite->scale <= 0xFF)
        {
            body_a->sprite->scale = scale + 0x10;
            if ((s16)body_a->sprite->scale > 0xFF)
            {
                body_a->sprite->scale = 0x100;
                if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
                {
                    sprite_set_action(body_a, 0, -1, 0);
                    ((struct HeroMirrorBits *)&body_a->sprite->mirror)->flip = 0;
                    ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
                }
            }
        }
    }
    if ((body_b->flags & 0x20) != 0)
    {
        obj_release(body_b->child);
        set_slot_value(0x55, -1);
        ((struct HeroCcBits *)&body_b->cc)->low = 0;
        body_b->scale = 0x100;
        resolve_contact_action(body_b, partner, 0, 0, (const u8 *)0x0483818D);
        body_b->count = body_b->count + 1;
        if (partner->data->displayTimer > 0 || (partner->data->flags111 & 2) != 0)
        {
            s32 fx_x = partner->targetX / 0x100 + partner->ofsX;
            s32 fx_y = partner->targetY / 0x100 + partner->ofsY;

            effect_spawn_at(0x6C4, fx_x, fx_y, 0, partner);
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            sprite_set_action(body_b, 5, 0x2066, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
        }
        if (body_b->child != 0)
        {
            sprite_set_action(body_b->child, 5, 0x2095, 0);
            ((struct HeroSpriteBits *)&body_b->child->sprite->flags)->mode = 0;
        }
        owner->state = hero_b_place_at_target_h;
    }
}

void hero_b_place_at_target_h(struct HeroBody *owner)
{
    struct Hero *hero_b = gUnk_03000FD8->heroB;
    struct HeroBody *body_b = &hero_b->body;

    if ((body_b->sprite->flags & 8) != 0)
    {
        set_slot_value(0x2B, -1);
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            sprite_set_action(body_b, 6, 0x2066, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
        }
        if (body_b->child != 0)
        {
            sprite_set_action(body_b->child, 6, 0x2095, 0);
            ((struct HeroSpriteBits *)&body_b->child->sprite->flags)->mode = 0;
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            s32 x = body_b->targetX / 0x100 + body_b->ofsX - 0x18;
            s32 y = body_b->targetY / 0x100 + body_b->ofsY;

            place_targets(body_b, x, y, 0, -1);
            body_b->height = body_b->targetZ / 0x100 + 0xC;
            set_motion_offsets(body_b, 0x400);
        }
        body_b->rate = 0x2E8;
        owner->state = hero_pair_place_both_finish_step;
    }
}

void hero_pair_place_both_finish_step(struct HeroBody *body)
{
    struct Hero *hero_a = gUnk_03000FD8->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct Hero *hero_b = gUnk_03000FD8->heroB;
    struct HeroBody *body_b = &hero_b->body;

    step_motion(body_b);
    if ((hero_b->body.flags & 0x20) != 0)
    {
        set_slot_value(0x53, -1);
        if ((hero_a->body.mode & 6) == 2 || (hero_a->body.mode & 6) == 4)
        {
            place_targets(body_a, body_a->data->targetX / 0x100, body_a->data->targetY / 0x100,
                body_a->data->targetH / 0x100, -1);
            set_motion_start(body_a, 0x400);
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            if (body_a->data->pose == -1)
                sprite_set_action(body_a, 0, 0x2002, 0);
            else
                sprite_set_action(body_a, 0, 0x204F, 0);
            {
                struct HeroSprite *sprite = body_a->sprite;
                s32 flip = 0;
                struct HeroSpan *span = &body_a->span;

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
            place_targets(body_b, body_b->data->targetX / 0x100, body_b->data->targetY / 0x100,
                body_b->data->targetH / 0x100, -1);
            set_motion_start(body_b, 0x400);
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            if (body_b->data->pose == -1)
                sprite_set_action(body_b, 0, 0x2002, 0);
            else
                sprite_set_action(body_b, 0, 0x204F, 0);
            {
                struct HeroSprite *sprite = body_b->sprite;
                s32 flip = 0;
                struct HeroSpan *span = &body_b->span;

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
