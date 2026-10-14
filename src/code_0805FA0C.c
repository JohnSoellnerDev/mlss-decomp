#include "global.h"
#include "ram_routines.h"

struct ActorPose
{
    u8 filler0[0x0C];
    u16 unkC;
    u8 filler0E[0x11 - 0x0E];
    u8 unk11;
    u8 flags;
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
void sprite_set_action(struct ActorObj *obj, u32 action, u32 a2, u32 a3);
void set_slot_value(u32 id, s32 value);
void obj_release(struct ActorObj *obj);
void wait_for_index(struct ActorObj *obj);
void map_obj_wait_action2(struct ActorObj *obj);
void map_obj_place_at_base(struct ActorObj *obj);
void map_obj_place_owner_sprite(struct ActorObj *obj);
void obj_pose_active_action0_clear(struct ActorObj *obj);
void obj_pose_active_choose_motion(struct ActorObj *obj);
void pose_steps_obj_wait_ready_action2_b(struct ActorObj *obj);
void sound_stop_song(u32 song);
void map_obj_action5_spawn(struct ActorObj *obj);
void pose_steps_obj_pose_active_spawn_child(struct ActorObj *obj);
void obj_pose_active_action0c_timer8(struct ActorObj *obj);
void obj_pose_active_action8(struct ActorObj *obj);
void obj_pose_active_timer4_action9(struct ActorObj *obj);
void obj_pose_active_action6_timer1(struct ActorObj *obj);
void obj_pose_active_aim_part_action3(struct ActorObj *obj);
void obj_pos_x_dec_then_cull(struct ActorObj *obj);
void obj_pose_active_callback_clear_link(struct ActorObj *obj);
void obj_pose_active_action2(struct ActorObj *obj);
void obj_pose_active_timer_action7_effect(struct ActorObj *obj);
void obj_wait_ready_action2_external(struct ActorObj *obj);
void pose_steps_obj_pose_active_timer_action_d(struct ActorObj *obj);
void pose_steps_obj_index_zero_action4(struct ActorObj *obj);
s32 obj_query_link_slot_wait(struct ActorObj *obj);
void obj_timer_down_then_idle(struct ActorObj *obj);
void obj_pose_active_action5(struct ActorObj *obj);
void obj_pose_active_timer_action_a(struct ActorObj *obj);
void obj_pose_active_spawn_child_b(struct ActorObj *obj);
void obj_move_and_cull(struct ActorObj *obj);
void start_motion(struct ActorObj *obj);
void obj_list_push_default(struct ActorObj *obj);
u32 lookup_link_slot(struct ActorObj *obj);
s32 query_contact(struct ActorObj *obj);
u32 partner_link_update(struct ActorObj *obj, u32 partner, u32 mode);
void effect_spawn_at(u32 type, s32 x, s32 y, s32 h, struct ActorObj *owner);
u32 effect_from_table_obj(struct ActorObj *obj, u32 id);
extern u32 gUnk_03000E3C;
void offset_three_words_camera(s32 *a, s32 *b, s32 *c, u32 d);
void map_obj_action4_wait(struct ActorObj *obj);
void map_obj_settle_count(struct ActorObj *obj);
void obj_index_zero_reset_x(struct ActorObj *obj);
void obj_list_remove(struct ActorObj *obj);
void obj_advance_x_until_param(struct ActorObj *obj);
struct ActorObj *obj_spawn(struct ActorPart *part, u32 type, s32 x, s32 y, s32 h, u32 a, u32 b);
void on_arrive(struct ActorObj *obj);
void obj_pose_active_action1_timer8(struct ActorObj *obj);
void obj_index_zero_setup_action4(struct ActorObj *obj);
void obj_index_zero_slot_action4(struct ActorObj *obj);
void obj_index_zero_slot_action4_b(struct ActorObj *obj);
s32 obj_query_part_start_motion(struct ActorObj *obj);
void obj_part_ba_zero_finish(struct ActorObj *obj);
void obj_slide_step_then_wrap(struct ActorObj *obj);
s32 obj_query_part_place_and_finish(struct ActorObj *obj);
s32 obj_place_sprite_1118_and_finish(struct ActorObj *obj, u32 a);
void map_obj_spawn_child(struct ActorObj *obj);
void obj_timer_down_action7_then_idle(struct ActorObj *obj);
void obj_arc_step_timer(struct ActorObj *obj);

ASM_FUNC("asm/code_0805FA0C/map_obj_drift_x.s", void map_obj_drift_x(void))

ASM_FUNC("asm/code_0805FA0C/obj_orbit_place.s", void obj_orbit_place(void))

void map_obj_place_from_offsets(struct ActorObj *obj)
{
    s32 screen[3];

    obj->posX = obj->dx * 0x100;
    obj->posY = obj->dy * 0x100;
    obj->posH = obj->dh * 0x100;
    screen[0] = obj->posX / 0x100;
    screen[1] = obj->posY / 0x100;
    screen[2] = obj->posH / 0x100;
    offset_three_words_camera(&screen[0], &screen[1], &screen[2], 0);
    if (obj->param9C != 0)
    {
        obj_release(obj);
    }
}

ASM_FUNC("asm/code_0805FA0C/obj_set_sprite_offsets.s", void obj_set_sprite_offsets(void))

void map_obj_settle(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0, 0);
        if (obj->param9C == 0)
        {
            obj->state = wait_for_index;
        }
        else
        {
            if (obj->param9C == 1)
            {
                obj->paramA0 = 0;
            }
            else
            {
                obj->paramA0 = 0x96;
            }
            obj->state = map_obj_settle_count;
        }
    }
}

s32 unknown_part_steps_0805fde4(struct ActorObj *obj)
{
    obj->param9C = 2;
    obj->state = map_obj_wait_action2;
    return 1;
}

s32 map_obj_wait_mode1(struct ActorObj *obj)
{
    obj->param9C = 1;
    obj->state = map_obj_wait_action2;
    return 1;
}

s32 map_obj_wait_mode0(struct ActorObj *obj)
{
    obj->param9C = 0;
    obj->state = map_obj_wait_action2;
    return 1;
}

void map_obj_settle_count(struct ActorObj *obj)
{
    obj->paramA0--;
    if (obj->paramA0 < 0)
    {
        obj->state = wait_for_index;
    }
}

void map_obj_wait_action2(struct ActorObj *obj)
{
    if (!is_state_busy())
    {
        sprite_set_action(obj, 2, 0, 0);
        set_slot_value(0x118, -1);
        obj->state = map_obj_place_at_base;
    }
}

void map_obj_place_at_base(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 3, 0, 0);
        effect_spawn_at(0x12D5, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        obj->state = map_obj_action4_wait;
    }
}

void map_obj_action4_wait(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 4, 0, 0);
        obj->state = map_obj_action5_spawn;
    }
}

void map_obj_place_owner_sprite(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 3, 0, 0);
        set_slot_value(0x3A, -1);
        if (obj->dy == 0)
        {
            effect_spawn_at(0xDCE, obj->owner->part->targetX / 0x100, obj->owner->part->targetY / 0x100,
                            obj->owner->part->targetH / 0x100, obj);
        }
        else
        {
            effect_spawn_at(0xE1F, obj->owner->part->targetX / 0x100, obj->owner->part->targetY / 0x100,
                            obj->owner->part->targetH / 0x100, obj);
        }
        obj->state = map_obj_spawn_child;
    }
}

ASM_FUNC("asm/code_0805FA0C/map_obj_spawn_child.s", void map_obj_spawn_child(struct ActorObj *obj))

ASM_FUNC("asm/code_0805FA0C/pose_steps_obj_pose_active_spawn_child.s", void pose_steps_obj_pose_active_spawn_child(struct ActorObj *obj))

void obj_slide_step_then_wrap(struct ActorObj *obj)
{
    s32 dX = obj->param9C - obj->targetX;
    s32 dY = obj->paramA0 - obj->targetY;

    if (obj->dy != 0)
    {
        *(u16 *)&obj->dx += 0x10;
    }
    else
    {
        *(u16 *)&obj->dx += 0x13;
    }
    obj->posX -= obj->dx;
    obj->posY = (obj->posX - obj->param9C) * dY / dX + obj->paramA0;
    {
        s32 t = obj->timer;

        t++;
        obj->timer = t;
        if ((s16)t > 7)
        {
            obj->timer = 0;
            effect_spawn_at(0xE72, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        }
    }
    if (obj->posX < -0x20)
    {
        obj_list_remove(obj);
        obj->posX = 0x11000;
        obj->posY = obj->paramA0;
        obj->state = obj_advance_x_until_param;
    }
}

void obj_drift_fast(struct ActorObj *obj)
{
    s32 screen[3];
    s32 dX = obj->paramA4 - obj->targetX;
    s32 dY = obj->paramA8 - obj->targetY;

    *(u16 *)&obj->dx += 0xC;
    obj->posX -= obj->dx;
    obj->posY = (obj->posX - obj->paramA4) * dY / dX + obj->paramA8;
    screen[0] = obj->posX / 0x100;
    screen[1] = obj->posY / 0x100;
    screen[2] = obj->posH / 0x100;
    offset_three_words_camera(&screen[0], &screen[1], &screen[2], 0);
    {
        s32 t = obj->timer;

        t++;
        obj->timer = t;
        if ((s16)t > 0xB)
        {
            obj->timer = 0;
            effect_spawn_at(0xE7A, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        }
    }
    if (screen[0] < -0x20)
    {
        obj_release(obj);
    }
}

void obj_drift_slow(struct ActorObj *obj)
{
    s32 screen[3];
    s32 dX = obj->paramA4 - obj->targetX;
    s32 dY = obj->paramA8 - obj->targetY;

    *(u16 *)&obj->dx += 4;
    obj->posX -= obj->dx;
    obj->posY = (obj->posX - obj->paramA4) * dY / dX + obj->paramA8;
    screen[0] = obj->posX / 0x100;
    screen[1] = obj->posY / 0x100;
    screen[2] = obj->posH / 0x100;
    offset_three_words_camera(&screen[0], &screen[1], &screen[2], 0);
    {
        s32 t = obj->timer;

        t++;
        obj->timer = t;
        if ((s16)t > 0xB)
        {
            obj->timer = 0;
            effect_spawn_at(0xE7A, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        }
    }
    if (screen[0] < -0x20)
    {
        obj_release(obj);
    }
}

s32 obj_query_part_settle(struct ActorObj *obj)
{
    s32 result = query_contact(obj);
    s32 ret;

    if (result == 0)
    {
        obj->dx = obj->dx / 2;
        obj->dy = 0;
        obj->dh = 0;
        sprite_set_action(obj, 1, 0, 0);
        obj->state = obj_move_and_cull;
        ret = 0;
    }
    else
    {
        ret = result;
    }
    return ret;
}

ASM_FUNC("asm/code_0805FA0C/obj_query_part_start_motion.s", s32 obj_query_part_start_motion(struct ActorObj *obj))

void obj_part_ba_zero_finish(struct ActorObj *obj)
{
    if (obj->part->valueBA == 0)
    {
        obj_release(obj);
    }
}

void obj_advance_x_until_param(struct ActorObj *obj)
{
    if (obj->dy != 0)
    {
        *(u16 *)&obj->dx += 0x10;
    }
    else
    {
        *(u16 *)&obj->dx += 0x13;
    }
    obj->posX -= obj->dx;
    if (obj->posX < obj->param9C)
    {
        obj->dh = 0;
        obj->posX = obj->param9C;
        sprite_set_action(obj, 8, 0, 0);
        obj->state = obj_pose_active_callback_clear_link;
    }
}

void obj_pose_active_action5_then_spawn(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 5, 0, 0);
        obj->state = pose_steps_obj_pose_active_spawn_child;
    }
}

s32 unknown_part_steps_08060620(struct ActorObj *obj)
{
    obj->dx = 0x100;
    obj->targetX = obj->owner->part->targetX;
    obj->targetY = obj->owner->part->targetY;
    obj->param9C = obj->posX;
    obj->paramA0 = obj->posY;
    obj->dy = 1;
    obj->state = obj_wait_ready_action2_external;
    return 1;
}

s32 obj_setup_owner_target_drift(struct ActorObj *obj)
{
    obj->dx = 0x100;
    obj->targetX = obj->owner->part->targetX;
    obj->targetY = obj->owner->part->targetY;
    obj->param9C = obj->posX;
    obj->paramA0 = obj->posY;
    obj->dy = 0;
    obj->state = obj_wait_ready_action2_external;
    return 1;
}

ASM_FUNC("asm/code_0805FA0C/obj_move_and_cull.s", void obj_move_and_cull(struct ActorObj *obj))

void obj_index_zero_reset_x(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        s32 bits = obj->pose->unk11;

        bits |= 0x40;
        obj->pose->unk11 = bits;
        obj->dh = 0;
        obj->posX = obj->param9C;
        sprite_set_action(obj, 8, 0, 0);
        obj->state = obj_pose_active_callback_clear_link;
    }
}

void obj_pose_active_callback_clear_link(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        void (*callback)(struct ActorObj *) = obj->handler68;

        if (callback != 0)
        {
            callback(obj);
        }
        obj->link = 0;
    }
}

void obj_wait_ready_action2_external(struct ActorObj *obj)
{
    if (!is_state_busy())
    {
        sprite_set_action(obj, 2, 0, 0);
        obj->state = map_obj_place_owner_sprite;
    }
}

ASM_FUNC("asm/code_0805FA0C/obj_pose_active_choose_motion.s", void obj_pose_active_choose_motion(struct ActorObj *obj))

ASM_FUNC("asm/code_0805FA0C/obj_pose_active_spawn_child_b.s", void obj_pose_active_spawn_child_b(struct ActorObj *obj))

s32 obj_query_part_place_and_finish(struct ActorObj *obj)
{
    s32 result = query_contact(obj);
    s32 ret;

    if (result == 0)
    {
        effect_spawn_at(0x110F, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        obj_release(obj);
        ret = 0;
    }
    else
    {
        ret = result;
    }
    return ret;
}

s32 obj_place_sprite_1118_and_finish(struct ActorObj *obj, u32 a)
{
    partner_link_update(obj, a, 0xFF);
    effect_spawn_at(0x1118, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
    obj_release(obj);
    return 0;
}

void obj_pose_active_action1_timer8(struct ActorObj *obj)
{
    obj->posX -= 0x166;
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 1, 0, 0);
        obj->timer = 8;
        obj->state = obj_pos_x_dec_then_cull;
    }
}

void obj_index_zero_slot_action4(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        sound_stop_song(0x5C);
        sprite_set_action(obj, 4, 0, 0);
        obj->state = obj_pose_active_action0c_timer8;
    }
}

void obj_index_zero_slot_action4_b(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        sound_stop_song(0x5C);
        sprite_set_action(obj, 4, 0, 0);
        obj->state = obj_pose_active_action8;
    }
}

ASM_FUNC("asm/code_0805FA0C/obj_pose_active_aim_part_action3.s", void obj_pose_active_aim_part_action3(struct ActorObj *obj))

void obj_index_zero_setup_action4(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        sound_stop_song(0x5C);
        obj_list_push_default(obj);
        obj->handler60 = obj_query_link_slot_wait;
        sprite_set_action(obj, 4, 0, 0);
        obj->state = obj_pose_active_action5;
    }
}

s32 obj_query_link_slot_wait(struct ActorObj *obj)
{
    s32 slot = lookup_link_slot(obj);
    s32 result;

    if (slot == 0)
    {
        obj->index = slot;
        sprite_set_action(obj, 1, 0, 0);
        obj->timer = 0x1E;
        obj->state = obj_timer_down_then_idle;
        result = 0;
    }
    else
    {
        result = slot;
    }
    return result;
}

s32 unknown_part_steps_08060bd8(struct ActorObj *obj)
{
    obj->param9C = 2;
    obj->state = pose_steps_obj_wait_ready_action2_b;
    return 1;
}

s32 pose_steps_obj_wait_mode1(struct ActorObj *obj)
{
    obj->param9C = 1;
    obj->state = pose_steps_obj_wait_ready_action2_b;
    return 1;
}

s32 pose_steps_obj_wait_mode0(struct ActorObj *obj)
{
    obj->param9C = 0;
    obj->state = pose_steps_obj_wait_ready_action2_b;
    return 1;
}

ASM_FUNC("asm/code_0805FA0C/obj_pos_x_dec_then_cull.s", void obj_pos_x_dec_then_cull(struct ActorObj *obj))
