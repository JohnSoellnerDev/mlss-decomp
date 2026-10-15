#include "global.h"

struct ActorPose
{
    u8 filler0[0x11];
    u8 byte11;
    u8 flags;
    u8 filler13[0x21 - 0x13];
    u8 busy;
};

struct ActorPoseFlagBits
{
    u8 bit0 : 1;
    u8 mode : 2;
    u8 high : 5;
};

struct ActorFlagBits
{
    u8 low5 : 5;
    u8 bit5 : 1;
    u8 high : 2;
};

struct ActorPart
{
    u8 filler0[0xD8];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 fillerE4[0xEC - 0xE4];
    s16 targetIndex;
    u8 fillerEE[0xF6 - 0xEE];
    s16 unkF6;
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
    struct ActorPart *data;
    struct ActorObj *owner;
    struct ActorObj *child;
    u8 filler34[0x38 - 0x34];
    s32 baseX;
    s32 baseY;
    s32 baseH;
    u8 filler44[0x4C - 0x44];
    void (*state)(struct ActorObj *);
    void (*handler50)(struct ActorObj *);
    s32 savedState;
    void (*handler58)(struct ActorObj *);
    void (*handler5C)(struct ActorObj *);
    void (*handler60)(struct ActorObj *);
    void (*handler64)(struct ActorObj *);
    void (*handler68)(struct ActorObj *);
    s32 childValue;
    u8 filler70[0x74 - 0x70];
    u8 byte74;
    u8 filler75[0x77 - 0x75];
    u8 mode;
    u8 dir;
    u8 flags;
    s16 motionStart;
    s16 motionSpeed;
    u16 progress;
    s32 index;
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler90[0x94 - 0x90];
    s32 dhStep;
    s32 ground;
    s32 param9C;
    s32 paramA0;
    s32 paramA4;
    s32 unkA8;
    u16 timer;
    s16 dx;
    s16 dy;
    s16 dh;
};

u32 is_state_busy(void);
void on_arrive(struct ActorObj *obj);
void motion_timed(struct ActorObj *obj, u32 type, u32 value);
u32 lookup_part(struct ActorObj *obj, u32 a, u32 b);
u32 lookup_link_slot(struct ActorObj *obj, u32 slot);
void set_slot_value(u32 id, s32 value);
u32 effect_from_table_obj(struct ActorObj *obj, u32 id);
void obj_release(struct ActorObj *obj);
void sound_stop_song(u32 song);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 h, struct ActorObj *owner);
void offset_three_words_camera(s32 *a, s32 *b, s32 *c, u32 d);
void start_motion(struct ActorObj *obj);
void obj_detach_active(struct ActorPose *pose);
void obj_move_clamp_b(struct ActorObj *obj);
void obj_link_partner_span_start(struct ActorObj *obj);
void obj_anim3_timer24_then_next(struct ActorObj *obj);
u32 jump_velocity(s16 *dx, s16 *dy, s16 *dh, s32 dX, s32 dY, s32 dH, s32 a, s32 b, s32 c, s32 d);
u32 lookup_slot(struct ActorObj *obj, u32 slot);
void obj_spawn_child_a(struct ActorObj *obj);
void owner_moves_obj_spawn_child_b(struct ActorObj *obj);
void sprite_set_action(struct ActorObj *obj, u32 action, u32 a2, u32 a3);
void obj_list_push_default(struct ActorObj *obj);
void obj_list_remove(struct ActorObj *obj);
s32 find_slot_by_kind(u32 kind);
u32 mt19937_next(void);
u32 sub_081DE850(u32 n, u32 r);
void obj_retarget_c(struct ActorObj *obj);
void obj_place_finish(struct ActorObj *obj);
void obj_pose_update_b(struct ActorObj *obj);
void owner_moves_obj_prepare_b(struct ActorObj *obj);
void obj_pose_update_if_active(struct ActorObj *obj);
u32 obj_start_move_if_zero(struct ActorObj *obj, u32 slot);
u32 obj_choose_state_by_query(struct ActorObj *obj);
void obj_wait_ready_then_anim(struct ActorObj *obj);
void obj_create_part_then_action3(struct ActorObj *obj);
void obj_wait_ready_then_anim_b(struct ActorObj *obj);
void anim_steps_obj_move_step(struct ActorObj *obj);
void obj_anim_2_6_step_a(struct ActorObj *obj);
void obj_anim_2_6_step_b(struct ActorObj *obj);
void obj_anim_3_7_wait_a(struct ActorObj *obj);
void obj_anim_3_7_wait_b(struct ActorObj *obj);
void obj_anim_4_8_wait_a(struct ActorObj *obj);
void obj_anim_4_8_wait_b(struct ActorObj *obj);
void obj_load_target_motion(struct ActorObj *obj);
void obj_wait_then_effect(struct ActorObj *obj);
u32 obj_lookup_part_then_check(struct ActorObj *obj, u32 a, u32 b);
void obj_wait_then_state_78c(struct ActorObj *obj);
void obj_action2_notify_then_state(struct ActorObj *obj);
void obj_clear_timer_action2_notify(struct ActorObj *obj);
void obj_state_if_part_active(struct ActorObj *obj);
void obj_action_a_or_b_then_state(struct ActorObj *obj);
void obj_pose_reset_target_motion(struct ActorObj *obj);
void obj_pose_reset_target_motion_b(struct ActorObj *obj);
void obj_wait_then_notify_action2(struct ActorObj *obj);
void obj_state_when_pose_byte_zero(struct ActorObj *obj);
void obj_wait_index_then_anim3(struct ActorObj *obj);
void obj_wait_index_then_anim3_b(struct ActorObj *obj);
void obj_pose_reset_part_motion(struct ActorObj *obj);
void obj_to_load_target_if_active(struct ActorObj *obj);
void obj_to_state_aaa0_if_active(struct ActorObj *obj);
void obj_wait_index_notify_anim3(struct ActorObj *obj);
void obj_wait_prepare_action5_then_state(struct ActorObj *obj);
void obj_anim5_if_active_then_state(struct ActorObj *obj);
u32 obj_lookup_part_b(struct ActorObj *obj, u32 slot);
void obj_to_state_aba4_if_active(struct ActorObj *obj);
void obj_anim6_pose_update_if_active(struct ActorObj *obj);
void obj_wait_action6_then_state(struct ActorObj *obj);
void obj_wait_action7_then_state(struct ActorObj *obj);
void obj_to_state_ac38_if_active(struct ActorObj *obj);
void obj_wait_action8_notify_effect(struct ActorObj *obj);
void obj_clear_timer_to_acbc_if_active(struct ActorObj *obj);
void obj_wait_prep_a_action6_then_state(struct ActorObj *obj);
void obj_spawn_child_part(struct ActorObj *obj);
void anim_steps_obj_fall_step(struct ActorObj *obj);
void obj_damp_and_spawn_child(struct ActorObj *obj);
void obj_damp_step_limit(struct ActorObj *obj);
void obj_fall_step_b(struct ActorObj *obj);
void obj_pose_update_if_active_b(struct ActorObj *obj);
void obj_pose_update_if_active_c(struct ActorObj *obj);
u32 obj_start_move_if_zero_b(struct ActorObj *obj, u32 slot);
void obj_start_timed_motion_4(struct ActorObj *obj);
void obj_start_timed_motion_2(struct ActorObj *obj);
void anim_steps_obj_move_step_b(struct ActorObj *obj);
void obj_create_part_notify_then_state(struct ActorObj *obj);
void obj_action2_pose2_then_spawn(struct ActorObj *obj);
void obj_wait_target_h800_motion(struct ActorObj *obj);
void obj_index_timer_motion_b(struct ActorObj *obj);
void obj_target_motion_action8_notify(struct ActorObj *obj);
void obj_index_timer_motion_c(struct ActorObj *obj);
void obj_index_prep_a_motion_part(struct ActorObj *obj);
void obj_index_prep_a_motion_part_b(struct ActorObj *obj);
void obj_pose_update_if_index_zero(struct ActorObj *obj);
void obj_wait_ready_notify_8d_then_state(struct ActorObj *obj);
void obj_pose_update_if_index_zero_b(struct ActorObj *obj);
void obj_wait_ready_notify_8d_then_state_b(struct ActorObj *obj);
void obj_target_part_motion_speed5(struct ActorObj *obj);
void obj_target_part_motion_speed2(struct ActorObj *obj);

static inline void pose_set_mode_2(struct ActorObj *obj)
{
    ((struct ActorPoseFlagBits *)&obj->pose->flags)->mode = 1;
}

void obj_anim_2_6_step_a(struct ActorObj *obj)
{
    sprite_set_action(obj, obj->owner->data->targetIndex == -1 ? 2 : 6, 0, 0);
    pose_set_mode_2(obj);
    obj->state = obj_anim_3_7_wait_a;
}

void obj_anim_2_6_step_b(struct ActorObj *obj)
{
    sprite_set_action(obj, obj->owner->data->targetIndex == -1 ? 2 : 6, 0, 0);
    pose_set_mode_2(obj);
    obj->state = obj_anim_3_7_wait_b;
}

void obj_anim_3_7_wait_a(struct ActorObj *obj)
{
    if (obj->pose->flags & 0x08)
    {
        sprite_set_action(obj, obj->owner->data->targetIndex == -1 ? 3 : 7, 0, 0);
        obj->timer = 0x18;
        obj->state = obj_anim_4_8_wait_a;
    }
}

void obj_anim_3_7_wait_b(struct ActorObj *obj)
{
    if (obj->pose->flags & 0x08)
    {
        sprite_set_action(obj, obj->owner->data->targetIndex == -1 ? 3 : 7, 0, 0);
        obj->timer = 0x18;
        obj->state = obj_anim_4_8_wait_b;
    }
}

void obj_anim_4_8_wait_a(struct ActorObj *obj)
{
    s32 timer = obj->timer - 1;

    obj->timer = timer;
    if ((s16)timer < 0)
    {
        sprite_set_action(obj, obj->owner->data->targetIndex == -1 ? 4 : 8, 0, 0);
        pose_set_mode_2(obj);
        obj->state = owner_moves_obj_spawn_child_b;
    }
}

void obj_anim_4_8_wait_b(struct ActorObj *obj)
{
    s32 timer = obj->timer - 1;

    obj->timer = timer;
    if ((s16)timer < 0)
    {
        sprite_set_action(obj, obj->owner->data->targetIndex == -1 ? 4 : 8, 0, 0);
        pose_set_mode_2(obj);
        obj->state = obj_spawn_child_a;
    }
}

void obj_load_target_motion(struct ActorObj *obj)
{
    s32 timer = obj->timer - 1;

    obj->timer = timer;
    if ((s16)timer < 0)
    {
        obj->targetX = obj->owner->data->targetX;
        obj->targetY = obj->owner->data->targetY;
        obj->targetH = obj->owner->data->targetH;
        ((struct ActorFlagBits *)&obj->flags)->bit5 = 1;
        obj->motionSpeed = 0x100;
        obj->motionStart = 0;
        start_motion(obj);
        sprite_set_action(obj, 4, 0, 0);
        obj->timer = 8;
        obj->state = obj_wait_then_effect;
    }
}

void obj_wait_then_effect(struct ActorObj *obj)
{
    s32 index = obj->index;

    if (index == 0)
    {
        s32 timer = obj->timer - 1;

        obj->timer = timer;
        if ((s16)timer < 0)
        {
            obj->unkA8 = index;
            sprite_set_action(obj, 9, 0, 0);
            effect_from_table_obj(obj, 0x138A);
            set_slot_value(0xAF, -1);
            obj->handler5C = (void (*)(struct ActorObj *))obj_lookup_part_then_check;
            obj->savedState = index;
            obj->timer = index;
            obj->state = obj_wait_then_state_78c;
        }
    }
}

u32 obj_lookup_part_then_check(struct ActorObj *obj, u32 a, u32 b)
{
    struct ActorObj *link = (struct ActorObj *)((u8 *)obj->data + 0x08);
    u32 result = lookup_part(link, a, b);

    if (result == 0)
    {
        obj_state_if_part_active(link);
    }
    return result;
}

u32 unknown_code_080da692(struct ActorObj *obj)
{
    obj->state = obj_action2_notify_then_state;
    return 1;
}

u32 sub_080DA6A0(struct ActorObj *obj)
{
    obj->state = obj_clear_timer_action2_notify;
    return 1;
}

u32 sub_080DA6AC(struct ActorObj *obj)
{
    obj->state = obj_clear_timer_action2_notify;
    if (sub_081DE850(2, mt19937_next()) != 0)
    {
        obj->state = obj_action2_notify_then_state;
    }
    return 1;
}

void obj_wait_then_state_78c(struct ActorObj *obj)
{
    s32 timer = obj->timer - 1;

    obj->timer = timer;
    if ((s16)timer < 0)
    {
        obj->state = obj_action_a_or_b_then_state;
    }
}

void obj_action2_notify_then_state(struct ActorObj *obj)
{
    sprite_set_action(obj, 2, 0, 0);
    set_slot_value(0x38, -1);
    pose_set_mode_2(obj);
    obj->state = obj_pose_reset_target_motion;
}

void obj_clear_timer_action2_notify(struct ActorObj *obj)
{
    obj->timer = 0;
    sprite_set_action(obj, 2, 0, 0);
    set_slot_value(0x38, -1);
    pose_set_mode_2(obj);
    obj->state = obj_pose_reset_target_motion_b;
}

void obj_state_if_part_active(struct ActorObj *obj)
{
    if (obj->data->unkF6 != 0)
    {
        obj->state = obj_state_when_pose_byte_zero;
    }
}

void obj_action_a_or_b_then_state(struct ActorObj *obj)
{
    if (obj->pose->flags & 0x08)
    {
        s32 target = obj->unkA8;

        if (target != 0)
        {
            sprite_set_action(obj, 0xA, 0, 0);
        }
        else
        {
            sprite_set_action(obj, 0xB, 0, 0);
            effect_from_table_obj(obj, 0x13CA);
            obj->handler5C = (void (*)(struct ActorObj *))obj_lookup_part_then_check;
            obj->savedState = target;
        }
        obj->timer = 0x18;
        obj->state = obj_wait_then_notify_action2;
    }
}

void obj_pose_reset_target_motion(struct ActorObj *obj)
{
    if (obj->pose->flags & 0x08)
    {
        obj_detach_active(obj->pose);
        obj->targetX = obj->owner->data->targetX + 0x2000;
        obj->targetY = obj->owner->data->targetY;
        obj->targetH = obj->owner->data->targetH;
        ((struct ActorFlagBits *)&obj->flags)->bit5 = 1;
        obj->motionSpeed = 0x200;
        obj->motionStart = 0;
        start_motion(obj);
        obj->state = obj_wait_index_then_anim3;
    }
}

void obj_pose_reset_target_motion_b(struct ActorObj *obj)
{
    if (obj->pose->flags & 0x08)
    {
        obj_detach_active(obj->pose);
        obj->targetX = obj->owner->data->targetX + 0x2000;
        obj->targetY = obj->owner->data->targetY;
        obj->targetH = obj->owner->data->targetH;
        ((struct ActorFlagBits *)&obj->flags)->bit5 = 1;
        obj->motionSpeed = 0x200;
        obj->motionStart = 0;
        start_motion(obj);
        obj->state = obj_wait_index_then_anim3_b;
    }
}

void obj_wait_then_notify_action2(struct ActorObj *obj)
{
    s32 timer = obj->timer - 1;

    obj->timer = timer;
    if ((s16)timer < 0)
    {
        set_slot_value(0x38, -1);
        sprite_set_action(obj, 2, 0, 0);
        pose_set_mode_2(obj);
        obj->state = obj_pose_reset_part_motion;
    }
}

void obj_state_when_pose_byte_zero(struct ActorObj *obj)
{
    u8 value = obj->pose->busy;

    if (value == 0)
    {
        obj->timer = value;
        obj->state = obj_wait_then_notify_action2;
    }
}

void obj_wait_index_then_anim3(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        sprite_set_action(obj, 3, 0, 0);
        pose_set_mode_2(obj);
        obj->timer = 8;
        obj->state = obj_to_load_target_if_active;
    }
}

void obj_wait_index_then_anim3_b(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        sprite_set_action(obj, 3, 0, 0);
        pose_set_mode_2(obj);
        obj->timer = 0x20;
        obj->state = obj_to_state_aaa0_if_active;
    }
}

void obj_pose_reset_part_motion(struct ActorObj *obj)
{
    if (obj->pose->flags & 0x08)
    {
        obj_detach_active(obj->pose);
        obj->targetX = obj->data->targetX;
        obj->targetY = obj->data->targetY;
        obj->targetH = obj->data->targetH;
        ((struct ActorFlagBits *)&obj->flags)->bit5 = 1;
        obj->motionSpeed = 0x400;
        obj->motionStart = 0;
        start_motion(obj);
        obj->state = obj_wait_index_notify_anim3;
    }
}

void obj_to_load_target_if_active(struct ActorObj *obj)
{
    if (obj->pose->flags & 0x08)
    {
        obj->state = obj_load_target_motion;
    }
}

void obj_to_state_aaa0_if_active(struct ActorObj *obj)
{
    if (obj->pose->flags & 0x08)
    {
        obj->state = obj_wait_prepare_action5_then_state;
    }
}

void obj_wait_index_notify_anim3(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        set_slot_value(0xAF, -1);
        sprite_set_action(obj, 3, 0, 0);
        pose_set_mode_2(obj);
        obj->state = obj_anim5_if_active_then_state;
    }
}

void obj_wait_prepare_action5_then_state(struct ActorObj *obj)
{
    s32 timer = obj->timer - 1;

    obj->timer = timer;
    if ((s16)timer < 0)
    {
        obj_list_push_default(obj);
        obj->handler5C = (void (*)(struct ActorObj *))obj_lookup_part_then_check;
        obj->handler60 = (void (*)(struct ActorObj *))obj_lookup_part_b;
        obj->handler64 = (void (*)(struct ActorObj *))obj_lookup_part_b;
        sprite_set_action(obj, 5, 0, 0);
        set_slot_value(0x38, -1);
        pose_set_mode_2(obj);
        obj->state = obj_to_state_aba4_if_active;
    }
}

void obj_anim5_if_active_then_state(struct ActorObj *obj)
{
    if (obj->pose->flags & 0x08)
    {
        sprite_set_action(obj, 5, 0, 0);
        pose_set_mode_2(obj);
        obj->state = obj_anim6_pose_update_if_active;
    }
}

u32 obj_lookup_part_b(struct ActorObj *obj, u32 slot)
{
    struct ActorObj *link = (struct ActorObj *)((u8 *)obj->data + 0x08);
    u32 result = lookup_link_slot(link, slot);

    if (result == 0)
    {
        obj_state_if_part_active(link);
    }
    return result;
}

void obj_to_state_aba4_if_active(struct ActorObj *obj)
{
    if (obj->pose->flags & 0x08)
    {
        obj->state = obj_wait_action6_then_state;
    }
}

void obj_anim6_pose_update_if_active(struct ActorObj *obj)
{
    if (obj->pose->flags & 0x08)
    {
        sprite_set_action(obj, 6, 0, 0);
        on_arrive(obj);
    }
}

void obj_wait_action6_then_state(struct ActorObj *obj)
{
    s32 timer = obj->timer - 1;

    obj->timer = timer;
    if ((s16)timer < 0)
    {
        sprite_set_action(obj, 6, 0, 0);
        obj->timer = 0;
        obj->state = obj_wait_action7_then_state;
    }
}

void obj_wait_action7_then_state(struct ActorObj *obj)
{
    s32 timer = obj->timer - 1;

    obj->timer = timer;
    if ((s16)timer < 0)
    {
        sprite_set_action(obj, 7, 0, 0);
        pose_set_mode_2(obj);
        obj->timer = 0;
        obj->state = obj_to_state_ac38_if_active;
    }
}

void obj_to_state_ac38_if_active(struct ActorObj *obj)
{
    if (obj->pose->flags & 0x08)
    {
        obj->state = obj_wait_action8_notify_effect;
    }
}

void obj_wait_action8_notify_effect(struct ActorObj *obj)
{
    s32 timer = obj->timer - 1;

    obj->timer = timer;
    if ((s16)timer < 0)
    {
        sprite_set_action(obj, 8, 0, 0);
        set_slot_value(0x83, -1);
        pose_set_mode_2(obj);
        effect_from_table_obj(obj, 0x1349);
        obj->handler60 = (void (*)(struct ActorObj *))obj_lookup_part_b;
        obj->savedState = 0;
        obj->state = obj_clear_timer_to_acbc_if_active;
    }
}

void obj_clear_timer_to_acbc_if_active(struct ActorObj *obj)
{
    if (obj->pose->flags & 0x08)
    {
        obj->timer = 0;
        obj->state = obj_wait_prep_a_action6_then_state;
    }
}

void obj_wait_prep_a_action6_then_state(struct ActorObj *obj)
{
    s32 timer = obj->timer - 1;

    obj->timer = timer;
    if ((s16)timer < 0)
    {
        obj_list_remove(obj);
        sprite_set_action(obj, 6, 0, 0);
        obj->timer = 8;
        obj->state = obj_wait_then_notify_action2;
    }
}
