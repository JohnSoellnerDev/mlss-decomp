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
u32 sub_081DE850(u32 n, u32 r);
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

void obj_link_partner_span_start(struct ActorObj *obj)
{
    s32 index = obj->index;

    if (index == 0)
    {
        struct ActorPart *data;
        u8 *flags;

        set_slot_value(0x8D, -1);
        obj->targetX = (data = obj->owner->data)->targetX + 0x2800;
        obj->targetY = data->targetY;
        obj->targetH = data->targetH;
        flags = &obj->flags;
        *flags |= 0x20;
        obj->motionSpeed = 0x500;
        obj->motionStart = index;
        start_motion(obj);
        sprite_set_action(obj, 6, 0, 0);
        obj->state = obj_list_setup_anim7;
    }
}

void obj_anim3_timer24_then_next(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        sprite_set_action(obj, 3, 0, 0);
        obj->timer = 0x18;
        obj->state = obj_wait_target_h800_motion;
    }
}

void obj_list_setup_anim7(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        set_slot_value(0x2E, -1);
        obj_list_push_default(obj);
        obj->handler58 = (void (*)(struct ActorObj *))part_unlink_clear_81;
        obj->handler5C = (void (*)(struct ActorObj *))part_lookup_clear_81;
        obj->handler60 = (void (*)(struct ActorObj *))obj_cb_lookup_link_slot;
        sprite_set_action(obj, 7, 0, 0);
        obj->timer = 0x10;
        obj->state = obj_target_motion_action8_notify;
    }
}

u32 obj_cb_lookup_link_slot(struct ActorObj *obj, u32 a, u32 b)
{
    sound_stop_song(0x81);
    return lookup_link_slot(obj, a, b);
}

u32 part_lookup_clear_81(struct ActorObj *obj, u32 a, u32 b)
{
    sound_stop_song(0x81);
    return lookup_part(obj, a, b);
}

u32 part_unlink_clear_81(struct ActorObj *obj, u32 a, u32 b)
{
    sound_stop_song(0x81);
    return partner_link_update(obj, a, b);
}

void obj_begin_part_to_owner_x52(struct ActorObj *obj)
{
    struct ActorPart *data;
    s32 h;

    obj->targetX = (data = obj->owner->data)->targetX + 0x3400;
    obj->targetY = data->targetY;
    obj->targetH = (h = data->targetH);
    obj->ground = h;
    obj->dhStep = jump_velocity(&obj->dx, &obj->dy, &obj->dh, obj->targetX - obj->baseX, obj->targetY - obj->baseY, h - obj->baseH, 0x400, 0x400, 0xFD, 0);
    set_slot_value(0x8D, -1);
    sprite_set_action(obj, 2, 0, 0);
    obj->state = obj_settle_step_roll_bounces;
}

void obj_settle_step_roll_bounces(struct ActorObj *obj)
{
    obj->dx = obj->dx * 0xFD / 0x100;
    obj->dy = obj->dy * 0xFD / 0x100;
    obj->dh += obj->dhStep;
    obj->posX = obj->baseX + obj->dx;
    obj->posY = obj->baseY + obj->dy;
    obj->posH = obj->baseH + obj->dh;
    if (obj->dh <= 0 && obj->posH <= obj->ground)
    {
        obj->posH = obj->ground;
        sprite_set_action(obj, 4, 0, 0);
        obj->spawnLeft = sub_081DE850(3, mt19937_next());
        obj->timer = 8;
        obj->state = obj_settle_wait_bounce;
    }
}

void obj_settle_wait_bounce(struct ActorObj *obj)
{
    obj->timer--;
    if ((s16)obj->timer < 0)
    {
        struct ActorPart *data;
        s32 h;

        obj->spawnLeft--;
        if (obj->spawnLeft >= 0)
        {
            obj->state = obj_begin_settle;
        }
        else
        {
            obj_list_push_default(obj);
            obj->targetX = (data = obj->owner->data)->targetX;
            obj->targetY = data->targetY;
            obj->targetH = (h = data->targetH);
            obj->ground = h;
            obj->dhStep = jump_velocity(&obj->dx, &obj->dy, &obj->dh, (obj->targetX - obj->baseX) / 2, (obj->targetY - obj->baseY) / 2, (h - obj->baseH) / 2, 0x800, 0x200, 0xFD, 0);
            set_slot_value(0x8D, -1);
            sprite_set_action(obj, 3, 0, 0);
            obj->state = obj_settle_step_land_then_bce4;
        }
    }
}

void obj_settle_step_land_then_bce4(struct ActorObj *obj)
{
    obj->dx = obj->dx * 0xFD / 0x100;
    obj->dy = obj->dy * 0xFD / 0x100;
    obj->dh += obj->dhStep;
    obj->posX = obj->baseX + obj->dx;
    obj->posY = obj->baseY + obj->dy;
    obj->posH = obj->baseH + obj->dh;
    if (obj->dh <= 0 && obj->posH <= obj->ground)
    {
        obj->posH = obj->ground;
        sprite_set_action(obj, 4, 0, 0);
        obj->timer = 8;
        obj->state = obj_wait_timer_hop_then_bd88;
    }
}

void obj_wait_timer_hop_then_bd88(struct ActorObj *obj)
{
    obj->timer--;
    if ((s16)obj->timer < 0)
    {
        struct ActorPart *data;
        u8 *flags;
        s32 h;

        obj->targetX = (data = obj->owner->data)->targetX;
        obj->targetY = data->targetY;
        obj->targetH = (h = data->targetH);
        obj->ground = h;
        obj->dhStep = jump_velocity(&obj->dx, &obj->dy, &obj->dh, obj->targetX - obj->baseX, obj->targetY - obj->baseY, h - obj->baseH, 0x800, 0x200, 0xFD, 0);
        set_slot_value(0x8D, -1);
        sprite_set_action(obj, 3, 0, 0);
        obj->state = obj_settle_step_land_then_be2c;
    }
}

void obj_settle_step_land_then_be2c(struct ActorObj *obj)
{
    obj->dx = obj->dx * 0xFD / 0x100;
    obj->dy = obj->dy * 0xFD / 0x100;
    obj->dh += obj->dhStep;
    obj->posX = obj->baseX + obj->dx;
    obj->posY = obj->baseY + obj->dy;
    obj->posH = obj->baseH + obj->dh;
    if (obj->dh <= 0 && obj->posH <= obj->ground)
    {
        obj->posH = obj->ground;
        sprite_set_action(obj, 4, 0, 0);
        obj->timer = 0x10;
        obj->state = obj_wait_timer_release_hop_then_bed0;
    }
}

void obj_wait_timer_release_hop_then_bed0(struct ActorObj *obj)
{
    obj->timer--;
    if ((s16)obj->timer < 0)
    {
        struct ActorPart *data;
        u8 *flags;
        s32 h;

        obj_list_remove(obj);
        obj->targetX = (data = obj->data)->targetX;
        obj->targetY = data->targetY;
        obj->targetH = (h = data->targetH);
        obj->ground = h;
        obj->dhStep = jump_velocity(&obj->dx, &obj->dy, &obj->dh, obj->targetX - obj->baseX, obj->targetY - obj->baseY, h - obj->baseH, 0x400, 0x400, 0xFD, 0);
        set_slot_value(0x8D, -1);
        sprite_set_action(obj, 2, 0, 0);
        obj->state = obj_settle_step_land_arrive;
    }
}

void obj_settle_step_land_arrive(struct ActorObj *obj)
{
    obj->dx = obj->dx * 0xFD / 0x100;
    obj->dy = obj->dy * 0xFD / 0x100;
    obj->dh += obj->dhStep;
    obj->posX = obj->baseX + obj->dx;
    obj->posY = obj->baseY + obj->dy;
    obj->posH = obj->baseH + obj->dh;
    if (obj->dh <= 0 && obj->posH <= obj->ground)
    {
        on_arrive(obj);
    }
}

void obj_settle_step(struct ActorObj *obj)
{
    obj->dx = obj->dx * 0xFD / 0x100;
    obj->dy = obj->dy * 0xFD / 0x100;
    obj->dh += obj->dhStep;
    obj->posX = obj->baseX + obj->dx;
    obj->posY = obj->baseY + obj->dy;
    obj->posH = obj->baseH + obj->dh;
    if (obj->dh <= 0 && obj->posH <= obj->ground)
    {
        obj->timer = 8;
        obj->state = obj_settle_wait_bounce;
    }
}

void obj_begin_settle(struct ActorObj *obj)
{
    s32 h;

    obj->targetX = obj->baseX;
    obj->targetY = obj->baseY;
    obj->targetH = (h = obj->baseH);
    obj->ground = h;
    obj->dhStep = jump_velocity(&obj->dx, &obj->dy, &obj->dh, obj->targetX - obj->baseX, obj->targetY - obj->baseY, h - obj->baseH, 0xC00, 0x200, 0xFD, 0);
    set_slot_value(0x8D, -1);
    sprite_set_action(obj, 3, 0, 0);
    obj->state = obj_settle_step;
}

u32 part_step_obj_wait_free_then_ba20(struct ActorObj *obj)
{
    obj->state = obj_wait_free_then_ba20;
    return 1;
}

void obj_wait_free_then_ba20(struct ActorObj *obj)
{
    if (is_state_busy() == 0)
    {
        obj->state = obj_begin_part_to_owner_x52;
    }
}

void obj_wait_then_follow_owner(struct ActorObj *obj)
{
    obj->timer--;
    if ((s16)obj->timer < 0)
    {
        struct ActorPart *data;
        u8 *flags;

        obj->targetX = (data = obj->owner->data)->targetX + 0x2C00;
        obj->targetY = data->targetY;
        obj->targetH = data->targetH;
        flags = &obj->flags;
        *flags |= 0x20;
        obj->motionSpeed = 0x400;
        obj->motionStart = 0;
        start_motion(obj);
        sprite_set_action(obj, 2, 0, 0);
        obj->state = obj_wait_index_action3_timer;
    }
}

void obj_wait_then_follow_owner_speed(struct ActorObj *obj)
{
    obj->timer--;
    if ((s16)obj->timer < 0)
    {
        struct ActorPart *data;
        u8 *flags;

        obj_list_push_default(obj);
        obj->targetX = (data = obj->owner->data)->targetX;
        obj->targetY = data->targetY;
        obj->targetH = data->targetH;
        flags = &obj->flags;
        *flags |= 0x20;
        obj->motionSpeed = obj->param9C << 8;
        obj->motionStart = 0;
        start_motion(obj);
        sprite_set_action(obj, 4, 0, 0);
        obj->timer = 8;
        obj->state = obj_wait_index_then_move_to_data;
    }
}

void obj_wait_index_then_move_to_data(struct ActorObj *obj)
{
    s32 index = obj->index;

    if (index == 0)
    {
        struct ActorPart *data;
        u8 *flags;

        obj->timer--;
        if ((s16)obj->timer <= 0)
        {
            obj_list_remove(obj);
            obj->targetX = (data = obj->data)->targetX;
            obj->targetY = data->targetY;
            obj->targetH = data->targetH;
            flags = &obj->flags;
            *flags |= 0x20;
            obj->motionSpeed = 0x600;
            obj->motionStart = index;
            start_motion(obj);
            sprite_set_action(obj, 5, 0, 0);
            obj->state = obj_wait_index_release_11a;
        }
    }
}

void obj_drop_until_ground(struct ActorObj *obj)
{
    obj->dx = obj->dx * 0xFD / 0x100;
    obj->dy = obj->dy * 0xFD / 0x100;
    obj->dh += obj->dhStep;
    obj->posX = obj->baseX + obj->dx;
    obj->posY = obj->baseY + obj->dy;
    obj->posH = obj->baseH + obj->dh;
    if (obj->posH <= obj->ground)
    {
        struct ActorPose *pose;
        s32 flags;
        s32 bit;
        s32 cleared;

        obj->posH = obj->ground;
        sprite_set_action(obj, 2, 0, 0);
        pose = obj->pose;
        flags = pose->byte11;
        bit = ((u32)flags << 25) >> 31;
        bit = bit ^ 1;
        bit = bit << 6;
        cleared = -0x41 & flags;
        cleared = cleared | bit;
        pose->byte11 = cleared;
        set_slot_value(0x11A, -1);
        obj->timer = 0x20;
        obj->state = obj_wait_then_move_to_edge;
    }
}

void obj_wait_then_move_to_edge(struct ActorObj *obj)
{
    obj->timer--;
    if ((s16)obj->timer < 0)
    {
        struct ActorPart *data;
        u8 *flags;

        obj->targetX = (data = obj->owner->data)->targetX + (obj->param9C << 8);
        obj->targetY = data->targetY;
        obj->targetH = data->targetH;
        obj->targetX = 0x11000;
        flags = &obj->flags;
        *flags |= 0x20;
        obj->motionSpeed = obj->paramA4 << 8;
        obj->motionStart = 0;
        start_motion(obj);
        obj->timer = 0x14;
        obj->state = obj_wait_then_place_at_owner;
    }
}

void obj_wait_then_place_at_owner(struct ActorObj *obj)
{
    s32 index = obj->index;

    if (index == 0)
    {
        struct ActorPart *data;
        u8 *flags;
        s32 x;
        s32 y;
        s32 h;

        obj->timer--;
        if ((s16)obj->timer < 0)
        {
            obj_list_push_default(obj);
            obj->handler5C = (void (*)(struct ActorObj *))obj_await_part;
            data = obj->owner->data;
            x = data->targetX + (obj->paramA0 << 8);
            obj->baseX = x;
            obj->posX = x;
            y = data->targetY;
            obj->baseY = y;
            obj->posY = y;
            h = data->targetH;
            obj->baseH = h;
            obj->posH = h;
            obj->targetX = data->targetX + 0x400;
            obj->targetY = data->targetY;
            obj->targetH = data->targetH;
            flags = &obj->flags;
            *flags |= 0x20;
            obj->motionSpeed = obj->paramA4 << 8;
            obj->motionStart = index;
            start_motion(obj);
            sprite_set_action(obj, 4, 0, 0);
            obj->state = obj_setup_move_to_data;
        }
    }
}

void obj_setup_move_to_data(struct ActorObj *obj)
{
    s32 index = obj->index;

    if (index == 0)
    {
        struct ActorPart *data;
        u8 *flags;

        obj_list_remove(obj);
        obj->targetX = (data = obj->data)->targetX;
        obj->targetY = data->targetY;
        obj->targetH = data->targetH;
        flags = &obj->flags;
        *flags |= 0x20;
        obj->motionSpeed = 0x400;
        obj->motionStart = index;
        start_motion(obj);
        sprite_set_action(obj, 2, 0, 0);
        obj->state = obj_wait_index_pose_toggle;
    }
}

u32 obj_await_part(struct ActorObj *obj, u32 a, u32 b)
{
    u32 result;

    obj->savedState = obj->state;
    result = lookup_part(obj, a, b);
    if (result == 0)
    {
        obj->state = obj_await_part_done;
    }
    return result;
}

void obj_await_part_done(struct ActorObj *obj)
{
    if (obj->pose->busy == 0)
    {
        sprite_set_action(obj, 2, 0, 0);
        obj->state = obj->savedState;
    }
}

void obj_wait_index_release_11a(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        sound_stop_song(0x11A);
        on_arrive(obj);
    }
}

void obj_wait_index_action3_timer(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        sprite_set_action(obj, 3, 0, 0);
        obj->timer = obj->paramA0;
        obj->state = obj_wait_then_follow_owner_speed;
    }
}

u32 sprite_state_chain_a(struct ActorObj *obj)
{
    obj->state = obj_wait_free_then_begin_drop;
    return 1;
}

u32 sprite_state_chain_b(struct ActorObj *obj)
{
    obj->state = obj_wait_free_then_action2_slot;
    return 1;
}

u32 sprite_state_random_chain_a(struct ActorObj *obj)
{
    u32 r = mt19937_next();

    if (sub_081DE850(1, r) != 0)
    {
        obj->state = obj_wait_free_then_action2_slot;
    }
    else
    {
        obj->state = obj_wait_free_then_begin_drop;
    }
    return 1;
}

void obj_wait_index_pose_toggle(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        struct ActorPose *pose;
        s32 flags;
        s32 bit;
        s32 cleared;

        sound_stop_song(0x11A);
        pose = obj->pose;
        flags = pose->byte11;
        bit = ((u32)flags << 25) >> 31;
        bit = bit ^ 1;
        bit = bit << 6;
        cleared = -0x41 & flags;
        cleared = cleared | bit;
        pose->byte11 = cleared;
        on_arrive(obj);
    }
}

void obj_wait_free_then_begin_drop(struct ActorObj *obj)
{
    if (is_state_busy() == 0)
    {
        obj->state = obj_begin_drop;
    }
}

void obj_wait_free_then_action2_slot(struct ActorObj *obj)
{
    if (is_state_busy() == 0)
    {
        obj->state = obj_action2_slot_then_wait;
    }
}

void obj_begin_drop(struct ActorObj *obj)
{
    s32 h;

    obj->ground = (h = obj->data->targetH);
    obj->dhStep = jump_velocity(&obj->dx, &obj->dy, &obj->dh, 0, 0, h - obj->baseH, 0x1400, 0x4CC, 0xFD, 0);
    set_slot_value(0x30, -1);
    obj->state = obj_drop_until_ground;
}

void obj_action2_slot_then_wait(struct ActorObj *obj)
{
    sprite_set_action(obj, 2, 0, 0);
    set_slot_value(0x11A, -1);
    obj->timer = 0x30;
    obj->state = obj_wait_then_follow_owner;
}
