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

extern struct ActorWorld *gUnk_03000FD8;

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

struct PoseBits6
{
    u8 low : 6;
    u8 bit6 : 1;
    u8 high : 1;
} __attribute__((packed));

struct Flags78Bits
{
    u8 low5 : 5;
    u8 kind : 3;
};

struct ObjFlags77
{
    u8 filler0[0x77];
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

struct Flags79Bit0
{
    u8 bit0 : 1;
    u8 unk1 : 7;
};

// The spawned child takes over flag bits 5 and 6 of its parent.
static inline void copy_flags77_bits(struct ActorObj *obj, struct ActorObj *child)
{
    ((struct ObjFlags77 *)child)->bit6 = ((struct ObjFlags77 *)obj)->bit6;
    ((struct ObjFlags77 *)child)->bit5 = ((struct ObjFlags77 *)obj)->bit5;
}

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

u32 actor_start_action2_then_move_high(struct ActorObj *obj)
{
    obj->state = actor_action2_then_move_high;
    return 1;
}

u32 actor_init_side_mode(struct ActorObj *obj)
{
    if (obj->param9C == 0)
    {
        obj->paramA4 = 3;
    }
    else
    {
        obj->paramA4 = 9;
    }
    if (obj->owner->part->slot == -1)
    {
        obj->paramA8 = 0;
    }
    else
    {
        obj->paramA8 = 1;
    }
    obj->state = actor_action_from_a4_notify;
    return 1;
}

u32 actor_default_speed(struct ActorObj *obj)
{
    if (obj->param9C == 0)
    {
        obj->param9C = 0x180;
    }
    obj->state = actor_action5_then_spawn_child;
    return 1;
}

void actor_action2_then_move_high(struct ActorObj *obj)
{
    sprite_set_action(obj, 2, 0, 0);
    obj->state = obj_pose_active_move_high;
}

void actor_release_when_free(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        obj_release(obj);
    }
}

void actor_action_from_a4_notify(struct ActorObj *obj)
{
    sprite_set_action(obj, obj->paramA4, 0, 0);
    set_slot_value(0x88, -1);
    obj->state = actor_side_action_when_active;
}

void actor_stop_when_pose_active(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        obj->state = 0;
    }
}

void actor_action5_then_spawn_child(struct ActorObj *obj)
{
    sprite_set_action(obj, 5, 0, 0);
    obj->state = obj_spawn_part_child;
    obj->timer = 0;
}

void actor_side_action_when_active(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        if (obj->paramA8 == 0)
        {
            sprite_set_action(obj, obj->paramA4 + 1, 0, 0);
        }
        else
        {
            sprite_set_action(obj, obj->paramA4 + 4, 0, 0);
        }
        obj->state = actor_side_action_timer8_child;
    }
}

ASM_FUNC("asm/code_0806BCCC/actor_side_action_timer8_child.s", void actor_side_action_timer8_child(struct ActorObj *obj))

void actor_spawn_motion_child(struct ActorObj *obj)
{
    struct ActorObj *child;

    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 4, 0, 0);
        set_slot_value(0x86, -1);
        effect_spawn_at(0x1C07, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        child = obj_spawn(obj->part, 0x40D3, obj->baseX / 0x100, obj->baseY / 0x100 + 2,
                          obj->baseH / 0x100 + 0x20, ((struct PoseBits6 *)&obj->pose->unk11)->bit6, 1);
        obj->child = child;
        ((struct Flags78Bits *)&child->flags78)->kind = 6;
        ((struct ObjFlags77 *)child)->bit6 = ((struct ObjFlags77 *)obj)->bit6;
        ((struct ObjFlags77 *)child)->bit5 = ((struct ObjFlags77 *)obj)->bit5;
        child->handler68 = on_arrive;
        child->unk6C = obj->unk6C;
        child->owner = obj->owner;
        obj_list_push_default(child);
        child->handler58 = (void (*)(struct ActorObj *))actor_unlink_partners_and_release;
        child->handler60 = (void (*)(struct ActorObj *))actor_wait_for_slot;
        child->targetX = obj->owner->part->targetX;
        child->targetY = obj->owner->part->targetY;
        child->targetH = obj->owner->part->targetH + 0x8000;
        ((struct Flags79Bits *)&child->flags79)->bit5 = 1;
        child->motionSpeed = 0x400;
        child->motionStart = 0;
        start_motion(child);
        child->tag = obj->tag;
        child->state = actor_wait_index_then_timer;
        obj->state = actor_idle_when_pose_active;
    }
}

ASM_FUNC("asm/code_0806BCCC/actor_retarget_to_owner_part.s", void actor_retarget_to_owner_part(struct ActorObj *obj))

void actor_plan_arc_path(struct ActorObj *obj)
{
    s32 dirX;
    s32 dirY;
    s32 dist;
    s32 steps;
    s32 half;
    s32 speed;

    obj->targetX = obj->part->targetX;
    obj->targetY = obj->part->targetY;
    obj->targetH = obj->part->targetH;
    obj->dx = obj->baseX / 0x100;
    obj->dy = obj->baseY / 0x100;
    obj->dh = obj->baseH / 0x100;
    dirX = obj->targetX - (obj->dx << 8);
    dirY = obj->targetY - (obj->dy << 8);
    dist = MATH_ROUTINE(u32 (*)(u32), iwram_sqrt)(dirX * dirX + dirY * dirY);
    steps = dist / 0x500;
    half = steps / 2;
    speed = 0x3800 / (half * half);
    if (speed == 1)
    {
        speed = 2;
    }
    obj->param9C = -speed;
    obj->paramA0 = steps;
    obj->paramA4 = dirX;
    obj->paramA8 = dirY;
    obj->timer = 0;
    obj->state = actor_arc_move_step;
}

ASM_FUNC("asm/code_0806BCCC/actor_arc_move_step.s", void actor_arc_move_step(struct ActorObj *obj))

void actor_effect_then_wait_index(struct ActorObj *obj)
{
    s32 t = obj->timer;

    t--;
    obj->timer = t;
    if ((t << 16) <= 0)
    {
        obj->timer = 0xA;
        effect_spawn_at(0x1C52, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
    }
    if (obj->index == 0)
    {
        sound_stop_song(0x6C);
        if (obj->paramA8 == 0)
        {
            sprite_set_action(obj, 6, 0, 0);
        }
        else
        {
            sprite_set_action(obj, 0xE, 0, 0);
        }
        obj->timer = 0;
        obj->state = actor_spawn_counted_child;
    }
}

void actor_spawn_counted_child(struct ActorObj *obj)
{
    struct ActorObj *child;

    if (obj->pose->flags & 8)
    {
        if ((s16)--obj->timer <= 0)
        {
            if (obj->paramA8 == 0)
                sprite_set_action(obj, 7, 0, 0);
            else
                sprite_set_action(obj, 0xF, 0, 0);
            child = obj_spawn(obj->part, 0x40D0, obj->baseX / 0x100, obj->baseY / 0x100,
                              obj->baseH / 0x100, ((struct PoseBits6 *)&obj->pose->unk11)->bit6, 1);
            obj->child = child;
            ((struct Flags79Bit0 *)&child->flags79)->bit0 = 1;
            copy_flags77_bits(obj, child);
            child->handler68 = on_arrive;
            child->unk6C = obj->unk6C;
            child->owner = obj->owner;
            child->counter = obj->counter + 1;
            obj->state = actor_action8_or_10_timer;
        }
    }
}

ASM_FUNC("asm/code_0806BCCC/actor_finish_with_child_effect.s", void actor_finish_with_child_effect(struct ActorObj *obj))
