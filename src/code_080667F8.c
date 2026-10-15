#include "global.h"

struct ActorPose
{
    u8 filler0[0x0C];
    u16 unkC;
    u8 filler0E[0x11 - 0x0E];
    u8 unk11;
    u8 flags;
    u8 filler13[0x20 - 0x13];
    u8 cmd20;
};

struct ActorPart
{
    u8 filler0[0xB4];
    u8 flagB4;
    u8 fillerB5[0xBA - 0xB5];
    s16 valueBA;
    u8 fillerBC[0xD8 - 0xBC];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler1[0xEC - 0xE4];
    s16 slot;
    u8 filler2[0xF6 - 0xEE];
    s16 unkF6;
    u8 filler3[0x11E - 0xF8];
    u16 unk11E;
};

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
    s32 (*handler58)(struct ActorObj *, u32);
    s32 (*handler5C)(struct ActorObj *, s32, s32);
    s32 (*handler60)(struct ActorObj *);
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

s32 is_state_busy(void);
s32 query_contact(struct ActorObj *obj, s32 a, s32 b);
s32 obj_query_contact(struct ActorObj *obj, s32 a, s32 b);
u32 lookup_slot(struct ActorObj *obj);
u32 lookup_link_slot(struct ActorObj *obj);
void sprite_set_action(struct ActorObj *obj, u32 action, u32 a2, u32 a3);
void set_slot_value(u32 id, s32 value);
void sound_stop_song(u32 song);
void wait_for_index(struct ActorObj *obj);
void obj_release(struct ActorObj *obj);
void obj_list_remove(struct ActorObj *obj);
void obj_list_push_default(struct ActorObj *obj);
struct ActorObj *obj_spawn_linked(struct ActorObj *parent, u32 kind, u32 value);
u32 effect_from_table_obj(struct ActorObj *obj, u32 id);
void effect_spawn_at(u32 type, s32 x, s32 y, s32 h, struct ActorObj *owner);
extern u32 gUnk_03000E3C;
void obj_pose_active_place_sprite_a(struct ActorObj *obj);
void obj_pose_active_finish(struct ActorObj *obj);
void obj_active_place_wait(struct ActorObj *obj);
void obj_active_move_to_owner(struct ActorObj *obj);
void obj_active_move_to_owner_action_d(struct ActorObj *obj);
void obj_active_spawn_child_5(struct ActorObj *obj);
void obj_action2_cmd10(struct ActorObj *obj);
void obj_timer_next_action(struct ActorObj *obj);
void obj_timer_action_a_slot(struct ActorObj *obj);
void obj_timer_remove_action_f(struct ActorObj *obj);
void obj_child_timer_finish(struct ActorObj *obj);
void obj_active_spawn_part_sprite(struct ActorObj *obj);
void obj_spawn_pair_child(struct ActorObj *obj);
void obj_motion_to_owner_800(struct ActorObj *obj);
void obj_idle_check_motion(struct ActorObj *obj);
void obj_no_child_action_c(struct ActorObj *obj);
void obj_wait_ready_action2_c(struct ActorObj *obj);
void glide_obj_wait_ready_action2(struct ActorObj *obj);
void glide_obj_wait_ready_action2_b(struct ActorObj *obj);
void glide_obj_timer_action8(struct ActorObj *obj);
void obj_spawn_effect_child(struct ActorObj *obj);
void obj_set_target_xyh(struct ActorObj *obj, s32 x, s32 y, s32 h);
void obj_counter_action8(struct ActorObj *obj);
void obj_active_timer_action_b(struct ActorObj *obj);
void obj_active_wait_state(struct ActorObj *obj);
void obj_active_motion_14c(struct ActorObj *obj);
void obj_active_motion_14c_b(struct ActorObj *obj);
void obj_index_wait_idle_action0(struct ActorObj *obj);
void obj_init_timer_14(struct ActorObj *obj);
void glide_obj_index_wait_action4(struct ActorObj *obj);
void obj_index_wait_slot_then_state(struct ActorObj *obj);
void obj_spawn_linked_child(struct ActorObj *obj);
void obj_timer_dispatch_10_11(struct ActorObj *obj);
void obj_timer_spawn_child_b(struct ActorObj *obj);
void start_motion(struct ActorObj *obj);
void resolve_contact_action(struct ActorObj *a, struct ActorObj *b, s32 c, s32 d, u32 e);
void obj_active_action_6_or_a(struct ActorObj *obj);
void obj_track_child_timer(struct ActorObj *obj);
void obj_respawn_child_6(struct ActorObj *obj);
void obj_respawn_child_7_move(struct ActorObj *obj);
void obj_wait_slots_action(struct ActorObj *obj);
void obj_index_wait_finish_child(struct ActorObj *obj);
void obj_track_child_then_finish(struct ActorObj *obj);
u32 obj_link_lookup_action1(struct ActorObj *obj);
struct ActorObj *obj_spawn(struct ActorPart *part, u32 type, s32 x, s32 y, s32 h, u32 a, u32 b);
void on_arrive(struct ActorObj *obj);
void obj_stop(struct ActorObj *obj);

ASM_FUNC("asm/code_080667F8/obj_spawn_pair_child.s", void obj_spawn_pair_child(struct ActorObj *obj))

void obj_motion_init_600(struct ActorObj *obj)
{
    u8 *flags;

    obj->dx = obj->baseX / 0x100;
    obj->dy = obj->baseY / 0x100;
    obj->dh = obj->baseH / 0x100;
    obj->targetX = obj->owner->part->targetX;
    obj->targetY = obj->owner->part->targetY + 2;
    obj->targetH = obj->owner->part->targetH + 0x1000;
    flags = &obj->flags;
    *flags |= 0x20;
    obj->motionSpeed = 0x600;
    obj->motionStart = 0;
    start_motion(obj);
    obj_set_target_xyh(obj->child, obj->posX, obj->posY, obj->posH);
    obj->state = obj_track_child_timer;
}

void obj_child_timer_finish(struct ActorObj *obj)
{
    struct ActorObj *node;
    u8 *flags;

    obj_set_target_xyh(obj->child, obj->posX, obj->posY, obj->posH);
    obj->timer--;
    if ((s16)obj->timer <= 0)
    {
        node = (struct ActorObj *)((u8 *)obj->part + 8);
        node->handler68 = on_arrive;
        node->link = obj->link;
        obj->link = 0;
        if (node->part->unkF6 == 0)
        {
            obj_stop(node);
            obj_release(obj->child);
            obj_release(obj);
        }
        else
        {
            obj->targetX = obj->dx << 8;
            obj->targetY = obj->dy << 8;
            obj->targetH = obj->dh << 8;
            flags = &obj->flags;
            *flags |= 0x20;
            obj->motionSpeed = 0x600;
            obj->motionStart = 0;
            start_motion(obj);
            obj->state = obj_track_child_then_finish;
        }
    }
}

ASM_FUNC("asm/code_080667F8/obj_fan_out_children.s", void obj_fan_out_children(struct ActorObj *obj))

s32 obj_query_contact_action1(struct ActorObj *obj, struct ActorObj *owner, s32 b)
{
    s32 result = query_contact(obj, (s32)owner, b);
    if (result != 0)
    {
        return result;
    }
    resolve_contact_action(owner, obj, obj->posX / 0x100, obj->posH / 0x100,
                           owner->part->slot != -1 ? 0x13 : 0x12);
    sprite_set_action(obj, 1, 0, 0);
    obj->timer = 8;
    obj->state = obj_child_timer_finish;
    return 0;
}

void obj_set_target_xyh(struct ActorObj *obj, s32 x, s32 y, s32 h)
{
    obj->targetX = x;
    obj->targetY = y;
    obj->targetH = h;
}

void obj_track_child_then_finish(struct ActorObj *obj)
{
    obj_set_target_xyh(obj->child, obj->posX, obj->posY, obj->posH);
    if (obj->index == 0)
    {
        obj_release(obj->child);
        obj_release(obj);
    }
}

void obj_track_child_timer(struct ActorObj *obj)
{
    obj_set_target_xyh(obj->child, obj->posX, obj->posY, obj->posH);
    if (obj->index == 0)
    {
        obj->timer = 0x1E;
        obj->state = obj_child_timer_finish;
    }
}

void obj_owner_slot_action(struct ActorObj *obj)
{
    struct ActorObj *target;

    if (obj->paramA0 == 1)
    {
        target = obj->child->child;
    }
    else
    {
        target = obj->child;
    }
    if (target == NULL)
    {
        set_slot_value(0x114, -1);
        if (obj->param9C == 0)
        {
            sprite_set_action(obj, 8, 0, 0);
        }
        else
        {
            sprite_set_action(obj, 0xC, 0, 0);
        }
        obj->state = obj_active_motion_14c_b;
    }
}

void obj_wait_slots_action(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        if (obj->paramA0 == 0)
        {
            sound_stop_song(0x129);
        }
        else
        {
            sound_stop_song(0x12A);
        }
        if (obj->param9C == 0)
        {
            sprite_set_action(obj, 5, 0, 0);
        }
        else
        {
            sprite_set_action(obj, 9, 0, 0);
        }
        obj->state = obj_active_action_6_or_a;
        if (obj->paramA0 == 0)
        {
            set_slot_value(0x123, -1);
        }
        else
        {
            set_slot_value(0x124, -1);
        }
    }
}

s32 obj_check_owner_slot_a0_1(struct ActorObj *obj)
{
    obj->paramA0 = 1;
    if (obj->owner->part->slot == -1)
    {
        obj->param9C = 0;
    }
    else
    {
        obj->param9C = 1;
    }
    obj->state = obj_idle_check_motion;
    return 1;
}

s32 obj_check_owner_slot_a0_0(struct ActorObj *obj)
{
    obj->paramA0 = 0;
    if (obj->owner->part->slot == -1)
    {
        obj->param9C = 0;
    }
    else
    {
        obj->param9C = 1;
    }
    obj->state = obj_idle_check_motion;
    return 1;
}

void part_hook_next(struct ActorObj *obj)
{
    obj->handler54 = obj_spawn_effect_child;
}

void obj_active_motion_14c_b(struct ActorObj *obj)
{
    u8 *flags;
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0xD, 0, 0);
        obj->targetX = obj->dx << 8;
        obj->targetY = obj->dy << 8;
        obj->targetH = obj->baseH;
        flags = &obj->flags;
        *flags |= 0x20;
        obj->motionSpeed = 0x14C;
        obj->motionStart = 0;
        start_motion(obj);
        obj->state = obj_index_wait_idle_action0;
    }
}

void obj_active_action_6_or_a(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        if (obj->param9C == 0)
        {
            sprite_set_action(obj, 6, 0, 0);
        }
        else
        {
            sprite_set_action(obj, 0xA, 0, 0);
        }
        obj->state = obj_spawn_pair_child;
    }
}

void obj_idle_check_motion(struct ActorObj *obj)
{
    if (!is_state_busy())
    {
        sprite_set_action(obj, 2, 0, 0);
        obj->dx = obj->baseX / 0x100;
        obj->dy = obj->baseY / 0x100;
        if (obj->paramA0 != 0)
        {
            obj->param9C = (obj->param9C + 1) & 1;
        }
        obj->state = obj_active_motion_14c;
    }
}

void obj_spawn_effect_child(struct ActorObj *obj)
{
    struct ActorObj *child = obj_spawn_linked(obj, 0x4095, 0);
    u8 *fp;
    s32 flags;
    s32 count;
    fp = &child->flags;
    flags = *fp;
    flags |= 1;
    flags |= 8;
    *fp = flags;
    count = obj->byte75;
    child->byte75 = count - 2;
    sprite_set_action(obj, 0, 0, 0);
    obj->state = 0;
}

void obj_index_wait_idle_action0(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        sprite_set_action(obj, 0, 0, 0);
        obj->state = wait_for_index;
    }
}

ASM_FUNC("asm/code_080667F8/obj_spawn_part_start.s", void obj_spawn_part_start(struct ActorObj *obj))
