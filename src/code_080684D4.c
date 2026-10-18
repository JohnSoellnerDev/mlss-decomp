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

ASM_FUNC("asm/code_080684D4/obj_child_arc_land.s", void obj_child_arc_land(struct Actor *obj))

void obj_wait_free_action5(struct Actor *obj)
{
    s32 busy = is_state_busy();
    if (busy == 0)
    {
        u8 *fp;
        sprite_set_action(obj, 5, 0, 0);
        obj->targetX = obj->owner->data->targetX + 0x3800;
        obj->targetY = obj->owner->data->targetY;
        obj->targetH = obj->baseH;
        fp = &obj->flags;
        *fp |= 0x20;
        obj->motionSpeed = 0x200;
        obj->motionStart = busy;
        start_motion(obj);
        obj->timer = obj->param9C;
        set_slot_value(0x81, -1);
        obj->state = obj_wait_index_then_dx;
    }
}

ASM_FUNC("asm/code_080684D4/obj_spawn_child_append.s", void obj_spawn_child_append(struct Actor *obj))

void obj_timer_dx_steps(struct Actor *obj)
{
    if (obj->pose->flags & 8)
    {
        if ((s16)obj->timer == 0)
        {
            goto wait_state;
        }
        sprite_set_action(obj, 6, 0, 0);
    }
    obj->dx--;
    if ((s16)obj->dx > 0)
    {
        return;
    }
    obj->timer--;
    if ((s16)obj->timer > 0)
    {
        sprite_set_action(obj, 7, 0, 0);
        obj->state = obj_spawn_child_append;
        return;
    }
wait_state:
    sprite_set_action(obj, 9, 0, 0);
    obj->state = wait_for_index;
}

u32 obj_contact_callback(struct Actor *obj, u32 a, u32 b)
{
    u32 result = query_contact(obj, a, b);
    if (result != 0)
    {
        return result;
    }
    obj->state = obj_fall_and_remove;
    {
        u32 id = 0x10C6;
        s32 x = base_to_px(obj->baseX);
        s32 y = base_to_px(obj->baseY);
        s32 h = base_to_px(obj->baseH);
        effect_spawn_at(id, x, y, h, obj);
    }
    return 0;
}

u32 obj_link_owner_callback(struct Actor *obj)
{
    partner_link_update(obj, obj->owner, 0xFF);
    obj->state = obj_place_sprite_and_remove;
    return 0;
}

void obj_drift_left_child(struct Actor *obj)
{
    s32 x;
    s32 old = obj->x;

    x = old + -0x133;
    obj->x = x;
    if (x < 0)
    {
        x = old - 0x34;
    }
    old = x >> 8;
    if (old <= -0x20)
    {
        obj_release(obj);
    }
}

void obj_wait_index_then_dx(struct Actor *obj)
{
    if (obj->index == 0)
    {
        sprite_set_action(obj, 6, 0, 0);
        obj->dx = obj->param9C;
        obj->state = obj_dx_timer_then_action7;
    }
}

u32 obj_owner_check_place(struct Actor *obj, struct Actor *other)
{
    u32 result;
    if (other != other->data->ref38)
    {
        result = 1;
    }
    else
    {
        u32 id = 0x10AD;
        s32 x = base_to_px(obj->baseX);
        s32 y = base_to_px(obj->baseY);
        s32 h = base_to_px(obj->baseH);
        effect_spawn_at(id, x, y, h, obj);
        obj->state = obj_remove_callback;
        result = 0;
    }
    return result;
}

void obj_drift_by_target_x(struct Actor *obj)
{
    s32 x = obj->x - obj->targetX;
    obj->x = x;
    if (x < 0)
    {
        x += 0xFF;
    }
    if ((x >> 8) < -0x20)
    {
        obj_release(obj);
    }
}

void obj_idle_when_pose_active(struct Actor *obj)
{
    if (obj->pose->flags & 8)
    {
        obj->state = wait_for_index;
    }
}

u32 obj_init_curve_then_action5(struct Actor *obj)
{
    if (obj->param9C == 0)
    {
        obj->param9C = 2;
        obj->paramA0 = 1;
    }
    obj->state = obj_wait_free_action5;
    return 1;
}

u32 obj_arc_init_wait(struct Actor *obj)
{
    obj->state = obj_wait_free_action2_arc;
    return 1;
}

void obj_fall_and_remove(struct Actor *obj)
{
    s32 xOld = obj->x;
    s32 xNew = xOld - 0x133;
    s32 hOld;
    s32 hNew;
    s32 xChk;
    s32 hChk;

    obj->x = xNew;
    hOld = obj->h;
    hNew = hOld - 0x200;
    obj->h = hNew;
    xChk = xNew;
    if (xChk < 0)
    {
        xChk = xOld - 0x34;
    }
    if ((xChk >> 8) > -0x20)
    {
        hChk = hNew;
        if (hChk < 0)
        {
            hChk = hOld - 0x101;
        }
        if ((hChk >> 8) > 0)
        {
            return;
        }
    }
    obj_release(obj);
}

void obj_place_sprite_and_remove(struct Actor *obj)
{
    u32 id = 0x10BE;
    s32 x = base_to_px(obj->baseX);
    s32 y = base_to_px(obj->baseY);
    s32 h = base_to_px(obj->baseH);
    effect_spawn_at(id, x, y, h, obj);
    obj_release(obj);
}

void obj_dx_timer_then_action7(struct Actor *obj)
{
    if (obj->pose->flags & 8)
    {
        obj->dx--;
        if ((s16)obj->dx <= 0)
        {
            sprite_set_action(obj, 7, 0, 0);
            obj->state = obj_spawn_child_append;
        }
    }
}

void obj_remove_callback(struct Actor *obj)
{
    obj_release(obj);
}

void obj_wait_free_action2_arc(struct Actor *obj)
{
    if (!is_state_busy())
    {
        sprite_set_action(obj, 2, 0, 0);
        obj->state = obj_pose_then_spawn_arc;
    }
}

void obj_pose_then_spawn_arc(struct Actor *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 3, 0, 0);
        obj->state = obj_spawn_arc_child;
    }
}

void obj_spawn_child_412b(struct Actor *obj)
{
    struct Actor *child;
    struct ActorData *part;
    u32 busy;

    busy = is_state_busy();
    if (busy == 0)
    {
        part = obj->data;
        child = obj_spawn(part, 0x412B, 0x100, obj->baseY / 0x100 + 2, obj->baseH / 0x100 + 2,
                          ((u32)obj->pose->unk11 << 25) >> 31, 1);
        obj->child = child;
        ((struct ObjFlags79Bit0 *)child)->bit0 = 1;
        inherit_flags_77(obj, child);
        child->callback68 = on_arrive;
        child->link = obj->link;
        child->owner = obj->owner;
        child->tag = obj->tag;
        child->targetX = obj->baseX + 0x2200;
        child->timer = busy;
        child->state = actor_slide_left_to_target;
        if (obj->paramA8 == 0)
        {
            set_slot_value(0x81, -1);
        }
        else
        {
            set_slot_value(0x81, -1);
        }
        obj->state = obj_wait_child_then_place;
    }
}
