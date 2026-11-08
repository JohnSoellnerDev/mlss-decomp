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

struct SpriteModeBits
{
    u8 lo1 : 1;
    u8 mode : 2;
    u8 hi5 : 5;
};

struct FlagBit5
{
    u8 low : 5;
    u8 bit5 : 1;
    u8 high : 2;
};

struct FlagBit1
{
    u8 lo : 1;
    u8 bit1 : 1;
    u8 hi : 6;
};

struct SpriteNode
{
    void *next;
    struct Sprite *sprite;
    void **link;
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
extern void anim7_when_shown(struct Body *body);
extern void setup_group_motion(struct Body *body);
extern struct Body *obj_spawn_linked(struct Body *parent, s32 kind, s32 zero);
extern void end_anim_when_active_b(void);
extern void anim4_with_effect_rate(struct Body *body);
extern void arrange_group_on_ellipse(struct Body *body);
extern s32 candidates_advance_progress(struct Body *body);
extern void settle_members_then_orbit(struct Body *body);
extern void orbit_parts_sway(void);
extern void set_motion_start(struct Body *body, u32 speed);
extern s8 gScriptA[];
extern void companion_jump_land_step_toward_partner(void);
extern void place_chain_nodes(void);
extern void *sprite_node_take(void);
extern void *sprite_node_alloc(s32 x, s32 y, s32 z);
extern void start_timed_motion_if_free(struct Body *body);

void anim4_with_effect_rate(struct Body *body)
{
    if (body->sprite->flags & 8)
    {
        set_slot_value(0x116, -1);
        sprite_set_action(body, 4, 0, 0);
        ((struct SpriteModeBits *)&body->sprite->flags)->mode = 0;
        if (body->data->unkF6 != 0)
        {
            body->unk7A = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0x4000, 0x3C);
            body->unk7E = 0;
            ((struct FlagBit5 *)&body->flags)->bit5 = 0;
        }
        body->unkA8 = 0;
        body->state = advance_and_fx_when_ready;
    }
}

void advance_and_fx_when_ready(struct Body *body)
{
    state_stack_advance_progress(body);
    body->unkA8 += 1;
    if (body->unkA8 > 12)
    {
        body->unkA8 = 0;
        effect_spawn_at(0x2622, body->posX / 0x100 + body->landX, body->posY / 0x100 + body->landY,
                        body->posZ / 0x100 + body->landZ, body);
    }
    if ((body->flags & 0x20) != 0)
    {
        s32 bits;
        s32 flags;
        struct Sprite *spr;

        set_slot_value(0x85, -1);
        sprite_set_action(body, 5, 0, 0);
        spr = body->sprite;
        flags = spr->flags;
        bits = -7;
        bits &= flags;
        bits |= 2;
        spr->flags = bits;
        body->state = (void *)spawn_effect_child;
    }
}

void spawn_effect_child(struct Body *body)
{
    if ((body->sprite->flags & 8) != 0)
    {
        struct Body *child;
        s32 bits;
        s32 flags;
        struct Sprite *spr;

        effect_spawn_at(0x263A, body->posX / 0x100 + body->landX, body->posY / 0x100 + body->landY,
                        body->posZ / 0x100 + body->landZ, body);
        child = obj_spawn_linked(body, 0x4040, 0);
        child->posX += body->landX << 8;
        child->posY += body->landY << 8;
        child->posZ += body->landZ << 8;
        child->flags78 = (child->flags78 & 0x1F) | 0x40;
        child->state = (void *)setup_group_motion;
        sprite_set_action(body, 6, 0, 0);
        spr = body->sprite;
        flags = spr->flags;
        bits = -7;
        bits &= flags;
        bits |= 2;
        spr->flags = bits;
        body->state = (void *)anim7_when_shown;
    }
}

void setup_group_motion(struct Body *body)
{
    struct Body *partner = body->partner;

    obj_list_push_default(body);
    body->unk58 = (void *)link_partner_effect_remove;
    body->unk60 = (void *)slot_effect_then_detach;
    if (body->data->unkF6 != 0)
    {
        s32 z = body->unk9C;

        place_targets(body, partner->targetX / 0x100, partner->targetY / 0x100, z, -1);
        body->unk92 = body->unkA4;
        set_motion_offsets(body, body->unkA0 << 8);
    }
    body->state = (void *)step_then_remove_if_flagged;
}

void step_and_detach_child(struct Body *body)
{
    struct Body *partner = body->partner;

    step_motion(body);
    if ((body->flags & 0x20) != 0)
    {
        struct Body *member = body->data->member;
        s32 minusOne = -1;

        set_slot_value(0xAD, minusOne);
        resolve_contact_action(member, partner, 0, 0, (member->data->pose != minusOne) ? 0x13 : 0x12);
        partner->unkB8 = 0x193;
        effect_spawn_at(0x2659, partner->posX / 0x100, partner->posY / 0x100, partner->posZ / 0x100, partner);
        obj_detach_active(body->sprite);
        body->state = (void *)remove_when_group_idle;
    }
}

void remove_when_group_idle(struct Body *body)
{
    if (body->data->group->state == 0)
    {
        obj_release(body);
    }
}

void step_then_remove_if_flagged(struct Body *body)
{
    step_motion(body);
    if ((body->flags & 0x20) != 0)
    {
        obj_release(body);
    }
}

s32 slot_effect_then_detach(struct Body *body, struct Body *other)
{
    s32 slot = lookup_slot(body, other);

    if (slot == 0)
    {
        effect_spawn_at(0x2661, body->posX / 0x100, body->posY / 0x100, body->posZ / 0x100, body);
        obj_list_remove(body);
        body->partner = (struct Body *)((u8 *)body->data + 8);
        body->state = (void *)group_targets_then_step;
    }
    return slot;
}

s32 link_partner_effect_remove(struct Body *body)
{
    s32 result = partner_link_update(body);
    effect_spawn_at(0x2651, body->posX / 0x100, body->posY / 0x100, body->posZ / 0x100, body);
    obj_release(body);
    return result;
}

void anim7_when_shown(struct Body *body)
{
    if (body->sprite->flags & 8)
    {
        sprite_set_action(body, 7, 0, 0);
        ((struct SpriteModeBits *)&body->sprite->flags)->mode = 0;
        body->state = 0;
    }
}

s32 enter_anim_by_sprite_key(struct Body *body)
{
    body->unk50 = 0;
    body->state = (void *)anim_by_sprite_key;
    body->unk7C = 0x100;
    return 1;
}

void group_targets_then_step(struct Body *body)
{
    struct Body *partner = body->partner;

    if (body->data->unkF6 != 0)
    {
        u16 v;

        place_targets(body, partner->targetX / 0x100, partner->targetY / 0x100, 0x10, -1);
        v = 0x10;
        body->unk92 = v;
        set_motion_offsets(body, 0x600);
    }
    body->state = (void *)step_and_detach_child;
}

NONMATCH("asm/code_0809F588/build_part_chain.s", void build_part_chain(struct Body *body))
{
    struct Body *part;
    void *node;
    void **link;
    s32 i;

    if ((body->sprite->flags & 8) != 0)
    {
        sprite_set_action(body, 0x19, 0, 0);
        ((struct SpriteModeBits *)&body->sprite->flags)->mode = 0;
        part = obj_spawn_linked(body, 0x4032, 0);
        part->flags78 = (part->flags78 & 0x1F) | 0x40;
        ((struct FlagBit1 *)&part->flags)->bit1 = 1;
        part->state = (void *)arrange_group_on_ellipse;
        part = obj_spawn_linked(part, 0x4031, 0);
        part->flags78 = (part->flags78 & 0x1F) | 0x40;
        ((struct FlagBit1 *)&part->flags)->bit1 = 1;
        part = obj_spawn_linked(part, 0x4033, 0);
        part->flags78 = (part->flags78 & 0x1F) | 0x40;
        ((struct FlagBit1 *)&part->flags)->bit1 = 1;
        obj_list_push_default(part);
        link = &part->unkC;
        node = sprite_node_take();
        part->unk4 = node;
        ((struct SpriteNode *)node)->sprite = part->sprite;
        ((struct SpriteNode *)node)->link = link;
        for (i = 0; i < 8; i++)
        {
            link = *link = sprite_node_alloc(part->posX, part->posY, part->posZ);
        }
        ((struct SpriteNode *)link)->next = 0;
        link = 0;
        part->state = (void *)place_chain_nodes;
        part = obj_spawn_linked(part, 0x4033, 0);
        part->flags78 = (part->flags78 & 0x1F) | 0x40;
        ((struct FlagBit1 *)&part->flags)->bit1 = 1;
        obj_list_push_default(part);
        set_slot_value(0xD5, -1);
        body->state = link;
    }
}
END_NONMATCH

NONMATCH("asm/code_0809F588/arrange_group_on_ellipse.s", void arrange_group_on_ellipse(struct Body *body))
{
    s32 x;
    s32 y;

    x = (body->data->posX / 0x100) - ((gCosineTable[216] / 0x40) * 0xA0) / 0x100;
    y = (body->data->posY / 0x100) - ((((gSineTable[216] / 0x40) * 0xA0) / 0x100) * 0x8C) / 0x100;
    place_targets(body, x, y, 0x3C, 0x100);
    x = (body->data->posX / 0x100) - ((gCosineTable[40] / 0x40) * 0xA0) / 0x100;
    y = (body->data->posY / 0x100) - ((((gSineTable[40] / 0x40) * 0xA0) / 0x100) * 0x8C) / 0x100;
    place_targets(body->child, x, y, 0x3C, 0x100);
    set_motion_start(body, 0x200);
    set_motion_start(body->child, 0x200);
    body->state = (void *)settle_members_then_orbit;
}
END_NONMATCH

NONMATCH("asm/code_0809F588/orbit_parts_step.s", void orbit_parts_step(struct Body *body))
{
    struct Body *group;
    struct Body *child;
    s16 *cb;
    s16 *sb;
    s32 t;
    s32 modeA;
    s32 modeB;
    s32 pos;
    s32 count;
    s32 entry;
    s32 byte;
    s32 cmd;
    s32 dx;

    group = (struct Body *)((u8 *)body->data + 8);
    t = body->landX * 0xCC + 0x270000;
    if (t < 0)
        t += 0xFF;
    body->landX = t >> 8;
    cb = gCosineTable;
    body->posX = group->targetX - (cb[(body->landX / 0x100) & 0xFF] / 0x40) * 0xA0;
    sb = gSineTable;
    body->posY = group->targetY - ((sb[(body->landX / 0x100) & 0xFF] / 0x40) * 0x5780) / 0x100;
    child = body->child;
    t = child->landX * 0xCC + 0xD0000;
    if (t < 0)
        t += 0xFF;
    child->landX = t >> 8;
    child->posX = group->targetX - (cb[(child->landX / 0x100) & 0xFF] / 0x40) * 0xA0;
    child->posY = group->targetY - ((sb[(child->landX / 0x100) & 0xFF] / 0x40) * 0x5780) / 0x100;
    if ((candidates_advance_progress(body) << 24) != 0)
        return;
    pos = 0;
    for (count = body->unk9C; count > 0; count--)
    {
        entry = gScriptA[pos];
        pos++;
        if (entry != 0)
        {
            do
            {
                byte = gScriptA[pos];
                pos++;
            } while (byte != 0);
        }
    }
    body->unkA8 = pos;
    body->landY = 0;
    cmd = gScriptA[body->unkA8 + 1];
    switch (cmd)
    {
    case 1:
        modeA = 0;
        break;
    case 2:
        modeA = 0;
        break;
    case 3:
        modeA = 0x50;
        break;
    case 4:
        modeA = 0x50;
        break;
    }
    body->unkB2 = modeA;
    cmd = gScriptA[body->unkA8 + 1];
    switch (cmd)
    {
    case 1:
        modeB = 0;
        break;
    case 2:
        modeB = 0x50;
        break;
    case 3:
        modeB = 0;
        break;
    case 4:
        modeB = 0x50;
        break;
    }
    body->child->unkB2 = modeB;
    body->jumpDY = gScriptA[body->unkA8] << 4;
    body->unkA8 += 2;
    if (body->jumpDY < 0)
    {
        dx = 0xC0;
        if ((u8)(body->landX / 0x100) <= 0xC0)
            dx = -0x40;
    }
    else
    {
        dx = 0xC0;
        if ((u8)(body->landX / 0x100) > 0xBF)
            dx = 0x1C0;
    }
    body->jumpDX = dx;
    body->state = (void *)orbit_parts_sway;
}
END_NONMATCH

ASM_FUNC("asm/code_0809F588/orbit_parts_sway.s", void orbit_parts_sway(void))

void land_group_with_effect(struct Body *body)
{
    s32 stepBody = step_motion(body);
    s32 stepChild = step_motion(body->child);
    s32 negBody;
    s32 negChild;

    negBody = -(u8)stepBody;
    negChild = -(u8)stepChild;
    if ((negChild | negBody) >= 0)
    {
        s32 bits;
        s32 flags;
        struct Sprite *spr;

        obj_release(body->child->child->child);
        obj_release(body->child->child);
        obj_release(body->child);
        obj_release(body);
        body = (struct Body *)((u8 *)body->data + 8);
        sprite_set_action(body, 0x18, 0, 0);
        spr = body->sprite;
        flags = spr->flags;
        bits = -7;
        bits &= flags;
        spr->flags = bits;
        effect_spawn_at(0x2AB7, body->posX / 0x100, body->posY / 0x100, body->posZ / 0x100, body);
        body->state = (void *)end_anim_when_active_b;
    }
}
