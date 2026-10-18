#include "global.h"
#include "ram_routines.h"

extern u8 *gEffectState;
extern u8 *gUnk_03000FC0;
extern u8 *gWorld;
extern u8 *gWorkBuf;
extern const u8 gDefaultTaskDesc[];
extern const u8 gSlotTableTag[];
extern const u8 gObjTaskHandler[];
extern const u8 gOwnerStateTag[];
extern u32 gTextExtraFlagPtrs[];
extern u32 gTextCountPtrs[];
extern u32 gTextKindPtrs[];
extern u32 gTextModePtrs[];
extern u32 gTextHalfPtrs[];
extern u8 *gMemRoutines;

void task_insert(void *node, u8 kind, u32 data);
void task_remove(void *node, u32 kind);
u32 flag_2b5_bit3(void *obj);
u32 obj_flags_2b5_07_set(void *obj);
u32 obj_has_pending_bits(void *obj);
void reset_obj_params_b(u8 *dst, u8 *obj, u32 a, u32 b, u32 c, u32 d);
void obj_finish_move_step(void *obj);
void obj_stop_motion(void *obj);
u32 step_effect_slot(u8 *owner, u8 *arg, u8 *obj);
u32 obj_command_dispatch(u8 *owner, u8 *arg, u8 *obj, u32 *buf);
void decode_stream_run(u8 *rec, u32 *buf, u8 *block, u8 *p3, u8 *p4, u32 advanced);
void slot_apply_anim(u8 *owner, u32 kind);
void *heap_alloc_raw(u32 pool, u32 size, const u8 *tag);
u32 find_or_alloc_obj_slots(u32 a, u32 b, u32 c, u32 d, u32 e, void *f, s32 g);
void ensure_code_entry(u32 code);
void release_obj_parts(u8 *p, u32 n);
void heap_free(void *p);
void heap_free_b(void *p);
void obj_setup_load(void *obj, u32 code);
void obj_load_record(void *obj, u32 a, s32 b, u32 c, u32 d);
void sprite_entry_update(void *obj);
void cpu_set(const void *src, void *dst, u32 ctl);
void set_obj_target(u32 item, s32 a, s32 b);
void cmd_update_kind(u8 *owner, u8 *arg, u8 kind);
void cmd_part_scan(u8 *owner, u8 *node, u32 idx);
void cmd_slot_clear_flag2(u8 *owner, u8 *node, u32 idx);
void clear_node_bit9(u8 *owner, u8 *node);
void slot_clear_flag800_if_idle(u8 *owner, u8 *node);
void tick_obj_timer(void *timer, void *state);
void world_scroll_step(u8 *owner, u8 *obj);
u32 cmd_reset_obj_part(u8 *obj, u8 *state, u32 *params, u8 idx);
void alloc_code_entry(u32 code);
u32 create_obj_de4(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g);

struct ObjFlags12
{
    u8 bit0 : 1;
    u8 mode : 2;
    u8 rest : 5;
};

struct FlagBit0
{
    u8 bit0 : 1;
    u8 rest : 7;
};

static inline void set_flag_bit0(u8 *p, u32 v)
{
    ((struct FlagBit0 *)p)->bit0 = v;
}

NONMATCH("asm/code_080F2448/cmd_set_global_flag.s", u32 cmd_set_global_flag(u32 unused0, u32 unused1, u8 *cmd))
{
    u8 *flag = gEffectState + 0x55A;

    set_flag_bit0(flag, *cmd & 1);
    return 1;
}
END_NONMATCH

NONMATCH("asm/code_080F2448/cmd_stop_move.s", u32 cmd_stop_move(u8 *owner, u32 unused, u32 *cmd))
{
    u32 off = (cmd[0] << 2) + 0x28;
    u8 **slot = (u8 **)(*(u8 **)(owner + 0x14) + off);
    u8 *obj = *slot;
    u8 *block = obj + 0x58;
    u16 *flags = (u16 *)(obj + 0xF8);

    if (*flags & 2)
    {
        obj_finish_move_step(obj);
    }
    if (*flags & 0x40)
    {
        obj_stop_motion(*slot);
    }
    *flags = 0;
    *(u32 *)(obj + 0x58) = *(u32 *)(block + 4);
    *(u32 *)(block + 0x10) = 0;
    return 1;
}
END_NONMATCH

ASM_FUNC("asm/code_080F2448/cmd_find_slot.s", void cmd_find_slot(void))

NONMATCH("asm/code_080F2448/cmd_set_clear_flag4.s", u32 cmd_set_clear_flag4(u8 *owner, u32 unused, u32 *cmd))
{
    u32 index = cmd[1];
    u32 list = *(u32 *)(owner + 0x14);
    u8 *obj = *(u8 **)((index << 2) + list + 0x28);
    u32 mode = cmd[0];
    u16 v;

    if (mode == 0)
    {
        v = 4 | *(u16 *)(obj + 0xF8);
    }
    else if (mode == 1)
    {
        v = 0xFFFB & *(u16 *)(obj + 0xF8);
    }
    else
    {
        return 1;
    }
    *(u16 *)(obj + 0xF8) = v;
    return 1;
}
END_NONMATCH

u32 unknown_thumb_080f2570(u32 unused, u8 *obj)
{
    if ((obj[0x2B5] & 7) == 0)
    {
        return 0;
    }
    if (obj[0x2B6] & 0x40)
    {
        if (obj[0x24] & 0x78)
        {
            return 0;
        }
    }
    if ((obj[0x2B6] & 0x80) == 0)
    {
        return 1;
    }
    if ((obj[0x25] & 0x78) == 0)
    {
        return 1;
    }
    return 0;
}

void sub_080F25D0(u8 *a, u8 *obj)
{
    if (*(u32 *)(obj + 8) != 0 && ((obj[0xA0] ^ 1) & 1))
    {
        reset_obj_params_b(a + 0x1C, obj, *(u32 *)(obj + 8), *(u32 *)(obj + 0xC),
                           *(u16 *)(obj + 0xA2), obj[0xA5]);
    }
}

void sub_080F2610(u32 unused, u8 *obj, u8 *state)
{
    if ((u8)flag_2b5_bit3(obj) == 0)
    {
        *(u16 *)(state + 0xA0) ^= 0x40;
    }
}

void sub_080F2634(u32 unused, u8 *obj, u8 *state)
{
    if ((u8)obj_flags_2b5_07_set(obj) == 0)
    {
        *(u16 *)(state + 0xA0) ^= 2;
    }
}

void sub_080F2658(u8 *owner, u32 unused, u8 *state)
{
    if ((u8)obj_has_pending_bits(*(void **)(owner + 0x14)) == 0)
    {
        *(u16 *)(state + 0xA0) ^= 0x100;
    }
}

u32 sub_080F2680(u8 *owner, u8 *arg, u8 *obj)
{
    u32 buf[13];
    u8 res;

    if (((obj[0xA0] ^ 1) & 1) != 0)
    {
        return 0;
    }
    res = step_effect_slot(owner, arg, obj);
    while (res == 0)
    {
        decode_stream_run(obj, buf, gUnk_03000FC0, gEffectState + 0x4B0, gUnk_03000FC0 + 0x38C, res);
        if ((u8)obj_command_dispatch(owner, arg, obj, buf) == 0)
        {
            return 1;
        }
        res = step_effect_slot(owner, arg, obj);
    }
    return 1;
}

void sub_080F26F4(u8 *obj, u32 kind)
{
    *(const u8 **)(obj + 0x18) = gDefaultTaskDesc;
    task_remove(obj, kind);
}

u8 *sub_080F2708(u8 *obj, u8 kind, u32 data)
{
    task_insert(obj, kind, data);
    *(const u8 **)(obj + 0x18) = gDefaultTaskDesc;
    return obj;
}

NONMATCH("asm/code_080F2448/init_obj_slot_table.s", void *init_obj_slot_table(u8 **out))
{
    u8 *block;
    u8 *entry;
    u32 local;
    u32 none;
    s16 i;

    block = heap_alloc_raw(1, 0x40, gSlotTableTag);
    *out = block;
    ((void (*)(u32, void *, u32))gMemRoutines)(0, block, 0x40);
    find_or_alloc_obj_slots(7, 0x8001, 1, 0, 0, &local, 0xF);
    ensure_code_entry(0x8004);
    alloc_code_entry(0x8004);
    i = 4;
    none = 0xFFFFFFFF;
    entry = block;
    for (; i > 0; )
    {
        entry[0xC] = 0xFF;
        *(u32 *)entry = create_obj_de4(0, none, 1, none, none, none, none);
        i--;
        entry += 0x10;
    }
    return out;
}
END_NONMATCH

ASM_FUNC("asm/code_080F2448/cmd_spawn_obj.s", void cmd_spawn_obj(void))

u8 *owner_init_state(u8 *owner, u8 prio, u32 data)
{
    u8 *state;
    u32 size;
    u32 *slot;
    u8 *base;
    s16 i;

    task_insert(owner, prio, data);
    *(const u8 **)(owner + 0x18) = gObjTaskHandler;
    size = 0x17A0;
    state = heap_alloc_raw(1, size, gOwnerStateTag);
    *(u8 **)(owner + 0x2C) = state;
    ((void (*)(u32, void *, u32))gMemRoutines)(0, state, size);
    base = gUnk_03000FC0;
    *(u32 *)(base + 0x55C) = gTextExtraFlagPtrs[1];
    *(u32 *)(base + 0x560) = gTextCountPtrs[1];
    *(u32 *)(base + 0x564) = gTextKindPtrs[1];
    *(u32 *)(base + 0x568) = gTextModePtrs[1];
    *(u32 *)(base + 0x56C) = gTextHalfPtrs[1];
    i = 2;
    slot = (u32 *)(owner + 0x20);
    for (; i > 0; i--)
    {
        *slot = 0;
        slot++;
    }
    *(u32 *)(owner + 0x30) = 0;
    *(u32 *)(owner + 0x28) = 0;
    return owner;
}

void free_task_state(u8 *task, u32 arg)
{
    u8 *block;
    u8 *base;
    u32 *slot;
    s16 i;

    *(const u8 **)(task + 0x18) = gObjTaskHandler;
    i = 2;
    slot = (u32 *)(task + 0x20);
    for (; i > 0; i--)
    {
        if (*slot != 0)
        {
            release_obj_parts((u8 *)*slot, 3);
        }
        *slot = 0;
        slot++;
    }
    block = *(u8 **)(task + 0x28);
    if (block != 0)
    {
        heap_free(*(u8 **)block);
        heap_free_b(block);
        *(u32 *)(task + 0x28) = 0;
    }
    if (*(u8 **)(task + 0x2C) != 0)
    {
        heap_free(*(u8 **)(task + 0x2C));
        *(u32 *)(task + 0x2C) = 0;
    }
    *(u32 *)(task + 0x30) = 0;
    base = gUnk_03000FC0;
    *(u32 *)(base + 0x55C) = gTextExtraFlagPtrs[0];
    *(u32 *)(base + 0x560) = gTextCountPtrs[0];
    *(u32 *)(base + 0x564) = gTextKindPtrs[0];
    *(u32 *)(base + 0x568) = gTextModePtrs[0];
    *(u32 *)(base + 0x56C) = gTextHalfPtrs[0];
    task_remove(task, arg);
}

ASM_FUNC("asm/code_080F2448/task_update.s", void task_update(void))

NONMATCH("asm/code_080F2448/task_slot_write.s", void task_slot_write(u8 *task, u8 slot_arg, u32 arg2))
{
    u8 *slot_base;
    u8 *rec;
    u32 args[2];
    u32 val_a;
    u32 val_b;
    u8 byte_c;
    u32 byte_1a6;
    u16 count;

    slot_base = *(u8 **)(task + 0x2C) + (slot_arg << 8);
    rec = slot_base + 0x100;
    args[0] = 1;
    args[1] = arg2;
    count = *(u16 *)(slot_base + 0x1A0);
    if (count != 0)
    {
        if (*(u32 *)(rec + 8) == 0)
        {
            val_a = *(u32 *)rec;
            val_b = *(u32 *)(rec + 4);
            byte_c = slot_base[0x1A4];
            *(u32 *)(slot_base + 0x1AC) = *(u32 *)(rec + 0x10);
            *(u32 *)(slot_base + 0x1B0) = *(u32 *)(rec + 0x14);
            *(u32 *)(slot_base + 0x1F4) = *(u32 *)(slot_base + 0x198);
            *(u32 *)(slot_base + 0x1F8) = *(u32 *)(slot_base + 0x19C);
            byte_1a6 = slot_base[0x1A6];
            slot_base[0x1FC] = byte_1a6;
            cpu_set(slot_base + 0x158, slot_base + 0x1B4, 0x20);
        }
        else
        {
            val_a = *(u32 *)(rec + 8);
            val_b = *(u32 *)(rec + 0xC);
            byte_c = slot_base[0x1A5];
            count = *(u16 *)(slot_base + 0x1A2);
        }
        *(u16 *)(rec + 0xA0) = 0;
        cmd_reset_obj_part(task, *(u8 **)(task + 0x2C), args, slot_arg);
        *(u32 *)(rec + 8) = val_a;
        *(u32 *)(rec + 0xC) = val_b;
        *(u16 *)(rec + 0xA2) = count;
        rec[0xA5] = byte_c;
    }
    else
    {
        cmd_reset_obj_part(task, *(u8 **)(task + 0x2C), args, slot_arg);
    }
}
END_NONMATCH

ASM_FUNC("asm/code_080F2448/task_tick_objects.s", void task_tick_objects(void))

ASM_FUNC("asm/code_080F2448/cmd_update_kind.s", void cmd_update_kind(u8 *owner, u8 *arg, u8 kind))

ASM_FUNC("asm/code_080F2448/world_scroll_step.s", void world_scroll_step(u8 *owner, u8 *obj))

ASM_FUNC("asm/code_080F2448/world_target_motion.s", void world_target_motion(void))

ASM_FUNC("asm/code_080F2448/node_step.s", void node_step(void))

ASM_FUNC("asm/code_080F2448/run_node_command.s", void run_node_command(void))
