#include "global.h"

struct Obj
{
    u8 filler0[0x12];
    u8 status;
    u8 filler13[0x1F - 0x13];
    u8 unk1F;
    u8 unk20;
    u8 unk21;
    u8 filler22[0xA0 - 0x22];
    u16 unkA0;
};

struct Slot
{
    u8 filler0[0x7F];
    u8 unk7F : 7;
    u8 flag7F7 : 1;
    u8 flag80 : 1;
    u8 unk80 : 7;
    u8 filler81[0xC6 - 0x81];
    s16 unkC6;
};

struct World
{
    u8 filler0[0x58];
    struct Slot *slots[8];
};

struct OwnerState
{
    u8 filler0[0x179C];
    u8 slotCode;
};

struct Owner
{
    u8 filler0[0x2C];
    struct OwnerState *state;
};

extern struct World *gWorld;
extern u8 *gWorkBuf;

void slot_set_value(struct Owner *owner, u32 idx, s32 value, u32 flag);
void slot_set_vec(u32 idx, s32 x, s32 y, s32 z, u32 flag);
void slot_start_motion(u32 idx, u32 flag);
void slot_set_pos_rel(struct Owner *owner, u32 idx, s32 x, s32 y, s32 z);
void slot_set_pos_abs(struct Owner *owner, u32 idx, s32 x, s32 y, s32 z);
void refresh_slot_view(struct Owner *owner, s32 idx);
u32 flags_vars_set_obj_state(void *root, void *dst, struct Obj *obj, u32 state, u32 zero);
void sprite_set_action(void *obj, u32 a, u32 b, u32 c);
void slot_reload_record(struct Owner *owner, s32 idx, s32 value);
void slot_set_flag6(struct Owner *owner, s32 idx, s32 flag);
void obj_detach_active(void *obj);
void slot_set_bias(struct Owner *owner, u32 idx, s32 value);

ASM_FUNC("asm/code_080F3F2C/cmd_reset_slot_node.s", void cmd_reset_slot_node(void))

u32 cmd_copy_if_active(struct Owner *owner, u32 *dst, u32 *block)
{
    u8 idx;
    struct Slot *p;
    u8 *entry;
    u32 cond;
    u32 active;
    u32 cmp;
    if (owner->state->slotCode <= 6)
        idx = owner->state->slotCode - 1;
    else
        idx = owner->state->slotCode - 7;
    cond = *block++;
    if (cond == 0)
    {
        p = gWorld->slots[idx];
        entry = (u8 *)p + 8;
        active = 0;
        if (p->unkC6 != 0 || (p->flag7F7 & p->flag80) != 0)
        {
            if (((struct Obj *)*(u32 *)(entry + 8))->unk21 != 0)
                active = 1;
        }
        cmp = active;
    }
    if (cmp == *block)
        *dst = block[1];
    return 1;
}

ASM_FUNC("asm/code_080F3F2C/entry_flag_command.s", void entry_flag_command(void))

ASM_FUNC("asm/code_080F3F2C/entry_value_command.s", void entry_value_command(void))

ASM_FUNC("asm/code_080F3F2C/entry_mode_command.s", void entry_mode_command(void))

ASM_FUNC("asm/code_080F3F2C/entry_flag_store.s", void entry_flag_store(void))

ASM_FUNC("asm/code_080F3F2C/owner_entry_cmds_cmd_slot_set_motion.s", void owner_entry_cmds_cmd_slot_set_motion(void))

ASM_FUNC("asm/code_080F3F2C/cmd_slot_set_position.s", void cmd_slot_set_position(void))

ASM_FUNC("asm/code_080F3F2C/cmd_slot_set_step.s", void cmd_slot_set_step(void))

ASM_FUNC("asm/code_080F3F2C/cmd_copy_slot_flag.s", void cmd_copy_slot_flag(void))

ASM_FUNC("asm/code_080F3F2C/cmd_slot_restart.s", void cmd_slot_restart(void))

ASM_FUNC("asm/code_080F3F2C/cmd_slot_set_object_state.s", void cmd_slot_set_object_state(void))

ASM_FUNC("asm/code_080F3F2C/cmd_slot_set_flags.s", void cmd_slot_set_flags(void))

ASM_FUNC("asm/code_080F3F2C/cmd_slot_apply_state.s", void cmd_slot_apply_state(void))

ASM_FUNC("asm/code_080F3F2C/cmd_slot_reset_record.s", void cmd_slot_reset_record(void))

ASM_FUNC("asm/code_080F3F2C/cmd_slot_set_record_state.s", void cmd_slot_set_record_state(void))

ASM_FUNC("asm/code_080F3F2C/cmd_slot_reload_object.s", void cmd_slot_reload_object(void))

ASM_FUNC("asm/code_080F3F2C/cmd_slot_set_cmd_state.s", void cmd_slot_set_cmd_state(void))

ASM_FUNC("asm/code_080F3F2C/cmd_slot_reload_flagged.s", void cmd_slot_reload_flagged(void))

ASM_FUNC("asm/code_080F3F2C/cmd_slot_mode_owner.s", void cmd_slot_mode_owner(void))

ASM_FUNC("asm/code_080F3F2C/cmd_slot_mode_cmd.s", void cmd_slot_mode_cmd(void))

ASM_FUNC("asm/code_080F3F2C/cmd_slot_copy_tile.s", void cmd_slot_copy_tile(void))

ASM_FUNC("asm/code_080F3F2C/cmd_set_var_distance.s", void cmd_set_var_distance(void))

ASM_FUNC("asm/code_080F3F2C/cmd_set_var_from_source.s", void cmd_set_var_from_source(void))
