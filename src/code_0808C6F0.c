#include "global.h"
#include "ram_routines.h"

struct Pose
{
    u8 filler0[0x11];
    u8 dir;
    u8 flags;
    u8 filler13[0x20 - 0x13];
    u8 result;
    u8 filler21[0x23 - 0x21];
    u8 count;
};

struct PoseFlagBits
{
    u8 unk0_0 : 1;
    u8 mode : 2;
    u8 unk0_3 : 1;
    u8 hold : 1;
    u8 unk0_5 : 3;
};

struct PoseDirBits
{
    u8 unk0_0 : 6;
    u8 turn : 1;
    u8 unk0_7 : 1;
};

struct DataLockBits
{
    u8 unk0 : 2;
    u8 locked : 1;
    u8 unk3 : 5;
};

struct Data
{
    u8 filler0[0x38];
    s32 unk38;
    u8 filler3C[0xB4 - 0x3C];
    u16 unkB4;
    u8 fillerB6[0xEC - 0xB6];
    s16 slot;
    u8 fillerEE[0x111 - 0xEE];
    u8 lock;
    u8 filler112[0x114 - 0x112];
    u32 slotValue;
    u8 filler118[0x11E - 0x118];
    u16 unk11E;
    u8 filler120[0x12D - 0x120];
    u8 kind;
};

struct Actor
{
    u8 filler0[0x08];
    struct Pose *pose;
    u8 filler0C[0x10 - 0x0C];
    s32 x;
    s32 y;
    s32 h;
    u8 filler1C[0x28 - 0x1C];
    struct Data *data;
    struct Actor *owner;
    struct Actor *child;
    u8 filler34[0x38 - 0x34];
    s32 baseX;
    s32 baseY;
    s32 baseH;
    u8 unk44;
    u8 unk45;
    u8 unk46;
    u8 unk47;
    u8 unk48;
    u8 unk49;
    u8 filler4A[0x4C - 0x4A];
    void (*state)(struct Actor *);
    void (*next)(struct Actor *);
    void (*resume)(struct Actor *);
    u8 filler58[0x68 - 0x58];
    void (*callback)(struct Actor *);
    const u8 *anim;
    u8 filler70[0x76 - 0x70];
    u8 flags76;
    u8 flags77;
    u8 filler78;
    u8 flags79;
    u16 unk7A;
    u16 unk7C;
    u8 filler7E[2];
    s32 index;
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler90[0x98 - 0x90];
    s32 mode;
    s32 unk9C;
    s32 unkA0;
    s32 unkA4;
    s32 mask;
    u16 timer;
    s16 dx;
    s16 dy;
    s16 dh;
};

typedef s32 (*MathFunc)(s32, s32);

extern const u8 gAnim284[];
struct KeyState
{
    u8 filler0[0x3C];
    u16 held;
};

extern struct KeyState gUnk_0300034C;
extern u8 *gUnk_03000FD8;
extern u8 *gUnk_03000FF4;

void sprite_set_action(struct Actor *obj, u32 action, u32 part, u32 extra);
void set_slot_value(u32 id, s32 value);
void sound_stop_song(u32 id);
void set_cur_flags_both(u32 value);
void obj_list_push_default(struct Actor *obj);
void obj_list_remove(struct Actor *obj);
void motion_timed(struct Actor *obj, u32 kind, u32 value);
void start_motion(struct Actor *obj);
s32 partner_link_update(struct Actor *obj, struct Actor *partner, u32 mode);
void effect_spawn_at(u32 id, s32 x, s32 y, s32 h, struct Actor *owner);
struct Actor *obj_spawn(struct Data *data, u32 kind, s32 x, s32 y, s32 h, s32 a, s32 b);
void obj_release(struct Actor *obj);
void on_arrive(struct Actor *obj);
void end_action(struct Actor *obj);
s32 list_slot_start(struct Actor *obj);
s32 charge_release_setup_slot_state(struct Actor *obj);
s32 hold_decay_setup_slot_state(struct Actor *obj);
s32 random_jump_setup_slot_spawn(struct Actor *obj);
s32 jump_orbit_setup_slot_spawn(struct Actor *obj);
s32 dispatch_slot_random_jump_a(struct Actor *obj);
s32 dispatch_slot_random_jump_b(struct Actor *obj);
s32 dispatch_slot_random_pause_jump(struct Actor *obj);
s32 charge_release_dispatch_slot_random_jump(struct Actor *obj);
s32 hold_decay_dispatch_slot_pause_jump(struct Actor *obj);
s32 random_jump_dispatch_slot_random_jump(struct Actor *obj);
s32 dispatch_slot_list(struct Actor *obj);
s32 dispatch_slot_begin_hold(struct Actor *obj);
s32 dispatch_slot_jump(struct Actor *obj);
s32 jump_orbit_dispatch_slot_pause_jump(struct Actor *obj);
s32 dispatch_slot_hold(struct Actor *obj);
void jump_orbit_setup_list_slot_a(struct Actor *obj);
void jump_orbit_setup_list_slot_b(struct Actor *obj);
void spawn_part_by_mode(struct Actor *obj);
void set_action_by_mode(struct Actor *obj);
void wait_then_spawn_walker(struct Actor *obj);
void wait_then_spawn_traveler(struct Actor *obj);
void finish_child_and_move(struct Actor *obj);
void release_then_move(struct Actor *obj);
void spawn_child_4059(struct Actor *obj);
void spawn_child_405a(struct Actor *obj);
void spawn_child_4059_b(struct Actor *obj);
void spawn_child_4068_sprite(struct Actor *obj);
void spawn_child_4068_sprite_b(struct Actor *obj);
void spawn_child_4068_sprite_c(struct Actor *obj);
void spawn_child_4068_sprite_d(struct Actor *obj);
void dispatch_slot_spawn_ext(struct Actor *obj);
void hold_then_orbit_b(struct Actor *obj);

s32 part_step_arm_timed_motion_art2_v12(struct Actor *obj);
s32 part_step_arm_slot_motions(struct Actor *obj);
s32 enter_arm_timed_motion_art6_v5(struct Actor *obj);
s32 enter_arm_timed_motion_art2_v5(struct Actor *obj);
s32 enter_arm_timed_motion_art6_v5_b(struct Actor *obj);
s32 enter_arm_timed_motion_art2_v5_b(struct Actor *obj);
s32 enter_arm_timed_motion_art7_v3(struct Actor *obj);
s32 enter_arm_timed_motion_art2_v3(struct Actor *obj);
s32 part_step_start_move_to_owner(struct Actor *obj);
s32 part_step_wait_mode_3(struct Actor *obj);
s32 part_step_wait_mode_2(struct Actor *obj);
s32 part_step_wait_mode_1(struct Actor *obj);
s32 part_step_wait_mode_0(struct Actor *obj);
s32 part_step_move_to_owner_base_3000(struct Actor *obj);
void arrive_when_timer_done(struct Actor *obj);
void pose_update_if_visible_a(struct Actor *obj);
void pose_update_if_visible_b(struct Actor *obj);
void pose_update_if_visible_c(struct Actor *obj);
void pose_update_if_visible_d(struct Actor *obj);
void pose_update_if_visible_e(struct Actor *obj);
void pose_update_if_visible_f(struct Actor *obj);
void pose_update_if_visible_g(struct Actor *obj);
void on_arrive_if_posed_b(struct Actor *obj);
void finish_if_posed(struct Actor *obj);
void arm_timed_motion_art2_v12(struct Actor *obj);
void arm_timed_motion_art6_v5(struct Actor *obj);
void arm_timed_motion_art2_v5(struct Actor *obj);
void arm_timed_motion_art6_v5_b(struct Actor *obj);
void arm_timed_motion_art2_v5_b(struct Actor *obj);
void arm_timed_motion_art7_v3(struct Actor *obj);
void arm_timed_motion_art2_v3(struct Actor *obj);
void turn_and_arrive(struct Actor *obj);
void clear_record_b4(struct Actor *obj);
void reset_index_and_act2(struct Actor *obj);
void state_advance_pos_by_vel(struct Actor *obj);
void wait_then_act8(struct Actor *obj);
void wait_then_act4(struct Actor *obj);
void wait_then_act8_b(struct Actor *obj);
void wait_then_act4_b(struct Actor *obj);
void wait_then_act9(struct Actor *obj);
void wait_then_act5(struct Actor *obj);
void wait_then_act5_b(struct Actor *obj);
void begin_wait_act7(struct Actor *obj);
void begin_wait_act3(struct Actor *obj);
void begin_wait_act7_b(struct Actor *obj);
void begin_wait_act3_b(struct Actor *obj);
void begin_wait_act8(struct Actor *obj);
void begin_wait_act3_c(struct Actor *obj);
void begin_wait_act4(struct Actor *obj);
void begin_motion_wait_a(struct Actor *obj);
void begin_motion_wait_b(struct Actor *obj);
void start_move_to_owner_b(struct Actor *obj);
void sub_0808C7C8(struct Actor *obj);
void start_action5_if_idle(struct Actor *obj);
void wait_then_set_action(struct Actor *obj);
void wait_then_remove_part(struct Actor *obj);
void place_effect_and_remove(struct Actor *obj);
void arm_slot_motions(struct Actor *obj);
void start_move_to_owner(struct Actor *obj);
void wait_release_child(struct Actor *obj);
void arm_wait_by_mode(struct Actor *obj, u32 defaultTimer);
void move_to_owner_base_3000(struct Actor *obj);
void await_child_pose(struct Actor *obj);
void remove_if_no_index(struct Actor *obj);
s32 try_park_in_slot(struct Actor *obj);
void slot_wait_then_resume(struct Actor *obj);
s32 setup_slot_by_flags(struct Actor *obj);
s32 poll_action_end(struct Actor *obj);
void end_action_if_done(struct Actor *obj);
void orbit_slots_spawn_orbit_child(struct Actor *obj);
void orbit_slots_orbit_step_a(struct Actor *obj);
void orbit_slots_orbit_step_b(struct Actor *obj);
void hold_then_orbit(struct Actor *obj);
void orbit_position_a(struct Actor *obj);
void finish_orbit(struct Actor *obj);
void orbit_slots_spawn_orbit_child_b(struct Actor *obj);
void orbit_slots_orbit_step_c(struct Actor *obj);
void orbit_slots_orbit_step_d(struct Actor *obj);
s32 dispatch_slot_by_flags(struct Actor *obj);

// flags76: action mode and kind
struct ActBits
{
    u8 unk0 : 1;
    u8 mode : 2;
    u8 kind : 3;
    u8 unk6 : 2;
};

// Shared epilogue of an action: unlock the data slot, drop the child, reset the pose and
// return to action 0. Inlined into its callers (the argument order of the call differs from
// a plain call).
static inline void actor_end_action(struct Actor *obj)
{
    if (((struct ActBits *)&obj->flags76)->mode == 1)
    {
        if (obj->data != NULL)
            ((struct DataLockBits *)&obj->data->lock)->locked = 0;
        if (obj->child != NULL)
        {
            obj_release(obj->child);
            obj->child = NULL;
        }
        ((struct PoseDirBits *)&obj->pose->dir)->turn = 0;
        ((struct PoseFlagBits *)&obj->pose->flags)->hold = 0;
        obj->pose->result = 0x10;
        sprite_set_action(obj, 0, -1, 0);
        obj->state = NULL;
    }
}

void pose_update_if_visible_a(struct Actor *obj)
{
    if (obj->pose->flags & 8)
        on_arrive(obj);
}

void wait_then_set_action(struct Actor *obj)
{
    u16 t = obj->timer;

    obj->timer = t - 1;
    if ((s16)t <= 0)
    {
        set_slot_value(0x128, -1);
        switch (obj->mode)
        {
        case 1:
            sprite_set_action(obj, 8, 0, 0);
            break;
        case 0:
        case 2:
            sprite_set_action(obj, 4, 0, 0);
            break;
        case 3:
            sprite_set_action(obj, 8, 0, 0);
            break;
        }
        obj->state = spawn_part_by_mode;
    }
}

s32 part_step_wait_mode_3(struct Actor *obj)
{
    obj->state = (void (*)(struct Actor *))arm_wait_by_mode;
    obj->mode = 3;
    return 1;
}

s32 part_step_wait_mode_2(struct Actor *obj)
{
    obj->state = (void (*)(struct Actor *))arm_wait_by_mode;
    obj->mode = 2;
    return 1;
}

ASM_FUNC("asm/code_0808C6F0/part_step_wait_mode_1.s", s32 part_step_wait_mode_1(struct Actor *obj))

s32 part_step_wait_mode_0(struct Actor *obj)
{
    obj->state = (void (*)(struct Actor *))arm_wait_by_mode;
    obj->mode = 0;
    return 1;
}

void sub_0808C7C8(struct Actor *obj)
{
    obj->dx = obj->x - obj->baseX;
    obj->dy = obj->y - obj->baseY;
    obj->dh = obj->h - obj->baseH;
    obj->state = clear_record_b4;
}

void start_action5_if_idle(struct Actor *obj)
{
    if ((s16)obj->timer == 0)
    {
        sprite_set_action(obj, 5, 0, 0);
        obj->child->timer = 0x10;
        obj->state = on_arrive_if_posed_b;
    }
}

s32 part_step_arm_timed_motion_art2_v12(struct Actor *obj)
{
    obj->state = arm_timed_motion_art2_v12;
    return 1;
}

void wait_then_remove_part(struct Actor *obj)
{
    struct Actor *owner = obj->owner;
    u16 t;
    s32 v;

    if (!(owner->flags76 & 0x80) && owner->h == 0)
        partner_link_update(obj, owner, 0xFF);
    t = obj->timer;
    obj->timer = t - 1;
    if ((s16)t <= 0)
    {
        v = obj->data->unk38;
        if (v == 0)
            obj->data->unkB4 = v;
        sound_stop_song(0x5E);
        obj_release(obj);
    }
}

void arrive_when_timer_done(struct Actor *obj)
{
    if ((s16)obj->timer == 0)
        on_arrive(obj);
}

s32 part_step_arm_slot_motions(struct Actor *obj)
{
    obj->state = arm_slot_motions;
    return 1;
}

void pose_update_if_visible_b(struct Actor *obj)
{
    if (obj->pose->flags & 8)
        on_arrive(obj);
}

s32 enter_arm_timed_motion_art6_v5(struct Actor *obj)
{
    obj->state = arm_timed_motion_art6_v5;
    return 1;
}

void pose_update_if_visible_c(struct Actor *obj)
{
    if (obj->pose->flags & 8)
        on_arrive(obj);
}

s32 enter_arm_timed_motion_art2_v5(struct Actor *obj)
{
    obj->state = arm_timed_motion_art2_v5;
    return 1;
}

void pose_update_if_visible_d(struct Actor *obj)
{
    if (obj->pose->flags & 8)
        on_arrive(obj);
}

s32 enter_arm_timed_motion_art6_v5_b(struct Actor *obj)
{
    obj->state = arm_timed_motion_art6_v5_b;
    return 1;
}

void pose_update_if_visible_e(struct Actor *obj)
{
    if (obj->pose->flags & 8)
        on_arrive(obj);
}

s32 enter_arm_timed_motion_art2_v5_b(struct Actor *obj)
{
    obj->state = arm_timed_motion_art2_v5_b;
    return 1;
}

void place_effect_and_remove(struct Actor *obj)
{
    if (obj->index == 0)
    {
        set_slot_value(0xAD, -1);
        effect_spawn_at(0xC4B, obj->x / 0x100, obj->y / 0x100, obj->h / 0x100, obj);
        obj_release(obj);
    }
}

s32 part_step_move_to_owner_base_3000(struct Actor *obj)
{
    obj->state = move_to_owner_base_3000;
    obj->resume = reset_index_and_act2;
    return 1;
}

void pose_update_if_visible_f(struct Actor *obj)
{
    if (obj->pose->flags & 8)
        on_arrive(obj);
}

s32 enter_arm_timed_motion_art7_v3(struct Actor *obj)
{
    obj->state = arm_timed_motion_art7_v3;
    return 1;
}

void pose_update_if_visible_g(struct Actor *obj)
{
    if (obj->pose->flags & 8)
        on_arrive(obj);
}

s32 enter_arm_timed_motion_art2_v3(struct Actor *obj)
{
    obj->state = arm_timed_motion_art2_v3;
    return 1;
}

void remove_if_no_index(struct Actor *obj)
{
    if (obj->index == 0)
    {
        set_slot_value(0xAD, -1);
        sprite_set_action(obj, 1, 0, 0);
        obj_list_remove(obj);
        obj->state = finish_if_posed;
    }
}

void turn_and_arrive(struct Actor *obj)
{
    struct Pose *pose;

    if (obj->index == 0)
    {
        sound_stop_song(0x6C);
        pose = obj->pose;
        ((struct PoseDirBits *)&pose->dir)->turn = 1;
        on_arrive(obj);
    }
}

void await_child_pose(struct Actor *obj)
{
    struct Actor *child = obj->child;
    s32 f;

    if (child->pose->flags & 8)
    {
        sprite_set_action(child, 1, 0, 0);
        ((struct PoseFlagBits *)&obj->child->pose->flags)->mode = 0;
        obj->timer = 8;
        obj->state = wait_release_child;
    }
}

s32 part_step_start_move_to_owner(struct Actor *obj)
{
    obj->state = start_move_to_owner;
    return 1;
}

ASM_FUNC("asm/code_0808C6F0/arm_wait_by_mode.s", void arm_wait_by_mode(struct Actor *obj, u32 defaultTimer))

void clear_record_b4(struct Actor *obj)
{
    s32 index = obj->index;

    if (index == 0)
    {
        sound_stop_song(0x5E);
        obj->data->unkB4 = index;
        obj->state = state_advance_pos_by_vel;
    }
}

void on_arrive_if_posed_b(struct Actor *obj)
{
    if (obj->pose->flags & 8)
        on_arrive(obj);
}

void arm_timed_motion_art2_v12(struct Actor *obj)
{
    obj->timer = 12;
    motion_timed(obj, 2, 12);
    obj->next = wait_then_spawn_walker;
}

void arm_slot_motions(struct Actor *obj)
{
    obj->timer = 0x18;
    if (obj->owner->data->slot == -1)
        motion_timed(obj, 4, 0);
    if (obj->owner->data->slot == -2)
        motion_timed(obj, 6, 0);
    obj->next = wait_then_spawn_traveler;
}

void arm_timed_motion_art6_v5(struct Actor *obj)
{
    obj->timer = 5;
    motion_timed(obj, 6, 5);
    obj->next = begin_wait_act7;
}

void arm_timed_motion_art2_v5(struct Actor *obj)
{
    obj->timer = 5;
    motion_timed(obj, 2, 5);
    obj->next = begin_wait_act3;
}

void arm_timed_motion_art6_v5_b(struct Actor *obj)
{
    obj->timer = 5;
    motion_timed(obj, 6, 5);
    obj->next = begin_wait_act7_b;
}

void arm_timed_motion_art2_v5_b(struct Actor *obj)
{
    obj->timer = 5;
    motion_timed(obj, 2, 5);
    obj->next = begin_wait_act3_b;
}

void reset_index_and_act2(struct Actor *obj)
{
    obj->index = 0;
    obj->timer = 0;
    sprite_set_action(obj, 2, 0, 0);
    obj->state = release_then_move;
}

ASM_FUNC("asm/code_0808C6F0/move_to_owner_base_3000.s", void move_to_owner_base_3000(struct Actor *obj))

void arm_timed_motion_art7_v3(struct Actor *obj)
{
    obj->timer = 3;
    motion_timed(obj, 7, 3);
    obj->next = begin_wait_act8;
}

void arm_timed_motion_art2_v3(struct Actor *obj)
{
    obj->timer = 3;
    motion_timed(obj, 2, 3);
    obj->next = begin_wait_act3_c;
}

void finish_if_posed(struct Actor *obj)
{
    if (obj->pose->flags & 8)
        obj_release(obj);
}

void wait_release_child(struct Actor *obj)
{
    struct Actor *owner = obj->owner;
    u16 t;
    s32 f;

    if (!(owner->flags76 & 0x80) && owner->h == 0)
        partner_link_update(obj, owner, 0xFF);
    t = obj->timer;
    obj->timer = t - 1;
    if ((s16)t <= 0)
    {
        sprite_set_action(obj, 7, 0, 0);
        sprite_set_action(obj->child, 2, 0, 0);
        ((struct PoseFlagBits *)&obj->child->pose->flags)->mode = 1;
        obj->state = finish_child_and_move;
    }
}

ASM_FUNC("asm/code_0808C6F0/start_move_to_owner.s", void start_move_to_owner(struct Actor *obj))

void state_advance_pos_by_vel(struct Actor *obj)
{
    obj->x += obj->dx;
    obj->y += obj->dy;
    obj->h += obj->dh;
}

void begin_wait_act7(struct Actor *obj)
{
    s32 f;

    sprite_set_action(obj, 7, 0, 0);
    ((struct PoseFlagBits *)&obj->pose->flags)->mode = 0;
    obj->timer = 0x2d;
    obj->state = wait_then_act8;
}

void begin_wait_act3(struct Actor *obj)
{
    s32 f;

    sprite_set_action(obj, 3, 0, 0);
    ((struct PoseFlagBits *)&obj->pose->flags)->mode = 0;
    obj->timer = 0x2d;
    obj->state = wait_then_act4;
}

void begin_wait_act7_b(struct Actor *obj)
{
    s32 f;

    sprite_set_action(obj, 7, 0, 0);
    ((struct PoseFlagBits *)&obj->pose->flags)->mode = 0;
    obj->timer = 0x2d;
    obj->state = wait_then_act8_b;
}

void begin_wait_act3_b(struct Actor *obj)
{
    s32 f;

    sprite_set_action(obj, 3, 0, 0);
    ((struct PoseFlagBits *)&obj->pose->flags)->mode = 0;
    obj->timer = 0x2d;
    obj->state = wait_then_act4_b;
}

void begin_motion_wait_a(struct Actor *obj)
{
    if (obj->index == 0)
    {
        sound_stop_song(0x81);
        obj_list_push_default(obj);
        obj->timer = 0x18;
        motion_timed(obj, 3, 0);
        obj->next = start_move_to_owner_b;
    }
}

void begin_wait_act8(struct Actor *obj)
{
    s32 f;

    sprite_set_action(obj, 8, 0, 0);
    ((struct PoseFlagBits *)&obj->pose->flags)->mode = 0;
    obj->timer = 0xc;
    obj->state = wait_then_act9;
}

void begin_wait_act3_c(struct Actor *obj)
{
    s32 f;

    sprite_set_action(obj, 3, 0, 0);
    ((struct PoseFlagBits *)&obj->pose->flags)->mode = 0;
    obj->timer = 0xc;
    obj->state = wait_then_act5;
}

void begin_motion_wait_b(struct Actor *obj)
{
    if (obj->index == 0)
    {
        sound_stop_song(0x6C);
        obj->timer = 0x18;
        motion_timed(obj, 3, 0x20);
        obj->next = begin_wait_act4;
    }
}

void wait_then_act8(struct Actor *obj)
{
    u16 t = obj->timer;

    obj->timer = t - 1;
    if ((s16)t <= 0)
    {
        sprite_set_action(obj, 8, 0, 0);
        obj->state = spawn_child_4068_sprite_d;
    }
}

void wait_then_act4(struct Actor *obj)
{
    u16 t = obj->timer;

    obj->timer = t - 1;
    if ((s16)t <= 0)
    {
        sprite_set_action(obj, 4, 0, 0);
        obj->state = spawn_child_4068_sprite_c;
    }
}

void wait_then_act8_b(struct Actor *obj)
{
    u16 t = obj->timer;

    obj->timer = t - 1;
    if ((s16)t <= 0)
    {
        sprite_set_action(obj, 8, 0, 0);
        obj->state = spawn_child_4068_sprite_b;
    }
}

void wait_then_act4_b(struct Actor *obj)
{
    u16 t = obj->timer;

    obj->timer = t - 1;
    if ((s16)t <= 0)
    {
        sprite_set_action(obj, 4, 0, 0);
        obj->state = spawn_child_4068_sprite;
    }
}

ASM_FUNC("asm/code_0808C6F0/start_move_to_owner_b.s", void start_move_to_owner_b(struct Actor *obj))

void wait_then_act9(struct Actor *obj)
{
    u16 t = obj->timer;

    obj->timer = t - 1;
    if ((s16)t <= 0)
    {
        sprite_set_action(obj, 9, 0, 0);
        obj->state = spawn_child_4059_b;
    }
}

void wait_then_act5(struct Actor *obj)
{
    u16 t = obj->timer;

    obj->timer = t - 1;
    if ((s16)t <= 0)
    {
        sprite_set_action(obj, 5, 0, 0);
        obj->state = spawn_child_4059;
    }
}

void begin_wait_act4(struct Actor *obj)
{
    s32 f;

    sprite_set_action(obj, 4, 0, 0);
    ((struct PoseFlagBits *)&obj->pose->flags)->mode = 0;
    obj->state = wait_then_act5_b;
    obj->timer = 0x1E;
}

void wait_then_act5_b(struct Actor *obj)
{
    u16 t = obj->timer;

    obj->timer = t - 1;
    if ((s16)t <= 0)
    {
        sprite_set_action(obj, 5, 0, 0);
        obj->state = spawn_child_405a;
    }
}

ASM_FUNC("asm/code_0808C6F0/dispatch_slot_by_flags.s", s32 dispatch_slot_by_flags(struct Actor *obj))

ASM_FUNC("asm/code_0808C6F0/try_park_in_slot.s", s32 try_park_in_slot(struct Actor *obj))

ASM_FUNC("asm/code_0808C6F0/slot_wait_then_resume.s", void slot_wait_then_resume(struct Actor *obj))

ASM_FUNC("asm/code_0808C6F0/setup_slot_by_flags.s", s32 setup_slot_by_flags(struct Actor *obj))

s32 poll_action_end(struct Actor *obj)
{
    if (((struct ActBits *)&obj->flags76)->kind == 0)
        return 0;
    actor_end_action(obj);
    return 1;
}

void end_action_if_done(struct Actor *obj)
{
    if (poll_action_end(obj) == 0 && (obj->pose->flags & 8))
        actor_end_action(obj);
}

ASM_FUNC("asm/code_0808C6F0/orbit_slots_spawn_orbit_child.s", void orbit_slots_spawn_orbit_child(struct Actor *obj))

void orbit_slots_orbit_step_a(struct Actor *obj)
{
    s32 done;

    obj->state = orbit_slots_orbit_step_a;
    done = poll_action_end(obj);
    if (done == 0)
    {
        if ((s16)obj->timer == 0)
            sprite_set_action(obj, 0, 0x2025, 0);
        if ((s16)obj->timer == 2)
            sprite_set_action(obj, 0, 0x2025, 1);
        if ((s16)obj->timer == 4)
            sprite_set_action(obj, 0, 0x2025, 2);
        if ((s16)obj->timer == 6)
            sprite_set_action(obj, 0, 0x2025, 3);
        if ((s16)obj->timer == 7)
            set_cur_flags_both(0x40000000);
        if ((s16)obj->timer != 8)
        {
            if ((gUnk_0300034C.held & obj->mask) == 0)
            {
                obj->timer = (s16)obj->timer * 2;
                obj->state = orbit_slots_orbit_step_b;
                return;
            }
        }
        else
        {
            sprite_set_action(obj, 1, 0, 0);
            ((struct PoseFlagBits *)&obj->pose->flags)->mode = 0;
            obj->timer = done;
            obj->state = hold_then_orbit;
        }
        ((struct PoseFlagBits *)&obj->pose->flags)->hold = 1;
        obj->timer++;
    }
}

ASM_FUNC("asm/code_0808C6F0/orbit_slots_orbit_step_b.s", void orbit_slots_orbit_step_b(struct Actor *obj))

void hold_then_orbit(struct Actor *obj)
{
    s32 held;
    s32 f;

    if (poll_action_end(obj) != 0)
        return;
    held = gUnk_0300034C.held & obj->mask;
    if (held == 0)
    {
        obj->unk9C = 1;
        obj->child->unkA0 = 0xB400;
        obj->child->unkA4 = -0x1CC;
        sprite_set_action(obj, 2, 0, 0);
        ((struct PoseFlagBits *)&obj->pose->flags)->hold = 1;
        obj->state = orbit_position_a;
        obj->timer = held;
        return;
    }
    if ((s16)obj->timer > 0x31)
    {
        if (obj->data->kind == 0x1E)
        {
            if ((gUnk_03000FD8[9] & 0xF0) == 0x10)
            {
                sprite_set_action(obj, 4, 0, 0);
                ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
                obj->pose->result = MATH_ROUTINE(MathFunc, iwram_div)(0xC0, 8);
                ((struct PoseFlagBits *)&obj->pose->flags)->hold = 0;
                obj->state = end_action_if_done;
                return;
            }
            obj->timer = 0x32;
            return;
        }
        sprite_set_action(obj, 5, 0, 0);
        ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
        ((struct PoseFlagBits *)&obj->pose->flags)->hold = 0;
        obj->state = end_action_if_done;
        set_cur_flags_both(0x40000000);
        return;
    }
    if ((s16)obj->timer > 0x18)
    {
        ((struct PoseFlagBits *)&obj->pose->flags)->hold = 0;
    }
    obj->timer++;
}

ASM_FUNC("asm/code_0808C6F0/orbit_position_a.s", void orbit_position_a(struct Actor *obj))

void finish_orbit(struct Actor *obj)
{
    if (obj->data != NULL)
        ((struct DataLockBits *)&obj->data->lock)->locked = 0;
    if (poll_action_end(obj) == 0 && (s16)obj->timer++ > 23)
        actor_end_action(obj);
}

ASM_FUNC("asm/code_0808C6F0/orbit_slots_spawn_orbit_child_b.s", void orbit_slots_spawn_orbit_child_b(struct Actor *obj))

void orbit_slots_orbit_step_c(struct Actor *obj)
{
    s32 done;

    obj->state = orbit_slots_orbit_step_c;
    done = poll_action_end(obj);
    if (done == 0)
    {
        if ((s16)obj->timer == 0)
            sprite_set_action(obj, 0, 0x2054, 0);
        if ((s16)obj->timer == 3)
            sprite_set_action(obj, 0, 0x2054, 1);
        if ((s16)obj->timer == 6)
            sprite_set_action(obj, 0, 0x2054, 2);
        if ((s16)obj->timer == 9)
            sprite_set_action(obj, 0, 0x2054, 3);
        if ((s16)obj->timer == 0xB)
            set_cur_flags_both(0x40000000);
        if ((s16)obj->timer != 0xC)
        {
            if ((gUnk_0300034C.held & obj->mask) == 0)
            {
                obj->timer = (s16)obj->timer * 3;
                obj->state = orbit_slots_orbit_step_d;
                return;
            }
        }
        else
        {
            sprite_set_action(obj, 1, 0, 0);
            ((struct PoseFlagBits *)&obj->pose->flags)->mode = 0;
            obj->timer = done;
            obj->state = hold_then_orbit_b;
        }
        ((struct PoseFlagBits *)&obj->pose->flags)->hold = 1;
        obj->timer++;
    }
}

ASM_FUNC("asm/code_0808C6F0/orbit_slots_orbit_step_d.s", void orbit_slots_orbit_step_d(struct Actor *obj))
