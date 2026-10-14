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
    u8 anim;
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

struct HeroChildFlagBits
{
    u8 bit0 : 1;
    u8 unk1 : 1;
    u8 bit2 : 1;
    u8 unk3 : 5;
};

struct HeroCcBits
{
    u8 low : 7;
    u8 high : 1;
};

struct HeroEffectDesc
{
    u8 filler0[0x1C];
    u8 unk1C;
};

struct HeroDataFlagBits
{
    u8 unk0 : 1;
    u8 partnerLinked : 1;
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
    u8 filler0EE[0xF0 - 0xEE];
    struct HeroEffectDesc *effect;
    u8 filler0F4[0xF6 - 0xF4];
    s16 displayTimer;
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
    u8 unk75;
    u8 mode;
    u8 filler77[0x78 - 0x77];
    u8 unk78;
    u8 flags;
    u16 rate;
    u16 frame;
    s16 progress;
    u8 filler80[0x84 - 0x80];
    s16 spanA;
    s16 spanB;
    u32 spanY;
    u8 filler8C[0x92 - 0x8C];
    u16 height;
    u8 filler94[0x9C - 0x94];
    u32 unk9C;
    u32 unkA0;
    u8 filler0A4[0xA8 - 0xA4];
    u32 unkA8;
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

struct World
{
    u8 filler0[0x0A];
    u8 modeByte;
    u8 filler0B[0x0E - 0x0B];
    u8 unk0E;
    u8 filler0F[0x70 - 0x0F];
    struct Hero *heroA;
    struct Hero *heroB;
    u8 filler78[0x24C - 0x78];
    void *trackedObj;
};

extern struct World *gUnk_03000FD8;
extern u8 *gUnk_03000FF4;

void step_motion(struct HeroBody *body);
void step_motion_scaled(struct HeroBody *body);
void sprite_set_action(struct HeroBody *obj, u32 action, u32 arg, u32 extra);
void set_slot_value(u32 id, s32 value);
void body_advance_end_if_active(struct HeroBody *body);
void hero_a_spawn_child_counted(struct HeroBody *body);
void hero_a_child_wait_active(struct HeroBody *body);
void hero_start_step_eighth(struct HeroBody *body);
void body_anim_next_then_end(struct HeroBody *body);
void hero_pair_clear_anim_mirror(struct HeroBody *body);
void set_motion_offsets(struct HeroBody *body, u32 offsets);
void obj_release(void *obj);
void place_targets(struct HeroBody *body, s32 x, s32 y, s32 z, s32 w);
void set_motion_speed_aux(struct HeroBody *body, u32 speed, u32 aux);
void resolve_contact_action(struct HeroBody *a, struct HeroBody *b, u32 c, u32 d, u32 e);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 h, struct HeroBody *owner);
void hero_a_child_remove(struct HeroBody *body);
void state_stack_advance_progress(struct HeroBody *body);
struct HeroBody *obj_spawn_linked(struct HeroBody *parent, u32 kind, u32 value);
void hero_a_place_at_b_target(struct HeroBody *body);
void hero_a_step_press_pose_d0878(struct HeroBody *body);
void enter_finish_rebuild(struct HeroBody *body);
void set_motion_start(struct HeroBody *body, u32 speed);
u32 find_frame_entry(struct HeroBody *obj, u32 index);
void body_step_window_d16c0(struct HeroBody *body);

ASM_FUNC("asm/code_080D0374/hero_body_blend_to_a.s", void hero_body_blend_to_a(void))

void hero_start_step_eighth(struct HeroBody *body)
{
    if ((body->mode & 6) == 2 || (body->mode & 6) == 4)
    {
        body->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 8);
        body->progress = 0;
        ((struct HeroFlagBits *)&body->flags)->active = 0;
    }
    body->state = body_advance_end_if_active;
}

void hero_a_place_targets_anim5(struct HeroBody *body)
{
    struct HeroBody *body_a;
    u8 mode;

    mode = gUnk_03000FD8->modeByte;
    gUnk_03000FD8->modeByte = mode | 0xC0;
    body_a = &gUnk_03000FD8->heroA->body;
    if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
    {
        s32 x = (body_a->targetX / 256) + body_a->ofsX;
        s32 y = (body_a->targetY / 256) + body_a->ofsY;
        place_targets(body_a, x, y, 0, -1);
        {
            u16 h = 0x14;
            body_a->height = h;
        }
        set_motion_offsets(body_a, 0x800);
    }
    if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
    {
        if (body_a->data->poseIndex == -1)
        {
            sprite_set_action(body_a, 5, 0x2000, 0);
        }
        else
        {
            sprite_set_action(body_a, 5, 0x204D, 0);
        }
        ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
    }
    ((struct HeroMirrorBits *)&body_a->sprite->mirror)->flip = 1;
    body->state = hero_pair_clear_anim_mirror;
}

void hero_pair_clear_anim_mirror(struct HeroBody *body)
{
    struct Hero *hero_a = gUnk_03000FD8->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct Hero *hero_b = gUnk_03000FD8->heroB;
    struct HeroBody *body_b = &hero_b->body;

    step_motion(body_a);
    if ((body_a->flags & 0x20) != 0)
    {
        set_slot_value(0x65, -1);
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            sprite_set_action(body_b, 0, -1, 0);
            ((struct HeroMirrorBits *)&body_b->sprite->mirror)->flip = 0;
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 0;
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            sprite_set_action(body_a, 0, -1, 0);
            ((struct HeroMirrorBits *)&body_a->sprite->mirror)->flip = 0;
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 0;
        }
        ((struct HeroMirrorBits *)&body_a->sprite->mirror)->flip = 1;
        body->state = hero_a_spawn_child_counted;
    }
}

void hero_a_spawn_child_counted(struct HeroBody *body)
{
    struct Hero *hero_a = gUnk_03000FD8->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct HeroBody *body_b = &gUnk_03000FD8->heroB->body;
    struct HeroBody *child;

    if ((s32)body_a->unkA0 <= 0)
    {
        if (body_a->child->state == 0)
        {
            if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
            {
                body_a->state = hero_a_place_at_b_target;
            }
        }
    }
    else
    {
        set_slot_value(0x9C, -1);
        body_a->unkA0 = body_a->unkA0 - 1;
        child = obj_spawn_linked(body_a, 0x208A, 0);
        child->posX += 0xE00;
        child->posZ += 0x1000;
        ((struct HeroChildFlagBits *)&child->flags)->bit0 = 0;
        ((struct HeroChildFlagBits *)&child->flags)->bit2 = 1;
        child->sprite->unkE = body_a->sprite->unkE - 3;
        if ((child->mode & 6) == 2 || (child->mode & 6) == 4)
        {
            sprite_set_action(child, 1, 0x2092, 0);
            ((struct HeroSpriteBits *)&child->sprite->flags)->mode = 0;
        }
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            sprite_set_action(body_a, 2, 0x2036, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
        ((struct HeroMirrorBits *)&body->sprite->mirror)->flip = 0;
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            sprite_set_action(body_b, 0, 0x2054, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
        }
        {
            struct HeroBody *old = body_a->child;

            if ((old->mode & 6) == 2 || (old->mode & 6) == 4)
            {
                s32 x = (body_b->targetX / 256) + body_b->ofsX + 6;
                s32 y = (body_b->targetY / 256) + body_b->ofsY;
                place_targets(old, x, y, 0xC, -1);
                {
                    struct HeroBody *new_child = body_a->child;
                    u16 h = 0x64;
                    new_child->height = h;
                }
                set_motion_offsets(body_a->child, 0x400);
            }
        }
        body_a->child->timer = body_a->child->timer & 0xFFF;
        body->state = hero_a_step_press_pose_d0878;
    }
}

ASM_FUNC("asm/code_080D0374/hero_a_step_press_pose_d0878.s", void hero_a_step_press_pose_d0878(struct HeroBody *body))

void hero_a_child_finish_place(struct HeroBody *body)
{
    struct Hero *hero_a = gUnk_03000FD8->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct Hero *hero_b = gUnk_03000FD8->heroB;
    struct HeroBody *body_b = &hero_b->body;

    if ((body_b->sprite->flags & 8) != 0)
    {
        set_slot_value(0x11F, -1);
        if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
        {
            sprite_set_action(body_b, 0, 0x204D, 0);
            ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 0;
        }
        obj_release(body_a->child);
        if ((body_a->child->mode & 6) == 2 || (body_a->child->mode & 6) == 4)
        {
            s32 x = (body_a->targetX / 256) + body_a->ofsX;
            s32 y = (body_a->targetY / 256) + body_a->ofsY;
            s32 z = (body_a->targetZ / 256) + 8;
            place_targets(body_a->child, x, y, z, -1);
            set_motion_speed_aux(body_a->child, 0, 0x66);
        }
        body->state = hero_a_child_wait_active;
    }
}

void hero_pair_place_both_finish_a_idle(struct HeroBody *body);
void hero_child_place_on_partner(struct HeroBody *body);

void hero_a_child_wait_active(struct HeroBody *body)
{
    struct Hero *hero_a = gUnk_03000FD8->heroA;
    struct HeroBody *body_a = &hero_a->body;

    step_motion_scaled(body_a->child);
    if ((body_a->child->flags & 0x20) != 0)
    {
        set_slot_value(0x9C, -1);
        if ((body_a->mode & 6) == 2 || (body_a->mode & 6) == 4)
        {
            sprite_set_action(body_a, 5, 0x2036, 0);
            ((struct HeroSpriteBits *)&body_a->sprite->flags)->mode = 1;
        }
        if ((body_a->child->mode & 6) == 2 || (body_a->child->mode & 6) == 4)
        {
            body_a->child->state = hero_child_place_on_partner;
        }
        body->state = hero_pair_place_both_finish_a_idle;
    }
}

ASM_FUNC("asm/code_080D0374/hero_pair_place_both_finish_a_idle.s", void hero_pair_place_both_finish_a_idle(struct HeroBody *body))

void hero_a_child_link_partner_fx(struct HeroBody *body);

void hero_child_place_on_partner(struct HeroBody *body)
{
    struct Hero *hero_a = gUnk_03000FD8->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct HeroBody *partner = body->partner;
    struct HeroBody *child = body_a->child;

    if ((child->mode & 6) == 2 || (child->mode & 6) == 4)
    {
        s32 x = (partner->targetX / 256) + partner->ofsX;
        s32 y = (partner->targetY / 256) + partner->ofsY;
        s32 z = (partner->targetZ / 256) + partner->ofsZ - 8;
        place_targets(child, x, y, z, -1);
        {
            struct HeroBody *new_child = body_a->child;
            u16 h = 0x20;
            new_child->height = h;
        }
        set_motion_offsets(body_a->child, 0x800);
    }
    body->state = hero_a_child_link_partner_fx;
}

void hero_a_child_link_partner_fx(struct HeroBody *body)
{
    struct Hero *hero_a = gUnk_03000FD8->heroA;
    struct HeroBody *body_a = &hero_a->body;
    struct HeroBody *partner = body->partner;

    step_motion(body_a->child);
    if ((body_a->child->flags & 0x20) != 0)
    {
        ((struct HeroDataFlagBits *)&partner->data->flags111)->partnerLinked = 0;
        set_slot_value(0x55, -1);
        ((struct HeroCcBits *)&body_a->child->cc)->low = 0x46;
        body_a->child->scale = 0x100;
        resolve_contact_action(body_a->child, partner, 0, 0, 0x0413438C);
        body_a->child->count = body_a->child->count + 1;
        if (partner->data->displayTimer > 0 || (partner->data->flags111 & 2) != 0)
        {
            s32 x = (partner->targetX / 256) + partner->ofsX;
            s32 y = (partner->targetY / 256) + partner->ofsY;
            effect_spawn_at(0x7D, x, y, 0, partner);
        }
        body->state = hero_a_child_remove;
    }
}

void body_place_at_partner_target(struct HeroBody *body);

void hero_b_anim_and_step_rate8_alt(struct HeroBody *body)
{
    struct Hero *hero_b = gUnk_03000FD8->heroB;
    struct HeroBody *body_b = &hero_b->body;

    set_slot_value(0xAF, -1);
    if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
    {
        sprite_set_action(body_b, 2, 0x2054, 0);
        ((struct HeroSpriteBits *)&body_b->sprite->flags)->mode = 1;
    }
    if ((body_b->mode & 6) == 2 || (body_b->mode & 6) == 4)
    {
        body_b->rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 8);
        body_b->progress = 0;
        ((struct HeroFlagBits *)&body_b->flags)->active = 0;
    }
    body->state = body_place_at_partner_target;
}

void body_place_at_partner_target(struct HeroBody *body)
{
    struct HeroBody *body_a = &gUnk_03000FD8->heroA->body;
    struct HeroBody *body_b = &gUnk_03000FD8->heroB->body;
    struct HeroBody *partner = body->partner;

    state_stack_advance_progress(body_b);
    step_motion(body);
    if ((body_b->flags & 0x20) != 0)
    {
        set_slot_value(0x7D, -1);
        if ((body->mode & 6) == 2 || (body->mode & 6) == 4)
        {
            s32 x = (partner->targetX / 256) + partner->ofsX;
            s32 y = (partner->targetY / 256) + partner->ofsY;
            s32 z = (partner->targetZ / 256) + partner->ofsZ - 10;
            s32 height;
            s32 sq;

            place_targets(body, x, y, z, -1);
            if (partner == (struct HeroBody *)((u8 *)partner->data + 8))
            {
                height = partner->ofsZ;
            }
            else
            {
                find_frame_entry(partner, 0);
                height = partner->unk47;
            }
            height += partner->targetZ / 256;
            sq = height * height;
            height = height + MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x400, (sq / 0x40) + 0x20);
            body->height = height;
            set_motion_offsets(body, 0x800);
        }
        ((struct HeroMirrorBits *)&body->sprite->mirror)->flip = 1;
        body_a->timer = body_a->timer & 0xFFF;
        body->unkA8 = 1;
        body->timer = body->timer & 0xFFF;
        body->state = body_step_window_d16c0;
    }
}

ASM_FUNC("asm/code_080D0374/body_step_window_d16c0.s", void body_step_window_d16c0(struct HeroBody *body))
