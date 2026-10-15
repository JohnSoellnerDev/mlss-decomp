#include "global.h"

struct ActorPose
{
    u8 filler0[0x0C];
    u16 unkC;
    u8 filler0E[0x11 - 0x0E];
    u8 unk11;
    u8 flags;
    u8 filler13[0x20 - 0x13];
    u8 mode;
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
    u8 filler50[0x58 - 0x50];
    s32 (*handler58)(struct ActorObj *, u32);
    s32 (*handler5C)(struct ActorObj *);
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
    u8 filler7E[0x80 - 0x7E];
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
void wait_for_index(struct ActorObj *obj);
u32 partner_link_update(struct ActorObj *obj, u32 partner, u32 mode);
void obj_begin_move_to_params(struct ActorObj *obj);
u32 mt19937_next(void);
s32 sub_081DE850(s32 range, u32 seed);
s32 is_state_busy(void);
void sprite_set_action(struct ActorObj *obj, u32 action, u32 a2, u32 a3);
void obj_action0_to_idle(struct ActorObj *obj);
void obj_action3_wait_pose(struct ActorObj *obj);
void obj_spawn_child_when_none(struct ActorObj *obj);
void obj_wait_ready_action_flag(struct ActorObj *obj);
void obj_timer_expired_set_motion(struct ActorObj *obj);
void obj_begin_approach(struct ActorObj *obj);
void obj_pose_active_aim_at_owner_part(struct ActorObj *obj);
void obj_tick_counter_place(struct ActorObj *obj);
void obj_spawn_child_glide(struct ActorObj *obj);
void obj_slot_notify_action(struct ActorObj *obj);
void obj_begin_table_jump(struct ActorObj *obj);
void base_moves_obj_wait_ready_action2(struct ActorObj *obj);
void obj_index_zero_action5_timer2(struct ActorObj *obj);
void obj_timer_down_idle7(struct ActorObj *obj);
s32 obj_lookup_slot_free_heap_then_wait(struct ActorObj *obj);
void obj_timer_down_action4_then_idle(struct ActorObj *obj);
void obj_count_timer_then_move(struct ActorObj *obj);
void obj_begin_drop_to_base(struct ActorObj *obj);
u32 obj_unlink_reset(struct ActorObj *obj, u32 a, u32 b);
void sound_stop_song(u32 song);
void actor_pose_steps(struct ActorObj *obj);
void obj_begin_move_to_owner(struct ActorObj *obj);
void obj_spawn_child_sprite(struct ActorObj *obj);
void obj_begin_table_move(struct ActorObj *obj);
void obj_index_zero_set_motion(struct ActorObj *obj);
void obj_timer_action4_or_8(struct ActorObj *obj);
u32 lookup_slot(struct ActorObj *obj);
void resolve_contact_action(struct ActorObj *owner, struct ActorObj *obj, s32 a, s32 b, u32 code);
void start_motion(struct ActorObj *obj);
void obj_index_zero_set_motion_b(struct ActorObj *obj);
void obj_index_zero_action9_then_idle(struct ActorObj *obj);
void obj_wait_action7(struct ActorObj *obj);
void obj_action3_begin(struct ActorObj *obj);
void obj_list_push_default(struct ActorObj *obj);
void obj_pose_active_action3_place(struct ActorObj *obj);
void set_slot_value(u32 id, s32 value);
void effect_spawn_at(u32 type, s32 x, s32 y, s32 h, struct ActorObj *owner);
u8 poll_spline_path(u32 obj, s32 *outX, s32 *outY);
void heap_free_b(void *p);
extern u8 gParamTable[];
extern u32 gUnk_03000E18;
extern s32 gUnk_03000E1C;
extern s32 gUnk_03000E20;
void obj_wait_ready_setup_motion(struct ActorObj *obj);
void obj_timer_end_build_anim_record(struct ActorObj *obj);
void obj_remove_child_idle(struct ActorObj *obj);
void obj_wait_ready_spawn_glide(struct ActorObj *obj);
void obj_wait_ready_setup_offsets(struct ActorObj *obj);
void base_moves_obj_wait_ready_place_sprite(struct ActorObj *obj);

ASM_FUNC("asm/code_080616CC/obj_pose_active_aim_at_owner_part.s", void obj_pose_active_aim_at_owner_part(struct ActorObj *obj))

ASM_FUNC("asm/code_080616CC/obj_begin_approach.s", void obj_begin_approach(struct ActorObj *obj))

void obj_begin_idle_at_base(struct ActorObj *obj)
{
    s32 idx = obj->index;
    s32 f;

    if (idx == 0)
    {
        obj->targetX = obj->baseX;
        obj->targetY = obj->baseY;
        obj->targetH = obj->baseH - 0x3000;
        f = obj->flags;
        f |= 0x20;
        obj->flags = f;
        obj->motionSpeed = 0x280;
        obj->motionStart = idx;
        start_motion(obj);
        sprite_set_action(obj, 5, 0, 0);
        obj_list_push_default(obj);
        obj->handler58 = (s32 (*)(struct ActorObj *, u32))obj_unlink_reset;
        f = obj->mode;
        f |= 0x20;
        obj->mode = f;
        obj->timer = 0;
        obj->paramA8 = 0;
        obj->state = obj_count_timer_then_move;
    }
}

ASM_FUNC("asm/code_080616CC/obj_begin_move_to_owner.s", void obj_begin_move_to_owner(struct ActorObj *obj))

ASM_FUNC("asm/code_080616CC/obj_begin_drop_to_base.s", void obj_begin_drop_to_base(struct ActorObj *obj))

ASM_FUNC("asm/code_080616CC/obj_begin_move_to_params.s", void obj_begin_move_to_params(struct ActorObj *obj))

u32 obj_unlink_reset(struct ActorObj *obj, u32 a, u32 b)
{
    partner_link_update(obj, a, b);
    obj->index = 0;
    obj->state = obj_begin_move_to_params;
    return 0;
}

void obj_wait_action7(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        sprite_set_action(obj, 7, 0, 0);
        obj->state = obj_action0_to_idle;
    }
}

void obj_count_timer_then_move(struct ActorObj *obj)
{
    if (obj->timer <= 0x3FFF)
    {
        if (obj->owner->baseH > 0)
        {
            obj->paramA8 = 1;
        }
    }
    obj->timer++;
    if (obj->index == 0)
    {
        sprite_set_action(obj, 3, 0, 0);
        obj->pose->mode = 0x10;
        obj->state = obj_begin_move_to_owner;
    }
}

void obj_begin_hover_at_base(struct ActorObj *obj)
{
    s32 idx = obj->index;
    s32 f;

    if (idx == 0)
    {
        obj->targetX = obj->baseX;
        obj->targetY = obj->baseY;
        obj->targetH = obj->baseH - 0x3000;
        f = obj->flags;
        f |= 0x20;
        obj->flags = f;
        obj->motionSpeed = 0x280;
        obj->motionStart = idx;
        start_motion(obj);
        sprite_set_action(obj, 5, 0, 0);
        obj->state = obj_action3_begin;
    }
}

s32 obj_store_base_as_params(struct ActorObj *obj)
{
    obj->param9C = obj->baseX / 0x100;
    obj->paramA0 = obj->baseY / 0x100;
    obj->paramA4 = obj->baseH / 0x100;
    obj->state = base_moves_obj_wait_ready_action2;
    return 1;
}

void obj_action0_to_idle(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0, 0);
        obj->state = wait_for_index;
    }
}

void obj_action3_begin(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        sprite_set_action(obj, 3, 0, 0);
        obj->pose->mode = 0x10;
        obj->state = obj_begin_approach;
    }
}

void base_moves_obj_wait_ready_action2(struct ActorObj *obj)
{
    if (!is_state_busy())
    {
        sprite_set_action(obj, 2, 0, 0);
        obj->state = obj_action3_wait_pose;
    }
}

void obj_action3_wait_pose(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 3, 0, 0);
        obj->pose->mode = 0x10;
        obj->state = obj_pose_active_aim_at_owner_part;
    }
}

void obj_tick_counter_place(struct ActorObj *obj)
{
    obj->paramA0++;
    if (obj->paramA0 >= obj->paramA4)
    {
        obj->paramA0 = 0;
        if (obj->param9C == 0)
        {
            effect_spawn_at(0x125C, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        }
        else
        {
            effect_spawn_at(0x1274, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        }
    }
}

void base_moves_obj_wait_ready_place_sprite(struct ActorObj *obj)
{
    if (is_state_busy() == 0)
    {
        sprite_set_action(obj, 2, 0, 0);
        if (obj->param9C == 0)
        {
            effect_spawn_at(0x1254, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
            set_slot_value(0x88, -1);
        }
        else
        {
            effect_spawn_at(0x126C, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
            set_slot_value(0x88, -1);
        }
        obj->state = obj_begin_table_move;
    }
}

ASM_FUNC("asm/code_080616CC/obj_begin_table_move.s", void obj_begin_table_move(struct ActorObj *obj))

ASM_FUNC("asm/code_080616CC/obj_begin_table_jump.s", void obj_begin_table_jump(struct ActorObj *obj))

ASM_FUNC("asm/code_080616CC/obj_tick_wait_table_timer.s", void obj_tick_wait_table_timer(void))

ASM_FUNC("asm/code_080616CC/obj_tick_action4_timer.s", void obj_tick_action4_timer(void))

s32 unknown_part_steps_08062044(struct ActorObj *obj)
{
    obj->param9C = 1;
    obj->state = base_moves_obj_wait_ready_place_sprite;
    return 1;
}

s32 sub_08062058(struct ActorObj *obj)
{
    obj->param9C = 0;
    obj->state = base_moves_obj_wait_ready_place_sprite;
    return 1;
}

void obj_timer_down_idle7(struct ActorObj *obj)
{
    s32 t;

    obj_tick_counter_place(obj);
    t = obj->timer;
    t--;
    obj->timer = t;
    if ((s16)t <= 0)
    {
        sprite_set_action(obj, 7, 0, 0);
        obj->state = wait_for_index;
    }
}

void obj_timer_down_action5(struct ActorObj *obj)
{
    s32 t;

    obj_tick_counter_place(obj);
    if (obj->pose->flags & 8)
    {
        t = obj->timer;
        t--;
        obj->timer = t;
        if ((s16)t <= 0)
        {
            sprite_set_action(obj, 5, 0, 0);
            obj->state = obj_begin_table_jump;
        }
    }
}

ASM_FUNC("asm/code_080616CC/obj_slot_notify_action.s", void obj_slot_notify_action(struct ActorObj *obj))

ASM_FUNC("asm/code_080616CC/obj_spawn_child_sprite.s", void obj_spawn_child_sprite(struct ActorObj *obj))

ASM_FUNC("asm/code_080616CC/obj_spawn_child_glide.s", void obj_spawn_child_glide(struct ActorObj *obj))

ASM_FUNC("asm/code_080616CC/obj_spawn_child_when_none.s", void obj_spawn_child_when_none(struct ActorObj *obj))

ASM_FUNC("asm/code_080616CC/obj_update_jump_arc.s", void obj_update_jump_arc(void))

void obj_action0_then_spawn(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0, 0);
        obj->state = obj_spawn_child_when_none;
    }
}

void obj_update_scroll_left(struct ActorObj *obj)
{
    s32 x;

    obj->pose->unkC += 0x1600;
    x = obj->posX;
    x += -0x280;
    obj->posX = x;
    obj->posY = ((x - (obj->dx << 8)) * obj->targetY) / obj->targetX + (obj->dy << 8);
    if (x < -0x20)
    {
        obj_release(obj);
    }
}

void obj_action0_to_idle_b(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0, 0);
        obj->state = wait_for_index;
    }
}

void obj_timer_action4_or_8(struct ActorObj *obj)
{
    s32 t;

    if (obj->pose->flags & 8)
    {
        t = obj->timer;
        t--;
        obj->timer = t;
        if ((s16)t <= 0)
        {
            if ((obj->param9C & 1) == 0)
            {
                sprite_set_action(obj, 4, 0, 0);
            }
            else
            {
                sprite_set_action(obj, 8, 0, 0);
            }
            obj->state = obj_spawn_child_sprite;
        }
    }
}

s32 obj_bump_param_9c_a(struct ActorObj *obj)
{
    s32 *p = &obj->param9C;
    u32 v = *p;

    if (v > 2)
    {
        v = 2;
    }
    else
    {
        v += 2;
    }
    *p = v;
    obj->paramA0 = 1;
    obj->state = obj_wait_ready_action_flag;
    return 1;
}

s32 obj_param_from_slot_a(struct ActorObj *obj)
{
    if (obj->owner->part->slot == -1)
    {
        obj->param9C = 0;
    }
    else
    {
        obj->param9C = 1;
    }
    obj->paramA0 = 1;
    obj->state = obj_wait_ready_action_flag;
    return 1;
}

s32 unknown_part_steps_080628f0(struct ActorObj *obj)
{
    obj->state = obj_remove_child_idle;
    return 1;
}

s32 obj_bump_param_9c_b(struct ActorObj *obj)
{
    s32 *p = &obj->param9C;
    u32 v = *p;

    if (v > 2)
    {
        v = 2;
    }
    else
    {
        v += 2;
    }
    *p = v;
    obj->paramA0 = 0;
    obj->state = obj_wait_ready_action_flag;
    return 1;
}

s32 unknown_part_steps_08062928(struct ActorObj *obj)
{
    obj->state = obj_wait_ready_spawn_glide;
    return 1;
}

s32 obj_param_from_slot_b(struct ActorObj *obj)
{
    if (obj->owner->part->slot == -1)
    {
        obj->param9C = 0;
    }
    else
    {
        obj->param9C = 1;
    }
    obj->paramA0 = 0;
    obj->state = obj_wait_ready_action_flag;
    return 1;
}

void obj_remove_child_idle(struct ActorObj *obj)
{
    if (obj->child != NULL)
    {
        obj_release(obj->child);
    }
    obj->state = wait_for_index;
}

void obj_wait_ready_spawn_glide(struct ActorObj *obj)
{
    if (obj->child != NULL)
    {
        obj->state = wait_for_index;
    }
    else if (is_state_busy() == 0)
    {
        sprite_set_action(obj, 0xA, 0, 0);
        obj->state = obj_spawn_child_glide;
    }
}

void obj_wait_ready_action_flag(struct ActorObj *obj)
{
    s32 bit;

    if (obj->child != NULL)
    {
        obj_release(obj->child);
    }
    if (is_state_busy() == 0)
    {
        bit = obj->param9C & 1;
        switch (bit)
        {
        case 0:
            sprite_set_action(obj, 2, 0, 0);
            break;
        case 1:
            sprite_set_action(obj, 6, 0, 0);
            break;
        }
        obj->state = obj_slot_notify_action;
    }
}

ASM_FUNC("asm/code_080616CC/obj_begin_linked_wait.s", void obj_begin_linked_wait(void))
