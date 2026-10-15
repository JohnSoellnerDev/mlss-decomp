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
    u8 filler2[0x11E - 0xEE];
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

void obj_no_sprite_idle_b(struct ActorObj *obj)
{
    if (obj->child == NULL)
    {
        sprite_set_action(obj, 0, 0, 0);
        obj->state = wait_for_index;
    }
}

s32 unknown_part_steps_08065b34(struct ActorObj *obj)
{
    obj->state = obj_wait_ready_action2_c;
    return 1;
}

s32 sub_08065B40(struct ActorObj *obj)
{
    obj->state = glide_obj_wait_ready_action2;
    return 1;
}

s32 obj_query_part_action3(struct ActorObj *obj, s32 a, s32 b)
{
    s32 result = query_contact(obj, a, b);
    if (result == 0)
    {
        sprite_set_action(obj, 3, 0, 0);
        obj->state = obj_pose_active_finish;
    }
    return result;
}

void obj_pose_active_effect_c(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 3, 0, 0);
        gUnk_03000E3C = effect_from_table_obj(obj, 0x151D);
        obj->handler5C = obj_query_contact;
        obj->state = obj_pose_active_finish;
    }
}

void obj_place_sprite_finish_b(struct ActorObj *obj)
{
    set_slot_value(0xAD, -1);
    effect_spawn_at(0xEFF, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
    obj_release(obj);
}

void obj_wait_ready_action2_c(struct ActorObj *obj)
{
    if (!is_state_busy())
    {
        sprite_set_action(obj, 2, 0, 0);
        obj->state = obj_active_place_wait;
    }
}

void glide_obj_wait_ready_action2(struct ActorObj *obj)
{
    if (!is_state_busy())
    {
        sprite_set_action(obj, 2, 0, 0);
        obj->state = obj_pose_active_place_sprite_a;
    }
}

s32 obj_query_contact(struct ActorObj *obj, s32 a, s32 b)
{
    return query_contact(obj, a, b);
}

void obj_active_place_wait(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 3, 0, 0);
        effect_spawn_at(0x230A, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        obj->timer = 0x4A;
        set_slot_value(0x119, -1);
        obj->state = obj_timer_spawn_child_b;
    }
}

ASM_FUNC("asm/code_08065B10/obj_active_move_to_owner.s", void obj_active_move_to_owner(struct ActorObj *obj))

ASM_FUNC("asm/code_08065B10/obj_active_spawn_child_5.s", void obj_active_spawn_child_5(struct ActorObj *obj))

ASM_FUNC("asm/code_08065B10/obj_respawn_child_6.s", void obj_respawn_child_6(struct ActorObj *obj))

ASM_FUNC("asm/code_08065B10/obj_respawn_child_7_move.s", void obj_respawn_child_7_move(struct ActorObj *obj))

ASM_FUNC("asm/code_08065B10/obj_spawn_linked_child.s", void obj_spawn_linked_child(struct ActorObj *obj))

ASM_FUNC("asm/code_08065B10/obj_timer_next_action.s", void obj_timer_next_action(struct ActorObj *obj))

ASM_FUNC("asm/code_08065B10/obj_lift_step_finish.s", void obj_lift_step_finish(struct ActorObj *obj))

ASM_FUNC("asm/code_08065B10/obj_active_move_to_owner_action_d.s", void obj_active_move_to_owner_action_d(struct ActorObj *obj))

void obj_index_wait_slot_then_state(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        sprite_set_action(obj, 0xE, 0, 0);
        obj->timer = obj->paramA8;
        obj->state = obj_timer_action_a_slot;
        sound_stop_song(0x81);
        set_slot_value(0xAF, -1);
    }
}

u32 obj_lookup_slot_or_finish(struct ActorObj *obj)
{
    u32 result = lookup_slot(obj);
    if (result != 0)
    {
        return result;
    }
    effect_spawn_at(0x113D, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
    obj_release(obj);
    return 0;
}

u32 obj_link_lookup_action1(struct ActorObj *obj)
{
    u32 result = lookup_link_slot(obj);
    if (result != 0)
    {
        return result;
    }
    if (obj->child != NULL)
    {
        obj_release(obj->child);
    }
    sprite_set_action(obj, 1, 0, 0);
    obj->timer = 8;
    obj->state = obj_timer_next_action;
    return 0;
}

void obj_index_wait_finish_child(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        obj_release(obj->child);
        obj->timer = 3;
        obj->state = glide_obj_timer_action8;
    }
}

void glide_obj_index_wait_action4(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        sprite_set_action(obj, 4, 0, 0);
        obj->state = obj_active_spawn_child_5;
        sound_stop_song(0x81);
    }
}

s32 obj_init_side_counter(struct ActorObj *obj)
{
    if (obj->param9C == 0)
    {
        obj->param9C = 2;
    }
    obj->paramA8 = obj->param9C;
    obj->state = glide_obj_wait_ready_action2_b;
    return 1;
}

s32 obj_init_motion_params(struct ActorObj *obj)
{
    if (obj->param9C == 0)
    {
        obj->param9C = 3;
        obj->paramA0 = 0x20;
    }
    obj->paramA4 = obj->paramA0;
    obj->paramA8 = obj->param9C;
    obj->state = obj_action2_cmd10;
    return 1;
}

void obj_timer_action_a_slot(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        obj->timer--;
        if ((s16)obj->timer > 0)
        {
            set_slot_value(0xAF, -1);
        }
        else
        {
            sprite_set_action(obj, 0xA, 0, 0);
            obj->timer = obj->paramA8 - 1;
            obj->state = obj_active_timer_action_b;
            set_slot_value(0xD5, -1);
        }
    }
}

void glide_obj_wait_ready_action2_b(struct ActorObj *obj)
{
    if (!is_state_busy())
    {
        sprite_set_action(obj, 2, 0, 0);
        obj->state = obj_active_move_to_owner_action_d;
    }
}

void glide_obj_timer_action8(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        obj->timer--;
        if ((s16)obj->timer <= 0)
        {
            sprite_set_action(obj, 8, 0, 0);
            obj->timer = obj->paramA8;
            obj->state = obj_active_spawn_part_sprite;
        }
    }
}

void obj_action2_cmd10(struct ActorObj *obj)
{
    sprite_set_action(obj, 2, 0, 0);
    obj->pose->cmd20 = 0x10;
    obj->state = obj_active_move_to_owner;
}

void obj_active_timer_action_b(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        obj->timer--;
        if ((s16)obj->timer > 0)
        {
            set_slot_value(0xD5, -1);
        }
        else
        {
            sprite_set_action(obj, 0xB, 0, 0);
            obj_list_push_default(obj);
            obj->state = obj_motion_to_owner_800;
        }
    }
}

void obj_active_spawn_part_sprite(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 9, 0, 0);
        effect_spawn_at(0x112B, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        obj_spawn_linked_child(obj);
        set_slot_value(0x9C, -1);
        obj->state = obj_timer_dispatch_10_11;
    }
}

ASM_FUNC("asm/code_08065B10/obj_motion_to_owner_800.s", void obj_motion_to_owner_800(struct ActorObj *obj))

void obj_timer_dispatch_10_11(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        obj->timer--;
        if ((s16)obj->timer > 0)
        {
            sprite_set_action(obj, 0x10, 0, 0);
            obj->param9C = 4;
            obj->state = obj_counter_action8;
        }
        else
        {
            sprite_set_action(obj, 0x11, 0, 0);
            obj->state = obj_active_wait_state;
        }
    }
}

void obj_init_timer_14(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        obj->timer = 0x14;
        obj->state = obj_timer_remove_action_f;
    }
}

void obj_active_wait_state(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        obj->state = obj_no_child_action_c;
    }
}

void obj_counter_action8(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        obj->param9C--;
        if (obj->param9C <= 0)
        {
            sprite_set_action(obj, 8, 0, 0);
            obj->state = obj_active_spawn_part_sprite;
        }
    }
}

void obj_timer_remove_action_f(struct ActorObj *obj)
{
    obj->timer--;
    if ((s16)obj->timer <= 0)
    {
        obj_list_remove(obj);
        sprite_set_action(obj, 0xF, 0, 0);
        obj->state = wait_for_index;
    }
}

void obj_no_child_action_c(struct ActorObj *obj)
{
    u8 count = 0;
    struct ActorObj *walk = obj->child;
    if (walk != NULL)
    {
        do
        {
            count++;
            walk = walk->child;
        } while (walk != NULL);
    }
    if (count == 0)
    {
        sprite_set_action(obj, 0xC, 0, 0);
        obj->state = wait_for_index;
    }
}

ASM_FUNC("asm/code_08065B10/obj_active_motion_14c.s", void obj_active_motion_14c(struct ActorObj *obj))

ASM_FUNC("asm/code_08065B10/obj_spawn_pair_child.s", void obj_spawn_pair_child(struct ActorObj *obj))

ASM_FUNC("asm/code_08065B10/obj_motion_init_600.s", void obj_motion_init_600(struct ActorObj *obj))

ASM_FUNC("asm/code_08065B10/obj_child_timer_finish.s", void obj_child_timer_finish(struct ActorObj *obj))

ASM_FUNC("asm/code_08065B10/obj_fan_out_children.s", void obj_fan_out_children(struct ActorObj *obj))

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

ASM_FUNC("asm/code_08065B10/obj_wait_slots_action.s", void obj_wait_slots_action(struct ActorObj *obj))

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

ASM_FUNC("asm/code_08065B10/obj_active_motion_14c_b.s", void obj_active_motion_14c_b(struct ActorObj *obj))

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

ASM_FUNC("asm/code_08065B10/obj_spawn_effect_child.s", void obj_spawn_effect_child(struct ActorObj *obj))

void obj_index_wait_idle_action0(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        sprite_set_action(obj, 0, 0, 0);
        obj->state = wait_for_index;
    }
}

ASM_FUNC("asm/code_08065B10/obj_spawn_part_start.s", void obj_spawn_part_start(struct ActorObj *obj))
