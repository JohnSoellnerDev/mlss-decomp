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

struct Lo3Bits
{
    u8 lo3 : 3;
    u8 rest : 5;
};

struct Flags77Bits
{
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 bit7 : 1;
};

struct SlotEntry
{
    u8 filler0[0x2C];
    u16 v2C;
    u8 filler2E[0x3C - 0x2E];
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

extern struct KeyState gSys;
extern u8 *gWorld;

struct WorldSlots
{
    u8 filler0[0x264];
    void (*savedState[2])(struct Actor *); /* each hero's state while it is parked (try_park_in_slot), by ~Data.slot */
};

/* Data.slot of the two heroes: the bitwise NOT of their gParty index */
#define HERO_INDEX_MARIO -1
#define HERO_INDEX_LUIGI -2
extern u8 *gParty;

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

s32 part_step_wait_mode_1(struct Actor *obj)
{
    s32 mode;

    obj->state = (void (*)(struct Actor *))arm_wait_by_mode;
    mode = 1;
    obj->mode = mode;
    return mode;
}

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

void arm_wait_by_mode(struct Actor *obj, u32 defaultTimer)
{
    struct Actor *owner = obj->owner;

    switch (obj->mode)
    {
    case 0:
        defaultTimer = 8;
        break;
    case 1:
        defaultTimer = 8;
        break;
    case 2:
        defaultTimer = 8;
        break;
    case 3:
        defaultTimer = 8;
        break;
    }
    obj->timer = defaultTimer;
    if (owner->data->slot == -1)
        motion_timed(obj, 2, 8);
    if (owner->data->slot == -2)
        motion_timed(obj, 6, 8);
    obj->next = set_action_by_mode;
}

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

void move_to_owner_base_3000(struct Actor *obj)
{
    struct Actor *owner;
    u8 *fp;

    owner = obj->owner;
    obj->targetX = owner->baseX + 0x3000;
    obj->targetY = owner->baseY;
    obj->targetH = owner->baseH;
    fp = &obj->flags79;
    *fp |= 0x20;
    obj->unk7C = 0x200;
    obj->unk7A = 0;
    start_motion(obj);
    sprite_set_action(obj, 2, 0, 0);
    set_slot_value(0x81, -1);
    ((struct PoseFlagBits *)&obj->pose->flags)->mode = 0;
    obj->state = begin_motion_wait_a;
}

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

void start_move_to_owner(struct Actor *obj)
{
    struct Actor *owner = obj->owner;

    set_slot_value(0x6c, -1);
    obj->targetX = owner->baseX + 0x4800;
    obj->targetY = owner->baseY;
    obj->targetH = owner->baseH;
    *(u8 *)&obj->flags79 |= 0x20;
    obj->unk7C = 0x200;
    obj->unk7A = 0;
    start_motion(obj);
    sprite_set_action(obj, 2, 0, 0);
    obj->state = begin_motion_wait_b;
}

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

void start_move_to_owner_b(struct Actor *obj)
{
    struct Actor *owner;
    u8 *fp;

    owner = obj->owner;
    obj->targetX = owner->baseX;
    obj->targetY = owner->baseY;
    obj->targetH = obj->baseH;
    fp = &obj->flags79;
    *fp |= 0x20;
    obj->unk7C = 0x200;
    obj->unk7A = 0;
    start_motion(obj);
    obj->timer = 0xA;
    sprite_set_action(obj, 4, 0, 0);
    ((struct PoseFlagBits *)&obj->pose->flags)->mode = 0;
    set_slot_value(0xD5, -1);
    obj->state = release_then_move;
}

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

/*
 * Starts a hero's move with the handler for the battle's current mode and the hero's status:
 * gWorld flags0C bits 7 (0x80) and 4 (0x10) pick one of four handler sets, and within a set the
 * hero's status bits 0x400 / 0x800 (the two statuses that swap its sprite set) pick the variant.
 * The plain case sets up hero A's (slot -1) or hero B's (slot -2) move here and arrives through
 * on_arrive. Returns 1 (handled).
 */
s32 dispatch_slot_by_flags(struct Actor *hero)
{
    if ((gWorld[0xC] & 0x80) != 0)
    {
        if ((gWorld[0xC] & 0x10) != 0)
        {
            if ((hero->data->unk11E & 0x400) != 0)
                return dispatch_slot_random_jump_b(hero);
            if ((hero->data->unk11E & 0x800) != 0)
                return dispatch_slot_random_pause_jump(hero);
            return dispatch_slot_random_jump_a(hero);
        }
        if ((hero->data->unk11E & 0x400) != 0)
            return charge_release_dispatch_slot_random_jump(hero);
        if ((hero->data->unk11E & 0x800) != 0)
            return hold_decay_dispatch_slot_pause_jump(hero);
        return random_jump_dispatch_slot_random_jump(hero);
    }
    if ((gWorld[0xC] & 0x10) != 0)
    {
        if ((hero->data->unk11E & 0x400) != 0)
            return dispatch_slot_list(hero);
        if ((hero->data->unk11E & 0x800) != 0)
            return dispatch_slot_begin_hold(hero);
        return dispatch_slot_jump(hero);
    }
    if ((hero->data->unk11E & 0x400) != 0)
        return jump_orbit_dispatch_slot_pause_jump(hero);
    if ((hero->data->unk11E & 0x800) != 0)
        return dispatch_slot_hold(hero);
    if (hero->data->slot == -1)
        jump_orbit_setup_list_slot_b(hero);
    else if (hero->data->slot == -2)
        jump_orbit_setup_list_slot_a(hero);
    hero->callback = on_arrive;
    return 1;
}

/* At the top of a hero's jump: a hero whose Data.kind is 0x1C and whose button is held (A for
   Mario, B for Luigi) hangs there: action 0xF for up to 12 frames, its current state saved in
   gWorld's savedState until slot_wait_then_resume brings it back. Returns 1 if the hero was parked. */
s32 try_park_in_slot(struct Actor *hero)
{
    if (hero->data->kind != 0x1C)
        return 0;
    if (hero->data->slot == HERO_INDEX_MARIO)
    {
        if (((gSys.held ^ A_BUTTON) & A_BUTTON) != 0)
            return 0;
        sprite_set_action(hero, 0xF, -1, 0);
    }
    else
    {
        if ((gSys.held & B_BUTTON) == 0)
            return 0;
        sprite_set_action(hero, 0xF, -1, 0);
    }
    hero->timer = 0xC;
    ((struct WorldSlots *)gWorld)->savedState[~hero->data->slot] = hero->state;
    hero->state = slot_wait_then_resume;
    return 1;
}

/* State of a hero parked by try_park_in_slot: waits while its button (A for Mario, B for Luigi)
   stays held, at most `timer` frames, then plays action 6 and goes back to the state it had. */
void slot_wait_then_resume(struct Actor *hero)
{
    u16 timer;
    s32 button;
    u16 held;

    timer = hero->timer;
    if ((s16)hero->timer > 0)
    {
        hero->timer = timer - 1;
        if (hero->data->slot == HERO_INDEX_MARIO)
        {
            held = gSys.held;
            button = A_BUTTON;
        }
        else
        {
            held = gSys.held;
            button = B_BUTTON;
        }
        if ((button & held) != 0)
            return;
    }
    sprite_set_action(hero, 6, -1, 0);
    ((struct PoseFlagBits *)&hero->pose->flags)->mode = 1;
    ((struct Lo3Bits *)&hero->flags77)->lo3 = 1;
    {
        struct WorldSlots *world = (struct WorldSlots *)gWorld;

        hero->state = world->savedState[~hero->data->slot];
        world->savedState[~hero->data->slot] = 0;
    }
}



/*
 * Sets up a hero's orbit move with the handler for the battle's current mode and the hero's
 * status, like dispatch_slot_by_flags: gWorld flags0C bit 7 (0x80) picks the handler set, and the
 * hero's status bits 0x400 / 0x800 pick the variant. The plain case clears flags77 bits 5-6,
 * copies the u16 at 0x2C of the hero's gParty record (index ~slot) into its data's slotValue and
 * spawns the orbit child next (dispatch_slot_spawn_ext), ending through end_action. Returns 1.
 */
s32 setup_slot_by_flags(struct Actor *hero)
{
    if ((gWorld[0xC] & 0x80) != 0)
    {
        if ((hero->data->unk11E & 0x400) != 0)
            return charge_release_setup_slot_state(hero);
        if ((hero->data->unk11E & 0x800) != 0)
            return hold_decay_setup_slot_state(hero);
        return random_jump_setup_slot_spawn(hero);
    }
    if ((hero->data->unk11E & 0x400) != 0)
        return jump_orbit_setup_slot_spawn(hero);
    if ((hero->data->unk11E & 0x800) != 0)
        return list_slot_start(hero);
    ((struct Flags77Bits *)&hero->flags77)->bit6 = 0;
    ((struct Flags77Bits *)&hero->flags77)->bit5 = 0;
    {
        u32 *dest;
        struct SlotEntry *member;

        dest = &hero->data->slotValue;
        member = (struct SlotEntry *)gParty;
        member += ~hero->data->slot;
        *dest = member->v2C;
    }
    hero->state = dispatch_slot_spawn_ext;
    hero->callback = end_action;
    return 1;
}


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

// Starts the first orbit move: spawns the orbiting child at obj's base point (in whole pixels),
// in action mode 3 with its box bytes (unk44-unk49: -10, 8, -10, 10, -12, 4), aiming at obj's base
// X and its own base height, with animation gAnim284 and on_arrive as its callback. Then resets
// the timer, clears slot value 0x7C and runs the first wind-up frame (orbit_slots_orbit_step_a).
void orbit_slots_spawn_orbit_child(struct Actor *obj)
{
    struct Actor *child;
    u8 *flags;

    child = obj_spawn(obj->data, 0, obj->baseX / 256, obj->baseY / 256, obj->baseH / 256, 0, 1);
    obj->child = child;
    flags = &child->flags76;
    *flags = *flags | 6; // ActBits.mode = 3
    child->unk44 = 0xF6;
    child->unk45 = 8;
    child->unk46 = 0xF6;
    child->unk47 = 0xA;
    child->unk48 = 0xF4;
    child->unk49 = 4;
    ((struct Lo3Bits *)&child->flags77)->lo3 = 2;
    child->targetX = obj->baseX;
    child->targetH = child->baseH;
    child->anim = gAnim284;
    flags = &child->flags76;
    *flags = *flags | 0x40;
    child->callback = on_arrive;
    flags = &child->flags79;
    *flags = *flags | 1;
    obj->timer = 0;
    set_slot_value(0x7C, -1);
    orbit_slots_orbit_step_a(obj);
}

/* Charge wind-up, one frame (timer = frames so far): while the button (obj->mask) is held, steps
   animation 0x2025 through frames 0-3 (at timer 0, 2, 4 and 6) with the pose held, and at timer 7
   sets flag 0x40000000 in both of the current state's flag words. At timer 8 the wind-up is
   complete: action 1, timer reset and on to hold_then_orbit. Letting go earlier doubles the timer
   and hands over to orbit_slots_orbit_step_b, which winds back down. */
void orbit_slots_orbit_step_a(struct Actor *obj)
{
    s32 actionEnded;

    obj->state = orbit_slots_orbit_step_a;
    actionEnded = poll_action_end(obj);
    if (actionEnded == 0)
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
            if ((gSys.held & obj->mask) == 0)
            {
                /* let go too early */
                obj->timer = (s16)obj->timer * 2;
                obj->state = orbit_slots_orbit_step_b;
                return;
            }
        }
        else
        {
            sprite_set_action(obj, 1, 0, 0);
            ((struct PoseFlagBits *)&obj->pose->flags)->mode = 0;
            obj->timer = actionEnded;
            obj->state = hold_then_orbit;
        }
        ((struct PoseFlagBits *)&obj->pose->flags)->hold = 1;
        obj->timer++;
    }
}

/* Charge let go too early (after orbit_slots_orbit_step_a doubled the timer): winds animation 0x2025
   back down, the timer falling by 2 a frame and showing frame 2, 1, then 0 (only frames the wind-up
   reached), the pose held between steps. Once the timer runs out it plays action 4 once at frame
   time 0xC0 / 8 and ends the action when it is done. */
void orbit_slots_orbit_step_b(struct Actor *obj)
{
    s32 charge;
    u32 frameTime;

    obj->state = orbit_slots_orbit_step_b;
    if (poll_action_end(obj) != 0)
        return;
    charge = (s16)obj->timer;
    if (charge < 0)
    {
        sprite_set_action(obj, 4, 0, 0);
        ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
        frameTime = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0xC0, 8);
        obj->pose->result = frameTime;
        ((struct PoseFlagBits *)&obj->pose->flags)->hold = 0;
        obj->state = end_action_if_done;
        return;
    }
    /* pose->count: the frame the wind-up got to */
    if (charge <= 3)
    {
        if (obj->pose->count != 0)
            sprite_set_action(obj, 0, 0x2025, 0);
    }
    else if (charge <= 7)
    {
        if (obj->pose->count > 1)
            sprite_set_action(obj, 0, 0x2025, 1);
    }
    else if (charge <= 11)
    {
        if (obj->pose->count > 2)
            sprite_set_action(obj, 0, 0x2025, 2);
    }
    ((struct PoseFlagBits *)&obj->pose->flags)->hold = 1;
    obj->timer -= 2;
}


/* Charge state after orbit_slots_orbit_step_a: while the button (obj->mask) is held the timer counts
   up and the pose's hold flag goes off after 24 frames. Letting go starts the orbit (child angle
   0xB400, speed -0x1CC; orbit_position_a). Held past 49 frames, the charge fails (action 5,
   set_cur_flags_both); data kind 0x1E instead waits there until gWorld[9]'s high nibble is 1, then
   cancels with action 4. */
void hold_then_orbit(struct Actor *obj)
{
    s32 held;

    if (poll_action_end(obj) != 0)
        return;
    held = gSys.held & obj->mask;
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
    if ((s16)obj->timer > 49)
    {
        if (obj->data->kind == 0x1E)
        {
            if ((gWorld[9] & 0xF0) == 0x10)
            {
                sprite_set_action(obj, 4, 0, 0);
                ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
                obj->pose->result = MATH_ROUTINE(MathFunc, iwram_div)(0xC0, 8);
                ((struct PoseFlagBits *)&obj->pose->flags)->hold = 0;
                obj->state = end_action_if_done;
                return;
            }
            obj->timer = 50;
            return;
        }
        sprite_set_action(obj, 5, 0, 0);
        ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
        ((struct PoseFlagBits *)&obj->pose->flags)->hold = 0;
        obj->state = end_action_if_done;
        set_cur_flags_both(0x40000000);
        return;
    }
    if ((s16)obj->timer > 24)
    {
        ((struct PoseFlagBits *)&obj->pose->flags)->hold = 0;
    }
    obj->timer++;
}

/* One frame of orbit a: swings the child around a circle above obj's base point (child angle
 * unkA0 += its speed unkA4, the speed bent by 0xC00 towards obj's facing), steps obj's
 * animation 0x2025 through frames 1-5, and at step 7 releases the child (action mode 3),
 * locks obj's data and goes on to finish_orbit. */
void orbit_position_a(struct Actor *obj)
{
    struct Actor *child;
    s32 *angle;
    s32 *speed;
    const s16 *cosTable;
    const s16 *cosEntry;
    const s16 *sinEntry;
    s32 wrapped;
    s32 tableIdx;
    s32 cosScaled;
    s32 sinScaled;
    s32 centerH;

    if (poll_action_end(obj) != 0)
        return;
    child = obj->child;
    ((struct ActBits *)&child->flags76)->mode = 1;
    angle = &child->unkA0;
    speed = &child->unkA4;
    *angle += *speed;
    wrapped = MATH_ROUTINE(MathFunc, iwram_div)(*angle, 360);
    cosTable = gCosineTable;
    tableIdx = wrapped & 0xFF;
    cosEntry = cosTable + tableIdx;
    cosScaled = *cosEntry / 64;
    child->x = obj->baseX + cosScaled * 22;
    centerH = obj->baseH + 0x1600;
    sinEntry = &gSineTable[tableIdx];
    sinScaled = *sinEntry / 64;
    child->h = centerH + sinScaled * 22;
    *speed = (obj->pose->dir & 0x40) != 0 ? *speed + 0xC00 : *speed - 0xC00;
    if ((s16)obj->timer == 2)
        sprite_set_action(obj, 2, 0x2025, 1);
    if ((s16)obj->timer == 3)
        sprite_set_action(obj, 2, 0x2025, 2);
    if ((s16)obj->timer == 4)
        sprite_set_action(obj, 2, 0x2025, 3);
    if ((s16)obj->timer == 5)
        sprite_set_action(obj, 2, 0x2025, 4);
    if ((s16)obj->timer == 6)
        sprite_set_action(obj, 2, 0x2025, 5);
    if ((s16)obj->timer == 7)
    {
        u8 *childAct;

        childAct = &child->flags76;
        *childAct = *childAct | 6; // ActBits.mode = 3: release the child
        sprite_set_action(obj, 3, 0, 0);
        ((struct PoseFlagBits *)&obj->pose->flags)->mode = 0;
        ((struct PoseFlagBits *)&obj->pose->flags)->hold = 0;
        obj->timer = 0;
        obj->state = finish_orbit;
        if (obj->data != NULL)
            ((struct DataLockBits *)&obj->data->lock)->locked = 1;
    }
    else
    {
        ((struct PoseFlagBits *)&obj->pose->flags)->hold = 1;
        obj->timer++;
    }
}

/* After orbit_position_a has thrown the child: unlocks obj's data again, then ends the action
 * once 24 more frames have passed (unless poll_action_end ends it first). */
void finish_orbit(struct Actor *obj)
{
    if (obj->data != NULL)
        ((struct DataLockBits *)&obj->data->lock)->locked = 0;
    if (poll_action_end(obj) == 0 && (s16)obj->timer++ > 23)
        actor_end_action(obj);
}

/* Start of the second orbit move: spawns the child the actor holds at its base position (action
 * mode 3, hit box x -10/8, y -10/10, h -12/4, animation gAnim284, on_arrive when it lands), plays
 * sound 0x7C and runs the wind-up, orbit_slots_orbit_step_c, from frame 0. */
void orbit_slots_spawn_orbit_child_b(struct Actor *obj)
{
    struct Actor *child;
    u8 *flags;

    child = obj_spawn(obj->data, 0, obj->baseX / 256, obj->baseY / 256, obj->baseH / 256, 0, 1);
    obj->child = child;
    flags = &child->flags76;
    *flags = *flags | 6; /* ActBits.mode = 3 */
    child->unk44 = -10;
    child->unk45 = 8;
    child->unk46 = -10;
    child->unk47 = 10;
    child->unk48 = -12;
    child->unk49 = 4;
    ((struct Lo3Bits *)&child->flags77)->lo3 = 2;
    child->targetX = obj->baseX;
    child->targetH = child->baseH;
    child->anim = gAnim284;
    flags = &child->flags76;
    *flags = *flags | 0x40;
    child->callback = on_arrive;
    flags = &child->flags79;
    *flags = *flags | 1;
    obj->timer = 0;
    set_slot_value(0x7C, -1);
    orbit_slots_orbit_step_c(obj);
}

/* Wind-up of the second orbit move (animation 0x2054): while the button (obj->mask) is held, the
   timer counts up and shows wind-up frames 0-3 every 3 frames (frame 11 sets the 0x40000000 flag).
   At frame 12 the wind-up is complete and hold_then_orbit_b takes over; letting go earlier hands the
   timer (scaled by 3) to orbit_slots_orbit_step_d, which winds the animation back down. */
void orbit_slots_orbit_step_c(struct Actor *obj)
{
    s32 actionEnded;

    obj->state = orbit_slots_orbit_step_c;
    actionEnded = poll_action_end(obj);
    if (actionEnded == 0)
    {
        if ((s16)obj->timer == 0)
            sprite_set_action(obj, 0, 0x2054, 0);
        if ((s16)obj->timer == 3)
            sprite_set_action(obj, 0, 0x2054, 1);
        if ((s16)obj->timer == 6)
            sprite_set_action(obj, 0, 0x2054, 2);
        if ((s16)obj->timer == 9)
            sprite_set_action(obj, 0, 0x2054, 3);
        if ((s16)obj->timer == 11)
            set_cur_flags_both(0x40000000);
        if ((s16)obj->timer != 12)
        {
            if ((gSys.held & obj->mask) == 0)
            {
                /* let go early: wind back down */
                obj->timer = (s16)obj->timer * 3;
                obj->state = orbit_slots_orbit_step_d;
                return;
            }
        }
        else
        {
            /* fully wound up */
            sprite_set_action(obj, 1, 0, 0);
            ((struct PoseFlagBits *)&obj->pose->flags)->mode = 0;
            obj->timer = actionEnded;
            obj->state = hold_then_orbit_b;
        }
        ((struct PoseFlagBits *)&obj->pose->flags)->hold = 1;
        obj->timer++;
    }
}

/* Let-go state of orbit_slots_orbit_step_c's wind-up (timer = wind-up frames * 3): plays animation
   0x2054 back down, frame 2, 1 then 0 (each only if pose->count has it), as the timer counts down
   by 3 a frame. Past 0 the move is cancelled: action 4, pose mode 1 with result 0xC0 / 12, and
   end_action_if_done takes over. */
void orbit_slots_orbit_step_d(struct Actor *obj)
{
    s32 windup;
    u32 rate;

    obj->state = orbit_slots_orbit_step_d;
    if (poll_action_end(obj) != 0)
        return;
    windup = (s16)obj->timer;
    if (windup < 0)
    {
        sprite_set_action(obj, 4, 0, 0);
        ((struct PoseFlagBits *)&obj->pose->flags)->mode = 1;
        rate = MATH_ROUTINE(u32 (*)(u32, u32), iwram_div)(0xC0, 0xC);
        obj->pose->result = rate;
        ((struct PoseFlagBits *)&obj->pose->flags)->hold = 0;
        obj->state = end_action_if_done;
        return;
    }
    if (windup <= 8)
    {
        if (obj->pose->count != 0)
            sprite_set_action(obj, 0, 0x2054, 0);
    }
    else if (windup <= 17)
    {
        if (obj->pose->count > 1)
            sprite_set_action(obj, 0, 0x2054, 1);
    }
    else if (windup <= 26)
    {
        if (obj->pose->count > 2)
            sprite_set_action(obj, 0, 0x2054, 2);
    }
    ((struct PoseFlagBits *)&obj->pose->flags)->hold = 1;
    obj->timer -= 3;
}
