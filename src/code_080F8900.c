#include "global.h"

struct OwnerObj
{
    u8 filler0[0x2C];
    u8 *state;
};

ASM_FUNC("asm/code_080F8900/cmd_slot_flag_20.s", void cmd_slot_flag_20(void))

ASM_FUNC("asm/code_080F8900/cmd_slot_set_fields.s", void cmd_slot_set_fields(void))

ASM_FUNC("asm/code_080F8900/obj_slot_set_fields.s", void obj_slot_set_fields(void))

ASM_FUNC("asm/code_080F8900/cmd_call_cf0.s", void cmd_call_cf0(void))

ASM_FUNC("asm/code_080F8900/cmd_obj_chain.s", void cmd_obj_chain(void))

ASM_FUNC("asm/code_080F8900/cmd_world_flag_2b4.s", void cmd_world_flag_2b4(void))

ASM_FUNC("asm/code_080F8900/cmd_key_word.s", void cmd_key_word(void))

ASM_FUNC("asm/code_080F8900/cmd_swap_slots.s", void cmd_swap_slots(void))

ASM_FUNC("asm/code_080F8900/world_cmds_cmd_world_mode.s", void world_cmds_cmd_world_mode(void))

ASM_FUNC("asm/code_080F8900/cmd_effect_mode.s", void cmd_effect_mode(void))

ASM_FUNC("asm/code_080F8900/cmd_reward_or_update.s", void cmd_reward_or_update(void))

ASM_FUNC("asm/code_080F8900/cmd_find_slot_set_var.s", void cmd_find_slot_set_var(void))

ASM_FUNC("asm/code_080F8900/cmd_slot_get_set_var.s", void cmd_slot_get_set_var(void))

ASM_FUNC("asm/code_080F8900/cmd_flag_op.s", void cmd_flag_op(void))

ASM_FUNC("asm/code_080F8900/cmd_world_flag_2f8.s", void cmd_world_flag_2f8(void))

ASM_FUNC("asm/code_080F8900/cmd_slot_refresh.s", void cmd_slot_refresh(void))

ASM_FUNC("asm/code_080F8900/cmd_slot_shift_pos.s", void cmd_slot_shift_pos(void))

ASM_FUNC("asm/code_080F8900/cmd_slot_resync_pos.s", void cmd_slot_resync_pos(void))

ASM_FUNC("asm/code_080F8900/cmd_slot_flag_copy.s", void cmd_slot_flag_copy(void))

ASM_FUNC("asm/code_080F8900/cmd_slot_set_stream.s", void cmd_slot_set_stream(void))

ASM_FUNC("asm/code_080F8900/cmd_wait_flag_copy.s", void cmd_wait_flag_copy(void))

ASM_FUNC("asm/code_080F8900/cmd_owner_flag_copy.s", void cmd_owner_flag_copy(void))

ASM_FUNC("asm/code_080F8900/cmd_restore_word0.s", void cmd_restore_word0(void))

ASM_FUNC("asm/code_080F8900/cmd_owner_set_var.s", void cmd_owner_set_var(void))

ASM_FUNC("asm/code_080F8900/cmd_owner_part_copy.s", void cmd_owner_part_copy(void))

ASM_FUNC("asm/code_080F8900/cmd_owner_part_reset.s", void cmd_owner_part_reset(void))

ASM_FUNC("asm/code_080F8900/cmd_reset_obj_part.s", void cmd_reset_obj_part(void))

ASM_FUNC("asm/code_080F8900/cmd_part_shift.s", void cmd_part_shift(void))

ASM_FUNC("asm/code_080F8900/cmd_find_part_copy.s", void cmd_find_part_copy(void))

ASM_FUNC("asm/code_080F8900/cmd_part_flag4.s", void cmd_part_flag4(void))

ASM_FUNC("asm/code_080F8900/cmd_node_stream.s", void cmd_node_stream(void))

ASM_FUNC("asm/code_080F8900/cmd_slot_clear_flag2.s", void cmd_slot_clear_flag2(void))

ASM_FUNC("asm/code_080F8900/cmd_part_scan.s", void cmd_part_scan(void))

ASM_FUNC("asm/code_080F8900/clear_node_bit9.s", void clear_node_bit9(void))

ASM_FUNC("asm/code_080F8900/slot_clear_flag800_if_idle.s", void slot_clear_flag800_if_idle(void))

ASM_FUNC("asm/code_080F8900/setup_bg_entry.s", void setup_bg_entry(void))

ASM_FUNC("asm/code_080F8900/create_text_entry.s", void create_text_entry(void))

u16 read_entry_half_1a0(struct OwnerObj *obj, u8 idx)
{
    return *(u16 *)(obj->state + ((u32)idx << 8) + 0x1A0);
}

void unknown_thumb_080f95f2(struct OwnerObj *obj, u8 value)
{
    *(u32 *)(obj->state + 0x1784) = value;
}

void sub_080F9608(struct OwnerObj *obj, u8 value)
{
    *(u32 *)(obj->state + 0x177C) = value;
}

void set_state_word_1780(struct OwnerObj *obj, u8 value)
{
    *(u32 *)(obj->state + 0x1780) = value;
}

ASM_FUNC("asm/code_080F8900/unknown_thumb_080f962c.s", void unknown_thumb_080f962c(void))

ASM_FUNC("asm/code_080F8900/sub_080F964C.s", void sub_080F964C(void))

ASM_FUNC("asm/code_080F8900/sub_080F9660.s", void sub_080F9660(void))

ASM_FUNC("asm/code_080F8900/sub_080F9674.s", void sub_080F9674(void))

ASM_FUNC("asm/code_080F8900/reset_entry_part.s", void reset_entry_part(void))

ASM_FUNC("asm/code_080F8900/reset_root_params.s", void reset_root_params(void))

ASM_FUNC("asm/code_080F8900/unknown_thumb_080f970e.s", void unknown_thumb_080f970e(void))

ASM_FUNC("asm/code_080F8900/sub_080F9738.s", void sub_080F9738(void))

ASM_FUNC("asm/code_080F8900/sub_080F9778.s", void sub_080F9778(void))

ASM_FUNC("asm/code_080F8900/sub_080F9794.s", void sub_080F9794(void))

ASM_FUNC("asm/code_080F8900/owner_step.s", void owner_step(void))

ASM_FUNC("asm/code_080F8900/unknown_thumb_080f98d8.s", void unknown_thumb_080f98d8(void))

ASM_FUNC("asm/code_080F8900/owner_init_entry.s", void owner_init_entry(void))

ASM_FUNC("asm/code_080F8900/owner_tick_records.s", void owner_tick_records(void))

ASM_FUNC("asm/code_080F8900/owner_reset.s", void owner_reset(void))

ASM_FUNC("asm/code_080F8900/slot_load_object.s", void slot_load_object(void))

ASM_FUNC("asm/code_080F8900/slot_store_object.s", void slot_store_object(void))

ASM_FUNC("asm/code_080F8900/record_advance.s", void record_advance(void))

ASM_FUNC("asm/code_080F8900/record_dispatch.s", void record_dispatch(void))
