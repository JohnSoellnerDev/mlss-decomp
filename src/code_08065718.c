#include "global.h"

struct ActorPose
{
    u8 filler0[0x0C];
    u16 unkC;
    u8 filler0E[0x11 - 0x0E];
    u8 unk11;
    u8 flags;
};

struct Flags77Bits
{
    u8 low5 : 5;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 high : 1;
};

struct Flags79Bits
{
    u8 low5 : 5;
    u8 bit5 : 1;
    u8 high : 2;
};

struct PoseBits
{
    u8 low : 1;
    u8 mode : 2;
    u8 high : 5;
};

struct ActorPart
{
    u8 filler0[0xB4];
    u16 unkB4;
    u8 filler1[0xD8 - 0xB6];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler2[0xEC - 0xE4];
    s16 slot;
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
    void (*handler58)(struct ActorObj *);
    void (*handler5C)(struct ActorObj *);
    void (*handler60)(struct ActorObj *);
    u8 filler64[0x68 - 0x64];
    void (*handler68)(struct ActorObj *);
    u32 unk6C;
    u8 filler70[0x74 - 0x70];
    u8 tag;
    u8 counter;
    u8 filler76[0x77 - 0x76];
    u8 flags77;
    u8 flags78;
    u8 flags79;
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

void sprite_set_action(struct ActorObj *obj, u32 action, u32 a2, u32 a3);
void set_slot_value(u32 id, s32 value);
void obj_release(struct ActorObj *obj);
void effect_spawn_at(u32 type, s32 x, s32 y, s32 h, struct ActorObj *owner);
u32 effect_from_table_obj(struct ActorObj *obj, u32 id);
u32 is_state_busy(struct ActorObj *obj);
u32 lookup_slot(struct ActorObj *obj);
u32 lookup_part(struct ActorObj *obj);
void start_motion(struct ActorObj *obj);
void sound_stop_song(u32 song);
void wait_for_index(struct ActorObj *obj);
void obj_query_part_action3(struct ActorObj *obj);
void obj_pose_active_effect_c(struct ActorObj *obj);
extern u32 gUnk_03000E3C;
extern u32 gUnk_03000E18;
extern s32 gUnk_03000E60;
extern s32 gUnk_03000E64;
u32 poll_spline_path(u32 buf, s32 *x, s32 *h);
void offset_three_words_camera(s32 *a, s32 *b, s32 *c, u32 d);
void heap_free_b(u32 ptr);
void set_targets_from_record(struct ActorObj *obj);
void obj_motion_from_owner(struct ActorObj *obj);
void obj_place_sprite_finish_b(struct ActorObj *obj);
u32 query_contact(struct ActorObj *obj);

void obj_timer_effect_b(struct ActorObj *obj);
void obj_action7_wait(struct ActorObj *obj);
void obj_wait_ready_action3(struct ActorObj *obj);
u32 obj_query_effect(struct ActorObj *obj);
void obj_effect_action_c(struct ActorObj *obj);
void obj_drift_left(struct ActorObj *obj);
void obj_countdown_then_action1(struct ActorObj *obj);
void obj_pose_active_action0_ext(struct ActorObj *obj);
void parts_obj_pose_active_timer_action_d(struct ActorObj *obj);
void obj_spawn_part_sprite(struct ActorObj *obj);
void obj_slide_step_then_cull(struct ActorObj *obj);
void parts_obj_spawn_child_launch(struct ActorObj *obj);
void obj_update_position_timer(struct ActorObj *obj);
u32 obj_slot_check_action1(struct ActorObj *obj);
void obj_pose_active_stop(struct ActorObj *obj);
u32 obj_query_part_then_state(struct ActorObj *obj);
u32 obj_lookup_slot_then_state(struct ActorObj *obj);
void obj_pose_active_action0_next(struct ActorObj *obj);
u32 parts_obj_start_wait_place_sprite(struct ActorObj *obj);
u32 obj_start_choose_zero(struct ActorObj *obj);
u32 obj_start_restart_action6(struct ActorObj *obj);
u32 obj_start_choose_one(struct ActorObj *obj);
u32 obj_start_action3_zero(struct ActorObj *obj);
void obj_move_xh_then_cull(struct ActorObj *obj);
void obj_restart_action6(struct ActorObj *obj);
void parts_obj_wait_ready_place_sprite(struct ActorObj *obj);
void parts_obj_fall_step(struct ActorObj *obj);
void obj_place_sprite_and_finish(struct ActorObj *obj);
void actor_parts_steps(struct ActorObj *obj);
void obj_action3_restart_spawn(struct ActorObj *obj);
void obj_wait_ready_choose_state(struct ActorObj *obj);
void obj_timer_up_then_idle(struct ActorObj *obj);
void obj_pose_active_action3(struct ActorObj *obj);
void parts_obj_pose_active_clear_state(struct ActorObj *obj);
void obj_active_place_then_wait(struct ActorObj *obj);
void parts_obj_spawn_child_b(struct ActorObj *obj);
void unknown_code_08065044(struct ActorObj *obj);
void obj_poll_buffer_or_free(struct ActorObj *obj);
void obj_timer_start_motion(struct ActorObj *obj);
void obj_wait_then_slide_and_finish(struct ActorObj *obj);
void obj_move_step_c(struct ActorObj *obj);
void obj_move_step_d(struct ActorObj *obj);
void obj_glide_step_finish(struct ActorObj *obj);
void obj_wait_flag_then_cull(struct ActorObj *obj);
void obj_wait_then_state_5619(struct ActorObj *obj);
void obj_pose_active_action5_or_9(struct ActorObj *obj);
u32 obj_check_flag_a0_zero(struct ActorObj *obj);
u32 obj_check_flag_a0_one(struct ActorObj *obj);
void parts_obj_pose_active_idle(struct ActorObj *obj);
void parts_obj_wait_ready_action2(struct ActorObj *obj);
void obj_pose_active_place_sprite_a(struct ActorObj *obj);
void obj_timer_spawn_child_a(struct ActorObj *obj);
void obj_timer_spawn_child_b(struct ActorObj *obj);
void obj_timer_spawn_effect_place(struct ActorObj *obj);
u32 obj_lookup_place_cull(struct ActorObj *obj);
void obj_pose_active_finish(struct ActorObj *obj);
void obj_pose_active_effect_b(struct ActorObj *obj);
u32 obj_lookup_slot_then_state_bc0(struct ActorObj *obj);
void obj_move_place_cull(struct ActorObj *obj);
void obj_no_sprite_idle(struct ActorObj *obj);

struct ActorObj *obj_spawn(struct ActorPart *part, u32 kind, s32 x, s32 y, s32 h, u32 a, u32 b);
void obj_list_push_default(struct ActorObj *obj);
void on_arrive(struct ActorObj *obj);
void obj_no_sprite_idle_b(struct ActorObj *obj);

struct PoseBits6
{
    u8 low : 6;
    u8 bit6 : 1;
    u8 high : 1;
} __attribute__((packed));

struct ObjFlags77
{
    u8 filler0[0x77];
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

struct Flags78Bits
{
    u8 low5 : 5;
    u8 kind : 3;
};

// The spawned child takes over the kind and two flag bits of its parent.
static inline void obj_child_flags(struct ActorObj *obj, struct ActorObj *child, u32 kind)
{
    ((struct Flags78Bits *)&child->flags78)->kind = kind >> 5;
    ((struct ObjFlags77 *)child)->bit6 = ((struct ObjFlags77 *)obj)->bit6;
    ((struct ObjFlags77 *)child)->bit5 = ((struct ObjFlags77 *)obj)->bit5;
}

void obj_timer_spawn_child_a(struct ActorObj *obj)
{
    struct ActorObj *child;

    if ((s16)--obj->timer > 0)
        return;
    sprite_set_action(obj, 4, 0, 0);
    child = obj_spawn(obj->part, 0x40DC, obj->owner->part->targetX / 256 + 0x30, obj->owner->part->targetY / 256,
                      obj->baseH / 256 + 0x48, ((struct PoseBits6 *)&obj->pose->unk11)->bit6, 1);
    obj->child = child;
    obj_child_flags(obj, child, 0x40);
    child->handler68 = on_arrive;
    child->unk6C = obj->unk6C;
    child->owner = obj->owner;
    obj_list_push_default(child);
    child->handler60 = (void (*)(struct ActorObj *))obj_lookup_slot_then_state_bc0;
    child->targetX = child->baseX;
    child->targetH = child->baseH;
    child->state = obj_move_place_cull;
    set_slot_value(0x10F, -1);
    obj->state = obj_no_sprite_idle_b;
}

void obj_timer_spawn_child_b(struct ActorObj *obj)
{
    struct ActorObj *child;

    if ((s16)--obj->timer > 0)
        return;
    child = obj_spawn(obj->part, 0x40DD, obj->owner->part->targetX / 256, obj->owner->part->targetY / 256 + 2,
                      obj->baseH / 256, ((struct PoseBits6 *)&obj->pose->unk11)->bit6, 1);
    obj->child = child;
    obj_child_flags(obj, child, 0x20);
    child->handler68 = on_arrive;
    child->unk6C = obj->unk6C;
    child->owner = obj->owner;
    obj_list_push_default(child);
    child->timer = 8;
    child->state = obj_timer_spawn_effect_place;
    set_slot_value(0x8C, -1);
    obj->state = obj_no_sprite_idle;
}

void obj_timer_spawn_effect_place(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        s32 t = obj->timer;

        t--;
        obj->timer = t;
        if ((t << 16) <= 0)
        {
            sprite_set_action(obj, 1, 0, 0);
            gUnk_03000E3C = effect_from_table_obj(obj, 0x14F2);
            obj->handler5C = (void (*)(struct ActorObj *))obj_lookup_place_cull;
            effect_spawn_at(0xF16, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
            obj->state = obj_pose_active_effect_b;
        }
    }
}

u32 obj_lookup_place_cull(struct ActorObj *obj)
{
    u32 result = query_contact(obj);

    if (result == 0)
    {
        effect_spawn_at(0xF24, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        obj_release(obj);
        obj->state = obj_pose_active_finish;
    }
    return result;
}

void obj_pose_active_finish(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        obj_release(obj);
    }
}

void obj_pose_active_effect_b(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 2, 0, 0);
        gUnk_03000E3C = effect_from_table_obj(obj, 0x1509);
        obj->handler5C = obj_query_part_action3;
        obj->state = obj_pose_active_effect_c;
    }
}

u32 obj_lookup_slot_then_state_bc0(struct ActorObj *obj)
{
    u32 result = lookup_slot(obj);

    if (result != 0)
    {
        return result;
    }
    obj->state = obj_place_sprite_finish_b;
    return 0;
}

void obj_move_place_cull(struct ActorObj *obj)
{
    s32 rel;
    s32 h;
    s32 sq;
    s32 sev;

    obj->pose->unkC += 0x200;
    obj->posX -= 0xE6;
    rel = obj->posX - obj->targetX;
    if (rel < 0)
        rel += 0xFF;
    rel >>= 8;
    sq = rel * rel;
    sev = sq * 17;
    h = obj->targetH - sev;
    obj->posH = h;
    if (h <= 2047)
    {
        effect_spawn_at(0xF08, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        obj_release(obj);
    }
}

void obj_no_sprite_idle(struct ActorObj *obj)
{
    if (obj->child == 0)
    {
        sprite_set_action(obj, 0, 0, 0);
        obj->state = wait_for_index;
    }
}
