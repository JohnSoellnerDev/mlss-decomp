#include "global.h"
#include "ram_routines.h"

struct HeroSprite
{
    u8 filler0[0x06];
    s16 scale;
    u8 filler8[0x11 - 0x08];
    u8 mirror;
    u8 flags;
    u8 filler13[0x16 - 0x13];
    u16 unk16;
    u8 filler18[0x1E - 0x18];
    u8 unk1E;
    u8 filler1F[0x20 - 0x1F];
    u8 frame;
    u8 unk21;
    u8 filler22;
    u8 unk23;
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
    s16 rate;
    s16 frame;
    u16 progress;
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

struct SpriteListItem
{
    struct SpriteListItem *next;
    u8 filler4[0x08 - 0x04];
    struct HeroSprite *sprite;
};

struct World
{
    u8 filler0[0x0A];
    u8 modeByte;
    u8 filler0B;
    u8 layerFlags;
    u8 filler0D[0x70 - 0x0D];
    struct Hero *heroA;
    struct Hero *heroB;
    struct SpriteListItem *sprites;
};

struct WorldModeBits
{
    u8 unk0 : 4;
    u8 field4 : 2;
    u8 unk6 : 2;
};

struct KeyState
{
    u8 filler0[0x3E];
    u16 held;
};

extern struct World *gWorld;
extern struct KeyState gSys;

void state_stack_advance_progress(struct HeroBody *body);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void set_slot_value(u32 id, s32 value);
void step_motion(struct HeroBody *body);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void set_motion_start(struct HeroBody *body, u32 speed);
void set_motion_speed_aux(struct HeroBody *body, u32 speed, u32 aux);
void set_motion_offsets(struct HeroBody *body, u32 offsets);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 z, struct HeroBody *owner);
void enter_finish_rebuild(struct HeroBody *body);
void start_action_4(struct HeroBody *body);
void hero_b_progress_end(struct HeroBody *body);
void hero_b_active_hand_over(struct HeroBody *body);
void set_body_part(struct HeroBody *body, u32 art, s32 x, s32 y, u32 extra);
void body_set_pose(u32 art, struct HeroBody *body, s32 x, s32 y);
void obj_free_parts(struct HeroBody *body);
void display_flush(void);
void sprite_objects_draw_obj_tiles(u32 a, u32 b);
u32 find_frame_entry(struct HeroBody *obj, u32 index);
void hero_a_on_partner(struct HeroBody *body);
void hero_a_link_to_partner_fx(struct HeroBody *body);

void hero_pair_finish_b_place_a_action_copy(struct HeroBody *body);
void hero_b_move_to_partner_pose(struct HeroBody *body);
void hero_a_active_copy_pos_fx(struct HeroBody *body);
void hero_a_active_place_partner_anims(struct HeroBody *body);
void hero_a_walk_to_partner_then_pair(struct HeroBody *body);
void hero_pair_place_action_both(void);
void hero_b_active_place_at_a_pose(struct HeroBody *body);
void hero_b_active_start_step(struct HeroBody *body);
void hero_a_snap_to_b_target(struct HeroBody *body);
void hero_a_active_follow_b(struct HeroBody *body);
void hero_pair_follow_step(struct HeroBody *body);

void hero_pair_finish_b_place_a_action_copy(struct HeroBody *body)
{
    struct Hero *hero_a = gWorld->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct Hero *hero_b = gWorld->heroB;
    struct HeroBody *body_b = &hero_b->body;
    struct HeroData *data;
    struct HeroSprite *sprite;
    s32 tx;
    s32 ty;
    s32 tz;
    s32 x;
    s32 y;
    s32 z;

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
            tz = data->targetH;
            if (tz < 0)
                tz += 0xFF;
            z = tz >> 8;
            place_targets(body_b, x, y, z, -1);
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
            tz = data->targetH;
            if (tz < 0)
                tz += 0xFF;
            z = tz >> 8;
            place_targets(body_a, x, y, z, -1);
            tz = body_a->targetZ;
            if (tz < 0)
                tz += 0xFF;
            body_a->height = (tz >> 8) + 0x28;
            set_motion_offsets(body_a, 0x7CC);
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            body_a->state = 0;
            start_action_4(body_a);
        }
    }
}

void hero_b_move_to_partner_pose(struct HeroBody *body)
{
    struct Hero *hero_b = gWorld->heroB;
    struct HeroBody *body_b = &hero_b->body;
    struct HeroBody *partner = body->partner;
    struct HeroSprite *sprite;
    struct HeroSpan *span;
    s32 tx;
    s32 ty;
    s32 x;
    s32 y;

    if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
    {
        tx = partner->targetX;
        if (tx < 0)
            tx += 0xFF;
        x = (tx >> 8) + partner->ofsX;
        ty = partner->targetY;
        if (ty < 0)
            ty += 0xFF;
        y = (ty >> 8) + partner->ofsY;
        place_targets(body_b, x, y, 0, -1);
        set_motion_start(body_b, 0x200);
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
    body->state = hero_b_progress_end;
}

void hero_a_active_copy_pos_fx(struct HeroBody *body)
{
    struct Hero *hero_a = gWorld->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct HeroBody *body_b = &gWorld->heroB->body;

    step_motion(body_a);
    body_b->posX = body_a->posX;
    body_b->posY = body_a->posY;
    body_b->posZ = body_a->posZ - 0x1E00;
    if ((hero_a->body.flags & 0x20) != 0)
    {
        set_slot_value(0x8E, -1);
        body_b->posX = body_a->posX;
        body_b->posY = body_a->posY;
        body_b->posZ = body_a->posZ;
        if ((hero_a->body.mode & 6) == 2 || (hero_a->body.mode & 6) == 4)
        {
            sprite_set_action(body_a, 5, 0x2030, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            sprite_set_action(body_b, 3, 0x205F, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
        }
        effect_spawn_at(0x2F90, body_a->posX / 0x100, body_a->posY / 0x100,
                        body_a->posZ / 0x100 + 0x17, body_a);
        body->state = hero_a_active_place_partner_anims;
    }
}

void hero_a_active_place_partner_anims(struct HeroBody *body)
{
    struct Hero *hero_a = gWorld->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct Hero *hero_b = gWorld->heroB;
    struct HeroBody *body_b = &hero_b->body;
    struct HeroBody *partner = body->partner;
    s32 tx;
    s32 ty;
    s32 tz;
    s32 x;
    s32 y;
    s32 z;

    if ((body_a->sprite->flags & 8) != 0)
    {
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
            tz = partner->targetZ;
            if (tz < 0)
                tz += 0xFF;
            z = (tz >> 8) + partner->ofsZ;
            place_targets(body_a, x, y, z, -1);
            set_motion_speed_aux(body_a, 0, 0xC0);
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            sprite_set_action(body_a, 6, 0x2030, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            sprite_set_action(body_b, 4, 0x205F, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
        }
        body_a->timer = body_a->timer & 0xFFF;
        body->state = hero_a_walk_to_partner_then_pair;
    }
}
