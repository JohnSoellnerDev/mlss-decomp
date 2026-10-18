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

struct PoseFlagBits
{
    u16 unk0 : 5;
    u16 field5 : 5;
    u16 unk10 : 6;
};

struct PoseRecord
{
    u8 filler0[0x46];
    u8 byte46;
    u8 filler47;
    struct PoseFlagBits word48;
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
    u8 filler44[0x4C - 0x44];
    void (*state)(struct HeroBody *);
    u32 unk50;
    u8 filler54[0x6C - 0x54];
    void *anim;
    u8 filler70[0x75 - 0x70];
    u8 unk75;
    u8 mode;
    u8 filler77[0x79 - 0x77];
    u8 flags;
    s16 rate;
    u16 frame;
    s16 progress;
    u8 filler80[0x84 - 0x80];
    struct HeroSpan span;
    u8 filler88[0x92 - 0x88];
    u16 height;
    u8 filler94[0xAC - 0x94];
    s16 step;
    u16 timer;
    s16 jitterX;
    s16 jitterY;
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

struct HeroNode
{
    struct HeroNode *next;
    u8 filler4[0x08 - 0x04];
    struct HeroSprite *sprite;
};

struct World
{
    u8 filler0[0x0A];
    u8 modeByte;
    u8 filler0B;
    u8 flagsC;
    u8 filler0D;
    u8 byte0E;
    u8 filler0F[0x70 - 0x0F];
    struct Hero *heroA;
    struct Hero *heroB;
    struct HeroNode *nodes;
};

extern struct World *gWorld;
extern u8 *gParty;
extern u8 gSys[];

void hero_b_set_rate_if_idle(struct HeroBody *body);
void hero_b_visible_anim_b_rate_a_alt(struct HeroBody *body);
void hero_a_active_link_place_finish(struct HeroBody *body);
void hero_b_visible_anim_b_rate_a(struct HeroBody *body);
void hero_pair_place_and_finish(struct HeroBody *body);
void hero_pair_b_active_slot_anim_next(struct HeroBody *body);
void hero_a_active_step_fx_anim_alt(struct HeroBody *body);
void hero_a_jitter_step_alt_7d58(struct HeroBody *body);
void hero_a_sprite_active_place_target_alt(struct HeroBody *body);
void set_motion_start(struct HeroBody *body, u32 speed);
void hero_a_pair_step_83f8(struct HeroBody *body);
void hero_a_clear_partner_flag_anim(struct HeroBody *body);
void enter_finish_rebuild(struct HeroBody *body);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void set_motion_offsets(struct HeroBody *body, u32 offsets);
void hero_pair_pose_setup_place_finish(struct HeroBody *body);
void jitter_place_a_hero_pair_place_both_finish_alt(struct HeroBody *body);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 z, struct HeroBody *owner);
void set_slot_value(u32 id, s32 value);
void state_stack_advance_progress(struct HeroBody *body);
void hero_b_visible_set_rate_20(struct HeroBody *body);
void hero_b_wait_idle(struct HeroBody *body);
void enter_finish_flag(struct HeroBody *body);
void hero_b_step_c94a0(struct HeroBody *body);
void obj_stop(struct HeroBody *obj);
void schedule_pose_task(struct HeroBody *obj);
void step_motion(struct HeroBody *body);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);

void hero_a_sprite_active_place_target_alt(struct HeroBody *body)
{
    struct Hero *hero_a = gWorld->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct HeroData *data;
    s32 tx;
    s32 ty;
    s32 th;
    s32 x;
    s32 y;
    s32 h;

    if ((body_a->sprite->flags & 8) != 0)
    {
        if ((hero_a->body.mode & 6) == 2 || (hero_a->body.mode & 6) == 4)
        {
            sprite_set_action(body_a, 5, 0x2034, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            data = body_a->data;
            tx = data->targetX;
            if (tx < 0)
                tx += 0xFF;
            x = (tx >> 8) + 9;
            ty = data->targetY;
            if (ty < 0)
                ty += 0xFF;
            y = (ty >> 8) + 0x10;
            th = data->targetH;
            if (th < 0)
                th += 0xFF;
            h = th >> 8;
            place_targets(body_a, x, y, h, -1);
            th = body_a->targetZ;
            if (th < 0)
                th += 0xFF;
            body_a->height = (th >> 8) + 0x20;
            set_motion_offsets(body_a, 0x400);
        }
        body->state = hero_a_active_step_fx_anim_alt;
    }
}

void hero_a_active_step_fx_anim_alt(struct HeroBody *body)
{
    struct Hero *hero = gWorld->heroA;
    struct HeroBody *body_a = &hero->body;
    s32 tx;
    s32 ty;
    s32 tz;
    s32 x;
    s32 y;
    s32 z;

    step_motion(body_a);
    if ((hero->body.flags & 0x20) != 0)
    {
        set_slot_value(0x2E, -1);
        tx = body_a->posX;
        if (tx < 0)
            tx += 0xFF;
        x = tx >> 8;
        ty = body_a->posY;
        if (ty < 0)
            ty += 0xFF;
        y = ty >> 8;
        tz = body_a->posZ;
        if (tz < 0)
            tz += 0xFF;
        z = tz >> 8;
        effect_spawn_at(0x1E, x, y, z, body_a);
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            sprite_set_action(body_a, 7, 0x2034, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
        body->state = jitter_place_a_hero_pair_place_both_finish_alt;
    }
}

void jitter_place_a_hero_pair_place_both_finish_alt(struct HeroBody *body)
{
    struct Hero *hero_a = gWorld->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct HeroBody *body_b = &gWorld->heroB->body;
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

void hero_a_clear_partner_flag_anim(struct HeroBody *body)
{
    struct Hero *hero_a = gWorld->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct HeroBody *partner = body->partner;

    ((struct HeroDataFlagBits *)&partner->data->flags111)->bit1 = 0;
    if (partner->data->unkF6 == 0)
        obj_stop(partner);
    if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
    {
        sprite_set_action(body_a, 6, 0x2034, 0);
        ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
    }
    body->state = hero_a_sprite_active_place_target_alt;
}

void hero_b_sprite_active_anim3_to_3bec(struct HeroBody *body)
{
    struct Hero *hero = gWorld->heroB;
    struct HeroBody *body_b = &hero->body;

    if ((body_b->sprite->flags & 8) != 0)
    {
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            sprite_set_action(body_b, 3, 0x2063, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
        }
        body->state = hero_b_visible_anim_b_rate_a_alt;
    }
}

void hero_a_step_sprite_anim5_to_3754(struct HeroBody *body)
{
    struct Hero *hero = gWorld->heroA;
    struct HeroBody *body_a = &hero->body;

    step_motion(body_a);
    if ((body_a->sprite->flags & 8) != 0)
    {
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            sprite_set_action(body_a, 5, 0x2034, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        body->state = hero_a_active_link_place_finish;
    }
}

void hero_b_sprite_active_anim3_to_2f20(struct HeroBody *body)
{
    struct Hero *hero = gWorld->heroB;
    struct HeroBody *body_b = &hero->body;

    if ((body_b->sprite->flags & 8) != 0)
    {
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            sprite_set_action(body_b, 3, 0x2063, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
        }
        body->state = hero_b_visible_anim_b_rate_a;
    }
}

void hero_a_sprite_active_anim8_to_2520(struct HeroBody *body)
{
    struct Hero *hero = gWorld->heroA;
    struct HeroBody *body_a = &hero->body;

    if ((body_a->sprite->flags & 8) != 0)
    {
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            sprite_set_action(body_a, 8, 0x2034, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
        body->state = hero_pair_place_and_finish;
    }
}

void hero_b_step_active_to_176c(struct HeroBody *body)
{
    struct Hero *hero = gWorld->heroB;

    step_motion(&hero->body);
    if ((hero->body.flags & 0x20) != 0)
        body->state = hero_b_set_rate_if_idle;
}

u32 hero_pair_start_store_pose(struct HeroBody *body)
{
    struct HeroData *data;
    u8 *record;
    struct HeroBody *body_a;
    struct HeroBody *body_b;
    struct HeroBody *partner;
    void *anim;

    record = gParty - ((((body->data->poseIndex << 4) - body->data->poseIndex) << 2) + 0x3C);
    data = body->data;
    data->poseValue = *(u16 *)(record + 0x2A);
    body_a = &gWorld->heroA->body;
    body_b = &gWorld->heroB->body;
    partner = body->partner;
    body_b->partner = partner;
    body_a->partner = partner;
    anim = body->anim;
    body_b->anim = anim;
    body_a->anim = anim;
    partner->unk50 = 0;
    body_b->unk50 = 0;
    body_a->unk50 = 0;
    partner->frame = 0x100;
    body_b->frame = 0x100;
    body_a->frame = 0x100;
    body->state = hero_pair_pose_setup_place_finish;
    body_b->count = 0;
    body_a->count = 0;
    schedule_pose_task(partner);
    return 1;
}

void hero_pair_pose_setup_place_finish(struct HeroBody *body)
{
    struct Hero *hero_a;
    struct Hero *hero_b;
    struct HeroBody *body_a;
    struct HeroBody *body_b;
    struct HeroData *data;
    struct PoseRecord *pose;
    struct World *world;
    s32 bit3;
    s32 bit5;
    s32 mode_bits;
    s32 tx;
    s32 ty;
    s32 tz;
    s32 x;
    s32 y;
    s32 z;

    gWorld->byte0E = 6;
    world = gWorld;
    pose = (struct PoseRecord *)gParty;
    bit3 = (pose->byte46 >> 3) & 1;
    bit5 = pose->word48.field5;
    bit5 &= 1;
    mode_bits = (bit3 + (bit5 << 1)) << 6;
    world->modeByte = (world->modeByte & 0x3F) | mode_bits;

    hero_a = gWorld->heroA;
    body_a = &hero_a->body;
    hero_b = gWorld->heroB;
    body_b = &hero_b->body;
    body_b->unk75 = body_a->unk75 - 0x10;
    set_slot_value(0x53, -1);

    if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
    {
        data = body_a->data;
        tx = data->targetX;
        if (tx < 0)
            tx += 0xFF;
        x = (tx >> 8) + 8;
        ty = data->targetY;
        if (ty < 0)
            ty += 0xFF;
        y = (ty >> 8) + 0x10;
        tz = data->targetH;
        if (tz < 0)
            tz += 0xFF;
        z = tz >> 8;
        place_targets(body_a, x, y, z, -1);
        set_motion_start(body_a, 0x200);
    }
    if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
    {
        sprite_set_action(body_a, 0, 0x2034, 0);
        ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
    }
    if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
    {
        body_a->state = 0;
        enter_finish_flag(body_a);
    }
    if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
    {
        data = body_b->data;
        tx = data->targetX;
        if (tx < 0)
            tx += 0xFF;
        x = (tx >> 8) - 10;
        ty = data->targetY;
        if (ty < 0)
            ty += 0xFF;
        y = (ty >> 8) - 0x10;
        tz = data->targetH;
        if (tz < 0)
            tz += 0xFF;
        z = tz >> 8;
        place_targets(body_b, x, y, z, -1);
        set_motion_start(body_b, 0x200);
    }
    if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
    {
        sprite_set_action(body_b, 0, 0x2063, 0);
        ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 0;
    }
    body->state = hero_b_wait_idle;
}

void hero_b_step_rate_16_when_a_idle(struct HeroBody *body)
{
    struct Hero *hero_a = gWorld->heroA;
    struct Hero *hero_b = gWorld->heroB;
    u32 state_a = (u32)hero_a->body.state;

    if (state_a == 0)
    {
        if ((hero_b->body.mode & 6) == 2 || (hero_b->body.mode & 6) == 4)
        {
            hero_b->body.rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0x10);
            hero_b->body.progress = state_a;
            ((struct HeroFlagBits *)&hero_b->body.flags)->active = 0;
        }
        body->state = hero_pair_b_active_slot_anim_next;
    }
}

void hero_pair_b_active_slot_anim_next(struct HeroBody *body)
{
    struct Hero *hero_a = gWorld->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct Hero *hero_b = gWorld->heroB;
    struct HeroBody *body_b = &hero_b->body;

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
        body->state = hero_b_visible_set_rate_20;
    }
}

void hero_b_visible_set_rate_20(struct HeroBody *body)
{
    struct Hero *hero_b = gWorld->heroB;
    struct HeroBody *body_b = &hero_b->body;

    if ((body_b->sprite->flags & 8) != 0)
    {
        set_slot_value(0x88, -1);
        if ((hero_b->body.mode & 6) == 2 || (hero_b->body.mode & 6) == 4)
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
        body->state = hero_b_step_c94a0;
    }
}
