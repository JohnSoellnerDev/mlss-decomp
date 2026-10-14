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
extern struct WaveWorld *gUnk_03000FD8;
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

s32 wave_obj_dispatch_by_mode(struct WaveObj *ctl, struct WaveObj *obj)
{
    s32 mode = ctl->phase;
    s32 cursor;
    s32 sign;

    switch (mode)
    {
    case 0:
        cursor = menu_cursor_pos(obj->part->cursorX, obj->part->cursorY, 8);
        sign = signed_byte_e4(obj->part);
        popup_digit_spawn(cursor - obj->part->cursorX, obj->baseX / 0x100, obj->baseY / 0x100, sign, 1);
        obj->part->cursorX = cursor;
        effect_spawn_at(0xF93, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        break;
    case 1:
        gUnk_03000FD8->unk18 = mode;
        effect_spawn_at(0x2375, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        set_slot_value(0x2C, -1);
        break;
    case 2:
        anim_bit1_chance(obj->part, 0xC8);
        effect_spawn_at(0x2396, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        set_slot_value(0xA0, -1);
        break;
    default:
        partner_link_update(ctl, obj, 0xFF);
        effect_spawn_at(0x2381, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
        obj_list_remove(ctl);
        break;
    }
    obj_release(ctl);
    return -1;
}

void obj_index_wait_end(struct WaveObj *obj)
{
    if (obj->index == 0)
    {
        if (obj->select != 0)
        {
            sprite_set_action(obj, 0x29, 0, 0);
        }
        else
        {
            sprite_set_action(obj, 0x2A, 0, 0);
        }
        obj->state = obj_pose_action_2b;
    }
}

void obj_pose_action_27(struct WaveObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0x27, 0, 0);
        obj->timer = 1;
        obj->state = obj_counter_next_target;
    }
}

u32 obj_lookup_part_move(struct WaveObj *obj)
{
    u8 *flags;
    u32 result = lookup_link_slot(obj);

    if (result != 0)
    {
        return result;
    }
    sprite_set_action(obj, 0x20, 0, 0);
    obj->targetX = obj->part->targetX;
    obj->targetY = obj->part->targetY;
    obj->phase = obj->part->targetH;
    obj->motionSpeed = 0x180;
    flags = &obj->flags;
    *flags |= 0x20;
    obj->motionStart = result;
    start_motion(obj);
    obj->state = obj_idle_when_index_zero;
    return 0;
}

u32 obj_notify_and_show(struct WaveObj *obj, struct WaveObj *other)
{
    set_slot_value(0x12B, -1);
    partner_link_update(obj, other, 0xFF);
    effect_spawn_at(0x2012, obj->baseX / 0x100, obj->baseY / 0x100, obj->baseH / 0x100, obj);
    obj->state = obj_count_down_exit;
    return 0;
}

void obj_idle_when_index_zero(struct WaveObj *obj)
{
    if (obj->index == 0)
    {
        sprite_set_action(obj, 0, 0, 0);
        obj->state = 0;
    }
}

void obj_index_zero_count(struct WaveObj *obj)
{
    if (obj->index == 0)
    {
        obj->select++;
        obj->state = obj_setup_motion_slot;
    }
}

void obj_setup_motion_slot(struct WaveObj *obj)
{
    struct WaveObj *cand = gUnk_03000FD8->slots[6 + (obj->select & 1)];
    u8 *flags;

    obj->targetX = cand->link->targetX + 0x2000;
    obj->targetY = cand->link->targetY;
    obj->phase = obj->baseH;
    obj->motionSpeed = 0x180;
    flags = &obj->flags;
    *flags |= 0x20;
    obj->motionStart = 0;
    start_motion(obj);
    set_slot_value(0x11C, -1);
    obj->state = obj_index_action_1c;
}

void obj_random_pick_start(struct WaveObj *obj)
{
    if (obj->pose->flags & 8)
    {
        sprite_set_action(obj, 0x1B, 0, 0);
        obj->select = mt19937_next() & 1;
        gUnk_03000E3C = 0;
        obj_list_push_default(obj);
        obj->cb60 = (void (*)(struct WaveObj *))obj_lookup_part_move;
        obj->state = obj_setup_motion_slot;
    }
}

u32 obj_lookup_part_setup(struct WaveObj *obj)
{
    u8 *flags;
    u32 result = lookup_link_slot(obj);

    if (result != 0)
    {
        return result;
    }
    sound_stop_song(0x11C);
    obj->targetX = obj->part->targetX;
    obj->targetY = obj->part->targetY;
    obj->phase = obj->part->targetH;
    obj->motionSpeed = 0x280;
    flags = &obj->flags;
    *flags |= 0x20;
    obj->motionStart = result;
    start_motion(obj);
    obj->state = obj_index_action_18;
    return 0;
}

u32 obj_setup_wait_target(struct WaveObj *obj, struct WaveObj *other)
{
    partner_link_update(obj, other, 0xFF);
    obj_list_remove(obj);
    obj->x = obj->owner->part->targetX;
    sprite_set_action(obj, 8, 0, 0);
    obj->timer = 2;
    obj->state = obj_timer_end_idle;
    return 0;
}

void obj_timer_end_idle(struct WaveObj *obj)
{
    if (obj->pose->flags & 8)
    {
        obj->timer--;
        if (obj->timer <= 0)
        {
            sprite_set_action(obj, 0x15, 0, 0);
            obj->state = 0;
        }
    }
}

void obj_index_action_18(struct WaveObj *obj)
{
    if (obj->index == 0)
    {
        sprite_set_action(obj, 0x18, 0, 0);
        obj->state = obj_pose_idle_b;
    }
}
