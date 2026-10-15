#include "global.h"
#include "ram_routines.h"

struct ActorPose
{
    u8 filler0[0x0C];
    u16 phase;
    u8 filler0E[0x11 - 0x0E];
    u8 unk11;
    u8 flags;
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
    s16 cursorX;
    s16 cursorY;
    u8 filler3[0x104 - 0xFA];
    s16 unk104;
    u8 filler4[0x10A - 0x106];
    u16 unk10A;
    u16 unk10C;
};

struct PoseBits
{
    u8 low : 6;
    u8 bit6 : 1;
    u8 top : 1;
};

struct ActorWorld
{
    u8 filler0[0x58];
    u8 *partners[8];
};

extern struct ActorWorld *gUnk_03000FD8;
extern u32 gUnk_03000E3C;
extern u8 gUnk_03000E7C;
extern u8 gUnk_03000E7D;
extern const u8 gStepSequenceTable[];

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
    u8 filler50[0x54 - 0x50];
    void (*handler54)(struct ActorObj *);
    void (*handler58)(struct ActorObj *);
    void (*handler5C)(struct ActorObj *);
    void (*handler60)(struct ActorObj *);
    u8 filler64[0x68 - 0x64];
    void (*handler68)(struct ActorObj *);
    s32 link;
    u8 filler70[0x74 - 0x70];
    u8 byte74;
    u8 byte75;
    u8 filler76;
    u8 mode;
    u8 dir;
    u8 flags;
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

void obj_release(struct ActorObj *obj);
void sprite_set_action(struct ActorObj *obj, u32 action, u32 a2, u32 a3);
void wait_for_index(struct ActorObj *obj);
void sound_stop_song(u32 song);
void set_slot_value(u32 id, s32 value);
void obj_list_push_default(struct ActorObj *obj);
u32 lookup_slot(struct ActorObj *obj, u32 slot);
void obj_arc_start(struct ActorObj *obj);
void obj_timer_drift_left_exit(struct ActorObj *obj);
void start_motion(struct ActorObj *obj);
void obj_anim5_then_spawn(struct ActorObj *obj);
void actor_side_walk_idle(struct ActorObj *obj);
void obj_place_at_base_when_index_zero(struct ActorObj *obj);
void obj_effect_sync_bytes(struct ActorObj *obj);
void obj_timer_follow_owner(struct ActorObj *obj);
void obj_start_move_right(struct ActorObj *obj);
void obj_copy_bytes_sprite_88(struct ActorObj *obj);
void obj_init_pair_motion(struct ActorObj *obj);
void obj_anim6_wait4(struct ActorObj *obj);
void obj_move_to_part_x_offset(struct ActorObj *obj);
void obj_spawn_child_4115(struct ActorObj *obj);
void obj_spawn_child_4113(struct ActorObj *obj);
void obj_place_sprite_anim9(struct ActorObj *obj);
void obj_link_owner_go(struct ActorObj *obj);
void obj_lookup_slot_start(struct ActorObj *obj);
void obj_setup_wait_target(struct ActorObj *obj);
void obj_lookup_part_setup(struct ActorObj *obj);
void obj_fall_to_ground(struct ActorObj *obj);
void obj_wave_left(struct ActorObj *obj);
void obj_timer_to_move(struct ActorObj *obj);
void obj_wave_left_to_target(struct ActorObj *obj);
void obj_wave_rise_stop(struct ActorObj *obj);
void obj_wave_fall_stop(struct ActorObj *obj);
void obj_move_by_path(void);
void obj_count_down_exit(struct ActorObj *obj);
void obj_timer_table_next(struct ActorObj *obj);
void obj_link_then_finish(struct ActorObj *obj);
struct ActorObj *obj_spawn(struct ActorPart *part, u32 type, s32 x, s32 y, s32 h, u32 a, u32 b);
void obj_index_zero_count(struct ActorObj *obj);
void obj_notify_and_show(struct ActorObj *obj);
void obj_lookup_part_move(struct ActorObj *obj);
void obj_setup_part_motion(struct ActorObj *obj);
u32 effect_from_table_obj(struct ActorObj *obj, u32 id);
u32 mt19937_next(void);
void obj_list_remove(struct ActorObj *obj);
u32 partner_link_update(struct ActorObj *obj, struct ActorObj *partner, u32 mode);
void obj_anim_a_wait60(struct ActorObj *obj);
void obj_countdown_then_anim_b(struct ActorObj *obj);
void obj_wait_then_spawn_child(struct ActorObj *obj);
void side_walk_obj_timer_spawn_effect(struct ActorObj *obj);
void obj_pose_release_partner(struct ActorObj *obj);
struct ActorObj *obj_spawn_linked(struct ActorObj *parent, u32 kind, u32 value);
void effect_spawn_at(u32 type, s32 x, s32 y, s32 h, struct ActorObj *owner);

void obj_finish_if_active(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        obj_release(obj);
    }
}

void obj_idle_when_active(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        obj->owner->state = 0;
        sprite_set_action(obj, 7, 0, 0);
        obj->state = wait_for_index;
    }
}

void obj_prepare_when_index_clear(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        sound_stop_song(0x99);
        sprite_set_action(obj, 4, 0, 0);
        obj_list_push_default(obj);
        set_slot_value(0x118, -1);
        obj->state = obj_anim5_then_spawn;
    }
}

u32 obj_lookup_slot_step(struct ActorObj *obj, u32 slot)
{
    u32 result = lookup_slot(obj, slot);

    if (result == 0)
    {
        obj->state = obj_arc_start;
        sound_stop_song(0x6C);
        set_slot_value(0x40, -1);
        return 0;
    }
    return result;
}

u32 obj_owner_callback_step(struct ActorObj *obj)
{
    void (*callback)(struct ActorObj *) = obj->owner->handler68;

    if (callback != 0)
    {
        callback(obj->owner);
    }
    obj->owner->state = actor_side_walk_idle;
    obj->state = obj_timer_drift_left_exit;
    return 0;
}

void obj_move_to_base_right(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0xC, 0, 0);
        sprite_set_action(obj->child, 0xC, 0, 0);
        obj->targetX = obj->baseX + 0x8000;
        obj->targetY = obj->baseY;
        obj->targetH = obj->baseH;
        obj->motionSpeed = 0x200;
        *(u8 *)&obj->flags |= 0x20;
        obj->motionStart = 0;
        start_motion(obj);
        set_slot_value(0x6C, -1);
        obj->state = obj_place_at_base_when_index_zero;
    }
}

u32 actor_side_walk_steps(struct ActorObj *obj)
{
    obj->paramA8 = 1;
    obj->index = 0;
    obj->state = obj_effect_sync_bytes;
    return 0;
}


void obj_sync_child_reset(struct ActorObj *obj)
{
    struct ActorObj *child = obj->child;

    child->posX = obj->posX;
    child->posY = obj->posY;
    child->posH = obj->posH;
    if (obj->index == 0)
    {
        obj->state = 0;
        child->state = 0;
    }
}


void obj_reset_when_index_zero(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        obj->state = 0;
        obj->child->state = 0;
    }
}


void actor_side_walk_idle(struct ActorObj *obj)
{
}


void obj_anim5_when_index_zero(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        sound_stop_song(0x6C);
        sprite_set_action(obj, 5, 0, 0);
        sprite_set_action(obj->child, 5, 0, 0);
        obj->state = obj_anim6_wait4;
    }
}


void obj_wait_30_when_index_zero(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        obj->timer = 0x1E;
        obj->state = obj_timer_follow_owner;
    }
}


u32 obj_step_move_to_part_x(struct ActorObj *obj)
{
    obj->state = obj_move_to_part_x_offset;
    return 1;
}


u32 obj_step_start_move_right(struct ActorObj *obj)
{
    obj->state = obj_start_move_right;
    return 1;
}


u32 obj_store_owner_slot_flag(struct ActorObj *obj)
{
    s32 slot = obj->owner->part->slot;

    if (slot == -1)
    {
        obj->paramA0 = 0;
    }
    else
    {
        obj->paramA0 = 1;
    }
    obj->state = obj_copy_bytes_sprite_88;
    return 1;
}


u32 obj_store_owner_slot_flag_alt(struct ActorObj *obj)
{
    s32 slot = obj->owner->part->slot;

    if (slot == -1)
    {
        obj->paramA0 = 0;
    }
    else
    {
        obj->paramA0 = 1;
    }
    obj->state = obj_init_pair_motion;
    return 1;
}


void part_hook_next_pair(struct ActorObj *obj)
{
    obj->handler54 = obj_spawn_child_4115;
}


void part_hook_spawn_4113(struct ActorObj *obj)
{
    obj->handler54 = obj_spawn_child_4113;
}


void obj_move_to_part_x_offset(struct ActorObj *obj)
{
    sprite_set_action(obj, 8, 0, 0);
    obj->targetX = obj->owner->part->targetX + 0x3800;
    obj->targetY = obj->owner->part->targetY;
    obj->targetH = obj->baseH;
    obj->motionSpeed = 0x180;
    *(u8 *)&obj->flags |= 0x20;
    obj->motionStart = 0;
    start_motion(obj);
    obj->state = obj_place_sprite_anim9;
}


void obj_anim5_then_spawn(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 5, 0, 0);
        obj->timer = 0xA;
        obj->state = obj_wait_then_spawn_child;
    }
}


void obj_place_at_base_when_index_zero(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        sprite_set_action(obj, 0xD, 0, 0);
        sprite_set_action(obj->child, 0xD, 0, 0);
        sound_stop_song(0x6C);
        set_slot_value(0x8C, -1);
        effect_spawn_at(0x1FA6, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        obj->state = obj_pose_release_partner;
    }
}


void obj_anim6_wait4(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 6, 0, 0);
        sprite_set_action(obj->child, 6, 0, 0);
        obj->timer = 4;
        obj->state = side_walk_obj_timer_spawn_effect;
    }
}


void obj_spawn_child_4115(struct ActorObj *obj)
{
    struct ActorObj *child;

    child = obj_spawn_linked(obj, 0x4115, 0);
    *(u8 *)&child->flags |= 1;
    *(u8 *)&child->flags |= 8;
    child->byte75 = obj->byte75 + 1;
    sprite_set_action(obj, 0, 0, 0);
    obj->state = 0;
}


void obj_spawn_child_4113(struct ActorObj *obj)
{
    struct ActorObj *child;

    child = obj_spawn_linked(obj, 0x4113, 0);
    *(u8 *)&child->flags |= 1;
    *(u8 *)&child->flags |= 8;
    child->byte75 = obj->byte75 + 1;
    sprite_set_action(obj, 0, 0, 0);
    obj->state = 0;
}


void obj_place_sprite_anim9(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        sprite_set_action(obj, 9, 0, 0);
        effect_spawn_at(0x1E8A, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        obj->state = obj_anim_a_wait60;
    }
}


void obj_anim_a_wait60(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0xA, 0, 0);
        obj->timer = 0x3C;
        obj->state = obj_countdown_then_anim_b;
    }
}


void obj_countdown_then_anim_b(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        obj->timer--;
        if ((s16)obj->timer <= 0)
        {
            sprite_set_action(obj, 0xB, 0, 0);
            obj->state = wait_for_index;
        }
    }
}
