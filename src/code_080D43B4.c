#include "global.h"
#include "ram_routines.h"

struct HeroSprite
{
    u8 filler0[0x0E];
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
    u8 filler0E4[0xEC - 0xE4];
    s16 poseIndex;
    u8 filler0EE[0xF6 - 0xEE];
    s16 unkF6;
    u8 filler0F8[0x111 - 0xF8];
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

struct WorldModeBits
{
    u8 low4 : 4;
    u8 mid : 2;
    u8 high : 2;
};

struct DrawNode
{
    struct DrawNode *next;
    u8 filler4[0x08 - 0x04];
    struct HeroSprite *sprite;
};

struct KeyState
{
    u8 filler0[0x3E];
    u16 pressed;
};

struct World
{
    u8 filler0[0x0A];
    u8 modeByte;
    u8 filler0B[0x0C - 0x0B];
    u8 view;
    u8 filler0D[0x0E - 0x0D];
    u8 unk0E;
    u8 filler0F[0x70 - 0x0F];
    struct Hero *heroA;
    struct Hero *heroB;
    struct DrawNode *nodes;
    u8 filler7C[0x24C - 0x7C];
    void *focus24C;
};

extern struct World *gWorld;
extern u8 *gParty;
extern struct KeyState gSys;

void step_motion(struct HeroBody *body);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void set_slot_value(u32 id, s32 value);
void obj_release(void *obj);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void enter_finish_rebuild(struct HeroBody *body);
void set_motion_start(struct HeroBody *body, u32 speed);
void schedule_pose_task(struct HeroBody *obj);
void state_stack_advance_progress(struct HeroBody *body);
void resolve_contact_action(struct HeroBody *a, struct HeroBody *b, u32 c, u32 d, u32 e);
s32 obj_last_x(void *obj);
s32 obj_last_y(void *obj);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 h, struct HeroBody *owner);
struct HeroBody *obj_spawn_linked(struct HeroBody *parent, u32 kind, u32 value);

void hero_a_step16_when_b_idle(struct HeroBody *body);
void hero_pair_place_both_finish_sprite(struct HeroBody *body);
void hero_pair_place_both_finish_sprite_alt(struct HeroBody *body);
void hero_pair_place_both_tracked(struct HeroBody *body);
void hero_b_spawn_child_chain(struct HeroBody *body);
void hero_b_place_at_a_height(struct HeroBody *body);
void hero_b_link_partner_step16(struct HeroBody *body);
void hero_b_step_keys_d4a18(struct HeroBody *body);
void hero_a_mode_place_finish(struct HeroBody *body);
void hero_b_window_d5114(struct HeroBody *body);
void set_body_part(struct HeroBody *body, u32 art, s32 x, s32 y, u32 extra);
void body_set_pose(u32 art, struct HeroBody *body, s32 x, s32 y);
void obj_free_parts(struct HeroBody *body);
void display_flush(void);
void sprite_objects_draw_obj_tiles(u32 a, u32 b);
void hero_b_step_window_d3c38(struct HeroBody *body);
void hero_b_end_a(struct HeroBody *body);
u32 find_frame_entry(struct HeroBody *obj, u32 index);
void set_motion_offsets(struct HeroBody *body, u32 offsets);
void hero_pair_place_both_finish_adv(struct HeroBody *body);

void hero_pair_anim_place_b_at_partner(struct HeroBody *body)
{
    struct Hero *hero_a = gWorld->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct Hero *hero_b = gWorld->heroB;
    struct HeroBody *body_b = &hero_b->body;
    struct HeroBody *partner = body->partner;
    s32 tx;
    s32 ty;
    s32 x;
    s32 y;

    step_motion(body_b);
    if ((hero_b->body.flags & 0x20) != 0)
    {
        if ((hero_a->body.mode & 6) == 2 || (hero_a->body.mode & 6) == 4)
        {
            sprite_set_action(body_a, 0, 0x2037, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            sprite_set_action(body_b, 4, 0x2055, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 0;
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            tx = partner->targetX;
            if (tx < 0)
                tx += 0xFF;
            x = (tx >> 8) + partner->ofsX - 10;
            ty = partner->targetY;
            if (ty < 0)
                ty += 0xFF;
            y = (ty >> 8) + partner->ofsY;
            place_targets(body_b, x, y, 0, -1);
            set_motion_start(body_b, 0x200);
        }
        body->state = hero_b_link_partner_step16;
    }
}

void hero_b_link_partner_step16(struct HeroBody *body)
{
    struct HeroBody *body_a = &gWorld->heroA->body;
    struct Hero *hero_b = gWorld->heroB;
    struct HeroBody *body_b = &hero_b->body;
    struct HeroBody *partner = body->partner;
    struct HeroData *data;
    s32 tx;
    s32 ty;
    s32 x;
    s32 y;

    step_motion(body_b);
    body_a->posX += body_b->posX - body_b->targetX;
    body_a->posY += body_b->posY - body_b->targetY;
    body_a->posZ += body_b->posZ - body_b->targetZ;
    if ((hero_b->body.flags & 0x20) != 0)
    {
        set_slot_value(0x63, -1);
        ((struct HeroCcBits *)&body_b->cc)->low = 0;
        body_b->scale = 0x100;
        resolve_contact_action(body_b, partner, 0, 0, 0x0801808D);
        body_b->count = body_b->count + 1;
        data = partner->data;
        if (data->unkF6 > 0 || (data->unk111 & 2) != 0)
        {
            tx = partner->targetX;
            if (tx < 0)
                tx += 0xFF;
            x = (tx >> 8) + partner->ofsX;
            ty = partner->targetY;
            if (ty < 0)
                ty += 0xFF;
            y = (ty >> 8) + partner->ofsY;
            effect_spawn_at(0x6C4, x, y, 0, partner);
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            body_b->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0x10);
            body_b->progress = 0;
            ((struct HeroFlagBits *)&body_b->flags)->active = 0;
        }
        body->state = hero_pair_place_both_finish_adv;
    }
}

void hero_pair_place_both_finish_adv(struct HeroBody *body)
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

    state_stack_advance_progress(body_b);
    if ((hero_b->body.flags & 0x20) != 0)
    {
        set_slot_value(0x53, -1);
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

void hero_b_child_align_finish(struct HeroBody *body)
{
    struct Hero *hero_b = gWorld->heroB;
    struct HeroBody *body_b = &hero_b->body;

    obj_release(body_b->child->child);
    body_b->child->posX = body_b->posX;
    body_b->child->posY = body_b->posY;
    body_b->child->posZ = body_b->posZ;
    if ((hero_b->body.mode & 6) == 2 || (hero_b->body.mode & 6) == 4)
    {
        sprite_set_action(body_b, 1, 0x2066, 0);
        ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
    }
    if (body_b->child != 0)
    {
        sprite_set_action(body_b->child, 1, 0x2095, 0);
        ((struct HeroSpriteBits *)&body_b->child->sprite->flags)->mode = 0;
    }
    body->state = hero_b_place_at_a_height;
}

void hero_b_place_at_a_height(struct HeroBody *body)
{
    struct HeroBody *body_a = &gWorld->heroA->body;
    struct Hero *hero_b = gWorld->heroB;
    struct HeroBody *body_b = &hero_b->body;
    s32 tx;
    s32 ty;
    s32 tz;
    s32 x;
    s32 y;
    s32 z;
    s32 h;

    if ((body_b->sprite->flags & 8) != 0)
    {
        set_slot_value(0x2B, -1);
        if ((hero_b->body.mode & 6) == 2 || (hero_b->body.mode & 6) == 4)
        {
            sprite_set_action(body_b, 2, 0x2066, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
        }
        if (body_b->child != 0)
        {
            sprite_set_action(body_b->child, 2, 0x2095, 0);
            ((struct HeroSpriteBits *)&body_b->child->sprite->flags)->mode = 0;
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            tx = body_a->targetX;
            if (tx < 0)
                tx += 0xFF;
            x = (tx >> 8) + body_a->ofsX;
            ty = body_a->targetY;
            if (ty < 0)
                ty += 0xFF;
            y = (ty >> 8) + body_a->ofsY;
            z = body_a->targetZ;
            if (z < 0)
                z += 0xFF;
            z = z >> 8;
            z = z + body_a->ofsZ;
            z = z - 12;
            place_targets(body_b, x, y, z, -1);
            if ((u8 *)body_a == (u8 *)body_a->data + 8)
                h = body_a->ofsZ;
            else
            {
                find_frame_entry(body_a, 0);
                h = body_a->unk47;
            }
            tz = body_a->targetZ;
            if (tz < 0)
                tz += 0xFF;
            h = h + (tz >> 8);
            h = h + MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x400, (h * h) / 64 + 0x20);
            body_b->height = h;
            set_motion_offsets(body_b, 0x400);
        }
        body_b->timer = body_b->timer & 0xFFF;
        body->state = hero_b_step_keys_d4a18;
    }
}
