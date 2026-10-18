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
u32 obj_drop_init_flag1(struct Actor *obj);
u32 obj_drop_init_flag0(struct Actor *obj);
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
u32 obj_arc_init_wait(struct Actor *obj);
void obj_fall_and_remove(struct Actor *obj);
void obj_place_sprite_and_remove(struct Actor *obj);
void obj_dx_timer_then_action7(struct Actor *obj);
void obj_remove_callback(struct Actor *obj);
void obj_wait_free_action2_arc(struct Actor *obj);
void obj_pose_then_spawn_arc(struct Actor *obj);
void obj_spawn_child_412b(struct Actor *obj);

ASM_FUNC("asm/code_08067338/part_arc_move.s", void part_arc_move(struct Actor *obj))

u32 obj_link_place_remove(struct Actor *obj, struct Actor *other)
{
    u32 id = 0xD0C;
    s32 x;
    s32 y;
    s32 h;
    partner_link_update(obj, other, 0xFF);
    x = base_to_px(obj->baseX);
    y = base_to_px(obj->baseY);
    h = base_to_px(obj->baseH);
    effect_spawn_at(id, x, y, h, obj);
    obj_release(obj);
    return 0;
}

void obj_start_countdown_action9(struct Actor *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 9, 0, 0);
        obj->timer = 2;
        obj->state = obj_countdown_step;
    }
}

u32 obj_drop_init_flag1(struct Actor *obj)
{
    obj->param9C = 1;
    obj->state = obj_wait_free_action2_drop;
    return 1;
}

u32 obj_drop_init_flag0(struct Actor *obj)
{
    obj->param9C = 0;
    obj->state = obj_wait_free_action2_drop;
    return 1;
}

void obj_countdown_step(struct Actor *obj)
{
    if (obj->pose->flags & 8)
    {
        obj->timer--;
        if ((s16)obj->timer <= 0)
        {
            obj->paramA0++;
            obj->paramA4--;
            if (obj->paramA4 > 0)
            {
                sprite_set_action(obj, 7, 0, 0);
                obj->state = obj_spawn_part_start;
            }
            else
            {
                sprite_set_action(obj, 0xA, 0, 0);
                obj->state = obj_action11_then_idle;
            }
        }
    }
}

void obj_wait_free_action2_drop(struct Actor *obj)
{
    if (!is_state_busy())
    {
        sprite_set_action(obj, 2, 0, 0);
        obj->state = obj_begin_drop_motion;
    }
}

void obj_action11_then_idle(struct Actor *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0xB, 0, 0);
        obj->state = wait_for_index;
    }
}

void obj_begin_drop_motion(struct Actor *obj)
{
    if (obj->pose->flags & 8)
    {
        u8 *fp;
        obj->targetX = 0x7800;
        obj->targetY = 0x6800;
        obj->targetH = obj->baseH;
        fp = &obj->flags;
        *fp |= 0x20;
        obj->motionSpeed = 0x180;
        obj->motionStart = 0;
        start_motion(obj);
        sprite_set_action(obj, 3, 0, 0);
        obj->state = obj_wait_index_then_action;
    }
}

void obj_wait_index_then_action(struct Actor *obj)
{
    if (obj->index == 0)
    {
        if (obj->param9C == 0)
        {
            sprite_set_action(obj, 4, 0, 0);
        }
        else
        {
            sprite_set_action(obj, 5, 0, 0);
        }
        obj->timer = 4;
        obj->state = obj_timer_random_jump;
        set_slot_value(0xD5, -1);
    }
}

void obj_timer_random_jump(struct Actor *obj)
{
    if (obj->pose->flags & 8)
    {
        obj->timer--;
        if ((s16)obj->timer <= 0)
        {
            sprite_set_action(obj, 6, 0, 0);
            obj->paramA0 = obj->param9C;
            obj->paramA4 = (mt19937_next() & 1) + 3;
            obj->state = obj_reset_to_spawn;
        }
    }
}

void obj_reset_to_spawn(struct Actor *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 7, 0, 0);
        obj->state = obj_spawn_part_start;
    }
}

void obj_aim_at_owner_then_wait(struct Actor *obj)
{
    if (obj->pose->flags & 8)
    {
        u8 *fp;
        obj->targetX = obj->owner->data->targetX + 0x3800;
        obj->targetY = obj->owner->data->targetY;
        obj->targetH = obj->baseH;
        fp = &obj->flags;
        *fp |= 0x20;
        obj->motionSpeed = 0x200;
        obj->motionStart = 0;
        start_motion(obj);
        sprite_set_action(obj, 3, 0, 0);
        obj->state = obj_wait_slot_then_follow;
        set_slot_value(0x11C, -1);
    }
}

void obj_wait_slot_then_follow(struct Actor *obj)
{
    u32 index = obj->index;
    if (index == 0)
    {
        u8 *fp;
        sound_stop_song(0x11C);
        sprite_set_action(obj, 4, 0, 0);
        obj->targetX = obj->owner->data->targetX + 0x1600;
        obj->targetY = obj->owner->data->targetY;
        obj->targetH = obj->baseH;
        fp = &obj->flags;
        *fp |= 0x20;
        obj->motionSpeed = 0x200;
        obj->motionStart = index;
        start_motion(obj);
        obj_list_push_default(obj);
        obj->cb58 = actor_arc_flight_callback;
        obj->cb60 = obj_find_link_or_wait;
        obj->timer = (mt19937_next() & 3) + 1;
        obj->state = obj_follow_owner_step;
    }
}

ASM_FUNC("asm/code_08067338/obj_follow_owner_step.s", void obj_follow_owner_step(struct Actor *obj))

ASM_FUNC("asm/code_08067338/part_arc_land_spawn.s", void part_arc_land_spawn(struct Actor *obj))
