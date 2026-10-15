#include "global.h"

struct OwnerState
{
    u8 filler0[0x9A1];
    u8 mode9A1;
};

struct Owner
{
    u8 filler0[0x14];
    u8 *list;
    u32 type;
    u8 filler1C[0x20 - 0x1C];
    u32 aux;
    struct OwnerState *state;
    u8 *entry;
};

void clear_command_records(void);
void owner_reset(struct Owner *owner);
void clear_slot_rect(u8 *rect, u32 n);
void copy_map_rects(u8 *rects, u32 n);


ASM_FUNC("asm/code_080FA598/cmd_set_value.s", void cmd_set_value(void))

ASM_FUNC("asm/code_080FA598/cmd_slot_value_by_kind.s", void cmd_slot_value_by_kind(void))

ASM_FUNC("asm/code_080FA598/cmd_slot_op.s", void cmd_slot_op(void))

ASM_FUNC("asm/code_080FA598/cmd_slot_field.s", void cmd_slot_field(void))

ASM_FUNC("asm/code_080FA598/cmd_place_object.s", void cmd_place_object(void))

ASM_FUNC("asm/code_080FA598/cmd_start_move_list.s", void cmd_start_move_list(void))

ASM_FUNC("asm/code_080FA598/cmd_set_position.s", void cmd_set_position(void))

ASM_FUNC("asm/code_080FA598/cmd_place_sprite.s", void cmd_place_sprite(void))

ASM_FUNC("asm/code_080FA598/cmd_aux_entry.s", void cmd_aux_entry(void))

ASM_FUNC("asm/code_080FA598/cmd_latch_when_ready.s", void cmd_latch_when_ready(void))

ASM_FUNC("asm/code_080FA598/cmd_list_toggle.s", void cmd_list_toggle(void))

ASM_FUNC("asm/code_080FA598/unknown_thumb_080faf3a.s", void unknown_thumb_080faf3a(void))

ASM_FUNC("asm/code_080FA598/cmd_start_move.s", void cmd_start_move(void))

ASM_FUNC("asm/code_080FA598/cmd_list_mode.s", void cmd_list_mode(void))

ASM_FUNC("asm/code_080FA598/cmd_set_mode_1.s", void cmd_set_mode_1(void))

ASM_FUNC("asm/code_080FA598/cmd_latch_when_free.s", void cmd_latch_when_free(void))

ASM_FUNC("asm/code_080FA598/cmd_push_handle.s", void cmd_push_handle(void))

ASM_FUNC("asm/code_080FA598/cmd_set_object_flags_list.s", void cmd_set_object_flags_list(void))

ASM_FUNC("asm/code_080FA598/cmd_set_obj_flags_list2.s", void cmd_set_obj_flags_list2(void))

ASM_FUNC("asm/code_080FA598/cmd_obj_op_ae49.s", void cmd_obj_op_ae49(void))

ASM_FUNC("asm/code_080FA598/cmd_place_slot_flags.s", void cmd_place_slot_flags(void))

ASM_FUNC("asm/code_080FA598/cmd_set_pos_for_current.s", void cmd_set_pos_for_current(void))

ASM_FUNC("asm/code_080FA598/cmd_object_op_cur.s", void cmd_object_op_cur(void))

ASM_FUNC("asm/code_080FA598/cmd_place_obj_sync.s", void cmd_place_obj_sync(void))

ASM_FUNC("asm/code_080FA598/cmd_place_obj_flags3.s", void cmd_place_obj_flags3(void))

ASM_FUNC("asm/code_080FA598/cmd_obj_setup_global.s", void cmd_obj_setup_global(void))

ASM_FUNC("asm/code_080FA598/cmd_reset_next_record.s", void cmd_reset_next_record(void))

ASM_FUNC("asm/code_080FA598/cmd_set_slot_flag.s", void cmd_set_slot_flag(void))

ASM_FUNC("asm/code_080FA598/cmd_find_record.s", void cmd_find_record(void))

ASM_FUNC("asm/code_080FA598/cmd_set_next_flag4.s", void cmd_set_next_flag4(void))

ASM_FUNC("asm/code_080FA598/run_record.s", void run_record(void))

ASM_FUNC("asm/code_080FA598/owner_release_handles.s", void owner_release_handles(void))

ASM_FUNC("asm/code_080FA598/clear_command_records.s", void clear_command_records(void))

ASM_FUNC("asm/code_080FA598/tick_slot_timer.s", void tick_slot_timer(void))

ASM_FUNC("asm/code_080FA598/toggle_on_motion_done.s", void toggle_on_motion_done(void))

u8 state_mode(struct Owner *owner)
{
    return owner->state->mode9A1;
}

ASM_FUNC("asm/code_080FA598/owner_stop.s", void owner_stop(void))

ASM_FUNC("asm/code_080FA598/clear_slot_marks.s", void clear_slot_marks(void))

void unknown_thumb_080fb7b4(struct Owner *owner)
{
    clear_command_records();
    owner_reset(owner);
}

ASM_FUNC("asm/code_080FA598/begin_state_mode.s", void begin_state_mode(void))

void unknown_thumb_080fb7f8(struct Owner *owner)
{
    clear_slot_rect(owner->entry, 4);
    copy_map_rects(owner->entry, 0x10);
}

ASM_FUNC("asm/code_080FA598/owner_destroy.s", void owner_destroy(void))

ASM_FUNC("asm/code_080FA598/owner_create.s", void owner_create(void))

ASM_FUNC("asm/code_080FA598/load_object_def.s", void load_object_def(void))

ASM_FUNC("asm/code_080FA598/init_object_from_def.s", void init_object_from_def(void))
