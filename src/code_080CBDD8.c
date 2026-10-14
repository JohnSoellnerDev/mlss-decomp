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
    u8 high : 1;
};

struct HeroSpan
{
    s16 a;
    s16 b;
};

struct HeroExt
{
    u8 filler0[0x04];
    s8 ofsA;
    s8 ofsB;
};

struct HeroData
{
    u8 filler0[0xD8];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    struct HeroExt *ext;
    u8 filler0E8[0xEC - 0xE8];
    s16 poseIndex;
    u8 filler0EE[0xF6 - 0xEE];
    s16 flagsF6;
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
    struct HeroBody *next;
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
    u32 unk94;
    u32 unk98;
    s32 unk9C;
    u32 link;
    u8 filler0A4[0xAC - 0xA4];
    s16 step;
    u16 timer;
    s16 jitterY;
    u8 filler0B2[0xC3 - 0xB2];
    s8 ofsX;
    s8 ofsY;
    s8 ofsZ;
    u8 filler0C6[0xCA - 0xC6];
    u16 scale;
    u8 cc;
    u8 filler0CD[0xD0 - 0xCD];
};

struct Hero
{
    u8 filler0[0x08];
    struct HeroBody body;
    u8 filler0D8[0xE4 - 0xD8];
    struct HeroExt *ext;
    u8 filler0E8[0x110 - 0xE8];
    u8 flags110;
};

struct World
{
    u8 filler0[0x58];
    struct Hero *objList[6];
    struct Hero *heroA;
    struct Hero *heroB;
};

struct KeyState
{
    u8 filler0[0x3C];
    u16 held;
};

extern struct World *gUnk_03000FD8;
extern struct KeyState gUnk_0300034C;

void set_slot_value(u32 id, s32 value);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void set_motion_speed_aux(struct HeroBody *body, u32 speed, u32 aux);
void hero_pair_follow_ae14(struct HeroBody *body);
void state_stack_advance_progress(struct HeroBody *body);
void set_motion_offsets(struct HeroBody *body, u32 offsets);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void hero_pair_anim_a_wait(struct HeroBody *body);
void hero_pair_find_partner(struct HeroBody *body);
void hero_b_step_window_cb508(struct HeroBody *body);
void hero_b_sprite_active_place_finish(struct HeroBody *body);
u32 mt19937_next(void);
u32 sub_081DE850(u32 range, u32 rnd);
void step_motion(struct HeroBody *body);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 h, struct HeroBody *owner);
void enter_finish_rebuild(struct HeroBody *body);
void set_motion_start(struct HeroBody *body, u32 speed);
void hero_pair_z_step_place_partner(struct HeroBody *body);
void set_body_part(struct HeroBody *body, u32 art, s32 x, s32 y, u32 extra);
void obj_free_parts(struct HeroBody *body);
void display_flush(void);

void hero_b_sprite_active_place_finish(struct HeroBody *body)
{
    struct World *world = gUnk_03000FD8;
    struct HeroBody *body_b = &world->heroB->body;

    if ((body_b->sprite->flags & 8) != 0)
    {
        set_slot_value(0x53, -1);
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            struct HeroData *data = body_b->data;

            place_targets(body_b, data->targetX / 0x100, data->targetY / 0x100, data->targetH / 0x100, -1);
            set_motion_start(body_b, 0x400);
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            if (body_b->data->poseIndex == -1)
            {
                sprite_set_action(body_b, 0, 0x2002, 0);
            }
            else
            {
                sprite_set_action(body_b, 0, 0x204F, 0);
            }
            {
                struct HeroSprite *sprite = body_b->sprite;
                s32 flip = 0;
                struct HeroSpan *span = &body_b->span;

                if (span->a >= span->b)
                {
                    flip = 1;
                }
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


void hero_a_active_place_target_anim(struct HeroBody *body)
{
    struct World *world = gUnk_03000FD8;
    struct HeroBody *body_a = &world->heroA->body;

    state_stack_advance_progress(body_a);
    if ((body_a->flags & 0x20) != 0)
    {
        set_slot_value(0x2B, -1);
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            s32 x = body_a->targetX / 0x100 + body_a->ofsX - 0x1E;
            s32 y = body_a->targetY / 0x100 + body_a->ofsY - 0xA;

            place_targets(body_a, x, y, 0, -1);
            {
                u16 height = 0x24;

                body_a->height = height;
            }
            set_motion_offsets(body_a, 0x480);
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            sprite_set_action(body_a, 0, 0x2035, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        body->state = hero_pair_anim_a_wait;
    }
}

void hero_a_sprite_active_place_finish(void)
{
    struct World *world = gUnk_03000FD8;
    struct HeroBody *body_a = &world->heroA->body;

    if ((body_a->sprite->flags & 8) != 0)
    {
        set_slot_value(0x53, -1);
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            struct HeroData *data = body_a->data;

            place_targets(body_a, data->targetX / 0x100, data->targetY / 0x100, data->targetH / 0x100, -1);
            set_motion_start(body_a, 0x400);
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            if (body_a->data->poseIndex == -1)
            {
                sprite_set_action(body_a, 0, 0x2002, 0);
            }
            else
            {
                sprite_set_action(body_a, 0, 0x204F, 0);
            }
            {
                struct HeroSprite *sprite = body_a->sprite;
                s32 flip = 0;
                struct HeroSpan *span = &body_a->span;

                if (span->a >= span->b)
                {
                    flip = 1;
                }
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


void hero_pair_find_partner(struct HeroBody *body)
{
    struct HeroBody *partner = body->partner;
    s32 dx;
    s32 dy;
    s32 base_x;
    s32 base_y;
    s32 best;
    s32 slot;

    if (partner == 0 || (partner->mode & 6) != 4)
    {
        u32 count = 0;
        s32 k = 0;

        do
        {
            struct HeroBody *cand = &gUnk_03000FD8->objList[k]->body;

            if ((cand->mode & 6) == 4 && (cand->data->flags110 & 0x20) == 0)
            {
                u32 rnd;

                count++;
                rnd = mt19937_next();
                if (sub_081DE850(count, rnd) == 0)
                {
                    body->partner = cand;
                }
            }
            {
                s32 next = k + 1;

                cand = &gUnk_03000FD8->objList[next]->body;
            }
            if ((cand->mode & 6) == 4 && (cand->data->flags110 & 0x20) == 0)
            {
                u32 rnd;

                count++;
                rnd = mt19937_next();
                if (sub_081DE850(count, rnd) == 0)
                {
                    body->partner = cand;
                }
            }
            k += 2;
        } while (k <= 5);
    }
    else
    {
        if ((gUnk_0300034C.held & 0xF0) == 0)
        {
            return;
        }
        base_x = partner->targetX / 0x100 + partner->data->ext->ofsA;
        base_y = partner->targetY / 0x100 + partner->data->ext->ofsB;
        best = 0x7FFFFFFF;
        for (slot = 0; slot <= 5; slot++)
        {
            struct Hero *obj = gUnk_03000FD8->objList[slot];
            s32 keys = gUnk_0300034C.held & 0xF0;

            switch (keys)
            {
                case 0x40:
                    dx = base_y - (obj->body.targetY / 0x100 + obj->ext->ofsB);
                    dy = (obj->body.targetX / 0x100 + obj->ext->ofsA) - base_x;
                    break;
                case 0x80:
                    dx = (obj->body.targetY / 0x100 + obj->ext->ofsB) - base_y;
                    dy = (obj->body.targetX / 0x100 + obj->ext->ofsA) - base_x;
                    break;
                case 0x20:
                    dx = base_x - (obj->body.targetX / 0x100 + obj->ext->ofsA);
                    dy = (obj->body.targetY / 0x100 + obj->ext->ofsB) - base_y;
                    break;
                case 0x10:
                    dx = (obj->body.targetX / 0x100 + obj->ext->ofsA) - base_x;
                    dy = (obj->body.targetY / 0x100 + obj->ext->ofsB) - base_y;
                    break;
            }
            if ((obj->flags110 & 0x20) == 0 && (obj->body.mode & 6) == 4)
            {
                s32 dist = dx * dx + dy * dy;

                if (dx > 0)
                {
                    dist = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(dist, dx);
                    if (best > dist)
                    {
                        best = dist;
                        body->partner = &obj->body;
                    }
                }
            }
        }
    }
}
