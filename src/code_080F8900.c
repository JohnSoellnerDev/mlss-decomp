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

struct OwnerFlag9A2
{
    u8 b0 : 1;
    u8 rest : 7;
};

struct HeapOwner
{
    void *block;
};

struct WorldSlotTable
{
    u8 filler0[0x58];
    u8 *slots[8];
};

extern u8 *gWorld;
extern void (*gMemRoutines)(u32 a, void *b, u32 c);
extern const u8 gTextEntryTag[];
extern u8 gSys[];
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
u32 slot_find_max_key(void *unused0, s32 threshold);
u32 slot_find_min_key(void *unused0, s32 threshold);
u32 slot_widest_span(void *unused0, s32 threshold);
s32 slot_get_field(void *obj, s32 idx, s32 field);

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
void unpack_stream_values(u8 *a, u32 *stream, u32 *p1, u32 *p2, u32 *p3);
void slot_set_value(struct OwnerObj *obj, u32 idx, s32 value, u32 flag);
void slot_set_vec(u32 idx, s32 x, s32 y, s32 z, u32 flag);
void slot_start_motion(u32 idx, u32 flag);
void set_mark_callbacks(void);
void spawn_slot_markers(void);
void set_world_flag0(u8 flag);
void update_slot_markers(void);
void release_slot_objects(void);
void release_timer_tiles(void);
extern u8 *gUnk_03000FC0;
u32 node_step(struct OwnerObj *obj, u8 *node);
u32 run_node_command(struct OwnerObj *obj, u8 *node, u32 *buf);
void decode_stream_run(u8 *rec, u32 *buf, u8 *block, u8 *p3, u8 *p4, u8 *advanced);

struct Flags110
{
    u8 lo5 : 5;
    u8 bit5 : 1;
    u8 hi2 : 2;
};

struct NodeFlags1F
{
    u8 f0 : 2;
    u8 f2 : 2;
    u8 f4 : 2;
    u8 f6 : 2;
};

u32 cmd_slot_flag_20(void *obj, u32 unused, u32 *p)
{
    u8 *entry = ((struct WorldSlotTable *)gWorld)->slots[p[1]];
    u32 flag = 0;

    if (p[0] == 0)
    {
        flag = 1;
    }
    ((struct Flags110 *)(entry + 0x110))->bit5 = flag;
    return 1;
}

NONMATCH("asm/code_080F8900/cmd_slot_set_fields.s", u32 cmd_slot_set_fields(struct OwnerObj *obj, u32 unused, u32 *p))
{
    u8 *entry = ((struct WorldSlotTable *)gWorld)->slots[*p++];
    u32 v = *p++;
    u32 low;
    u32 hi;
    u8 *flags;
    u32 t;

    *(u16 *)(entry + 0xD0) = v;
    low = *p++;
    flags = entry + 0xD4;
    t = *flags;
    t = (low & 0x7F) | (t & -0x80);
    *flags = t;
    hi = *p;
    *(u16 *)(entry + 0xD2) = hi;
    *flags |= 0x80;
    return 1;
}
END_NONMATCH

NONMATCH("asm/code_080F8900/obj_slot_set_fields.s", u32 obj_slot_set_fields(struct OwnerObj *obj, u8 *node, u32 *p))
{
    u8 *entry = ((struct WorldSlotTable *)gWorld)->slots[((u32)node[0xFD] << 28) >> 28];
    u32 v = *p++;
    u32 low;
    u32 hi;
    u8 *flags;
    u32 t;

    *(u16 *)(entry + 0xD0) = v;
    low = *p++;
    flags = entry + 0xD4;
    t = *flags;
    t = (low & 0x7F) | (t & -0x80);
    *flags = t;
    hi = *p;
    *(u16 *)(entry + 0xD2) = hi;
    *flags |= 0x80;
    return 1;
}
END_NONMATCH

u32 cmd_call_cf0(void *obj, u32 unused, s32 *p)
{
    u8 *world = gWorld;
    u8 *cur = *(u8 **)(world + 0x7C);

    if (cur != 0)
    {
        s32 idx = p[0];
        u8 *rec;

        if (idx < 0)
        {
            rec = cur;
        }
        else
        {
            rec = ((struct WorldSlotTable *)world)->slots[idx];
        }
        if (*(u32 *)(rec + 0x54) != 0)
        {
            set_targets_from_record(rec + 8);
        }
    }
    return 1;
}

ASM_FUNC("asm/code_080F8900/cmd_obj_chain.s", u32 cmd_obj_chain(struct OwnerObj *obj, u8 *dst, u32 *p))

u32 cmd_world_flag_2b4(void *obj, u32 unused, u32 *p)
{
    u8 *world = gWorld;
    u32 v = p[0];

    *(u16 *)(world + 0x2B4) = v;
    world[0x2C1] |= 8;
    return 1;
}

NONMATCH("asm/code_080F8900/cmd_key_word.s", u32 cmd_key_word(void *obj, u32 unused, u32 *p))
{
    u32 low = ~*p & 0x3FF;
    u16 *keys = (u16 *)(gSys + 0x78);

    *keys = (*keys & 0xFFFFFC00) | low;
    return 1;
}
END_NONMATCH

NONMATCH("asm/code_080F8900/cmd_swap_slots.s", u32 cmd_swap_slots(void *obj, u32 unused, u32 *p))
{
    u32 ia = p[0];
    u8 **slot_a = ((struct WorldSlotTable *)gWorld)->slots + ia;
    u8 *old_a = *slot_a;
    u8 **slot_b = ((struct WorldSlotTable *)gWorld)->slots + p[1];
    u8 *old_b = *slot_b;

    *slot_a = old_b;
    *slot_b = old_a;
    (*slot_a)[0xF4] = ia;
    ((struct WorldSlotTable *)gWorld)->slots[p[1]][0xF4] = p[1];
    return 1;
}
END_NONMATCH

struct WorldBit3
{
    u8 lo3 : 3;
    u8 bit3 : 1;
    u8 hi4 : 4;
};

struct WorldBit4
{
    u8 lo4 : 4;
    u8 bit4 : 1;
    u8 hi3 : 3;
};

NONMATCH("asm/code_080F8900/world_cmds_cmd_world_mode.s", u32 world_cmds_cmd_world_mode(void *obj, u32 unused, u32 *p))
{
    switch (*p++)
    {
    case 0:
        if (p[0] != 0)
        {
            spawn_slot_markers();
        }
        else
        {
            set_mark_callbacks();
        }
        break;
    case 1:
        set_world_flag0((u8)p[0]);
        break;
    case 2:
        ((struct WorldBit3 *)(gWorld + 0xB))->bit3 = (p[0] == 0);
        if (p[0] == 0)
        {
            release_slot_objects();
        }
        else
        {
            update_slot_markers();
        }
        break;
    case 3:
        if (p[0] != 0)
        {
            start_timer_display();
        }
        else
        {
            release_timer_tiles();
        }
        break;
    case 4:
        ((struct WorldBit4 *)(gWorld + 0x2BF))->bit4 = p[0] & 1;
        break;
    }
    return 1;
}
END_NONMATCH

ASM_FUNC("asm/code_080F8900/cmd_effect_mode.s", u32 cmd_effect_mode(void *obj, u32 unused, u32 *p))

ASM_FUNC("asm/code_080F8900/cmd_reward_or_update.s", u32 cmd_reward_or_update(struct OwnerObj *obj, u32 arg, u32 *p))

u32 cmd_find_slot_set_var(struct OwnerObj *obj, u8 *var, s16 *p, u32 ctx)
{
    u32 value;

    switch (*(s32 *)(p + 2))
    {
    case 0:
        value = slot_find_max_key(obj, *(s32 *)(p + 4));
        break;
    case 1:
        value = slot_find_min_key(obj, *(s32 *)(p + 4));
        break;
    case 2:
        value = slot_widest_span(obj, *(s32 *)(p + 4));
        break;
    }
    set_var((u8 *)ctx, var + 0x18, obj->state + 0x1794, var + 0xA8, p[0], value);
    return 1;
}

u32 cmd_slot_get_set_var(struct OwnerObj *obj, u8 *var, s16 *p, u32 ctx)
{
    u32 value = slot_get_field(obj, p[2], *(s32 *)(p + 4));

    set_var((u8 *)ctx, var + 0x18, obj->state + 0x1794, var + 0xA8, p[0], value);
    return 1;
}

ASM_FUNC("asm/code_080F8900/cmd_flag_op.s", u32 cmd_flag_op(struct OwnerObj *obj, u32 *dst, u32 *p))

u32 cmd_world_flag_2f8(void *obj, u32 unused, u32 *p)
{
    u8 *dst;
    u8 val;

    switch (p[0])
    {
    case 0:
        dst = gWorld + 0x2F8;
        val = 1;
        break;
    case 1:
        dst = gWorld + 0x2F8;
        val = 0;
        break;
    default:
        return 1;
    }
    *dst = val;
    return 1;
}

NONMATCH("asm/code_080F8900/cmd_slot_refresh.s", u32 cmd_slot_refresh(struct OwnerObj *obj, u32 unused, s16 *p))
{
    u8 *entry;
    s32 idx;

    refresh_slot_view(obj, p[0]);
    slot_reload_record(obj, p[0], p[2]);
    slot_set_flag6(obj, p[0], ((s8 *)p)[8]);
    idx = *(s32 *)p;
    if (idx < 8)
    {
        entry = ((struct WorldSlotTable *)gWorld)->slots[idx] + 8;
    }
    else
    {
        entry = gWorkBuf + idx * 0xD0 + 0x300;
    }
    ((u8 *)*(u8 **)(entry + 8))[0x12] |= 0x10;
    return 1;
}
END_NONMATCH

u32 cmd_slot_shift_pos(struct OwnerObj *obj, u32 unused, s32 *p)
{
    u8 base = obj->state[0x179C];
    u8 idx;
    s16 x, y, z;

    if (base < 7)
    {
        idx = base - 1;
    }
    else
    {
        idx = base - 7;
    }
    slot_read_pos(obj, (s16)p[0], &x, &y, &z);
    slot_set_pos_abs(obj, idx, p[1] + x, p[2] + y, p[3] + z);
    return 0;
}

u32 cmd_slot_resync_pos(struct OwnerObj *obj, u32 unused, s16 *p)
{
    u8 base = obj->state[0x179C];
    u8 idx;
    s16 x, y, z;

    if (base < 7)
    {
        idx = base - 1;
    }
    else
    {
        idx = base - 7;
    }
    slot_read_pos(obj, p[0], &x, &y, &z);
    slot_set_pos_abs(obj, idx, x, y, z);
    return 0;
}

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
        gWorld[0x2BF] |= 2;
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

void unpack_stream_values(u8 *a, u32 *stream, u32 *p1, u32 *p2, u32 *p3);
void slot_set_value(struct OwnerObj *obj, u32 idx, s32 value, u32 flag);
void slot_set_vec(u32 idx, s32 x, s32 y, s32 z, u32 flag);
void slot_start_motion(u32 idx, u32 flag);
u32 cmd_owner_part_reset(struct OwnerObj *obj, u32 unused, u32 *p)
{
    u8 *part;
    s16 count;

    switch (*p++)
    {
    case 0:
        part = obj->state;
        break;
    case 1:
        part = obj->state + (((*p + 1) << 24) >> 16);
        break;
    default:
        part = (u8 *)unused;
        break;
    }
    *(u32 *)(part + 4) = *(u32 *)part;
    *(u16 *)(part + 0xA0) = 1;
    for (count = 0x17; count > 0; count--)
    {
    }
    return 1;
}

ASM_FUNC("asm/code_080F8900/cmd_reset_obj_part.s", u32 cmd_reset_obj_part(struct OwnerObj *obj, u8 *state, u32 *params, u8 idx))

u32 cmd_part_shift(struct OwnerObj *obj, u32 unused, u32 *p)
{
    u8 *part = obj->state + (p[0] << 8);
    u8 *base = part + 0x700;

    *(u16 *)(part + 0x7A0) = 0;
    *(u32 *)base = *(u32 *)(base + 4);
    return 1;
}

NONMATCH("asm/code_080F8900/cmd_find_part_copy.s", u32 cmd_find_part_copy(struct OwnerObj *obj, u32 *dst, u32 *p, u8 key))
{
    u8 *part = obj->state + 0x700;
    s16 count = 0x10;

    if (p[0] == 0)
    {
        part += p[1] << 8;
        count = 1;
    }
    while (count > 0)
    {
        if ((*(u16 *)(part + 0xA0) & 1) && part[0xA4] == key)
        {
            dst[0] = dst[5];
            return 0;
        }
        part += 0x100;
        count--;
    }
    return 1;
}
END_NONMATCH

u32 slot_find_max_key(void *unused0, s32 threshold);
u32 slot_find_min_key(void *unused0, s32 threshold);
u32 slot_widest_span(void *unused0, s32 threshold);
s32 slot_get_field(void *obj, s32 idx, s32 field);
NONMATCH("asm/code_080F8900/cmd_part_flag4.s", u32 cmd_part_flag4(struct OwnerObj *obj, u32 unused, u32 *p))
{
    u8 *part = obj->state + (p[1] << 8);
    u16 *flags;
    u32 v;

    switch (p[0])
    {
    case 0:
        flags = (u16 *)(part + 0x7A0);
        v = 4 | *flags;
        break;
    case 1:
        flags = (u16 *)(part + 0x7A0);
        v = 0xFFFB & *flags;
        break;
    default:
        return 1;
    }
    *flags = v;
    return 1;
}
END_NONMATCH

u32 cmd_node_stream(struct OwnerObj *obj, u8 *node)
{
    u32 buf[13];

    if (((node[0xA0] ^ 1) & 1) != 0)
    {
        return 0;
    }
    while ((u8)node_step(obj, node) == 0)
    {
        decode_stream_run(node, buf, gUnk_03000FC0, obj->state + 0x1700, obj->state + 0x1794, node + 0xA8);
        if ((u8)run_node_command(obj, node, buf) == 0)
        {
            return 1;
        }
    }
    return 1;
}

NONMATCH("asm/code_080F8900/cmd_slot_clear_flag2.s", void cmd_slot_clear_flag2(struct OwnerObj *obj, u8 *node, u8 idx))
{
    u8 *entry;

    if (idx < 8)
    {
        entry = ((struct WorldSlotTable *)gWorld)->slots[idx] + 8;
    }
    else
    {
        entry = gWorkBuf + idx * 0xD0 + 0x300;
    }
    if (*(u32 *)(entry + 0x80) == 0)
    {
        *(u16 *)(node + 0xA0) &= 0xFFFD;
    }
}
END_NONMATCH

ASM_FUNC("asm/code_080F8900/cmd_part_scan.s", void cmd_part_scan(void))

void clear_node_bit9(struct OwnerObj *obj, u8 *node)
{
    if (*(u32 *)(obj->state + 0x1788) & 0x4000)
    {
        *(u16 *)(node + 0xA0) &= 0xFDFF;
    }
}

void slot_clear_flag800_if_idle(struct OwnerObj *obj, u8 *node)
{
    u32 idx = node[0xFD] >> 4;
    u8 *state = obj->state;

    if ((*(u16 *)((idx << 8) + (u32)state + 0x7A0) & 0x20) == 0)
    {
        *(u16 *)(node + 0xA0) &= 0xF7FF;
    }
}

ASM_FUNC("asm/code_080F8900/setup_bg_entry.s", void setup_bg_entry(void))

void set_mark_callbacks(void);
void spawn_slot_markers(void);
void set_world_flag0(u8 flag);
void update_slot_markers(void);
void release_slot_objects(void);
void release_timer_tiles(void);
u32 create_text_entry(u8 *owner, u8 idx, u32 a, u32 b, u32 c)
{
    u32 *slot;
    void *text;
    u32 off = (u32)idx << 2;

    owner += 0x20;
    slot = (u32 *)(owner + off);

    text = heap_alloc2(8, 0, gTextEntryTag, 0);
    text = init_text_obj(text, a, b, c);
    *slot = (u32)text;
    return load_u32(text);
}

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

void owner_clear_flags_1788(struct OwnerObj *obj, u32 mask)
{
    *(u32 *)(obj->state + 0x1788) &= ~mask;
}

void owner_set_flags_1788(struct OwnerObj *obj, u32 mask)
{
    *(u32 *)(obj->state + 0x1788) |= mask;
}

NONMATCH("asm/code_080F8900/sub_080F9674.s", void sub_080F9674(struct OwnerObj *obj, u8 idx))
{
    u32 off = (u32)idx << 8;
    u32 size = 0x100;

    gMemRoutines(0, obj->state + (off + size), size);
    gMemRoutines(0, obj->state + (off + 0x700), size);
}
END_NONMATCH

void reset_entry_part(struct OwnerObj *obj, u8 idx, u32 a)
{
    u32 params[2];
    u8 *state;
    struct PartCtl *part;

    params[0] = 1;
    params[1] = a;
    state = obj->state;
    part = (struct PartCtl *)(((u32)idx << 8) + (u32)state);
    part->unk1A0 = 0;
    cmd_reset_obj_part(obj, obj->state, params, idx);
}

void reset_root_params(struct OwnerObj *obj, u32 a)
{
    reset_obj_params_b((u8 *)obj + 0x1C, obj->state, a, 0, 1, 0xFF);
}

void unknown_thumb_080f970e(struct SlotTable *table, u8 idx)
{
    u32 off = (u8)idx << 4;
    u8 *base = table->entries;
    struct SlotPos *obj;
    struct SlotEntry *entry;
    u32 sum1;

    base += off;
    entry = (struct SlotEntry *)base;
    obj = entry->sprite;
    obj->unk0 = entry->unk4 + entry->unk8;
    sum1 = entry->unkA;
    base = (u8 *)(u32)entry->unk6;
    sum1 += (u32)base;
    obj->unk2 = sum1;
    obj->unkE = 0x7FFF - sum1;
}

u32 slot_countdown_step(struct SlotTable *table, u8 idx)
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

void slot_detach_entry(struct SlotTable *table, u8 idx)
{
    struct SlotEntry *entry = (struct SlotEntry *)(table->entries + ((u32)idx << 4));

    obj_detach_active(entry->sprite);
    entry->flag0F = 0;
}

void heap_free_owner(struct HeapOwner *obj, u32 flags)
{
    heap_free(obj->block);
    if (flags & 1)
    {
        heap_free_b(obj);
    }
}

u32 owner_tick_records(u8 *owner);
u32 run_record(struct OwnerObj *owner, u8 *rec);
u8 text_layer_step(u8 *owner);
u32 slot_countdown(struct SlotTable *table, u8 idx);
void update_slot_bounds(struct SlotTable *table, u8 idx);
void owner_step(struct OwnerObj *owner)
{
    u8 *state = *(u8 **)((u8 *)owner + 0x24);
    u8 *cnt = state + 0x9A0;
    u8 *end;
    struct SlotEntry *entry;
    s16 i;

    ((struct OwnerFlag9A2 *)(state + 0x9A2))->b0 = 0;
    owner_tick_records((u8 *)owner);
    *cnt = 0;
    if ((*(u16 *)(state + 0xA0) & 0x14) == 0)
    {
        run_record(owner, state);
    }
    *cnt = *cnt + 1;
    state += 0xA8;
    end = state + (0xFC << 3);
    while (state != end)
    {
        if ((*(u16 *)(state + 0xA0) & 0x36) == 0)
        {
            *(u32 *)(*(u8 **)((u8 *)owner + 0x24) + 0x994) = *cnt - 1;
            run_record(owner, state);
        }
        state += 0xA8;
        *cnt = *cnt + 1;
    }
    if (*(u8 *)(*(u8 **)(*(u8 **)((u8 *)owner + 0x28) + 4) + 0x1B2) != 0)
    {
        *(u8 *)(*(u8 **)(*(u8 **)((u8 *)owner + 0x28) + 4) + 0x1B2) = text_layer_step(*(u8 **)((u8 *)owner + 0x28));
    }
    if ((*(u8 *)(*(u8 **)((u8 *)owner + 0x24) + 0x9A3) & 1) != 0)
    {
        i = 0;
        entry = (struct SlotEntry *)(*(struct SlotTable **)((u8 *)owner + 0x20))->entries;
        do
        {
            if ((u8)slot_countdown(*(struct SlotTable **)((u8 *)owner + 0x20), (u8)i) != 0)
            {
                if (entry->filler0E != 0)
                {
                    u8 *src = *(u8 **)(*(u8 **)((u8 *)owner + 0x14) + 0x158) + (entry->filler0C << 6);
                    s32 v = *(s32 *)(src + 0x14);
                    if (v < 0)
                    {
                        v += 0xFF;
                    }
                    entry->unk8 = v >> 8;
                    v = *(s32 *)(src + 0x18);
                    if (v < 0)
                    {
                        v += 0xFF;
                    }
                    entry->unkA = (v >> 8) - src[0x3D];
                }
                update_slot_bounds(*(struct SlotTable **)((u8 *)owner + 0x20), (u8)i);
            }
            i++;
            entry++;
        } while (i < 4);
    }
}

ASM_FUNC("asm/code_080F8900/unknown_thumb_080f98d8.s", void unknown_thumb_080f98d8(void))

ASM_FUNC("asm/code_080F8900/owner_init_entry.s", void owner_init_entry(void))

ASM_FUNC("asm/code_080F8900/owner_tick_records.s", u32 owner_tick_records(u8 *owner))

ASM_FUNC("asm/code_080F8900/owner_reset.s", void owner_reset(void))

ASM_FUNC("asm/code_080F8900/slot_load_object.s", void slot_load_object(void))

void slot_store_object(void *owner, u32 idx)
{
    u32 t = idx << 24;
    u8 **table = (u8 **)(*(u8 **)((u8 *)owner + 0x14) + 0x158);
    u8 *entry = *table + (t >> 18);
    u8 *node = *(u8 **)entry;

    *(u16 *)(node + 0xE) = *(u16 *)(entry + 0x12);
    {
        u32 a = entry[0xF];
        node[0x20] = a;
    }
    ((struct NodeFlags1F *)(node + 0x1F))->f0 = ((u32)entry[0x10] << 27) >> 30;
    ((struct NodeFlags1F *)(node + 0x1F))->f2 = ((u32)entry[0x10] << 25) >> 30;
    ((struct NodeFlags1F *)(node + 0x1F))->f4 = ((u32)*(u16 *)(entry + 0x10) << 23) >> 30;
    ((struct NodeFlags1F *)(node + 0x1F))->f6 = ((u32)entry[0x11] << 29) >> 30;
}

ASM_FUNC("asm/code_080F8900/record_advance.s", void record_advance(void))

ASM_FUNC("asm/code_080F8900/record_dispatch.s", void record_dispatch(void))
