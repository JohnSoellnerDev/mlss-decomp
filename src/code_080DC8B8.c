#include "global.h"
#include "ram_routines.h"

struct ActorPose
{
    u8 filler0[0x11];
    u8 byte11;
    u8 flags;
    u8 filler13[0x21 - 0x13];
    u8 busy;
};

struct ActorPart
{
    u8 filler0[0xD8];
    s32 targetX;
    s32 targetY;
    s32 targetH;
};

struct ActorPoseFlagBits
{
    u8 bit0 : 1;
    u8 mode : 2;
    u8 high : 5;
};

struct ActorModeBits
{
    u8 low5 : 5;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 high : 1;
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
    u8 filler50[0x54 - 0x50];
    void (*savedState)(struct ActorObj *);
    void (*handler58)(struct ActorObj *);
    void (*handler5C)(struct ActorObj *);
    void (*handler60)(struct ActorObj *);
    u8 filler64[0x68 - 0x64];
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
    s32 spawnLeft;
    u16 timer;
    s16 dx;
    s16 dy;
    s16 dh;
};

u32 is_state_busy(void);
u32 mt19937_next(void);
u32 rand_below(u32 n, u32 r);
void sprite_set_action(struct ActorObj *obj, u32 action, u32 a2, u32 a3);
void set_slot_value(u32 id, s32 value);
void obj_list_push_default(struct ActorObj *obj);
void obj_wait_target_h800_motion(struct ActorObj *obj);
void obj_target_motion_action8_notify(struct ActorObj *obj);
void obj_list_remove(struct ActorObj *obj);
void start_motion(struct ActorObj *obj);
void obj_release(struct ActorObj *obj);
void offset_three_words_camera(s32 *a, s32 *b, s32 *c, u32 d);
u32 jump_velocity(s16 *dx, s16 *dy, s16 *dh, s32 dX, s32 dY, s32 dH, s32 a, s32 b, s32 c, s32 d);
void obj_spawn_aimed_child_pair(struct ActorObj *obj);
void move_step(s16 *x, s16 *y, s16 *h, s32 dX, s32 dY, s32 dH, s32 tempo);
struct ActorObj *obj_spawn(struct ActorPart *part, u32 type, s32 x, s32 y, s32 h, u32 a, u32 b);
void obj_begin_fall_to_owner_alt(struct ActorObj *obj);
void obj_slot_found_recoil_kids(struct ActorObj *obj);
void obj_pose_active_idle_arrive(struct ActorObj *obj);
void sound_stop_song(u32 song);
void on_arrive(struct ActorObj *obj);
void wait_for_index(struct ActorObj *obj);
u32 partner_link_update(struct ActorObj *obj, u32 partner, u32 mode);
u32 lookup_part(struct ActorObj *obj, u32 a, u32 b);
u32 lookup_link_slot(struct ActorObj *obj, u32 a, u32 b);
void release_object_data(struct ActorPart *data);

void obj_link_partner_span_start(struct ActorObj *obj);
void obj_anim3_timer24_then_next(struct ActorObj *obj);
void obj_list_setup_anim7(struct ActorObj *obj);
u32 obj_cb_lookup_link_slot(struct ActorObj *obj, u32 a, u32 b);
u32 part_lookup_clear_81(struct ActorObj *obj, u32 a, u32 b);
u32 part_unlink_clear_81(struct ActorObj *obj, u32 a, u32 b);
void obj_begin_part_to_owner_x52(struct ActorObj *obj);
void obj_settle_step_roll_bounces(struct ActorObj *obj);
void obj_settle_wait_bounce(struct ActorObj *obj);
void obj_settle_step_land_then_bce4(struct ActorObj *obj);
void obj_wait_timer_hop_then_bd88(struct ActorObj *obj);
void obj_settle_step_land_then_be2c(struct ActorObj *obj);
void obj_wait_timer_release_hop_then_bed0(struct ActorObj *obj);
void obj_settle_step_land_arrive(struct ActorObj *obj);
void obj_settle_step(struct ActorObj *obj);
void obj_begin_settle(struct ActorObj *obj);
u32 part_step_obj_wait_free_then_ba20(struct ActorObj *obj);
void obj_wait_free_then_ba20(struct ActorObj *obj);
void obj_wait_then_follow_owner(struct ActorObj *obj);
void obj_wait_then_follow_owner_speed(struct ActorObj *obj);
void obj_wait_index_then_move_to_data(struct ActorObj *obj);
void obj_drop_until_ground(struct ActorObj *obj);
void obj_wait_then_move_to_edge(struct ActorObj *obj);
void obj_wait_then_place_at_owner(struct ActorObj *obj);
void obj_setup_move_to_data(struct ActorObj *obj);
u32 obj_await_part(struct ActorObj *obj, u32 a, u32 b);
void obj_await_part_done(struct ActorObj *obj);
void obj_wait_index_release_11a(struct ActorObj *obj);
void obj_wait_index_action3_timer(struct ActorObj *obj);
u32 sprite_state_chain_a(struct ActorObj *obj);
u32 sprite_state_chain_b(struct ActorObj *obj);
u32 sprite_state_random_chain_a(struct ActorObj *obj);
void obj_wait_index_pose_toggle(struct ActorObj *obj);
void obj_wait_free_then_begin_drop(struct ActorObj *obj);
void obj_wait_free_then_action2_slot(struct ActorObj *obj);
void obj_begin_drop(struct ActorObj *obj);
void obj_action2_slot_then_wait(struct ActorObj *obj);
void obj_random_move_start(struct ActorObj *obj);
void obj_wait_then_move_screen(struct ActorObj *obj);
void obj_wait_index_then_slot84(struct ActorObj *obj);
void obj_wait_index_release_data(struct ActorObj *obj);
u32 sprite_state_step_b(struct ActorObj *obj);
void obj_timer_compute_step(struct ActorObj *obj);
void move_states_obj_move_toward_target(struct ActorObj *obj);
void obj_pose_active_attach_unlink(struct ActorObj *obj);
void obj_wait_index_then_release_move(struct ActorObj *obj);
void obj_wait_index_go_idle(struct ActorObj *obj);
u32 obj_unlink_part(struct ActorObj *obj, u32 a, u32 b);
void obj_pose_active_release_move(struct ActorObj *obj);
void actor_return_to_base_steps(struct ActorObj *obj);
u32 sprite_state_chain_c(struct ActorObj *obj);
u32 sub_080DCCA8(struct ActorObj *obj);
u32 sprite_state_random_chain_b(struct ActorObj *obj);
void obj_wait_free_then_move_d(struct ActorObj *obj);
void obj_wait_index_pose_update(struct ActorObj *obj);
void obj_wait_free_then_cd90(struct ActorObj *obj);
void obj_move_to_owner_x52(struct ActorObj *obj);
void obj_move_to_owner_x32_slot(struct ActorObj *obj);
void obj_wait_index_action7(struct ActorObj *obj);
void obj_wait_index_action3_pose(struct ActorObj *obj);
void move_states_obj_timer_action8(struct ActorObj *obj);
void obj_pose_active_action4(struct ActorObj *obj);
void obj_screen_cull_step(struct ActorObj *obj);
void obj_drop_land_stop(struct ActorObj *obj);
void obj_drop_clamp_to_limit(struct ActorObj *obj);

void obj_wait_index_then_slot84(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        sprite_set_action(obj, 5, 0, 0);
        set_slot_value(0x84, -1);
        obj->timer = 0x20;
        obj->state = obj_wait_then_move_screen;
    }
}

void obj_wait_index_release_data(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        release_object_data(obj->data);
        on_arrive(obj);
    }
}

u32 sprite_state_step_b(struct ActorObj *obj)
{
    obj->state = obj_random_move_start;
    return 1;
}

void obj_timer_compute_step(struct ActorObj *obj)
{
    struct ActorPart *data;
    s32 dX;
    s32 dY;
    s32 dH;
    s32 dist;
    s32 t;

    obj->timer--;
    if ((s16)obj->timer < 0)
    {
        obj->targetX = obj->baseX;
        obj->targetY = obj->baseY;
        obj->targetH = obj->baseH;
        data = obj->owner->data;
        dX = (data->targetX - obj->baseX) / 4;
        dY = (data->targetY - obj->baseY) / 4;
        dH = (data->targetH - obj->baseH) / 4;
        dist = ((s32 (*)(s32))gMathRoutines)(dX * dX + dY * dY + dH * dH);
        t = (dist << 3) + (obj->dx = 0x200);
        obj->dy = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)((t >> 1) + 0xFFFC0000, t);
        obj->savedState = actor_return_to_base_steps;
        sprite_set_action(obj, 5, 0, 0);
        obj->state = move_states_obj_move_toward_target;
    }
}

void move_states_obj_move_toward_target(struct ActorObj *obj)
{
    s16 x;
    s16 y;
    s16 h;
    struct ActorPart *data;
    s16 *dx = &obj->dx;

    *dx = *dx + obj->dy;
    move_step(&x, &y, &h, (data = obj->owner->data)->targetX - obj->targetX, data->targetY - obj->targetY, obj->owner->data->targetH - obj->targetH, *dx);
    obj->posX = obj->baseX + x;
    obj->posY = obj->baseY + y;
    obj->posH = obj->baseH + h;
    if (*dx <= -0x200)
    {
        sprite_set_action(obj, 6, 0, 0);
        ((struct ActorPoseFlagBits *)&obj->pose->flags)->mode = 1;
        obj->state = obj_pose_active_release_move;
    }
}

void obj_pose_active_attach_unlink(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        struct ActorPart *data;
        u8 *flags;

        obj_list_push_default(obj);
        obj->handler58 = (void (*)(struct ActorObj *))obj_unlink_part;
        obj->targetX = (data = obj->owner->data)->targetX;
        obj->targetY = data->targetY;
        obj->targetH = data->targetH;
        flags = &obj->flags;
        *flags |= 0x20;
        obj->motionSpeed = 0x200;
        obj->motionStart = 0;
        start_motion(obj);
        sprite_set_action(obj, 9, 0, 0);
        set_slot_value(0x81, -1);
        obj->timer = 2;
        obj->state = obj_wait_index_then_release_move;
    }
}

void obj_wait_index_then_release_move(struct ActorObj *obj)
{
    s32 index = obj->index;

    if (index == 0)
    {
        struct ActorPart *data;
        u8 *flags;

        obj->timer--;
        if ((s16)obj->timer < 0)
        {
            obj_list_remove(obj);
            sound_stop_song(0x81);
            obj->targetX = (data = obj->owner->data)->targetX + 0x3400;
            obj->targetY = data->targetY;
            obj->targetH = data->targetH;
            flags = &obj->flags;
            *flags |= 0x20;
            obj->motionSpeed = 0x600;
            obj->motionStart = index;
            start_motion(obj);
            sprite_set_action(obj, 2, 0, 0);
            obj->state = obj_wait_index_go_idle;
        }
    }
}

void obj_wait_index_go_idle(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        obj->state = wait_for_index;
    }
}

u32 obj_unlink_part(struct ActorObj *obj, u32 a, u32 b)
{
    u32 result = partner_link_update(obj, a, b);
    sound_stop_song(0x81);
    return result;
}

void obj_pose_active_release_move(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        struct ActorPart *data;
        u8 *flags;

        obj_list_remove(obj);
        obj->targetX = (data = obj->data)->targetX;
        obj->targetY = data->targetY;
        obj->targetH = data->targetH;
        flags = &obj->flags;
        *flags |= 0x20;
        obj->motionSpeed = 0x600;
        obj->motionStart = 0;
        start_motion(obj);
        sprite_set_action(obj, 2, 0, 0);
        obj->state = obj_wait_index_pose_update;
    }
}

void actor_return_to_base_steps(struct ActorObj *obj)
{
    obj->dx = 0xFF9C;
    obj->state = move_states_obj_move_toward_target;
}

u32 sprite_state_chain_c(struct ActorObj *obj)
{
    obj->state = obj_wait_free_then_move_d;
    return 1;
}

u32 sub_080DCCA8(struct ActorObj *obj)
{
    obj->state = obj_wait_free_then_cd90;
    return 1;
}

u32 sprite_state_random_chain_b(struct ActorObj *obj)
{
    u32 r = mt19937_next();

    if (rand_below(1, r) != 0)
    {
        obj->state = obj_wait_free_then_cd90;
    }
    else
    {
        obj->state = obj_wait_free_then_move_d;
    }
    return 1;
}

void obj_wait_free_then_move_d(struct ActorObj *obj)
{
    if (is_state_busy() == 0)
    {
        obj->state = obj_move_to_owner_x52;
    }
}

void obj_wait_index_pose_update(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        on_arrive(obj);
    }
}

void obj_wait_free_then_cd90(struct ActorObj *obj)
{
    if (is_state_busy() == 0)
    {
        obj->state = obj_move_to_owner_x32_slot;
    }
}

void obj_move_to_owner_x52(struct ActorObj *obj)
{
    struct ActorPart *data;
    u8 *flags;

    obj->targetX = (data = obj->owner->data)->targetX + 0x3400;
    obj->targetY = data->targetY;
    obj->targetH = data->targetH;
    flags = &obj->flags;
    *flags |= 0x20;
    obj->motionSpeed = 0x400;
    obj->motionStart = 0;
    start_motion(obj);
    obj->state = obj_wait_index_action7;
}

void obj_move_to_owner_x32_slot(struct ActorObj *obj)
{
    struct ActorPart *data;
    u8 *flags;

    obj->targetX = (data = obj->owner->data)->targetX + 0x2000;
    obj->targetY = data->targetY;
    obj->targetH = data->targetH;
    flags = &obj->flags;
    *flags |= 0x20;
    obj->motionSpeed = 0x400;
    obj->motionStart = 0;
    start_motion(obj);
    set_slot_value(0x81, -1);
    sprite_set_action(obj, 2, 0, 0);
    obj->state = obj_wait_index_action3_pose;
}

void obj_wait_index_action7(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        sprite_set_action(obj, 7, 0, 0);
        obj->timer = 0x20;
        obj->state = move_states_obj_timer_action8;
    }
}

void obj_wait_index_action3_pose(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        sprite_set_action(obj, 3, 0, 0);
        ((struct ActorPoseFlagBits *)&obj->pose->flags)->mode = 1;
        obj_list_push_default(obj);
        obj->state = obj_pose_active_action4;
    }
}

void move_states_obj_timer_action8(struct ActorObj *obj)
{
    obj->timer--;
    if ((s16)obj->timer < 0)
    {
        sprite_set_action(obj, 8, 0, 0);
        obj->state = obj_pose_active_attach_unlink;
    }
}

void obj_pose_active_action4(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 4, 0, 0);
        set_slot_value(0x62, -1);
        obj->timer = 0x20;
        obj->state = obj_timer_compute_step;
    }
}

void obj_screen_cull_step(struct ActorObj *obj)
{
    s32 screen[3];

    obj->dx += 4;
    if (obj->pose->byte11 & 0x40)
    {
        obj->posX = obj->baseX - obj->dx;
    }
    else
    {
        obj->posX = obj->baseX + obj->dx;
    }
    screen[0] = obj->posX / 0x100;
    screen[1] = obj->posY / 0x100;
    screen[2] = obj->posH / 0x100;
    offset_three_words_camera(&screen[0], &screen[1], &screen[2], 0);
    if (screen[0] <= -0x10)
    {
        obj_release(obj);
    }
}

void obj_drop_land_stop(struct ActorObj *obj)
{
    obj->dx = obj->dx * 0xFD / 0x100;
    obj->dy = obj->dy * 0xFD / 0x100;
    obj->dh += obj->dhStep;
    obj->posX = obj->baseX + obj->dx;
    obj->posY = obj->baseY + obj->dy;
    obj->posH = obj->baseH + obj->dh;
    if (obj->dh <= 0)
    {
        obj_spawn_aimed_child_pair(obj);
        obj->childValue = 0;
        sprite_set_action(obj, 0xC, 0, 0);
        ((struct ActorPoseFlagBits *)&obj->pose->flags)->mode = 1;
        obj->state = obj_drop_clamp_to_limit;
    }
}

void obj_drop_clamp_to_limit(struct ActorObj *obj)
{
    obj->dx = obj->dx * 0xFD / 0x100;
    obj->dy = obj->dy * 0xFD / 0x100;
    obj->dh += obj->dhStep;
    obj->posX = obj->baseX + obj->dx;
    obj->posY = obj->baseY + obj->dy;
    obj->posH = obj->baseH + obj->dh;
    if (obj->posH <= obj->ground)
    {
        obj->posH = obj->ground;
        sprite_set_action(obj, 0, 0, 0);
        on_arrive(obj);
    }
}
