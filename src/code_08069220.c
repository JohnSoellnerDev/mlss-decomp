#include "global.h"
#include "ram_routines.h"

struct ActorPose
{
    u8 filler0[0x11];
    u8 unk11;
    u8 flags;
};

struct ActorPart
{
    u8 filler0[0xD8];
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler0E4[0xF6 - 0xE4];
    s16 unkF6;
    u8 filler0F8[0x111 - 0xF8];
    u8 flags111;
};

struct ActorCands
{
    u8 filler0[0x70];
    u8 *cand70;
    u8 *cand74;
};

struct PoseModeBits
{
    u8 unk0 : 1;
    u8 mode : 2;
    u8 rest : 5;
};

struct ActorFlags79Bits
{
    u8 bit0 : 1;
    u8 low : 2;
    u8 bit3 : 1;
    u8 bit4 : 1;
    u8 bit5 : 1;
    u8 high : 2;
};

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
    void (*callback)(struct ActorObj *);
    s32 link;
    u8 filler70[0x74 - 0x70];
    u8 byte74;
    u8 filler75[0x77 - 0x75];
    u8 flags77;
    u8 flags78;
    u8 flags79;
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

// The spawned child takes over two flag bits of its parent.
static inline void obj_child_flags(struct ActorObj *obj, struct ActorObj *child)
{
    ((struct ObjFlags77 *)child)->bit6 = ((struct ObjFlags77 *)obj)->bit6;
    ((struct ObjFlags77 *)child)->bit5 = ((struct ObjFlags77 *)obj)->bit5;
}

void sprite_set_action(struct ActorObj *obj, u32 action, u32 a2, u32 a3);
void set_slot_value(u32 id, s32 value);
void sound_stop_song(u32 song);
void obj_release(struct ActorObj *obj);
void release_object(void *part);
void task_list_remove(void *part);
void obj_list_remove(struct ActorObj *obj);
void obj_list_push_default(struct ActorObj *obj);
void start_motion(struct ActorObj *obj);
void wait_for_index(struct ActorObj *obj);
void effect_spawn_at(u32 type, s32 x, s32 y, s32 h, struct ActorObj *owner);
u32 effect_from_table_obj(struct ActorObj *obj, u32 id);
u32 partner_link_update(struct ActorObj *obj, struct ActorObj *partner, u32 mode);
u32 lookup_part(struct ActorObj *obj);
u32 lookup_slot(struct ActorObj *obj);
u32 is_state_busy(struct ActorObj *obj);
u32 mt19937_next(void);
void obj_spawn_child_412b(struct ActorObj *obj);
void obj_pick_random_target(struct ActorObj *obj);
void obj_index_zero_idle(struct ActorObj *obj);
void on_arrive(struct ActorObj *obj);
void obj_find_slot(struct ActorObj *obj);
void jump_obj_fall_step(struct ActorObj *obj);
void obj_jump_arc_land(struct ActorObj *obj);
struct ActorObj *obj_spawn(struct ActorPart *part, u32 type, s32 x, s32 y, s32 h, u32 a, u32 b);
extern u32 gUnk_03000E3C;
extern struct ActorCands *gWorld;
void obj_link_owner_then_wait(struct ActorObj *obj);
void obj_query_part_then_idle(struct ActorObj *obj);

void obj_wait_child_then_place(struct ActorObj *obj);
void obj_timer_wave_state(struct ActorObj *obj);
void actor_start_pair(struct ActorObj *obj);
void sprite_drop_left(struct ActorObj *obj);
void sprite_slide_left(struct ActorObj *obj);
void actor_slide_left_to_target(struct ActorObj *obj);
void actor_wait_child_then_move(struct ActorObj *obj);
void actor_timed_restart(struct ActorObj *obj);
void actor_announce_then_act(struct ActorObj *obj);
u32 unknown_part_steps_080691a8(struct ActorObj *obj);
u32 actor_init_mode0(struct ActorObj *obj);
void actor_setup_part_wait(struct ActorObj *obj);
void actor_begin_action_c_or_f(struct ActorObj *obj);
void part_drift_right(struct ActorObj *obj);
void actor_wait_then_action_d(struct ActorObj *obj);
void actor_drop_to_ground(struct ActorObj *obj);
void actor_scroll_out_left(struct ActorObj *obj);
void actor_move_to_part_x(struct ActorObj *obj);
void actor_idle_reset(struct ActorObj *obj);
void actor_start_countdown(struct ActorObj *obj);
u32 actor_init_random_wait_mode1(struct ActorObj *obj);
u32 actor_init_random_wait_mode0(struct ActorObj *obj);
void actor_countdown_then_act(struct ActorObj *obj);
void actor_wait_ready_then_jump(struct ActorObj *obj);
void actor_start_entry(struct ActorObj *obj);
u32 actor_link_owner_scroll(struct ActorObj *obj);
u32 actor_lookup_part_then_idle(struct ActorObj *obj);
void actor_slide_left_to_part_x(struct ActorObj *obj);
void actor_fire_event_idle(struct ActorObj *obj);
void actor_action_b_idle(struct ActorObj *obj);
void actor_land_and_move(struct ActorObj *obj);
void actor_start_motion_wait(struct ActorObj *obj);
void actor_wait_index_action6(struct ActorObj *obj);
void actor_link_partners_jump(struct ActorObj *obj);
void actor_fall_place_sprite(struct ActorObj *obj);
u32 actor_lookup_slot_wait(struct ActorObj *obj);
void actor_timeout_release(struct ActorObj *obj);
void actor_wait_index_action4(struct ActorObj *obj);
u32 unknown_part_steps_08069c48(struct ActorObj *obj);
u32 actor_init_pose_wait(struct ActorObj *obj);
void actor_action9_idle(struct ActorObj *obj);
void actor_wait_ready_then_fly(struct ActorObj *obj);
void actor_fly_then_land_ext(struct ActorObj *obj);
void actor_move_countdown_next(struct ActorObj *obj);
void actor_wait_ready_show_sprite(struct ActorObj *obj);
void actor_spawn_child_4072(struct ActorObj *obj);
void actor_aim_and_count(struct ActorObj *obj);
void actor_jump_to_target_a(struct ActorObj *obj);
void actor_jump_to_target_b(struct ActorObj *obj);
void obj_jump_arc(struct ActorObj *obj);

void actor_begin_action_c_or_f(struct ActorObj *obj)
{
    if (obj->pose->flags & 8)
    {
        if (obj->paramA8 == 0)
        {
            sprite_set_action(obj, 0xC, 0, 0);
            obj->timer = 1;
        }
        else
        {
            sprite_set_action(obj, 0xF, 0, 0);
            obj->timer = 1;
        }
        obj->state = actor_wait_then_action_d;
    }
}
