#include "global.h"
#include "ram_routines.h"

struct ActorPose
{
    u8 filler0[0x11];
    u8 byte11;
    u8 flags;
    u8 filler13[0x21 - 0x13];
    u8 busy;
};

struct ActorPart
{
    u8 filler0[0xD8];
    s32 targetX;
    s32 targetY;
    s32 targetH;
};

struct ActorPoseFlagBits
{
    u8 bit0 : 1;
    u8 mode : 2;
    u8 high : 5;
};

struct ActorModeBits
{
    u8 low5 : 5;
    u8 bit5 : 1;
    u8 bit6 : 1;
    u8 high : 1;
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
    struct ActorPart *data;
    struct ActorObj *owner;
    struct ActorObj *child;
    u8 filler34[0x38 - 0x34];
    s32 baseX;
    s32 baseY;
    s32 baseH;
    u8 filler44[0x4C - 0x44];
    void (*state)(struct ActorObj *);
    u8 filler50[0x54 - 0x50];
    void (*savedState)(struct ActorObj *);
    void (*handler58)(struct ActorObj *);
    void (*handler5C)(struct ActorObj *);
    void (*handler60)(struct ActorObj *);
    u8 filler64[0x68 - 0x64];
    void (*handler68)(struct ActorObj *);
    s32 childValue;
    u8 filler70[0x74 - 0x70];
    u8 byte74;
    u8 filler75[0x77 - 0x75];
    u8 mode;
    u8 dir;
    u8 flags;
    s16 motionStart;
    s16 motionSpeed;
    u16 progress;
    s32 index;
    s32 targetX;
    s32 targetY;
    s32 targetH;
    u8 filler90[0x94 - 0x90];
    s32 dhStep;
    s32 ground;
    s32 param9C;
    s32 paramA0;
    s32 paramA4;
    s32 spawnLeft;
    u16 timer;
    s16 dx;
    s16 dy;
    s16 dh;
};

u32 is_state_busy(void);
u32 mt19937_next(void);
u32 sub_081DE850(u32 n, u32 r);
void sprite_set_action(struct ActorObj *obj, u32 action, u32 a2, u32 a3);
void set_slot_value(u32 id, s32 value);
void obj_list_push_default(struct ActorObj *obj);
void obj_wait_target_h800_motion(struct ActorObj *obj);
void obj_target_motion_action8_notify(struct ActorObj *obj);
void obj_list_remove(struct ActorObj *obj);
void start_motion(struct ActorObj *obj);
void obj_release(struct ActorObj *obj);
void offset_three_words_camera(s32 *a, s32 *b, s32 *c, u32 d);
u32 jump_velocity(s16 *dx, s16 *dy, s16 *dh, s32 dX, s32 dY, s32 dH, s32 a, s32 b, s32 c, s32 d);
void obj_spawn_aimed_child_pair(struct ActorObj *obj);
void move_step(s16 *x, s16 *y, s16 *h, s32 dX, s32 dY, s32 dH, s32 tempo);
struct ActorObj *obj_spawn(struct ActorPart *part, u32 type, s32 x, s32 y, s32 h, u32 a, u32 b);
void obj_begin_fall_to_owner_alt(struct ActorObj *obj);
void obj_slot_found_recoil_kids(struct ActorObj *obj);
void obj_pose_active_idle_arrive(struct ActorObj *obj);
void sound_stop_song(u32 song);
void on_arrive(struct ActorObj *obj);
void wait_for_index(struct ActorObj *obj);
u32 partner_link_update(struct ActorObj *obj, u32 partner, u32 mode);
u32 lookup_part(struct ActorObj *obj, u32 a, u32 b);
u32 lookup_link_slot(struct ActorObj *obj, u32 a, u32 b);
void release_object_data(struct ActorPart *data);

void obj_link_partner_span_start(struct ActorObj *obj);
void obj_anim3_timer24_then_next(struct ActorObj *obj);
void obj_list_setup_anim7(struct ActorObj *obj);
u32 obj_cb_lookup_link_slot(struct ActorObj *obj, u32 a, u32 b);
u32 part_lookup_clear_81(struct ActorObj *obj, u32 a, u32 b);
u32 part_unlink_clear_81(struct ActorObj *obj, u32 a, u32 b);
void obj_begin_part_to_owner_x52(struct ActorObj *obj);
void obj_settle_step_roll_bounces(struct ActorObj *obj);
void obj_settle_wait_bounce(struct ActorObj *obj);
void obj_settle_step_land_then_bce4(struct ActorObj *obj);
void obj_wait_timer_hop_then_bd88(struct ActorObj *obj);
void obj_settle_step_land_then_be2c(struct ActorObj *obj);
void obj_wait_timer_release_hop_then_bed0(struct ActorObj *obj);
void obj_settle_step_land_arrive(struct ActorObj *obj);
void obj_settle_step(struct ActorObj *obj);
void obj_begin_settle(struct ActorObj *obj);
u32 part_step_obj_wait_free_then_ba20(struct ActorObj *obj);
void obj_wait_free_then_ba20(struct ActorObj *obj);
void obj_wait_then_follow_owner(struct ActorObj *obj);
void obj_wait_then_follow_owner_speed(struct ActorObj *obj);
void obj_wait_index_then_move_to_data(struct ActorObj *obj);
void obj_drop_until_ground(struct ActorObj *obj);
void obj_wait_then_move_to_edge(struct ActorObj *obj);
void obj_wait_then_place_at_owner(struct ActorObj *obj);
void obj_setup_move_to_data(struct ActorObj *obj);
u32 obj_await_part(struct ActorObj *obj, u32 a, u32 b);
void obj_await_part_done(struct ActorObj *obj);
void obj_wait_index_release_11a(struct ActorObj *obj);
void obj_wait_index_action3_timer(struct ActorObj *obj);
u32 sprite_state_chain_a(struct ActorObj *obj);
u32 sprite_state_chain_b(struct ActorObj *obj);
u32 sprite_state_random_chain_a(struct ActorObj *obj);
void obj_wait_index_pose_toggle(struct ActorObj *obj);
void obj_wait_free_then_begin_drop(struct ActorObj *obj);
void obj_wait_free_then_action2_slot(struct ActorObj *obj);
void obj_begin_drop(struct ActorObj *obj);
void obj_action2_slot_then_wait(struct ActorObj *obj);
void obj_random_move_start(struct ActorObj *obj);
void obj_wait_then_move_screen(struct ActorObj *obj);
void obj_wait_index_then_slot84(struct ActorObj *obj);
void obj_wait_index_release_data(struct ActorObj *obj);
u32 sprite_state_step_b(struct ActorObj *obj);
void obj_timer_compute_step(struct ActorObj *obj);
void move_states_obj_move_toward_target(struct ActorObj *obj);
void obj_pose_active_attach_unlink(struct ActorObj *obj);
void obj_wait_index_then_release_move(struct ActorObj *obj);
void obj_wait_index_go_idle(struct ActorObj *obj);
u32 obj_unlink_part(struct ActorObj *obj, u32 a, u32 b);
void obj_pose_active_release_move(struct ActorObj *obj);
void actor_return_to_base_steps(struct ActorObj *obj);
u32 sprite_state_chain_c(struct ActorObj *obj);
u32 sub_080DCCA8(struct ActorObj *obj);
u32 sprite_state_random_chain_b(struct ActorObj *obj);
void obj_wait_free_then_move_d(struct ActorObj *obj);
void obj_wait_index_pose_update(struct ActorObj *obj);
void obj_wait_free_then_cd90(struct ActorObj *obj);
void obj_move_to_owner_x52(struct ActorObj *obj);
void obj_move_to_owner_x32_slot(struct ActorObj *obj);
void obj_wait_index_action7(struct ActorObj *obj);
void obj_wait_index_action3_pose(struct ActorObj *obj);
void move_states_obj_timer_action8(struct ActorObj *obj);
void obj_pose_active_action4(struct ActorObj *obj);
void obj_screen_cull_step(struct ActorObj *obj);
void obj_drop_land_stop(struct ActorObj *obj);
void obj_drop_clamp_to_limit(struct ActorObj *obj);

ASM_FUNC("asm/code_080DD090/move_states_obj_pose_active_spawn_child.s", void move_states_obj_pose_active_spawn_child(void))
