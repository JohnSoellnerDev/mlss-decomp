#include "global.h"
#include "ram_routines.h"

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
extern void move_between_poses(struct Body *body);
extern void counter_then_begin_motion(struct Body *body);
extern void fall_step_then_remove(struct Body *body);
extern void anim_by_sprite_key(struct Body *body);
extern void stop_anim_when_shown(struct Body *body);
extern void anim_by_partner_pose(struct Body *body);
extern void spawn_child_part(struct Body *body);
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
extern void companion_jump_land_step_toward_partner(struct Body *body);
extern void place_chain_nodes(void);
extern void *sprite_node_take(void);
extern void *sprite_node_alloc(s32 x, s32 y, s32 z);
extern void start_timed_motion_if_free(struct Body *body);

void step_motion_or_remove(struct Body *body)
{
    if ((u8)step_motion(body) == 0)
    {
        obj_release(body);
    }
}

void anim_b_then_wait(struct Body *body)
{
    if (body->data->unk113 == 0)
    {
        s32 bits;
        s32 flags;
        struct Sprite *spr;

        sprite_set_action(body, 0xB, 0, 0);
        spr = body->sprite;
        flags = spr->flags;
        bits = -7;
        bits &= flags;
        spr->flags = bits;
        body->state = (void *)stop_anim_when_shown;
    }
}

s32 part_step_spawn_hero_children(struct Body *body)
{
    body->state = (void *)spawn_hero_children;
    return 1;
}

void stop_anim_when_shown(struct Body *body)
{
    if ((body->sprite->flags & 8) != 0)
    {
        sprite_set_action(body, 0, -1, 0);
        body->state = 0;
    }
}

void move_between_poses(struct Body *body)
{
    struct Body *partner = body->partner;
    struct BodyData *data = partner->data;
    struct BodyData *a;
    struct BodyData *b;

    if (data->pose == -1)
    {
        b = data;
        a = (struct BodyData *)((u8 *)data + 0x130);
    }
    else if (data->pose == -2)
    {
        b = (struct BodyData *)((u8 *)data - 0x130);
        a = data;
    }
    if (b->pose != 0)
    {
        if (a->pose != 0)
        {
            body->landX = (b->limitX + a->limitX) / 2 + body->unkA4 * 0x100;
            body->landY = (b->height + a->height) / 2 + 0x200;
        }
        else
        {
            body->landX = b->posX + body->unkA4 * 0x100;
            body->landY = b->posY + 0x200;
        }
    }
    else
    {
        body->landX = a->posX + body->unkA4 * 0x100;
        body->landY = a->posY + 0x200;
    }
    body->landZ = partner->data->targetH;
    *((u8 *)body + 0x79) |= 0x20;
    body->unk7C = 0x600;
    body->unk7A = 0;
    start_motion(body);
    sprite_set_action(body, 0xF, 0, 0);
    body->state = (void *)start_timed_motion_if_free;
}

ASM_FUNC("asm/code_0809EC74/spawn_child_part.s", void spawn_child_part(struct Body *body))

ASM_FUNC("asm/code_0809EC74/companion_jump_land_step_toward_partner.s", void companion_jump_land_step_toward_partner(struct Body *body))
