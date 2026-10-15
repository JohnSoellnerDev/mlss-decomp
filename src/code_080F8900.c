#include "global.h"

struct OwnerObj
{
    u8 filler0[0x2C];
    u8 *state;
};

struct SlotTable
{
    u8 *entries;
};

struct SlotEntry
{
    void *sprite;
    u16 unk4;
    u16 unk6;
    u16 unk8;
    u16 unkA;
    u8 filler0C;
    u8 count0D;
    u8 filler0E;
    u8 flag0F;
};

struct SlotPos
{
    u16 unk0;
    u16 unk2;
    u8 filler4[0xA];
    u16 unkE;
};

struct PartCtl
{
    u8 filler0[0x1A0];
    u16 unk1A0;
};

struct KeyWord
{
    u16 low : 10;
    u16 high : 6;
};

struct HeapOwner
{
    void *block;
};

extern u8 *gUnk_03000FD8;
extern void (*gMemRoutines)(u32 a, void *b, u32 c);
extern const u8 gTextEntryTag[];
extern u8 gUnk_0300034C[];
extern u8 *gWorkBuf;
void state_apply_hero_stats(struct OwnerObj *owner);
void slot_read_pos(void *obj, s32 idx, s16 *a, s16 *b, s16 *c);
void slot_set_pos_abs(void *obj, u32 idx, s32 x, s32 y, s32 z);
void refresh_slot_view(void *obj, s32 idx);
void slot_reload_record(void *obj, s32 idx, s32 value);
void slot_set_flag6(void *obj, s32 idx, s32 flag);
void set_var(u8 *mid, u8 *low, u8 *flagsA, u8 *flagsB, s16 index, u32 value);
void set_text_ptrs(u32 a);
void start_timer_display(void);
void voice_message_step(void);

void heap_free(void *p);
void heap_free_b(void *p);
void obj_detach_active(void *sprite);
void *heap_alloc2(u32 size, u32 a, const void *table, u32 c);
void *init_text_obj(void *mem, u32 a, u32 b, u32 c);
u32 load_u32(void *p);
void reset_obj_params_b(void *ctx, void *obj, u32 a, u32 b, u16 s5, u8 s6);
u32 restore_obj_word0(void *ctx, void *obj);
u32 cmd_reset_obj_part(struct OwnerObj *obj, u8 *state, u32 *params, u8 idx);
void set_targets_from_record(void *obj);
void change_item_count(u8 item, s16 count);
u32 cmd_update_and_store(void *ctx, u32 arg);

ASM_FUNC("asm/code_080F8900/cmd_slot_flag_20.s", u32 cmd_slot_flag_20(void *obj, u32 unused, u32 *p))

ASM_FUNC("asm/code_080F8900/cmd_slot_set_fields.s", u32 cmd_slot_set_fields(struct OwnerObj *obj, u32 unused, u32 *p))

ASM_FUNC("asm/code_080F8900/obj_slot_set_fields.s", u32 obj_slot_set_fields(struct OwnerObj *obj, u8 *node, u32 *p))

ASM_FUNC("asm/code_080F8900/cmd_call_cf0.s", u32 cmd_call_cf0(void *obj, u32 unused, s32 *p))

ASM_FUNC("asm/code_080F8900/cmd_obj_chain.s", u32 cmd_obj_chain(struct OwnerObj *obj, u8 *dst, u32 *p))

u32 cmd_world_flag_2b4(void *obj, u32 unused, u32 *p)
{
    u8 *world = gUnk_03000FD8;
    u32 v = p[0];

    *(u16 *)(world + 0x2B4) = v;
    world[0x2C1] |= 8;
    return 1;
}

ASM_FUNC("asm/code_080F8900/cmd_key_word.s", u32 cmd_key_word(void *obj, u32 unused, u32 *p))

ASM_FUNC("asm/code_080F8900/cmd_swap_slots.s", u32 cmd_swap_slots(void *obj, u32 unused, u32 *p))

ASM_FUNC("asm/code_080F8900/world_cmds_cmd_world_mode.s", void world_cmds_cmd_world_mode(void))

ASM_FUNC("asm/code_080F8900/cmd_effect_mode.s", u32 cmd_effect_mode(void *obj, u32 unused, u32 *p))

ASM_FUNC("asm/code_080F8900/cmd_reward_or_update.s", u32 cmd_reward_or_update(struct OwnerObj *obj, u32 arg, u32 *p))

ASM_FUNC("asm/code_080F8900/cmd_find_slot_set_var.s", u32 cmd_find_slot_set_var(struct OwnerObj *obj, u8 *var, u32 *p, u32 ctx))

ASM_FUNC("asm/code_080F8900/cmd_slot_get_set_var.s", void cmd_slot_get_set_var(void))

ASM_FUNC("asm/code_080F8900/cmd_flag_op.s", u32 cmd_flag_op(struct OwnerObj *obj, u32 *dst, u32 *p))

u32 cmd_world_flag_2f8(void *obj, u32 unused, u32 *p)
{
    u8 *dst;
    u8 val;

    switch (p[0])
    {
    case 0:
        dst = gUnk_03000FD8 + 0x2F8;
        val = 1;
        break;
    case 1:
        dst = gUnk_03000FD8 + 0x2F8;
        val = 0;
        break;
    default:
        return 1;
    }
    *dst = val;
    return 1;
}

ASM_FUNC("asm/code_080F8900/cmd_slot_refresh.s", u32 cmd_slot_refresh(struct OwnerObj *obj, u32 unused, u8 *p))

ASM_FUNC("asm/code_080F8900/cmd_slot_shift_pos.s", u32 cmd_slot_shift_pos(struct OwnerObj *obj, u32 unused, u32 *p))

ASM_FUNC("asm/code_080F8900/cmd_slot_resync_pos.s", u32 cmd_slot_resync_pos(struct OwnerObj *obj, u32 unused, s16 *p))

ASM_FUNC("asm/code_080F8900/cmd_slot_flag_copy.s", u32 cmd_slot_flag_copy(struct OwnerObj *obj, u32 *dst, u32 *p))

ASM_FUNC("asm/code_080F8900/cmd_slot_set_stream.s", u32 cmd_slot_set_stream(struct OwnerObj *obj, u8 *node, u32 *p))

ASM_FUNC("asm/code_080F8900/cmd_wait_flag_copy.s", u32 cmd_wait_flag_copy(struct OwnerObj *obj, u32 *dst, u32 *p))

u32 cmd_owner_flag_copy(struct OwnerObj *obj, u32 *dst, u32 *p)
{
    u32 sel = (u8)(*p++ + 1);
    u32 flag = *p++;
    u32 bit;

    if (flag == 0)
    {
        u8 *part = obj->state;
        u32 x;

        part += sel << 8;
        x = *(u16 *)(part + 0xA0) & 0xFF7;
        bit = ((-x) | x) >> 31;
    }
    if (bit != *p)
    {
        return 1;
    }
    *dst = p[1];
    return 1;
}

u8 cmd_restore_word0(void *obj, void *arg, u32 *p)
{
    if (p[0] != 0)
    {
        gUnk_03000FD8[0x2BF] |= 2;
    }
    return restore_obj_word0((u8 *)obj + 0x1C, arg);
}

ASM_FUNC("asm/code_080F8900/cmd_owner_set_var.s", u32 cmd_owner_set_var(struct OwnerObj *obj, u8 *var, u32 *p, u32 ctx))

u32 cmd_owner_part_copy(struct OwnerObj *obj, u32 *dst, u32 *p)
{
    u32 sel = *p++;
    u8 *part;

    switch (sel)
    {
    case 0:
        part = obj->state;
        break;
    case 1:
        part = obj->state + ((p[1] << 8) + 0x100);
        break;
    }
    if (*(u16 *)(part + 0xA0) & 1)
    {
        *dst = *p;
    }
    return 1;
}

ASM_FUNC("asm/code_080F8900/cmd_owner_part_reset.s", u32 cmd_owner_part_reset(struct OwnerObj *obj, u32 unused, u32 *p))

ASM_FUNC("asm/code_080F8900/cmd_reset_obj_part.s", u32 cmd_reset_obj_part(struct OwnerObj *obj, u8 *state, u32 *params, u8 idx))

u32 cmd_part_shift(struct OwnerObj *obj, u32 unused, u32 *p)
{
    u8 *part = obj->state + (p[0] << 8);
    u8 *base = part + 0x700;

    *(u16 *)(part + 0x7A0) = 0;
    *(u32 *)base = *(u32 *)(base + 4);
    return 1;
}

ASM_FUNC("asm/code_080F8900/cmd_find_part_copy.s", void cmd_find_part_copy(void))

ASM_FUNC("asm/code_080F8900/cmd_part_flag4.s", u32 cmd_part_flag4(struct OwnerObj *obj, u32 unused, u32 *p))

ASM_FUNC("asm/code_080F8900/cmd_node_stream.s", void cmd_node_stream(void))

ASM_FUNC("asm/code_080F8900/cmd_slot_clear_flag2.s", void cmd_slot_clear_flag2(struct OwnerObj *obj, u8 *node, u8 idx))

ASM_FUNC("asm/code_080F8900/cmd_part_scan.s", void cmd_part_scan(void))

void clear_node_bit9(struct OwnerObj *obj, u8 *node)
{
    if (*(u32 *)(obj->state + 0x1788) & 0x4000)
    {
        *(u16 *)(node + 0xA0) &= 0xFDFF;
    }
}

ASM_FUNC("asm/code_080F8900/slot_clear_flag800_if_idle.s", void slot_clear_flag800_if_idle(struct OwnerObj *obj, u8 *node))

ASM_FUNC("asm/code_080F8900/setup_bg_entry.s", void setup_bg_entry(void))

ASM_FUNC("asm/code_080F8900/create_text_entry.s", u32 create_text_entry(u8 *owner, u8 idx, u32 a, u32 b, u32 c))

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

void unknown_thumb_080f962c(struct OwnerObj *obj, u32 mask)
{
    *(u32 *)(obj->state + 0x1788) |= mask;
    *(u32 *)(obj->state + 0x1798) |= mask;
}

void sub_080F964C(struct OwnerObj *obj, u32 mask)
{
    *(u32 *)(obj->state + 0x1788) &= ~mask;
}

void sub_080F9660(struct OwnerObj *obj, u32 mask)
{
    *(u32 *)(obj->state + 0x1788) |= mask;
}

ASM_FUNC("asm/code_080F8900/sub_080F9674.s", void sub_080F9674(struct OwnerObj *obj, u8 idx))

ASM_FUNC("asm/code_080F8900/reset_entry_part.s", void reset_entry_part(struct OwnerObj *obj, u8 idx, u32 a))

void reset_root_params(struct OwnerObj *obj, u32 a)
{
    reset_obj_params_b((u8 *)obj + 0x1C, obj->state, a, 0, 1, 0xFF);
}

ASM_FUNC("asm/code_080F8900/unknown_thumb_080f970e.s", void unknown_thumb_080f970e(struct SlotTable *table, u8 idx))

u32 sub_080F9738(struct SlotTable *table, u8 idx)
{
    u32 off = (u32)idx << 4;
    u32 n;
    u8 count;

    if (((struct SlotEntry *)(table->entries + off))->flag0F == 0)
    {
        return 0;
    }
    n = ((struct SlotEntry *)(table->entries + off))->count0D;
    if (n == 0)
    {
        return 1;
    }
    n = n - 1;
    ((struct SlotEntry *)(table->entries + off))->count0D = n;
    count = n;
    if (count != 0)
    {
        return 1;
    }
    {
        struct SlotEntry *entry = (struct SlotEntry *)(table->entries + off);

        obj_detach_active(entry->sprite);
        entry->flag0F = count;
    }
    return 0;
}

void sub_080F9778(struct SlotTable *table, u8 idx)
{
    struct SlotEntry *entry = (struct SlotEntry *)(table->entries + ((u32)idx << 4));

    obj_detach_active(entry->sprite);
    entry->flag0F = 0;
}

void sub_080F9794(struct HeapOwner *obj, u32 flags)
{
    heap_free(obj->block);
    if (flags & 1)
    {
        heap_free_b(obj);
    }
}

ASM_FUNC("asm/code_080F8900/owner_step.s", void owner_step(void))

ASM_FUNC("asm/code_080F8900/unknown_thumb_080f98d8.s", void unknown_thumb_080f98d8(void))

ASM_FUNC("asm/code_080F8900/owner_init_entry.s", void owner_init_entry(void))

ASM_FUNC("asm/code_080F8900/owner_tick_records.s", void owner_tick_records(void))

ASM_FUNC("asm/code_080F8900/owner_reset.s", void owner_reset(void))

ASM_FUNC("asm/code_080F8900/slot_load_object.s", void slot_load_object(void))

ASM_FUNC("asm/code_080F8900/slot_store_object.s", void slot_store_object(void *owner, u32 idx))

ASM_FUNC("asm/code_080F8900/record_advance.s", void record_advance(void))

ASM_FUNC("asm/code_080F8900/record_dispatch.s", void record_dispatch(void))
