#include "global.h"

struct Sprite
{
    u8 filler0[0x11];
    u8 b11;
    u8 flags;
    u8 filler13[0x16 - 0x13];
    u16 key16;
    u8 filler18[0x1E - 0x18];
    u8 b1E;
    u8 filler1F[0x21 - 0x1F];
    u8 state;
};

struct BodyData
{
    u8 filler0[0x10];
    struct Sprite *group;
    u8 filler14[0x34 - 0x14];
    struct Body *member;
    u8 filler38[0x40 - 0x38];
    s32 posX;
    s32 posY;
    s32 posZ;
    u8 filler4C[0xD8 - 0x4C];
    s32 limitX;
    s32 height;
    s32 targetH;
    u8 fillerE4[0xEC - 0xE4];
    s16 pose;
    u8 fillerEE[0xF6 - 0xEE];
    s16 unkF6;
    u8 fillerF8[0x102 - 0xF8];
    s16 unk102;
    s16 unk104;
    u8 filler106[0x10A - 0x106];
    u16 unk10A;
    u16 unk10C;
    u8 unk10E[0x113 - 0x10E];
    u8 unk113;
    u32 unk114;
};

struct Body
{
    u8 filler0[0x4];
    void *unk4;
    struct Sprite *sprite;
    void *unkC;
    s32 posX;
    s32 posY;
    s32 posZ;
    u8 filler1C[0x28 - 0x1C];
    struct BodyData *data;
    struct Body *partner;
    struct Body *child;
    u8 filler34[0x38 - 0x34];
    s32 targetX;
    s32 targetY;
    s32 targetZ;
    u8 filler44[0x4C - 0x44];
    void *state;
    void *unk50;
    void *unk54;
    void *unk58;
    void *unk5C;
    void *unk60;
    u8 filler64[0x68 - 0x64];
    void *unk68;
    u32 unk6C;
    u8 filler70[0x78 - 0x70];
    u8 flags78;
    u8 flags;
    u16 unk7A;
    u16 unk7C;
    u16 unk7E;
    u32 unk80;
    s32 landX;
    s32 landY;
    s32 landZ;
    u8 filler90[0x92 - 0x90];
    u16 unk92;
    u8 filler94[0x9C - 0x94];
    s32 unk9C;
    u32 unkA0;
    u32 unkA4;
    s32 unkA8;
    u16 counter;
    u16 jumpDX;
    s16 jumpDY;
    u16 unkB2;
    u8 fillerB4[0xB8 - 0xB4];
    u16 unkB8;
};

extern void spawn_hero_children(void);
extern void spawn_hero_children(void);
extern void move_between_poses(void);
extern void counter_then_begin_motion(struct Body *body);
extern void fall_step_then_remove(struct Body *body);
extern void anim_by_sprite_key(struct Body *body);
extern void stop_anim_when_shown(struct Body *body);
extern void anim_by_partner_pose(struct Body *body);
extern void spawn_child_part(void);
extern void group_targets_then_step(struct Body *body);
extern void step_and_detach_child(struct Body *body);
extern void advance_and_fx_when_ready(struct Body *body);
extern void remove_when_group_idle(struct Body *body);
extern void obj_list_push_default(struct Body *body);
extern void obj_detach_active(struct Sprite *sprite);
extern void resolve_contact_action(struct Body *member, struct Body *partner, s32 a, s32 b, s32 code);
extern s32 link_partner_effect_remove(struct Body *body);
extern void step_then_remove_if_flagged(struct Body *body);
extern s32 slot_effect_then_detach(struct Body *body, struct Body *other);
extern void wait_for_index(void);
extern void set_state_when_shown(struct Body *body);
extern void on_arrive(void);
extern s32 step_motion(struct Body *body);
extern void obj_release(struct Body *body);
extern void obj_stop(struct Body *body);
extern void obj_list_remove(struct Body *body);
extern void start_motion(struct Body *body);
extern void sprite_set_action(struct Body *body, s32 action, s32 a, s32 b);
extern void motion_timed(struct Body *body, s32 frames, s32 zero);
extern void set_slot_value(s32 slot, s32 value);
extern s32 partner_link_update(struct Body *body);
extern s32 lookup_slot(struct Body *body, struct Body *other);
extern s32 query_contact(struct Body *body, struct Body *other, s32 arg);
extern void apply_partner_effect(struct Body *a, struct Body *b, s32 strength, s32 c, s32 x, s32 z, s32 d);
extern void reset_to_counter_state(struct Body *body);
extern void effect_spawn_at(s32 effect, s32 x, s32 y, s32 z, struct Body *body);
extern void place_targets(struct Body *body, s32 x, s32 y, s32 z, s32 flag);
extern void set_motion_offsets(struct Body *body, s32 value);
extern void move_step(s16 *x, s16 *y, s16 *z, s32 dx, s32 dy, s32 dz, s32 speed);
extern void state_stack_advance_progress(struct Body *body);
extern void spawn_effect_child(struct Body *body);
extern void anim7_when_shown(void);
extern void setup_group_motion(struct Body *body);
extern struct Body *obj_spawn_linked(struct Body *parent, s32 kind, s32 zero);
extern void end_anim_when_active_b(void);
extern void anim4_with_effect_rate(void);
extern void arrange_group_on_ellipse(void);
extern void companion_jump_land_step_toward_partner(void);
extern void place_chain_nodes(void);
extern void *sprite_node_take(void);
extern void *sprite_node_alloc(s32 x, s32 y, s32 z);
extern void start_timed_motion_if_free(struct Body *body);

s32 contact_effect(struct Body *body, struct Body *other, s32 arg)
{
    s32 contact = query_contact(body, other, arg);

    if (contact == 0)
    {
        struct BodyData *otherData = other->data;
        s32 otherWord = otherData->unk114;
        s32 otherW = otherData->unk102;
        u16 otherH = otherData->unk10A;
        struct BodyData *bodyData = body->data;
        s32 bodyW = bodyData->unk104;
        u16 bodyH = bodyData->unk10C;
        s32 otherProd;
        s32 strength;
        s32 bodyScale;

        strength = otherW * 0x66;
        otherProd = strength * otherH;
        if (otherProd < 0)
        {
            otherProd += 0xFF;
        }
        strength = otherProd >> 8;
        bodyScale = bodyW * 3;
        bodyScale = bodyScale + (bodyScale << 4);
        otherProd = bodyH * bodyScale;
        if (otherProd < 0)
        {
            otherProd += 0xFF;
        }
        strength -= otherProd >> 8;
        strength /= 2;
        if (otherWord > 0x100)
        {
            s32 scaled = otherWord * strength;

            strength = scaled;
            if (strength < 0)
            {
                scaled = strength + 0xFFFF;
            }
            strength = scaled >> 16;
        }
        else
        {
            s32 scaled = strength;

            if (strength < 0)
            {
                scaled += 0xFF;
            }
            strength = scaled >> 8;
        }
        if (strength <= 1)
        {
            strength = 1;
        }
        apply_partner_effect(other, body, strength, 0, other->posX / 0x100, other->posZ / 0x100, 0);
        reset_to_counter_state(body);
        body->counter = 8;
    }
    return contact;
}

s32 slot_contact_effect(struct Body *body, struct Body *other)
{
    s32 slot = lookup_slot(body, other);

    if (slot == 0)
    {
        struct BodyData *otherData = other->data;
        s32 otherWord = otherData->unk114;
        s32 otherW = otherData->unk102;
        u16 otherH = otherData->unk10A;
        struct BodyData *bodyData = body->data;
        s32 bodyW = bodyData->unk104;
        u16 bodyH = bodyData->unk10C;
        s32 otherProd;
        s32 strength;
        s32 bodyScale;

        strength = otherW * 0x66;
        otherProd = strength * otherH;
        if (otherProd < 0)
        {
            otherProd += 0xFF;
        }
        strength = otherProd >> 8;
        bodyScale = bodyW * 3;
        bodyScale = bodyScale + (bodyScale << 4);
        otherProd = bodyH * bodyScale;
        if (otherProd < 0)
        {
            otherProd += 0xFF;
        }
        strength -= otherProd >> 8;
        strength /= 2;
        if (otherWord > 0x100)
        {
            s32 scaled = otherWord * strength;

            strength = scaled;
            if (strength < 0)
            {
                scaled = strength + 0xFFFF;
            }
            strength = scaled >> 16;
        }
        else
        {
            s32 scaled = strength;

            if (strength < 0)
            {
                scaled += 0xFF;
            }
            strength = scaled >> 8;
        }
        if (strength <= 1)
        {
            strength = 1;
        }
        apply_partner_effect(other, body, strength, 0, other->posX / 0x100, other->posZ / 0x100, 0);
        reset_to_counter_state(body);
        body->counter = 8;
    }
    return slot;
}

void counter_then_begin_motion(struct Body *body)
{
    u16 counter = body->counter;

    if ((s16)body->counter != 0)
    {
        body->counter = counter - 1;
    }
    else
    {
        u8 spriteState = body->sprite->state;

        if (spriteState == 0)
        {
            struct BodyData *group = body->data;
            s32 flags;

            body->landX = group->posX - 0xA00;
            body->landY = group->posY;
            body->landZ = group->posZ + 0xD00;
            flags = body->flags;
            flags |= 0x20;
            body->flags = flags;
            body->unk7C = 0x400;
            body->unk7A = spriteState;
            start_motion(body);
            body->jumpDY = group->posZ + 0x680;
            body->unkB2 = spriteState;
            body->state = (void *)fall_step_then_remove;
        }
    }
}

void fall_step_then_remove(struct Body *body)
{
    u16 *speed = &body->unkB2;
    s32 gap = body->jumpDY - body->targetZ;
    u32 zero;

    if (gap < 0)
    {
        gap += 7;
    }
    *speed = (gap >> 3) + *speed;
    body->posZ += (s16)*speed;
    zero = body->unk80;
    if (zero == 0)
    {
        struct Body *group = (struct Body *)((u8 *)body->data + 8);

        group->unk68 = (void *)on_arrive;
        group->unk6C = body->unk6C;
        body->unk6C = zero;
        if (group->data->unkF6 != 0)
        {
            group->state = (void *)set_state_when_shown;
            sprite_set_action(group, 0x13, 0, 0);
        }
        else
        {
            obj_stop(group);
        }
        obj_release(body);
    }
}

void reset_to_counter_state(struct Body *body)
{
    body->unk80 = 0;
    body->state = (void *)counter_then_begin_motion;
    body->counter = 0;
}

void set_state_when_shown(struct Body *body)
{
    if ((body->sprite->flags & 8) != 0)
    {
        body->state = (void *)wait_for_index;
    }
}

void start_timed_motion_if_free(struct Body *body)
{
    if (body->unk80 == 0)
    {
        struct Body *partner = body->partner;

        body->counter = body->unkA0;
        motion_timed(body, 0xF - partner->data->pose, 0);
        body->unk50 = (void *)anim_by_partner_pose;
        set_slot_value(0x62, -1);
    }
}

s32 part_step_move_between_poses(struct Body *body)
{
    body->state = (void *)move_between_poses;
    return 1;
}

void anim_by_partner_pose(struct Body *body)
{
    struct Body *partner = body->partner;

    if (partner->data->pose == -1)
    {
        sprite_set_action(body, 0x14, 0, 0);
    }
    if (partner->data->pose == -2)
    {
        sprite_set_action(body, 0x15, 0, 0);
    }
    body->state = (void *)spawn_child_part;
}

void anim_by_sprite_key(struct Body *body)
{
    struct Sprite *sprite = body->sprite;
    u32 key = sprite->key16 | ((((u32)sprite->b1E << 28) >> 16) + 0x1000);
    s32 bits;
    s32 flags;
    struct Sprite *spr;

    if (key == 0x4018)
    {
        body->landX = -1;
        body->landY = 1;
        body->landZ = 0x18;
    }
    else if (key == 0x401A)
    {
        body->landX = -1;
        body->landY = 1;
        body->landZ = 0x13;
    }
    else if (key == 0x401E)
    {
        body->landX = -3;
        body->landY = 1;
        body->landZ = 0x13;
    }
    else if (key == 0x401D)
    {
        body->landX = -2;
        body->landY = 1;
        body->landZ = 0x1A;
    }
    else if (key == 0x401C)
    {
        body->landX = -4;
        body->landY = 1;
        body->landZ = 0x16;
    }
    else if (key == 0x4019)
    {
        body->landX = -3;
        body->landY = 1;
        body->landZ = 0x16;
    }
    else if (key == 0x401B)
    {
        body->landX = -5;
        body->landY = 1;
        body->landZ = 0x15;
    }
    sprite_set_action(body, 3, 0, 0);
    spr = body->sprite;
    flags = spr->flags;
    bits = -7;
    bits &= flags;
    bits |= 2;
    spr->flags = bits;
    body->state = (void *)anim4_with_effect_rate;
}
