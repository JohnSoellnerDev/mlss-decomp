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
    u8 fillerA2[0xFD - 0xA2];
    u8 unkFD;
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
    u8 filler78[4];
    u8 *cur;
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

struct Cmd
{
    u32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
};

struct Entry
{
    u8 filler0[8];
    struct Obj *obj8;
    u8 filler0C[0x7A - 0x0C];
    u16 unk7A;
    u16 unk7C;
    u8 filler7E[0x90 - 0x7E];
    u16 unk90;
    u8 filler92[0x94 - 0x92];
    s32 unk94;
};

struct StatusBits
{
    u8 lo4 : 4;
    u8 bit4 : 1;
    u8 hi3 : 3;
};

extern struct World *gWorld;
extern u8 *gWorkBuf;

void set_var(u8 *mid, u8 *low, u8 *flagsA, u8 *flagsB, s16 index, u32 value);
u32 world_frame_index(struct Owner *owner);
u32 world_frame_index_b(struct Owner *owner);

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
void slot_read_pos(struct Owner *owner, s32 idx, s16 *x, s16 *y, s16 *z);
void apply_pose_to_parts(void *obj);

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
u32 owner_entry_cmds_cmd_slot_set_motion(struct Owner *owner, struct Obj *obj, struct Cmd *cmd)
{
    u8 idx;
    u32 flag;
    s32 value;
    if (owner->state->slotCode <= 6)
        idx = owner->state->slotCode - 1;
    else
        idx = owner->state->slotCode - 7;
    if (cmd->unk0 != 0)
    {
        cmd->unk8 = cmd->unk8 << 4;
        cmd->unkC = cmd->unkC << 4;
        cmd->unk10 = cmd->unk10 << 4;
    }
    value = cmd->unk14;
    flag = 0;
    if (((cmd->unk4 >> 1) & 3) == 1)
        flag = 1;
    slot_set_value(owner, idx, value, flag);
    slot_set_vec(idx, cmd->unk8, cmd->unkC, cmd->unk10, ((u8)cmd->unk4 ^ 1) & 1);
    slot_start_motion(idx, 1);
    obj->unkA0 |= 2;
    return 0;
}

u32 cmd_slot_set_position(struct Owner *owner, u32 unused, struct Cmd *cmd)
{
    u8 idx;
    idx = owner->state->slotCode <= 6 ? owner->state->slotCode - 1 : owner->state->slotCode - 7;
    if ((cmd->unk0 & 2) != 0)
    {
        cmd->unk4 = cmd->unk4 << 4;
        cmd->unk8 = cmd->unk8 << 4;
        cmd->unkC = cmd->unkC << 4;
        if ((((u8)cmd->unk0 ^ 1) & 1) != 0)
        {
            cmd->unk4 += 8;
            cmd->unk8 += 8;
        }
    }
    if ((cmd->unk0 & 1) != 0)
        slot_set_pos_rel(owner, idx, cmd->unk4, cmd->unk8, cmd->unkC);
    else
        slot_set_pos_abs(owner, idx, cmd->unk4, cmd->unk8, cmd->unkC);
    return 0;
}

ASM_FUNC("asm/code_080F3F2C/cmd_slot_set_step.s", void cmd_slot_set_step(void))

ASM_FUNC("asm/code_080F3F2C/cmd_copy_slot_flag.s", void cmd_copy_slot_flag(void))

ASM_FUNC("asm/code_080F3F2C/cmd_slot_restart.s", void cmd_slot_restart(void))


NONMATCH("asm/code_080F3F2C/cmd_slot_set_object_state.s", u32 cmd_slot_set_object_state(struct Owner *owner, void *dst, struct Cmd *cmd))
{
    u32 idx;
    u32 code;
    u16 shade;
    struct Entry *entry;
    struct Obj *inner;
    code = owner->state->slotCode;
    if (code <= 6)
        code = code - 1;
    else
        code = code - 7;
    idx = (u8)code;
    refresh_slot_view(owner, idx);
    if (idx <= 7)
        entry = ((struct Entry *)((u8 *)gWorld->slots[idx] + 8));
    else
        entry = (struct Entry *)(gWorkBuf + 0xD0 * idx + 0x300);
    inner = entry->obj8;
    if (inner != 0)
        shade = (s8)inner->unk20;
    else
        shade = 0x10;
    if (idx <= 7)
        entry = ((struct Entry *)((u8 *)gWorld->slots[idx] + 8));
    else
        entry = (struct Entry *)(gWorkBuf + 0xD0 * idx + 0x300);
    sprite_set_action(entry, cmd->unk4, cmd->unk0, 0);
    slot_set_flag6(owner, idx, *(s8 *)((u8 *)cmd + 8));
    if (idx <= 7)
        entry = ((struct Entry *)((u8 *)gWorld->slots[idx] + 8));
    else
        entry = (struct Entry *)(gWorkBuf + 0xD0 * idx + 0x300);
    inner = entry->obj8;
    inner->unk20 = shade;
    inner->status &= ~0x10;
    return (u8)flags_vars_set_obj_state((u8 *)owner + 0x1C, dst, inner, *(u8 *)((u8 *)cmd + 0xC), 0);
}
END_NONMATCH

NONMATCH("asm/code_080F3F2C/cmd_slot_set_flags.s", u32 cmd_slot_set_flags(struct Owner *owner, void *unused, struct Cmd *cmd))
{
    u32 idx;
    u32 code;
    u16 shade;
    struct Entry *entry;
    struct Obj *inner;
    code = owner->state->slotCode;
    if (code <= 6)
        code = code - 1;
    else
        code = code - 7;
    refresh_slot_view(owner, idx = (u8)code);
    if (idx <= 7)
        entry = ((struct Entry *)((u8 *)gWorld->slots[idx] + 8));
    else
        entry = (struct Entry *)(gWorkBuf + 0xD0 * idx + 0x300);
    inner = entry->obj8;
    if (inner != 0)
        shade = (s8)inner->unk20;
    else
        shade = 0x10;
    if (idx <= 7)
        entry = ((struct Entry *)((u8 *)gWorld->slots[idx] + 8));
    else
        entry = (struct Entry *)(gWorkBuf + 0xD0 * idx + 0x300);
    sprite_set_action(entry, cmd->unk4, cmd->unk0, 0);
    slot_reload_record(owner, idx, *(s16 *)((u8 *)cmd + 8));
    slot_set_flag6(owner, idx, *(s8 *)((u8 *)cmd + 0xC));
    if (idx <= 7)
        entry = ((struct Entry *)((u8 *)gWorld->slots[idx] + 8));
    else
        entry = (struct Entry *)(gWorkBuf + 0xD0 * idx + 0x300);
    inner = entry->obj8;
    inner->unk20 = shade;
    inner->status |= 0x10;
    return 1;
}
END_NONMATCH

ASM_FUNC("asm/code_080F3F2C/cmd_slot_apply_state.s", void cmd_slot_apply_state(void))


ASM_FUNC("asm/code_080F3F2C/cmd_slot_reset_record.s", void cmd_slot_reset_record(void))


ASM_FUNC("asm/code_080F3F2C/cmd_slot_set_record_state.s", void cmd_slot_set_record_state(void))
NONMATCH("asm/code_080F3F2C/cmd_slot_reload_object.s", u32 cmd_slot_reload_object(struct Owner *owner, void *unused, struct Cmd *cmd))
{
    struct Entry *entry;
    struct Obj *inner;
    u16 shade;
    refresh_slot_view(owner, *(s16 *)cmd);
    if ((s32)cmd->unk0 <= 7)
        entry = ((struct Entry *)((u8 *)gWorld->slots[cmd->unk0] + 8));
    else
        entry = (struct Entry *)(gWorkBuf + 0xD0 * cmd->unk0 + 0x300);
    inner = entry->obj8;
    if (inner != 0)
        shade = (s8)inner->unk20;
    else
        shade = 0x10;
    if ((s32)cmd->unk0 <= 7)
        entry = ((struct Entry *)((u8 *)gWorld->slots[cmd->unk0] + 8));
    else
        entry = (struct Entry *)(gWorkBuf + 0xD0 * cmd->unk0 + 0x300);
    sprite_set_action(entry, cmd->unk8, cmd->unk4, 0);
    slot_reload_record(owner, *(s16 *)cmd, *(s16 *)((u8 *)cmd + 0xC));
    slot_set_flag6(owner, *(s16 *)cmd, *(s8 *)((u8 *)cmd + 0x10));
    if ((s32)cmd->unk0 <= 7)
        entry = ((struct Entry *)((u8 *)gWorld->slots[cmd->unk0] + 8));
    else
        entry = (struct Entry *)(gWorkBuf + 0xD0 * cmd->unk0 + 0x300);
    inner = entry->obj8;
    inner->unk20 = shade;
    ((struct StatusBits *)&inner->status)->bit4 = 1;
    return 1;
}
END_NONMATCH

NONMATCH("asm/code_080F3F2C/cmd_slot_set_cmd_state.s", u32 cmd_slot_set_cmd_state(struct Owner *owner, u8 *dst, struct Cmd *cmd))
{
    struct Entry *entry;
    struct Obj *inner;
    u32 hi;
    if (cmd->unkC != 0)
    {
        ((struct Obj *)dst)->unkA0 |= 0x800;
        hi = cmd->unk0 << 4;
        ((struct Obj *)dst)->unkFD = (((struct Obj *)dst)->unkFD & 0xF) | hi;
    }
    dst = (u8 *)owner->state + 0x700 + (cmd->unk0 << 8);
    refresh_slot_view(owner, (s16)cmd->unk0);
    if ((s32)cmd->unk0 <= 7)
        entry = ((struct Entry *)((u8 *)gWorld->slots[cmd->unk0] + 8));
    else
        entry = (struct Entry *)(gWorkBuf + 0xD0 * cmd->unk0 + 0x300);
    sprite_set_action(entry, cmd->unk4, 0, 0);
    slot_set_flag6(owner, *(s16 *)cmd, *(s8 *)((u8 *)cmd + 8));
    if ((s32)cmd->unk0 <= 7)
        entry = ((struct Entry *)((u8 *)gWorld->slots[cmd->unk0] + 8));
    else
        entry = (struct Entry *)(gWorkBuf + 0xD0 * cmd->unk0 + 0x300);
    inner = entry->obj8;
    ((struct StatusBits *)&inner->status)->bit4 = 0;
    return (u8)flags_vars_set_obj_state((u8 *)owner + 0x1C, dst, inner, *(u8 *)((u8 *)cmd + 0xC), 0);
}
END_NONMATCH

NONMATCH("asm/code_080F3F2C/cmd_slot_reload_flagged.s", u32 cmd_slot_reload_flagged(struct Owner *owner, void *unused, struct Cmd *cmd))
{
    s32 idx;
    struct Entry *entry;
    refresh_slot_view(owner, *(s16 *)cmd);
    idx = cmd->unk0;
    if (idx <= 7)
        entry = ((struct Entry *)((u8 *)gWorld->slots[idx] + 8));
    else
        entry = (struct Entry *)(gWorkBuf + 0xD0 * idx + 0x300);
    sprite_set_action(entry, cmd->unk4, 0, 0);
    slot_reload_record(owner, *(s16 *)cmd, *(s16 *)((u8 *)cmd + 8));
    slot_set_flag6(owner, *(s16 *)cmd, *(s8 *)((u8 *)cmd + 0xC));
    idx = cmd->unk0;
    if (idx <= 7)
        entry = ((struct Entry *)((u8 *)gWorld->slots[idx] + 8));
    else
        entry = (struct Entry *)(gWorkBuf + 0xD0 * idx + 0x300);
    ((struct StatusBits *)&entry->obj8->status)->bit4 = 1;
    return 1;
}
END_NONMATCH

ASM_FUNC("asm/code_080F3F2C/cmd_slot_mode_owner.s", void cmd_slot_mode_owner(void))
ASM_FUNC("asm/code_080F3F2C/cmd_slot_mode_cmd.s", void cmd_slot_mode_cmd(void))
ASM_FUNC("asm/code_080F3F2C/cmd_slot_copy_tile.s", void cmd_slot_copy_tile(void))

ASM_FUNC("asm/code_080F3F2C/cmd_set_var_distance.s", void cmd_set_var_distance(void))
u32 cmd_set_var_from_source(struct Owner *owner, u8 *dst, struct Cmd *cmd, u8 *extra)
{
    s32 value;

    switch (cmd->unk4)
    {
    case 0:
        value = *((u8 *)gWorld + 0x2BB);
        break;
    case 1:
        {
            s16 *p = (s16 *)((u8 *)gWorld->cur + 0xEC);
            if (*p >= 0)
                value = *p;
            else
                value = 5 - *p;
        }
        break;
    case 2:
        if (*(s16 *)((u8 *)gWorld->cur + 0xEC) < 0)
            value = ((u32)*((u8 *)gWorld->cur + 0x110) << 27) >> 27;
        else
            value = -1;
        break;
    case 3:
        if (*(u8 **)((u8 *)gWorld->cur + 0x34) != 0)
            value = *(*(u8 **)(*(u8 **)((u8 *)gWorld->cur + 0x34) + 0x28) + 0xF4);
        else
            value = -1;
        break;
    case 4:
        value = *((u8 *)gWorld->cur + 0xF4);
        break;
    case 5:
        value = *(u16 *)((u8 *)gWorld + 0x2B6);
        break;
    case 6:
        value = world_frame_index(owner);
        break;
    case 7:
        value = world_frame_index_b(owner);
        break;
    case 8:
        value = *((u8 *)gWorld->cur + 0x113);
        break;
    case 9:
        value = ((u32)*((u8 *)gWorld + 0x2C1) << 30) >> 31;
        break;
    }
    set_var(extra, dst + 0x18, (u8 *)owner->state + 0x1794, dst + 0xA8, *(s16 *)cmd, value);
    return 1;
}
