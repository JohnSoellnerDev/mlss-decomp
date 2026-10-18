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
s32 rand_below(s32 range, u32 seed);
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

void obj_timer_expired_set_motion(struct ActorObj *obj)
{
    s32 t;
    s32 f;

    if (obj->pose->flags & 8)
    {
        t = obj->timer;
        t--;
        obj->timer = t;
        if ((s16)t <= 0)
        {
            sprite_set_action(obj, 6, 0, 0);
            obj->targetX = obj->owner->part->targetX;
            obj->targetY = obj->owner->part->targetY;
            obj->targetH = (obj->dh - 0x10) << 8;
            obj->dh = obj->posH / 0x100;
            f = obj->flags;
            f |= 0x20;
            obj->flags = f;
            obj->motionSpeed = 0x300;
            obj->motionStart = 0;
            start_motion(obj);
            obj->state = obj_index_zero_set_motion;
            set_slot_value(0x11D, -1);
        }
    }
}

void obj_index_zero_set_motion(struct ActorObj *obj)
{
    s32 idx = obj->index;
    s32 f;

    if (idx == 0)
    {
        sprite_set_action(obj, 7, 0, 0);
        obj->targetX = obj->owner->part->targetX;
        obj->targetY = obj->owner->part->targetY;
        obj->targetH = obj->dh << 8;
        f = obj->flags;
        f |= 0x20;
        obj->flags = f;
        obj->motionSpeed = 0x100;
        obj->motionStart = idx;
        start_motion(obj);
        obj->state = obj_index_zero_set_motion_b;
    }
}

void obj_poll_anim_buffer(struct ActorObj *obj)
{
    u8 running = poll_spline_path(gUnk_03000E18, &gUnk_03000E1C, &gUnk_03000E20);

    if (running == 1)
    {
        obj->posX = gUnk_03000E1C;
        obj->posY = gUnk_03000E20;
    }
    else
    {
        obj->posX = gUnk_03000E1C;
        obj->posY = gUnk_03000E20;
        heap_free_b((void *)gUnk_03000E18);
        obj->state = obj_index_zero_action5_timer2;
    }
}

void obj_timer_down_action7_then_idle(struct ActorObj *obj)
{
    s32 t = obj->timer;

    t--;
    obj->timer = t;
    if ((s16)t <= 0)
    {
        sprite_set_action(obj, 7, 0, 0);
        obj->state = wait_for_index;
    }
}

s32 unknown_code_08061424(struct ActorObj *obj)
{
    obj->paramA8 = 2;
    obj->state = obj_wait_ready_setup_offsets;
    return 1;
}

s32 sub_08061438(struct ActorObj *obj)
{
    obj->paramA8 = 1;
    obj->state = obj_wait_ready_setup_offsets;
    return 1;
}

s32 obj_choose_random_param_a8(struct ActorObj *obj)
{
    obj->paramA8 = rand_below(3, mt19937_next());
    obj->state = obj_wait_ready_setup_offsets;
    return 1;
}

s32 unknown_part_steps_08061474(struct ActorObj *obj)
{
    obj->state = obj_wait_ready_setup_motion;
    return 1;
}

void obj_index_zero_set_motion_b(struct ActorObj *obj)
{
    s32 idx = obj->index;
    s32 f;

    if (idx == 0)
    {
        sprite_set_action(obj, 8, 0, 0);
        obj->targetX = obj->dx << 8;
        obj->targetY = obj->dy << 8;
        obj->targetH = obj->dh << 8;
        f = obj->flags;
        f |= 0x20;
        obj->flags = f;
        obj->motionSpeed = 0x200;
        obj->motionStart = idx;
        start_motion(obj);
        obj->state = obj_index_zero_action9_then_idle;
    }
}

void obj_index_zero_action5_timer2(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        sprite_set_action(obj, 5, 0, 0);
        obj->timer = 2;
        obj->state = obj_timer_expired_set_motion;
    }
}

void obj_wait_ready_setup_offsets(struct ActorObj *obj)
{
    if (!is_state_busy())
    {
        obj_list_push_default(obj);
        obj->handler60 = obj_lookup_slot_free_heap_then_wait;
        obj->dh = obj->posH / 0x100;
        obj->dx = obj->posX / 0x100;
        obj->dy = obj->posY / 0x100;
        sprite_set_action(obj, 2, 0, 0);
        obj->state = obj_pose_active_action3_place;
    }
}

s32 obj_lookup_slot_free_heap_then_wait(struct ActorObj *obj)
{
    u32 found;
    struct ActorObj *owner;

    found = lookup_slot(obj);
    if (found != 0)
    {
        return found;
    }
    heap_free_b((void *)gUnk_03000E18);
    owner = obj->owner;
    resolve_contact_action(owner, obj, obj->posX / 0x100, obj->posH / 0x100,
                           (owner->part->slot != -1) ? 0x13 : 0x12);
    sprite_set_action(obj, 1, 0, 0);
    obj->timer = 0x20;
    obj->state = obj_timer_down_action4_then_idle;
    return 0;
}

void obj_index_zero_action9_then_idle(struct ActorObj *obj)
{
    if (obj->index == 0)
    {
        sprite_set_action(obj, 9, 0, 0);
        obj->state = wait_for_index;
    }
}

void obj_pose_active_action3_place(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 3, 0, 0);
        effect_spawn_at(0x124C, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        obj->targetH = obj->baseH;
        obj->state = actor_pose_steps;
    }
}

void obj_timer_down_action4_then_idle(struct ActorObj *obj)
{
    s32 t = obj->timer;

    t--;
    obj->timer = t;
    if ((s16)t <= 0)
    {
        sprite_set_action(obj, 4, 0, 0);
        obj->state = wait_for_index;
    }
}

void actor_pose_steps(struct ActorObj *obj)
{
    obj->timer = 0;
    obj->state = obj_timer_end_build_anim_record;
}
