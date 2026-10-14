#include "global.h"
#include "ram_routines.h"

struct HeroSprite
{
    u8 filler0[0x02];
    s16 ofsY;
    u8 filler4[0x0E - 0x04];
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
    s8 *offsets;
    u8 filler0E8[0xEC - 0xE8];
    s16 poseIndex;
    u8 filler0EE[0xF4 - 0xEE];
    u8 count;
    u8 filler0F5[0xF6 - 0xF5];
    s16 unkF6;
    u8 filler0F8[0x110 - 0xF8];
    u8 unk110;
    u8 unk111;
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
    u8 filler70[0x75 - 0x70];
    u8 order;
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
    u8 filler94[0x9C - 0x94];
    u32 place;
    s32 counter;
    u32 unkA4;
    s32 unkA8;
    s16 step;
    u16 timer;
    u8 filler0B0[0xB2 - 0xB0];
    u16 vel;
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
    u8 filler0[0x0C];
    u8 flags;
    u8 filler0D[0x58 - 0x0D];
    struct Hero *slots[6];
    struct Hero *heroA;
    struct Hero *heroB;
    struct WorldLink *list;
    u8 filler7C[0x24C - 0x7C];
    void *trackedObj;
};

struct WorldLink
{
    struct WorldLink *next;
    u32 unk4;
    void *obj;
};

struct ObjFlags
{
    u8 filler0[0x20];
    u8 flags;
};

struct KeyState
{
    u8 filler0[0x3C];
    u16 held;
};

extern struct KeyState gUnk_0300034C;

extern struct World *gUnk_03000FD8;

void state_stack_advance_progress(struct HeroBody *body);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void set_slot_value(u32 id, s32 value);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void set_motion_start(struct HeroBody *body, u32 speed);
void set_motion_offsets(struct HeroBody *body, u32 offsets);
s32 obj_last_x(void *obj);
s32 obj_last_y(void *obj);

void hero_a_active_anims(struct HeroBody *body);
void hero_b_place_from_a(struct HeroBody *body);
void hero_b_step_end_d354(struct HeroBody *body);
void hero_b_enter_side(struct HeroBody *body);
void hero_b_join_partner(struct HeroBody *body);
void hero_b_set_fall_timers(struct HeroBody *body);
void step_motion(struct HeroBody *body);
void enter_finish_rebuild(struct HeroBody *body);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 z, struct HeroBody *owner);
u32 resolve_contact_action(struct HeroBody *a, struct HeroBody *b, u32 c, u32 d, u32 e);
void obj_slot_update(struct HeroBody *obj);
void display_flush(void);
void obj_free_parts(struct HeroBody *body);
void body_set_pose(u32 art, struct HeroBody *body, s32 x, s32 y);
struct HeroSprite *find_lowest_order_pose(struct HeroBody *body);
void pair_action_b_follow_a(struct HeroBody *body);
void pair_action_b_follow_partner(struct HeroBody *body);
void pair_action_a_hold(struct HeroBody *body);
void pair_action_a_start(struct HeroBody *body);
struct HeroBody *obj_spawn_linked(struct HeroBody *parent, u32 kind, u32 value);
void hero_b_to_partner_slow(struct HeroBody *body);
void pair_action_pick_partner(struct HeroBody *body);
u32 mt19937_next(void);
s32 sub_081DE850(s32 a, u32 b);
void obj_release(struct HeroBody *obj);
void hero_b_to_partner_fast(struct HeroBody *body);
void pair_finish_and_rebuild(struct HeroBody *body);

void hero_b_place_beside_partner(struct HeroBody *body)
{
    struct Hero *hero_b = gUnk_03000FD8->heroB;
    struct HeroBody *body_b = &hero_b->body;
    struct HeroBody *partner = body->partner;

    if ((partner->x - body_b->x) / 0x10 < -0x2D0)
    {
        if ((hero_b->body.mode & 6) == 2 || (hero_b->body.mode & 6) == 4)
        {
            place_targets(body_b, body_b->targetX / 0x100 + body_b->ofsX - 0x1E,
                          body_b->targetY / 0x100 + body_b->ofsY, 0, -1);
            {
                u16 h = 0x28;
                hero_b->body.height = h;
            }
            set_motion_offsets(body_b, 0x600);
        }
        body_b->place = 1;
    }
    else
    {
        if ((hero_b->body.mode & 6) == 2 || (hero_b->body.mode & 6) == 4)
        {
            place_targets(body_b, partner->targetX / 0x100 + partner->ofsX,
                          partner->targetY / 0x100 + partner->ofsY,
                          partner->targetZ / 0x100 + partner->ofsZ, -1);
            {
                u16 h = 0x28;
                hero_b->body.height = h;
            }
            set_motion_offsets(body_b, 0x600);
        }
        body_b->place = 0;
    }
    body->state = hero_b_join_partner;
}

void hero_b_join_partner(struct HeroBody *body)
{
    struct Hero *hero_b = gUnk_03000FD8->heroB;
    struct HeroBody *body_b = &hero_b->body;
    struct HeroBody *partner = body->partner;
    struct HeroData *data;
    u32 link;

    step_motion(body_b);
    if ((hero_b->body.flags & 0x20) != 0)
    {
        if (body_b->place != 0)
        {
            effect_spawn_at(0x2FF9, body_b->x / 0x100, body_b->y / 0x100, body_b->z / 0x100, body_b);
            set_slot_value(0x30, -1);
            if ((body->mode & 6) == 2 || (body->mode & 6) == 4)
            {
                body->state = hero_b_place_beside_partner;
            }
        }
        else
        {
            set_slot_value(0x63, -1);
            ((struct HeroCcBits *)&hero_b->body.cc)->low = 0;
            body_b->scale = 0x100;
            link = resolve_contact_action(body_b, partner, 0, 0, 0x040210A8);
            body_b->count = body_b->count + 1;
            if (link == 1)
            {
                obj_slot_update(body_b);
            }
            else
            {
                data = partner->data;
                if (data->unkF6 > 0 || (data->unk111 & 2) != 0)
                {
                    s32 px = partner->targetX / 0x100 + partner->ofsX;
                    s32 py = partner->targetY / 0x100 + partner->ofsY;
                    s32 pz = partner->targetZ / 0x100 + partner->ofsZ;
                    effect_spawn_at(0x6E3, px, py, pz, partner);
                }
                if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
                {
                    sprite_set_action(body_b, 0, 0x2005, 0);
                    ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
                }
                ((struct HeroMirrorBits *)&body_b->sprite->mirror)->flip = 1;
                body->state = hero_b_set_fall_timers;
            }
        }
    }
}

void hero_b_fall_and_land(struct HeroBody *body)
{
    struct Hero *hero_b = gUnk_03000FD8->heroB;
    struct HeroBody *body_b = &hero_b->body;
    s32 scaled;

    scaled = (s16)body_b->timer * 0xDE;
    if (scaled < 0)
        scaled += 0xFF;
    body_b->timer = scaled >> 8;
    body_b->vel -= 0x80;
    body_b->x += (s16)body_b->timer;
    body_b->z += (s16)body_b->vel;
    if (body_b->z <= -1024)
    {
        set_slot_value(0x47, -1);
        body_b->timer = 0;
        body_b->vel = 0;
        body_b->z = 0;
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            body_b->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 8);
            body_b->progress = 0;
            ((struct HeroFlagBits *)&body_b->flags)->active = 0;
        }
        body->state = pair_finish_and_rebuild;
    }
}

void pair_finish_and_rebuild(struct HeroBody *body)
{
    struct Hero *hero_a = gUnk_03000FD8->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct Hero *hero_b = gUnk_03000FD8->heroB;
    struct HeroBody *body_b = &hero_b->body;
    struct HeroSprite *sprite;
    struct HeroSpan *span;

    state_stack_advance_progress(body_b);
    if ((hero_b->body.flags & 0x20) != 0)
    {
        body_b->z = 0;
        set_slot_value(0x53, -1);
        if ((hero_b->body.mode & 6) == 2 || (hero_b->body.mode & 6) == 4)
        {
            place_targets(body_b, body_b->data->targetX / 0x100, body_b->data->targetY / 0x100,
                          body_b->data->targetH / 0x100, -1);
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
            place_targets(body_a, body_a->data->targetX / 0x100, body_a->data->targetY / 0x100,
                          body_a->data->targetH / 0x100, -1);
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
