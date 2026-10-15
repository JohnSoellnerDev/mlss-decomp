#include "global.h"

struct Actor
{
    u8 filler0[0xC];
    s32 posX;
    s32 posY;
    s32 posZ;
    s32 dz;
    u8 filler1C[0x198 - 0x1C];
    void *state;
    void *stateNext;
    void *handler;
    void *handlerNext;
    void *callback;
    void *rec;
    s16 *sensor;
    s16 vecX;
    s16 vecY;
    s16 vecZ;
    u8 filler1BE[0x1D0 - 0x1BE];
    u16 index;
    u16 timerB;
    u16 timer;
    u8 flags1D6;
    u8 filler1D7[0x1DC - 0x1D7];
    s32 homeX;
    s32 homeY;
    s32 homeZ;
    s32 homeDz;
    u8 filler1EC[0x214 - 0x1EC];
    u8 mode;
    u8 filler215[0x239 - 0x215];
    u8 byte239;
    u8 filler23A[0x23E - 0x23A];
    s16 speed2;
    u8 filler240[0x242 - 0x240];
    s16 vx;
    s16 vy;
    u8 filler246[0x248 - 0x246];
    s32 accel;
    u8 filler24C[0x274 - 0x24C];
    u16 scale;
    u8 filler276[0x294 - 0x276];
    u8 *sprite;
    u8 filler298[0x2B6 - 0x298];
    u8 flags2B6;
    u8 filler2B7[0x2D4 - 0x2B7];
    s32 tgtZ;
    u8 filler2D8[0x2DC - 0x2D8];
    s16 zspeed;
    u8 filler2DE[0x2EC - 0x2DE];
    s32 landZ;
    u8 filler2F0[0x33E - 0x2F0];
    u8 flags33E;
};

u8 step_pos_xyz(struct Actor *ent);
void enemy_hop_land_on_partner(void);

void enemy_enter_contact_state(void *owner, struct Actor *ent);

ASM_FUNC("asm/code_08142A38/enemy_start_return_home.s", void enemy_start_return_home(void))

ASM_FUNC("asm/code_08142A38/enemy_collide_then_idle.s", void enemy_collide_then_idle(void))

ASM_FUNC("asm/code_08142A38/enemy_idle_handler.s", void enemy_idle_handler(void))

ASM_FUNC("asm/code_08142A38/enemy_engage_setup.s", void enemy_engage_setup(void))

ASM_FUNC("asm/code_08142A38/enemy_charge_handler.s", void enemy_charge_handler(void))

ASM_FUNC("asm/code_08142A38/enemy_sway_then_check.s", void enemy_sway_then_check(void))

ASM_FUNC("asm/code_08142A38/contact_states_enemy_return_home.s", void contact_states_enemy_return_home(void))

ASM_FUNC("asm/code_08142A38/enemy_wait_after_step.s", void enemy_wait_after_step(void))

ASM_FUNC("asm/code_08142A38/enemy_begin_motion.s", void enemy_begin_motion(void))

ASM_FUNC("asm/code_08142A38/contact_states_enemy_reset_idle.s", void contact_states_enemy_reset_idle(void))

ASM_FUNC("asm/code_08142A38/find_touching_partner.s", void find_touching_partner(void))

ASM_FUNC("asm/code_08142A38/enemy_wait_after_step_b.s", void enemy_wait_after_step_b(void))

ASM_FUNC("asm/code_08142A38/enemy_wait_sprite_timer.s", void enemy_wait_sprite_timer(void))

ASM_FUNC("asm/code_08142A38/enemy_reset_state_b.s", void enemy_reset_state_b(void))

ASM_FUNC("asm/code_08142A38/enemy_z_step_done_wait.s", void enemy_z_step_done_wait(void))

ASM_FUNC("asm/code_08142A38/enemy_move_home_3d.s", void enemy_move_home_3d(void))

ASM_FUNC("asm/code_08142A38/enemy_wander_random_edge.s", void enemy_wander_random_edge(void))

ASM_FUNC("asm/code_08142A38/enemy_start_wander_setup.s", void enemy_start_wander_setup(void))

ASM_FUNC("asm/code_08142A38/enemy_rand_setup.s", void enemy_rand_setup(void))

ASM_FUNC("asm/code_08142A38/enemy_sensor_hit.s", void enemy_sensor_hit(void))

void enemy_sibling_noop_state(void)
{
}

ASM_FUNC("asm/code_08142A38/enemy_activate.s", void enemy_activate(void))

ASM_FUNC("asm/code_08142A38/enemy_reset_timer.s", void enemy_reset_timer(void))

ASM_FUNC("asm/code_08142A38/enemy_fall_step.s", void enemy_fall_step(void))

ASM_FUNC("asm/code_08142A38/enemy_land_reload_sprite.s", void enemy_land_reload_sprite(void))

ASM_FUNC("asm/code_08142A38/enemy_wait_after_step_c.s", void enemy_wait_after_step_c(void))

ASM_FUNC("asm/code_08142A38/enemy_after_fall_check_box.s", void enemy_after_fall_check_box(void))

ASM_FUNC("asm/code_08142A38/enemy_hop_fall_then_next.s", void enemy_hop_fall_then_next(void))

void enemy_xyz_step_done(void *unused, struct Actor *ent, void **out)
{
    if (!step_pos_xyz(ent))
    {
        *out = enemy_hop_land_on_partner;
    }
}

ASM_FUNC("asm/code_08142A38/find_partner_in_box.s", void find_partner_in_box(void))

ASM_FUNC("asm/code_08142A38/enemy_box_point_then_collide.s", void enemy_box_point_then_collide(void))

ASM_FUNC("asm/code_08142A38/enemy_dash_to_owner_target.s", void enemy_dash_to_owner_target(void))

ASM_FUNC("asm/code_08142A38/enemy_wait_sprite_flag_then_load.s", void enemy_wait_sprite_flag_then_load(void))

ASM_FUNC("asm/code_08142A38/enemy_reset_collide.s", void enemy_reset_collide(void))

void entity_mode_46_noop(void)
{
}

ASM_FUNC("asm/code_08142A38/enemy_set_default_handlers.s", void enemy_set_default_handlers(void))

ASM_FUNC("asm/code_08142A38/enemy_start_motion_if_free.s", void enemy_start_motion_if_free(void))

ASM_FUNC("asm/code_08142A38/enemy_partner_in_box_check.s", void enemy_partner_in_box_check(void))

ASM_FUNC("asm/code_08142A38/enemy_after_step_box_check.s", void enemy_after_step_box_check(void))

ASM_FUNC("asm/code_08142A38/enemy_load_frame_wait.s", void enemy_load_frame_wait(void))

ASM_FUNC("asm/code_08142A38/enemy_sprite_scale_check.s", void enemy_sprite_scale_check(void))

ASM_FUNC("asm/code_08142A38/enemy_contact_check_state.s", void enemy_contact_check_state(void))

ASM_FUNC("asm/code_08142A38/enemy_contact_state_handler.s", void enemy_contact_state_handler(void))

ASM_FUNC("asm/code_08142A38/enemy_run_handler_advance.s", void enemy_run_handler_advance(void))

ASM_FUNC("asm/code_08142A38/enemy_enter_next_state.s", void enemy_enter_next_state(void))

ASM_FUNC("asm/code_08142A38/enemy_enter_contact_state.s", void enemy_enter_contact_state(void *owner, struct Actor *ent))

ASM_FUNC("asm/code_08142A38/enemy_mark_and_enter_contact.s", void enemy_mark_and_enter_contact(void *owner, struct Actor *ent))

ASM_FUNC("asm/code_08142A38/enemy_contact_engage.s", void enemy_contact_engage(void))
