#include "global.h"
#include "ram_routines.h"

struct Pose
{
    u8 filler0[0xC];
    u16 unkC;
    u8 filler0E[0x11 - 0x0E];
    u8 unk11;
    u8 flags;
};

struct ActorData
{
    u8 filler0[0x38];
    struct Actor *ref38;
    u8 filler3C[0xD8 - 0x3C];
    s32 targetX;
    s32 targetY;
    u8 fillerE0[0xEC - 0xE0];
    s16 slot;
};

struct Actor
{
    u8 filler0[0x8];
    struct Pose *pose;
    u8 filler0C[0x10 - 0x0C];
    s32 x;
    s32 y;
    s32 h;
    u8 filler1C[0x28 - 0x1C];
    struct ActorData *data;
    struct Actor *owner;
    struct Actor *child;
    u8 filler34[0x38 - 0x34];
    s32 baseX;
    s32 baseY;
    s32 baseH;
    u8 filler44[0x4C - 0x44];
    void (*state)(struct Actor *);
    u8 filler50[0x58 - 0x50];
    u32 (*cb58)();
    u32 (*cb5C)();
    u32 (*cb60)();
    u8 filler64[0x68 - 0x64];
    void (*callback68)(struct Actor *);
    u32 link;
    u8 filler70[0x74 - 0x70];
    u8 tag;
    u8 filler75[0x77 - 0x75];
    u8 flags77;
    u8 flags78;
    u8 flags;
    s16 motionStart;
    s16 motionSpeed;
    u32 index;
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler90[0x9C - 0x90];
    s32 param9C;
    s32 paramA0;
    s32 paramA4;
    s32 paramA8;
    u16 timer;
    u16 dx;
    s16 dy;
    s16 dh;
};

void sprite_set_action(struct Actor *obj, u32 action, u32 arg, u32 extra);
u32 is_state_busy(void);
void wait_for_index(struct Actor *obj);
void obj_release(struct Actor *obj);
void partner_link_update(struct Actor *obj, struct Actor *other, u32 kind);
void start_motion(struct Actor *obj);
void set_slot_value(u32 id, u32 value);
void sound_stop_song(u32 song);
void sprite_entry_update(struct Pose *pose);
u32 lookup_link_slot(struct Actor *obj, u32 key);
u32 mt19937_next(void);
void obj_spawn_part_start(struct Actor *obj);
void obj_list_push_default(struct Actor *obj);
struct Actor *obj_spawn(struct ActorData *part, u32 type, s32 x, s32 y, s32 h, u32 a, u32 b);
void on_arrive(struct Actor *obj);
void actor_slide_left_to_target(struct Actor *obj);
void obj_wait_child_then_place(struct Actor *obj);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 h, struct Actor *owner);
u32 query_contact(struct Actor *obj, u32 a, u32 b);

struct ObjFlags79Bit0
{
    u8 filler0[0x79];
    u8 bit0 : 1;
    u8 rest : 7;
};

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

static inline void inherit_flags_77(struct Actor *parent, struct Actor *child)
{
    ((struct ObjFlags77 *)child)->bit6 = ((struct ObjFlags77 *)parent)->bit6;
    ((struct ObjFlags77 *)child)->bit5 = ((struct ObjFlags77 *)parent)->bit5;
}

static inline s32 base_to_px(s32 v)
{
    if (v < 0)
    {
        v += 0xFF;
    }
    return v >> 8;
}

void part_arc_move(struct Actor *obj);
u32 obj_link_place_remove(struct Actor *obj, struct Actor *other);
void obj_start_countdown_action9(struct Actor *obj);
u32 unknown_part_steps_08067500(struct Actor *obj);
u32 sub_08067514(struct Actor *obj);
void obj_countdown_step(struct Actor *obj);
void obj_wait_free_action2_drop(struct Actor *obj);
void obj_action11_then_idle(struct Actor *obj);
void obj_begin_drop_motion(struct Actor *obj);
void obj_wait_index_then_action(struct Actor *obj);
void obj_timer_random_jump(struct Actor *obj);
void obj_reset_to_spawn(struct Actor *obj);
void obj_aim_at_owner_then_wait(struct Actor *obj);
void obj_wait_slot_then_follow(struct Actor *obj);
void obj_follow_owner_step(struct Actor *obj);
void part_arc_land_spawn(struct Actor *obj);
void obj_drift_step(struct Actor *obj);
u32 actor_arc_flight_callback(struct Actor *obj);
u32 obj_find_link_or_wait(struct Actor *obj, u32 key);
void obj_timer_remove(struct Actor *obj);
void obj_child_lost_recover(struct Actor *obj);
void actor_landing_idle(struct Actor *obj);
void obj_restart_follow(struct Actor *obj);
u32 obj_owner_slot_mode2(struct Actor *obj);
u32 obj_owner_slot_mode1(struct Actor *obj);
u32 obj_owner_slot_mode0(struct Actor *obj);
void obj_count_then_wait(struct Actor *obj);
void obj_wait_free_action2_follow(struct Actor *obj);
void obj_fall_bounce_step(struct Actor *obj);
void obj_spawn_arc_child(struct Actor *obj);
void obj_child_arc_land(struct Actor *obj);
void obj_wait_free_action5(struct Actor *obj);
void obj_spawn_child_append(struct Actor *obj);
void obj_timer_dx_steps(struct Actor *obj);
u32 obj_contact_callback(struct Actor *obj, u32 a, u32 b);
u32 obj_link_owner_callback(struct Actor *obj);
void obj_drift_left_child(struct Actor *obj);
void obj_wait_index_then_dx(struct Actor *obj);
u32 obj_owner_check_place(struct Actor *obj, struct Actor *other);
void obj_drift_by_target_x(struct Actor *obj);
void obj_idle_when_pose_active(struct Actor *obj);
u32 obj_init_curve_then_action5(struct Actor *obj);
u32 unknown_part_steps_080689cc(struct Actor *obj);
void obj_fall_and_remove(struct Actor *obj);
void obj_place_sprite_and_remove(struct Actor *obj);
void obj_dx_timer_then_action7(struct Actor *obj);
void obj_remove_callback(struct Actor *obj);
void obj_wait_free_action2_arc(struct Actor *obj);
void obj_pose_then_spawn_arc(struct Actor *obj);
void obj_spawn_child_412b(struct Actor *obj);

void obj_drift_step(struct Actor *obj)
{
    obj->x = (obj->paramA4 * (s16)obj->timer) / obj->paramA0 + ((s16)obj->dx << 8);
    obj->y = (obj->paramA8 * (s16)obj->timer) / obj->paramA0 + ((s16)obj->dy << 8);
    {
        s32 offset = (s16)obj->timer - obj->paramA0 / 2;
        s32 sq = offset * offset;
        s32 curve = obj->param9C * sq;
        s32 dhTerm = ((s16)obj->dh << 8) + 0x3000;
        obj->h = curve + dhTerm;
    }
    obj->timer--;
    if ((s16)obj->timer < 0)
    {
        sprite_set_action(obj, 7, 0, 0);
        obj->state = wait_for_index;
    }
}

u32 actor_arc_flight_callback(struct Actor *obj)
{
    return -1;
}

u32 obj_find_link_or_wait(struct Actor *obj, u32 key)
{
    u32 found = lookup_link_slot(obj, key);
    u32 result;
    if (found == 0)
    {
        sprite_set_action(obj, 1, 0, 0);
        obj->index = found;
        obj->targetX = 0x14;
        obj->state = obj_count_then_wait;
        result = 0;
    }
    else
    {
        result = found;
    }
    return result;
}

void obj_timer_remove(struct Actor *obj)
{
    obj->timer--;
    if ((s16)obj->timer <= 0)
    {
        obj_release(obj);
    }
}

void obj_child_lost_recover(struct Actor *obj)
{
    if (obj->child == 0)
    {
        sound_stop_song(0x83);
        obj->owner->state = 0;
        sprite_entry_update(obj->pose);
        sprite_entry_update(obj->owner->pose);
        sprite_set_action(obj, 6, 0, 0);
        obj->timer--;
        partner_link_update(obj, obj->owner, 0xFF);
        obj->state = obj_drift_step;
    }
}

void actor_landing_idle(struct Actor *obj)
{
}

void obj_restart_follow(struct Actor *obj)
{
    u32 index = obj->index;
    if (index == 0)
    {
        u8 *fp;
        obj->targetX = obj->owner->data->targetX + 0x1600;
        obj->targetY = obj->owner->data->targetY;
        obj->targetH = obj->baseH;
        fp = &obj->flags;
        *fp |= 0x20;
        obj->motionSpeed = 0x200;
        obj->motionStart = index;
        start_motion(obj);
        obj->state = obj_follow_owner_step;
    }
}

u32 obj_owner_slot_mode2(struct Actor *obj)
{
    obj->paramA0 = 2;
    if (obj->owner->data->slot == -1)
    {
        obj->param9C = 0;
    }
    else
    {
        obj->param9C = 1;
    }
    obj->state = obj_wait_free_action2_follow;
    return 1;
}

u32 obj_owner_slot_mode1(struct Actor *obj)
{
    obj->paramA0 = 1;
    if (obj->owner->data->slot == -1)
    {
        obj->param9C = 0;
    }
    else
    {
        obj->param9C = 1;
    }
    obj->state = obj_wait_free_action2_follow;
    return 1;
}

u32 obj_owner_slot_mode0(struct Actor *obj)
{
    obj->paramA0 = 0;
    if (obj->owner->data->slot == -1)
    {
        obj->param9C = 0;
    }
    else
    {
        obj->param9C = 1;
    }
    obj->state = obj_wait_free_action2_follow;
    return 1;
}

void obj_count_then_wait(struct Actor *obj)
{
    s32 remaining = obj->targetX - 1;
    obj->targetX = remaining;
    if (remaining <= 0)
    {
        obj->targetH = 0x300;
        obj->state = obj_fall_bounce_step;
    }
}

void obj_wait_free_action2_follow(struct Actor *obj)
{
    if (!is_state_busy())
    {
        sprite_set_action(obj, 2, 0, 0);
        obj->state = obj_aim_at_owner_then_wait;
    }
}

void obj_fall_bounce_step(struct Actor *obj)
{
    obj->x += 0x200;
    obj->h -= obj->targetH;
    obj->targetH += 0x66;
    if (obj->h <= 0)
    {
        obj->h = 0;
        obj->state = wait_for_index;
    }
}

void obj_spawn_arc_child(struct Actor *obj)
{
    struct Actor *child;
    s32 ddx;
    s32 ddy;
    s32 len;
    s32 q;
    s32 half;
    s32 d;

    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 4, 0, 0);
        effect_spawn_at(0x1092, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        child = obj_spawn(obj->data, 0x4106, obj->baseX / 0x100 - 0x12, obj->baseY / 0x100,
                          obj->baseH / 0x100 + 0x10, ((u32)obj->pose->unk11 << 25) >> 31, 1);
        obj->child = child;
        ((struct Flags78Bits *)&child->flags78)->kind = 1;
        inherit_flags_77(obj, child);
        child->callback68 = on_arrive;
        child->link = obj->link;
        child->owner = obj->owner;
        obj_list_push_default(child);
        child->cb60 = obj_owner_check_place;
        child->tag = obj->tag;
        child->timer = 0;
        child->targetX = obj->owner->data->targetX + 0x3600;
        child->targetY = obj->owner->data->targetY;
        child->targetH = child->baseH;
        child->dx = child->baseX / 0x100;
        child->dy = child->baseY / 0x100;
        child->dh = child->baseH / 0x100;
        ddx = child->targetX - ((s16)child->dx << 8);
        ddy = child->targetY - (child->dy << 8);
        len = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(ddx * ddx + ddy * ddy);
        q = len / 0x200;
        half = q / 2;
        d = 0x3000 / (half * half);
        if (d == 1)
        {
            d = 2;
        }
        child->param9C = -d;
        child->paramA0 = q;
        child->paramA4 = ddx;
        child->paramA8 = ddy;
        child->timer = 0;
        child->state = obj_child_arc_land;
        child->targetH = obj->param9C;
        set_slot_value(0x57, -1);
        obj->state = obj_idle_when_pose_active;
    }
}
