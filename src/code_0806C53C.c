#include "global.h"
#include "ram_routines.h"

struct ActorPose
{
    u8 filler0[0x11];
    u8 unk11;
    u8 flags;
    u8 filler13[0x20 - 0x13];
    u8 cmd;
};

#define POSE_FLAG_ANIM_DONE 0x08 /* ActorPose.flags: the current animation has finished */

struct Flags79Bits
{
    u8 low5 : 5;
    u8 bit5 : 1;
    u8 high : 2;
};

struct Flags77Bits
{
    u8 low6 : 6;
    u8 bit6 : 1;
    u8 high : 1;
};

struct ActorPart
{
    u8 filler0[0xD8];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler1[0xEC - 0xE4];
    s16 slot;
    u8 filler2[0xF6 - 0xEE];
    s16 unkF6;
};

struct ActorWorld
{
    u8 filler0[0x58];
    u8 *partners[8];
};

extern struct ActorWorld *gWorld;

struct ActorObj
{
    u8 filler0[0x08];
    struct ActorPose *pose;
    u8 filler0C[0x10 - 0x0C];
    s32 posX;
    s32 posY;
    s32 posH;
    u8 filler1C[0x28 - 0x1C];
    struct ActorPart *part;
    struct ActorObj *owner;
    struct ActorObj *child;
    u8 filler34[0x38 - 0x34];
    s32 baseX;
    s32 baseY;
    s32 baseH;
    u8 filler44[0x4C - 0x44];
    void (*state)(struct ActorObj *);
    u8 filler50[0x58 - 0x50];
    void (*handler58)(struct ActorObj *);
    void (*handler5C)(struct ActorObj *);
    void (*handler60)(struct ActorObj *);
    u8 filler64[0x68 - 0x64];
    void (*handler68)(struct ActorObj *);
    u32 unk6C;
    u8 filler70[0x74 - 0x70];
    u8 tag;
    u8 counter;
    u8 filler76[0x77 - 0x76];
    u8 flags77;
    u8 flags78;
    u8 flags79;
    s16 motionStart;
    s16 motionSpeed;
    s32 index;
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler90[0x9C - 0x90];
    s32 param9C;
    s32 paramA0;
    s32 paramA4;
    s32 paramA8;
    u16 timer;
    s16 dx;
    s16 dy;
    s16 dh;
};

void sprite_set_action(struct ActorObj *obj, u32 action, u32 a2, u32 a3);
void set_slot_value(u32 id, s32 value);
void sound_stop_song(u32 song);
void obj_release(struct ActorObj *obj);
u32 lookup_slot(struct ActorObj *obj, u32 slot);
u32 partner_link_update(struct ActorObj *obj, u32 partner, u32 mode);
void start_motion(struct ActorObj *obj);
void obj_stop(struct ActorObj *obj);
void obj_index_zero_action4_exit(struct ActorObj *obj);
void obj_list_push_default(struct ActorObj *obj);
struct ActorObj *obj_spawn(struct ActorPart *part, u32 type, s32 x, s32 y, s32 h, u32 a, u32 b);
void on_arrive(struct ActorObj *obj);
void effect_spawn_at(u32 type, s32 x, s32 y, s32 h, struct ActorObj *owner);
u32 mt19937_next(void);
void obj_spawn_part_child(struct ActorObj *obj);
void obj_pose_active_move_high(struct ActorObj *obj);
void resolve_contact_action(struct ActorObj *owner, struct ActorObj *obj, u32 a, u32 b, u32 code);
u32 query_contact(struct ActorObj *obj, struct ActorObj *other, u32 arg);
void obj_spawn_child_by_slot(struct ActorObj *obj);
extern u32 gUnk_03000E3C;
u32 effect_from_table_obj(struct ActorObj *obj, u32 id);

u32 actor_start_action2_then_move_high(struct ActorObj *obj);
u32 actor_init_side_mode(struct ActorObj *obj);
u32 actor_default_speed(struct ActorObj *obj);
void actor_action2_then_move_high(struct ActorObj *obj);
void actor_release_when_free(struct ActorObj *obj);
void actor_action_from_a4_notify(struct ActorObj *obj);
void actor_stop_when_pose_active(struct ActorObj *obj);
void actor_action5_then_spawn_child(struct ActorObj *obj);
void actor_side_action_when_active(struct ActorObj *obj);
void actor_side_action_timer8_child(struct ActorObj *obj);
void actor_spawn_motion_child(struct ActorObj *obj);
void actor_retarget_to_owner_part(struct ActorObj *obj);
void actor_plan_arc_path(struct ActorObj *obj);
void actor_arc_move_step(struct ActorObj *obj);
void actor_effect_then_wait_index(struct ActorObj *obj);
void actor_spawn_counted_child(struct ActorObj *obj);
void actor_finish_with_child_effect(struct ActorObj *obj);
void actor_finish_child_effect_b(struct ActorObj *obj);
void actor_countdown_release_child(struct ActorObj *obj);
void actor_start_motion_to_part(struct ActorObj *obj);
u32 actor_link_partner_then_step(struct ActorObj *obj, struct ActorObj *other);
void actor_effect_then_clear_index(struct ActorObj *obj);
void actor_action_a_or_12_child(struct ActorObj *obj);
void actor_action8_or_10_timer(struct ActorObj *obj);
u32 actor_wait_for_slot(struct ActorObj *obj, u32 slot);
u32 actor_unlink_partners_and_release(struct ActorObj *obj);
void actor_countdown_release_partners(struct ActorObj *obj);
void actor_release_when_free_b(struct ActorObj *obj);
void actor_wait_index_then_timer(struct ActorObj *obj);
void actor_idle_when_pose_active(struct ActorObj *obj);
u32 actor_set_home_side1(struct ActorObj *obj);
u32 actor_set_home_side0(struct ActorObj *obj);
u32 actor_start_action2_then_action3(struct ActorObj *obj);
void actor_set_home_and_motion(struct ActorObj *obj);
void actor_action2_then_action3(struct ActorObj *obj);
void actor_action3_spawn_effect(struct ActorObj *obj);
void actor_wait_then_side_action(struct ActorObj *obj);
void actor_spawn_effect_set_cmd(struct ActorObj *obj);
void actor_spawn_lift_child(struct ActorObj *obj);
void actor_random_partner_link(struct ActorObj *obj);
void actor_linear_wave_step(struct ActorObj *obj);
u32 actor_vanish_if_slot_missing(struct ActorObj *obj, u32 slot);
u32 actor_link_and_release(struct ActorObj *obj, u32 other);
void actor_countdown_then_timer(struct ActorObj *obj);
u32 actor_link_contact_partner(struct ActorObj *obj, struct ActorObj *other, u32 arg);
u32 actor_set_speed_link(struct ActorObj *obj, u32 other);
void obj_start_effect_by_side(struct ActorObj *obj);
void obj_flip_side_walk(struct ActorObj *obj);
void obj_side_action_timer(struct ActorObj *obj);
u32 obj_pick_random_count(struct ActorObj *obj);
u32 obj_start_count_side1(struct ActorObj *obj);
u32 obj_start_count_side0(struct ActorObj *obj);
void obj_timer_action15(struct ActorObj *obj);
void obj_place_sprite_at_base(struct ActorObj *obj);
void obj_stop_clear_state(struct ActorObj *obj);
void obj_walk_by_side(struct ActorObj *obj);
void obj_pose_action15(struct ActorObj *obj);
void obj_fall_and_launch(struct ActorObj *obj);

/* Actor state: once the animation has finished and the timer runs out, flags this as the last
   round (targetX = 1, read by actor_link_partner_then_step), clears slot 0x119, plays the finishing
   action on the actor and its child (0xB, or 0x13 for the paramA8 variant), starts the matching
   effect (kept in gUnk_03000E3C) and moves on to actor_countdown_release_child, with
   actor_link_partner_then_step as the partner-contact handler. */
void actor_finish_child_effect_b(struct ActorObj *obj)
{
    u32 effectId;

    if (obj->pose->flags & POSE_FLAG_ANIM_DONE)
    {
        s32 timer = obj->timer;

        timer--;
        obj->timer = timer;
        if ((timer << 16) <= 0)
        {
            obj->targetX = 1;
            set_slot_value(0x119, -1);
            if (obj->paramA8 == 0)
            {
                sprite_set_action(obj, 0xB, 0, 0);
                sprite_set_action(obj->child, 0xB, 0, 0);
                effectId = 0x1569;
            }
            else
            {
                sprite_set_action(obj, 0x13, 0, 0);
                sprite_set_action(obj->child, 0x13, 0, 0);
                effectId = 0x15D3;
            }
            gUnk_03000E3C = effect_from_table_obj(obj, effectId);
            obj->handler58 = (void (*)(struct ActorObj *))actor_link_partner_then_step;
            obj->state = actor_countdown_release_child;
        }
    }
}

void actor_countdown_release_child(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        obj->param9C--;
        if (obj->param9C > 0)
        {
            if (obj->paramA8 == 0)
            {
                sprite_set_action(obj, 8, 0, 0);
            }
            else
            {
                sprite_set_action(obj, 0x10, 0, 0);
            }
            obj->timer = 8;
            obj->state = actor_finish_with_child_effect;
        }
        else
        {
            obj_release(obj->child);
            if (obj->paramA8 == 0)
            {
                sprite_set_action(obj, 0xC, 0, 0);
            }
            else
            {
                sprite_set_action(obj, 0x14, 0, 0);
            }
            obj->state = actor_start_motion_to_part;
        }
    }
}

void actor_start_motion_to_part(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0xD, 0, 0);
        obj->targetX = obj->part->targetX;
        obj->targetY = obj->part->targetY;
        obj->targetH = obj->part->targetH;
        ((struct Flags79Bits *)&obj->flags79)->bit5 = 1;
        obj->motionSpeed = 0x180;
        obj->motionStart = 0;
        start_motion(obj);
        obj->timer = 0xA;
        obj->state = actor_effect_then_clear_index;
    }
}

u32 actor_link_partner_then_step(struct ActorObj *obj, struct ActorObj *other)
{
    effect_spawn_at(0x1C4A, other->baseX / 0x100, other->baseY / 0x100, other->baseH / 0x100, obj);
    partner_link_update(obj, (u32)other, 0xFF);
    if (obj->targetX == 0)
    {
        obj->state = actor_action_a_or_12_child;
    }
    else
    {
        obj->state = actor_countdown_release_child;
    }
    return 0;
}

void actor_effect_then_clear_index(struct ActorObj *obj)
{
    s32 t = obj->timer;

    t--;
    obj->timer = t;
    if ((t << 16) <= 0)
    {
        obj->timer = 0xA;
        effect_spawn_at(0x1C5B, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
    }
    if (obj->index == 0)
    {
        sprite_set_action(obj, 0, 0, 0);
        obj->state = 0;
    }
}

void actor_action_a_or_12_child(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        if (obj->paramA8 == 0)
        {
            sprite_set_action(obj, 0xA, 0, 0);
            sprite_set_action(obj->child, 0xA, 0, 0);
        }
        else
        {
            sprite_set_action(obj, 0x12, 0, 0);
            sprite_set_action(obj->child, 0x12, 0, 0);
        }
        obj->timer = 8;
        obj->state = actor_finish_child_effect_b;
    }
}

void actor_action8_or_10_timer(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        if (obj->paramA8 == 0)
        {
            sprite_set_action(obj, 8, 0, 0);
        }
        else
        {
            sprite_set_action(obj, 0x10, 0, 0);
        }
        obj->timer = 8;
        obj->state = actor_finish_with_child_effect;
    }
}

u32 actor_wait_for_slot(struct ActorObj *obj, u32 slot)
{
    u32 result = lookup_slot(obj, slot);
    if (result == 0)
    {
        obj->state = actor_plan_arc_path;
        return 0;
    }
    return result;
}

/* gWorld->partners[0..5]: the party slots before the two heroes (6 and 7); actor body 8 bytes in */
#define PARTY_SLOT_COUNT 6
#define HERO_SLOT_A 6
#define HERO_SLOT_B 7
#define PARTNER_BODY_OFFSET 8
/* mode bits (bits 1-2) at this offset of a partners[] entry; partner_link_update only hits
 * PARTNER_MODE_1 and PARTNER_MODE_SHAKEN */
#define PARTNER_MODE_OFFSET 0x7E
#define PARTNER_MODE_MASK 6
#define PARTNER_MODE_1 (1 << 1)
#define PARTNER_MODE_SHAKEN (2 << 1) /* knocked by the landing in actor_arc_move_step */

/* Impact of the actor's attack: spawns effect 0x1C19 at its owner's part, plays sound 0x40, hits
 * each of the two heroes that is in PARTNER_MODE_1 (partner_link_update), then removes itself. */
u32 actor_unlink_partners_and_release(struct ActorObj *obj)
{
    struct ActorPart *part;
    u8 *entryA;
    u8 *heroA;
    u8 *entryB;
    u8 *heroB;

    part = obj->owner->part;
    effect_spawn_at(0x1C19, part->targetX / 0x100, part->targetY / 0x100, part->targetH / 0x100, obj);
    set_slot_value(0x40, -1);
    entryA = gWorld->partners[HERO_SLOT_A];
    heroA = entryA + PARTNER_BODY_OFFSET;
    entryB = gWorld->partners[HERO_SLOT_B];
    heroB = entryB + PARTNER_BODY_OFFSET;
    if ((entryA[PARTNER_MODE_OFFSET] & PARTNER_MODE_MASK) == PARTNER_MODE_1)
    {
        partner_link_update(obj, (u32)heroA, 0xFF);
    }
    if ((entryB[PARTNER_MODE_OFFSET] & PARTNER_MODE_MASK) == PARTNER_MODE_1)
    {
        partner_link_update(obj, (u32)heroB, 0xFF);
    }
    obj->state = 0;
    obj_release(obj);
    return 0;
}

/*
 * Counts targetX down (actor_arc_move_step sets it to 30 on landing); at zero, every party
 * member the landing knocked returns to its idle action (or just stops), and the actor goes away.
 */
void actor_countdown_release_partners(struct ActorObj *obj)
{
    u8 *entry;
    struct ActorObj *partner;
    s32 i;
    s32 odd;
    s32 countdown;

    countdown = obj->targetX - 1;
    obj->targetX = countdown;
    if (countdown <= 0)
    {
        for (i = 0; i < PARTY_SLOT_COUNT; i += 2)
        {
            entry = gWorld->partners[i];
            partner = (struct ActorObj *)(entry + PARTNER_BODY_OFFSET);
            if ((entry[PARTNER_MODE_OFFSET] & PARTNER_MODE_MASK) == PARTNER_MODE_SHAKEN)
            {
                if (partner->part->unkF6 != 0)
                {
                    sprite_set_action(partner, 0, 0, 0);
                }
                else
                {
                    obj_stop(partner);
                }
            }
            odd = i + 1;
            entry = gWorld->partners[odd];
            partner = (struct ActorObj *)(entry + PARTNER_BODY_OFFSET);
            if ((entry[PARTNER_MODE_OFFSET] & PARTNER_MODE_MASK) == PARTNER_MODE_SHAKEN)
            {
                if (partner->part->unkF6 != 0)
                {
                    sprite_set_action(partner, 0, 0, 0);
                }
                else
                {
                    obj_stop(partner);
                }
            }
        }
        obj->state = 0;
        obj_release(obj);
    }
}

void actor_release_when_free_b(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        obj_release(obj);
    }
}

void actor_wait_index_then_timer(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        obj->timer = 8;
        obj->state = actor_retarget_to_owner_part;
    }
}

void actor_idle_when_pose_active(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0, 0);
        obj->state = 0;
    }
}

u32 actor_set_home_side1(struct ActorObj *obj)
{
    obj->paramA8 = 1;
    obj->state = actor_set_home_and_motion;
    return 1;
}

u32 actor_set_home_side0(struct ActorObj *obj)
{
    obj->paramA8 = 0;
    obj->state = actor_set_home_and_motion;
    return 1;
}

u32 actor_start_action2_then_action3(struct ActorObj *obj)
{
    obj->state = actor_action2_then_action3;
    return 1;
}

void actor_set_home_and_motion(struct ActorObj *obj)
{
    sprite_set_action(obj, 5, 0, 0);
    obj->targetX = obj->baseX - 0x4800;
    obj->targetY = obj->baseY;
    obj->targetH = obj->baseH;
    ((struct Flags79Bits *)&obj->flags79)->bit5 = 1;
    obj->motionSpeed = 0x180;
    obj->motionStart = 0;
    start_motion(obj);
    obj->timer = 0xA;
    set_slot_value(0x6C, -1);
    obj->state = actor_effect_then_wait_index;
}

void actor_action2_then_action3(struct ActorObj *obj)
{
    sprite_set_action(obj, 2, 0, 0);
    obj->state = actor_action3_spawn_effect;
}

void actor_action3_spawn_effect(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 3, 0, 0);
        set_slot_value(0x10C, -1);
        effect_spawn_at(0x1BFE, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        obj->state = actor_spawn_motion_child;
    }
}
