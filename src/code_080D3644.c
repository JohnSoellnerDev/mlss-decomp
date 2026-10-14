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

extern struct World *gUnk_03000FD8;
extern u8 *gUnk_03000FF4;
extern struct KeyState gUnk_0300034C;

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

void hero_b_finish_child_anim9(struct HeroBody *body)
{
    struct HeroBody *body_a = &gUnk_03000FD8->heroA->body;
    struct Hero *hero_b = gUnk_03000FD8->heroB;
    struct HeroBody *body_b = &hero_b->body;

    if ((body_b->sprite->flags & 8) != 0)
    {
        obj_release(body_a->child);
        set_slot_value(0x38, -1);
        if ((hero_b->body.mode & 6) == 2 || (hero_b->body.mode & 6) == 4)
        {
            sprite_set_action(body_b, 0, 0x2009, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
        }
        body->state = hero_pair_place_both_finish_sprite_alt;
    }
}

void hero_b_finish_child_anim9_alt(struct HeroBody *body)
{
    struct HeroBody *body_a = &gUnk_03000FD8->heroA->body;
    struct Hero *hero_b = gUnk_03000FD8->heroB;
    struct HeroBody *body_b = &hero_b->body;

    if ((body_b->sprite->flags & 8) != 0)
    {
        obj_release(body_a->child);
        set_slot_value(0x38, -1);
        if ((hero_b->body.mode & 6) == 2 || (hero_b->body.mode & 6) == 4)
        {
            sprite_set_action(body_b, 0, 0x2009, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
        }
        body->state = hero_pair_place_both_finish_sprite;
    }
}

void hero_a_anim_reset_mirror(struct HeroBody *body)
{
    struct Hero *hero = gUnk_03000FD8->heroA;
    struct HeroBody *body_a = &hero->body;

    step_motion(body_a);
    if ((hero->body.flags & 0x20) != 0)
    {
        if ((hero->body.mode & 6) == 2 || (hero->body.mode & 6) == 4)
        {
            sprite_set_action(body_a, 0, -1, 0);
            ((struct HeroMirrorBits *)&body_a->sprite->mirror)->flip = 0;
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        body->state = hero_a_step16_when_b_idle;
    }
}

u32 hero_pair_start_with_pose(struct HeroBody *body)
{
    struct HeroData *data;
    u8 *record;
    struct HeroBody *body_a;
    struct HeroBody *body_b;
    struct HeroBody *partner;
    void *anim;

    record = gUnk_03000FF4 - ((((body->data->poseIndex << 4) - body->data->poseIndex) << 2) + 0x3C);
    data = body->data;
    data->poseValue = *(u16 *)(record + 0x2A);
    body_a = &gUnk_03000FD8->heroA->body;
    body_b = &gUnk_03000FD8->heroB->body;
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
    body->state = hero_pair_place_both_tracked;
    body_b->count = 0;
    body_a->count = 0;
    schedule_pose_task(partner);
    return 1;
}

ASM_FUNC("asm/code_080D3644/hero_pair_place_both_tracked.s", void hero_pair_place_both_tracked(struct HeroBody *body))

void hero_b_step16_when_a_free(struct HeroBody *body)
{
    struct Hero *hero_a = gUnk_03000FD8->heroA;
    struct Hero *hero_b = gUnk_03000FD8->heroB;

    if (hero_a->body.state == 0)
    {
        if ((hero_b->body.mode & 6) == 2 || (hero_b->body.mode & 6) == 4)
        {
            hero_b->body.rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0x10);
            hero_b->body.progress = 0;
            ((struct HeroFlagBits *)&hero_b->body.flags)->active = 0;
        }
        body->state = hero_b_spawn_child_chain;
    }
}

void hero_b_spawn_child_chain(struct HeroBody *body)
{
    struct Hero *hero_b = gUnk_03000FD8->heroB;
    struct HeroBody *body_b = &hero_b->body;
    struct HeroBody *child;

    state_stack_advance_progress(body_b);
    if ((hero_b->body.flags & 0x20) != 0)
    {
        set_slot_value(0x87, -1);
        if ((hero_b->body.mode & 6) == 2 || (hero_b->body.mode & 6) == 4)
        {
            sprite_set_action(body_b, 0, 0x2055, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
        }
        child = obj_spawn_linked(body_b, 0x208B, 0);
        child->flags |= 4;
        child->sprite->unkE = body_b->sprite->unkE - 3;
        child->flags |= 1;
        child->posX += 0x1200;
        child->posZ += 0xE00;
        child = obj_spawn_linked(child, 0, 0);
        sprite_set_action(child, 0, 0x20B9, 0);
        child->posZ += 0x1000;
        ((struct HeroSpriteBits *)&child->sprite->flags)->mode = 0;
        child->flags |= 4;
        child->sprite->unkE = body_b->sprite->unkE + 2;
        {
            s32 f = child->flags;
            child->flags = 1 | f;
        }
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            body_b->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0x78);
            body_b->progress = 0;
            ((struct HeroFlagBits *)&body_b->flags)->active = 0;
        }
        body_b->timer = body_b->timer & 0xFFF;
        body->state = hero_b_step_window_d3c38;
    }
}

ASM_FUNC("asm/code_080D3644/hero_b_step_window_d3c38.s", void hero_b_step_window_d3c38(struct HeroBody *body))
