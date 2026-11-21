#include "global.h"
#include "ram_routines.h"

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
    u8 filler80[0x278 - 0x80];
    struct Entry *entry278;
    u8 filler27C[0x284 - 0x27C];
    struct Entry *entries284[2];
    u8 filler28C[0x368 - 0x28C];
    struct Obj *obj368;
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

/* Arguments of cmd_slot_set_flags. */
struct SlotActionCmd
{
    u32 anim;
    s32 action;
    s16 record;
    u8 filler0A[0x0C - 0x0A];
    s8 flag6;
};

/* Arguments of cmd_slot_reset_record. */
struct SlotRecordCmd
{
    u32 action;     /* SLOT_ACTION_CURRENT: the object's current action (Obj.unk21) */
    s16 record;
    u8 filler06[0x08 - 0x06];
    s8 flag6;
};
#define SLOT_ACTION_CURRENT 0xFF

/* Arguments of cmd_slot_apply_state. */
struct SlotActionWaitCmd
{
    u32 action;
    s8 flag6;
    u8 filler05[0x08 - 0x05];
    u8 wait;        /* nonzero: the script waits on the object (flags_vars_set_obj_state) */
};

struct Words7
{
    s32 w[7];
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
    u16 unk92;      /* move speed (8.8 px per frame), set by cmd_slot_set_step */
    s32 unk94;      /* speed change per frame that stops the move; 0 when not moving */
};

struct StatusBits
{
    u8 lo4 : 4;
    u8 bit4 : 1;
    u8 hi3 : 3;
};

/* Byte 0xFD of a slot's 0x100-byte script record (owner->state + 0x700 + slot * 0x100): the
 * record's own slot in the low nibble, the slot it is linked to in the high one. */
struct ScriptSlotNibbles
{
    u8 filler0[0xFD];
    u8 ownSlot : 4;
    u8 linkedSlot : 4;
};

/* Arguments of cmd_slot_set_cmd_state. */
struct SlotStateCmd
{
    u32 slot;
    s32 action;
    s8 flag6;
    u8 filler9[3];
    s32 state;
};

/* Arguments of cmd_slot_reload_flagged. */
struct SlotReloadCmd
{
    u32 slot;
    s32 action;
    s16 record;
    u8 filler0A[0x0C - 0x0A];
    s8 flag6;
};

struct ObjBits1F
{
    u8 filler0[0x1F];
    u8 c : 2;
    u8 b : 2;
    u8 a : 2;
    u8 hi : 2;
};

struct FlagBit0At79
{
    u8 filler0[0x79];
    u8 bit0 : 1;
    u8 hi7 : 7;
};

struct FlagBit7At110
{
    u8 filler0[0x110];
    u8 lo7 : 7;
    u8 bit7 : 1;
};

extern struct World *gWorld;
extern u8 *gWorkBuf;

/* The Entry of slot idx: slots 0-7 keep it at +8 of gWorld->slots[idx], slots 8-15 in an
 * array of 0xD0-byte records at gWorkBuf + 0x980. */
struct WorkSlot
{
    u8 data[0xD0];
};
#define WORLD_SLOT_ENTRY(idx) ((struct Entry *)((u8 *)gWorld->slots[idx] + 8))
#define WORK_SLOT_ENTRY(idx) ((struct Entry *)((struct WorkSlot *)(gWorkBuf + 0x980) + (idx) - 8))

/* The slot a script owner runs on: slot codes 1-6 are slots 0-5, codes 7 and up slots 0 and up. */
#define OWNER_SLOT(owner) ((owner)->state->slotCode - ((owner)->state->slotCode <= 6 ? 1 : 7))

/* Script arguments for a fixed-point value: either a whole number, or ARG_FRACTION | n for n/16. */
#define ARG_FRACTION 0x40
#define ARG_FRACTION_MASK 0xF

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
void task_spawn_wait_idle(void *obj);
void reset_obj_params_a(void *ctx, void *rec, u32 a, u32 b, u32 mode, u32 sub);
void reset_obj_params_b(void *ctx, void *rec, u32 a, u32 b, u32 mode, u32 sub);

NONMATCH("asm/code_080F3F2C/cmd_reset_slot_node.s", u32 cmd_reset_slot_node(struct Owner *owner, struct Obj *node, struct Cmd *cmd, u8 sub))
{
    u32 mode;
    u32 slot;
    u32 low;
    u16 h;
    u8 *rec;
    u8 *base;
    u8 *nib;

    slot = cmd->unk4;
    base = (u8 *)owner->state + (slot << 8);
    rec = base + 0x700;
    mode = 1;
    nib = base + 0x7FD;
    low = slot & 0xF;
    {
        s32 mk = -16;
        *nib = (*nib & mk) | low;
    }
    if ((cmd->unk0 & 1) != 0)
    {
        node->unkA0 |= 4;
        mode = 9;
    }
    switch (cmd->unk0)
    {
    case 0:
    case 1:
        reset_obj_params_b((u8 *)owner + 0x1C, rec, *(u32 *)node, cmd->unk8, mode, sub);
        *(u32 *)node = cmd->unk8;
        break;
    case 2:
    case 3:
        reset_obj_params_b((u8 *)owner + 0x1C, rec, cmd->unk8, 0, mode, sub);
        break;
    case 4:
    case 5:
        if (((rec[0xA0] ^ 1) & 1) != 0)
            reset_obj_params_b((u8 *)owner + 0x1C, rec, *(u32 *)node, cmd->unk8, mode, sub);
        else
            reset_obj_params_a((u8 *)owner + 0x1C, rec, *(u32 *)node, cmd->unk8, mode, sub);
        *(u32 *)node = cmd->unk8;
        break;
    case 6:
    case 7:
        if (((rec[0xA0] ^ 1) & 1) != 0)
            reset_obj_params_b((u8 *)owner + 0x1C, rec, cmd->unk8, 0, mode, sub);
        else
            reset_obj_params_a((u8 *)owner + 0x1C, rec, cmd->unk8, 0, mode, sub);
        break;
    }
    {
        u32 v = node->unkA0 >> 2;
        s32 one = 1;
        return one & ~v;
    }
}
END_NONMATCH

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

/* Script command on the running script's own slot: cmd[0] >> 1 picks what to set, bit 0 is the
 * on/off value. 0: show the sprite again (re-run its current action, keeping status bit 4) or
 * take it off the active list; 1: clear (on) or set (off) the entry's flag bit at +0x79;
 * 2: the slot's flag bit 7 at +0x110. */
u32 entry_flag_command(struct Owner *owner, void *unused, struct Cmd *cmd)
{
    u8 idx;
    u32 arg;

    idx = owner->state->slotCode <= 6 ? owner->state->slotCode - 1 : owner->state->slotCode - 7;
    arg = cmd->unk0;
    switch ((s32)arg >> 1)
    {
    case 0:
        if ((arg & 1) != 0)
        {
            struct Entry *entry;
            u32 keepBit4;

            if (idx <= 7)
                entry = WORLD_SLOT_ENTRY(idx);
            else
                entry = WORK_SLOT_ENTRY(idx);
            keepBit4 = ((u32)entry->obj8->status << 27) >> 31;
            sprite_set_action(entry, entry->obj8->unk21, 0, 0);
            ((struct StatusBits *)&entry->obj8->status)->bit4 = keepBit4;
        }
        else
        {
            struct Entry *entry;

            if (idx <= 7)
                entry = WORLD_SLOT_ENTRY(idx);
            else
                entry = WORK_SLOT_ENTRY(idx);
            obj_detach_active(entry->obj8);
        }
        break;
    case 1:
        {
            struct Entry *entry;

            if (idx <= 7)
                entry = WORLD_SLOT_ENTRY(idx);
            else
                entry = WORK_SLOT_ENTRY(idx);
            ((struct FlagBit0At79 *)entry)->bit0 = (cmd->unk0 & 1) ^ 1;
        }
        break;
    case 2:
        ((struct FlagBit7At110 *)gWorld->slots[idx])->bit7 = arg & 1;
        break;
    }
    return 1;
}
NONMATCH("asm/code_080F3F2C/entry_value_command.s", u32 entry_value_command(struct Owner *owner, void *unused, struct Cmd *node))
{
    u8 idx;
    s32 w;
    struct Entry *entry;
    struct Obj *obj;
    s32 v;
    u32 b;
    u32 *vp;
    u32 kind;

    idx = owner->state->slotCode <= 6 ? owner->state->slotCode - 1 : owner->state->slotCode - 7;
    vp = (u32 *)node;
    kind = *vp++;
    switch (kind)
    {
    case 1:
        if (idx <= 7)
            entry = (struct Entry *)((u8 *)gWorld->slots[idx] + 8);
        else
            entry = (struct Entry *)(gWorkBuf + 0xD0 * idx + 0x300);
        obj = entry->obj8;
        if (obj != 0)
        {
            w = *vp;
            if ((w & 0x40) == 0)
                v = w << 4;
            else
                v = w & 0xF;
            obj->unk20 = v;
        }
        break;
    case 0:
        w = *vp;
        if ((w & 0x40) != 0)
        {
            if (idx <= 7)
                entry = (struct Entry *)((u8 *)gWorld->slots[idx] + 8);
            else
                entry = (struct Entry *)(gWorkBuf + 0xD0 * idx + 0x300);
            entry->unk90 = (w & 0xF) << 4;
        }
        else
        {
            if (idx <= 7)
                entry = (struct Entry *)((u8 *)gWorld->slots[idx] + 8);
            else
                entry = (struct Entry *)(gWorkBuf + 0xD0 * idx + 0x300);
            entry->unk90 = w << 8;
        }
        break;
    case 2:
        if (idx <= 0xF)
        {
            if (idx <= 7)
                entry = (struct Entry *)((u8 *)gWorld->slots[idx] + 8);
            else
                entry = (struct Entry *)(gWorkBuf + 0xD0 * idx + 0x300);
            obj = entry->obj8;
        }
        else if (idx == 0x10)
        {
            entry = *(struct Entry **)((u8 *)gWorld + 0x278);
            obj = entry->obj8;
        }
        else if (idx <= 0x12)
        {
            entry = *(struct Entry **)((u8 *)gWorld + 0x284 + ((idx - 0x11) << 2));
            obj = entry->obj8;
        }
        else if (idx == 0x13)
        {
            obj = *(struct Obj **)((u8 *)gWorld + 0x368);
        }
        else
        {
            obj = 0;
        }
        if (obj != 0)
        {
            w = *vp & 3;
            b = obj->unk1F;
            b = (((b & -49) | (w << 4)) & -13) | (w << 2);
            b = (b & -4) | w;
            obj->unk1F = b;
        }
        break;
    case 3:
        slot_set_bias(owner, idx, (s8)*(u8 *)vp);
        break;
    case 4:
        if (idx <= 7)
            entry = (struct Entry *)((u8 *)gWorld->slots[idx] + 8);
        else
            entry = (struct Entry *)(gWorkBuf + 0xD0 * idx + 0x300);
        w = *vp;
        if (w > 1)
            v = 1;
        else if (w == 1)
            v = 0;
        else
            v = -1;
        *((u8 *)entry + 0x74) = v;
        break;
    }
    return 1;
}
END_NONMATCH

/* Script command like entry_flag_command, on the slot cmd[1] names instead of the script's own:
 * cmd[0] >> 1 picks what to set, bit 0 is the on/off value. 0: show the sprite again (re-run its
 * current action, keeping status bit 4) or take it off the active list; 1: clear (on) or set (off)
 * the entry's flag bit at +0x79; 2: the slot's flag bit 7 at +0x110. */
u32 entry_mode_command(void *unused0, void *unused1, struct Cmd *cmd)
{
    u32 arg;
    s32 what;

    arg = cmd->unk0;
    what = (s32)arg >> 1;
    switch (what)
    {
    case 0:
        if ((arg & 1) != 0)
        {
            struct Entry *entry;
            u32 keepBit4;

            if (cmd->unk4 <= 7)
                entry = WORLD_SLOT_ENTRY(cmd->unk4);
            else
                entry = WORK_SLOT_ENTRY(cmd->unk4);
            keepBit4 = ((u32)entry->obj8->status << 27) >> 31;
            sprite_set_action(entry, entry->obj8->unk21, 0, 0);
            ((struct StatusBits *)&entry->obj8->status)->bit4 = keepBit4;
        }
        else
        {
            struct Entry *entry;

            if (cmd->unk4 <= 7)
                entry = WORLD_SLOT_ENTRY(cmd->unk4);
            else
                entry = WORK_SLOT_ENTRY(cmd->unk4);
            obj_detach_active(entry->obj8);
        }
        break;
    case 1:
        {
            struct Entry *entry;

            if (cmd->unk4 <= 7)
                entry = WORLD_SLOT_ENTRY(cmd->unk4);
            else
                entry = WORK_SLOT_ENTRY(cmd->unk4);
            ((struct FlagBit0At79 *)entry)->bit0 = (cmd->unk0 & 1) ^ 1;
        }
        break;
    case 2:
        ((struct FlagBit7At110 *)gWorld->slots[cmd->unk4])->bit7 = arg & 1;
        break;
    }
    return 1;
}

/* Script command: cmd[0] is a slot, cmd[1] what to set from cmd[2]: 0 the entry's 8.8 value
 * at +0x90, 1 its object's 12.4 value at +0x20, 2 the object's three 2-bit fields at +0x1F
 * (slots 16-19 are the world's extra entries and object), 3 the slot's bias. */
u32 entry_flag_store(struct Owner *owner, void *unused, u32 *cmd)
{
    u8 idx;
    s32 what;
    u32 value;

    idx = *cmd++;
    what = *cmd++;
    switch (what)
    {
    case 1:
    {
        struct Entry *entry;

        if (idx <= 7)
            entry = WORLD_SLOT_ENTRY(idx);
        else
            entry = WORK_SLOT_ENTRY(idx);
        if (entry->obj8 != 0)
        {
            value = *cmd;
            if (value & ARG_FRACTION)
                value &= ARG_FRACTION_MASK;
            else
                value <<= 4;
            entry->obj8->unk20 = value;
        }
        break;
    }
    case 0:
        if ((*cmd & ARG_FRACTION) != 0)
        {
            struct Entry *entry;

            if (idx <= 7)
                entry = WORLD_SLOT_ENTRY(idx);
            else
                entry = WORK_SLOT_ENTRY(idx);
            entry->unk90 = (*cmd & ARG_FRACTION_MASK) << 4;
        }
        else
        {
            struct Entry *entry;

            if (idx <= 7)
                entry = WORLD_SLOT_ENTRY(idx);
            else
                entry = WORK_SLOT_ENTRY(idx);
            entry->unk90 = *cmd << 8;
        }
        break;
    case 2:
    {
        struct Obj *obj;
        struct Entry *entry;

        if (idx <= 0xF)
        {
            if (idx <= 7)
                entry = WORLD_SLOT_ENTRY(idx);
            else
                entry = WORK_SLOT_ENTRY(idx);
            obj = entry->obj8;
        }
        else if (idx == 0x10)
        {
            entry = gWorld->entry278;
            obj = entry->obj8;
        }
        else if (idx <= 0x12)
        {
            entry = gWorld->entries284[idx - 0x11];
            obj = entry->obj8;
        }
        else if (idx == 0x13)
        {
            obj = gWorld->obj368;
        }
        else
        {
            obj = 0;
        }
        if (obj != 0)
        {
            value = *cmd & 3;
            ((struct ObjBits1F *)obj)->a = value;
            ((struct ObjBits1F *)obj)->b = value;
            ((struct ObjBits1F *)obj)->c = value;
        }
        break;
    }
    case 3:
        slot_set_bias(owner, idx, (s8)*cmd);
        break;
    }
    return 1;
}

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

/*
 * Script command: starts the owner's slot moving at args[0] (1/16 px per frame) and sets the
 * deceleration that brings it to a stop after args[1] pixels (rounded: (D/2 - v*v) / D with
 * D = v + 2 * distance in 8.8), then waits for it to stop (task_spawn_wait_idle).
 */
u32 cmd_slot_set_step(struct Owner *owner, void *dst, s32 *args)
{
    struct Entry *entry;
    s32 divisor;
    s32 speed;
    s32 distance;

    if (OWNER_SLOT(owner) <= 7)
        entry = WORLD_SLOT_ENTRY(OWNER_SLOT(owner));
    else
        entry = WORK_SLOT_ENTRY(OWNER_SLOT(owner));
    args[0] = args[0] << 4; /* 1/16 px -> 8.8 */
    entry->unk92 = args[0];
    distance = args[1];
    speed = args[0];
    divisor = speed + distance * 512;
    entry->unk94 = MATH_ROUTINE(s32 (*)(s32, s32), iwram_div)(-speed * speed + divisor / 2, divisor);
    task_spawn_wait_idle(entry);
    return 1;
}

/* Script branch on the running script's own slot: stopped is 1 once the slot's move has ended
 * (Entry.unk94 is 0). If that differs from args[0], the script jumps to args[1] (*jump). */
u32 cmd_copy_slot_flag(struct Owner *owner, u32 *jump, u32 *args)
{
    u32 stopped = 0;
    struct Entry *entry;

    if (OWNER_SLOT(owner) <= 7)
        entry = WORLD_SLOT_ENTRY(OWNER_SLOT(owner));
    else
        entry = WORK_SLOT_ENTRY(OWNER_SLOT(owner));
    if (entry->unk94 == 0)
        stopped = 1;
    if (stopped != *args++)
        *jump = *args;
    return 1;
}

NONMATCH("asm/code_080F3F2C/cmd_slot_restart.s", u32 cmd_slot_restart(struct Owner *owner, struct Obj *dst, struct Cmd *cmd))
{
    u8 idx;
    struct Entry *entry;
    s16 x;
    s16 y;
    s16 z;
    u16 w;

    if (owner->state->slotCode <= 6)
        idx = (u8)(owner->state->slotCode - 1);
    else
        idx = (u8)(owner->state->slotCode - 7);
    if (idx <= 7)
        entry = (struct Entry *)((u8 *)gWorld->slots[idx] + 8);
    else
        entry = (struct Entry *)(gWorkBuf + 0x300 + 0xD0 * idx);
    w = entry->unk90;
    entry->unk7C = w;
    entry->unk7A = 0;
    slot_read_pos(owner, *(s16 *)cmd, &x, &y, &z);
    slot_set_vec(idx, x, y, z, 1);
    slot_start_motion(idx, 1);
    dst->unkA0 |= 2;
    return 0;
}
END_NONMATCH


NONMATCH("asm/code_080F3F2C/cmd_slot_set_object_state.s", u32 cmd_slot_set_object_state(struct Owner *owner, void *dst, struct Cmd *cmd))
{
    u8 idx;
    u16 shade;
    struct Entry *entry;
    struct Obj *inner;
    idx = owner->state->slotCode <= 6 ? owner->state->slotCode - 1 : owner->state->slotCode - 7;
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
    ((struct StatusBits *)&inner->status)->bit4 = 0;
    return (u8)flags_vars_set_obj_state((u8 *)owner + 0x1C, dst, inner, *(u8 *)((u8 *)cmd + 0xC), 0);
}
END_NONMATCH

/* Script command on the running script's own slot: sets the slot's sprite action, reloads its
 * record and sets its flag 6, keeping the object's byte at +0x20 across the action change, and
 * sets the object's status bit 4. */
u32 cmd_slot_set_flags(struct Owner *owner, void *unused, struct SlotActionCmd *cmd)
{
    u8 idx;
    u16 savedUnk20;
    struct Entry *entry;
    struct Obj *obj;

    idx = owner->state->slotCode <= 6 ? owner->state->slotCode - 1 : owner->state->slotCode - 7;
    refresh_slot_view(owner, idx);
    if (idx <= 7)
        entry = WORLD_SLOT_ENTRY(idx);
    else
        entry = WORK_SLOT_ENTRY(idx);
    obj = entry->obj8;
    if (obj != NULL)
        savedUnk20 = (s8)obj->unk20;
    else
        savedUnk20 = 0x10;
    if (idx <= 7)
        entry = WORLD_SLOT_ENTRY(idx);
    else
        entry = WORK_SLOT_ENTRY(idx);
    sprite_set_action(entry, cmd->action, cmd->anim, 0);
    slot_reload_record(owner, idx, cmd->record);
    slot_set_flag6(owner, idx, cmd->flag6);
    if (idx <= 7)
        entry = WORLD_SLOT_ENTRY(idx);
    else
        entry = WORK_SLOT_ENTRY(idx);
    obj = entry->obj8;
    obj->unk20 = savedUnk20;
    ((struct StatusBits *)&obj->status)->bit4 = 1;
    return 1;
}

/* Script command on the running script's own slot: refreshes its view, sets its sprite action and
 * its flag 6 and clears the object's status bit 4; with a nonzero cmd->wait the script then waits
 * on the object (returns 0), otherwise it goes on (returns 1). */
u32 cmd_slot_apply_state(struct Owner *owner, void *script, struct SlotActionWaitCmd *cmd)
{
    u8 idx;
    struct Entry *entry;
    struct Obj *obj;

    idx = owner->state->slotCode <= 6 ? owner->state->slotCode - 1 : owner->state->slotCode - 7;
    refresh_slot_view(owner, idx);
    if (idx <= 7)
        entry = WORLD_SLOT_ENTRY(idx);
    else
        entry = WORK_SLOT_ENTRY(idx);
    sprite_set_action(entry, cmd->action, 0, 0);
    slot_set_flag6(owner, idx, cmd->flag6);
    if (idx <= 7)
        entry = WORLD_SLOT_ENTRY(idx);
    else
        entry = WORK_SLOT_ENTRY(idx);
    obj = entry->obj8;
    ((struct StatusBits *)&obj->status)->bit4 = 0;
    return (u8)flags_vars_set_obj_state((u8 *)owner + 0x1C, script, obj, cmd->wait, 0);
}


/* Script command on the running script's own slot: refreshes its view, sets its sprite action
 * (SLOT_ACTION_CURRENT re-runs the object's current one, written back into the command), reloads
 * its record and sets its flag 6, then sets the object's status bit 4. */
u32 cmd_slot_reset_record(struct Owner *owner, void *dst, struct SlotRecordCmd *cmd)
{
    u8 idx;
    struct Entry *entry;
    struct Obj *obj;

    idx = owner->state->slotCode <= 6 ? owner->state->slotCode - 1 : owner->state->slotCode - 7;
    if (idx <= 7)
        entry = WORLD_SLOT_ENTRY(idx);
    else
        entry = WORK_SLOT_ENTRY(idx);
    obj = entry->obj8;
    refresh_slot_view(owner, idx);
    if (cmd->action == SLOT_ACTION_CURRENT)
    {
        if (obj != NULL)
            cmd->action = obj->unk21;
        else
            cmd->action = (u32)obj; /* no object: action 0 */
    }
    if (idx <= 7)
        entry = WORLD_SLOT_ENTRY(idx);
    else
        entry = WORK_SLOT_ENTRY(idx);
    sprite_set_action(entry, cmd->action, 0, 0);
    slot_reload_record(owner, idx, cmd->record);
    slot_set_flag6(owner, idx, cmd->flag6);
    if (obj != NULL)
        ((struct StatusBits *)&obj->status)->bit4 = 1;
    return 1;
}


NONMATCH("asm/code_080F3F2C/cmd_slot_set_record_state.s", u32 cmd_slot_set_record_state(struct Owner *owner, u8 *dst, struct Cmd *cmd))
{
    struct Entry *entry;
    struct Obj *inner;
    u32 shade;
    u32 hi;

    if (cmd->unk10 != 0)
    {
        ((struct Obj *)dst)->unkA0 |= 0x800;
        hi = cmd->unk0 << 4;
        ((struct Obj *)dst)->unkFD = (((struct Obj *)dst)->unkFD & 0xF) | hi;
    }
    dst = (u8 *)owner->state + ((cmd->unk0 << 8) + 0x700);
    refresh_slot_view(owner, (s16)cmd->unk0);
    if ((s32)cmd->unk0 <= 7)
        entry = (struct Entry *)((u8 *)gWorld->slots[cmd->unk0] + 8);
    else
        entry = (struct Entry *)(gWorkBuf + 0xD0 * cmd->unk0 + 0x300);
    inner = entry->obj8;
    if (inner != 0)
        shade = (u16)(s8)inner->unk20;
    else
        shade = 0x10;
    if ((s32)cmd->unk0 <= 7)
        entry = (struct Entry *)((u8 *)gWorld->slots[cmd->unk0] + 8);
    else
        entry = (struct Entry *)(gWorkBuf + 0xD0 * cmd->unk0 + 0x300);
    sprite_set_action(entry, cmd->unk8, cmd->unk4, 0);
    slot_set_flag6(owner, (s16)cmd->unk0, (s8)cmd->unkC);
    if ((s32)cmd->unk0 <= 7)
        entry = (struct Entry *)((u8 *)gWorld->slots[cmd->unk0] + 8);
    else
        entry = (struct Entry *)(gWorkBuf + 0xD0 * cmd->unk0 + 0x300);
    inner = entry->obj8;
    inner->unk20 = shade;
    ((struct StatusBits *)&inner->status)->bit4 = 0;
    return (u8)flags_vars_set_obj_state((u8 *)owner + 0x1C, dst, inner, (u8)cmd->unk10, 0);
}
END_NONMATCH

/* Script command, like cmd_slot_set_flags but on slot cmd->unk0: sets the slot's sprite action
 * (cmd->unk8, with anim cmd->unk4), reloads its record (s16 at +0xC) and sets its flag 6 (s8 at
 * +0x10), keeping the object's byte at +0x20 across the action change, and sets the object's
 * status bit 4. */
u32 cmd_slot_reload_object(struct Owner *owner, void *unused, struct Cmd *cmd)
{
    struct Entry *entry;
    struct Obj *obj;
    u16 savedUnk20;

    refresh_slot_view(owner, *(s16 *)cmd);
    if ((s32)cmd->unk0 <= 7)
        entry = WORLD_SLOT_ENTRY(cmd->unk0);
    else
        entry = WORK_SLOT_ENTRY((s32)cmd->unk0);
    obj = entry->obj8;
    if (obj != NULL)
        savedUnk20 = (s8)obj->unk20;
    else
        savedUnk20 = 0x10;
    if ((s32)cmd->unk0 <= 7)
        entry = WORLD_SLOT_ENTRY(cmd->unk0);
    else
        entry = WORK_SLOT_ENTRY((s32)cmd->unk0);
    sprite_set_action(entry, cmd->unk8, cmd->unk4, 0);
    slot_reload_record(owner, *(s16 *)cmd, *(s16 *)((u8 *)cmd + 0xC));
    slot_set_flag6(owner, *(s16 *)cmd, *(s8 *)((u8 *)cmd + 0x10));
    if ((s32)cmd->unk0 <= 7)
        entry = WORLD_SLOT_ENTRY(cmd->unk0);
    else
        entry = WORK_SLOT_ENTRY((s32)cmd->unk0);
    obj = entry->obj8;
    obj->unk20 = savedUnk20;
    ((struct StatusBits *)&obj->status)->bit4 = 1;
    return 1;
}

/* Script command on slot cmd->slot: if cmd->state is set, links the running script to the slot
 * (flag 0x800, slot in the high nibble of byte 0xFD). Then sets the slot's sprite action and its
 * flag 6, clears its object's status bit 4 and sets the state of the slot's script record. */
u32 cmd_slot_set_cmd_state(struct Owner *owner, u8 *script, struct SlotStateCmd *cmd)
{
    struct Entry *entry;
    struct Obj *obj;

    if (cmd->state != 0)
    {
        ((struct Obj *)script)->unkA0 |= 0x800;
        ((struct ScriptSlotNibbles *)script)->linkedSlot = cmd->slot;
    }
    script = (u8 *)owner->state + 0x700 + (cmd->slot << 8);
    refresh_slot_view(owner, (s16)cmd->slot);
    if ((s32)cmd->slot <= 7)
        entry = WORLD_SLOT_ENTRY(cmd->slot);
    else
        entry = WORK_SLOT_ENTRY((s32)cmd->slot);
    sprite_set_action(entry, cmd->action, 0, 0);
    slot_set_flag6(owner, *(s16 *)&cmd->slot, cmd->flag6);
    if ((s32)cmd->slot <= 7)
        entry = WORLD_SLOT_ENTRY(cmd->slot);
    else
        entry = WORK_SLOT_ENTRY((s32)cmd->slot);
    obj = entry->obj8;
    ((struct StatusBits *)&obj->status)->bit4 = 0;
    return (u8)flags_vars_set_obj_state((u8 *)owner + 0x1C, script, obj, *(u8 *)&cmd->state, 0);
}

/* Script command on slot cmd->slot: sets the slot's sprite action, reloads its record and sets
 * its flag 6, then sets the status bit 4 of the slot's object. */
u32 cmd_slot_reload_flagged(struct Owner *owner, void *unused, struct SlotReloadCmd *cmd)
{
    struct Entry *entry;

    refresh_slot_view(owner, *(s16 *)&cmd->slot);
    if ((s32)cmd->slot <= 7)
        entry = WORLD_SLOT_ENTRY(cmd->slot);
    else
        entry = WORK_SLOT_ENTRY((s32)cmd->slot);
    sprite_set_action(entry, cmd->action, 0, 0);
    slot_reload_record(owner, *(s16 *)&cmd->slot, cmd->record);
    slot_set_flag6(owner, *(s16 *)&cmd->slot, cmd->flag6);
    if ((s32)cmd->slot <= 7)
        entry = WORLD_SLOT_ENTRY(cmd->slot);
    else
        entry = WORK_SLOT_ENTRY((s32)cmd->slot);
    ((struct StatusBits *)&entry->obj8->status)->bit4 = 1;
    return 1;
}

/* Script command on the owner's current slot (slotCode - 7), like cmd_slot_mode_cmd: cmd->unk0
 * 0 sets and 1 clears status bit 4 of the slot's object (slots 0-15, 16: gWorld->entry278,
 * 17-18: gWorld->entries284, 19: gWorld->obj368); 2 applies the pose to the slot entry's parts. */
u32 cmd_slot_mode_owner(struct Owner *owner, void *unused, struct Cmd *cmd)
{
    u8 slot;
    s32 mode;
    struct Obj *obj;
    struct Entry *entry;

    slot = owner->state->slotCode - 7;
    mode = cmd->unk0;
    switch (mode)
    {
    case 0:
        if (slot <= 15)
        {
            if (slot <= 7)
                entry = WORLD_SLOT_ENTRY(slot);
            else
                entry = WORK_SLOT_ENTRY(slot);
            obj = entry->obj8;
        }
        else if (slot == 16)
        {
            obj = gWorld->entry278->obj8;
        }
        else if (slot <= 18)
        {
            obj = gWorld->entries284[slot - 17]->obj8;
        }
        else if (slot == 19)
        {
            obj = gWorld->obj368;
        }
        else
        {
            obj = 0;
        }
        if (obj != 0)
            ((struct StatusBits *)&obj->status)->bit4 = 1;
        break;
    case 1:
        if (slot <= 15)
        {
            if (slot <= 7)
                entry = WORLD_SLOT_ENTRY(slot);
            else
                entry = WORK_SLOT_ENTRY(slot);
            obj = entry->obj8;
        }
        else if (slot == 16)
        {
            obj = gWorld->entry278->obj8;
        }
        else if (slot <= 18)
        {
            obj = gWorld->entries284[slot - 17]->obj8;
        }
        else if (slot == 19)
        {
            obj = gWorld->obj368;
        }
        else
        {
            obj = 0;
        }
        if (obj != 0)
            ((struct StatusBits *)&obj->status)->bit4 = 0;
        break;
    case 2:
        if (slot <= 7)
            entry = WORLD_SLOT_ENTRY(slot);
        else
            entry = WORK_SLOT_ENTRY(slot);
        apply_pose_to_parts(entry);
        break;
    }
    return 1;
}
/* Script command on the slot given in args[1], like cmd_slot_mode_owner: args[0] 0 sets and 1 clears
 * status bit 4 of the slot's object (slots 0-15, 16: gWorld->entry278, 17-18: gWorld->entries284,
 * 19: gWorld->obj368); 2 applies the pose to the slot entry's parts. */
u32 cmd_slot_mode_cmd(struct Owner *owner, void *unused, s32 *args)
{
    s32 mode;
    struct Obj *obj;
    struct Entry *entry;

    mode = *args++;
    switch (mode)
    {
    case 0:
    {
        s32 slot = *args;

        if (slot <= 15)
        {
            if (slot <= 7)
                entry = WORLD_SLOT_ENTRY(slot);
            else
                entry = WORK_SLOT_ENTRY(slot);
            obj = entry->obj8;
        }
        else if (slot == 16)
        {
            obj = gWorld->entry278->obj8;
        }
        else if (slot <= 18)
        {
            obj = gWorld->entries284[slot - 17]->obj8;
        }
        else if (slot == 19)
        {
            obj = gWorld->obj368;
        }
        else
        {
            obj = 0;
        }
        if (obj != 0)
            ((struct StatusBits *)&obj->status)->bit4 = 1;
        break;
    }
    case 1:
    {
        s32 slot = *args;

        if (slot <= 15)
        {
            if (slot <= 7)
                entry = WORLD_SLOT_ENTRY(slot);
            else
                entry = WORK_SLOT_ENTRY(slot);
            obj = entry->obj8;
        }
        else if (slot == 16)
        {
            obj = gWorld->entry278->obj8;
        }
        else if (slot <= 18)
        {
            obj = gWorld->entries284[slot - 17]->obj8;
        }
        else if (slot == 19)
        {
            obj = gWorld->obj368;
        }
        else
        {
            obj = 0;
        }
        if (obj != 0)
            ((struct StatusBits *)&obj->status)->bit4 = 0;
        break;
    }
    case 2:
    {
        s32 slot = *args;

        if (slot <= 7)
            entry = WORLD_SLOT_ENTRY(slot);
        else
            entry = WORK_SLOT_ENTRY(slot);
        apply_pose_to_parts(entry);
        break;
    }
    }
    return 1;
}
ASM_FUNC("asm/code_080F3F2C/cmd_slot_copy_tile.s", void cmd_slot_copy_tile(void))

/* Script command: stores the distance in pixels between the battle objects in slots cmd->unk8 and
 * cmd->unkC into script variable *(s16 *)cmd (see cmd_set_var_from_source). Only mode 0
 * (cmd->unk4) exists; any other leaves the value unset. Positions are 24.8 fixed point: each is
 * brought down to 1/16 pixel first so the squares don't overflow. */
u32 cmd_set_var_distance(struct Owner *owner, u8 *thread, struct Cmd *cmd, u8 *frame)
{
    s32 distance;
    s32 pos[14];  /* the 7 words at +0x14 of each slot: [1..3] are x, y, z */
    s32 *posB;

    if (cmd->unk4 == 0)
    {
        *(struct Words7 *)pos = *(struct Words7 *)((u8 *)gWorld->slots[cmd->unk8] + 0x14);
        posB = &pos[7];
        *(struct Words7 *)posB = *(struct Words7 *)((u8 *)gWorld->slots[cmd->unkC] + 0x14);
        pos[1] = pos[1] / 0x10;
        pos[2] = pos[2] / 0x10;
        pos[3] = pos[3] / 0x10;
        posB[1] = posB[1] / 0x10;
        posB[2] = posB[2] / 0x10;
        posB[3] = posB[3] / 0x10;
        distance = MATH_ROUTINE(s32 (*)(s32), iwram_sqrt)(
            (posB[1] - pos[1]) * (posB[1] - pos[1]) + (posB[2] - pos[2]) * (posB[2] - pos[2]) + (posB[3] - pos[3]) * (posB[3] - pos[3])) / 0x10;
    }
    set_var(frame, thread + 0x18, (u8 *)owner->state + 0x1794, thread + 0xA8, *(s16 *)cmd, distance);
    return 1;
}
/* Script command: stores a value from one of ten battle sources (cmd->unk4) into script
 * variable *(s16 *)cmd (set_var: <= 60 a local word, then a flag, then a global var).
 * gWorld->cur is the acting battle object; its +0xEC is >= 0 for an enemy and ~party
 * index for a hero. */
u32 cmd_set_var_from_source(struct Owner *owner, u8 *thread, struct Cmd *cmd, u8 *frame)
{
    s32 value;

    switch (cmd->unk4)
    {
    case 0:
        value = *((u8 *)gWorld + 0x2BB);
        break;
    case 1: /* the actor's id: an enemy's own, a hero's party index + 6 */
        {
            s16 *actorIndex = (s16 *)((u8 *)gWorld->cur + 0xEC);
            if (*actorIndex >= 0)
                value = *actorIndex;
            else
                value = 5 - *actorIndex;
        }
        break;
    case 2: /* a hero's low 5 bits at +0x110, -1 for an enemy */
        if (*(s16 *)((u8 *)gWorld->cur + 0xEC) < 0)
            value = ((u32)*((u8 *)gWorld->cur + 0x110) << 27) >> 27;
        else
            value = -1;
        break;
    case 3: /* byte +0xF4 of the object linked at +0x34 (through its +0x28), or -1 */
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
    case 9: /* bit 1 of gWorld's contact flags at +0x2C1 */
        value = ((u32)*((u8 *)gWorld + 0x2C1) << 30) >> 31;
        break;
    }
    set_var(frame, thread + 0x18, (u8 *)owner->state + 0x1794, thread + 0xA8, *(s16 *)cmd, value);
    return 1;
}
