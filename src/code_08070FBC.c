#include "global.h"

struct WavePose
{
    u8 filler0[0x11];
    u8 unk11;
    u8 flags;
};

struct WavePart
{
    u8 filler0[0xD8];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler0E4[0xEC - 0xE4];
    s16 slot;
    u8 filler0EE[0xF6 - 0xEE];
    s16 cursorX;
    s16 cursorY;
};

struct WaveObj
{
    u8 filler0[0x8];
    struct WavePose *pose;
    u8 filler0C[0x10 - 0x0C];
    s32 x;
    s32 y;
    s32 h;
    u8 filler1C[0x28 - 0x1C];
    struct WavePart *part;
    struct WaveObj *owner;
    struct WavePart *link;
    u8 filler34[0x38 - 0x34];
    s32 baseX;
    s32 baseY;
    s32 baseH;
    u8 filler44[0x4C - 0x44];
    void (*state)(struct WaveObj *);
    u8 filler50[0x58 - 0x50];
    void (*cb58)(struct WaveObj *);
    void (*cb5C)(struct WaveObj *);
    void (*cb60)(struct WaveObj *);
    u8 filler64[0x68 - 0x64];
    void (*cb68)(struct WaveObj *);
    s32 unk6C;
    u8 filler70[0x74 - 0x70];
    u8 unk74;
    u8 unk75;
    u8 filler76[0x79 - 0x76];
    u8 flags;
    s16 motionStart;
    s16 motionSpeed;
    u8 filler7E[0x7F - 0x7E];
    u8 unk7F;
    u32 index;
    s32 targetX;
    s32 targetY;
    s32 phase;
    u8 filler90[0x9C - 0x90];
    s32 counter;
    s32 param0;
    s32 param4;
    s32 select;
    s16 timer;
    s16 dx;
    s16 dy;
    s16 dh;
    u16 unkB4;
    s16 unkB6;
    u16 unkB8;
    s16 unkBA;
};

struct WaveWorld
{
    u8 filler0[0x18];
    u16 unk18;
    u8 filler1A[0x58 - 0x1A];
    struct WaveObj *slots[8];
};

void sprite_set_action(void *obj, u32 action, u32 arg, u32 extra);
void set_slot_value(u32 id, s32 value);
void sound_stop_song(u32 song);
u32 partner_link_update(struct WaveObj *obj, struct WaveObj *owner, u32 flag);
void obj_detach_active(struct WavePose *pose);
void obj_list_remove(struct WaveObj *obj);
void obj_release(void *obj);
void release_object(struct WavePart *part);
void task_list_remove(struct WavePart *part);

extern u8 gUnk_03000E7C;
extern u8 gUnk_03000E7D;

void obj_wave_left(struct WaveObj *obj);
void obj_spawn_child_arc(struct WaveObj *obj);
void obj_spawn_child_seq(struct WaveObj *obj);
void obj_spawn_child_from_table(struct WaveObj *obj);
void obj_launch_to_target(struct WaveObj *obj);
void obj_countdown_place_and_setup(struct WaveObj *obj);
void obj_timer_release(struct WaveObj *obj);
void wave_obj_timer_spawn_effect(struct WaveObj *obj);
void random_launch_step(struct WaveObj *obj);
void wait_for_index(struct WaveObj *obj);
void obj_motion_start_15(struct WaveObj *obj);
void obj_motion_start_23(struct WaveObj *obj);
void obj_motion_start_2(struct WaveObj *obj);
void obj_action5_notify46(struct WaveObj *obj);
void obj_pose_idle_b(struct WaveObj *obj);
void obj_table_step(struct WaveObj *obj);
void obj_setup_motion_slot(struct WaveObj *obj);
void place_sprite_at_base(struct WaveObj *obj);
void obj_pose_action_6_finish(struct WaveObj *obj);
void obj_counter_next_target(struct WaveObj *obj);
void begin_anim_1d_when_active(struct WaveObj *obj);
void wait_slot_then_place(struct WaveObj *obj);
void actor_speed_move_idle(struct WaveObj *obj);
void obj_action5_next(struct WaveObj *obj);
void obj_pose_action_2b(struct WaveObj *obj);
void obj_pose_idle_c(struct WaveObj *obj);
void obj_index_action_18(struct WaveObj *obj);
void obj_idle_when_index_zero(struct WaveObj *obj);
void obj_motion_start_24(struct WaveObj *obj);
void obj_motion_start_prep(struct WaveObj *obj);
void anim25_when_idle(struct WaveObj *obj);
void start_anim_16_when_idle(struct WaveObj *obj);
void actor_wave_steps(struct WaveObj *obj);
void timer_then_anim5(struct WaveObj *obj);
void obj_land_random_target(struct WaveObj *obj);
void start_motion(struct WaveObj *obj);
void timer_then_release_part(struct WaveObj *obj);
void timer_then_anim7(struct WaveObj *obj);
void obj_timer_end_idle(struct WaveObj *obj);
void obj_gravity_bounce(struct WaveObj *obj);
void obj_load_table_entry(struct WaveObj *obj);
u32 obj_lookup_part_move(struct WaveObj *obj);
void obj_start_wave_left(struct WaveObj *obj);
void obj_count_down_exit(struct WaveObj *obj);
void obj_list_push_default(struct WaveObj *obj);
void effect_spawn_at(u32 effect, s32 x, s32 y, s32 h, struct WaveObj *owner);
u32 query_contact(struct WaveObj *obj);
u32 lookup_link_slot(struct WaveObj *obj);
u32 mt19937_next(void);
u32 finish_owner_pose(struct WaveObj *obj);
u32 obj_cb_link_or_wait(struct WaveObj *obj);

extern const u8 gStepSequenceTable[];
extern u32 gUnk_03000E3C;
extern struct WaveWorld *gWorld;
u32 lookup_slot(struct WaveObj *obj);
void sprite_entry_update(struct WavePose *pose);
void wait_timer_then_move(struct WaveObj *obj);
void wait_timer_then_place(struct WaveObj *obj);
void obj_wave_right(struct WaveObj *obj);
void release_part_when_idle(struct WaveObj *obj);
void anim7_then_idle(struct WaveObj *obj);
void accelerate_height(struct WaveObj *obj);
void obj_index_action_1c(struct WaveObj *obj);
void arm_slot_then_anim3(struct WaveObj *obj);
void obj_index_wait_end(struct WaveObj *obj);
u32 arm_wait(struct WaveObj *obj);
void remove_child_and_move(struct WaveObj *obj);
struct WaveObj *obj_spawn(struct WavePart *part, u32 kind, s32 x, s32 y, s32 h, s32 flag, u32 extra);
void on_arrive(struct WaveObj *obj);
void lookup_part_b_then_step(struct WaveObj *obj);
void move_by_velocity(struct WaveObj *obj);
u32 obj_setup_and_place(struct WaveObj *obj);
void handle_group_mode(struct WaveObj *obj);
void start_move_to_part_target(struct WaveObj *obj);
s32 menu_cursor_pos(s32 x, s32 y, u32 n);
s32 signed_byte_e4(void *part);
void popup_digit_spawn(s32 a, s32 b, s32 c, s32 d, u32 e);
void anim_bit1_chance(void *part, u32 chance);
u32 query_or_arm_wait(struct WaveObj *obj);

u32 unknown_part_steps_08070fbc(struct WaveObj *obj)
{
    obj->state = obj_motion_start_23;
    return 1;
}

u32 obj_counter_init_5(struct WaveObj *obj)
{
    if (obj->counter == 0)
    {
        obj->counter = 5;
    }
    obj->state = obj_launch_to_target;
    return 1;
}

u32 unknown_part_steps_08070fe8(struct WaveObj *obj)
{
    obj->state = obj_motion_start_15;
    return 1;
}

void obj_setup_part_motion(struct WaveObj *obj)
{
    u8 *flags;
    s32 index = obj->index;

    if (index == 0)
    {
        obj->targetX = obj->owner->part->targetX + 0x3000;
        obj->targetY = obj->owner->part->targetY;
        obj->phase = obj->baseH;
        obj->motionSpeed = 0x280;
        flags = &obj->flags;
        *flags |= 0x20;
        obj->motionStart = index;
        start_motion(obj);
        obj->state = obj_motion_start_prep;
    }
}

u32 obj_lookup_slot_start(struct WaveObj *obj)
{
    u32 result = lookup_slot(obj);

    if (result == 0)
    {
        obj_list_remove(obj);
        obj->dx = obj->x / 0x100;
        obj->dy = obj->y / 0x100;
        obj->param4 = obj->part->targetX - (obj->dx << 8);
        obj->select = obj->part->targetY - (obj->dy << 8);
        obj->phase += 0x3000;
        obj->state = obj_wave_right;
        return 0;
    }
    return result;
}

u32 obj_link_owner_go(struct WaveObj *obj)
{
    partner_link_update(obj, obj->owner, 0xFF);
    obj->state = obj_wave_left;
    return 0;
}

u32 unknown_part_steps_080710e8(struct WaveObj *obj)
{
    obj->state = obj_action5_notify46;
    return 1;
}

u32 obj_slot_check_start(struct WaveObj *obj)
{
    s16 slot = obj->owner->part->slot;

    if (slot == -1)
    {
        obj->counter = 0;
    }
    else
    {
        obj->counter = 1;
    }
    obj->phase = 0x300;
    obj->state = obj_gravity_bounce;
    return 1;
}

void obj_timer_then_table_step(struct WaveObj *obj)
{
    obj->timer--;
    if (obj->timer <= 0)
    {
        obj->state = obj_table_step;
    }
}

void obj_timer_to_f870(struct WaveObj *obj)
{
    obj->timer--;
    if (obj->timer <= 0)
    {
        obj->state = obj_spawn_child_seq;
    }
}

void obj_table_step(struct WaveObj *obj)
{
    void (*next)(struct WaveObj *);

    gUnk_03000E7D++;
    if (gUnk_03000E7D > 3 || gStepSequenceTable[gUnk_03000E7C * 6 + gUnk_03000E7D] == 0xFF)
    {
        sprite_set_action(obj, 8, 0, 0);
        next = 0;
    }
    else
    {
        next = obj_load_table_entry;
    }
    obj->state = next;
}

void obj_load_table_entry(struct WaveObj *obj)
{
    obj->param4 = 0;
    obj->select = 0;
    obj->counter = gStepSequenceTable[gUnk_03000E7D + gUnk_03000E7C * 6] + 1;
    obj->state = obj_spawn_child_seq;
    set_slot_value(0xD5, -1);
}

void obj_timer_table_next(struct WaveObj *obj)
{
    void (*next)(struct WaveObj *);

    obj->timer--;
    if (obj->timer <= 0)
    {
        obj_release(obj->link);
        gUnk_03000E7D++;
        if (gUnk_03000E7D > 3 || gStepSequenceTable[gUnk_03000E7C * 6 + gUnk_03000E7D] == 0xFF)
        {
            gUnk_03000E7D = 0;
            next = obj_load_table_entry;
        }
        else
        {
            next = obj_spawn_child_from_table;
        }
        obj->state = next;
    }
}

void obj_pose_action_5(struct WaveObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 5, 0, 0);
        obj->state = obj_pose_action_6_finish;
    }
}

void obj_timer_finish(struct WaveObj *obj)
{
    obj->timer--;
    if (obj->timer <= 0)
    {
        obj_release(obj);
    }
}

void obj_pose_idle(struct WaveObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0, 0);
        obj->state = 0;
    }
}

u32 actor_wave_sequence_step(struct WaveObj *obj)
{
    gUnk_03000E7C = obj->counter;
    gUnk_03000E7D = 0;
    obj->state = obj_spawn_child_from_table;
    return 1;
}

u32 obj_slot_check_b(struct WaveObj *obj)
{
    s16 slot = obj->owner->part->slot;

    if (slot == -1)
    {
        obj->param0 = 0;
    }
    else
    {
        obj->param0 = 1;
    }
    obj->state = obj_motion_start_2;
    return 1;
}

u32 obj_random_offsets(struct WaveObj *obj)
{
    if (obj->counter == 0 && obj->param0 == 0)
    {
        obj->counter = ((mt19937_next() & 3) << 4) + 0x60;
        obj->param0 = ((mt19937_next() & 3) << 4) + 0x60;
    }
    obj->counter <<= 8;
    obj->param0 <<= 8;
    obj->state = obj_action5_next;
    return 1;
}

void obj_pose_action_2b(struct WaveObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0x2B, 0, 0);
        effect_spawn_at(0x201C, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        obj->state = obj_pose_idle_c;
    }
}

void obj_motion_start_23(struct WaveObj *obj)
{
    u8 *flags;
    sprite_set_action(obj, 0x23, 0, 0);
    obj->targetX = 0x8C << 9;
    obj->targetY = obj->baseY;
    obj->phase = obj->baseH;
    obj->motionSpeed = 0x199;
    flags = &obj->flags;
    *flags |= 0x20;
    obj->motionStart = 0;
    start_motion(obj);
    obj->state = obj_motion_start_24;
    set_slot_value(0x11C, -1);
}

void obj_index_action_1c(struct WaveObj *obj)
{
    if (obj->index == 0)
    {
        sound_stop_song(0x11C);
        sprite_set_action(obj, 0x1C, 0, 0);
        obj->state = begin_anim_1d_when_active;
    }
}

void obj_pose_idle_b(struct WaveObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0, 0);
        obj->state = 0;
    }
}

void actor_wave_steps(struct WaveObj *obj)
{
    obj->state = 0;
}

void obj_motion_start_15(struct WaveObj *obj)
{
    u8 *flags;
    sprite_set_action(obj, 0x15, 0, 0);
    obj->targetX = 0xF0 << 7;
    obj->targetY = 0xD0 << 7;
    obj->phase = obj->baseH;
    obj->motionSpeed = 0x100;
    flags = &obj->flags;
    *flags |= 0x20;
    obj->motionStart = 0;
    start_motion(obj);
    obj->state = start_anim_16_when_idle;
    set_slot_value(0x11C, -1);
}

void obj_motion_start_prep(struct WaveObj *obj)
{
    s32 index = obj->index;
    u8 *flags;

    if (index == 0)
    {
        obj_list_push_default(obj);
        obj->cb58 = (void (*)(struct WaveObj *))arm_wait;
        obj->cb5C = (void (*)(struct WaveObj *))query_or_arm_wait;
        obj->targetX = 0xFFFFE000;
        obj->targetY = obj->owner->part->targetY;
        obj->phase = obj->baseH;
        obj->motionSpeed = 0x280;
        flags = &obj->flags;
        *flags |= 0x20;
        obj->motionStart = index;
        start_motion(obj);
        obj->state = release_part_when_idle;
    }
}

void obj_action5_notify46(struct WaveObj *obj)
{
    sprite_set_action(obj, 5, 0, 0);
    set_slot_value(0x46, -1);
    obj->state = place_sprite_at_base;
}

void obj_gravity_bounce(struct WaveObj *obj)
{
    obj->h += obj->phase;
    obj->phase -= 0x80;
    if (obj->h <= 0)
    {
        obj->h = 0;
        obj->counter--;
        if (obj->counter == 0)
        {
            obj->phase = 0x300;
        }
        else
        {
            obj->state = obj_start_wave_left;
        }
    }
}

void obj_pose_action_6_finish(struct WaveObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 6, 0, 0);
        set_slot_value(0x85, -1);
        effect_spawn_at(0x2061, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        partner_link_update(obj, obj->owner, 0xFF);
        sprite_entry_update(obj->owner->pose);
        obj->owner->state = wait_for_index;
        obj->state = anim7_then_idle;
    }
}

void obj_motion_start_2(struct WaveObj *obj)
{
    u8 *flags;

    sprite_set_action(obj, 2, 0, 0);
    obj->targetX = obj->owner->part->targetX + 0x5000;
    obj->targetY = obj->owner->part->targetY;
    obj->phase = obj->baseH;
    obj->motionSpeed = 0x180;
    flags = &obj->flags;
    *flags |= 0x20;
    obj->motionStart = 0;
    start_motion(obj);
    set_slot_value(0x11B, -1);
    obj->state = arm_slot_then_anim3;
}

void obj_action5_next(struct WaveObj *obj)
{
    sprite_set_action(obj, 5, 0, 0);
    obj->state = obj_spawn_child_arc;
}

void obj_pose_idle_c(struct WaveObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0, 0, 0);
        obj->state = 0;
    }
}

void obj_motion_start_24(struct WaveObj *obj)
{
    s32 index = obj->index;
    u8 *flags;

    if (index == 0)
    {
        sprite_set_action(obj, 0x24, 0, 0);
        obj->targetX = 0xA800;
        obj->targetY = 0x6800;
        obj->phase = obj->baseH;
        obj->motionSpeed = 0x199;
        flags = &obj->flags;
        *flags |= 0x20;
        obj->motionStart = index;
        start_motion(obj);
        obj->state = anim25_when_idle;
    }
}

void begin_anim_1d_when_active(struct WaveObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0x1D, 0, 0);
        obj->timer = 5;
        obj->state = wave_obj_timer_spawn_effect;
    }
}

void start_anim_16_when_idle(struct WaveObj *obj)
{
    if (obj->index == 0)
    {
        sound_stop_song(0x11C);
        sprite_set_action(obj, 0x16, 0, 0);
        obj->timer = 4;
        obj->state = obj_timer_release;
    }
}

u32 query_or_arm_wait(struct WaveObj *obj)
{
    u32 result = query_contact(obj);

    if (result != 0)
    {
        return result;
    }
    sound_stop_song(0xA7);
    sprite_set_action(obj, 4, 0x40E7, 0);
    obj->index = result;
    obj->state = wait_slot_then_place;
    set_slot_value(0x55, -1);
    return 0;
}

u32 arm_wait(struct WaveObj *obj)
{
    sound_stop_song(0xA7);
    sprite_set_action(obj, 4, 0x40E7, 0);
    obj->index = 0;
    obj->state = wait_slot_then_place;
    set_slot_value(0x55, -1);
    return 0;
}

void release_part_when_idle(struct WaveObj *obj)
{
    if (obj->index == 0)
    {
        sound_stop_song(0xA7);
        obj_list_remove(obj);
        release_object(obj->part);
        task_list_remove(obj->part);
    }
}

void place_sprite_at_base(struct WaveObj *obj)
{
    effect_spawn_at(0x209E, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
    sprite_set_action(obj, 2, 0x40E7, 0);
    obj->counter = 0x300;
    obj->param0 = 0x33;
    obj->state = obj_land_random_target;
}

void anim7_then_idle(struct WaveObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 7, 0, 0);
        obj->state = wait_for_index;
        obj->owner->state = 0;
    }
}

void arm_slot_then_anim3(struct WaveObj *obj)
{
    if (obj->index == 0)
    {
        sound_stop_song(0x11B);
        set_slot_value(0xD2, -1);
        sprite_set_action(obj, 3, 0, 0);
        obj->timer = 10;
        obj->counter = 10;
        obj_list_push_default(obj);
        obj->cb58 = (void (*)(struct WaveObj *))finish_owner_pose;
        obj->cb60 = (void (*)(struct WaveObj *))obj_cb_link_or_wait;
        obj->state = obj_countdown_place_and_setup;
    }
}

void anim25_when_idle(struct WaveObj *obj)
{
    if (obj->index == 0)
    {
        sound_stop_song(0x11C);
        obj->phase = 0x14;
        sprite_set_action(obj, 0x25, 0, 0);
        obj->state = random_launch_step;
    }
}

void wait_slot_then_place(struct WaveObj *obj)
{
    if (obj->pose->flags & 8)
    {
        effect_spawn_at(0x207A, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        partner_link_update(obj, obj->owner, 0xFF);
        obj->timer = 5;
        obj->state = timer_then_release_part;
    }
}

u32 obj_cb_link_or_wait(struct WaveObj *obj)
{
    u32 result = lookup_link_slot(obj);

    if (result == 0)
    {
        sound_stop_song(0xD2);
        sprite_set_action(obj, 1, 0, 0);
        obj->targetX = 0x3C;
        obj->state = timer_then_anim7;
        obj->owner->state = wait_for_index;
        return 0;
    }
    return result;
}

u32 finish_owner_pose(struct WaveObj *obj)
{
    sound_stop_song(0xD2);
    if (obj->owner->cb68 != 0)
    {
        obj->owner->cb68(obj->owner);
    }
    obj->owner->state = actor_speed_move_idle;
    obj_detach_active(obj->owner->pose);
    obj->state = obj_countdown_place_and_setup;
    return 0;
}

void timer_then_anim7(struct WaveObj *obj)
{
    obj->timer--;
    if (obj->timer <= 0)
    {
        sprite_set_action(obj, 7, 0, 0);
        obj->state = wait_for_index;
    }
}

void actor_speed_move_idle(struct WaveObj *obj)
{
}

void timer_then_release_part(struct WaveObj *obj)
{
    obj->timer--;
    if (obj->timer <= 0)
    {
        release_object(obj->part);
        task_list_remove(obj->part);
    }
}

void spawn_child_and_arm(struct WaveObj *obj)
{
    struct WaveObj *child;
    u8 *flags;

    if (obj->pose->flags & 8)
    {
        child = obj_spawn(obj->part, 0x40B5, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, ((u32)obj->pose->unk11 << 25) >> 31, 1);
        obj->link = (struct WavePart *)child;
        child->unk74 = obj->unk74;
        child->unk75 = obj->unk75 - 1;
        flags = &child->flags;
        *flags |= 8;
        child->state = 0;
        sprite_set_action(obj, 2, 0x40B4, 0);
        sprite_set_action(obj->link, 2, 0, 0);
        obj_detach_active((void *)gWorld->slots[0]->x);
        obj->state = start_move_to_part_target;
    }
}

void start_move_to_part_target(struct WaveObj *obj)
{
    u8 *flags;

    if (obj->pose->flags & 8)
    {
        set_slot_value(0x6C, -1);
        sprite_set_action(obj, 3, 0x40B4, 0);
        sprite_set_action(obj->link, 3, 0, 0);
        obj->targetX = obj->owner->part->targetX + 0xA000;
        obj->targetY = obj->owner->part->targetY;
        obj->phase = obj->baseH;
        obj->motionSpeed = 0x100;
        flags = &obj->flags;
        *flags |= 0x20;
        obj->motionStart = 0;
        start_motion(obj);
        obj->timer = 8;
        obj->state = wait_timer_then_move;
    }
}

void wait_timer_then_move(struct WaveObj *obj)
{
    obj->timer--;
    if (obj->timer <= 0)
    {
        effect_spawn_at(0x21A6, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        obj->timer = 8;
    }
    if (obj->index == 0)
    {
        obj->timer = obj->counter;
        obj->state = timer_then_anim5;
    }
}
